#ifndef __BILL_QB_RESULT_H__
#define __BILL_QB_RESULT_H__

#include "../include/precom.h"
#include "../include/format_out.h"
#include "../include/frame.h"
#include "qb_comm.h"
#include "file_group.h"
#include "bill_rec.h"
#include <iterator>

class CBillQBResult
{
public:
    //构造函数
    CBillQBResult(UINT2 uDeskNo, int nWndNo, ACE_Mutex* pMutex, CFormat *pFormat,
                  UINT4 uPageSize, CFileGroup* pFileGroup, char* pResultPath);
    ~CBillQBResult();  //析构函数

    void Init();//初始化函数

    void AddARecordOffset(int nStartFileNo, int nStartOffset);//增加一条话单的偏移量

    BOOL SaveToFile();//将记录下的话单偏移量全部保存到文件中

    int GetPageCount();//得到话单页数

    UINT4 GetRecordCount();//得到话单记录条数

    void GetProcess(UINT4& nRecordCount, int& nPercent, int& nCurPage);//获得查询或浏览的进度

    int GetData(int nPageNo, BOOL bRuning, LIST<STRING>& qResult, BOOL bDisplayErrBill);//获取指定页话单记录数据函数

protected:

    //从文件中获得偏移量
    BOOL GetDataFromFile(int nStartRecord, int nRecordCount, ResultOffset*& pResultOffset);

    //从内存中获得偏移量
    BOOL GetDataFromMem(int nStartRecord, int nRecordCount, ResultOffset*& pResultOffset);

    //计算话单页数
    int ComputePageCount();                     //计算总的话单页数

protected:

    CFormat*            m_pFormat;              //格式库对象

    STRING              m_strTmpFileName;       //临时文件名
    FILE*               m_pTmpFile;             //临时文件指针
    VECTOR<ResultOffset> m_ResultOffsetList;    //偏移量列表


    int                 m_nPageCount;           //包括的页数
    int                 m_nFileNo;              //当前被浏览的文件号
    UINT4               m_uPageSize;            //页大小
    UINT4               m_uRecordCount;         //符合条件的话单数量
    LIST<CBillRecord*>  m_pRecordList;          //话单记录对象列表

    CFileGroup*         m_pFileGroup;           //文件组对象
    VECTOR<STRING>      m_qFileNames;
    STRING              m_szPath;    
    INT8                m_nTotalFileLength;     //文件组所有文件的长度

    ACE_Mutex*          m_pFormatMutex;
    ACE_Mutex           m_Mutex;
};

#endif //__BILL_QB_RESULT_H__





















