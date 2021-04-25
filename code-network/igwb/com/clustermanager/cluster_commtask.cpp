#include "../include/base_type.h"
#include "cluster_network.h"
#include "cluster_commtask.h"
#include <assert.h>
#include "ace/High_Res_Timer.h"

/********************************************************
* ����: CClusterCommTask::CClusterCommTask
* ����: ���캯��
* ����: pNetwork : CClusterNetwork�����ָ��
* ���:
* ����:
********************************************************/
CClusterCommTask::CClusterCommTask(CClusterNetwork* pNetwork)
{
    assert(NULL != pNetwork);
    m_pNetwork = pNetwork;

	ACE_Timer_Queue* Tq = m_Reactor.timer_queue();
	ACE_High_Res_Timer::global_scale_factor (); 
    Tq->gettimeofday (&ACE_High_Res_Timer::gettimeofday_hr);
}

/********************************************************
* ����: CClusterCommTask::~CClusterCommTask
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CClusterCommTask::~CClusterCommTask()
{
    m_pNetwork = NULL;
}

/********************************************************
* ����: CClusterCommTask::open
* ����: ������������һ�����ڼ��Reactor�¼����߳�
* ����: args :
* ���:
* ����: int �����Ƿ�ɹ�,0��ʾ�ɹ�,����ֵ��ʾʧ��
********************************************************/
int CClusterCommTask::open(void* args)
{
    //����ACE_Task_Base�ṩ��activate����
    return activate();
}

/********************************************************
* ����: CClusterCommTask::svc
* ����: �̺߳���,���������ṩһ��Reactor���¼�ѭ��
* ����:
* ���:
* ����: int
********************************************************/
int CClusterCommTask::svc()
{
    m_Reactor.owner(ACE_Thread::self());
    ACE_Time_Value time_out = ACE_Time_Value(1);
    m_Timer = ACE_High_Res_Timer::gettimeofday();

    while(1)
    {
        if (!m_Reactor.reactor_event_loop_done())
        {
            m_Reactor.handle_events(time_out);
        }
        else
        {
            //�˳�ѭ��
            break;
        }

        ACE_Time_Value ace_now = ACE_High_Res_Timer::gettimeofday();
        time_out = ace_now - m_Timer;
        if(time_out >= ACE_Time_Value(1))
        {
            m_pNetwork->Timer1Sec();
            m_Timer  = ace_now;
            time_out = ACE_Time_Value(1);
        }
        else
        {
            time_out = ACE_Time_Value(1) - time_out;
        }
    }

    return 0;
}

/********************************************************
* ����: CClusterCommTask::RegisterHandle
* ����: ��CSocketImpl����Ķ���ע�ᵽ�������ACE_Reactor������
* ����: ACE_Event_Handler* pSocket : ��ע���Socket�����ָ��
*       ACE_Reactor_Mask mask      : ��ע����¼�
* ���:
* ����: int ע���Ƿ�ɹ�
********************************************************/
int CClusterCommTask::RegisterHandle(ACE_Event_Handler* pSocket,
                                     ACE_Reactor_Mask mask)
{
    int nRegResult = m_Reactor.register_handler(pSocket, mask);

    return nRegResult;
}

/********************************************************
* ����: CClusterCommTask::QuitCommTask
* ����: �����߳�
* ����:
* ���:
* ����: void
********************************************************/
void CClusterCommTask::QuitCommTask()
{
    m_Reactor.end_reactor_event_loop();
    //����ACE_Task_Base�ṩ��wait�����ȴ��߳̽���
    wait();
}