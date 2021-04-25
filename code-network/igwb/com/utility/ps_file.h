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

    //读取一张原始话单记录数据
    virtual BOOL ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain); 
	
    //读取一张最终话单话单记录数据
    virtual BOOL ReadFinal(void*& pData, UINT4& uDataLen, BOOL& bRemain);  

	BYTE* m_pASN1RecContent;    //用于进行话单解码的内存
    void* m_pASN1IniGlobal;     //用于进行原始话单解码的环境变量
    void* m_pASN1FinalGlobal;   //用于进行最终话单解码的环境变量
    CPSAsnEncap m_PSAsnClosor;
};

#endif // !defined(AFX_3G_FILE_H__C41D314E_BD2E_4EB8_B672_FE296482458F__INCLUDED_)
