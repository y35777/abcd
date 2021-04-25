/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:persist_resource.h
    ������Ϣ:
    
    ����CPersistRESOURCE��Ķ���,�������������������й��������Ϣ.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ŷ�,2002-05-30,����.
*/

#ifndef _PERSIST_RESOURCE_H
#define _PERSIST_RESOURCE_H

#include "../include/precom.h"

class CPersistResource
{
public:
    CPersistResource(){};
    virtual ~CPersistResource();

    static CPersistResource* Instance();
    static void Destory();
    BOOL Init();
    const char* GetStringFromID(INT4 nID);

protected:
    MAP<INT4,STRING> ResourceString;
    static CPersistResource* g_pResource;
    ACE_Mutex                m_Mutex;
};

#endif
