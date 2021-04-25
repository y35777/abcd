#ifndef __CLUSTER_COMM_TASK_H__
#define __CLUSTER_COMM_TASK_H__

#include "ace/Task.h"

class CClusterCommTask:public ACE_Task_Base
{
/*
    成员函数
*/
public:
    CClusterCommTask(CClusterNetwork* pNetwork);
    virtual ~CClusterCommTask();

    //本函数会启动一个用于检测Reactor事件的线程
    virtual int open(void* args = 0);

    //线程函数,本函数是提供一个Reactor的事件循环
    virtual int svc(void);

    //将CSocketImpl子类的对象注册到本对象的ACE_Reactor对象中
    virtual int RegisterHandle(ACE_Event_Handler* pSocket,
                               ACE_Reactor_Mask mask);

    //结束线程
    void QuitCommTask(void);

/*
    成员变量
*/
protected:
    //ACE_Reactor对象
    ACE_Reactor         m_Reactor;

    //CClusterNetwork对象的指针
    CClusterNetwork*    m_pNetwork;

    //秒定时器的起始时间
    ACE_Time_Value      m_Timer;
};

#endif //__CLUSTER_COMM_TASK_H__
