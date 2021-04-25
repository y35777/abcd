// 5ess_file.h: interface for the C5essFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_5ESS_FILE_H__EE893F39_6BE2_4B53_A918_1E6063F23E59__INCLUDED_)
#define AFX_5ESS_FILE_H__EE893F39_6BE2_4B53_A918_1E6063F23E59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "igwb_file.h"

class C5essFile : public CIGWBFile  
{
public:
	C5essFile(UINT4 uBillType, UINT4 uBillLen, int nAPID);
	virtual ~C5essFile();

protected:

    //读取一张原始话单记录数据
    virtual BOOL ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain); 
	
    //读取一张最终话单记录数据
    virtual BOOL ReadFinal(void*& pData, UINT4& uDataLen, BOOL& bRemain);

	//获得记录的长度
    INT2 GetRecordLen(BYTE* pData); 

	//读详细话单
	BOOL ReadOriginal_Detail(void*& pData, UINT4& uDataLen, BOOL& bRemain); 

	//读计次表话单
	BOOL ReadOriginal_Meter(void*& pData, UINT4& uDataLen, BOOL& bRemain); 

protected:

	UINT4 m_MetertValue;
	char* m_szLine;

};

#endif // !defined(AFX_5ESS_FILE_H__EE893F39_6BE2_4B53_A918_1E6063F23E59__INCLUDED_)
