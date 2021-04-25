// zxj_file.h: interface for the CZxjFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZXJ_FILE_H__F4215A7A_E1D8_4E81_AA60_1326F2C5A6F5__INCLUDED_)
#define AFX_ZXJ_FILE_H__F4215A7A_E1D8_4E81_AA60_1326F2C5A6F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "igwb_file.h"

class CZxjFile : public CIGWBFile  
{
public:
	CZxjFile(UINT4 uBillType, UINT4 uBillLen, int nAPID);

	virtual ~CZxjFile();

protected:
	
    //��ȡһ��ԭʼ������¼����
    virtual BOOL ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain); 

    //��ȡһ�����ջ�����¼����
    virtual BOOL ReadFinal(void*& pData, UINT4& uDataLen, BOOL& bRemain);

protected:

	//�汾��
	INT2 m_nVersion;
};

#endif // !defined(AFX_ZXJ_FILE_H__F4215A7A_E1D8_4E81_AA60_1326F2C5A6F5__INCLUDED_)
