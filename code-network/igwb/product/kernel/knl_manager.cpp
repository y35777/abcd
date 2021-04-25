#include "knl_manager.h"
#include "../../com/utility/frame_adapter.h"
#include "../../com/toolbase/frame_tool.h"
#include "../../com/clusterinterface/cluster_interface.h"
#include "../../com/monitor/monitor.h"
#include "../../com/licensemanager/license_manager.h"
#include "../../com/include/signal_tab.h"

/****************************************************************************
����ԭ��    public:CKenelMgr::CKenelMgr()
��������    ���캯��
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
CKenelMgr::CKenelMgr()
{
	//by ldf �������ⵥ D11610  2002.3.14�޸�
    m_nRecvSignal   = 0;
    m_pFrameAdapter = NULL;
}


/****************************************************************************
����ԭ��    public:CKenelMgr::~CKenelMgr()
��������    ��������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
CKenelMgr::~CKenelMgr()
{
    if(m_pFrameAdapter)
    {
        delete m_pFrameAdapter;
        m_pFrameAdapter = NULL;
    }
}


/****************************************************************************
����ԭ��    protected:int CKenelMgr::InitInstance(void)
��������    ��ʼ������
�������    ��
�������    ��
����ֵ      ERR_SUCCESS---�ɹ�
            ERR_FAIL   ---ʧ��
�׳��쳣    ��
*****************************************************************************/
int CKenelMgr::InitInstance(void)
{
    int nRet = CModuleManager::InitInstance();
    if(nRet != ERR_SUCCESS) return nRet;

	//by ldf �������ⵥ D11610  2002.3.14�޸�
    ACE_Sig_Set sigset;
    sigset.fill_set();
    
    reactor()->register_handler(sigset, this, 0);

    m_pFrameAdapter = new CFrameAdapter;
    CFrameTools::SetInstance(m_pFrameAdapter);

    return 0;
}


/****************************************************************************
����ԭ��    protected:int CKenelMgr::ExitInstance(void)
��������    ������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CKenelMgr::ExitInstance(void)
{
    CFrameTools::SetInstance(NULL);
    CModuleManager::ExitInstance();
}


/****************************************************************************
����ԭ��    protected:int CKenelMgr::CreateWorkModules(void)
��������    ������ģ���߳�
�������    ��
�������    ��
����ֵ      ERR_SUCCESS---�ɹ�
            ERR_FAIL   ---ʧ��
�׳��쳣    ��
*****************************************************************************/
int CKenelMgr::CreateWorkModules(void)
{
    //���ģ��
    m_WorkModules.push_back(new CMonitor);

    //˫���ӿ�ģ��
    m_WorkModules.push_back(new CClusterInterface);

	//License Manager
	m_WorkModules.push_back(new CLicenseManager);

    if (ActiveWorkModules()) return ERR_SUCCESS;

    return ERR_FAIL;
}


/****************************************************************************
����ԭ��    protected:int CKenelMgr::handle_signal(int signum, 
                                                   siginfo_t* st,
                                                   ucontext_t* ut)
��������    �����ź�
�������    signum---�ź���
�������    ��
����ֵ      0     ---�ɹ�
            ����ֵ---ʧ��
�׳��쳣    ��
*****************************************************************************/
int CKenelMgr::handle_signal(int signum, 
                             siginfo_t* st,
                             ucontext_t* ut)
{
	//by ldf �������ⵥ D11610  2002.3.14�޸�
    if(( signum > 0) && ( signum <= 36))
    {
    	m_nRecvSignal = signum;
    }

    return 0;
}

/****************************************************************************
����ԭ��    protected:void CKenelMgr::OnTimer(const long nTimerID)
��������    ��ʱ������
�������    nTimerID---��ʱ��ID
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CKenelMgr::OnTimer(const long nTimerID)
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
