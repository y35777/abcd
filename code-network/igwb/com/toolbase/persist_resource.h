/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:persist_resource.h
    描述信息:
    
    包含CPersistRESOURCE类的定义,该类用于在整个进程中管理错误消息.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    李杜芳,2002-05-30,创建.
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
