#include "../include/frame.h"
#include "cluster_alarm.h"
#include "heartbeat_link.h"
#include "res_object.h"
#include "resource.h"


/********************************************************
* 名称: CClusterAlarm::CClusterAlarm
* 功能: 构造函数
* 输入:
* 输出:
* 返回:
********************************************************/
CClusterAlarm::CClusterAlarm()
{
    //
}

/********************************************************
* 名称: CClusterAlarm::~CClusterAlarm
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CClusterAlarm::~CClusterAlarm()
{
    //
}

/********************************************************
* 名称: CClusterAlarm::OnNotify
* 功能: 双机事件信息处理函数
* 输入:
*       ECLUSTER_NOTIFY uMsg : 事件信息码
*       UINT4 uParam1        : 参数一，可能是故障对象的指针
*       UINT4 uParam2        : 参数二，保留参数
* 输出:
* 返回:
********************************************************/
void CClusterAlarm::OnNotify(ECLUSTER_NOTIFY uMsg,
                             UINT4 uParam1, UINT4 uParam2)
{
    switch (uMsg)
    {
        case NOTIFY_HEARTLINK_BROKEN:
        //心跳链路中断的通知命令码
        {
            CHeartBeatLink* pLink =  (CHeartBeatLink*) uParam1;
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_HEART_BEAT_LINK_BROKEN,
                   pLink->GetLinkIndex(),
                   pLink->GetLinkName(),
                   pLink->GetLinkType());
            TRACE(MTS_CLSTR,
                   S_HEART_BEAT_LINK_BROKEN,
                   pLink->GetLinkIndex(),
                   pLink->GetLinkName(),
                   pLink->GetLinkType());
            break;
        }
        case NOTIFY_HEARTLINK_RECOVERY:
        //心跳链路恢复的通知命令码
        {
            CHeartBeatLink* pLink =  (CHeartBeatLink*) uParam1;
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_HEART_BEAT_LINK_RECOVERY,
                   pLink->GetLinkIndex(),
                   pLink->GetLinkName(),
                   pLink->GetLinkType());
            TRACE(MTS_CLSTR,
                   S_HEART_BEAT_LINK_RECOVERY,
                   pLink->GetLinkIndex(),
                   pLink->GetLinkName(),
                   pLink->GetLinkType());
            break;
        }
        case NOTIFY_HEARTBEAT_BROKEN:
        //心跳中断的通知命令码
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_HEART_BEAT_BROKEN);
            TRACE(MTS_CLSTR, S_HEART_BEAT_BROKEN);
            break;
        }
        case NOTIFY_HEARTBEAT_RECOVERY:
        //心跳恢复的通知命令码
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_HEART_BEAT_RECOVERY);
            TRACE(MTS_CLSTR, S_HEART_BEAT_RECOVERY);
            break;
        }
        case NOTIFY_STARTRESOURCE_FAIL:
        //启动资源失败
        {
            CResObject* pResObject =  (CResObject*) uParam1;
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_START_RES_FAIL, pResObject->GetResName(),
                   pResObject->GetResType());
            TRACE(MTS_CLSTR,
                  S_START_RES_FAIL, pResObject->GetResName(),
                  pResObject->GetResType());

            break;
        }
        case NOTIFY_START_ASYN_RES_FAIL:
        //启动异步资源失败
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT, S_START_ASYNC_RES_FAIL);
            TRACE(MTS_CLSTR, S_START_ASYNC_RES_FAIL);
            break;
        }
        case NOTIFY_STOPRESOURCE_FAIL:
        //停止资源失败
        {
            CResObject* pResObject =  (CResObject*) uParam1;
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_STOP_RES_FAIL, pResObject->GetResName(),
                   pResObject->GetResType());
            TRACE(MTS_CLSTR,
                  S_STOP_RES_FAIL, pResObject->GetResName(),
                  pResObject->GetResType());
            //通过TRACE及MSGOUT等方式输出失败资源的名称;
            break;
        }
        case NOTIFY_RESOURCE_FAIL:
        //资源失败的通知命令码
        {
            CResObject* pResObject =  (CResObject*) uParam1;
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_RESOURCE_FAIL, pResObject->GetResName(),
                   pResObject->GetResType());
            TRACE(MTS_CLSTR,
                  S_RESOURCE_FAIL, pResObject->GetResName(),
                  pResObject->GetResType());
            break;
        }
        case NOTIFY_RESOURCE_RECOVERY:
        //资源恢复的通知命令码
        {
            CResObject* pResObject =  (CResObject*) uParam1;
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_RESOURCE_RECOVERY, pResObject->GetResName(),
                   pResObject->GetResType());
            TRACE(MTS_CLSTR,
                  S_RESOURCE_RECOVERY, pResObject->GetResName(),
                  pResObject->GetResType());
            break;
        }
        case NOTIFY_ASYN_RES_FAIL:
        //异步资源失败的通知命令码
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_AYSN_RESOURCE_FAIL);
            TRACE(MTS_CLSTR, S_AYSN_RESOURCE_FAIL);
            break;
        }
        case NOTIFY_HARDDISK_FAIL:
        //阵列硬盘故障
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_HARDDISK_FAIL, uParam1);
            TRACE(MTS_CLSTR, S_HARDDISK_FAIL, uParam1);
            break;
        }
        case NOTIFY_HARDDISK_RECOVERY:
        //阵列硬盘故障恢复
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_HARDDISK_RECOVERY);
            TRACE(MTS_CLSTR, S_HARDDISK_RECOVERY);
            break;
        }
        case NOTIFY_ACTIVE_SUCCESS:
        //激活成功通知命令码
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, S_ACTIVE_SUCCESS);
            TRACE(MTS_CLSTR, S_ACTIVE_SUCCESS);
            break;
        }
        case NOTIFY_ACTIVE_FAIL:
        //激活失败通知命令码
        {
            TRACE(MTS_CLSTR, S_ACTIVE_FAILED);
            break;
        }
        case NOTIFY_DEACTIVE_SUCCESS:
        //去激活成功通知命令码
        {
            TRACE(MTS_CLSTR, S_DEACTIVE_SUCCESS);
            break;
        }
        case NOTIFY_FAILOVER:
        //发生FailOver切换的通知命令码
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT, S_BEGIN_FAIL_OVER, uParam1);
            TRACE(MTS_CLSTR, S_BEGIN_FAIL_OVER, uParam1);
            break;
        }
        case NOTIFY_FAILOVER_SUCCESS:
        //FailOver切换成功
        {
            TRACE(MTS_CLSTR, S_FAIL_OVER_SUCCESS);
            break;
        }
        case NOTIFY_FAILOVER_FAIL:
        //FailOver切换失败
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT, S_FAIL_OVER_FAILED);
            TRACE(MTS_CLSTR, S_FAIL_OVER_FAILED);
            break;
        }
        case NOTIFY_FAILOVER_RESP_SUCCESS:
        //FailOver响应端操作成功
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT, S_FAIL_OVER_RESP_SUCCESS);
            TRACE(MTS_CLSTR, S_FAIL_OVER_RESP_SUCCESS);
            break;
        }
        case NOTIFY_FAILOVER_RESP_FAIL:
        //FailOver响应端操作失败
        {
            TRACE(MTS_CLSTR, S_FAIL_OVER_RESP_FAILED);
            break;
        }
        case NOTIFY_AGGRESSIVE_ACTIVE:
        //主动激活
        {
            TRACE(MTS_CLSTR, S_AGGRESSIVE_ACTIVE);
            break;
        }
        case NOTIFY_AGGRESSIVE_ACTIVE_SUCCESS:
        //主动激活成功
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT, S_AGGRESSIVE_ACTIVE_SUCCESS);
            TRACE(MTS_CLSTR, S_AGGRESSIVE_ACTIVE_SUCCESS);
            //通过TRACE及MSGOUT等方式输出主动激活操作成功的消息;
            break;
        }
        case NOTIFY_AGGRESSIVE_ACTIVE_FAIL:
        //主动激活失败
        {
            TRACE(MTS_CLSTR, S_AGGRESSIVE_ACTIVE_FAIL);
            break;
        }
        case NOTIFY_AGGRESSIVE_DEACTIVE:
        //主动去激活
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT, S_AGGRESSIVE_DEACTIVE);
            TRACE(MTS_CLSTR, S_AGGRESSIVE_DEACTIVE);
            break;
        }
        case NOTIFY_AGGRESSIVE_DEACTIVE_COMPLETE:
        //主动去激活完成
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT, S_AGGRESSIVE_DEACTIVE_COMPLETE);
            TRACE(MTS_CLSTR, S_AGGRESSIVE_DEACTIVE_COMPLETE);
            break;
        }
        case NOTIFY_HANDOVER:
        //发生HandOver切换的通知命令码
        {
            TRACE(MTS_CLSTR, S_BEGIN_HANDOVER);
            break;
        }
        case NOTIFY_HANDOVER_SUCCESS:
        //HandOver切换成功
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT, S_HANDOVER_SUCCESS);
            TRACE(MTS_CLSTR, S_HANDOVER_SUCCESS);
            break;
        }
        case NOTIFY_HANDOVER_FAIL:
        //HandOver切换失败
        {
            TRACE(MTS_CLSTR, S_HANDOVER_FAIL);
            break;
        }
        case NOTIFY_PEERNODE_RESTART:
        //对端重启
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, S_PEERNODE_RESTART);
            TRACE(MTS_CLSTR, S_PEERNODE_RESTART);
            break;
        }
        case NOTIFY_LOCALNODE_FAILURE:
        //本端发生故障，除心跳中断的情况外，不能再激活
        {
            TRACE(MTS_CLSTR, S_LOCALNODE_FAILURE);
            break;
        }
        default:
            break;
    }
}

/********************************************************
* 名称: CClusterAlarm::Timer1Sec
* 功能: 秒定时器
* 输入:
* 输出:
* 返回: void
********************************************************/
void CClusterAlarm::Timer1Sec()
{
    //
}
