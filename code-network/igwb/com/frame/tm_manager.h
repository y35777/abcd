/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:tm_manager.h
    描述信息:
    
    包含CTimerManager类的定义.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-16,创建.
*/

#ifndef _TIMER_MANAGER_H
#define _TIMER_MANAGER_H

#include "../include/precom.h"

class CTimerManager
{
public:
    CTimerManager();
    virtual ~CTimerManager();

    void SetTimer(ACE_Event_Handler* pTimerHandler, 
                    int nInterval, 
                    long* const pTimerID = 0);
    void KillTimer(const long nTimerID);
    void ResetTimer(const long nTimerID, const int nInterval);

    void Run();
    void Stop();

protected:
    
    /* Reactor对象的指针,用于产生定时器事件. */
    ACE_Reactor m_TimerReactor;

    /* ACE线程管理对象. */
    ACE_Thread_Manager m_Tgr;

    /* 线程函数. */
    static void TimerMgrThreadProc(ACE_Reactor* pTimerReactor);

};

#endif
