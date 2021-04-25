// s1240_file.h: interface for the CS1240File class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_S1240_FILE_H__1D05E3FA_F9E8_4EFF_88ED_586AF9ADE99E__INCLUDED_)
#define AFX_S1240_FILE_H__1D05E3FA_F9E8_4EFF_88ED_586AF9ADE99E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "igwb_file.h"

class CS1240File : public CIGWBFile  
{
public:
	CS1240File(UINT4 uBillType, UINT4 uBillLen, int nAPID);
	virtual ~CS1240File();

protected:
	
    //读取一张原始话单记录数据
    virtual BOOL ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain); 

    //读取一张最终话单记录数据
    virtual BOOL ReadFinal(void*& pData, UINT4& uDataLen, BOOL& bRemain);

	//获得记录长度
	INT2 GetRecordLen(BYTE* pData); 

protected:

	//每块的剩余长度
	int    m_nBlockLeftLen;

};

#endif // !defined(AFX_S1240_FILE_H__1D05E3FA_F9E8_4EFF_88ED_586AF9ADE99E__INCLUDED_)
