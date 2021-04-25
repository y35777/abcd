/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:pck_cache.cpp
    ������Ϣ:
    
    ����CPacketCache���ʵ��,����������ǰ����ģ���
    ��������ģ��֮�佻��������.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
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
    ��������:
    ��ջ������������е�����,���ջ�����ռ�õĿռ�.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
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
    ��������:�ӻ�������������ͷ����ȡһ��������.

    1.����������Ч,ֱ�ӷ��ػ�����;
    2.����������Ч,�Ӵ����ļ��лָ�����.
    3.���ָ�������ʧ��,��дTrace,������FALSE.

    ����:
    pPacket,����������е�����.
    uPacketLen,��������������ݵĳ���.

    ����:�ɹ�ȡ�û���������,����TRUE;ʧ�ܷ���FALSE.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
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
            /* ��������Ч,ֱ�ӷ��� */

            m_Mutex.acquire();
            m_uPacketMemUsed -= pack->uPacketLen;
            m_Mutex.release();
        }
        else if(pack->uPacketLen > 0)
        {
            /* ��������Ч,�Ӵ����лָ� */
            
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

    /* ��������û�л�����,����FALSE. */

    TRACE(MTS_MANAGER,S_FRAME_CACHE_NO_PACKETS);
    return FALSE;
}

/*
    ��������:��һ��������׷�ӵ���������ͷ��.

    1.�������������㹻�ռ�,ֱ�Ӱѻ�����׷�ӵ�������ͷ��;
    2.������ȥ��û���㹻�ռ�,���ͷŻ�����������,�ٰѿհ�
      ׷�ӵ���������ͷ��.
    3.��׷�ӻ�����ʧ��,��дTrace,������FALSE.

    ����:
    pPacketStatus,��������ָ��,������ʧ��,�����е�pPakcet
    �п��ܱ��޸�.

    ����:�ɹ�ȡ�û���������,����TRUE;ʧ�ܷ���FALSE.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
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
            ��Ч��������׷�ӵ���������ͷ��,
            ���ӻ����������ݳ��ȱ���.
        */
        m_Mutex.acquire();
        m_uPacketMemUsed += pPacketStatus->uPacketLen;
        m_Mutex.release();
    }

    return TRUE;
}

