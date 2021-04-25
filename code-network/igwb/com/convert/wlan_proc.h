#ifndef __WLANPROCESS_H__
#define __WLANPROCESS_H__

#include "bill_proc.h"
#include "proc_chanl.h"
#include "ace/DLL.h"

class CWLANChannel : public CProcChannel  
{
public:
    CWLANChannel();
    virtual ~CWLANChannel();   
    virtual BOOL AddABill(SBill& ABill);

protected:
    //ת��һ�Ż���
    BOOL ConvertABill(SBill* pABill, SBill* pConvertedBill, CFormat* pDestFmt);

protected:
    
};

class CBPWLAN : public CBillProcess  
{
public:
    CBPWLAN();
    virtual ~CBPWLAN();

    BOOL Init(UINT4 uAPID, BOOL bNeedConvert, BOOL bNeedMerge); //��ʼ������

protected:
    //����Ԥ���������������Ϊ���Ż���
    BOOL PreProcess(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill);
	//����һ������Ҫ�ϲ��Ļ���
    virtual BOOL ProcessABill(SBill& OrgBill, SBill& PreBill);

    ACE_DLL* m_pDll;
};

#endif 
