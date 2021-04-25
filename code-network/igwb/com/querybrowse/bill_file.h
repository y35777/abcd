#ifndef __BILLFILE_H__
#define __BILLFILE_H__

#include "../include/precom.h"
#include "../include/format_out.h"
#include "bill_rec.h"
#include "qb_comm.h"

class CIGWBFile;
class CBillFile
{
public:
    //构造函数
    CBillFile(CFormat *pFormat, UINT4 uBillType, UINT4 uAPType, int nAPID, ACE_Mutex* pMutex);

    //析构函数
    virtual ~CBillFile();

    //打开指定的话单文件和设置相应的读写位置指针函数
    BOOL Open(const char* szPathFile, int nOffset);

    //打开指定的话单文件和设置相应的读写位置指针函数
    BOOL OpenRstFile(const char* szPathFile, int nOffset);

    //获取下一张符合条件的话单记录的偏移地址
    BOOL GetNextBillOffset(int& nCurOffset, BOOL& bRemain);

    int  GetFileLen();
    //获取指定文件指定位置的话单记录数据
    int GetRstRecord(int nStartOffset,
                     BYTE*& pBillRecord,
                     UINT4& uBillLen);

    //获取文件的当前读写位置函数
    int GetCurOffset();

    //设置文件的读写指针位置函数
    BOOL SetOffset(int nOffset);

    //设置查询条件字符串函数
    BOOL SetQueryCond(char* szQueryCond);

    //关闭文件函数
    BOOL Close();

    //设置是否显示错误的话单
    void SetDisplayErrBill(BOOL bDisplayErrBill);

    //设置是否合并标志
    void SetMustMerge(BOOL bMustMerge);
    void SetFinalSPFileFmt(BOOL bFinalSPFileFmt);

    //对话单的额外判断
    virtual BOOL IsMatchedBill(void* pData, int nLen){return TRUE;};

protected:

    //IGWB文件指针
    CIGWBFile*  m_pIGWBFile;

    //浏览格式对象指针
    CFormat*    m_pFormat;

    //查询条件字符串
    char        m_szQueryCond[MAX_COND_LEN];

    ACE_Mutex*  m_pMutex;

    //是否显示错误话单
    BOOL        m_bDisplayErrBill;

    //用于获取指定记录的IGWB文件指针
    CIGWBFile*  m_pRstIGWBFile;
};

#endif
