
#include "co_manager.h"
#include "../../com/utility/frame_adapter.h"
#include "../../com/toolbase/frame_tool.h"

#include "../../com/include/toolbox.h"
#include "../../com/include/signal_tab.h"
#include "../../com/collector/collector.h"
#include "../../com/colconvert/collect_convertor.h"
#include "resource.h"

/****************************************************************************
函数原型    public:CCollectorMgr::CCollectorMgr()
功能描述    构造函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
CCollectorMgr::CCollectorMgr()
{
    //by ldf 根据问题单 D11610  2002.3.14修改
    m_nRecvSignal   = 0;
    m_pFrameAdapter = NULL;
}


/****************************************************************************
函数原型    public:CCollectorMgr::~CCollectorMgr()
功能描述    析构函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
CCollectorMgr::~CCollectorMgr()
{
    if(m_pFrameAdapter)
    {
        delete m_pFrameAdapter;
        m_pFrameAdapter = NULL;
    }
}


/****************************************************************************
函数原型    protected:int CCollectorMgr::InitInstance(void)
功能描述    初始化函数
输入参数    无
输出参数    无
返回值      ERR_SUCCESS---成功
            ERR_FAIL   ---失败
抛出异常    无
*****************************************************************************/
int CCollectorMgr::InitInstance(void)
{
    int nRet = CModuleManager::InitInstance();
    if (nRet != ERR_SUCCESS) return nRet;

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
函数原型    protected:int CCollectorMgr::ExitInstance(void)
功能描述    清理函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CCollectorMgr::ExitInstance(void)
{
    CFrameTools::SetInstance(NULL);
    CModuleManager::ExitInstance();
}

/****************************************************************************
函数原型    protected:int CCollectorMgr::CreateWorkModules(void)
功能描述    创建子模块线程
输入参数    无
输出参数    无
返回值      ERR_SUCCESS---成功
            ERR_FAIL   ---失败
抛出异常    无
*****************************************************************************/
int CCollectorMgr::CreateWorkModules(void)
{
    //采集模块
    m_WorkModules.push_back(new CCollectorModule());

    //added by chenliangwei
    //采集转换模块
    m_WorkModules.push_back(new CCollectConvertorModule());
    //added end

    return ERR_SUCCESS;
}

/****************************************************************************
函数原型    protected:void CCollectorMgr::ParseMsg(MSG_INTRA* pMsgParsed)
功能描述    解析消息帧
输入参数    pMsgParsed--消息帧指针
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CCollectorMgr::ParseMsg(MSG_INTRA* pMsgParsed)
{
    switch(pMsgParsed->CmdCode)
    {
    case CMD_AUXUPGRADE:
        BeginAuxUpgrade();
        break;

    case CMD_ANS_AUXUPGRADE:
        EndAuxUpgrade();
        break;

    default:
        CModuleManager::ParseMsg(pMsgParsed);
        break;
    }
}


/****************************************************************************
函数原型    protected:void CCollectorMgr::BeginAuxUpgrade(void)
功能描述    开始辅助升级
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CCollectorMgr::BeginAuxUpgrade(void)
{
    MSG_INTRA *pMsg     = new MSG_INTRA;
    pMsg->SenderPid     = PT_SELF; 
    pMsg->SenderMid     = MT_MANAGER;
    pMsg->ReceiverPid   = PT_SELF;
    pMsg->ReceiverMid   = MT_COLLECTOR;
    pMsg->CmdCode       = CMD_AUXUPGRADE;
    SendMsg(pMsg);

    TRACE(THISMODULE, S_BEGIN_AUXUPGRADE);
    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, S_BEGIN_AUXUPGRADE);
}


/****************************************************************************
函数原型    protected:void CCollectorMgr::EndAuxUpgrade(void)
功能描述    辅助升级结束
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CCollectorMgr::EndAuxUpgrade(void)
{
    MSG_INTRA *pMsg     = new MSG_INTRA;
    pMsg->SenderPid     = GetProcessType(); 
    pMsg->SenderMid     = MT_MANAGER;
    pMsg->ReceiverPid   = PT_KERNEL;
    pMsg->ReceiverMid   = MT_MONITOR;
    pMsg->CmdCode       = CMD_ANS_AUXUPGRADE;
    SendMsg(pMsg);

    TRACE(THISMODULE, S_END_AUXUPGRADE);
    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, S_END_AUXUPGRADE);
}


/****************************************************************************
函数原型    protected:int CCollectorMgr::handle_signal(int signum,
                                               siginfo_t* st,
                                               ucontext_t* ut)
功能描述    捕获信号
输入参数    signum---信号码
输出参数    无
返回值      0     ---成功
            非零值---失败
抛出异常    无
*****************************************************************************/
int CCollectorMgr::handle_signal(int signum,
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
函数原型    protected:void CCollectorMgr::OnTimer(const long nTimerID)
功能描述    定时器函数
输入参数    nTimerID---定时器ID
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CCollectorMgr::OnTimer(const long nTimerID)
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
