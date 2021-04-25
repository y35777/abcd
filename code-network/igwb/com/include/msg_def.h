#ifndef __MESSAGEDEF_H__
#define __MESSAGEDEF_H__

#include "../include/precom.h"

//网络模块发往前存盘模块的"请求保存原始话单包"消息所带的内容
struct SFrontPacketInfo
{
    UINT4 uPID;             //需保存的原始话单包的包号
    BYTE* pPacket;          //需保存的原始话单包的指针
    UINT4 uPacketLen;       //需保存的原始话单包的长度
};

//话单处理模块发往后存盘模块的"请求保存最终话单包"消息所带的内容
struct SBackPacketInfo
{
    BYTE yChannelID;//通道号
    UINT4 uPID;     //需保存的原始话单包的包号
    BYTE* pPacket;  //需保存的原始话单包的指针
    UINT4 uLen;     //需保存的原始话单包的长度
    UINT4 uBillCsn; //当前处理的话单流水号
};

//后存盘模块发往话单处理模块的"通知状态"消息所带的内容
struct SInformStatus
{
    UINT4 uLastPID; //后存盘模块的各通道中所保存的最后一包最终话单的包号的最小值
    UINT4 uaBillCsns[255];  //各通道最后的话单流水号的数组，下标表示通道号
};

//前后存盘模块的同步数据结构
struct SSaveSyncData
{
    UINT4 uPID;       //当前包号；
    UINT4 uFileCsn;   //当前文件序列号；
    UINT4 uBillCsn;   //后存盘模块的话单序列号
};

#endif
