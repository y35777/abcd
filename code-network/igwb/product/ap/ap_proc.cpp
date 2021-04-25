/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:ap_proc.cpp
    描述信息:
    
    本文件中定义了接入点进程的main()函数.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    王锋,2001-11-14,创建.
*/

#include "ap_manager.h"
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
    //赋接入点进程号
    if (argc < 2) return 0;
    int pt = atoi(argv[1]) + PT_AP_BASE - 1;
    CMsgProcessor::g_pt = (PROCESS_TYPE)pt;
    //接入点名称
    sprintf(CMsgProcessor::g_szPt,"%s%d",PTS_AP_BASE,
        (pt - PT_AP_BASE + 1));

#ifdef _PLATFORM_WIN32

    char szTitle[100];
    sprintf(szTitle, "%s_%s", S_AP_MUTEX, argv[1]);

	STRING strTitle = szTitle;
    if((GetVersion() & 0xFF) >= 5)
    {
        strTitle = "Global\\" + strTitle;
    }

    ACE_Process_Mutex* pMutex = new ACE_Process_Mutex(strTitle.c_str());

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
    strcat(szTraceName, argv[1]);
    strcat(szTraceName, "_trace.txt");
	CreateTrace(szTraceName);

    TRACE(pAppName, S_PROCESS_START, S_IGATEWAY_BILL_VER);
    //赋进程描述 张玉新2002-05-20 问题单D13657
    CINIFile *pIniFile = new CINIFile(GetCfgFilePath());
    int nRet = pIniFile->Open(); 
    if(nRet != TRUE)
    {
        TRACE(pAppName, S_PROCESS_OPEN_INIFAIL);
		delete pIniFile;
        return ERR_FAIL;
    }
    char szAPTmp[32];
    sprintf(szAPTmp, "%s%s", CFG_SEC_AP, argv[1]);      //AccessPointX
    pIniFile->GetString(szAPTmp,              
                      CFG_AP_NAME,                      //APName
                      "3G",
                      CMsgProcessor::g_szPtDes,
                      32);
    delete pIniFile;
    //修改结束
    //创建KERNEL进程中的管理者对象，并运行
    CAPMgr *theAPMgr = new CAPMgr;
    theAPMgr->Run();
    theAPMgr->Wait();
    delete theAPMgr;

    TRACE(pAppName, S_PROCESS_STOP);

#ifdef _PLATFORM_WIN32
    
    pMutex->release();
    delete pMutex;

#endif //_PLATFORM_WIN32

    return 0;
}
