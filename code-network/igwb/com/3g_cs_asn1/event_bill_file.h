#ifndef __EVENT_BILL_FILE_H__
#define __EVENT_BILL_FILE_H__

#include "../include/base_type.h"
#include "MSCV9BillFormat.h"


typedef LIST<int>  OFFSETLIST;


class CEventBillFile
{
public:
    CEventBillFile();
    ~CEventBillFile();

    //��ʼ��
    BOOL Init(const char* szFileName);

    //����һ���¼�����
    BOOL AddAEventBill(MSC_Bill_Report* pEventBill, int nOffsetOfSrcFile);

    //ͨ����ͨ���������¼�����
    BOOL FindEventBills(MSC_Bill_Report* pBill,
                        MSC_Bill_Report*& pEventBill,
                        int& nEventCnt);
    
    //ͨ����ͨ����ɾ���¼�����
    BOOL RemoveEventBills(MSC_Bill_Report* pEventBill);

    void SetSrcFileName(const char* szSrcFileName);

    //����EventMap���¼��ļ���    
    BOOL SaveToFile();
    
    //�����¼��ļ�
    BOOL LoadFromFile();

private:

    //��ʶͬһ�����ļ�
    struct SMSCBillKey
    {
        BYTE  ModuleNum;
        UINT4 FirstSequence;

        bool operator<(const struct SMSCBillKey& right) const
        {
            if (ModuleNum == right.ModuleNum)
                return (FirstSequence < right.FirstSequence);

            return (ModuleNum < right.ModuleNum);
        }
    };

    MAP<SMSCBillKey, OFFSETLIST> m_EventMap;   //����ǰ�ļ��⣬�����¼�����
    char m_szEventFileName[MAX_PATH];          //�¼��ļ���

    MAP<SMSCBillKey, OFFSETLIST> m_NewEventMap; //��ǰ�ļ����¼�����
    char m_szSrcFileName[MAX_PATH];             //��ǰ�ļ���

    int m_nBillLen;                             //��������
    int m_nBillCntOfBuf;                        //��ȡ���滰���ļ�����������
};

#endif
