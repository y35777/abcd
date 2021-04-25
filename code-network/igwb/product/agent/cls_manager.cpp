
#include "cls_manager.h"
#include "../../com/include/toolbox.h"
#include "../../com/utility/frame_adapter.h"
#include "../../com/toolbase/frame_tool.h"
#ifdef _PLATFORM_WIN32

#include "../../com/frame/wrk_module.h"
#include "../../com/clustermanager/cluster_module.h"

#else

#include "../../com/agent/cluster_agent.h"

#endif
#include "../../com/include/signal_tab.h"

/****************************************************************************
����ԭ��    public:CClstrMgr::CClstrMgr()
��������    ���캯��
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
CClstrMgr::CClstrMgr()
{
	//by ldf �������ⵥ D11610  2002.3.14�޸�
    m_nRecvSignal   = 0;
    m_pFrameAdapter = NULL;
}


/****************************************************************************
����ԭ��    public:CClstrMgr::~CClstrMgr()
��������    ��������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
CClstrMgr::~CClstrMgr()
{
    if(m_pFrameAdapter)
    {
        delete m_pFrameAdapter;
        m_pFrameAdapter = NULL;
    }
}


/****************************************************************************
����ԭ��    protected:int CClstrMgr::InitInstance(void)
��������    ��ʼ������
�������    ��
�������    ��
����ֵ      ERR_SUCCESS---�ɹ�
            ERR_FAIL   ---ʧ��
�׳��쳣    ��
*****************************************************************************/
int CClstrMgr::InitInstance(void)
{
    int nRet = CModuleManager::InitInstance();
    if(nRet != ERR_SUCCESS) return nRet;

	//by ldf �������ⵥ D11610  2002.3.14�޸�
    ACE_Sig_Set sigset;
    sigset.fill_set();
    sigset.sig_del(SIGTERM);
    sigset.sig_del(SIGCLD);
    
    reactor()->register_handler(sigset, this, 0);

    m_pFrameAdapter = new CFrameAdapter;
    CFrameTools::SetInstance(m_pFrameAdapter);

    return 0;
}

/****************************************************************************
����ԭ��    protected:int CClstrMgr::ExitInstance(void)
��������    ������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CClstrMgr::ExitInstance(void)
{
    CFrameTools::SetInstance(NULL);
    CModuleManager::ExitInstance();
}

/****************************************************************************
����ԭ��    protected:int CClstrMgr::CreateWorkModules(void)
��������    ������ģ���߳�
�������    ��
�������    ��
����ֵ      ERR_SUCCESS---�ɹ�
            ERR_FAIL   ---ʧ��
�׳��쳣    ��
*****************************************************************************/
int CClstrMgr::CreateWorkModules(void)
{
    //�����ò���
    CINIFile* pINIFile = new CINIFile(GetCfgFilePath());
    pINIFile->Open();

    //�澯��ⶨʱ�������ƹ涨Agentÿ��m_uAlarmInterval
    //ʱ�������һ���йصĸ澯��
    BOOL bNoCluster;
    int nNoCluster = pINIFile->GetInt(
                          CFG_SEC_COMMON,
                          CFG_COMMON_KEY_NO_CLUSTER,
                          CFG_COMMON_KEY_NO_CLUSTER_DEFAULT);

    int nClusterMode = pINIFile->GetInt(
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
    
    pINIFile->Close();
    delete pINIFile;

#ifdef _PLATFORM_WIN32

    m_WorkModules.push_back(new CClusterModule);

#elif defined(_PLATFORM_SOLARIS)

    m_WorkModules.push_back(new CClusterAgent(bNoCluster));

#else

    m_WorkModules.push_back(new CClusterAgent(true));

#endif

    if (ActiveWorkModules()) return ERR_SUCCESS;

    return ERR_FAIL;
}


/****************************************************************************
����ԭ��    protected:int CClstrMgr::handle_signal(int signum, 
                                                   siginfo_t* st,
                                                   ucontext_t* ut)
��������    �����ź�
�������    signum---�ź���
�������    ��
����ֵ      0     ---�ɹ�
            ����ֵ---ʧ��
�׳��쳣    ��
*****************************************************************************/
int CClstrMgr::handle_signal(int signum, 
                             siginfo_t* st,
                             ucontext_t* ut)
{
	//by ldf �������ⵥ D11610  2002.3.14�޸�
    if(( signum > 0) || ( signum <= 36))
    {
        m_nRecvSignal = signum;

    }
    
    return 0;
}

/****************************************************************************
����ԭ��    protected:void CClstrMgr::OnTimer(const long nTimerID)
��������    ��ʱ������
�������    nTimerID---��ʱ��ID
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CClstrMgr::OnTimer(const long nTimerID)
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
/****************************************************************************
����ԭ��    protected:void CAPMgr::ParseMsg(MSG_INTRA* pMsgParsed)
��������    ������Ϣ֡
�������    pMsgParsed--��Ϣָ֡��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CClstrMgr::ParseMsg(MSG_INTRA* pMsgParsed)
{
    switch(pMsgParsed->CmdCode)
    {
    case CMD_ANS_CLSTR_STATUS:
		{
			MSG_INTRA *pMsg = new MSG_INTRA;

			pMsg->SenderPid   = GetProcessType(); 
			pMsg->SenderMid   = GetModuleType();
			pMsg->ReceiverPid = PT_KERNEL;
			pMsg->ReceiverMid = MT_MONITOR;
			pMsg->CmdCode     = CMD_ANS_CLSTR_STATUS;

			SendMsg(pMsg);

			break;
		}
    default:        
        CModuleManager::ParseMsg(pMsgParsed);
        break;
    }
}
