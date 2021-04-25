#ifndef __CLUSTER_COMM_TASK_H__
#define __CLUSTER_COMM_TASK_H__

#include "ace/Task.h"

class CClusterCommTask:public ACE_Task_Base
{
/*
    ��Ա����
*/
public:
    CClusterCommTask(CClusterNetwork* pNetwork);
    virtual ~CClusterCommTask();

    //������������һ�����ڼ��Reactor�¼����߳�
    virtual int open(void* args = 0);

    //�̺߳���,���������ṩһ��Reactor���¼�ѭ��
    virtual int svc(void);

    //��CSocketImpl����Ķ���ע�ᵽ�������ACE_Reactor������
    virtual int RegisterHandle(ACE_Event_Handler* pSocket,
                               ACE_Reactor_Mask mask);

    //�����߳�
    void QuitCommTask(void);

/*
    ��Ա����
*/
protected:
    //ACE_Reactor����
    ACE_Reactor         m_Reactor;

    //CClusterNetwork�����ָ��
    CClusterNetwork*    m_pNetwork;

    //�붨ʱ������ʼʱ��
    ACE_Time_Value      m_Timer;
};

#endif //__CLUSTER_COMM_TASK_H__
