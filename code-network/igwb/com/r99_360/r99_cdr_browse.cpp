/*****************************************************
//		3GCDRBrowse.cpp
//*****************************************************
//Copyright 2001 by Huawei CO.,LTD.
// All rights reserved.
// Compiler:     Visual C++ 6.0
// Author:       Libo
// Date:         01-4-24
// Description:  该文件实现了WCDMA话单浏览的主要功能
// 
// Side Effects: NONE
// Functions:
// Notes:    
// 
// Update: 01-4-24 下午 02:13:07
// Date              Name              Description 
============== ================ ======================
## 01-4-24       Libo
*****************************************************/
#include "r99_cdr_browse.h"

enum CBB_OPERATOR_ID g_nCBB_OI;

#define NETWORKTOMSDIRECTION 1
#define MSTONETWORKDIRECTION NULL

//by ldf 2003-11-05 对应问题单 SWPD00365，按照和Moto沟通结果进行Qos域的缩写
#define QOSRELIABILITYENUMINFO_LENGTH 6
static ENUMITEMINFO QosReliabilityEnumInfo[] = 
{
	0, "Unspecified", //"Unspecified Reliability",
	1, "Ack GTP", //"Acknowledged GTP",
	2, "Unack GTP Ack LLC", //"Unack GTP Acknow LLC",
	3, "Unack GTP LLC Ack RLC", //"Unack GTP LLC Acknow RLC",
	4, "Unack GTP LLC RLC", //"Unack GTP LLC RLC",
	5, "Unack Unprotected Data" //"Unack now Unprotected Data"
};

#define QOSDELAYENUMINFO_LENGTH 4
static ENUMITEMINFO QoSDelayEnumInfo[] = 
{
	1, "1", //"Delay Class1",
	2, "2", //"Delay Class2",
	3, "3", //"Delay Class3",
	4, "4" //"Delay Class4"
};

#define QOSPRECEDENCEENUMINFO_LENGTH 4
static ENUMITEMINFO QoSPrecedenceEnumInfo[] = 
{
	0, "Unspecified", //"QoS Precedence Unspecified",
	1, "High", //"High Priority",
	2, "Normal", //"Normal Priority",
	3, "Low" //"Low Priority"
};

#define QOSPEAKTHROUGHPUTENUMINFO_LENGTH 10
static ENUMITEMINFO QoSPeakThroughputEnumInfo[] = 
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

#define QOSMEANTHROUGHPUTENUMINFO_LENGTH 19
static ENUMITEMINFO QoSMeanThroughputEnumInfo[] = 
{
	0, "Best Effort",
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

#define CHANGECONDITIONENUMINFO_LENGTH 3
static ENUMITEMINFO ChangeConditionEnumInfo [] =
{
	0, "QoS Change",
	1, "Tariff Time",
	2, "Record Closure"
};

//R99协议版本需要增加21, "intraSGSN IntersystemChange"，ggf，2004.4.7
#define CAUSEFORRECCLOSINGENUNINFO_LENGTH	10
static ENUMITEMINFO CauseForRecClosingEnumInfo [] = 
{
	0, "Normal Release",
	4, "Abnormal Release",
	5, "CAMEL Init Call Release",
	16, "Volume Limit",
	17, "Time Limit",
	18, "SGSN Change",
	19, "maxChangeCond",
	20, "Management Intervention",
	21, "intraSGSN IntersystemChange",
	30, "Content"
};

#define APNSELECTIONMODEENUMINFO_LENGTH 3
static ENUMITEMINFO ApnSelectionModeEnumInfo [] = 
{
	0, "MS or Network Provided Subscription Verified",
	1, "MS Provided Subscription Not Verified",
	2, "Network Provided Subscription Not Verified"
};

//Z30971增加合并结果为onlyOneCDRGenerated的情况
#define CONSOLIDATIONRESULTENUMINFO_LENGTH 5
static ENUMITEMINFO ConsolidationResultEnumInfo [] = 
{
	0, "normal",
	1, "notNormal",
	2, "forInterSGSNConsolidation",
	3, "reachLimit",
	4, "onlyOneCDRGenerated"
};

//Added by ZhengYuqun 2001/09/29
#define QOS99RELIABILITYENUMINFO_LENGTH 8
static ENUMITEMINFO Qos99ReliabilityEnumInfo[] = 
{
	0, "Subscribed", //"Subscribed reliability class",
	1, "Ack GTP,LLC,RLC;Protected data", //"Acknowledged GTP, LLC, and RLC; Protected data",
	2, "Unack GTP; Ack LLC,RLC,Protected data", //"Unacknowledged GTP; Acknowledged LLC and RLC, Protected data",
	3, "Unack GTP,LLC; Ack RLC,Protected data", //"Unacknowledged GTP and LLC; Acknowledged RLC, Protected data",
	4, "Unack GTP,LLC,RLC,Protected data", //"Unacknowledged GTP, LLC, and RLC, Protected data",
	5, "Unack GTP,LLC,RLC,Unprotected data", //"Unacknowledged GTP, LLC, and RLC, Unprotected data",
	6, "Unack GTP,LLC; Ack RLC,Protected data",  //Added by ZhengYuqun SWPD02045 2003-10-24
	7, "Reserved" //"Reserved" 
};

#define QOS99DELAYENUMINFO_LENGTH 8
static ENUMITEMINFO QoS99DelayEnumInfo[] = 
{
	0,	"Subscribed", //"Subscribed delay class",
	1,	"1", //"Delay class 1",
	2,	"2", //"Delay class 2",
	3,	"3", //"Delay class 3",
	4,	"4(Best Effort)", //"Delay class 4 (best effort)",
	5,	"4(Best Effort)", //"Delay Class 4 (best effort)",	//增加5和6，Added by ZhengYuqun SWPD02045 2003-10-24 
	6,	"4(Best Effort)", //"Delay Class 4 (best effort)",
	7,	"Reserved" //"Reserved"
};

#define QOS99RECEDENCEENUMINFO_LENGTH 8
static ENUMITEMINFO QoS99PrecedenceEnumInfo[] = 
{
	0,	"Subscribed", //"Subscribed precedence",
	1,	"High", //"High priority",
	2,	"Normal", //"Normal priority",
	3,	"Low", //"Low priority",
	4,	"Normal", //"Normal priority",	//增加4、5和6，Added by ZhengYuqun SWPD02045 2003-10-24
	5,	"Normal", //"Normal priority",
	6,	"Normal",	//"Normal priority",
	7,	"Reserved" //"Reserved"
};

#define QOS99PEAKTHROUGHPUTENUMINFO_LENGTH 16
static ENUMITEMINFO QoS99PeakThroughputEnumInfo[] = 
{
	0,  "Subscribed", //"Subscribed peak throughput",
	1,  "1000octet/s", //"Up to 1000 octet/s",
	2,  "2000octet/s", //"Up to 2000 octet/s",
	3,  "4000octet/s", //"Up to 4000 octet/s",
	4,  "8000octet/s", //"Up to 8000 octet/s",
	5,  "16000octet/s", //"Up to 16000 octet/s",
	6,  "32000octet/s", //"Up to 32000 octet/s",
	7,  "64000octet/s", //"Up to 64000 octet/s",
	8,  "128000octet/s", //"Up to 128000 octet/s",
	9,  "256000octet/s", //"Up to 256000 octet/s",
	10, "1000octet/s", //"Up to 1000 octet/s",	//增加10到14，Added by ZhengYuqun SWPD02045 2003-10-24
	11, "1000octet/s", //"Up to 1000 octet/s",
	12, "1000octet/s", //"Up to 1000 octet/s",
	13, "1000octet/s", //"Up to 1000 octet/s",
	14, "1000octet/s", //"Up to 1000 octet/s",
	15, "Reserved" //"Reserved"
};

#define QOS99MEANTHROUGHPUTENUMINFO_LENGTH 32
static ENUMITEMINFO QoS99MeanThroughputEnumInfo[] = 
{
	0,  "Subscribed", //"Subscribed mean throughput",
	1,  "100octet/h", //"100 octet/h",
	2,  "200octet/h", //"200 octet/h",
	3,  "500octet/h", //"500 octet/h",
	4,  "1000octet/h", //"1 000 octet/h",
	5,  "2000octet/h", //"2 000 octet/h",
	6,  "5000octet/h", //"5 000 octet/h",
	7,  "10000octet/h", //"10 000 octet/h",
	8,  "20000octet/h", //"20 000 octet/h",
	9,  "50000octet/h", //"50 000 octet/h",
	10, "100000octet/h", //"100 000 octet/h",
	11, "200000octet/h", //"200 000 octet/h",
	12, "500000octet/h", //"500 000 octet/h",
	13, "1000000octet/h", //"1 000 000 octet/h",
	14, "2000000octet/h", //"2 000 000 octet/h",
	15, "5000000octet/h", //"5 000 000 octet/h",
	16, "10000000octet/h", //"10 000 000 octet/h",
	17, "20000000octet/h", //"20 000 000 octet/h",
	18, "50000000octet/h", //"50 000 000 octet/h",
	19, "Best Effort", //"Best effort",		//Added by ZhengYuqun SWPD02045	2003-10-24
	20, "Best Effort", //"Best effort",
	21, "Best Effort", //"Best effort",
	22, "Best Effort", //"Best effort",
	23, "Best Effort", //"Best effort",
	24, "Best Effort", //"Best effort",
	25, "Best Effort", //"Best effort",
	26, "Best Effort", //"Best effort",
	27, "Best Effort", //"Best effort",
	28, "Best Effort", //"Best effort",
	29, "Best Effort", //"Best effort",
	30,	"Reserved", //"Reserved",
	31, "Best Effort" //"Best effort"
};

#define QOS99DELIVERYOFERRONEOUSSDUSENUMINFO_LENGTH 5
static ENUMITEMINFO QoS99DeliveryOfErroneousSDUsEnumInfo[] = 
{
	0,	"Subscribed", //"Subscribed delivery of erroneous SDUs",
	1,	"-", //"No detect ('-')",
	2,	"Yes", //"Erroneous SDUs are delivered ('yes')",
	3, 	"No", //"Erroneous SDUs are not delivered ('no')",
	7,	"Reserved" //"Reserved"
};

#define QOS99DELIVERYORDERENUMINFO_LENGTH 4
static ENUMITEMINFO QoS99DeliveryOrderEnumInfo[] = 
{
	0,	"Subscribed", //"Subscribed delivery order",
	1,	"Yes", //"With delivery order ('yes')",
	2,	"No", //"Without delivery order ('no')",
	3,	"Reserved" //"Reserved"
};

#define QOS99TRAFFICCLASSENUMINFO_LENGTH 6
static ENUMITEMINFO QoS99TrafficClassEnumInfo[] = 
{
	0,	"Subscribed", //"Subscribed traffic class",
	1,	"Conversational", //"Conversational class",
	2,	"Stream", //"Streaming class",
	3,	"Interactive", //"Interactive class",
	4,	"Background", //"Background class",
	7,	"Reserved" //"Reserved"
};

#define QOS99MAXIMUMSDUSIZEENUMINFO_LENGTH 5
static ENUMITEMINFO QoS99MaximumSDUSizeEnumInfo[] = 
{
	0,	 "Subscribed", //"Subscribed maximum SDU size",
	//1 - 150 为 "10 octets"到"1500 octets"，在程序中写
	151, "1502octets", //"1502 octets",
	152, "1510octets", //"1510 octets", 
	153, "1520octets", //"1520 octets",
	255, "Reserved" //"Reserved"
};

#define QOS99MAXIMUMBITRATEFORUPLINKENUMINFO_LENGTH 2
static ENUMITEMINFO QoS99MaximumBitRateForUplinkEnumInfo[] = 
{
	0, "Subscribed", //"Subscribed maximum bit rate for uplink",
	//1 - 63 为  1 kbps 到 63 kbps，在程序中写
	//64 - 127 为  64 kbps 到 568 kbps，在程序中写
	//128 - 254  为 576 kbps 到 8640 kbps，在程序中写
	255, "0kbps"
};

#define QOS99MAXIMUMBITRATEFORDOWNLINKENUMINFO_LENGTH 2
static ENUMITEMINFO QoS99MaximumBitRateForDownlinkEnumInfo[] = 
{
	0, "Subscribed", //"Subscribed maximum bit rate for downlink",
	//1 - 63 为  1 kbps 到 63 kbps，在程序中写
	//64 - 127 为  64 kbps 到 568 kbps，在程序中写
	//128 - 254  为 576 kbps 到 8640 kbps，在程序中写
	255, "0kbps"
};

#define QOS99RESIDUALBERENUMINFO_LENGTH 11
static ENUMITEMINFO QoS99ResidualBEREnumInfo[] = 
{
	0,  "Subscribed", //"Subscribed residual BER",
	1,  "5*10-2", //"5*10-2",
	2,  "1*10-2", //"1*10-2",
	3,  "5*10-3", //"5*10-3",
	4,  "4*10-3", //"4*10-3",
	5,  "1*10-3", //"1*10-3",
	6,  "1*10-4", //"1*10-4",
	7,  "1*10-5", //"1*10-5",
	8,  "1*10-6", //"1*10-6",
	9,  "6*10-8", //"6*10-8",
	15, "Reserved" //"Reserved"
};

#define QOS99SDUERRORRATIOENUMINFO_LENGTH 9
static ENUMITEMINFO QoS99SDUErrorRatioEnumInfo[] = 
{
	0,  "Subscribed", //"Subscribed SDU error ratio",
	1,  "1*10-2", //"1*10-2",
	2,  "7*10-3", //"7*10-3",
	3,  "1*10-3", //"1*10-3",
	4,  "1*10-4", //"1*10-4",
	5,  "1*10-5", //"1*10-5",
	6,  "1*10-6", //"1*10-6",
	7,  "1*10-1", //"1*10-1",
	15,	"Reserved" //"Reserved"
};

#define QOS99TRAFFICHANDLINGPRIORITYENUMINFO_LENGTH 4  //2改为4  ZhengYuqun 2003-09-10  SWPD01505
static ENUMITEMINFO QoS99TrafficHandlingPriorityEnumInfo[] = 
{
	0, "Subscribed", //"Subscribed traffic handling priority",
	1, "1", //"Priority level 1",
	2, "2", //"Priority level 2",
	3, "3" //"Priority level 3"
};

#define QOS99TRANSFERDELAYENUMINFO_LENGTH 2
static ENUMITEMINFO QoS99TransferDelayEnumInfo[] = 
{
	0,  "Subscribed", //"Subscribed transfer delay",
	//1 - 15  为 10 ms 到 150 ms，程序中写
	//16 - 31 为 200 ms 到 950 ms，程序中写
	//32 - 62 为 1000 ms 到 4100 ms，程序中写
	63, "Reserved"
};

#define QOS99GUARANTEEDBITRATEFORUPLINKENUMINFO_LENGTH 2
static ENUMITEMINFO QoS99GuaranteedBitRateForUplinkEnumInfo[] = 
{
	0, "Subscribed", //"Subscribed maximum bit rate for uplink",
	//1 - 63 为  1 kbps 到 63 kbps，在程序中写
	//64 - 127 为  64 kbps 到 568 kbps，在程序中写
	//128 - 254  为 576 kbps 到 8640 kbps，在程序中写
	255, "0kbps"
};

#define QOS99GUARANTEEDBITRATEFORDOWNLINKENUMINFO_LENGTH 2
static ENUMITEMINFO QoS99GuaranteedBitRateForDownlinkEnumInfo[] = 
{
	0, "Subscribed", //"Subscribed maximum bit rate for uplink",
	//1 - 63 为  1 kbps 到 63 kbps，在程序中写
	//64 - 127 为  64 kbps 到 568 kbps，在程序中写
	//128 - 254  为 576 kbps 到 8640 kbps，在程序中写
	255, "0kbps"
};
//End Of Adding

//added by maimaoshi at 2002-05-31(D13999)
#define SYSTEMTYPEENUMINFO_LENGTH 3
static ENUMITEMINFO SYSTEMTYPEENUMINFOEnumInfo[] = 
{
	0, "unknown",
	1, "iuUTRAN",
	2, "gERAN"	
};
//end by maimaoshi

//added by zouyongsheng 2002-08-09
#define DEFAULTTRANSACTIONHANDLINGENUMINFO_LENGTH 2
static ENUMITEMINFO DEFAULTTRANSACTIONHANDLINGENUMINFO[] =
{
	0, "DefaultGPRS Handling continueTransaction",   
	1, "DefaultGPRS Handling releaseTransaction" 
};

#define LEVELOFCAMELSERVICE_LENGTH 3
static ENUMITEMINFO LEVELOFCAMELSERVICE[] = 
{
    0x80, "basic",
    0x40, "callDurationSupervision",
    0x20, "onlineCharging"
};

#define DEFAULTSMSHANDLINGENUMINFO_LENGTH 2
static ENUMITEMINFO DEFAULTSMSHANDLINGENUMINFO[] =
{
	0, "DefaultSMS Handling continueTransaction",
	1, "DefaultSMS Handling releaseTransaction"
};
//end

#define R99_360_RECORDE_TYPE_ENUNINFO_LENGTH 5
static ENUMITEMINFO R99_360_Recorde_Type_EnumInfo[] =
{
    18,	"S-CDR",
    19,	"G-CDR",
    20,	"M-CDR",
    21,	"S-SMO-CDR",
    22,	"S-SMT-CDR"
};

/******************************************************
// Function name: 	GetAllFieldTextValueOf3GCDR
// Author:        Libo
// Date:          01-4-24
// Description:   3G话单浏览处理的入口函数
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
//01-4-24       Libo
******************************************************/
int GetAllFieldTextValueOf3GCDR(char *pCDR,
                                int nFormatType,
                                char *pBuffer,
                                int nLength)
{
	int nLen = 0;
	switch (nFormatType)
	{
	case S_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf3GSGSNPDP(pCDR, pBuffer, nLength);
		}
		break;
	case G_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf3GGGSNPDP(pCDR, pBuffer, nLength);
		}
		break;
	case M_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf3GSGSNMM(pCDR, pBuffer, nLength);
		}
		break;
	case S_SMO_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf3GSGSNSMO(pCDR, pBuffer, nLength);
		}
		break;
	case S_SMT_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf3GSGSNSMT(pCDR, pBuffer, nLength);
		}
		break;
	case L_S_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf3GLARGESGSNPDP(pCDR, pBuffer, nLength);
		}
		break;
	case L_G_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf3GLARGEGGSNPDP(pCDR, pBuffer, nLength);
		}
		break;
	/*	
	case L_M_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf3GLARGESGSNMM(pCDR, pBuffer, nLength);
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
// Function name: 	GetQueryFieldTextValueOf3GCDR
// Author:        XDB
// Date:          2001.11.07
// Description:   获取3G话单查询条件域值入口函数
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
//01-4-24       Libo
******************************************************/
int GetQueryFieldTextValueOf3GCDR(char *pCDR,
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
			nLen = GetTextValueOf_Q_3GSGSNPDP(pCDR, pBuffer, nLength, pValues);
		}
		break;
	case G_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf_Q_3GGGSNPDP(pCDR, pBuffer, nLength, pValues);
		}
		break;
	case M_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf_Q_3GSGSNMM(pCDR, pBuffer, nLength, pValues);
		}
		break;
	case S_SMO_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf_Q_3GSGSNSMO(pCDR, pBuffer, nLength, pValues);
		}
		break;
	case S_SMT_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf_Q_3GSGSNSMT(pCDR, pBuffer, nLength, pValues);
		}
		break;
	case L_S_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf_Q_3GLARGESGSNPDP(pCDR, pBuffer, nLength, pValues);
		}
		break;
	case L_G_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf_Q_3GLARGEGGSNPDP(pCDR, pBuffer, nLength, pValues);
		}
		break;
	/*
	case L_M_CDR_FORMATTYPE:
		{
			nLen = GetTextValueOf_Q_3GLARGESGSNMM(pCDR, pBuffer, nLength, pValues);
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
// Function name: 	GetTextValueOf3GSGSNPDP
// Author:        Libo
// Date:          01-4-28
// Description:   处理S-CDR话单浏览解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-4-28       Libo
******************************************************/
int GetTextValueOf3GSGSNPDP(char *pValue, char *pBuffer, int nLength)
{
//#define SCDRBUFFERLENGTH 65536   //Commented by ZhengYuqun 2001/09/26
#define SCDRBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	CDR3G_SGSNPDPRecord *pBill = (CDR3G_SGSNPDPRecord*)pValue;
	int  nLen = 0;  
    int nTempLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != CDR3G_CallEventRecordType_sgsnPDPRecord)
		return 0;

	//char *pTempBuffer = new char[SCDRBUFFERLENGTH];
	//Modified By ZhengYuqun 2001/09/26
	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;
	
    //recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R99_360_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R99_360_Recorde_Type_EnumInfo;
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
	if (pBill->bit_mask & CDR3G_SGSNPDPRecord_networkInitiation_present)
	{
		nLen += BoolToStr(pBill->networkInitiation, &pTempBuffer[nLen], 
			nLength - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//servedIMSI
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI, 
							&pTempBuffer[nLen], nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//servedIMEI
	if (pBill->bit_mask & CDR3G_SGSNPDPRecord_servedIMEI_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMEI,
							&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//sgsnAddress
	nLen += _3GIPAddrToStr(&pBill->sgsnAddress, &pTempBuffer[nLen], nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//msNetworkCapability
	if (pBill->bit_mask & CDR3G_SGSNPDPRecord_msNetworkCapability_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->msNetworkCapability, &pTempBuffer[nLen], 
							nLength - nLen, "0x", FALSE);							
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
	
	//routingArea
	if (pBill->bit_mask & CDR3G_SGSNPDPRecord_routingArea_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->routingArea, &pTempBuffer[nLen],
							nLength - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//locationAreaCode
	if (pBill->bit_mask & CDR3G_SGSNPDPRecord_locationAreaCode_present)
	{
		//modified by Libo on Jul 5, 2001 for D03483
		//nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationAreaCode, &pTempBuffer[nLen],
		//					SCDRBUFFERLENGTH - nLen, "0x", TRUE);
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationAreaCode, &pTempBuffer[nLen],
							nLength - nLen, "0x", FALSE);
		//modify end.
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//cellIdentifier
	if (pBill->bit_mask & CDR3G_SGSNPDPRecord_cellIdentifier_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->cellIdentifier, &pTempBuffer[nLen],
							nLength - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//chargingID
	nLen += NumToDecStr(pBill->chargingID, &pTempBuffer[nLen], 
						nLength - nLen, FALSE);
	pTempBuffer[nLen++] = '^';

	//ggsnAddressUsed
	nLen += _3GIPAddrToStr(&pBill->ggsnAddressUsed, &pTempBuffer[nLen], nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//accessPointNameNI
	strcpy(&pTempBuffer[nLen], pBill->accessPointNameNI);
	nLen += strlen(pBill->accessPointNameNI);
	pTempBuffer[nLen++] = '^';

	//pdpType
	//modified by Libo on Jul 5, 2001 for D03483
	//nLen += HexOctecToStr((LPOCTECSTRING)&pBill->pdpType, &pTempBuffer[nLen],
	//					SCDRBUFFERLENGTH - nLen, "0x", TRUE);
	nLen += HexOctecToStr((LPOCTECSTRING)&pBill->pdpType, &pTempBuffer[nLen],
						nLength - nLen, "0x", FALSE);
	//modified end.
	pTempBuffer[nLen++] = '^';

	//Added By maimaoshi at 2002-05-31(D13999)
	//servedPDPAddress
	if (pBill->bit_mask & CDR3G_SGSNPDPRecord_servedPDPAddress_present)
	{
		nLen += _3GPDPAddrToStr(&pBill->servedPDPAddress, &pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
	//End by maimaoshi

    /************ Deleted by XDB on 2002.1.7 *********************************/
	//listOfTrafficVolumes
	//ListOfTrafficVolumesToStr增加一个参数 Modified by ZhengYuqun 2001/09/28
	CDR3G_NetworkInitiatedPDPContext networkInitiation = pBill->bit_mask & CDR3G_SGSNPDPRecord_networkInitiation_present ? pBill->networkInitiation : NULL;
	nLen += ListOfTrafficVolumesToStr(&pBill->listOfTrafficVolumes, &pTempBuffer[nLen], 
										nLength - nLen, networkInitiation);
	pTempBuffer[nLen++] = '^';
    /************ End of deleted by XDB on 2002.1.7 **************************/

	//recordOpeningTime
	nLen += TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime, &pTempBuffer[nLen],
							nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//duration
	nLen += NumToDecStr(pBill->duration, &pTempBuffer[nLen], nLength - nLen, FALSE);
	pTempBuffer[nLen++] = '^';

	//sgsnChange
	if (pBill->bit_mask & CDR3G_SGSNPDPRecord_sgsnChange_present)
	{
		nLen += BoolToStr(pBill->sgsnChange, &pTempBuffer[nLen], nLength - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//causeForRecClosing
	EnumInfo.nCount = CAUSEFORRECCLOSINGENUNINFO_LENGTH;
	EnumInfo.pInfoTable = CauseForRecClosingEnumInfo;
	nTempLen = EnumToStr(&EnumInfo, pBill->causeForRecClosing,&pTempBuffer[nLen], nLength - nLen);
	
    /*  //Modified by XDB on 2001.11.09 */
    if (nTempLen)
		nLen += nTempLen;
	else
	{
		strcpy(&pTempBuffer[nLen], "Unknown reason");
		nLen += strlen("Unknown reason");
	}
	pTempBuffer[nLen++] = '^';

	//diagnostics
	if (pBill->bit_mask & CDR3G_SGSNPDPRecord_diagnostics_present)
	{
		nLen += DiagnosticsToStr(&pBill->diagnostics, &pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//recordSequenceNumber
	if (pBill->bit_mask & CDR3G_SGSNPDPRecord_recordSequenceNumber_present)
	{
		nLen += NumToDecStr(pBill->recordSequenceNumber, &pTempBuffer[nLen], 
							nLength - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//nodeID
	if (pBill->bit_mask & CDR3G_SGSNPDPRecord_nodeID_present)
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

    /************ Deleted by XDB on 2002.1.7 **********************************/
	//recordExtensions
	if (pBill->bit_mask & CDR3G_SGSNPDPRecord_recordExtensions_present)
	{
		nLen += RecordExtensionsToStr(&pBill->recordExtensions, &pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
    /************ End of deleted by XDB on 2002.1.7 **************************/

	//localSequenceNumber
	if (pBill->bit_mask & CDR3G_SGSNPDPRecord_localSequenceNumber_present)
	{
		nLen += NumToDecStr(pBill->localSequenceNumber, &pTempBuffer[nLen], nLength - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//apnSelectionMode
	if (pBill->bit_mask & CDR3G_SGSNPDPRecord_apnSelectionMode_present)
	{
		EnumInfo.nCount = APNSELECTIONMODEENUMINFO_LENGTH;
		EnumInfo.pInfoTable = ApnSelectionModeEnumInfo;
		nLen += EnumToStr(&EnumInfo, pBill->apnSelectionMode, &pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//accessPointNameOI
	strcpy(&pTempBuffer[nLen], pBill->accessPointNameOI);
	nLen += strlen(pBill->accessPointNameOI);
	pTempBuffer[nLen++] = '^';

	//servedMSISDN
	if (pBill->bit_mask & CDR3G_SGSNPDPRecord_servedMSISDN_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedMSISDN, &pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//chargingCharacteristics
	if (pBill->bit_mask & CDR3G_SGSNPDPRecord_chargingCharacteristics_present)
	{
		nLen += ChargingCharacteristicsToStr(&pBill->chargingCharacteristics, &pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	
	//systemType
	if (pBill->bit_mask & CDR3G_SGSNPDPRecord_systemType_present)
	{
		//Added By maimaoshi at 2002-05-31(D13999)
		EnumInfo.nCount = SYSTEMTYPEENUMINFO_LENGTH;
		EnumInfo.pInfoTable = SYSTEMTYPEENUMINFOEnumInfo;
		nLen += EnumToStr(&EnumInfo, pBill->systemType, &pTempBuffer[nLen], nLength - nLen);

		//nLen += SystemTypeToStr(&pBill->systemType, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
		
		//End By maimaoshi 
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//added by zouyongsheng  2002-08-09
	//cAMELInfomationPDP
	if(pBill->bit_mask & CDR3G_cAMELInformationPDP_present)
	{
		nLen += _3GSGSNPDP_CAMELInformationToStr(&pBill->cAMELInformationPDP, &pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
	//cAMELInfomationPDP end 
	
	//rNCUnsentDownlinkVolume
	if (pBill->bit_mask & CDR3G_rNCUnsentDownlinkVolume_present)
	{
		nLen += NumToDecStr(pBill->rNCUnsentDownlinkVolume, &pTempBuffer[nLen], nLength - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	pTempBuffer[nLen] = 0;

	//Modifed By ZhengYuqun 2001/09/26
	//if (nLen < nLength)
	//	memcpy(pBuffer, pTempBuffer, nLen + 1);
	//else
	//	nLen = 0;
	if (nLen >= nLength)
		nLen = 0;

	//delete []pTempBuffer; Commented By ZhengYuqun 2001/09/26
	return nLen;
}

/******************************************************
// Function name: 	GetTextValueOf3GGGSNPDP
// Author:        Libo
// Date:          01-4-28
// Description:   处理G-CDR话单浏览解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-4-28       Libo
******************************************************/
int GetTextValueOf3GGGSNPDP(char *pValue, char *pBuffer, int nLength)
{
//#define GCDRBUFFERLENGTH 65536   //Commented by ZhengYuqun 2001/09/26
#define GCDRBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	CDR3G_GGSNPDPRecord *pBill = (CDR3G_GGSNPDPRecord*)pValue;
	int nLen = 0;
    int nTempLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != CDR3G_CallEventRecordType_ggsnPDPRecord)
		return 0;

	//char *pTempBuffer = new char[GCDRBUFFERLENGTH];
	//Modified By ZhengYuqun 2001/09/26
	char *pTempBuffer = pBuffer;
	
	if (!pTempBuffer)
		return 0;
	
    //recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R99_360_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R99_360_Recorde_Type_EnumInfo;
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
	if (pBill->bit_mask & CDR3G_GGSNPDPRecord_networkInitiation_present)
	{
		nLen += BoolToStr(pBill->networkInitiation, &pTempBuffer[nLen], 
							nLength - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//servedIMSI
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI, 
							&pTempBuffer[nLen], nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//ggsnAddress
	nLen += _3GIPAddrToStr(&pBill->ggsnAddress, &pTempBuffer[nLen], nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//chargingID
	nLen += NumToDecStr(pBill->chargingID, &pTempBuffer[nLen], 
						nLength - nLen, FALSE);
	pTempBuffer[nLen++] = '^';

	//sgsnAddress
	for (unsigned short i = 0; i < pBill->sgsnAddress.count; i++)
	{
		pTempBuffer[nLen++] = '{';
		nLen += _3GIPAddrToStr(&pBill->sgsnAddress.value[i], &pTempBuffer[nLen], 
								nLength - nLen);
		pTempBuffer[nLen++] = '}';
	}
	pTempBuffer[nLen++] = '^';

	//accessPointNameNI
	strcpy(&pTempBuffer[nLen], pBill->accessPointNameNI);
	nLen += strlen(pBill->accessPointNameNI);
	pTempBuffer[nLen++] = '^';

	//pdpType
	//modified by Libo on Jul 5, 2001 for D03483
	//nLen += HexOctecToStr((LPOCTECSTRING)&pBill->pdpType, &pTempBuffer[nLen],
	//					nLength - nLen, "0x", TRUE);
	nLen += HexOctecToStr((LPOCTECSTRING)&pBill->pdpType, &pTempBuffer[nLen],
						nLength - nLen, "0x", FALSE);
	//modify end
	pTempBuffer[nLen++] = '^';

	//Added bu maimaoshi at 2002-05-31(D13999)
	//servedPDPAddress
	if (pBill->bit_mask & CDR3G_GGSNPDPRecord_servedPDPAddress_present)
	{
		nLen += _3GPDPAddrToStr(&pBill->servedPDPAddress, &pTempBuffer[nLen], 
							nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
	//End by maimaoshi

	//dynamicAddressFlag
	if (pBill->bit_mask & CDR3G_dynamicAddressFlag_present)
	{
		nLen += BoolToStr(pBill->dynamicAddressFlag, &pTempBuffer[nLen], 
							nLength - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

    /************ Deleted by XDB on 2002.1.7 *********************************/
	//listOfTrafficVolumes
	//ListOfTrafficVolumesToStr增加一个参数 Modified by ZhengYuqun 2001/09/28
	CDR3G_NetworkInitiatedPDPContext networkInitiation = pBill->bit_mask & CDR3G_GGSNPDPRecord_networkInitiation_present ? pBill->networkInitiation : NULL;
	nLen += ListOfTrafficVolumesToStr(&pBill->listOfTrafficVolumes, &pTempBuffer[nLen], 
										nLength - nLen, networkInitiation);
	pTempBuffer[nLen++] = '^';
    /************ End of deleted by XDB on 2002.1.7 **************************/

	//recordOpeningTime
	nLen += TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime, &pTempBuffer[nLen],
							nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//duration
	nLen += NumToDecStr(pBill->duration, &pTempBuffer[nLen], 
						nLength - nLen, FALSE);
	pTempBuffer[nLen++] = '^';

	//causeForRecClosing
	EnumInfo.nCount = CAUSEFORRECCLOSINGENUNINFO_LENGTH;
	EnumInfo.pInfoTable = CauseForRecClosingEnumInfo;
	nTempLen = EnumToStr(&EnumInfo, pBill->causeForRecClosing,&pTempBuffer[nLen], 
						nLength - nLen);
    /*  //Modified by XDB on 2001.11.09 */
	if (nTempLen)
		nLen += nTempLen;
	else
	{
		strcpy(&pTempBuffer[nLen], "Unknown reason");
		nLen += strlen("Unknown reason");
	}
	pTempBuffer[nLen++] = '^';

	//diagnostics
	if (pBill->bit_mask & CDR3G_GGSNPDPRecord_diagnostics_present)
	{
		nLen += DiagnosticsToStr(&pBill->diagnostics, &pTempBuffer[nLen], 
									nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//recordSequenceNumber
	if (pBill->bit_mask & CDR3G_GGSNPDPRecord_recordSequenceNumber_present)
	{
		nLen += NumToDecStr(pBill->recordSequenceNumber, &pTempBuffer[nLen], 
							nLength - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//nodeID
	if (pBill->bit_mask & CDR3G_GGSNPDPRecord_nodeID_present)
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

    /************ Deleted by XDB on 2002.1.7 *********************************/
	//recordExtensions
	if (pBill->bit_mask & CDR3G_GGSNPDPRecord_recordExtensions_present)
	{
        switch (g_nCBB_OI)
        {			    
        case CBB_OPERATOR_ID_CMCC:
            nLen += CMCC_R99_GCDRRecordExtensionsToStr(&pBill->recordExtensions, &pTempBuffer[nLen], nLength - nLen);
            break;
            
        default:
            nLen += R99_GCDRRecordExtensionsToStr(&pBill->recordExtensions, &pTempBuffer[nLen], nLength - nLen);
            break;			    
        }		
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
    /************ End of deleted by XDB on 2002.1.7 **************************/

	//localSequenceNumber
	if (pBill->bit_mask & CDR3G_GGSNPDPRecord_localSequenceNumber_present)
	{
		nLen += NumToDecStr(pBill->localSequenceNumber, &pTempBuffer[nLen], 
							nLength - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//apnSelectionMode
	if (pBill->bit_mask & CDR3G_GGSNPDPRecord_apnSelectionMode_present)
	{
		EnumInfo.nCount = APNSELECTIONMODEENUMINFO_LENGTH;
		EnumInfo.pInfoTable = ApnSelectionModeEnumInfo;
		nLen += EnumToStr(&EnumInfo, pBill->apnSelectionMode, &pTempBuffer[nLen], 
							nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//servedMSISDN
	if (pBill->bit_mask & CDR3G_GGSNPDPRecord_servedMSISDN_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedMSISDN, &pTempBuffer[nLen], 
									nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//chargingCharacteristics
	if (pBill->bit_mask & CDR3G_GGSNPDPRecord_chargingCharacteristics_present)
	{
		nLen += ChargingCharacteristicsToStr(&pBill->chargingCharacteristics, 
											&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	pTempBuffer[nLen] = 0;
	
	//Modifed By ZhengYuqun 2001/09/26
	//if (nLen < nLength)
	//	memcpy(pBuffer, pTempBuffer, nLen + 1);
	//else
	//	nLen = 0;
	if (nLen >= nLength)
		nLen = 0;

	//delete []pTempBuffer; Commented By ZhengYuqun 2001/09/26
	return nLen;
}


/******************************************************
// Function name: 	GetTextValueOf3GSGSNMM
// Author:        Libo
// Date:          01-4-28
// Description:   处理M-CDR话单浏览解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-4-28       Libo
******************************************************/
int GetTextValueOf3GSGSNMM(char *pValue, char *pBuffer, int nLength)
{
//#define MCDRBUFFERLENGTH 65536   //Commented by ZhengYuqun 2001/09/26
#define MCDRBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	CDR3G_SGSNMMRecord *pBill = (CDR3G_SGSNMMRecord*)pValue;
	int nLen = 0;
    int nTempLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != CDR3G_CallEventRecordType_sgsnMMRecord)
		return 0;

	//char *pTempBuffer = new char[MCDRBUFFERLENGTH];
	//Modified By ZhengYuqun 2001/09/26
	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;

    //recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R99_360_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R99_360_Recorde_Type_EnumInfo;
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
							&pTempBuffer[nLen], nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//servedIMEI
	if (pBill->bit_mask & CDR3G_SGSNMMRecord_servedIMEI_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMEI,
							&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//sgsnAddress
	nLen += _3GIPAddrToStr(&pBill->sgsnAddress, &pTempBuffer[nLen], 
							nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//msNetworkCapability
	if (pBill->bit_mask & CDR3G_SGSNMMRecord_msNetworkCapability_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->msNetworkCapability, &pTempBuffer[nLen], 
							nLength - nLen, "0x", FALSE);							
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//routingArea
	if (pBill->bit_mask & CDR3G_SGSNMMRecord_routingArea_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->routingArea, &pTempBuffer[nLen],
								nLength - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//locationAreaCode
	if (pBill->bit_mask & CDR3G_SGSNMMRecord_locationAreaCode_present)
	{
		//modified by Libo on Jul 5, 2001 for D03483
		//nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationAreaCode, &pTempBuffer[nLen],
		//					MCDRBUFFERLENGTH - nLen, "0x", TRUE);
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationAreaCode, &pTempBuffer[nLen],
							nLength - nLen, "0x", FALSE);
		//modify end.
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//cellIdentifier
	if (pBill->bit_mask & CDR3G_SGSNMMRecord_cellIdentifier_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->cellIdentifier, &pTempBuffer[nLen],
							nLength - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

    /************ Deleted by XDB on 2002.1.11 ********************************/
	//changeLocation
	if (pBill->bit_mask & CDR3G_changeLocation_present)
	{
		nLen += ChangeLocationsToStr(&pBill->changeLocation, 
									&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
    /************ End of deleted by XDB on 2002.1.7 **************************/

	//recordOpeningTime
	nLen += TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime, &pTempBuffer[nLen],
							nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//duration
	if (pBill->bit_mask & CDR3G_duration_present)
	{
		nLen += NumToDecStr(pBill->duration, &pTempBuffer[nLen], 
							nLength - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//sgsnChange
	//update by maimaoshi,2003-02-28
	if (pBill->bit_mask & CDR3G_SGSNMMRecord_sgsnChange_present)
	{
		nLen += BoolToStr(pBill->sgsnChange, &pTempBuffer[nLen], 
							nLength - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//causeForRecClosing
	EnumInfo.nCount = CAUSEFORRECCLOSINGENUNINFO_LENGTH;
	EnumInfo.pInfoTable = CauseForRecClosingEnumInfo;
	nTempLen = EnumToStr(&EnumInfo, pBill->causeForRecClosing,
						&pTempBuffer[nLen], nLength - nLen);
    /*  //Modified by XDB on 2001.11.09 */
	if (nTempLen)
		nLen += nTempLen;
	else
	{
		strcpy(&pTempBuffer[nLen], "Unknown reason");
		nLen += strlen("Unknown reason");
	}
	pTempBuffer[nLen++] = '^';

	//diagnostics
	if (pBill->bit_mask & CDR3G_SGSNMMRecord_diagnostics_present)
	{
		nLen += DiagnosticsToStr(&pBill->diagnostics, &pTempBuffer[nLen], 
									nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//recordSequenceNumber
	if (pBill->bit_mask & CDR3G_SGSNMMRecord_recordSequenceNumber_present)
	{
		nLen += NumToDecStr(pBill->recordSequenceNumber, &pTempBuffer[nLen], 
							nLength - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//nodeID
	if (pBill->bit_mask & CDR3G_SGSNMMRecord_nodeID_present)
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

    /************ Deleted by XDB on 2002.1.7 *********************************/
	//recordExtensions
	if (pBill->bit_mask & CDR3G_SGSNMMRecord_recordExtensions_present)
	{
		nLen += RecordExtensionsToStr(&pBill->recordExtensions, &pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
    /************ End of deleted by XDB on 2002.1.7 **************************/

	//localSequenceNumber
	if (pBill->bit_mask & CDR3G_SGSNMMRecord_localSequenceNumber_present)
	{
		nLen += NumToDecStr(pBill->localSequenceNumber, &pTempBuffer[nLen], 
							nLength - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//servedMSISDN
	if (pBill->bit_mask & CDR3G_SGSNMMRecord_servedMSISDN_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedMSISDN, 
							&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//chargingCharacteristics
	if (pBill->bit_mask & CDR3G_SGSNMMRecord_chargingCharacteristics_present)
	{
		nLen += ChargingCharacteristicsToStr(&pBill->chargingCharacteristics, 
								&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//added by zouyongsheng  2002-08-16
	//cAMELInfomationMM
	if(pBill->bit_mask & CDR3G_cAMELInformationMM_present)
	{
		nLen += _3GSGSNMM_CAMELInformationToStr(&pBill->cAMELInformationMM, &pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
	//cAMELInfomationPDP end 
	
	//systemType
	if (pBill->bit_mask & CDR3G_SGSNMMRecord_systemType_present)
	{
		//Added By maimaoshi at 2002-05-31(D13999)
		EnumInfo.nCount = SYSTEMTYPEENUMINFO_LENGTH;
		EnumInfo.pInfoTable = SYSTEMTYPEENUMINFOEnumInfo;
		nLen += EnumToStr(&EnumInfo, pBill->systemType, &pTempBuffer[nLen], nLength - nLen);

		//nLen += SystemTypeToStr(&pBill->systemType, &pTempBuffer[nLen], MCDRBUFFERLENGTH - nLen);
		
		//End by maimaoshi
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	pTempBuffer[nLen] = 0;

	//Modifed By ZhengYuqun 2001/09/26
	//if (nLen < nLength)
	//	memcpy(pBuffer, pTempBuffer, nLen + 1);
	//else
	//	nLen = 0;
	if (nLen >= nLength)
		nLen = 0;

	//delete []pTempBuffer; Commented By ZhengYuqun 2001/09/26
	return nLen;
}


/******************************************************
// Function name: 	GetTextValueOf3GSGSNSMO
// Author:        Libo
// Date:          01-4-28
// Description:   处理S-SMO-CDR话单浏览解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-4-28       Libo
//02-9-18       ZouYongsheng     增加Camel域的显示
******************************************************/
int GetTextValueOf3GSGSNSMO(char *pValue, char *pBuffer, int nLength)
{
//#define SMOBUFFERLENGTH 65536   //Commented by ZhengYuqun 2001/09/26
#define SMOBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	CDR3G_SGSNSMORecord *pBill = (CDR3G_SGSNSMORecord*)pValue;
	int nLen = 0;
	int nTempLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != CDR3G_CallEventRecordType_sgsnSMORecord)
		return 0;

	//char *pTempBuffer = new char[SMOBUFFERLENGTH];
	//Modified By ZhengYuqun 2001/09/26
	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;

    //recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R99_360_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R99_360_Recorde_Type_EnumInfo;
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
							&pTempBuffer[nLen], nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//servedIMEI
	if (pBill->bit_mask & CDR3G_SGSNSMORecord_servedIMEI_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMEI,
							&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//servedMSISDN
	if (pBill->bit_mask & CDR3G_SGSNSMORecord_servedMSISDN_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedMSISDN, 
								&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//msNetworkCapability
	nLen += HexOctecToStr((LPOCTECSTRING)&pBill->msNetworkCapability, &pTempBuffer[nLen], 
						nLength - nLen, "0x", FALSE);							
	pTempBuffer[nLen++] = '^';

	//serviceCentre
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->serviceCentre, &pTempBuffer[nLen], 
							nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//recordingEntity
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->recordingEntity, &pTempBuffer[nLen],
							nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//locationArea
	if (pBill->bit_mask & CDR3G_SGSNSMORecord_locationArea_present)
	{
		//modified by Libo on Jul 6, 2001 for D03483
		//nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationArea, &pTempBuffer[nLen],
		//					SMOBUFFERLENGTH - nLen, "0x", TRUE);
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationArea, &pTempBuffer[nLen],
							nLength - nLen, "0x", FALSE);
		//modify end.
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//routingArea
	if (pBill->bit_mask & CDR3G_SGSNSMORecord_routingArea_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->routingArea, &pTempBuffer[nLen],
							nLength - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//cellIdentifier
	if (pBill->bit_mask & CDR3G_SGSNSMORecord_cellIdentifier_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->cellIdentifier, &pTempBuffer[nLen],
							nLength - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//messageReference
	nLen += NumToDecStr((long)pBill->messageReference.value[0], &pTempBuffer[nLen], 
							nLength - nLen, FALSE);
	pTempBuffer[nLen++] = '^';

	//originationTime
	nLen += TimeStampToStr((LPTIMESTAMP)&pBill->originationTime, &pTempBuffer[nLen],
							nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//smsResult
	if (pBill->bit_mask & CDR3G_SGSNSMORecord_smsResult_present)
	{
		nLen += DiagnosticsToStr(&pBill->smsResult, &pTempBuffer[nLen], 
									nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//recordExtensions
	if (pBill->bit_mask & CDR3G_SGSNSMORecord_recordExtensions_present)
	{
		nLen += RecordExtensionsToStr(&pBill->recordExtensions, &pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//nodeID
	if (pBill->bit_mask & CDR3G_SGSNSMORecord_nodeID_present)
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
	if (pBill->bit_mask & CDR3G_SGSNSMORecord_localSequenceNumber_present)
	{
		nLen += NumToDecStr(pBill->localSequenceNumber, &pTempBuffer[nLen], 
							nLength - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//chargingCharacteristics
	if (pBill->bit_mask & CDR3G_SGSNSMORecord_chargingCharacteristics_present)
	{
		nLen += ChargingCharacteristicsToStr(&pBill->chargingCharacteristics, 
								&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//systemType
	if (pBill->bit_mask & CDR3G_SGSNSMORecord_systemType_present)
	{
		//Added By maimaoshi at 2002-05-31(D13999)
		EnumInfo.nCount = SYSTEMTYPEENUMINFO_LENGTH;
		EnumInfo.pInfoTable = SYSTEMTYPEENUMINFOEnumInfo;
		nLen += EnumToStr(&EnumInfo, pBill->systemType, &pTempBuffer[nLen], nLength - nLen);

		//nLen += SystemTypeToStr(&pBill->systemType, &pTempBuffer[nLen], SMOBUFFERLENGTH - nLen);
		
		//End By maimaoshi
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//destinationNumber
	if (pBill->bit_mask & CDR3G_destinationNumber_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->destinationNumber, 
								&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//Added by zouyongsheng 2002.09.18
	//cAMELInformationSMS
	if(pBill->bit_mask & CDR3G_cAMELInformationSMS_present)
	{
		nLen += _3GSGSNSMO_CAMELInfomationToStr(&pBill->cAMELInformationSMS,
			                   	   &pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
	//Added end.

	pTempBuffer[nLen] = 0;

	//Modifed By ZhengYuqun 2001/09/26
	//if (nLen < nLength)
	//	memcpy(pBuffer, pTempBuffer, nLen + 1);
	//else
	//	nLen = 0;
	if (nLen >= nLength)
		nLen = 0;

	//delete []pTempBuffer; Commented By ZhengYuqun 2001/09/26
	return nLen;
}


/******************************************************
// Function name: 	GetTextValueOf3GSGSNSMT
// Author:        Libo
// Date:          01-4-28
// Description:   处理S-SMT-CDR话单浏览解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-4-28       Libo
******************************************************/
int GetTextValueOf3GSGSNSMT(char *pValue, char *pBuffer, int nLength)
{
//#define SMTBUFFERLENGTH 65536   //Commented by ZhengYuqun 2001/09/26
#define SMTBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	CDR3G_SGSNSMTRecord *pBill = (CDR3G_SGSNSMTRecord*)pValue;
	int nLen = 0;
	int nTempLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != CDR3G_CallEventRecordType_sgsnSMTRecord)
		return 0;

	//char *pTempBuffer = new char[SMTBUFFERLENGTH];
	//Modified By ZhengYuqun 2001/09/26
	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;

    //recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R99_360_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R99_360_Recorde_Type_EnumInfo;
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
							&pTempBuffer[nLen], nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//servedIMEI
	if (pBill->bit_mask & CDR3G_SGSNSMTRecord_servedIMEI_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMEI,
							&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//servedMSISDN
	if (pBill->bit_mask & CDR3G_SGSNSMTRecord_servedMSISDN_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedMSISDN, 
								&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//msNetworkCapability
	nLen += HexOctecToStr((LPOCTECSTRING)&pBill->msNetworkCapability, &pTempBuffer[nLen], 
						nLength - nLen, "0x", FALSE);							
	pTempBuffer[nLen++] = '^';

	//serviceCentre
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->serviceCentre, &pTempBuffer[nLen], 
							nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//recordingEntity
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->recordingEntity, &pTempBuffer[nLen],
							nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//locationArea
	if (pBill->bit_mask & CDR3G_SGSNSMTRecord_locationArea_present)
	{
		//modified by Libo on Jul 6, 2001 for D03483
		//nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationArea, &pTempBuffer[nLen],
		//					SMTBUFFERLENGTH - nLen, "0x", TRUE);
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationArea, &pTempBuffer[nLen],
							nLength - nLen, "0x", FALSE);
		//modify end.
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//routingArea
	if (pBill->bit_mask & CDR3G_SGSNSMTRecord_routingArea_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->routingArea, &pTempBuffer[nLen],
							nLength - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//cellIdentifier
	if (pBill->bit_mask & CDR3G_SGSNSMTRecord_cellIdentifier_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->cellIdentifier, &pTempBuffer[nLen],
							nLength - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//originationTime
	nLen += TimeStampToStr((LPTIMESTAMP)&pBill->originationTime, &pTempBuffer[nLen],
							nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//smsResult
	if (pBill->bit_mask & CDR3G_SGSNSMTRecord_smsResult_present)
	{
		nLen += DiagnosticsToStr(&pBill->smsResult, &pTempBuffer[nLen], 
									nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//recordExtensions
	if (pBill->bit_mask & CDR3G_SGSNSMTRecord_recordExtensions_present)
	{
		nLen += RecordExtensionsToStr(&pBill->recordExtensions, &pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//nodeID
	if (pBill->bit_mask & CDR3G_SGSNSMTRecord_nodeID_present)
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
	if (pBill->bit_mask & CDR3G_SGSNSMTRecord_localSequenceNumber_present)
	{
		nLen += NumToDecStr(pBill->localSequenceNumber, &pTempBuffer[nLen], 
							nLength - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//chargingCharacteristics
	if (pBill->bit_mask & CDR3G_SGSNSMTRecord_chargingCharacteristics_present)
	{
		nLen += ChargingCharacteristicsToStr(&pBill->chargingCharacteristics, 
								&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//systemType
	if (pBill->bit_mask & CDR3G_SGSNSMTRecord_systemType_present)
	{
		//Added By maimaoshi at 2002-05-31(D13999)
		EnumInfo.nCount = SYSTEMTYPEENUMINFO_LENGTH;
		EnumInfo.pInfoTable = SYSTEMTYPEENUMINFOEnumInfo;
		nLen += EnumToStr(&EnumInfo, pBill->systemType, &pTempBuffer[nLen], nLength - nLen);

		//nLen += SystemTypeToStr(&pBill->systemType, &pTempBuffer[nLen], SMTBUFFERLENGTH - nLen);
		
		//End By maimaoshi
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	pTempBuffer[nLen] = 0;

	//Modifed By ZhengYuqun 2001/09/26
	//if (nLen < nLength)
	//	memcpy(pBuffer, pTempBuffer, nLen + 1);
	//else
	//	nLen = 0;
	if (nLen >= nLength)
		nLen = 0;

	//delete []pTempBuffer; Commented By ZhengYuqun 2001/09/26
	return nLen;
}


/******************************************************
// Function name: 	GetTextValueOf3GLARGESGSNPDP
// Author:        Libo
// Date:          01-5-23
// Description:   处理合并后S-CDR话单浏览解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-5-23       Libo
******************************************************/
int GetTextValueOf3GLARGESGSNPDP(char *pValue, char *pBuffer, int nLength)
{
	_3GCDR_SGSNPDPRecord *pBill = (_3GCDR_SGSNPDPRecord*)pValue;
	int nLen = 0;
    int nTempLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != _3GCDR_CallEventRecordType_sgsnPDPRecord)
		return 0;

	//char *pTempBuffer = new char[SCDRBUFFERLENGTH];
	//Modified By ZhengYuqun 2001/09/26
	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;
	
    //recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R99_360_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R99_360_Recorde_Type_EnumInfo;
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
	if (pBill->bit_mask & _3GCDR_SGSNPDPRecord_networkInitiation_present)
	{
		nLen += BoolToStr(pBill->networkInitiation, &pTempBuffer[nLen], 
			nLength - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//servedIMSI
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI, 
							&pTempBuffer[nLen], nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//servedIMEI
	if (pBill->bit_mask & _3GCDR_SGSNPDPRecord_servedIMEI_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMEI,
							&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//sgsnAddress
	nLen += _3GIPAddrToStr((CDR3G_IPAddress*)&pBill->sgsnAddress, &pTempBuffer[nLen], nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//msNetworkCapability
	if (pBill->bit_mask & _3GCDR_SGSNPDPRecord_msNetworkCapability_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->msNetworkCapability, &pTempBuffer[nLen], 
							nLength - nLen, "0x", FALSE);							
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
	
	//routingArea
	if (pBill->bit_mask & _3GCDR_SGSNPDPRecord_routingArea_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->routingArea, &pTempBuffer[nLen],
							nLength - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//locationAreaCode
	if (pBill->bit_mask & _3GCDR_SGSNPDPRecord_locationAreaCode_present)
	{
		//modified by Libo on Jul 5, 2001 for D03483
		//nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationAreaCode, &pTempBuffer[nLen],
		//					SCDRBUFFERLENGTH - nLen, "0x", TRUE);
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationAreaCode, &pTempBuffer[nLen],
							nLength - nLen, "0x", FALSE);
		//modify end.
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//cellIdentifier
	if (pBill->bit_mask & _3GCDR_SGSNPDPRecord_cellIdentifier_present)
	{
		nLen += HexOctecToStr((LPOCTECSTRING)&pBill->cellIdentifier, &pTempBuffer[nLen],
							nLength - nLen, "0x", FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//chargingID
	nLen += NumToDecStr(pBill->chargingID, &pTempBuffer[nLen], 
						nLength - nLen, FALSE);
	pTempBuffer[nLen++] = '^';

	//ggsnAddressUsed
	nLen += _3GIPAddrToStr((CDR3G_IPAddress*)&pBill->ggsnAddressUsed, &pTempBuffer[nLen], nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//accessPointNameNI
	strcpy(&pTempBuffer[nLen], pBill->accessPointNameNI);
	nLen += strlen(pBill->accessPointNameNI);
	pTempBuffer[nLen++] = '^';

	//pdpType
	//modified by Libo on Jul 5, 2001 for D03483
	//nLen += HexOctecToStr((LPOCTECSTRING)&pBill->pdpType, &pTempBuffer[nLen],
	//					SCDRBUFFERLENGTH - nLen, "0x", TRUE);
	nLen += HexOctecToStr((LPOCTECSTRING)&pBill->pdpType, &pTempBuffer[nLen],
						nLength - nLen, "0x", FALSE);
	//modify end.
	pTempBuffer[nLen++] = '^';

	//Added by maimaoshi at 2002-05-31(D13999)
	//servedPDPAddress
	if (pBill->bit_mask & _3GCDR_SGSNPDPRecord_servedPDPAddress_present)
	{
		nLen += _3GPDPAddrToStr((CDR3G_PDPAddress*)&pBill->servedPDPAddress, &pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
	//End by maimaoshi

    /************ Deleted by XDB on 2002.1.7 *********************************/
	//listOfTrafficVolumes
	//ListOfTrafficVolumesToStr增加一个参数 Modified by ZhengYuqun 2001/09/28
	_3GCDR_NetworkInitiatedPDPContext networkInitiation = pBill->bit_mask & _3GCDR_SGSNPDPRecord_networkInitiation_present ? pBill->networkInitiation : NULL;
	nLen += ListOfTrafficVolumesToStr((CDR3G__seqof1*)&pBill->listOfTrafficVolumes, &pTempBuffer[nLen], 
										nLength - nLen, networkInitiation);
	pTempBuffer[nLen++] = '^';
    /************ End of deleted by XDB on 2002.1.7 **************************/

	//recordOpeningTime
	nLen += TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime, &pTempBuffer[nLen],
							nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//duration
	nLen += NumToDecStr(pBill->duration, &pTempBuffer[nLen], nLength - nLen, FALSE);
	pTempBuffer[nLen++] = '^';

	//sgsnChange
	if (pBill->bit_mask & _3GCDR_SGSNPDPRecord_sgsnChange_present)
	{
		nLen += BoolToStr(pBill->sgsnChange, &pTempBuffer[nLen], nLength - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//causeForRecClosing
	EnumInfo.nCount = CAUSEFORRECCLOSINGENUNINFO_LENGTH;
	EnumInfo.pInfoTable = CauseForRecClosingEnumInfo;
	nTempLen = EnumToStr(&EnumInfo, pBill->causeForRecClosing,&pTempBuffer[nLen], nLength - nLen);

    /*  //Modified by XDB on 2001.11.09 */
    if (nTempLen)
		nLen += nTempLen;
	else
	{
		strcpy(&pTempBuffer[nLen], "Unknown reason)");
		nLen += strlen("Unknown reason");
	}
	pTempBuffer[nLen++] = '^';

	//diagnostics
	if (pBill->bit_mask & _3GCDR_SGSNPDPRecord_diagnostics_present)
	{
		nLen += DiagnosticsToStr((CDR3G_Diagnostics*)&pBill->diagnostics, &pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

        //maimaoshi,2002-08-21,Added "{}"
	//recSequenceNumList
	if (pBill->bit_mask & _3GCDR_SGSNPDPRecord_recSequenceNumList_present)
	{
		pTempBuffer[nLen++] = '{';
		strcpy(&pTempBuffer[nLen], "sgsnAddress:");
		nLen += strlen("sgsnAddress:");
		nLen += _3GIPAddrToStr((CDR3G_IPAddress*)&pBill->recSequenceNumList.sgsnAddress, &pTempBuffer[nLen], 
								nLength - nLen);
		if (pBill->recSequenceNumList.bit_mask & _3GCDR_recSequenceNumber_present)
		{
			strcpy(&pTempBuffer[nLen], " recSequenceNumber:");
			nLen += strlen(" recSequenceNumber:");
			for (UINT4 i = 0; i < pBill->recSequenceNumList.recSequenceNumber.count; i++)
			{
				nLen += NumToDecStr(pBill->recSequenceNumList.recSequenceNumber.value[i],
									&pTempBuffer[nLen], nLength - nLen, FALSE);
				pTempBuffer[nLen++] = ' ';
			}
		}
		pTempBuffer[nLen++] = '}';		
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//nodeID
	if (pBill->bit_mask & _3GCDR_SGSNPDPRecord_nodeID_present)
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

    /************ Deleted by XDB on 2002.1.7 **********************************/
	//recordExtensions
	if (pBill->bit_mask & _3GCDR_SGSNPDPRecord_recordExtensions_present)
	{
		nLen += RecordExtensionsToStr((CDR3G_ManagementExtensions*)&pBill->recordExtensions, &pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
    /************ End of deleted by XDB on 2002.1.7 **************************/

	//apnSelectionMode
	if (pBill->bit_mask & _3GCDR_SGSNPDPRecord_apnSelectionMode_present)
	{
		EnumInfo.nCount = APNSELECTIONMODEENUMINFO_LENGTH;
		EnumInfo.pInfoTable = ApnSelectionModeEnumInfo;
		nLen += EnumToStr(&EnumInfo, pBill->apnSelectionMode, &pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//accessPointNameOI
	strcpy(&pTempBuffer[nLen], pBill->accessPointNameOI);
	nLen += strlen(pBill->accessPointNameOI);
	pTempBuffer[nLen++] = '^';

	//servedMSISDN
	if (pBill->bit_mask & _3GCDR_SGSNPDPRecord_servedMSISDN_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedMSISDN, &pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//chargingCharacteristics
	if (pBill->bit_mask & _3GCDR_SGSNPDPRecord_chargingCharacteristics_present)
	{
		nLen += ChargingCharacteristicsToStr((CDR3G_ChargingCharacteristics*)&pBill->chargingCharacteristics, &pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//systemType
	if (pBill->bit_mask & _3GCDR_SGSNPDPRecord_systemType_present)
	{
		//Added By maimaoshi at 2002-05-31(D13999)
		EnumInfo.nCount = SYSTEMTYPEENUMINFO_LENGTH;
		EnumInfo.pInfoTable = SYSTEMTYPEENUMINFOEnumInfo;
		nLen += EnumToStr(&EnumInfo, pBill->systemType, &pTempBuffer[nLen], nLength - nLen);

		//nLen += SystemTypeToStr((CDR3G_SystemType*)&pBill->systemType, &pTempBuffer[nLen], SCDRBUFFERLENGTH - nLen);
		
		//End By maimaoshi
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//added by zouyongsheng  2002-08-09
	//cAMELInfomationPDP
	if(pBill->bit_mask & _3GCDR_cAMELInformationPDP_present)
	{
		nLen += _3GLARGESGSNPDP_CAMELInformationToStr(&pBill->cAMELInformationPDP, &pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
	//cAMELInfomationPDP end 


	//rNCUnsentDownlinkVolume
	if (pBill->bit_mask & _3GCDR_rNCUnsentDownlinkVolume_present)
	{
		nLen += NumToDecStr(pBill->rNCUnsentDownlinkVolume, &pTempBuffer[nLen], nLength - nLen, FALSE);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//consolidationResult
	EnumInfo.nCount = CONSOLIDATIONRESULTENUMINFO_LENGTH;
	EnumInfo.pInfoTable = ConsolidationResultEnumInfo;
	nLen += EnumToStr(&EnumInfo, pBill->consolidationResult, 
					&pTempBuffer[nLen], nLength - nLen);
	pTempBuffer[nLen++] = '^';

	pTempBuffer[nLen] = 0;

	//Modifed By ZhengYuqun 2001/09/26
	//if (nLen < nLength)
	//	memcpy(pBuffer, pTempBuffer, nLen + 1);
	//else
	//	nLen = 0;
	if (nLen >= nLength)
		nLen = 0;

	//delete []pTempBuffer; Commented By ZhengYuqun 2001/09/26
	return nLen;
}


/******************************************************
// Function name: 	GetTextValueOf3GLARGEGGSNPDP
// Author:        Libo
// Date:          01-5-23
// Description:   处理合并后G-CDR话单浏览解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-5-23       Libo
******************************************************/
int GetTextValueOf3GLARGEGGSNPDP(char *pValue, char *pBuffer, int nLength)
{
	_3GCDR_GGSNPDPRecord *pBill = (_3GCDR_GGSNPDPRecord*)pValue;
	int nLen = 0;
    int nTempLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != _3GCDR_CallEventRecordType_ggsnPDPRecord)
		return 0;

	//char *pTempBuffer = new char[GCDRBUFFERLENGTH];
	//Modified By ZhengYuqun 2001/09/26
	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;
	
    //recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R99_360_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R99_360_Recorde_Type_EnumInfo;
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
	if (pBill->bit_mask & _3GCDR_GGSNPDPRecord_networkInitiation_present)
	{
		nLen += BoolToStr(pBill->networkInitiation, &pTempBuffer[nLen], 
							nLength - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//servedIMSI
	nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI, 
							&pTempBuffer[nLen], nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//ggsnAddress
	nLen += _3GIPAddrToStr((CDR3G_IPAddress*)&pBill->ggsnAddress, &pTempBuffer[nLen], nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//chargingID
	nLen += NumToDecStr(pBill->chargingID, &pTempBuffer[nLen], 
						nLength - nLen, FALSE);
	pTempBuffer[nLen++] = '^';

    unsigned short i = 0;  //Moved by XDB from inner "for" to outer "for"
	//sgsnAddress
	for (i = 0; i < pBill->sgsnAddress.count; i++)
	{
		pTempBuffer[nLen++] = '{';
		nLen += _3GIPAddrToStr((CDR3G_IPAddress*)&pBill->sgsnAddress.value[i], &pTempBuffer[nLen], 
								nLength - nLen);
		pTempBuffer[nLen++] = '}';
	}
	pTempBuffer[nLen++] = '^';

	//accessPointNameNI
	strcpy(&pTempBuffer[nLen], pBill->accessPointNameNI);
	nLen += strlen(pBill->accessPointNameNI);
	pTempBuffer[nLen++] = '^';

	//pdpType
	//modified by Libo on Jul 5, 2001 for D03483
	//nLen += HexOctecToStr((LPOCTECSTRING)&pBill->pdpType, &pTempBuffer[nLen],
	//					GCDRBUFFERLENGTH - nLen, "0x", TRUE);
	nLen += HexOctecToStr((LPOCTECSTRING)&pBill->pdpType, &pTempBuffer[nLen],
						nLength - nLen, "0x", FALSE);
	//modify end.
	pTempBuffer[nLen++] = '^';

	//Added by maimaoshi at 2002-05-31(D13999)
	//servedPDPAddress
	if (pBill->bit_mask & _3GCDR_GGSNPDPRecord_servedPDPAddress_present)
	{
		nLen += _3GPDPAddrToStr((CDR3G_PDPAddress*)&pBill->servedPDPAddress, &pTempBuffer[nLen], 
							nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
	//End by maimaoshi

	//dynamicAddressFlag
	if (pBill->bit_mask & _3GCDR_dynamicAddressFlag_present)
	{
		nLen += BoolToStr(pBill->dynamicAddressFlag, &pTempBuffer[nLen], 
							nLength - nLen, 0);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

    /************ Deleted by XDB on 2002.1.7 *********************************/
	//listOfTrafficVolumes
	//ListOfTrafficVolumesToStr增加一个参数 Modified by ZhengYuqun 2001/09/28
	_3GCDR_NetworkInitiatedPDPContext networkInitiation = pBill->bit_mask & _3GCDR_GGSNPDPRecord_networkInitiation_present ? pBill->networkInitiation : NULL;
	nLen += ListOfTrafficVolumesToStr((CDR3G__seqof1*)&pBill->listOfTrafficVolumes, &pTempBuffer[nLen], 
										nLength - nLen, networkInitiation);
	pTempBuffer[nLen++] = '^';
    /************ End of deleted by XDB on 2002.1.7 **************************/

	//recordOpeningTime
	nLen += TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime, &pTempBuffer[nLen],
							nLength - nLen);
	pTempBuffer[nLen++] = '^';

	//duration
	nLen += NumToDecStr(pBill->duration, &pTempBuffer[nLen], 
						nLength - nLen, FALSE);
	pTempBuffer[nLen++] = '^';

	//causeForRecClosing
	EnumInfo.nCount = CAUSEFORRECCLOSINGENUNINFO_LENGTH;
	EnumInfo.pInfoTable = CauseForRecClosingEnumInfo;
	nTempLen = EnumToStr(&EnumInfo, pBill->causeForRecClosing,&pTempBuffer[nLen], 
						nLength - nLen);
    /*  //Modified by XDB on 2001.11.09 */
	if (nTempLen)
		nLen += nTempLen;
	else
	{
		strcpy(&pTempBuffer[nLen], "Unknown reason");
		nLen += strlen("Unknown reason");
	}
	pTempBuffer[nLen++] = '^';

	//diagnostics
	if (pBill->bit_mask & _3GCDR_GGSNPDPRecord_diagnostics_present)
	{
		nLen += DiagnosticsToStr((CDR3G_Diagnostics*)&pBill->diagnostics, &pTempBuffer[nLen], 
									nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';


        //maimaoshi,2002-08-21,Added "{}"
	//recSequenceNumList
	if (pBill->bit_mask & _3GCDR_GGSNPDPRecord_recSequenceNumList_present)
	{
		pTempBuffer[nLen++] = '{';
		strcpy(&pTempBuffer[nLen], "sgsnAddress:");
		nLen += strlen("sgsnAddress:");
		nLen += _3GIPAddrToStr((CDR3G_IPAddress*)&pBill->recSequenceNumList.sgsnAddress, &pTempBuffer[nLen], 
								nLength - nLen);		
		if (pBill->recSequenceNumList.bit_mask & _3GCDR_recSequenceNumber_present)
		{
			strcpy(&pTempBuffer[nLen], " recSequenceNumber:");
			nLen += strlen(" recSequenceNumber:");
			for (i = 0; i < pBill->recSequenceNumList.recSequenceNumber.count; i++)
			{
				nLen += NumToDecStr(pBill->recSequenceNumList.recSequenceNumber.value[i],
									&pTempBuffer[nLen], nLength - nLen, FALSE);
				pTempBuffer[nLen++] = ' ';
			}
		}
		pTempBuffer[nLen++] = '}';		
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//nodeID
	if (pBill->bit_mask & _3GCDR_GGSNPDPRecord_nodeID_present)
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

    /************ Deleted by XDB on 2002.1.7 *********************************/
	//recordExtensions
	if (pBill->bit_mask & _3GCDR_GGSNPDPRecord_recordExtensions_present)
	{
        switch (g_nCBB_OI)
        {			    
        case CBB_OPERATOR_ID_CMCC:
            nLen += CMCC_R99_GCDRRecordExtensionsToStr((CDR3G_ManagementExtensions*)&pBill->recordExtensions, 
                         &pTempBuffer[nLen], nLength - nLen);
            break;
            
        default:
            nLen += R99_GCDRRecordExtensionsToStr((CDR3G_ManagementExtensions*)&pBill->recordExtensions, 
                         &pTempBuffer[nLen], nLength - nLen);
            break;			    
        }		
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';
    /************ End of deleted by XDB on 2002.1.7 **************************/

	//apnSelectionMode
	if (pBill->bit_mask & _3GCDR_GGSNPDPRecord_apnSelectionMode_present)
	{
		EnumInfo.nCount = APNSELECTIONMODEENUMINFO_LENGTH;
		EnumInfo.pInfoTable = ApnSelectionModeEnumInfo;
		nLen += EnumToStr(&EnumInfo, pBill->apnSelectionMode, &pTempBuffer[nLen], 
							nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//servedMSISDN
	if (pBill->bit_mask & _3GCDR_GGSNPDPRecord_servedMSISDN_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedMSISDN, &pTempBuffer[nLen], 
									nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//chargingCharacteristics
	if (pBill->bit_mask & _3GCDR_GGSNPDPRecord_chargingCharacteristics_present)
	{
		nLen += ChargingCharacteristicsToStr((CDR3G_ChargingCharacteristics*)&pBill->chargingCharacteristics, 
											&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

	//consolidationResult
	EnumInfo.nCount = CONSOLIDATIONRESULTENUMINFO_LENGTH;
	EnumInfo.pInfoTable = ConsolidationResultEnumInfo;
	nLen += EnumToStr(&EnumInfo, pBill->consolidationResult, 
					&pTempBuffer[nLen], nLength - nLen);
	pTempBuffer[nLen++] = '^';

	pTempBuffer[nLen] = 0;

	//Modifed By ZhengYuqun 2001/09/26
	//if (nLen < nLength)
	//	memcpy(pBuffer, pTempBuffer, nLen + 1);
	//else
	//	nLen = 0;
	if (nLen >= nLength)
		nLen = 0;

	//delete []pTempBuffer; Commented By ZhengYuqun 2001/09/26
	return nLen;
}


/******************************************************
// Function name: 	GetTextValueOf3GLARGESGSNMM
// Author:        Libo
// Date:          01-5-23
// Description:   处理合并后M-CDR话单浏览解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-5-23       Libo
******************************************************/
int GetTextValueOf3GLARGESGSNMM(char *pValue, char *pBuffer, int nLength)
{	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////

/******************************************************
// Function name: 	_3GIPAddrToStr
// Author:        Libo
// Date:          01-4-28
// Description:   将CDR3G_IPAddress结构的数据转换成字符串
// Input:
//       arg1 :CDR3G_IPAddress *pIPAddr	待转换数据
//       arg2 :char *pBuffer			存放返回字符串的数据区
//       arg3 :int nLength				数据区可用长度
// Return value: int					转换后的字符串长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-4-28       Libo
******************************************************/
int _3GIPAddrToStr(CDR3G_IPAddress *pIPAddr, char *pBuffer, int nLength)
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
	case CDR3G_iPBinaryAddress_chosen:	//Bin类型
		{
			switch (pIPAddr->u.iPBinaryAddress.choice)	//判断是IPV4还是IPV6
			{
			case CDR3G_iPBinV4Address_chosen:			//IPV4
				{
					nLen = OctecToIPAddrStr((LPOCTECSTRING)&pIPAddr->u.iPBinaryAddress.u.iPBinV4Address, 
											pBuffer, nLength, BINIPV4_STYLE);
				}
				break;
			case CDR3G_iPBinV6Address_chosen:			//IPV6
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
	case CDR3G_iPTextRepresentedAddress_chosen:	//Text类型
		{
			switch (pIPAddr->u.iPTextRepresentedAddress.choice)	//判断是IPV4还是IPV6
			{
			case CDR3G_iPTextV4Address_chosen:			//IPV4
				{
					if (nLength < 16)
						return 0;
					memcpy(pBuffer, pIPAddr->u.iPTextRepresentedAddress.u.iPTextV4Address, 15);
					pBuffer[15] = 0;
					nLen = strlen(pBuffer);
				}
				break;
			case CDR3G_iPTextV6Address_chosen:			//IPV6
				{
					if (nLength < 40)
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
// Function name: 	_3GPDPAddrToStr
// Author:        Libo
// Date:          01-4-28
// Description:   将CDR3G_PDPAddress结构的数据转换成字符串
// Input:
//       arg1 :CDR3G_PDPAddress *pPDPAddress	待转换的数据	
//       arg2 :char *pBuffer					存放返回字符串的数据区
//       arg3 :int nLength						数据区可用长度
// Return value: int							转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-4-28       Libo
******************************************************/
int _3GPDPAddrToStr(CDR3G_PDPAddress *pPDPAddress, char *pBuffer, int nLength)
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
	case CDR3G_iPAddress_chosen:
		{
			nLen = _3GIPAddrToStr(&pPDPAddress->u.iPAddress, pBuffer, nLength);
		}
		break;
	case CDR3G_eTSIAddress_chosen:
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
// Function name: 	ListOfTrafficVolumesToStr
// Author:        Libo
// Date:          01-5-21
// Description:   转换ListOfTrafficVolumes类型的数据为字符串
// Input:
//       arg1 :CDR3G__seqof1 *pList	待转换的数据
//       arg2 :char *pBuffer		转换后数据存放的Buffer
//       arg3 :int nLength			Buffer的可用长度
//       arg4 :_3GCDR_NetworkInitiatedPDPContext networkInitiation 代表是MS to netWork还是netWork to MS，Modified By ZhengYuqun 2001/09/28
// Return value: int				转换后字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-5-21       Libo
******************************************************/
int ListOfTrafficVolumesToStr(CDR3G__seqof1 *pList, char *pBuffer, int nLength, char networkInitiation)
{
//#define LISTOFTRAFFICVOLUMESBUFFERLENGTH 65536   //Commented by ZhengYuqun 2001/09/26
#define LISTOFTRAFFICVOLUMESBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	int nLen = 0;
	int nCount = pList->count;
	CDR3G_ChangeOfCharCondition *pItem;

	ENUMINFO EnumInfo;
	EnumInfo.nCount = CHANGECONDITIONENUMINFO_LENGTH; //3
	EnumInfo.pInfoTable = ChangeConditionEnumInfo;
	
	//ASSERT((BOOL)(pBuffer && pList));
	if ((pBuffer == NULL) || (pList == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;	

	//char *pTempBuffer = new char[LISTOFTRAFFICVOLUMESBUFFERLENGTH];
	//Modified By ZhengYuqun 2001/09/26
	//char *pTempBuffer = g_p3GTempBuffer;
	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;
	pTempBuffer[nLen++] = '{';
	for (int i = 0; i < nCount; i++)
	{
		if (i==939) 
			i=939;
		pItem = &pList->value[i];

		pTempBuffer[nLen++] = '{';

		if (pItem->bit_mask & CDR3G_qosRequested_present)
		{
			strcpy(&pTempBuffer[nLen], "QoSReq:");
			nLen += strlen("QoSReq:");
			//增加一个参数  ZhengYuqun 2001/09/28
			nLen += _3GQosInformationToStr(&pItem->qosRequested, &pTempBuffer[nLen], 
											nLength - nLen, networkInitiation);
			pTempBuffer[nLen++] = ' ';
		}	

		if (pItem->bit_mask & CDR3G_qosNegotiated_present)
		{
			strcpy(&pTempBuffer[nLen], "QoSNeg:");
			nLen += strlen("QoSNeg:");
			//增加一个参数  ZhengYuqun 2001/09/28
			nLen += _3GQosInformationToStr(&pItem->qosNegotiated, &pTempBuffer[nLen], 
											nLength - nLen, networkInitiation);
			pTempBuffer[nLen++] = ' ';
		}
		
		strcpy(&pTempBuffer[nLen], "VolUpLk:");
		nLen += strlen("VolUpLk:");
		nLen += NumToHexStr(pItem->dataVolumeGPRSUplink, &pTempBuffer[nLen], 
							nLength - nLen, "0x");
		pTempBuffer[nLen++] = ' ';

		strcpy(&pTempBuffer[nLen], "VolDnLk:");
		nLen += strlen("VolDnLk:");
		nLen += NumToHexStr(pItem->dataVolumeGPRSDownlink, &pTempBuffer[nLen], 
							nLength - nLen, "0x");
		pTempBuffer[nLen++] = ' ';

		strcpy(&pTempBuffer[nLen], "ChangeCon:");
		nLen += strlen("ChangeCon:");
		nLen += EnumToStr(&EnumInfo, pItem->changeCondition, &pTempBuffer[nLen], 
							nLength - nLen);
		pTempBuffer[nLen++] = ' ';

		strcpy(&pTempBuffer[nLen], "ChangeTime:");
		nLen += strlen("ChangeTime:");
		nLen += TimeStampToStr((LPTIMESTAMP)&pItem->changeTime, &pTempBuffer[nLen], 
								nLength - nLen);
		pTempBuffer[nLen++] = '}';
		
	}
	pTempBuffer[nLen++] = '}';
	
	pTempBuffer[nLen] = 0;

	//if (nLength > nLen)
	//	memcpy(pBuffer, pTempBuffer, nLen + 1);
	//else
	//	nLen = 0;
	if(nLen >= nLength)
	{
		nLen = 0;
	}

	//delete []pTempBuffer; Commented By ZhengYuqun 2001/09/26
	return nLen;
}


/******************************************************
// Function name: 	_3GQosInformationToStr
// Author:        Libo
// Date:          01-5-21
// Description:   转换QosInformation类型的数据为字符串
// Input:
//       arg1 :CDR3G_QoSInformation *pQosInfo	待转换的数据
//       arg2 :char *pBuffer		转换后数据存放的Buffer
//       arg3 :int nLength			Buffer的可用长度
//       arg4 :_3GCDR_NetworkInitiatedPDPContext networkInitiation 代表是MS to netWork还是netWork to MS，Modified By ZhengYuqun 2001/09/28
// Return value: int				转换后字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-5-21       Libo
******************************************************/
int _3GQosInformationToStr(CDR3G_QoSInformation *pQosInfo, char *pBuffer, int nLength, char networkInitiation)
{
//#define QOSBUFFERLENGTH 65536   //Commented by ZhengYuqun 2001/09/26
#define QOSBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	int nLen = 0;

	//ASSERT((BOOL)(pBuffer && pQosInfo));
	if ((pBuffer == NULL) || (pQosInfo == NULL))
	{
	     return 0;
	}
	
	if (pQosInfo->choice == CDR3G_gsmQosInformation_chosen)
	{
		//char *pTempBuffer = new char[QOSBUFFERLENGTH];
		//Modified By ZhengYuqun 2001/09/26
		char *pTempBuffer = pBuffer;//g_p3GTempBuffer;


		ENUMINFO EnumInfo;

		if (!pTempBuffer)
			return 0;
		pTempBuffer[0] = 0;

		strcpy(pTempBuffer, "ReliabClass(");
		nLen = strlen("ReliabClass(");	
		EnumInfo.nCount = QOSRELIABILITYENUMINFO_LENGTH; //6
		EnumInfo.pInfoTable = QosReliabilityEnumInfo;
		nLen += EnumToStr(&EnumInfo, pQosInfo->u.gsmQosInformation.reliability, &pTempBuffer[nLen], (nLength - nLen));
		
		strcpy(&pTempBuffer[nLen], ")DelayClass(");
		nLen += strlen(")DelayClass(");
		EnumInfo.nCount = QOSDELAYENUMINFO_LENGTH; //4
		EnumInfo.pInfoTable = QoSDelayEnumInfo;
		nLen += EnumToStr(&EnumInfo, pQosInfo->u.gsmQosInformation.delay, &pTempBuffer[nLen], (nLength - nLen));
		
		strcpy(&pTempBuffer[nLen], ")PrecedClass(");
		nLen += strlen(")PrecedClass(");
		EnumInfo.nCount = QOSPRECEDENCEENUMINFO_LENGTH; //10
		EnumInfo.pInfoTable = QoSPrecedenceEnumInfo;
		nLen += EnumToStr(&EnumInfo, pQosInfo->u.gsmQosInformation.precedence, &pTempBuffer[nLen], (nLength - nLen));

		strcpy(&pTempBuffer[nLen], ")PeakThrput(");
		nLen += strlen(")PeakThrput(");
		EnumInfo.nCount = QOSPEAKTHROUGHPUTENUMINFO_LENGTH;
		EnumInfo.pInfoTable = QoSPeakThroughputEnumInfo;
		nLen += EnumToStr(&EnumInfo, pQosInfo->u.gsmQosInformation.peakThroughput, &pTempBuffer[nLen], (nLength - nLen));
		
		strcpy(&pTempBuffer[nLen], ")MeanThrput(");
		nLen += strlen(")MeanThrput(");
		EnumInfo.nCount = QOSMEANTHROUGHPUTENUMINFO_LENGTH;
		EnumInfo.pInfoTable = QoSMeanThroughputEnumInfo;
		nLen += EnumToStr(&EnumInfo, pQosInfo->u.gsmQosInformation.meanThroughput, &pTempBuffer[nLen], (nLength - nLen));
		pTempBuffer[nLen++] = ')';
		
		pTempBuffer[nLen] = 0;
		//if (nLen < nLength)
		//	memcpy(pBuffer, pTempBuffer, nLen + 1);
		//else
		if (nLen >= nLength)
			nLen = 0;
		//delete []pTempBuffer; Commented By ZhengYuqun 2001/09/26
	}
	else if (pQosInfo->choice == CDR3G_umtsQosInformation_chosen)
	{
		//nLen = OctecToStr((LPOCTECSTRING)&pQosInfo->u.umtsQosInformation, pBuffer, nLength);
		char *pTempBuffer = pBuffer;

		if (networkInitiation == NETWORKTOMSDIRECTION || networkInitiation == MSTONETWORKDIRECTION)
		{
			ENUMINFO EnumInfo;

			if (!pTempBuffer)
				return 0;
			pTempBuffer[0] = 0;
			
			//跳过一个字节 Modified by ZhengYuqun 2003-09-10  SWPD01505
            //S_QOS99* umtsQosInfo = (S_QOS99*)&pQosInfo->u.umtsQosInformation.value;
            S_QOS99* umtsQosInfo = (S_QOS99*)&pQosInfo->u.umtsQosInformation.value[1];
			
			strcpy(pTempBuffer, "ReliabClass(");
			nLen = strlen(pTempBuffer);
			EnumInfo.nCount = QOS99RELIABILITYENUMINFO_LENGTH; 
			EnumInfo.pInfoTable = Qos99ReliabilityEnumInfo;
			nLen += EnumToStrForQoS99(&EnumInfo, umtsQosInfo->Qos98.ReliabClass, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);
			
			strcpy(&pTempBuffer[nLen], ")DelayClass(");
			nLen += strlen(&pTempBuffer[nLen]);
			EnumInfo.nCount = QOS99DELAYENUMINFO_LENGTH; 
			EnumInfo.pInfoTable = QoS99DelayEnumInfo;
			nLen += EnumToStrForQoS99(&EnumInfo, umtsQosInfo->Qos98.DelayClass, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);
	
			strcpy(&pTempBuffer[nLen], ")PrecedClass(");
			nLen += strlen(&pTempBuffer[nLen]);
			EnumInfo.nCount = QOS99RECEDENCEENUMINFO_LENGTH; 
			EnumInfo.pInfoTable = QoS99PrecedenceEnumInfo;
			nLen += EnumToStrForQoS99(&EnumInfo, umtsQosInfo->Qos98.PrecedClass, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

			strcpy(&pTempBuffer[nLen], ")PeakThrput(");
			nLen += strlen(&pTempBuffer[nLen]);
			EnumInfo.nCount = QOS99PEAKTHROUGHPUTENUMINFO_LENGTH; 
			EnumInfo.pInfoTable = QoS99PeakThroughputEnumInfo;
			nLen += EnumToStrForQoS99(&EnumInfo, umtsQosInfo->Qos98.PeakThrougout, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

			strcpy(&pTempBuffer[nLen], ")MeanThrput(");
			nLen += strlen(&pTempBuffer[nLen]);
			EnumInfo.nCount = QOS99MEANTHROUGHPUTENUMINFO_LENGTH; 
			EnumInfo.pInfoTable = QoS99MeanThroughputEnumInfo;
			nLen += EnumToStrForQoS99(&EnumInfo, umtsQosInfo->Qos98.MeanThrougout, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

			strcpy(&pTempBuffer[nLen], ")DelivErrSDUs(");
			nLen += strlen(&pTempBuffer[nLen]);
			EnumInfo.nCount = QOS99DELIVERYOFERRONEOUSSDUSENUMINFO_LENGTH; 
			EnumInfo.pInfoTable = QoS99DeliveryOfErroneousSDUsEnumInfo;
			nLen += EnumToStrForQoS99(&EnumInfo, umtsQosInfo->QosExt.DeliveryOfErroneousSDU, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

			strcpy(&pTempBuffer[nLen], ")DelivOrder(");
			nLen += strlen(&pTempBuffer[nLen]);
			EnumInfo.nCount = QOS99DELIVERYORDERENUMINFO_LENGTH; 
			EnumInfo.pInfoTable = QoS99DeliveryOrderEnumInfo;
			nLen += EnumToStrForQoS99(&EnumInfo, umtsQosInfo->QosExt.DeliveryOrder, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);
			
			strcpy(&pTempBuffer[nLen], ")TraffClass(");
			nLen += strlen(&pTempBuffer[nLen]);
			EnumInfo.nCount = QOS99TRAFFICCLASSENUMINFO_LENGTH; 
			EnumInfo.pInfoTable = QoS99TrafficClassEnumInfo;
			nLen += EnumToStrForQoS99(&EnumInfo, umtsQosInfo->QosExt.TrafficClass, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

			strcpy(&pTempBuffer[nLen], ")MaxSDUSize(");
			nLen += strlen(&pTempBuffer[nLen]);
			EnumInfo.nCount = QOS99MAXIMUMSDUSIZEENUMINFO_LENGTH; 
			EnumInfo.pInfoTable = QoS99MaximumSDUSizeEnumInfo;
			if (umtsQosInfo->QosExt.MaximumSDUsize >= 1 && umtsQosInfo->QosExt.MaximumSDUsize <= 150)
				nLen += sprintf(&pTempBuffer[nLen], "%doctets", umtsQosInfo->QosExt.MaximumSDUsize * 10);
			else
				nLen += EnumToStrForQoS99(&EnumInfo, umtsQosInfo->QosExt.MaximumSDUsize, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

			strcpy(&pTempBuffer[nLen], ")MaxBRUpLk(");
			nLen += strlen(&pTempBuffer[nLen]);
			EnumInfo.nCount = QOS99MAXIMUMBITRATEFORUPLINKENUMINFO_LENGTH; 
			EnumInfo.pInfoTable = QoS99MaximumBitRateForUplinkEnumInfo;
			if (umtsQosInfo->QosExt.MaximumBitRateForUplink >= 1 && umtsQosInfo->QosExt.MaximumBitRateForUplink <= 63)
				nLen += sprintf(&pTempBuffer[nLen], "%dkbps", umtsQosInfo->QosExt.MaximumBitRateForUplink);
			else if (umtsQosInfo->QosExt.MaximumBitRateForUplink >= 64 && umtsQosInfo->QosExt.MaximumBitRateForUplink <= 127)  //增加64到127的判断 ZhengYuqun 2003-09-10  SWPD01505
				nLen += sprintf(&pTempBuffer[nLen], "%dkbps", 64 + (umtsQosInfo->QosExt.MaximumBitRateForUplink - 64) * 8);
			else if (umtsQosInfo->QosExt.MaximumBitRateForUplink >= 128 && umtsQosInfo->QosExt.MaximumBitRateForUplink <= 254)
				nLen += sprintf(&pTempBuffer[nLen], "%dkbps", 576 + (umtsQosInfo->QosExt.MaximumBitRateForUplink - 128) * 64);
			else
				nLen += EnumToStrForQoS99(&EnumInfo, umtsQosInfo->QosExt.MaximumBitRateForUplink, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

			strcpy(&pTempBuffer[nLen], ")MaxBRDnLk(");
			nLen += strlen(&pTempBuffer[nLen]);
			EnumInfo.nCount = QOS99MAXIMUMBITRATEFORDOWNLINKENUMINFO_LENGTH; 
			EnumInfo.pInfoTable = QoS99MaximumBitRateForDownlinkEnumInfo;
			if (umtsQosInfo->QosExt.MaximumBitRateForDownlink >= 1 && umtsQosInfo->QosExt.MaximumBitRateForDownlink <= 63)
				nLen += sprintf(&pTempBuffer[nLen], "%dkbps", umtsQosInfo->QosExt.MaximumBitRateForDownlink);
			else if (umtsQosInfo->QosExt.MaximumBitRateForDownlink >= 64 && umtsQosInfo->QosExt.MaximumBitRateForDownlink <= 127)  //增加64到127的判断 ZhengYuqun 2003-09-10  SWPD01505
				nLen += sprintf(&pTempBuffer[nLen], "%dkbps", 64 + (umtsQosInfo->QosExt.MaximumBitRateForDownlink - 64) * 8);
			else if (umtsQosInfo->QosExt.MaximumBitRateForDownlink >= 128 && umtsQosInfo->QosExt.MaximumBitRateForDownlink <= 254)
				nLen += sprintf(&pTempBuffer[nLen], "%dkbps", 576 + (umtsQosInfo->QosExt.MaximumBitRateForDownlink - 128) * 64);
			else
				nLen += EnumToStrForQoS99(&EnumInfo, umtsQosInfo->QosExt.MaximumBitRateForDownlink, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

			strcpy(&pTempBuffer[nLen], ")SDUErrRatio(");
			nLen += strlen(&pTempBuffer[nLen]);
			EnumInfo.nCount = QOS99SDUERRORRATIOENUMINFO_LENGTH; 
			EnumInfo.pInfoTable = QoS99SDUErrorRatioEnumInfo;
			nLen += EnumToStrForQoS99(&EnumInfo, umtsQosInfo->QosExt.SDUErrorRatio, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

			strcpy(&pTempBuffer[nLen], ")ResidBER(");
			nLen += strlen(&pTempBuffer[nLen]);
			EnumInfo.nCount = QOS99RESIDUALBERENUMINFO_LENGTH; 
			EnumInfo.pInfoTable = QoS99ResidualBEREnumInfo;
			nLen += EnumToStrForQoS99(&EnumInfo, umtsQosInfo->QosExt.ResidualBER, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

			strcpy(&pTempBuffer[nLen], ")TraffPrior(");
			nLen += strlen(&pTempBuffer[nLen]);
			EnumInfo.nCount = QOS99TRAFFICHANDLINGPRIORITYENUMINFO_LENGTH; 
			EnumInfo.pInfoTable = QoS99TrafficHandlingPriorityEnumInfo;
			nLen += EnumToStrForQoS99(&EnumInfo, umtsQosInfo->QosExt.TrafficHandlingPriority, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

			strcpy(&pTempBuffer[nLen], ")TransDelay(");
			nLen += strlen(&pTempBuffer[nLen]);
			EnumInfo.nCount = QOS99TRANSFERDELAYENUMINFO_LENGTH; 
			EnumInfo.pInfoTable = QoS99TransferDelayEnumInfo;
			if (umtsQosInfo->QosExt.TransferDelay >= 1 && umtsQosInfo->QosExt.TransferDelay <= 15)
				nLen += sprintf(&pTempBuffer[nLen], "%dms", umtsQosInfo->QosExt.TransferDelay * 10);
			else if (umtsQosInfo->QosExt.TransferDelay >= 16 && umtsQosInfo->QosExt.TransferDelay <= 31)
				nLen += sprintf(&pTempBuffer[nLen], "%dms", 200 + (umtsQosInfo->QosExt.TransferDelay - 16) * 50);
			else if (umtsQosInfo->QosExt.TransferDelay >= 32 && umtsQosInfo->QosExt.TransferDelay <= 62)
				nLen += sprintf(&pTempBuffer[nLen], "%dms", 1000 + (umtsQosInfo->QosExt.TransferDelay - 32) * 100);
			else
				nLen += EnumToStrForQoS99(&EnumInfo, umtsQosInfo->QosExt.TransferDelay, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

			strcpy(&pTempBuffer[nLen], ")GntBRUpLk(");
			nLen += strlen(&pTempBuffer[nLen]);
			EnumInfo.nCount = QOS99GUARANTEEDBITRATEFORUPLINKENUMINFO_LENGTH; 
			EnumInfo.pInfoTable = QoS99GuaranteedBitRateForUplinkEnumInfo;
			if (umtsQosInfo->QosExt.GuaranteedBitRateForUplink >= 1 && umtsQosInfo->QosExt.GuaranteedBitRateForUplink <= 63)
				nLen += sprintf(&pTempBuffer[nLen], "%dkbps", umtsQosInfo->QosExt.GuaranteedBitRateForUplink);
			else if (umtsQosInfo->QosExt.GuaranteedBitRateForUplink >= 64 && umtsQosInfo->QosExt.GuaranteedBitRateForUplink <= 127)  //增加64到127的判断 ZhengYuqun 2003-09-10  SWPD01505
				nLen += sprintf(&pTempBuffer[nLen], "%dkbps", 64 + (umtsQosInfo->QosExt.GuaranteedBitRateForUplink - 64) * 8);			
			else if (umtsQosInfo->QosExt.GuaranteedBitRateForUplink >= 128 && umtsQosInfo->QosExt.GuaranteedBitRateForUplink <= 254)
				nLen += sprintf(&pTempBuffer[nLen], "%dkbps", 576 + (umtsQosInfo->QosExt.GuaranteedBitRateForUplink - 128) * 64);
			else
				nLen += EnumToStrForQoS99(&EnumInfo, umtsQosInfo->QosExt.GuaranteedBitRateForUplink, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

			strcpy(&pTempBuffer[nLen], ")GntBRDnLk(");
			nLen += strlen(&pTempBuffer[nLen]);
			EnumInfo.nCount = QOS99GUARANTEEDBITRATEFORDOWNLINKENUMINFO_LENGTH; 
			EnumInfo.pInfoTable = QoS99GuaranteedBitRateForDownlinkEnumInfo;
			if (umtsQosInfo->QosExt.GuaranteedBitRateForDownlink >= 1 && umtsQosInfo->QosExt.GuaranteedBitRateForDownlink <= 63)
				nLen += sprintf(&pTempBuffer[nLen], "%dkbps", umtsQosInfo->QosExt.GuaranteedBitRateForDownlink);
			else if (umtsQosInfo->QosExt.GuaranteedBitRateForDownlink >= 64 && umtsQosInfo->QosExt.GuaranteedBitRateForDownlink <= 127)  //增加64到127的判断 ZhengYuqun 2003-09-10  SWPD01505
				nLen += sprintf(&pTempBuffer[nLen], "%dkbps", 64 + (umtsQosInfo->QosExt.GuaranteedBitRateForDownlink - 64) * 8);
			else if (umtsQosInfo->QosExt.GuaranteedBitRateForDownlink >= 128 && umtsQosInfo->QosExt.GuaranteedBitRateForDownlink <= 254)
				nLen += sprintf(&pTempBuffer[nLen], "%dkbps", 576 + (umtsQosInfo->QosExt.GuaranteedBitRateForDownlink - 128) * 64);
			else
				nLen += EnumToStrForQoS99(&EnumInfo, umtsQosInfo->QosExt.GuaranteedBitRateForDownlink, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);
			pTempBuffer[nLen++] = ')';
			
			pTempBuffer[nLen] = 0;
			//if (nLen < nLength)
			//	memcpy(pBuffer, pTempBuffer, nLen + 1);
			//else
			if (nLen >= nLength)
				nLen = 0;
		}
		else
			nLen = HexOctecToStr((LPOCTECSTRING)&pQosInfo->u.umtsQosInformation, pBuffer, nLength, "0x", FALSE);
	}
	else
	{
		pBuffer[0] = 0;
	}
	return nLen;
}


/******************************************************
// Function name: 	DiagnosticsToStr
// Author:        Libo
// Date:          01-5-21
// Description:   转换Diagnostics类型的数据为字符串
// Input:
//       arg1 :CDR3G_Diagnostics *pDiagnostics	待转换的数据
//       arg2 :char *pBuffer		转换后数据存放的Buffer
//       arg3 :int nLength			Buffer的可用长度
// Return value: int				转换后字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-5-21       Libo
******************************************************/
int DiagnosticsToStr(CDR3G_Diagnostics *pDiagnostics, char *pBuffer, int nLength)
{
//#define DIAGNOSTICSBUFFERLENGTH	65536   //Commented by ZhengYuqun 2001/09/26
#define DIAGNOSTICSBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	int nLen = 0;

	//ASSERT((BOOL)(pBuffer && pDiagnostics));
	if ((pBuffer == NULL) || (pDiagnostics == NULL))
	{
	     return 0;
	}

	//char *pTempBuffer = new char[DIAGNOSTICSBUFFERLENGTH];
	//Modified By ZhengYuqun 2001/09/26
	//char *pTempBuffer = g_p3GTempBuffer;
	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;

	switch (pDiagnostics->choice)
	{
	case CDR3G_gsm0408Cause_chosen:
		{
			strcpy(pTempBuffer, "GSM0408Cause:");
			nLen += strlen("GSM0408Cause:");
			nLen += NumToDecStr(pDiagnostics->u.gsm0408Cause, &pTempBuffer[nLen], 
								nLength - nLen, FALSE);
		}
		break;
	case CDR3G_gsm0902MapErrorValue_chosen :
		{
			strcpy(pTempBuffer, "GSM0902MapErrorValue:");
			nLen += strlen("GSM0902MapErrorValue:");
			nLen += NumToDecStr(pDiagnostics->u.gsm0902MapErrorValue, &pTempBuffer[nLen],
								nLength - nLen, FALSE);
		}
		break;
	case CDR3G_ccittQ767Cause_chosen:
		{
			strcpy(pTempBuffer, "ccittQ767Cause:");
			nLen += strlen("ccittQ767Cause:");
			nLen += NumToDecStr(pDiagnostics->u.ccittQ767Cause, &pTempBuffer[nLen],
								nLength - nLen, FALSE);
		}
		break;
	case CDR3G_networkSpecificCause_chosen:
		{
			strcpy(pTempBuffer, "NetworkSpecificCause:(");
			nLen += strlen("NetworkSpecificCause:(");
			nLen += RecordExtensionToStr(&pDiagnostics->u.networkSpecificCause, &pTempBuffer[nLen],
										 nLength - nLen);
			pTempBuffer[nLen++] = ')';
		}
		break;
	case CDR3G_manufacturerSpecificCause_chosen:
		{
			strcpy(pTempBuffer, "ManufacturerSpecificCause:(");
			nLen += strlen("ManufacturerSpecificCause:(");
			nLen += RecordExtensionToStr(&pDiagnostics->u.manufacturerSpecificCause, &pTempBuffer[nLen],
										 nLength - nLen);
			pTempBuffer[nLen++] = ')';
		}
		break;
	default:
		break;
	}

	pTempBuffer[nLen] = 0;
	//if (nLen < nLength)
	//	memcpy(pBuffer, pTempBuffer, nLen + 1);
	//else
	//	nLen = 0;
	if(nLen >= nLength)
	{
		nLen = 0;
	}
	//delete []pTempBuffer; Commented By ZhengYuqun 2001/09/26
	return nLen;
}


/******************************************************
// Function name: 	RecordExtensionToStr
// Author:        Libo
// Date:          01-5-21
// Description:   转换CDR3G_ManagementExtension类型的数据为字符串
// Input:
//       arg1 :CDR3G_ManagementExtension *pData	待转换的数据
//       arg2 :char *pBuffer		转换后数据存放的Buffer
//       arg3 :int nLength			Buffer的可用长度
// Return value: int				转换后字符串的长度
// Notes:
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-5-21       Libo
******************************************************/
int RecordExtensionToStr(CDR3G_ManagementExtension *pData, char *pBuffer, int nLength)
{
//#define RECORDEXTENSIONBUFFERLENGTH 65536   //Commented by ZhengYuqun 2001/09/26
#define RECORDEXTENSIONBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	int nLen = 0;

	pBuffer[0] = 0;

	//ASSERT((BOOL)(pBuffer && pData));
	if ((pBuffer == NULL) || (pData == NULL))
	{
	     return 0;
	}

	//char *pTempBuffer = new char[RECORDEXTENSIONBUFFERLENGTH];
	//Modified By ZhengYuqun 2001/09/26
	//char *pTempBuffer = g_p3GTempBuffer;
    char *pTempBuffer = pBuffer;

	strcpy(pTempBuffer, "Identifier:");
	nLen += strlen("Identifier:");
	nLen += HexOctecToStr(pData->identifier.length, pData->identifier.value, &pTempBuffer[nLen], 
							nLength - nLen, "0x");  //modified by YanYong 2004-05-27 问题单SPWD05340
	pTempBuffer[nLen++] = ' ';

	if (pData->bit_mask & CDR3G_significance_present)
	{
		strcpy(&pTempBuffer[nLen], "Significance:");
		nLen += strlen("Significance:");
		nLen += BoolToStr(pData->significance, &pTempBuffer[nLen],nLength - nLen, 0);
		pTempBuffer[nLen++] = ' ';  
	}

	strcpy(&pTempBuffer[nLen], "Information:");
	nLen += strlen("Information:");
	nLen += HexOctecToStr(pData->information.length, pData->information.value, &pTempBuffer[nLen],
							nLength - nLen, "0x"); //modified by YanYong 2004-05-27 问题单SPWD05340

	pTempBuffer[nLen] = 0;
	//if (nLen < nLength)
	//	memcpy(pBuffer, pTempBuffer, nLen + 1);
	//else
	//	nLen = 0;
	if(nLen >= nLength)
	{
		nLen = 0;
	}
	//delete []pTempBuffer; Commented By ZhengYuqun 2001/09/26
	return nLen;
}

/******************************************************
// Function name: 	RecordExtensionsToStr
// Author:        Libo
// Date:          01-5-21
// Description:	转换CDR3G_ManagementExtensions类型的数据为字符串   
// Input:
//       arg1 :CDR3G_ManagementExtensions *pData	待转换的数据
//       arg2 :char *pBuffer	转换后数据存放的Buffer
//       arg3 :int nLength		Buffer可用的长度
// Return value: int			转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-5-21       Libo
******************************************************/
int RecordExtensionsToStr(CDR3G_ManagementExtensions *pData, char *pBuffer, int nLength)
{
	int nLen = 0;
	int nTempLen = 0;

	//ASSERT((BOOL)(pBuffer && pData));
	if ((pBuffer == NULL) || (pData == NULL))
	{
	     return 0;
	}

	for (unsigned int i = 0; i < pData->count; i++)
	{
		if (nLength > nLen)
			pBuffer[nLen++] = '{';
		else
		{
			pBuffer[0] = 0;
			nLen = 0;
			break;
		}

		nTempLen = RecordExtensionToStr(&pData->value[i], &pBuffer[nLen], nLength - nLen);
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
// Function name: 	ChangeLocationToStr
// Author:        Libo
// Date:          01-5-21
// Description:   转换CDR3G_ChangeLocation类型的数据为字符串
// Input:
//       arg1 :CDR3G_ChangeLocation *pData	待转换的数据
//       arg2 :char *pBuffer		转换后数据存放的Buffer
//       arg3 :int nLength			Buffer可用的长度
// Return value: int				转换后字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-5-21       Libo
******************************************************/
int ChangeLocationToStr(CDR3G_ChangeLocation *pData, char *pBuffer, int nLength)
{
//#define CHANGELOCATIONBUFFERLENGTH	65536   //Commented by ZhengYuqun 2001/09/26
#define CHANGELOCATIONBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	int nLen = 0;

	//ASSERT((BOOL)(pBuffer && pData));
	if ((pBuffer == NULL) || (pData == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;

	//char *pTempBuffer = new char[CHANGELOCATIONBUFFERLENGTH];
	//Modified By ZhengYuqun 2001/09/26
	//char *pTempBuffer = g_p3GTempBuffer;
	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;

	strcpy(pTempBuffer, "LocationAreaCode:");
	nLen += strlen("LocationAreaCode:");
	//updateed by maimaoshi,2003-01-04
	//nLen += DecOctecToStr((LPOCTECSTRING)&pData->locationAreaCode, &pTempBuffer[nLen], 
	//						nLength - nLen, FALSE);
	nLen += HexOctecToStr((LPOCTECSTRING)&pData->locationAreaCode, &pTempBuffer[nLen], 
							nLength - nLen, "0x", FALSE);
	//end by maimaoshi
						
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "RoutingAreaCode:");
	nLen += strlen("RoutingAreaCode:");
	nLen += HexOctecToStr((LPOCTECSTRING)&pData->routingAreaCode, &pTempBuffer[nLen],
							nLength - nLen, "0x");
	pTempBuffer[nLen++] = ' ';

	if (pData->bit_mask & CDR3G_cellId_present)
	{
		strcpy(&pTempBuffer[nLen], "CellID:");
		nLen += strlen("CellID:");
		//by ldf 2003-02-17 对应问题单 D20359
		nLen += HexOctecToStr((LPOCTECSTRING)&pData->cellId, &pTempBuffer[nLen],
								nLength - nLen, "0x",FALSE);
		//end
		pTempBuffer[nLen++] = ' ';
	}

	strcpy(&pTempBuffer[nLen], "ChangeTime:");
	nLen += strlen("ChangeTime:");
	nLen += TimeStampToStr((LPTIMESTAMP)&pData->changeTime, &pTempBuffer[nLen], 
							nLength - nLen);
	pTempBuffer[nLen] = 0;
	//if (nLen < nLength)
	//	memcpy(pBuffer, pTempBuffer, nLen + 1);
	//else
	//	nLen = 0;
	if(nLen >= nLength)
	{
		nLen = 0;
	}
	//delete []pTempBuffer; Commented By ZhengYuqun 2001/09/26
	return nLen;
}


/******************************************************
// Function name: 	ChangeLocationsToStr
// Author:        Libo
// Date:          01-5-21
// Description:   转换CHANGELOCATIONS类型的数据为字符串
// Input:
//       arg1 :CDR3G__seqof3 *pData		待转换的数据
//       arg2 :char *pBuffer			转换后数据存放的Buffer
//       arg3 :int nLength				Buffer可用的长度
// Return value: int					转换后字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-5-21       Libo
******************************************************/
int ChangeLocationsToStr(CDR3G__seqof3 *pData, char *pBuffer, int nLength)
{
	int nLen = 0;
	int nTempLen = 0;

	//ASSERT((BOOL)(pBuffer && pData));
	if ((pBuffer == NULL) || (pData == NULL))
	{
	     return 0;
	}
	
	pBuffer[nLen++] = '{';
	for (unsigned int i = 0; i < pData->count; i++)
	{
		if (nLength > nLen)
			pBuffer[nLen++] = '{';
		else
		{
			pBuffer[0] = 0;			
			return 0;
		}

		nTempLen = ChangeLocationToStr(&pData->value[i], &pBuffer[nLen], nLength - nLen);
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
// Function name: 	ChargingCharacteristicsToStr
// Author:        Libo
// Date:          01-5-23
// Description:   转换CDR3G_ChargingCharacteristics类型的数据为字符串
// Input:
//       arg1 :CDR3G_ChargingCharacteristics *pData	待转换的数据
//       arg2 :char *pBuffer	转换后数据存放的Buffer
//       arg3 :int nLength		Buffer的可用长度
// Return value: int			转换后字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-5-23       Libo
******************************************************/
int ChargingCharacteristicsToStr(CDR3G_ChargingCharacteristics *pData, char *pBuffer, int nLength)
{
//#define CCBUFFERLENGTH 65536   //Commented by ZhengYuqun 2001/09/26
#define CCBUFFERLENGTH _BROWSETEMPBUFFERLENGTH
	
	int nLen = 0;

	//ASSERT((BOOL)(pBuffer && pData));
	if ((pBuffer == NULL) || (pData == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;

	//char *pTempBuffer = new char[CCBUFFERLENGTH];
	//Modified By ZhengYuqun 2001/09/26
	//char *pTempBuffer = g_p3GTempBuffer;
	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;

	switch (pData->value[0] & 0x0f)
	{
	case 1:
		{
			strcpy(pTempBuffer, "Hot Billing");
			nLen = strlen("Hot Billing");
		}
		break;
	case 2:
		{
			strcpy(pTempBuffer, "Flat Rate");
			nLen = strlen("Flat Rate");
		}
		break;
	case 4:
		{
            //对应问题单D20103 李杜芳 2003-01-14
			strcpy(pTempBuffer, "Prepaid service");
			nLen = strlen("Prepaid service");
		}
		break;
	case 8:
		{
            //对应问题单D20103 李杜芳 2003-01-14
			strcpy(pTempBuffer, "Normal billing");
			nLen = strlen("Normal billing");
		}
		break;
	default:
		{
			sprintf(pTempBuffer, "Unknown value(0x%02X%02X)", pData->value[0], pData->value[1]);
			nLen = strlen(pTempBuffer);
		}
		break;
	}
	
	//if (nLen < nLength)
	//	strcpy(pBuffer, pTempBuffer);
	//else
	//	nLen = 0;
	if(nLen >= nLength)
	{
		nLen = 0;
	}
	
	//delete []pTempBuffer; Commented By ZhengYuqun 2001/09/26
	return nLen;
}


/******************************************************
// Function name: 	SystemTypeToStr
// Author:        Libo
// Date:          01-5-23
// Description:   转换CDR3G_SystemType类型的数据为字符串
// Input:
//       arg1 :CDR3G_SystemType *pData	待转换的数据
//       arg2 :char *pBuffer			转换后数据存放的Buffer
//       arg3 :int nLength				Buffer的可用长度
// Return value: int					转换后字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-5-23       Libo
******************************************************/
int SystemTypeToStr(CDR3G_SystemType *pData, char *pBuffer, int nLength)
{
//#define STBUFFERLENGTH 65536   //Commented by ZhengYuqun 2001/09/26
#define STBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	int nLen = 0;

	//ASSERT((BOOL)(pBuffer && pData));
	if ((pBuffer == NULL) || (pData == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;

	//char *pTempBuffer = new char[STBUFFERLENGTH];
	//Modified By ZhengYuqun 2001/09/26
	//char *pTempBuffer = g_p3GTempBuffer;
	char *pTempBuffer = pBuffer;


	if (!pTempBuffer)
		return 0;

	if (*pData == CDR3G_iuUTRAN)
	{
		strcpy(pTempBuffer, "3G System");
		nLen = strlen("3G System");
	}
	else
	{
		sprintf(pTempBuffer, "Unknown value(0x%08X)", *pData);
		nLen = strlen(pTempBuffer);
	}

	//if (nLen < nLength)
	//	strcpy(pBuffer, pTempBuffer);
	//else
	//	nLen = 0;
	if(nLen >= nLength)
	{
		nLen = 0;
	}

	//delete []pTempBuffer; Commented By ZhengYuqun 2001/09/26
	return nLen;
}

/******************************************************
// Function name: 	GetTextValueOf_Q_3GSGSNPDP
// Author:        ZhengYuqun
// Date:          01-7-30
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
//01-7-30       ZhengYuqun
******************************************************/
int GetTextValueOf_Q_3GSGSNPDP(char *pValue,
                               char *pBuffer,
                               int nLength,
                               QUERY_COND_FIELDS_VALUE* pValues)
{
//#define SCDRBUFFERLENGTH 65536   //Commented by ZhengYuqun 2001/09/26
#define SCDRBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	CDR3G_SGSNPDPRecord *pBill = (CDR3G_SGSNPDPRecord*)pValue;
	int nLen = 0;
    int nTempLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != CDR3G_CallEventRecordType_sgsnPDPRecord)
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
                EnumInfo.nCount = R99_360_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R99_360_Recorde_Type_EnumInfo;
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
                EnumInfo.nCount = CAUSEFORRECCLOSINGENUNINFO_LENGTH;
                EnumInfo.pInfoTable = CauseForRecClosingEnumInfo;
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
                if (pBill->bit_mask & CDR3G_SGSNPDPRecord_recordSequenceNumber_present)
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
                if (pBill->bit_mask & CDR3G_SGSNPDPRecord_servedMSISDN_present)
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
            //chargingCharacteristics
            else if(value.sFieldName.compare("chargingCharacteristics") == 0) 
            {
                if (pBill->bit_mask & CDR3G_SGSNPDPRecord_chargingCharacteristics_present)
                {
                    ChargingCharacteristicsToStr(&pBill->chargingCharacteristics, pTempBuffer, nLength);
                    value.sFieldValue = pTempBuffer;
                }
                else
                {
                    value.sFieldValue = _EMPTYSTRING;
                }
                nLen++;
            }
            //systemType
            else if(value.sFieldName.compare("systemType") == 0)
            {
                if (pBill->bit_mask & CDR3G_SGSNPDPRecord_systemType_present)
                {
                    ENUMINFO EnumInfo;
                    EnumInfo.nCount = SYSTEMTYPEENUMINFO_LENGTH;
                    EnumInfo.pInfoTable =  SYSTEMTYPEENUMINFOEnumInfo;
                    EnumToStr(&EnumInfo,
                              pBill->systemType,
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
// Function name: 	GetTextValueOf_Q_3GGGSNPDP
// Author:        ZhengYuqun
// Date:          01-7-30
// Description:   处理G-CDR话单浏览解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-7-30       ZhengYuqun
******************************************************/
int GetTextValueOf_Q_3GGGSNPDP(char *pValue,
                               char *pBuffer,
                               int nLength,
                               QUERY_COND_FIELDS_VALUE* pValues)
{
//#define GCDRBUFFERLENGTH 65536   //Commented by ZhengYuqun 2001/09/26
#define GCDRBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	CDR3G_GGSNPDPRecord *pBill = (CDR3G_GGSNPDPRecord*)pValue;
	int nLen = 0;
    int nTempLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != CDR3G_CallEventRecordType_ggsnPDPRecord)
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
                EnumInfo.nCount = R99_360_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R99_360_Recorde_Type_EnumInfo;
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
	            EnumInfo.nCount = CAUSEFORRECCLOSINGENUNINFO_LENGTH;
	            EnumInfo.pInfoTable = CauseForRecClosingEnumInfo;
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
                if (pBill->bit_mask & CDR3G_GGSNPDPRecord_recordSequenceNumber_present)
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
                if (pBill->bit_mask & CDR3G_GGSNPDPRecord_servedMSISDN_present)
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
            //chargingCharacteristics
            else if(value.sFieldName.compare("chargingCharacteristics") == 0) 
            {
                if (pBill->bit_mask & CDR3G_GGSNPDPRecord_chargingCharacteristics_present)
                {
                    ChargingCharacteristicsToStr(&pBill->chargingCharacteristics, pTempBuffer, nLength);
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
// Function name: 	GetTextValueOf_Q_3GSGSNMM
// Author:        ZhengYuqun
// Date:          01-7-30
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
//01-7-30       ZhengYuqun
******************************************************/
int GetTextValueOf_Q_3GSGSNMM(char *pValue,
                              char *pBuffer,
                              int nLength,
                              QUERY_COND_FIELDS_VALUE* pValues)
{
//#define MCDRBUFFERLENGTH 65536   //Commented by ZhengYuqun 2001/09/26
#define MCDRBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	CDR3G_SGSNMMRecord *pBill = (CDR3G_SGSNMMRecord*)pValue;
	int nLen = 0;
    int nTempLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != CDR3G_CallEventRecordType_sgsnMMRecord)
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
                EnumInfo.nCount = R99_360_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R99_360_Recorde_Type_EnumInfo;
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
                if (pBill->bit_mask & CDR3G_duration_present)
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
	            EnumInfo.nCount = CAUSEFORRECCLOSINGENUNINFO_LENGTH;
	            EnumInfo.pInfoTable = CauseForRecClosingEnumInfo;
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
                if (pBill->bit_mask & CDR3G_SGSNMMRecord_recordSequenceNumber_present)
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
                if (pBill->bit_mask & CDR3G_SGSNMMRecord_servedMSISDN_present)
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
            //chargingCharacteristics
            else if(value.sFieldName.compare("chargingCharacteristics") == 0) 
            {
                if (pBill->bit_mask & CDR3G_SGSNMMRecord_chargingCharacteristics_present)
                {
                    ChargingCharacteristicsToStr(&pBill->chargingCharacteristics, pTempBuffer, nLength);
                    value.sFieldValue = pTempBuffer;
                }
                else
                {
                    value.sFieldValue = _EMPTYSTRING;
                }
                nLen++;
            }
            //systemType
            else if(value.sFieldName.compare("systemType") == 0)
            {
                if (pBill->bit_mask & CDR3G_SGSNMMRecord_systemType_present)
                {
                    ENUMINFO EnumInfo;
	                EnumInfo.nCount = SYSTEMTYPEENUMINFO_LENGTH;
	                EnumInfo.pInfoTable =  SYSTEMTYPEENUMINFOEnumInfo;
                    EnumToStr(&EnumInfo,
                              pBill->systemType,
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
// Function name: 	GetTextValueOf_Q_3GSGSNSMO
// Author:        ZhengYuqun
// Date:          01-7-30
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
//01-7-30       ZhengYuqun
******************************************************/
int GetTextValueOf_Q_3GSGSNSMO(char *pValue,
                               char *pBuffer,
                               int nLength,
                               QUERY_COND_FIELDS_VALUE* pValues)
{
//#define SMOBUFFERLENGTH 65536   //Commented by ZhengYuqun 2001/09/26
#define SMOBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	CDR3G_SGSNSMORecord *pBill = (CDR3G_SGSNSMORecord*)pValue;
	int nLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != CDR3G_CallEventRecordType_sgsnSMORecord)
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
                EnumInfo.nCount = R99_360_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R99_360_Recorde_Type_EnumInfo;
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
                if (pBill->bit_mask & CDR3G_SGSNSMORecord_servedMSISDN_present)
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
            //chargingCharacteristics
            else if(value.sFieldName.compare("chargingCharacteristics") == 0) 
            {
                if (pBill->bit_mask & CDR3G_SGSNSMORecord_chargingCharacteristics_present)
                {
                    ChargingCharacteristicsToStr(&pBill->chargingCharacteristics, pTempBuffer, nLength);
                    value.sFieldValue = pTempBuffer;
                }
                else
                {
                    value.sFieldValue = _EMPTYSTRING;
                }
                nLen++;
            }
            //systemType
            else if(value.sFieldName.compare("systemType") == 0)
            {
                if (pBill->bit_mask & CDR3G_SGSNSMORecord_systemType_present)
                {
                    ENUMINFO EnumInfo;
	                EnumInfo.nCount = SYSTEMTYPEENUMINFO_LENGTH;
	                EnumInfo.pInfoTable =  SYSTEMTYPEENUMINFOEnumInfo;
                    EnumToStr(&EnumInfo,
                              pBill->systemType,
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
// Function name: 	GetTextValueOf_Q_3GSGSNSMT
// Author:        ZhengYuqun
// Date:          01-7-30
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
//01-7-30       ZhengYuqun
******************************************************/
int GetTextValueOf_Q_3GSGSNSMT(char *pValue,
                               char *pBuffer,
                               int nLength,
                               QUERY_COND_FIELDS_VALUE* pValues)
{
//#define SMTBUFFERLENGTH 65536   //Commented by ZhengYuqun 2001/09/26
#define SMTBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	CDR3G_SGSNSMTRecord *pBill = (CDR3G_SGSNSMTRecord*)pValue;
	int nLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != CDR3G_CallEventRecordType_sgsnSMTRecord)
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
                EnumInfo.nCount = R99_360_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R99_360_Recorde_Type_EnumInfo;
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
                if (pBill->bit_mask & CDR3G_SGSNSMTRecord_servedMSISDN_present)
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
            //chargingCharacteristics
            else if(value.sFieldName.compare("chargingCharacteristics") == 0) 
            {
                if (pBill->bit_mask & CDR3G_SGSNSMTRecord_chargingCharacteristics_present)
                {
                    ChargingCharacteristicsToStr(&pBill->chargingCharacteristics, pTempBuffer, nLength);
                    value.sFieldValue = pTempBuffer;
                }
                else
                {
                    value.sFieldValue = _EMPTYSTRING;
                }
                nLen++;
            }
            //systemType
            else if(value.sFieldName.compare("systemType") == 0)
            {
                if (pBill->bit_mask & CDR3G_SGSNSMTRecord_systemType_present)
                {
                    ENUMINFO EnumInfo;
	                EnumInfo.nCount = SYSTEMTYPEENUMINFO_LENGTH;
	                EnumInfo.pInfoTable =  SYSTEMTYPEENUMINFOEnumInfo;
                    EnumToStr(&EnumInfo,
                              pBill->systemType,
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
// Function name: 	GetTextValueOf_Q_3GLARGESGSNPDP
// Author:        ZhengYuqun
// Date:          01-7-30
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
//01-7-30       ZhengYuqun
******************************************************/
int GetTextValueOf_Q_3GLARGESGSNPDP(char *pValue,
                                    char *pBuffer,
                                    int nLength,
                                    QUERY_COND_FIELDS_VALUE* pValues)
{
	_3GCDR_SGSNPDPRecord *pBill = (_3GCDR_SGSNPDPRecord*)pValue;
	int nLen = 0;
    int nTempLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != _3GCDR_CallEventRecordType_sgsnPDPRecord)
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
                EnumInfo.nCount = R99_360_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R99_360_Recorde_Type_EnumInfo;
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
	            EnumInfo.nCount = CAUSEFORRECCLOSINGENUNINFO_LENGTH;
	            EnumInfo.pInfoTable = CauseForRecClosingEnumInfo;
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
                if (pBill->bit_mask & _3GCDR_SGSNPDPRecord_servedMSISDN_present)
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
            //chargingCharacteristics
            else if(value.sFieldName.compare("chargingCharacteristics") == 0) 
            {
                if (pBill->bit_mask & _3GCDR_SGSNPDPRecord_chargingCharacteristics_present)
                {
                    ChargingCharacteristicsToStr((CDR3G_ChargingCharacteristics*)&pBill->chargingCharacteristics, pTempBuffer, nLength);
                    value.sFieldValue = pTempBuffer;
                }
                else
                {
                    value.sFieldValue = _EMPTYSTRING;
                }
                nLen++;
            }
            //systemType
            else if(value.sFieldName.compare("systemType") == 0)
            {
                if (pBill->bit_mask & _3GCDR_SGSNPDPRecord_systemType_present)
                {
                    ENUMINFO EnumInfo;
	                EnumInfo.nCount = SYSTEMTYPEENUMINFO_LENGTH;
	                EnumInfo.pInfoTable =  SYSTEMTYPEENUMINFOEnumInfo;
                    EnumToStr(&EnumInfo,
                              pBill->systemType,
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
// Function name: 	GetTextValueOf3GLARGEGGSNPDP
// Author:        ZhengYuqun
// Date:          01-7-30
// Description:   处理合并后G-CDR话单浏览解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-7-30       ZhengYuqun
******************************************************/
int GetTextValueOf_Q_3GLARGEGGSNPDP(char *pValue,
                                    char *pBuffer,
                                    int nLength,
                                    QUERY_COND_FIELDS_VALUE* pValues)
{
	_3GCDR_GGSNPDPRecord *pBill = (_3GCDR_GGSNPDPRecord*)pValue;
	int nLen = 0;
    int nTempLen = 0;

	if ((pValue == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	if (pBill->recordType != _3GCDR_CallEventRecordType_ggsnPDPRecord)
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
                EnumInfo.nCount = R99_360_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R99_360_Recorde_Type_EnumInfo;
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
	            EnumInfo.nCount = CAUSEFORRECCLOSINGENUNINFO_LENGTH;
	            EnumInfo.pInfoTable = CauseForRecClosingEnumInfo;
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
                if (pBill->bit_mask & _3GCDR_GGSNPDPRecord_servedMSISDN_present)
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
            //chargingCharacteristics
            else if(value.sFieldName.compare("chargingCharacteristics") == 0) 
            {
                if (pBill->bit_mask & _3GCDR_GGSNPDPRecord_chargingCharacteristics_present)
                {
                    ChargingCharacteristicsToStr((CDR3G_ChargingCharacteristics*)&pBill->chargingCharacteristics, pTempBuffer, nLength);
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
// Function name: 	GetTextValueOf_Q_3GLARGESGSNMM
// Author:        ZhengYuqun
// Date:          01-7-30
// Description:   处理合并后M-CDR话单浏览解释的函数
// Input:
//       arg1 :char *pValue		话单数据
//       arg2 :char *pBuffer	存放结果的数据区
//       arg3 :int nLength		数据区的有效长度
// Return value: int			结果字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-7-30       ZhengYuqun
******************************************************/
int GetTextValueOf_Q_3GLARGESGSNMM(char *pValue,
                                   char *pBuffer,
                                   int nLength,
                                   QUERY_COND_FIELDS_VALUE* pValues)
{
	return 0;
}

/******************************************************
// Function name: 	EnumToStrForQOS99
// Author:        ZhengYuqun
// Date:          01-9-29
// Description:   将Enum值按给出的信息转换成对应的字符串，本函数只适用于QoS99
// Input:
//       arg1 :LPENUMINFO pEnumInfo	枚举信息结构的指针
//       arg2 :int nEnumValue		待转换的枚举值
//       arg3 :char *pBuffer		存放返回字符串的数据区
//       arg4 :int nLength			数据区可用长度
//		 arg5 :_3GCDR_NetworkInitiatedPDPContext networkInitiation  指示方向是MS to netWork还是netWork to MS
// Return value: int				转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//01-4-28       ZhengYuqun
******************************************************/
int EnumToStrForQoS99(LPENUMINFO pEnumInfo, int nEnumValue, char *pBuffer, int nLength, char networkInitiation)
{

	//ASSERT((BOOL)(pBuffer && pEnumInfo));
	if ((pBuffer == NULL) || (pEnumInfo == NULL))
	{
	     return 0;
	}

	int nLen;

	pBuffer[0] = 0;

	int nCount = pEnumInfo->nCount;
	ENUMITEMINFO *pItem;
	BOOL bSuccess = FALSE;

	//当netWork to MS 的时候，如果值为0，则显示为"Reserved"
	if (networkInitiation == NETWORKTOMSDIRECTION && nEnumValue == 0)
	{
		strcpy(pBuffer, "Reserved");
		nLen = strlen(pBuffer);
		return nLen;
	}

	for (int i = 0; i < nCount; i++)
	{
		pItem = &pEnumInfo->pInfoTable[i];
		if (pItem->nValue == nEnumValue)
		{
			bSuccess = TRUE;
			break;
		}
	}

	if (!bSuccess)
		return 0;
		
	nLen = strlen(pItem->pString);
	if (nLength < (nLen + 1))
		return 0;

	strcpy(pBuffer, pItem->pString);

	return nLen;
}

/******************************************************
// Function name: 	_3GSGSNPDP_CAMELInformationToStr
// Author:        ZouYongsheng
// Date:          02-8-09
// Description:   将SCDR中的cAMELInformationPDP域转化为字符串
// Input:
//       arg1 :CDR3G_CAMELInformationPDP *pData  SCDR中CAMEL域数据区
//       arg2 :char *pBuffer		存放返回字符串的数据区
//       arg3 :int nLength		    数据区可用长度
//
// Return value: int				转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//02-8-09       ZouYongsheng
******************************************************/
int _3GSGSNPDP_CAMELInformationToStr(CDR3G_CAMELInformationPDP *pData, char *pBuffer, int nLength)
{
#define CAMELINFORMATIONPDPLENGTH _BROWSETEMPBUFFERLENGTH

	int nLen = 0;
	
	//ASSERT((BOOL)(pBuffer && pData));
	if ((pBuffer == NULL) || (pData == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;	

	//char *pTempBuffer = g_p3GTempBuffer;
	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;

	strcpy(&pTempBuffer[nLen], "sCFAddress:");
	nLen += strlen("sCFAddress:");
	
	if(pData->bit_mask & CDR3G_CAMELInformationPDP_sCFAddress_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pData->sCFAddress, 
			&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';
	
	strcpy(&pTempBuffer[nLen], "serviceKey:");
	nLen += strlen("serviceKey:");

	if(pData->bit_mask & CDR3G_CAMELInformationPDP_serviceKey_present)
	{
		nLen += NumToDecStr((long)pData->serviceKey, 
			&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "defaultTransactionHandling:");
	nLen += strlen("defaultTransactionHandling:");

	if(pData->bit_mask & CDR3G_CAMELInformationPDP_defaultTransactionHandling_present)
	{
		ENUMINFO EnumInfo;
	    EnumInfo.nCount = DEFAULTTRANSACTIONHANDLINGENUMINFO_LENGTH;
	    EnumInfo.pInfoTable = DEFAULTTRANSACTIONHANDLINGENUMINFO;
	    nLen += EnumToStr(&EnumInfo, pData->defaultTransactionHandling, &pTempBuffer[nLen], 
			         nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "cAMELAccessPointNameNI:");
	nLen += strlen("cAMELAccessPointNameNI:");

	if(pData->bit_mask & CDR3G_cAMELAccessPointNameNI_present)
	{
		nLen += sprintf(&pTempBuffer[nLen], "%s", pData->cAMELAccessPointNameNI);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

    strcpy(&pTempBuffer[nLen], "cAMELAccessPointNameOI:");
	nLen += strlen("cAMELAccessPointNameOI:");

	if(pData->bit_mask & CDR3G_cAMELAccessPointNameOI_present)
	{
		nLen += sprintf(&pTempBuffer[nLen], "%s", pData->cAMELAccessPointNameOI);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "numberOfDPEncountered:");
	nLen += strlen("numberOfDPEncountered:");

    if(pData->bit_mask & CDR3G_CAMELInformationPDP_numberOfDPEncountered_present)
	{
		nLen += NumToDecStr((long)pData->numberOfDPEncountered,
			&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';
       
    strcpy(&pTempBuffer[nLen], "levelOfCAMELService:");
	nLen += strlen("levelOfCAMELService:");

	if(pData->bit_mask & CDR3G_CAMELInformationPDP_levelOfCAMELService_present)
	{
		ENUMINFO EnumInfo;
	    EnumInfo.nCount = LEVELOFCAMELSERVICE_LENGTH;
	    EnumInfo.pInfoTable = LEVELOFCAMELSERVICE;
	    nLen += EnumToStr(&EnumInfo, pData->levelOfCAMELService, &pTempBuffer[nLen], 
			         nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "freeFormatData:");
	nLen += strlen("freeFormatData:");

	if(pData->bit_mask & CDR3G_CAMELInformationPDP_freeFormatData_present)
	{
		nLen += OctecToStr((LPOCTECSTRING)&pData->freeFormatData, 
			&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "fFDAppendIndicator:");
	nLen += strlen("fFDAppendIndicator:");

	if(pData->bit_mask & CDR3G_CAMELInformationPDP_fFDAppendIndicator_present)
	{
		nLen += BoolToStr(pData->fFDAppendIndicator, 
			&pTempBuffer[nLen], nLength - nLen, 1);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}

	pTempBuffer[nLen] = 0;

	//if (nLen < nLength)
	//	memcpy(pBuffer, pTempBuffer, nLen + 1);
	//else
	//	nLen = 0;
	if(nLen >= nLength)
	{
		nLen = 0;
	}
	
	return nLen;	
}

/******************************************************
// Function name: 	_3GLARGESGSNPDP_CAMELInformationToStr
// Author:        ZouYongsheng
// Date:          02-8-09
// Description:   将L_SCDR中的cAMELInformationPDP域转化为字符串
// Input:
//       arg1 :_3GCDR_CAMELInformationPDP *pData  SCDR中CAMEL域数据区
//       arg2 :char *pBuffer		存放返回字符串的数据区
//       arg3 :int nLength		    数据区可用长度
//
// Return value: int				转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//02-8-15       ZouYongsheng
******************************************************/
int _3GLARGESGSNPDP_CAMELInformationToStr(_3GCDR_CAMELInformationPDP *pData, char *pBuffer, int nLength)
{
#define CAMELINFORMATIONPDPLENGTH _BROWSETEMPBUFFERLENGTH

	int nLen = 0;
	
	//ASSERT((BOOL)(pBuffer && pData));
	if ((pBuffer == NULL) || (pData == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;	

	//char *pTempBuffer = g_p3GTempBuffer;
	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;

	strcpy(&pTempBuffer[nLen], "sCFAddress:");
	nLen += strlen("sCFAddress:");
	
	if(pData->bit_mask & _3GCDR_CAMELInformationPDP_sCFAddress_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pData->sCFAddress, 
			&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';
	
	strcpy(&pTempBuffer[nLen], "serviceKey:");
	nLen += strlen("serviceKey:");

	if(pData->bit_mask & _3GCDR_CAMELInformationPDP_serviceKey_present)
	{
		nLen += NumToDecStr((long)pData->serviceKey, 
			&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "defaultTransactionHandling:");
	nLen += strlen("defaultTransactionHandling:");

	if(pData->bit_mask & _3GCDR_CAMELInformationPDP_defaultTransactionHandling_present)
	{
		ENUMINFO EnumInfo;
	    EnumInfo.nCount = DEFAULTTRANSACTIONHANDLINGENUMINFO_LENGTH;
	    EnumInfo.pInfoTable = DEFAULTTRANSACTIONHANDLINGENUMINFO;
	    nLen += EnumToStr(&EnumInfo, pData->defaultTransactionHandling, &pTempBuffer[nLen], 
			nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "cAMELAccessPointNameNI:");
	nLen += strlen("cAMELAccessPointNameNI:");
	
	if(pData->bit_mask & _3GCDR_cAMELAccessPointNameNI_present)
	{
		nLen += sprintf(&pTempBuffer[nLen], "%s", pData->cAMELAccessPointNameNI);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

    strcpy(&pTempBuffer[nLen], "cAMELAccessPointNameOI:");
	nLen += strlen("cAMELAccessPointNameOI:");

	if(pData->bit_mask & _3GCDR_cAMELAccessPointNameOI_present)
	{
		nLen += sprintf(&pTempBuffer[nLen], "%s", pData->cAMELAccessPointNameOI);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "numberOfDPEncountered:");
    nLen += strlen("numberOfDPEncountered:");

    if(pData->bit_mask & _3GCDR_CAMELInformationPDP_numberOfDPEncountered_present)
	{
		nLen += NumToDecStr((long)pData->numberOfDPEncountered,
			&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';
       
    strcpy(&pTempBuffer[nLen], "levelOfCAMELService:");
	nLen += strlen("levelOfCAMELService:");

	if(pData->bit_mask & _3GCDR_CAMELInformationPDP_levelOfCAMELService_present)
	{
		ENUMINFO EnumInfo;
	    EnumInfo.nCount = LEVELOFCAMELSERVICE_LENGTH;
	    EnumInfo.pInfoTable = LEVELOFCAMELSERVICE;
	    nLen += EnumToStr(&EnumInfo, pData->levelOfCAMELService, &pTempBuffer[nLen], 
			         nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "freeFormatData:");
    nLen += strlen("freeFormatData:");

	if(pData->bit_mask & _3GCDR_CAMELInformationPDP_freeFormatData_present)
	{
		nLen += OctecToStr((LPOCTECSTRING)&pData->freeFormatData, 
			&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "fFDAppendIndicator:");
    nLen += strlen("fFDAppendIndicator:");

	if(pData->bit_mask & _3GCDR_CAMELInformationPDP_fFDAppendIndicator_present)
	{
		nLen += BoolToStr(pData->fFDAppendIndicator, 
			&pTempBuffer[nLen], nLength - nLen, 1);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}

	pTempBuffer[nLen] = 0;

	//if (nLen < nLength)
	//	memcpy(pBuffer, pTempBuffer, nLen + 1);
	//else
	//	nLen = 0;
	if(nLen >= nLength)
	{
		nLen = 0;
	}
	
	return nLen;	
}

/******************************************************
// Function name: 	_3GSGSNMM_CAMELInformationToStr
// Author:        ZouYongsheng
// Date:          02-8-16
// Description:   将MCDR中的cAMELInformationMM域转化为字符串
// Input:
//       arg1 :CDR3G_CAMELInformationMM *pData  MCDR中CAMEL域数据区
//       arg2 :char *pBuffer		存放返回字符串的数据区
//       arg3 :int nLength		    数据区可用长度
//
// Return value: int				转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//02-8-16       ZouYongsheng
******************************************************/
int _3GSGSNMM_CAMELInformationToStr(CDR3G_CAMELInformationMM *pData, char *pBuffer, int nLength)
{
#define CAMELINFORMATIONMMLENGTH _BROWSETEMPBUFFERLENGTH

	int nLen = 0;
	
	//ASSERT((BOOL)(pBuffer && pData));
	if ((pBuffer == NULL) || (pData == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;	

	//char *pTempBuffer = g_p3GTempBuffer;
	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;

	strcpy(&pTempBuffer[nLen], "sCFAddress:");
	nLen += strlen("sCFAddress:");
	
	if(pData->bit_mask & CDR3G_CAMELInformationMM_sCFAddress_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pData->sCFAddress, 
			&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';
	
	strcpy(&pTempBuffer[nLen], "serviceKey:");
	nLen += strlen("serviceKey:");

	if(pData->bit_mask & CDR3G_CAMELInformationMM_serviceKey_present)
	{
		nLen += NumToDecStr((long)pData->serviceKey, 
			&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "defaultTransactionHandling:");
	nLen += strlen("defaultTransactionHandling:");

	if(pData->bit_mask & CDR3G_CAMELInformationMM_defaultTransactionHandling_present)
	{
		ENUMINFO EnumInfo;
	    EnumInfo.nCount = DEFAULTTRANSACTIONHANDLINGENUMINFO_LENGTH;
	    EnumInfo.pInfoTable = DEFAULTTRANSACTIONHANDLINGENUMINFO;
	    nLen += EnumToStr(&EnumInfo, pData->defaultTransactionHandling, &pTempBuffer[nLen], 
			         nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "numberOfDPEncountered:");
	nLen += strlen("numberOfDPEncountered:");

    if(pData->bit_mask & CDR3G_CAMELInformationMM_numberOfDPEncountered_present)
	{
		nLen += NumToDecStr((long)pData->numberOfDPEncountered,
			&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';
       
    strcpy(&pTempBuffer[nLen], "levelOfCAMELService:");
	nLen += strlen("levelOfCAMELService:");

	if(pData->bit_mask & CDR3G_CAMELInformationMM_levelOfCAMELService_present)
	{
		ENUMINFO EnumInfo;
	    EnumInfo.nCount = LEVELOFCAMELSERVICE_LENGTH;
	    EnumInfo.pInfoTable = LEVELOFCAMELSERVICE;
	    nLen += EnumToStr(&EnumInfo, pData->levelOfCAMELService, &pTempBuffer[nLen], 
			         nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "freeFormatData:");
	nLen += strlen("freeFormatData:");

	if(pData->bit_mask & CDR3G_CAMELInformationMM_freeFormatData_present)
	{
		nLen += OctecToStr((LPOCTECSTRING)&pData->freeFormatData, 
			&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "fFDAppendIndicator:");
	nLen += strlen("fFDAppendIndicator:");

	if(pData->bit_mask & CDR3G_CAMELInformationMM_fFDAppendIndicator_present)
	{
		nLen += BoolToStr(pData->fFDAppendIndicator, 
			&pTempBuffer[nLen], nLength - nLen, 1);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}

	pTempBuffer[nLen] = 0;

	//if (nLen < nLength)
	//	memcpy(pBuffer, pTempBuffer, nLen + 1);
	//else
	//	nLen = 0;
	if(nLen >= nLength)
	{
		nLen = 0;
	}
	
	return nLen;	
}

/******************************************************
// Function name: _3GLARGESGSNMM_CAMELInformationToStr
// Author:        ZouYongsheng
// Date:          02-8-16
// Description:   将L_MCDR中的cAMELInformationMM域转化为字符串
// Input:
//       arg1 :_3GCDR_CAMELInformationMM *pData  L_MCDR中CAMEL域数据区
//       arg2 :char *pBuffer		存放返回字符串的数据区
//       arg3 :int nLength		    数据区可用长度
//
// Return value: int				转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//02-8-16       ZouYongsheng
******************************************************/
int _3GLARGESGSNMM_CAMELInformationToStr(_3GCDR_CAMELInformationMM *pData, char *pBuffer, int nLength)
{
#define CAMELINFORMATIONMMLENGTH _BROWSETEMPBUFFERLENGTH

	int nLen = 0;
	
	//ASSERT((BOOL)(pBuffer && pData));
	if ((pBuffer == NULL) || (pData == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;	

	//char *pTempBuffer = g_p3GTempBuffer;
	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;

	strcpy(&pTempBuffer[nLen], "sCFAddress:");
	nLen += strlen("sCFAddress:");
	
	if(pData->bit_mask & _3GCDR_CAMELInformationMM_sCFAddress_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pData->sCFAddress, 
			&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';
	
	strcpy(&pTempBuffer[nLen], "serviceKey:");
	nLen += strlen("serviceKey:");

	if(pData->bit_mask & _3GCDR_CAMELInformationMM_serviceKey_present)
	{
		nLen += NumToDecStr((long)pData->serviceKey, 
			&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "defaultTransactionHandling:");
	nLen += strlen("defaultTransactionHandling:");

	if(pData->bit_mask & _3GCDR_CAMELInformationMM_defaultTransactionHandling_present)
	{
		ENUMINFO EnumInfo;
	    EnumInfo.nCount = DEFAULTTRANSACTIONHANDLINGENUMINFO_LENGTH;
	    EnumInfo.pInfoTable = DEFAULTTRANSACTIONHANDLINGENUMINFO;
	    nLen += EnumToStr(&EnumInfo, pData->defaultTransactionHandling, &pTempBuffer[nLen], 
			         nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "numberOfDPEncountered:");
	nLen += strlen("numberOfDPEncountered:");

    if(pData->bit_mask & _3GCDR_CAMELInformationMM_numberOfDPEncountered_present)
	{
		nLen += NumToDecStr((long)pData->numberOfDPEncountered,
			&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';
       
    strcpy(&pTempBuffer[nLen], "levelOfCAMELService:");
	nLen += strlen("levelOfCAMELService:");

	if(pData->bit_mask & _3GCDR_CAMELInformationMM_levelOfCAMELService_present)
	{
		ENUMINFO EnumInfo;
	    EnumInfo.nCount = LEVELOFCAMELSERVICE_LENGTH;
	    EnumInfo.pInfoTable = LEVELOFCAMELSERVICE;
	    nLen += EnumToStr(&EnumInfo, pData->levelOfCAMELService, &pTempBuffer[nLen], 
			         nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "freeFormatData:");
	nLen += strlen("freeFormatData:");

	if(pData->bit_mask & _3GCDR_CAMELInformationMM_freeFormatData_present)
	{
		nLen += OctecToStr((LPOCTECSTRING)&pData->freeFormatData, 
			&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "fFDAppendIndicator:");
	nLen += strlen("fFDAppendIndicator:");

	if(pData->bit_mask & _3GCDR_CAMELInformationMM_fFDAppendIndicator_present)
	{
		nLen += BoolToStr(pData->fFDAppendIndicator, 
			&pTempBuffer[nLen], nLength - nLen, 1);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}

	pTempBuffer[nLen] = 0;

	//if (nLen < nLength)
	//	memcpy(pBuffer, pTempBuffer, nLen + 1);
	//else
	//	nLen = 0;
	if(nLen >= nLength)
	{
		nLen = 0;
	}
	
	return nLen;	
}


/******************************************************
// Function name: _3GSGSNSMO_CAMELInfomationToStr
// Author:        ZouYongsheng
// Date:          02-8-09
// Description:   将SMOCDR中的cAMELInformationSMS域转化为字符串
// Input:
//       arg1 :CDR3G_CAMELInformationSMS *pData  SMOCDR中CAMEL域数据区
//       arg2 :char *pBuffer		存放返回字符串的数据区
//       arg3 :int nLength		    数据区可用长度
//
// Return value: int				转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//02-8-09       ZouYongsheng
******************************************************/
int _3GSGSNSMO_CAMELInfomationToStr(CDR3G_CAMELInformationSMS *pData, char *pBuffer, int nLength)
{
#define CAMELINFORMATIONSMSLENGTH _BROWSETEMPBUFFERLENGTH

	int nLen = 0;
	
	//ASSERT((BOOL)(pBuffer && pData));
	if ((pBuffer == NULL) || (pData == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;	

	//char *pTempBuffer = g_p3GTempBuffer;
	//char *pTempBuffer = g_pTempBillBuffer;
	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;

	strcpy(&pTempBuffer[nLen], "sCFAddress:");
	nLen += strlen("sCFAddress:");
	
	if(pData->bit_mask & CDR3G_CAMELInformationSMS_sCFAddress_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pData->sCFAddress, 
			&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';
	
	strcpy(&pTempBuffer[nLen], "serviceKey:");
	nLen += strlen("serviceKey:");

	if(pData->bit_mask & CDR3G_CAMELInformationSMS_serviceKey_present)
	{
		nLen += NumToDecStr((long)pData->serviceKey, 
			&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "defaultSMSHandling:");
	nLen += strlen("defaultSMSHandling:");

	if(pData->bit_mask & CDR3G_defaultSMSHandling_present)
	{
		ENUMINFO EnumInfo;
	    EnumInfo.nCount = DEFAULTSMSHANDLINGENUMINFO_LENGTH;
	    EnumInfo.pInfoTable = DEFAULTSMSHANDLINGENUMINFO;
	    nLen += EnumToStr(&EnumInfo, pData->defaultSMSHandling, &pTempBuffer[nLen], 
			         nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "cAMELCallingPartyNumber:");
	nLen += strlen("cAMELCallingPartyNumber:");

	if(pData->bit_mask & CDR3G_cAMELCallingPartyNumber_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pData->cAMELCallingPartyNumber, 
								&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

    strcpy(&pTempBuffer[nLen], "cAMELDestinationSubscriberNumber:");
	nLen += strlen("cAMELDestinationSubscriberNumber:");

	if(pData->bit_mask & CDR3G_cAMELDestinationSubscriberNumber_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pData->cAMELDestinationSubscriberNumber, 
								&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';

	strcpy(&pTempBuffer[nLen], "cAMELSMSCAddress:");
	nLen += strlen("cAMELSMSCAddress:");

    if(pData->bit_mask & CDR3G_cAMELSMSCAddress_present)
	{
		nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pData->cAMELSMSCAddress, &pTempBuffer[nLen], 
							nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = ' ';
       
   	strcpy(&pTempBuffer[nLen], "freeFormatData:");
	nLen += strlen("freeFormatData:");

	if(pData->bit_mask & CDR3G_CAMELInformationSMS_freeFormatData_present)
	{
		nLen += OctecToStr((LPOCTECSTRING)&pData->freeFormatData, 
			&pTempBuffer[nLen], nLength - nLen);
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	
	pTempBuffer[nLen] = 0;

	//if (nLen < nLength)
	//	memcpy(pBuffer, pTempBuffer, nLen + 1);
	//else
	//	nLen = 0;
	if(nLen >= nLength)
	{
		nLen = 0;
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
int R99_GCDRRecordExtensionsToStr(CDR3G_ManagementExtensions *pData, char *pBuffer, int nLength)
{
	int nLen = 0;
	
	if ((pData == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;	
	char *pTempBuffer = pBuffer;

	//recordExtensions目前只定义了19个域
	int nSets = (pData->count / 19);

	for (int i = 0; i < nSets; i++)
	{
		strcpy(&pTempBuffer[nLen], "{");
		nLen += 1;

		for (int j = 0; j < 19; j++)
		{
			int nIndex = i * 19 + j;
			switch (j)
			{
				//ServerIP
			    case 0:
				{
					if ((pData->value[nIndex].information.value == NULL) 
					 || (pData->value[nIndex].information.length != sizeof(unsigned long)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //modified by YanYong 2004-05-27
                        nLen += 1;
						return nLen;
					}

					strcpy(&pTempBuffer[nLen], "ServerIP:");
					nLen += strlen("ServerIP:");
					unsigned long nServerIP;
					memcpy(&nServerIP, pData->value[nIndex].information.value, sizeof(unsigned long));
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
					break;
				}
				
				//ServerPort
				case 1:
				{
					if ((pData->value[nIndex].information.value == NULL) 
					 || (pData->value[nIndex].information.length != sizeof(unsigned short)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //modified by YanYong 2004-05-27
                        nLen += 1;
						return nLen;
					}

					unsigned short nServerPort;
					memcpy(&nServerPort, pData->value[nIndex].information.value, sizeof(unsigned short));
					nServerPort = NTOHS(nServerPort);

					strcpy(&pTempBuffer[nLen], "ServerPort:");
					nLen += strlen("ServerPort:");
					nLen += NumToDecStr(nServerPort, &pTempBuffer[nLen], nLength - nLen, FALSE);
					pTempBuffer[nLen++] = ' ';
					break;
				}

				//ContentRecordFlag
				case 2:
				{
					if ((pData->value[nIndex].information.value == NULL) 
					 || (pData->value[nIndex].information.length != sizeof(unsigned char)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //modified by YanYong 2004-05-27
                        nLen += 1;
						return nLen;
					}

					strcpy(&pTempBuffer[nLen], "ContentRecordFlag:");
					nLen += strlen("ContentRecordFlag:");

					strcpy(&pTempBuffer[nLen], "Reserved");
					nLen += strlen("Reserved");
					
					/*
					unsigned char nContentRecordFlag;
					memcpy(&nContentRecordFlag, pData->value[nIndex].information.value, sizeof(unsigned char));
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
					break;
				}

				//TransferProtocol
				case 3:
				{
					if ((pData->value[nIndex].information.value == NULL) 
					 || (pData->value[nIndex].information.length != sizeof(unsigned short)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //modified by YanYong 2004-05-27
                        nLen += 1;
						return nLen;
					}

					strcpy(&pTempBuffer[nLen], "TransferProtocol:");
					nLen += strlen("TransferProtocol:");
					unsigned short nTransferProtocol;
					memcpy(&nTransferProtocol, pData->value[nIndex].information.value, sizeof(unsigned short));					
					nTransferProtocol = NTOHS(nTransferProtocol);

					//GGSN目前只支持TCP协议
					strcpy(&pTempBuffer[nLen], "TCP");
					nLen += strlen("TCP");
					pTempBuffer[nLen++] = ' ';
					break;
				}

				//MsIP
				case 4:
				{
					if ((pData->value[nIndex].information.value == NULL) 
					 || (pData->value[nIndex].information.length != sizeof(unsigned long)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //modified by YanYong 2004-05-27
                        nLen += 1;
						return nLen;
					}

					strcpy(&pTempBuffer[nLen], "MsIP:");
					nLen += strlen("MsIP:");
					unsigned long nMsIP;
					memcpy(&nMsIP, pData->value[nIndex].information.value, sizeof(unsigned long));
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
					break;
				}

				//MsPort
				case 5:
				{
					if ((pData->value[nIndex].information.value == NULL) 
					 || (pData->value[nIndex].information.length != sizeof(unsigned short)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //modified by YanYong 2004-05-27
                        nLen += 1;
						return nLen;
					}

					unsigned short nMsPort;
					memcpy(&nMsPort, pData->value[nIndex].information.value, sizeof(unsigned short));
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
					break;
				}

				//AppProtocol
				case 6:
				{
					if ((pData->value[nIndex].information.value == NULL) 
					 || (pData->value[nIndex].information.length != sizeof(unsigned short)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //modified by YanYong 2004-05-27
                        nLen += 1;
						return nLen;
					}

					unsigned short nAppProtocol;
					memcpy(&nAppProtocol, pData->value[nIndex].information.value, sizeof(unsigned short));
					nAppProtocol = NTOHS(nAppProtocol);

					strcpy(&pTempBuffer[nLen], "AppProtocol:");
					nLen += strlen("AppProtocol:");

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
					break;					
				}

				//UpFlow
				case 7:
				{
					if ((pData->value[nIndex].information.value == NULL) 
					 || (pData->value[nIndex].information.length != sizeof(unsigned long)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //modified by YanYong 2004-05-27
                        nLen += 1;
						return nLen;
					}

					unsigned long nUpFlow;
					memcpy(&nUpFlow, pData->value[nIndex].information.value, sizeof(unsigned long));
					nUpFlow = NTOHL(nUpFlow);

					strcpy(&pTempBuffer[nLen], "UpFlow:");
					nLen += strlen("UpFlow:");
					nLen += NumToDecStr(nUpFlow, &pTempBuffer[nLen], nLength - nLen, FALSE);
					pTempBuffer[nLen++] = ' ';
					break;					
				}

				//DownFlow
				case 8:
				{
					if ((pData->value[nIndex].information.value == NULL)
					 || (pData->value[nIndex].information.length != sizeof(unsigned long)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //modified by YanYong 2004-05-27
                        nLen += 1;
						return nLen;
					}

					unsigned long nDownFlow;
					memcpy(&nDownFlow, pData->value[nIndex].information.value, sizeof(unsigned long));
					nDownFlow = NTOHL(nDownFlow);

					strcpy(&pTempBuffer[nLen], "DownFlow:");
					nLen += strlen("DownFlow:");
					nLen += NumToDecStr(nDownFlow, &pTempBuffer[nLen], nLength - nLen, FALSE);
					pTempBuffer[nLen++] = ' ';
					break;					
				}

				//Duration
				case 9:
				{
					if ((pData->value[nIndex].information.value == NULL)
					 || (pData->value[nIndex].information.length != sizeof(unsigned long)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //modified by YanYong 2004-05-27
                        nLen += 1;
						return nLen;
					}

					unsigned long nDuration;
					memcpy(&nDuration, pData->value[nIndex].information.value, sizeof(unsigned long));
					nDuration = NTOHL(nDuration);

					strcpy(&pTempBuffer[nLen], "Duration:");
					nLen += strlen("Duration:");

					nLen += NumToDecStr(nDuration, &pTempBuffer[nLen], nLength - nLen, FALSE);
					/*
					strcpy(&pTempBuffer[nLen], "Reserved");
					nLen += strlen("Reserved");
					*/

					pTempBuffer[nLen++] = ' ';
					break;					
				}

				//URLString
				case 10:
				{
					strcpy(&pTempBuffer[nLen], "URL:");
					nLen += strlen("URL:");

					if ((pData->value[nIndex].information.value != NULL)
					 || (pData->value[nIndex].information.length != 0))
					{
						char tmpBlankStr[256];
						int ntmpStrLen = pData->value[nIndex].information.length;
						
						//在GGSN的定义此域为200字节，256其实没有必要
						if( ntmpStrLen < 256)
						{
							memcpy(tmpBlankStr, pData->value[nIndex].information.value, ntmpStrLen);
							tmpBlankStr[ntmpStrLen] = '\0';
						}
						else
						{
							memcpy(tmpBlankStr, pData->value[nIndex].information.value, 252);
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
						memcpy(&pTempBuffer[nLen], pData->value[nIndex].information.value, 
						pData->value[nIndex].information.length);						
						nLen += pData->value[nIndex].information.length;
						*/
					}
					else
					{
						strcpy(&pTempBuffer[nLen], "Empty Value");
						nLen += strlen("Empty Value");
					}


					pTempBuffer[nLen++] = ' ';
					break;					
				}

				//FlowRateInformation
				case 11:
				{
					if ((pData->value[nIndex].information.value == NULL) 
					 || (pData->value[nIndex].information.length != sizeof(unsigned short)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //modified by YanYong 2004-05-27
                        nLen += 1;
						return nLen;
					}

					unsigned short nFlowRateInfo;
					memcpy(&nFlowRateInfo, pData->value[nIndex].information.value, sizeof(unsigned short));
					nFlowRateInfo = NTOHS(nFlowRateInfo);

					strcpy(&pTempBuffer[nLen], "FlowRateInfo:");
					nLen += strlen("FlowRateInfo:");
					nLen += NumToDecStr(nFlowRateInfo, &pTempBuffer[nLen], nLength - nLen, FALSE);
					pTempBuffer[nLen++] = ' ';
					break;
				}

				//TimeRateInformation
				case 12:
				{
					if ((pData->value[nIndex].information.value == NULL) 
					 || (pData->value[nIndex].information.length != sizeof(unsigned short)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //modified by YanYong 2004-05-27
                        nLen += 1;
						return nLen;
					}

					unsigned short nTimeRateInfo;
					memcpy(&nTimeRateInfo, pData->value[nIndex].information.value, sizeof(unsigned short));
					nTimeRateInfo = NTOHS(nTimeRateInfo);

					strcpy(&pTempBuffer[nLen], "TimeRateInfo:");
					nLen += strlen("TimeRateInfo:");

					//nLen += NumToDecStr(nTimeRateInfo, &pTempBuffer[nLen], nLength - nLen, FALSE);
					strcpy(&pTempBuffer[nLen], "Reserved");
					nLen += strlen("Reserved");

					pTempBuffer[nLen++] = ' ';
					break;
				}

				//FlowBillInformation
				case 13:
				{
					if ((pData->value[nIndex].information.value == NULL) 
					 || (pData->value[nIndex].information.length != sizeof(unsigned long)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //modified by YanYong 2004-05-27
                        nLen += 1;
						return nLen;
					}

					unsigned long nFlowBillInfo;
					memcpy(&nFlowBillInfo, pData->value[nIndex].information.value, sizeof(unsigned long));
					nFlowBillInfo = NTOHL(nFlowBillInfo);

					strcpy(&pTempBuffer[nLen], "FlowBillInfo:");
					nLen += strlen("FlowBillInfo:");
					nLen += NumToDecStr(nFlowBillInfo, &pTempBuffer[nLen], nLength - nLen, FALSE);
					pTempBuffer[nLen++] = ' ';
					break;
				}

				//Time Bill Information
				case 14:
				{
					if ((pData->value[nIndex].information.value == NULL)
					 || (pData->value[nIndex].information.length != sizeof(unsigned long)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //modified by YanYong 2004-05-27
                        nLen += 1;
						return nLen;
					}

					unsigned long nTimeBillInfo;
					memcpy(&nTimeBillInfo, pData->value[nIndex].information.value, sizeof(unsigned long));
					nTimeBillInfo = NTOHL(nTimeBillInfo);

					strcpy(&pTempBuffer[nLen], "TimeBillInfo:");
					nLen += strlen("TimeBillInfo:");

					//nLen += NumToDecStr(nTimeBillInfo, &pTempBuffer[nLen], nLength - nLen, FALSE);
					strcpy(&pTempBuffer[nLen], "Reserved");
					nLen += strlen("Reserved");

					pTempBuffer[nLen++] = ' ';
					break;
				}

				//URLBillInformation
				case 15:
				{
					if ((pData->value[nIndex].information.value == NULL) 
					 || (pData->value[nIndex].information.length != sizeof(unsigned long)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //modified by YanYong 2004-05-27
                        nLen += 1;
						return nLen;
					}

					unsigned long nURLBillInfo;
					memcpy(&nURLBillInfo, pData->value[nIndex].information.value, sizeof(unsigned long));
					nURLBillInfo = NTOHL(nURLBillInfo);

					strcpy(&pTempBuffer[nLen], "URLBillInfo:");
					nLen += strlen("URLBillInfo:");
					nLen += NumToDecStr(nURLBillInfo, &pTempBuffer[nLen], nLength - nLen, FALSE);
					pTempBuffer[nLen++] = ' ';
					break;
				}

				//URLRateInformation
				case 16:
				{
					if ((pData->value[nIndex].information.value == NULL)
					 || (pData->value[nIndex].information.length != sizeof(unsigned short)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //modified by YanYong 2004-05-27
                        nLen += 1;
						return nLen;
					}

					unsigned short nURLRateInfo;
					memcpy(&nURLRateInfo, pData->value[nIndex].information.value, sizeof(unsigned short));
					nURLRateInfo = NTOHS(nURLRateInfo);

					strcpy(&pTempBuffer[nLen], "URLRateInfo:");
					nLen += strlen("URLRateInfo:");
					nLen += NumToDecStr(nURLRateInfo, &pTempBuffer[nLen], nLength - nLen, FALSE);
					pTempBuffer[nLen++] = ' ';
					break;
				}

				//ContentRecordClose
				case 17:
				{
					if ((pData->value[nIndex].information.value == NULL)
					 || (pData->value[nIndex].information.length != sizeof(unsigned char)))
					{
                        strcpy(&pTempBuffer[nLen], "}"); //modified by YanYong 2004-05-27
                        nLen += 1;
						return nLen;
					}

					strcpy(&pTempBuffer[nLen], "ContentRecordClose:");
					nLen += strlen("ContentRecordClose:");
					unsigned char nContentRecordClose;
					memcpy(&nContentRecordClose, pData->value[nIndex].information.value, sizeof(unsigned char));
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
					break;
				}

				//Reserve
				default:
				{
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
int CMCC_R99_GCDRRecordExtensionsToStr(CDR3G_ManagementExtensions *pData, char *pBuffer, int nLength)
{
	int nLen = 0;
	
	if ((pData == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;	
	char *pTempBuffer = pBuffer;

	//recordExtensions目前只定义了19个域
	int nSets = (pData->count / 19);

	for (int i = 0; i < nSets; i++)
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
			int nIndex = i * 19 + j;
			switch (j)
			{
				//ExetensionType
			    case 0:
				{
					if ((pData->value[nIndex].information.value == NULL) 
					 || (pData->value[nIndex].information.length != sizeof(unsigned char)))
					{
						return nLen;
					}
					unsigned char nExetensionType;
					memcpy(&nExetensionType, pData->value[nIndex].information.value, sizeof(unsigned char));					
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
					break;
				}
				
				//ServiceNum 
				case 1:
				{
					if ((pData->value[nIndex].information.value == NULL) 
					 || (pData->value[nIndex].information.length != sizeof(unsigned long)))
					{
						return nLen;
					}

					memcpy(&nServiceNum, pData->value[nIndex].information.value, sizeof(unsigned long));
					nServiceNum = NTOHL(nServiceNum);
					sprintf(szTemp, "ServiceNum:%u ", nServiceNum);
					memcpy(&pTempBuffer[nLen], szTemp, strlen(szTemp));
					nLen += strlen(szTemp);					
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
					    if ((pData->value[nIndex].information.value == NULL) 
					     || (pData->value[nIndex].information.length != sizeof(unsigned long)))
					    {
    						return nLen;
	    				}

					    memcpy(&nServiceID, pData->value[nIndex].information.value, sizeof(unsigned long));
					    nServiceID = NTOHL(nServiceID);					
					    sprintf(szTemp, "{ServiceID:%u ", nServiceID);
                        memcpy(&pTempBuffer[nLen], szTemp, strlen(szTemp));
                        nLen += strlen(szTemp);					
				    }
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
					    if ((pData->value[nIndex].information.value == NULL) 
					     || (pData->value[nIndex].information.length != sizeof(unsigned long)))
					    {
    						return nLen;
	    				}

					    memcpy(&nUpFlow, pData->value[nIndex].information.value, sizeof(unsigned long));
					    nUpFlow = NTOHL(nUpFlow);					
					    sprintf(szTemp, "UpFlow:%u ", nUpFlow);
                        memcpy(&pTempBuffer[nLen], szTemp, strlen(szTemp));
                        nLen += strlen(szTemp);					
				    }
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
						
					    if ((pData->value[nIndex].information.value == NULL) 
					     || (pData->value[nIndex].information.length != sizeof(unsigned long)))
					    {
    						return nLen;
	    				}

					    memcpy(&nDownFlow, pData->value[nIndex].information.value, sizeof(unsigned long));
					    nDownFlow = NTOHL(nDownFlow);					
					    sprintf(szTemp, "DownFlow:%u}", nDownFlow);
                        memcpy(&pTempBuffer[nLen], szTemp, strlen(szTemp));
                        nLen += strlen(szTemp);					
				    }
					break;
				}

				//Reserve
				default:
				{
					break;
				}
			}
		}

		strcpy(&pTempBuffer[nLen], "}");
		nLen += 1;
	}

	return nLen;	
}

