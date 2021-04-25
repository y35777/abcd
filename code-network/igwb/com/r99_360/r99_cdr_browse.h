/*****************************************************
//		3GCDRBrowse.h
//*****************************************************
//Copyright 2001 by Huawei CO.,LTD.
// All rights reserved.
// Compiler:     Visual C++ 6.0
// Author:       Libo
// Date:         01-4-24
// Description:  
// 
// Side Effects: NONE
// Functions:
// Notes:    
// 
// Update: 01-4-24 下午 02:15:46
// Date              Name              Description 
============== ================ ======================
## 01-4-24       Libo
*****************************************************/
#ifndef __3GCDRBROWSE_H__
#define __3GCDRBROWSE_H__

#include "../format/format_comm.h"
#include "asn1_r99/CDRF_3GCDR_CG.h"
#include "asn1_r99/CDRF_3GCDR.h"

#include "../format/utility_ex.h"

#pragma warning(disable : 4200)
/*
//关于3G格式名的定义
#define S_CDR_3GFORMATNAME		("3G-SGSNPDP")
#define G_CDR_3GFORMATNAME		("3G-GGSNPDP")
#define M_CDR_3GFORMATNAME		("3G-SGSNMM")
#define S_SMO_CDR_3GFORMATNAME	("3G-SGSNSMO")
#define S_SMT_CDR_3GFORMATNAME	("3G-SGSNSMT")
#define L_S_CDR_3GFORMATNAME	("3G-LARGESGSNPDP")
#define L_G_CDR_3GFORMATNAME	("3G-LARGEGGSNPDP")
#define L_M_CDR_3GFORMATNAME	("3G-LARGESGSNMM")

//Added By ZhengYuqun 2001/07/30
#define Q_S_CDR_3GFORMATNAME		("Q_3G-SGSNPDP")
#define Q_G_CDR_3GFORMATNAME		("Q_3G-GGSNPDP")
#define Q_M_CDR_3GFORMATNAME		("Q_3G-SGSNMM")
#define Q_S_SMO_CDR_3GFORMATNAME	("Q_3G-SGSNSMO")
#define Q_S_SMT_CDR_3GFORMATNAME	("Q_3G-SGSNSMT")
#define Q_L_S_CDR_3GFORMATNAME	("Q_3G-LARGESGSNPDP")
#define Q_L_G_CDR_3GFORMATNAME	("Q_3G-LARGEGGSNPDP")
#define Q_L_M_CDR_3GFORMATNAME	("Q_3G-LARGESGSNMM")
//End Of Adding
*/
//Added By ZhengYuqun 2001/09/28
//增加大端字节序 ZhengYuqun 2003-09-10  SWPD01505
typedef  struct 
{
#ifdef _PLATFORM_WIN32
/*小端字序*/
	unsigned char     ReliabClass : 3;
	unsigned char     DelayClass : 3;           
	unsigned char     Spare1 : 2;
	unsigned char     PrecedClass : 3;
	unsigned char     Spare2 : 1;
	unsigned char     PeakThrougout : 4;
	unsigned char     MeanThrougout : 5;
	unsigned char     Spare3 : 3;
#else
/*大端字序*/
    unsigned char     Spare1 : 2;
    unsigned char     DelayClass : 3;                      
    unsigned char     ReliabClass : 3;
    unsigned char     PeakThrougout : 4;
    unsigned char     Spare2 : 1;
    unsigned char     PrecedClass : 3;
    unsigned char     Spare3 : 3;
    unsigned char     MeanThrougout : 5;
#endif
}S_QOS98;

typedef  struct 
{
#ifdef _PLATFORM_WIN32
/*小端字序*/
	unsigned char      DeliveryOfErroneousSDU:3;
	unsigned char      DeliveryOrder:2;
	unsigned char      TrafficClass:3;
	unsigned char      MaximumSDUsize;
	unsigned char      MaximumBitRateForUplink;
	unsigned char      MaximumBitRateForDownlink;
	unsigned char      SDUErrorRatio:4;
	unsigned char      ResidualBER:4;
	unsigned char      TrafficHandlingPriority:2;
	unsigned char      TransferDelay:6;
	unsigned char      GuaranteedBitRateForUplink;
	unsigned char      GuaranteedBitRateForDownlink;
#else
/*大端字序*/
    unsigned char      TrafficClass:3;
    unsigned char      DeliveryOrder:2;
    unsigned char      DeliveryOfErroneousSDU:3;
    unsigned char      MaximumSDUsize;
    unsigned char      MaximumBitRateForUplink;
    unsigned char      MaximumBitRateForDownlink;
    unsigned char      ResidualBER:4;
    unsigned char      SDUErrorRatio:4;
    unsigned char      TransferDelay:6;
    unsigned char      TrafficHandlingPriority:2;
    unsigned char      GuaranteedBitRateForUplink;
    unsigned char      GuaranteedBitRateForDownlink;
#endif
}S_QOS_EXT;

typedef  struct
{
       S_QOS98        Qos98;
       S_QOS_EXT      QosExt;
} S_QOS99;
//End Of Addding

//GGSN关于Release 1999内容计费的recordExtensions填写格式
typedef struct tagGCDR_R99_RecordExtensions 
{
    unsigned long   ulServerIP;				/*  Server IP*/	
    unsigned short  usServerPort;			/*  Server端口号*/
    unsigned char   ucContentRecordFlag; 	/* 内容计费话单的类型1字节，必须
												值0：标识该话单是针对一次URL连接产生的话单
												值1：标识该话单是针对服务器流量产生的话单
												值2：标识该话单是针对访问服务器持续时间累加的话单
												值3：标识该话单是针对访问服务器流量和持续时间累加的混合话单
												其他值：保留用于上层应用协议内容计费的扩展 */
    unsigned short usTransferProtocol; 		/* 内容计费话单的传输层协议, 该协议目前只支持TCP报文*/
    unsigned long  ulMsIP; 					/* 手机用户的IP地址*/
    unsigned short usMsPort;  				/* 手机用户的端口信息*/
    unsigned short usAppProtocol;     		/* 应用层协议*/
    unsigned long  ulUpFlow;       			/* 访问服务器统计的话单上行流量数据*/
    unsigned long  ulDownFlow;       		/* 访问服务器统计的话单下行流量数据*/
    unsigned long  ulDuration;  			/* 访问服务器统计的话单链接时间持续累加数据*/
    unsigned char  ucUrlString[200]; 		/* URL解析结果, 话单类型为0时必须填写*/
    unsigned short usFlowRateInfo; 			/* 流量费率信息*/
    unsigned short usTimeRateInfo; 			/* 时间费率信息*/
    unsigned long  usFlowBillInfo; 			/* 流量费用信息*/
    unsigned long  usTimeBillInfo; 			/* 时间费用信息*/
    unsigned long  usURLBillInfo; 			/* URL费用信息*/
    unsigned short usURLRateInfo; 			/* URL费率信息*/
    unsigned char  ucContentRecordClose;    /*  内容计费话单关闭原因1字节，必须
												值0：用户正常上下文去激活
												值1：URL记次关闭话单
												值2：流量超过上限产生话单
												值3：时间持续达到上限产生话单
												值4：系统关闭生成话单（例如主备倒换、系统资源达到上限、异常关闭等原因）*/
    unsigned char ucReserve;				/* 保留字段,字节对齐*/
    
} GCDR_R99_RecordExtensions;

int R99_GCDRRecordExtensionsToStr(CDR3G_ManagementExtensions *pData, char *pBuffer, int nLength);
int CMCC_R99_GCDRRecordExtensionsToStr(CDR3G_ManagementExtensions *pData, char *pBuffer, int nLength);

int GetAllFieldTextValueOf3GCDR(char *pCDR, int nFormatType, char *pBuffer, int nLength);

//Added by XDB on 2001.11.07
int GetQueryFieldTextValueOf3GCDR(char *pCDR,
                                  int nFormatType,
                                  char *pBuffer,
                                  int nLength,
                                  QUERY_COND_FIELDS_VALUE* pValues);

int GetTextValueOf3GSGSNPDP(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GGGSNPDP(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GSGSNMM(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GSGSNSMO(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GSGSNSMT(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GLARGESGSNPDP(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GLARGEGGSNPDP(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GLARGESGSNMM(char *pValue, char *pBuffer, int nLength);

//Added By ZhengYuqun 2001/07/30
int GetTextValueOf_Q_3GSGSNPDP(char *pValue,
                               char *pBuffer,
                               int nLength,
                               QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_3GGGSNPDP(char *pValue,
                               char *pBuffer,
                               int nLength,
                               QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_3GSGSNMM(char *pValue,
                              char *pBuffer,
                              int nLength,
                              QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_3GSGSNSMO(char *pValue,
                               char *pBuffer,
                               int nLength,
                               QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_3GSGSNSMT(char *pValue,
                               char *pBuffer,
                               int nLength,
                               QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_3GLARGESGSNPDP(char *pValue,
                                    char *pBuffer,
                                    int nLength,
                                    QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_3GLARGEGGSNPDP(char *pValue,
                                    char *pBuffer,
                                    int nLength,
                                    QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_3GLARGESGSNMM(char *pValue,
                                   char *pBuffer,
                                   int nLength,
                                   QUERY_COND_FIELDS_VALUE* pValues);

int EnumToStrForQoS99(LPENUMINFO pEnumInfo, int nEnumValue, char *pBuffer, int nLength, char networkInitiation);

int _3GIPAddrToStr(CDR3G_IPAddress *pIPAddr, char *pBuffer, int nLength);
int _3GPDPAddrToStr(CDR3G_PDPAddress *pPDPAddress, char *pBuffer, int nLength);

//增加一个参数，代表是MS to netWork还是netWork to MS，Modified By ZhengYuqun 2001/09/28
int ListOfTrafficVolumesToStr(CDR3G__seqof1 *pList, char *pBuffer, int nLength, char networkInitiation = 0); 

//增加一个参数，代表是MS to netWork还是netWork to MS，Modified By ZhengYuqun 2001/09/28
int _3GQosInformationToStr(CDR3G_QoSInformation *pQosInfo, char *pBuffer, int nLength, char networkInitiation = 0);
int DiagnosticsToStr(CDR3G_Diagnostics *pDiagnostics, char *pBuffer, int nLength);
int RecordExtensionToStr(CDR3G_ManagementExtension *pData, char *pBuffer, int nLength);
int RecordExtensionsToStr(CDR3G_ManagementExtensions *pData, char *pBuffer, int nLength);
int ChangeLocationToStr(CDR3G_ChangeLocation *pData, char *pBuffer, int nLength);
int ChangeLocationsToStr(CDR3G__seqof3 *pData, char *pBuffer, int nLength);
int ChargingCharacteristicsToStr(CDR3G_ChargingCharacteristics *pData, char *pBuffer, int nLength);
int SystemTypeToStr(CDR3G_SystemType *pData, char *pBuffer, int nLength);

//added by zouyongsheng 2002-08-09
int _3GSGSNPDP_CAMELInformationToStr(CDR3G_CAMELInformationPDP *pData, char *pBuffer, int nLength);
int _3GSGSNMM_CAMELInformationToStr(CDR3G_CAMELInformationMM *pData, char *pbuffer, int nLength);
int _3GLARGESGSNPDP_CAMELInformationToStr(_3GCDR_CAMELInformationPDP *pData, char *pBuffer, int nLength);
int _3GLARGESGSNMM_CAMELInformationToStr(_3GCDR_CAMELInformationMM *pData, char *pbuffer, int nLength);
int _3GSGSNSMO_CAMELInfomationToStr(CDR3G_CAMELInformationSMS *pData, char *pBuffer, int nLength);


#endif //__3GCDRBROWSE_H__
