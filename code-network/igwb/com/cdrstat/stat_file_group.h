#ifndef __STAT_FILE_GROUP_H__
#define __STAT_FILE_GROUP_H__

#ifdef _PLATFORM_WIN32
#pragma warning(disable:4503)
#endif

#include "../include/toolbox.h"

class CCDRStatFile;

class CCDRStatFileGroup
{
public:
    CCDRStatFileGroup();
    virtual ~CCDRStatFileGroup();
public:

    //设置参数，进行初始化操作
    int Init(UINT4 uAPType,
             UINT4 uBillType,
             BOOL  bMustMerge,
             BOOL  bFinalSPFileFmt,
             UINT4 uBillLen,
             int nAPID);

    //准备指定日期范围内的话单文件
    int PrepareFiles(const char* const szPath);

    //获取下一张话单记录数据
    BOOL GetNextRecord(BYTE*& pCDRRecord,  UINT4& nCDRLen, BOOL&  bRemain);

    //按照文件号打开文件
    BOOL Open(int nFileNo);

    //打开下一个文件
    BOOL OpenNextFile(BOOL& bRemain);

    //转换查询条件
    void TransQueryCond(const char* szCond,
                        const char* szBeginDate,
                        const char* szEndDate,
                        char* szExCond);

protected:

    //话单文件的路径
    char            m_szPath[MAX_PATH];
    //话单文件对象
    CCDRStatFile*   m_pCDRStatFile;
    //话单文件名称列表
    VECTOR<STRING>  m_FileNames;
    //当前打开的话单文件序号
    int             m_nCurFileNo;
};
#endif //__STAT_FILE_GROUP_H__

