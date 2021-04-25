#include "cfg_manager.h"

#include "../../com/mml/mml_svr.h"
#include "../../com/cfgsvr/cfg_svr.h"
#include "../../com/include/signal_tab.h"
#include "../../com/utility/frame_adapter.h"
#include "../../com/toolbase/frame_tool.h"

/****************************************************************************
����ԭ��	public:CCfgMger::CCfgMger()
��������	���캯��
�������	��
�������	��
����ֵ	    ��
�׳��쳣	��
*****************************************************************************/
CCfgMger::CCfgMger()
{
	//by ldf �������ⵥ D11610  2002.3.14�޸�
    m_nRecvSignal   = 0;
    m_pFrameAdapter = NULL;
}


/****************************************************************************
����ԭ��	public:CCfgMger::~CCfgMger()
��������	��������
�������	��
�������	��
����ֵ	    ��
�׳��쳣	��
*****************************************************************************/
CCfgMger::~CCfgMger()
{
    if(m_pFrameAdapter)
    {
        delete m_pFrameAdapter;
        m_pFrameAdapter = NULL;
    }
}


/****************************************************************************
����ԭ��	protected:int CCfgMger::InitInstance(void)
��������	��ʼ������
�������	��
�������	��
����ֵ	    ERR_SUCCESS---�ɹ�
            ERR_FAIL   ---ʧ��
�׳��쳣	��
*****************************************************************************/
int CCfgMger::InitInstance(void)
{
    int nRet = CModuleManager::InitInstance();
    if (nRet != ERR_SUCCESS) return nRet;

	//by ldf �������ⵥ D11610  2002.3.14�޸�
    ACE_Sig_Set sigset;
    sigset.fill_set();
    reactor()->register_handler(sigset, this, 0);

    m_pFrameAdapter = new CFrameAdapter;
    CFrameTools::SetInstance(m_pFrameAdapter);

	ActiveWorkModules();

    return ERR_SUCCESS;
}

/****************************************************************************
����ԭ��    protected:int CCfgMger::ExitInstance(void)
��������    ������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CCfgMger::ExitInstance(void)
{
    CFrameTools::SetInstance(NULL);
    CModuleManager::ExitInstance();
}

/****************************************************************************
����ԭ��	protected:int CCfgMger::CreateWorkModules(void)
��������	������ģ���߳�
�������	��
�������	��
����ֵ	    ERR_SUCCESS---�ɹ�
            ERR_FAIL   ---ʧ��
�׳��쳣	��
*****************************************************************************/
int CCfgMger::CreateWorkModules(void)
{
    //MML Serverģ��
    m_WorkModules.push_back(new CMMLServer);
    //Զ������ģ��
    m_WorkModules.push_back(new CCfgSvr);

    return ERR_SUCCESS;
}


/****************************************************************************
����ԭ��	protected:int CCfgMger::handle_signal(int signum, 
                                                 siginfo_t* st,
                                                 ucontext_t* ut)
��������	�����ź�
�������	signum---�ź���
�������	��
����ֵ	    0     ---�ɹ�
            ����ֵ---ʧ��
�׳��쳣	��
*****************************************************************************/
int CCfgMger::handle_signal(int signum, 
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
����ԭ��    protected:void CCfgMger::OnTimer(const long nTimerID)
��������    ��ʱ������
�������    nTimerID---��ʱ��ID
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CCfgMger::OnTimer(const long nTimerID)
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
