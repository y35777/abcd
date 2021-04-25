/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:msg_proc.h
    ������Ϣ:
    
    ����CMsgProcessor��Ķ���,����������������Ҫ������Ϣ��ĸ���.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/

#ifndef _MESSAGE_PROCESSOR_H
#define _MESSAGE_PROCESSOR_H

//by ldf 2002.6.7 ��Ӧ���ⵥD14393
#include "../include/toolbox.h"
//�޸����

/* ����ȫ�ֶ���ĳ��������ݽṹ */
#include "../include/precom.h"

/* CTimerManager��Ķ���ͷ�ļ� */
#include "tm_manager.h"

#define PROCESS_NAME_LEN        128

class CCommLayer;
class CMsgRouter;

class CMsgProcessor:public ACE_Event_Handler
{
protected:
    struct STimerStatus
    {
        long TimerID;       //��ʱ��ID
        BOOL flgTrigger;    //��ʱ��������־,TRUE��ʾ�Ѵ���
    };

    typedef std::list<STimerStatus*> LISTTIMERSTATUS;
    
public:
    CMsgProcessor(PROCESS_TYPE pt);
    CMsgProcessor();
    virtual ~CMsgProcessor();

    /* ·�ɱ� */
    static CMsgRouter* m_ptheRouter;

    /* ͨ�Ų� */
    static CCommLayer* m_ptheCommLayer;

    /* ����Э����ٱ�־ */
    static BOOL g_bTrackProtocol;

    /* ���е�����Ϣ�ļ���,ȡֵ�����Ǹ��ּ���İ�λ�� */
    static BYTE g_nMsgOutLevel;

    /* ��������,����˵���̱�� */
    static PROCESS_TYPE g_pt;
	//������ 2002-05-20 ���ⵥD13657
	//������������
    static char g_szPtDes[32];
	//�޸Ľ���
    /* ������ */
    static char g_szPt[PROCESS_NAME_LEN];

    /*���������ͳ�Ʊ�־ */
    static BOOL g_bPerfIterm;

    static PROCESS_TYPE GetProcessType();
    static const char *GetModuleDesp(MODULE_TYPE mt);
    static const char *GetProcessDesp(PROCESS_TYPE pt);

    /* ��Ϣ���,��CMsgRouter�������,����Ϣ������Ϣ����β��. */
    int Enqueue(MSG_INTRA* pMsgIn);

    /* ��Ϣ���,��CMsgRouter�������,����Ϣ������Ϣ����ͷ��. */
    int EnqueueHead(MSG_INTRA* pMsgIn);

    /* 
        ������Ϣ,�÷�������CMsgRouter�����route����,
        ����Ϣ·�ɵ�Ŀ��ģ��.
    */
    static int SendMsg(MSG_INTRA* pMsgOut);

    /* ����ʱ����Ϣ */
    virtual int handle_timeout(const ACE_Time_Value &current_time,
                               const void * act = 0);

    /* �����߳� */
    int Run(void);

    /* �ȴ��߳̽��� */
    int Wait(const long nTimeOut = 0);

    static void TrackProtocol(const char* pMsgProtocol);

    static void MessageOut(const char* szModuleName,
                            const BYTE nMsgOutLevel, 
                            const char* szFmt, ...);
    // ������ 2002-03-13 ���ⵥD11606
    static void WriteLog(const char* szModuleName,
                         const char* szSysLog, 
                         BYTE bLogResult = SLog::OPER_SUCCESS);

    static void WriteLog(const SLog* pLog);

    /* ��˫Agentģ�鷢�͵������� */
    static void AskForFailOver(const MODULE_TYPE mt, 
                               const BYTE nCause, 
                               const void* pData = 0, 
                               const UINT4 nLen = 0);

    /* ��Agentģ�鷢��ͬ������ */
    static void WriteSyncData(const MODULE_TYPE mt,
                              const void* pData,
                              const UINT4 nLen,
                              const UINT4 nChlID = 0);

    virtual MODULE_TYPE GetModuleType() = 0;

    /* ��澯ģ�鷢�͸澯��Ϣ */
    static void SendAlarm(const SInnerAlarm* pAlarm);

protected:

    /* ��Ϣ���� */
    ACE_Message_Queue<ACE_MT_SYNCH> m_MsgQue;

    /* ��ʱ������״̬�� */
    LISTTIMERSTATUS m_TimerStatusTab;

    /* ��ʱ������ */
    static CTimerManager m_theTimerMgr;

    /* ���ڱ�����ʱ������״̬��Ļ����� */
    ACE_Mutex m_TimerMutex;

    /* ��ʱ���̶߳��� */
    ACE_Thread_Manager m_Tgr;


    virtual int InitInstance(void);
    virtual void ExitInstance(void);

    long SetTimer(int nInterval);
    void KillTimer(const long nTimerID);
    virtual void OnTimer(const long nTimerID);

    virtual void ParseMsg(MSG_INTRA* pMsgParsed){};

    void SendQuitToMe();

protected:
    ACE_Auto_Event *m_pMsgEvent;

    void KillTimer();
    void OnIdle(void);
    void Schedule();

    /* ��Ϣѭ���̺߳���. */
    static void ScheduleThreadProc(CMsgProcessor* pModule);
};

#define MSGOUT CMsgProcessor::MessageOut
#define THISMODULE CMsgProcessor::GetModuleDesp(this->GetModuleType())

#endif

