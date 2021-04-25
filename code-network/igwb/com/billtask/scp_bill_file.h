#ifndef __SCP_BILL_FILE_H__
#define __SCP_BILL_FILE_H__

class CScpRvyTask;
#include "../querybrowse/bill_rec.h"
#include "../format/format.h"

class CScpBillFile
{
public:
    CScpBillFile(CFormat *pFormat, 
                 UINT4 uBillType, 
                 UINT4 uAPType, 
                 CScpRvyTask* pScpRvyTask);

    virtual ~CScpBillFile();  //析构函数

    //打开指定的话单文件和设置相应的读写位置指针函数
    BOOL Open(const char* szPathFile, int nOffset);
    //获取下一张符合条件的话单记录数据函数
    BOOL GetNextBill(CBillRecord*& pBillRecord, 
                    int& nCurOffset, BOOL& bRemain);
    int GetCurOffset();                            //获取文件的当前读写位置函数
    BOOL SetOffset(int nOffset);                   //设置文件的读写指针位置函数
    BOOL SetQueryCond(char* szQueryCond);          //设置查询条件字符串函数
    BOOL Close();                                  //关闭文件函数

protected:
    //读取一张话单记录数据函数
    BOOL Read(void*& pData, UINT4& uDataLen, BOOL& bRemain);  
    //对话单的额外判断
    virtual BOOL IsMatchedBill(void* pData, int nLen);

protected:
    //CScpRvyTask对象的指针
    CScpRvyTask* m_pScpRvyTask;

    UINT4    m_uAPType;         //接入点类型
    UINT4    m_uBillType;       //话单类型
    CFormat* m_pFormat;         //浏览格式对象指针
    UINT4    m_uBillLen;        //话单的浏览格式的原格式长度
    FILE*    m_pFileStream;     //文件流指针
    char     m_szFileFullPath[MAX_PATH];  //当前打开的话单文件的全路径

    //以下成员变量针对需要ASN.1编解码的情况
    BYTE*    m_pMemFile;        //内存映射文件指针，初始化为NULL
	int      m_nMemFileLen;     //内存映射文件的长度
	int      m_nMemDataLen;     //内存映射文件数据的长度
    int      m_nCurOffset;      //内存映射文件中当前读写位置的偏移量，初始化为0};
};

#endif //__SCP_BILL_FILE_H__
