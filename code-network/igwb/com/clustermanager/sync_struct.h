#ifndef __SYNC_STRUCT_H__
#define __SYNC_STRUCT_H__

struct SSyncFrame
{
    UINT4   uLen;           //数据帧长度
    BYTE    Data[1];        //同步数据
};
#define SYNC_FRAME_HEAD_LENGTH  offsetof(SSyncFrame, Data)

struct SMscAPSyncFrame      //一个接入点下的同步数据
{
    UINT4   uAPointID;      //接入点号
    UINT4   uDiffTime;      //时间差
    UINT4   uDiffPackId;    //包号差
    UINT4   uLen;           //数据帧长度
    BYTE    Data[1];        //同步数据
};
#define  MSC_AP_SYNC_FRAME_HEAD_LENGTH  offsetof(SMscAPSyncFrame, Data)

struct SMscSyncData
{
    UINT4   uMID;           //产生同步数据的模块号
    UINT4   uChannel;       //通道号，标识通道，用于后存盘模块；
    UINT4   uLength;        //同步数据长度
    BYTE    Data[1];        //同步数据
};
#define  MSC_SYNC_DATA_HEAD_LENGTH  offsetof(SMscSyncData, Data)

struct SMscNetSyncData      //网络模块状态同步数据
{
    UINT4   uPackId;            //网络模块的包号
    BOOL    bWaitForResponse;   //是否等待前存盘应答已提交的帧数据
    BYTE    Data[1];            //网络模块状态文件中的其它信息，包括滑动窗口流水号
};

#endif //__SYNC_STRUCT_H__
