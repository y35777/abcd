#ifndef __CONVERTCOMMON_H__
#define __CONVERTCOMMON_H__

#include "../include/precom.h"

struct SBill
{
    INT4 nLen;          //话单长度
    BYTE* pBillBuf;     //话单内容
};

struct SChannelSynInfo
{
    UINT4 uLastPID;     //后存盘模块该通道保存的最后一包话单的包号
    UINT4 uBillCsn;     //话单流水号（STP和GSM处理中使用）
};

#endif
