#ifndef __ADAPTER_INCLUDE__
#define __ADAPTER_INCLUDE__


#include "../../include/base_type.h"
#include "../collector_adapter.h"

#define GETAPPPATH CCollectorAdapter::GetAppPath()

#define COTRACE CCollectorAdapter::Trace

#define COMSGOUT CCollectorAdapter::MessageOut

#define SENDALARM CCollectorAdapter::SendAlarm

inline void WRITELOG_C(const char* szFmt, ...)
{
    char pBuf[1024];
    va_list argptr;

    va_start(argptr, szFmt);
    VSNPRINTF(pBuf,1024,szFmt,argptr);
    pBuf[1023] = 0;

    va_end(argptr);

    if(pBuf[0] != '\0')
    {
        CCollectorAdapter::WriteLog(pBuf);
    }
}

inline void WRITELOG_F(const char* szFmt, ...)
{
    char pBuf[1024];
    va_list argptr;

    va_start(argptr, szFmt);
    VSNPRINTF(pBuf,1024,szFmt,argptr);
    pBuf[1023] = 0;

    va_end(argptr);

    if(pBuf[0] != '\0')
    {
        CCollectorAdapter::WriteLog(pBuf, SLog::OPER_FAIL);
    }
}
#endif //__ADAPTER_INCLUDE__
