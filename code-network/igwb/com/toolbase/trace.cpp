/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:trace.cpp
    描述信息:
    
    包含CTrace类的实现,该类用于在整个进程中支持TRACE信息.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-16,创建.
*/
#include "trace.h"

/*
    每当写TRACE的次数超过_FLUSH_TRACE_TIMES_,就刷新一次磁盘文件.
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
    函数功能:析构函数,关闭Trace文件,释放互斥锁.

    修改记录:
    周拥辉,2001-10-16,创建.
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
    函数功能:析构函数,关闭Trace文件,释放互斥锁.

    1.以追加的方式打开Trace文件.
    2.把Trace文件名的后缀更改为tmp,以作为备份Trace文件名.

    参数:
    szFileName,Trace文件名.

    修改记录:
    周拥辉,2001-10-16,创建.
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
    //修改D11623---Trace临时文件名少一个字符
    m_pTempFileName[pPos - m_pFileName ] = '\0';
    //修改完毕
    strcat(m_pTempFileName, ".tmp");
}

/*
    函数功能:写一条TRACE记录的时间部分.

    1.在写Trace文件之前首先获得互斥锁.
    2.把当前时间以"Sat Jul 21 13:05:06 2001"格式写入文件.

    修改记录:
    周拥辉,2001-10-16,创建.
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
    函数功能:写一条TRACE记录的时间部分.

    1.若Trace文件超大则关闭之,并生成新文件.
    2.保留Trace文件的一个备份.
    3.写Trace文件完毕,释放互斥锁.

    修改记录:
    周拥辉,2001-10-16,创建.
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
    函数功能:写一条TRACE记录.

    1.若Trace文件未打开就不作任何操作.

    参数:
    pFmt,Trace记录字符串指针.

    修改记录:
    周拥辉,2001-10-16,创建.
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
    函数功能:写一条TRACE记录.

    1.若Trace文件未打开就不写Trace文件.
    2.若输入表达式为真就不终止进程.
    3.若输入表达式为假就终止进程.

    参数:
    expression,需要检查的逻辑表达式,为零触发Assert.
    szSourceFile,调用该函数的语句所在的源文件名的全路径名.
    nLineNum,调用该函数的语句所在的行号.

    修改记录:
    周拥辉,2001-10-16,创建.
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

    /* 终止进程 */
    _exit(0);
#endif
}

#ifdef  _PLATFORM_WIN32

/*
    函数功能:写一条TRACE记录.

    1.若Trace文件未打开就不写Trace文件.
    2.若输入表达式为真就不终止进程.
    3.若输入表达式为假就终止进程.

    参数:
    pException,NT异常的指针.

    修改记录:
    周拥辉,2001-10-16,创建.
*/
void CTrace::TraceNTException(LPEXCEPTION_POINTERS pException)
{
    if(m_pTraceFile == NULL) return;

    //输出EXCEPTION记录头内容。
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
        输出栈中前1024个字节的内容,记录现场栈,
        其中函数返回地址用括号包围.
    */
    TraceContent(S_LN_CR);
    TraceContent("Stack:");
    TraceContent(S_LN_CR);

    BYTE *pStack = (BYTE *)pException->ContextRecord->Ebp;
    UINT4 BP = (UINT4)pStack;
    for (int i = 0; i < 1024 / 16; i++)
    {
        //输出每行的地址
        sprintf(pBuf, "%08x   ", pStack);
        TraceContent(pBuf);
        for (int j = 0; j < 16; j++)
        {
            BYTE *p = pStack + j;
            //返回地址的第一个字节
            if ((UINT4)p == BP + 4)
            {
                sprintf(pBuf, "(%02x", *p);
            }
            else
            //返回地址的最后一个字节
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
