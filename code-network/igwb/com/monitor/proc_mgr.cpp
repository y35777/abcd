/***************************************************************************
 *��Ʒ��   ��iGateway Bill V200
 *ģ����   ��monitor
 *�ļ���   ��proc_mgr.h
 *������Ϣ ����CProcessManager��ʵ�֡�
             CProcessManager����ACE�ṩ��ACE_ProcessManager���ACE_Process_Options
             ����ʵ�ֽ��̹����ܣ�����ACE_Process_Options��������̴��������в���
             ��ACE_Process_Manager��װ��WIN32ƽ̨�µ�CreateProcess�Ĺ����Լ�UNIX��
             ��fork��exec�Ĺ��ܡ�
 *�汾˵�� ��V200R001i00M00B00D1130A
 *��Ȩ��Ϣ ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����     ������ƽ
 *����ʱ�� ��2001-10-30
 *�޸ļ�¼ ��
*****************************************************************************/

#include "proc_mgr.h"
#include "proc_child.h"
#include "monitor.h"
#include "../utility/mml_report.h"
#include "../include/resource.h"
#include "ace/High_Res_Timer.h"

/****************************************************************************
����ԭ��	public: CProcessManager()
��������	���캯��
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
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
����ԭ��	public: ~CProcessManager()
��������	��������
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
*****************************************************************************/
CProcessManager:: ~CProcessManager()
{
    LISTPROCCHILD::iterator it;

    //����ӽ����б�
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
����ԭ��	public: int InitInstance()
��������	��ʼ������
�������	�ޡ�
�������	�ޡ�
����ֵ  	�����ʼ�������������򷵻�ERR_SUCCESS�����򷵻�ERR_FAIL��
�׳��쳣	�ޡ�
*******************************************************************************/
int CProcessManager:: InitInstance()
{
    //������������
    int nRet = CreateAllProcessChild();
    
    return nRet;
}

/*****************************************************************************
����ԭ��	protected: int CreateAllProcessChild()
��������	��������ҵ����̶���
�������	�ޡ�
�������	�ޡ�
����ֵ  	��������ҵ����̶���ɹ����򷵻�ERR_SUCCESS�����򷵻�ERR_FAIL��
�׳��쳣	�ޡ�
******************************************************************************/
int CProcessManager:: CreateAllProcessChild()
{
    SMappedAppInfo *pInfo = NULL;
    CProcessChild *pChild = NULL;

    //�������ļ��������ģ��ʹ�õĲ���
    CINIFile *pIniFile = new CINIFile(GetCfgFilePath());
    if (!pIniFile->Open())
    {
        delete pIniFile;
        return ERR_OPEN_CFG_FILE;
    }

    //�������ļ��ж�ȡ�������
    int nAPCount = pIniFile->GetInt(CFG_SEC_COMMON,
                                    CFG_COMMON_KEY_APCOUNT, 
                                    CFG_COMMON_KEY_APCOUNT_DEFAULT);

    //����������Ϊ�㣬��δ��������
    if (0 > nAPCount)
    {
        pIniFile->Close();
        delete pIniFile;

        return ERR_FAIL;
    }

    //���OM(����ά��)����
    pInfo = new SMappedAppInfo;
    pInfo->bCanRestart = TRUE;       //��������
    pInfo->bStart      = TRUE;       //��Ҫ�����ý���
    pInfo->nPType      = PT_OM;      //��������

    //����OM��������ʶ
    strcpy(pInfo->szAppName, S_APPNAME_OM);

    //��WIN32ƽ̨�£���ִ���ļ���Ҫ��".exe"��׺
    #ifdef _PLATFORM_WIN32
        strcat(pInfo->szAppName, ".exe");
    #endif 
 
    //����OM���̵�ȫ·����
    strcpy(pInfo->szPathName, GetAppPath()); 
    strcat(pInfo->szPathName, ACE_DIRECTORY_SEPARATOR_STR); 
    strcat(pInfo->szPathName, S_APPNAME_OM); 
    
    //��WIN32ƽ̨�£���ִ���ļ���Ҫ��".exe"��׺
    #ifdef _PLATFORM_WIN32
        strcat(pInfo->szPathName, ".exe");
    #endif 

    //�����ӽ��̲������ӽ�����Ҫ�����в���
    pChild = new CProcessChild(pInfo);
    //ʹOM���̳�Ϊ��������
    pChild->EnableBaseProc();

    if (Add(pChild) != ERR_SUCCESS)
    {
        delete pChild;
        
        pIniFile->Close();
        delete pIniFile;

        return ERR_FAIL;
    }

    //��ӽ������̣�����Ӧ����һ����������
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
        
        //��������ID��Ҫ����һ��ƫ����
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

        //��WIN32ƽ̨�£���ִ���ļ���Ҫ��".exe"��׺
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

    //������Ҫ���������Ľ�����Ϣ
    int nAddProcNum = pIniFile->GetInt(CFG_SEC_ADDPROC, 
                                       CFG_ADDPROC_SEC_PROCNUM, 
                                       0);
    //���������Ҫ���������Ľ��̣����ӿ���ʱ��ʹ�ã���Ϊ�Ժ�����
    //����С�����ṩ�˽ӿ�
    char pszBuf[MAX_PATH];
    for (i = 1; i <= nAddProcNum; i++)
    {
        pInfo = new SMappedAppInfo;
        pInfo->bCanRestart = TRUE;
        pInfo->bStart      = TRUE;
       
        //��������
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

         //�����̱��
         pInfo->nPType = (PROCESS_TYPE)(pIniFile->GetInt(pInfo->szAppName, 
                                                         CFG_ADDPROC_SEC_PTYPE, 
                                                         PT_UNKNOWN));
         //��·����
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

         //�������в���
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

         //����������ӽ���
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
����ԭ��	protected: int Add(CProcessChild *pChild)
��������	����ҵ�����ָ����ӵ�ҵ�����ָ��list��
�������	pChild: ����ӵ��ӽ��̶���ָ�롣
�������	�ޡ�
����ֵ  	����ҵ�����ָ����ӵ�ҵ�����ָ��LIST���Ƿ�ɹ�������ɹ����򷵻�
                ERR_SUCCESS�����򷵻�ERR_FAIL��
�׳��쳣	�ޡ�
************************************************************************************/
int CProcessManager:: Add(CProcessChild* pChild)
{
    m_ProcChild.push_back(pChild);

    return ERR_SUCCESS; 
}

/********************************************************************************
����ԭ��	public: void ProcInitAns(PROCESS_TYPE nPType)
��������	�����ɸ�ҵ������й���ģ�鷢�����ĳ�ʼ��Ӧ��
�������	���س�ʼ��Ӧ���ҵ����̱�š�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
*********************************************************************************/
void CProcessManager:: ProcInitAns(PROCESS_TYPE nPType)
{
    if ( (g_lDebugVersion == 0 ) && (GetStatus(nPType) != APP_START_PENDING) )
    {
        return;
    }
	
    //����ڵ���״̬���߹���ģ���Ѿ������ÿ�յ�һ������Ӧ��
    //�ͼ���һ�θ��ӽ���
    if (g_lDebugVersion || (PROCMGR_ACTIVE == m_nStatus) || (PROCMGR_ACTIVING == m_nStatus))
    {
        //���ݽ���ID���ҽ���
        CProcessChild *pChild = LookUp(nPType);
		
        //δ�ҵ�����������
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
����ԭ��	public: void ProcActiveAns(PROCESS_TYPE nPType)
��������	�����ɸ���ҵ����̷������ļ���Ӧ��
�������	���ؼ���Ӧ���ҵ����̱�š�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
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
����ԭ��	protected: void ProcFailedRecovery(PROCESS_TYPE nPType)
��������	����ҵ����̱��ػָ�ʧ�ܵĴ���
�������	���б��ػָ�ʧ�ܵ�ҵ����̱�š�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
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
����ԭ��	protected: CProcessChild *LookUp(PROCESS_TYPE nPType)
��������	ͨ�������ҵ����̱�Ų�ѯ��ҵ����̶����ָ�롣
�������	ҵ����̱�š�
�������	�ޡ�
����ֵ  	����ָ��ҵ����̱�ŵĽ��̶���ָ�롣
�׳��쳣	�ޡ�
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
����ԭ��	protected: CProcessChild *LookUp(pid_t nPid)
��������	ͨ������Ľ���ID�Ų�ѯ��ҵ����̶����ָ�롣
�������	ҵ�����ID�š�
�������	�ޡ�
����ֵ  	����ָ��ҵ�����ID�ŵĽ��̶���ָ�롣
�׳��쳣	�ޡ�
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
����ԭ��	protected: int Start(PROCESS_TYPE nPType)
��������	����ָ����ҵ����̡�
�������	ҵ����̱�š�
�������	�ޡ�
����ֵ  	��������ָ����ҵ������Ƿ�ɹ�����������ɹ����򷵻�ERR_SUCCESS��
                ���򷵻ط���ֵ��
�׳��쳣	�ޡ�
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
����ԭ��	public: int Start()
��������	��������ҵ����̡�
�������	�ޡ�
�������	�ޡ�
����ֵ  	������������ҵ������Ƿ�ɹ���
�׳��쳣	�ޡ�
***********************************************************************************/
int CProcessManager:: Start()
{
    int nRet = ERR_SUCCESS;
    LISTPROCCHILD::iterator it;

    //�������������ӽ���
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
        m_uInitTimeOut = 0;    //��ʼ����ʱʱ�����
        m_nStatus = PROCMGR_START_PENDING;
        
        TRACE(MTS_MONITOR, SZ_MSG_ALL_PROCESS_OK);
        StartWatchDog();
    }
    else
    {
        Stop();   //��������κ�һ���ӽ���ʧ�ܣ���ر������Ѿ��������ӽ���
        m_nStatus = PROCMGR_FAILURE;
    }
    
    return nRet;
}

/****************************************************************************************
����ԭ��	protected: void Stop(PROCESS_TYPE nPType)
��������	ǿ�йر�ָ����ҵ����̡�
�������	ҵ����̱�š�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
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
����ԭ��	protected: void Stop()
��������	�ر����е�ҵ����̡�
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
*****************************************************************************************/
void CProcessManager:: Stop()
{
    //���ػ��߳��˳���־
    m_bExitFlag = TRUE;
    m_nStatus = PROCMGR_STOP_PENDING;

    //�ȴ��ػ��߳��˳�
    ACE_Time_Value timeout = ACE_OS::gettimeofday();
    timeout += (ACE_Time_Value)20;

    m_ThdMgr.wait(&timeout);

    //������ҵ����̶����״̬ΪAPP_STOP_PENDING
    //������ҵ����̷��ر���Ϣ
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

    //�ȴ�����ҵ������˳�
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
����ԭ��	public: int WaitExitPid(UINT4 uSec)
��������	�ȴ������ӽ���ֱ����ʱ�������˳��Ľ���PID
�������	��ʱʱ��,��λ��s��
�������	�ޡ�
����ֵ  	>0---��ǰ�˳��Ľ���PID
            ==0--��ʱ��
�׳��쳣	�ޡ�
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
// ������: 	CProcessManager::Wait
// ����:    Wangfeng
// ʱ��:    01-11-22
// ����:    �ȴ������ӽ���ֱ����ʱ�����ص�ǰδ�˳��Ľ�����
// ����:
//       ����1 :UINT4 uSec
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
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
����ԭ��	public: int Active()
��������	��������ҵ����̣�������𼤻�����ɹ����򷵻�ERR_SUCCESS�����򷵻�ERR_FAIL��
�������	�ޡ�
�������	�ޡ�
����ֵ  	���ؼ�������ҵ������Ƿ�ɹ���
�׳��쳣	�ޡ�
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
����ԭ��	public: void DeActive()
��������	ȥ��������ҵ����̡�
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
*****************************************************************************************/
void CProcessManager:: DeActive()
{
    Stop();
}

/****************************************************************************************
����ԭ��	protected: int StartWatchDog()
��������	�������ҵ����̵��ػ��̺߳�����
�������	�ޡ�
�������	�ޡ�
����ֵ  	�����������ҵ����̵��ػ��߳��Ƿ�ɹ���
�׳��쳣	�ޡ�
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
����ԭ��	protected: static void ProcWatchDog(CProcessManager* pMgr)
��������	���ҵ����̵��ػ��̺߳�����
�������	CProcessManager �����ָ�롣
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
******************************************************************************************/
void CProcessManager:: ProcWatchDog(CProcessManager* pMgr)
{
    pMgr->WatchDog();
}

/***************************************************************************************
����ԭ��	protected: void WatchDog()
��������	���ҵ������м������ҵ����̵ĺ�����
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
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
����ԭ��	protected: int ProcRecovery(PROCESS_TYPE nPType)
��������	��ĳҵ����̽��б��ػָ�������
�������	ҵ����̱�š�
�������	�ޡ�
����ֵ  	������𱾵ػָ��ɹ����򷵻�ERR_SUCCESS �����򷵻�ERR_FAIL��
�׳��쳣	�ޡ�
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
����ԭ��	public: void Timer5Sec()
��������	5�붨ʱ������
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
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
                    
                    // ������ 2002-03-13 add ���ⵥD11733
                    CMMLReport Report(MTS_MONITOR);
                    Report.MakeReportHead();            //���챨��ͷ
                    //�����������
                    Report.MakeCmdDisplay("AUP:", 0, ERR_SUCCESS, MTS_MONITOR);
                    //���췵�ر�������
                    Report.AddString(SZ_AUP_IS_COMPLETE);
                    //��������
                    Report.AddString(STR_NEWLINE);
                    //���챨��β 
                    Report.MakeReportTail();              
                    const char* pChar = Report.GetReport();
                    int nLen = strlen(pChar); 
                    
                    //��MML�ͻ���Ӧ������������
                    MSG_INTRA* pAns = new(nLen + 1)MSG_INTRA;
                    strcpy((char *)(pAns->Data), (const char *)pChar);
                    pAns->SenderPid     = CMsgProcessor::GetProcessType();
                    pAns->SenderPid     = MT_MONITOR;
                    pAns->ReceiverPid   = PT_OM; 
                    pAns->ReceiverMid   = MT_MMLDEBUG;
                    pAns->AppType       = APP_MML_NORM;
                    pAns->ClientNo      = 0;
                    pAns->IPAddr        = 0; 
                    CMsgProcessor::SendMsg(pAns);    //������Ϣ
                    //add over for D11733
                }
            }
            break;

        default:
            break;
    }
}

/************************************************************************************************
����ԭ��	protected: EAPPSTATUS GetStatus(PROCESS_TYPE nPType)
��������	���ָ��ҵ����̵�ǰ��״̬��
�������	ҵ����̱�š�
�������	�ޡ�
����ֵ  	ָ��ҵ����̵�ǰ��״̬��
�׳��쳣	�ޡ�
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
����ԭ��	public: EPROCMGRSTATUS  GetStatus()
��������	���ҵ����̹����߶���ǰ��״̬��
�������	�ޡ�
�������	�ޡ�
����ֵ  	����ҵ����̹����߶���ǰ��״̬��
�׳��쳣	�ޡ�
*************************************************************************************************/
EPROCMGRSTATUS CProcessManager:: GetStatus()
{
    return m_nStatus;
}

/****************************************************************************************
����ԭ��	protected: int SetStatus(PROCESS_TYPE nPType, EAPPSTATUS nStatus)
��������	����ָ��ҵ����̵�ǰ��״̬��
�������	nPType---ҵ����̱�š� nStatus---ҵ�����״ֵ̬
�������	�ޡ�
����ֵ  	����ָ��ҵ����̵�ǰ��״̬�Ƿ�ɹ���
�׳��쳣	�ޡ�
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
����ԭ��	public: void  ProcAuxUpgradeCmd()
��������	����MMLģ��ĸ����������
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
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

/**************************�������ɻ���************************************************/
void CProcessManager::ProduceCDR()
{
    LISTPROCCHILD::iterator it;
    
    for (it = m_ProcChild.begin(); it != m_ProcChild.end(); it++)
    {
        CMonitor::SendMsgToManager((*it)->GetProcessType(), MML_PRODUCE_CDR);
    }
}

/****************************************************************************************
����ԭ��	public: void ProcAuxUpgradeAns(PROCESS_TYPE nPType)
��������	����������̷����ĸ�������Ӧ��
�������	����������Ӧ��Ľ������̺š�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
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
����ԭ��	public: EAUXUPGSTATUS  GetAuxUpgStatus()
��������	���ҵ����̹����߶���ĸ�������״̬��
�������	�ޡ�
�������	�ޡ�
����ֵ  	ҵ����̹����߶���ĸ�������״̬��
�׳��쳣	�ޡ�
*****************************************************************************************/
EAUXUPGSTATUS CProcessManager:: GetAuxUpgStatus()
{
    return m_nAuxUpdStatus;
}

/****************************************************************************************
����ԭ��	public: void  AuxUpgFailSetStatus()
��������	��������ʧ�ܣ��Ѹ�������״̬����ΪAUXUPG_STOPPED��
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
*****************************************************************************************/
//by ldf 2002.4.8 ��Ӧ���ⵥ D12433
void CProcessManager:: AuxUpgFailSetStatus()
{
    LISTPROCCHILD::iterator it;
	
    for (it = m_ProcChild.begin(); it != m_ProcChild.end(); it++)
    {
        (*it)->SetAuxUpgStatus(AUXUPG_STOPPED);
    }

    m_nAuxUpdStatus = AUXUPG_STOPPED;
}
//�޸����

