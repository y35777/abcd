/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:pck_cache.h
    描述信息:
    
    包含CPacketCache类的定义,该类用于在前存盘模块和
    话单处理模块之间交换话单包.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-16,创建.
*/

#ifndef _PACKET_CATCHE_H
#define _PACKET_CATCHE_H

#include "../include/precom.h"

/* 话单包缓冲区中的包结构 */
struct SPacketCacheEle
{
    UINT4 uPacketID;                //包号
    UINT4 uPacketLen;               //话单包的长度
    void* pPacket;                  //指向话单包的数据
    char szFilePath[MAX_PATH];      //话单包所在文件的绝对路径
    UINT4 uStartPos;                //话单包在文件中的起始位置
};

#define PACKET_CACHE_ELE_LENGTH sizeof(SPacketCacheEle)

class CPacketCache
{
public:
    CPacketCache(UINT4 uLowLimit, UINT4 uHighLimit);
    virtual ~CPacketCache();

    BOOL PutPacket(SPacketCacheEle* pPacketStatus);
    BOOL GetPacket(void*& pPacket, UINT4& uPacketLen);

protected:

    /* 存放话单包的缓冲区 */
    ACE_Message_Queue<ACE_MT_SYNCH> m_theCache;

    UINT4 m_uPacketMemHigh;         //缓冲区中话单包内存占用的上限

    UINT4 m_uPacketMemLow;          //缓冲区中话单包内存占用的下限
                                    /* 该成员变量目前不使用. */

    UINT4 m_uPacketMemUsed;         //缓冲区中话单包内存占用的实际值

    ACE_Mutex m_Mutex;              //保护缓冲区的互斥锁
    BOOL m_bValidZone;              //是否正处于有效区间
                                    //若缓冲区中所有包均有效,则为TRUE
                                    /* 该成员变量目前不使用. */
};

#endif
