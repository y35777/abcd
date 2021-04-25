#include "../include/base_type.h"
#include "alarm_encap.h"
#include "../include/toolbox.h"
#include "../include/resource.h"

CAlarmUtilEncap::CAlarmUtilEncap()
{
    m_pDll          = NULL;
}

CAlarmUtilEncap::~CAlarmUtilEncap()
{
    if(NULL != m_pDll)
    {
        m_pDll->close();
        delete m_pDll;
        m_pDll = NULL;
    }
}

IAlarmUtil* CAlarmUtilEncap::CreateAlarmUtil()
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

    CREATEALARMUTIL f = (CREATEALARMUTIL)m_pDll->symbol("CreateAlarmUtil");
    if(NULL == f)
    {
        m_pDll->close();
        delete m_pDll;
        m_pDll = NULL;
    }
   
    return f();
}
