/////////////////////////////////////////////////////////////////////
// PWD.cpp  -- ���̼��ӷ���
// chenghui, 1999.7.2
// ԭPWD.cpp��1999.4��ɣ�������Ҫʹ��MFC���ܣ�������ʹ��VC��
// AppWizard���ɳ����ܣ�����ԭPWD.cpp����Ҫ���뿽�������ļ�
// �С����������޸ģ�
// ��һ��������n����֮���ظ�������m��֮�󣬲��ٳ��������ó���
// ����������һ���쳣�������
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// ���´���ΪVC�����ɵĿ�ܴ���
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
// ���´���ΪCServiceBase��ʵ�֣�CServiceBase����ʱ������������
// ���ɸ�CProcess��һ������ĳһ��CProcess��������������������
// ��CProcess��������ڹ涨ʱ��������CProcess�����涨��������
// ��������������CProcess�������ø�CProcess��OnException����
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
// CServiceBase::Install��װPWD����
// ������: _tmain
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

    //����÷����Ѿ����� ZhengYuqun 2004-06-14 SWPD05961
    if (hService = OpenService(hSCManager,
        PWD_SERVICE_NAME, SERVICE_ALL_ACCESS))
    {
        //��ֹͣ��Ȼ�����·��Ϊ��·���������������Ϊ�Զ�����
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
// CServiceBase::Removeɾ��PWD����
// ������: _tmain
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
// CServiceBase::StartDispatcher��ʼ����Service
// ������: _tmain
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
// CServiceBase::Main�ṩServiceMain�ص��������ú�����
// m_pDispatchTable.lpServiceProc�ṩ��StartDispatcher
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
// CServiceBase::ServiceHandler�ṩServiceHandler�ص�������
// �ú�������ΪRegisterServiceCtrlHandler�Ĳ����ṩ��SCM��
// SCM����Ҫֹͣ�÷���ʱ��SERVICE_CONTROL_STOPΪ��������
// �øú���������ͨ������m_hStopEvent������Run������
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
// CServiceBase::SetServiceState��SCM���汾�����
// ״̬��dwStateָ�����ڵ�״̬��
// ������: CServiceBase::ServiceMain
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
// CServiceBase::ReportEvent���¼���¼������һ����¼
// �¼����ͺ���Ӧ����Ϣ�ֱ���wType��szMsgָ��
// ������: CServiceBase::ServiceMain, CServiceBase::Run
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
// CServiceBase::Run�������������ӽ���֪��m_hStopEvent
// ����λ��
// �����ߣ�CServiceBase::ServiceMain
void CServiceBase::Run()
{
    OnRun();
}

//////////////////////////////////////////////////////////////////////
// CServiceBase::StartAndStopHandler������������ֹ����
// ������: CServiceBase::Start, CServiceBase::Stop
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
