#ifndef __CLUSTER_IGWB_ALARM_H__
#define __CLUSTER_IGWB_ALARM_H__

#include "cluster_alarm.h"

class CIGWBAlarm : public CClusterAlarm
{
public:

    CIGWBAlarm();
    virtual ~CIGWBAlarm();

    //双机事件信息处理函数
    virtual void OnNotify(ECLUSTER_NOTIFY uMsg,
                          UINT4 uParam1 = 0,
                          UINT4 uParam2 = 0);
    //秒事件处理函数
    virtual void Timer1Sec();

protected:
    //增加历史告警帧
    void AddHisAlarm(SInnerAlarm& alarm);

    //删除历史告警帧
    void RmvHisAlarm(SInnerAlarm& alarm);

    //历史告警帧的LIST
    LIST<SInnerAlarm> m_HisAlarmList;

    //是否在倒换故障状态
    BOOL m_bSwitchFault;
};


#endif //__CLUSTER_IGWB_ALARM_H__
