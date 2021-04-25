
#include "ap_manager.h"

#include "../../com/include/toolbox.h"
#include "../../com/frontsave/front_stor.h"
#include "../../com/backsave/back_stor.h"
#include "../../com/netbase/network.h"
#include "../../com/convert/convert.h"
#include "resource.h"
#include "../../com/include/signal_tab.h"
#include "../../com/utility/frame_adapter.h"
#include "../../com/toolbase/frame_tool.h"

/****************************************************************************
����ԭ��    public:CAPMgr::CAPMgr()
��������    ���캯��
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
CAPMgr::CAPMgr()
{
	//by ldf �������ⵥ D11610  2002.3.14�޸�
    m_nRecvSignal   =0;
    m_pFrameAdapter = NULL;
}


/****************************************************************************
����ԭ��    public:CAPMgr::~CAPMgr()
��������    ��������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
CAPMgr::~CAPMgr()
{
    if (m_pPacketCache)
    {
        delete m_pPacketCache;
        m_pPacketCache = NULL;
    }
    
    if(m_pFrameAdapter)
    {
        delete m_pFrameAdapter;
        m_pFrameAdapter = NULL;
    }
}


/****************************************************************************
����ԭ��    protected:int CAPMgr::InitInstance(void)
��������    ��ʼ������
�������    ��
�������    ��
����ֵ      ERR_SUCCESS---�ɹ�
            ERR_FAIL   ---ʧ��
�׳��쳣    ��
*****************************************************************************/
int CAPMgr::InitInstance(void)
{
    int nRet;
    //��ȡ�����ļ��еĻ����������ޣ���������������������
    CINIFile *pIniFile = new CINIFile((char *)GetCfgFilePath());
    pIniFile->Open(); 

    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP,
        GetProcessType() - PT_AP_BASE + 1);

    UINT4 uLowLimit = (UINT4)pIniFile->GetInt(strSec,
        CFG_AP_KEY_PACKETLOWLIMIT, 
        CFG_AP_KEY_PACKETLOWLIMIT_DEFAULT);

    UINT4 uHighLimit = (UINT4)pIniFile->GetInt(strSec, 
        CFG_AP_KEY_PACKETHIGHLIMIT, 
        CFG_AP_KEY_PACKETHIGHLIMIT_DEFAULT);

	//�����̱��
	pIniFile->GetString(strSec, CFG_AP_NAME, strSec, m_szApName, 32);
     
    pIniFile->Close();
    delete pIniFile;

    m_pPacketCache = new CPacketCache(uLowLimit, uHighLimit); 

    nRet = CModuleManager::InitInstance();
    if (nRet != ERR_SUCCESS) return nRet;
    
	//by ldf �������ⵥ D11610  2002.3.14�޸�
    ACE_Sig_Set sigset;
    sigset.fill_set();
    reactor()->register_handler(sigset, this, 0);

    m_pFrameAdapter = new CFrameAdapter;
    CFrameTools::SetInstance(m_pFrameAdapter);

    //��KERNEL�����еļ��ģ�鷢��ʼ��Ӧ��
    MSG_INTRA *pMsg     = new MSG_INTRA;
    pMsg->SenderPid     = GetProcessType(); 
    pMsg->SenderMid     = GetModuleType();
    pMsg->ReceiverPid   = PT_KERNEL;
    pMsg->ReceiverMid   = MT_MONITOR;
    pMsg->CmdCode       = CMD_ANS_INIT;
    SendMsg(pMsg);

	//��OM���̵�����ģ�鷢�ͽӵ�����
	MSG_INTRA *pMsg1     = new (strlen(m_szApName) + 1)MSG_INTRA;
    pMsg1->SenderPid     = GetProcessType(); 
    pMsg1->SenderMid     = GetModuleType();
    pMsg1->ReceiverPid   = PT_OM;
    pMsg1->ReceiverMid   = MT_PERF;
    pMsg1->CmdCode       = CMD_AP_NAME;

	memcpy(pMsg1->Data, m_szApName, strlen(m_szApName));
	SendMsg(pMsg1);

    return ERR_SUCCESS;
}


/****************************************************************************
����ԭ��    protected:int CClstrMgr::ExitInstance(void)
��������    ������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CAPMgr::ExitInstance(void)
{
    CFrameTools::SetInstance(NULL);
    CModuleManager::ExitInstance();
}


/****************************************************************************
����ԭ��    protected:int CAPMgr::CreateWorkModules(void)
��������    ������ģ���߳�
�������    ��
�������    ��
����ֵ      ERR_SUCCESS---�ɹ�
            ERR_FAIL   ---ʧ��
�׳��쳣    ��
*****************************************************************************/
int CAPMgr::CreateWorkModules(void)
{
    int nAPType = GetProcessType() - PT_AP_BASE + 1;
    //�����ģ��
    m_WorkModules.push_back(new CBackStorage(nAPType));

    //��������ģ��
    m_WorkModules.push_back(new CBillProcessor(nAPType, m_pPacketCache));

    //ǰ����ģ��
    m_WorkModules.push_back(new CFrontStorage(nAPType, m_pPacketCache));

    //����ģ��
    m_WorkModules.push_back(new CNetWork);

    return ERR_SUCCESS;
}


/****************************************************************************
����ԭ��    protected:void CAPMgr::ParseMsg(MSG_INTRA* pMsgParsed)
��������    ������Ϣ֡
�������    pMsgParsed--��Ϣָ֡��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CAPMgr::ParseMsg(MSG_INTRA* pMsgParsed)
{
    switch(pMsgParsed->CmdCode)
    {
    case CMD_AUXUPGRADE:
        BeginAuxUpgrade();
        break;

    case CMD_SENDEMPTYPACKET:
        ReqBPAuxUpgrad();
        break;

    case CMD_ANS_AUXUPGRADE:
        EndAuxUpgrade();
        break;
	case CMD_GET_APNAME:
		{
			//��OM���̵�����ģ�鷢�ͽӵ�����
			MSG_INTRA *pMsg1     = new (strlen(m_szApName) + 1)MSG_INTRA;
			pMsg1->SenderPid     = GetProcessType(); 
			pMsg1->SenderMid     = GetModuleType();
			pMsg1->ReceiverPid   = PT_OM;
			pMsg1->ReceiverMid   = MT_PERF;
			pMsg1->CmdCode       = CMD_AP_NAME;

			memcpy(pMsg1->Data, m_szApName, strlen(m_szApName));
			
			SendMsg(pMsg1);
			break;
		}
    case MML_PRODUCE_CDR:
        ProduceCDR();
        break;       

    default:
        //by ldf 2002.4.10 ��Ӧ���ⵥ D12489
        CModuleManager::ParseMsg(pMsgParsed);
        //�޸����
        break;
    }
}


/****************************************************************************
����ԭ��    protected:void CAPMgr::BeginAuxUpgrade(void)
��������    ��ʼ��������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CAPMgr::BeginAuxUpgrade(void)
{
    MSG_INTRA *pMsg     = new MSG_INTRA;
    pMsg->SenderPid     = PT_SELF; 
    pMsg->SenderMid     = MT_MANAGER;
    pMsg->ReceiverPid   = PT_SELF;
    pMsg->ReceiverMid   = MT_NETWORK;
    pMsg->CmdCode       = CMD_AUXUPGRADE;
    SendMsg(pMsg);

    TRACE(THISMODULE, S_BEGIN_AUXUPGRADE);
    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, S_BEGIN_AUXUPGRADE);
}
/********�������ɻ���*******************************************************/
void CAPMgr::ProduceCDR(void)
{
    MSG_INTRA *pMsg     = new MSG_INTRA;
    pMsg->SenderPid     = PT_SELF; 
    pMsg->SenderMid     = MT_MANAGER;
    pMsg->ReceiverPid   = PT_SELF;
    pMsg->ReceiverMid   = MT_BACKSAVE;
    pMsg->CmdCode       = MML_PRODUCE_CDR;
    SendMsg(pMsg);
    
    TRACE(THISMODULE, GetStringFromID(5002));
    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, GetStringFromID(5002));
}
/****************************************************************************
����ԭ��    protected:void CAPMgr::ReqBPAuxUpgrad(void)
��������    ���󻰵�����ģ�鿪ʼ���տհ�
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CAPMgr::ReqBPAuxUpgrad(void)
{
    MSG_INTRA *pMsg     = new MSG_INTRA;
    pMsg->SenderPid     = PT_SELF; 
    pMsg->SenderMid     = MT_MANAGER;
    pMsg->ReceiverPid   = PT_SELF;
    pMsg->ReceiverMid   = MT_BILLPROCESSOR;
    pMsg->CmdCode       = CMD_AUXUPGRADE;
    SendMsg(pMsg);

    TRACE(THISMODULE, S_REQ_BP_AUXUPGRADE);
    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, S_REQ_BP_AUXUPGRADE);
}


/****************************************************************************
����ԭ��    protected:void CAPMgr::EndAuxUpgrade(void)
��������    ������������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CAPMgr::EndAuxUpgrade(void)
{
    MSG_INTRA *pMsg     = new MSG_INTRA;
    pMsg->SenderPid     = GetProcessType(); 
    pMsg->SenderMid     = MT_MANAGER;
    pMsg->ReceiverPid   = PT_KERNEL;
    pMsg->ReceiverMid   = MT_MONITOR;
    pMsg->CmdCode       = CMD_ANS_AUXUPGRADE;
    SendMsg(pMsg);

    TRACE(THISMODULE, S_END_AUXUPGRADE);
    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, S_END_AUXUPGRADE);
}


/****************************************************************************
����ԭ��    protected:int CAPMgr::handle_signal(int signum, 
                                               siginfo_t* st,
                                               ucontext_t* ut)
��������    �����ź�
�������    signum---�ź���
�������    ��
����ֵ      0     ---�ɹ�
            ����ֵ---ʧ��
�׳��쳣    ��
*****************************************************************************/
int CAPMgr::handle_signal(int signum, 
                          siginfo_t* st,
                          ucontext_t* ut)
{
	//by ldf �������ⵥ D11610  2002.3.14�޸�
    if(( signum < 0) || ( signum > 36))
    {
        TRACE(THISMODULE, S_RECV_UNKNOW_SIGNAL, signum);
        return 0;
    }
	
	m_nRecvSignal = signum;
	
	//�������Ҫ�˳����źţ���ע���Ը��źŵĲ���
	if (g_nSignalToExit[m_nRecvSignal])
	{
		return -1;
	}
    return 0;
}

/****************************************************************************
����ԭ��    protected:void CAPMgr::OnTimer(const long nTimerID)
��������    ��ʱ������
�������    nTimerID---��ʱ��ID
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CAPMgr::OnTimer(const long nTimerID)
{
    if(nTimerID == m_lTimerIDBase)
    {
		//by ldf �������ⵥ D11610  2002.3.14�޸�
		if ((m_nRecvSignal > 0) && (m_nRecvSignal <= 36))
		{
			if (g_nSignalToExit[m_nRecvSignal])
			//��Ҫ�˳����ź�
			{
				TRACE(THISMODULE, S_RECV_SIGNAL_EXIT, 
					g_szSignalDesp[m_nRecvSignal]);
				
				SendQuitToMe();
			}
			else
			//���Ժ��Ե��ź�
			{
				TRACE(THISMODULE, S_RECV_SIGNAL_IGNORED,
					g_szSignalDesp[m_nRecvSignal]);
			}
		}
		else if (m_nRecvSignal != 0)
		{
			TRACE(THISMODULE, S_RECV_SIGNAL_DEFAULT, m_nRecvSignal);
		}	    	
        m_nRecvSignal = 0;
	}
	
	CModuleManager::OnTimer(nTimerID);
}
