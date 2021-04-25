#ifndef __STPPROCESS_H__
#define __STPPROCESS_H__

#include "proc_chanl.h"
#include "bill_proc.h"

class CSTPChannel : public CProcChannel  
{
public:
    CSTPChannel();
    virtual ~CSTPChannel();

protected:
    void ProcessSerialNo(SBill* &pABill);   //处理STP话单的流水号
    BOOL ConvertABill(SBill* pABill, SBill* pConvertedBill,CFormat* pDestFmt);
};


class CBPSTP : public CBillProcess  
{
public:
    CBPSTP();
    virtual ~CBPSTP();

    BOOL Init(UINT4 uAPID, BOOL bNeedConvert, BOOL bNeedMerge); //初始化函数

protected:
    //话单预处理，将话单包拆分为单张话单
    BOOL PreProcess(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill);
};

#endif
