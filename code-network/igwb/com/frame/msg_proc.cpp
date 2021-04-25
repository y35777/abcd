/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:msg_proc.cpp
    ������Ϣ:
    
    ����CMsgProcessor���ʵ��,����������������Ҫ������Ϣ��ĸ���.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/
#include "msg_proc.h"

/* CMsgRoutor��Ķ���ͷ�ļ�. */
#include "msg_router.h"
#include "../include/comm.h"
#include "../include/toolbox.h"
#include "../utility/mml_report.h"

#define SIZE_MSG_BLOCK 4

/*
    ��Ϣ�����������100���������,��������ģ��ÿ������1��������,
    100���ܹ�֧��291Сʱ.
*/
#define MSG_QUEUE_HIGH_WATER_MARK   4 * 1024 * 1024
#define MSG_QUEUE_LOW_WATER_MARK    4 * 1024 * 1024


CCommLayer* CMsgProcessor::m_ptheCommLayer = NULL;
CMsgRouter* CMsgProcessor::m_ptheRouter = NULL;
CTimerManager CMsgProcessor::m_theTimerMgr;

BOOL CMsgProcessor::g_bTrackProtocol = FALSE;
//by ldf 2004-07-07 ��Ӧ���ⵥSWPD06473
BOOL CMsgProcessor::g_bPerfIterm = TRUE;
//end
BYTE CMsgProcessor::g_nMsgOutLevel = MSGOUT_LEVEL_URGENT;

PROCESS_TYPE CMsgProcessor::g_pt = PT_SELF;

char CMsgProcessor::g_szPt[PROCESS_NAME_LEN];
//������ 2002-05-20 ���ⵥD13657
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
    ��������:��������.

    1.�ͷ���Ϣ������������Ϣ.
    2.�ͷ��¼�����.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
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
    ��������:��ʼ������.
    �ú�������Ϣѭ��������ͷ������,Ϊ��ʼ��Ϣѭ����׼��.

    1.��·�ɱ�ע����Ϣ�������.
    2.һ�����͵���Ϣ���������·�ɱ���ֻ��ע��һ��.

    ����:
    1.��·�ɱ����Ѿ�������ͬ���͵Ķ���,���سɹ�.
    2.���򷵻�ʧ��.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
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
    ��������:�������.
    �ú�������Ϣѭ��������β������,���������.

    1.��·�ɱ���ɾ����Ϣ�������.
    2.ȡ������ע��Ķ�ʱ��.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
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
    ��������:����ģ�����ͻ��ģ��������ı�.

    1.�������ģ������δ�����򷵻�NULL.

    ����:
    mt,ģ������.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
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
    //by ldf 2002.6.11 ��Ӧ���ⵥD14482
    return "";
    //�޸����
}

/*
    ��������:���ݽ������ͻ�ý��̵������ı�.

    1.�������ģ������δ�����򷵻�NULL.

    ����:
    mt,ģ������.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
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
            /* ��δ��벻���̰߳�ȫ�� */

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
    //by ldf 2002.6.11 ��Ӧ���ⵥD14482
    return "";
    //�޸����
}

/*
    ��������:������Ϣ.

    1.�÷�������CMsgRouter�����route����.

    ����:
    pMsgOut,��Ϣָ��.

    ����:
    1.����Ϣ�ɹ���������ߵ���Ϣ����,���سɹ�(ͬһ������).
    2.���򷵻�ʧ��.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
*/
int CMsgProcessor::SendMsg(MSG_INTRA* pMsgOut)
{
    ASSERT(pMsgOut != NULL);
    return m_ptheRouter->Route(pMsgOut);
}

/*
    ��������:��Ϣ���.

    1.�÷�����CMsgRouter�������.

    ����:
    pMsgIn,��Ϣָ��.

    ����:
    1.����Ϣ��ӳɹ�,���سɹ�.
    2.���򷵻�ʧ��.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
*/
int CMsgProcessor::Enqueue(MSG_INTRA* pMsgIn)
{
    ASSERT(pMsgIn != NULL);

    ACE_Message_Block *mb= new ACE_Message_Block(SIZE_MSG_BLOCK);

    *((UINT4 *)(mb->wr_ptr())) = (UINT4)pMsgIn;
    mb->wr_ptr(SIZE_MSG_BLOCK);

    ACE_Time_Value tv(1);

    /* ��������,1�볬ʱ */
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
    ��������:��Ϣ���(���ӵ�ͷ��).

    1.�÷�����CMsgRouter�������.

    ����:
    pMsgIn,��Ϣָ��.

    ����:
    1.����Ϣ��ӳɹ�,���سɹ�.
    2.���򷵻�ʧ��.

    �޸ļ�¼:
    zhouyonghui,2002-01-21,����.
*/
int CMsgProcessor::EnqueueHead(MSG_INTRA* pMsgIn)
{
    ASSERT(pMsgIn != NULL);

    ACE_Message_Block *mb= new ACE_Message_Block(SIZE_MSG_BLOCK);

    *((UINT4 *)(mb->wr_ptr())) = (UINT4)pMsgIn;
    mb->wr_ptr(SIZE_MSG_BLOCK);

    ACE_Time_Value tv(1);

    /* ��������,1�볬ʱ */
    if(m_MsgQue.enqueue_head(mb,&tv) == -1)
    {
        delete mb;
        return ERR_FAIL;
    }

    m_pMsgEvent->signal();
    return ERR_SUCCESS;
}

/*
    ��������:ע��ʱ��.

    ����:
    nInterval,ʱ�ӵ�ʱ��,��λ����.

    ����:
    ����ʱ��ID,û��ʧ�ܷ���.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
*/
long CMsgProcessor::SetTimer(int nInterval)
{
    ASSERT(nInterval >= 0);

    STimerStatus *pTimerStatus = new STimerStatus;
    pTimerStatus->TimerID = 0;
    pTimerStatus->flgTrigger = FALSE;

    /*
        ע�ᶨʱ���������m_TimerMutex�����Ĵ���֮��,��������.
        ��Ϊ��������һ�����:��ʱ��������ͬʱ,��SetTimer.
    */
    m_theTimerMgr.SetTimer(this,nInterval,&(pTimerStatus->TimerID));

    m_TimerMutex.acquire();
    m_TimerStatusTab.push_back(pTimerStatus);
    m_TimerMutex.release();

    return pTimerStatus->TimerID;
}

/*
    ��������:ע��ʱ��.

    ����:
    nTimerID,ʱ��ID.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
    zhouyonghui,2001-12-21,�����������.
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
        ע����ʱ���������m_TimerMutex�����Ĵ���֮��,��������.
        ��Ϊ��������һ�����:��ʱ��������ͬʱ,��KillTimer.
    */
    if(bMatched == TRUE)
    {
        m_theTimerMgr.KillTimer(nTimerID);
    }
}

/*
    ��������:ע������ʱ��.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
*/
void CMsgProcessor::KillTimer()
{
    /*
        ע����ʱ���������m_TimerMutex�����Ĵ���֮��,��������.
        ��Ϊ��������һ�����:��ʱ��������ͬʱ,��KillTimer.
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
    ��������:����ʱ����Ϣ.

    1.ʱ����Ϣ����������ȼ�,ֻ����û���κ�������Ϣ������²Żᱻ����.
    2.����Ϊÿһ��������ʱ�ӵ���һ��OnTimer()����.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
*/
void CMsgProcessor::OnIdle(void)
{
    LISTTIMERSTATUS lstTriggeredTimer;

    /*
        �ռ��Ѿ������Ķ�ʱ��.
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
        ����ʱ���¼�.
    */
    for(i = lstTriggeredTimer.begin();i != lstTriggeredTimer.end();i++)
    {
        OnTimer((*i)->TimerID);
    }
}

/*
    ��������:��Ϣѭ������.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
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
    ��������:ʱ����Ϣ���.

    1.�÷�����CTimerManager�������,ÿ����ʱ������,
      �͵��øú������ô�����־.

    ����:
    current_time,��ǰʱ��.
    act,ʱ��IDָ��.

    ����:�ú�����ACE_Event_Handler���Ա����������,ֻ����0.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
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
    ��������:�̺߳���,������Ϣ��������Schedule()����������Ϣѭ��.

    ����:
    pModule,��Ϣ��������ָ��.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
*/
void CMsgProcessor::ScheduleThreadProc(CMsgProcessor* pModule)
{
    ASSERT(pModule != NULL);
    pModule->Schedule();
}

/*
    ��������:�����߳�.

    ����:�����̳߳ɹ�,����TRUE;���򷵻�FALSE.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
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
    ��������:�ȴ��߳̽���.

    ����:
    nTimeOut,�ȴ��߳̽����ĳ�ʱʱ��,Ϊ0��һֱ�ȴ�ֱ������.

    ����:�̵߳���ֹ״̬.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
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
    
    //���ⵥ��SWPD00562��2003��7��23�գ�Zou Yongsheng�޸�
    if((nMsgOutLevel & g_nMsgOutLevel) == 0)
	{
		return;
	}
	//�޸Ľ���

    va_start(argptr, szFmt);
    VSNPRINTF(pBuf,1024,szFmt,argptr);
    pBuf[1023] = 0;

    va_end(argptr);

    //������ 2002-05-20 ���ⵥD13657
	char szTmp[48];
	SNPRINTF(szTmp,48,"(%s)%s",g_szPtDes,szModuleName);
	szTmp[47] = 0;
    /* ����MML����. */
     
    CMMLReport Report(szTmp);//������ 2002-05-20 ���ⵥD13657
    Report.MakeReportHead();

    /* ����ͷ�ڶ��� */

    Report.AddString(S_FRAME_MML_REPORT_HEADER_PART1);
    //by ldf 2002.6.10 ��Ӧ���ⵥD14393
    Report.FillSpace(4);
    //�޸����

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

    //by ldf 2002.6.10 ��Ӧ���ⵥD14393
    Report.FillSpace(8);
    Report.AddString(S_FRAME_MML_REPORT_HEADER_PART2);
    Report.FillSpace(3);
    //�޸����

    Report.AddString(STR_NEWLINE);

    /* ���� */
    Report.MakeReportItem2(S_FRAME_DEBUG_CONTENT,pBuf);
    Report.MakeReportItem2(S_FRAME_DEBUG_INFO,szModuleName);

    /* ����β */
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
    ��������:��MML Server���͸澯��Ϣ.
    ��������ʧ�ܵ����,������ʧ����Ϣ������.

    ����:
    pAlarm,�澯��Ϣָ��.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
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
    ��������:���Լ������˳���Ϣ.
    ��������ʧ�ܵ����,������ʧ����Ϣ������.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
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
    ��������:����־ģ�鷢����־��Ϣ.
    ��������ʧ�ܵ����,������ʧ����Ϣ������.

    ����:
    szModuleName,������Ϣ��ģ����.
    szSysLog,��Ϣ�ı�.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
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

    //������ 2002-05-20 ���ⵥD13657
	SNPRINTF(pLog->szUserName,48,"(%s)%s",g_szPtDes,szModuleName);
    //�޸Ľ���
    pLog->szUserName[47] = 0;

    pLog->LogType = (BYTE)(SLog::LOG_SYS);
    pLog->uIPAddr = 0;
    //������ 2002-03-13 ���ⵥ D11606
    pLog->OperResult = bLogResult;
    //over
    pLog->szOperCmd[0] = '\0';
    strncpy(pLog->szCmdDesc, szSysLog,LOG_CMD_DESC_LEN);
    pLog->szCmdDesc[LOG_CMD_DESC_LEN-1] = 0;

    SendMsg(pMsg);
}

/*
    ��������:����־ģ�鷢����־��Ϣ.
    ��������ʧ�ܵ����,������ʧ����Ϣ������.

    ����:
    pLog,��־��Ϣ�ṹ.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
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
	//������ 2002-05-20 ���ⵥD13657
	if(pLog->LogType == SLog::LOG_SYS)
	{	
		SLog *pTmp = (SLog *)(pMsg->Data);
		char szTmp[48];
		SNPRINTF(szTmp,48,"(%s)%s",g_szPtDes,pLog->szUserName);
		szTmp[47] = 0;
		strncpy(pTmp->szUserName,szTmp,48);
	}
	//�޸Ľ���

    SendMsg(pMsg);
}

/*
    ��������:��Agentģ�鷢�͵�����Ϣ.
    ��������ʧ�ܵ����,������ʧ����Ϣ������.

    ����:
    mt,���뵹����ģ���.
    nCause,���뵹����ԭ��,����ԭ������basetype.h��.
    pData,����ʱ���ܷ��͸��Զ˷�����������.
    nLen,pData���ݵĳ���.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
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
    ��������:��Agentģ�鷢��ͬ������.
    ��������ʧ�ܵ����,������ʧ����Ϣ������.

    ����:
    mt,���뵹����ģ���.
    pData,ͬ������.
    nLen,ͬ�����ݵĳ���.
    nChlID,ͨ����,��mt����MT_BACKSAVEʱ�ò���������.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
*/
void CMsgProcessor::WriteSyncData(const MODULE_TYPE mt,
                                  const void* pData,
                                  const UINT4 nLen,
                                  const UINT4 nChlID)
{
    MSG_INTRA *pMsg = new(sizeof(UINT4) + nLen) MSG_INTRA;
    pMsg->SenderPid = GetProcessType();
    pMsg->SenderMid = mt;
    //by ldf 2002.5.21 ��Ӧ���ⵥD13761
    pMsg->ReceiverPid = PT_CLSTR;
    pMsg->ReceiverMid = MT_CLSTR;
	//�޸����
    pMsg->CmdCode = CMD_SYNC_DATA;
    memcpy(pMsg->Data, &nChlID, sizeof(UINT4));
    memcpy(pMsg->Data + sizeof(UINT4), pData, nLen);

    SendMsg(pMsg);
}

void CMsgProcessor::OnTimer(const long nTimerID)
{
    /* ȱʡ������ʱ����Ϣ. */
}
