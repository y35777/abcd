/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:persist_err.h
    描述信息:
    
    包含CPersistErr类的定义,该类用于在整个进程中管理错误消息.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-16,创建.
*/

#ifndef _PERSIST_ERROR_H
#define _PERSIST_ERROR_H

#include "../include/precom.h"

class CPersistErr
{
public:
    CPersistErr(){};
    virtual ~CPersistErr();

    static CPersistErr* Instance();
    static void Destory();
    BOOL Init();
    const char* GetError(INT4 nErrno);

protected:
    static CPersistErr* g_pErr;
    MAP<UINT4,char*> m_ErrMap;
};

#endif
