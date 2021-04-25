#include "../../../com/include/toolbox.h"
#include "../cfg_manager.h"
#include "cfg_service.h"

/* 程序名不带路径名和后缀 */
extern IGWB_Svc_Export char g_szAppName[MAX_PATH];

void CCFGService::OnRun()
{
	TRACE(g_szAppName, S_PROCESS_START, S_IGATEWAY_BILL_VER);

	//创建KERNEL进程中的管理者对象，并运行
	CCfgMger *theCfgMgr = new CCfgMger;
	theCfgMgr->Run();
	theCfgMgr->Wait();
	delete theCfgMgr;

	TRACE(g_szAppName, S_PROCESS_STOP);
}

void CCFGService::OnStop()
{
    MSG_INTRA *pMsg = new MSG_INTRA;
    pMsg->SenderPid = PT_SELF;
    pMsg->SenderMid = MT_MANAGER;
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = MT_MANAGER;
    pMsg->CmdCode = CMD_QUIT;
    CMsgProcessor::SendMsg(pMsg);
}

int startServiceMain(int argc, char **const argv)
{
/*
    //初始化MFC
	if (!AfxWinInit(::GetModuleHandle(NULL), 
        NULL, ::GetCommandLine(), 0))
	{
		printf("Fatal Error: MFC initialization failed");
		return 1;
	}
*/
    CServiceBase::g_ServInstance = new CCFGService;

    //服务处理
    if ((argc > 1) &&
        ((*argv[1] == '-') || (*argv[1] == '/')))
    {
        if (stricmp("install", argv[1] + 1) == 0)
        {
            BOOL bRet = CServiceBase::g_ServInstance->Install();
            if(bRet)
            {
                TRACE(g_szAppName, "Service installed");
            }
        }
        else if (stricmp("remove", argv[1] + 1) == 0)
        {
            BOOL bRet = CServiceBase::g_ServInstance->Remove();
            if(bRet)
            {
                TRACE(g_szAppName, "Service removed");
            }
        }
        else if (stricmp("start", argv[1] + 1) == 0)
        {
            BOOL bRet = CServiceBase::g_ServInstance->Start();
            if(bRet)
            {
                TRACE(g_szAppName, "Service started");
            }
        }
        else if (stricmp("stop", argv[1] + 1) == 0)
        {
            BOOL bRet = CServiceBase::g_ServInstance->Stop();
            if(bRet)
            {
                TRACE(g_szAppName, "Service stopped");
            }
        }
        else if (stricmp("startapp", argv[1] + 1) == 0)
        {
            CServiceBase::g_ServInstance->StartDispatcher();
        }
        else
        {
            printf("cfg_proc -install    to install the service\n");
            printf("cfg_proc -remove     to remove the service\n");
            printf("cfg_proc -start      to start the service\n");
            printf("cfg_proc -startapp   to start the app only by service\n");
            printf("cfg_proc -stop       to stop the service\n");
        }
    }
    else if(argc == 1)
    {
        //隐藏窗口
        HWND hWnd = GetConsoleWindowHandle();
        if(NULL != hWnd)
        {
            ShowWindow(hWnd, SW_HIDE);
        }
        CServiceBase::g_ServInstance->Run();
    }
    else
    {
        printf("cfg_proc -install    to install the service\n");
        printf("cfg_proc -remove     to remove the service\n");
        printf("cfg_proc -start      to start the service\n");
        printf("cfg_proc -startapp   to start the app only by service\n");
        printf("cfg_proc -stop       to stop the service\n");
    }

    delete CServiceBase::g_ServInstance;

    return 0;
}

