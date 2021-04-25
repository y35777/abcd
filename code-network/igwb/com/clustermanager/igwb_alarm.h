#ifndef __CLUSTER_IGWB_ALARM_H__
#define __CLUSTER_IGWB_ALARM_H__

#include "cluster_alarm.h"

class CIGWBAlarm : public CClusterAlarm
{
public:

    CIGWBAlarm();
    virtual ~CIGWBAlarm();

    //˫���¼���Ϣ������
    virtual void OnNotify(ECLUSTER_NOTIFY uMsg,
                          UINT4 uParam1 = 0,
                          UINT4 uParam2 = 0);
    //���¼�������
    virtual void Timer1Sec();

protected:
    //������ʷ�澯֡
    void AddHisAlarm(SInnerAlarm& alarm);

    //ɾ����ʷ�澯֡
    void RmvHisAlarm(SInnerAlarm& alarm);

    //��ʷ�澯֡��LIST
    LIST<SInnerAlarm> m_HisAlarmList;

    //�Ƿ��ڵ�������״̬
    BOOL m_bSwitchFault;
};


#endif //__CLUSTER_IGWB_ALARM_H__
