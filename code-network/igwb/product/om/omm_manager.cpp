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
函数原型	public:COMMger::COMMger()
功能描述	构造函数
输入参数	无
输出参数	无
返回值	    无
抛出异常	无
*****************************************************************************/
COMMger::COMMger()
{
	//by ldf 根据问题单 D11610  2002.3.14修改
    m_nRecvSignal   = 0;
    m_pFrameAdapter = NULL;
}


/****************************************************************************
函数原型	public:COMMger::~COMMger()
功能描述	析构函数
输入参数	无
输出参数	无
返回值	    无
抛出异常	无
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
函数原型	protected:int COMMger::InitInstance(void)
功能描述	初始化函数
输入参数	无
输出参数	无
返回值	    ERR_SUCCESS---成功
            ERR_FAIL   ---失败
抛出异常	无
*****************************************************************************/
int COMMger::InitInstance(void)
{
    int nRet = CModuleManager::InitInstance();
    if (nRet != ERR_SUCCESS) return nRet;

	//by ldf 根据问题单 D11610  2002.3.14修改
    ACE_Sig_Set sigset;
    sigset.fill_set();
    reactor()->register_handler(sigset, this, 0);

    m_pFrameAdapter = new CFrameAdapter;
    CFrameTools::SetInstance(m_pFrameAdapter);

    //向KERNEL进程中的监控模块发初始化应答
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
函数原型    protected:int COMMger::ExitInstance(void)
功能描述    清理函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void COMMger::ExitInstance(void)
{
    CFrameTools::SetInstance(NULL);
    CModuleManager::ExitInstance();
}


/****************************************************************************
函数原型	protected:int COMMger::CreateWorkModules(void)
功能描述	创建子模块线程
输入参数	无
输出参数	无
返回值	    ERR_SUCCESS---成功
            ERR_FAIL   ---失败
抛出异常	无
*****************************************************************************/
int COMMger::CreateWorkModules(void)
{
    //MML Server模块
    m_WorkModules.push_back(new CMMLServer);

    //告警模块
    m_WorkModules.push_back(new CAlarmServer);

    //备份模块
	m_WorkModules.push_back(new CBackupServer);

	//话单统计模块
	m_WorkModules.push_back(new CCDRStatSvr);

    //话单浏览查询模块
    m_WorkModules.push_back(new CBillQBServer);

    //性能模块
    m_WorkModules.push_back(new CPerf);

	//日志模块
    m_WorkModules.push_back(new CLog);

#ifdef _PRODUCT_CCS
    //采集代理模块
    m_WorkModules.push_back(new CCollectorAgent);
#elif defined(_PRODUCT_NGN)
	//SCP话单恢复模块
    m_WorkModules.push_back(new CBillTaskServer);
#else
#endif

    return ERR_SUCCESS;
}


/****************************************************************************
函数原型	protected:int COMMger::handle_signal(int signum,
                                                 siginfo_t* st,
                                                 ucontext_t* ut)
功能描述	捕获信号
输入参数	signum---信号码
输出参数	无
返回值	    0     ---成功
            非零值---失败
抛出异常	无
*****************************************************************************/
int COMMger::handle_signal(int signum,
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
函数原型    protected:void COMMger::OnTimer(const long nTimerID)
功能描述    定时器函数
输入参数    nTimerID---定时器ID
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void COMMger::OnTimer(const long nTimerID)
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
