#ifndef __CLUSTER_ALARM_H__
#define __CLUSTER_ALARM_H__

#include "cluster_struct.h"

class CClusterAlarm
{
public:
    CClusterAlarm();
    virtual ~CClusterAlarm();

    //双机事件信息处理函数
    virtual void OnNotify(ECLUSTER_NOTIFY uMsg,
                          UINT4 uParam1 = 0,
                          UINT4 uParam2 = 0);
    //秒事件处理函数
    virtual void Timer1Sec();
};

#endif //__CLUSTER_ALARM_H__
