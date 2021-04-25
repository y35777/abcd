
#include "ap_manager.h"

#include "../../com/include/toolbox.h"
#include "../../com/frontsave/front_stor.h"
#include "../../com/backsave/back_stor.h"
#include "../../com/netbase/network.h"
#include "../../com/convert/convert.h"
#include "resource.h"
#include "../../com/include/signal_tab.h"
#include "../../com/utility/frame_adapter.h"
#include "../../com/toolbase/frame_tool.h"

/****************************************************************************
函数原型    public:CAPMgr::CAPMgr()
功能描述    构造函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
CAPMgr::CAPMgr()
{
	//by ldf 根据问题单 D11610  2002.3.14修改
    m_nRecvSignal   =0;
    m_pFrameAdapter = NULL;
}


/****************************************************************************
函数原型    public:CAPMgr::~CAPMgr()
功能描述    析构函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
CAPMgr::~CAPMgr()
{
    if (m_pPacketCache)
    {
        delete m_pPacketCache;
        m_pPacketCache = NULL;
    }
    
    if(m_pFrameAdapter)
    {
        delete m_pFrameAdapter;
        m_pFrameAdapter = NULL;
    }
}


/****************************************************************************
函数原型    protected:int CAPMgr::InitInstance(void)
功能描述    初始化函数
输入参数    无
输出参数    无
返回值      ERR_SUCCESS---成功
            ERR_FAIL   ---失败
抛出异常    无
*****************************************************************************/
int CAPMgr::InitInstance(void)
{
    int nRet;
    //读取配置文件中的话单包上下限，创建话单包缓冲区对象
    CINIFile *pIniFile = new CINIFile((char *)GetCfgFilePath());
    pIniFile->Open(); 

    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP,
        GetProcessType() - PT_AP_BASE + 1);

    UINT4 uLowLimit = (UINT4)pIniFile->GetInt(strSec,
        CFG_AP_KEY_PACKETLOWLIMIT, 
        CFG_AP_KEY_PACKETLOWLIMIT_DEFAULT);

    UINT4 uHighLimit = (UINT4)pIniFile->GetInt(strSec, 
        CFG_AP_KEY_PACKETHIGHLIMIT, 
        CFG_AP_KEY_PACKETHIGHLIMIT_DEFAULT);

	//读进程编号
	pIniFile->GetString(strSec, CFG_AP_NAME, strSec, m_szApName, 32);
     
    pIniFile->Close();
    delete pIniFile;

    m_pPacketCache = new CPacketCache(uLowLimit, uHighLimit); 

    nRet = CModuleManager::InitInstance();
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

	//向OM进程的性能模块发送接点名称
	MSG_INTRA *pMsg1     = new (strlen(m_szApName) + 1)MSG_INTRA;
    pMsg1->SenderPid     = GetProcessType(); 
    pMsg1->SenderMid     = GetModuleType();
    pMsg1->ReceiverPid   = PT_OM;
    pMsg1->ReceiverMid   = MT_PERF;
    pMsg1->CmdCode       = CMD_AP_NAME;

	memcpy(pMsg1->Data, m_szApName, strlen(m_szApName));
	SendMsg(pMsg1);

    return ERR_SUCCESS;
}


/****************************************************************************
函数原型    protected:int CClstrMgr::ExitInstance(void)
功能描述    清理函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CAPMgr::ExitInstance(void)
{
    CFrameTools::SetInstance(NULL);
    CModuleManager::ExitInstance();
}


/****************************************************************************
函数原型    protected:int CAPMgr::CreateWorkModules(void)
功能描述    创建子模块线程
输入参数    无
输出参数    无
返回值      ERR_SUCCESS---成功
            ERR_FAIL   ---失败
抛出异常    无
*****************************************************************************/
int CAPMgr::CreateWorkModules(void)
{
    int nAPType = GetProcessType() - PT_AP_BASE + 1;
    //后存盘模块
    m_WorkModules.push_back(new CBackStorage(nAPType));

    //话单处理模块
    m_WorkModules.push_back(new CBillProcessor(nAPType, m_pPacketCache));

    //前存盘模块
    m_WorkModules.push_back(new CFrontStorage(nAPType, m_pPacketCache));

    //网络模块
    m_WorkModules.push_back(new CNetWork);

    return ERR_SUCCESS;
}


/****************************************************************************
函数原型    protected:void CAPMgr::ParseMsg(MSG_INTRA* pMsgParsed)
功能描述    解析消息帧
输入参数    pMsgParsed--消息帧指针
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CAPMgr::ParseMsg(MSG_INTRA* pMsgParsed)
{
    switch(pMsgParsed->CmdCode)
    {
    case CMD_AUXUPGRADE:
        BeginAuxUpgrade();
        break;

    case CMD_SENDEMPTYPACKET:
        ReqBPAuxUpgrad();
        break;

    case CMD_ANS_AUXUPGRADE:
        EndAuxUpgrade();
        break;
	case CMD_GET_APNAME:
		{
			//向OM进程的性能模块发送接点名称
			MSG_INTRA *pMsg1     = new (strlen(m_szApName) + 1)MSG_INTRA;
			pMsg1->SenderPid     = GetProcessType(); 
			pMsg1->SenderMid     = GetModuleType();
			pMsg1->ReceiverPid   = PT_OM;
			pMsg1->ReceiverMid   = MT_PERF;
			pMsg1->CmdCode       = CMD_AP_NAME;

			memcpy(pMsg1->Data, m_szApName, strlen(m_szApName));
			
			SendMsg(pMsg1);
			break;
		}
    case MML_PRODUCE_CDR:
        ProduceCDR();
        break;       

    default:
        //by ldf 2002.4.10 对应问题单 D12489
        CModuleManager::ParseMsg(pMsgParsed);
        //修改完毕
        break;
    }
}


/****************************************************************************
函数原型    protected:void CAPMgr::BeginAuxUpgrade(void)
功能描述    开始辅助升级
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CAPMgr::BeginAuxUpgrade(void)
{
    MSG_INTRA *pMsg     = new MSG_INTRA;
    pMsg->SenderPid     = PT_SELF; 
    pMsg->SenderMid     = MT_MANAGER;
    pMsg->ReceiverPid   = PT_SELF;
    pMsg->ReceiverMid   = MT_NETWORK;
    pMsg->CmdCode       = CMD_AUXUPGRADE;
    SendMsg(pMsg);

    TRACE(THISMODULE, S_BEGIN_AUXUPGRADE);
    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, S_BEGIN_AUXUPGRADE);
}
/********立即生成话单*******************************************************/
void CAPMgr::ProduceCDR(void)
{
    MSG_INTRA *pMsg     = new MSG_INTRA;
    pMsg->SenderPid     = PT_SELF; 
    pMsg->SenderMid     = MT_MANAGER;
    pMsg->ReceiverPid   = PT_SELF;
    pMsg->ReceiverMid   = MT_BACKSAVE;
    pMsg->CmdCode       = MML_PRODUCE_CDR;
    SendMsg(pMsg);
    
    TRACE(THISMODULE, GetStringFromID(5002));
    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, GetStringFromID(5002));
}
/****************************************************************************
函数原型    protected:void CAPMgr::ReqBPAuxUpgrad(void)
功能描述    请求话单处理模块开始接收空包
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CAPMgr::ReqBPAuxUpgrad(void)
{
    MSG_INTRA *pMsg     = new MSG_INTRA;
    pMsg->SenderPid     = PT_SELF; 
    pMsg->SenderMid     = MT_MANAGER;
    pMsg->ReceiverPid   = PT_SELF;
    pMsg->ReceiverMid   = MT_BILLPROCESSOR;
    pMsg->CmdCode       = CMD_AUXUPGRADE;
    SendMsg(pMsg);

    TRACE(THISMODULE, S_REQ_BP_AUXUPGRADE);
    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, S_REQ_BP_AUXUPGRADE);
}


/****************************************************************************
函数原型    protected:void CAPMgr::EndAuxUpgrade(void)
功能描述    辅助升级结束
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CAPMgr::EndAuxUpgrade(void)
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
函数原型    protected:int CAPMgr::handle_signal(int signum, 
                                               siginfo_t* st,
                                               ucontext_t* ut)
功能描述    捕获信号
输入参数    signum---信号码
输出参数    无
返回值      0     ---成功
            非零值---失败
抛出异常    无
*****************************************************************************/
int CAPMgr::handle_signal(int signum, 
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
函数原型    protected:void CAPMgr::OnTimer(const long nTimerID)
功能描述    定时器函数
输入参数    nTimerID---定时器ID
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CAPMgr::OnTimer(const long nTimerID)
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
