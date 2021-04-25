
#if !defined(AFX_PWD_H__F8204F49_3025_11D3_8D00_00C0DF2467C6__INCLUDED_)
#define AFX_PWD_H__F8204F49_3025_11D3_8D00_00C0DF2467C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "winsvc.h"

////////////////////////////////////////////////////////////////
// CProcess::CreateProcess返回值定义
#define CREATE_PROCESS_SUCCESS      0
#define CREATE_PROCESS_FAILED       1
#define REPEAT_TOO_MANY_TIMES       2

///////////////////////////////////////////////////////////////////
// CServiceBase常量定义
#define PWD_SERVICE_NAME            "CFGService"
#define PWD_SERVICE_DISPLAYNAME     "Cfg Server Servive"

class CServiceBase
{
public:
    CServiceBase();
    ~CServiceBase();

public:
    BOOL Install();
    BOOL Remove();
    BOOL StartDispatcher();
    BOOL Start();
    BOOL Stop();
    void Run();

protected:
    virtual void OnRun(){};
    virtual void OnStop(){};

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
    static CServiceBase* g_ServInstance;
};

#endif // !defined(AFX_PWD_H__F8204F49_3025_11D3_8D00_00C0DF2467C6__INCLUDED_)
