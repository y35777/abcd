#include "../include/frame.h"
#include "heartbeat_link.h"
#include "res_object.h"
#include "igwb_alarm.h"


/********************************************************
* 名称: CIGWBAlarm::CIGWBAlarm
* 功能: 构造函数
* 输入:
* 输出:
* 返回:
********************************************************/
CIGWBAlarm::CIGWBAlarm()
{
    //
    m_bSwitchFault = FALSE;
}

/********************************************************
* 名称: CIGWBAlarm::~CIGWBAlarm
* 功能:
* 输入:
* 输出:
* 返回:
********************************************************/
CIGWBAlarm::~CIGWBAlarm()
{
    //
}

/********************************************************
* 名称: CIGWBAlarm::OnNotify
* 功能: 双机事件信息处理函数
* 输入:
*       ECLUSTER_NOTIFY uMsg : 事件信息码
*       UINT4 uParam1        : 参数一，可能是故障对象的指针
*       UINT4 uParam2        : 参数二，保留参数
* 输出:
* 返回:
********************************************************/
void CIGWBAlarm::OnNotify(ECLUSTER_NOTIFY uMsg,
                          UINT4 uParam1, UINT4 uParam2)
{
    CClusterAlarm::OnNotify(uMsg, uParam1, uParam2);

    SInnerAlarm Alarm;
    switch (uMsg)
    {
        case NOTIFY_HEARTLINK_BROKEN:
        //心跳链路中断的通知命令码。
        {
            CHeartBeatLink* pLink = (CHeartBeatLink*)uParam1;
            Alarm.yAlarmType      = AT_FAULT;
            Alarm.uAlarmID        = ALARM_ID_PRVNET;
			memset(Alarm.yAlarmPara, 0xff, MAX_ALARM_PARA);
            Alarm.yAlarmPara[0]   = pLink->GetLinkIndex();              
			
            CMsgProcessor::SendAlarm(&Alarm);
            AddHisAlarm(Alarm);
            break;
        }
        case NOTIFY_HEARTLINK_RECOVERY:
        //心跳恢复的通知命令码。
        {
            CHeartBeatLink* pLink = (CHeartBeatLink*)uParam1;
            Alarm.yAlarmType      = AT_RESTORE;
            Alarm.uAlarmID        = ALARM_ID_PRVNET;
			memset(Alarm.yAlarmPara, 0xff, MAX_ALARM_PARA);
            Alarm.yAlarmPara[0]   = pLink->GetLinkIndex();         
			
            CMsgProcessor::SendAlarm(&Alarm);
            Alarm.yAlarmType      = AT_FAULT;
            RmvHisAlarm(Alarm);
            break;
        }
        case NOTIFY_HEARTBEAT_BROKEN:
        //心跳中断的通知命令码。
        {
            Alarm.yAlarmType = AT_FAULT;
            Alarm.uAlarmID   = ALARM_ID_HEARTBEAT;
			memset(Alarm.yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);           
			
            CMsgProcessor::SendAlarm(&Alarm);
            AddHisAlarm(Alarm);
            break;
        }
        case NOTIFY_HEARTBEAT_RECOVERY:
        //心跳恢复的通知命令码。
        {
            Alarm.yAlarmType = AT_RESTORE;
            Alarm.uAlarmID   = ALARM_ID_HEARTBEAT;
			memset(Alarm.yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);           
			
            CMsgProcessor::SendAlarm(&Alarm);
            Alarm.yAlarmType = AT_FAULT;
            RmvHisAlarm(Alarm);
            break;
        }
        case NOTIFY_RESOURCE_FAIL:
        //资源失败的通知命令码。
        {
            CResObject* pResObject = (CResObject*)uParam1;
            Alarm.yAlarmType       = AT_FAULT;
            Alarm.uAlarmID         = pResObject->GetAlarmID();
			memset(Alarm.yAlarmPara, 0xff, MAX_ALARM_PARA);
            Alarm.yAlarmPara[0]    = pResObject->GetAlarmPara();           
			
            CMsgProcessor::SendAlarm(&Alarm);
            break;
        }
        case NOTIFY_RESOURCE_RECOVERY:
        //资源恢复的通知命令码。
        {
            CResObject* pResObject = (CResObject*)uParam1;
            Alarm.yAlarmType       = AT_RESTORE;
            Alarm.uAlarmID         = pResObject->GetAlarmID();
			memset(Alarm.yAlarmPara, 0xff, MAX_ALARM_PARA);
            Alarm.yAlarmPara[0]    = pResObject->GetAlarmPara();           

            CMsgProcessor::SendAlarm(&Alarm);
            break;
        }
        case NOTIFY_HARDDISK_FAIL:
        //阵列硬盘故障
    {
            Alarm.yAlarmType     = AT_FAULT;
            Alarm.uAlarmID       = ALARM_ID_DISK;
			memset(Alarm.yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);               
			
            CMsgProcessor::SendAlarm(&Alarm);
            break;
        }
        case NOTIFY_HARDDISK_RECOVERY:
        //阵列硬盘故障恢复
        {
            Alarm.yAlarmType     = AT_RESTORE;
            Alarm.uAlarmID       = ALARM_ID_DISK;
			memset(Alarm.yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);              
			
            CMsgProcessor::SendAlarm(&Alarm);
            break;
        }
        case NOTIFY_FAILOVER_RESP_SUCCESS:
        //FailOver响应端操作成功
        {
            if(!m_bSwitchFault)
            {
                Alarm.yAlarmType     = AT_FAULT;
                Alarm.uAlarmID       = ALARM_ID_FAILOVER;
			    memset(Alarm.yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);
			    
                CMsgProcessor::SendAlarm(&Alarm);

                m_bSwitchFault = TRUE;
            }
            break;
        }
        case NOTIFY_PEERNODE_RESTART:
        //对端重启
        {
            if(m_bSwitchFault)
            {
                Alarm.yAlarmType     = AT_RESTORE;
                Alarm.uAlarmID       = ALARM_ID_FAILOVER;
			    memset(Alarm.yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);
			    
                CMsgProcessor::SendAlarm(&Alarm);

                m_bSwitchFault = FALSE;
            }
            break;
        }
        default:
            break;
    }
}

/********************************************************
* 名称: CIGWBAlarm::Timer1Sec
* 功能: 秒定时器
* 输入:
* 输出:
* 返回: void
********************************************************/
void CIGWBAlarm::Timer1Sec()
{
    CClusterAlarm::Timer1Sec();

    static int nSendHisAlarmInv = 0;
    if(++nSendHisAlarmInv >= 10)
    {
        LIST<SInnerAlarm>::iterator i;
        for (i = m_HisAlarmList.begin(); i != m_HisAlarmList.end(); i++)
        {
            CMsgProcessor::SendAlarm(&(*i));
#ifdef _ONLY_FOR_TEST
            TRACE(MTS_CLSTR, 
                  "SendAlarm(ID=%d, TYPE=%d, PARA=%d)",
                  i->uAlarmID,
                  i->yAlarmType,
                  i->yAlarmPara);
#endif _ONLY_FOR_TEST
        }
        nSendHisAlarmInv = 0;
    }
}

/********************************************************
* 名称: CIGWBAlarm::AddHisAlarm
* 功能: 增加历史告警帧
* 输入: alarm : 历史告警帧结构的引用
* 输出: 
* 返回: void 
********************************************************/
void CIGWBAlarm::AddHisAlarm(SInnerAlarm& alarm)
{
    LIST<SInnerAlarm>::iterator i;
    for (i = m_HisAlarmList.begin(); i != m_HisAlarmList.end(); i++)
    {
        if((alarm.uAlarmID == i->uAlarmID)
           && (alarm.yAlarmType == i->yAlarmType)
           && (memcmp(alarm.yAlarmPara, i->yAlarmPara, MAX_ALARM_PARA) == 0))
        {
            return;
        }
    }
    m_HisAlarmList.push_back(alarm);
}

/********************************************************
* 名称: CIGWBAlarm::RmvHisAlarm
* 功能: 删除历史告警帧
* 输入: alarm : 历史告警帧结构的引用
* 输出: 
* 返回: void 
********************************************************/
void CIGWBAlarm::RmvHisAlarm(SInnerAlarm& alarm)
{
    LIST<SInnerAlarm>::iterator i;
    for (i = m_HisAlarmList.begin(); i != m_HisAlarmList.end(); i++)
    {
        if((alarm.uAlarmID == i->uAlarmID)
           && (alarm.yAlarmType == i->yAlarmType)
           && (memcmp(alarm.yAlarmPara, i->yAlarmPara, MAX_ALARM_PARA) == 0))
        {
            m_HisAlarmList.erase(i);
            return;
        }
    }
}
