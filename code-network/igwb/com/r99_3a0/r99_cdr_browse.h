/*****************************************************
//		r99_CDR_Browse.h
//*****************************************************
//Copyright 2003 by Huawei CO.,LTD.
// All rights reserved.
// Compiler:     Visual C++ 6.0
// Author:       GGF
// Date:         2003-09-28
// Description:  
// 
// Side Effects: NONE
// Functions:
// Notes:    
// 
// Update: 
// Date              Name              Description 
============== ================ ======================
## 
*****************************************************/
#ifndef R99_CDR_BROWSE_H
#define R99_CDR_BROWSE_H

#include "../format/format_comm.h"
#include "asn1_r99/CDRF_CMCC_3GCDR.h"
#include "asn1_r99/CDRF_CMCC_3GCDR_CG.h"

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
}S_R99_QOS98;

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
}S_R99_QOS_EXT;

typedef  struct
{
       S_R99_QOS98        Qos98;
       S_R99_QOS_EXT      QosExt;
} S_R99_QOS99;


int GetAllFieldTextValueOfR99CDR(char *pCDR, int nFormatType, char *pBuffer, int nLength);

int GetQueryFieldTextValueOfR99CDR(char *pCDR,
                                   int nFormatType,
                                   char *pBuffer,
                                   int nLength,
                                   QUERY_COND_FIELDS_VALUE* pValues);

int GetTextValueOfR99SGSNPDP(char *pValue, char *pBuffer, int nLength);
int GetTextValueOfR99GGSNPDP(char *pValue, char *pBuffer, int nLength);
int GetTextValueOfR99SGSNMM(char *pValue, char *pBuffer, int nLength);
int GetTextValueOfR99SGSNSMO(char *pValue, char *pBuffer, int nLength);
int GetTextValueOfR99SGSNSMT(char *pValue, char *pBuffer, int nLength);
int GetTextValueOfR99LARGESGSNPDP(char *pValue, char *pBuffer, int nLength);
int GetTextValueOfR99LARGEGGSNPDP(char *pValue, char *pBuffer, int nLength);
int GetTextValueOfR99LARGESGSNMM(char *pValue, char *pBuffer, int nLength);

int GetTextValueOf_Q_R99SGSNPDP(char *pValue,
                                char *pBuffer,
                                int nLength,
                                QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_R99GGSNPDP(char *pValue,
                                char *pBuffer,
                                int nLength,
                                QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_R99SGSNMM(char *pValue,
                               char *pBuffer,
                               int nLength,
                               QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_R99SGSNSMO(char *pValue,
                                char *pBuffer,
                                int nLength,
                                QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_R99SGSNSMT(char *pValue,
                                char *pBuffer,
                                int nLength,
                                QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_R99LARGESGSNPDP(char *pValue,
                                     char *pBuffer,
                                     int nLength,
                                     QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_R99LARGEGGSNPDP(char *pValue,
                                     char *pBuffer,
                                     int nLength,
                                     QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_R99LARGESGSNMM(char *pValue,
                                    char *pBuffer,
                                    int nLength,
                                    QUERY_COND_FIELDS_VALUE* pValues);

int R99_EnumToStrForQoS99(LPENUMINFO pEnumInfo, int nEnumValue, char *pBuffer, int nLength, char networkInitiation);

int R99_IPAddrToStr(CMCC_3GCDR_IPAddress *pIPAddr, char *pBuffer, int nLength);
int R99_PDPAddrToStr(CMCC_3GCDR_PDPAddress *pPDPAddress, char *pBuffer, int nLength);

//增加一个参数，代表是MS to netWork还是netWork to MS
int R99_ListOfTrafficVolumesToStr(CMCC_3GCDR__seqof1 *pList, char *pBuffer, int nLength, char networkInitiation = 0); 

//增加一个参数，代表是MS to netWork还是netWork to MS
int R99_QosInformationToStr(CMCC_3GCDR_QoSInformation *pQosInfo, char *pBuffer, int nLength, char networkInitiation = 0);
int R99_DiagnosticsToStr(CMCC_3GCDR_Diagnostics *pDiagnostics, char *pBuffer, int nLength);
int R99_RecordExtensionToStr(CMCC_3GCDR_ManagementExtension *pData, char *pBuffer, int nLength);
int R99_RecordExtensionsToStr(CMCC_3GCDR_ManagementExtensions *pData, char *pBuffer, int nLength);
int R99_GCDRRecordExtensionsToStr(CMCC_3GCDR_ManagementExtensions *pData, char *pBuffer, int nLength);
int CMCC_R99_GCDRRecordExtensionsToStr(CMCC_3GCDR_ManagementExtensions *pData, char *pBuffer, int nLength);

int R99_ChangeLocationToStr(CMCC_3GCDR_ChangeLocation *pData, char *pBuffer, int nLength);
int R99_ChangeLocationsToStr(CMCC_3GCDR_SGSNMMRecord::CMCC_3GCDR__seqof3 *pData, char *pBuffer, int nLength);
int R99_ChargingCharacteristicsToStr(CMCC_3GCDR_ChargingCharacteristics *pData, char *pBuffer, int nLength);
int R99_SystemTypeToStr(CMCC_3GCDR_SystemType *pData, char *pBuffer, int nLength);

int R99_SGSNPDP_CAMELInformationToStr(CMCC_3GCDR_CAMELInformationPDP *pData, char *pBuffer, int nLength);
int R99_SGSNMM_CAMELInformationToStr(CMCC_3GCDR_CAMELInformationMM *pData, char *pbuffer, int nLength);
int R99_LARGESGSNPDP_CAMELInformationToStr(CMCC_3GCDR_CG_CAMELInformationPDP *pData, char *pBuffer, int nLength);
int R99_LARGESGSNMM_CAMELInformationToStr(CMCC_3GCDR_CG_CAMELInformationMM *pData, char *pbuffer, int nLength);
int R99_SGSNSMO_CAMELInfomationToStr(CMCC_3GCDR_CAMELInformationSMS *pData, char *pBuffer, int nLength);


#endif //R99_CDR_BROWSE_H
