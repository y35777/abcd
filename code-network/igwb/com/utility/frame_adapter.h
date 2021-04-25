#ifndef __FRAME_ADAPTER_H__
#define __FRAME_ADAPTER_H__

#include "../include/i_frame_adapter.h"

class CFrameAdapter : public IFrameAdapter
{
public:
    virtual void MessageOut(const char* szModuleName,
                            const BYTE nMsgOutLevel,
                            const char* szFmt, ...);

    virtual void TrackProtocol(const char* pMsgProtocol);
    
    virtual void WriteLog(const char* szModuleName,
                          const char* szSysLog,
                          BYTE bLogResult = SLog::OPER_SUCCESS);
    virtual void WriteLog(const SLog* pLog);

    /* 向双Agent模块发送倒换命令 */
    virtual void AskForFailOver(const MODULE_TYPE mt, 
                                const BYTE nCause,
                                const void* pData = 0,
                                const UINT4 nLen = 0);

    /* 向Agent模块发送同步数据 */
    virtual void WriteSyncData(const MODULE_TYPE mt,
                               const void* pData,
                               const UINT4 nLen,
                               const UINT4 nChlID = 0);

    /* 向告警模块发送告警消息 */
    virtual void SendAlarm(const SInnerAlarm* pAlarm);

    virtual int GetProcessType(void);
};

#endif //__FRAME_ADAPTER_H__
