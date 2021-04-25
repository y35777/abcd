#include "mtr_manager.h"
#include "../../com/include/resource.h"
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

extern int startServiceMain(int argc, char **const argv);

#endif


int main(int argc, char **const argv)
{
#ifdef _PLATFORM_WIN32

	STRING strTitle = S_METERACC_MUTEX;
    if((GetVersion() & 0xFF) >= 5)
    {
        strTitle = "Global\\" + strTitle;
    }

    ACE_Process_Mutex* pMutex = new ACE_Process_Mutex(strTitle.c_str());
    
	if ((argc == 1) 
		|| ((stricmp("/start", argv[1]) == 0) || (stricmp("-start", argv[1]) == 0))
		|| ((stricmp("/install", argv[1]) == 0) || (stricmp("-install", argv[1]) == 0)))		
    {   		
		//防止进程重入
		if(pMutex->tryacquire())
		{
			delete pMutex;
			return -1;
		}		
	}

	//注册Exception回调
    SetUnhandledExceptionFilter(ExceptionFilter);

#endif

	//赋进程编号
	CMsgProcessor::g_pt = PT_METERACC;

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
	
        strncpy(CMsgProcessor::g_szPtDes, SZ_PROCESS_MTR_DES, 32);
	CMsgProcessor::g_szPtDes[31] = 0;
    
#ifdef _PLATFORM_WIN32
    startServiceMain(argc, argv);
    pMutex->release();
    delete pMutex;
#else //_PLATFORM_WIN32
	TRACE(pAppName, S_PROCESS_START, S_IGATEWAY_BILL_VER);

	//创建KERNEL进程中的管理者对象，并运行
	CMeterAccMgr *theMgr = new CMeterAccMgr;
	theMgr->Run();
	theMgr->Wait();
	delete theMgr;

	TRACE(pAppName, S_PROCESS_STOP);
#endif //_PLATFORM_WIN32

	return 0;
}
