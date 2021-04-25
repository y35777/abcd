#ifndef __FRAME_TOOLS_H__
#define __FRAME_TOOLS_H__

#include "../include/base_type.h"
#include "../include/i_frame_adapter.h"

class IGWB_Svc_Export CFrameTools
{
public:
    static void SetInstance(IFrameAdapter* pFrameAdapter);

    static void MessageOut(const char* szModuleName,
                            const BYTE nMsgOutLevel,
                            const char* szFmt, ...);

    static void TrackProtocol(const char* pMsgProtocol);
    
    static void WriteLog(const char* szModuleName,
                          const char* szSysLog,
                          BYTE bLogResult = SLog::OPER_SUCCESS);
    static void WriteLog(const SLog* pLog);

    /* 向双Agent模块发送倒换命令 */
    static void AskForFailOver(const MODULE_TYPE mt, 
                                const BYTE nCause,
                                const void* pData = 0,
                                const UINT4 nLen = 0);

    /* 向Agent模块发送同步数据 */
    static void WriteSyncData(const MODULE_TYPE mt,
                               const void* pData,
                               const UINT4 nLen,
                               const UINT4 nChlID = 0);

    /* 向告警模块发送告警消息 */
    static void SendAlarm(const SInnerAlarm* pAlarm);

    static int GetProcessType(void);

protected:
    static IFrameAdapter* g_pFrameAdapter;
};

#endif //__FRAME_TOOLS_H__
