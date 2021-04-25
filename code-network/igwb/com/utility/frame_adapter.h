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

    /* ��˫Agentģ�鷢�͵������� */
    virtual void AskForFailOver(const MODULE_TYPE mt, 
                                const BYTE nCause,
                                const void* pData = 0,
                                const UINT4 nLen = 0);

    /* ��Agentģ�鷢��ͬ������ */
    virtual void WriteSyncData(const MODULE_TYPE mt,
                               const void* pData,
                               const UINT4 nLen,
                               const UINT4 nChlID = 0);

    /* ��澯ģ�鷢�͸澯��Ϣ */
    virtual void SendAlarm(const SInnerAlarm* pAlarm);

    virtual int GetProcessType(void);
};

#endif //__FRAME_ADAPTER_H__
