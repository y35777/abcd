#include "../include/toolbox.h"
#include "../include/msg_def.h"
#include "../include/perf_item.h"
#include "net_status.h"
#include "data_buffer.h"
#include "network.h"
#include "gtp_related.h"
#include "slide_wnd_related.h"	//����Զ2002.04.22���
#include "scan_app.h"
#include "wlan_as_app.h"


//����Ŀ¼
char CNetWork::m_szWorkDir[MAX_PATH] = "";

/******************************************************
// ������:  CNetWork::CNetWork
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ���캯��
// ����:
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CNetWork::CNetWork()
{
    m_pNetApp                   = NULL;
    m_Timer1SecId               = 0;
    m_nLowAtTime                = 0;
    m_TimerSendPacketId         = 0;
    m_bSendOnResponse           = FALSE; 
    m_bWaitForResponse          = FALSE;
    m_bMustMerge                = FALSE;
    m_uMaxPackageSize           = 0;
    m_uSendEmptyPackageCount    = 0;
    m_uPacketId                 = 0;
    m_pNetWorkStatusInfo        = new CNetWorkStatusInfo(
                                        sizeof(SNetWorkStatusInfo));
    m_pPtclTracker              = 0;
    m_uStatus                   = NWSTATUS_INIT;
    m_szStatusFileName[0]       = '\0';
    m_uLowAtSize                = 1024 * CFG_AP_KEY_LOW_AT_SIZE_DEFAULT;
    m_szStackType[0]            = '\0';
}


/******************************************************
// ������:  CNetWork::~CNetWork
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ��������
// ����:
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CNetWork::~CNetWork()
{
    if (m_pNetWorkStatusInfo)
    {
        delete m_pNetWorkStatusInfo;
        m_pNetWorkStatusInfo = 0;
    }
}



/******************************************************
// ������:  CNetWork::InitInstance
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ��ʼ��ʵ������
// ����:
//       ����1 :void
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CNetWork::InitInstance(void)
{
    int nRet;
    nRet = CWorkModule::InitInstance();
    if (nRet != ERR_SUCCESS) return nRet;

    //�������ļ�
    CINIFile *pIniFile;
    pIniFile = new CINIFile((char *)GetCfgFilePath());
    pIniFile->Open();

    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP, 
            GetProcessType() - PT_AP_BASE + 1);

    //���MustMerge=1����TRUE��m_bMustMerge������FALSE
    int nMustMerge = pIniFile->GetInt(strSec,
                                      CFG_AP_KEY_MUST_MERGE,
                                      CFG_AP_KEY_MUST_MERGE_DEFAULT);
    if(nMustMerge) m_bMustMerge = TRUE;

    //��ȡ��󻰵������ȸ�����Ա����m_nMaxPackageSize
    m_uMaxPackageSize = pIniFile->GetInt(strSec,
                                  CFG_AP_KEY_MAX_PACKAGE_SIZE,
                                  CFG_AP_KEY_MAX_PACKAGE_SIZE_DEFAULT);
    m_uMaxPackageSize *= 1024;

    //�����ͳ�ʱ��
    m_nLowAtTime = pIniFile->GetInt(strSec,
                                    CFG_AP_KEY_LOW_AT_TIME,
                                    CFG_AP_KEY_LOW_AT_TIME_DEFAULT);
    //��õͳ�����С
    m_uLowAtSize = pIniFile->GetInt(strSec,
                                    CFG_AP_KEY_LOW_AT_SIZE,
                                    CFG_AP_KEY_LOW_AT_SIZE_DEFAULT);
    m_uLowAtSize *= 1024;

    //���ÿ������Ҫ���͵����հ���
    m_uSendEmptyPackageMaxPerSec = pIniFile->GetInt(strSec,
                                    CFG_AP_KEY_MAX_EMPTY_PACKAGE,
                                    CFG_AP_KEY_MAX_EMPTY_PACKAGE_DEFAULT);

    //�������ļ�����Ӧ������ж�������ģ��״̬�ļ��Ĺ���Ŀ¼��
    //����m_szWorkDir
    pIniFile->GetString(CFG_SEC_DISK_FILE,
                        CFG_MAIN_STATUS_ROOT_DIR,
                        CFG_MAIN_STATUS_ROOT_DIR_DEFAULT,
                        (char*)m_szWorkDir,
                        MAX_PATH);
    char szAPName[255];
    pIniFile->GetString(strSec, CFG_AP_NAME, 
                        strSec, szAPName, 255);

    strcat((char*)m_szWorkDir, ACE_DIRECTORY_SEPARATOR_STR);
    strcat((char*)m_szWorkDir, szAPName);
    strcat((char*)m_szWorkDir, ACE_DIRECTORY_SEPARATOR_STR);
    strcat((char*)m_szWorkDir, "network");
    strcat((char*)m_szWorkDir, ACE_DIRECTORY_SEPARATOR_STR);

    //��������Ŀ¼
    if(!CreateDeepDir(m_szWorkDir))
    {
        TRACE(THISMODULE, S_NET_CREATE_NET_STATUS_DIR, 
              m_szWorkDir, ACE_OS::last_error());
        pIniFile->Close();
        delete pIniFile;
        return ERR_FAIL;
    }

    //��״̬�ļ���ȫ·����
    sprintf(m_szStatusFileName, "%s%s", m_szWorkDir, "status.dat");

    //ͨ����ǰ�����Ŵ������ļ��л�õ�ǰ��������ͣ�
    int nAPType = pIniFile->GetInt(strSec,
                                   CFG_AP_KEY_APTYPE,
                                   CFG_AP_KEY_APTYPE_DEFAULT);

    //ͨ����ǰ�����Ŵ������ļ��л�õ�ǰ��������ͣ�
    int nAddModuleNum = pIniFile->GetInt(strSec,
                                         CFG_AP_KEY_ADD_MOD_NUM,
                                         CFG_AP_KEY_ADD_MOD_NUM_DEFAULT);

    //ͨ����ǰ�����Ŵ������ļ��л���Ƿ����ļ�ɨ��Э��
    pIniFile->GetString(strSec,
                        CFG_AP_KEY_ADDITION_STACK,
                        CFG_AP_KEY_ADDITION_STACK_DEFAULT,
                        m_szStackType,
                        sizeof(m_szStackType));

    pIniFile->Close();
    delete pIniFile;
    
    BYTE yAPType = (BYTE)GetProcessType() - PT_AP_BASE + 1;
    if(ACE_OS::strcasecmp(m_szStackType, "ScanStack") == 0)
    {
        m_pNetApp = new CScanApp(this);
    }
    else if(ACE_OS::strcasecmp(m_szStackType, "WlanAsStack") == 0)
    {
        m_pNetApp = new CWlanAsApp(this);
    }
    else
    {
        switch(nAPType)
        {
		case AP_WLAN:
        case AP_PS:
        case AP_PS_RESERVED:
            m_pNetApp = new CGtpNetApp(this);
            m_pPtclTracker = new CGtpTrack();
            break;

		//����Զ2002.04.22�޸�
        case AP_128:
            if(nAddModuleNum)
            {
	            m_pNetApp = new C128NetApp(this);
            }
            else
            {
	            m_pNetApp = new CSlideWndNetApp(this);
            }
            m_pPtclTracker = new CAppFrmTrack();
            break;

        case AP_STP:
	        m_pNetApp = new CSlideWndNetApp(this);
            m_pPtclTracker = new CAppFrmTrack();
            break;

        case AP_GSM:
			m_pNetApp = new CGSMNetApp(this);
            m_pPtclTracker = new CAppFrmTrack();
			break;

		case AP_SOFTX:
			m_pNetApp = new CSoftXNetApp(this);
            m_pPtclTracker = new CSysFrmTrack();
			break;

		//����Զ2002.04.22�޸Ľ���

        default:
            TRACE(THISMODULE, S_NET_AP_TYPE_UNKNOWN, nAPType);
            break;
        }
    }

    //��ʼ������Ӧ�ò�
    if((!m_pNetApp) || (m_pNetApp->Init() != ERR_SUCCESS))
    {
        TRACE(THISMODULE, S_NET_INIT_NETAPP);
        return ERR_FAIL;
    }
    
    return nRet; 
}



/******************************************************
// ������:  CNetWork::GetPacketId
// ����:    Wangfeng
// ʱ��:    01-10-24
// ����:    ��ȡ��ǰ����
// ����:
// ����ֵ: UINT4 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
UINT4 CNetWork::GetPacketId()
{
    return m_uPacketId;
}


/******************************************************
// ������:  CNetWork::IsWaitForResponse
// ����:    Wangfeng
// ʱ��:    01-10-26
// ����:    ����ģ���Ƿ����ڵȴ�ǰ���̵�Ӧ��
// ����:
// ����ֵ: BOOL 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-26      Wangfeng
******************************************************/
BOOL CNetWork::IsWaitForResponse()
{
    return m_bWaitForResponse;
}


/******************************************************
// ������:  CNetWork::ExitInstance
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    �˳�ʱ��������
// ����:
//       ����1 :void
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CNetWork::ExitInstance(void)
{
    if(m_pNetApp != NULL)
    {
        m_pNetApp->DeActive();
        delete m_pNetApp;
        m_pNetApp = NULL;
    }

    if(m_pPtclTracker != NULL)
    {
        delete m_pPtclTracker;
        m_pPtclTracker = NULL;
    }

    KillTimer(m_Timer1SecId);
    KillTimer(m_TimerSendPacketId);

    CWorkModule::ExitInstance();
}


/******************************************************
// ������:  CNetWork::GetModuleType
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ����ģ�����Ϣ
// ����:
// ����ֵ: MODULE_TYPE 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
MODULE_TYPE CNetWork::GetModuleType()
{
    return MT_NETWORK;
}


/******************************************************
// ������:  CNetWork::ParseMsg
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ��Ϣ����
// ����:
//       ����1 :MSG_INTRA* pMsgParsed
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CNetWork::ParseMsg(MSG_INTRA* pMsgParsed)
{
    MSG_INTRA *pMsg;
    int nRet = 0;
    switch(pMsgParsed->CmdCode)
    {
    case CMD_MODULE_ACTIVE:
        //�յ�����ģ�鷢�͵ļ��ģ��������ǰ����ģ�鷢
        //���������ָ�����Ϣ
        pMsg = new MSG_INTRA;
        pMsg->SenderPid     = PT_SELF; 
        pMsg->SenderMid     = MT_NETWORK;
        pMsg->ReceiverPid   = PT_SELF;
        pMsg->ReceiverMid   = MT_FRONTSAVE;
        pMsg->CmdCode       = CMD_RESUME;
        SendMsg(pMsg);

        //��ʼ��״̬�ļ�,���������װ��,�������򴴽����ļ�
        nRet = InitStatus();
        if (ERR_SUCCESS != nRet)
        {
            TRACE(THISMODULE, S_NET_INIT_STATUS_FILE, nRet);
            SendQuitToMe();
        }
        break;

    case CMD_RESUME_OVER:
        //��Ϊ�����ָ������Ϣ��ȡ����Ϣ��ǰ���̷��صĵ�ǰ��ID��
        //����m_pNetApp���󣬲���Ǩ״̬Ϊ������״̬��
        if(NWSTATUS_ACTIVE == m_uStatus) break;

        //��������
        if(AdjustStatus(*(UINT4 *)pMsgParsed->Data) != ERR_SUCCESS)
        {
            TRACE(THISMODULE, S_NET_FAIL_TO_RECOVERY);
            SendQuitToMe();
            m_uStatus = NWSTATUS_DEACTIVE;
            break;
        }

        //���õͳ���ʱ��
        m_TimerSendPacketId = SetTimer(m_nLowAtTime);

        //�������¼���ʱ��
        m_Timer1SecId = SetTimer(1000);

        //��������Ӧ�ò�
        m_pNetApp->Active();
        m_uStatus = NWSTATUS_ACTIVE;
        break;

    case CMD_HOST_DATA:
        //��Ϊ����Э����Ϣ������m_pNetApp��OnReceive��Э������
        //֡������ڣ�
        m_pNetApp->OnReceive((SNetFrame*)pMsgParsed);
        break;

    case CMD_SAVE_SUCCEED:
        //��Ϊǰ����Ӧ����Ϣ������m_pNetApp��OnResponse��Ȼ�����
        //SaveStatus����״̬�ļ�
        ASSERT(*(UINT4 *)pMsgParsed->Data == m_uPacketId);
        
        //Ӧ��
        OnResponse(TRUE);

        //����Ǹ�������״̬����һ���ڷ��հ���С��20������ǰ���̷��հ�
        switch(m_uStatus)
        {
        case NWSTATUS_AUXUPGRADE:
            if (m_uSendEmptyPackageCount < m_uSendEmptyPackageMaxPerSec)
            {
                SendEmptyPackage();
                m_uSendEmptyPackageCount++;
            }
            break;

        case NWSTATUS_ACTIVE:
            if(m_bSendOnResponse)
            {
                if(m_pNetApp->GetReadyDataSize() > 0) 
                {
                    SendBillPackage();
                }
                m_bSendOnResponse = FALSE;
            }
            else if(m_pNetApp->GetReadyDataSize() >= m_uLowAtSize)
            {
                SendBillPackage();
            }
            break;
        //by ldf 2002.4.8 ��Ӧ���ⵥ D12457
        default:
            {
                break;
            }
        }
        break;
        //�޸����

    case CMD_SYNC_PACKETID:
        //��Ϊ������ͬ��ԭʼ��������������Ϣ�е�PacketID����m_PacketId
        //��Ϣ�е�PacketIDΪǰ�����ѳɹ�����İ���
        //ע��:��Ϣ�е�PacketIDΪǰ���̵�ǰ�ѳɹ�����İ���
        if (m_uPacketId != *(UINT4 *)pMsgParsed->Data)
        {
            BOOL bExit = FALSE;
            if (m_uPacketId > *(UINT4 *)pMsgParsed->Data)
            {
                bExit = TRUE;
            }

            m_pNetApp->OnResponse(FALSE);
            m_bWaitForResponse = FALSE;
            m_uPacketId = *(UINT4 *)pMsgParsed->Data;
            SaveStatus();
            SyncStatus();

            if (bExit)
            {
                m_pNetApp->DeActive();
                TRACE(THISMODULE, S_NET_FAIL_TO_RECOVERY);
                SendQuitToMe();
                m_uStatus = NWSTATUS_DEACTIVE;
            }
        }
        break;

    case CMD_BIN_ALARM:
		//����Զ2002.04.16���
        //��Ϊ�澯ģ��ĸ澯ת����Ϣ������m_NetApp�����DispatchAlarm�ӿ�
        m_pNetApp->DispatchAlarm(pMsgParsed);
        break;
		//����Զ2002.04.16��ӽ���

    case CMD_AUXUPGRADE:
        //��Ϊ����������Ϣ��ȥ����m_pNetApp���󣬱�Ǩ����������
        //��״̬���������͡�֪ͨ�Ѿ���ʼ���Ϳհ���������ģ�飻
        if (NWSTATUS_ACTIVE == m_uStatus)
        {
            m_uStatus = NWSTATUS_AUXUPGRADE;
            m_pNetApp->DeActive();
            pMsg = new MSG_INTRA;
            pMsg->SenderPid     = PT_SELF; 
            pMsg->SenderMid     = MT_NETWORK;
            pMsg->ReceiverPid   = PT_SELF;
            pMsg->ReceiverMid   = MT_MANAGER;
            pMsg->CmdCode       = CMD_SENDEMPTYPACKET;
            SendMsg(pMsg);
        }
        else
        {
            TRACE(THISMODULE, S_NET_IGNORE_AUXUPGRADE, m_uStatus);
            MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, 
                   S_NET_IGNORE_AUXUPGRADE, m_uStatus);

            //by ldf 2002.4.8 ��Ӧ���ⵥD12433 D12459
            pMsg = new MSG_INTRA;
            pMsg->SenderPid     = GetProcessType(); 
            pMsg->SenderMid     = GetModuleType();
            pMsg->ReceiverPid   = PT_KERNEL;
            pMsg->ReceiverMid   = MT_MONITOR;
            pMsg->CmdCode       = CMD_AUXUPGRADE_FAIL;
            SendMsg(pMsg);
            //�޸����
        }
        break;

    case CMD_STOP_NETWORK:
        //��Ϊ�����̿ռ�����������ȥ��������Ӧ�ò����
        m_pNetApp->DeActive();
        m_uStatus = NWSTATUS_DEACTIVE;
        break;

    case CMD_RESUME_NETWORK:
        //��Ϊ�����̿ռ�ָ������򼤻�����Ӧ�ò����
        m_pNetApp->Active();
        m_uStatus = NWSTATUS_ACTIVE;
        break;

    default:
        break;
    }

    CWorkModule::ParseMsg(pMsgParsed);
}



/******************************************************
// ������:  CNetWork::OnTimer
// ����:    Wangfeng
// ʱ��:    01-10-24
// ����:    ��ʱ������
// ����:
//       ����1 :const long nTimerID
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
void CNetWork::OnTimer(const long nTimerID)
{
    if(nTimerID == m_Timer1SecId)
    {
        Timer1Sec();
    }
    else if(nTimerID == m_TimerSendPacketId)
    {
        LowAtTimer();
    }

    CWorkModule::OnTimer(nTimerID);
}




/******************************************************
// ������:  CNetWork::OnResponse
// ����:    Wangfeng
// ʱ��:    01-10-29
// ����:    ���ϴ��ṩ��ǰ����ģ�鱣��Ļ������ݽ��к����Ĵ���
// ����:
//       ����1 :BOOL const bSaved
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-29      Wangfeng
******************************************************/
void CNetWork::OnResponse(BOOL const bSaved)
{
    //�������Ӧ��
    m_pNetApp->OnResponse(bSaved);
    m_bWaitForResponse = FALSE;
    SaveStatus();
    SyncStatus();
}


/******************************************************
// ������:  CNetWork::Timer1Sec
// ����:    Wangfeng
// ʱ��:    01-10-24
// ����:    1�붨ʱ������
// ����:
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
void CNetWork::Timer1Sec()
{
    switch(m_uStatus)
    {
    case NWSTATUS_AUXUPGRADE:
        if (FALSE == m_bWaitForResponse)
        {
            SendEmptyPackage();
        }
        m_uSendEmptyPackageCount = 0;
        break;

    default:
        break;
    }
    m_pNetApp->Timer1Sec();
}



/******************************************************
// ������:  CNetWork::LowAtTimer
// ����:    Wangfeng
// ʱ��:    01-10-24
// ����:    ��ʱ����¼�����
// ����:
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
void CNetWork::LowAtTimer()
{
    switch(m_uStatus)
    {
    case NWSTATUS_ACTIVE:
        if (m_bWaitForResponse)
        {
            m_bSendOnResponse = TRUE;
        }
        else 
        {//�����׼���õĻ����ߴ����0������ǰ�淢�ͻ�����
            if(m_pNetApp->GetReadyDataSize() > 0) 
            {
                SendBillPackage();
            }
            else if(m_bMustMerge)
            {
                //���Ҫ���кϲ�����Ҫ���հ�
                SendEmptyPackage();
            }
        }
        break;

    default:
        break;
    }
}



/******************************************************
// ������:  CNetWork::AdjustStatus
// ����:    Wangfeng
// ʱ��:    01-10-24
// ����:    ���������ָ�����
// ����:
//       ����1 :UINT4 uPacketSaved--��ǰ����ģ�鷢������
//                                  �ɹ����̵İ���
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
int CNetWork::AdjustStatus(UINT4 uPacketSaved)
{
    int nRet = ERR_SUCCESS;

    if (m_bWaitForResponse)
    {
        //�����ڵȴ�Ӧ��״̬�Ұ�����ȣ���������һ���ѳɹ���
        //�棬��Ӧ��ʧ��Ӧ����Ӧ��
        if (m_uPacketId == uPacketSaved)  
        {
            OnResponse(TRUE);
        }
        //�����ڵȴ�Ӧ��״̬�Ұ��Ų��ȣ���������һ������δ����
        //����ģ��򵽴�֮��δ�ɹ����棬��ʱӦ��������İ���
        else if (m_uPacketId == uPacketSaved + 1)
        {
            m_pNetApp->OnResponse(FALSE);
            m_uPacketId = uPacketSaved;
            m_bWaitForResponse = FALSE;
            SaveStatus();
            SyncStatus();
        }
        else
        {
            //����ģ����ǰ����ģ��İ��Ų�����
            TRACE(THISMODULE, 
                  S_NET_ADJUST_PACKAGE_ID,
                  m_bWaitForResponse,
                  m_uPacketId, uPacketSaved);
            m_pNetApp->OnResponse(FALSE);
            if (m_uPacketId > uPacketSaved)
            //�����ǰ״̬�ļ��еİ��Ŵ���ǰ���̵İ���,��������,�п���
            //����һЩ���к��ļ�����Ž���m_uPacketId��uPacketSaved֮��,
            //����ͨ��������ɾ����Щ���к��ļ�.
            {
                nRet = ERR_FAIL;
            }
            m_uPacketId = uPacketSaved;
            m_bWaitForResponse = FALSE;
            SaveStatus();
            SyncStatus();
        }
    }
    else
    {
        //����ģ����ǰ����ģ��İ��Ų�����
        if(uPacketSaved != m_uPacketId) 
        {
            TRACE(THISMODULE, 
                  S_NET_ADJUST_PACKAGE_ID,
                  m_bWaitForResponse,
                  m_uPacketId, uPacketSaved);

            if (m_uPacketId > uPacketSaved)
            {
                nRet = ERR_FAIL;
            }
            m_uPacketId = uPacketSaved;
            SaveStatus();
            SyncStatus();
        }
    }
    return nRet;
}



/******************************************************
// ������:  CNetWork::SendMsgtoHost
// ����:    Wangfeng
// ʱ��:    01-10-30
// ����:    ������Ϣ������
// ����:
//       ����1 :MSG_INTRA* pMsg
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-30      Wangfeng
******************************************************/
int CNetWork::SendMsgtoHost(MSG_INTRA* pMsg)
{
    return SendMsg(pMsg);
}



/******************************************************
// ������:  CNetWork::SendBillPackage
// ����:    Wangfeng
// ʱ��:    01-10-24
// ����:    ���ͻ������ݰ�
// ����:
//       ����1 :BOOL bGetNewData--�Ƿ�ȡ�µĻ���������
// ����ֵ: BOOL -- �Ƿ񷢳�������
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
BOOL CNetWork::SendBillPackage(BOOL bGetNewData)
{

    //ȡ��׼���õĻ����ߴ�
    static UINT4 uLen = 0;
    static char* pPacket = 0;
    if(bGetNewData)
    {
        CDataBuffer DBuf;

        //ȡ��������
        int nReadyDataSize = m_pNetApp->GetReadyDataSize();
        if(0 == nReadyDataSize) return FALSE;
        if(nReadyDataSize > m_uMaxPackageSize) 
        {
            nReadyDataSize = m_uMaxPackageSize;
        }

        //ȡ�û�������
        DBuf.Alloc(nReadyDataSize);
        m_pNetApp->GetReadyData(&DBuf);
        DBuf.Get(pPacket, uLen);
    }
    //��鵱ǰ�������Ƿ���Ч
    if ((NULL == pPacket) || (0 == uLen)) return FALSE;
    
    //��ǰ���̷��͵�ǰ������
    SFrontPacketInfo BillPackage;
    BillPackage.uPID = ++m_uPacketId;
    BillPackage.pPacket = (BYTE*)pPacket;
    BillPackage.uPacketLen = uLen;

    MSG_INTRA *pMsg = new(sizeof(SFrontPacketInfo)) MSG_INTRA;
    pMsg->SenderPid = PT_SELF; 
    pMsg->SenderMid = MT_NETWORK;
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = MT_FRONTSAVE;
    pMsg->CmdCode = CMD_SAVE_PACKET;
    memcpy(pMsg->Data, &BillPackage, sizeof(SFrontPacketInfo));
    SendMsg(pMsg);

    //����״̬
    m_bWaitForResponse = TRUE;
    SaveStatus();
    SyncStatus();

    return TRUE;
}



/******************************************************
// ������:  CNetWork::SendEmptyPackage
// ����:    Wangfeng
// ʱ��:    01-10-24
// ����:    ��ǰ���̷��Ϳհ�
// ����:
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
void CNetWork::SendEmptyPackage()
{
    //��ǰ���̷��Ϳհ�
    SFrontPacketInfo BillPackage;
    BillPackage.uPID = ++m_uPacketId;
    BillPackage.pPacket = 0;
    BillPackage.uPacketLen = 0;

    MSG_INTRA *pMsg = new(sizeof(SFrontPacketInfo)) MSG_INTRA;
    pMsg->SenderPid = PT_SELF; 
    pMsg->SenderMid = MT_NETWORK;
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = MT_FRONTSAVE;
    pMsg->CmdCode = CMD_SAVE_PACKET;
    memcpy(pMsg->Data, &BillPackage, sizeof(SFrontPacketInfo));
    SendMsg(pMsg);

    //����״̬
    m_bWaitForResponse = TRUE;
    SaveStatus();
    SyncStatus();
}



/******************************************************
// ������:  CNetWork::SyncStatus
// ����:    Wangfeng
// ʱ��:    01-10-29
// ����:    ����״̬����
// ����:
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-29      Wangfeng
******************************************************/
void CNetWork::SyncStatus()
{
    int nBufLen = CNetStatusInfo::GetAllStatusDataLen();

    BYTE *pBuffer = new BYTE[nBufLen];

    if(CNetStatusInfo::GetAllStatusData(pBuffer, nBufLen))
    {
        WriteSyncData(GetModuleType(), pBuffer, nBufLen);
    }

    delete [] pBuffer;
}



/******************************************************
// ������:  CNetWork::SaveStatus
// ����:    Wangfeng
// ʱ��:    01-10-24
// ����:    ����״̬
// ����:
// ����ֵ: void 
// ע��:    ע�⣺���Բ��������ύ��δӦ�������½���
//          ��������״̬�����������ͬһ���ύ����ͬʱ
//          �����������������к��б��ļ��С�
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
int CNetWork::SaveStatus()
{
    SNetWorkStatusInfo StatusInfo;
    StatusInfo.uPacketId = m_uPacketId;
    StatusInfo.bWaitForResponse = m_bWaitForResponse;
    m_pNetWorkStatusInfo->Set((BYTE*)&StatusInfo, 
                              sizeof(SNetWorkStatusInfo));

    if((m_pNetApp->SaveStatus() != 0) || (CNetStatusInfo::FlushAll() != 0))
    {
        m_pNetApp->DeActive();
        SendQuitToMe();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}



/******************************************************
// ������:  CNetWork::LoadStatus
// ����:    Wangfeng
// ʱ��:    01-10-24
// ����:    ��ȡ״̬
// ����:
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
int CNetWork::LoadStatus()
{
    //����״̬�ļ���ͷ��
    SNetWorkStatusInfo StatusInfo;
    UINT4 uInputLen = m_pNetWorkStatusInfo->Get((BYTE*)&StatusInfo, 
                                        sizeof(SNetWorkStatusInfo));
    if(uInputLen != sizeof(SNetWorkStatusInfo))
    {
        return ERR_FAIL;
    }

    m_uPacketId = StatusInfo.uPacketId;
    m_bWaitForResponse = StatusInfo.bWaitForResponse;

    //����״̬�ļ���Э�鲿��
    return m_pNetApp->LoadStatus();
}


/******************************************************
// ������: 	CNetWork::InitStatus
// ����:    Wangfeng
// ʱ��:    02-1-29
// ����:    �ڳ�ʼ����ɺ󣬵�һ�ζ�ȡ״̬
// ����:
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//02-1-29       Wangfeng
******************************************************/
int CNetWork::InitStatus()
{
    //�Զ�/д��ʽ��״̬�ļ�������ɹ�����װ��״̬�ļ�
    //���ʧ�ܣ������´���һ��״̬��������
    int nRet = CNetStatusInfo::Init(m_szStatusFileName);
    if(ERR_SUCCESS == nRet)
    {
        ACE_stat st;
        nRet = ACE_OS::stat(m_szStatusFileName, &st);
        if((0 == nRet) && (st.st_size > 0))
        {
            //װ��״̬�ļ�
            return LoadStatus();
        }
    }
    else //�ļ�������
    {
        //����һ���µ�״̬
        FILE* file = fopen(m_szStatusFileName, "wb");
        if(!file)
        {
            TRACE(THISMODULE, S_NET_CRT_NET_STATUS_FILE, 
                  errno, strerror(errno));
            return ERR_FAIL;
        }
        fclose(file);
        nRet = CNetStatusInfo::Init(m_szStatusFileName);
    }

    return ERR_SUCCESS;
}

void CNetWork::TrackProtocol(MSG_INTRA* pMsg, int nProcRst)
{
    if(CMsgProcessor::g_bTrackProtocol)
    {
        CMsgProcessor::TrackProtocol(m_pPtclTracker->Analysis(pMsg, nProcRst));
    }
}
