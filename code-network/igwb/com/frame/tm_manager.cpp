/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:tm_manager.cpp
    描述信息:
    
    包含CTimerManager类的实现.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-16,创建.
*/
#include "tm_manager.h"
#include "msg_proc.h"
#include "ace/High_Res_Timer.h"

CTimerManager::CTimerManager()
{
	ACE_Timer_Queue* Tq = m_TimerReactor.timer_queue();
	ACE_High_Res_Timer::global_scale_factor (); 
    Tq->gettimeofday (&ACE_High_Res_Timer::gettimeofday_hr);
}

CTimerManager::~CTimerManager()
{
}

/*
    函数功能:注册定时器.

    参数:
    pTimerHandler,定时器消息相应对象的指针.
    nInterval,定时器的时长.
    pTimerID,输出定时器ID.

    修改记录:
    周拥辉,2001-10-16,创建.
*/
void CTimerManager::SetTimer(ACE_Event_Handler* pTimerHandler, 
                             int nInterval, 
                             long* const pTimerID)
{
    ACE_Time_Value tm_val;
    tm_val.msec(nInterval);
    
    if (pTimerID == NULL)
    {
        m_TimerReactor.schedule_timer(pTimerHandler, 
                        (const void *)0, 
                        tm_val,
                        tm_val);
    }
    else
    {
        *pTimerID = m_TimerReactor.schedule_timer(pTimerHandler, 
                        (const void *)pTimerID, 
                        tm_val,
                        tm_val);
    }
}

/*
    函数功能:注销指定的定时器.

    参数:
    nTimerID,定时器ID.

    修改记录:
    周拥辉,2001-10-16,创建.
*/
void CTimerManager::KillTimer(const long nTimerID)
{
    m_TimerReactor.cancel_timer(nTimerID);
}

/*
    函数功能:重新注册指定的定时器.

    参数:
    nTimerID,定时器ID.
    nInterval,新的时长.

    修改记录:
    周拥辉,2001-10-16,创建.
*/
void CTimerManager::ResetTimer(const long nTimerID,
                               const int nInterval)
{
    ACE_Time_Value tm_val;
    tm_val.msec(nInterval);
    m_TimerReactor.reset_timer_interval(nTimerID, tm_val);
}

/*
    函数功能:定时器消息循环函数.

    参数:
    pTimerReactor,产生定时器消息的对象.

    修改记录:
    周拥辉,2001-10-16,创建.
*/
void CTimerManager::TimerMgrThreadProc(ACE_Reactor* pTimerReactor)
{
    pTimerReactor->owner(ACE_Thread::self());

    while(TRUE)
    {
        if (!pTimerReactor->reactor_event_loop_done())
        {
            pTimerReactor->handle_events();
        }
        else
        {
            break;
        }
    }
}

/*
    函数功能:启动定时器线程.
    若启动线程失败,写Trace.

    修改记录:
    周拥辉,2001-10-16,创建.
*/
void CTimerManager::Run()
{
    if(m_Tgr.spawn((ACE_THR_FUNC) TimerMgrThreadProc,
                    (void *)&m_TimerReactor) == -1)
    {
        TRACE(MTS_MANAGER,S_FRAME_TM_MGR_START_THR_FAIL);
        //End process?
    }
}

/*
    函数功能:停止定时器线程.

    修改记录:
    周拥辉,2001-10-16,创建.
*/
void CTimerManager::Stop()
{
    m_TimerReactor.end_reactor_event_loop();
    m_Tgr.wait(0);
}
