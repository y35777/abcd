/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:trace.h
    描述信息:
    
    包含CTrace类的定义,该类用于在整个进程中支持TRACE信息.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-16,创建.
*/

#ifndef _TRACE_H
#define _TRACE_H

#include "../include/precom.h"
//by ldf 2002.6.7 对应问题单D14393
#include "../include/toolbox.h"
//修改完毕

/* 定义TRACE文件的最大长度 */
#define MAX_TRACE_FILE_SIZE     1024 * 1024 * 6

class CTrace
{
public:
    CTrace();
    virtual ~CTrace();

    void Open(const char* szFileName);
    void Close();
    void Trace(const char *szBuf);
    void TraceAssert(const BOOL expression,
                     const char* szSourceFile, 
                     const int nLineNum);

#ifdef  _PLATFORM_WIN32
    void TraceNTException(LPEXCEPTION_POINTERS pException);
#endif

protected:
    FILE* m_pTraceFile;             //TRACE文件FILE指针
    char m_pFileName[MAX_PATH];     //TRACE文件的全路径名
    char m_pTempFileName[MAX_PATH]; //TRACE超长时备份文件的全路径名
    UINT4 m_nMaxFileSize;           //TRACE文件的最大长度

    ACE_Mutex m_lock;               //线程互斥锁

    void TraceBegin();              //写TRACE信息头
    void TraceContent(const char *pMsg);  //写TRACE信息体
    void TraceEnd();                //写TRACE信息尾
};

#endif
