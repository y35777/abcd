/*****************************************************
//		GPRSCDRBrowse.cpp
//*****************************************************
// Copyright 2002 by Huawei CO.,LTD.
// All rights reserved.
// Compiler:     Visual C++ 6.0
// Author:       maimaoshi
// Date:         2002-03-18
// Description:  该文件实现了GPRS话单浏览的主要功能
// 
// Side Effects: 
// Functions:
// Notes:    
// 
// Update: 2002-03-18 AM 10:28:00
// Date              Name              Description 
============== ================ ======================

*****************************************************/
#include "r98_cdr_browse.h"

enum CBB_OPERATOR_ID g_nCBB_OI;

#define NETWORKTOMSDIRECTION 1
#define MSTONETWORKDIRECTION NULL

//by ldf 2003-11-05 对应问题单 SWPD00365，按照和Moto沟通结果进行Qos域的缩写
#define GprsQosReliabilityEnumInfo_LENGTH 6
static ENUMITEMINFO GprsQosReliabilityEnumInfo[] = 
{
	0, "Unspecified", //"Unspecified Reliability",
	1, "Ack GTP", //"Acknowledged GTP",
	2, "Unack GTP Ack LLC", //"Unack GTP Acknow LLC",
	3, "Unack GTP LLC Ack RLC", //"Unack GTP LLC Acknow RLC",
	4, "Unack GTP LLC RLC", //"Unack GTP LLC RLC",
	5, "Unack Unprotected Data" //"Unack now Unprotected Data"
};

#define GprsQoSDelayEnumInfo_LENGTH 4
static ENUMITEMINFO GprsQoSDelayEnumInfo[] = 
{
	1, "1", //"Delay Class1",
	2, "2", //"Delay Class2",
	3, "3", //"Delay Class3",
	4, "4" //"Delay Class4"
};

#define GprsQoSPrecedenceEnumInfo_LENGTH 4
static ENUMITEMINFO GprsQoSPrecedenceEnumInfo[] = 
{
	0, "Unspecified", //"UnspecifiedQos",
	1, "High", //"High Priority",
	2, "Normal", //"Normal Priority",
	3, "Low" //"Low Priority"
};

#define GprsQoSPeakThroughputEnumInfo_LENGTH 10
static ENUMITEMINFO GprsQoSPeakThroughputEnumInfo[] = 
{
	0, "Unspecified", //"QoS Peak Throughput Unspecified",
	1, "1000octet/s", //"upTo1000octetPs",
	2, "2000octet/s", //"upTo2000octetPs",
	3, "4000octet/s", //"upTo4000octetPs",
	4, "8000octet/s", //"upTo8000octetPs",
	5, "16000octet/s", //"upTo16000octetPs",
	6, "32000octet/s", //"upTo32000octetPs",
	7, "64000octet/s", //"upTo64000octetPs",
	8, "128000octet/s", //"upTo128000octetPs",
	9, "256000octet/s" //"upTo256000octetPs"
};

#define GprsQoSMeanThroughputEnumInfo_LENGTH 19
static ENUMITEMINFO GprsQoSMeanThroughputEnumInfo[] = 
{
	0, "Best Effort", //"Best Effort",
	1, "100octet/h", //"mean100octetPh",
	2, "200octet/h", //"mean200octetPh",
	3, "500octet/h", //"mean500octetPh",
	4, "1000octet/h", //"mean1000octetPh",
	5, "2000octet/h", //"mean2000octetPh",
	6, "5000octet/h", //"mean5000octetPh",
	7, "10000octet/h", //"mean10000octetPh",
	8, "20000octet/h", //"mean20000octetPh",
	9, "50000octet/h", //"mean50000octetPh",
	10, "100000octet/h", //"mean100000octetPh",
	11, "200000octet/h", //"mean200000octetPh",
	12, "500000octet/h", //"mean500000octetPh",
	13, "1000000octet/h", //"mean1000000octetPh",
	14, "2000000octet/h", //"mean2000000octetPh",
	15, "5000000octet/h", //"mean5000000octetPh",
	16, "10000000octet/h", //"mean10000000octetPh",
	17, "20000000octet/h", //"mean20000000octetPh",
	18, "50000000octet/h" //"mean50000000octetPh"
};

#define GprsChangeConditionEnumInfo_LENGTH 3
static ENUMITEMINFO GprsChangeConditionEnumInfo [] =
{
	0, "QoS Change",
	1, "Tariff Time",
	2, "Record Closure"
};

#define GPRSCAUSEFORRECCLOSINGENUNINFO_LENGTH	8
static ENUMITEMINFO GprsCauseForRecClosingEnumInfo [] = 
{
	0, "Normal Release",
	4, "Abnormal Release",
	16, "Volume Limit",
	17, "Time Limit",
	18, "SGSN Change",
	19, "maxChangeCond",
	20, "Management Intervention",
	30, "Content"
};

#define GprsApnSelectionModeEnumInfo_LENGTH 3
static ENUMITEMINFO GprsApnSelectionModeEnumInfo [] = 
{
	0, "MS or Network Provided Subscription Verified",
	1, "MS Provided Subscription Not Verified",
	2, "Network Provided Subscription Not Verified"
};

#define GprsConsolidationResultEnumInfo_LENGTH 5
static ENUMITEMINFO GprsConsolidationResultEnumInfo [] = 
{
	0, "normal",
	1, "notNormal",
	2, "forInterSGSNConsolidation",
	3, "reachLimit",
	4, "onlyOneCDRGenerated"
};

#define R98_760_RECORDE_TYPE_ENUNINFO_LENGTH 5
static ENUMITEMINFO R98_760_Recorde_Type_EnumInfo[] =
{
    18,	"S-CDR",
    19,	"G-CDR",
    20,	"M-CDR",
    21,	"S-SMO-CDR",
    22,	"S-SMT-CDR"
};

/******************************************************
// Function name: 	GetAllFieldTextValueOfGPRSCDR
// Author:          maimaoshi
// Date:            2002-03-19
// Description:     GPRS话单浏览处理的入口函数
// Input:
//       arg1 :char *pCDR			输入话单的数据指针
//       arg2 :char *pFormatName	格式名字符串
//       arg3 :char *pBuffer		放结果字符串的缓冲区
//       arg4 :int nLength			存放结果字符串的缓冲区的有效长度
// Return value: int				转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
namespace NS_R98_760{
int GetAllFieldTextValueOfGPRSCDR(char *pCDR, int nFormatType, char *pBuffer, int nLength)
{
	int nLen = 0;
	switch (nFormatType)
	{
	case S_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOfGPRSSGSNPDP(pCDR, pBuffer, nLength);
		}
		break;
	case G_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOfGPRSGGSNPDP(pCDR, pBuffer, nLength);
		}
		break;
	case M_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOfGPRSSGSNMM(pCDR, pBuffer, nLength);
		}
		break;
	case S_SMO_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOfGPRSSGSNSMO(pCDR, pBuffer, nLength);
		}
		break;
	case S_SMT_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOfGPRSSGSNSMT(pCDR, pBuffer, nLength);
		}
		break;
	case L_S_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOfGPRSLARGESGSNPDP(pCDR, pBuffer, nLength);
		}
		break;
	case L_G_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOfGPRSLARGEGGSNPDP(pCDR, pBuffer, nLength);
		}
		break;		
    /*	
    case L_M_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOfGPRSLARGESGSNMM(pCDR, pBuffer, nLength);
		}
		break;
	*/	
	default:
		pBuffer[0] = 0;
	}
	return nLen;
}

/******************************************************
// Function name: 	GetQueryFieldTextValueOfGPRSCDR
// Author:          maimaoshi
// Date:            2002-03-19
// Description:     获取GPRS话单查询条件域值入口函数
// Input:
//       arg1 :char *pCDR			输入话单的数据指针
//       arg2 :char *pFormatName	格式名字符串
//       arg3 :char *pBuffer		放结果字符串的缓冲区
//       arg4 :int nLength			存放结果字符串的缓冲区的有效长度
// Return value: int				转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int GetQueryFieldTextValueOfGPRSCDR(char *pCDR,
                                    int nFormatType,
                                    char *pBuffer,
                                    int nLength,
                                    QUERY_COND_FIELDS_VALUE* pValues)
{
	int nLen = 0;
	switch (nFormatType)
	{
	case S_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf_Q_GPRSSGSNPDP(pCDR, pBuffer, nLength, pValues);
		}
		break;
	case G_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf_Q_GPRSGGSNPDP(pCDR, pBuffer, nLength, pValues);
		}
		break;
	case M_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf_Q_GPRSSGSNMM(pCDR, pBuffer, nLength, pValues);
		}
		break;
	case S_SMO_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf_Q_GPRSSGSNSMO(pCDR, pBuffer, nLength, pValues);
		}
		break;
	case S_SMT_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf_Q_GPRSSGSNSMT(pCDR, pBuffer, nLength, pValues);
		}
		break;
	case L_S_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf_Q_GPRSLARGESGSNPDP(pCDR, pBuffer, nLength, pValues);
		}
		break;
	case L_G_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf_Q_GPRSLARGEGGSNPDP(pCDR, pBuffer, nLength, pValues);
		}
		break;
    /*	
	case L_M_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf_Q_GPRSLARGESGSNMM(pCDR, pBuffer, nLength, pValues);
		}
		break;
    */	
	default:
		pBuffer[0] = 0;
	}
	return nLen;
}

//////////////////////////////////////////////////////////////////////////////


/******************************************************
// Function name: 	GetTextValueOfGPRSSGSNPDP
// Author:          maimaoshi
// Date:            2002-03-19
// Description:     处理S-CDR话单浏览解释的函数(28个字段)
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
============ ================== ======================
//
******************************************************/
int GetTextValueOfGPRSSGSNPDP(char *pValue, char *pBuffer, int nLength)
{
#define SCDRBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	Org_SGSNPDPRecord *pBill = (Org_SGSNPDPRecord*)pValue;
	int  nLen = 0;  
    int nTempLen = 0;

	//ASSERT((BOOL)(pValue && pBuffer));
	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != Org_CallEventRecordType_sgsnPDPRecord)
		return 0;

	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;
	
	//recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R98_760_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R98_760_Recorde_Type_EnumInfo;
    nTempLen = EnumToStr(&EnumInfo,
                         pBill->recordType,
                         &pTempBuffer[nLen],
                         nLength - nLen);

    if (nTempLen)
        nLen += nTempLen;
    else
    {
        return 0;
    }
    pTempBuffer[nLen++] = '^';

	//networkInitiation
	if (pBill->bit_mask & Org_SGSNPDPRecord_networkInitiation_present)
	{
		nLen += BoolToStr(pBill->networkInitiation, &pTempBuffer[nLen], 
			SCDRBUFFERLENGTH - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//anonymousAccessIndicator
	if (pBill->bit_mask & Org_SGSNPDPRecord_anonymousAccessIndicator_present)
	{
		nLen += BoolToStr(pBill->anonymousAccessIndicator, &pTempBuffer[nLen], 
			SCDRBUFFERLENGTH - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';


	//servedIMSI
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI, 
							&pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//servedIMEI
	if (pBill->bit_mask & Org_SGSNPDPRecord_servedIMEI_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMEI,
							&pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//servedMSISDN
	if (pBill->bit_mask & Org_SGSNPDPRecord_servedMSISDN_present)
	{
	    nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedMSISDN, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	
	pTempBuffer[nLen++] = '^';

	//sgsnAddress
	nLen += _GPRSIPAddrToStr(&pBill->sgsnAddress, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//msNetworkCapability
	if (pBill->bit_mask & Org_SGSNPDPRecord_msNetworkCapability_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->msNetworkCapability, &pTempBuffer[nLen], 
							SCDRBUFFERLENGTH - nLen, "0x", FALSE);							
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
	
	//routingArea
	if (pBill->bit_mask & Org_SGSNPDPRecord_routingArea_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->routingArea, &pTempBuffer[nLen],
							SCDRBUFFERLENGTH - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//locationAreaCode
	if (pBill->bit_mask & Org_SGSNPDPRecord_locationAreaCode_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationAreaCode, &pTempBuffer[nLen],
							SCDRBUFFERLENGTH - nLen, "0x", FALSE);	
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//cellIdentity
	if (pBill->bit_mask & Org_SGSNPDPRecord_cellIdentity_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->cellIdentity, &pTempBuffer[nLen],
							SCDRBUFFERLENGTH - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//chargingID
	nLen += NumToDecStr(pBill->chargingID, &pTempBuffer[nLen], 
						SCDRBUFFERLENGTH - nLen, FALSE);
	pTempBuffer[nLen++] = '^';

	//ggsnAddressUsed
	nLen += _GPRSIPAddrToStr(&pBill->ggsnAddressUsed, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//accessPointNameNI
	strcpy(&pTempBuffer[nLen], pBill->accessPointNameNI);
	nLen += strlen(pBill->accessPointNameNI);
	pTempBuffer[nLen++] = '^';

	//pdpType
	nLen += HexOctecToStr((LPOCTECSTRING)&pBill->pdpType, &pTempBuffer[nLen],
						SCDRBUFFERLENGTH - nLen, "0x", FALSE);
	pTempBuffer[nLen++] = '^';

	//servedPDPAddress
	nLen += _GPRSPDPAddrToStr(&pBill->servedPDPAddress, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	
	//listOfTrafficVolumes$$$，链表结构
	nLen += Gprs_ListOfTrafficVolumesToStr(pBill->listOfTrafficVolumes, &pTempBuffer[nLen], 
										SCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//recordOpeningTime
	nLen += TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime, &pTempBuffer[nLen],
							SCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//duration
	nLen += NumToDecStr(pBill->duration, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen, FALSE);
	pTempBuffer[nLen++] = '^';

	//sgsnChange
	if (pBill->bit_mask & Org_SGSNPDPRecord_sgsnChange_present)
	{
		nLen += BoolToStr(pBill->sgsnChange, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//causeForRecClosing
	EnumInfo.nCount = GPRSCAUSEFORRECCLOSINGENUNINFO_LENGTH;
	EnumInfo.pInfoTable = GprsCauseForRecClosingEnumInfo;
	nTempLen = EnumToStr(&EnumInfo, pBill->causeForRecClosing,&pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	
    if (nTempLen)
		nLen += nTempLen;
	else
	{
		strcpy(&pTempBuffer[nLen], "Unknown reason");
		nLen += strlen("Unknown reason");
	}
	pTempBuffer[nLen++] = '^';

	//diagnostics
	if (pBill->bit_mask & Org_SGSNPDPRecord_diagnostics_present)
	{
		nLen += Gprs_DiagnosticsToStr(&pBill->diagnostics, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//recordSequenceNumber
	if (pBill->bit_mask & Org_SGSNPDPRecord_recordSequenceNumber_present)
	{
		nLen += NumToDecStr(pBill->recordSequenceNumber, &pTempBuffer[nLen], 
							SCDRBUFFERLENGTH - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//nodeID
	if (pBill->bit_mask & Org_SGSNPDPRecord_nodeID_present)
	{
		strcpy(&pTempBuffer[nLen], pBill->nodeID);
		nLen += strlen(pBill->nodeID);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//recordExtensions$$$,是链表
	if (pBill->bit_mask & Org_SGSNPDPRecord_recordExtensions_present)
	{
		nLen += Gprs_RecordExtensionsToStr(pBill->recordExtensions, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

        //added by maimaoshi,2003-04-12
	//apnSelectionMode
	if(pBill->bit_mask & Org_SGSNPDPRecord_apnSelectionMode_present)
	{
		EnumInfo.nCount = GprsApnSelectionModeEnumInfo_LENGTH;
		EnumInfo.pInfoTable = GprsApnSelectionModeEnumInfo;
		nLen += EnumToStr(&EnumInfo, pBill->apnSelectionMode, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;		
	}
	pTempBuffer[nLen++] = '^';
	//end

	//accessPointNameOI
	strcpy(&pTempBuffer[nLen], pBill->accessPointNameOI);
	nLen += strlen(pBill->accessPointNameOI);
	pTempBuffer[nLen++] = '^';

	//localSequenceNumber
	if (pBill->bit_mask & Org_SGSNPDPRecord_localSequenceNumber_present)
	{
		nLen += NumToDecStr(pBill->localSequenceNumber, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	pTempBuffer[nLen] = 0;

	if (nLen >= nLength)
		nLen = 0;

	return nLen;
}

/******************************************************
// Function name: 	GetTextValueOfGPRSGGSNPDP
// Author:          maimaoshi
// Date:            2002-03-19
// Description:     处理G-CDR话单浏览解释的函数(23个字段)
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int GetTextValueOfGPRSGGSNPDP(char *pValue, char *pBuffer, int nLength)
{
#define GCDRBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	Org_GGSNPDPRecord *pBill = (Org_GGSNPDPRecord*)pValue;
	int nLen = 0;
    int nTempLen = 0;

	//ASSERT((BOOL)(pValue && pBuffer));
	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != Org_CallEventRecordType_ggsnPDPRecord)
		return 0;

	char *pTempBuffer = pBuffer;
	
	if (!pTempBuffer)
		return 0;
	
	//recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R98_760_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R98_760_Recorde_Type_EnumInfo;
    nTempLen = EnumToStr(&EnumInfo,
                         pBill->recordType,
                         &pTempBuffer[nLen],
                         nLength - nLen);

    if (nTempLen)
        nLen += nTempLen;
    else
    {
        return 0;
    }
    pTempBuffer[nLen++] = '^';

	//networkInitiation
	if (pBill->bit_mask & Org_GGSNPDPRecord_networkInitiation_present)
	{
		nLen += BoolToStr(pBill->networkInitiation, &pTempBuffer[nLen], 
							GCDRBUFFERLENGTH - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//anonymousAccessIndicator
	if (pBill->bit_mask & Org_GGSNPDPRecord_anonymousAccessIndicator_present)
	{
		nLen += BoolToStr(pBill->anonymousAccessIndicator, &pTempBuffer[nLen], 
			SCDRBUFFERLENGTH - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//servedIMSI
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI, 
							&pTempBuffer[nLen], GCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//servedMSISDN
	if (pBill->bit_mask & Org_GGSNPDPRecord_servedMSISDN_present)
	{
	    nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedMSISDN, &pTempBuffer[nLen], 
									    GCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//ggsnAddress
	nLen += _GPRSIPAddrToStr(&pBill->ggsnAddress, &pTempBuffer[nLen], GCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//chargingID
	nLen += NumToDecStr(pBill->chargingID, &pTempBuffer[nLen], 
						GCDRBUFFERLENGTH - nLen, FALSE);
	pTempBuffer[nLen++] = '^';

	//sgsnAddress$$$，struct Org__seqof2
	nLen += _GPRSSGSNAddrToStr(pBill->sgsnAddress, &pTempBuffer[nLen], 
								GCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//accessPointNameNI
	strcpy(&pTempBuffer[nLen], pBill->accessPointNameNI);
	nLen += strlen(pBill->accessPointNameNI);
	pTempBuffer[nLen++] = '^';

	//pdpType
	nLen += HexOctecToStr((LPOCTECSTRING)&pBill->pdpType, &pTempBuffer[nLen],
						GCDRBUFFERLENGTH - nLen, "0x", FALSE);
	pTempBuffer[nLen++] = '^';

	//servedPDPAddress
	nLen += _GPRSPDPAddrToStr(&pBill->servedPDPAddress, &pTempBuffer[nLen], 
							GCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//remotePDPAddress$$$，struct Org__seqof3
	if (pBill->bit_mask & Org_remotePDPAddress_present)
	{
		nLen += _GPRSREMOTEPDPAddrToStr(pBill->remotePDPAddress, &pTempBuffer[nLen], 
									GCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//dynamicAddressFlag
	if (pBill->bit_mask & Org_dynamicAddressFlag_present)
	{
		nLen += BoolToStr(pBill->dynamicAddressFlag, &pTempBuffer[nLen], 
							GCDRBUFFERLENGTH - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//listOfTrafficVolumes$$$，struct Org__seqof1_
	nLen += Gprs_ListOfTrafficVolumesToStr(pBill->listOfTrafficVolumes, &pTempBuffer[nLen], 
										GCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';
   
	//recordOpeningTime
	nLen += TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime, &pTempBuffer[nLen],
							GCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//duration
	nLen += NumToDecStr(pBill->duration, &pTempBuffer[nLen], 
						GCDRBUFFERLENGTH - nLen, FALSE);
	pTempBuffer[nLen++] = '^';

	//causeForRecClosing
	EnumInfo.nCount = GPRSCAUSEFORRECCLOSINGENUNINFO_LENGTH;
	EnumInfo.pInfoTable = GprsCauseForRecClosingEnumInfo;
	nTempLen = EnumToStr(&EnumInfo, pBill->causeForRecClosing,&pTempBuffer[nLen], 
						GCDRBUFFERLENGTH - nLen);
    
	if (nTempLen)
		nLen += nTempLen;
	else
	{
		strcpy(&pTempBuffer[nLen], "Unknown reason");
		nLen += strlen("Unknown reason");
	}
	pTempBuffer[nLen++] = '^';

	//diagnostics
	if (pBill->bit_mask & Org_GGSNPDPRecord_diagnostics_present)
	{
		nLen += Gprs_DiagnosticsToStr(&pBill->diagnostics, &pTempBuffer[nLen], 
									GCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//recordSequenceNumber
	if (pBill->bit_mask & Org_GGSNPDPRecord_recordSequenceNumber_present)
	{
		nLen += NumToDecStr(pBill->recordSequenceNumber, &pTempBuffer[nLen], 
							GCDRBUFFERLENGTH - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//nodeID
	if (pBill->bit_mask & Org_GGSNPDPRecord_nodeID_present)
	{
		strcpy(&pTempBuffer[nLen], pBill->nodeID);
		nLen += strlen(pBill->nodeID);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

 	//recordExtensions
	if (pBill->bit_mask & Org_GGSNPDPRecord_recordExtensions_present)
	{
		switch (g_nCBB_OI)
		{			    
		case CBB_OPERATOR_ID_CMCC:
		    nLen += CMCC_R98_GCDRRecordExtensionsToStr((Org_ManagementExtensions_*)pBill->recordExtensions, 
                        &pTempBuffer[nLen], GCDRBUFFERLENGTH - nLen);
		    break;
			    
		default:
    		nLen += R98_GCDRRecordExtensionsToStr((Org_ManagementExtensions_*)pBill->recordExtensions, 
                       &pTempBuffer[nLen], GCDRBUFFERLENGTH - nLen);
            break;			    
        }
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';


	//added by maimaoshi,2003-04-12
	//apnSelectionMode
	if(pBill->bit_mask & Org_GGSNPDPRecord_apnSelectionMode_present)
	{
		EnumInfo.nCount = GprsApnSelectionModeEnumInfo_LENGTH;
		EnumInfo.pInfoTable = GprsApnSelectionModeEnumInfo;
		nLen += EnumToStr(&EnumInfo, pBill->apnSelectionMode, &pTempBuffer[nLen], 
								GCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;		
	}
	pTempBuffer[nLen++] = '^';
	//end

	//localSequenceNumber
	if (pBill->bit_mask & Org_GGSNPDPRecord_localSequenceNumber_present)
	{
		nLen += NumToDecStr(pBill->localSequenceNumber, &pTempBuffer[nLen], 
							GCDRBUFFERLENGTH - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	pTempBuffer[nLen] = 0;
	
	if (nLen >= nLength)
		nLen = 0;

	return nLen;
}


/******************************************************
// Function name: 	GetTextValueOfGPRSSGSNMM
// Author:          maimaoshi
// Date:            2002-03-19
// Description:     处理M-CDR话单浏览解释的函数(18个字段)
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int GetTextValueOfGPRSSGSNMM(char *pValue, char *pBuffer, int nLength)
{
#define MCDRBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	Org_SGSNMMRecord *pBill = (Org_SGSNMMRecord*)pValue;
	int nLen = 0;
    int nTempLen = 0;

	//ASSERT((BOOL)(pValue && pBuffer));
	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != Org_CallEventRecordType_sgsnMMRecord)
		return 0;

	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;

	//recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R98_760_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R98_760_Recorde_Type_EnumInfo;
    nTempLen = EnumToStr(&EnumInfo,
                         pBill->recordType,
                         &pTempBuffer[nLen],
                         nLength - nLen);

    if (nTempLen)
        nLen += nTempLen;
    else
    {
        return 0;
    }
    pTempBuffer[nLen++] = '^';

	//servedIMSI
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI, 
							&pTempBuffer[nLen], MCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//servedIMEI
	if (pBill->bit_mask & Org_SGSNMMRecord_servedIMEI_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMEI,
							&pTempBuffer[nLen], MCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//sgsnAddress
	nLen += _GPRSIPAddrToStr(&pBill->sgsnAddress, &pTempBuffer[nLen], 
							MCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//msNetworkCapability
	if (pBill->bit_mask & Org_SGSNMMRecord_msNetworkCapability_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->msNetworkCapability, &pTempBuffer[nLen], 
							MCDRBUFFERLENGTH - nLen, "0x", FALSE);							
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//routingArea
	if (pBill->bit_mask & Org_SGSNMMRecord_routingArea_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->routingArea, &pTempBuffer[nLen],
								MCDRBUFFERLENGTH - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//locationAreaCode
	if (pBill->bit_mask & Org_SGSNMMRecord_locationAreaCode_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationAreaCode, &pTempBuffer[nLen],
							MCDRBUFFERLENGTH - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//cellIdentity
	if (pBill->bit_mask & Org_SGSNMMRecord_cellIdentity_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->cellIdentity, &pTempBuffer[nLen],
							MCDRBUFFERLENGTH - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

    
	//changeLocation$$$，struct Org__seqof4
	if (pBill->bit_mask & Org_changeLocation_present)
	{
		nLen += Gprs_ChangeLocationsToStr(pBill->changeLocation, 
									&pTempBuffer[nLen], MCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//recordOpeningTime
	nLen += TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime, &pTempBuffer[nLen],
							MCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//duration
	if (pBill->bit_mask & Org_duration_present)
	{
		nLen += NumToDecStr(pBill->duration, &pTempBuffer[nLen], 
							MCDRBUFFERLENGTH - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//sgsnChange
	if (pBill->bit_mask & Org_SGSNMMRecord_sgsnChange_present)
	{
		nLen += BoolToStr(pBill->sgsnChange, &pTempBuffer[nLen], 
							MCDRBUFFERLENGTH - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//causeForRecClosing
	EnumInfo.nCount = GPRSCAUSEFORRECCLOSINGENUNINFO_LENGTH;
	EnumInfo.pInfoTable = GprsCauseForRecClosingEnumInfo;
	nTempLen = EnumToStr(&EnumInfo, pBill->causeForRecClosing,
						&pTempBuffer[nLen], MCDRBUFFERLENGTH - nLen);

	if (nTempLen)
		nLen += nTempLen;
	else
	{
		strcpy(&pTempBuffer[nLen], "Unknown reason");
		nLen += strlen("Unknown reason");
	}
	pTempBuffer[nLen++] = '^';

	//diagnostics
	if (pBill->bit_mask & Org_SGSNMMRecord_diagnostics_present)
	{
		nLen += Gprs_DiagnosticsToStr(&pBill->diagnostics, &pTempBuffer[nLen], 
									MCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//recordSequenceNumber
	if (pBill->bit_mask & Org_SGSNMMRecord_recordSequenceNumber_present)
	{
		nLen += NumToDecStr(pBill->recordSequenceNumber, &pTempBuffer[nLen], 
							MCDRBUFFERLENGTH - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//nodeID
	if (pBill->bit_mask & Org_SGSNMMRecord_nodeID_present)
	{
		strcpy(&pTempBuffer[nLen], pBill->nodeID);
		nLen += strlen(pBill->nodeID);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//recordExtensions
	if (pBill->bit_mask & Org_SGSNMMRecord_recordExtensions_present)
	{
		nLen += Gprs_RecordExtensionsToStr(pBill->recordExtensions, &pTempBuffer[nLen], 
										MCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//localSequenceNumber
	if (pBill->bit_mask & Org_SGSNMMRecord_localSequenceNumber_present)
	{
		nLen += NumToDecStr(pBill->localSequenceNumber, &pTempBuffer[nLen], 
							MCDRBUFFERLENGTH - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	pTempBuffer[nLen] = 0;

	if (nLen >= nLength)
		nLen = 0;

	return nLen;
}


/******************************************************
// Function name: 	GetTextValueOfGPRSSGSNSMO
// Author:          maimaoshi
// Date:            2002-03-19
// Description:     处理S-SMO-CDR话单浏览解释的函数(16个字段)
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int GetTextValueOfGPRSSGSNSMO(char *pValue, char *pBuffer, int nLength)
{
#define SMOBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	Org_SGSNSMORecord *pBill = (Org_SGSNSMORecord*)pValue;
	int nLen = 0;
	int nTempLen = 0;

	//ASSERT((BOOL)(pValue && pBuffer));
	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != Org_CallEventRecordType_sgsnSMORecord)
		return 0;

	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;

	//recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R98_760_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R98_760_Recorde_Type_EnumInfo;
    nTempLen = EnumToStr(&EnumInfo,
                         pBill->recordType,
                         &pTempBuffer[nLen],
                         nLength - nLen);

    if (nTempLen)
        nLen += nTempLen;
    else
    {
        return 0;
    }
    pTempBuffer[nLen++] = '^';

	//servedIMSI
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI, 
							&pTempBuffer[nLen], SMOBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//servedIMEI
	if (pBill->bit_mask & Org_SGSNSMORecord_servedIMEI_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMEI,
							&pTempBuffer[nLen], SMOBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//servedMSISDN
	if (pBill->bit_mask & Org_SGSNSMORecord_servedMSISDN_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedMSISDN, 
								&pTempBuffer[nLen], SMOBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//msNetworkCapability
	nLen += HexOctecToStr((LPOCTECSTRING)&pBill->msNetworkCapability, &pTempBuffer[nLen], 
						SMOBUFFERLENGTH - nLen, "0x", FALSE);							
	pTempBuffer[nLen++] = '^';

	//serviceCentre
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->serviceCentre, &pTempBuffer[nLen], 
							SMOBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//recordingEntity
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->recordingEntity, &pTempBuffer[nLen],
							SMOBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//locationArea
	if (pBill->bit_mask & Org_SGSNSMORecord_locationArea_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationArea, &pTempBuffer[nLen],
							SMOBUFFERLENGTH - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//routingArea
	if (pBill->bit_mask & Org_SGSNSMORecord_routingArea_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->routingArea, &pTempBuffer[nLen],
							SMOBUFFERLENGTH - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//cellIdentity
	if (pBill->bit_mask & Org_SGSNSMORecord_cellIdentity_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->cellIdentity, &pTempBuffer[nLen],
							SMOBUFFERLENGTH - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//messageReference
	nLen += NumToDecStr((long)pBill->messageReference.value[0], &pTempBuffer[nLen], 
							SMOBUFFERLENGTH - nLen, FALSE);
	pTempBuffer[nLen++] = '^';

	//originationTime
	nLen += TimeStampToStr((LPTIMESTAMP)&pBill->originationTime, &pTempBuffer[nLen],
							SMOBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//smsResult
	if (pBill->bit_mask & Org_SGSNSMORecord_smsResult_present)
	{
		nLen += Gprs_DiagnosticsToStr(&pBill->smsResult, &pTempBuffer[nLen], 
									SMOBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//recordExtensions
	if (pBill->bit_mask & Org_SGSNSMORecord_recordExtensions_present)
	{
		nLen += Gprs_RecordExtensionsToStr(pBill->recordExtensions, &pTempBuffer[nLen], 
										SMOBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//nodeID
	if (pBill->bit_mask & Org_SGSNSMORecord_nodeID_present )
	{
		strcpy(&pTempBuffer[nLen], pBill->nodeID);
		nLen += strlen(pBill->nodeID);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//localSequenceNumber
	if (pBill->bit_mask & Org_SGSNSMORecord_localSequenceNumber_present)
	{
		nLen += NumToDecStr(pBill->localSequenceNumber, &pTempBuffer[nLen], 
							SMOBUFFERLENGTH - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	pTempBuffer[nLen] = 0;

	if (nLen >= nLength)
		nLen = 0;

	return nLen;
}


/******************************************************
// Function name: 	GetTextValueOfGPRSSGSNSMT
// Author:          maimaoshi
// Date:            2002-03-19
// Description:     处理S-SMT-CDR话单浏览解释的函数(15个字段)
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//2002-03-19       maimaoshi
******************************************************/
int GetTextValueOfGPRSSGSNSMT(char *pValue, char *pBuffer, int nLength)
{
#define SMTBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	Org_SGSNSMTRecord *pBill = (Org_SGSNSMTRecord*)pValue;
	int nLen = 0;
	int nTempLen = 0;

	//ASSERT((BOOL)(pValue && pBuffer));
	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != Org_CallEventRecordType_sgsnSMTRecord)
		return 0;

	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;

	//recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R98_760_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R98_760_Recorde_Type_EnumInfo;
    nTempLen = EnumToStr(&EnumInfo,
                         pBill->recordType,
                         &pTempBuffer[nLen],
                         nLength - nLen);

    if (nTempLen)
        nLen += nTempLen;
    else
    {
        return 0;
    }
    pTempBuffer[nLen++] = '^';

	//servedIMSI
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI,
							&pTempBuffer[nLen], SMTBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//servedIMEI
	if (pBill->bit_mask & Org_SGSNSMTRecord_servedIMEI_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMEI,
							&pTempBuffer[nLen], SMTBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//servedMSISDN
	if (pBill->bit_mask & Org_SGSNSMTRecord_servedMSISDN_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedMSISDN, 
								&pTempBuffer[nLen], SMTBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//msNetworkCapability
	nLen += HexOctecToStr((LPOCTECSTRING)&pBill->msNetworkCapability, &pTempBuffer[nLen], 
						SMTBUFFERLENGTH - nLen, "0x", FALSE);							
	pTempBuffer[nLen++] = '^';

	//serviceCentre
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->serviceCentre, &pTempBuffer[nLen], 
							SMTBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//recordingEntity
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->recordingEntity, &pTempBuffer[nLen],
							SMTBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//locationArea
	if (pBill->bit_mask & Org_SGSNSMTRecord_locationArea_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationArea, &pTempBuffer[nLen],
							SMTBUFFERLENGTH - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//routingArea
	if (pBill->bit_mask & Org_SGSNSMTRecord_routingArea_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->routingArea, &pTempBuffer[nLen],
							SMTBUFFERLENGTH - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//cellIdentity
	if (pBill->bit_mask & Org_SGSNSMTRecord_cellIdentity_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->cellIdentity, &pTempBuffer[nLen],
							SMTBUFFERLENGTH - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//originationTime
	nLen += TimeStampToStr((LPTIMESTAMP)&pBill->originationTime, &pTempBuffer[nLen],
							SMTBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//smsResult
	if (pBill->bit_mask & Org_SGSNSMTRecord_smsResult_present)
	{
		nLen += Gprs_DiagnosticsToStr(&pBill->smsResult, &pTempBuffer[nLen], 
									SMTBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//recordExtensions
	if (pBill->bit_mask & Org_SGSNSMTRecord_recordExtensions_present)
	{
		nLen += Gprs_RecordExtensionsToStr(pBill->recordExtensions, &pTempBuffer[nLen], 
										SMTBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//nodeID
	if (pBill->bit_mask & Org_SGSNSMTRecord_nodeID_present)
	{
		strcpy(&pTempBuffer[nLen], pBill->nodeID);
		nLen += strlen(pBill->nodeID);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//localSequenceNumber
	if (pBill->bit_mask & Org_SGSNSMTRecord_localSequenceNumber_present)
	{
		nLen += NumToDecStr(pBill->localSequenceNumber, &pTempBuffer[nLen], 
							SMTBUFFERLENGTH - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	pTempBuffer[nLen] = 0;

	if (nLen >= nLength)
		nLen = 0;

	return nLen;
}


/******************************************************
// Function name: 	GetTextValueOfGPRSLARGESGSNPDP
// Author:          maimaoshi
// Date:            2002-03-19
// Description:     处理合并后S-CDR话单浏览解释的函数(27个字段)
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int GetTextValueOfGPRSLARGESGSNPDP(char *pValue, char *pBuffer, int nLength)
{
	Fin_SGSNPDPRecord *pBill = (Fin_SGSNPDPRecord*)pValue;
	int nLen = 0;
    int nTempLen = 0;

	//ASSERT((BOOL)(pValue && pBuffer));
	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != Fin_CallEventRecordType_sgsnPDPRecord)
		return 0;

	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;
	
	//recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R98_760_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R98_760_Recorde_Type_EnumInfo;
    nTempLen = EnumToStr(&EnumInfo,
                         pBill->recordType,
                         &pTempBuffer[nLen],
                         nLength - nLen);

    if (nTempLen)
        nLen += nTempLen;
    else
    {
        return 0;
    }
    pTempBuffer[nLen++] = '^';

	//networkInitiation
	if (pBill->bit_mask & Fin_SGSNPDPRecord_networkInitiation_present)
	{
		nLen += BoolToStr(pBill->networkInitiation, &pTempBuffer[nLen], 
			SCDRBUFFERLENGTH - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//anonymousAccessIndicator
	if (pBill->bit_mask & Fin_SGSNPDPRecord_anonymousAccessIndicator_present)
	{
		nLen += BoolToStr(pBill->anonymousAccessIndicator, &pTempBuffer[nLen], 
			SCDRBUFFERLENGTH - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//servedIMSI
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI, 
							&pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//servedIMEI
	if (pBill->bit_mask & Fin_SGSNPDPRecord_servedIMEI_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMEI,
							&pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	
	//servedMSISDN	
	if (pBill->bit_mask & Fin_SGSNPDPRecord_servedMSISDN_present)
	{
	    nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedMSISDN, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	
	pTempBuffer[nLen++] = '^';


	//sgsnAddress$$$，是链表,
	nLen += _GPRSSGSNAddrToStr((struct Org__seqof2*)pBill->sgsnAddress, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//msNetworkCapability
	if (pBill->bit_mask & Fin_SGSNPDPRecord_msNetworkCapability_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->msNetworkCapability, &pTempBuffer[nLen], 
							SCDRBUFFERLENGTH - nLen, "0x", FALSE);							
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
	
	//routingArea
	if (pBill->bit_mask & Fin_SGSNPDPRecord_routingArea_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->routingArea, &pTempBuffer[nLen],
							SCDRBUFFERLENGTH - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//locationAreaCode
	if (pBill->bit_mask & Fin_SGSNPDPRecord_locationAreaCode_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationAreaCode, &pTempBuffer[nLen],
							SCDRBUFFERLENGTH - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//cellIdentity
	if (pBill->bit_mask & Fin_SGSNPDPRecord_cellIdentity_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->cellIdentity, &pTempBuffer[nLen],
							SCDRBUFFERLENGTH - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//chargingID
	nLen += NumToDecStr(pBill->chargingID, &pTempBuffer[nLen], 
						SCDRBUFFERLENGTH - nLen, FALSE);
	pTempBuffer[nLen++] = '^';

	//ggsnAddressUsed$$$,Org_IPAddress和Fin_IPAddress是一个相同的结构
	nLen += _GPRSIPAddrToStr((Org_IPAddress*)&pBill->ggsnAddressUsed, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//accessPointNameNI
	strcpy(&pTempBuffer[nLen], pBill->accessPointNameNI);
	nLen += strlen(pBill->accessPointNameNI);
	pTempBuffer[nLen++] = '^';

	//pdpType
	nLen += HexOctecToStr((LPOCTECSTRING)&pBill->pdpType, &pTempBuffer[nLen],
						SCDRBUFFERLENGTH - nLen, "0x", FALSE);
	pTempBuffer[nLen++] = '^';

	//servedPDPAddress$$$,Org_PDPAddress和Fin_PDPAddress是一个相同的结构
	nLen += _GPRSPDPAddrToStr((Org_PDPAddress*)&pBill->servedPDPAddress, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//listOfTrafficVolumes$$$,struct Fin__seqof2_和struct Org__seqof1_是相同的
	nLen += Gprs_ListOfTrafficVolumesToStr((struct Org__seqof1_*)pBill->listOfTrafficVolumes, &pTempBuffer[nLen], 
										SCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//recordOpeningTime
	nLen += TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime, &pTempBuffer[nLen],
							SCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//duration
	nLen += NumToDecStr(pBill->duration, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen, FALSE);
	pTempBuffer[nLen++] = '^';

	//sgsnChange
	if (pBill->bit_mask & Fin_SGSNPDPRecord_sgsnChange_present)
	{
		nLen += BoolToStr(pBill->sgsnChange, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//causeForRecClosing
	EnumInfo.nCount = GPRSCAUSEFORRECCLOSINGENUNINFO_LENGTH;
	EnumInfo.pInfoTable = GprsCauseForRecClosingEnumInfo;
	nTempLen = EnumToStr(&EnumInfo, pBill->causeForRecClosing,&pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);

    if (nTempLen)
		nLen += nTempLen;
	else
	{
		strcpy(&pTempBuffer[nLen], "Unknown reason)");
		nLen += strlen("Unknown reason");
	}
	pTempBuffer[nLen++] = '^';

	//recordSequenceNumber$$$,是struct Fin__seqof3结构链表
	if (pBill->bit_mask & Fin_SGSNPDPRecord_recordSequenceNumber_present)
	{
		nLen += CvtRecordSequenceNumbertoStr(pBill->recordSequenceNumber, 
		            	&pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//nodeID
	if (pBill->bit_mask & Fin_SGSNPDPRecord_nodeID_present)
	{
		strcpy(&pTempBuffer[nLen], pBill->nodeID);
		nLen += strlen(pBill->nodeID);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//recordExtensions$$$,Org_ManagementExtensions_和Fin_ManagementExtensions_相同
	if (pBill->bit_mask & Fin_SGSNPDPRecord_recordExtensions_present)
	{
		nLen += Gprs_RecordExtensionsToStr((Org_ManagementExtensions_*)pBill->recordExtensions, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

        //added by maimaoshi,2003-04-12
	//apnSelectionMode
	if(pBill->bit_mask & Fin_SGSNPDPRecord_apnSelectionMode_present)
	{
		EnumInfo.nCount = GprsApnSelectionModeEnumInfo_LENGTH;
		EnumInfo.pInfoTable = GprsApnSelectionModeEnumInfo;
		nLen += EnumToStr(&EnumInfo, pBill->apnSelectionMode, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;		
	}	
	pTempBuffer[nLen++] = '^';
	//end

	//accessPointNameOI
	strcpy(&pTempBuffer[nLen], pBill->accessPointNameOI);
	nLen += strlen(pBill->accessPointNameOI);
	pTempBuffer[nLen++] = '^';

	//consolidationResult
	EnumInfo.nCount = GprsConsolidationResultEnumInfo_LENGTH;
	EnumInfo.pInfoTable = GprsConsolidationResultEnumInfo;
	nLen += EnumToStr(&EnumInfo, pBill->consolidationResult, 
					&pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	pTempBuffer[nLen] = 0;

	if (nLen >= nLength)
		nLen = 0;

	return nLen;
}


/******************************************************
// Function name: 	GetTextValueOfGPRSLARGEGGSNPDP
// Author:          maimaoshi
// Date:            01-5-23
// Description:     处理合并后G-CDR话单浏览解释的函数(22个字段)
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int GetTextValueOfGPRSLARGEGGSNPDP(char *pValue, char *pBuffer, int nLength)
{
	Fin_GGSNPDPRecord *pBill = (Fin_GGSNPDPRecord*)pValue;
	int nLen = 0;
    int nTempLen = 0;

	//ASSERT((BOOL)(pValue && pBuffer));
	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != Fin_CallEventRecordType_ggsnPDPRecord)
		return 0;

	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;
	
	//recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R98_760_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R98_760_Recorde_Type_EnumInfo;
    nTempLen = EnumToStr(&EnumInfo,
                         pBill->recordType,
                         &pTempBuffer[nLen],
                         nLength - nLen);

    if (nTempLen)
        nLen += nTempLen;
    else
    {
        return 0;
    }
    pTempBuffer[nLen++] = '^';

	//networkInitiation
	if (pBill->bit_mask & Fin_GGSNPDPRecord_networkInitiation_present)
	{
		nLen += BoolToStr(pBill->networkInitiation, &pTempBuffer[nLen], 
							GCDRBUFFERLENGTH - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//anonymousAccessIndicator
	if (pBill->bit_mask & Fin_GGSNPDPRecord_anonymousAccessIndicator_present)
	{
		nLen += BoolToStr(pBill->anonymousAccessIndicator, &pTempBuffer[nLen], 
			SCDRBUFFERLENGTH - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//servedIMSI
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI, 
							&pTempBuffer[nLen], GCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//servedMSISDN
	if (pBill->bit_mask & Fin_GGSNPDPRecord_servedMSISDN_present)
	{
	    nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedMSISDN, &pTempBuffer[nLen], 
									    GCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//ggsnAddress$$$,Org_IPAddress和Fin_IPAddress是相同的结构
	nLen += _GPRSIPAddrToStr((Org_IPAddress*)&pBill->ggsnAddress, &pTempBuffer[nLen], GCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//chargingID
	nLen += NumToDecStr(pBill->chargingID, &pTempBuffer[nLen], 
						GCDRBUFFERLENGTH - nLen, FALSE);
	pTempBuffer[nLen++] = '^';

	//sgsnAddress$$$,是链表    
	nLen += _GPRSSGSNAddrToStr((struct Org__seqof2*)pBill->sgsnAddress, &pTempBuffer[nLen], GCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//accessPointNameNI
	strcpy(&pTempBuffer[nLen], pBill->accessPointNameNI);
	nLen += strlen(pBill->accessPointNameNI);
	pTempBuffer[nLen++] = '^';

	//pdpType
	nLen += HexOctecToStr((LPOCTECSTRING)&pBill->pdpType, &pTempBuffer[nLen],
						GCDRBUFFERLENGTH - nLen, "0x", FALSE);
	pTempBuffer[nLen++] = '^';

	//servedPDPAddress
	nLen += _GPRSPDPAddrToStr((Org_PDPAddress*)&pBill->servedPDPAddress, &pTempBuffer[nLen], 
							GCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//remotePDPAddress$$$，struct Org__seqof3
	if (pBill->bit_mask & Fin_remotePDPAddress_present)
	{

		nLen += _GPRSREMOTEPDPAddrToStr((struct Org__seqof3*)pBill->remotePDPAddress, &pTempBuffer[nLen], 
									GCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//dynamicAddressFlag
	if (pBill->bit_mask & Fin_dynamicAddressFlag_present)
	{
		nLen += BoolToStr(pBill->dynamicAddressFlag, &pTempBuffer[nLen], 
							GCDRBUFFERLENGTH - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//listOfTrafficVolumes
	nLen += Gprs_ListOfTrafficVolumesToStr((struct Org__seqof1_*)pBill->listOfTrafficVolumes, &pTempBuffer[nLen], 
										GCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//recordOpeningTime
	nLen += TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime, &pTempBuffer[nLen],
							GCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//duration
	nLen += NumToDecStr(pBill->duration, &pTempBuffer[nLen], 
						GCDRBUFFERLENGTH - nLen, FALSE);
	pTempBuffer[nLen++] = '^';

	//causeForRecClosing
	EnumInfo.nCount = GPRSCAUSEFORRECCLOSINGENUNINFO_LENGTH;
	EnumInfo.pInfoTable = GprsCauseForRecClosingEnumInfo;
	nTempLen = EnumToStr(&EnumInfo, pBill->causeForRecClosing,&pTempBuffer[nLen], 
						GCDRBUFFERLENGTH - nLen);

	if (nTempLen)
		nLen += nTempLen;
	else
	{
		strcpy(&pTempBuffer[nLen], "Unknown reason");
		nLen += strlen("Unknown reason");
	}
	pTempBuffer[nLen++] = '^';

	//recordSequenceNumber$$$,是链表
	if (pBill->bit_mask & Fin_GGSNPDPRecord_recordSequenceNumber_present)
	{
		nLen += RecordSequenceNumbertoStr(&pBill->recordSequenceNumber, 
		            	&pTempBuffer[nLen], GCDRBUFFERLENGTH - nLen);
				
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//nodeID
	if (pBill->bit_mask & Fin_GGSNPDPRecord_nodeID_present)
	{
		strcpy(&pTempBuffer[nLen], pBill->nodeID);
		nLen += strlen(pBill->nodeID);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//recordExtensions Org_ManagementExtensions_与Fin_ManagementExtensions_结构相同
	if (pBill->bit_mask & Fin_GGSNPDPRecord_recordExtensions_present)
	{
		switch (g_nCBB_OI)
		{			    
		case CBB_OPERATOR_ID_CMCC:
		    nLen += CMCC_R98_GCDRRecordExtensionsToStr((Org_ManagementExtensions_*)pBill->recordExtensions, 
                        &pTempBuffer[nLen], GCDRBUFFERLENGTH - nLen);
		    break;
			    
		default:
    		nLen += R98_GCDRRecordExtensionsToStr((Org_ManagementExtensions_*)pBill->recordExtensions, 
                       &pTempBuffer[nLen], GCDRBUFFERLENGTH - nLen);
            break;			    
        }
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	
	//added by maimaoshi,2003-04-12
	//apnSelectionMode
	if(pBill->bit_mask & Fin_GGSNPDPRecord_apnSelectionMode_present)
	{
		EnumInfo.nCount = GprsApnSelectionModeEnumInfo_LENGTH;
		EnumInfo.pInfoTable = GprsApnSelectionModeEnumInfo;
		nLen += EnumToStr(&EnumInfo, pBill->apnSelectionMode, &pTempBuffer[nLen], 
								GCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;		
	}
	pTempBuffer[nLen++] = '^';
	//end

	//consolidationResult
	EnumInfo.nCount = GprsConsolidationResultEnumInfo_LENGTH;
	EnumInfo.pInfoTable = GprsConsolidationResultEnumInfo;
	nLen += EnumToStr(&EnumInfo, pBill->consolidationResult, 
					&pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	pTempBuffer[nLen] = 0;

	if (nLen >= nLength)
		nLen = 0;

	return nLen;
}


/******************************************************
// Function name: 	GetTextValueOfGPRSLARGESGSNMM
// Author:          maimaoshi
// Date:            2002-03-19
// Description:     处理合并后M-CDR话单浏览解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int GetTextValueOfGPRSLARGESGSNMM(char *pValue, char *pBuffer, int nLength)
{
	int nLen = 0;
    int nTempLen = 0;

	Fin_SGSNMMRecord *pBill = (Fin_SGSNMMRecord*)pValue;

	//ASSERT((BOOL)(pValue && pBuffer));
	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != Fin_CallEventRecordType_sgsnMMRecord)
		return 0;

	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;

	//recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R98_760_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R98_760_Recorde_Type_EnumInfo;
    nTempLen = EnumToStr(&EnumInfo,
                         pBill->recordType,
                         &pTempBuffer[nLen],
                         nLength - nLen);

    if (nTempLen)
        nLen += nTempLen;
    else
    {
        return 0;
    }
    pTempBuffer[nLen++] = '^';

	//servedIMSI
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI, 
							&pTempBuffer[nLen], MCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//servedIMEI
	if (pBill->bit_mask & Fin_SGSNMMRecord_servedIMEI_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMEI,
							&pTempBuffer[nLen], MCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//sgsnAddress
	nLen += _GPRSIPAddrToStr((Org_IPAddress*)&pBill->sgsnAddress, &pTempBuffer[nLen], 
							MCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//msNetworkCapability
	if (pBill->bit_mask & Fin_SGSNMMRecord_msNetworkCapability_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->msNetworkCapability, &pTempBuffer[nLen], 
							MCDRBUFFERLENGTH - nLen, "0x", FALSE);							
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//routingArea
	if (pBill->bit_mask & Fin_SGSNMMRecord_routingArea_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->routingArea, &pTempBuffer[nLen],
								MCDRBUFFERLENGTH - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//locationAreaCode
	if (pBill->bit_mask & Fin_SGSNMMRecord_locationAreaCode_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationAreaCode, &pTempBuffer[nLen],
							MCDRBUFFERLENGTH - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//cellIdentity
	if (pBill->bit_mask & Fin_SGSNMMRecord_cellIdentity_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->cellIdentity, &pTempBuffer[nLen],
							MCDRBUFFERLENGTH - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

  
	//changeLocation
	if (pBill->bit_mask & Fin_changeLocation_present)
	{
		nLen += Gprs_ChangeLocationsToStr((struct Org__seqof4*)pBill->changeLocation, 
									&pTempBuffer[nLen], MCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
   
	//recordOpeningTime
	nLen += TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime, &pTempBuffer[nLen],
							MCDRBUFFERLENGTH - nLen);
	pTempBuffer[nLen++] = '^';

	//duration
	if (pBill->bit_mask & Fin_duration_present)
	{
		nLen += NumToDecStr(pBill->duration, &pTempBuffer[nLen], 
							MCDRBUFFERLENGTH - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//sgsnChange
	if (pBill->bit_mask & Fin_SGSNMMRecord_sgsnChange_present)
	{
		nLen += BoolToStr(pBill->sgsnChange, &pTempBuffer[nLen], 
							MCDRBUFFERLENGTH - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//causeForRecClosing
	EnumInfo.nCount = GPRSCAUSEFORRECCLOSINGENUNINFO_LENGTH;
	EnumInfo.pInfoTable = GprsCauseForRecClosingEnumInfo;
	nTempLen = EnumToStr(&EnumInfo, pBill->causeForRecClosing,
						&pTempBuffer[nLen], MCDRBUFFERLENGTH - nLen);

	if (nTempLen)
		nLen += nTempLen;
	else
	{
		strcpy(&pTempBuffer[nLen], "Unknown reason");
		nLen += strlen("Unknown reason");
	}
	pTempBuffer[nLen++] = '^';

	//diagnostics
	if (pBill->bit_mask & Fin_diagnostics_present)
	{
		nLen += Gprs_DiagnosticsToStr((Org_Diagnostics *)&pBill->diagnostics, &pTempBuffer[nLen], 
									MCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//recordSequenceNumber
	if (pBill->bit_mask & Fin_SGSNMMRecord_recordSequenceNumber_present)
	{
		nLen += NumToDecStr(pBill->recordSequenceNumber, &pTempBuffer[nLen], 
							MCDRBUFFERLENGTH - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//nodeID
	if (pBill->bit_mask & Fin_SGSNMMRecord_nodeID_present)
	{
		strcpy(&pTempBuffer[nLen], pBill->nodeID);
		nLen += strlen(pBill->nodeID);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

 
	//recordExtensions
	if (pBill->bit_mask & Fin_SGSNMMRecord_recordExtensions_present)
	{
		nLen += Gprs_RecordExtensionsToStr((Org_ManagementExtensions_ *)pBill->recordExtensions, &pTempBuffer[nLen], 
										MCDRBUFFERLENGTH - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
 
	//localSequenceNumber
	if (pBill->bit_mask & Fin_SGSNMMRecord_localSequenceNumber_present)
	{
		nLen += NumToDecStr(pBill->localSequenceNumber, &pTempBuffer[nLen], 
							MCDRBUFFERLENGTH - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	pTempBuffer[nLen] = 0;


	if (nLen >= nLength)
		nLen = 0;

	return nLen;
	
}

//////////////////////////////////////////////////////////////////////////////

/******************************************************
// Function name: _GPRSIPAddrToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   将Org_IPAddress结构的数据转换成字符串
// Input:
//       arg1 :Org_IPAddress *pIPAddr	待转换数据
//       arg2 :char *pBuffer			存放返回字符串的数据区
//       arg3 :int nLength				数据区可用长度
// Return value: int					转换后的字符串长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int _GPRSIPAddrToStr(Org_IPAddress *pIPAddr, char *pBuffer, int nLength)
{
	int nLen;

	//ASSERT((BOOL)pIPAddr);
	if (pIPAddr == NULL)
	{
	     return 0;
	}

	pBuffer[0] = 0;

	switch (pIPAddr->choice)	//判断是Bin还是Text类型
	{
	case Org_iPBinaryAddress_chosen:	//Bin类型
		{
			switch (pIPAddr->u.iPBinaryAddress.choice)	//判断是IPV4还是IPV6
			{
			case Org_iPBinV4Address_chosen:			//IPV4
				{
					nLen = OctecToIPAddrStr((LPOCTECSTRING)&pIPAddr->u.iPBinaryAddress.u.iPBinV4Address, 
											pBuffer, nLength, BINIPV4_STYLE);
				}
				break;
			case Org_iPBinV6Address_chosen:			//IPV6
				{
					nLen = OctecToIPAddrStr((LPOCTECSTRING)&pIPAddr->u.iPBinaryAddress.u.iPBinV6Address,
											pBuffer, nLength, BINIPV6_STYLE);
				}
				break;
			default:
				{
					nLen = 0;
				}
				break;
			}
		}
		break;
	case Org_iPTextRepresentedAddress_chosen:	//Text类型
		{
			switch (pIPAddr->u.iPTextRepresentedAddress.choice)	//判断是IPV4还是IPV6
			{
			case Org_iPTextV4Address_chosen:			//IPV4
				{
					if (nLength < 16)
						return 0;
					memcpy(pBuffer, pIPAddr->u.iPTextRepresentedAddress.u.iPTextV4Address, 15);
					pBuffer[15] = 0;
					nLen = strlen(pBuffer);
				}
				break;
			case Org_iPTextV6Address_chosen:			//IPV6
				{
					if (nLength < 46)
						return 0;
					memcpy(pBuffer, pIPAddr->u.iPTextRepresentedAddress.u.iPTextV6Address, 45);
					pBuffer[45] = 0;					
					nLen = strlen(pBuffer);
				}
				break;
			default:
				{
					nLen = 0;
				}
				break;
			}
		}
		break;
	default:
		{
			nLen = 0;
		}
		break;
	}

	return nLen;
}


/******************************************************
// Function name: _GPRSPDPAddrToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   将Org_PDPAddress结构的数据转换成字符串
// Input:
//       arg1 :Org_PDPAddress *pPDPAddress	待转换的数据	
//       arg2 :char *pBuffer					存放返回字符串的数据区
//       arg3 :int nLength						数据区可用长度
// Return value: int							转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int _GPRSPDPAddrToStr(Org_PDPAddress *pPDPAddress, char *pBuffer, int nLength)
{
	int nLen = 0;

	//ASSERT((BOOL)pPDPAddress);
	if (pPDPAddress == NULL)
	{
	     return 0;
	}

	pBuffer[0] = 0;

	switch (pPDPAddress->choice)
	{
	case Org_iPAddress_chosen:
		{
			nLen = _GPRSIPAddrToStr(&pPDPAddress->u.iPAddress, pBuffer, nLength);
		}
		break;
	case Org_eTSIAddress_chosen:
		{
			nLen = OctecToStr((LPOCTECSTRING)&pPDPAddress->u.eTSIAddress, pBuffer, nLength);
		}
		break;
	default:
		nLen = 0;
		break;
	}

	return nLen;
}

/******************************************************
// Function name: _GPRSSGSNAddrToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   将Org__seqof2结构的数据转换成字符串

                  struct Org__seqof2 {
				  struct Org__seqof2 *next;
				  Org_GSNAddress  value;
				  } *sgsnAddress;

// Input:
//       arg1 :struct Org__seqof2 *pData        待转换的数据	
//       arg2 :char *pBuffer					存放返回字符串的数据区
//       arg3 :int nLength						数据区可用长度
// Return value: int							转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int _GPRSSGSNAddrToStr(Org__seqof2 *pData, char *pBuffer, int nLength)
{
	int nLen = 0, nTempLen = 0;
	Org__seqof2* pNode = NULL;
	Org_GSNAddress*     pAddress = NULL;

	//ASSERT((BOOL)(pBuffer && pData));
	if ((pBuffer == NULL) || (pData == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;	

	pNode = pData;

	while (pNode != NULL)
	{
		if (nLength > nLen)
			pBuffer[nLen++] = '{';
		else
		{
			pBuffer[0] = 0;			
			return 0;
		}
		
		pAddress = &pNode->value;

		nTempLen = _GPRSIPAddrToStr(pAddress, &pBuffer[nLen], nLength - nLen);
		if (nTempLen)
			nLen += nTempLen;
		else
		{
			pBuffer[0] = 0;
			return 0;
		}

		if (nLength > nLen)
			pBuffer[nLen++] = '}';
		else
		{
			pBuffer[0] = 0;
			return 0;
		}
		
		//前进到下一个接点进行解释
		pNode = pNode->next;

	}

	if (nLength > nLen)
		pBuffer[nLen] = 0;
	else
	{
		pBuffer[0] = 0;
		nLen = 0;
	}
	return nLen;

}

/******************************************************
// Function name: _GPRSREMOTEPDPAddrToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   将struct Org__seqof3结构的数据转换成字符串

                  struct Org__seqof3 {
				  struct Org__seqof3 *next;
				  Org_PDPAddress  value;
				  } *remotePDPAddress

// Input:
//       arg1 :struct Org__seqof3 *pData	    待转换的数据	
//       arg2 :char *pBuffer					存放返回字符串的数据区
//       arg3 :int nLength						数据区可用长度
// Return value: int							转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int _GPRSREMOTEPDPAddrToStr(Org__seqof3 *pData, char *pBuffer, int nLength)
{
	int nLen = 0, nTempLen = 0;
	Org__seqof3* pNode = NULL;
	Org_PDPAddress*     pAddress = NULL;

	//ASSERT((BOOL)(pBuffer && pData));
	if ((pBuffer == NULL) || (pData == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;	

	pNode = pData;

	while (pNode != NULL)
	{
		if (nLength > nLen)
			pBuffer[nLen++] = '{';
		else
		{
			pBuffer[0] = 0;			
			return 0;
		}
		
		pAddress = &pNode->value;

		nTempLen = _GPRSPDPAddrToStr(pAddress, &pBuffer[nLen], nLength - nLen);
		if (nTempLen)
			nLen += nTempLen;
		else
		{
			pBuffer[0] = 0;
			return 0;
		}

		if (nLength > nLen)
			pBuffer[nLen++] = '}';
		else
		{
			pBuffer[0] = 0;
			return 0;
		}
		
		//前进到下一个接点进行解释
		pNode = pNode->next;

	}

	if (nLength > nLen)
		pBuffer[nLen] = 0;
	else
	{
		pBuffer[0] = 0;
		nLen = 0;
	}
	return nLen;

}

/******************************************************
// Function name: Gprs_ListOfTrafficVolumesToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   转换Org__seqof1_类型的数据为字符串
                
				  typedef struct Org__seqof1_ {
			      struct Org__seqof1_ *next;
				  Org_ChangeOfCharCondition value;
				  } *Org__seqof1;				
// Input:
//       arg1 :struct Org__seqof1_ *pList	待转换的数据
//       arg2 :char *pBuffer		转换后数据存放的Buffer
//       arg3 :int nLength			Buffer的可用长度
// Return value: int				转换后字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int Gprs_ListOfTrafficVolumesToStr(Org__seqof1_ *pList, char *pBuffer, int nLength)
{

	int nLen = 0;
	Org__seqof1_ *pNode = NULL;
	Org_ChangeOfCharCondition *pItem;

	ENUMINFO EnumInfo;
	EnumInfo.nCount = GprsChangeConditionEnumInfo_LENGTH; //3
	EnumInfo.pInfoTable = GprsChangeConditionEnumInfo;
	
	//ASSERT((BOOL)(pBuffer && pList));
	if ((pBuffer == NULL) || (pList == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;	

	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;

	pNode = pList;

	pTempBuffer[nLen++] = '{';
	while (pNode != NULL)
	{
		//取出值内容
		pItem = &pNode->value;

		pTempBuffer[nLen++] = '{';

		if (pItem->bit_mask & Org_qosRequested_present)
		{
			strcpy(&pTempBuffer[nLen], "QoSRequested:");
			nLen += strlen("QoSRequested:");
			
			nLen += _GprsQosInformationToStr(&pItem->qosRequested, &pTempBuffer[nLen], 
											nLength - nLen);
			pTempBuffer[nLen++] = ' ';
		}	

		if (pItem->bit_mask & Org_qosNegotiated_present)
		{
			strcpy(&pTempBuffer[nLen], "QoSNegotiated:");
			nLen += strlen("QoSNegotiated:");
			
			nLen += _GprsQosInformationToStr(&pItem->qosNegotiated, &pTempBuffer[nLen], 
											nLength - nLen);
			pTempBuffer[nLen++] = ' ';
		}
		
		strcpy(&pTempBuffer[nLen], "DataVolumeGPRSUplink:");
		nLen += strlen("DataVolumeGPRSUplink:");
		nLen += NumToHexStr(pItem->dataVolumeGPRSUplink, &pTempBuffer[nLen], 
							nLength - nLen, "0x");
		pTempBuffer[nLen++] = ' ';

		strcpy(&pTempBuffer[nLen], "DataVolumeGPRSDownlink:");
		nLen += strlen("DataVolumeGPRSDownlink:");
		nLen += NumToHexStr(pItem->dataVolumeGPRSDownlink, &pTempBuffer[nLen], 
							nLength - nLen, "0x");
		pTempBuffer[nLen++] = ' ';

		strcpy(&pTempBuffer[nLen], "ChangeCondition:");
		nLen += strlen("ChangeCondition:");
		nLen += EnumToStr(&EnumInfo, pItem->changeCondition, &pTempBuffer[nLen], 
							nLength - nLen);
		pTempBuffer[nLen++] = ' ';

		strcpy(&pTempBuffer[nLen], "ChangeTime:");
		nLen += strlen("ChangeTime:");
		nLen += TimeStampToStr((LPTIMESTAMP)&pItem->changeTime, &pTempBuffer[nLen], 
								nLength - nLen);
		pTempBuffer[nLen++] = '}';
		
		//进入下一个接点进行解释
		pNode = pNode->next;
	}
	pTempBuffer[nLen++] = '}';

	pTempBuffer[nLen] = 0;
	if(nLen >= nLength)
	{
		nLen = 0;
	}

	return nLen;

}


/******************************************************
// Function name: _GprsQosInformationToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   转换Org_QoSInformation类型的数据为字符串
// Input:
//       arg1 :Org_QoSInformation *pQosInfo	待转换的数据
//       arg2 :char *pBuffer		转换后数据存放的Buffer
//       arg3 :int nLength			Buffer的可用长度
// Return value: int				转换后字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int _GprsQosInformationToStr(Org_QoSInformation *pQosInfo, char *pBuffer, int nLength)
{
#define QOSBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	int nLen = 0;

	//ASSERT((BOOL)(pBuffer && pQosInfo));
	if ((pBuffer == NULL) || (pQosInfo == NULL))
	{
	     return 0;
	}


	char *pTempBuffer = pBuffer;

	ENUMINFO EnumInfo;

	if (!pTempBuffer)
		return 0;
	pTempBuffer[0] = 0;

	strcpy(pTempBuffer, "ReliabClass(");
	nLen = strlen("ReliabClass(");	
	EnumInfo.nCount = GprsQosReliabilityEnumInfo_LENGTH; //6
	EnumInfo.pInfoTable = GprsQosReliabilityEnumInfo;
	nLen += EnumToStr(&EnumInfo, pQosInfo->reliability, &pTempBuffer[nLen], (QOSBUFFERLENGTH - nLen));
		
	strcpy(&pTempBuffer[nLen], ")DelayClass(");
	nLen += strlen(")DelayClass(");
	EnumInfo.nCount = GprsQoSDelayEnumInfo_LENGTH; //4
	EnumInfo.pInfoTable = GprsQoSDelayEnumInfo;
	nLen += EnumToStr(&EnumInfo, pQosInfo->delay, &pTempBuffer[nLen], (QOSBUFFERLENGTH - nLen));
		
	strcpy(&pTempBuffer[nLen], ")PrecedClass(");
	nLen += strlen(")PrecedClass(");
	EnumInfo.nCount = GprsQoSPrecedenceEnumInfo_LENGTH; //4
	EnumInfo.pInfoTable = GprsQoSPrecedenceEnumInfo;
	nLen += EnumToStr(&EnumInfo, pQosInfo->precedence, &pTempBuffer[nLen], (QOSBUFFERLENGTH - nLen));

	strcpy(&pTempBuffer[nLen], ")PeakThrput(");
	nLen += strlen(")PeakThrput(");
	EnumInfo.nCount = GprsQoSPeakThroughputEnumInfo_LENGTH;//10
	EnumInfo.pInfoTable = GprsQoSPeakThroughputEnumInfo;
	nLen += EnumToStr(&EnumInfo, pQosInfo->peakThroughput, &pTempBuffer[nLen], (QOSBUFFERLENGTH - nLen));
		
	strcpy(&pTempBuffer[nLen], ")MeanThrput(");
	nLen += strlen(")MeanThrput(");
	EnumInfo.nCount = GprsQoSMeanThroughputEnumInfo_LENGTH;//19
	EnumInfo.pInfoTable = GprsQoSMeanThroughputEnumInfo;
	nLen += EnumToStr(&EnumInfo, pQosInfo->meanThroughput, &pTempBuffer[nLen], (QOSBUFFERLENGTH - nLen));
	pTempBuffer[nLen++] = ')';
		
	pTempBuffer[nLen] = 0;

	if (nLen >= nLength)
		nLen = 0;

	return nLen;
}


/******************************************************
// Function name: Gprs_DiagnosticsToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   转换Org_Diagnostics类型的数据为字符串
// Input:
//       arg1 :Org_Diagnostics *pDiagnostics	待转换的数据
//       arg2 :char *pBuffer		转换后数据存放的Buffer
//       arg3 :int nLength			Buffer的可用长度
// Return value: int				转换后字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int Gprs_DiagnosticsToStr(Org_Diagnostics *pDiagnostics, char *pBuffer, int nLength)
{

	int nLen = 0;

	//ASSERT((BOOL)(pBuffer && pDiagnostics));
	if ((pBuffer == NULL) || (pDiagnostics == NULL))
	{
	     return 0;
	}

	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;

	switch (pDiagnostics->choice)
	{
	case Org_gsm0408Cause_chosen:
		{
			strcpy(pTempBuffer, "GSM0408Cause:");
			nLen += strlen("GSM0408Cause:");
			nLen += NumToDecStr(pDiagnostics->u.gsm0408Cause, &pTempBuffer[nLen], 
								nLength - nLen, FALSE);
		}
		break;
	case Org_gsm0902MapErrorValue_chosen :
		{
			strcpy(pTempBuffer, "GSM0902MapErrorValue:");
			nLen += strlen("GSM0902MapErrorValue:");
			nLen += NumToDecStr(pDiagnostics->u.gsm0902MapErrorValue, &pTempBuffer[nLen],
								nLength - nLen, FALSE);
		}
		break;
	case Org_ccittQ767Cause_chosen:
		{
			strcpy(pTempBuffer, "ccittQ767Cause:");
			nLen += strlen("ccittQ767Cause:");
			nLen += NumToDecStr(pDiagnostics->u.ccittQ767Cause, &pTempBuffer[nLen],
								nLength - nLen, FALSE);
		}
		break;
	case Org_networkSpecificCause_chosen:
		{
			strcpy(pTempBuffer, "NetworkSpecificCause:(");
			nLen += strlen("NetworkSpecificCause:(");
			nLen += Gprs_RecordExtensionToStr(&pDiagnostics->u.networkSpecificCause, &pTempBuffer[nLen],
										 nLength - nLen);
			pTempBuffer[nLen++] = ')';
		}
		break;
	case Org_manufacturerSpecificCause_chosen:
		{
			strcpy(pTempBuffer, "ManufacturerSpecificCause:(");
			nLen += strlen("ManufacturerSpecificCause:(");
			nLen += Gprs_RecordExtensionToStr(&pDiagnostics->u.manufacturerSpecificCause, &pTempBuffer[nLen],
										 nLength - nLen);
			pTempBuffer[nLen++] = ')';
		}
		break;
	default:
		break;
	}

	pTempBuffer[nLen] = 0;
	if(nLen >= nLength)
	{
		nLen = 0;
	}

	return nLen;
}


/******************************************************
// Function name: Gprs_RecordExtensionToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   转换Org_ManagementExtension类型的数据为字符串
// Input:
//       arg1 :Org_ManagementExtension *pData	待转换的数据
//       arg2 :char *pBuffer		转换后数据存放的Buffer
//       arg3 :int nLength			Buffer的可用长度
// Return value: int				转换后字符串的长度
// Notes:
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int Gprs_RecordExtensionToStr(Org_ManagementExtension *pData, char *pBuffer, int nLength)
{

	int nLen = 0;

	pBuffer[0] = 0;

	//ASSERT((BOOL)(pBuffer && pData));
	if ((pBuffer == NULL) || (pData == NULL))
	{
	     return 0;
	}

	char *pTempBuffer = pBuffer;


	strcpy(pTempBuffer, "Identifier:");
	nLen += strlen("Identifier:");
	nLen += HexOctecToStr(pData->identifier.length, pData->identifier.value, &pTempBuffer[nLen], 
							nLength - nLen, "0x"); // modified by YanYong 2004-05-27 问题单SWPD05340
	pTempBuffer[nLen++] = ' ';

	if (pData->bit_mask & Org_significance_present)
	{
		strcpy(&pTempBuffer[nLen], "Significance:");
		nLen += strlen("Significance:");
		nLen += BoolToStr(pData->significance, &pTempBuffer[nLen],nLength - nLen, 0);
		pTempBuffer[nLen++] = ' ';
	}

	strcpy(&pTempBuffer[nLen], "Information:");
	nLen += strlen("Information:");
	nLen += HexOctecToStr(pData->information.length, pData->information.value, &pTempBuffer[nLen],
							nLength - nLen, "0x"); // modified by YanYong 2004-05-27 问题单SWPD05340

	pTempBuffer[nLen] = 0;
	if(nLen >= nLength)
	{
		nLen = 0;
	}

	return nLen;
}

/******************************************************
// Function name: RecordExtensionsToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:	  转换Org_ManagementExtensions_类型的数据为字符串 

                  typedef struct Org_ManagementExtensions_ {
				  struct Org_ManagementExtensions_ *next;
				  Org_ManagementExtension value;
				  } *Org_ManagementExtensions;  
// Input:
//       arg1 :Org_ManagementExtensions_ *pData	待转换的数据
//       arg2 :char *pBuffer	转换后数据存放的Buffer
//       arg3 :int nLength		Buffer可用的长度
// Return value: int			转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int Gprs_RecordExtensionsToStr(Org_ManagementExtensions_ *pData, char *pBuffer, int nLength)
{
	int nLen = 0;
	int nTempLen = 0;
	Org_ManagementExtension* pExtension = NULL;
	Org_ManagementExtensions_* pNode = NULL;

	//ASSERT((BOOL)(pBuffer && pData));
	if ((pBuffer == NULL) || (pData == NULL))
	{
	     return 0;
	}

	pNode = pData;

	while (pNode != NULL)
	{
		if (nLength > nLen)
			pBuffer[nLen++] = '{';
		else
		{
			pBuffer[0] = 0;
			nLen = 0;
			break;
		}
		
		pExtension = &pNode->value;

		nTempLen = Gprs_RecordExtensionToStr(pExtension, &pBuffer[nLen], nLength - nLen);
		if (nTempLen)
			nLen += nTempLen;
		else
		{
			nLen = 0;
			break;
		}

		if (nLength > nLen)
			pBuffer[nLen++] = '}';
		else
		{
			pBuffer[0] = 0;
			nLen = 0;
			break;
		}
		
		//前进到链表的下一个接点进行解释
		pNode = pNode->next;

	}

	if (nLength > nLen)
	{
		pBuffer[nLen] = 0;
	}
	else
	{
		pBuffer[0] = 0;
		nLen = 0;
	}

	return nLen;
}


/******************************************************
// Function name: Gprs_ChangeLocationToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   转换Org_ChangeLocation类型的数据为字符串
// Input:
//       arg1 :Org_ChangeLocation *pData	待转换的数据
//       arg2 :char *pBuffer		转换后数据存放的Buffer
//       arg3 :int nLength			Buffer可用的长度
// Return value: int				转换后字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int Gprs_ChangeLocationToStr(Org_ChangeLocation *pData, char *pBuffer, int nLength)
{

	int nLen = 0;

	//ASSERT((BOOL)(pBuffer && pData));
	if ((pBuffer == NULL) || (pData == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;

	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;

	strcpy(pTempBuffer, "LocationAreaCode:");
	nLen += strlen("LocationAreaCode:");
	nLen += HexOctecToStr((LPOCTECSTRING)&pData->locationAreaCode, &pTempBuffer[nLen], 
							nLength - nLen, "0x", FALSE);
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "RoutingAreaCode:");
	nLen += strlen("RoutingAreaCode:");
	nLen += HexOctecToStr((LPOCTECSTRING)&pData->routingAreaCode, &pTempBuffer[nLen],
							nLength - nLen, "0x",FALSE);
	pTempBuffer[nLen++] = ' ';

	if (pData->bit_mask & Org_cellId_present)
	{
		strcpy(&pTempBuffer[nLen], "CellID:");
		nLen += strlen("CellID:");
		nLen += HexOctecToStr((LPOCTECSTRING)&pData->cellId, &pTempBuffer[nLen],
								nLength - nLen, "0x", FALSE);
		pTempBuffer[nLen++] = ' ';
	}

	strcpy(&pTempBuffer[nLen], "ChangeTime:");
	nLen += strlen("ChangeTime:");
	nLen += TimeStampToStr((LPTIMESTAMP)&pData->changeTime, &pTempBuffer[nLen], 
							nLength - nLen);
	pTempBuffer[nLen] = 0;
	if(nLen >= nLength)
	{
		nLen = 0;
	}

	return nLen;
}


/******************************************************
// Function name: Gprs_ChangeLocationsToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   转换Org__seqof4类型的数据为字符串

                  struct Org__seqof4 {
				  struct Org__seqof4 *next;
				  Org_ChangeLocation value;
			      } *changeLocation
// Input:
//       arg1 :struct Org__seqof4 *pData		待转换的数据
//       arg2 :char *pBuffer			转换后数据存放的Buffer
//       arg3 :int nLength				Buffer可用的长度
// Return value: int					转换后字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int Gprs_ChangeLocationsToStr(Org__seqof4 *pData, char *pBuffer, int nLength)
{
	int nLen = 0;
	int nTempLen = 0;
	Org__seqof4* pNode = NULL;
	Org_ChangeLocation* pLocation = NULL;

	//ASSERT((BOOL)(pBuffer && pData));
	if ((pBuffer == NULL) || (pData == NULL))
	{
	     return 0;
	}

	pNode = pData;

	pBuffer[nLen++] = '{';
	while (pNode != NULL)
	{
		if (nLength > nLen)
			pBuffer[nLen++] = '{';
		else
		{
			pBuffer[0] = 0;			
			return 0;
		}
		
		pLocation = &pNode->value;

		nTempLen = Gprs_ChangeLocationToStr(pLocation, &pBuffer[nLen], nLength - nLen);
		if (nTempLen)
			nLen += nTempLen;
		else
		{
			pBuffer[0] = 0;
			return 0;
		}

		if (nLength > nLen)
			pBuffer[nLen++] = '}';
		else
		{
			pBuffer[0] = 0;
			return 0;
		}
		
		//前进到下一个接点进行解释
		pNode = pNode->next;
	}
	pBuffer[nLen++] = '}';


	if (nLength > nLen)
		pBuffer[nLen] = 0;
	else
	{
		pBuffer[0] = 0;
		nLen = 0;
	}
	return nLen;
}
/******************************************************
// Function name: CvtRecordSequenceNumbertoStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   转换Fin__seqof3类型的数据为字符串
			      struct Fin__seqof3 {
				  struct Fin__seqof3 *next;
				  Fin_RECORDSequenceNumber value;
				  } *recordSequenceNumber

                  typedef struct Fin_RECORDSequenceNumber {
				  Fin_GSNAddress  gsnAddress;
				  struct Fin_RecordNumberList_ *recordNumberList;
			      } Fin_RECORDSequenceNumber;

			      typedef struct Fin_RecordNumberList_ {
				  struct Fin_RecordNumberList_ *next;
				  Fin_RecordNumber value;
			      } *Fin_RecordNumberList

// Input:
//       arg1 :struct Fin__seqof3 *pData	待转换的数据
//       arg2 :char *pBuffer	转换后数据存放的Buffer
//       arg3 :int nLength		Buffer的可用长度
// Return value: int			转换后字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int CvtRecordSequenceNumbertoStr(Fin__seqof3 *pData, char *pBuffer, int nLength)
{
	int nLen = 0;
	int nTempLen = 0;
	Fin__seqof3* pNode = NULL;
	Fin_RECORDSequenceNumber* pSequenceNumber = NULL;

	//ASSERT((BOOL)(pBuffer && pData));
	if ((pBuffer == NULL) || (pData == NULL))
	{
	     return 0;
	}

	pNode = pData;

	while (pNode != NULL)
	{
		if (nLength > nLen)
			pBuffer[nLen++] = '{';
		else
		{
			pBuffer[0] = 0;			
			return 0;
		}
		
		pSequenceNumber = &pNode->value;

		nTempLen = RecordSequenceNumbertoStr(pSequenceNumber, &pBuffer[nLen], nLength - nLen);
		if (nTempLen)
			nLen += nTempLen;
		else
		{
			pBuffer[0] = 0;
			return 0;
		}

		if (nLength > nLen)
			pBuffer[nLen++] = '}';
		else
		{
			pBuffer[0] = 0;
			return 0;
		}
		
		//前进到下一个接点进行解释
		pNode = pNode->next;
	}


	if (nLength > nLen)
		pBuffer[nLen] = 0;
	else
	{
		pBuffer[0] = 0;
		nLen = 0;
	}

	return nLen;
}

/******************************************************
// Function name: RecordSequenceNumbertoStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   转换Fin_RECORDSequenceNumber类型的数据为字符串
			      
                  typedef struct Fin_RECORDSequenceNumber {
				  Fin_GSNAddress  gsnAddress;
				  struct Fin_RecordNumberList_ *recordNumberList;
			      } Fin_RECORDSequenceNumber;

                  typedef struct Fin_RecordNumberList_ {
				  struct Fin_RecordNumberList_ *next;
				  Fin_RecordNumber value;
			      } *Fin_RecordNumberList

// Input:
//       arg1 :struct Fin__seqof3 *pData	待转换的数据
//       arg2 :char *pBuffer	转换后数据存放的Buffer
//       arg3 :int nLength		Buffer的可用长度
// Return value: int			转换后字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int RecordSequenceNumbertoStr(Fin_RECORDSequenceNumber *pData, char *pBuffer, int nLength)
{
	int nLen = 0;
	int nTempLen = 0;
	Fin_RecordNumberList_* pNumberList = NULL;

	//ASSERT((BOOL)(pBuffer && pData));
	if ((pBuffer == NULL) || (pData == NULL))
	{
	     return 0;
	}

	if (nLength > nLen)
		pBuffer[nLen++] = '{';
	else
	{
		pBuffer[0] = 0;			
		return 0;
	}

	//解释gsnAddress
	strcpy(&pBuffer[nLen], "sgsnAddress:");
	nLen += strlen("sgsnAddress:");
	nLen += _GPRSIPAddrToStr((Org_IPAddress*)&pData->gsnAddress, &pBuffer[nLen], nLength - nLen);


	//解释链表recordNumberList，是一串整数		
	
	strcpy(&pBuffer[nLen], " recSequenceNumber:");
	nLen += strlen(" recSequenceNumber:");
		
	pNumberList = pData->recordNumberList;
	while (pNumberList != NULL)
	{
		nLen += NumToDecStr(pNumberList->value, &pBuffer[nLen], nLength - nLen, FALSE);
		pBuffer[nLen++] = ' ';

		pNumberList = pNumberList->next;

	}
		

	if (nLength > nLen)
		pBuffer[nLen++] = '}';
	else
	{
		pBuffer[0] = 0;
		return 0;
	}


	if (nLength > nLen)
		pBuffer[nLen] = 0;
	else
	{
		pBuffer[0] = 0;
		nLen = 0;
	}

	return nLen;
}

/******************************************************
// Function name: GetTextValueOf_Q_GPRSSGSNPDP
// Author:        maimaoshi
// Date:          2002-03-21
// Description:   处理S-CDR话单查询解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int GetTextValueOf_Q_GPRSSGSNPDP(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues)
{
#define SCDRBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	Org_SGSNPDPRecord *pBill = (Org_SGSNPDPRecord*)pValue;
	int  nLen = 0;  
    int nTempLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != Org_CallEventRecordType_sgsnPDPRecord)
		return 0;

    if(NULL != pValues)
    {
        char *pTempBuffer = pBuffer;

        for(int i = 0; i < pValues->size(); i++)
        {
            SQUERY_COND_FIELDS_VALUE& value = (*pValues)[i];

            //recordType
            if(value.sFieldName.compare("recordType") == 0)
            {
                ENUMINFO EnumInfo;
                EnumInfo.nCount = R98_760_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R98_760_Recorde_Type_EnumInfo;
                EnumToStr(&EnumInfo,
                          pBill->recordType,
                          pTempBuffer,
                          nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //servedIMSI
            else if(value.sFieldName.compare("servedIMSI") == 0)
            {
                CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI,
                                 pTempBuffer,
                                 nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //问题单 SWPD05235 YanYong 2004-05-21
            //accessPointNameNI
            else if(value.sFieldName.compare("accessPointNameNI") == 0)
            {
                value.sFieldValue = pBill->accessPointNameNI;
                nLen++;
            }
            //</SWPD05235>            
            //recordOpeningTime
            else if(value.sFieldName.compare("recordOpeningTime") == 0)
            {
                TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime,
                               pTempBuffer,
                               nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //duration
            else if(value.sFieldName.compare("duration") == 0)
            {
                NumToDecStr(pBill->duration,
                            pTempBuffer,
                            nLength,
                            FALSE);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //causeForRecClosing
            else if(value.sFieldName.compare("causeForRecClosing") == 0)
            {
                ENUMINFO EnumInfo;
	            EnumInfo.nCount = GPRSCAUSEFORRECCLOSINGENUNINFO_LENGTH;
	            EnumInfo.pInfoTable = GprsCauseForRecClosingEnumInfo;
                nTempLen = EnumToStr(&EnumInfo,
                                     pBill->causeForRecClosing,
                                     pTempBuffer,
                                     nLength);

                if (nTempLen == 0)
                {
                    strcpy(pTempBuffer, "Unknown reason");
                }
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //recordSequenceNumber
            else if(value.sFieldName.compare("recordSequenceNumber") == 0)
            {
                if (pBill->bit_mask & Org_SGSNPDPRecord_recordSequenceNumber_present)
                {
                    NumToDecStr(pBill->recordSequenceNumber,
                                pTempBuffer,
                                nLength,
                                FALSE);
                    value.sFieldValue = pTempBuffer;
                }
                else
                {
                    value.sFieldValue = _EMPTYSTRING;
                }
                nLen++;
            }
            //servedMSISDN
            else if(value.sFieldName.compare("servedMSISDN") == 0)
            {
                if (pBill->bit_mask & Org_SGSNPDPRecord_servedMSISDN_present)
                {
                    CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedMSISDN,
                                     pTempBuffer,
                                     nLength);
                    value.sFieldValue = pTempBuffer;
                }
                else
                {
                    value.sFieldValue = _EMPTYSTRING;
                }
                nLen++;
            }
        }
    }
		
	return nLen;
}

/******************************************************
// Function name: GetTextValueOf_Q_GPRSGGSNPDP
// Author:        maimaoshi
// Date:          2002-03-21
// Description:   处理G-CDR话单查询解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int GetTextValueOf_Q_GPRSGGSNPDP(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues)
{
#define GCDRBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	Org_GGSNPDPRecord *pBill = (Org_GGSNPDPRecord*)pValue;
	int nLen = 0;
    int nTempLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != Org_CallEventRecordType_ggsnPDPRecord)
		return 0;

    if(NULL != pValues)
    {
        char *pTempBuffer = pBuffer;

        for(int i = 0; i < pValues->size(); i++)
        {
            SQUERY_COND_FIELDS_VALUE& value = (*pValues)[i];

            //recordType
            if(value.sFieldName.compare("recordType") == 0)
            {
                ENUMINFO EnumInfo;
                EnumInfo.nCount = R98_760_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R98_760_Recorde_Type_EnumInfo;
                EnumToStr(&EnumInfo,
                          pBill->recordType,
                          pTempBuffer,
                          nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //servedIMSI
            else if(value.sFieldName.compare("servedIMSI") == 0)
            {
                CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI,
                                  pTempBuffer,
                                  nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //问题单 SWPD05235 YanYong 2004-05-21
            //accessPointNameNI
            else if(value.sFieldName.compare("accessPointNameNI") == 0)
            {
                value.sFieldValue = pBill->accessPointNameNI;
                nLen++;
            }
            //</SWPD05235>            
            //recordOpeningTime
            else if(value.sFieldName.compare("recordOpeningTime") == 0)
            {
                TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime,
                               pTempBuffer,
                               nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //duration
            else if(value.sFieldName.compare("duration") == 0)
            {
                NumToDecStr(pBill->duration,
                            pTempBuffer,
                            nLength,
                            FALSE);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //causeForRecClosing
            else if(value.sFieldName.compare("causeForRecClosing") == 0)
            {
                ENUMINFO EnumInfo;
	            EnumInfo.nCount = GPRSCAUSEFORRECCLOSINGENUNINFO_LENGTH;
	            EnumInfo.pInfoTable = GprsCauseForRecClosingEnumInfo;
                nTempLen = EnumToStr(&EnumInfo,
                                     pBill->causeForRecClosing,
                                     pTempBuffer,
                                     nLength);

                if (nTempLen == 0)
                {
                    strcpy(pTempBuffer, "Unknown reason");
                }
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //recordSequenceNumber
            else if(value.sFieldName.compare("recordSequenceNumber") == 0)
            {
                if (pBill->bit_mask & Org_GGSNPDPRecord_recordSequenceNumber_present)
                {
                    NumToDecStr(pBill->recordSequenceNumber,
                                pTempBuffer,
                                nLength,
                                FALSE);
                    value.sFieldValue = pTempBuffer;
                }
                else
                {
                    value.sFieldValue = _EMPTYSTRING;
                }
                nLen++;
            }
            //servedMSISDN
            else if(value.sFieldName.compare("servedMSISDN") == 0)
            {
                if (pBill->bit_mask & Org_GGSNPDPRecord_servedMSISDN_present)
                {
                    CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedMSISDN,
                                     pTempBuffer,
                                     nLength);
                    value.sFieldValue = pTempBuffer;
                }
                else
                {
                    value.sFieldValue = _EMPTYSTRING;
                }
                nLen++;
            }
        }
    }
	
	return nLen;
}


/******************************************************
// Function name: GetTextValueOf_Q_GPRSSGSNMM
// Author:        maimaoshi
// Date:          2002-03-21
// Description:   处理M-CDR话单查询解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int GetTextValueOf_Q_GPRSSGSNMM(char *pValue,
                                char *pBuffer,
                                int nLength,
                                QUERY_COND_FIELDS_VALUE* pValues)
{
#define MCDRBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	Org_SGSNMMRecord *pBill = (Org_SGSNMMRecord*)pValue;
	int nLen = 0;
    int nTempLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != Org_CallEventRecordType_sgsnMMRecord)
		return 0;

    if(NULL != pValues)
    {
        char *pTempBuffer = pBuffer;

        for(int i = 0; i < pValues->size(); i++)
        {
            SQUERY_COND_FIELDS_VALUE& value = (*pValues)[i];

            //recordType
            if(value.sFieldName.compare("recordType") == 0)
            {
                ENUMINFO EnumInfo;
                EnumInfo.nCount = R98_760_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R98_760_Recorde_Type_EnumInfo;
                EnumToStr(&EnumInfo,
                          pBill->recordType,
                          pTempBuffer,
                          nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //servedIMSI
            else if(value.sFieldName.compare("servedIMSI") == 0)
            {
                CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI,
                                  pTempBuffer,
                                  nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //recordOpeningTime
            else if(value.sFieldName.compare("recordOpeningTime") == 0)
            {
                TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime,
                               pTempBuffer,
                               nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //duration
            else if(value.sFieldName.compare("duration") == 0)
            {
                if (pBill->bit_mask & Org_duration_present)
                {
                    NumToDecStr(pBill->duration,
                                pTempBuffer,
                                nLength,
                                FALSE);
                    value.sFieldValue = pTempBuffer;
                }
                else
                {
                    value.sFieldValue = _EMPTYSTRING;
                }
                nLen++;
            }
            //causeForRecClosing
            else if(value.sFieldName.compare("causeForRecClosing") == 0)
            {
                ENUMINFO EnumInfo;
	            EnumInfo.nCount = GPRSCAUSEFORRECCLOSINGENUNINFO_LENGTH;
	            EnumInfo.pInfoTable = GprsCauseForRecClosingEnumInfo;
                nTempLen = EnumToStr(&EnumInfo,
                                     pBill->causeForRecClosing,
                                     pTempBuffer,
                                     nLength);

                if (nTempLen == 0)
                {
                    strcpy(pTempBuffer, "Unknown reason");
                }
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //recordSequenceNumber
            else if(value.sFieldName.compare("recordSequenceNumber") == 0)
            {
                if (pBill->bit_mask & Org_SGSNMMRecord_recordSequenceNumber_present)
                {
                    NumToDecStr(pBill->recordSequenceNumber,
                                pTempBuffer,
                                nLength,
                                FALSE);
                    value.sFieldValue = pTempBuffer;
                }
                else
                {
                    value.sFieldValue = _EMPTYSTRING;
                }
                nLen++;
            }
        }
    }

	return nLen;
}


/******************************************************
// Function name: GetTextValueOf_Q_GPRSSGSNSMO
// Author:        maimaoshi
// Date:          2002-03-21
// Description:   处理S-SMO-CDR话单查询解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int GetTextValueOf_Q_GPRSSGSNSMO(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues)
{
#define SMOBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	Org_SGSNSMORecord *pBill = (Org_SGSNSMORecord*)pValue;
	int nLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != Org_CallEventRecordType_sgsnSMORecord)
		return 0;

    if(NULL != pValues)
    {
        char *pTempBuffer = pBuffer;

        for(int i = 0; i < pValues->size(); i++)
        {
            SQUERY_COND_FIELDS_VALUE& value = (*pValues)[i];

            //recordType
            if(value.sFieldName.compare("recordType") == 0)
            {
                ENUMINFO EnumInfo;
                EnumInfo.nCount = R98_760_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R98_760_Recorde_Type_EnumInfo;
                EnumToStr(&EnumInfo,
                          pBill->recordType,
                          pTempBuffer,
                          nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //修改问题单SWPD04788  by YanYong 2004-04-26
            //servedIMSI
            else if(value.sFieldName.compare("servedIMSI") == 0)
            {
                CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI, 
                    pTempBuffer, nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //</SWPD04788>       
            //servedMSISDN
            else if(value.sFieldName.compare("servedMSISDN") == 0)
            {
                if (pBill->bit_mask & Org_SGSNSMORecord_servedMSISDN_present)
                {
                    CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedMSISDN,
                                     pTempBuffer,
                                     nLength);
                    value.sFieldValue = pTempBuffer;
                }
                else
                {
                    value.sFieldValue = _EMPTYSTRING;
                }
                nLen++;
            }
            //originationTime
            else if(value.sFieldName.compare("originationTime") == 0)
            {
                TimeStampToStr((LPTIMESTAMP)&pBill->originationTime,
                               pTempBuffer,
                               nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
        }
    }

	return nLen;
}


/******************************************************
// Function name: GetTextValueOf_Q_GPRSSGSNSMT
// Author:        maimaoshi
// Date:          2002-03-21
// Description:   处理S-SMT-CDR话单查询解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int GetTextValueOf_Q_GPRSSGSNSMT(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues)
{
#define SMTBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	Org_SGSNSMTRecord *pBill = (Org_SGSNSMTRecord*)pValue;
	int nLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != Org_CallEventRecordType_sgsnSMTRecord)
		return 0;

    if(NULL != pValues)
    {
        char *pTempBuffer = pBuffer;

        for(int i = 0; i < pValues->size(); i++)
        {
            SQUERY_COND_FIELDS_VALUE& value = (*pValues)[i];

            //recordType
            if(value.sFieldName.compare("recordType") == 0)
            {
                ENUMINFO EnumInfo;
                EnumInfo.nCount = R98_760_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R98_760_Recorde_Type_EnumInfo;
                EnumToStr(&EnumInfo,
                          pBill->recordType,
                          pTempBuffer,
                          nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //修改问题单SWPD04788  by YanYong 2004-04-26
            //servedIMSI
            else if(value.sFieldName.compare("servedIMSI") == 0)
            {
                CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI, 
                    pTempBuffer, nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //</SWPD04788>   
            //servedMSISDN
            else if(value.sFieldName.compare("servedMSISDN") == 0)
            {
                if (pBill->bit_mask & Org_SGSNSMTRecord_servedMSISDN_present)
                {
                    CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedMSISDN,
                                     pTempBuffer,
                                     nLength);
                    value.sFieldValue = pTempBuffer;
                }
                else
                {
                    value.sFieldValue = _EMPTYSTRING;
                }
                nLen++;
            }
            //originationTime
            else if(value.sFieldName.compare("originationTime") == 0)
            {
                TimeStampToStr((LPTIMESTAMP)&pBill->originationTime,
                               pTempBuffer,
                               nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
        }
    }
	
	return nLen;
}


/******************************************************
// Function name: GetTextValueOf_Q_GPRSLARGESGSNPDP
// Author:        maimaoshi
// Date:          2002-03-21
// Description:   处理合并后S-CDR话单查询解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int GetTextValueOf_Q_GPRSLARGESGSNPDP(char *pValue,
                                      char *pBuffer,
                                      int nLength,
                                      QUERY_COND_FIELDS_VALUE* pValues)
{
	Fin_SGSNPDPRecord *pBill = (Fin_SGSNPDPRecord*)pValue;
	int nLen = 0;
    int nTempLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != Fin_CallEventRecordType_sgsnPDPRecord)
		return 0;

    if(NULL != pValues)
    {
        char *pTempBuffer = pBuffer;

        for(int i = 0; i < pValues->size(); i++)
        {
            SQUERY_COND_FIELDS_VALUE& value = (*pValues)[i];

            //recordType
            if(value.sFieldName.compare("recordType") == 0)
            {
                ENUMINFO EnumInfo;
                EnumInfo.nCount = R98_760_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R98_760_Recorde_Type_EnumInfo;
                EnumToStr(&EnumInfo,
                          pBill->recordType,
                          pTempBuffer,
                          nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //servedIMSI
            else if(value.sFieldName.compare("servedIMSI") == 0)
            {
                CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI,
                                 pTempBuffer,
                                 nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //问题单 SWPD05235 YanYong 2004-05-21
            //accessPointNameNI
            else if(value.sFieldName.compare("accessPointNameNI") == 0)
            {
                value.sFieldValue = pBill->accessPointNameNI;
                nLen++;
            }
            //</SWPD05235>            
            //recordOpeningTime
            else if(value.sFieldName.compare("recordOpeningTime") == 0)
            {
                TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime,
                               pTempBuffer,
                               nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //duration
            else if(value.sFieldName.compare("duration") == 0)
            {
                NumToDecStr(pBill->duration,
                            pTempBuffer,
                            nLength,
                            FALSE);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //causeForRecClosing
            else if(value.sFieldName.compare("causeForRecClosing") == 0)
            {
                ENUMINFO EnumInfo;
	            EnumInfo.nCount = GPRSCAUSEFORRECCLOSINGENUNINFO_LENGTH;
	            EnumInfo.pInfoTable = GprsCauseForRecClosingEnumInfo;
                nTempLen = EnumToStr(&EnumInfo,
                                     pBill->causeForRecClosing,
                                     pTempBuffer,
                                     nLength);

                if (nTempLen == 0)
                {
                    strcpy(pTempBuffer, "Unknown reason");
                }
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //servedMSISDN
            else if(value.sFieldName.compare("servedMSISDN") == 0)
            {
                if (pBill->bit_mask & Fin_SGSNPDPRecord_servedMSISDN_present)
                {
                    CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedMSISDN,
                                     pTempBuffer,
                                     nLength);
                    value.sFieldValue = pTempBuffer;
                }
                else
                {
                    value.sFieldValue = _EMPTYSTRING;
                }
                nLen++;
            }
        }
    }
    
	return nLen;
}


/******************************************************
// Function name: GetTextValueOf_Q_GPRSLARGEGGSNPDP
// Author:        maimaoshi
// Date:          2002-03-21
// Description:   处理合并后G-CDR话单查询解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int GetTextValueOf_Q_GPRSLARGEGGSNPDP(char *pValue,
                                      char *pBuffer,
                                      int nLength,
                                      QUERY_COND_FIELDS_VALUE* pValues)
{
	Fin_GGSNPDPRecord *pBill = (Fin_GGSNPDPRecord*)pValue;
	int nLen = 0;
    int nTempLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != Fin_CallEventRecordType_ggsnPDPRecord)
		return 0;

    if(NULL != pValues)
    {
        char *pTempBuffer = pBuffer;

        for(int i = 0; i < pValues->size(); i++)
        {
            SQUERY_COND_FIELDS_VALUE& value = (*pValues)[i];

            //recordType
            if(value.sFieldName.compare("recordType") == 0)
            {
                ENUMINFO EnumInfo;
                EnumInfo.nCount = R98_760_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R98_760_Recorde_Type_EnumInfo;
                EnumToStr(&EnumInfo,
                          pBill->recordType,
                          pTempBuffer,
                          nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //servedIMSI
            else if(value.sFieldName.compare("servedIMSI") == 0)
            {
                CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI,
                                  pTempBuffer,
                                  nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //问题单 SWPD05235 YanYong 2004-05-21
            //accessPointNameNI
            else if(value.sFieldName.compare("accessPointNameNI") == 0)
            {
                value.sFieldValue = pBill->accessPointNameNI;
                nLen++;
            }
            //</SWPD05235>            
            //recordOpeningTime
            else if(value.sFieldName.compare("recordOpeningTime") == 0)
            {
                TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime,
                               pTempBuffer,
                               nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //duration
            else if(value.sFieldName.compare("duration") == 0)
            {
                NumToDecStr(pBill->duration,
                            pTempBuffer,
                            nLength,
                            FALSE);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //causeForRecClosing
            else if(value.sFieldName.compare("causeForRecClosing") == 0)
            {
                ENUMINFO EnumInfo;
	            EnumInfo.nCount = GPRSCAUSEFORRECCLOSINGENUNINFO_LENGTH;
	            EnumInfo.pInfoTable = GprsCauseForRecClosingEnumInfo;
                nTempLen = EnumToStr(&EnumInfo,
                                     pBill->causeForRecClosing,
                                     pTempBuffer,
                                     nLength);

                if (nTempLen == 0)
                {
                    strcpy(pTempBuffer, "Unknown reason");
                }
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //servedMSISDN
            else if(value.sFieldName.compare("servedMSISDN") == 0)
            {
                if (pBill->bit_mask & Fin_GGSNPDPRecord_servedMSISDN_present)
                {
                    CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedMSISDN,
                                     pTempBuffer,
                                     nLength);
                    value.sFieldValue = pTempBuffer;
                }
                else
                {
                    value.sFieldValue = _EMPTYSTRING;
                }
                nLen++;
            }
        }
    }	
	
	return nLen;
}


/******************************************************
// Function name: GetTextValueOf_Q_GPRSLARGESGSNMM
// Author:        maimaoshi
// Date:          2002-03-21
// Description:   处理合并后M-CDR话单查询解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
int GetTextValueOf_Q_GPRSLARGESGSNMM(char *pValue,
                                     char *pBuffer,
                                     int nLength,
                                     QUERY_COND_FIELDS_VALUE* pValues)
{
	Fin_SGSNMMRecord *pBill = (Fin_SGSNMMRecord*)pValue;
	int nLen = 0;
    int nTempLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != Fin_CallEventRecordType_sgsnMMRecord)
		return 0;

    if(NULL != pValues)
    {
        char *pTempBuffer = pBuffer;

        for(int i = 0; i < pValues->size(); i++)
        {
            SQUERY_COND_FIELDS_VALUE& value = (*pValues)[i];

            //recordType
            if(value.sFieldName.compare("recordType") == 0)
            {
                ENUMINFO EnumInfo;
                EnumInfo.nCount = R98_760_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R98_760_Recorde_Type_EnumInfo;
                EnumToStr(&EnumInfo,
                          pBill->recordType,
                          pTempBuffer,
                          nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //servedIMSI
            else if(value.sFieldName.compare("servedIMSI") == 0)
            {
                CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI,
                                  pTempBuffer,
                                  nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //recordOpeningTime
            else if(value.sFieldName.compare("recordOpeningTime") == 0)
            {
                TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime,
                               pTempBuffer,
                               nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //duration
            else if(value.sFieldName.compare("duration") == 0)
            {
                if (pBill->bit_mask & Fin_duration_present)
                {
                    NumToDecStr(pBill->duration,
                                pTempBuffer,
                                nLength,
                                FALSE);
                    value.sFieldValue = pTempBuffer;
                }
                else
                {
                    value.sFieldValue = _EMPTYSTRING;
                }
                nLen++;
            }
            //causeForRecClosing
            else if(value.sFieldName.compare("causeForRecClosing") == 0)
            {
                ENUMINFO EnumInfo;
	            EnumInfo.nCount = GPRSCAUSEFORRECCLOSINGENUNINFO_LENGTH;
	            EnumInfo.pInfoTable = GprsCauseForRecClosingEnumInfo;
                nTempLen = EnumToStr(&EnumInfo,
                                     pBill->causeForRecClosing,
                                     pTempBuffer,
                                     nLength);

                if (nTempLen == 0)
                {
                    strcpy(pTempBuffer, "Unknown reason");
                }
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
        }
    }

	return nLen;
}

/*
功能:
	 按GGSN定义的基于内容计费的recordExtensions域填写格式转文本。
输入:
	pValue	:	内容计费信息
	pBuffer	:	存放返回字符串的数据区
	nLength	:	pBuffer可用长度
输出:
	转换后的字符串的长度	
*/
int R98_GCDRRecordExtensionsToStr(Org_ManagementExtensions_ *pData, char *pBuffer, int nLength)
{
	int nLen = 0;
	
	if ((pData == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;	
	char *pTempBuffer = pBuffer;
	Org_ManagementExtensions_* pNextExt = pData;

	//recordExtensions目前只定义了19个域
	while (pNextExt)
	{
		strcpy(&pTempBuffer[nLen], "{");
		nLen += 1;

		for (int j = 0; j < 19; j++)
		{
			if (pNextExt == NULL)
			{
                strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-27 问题单SWPD05340
                nLen += 1;
				return nLen;
			}

			switch (j)
			{
				//Server IP
			    case 0:
				{
					if ((pNextExt->value.information.value == NULL) 
					 || (pNextExt->value.information.length != sizeof(unsigned long)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-27 问题单SWPD05340
                        nLen += 1;                        
						return nLen;
					}

					strcpy(&pTempBuffer[nLen], "ServerIP:");
					nLen += strlen("ServerIP:");
					unsigned long nServerIP;
					memcpy(&nServerIP, pNextExt->value.information.value, sizeof(unsigned long));
					struct in_addr Addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
					Addr.s_addr = nServerIP;
#else //_PLATFORM_HPUX
					Addr.S_un.S_addr = nServerIP;
#endif//_PLATFORM_HPUX
					char szIPAddress[32];
					if (ACE_OS::inet_ntoa(Addr))
					{
						SNPRINTF(szIPAddress, 32, "%s", ACE_OS::inet_ntoa(Addr));
					}
					else
					{
						strcpy(szIPAddress, "Invalid");
					}					
					memcpy(&pTempBuffer[nLen], szIPAddress, strlen(szIPAddress));
					nLen += strlen(szIPAddress);					
					pTempBuffer[nLen++] = ' ';					
					pNextExt = pNextExt->next;
					break;
				}
				
				//Server Port
				case 1:
				{
					if ((pNextExt->value.information.value == NULL)
					 || (pNextExt->value.information.length != sizeof(unsigned short)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-27 问题单SWPD05340
                        nLen += 1;
						return nLen;
					}

					unsigned short nServerPort;
					memcpy(&nServerPort, pNextExt->value.information.value, sizeof(unsigned short));
					nServerPort = NTOHS(nServerPort);

					strcpy(&pTempBuffer[nLen], "ServerPort:");
					nLen += strlen("ServerPort:");
					nLen += NumToDecStr(nServerPort, &pTempBuffer[nLen], nLength - nLen, FALSE);
					pTempBuffer[nLen++] = ' ';
					pNextExt = pNextExt->next;
					break;
				}

				//ContentRecordFlag
				case 2:
				{
					if ((pNextExt->value.information.value == NULL) 
					 || (pNextExt->value.information.length != sizeof(unsigned char)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-27 问题单SWPD05340
                        nLen += 1;
						return nLen;
					}

					strcpy(&pTempBuffer[nLen], "ContentRecordFlag:");
					nLen += strlen("ContentRecordFlag:");
					
					strcpy(&pTempBuffer[nLen], "Reserved");
					nLen += strlen("Reserved");
                    /*
					unsigned char nContentRecordFlag;
					memcpy(&nContentRecordFlag, pNextExt->value.information.value, sizeof(unsigned char));
					switch (nContentRecordFlag)
					{
						case 0:
						{
							strcpy(&pTempBuffer[nLen], "URL");
							nLen += strlen("URL");
							break;
						}

						case 1:
						{
							strcpy(&pTempBuffer[nLen], "Volume");
							nLen += strlen("Volume");
							break;
						}

						case 2:
						{
							strcpy(&pTempBuffer[nLen], "Duration");
							nLen += strlen("Duration");
							break;
						}
			
						case 3:
						{
							strcpy(&pTempBuffer[nLen], "Volume&Duration");
							nLen += strlen("Volume&Duration");
							break;
						}

						default:
						{
							strcpy(&pTempBuffer[nLen], "Reserved");
							nLen += strlen("Reserved");
							break;
						}
					}
                    */

					pTempBuffer[nLen++] = ' ';
					pNextExt = pNextExt->next;
					break;
				}

				//TransferProtocol
				case 3:
				{
					if ((pNextExt->value.information.value == NULL) 
					 || (pNextExt->value.information.length != sizeof(unsigned short)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-27 问题单SWPD05340
                        nLen += 1;
						return nLen;
					}

					strcpy(&pTempBuffer[nLen], "TransferProtocol:");
					nLen += strlen("TransferProtocol:");
					unsigned short nTransferProtocol;
					memcpy(&nTransferProtocol, pNextExt->value.information.value, sizeof(unsigned short));					
					nTransferProtocol = NTOHS(nTransferProtocol);

					//GGSN目前只支持TCP协议
					strcpy(&pTempBuffer[nLen], "TCP");
					nLen += strlen("TCP");
					pTempBuffer[nLen++] = ' ';
					pNextExt = pNextExt->next;
					break;
				}

				//Ms IP
				case 4:
				{
					if ((pNextExt->value.information.value == NULL) 
					 || (pNextExt->value.information.length != sizeof(unsigned long)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-27 问题单SWPD05340
                        nLen += 1;
						return nLen;
					}

					strcpy(&pTempBuffer[nLen], "MsIP:");
					nLen += strlen("MsIP:");
					unsigned long nMsIP;
					memcpy(&nMsIP, pNextExt->value.information.value, sizeof(unsigned long));
					struct in_addr Addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
					Addr.s_addr = nMsIP;
#else //_PLATFORM_HPUX
					Addr.S_un.S_addr = nMsIP;
#endif//_PLATFORM_HPUX
					char szIPAddress[32];
					if (ACE_OS::inet_ntoa(Addr))
					{
						SNPRINTF(szIPAddress, 32, "%s", ACE_OS::inet_ntoa(Addr));
					}
					else
					{
						strcpy(szIPAddress, "Invalid");
					}
					
					if(!strcmp("0.0.0.0",szIPAddress))
					{
						strcpy(&pTempBuffer[nLen], "Reserved");
						nLen += strlen("Reserved");
					}
					else
					{
						memcpy(&pTempBuffer[nLen], szIPAddress, strlen(szIPAddress));
						nLen += strlen(szIPAddress);
					}

					pTempBuffer[nLen++] = ' ';
					pNextExt = pNextExt->next;
					break;
				}

				//MsPort
				case 5:
				{
					if ((pNextExt->value.information.value == NULL) 
					 || (pNextExt->value.information.length != sizeof(unsigned short)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-27 问题单SWPD05340
                        nLen += 1;
						return nLen;
					}

					unsigned short nMsPort;
					memcpy(&nMsPort, pNextExt->value.information.value, sizeof(unsigned short));
					nMsPort = NTOHS(nMsPort);

					strcpy(&pTempBuffer[nLen], "MsPort:");
					nLen += strlen("MsPort:");
					
					if(0 == nMsPort)
					{
						strcpy(&pTempBuffer[nLen], "Reserved");
						nLen += strlen("Reserved");
					}
					else
					{
						nLen += NumToDecStr(nMsPort, &pTempBuffer[nLen], nLength - nLen, FALSE);
					}

					pTempBuffer[nLen++] = ' ';
					pNextExt = pNextExt->next;
					break;
				}

				//AppProtocol
				case 6:
				{
					if ((pNextExt->value.information.value == NULL) 
					 || (pNextExt->value.information.length != sizeof(unsigned short)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-27 问题单SWPD05340
                        nLen += 1;
						return nLen;
					}

					unsigned short nAppProtocol;
					memcpy(&nAppProtocol, pNextExt->value.information.value, sizeof(unsigned short));
					nAppProtocol = NTOHS(nAppProtocol);

					strcpy(&pTempBuffer[nLen], "AppProrocol:");
					nLen += strlen("AppProrocol:");
	
					switch (nAppProtocol)
					{
						case 1:
						{
							strcpy(&pTempBuffer[nLen], "HTTP");
							nLen += strlen("HTTP");
							break;
						}

						case 2:
						{
							strcpy(&pTempBuffer[nLen], "VOD");
							nLen += strlen("VOD");
							break;
						}

						case 3:
						{
							strcpy(&pTempBuffer[nLen], "WAP");
							nLen += strlen("WAP");
							break;
						}

						case 4:
						{
							strcpy(&pTempBuffer[nLen], "EMAIL");
							nLen += strlen("EMAIL");
							break;
						}

						case 5:
						{
							strcpy(&pTempBuffer[nLen], "FTP");
							nLen += strlen("FTP");
							break;
						}
						
						default:
						{
							strcpy(&pTempBuffer[nLen], "Unknown");
							nLen += strlen("Unknown");
							break;
						}
					}

					pTempBuffer[nLen++] = ' ';
					pNextExt = pNextExt->next;
					break;					
				}

				//UpFlow
				case 7:
				{
					if ((pNextExt->value.information.value == NULL) 
					 || (pNextExt->value.information.length != sizeof(unsigned long)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-27 问题单SWPD05340
                        nLen += 1;
						return nLen;
					}

					unsigned long nUpFlow;
					memcpy(&nUpFlow, pNextExt->value.information.value, sizeof(unsigned long));
					nUpFlow = NTOHL(nUpFlow);

					strcpy(&pTempBuffer[nLen], "UpFlow:");
					nLen += strlen("UpFlow:");
					nLen += NumToDecStr(nUpFlow, &pTempBuffer[nLen], nLength - nLen, FALSE);
					pTempBuffer[nLen++] = ' ';
					pNextExt = pNextExt->next;
					break;					
				}

				//DownFlow
				case 8:
				{
					if ((pNextExt->value.information.value == NULL) 
					 || (pNextExt->value.information.length != sizeof(unsigned long)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-27 问题单SWPD05340
                        nLen += 1;
						return nLen;
					}

					unsigned long nDownFlow;
					memcpy(&nDownFlow, pNextExt->value.information.value, sizeof(unsigned long));
					nDownFlow = NTOHL(nDownFlow);

					strcpy(&pTempBuffer[nLen], "DownFlow:");
					nLen += strlen("DownFlow:");
					nLen += NumToDecStr(nDownFlow, &pTempBuffer[nLen], nLength - nLen, FALSE);
					pTempBuffer[nLen++] = ' ';
					pNextExt = pNextExt->next;
					break;					
				}

				//Duration
				case 9:
				{
					if ((pNextExt->value.information.value == NULL) 
					 || (pNextExt->value.information.length != sizeof(unsigned long)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-27 问题单SWPD05340
                        nLen += 1;
						return nLen;
					}

					unsigned long nDuration;
					memcpy(&nDuration, pNextExt->value.information.value, sizeof(unsigned long));
					nDuration = NTOHL(nDuration);

					strcpy(&pTempBuffer[nLen], "Duration:");
					nLen += strlen("Duration:");

					nLen += NumToDecStr(nDuration, &pTempBuffer[nLen], nLength - nLen, FALSE);
					/*
					strcpy(&pTempBuffer[nLen], "Reserved");
					nLen += strlen("Reserved");
					*/
					
					pTempBuffer[nLen++] = ' ';
					pNextExt = pNextExt->next;
					break;					
				}

				//URLString
				case 10:
				{
					strcpy(&pTempBuffer[nLen], "URL:");
					nLen += strlen("URL:");

					if ((pNextExt->value.information.value != NULL) 
						|| (pNextExt->value.information.length != 0))
					{
						char tmpBlankStr[256];
						int ntmpStrLen = pNextExt->value.information.length;
						
						//在GGSN的定义此域为200字节，256其实没有必要
						if( ntmpStrLen < 256)
						{
							memcpy(tmpBlankStr, pNextExt->value.information.value, ntmpStrLen);
							tmpBlankStr[ntmpStrLen] = '\0';
						}
						else
						{
							memcpy(tmpBlankStr, pNextExt->value.information.value, 252);
							tmpBlankStr[252] = '\0';
							strcat(tmpBlankStr, "...");
						}
						
						//找到连续的两个空格的情况，因为报文解析原因，
						//需要把两个空格变为一个空格,并且末尾不能有空格
						char* szTemp=tmpBlankStr;
						
						BOOL bDoubleBlank = TRUE;
						
						while (bDoubleBlank)
						{
							int nTmpPos = 0;
							STRING szTmp2 = szTemp;

							nTmpPos = szTmp2.find("  ",nTmpPos);
							if ( nTmpPos >= 0)
							{
								szTemp += nTmpPos + 1;
								tmpBlankStr[nTmpPos] = '\0';

								strcat(tmpBlankStr,szTemp);
								szTemp = tmpBlankStr;
							}
							else
							{
								bDoubleBlank = FALSE;
							}
							
						}
						
						ntmpStrLen = strlen(tmpBlankStr);
						
						if(ntmpStrLen == 0)
						{
							pTempBuffer[nLen++] = ' ';
							break;
						}
						
						if(tmpBlankStr[ntmpStrLen-1] == ' ')
						{
							tmpBlankStr[ntmpStrLen-1] = '\0';

						}
						
						ntmpStrLen = strlen(tmpBlankStr);

						memcpy(&pTempBuffer[nLen], tmpBlankStr, ntmpStrLen);						
						nLen += ntmpStrLen;

						/*
						memcpy(&pTempBuffer[nLen], pNextExt->value.information.value, pNextExt->value.information.length);						
						nLen += pNextExt->value.information.length;
						*/
					}
					else
					{
						strcpy(&pTempBuffer[nLen], "Empty Value");
						nLen += strlen("Empty Value");
					}

					pTempBuffer[nLen++] = ' ';
					pNextExt = pNextExt->next;
					break;					
				}

				//FlowRateInformation
				case 11:
				{
					if ((pNextExt->value.information.value == NULL) 
					 || (pNextExt->value.information.length != sizeof(unsigned short)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-27 问题单SWPD05340
                        nLen += 1;
						return nLen;
					}

					unsigned short nFlowRateInfo;
					memcpy(&nFlowRateInfo, pNextExt->value.information.value, sizeof(unsigned short));
					nFlowRateInfo = NTOHS(nFlowRateInfo);

					strcpy(&pTempBuffer[nLen], "FlowRateInfo:");
					nLen += strlen("FlowRateInfo:");
					nLen += NumToDecStr(nFlowRateInfo, &pTempBuffer[nLen], nLength - nLen, FALSE);
					pTempBuffer[nLen++] = ' ';
					pNextExt = pNextExt->next;
					break;
				}

				//TimeRateInformation
				case 12:
				{
					if ((pNextExt->value.information.value == NULL) 
					 || (pNextExt->value.information.length != sizeof(unsigned short)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-27 问题单SWPD05340
                        nLen += 1;
						return nLen;
					}

					unsigned short nTimeRateInfo;
					memcpy(&nTimeRateInfo, pNextExt->value.information.value, sizeof(unsigned short));
					nTimeRateInfo = NTOHS(nTimeRateInfo);

					strcpy(&pTempBuffer[nLen], "TimeRateInfo:");
					nLen += strlen("TimeRateInfo:");

					//nLen += NumToDecStr(nTimeRateInfo, &pTempBuffer[nLen], nLength - nLen, FALSE);
					strcpy(&pTempBuffer[nLen], "Reserved");
					nLen += strlen("Reserved");

					pTempBuffer[nLen++] = ' ';
					pNextExt = pNextExt->next;
					break;
				}

				//FlowBillInformation
				case 13:
				{
					if ((pNextExt->value.information.value == NULL) 
					 || (pNextExt->value.information.length != sizeof(unsigned long)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-27 问题单SWPD05340
                        nLen += 1;
						return nLen;
					}

					unsigned long nFlowBillInfo;
					memcpy(&nFlowBillInfo, pNextExt->value.information.value, sizeof(unsigned long));
					nFlowBillInfo = NTOHL(nFlowBillInfo);

					strcpy(&pTempBuffer[nLen], "FlowBillInfo:");
					nLen += strlen("FlowBillInfo:");
					nLen += NumToDecStr(nFlowBillInfo, &pTempBuffer[nLen], nLength - nLen, FALSE);
					pTempBuffer[nLen++] = ' ';
					pNextExt = pNextExt->next;
					break;
				}

				//Time Bill Information
				case 14:
				{
					if ((pNextExt->value.information.value == NULL) 
					 || (pNextExt->value.information.length != sizeof(unsigned long)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-27 问题单SWPD05340
                        nLen += 1;
						return nLen;
					}

					unsigned long nTimeBillInfo;
					memcpy(&nTimeBillInfo, pNextExt->value.information.value, sizeof(unsigned long));
					nTimeBillInfo = NTOHL(nTimeBillInfo);

					strcpy(&pTempBuffer[nLen], "TimeBillInfo:");
					nLen += strlen("TimeBillInfo:");

					//nLen += NumToDecStr(nTimeBillInfo, &pTempBuffer[nLen], nLength - nLen, FALSE);
					strcpy(&pTempBuffer[nLen], "Reserved");
					nLen += strlen("Reserved");

					pTempBuffer[nLen++] = ' ';
					pNextExt = pNextExt->next;
					break;
				}

				//URLBillInformation
				case 15:
				{
					if ((pNextExt->value.information.value == NULL) 
					 || (pNextExt->value.information.length != sizeof(unsigned long)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-27 问题单SWPD05340
                        nLen += 1;
						return nLen;
					}

					unsigned long nURLBillInfo;
					memcpy(&nURLBillInfo, pNextExt->value.information.value, sizeof(unsigned long));
					nURLBillInfo = NTOHL(nURLBillInfo);

					strcpy(&pTempBuffer[nLen], "URLBillInfo:");
					nLen += strlen("URLBillInfo:");
					nLen += NumToDecStr(nURLBillInfo, &pTempBuffer[nLen], nLength - nLen, FALSE);
					pTempBuffer[nLen++] = ' ';
					pNextExt = pNextExt->next;
					break;
				}

				//URLRateInformation
				case 16:
				{
					if ((pNextExt->value.information.value == NULL) 
					 || (pNextExt->value.information.length != sizeof(unsigned short)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-27 问题单SWPD05340
                        nLen += 1;
						return nLen;
					}

					unsigned short nURLRateInfo;
					memcpy(&nURLRateInfo, pNextExt->value.information.value, sizeof(unsigned short));
					nURLRateInfo = NTOHS(nURLRateInfo);

					strcpy(&pTempBuffer[nLen], "URLRateInfo:");
					nLen += strlen("URLRateInfo:");
					nLen += NumToDecStr(nURLRateInfo, &pTempBuffer[nLen], nLength - nLen, FALSE);
					pTempBuffer[nLen++] = ' ';
					pNextExt = pNextExt->next;
					break;
				}

				//ContentRecordClose
				case 17:
				{
					if ((pNextExt->value.information.value == NULL)
					 || (pNextExt->value.information.length != sizeof(unsigned char)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-27 问题单SWPD05340
                        nLen += 1;
						return nLen;
					}

					strcpy(&pTempBuffer[nLen], "ContentRecordClose:");
					nLen += strlen("ContentRecordClose:");
					unsigned char nContentRecordClose;
					memcpy(&nContentRecordClose, pNextExt->value.information.value, sizeof(unsigned char));
					switch (nContentRecordClose)
					{
						case 0:
						{
							strcpy(&pTempBuffer[nLen], "PDP normal deactive");
							nLen += strlen("PDP normal deactive");
							break;
						}

						case 1:
						{
							strcpy(&pTempBuffer[nLen], "URL counter");
							nLen += strlen("URL counter");
							break;
						}

						case 2:
						{
							strcpy(&pTempBuffer[nLen], "Volume limit");
							nLen += strlen("Volume limit");
							break;
						}

						case 3:
						{
							strcpy(&pTempBuffer[nLen], "Time limit");
							nLen += strlen("Time limit");
							break;
						}
			
						case 4:
						{
							strcpy(&pTempBuffer[nLen], "Time out");
							nLen += strlen("Time out");
							break;
						}


						case 5:
						{
							strcpy(&pTempBuffer[nLen], "Manual handover");
							nLen += strlen("Manual handover");
							break;
						}

						case 6:
						{
							strcpy(&pTempBuffer[nLen], "System err");
							nLen += strlen("System err");
							break;
						}

						default:
						{
							strcpy(&pTempBuffer[nLen], "Reserved");
							nLen += strlen("Reserved");
							break;
						}
					}

					pTempBuffer[nLen++] = ' ';
					pNextExt = pNextExt->next;
					break;
				}

				//Reserve
				default:
				{
					pNextExt = pNextExt->next;
					break;
				}
			}
		}

		strcpy(&pTempBuffer[nLen], "}");
		nLen += 1;
	}

	return nLen;
}

/*
功能:
	 按GGSN定义的基于内容计费的recordExtensions域填写格式转文本（CMCC格式）。
输入:
	pValue	:	内容计费信息
	pBuffer	:	存放返回字符串的数据区
	nLength	:	pBuffer可用长度
输出:
	转换后的字符串的长度	
*/
int CMCC_R98_GCDRRecordExtensionsToStr(Org_ManagementExtensions_ *pData, char *pBuffer, int nLength)
{
	int nLen = 0;
	
	if ((pData == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;	
	char *pTempBuffer = pBuffer;
	Org_ManagementExtensions_* pNextExt = pData;

	//recordExtensions目前只定义了19个域
	while (pNextExt)
	{
		int nServiceNum = 0;
		char szTemp[256] = "";
		unsigned long nServiceID;
		unsigned long nUpFlow;
		unsigned long nDownFlow;

		strcpy(&pTempBuffer[nLen], "{");
		nLen += 1;

		for (int j = 0; j < 19; j++)
		{
			if (pNextExt == NULL)
			{
				return nLen;
			}

			switch (j)
			{
				//ExetensionType
			    case 0:
				{
					if ((pNextExt->value.information.value == NULL) 
					 || (pNextExt->value.information.length != sizeof(unsigned char)))
					{
						return nLen;
					}
					unsigned char nExetensionType;
					memcpy(&nExetensionType, pNextExt->value.information.value, sizeof(unsigned char));					
					if (nExetensionType == 1)
					{
						strcpy(szTemp, "ExetensionType:CBB ");
					}
					else
					{
						sprintf(szTemp, "ExetensionType:Unknown(%d) ", nExetensionType);
					}					

					memcpy(&pTempBuffer[nLen], szTemp, strlen(szTemp));
					nLen += strlen(szTemp);
                    pNextExt = pNextExt->next;
					break;
				}
				
				//ServiceNum 
				case 1:
				{
					if ((pNextExt->value.information.value == NULL) 
					 || (pNextExt->value.information.length != sizeof(unsigned long)))
					{
						return nLen;
					}

					memcpy(&nServiceNum, pNextExt->value.information.value, sizeof(unsigned long));
					nServiceNum = NTOHL(nServiceNum);
					sprintf(szTemp, "ServiceNum:%u ", nServiceNum);
					memcpy(&pTempBuffer[nLen], szTemp, strlen(szTemp));
					nLen += strlen(szTemp);	
                    pNextExt = pNextExt->next;
					break;
				}

				//ServiceID
				case 2:
				case 5:
				case 8:
				case 11:
				case 14:
				{
					if (nServiceNum)
					{						
					    if ((pNextExt->value.information.value == NULL) 
					     || (pNextExt->value.information.length != sizeof(unsigned long)))
					    {
    						return nLen;
	    				}

					    memcpy(&nServiceID, pNextExt->value.information.value, sizeof(unsigned long));
					    nServiceID = NTOHL(nServiceID);					
					    sprintf(szTemp, "{ServiceID:%u ", nServiceID);
                        memcpy(&pTempBuffer[nLen], szTemp, strlen(szTemp));
                        nLen += strlen(szTemp);					
				    }
                    pNextExt = pNextExt->next;
					break;
				}

				//UpFlow
				case 3:
				case 6:
				case 9:
				case 12:
				case 15:
				{
					if (nServiceNum)
					{
					    if ((pNextExt->value.information.value == NULL) 
					     || (pNextExt->value.information.length != sizeof(unsigned long)))
					    {
    						return nLen;
	    				}

					    memcpy(&nUpFlow, pNextExt->value.information.value, sizeof(unsigned long));
					    nUpFlow = NTOHL(nUpFlow);					
					    sprintf(szTemp, "UpFlow:%u ", nUpFlow);
                        memcpy(&pTempBuffer[nLen], szTemp, strlen(szTemp));
                        nLen += strlen(szTemp);					
				    }
                    pNextExt = pNextExt->next;
					break;
				}

				//DownFlow
				case 4:
				case 7:
				case 10:
				case 13:
				case 16:
				{
					if (nServiceNum)
					{
						nServiceNum--;
						
					    if ((pNextExt->value.information.value == NULL) 
					     || (pNextExt->value.information.length != sizeof(unsigned long)))
					    {
    						return nLen;
	    				}

					    memcpy(&nDownFlow, pNextExt->value.information.value, sizeof(unsigned long));
					    nDownFlow = NTOHL(nDownFlow);					
					    sprintf(szTemp, "DownFlow:%u}", nDownFlow);
                        memcpy(&pTempBuffer[nLen], szTemp, strlen(szTemp));
                        nLen += strlen(szTemp);					
				    }
                    pNextExt = pNextExt->next;
					break;
				}

				//Reserve
				default:
				{
                    pNextExt = pNextExt->next;
					break;
				}
			}
		}

		strcpy(&pTempBuffer[nLen], "}");
		nLen += 1;
	}

	return nLen;	
}


};


