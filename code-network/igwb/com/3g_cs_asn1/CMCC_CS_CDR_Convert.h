#ifndef __CMCC_CS_CDR_CONVERT_H__
#define __CMCC_CS_CDR_CONVERT_H__

#include "MSCV9BillFormat.h"
#include "CMCC_CS_R4_CDR.h"

namespace CMCC_CS_R4_CDR_CONVERT
{
BOOL ConvertABill(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//check IMEI话单转换
BOOL ConvertABill_CHECK_IMEI(const BYTE* pSrcBill, const int nLen, CMCC_CS_ObservedIMEITicket* pDestBill);

//移动始发话单转换
BOOL ConvertABill_MOC(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//前转话单转换
BOOL ConvertABill_CFW(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                      const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//移动终结话单转换
BOOL ConvertABill_MTC(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

BOOL ConvertABill_GWO(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

BOOL ConvertABill_GWI(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//漫游话单转换
BOOL ConvertABill_ROAM(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//汇接话单转换
BOOL ConvertABill_TRANSIT(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//补充业务操作话单转换
BOOL ConvertABill_SS_ACT(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//短消息始发话单转换
BOOL ConvertABill_MO_SMS(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//短消息终结话单转换
BOOL ConvertABill_MT_SMS(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//TCAMEL话单转换
BOOL ConvertABill_TCAMEL(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//HLR查询话单转换
BOOL ConvertABill_QUERY_HLR(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//移动终结定位业务话单转换
BOOL ConvertABill_MT_LR(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//移动始发定位业务话单转换
BOOL ConvertABill_MO_LR(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//网络侧发起的定位业务话单转换
BOOL ConvertABill_NI_LR(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//漫游话单
BOOL ConvertABill_ROAM(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//公用设备话单
BOOL ConvertABill_COMMON_EQUIP(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill);

//void DeleteABill(CMCC_CS_CallEventRecord* pDestBill);

//将BCD码串转换为带尾0的字符串
//UINT4 BCDtoString(const BYTE* pSrcBCD, char* szDstStr, UINT4 uSrcLen, UINT4 uDstLen = 0);

//将带尾0的字符串转换为BCD码串
//UINT4 StringtoBCD(const char* szSrcStr, BYTE* pDstBCD, UINT4 uDstLen = 0, UINT4 uSrcLen = 0);

//将时间戳转换为3GPP规定的标准格式
void ConvertTimeStamp(CMCC_CS_TimeStamp& ts, const BYTE* pTime); 

//转换通话终止原因值
void ConvertCauseForTerm(int& nOutCause, const int nInCause);

//转换业务码
void ConvertBasicServiceCode(CMCC_CS_BasicServiceCode& bsc, const BYTE nServiceCode, const BYTE TeleBearCode);

//删除编码前的话单结构
//void DeleteABill(CMCC_CS_CallEventRecord* pASN1Bill);

//将BCD码转换为反向BCD码
//int BCDtoRBCD(BYTE* pRBCD, BYTE* pBCD, const int nLen);

//将模块号和首话单流水号组装成CallReference
void GetCallReference(CMCC_CS_CallReference& callReference, const unsigned char nModuleNum, const int nCount);

void ConvertSpeechVersion(BYTE &nOutValue, const int nInValue);

//判断LAC和CellID是否有效
BOOL IsLACandCellIDValid(BYTE* LAC, BYTE* CellID);

//补充业务码转换
void ConvertSScode(BYTE* pSScode, BYTE* pDateTime, CMCC_CS__seqof3_** pDest);

void Link_SScode(BYTE* pDateTime,CMCC_CS__seqof3_** pDest,const int nSS);
};

#endif  //__CMCC_CS_CDR_CONVERT_H__
