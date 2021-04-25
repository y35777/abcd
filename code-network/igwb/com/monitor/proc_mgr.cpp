/***************************************************************************
 *产品名   ：iGateway Bill V200
 *模块名   ：monitor
 *文件名   ：proc_mgr.h
 *描述信息 ：类CProcessManager的实现。
             CProcessManager利用ACE提供的ACE_ProcessManager类和ACE_Process_Options
             类来实现进程管理功能，其中ACE_Process_Options可以向进程传入命令行参数
             ，ACE_Process_Manager封装了WIN32平台下的CreateProcess的功能以及UNIX下
             的fork和exec的功能。
 *版本说明 ：V200R001i00M00B00D1130A
 *版权信息 ：版权所有（C）2001-2002 华为技术有限公司
 *作者     ：邱意平
 *创建时间 ：2001-10-30
 *修改记录 ：
*****************************************************************************/

#include "proc_mgr.h"
#include "proc_child.h"
#include "monitor.h"
#include "../utility/mml_report.h"
#include "../include/resource.h"
#include "ace/High_Res_Timer.h"

/****************************************************************************
函数原型	public: CProcessManager()
功能描述	构造函数
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
*****************************************************************************/
CProcessManager:: CProcessManager()
{
    m_nStatus        = PROCMGR_STOPPED;
    m_bExitFlag      = FALSE;
    m_uInitTimeOut   = 0;
    m_uActiveTimeOut = 0; 
    m_nAuxUpdStatus  = AUXUPG_STOPPED;
}

/****************************************************************************
函数原型	public: ~CProcessManager()
功能描述	析构函数
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
*****************************************************************************/
CProcessManager:: ~CProcessManager()
{
    LISTPROCCHILD::iterator it;

    //清空子进程列表
    for (it = m_ProcChild.begin(); it != m_ProcChild.end(); it++)
    {
       if (NULL != *it) 
       {
           delete *it;
       }
    }
    
    m_ProcChild.clear();
}

/******************************************************************************
函数原型	public: int InitInstance()
功能描述	初始化函数
输入参数	无。
输出参数	无。
返回值  	如果初始化过程正常，则返回ERR_SUCCESS，否则返回ERR_FAIL。
抛出异常	无。
*******************************************************************************/
int CProcessManager:: InitInstance()
{
    //创建各个进程
    int nRet = CreateAllProcessChild();
    
    return nRet;
}

/*****************************************************************************
函数原型	protected: int CreateAllProcessChild()
功能描述	创建所有业务进程对象
输入参数	无。
输出参数	无。
返回值  	创建所有业务进程对象成功，则返回ERR_SUCCESS，否则返回ERR_FAIL。
抛出异常	无。
******************************************************************************/
int CProcessManager:: CreateAllProcessChild()
{
    SMappedAppInfo *pInfo = NULL;
    CProcessChild *pChild = NULL;

    //打开配置文件，读监控模块使用的参数
    CINIFile *pIniFile = new CINIFile(GetCfgFilePath());
    if (!pIniFile->Open())
    {
        delete pIniFile;
        return ERR_OPEN_CFG_FILE;
    }

    //从配置文件中读取接入点数
    int nAPCount = pIniFile->GetInt(CFG_SEC_COMMON,
                                    CFG_COMMON_KEY_APCOUNT, 
                                    CFG_COMMON_KEY_APCOUNT_DEFAULT);

    //如果接入点数为零，或未配接入点数
    if (0 > nAPCount)
    {
        pIniFile->Close();
        delete pIniFile;

        return ERR_FAIL;
    }

    //添加OM(操作维护)进程
    pInfo = new SMappedAppInfo;
    pInfo->bCanRestart = TRUE;       //可以重启
    pInfo->bStart      = TRUE;       //需要启动该进程
    pInfo->nPType      = PT_OM;      //进程类型

    //设置OM进程名标识
    strcpy(pInfo->szAppName, S_APPNAME_OM);

    //在WIN32平台下，可执行文件名要有".exe"后缀
    #ifdef _PLATFORM_WIN32
        strcat(pInfo->szAppName, ".exe");
    #endif 
 
    //设置OM进程的全路径名
    strcpy(pInfo->szPathName, GetAppPath()); 
    strcat(pInfo->szPathName, ACE_DIRECTORY_SEPARATOR_STR); 
    strcat(pInfo->szPathName, S_APPNAME_OM); 
    
    //在WIN32平台下，可执行文件名要有".exe"后缀
    #ifdef _PLATFORM_WIN32
        strcat(pInfo->szPathName, ".exe");
    #endif 

    //创建子进程并传入子进程需要的运行参数
    pChild = new CProcessChild(pInfo);
    //使OM进程成为基础进程
    pChild->EnableBaseProc();

    if (Add(pChild) != ERR_SUCCESS)
    {
        delete pChild;
        
        pIniFile->Close();
        delete pIniFile;

        return ERR_FAIL;
    }

    //添加接入点进程，至少应该有一个接入点进程
    int i = 0;
    MAP<STRING,int> map_ap_name;
    char szSec[20];
    char szAPName[50];
    for (i = 0; i < nAPCount; i++)
    {
        sprintf(szSec, "%s%d", CFG_SEC_AP, i + 1);
        int nCollectType = pIniFile->GetInt(szSec,
                                            CFG_AP_KEY_COLTYPE,
                                            CFG_AP_KEY_COLTYPE_DEFAULT);
        pIniFile->GetString(szSec,
                            CFG_AP_NAME,
                            "",
                            szAPName,
                            sizeof(szAPName));

        if(map_ap_name.find(szAPName) != map_ap_name.end())
        {
            pIniFile->Close();
            delete pIniFile;

            TRACE(MTS_MONITOR, 
                  S_AP_NAME_DUP, 
                  i + 1, 
                  map_ap_name[szAPName]);
            return ERR_FAIL;
        }

        map_ap_name[szAPName] = i + 1;
        pInfo = new SMappedAppInfo;
        pInfo->bCanRestart = TRUE;
        pInfo->bStart      = TRUE;
        
        //接入点进程ID需要增加一个偏移量
        if(nCollectType > 0)
        {
            pInfo->nPType = (PROCESS_TYPE)(PT_CO_BASE + i);
            strcpy(pInfo->szAppName, S_APPNAME_CO);
        }
        else
        {
            pInfo->nPType = (PROCESS_TYPE)(PT_AP_BASE + i);
            strcpy(pInfo->szAppName, S_APPNAME_AP);
        }

        //在WIN32平台下，可执行文件名要有".exe"后缀
        #ifdef _PLATFORM_WIN32
            strcat(pInfo->szAppName, ".exe");
        #endif 

        strcpy(pInfo->szPathName, GetAppPath()); 
        strcat(pInfo->szPathName, ACE_DIRECTORY_SEPARATOR_STR); 
        strcat(pInfo->szPathName, pInfo->szAppName); 

        pChild = new CProcessChild(pInfo);

        if (Add(pChild) != ERR_SUCCESS)
        {
            delete pChild;

            pIniFile->Close();
            delete pIniFile;

            return ERR_FAIL;
        }
    }

    //读出需要额外启动的进程信息
    int nAddProcNum = pIniFile->GetInt(CFG_SEC_ADDPROC, 
                                       CFG_ADDPROC_SEC_PROCNUM, 
                                       0);
    //添加其它需要额外启动的进程，本接口暂时不使用，但为以后增加
    //辅助小程序提供了接口
    char pszBuf[MAX_PATH];
    for (i = 1; i <= nAddProcNum; i++)
    {
        pInfo = new SMappedAppInfo;
        pInfo->bCanRestart = TRUE;
        pInfo->bStart      = TRUE;
       
        //读进程名
        char szNum[10];
        sprintf(szNum, "%d", i);
        pszBuf[0] = '\0';

        pIniFile->GetString(CFG_SEC_ADDPROC, szNum, "", pszBuf, MAX_PATH);

        if (strlen(pszBuf) == 0)
        {
            delete pInfo;
            pIniFile->Close();
            delete pIniFile;
            return ERR_FAIL;
         }
          
         strcpy(pInfo->szAppName, pszBuf);

         //读进程编号
         pInfo->nPType = (PROCESS_TYPE)(pIniFile->GetInt(pInfo->szAppName, 
                                                         CFG_ADDPROC_SEC_PTYPE, 
                                                         PT_UNKNOWN));
         //读路径名
         pszBuf[0] = '\0';

         pIniFile->GetString(pInfo->szAppName, CFG_ADDPROC_SEC_APPNAME, 
                             "", pszBuf, MAX_PATH);
         if (strlen(pszBuf) == 0)
         {
             delete pInfo;
             pIniFile->Close();
             delete pIniFile;

             return ERR_FAIL;
         }
        
         strcpy(pInfo->szPathName, GetAppPath()); 
         strcat(pInfo->szPathName, ACE_DIRECTORY_SEPARATOR_STR); 
         strcat(pInfo->szPathName, pszBuf); 

         //读命令行参数
         pIniFile->GetString(pInfo->szAppName, CFG_ADDPROC_SEC_CMDPARA, 
                             "", pszBuf, MAX_PATH);
         if (strlen(pszBuf) == 0) 
         {
             delete pInfo;
             pIniFile->Close();
             delete pIniFile;

             return ERR_FAIL;
         }
         
//         strcat(pInfo->szPathName, " "); 
//         strcat(pInfo->szPathName, pszBuf); 

         //创建并添加子进程
         pChild = new CProcessChild(pInfo);
         if (Add(pChild) != ERR_SUCCESS)
         {
             delete pChild;
             pIniFile->Close();
             delete pIniFile;

             return ERR_FAIL;
         }
    }

    pIniFile->Close();
    delete pIniFile;

    return ERR_SUCCESS;
}

/***********************************************************************************
函数原型	protected: int Add(CProcessChild *pChild)
功能描述	将该业务进程指针添加到业务进程指针list中
输入参数	pChild: 待添加的子进程对象指针。
输出参数	无。
返回值  	将该业务进程指针添加到业务进程指针LIST中是否成功，如果成功，则返回
                ERR_SUCCESS，否则返回ERR_FAIL。
抛出异常	无。
************************************************************************************/
int CProcessManager:: Add(CProcessChild* pChild)
{
    m_ProcChild.push_back(pChild);

    return ERR_SUCCESS; 
}

/********************************************************************************
函数原型	public: void ProcInitAns(PROCESS_TYPE nPType)
功能描述	处理由各业务进程中管理模块发送来的初始化应答。
输入参数	返回初始化应答的业务进程编号。
输出参数	无。
返回值  	无。
抛出异常	无。
*********************************************************************************/
void CProcessManager:: ProcInitAns(PROCESS_TYPE nPType)
{
    if ( (g_lDebugVersion == 0 ) && (GetStatus(nPType) != APP_START_PENDING) )
    {
        return;
    }
	
    //如果在调试状态或者管理模块已经激活，则每收到一条激活应答，
    //就激活一次该子进程
    if (g_lDebugVersion || (PROCMGR_ACTIVE == m_nStatus) || (PROCMGR_ACTIVING == m_nStatus))
    {
        //根据进程ID查找进程
        CProcessChild *pChild = LookUp(nPType);
		
        //未找到，函数返回
        if (NULL == pChild)
        {
            return;
        }
        
        SetStatus(nPType, APP_ACTIVING);
		pChild->Active();
        if (PROCMGR_ACTIVE != m_nStatus)
        {
            m_nStatus = PROCMGR_ACTIVING;
        }
    }
    else 
    {
        SetStatus(nPType, APP_READY);
    }
}

/*******************************************************************************
函数原型	public: void ProcActiveAns(PROCESS_TYPE nPType)
功能描述	处理由各个业务进程发送来的激活应答。
输入参数	返回激活应答的业务进程编号。
输出参数	无。
返回值  	无。
抛出异常	无。
********************************************************************************/
void CProcessManager:: ProcActiveAns(PROCESS_TYPE nPType)
{
    if ( ( g_lDebugVersion == 0 ) && ( GetStatus(nPType) != APP_ACTIVING) )
    {
        return;
    }

    SetStatus(nPType, APP_ACTIVE);

    if (g_lDebugVersion)
    {
        m_nStatus = PROCMGR_ACTIVE;
    }
}

/*******************************************************************************
函数原型	protected: void ProcFailedRecovery(PROCESS_TYPE nPType)
功能描述	进行业务进程本地恢复失败的处理。
输入参数	进行本地恢复失败的业务进程编号。
输出参数	无。
返回值  	无。
抛出异常	无。
********************************************************************************/
void CProcessManager:: ProcFailedRecovery(PROCESS_TYPE nPType)
{
    CProcessChild *pChild = NULL;
    
    pChild  = LookUp(nPType);
	
    if (NULL == pChild)
    {
        return;
    }

    if (pChild->CanRestart())
    {
        Stop(nPType);
    }
    else
    {
        m_nStatus = PROCMGR_FAILURE;
        m_bExitFlag = TRUE;
    }
}

/*********************************************************************************
函数原型	protected: CProcessChild *LookUp(PROCESS_TYPE nPType)
功能描述	通过输入的业务进程编号查询该业务进程对象的指针。
输入参数	业务进程编号。
输出参数	无。
返回值  	返回指定业务进程编号的进程对象指针。
抛出异常	无。
**********************************************************************************/
CProcessChild* CProcessManager:: LookUp(PROCESS_TYPE nPType)
{
    LISTPROCCHILD::iterator it;

    for (it = m_ProcChild.begin(); it != m_ProcChild.end(); it++)
    {
        if ((*it)->GetProcessType() == nPType)
        {
            return (*it);
        }
   }
   
   return NULL;
}

/**********************************************************************************
函数原型	protected: CProcessChild *LookUp(pid_t nPid)
功能描述	通过输入的进程ID号查询该业务进程对象的指针。
输入参数	业务进程ID号。
输出参数	无。
返回值  	返回指定业务进程ID号的进程对象指针。
抛出异常	无。
***********************************************************************************/
CProcessChild* CProcessManager::LookUp(pid_t nPid)
{
    LISTPROCCHILD::iterator it;

    for (it = m_ProcChild.begin(); it != m_ProcChild.end(); it++)
    {
        if ((*it)->GetPid() == nPid)
        {
            return (*it);
        }
    }
    
    return NULL;
}

/***********************************************************************************
函数原型	protected: int Start(PROCESS_TYPE nPType)
功能描述	启动指定的业务进程。
输入参数	业务进程编号。
输出参数	无。
返回值  	返回启动指定的业务进程是否成功，如果启动成功，则返回ERR_SUCCESS，
                否则返回非零值。
抛出异常	无。
************************************************************************************/
int CProcessManager:: Start(PROCESS_TYPE nPType)
{
    CProcessChild *pChild = LookUp(nPType);
	
    if (NULL == pChild) 
    {
        return ERR_FAIL;
    }
    
    return pChild->Start(&m_ProcMgr);
}

/**********************************************************************************
函数原型	public: int Start()
功能描述	启动所有业务进程。
输入参数	无。
输出参数	无。
返回值  	返回启动所有业务进程是否成功。
抛出异常	无。
***********************************************************************************/
int CProcessManager:: Start()
{
    int nRet = ERR_SUCCESS;
    LISTPROCCHILD::iterator it;

    //依次启动各个子进程
    for (it = m_ProcChild.begin(); it != m_ProcChild.end(); it++)
    {
        nRet = Start((*it)->GetProcessType());

        if (ERR_SUCCESS != nRet)
        {
            TRACE(MTS_MONITOR, SZ_MSG_STARTPROCESS_FAIL, (*it)->GetAppName());
            break;
        }
    }

    if (ERR_SUCCESS == nRet)
    {
        m_uInitTimeOut = 0;    //初始化超时时间归零
        m_nStatus = PROCMGR_START_PENDING;
        
        TRACE(MTS_MONITOR, SZ_MSG_ALL_PROCESS_OK);
        StartWatchDog();
    }
    else
    {
        Stop();   //如果启动任何一个子进程失败，则关闭所有已经启动的子进程
        m_nStatus = PROCMGR_FAILURE;
    }
    
    return nRet;
}

/****************************************************************************************
函数原型	protected: void Stop(PROCESS_TYPE nPType)
功能描述	强行关闭指定的业务进程。
输入参数	业务进程编号。
输出参数	无。
返回值  	无。
抛出异常	无。
*****************************************************************************************/
void CProcessManager:: Stop(PROCESS_TYPE nPType)
{
    CProcessChild *pChild = LookUp(nPType);
	
    if (NULL == pChild)
    {
        return;
    }

    pChild->Stop();
}

/****************************************************************************************
函数原型	protected: void Stop()
功能描述	关闭所有的业务进程。
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
*****************************************************************************************/
void CProcessManager:: Stop()
{
    //置守护线程退出标志
    m_bExitFlag = TRUE;
    m_nStatus = PROCMGR_STOP_PENDING;

    //等待守护线程退出
    ACE_Time_Value timeout = ACE_OS::gettimeofday();
    timeout += (ACE_Time_Value)20;

    m_ThdMgr.wait(&timeout);

    //置所有业务进程对象的状态为APP_STOP_PENDING
    //向所有业务进程发关闭消息
    LISTPROCCHILD::iterator it;
    BOOL bSendQuitToAP = FALSE;
    for (it = m_ProcChild.begin(); it != m_ProcChild.end(); it++)
    {
        if((*it)->GetbBaseProc() || APP_STOPPED == (*it)->GetStatus())
        {
            continue;
        }
        (*it)->SetStatus(APP_STOP_PENDING);
        CMonitor::SendMsgToManager((*it)->GetProcessType(), CMD_QUIT);
        bSendQuitToAP = TRUE;
    }

    int nSecRemain = ONE_MINUTE_IN_SECOND;
    if(bSendQuitToAP)
    {
        for (it = m_ProcChild.begin(); it != m_ProcChild.end(); it++)
        {
            if((*it)->GetbBaseProc())
            {
                continue;
            }

            if(nSecRemain > 0)
            {
                nSecRemain = WaitPid((*it)->GetPid(), nSecRemain);
            }
            else
            {
                break;
            }
        }
    }

    for (it = m_ProcChild.begin(); it != m_ProcChild.end(); it++)
    {
        if((*it)->GetbBaseProc())
        {
            (*it)->SetStatus(APP_STOP_PENDING);
            CMonitor::SendMsgToManager((*it)->GetProcessType(), CMD_QUIT);
        }
    }

    //等待所有业务进程退出
    if(Wait(TIME_OF_PROC_DEACTIVE - ONE_MINUTE_IN_SECOND))
    {
        for (it = m_ProcChild.begin(); it != m_ProcChild.end(); it++)
        {
            (*it)->Stop();
        }
    }

    m_nStatus = PROCMGR_STOPPED;
}


/***************************************************************
函数原型	public: int WaitExitPid(UINT4 uSec)
功能描述	等待所有子进程直到超时，返回退出的进程PID
输入参数	超时时间,单位：s。
输出参数	无。
返回值  	>0---当前退出的进程PID
            ==0--超时。
抛出异常	无。
***************************************************************/
pid_t CProcessManager::WaitExitPid(UINT4 uSec)
{
#ifdef _PLATFORM_WIN32

    return m_ProcMgr.wait(0, ACE_Time_Value(uSec));

#else

    int sig;
    pid_t nRetPid = m_ProcMgr.wait(0, ACE_Time_Value::zero);
    if(nRetPid) return nRetPid;

    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGALRM);
    sigaddset(&sigset, SIGCHLD);
    alarm(uSec);
    sigwait(&sigset, &sig);
    return m_ProcMgr.wait(0, ACE_Time_Value::zero);

#endif
}


/******************************************************
// 函数名: 	CProcessManager::Wait
// 作者:    Wangfeng
// 时期:    01-11-22
// 描述:    等待所有子进程直到超时，返回当前未退出的进程数
// 输入:
//       参数1 :UINT4 uSec
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-11-22      Wangfeng
******************************************************/
int CProcessManager::Wait(UINT4 uSec)
{
#ifdef _PLATFORM_WIN32

    return m_ProcMgr.wait(ACE_Time_Value(uSec));

#else

    int sig;
    int nRemainProc = 0;
    long lSecRemain = uSec;
    time_t tBegin;
    time(&tBegin);

    while((nRemainProc = m_ProcMgr.wait(ACE_Time_Value::zero)) && 
          (lSecRemain > 0))
    {
        sigset_t sigset;
        sigemptyset(&sigset);
        sigaddset(&sigset, SIGALRM);
        sigaddset(&sigset, SIGCHLD);
        alarm(lSecRemain);
        sigwait(&sigset, &sig);

        time_t tNow;
        time(&tNow);
        lSecRemain  = tNow - tBegin - uSec;
    }
    return nRemainProc;

#endif
}

int CProcessManager::WaitPid(pid_t nPID, int nSec)
{
#ifdef _PLATFORM_WIN32
    int nSecRemain = nSec;
    time_t tBegin;
    time(&tBegin);

    pid_t nRetPID = m_ProcMgr.wait(nPID, ACE_Time_Value(nSecRemain));
    
    time_t tNow;
    time(&tNow);
    nSecRemain  = tNow - tBegin - nSec;

    if(nSecRemain < 0)
    {
        nSecRemain = 0;
    }

    return nSecRemain;

#else

    int sig;
    int nRemainProc = 0;
    int nSecRemain = nSec;
    time_t tBegin;
    time(&tBegin);

    while((nRemainProc != m_ProcMgr.wait(nPID, ACE_Time_Value::zero)) && 
          (nSecRemain > 0))
    {
        sigset_t sigset;
        sigemptyset(&sigset);
        sigaddset(&sigset, SIGALRM);
        sigaddset(&sigset, SIGCHLD);
        alarm(nSecRemain);
        sigwait(&sigset, &sig);

        time_t tNow;
        time(&tNow);
        nSecRemain  = tNow - tBegin - nSec;
    }

    if(nSecRemain < 0)
    {
        nSecRemain = 0;
    }

    return nSecRemain;

#endif
}

/*****************************************************************************************
函数原型	public: int Active()
功能描述	激活所有业务进程，如果发起激活操作成功，则返回ERR_SUCCESS，否则返回ERR_FAIL。
输入参数	无。
输出参数	无。
返回值  	返回激活所有业务进程是否成功。
抛出异常	无。
******************************************************************************************/
int CProcessManager:: Active()
{
    int nRet = ERR_SUCCESS;
    LISTPROCCHILD::iterator it;
    
    for (it = m_ProcChild.begin(); it != m_ProcChild.end(); it++)
    {
        nRet = (*it)->Active();
        if (ERR_SUCCESS != nRet)
        {
            break;
        }
    }
    
    if (ERR_SUCCESS == nRet)
    {
        m_uActiveTimeOut = 0;
        m_nStatus = PROCMGR_ACTIVING;
        
    }
    else
    {
        Stop();
        m_nStatus = PROCMGR_FAILURE;
    }
    
    return nRet;
}

/****************************************************************************************
函数原型	public: void DeActive()
功能描述	去激活所有业务进程。
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
*****************************************************************************************/
void CProcessManager:: DeActive()
{
    Stop();
}

/****************************************************************************************
函数原型	protected: int StartWatchDog()
功能描述	启动监控业务进程的守护线程函数。
输入参数	无。
输出参数	无。
返回值  	返回启动监控业务进程的守护线程是否成功。
抛出异常	无。
*****************************************************************************************/
int CProcessManager:: StartWatchDog()
{
    int nRet = m_ThdMgr.spawn((ACE_THR_FUNC)ProcWatchDog, (void *)this);
    if (-1 == nRet)
    {
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

/*****************************************************************************************
函数原型	protected: static void ProcWatchDog(CProcessManager* pMgr)
功能描述	监控业务进程的守护线程函数。
输入参数	CProcessManager 对象的指针。
输出参数	无。
返回值  	无。
抛出异常	无。
******************************************************************************************/
void CProcessManager:: ProcWatchDog(CProcessManager* pMgr)
{
    pMgr->WatchDog();
}

/***************************************************************************************
函数原型	protected: void WatchDog()
功能描述	监控业务进程中监控其它业务进程的函数。
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
****************************************************************************************/
void CProcessManager:: WatchDog()
{
    while (!m_bExitFlag)
    {
        pid_t nRetPid = WaitExitPid(5);
        if ((nRetPid > 0) && (!m_bExitFlag))
        {
            CProcessChild *pChild = LookUp(nRetPid);
            if (NULL == pChild) 
            {
                continue;
            }

            PROCESS_TYPE pType = pChild->GetProcessType();
            int nRet = ProcRecovery(pType);
            if (ERR_SUCCESS != nRet)
            {
                TRACE(MTS_MONITOR, SZ_MSG_EXECPT_OVER, pChild->GetAppName());
                MSGOUT(MTS_MONITOR, MSGOUT_LEVEL_IMPORTANT, SZ_MSG_EXECPT_OVER, 
                       pChild->GetAppName());

                ProcFailedRecovery(pType);
            }
        }
    }

    TRACE(MTS_MONITOR, SZ_MSG_WATCHDOG_EXIT);
}

/********************************************************************************************
函数原型	protected: int ProcRecovery(PROCESS_TYPE nPType)
功能描述	对某业务进程进行本地恢复操作。
输入参数	业务进程编号。
输出参数	无。
返回值  	如果发起本地恢复成功，则返回ERR_SUCCESS ，否则返回ERR_FAIL。
抛出异常	无。
*********************************************************************************************/
int CProcessManager:: ProcRecovery(PROCESS_TYPE nPType)
{
    CProcessChild *pChild = LookUp(nPType);

    if (NULL == pChild)
    {
        return ERR_FAIL;
    }

    if (!pChild->CanRestart())
    {
        return ERR_FAIL;
    }

    if(m_nStatus == PROCMGR_ACTIVE)
    {
        if(pChild->GetStatus() == APP_SCHEDULED_START)
        {
            return ERR_SUCCESS;
        }
        else
        {
            SetStatus(nPType, APP_STOPPED);
        }
    }
   
   return Start(nPType);
}

/********************************************************************************************
函数原型	public: void Timer5Sec()
功能描述	5秒定时器处理。
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
*********************************************************************************************/
void CProcessManager::Timer5Sec()
{
    LISTPROCCHILD::iterator it;
    BOOL bInitFinished   = TRUE;
    BOOL bActiveFinished = TRUE;
    BOOL bAuxUpdFinished = TRUE;

    switch (m_nStatus)
    {
        case PROCMGR_START_PENDING:
            for (it = m_ProcChild.begin(); it != m_ProcChild.end(); it++)
            {
                if ((*it)->GetStatus() != APP_READY)
                {
                    if (m_uInitTimeOut >= TIME_OF_PROC_INIT)
                    {
                        (*it)->Stop();
                        m_nStatus = PROCMGR_FAILURE;
                        
                        return;
                    }
                    else
                    {
                         bInitFinished = FALSE;

                         break;
                    }
                }
            }
             
            if (bInitFinished)
            {
                m_nStatus = PROCMGR_READY;
            }
            else
            {
                m_uInitTimeOut += 5;
            }
             
            break;

        case PROCMGR_ACTIVING:
            for (it = m_ProcChild.begin(); it != m_ProcChild.end(); it++)
            {
                if ((*it)->GetStatus() != APP_ACTIVE)
                {
                    if (m_uActiveTimeOut >= TIME_OF_PROC_ACTIVE)
                    {
                        (*it)->Stop();
                        m_nStatus = PROCMGR_FAILURE;
                        return;
                    }
                    else
                    {
                        bActiveFinished = FALSE;
                        break;
                    }
                }
            }
            
            if (bActiveFinished)
            {
                m_nStatus = PROCMGR_ACTIVE;
            }
            else
            {
                m_uActiveTimeOut += 5;
            }
             
            break;

        case PROCMGR_ACTIVE:
            for (it = m_ProcChild.begin(); it != m_ProcChild.end(); it++)
            {
                time_t tLastTime = (*it)->GetLastStartTime();
                time_t tCurTime;
                time(&tCurTime);
                switch ((*it)->GetStatus())
                {
                    case APP_START_PENDING:
                        if (tCurTime - tLastTime > TIME_OF_PROC_INIT)
                        {
                            ProcFailedRecovery((*it)->GetProcessType());
                        }
     
                        break;

                    case APP_ACTIVING:
                        if (tCurTime - tLastTime > TIME_OF_PROC_INIT + 
                                                   TIME_OF_PROC_ACTIVE)
                        {
                            ProcFailedRecovery((*it)->GetProcessType());
                        }
     
                        break;
						
                    case APP_SCHEDULED_START:
                        if ((*it)->GetLeftStartDelay(5) <= 0)
                        {
                            (*it)->Start(&m_ProcMgr);
                        }
						
                        break;
						
                    default:
                        break;
                }
            }

            if (m_nAuxUpdStatus == AUXUPG_START)
            {
                for (it = m_ProcChild.begin(); it != m_ProcChild.end(); it++)
                {
                    if ((*it)->GetProcessType() >=  PT_AP_BASE)
                    {
                        if((*it)->GetAuxUpgStatus() != AUXUPG_COMPLETED)
                        {
                            bAuxUpdFinished = FALSE;
                            break;
                        }
                    }
                }

                if (bAuxUpdFinished) 
                {
                    m_nAuxUpdStatus = AUXUPG_COMPLETED;
                    MSGOUT(MTS_MONITOR, MSGOUT_LEVEL_IMPORTANT, 
                           SZ_AUP_IS_COMPLETE);
                    CMonitor::WriteLog(MTS_MONITOR, SZ_AUP_IS_COMPLETE);
                    
                    // 张玉新 2002-03-13 add 问题单D11733
                    CMMLReport Report(MTS_MONITOR);
                    Report.MakeReportHead();            //构造报文头
                    //构造命令回显
                    Report.MakeCmdDisplay("AUP:", 0, ERR_SUCCESS, MTS_MONITOR);
                    //构造返回报文正文
                    Report.AddString(SZ_AUP_IS_COMPLETE);
                    //增加新行
                    Report.AddString(STR_NEWLINE);
                    //构造报文尾 
                    Report.MakeReportTail();              
                    const char* pChar = Report.GetReport();
                    int nLen = strlen(pChar); 
                    
                    //向MML客户端应答辅助升级命令
                    MSG_INTRA* pAns = new(nLen + 1)MSG_INTRA;
                    strcpy((char *)(pAns->Data), (const char *)pChar);
                    pAns->SenderPid     = CMsgProcessor::GetProcessType();
                    pAns->SenderPid     = MT_MONITOR;
                    pAns->ReceiverPid   = PT_OM; 
                    pAns->ReceiverMid   = MT_MMLDEBUG;
                    pAns->AppType       = APP_MML_NORM;
                    pAns->ClientNo      = 0;
                    pAns->IPAddr        = 0; 
                    CMsgProcessor::SendMsg(pAns);    //发送消息
                    //add over for D11733
                }
            }
            break;

        default:
            break;
    }
}

/************************************************************************************************
函数原型	protected: EAPPSTATUS GetStatus(PROCESS_TYPE nPType)
功能描述	获得指定业务进程当前的状态。
输入参数	业务进程编号。
输出参数	无。
返回值  	指定业务进程当前的状态。
抛出异常	无。
*************************************************************************************************/
EAPPSTATUS CProcessManager:: GetStatus(PROCESS_TYPE nPType)
{
    CProcessChild *pChild = LookUp(nPType);
    if (NULL == pChild)
    {
        return (EAPPSTATUS)-1;
    }
    
    return pChild->GetStatus();
}

/************************************************************************************************
函数原型	public: EPROCMGRSTATUS  GetStatus()
功能描述	获得业务进程管理者对象当前的状态。
输入参数	无。
输出参数	无。
返回值  	返回业务进程管理者对象当前的状态。
抛出异常	无。
*************************************************************************************************/
EPROCMGRSTATUS CProcessManager:: GetStatus()
{
    return m_nStatus;
}

/****************************************************************************************
函数原型	protected: int SetStatus(PROCESS_TYPE nPType, EAPPSTATUS nStatus)
功能描述	设置指定业务进程当前的状态。
输入参数	nPType---业务进程编号。 nStatus---业务进程状态值
输出参数	无。
返回值  	设置指定业务进程当前的状态是否成功。
抛出异常	无。
*****************************************************************************************/
int CProcessManager:: SetStatus(PROCESS_TYPE nPType, EAPPSTATUS nStatus)
{
    CProcessChild *pChild = LookUp(nPType);

    if (NULL == pChild)
    {
        return ERR_FAIL;
    }

    pChild->SetStatus(nStatus);
   
    return ERR_SUCCESS;
}

/****************************************************************************************
函数原型	public: void  ProcAuxUpgradeCmd()
功能描述	处理MML模块的辅助升级命令。
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
*****************************************************************************************/
void CProcessManager:: ProcAuxUpgradeCmd()
{
    LISTPROCCHILD::iterator it;
	
    for (it = m_ProcChild.begin(); it != m_ProcChild.end(); it++)
    {
        CMonitor::SendMsgToManager((*it)->GetProcessType(), CMD_AUXUPGRADE);
        (*it)->SetAuxUpgStatus(AUXUPG_START);
    }

    m_nAuxUpdStatus = AUXUPG_START;
}

/**************************立即生成话单************************************************/
void CProcessManager::ProduceCDR()
{
    LISTPROCCHILD::iterator it;
    
    for (it = m_ProcChild.begin(); it != m_ProcChild.end(); it++)
    {
        CMonitor::SendMsgToManager((*it)->GetProcessType(), MML_PRODUCE_CDR);
    }
}

/****************************************************************************************
函数原型	public: void ProcAuxUpgradeAns(PROCESS_TYPE nPType)
功能描述	处理接入点进程发来的辅助升级应答。
输入参数	发辅助升级应答的接入点进程号。
输出参数	无。
返回值  	无。
抛出异常	无。
*****************************************************************************************/
void CProcessManager:: ProcAuxUpgradeAns(PROCESS_TYPE nPType)
{
    CProcessChild *pChild = LookUp(nPType);
	
    if (NULL == pChild)
    {
        return;
    }
	
    pChild->SetAuxUpgStatus(AUXUPG_COMPLETED);
}

/****************************************************************************************
函数原型	public: EAUXUPGSTATUS  GetAuxUpgStatus()
功能描述	获得业务进程管理者对象的辅助升级状态。
输入参数	无。
输出参数	无。
返回值  	业务进程管理者对象的辅助升级状态。
抛出异常	无。
*****************************************************************************************/
EAUXUPGSTATUS CProcessManager:: GetAuxUpgStatus()
{
    return m_nAuxUpdStatus;
}

/****************************************************************************************
函数原型	public: void  AuxUpgFailSetStatus()
功能描述	辅助升级失败，把辅助升级状态回置为AUXUPG_STOPPED。
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
*****************************************************************************************/
//by ldf 2002.4.8 对应问题单 D12433
void CProcessManager:: AuxUpgFailSetStatus()
{
    LISTPROCCHILD::iterator it;
	
    for (it = m_ProcChild.begin(); it != m_ProcChild.end(); it++)
    {
        (*it)->SetAuxUpgStatus(AUXUPG_STOPPED);
    }

    m_nAuxUpdStatus = AUXUPG_STOPPED;
}
//修改完毕

