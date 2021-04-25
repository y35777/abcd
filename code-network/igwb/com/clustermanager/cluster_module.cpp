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
* ����: CClusterModule::CClusterModule
* ����: ���캯��
* ����:
* ���:
* ����:
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

    //�����첽��Դ��������ĳ�ʱʱ�䣬ȱʡֵΪ180��;
    m_lMaxActiveTime = pIniFile->GetInt(CFG_SEC_CLUSTER,
                                        CFG_CLSTR_ASYN_ACTIVE_TIME,
                                        CFG_CLSTR_ASYN_ACTIVE_TIME_DEFAULT);
    //�����첽��Դȥ��������ĳ�ʱʱ�䣬ȱʡֵΪ180��;
    m_lMaxDeActiveTime = pIniFile->GetInt(CFG_SEC_CLUSTER,
                                          CFG_CLSTR_ASYN_DEACTIVE_TIME,
                                          CFG_CLSTR_ASYN_DEACTIVE_TIME_DEFAULT);
    pIniFile->Close();
    delete pIniFile;
}

/********************************************************
* ����: CClusterModule::~CClusterModule
* ����: ��������
* ����:
* ���:
* ����:
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
* ����: CClusterModule::InitInstance
* ����: ��ʼ������
* ����:
* ���:
* ����: int ��ʼ�������Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CClusterModule::InitInstance()
{
    int nRet = CWorkModule::InitInstance();
    if(nRet != ERR_SUCCESS)
    {
        return nRet;
    }

#ifdef _PLATFORM_WIN32
//��֤KERNEL������
	STRING strTitle = S_KERNEL_MUTEX;
    if((GetVersion() & 0xFF) >= 5)
    {
        strTitle = "Global\\" + strTitle;
    }

    ACE_Process_Mutex* pMutex = new ACE_Process_Mutex(strTitle.c_str());
    int nWaitCount = 30;

    //��ֹ��������
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

    //�����ڵ��
    m_nNodeID = pIniFile->GetInt(CFG_SEC_COMMON,
                                 CFG_SERVERNO,
                                 0);
    //��ȡ���������������
    BOOL bInstallDisk = pIniFile->GetInt(CFG_SEC_CLUSTER,
                                   CFG_CLSTR_INSTALL_DISK_ARRAY,
                                   TRUE);

    //��ȡ�Ƿ���˫��
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
    //ClusterModeδ����
    {
        if(nNoCluster == CFG_COMMON_KEY_NO_CLUSTER_DEFAULT)
        //NoClusterδ����
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
    //����CShellShow����
    char szClusterPath[MAX_PATH];
    SNPRINTF(szClusterPath, MAX_PATH, "%s\\%s.exe", GetAppPath(), GetAppName());
    szClusterPath[MAX_PATH - 1] = '\0';
    m_pShellShow = new CShellShow(szClusterPath);

    //����m_pShellShow��Add��������ʾһ��ȥ�����Сͼ��;
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

    //����CAsynResKernel����
    char szKernelPath[MAX_PATH];
    SNPRINTF(szKernelPath, MAX_PATH, "%s\\%s.exe", GetAppPath(), S_APPNAME_KERNEL);
    szKernelPath[MAX_PATH - 1] = '\0';
    m_pAsynResKernel = new CAsynResKernel(szKernelPath, this,
                                          S_APPNAME_KERNEL);
    if(m_pAsynResKernel->Create() != ERR_SUCCESS)
    {
        return ERR_INIT_ASYN_OBJ;
    }

    //����CSyncManager����
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

    //ע��һ��1��Ķ�ʱ��
    m_lTimer1Sec = SetTimer(ONE_SECOND_IN_MILISEC);

    //���CComCluster��ĵ�ʵ��ָ��
    CIGWBAlarm* pAlarm = new CIGWBAlarm;
    m_pComCluster = CComCluster::Instance(m_pAsynResKernel, pAlarm);
    if(NULL == m_pComCluster)
    {
        return ERR_INIT_COM_CLUSTER;
    }

    return ERR_SUCCESS;
}

/********************************************************
* ����: CClusterModule::ExitInstance
* ����: �˳�ǰ���������
* ����:
* ���:
* ����: void
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
* ����: CClusterModule::GetModuleType
* ����: ��ȡģ������
* ����:
* ���:
* ����: MODULE_TYPE ģ������
********************************************************/
MODULE_TYPE CClusterModule::GetModuleType()
{
    return MT_CLSTR;
}

/********************************************************
* ����: CClusterModule::Active
* ����: ��������ӿ�
* ����:
* ���:
* ����: int �����Ƿ�ɹ�
********************************************************/
int CClusterModule::Active()
{
    m_tBeginActiveTime = time(NULL);

    return ERR_SUCCESS;
}

/********************************************************
* ����: CClusterModule::DeActive
* ����: ȥ��������ӿ�
* ����:
* ���:
* ����: int �Ƿ�ȥ����ɹ���0��ʾ�ɹ�������ֵ��ʾȥ����ʧ��
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
* ����: CClusterModule::ParseMsg
* ����: ��Ϣ�ַ�����
* ����: pMsgParsed : ���ַ�����Ϣָ֡��
* ���:
* ����: void
********************************************************/
void CClusterModule::ParseMsg(MSG_INTRA* pMsgParsed)
{
    //����ϴν����쳣�˳�����������ʱ�����ܻ��յ�ֶ��Kernel����Ϣ
    //��Щ��Ϣ�ǲ��ܽ��յ�
    if(m_tBeginActiveTime == 0)
    {
        return;
    }

    switch (pMsgParsed->CmdCode)
    {
    case CMD_SYNC_DATA:
    //����ģ�鷢����ͬ������
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
    //��������
        OnAskFailOver((SFailOverCause*) pMsgParsed->Data);
        break;

    case CMD_ANS_ACTIVE:
    //����Ӧ��
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
    //��MML SERVERת���Ŀͻ�������
        {
            //��MML��������ȡ������
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
    //����˫��״̬
        OnReqStatus();
        break;

    default:
    //δ��ʶ�������֡
        CWorkModule::ParseMsg(pMsgParsed);
        break;
    }
}

/********************************************************
* ����: CClusterModule::OnRecvSyncData
* ����: �����ҵ��ģ�鷢����ͬ������
* ����: pFrame : ������ģ�鷢������ͬ������֡
* ���:
* ����: void
********************************************************/
void CClusterModule::OnRecvSyncData(SSyncFrame* pFrame)
{
    m_SyncDataMutex.acquire();

    if(NULL != m_pSyncManager)
    {
        //����ͬ������
        m_pSyncManager->PutSyncData(pFrame, pFrame->uLen +
                                    SYNC_FRAME_HEAD_LENGTH);
    }

    m_SyncDataMutex.release();
}

/********************************************************
* ����: CClusterModule::OnDoSyncData
* ����: ʵʩ״̬���ݵ�ͬ������
* ����: pData : ͬ������ָ֡��
*       nLen  : ͬ������֡����
* ���:
* ����: void
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

    //ʵʩͬ�����ݲ���
    m_pSyncManager->DoSyncData(pFrame, pFrame->uLen +
                               SYNC_FRAME_HEAD_LENGTH);

    m_SyncDataMutex.release();
}

/********************************************************
* ����: CClusterModule::GetSyncData
* ����: ��ȡ״̬ͬ������
* ����: BOOL bReadFile : �Ƿ���ļ��ж����µ�����
* ���: pData          : �����������ָ��
*       nLen           : ����������ĳ���
* ����: int
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
        //��ȡͬ������
        nRet = m_pSyncManager->GetSyncData(pData, nLen, bReadFile);
    }

    m_SyncDataMutex.release();

    return nRet;
}

/********************************************************
* ����: CClusterModule::OnAskFailOver
* ����: ������ϵ�������
* ����: pCause : ����ԭ��
* ���:
* ����: void
********************************************************/
void CClusterModule::OnAskFailOver(SFailOverCause* pCause)
{
    //�������FailOver��������
    m_pComCluster->DoFailOver(pCause->Cause);
}

/********************************************************
* ����: CClusterModule::OnReqStatus
* ����: ����˫��״̬������Ϣ
* ����:
* ���:
* ����: void
********************************************************/
void CClusterModule::OnReqStatus()
{
    MSG_INTRA* pMsg   = new (2)MSG_INTRA;

    pMsg->SenderPid   = PT_CLSTR;
    pMsg->SenderMid   = GetModuleType();
    pMsg->ReceiverPid = PT_OM;
    pMsg->ReceiverMid = MT_PERF;            //���͸�����ͳ��ģ��
    pMsg->CmdCode     = CMD_ANS_HB_STATUS;  //����״̬��ѯӦ��

    UINT2 uStatus = m_pComCluster->GetHeartBeatStatus();
    memcpy(pMsg->Data, (BYTE*)&uStatus, 2); //��һ�ֽڴ��˽������

    SendMsg(pMsg);                          //������Ϣ
}

/********************************************************
* ����: CClusterModule::OnManualOver
* ����: �����ֶ��л�����
* ����: pMsgParsed : ��Ϣ֡
* ���:
* ����: void
********************************************************/
void CClusterModule::OnManualOver(MSG_INTRA* pMsgParsed)
{
    STRING strErrInfo;
    STRING strDetailInfo;
    CParseResult pr(pMsgParsed);
    pr.CreateParseResult(pMsgParsed);

    //�����ֶ�����
    int nRet = m_pComCluster->DoManualOver(strErrInfo, strDetailInfo);

    //��Ҫ������д��־
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

    WriteLog(&log);   //д��־

    CMMLReport Report(THISMODULE);
    Report.MakeReportHead();
    Report.MakeCmdDisplay(pr.GetCmdLine(), 0, nRet, THISMODULE);
    Report.AddString(strErrInfo.c_str());
    Report.AddString(STR_NEWLINE);  //����
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
* ����: CClusterModule::OnReset
* ����: �����ֹ���������
* ����: pMsgParsed : ��Ϣ֡
* ���:
* ����: void
********************************************************/
void CClusterModule::OnReset(MSG_INTRA* pMsgParsed)
{
    //��¼����Ա�û���
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
	
    //��Ҫ������д��־
    SLog log;
    log.LogType    = SLog::LOG_OPER; 
    log.uIPAddr    = pMsgParsed->IPAddr;
    log.OperResult = SLog::OPER_SUCCESS;
    strcpy(log.szUserName, pMsgParsed->UserName);
    strcpy(log.szOperCmd, S_AGENT_MML_CMD_RESET);
    strcpy(log.szCmdDesc, S_AGENT_MML_CMDDESC_RESET);
	
    WriteLog(&log);   //д��־
	
    //��Ӧ�ֹ���������
    char szAnswer[128];
    sprintf(szAnswer, "%s", S_MANNUL_RESET_SUCCESS);
	
    CParseResult pr(pMsgParsed);
    pr.CreateParseResult(pMsgParsed);
    
    CMMLReport Report(THISMODULE);
    Report.MakeReportHead();
    Report.MakeCmdDisplay(pr.GetCmdLine(), 0, ERR_SUCCESS, THISMODULE);
    Report.AddString(szAnswer);
    Report.AddString(STR_NEWLINE);  //����
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
	
    //˫�������˳�
    m_bExit = TRUE;
    m_pComCluster->DoExit();
}
/********************************************************
* ����: void  OnTimer(const long nTimerID)
* ����: ��ʱ������
* ����: ʱ��ID ��ֻ��
* ���:
* ����: void
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
* ����: CClusterModule::Timer1Sec
* ����: 1���¼�������
* ����:
* ���:
* ����: void
********************************************************/
void CClusterModule::Timer1Sec()
{
    //m_pAsynResKernel״̬Ϊ���ڼ���״̬�����ж�KERNEL����ʱ���Ƿ��Ѵ���m_lMaxActiveTime��
    //���Ϊ�棬����KERNEL���̷��˳���Ϣ������m_pAsynResKernel״̬Ϊ����״̬
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
    //���m_pAsynResKernel״̬Ϊ����ȥ����״̬������KERNEL�����Ƿ����

        if(m_pAsynResKernel->CheckStatus() != ERR_SUCCESS)
        //���KERNEL�������˳�������m_pAsynResKernel״̬Ϊȥ����״̬��
        {
            if(NULL != m_pShellShow)
            {
                m_pShellShow->DeActive();
            }
            m_pAsynResKernel->SetStatus(EASYNRES_DEACTIVE);
            m_pAsynResKernel->SetPidInValid();
            //����ͬ������
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
        //���KERNEL���̻������У����ж�KERNELȥ����ʱ���Ƿ��Ѵ���
        //m_lMaxActiveTime�����Ϊ�棬�����ACE::terminate������
        //ǿ��ɱ��KERNEL������m_pAsynResKernel״̬Ϊȥ����״̬��
        {
            m_pAsynResKernel->Terminate();
            if(NULL != m_pShellShow)
            {
                m_pShellShow->DeActive();
            }
            m_pAsynResKernel->SetStatus(EASYNRES_FAILURE);
            m_pAsynResKernel->SetPidInValid();
            //����ͬ������
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

    //��ʱ��OM���̵�����ģ�鷢˫��״̬��Ϣ
    static int nReqStatusInv = 0;
    if(++nReqStatusInv >= 5)
    {
        OnReqStatus();
        nReqStatusInv = 0;
    };

    //����ͬ������
    if(m_bActive)
    {
        SendSyncData(FALSE);
    }

    //���˫�������ȥ����������ȴ��˳�������Ϣ�ñ������˳�
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
* ����: CClusterModule::SendSyncData
* ����: ��Զ˷���ͬ������֡
* ����: bReadFile : �Ƿ���ļ��ж�ȡͬ������
* ���: 
* ����: void 
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
