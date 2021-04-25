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
	
    //��ȡһ��ԭʼ������¼����
    virtual BOOL ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain); 

    //��ȡһ�����ջ�����¼����
    virtual BOOL ReadFinal(void*& pData, UINT4& uDataLen, BOOL& bRemain);

	//��ü�¼����
	INT2 GetRecordLen(BYTE* pData); 

protected:

	//ÿ���ʣ�೤��
	int    m_nBlockLeftLen;

};

#endif // !defined(AFX_S1240_FILE_H__1D05E3FA_F9E8_4EFF_88ED_586AF9ADE99E__INCLUDED_)
