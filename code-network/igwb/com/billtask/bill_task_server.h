#ifndef __BILL_TASK_SERVER_H__
#define __BILL_TASK_SERVER_H__

#include "../include/frame.h"
#include "bill_task.h"

class CBillTaskServer : public CWorkModule
{
public:

    CBillTaskServer();
    ~CBillTaskServer();

protected:

    //ȡģ���
    virtual MODULE_TYPE GetModuleType();

    //��ʼ��
    virtual int InitInstance();

    //ɨβ����
    virtual void ExitInstance();

    //��������ͷַ�
    virtual void ParseMsg(MSG_INTRA* pMsgParsed);

    //��ʱ���¼��Ĵ�����ں���
    virtual void OnTimer(const long nTimerID);

    //5�붨ʱ���¼��Ĵ�����ں���
    void Timer5Sec();

    //SCP�����ָ�������
    void ProcScpRecovery(MSG_INTRA* pMsgParsed);

    //SCP�رո澯��������
    void ProcScpCloseAlarm(MSG_INTRA* pMsgParsed);

    //�����ָ��������
    LIST<CBillTask*>	    m_TaskList;

    //���ʱ�ʶ�������ŵ������ŵ�MAP
    MAP<SScpApplyKey, int>	m_MapDpctoAP;

    //5�붨ʱ��ID
    long	                m_lTimer5Sec;

    //��ǰ����ID��
    int                 m_nTaskID;
};

#endif //__BILL_TASK_SERVER_H__
