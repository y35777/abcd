#include "knl_manager.h"
#include "../../com/utility/frame_adapter.h"
#include "../../com/toolbase/frame_tool.h"
#include "../../com/clusterinterface/cluster_interface.h"
#include "../../com/monitor/monitor.h"
#include "../../com/licensemanager/license_manager.h"
#include "../../com/include/signal_tab.h"

/****************************************************************************
函数原型    public:CKenelMgr::CKenelMgr()
功能描述    构造函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
CKenelMgr::CKenelMgr()
{
	//by ldf 根据问题单 D11610  2002.3.14修改
    m_nRecvSignal   = 0;
    m_pFrameAdapter = NULL;
}


/****************************************************************************
函数原型    public:CKenelMgr::~CKenelMgr()
功能描述    析构函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
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
函数原型    protected:int CKenelMgr::InitInstance(void)
功能描述    初始化函数
输入参数    无
输出参数    无
返回值      ERR_SUCCESS---成功
            ERR_FAIL   ---失败
抛出异常    无
*****************************************************************************/
int CKenelMgr::InitInstance(void)
{
    int nRet = CModuleManager::InitInstance();
    if(nRet != ERR_SUCCESS) return nRet;

	//by ldf 根据问题单 D11610  2002.3.14修改
    ACE_Sig_Set sigset;
    sigset.fill_set();
    
    reactor()->register_handler(sigset, this, 0);

    m_pFrameAdapter = new CFrameAdapter;
    CFrameTools::SetInstance(m_pFrameAdapter);

    return 0;
}


/****************************************************************************
函数原型    protected:int CKenelMgr::ExitInstance(void)
功能描述    清理函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CKenelMgr::ExitInstance(void)
{
    CFrameTools::SetInstance(NULL);
    CModuleManager::ExitInstance();
}


/****************************************************************************
函数原型    protected:int CKenelMgr::CreateWorkModules(void)
功能描述    创建子模块线程
输入参数    无
输出参数    无
返回值      ERR_SUCCESS---成功
            ERR_FAIL   ---失败
抛出异常    无
*****************************************************************************/
int CKenelMgr::CreateWorkModules(void)
{
    //监控模块
    m_WorkModules.push_back(new CMonitor);

    //双机接口模块
    m_WorkModules.push_back(new CClusterInterface);

	//License Manager
	m_WorkModules.push_back(new CLicenseManager);

    if (ActiveWorkModules()) return ERR_SUCCESS;

    return ERR_FAIL;
}


/****************************************************************************
函数原型    protected:int CKenelMgr::handle_signal(int signum, 
                                                   siginfo_t* st,
                                                   ucontext_t* ut)
功能描述    捕获信号
输入参数    signum---信号码
输出参数    无
返回值      0     ---成功
            非零值---失败
抛出异常    无
*****************************************************************************/
int CKenelMgr::handle_signal(int signum, 
                             siginfo_t* st,
                             ucontext_t* ut)
{
	//by ldf 根据问题单 D11610  2002.3.14修改
    if(( signum > 0) && ( signum <= 36))
    {
    	m_nRecvSignal = signum;
    }

    return 0;
}

/****************************************************************************
函数原型    protected:void CKenelMgr::OnTimer(const long nTimerID)
功能描述    定时器函数
输入参数    nTimerID---定时器ID
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CKenelMgr::OnTimer(const long nTimerID)
{
    if(nTimerID == m_lTimerIDBase)
    {
		//by ldf 根据问题单 D11610  2002.3.14修改
    	if ((m_nRecvSignal > 0) && (m_nRecvSignal <= 36))
    	{
	        if (g_nSignalToExit[m_nRecvSignal])
	        //需要退出的信号
	        {
	            TRACE(THISMODULE, S_RECV_SIGNAL_EXIT, 
	                  g_szSignalDesp[m_nRecvSignal]);
	
	            SendQuitToMe();
	        }
	        else
	        //可以忽略的信号
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
