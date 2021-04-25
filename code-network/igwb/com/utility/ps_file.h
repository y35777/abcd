 // 3g_file.h: interface for the CPSFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3G_FILE_H__C41D314E_BD2E_4EB8_B672_FE296482458F__INCLUDED_)
#define AFX_3G_FILE_H__C41D314E_BD2E_4EB8_B672_FE296482458F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "igwb_file.h"
#include "../toolbase/ps_asn_encap.h"

class CPSFile : public CIGWBFile  
{
public:
	CPSFile(UINT4 uBillType, UINT4 uBillLen, int nAPID);
	virtual ~CPSFile();

protected:

    //��ȡһ��ԭʼ������¼����
    virtual BOOL ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain); 
	
    //��ȡһ�����ջ���������¼����
    virtual BOOL ReadFinal(void*& pData, UINT4& uDataLen, BOOL& bRemain);  

	BYTE* m_pASN1RecContent;    //���ڽ��л���������ڴ�
    void* m_pASN1IniGlobal;     //���ڽ���ԭʼ��������Ļ�������
    void* m_pASN1FinalGlobal;   //���ڽ������ջ�������Ļ�������
    CPSAsnEncap m_PSAsnClosor;
};

#endif // !defined(AFX_3G_FILE_H__C41D314E_BD2E_4EB8_B672_FE296482458F__INCLUDED_)
