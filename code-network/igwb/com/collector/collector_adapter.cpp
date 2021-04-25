#include "../include/frame.h"
#include "../include/toolbox.h"
#include "collector_adapter.h"

char CCollectorAdapter::g_szFrontSaveRoot[MAX_PATH] = "";
char CCollectorAdapter::g_szBackSaveRoot[MAX_PATH]  = "";
char CCollectorAdapter::g_szSecondBackSaveRoot[MAX_PATH]  = "";
char CCollectorAdapter::g_szAPName[100]             = "";
int  CCollectorAdapter::g_nAPSaveSecond             = -1;
int  CCollectorAdapter::g_nNeedSecondAdapter        = -1;

void CCollectorAdapter::Trace(const char* szFmt, ...)
{
    char buf[1024];
    char* pBuf = buf;

    va_list argptr;

    va_start(argptr, szFmt);
    VSNPRINTF(pBuf,1024,szFmt,argptr);
    buf[1023] = 0;

    va_end(argptr);

    TRACE(MTS_COLLECTOR, buf);
}

void CCollectorAdapter::TraceAssert(const char* szSourceFile,
                                    const int nLineNum)
{
    ::TraceAssert(TRUE, szSourceFile, nLineNum);
}

void CCollectorAdapter::MessageOut(const BYTE nMsgOutLevel, const char* szFmt, ...)
{
    char buf[1024];
    char* pBuf = buf;

    va_list argptr;

    va_start(argptr, szFmt);
    VSNPRINTF(pBuf,1024,szFmt,argptr);
    buf[1023] = 0;

    va_end(argptr);

    MSGOUT(MTS_COLLECTOR, nMsgOutLevel, buf);
}

void CCollectorAdapter::WriteLog(const char* szSysLog,
                                 BYTE bLogResult)
{
    CMsgProcessor::WriteLog(MTS_COLLECTOR, szSysLog, bLogResult);
}

void CCollectorAdapter::WriteLog(const SLog* pLog)
{
    CMsgProcessor::WriteLog(pLog);
}

/* 向双Agent模块发送倒换命令 */
void CCollectorAdapter::AskForFailOver(const BYTE nCause,
                                       const void* pData,
                                       const UINT4 nLen)
{
    CMsgProcessor::AskForFailOver(MT_COLLECTOR, nCause, pData, nLen);
}

/* 向告警模块发送告警消息 */
void CCollectorAdapter::SendAlarm(const SInnerAlarm* pAlarm)
{
    CMsgProcessor::SendAlarm(pAlarm);
}

const char* CCollectorAdapter::GetAppPath(void)
{
    return ::GetAppPath();
}

const char* CCollectorAdapter::GetAPName(void)
{
    if(g_szAPName[0] == '\0')
    {
        CINIFile ini(GetCfgFilePath());
        ini.Open();
        char szSec[20];
        sprintf(szSec, "%s%d", CFG_SEC_AP, GetProcessType() - PT_CO_BASE + 1);

        ini.GetString(szSec,
                      CFG_AP_NAME,
                      szSec,
                      g_szAPName,
                      sizeof(g_szAPName));
    }

    return g_szAPName;
}

int CCollectorAdapter::GetProcessType(void)
{
    return (int)CMsgProcessor::GetProcessType();
}

int CCollectorAdapter::GetFtpPasvMode(void)
{
	CINIFile ini(GetCfgFilePath());
	ini.Open();
	char szSec[20];
	sprintf(szSec, "%s%d", CFG_SEC_AP, GetProcessType() - PT_CO_BASE + 1);
	int nFtpMode = ini.GetInt(CFG_SEC_COMMON,
							  CFG_NET_BACKUP_FTP_IS_PASSIVE,
							  0);
	nFtpMode = ini.GetInt(szSec,
						  CFG_NET_BACKUP_FTP_IS_PASSIVE,
						  nFtpMode);
	return nFtpMode;
}

const char* CCollectorAdapter::GetCfgFilePath(void)
{
    return ::GetCfgFilePath();
}

const char* CCollectorAdapter::GetFrontSavePath(void)
{
    if(g_szFrontSaveRoot[0] == '\0')
    {
        CINIFile ini(GetCfgFilePath());
        ini.Open();
        ini.GetString(CFG_SEC_DISK_FILE,
                      CFG_FRONT_SAVE_ROOT_DIR,
                      CFG_FRONT_SAVE_ROOT_DIR_DEFAULT,
                      g_szFrontSaveRoot,
                      sizeof(g_szFrontSaveRoot));

        SNPRINTF(g_szFrontSaveRoot, 
                 sizeof(g_szFrontSaveRoot),
                 "%s/%s",
                 g_szFrontSaveRoot,
                 GetAPName());
        g_szFrontSaveRoot[sizeof(g_szFrontSaveRoot) - 1] = '\0';
    }

    return g_szFrontSaveRoot;
}

const char* CCollectorAdapter::GetBackSavePath(void)
{
    if(g_szBackSaveRoot[0] == '\0')
    {
        CINIFile ini(GetCfgFilePath());
        ini.Open();
        ini.GetString(CFG_SEC_DISK_FILE,
                      CFG_BACK_SAVE_ROOT_DIR,
                      CFG_BACK_SAVE_ROOT_DIR_DEFAULT,
                      g_szBackSaveRoot,
                      sizeof(g_szBackSaveRoot));

        SNPRINTF(g_szBackSaveRoot, 
                 sizeof(g_szBackSaveRoot),
                 "%s/%s",
                 g_szBackSaveRoot,
                 GetAPName());
        g_szBackSaveRoot[sizeof(g_szBackSaveRoot) - 1] = '\0';
    }

    return g_szBackSaveRoot;
}

const char* CCollectorAdapter::GetSecondBackSavePath(void)
{
    if(g_szSecondBackSaveRoot[0] == '\0')
    {
        CINIFile ini(GetCfgFilePath());
        ini.Open();
        ini.GetString(CFG_SEC_DISK_FILE,
                      CFG_BACK_SAVE_ROOT_DIR,
                      CFG_BACK_SAVE_ROOT_DIR_DEFAULT,
                      g_szSecondBackSaveRoot,
                      sizeof(g_szSecondBackSaveRoot));

        SNPRINTF(g_szSecondBackSaveRoot, 
                 sizeof(g_szSecondBackSaveRoot),
                 "%s/Second/%s",
                 g_szSecondBackSaveRoot,
                 GetAPName());
        g_szSecondBackSaveRoot[sizeof(g_szSecondBackSaveRoot) - 1] = '\0';
    }

    //modifyed by chenliangwei, 2003-08-05
    return g_szSecondBackSaveRoot;
    //return g_szBackSaveRoot;
    //modifyed end
}

BOOL CCollectorAdapter::GetbAPSaveSecond(void)
{
    if(g_nAPSaveSecond == -1)
    {
        CINIFile ini(GetCfgFilePath());
        ini.Open();

        g_nAPSaveSecond = ini.GetInt(CFG_SEC_DISK_FILE, 
                                     CFG_SAVE_SECOND, 
                                     CFG_SAVE_SECOND_DEFAULT);
        char szAPSection[25];
        
        //modifyed by chenliangwei, 2003-08-05
        //sprintf(szAPSection, "%s%d", CFG_SEC_AP, GetProcessType());
        sprintf(szAPSection, "%s%d", CFG_SEC_AP, GetProcessType() - PT_CO_BASE + 1);
        //modifyed end

        g_nAPSaveSecond = ini.GetInt(szAPSection,
                                     CFG_SAVE_SECOND, 
                                     g_nAPSaveSecond);
    }

    return g_nAPSaveSecond;
}

BOOL CCollectorAdapter::GetbNeedSencondAdapter(void)
{
    if(g_nNeedSecondAdapter == -1)
    {
        CINIFile ini(GetCfgFilePath());
        ini.Open();

  	    char szPatchName[MAX_PATH] = "";      
        ini.GetString(CFG_SEC_DISK_FILE,
                      CFG_BACK_SAVE_PATCH_NAME,
                      CFG_BACK_SAVE_PATCH_NAME_DEFAULT,
                      szPatchName,
                      sizeof(szPatchName));

        char szAPSection[25];
        
        sprintf(szAPSection, "%s%d", CFG_SEC_AP, GetProcessType() - PT_CO_BASE + 1);

        ini.GetString(szAPSection,
                      CFG_BACK_SAVE_PATCH_NAME,
                      szPatchName,
                      szPatchName,
                      sizeof(szPatchName));

        if(szPatchName[0] != '\0')
        {
            g_nNeedSecondAdapter = 1;
        }
        else
        {
            g_nNeedSecondAdapter = 0;
        }

    }

    return g_nNeedSecondAdapter;
}

