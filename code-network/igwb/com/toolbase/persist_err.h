/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:persist_err.h
    ������Ϣ:
    
    ����CPersistErr��Ķ���,�������������������й��������Ϣ.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
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
