#include "../include/frame.h"
#include "cluster_toolbox.h"

#ifdef _PLATFORM_WIN32
int g_nOSVersion = GetVersion() & 0xFF;
#else
int g_nOSVersion == INVALID_OS_VERSION;
#endif //_PLATFORM_WIN32

int ExecuteProcess(const char* const szExeName, UINT4* pExitCode)
{
    ACE_Process_Options POptions; 
    POptions.command_line("%s", szExeName);

    ACE_Process_Manager* pProcMgr = ACE_Process_Manager::instance();

    int nPid = pProcMgr->spawn(POptions);
    if(ACE_INVALID_PID == nPid)
    {
        return ERR_FAIL;
    }

    return pProcMgr->wait(nPid, pExitCode) - nPid;
}

int ExecuteProcess(const char* const szExeName)
{
    UINT4 nExitCode;

    if((ExecuteProcess(szExeName, &nExitCode) == ERR_SUCCESS) && 
       (nExitCode == 0))
    {
        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

////////////////////////////////////////////////////////////
// Reboot -- 重新启动NT
// 返回值：
//     true  -- 成功。
//     false -- 失败。原因可能为：用户权限不够；有程序不能响应退出命令。
BOOL Reboot(UINT uFlags)
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    // Get a token for this process.
    if (!OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        return FALSE;
    }
    // Get the LUID for the shutdown privilege.
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
        &tkp.Privileges[0].Luid);

    tkp.PrivilegeCount = 1;  // one privilege to set
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // Get the shutdown privilege for this process.

    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
        (PTOKEN_PRIVILEGES)NULL, 0);

    // Cannot test the return value of AdjustTokenPrivileges.
    if (GetLastError() != ERROR_SUCCESS)
    {
        return FALSE;
        //error("AdjustTokenPrivileges");
    }

    // Shut down the system and force all applications to close.

    if (!ExitWindowsEx(EWX_REBOOT|uFlags, 0))
    {
        return FALSE;
        //error("ExitWindowsEx");
    }
    return TRUE;
}

