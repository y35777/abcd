#include "../include/base_type.h"
#include "stat_rst_encap.h"
#include "../include/toolbox.h"
#include "../include/resource.h"

CStatRstEncap::CStatRstEncap()
{
    m_pDll          = NULL;
}

CStatRstEncap::~CStatRstEncap()
{
    if(NULL != m_pDll)
    {
        m_pDll->close();
        delete m_pDll;
        m_pDll = NULL;
    }
}

ICDRStatRst* CStatRstEncap::CreateCDRStatRst()
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

    CREATECDRSTATRST f = (CREATECDRSTATRST)m_pDll->symbol("CreateCDRStatRst");
    if(NULL == f)
    {
        m_pDll->close();
        delete m_pDll;
        m_pDll = NULL;
    }

    return f();
}
