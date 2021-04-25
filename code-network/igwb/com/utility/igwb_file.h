// igwb_file.h: interface for the CIGWBFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGWB_FILE_H__B69AD699_8396_41C0_8C24_5B37B5572B02__INCLUDED_)
#define AFX_IGWB_FILE_H__B69AD699_8396_41C0_8C24_5B37B5572B02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../include/precom.h"

class CIGWBFile  
{
public:
	//构造函数
	CIGWBFile(UINT4 uBillType, UINT4 uBillLen, int nAPID);

	//析构函数
	virtual ~CIGWBFile();

    //打开指定的话单文件和设置相应的读写位置指针函数
    BOOL Open(const char* szPathFile, int nOffset);

	//获取文件的当前读写位置函数
    int GetCurOffset();      

	//设置文件的读写指针位置函数
    BOOL SetOffset(int nOffset);  

	//关闭文件函数
    BOOL Close();   

	//设置是否合并标志
    void    SetMustMerge(BOOL bMustMerge);
    void    SetFinalSPFileFmt(BOOL bFinalSPFileFmt);

    //读取一张话单记录数据函数
    virtual BOOL Read(void*& pData, UINT4& uDataLen, BOOL& bRemain);  

    //读取一张原始话单记录数据
    virtual BOOL ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain); 
	
    //读取一张最终话单话单记录数据
    virtual BOOL ReadFinal(void*& pData, UINT4& uDataLen, BOOL& bRemain);  	 	
   
    inline int GetFileLen()
    {
        return m_nMemDataLen; 
    }
protected:
	
	//话单类型
    UINT4    m_uBillType;       
    
	//话单的浏览格式的原格式长度
    UINT4    m_uBillLen;        

	//文件流指针
    FILE*    m_pFileStream;     

	//当前打开的话单文件的全路径
    char     m_szFileFullPath[MAX_PATH];  

	//内存映射文件指针，初始化为NULL
	BYTE*    m_pMemFile;    
    
	//内存映射文件的长度
	int      m_nMemFileLen; 
	
	//内存映射文件数据的长度
	int      m_nMemDataLen;   
	
	//内存映射文件中当前读写位置的偏移量，初始化为0 
    int      m_nCurOffset;    
	
	//最终话单是否已合并
	BOOL     m_bMustMerge;

	//最终话单是否是特殊格式
    BOOL     m_bFinalSPFileFmt;

    //接入点号
	int      m_nAPID;
};

#endif // !defined(AFX_IGWB_FILE_H__B69AD699_8396_41C0_8C24_5B37B5572B02__INCLUDED_)
