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

    //初始化
    BOOL Init(const char* szFileName);

    //增加一条事件话单
    BOOL AddAEventBill(MSC_Bill_Report* pEventBill, int nOffsetOfSrcFile);

    //通过普通话单查找事件话单
    BOOL FindEventBills(MSC_Bill_Report* pBill,
                        MSC_Bill_Report*& pEventBill,
                        int& nEventCnt);
    
    //通过普通话单删除事件话单
    BOOL RemoveEventBills(MSC_Bill_Report* pEventBill);

    void SetSrcFileName(const char* szSrcFileName);

    //保存EventMap到事件文件中    
    BOOL SaveToFile();
    
    //加载事件文件
    BOOL LoadFromFile();

private:

    //标识同一话单的键
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

    MAP<SMSCBillKey, OFFSETLIST> m_EventMap;   //除当前文件外，所有事件话单
    char m_szEventFileName[MAX_PATH];          //事件文件名

    MAP<SMSCBillKey, OFFSETLIST> m_NewEventMap; //当前文件的事件话单
    char m_szSrcFileName[MAX_PATH];             //当前文件名

    int m_nBillLen;                             //话单长度
    int m_nBillCntOfBuf;                        //读取保存话单文件缓冲区长度
};

#endif
