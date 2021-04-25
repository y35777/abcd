#include "../include/toolbox.h"
#include "../include/export_dll.h"
#include "field_filter.h"

#define S_CDR_FILTER_FILE   "/config/cdr_filter/cdr_filter.ini"

CCDRFieldFilter* CCDRFieldFilter::g_pInstance = NULL;
#ifdef _FILTER_MULTITHREAD
ACE_Mutex CCDRFieldFilter::g_Mutex;
#endif //_FILTER_MULTITHREAD

CCDRFieldFilter::CCDRFieldFilter()
{
    STRING strFileName = GetAppPath();
    strFileName       += S_CDR_FILTER_FILE;
    m_nValidDefault    = TRUE;

    m_IniFile = new CINIFile(strFileName.c_str());
    if(m_IniFile != NULL)
    {
        m_IniFile->Open();
    }
}

CCDRFieldFilter::CCDRFieldFilter(const char* szFileName)
{
    m_IniFile = new CINIFile(szFileName);
    if(m_IniFile != NULL)
    {
        m_IniFile->Open();
    }
}

CCDRFieldFilter::~CCDRFieldFilter()
{
    if(NULL != m_IniFile)
    {
        delete m_IniFile;
        m_IniFile = NULL;
    }
}

CCDRFieldFilter* CCDRFieldFilter::GetInstance()
{
#ifdef _FILTER_MULTITHREAD
    g_Mutex.acquire();
#endif //_FILTER_MULTITHREAD

    if(g_pInstance == NULL)
    {
        g_pInstance = new CCDRFieldFilter;
    }

#ifdef _FILTER_MULTITHREAD
    g_Mutex.release();
#endif //_FILTER_MULTITHREAD

    return g_pInstance;
}

CCDRFieldFilter* CCDRFieldFilter::GetInstance(const char* szFileName)
{
#ifdef _FILTER_MULTITHREAD
    g_Mutex.acquire();
#endif //_FILTER_MULTITHREAD

    if(g_pInstance == NULL)
    {
        g_pInstance = new CCDRFieldFilter(szFileName);
    }

#ifdef _FILTER_MULTITHREAD
    g_Mutex.release();
#endif //_FILTER_MULTITHREAD

    return g_pInstance;
}

void CCDRFieldFilter::Destroy()
{
#ifdef _FILTER_MULTITHREAD
    g_Mutex.acquire();
#endif //_FILTER_MULTITHREAD

    if(g_pInstance != NULL)
    {
        delete g_pInstance;
        g_pInstance = NULL;
    }

#ifdef _FILTER_MULTITHREAD
    g_Mutex.release();
#endif //_FILTER_MULTITHREAD
}

void CCDRFieldFilter::SetValidDefault(int nDefault)
{
    m_nValidDefault = nDefault;
}

BOOL CCDRFieldFilter::GetField(const char* szBillType, const char* szFieldName)
{
#ifdef _FILTER_MULTITHREAD
    m_Mutex.acquire();
#endif //_FILTER_MULTITHREAD

    int nRet      = FALSE;
    STRING strKey = szBillType;
    strKey       += szFieldName;
    MAP<STRING, BOOL>::iterator iter = m_Key2ValueMap.find(strKey);
    if(iter != m_Key2ValueMap.end())
    {
        nRet = iter->second;
    }
    else
    {
        if(m_IniFile != NULL)
        {
            nRet = m_IniFile->GetInt(szBillType, szFieldName, m_nValidDefault);
        }

        m_Key2ValueMap[strKey] = nRet;
    }

#ifdef _FILTER_MULTITHREAD
    m_Mutex.release();
#endif //_FILTER_MULTITHREAD

    return nRet;
}

