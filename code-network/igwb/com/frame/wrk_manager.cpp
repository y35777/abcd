/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:wrk_manager.cpp
    描述信息:
    
    包含CModuleManager类的实现,该类用于在整个进程中交换消息.
    CMsgProcessor类包含该类的对象,以支持消息交换.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-16,创建.
*/

#include "wrk_manager.h"
#include "msg_router.h"
#include "wrk_module.h"

/* 定义CCommlayer类的头文件 */
#include "../include/comm.h"

/* 定义CperfIterm类的头文件 */
#include "../include/perf_item.h"
//张玉新 2002-04-25 问题单 D12515
#include "../toolbase/ini_file.h"
#include "../utility/config.h"
CModuleManager::CModuleManager()
{
    m_lTimerIDBase = -1;
    m_lTimerCounterPerf = 0;
    m_nModuleReady = 0;

    m_nStatus = WKMGR_INIT_PENDING;
}

CModuleManager::~CModuleManager()
{
}

/*
    函数功能:初始化函数,在消息循环函数中调用.

    1.创建消息路由对象.
    2.创建网络通信层对象.
    3.向路由表中注册自己.
    4.设置一个基准定时器.
    5.创建各个业务模块.
    6.启动定时器.

    返回:返回错误代码,错误代码定义在basetype.h文件中.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
int CModuleManager::InitInstance(void)
{
    ASSERT(GetProcessType() > 0);
    int nRet = ERR_SUCCESS;

    if(m_ptheRouter == NULL)
    {
        m_ptheRouter = new CMsgRouter(GetProcessType());
    }

    if(m_ptheCommLayer == NULL)
    {
        m_ptheCommLayer = CCommLayer::Instance(GetProcessType());
        if(m_ptheCommLayer == NULL)
        {
            TRACE(MTS_MANAGER,S_FRAME_CREATE_COM_OBJ_FAIL);
            return ERR_FAIL;
        }
    }

    //调用基类的InitInstance函数,在路由表中注册自己.
    nRet = CMsgProcessor::InitInstance();
    if (nRet != ERR_SUCCESS)
    {
        return nRet;
    }
	
    m_lTimerIDBase = SetTimer(ONE_SECOND_IN_MILISEC);

    //创建所有业务模块对象.
    nRet = CreateWorkModules();
    if(nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //时钟必须在本函数中启动.
    m_theTimerMgr.Run();

    //置状态为READY,代表初始化成功,等待Kernel的激活命令.
    m_nStatus = WKMGR_READY;
    //张玉新 2002-04-25 问题单D12515
    //张玉新 2002-03-13 问题单D11728
	CINIFile iniFile(GetCfgFilePath());
	BOOL bRet = iniFile.Open(); 
	if(bRet != TRUE)
	{
		char szTmpBuf[256];
		sprintf(szTmpBuf, "\r\n\t\t\t\t     Errno: %d (%s)", errno, strerror(errno));
		STRING sTmpInfo = S_FRAME_OPEN_INI_FAIL;//"打开配置文件失败"
		sTmpInfo += GetCfgFilePath();
		sTmpInfo += szTmpBuf;
		TRACE(THISMODULE,sTmpInfo.c_str());
		return ERR_FAIL;
	}
	CMsgProcessor::g_nMsgOutLevel = iniFile.GetInt(CFG_SEC_COMMON,
	                                               CFG_COMMON_KEY_MSGOUTLEVEL,
	                                               MSGOUT_LEVEL_IMPORTANT|MSGOUT_LEVEL_URGENT);
	iniFile.Close();
	//修改结束
    return ERR_SUCCESS;
}

/*
    函数功能:消息处理函数.

    参数:
    pMsgParsed,消息指针.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
void CModuleManager::ParseMsg(MSG_INTRA* pMsgParsed)
{
    static BOOL bRecvActiveCmd = FALSE;

    switch (pMsgParsed->CmdCode)
    {
    case CMD_ACTIVE:
        /*
            收到Kernel发送的激活命令,激活各个业务模块.
            只响应一次激活命令.
        */
        if((bRecvActiveCmd == TRUE) || (m_nStatus != WKMGR_READY))
        {
            return;
        }

        bRecvActiveCmd = TRUE;

        if(ActiveWorkModules() == FALSE) 
        {
            m_nStatus = WKMGR_FAILURE;
            TRACE(MTS_MANAGER,S_FRAME_ACTIVE_MODULE_FAIL);
            SendQuitToMe();
        }

        break;

    case CMD_READY:
        /*
            收到所有业务模块发送的就绪消息后,
            向各个业务模块发送激活命令,
            并向Kernel模块发送激活应答消息.
        */
        m_nModuleReady++;    //该变量包括管理模块自己.
        if(m_nModuleReady == m_WorkModules.size() + 1)
        {
            LIST_WORK_MODULE::iterator i;
            for (i = m_WorkModules.begin();
                 i != m_WorkModules.end(); i++)
            {
                MSG_INTRA *pMsg = new MSG_INTRA;
                pMsg->SenderPid = PT_SELF; 
                pMsg->SenderMid = GetModuleType();
                pMsg->ReceiverPid = PT_SELF;
                pMsg->ReceiverMid = (*i)->GetModuleType();
                pMsg->CmdCode = CMD_MODULE_ACTIVE;
                SendMsg(pMsg);
            }

            m_nStatus = WKMGR_ACTIVE;

            //向Kernel进程中的监控模块发送激活应答消息.
            MSG_INTRA *pMsg = new MSG_INTRA;
            pMsg->SenderPid = GetProcessType();
            pMsg->SenderMid = GetModuleType();
            pMsg->ReceiverPid = PT_KERNEL;
            pMsg->ReceiverMid = MT_MONITOR;
            pMsg->CmdCode = CMD_ANS_ACTIVE;

            SendMsg(pMsg);

            MSGOUT(THISMODULE,MSGOUT_LEVEL_IMPORTANT,
                S_FRAME_ACTIVE_MODULE_SUCCEED);
        }
        
        break;
        
    default:
        //张玉新 2002-04-25 问题单 D12535
		//TRACE(MTS_MANAGER,S_FRAME_UNPROCESSED_MSG,pMsgParsed->SenderPid,
        //                                          pMsgParsed->SenderMid,
        //                                          pMsgParsed->CmdCode);
        break;
    }
}

/*
    函数功能:启动所有业务模块线程.

    1.调用各个业务模块的Run()函数启动各个线程.

    返回:成功启动所有业务模块返回TRUE,失败返回FALSE.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
BOOL CModuleManager::ActiveWorkModules()
{
    LIST_WORK_MODULE::iterator i;
    for (i = m_WorkModules.begin(); i != m_WorkModules.end(); i++)
    {
        if(((*i)->Run()) != ERR_SUCCESS)
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*
    函数功能:终止所有业务模块线程.

    1.通过向所有业务模块发送退出消息来终止业务模块.
    不处理发送消息失败的情况.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
void CModuleManager::DeActiveWorkModules()
{
    //停止定时器
    m_theTimerMgr.Stop();

    LIST_WORK_MODULE::iterator i;
    for (i = m_WorkModules.begin(); i != m_WorkModules.end(); i++)
    {
        MSG_INTRA *pMsg = new MSG_INTRA;
        pMsg->SenderPid = PT_SELF; 
        pMsg->SenderMid = GetModuleType();
        pMsg->ReceiverPid = PT_SELF;
        pMsg->CmdCode = CMD_QUIT;
        pMsg->ReceiverMid = (*i)->GetModuleType();

        SendMsg(pMsg);
    }
}

/*
    函数功能:清除函数.

    1.终止并等待各个业务模块结束.
    2.注销定时器.
    3.终止性能统计.
    4.删除路由表和通信层.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
void CModuleManager::ExitInstance(void)
{
    /*
        终止并等待各个业务模块退出.
    */
    DeActiveWorkModules();

    LIST_WORK_MODULE::iterator i;
    for (i = m_WorkModules.begin(); i != m_WorkModules.end(); i++)
    {
        (*i)->Wait();
        delete (*i);
    }

    m_WorkModules.clear();

    KillTimer(m_lTimerIDBase);
    CPerfItem::ReleaseAllItems();
	CLinkStatItem::ReleaseAllItems();
    CMsgProcessor::ExitInstance();

    if(m_ptheCommLayer) 
    {
        m_ptheCommLayer->Destroy();
        m_ptheCommLayer = NULL;
    }

    /*
        删除消息中转对象,必须在删除通信层对象之后进行,
        因为消息中转对象删除之后,可能通信层对象仍然在发送消息.
    */
    if(m_ptheRouter)  
    {
        delete m_ptheRouter;
        m_ptheRouter = NULL;
    }
}

/*
    函数功能:处理时钟消息.

    参数:nTimerID时钟ID.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
void CModuleManager::OnTimer(const long nTimerID)
{
    if(nTimerID == m_lTimerIDBase)
    {
        /*
            同步性能统计开关的状态.
        */
        CPerfItem::SetPerfOn(g_bPerfIterm);

        if((m_lTimerCounterPerf++ > 10) && (g_bPerfIterm == TRUE))
        {
            /*
                若nTimerID为基准定时器ID,则执行性能统计.
                1.终止当前统计周期.
                2.开始一个新的统计周期.
            */

            SPerfItem* pItems = NULL;
            UINT2 nItemCount = 0;

            CPerfItem::EndStatistic(pItems,nItemCount);
            
            m_lTimerCounterPerf = 0;

            if(nItemCount == 0) return;

            MSG_INTRA *pMsg = new(sizeof(SPerfItem) * nItemCount) MSG_INTRA;
            memcpy(pMsg->Data,pItems,sizeof(SPerfItem) * nItemCount);
            delete pItems;

            pMsg->SenderPid = GetProcessType(); 
            pMsg->SenderMid = GetModuleType();
            pMsg->ReceiverPid = PT_OM;
            pMsg->ReceiverMid = MT_PERF;
            pMsg->CmdCode =  CMD_SAVE_PERF;
            
            SendMsg(pMsg);
            CPerfItem::BeginStatistic();            
        }
    }

    /* 时钟消息送基类 */
    CMsgProcessor::OnTimer(nTimerID);
}
