/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:pck_cache.h
    ������Ϣ:
    
    ����CPacketCache��Ķ���,����������ǰ����ģ���
    ��������ģ��֮�佻��������.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/

#ifndef _PACKET_CATCHE_H
#define _PACKET_CATCHE_H

#include "../include/precom.h"

/* �������������еİ��ṹ */
struct SPacketCacheEle
{
    UINT4 uPacketID;                //����
    UINT4 uPacketLen;               //�������ĳ���
    void* pPacket;                  //ָ�򻰵���������
    char szFilePath[MAX_PATH];      //�����������ļ��ľ���·��
    UINT4 uStartPos;                //���������ļ��е���ʼλ��
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

    /* ��Ż������Ļ����� */
    ACE_Message_Queue<ACE_MT_SYNCH> m_theCache;

    UINT4 m_uPacketMemHigh;         //�������л������ڴ�ռ�õ�����

    UINT4 m_uPacketMemLow;          //�������л������ڴ�ռ�õ�����
                                    /* �ó�Ա����Ŀǰ��ʹ��. */

    UINT4 m_uPacketMemUsed;         //�������л������ڴ�ռ�õ�ʵ��ֵ

    ACE_Mutex m_Mutex;              //�����������Ļ�����
    BOOL m_bValidZone;              //�Ƿ���������Ч����
                                    //�������������а�����Ч,��ΪTRUE
                                    /* �ó�Ա����Ŀǰ��ʹ��. */
};

#endif
