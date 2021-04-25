#ifndef __CLUSTER_STRUCT_H_
#define __CLUSTER_STRUCT_H_

#include "../include/base_type.h"

enum ECLUSTER_NOTIFY
{
    NOTIFY_HEARTLINK_BROKEN,            //心跳链路中断的通知命令码
    NOTIFY_HEARTLINK_RECOVERY,          //心跳恢复的通知命令码
    NOTIFY_HEARTBEAT_BROKEN,            //心跳中断的通知命令码
    NOTIFY_HEARTBEAT_RECOVERY,          //心跳恢复的通知命令码
    NOTIFY_STARTRESOURCE_FAIL,          //启动资源失败
    NOTIFY_START_ASYN_RES_FAIL,         //启动异步资源失败
    NOTIFY_STOPRESOURCE_FAIL,           //停止资源失败
    NOTIFY_RESOURCE_FAIL,               //资源失败的通知命令码
    NOTIFY_RESOURCE_RECOVERY,           //资源恢复的通知命令码
    NOTIFY_ASYN_RES_FAIL,               //异步资源失败
    NOTIFY_HARDDISK_FAIL,               //阵列磁盘失败的通知命令码
    NOTIFY_HARDDISK_RECOVERY,           //阵列磁盘恢复的通知命令码
    NOTIFY_ACTIVE_SUCCESS,              //激活成功通知命令码
    NOTIFY_ACTIVE_FAIL,                 //激活失败通知命令码
    NOTIFY_DEACTIVE_SUCCESS,            //去激活成功通知命令码
    NOTIFY_FAILOVER,                    //发生FailOver切换的通知命令码
    NOTIFY_FAILOVER_SUCCESS,            //FailOver切换成功
    NOTIFY_FAILOVER_FAIL,               //FailOver切换失败
    NOTIFY_FAILOVER_RESP_SUCCESS,       //FailOver响应端操作成功
    NOTIFY_FAILOVER_RESP_FAIL,          //FailOver响应端操作失败
    NOTIFY_AGGRESSIVE_ACTIVE,           //主动激活
    NOTIFY_AGGRESSIVE_ACTIVE_FAIL,      //主动激活失败
    NOTIFY_AGGRESSIVE_ACTIVE_SUCCESS,   //主动激活成功
    NOTIFY_AGGRESSIVE_DEACTIVE,         //主动去激活
    NOTIFY_AGGRESSIVE_DEACTIVE_COMPLETE,//主动去激活完成
    NOTIFY_HANDOVER,                    //发生HandOver切换的通知命令码
    NOTIFY_HANDOVER_SUCCESS,            //HandOver切换成功
    NOTIFY_HANDOVER_FAIL,               //HandOver切换失败
    NOTIFY_PEERNODE_RESTART,            //对端重启
    NOTIFY_LOCALNODE_FAILURE            //本端发生故障，除心跳中断的情况外，不能再激活
};

#define CLUSTER_FRAME_MAXSIZE    (64 * 1024)
#define CLUSTER_FRAME_HEADLENGTH (offsetof(SClusterFrame,Data))
struct SClusterFrame
{
    UINT4 uCheckSum;     //校验域
    UINT4 uSeqNum;       //帧序列号
    INT4  nCmdCode;      //数据桢的命令字
    INT4  nLen;          //从data开始的数据长度
    BYTE  Data[1];       //数据

    void* operator new(size_t classSize, size_t DataLength = 0)
    {
        void* ptr = NULL;
        UINT4 uLen = (DataLength > 0) ? DataLength : 0;

        UINT4 length = uLen + CLUSTER_FRAME_HEADLENGTH;
        if (length > CLUSTER_FRAME_MAXSIZE)
        {
            return NULL;
        }

        ptr = ::new char[length];
        if(ptr)
        {
            memset(ptr, 0, length);
            ((SClusterFrame*)ptr)->nLen = length - CLUSTER_FRAME_HEADLENGTH;
        }
        return ptr;
    };

    //填写校验域
    void FillCheckSum()
    {
        uCheckSum = uSeqNum ^ nCmdCode ^ nLen;
    };

    //验证校验域
    BOOL VerifyCheckSum()
    {
        UINT4 uValue = uSeqNum ^ nCmdCode ^ nLen;

        return uValue == uCheckSum;
    };
};

enum CLS_CMD_TYPE
{

    /*
        双机控制用到的命令码
    */
    #define CMD_CLS_LOGICAL_MINCODE     0
    #define CMD_CLS_LOGICAL_MAXCODE     99

    CMD_CLS_SHAKEHANDS          = 0,	//双机握手帧
    CMD_CLS_HEARTBEAT			= 1,	//双机心跳帧
    CMD_CLS_HANDOVER			= 2,	//HandOver命令码
    CMD_CLS_ANS_HANDOVER		= 3,	//HandOver应答码
    CMD_CLS_FAILOVER			= 4,	//FailOver命令码
    CMD_CLS_ANS_FAILOVER		= 5,	//FailOver应答码
    CMD_CLS_ASKFOR_HANDOVER		= 6,	//手动切换时，请求对端发起HandOver操作的命令码
    CMD_CLS_ASKFOR_ACTIVE		= 7,	//退出前，请求对端发起主动激活操作的命令码

    /*
        双机间异步资源通讯用到的命令码
    */
    #define CMD_CLS_ASYNRES_MINCODE     100
    #define CMD_CLS_ASYNRES_MAXCODE     199

    CMD_CLS_SYNC_DATA           = 100	//同步数据命令码
};

#endif //__CLUSTER_STRUCT_H_
