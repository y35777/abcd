///////////////////////////////////////////////////////////////////////////////
//GTP'�Ʒѽӿ����� Z30971 2004/06

#ifdef _PLATFORM_WIN32
#include "../../com/include/product.h"
#endif //_PLATFORM_WIN32

#include "bs_manager.h"
#include "../../com/cdrsender/cdrsnd_svr.h"

#include "../../com/include/signal_tab.h"

/****************************************************************************
����ԭ��	public:CBSMger::CBSMger()
��������	���캯��
�������	��
�������	��
����ֵ	    ��
�׳��쳣	��
*****************************************************************************/
CBSMger::CBSMger()
{
    m_nRecvSignal = 0;
}

/****************************************************************************
����ԭ��	public:CBSMger::~CBSMger()
��������	��������
�������	��
�������	��
����ֵ	    ��
�׳��쳣	��
*****************************************************************************/
CBSMger::~CBSMger()
{
    //
}
/****************************************************************************
����ԭ��	protected:int CBSMger::InitInstance(void)
��������	��ʼ������
�������	��
�������	��
����ֵ	    ERR_SUCCESS---�ɹ�
            ERR_FAIL   ---ʧ��
�׳��쳣	��
*****************************************************************************/
int CBSMger::InitInstance(void)
{
    int nRet = CModuleManager::InitInstance();
    if (nRet != ERR_SUCCESS) return nRet;

    ACE_Sig_Set sigset;
    sigset.fill_set();
    reactor()->register_handler(sigset, this, 0);

    ActiveWorkModules();

    return ERR_SUCCESS;
}


/****************************************************************************
����ԭ��	protected:int CBSMger::CreateWorkModules(void)
��������	������ģ���߳�
�������	��
�������	��
����ֵ	    ERR_SUCCESS---�ɹ�
            ERR_FAIL   ---ʧ��
�׳��쳣	��
*****************************************************************************/
int CBSMger::CreateWorkModules(void)
{
    //GTP' �Ʒѽӿ�ģ��
    m_WorkModules.push_back(new CCDRSndSvr);

    return ERR_SUCCESS;
}


/****************************************************************************
����ԭ��	protected:int CBSMger::handle_signal(int signum,
                                                 siginfo_t* st,
                                                 ucontext_t* ut)
��������	�����ź�
�������	signum---�ź���
�������	��
����ֵ	    0     ---�ɹ�
            ����ֵ---ʧ��
�׳��쳣	��
*****************************************************************************/
int CBSMger::handle_signal(int signum,
                           siginfo_t* st,
                           ucontext_t* ut)
{
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
����ԭ��    protected:void CBSMger::OnTimer(const long nTimerID)
��������    ��ʱ������
�������    nTimerID---��ʱ��ID
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CBSMger::OnTimer(const long nTimerID)
{
    if(nTimerID == m_lTimerIDBase)
    {
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
