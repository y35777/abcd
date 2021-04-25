/**************************************************************************
 *��Ʒ��    ��iGateway Bill V200        
 *ģ����    ��GTP'�Ʒѽӿ�
 *�ļ���    : cdr_buffer.h
 *������Ϣ  �����ļ�����CCDRBuffer�Ķ���
 *�汾˵��  ��V200R002B02   
 *��Ȩ��Ϣ  ����Ȩ���У�C��2004-2005 ��Ϊ�������޹�˾
 *����      ��Z30971
 *����ʱ��  ��2004-06-19
 *�޸ļ�¼  ��
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