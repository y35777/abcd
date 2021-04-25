/**************************************************************************
 *产品名    ：iGateway Bill V200        
 *模块名    ：GTP'计费接口
 *文件名    : cdr_buffer.h
 *描述信息  ：本文件是类CCDRBuffer的定义
 *版本说明  ：V200R002B02   
 *版权信息  ：版权所有（C）2004-2005 华为技术有限公司
 *作者      ：Z30971
 *创建时间  ：2004-06-19
 *修改记录  ：
 **************************************************************************/
#ifndef __GTP_BUFFER_H__
#define __GTP_BUFFER_H__

#include "../include/base_type.h"
#include <ace/Synch.h>
#include <deque>

//////////////////////////////////////////////////////////////////////////

class CCDRBuffer
{
public:
    CCDRBuffer(UINT2 nSize = 20);
    virtual ~CCDRBuffer();
public:
    void  PutPacket(void* pPacket);
    void* GetPacket();
    void  RemovePacket();
    UINT2 Count();

protected:
    void Clear();

protected:
    typedef std::deque<void* >  PACKETLIST;
    
    PACKETLIST        m_ListPacket;
    UINT2             m_nMaxSize;

    ACE_Semaphore*    m_pSemaphore; 
    ACE_Mutex         m_Mutex;
};
#endif //__GTP_BUFFER_H__