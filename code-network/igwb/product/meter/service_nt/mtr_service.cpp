#include "../../../com/include/toolbox.h"
#include "../mtr_manager.h"
#include "mtr_service.h"
#include "../../../com/include/resource.h"


CMeterAccService* CMeterAccService::g_pServInstance = NULL;
/* 程序名不带路径名和后缀 */
extern IGWB_Svc_Export char g_szAppName[MAX_PATH];
extern IGWB_Svc_Export char g_szAppPath[MAX_PATH];


CMeterAccService::CMeterAccService()
{
    m_hStopEvent    = NULL;
	pMutexAuto      = NULL; 
}

CMeterAccService::~CMeterAccService()
{
	if(pMutexAuto != NULL)
	{
		pMutexAuto->release();
		delete pMutexAuto;
	}	
}

extern "C" {
typedef BOOL  (WINAPI *CHANGESERVICECONFIG2)(SC_HANDLE, DWORD, LPVOID);	
}
///////////////////////////////////////////////////
// CMeterAccService::Install安装服务
// 调用者: _tmain
BOOL CMeterAccService::Install()
{
    SC_HANDLE hService;
    SC_HANDLE hSCManager;
    char szPath[MAX_PATH];
    char szAppPath[MAX_PATH];

    if (!GetModuleFileName(NULL, szPath, MAX_PATH))
    {
        printf("Failed to call GetModuleFileName. Error Code = %d\n", GetLastError());
        return FALSE;
    }

    _snprintf(szAppPath,
              sizeof(szAppPath),
              "%s -startapp",
              szPath);
    szAppPath[sizeof(szAppPath) - 1] = '\0';

    if (!(hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)))
    {
        printf("Failed to call OpenSCManager. Error Code = %d\n", GetLastError());
        return FALSE;
    }

    if (!(hService = CreateService(hSCManager,
                                   METER_ACC_SERVICE_NAME,
                                   METER_ACC_SERVICE_DISPLAYNAME,
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
        printf("Failed to call CreateService. Error Code = %d\n", GetLastError());
        return FALSE;
    }

    CHANGESERVICECONFIG2 f = NULL;
    HMODULE h = LoadLibrary("Advapi32.dll");
    if (h != NULL)
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
    printf("Service Installed.\n");

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);

    return TRUE;
}

//////////////////////////////////////////////////
// CMeterAccService::Remove删除服务
// 调用者: _tmain
BOOL CMeterAccService::Remove()
{
    SC_HANDLE hService;
    SC_HANDLE hSCManager;

    if (!(hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)))
    {
        printf("Failed to call OpenSCManager. Error Code = %d\n", GetLastError());
    }
    else if (!(hService = OpenService(hSCManager, METER_ACC_SERVICE_NAME, SERVICE_ALL_ACCESS)))
    {
        printf("Failed to call OpenService. Error Code = %d\n", GetLastError());
    }

    BOOL bSuccess = FALSE;
    if (hSCManager && hService)
    {
        SERVICE_STATUS status;
        QueryServiceStatus(hService, &status);

        if (status.dwCurrentState != SERVICE_STOPPED)
        {
            printf("Service is running, stop it first.");
        }
        else if (!DeleteService(hService))
        {
            printf("Failed to call DeleteService. Error Code = %d\n", GetLastError());
        }
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

BOOL CMeterAccService::Start()
{
    return StartAndStopHandler(TRUE);
}

BOOL CMeterAccService::Stop()
{
    return StartAndStopHandler(FALSE);
}

//////////////////////////////////////////////////
// CMeterAccService::StartDispatcher开始运行Service
// 调用者: _tmain
SERVICE_TABLE_ENTRY CMeterAccService::m_pDispatchTable[] = 
{
    {METER_ACC_SERVICE_NAME, CMeterAccService::ServiceMain},
    { NULL, NULL }
};

BOOL CMeterAccService::StartDispatcher()
{
    return StartServiceCtrlDispatcher(m_pDispatchTable);
}

///////////////////////////////////////////////////////
// CMeterAccService::Main提供ServiceMain回调函数。该函数由
// m_pDispatchTable.lpServiceProc提供给StartDispatcher
void WINAPI CMeterAccService::ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
    g_pServInstance->m_hServiceStatus = RegisterServiceCtrlHandler(
            METER_ACC_SERVICE_NAME, ServiceHandler);

    char szErrMsg[1024];
    if (!g_pServInstance->m_hServiceStatus) 
    {
        _snprintf(szErrMsg, sizeof(szErrMsg), 
                  "Failed to call RegisterServiceCtrlHandler. Error Code = %d", GetLastError());
        szErrMsg[sizeof(szErrMsg) - 1] = '\0';

        ReportEvent(EVENTLOG_ERROR_TYPE, szErrMsg);
        return;
    }

    g_pServInstance->SetServiceState(SERVICE_START_PENDING);
    g_pServInstance->m_hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    g_pServInstance->SetServiceState(SERVICE_RUNNING);

    g_pServInstance->Run();

    g_pServInstance->SetServiceState(SERVICE_STOPPED);

    SetEvent(g_pServInstance->m_hStopEvent);
}

////////////////////////////////////////////////////////////
// CMeterAccService::ServiceHandler提供ServiceHandler回调函数。
// 该函数被作为RegisterServiceCtrlHandler的参数提供给SCM。
// SCM在需要停止该服务时以SERVICE_CONTROL_STOP为参数来调
// 用该函数。函数通过设置m_hStopEvent来导致Run结束。
void WINAPI CMeterAccService::ServiceHandler(DWORD dwControl)
{
    switch (dwControl)
    {
    case SERVICE_CONTROL_STOP:
        g_pServInstance->OnStop();
        if(g_pServInstance->m_hStopEvent != NULL)
        {
            WaitForSingleObject(g_pServInstance->m_hStopEvent, INFINITE);
            CloseHandle(g_pServInstance->m_hStopEvent);
        }
        break;

    default:
        break;
    }
}

//////////////////////////////////////////////////////
// CMeterAccService::SetServiceState向SCM报告本服务的
// 状态，dwState指定现在的状态。
// 调用者: CMeterAccService::ServiceMain
void CMeterAccService::SetServiceState(DWORD dwState)
{
    SERVICE_STATUS status;

    status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    status.dwCurrentState = dwState;
    if (dwState == SERVICE_START_PENDING)
    {
        status.dwControlsAccepted = 0;
    }
    else
    {
        status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    }
    
    status.dwWin32ExitCode = 0;
    status.dwServiceSpecificExitCode = 0;
    status.dwCheckPoint = 0;
    status.dwWaitHint = 0;
    
    SetServiceStatus(g_pServInstance->m_hServiceStatus, &status);
}

///////////////////////////////////////////////////////
// CMeterAccService::ReportEvent向事件记录中增加一条记录
// 事件类型和相应的信息分别由wType和szMsg指定
// 调用者: CMeterAccService::ServiceMain, CMeterAccService::Run
void CMeterAccService::ReportEvent(WORD wType, LPCTSTR szMsg)
{
    HANDLE hEventLog;
    if (hEventLog = RegisterEventSource(
        NULL, METER_ACC_SERVICE_NAME))
    {
        ::ReportEvent(hEventLog, wType, 0, 0, NULL, 1, 0, &szMsg, NULL);
        DeregisterEventSource(hEventLog);
    }
}

////////////////////////////////////////////////////////
// CMeterAccService::Run负责启动并监视进程知道m_hStopEvent
// 被置位。
// 调用者：CMeterAccService::ServiceMain
void CMeterAccService::Run()
{
    STRING strTitle = S_METERACC_MUTEX;
    if((GetVersion() & 0xFF) >= 5)
    {
        strTitle = "Global\\Auto" + strTitle ;
    }
	pMutexAuto = new ACE_Process_Mutex(strTitle.c_str()); 
	//防止进程重入
	if(pMutexAuto->tryacquire())
	{
		return ;
	}	
    OnRun();
}

//////////////////////////////////////////////////////////////////////
// CMeterAccService::StartAndStopHandler负责启动或终止进程
// 调用者: CMeterAccService::Start, CMeterAccService::Stop
BOOL CMeterAccService::StartAndStopHandler(BOOL bStart)
{
    SC_HANDLE hService;
    SC_HANDLE hSCManager;

    char szErrMsg[1024];

    if (!(hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)))
    {
        _snprintf(szErrMsg, sizeof(szErrMsg), 
                  "Failed to call OpenSCManager. Error Code = %d", GetLastError());
        szErrMsg[sizeof(szErrMsg) - 1] = '\0';

        ReportEvent(EVENTLOG_ERROR_TYPE, szErrMsg);
                    
    }
    else if (!(hService = OpenService(hSCManager, METER_ACC_SERVICE_NAME, SERVICE_ALL_ACCESS)))
    {
        _snprintf(szErrMsg, sizeof(szErrMsg), 
                  "Failed to call OpenService. Error Code = %d", GetLastError());
        szErrMsg[sizeof(szErrMsg) - 1] = '\0';

        ReportEvent(EVENTLOG_ERROR_TYPE, szErrMsg);
    }

    BOOL bSuccess = FALSE;
    if (hSCManager && hService)
    {
        if (bStart)
        {
            if (!StartService(hService, 0, NULL))
            {
                _snprintf(szErrMsg, sizeof(szErrMsg), 
                          "Failed to call StartService. Error Code = %d", GetLastError());
                szErrMsg[sizeof(szErrMsg) - 1] = '\0';

                ReportEvent(EVENTLOG_ERROR_TYPE, szErrMsg);
            }
            else
            {
                ReportEvent(EVENTLOG_INFORMATION_TYPE, "Service Started");
                bSuccess = TRUE;
            }
        } else
        {
            SERVICE_STATUS status;
            if (!ControlService(hService, SERVICE_CONTROL_STOP, &status))
            {
                _snprintf(szErrMsg, sizeof(szErrMsg), 
                          "Failed to call ControlService. Error Code = %d", GetLastError());
                szErrMsg[sizeof(szErrMsg) - 1] = '\0';

                ReportEvent(EVENTLOG_ERROR_TYPE, szErrMsg);
            }
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

void CMeterAccService::OnRun()
{
    TRACE(g_szAppName, S_PROCESS_START, S_IGATEWAY_BILL_VER);
	//创建KERNEL进程中的管理者对象，并运行
	CMeterAccMgr *theMgr = new CMeterAccMgr;
	theMgr->Run();
	theMgr->Wait();
	delete theMgr;

	TRACE(g_szAppName, S_PROCESS_STOP);
}

void CMeterAccService::OnStop()
{
    MSG_INTRA *pMsg     = new MSG_INTRA;
    pMsg->SenderPid     = PT_SELF;
    pMsg->SenderMid     = MT_MANAGER;
    pMsg->ReceiverPid   = PT_SELF;
    pMsg->ReceiverMid   = MT_MANAGER;
    pMsg->CmdCode       = CMD_QUIT;

    CMsgProcessor::SendMsg(pMsg);
}

int startServiceMain(int argc, char **const argv)
{
    CMeterAccService::g_pServInstance = new CMeterAccService;

    //服务处理
    if ((argc > 1) && ((*argv[1] == '-') || (*argv[1] == '/')))
    {
        if (stricmp("install", argv[1] + 1) == 0)
        {
            BOOL bRet = CMeterAccService::g_pServInstance->Install();
            if (bRet)
            {
                TRACE(g_szAppName, "Service installed");
            }
        }
        else if (stricmp("remove", argv[1] + 1) == 0)
        {
            BOOL bRet = CMeterAccService::g_pServInstance->Remove();
            if (bRet)
            {
                TRACE(g_szAppName, "Service removed");
            }
        }
        else if (stricmp("start", argv[1] + 1) == 0)
        {
            BOOL bRet = CMeterAccService::g_pServInstance->Start();
            if (bRet)
            {
                TRACE(g_szAppName, "Service started");
            }
        }
        else if (stricmp("stop", argv[1] + 1) == 0)
        {
            BOOL bRet = CMeterAccService::g_pServInstance->Stop();
            if (bRet)
            {
                TRACE(g_szAppName, "Service stopped");
            }
        }
        else if (stricmp("startapp", argv[1] + 1) == 0)
        {
            CMeterAccService::g_pServInstance->StartDispatcher();
        }
        else
        {
            printf("mtr_proc -install    to install the service\n");
            printf("mtr_proc -remove     to remove the service\n");
            printf("mtr_proc -start      to start the service\n");
            printf("mtr_proc -startapp   to start the application only by service\n");
            printf("mtr_proc -stop       to stop the service\n");
        }
    }
    else if(argc == 1)
    {
        //隐藏窗口
        HWND hWnd = GetConsoleWindowHandle();
        if (NULL != hWnd)
        {
            ShowWindow(hWnd, SW_HIDE);
        }

        CMeterAccService::g_pServInstance->Run();
    }
    else
    {
        printf("mtr_proc -install    to install the service\n");
        printf("mtr_proc -remove     to remove the service\n");
        printf("mtr_proc -start      to start the service\n");
        printf("mtr_proc -startapp   to start the application only by service\n");
        printf("mtr_proc -stop       to stop the service\n");
    }

    delete CMeterAccService::g_pServInstance;

    return 0;
}
