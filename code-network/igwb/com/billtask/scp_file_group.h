#ifndef __SCP_FILE_GROUP_H__
#define __SCP_FILE_GROUP_H__

class CScpRvyTask;
#include "../querybrowse/bill_rec.h"
#include "scp_bill_file.h"

class CScpFileGroup
{
public:
    CScpFileGroup(CFormat* pFormat, 
                  UINT4 uBillType,
                  UINT4 uAPType,
                  CScpRvyTask* pScpRvyTask);
    virtual ~CScpFileGroup();

    BOOL Open(int nFileNo, int nOffset);  //打开话单文件函数
    //获取当前打开的话单文件序号和其当前读写位置函数
    void GetCurPosition(int& nFileNo, int& nOffset); 
    //获取下一张符合条件的话单记录数据函数
    BOOL GetNextRecord(CBillRecord*& pBillRecord, int& nStartFileNo, 
                       int& nStartOffset, BOOL& bRemain);
    //准备话单文件函数，针对浏览任务
    BOOL PrepareFile(const char* szFileFullPath);
    //准备话单文件函数，针对查询任务
    BOOL PrepareFiles(const char* szPath, 
                      const char* szStartRange, const char*  szEndRange, 
                      UINT4 uFlg, char* szQueryCond);

protected:
    BOOL OpenNextFile(BOOL& bRemain);   //打开下一个话单文件

protected:
    //话单文件名称列表，按照文件修改时间排序
    VECTOR<STRING> m_qFileNames;  
    CScpBillFile*  m_pBillFile;         //话单文件对象指针
    char       m_szPath[MAX_PATH];  //话单文件的路径
    int        m_nCurFileNo;        //当前打开的话单文件序号
    int        m_nCurOffset;        //当前打开的话单文件的读写指针位置
    CFormat*   m_pFormat;           //格式对象
    UINT4      m_uBillType;         //话单类型
    UINT4      m_uAPType;           //接入点类型
};

#endif //__SCP_FILE_GROUP_H__
