
#include "cls_manager.h"
#include "../../com/include/toolbox.h"
#include "../../com/utility/frame_adapter.h"
#include "../../com/toolbase/frame_tool.h"
#ifdef _PLATFORM_WIN32

#include "../../com/frame/wrk_module.h"
#include "../../com/clustermanager/cluster_module.h"

#else

#include "../../com/agent/cluster_agent.h"

#endif
#include "../../com/include/signal_tab.h"

/****************************************************************************
函数原型    public:CClstrMgr::CClstrMgr()
功能描述    构造函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
CClstrMgr::CClstrMgr()
{
	//by ldf 根据问题单 D11610  2002.3.14修改
    m_nRecvSignal   = 0;
    m_pFrameAdapter = NULL;
}


/****************************************************************************
函数原型    public:CClstrMgr::~CClstrMgr()
功能描述    析构函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
CClstrMgr::~CClstrMgr()
{
    if(m_pFrameAdapter)
    {
        delete m_pFrameAdapter;
        m_pFrameAdapter = NULL;
    }
}


/****************************************************************************
函数原型    protected:int CClstrMgr::InitInstance(void)
功能描述    初始化函数
输入参数    无
输出参数    无
返回值      ERR_SUCCESS---成功
            ERR_FAIL   ---失败
抛出异常    无
*****************************************************************************/
int CClstrMgr::InitInstance(void)
{
    int nRet = CModuleManager::InitInstance();
    if(nRet != ERR_SUCCESS) return nRet;

	//by ldf 根据问题单 D11610  2002.3.14修改
    ACE_Sig_Set sigset;
    sigset.fill_set();
    sigset.sig_del(SIGTERM);
    sigset.sig_del(SIGCLD);
    
    reactor()->register_handler(sigset, this, 0);

    m_pFrameAdapter = new CFrameAdapter;
    CFrameTools::SetInstance(m_pFrameAdapter);

    return 0;
}

/****************************************************************************
函数原型    protected:int CClstrMgr::ExitInstance(void)
功能描述    清理函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CClstrMgr::ExitInstance(void)
{
    CFrameTools::SetInstance(NULL);
    CModuleManager::ExitInstance();
}

/****************************************************************************
函数原型    protected:int CClstrMgr::CreateWorkModules(void)
功能描述    创建子模块线程
输入参数    无
输出参数    无
返回值      ERR_SUCCESS---成功
            ERR_FAIL   ---失败
抛出异常    无
*****************************************************************************/
int CClstrMgr::CreateWorkModules(void)
{
    //读配置参数
    CINIFile* pINIFile = new CINIFile(GetCfgFilePath());
    pINIFile->Open();

    //告警检测定时间隔，设计规定Agent每隔m_uAlarmInterval
    //时间间隔检测一次有关的告警项
    BOOL bNoCluster;
    int nNoCluster = pINIFile->GetInt(
                          CFG_SEC_COMMON,
                          CFG_COMMON_KEY_NO_CLUSTER,
                          CFG_COMMON_KEY_NO_CLUSTER_DEFAULT);

    int nClusterMode = pINIFile->GetInt(
                          CFG_SEC_COMMON,
                          CFG_COMMON_KEY_CLUSTER_MODE,
                          CFG_COMMON_KEY_CLUSTER_MODE_TEST);
    if(nClusterMode == CFG_COMMON_KEY_CLUSTER_MODE_TEST)
    //ClusterMode未配置
    {
        if(nNoCluster == CFG_COMMON_KEY_NO_CLUSTER_DEFAULT)
        //NoCluster未配置
        {
            bNoCluster = !CFG_COMMON_KEY_CLUSTER_MODE_DEFAULT;
        }
        else
        {
            bNoCluster = nNoCluster;
        }
    }
    else
    {
        bNoCluster = !nClusterMode;
    }
    
    pINIFile->Close();
    delete pINIFile;

#ifdef _PLATFORM_WIN32

    m_WorkModules.push_back(new CClusterModule);

#elif defined(_PLATFORM_SOLARIS)

    m_WorkModules.push_back(new CClusterAgent(bNoCluster));

#else

    m_WorkModules.push_back(new CClusterAgent(true));

#endif

    if (ActiveWorkModules()) return ERR_SUCCESS;

    return ERR_FAIL;
}


/****************************************************************************
函数原型    protected:int CClstrMgr::handle_signal(int signum, 
                                                   siginfo_t* st,
                                                   ucontext_t* ut)
功能描述    捕获信号
输入参数    signum---信号码
输出参数    无
返回值      0     ---成功
            非零值---失败
抛出异常    无
*****************************************************************************/
int CClstrMgr::handle_signal(int signum, 
                             siginfo_t* st,
                             ucontext_t* ut)
{
	//by ldf 根据问题单 D11610  2002.3.14修改
    if(( signum > 0) || ( signum <= 36))
    {
        m_nRecvSignal = signum;

    }
    
    return 0;
}

/****************************************************************************
函数原型    protected:void CClstrMgr::OnTimer(const long nTimerID)
功能描述    定时器函数
输入参数    nTimerID---定时器ID
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CClstrMgr::OnTimer(const long nTimerID)
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
/****************************************************************************
函数原型    protected:void CAPMgr::ParseMsg(MSG_INTRA* pMsgParsed)
功能描述    解析消息帧
输入参数    pMsgParsed--消息帧指针
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CClstrMgr::ParseMsg(MSG_INTRA* pMsgParsed)
{
    switch(pMsgParsed->CmdCode)
    {
    case CMD_ANS_CLSTR_STATUS:
		{
			MSG_INTRA *pMsg = new MSG_INTRA;

			pMsg->SenderPid   = GetProcessType(); 
			pMsg->SenderMid   = GetModuleType();
			pMsg->ReceiverPid = PT_KERNEL;
			pMsg->ReceiverMid = MT_MONITOR;
			pMsg->CmdCode     = CMD_ANS_CLSTR_STATUS;

			SendMsg(pMsg);

			break;
		}
    default:        
        CModuleManager::ParseMsg(pMsgParsed);
        break;
    }
}
