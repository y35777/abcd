#ifndef __FIELD_FILTER_H__
#define __FIELD_FILTER_H__

#include "ini_file.h"

/*
    如果要以多线程方式使用本类，请打开下面的定义
*/
//#define _FILTER_MULTITHREAD

class IGWB_Svc_Export CCDRFieldFilter
{
public:
    CCDRFieldFilter();
    CCDRFieldFilter(const char* szFileName);
    ~CCDRFieldFilter();

    BOOL GetField(const char* szBillType, const char* szFieldName);
    void SetValidDefault(int nDefault);

    static CCDRFieldFilter* GetInstance();
    static CCDRFieldFilter* GetInstance(const char* szFileName);

    static void Destroy();

protected:
    static CCDRFieldFilter* g_pInstance;

    CINIFile*               m_IniFile;

    MAP<STRING, BOOL>       m_Key2ValueMap;

    int                     m_nValidDefault;
#ifdef _FILTER_MULTITHREAD

    //保护m_IniFile
    ACE_Mutex               m_Mutex;

    //保护全局对象
    static ACE_Mutex        g_Mutex;

#endif //_FILTER_MULTITHREAD
};

//建议直接使用此宏
#define GET_CDR_FIELD_FILTER(field, section) \
    CCDRFieldFilter::GetInstance()->GetField(#section, #field)
    
#endif //__FIELD_FILTER_H__
