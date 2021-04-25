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
    void ProcessSerialNo(SBill* &pABill);   //����STP��������ˮ��
    BOOL ConvertABill(SBill* pABill, SBill* pConvertedBill,CFormat* pDestFmt);
};


class CBPSTP : public CBillProcess  
{
public:
    CBPSTP();
    virtual ~CBPSTP();

    BOOL Init(UINT4 uAPID, BOOL bNeedConvert, BOOL bNeedMerge); //��ʼ������

protected:
    //����Ԥ���������������Ϊ���Ż���
    BOOL PreProcess(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill);
};

#endif
