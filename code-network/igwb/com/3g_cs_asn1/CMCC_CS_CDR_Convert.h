#ifndef __CMCC_CS_CDR_CONVERT_H__
#define __CMCC_CS_CDR_CONVERT_H__

#include "MSCV9BillFormat.h"
#include "CMCC_CS_R4_CDR.h"

namespace CMCC_CS_R4_CDR_CONVERT
{
BOOL ConvertABill(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//check IMEI����ת��
BOOL ConvertABill_CHECK_IMEI(const BYTE* pSrcBill, const int nLen, CMCC_CS_ObservedIMEITicket* pDestBill);

//�ƶ�ʼ������ת��
BOOL ConvertABill_MOC(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//ǰת����ת��
BOOL ConvertABill_CFW(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                      const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//�ƶ��սỰ��ת��
BOOL ConvertABill_MTC(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

BOOL ConvertABill_GWO(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

BOOL ConvertABill_GWI(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//���λ���ת��
BOOL ConvertABill_ROAM(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//��ӻ���ת��
BOOL ConvertABill_TRANSIT(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//����ҵ���������ת��
BOOL ConvertABill_SS_ACT(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//����Ϣʼ������ת��
BOOL ConvertABill_MO_SMS(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//����Ϣ�սỰ��ת��
BOOL ConvertABill_MT_SMS(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//TCAMEL����ת��
BOOL ConvertABill_TCAMEL(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//HLR��ѯ����ת��
BOOL ConvertABill_QUERY_HLR(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//�ƶ��սᶨλҵ�񻰵�ת��
BOOL ConvertABill_MT_LR(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//�ƶ�ʼ����λҵ�񻰵�ת��
BOOL ConvertABill_MO_LR(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//����෢��Ķ�λҵ�񻰵�ת��
BOOL ConvertABill_NI_LR(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//���λ���
BOOL ConvertABill_ROAM(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//�����豸����
BOOL ConvertABill_COMMON_EQUIP(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//void DeleteABill(CMCC_CS_CallEventRecord* pDestBill);

//��BCD�봮ת��Ϊ��β0���ַ���
//UINT4 BCDtoString(const BYTE* pSrcBCD, char* szDstStr, UINT4 uSrcLen, UINT4 uDstLen = 0);

//����β0���ַ���ת��ΪBCD�봮
//UINT4 StringtoBCD(const char* szSrcStr, BYTE* pDstBCD, UINT4 uDstLen = 0, UINT4 uSrcLen = 0);

//��ʱ���ת��Ϊ3GPP�涨�ı�׼��ʽ
void ConvertTimeStamp(CMCC_CS_TimeStamp& ts, const BYTE* pTime); 

//ת��ͨ����ֹԭ��ֵ
void ConvertCauseForTerm(int& nOutCause, const int nInCause);

//ת��ҵ����
void ConvertBasicServiceCode(CMCC_CS_BasicServiceCode& bsc, const BYTE nServiceCode, const BYTE TeleBearCode);

//ɾ������ǰ�Ļ����ṹ
//void DeleteABill(CMCC_CS_CallEventRecord* pASN1Bill);

//��BCD��ת��Ϊ����BCD��
//int BCDtoRBCD(BYTE* pRBCD, BYTE* pBCD, const int nLen);

//��ģ��ź��׻�����ˮ����װ��CallReference
void GetCallReference(CMCC_CS_CallReference& callReference, const unsigned char nModuleNum, const int nCount);

void ConvertSpeechVersion(BYTE &nOutValue, const int nInValue);

//�ж�LAC��CellID�Ƿ���Ч
BOOL IsLACandCellIDValid(BYTE* LAC, BYTE* CellID);

//����ҵ����ת��
void ConvertSScode(BYTE* pSScode, BYTE* pDateTime, CMCC_CS__seqof3_** pDest);

void Link_SScode(BYTE* pDateTime,CMCC_CS__seqof3_** pDest,const int nSS);
};

#endif  //__CMCC_CS_CDR_CONVERT_H__
