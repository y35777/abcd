#ifndef __BILL_TASK_H__
#define __BILL_TASK_H__

#include "ace/Task.h"
#include "bill_task_struct.h"

class CBillTask : public ACE_Task_Base
{
public:
    CBillTask(int nTaskID);
    virtual ~CBillTask();

    //��ʼ��
    virtual int Init() = 0;

    //ͨ����Ϣ֡�ж��Ƿ���ͬһ����
    virtual BOOL IsMatchedTask(MSG_INTRA* pMsg) = 0;

    //�ȴ��˳��Ĳ���
    void WaitForExit();

    //ȡ����״̬��ֵ���������Ƿ���������
    BOOL GetActiveValue();

    //���ü���״̬��ֵ
    void SetActiveValue(BOOL bActive);

protected:

    //ActiveObject������̺߳�����ͨ��run����������
    virtual int svc();

    //ʵʩ�ض�����
    virtual void ProcBillTask() = 0;

    //�Ƿ��ڼ���״̬
    BOOL        m_bActive;

    //��������
    TASK_TYPE   m_nTaskType;

    //����ID��
    int         m_nTaskID;

    //����ö������������Ҫ���߳��˳�������Ҫ�øó�Ա����ΪTRUE
    BOOL        m_bAskForExit;

};

#endif //__BILL_TASK_H__
