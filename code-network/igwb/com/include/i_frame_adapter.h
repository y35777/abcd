#ifndef __I_FRAME_ADAPTER_H__
#define __I_FRAME_ADAPTER_H__



class IFrameAdapter
{
public:
    virtual void MessageOut(const char* szModuleName,
                            const BYTE nMsgOutLevel,
                            const char* szFmt, ...) = 0;

    virtual void TrackProtocol(const char* pMsgProtocol) = 0;
    
    virtual void WriteLog(const char* szModuleName,
                          const char* szSysLog,
                          BYTE bLogResult = SLog::OPER_SUCCESS) = 0;
    virtual void WriteLog(const SLog* pLog) = 0;

    /* ��˫Agentģ�鷢�͵������� */
    virtual void AskForFailOver(const MODULE_TYPE mt, 
                                const BYTE nCause,
                                const void* pData = 0,
                                const UINT4 nLen = 0) = 0;

    /* ��Agentģ�鷢��ͬ������ */
    virtual void WriteSyncData(const MODULE_TYPE mt,
                               const void* pData,
                               const UINT4 nLen,
                               const UINT4 nChlID = 0) = 0;
                               
    /* ��澯ģ�鷢�͸澯��Ϣ */
    virtual void SendAlarm(const SInnerAlarm* pAlarm) = 0;

    virtual int GetProcessType(void) = 0;
};

#endif //__I_FRAME_ADAPTER_H__
