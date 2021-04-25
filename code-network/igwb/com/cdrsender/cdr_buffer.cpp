/**************************************************************************
 *产品名    ：iGateway Bill V200        
 *模块名    ：GTP'计费接口
 *文件名    : cdr_buffer.cpp
 *描述信息  ：本文件是类CCDRBuffer的定义
 *版本说明  ：V200R002B02   
 *版权信息  ：版权所有（C）2004-2005 华为技术有限公司
 *作者      ：Z30971
 *创建时间  ：2004-06-19
 *修改记录  ：
 **************************************************************************/
#include "cdr_buffer.h"
#include "cdr_producer.h"

//////////////////////////////////////////////////////////////////////////
/**************************************************************************
 *函数原型  : CCDRBuffer::CCDRBuffer(UINT2 nSize)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
CCDRBuffer::CCDRBuffer(UINT2 nSize)
{
    m_nMaxSize = nSize;
    m_pSemaphore = new ACE_Semaphore(m_nMaxSize);
}
/**************************************************************************
 *函数原型  : CCDRBuffer::~CCDRBuffer()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/ 
CCDRBuffer::~CCDRBuffer()
{
    Clear();
    delete m_pSemaphore;
    m_pSemaphore = NULL;
}
/**************************************************************************
 *函数原型  : void  CCDRBuffer::PutPacket(void* pPacket)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/ 
void  CCDRBuffer::PutPacket(void* pPacket)
{
    m_pSemaphore->acquire();
    
    m_Mutex.acquire();
    m_ListPacket.push_back(pPacket);
    m_Mutex.release();
}
/**************************************************************************
 *函数原型  : void* CCDRBuffer::GetPacket()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
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
 *函数原型  : void  CCDRBuffer::RemovePacket()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
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
 *函数原型  : UINT2 CCDRBuffer::Count()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/     
UINT2 CCDRBuffer::Count()
{
    m_Mutex.acquire();
    int nCount = m_ListPacket.size();
    m_Mutex.release();

    return nCount;
}
/**************************************************************************
 *函数原型  : void CCDRBuffer::Clear()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
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


