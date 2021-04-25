/******************************************************************************
Copyright (C), 2003-2011, Huawei Tech. Co., Ltd.

  /******************************************************************************
  File Name       : r4_cdr_browse.cpp
  Version         : 1.0
  Author          : Gan Zhiliang
  Created         : 2003-06-06
  Last Modified   :
  Description     : 该文件实现了3GR4话单浏览的主要功能
  Function List   :
  History         :
  1 Date          : 2003-06-06
  Author        : Gan Zhiliang
  Modification  : Created file
******************************************************************************/


#include "r4_cdr_browse.h"
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/

enum CBB_OPERATOR_ID g_nCBB_OI;


#define NETWORKTOMSDIRECTION 1
#define MSTONETWORKDIRECTION NULL

#define R4CHANGECONDITIONENUMINFO_LENGTH 3
static ENUMITEMINFO R4ChangeConditionEnumInfo [] =
{
    0, "QoS Change",
    1, "Tariff Time",
    2, "Record Closure"
};

#define R4CAUSEFORRECCLOSINGENUNINFO_LENGTH 13
static ENUMITEMINFO R4CauseForRecClosingEnumInfo [] =
{
    0, "Normal Release",
    4, "Abnormal Release",
    5, "CAMEL Init Call Release",
    16, "Volume Limit",
    17, "Time Limit",
    18, "SGSN Change",
    19, "maxChangeCond",
    20, "Management Intervention",
    21, "intraSGSN IntersystemChange", //R4协议多了此种类型
    52, "unauthorizedRequestingNetwork",
    53, "unauthorizedLCSClient",
    54, "positionMethodFailure",
    58,	"unknownOrUnreachableLCSClient"
};


#define APNSELECTIONMODEENUMINFO_LENGTH 3
static ENUMITEMINFO R4ApnSelectionModeEnumInfo [] =
{
    0, "MS or Network Provided Subscription Verified",
    1, "MS Provided Subscription Not Verified",
    2, "Network Provided Subscription Not Verified"
};

#define R4CONSOLIDATIONRESULTENUMINFO_LENGTH 5
static ENUMITEMINFO R4ConsolidationResultEnumInfo [] =
{
    0, "normal",
    1, "abnormal",
    2, "forInterSGSNConsolidation",
    3, "reachLimit",
    4, "onlyOneCDRGenerated"
};

//参照R99对QosInformation进行详细解释 2004/01/12
#define QOSR4RELIABILITYENUMINFO_LENGTH 8
static ENUMITEMINFO QosR4ReliabilityEnumInfo[] = 
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

#define QOSR4DELAYENUMINFO_LENGTH 8
static ENUMITEMINFO QoSR4DelayEnumInfo[] = 
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

#define QOSR4RECEDENCEENUMINFO_LENGTH 8
static ENUMITEMINFO QoSR4PrecedenceEnumInfo[] = 
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

#define QOSR4PEAKTHROUGHPUTENUMINFO_LENGTH 16
static ENUMITEMINFO QoSR4PeakThroughputEnumInfo[] = 
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

#define QOSR4MEANTHROUGHPUTENUMINFO_LENGTH 32
static ENUMITEMINFO QoSR4MeanThroughputEnumInfo[] = 
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

#define QOSR4DELIVERYOFERRONEOUSSDUSENUMINFO_LENGTH 5
static ENUMITEMINFO QoSR4DeliveryOfErroneousSDUsEnumInfo[] = 
{
	0,	"Subscribed", //"Subscribed delivery of erroneous SDUs",
	1,	"-", //"No detect ('-')",
	2,	"Yes", //"Erroneous SDUs are delivered ('yes')",
	3, 	"No", //"Erroneous SDUs are not delivered ('no')",
	7,	"Reserved" //"Reserved"
};

#define QOSR4DELIVERYORDERENUMINFO_LENGTH 4
static ENUMITEMINFO QoSR4DeliveryOrderEnumInfo[] = 
{
	0,	"Subscribed", //"Subscribed delivery order",
	1,	"Yes", //"With delivery order ('yes')",
	2,	"No", //"Without delivery order ('no')",
	3,	"Reserved" //"Reserved"
};

#define QOSR4TRAFFICCLASSENUMINFO_LENGTH 6
static ENUMITEMINFO QoSR4TrafficClassEnumInfo[] = 
{
	0,	"Subscribed", //"Subscribed traffic class",
	1,	"Conversational", //"Conversational class",
	2,	"Stream", //"Streaming class",
	3,	"Interactive", //"Interactive class",
	4,	"Background", //"Background class",
	7,	"Reserved" //"Reserved"
};

#define QOSR4MAXIMUMSDUSIZEENUMINFO_LENGTH 5
static ENUMITEMINFO QoSR4MaximumSDUSizeEnumInfo[] = 
{
	0,	 "Subscribed", //"Subscribed maximum SDU size",
	//1 - 150 为 "10 octets"到"1500 octets"，在程序中写
	151, "1502octets", //"1502 octets",
	152, "1510octets", //"1510 octets", 
	153, "1520octets", //"1520 octets",
	255, "Reserved" //"Reserved"
};

#define QOSR4MAXIMUMBITRATEFORUPLINKENUMINFO_LENGTH 2
static ENUMITEMINFO QoSR4MaximumBitRateForUplinkEnumInfo[] = 
{
	0, "Subscribed", //"Subscribed maximum bit rate for uplink",
	//1 - 63 为  1 kbps 到 63 kbps，在程序中写
	//64 - 127 为  64 kbps 到 568 kbps，在程序中写
	//128 - 254  为 576 kbps 到 8640 kbps，在程序中写
	255, "0kbps"
};

#define QOSR4MAXIMUMBITRATEFORDOWNLINKENUMINFO_LENGTH 2
static ENUMITEMINFO QoSR4MaximumBitRateForDownlinkEnumInfo[] = 
{
	0, "Subscribed", //"Subscribed maximum bit rate for downlink",
	//1 - 63 为  1 kbps 到 63 kbps，在程序中写
	//64 - 127 为  64 kbps 到 568 kbps，在程序中写
	//128 - 254  为 576 kbps 到 8640 kbps，在程序中写
	255, "0kbps"
};

#define QOSR4RESIDUALBERENUMINFO_LENGTH 11
static ENUMITEMINFO QoSR4ResidualBEREnumInfo[] = 
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

#define QOSR4SDUERRORRATIOENUMINFO_LENGTH 9
static ENUMITEMINFO QoSR4SDUErrorRatioEnumInfo[] = 
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

#define QOSR4TRAFFICHANDLINGPRIORITYENUMINFO_LENGTH 4  //2改为4  ZhengYuqun 2003-09-10  SWPD01505
static ENUMITEMINFO QoSR4TrafficHandlingPriorityEnumInfo[] = 
{
	0, "Subscribed", //"Subscribed traffic handling priority",
	1, "1", //"Priority level 1",
	2, "2", //"Priority level 2",
	3, "3" //"Priority level 3"
};

#define QOSR4TRANSFERDELAYENUMINFO_LENGTH 2
static ENUMITEMINFO QoSR4TransferDelayEnumInfo[] = 
{
	0,  "Subscribed", //"Subscribed transfer delay",
	//1 - 15  为 10 ms 到 150 ms，程序中写
	//16 - 31 为 200 ms 到 950 ms，程序中写
	//32 - 62 为 1000 ms 到 4100 ms，程序中写
	63, "Reserved"
};

#define QOSR4GUARANTEEDBITRATEFORUPLINKENUMINFO_LENGTH 2
static ENUMITEMINFO QoSR4GuaranteedBitRateForUplinkEnumInfo[] = 
{
	0, "Subscribed", //"Subscribed maximum bit rate for uplink",
	//1 - 63 为  1 kbps 到 63 kbps，在程序中写
	//64 - 127 为  64 kbps 到 568 kbps，在程序中写
	//128 - 254  为 576 kbps 到 8640 kbps，在程序中写
	255, "0kbps"
};

#define QOSR4GUARANTEEDBITRATEFORDOWNLINKENUMINFO_LENGTH 2
static ENUMITEMINFO QoSR4GuaranteedBitRateForDownlinkEnumInfo[] = 
{
	0, "Subscribed", //"Subscribed maximum bit rate for uplink",
	//1 - 63 为  1 kbps 到 63 kbps，在程序中写
	//64 - 127 为  64 kbps 到 568 kbps，在程序中写
	//128 - 254  为 576 kbps 到 8640 kbps，在程序中写
	255, "0kbps"
};
//End Of Adding

#define R4SYSTEMTYPEENUMINFO_LENGTH 3
static ENUMITEMINFO R4SYSTEMTYPEENUMINFOEnumInfo[] =
{
    0, "unknown",
    1, "iuUTRAN",
    2, "gERAN"
};



#define R4DEFAULTTRANSACTIONHANDLINGENUMINFO_LENGTH 2
static ENUMITEMINFO R4DEFAULTTRANSACTIONHANDLINGENUMINFO[] =
{
    0, "DefaultGPRS Handling continueTransaction",
    1, "DefaultGPRS Handling releaseTransaction"
};

#define R4LEVELOFCAMELSERVICE_LENGTH 3
static ENUMITEMINFO R4LEVELOFCAMELSERVICE[] =
{
    0x80, "basic",
    0x40, "callDurationSupervision",
    0x20, "onlineCharging"
};

#define R4DEFAULTSMSHANDLINGENUMINFO_LENGTH 2
static ENUMITEMINFO R4DEFAULTSMSHANDLINGENUMINFO[] =
{
    0, "DefaultSMS Handling continueTransaction",
    1, "DefaultSMS Handling releaseTransaction"
};

//by ldf 2004-02-12 对应问题单SWPD03517,这里枚举长度由5改为6
#define DEFAULTR4chChSELECTIONMODEINFOEnumInfo_LENGTH 6
static ENUMITEMINFO R4chChSELECTIONMODEINFOEnumInfo[] =
{
    0, "sGSNSupplied",
    1, "subscriptionSpecific",
    2, "aPNSpecific",
    3, "homeDefault",
    4, "roamingDefault",
    5, "visitingDefault"
};

#define DEFAULTR4OrgLCSClientTypeEnumInfo_LENGTH 4
static ENUMITEMINFO R4OrgLCSClientTypeEnumInfo[] =
{
    0, "emergencyServices",
    1, "valueAddedServices",
    2, "plmnOperatorServices",
    3, "lawfulInterceptServices"
};

#define DEFAULTR4_Org_LCSClientInternalIDEnumInfo_LENGTH 5
static ENUMITEMINFO R4_Org_LCSClientInternalIDEnumInfo[] =
{
    0, "broadcastService",
    1, "M_HPLMN",
    2, "M_VPLMN",
    3, "anonymousLocation",
    4, "targetMSsubscribedService"
};

#define DEFAULTR4_Org_LocationEstimateTypeEnumInfo_LENGTH 5
static ENUMITEMINFO R4_Org_LocationEstimateTypeEnumInfo[] =
{
    0, "currentLocation",
    1, "currentOrLastKnownLocation",
    2, "initialLocation",
    3, "activateDeferredLocation",
    4, "cancelDeferredLocation"
};

#define DEFAULTR4_Org_ResponseTimeCategoryEnumInfo_LENGTH 2
static ENUMITEMINFO R4_Org_ResponseTimeCategoryEnumInfo[] =
{
    0, "lowdelay",
    1, "delaytolerant"
};

#define DEFAULTR4_Org_NotificationToMSUserEnumInfo_LENGTH 4
static ENUMITEMINFO R4_Org_NotificationToMSUserEnumInfo[] =
{
    0, "notifyLocationAllowed",
    1, "notifyAndVerify_LocationAllowedIfNoResponse",
    2, "notifyAndVerify_LocationNotAllowedIfNoResponse",
    3, "locationNotAllowed"
};

#define DEFAULTR4_Org_LocationMethodEnumInfo_LENGTH 5
static ENUMITEMINFO R4_Org_LocationMethodEnumInfo[] =
{
    0, "msBasedEOTD",
    1, "msAssistedEOTD",
    2, "assistedGPS",
    3, "msBasedOTDOA",
    4, "msAssistedOTDOA"
};

#define DEFAULTR4_Org_PositionMethodFailureCauseEnumInfo_LENGTH 9
static ENUMITEMINFO R4_Org_PositionMethodFailureCauseEnumInfo[] =
{
     0, "congestion",
     1, "insufficientResources",
     2, "insufficientMeasurementData",
     3, "inconsistentMeasurementData",
     4, "locationProcedureNotCompleted",
     5, "locationProcedureNotSupportedByTargetMS",
     6, "qoSNotAttainable",
     7, "positionMethodNotAvailableInNetwork",
     8, "positionMethodNotAvailableInLocationArea"
};

#define DEFAULTR4_Org_UnauthorizedLCSClientCauseEnumInfo_LENGTH 5
static ENUMITEMINFO R4_Org_UnauthorizedLCSClientCauseEnumInfo[] =
{
     0, "noAdditionalInformation",
     1, "clientNotInMSPrivacyExceptionList",
     2, "callToClientNotSetup",
     3, "privacyOverrideNotApplicable",
     4, "disallowedByLocalRegulatoryRequirements"
};

#define R4_440_RECORDE_TYPE_ENUNINFO_LENGTH 8
static ENUMITEMINFO R4_440_Recorde_Type_EnumInfo[] =
{
    18,	S_CDR_DESC,
    19,	G_CDR_DESC,
    20,	M_CDR_DESC,
    21,	S_SMO_CDR_DESC,
    22,	S_SMT_CDR_DESC,
    26, S_LCS_MT_CDR_DESC,
    27, S_LCO_MT_CDR_DESC,
    28, S_LCN_MT_CDR_DESC,
};

/*******************************************************************************
Function        : GetAllFieldTextValueOf3GR4CDR
Description     : 3G话单浏览处理的入口函数
Input           : char *pCDR  输入话单的数据指针
: int nFormatType  格式名字符串
: char *pBuffer  放结果字符串的缓冲区
: int nLength  存放结果字符串的缓冲区的有效长度
Output          :
Return          : int 转换后的字符串的长度
Calls           :
Called by       :

  History         :
  1 Date          : 2003-06-06
  Author        : Gan Zhiliang
  Modification  : Created file
*******************************************************************************/
int GetAllFieldTextValueOf3GR4CDR(char *pCDR, int nFormatType, char *pBuffer, int nLength)
{
    int nLen = 0;
    switch (nFormatType)
    {
    case S_CDR_FORMATTYPE:
        {
            nLen = GetTextValueOf3GR4SGSNPDP(pCDR, pBuffer, nLength);
        }
        break;
    case G_CDR_FORMATTYPE:
        {
            nLen = GetTextValueOf3GR4GGSNPDP(pCDR, pBuffer, nLength);
        }
        break;
    case M_CDR_FORMATTYPE:
        {
            nLen = GetTextValueOf3GR4SGSNMM(pCDR, pBuffer, nLength);
        }
        break;
    case S_SMO_CDR_FORMATTYPE:
        {
            nLen = GetTextValueOf3GR4SGSNSMO(pCDR, pBuffer, nLength);
        }
        break;
    case S_SMT_CDR_FORMATTYPE:
        {
            nLen = GetTextValueOf3GR4SGSNSMT(pCDR, pBuffer, nLength);
        }
        break;
    case L_S_CDR_FORMATTYPE:
        {
            nLen = GetTextValueOf3GR4LARGESGSNPDP(pCDR, pBuffer, nLength);
        }
        break;
    case L_G_CDR_FORMATTYPE:
        {
            nLen = GetTextValueOf3GR4LARGEGGSNPDP(pCDR, pBuffer, nLength);
        }
        break;
    case LCS_MT_CDR:
        {
            nLen = GetTextValueOf3GR4SGSNLCT(pCDR, pBuffer, nLength);
        }
        break;
    case LCS_MO_CDR:
        {
            nLen = GetTextValueOf3GR4SGSNLCO(pCDR, pBuffer, nLength);
        }
        break;
    case LCS_NI_CDR:
        {
            nLen = GetTextValueOf3GR4SGSNLCN(pCDR, pBuffer, nLength);
        }
        break;

    default:
        pBuffer[0] = 0;
    }
    return nLen;
}

/*******************************************************************************
Function        : GetQueryFieldTextValueOf3GR4CDR
Description     : 获取3GR4话单查询条件域值入口函数
Input           : char *pCDR 输入话单的数据指针
: int nFormatType  格式名字符串
: char *pBuffer  放结果字符串的缓冲区
: int nLength  存放结果字符串的缓冲区的有效长度
Output          :
Return          : int 转换后的字符串的长度
Calls           :
Called by       :

  History         :
  1 Date          : 2003-06-06
  Author        : Gan Zhiliang
  Modification  : Created file
*******************************************************************************/
int GetQueryFieldTextValueOf3GR4CDR(char *pCDR,
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
            nLen = GetTextValueOf_Q_3GR4SGSNPDP(pCDR, pBuffer, nLength, pValues);
        }
        break;
    case G_CDR_FORMATTYPE:
        {
            nLen = GetTextValueOf_Q_3GR4GGSNPDP(pCDR, pBuffer, nLength, pValues);
        }
        break;
    case M_CDR_FORMATTYPE:
        {
            nLen = GetTextValueOf_Q_3GR4SGSNMM(pCDR, pBuffer, nLength, pValues);
        }
        break;
    case S_SMO_CDR_FORMATTYPE:
        {
            nLen = GetTextValueOf_Q_3GR4SGSNSMO(pCDR, pBuffer, nLength, pValues);
        }
        break;
    case S_SMT_CDR_FORMATTYPE:
        {
            nLen = GetTextValueOf_Q_3GR4SGSNSMT(pCDR, pBuffer, nLength, pValues);
        }
        break;
    case L_S_CDR_FORMATTYPE:
        {
            nLen = GetTextValueOf_Q_3GR4LARGESGSNPDP(pCDR, pBuffer, nLength, pValues);
        }
        break;
    case L_G_CDR_FORMATTYPE:
        {
            nLen = GetTextValueOf_Q_3GR4LARGEGGSNPDP(pCDR, pBuffer, nLength, pValues);
        }
        break;
    case LCS_MT_CDR:
        {
            nLen = GetTextValueOf_Q_3GR4SGSNLCT(pCDR, pBuffer, nLength, pValues);
        }
        break;
    case LCS_MO_CDR:
        {
            nLen = GetTextValueOf_Q_3GR4SGSNLCO(pCDR, pBuffer, nLength, pValues);
        }
        break;
    case LCS_NI_CDR:
        {
            nLen = GetTextValueOf_Q_3GR4SGSNLCN(pCDR, pBuffer, nLength, pValues);
        }
        break;
    default:
        pBuffer[0] = 0;
    }
    return nLen;
}

/*******************************************************************************
Function        : GetTextValueOf3GR4SGSNPDP
Description     : 处理S-CDR话单浏览解释的函数
Input           : char *pValue 话单数据
: char *pBuffer 存放结果的数据区
: int nLength 数据区的有效长度
Output          :
Return          : int 结果字符串的长度
Calls           :
Called by       :

  History         :
  1 Date          : 2003-06-06
  Author        : Gan Zhiliang
  Modification  : Created file
*******************************************************************************/
int GetTextValueOf3GR4SGSNPDP(char *pValue, char *pBuffer, int nLength)
{

    R4_Org_SGSNPDPRecord  *pBill = (R4_Org_SGSNPDPRecord*)pValue;
    int  nLen = 0;
    int nTempLen = 0;

    //ASSERT((BOOL)(pValue && pBuffer));
    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Org_CallEventRecordType_sgsnPDPRecord)
        return 0;

    char *pTempBuffer = pBuffer;

    if (!pTempBuffer)
        return 0;

    //recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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
    if (pBill->bit_mask & R4_Org_SGSNPDPRecord_networkInitiation_present)
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
    if (pBill->bit_mask & R4_Org_SGSNPDPRecord_servedIMEI_present)
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
    if(pBill->bit_mask & R4_Org_SGSNPDPRecord_sgsnAddress_present)
    {
        nLen += R43GIPAddrToStr(&pBill->sgsnAddress, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //msNetworkCapability
    if (pBill->bit_mask & R4_Org_SGSNPDPRecord_msNetworkCapability_present)
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
    if (pBill->bit_mask & R4_Org_SGSNPDPRecord_routingArea_present)
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
    if (pBill->bit_mask & R4_Org_SGSNPDPRecord_locationAreaCode_present)
    {
        nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationAreaCode, &pTempBuffer[nLen],
            nLength - nLen, "0x", FALSE);

    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //cellIdentifier
    if (pBill->bit_mask & R4_Org_SGSNPDPRecord_cellIdentifier_present)
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
    nLen += R43GIPAddrToStr(&pBill->ggsnAddressUsed, &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //accessPointNameNI
    if(pBill->bit_mask & R4_Org_SGSNPDPRecord_accessPointNameNI_present)
    {

        strcpy(&pTempBuffer[nLen], pBill->accessPointNameNI);
        nLen += strlen(pBill->accessPointNameNI);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //pdpType
    if(pBill->bit_mask & R4_Org_SGSNPDPRecord_pdpType_present)
    {
        nLen += HexOctecToStr((LPOCTECSTRING)&pBill->pdpType, &pTempBuffer[nLen],
            nLength - nLen, "0x", FALSE);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //servedPDPAddress
    if (pBill->bit_mask & R4_Org_SGSNPDPRecord_servedPDPAddress_present)
    {
        nLen += R43GPDPAddrToStr(&pBill->servedPDPAddress, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    if(pBill->bit_mask & R4_Org_SGSNPDPRecord_listOfTrafficVolumes_present)
    {

        R4_Org_NetworkInitiatedPDPContext networkInitiation = pBill->bit_mask & R4_Org_SGSNPDPRecord_networkInitiation_present ? pBill->networkInitiation : NULL;
        nLen += R4ListOfTrafficVolumesToStr(pBill->listOfTrafficVolumes, &pTempBuffer[nLen], nLength - nLen, networkInitiation);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //recordOpeningTime
    nLen += TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime, &pTempBuffer[nLen],
        nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //duration
    nLen += NumToDecStr(pBill->duration, &pTempBuffer[nLen], nLength - nLen, FALSE);
    pTempBuffer[nLen++] = '^';

    //sgsnChange
    if (pBill->bit_mask & R4_Org_SGSNPDPRecord_sgsnChange_present)
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
    EnumInfo.nCount = R4CAUSEFORRECCLOSINGENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R4CauseForRecClosingEnumInfo;
    nTempLen = EnumToStr(&EnumInfo, pBill->causeForRecClosing,&pTempBuffer[nLen], nLength - nLen);

    if (nTempLen)
        nLen += nTempLen;
    else
    {
        strcpy(&pTempBuffer[nLen], "Unknown reason");
        nLen += strlen("Unknown reason");
    }
    pTempBuffer[nLen++] = '^';

    //diagnostics
    if (pBill->bit_mask & R4_Org_SGSNPDPRecord_diagnostics_present)
    {
        nLen += R4DiagnosticsToStr(&pBill->diagnostics, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //recordSequenceNumber
    if (pBill->bit_mask & R4_Org_SGSNPDPRecord_recordSequenceNumber_present)
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
    if (pBill->bit_mask & R4_Org_SGSNPDPRecord_nodeID_present)
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
    if (pBill->bit_mask & R4_Org_SGSNPDPRecord_recordExtensions_present)
    {
        nLen += R4RecordExtensionsToStr(pBill->recordExtensions, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //localSequenceNumber
    if (pBill->bit_mask & R4_Org_SGSNPDPRecord_localSequenceNumber_present)
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
    if (pBill->bit_mask & R4_Org_SGSNPDPRecord_apnSelectionMode_present)
    {
        EnumInfo.nCount = APNSELECTIONMODEENUMINFO_LENGTH;
        EnumInfo.pInfoTable = R4ApnSelectionModeEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->apnSelectionMode, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //accessPointNameOI
    if(pBill->bit_mask & R4_Org_accessPointNameOI_present)
    {
        strcpy(&pTempBuffer[nLen], pBill->accessPointNameOI);
        nLen += strlen(pBill->accessPointNameOI);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //servedMSISDN
    if (pBill->bit_mask & R4_Org_SGSNPDPRecord_servedMSISDN_present)
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
    nLen += R4ChargingCharacteristicsToStr(&pBill->chargingCharacteristics, &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';


    //systemType
    if (pBill->bit_mask & R4_Org_SGSNPDPRecord_systemType_present)
    {
        EnumInfo.nCount = R4SYSTEMTYPEENUMINFO_LENGTH;
        EnumInfo.pInfoTable = R4SYSTEMTYPEENUMINFOEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->systemType, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //cAMELInfomationPDP
    if(pBill->bit_mask & R4_Org_cAMELInformationPDP_present)
    {
        nLen += R43GSGSNPDP_CAMELInformationToStr(&pBill->cAMELInformationPDP, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //rNCUnsentDownlinkVolume
    if (pBill->bit_mask & R4_Org_rNCUnsentDownlinkVolume_present)
    {
        nLen += NumToDecStr(pBill->rNCUnsentDownlinkVolume, &pTempBuffer[nLen], nLength - nLen, FALSE);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //chChSelectionMode
    if (pBill->bit_mask & R4_Org_SGSNPDPRecord_chChSelectionMode_present)
    {
        EnumInfo.nCount = DEFAULTR4chChSELECTIONMODEINFOEnumInfo_LENGTH;
        EnumInfo.pInfoTable = R4chChSELECTIONMODEINFOEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->chChSelectionMode, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //dynamicAddressFlag
    if (pBill->bit_mask & R4_Org_SGSNPDPRecord_dynamicAddressFlag_present)
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

    pTempBuffer[nLen] = 0;


    if (nLen >= nLength)
        nLen = 0;

    return nLen;
}

/*******************************************************************************
Function        : GetTextValueOf3GR4GGSNPDP
Description     : 处理G-CDR话单浏览解释的函数
Input           : char *pValue 话单数据
: char *pBuffer 存放结果的数据区
: int nLength  数据区的有效长度
Output          :
Return          : int  结果字符串的长度
Calls           :
Called by       :

History         :
1 Date          : 2003-06-06
Author        : Gan Zhiliang
Modification  : Created file
*******************************************************************************/
int GetTextValueOf3GR4GGSNPDP(char *pValue, char *pBuffer, int nLength)
{

    R4_Org_GGSNPDPRecord *pBill = (R4_Org_GGSNPDPRecord*)pValue;
    int nLen = 0;
    int nTempLen = 0;

    //ASSERT((BOOL)(pValue && pBuffer));
    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Org_CallEventRecordType_ggsnPDPRecord)
        return 0;

    char *pTempBuffer = pBuffer;

    if (!pTempBuffer)
        return 0;

    //recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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
    if (pBill->bit_mask & R4_Org_GGSNPDPRecord_networkInitiation_present)
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
    nLen += R43GIPAddrToStr(&pBill->ggsnAddress, &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //chargingID
    nLen += NumToDecStr(pBill->chargingID, &pTempBuffer[nLen],
        nLength - nLen, FALSE);
    pTempBuffer[nLen++] = '^';

    //sgsnAddress
    for (; pBill->sgsnAddress != NULL; pBill->sgsnAddress = pBill->sgsnAddress->next)
    {
        pTempBuffer[nLen++] = '{';
        nLen += R43GIPAddrToStr(&pBill->sgsnAddress->value, &pTempBuffer[nLen],
            nLength - nLen);
        pTempBuffer[nLen++] = '}';
    }

    pTempBuffer[nLen++] = '^';

    //accessPointNameNI
    if(pBill->bit_mask & R4_Org_GGSNPDPRecord_accessPointNameNI_present)
    {

        strcpy(&pTempBuffer[nLen], pBill->accessPointNameNI);
        nLen += strlen(pBill->accessPointNameNI);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //pdpType
    if(pBill->bit_mask & R4_Org_GGSNPDPRecord_pdpType_present)
    {
        nLen += HexOctecToStr((LPOCTECSTRING)&pBill->pdpType, &pTempBuffer[nLen],
            nLength - nLen, "0x", FALSE);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //servedPDPAddress
    if (pBill->bit_mask & R4_Org_GGSNPDPRecord_servedPDPAddress_present)
    {
        nLen += R43GPDPAddrToStr(&pBill->servedPDPAddress, &pTempBuffer[nLen],
            nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //dynamicAddressFlag
    if (pBill->bit_mask & R4_Org_GGSNPDPRecord_dynamicAddressFlag_present)
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

    //listOfTrafficVolumes
    if(pBill->bit_mask & R4_Org_GGSNPDPRecord_listOfTrafficVolumes_present)
    {
        R4_Org_NetworkInitiatedPDPContext networkInitiation = pBill->bit_mask & R4_Org_GGSNPDPRecord_networkInitiation_present ? pBill->networkInitiation : NULL;
        nLen += R4ListOfTrafficVolumesToStr(pBill->listOfTrafficVolumes, &pTempBuffer[nLen],
            nLength - nLen, networkInitiation);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //recordOpeningTime
    nLen += TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime, &pTempBuffer[nLen],
        nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //duration
    nLen += NumToDecStr(pBill->duration, &pTempBuffer[nLen],
        nLength - nLen, FALSE);
    pTempBuffer[nLen++] = '^';

    //causeForRecClosing
    EnumInfo.nCount = R4CAUSEFORRECCLOSINGENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R4CauseForRecClosingEnumInfo;
    nTempLen = EnumToStr(&EnumInfo, pBill->causeForRecClosing,&pTempBuffer[nLen],
        nLength - nLen);
    if (nTempLen)
        nLen += nTempLen;
    else
    {
        strcpy(&pTempBuffer[nLen], "Unknown reason");
        nLen += strlen("Unknown reason");
    }
    pTempBuffer[nLen++] = '^';

    //diagnostics
    if (pBill->bit_mask & R4_Org_GGSNPDPRecord_diagnostics_present)
    {
        nLen += R4DiagnosticsToStr(&pBill->diagnostics, &pTempBuffer[nLen],
            nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //recordSequenceNumber
    if (pBill->bit_mask & R4_Org_GGSNPDPRecord_recordSequenceNumber_present)
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
    if (pBill->bit_mask & R4_Org_GGSNPDPRecord_nodeID_present)
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
    if (pBill->bit_mask & R4_Org_GGSNPDPRecord_recordExtensions_present)
    {
        switch (g_nCBB_OI)
        {			    
        case CBB_OPERATOR_ID_CMCC:
            nLen += CMCC_R4_GCDRRecordExtensionsToStr(pBill->recordExtensions, 
                         &pTempBuffer[nLen], nLength - nLen);
            break;
            
        default:
            nLen += R4_GCDRRecordExtensionsToStr(pBill->recordExtensions, 
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

    //localSequenceNumber
    if (pBill->bit_mask & R4_Org_GGSNPDPRecord_localSequenceNumber_present)
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
    if (pBill->bit_mask & R4_Org_GGSNPDPRecord_apnSelectionMode_present)
    {
        EnumInfo.nCount = APNSELECTIONMODEENUMINFO_LENGTH;
        EnumInfo.pInfoTable = R4ApnSelectionModeEnumInfo;
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
    if (pBill->bit_mask & R4_Org_GGSNPDPRecord_servedMSISDN_present)
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
    nLen += R4ChargingCharacteristicsToStr(&pBill->chargingCharacteristics,
        &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //chChSelectionMode
    if (pBill->bit_mask & R4_Org_GGSNPDPRecord_chChSelectionMode_present)
    {
        EnumInfo.nCount = DEFAULTR4chChSELECTIONMODEINFOEnumInfo_LENGTH;
        EnumInfo.pInfoTable = R4chChSELECTIONMODEINFOEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->chChSelectionMode, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //added by ggf, 2003-11-20, sgsnPLMNIdentifier
    if (pBill->bit_mask & R4_Org_sgsnPLMNIdentifier_present)
    {
        //by ldf 2004-03-26 对应问题单SWPD03933
        nLen += CvtBCDOctecToStr2((LPOCTECSTRING)&pBill->sgsnPLMNIdentifier, &pTempBuffer[nLen], nLength - nLen);
        //end
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';
	//end.

    pTempBuffer[nLen] = 0;


    if (nLen >= nLength)
        nLen = 0;

    return nLen;
}

/*******************************************************************************
Function        : GetTextValueOf3GR4SGSNMM
Description     : 处理M-CDR话单浏览解释的函数
Input           : char *pValue话单数据
: char *pBuffer存放结果的数据区
: int nLength数据区的有效长度
Output          :
Return          : int结果字符串的长度
Calls           :
Called by       :

History         :
1 Date          : 2003-06-06
Author        : Gan Zhiliang
Modification  : Created file
*******************************************************************************/
int GetTextValueOf3GR4SGSNMM(char *pValue, char *pBuffer, int nLength)
{

    R4_Org_SGSNMMRecord *pBill = (R4_Org_SGSNMMRecord*)pValue;
    int nLen = 0;
    int nTempLen = 0;

    //ASSERT((BOOL)(pValue && pBuffer));
    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Org_CallEventRecordType_sgsnMMRecord)
        return 0;

    char *pTempBuffer = pBuffer;

    if (!pTempBuffer)
        return 0;

    //recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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
    if (pBill->bit_mask & R4_Org_SGSNMMRecord_servedIMEI_present)
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
    if(pBill->bit_mask & R4_Org_SGSNMMRecord_sgsnAddress_present)
    {
        nLen += R43GIPAddrToStr(&pBill->sgsnAddress, &pTempBuffer[nLen],
            nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //msNetworkCapability
    if (pBill->bit_mask & R4_Org_SGSNMMRecord_msNetworkCapability_present)
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
    if (pBill->bit_mask & R4_Org_SGSNMMRecord_routingArea_present)
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
    if (pBill->bit_mask & R4_Org_SGSNMMRecord_locationAreaCode_present)
    {
        nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationAreaCode, &pTempBuffer[nLen],
            nLength - nLen, "0x", FALSE);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //cellIdentifier
    if (pBill->bit_mask & R4_Org_SGSNMMRecord_cellIdentifier_present)
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

    //changeLocation
    if (pBill->bit_mask & R4_Org_changeLocation_present)
    {
        nLen += R4ChangeLocationsToStr((R4_Org_SGSNMMRecord::R4_Org__seqof3 *)pBill->changeLocation,
            &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //recordOpeningTime
    nLen += TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime, &pTempBuffer[nLen],
        nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //duration
    if (pBill->bit_mask & R4_Org_duration_present)
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
    if (pBill->bit_mask & R4_Org_SGSNMMRecord_sgsnChange_present)
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
    EnumInfo.nCount = R4CAUSEFORRECCLOSINGENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R4CauseForRecClosingEnumInfo;
    nTempLen = EnumToStr(&EnumInfo, pBill->causeForRecClosing,
        &pTempBuffer[nLen], nLength - nLen);
    if (nTempLen)
        nLen += nTempLen;
    else
    {
        strcpy(&pTempBuffer[nLen], "Unknown reason");
        nLen += strlen("Unknown reason");
    }
    pTempBuffer[nLen++] = '^';

    //diagnostics
    if (pBill->bit_mask & R4_Org_SGSNMMRecord_diagnostics_present)
    {
        nLen += R4DiagnosticsToStr(&pBill->diagnostics, &pTempBuffer[nLen],
            nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //recordSequenceNumber
    if (pBill->bit_mask & R4_Org_SGSNMMRecord_recordSequenceNumber_present)
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
    if (pBill->bit_mask & R4_Org_SGSNMMRecord_nodeID_present)
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
    if (pBill->bit_mask & R4_Org_SGSNMMRecord_recordExtensions_present)
    {
        nLen += R4RecordExtensionsToStr(pBill->recordExtensions, &pTempBuffer[nLen],
            nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //localSequenceNumber
    if (pBill->bit_mask & R4_Org_SGSNMMRecord_localSequenceNumber_present)
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
    if (pBill->bit_mask & R4_Org_SGSNMMRecord_servedMSISDN_present)
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
    nLen += R4ChargingCharacteristicsToStr(&pBill->chargingCharacteristics,
        &pTempBuffer[nLen], nLength - nLen);

    pTempBuffer[nLen++] = '^';

    //cAMELInfomationMM
    if(pBill->bit_mask & R4_Org_cAMELInformationMM_present)
    {
        nLen += R43GSGSNMM_CAMELInformationToStr(&pBill->cAMELInformationMM, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //systemType
    if (pBill->bit_mask & R4_Org_SGSNMMRecord_systemType_present)
    {
        EnumInfo.nCount = R4SYSTEMTYPEENUMINFO_LENGTH;
        EnumInfo.pInfoTable = R4SYSTEMTYPEENUMINFOEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->systemType, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //chChSelectionMode
    if (pBill->bit_mask & R4_Org_SGSNMMRecord_chChSelectionMode_present)
    {
        EnumInfo.nCount = DEFAULTR4chChSELECTIONMODEINFOEnumInfo_LENGTH;
        EnumInfo.pInfoTable = R4chChSELECTIONMODEINFOEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->chChSelectionMode, &pTempBuffer[nLen], nLength - nLen);
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

/*******************************************************************************
Function        : GetTextValueOf3GR4SGSNSMO
Description     : 处理S-SMO-CDR话单浏览解释的函数
Input           : char *pValue 话单数据
: char *pBuffer 存放结果的数据区
: int nLength 数据区的有效长度
Output          :
Return          : int 结果字符串的长度
Calls           :
Called by       :

  History         :
  1 Date          : 2003-06-06
  Author        : Gan Zhiliang
  Modification  : Created file
*******************************************************************************/
int GetTextValueOf3GR4SGSNSMO(char *pValue, char *pBuffer, int nLength)
{

    R4_Org_SGSNSMORecord *pBill = (R4_Org_SGSNSMORecord*)pValue;
    int nLen = 0;
    int nTempLen = 0;

    //ASSERT((BOOL)(pValue && pBuffer));
    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Org_CallEventRecordType_sgsnSMORecord)
        return 0;

    char *pTempBuffer = pBuffer;

    if (!pTempBuffer)
        return 0;

    //recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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
    if (pBill->bit_mask & R4_Org_SGSNSMORecord_servedIMEI_present)
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
    if (pBill->bit_mask & R4_Org_SGSNSMORecord_servedMSISDN_present)
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
    if(pBill->bit_mask & R4_Org_SGSNSMORecord_msNetworkCapability_present)
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

    //serviceCentre
    if(pBill->bit_mask & R4_Org_SGSNSMORecord_serviceCentre_present)
    {
        nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->serviceCentre, &pTempBuffer[nLen],
            nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //recordingEntity
    if(pBill->bit_mask & R4_Org_SGSNSMORecord_recordingEntity_present)
    {
        nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->recordingEntity, &pTempBuffer[nLen],
            nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }

    pTempBuffer[nLen++] = '^';

    //locationArea
    if (pBill->bit_mask & R4_Org_SGSNSMORecord_locationArea_present)
    {
        nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationArea, &pTempBuffer[nLen],
            nLength - nLen, "0x", FALSE);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //routingArea
    if (pBill->bit_mask & R4_Org_SGSNSMORecord_routingArea_present)
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
    if (pBill->bit_mask & R4_Org_SGSNSMORecord_cellIdentifier_present)
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
    nLen += NumToDecStr((long)pBill->messageReference.value, &pTempBuffer[nLen],
        nLength - nLen, FALSE);
    pTempBuffer[nLen++] = '^';

    //eventTimeStamp
    nLen += TimeStampToStr((LPTIMESTAMP)&pBill->eventTimeStamp, &pTempBuffer[nLen],
        nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //smsResult
    if (pBill->bit_mask & R4_Org_SGSNSMORecord_smsResult_present)
    {
        nLen += R4DiagnosticsToStr(&pBill->smsResult, &pTempBuffer[nLen],
            nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //recordExtensions
    if (pBill->bit_mask & R4_Org_SGSNSMORecord_recordExtensions_present)
    {
        nLen += R4RecordExtensionsToStr(pBill->recordExtensions, &pTempBuffer[nLen],
            nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //nodeID
    if (pBill->bit_mask & R4_Org_SGSNSMORecord_nodeID_present)
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
    if (pBill->bit_mask & R4_Org_SGSNSMORecord_localSequenceNumber_present)
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

    nLen += R4ChargingCharacteristicsToStr(&pBill->chargingCharacteristics,
        &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //systemType
    if (pBill->bit_mask & R4_Org_SGSNSMORecord_systemType_present)
    {
        EnumInfo.nCount = R4SYSTEMTYPEENUMINFO_LENGTH;
        EnumInfo.pInfoTable = R4SYSTEMTYPEENUMINFOEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->systemType, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //destinationNumber
    if (pBill->bit_mask & R4_Org_destinationNumber_present)
    {
        R4_Org_CalledNumber destinationNumber = pBill->destinationNumber;
        destinationNumber.length = destinationNumber.length - 2;
        if(destinationNumber.length < 0)
        {
            destinationNumber.length = 0;
        }
        else
        {
            memcpy(destinationNumber.value, destinationNumber.value + 2, destinationNumber.length);
        }
        nLen += CvtBCDOctecToStr(LPOCTECSTRING(&destinationNumber), &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //cAMELInformationSMS
    if(pBill->bit_mask & R4_Org_SGSNSMORecord_cAMELInformationSMS_present)
    {
        nLen += R43GSGSNSMO_CAMELInfomationToStr(&pBill->cAMELInformationSMS,
            &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //chChSelectionMode
    if (pBill->bit_mask & R4_Org_SGSNSMORecord_chChSelectionMode_present)
    {
        EnumInfo.nCount = DEFAULTR4chChSELECTIONMODEINFOEnumInfo_LENGTH;
        EnumInfo.pInfoTable = R4chChSELECTIONMODEINFOEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->chChSelectionMode, &pTempBuffer[nLen], nLength - nLen);
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

/*******************************************************************************
Function        : GetTextValueOf3GR4SGSNSMT
Description     : 处理S-SMT-CDR话单浏览解释的函数
Input           : char *pValue 话单数据
: char *pBuffer 存放结果的数据区
: int nLength 数据区的有效长度
Output          :
Return          : int 结果字符串的长度
Calls           :
Called by       :

History         :
1 Date          : 2003-06-06
Author        : Gan Zhiliang
Modification  : Created file
*******************************************************************************/
int GetTextValueOf3GR4SGSNSMT(char *pValue, char *pBuffer, int nLength)
{

    R4_Org_SGSNSMTRecord *pBill = (R4_Org_SGSNSMTRecord*)pValue;
    int nLen = 0;
    int nTempLen = 0;

    //ASSERT((BOOL)(pValue && pBuffer));
    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Org_CallEventRecordType_sgsnSMTRecord)
        return 0;

    char *pTempBuffer = pBuffer;

    if (!pTempBuffer)
        return 0;

    //recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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
    if (pBill->bit_mask & R4_Org_SGSNSMTRecord_servedIMEI_present)
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
    if (pBill->bit_mask & R4_Org_SGSNSMTRecord_servedMSISDN_present)
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
    if(pBill->bit_mask & R4_Org_SGSNSMTRecord_msNetworkCapability_present)
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

    //serviceCentre
    if(pBill->bit_mask & R4_Org_SGSNSMTRecord_serviceCentre_present)
    {
        nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->serviceCentre, &pTempBuffer[nLen],
            nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //recordingEntity
    if(pBill->bit_mask & R4_Org_SGSNSMTRecord_recordingEntity_present)
    {
        nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->recordingEntity, &pTempBuffer[nLen],
            nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //locationArea
    if (pBill->bit_mask & R4_Org_SGSNSMTRecord_locationArea_present)
    {
        nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationArea, &pTempBuffer[nLen],
            nLength - nLen, "0x", FALSE);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //routingArea
    if (pBill->bit_mask & R4_Org_SGSNSMTRecord_routingArea_present)
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
    if (pBill->bit_mask & R4_Org_SGSNSMTRecord_cellIdentifier_present)
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

    //eventTimeStamp
    nLen += TimeStampToStr((LPTIMESTAMP)&pBill->eventTimeStamp, &pTempBuffer[nLen],
        nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //smsResult
    if (pBill->bit_mask & R4_Org_SGSNSMTRecord_smsResult_present)
    {
        nLen += R4DiagnosticsToStr(&pBill->smsResult, &pTempBuffer[nLen],
            nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //recordExtensions
    if (pBill->bit_mask & R4_Org_SGSNSMTRecord_recordExtensions_present)
    {
        nLen += R4RecordExtensionsToStr(pBill->recordExtensions, &pTempBuffer[nLen],
            nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //nodeID
    if (pBill->bit_mask & R4_Org_SGSNSMTRecord_nodeID_present)
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
    if (pBill->bit_mask & R4_Org_SGSNSMTRecord_localSequenceNumber_present)
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
    nLen += R4ChargingCharacteristicsToStr(&pBill->chargingCharacteristics,
        &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //systemType
    if (pBill->bit_mask & R4_Org_SGSNSMTRecord_systemType_present)
    {
        EnumInfo.nCount = R4SYSTEMTYPEENUMINFO_LENGTH;
        EnumInfo.pInfoTable = R4SYSTEMTYPEENUMINFOEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->systemType, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //chChSelectionMode
    if (pBill->bit_mask & R4_Org_SGSNSMTRecord_chChSelectionMode_present)
    {
        EnumInfo.nCount = DEFAULTR4chChSELECTIONMODEINFOEnumInfo_LENGTH;
        EnumInfo.pInfoTable = R4chChSELECTIONMODEINFOEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->chChSelectionMode, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //cAMELInformationSMS
    if(pBill->bit_mask & R4_Org_SGSNSMTRecord_cAMELInformationSMS_present)
    {
        nLen += R43GSGSNSMO_CAMELInfomationToStr(&pBill->cAMELInformationSMS,
            &pTempBuffer[nLen], nLength - nLen);
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


/*******************************************************************************
Function        : GetTextValueOf3GR4SGSNLCT
Description     : 处理SGSN-LCT话单的浏览问题
Input           : char *pValue 话单数据
: char *pBuffer 存放结果的数据区
: int nLength 数据区的有效长度
Output          :
Return          : int 结果字符串的长度
Calls           :
Called by       :

  History         :
  1 Date          : 2003-6-2
  Author        : Gan Zhiliang
  Modification  : Created file
*******************************************************************************/
int GetTextValueOf3GR4SGSNLCT(char *pValue, char *pBuffer, int nLength)
{

    R4_Org_SGSNLCTRecord *pBill = (R4_Org_SGSNLCTRecord*)pValue;
    int nLen = 0;
	int nTempLen = 0;

    //ASSERT((BOOL)(pValue && pBuffer));
    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Org_sgsnMtLCSRecord)
        return 0;

    char *pTempBuffer = pBuffer;

    if (!pTempBuffer)
        return 0;

    //recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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

    //recordingEntity
    nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->recordingEntity,
        &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //lcsClientType
    EnumInfo.nCount = DEFAULTR4OrgLCSClientTypeEnumInfo_LENGTH;
    EnumInfo.pInfoTable = R4OrgLCSClientTypeEnumInfo;
    nLen += EnumToStr(&EnumInfo, pBill->lcsClientType, &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';


    //lcsClientIdentity
    nLen += R4LcsClientIdentityToStr(&(pBill->lcsClientIdentity), &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //servedIMSI
    nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI,
        &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //servedMSISDN
    if (pBill->bit_mask & R4_Org_SGSNLCTRecord_servedMSISDN_present)
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

    //sgsnAddress
    if(pBill->bit_mask & R4_Org_SGSNLCTRecord_sgsnAddress_present)
    {
        nLen += R43GIPAddrToStr(&pBill->sgsnAddress, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //locationType
    nLen += R4LocationTypeToStr(&pBill->locationType, &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //lcsQos
    if(pBill->bit_mask & R4_Org_SGSNLCTRecord_lcsQos_present)
    {
        nLen += BCDOctecToStr((LPOCTECSTRING)&pBill->lcsQos,
            &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;

    }
    pTempBuffer[nLen++] = '^';

    //lcsPriority
    if(pBill->bit_mask & R4_Org_SGSNLCTRecord_lcsPriority_present)
    {
        nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->lcsPriority,
            &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;

    }
    pTempBuffer[nLen++] = '^';

    //mlcNumber
    nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->mlcNumber, &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //eventTimeStamp
    nLen += TimeStampToStr((LPTIMESTAMP)&pBill->eventTimeStamp, &pTempBuffer[nLen],
        nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //measurementDuration
    if ( pBill->bit_mask & R4_Org_SGSNLCTRecord_measurementDuration_present )
    {

        nLen += NumToDecStr(pBill->measurementDuration, &pTempBuffer[nLen], nLength - nLen, FALSE);

    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }

    pTempBuffer[nLen++] = '^';

    //notificationToMSUser
    if ( pBill->bit_mask & R4_Org_notificationToMSUser_present )
    {
        EnumInfo.nCount = DEFAULTR4_Org_NotificationToMSUserEnumInfo_LENGTH;
        EnumInfo.pInfoTable = R4_Org_NotificationToMSUserEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->notificationToMSUser, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //privacyOverride
    if ( pBill->bit_mask &  R4_Org_privacyOverride_present)
    {
        pTempBuffer[nLen++] = pBill->privacyOverride;

    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //location
    if ( pBill->bit_mask & R4_Org_SGSNLCTRecord_location_present )
    {
        nLen += LocationAreaAndCellToStr(&pBill->location,&pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //routingArea
    if ( pBill->bit_mask & R4_Org_SGSNLCTRecord_routingArea_present )
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

    //locationEstimate
    if( pBill->bit_mask & R4_Org_SGSNLCTRecord_locationEstimate_present)
    {
         nLen += BCDOctecToStr((LPOCTECSTRING)&pBill->locationEstimate,
            &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //lcsCause
    if( pBill->bit_mask & R4_Org_SGSNLCTRecord_lcsCause_present)
    {
        nLen += BCDOctecToStr((LPOCTECSTRING)&pBill->lcsCause,
            &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //diagnostics
    if (pBill->bit_mask & R4_Org_SGSNLCTRecord_diagnostics_present)
    {
        nLen += R4DiagnosticsToStr(&pBill->diagnostics, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //nodeID
    if (pBill->bit_mask & R4_Org_SGSNLCTRecord_nodeID_present)
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
    if (pBill->bit_mask & R4_Org_SGSNLCTRecord_localSequenceNumber_present)
    {
        nLen += NumToDecStr(pBill->localSequenceNumber, &pTempBuffer[nLen], nLength - nLen, FALSE);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //chargingCharacteristics
    nLen += R4ChargingCharacteristicsToStr((R4_Org_ChargingCharacteristics*)&pBill->chargingCharacteristics,
        &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //chChSelectionMode
    if (pBill->bit_mask & R4_Org_SGSNLCTRecord_chChSelectionMode_present)
    {
        EnumInfo.nCount = DEFAULTR4chChSELECTIONMODEINFOEnumInfo_LENGTH;
        EnumInfo.pInfoTable = R4chChSELECTIONMODEINFOEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->chChSelectionMode, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //systemType
    if (pBill->bit_mask & R4_Org_SGSNLCTRecord_systemType_present)
    {
        EnumInfo.nCount = R4SYSTEMTYPEENUMINFO_LENGTH;
        EnumInfo.pInfoTable = R4SYSTEMTYPEENUMINFOEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->systemType, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //recordExtensions
    if (pBill->bit_mask & R4_Org_SGSNLCTRecord_recordExtensions_present)
    {
        nLen += R4RecordExtensionsToStr(pBill->recordExtensions, &pTempBuffer[nLen],
            nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

	//causeForRecClosing
    EnumInfo.nCount = R4CAUSEFORRECCLOSINGENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R4CauseForRecClosingEnumInfo;
    nTempLen = EnumToStr(&EnumInfo, pBill->causeForRecClosing,
        &pTempBuffer[nLen], nLength - nLen);
    if (nTempLen)
        nLen += nTempLen;
    else
    {
        strcpy(&pTempBuffer[nLen], "Unknown reason");
        nLen += strlen("Unknown reason");
    }
    pTempBuffer[nLen++] = '^';

    pTempBuffer[nLen] = 0;


    if (nLen >= nLength)
        nLen = 0;

    return nLen;

}

/*******************************************************************************
Function        : GetTextValueOf3GR4SGSNLCO
Description     : 处理SGSN-LCO话单的浏览问题
Input           : char *pValue 话单数据
: char *pBuffer 存放结果的数据区
: int nLength 数据区的有效长度
Output          :
Return          : int 结果字符串的长度
Calls           :
Called by       :
History         :
1 Date          : 2003-6-2
Author        : Gan Zhiliang
Modification  : Created file
*******************************************************************************/
int GetTextValueOf3GR4SGSNLCO(char *pValue, char *pBuffer, int nLength)
{

    R4_Org_SGSNLCORecord *pBill = (R4_Org_SGSNLCORecord*)pValue;
    int nLen = 0;
	int nTempLen = 0;

    //ASSERT((BOOL)(pValue && pBuffer));
    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Org_sgsnMoLCSRecord)
        return 0;

    char *pTempBuffer = pBuffer;

    if (!pTempBuffer)
        return 0;

    //recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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

    //recordingEntity
    nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->recordingEntity,
        &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //lcsClientType
    if(pBill->bit_mask & R4_Org_SGSNLCORecord_lcsClientType_present)
    {
        EnumInfo.nCount = DEFAULTR4OrgLCSClientTypeEnumInfo_LENGTH;
        EnumInfo.pInfoTable = R4OrgLCSClientTypeEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->lcsClientType, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;

    }
    pTempBuffer[nLen++] = '^';


    //lcsClientIdentity
    if(pBill->bit_mask & R4_Org_SGSNLCORecord_lcsClientIdentity_present)
    {
        nLen += R4LcsClientIdentityToStr(&(pBill->lcsClientIdentity), &pTempBuffer[nLen], nLength - nLen);
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

    //servedMSISDN
    if (pBill->bit_mask & R4_Org_SGSNLCORecord_servedMSISDN_present)
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

    //sgsnAddress
    if(pBill->bit_mask & R4_Org_SGSNLCORecord_sgsnAddress_present)
    {
        nLen += R43GIPAddrToStr(&pBill->sgsnAddress, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //locationMethod
    EnumInfo.nCount = DEFAULTR4_Org_LocationMethodEnumInfo_LENGTH;
    EnumInfo.pInfoTable = R4_Org_LocationMethodEnumInfo;
    nLen += EnumToStr(&EnumInfo, pBill->locationMethod, &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //lcsQos
    if(pBill->bit_mask & R4_Org_SGSNLCORecord_lcsQos_present)
    {
        nLen += BCDOctecToStr((LPOCTECSTRING)&pBill->lcsQos,
            &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;

    }
    pTempBuffer[nLen++] = '^';

    //lcsPriority
    if(pBill->bit_mask & R4_Org_SGSNLCORecord_lcsPriority_present)
    {
        nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->lcsPriority,
            &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;

    }
    pTempBuffer[nLen++] = '^';

    //mlcNumber
    if(pBill->bit_mask & R4_Org_SGSNLCORecord_mlcNumber_present)
    {
        nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->mlcNumber, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }

    pTempBuffer[nLen++] = '^';

    //eventTimeStamp
    nLen += TimeStampToStr((LPTIMESTAMP)&pBill->eventTimeStamp, &pTempBuffer[nLen],
        nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //measurementDuration
    if ( pBill->bit_mask & R4_Org_SGSNLCORecord_measurementDuration_present )
    {

        nLen += NumToDecStr(pBill->measurementDuration, &pTempBuffer[nLen], nLength - nLen, FALSE);

    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }

    pTempBuffer[nLen++] = '^';


    //location
    if ( pBill->bit_mask & R4_Org_SGSNLCORecord_location_present )
    {
        nLen += LocationAreaAndCellToStr(&pBill->location,&pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //routingArea
    if ( pBill->bit_mask & R4_Org_SGSNLCORecord_routingArea_present )
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

    //locationEstimate
    if( pBill->bit_mask & R4_Org_SGSNLCTRecord_locationEstimate_present)
    {
        nLen += BCDOctecToStr((LPOCTECSTRING)&pBill->locationEstimate,
            &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //lcsCause
    if( pBill->bit_mask & R4_Org_SGSNLCTRecord_lcsCause_present)
    {
        nLen += BCDOctecToStr((LPOCTECSTRING)&pBill->lcsCause,
            &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //diagnostics
    if (pBill->bit_mask & R4_Org_SGSNLCORecord_diagnostics_present)
    {
        nLen += R4DiagnosticsToStr((R4_Org_Diagnostics*)&pBill->diagnostics, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //nodeID
    if (pBill->bit_mask & R4_Org_SGSNLCORecord_nodeID_present)
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
    if (pBill->bit_mask & R4_Org_SGSNLCORecord_localSequenceNumber_present)
    {
        nLen += NumToDecStr(pBill->localSequenceNumber, &pTempBuffer[nLen], nLength - nLen, FALSE);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //chargingCharacteristics
    nLen += R4ChargingCharacteristicsToStr((R4_Org_ChargingCharacteristics*)&pBill->chargingCharacteristics,
        &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //chChSelectionMode
    if (pBill->bit_mask & R4_Org_SGSNLCORecord_chChSelectionMode_present)
    {
        EnumInfo.nCount = DEFAULTR4chChSELECTIONMODEINFOEnumInfo_LENGTH;
        EnumInfo.pInfoTable = R4chChSELECTIONMODEINFOEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->chChSelectionMode, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //systemType
    if (pBill->bit_mask & R4_Org_SGSNLCORecord_systemType_present)
    {
        EnumInfo.nCount = R4SYSTEMTYPEENUMINFO_LENGTH;
        EnumInfo.pInfoTable = R4SYSTEMTYPEENUMINFOEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->systemType, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //recordExtensions
    if (pBill->bit_mask & R4_Org_SGSNLCORecord_recordExtensions_present)
    {
        nLen += R4RecordExtensionsToStr(pBill->recordExtensions, &pTempBuffer[nLen],
            nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

	//causeForRecClosing
    EnumInfo.nCount = R4CAUSEFORRECCLOSINGENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R4CauseForRecClosingEnumInfo;
    nTempLen = EnumToStr(&EnumInfo, pBill->causeForRecClosing,
        &pTempBuffer[nLen], nLength - nLen);
    if (nTempLen)
        nLen += nTempLen;
    else
    {
        strcpy(&pTempBuffer[nLen], "Unknown reason");
        nLen += strlen("Unknown reason");
    }
    pTempBuffer[nLen++] = '^';


    pTempBuffer[nLen] = 0;

    if (nLen >= nLength)
        nLen = 0;

    return nLen;

}


/*******************************************************************************
Function        : GetTextValueOf3GR4SGSNLCN
Description     : 处理SGSN-LCN话单的浏览问题
Input           : char *pValue 话单数据
: char *pBuffer 存放结果的数据区
: int nLength 数据区的有效长度
Output          :
Return          : int 结果字符串的长度
Calls           :
Called by       :

  History         :
  1 Date          : 2003-6-2
  Author        : Gan Zhiliang
  Modification  : Created file
*******************************************************************************/
int GetTextValueOf3GR4SGSNLCN(char *pValue, char *pBuffer, int nLength)
{

    R4_Org_SGSNLCNRecord *pBill = (R4_Org_SGSNLCNRecord*)pValue;
    int nLen = 0;
	int nTempLen = 0;

    //ASSERT((BOOL)(pValue && pBuffer));
    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Org_sgsnNiLCSRecord)
        return 0;

    char *pTempBuffer = pBuffer;

    if (!pTempBuffer)
        return 0;

    //recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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

    //recordingEntity
    nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->recordingEntity,
        &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //lcsClientType
    if(pBill->bit_mask & R4_Org_SGSNLCNRecord_lcsClientType_present)
    {
        EnumInfo.nCount = DEFAULTR4OrgLCSClientTypeEnumInfo_LENGTH;
        EnumInfo.pInfoTable = R4OrgLCSClientTypeEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->lcsClientType, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;

    }
    pTempBuffer[nLen++] = '^';


    //lcsClientIdentity

    if(pBill->bit_mask & R4_Org_SGSNLCNRecord_lcsClientIdentity_present)
    {
        nLen += R4LcsClientIdentityToStr(&(pBill->lcsClientIdentity), &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;

    }
    pTempBuffer[nLen++] = '^';

    //servedIMSI
    if(pBill->bit_mask & R4_Org_servedIMSI_present)
    {
        nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI,
            &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //servedMSISDN
    if (pBill->bit_mask & R4_Org_SGSNLCNRecord_servedMSISDN_present)
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

    //sgsnAddress
    if(pBill->bit_mask & R4_Org_SGSNLCNRecord_sgsnAddress_present)
    {
        nLen += R43GIPAddrToStr(&pBill->sgsnAddress, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //servedIMEI
    if (pBill->bit_mask & R4_Org_SGSNLCNRecord_servedIMEI_present)
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


    //lcsQos
    if(pBill->bit_mask & R4_Org_SGSNLCNRecord_lcsQos_present)
    {
        nLen += BCDOctecToStr((LPOCTECSTRING)&pBill->lcsQos,
            &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;

    }
    pTempBuffer[nLen++] = '^';

    //lcsPriority
    if(pBill->bit_mask & R4_Org_SGSNLCNRecord_lcsPriority_present)
    {
        nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->lcsPriority,
            &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;

    }
    pTempBuffer[nLen++] = '^';

    //mlcNumber
    if(pBill->bit_mask & R4_Org_SGSNLCNRecord_mlcNumber_present)
    {
        nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pBill->mlcNumber, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }

    pTempBuffer[nLen++] = '^';

    //eventTimeStamp
    nLen += TimeStampToStr((LPTIMESTAMP)&pBill->eventTimeStamp, &pTempBuffer[nLen],
        nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //measurementDuration
    if ( pBill->bit_mask & R4_Org_SGSNLCNRecord_measurementDuration_present )
    {

        nLen += NumToDecStr(pBill->measurementDuration, &pTempBuffer[nLen], nLength - nLen, FALSE);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }

    pTempBuffer[nLen++] = '^';


    //location
    if ( pBill->bit_mask & R4_Org_SGSNLCNRecord_location_present )
    {
        nLen += LocationAreaAndCellToStr(&pBill->location,&pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //routingArea
    if ( pBill->bit_mask & R4_Org_SGSNLCNRecord_routingArea_present )
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

    //locationEstimate
    if( pBill->bit_mask & R4_Org_SGSNLCTRecord_locationEstimate_present)
    {
        nLen += BCDOctecToStr((LPOCTECSTRING)&pBill->locationEstimate,
            &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //lcsCause
    if( pBill->bit_mask & R4_Org_SGSNLCTRecord_lcsCause_present)
    {
        nLen += BCDOctecToStr((LPOCTECSTRING)&pBill->lcsCause,
            &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';


    //diagnostics
    if (pBill->bit_mask & R4_Org_SGSNLCNRecord_diagnostics_present)
    {
        nLen += R4DiagnosticsToStr((R4_Org_Diagnostics*)&pBill->diagnostics, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //nodeID
    if (pBill->bit_mask & R4_Org_SGSNLCNRecord_nodeID_present)
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
    if (pBill->bit_mask & R4_Org_SGSNLCNRecord_localSequenceNumber_present)
    {
        nLen += NumToDecStr(pBill->localSequenceNumber, &pTempBuffer[nLen], nLength - nLen, FALSE);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //chargingCharacteristics
    nLen += R4ChargingCharacteristicsToStr((R4_Org_ChargingCharacteristics*)&pBill->chargingCharacteristics,
        &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //chChSelectionMode
    if (pBill->bit_mask & R4_Org_SGSNLCNRecord_chChSelectionMode_present)
    {
        EnumInfo.nCount = DEFAULTR4chChSELECTIONMODEINFOEnumInfo_LENGTH;
        EnumInfo.pInfoTable = R4chChSELECTIONMODEINFOEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->chChSelectionMode, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //systemType
    if (pBill->bit_mask & R4_Org_SGSNLCNRecord_systemType_present)
    {
        EnumInfo.nCount = R4SYSTEMTYPEENUMINFO_LENGTH;
        EnumInfo.pInfoTable = R4SYSTEMTYPEENUMINFOEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->systemType, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //recordExtensions
    if (pBill->bit_mask & R4_Org_SGSNLCNRecord_recordExtensions_present)
    {
        nLen += R4RecordExtensionsToStr(pBill->recordExtensions, &pTempBuffer[nLen],
            nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

	//causeForRecClosing
    EnumInfo.nCount = R4CAUSEFORRECCLOSINGENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R4CauseForRecClosingEnumInfo;
    nTempLen = EnumToStr(&EnumInfo, pBill->causeForRecClosing,
        &pTempBuffer[nLen], nLength - nLen);
    if (nTempLen)
        nLen += nTempLen;
    else
    {
        strcpy(&pTempBuffer[nLen], "Unknown reason");
        nLen += strlen("Unknown reason");
    }
    pTempBuffer[nLen++] = '^';


    pTempBuffer[nLen] = 0;

    if (nLen >= nLength)
        nLen = 0;

    return nLen;

}

/*****************************************************************************
 Prototype    : GetTextValueOf3GR4LARGESGSNPDP
 Description  : 处理合并后S-CDR话单浏览解释的函数
 Input        : char *pValue   话单数据
                char *pBuffer  存放结果的数据区
                int nLength    数据区的有效长度
 Output       : None
 Return Value : 结果字符串的长度
 Calls        :
 Called By    :

  History        :
  1.Date         : 2003-06-06
    Author       : Gan Zhiliang
    Modification : Created function

*****************************************************************************/
int GetTextValueOf3GR4LARGESGSNPDP(char * pValue, char * pBuffer, int nLength)
{
    R4_Fin_SGSNPDPRecord *pBill = (R4_Fin_SGSNPDPRecord*)pValue;
    int nLen = 0;
    int nTempLen = 0;

    //ASSERT((BOOL)(pValue && pBuffer));
    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Fin_CallEventRecordType_sgsnPDPRecord)
        return 0;

    char *pTempBuffer = pBuffer;

    if (!pTempBuffer)
        return 0;

    //recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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
    if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_networkInitiation_present)
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
    if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_servedIMEI_present)
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
    if(pBill->bit_mask & R4_Fin_SGSNPDPRecord_sgsnAddress_present)
    {
       for (; pBill->sgsnAddress != NULL; pBill->sgsnAddress = pBill->sgsnAddress->next)
      {
          pTempBuffer[nLen++] = '{';
          nLen += R43GIPAddrToStr((struct R4_Org_IPAddress *)&pBill->sgsnAddress->value, &pTempBuffer[nLen],
              nLength - nLen);
          pTempBuffer[nLen++] = '}';
      }

        //nLen += R43GIPAddrToStr((R4_Org_IPAddress *)pBill->sgsnAddress, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //msNetworkCapability
    if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_msNetworkCapability_present)
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
    if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_routingArea_present)
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
    if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_locationAreaCode_present)
    {
         nLen += HexOctecToStr((LPOCTECSTRING)&pBill->locationAreaCode, &pTempBuffer[nLen],
            nLength - nLen, "0x", FALSE);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //cellIdentifier
    if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_cellIdentifier_present)
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
    nLen += R43GIPAddrToStr((R4_Org_IPAddress *)&pBill->ggsnAddressUsed, &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //accessPointNameNI
    if(pBill->bit_mask & R4_Fin_SGSNPDPRecord_accessPointNameNI_present)
    {
        strcpy(&pTempBuffer[nLen], pBill->accessPointNameNI);
        nLen += strlen(pBill->accessPointNameNI);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //pdpType
    if( pBill->bit_mask & R4_Fin_SGSNPDPRecord_pdpType_present)
    {

        nLen += HexOctecToStr((LPOCTECSTRING)&pBill->pdpType, &pTempBuffer[nLen],
            nLength - nLen, "0x", FALSE);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //servedPDPAddress
    if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_servedPDPAddress_present)
    {
        nLen += R43GPDPAddrToStr((R4_Org_PDPAddress *)&pBill->servedPDPAddress, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //listOfTrafficVolumes
    if( pBill->bit_mask & R4_Fin_SGSNPDPRecord_listOfTrafficVolumes_present  )
    {
        R4_Fin_NetworkInitiatedPDPContext networkInitiation = pBill->bit_mask & R4_Fin_SGSNPDPRecord_networkInitiation_present ? pBill->networkInitiation : NULL;
        nLen += R4ListOfTrafficVolumesToStr((R4_Org__seqof1_*)pBill->listOfTrafficVolumes, &pTempBuffer[nLen],
            nLength - nLen, networkInitiation);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //recordOpeningTime
    nLen += TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime, &pTempBuffer[nLen],
        nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //duration
    nLen += NumToDecStr(pBill->duration, &pTempBuffer[nLen], nLength - nLen, FALSE);
    pTempBuffer[nLen++] = '^';

    //sgsnChange
    if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_sgsnChange_present)
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
    EnumInfo.nCount = R4CAUSEFORRECCLOSINGENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R4CauseForRecClosingEnumInfo;
    nTempLen = EnumToStr(&EnumInfo, pBill->causeForRecClosing,&pTempBuffer[nLen], nLength - nLen);

    if (nTempLen)
        nLen += nTempLen;
    else
    {
        strcpy(&pTempBuffer[nLen], "Unknown reason)");
        nLen += strlen("Unknown reason");
    }
    pTempBuffer[nLen++] = '^';

    //diagnostics
    if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_diagnostics_present)
    {
        nLen += R4DiagnosticsToStr((R4_Org_Diagnostics*)&pBill->diagnostics, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //recSequenceNumList
    if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_recSequenceNumList_present)
    {
        R4_Fin_SGSNPDPRecord::R4_Fin__seqof4* pList = pBill->recSequenceNumList;
        for( ; pList != NULL; pList = pList->next)
        {
            pTempBuffer[nLen++] = '{';
            strcpy(&pTempBuffer[nLen], "sgsnAddress:");
            nLen += strlen("sgsnAddress:");
            nLen += R43GIPAddrToStr((R4_Org_IPAddress *)&pList->value.sgsnAddress, &pTempBuffer[nLen],
                nLength - nLen);
            if (pList->value.bit_mask & R4_Fin_recSequenceNumberList_present)
            {
                strcpy(&pTempBuffer[nLen], " recSequenceNumber:");
                nLen += strlen(" recSequenceNumber:");
                
                R4_Fin__seqof1_* pNode = pList->value.recSequenceNumberList;
                for( ; pNode != NULL; pNode = pNode->next)
                {
                    nLen += NumToDecStr(pNode->value, &pTempBuffer[nLen], nLength - nLen, FALSE);
                    pTempBuffer[nLen++] = ' ';
                }
            }
            pTempBuffer[nLen++] = '}';
        }
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //nodeID
    if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_nodeID_present)
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
    if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_recordExtensions_present)
    {
        nLen += R4RecordExtensionsToStr((struct R4_Org_ManagementExtensions_ *)pBill->recordExtensions, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

	//localSequenceNumberList
    if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_localSequenceNumberList_present)
    {
        R4_Fin_SGSNPDPRecord::R4_Fin__seqof5* pList = pBill->localSequenceNumberList;
        for( ; pList != NULL; pList = pList->next)
        {        
            pTempBuffer[nLen++] = '{';
            strcpy(&pTempBuffer[nLen], "sgsnAddress:");
            nLen += strlen("sgsnAddress:");
            nLen += R43GIPAddrToStr((R4_Org_IPAddress *)&pList->value.gsnAddress, &pTempBuffer[nLen],
                nLength - nLen);
            if (pList->value.bit_mask & R4_Fin_localSeqNumberList_present)
            {
                strcpy(&pTempBuffer[nLen], " localSequenceNumber:");
                nLen += strlen(" localSequenceNumber:");

                R4_Fin__seqof1_* pNode = pList->value.localSeqNumberList;
                for( ; pNode != NULL; pNode = pNode->next)
                {
                    nLen += NumToDecStr(pNode->value, &pTempBuffer[nLen], nLength - nLen, FALSE);
                    pTempBuffer[nLen++] = ' ';
                }
            }
            pTempBuffer[nLen++] = '}';
        }
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //apnSelectionMode
    if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_apnSelectionMode_present)
    {
        EnumInfo.nCount = APNSELECTIONMODEENUMINFO_LENGTH;
        EnumInfo.pInfoTable = R4ApnSelectionModeEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->apnSelectionMode, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //accessPointNameOI
    if ( pBill->bit_mask & R4_Fin_accessPointNameOI_present )
    {
        strcpy(&pTempBuffer[nLen], pBill->accessPointNameOI);
        nLen += strlen(pBill->accessPointNameOI);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //servedMSISDN
    if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_servedMSISDN_present)
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
    nLen += R4ChargingCharacteristicsToStr((R4_Org_ChargingCharacteristics *)&pBill->chargingCharacteristics, &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //systemType
    if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_systemType_present)
    {
        EnumInfo.nCount = R4SYSTEMTYPEENUMINFO_LENGTH;
        EnumInfo.pInfoTable = R4SYSTEMTYPEENUMINFOEnumInfo;
        nLen += EnumToStr(&EnumInfo, pBill->systemType, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

     //cAMELInfomationPDP
    if(pBill->bit_mask & R4_Fin_cAMELInformationPDP_present)
    {
        nLen += R43GSGSNPDP_CAMELInformationToStr((R4_Org_CAMELInformationPDP *)&pBill->cAMELInformationPDP, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';


    //rNCUnsentDownlinkVolume
	if (pBill->bit_mask & R4_Fin_rNCUnsentDownlinkVolumeList_present)
	{
        R4_Fin_SGSNPDPRecord::R4_Fin__seqof6* pList = pBill->rNCUnsentDownlinkVolumeList;
        for( ; pList != NULL; pList = pList->next)
        {        
		    pTempBuffer[nLen++] = '{';
		    strcpy(&pTempBuffer[nLen], "sgsnAddress:");
		    nLen += strlen("sgsnAddress:");
		    nLen += R43GIPAddrToStr((R4_Org_IPAddress *)&pList->value.sgsnAddress, &pTempBuffer[nLen],
			    nLength - nLen);
		    if (pList->value.bit_mask & R4_Fin_rNCUnsentDnVolumeList_present)
		    {
			    strcpy(&pTempBuffer[nLen], " rNCUnsentDownlinkVolume:");
			    nLen += strlen(" rNCUnsentDownlinkVolume:");

                R4_Fin__seqof1_* pNode = pList->value.rNCUnsentDnVolumeList;
			    for( ; pNode != NULL; pNode = pNode->next)
			    {
				    nLen += NumToDecStr(pNode->value, &pTempBuffer[nLen], nLength - nLen, FALSE);
				    pTempBuffer[nLen++] = ' ';
			    }
		    }
		    pTempBuffer[nLen++] = '}';
        }
	}
	else
	{
		strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
		nLen += _EMPTYSTRINGLENGTH;
	}
	pTempBuffer[nLen++] = '^';

    //chChSelectionMode
    if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_chChSelectionMode_present)
      {
          EnumInfo.nCount = DEFAULTR4chChSELECTIONMODEINFOEnumInfo_LENGTH;
          EnumInfo.pInfoTable = R4chChSELECTIONMODEINFOEnumInfo;
          nLen += EnumToStr(&EnumInfo, pBill->chChSelectionMode, &pTempBuffer[nLen], nLength - nLen);
      }
      else
      {
          strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
          nLen += _EMPTYSTRINGLENGTH;
      }
      pTempBuffer[nLen++] = '^';

    //dynamicAddressFlag
     if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_dynamicAddressFlag_present)
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

    //consolidationResult
    EnumInfo.nCount = R4CONSOLIDATIONRESULTENUMINFO_LENGTH;
    EnumInfo.pInfoTable = R4ConsolidationResultEnumInfo;
    nLen += EnumToStr(&EnumInfo, pBill->consolidationResult,
        &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    pTempBuffer[nLen] = 0;


    if (nLen >= nLength)
        nLen = 0;

    return nLen;
}


/*****************************************************************************
 Prototype    : GetTextValueOf3GR4LARGEGGSNPDP
 Description  : 处理合并后G-CDR话单浏览解释的函数
 Input        : char *pValue
                char *pBuffer
                int nLength
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2003-06-06
    Author       : Gan Zhiliang
    Modification : Created function

*****************************************************************************/
int GetTextValueOf3GR4LARGEGGSNPDP(char *pValue, char *pBuffer, int nLength)
{
    R4_Fin_GGSNPDPRecord *pBill = (R4_Fin_GGSNPDPRecord*)pValue;
    int nLen = 0;
    int nTempLen = 0;

    //ASSERT((BOOL)(pValue && pBuffer));
    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Fin_CallEventRecordType_ggsnPDPRecord)
        return 0;

    char *pTempBuffer = pBuffer;

    if (!pTempBuffer)
        return 0;

    //recordType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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
    if (pBill->bit_mask & R4_Fin_GGSNPDPRecord_networkInitiation_present)
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
    nLen += R43GIPAddrToStr((struct R4_Org_IPAddress*)&pBill->ggsnAddress, &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //chargingID
    nLen += NumToDecStr(pBill->chargingID, &pTempBuffer[nLen],
        nLength - nLen, FALSE);
    pTempBuffer[nLen++] = '^';

    unsigned short i = 0;  //Moved by XDB from inner "for" to outer "for"
    //sgsnAddress
    for (; pBill->sgsnAddress != NULL; pBill->sgsnAddress = pBill->sgsnAddress->next)
    {
         pTempBuffer[nLen++] = '{';
         nLen += R43GIPAddrToStr((struct R4_Org_IPAddress*)&pBill->sgsnAddress->value, &pTempBuffer[nLen],
                nLength - nLen);
         pTempBuffer[nLen++] = '}';
     }
    pTempBuffer[nLen++] = '^';

    //accessPointNameNI
    if(pBill->bit_mask & R4_Fin_GGSNPDPRecord_accessPointNameNI_present)
    {
        strcpy(&pTempBuffer[nLen], pBill->accessPointNameNI);
        nLen += strlen(pBill->accessPointNameNI);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;

    }
    pTempBuffer[nLen++] = '^';

    //pdpType
    if(pBill->bit_mask & R4_Fin_GGSNPDPRecord_pdpType_present)
    {
        nLen += HexOctecToStr((LPOCTECSTRING)&pBill->pdpType, &pTempBuffer[nLen],
            nLength - nLen, "0x", FALSE);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }

    pTempBuffer[nLen++] = '^';

    //servedPDPAddress
    if (pBill->bit_mask & R4_Fin_GGSNPDPRecord_servedPDPAddress_present)
    {
        nLen += R43GPDPAddrToStr((R4_Org_PDPAddress*)&pBill->servedPDPAddress, &pTempBuffer[nLen],
            nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //dynamicAddressFlag
    if (pBill->bit_mask & R4_Fin_GGSNPDPRecord_dynamicAddressFlag_present)
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

    //listOfTrafficVolumes
    if ( pBill->bit_mask & R4_Fin_GGSNPDPRecord_listOfTrafficVolumes_present )
    {
        R4_Fin_NetworkInitiatedPDPContext networkInitiation = pBill->bit_mask & R4_Fin_GGSNPDPRecord_networkInitiation_present ? pBill->networkInitiation : NULL;
        nLen += R4ListOfTrafficVolumesToStr((R4_Org__seqof1_ *)pBill->listOfTrafficVolumes, &pTempBuffer[nLen],
        nLength - nLen, networkInitiation);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }

    pTempBuffer[nLen++] = '^';

    //recordOpeningTime
    nLen += TimeStampToStr((LPTIMESTAMP)&pBill->recordOpeningTime, &pTempBuffer[nLen],
        nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //duration
    nLen += NumToDecStr(pBill->duration, &pTempBuffer[nLen],
        nLength - nLen, FALSE);
    pTempBuffer[nLen++] = '^';

    //causeForRecClosing
    EnumInfo.nCount = R4CAUSEFORRECCLOSINGENUNINFO_LENGTH;
    EnumInfo.pInfoTable = R4CauseForRecClosingEnumInfo;
    nTempLen = EnumToStr(&EnumInfo, pBill->causeForRecClosing,&pTempBuffer[nLen],
        nLength - nLen);
    if (nTempLen)
        nLen += nTempLen;
    else
    {
        strcpy(&pTempBuffer[nLen], "Unknown reason");
        nLen += strlen("Unknown reason");
    }
    pTempBuffer[nLen++] = '^';

    //diagnostics
    if (pBill->bit_mask & R4_Fin_GGSNPDPRecord_diagnostics_present)
    {
        nLen += R4DiagnosticsToStr((R4_Org_Diagnostics*)&pBill->diagnostics, &pTempBuffer[nLen],
            nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';


    //recSequenceNumList
    if (pBill->bit_mask & R4_Fin_GGSNPDPRecord_recSequenceNumList_present)
    {
        pTempBuffer[nLen++] = '{';
        strcpy(&pTempBuffer[nLen], "sgsnAddress:");
        nLen += strlen("sgsnAddress:");
        nLen += R43GIPAddrToStr((R4_Org_IPAddress*)&pBill->recSequenceNumList.sgsnAddress, &pTempBuffer[nLen],
            nLength - nLen);
        if (pBill->recSequenceNumList.bit_mask & R4_Fin_recSequenceNumberList_present)
        {
            strcpy(&pTempBuffer[nLen], " recSequenceNumber:");
            nLen += strlen(" recSequenceNumber:");
           //for (i = 0; i < pBill->recSequenceNumList.recSequenceNumber.count; i++)
            for( ; pBill->recSequenceNumList.recSequenceNumberList != NULL; pBill->recSequenceNumList.recSequenceNumberList = pBill->recSequenceNumList.recSequenceNumberList->next)
            {
                nLen += NumToDecStr(pBill->recSequenceNumList.recSequenceNumberList->value,
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
    if (pBill->bit_mask & R4_Fin_GGSNPDPRecord_nodeID_present)
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
    if (pBill->bit_mask & R4_Fin_GGSNPDPRecord_recordExtensions_present)
    {

        switch (g_nCBB_OI)
        {			    
        case CBB_OPERATOR_ID_CMCC:
            nLen += CMCC_R4_GCDRRecordExtensionsToStr((R4_Org_ManagementExtensions_*)pBill->recordExtensions, 
                         &pTempBuffer[nLen], nLength - nLen);
            break;
            
        default:
            nLen += R4_GCDRRecordExtensionsToStr((R4_Org_ManagementExtensions_*)pBill->recordExtensions, 
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

    //localSequenceNumberList
	if (pBill->bit_mask & R4_Fin_GGSNPDPRecord_localSequenceNumberList_present)
    {
        pTempBuffer[nLen++] = '{';
        strcpy(&pTempBuffer[nLen], "GGSNAddress:");
        nLen += strlen("GGSNAddress:");
        nLen += R43GIPAddrToStr((R4_Org_IPAddress*)&pBill->localSequenceNumberList.gsnAddress, &pTempBuffer[nLen],
            nLength - nLen);
        if (pBill->localSequenceNumberList.bit_mask & R4_Fin_localSeqNumberList_present)
        {
            strcpy(&pTempBuffer[nLen], " localSequenceNumber:");
            nLen += strlen(" localSequenceNumber:");
			//for (i = 0; i < pBill->recSequenceNumList.recSequenceNumber.count; i++)
            for( ; pBill->localSequenceNumberList.localSeqNumberList != NULL; pBill->localSequenceNumberList.localSeqNumberList = pBill->localSequenceNumberList.localSeqNumberList->next)
            {
                nLen += NumToDecStr(pBill->localSequenceNumberList.localSeqNumberList->value,
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

    //apnSelectionMode
    if (pBill->bit_mask & R4_Fin_GGSNPDPRecord_apnSelectionMode_present)
    {
        EnumInfo.nCount = APNSELECTIONMODEENUMINFO_LENGTH;
        EnumInfo.pInfoTable = R4ApnSelectionModeEnumInfo;
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
    if (pBill->bit_mask & R4_Fin_GGSNPDPRecord_servedMSISDN_present)
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
    nLen += R4ChargingCharacteristicsToStr((R4_Org_ChargingCharacteristics *)&pBill->chargingCharacteristics,
            &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    //chChSelectionMode
    if (pBill->bit_mask & R4_Fin_GGSNPDPRecord_chChSelectionMode_present)
    {
         EnumInfo.nCount = DEFAULTR4chChSELECTIONMODEINFOEnumInfo_LENGTH;
         EnumInfo.pInfoTable = R4chChSELECTIONMODEINFOEnumInfo;
         nLen += EnumToStr(&EnumInfo, pBill->chChSelectionMode, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
          strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
          nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';

    //added by ggf, 2003-11-20, sgsnPLMNIdentifier
    if (pBill->bit_mask & R4_Fin_sgsnPLMNIdentifier_present)
    {
        nLen += CvtBCDOctecToStr2((LPOCTECSTRING)&pBill->sgsnPLMNIdentifier, &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = '^';
	//end.

    //consolidationResult
    EnumInfo.nCount = R4CONSOLIDATIONRESULTENUMINFO_LENGTH;
    EnumInfo.pInfoTable = R4ConsolidationResultEnumInfo;
    nLen += EnumToStr(&EnumInfo, pBill->consolidationResult,
        &pTempBuffer[nLen], nLength - nLen);
    pTempBuffer[nLen++] = '^';

    pTempBuffer[nLen] = 0;


    if (nLen >= nLength)
        nLen = 0;

    return nLen;
}


/*******************************************************************************
  Function        : R43GIPAddrToStr
  Description     : 将R4_Org_IPAddress结构的数据转换成字符串
  Input           : R4_Org_IPAddress *pIPAddr 待转换数据
                  : char *pBuffer 存放返回字符串的数据区
                  : int nLength 数据区可用长度
  Output          :
  Return          : int
  Calls           :
  Called by       :

  History         :
  1 Date          : 2003-6-6
    Author        : Gan Zhiliang
    Modification  : Created file
*******************************************************************************/
int R43GIPAddrToStr(R4_Org_IPAddress *pIPAddr, char *pBuffer, int nLength)
{
    int nLen;

    //ASSERT((BOOL)pIPAddr);
    if ((pIPAddr == NULL) || (pBuffer == NULL)) 
    {
         return 0;
    }

    pBuffer[0] = 0;

    switch (pIPAddr->choice)    //判断是Bin还是Text类型
    {
    case R4_Org_iPBinaryAddress_chosen:  //Bin类型
        {
            switch (pIPAddr->u.iPBinaryAddress.choice)  //判断是IPV4还是IPV6
            {
            case R4_Org_iPBinV4Address_chosen:           //IPV4
                {
                    nLen = OctecToIPAddrStr((LPOCTECSTRING)&pIPAddr->u.iPBinaryAddress.u.iPBinV4Address,
                        pBuffer, nLength, BINIPV4_STYLE);
                }
                break;
            case R4_Org_iPBinV6Address_chosen:           //IPV6
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
    case R4_Org_iPTextRepresentedAddress_chosen: //Text类型
        {
            switch (pIPAddr->u.iPTextRepresentedAddress.choice) //判断是IPV4还是IPV6
            {
            case R4_Org_iPTextV4Address_chosen:          //IPV4
                {
                    if (nLength < 16)
                        return 0;
                    memcpy(pBuffer, pIPAddr->u.iPTextRepresentedAddress.u.iPTextV4Address, 15);
                    pBuffer[15] = 0;
                    nLen = strlen(pBuffer);
                }
                break;
            case R4_Org_iPTextV6Address_chosen:          //IPV6
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

/*******************************************************************************
  Function        : R43GPDPAddrToStr
  Description     : 将R4_Org_PDPAddress结构的数据转换成字符串
  Input           : R4_Org_PDPAddress *pPDPAddress 待转换的数据
                  : char *pBuffer 存放返回字符串的数据区
                  : int nLength 数据区可用长度
  Output          :
  Return          : int 转换后的字符串的长度
  Calls           :
  Called by       :

  History         :
  1 Date          : 2003-6-6
    Author        : Gan Zhiliang
    Modification  : Created file
*******************************************************************************/
int R43GPDPAddrToStr(R4_Org_PDPAddress *pPDPAddress, char *pBuffer, int nLength)
{
    int nLen = 0;

    //ASSERT((BOOL)pPDPAddress);
    if ((pPDPAddress == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    pBuffer[0] = 0;

    switch (pPDPAddress->choice)
    {
    case R4_Org_iPAddress_chosen:
        {
            nLen = R43GIPAddrToStr(&pPDPAddress->u.iPAddress, pBuffer, nLength);
        }
        break;
    case R4_Org_eTSIAddress_chosen:
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
// Function name: 	R4ListOfTrafficVolumesToStr
// Author:        
// Date:          
// Description:   转换ListOfTrafficVolumes类型的数据为字符串
// Input:
//       arg1 :R4_Org__seqof1_ *pList	待转换的数据
//       arg2 :char *pBuffer		转换后数据存放的Buffer
//       arg3 :int nLength			Buffer的可用长度
//       arg4 :R4_Org_NetworkInitiatedPDPContext networkInitiation 代表是MS to netWork还是netWork to MS
// Return value: int				转换后字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 此函数从R99同步过来
******************************************************/
int R4ListOfTrafficVolumesToStr(R4_Org__seqof1_ *pList, char *pBuffer, int nLength, char networkInitiation)
{
#define LISTOFTRAFFICVOLUMESBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	int nLen = 0;
	R4_Org_ChangeOfCharCondition *pItem;

	ENUMINFO EnumInfo;
	EnumInfo.nCount = R4CHANGECONDITIONENUMINFO_LENGTH; //3
	EnumInfo.pInfoTable = R4ChangeConditionEnumInfo;
	
	//ASSERT((BOOL)(pBuffer && pList));
	if ((pBuffer == NULL) || (pList == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;	

	char *pTempBuffer = pBuffer;

	if (!pTempBuffer)
		return 0;
	pTempBuffer[nLen++] = '{';
	for(; pList != NULL; pList = pList->next)
	{
		pItem = &pList->value;

		pTempBuffer[nLen++] = '{';

		if (pItem->bit_mask & R4_Org_qosRequested_present)
		{
			strcpy(&pTempBuffer[nLen], "QoSReq:");
			nLen += strlen("QoSReq:");
			
			nLen += R4QosInformationToStr(&pItem->qosRequested, &pTempBuffer[nLen], 
											nLength - nLen, networkInitiation);
			pTempBuffer[nLen++] = ' ';
		}	

		if (pItem->bit_mask & R4_Org_qosNegotiated_present)
		{
			strcpy(&pTempBuffer[nLen], "QoSNeg:");
			nLen += strlen("QoSNeg:");
			
			nLen += R4QosInformationToStr(&pItem->qosNegotiated, &pTempBuffer[nLen], 
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


	if(nLen >= nLength)
	{
		nLen = 0;
	}

	return nLen;
}


/******************************************************
// Function name: 	R4QosInformationToStr
// Author:        
// Date:          
// Description:   转换QosInformation类型的数据为字符串
// Input:
//       arg1 :R4_Org_QoSInformation *pQosInfo	待转换的数据
//       arg2 :char *pBuffer		转换后数据存放的Buffer
//       arg3 :int nLength			Buffer的可用长度
//       arg4 :R4_Org_NetworkInitiatedPDPContext networkInitiation 代表是MS to netWork还是netWork to MS，Modified By ZhengYuqun 2001/09/28
// Return value: int				转换后字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 此函数从R99同步过来
******************************************************/
int R4QosInformationToStr(R4_Org_QoSInformation *pQosInfo, char *pBuffer, int nLength, char networkInitiation)
{
#define QOSBUFFERLENGTH _BROWSETEMPBUFFERLENGTH

	int nLen = 0;

	//ASSERT((BOOL)(pBuffer && pQosInfo));
	if ((pBuffer == NULL) || (pQosInfo == NULL))
	{
	     return 0;
	}
	
	char *pTempBuffer = pBuffer;
	if (networkInitiation == NETWORKTOMSDIRECTION || networkInitiation == MSTONETWORKDIRECTION)
	{
		ENUMINFO EnumInfo;

		if (!pTempBuffer)
			return 0;
		pTempBuffer[0] = 0;
			
		//跳过一个字节 Modified by ZhengYuqun 2003-09-10  SWPD01505
		S_QOSR4* umtsQosInfo = (S_QOSR4*)&pQosInfo->value[1];
			
		strcpy(pTempBuffer, "ReliabClass(");
		nLen = strlen(pTempBuffer);
		EnumInfo.nCount = QOSR4RELIABILITYENUMINFO_LENGTH; 
		EnumInfo.pInfoTable = QosR4ReliabilityEnumInfo;
		nLen += EnumToStrForQoSR4(&EnumInfo, umtsQosInfo->Qos.ReliabClass, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);
			
		strcpy(&pTempBuffer[nLen], ")DelayClass(");
		nLen += strlen(&pTempBuffer[nLen]);
		EnumInfo.nCount = QOSR4DELAYENUMINFO_LENGTH; 
		EnumInfo.pInfoTable = QoSR4DelayEnumInfo;
		nLen += EnumToStrForQoSR4(&EnumInfo, umtsQosInfo->Qos.DelayClass, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);
	
		strcpy(&pTempBuffer[nLen], ")PrecedClass(");
		nLen += strlen(&pTempBuffer[nLen]);
		EnumInfo.nCount = QOSR4RECEDENCEENUMINFO_LENGTH; 
		EnumInfo.pInfoTable = QoSR4PrecedenceEnumInfo;
		nLen += EnumToStrForQoSR4(&EnumInfo, umtsQosInfo->Qos.PrecedClass, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

		strcpy(&pTempBuffer[nLen], ")PeakThrput(");
		nLen += strlen(&pTempBuffer[nLen]);
		EnumInfo.nCount = QOSR4PEAKTHROUGHPUTENUMINFO_LENGTH; 
		EnumInfo.pInfoTable = QoSR4PeakThroughputEnumInfo;
		nLen += EnumToStrForQoSR4(&EnumInfo, umtsQosInfo->Qos.PeakThrougout, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

		strcpy(&pTempBuffer[nLen], ")MeanThrput(");
		nLen += strlen(&pTempBuffer[nLen]);
		EnumInfo.nCount = QOSR4MEANTHROUGHPUTENUMINFO_LENGTH; 
		EnumInfo.pInfoTable = QoSR4MeanThroughputEnumInfo;
		nLen += EnumToStrForQoSR4(&EnumInfo, umtsQosInfo->Qos.MeanThrougout, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

		strcpy(&pTempBuffer[nLen], ")DelivErrSDUs(");
		nLen += strlen(&pTempBuffer[nLen]);
		EnumInfo.nCount = QOSR4DELIVERYOFERRONEOUSSDUSENUMINFO_LENGTH; 
		EnumInfo.pInfoTable = QoSR4DeliveryOfErroneousSDUsEnumInfo;
		nLen += EnumToStrForQoSR4(&EnumInfo, umtsQosInfo->QosExt.DeliveryOfErroneousSDU, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

		strcpy(&pTempBuffer[nLen], ")DelivOrder(");
		nLen += strlen(&pTempBuffer[nLen]);
		EnumInfo.nCount = QOSR4DELIVERYORDERENUMINFO_LENGTH; 
		EnumInfo.pInfoTable = QoSR4DeliveryOrderEnumInfo;
		nLen += EnumToStrForQoSR4(&EnumInfo, umtsQosInfo->QosExt.DeliveryOrder, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);
			
		strcpy(&pTempBuffer[nLen], ")TraffClass(");
		nLen += strlen(&pTempBuffer[nLen]);
		EnumInfo.nCount = QOSR4TRAFFICCLASSENUMINFO_LENGTH; 
		EnumInfo.pInfoTable = QoSR4TrafficClassEnumInfo;
		nLen += EnumToStrForQoSR4(&EnumInfo, umtsQosInfo->QosExt.TrafficClass, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

		strcpy(&pTempBuffer[nLen], ")MaxSDUSize(");
		nLen += strlen(&pTempBuffer[nLen]);
		EnumInfo.nCount = QOSR4MAXIMUMSDUSIZEENUMINFO_LENGTH; 
		EnumInfo.pInfoTable = QoSR4MaximumSDUSizeEnumInfo;
		if (umtsQosInfo->QosExt.MaximumSDUsize >= 1 && umtsQosInfo->QosExt.MaximumSDUsize <= 150)
			nLen += sprintf(&pTempBuffer[nLen], "%doctets", umtsQosInfo->QosExt.MaximumSDUsize * 10);
		else
			nLen += EnumToStrForQoSR4(&EnumInfo, umtsQosInfo->QosExt.MaximumSDUsize, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

		strcpy(&pTempBuffer[nLen], ")MaxBRUpLk(");
		nLen += strlen(&pTempBuffer[nLen]);
		EnumInfo.nCount = QOSR4MAXIMUMBITRATEFORUPLINKENUMINFO_LENGTH; 
		EnumInfo.pInfoTable = QoSR4MaximumBitRateForUplinkEnumInfo;
		if (umtsQosInfo->QosExt.MaximumBitRateForUplink >= 1 && umtsQosInfo->QosExt.MaximumBitRateForUplink <= 63)
			nLen += sprintf(&pTempBuffer[nLen], "%dkbps", umtsQosInfo->QosExt.MaximumBitRateForUplink);
		else if (umtsQosInfo->QosExt.MaximumBitRateForUplink >= 64 && umtsQosInfo->QosExt.MaximumBitRateForUplink <= 127)  //增加64到127的判断 ZhengYuqun 2003-09-10  SWPD01505
			nLen += sprintf(&pTempBuffer[nLen], "%dkbps", 64 + (umtsQosInfo->QosExt.MaximumBitRateForUplink - 64) * 8);
		else if (umtsQosInfo->QosExt.MaximumBitRateForUplink >= 128 && umtsQosInfo->QosExt.MaximumBitRateForUplink <= 254)
			nLen += sprintf(&pTempBuffer[nLen], "%dkbps", 576 + (umtsQosInfo->QosExt.MaximumBitRateForUplink - 128) * 64);
		else
			nLen += EnumToStrForQoSR4(&EnumInfo, umtsQosInfo->QosExt.MaximumBitRateForUplink, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

		strcpy(&pTempBuffer[nLen], ")MaxBRDnLk(");
		nLen += strlen(&pTempBuffer[nLen]);
		EnumInfo.nCount = QOSR4MAXIMUMBITRATEFORDOWNLINKENUMINFO_LENGTH; 
		EnumInfo.pInfoTable = QoSR4MaximumBitRateForDownlinkEnumInfo;
		if (umtsQosInfo->QosExt.MaximumBitRateForDownlink >= 1 && umtsQosInfo->QosExt.MaximumBitRateForDownlink <= 63)
			nLen += sprintf(&pTempBuffer[nLen], "%dkbps", umtsQosInfo->QosExt.MaximumBitRateForDownlink);
		else if (umtsQosInfo->QosExt.MaximumBitRateForDownlink >= 64 && umtsQosInfo->QosExt.MaximumBitRateForDownlink <= 127)  //增加64到127的判断 ZhengYuqun 2003-09-10  SWPD01505
			nLen += sprintf(&pTempBuffer[nLen], "%dkbps", 64 + (umtsQosInfo->QosExt.MaximumBitRateForDownlink - 64) * 8);
		else if (umtsQosInfo->QosExt.MaximumBitRateForDownlink >= 128 && umtsQosInfo->QosExt.MaximumBitRateForDownlink <= 254)
			nLen += sprintf(&pTempBuffer[nLen], "%dkbps", 576 + (umtsQosInfo->QosExt.MaximumBitRateForDownlink - 128) * 64);
		else
			nLen += EnumToStrForQoSR4(&EnumInfo, umtsQosInfo->QosExt.MaximumBitRateForDownlink, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

		strcpy(&pTempBuffer[nLen], ")SDUErrRatio(");
		nLen += strlen(&pTempBuffer[nLen]);
		EnumInfo.nCount = QOSR4SDUERRORRATIOENUMINFO_LENGTH; 
		EnumInfo.pInfoTable = QoSR4SDUErrorRatioEnumInfo;
		nLen += EnumToStrForQoSR4(&EnumInfo, umtsQosInfo->QosExt.SDUErrorRatio, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

		strcpy(&pTempBuffer[nLen], ")ResidBER(");
		nLen += strlen(&pTempBuffer[nLen]);
		EnumInfo.nCount = QOSR4RESIDUALBERENUMINFO_LENGTH; 
		EnumInfo.pInfoTable = QoSR4ResidualBEREnumInfo;
		nLen += EnumToStrForQoSR4(&EnumInfo, umtsQosInfo->QosExt.ResidualBER, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

		strcpy(&pTempBuffer[nLen], ")TraffPrior(");
		nLen += strlen(&pTempBuffer[nLen]);
		EnumInfo.nCount = QOSR4TRAFFICHANDLINGPRIORITYENUMINFO_LENGTH; 
		EnumInfo.pInfoTable = QoSR4TrafficHandlingPriorityEnumInfo;
		nLen += EnumToStrForQoSR4(&EnumInfo, umtsQosInfo->QosExt.TrafficHandlingPriority, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

		strcpy(&pTempBuffer[nLen], ")TransDelay(");
		nLen += strlen(&pTempBuffer[nLen]);
		EnumInfo.nCount = QOSR4TRANSFERDELAYENUMINFO_LENGTH; 
		EnumInfo.pInfoTable = QoSR4TransferDelayEnumInfo;
		if (umtsQosInfo->QosExt.TransferDelay >= 1 && umtsQosInfo->QosExt.TransferDelay <= 15)
			nLen += sprintf(&pTempBuffer[nLen], "%dms", umtsQosInfo->QosExt.TransferDelay * 10);
		else if (umtsQosInfo->QosExt.TransferDelay >= 16 && umtsQosInfo->QosExt.TransferDelay <= 31)
			nLen += sprintf(&pTempBuffer[nLen], "%dms", 200 + (umtsQosInfo->QosExt.TransferDelay - 16) * 50);
		else if (umtsQosInfo->QosExt.TransferDelay >= 32 && umtsQosInfo->QosExt.TransferDelay <= 62)
			nLen += sprintf(&pTempBuffer[nLen], "%dms", 1000 + (umtsQosInfo->QosExt.TransferDelay - 32) * 100);
		else
			nLen += EnumToStrForQoSR4(&EnumInfo, umtsQosInfo->QosExt.TransferDelay, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

		strcpy(&pTempBuffer[nLen], ")GntBRUpLk(");
		nLen += strlen(&pTempBuffer[nLen]);
		EnumInfo.nCount = QOSR4GUARANTEEDBITRATEFORUPLINKENUMINFO_LENGTH; 
		EnumInfo.pInfoTable = QoSR4GuaranteedBitRateForUplinkEnumInfo;
		if (umtsQosInfo->QosExt.GuaranteedBitRateForUplink >= 1 && umtsQosInfo->QosExt.GuaranteedBitRateForUplink <= 63)
			nLen += sprintf(&pTempBuffer[nLen], "%dkbps", umtsQosInfo->QosExt.GuaranteedBitRateForUplink);
		else if (umtsQosInfo->QosExt.GuaranteedBitRateForUplink >= 64 && umtsQosInfo->QosExt.GuaranteedBitRateForUplink <= 127)  //增加64到127的判断 ZhengYuqun 2003-09-10  SWPD01505
			nLen += sprintf(&pTempBuffer[nLen], "%dkbps", 64 + (umtsQosInfo->QosExt.GuaranteedBitRateForUplink - 64) * 8);			
		else if (umtsQosInfo->QosExt.GuaranteedBitRateForUplink >= 128 && umtsQosInfo->QosExt.GuaranteedBitRateForUplink <= 254)
			nLen += sprintf(&pTempBuffer[nLen], "%dkbps", 576 + (umtsQosInfo->QosExt.GuaranteedBitRateForUplink - 128) * 64);
		else
			nLen += EnumToStrForQoSR4(&EnumInfo, umtsQosInfo->QosExt.GuaranteedBitRateForUplink, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);

		strcpy(&pTempBuffer[nLen], ")GntBRDnLk(");
		nLen += strlen(&pTempBuffer[nLen]);
		EnumInfo.nCount = QOSR4GUARANTEEDBITRATEFORDOWNLINKENUMINFO_LENGTH; 
		EnumInfo.pInfoTable = QoSR4GuaranteedBitRateForDownlinkEnumInfo;
		if (umtsQosInfo->QosExt.GuaranteedBitRateForDownlink >= 1 && umtsQosInfo->QosExt.GuaranteedBitRateForDownlink <= 63)
			nLen += sprintf(&pTempBuffer[nLen], "%dkbps", umtsQosInfo->QosExt.GuaranteedBitRateForDownlink);
		else if (umtsQosInfo->QosExt.GuaranteedBitRateForDownlink >= 64 && umtsQosInfo->QosExt.GuaranteedBitRateForDownlink <= 127)  //增加64到127的判断 ZhengYuqun 2003-09-10  SWPD01505
			nLen += sprintf(&pTempBuffer[nLen], "%dkbps", 64 + (umtsQosInfo->QosExt.GuaranteedBitRateForDownlink - 64) * 8);
		else if (umtsQosInfo->QosExt.GuaranteedBitRateForDownlink >= 128 && umtsQosInfo->QosExt.GuaranteedBitRateForDownlink <= 254)
			nLen += sprintf(&pTempBuffer[nLen], "%dkbps", 576 + (umtsQosInfo->QosExt.GuaranteedBitRateForDownlink - 128) * 64);
		else
			nLen += EnumToStrForQoSR4(&EnumInfo, umtsQosInfo->QosExt.GuaranteedBitRateForDownlink, &pTempBuffer[nLen], (nLength - nLen), networkInitiation);
		pTempBuffer[nLen++] = ')';
			
		pTempBuffer[nLen] = 0;
		
		if (nLen >= nLength)
			nLen = 0;
	}
	else
		nLen = HexOctecToStr((LPOCTECSTRING)pQosInfo, pBuffer, nLength - nLen, "0x", FALSE);
	
	return nLen;
}


/*******************************************************************************
  Function        : R4DiagnosticsToStr
  Description     : 转换R4_Org_Diagnostics类型的数据为字符串
  Input           : R4_Org_Diagnostics *pDiagnostics 待转换的数据
                  : char *pBuffer转换后数据存放的Buffer
                  : int nLength Buffer的可用长度
  Output          :
  Return          : int 转换后字符串的长度
  Calls           :
  Called by       :

  History         :
  1 Date          : 2003-6-6
    Author        : Gan Zhiliang
    Modification  : Created file
*******************************************************************************/
int R4DiagnosticsToStr(R4_Org_Diagnostics *pDiagnostics, char *pBuffer, int nLength)
{

    int nLen = 0;

    //ASSERT((BOOL)(pBuffer && pDiagnostics));
    if ((pDiagnostics == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    char *pTempBuffer = pBuffer;

    if (!pTempBuffer)
        return 0;

    switch (pDiagnostics->choice)
    {
    case R4_Org_gsm0408Cause_chosen:
        {
            strcpy(pTempBuffer, "GSM0408Cause:");
            nLen += strlen("GSM0408Cause:");
            nLen += NumToDecStr(pDiagnostics->u.gsm0408Cause, &pTempBuffer[nLen],
                nLength - nLen, FALSE);
        }
        break;
    case R4_Org_gsm0902MapErrorValue_chosen :
        {
            strcpy(pTempBuffer, "GSM0902MapErrorValue:");
            nLen += strlen("GSM0902MapErrorValue:");
            nLen += NumToDecStr(pDiagnostics->u.gsm0902MapErrorValue, &pTempBuffer[nLen],
                nLength - nLen, FALSE);
        }
        break;
    case R4_Org_ccittQ767Cause_chosen:
        {
            strcpy(pTempBuffer, "ccittQ767Cause:");
            nLen += strlen("ccittQ767Cause:");
            nLen += NumToDecStr(pDiagnostics->u.ccittQ767Cause, &pTempBuffer[nLen],
                nLength - nLen, FALSE);
        }
        break;
    case R4_Org_networkSpecificCause_chosen:
        {
            strcpy(pTempBuffer, "NetworkSpecificCause:(");
            nLen += strlen("NetworkSpecificCause:(");
            nLen += R4RecordExtensionToStr(&pDiagnostics->u.networkSpecificCause, &pTempBuffer[nLen],
                nLength - nLen);
            pTempBuffer[nLen++] = ')';
        }
        break;
    case R4_Org_manufacturerSpecificCause_chosen:
        {
            strcpy(pTempBuffer, "ManufacturerSpecificCause:(");
            nLen += strlen("ManufacturerSpecificCause:(");
            nLen += R4RecordExtensionToStr(&pDiagnostics->u.manufacturerSpecificCause, &pTempBuffer[nLen],
                nLength - nLen);
            pTempBuffer[nLen++] = ')';
        }
        break;
    case R4_Org_positionMethodFailureCause_chosen:
       {
       	strcpy(pTempBuffer, "positionMethodFailureCause:(");
       	nLen += strlen("positionMethodFailureCause:(");
       	ENUMINFO EnumInfo;
       	EnumInfo.nCount = DEFAULTR4_Org_PositionMethodFailureCauseEnumInfo_LENGTH;
       	EnumInfo.pInfoTable = R4_Org_PositionMethodFailureCauseEnumInfo;
       	nLen += EnumToStr(&EnumInfo, pDiagnostics->u.positionMethodFailureCause, &pTempBuffer[nLen],
       	     nLength - nLen);
       	pTempBuffer[nLen++] = ')';
       }
       break;
    case R4_Org_unauthorizedLCSClientCause_chosen:
       {
       	strcpy(pTempBuffer, "unauthorizedLCSClientCause:(");
       	nLen += strlen("unauthorizedLCSClientCause:(");
       	ENUMINFO EnumInfo;
       	EnumInfo.nCount = DEFAULTR4_Org_UnauthorizedLCSClientCauseEnumInfo_LENGTH;
       	EnumInfo.pInfoTable = R4_Org_UnauthorizedLCSClientCauseEnumInfo;
       	nLen += EnumToStr(&EnumInfo, pDiagnostics->u.unauthorizedLCSClientCause, &pTempBuffer[nLen],
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

/*******************************************************************************
  Function        : R4RecordExtensionToStr
  Description     : 转换R4_Org_ManagementExtension类型的数据为字符串
  Input           : R4_Org_ManagementExtension *pData 待转换的数据
                  : char *pBuffer 转换后数据存放的Buffer
                  : int nLength Buffer的可用长度
  Output          :
  Return          : int 转换后字符串的长度
  Calls           :
  Called by       :

  History         :
  1 Date          : 2003-6-9
    Author        : Gan Zhiliang
    Modification  : Created file
*******************************************************************************/
int R4RecordExtensionToStr(R4_Org_ManagementExtension *pData, char *pBuffer, int nLength)
{

    int nLen = 0;

    pBuffer[0] = 0;

    //ASSERT((BOOL)(pBuffer && pData));
    if ((pData == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    char *pTempBuffer = pBuffer;

    strcpy(pTempBuffer, "Identifier:");
    nLen += strlen("Identifier:");
    nLen += HexOctecToStr(pData->identifier.length, pData->identifier.value, &pTempBuffer[nLen],
        nLength - nLen, "0x");   //问题单SWPD05340 modified by YanYong 2004-05-27
    pTempBuffer[nLen++] = ' ';

    if (pData->bit_mask & R4_Org_significance_present)
    {
        strcpy(&pTempBuffer[nLen], "Significance:");
        nLen += strlen("Significance:");
        nLen += BoolToStr(pData->significance, &pTempBuffer[nLen],nLength - nLen, 0);
        pTempBuffer[nLen++] = ' ';
    }

    strcpy(&pTempBuffer[nLen], "Information:");
    nLen += strlen("Information:");
    nLen += HexOctecToStr(pData->information.length, pData->information.value, &pTempBuffer[nLen],
        nLength - nLen, "0x");  //问题单SWPD05340 modified by YanYong 2004-05-27

    pTempBuffer[nLen] = 0;

    if(nLen >= nLength)
    {
        nLen = 0;
    }

    return nLen;
}


/*******************************************************************************
  Function        : R4RecordExtensionsToStr
  Description     : 转换R4_Org_ManagementExtensions类型的数据为字符串
  Input           : struct R4_Org_ManagementExtensions_ *pData
                  : char *pBuffer
                  : int nLength
  Output          :
  Return          : int
  Calls           :
  Called by       :

  History         :
  1 Date          : 2003-6-9
    Author        : Gan Zhiliang
    Modification  : Created file
*******************************************************************************/
int R4RecordExtensionsToStr(struct R4_Org_ManagementExtensions_  *pData, char *pBuffer, int nLength)
{
    int nLen = 0;
    int nTempLen = 0;

    //ASSERT((BOOL)(pData && pBuffer));
    if ((pData == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    for( ; pData != NULL; pData = pData->next)
    {
        if (nLength > nLen)
            pBuffer[nLen++] = '{';
        else
        {
            pBuffer[0] = 0;
            nLen = 0;
            break;
        }

        nTempLen = R4RecordExtensionToStr(&pData->value, &pBuffer[nLen], nLength - nLen);
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


/*******************************************************************************
Function        : R4ChangeLocationToStr
Description     : 转换R4_Org_ChangeLocation类型的数据为字符串
Input           : R4_Org_ChangeLocation *pData 待转换的数据
: char *pBuffer 转换后数据存放的Buffer
: int nLength  Buffer可用的长度
Output          :
Return          : int 转换后字符串的长度
Calls           :
Called by       :

  History         :
  1 Date          : 2003-06-06
  Author        : Gan Zhiliang
  Modification  : Created file
*******************************************************************************/
int R4ChangeLocationToStr(R4_Org_ChangeLocation *pData, char *pBuffer, int nLength)
{

    int nLen = 0;

    //ASSERT((BOOL)(pBuffer && pData));
    if ((pData == NULL) || (pBuffer == NULL))
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
        nLength - nLen, "0x");
    pTempBuffer[nLen++] = ' ';

    if (pData->bit_mask & R4_Org_cellId_present)
    {
        strcpy(&pTempBuffer[nLen], "CellID:");
        nLen += strlen("CellID:");
        nLen += HexOctecToStr((LPOCTECSTRING)&pData->cellId, &pTempBuffer[nLen],
            nLength - nLen, "0x",FALSE);
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

/*******************************************************************************
Function        : R4ChangeLocationsToStr
Description     : 转换R4_Org_SGSNMMRecord::R4_Org__seqof3类型的数据为字符串
Input           : R4_Org_SGSNMMRecord::R4_Org__seqof3 *pData 待转换的数据
: char *pBuffer 转换后数据存放的Buffer
: int nLength Buffer可用的长度
Output          :
Return          : int 转换后字符串的长度
Calls           :
Called by       :

  History         :
  1 Date          : 2003-06-06
  Author        : Gan Zhiliang
  Modification  : Created file
*******************************************************************************/
int R4ChangeLocationsToStr(R4_Org_SGSNMMRecord::R4_Org__seqof3 *pData, char *pBuffer, int nLength)
{
    int nLen = 0;
    int nTempLen = 0;

    //ASSERT((BOOL)(pBuffer && pData));
    if ((pData == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    pBuffer[nLen++] = '{';
    for (; pData != NULL; pData = pData->next)
    {
        if (nLength > nLen)
            pBuffer[nLen++] = '{';
        else
        {
            pBuffer[0] = 0;
            return 0;
        }

        nTempLen = R4ChangeLocationToStr(&pData->value, &pBuffer[nLen], nLength - nLen);
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

/*****************************************************************************
Prototype    : R4ChargingCharacteristicsToStr
Description  : 转换R4_Org_ChargingCharacteristics类型的数据为字符串
Input        : R4_Org_ChargingCharacteristics *pData  待转换的数据
char *pBuffer  转换后数据存放的Buffer
int nLength  Buffer的可用长度
Output       : None
Return Value : int 转换后字符串的长度
Calls        :
Called By    :

  History        :
  1.Date         : 2003-06-06
  Author       : Gan Zhiliang
  Modification : Created function

*****************************************************************************/
int R4ChargingCharacteristicsToStr(R4_Org_ChargingCharacteristics *pData, char *pBuffer, int nLength)
{
    //#define CCBUFFERLENGTH 65536   //Commented by ZhengYuqun 2001/09/26

    int nLen = 0;

    //ASSERT((BOOL)(pData && pBuffer));
    if ((pData == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    pBuffer[0] = 0;

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
            strcpy(pTempBuffer, "Prepaid service");
            nLen = strlen("Prepaid service");
        }
        break;
    case 8:
        {
            strcpy(pTempBuffer, "Normal billing");
            nLen = strlen("Normal billing");
        }
        break;
    default:
        {
            sprintf(pTempBuffer, "Unknow value(0x%02X%02X)", pData->value[0], pData->value[1]);
            nLen = strlen(pTempBuffer);
        }
        break;
    }

    if(nLen >= nLength)
    {
        nLen = 0;
    }

    return nLen;
}



/*****************************************************************************
 Prototype    : GetTextValueOf_Q_3GR4SGSNPDP
 Description  : 处理S-CDR话单查询解释的函数
 Input        : char *pValue   话单数据
                char *pBuffer  存放结果的数据区
                int nLength    数据区的有效长度
 Output       : None结果字符串的长度
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2003-06-06
    Author       : Gan Zhiliang
    Modification : Created function

*****************************************************************************/
int GetTextValueOf_Q_3GR4SGSNPDP(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues)
{

    R4_Org_SGSNPDPRecord *pBill = (R4_Org_SGSNPDPRecord*)pValue;
    int nLen = 0;
    int nTempLen = 0;

    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Org_CallEventRecordType_sgsnPDPRecord)
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
                EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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
                EnumInfo.nCount = R4CAUSEFORRECCLOSINGENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R4CauseForRecClosingEnumInfo;
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
                if (pBill->bit_mask & R4_Org_SGSNPDPRecord_recordSequenceNumber_present)
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
                if (pBill->bit_mask & R4_Org_SGSNPDPRecord_servedMSISDN_present)
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
                R4ChargingCharacteristicsToStr(&pBill->chargingCharacteristics, pTempBuffer, nLength);
                value.sFieldValue = pTempBuffer;

                nLen++;

            }
            //systemType
            else if(value.sFieldName.compare("systemType") == 0)
            {
                if (pBill->bit_mask & R4_Org_SGSNPDPRecord_systemType_present)
                {
                    ENUMINFO EnumInfo;
                    EnumInfo.nCount = R4SYSTEMTYPEENUMINFO_LENGTH;
                    EnumInfo.pInfoTable =  R4SYSTEMTYPEENUMINFOEnumInfo;
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
            //servedPDPAddress
            else if(value.sFieldName.compare("servedPDPAddress") == 0)
            {
                if (pBill->bit_mask & R4_Org_SGSNPDPRecord_servedPDPAddress_present)
                {
                    R43GPDPAddrToStr(&pBill->servedPDPAddress,
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
            //chargingID
            else if(value.sFieldName.compare("chargingID") == 0)
            {
                NumToDecStr(pBill->chargingID, pTempBuffer,
                            nLength, FALSE);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
        }
    }

    return nLen;
}


/*****************************************************************************
 Prototype    : GetTextValueOf_Q_3GR4GGSNPDP
 Description  : 处理G-CDR话单浏览解释的函数
 Input        : char *pValue   话单数据
                char *pBuffer  存放结果的数据区
                int nLength    数据区的有效长度
 Output       : None
 Return Value : 结果字符串的长度
 Calls        :
 Called By    :

  History        :
  1.Date         : 2003-06-06
    Author       : Gan Zhiliang
    Modification : Created function

*****************************************************************************/
int GetTextValueOf_Q_3GR4GGSNPDP(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues)
{

    R4_Org_GGSNPDPRecord *pBill = (R4_Org_GGSNPDPRecord*)pValue;
    int nLen = 0;
    int nTempLen = 0;

    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Org_CallEventRecordType_ggsnPDPRecord)
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
                EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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
                EnumInfo.nCount = R4CAUSEFORRECCLOSINGENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R4CauseForRecClosingEnumInfo;
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
                if (pBill->bit_mask & R4_Org_GGSNPDPRecord_recordSequenceNumber_present)
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
                if (pBill->bit_mask & R4_Org_GGSNPDPRecord_servedMSISDN_present)
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
                R4ChargingCharacteristicsToStr(&pBill->chargingCharacteristics, pTempBuffer, nLength);
                value.sFieldValue = pTempBuffer;

                nLen++;
            }
            //servedPDPAddress
            else if(value.sFieldName.compare("servedPDPAddress") == 0)
            {
                if (pBill->bit_mask & R4_Org_GGSNPDPRecord_servedPDPAddress_present)
                {
                    R43GPDPAddrToStr(&pBill->servedPDPAddress,
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
            //chargingID
            else if(value.sFieldName.compare("chargingID") == 0)
            {
                NumToDecStr(pBill->chargingID, pTempBuffer,
                    nLength, FALSE);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
        }
    }

    return nLen;
}

/*****************************************************************************
 Prototype    : GetTextValueOf_Q_3GR4SGSNMM
 Description  : 处理M-CDR话单查询解释的函数
 Input        : char *pValue   话单数据
                char *pBuffer  存放结果的数据区
                int nLength    数据区的有效长度
 Output       : None
 Return Value : 结果字符串的长度
 Calls        :
 Called By    :

  History        :
  1.Date         : 2003-06-06
    Author       : Gan Zhiliang
    Modification : Created function

*****************************************************************************/
int GetTextValueOf_Q_3GR4SGSNMM(char *pValue,
                                char *pBuffer,
                                int nLength,
                                QUERY_COND_FIELDS_VALUE* pValues)
{

    R4_Org_SGSNMMRecord *pBill = (R4_Org_SGSNMMRecord*)pValue;
    int nLen = 0;
    int nTempLen = 0;

    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Org_CallEventRecordType_sgsnMMRecord)
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
                EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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
                if (pBill->bit_mask & R4_Org_duration_present)
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
                EnumInfo.nCount = R4CAUSEFORRECCLOSINGENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R4CauseForRecClosingEnumInfo;
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
                if (pBill->bit_mask & R4_Org_SGSNMMRecord_recordSequenceNumber_present)
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
                if (pBill->bit_mask & R4_Org_SGSNMMRecord_servedMSISDN_present)
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
                R4ChargingCharacteristicsToStr(&pBill->chargingCharacteristics, pTempBuffer, nLength);
                value.sFieldValue = pTempBuffer;

                nLen++;

            }
            //systemType
            else if(value.sFieldName.compare("systemType") == 0)
            {
                if (pBill->bit_mask & R4_Org_SGSNMMRecord_systemType_present)
                {
                    ENUMINFO EnumInfo;
                    EnumInfo.nCount = R4SYSTEMTYPEENUMINFO_LENGTH;
                    EnumInfo.pInfoTable =  R4SYSTEMTYPEENUMINFOEnumInfo;
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


/*****************************************************************************
 Prototype    : GetTextValueOf_Q_3GR4SGSNSMO
 Description  : 处理S-SMO-CDR话单查询解释的函数
 Input        : char *pValue   话单数据
                char *pBuffer  存放结果的数据区
                int nLength    数据区的有效长度
 Output       : None
 Return Value : 结果字符串的长度
 Calls        :
 Called By    :

  History        :
  1.Date         : 2003-06-06
    Author       : Gan Zhiliang
    Modification : Created function

*****************************************************************************/
int GetTextValueOf_Q_3GR4SGSNSMO(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues)
{

    R4_Org_SGSNSMORecord *pBill = (R4_Org_SGSNSMORecord*)pValue;
    int nLen = 0;

    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Org_CallEventRecordType_sgsnSMORecord)
        return 0;

    if(NULL != pValues)
    {
        char* pTempBuffer = pBuffer;

        for(int i = 0; i < pValues->size(); i++)
        {
            SQUERY_COND_FIELDS_VALUE& value = (*pValues)[i];

            //recordType
            if(value.sFieldName.compare("recordType") == 0)
            {
                ENUMINFO EnumInfo;
                EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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
                if (pBill->bit_mask & R4_Org_SGSNSMORecord_servedMSISDN_present)
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
            //eventTimeStamp
            else if(value.sFieldName.compare("eventTimeStamp") == 0)
            {
                TimeStampToStr((LPTIMESTAMP)&pBill->eventTimeStamp,
                               pTempBuffer,
                               nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //chargingCharacteristics
            else if(value.sFieldName.compare("chargingCharacteristics") == 0)
            {
                R4ChargingCharacteristicsToStr(&pBill->chargingCharacteristics, pTempBuffer, nLength);
                value.sFieldValue = pTempBuffer;

                nLen++;

            }
            //systemType
            else if(value.sFieldName.compare("systemType") == 0)
            {
                if (pBill->bit_mask & R4_Org_SGSNSMORecord_systemType_present)
                {
                    ENUMINFO EnumInfo;
                    EnumInfo.nCount = R4SYSTEMTYPEENUMINFO_LENGTH;
                    EnumInfo.pInfoTable =  R4SYSTEMTYPEENUMINFOEnumInfo;
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


/*****************************************************************************
 Prototype    : GetTextValueOf_Q_3GR4SGSNSMT
 Description  : 处理S-SMT-CDR话单查询解释的函数
 Input        : char *pValue   话单数据
                char *pBuffer  存放结果的数据区
                int nLength    数据区的有效长度
 Output       : None
 Return Value : 结果字符串的长度
 Calls        :
 Called By    :

  History        :
  1.Date         : 2003-06-06
    Author       : Gan Zhiliang
    Modification : Created function

*****************************************************************************/
int GetTextValueOf_Q_3GR4SGSNSMT(char * pValue,
                                 char * pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues)
{

    R4_Org_SGSNSMTRecord *pBill = (R4_Org_SGSNSMTRecord*)pValue;
    int nLen = 0;

    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Org_CallEventRecordType_sgsnSMTRecord)
        return 0;

    if(NULL != pValues)
    {
        char* pTempBuffer = pBuffer;

        for(int i = 0; i < pValues->size(); i++)
        {
            SQUERY_COND_FIELDS_VALUE& value = (*pValues)[i];

            //recordType
            if(value.sFieldName.compare("recordType") == 0)
            {
                ENUMINFO EnumInfo;
                EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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
                if (pBill->bit_mask & R4_Org_SGSNSMTRecord_servedMSISDN_present)
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
            //eventTimeStamp
            else if(value.sFieldName.compare("eventTimeStamp") == 0)
            {
                TimeStampToStr((LPTIMESTAMP)&pBill->eventTimeStamp,
                               pTempBuffer,
                               nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //chargingCharacteristics
            else if(value.sFieldName.compare("chargingCharacteristics") == 0)
            {
                R4ChargingCharacteristicsToStr(&pBill->chargingCharacteristics, pTempBuffer, nLength);
                value.sFieldValue = pTempBuffer;

                nLen++;

            }
            //systemType
            else if(value.sFieldName.compare("systemType") == 0)
            {
                if (pBill->bit_mask & R4_Org_SGSNSMTRecord_systemType_present)
                {
                    ENUMINFO EnumInfo;
                    EnumInfo.nCount = R4SYSTEMTYPEENUMINFO_LENGTH;
                    EnumInfo.pInfoTable =  R4SYSTEMTYPEENUMINFOEnumInfo;
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

/*****************************************************************************
 Prototype    : GetTextValueOf_Q_3GR4SGSNLCT
 Description  : 处理SGSN-LCT话单查询解释的函数
 Input        : char *pValue   话单数据
                char *pBuffer  存放结果的数据区
                int nLength    数据区的有效长度
 Output       : None
 Return Value : 结果字符串的长度
 Calls        :
 Called By    :

  History        :
  1.Date         : 2003-06-06
    Author       : Gan Zhiliang
    Modification : Created function

*****************************************************************************/
int GetTextValueOf_Q_3GR4SGSNLCT(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues)
{

    R4_Org_SGSNLCTRecord *pBill = (R4_Org_SGSNLCTRecord*)pValue;
    int nLen = 0;

    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Org_sgsnMtLCSRecord)
        return 0;

    if(NULL != pValues)
    {
        char* pTempBuffer = pBuffer;
        
        for(int i = 0; i < pValues->size(); i++)
        {
            SQUERY_COND_FIELDS_VALUE& value = (*pValues)[i];

            //recordType
            if(value.sFieldName.compare("recordType") == 0)
            {
                ENUMINFO EnumInfo;
                EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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
                if (pBill->bit_mask & R4_Org_SGSNLCTRecord_servedMSISDN_present)
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
            //eventTimeStamp
            else if(value.sFieldName.compare("eventTimeStamp") == 0)
            {
                TimeStampToStr((LPTIMESTAMP)&pBill->eventTimeStamp,
                               pTempBuffer,
                               nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //chargingCharacteristics
            else if(value.sFieldName.compare("chargingCharacteristics") == 0)
            {
                R4ChargingCharacteristicsToStr(&pBill->chargingCharacteristics, pTempBuffer, nLength);
                value.sFieldValue = pTempBuffer;

                nLen++;

            }
            //systemType
            else if(value.sFieldName.compare("systemType") == 0)
            {
                if (pBill->bit_mask & R4_Org_SGSNLCTRecord_systemType_present)
                {
                    ENUMINFO EnumInfo;
                    EnumInfo.nCount = R4SYSTEMTYPEENUMINFO_LENGTH;
                    EnumInfo.pInfoTable =  R4SYSTEMTYPEENUMINFOEnumInfo;
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

/*****************************************************************************
 Prototype    : GetTextValueOf_Q_3GR4SGSNLCO
 Description  : 处理SGSN-LCOR话单查询解释的函数
 Input        : char *pValue   话单数据
                char *pBuffer  存放结果的数据区
                int nLength    数据区的有效长度
 Output       : None
 Return Value : 结果字符串的长度
 Calls        :
 Called By    :

  History        :
  1.Date         : 2003-06-06
    Author       : Gan Zhiliang
    Modification : Created function

*****************************************************************************/
int GetTextValueOf_Q_3GR4SGSNLCO(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues)
{

    R4_Org_SGSNLCORecord *pBill = (R4_Org_SGSNLCORecord*)pValue;
    int nLen = 0;

    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Org_sgsnMoLCSRecord)
        return 0;

    if(NULL != pValues)
    {
        char* pTempBuffer = pBuffer;

        for(int i = 0; i < pValues->size(); i++)
        {
            SQUERY_COND_FIELDS_VALUE& value = (*pValues)[i];

            //recordType
            if(value.sFieldName.compare("recordType") == 0)
            {
                ENUMINFO EnumInfo;
                EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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
                if (pBill->bit_mask & R4_Org_SGSNLCORecord_servedMSISDN_present)
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
            //eventTimeStamp
            else if(value.sFieldName.compare("eventTimeStamp") == 0)
            {
                TimeStampToStr((LPTIMESTAMP)&pBill->eventTimeStamp,
                               pTempBuffer,
                               nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //chargingCharacteristics
            else if(value.sFieldName.compare("chargingCharacteristics") == 0)
            {
                R4ChargingCharacteristicsToStr(&pBill->chargingCharacteristics, pTempBuffer, nLength);
                value.sFieldValue = pTempBuffer;

                nLen++;

            }
            //systemType
            else if(value.sFieldName.compare("systemType") == 0)
            {
                if (pBill->bit_mask & R4_Org_SGSNLCORecord_systemType_present)
                {
                    ENUMINFO EnumInfo;
                    EnumInfo.nCount = R4SYSTEMTYPEENUMINFO_LENGTH;
                    EnumInfo.pInfoTable =  R4SYSTEMTYPEENUMINFOEnumInfo;
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

/*****************************************************************************
 Prototype    : GetTextValueOf_Q_3GR4SGSNLCN
 Description  : 处理S-SMT-CDR话单查询解释的函数
 Input        : char *pValue   话单数据
                char *pBuffer  存放结果的数据区
                int nLength    数据区的有效长度
 Output       : None
 Return Value : 结果字符串的长度
 Calls        :
 Called By    :

  History        :
  1.Date         : 2003-06-06
    Author       : Gan Zhiliang
    Modification : Created function

*****************************************************************************/
int GetTextValueOf_Q_3GR4SGSNLCN(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues)
{

    R4_Org_SGSNLCNRecord *pBill = (R4_Org_SGSNLCNRecord*)pValue;
    int nLen = 0;

    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Org_sgsnNiLCSRecord)
        return 0;

    if(NULL != pValues)
    {
        char* pTempBuffer = pBuffer;

        for(int i = 0; i < pValues->size(); i++)
        {
            SQUERY_COND_FIELDS_VALUE& value = (*pValues)[i];

            //recordType
            if(value.sFieldName.compare("recordType") == 0)
            {
                ENUMINFO EnumInfo;
                EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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
                if (pBill->bit_mask & R4_Org_servedIMSI_present)
                {
                    CvtBCDOctecToStr((LPOCTECSTRING)&pBill->servedIMSI, 
                        pTempBuffer, nLength);
                    value.sFieldValue = pTempBuffer;
                }
                else
                {
                    value.sFieldValue = _EMPTYSTRING;
                }
                nLen++;
            }
            //</SWPD04788> 
            //servedMSISDN
            else if(value.sFieldName.compare("servedMSISDN") == 0)
            {
                if (pBill->bit_mask & R4_Org_SGSNLCNRecord_servedMSISDN_present)
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
            //eventTimeStamp
            else if(value.sFieldName.compare("eventTimeStamp") == 0)
            {
                TimeStampToStr((LPTIMESTAMP)&pBill->eventTimeStamp,
                               pTempBuffer,
                               nLength);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
            //chargingCharacteristics
            else if(value.sFieldName.compare("chargingCharacteristics") == 0)
            {
                R4ChargingCharacteristicsToStr(&pBill->chargingCharacteristics, pTempBuffer, nLength);
                value.sFieldValue = pTempBuffer;

                nLen++;

            }
            //systemType
            else if(value.sFieldName.compare("systemType") == 0)
            {
                if (pBill->bit_mask & R4_Org_SGSNLCNRecord_systemType_present)
                {
                    ENUMINFO EnumInfo;
                    EnumInfo.nCount = R4SYSTEMTYPEENUMINFO_LENGTH;
                    EnumInfo.pInfoTable =  R4SYSTEMTYPEENUMINFOEnumInfo;
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



/*****************************************************************************
 Prototype    : GetTextValueOf_Q_3GR4LARGESGSNPDP
 Description  : 处理合并后S-CDR话单查询解释的函数
 Input        : char * pValue   话单数据
                char * pBuffer  存放结果的数据区
                int nLength     数据区的有效长度
 Output       : None
 Return Value : 结果字符串的长度
 Calls        :
 Called By    :

  History        :
  1.Date         : 2003-06-06
    Author       : Gan Zhiliang
    Modification : Created function

*****************************************************************************/
int GetTextValueOf_Q_3GR4LARGESGSNPDP(char * pValue,
                                      char * pBuffer,
                                      int nLength,
                                      QUERY_COND_FIELDS_VALUE* pValues)
{
    R4_Fin_SGSNPDPRecord *pBill = (R4_Fin_SGSNPDPRecord*)pValue;
    int nLen = 0;
    int nTempLen = 0;

    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Fin_CallEventRecordType_sgsnPDPRecord)
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
                EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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
                EnumInfo.nCount = R4CAUSEFORRECCLOSINGENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R4CauseForRecClosingEnumInfo;
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
                if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_servedMSISDN_present)
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
                R4ChargingCharacteristicsToStr((R4_Org_ChargingCharacteristics *)(&pBill->chargingCharacteristics), pTempBuffer, nLength);
                value.sFieldValue = pTempBuffer;

                nLen++;

            }
            //systemType
            else if(value.sFieldName.compare("systemType") == 0)
            {
                if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_systemType_present)
                {
                    ENUMINFO EnumInfo;
                    EnumInfo.nCount = R4SYSTEMTYPEENUMINFO_LENGTH;
                    EnumInfo.pInfoTable =  R4SYSTEMTYPEENUMINFOEnumInfo;
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
            //servedPDPAddress
            else if(value.sFieldName.compare("servedPDPAddress") == 0)
            {
                if (pBill->bit_mask & R4_Fin_SGSNPDPRecord_servedPDPAddress_present)
                {
                    R43GPDPAddrToStr((R4_Org_PDPAddress *)&pBill->servedPDPAddress,
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
            //chargingID
            else if(value.sFieldName.compare("chargingID") == 0)
            {
                NumToDecStr(pBill->chargingID, pTempBuffer,
                    nLength, FALSE);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
        }
    }

    return nLen;
}


/*****************************************************************************
 Prototype    : GetTextValueOf_Q_3GR4LARGEGGSNPDP
 Description  : 处理合并后G-CDR话单浏览解释的函数
 Input        : char *pValue   话单数据
                char *pBuffer  存放结果的数据区
                int nLength    数据区的有效长度
 Output       : None
 Return Value : 结果字符串的长度
 Calls        :
 Called By    :

  History        :
  1.Date         : 2003-06-06
    Author       : Gan Zhiliang
    Modification : Created function

*****************************************************************************/
int GetTextValueOf_Q_3GR4LARGEGGSNPDP(char *pValue,
                                      char *pBuffer,
                                      int nLength,
                                      QUERY_COND_FIELDS_VALUE* pValues)
{
    R4_Fin_GGSNPDPRecord *pBill = (R4_Fin_GGSNPDPRecord*)pValue;
    int nLen = 0;
    int nTempLen = 0;

    if ((pValue == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    if (pBill->recordType != R4_Fin_CallEventRecordType_ggsnPDPRecord)
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
                EnumInfo.nCount = R4_440_RECORDE_TYPE_ENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R4_440_Recorde_Type_EnumInfo;
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
                EnumInfo.nCount = R4CAUSEFORRECCLOSINGENUNINFO_LENGTH;
                EnumInfo.pInfoTable = R4CauseForRecClosingEnumInfo;
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
                if (pBill->bit_mask & R4_Fin_GGSNPDPRecord_servedMSISDN_present)
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
                R4ChargingCharacteristicsToStr((R4_Org_ChargingCharacteristics *)(&pBill->chargingCharacteristics), pTempBuffer, nLength);
                value.sFieldValue = pTempBuffer;

                nLen++;

            }
            //servedPDPAddress
            else if(value.sFieldName.compare("servedPDPAddress") == 0)
            {
                if (pBill->bit_mask & R4_Fin_GGSNPDPRecord_servedPDPAddress_present)
                {
                    R43GPDPAddrToStr((R4_Org_PDPAddress *)&pBill->servedPDPAddress,
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
            //chargingID
            else if(value.sFieldName.compare("chargingID") == 0)
            {
                NumToDecStr(pBill->chargingID, pTempBuffer,
                    nLength, FALSE);
                value.sFieldValue = pTempBuffer;
                nLen++;
            }
        }
    }

    return nLen;
}


/*****************************************************************************
 Prototype    : R43GSGSNPDP_CAMELInformationToStr
 Description  : 将SCDR中的cAMELInformationPDP域转化为字符串
 Input        : R4_Org_CAMELInformationPDP *pData  SCDR中CAMEL域数据区
                char *pBuffer  存放返回字符串的数据区
                int nLength  数据区可用长度
 Output       : None
 Return Value : 转换后的字符串的长度
 Calls        :
 Called By    :

  History        :
  1.Date         : 2003-06-06
    Author       : Gan Zhiliang
    Modification : Created function

*****************************************************************************/
int R43GSGSNPDP_CAMELInformationToStr(R4_Org_CAMELInformationPDP *pData, char *pBuffer, int nLength)
{

    int nLen = 0;

    //ASSERT((BOOL)(pBuffer && pData));
    if ((pData == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    pBuffer[0] = 0;

    char *pTempBuffer = pBuffer;

    if (!pTempBuffer)
        return 0;

    strcpy(&pTempBuffer[nLen], "sCFAddress:");
    nLen += strlen("sCFAddress:");

    if(pData->bit_mask & R4_Org_CAMELInformationPDP_sCFAddress_present)
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

    if(pData->bit_mask & R4_Org_CAMELInformationPDP_serviceKey_present)
    {
        nLen += NumToDecStr((long)pData->serviceKey,
            &pTempBuffer[nLen], nLength - nLen, FALSE);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = ' ';

    strcpy(&pTempBuffer[nLen], "defaultTransactionHandling:");
    nLen += strlen("defaultTransactionHandling:");

    if(pData->bit_mask & R4_Org_CAMELInformationPDP_defaultTransactionHandling_present)
    {
        ENUMINFO EnumInfo;
        EnumInfo.nCount = R4DEFAULTTRANSACTIONHANDLINGENUMINFO_LENGTH;
        EnumInfo.pInfoTable = R4DEFAULTTRANSACTIONHANDLINGENUMINFO;
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

    if(pData->bit_mask & R4_Org_cAMELAccessPointNameNI_present)
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

    if(pData->bit_mask & R4_Org_cAMELAccessPointNameOI_present)
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

    if(pData->bit_mask & R4_Org_CAMELInformationPDP_numberOfDPEncountered_present)
    {
        nLen += NumToDecStr((long)pData->numberOfDPEncountered,
            &pTempBuffer[nLen], nLength - nLen, FALSE);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = ' ';

    strcpy(&pTempBuffer[nLen], "levelOfCAMELService:");
    nLen += strlen("levelOfCAMELService:");

    if(pData->bit_mask & R4_Org_CAMELInformationPDP_levelOfCAMELService_present)
    {
        ENUMINFO EnumInfo;
        EnumInfo.nCount = R4LEVELOFCAMELSERVICE_LENGTH;
        EnumInfo.pInfoTable = R4LEVELOFCAMELSERVICE;
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

    if(pData->bit_mask & R4_Org_CAMELInformationPDP_freeFormatData_present)
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

    if(pData->bit_mask & R4_Org_CAMELInformationPDP_fFDAppendIndicator_present)
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


    if(nLen >= nLength)
    {
        nLen = 0;
    }

    return nLen;
}



/*****************************************************************************
 Prototype    : R43GSGSNMM_CAMELInformationToStr
 Description  : 将MCDR中的R4_Org_CAMELInformationMM域转化成为字符串
 Input        : R4_Org_CAMELInformationMM *pData MCDR中CAMEL域数据区
                char *pBuffer    存放返回字符串的数据区
                int nLength     数据区可用长度
 Output       : None
 Return Value : 转换后的字符串的长度
 Calls        :
 Called By    :

  History        :
  1.Date         : 2003－06－06
    Author       : Gan Zhiliang
    Modification : Created function

*****************************************************************************/
int R43GSGSNMM_CAMELInformationToStr(R4_Org_CAMELInformationMM *pData, char *pBuffer, int nLength)
{

    int nLen = 0;

    //ASSERT((BOOL)(pBuffer && pData));
    if ((pData == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    pBuffer[0] = 0;

      char *pTempBuffer = pBuffer;

    if (!pTempBuffer)
        return 0;

    strcpy(&pTempBuffer[nLen], "sCFAddress:");
    nLen += strlen("sCFAddress:");

    if(pData->bit_mask & R4_Org_CAMELInformationMM_sCFAddress_present)
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

    if(pData->bit_mask & R4_Org_CAMELInformationMM_serviceKey_present)
    {
        nLen += NumToDecStr((long)pData->serviceKey,
            &pTempBuffer[nLen], nLength - nLen, FALSE);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = ' ';

    strcpy(&pTempBuffer[nLen], "defaultTransactionHandling:");
    nLen += strlen("defaultTransactionHandling:");

    if(pData->bit_mask & R4_Org_CAMELInformationMM_defaultTransactionHandling_present)
    {
        ENUMINFO EnumInfo;
        EnumInfo.nCount = R4DEFAULTTRANSACTIONHANDLINGENUMINFO_LENGTH;
        EnumInfo.pInfoTable = R4DEFAULTTRANSACTIONHANDLINGENUMINFO;
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

    if(pData->bit_mask & R4_Org_CAMELInformationMM_numberOfDPEncountered_present)
    {
        nLen += NumToDecStr((long)pData->numberOfDPEncountered,
            &pTempBuffer[nLen], nLength - nLen, FALSE);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = ' ';

    strcpy(&pTempBuffer[nLen], "levelOfCAMELService:");
    nLen += strlen("levelOfCAMELService:");

    if(pData->bit_mask & R4_Org_CAMELInformationMM_levelOfCAMELService_present)
    {
        ENUMINFO EnumInfo;
        EnumInfo.nCount = R4LEVELOFCAMELSERVICE_LENGTH;
        EnumInfo.pInfoTable = R4LEVELOFCAMELSERVICE;
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

    if(pData->bit_mask & R4_Org_CAMELInformationMM_freeFormatData_present)
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

    if(pData->bit_mask & R4_Org_CAMELInformationMM_fFDAppendIndicator_present)
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


    if(nLen >= nLength)
    {
        nLen = 0;
    }

    return nLen;
}


/*******************************************************************************
  Function        : R43GSGSNSMO_CAMELInfomationToStr
  Description     : 将SMOCDR中的cAMELInformationSMS域转化为字符串
  Input           : R4_Org_CAMELInformationSMS *pData
                  : char *pBuffer
                  : int nLength
  Output          :
  Return          : int
  Calls           :
  Called by       :

  History         :
  1 Date          : 2003-6-9
    Author        : Gan Zhiliang
    Modification  : Created file
*******************************************************************************/
int R43GSGSNSMO_CAMELInfomationToStr(R4_Org_CAMELInformationSMS *pData, char *pBuffer, int nLength)
{

    int nLen = 0;

    //ASSERT((BOOL)(pBuffer && pData));
    if ((pData == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    pBuffer[0] = 0;


    char *pTempBuffer = pBuffer;

    if (!pTempBuffer)
        return 0;

    strcpy(&pTempBuffer[nLen], "sCFAddress:");
    nLen += strlen("sCFAddress:");

    if(pData->bit_mask & R4_Org_CAMELInformationSMS_sCFAddress_present)
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

    if(pData->bit_mask & R4_Org_CAMELInformationSMS_serviceKey_present)
    {
        nLen += NumToDecStr((long)pData->serviceKey,
            &pTempBuffer[nLen], nLength - nLen, FALSE);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = ' ';

    strcpy(&pTempBuffer[nLen], "defaultSMSHandling:");
    nLen += strlen("defaultSMSHandling:");

    if(pData->bit_mask & R4_Org_defaultSMSHandling_present)
    {
        ENUMINFO EnumInfo;
        EnumInfo.nCount = R4DEFAULTSMSHANDLINGENUMINFO_LENGTH;
        EnumInfo.pInfoTable = R4DEFAULTSMSHANDLINGENUMINFO;
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

    if(pData->bit_mask & R4_Org_cAMELCallingPartyNumber_present)
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

    if(pData->bit_mask & R4_Org_cAMELDestinationSubscriberNumber_present)
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

    if(pData->bit_mask & R4_Org_cAMELSMSCAddress_present)
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

    if(pData->bit_mask & R4_Org_CAMELInformationSMS_freeFormatData_present)
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

    strcpy(&pTempBuffer[nLen], "smsReferenceNumber:");
    nLen += strlen("smsReferenceNumber:");

    if(pData->bit_mask & R4_Org_smsReferenceNumber_present)
    {
        nLen += BCDOctecToStr((LPOCTECSTRING)&pData->smsReferenceNumber, //by zkq  2004-0708 SWPD06405
            &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }



    pTempBuffer[nLen] = 0;


    if(nLen >= nLength)
    {
        nLen = 0;
    }

    return nLen;
}


/*******************************************************************************
Function        : R4LcsClientIdentityToStr
Description     : 把LcsClientIdentity字段转换成字符串
Input           : R4_Org_LCSClientIdentity *pData R4_Org_LCSClientIdentity字段
: char *pBuffer 存放返回字符串的数据区
: int nLength 数据去可用长度
Output          :
Return          : int 转换后的字符串的长度
Calls           :
Called by       :

History         :
1 Date          : 2003-6-2
Author        : Gan Zhiliang
Modification  : Created file
*******************************************************************************/
int R4LcsClientIdentityToStr(R4_Org_LCSClientIdentity *pData, char *pBuffer, int nLength)
{
    int nLen = 0;

    //ASSERT((BOOL)(pBuffer && pData));
    if ((pData == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    pBuffer[0] = 0;
    char *pTempBuffer = pBuffer;

    if (!pTempBuffer)
        return 0;

    strcpy(&pTempBuffer[nLen], "lcsClientExternalID:");
    nLen += strlen("lcsClientExternalID:");

    //R4_Org_LCSClientExternalID
    if((pData->bit_mask & R4_Org_lcsClientExternalID_present)
        && (pData->lcsClientExternalID.bit_mask & R4_Org_externalAddress_present))
    {
        nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pData->lcsClientExternalID,
            &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = ' ';

    strcpy(&pTempBuffer[nLen], "lcsClientDialedByMS:");
    nLen += strlen("lcsClientDialedByMS:");
    //R4_Org_AddressString
    if(pData->bit_mask & R4_Org_lcsClientDialedByMS_present)
    {
        nLen += CvtBCDOctecToStr((LPOCTECSTRING)&pData->lcsClientDialedByMS,
            &pTempBuffer[nLen], nLength - nLen);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }
    pTempBuffer[nLen++] = ' ';


    //R4_Org_LCSClientInternalID
    strcpy(&pTempBuffer[nLen], "lcsClientInternalID:");
    nLen += strlen("lcsClientInternalID:");
    if(pData->bit_mask & R4_Org_lcsClientInternalID_present)
    {
        ENUMINFO EnumInfo;
        EnumInfo.nCount = DEFAULTR4_Org_LCSClientInternalIDEnumInfo_LENGTH;
        EnumInfo.pInfoTable = R4_Org_LCSClientInternalIDEnumInfo;
        nLen += EnumToStr(&EnumInfo, pData->lcsClientInternalID, &pTempBuffer[nLen], nLength - nLen);

    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }

    pTempBuffer[nLen] = 0;

    if(nLen >= nLength)
    {
        nLen = 0;
    }
    return nLen;

}

/*******************************************************************************
Function        : R4LocationTypeToStr
Description     : 把R4_Org_LocationType字段转换成字符串
Input           : R4_Org_LocationType*pData R4_Org_LocationType字段
: char *pBuffer 存放返回字符串的数据区
: int nLength 数据去可用长度
Output          :
Return          : int 转换后的字符串的长度
Calls           :
Called by       :

  History         :
  1 Date          : 2003-6-2
  Author        : Gan Zhiliang
  Modification  : Created file
*******************************************************************************/
int R4LocationTypeToStr(R4_Org_LocationType *pData, char *pBuffer, int nLength)
{
    int nLen = 0;

    //ASSERT((BOOL)(pBuffer && pData));
    if ((pData == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    pBuffer[0] = 0;
    char *pTempBuffer = pBuffer;

    if (!pTempBuffer)
        return 0;

    strcpy(&pTempBuffer[nLen], "locationEstimateType:");
    nLen += strlen("locationEstimateType:");

    //locationEstimateType
    ENUMINFO EnumInfo;
    EnumInfo.nCount = DEFAULTR4_Org_LocationEstimateTypeEnumInfo_LENGTH;
    EnumInfo.pInfoTable = R4_Org_LocationEstimateTypeEnumInfo;
    nLen += EnumToStr(&EnumInfo, pData->locationEstimateType, &pTempBuffer[nLen], nLength - nLen);

    pTempBuffer[nLen++] = ' ';

    strcpy(&pTempBuffer[nLen], "deferredLocationEventType:");
    nLen += strlen("deferredLocationEventType:");
    //deferredLocationEventType
    if(pData->bit_mask & R4_Org_deferredLocationEventType_present)
    {
        nLen += NumToDecStr(pData->deferredLocationEventType, &pTempBuffer[nLen], nLength - nLen, FALSE);
    }
    else
    {
        strcpy(&pTempBuffer[nLen], _EMPTYSTRING);
        nLen += _EMPTYSTRINGLENGTH;
    }

    pTempBuffer[nLen] = 0;

    if(nLen >= nLength)
    {
        nLen = 0;
    }
    return nLen;

}

/*****************************************************************************
  Prototype    : LocationAreaAndCellToStr
  Description  : 把LocationAreaAndCell类型的数据转换成字符串
  Input        : R4_Org_LocationAreaAndCell *pData
  char *pBuffer
  int nLength
  Output       : None
  Return Value : int
  Calls        :
  Called By    :

    History        :
    1.Date         : 2003-06-06
    Author       : Gan Zhiliang
    Modification : Created function
*****************************************************************************/
int LocationAreaAndCellToStr(R4_Org_LocationAreaAndCell *pData, char *pBuffer, int nLength)
{
    int nLen = 0;

    //ASSERT((BOOL)(pBuffer && pData));
    if ((pData == NULL) || (pBuffer == NULL))
    {
         return 0;
    }

    pBuffer[0] = 0;
    char *pTempBuffer = pBuffer;

    if (!pTempBuffer)
        return 0;

    strcpy(&pTempBuffer[nLen], "locationAreaCode:");
    nLen += strlen("locationAreaCode:");

    //locationAreaCode
    nLen += HexOctecToStr((LPOCTECSTRING)&pData->locationAreaCode, &pTempBuffer[nLen],
        nLength - nLen, "0x", FALSE);
    pTempBuffer[nLen++] = ' ';

    //cellId
    strcpy(&pTempBuffer[nLen], "cellId:");
    nLen += strlen("cellId:");
    nLen += HexOctecToStr((LPOCTECSTRING)&pData->cellId, &pTempBuffer[nLen],
        nLength - nLen, "0x", FALSE);

    pTempBuffer[nLen] = 0;

    if(nLen >= nLength)
    {
        nLen = 0;
    }
    return nLen;


}

int R4_GCDRRecordExtensionsToStr(R4_Org_ManagementExtensions_ *pData, char *pBuffer, int nLength)
{
	int nLen = 0;
	
	if ((pData == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;	
	char *pTempBuffer = pBuffer;
	R4_Org_ManagementExtensions_* pNextExt = pData;

	//recordExtensions目前只定义了19个域
	while (pNextExt)
	{
		strcpy(&pTempBuffer[nLen], "{");
		nLen += 1;

		for (int j = 0; j < 19; j++)
		{
			if (pNextExt == NULL)
			{
                strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-25 问题单SWPD05340
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
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-25 问题单SWPD05340
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
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-25 问题单SWPD05340
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
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-25 问题单SWPD05340
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
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-25 问题单SWPD05340
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
                        strcpy(&pTempBuffer[nLen], "}"); //add by YanYong 2004-05-25 问题单SWPD05340
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
                        strcpy(&pTempBuffer[nLen], "}");
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
                        strcpy(&pTempBuffer[nLen], "}");
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
                        strcpy(&pTempBuffer[nLen], "}");
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
                        strcpy(&pTempBuffer[nLen], "}");
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
                        strcpy(&pTempBuffer[nLen], "}");
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
                        strcpy(&pTempBuffer[nLen], "}");
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
                        strcpy(&pTempBuffer[nLen], "}");
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
                        strcpy(&pTempBuffer[nLen], "}");
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
                        strcpy(&pTempBuffer[nLen], "}");
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
                        strcpy(&pTempBuffer[nLen], "}");
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
                        strcpy(&pTempBuffer[nLen], "}");
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
                        strcpy(&pTempBuffer[nLen], "}");
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
int CMCC_R4_GCDRRecordExtensionsToStr(R4_Org_ManagementExtensions_ *pData, char *pBuffer, int nLength)
{
	int nLen = 0;
	
	if ((pData == NULL) || (pBuffer == NULL))
	{
	     return 0;
	}

	pBuffer[0] = 0;	
	char *pTempBuffer = pBuffer;
	R4_Org_ManagementExtensions_* pNextExt = pData;
    
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


/******************************************************
// Function name: 	EnumToStrForQOSR4
// Author:        
// Date:          
// Description:   将Enum值按给出的信息转换成对应的字符串
// Input:
//       arg1 :LPENUMINFO pEnumInfo	枚举信息结构的指针
//       arg2 :int nEnumValue		待转换的枚举值
//       arg3 :char *pBuffer		存放返回字符串的数据区
//       arg4 :int nLength			数据区可用长度
//		 arg5 :R4_Org_NetworkInitiatedPDPContext networkInitiation  指示方向是MS to netWork还是netWork to MS
// Return value: int				转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//
******************************************************/
int EnumToStrForQoSR4(LPENUMINFO pEnumInfo, int nEnumValue, char *pBuffer, int nLength, char networkInitiation)
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
