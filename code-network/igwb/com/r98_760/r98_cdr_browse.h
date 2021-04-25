
#ifndef __GPRSCDRBROWSE_H__
#define __GPRSCDRBROWSE_H__


#include "../format/format_comm.h"
#include "asn1_r98/CDRF_2GCDR_Org.h"
#include "asn1_r98/CDRF_2GCDR_Fin.h"
#include "../format/utility_ex.h"

#pragma warning(disable : 4200)
/*
//关于GPRS格式名的定义
#define S_CDR_GPRSFORMATNAME		("SGSNPDP")
#define G_CDR_GPRSFORMATNAME		("GGSNPDP")
#define M_CDR_GPRSFORMATNAME		("SGSNMM")
#define S_SMO_CDR_GPRSFORMATNAME	("SGSNSMO")
#define S_SMT_CDR_GPRSFORMATNAME	("SGSNSMT")
#define L_S_CDR_GPRSFORMATNAME	("LARGESGSNPDP")
#define L_G_CDR_GPRSFORMATNAME	("LARGEGGSNPDP")
#define L_M_CDR_GPRSFORMATNAME	("LARGESGSNMM")

#define Q_S_CDR_GPRSFORMATNAME		("Q_SGSNPDP")
#define Q_G_CDR_GPRSFORMATNAME		("Q_GGSNPDP")
#define Q_M_CDR_GPRSFORMATNAME		("Q_SGSNMM")
#define Q_S_SMO_CDR_GPRSFORMATNAME	("Q_SGSNSMO")
#define Q_S_SMT_CDR_GPRSFORMATNAME	("Q_SGSNSMT")
#define Q_L_S_CDR_GPRSFORMATNAME	("Q_LARGESGSNPDP")
#define Q_L_G_CDR_GPRSFORMATNAME	("Q_LARGEGGSNPDP")
#define Q_L_M_CDR_GPRSFORMATNAME	("Q_LARGESGSNMM")
*/

namespace NS_R98_760{
int R98_GCDRRecordExtensionsToStr(Org_ManagementExtensions_ *pData, char *pBuffer, int nLength);
int CMCC_R98_GCDRRecordExtensionsToStr(Org_ManagementExtensions_ *pData, char *pBuffer, int nLength);


//下面两个主函数，一个用于浏览，一个用于查询
int GetAllFieldTextValueOfGPRSCDR(char *pCDR, int nFormatType, char *pBuffer, int nLength);
int GetQueryFieldTextValueOfGPRSCDR(char *pCDR,
                                    int nFormatType,
                                    char *pBuffer,
                                    int nLength,
                                    QUERY_COND_FIELDS_VALUE* pValues);

//下面八个是浏览子函数
int GetTextValueOfGPRSSGSNPDP(char *pValue, char *pBuffer, int nLength);
int GetTextValueOfGPRSGGSNPDP(char *pValue, char *pBuffer, int nLength);
int GetTextValueOfGPRSSGSNMM(char *pValue, char *pBuffer, int nLength);
int GetTextValueOfGPRSSGSNSMO(char *pValue, char *pBuffer, int nLength);
int GetTextValueOfGPRSSGSNSMT(char *pValue, char *pBuffer, int nLength);
int GetTextValueOfGPRSLARGESGSNPDP(char *pValue, char *pBuffer, int nLength);
int GetTextValueOfGPRSLARGEGGSNPDP(char *pValue, char *pBuffer, int nLength);
int GetTextValueOfGPRSLARGESGSNMM(char *pValue, char *pBuffer, int nLength);

//下面八个是查询子函数
int GetTextValueOf_Q_GPRSSGSNPDP(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_GPRSGGSNPDP(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_GPRSSGSNMM(char *pValue,
                                char *pBuffer,
                                int nLength,
                                QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_GPRSSGSNSMO(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_GPRSSGSNSMT(char *pValue,
                                 char *pBuffer,
                                 int nLength,
                                 QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_GPRSLARGESGSNPDP(char *pValue,
                                      char *pBuffer,
                                      int nLength,
                                      QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_GPRSLARGEGGSNPDP(char *pValue,
                                      char *pBuffer,
                                      int nLength,
                                      QUERY_COND_FIELDS_VALUE* pValues);
int GetTextValueOf_Q_GPRSLARGESGSNMM(char *pValue,
                                     char *pBuffer,
                                     int nLength,
                                     QUERY_COND_FIELDS_VALUE* pValues);

int _GPRSIPAddrToStr(Org_IPAddress *pIPAddr, char *pBuffer, int nLength);
int _GPRSPDPAddrToStr(Org_PDPAddress *pPDPAddress, char *pBuffer, int nLength);
int _GprsQosInformationToStr(Org_QoSInformation *pQosInfo, char *pBuffer, int nLength);
int _GPRSSGSNAddrToStr(Org__seqof2 *pData, char *pBuffer, int nLength);
int _GPRSREMOTEPDPAddrToStr(Org__seqof3 *pData, char *pBuffer, int nLength);

int Gprs_ListOfTrafficVolumesToStr(Org__seqof1_ *pList, char *pBuffer, int nLength);
int Gprs_DiagnosticsToStr(Org_Diagnostics *pDiagnostics, char *pBuffer, int nLength);
int Gprs_RecordExtensionToStr(Org_ManagementExtension *pData, char *pBuffer, int nLength);
int Gprs_RecordExtensionsToStr(Org_ManagementExtensions_ *pData, char *pBuffer, int nLength);
int Gprs_ChangeLocationToStr(Org_ChangeLocation *pData, char *pBuffer, int nLength);
int Gprs_ChangeLocationsToStr(Org__seqof4 *pData, char *pBuffer, int nLength);

int RecordSequenceNumbertoStr(Fin_RECORDSequenceNumber *pData, char *pBuffer, int nLength);
int CvtRecordSequenceNumbertoStr(Fin__seqof3 *pData, char *pBuffer, int nLength);
};

#endif //__GPRSCDRBROWSE_H__
