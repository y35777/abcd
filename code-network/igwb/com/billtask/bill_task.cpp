#include "../include/base_type.h"
#include "bill_task.h"

/********************************************************
* 名称: CBillTask::CBillTask
* 功能: 构造函数
* 输入: nTaskID : 任务号
* 输出: 
* 返回: 
********************************************************/
CBillTask::CBillTask(int nTaskID)
{
    m_nTaskType     = TK_UNKNOWN;
    m_nTaskID       = nTaskID;
    m_bActive       = FALSE;
    m_bAskForExit   = FALSE;
}

/********************************************************
* 名称: CBillTask::~CBillTask
* 功能: 析构函数
* 输入: 
* 输出: 
* 返回: 
********************************************************/
CBillTask::~CBillTask()
{
    //
}

/********************************************************
* 名称: CBillTask::svc
* 功能: ActiveObject对象的线程函数，通过run函数来启动
* 输入: 
* 输出: 
* 返回: 
********************************************************/
int CBillTask::svc()
{
    ProcBillTask();

    m_bActive = FALSE;

    return 0;
}

/********************************************************
* 名称: CBillTask::WaitForExit
* 功能: 等待退出的操作
* 输入: 
* 输出: 
* 返回: 
********************************************************/
void CBillTask::WaitForExit()
{
    m_bAskForExit = TRUE;
    
    wait();
}

/********************************************************
* 名称: CBillTask::GetActiveValue
* 功能: 取激活状态的值，即任务是否正在运行
* 输入: 
* 输出: 
* 返回: 是否激活
********************************************************/
BOOL CBillTask::GetActiveValue()
{
    return m_bActive;
}

/********************************************************
* 名称: CBillTask::SetActiveValue
* 功能: 设置激活状态的值
* 输入: 是否激活
* 输出: 
* 返回: 
********************************************************/
void CBillTask::SetActiveValue(BOOL bActive)
{
    m_bActive = bActive;
}
