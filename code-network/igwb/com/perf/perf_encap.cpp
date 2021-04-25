#include "../include/base_type.h"
#include "perf_encap.h"
#include "../include/toolbox.h"
#include "../include/resource.h"

CPerfUtilEncap::CPerfUtilEncap()
{
    m_pDll          = NULL;
}

CPerfUtilEncap::~CPerfUtilEncap()
{
    if(NULL != m_pDll)
    {
        m_pDll->close();
        delete m_pDll;
        m_pDll = NULL;
    }
}

IPerfUtil* CPerfUtilEncap::CreatePerfUtil()
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

    CREATEPERFUTIL f = (CREATEPERFUTIL)m_pDll->symbol("CreatePerfUtil");
    if(NULL == f)
    {
        m_pDll->close();
        delete m_pDll;
        m_pDll = NULL;
    }
   
    return f();
}
