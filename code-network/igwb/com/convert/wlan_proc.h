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
    //转换一张话单
    BOOL ConvertABill(SBill* pABill, SBill* pConvertedBill, CFormat* pDestFmt);

protected:
    
};

class CBPWLAN : public CBillProcess  
{
public:
    CBPWLAN();
    virtual ~CBPWLAN();

    BOOL Init(UINT4 uAPID, BOOL bNeedConvert, BOOL bNeedMerge); //初始化函数

protected:
    //话单预处理，将话单包拆分为单张话单
    BOOL PreProcess(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill);
	//处理一包不需要合并的话单
    virtual BOOL ProcessABill(SBill& OrgBill, SBill& PreBill);

    ACE_DLL* m_pDll;
};

#endif 
