#ifndef __CONVERTCOMMON_H__
#define __CONVERTCOMMON_H__

#include "../include/precom.h"

struct SBill
{
    INT4 nLen;          //��������
    BYTE* pBillBuf;     //��������
};

struct SChannelSynInfo
{
    UINT4 uLastPID;     //�����ģ���ͨ����������һ�������İ���
    UINT4 uBillCsn;     //������ˮ�ţ�STP��GSM������ʹ�ã�
};

#endif
