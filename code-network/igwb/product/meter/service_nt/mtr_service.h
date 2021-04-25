#ifndef __METER_ACC_SERVICE_H__
#define __METER_ACC_SERVICE_H__

#include "winsvc.h"

///////////////////////////////////////////////////////////////////
// CMeterAccService常量定义
#define METER_ACC_SERVICE_NAME            "MeterAccService"
#define METER_ACC_SERVICE_DISPLAYNAME     "Meter Accumulation Servive"

class CMeterAccService
{
public:
    CMeterAccService();
    ~CMeterAccService();

public:
    BOOL Install();
    BOOL Remove();
    BOOL StartDispatcher();
    BOOL Start();
    BOOL Stop();
    void Run();

protected:
    virtual void OnRun();
    virtual void OnStop();

private:
    static SERVICE_TABLE_ENTRY m_pDispatchTable[];
    static void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);
    static void WINAPI ServiceHandler(DWORD dwControl);

    SERVICE_STATUS_HANDLE m_hServiceStatus;
    void SetServiceState(DWORD dwState);

    HANDLE m_hStopEvent;

    static void ReportEvent(WORD wType, LPCTSTR szMsg);
    BOOL StartAndStopHandler(BOOL bStart);

public:
    static CMeterAccService* g_pServInstance;
	ACE_Process_Mutex* pMutexAuto;
};

#endif // __METER_ACC_SERVICE_BASE_H__
