/////////////////////////////////////////////////////////////////////
// PWD.cpp  -- 进程监视服务
// chenghui, 1999.7.2
// 原PWD.cpp在1999.4完成，由于需要使用MFC功能，故重新使用VC的
// AppWizard生成程序框架，并将原PWD.cpp的重要代码拷贝到本文件
// 中。并做以下修改：
// 当一个进程在n秒钟之内重复启动了m次之后，不再尝试启动该程序，
// 而调用另外一个异常处理程序。
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// 以下代码为VC所生成的框架代码
#include "stdafx.h"
#include "service_base.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//CWinApp theApp;
DWORD ModifyDefaultDacl(HANDLE hProcess);

CServiceBase* CServiceBase::g_ServInstance = NULL;
//////////////////////////////////////////////////////////////
// 以下代码为CServiceBase的实现，CServiceBase启动时将创建并监视
// 若干个CProcess，一旦发现某一个CProcess结束，则立即重新启动
// 该CProcess。但如果在规定时间启动该CProcess超过规定次数，则
// 不再重新启动该CProcess，而调用该CProcess的OnException方法
//////////////////////////////////////////////////////////////
CServiceBase::CServiceBase()
{
    m_hStopEvent    = NULL;
}

CServiceBase::~CServiceBase()
{
}

extern "C" {
typedef BOOL  (WINAPI *CHANGESERVICECONFIG2)(SC_HANDLE, DWORD, LPVOID);	
}
///////////////////////////////////////////////////
// CServiceBase::Install安装PWD服务
// 调用者: _tmain
BOOL CServiceBase::Install()
{
    SC_HANDLE hService;
    SC_HANDLE hSCManager;
    char szPath[MAX_PATH];
    char szAppPath[MAX_PATH];

    if (!GetModuleFileName(NULL, szPath, MAX_PATH))
    {
        printf("Call GetModuleFileName Failed.");
        return FALSE;
    }

    _snprintf(szAppPath,
              sizeof(szAppPath),
              "%s -startapp",
              szPath);
    szAppPath[sizeof(szAppPath) - 1] = '\0';

    if (!(hSCManager = OpenSCManager(
        NULL, NULL, SC_MANAGER_ALL_ACCESS)))
    {
        printf("Call OpenSCManager Failed.");
        return FALSE;
    }

    //如果该服务已经存在 ZhengYuqun 2004-06-14 SWPD05961
    if (hService = OpenService(hSCManager,
        PWD_SERVICE_NAME, SERVICE_ALL_ACCESS))
    {
        //先停止，然后更新路径为新路径，更新启动类别为自动启动
        Stop();
        if(!ChangeServiceConfig(hService,
                                SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS ,
                                SERVICE_AUTO_START, 
                                SERVICE_ERROR_NORMAL,
                                szAppPath,
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL))
        {
            CloseServiceHandle(hService);
            CloseServiceHandle(hSCManager);
            printf("Call ChangeServiceConfig Failed.");
            return FALSE;
        }
    } //End SWPD05961
    else if (!(hService = CreateService(hSCManager,
                                   PWD_SERVICE_NAME,
                                   PWD_SERVICE_DISPLAYNAME,
                                   SERVICE_ALL_ACCESS,
                                   SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
                                   SERVICE_AUTO_START,
                                   SERVICE_ERROR_NORMAL,
                                   szAppPath,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL)))
    {
        CloseServiceHandle(hSCManager);
        printf("Call CreateService Failed.");
        return FALSE;
    }

    CHANGESERVICECONFIG2 f = NULL;
    HMODULE h = LoadLibrary("Advapi32.dll");
    if(h != NULL)
    {
        f = (CHANGESERVICECONFIG2)GetProcAddress(h, "ChangeServiceConfig2A");
        if(NULL != f)
        {
            SC_ACTION Sc_Action[3];
            Sc_Action[0].Type       = SC_ACTION_RESTART;
            Sc_Action[0].Delay      = 5000;
            Sc_Action[1].Type       = SC_ACTION_RESTART;
            Sc_Action[1].Delay      = 10000;
            Sc_Action[2].Type       = SC_ACTION_RESTART;
            Sc_Action[2].Delay      = 60000;

            SERVICE_FAILURE_ACTIONS f_actions;
            f_actions.dwResetPeriod = 0;
            f_actions.cActions      = 3;
            f_actions.lpCommand     = NULL;
            f_actions.lpRebootMsg   = NULL;
            f_actions.lpsaActions   = (SC_ACTION*)&Sc_Action;

            f(hService, SERVICE_CONFIG_FAILURE_ACTIONS, &f_actions);
        }
        FreeLibrary(h);
    }
    printf("Service Installed.");

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);

    return TRUE;
}

//////////////////////////////////////////////////
// CServiceBase::Remove删除PWD服务
// 调用者: _tmain
BOOL CServiceBase::Remove()
{
    SC_HANDLE hService;
    SC_HANDLE hSCManager;

    if (!(hSCManager = OpenSCManager(
            NULL, NULL, SC_MANAGER_ALL_ACCESS)))
        printf("Call OpenSCManager Failed.");
    else if (!(hService = OpenService(hSCManager,
            PWD_SERVICE_NAME, SERVICE_ALL_ACCESS)))
        printf("Call OpenService Failed.");

    BOOL bSuccess =FALSE;
    if (hSCManager && hService)
    {
        SERVICE_STATUS status;
        QueryServiceStatus(hService, &status);
        if (status.dwCurrentState != SERVICE_STOPPED)
            printf("Service is running, stop it first.");
        else if (!DeleteService(hService))
            printf("Call DeleteService Failed.");
        else
        {
            printf("Service removed.");
            bSuccess = TRUE;
        }
    }

    if (hSCManager) CloseServiceHandle(hSCManager);
    if (hService) CloseServiceHandle(hService);

    return bSuccess;
}

BOOL CServiceBase::Start()
{
    return StartAndStopHandler(TRUE);
}

BOOL CServiceBase::Stop()
{
    return StartAndStopHandler(FALSE);
}

//////////////////////////////////////////////////
// CServiceBase::StartDispatcher开始运行Service
// 调用者: _tmain
SERVICE_TABLE_ENTRY CServiceBase::m_pDispatchTable[] = 
{
    {PWD_SERVICE_NAME, CServiceBase::ServiceMain},
    { NULL, NULL }
};

BOOL CServiceBase::StartDispatcher()
{
    return StartServiceCtrlDispatcher(m_pDispatchTable);
}

///////////////////////////////////////////////////////
// CServiceBase::Main提供ServiceMain回调函数。该函数由
// m_pDispatchTable.lpServiceProc提供给StartDispatcher
void WINAPI CServiceBase::ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
    g_ServInstance->m_hServiceStatus = 
        RegisterServiceCtrlHandler(
            PWD_SERVICE_NAME, ServiceHandler);
    if (!g_ServInstance->m_hServiceStatus) 
    {
        ReportEvent(EVENTLOG_ERROR_TYPE, 
            "Call RegisterServiceCtrlHandler Failed");
        return;
    }

    g_ServInstance->SetServiceState(SERVICE_START_PENDING);
    g_ServInstance->m_hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    g_ServInstance->SetServiceState(SERVICE_RUNNING);

    g_ServInstance->Run();

    g_ServInstance->SetServiceState(SERVICE_STOPPED);

    SetEvent(g_ServInstance->m_hStopEvent);
}

////////////////////////////////////////////////////////////
// CServiceBase::ServiceHandler提供ServiceHandler回调函数。
// 该函数被作为RegisterServiceCtrlHandler的参数提供给SCM。
// SCM在需要停止该服务时以SERVICE_CONTROL_STOP为参数来调
// 用该函数。函数通过设置m_hStopEvent来导致Run结束。
void WINAPI CServiceBase::ServiceHandler(DWORD dwControl)
{
    switch (dwControl)
    {
    case SERVICE_CONTROL_STOP:
        g_ServInstance->OnStop();
        if(g_ServInstance->m_hStopEvent != NULL)
        {
            WaitForSingleObject(g_ServInstance->m_hStopEvent, INFINITE);
            CloseHandle(g_ServInstance->m_hStopEvent);
        }
        break;
    }
}

//////////////////////////////////////////////////////
// CServiceBase::SetServiceState向SCM报告本服务的
// 状态，dwState指定现在的状态。
// 调用者: CServiceBase::ServiceMain
void CServiceBase::SetServiceState(DWORD dwState)
{
    SERVICE_STATUS status;

    status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    status.dwCurrentState = dwState;
    if (dwState == SERVICE_START_PENDING)
        status.dwControlsAccepted = 0;
    else
        status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    
    status.dwWin32ExitCode = 0;
    status.dwServiceSpecificExitCode = 0;
    status.dwCheckPoint = 0;
    status.dwWaitHint = 0;
    
    SetServiceStatus(g_ServInstance->m_hServiceStatus, &status);
}

///////////////////////////////////////////////////////
// CServiceBase::ReportEvent向事件记录中增加一条记录
// 事件类型和相应的信息分别由wType和szMsg指定
// 调用者: CServiceBase::ServiceMain, CServiceBase::Run
void CServiceBase::ReportEvent(WORD wType, LPCTSTR szMsg)
{
    HANDLE hEventLog;
    if (hEventLog = RegisterEventSource(
        NULL, PWD_SERVICE_NAME))
    {
        ::ReportEvent(hEventLog, 
            wType, 0, 0, NULL, 1, 0, &szMsg, NULL);
        DeregisterEventSource(hEventLog);
    }
}

////////////////////////////////////////////////////////
// CServiceBase::Run负责启动并监视进程知道m_hStopEvent
// 被置位。
// 调用者：CServiceBase::ServiceMain
void CServiceBase::Run()
{
    OnRun();
}

//////////////////////////////////////////////////////////////////////
// CServiceBase::StartAndStopHandler负责启动或终止进程
// 调用者: CServiceBase::Start, CServiceBase::Stop
BOOL CServiceBase::StartAndStopHandler(BOOL bStart)
{
    SC_HANDLE hService;
    SC_HANDLE hSCManager;

    if (!(hSCManager = OpenSCManager(
            NULL, NULL, SC_MANAGER_ALL_ACCESS)))
        ReportEvent(EVENTLOG_ERROR_TYPE, "Call OpenSCManager Failed");
    else if (!(hService = OpenService(hSCManager,
            PWD_SERVICE_NAME, SERVICE_ALL_ACCESS)))
        ReportEvent(EVENTLOG_ERROR_TYPE, "Call OpenService Failed");

    BOOL bSuccess = FALSE;
    if (hSCManager && hService)
    {
        if (bStart)
        {
            if (!StartService(hService, 0, NULL))
                ReportEvent(EVENTLOG_ERROR_TYPE, "Call StartService Failed");
            else
            {
                ReportEvent(EVENTLOG_INFORMATION_TYPE, "Service Started");
                bSuccess = TRUE;
            }
        } else
        {
            SERVICE_STATUS status;
            if (!ControlService(hService, 
                    SERVICE_CONTROL_STOP, &status))
                ReportEvent(EVENTLOG_ERROR_TYPE, "Call ControlService Failed");
            else
            {
                ReportEvent(EVENTLOG_INFORMATION_TYPE, "Service Stoped");
                bSuccess = TRUE;
            }
        }
    }

    if (hSCManager) CloseServiceHandle(hSCManager);
    if (hService) CloseServiceHandle(hService);

    return bSuccess;
}
