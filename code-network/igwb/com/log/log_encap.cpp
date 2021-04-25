#include "../include/base_type.h"
#include "log_encap.h"
#include "../include/toolbox.h"
#include "../include/resource.h"

CLogUtilEncap::CLogUtilEncap()
{
    m_pDll          = NULL;
}

CLogUtilEncap::~CLogUtilEncap()
{
    if(NULL != m_pDll)
    {
        m_pDll->close();
        delete m_pDll;
        m_pDll = NULL;
    }
}

ILogUtil* CLogUtilEncap::CreateLogUtil()
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

    CREATELOGUTIL f = (CREATELOGUTIL)m_pDll->symbol("CreateLogUtil");
    if(NULL == f)
    {
        m_pDll->close();
        delete m_pDll;
        m_pDll = NULL;
    }
   
    return f();
}
