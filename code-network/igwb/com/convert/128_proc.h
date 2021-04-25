#ifndef __128PROCESS_H__
#define __128PROCESS_H__

#include "bill_proc.h"

class C128Channel : public CProcChannel  
{
public:
    C128Channel(BOOL bProcBillCsn);
    virtual ~C128Channel();

protected:
    void ProcessSerialNo(SBill* &pABill);           //����128��������ˮ��
    //ת��һ�Ż���
    BOOL ConvertABill(SBill* pABill, SBill* pConvertedBill, CFormat* pDestFmt);

protected:
    BOOL   m_bProcBillCsn;
};

class CBP128 : public CBillProcess  
{
public:
    CBP128();
    virtual ~CBP128();

    BOOL Init(UINT4 uAPID, BOOL bNeedConvert, BOOL bNeedMerge); //��ʼ������

protected:
    //����Ԥ���������������Ϊ���Ż���
    BOOL PreProcess(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill);

protected:
    SBill  m_stTmpBill;
};

#endif 
