#ifndef __COLLECTOR_ADAPTER_H__
#define __COLLECTOR_ADAPTER_H__



class CCollectorAdapter
{
public:
    static void Trace(const char* szFmt, ...);
    static void TraceAssert(const char* szSourceFile,
                            const int nLineNum);
    static void MessageOut(const BYTE nMsgOutLevel,
                           const char* szFmt, ...);

    static void WriteLog(const char* szSysLog,
                         BYTE bLogResult = SLog::OPER_SUCCESS);
    static void WriteLog(const SLog* pLog);

    /* 向双Agent模块发送倒换命令 */
    static void AskForFailOver(const BYTE nCause,
                               const void* pData = 0,
                               const UINT4 nLen = 0);

    /* 向告警模块发送告警消息 */
    static void SendAlarm(const SInnerAlarm* pAlarm);

    static const char* GetCfgFilePath(void);

    static const char* GetAppPath(void);
    
    //取接入点名
    static const char* GetAPName(void);

    static int GetProcessType(void);

    static const char* GetFrontSavePath(void);

    static const char* GetBackSavePath(void);

    static const char* GetSecondBackSavePath(void);

    static BOOL GetbAPSaveSecond(void);

    static BOOL GetbNeedSencondAdapter(void);

	static int  GetFtpPasvMode(void);
private:
    static char g_szFrontSaveRoot[MAX_PATH];
    static char g_szBackSaveRoot[MAX_PATH];
    static char g_szSecondBackSaveRoot[MAX_PATH];
    static char g_szAPName[100];
    static int  g_nAPSaveSecond;
    static int  g_nNeedSecondAdapter;
};

#endif //__COLLECTOR_ADAPTER_H__
