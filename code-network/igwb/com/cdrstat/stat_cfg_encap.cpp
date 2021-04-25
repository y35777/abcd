#include "../include/base_type.h"
#include "stat_cfg_encap.h"
#include "../include/toolbox.h"
#include "../include/resource.h"

CStatCfgEncap::CStatCfgEncap()
{
    m_pDll          = NULL;
}

CStatCfgEncap::~CStatCfgEncap()
{
    if(NULL != m_pDll)
    {
        m_pDll->close();
        delete m_pDll;
        m_pDll = NULL;
    }
}

ICDRStatCfg* CStatCfgEncap::CreateCDRStatCfg()
{
    if(NULL == m_pDll)
    {
        m_pDll = new ACE_DLL;
        int nRet = m_pDll->open(XML_UTIL_DLL_NAME);
        if(0 != nRet)
        {
            delete m_pDll;
            m_pDll = NULL;
            return NULL;
        }
    }

    CREATECDRSTATCFG f = (CREATECDRSTATCFG)m_pDll->symbol("CreateCDRStatCfg");
    if(NULL == f)
    {
        m_pDll->close();
        delete m_pDll;
        m_pDll = NULL;
    }

    return f();
}
