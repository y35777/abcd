// text_file.h: interface for the CTextFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXT_FILE_H__27556F96_270D_4170_9B49_D64735F99447__INCLUDED_)
#define AFX_TEXT_FILE_H__27556F96_270D_4170_9B49_D64735F99447__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "igwb_file.h"

class CTextFile : public CIGWBFile  
{
public:
	CTextFile(UINT4 uBillType, UINT4 uBillLen, int nAPID);
	virtual ~CTextFile();

    //读取一张原始话单记录数据
    virtual BOOL ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain); 
	
};

#endif // !defined(AFX_TEXT_FILE_H__27556F96_270D_4170_9B49_D64735F99447__INCLUDED_)
