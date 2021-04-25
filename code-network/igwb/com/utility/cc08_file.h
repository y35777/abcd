// cc08_file.h: interface for the CCc08File class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CC08_FILE_H__41D3AA16_CBD5_4A7B_94AA_AB48685457A5__INCLUDED_)
#define AFX_CC08_FILE_H__41D3AA16_CBD5_4A7B_94AA_AB48685457A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "igwb_file.h"

class CCc08File : public CIGWBFile  
{
public:
	CCc08File(UINT4 uBillType, UINT4 uBillLen, int nAPID);
	virtual ~CCc08File();

};

#endif // !defined(AFX_CC08_FILE_H__41D3AA16_CBD5_4A7B_94AA_AB48685457A5__INCLUDED_)
