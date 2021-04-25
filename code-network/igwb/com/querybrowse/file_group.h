#ifndef __FILEGROUP_H__
#define __FILEGROUP_H__

#include "../include/precom.h"
#include "../include/format_out.h"
#include "qb_comm.h"
#include "bill_file.h"
#include <iterator>

class CFileGroup
{
public:
    //构造函数
    CFileGroup(CFormat* pFormat, UINT4 uBillType, UINT4 uAPType, ACE_Mutex* pMutex, int nAPID);
    virtual ~CFileGroup();  //析构函数

    BOOL Open(int nFileNo, int nOffset);  //打开话单文件函数
    //获取当前打开的话单文件序号和其当前读写位置函数
    void GetCurPosition(int& nFileNo, int& nOffset);
    //获取下一张符合条件的话单记录数据的偏移地址
    BOOL GetNextRecordOffset(int& nStartFileNo, int& nStartOffset, BOOL& bRemain);
    //获取指定文件指定位置的话单记录数据
    int GetRstRecord(int nStartFileNo,
                     int nStartOffset,
                     BYTE*& pBillRecord,
                     UINT4& uBillLen);

    //准备话单文件函数，针对浏览任务
    BOOL PrepareFile(const char* szFileFullPath);
    //准备话单文件函数，针对查询任务
    BOOL PrepareFiles(const char* szPath,
                      const char* szStartRange, const char*  szEndRange,
                      UINT4 uFlg, char* szQueryCond);

    //<SWPD05145> add by YanYong 2004-05-25
    BOOL PrepareFiles(const char* szPath, LIST<STRING> &queryList, 
                      UINT4 uFlg, char* szQueryCond);
    //</SWPD05145>

    void SetMustMerge(BOOL bMustMerge);
    void SetFinalSPFileFmt(BOOL bFinalSPFileFmt);
    INT8 GetQueryedLength();//获得已查询的文件长度


    INT8 GetTotalFileLength(); //获得文件组中所有文件的长度
    INT8 CacalTotalFileLength(); //获得文件组中所有文件的长度
    //设置是否显示错误的话单
    inline void SetDisplayErrBill(BOOL bDisplayErrBill)
    {
        if(m_pBillFile)
        {
            m_pBillFile->SetDisplayErrBill(bDisplayErrBill);
        }
    }

    VECTOR<STRING> GetFileNames();
    STRING GetFilesPath();
    
protected:
    BOOL OpenNextFile(BOOL& bRemain);   //打开下一个话单文件

protected:
    //话单文件名称列表，按照文件修改时间排序
    VECTOR<STRING> m_qFileNames;
    CBillFile* m_pBillFile;         //话单文件对象指针
    char       m_szPath[MAX_PATH];  //话单文件的路径
    int        m_nCurFileNo;        //当前打开的话单文件序号
    int        m_nCurOffset;        //当前打开的话单文件的读写指针位置

    INT8       m_nTotalFileLength;  //文件组中所有文件的长度

    INT8       m_nQueryedFileLen;

    ACE_Mutex  m_Mutex;

    //用于获得指定记录的起始文件号变量
    int        m_nStartFileNo;

    //用于获得指定记录的起始文件名变量
    STRING     m_strRstFileName;
};

#endif
