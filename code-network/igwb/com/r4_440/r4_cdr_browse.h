/******************************************************************************

  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.

 ******************************************************************************
  File Name     : 3gr4_cdr_browse.h
  Version       : Initial Draft
  Author        : Gan Zhiliang
  Created       : 2003/6/6
  Last Modified :
  Description   : 3GPP R4Э�黰�������ͷ�ļ�
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
/*С������*/
	unsigned char     ReliabClass : 3;
	unsigned char     DelayClass : 3;           
	unsigned char     Spare1 : 2;
	unsigned char     PrecedClass : 3;
	unsigned char     Spare2 : 1;
	unsigned char     PeakThrougout : 4;
	unsigned char     MeanThrougout : 5;
	unsigned char     Spare3 : 3;
#else
/*�������*/
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
/*С������*/
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
/*�������*/
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

//GGSN����Release 4���ݼƷѵ�recordExtensions��д��ʽ
typedef struct tagGCDR_R4_RecordExtensions 
{
    unsigned long   ulServerIP;				/*  Server IP*/	
    unsigned short  usServerPort;			/*  Server�˿ں�*/
    unsigned char   ucContentRecordFlag; 	/* ���ݼƷѻ���������1�ֽڣ�����
												ֵ0����ʶ�û��������һ��URL���Ӳ����Ļ���
												ֵ1����ʶ�û�������Է��������������Ļ���
												ֵ2����ʶ�û�������Է��ʷ���������ʱ���ۼӵĻ���
												ֵ3����ʶ�û�������Է��ʷ����������ͳ���ʱ���ۼӵĻ�ϻ���
												����ֵ�����������ϲ�Ӧ��Э�����ݼƷѵ���չ */
    unsigned short usTransferProtocol; 		/* ���ݼƷѻ����Ĵ����Э��, ��Э��Ŀǰֻ֧��TCP����*/
    unsigned long  ulMsIP; 					/* �ֻ��û���IP��ַ*/
    unsigned short usMsPort;  				/* �ֻ��û��Ķ˿���Ϣ*/
    unsigned short usAppProtocol;     		/* Ӧ�ò�Э��*/
    unsigned long  ulUpFlow;       			/* ���ʷ�����ͳ�ƵĻ���������������*/
    unsigned long  ulDownFlow;       		/* ���ʷ�����ͳ�ƵĻ���������������*/
    unsigned long  ulDuration;  			/* ���ʷ�����ͳ�ƵĻ�������ʱ������ۼ�����*/
    unsigned char  ucUrlString[200]; 		/* URL�������, ��������Ϊ0ʱ������д*/
    unsigned short usFlowRateInfo; 			/* ����������Ϣ*/
    unsigned short usTimeRateInfo; 			/* ʱ�������Ϣ*/
    unsigned long  usFlowBillInfo; 			/* ����������Ϣ*/
    unsigned long  usTimeBillInfo; 			/* ʱ�������Ϣ*/
    unsigned long  usURLBillInfo; 			/* URL������Ϣ*/
    unsigned short usURLRateInfo; 			/* URL������Ϣ*/
    unsigned char  ucContentRecordClose;    /*  ���ݼƷѻ����ر�ԭ��1�ֽڣ�����
												ֵ0���û�����������ȥ����
												ֵ1��URL�Ǵιرջ���
												ֵ2�������������޲�������
												ֵ3��ʱ������ﵽ���޲�������
												ֵ4��ϵͳ�ر����ɻ�������������������ϵͳ��Դ�ﵽ���ޡ��쳣�رյ�ԭ��*/
    unsigned char ucReserve;				/* �����ֶ�,�ֽڶ���*/
    
} GCDR_R4_RecordExtensions;

int GetAllFieldTextValueOf3GR4CDR(char *pCDR, int nFormatType, char *pBuffer, int nLength);
int GetQueryFieldTextValueOf3GR4CDR(char *pCDR,
                                    int nFormatType,
                                    char *pBuffer,
                                    int nLength,
                                    QUERY_COND_FIELDS_VALUE* pValues);

//���ԭʼ������8������
int GetTextValueOf3GR4SGSNPDP(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GR4GGSNPDP(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GR4SGSNMM(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GR4SGSNSMO(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GR4SGSNSMT(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GR4SGSNLCT(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GR4SGSNLCO(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GR4SGSNLCN(char *pValue, char *pBuffer, int nLength);



//����ϲ���Ļ�����2������
int GetTextValueOf3GR4LARGESGSNPDP(char *pValue, char *pBuffer, int nLength);
int GetTextValueOf3GR4LARGEGGSNPDP(char *pValue, char *pBuffer, int nLength);

//��ѯ��ʽ��10������
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
