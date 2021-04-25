/*     
    产品名:iGateway Bill V200
    模块名:
    文件名:ini_file_ex.h
    描述信息:
    
    包含CINIFileEx类的定义, 该类扩展了CINIFile类的功能，
    实现了对INI文件的删除段、删除键以及文件不存在时创建一个文件的功能。


    修改记录:
    张玉新,  2003-08 ,   创建.
    陈凉威， 2003-11 ，  将之改名为CINIFileEx，并优化代码.
*/

#ifndef __INI_FILE_EX_H__
#define __INI_FILE_EX_H__

#include "../include/toolbox.h"

class IGWB_Svc_Export CINIFileEx : public CINIFile  
{
public:
    CINIFileEx(const char *szFileName);
    virtual ~CINIFileEx();

public:
    int DeleteKey(const char *szSection,const char *szKey);

    int DelSection(const char *szSection);
    
    int TruncateFile();
    int ReplaceSecWithStr(const char *szSection, const char *szSubstitution);
    int LoadFile();
    
public:
    BOOL Open();
    
protected:
    BYTE *m_pFileBuffer;
};

#endif //__INI_FILE_EX_H__
