#include "../include/export_dll.h"
#include "frame_tool.h"

IFrameAdapter* CFrameTools::g_pFrameAdapter = NULL;

void CFrameTools::SetInstance(IFrameAdapter* pFrameAdapter)
{
    if(NULL == g_pFrameAdapter)
    {
        g_pFrameAdapter = pFrameAdapter;
    }
}

void CFrameTools::MessageOut(const char* szModuleName,
                             const BYTE nMsgOutLevel,
                             const char* szFmt, ...)
{
    if(g_pFrameAdapter)
    {
        char buf[1024];
        char* pBuf = buf;
    
        va_list argptr;
    
        va_start(argptr, szFmt);
        VSNPRINTF(pBuf,1024,szFmt,argptr);
        buf[1023] = 0;
    
        va_end(argptr);
    
        g_pFrameAdapter->MessageOut(szModuleName, nMsgOutLevel, "%s", buf);
    }
}

void CFrameTools::TrackProtocol(const char* pMsgProtocol)
{
    if(g_pFrameAdapter)
    {
        g_pFrameAdapter->TrackProtocol(pMsgProtocol);
    }
}

void CFrameTools::WriteLog(const char* szModuleName,
                           const char* szSysLog,
                           BYTE bLogResult)
{
    if(g_pFrameAdapter)
    {
        g_pFrameAdapter->WriteLog(szModuleName, szSysLog, bLogResult);
    }
}

void CFrameTools::WriteLog(const SLog* pLog)
{
    if(g_pFrameAdapter)
    {
        g_pFrameAdapter->WriteLog(pLog);
    }
}

/* 向双Agent模块发送倒换命令 */
void CFrameTools::AskForFailOver(const MODULE_TYPE mt, 
                                 const BYTE nCause,
                                 const void* pData,
                                 const UINT4 nLen)
{
    if(g_pFrameAdapter)
    {
        g_pFrameAdapter->AskForFailOver(mt, nCause, pData, nLen);
    }
}

/* 向Agent模块发送同步数据 */
void CFrameTools::WriteSyncData(const MODULE_TYPE mt,
                                const void* pData,
                                const UINT4 nLen,
                                const UINT4 nChlID)
{
    if(g_pFrameAdapter)
    {
        g_pFrameAdapter->WriteSyncData(mt, pData, nLen, nChlID);
    }
}

/* 向告警模块发送告警消息 */
void CFrameTools::SendAlarm(const SInnerAlarm* pAlarm)
{
    if(g_pFrameAdapter)
    {
        g_pFrameAdapter->SendAlarm(pAlarm);
    }
}

int CFrameTools::GetProcessType(void)
{
    if(g_pFrameAdapter)
    {
        return (int)g_pFrameAdapter->GetProcessType();
    }

    return PT_SELF;
}
