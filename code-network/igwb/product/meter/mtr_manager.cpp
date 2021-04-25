#include "mtr_manager.h"
#include "../../com/utility/frame_adapter.h"
#include "../../com/toolbase/frame_tool.h"
#include "../../com/meteracc/meter_acc.h"
#include "../../com/include/signal_tab.h"
#include "../../com/include/resource.h"

/****************************************************************************
����ԭ��	public:CMeterAccMgr::CMeterAccMgr()
��������	���캯��
�������	��
�������	��
����ֵ	    ��
�׳��쳣	��
*****************************************************************************/
CMeterAccMgr::CMeterAccMgr()
{
    m_nRecvSignal   = 0;
    m_pFrameAdapter = NULL;
}


/****************************************************************************
����ԭ��	public:CMeterAccMgr::~CMeterAccMgr()
��������	��������
�������	��
�������	��
����ֵ	    ��
�׳��쳣	��
*****************************************************************************/
CMeterAccMgr::~CMeterAccMgr()
{
    if(m_pFrameAdapter)
    {
        delete m_pFrameAdapter;
        m_pFrameAdapter = NULL;
    }
}


/****************************************************************************
����ԭ��	protected:int CMeterAccMgr::InitInstance(void)
��������	��ʼ������
�������	��
�������	��
����ֵ	    ERR_SUCCESS---�ɹ�
            ERR_FAIL   ---ʧ��
�׳��쳣	��
*****************************************************************************/
int CMeterAccMgr::InitInstance(void)
{
    if (ERR_SUCCESS != CModuleManager::InitInstance())
    {
        return ERR_FAIL;
    }

    ACE_Sig_Set sigset;
    sigset.fill_set();
    reactor()->register_handler(sigset, this, 0);

    m_pFrameAdapter = new CFrameAdapter;
    CFrameTools::SetInstance(m_pFrameAdapter);

	ActiveWorkModules();

    return ERR_SUCCESS;
}


/****************************************************************************
����ԭ��    protected:int CMeterAccMgr::ExitInstance(void)
��������    ������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CMeterAccMgr::ExitInstance(void)
{
    CFrameTools::SetInstance(NULL);
    CModuleManager::ExitInstance();
}


/****************************************************************************
����ԭ��	protected:int CMeterAccMgr::CreateWorkModules(void)
��������	������ģ���߳�
�������	��
�������	��
����ֵ	    ERR_SUCCESS---�ɹ�
            ERR_FAIL   ---ʧ��
�׳��쳣	��
*****************************************************************************/
int CMeterAccMgr::CreateWorkModules(void)
{
    //Զ������ģ��
    m_WorkModules.push_back(new CMeterAcc);

    return ERR_SUCCESS;
}


/****************************************************************************
����ԭ��	protected:int CMeterAccMgr::handle_signal(int signum, 
                                                 siginfo_t* st,
                                                 ucontext_t* ut)
��������	�����ź�
�������	signum---�ź���
�������	��
����ֵ	    0     ---�ɹ�
            ����ֵ---ʧ��
�׳��쳣	��
*****************************************************************************/
int CMeterAccMgr::handle_signal(int signum, 
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
����ԭ��    protected:void CMeterAccMgr::OnTimer(const long nTimerID)
��������    ��ʱ������
�������    nTimerID---��ʱ��ID
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CMeterAccMgr::OnTimer(const long nTimerID)
{
    if (nTimerID == m_lTimerIDBase)
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
