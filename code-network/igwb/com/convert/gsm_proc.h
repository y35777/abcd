#ifndef __GSMPROCESS_H__
#define __GSMPROCESS_H__

#include "proc_chanl.h"
#include "bill_proc.h"

class CGSMChannel : public CProcChannel  
{
public:
    CGSMChannel();
    virtual ~CGSMChannel();

protected:
    void ProcessSerialNo(SBill* &pABill);           //处理GSM话单的流水号
    //转换一张话单
    BOOL ConvertABill(SBill* pABill, SBill* pConvertedBill, CFormat* pDestFmt);
};


class CBPGSM : public CBillProcess  
{
public:
    CBPGSM();
    virtual ~CBPGSM();

    BOOL Init(UINT4 uAPID, BOOL bNeedConvert, BOOL bNeedMerge); //初始化函数

protected:
    //话单预处理，将话单包拆分为单张话单
    BOOL PreProcess(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill);
};

#endif
