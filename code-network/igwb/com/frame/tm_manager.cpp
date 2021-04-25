/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:tm_manager.cpp
    ������Ϣ:
    
    ����CTimerManager���ʵ��.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
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
    ��������:ע�ᶨʱ��.

    ����:
    pTimerHandler,��ʱ����Ϣ��Ӧ�����ָ��.
    nInterval,��ʱ����ʱ��.
    pTimerID,�����ʱ��ID.

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
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
    ��������:ע��ָ���Ķ�ʱ��.

    ����:
    nTimerID,��ʱ��ID.

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/
void CTimerManager::KillTimer(const long nTimerID)
{
    m_TimerReactor.cancel_timer(nTimerID);
}

/*
    ��������:����ע��ָ���Ķ�ʱ��.

    ����:
    nTimerID,��ʱ��ID.
    nInterval,�µ�ʱ��.

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/
void CTimerManager::ResetTimer(const long nTimerID,
                               const int nInterval)
{
    ACE_Time_Value tm_val;
    tm_val.msec(nInterval);
    m_TimerReactor.reset_timer_interval(nTimerID, tm_val);
}

/*
    ��������:��ʱ����Ϣѭ������.

    ����:
    pTimerReactor,������ʱ����Ϣ�Ķ���.

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
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
    ��������:������ʱ���߳�.
    �������߳�ʧ��,дTrace.

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
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
    ��������:ֹͣ��ʱ���߳�.

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/
void CTimerManager::Stop()
{
    m_TimerReactor.end_reactor_event_loop();
    m_Tgr.wait(0);
}
