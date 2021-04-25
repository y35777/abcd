#include "../include/base_type.h"
#include "cluster_network.h"
#include "cluster_commtask.h"
#include <assert.h>
#include "ace/High_Res_Timer.h"

/********************************************************
* 名称: CClusterCommTask::CClusterCommTask
* 功能: 构造函数
* 输入: pNetwork : CClusterNetwork对象的指针
* 输出:
* 返回:
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
* 名称: CClusterCommTask::~CClusterCommTask
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CClusterCommTask::~CClusterCommTask()
{
    m_pNetwork = NULL;
}

/********************************************************
* 名称: CClusterCommTask::open
* 功能: 本函数会启动一个用于检测Reactor事件的线程
* 输入: args :
* 输出:
* 返回: int 操作是否成功,0表示成功,非零值表示失败
********************************************************/
int CClusterCommTask::open(void* args)
{
    //调用ACE_Task_Base提供的activate函数
    return activate();
}

/********************************************************
* 名称: CClusterCommTask::svc
* 功能: 线程函数,本函数是提供一个Reactor的事件循环
* 输入:
* 输出:
* 返回: int
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
            //退出循环
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
* 名称: CClusterCommTask::RegisterHandle
* 功能: 将CSocketImpl子类的对象注册到本对象的ACE_Reactor对象中
* 输入: ACE_Event_Handler* pSocket : 待注册的Socket对象的指针
*       ACE_Reactor_Mask mask      : 待注册的事件
* 输出:
* 返回: int 注册是否成功
********************************************************/
int CClusterCommTask::RegisterHandle(ACE_Event_Handler* pSocket,
                                     ACE_Reactor_Mask mask)
{
    int nRegResult = m_Reactor.register_handler(pSocket, mask);

    return nRegResult;
}

/********************************************************
* 名称: CClusterCommTask::QuitCommTask
* 功能: 结束线程
* 输入:
* 输出:
* 返回: void
********************************************************/
void CClusterCommTask::QuitCommTask()
{
    m_Reactor.end_reactor_event_loop();
    //调用ACE_Task_Base提供的wait函数等待线程结束
    wait();
}