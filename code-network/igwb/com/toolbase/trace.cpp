/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:trace.cpp
    ������Ϣ:
    
    ����CTrace���ʵ��,��������������������֧��TRACE��Ϣ.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/
#include "trace.h"

/*
    ÿ��дTRACE�Ĵ�������_FLUSH_TRACE_TIMES_,��ˢ��һ�δ����ļ�.
*/
#define _FLUSH_TRACE_TIMES_     5

#ifdef _PLATFORM_WIN32
#define S_LN_CR "\r\n"
#else
#define S_LN_CR "\n"
#endif

CTrace::CTrace()
{
    m_pTraceFile       = NULL;
    m_nMaxFileSize     = MAX_TRACE_FILE_SIZE;

	m_pFileName[0]     = 0;
	m_pTempFileName[0] = 0;
}

/*
    ��������:��������,�ر�Trace�ļ�,�ͷŻ�����.

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/
CTrace::~CTrace()
{
    Close();
    m_lock.release();
}

void CTrace::Close()
{
    if (m_pTraceFile != NULL)
    {
        fclose(m_pTraceFile);
        m_pTraceFile = NULL;
    }
}

/*
    ��������:��������,�ر�Trace�ļ�,�ͷŻ�����.

    1.��׷�ӵķ�ʽ��Trace�ļ�.
    2.��Trace�ļ����ĺ�׺����Ϊtmp,����Ϊ����Trace�ļ���.

    ����:
    szFileName,Trace�ļ���.

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/
void CTrace::Open(const char* szFileName)
{
    strncpy(m_pFileName, szFileName,MAX_PATH);
	m_pFileName[MAX_PATH  -1] = 0;

    m_pTraceFile = fopen(szFileName, "a");
    if (m_pTraceFile != NULL)
    {
        TraceContent(S_LN_CR);
    }

    char *pPos = strrchr(m_pFileName, '.');
    char *pPosPathDiv = strrchr(m_pFileName,
                ACE_DIRECTORY_SEPARATOR_CHAR);
    if(pPos < pPosPathDiv || pPos == NULL)
    {
        pPos = m_pFileName + strlen(m_pFileName);
    }
    strncpy(m_pTempFileName, m_pFileName, pPos - m_pFileName);
    //�޸�D11623---Trace��ʱ�ļ�����һ���ַ�
    m_pTempFileName[pPos - m_pFileName ] = '\0';
    //�޸����
    strcat(m_pTempFileName, ".tmp");
}

/*
    ��������:дһ��TRACE��¼��ʱ�䲿��.

    1.��дTrace�ļ�֮ǰ���Ȼ�û�����.
    2.�ѵ�ǰʱ����"Sat Jul 21 13:05:06 2001"��ʽд���ļ�.

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/
void CTrace::TraceBegin()
{
    m_lock.acquire();

    if(m_pTraceFile == NULL)
	{
		m_lock.release();
		return;
	}
    
    char pBuf[40];
    time_t sysTime;
    time(&sysTime);
    strcpy(pBuf, ctime(&sysTime));

    if(pBuf[strlen(pBuf) - 1] == '\n')
        pBuf[strlen(pBuf) - 1] = ' ';

    TraceContent(pBuf);
}

void CTrace::TraceContent(const char *pMsg)
{
    if(m_pTraceFile == NULL) return;
    
    fputs(pMsg, m_pTraceFile);
}

/*
    ��������:дһ��TRACE��¼��ʱ�䲿��.

    1.��Trace�ļ�������ر�֮,���������ļ�.
    2.����Trace�ļ���һ������.
    3.дTrace�ļ����,�ͷŻ�����.

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/
void CTrace::TraceEnd()
{
    static int nCount = 0;

    if(m_pTraceFile == NULL)
	{
		m_lock.release();
		return;
	}
    
	unsigned int nLen = ftell(m_pTraceFile);
	if(nLen == 0xFFFFFFFF)
	{
		m_lock.release();
		return;
	}

    if(nLen >= m_nMaxFileSize)
    {
         Close();
         remove(m_pTempFileName);
         rename(m_pFileName, m_pTempFileName);

         Open(m_pFileName);
         nCount = 0;
    }

    fflush(m_pTraceFile);

    m_lock.release();
}

/*
    ��������:дһ��TRACE��¼.

    1.��Trace�ļ�δ�򿪾Ͳ����κβ���.

    ����:
    pFmt,Trace��¼�ַ���ָ��.

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/
void CTrace::Trace(const char *szBuf)
{
    if(m_pTraceFile == NULL) return;
    
    TraceBegin();
    TraceContent(szBuf);
    TraceContent(S_LN_CR);
    TraceEnd();
}

/*
    ��������:дһ��TRACE��¼.

    1.��Trace�ļ�δ�򿪾Ͳ�дTrace�ļ�.
    2.��������ʽΪ��Ͳ���ֹ����.
    3.��������ʽΪ�پ���ֹ����.

    ����:
    expression,��Ҫ�����߼����ʽ,Ϊ�㴥��Assert.
    szSourceFile,���øú�����������ڵ�Դ�ļ�����ȫ·����.
    nLineNum,���øú�����������ڵ��к�.

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/
void CTrace::TraceAssert(const BOOL expression, 
                         const char* szSourceFile,
                         const int nLineNum)
{
    if(expression) return;

    if(m_pTraceFile != NULL)
    {
        char pMsg[1024];
        SNPRINTF(pMsg,1024,S_FRAME_TRACE_ASSERT,szSourceFile, nLineNum,S_LN_CR);
		pMsg[sizeof(pMsg) - 1] = 0;
		strncat(pMsg, S_LN_CR, sizeof(pMsg));
		pMsg[sizeof(pMsg) - 1] = 0;
        
        TraceBegin();
        TraceContent(pMsg);
        TraceEnd();
    }

#ifdef _DEBUG

    /* ��ֹ���� */
    _exit(0);
#endif
}

#ifdef  _PLATFORM_WIN32

/*
    ��������:дһ��TRACE��¼.

    1.��Trace�ļ�δ�򿪾Ͳ�дTrace�ļ�.
    2.��������ʽΪ��Ͳ���ֹ����.
    3.��������ʽΪ�پ���ֹ����.

    ����:
    pException,NT�쳣��ָ��.

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/
void CTrace::TraceNTException(LPEXCEPTION_POINTERS pException)
{
    if(m_pTraceFile == NULL) return;

    //���EXCEPTION��¼ͷ���ݡ�
    LPEXCEPTION_RECORD pExceptionRecord;
    pExceptionRecord =  pException->ExceptionRecord;

    char pBuf[1024];

    TraceBegin();
    while (pExceptionRecord)
    {
        TraceContent(S_LN_CR);
        TraceContent(S_LN_CR);
        TraceContent("nException");
        TraceContent(S_LN_CR);
        switch(pExceptionRecord->ExceptionCode)
        {
        case EXCEPTION_ACCESS_VIOLATION:
            sprintf(pBuf, "Exception    Code: "
                    "EXCEPTION_ACCESS_VIOLATION%s", S_LN_CR);
            break;
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
            sprintf(pBuf, "Exception    Code: "
                    "EXCEPTION_ARRAY_BOUNDS_EXCEEDED%s", S_LN_CR);
            break;
        case EXCEPTION_FLT_DENORMAL_OPERAND:
            sprintf(pBuf, "Exception    Code: "
                    "EXCEPTION_FLT_DENORMAL_OPERAND%s", S_LN_CR);
            break;
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            sprintf(pBuf, "Exception    Code: "
                    "EXCEPTION_FLT_DIVIDE_BY_ZERO%s", S_LN_CR);
            break;
        case EXCEPTION_FLT_INEXACT_RESULT:
            sprintf(pBuf, "Exception    Code: "
                    "EXCEPTION_FLT_INEXACT_RESULT%s", S_LN_CR);
            break;
        case EXCEPTION_FLT_INVALID_OPERATION:
            sprintf(pBuf,"Exception    Code: "
                    "EXCEPTION_FLT_INVALID_OPERATION%s", S_LN_CR);
            break;
        case EXCEPTION_FLT_OVERFLOW:
            sprintf(pBuf, "Exception    Code: "
                    "EXCEPTION_FLT_OVERFLOW%s", S_LN_CR);
            break;
        case EXCEPTION_FLT_STACK_CHECK:
            sprintf(pBuf, "Exception    Code: "
                    "EXCEPTION_FLT_STACK_CHECK%s", S_LN_CR);
            break;
        case EXCEPTION_FLT_UNDERFLOW:
            sprintf(pBuf, "Exception    Code: "
                    "EXCEPTION_FLT_UNDERFLOW%s", S_LN_CR);
            break;
        case EXCEPTION_INT_DIVIDE_BY_ZERO:
            sprintf(pBuf, "Exception    Code: "
                    "EXCEPTION_FLT_UNDERFLOW%s", S_LN_CR);
            break;
        case EXCEPTION_INT_OVERFLOW:
            sprintf(pBuf, "Exception    Code: "
                    "EXCEPTION_FLT_UNDERFLOW%s", S_LN_CR);
            break;
        case EXCEPTION_PRIV_INSTRUCTION:
            sprintf(pBuf, "Exception    Code: "
                    "EXCEPTION_FLT_UNDERFLOW%s", S_LN_CR);
            break;
        case EXCEPTION_NONCONTINUABLE_EXCEPTION:
            sprintf(pBuf, "Exception    Code: "
                    "EXCEPTION_NONCONTINUABLE_EXCEPTION%s", S_LN_CR);
            break;
        default:
            sprintf(pBuf, "Exception    Code: 0x%08x%s",
                    pExceptionRecord->ExceptionCode, S_LN_CR);
            break;
        }

        TraceContent(pBuf);
        sprintf(pBuf, "Exception   Flags: 0x%08x%s",
            pExceptionRecord->ExceptionFlags, S_LN_CR);
        TraceContent(pBuf);
        sprintf(pBuf, "Exception Address: 0x%08x%s",
            pExceptionRecord->ExceptionAddress, S_LN_CR);
        TraceContent(pBuf);

        pExceptionRecord = pExceptionRecord->ExceptionRecord;
    }
    
    /*
        ���ջ��ǰ1024���ֽڵ�����,��¼�ֳ�ջ,
        ���к������ص�ַ�����Ű�Χ.
    */
    TraceContent(S_LN_CR);
    TraceContent("Stack:");
    TraceContent(S_LN_CR);

    BYTE *pStack = (BYTE *)pException->ContextRecord->Ebp;
    UINT4 BP = (UINT4)pStack;
    for (int i = 0; i < 1024 / 16; i++)
    {
        //���ÿ�еĵ�ַ
        sprintf(pBuf, "%08x   ", pStack);
        TraceContent(pBuf);
        for (int j = 0; j < 16; j++)
        {
            BYTE *p = pStack + j;
            //���ص�ַ�ĵ�һ���ֽ�
            if ((UINT4)p == BP + 4)
            {
                sprintf(pBuf, "(%02x", *p);
            }
            else
            //���ص�ַ�����һ���ֽ�
            {
                if (((j != 0) && ((UINT4)p == BP + 8)) ||
                    ((j == 15) && ((UINT4)p == BP + 7)))
                {
                    sprintf(pBuf, ")%02x", *p);
                    BP = *(UINT4 *)BP;
                }
                else
                {
                    sprintf(pBuf, " %02x", *p);
                }
            }
            TraceContent(pBuf);
        }
        pStack += 16;
        TraceContent(S_LN_CR);
    }

    TraceEnd();
}

#endif
