// ewsd_file.h: interface for the CEwsdFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EWSD_FILE_H__E59B4725_9D7D_4E45_B273_7DD71362B21C__INCLUDED_)
#define AFX_EWSD_FILE_H__E59B4725_9D7D_4E45_B273_7DD71362B21C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "igwb_file.h"

class CEwsdFile : public CIGWBFile  
{
public:
	CEwsdFile(UINT4 uBillType, UINT4 uBillLen, int nAPID);
	virtual ~CEwsdFile();

protected:
	
    //��ȡһ��ԭʼ������¼����
    virtual BOOL ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain); 

};

#endif // !defined(AFX_EWSD_FILE_H__E59B4725_9D7D_4E45_B273_7DD71362B21C__INCLUDED_)
