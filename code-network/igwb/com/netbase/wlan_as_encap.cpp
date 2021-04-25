#include "../include/base_type.h"
#include "wlan_as_encap.h"
#include "../include/toolbox.h"
#include "../include/resource.h"

CWlanUtilEncap::CWlanUtilEncap()
{
    m_pDll = NULL;
}

CWlanUtilEncap::~CWlanUtilEncap()
{
    if(NULL != m_pDll)
    {
        m_pDll->close();
        delete m_pDll;
        m_pDll = NULL;
    }
}

IWlanAsOci* CWlanUtilEncap::CreateWlanAsOci(const char* szFmtDll)
{
    char szFmtDllPath[MAX_PATH];
#ifdef _PLATFORM_WIN32
    SNPRINTF(szFmtDllPath, sizeof(szFmtDllPath),
             "%s.dll", szFmtDll);
#elif defined(_PLATFORM_SOLARIS) || defined (_PLATFORM_LINUX) 
    SNPRINTF(szFmtDllPath, sizeof(szFmtDllPath),
             "lib%s.so", szFmtDll);
#elif defined(_PLATFORM_HPUX)
    SNPRINTF(szFmtDllPath, sizeof(szFmtDllPath),
             "lib%s.sl", szFmtDll);
#else
        #error macro _PLATFORM_??? undefined.
#endif
    szFmtDllPath[sizeof(szFmtDllPath) - 1] = '\0';
    if(NULL == m_pDll)
    {
        m_pDll = new ACE_DLL;
        int nRet = m_pDll->open(szFmtDllPath);
        if(0 != nRet)
        {
            delete m_pDll;
            m_pDll = NULL;
            return NULL;
        }
    }

    CREATEWLANASOCI f = (CREATEWLANASOCI)m_pDll->symbol("CreateWlanAsOci");
    if(NULL == f)
    {
        m_pDll->close();
        delete m_pDll;
        m_pDll = NULL;
    }
   
    return f();
}
