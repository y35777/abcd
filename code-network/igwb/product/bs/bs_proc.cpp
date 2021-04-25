///////////////////////////////////////////////////////////////////////////////
//GTP'计费接口增加 Z30971 2004/06
/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:bs_proc.cpp
    描述信息:
    
    本文件中定义了计费接口的main()函数.

    版本说明:V200R002i00M00B00D1130A
    版权信息:版权所有(C)2004-2005 华为技术有限公司

    修改记录:
    Z30971,2001-11-12,创建.
*/

#include "bs_manager.h"
#include "../../com/include/toolbox.h"

/* 主目录路径名,如:"c:\\billsvr"、"."或"" */
extern IGWB_Svc_Export char g_szAppPath[MAX_PATH];

/* 配置文件绝对路径名,如:"c:\\billsvr\\ini\\billsvr.ini" */
extern IGWB_Svc_Export char g_szCfgPathName[MAX_PATH];

/* 程序名不带路径名和后缀 */
extern IGWB_Svc_Export char g_szAppName[MAX_PATH];

#ifdef _PLATFORM_WIN32
LONG WINAPI ExceptionFilter(PEXCEPTION_POINTERS pException)
{
	TraceNTException(pException);

    _exit(1);
    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

int main(int argc, char **const argv)
{
#ifdef _PLATFORM_WIN32

    ACE_Process_Mutex* pMutex = new ACE_Process_Mutex(S_BS_MUTEX);
    
    //防止进程重入
    if(pMutex->tryacquire())
    {
        delete pMutex;
        return -1;
    }

    //隐藏窗口
    HWND hWnd = GetConsoleWindowHandle();
    if(NULL != hWnd)
    {
        ShowWindow(hWnd, SW_HIDE);
    }

	//注册Exception回调
    SetUnhandledExceptionFilter(ExceptionFilter);

#endif

	//赋进程编号
	CMsgProcessor::g_pt = PT_BS;

	//赋程序文件名
	char *pAppName = (char *)ACE::basename(argv[0]);
	if (char *pDot = strchr(pAppName, '.')) *pDot = '\0';
	strcpy(g_szAppName, pAppName);
	
	//赋程序路径名
	char *pPathName = (char *)ACE::dirname(argv[0]);
	strcpy(g_szAppPath, pPathName);

	//赋配置文件绝对路径名
	strcpy(g_szCfgPathName, pPathName);
	strcat(g_szCfgPathName, ACE_DIRECTORY_SEPARATOR_STR);
	strcat(g_szCfgPathName, S_CFG_FILE_NAME);

	//打开TRACE文件
	char szTraceName[MAX_PATH];
	strcpy(szTraceName, g_szAppPath);
	strcat(szTraceName, ACE_DIRECTORY_SEPARATOR_STR);
	strcat(szTraceName, "trace");

	if(!CreateDeepDir(szTraceName)) return -1;

	strcat(szTraceName, ACE_DIRECTORY_SEPARATOR_STR);
	strcat(szTraceName, g_szAppName);
	strcat(szTraceName, "_trace.txt");
	CreateTrace(szTraceName);
    
	//赋进程描述 
    strncpy(CMsgProcessor::g_szPtDes,SZ_PROCESS_BS_DES,32);
	CMsgProcessor::g_szPtDes[31] = 0;
    
	TRACE(pAppName, S_PROCESS_START, S_IGATEWAY_BILL_VER);

	//创建BS进程中的管理者对象，并运行
	CBSMger *theBSMgr = new CBSMger;
	theBSMgr->Run();
	theBSMgr->Wait();
	delete theBSMgr;

	TRACE(pAppName, S_PROCESS_STOP);
#ifdef _PLATFORM_WIN32
    
    pMutex->release();
    delete pMutex;

#endif //_PLATFORM_WIN32
	return 0;
}

