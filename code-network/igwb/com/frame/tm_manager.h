/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:tm_manager.h
    ������Ϣ:
    
    ����CTimerManager��Ķ���.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
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
    
    /* Reactor�����ָ��,���ڲ�����ʱ���¼�. */
    ACE_Reactor m_TimerReactor;

    /* ACE�̹߳������. */
    ACE_Thread_Manager m_Tgr;

    /* �̺߳���. */
    static void TimerMgrThreadProc(ACE_Reactor* pTimerReactor);

};

#endif
