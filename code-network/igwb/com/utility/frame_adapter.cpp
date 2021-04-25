#include "../include/frame.h"
#include "../include/toolbox.h"
#include "frame_adapter.h"

void CFrameAdapter::MessageOut(const char* szModuleName,
                               const BYTE nMsgOutLevel,
                               const char* szFmt, ...)
{
    char buf[1024];
    char* pBuf = buf;

    va_list argptr;

    va_start(argptr, szFmt);
    VSNPRINTF(pBuf,1024,szFmt,argptr);
    buf[1023] = 0;

    va_end(argptr);

    CMsgProcessor::MessageOut(szModuleName, nMsgOutLevel, "%s", buf);
}

void CFrameAdapter::TrackProtocol(const char* pMsgProtocol)
{
    CMsgProcessor::TrackProtocol(pMsgProtocol);
}

void CFrameAdapter::WriteLog(const char* szModuleName,
                             const char* szSysLog,
                             BYTE bLogResult)
{
    CMsgProcessor::WriteLog(szModuleName, szSysLog, bLogResult);
}

void CFrameAdapter::WriteLog(const SLog* pLog)
{
    CMsgProcessor::WriteLog(pLog);
}

/* ��˫Agentģ�鷢�͵������� */
void CFrameAdapter::AskForFailOver(const MODULE_TYPE mt, 
                                   const BYTE nCause,
                                   const void* pData,
                                   const UINT4 nLen)
{
    CMsgProcessor::AskForFailOver(mt, nCause, pData, nLen);
}

/* ��Agentģ�鷢��ͬ������ */
void CFrameAdapter::WriteSyncData(const MODULE_TYPE mt,
                                  const void* pData,
                                  const UINT4 nLen,
                                  const UINT4 nChlID)
{
    CMsgProcessor::WriteSyncData(mt, pData, nLen, nChlID);
}

/* ��澯ģ�鷢�͸澯��Ϣ */
void CFrameAdapter::SendAlarm(const SInnerAlarm* pAlarm)
{
    CMsgProcessor::SendAlarm(pAlarm);
}

int CFrameAdapter::GetProcessType(void)
{
    return (int)CMsgProcessor::GetProcessType();
}
