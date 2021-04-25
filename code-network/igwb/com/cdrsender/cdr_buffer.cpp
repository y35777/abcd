/**************************************************************************
 *��Ʒ��    ��iGateway Bill V200        
 *ģ����    ��GTP'�Ʒѽӿ�
 *�ļ���    : cdr_buffer.cpp
 *������Ϣ  �����ļ�����CCDRBuffer�Ķ���
 *�汾˵��  ��V200R002B02   
 *��Ȩ��Ϣ  ����Ȩ���У�C��2004-2005 ��Ϊ�������޹�˾
 *����      ��Z30971
 *����ʱ��  ��2004-06-19
 *�޸ļ�¼  ��
 **************************************************************************/
#include "cdr_buffer.h"
#include "cdr_producer.h"

//////////////////////////////////////////////////////////////////////////
/**************************************************************************
 *����ԭ��  : CCDRBuffer::CCDRBuffer(UINT2 nSize)
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
CCDRBuffer::CCDRBuffer(UINT2 nSize)
{
    m_nMaxSize = nSize;
    m_pSemaphore = new ACE_Semaphore(m_nMaxSize);
}
/**************************************************************************
 *����ԭ��  : CCDRBuffer::~CCDRBuffer()
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/ 
CCDRBuffer::~CCDRBuffer()
{
    Clear();
    delete m_pSemaphore;
    m_pSemaphore = NULL;
}
/**************************************************************************
 *����ԭ��  : void  CCDRBuffer::PutPacket(void* pPacket)
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/ 
void  CCDRBuffer::PutPacket(void* pPacket)
{
    m_pSemaphore->acquire();
    
    m_Mutex.acquire();
    m_ListPacket.push_back(pPacket);
    m_Mutex.release();
}
/**************************************************************************
 *����ԭ��  : void* CCDRBuffer::GetPacket()
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/ 
void* CCDRBuffer::GetPacket()
{
    void* pPacket = NULL;
    
    m_Mutex.acquire();
    if (!m_ListPacket.empty())
    {
        pPacket = m_ListPacket.front();
        m_ListPacket.pop_front();
        m_pSemaphore->release();
    }
    else
    {
        pPacket = NULL;
    }
    m_Mutex.release();

    return pPacket;
}
/**************************************************************************
 *����ԭ��  : void  CCDRBuffer::RemovePacket()
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/ 
void  CCDRBuffer::RemovePacket()
{
    m_Mutex.acquire();
    
    if (!m_ListPacket.empty())
    {
        m_ListPacket.pop_front();
        m_pSemaphore->release();
    }    
    m_Mutex.release();
}
/**************************************************************************
 *����ԭ��  : UINT2 CCDRBuffer::Count()
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/     
UINT2 CCDRBuffer::Count()
{
    m_Mutex.acquire();
    int nCount = m_ListPacket.size();
    m_Mutex.release();

    return nCount;
}
/**************************************************************************
 *����ԭ��  : void CCDRBuffer::Clear()
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/   
void CCDRBuffer::Clear()
{
    m_Mutex.acquire();
    
    PACKETLIST::iterator it = m_ListPacket.begin();
    while (it != m_ListPacket.end())
    {
        TBillPacket* pPacket = (TBillPacket*)(*it);
        delete[] pPacket->m_pData;

        it++;
    }

    m_ListPacket.clear();
    m_Mutex.release();
}


