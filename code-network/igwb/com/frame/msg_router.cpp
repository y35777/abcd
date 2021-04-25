/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:msg_router.cpp
    描述信息:
    
    包含CMsgRouter类的实现,该类用于在整个进程中交换消息.
    CMsgProcessor类包含该类的对象,以支持消息交换.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-16,创建.
*/

#include "msg_router.h"
#include "msg_proc.h"

/* CTimerManager类的定义头文件. */
#include "../include/comm.h"
#include "../include/toolbox.h"
#include "../utility/mml_report.h"

/* 定义CperfIterm类的头文件 */
#include "../include/perf_item.h"

CMsgRouter::CMsgRouter(const PROCESS_TYPE pt)
{
    m_pt = pt;
    m_bTrack = FALSE;
}

CMsgRouter::~CMsgRouter()
{
    m_RouteTab.clear();
}

/*
    函数功能:在路由表中注册消息处理对象.
    一个类型的对象只能在路由表中注册一个对象.

    1.若路由表中存在对应类型的对象,返回FALSE.
    2.若路由表中不存在对应类型的对象,把消息处理对象追加到表中.

    参数:
    mt,模块类型.
    pModule,消息处理对象指针.

    返回:注册成功返回TRUE,失败返回FALSE.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
BOOL CMsgRouter::Register(const MODULE_TYPE mt, CMsgProcessor* pModule)
{
    m_RouterRWMutex.acquire_write();

    MAPROUTE::iterator i = m_RouteTab.find(mt);
    if(i != m_RouteTab.end())
    {
        m_RouterRWMutex.release();
        return FALSE;
    }

    //注册指针业务模块到路由表。
    m_RouteTab[mt] = pModule;

    m_RouterRWMutex.release();
    return TRUE;
}

/*
    函数功能:从路由表中删除消息处理对象.
    一种类型的对象在路由表只有一个对象.

    1.若路由表中存在对应类型的对象,则删除.
    2.若路由表中不存在对应类型的对象,则不做任何处理.

    参数:
    mt,模块类型.

    返回:该函数只返回TRUE.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/

BOOL CMsgRouter::UnRegister(const MODULE_TYPE mt)
{
    m_RouterRWMutex.acquire_write();

    MAPROUTE::iterator i = m_RouteTab.find(mt);
    if(i != m_RouteTab.end())
    {
        m_RouteTab.erase(i);
    }

    m_RouterRWMutex.release();

    return TRUE;
}

/*
    函数功能:发送跟踪消息.
    跟踪消息被送往MML Server,并最终送到MML客户端.

    参数:pMsgTracked消息指针.

    返回:
*/
BOOL CMsgRouter::SendTrackedMsg(MSG_INTRA* pMsgTracked)
{
    char szBuf[256];

    /* 构造MML报文头. */

    CMMLReport Report(MTS_MSGDIST);
    Report.MakeReportHead();

    /* 报文头第二行 */
    Report.AddString(S_FRAME_MML_REPORT_HEADER_PART0);
    //by ldf 2002.6.10 对应问题单D14393
    Report.FillSpace(3);
    //修改完毕

    Report.AddString(STR_NEWLINE);

    /* 正文 */
    Report.MakeReportItem2(S_FRAME_SENDER_PID,
        CMsgProcessor::GetProcessDesp((PROCESS_TYPE)
        (pMsgTracked->SenderPid)));

    Report.MakeReportItem2(S_FRAME_SENDER_MID,
        CMsgProcessor::GetModuleDesp((MODULE_TYPE)
        (pMsgTracked->SenderMid)));

    Report.MakeReportItem2(S_FRAME_RECEIVER_PID,
        CMsgProcessor::GetProcessDesp((PROCESS_TYPE)
        (pMsgTracked->ReceiverPid)));

    Report.MakeReportItem2(S_FRAME_RECEIVER_MID,
        CMsgProcessor::GetModuleDesp((MODULE_TYPE)
        (pMsgTracked->ReceiverMid)));

    sprintf(szBuf,"%d",pMsgTracked->AppType);
    Report.MakeReportItem2(S_FRAME_APP_TYPE,szBuf);

    sprintf(szBuf,"%d",pMsgTracked->ClientNo);
    Report.MakeReportItem2(S_FRAME_CLIENT_NO,szBuf);

    Report.MakeReportItem2(S_FRAME_USER_NAME,pMsgTracked->UserName);

    struct in_addr addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
    addr.s_addr = pMsgTracked->IPAddr;
#else //_PLATFORM_HPUX
    addr.S_un.S_addr = pMsgTracked->IPAddr;
#endif//_PLATFORM_HPUX
    Report.MakeReportItem2(S_FRAME_CLIENT_IP,ACE_OS::inet_ntoa(addr));

    sprintf(szBuf,"%d",pMsgTracked->CmdCode);
    Report.MakeReportItem2(S_FRAME_CMD_CODE,szBuf);

    sprintf(szBuf,"%d",pMsgTracked->Len);
    Report.MakeReportItem2(S_FRAME_DATA_LENGTH,szBuf);

    /* 构造报文尾 */
    Report.MakeReportTail();

    const char* p = Report.GetReport();
    MSG_INTRA *pMsg = new(strlen(p) + 1) MSG_INTRA;
    strcpy((char*)(pMsg->Data),p);

    pMsg->SenderPid = CMsgProcessor::GetProcessType(); 
    pMsg->SenderMid = MT_UNKNOWN;
    pMsg->ReceiverPid = PT_OM;
    pMsg->ReceiverMid = MT_MMLDEBUG;
    pMsg->AppType = APP_MML_DEBUG;
    pMsg->CmdCode = CMD_TRACKMSG;

    return (CMsgProcessor::SendMsg(pMsg) == ERR_SUCCESS);
}

/*
    函数功能:把一条消息分发给路由表中的某个消息处理对象.

    1.若消息接收对象存在,则把消息方如接收对象的消息对列中.
    2.若消息接收对象不存在,则丢弃消息.
    3.若消息发送失败,则丢弃消息并写Trace.

    参数:
    pMsgRouted,待分发的消息指针.

    返回:返回错误代码,错误代码定义在basetype.h文件中.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
int CMsgRouter::Route(MSG_INTRA* pMsgRouted)
{
    ASSERT(pMsgRouted != NULL);

    /*
        m_bTrack = TRUE,发送跟踪消息.
        跟踪消息通过MML Server发给调试台.
    */
    if(m_bTrack && (pMsgRouted->CmdCode != CMD_TRACKMSG))
    {
        SendTrackedMsg(pMsgRouted);
    }

    /*
        补丁:若本进程是OMM进程,且消息的接收模块是MT_MML_DEBUG模块,
        则把消息的接收进程更改为PT_OUTSIDE,以便把消息发送给网络层,
        最终发送到MML客户端.
    */
    if((m_pt == PT_OM) && (pMsgRouted->ReceiverMid == MT_MMLDEBUG))
    {
        pMsgRouted->ReceiverPid = PT_OUTSIDE;
    }

    /*
        消息的接收者是进程外模块,把消息通过通信层发送出去.
    */
    if ((pMsgRouted->ReceiverPid != m_pt)
        && (pMsgRouted->ReceiverPid != PT_SELF))
    {
        int nRet = CMsgProcessor::m_ptheCommLayer->Send(pMsgRouted);
        //问题单 2002-03-13 delete old code for D11689
		//问题单 2002-04-26 问题单D13074
		delete pMsgRouted;
        return nRet;
    }
    else
    {
        /* 消息接收对象是路由对象 */
        if(pMsgRouted->ReceiverMid == MT_MSGDIST)
        {
            ProcessOwnMsg(pMsgRouted);          
            return ERR_SUCCESS;
        }

        /*
            在路由表中查找消息接收对象.

            1.若消息接收对象存在,则把消息放入对象的消息队列中.
            2.若消息接收对象不存在,则丢弃消息.
            3.在遍历路由表之前需要先上锁.
        */
        m_RouterRWMutex.acquire_read();

        MAPROUTE::iterator i = m_RouteTab.find(pMsgRouted->ReceiverMid);
        if(i != m_RouteTab.end())
        {
            int nRet = (*i).second->Enqueue(pMsgRouted);

            m_RouterRWMutex.release();
            if(nRet != ERR_SUCCESS)
            {
                //张玉新 2002-03-13 delete old code for 问题单 D11689
                /* 消息入队失败,丢弃消息 */
                delete pMsgRouted;
            }

            return nRet;
        }

        m_RouterRWMutex.release();

        //张玉新 2002-03-13 delete old code for 问题单D11689
        delete pMsgRouted;

        return ERR_FAIL;
    }
}

/*
    函数功能:处理发送给CMsgRouter对象的消息.
    若发现不能识别的小就写Trace.

    参数:
    pMsgOwn,消息指针.

    返回:无.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
void CMsgRouter::ProcessOwnMsg(MSG_INTRA* pMsgOwn)
{
    switch(pMsgOwn->CmdCode)
    {
    case CMD_TRACKMSG_ENABLE:       //打开消息跟踪
        m_bTrack = TRUE;
        break;
    case CMD_TRACKMSG_DISABLE:      //取消消息跟踪
        m_bTrack = FALSE;
        break;
    case CMD_TRACKPROTOCOL_ENABLE:  //打开协议跟踪
        CMsgProcessor::g_bTrackProtocol = TRUE;
        break;
    case CMD_TRACKPROTOCOL_DISABLE: //取消协议跟踪
        CMsgProcessor::g_bTrackProtocol = FALSE;
        break;
    
	//问题单：SWPD00562，2003年7月23日，Zou Yongsheng修改
    case CMD_SET_MSGOUT_LEVEL:      //设置输出级别
		CMsgProcessor::g_nMsgOutLevel = pMsgOwn->Data[0];
		break;
    //修改结束
    case CMD_PERFITERM_ENABLE:      //打开性能统计
        {
            if(CMsgProcessor::g_bPerfIterm != TRUE)
            {
                CMsgProcessor::g_bPerfIterm = TRUE;
                CPerfItem::BeginStatistic();
                CPerfItem::SetPerfOn(TRUE);
            }
        }
        break;
    case CMD_PERFITERM_DISABLE:     //取消性能统计
        {
            CMsgProcessor::g_bPerfIterm = FALSE;
            CPerfItem::SetPerfOn(FALSE);
        }
        break;
    default:
        {
            /* 未能识别的消息,写Trace */
			//张玉新 2002-04-25 问题单 12535
            //TRACE(CMsgProcessor::GetModuleDesp(MT_MSGDIST),
            //                        S_FRAME_UNPROCESSED_MSG,
            //                        pMsgOwn->SenderPid,
            //                        pMsgOwn->SenderMid,
            //                        pMsgOwn->CmdCode);
        }
        break;
    }

    delete pMsgOwn;
}
