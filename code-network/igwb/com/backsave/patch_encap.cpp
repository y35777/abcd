#include "../include/base_type.h"
#include "patch_encap.h"
#include "../include/toolbox.h"


CPatchEncap::CPatchEncap()
{
    m_pDll          = NULL;

    m_pInitFun      = NULL;
    m_pProcessFun   = NULL;
    m_pTerminateFun = NULL;
    m_bAviable      = FALSE;

	m_pCanCloseFun	= FALSE;

    m_pResetRemainTicket_WLAN = NULL;
    m_pIsFileOutPut_WLAN      = NULL;
    m_pGetAEmptyFilePara      = NULL;

}

CPatchEncap::~CPatchEncap()
{
    if(NULL != m_pDll)
    {
        m_pDll->close();
        delete m_pDll;
        m_pDll = NULL;
    }
}

int CPatchEncap::Init(int nAPId)
{
    //打开配置文件
    CINIFile Ini(GetCfgFilePath());
    Ini.Open();

    char szFmtDll[100] = "";

    Ini.GetString(CFG_SEC_DISK_FILE,
                  CFG_BACK_SAVE_PATCH_NAME,
                  CFG_BACK_SAVE_PATCH_NAME_DEFAULT,
                  szFmtDll,
                  sizeof(szFmtDll));
    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP, nAPId);
    Ini.GetString(strSec,
                  CFG_BACK_SAVE_PATCH_NAME,
                  szFmtDll,
                  szFmtDll,
                  sizeof(szFmtDll));
    if(szFmtDll[0] == '\0')
    {
        //不需要后存盘适配功能
        return 0;
    }

    //保存后存盘补丁动态库名称
    strncpy(m_szDllName, szFmtDll, sizeof(szFmtDll));    

#ifdef _PLATFORM_WIN32
    strncat(szFmtDll, ".dll", sizeof(szFmtDll) - strlen(szFmtDll));
#elif defined(_PLATFORM_SOLARIS) || defined (_PLATFORM_LINUX) 
    STRING strTemp = szFmtDll;
    SNPRINTF(szFmtDll, sizeof(szFmtDll),
             "lib%s.so", strTemp.c_str());
#elif defined(_PLATFORM_HPUX)
    STRING strTemp = szFmtDll;
    SNPRINTF(szFmtDll, sizeof(szFmtDll),
             "lib%s.sl", strTemp.c_str());
#else
    #error macro _PLATFORM_??? undefined.
#endif
    szFmtDll[sizeof(szFmtDll) - 1] = '\0';

    if(NULL == m_pDll)
    {
        m_pDll = new ACE_DLL;
        int nRet = m_pDll->open(szFmtDll);
        if(0 != nRet)
        {
            delete m_pDll;
            m_pDll = NULL;
            return -1;
        }
    }

    m_pInitFun = (PC_INIT)m_pDll->symbol("PC_Init");
    if(NULL == m_pInitFun)
    {
        m_pDll->close();
        delete m_pDll;
        m_pDll = NULL;
        m_pInitFun      = NULL;
        m_pProcessFun   = NULL;
        m_pTerminateFun = NULL;
		m_pCanCloseFun  = NULL;
        return -1;
    }

    m_pProcessFun = (PC_PROCESS)m_pDll->symbol("PC_Process");
    if(NULL == m_pProcessFun)
    {
        m_pDll->close();
        delete m_pDll;
        m_pDll = NULL;
        m_pInitFun      = NULL;
        m_pProcessFun   = NULL;
        m_pTerminateFun = NULL;
		m_pCanCloseFun  = NULL;		
        return -1;
    }

    m_pTerminateFun = (PC_TERMINATE)m_pDll->symbol("PC_Terminate");
    if(NULL == m_pTerminateFun)
    {
        m_pDll->close();
        delete m_pDll;
        m_pDll = NULL;
        m_pInitFun      = NULL;
        m_pProcessFun   = NULL;
        m_pTerminateFun = NULL;
		m_pCanCloseFun  = NULL;		
        return -1;
    }
	
	//by 杨善	2004-6-1	对应问题单SWPD05564	
	m_pCanCloseFun = (PC_CANCLOSE)m_pDll->symbol("PC_CanClose");
	//the end	对应问题单SWPD05564
	
    m_bAviable = TRUE;
    
    //added by lucy 2004-6-11 wlan backsave patch
    if(strcmp(WLAN_BACKSAVE_DLL_NAME, m_szDllName) == 0)
    {
        m_pResetRemainTicket_WLAN = (PC_RESETREMAINTICKET_WLAN)m_pDll->symbol("PC_ResetRemainTicket_WLAN");
        m_pIsFileOutPut_WLAN      = (PC_ISFILEOUTPUT_WLAN)m_pDll->symbol("PC_IsFileOutPut_WLAN");
        m_pGetAEmptyFilePara      = (PC_GETAEMPTYFILEPARA)m_pDll->symbol("PC_GetAEmptyFilePara");
        m_pCreateAEmptyFile       = (PC_CREATEAEMPTYFILE)m_pDll->symbol("PC_CreateAEmptyFile");

        if((NULL == m_pResetRemainTicket_WLAN) ||
           (NULL == m_pIsFileOutPut_WLAN)||
           (NULL == m_pGetAEmptyFilePara)||
           (NULL == m_pCreateAEmptyFile))
        {
            m_pDll->close();
            delete m_pDll;
            m_pDll          = NULL;
            m_pInitFun      = NULL;
            m_pProcessFun   = NULL;
            m_pTerminateFun = NULL;
            m_pCanCloseFun  = NULL;		
            m_pResetRemainTicket_WLAN = NULL;
            m_pIsFileOutPut_WLAN      = NULL;
            m_pGetAEmptyFilePara      = NULL;
            m_pCreateAEmptyFile       = NULL;
            return -1;
        }

    }
    //end add 
        
    return 0;
}

BOOL CPatchEncap::Init(const char* szConfigPath, BYTE nApID, UINT4 nChannelCount)
{
    if(NULL != m_pInitFun)
    {
        return (*m_pInitFun)(szConfigPath, nApID, nChannelCount);
    }

    return FALSE;
}

BOOL CPatchEncap::Process(const char* szFileName, char*& szDestFileName, UINT4 nChannelNum)
{
    if(NULL != m_pProcessFun)
    {
        return (*m_pProcessFun)(szFileName, szDestFileName, nChannelNum);
    }

    return FALSE;
}

BOOL CPatchEncap::Terminate()
{
    if(NULL != m_pTerminateFun)
    {
        return (*m_pTerminateFun)();
    }

    return FALSE;
}

//by 杨善	2004-6-1	对应问题单SWPD05564
BOOL CPatchEncap::CanClose(const char* szFileName)
{
    if(NULL != m_pCanCloseFun)
    {
        return (*m_pCanCloseFun)(szFileName);
    }
	
    return TRUE;
}
//the end	对应问题单SWPD05564


//added by lucy 2004-6-11 wlan backsave patch
STRING CPatchEncap::GetDllName()
{
    return  (STRING)m_szDllName;    
}

void CPatchEncap::ResetRemainTicket_WLAN()
{
    if(NULL != m_pResetRemainTicket_WLAN)
    {
        (*m_pResetRemainTicket_WLAN)();
    }
}

BOOL CPatchEncap::IsFileOutPut_WLAN(UINT4 nChannelID)
{
    if(NULL != m_pIsFileOutPut_WLAN)
    {
        return (*m_pIsFileOutPut_WLAN)(nChannelID);
    }
    
    return FALSE;    
}

void CPatchEncap::GetAEmptyFilePara(char *szFileName, char *szFileSuffix, UINT4 nChannelID)
{
    if(NULL != m_pGetAEmptyFilePara)
    {
        (*m_pGetAEmptyFilePara)(szFileName, szFileSuffix, nChannelID);  
    }
}

BOOL CPatchEncap::CreateAEmptyFile(char *strFileName, char *strFileSuffix)
{
    if(NULL != m_pCreateAEmptyFile) 
    {
        return (*m_pCreateAEmptyFile)(strFileName, strFileSuffix);
    }
    return FALSE;
}
//end add
