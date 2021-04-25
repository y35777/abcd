/******************************************************************************

  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.

 ******************************************************************************
  File Name     : 3gr4_cdr_browse.h
  Version       : Initial Draft
  Author        : Gan Zhiliang
  Created       : 2003/6/6
  Last Modified :
  Description   : 3GPP R4协议话单浏览的头文件
  Function List :
  History       :
  1.Date        : 2003-06-06
    Author      : Gan Zhiliang
    Modification: Created file

******************************************************************************/
#ifndef __3GR4CDRBROWSE_H__
#define __3GR4CDRBROWSE_H__

#include "../format/format_comm.h"
#include "asn1_r4/CDRF_3GPPR4_Org.h"
#include "asn1_r4/CDRF_3GPPR4_Fin.h"

#include "../format/utility_ex.h"

#pragma warning(disable : 4200)

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
}S_QOSR4_Main;

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
}S_QOSR4_EXT;

typedef  struct
{
       S_QOSR4_Main     Qos;
       S_QOSR4_EXT      QosExt;
} S_QOSR4;
//End Of Addding

//GGSN关于Release 4内容计费的recordExtensions填写格式
typedef struct tagGCDR_R4_RecordExtensions 
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
    
} GCDR_R4_RecordExtensions;

int GetAllFieldTextValueOf3GR4CDR(char *pCDR, int nFormatType, char *pBuffer, int nLength);
int GetQueryFieldTextValueOf3GR4CDR(char *pCDR,
                                    int nFormatType,
                                    char *pBuffer,
                                    int nLength,
                                    QUERY_COND_FIELDS_VALUE* pValues);

//浏览原始话单的8个函数
int GetTextValueOf3GR4SGSNPDP(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GR4GGSNPDP(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GR4SGSNMM(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GR4SGSNSMO(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GR4SGSNSMT(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GR4SGSNLCT(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GR4SGSNLCO(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GR4SGSNLCN(char *pValue, char *pBuffer, int nLength);



//浏览合并后的话单的2个函数
int GetTextValueOf3GR4LARGESGSNPDP(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GR4LARGEGGSNPDP(char *pValue, char *pBuffer, int nLength);

//查询格式的10个函数
int GetTextValueOf_Q_3GR4SGSNPDP(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_3GR4GGSNPDP(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_3GR4SGSNMM(char *pValue,
                                char *pBuffer,
                                int nLength,
                                QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_3GR4SGSNSMO(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_3GR4SGSNSMT(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_3GR4SGSNLCT(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_3GR4SGSNLCO(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_3GR4SGSNLCN(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_3GR4LARGESGSNPDP(char *pValue,
                                      char *pBuffer,
                                      int nLength,
                                      QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_3GR4LARGEGGSNPDP(char *pValue,
                                      char *pBuffer,
                                      int nLength,
                                      QUERY_COND_FIELDS_VALUE* pValues);
int EnumToStrForQoSR4(LPENUMINFO pEnumInfo, int nEnumValue, char *pBuffer, int nLength, char networkInitiation);
int R4QosInformationToStr(R4_Org_QoSInformation *pQosInfo, char *pBuffer, int nLength, char networkInitiation);

int R43GIPAddrToStr(R4_Org_IPAddress *pIPAddr, char *pBuffer, int nLength);
int R43GPDPAddrToStr(R4_Org_PDPAddress *pPDPAddress, char *pBuffer, int nLength);
int R4RecordExtensionToStr(R4_Org_ManagementExtension *pData, char *pBuffer, int nLength);
int R4RecordExtensionsToStr(struct R4_Org_ManagementExtensions_ *pData, char *pBuffer, int nLength);
int R4_GCDRRecordExtensionsToStr(R4_Org_ManagementExtensions_ *pData, char *pBuffer, int nLength);
int CMCC_R4_GCDRRecordExtensionsToStr(R4_Org_ManagementExtensions_ *pData, char *pBuffer, int nLength);

int R4ListOfTrafficVolumesToStr(R4_Org__seqof1_ *pList, char *pBuffer, int nLength, char networkInitiation); 

int R4DiagnosticsToStr(R4_Org_Diagnostics *pDiagnostics, char *pBuffer, int nLength);
int R4ChangeLocationToStr(R4_Org_ChangeLocation *pData, char *pBuffer, int nLength);
int R4ChangeLocationsToStr(R4_Org_SGSNMMRecord::R4_Org__seqof3 *pData, char *pBuffer, int nLength);
int R4ChargingCharacteristicsToStr(R4_Org_ChargingCharacteristics * pData, char * pBuffer, int nLength);
int R43GSGSNPDP_CAMELInformationToStr(R4_Org_CAMELInformationPDP *pData, char *pBuffer, int nLength);
int R43GSGSNMM_CAMELInformationToStr(R4_Org_CAMELInformationMM * pData, char * pbuffer, int nLength);
int R43GLARGESGSNPDP_CAMELInformationToStr(R4_Fin_CAMELInformationPDP *pData, char *pBuffer, int nLength);
int R43GSGSNSMO_CAMELInfomationToStr(R4_Org_CAMELInformationSMS *pData, char *pBuffer, int nLength);

int R4LcsClientIdentityToStr(R4_Org_LCSClientIdentity *pData, char *pBuffer, int nLength);
int R4LocationTypeToStr(R4_Org_LocationType *pData, char *pBuffer, int nLength);
int LocationAreaAndCellToStr(R4_Org_LocationAreaAndCell *pData, char *pBuffer, int nLength);

#endif //__3G_R4_CDRBROWSE_H__
