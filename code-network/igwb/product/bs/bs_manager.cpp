///////////////////////////////////////////////////////////////////////////////
//GTP'计费接口增加 Z30971 2004/06

#ifdef _PLATFORM_WIN32
#include "../../com/include/product.h"
#endif //_PLATFORM_WIN32

#include "bs_manager.h"
#include "../../com/cdrsender/cdrsnd_svr.h"

#include "../../com/include/signal_tab.h"

/****************************************************************************
函数原型	public:CBSMger::CBSMger()
功能描述	构造函数
输入参数	无
输出参数	无
返回值	    无
抛出异常	无
*****************************************************************************/
CBSMger::CBSMger()
{
    m_nRecvSignal = 0;
}

/****************************************************************************
函数原型	public:CBSMger::~CBSMger()
功能描述	析构函数
输入参数	无
输出参数	无
返回值	    无
抛出异常	无
*****************************************************************************/
CBSMger::~CBSMger()
{
    //
}
/****************************************************************************
函数原型	protected:int CBSMger::InitInstance(void)
功能描述	初始化函数
输入参数	无
输出参数	无
返回值	    ERR_SUCCESS---成功
            ERR_FAIL   ---失败
抛出异常	无
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
函数原型	protected:int CBSMger::CreateWorkModules(void)
功能描述	创建子模块线程
输入参数	无
输出参数	无
返回值	    ERR_SUCCESS---成功
            ERR_FAIL   ---失败
抛出异常	无
*****************************************************************************/
int CBSMger::CreateWorkModules(void)
{
    //GTP' 计费接口模块
    m_WorkModules.push_back(new CCDRSndSvr);

    return ERR_SUCCESS;
}


/****************************************************************************
函数原型	protected:int CBSMger::handle_signal(int signum,
                                                 siginfo_t* st,
                                                 ucontext_t* ut)
功能描述	捕获信号
输入参数	signum---信号码
输出参数	无
返回值	    0     ---成功
            非零值---失败
抛出异常	无
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

	//如果是需要退出的信号，则注销对该信号的捕获
	if (g_nSignalToExit[m_nRecvSignal])
	{
        return -1;
	}

	return 0;
}

/****************************************************************************
函数原型    protected:void CBSMger::OnTimer(const long nTimerID)
功能描述    定时器函数
输入参数    nTimerID---定时器ID
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CBSMger::OnTimer(const long nTimerID)
{
    if(nTimerID == m_lTimerIDBase)
    {
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
