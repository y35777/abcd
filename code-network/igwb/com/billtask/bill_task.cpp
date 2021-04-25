#include "../include/base_type.h"
#include "bill_task.h"

/********************************************************
* ����: CBillTask::CBillTask
* ����: ���캯��
* ����: nTaskID : �����
* ���: 
* ����: 
********************************************************/
CBillTask::CBillTask(int nTaskID)
{
    m_nTaskType     = TK_UNKNOWN;
    m_nTaskID       = nTaskID;
    m_bActive       = FALSE;
    m_bAskForExit   = FALSE;
}

/********************************************************
* ����: CBillTask::~CBillTask
* ����: ��������
* ����: 
* ���: 
* ����: 
********************************************************/
CBillTask::~CBillTask()
{
    //
}

/********************************************************
* ����: CBillTask::svc
* ����: ActiveObject������̺߳�����ͨ��run����������
* ����: 
* ���: 
* ����: 
********************************************************/
int CBillTask::svc()
{
    ProcBillTask();

    m_bActive = FALSE;

    return 0;
}

/********************************************************
* ����: CBillTask::WaitForExit
* ����: �ȴ��˳��Ĳ���
* ����: 
* ���: 
* ����: 
********************************************************/
void CBillTask::WaitForExit()
{
    m_bAskForExit = TRUE;
    
    wait();
}

/********************************************************
* ����: CBillTask::GetActiveValue
* ����: ȡ����״̬��ֵ���������Ƿ���������
* ����: 
* ���: 
* ����: �Ƿ񼤻�
********************************************************/
BOOL CBillTask::GetActiveValue()
{
    return m_bActive;
}

/********************************************************
* ����: CBillTask::SetActiveValue
* ����: ���ü���״̬��ֵ
* ����: �Ƿ񼤻�
* ���: 
* ����: 
********************************************************/
void CBillTask::SetActiveValue(BOOL bActive)
{
    m_bActive = bActive;
}
