#ifndef __BILL_TASK_H__
#define __BILL_TASK_H__

#include "ace/Task.h"
#include "bill_task_struct.h"

class CBillTask : public ACE_Task_Base
{
public:
    CBillTask(int nTaskID);
    virtual ~CBillTask();

    //初始化
    virtual int Init() = 0;

    //通过消息帧判断是否是同一任务
    virtual BOOL IsMatchedTask(MSG_INTRA* pMsg) = 0;

    //等待退出的操作
    void WaitForExit();

    //取激活状态的值，即任务是否正在运行
    BOOL GetActiveValue();

    //设置激活状态的值
    void SetActiveValue(BOOL bActive);

protected:

    //ActiveObject对象的线程函数，通过run函数来启动
    virtual int svc();

    //实施特定操作
    virtual void ProcBillTask() = 0;

    //是否处于激活状态
    BOOL        m_bActive;

    //任务类型
    TASK_TYPE   m_nTaskType;

    //任务ID号
    int         m_nTaskID;

    //如果该对象的所有者需要本线程退出，则需要置该成员变量为TRUE
    BOOL        m_bAskForExit;

};

#endif //__BILL_TASK_H__
