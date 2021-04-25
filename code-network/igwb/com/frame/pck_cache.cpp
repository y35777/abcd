/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:pck_cache.cpp
    描述信息:
    
    包含CPacketCache类的实现,该类用于在前存盘模块和
    话单处理模块之间交换话单包.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-16,创建.
*/

#include "pck_cache.h"
#include "msg_proc.h"

#define SIZE_PACTKET_BLOCK 4

#define CACHE_HIGH_WATER_MARK   4 * 1024 * 1024
#define CACHE_LOW_WATER_MARK    4 * 1024 * 1024

CPacketCache::CPacketCache(UINT4 uLowLimit, UINT4 uHighLimit)
{
    m_uPacketMemHigh = uHighLimit;
    m_uPacketMemLow = uLowLimit;
    m_uPacketMemUsed = 0;
    m_bValidZone = TRUE;

    m_theCache.high_water_mark(CACHE_HIGH_WATER_MARK);
    m_theCache.low_water_mark(CACHE_LOW_WATER_MARK);
}

/*
    函数功能:
    清空话单包缓冲区中的数据,回收话单包占用的空间.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
CPacketCache::~CPacketCache()
{
    ACE_Message_Block *mb = NULL;
    SPacketCacheEle *pack = NULL;

    while(m_theCache.message_count())
    {
        m_theCache.dequeue_head(mb);

        pack = (SPacketCacheEle*)(*(UINT4 *)(mb->rd_ptr()));

        m_Mutex.acquire();
        m_uPacketMemUsed -= pack->uPacketLen;
        m_Mutex.release();

        delete pack;
        delete mb;
    }

    ASSERT(m_uPacketMemUsed == 0);
}

/*
    函数功能:从话单包缓冲区中头部读取一个话单包.

    1.若话单包有效,直接返回话单包;
    2.若话单包无效,从磁盘文件中恢复出来.
    3.若恢复话单包失败,则写Trace,并返回FALSE.

    参数:
    pPacket,输出话单包中的数据.
    uPacketLen,输出话单包中数据的长度.

    返回:成功取得话单包数据,返回TRUE;失败返回FALSE.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
BOOL CPacketCache::GetPacket(void*& pPacket, UINT4& uPacketLen)
{
    if(m_theCache.message_count() > 0)
    {
        ACE_Message_Block *mb = NULL;
        if(m_theCache.dequeue_head(mb) == -1)
        {
            TRACE(MTS_MANAGER,S_FRAME_CACHE_GET_PACK_FAIL);
            return FALSE;
        }

        SPacketCacheEle* pack = 
            (SPacketCacheEle*)(*(UINT4 *)(mb->rd_ptr()));

        delete mb;
        void* p = pack->pPacket;

        if(p != NULL)
        {
            /* 话单包有效,直接返回 */

            m_Mutex.acquire();
            m_uPacketMemUsed -= pack->uPacketLen;
            m_Mutex.release();
        }
        else if(pack->uPacketLen > 0)
        {
            /* 话单包无效,从磁盘中恢复 */
            
            FILE* f = fopen(pack->szFilePath,"rb");
            if(f == NULL)
            {
                TRACE(MTS_MANAGER,S_FRAME_CACHE_OPEN_FILE_FAIL,
                    pack->szFilePath);
                delete pack;
                
                return FALSE;
            }
            
            if(fseek(f, pack->uStartPos, SEEK_SET) != 0)
            {
                fclose(f);
                TRACE(MTS_MANAGER,S_FRAME_CACHE_SEEK_FILE_FAIL,
                    pack->szFilePath,pack->uStartPos);
                delete pack;
                
                return FALSE;
            }
            
            p = new char[pack->uPacketLen];
            
            if(fread(p,pack->uPacketLen,1/*count*/,f) != 1)
            {
                fclose(f);
                TRACE(MTS_MANAGER,S_FRAME_CACHE_READ_FILE_FAIL,
                    pack->szFilePath);
                delete[] p;
                delete pack;
                
                return FALSE;
            }

            fclose(f);
        }

        pPacket = p;
        uPacketLen = pack->uPacketLen;
        delete pack;

        return TRUE;
    }

    /* 缓冲区中没有话单包,返回FALSE. */

    TRACE(MTS_MANAGER,S_FRAME_CACHE_NO_PACKETS);
    return FALSE;
}

/*
    函数功能:把一个话单包追加到缓冲区的头部.

    1.若缓冲区中有足够空间,直接把话单包追加到缓冲区头部;
    2.若缓冲去中没有足够空间,先释放话单包的数据,再把空包
      追加到缓冲区的头部.
    3.若追加话单包失败,则写Trace,并返回FALSE.

    参数:
    pPacketStatus,话单包的指针,若返回失败,参数中的pPakcet
    有可能被修改.

    返回:成功取得话单包数据,返回TRUE;失败返回FALSE.

    修改记录:
    zhouyonghui,2001-10-16,创建.
*/
BOOL CPacketCache::PutPacket(SPacketCacheEle* pPacketStatus)
{
    ASSERT(pPacketStatus != NULL);
    BOOL bDeleted = FALSE;

    if(m_uPacketMemUsed >= m_uPacketMemHigh)
    {
        delete[] pPacketStatus->pPacket;
        pPacketStatus->pPacket = NULL;
        bDeleted = TRUE;
    }

    ACE_Message_Block *mb = new ACE_Message_Block(SIZE_PACTKET_BLOCK);
    *((UINT4 *)(mb->wr_ptr())) = (UINT4)pPacketStatus;
    mb->wr_ptr(4);

    ACE_Time_Value tv(1);

    if(m_theCache.enqueue_prio(mb,&tv) == -1)
    {
        delete mb;
        TRACE(MTS_MANAGER,S_FRAME_CACHE_PUT_PACK_FAIL);

        return FALSE;
    }

    if(bDeleted != TRUE)
    {
        /*
            有效话单包被追加到缓冲区的头部,
            增加缓冲区中数据长度变量.
        */
        m_Mutex.acquire();
        m_uPacketMemUsed += pPacketStatus->uPacketLen;
        m_Mutex.release();
    }

    return TRUE;
}

