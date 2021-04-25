#ifdef _PLATFORM_WIN32
#include "../../com/include/product.h"
#endif //_PLATFORM_WIN32

#include "omm_manager.h"
#include "../../com/utility/frame_adapter.h"
#include "../../com/toolbase/frame_tool.h"
#include "../../com/mml/mml_svr.h"
#include "../../com/querybrowse/bill_qb.h"
#include "../../com/backup/backup_server.h"
#include "../../com/alarm/alarm_server.h"
#include "../../com/perf/perf.h"
#include "../../com/log/log.h"
#ifdef _PRODUCT_CCS
#include "../../com/coagent/co_agent.h"
#elif defined(_PRODUCT_NGN)
#include "../../com/billtask/bill_task_server.h"
#else
#endif
#include "../../com/include/signal_tab.h"
#include "../../com/cdrstat/cdr_stat_svr.h"

/****************************************************************************
����ԭ��	public:COMMger::COMMger()
��������	���캯��
�������	��
�������	��
����ֵ	    ��
�׳��쳣	��
*****************************************************************************/
COMMger::COMMger()
{
	//by ldf �������ⵥ D11610  2002.3.14�޸�
    m_nRecvSignal   = 0;
    m_pFrameAdapter = NULL;
}


/****************************************************************************
����ԭ��	public:COMMger::~COMMger()
��������	��������
�������	��
�������	��
����ֵ	    ��
�׳��쳣	��
*****************************************************************************/
COMMger::~COMMger()
{
    if(m_pFrameAdapter)
    {
        delete m_pFrameAdapter;
        m_pFrameAdapter = NULL;
    }
}


/****************************************************************************
����ԭ��	protected:int COMMger::InitInstance(void)
��������	��ʼ������
�������	��
�������	��
����ֵ	    ERR_SUCCESS---�ɹ�
            ERR_FAIL   ---ʧ��
�׳��쳣	��
*****************************************************************************/
int COMMger::InitInstance(void)
{
    int nRet = CModuleManager::InitInstance();
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

    return ERR_SUCCESS;
}


/****************************************************************************
����ԭ��    protected:int COMMger::ExitInstance(void)
��������    ������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void COMMger::ExitInstance(void)
{
    CFrameTools::SetInstance(NULL);
    CModuleManager::ExitInstance();
}


/****************************************************************************
����ԭ��	protected:int COMMger::CreateWorkModules(void)
��������	������ģ���߳�
�������	��
�������	��
����ֵ	    ERR_SUCCESS---�ɹ�
            ERR_FAIL   ---ʧ��
�׳��쳣	��
*****************************************************************************/
int COMMger::CreateWorkModules(void)
{
    //MML Serverģ��
    m_WorkModules.push_back(new CMMLServer);

    //�澯ģ��
    m_WorkModules.push_back(new CAlarmServer);

    //����ģ��
	m_WorkModules.push_back(new CBackupServer);

	//����ͳ��ģ��
	m_WorkModules.push_back(new CCDRStatSvr);

    //���������ѯģ��
    m_WorkModules.push_back(new CBillQBServer);

    //����ģ��
    m_WorkModules.push_back(new CPerf);

	//��־ģ��
    m_WorkModules.push_back(new CLog);

#ifdef _PRODUCT_CCS
    //�ɼ�����ģ��
    m_WorkModules.push_back(new CCollectorAgent);
#elif defined(_PRODUCT_NGN)
	//SCP�����ָ�ģ��
    m_WorkModules.push_back(new CBillTaskServer);
#else
#endif

    return ERR_SUCCESS;
}


/****************************************************************************
����ԭ��	protected:int COMMger::handle_signal(int signum,
                                                 siginfo_t* st,
                                                 ucontext_t* ut)
��������	�����ź�
�������	signum---�ź���
�������	��
����ֵ	    0     ---�ɹ�
            ����ֵ---ʧ��
�׳��쳣	��
*****************************************************************************/
int COMMger::handle_signal(int signum,
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
����ԭ��    protected:void COMMger::OnTimer(const long nTimerID)
��������    ��ʱ������
�������    nTimerID---��ʱ��ID
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void COMMger::OnTimer(const long nTimerID)
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
