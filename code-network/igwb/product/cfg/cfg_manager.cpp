#include "cfg_manager.h"

#include "../../com/mml/mml_svr.h"
#include "../../com/cfgsvr/cfg_svr.h"
#include "../../com/include/signal_tab.h"
#include "../../com/utility/frame_adapter.h"
#include "../../com/toolbase/frame_tool.h"

/****************************************************************************
函数原型	public:CCfgMger::CCfgMger()
功能描述	构造函数
输入参数	无
输出参数	无
返回值	    无
抛出异常	无
*****************************************************************************/
CCfgMger::CCfgMger()
{
	//by ldf 根据问题单 D11610  2002.3.14修改
    m_nRecvSignal   = 0;
    m_pFrameAdapter = NULL;
}


/****************************************************************************
函数原型	public:CCfgMger::~CCfgMger()
功能描述	析构函数
输入参数	无
输出参数	无
返回值	    无
抛出异常	无
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
函数原型	protected:int CCfgMger::InitInstance(void)
功能描述	初始化函数
输入参数	无
输出参数	无
返回值	    ERR_SUCCESS---成功
            ERR_FAIL   ---失败
抛出异常	无
*****************************************************************************/
int CCfgMger::InitInstance(void)
{
    int nRet = CModuleManager::InitInstance();
    if (nRet != ERR_SUCCESS) return nRet;

	//by ldf 根据问题单 D11610  2002.3.14修改
    ACE_Sig_Set sigset;
    sigset.fill_set();
    reactor()->register_handler(sigset, this, 0);

    m_pFrameAdapter = new CFrameAdapter;
    CFrameTools::SetInstance(m_pFrameAdapter);

	ActiveWorkModules();

    return ERR_SUCCESS;
}

/****************************************************************************
函数原型    protected:int CCfgMger::ExitInstance(void)
功能描述    清理函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CCfgMger::ExitInstance(void)
{
    CFrameTools::SetInstance(NULL);
    CModuleManager::ExitInstance();
}

/****************************************************************************
函数原型	protected:int CCfgMger::CreateWorkModules(void)
功能描述	创建子模块线程
输入参数	无
输出参数	无
返回值	    ERR_SUCCESS---成功
            ERR_FAIL   ---失败
抛出异常	无
*****************************************************************************/
int CCfgMger::CreateWorkModules(void)
{
    //MML Server模块
    m_WorkModules.push_back(new CMMLServer);
    //远程配置模块
    m_WorkModules.push_back(new CCfgSvr);

    return ERR_SUCCESS;
}


/****************************************************************************
函数原型	protected:int CCfgMger::handle_signal(int signum, 
                                                 siginfo_t* st,
                                                 ucontext_t* ut)
功能描述	捕获信号
输入参数	signum---信号码
输出参数	无
返回值	    0     ---成功
            非零值---失败
抛出异常	无
*****************************************************************************/
int CCfgMger::handle_signal(int signum, 
                           siginfo_t* st,
                           ucontext_t* ut)
{
	//by ldf 根据问题单 D11610  2002.3.14修改
	if(( signum < 0) || ( signum > 36))
    {
        TRACE(THISMODULE, S_RECV_UNKNOW_SIGNAL, signum);
        return 0;
    }
	
	m_nRecvSignal = signum;
	
	//如果是需要退出的信号，则注销对该信号的捕获
	if (g_nSignalToExit[m_nRecvSignal])
	{
        return -1;
	}
	
	return 0;
}

/****************************************************************************
函数原型    protected:void CCfgMger::OnTimer(const long nTimerID)
功能描述    定时器函数
输入参数    nTimerID---定时器ID
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CCfgMger::OnTimer(const long nTimerID)
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
