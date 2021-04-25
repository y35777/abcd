#include "../include/frame.h"
#include "../include/toolbox.h"
#include "../include/format_out.h"
#include "sync_struct.h"
#include "sync_manager.h"
#include "com_cluster.h"
#include "asyn_res_object.h"
#include "asyn_res_kernel.h"
#include "cluster_alarm.h"
#include "igwb_alarm.h"
#include "ShellShow.h"
#include "cluster_module.h"
#include "resource.h"
#include "../utility/mml_para.h"
#include "../utility/mml_report.h"



/********************************************************
* 名称: CClusterModule::CClusterModule
* 功能: 构造函数
* 输入:
* 输出:
* 返回:
********************************************************/
CClusterModule::CClusterModule()
{
    m_pAsynResKernel     = NULL;
    m_pShellShow         = NULL;
    m_pSyncManager       = NULL;
    m_pComCluster        = NULL;
    m_tBeginActiveTime   = 0;
    m_nNodeID            = 0;

    m_tBeginDeActiveTime = 0;
    m_lTimer1Sec         = 0;
	STRING strTitle      = S_CLUSTER_EXIT_MUTEX;
    if((GetVersion() & 0xFF) >= 5)
    {
        strTitle = "Global\\" + strTitle;
    }
    m_pExitMutex         = new ACE_Process_Mutex(strTitle.c_str());

	strTitle             = S_CLUSTER_SHOWICON_MUTEX;
    if((GetVersion() & 0xFF) >= 5)
    {
        strTitle = "Global\\" + strTitle;
    }
    m_pShowIconMutex     = new ACE_Process_Mutex(strTitle.c_str());
    m_bActive            = FALSE;
    m_bExit              = FALSE;

    CINIFile* pIniFile   = new CINIFile(GetCfgFilePath());
    pIniFile->Open();

    //读出异步资源激活操作的超时时间，缺省值为180秒;
    m_lMaxActiveTime = pIniFile->GetInt(CFG_SEC_CLUSTER,
                                        CFG_CLSTR_ASYN_ACTIVE_TIME,
                                        CFG_CLSTR_ASYN_ACTIVE_TIME_DEFAULT);
    //读出异步资源去激活操作的超时时间，缺省值为180秒;
    m_lMaxDeActiveTime = pIniFile->GetInt(CFG_SEC_CLUSTER,
                                          CFG_CLSTR_ASYN_DEACTIVE_TIME,
                                          CFG_CLSTR_ASYN_DEACTIVE_TIME_DEFAULT);
    pIniFile->Close();
    delete pIniFile;
}

/********************************************************
* 名称: CClusterModule::~CClusterModule
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CClusterModule::~CClusterModule()
{
    m_pAsynResKernel    = NULL;
    m_pShellShow        = NULL;
    m_pSyncManager      = NULL;
    m_pComCluster       = NULL;
    if(NULL != m_pExitMutex)
    {
        delete m_pExitMutex;
        m_pExitMutex = NULL;
    }

    if(NULL != m_pShowIconMutex)
    {
        delete m_pShowIconMutex;
        m_pShowIconMutex = NULL;
    }
}

/********************************************************
* 名称: CClusterModule::InitInstance
* 功能: 初始化操作
* 输入:
* 输出:
* 返回: int 初始化操作是否成功，0表示成功、非零值表示失败
********************************************************/
int CClusterModule::InitInstance()
{
    int nRet = CWorkModule::InitInstance();
    if(nRet != ERR_SUCCESS)
    {
        return nRet;
    }

#ifdef _PLATFORM_WIN32
//保证KERNEL不存在
	STRING strTitle = S_KERNEL_MUTEX;
    if((GetVersion() & 0xFF) >= 5)
    {
        strTitle = "Global\\" + strTitle;
    }

    ACE_Process_Mutex* pMutex = new ACE_Process_Mutex(strTitle.c_str());
    int nWaitCount = 30;

    //防止进程重入
    while(pMutex->tryacquire())
    {
        if(nWaitCount-- == 0)
        {
            delete pMutex;
            return ERR_INIT_KERNEL_EXIST;
        }

        MSG_INTRA *pMsg   = new MSG_INTRA;
        pMsg->SenderPid   = CMsgProcessor::GetProcessType();
        pMsg->SenderMid   = MT_CLSTR;
        pMsg->ReceiverPid = PT_KERNEL;
        pMsg->ReceiverMid = MT_CLSTRINTERFACE;
        pMsg->CmdCode     = CMD_EXIT;
        SendMsg(pMsg);
        ACE_OS::sleep(1);
    }

    pMutex->release();
    delete pMutex;

#endif
    
    
    CINIFile* pIniFile   = new CINIFile(GetCfgFilePath());
    pIniFile->Open();

    //读出节点号
    m_nNodeID = pIniFile->GetInt(CFG_SEC_COMMON,
                                 CFG_SERVERNO,
                                 0);
    //读取共享磁盘阵列设置
    BOOL bInstallDisk = pIniFile->GetInt(CFG_SEC_CLUSTER,
                                   CFG_CLSTR_INSTALL_DISK_ARRAY,
                                   TRUE);

    //读取是否是双机
    BOOL bNoCluster;
    int nNoCluster = pIniFile->GetInt(
                          CFG_SEC_COMMON,
                          CFG_COMMON_KEY_NO_CLUSTER,
                          CFG_COMMON_KEY_NO_CLUSTER_DEFAULT);

    int nClusterMode = pIniFile->GetInt(
                          CFG_SEC_COMMON,
                          CFG_COMMON_KEY_CLUSTER_MODE,
                          CFG_COMMON_KEY_CLUSTER_MODE_TEST);
    if(nClusterMode == CFG_COMMON_KEY_CLUSTER_MODE_TEST)
    //ClusterMode未配置
    {
        if(nNoCluster == CFG_COMMON_KEY_NO_CLUSTER_DEFAULT)
        //NoCluster未配置
        {
            bNoCluster = !CFG_COMMON_KEY_CLUSTER_MODE_DEFAULT;
        }
        else
        {
            bNoCluster = nNoCluster;
        }
    }
    else
    {
        bNoCluster = !nClusterMode;
    }
    
    pIniFile->Close();
    delete pIniFile;

#ifdef _PLATFORM_WIN32
    //创建CShellShow对象
    char szClusterPath[MAX_PATH];
    SNPRINTF(szClusterPath, MAX_PATH, "%s\\%s.exe", GetAppPath(), GetAppName());
    szClusterPath[MAX_PATH - 1] = '\0';
    m_pShellShow = new CShellShow(szClusterPath);

    //调用m_pShellShow的Add函数，显示一个去激活的小图标;
    HWND hWnd = GetConsoleWindowHandle();
    if(NULL != hWnd)
    {
        m_pShellShow->Add(hWnd, m_nNodeID);
    }
    else
    {
        TRACE(THISMODULE, S_CANNOT_CREATE_ICON);
    }
#endif

    //创建CAsynResKernel对象
    char szKernelPath[MAX_PATH];
    SNPRINTF(szKernelPath, MAX_PATH, "%s\\%s.exe", GetAppPath(), S_APPNAME_KERNEL);
    szKernelPath[MAX_PATH - 1] = '\0';
    m_pAsynResKernel = new CAsynResKernel(szKernelPath, this,
                                          S_APPNAME_KERNEL);
    if(m_pAsynResKernel->Create() != ERR_SUCCESS)
    {
        return ERR_INIT_ASYN_OBJ;
    }

    //创建CSyncManager对象
    if((bNoCluster) || (bInstallDisk))
    {
        m_pSyncManager = NULL;
    }
    else
    {
        m_pSyncManager = new CSyncManager;
        if((NULL == m_pSyncManager) ||
           (m_pSyncManager->InitInstance() != ERR_SUCCESS))
        {
            return ERR_INIT_SYNC_MGR;
        }
    }

    //注册一个1秒的定时器
    m_lTimer1Sec = SetTimer(ONE_SECOND_IN_MILISEC);

    //获得CComCluster类的单实例指针
    CIGWBAlarm* pAlarm = new CIGWBAlarm;
    m_pComCluster = CComCluster::Instance(m_pAsynResKernel, pAlarm);
    if(NULL == m_pComCluster)
    {
        return ERR_INIT_COM_CLUSTER;
    }

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CClusterModule::ExitInstance
* 功能: 退出前的清理操作
* 输入:
* 输出:
* 返回: void
********************************************************/
void CClusterModule::ExitInstance()
{
    CWorkModule::ExitInstance();

    if(m_pComCluster != NULL)
    {
        CComCluster::Destroy();
        m_pComCluster = NULL;
    }

    if(m_pShellShow!= NULL)
    {
        delete m_pShellShow;
        m_pShellShow = NULL;
    }

    if(m_pAsynResKernel != NULL)
    {
        delete m_pAsynResKernel;
        m_pAsynResKernel = NULL;
    }

    if(m_pSyncManager != NULL)
    {
        delete m_pSyncManager;
        m_pSyncManager = NULL;
    }
}

/********************************************************
* 名称: CClusterModule::GetModuleType
* 功能: 获取模块类型
* 输入:
* 输出:
* 返回: MODULE_TYPE 模块类型
********************************************************/
MODULE_TYPE CClusterModule::GetModuleType()
{
    return MT_CLSTR;
}

/********************************************************
* 名称: CClusterModule::Active
* 功能: 激活操作接口
* 输入:
* 输出:
* 返回: int 操作是否成功
********************************************************/
int CClusterModule::Active()
{
    m_tBeginActiveTime = time(NULL);

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CClusterModule::DeActive
* 功能: 去激活操作接口
* 输入:
* 输出:
* 返回: int 是否去激活成功，0表示成功，非零值表示去激活失败
********************************************************/
int CClusterModule::DeActive()
{
    m_tBeginDeActiveTime = time(NULL);
/*

    MSG_INTRA *pMsg = new MSG_INTRA;

    pMsg->SenderPid   = CMsgProcessor::GetProcessType();
    pMsg->SenderMid   = MT_CLSTR;
    pMsg->ReceiverPid = PT_KERNEL;
    pMsg->ReceiverMid = MT_CLSTRINTERFACE;

    pMsg->CmdCode = CMD_EXIT;

    return SendMsg(pMsg);
*/

    return 0;
}

/********************************************************
* 名称: CClusterModule::ParseMsg
* 功能: 消息分发函数
* 输入: pMsgParsed : 待分发的消息帧指针
* 输出:
* 返回: void
********************************************************/
void CClusterModule::ParseMsg(MSG_INTRA* pMsgParsed)
{
    //如果上次进程异常退出，本次启动时，可能会收到侄儿Kernel的消息
    //这些消息是不能接收的
    if(m_tBeginActiveTime == 0)
    {
        return;
    }

    switch (pMsgParsed->CmdCode)
    {
    case CMD_SYNC_DATA:
    //其它模块发来的同步数据
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_NORMAL,
                   S_RECV_SYNC_DATA_FROM_LOCAL, 
                   pMsgParsed->SenderPid - PT_AP_BASE + 1,
                   GetModuleDesp((MODULE_TYPE)pMsgParsed->SenderMid));

            UINT4 uLen = pMsgParsed->Len - sizeof(UINT4) +
                         SYNC_FRAME_HEAD_LENGTH +
                         MSC_AP_SYNC_FRAME_HEAD_LENGTH +
                         MSC_SYNC_DATA_HEAD_LENGTH;
            SSyncFrame* pSyncFrame = (SSyncFrame*)new char[uLen];
            if(NULL == pSyncFrame)
            {
                return;
            }
            pSyncFrame->uLen = uLen - SYNC_FRAME_HEAD_LENGTH;

            SMscAPSyncFrame* pAPSyncFrame = (SMscAPSyncFrame*)pSyncFrame->Data;
            pAPSyncFrame->uAPointID   = pMsgParsed->SenderPid - PT_AP_BASE + 1;
            pAPSyncFrame->uDiffTime   = UINT4(-1);
            pAPSyncFrame->uDiffPackId = 0;
            pAPSyncFrame->uLen        = pSyncFrame->uLen - 
                                        MSC_AP_SYNC_FRAME_HEAD_LENGTH;

            SMscSyncData* pSyncData   = (SMscSyncData*) pAPSyncFrame->Data;
            pSyncData->uMID           = pMsgParsed->SenderMid;
            pSyncData->uLength        = pMsgParsed->Len - sizeof(UINT4);
            memcpy((BYTE*)&(pSyncData->uChannel), pMsgParsed->Data, sizeof(UINT4));
            memcpy(pSyncData->Data, pMsgParsed->Data + sizeof(UINT4),
                   pSyncData->uLength);

            OnRecvSyncData(pSyncFrame);
            delete pSyncFrame;
        }
        break;

    case CMD_ASK_FAILOVER:
    //倒换请求
        OnAskFailOver((SFailOverCause*) pMsgParsed->Data);
        break;

    case CMD_ANS_ACTIVE:
    //激活应答
    {
        BOOL uSucc = 0;
        memcpy((BYTE*)&uSucc, pMsgParsed->Data, sizeof(BOOL));
        MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT,
               S_RECV_ACTIVE_ANS, uSucc);

        m_pAsynResKernel->AcquireLock();
        if(m_pAsynResKernel->GetStatus() == EASYNRES_ACTIVING)
        {
            if(uSucc)
            {
                if(NULL != m_pShellShow)
                {
                    m_pShellShow->Active();
                }
                m_bActive = TRUE;
                if(NULL != m_pSyncManager)
                {
                    m_pSyncManager->Close();
                }
                m_pAsynResKernel->SetStatus(EASYNRES_ACTIVE);
            }
            else
            {
                m_pAsynResKernel->SetStatus(EASYNRES_DEACTIVE);
            }
        }
        m_pAsynResKernel->ReleaseLock();
        break;
    }
    case CMD_MML:
    //由MML SERVER转发的客户端命令
        {
            //从MML命令行中取命令字
            CParseResult pr(pMsgParsed);
            pr.CreateParseResult(pMsgParsed);

            static time_t tLastManualSwitch = 0;
            time_t tNow                     = time(NULL);
            if ((MML_SWP == pr.GetCmdCode()) && (tNow - tLastManualSwitch > 5))
            {
                OnManualOver(pMsgParsed);
                tLastManualSwitch = tNow;
            }
			if ((MML_RESET == pr.GetCmdCode()) )
            {
                OnReset(pMsgParsed);
				
            }
        }
        break;

    case CMD_REQ_STATUS:
    //请求双机状态
        OnReqStatus();
        break;

    default:
    //未能识别的命令帧
        CWorkModule::ParseMsg(pMsgParsed);
        break;
    }
}

/********************************************************
* 名称: CClusterModule::OnRecvSyncData
* 功能: 处理各业务模块发来的同步数据
* 输入: pFrame : 由其它模块发送来的同步数据帧
* 输出:
* 返回: void
********************************************************/
void CClusterModule::OnRecvSyncData(SSyncFrame* pFrame)
{
    m_SyncDataMutex.acquire();

    if(NULL != m_pSyncManager)
    {
        //保存同步数据
        m_pSyncManager->PutSyncData(pFrame, pFrame->uLen +
                                    SYNC_FRAME_HEAD_LENGTH);
    }

    m_SyncDataMutex.release();
}

/********************************************************
* 名称: CClusterModule::OnDoSyncData
* 功能: 实施状态数据的同步操作
* 输入: pData : 同步数据帧指针
*       nLen  : 同步数据帧长度
* 输出:
* 返回: void
********************************************************/
void CClusterModule::OnDoSyncData(void* pData, int nLen)
{
    if((NULL == pData) || (0 >= nLen) || (NULL == m_pSyncManager))
    {
        return;
    }

    SSyncFrame* pFrame = (SSyncFrame*)pData;
    if(pFrame->uLen + SYNC_FRAME_HEAD_LENGTH != nLen)
    {
        return;
    }

    MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_NORMAL,
           S_RECV_SYNC_DATA_FROM_PEER, nLen);

    m_SyncDataMutex.acquire();

    //实施同步数据操作
    m_pSyncManager->DoSyncData(pFrame, pFrame->uLen +
                               SYNC_FRAME_HEAD_LENGTH);

    m_SyncDataMutex.release();
}

/********************************************************
* 名称: CClusterModule::GetSyncData
* 功能: 获取状态同步数据
* 输入: BOOL bReadFile : 是否从文件中读出新的数据
* 输出: pData          : 输出缓冲区的指针
*       nLen           : 输出缓冲区的长度
* 返回: int
********************************************************/
int CClusterModule::GetSyncData(void*& pData, int& nLen,
                                BOOL bReadFile)
{
    m_SyncDataMutex.acquire();
    int nRet = ERR_SUCCESS;

    pData = NULL;
    nLen  = 0;
    if(NULL != m_pSyncManager)
    {
        //获取同步数据
        nRet = m_pSyncManager->GetSyncData(pData, nLen, bReadFile);
    }

    m_SyncDataMutex.release();

    return nRet;
}

/********************************************************
* 名称: CClusterModule::OnAskFailOver
* 功能: 处理故障倒换申请
* 输入: pCause : 倒换原因
* 输出:
* 返回: void
********************************************************/
void CClusterModule::OnAskFailOver(SFailOverCause* pCause)
{
    //申请进行FailOver倒换操作
    m_pComCluster->DoFailOver(pCause->Cause);
}

/********************************************************
* 名称: CClusterModule::OnReqStatus
* 功能: 处理双机状态请求消息
* 输入:
* 输出:
* 返回: void
********************************************************/
void CClusterModule::OnReqStatus()
{
    MSG_INTRA* pMsg   = new (2)MSG_INTRA;

    pMsg->SenderPid   = PT_CLSTR;
    pMsg->SenderMid   = GetModuleType();
    pMsg->ReceiverPid = PT_OM;
    pMsg->ReceiverMid = MT_PERF;            //发送给性能统计模块
    pMsg->CmdCode     = CMD_ANS_HB_STATUS;  //心跳状态查询应答

    UINT2 uStatus = m_pComCluster->GetHeartBeatStatus();
    memcpy(pMsg->Data, (BYTE*)&uStatus, 2); //第一字节存放私网个数

    SendMsg(pMsg);                          //发送消息
}

/********************************************************
* 名称: CClusterModule::OnManualOver
* 功能: 处理手动切换请求
* 输入: pMsgParsed : 消息帧
* 输出:
* 返回: void
********************************************************/
void CClusterModule::OnManualOver(MSG_INTRA* pMsgParsed)
{
    STRING strErrInfo;
    STRING strDetailInfo;
    CParseResult pr(pMsgParsed);
    pr.CreateParseResult(pMsgParsed);

    //申请手动倒换
    int nRet = m_pComCluster->DoManualOver(strErrInfo, strDetailInfo);

    //重要操作，写日志
    SLog log;
    log.LogType    = SLog::LOG_OPER;
    log.uIPAddr    = pMsgParsed->IPAddr;
    if (ERR_SUCCESS == nRet)
    {
        strErrInfo = S_CLSTR_MANNULOVER_SUCCESS;
        log.OperResult = SLog::OPER_SUCCESS;
        
    	SInnerAlarm alm;
    	struct in_addr Addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
        Addr.s_addr = pMsgParsed->IPAddr;
#else //_PLATFORM_HPUX
        Addr.S_un.S_addr = pMsgParsed->IPAddr;
#endif//_PLATFORM_HPUX
    	
    	alm.uAlarmID   = ALARM_ID_MANUAL_SWITCHOVER;
    	alm.yAlarmType = AT_EVENT;
    	SNPRINTF((char *)(alm.yAlarmPara), MAX_ALARM_PARA, "%s(%s)", 
    		     pMsgParsed->UserName, ACE_OS::inet_ntoa(Addr));
    	
    	SendAlarm(&alm);
    }
    else
    {
        if(strErrInfo == "")
        {
            strErrInfo = S_CLSTR_MANNULOVER_FAIL;
        }
        if(strDetailInfo == "")
        {
            strDetailInfo = S_CLSTR_MANNULOVER_FAIL;
        }
        log.OperResult = SLog::OPER_FAIL;
    }

    strncpy(log.szUserName, pMsgParsed->UserName, sizeof(log.szUserName));
    log.szUserName[sizeof(log.szUserName) - 1] = '\0';
    strncpy(log.szOperCmd, S_AGENT_MML_CMD_SWP, sizeof(log.szOperCmd));
    log.szOperCmd[sizeof(log.szOperCmd) - 1] = '\0';
    strncpy(log.szCmdDesc, S_AGENT_MML_CMDDESC_SWP, sizeof(log.szCmdDesc));
    log.szCmdDesc[sizeof(log.szCmdDesc) - 1] = '\0';

    WriteLog(&log);   //写日志

    CMMLReport Report(THISMODULE);
    Report.MakeReportHead();
    Report.MakeCmdDisplay(pr.GetCmdLine(), 0, nRet, THISMODULE);
    Report.AddString(strErrInfo.c_str());
    Report.AddString(STR_NEWLINE);  //换行
    Report.MakeReportTail();

    const char* pChar = Report.GetReport();

    int nLen          = strlen(pChar);
    MSG_INTRA* pAns   = new (nLen)MSG_INTRA;
    pAns->SenderPid   = GetProcessType();
    pAns->SenderMid   = GetModuleType();
    pAns->ReceiverPid = PT_OM;
    pAns->ReceiverMid = MT_MMLDEBUG;
    pAns->AppType     = pMsgParsed->AppType;
    pAns->ClientNo    = pMsgParsed->ClientNo;
    pAns->IPAddr      = pMsgParsed->IPAddr;

    memcpy(pAns->Data, pChar, nLen);
    SendMsg(pAns);

    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, strDetailInfo.c_str());
    TRACE(THISMODULE, strDetailInfo.c_str());
}

/********************************************************
* 名称: CClusterModule::OnReset
* 功能: 处理手工重启请求
* 输入: pMsgParsed : 消息帧
* 输出:
* 返回: void
********************************************************/
void CClusterModule::OnReset(MSG_INTRA* pMsgParsed)
{
    //记录操作员用户名
    char szResetCfgPath[MAX_PATH];
    SNPRINTF(szResetCfgPath, sizeof(szResetCfgPath),
             "%s/%s", GetAppPath(), S_RESET_INFO_FILE_NAME);
    szResetCfgPath[sizeof(szResetCfgPath) - 1] = '\0';	

    CINIFile ini(szResetCfgPath);
    BOOL bSucc = ini.Open();
    if(bSucc)
    {
        char szUserName[48];        
        strcpy(szUserName, pMsgParsed->UserName);
        
        ini.WriteString(CFG_SEC_OPERATOR,CFG_OPERATOR_KEY_NAME,szUserName);
        ini.Close();
    }
    else
    {
        TRACE(MTS_CLSTR, S_BS_FAIL_OPEN_FILE, szResetCfgPath);
    }
	
    //重要操作，写日志
    SLog log;
    log.LogType    = SLog::LOG_OPER; 
    log.uIPAddr    = pMsgParsed->IPAddr;
    log.OperResult = SLog::OPER_SUCCESS;
    strcpy(log.szUserName, pMsgParsed->UserName);
    strcpy(log.szOperCmd, S_AGENT_MML_CMD_RESET);
    strcpy(log.szCmdDesc, S_AGENT_MML_CMDDESC_RESET);
	
    WriteLog(&log);   //写日志
	
    //响应手工重启请求
    char szAnswer[128];
    sprintf(szAnswer, "%s", S_MANNUL_RESET_SUCCESS);
	
    CParseResult pr(pMsgParsed);
    pr.CreateParseResult(pMsgParsed);
    
    CMMLReport Report(THISMODULE);
    Report.MakeReportHead();
    Report.MakeCmdDisplay(pr.GetCmdLine(), 0, ERR_SUCCESS, THISMODULE);
    Report.AddString(szAnswer);
    Report.AddString(STR_NEWLINE);  //换行
    Report.MakeReportTail();
    
    const char* pChar = Report.GetReport();
    
    int nLen          = strlen(pChar);
    MSG_INTRA* pAns   = new (nLen)MSG_INTRA;
    pAns->SenderPid   = GetProcessType();
    pAns->SenderMid   = GetModuleType();
    pAns->ReceiverPid = PT_OM;
    pAns->ReceiverMid = MT_MMLDEBUG;
    pAns->AppType     = pMsgParsed->AppType;
    pAns->ClientNo    = pMsgParsed->ClientNo;
    pAns->IPAddr      = pMsgParsed->IPAddr;
    
    memcpy(pAns->Data, pChar, nLen);
    SendMsg(pAns);
	
    //双机进程退出
    m_bExit = TRUE;
    m_pComCluster->DoExit();
}
/********************************************************
* 名称: void  OnTimer(const long nTimerID)
* 功能: 定时器处理
* 输入: 时钟ID ，只读
* 输出:
* 返回: void
********************************************************/
void CClusterModule::OnTimer(const long nTimerID)
{ 
    if (nTimerID == m_lTimer1Sec)
    {
        Timer1Sec();
    }

    CWorkModule::OnTimer(nTimerID);
}

/********************************************************
* 名称: CClusterModule::Timer1Sec
* 功能: 1秒事件处理函数
* 输入:
* 输出:
* 返回: void
********************************************************/
void CClusterModule::Timer1Sec()
{
    //m_pAsynResKernel状态为正在激活状态，则判断KERNEL激活时间是否已大于m_lMaxActiveTime，
    //如果为真，则向KERNEL进程发退出消息，并置m_pAsynResKernel状态为故障状态
    ASSERT(NULL != m_pAsynResKernel);

    m_pAsynResKernel->AcquireLock();
    time_t tNow = time(NULL);
    switch(m_pAsynResKernel->GetStatus())
    {
    case EASYNRES_ACTIVING:
        if(tNow - m_tBeginActiveTime > m_lMaxActiveTime)
        {
            MSG_INTRA* pMsg   = new MSG_INTRA;
            pMsg->SenderPid   = GetProcessType();
            pMsg->SenderMid   = GetModuleType();
            pMsg->ReceiverPid = PT_KERNEL;
            pMsg->ReceiverMid = MT_CLSTRINTERFACE;
            pMsg->CmdCode     = CMD_EXIT;

            SendMsg(pMsg);
            m_pAsynResKernel->SetStatus(EASYNRES_FAILURE);
        }
        break;

    case EASYNRES_DEACTIVING:
    //如果m_pAsynResKernel状态为正在去激活状态，则检查KERNEL进程是否存在

        if(m_pAsynResKernel->CheckStatus() != ERR_SUCCESS)
        //如果KERNEL进程已退出，则置m_pAsynResKernel状态为去激活状态。
        {
            if(NULL != m_pShellShow)
            {
                m_pShellShow->DeActive();
            }
            m_pAsynResKernel->SetStatus(EASYNRES_DEACTIVE);
            m_pAsynResKernel->SetPidInValid();
            //发送同步数据
            if(m_bActive)
            {
                SendSyncData(TRUE);
                m_bActive = FALSE;
            }
            if(NULL != m_pSyncManager)
            {
                m_pSyncManager->Open();
            }
        }
        else if(tNow - m_tBeginDeActiveTime > m_lMaxDeActiveTime)
        //如果KERNEL进程还在运行，则判断KERNEL去激活时间是否已大于
        //m_lMaxActiveTime，如果为真，则调用ACE::terminate函数，
        //强行杀掉KERNEL，并置m_pAsynResKernel状态为去激活状态。
        {
            m_pAsynResKernel->Terminate();
            if(NULL != m_pShellShow)
            {
                m_pShellShow->DeActive();
            }
            m_pAsynResKernel->SetStatus(EASYNRES_FAILURE);
            m_pAsynResKernel->SetPidInValid();
            //发送同步数据
            if(m_bActive)
            {
                SendSyncData(TRUE);
                m_bActive = FALSE;
            }
            if(NULL != m_pSyncManager)
            {
                m_pSyncManager->Open();
            }
        }
        else if(tNow - m_tBeginDeActiveTime > 2)
        {
            MSG_INTRA *pMsg   = new MSG_INTRA;
            pMsg->SenderPid   = CMsgProcessor::GetProcessType();
            pMsg->SenderMid   = MT_CLSTR;
            pMsg->ReceiverPid = PT_KERNEL;
            pMsg->ReceiverMid = MT_CLSTRINTERFACE;
            pMsg->CmdCode     = CMD_EXIT;
            SendMsg(pMsg);
        }
        break;

    default:
        break;
    }
    m_pAsynResKernel->ReleaseLock();

    //定时向OM进程的性能模块发双机状态信息
    static int nReqStatusInv = 0;
    if(++nReqStatusInv >= 5)
    {
        OnReqStatus();
        nReqStatusInv = 0;
    };

    //发送同步数据
    if(m_bActive)
    {
        SendSyncData(FALSE);
    }

    //如果双机已完成去激活操作，等待退出，则发消息让本进程退出
    if(m_pComCluster->GetbWaitForExit())
    {
        SendQuitToMe();
    }

    if (!m_bExit)
    {
        if(m_pExitMutex->tryacquire() == 0)
        {
            m_pExitMutex->release();
        }
        else
        {
            m_bExit = TRUE;
            m_pComCluster->DoExit();
        }
    }

    if(m_pShowIconMutex->tryacquire() == 0)
    {
        m_pShowIconMutex->release();
    }
    else
    {
        HWND hWnd = GetConsoleWindowHandle();
        if(NULL != hWnd)
        {
            m_pShellShow->Add(hWnd, m_nNodeID);
        }
    }
}

/********************************************************
* 名称: CClusterModule::SendSyncData
* 功能: 向对端发送同步数据帧
* 输入: bReadFile : 是否从文件中读取同步数据
* 输出: 
* 返回: void 
********************************************************/
void CClusterModule::SendSyncData(BOOL bReadFile)
{
    void* pData = NULL;
    int nLen    = 0;

    GetSyncData(pData, nLen, bReadFile);
    if(nLen > 0)
    {
        SClusterFrame* pFrame = new(nLen) SClusterFrame;
        pFrame->nCmdCode      = CMD_CLS_SYNC_DATA;
        memcpy(pFrame->Data, (BYTE*)pData, nLen);
        delete [] pData;
        m_pComCluster->Send(pFrame);

        MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_NORMAL,
               S_SEND_SYNC_DATA, nLen);
    }
}
