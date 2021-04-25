/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:msg_proc.h
    描述信息:
    
    包含CMsgProcessor类的定义,该类是所有其他需要处理消息类的父类.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-16,创建.
*/

#ifndef _MESSAGE_PROCESSOR_H
#define _MESSAGE_PROCESSOR_H

//by ldf 2002.6.7 对应问题单D14393
#include "../include/toolbox.h"
//修改完毕

/* 包含全局定义的常量和数据结构 */
#include "../include/precom.h"

/* CTimerManager类的定义头文件 */
#include "tm_manager.h"

#define PROCESS_NAME_LEN        128

class CCommLayer;
class CMsgRouter;

class CMsgProcessor:public ACE_Event_Handler
{
protected:
    struct STimerStatus
    {
        long TimerID;       //定时器ID
        BOOL flgTrigger;    //定时器触发标志,TRUE表示已触发
    };

    typedef std::list<STimerStatus*> LISTTIMERSTATUS;
    
public:
    CMsgProcessor(PROCESS_TYPE pt);
    CMsgProcessor();
    virtual ~CMsgProcessor();

    /* 路由表 */
    static CMsgRouter* m_ptheRouter;

    /* 通信层 */
    static CCommLayer* m_ptheCommLayer;

    /* 进行协议跟踪标志 */
    static BOOL g_bTrackProtocol;

    /* 运行调试信息的级别,取值可以是各种级别的按位或 */
    static BYTE g_nMsgOutLevel;

    /* 进程类型,或者说进程编号 */
    static PROCESS_TYPE g_pt;
	//张玉新 2002-05-20 问题单D13657
	//进程类型描述
    static char g_szPtDes[32];
	//修改结束
    /* 进程名 */
    static char g_szPt[PROCESS_NAME_LEN];

    /*否进行性能统计标志 */
    static BOOL g_bPerfIterm;

    static PROCESS_TYPE GetProcessType();
    static const char *GetModuleDesp(MODULE_TYPE mt);
    static const char *GetProcessDesp(PROCESS_TYPE pt);

    /* 消息入队,由CMsgRouter对象调用,把消息放入消息队列尾部. */
    int Enqueue(MSG_INTRA* pMsgIn);

    /* 消息入队,由CMsgRouter对象调用,把消息放入消息队列头部. */
    int EnqueueHead(MSG_INTRA* pMsgIn);

    /* 
        发送消息,该方法调用CMsgRouter对象的route方法,
        把消息路由到目标模块.
    */
    static int SendMsg(MSG_INTRA* pMsgOut);

    /* 处理时钟消息 */
    virtual int handle_timeout(const ACE_Time_Value &current_time,
                               const void * act = 0);

    /* 启动线程 */
    int Run(void);

    /* 等待线程结束 */
    int Wait(const long nTimeOut = 0);

    static void TrackProtocol(const char* pMsgProtocol);

    static void MessageOut(const char* szModuleName,
                            const BYTE nMsgOutLevel, 
                            const char* szFmt, ...);
    // 张玉新 2002-03-13 问题单D11606
    static void WriteLog(const char* szModuleName,
                         const char* szSysLog, 
                         BYTE bLogResult = SLog::OPER_SUCCESS);

    static void WriteLog(const SLog* pLog);

    /* 向双Agent模块发送倒换命令 */
    static void AskForFailOver(const MODULE_TYPE mt, 
                               const BYTE nCause, 
                               const void* pData = 0, 
                               const UINT4 nLen = 0);

    /* 向Agent模块发送同步数据 */
    static void WriteSyncData(const MODULE_TYPE mt,
                              const void* pData,
                              const UINT4 nLen,
                              const UINT4 nChlID = 0);

    virtual MODULE_TYPE GetModuleType() = 0;

    /* 向告警模块发送告警消息 */
    static void SendAlarm(const SInnerAlarm* pAlarm);

protected:

    /* 消息队列 */
    ACE_Message_Queue<ACE_MT_SYNCH> m_MsgQue;

    /* 定时器触发状态表 */
    LISTTIMERSTATUS m_TimerStatusTab;

    /* 定时器对象 */
    static CTimerManager m_theTimerMgr;

    /* 用于保护定时器触发状态表的互斥锁 */
    ACE_Mutex m_TimerMutex;

    /* 定时器线程对象 */
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

    /* 消息循环线程函数. */
    static void ScheduleThreadProc(CMsgProcessor* pModule);
};

#define MSGOUT CMsgProcessor::MessageOut
#define THISMODULE CMsgProcessor::GetModuleDesp(this->GetModuleType())

#endif

