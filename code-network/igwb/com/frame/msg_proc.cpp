/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:msg_proc.cpp
    描述信息:
    
    包含CMsgProcessor类的实现,该类是所有其他需要处理消息类的父类.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-16,创建.
*/
#include "msg_proc.h"

/* CMsgRoutor类的定义头文件. */
#include "msg_router.h"
#include "../include/comm.h"
#include "../include/toolbox.h"
#include "../utility/mml_report.h"

#define SIZE_MSG_BLOCK 4

/*
    消息队列最大容纳100万个话单包,按照网络模块每秒生成1个话单包,
    100万能够支持291小时.
*/
#define MSG_QUEUE_HIGH_WATER_MARK   4 * 1024 * 1024
#define MSG_QUEUE_LOW_WATER_MARK    4 * 1024 * 1024


CCommLayer* CMsgProcessor::m_ptheCommLayer = NULL;
CMsgRouter* CMsgProcessor::m_ptheRouter = NULL;
CTimerManager CMsgProcessor::m_theTimerMgr;

BOOL CMsgProcessor::g_bTrackProtocol = FALSE;
//by ldf 2004-07-07 对应问题单SWPD06473
BOOL CMsgProcessor::g_bPerfIterm = TRUE;
//end
BYTE CMsgProcessor::g_nMsgOutLevel = MSGOUT_LEVEL_URGENT;

PROCESS_TYPE CMsgProcessor::g_pt = PT_SELF;

char CMsgProcessor::g_szPt[PROCESS_NAME_LEN];
//张玉新 2002-05-20 问题单D13657
char CMsgProcessor::g_szPtDes[32];

CMsgProcessor::CMsgProcessor()
{
    m_pMsgEvent = new ACE_Auto_Event();

    m_MsgQue.high_water_mark(MSG_QUEUE_HIGH_WATER_MARK);
    m_MsgQue.low_water_mark(MSG_QUEUE_LOW_WATER_MARK);
}

CMsgProcessor::CMsgProcessor(PROCESS_TYPE pt)
{
    m_pMsgEvent = new ACE_Auto_Event();
    sprintf(g_szPt,"%s%d",PTS_AP_BASE,(pt - PT_AP_BASE + 1));

    m_MsgQue.high_water_mark(MSG_QUEUE_HIGH_WATER_MARK);
    m_MsgQue.low_water_mark(MSG_QUEUE_LOW_WATER_MARK);
}

/*
    函数功能:析构函数.

    1.释放消息队列中所有消息.
    2.释放事件对象.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
CMsgProcessor::~CMsgProcessor()
{
    ACE_Message_Block *mb = NULL;
    MSG_INTRA *pMsg = NULL;

    while(m_MsgQue.message_count())
    {
        m_MsgQue.dequeue_head(mb);
        pMsg = (MSG_INTRA*)(*(UINT4 *)(mb->rd_ptr()));

        delete mb;
        delete pMsg;
    }

    if (m_pMsgEvent != NULL)
    {
        delete m_pMsgEvent;
        m_pMsgEvent = NULL;
    }
}   

/*
    函数功能:初始化函数.
    该函数在消息循环函数的头部调用,为开始消息循环做准备.

    1.向路由表注册消息处理对象.
    2.一种类型的消息处理对象在路由表中只能注册一次.

    返回:
    1.若路由表中已经存在相同类型的对象,返回成功.
    2.否则返回失败.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
int CMsgProcessor::InitInstance(void)
{
    if(m_ptheRouter->Register(GetModuleType(), this))
    {
        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

/*
    函数功能:清除函数.
    该函数在消息循环函数的尾部调用,做清除工作.

    1.从路由表中删除消息处理对象.
    2.取消所有注册的定时器.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
void CMsgProcessor::ExitInstance(void)
{
    m_ptheRouter->UnRegister(GetModuleType());
    KillTimer();

}

PROCESS_TYPE CMsgProcessor::GetProcessType()
{
    return g_pt;
}

/*
    函数功能:根据模块类型获得模块的描述文本.

    1.若输入的模块类型未定义则返回NULL.

    参数:
    mt,模块类型.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
const char *CMsgProcessor::GetModuleDesp(MODULE_TYPE mt)
{
    switch(mt)
    {
    case MT_UNKNOWN:
        return MTS_UNKNOWN;
    case MT_MANAGER:
        return MTS_MANAGER;
    case MT_NETWORK:
        return MTS_NETWORK;
    case MT_FRONTSAVE:
        return MTS_FRONTSAVE;
    case MT_BILLPROCESSOR:
        return MTS_BILLPROCESSOR;
    case MT_BACKSAVE:
        return MTS_BACKSAVE;
    case MT_MONITOR:
        return MTS_MONITOR;
    case MT_CLSTRINTERFACE:
        return MTS_CLSTRINTERFACE;
    case MT_ALARM:
        return MTS_ALARM;
    case MT_BACKUP:
        return MTS_BACKUP;
    case MT_MMLSERVER:
        return MTS_MMLSERVER;
    case MT_BILLBROWSER:
        return MTS_BILLBROWSER;
    case MT_LOG:
        return MTS_LOG;
    case MT_MSGDIST:
        return MTS_MSGDIST;
    case MT_CLSTR:
        return MTS_CLSTR;
    case MT_OMCSERVERINTERFACE:
        return MTS_OMCSERVERINTERFACE;
    case MT_PERF:
        return MTS_PERF;
    case MT_MMLDEBUG:
        return MTS_MMLDEBUG;
    //maimaoshi,2002-08-08    
    case MT_BILLTASK:
        return MTS_BILLTASK;
    case MT_COLLECTOR:
        return MTS_COLLECTOR;
    case MT_COLAGENT:
        return MTS_COLAGENT;
    case MT_COLCONVERTOR:
        return MTS_COLCONVERTOR;
    case MT_CFGSVR:
        return MTS_CFGSVR;
	case MT_CDRSTAT:
		return MTS_CDRSTAT;
	case MT_LICENSE_MANAGER:
		return MTS_LICENSE_MANAGER;
    default:
        break;
    }
    //by ldf 2002.6.11 对应问题单D14482
    return "";
    //修改完毕
}

/*
    函数功能:根据进程类型获得进程的描述文本.

    1.若输入的模块类型未定义则返回NULL.

    参数:
    mt,模块类型.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
const char *CMsgProcessor::GetProcessDesp(PROCESS_TYPE pt)
{
    switch(pt)
    {
    case PT_SELF:
        return PTS_SELF;
    case PT_UNKNOWN:
        return PTS_UNKNOWN;
    case PT_OUTSIDE:
        return PTS_OUTSIDE;
    case PT_KERNEL:
        return PTS_KERNEL;
    case PT_OM:
        return PTS_OM;
    case PT_CLSTR:
        return PTS_CLSTR;
    case PT_CFGSVR:
        return PTS_CFGSVR;
    case PT_BROADCAST:
        return PTS_BROADCAST;
    default:
        if(pt == g_pt)
        {
            return g_szPt;
        }
        else
        {
            /* 这段代码不是线程安全的 */

            if((pt < PT_CO_BASE) && (pt >= PT_AP_BASE))
            {
                static char szBuf[128];
                sprintf(szBuf,"%s%d",PTS_AP_BASE,(pt - PT_AP_BASE + 1));
                return szBuf;
            }
            else if(pt >= PT_CO_BASE && pt <= MAX_PT_CO_TYPE)
            {
                static char szBuf[128];
                sprintf(szBuf,"%s%d",PTS_CO_BASE,(pt - PT_CO_BASE + 1));
                return szBuf;
            }
        }
        break;
    }
    //by ldf 2002.6.11 对应问题单D14482
    return "";
    //修改完毕
}

/*
    函数功能:发送消息.

    1.该方法调用CMsgRouter对象的route方法.

    参数:
    pMsgOut,消息指针.

    返回:
    1.若消息成功放入接收者的消息队列,返回成功(同一进程中).
    2.否则返回失败.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
int CMsgProcessor::SendMsg(MSG_INTRA* pMsgOut)
{
    ASSERT(pMsgOut != NULL);
    return m_ptheRouter->Route(pMsgOut);
}

/*
    函数功能:消息入队.

    1.该方法由CMsgRouter对象调用.

    参数:
    pMsgIn,消息指针.

    返回:
    1.若消息入队成功,返回成功.
    2.否则返回失败.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
int CMsgProcessor::Enqueue(MSG_INTRA* pMsgIn)
{
    ASSERT(pMsgIn != NULL);

    ACE_Message_Block *mb= new ACE_Message_Block(SIZE_MSG_BLOCK);

    *((UINT4 *)(mb->wr_ptr())) = (UINT4)pMsgIn;
    mb->wr_ptr(SIZE_MSG_BLOCK);

    ACE_Time_Value tv(1);

    /* 阻塞调用,1秒超时 */
    if(m_MsgQue.enqueue_prio(mb,&tv) == -1)
    {
        delete mb;

        MessageOut(MTS_MANAGER, MSGOUT_LEVEL_URGENT, S_MSG_ENQUE_FAILURE);
        return ERR_FAIL;
    }

    m_pMsgEvent->signal();
    return ERR_SUCCESS;
}

/*
    函数功能:消息入队(增加到头部).

    1.该方法由CMsgRouter对象调用.

    参数:
    pMsgIn,消息指针.

    返回:
    1.若消息入队成功,返回成功.
    2.否则返回失败.

    修改记录:
    zhouyonghui,2002-01-21,创建.
*/
int CMsgProcessor::EnqueueHead(MSG_INTRA* pMsgIn)
{
    ASSERT(pMsgIn != NULL);

    ACE_Message_Block *mb= new ACE_Message_Block(SIZE_MSG_BLOCK);

    *((UINT4 *)(mb->wr_ptr())) = (UINT4)pMsgIn;
    mb->wr_ptr(SIZE_MSG_BLOCK);

    ACE_Time_Value tv(1);

    /* 阻塞调用,1秒超时 */
    if(m_MsgQue.enqueue_head(mb,&tv) == -1)
    {
        delete mb;
        return ERR_FAIL;
    }

    m_pMsgEvent->signal();
    return ERR_SUCCESS;
}

/*
    函数功能:注册时钟.

    参数:
    nInterval,时钟的时长,单位是秒.

    返回:
    返回时钟ID,没有失败返回.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
long CMsgProcessor::SetTimer(int nInterval)
{
    ASSERT(nInterval >= 0);

    STimerStatus *pTimerStatus = new STimerStatus;
    pTimerStatus->TimerID = 0;
    pTimerStatus->flgTrigger = FALSE;

    /*
        注册定时器必须放在m_TimerMutex保护的代码之外,以免死锁.
        因为存在这样一种情况:定时器触发的同时,又SetTimer.
    */
    m_theTimerMgr.SetTimer(this,nInterval,&(pTimerStatus->TimerID));

    m_TimerMutex.acquire();
    m_TimerStatusTab.push_back(pTimerStatus);
    m_TimerMutex.release();

    return pTimerStatus->TimerID;
}

/*
    函数功能:注销时钟.

    参数:
    nTimerID,时钟ID.

    修改记录:
    zhouyonghui,2001-10-16,创建.
    zhouyonghui,2001-12-21,解决死锁问题.
*/
void CMsgProcessor::KillTimer(const long nTimerID)
{
    BOOL bMatched = FALSE;

    m_TimerMutex.acquire();

    LISTTIMERSTATUS::iterator i;
    for(i = m_TimerStatusTab.begin();i != m_TimerStatusTab.end();i++)
    {
        if((*i)->TimerID == nTimerID) 
        {
            bMatched = TRUE;
            delete (*i);
            m_TimerStatusTab.erase(i);

            break;
        }
    }

    m_TimerMutex.release();
    
    /*
        注销定时器必须放在m_TimerMutex保护的代码之外,以免死锁.
        因为存在这样一种情况:定时器触发的同时,又KillTimer.
    */
    if(bMatched == TRUE)
    {
        m_theTimerMgr.KillTimer(nTimerID);
    }
}

/*
    函数功能:注销所有时钟.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
void CMsgProcessor::KillTimer()
{
    /*
        注销定时器必须放在m_TimerMutex保护的代码之外,以免死锁.
        因为存在这样一种情况:定时器触发的同时,又KillTimer.
    */
    LISTTIMERSTATUS::iterator i;
    for(i = m_TimerStatusTab.begin();i != m_TimerStatusTab.end();i++)
    {
        m_theTimerMgr.KillTimer((*i)->TimerID);
    }

    m_TimerMutex.acquire();
    
    for(i = m_TimerStatusTab.begin();i != m_TimerStatusTab.end();i++)
    {
        delete (*i);
    }
    m_TimerStatusTab.clear();

    m_TimerMutex.release();
}

/*
    函数功能:处理时钟消息.

    1.时钟消息具有最低优先级,只有在没有任何其他消息的情况下才会被处理.
    2.函数为每一个触发的时钟调用一次OnTimer()函数.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
void CMsgProcessor::OnIdle(void)
{
    LISTTIMERSTATUS lstTriggeredTimer;

    /*
        收集已经触发的定时器.
    */
    m_TimerMutex.acquire();

    LISTTIMERSTATUS::iterator i;
    for(i = m_TimerStatusTab.begin();i != m_TimerStatusTab.end();i++)
    {
        if((*i)->flgTrigger == TRUE)
        {
            (*i)->flgTrigger = FALSE;
            lstTriggeredTimer.push_back(*i);
        }
    }

    m_TimerMutex.release();

    /*
        处理时钟事件.
    */
    for(i = lstTriggeredTimer.begin();i != lstTriggeredTimer.end();i++)
    {
        OnTimer((*i)->TimerID);
    }
}

/*
    函数功能:消息循环函数.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
void CMsgProcessor::Schedule()
{
    int nResult = InitInstance();

    if (nResult != ERR_SUCCESS)
    {
        TRACE(THISMODULE, S_FRAME_INIT_INSTANCE_FAIL, nResult);
    }
    else
    {
        MSG_INTRA *pMsg = new MSG_INTRA;
        pMsg->SenderPid = PT_SELF;
        pMsg->SenderMid = GetModuleType();
        pMsg->ReceiverPid = PT_SELF;
        pMsg->ReceiverMid = MT_MANAGER;
        pMsg->CmdCode = CMD_READY;
        SendMsg(pMsg);
    }

    ACE_Message_Block *mb = NULL;
    MSG_INTRA *pMsg = NULL;

    while(nResult == ERR_SUCCESS)
    {
        m_pMsgEvent->wait();

        while(m_MsgQue.message_count())
        {
            m_MsgQue.dequeue_head(mb);
            pMsg = (MSG_INTRA*)(*(UINT4 *)(mb->rd_ptr()));
            delete mb;

            if(pMsg->CmdCode == CMD_QUIT)
            {
                MSGOUT(THISMODULE,MSGOUT_LEVEL_URGENT,S_FRAME_RECEIVE_EXIT_CMD);
                TRACE(THISMODULE,S_FRAME_RECEIVE_EXIT_CMD);

                delete pMsg;
                nResult = ERR_FAIL;
                break;
            }

            ParseMsg(pMsg);
            delete pMsg;
        }

        if(nResult == ERR_FAIL) break;

        OnIdle();
    }

    ExitInstance();
}

/*
    函数功能:时钟消息入队.

    1.该方法由CTimerManager对象调用,每当计时器触发,
      就调用该函数设置触发标志.

    参数:
    current_time,当前时间.
    act,时钟ID指针.

    返回:该函数是ACE_Event_Handler类成员函数的重载,只返回0.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
int CMsgProcessor::handle_timeout(const ACE_Time_Value &current_time,
                                  const void * act)
{
    m_TimerMutex.acquire();

    LISTTIMERSTATUS::iterator i;
    for(i = m_TimerStatusTab.begin();i != m_TimerStatusTab.end();i++)
    {
        if((*i)->TimerID == *(long *)act)
        {
            (*i)->flgTrigger = TRUE;
            m_pMsgEvent->signal();
            break;
        }
    }

    m_TimerMutex.release();
    return 0;
}

/*
    函数功能:线程函数,调用消息处理对象的Schedule()函数进入消息循环.

    参数:
    pModule,消息处理对象的指针.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
void CMsgProcessor::ScheduleThreadProc(CMsgProcessor* pModule)
{
    ASSERT(pModule != NULL);
    pModule->Schedule();
}

/*
    函数功能:创建线程.

    返回:创建线程成功,返回TRUE;否则返回FALSE.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
int CMsgProcessor::Run(void)
{
    if(m_Tgr.spawn((ACE_THR_FUNC)ScheduleThreadProc, 
                    (void *)this) != -1)
    {
        return ERR_SUCCESS;
    }
    else
    {
        return ERR_FAIL;
    }
}

/*
    函数功能:等待线程结束.

    参数:
    nTimeOut,等待线程结束的超时时间,为0则一直等待直到结束.

    返回:线程的终止状态.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
int CMsgProcessor::Wait(const long nTimeOut)
{
    ASSERT(nTimeOut >= 0);

    if(nTimeOut != 0)
    {
        ACE_Time_Value tm_val;
        tm_val.msec(nTimeOut);
        return m_Tgr.wait(&tm_val);
    }
    else
    {
        return m_Tgr.wait(0);
    }
}

void CMsgProcessor::MessageOut(const char* szModuleName,
                               const BYTE nMsgOutLevel,
                               const char* szFmt, ...)
{
    char pBuf[1024];
    va_list argptr;
    
    //问题单：SWPD00562，2003年7月23日，Zou Yongsheng修改
    if((nMsgOutLevel & g_nMsgOutLevel) == 0)
	{
		return;
	}
	//修改结束

    va_start(argptr, szFmt);
    VSNPRINTF(pBuf,1024,szFmt,argptr);
    pBuf[1023] = 0;

    va_end(argptr);

    //张玉新 2002-05-20 问题单D13657
	char szTmp[48];
	SNPRINTF(szTmp,48,"(%s)%s",g_szPtDes,szModuleName);
	szTmp[47] = 0;
    /* 构造MML报文. */
     
    CMMLReport Report(szTmp);//张玉新 2002-05-20 问题单D13657
    Report.MakeReportHead();

    /* 报文头第二行 */

    Report.AddString(S_FRAME_MML_REPORT_HEADER_PART1);
    //by ldf 2002.6.10 对应问题单D14393
    Report.FillSpace(4);
    //修改完毕

    if(nMsgOutLevel == MSGOUT_LEVEL_URGENT)
    {
        Report.AddString(S_MSGOUT_LEVEL_URGENT);
    }
    else if(nMsgOutLevel == MSGOUT_LEVEL_IMPORTANT)
    {
        Report.AddString(S_MSGOUT_LEVEL_IMPORTANT);
    }
    else if(nMsgOutLevel == MSGOUT_LEVEL_SIDLY)
    {
        Report.AddString(S_MSGOUT_LEVEL_SIDLY);
    }
    else if(nMsgOutLevel == MSGOUT_LEVEL_NORMAL)
    {
        Report.AddString(S_MSGOUT_LEVEL_NORMAL);
    }
    else
    {
        Report.AddString(S_MSGOUT_LEVEL_NORMAL);
    }

    //by ldf 2002.6.10 对应问题单D14393
    Report.FillSpace(8);
    Report.AddString(S_FRAME_MML_REPORT_HEADER_PART2);
    Report.FillSpace(3);
    //修改完毕

    Report.AddString(STR_NEWLINE);

    /* 正文 */
    Report.MakeReportItem2(S_FRAME_DEBUG_CONTENT,pBuf);
    Report.MakeReportItem2(S_FRAME_DEBUG_INFO,szModuleName);

    /* 报文尾 */
    Report.MakeReportTail();

    const char* p = Report.GetReport();

    MSG_INTRA *pMsg = new(strlen(p) + 1) MSG_INTRA;
    strcpy((char*)(pMsg->Data), p);

    pMsg->SenderPid = GetProcessType(); 
    pMsg->SenderMid = MT_UNKNOWN;
    pMsg->ReceiverPid = PT_OM;
    pMsg->ReceiverMid = MT_MMLDEBUG;
    pMsg->AppType = APP_MML_DEBUG;
    pMsg->CmdCode = CMD_MSG_OUT;

    SendMsg(pMsg);
}

void CMsgProcessor::TrackProtocol(const char* pMsgProtocol)
{
    if(!g_bTrackProtocol) return;

    MSG_INTRA *pMsg = new(strlen(pMsgProtocol) + 1)MSG_INTRA;
    strcpy((char*)pMsg->Data,pMsgProtocol);

    pMsg->SenderPid = GetProcessType(); 
    pMsg->SenderMid = MT_UNKNOWN;
    pMsg->ReceiverPid = PT_OM;
    pMsg->ReceiverMid = MT_MMLDEBUG;
    pMsg->AppType = APP_MML_DEBUG;
    pMsg->CmdCode = CMD_PROTOCOL_TRACK;

    SendMsg(pMsg);
}

/*
    函数功能:向MML Server发送告警消息.
    不处理发送失败的情况,若发送失败消息被丢弃.

    参数:
    pAlarm,告警消息指针.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
void CMsgProcessor::SendAlarm(const SInnerAlarm* pAlarm)
{
    MSG_INTRA *pMsg = new(sizeof(SInnerAlarm)) MSG_INTRA;
    pMsg->SenderPid = GetProcessType();
    pMsg->SenderMid = MT_UNKNOWN;
    pMsg->ReceiverPid = PT_OM;
    pMsg->ReceiverMid = MT_ALARM;
    pMsg->CmdCode = CMD_INNER_ALARM;
    memcpy(pMsg->Data, pAlarm, pMsg->Len);

    SendMsg(pMsg);
}

/*
    函数功能:向自己发送退出消息.
    不处理发送失败的情况,若发送失败消息被丢弃.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
void CMsgProcessor::SendQuitToMe()
{
    MSG_INTRA *pMsg = new MSG_INTRA;
    pMsg->SenderPid = PT_SELF; 
    pMsg->SenderMid = GetModuleType();
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = GetModuleType();
    pMsg->CmdCode = CMD_QUIT;

    MSGOUT(THISMODULE,MSGOUT_LEVEL_URGENT,S_FRAME_SEND_QUIT_MSG_TO_ME);
    TRACE(THISMODULE,S_FRAME_SEND_QUIT_MSG_TO_ME);

    EnqueueHead(pMsg);
}

/*
    函数功能:向日志模块发送日志消息.
    不处理发送失败的情况,若发送失败消息被丢弃.

    参数:
    szModuleName,发送消息的模块名.
    szSysLog,消息文本.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
void CMsgProcessor::WriteLog(const char* szModuleName,
                             const char* szSysLog,
                             BYTE bLogResult)
{
    MSG_INTRA *pMsg = new(sizeof(SLog)) MSG_INTRA;
    pMsg->SenderPid = GetProcessType(); 
    pMsg->SenderMid = MT_UNKNOWN;
    pMsg->ReceiverPid = PT_OM;
    pMsg->ReceiverMid = MT_LOG;
    pMsg->CmdCode = CMD_WRITE_LOG;

    SLog *pLog = (SLog *)pMsg->Data;

    //张玉新 2002-05-20 问题单D13657
	SNPRINTF(pLog->szUserName,48,"(%s)%s",g_szPtDes,szModuleName);
    //修改结束
    pLog->szUserName[47] = 0;

    pLog->LogType = (BYTE)(SLog::LOG_SYS);
    pLog->uIPAddr = 0;
    //张玉新 2002-03-13 问题单 D11606
    pLog->OperResult = bLogResult;
    //over
    pLog->szOperCmd[0] = '\0';
    strncpy(pLog->szCmdDesc, szSysLog,LOG_CMD_DESC_LEN);
    pLog->szCmdDesc[LOG_CMD_DESC_LEN-1] = 0;

    SendMsg(pMsg);
}

/*
    函数功能:向日志模块发送日志消息.
    不处理发送失败的情况,若发送失败消息被丢弃.

    参数:
    pLog,日志消息结构.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
void CMsgProcessor::WriteLog(const SLog* pLog)
{
    MSG_INTRA *pMsg = new(sizeof(SLog)) MSG_INTRA;
    pMsg->SenderPid = GetProcessType(); 
    pMsg->SenderMid = MT_UNKNOWN;
    pMsg->ReceiverPid = PT_OM;
    pMsg->ReceiverMid = MT_LOG;
    pMsg->CmdCode = CMD_WRITE_LOG;
    memcpy(pMsg->Data, pLog, sizeof(SLog));
	//张玉新 2002-05-20 问题单D13657
	if(pLog->LogType == SLog::LOG_SYS)
	{	
		SLog *pTmp = (SLog *)(pMsg->Data);
		char szTmp[48];
		SNPRINTF(szTmp,48,"(%s)%s",g_szPtDes,pLog->szUserName);
		szTmp[47] = 0;
		strncpy(pTmp->szUserName,szTmp,48);
	}
	//修改结束

    SendMsg(pMsg);
}

/*
    函数功能:向Agent模块发送倒换消息.
    不处理发送失败的情况,若发送失败消息被丢弃.

    参数:
    mt,申请倒换的模块号.
    nCause,申请倒换的原因,倒换原因定义在basetype.h中.
    pData,倒换时可能发送给对端服务器的数据.
    nLen,pData数据的长度.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
void CMsgProcessor::AskForFailOver(const MODULE_TYPE mt, 
                            const BYTE nCause, 
                            const void* pData, 
                            const UINT4 nLen)
{
    MSG_INTRA *pMsg = new(sizeof(BYTE) + nLen) MSG_INTRA;
    pMsg->SenderPid = GetProcessType(); 
    pMsg->SenderMid = mt;
    pMsg->ReceiverPid = PT_KERNEL;
    pMsg->ReceiverMid = MT_CLSTRINTERFACE;
    pMsg->CmdCode = CMD_ASK_FAILOVER;
    memcpy(pMsg->Data, &nCause, sizeof(BYTE));
    memcpy(pMsg->Data + sizeof(BYTE), pData, nLen);

    SendMsg(pMsg);
}

/*
    函数功能:向Agent模块发送同步数据.
    不处理发送失败的情况,若发送失败消息被丢弃.

    参数:
    mt,申请倒换的模块号.
    pData,同步数据.
    nLen,同步数据的长度.
    nChlID,通道号,当mt等于MT_BACKSAVE时该参数有意义.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
void CMsgProcessor::WriteSyncData(const MODULE_TYPE mt,
                                  const void* pData,
                                  const UINT4 nLen,
                                  const UINT4 nChlID)
{
    MSG_INTRA *pMsg = new(sizeof(UINT4) + nLen) MSG_INTRA;
    pMsg->SenderPid = GetProcessType();
    pMsg->SenderMid = mt;
    //by ldf 2002.5.21 对应问题单D13761
    pMsg->ReceiverPid = PT_CLSTR;
    pMsg->ReceiverMid = MT_CLSTR;
	//修改完毕
    pMsg->CmdCode = CMD_SYNC_DATA;
    memcpy(pMsg->Data, &nChlID, sizeof(UINT4));
    memcpy(pMsg->Data + sizeof(UINT4), pData, nLen);

    SendMsg(pMsg);
}

void CMsgProcessor::OnTimer(const long nTimerID)
{
    /* 缺省处理丢弃时钟消息. */
}
