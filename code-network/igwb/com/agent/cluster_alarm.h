/**********************************************************************
 *产品名：   iGateway Bill V200
 *模块名：   agent
 *文件名：   cluster_alarm.h
 *描述信息： 本文件是类CClusterAlarm的定义文件。
             CClusterAlarm类封装了检测iGateway Bill V200中所有与SC紧密
             相关的故障以及一些资源管理上的故障。
 *版本说明： V200R002i00M00B00D1130A
 *版权信息： 版权所有（C）2001-2002 华为技术有限公司
 *作者：     邱意平
 *创建时间： 2001-10-30
 *修改记录：
 **********************************************************************/
#ifndef _CLUSTER_ALARM_H_
#define _CLUSTER_ALARM_H_

#include "agent_hdr.h"
#include "../include/frame.h"
#include "cluster.h"
#include "cluster2.h"
#include "cluster3.h"

class CCluster;
class CCluster2;
class CCluster3;

class CClusterAlarm
{
public:
    CClusterAlarm(CCluster *ptheCluster);
    ~CClusterAlarm();

    void Alarm(void);                //告警检测

private:
    PInnerAlarm GetAlarmItem(UINT2 uAlarmID, BYTE*  yAlarmPara);
    void SetAlarmItem(PInnerAlarm pAlarmItem, BOOL bStatus);
    
public:
    void AlarmPrvNet(void);          //检测私网告警
    void AlarmSwitch(void);          //检测倒换告警
    void AlarmSwitchRestore(void);   //检测倒换恢复告警
    void AlarmVolume(UINT2 uFlag);   //检测卷失效告警
    void AlarmResource(void);        //检测资源失效告警
    void AlarmIPs(void);             //检测IP资源失效告警
    void AlarmDisks(void);           //检测硬盘故障告警

private:
    LIST<SInnerAlarm*>*      m_pAlarmList;    //各个告警项列表
    CCluster*                m_ptheCluster;   //SC管理对象
    BOOL                     m_bAlarmRestore; //倒换恢复标志
    STRING                   m_sResourceType; //服务资源类型(FTAM/FTP)
    VM_TYPE                  m_uVMType;       //卷管理软件类型
    STRING                   m_sFrontVolume;  //前存盘逻辑卷名
    STRING                   m_sFrontDG;      //前存盘所在的磁盘组名
    STRING                   m_sBackVolume;   //后存盘逻辑卷名
    STRING                   m_sBackDG;       //后存盘所在的磁盘组名
};

#endif   //_CLUSTER_ALARM_H_
