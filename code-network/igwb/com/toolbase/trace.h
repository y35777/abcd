/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:trace.h
    ������Ϣ:
    
    ����CTrace��Ķ���,��������������������֧��TRACE��Ϣ.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/

#ifndef _TRACE_H
#define _TRACE_H

#include "../include/precom.h"
//by ldf 2002.6.7 ��Ӧ���ⵥD14393
#include "../include/toolbox.h"
//�޸����

/* ����TRACE�ļ�����󳤶� */
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
    FILE* m_pTraceFile;             //TRACE�ļ�FILEָ��
    char m_pFileName[MAX_PATH];     //TRACE�ļ���ȫ·����
    char m_pTempFileName[MAX_PATH]; //TRACE����ʱ�����ļ���ȫ·����
    UINT4 m_nMaxFileSize;           //TRACE�ļ�����󳤶�

    ACE_Mutex m_lock;               //�̻߳�����

    void TraceBegin();              //дTRACE��Ϣͷ
    void TraceContent(const char *pMsg);  //дTRACE��Ϣ��
    void TraceEnd();                //дTRACE��Ϣβ
};

#endif
