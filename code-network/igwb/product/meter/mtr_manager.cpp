#include "mtr_manager.h"
#include "../../com/utility/frame_adapter.h"
#include "../../com/toolbase/frame_tool.h"
#include "../../com/meteracc/meter_acc.h"
#include "../../com/include/signal_tab.h"
#include "../../com/include/resource.h"

/****************************************************************************
函数原型	public:CMeterAccMgr::CMeterAccMgr()
功能描述	构造函数
输入参数	无
输出参数	无
返回值	    无
抛出异常	无
*****************************************************************************/
CMeterAccMgr::CMeterAccMgr()
{
    m_nRecvSignal   = 0;
    m_pFrameAdapter = NULL;
}


/****************************************************************************
函数原型	public:CMeterAccMgr::~CMeterAccMgr()
功能描述	析构函数
输入参数	无
输出参数	无
返回值	    无
抛出异常	无
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
函数原型	protected:int CMeterAccMgr::InitInstance(void)
功能描述	初始化函数
输入参数	无
输出参数	无
返回值	    ERR_SUCCESS---成功
            ERR_FAIL   ---失败
抛出异常	无
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
函数原型    protected:int CMeterAccMgr::ExitInstance(void)
功能描述    清理函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CMeterAccMgr::ExitInstance(void)
{
    CFrameTools::SetInstance(NULL);
    CModuleManager::ExitInstance();
}


/****************************************************************************
函数原型	protected:int CMeterAccMgr::CreateWorkModules(void)
功能描述	创建子模块线程
输入参数	无
输出参数	无
返回值	    ERR_SUCCESS---成功
            ERR_FAIL   ---失败
抛出异常	无
*****************************************************************************/
int CMeterAccMgr::CreateWorkModules(void)
{
    //远程配置模块
    m_WorkModules.push_back(new CMeterAcc);

    return ERR_SUCCESS;
}


/****************************************************************************
函数原型	protected:int CMeterAccMgr::handle_signal(int signum, 
                                                 siginfo_t* st,
                                                 ucontext_t* ut)
功能描述	捕获信号
输入参数	signum---信号码
输出参数	无
返回值	    0     ---成功
            非零值---失败
抛出异常	无
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
	
	//如果是需要退出的信号，则注销对该信号的捕获
	if (g_nSignalToExit[m_nRecvSignal])
	{
        return -1;
	}
	
	return 0;
}

/****************************************************************************
函数原型    protected:void CMeterAccMgr::OnTimer(const long nTimerID)
功能描述    定时器函数
输入参数    nTimerID---定时器ID
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CMeterAccMgr::OnTimer(const long nTimerID)
{
    if (nTimerID == m_lTimerIDBase)
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
