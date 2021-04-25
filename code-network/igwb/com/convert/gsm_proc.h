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
    void ProcessSerialNo(SBill* &pABill);           //����GSM��������ˮ��
    //ת��һ�Ż���
    BOOL ConvertABill(SBill* pABill, SBill* pConvertedBill, CFormat* pDestFmt);
};


class CBPGSM : public CBillProcess  
{
public:
    CBPGSM();
    virtual ~CBPGSM();

    BOOL Init(UINT4 uAPID, BOOL bNeedConvert, BOOL bNeedMerge); //��ʼ������

protected:
    //����Ԥ���������������Ϊ���Ż���
    BOOL PreProcess(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill);
};

#endif
