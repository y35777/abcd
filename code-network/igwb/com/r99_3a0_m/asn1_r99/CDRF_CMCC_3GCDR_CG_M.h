/********************************************************/
/* Copyright (C) 2003 OSS Nokalva.  All rights reserved.*/
/********************************************************/

/* THIS FILE IS PROPRIETARY MATERIAL OF OSS NOKALVA
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF OSS NOKALVA.
 * THIS FILE MAY NOT BE DISTRIBUTED. */

/* Generated for: HuaWei Technologies, Inc., Shenzhen, China - license 8088 for Windows NT */
/* Abstract syntax: CDRF_R99_Fin_M */
/* Created: Fri Dec 19 17:31:44 2003 */
/* ASN.1 compiler version: 5.3 */
/* Target operating system: Windows NT/Windows 9x */
/* Target machine type: Intel x86 */
/* C compiler options required: -Zp4 (Microsoft) */
/* ASN.1 compiler options and file names specified:
 * -codefile CDRF_CMCC_3GCDR_CG_M.C -headerfile CDRF_CMCC_3GCDR_CG_M.h
 * -errorfile C:\WINDOWS\TEMP\2. -prefix CMCC_3GCDR_CG_M_ -nodebug -dualheader
 * -ansi -verbose -ber -root
 * D:\Application\ossasn1\win32\5.3.0\asn1dflt\asn1dflt.ms.zp4
 * D:\yutemp\lidufang\1218\R99\CDRF_CMCC_3GCDR_CG_M\CDRF_R99_Fin_M.asn
 */

#ifndef OSS_CDRF_R99_Fin_M
#define OSS_CDRF_R99_Fin_M

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "asn1hdr.h"
#include "asn1code.h"

#define          CMCC_3GCDR_CG_M_BSRecord_PDU 1

typedef struct CMCC_3GCDR_CG_M__ObjectID {
    unsigned short  length;
    unsigned char   *value;
} CMCC_3GCDR_CG_M__ObjectID;

typedef struct CMCC_3GCDR_CG_M__octet1 {
    unsigned short  length;
    unsigned char   value[15];
} CMCC_3GCDR_CG_M__octet1;

typedef struct CMCC_3GCDR_CG_M__octet3 {
    unsigned short  length;
    unsigned char   value[14];
} CMCC_3GCDR_CG_M__octet3;

typedef struct CMCC_3GCDR_CG_M__octet4 {
    unsigned short  length;
    unsigned char   value[10];
} CMCC_3GCDR_CG_M__octet4;

typedef struct CMCC_3GCDR_CG_M_BSXferSummaryRecord {
    CMCC_3GCDR_CG_M__octet1 cgnID;
    struct CMCC_3GCDR_CG_M__octet2 {
        unsigned short  length;
        unsigned char   value[34];
    } bsXferFile;
    CMCC_3GCDR_CG_M__octet3 openTime;
    CMCC_3GCDR_CG_M__octet3 closeTime;
    CMCC_3GCDR_CG_M__octet4 lrsnStart;
    CMCC_3GCDR_CG_M__octet4 lrsnEnd;
    struct CMCC_3GCDR_CG_M__octet5 {
        unsigned short  length;
        unsigned char   value[3];
    } cdrType;
    CMCC_3GCDR_CG_M__octet4 totalCdr;
    CMCC_3GCDR_CG_M__octet4 missingCdr;
    CMCC_3GCDR_CG_M__octet4 duplicateCdr;
    CMCC_3GCDR_CG_M__octet1 cgID;
} CMCC_3GCDR_CG_M_BSXferSummaryRecord;

typedef struct CMCC_3GCDR_CG_M_BSRecord {
    CMCC_3GCDR_CG_M_BSXferSummaryRecord bsXferRecord;
    struct CMCC_3GCDR_CG_M__seqof1 {
        unsigned int    count;
        struct CMCC_3GCDR_CG_M_CallEventRecord *value;
    } listOfCDR;
} CMCC_3GCDR_CG_M_BSRecord;

typedef int             CMCC_3GCDR_CG_M_CallEventRecordType;
#define                     CMCC_3GCDR_CG_M_CallEventRecordType_sgsnPDPRecord 18
#define                     CMCC_3GCDR_CG_M_CallEventRecordType_ggsnPDPRecord 19
#define                     CMCC_3GCDR_CG_M_CallEventRecordType_sgsnMMRecord 20
#define                     CMCC_3GCDR_CG_M_CallEventRecordType_sgsnSMORecord 21
#define                     CMCC_3GCDR_CG_M_CallEventRecordType_sgsnSMTRecord 22

typedef ossBoolean      CMCC_3GCDR_CG_M_NetworkInitiatedPDPContext;

typedef struct CMCC_3GCDR_CG_M_IMSI {
    unsigned short  length;
    unsigned char   value[8];
} CMCC_3GCDR_CG_M_IMSI;

typedef struct CMCC_3GCDR_CG_M_IMEI {
    unsigned short  length;
    unsigned char   value[8];
} CMCC_3GCDR_CG_M_IMEI;

typedef struct CMCC_3GCDR_CG_M_IPBinaryAddress {
    unsigned short  choice;
#       define      CMCC_3GCDR_CG_M_iPBinV4Address_chosen 1
#       define      CMCC_3GCDR_CG_M_iPBinV6Address_chosen 2
    union _union {
        struct CMCC_3GCDR_CG_M__octet6 {
            unsigned short  length;
            unsigned char   value[4];
        } iPBinV4Address;  /* to choose, set choice to
                            * CMCC_3GCDR_CG_M_iPBinV4Address_chosen */
        struct CMCC_3GCDR_CG_M__octet7 {
            unsigned short  length;
            unsigned char   value[16];
        } iPBinV6Address;  /* to choose, set choice to
                            * CMCC_3GCDR_CG_M_iPBinV6Address_chosen */
    } u;
} CMCC_3GCDR_CG_M_IPBinaryAddress;

typedef struct CMCC_3GCDR_CG_M_IPTextRepresentedAddress {
    unsigned short  choice;
#       define      CMCC_3GCDR_CG_M_iPTextV4Address_chosen 1
#       define      CMCC_3GCDR_CG_M_iPTextV6Address_chosen 2
    union _union {
        char            iPTextV4Address[16];  /* to choose, set choice to
                                    * CMCC_3GCDR_CG_M_iPTextV4Address_chosen */
        char            iPTextV6Address[46];  /* to choose, set choice to
                                    * CMCC_3GCDR_CG_M_iPTextV6Address_chosen */
    } u;
} CMCC_3GCDR_CG_M_IPTextRepresentedAddress;

typedef struct CMCC_3GCDR_CG_M_IPAddress {
    unsigned short  choice;
#       define      CMCC_3GCDR_CG_M_iPBinaryAddress_chosen 1
#       define      CMCC_3GCDR_CG_M_iPTextRepresentedAddress_chosen 2
    union _union {
        CMCC_3GCDR_CG_M_IPBinaryAddress iPBinaryAddress;  /* to choose, set
                                   * choice to
                                   * CMCC_3GCDR_CG_M_iPBinaryAddress_chosen */
        CMCC_3GCDR_CG_M_IPTextRepresentedAddress iPTextRepresentedAddress;  
                                        /* to choose, set choice to
                           * CMCC_3GCDR_CG_M_iPTextRepresentedAddress_chosen */
    } u;
} CMCC_3GCDR_CG_M_IPAddress;

typedef CMCC_3GCDR_CG_M_IPAddress CMCC_3GCDR_CG_M_GSNAddress;

typedef struct CMCC_3GCDR_CG_M_MSNetworkCapability {
    unsigned short  length;
    unsigned char   value[8];
} CMCC_3GCDR_CG_M_MSNetworkCapability;

typedef struct CMCC_3GCDR_CG_M_RoutingAreaCode {
    unsigned short  length;
    unsigned char   value[1];
} CMCC_3GCDR_CG_M_RoutingAreaCode;

typedef struct CMCC_3GCDR_CG_M_LocationAreaCode {
    unsigned short  length;
    unsigned char   value[2];
} CMCC_3GCDR_CG_M_LocationAreaCode;

typedef struct CMCC_3GCDR_CG_M_CellId {
    unsigned short  length;
    unsigned char   value[2];
} CMCC_3GCDR_CG_M_CellId;

typedef unsigned int    CMCC_3GCDR_CG_M_ChargingID;

typedef char            CMCC_3GCDR_CG_M_AccessPointNameNI[64];

typedef struct CMCC_3GCDR_CG_M_PDPType {
    unsigned short  length;
    unsigned char   value[2];
} CMCC_3GCDR_CG_M_PDPType;

typedef struct CMCC_3GCDR_CG_M_AddressString {
    unsigned short  length;
    unsigned char   value[20];
} CMCC_3GCDR_CG_M_AddressString;

typedef CMCC_3GCDR_CG_M_AddressString CMCC_3GCDR_CG_M_ETSIAddress;

typedef struct CMCC_3GCDR_CG_M_PDPAddress {
    unsigned short  choice;
#       define      CMCC_3GCDR_CG_M_iPAddress_chosen 1
#       define      CMCC_3GCDR_CG_M_eTSIAddress_chosen 2
    union _union {
        CMCC_3GCDR_CG_M_IPAddress iPAddress;  /* to choose, set choice to
                                          * CMCC_3GCDR_CG_M_iPAddress_chosen */
        CMCC_3GCDR_CG_M_ETSIAddress eTSIAddress;  /* to choose, set choice to
                                        * CMCC_3GCDR_CG_M_eTSIAddress_chosen */
    } u;
} CMCC_3GCDR_CG_M_PDPAddress;

typedef struct CMCC_3GCDR_CG_M_TimeStamp {
    unsigned short  length;
    unsigned char   value[9];
} CMCC_3GCDR_CG_M_TimeStamp;

typedef int             CMCC_3GCDR_CG_M_CallDuration;

typedef ossBoolean      CMCC_3GCDR_CG_M_SGSNChange;

typedef int             CMCC_3GCDR_CG_M_CauseForRecClosing;
#define                     CMCC_3GCDR_CG_M_normalRelease 0
#define                     CMCC_3GCDR_CG_M_abnormalRelease 4
#define                     CMCC_3GCDR_CG_M_cAMELInitCallRelease 5
#define                     CMCC_3GCDR_CG_M_volumeLimit 16
#define                     CMCC_3GCDR_CG_M_timeLimit 17
#define                     CMCC_3GCDR_CG_M_sGSNChange 18
#define                     CMCC_3GCDR_CG_M_maxChangeCond 19
#define                     CMCC_3GCDR_CG_M_managementIntervention 20

struct CMCC_3GCDR_CG_M__octet8 {
    unsigned int    length;
    unsigned char   *value;
}; 

typedef struct CMCC_3GCDR_CG_M_ManagementExtension {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_CG_M_significance_present 0x80
    CMCC_3GCDR_CG_M__ObjectID identifier;
    ossBoolean      significance;  /* CMCC_3GCDR_CG_M_significance_present not
                                    * set in bit_mask implies value is FALSE */
    struct CMCC_3GCDR_CG_M__octet8 information;
} CMCC_3GCDR_CG_M_ManagementExtension;

typedef struct CMCC_3GCDR_CG_M_Diagnostics {
    unsigned short  choice;
#       define      CMCC_3GCDR_CG_M_gsm0408Cause_chosen 1
#       define      CMCC_3GCDR_CG_M_gsm0902MapErrorValue_chosen 2
#       define      CMCC_3GCDR_CG_M_ccittQ767Cause_chosen 3
#       define      CMCC_3GCDR_CG_M_networkSpecificCause_chosen 4
#       define      CMCC_3GCDR_CG_M_manufacturerSpecificCause_chosen 5
    union _union {
        int             gsm0408Cause;  /* to choose, set choice to
                                       * CMCC_3GCDR_CG_M_gsm0408Cause_chosen */
        int             gsm0902MapErrorValue;  /* to choose, set choice to
                               * CMCC_3GCDR_CG_M_gsm0902MapErrorValue_chosen */
        int             ccittQ767Cause;  /* to choose, set choice to
                                     * CMCC_3GCDR_CG_M_ccittQ767Cause_chosen */
        CMCC_3GCDR_CG_M_ManagementExtension networkSpecificCause;  /* to choose,
                                   * set choice to
                               * CMCC_3GCDR_CG_M_networkSpecificCause_chosen */
        CMCC_3GCDR_CG_M_ManagementExtension manufacturerSpecificCause;  /* to
                                   * choose, set choice to
                          * CMCC_3GCDR_CG_M_manufacturerSpecificCause_chosen */
    } u;
} CMCC_3GCDR_CG_M_Diagnostics;

typedef char            CMCC_3GCDR_CG_M_NodeID[21];

typedef struct CMCC_3GCDR_CG_M_ManagementExtensions {
    unsigned int    count;
    CMCC_3GCDR_CG_M_ManagementExtension *value;
} CMCC_3GCDR_CG_M_ManagementExtensions;

typedef enum CMCC_3GCDR_CG_M_APNSelectionMode {
    CMCC_3GCDR_CG_M_mSorNetworkProvidedSubscriptionVerified = 0,
    CMCC_3GCDR_CG_M_mSProvidedSubscriptionNotVerified = 1,
    CMCC_3GCDR_CG_M_networkProvidedSubscriptionNotVerified = 2
} CMCC_3GCDR_CG_M_APNSelectionMode;

typedef char            CMCC_3GCDR_CG_M_AccessPointNameOI[38];

typedef struct CMCC_3GCDR_CG_M_ISDN_AddressString {
    unsigned short  length;
    unsigned char   value[9];
} CMCC_3GCDR_CG_M_ISDN_AddressString;

typedef CMCC_3GCDR_CG_M_ISDN_AddressString CMCC_3GCDR_CG_M_MSISDN;

typedef struct CMCC_3GCDR_CG_M_ChargingCharacteristics {
    unsigned short  length;
    unsigned char   value[2];
} CMCC_3GCDR_CG_M_ChargingCharacteristics;

typedef enum CMCC_3GCDR_CG_M_SystemType {
    CMCC_3GCDR_CG_M_unknown = 0,
    CMCC_3GCDR_CG_M_iuUTRAN = 1,
    CMCC_3GCDR_CG_M_gERAN = 2
} CMCC_3GCDR_CG_M_SystemType;

typedef CMCC_3GCDR_CG_M_AddressString CMCC_3GCDR_CG_M_SCFAddress;

typedef unsigned int    CMCC_3GCDR_CG_M_ServiceKey;

typedef enum CMCC_3GCDR_CG_M_DefaultGPRS_Handling {
    CMCC_3GCDR_CG_M_DefaultGPRS_Handling_continueTransaction = 0,
    CMCC_3GCDR_CG_M_DefaultGPRS_Handling_releaseTransaction = 1
} CMCC_3GCDR_CG_M_DefaultGPRS_Handling;

typedef CMCC_3GCDR_CG_M_AccessPointNameNI CMCC_3GCDR_CG_M_CAMELAccessPointNameNI;

typedef CMCC_3GCDR_CG_M_AccessPointNameOI CMCC_3GCDR_CG_M_CAMELAccessPointNameOI;

typedef int             CMCC_3GCDR_CG_M_NumberOfDPEncountered;

typedef unsigned char   CMCC_3GCDR_CG_M_LevelOfCAMELService;
#define                     CMCC_3GCDR_CG_M_basic 0x80
#define                     CMCC_3GCDR_CG_M_callDurationSupervision 0x40
#define                     CMCC_3GCDR_CG_M_onlineCharging 0x20

typedef struct CMCC_3GCDR_CG_M_FreeFormatData {
    unsigned short  length;
    unsigned char   value[160];
} CMCC_3GCDR_CG_M_FreeFormatData;

typedef ossBoolean      CMCC_3GCDR_CG_M_FFDAppendIndicator;

typedef struct CMCC_3GCDR_CG_M_CAMELInformationPDP {
    unsigned short  bit_mask;
#       define      CMCC_3GCDR_CG_M_CAMELInformationPDP_sCFAddress_present 0x8000
#       define      CMCC_3GCDR_CG_M_CAMELInformationPDP_serviceKey_present 0x4000
#       define      CMCC_3GCDR_CG_M_CAMELInformationPDP_defaultTransactionHandling_present 0x2000
#       define      CMCC_3GCDR_CG_M_cAMELAccessPointNameNI_present 0x1000
#       define      CMCC_3GCDR_CG_M_cAMELAccessPointNameOI_present 0x0800
#       define      CMCC_3GCDR_CG_M_CAMELInformationPDP_numberOfDPEncountered_present 0x0400
#       define      CMCC_3GCDR_CG_M_CAMELInformationPDP_levelOfCAMELService_present 0x0200
#       define      CMCC_3GCDR_CG_M_CAMELInformationPDP_freeFormatData_present 0x0100
#       define      CMCC_3GCDR_CG_M_CAMELInformationPDP_fFDAppendIndicator_present 0x0080
    CMCC_3GCDR_CG_M_SCFAddress sCFAddress;  /* optional; set in bit_mask
                    * CMCC_3GCDR_CG_M_CAMELInformationPDP_sCFAddress_present if
                    * present */
    CMCC_3GCDR_CG_M_ServiceKey serviceKey;  /* optional; set in bit_mask
                    * CMCC_3GCDR_CG_M_CAMELInformationPDP_serviceKey_present if
                    * present */
    CMCC_3GCDR_CG_M_DefaultGPRS_Handling defaultTransactionHandling;  
                                  /* optional; set in bit_mask
    * CMCC_3GCDR_CG_M_CAMELInformationPDP_defaultTransactionHandling_present if
    * present */
    CMCC_3GCDR_CG_M_CAMELAccessPointNameNI cAMELAccessPointNameNI;  
                                  /* optional; set in bit_mask
                            * CMCC_3GCDR_CG_M_cAMELAccessPointNameNI_present if
                            * present */
    CMCC_3GCDR_CG_M_CAMELAccessPointNameOI cAMELAccessPointNameOI;  
                                  /* optional; set in bit_mask
                            * CMCC_3GCDR_CG_M_cAMELAccessPointNameOI_present if
                            * present */
    CMCC_3GCDR_CG_M_NumberOfDPEncountered numberOfDPEncountered;  /* optional;
                                   * set in bit_mask
         * CMCC_3GCDR_CG_M_CAMELInformationPDP_numberOfDPEncountered_present if
         * present */
    CMCC_3GCDR_CG_M_LevelOfCAMELService levelOfCAMELService;  /* optional; set
                                   * in bit_mask
           * CMCC_3GCDR_CG_M_CAMELInformationPDP_levelOfCAMELService_present if
           * present */
    CMCC_3GCDR_CG_M_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                * CMCC_3GCDR_CG_M_CAMELInformationPDP_freeFormatData_present if
                * present */
    CMCC_3GCDR_CG_M_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in
                                   * bit_mask
            * CMCC_3GCDR_CG_M_CAMELInformationPDP_fFDAppendIndicator_present if
            * present */
} CMCC_3GCDR_CG_M_CAMELInformationPDP;

typedef enum CMCC_3GCDR_CG_M_ConsolidationResult {
    CMCC_3GCDR_CG_M_normal = 0,
    CMCC_3GCDR_CG_M_notnormal = 1,
    CMCC_3GCDR_CG_M_forInterSGSNConsolidation = 2,
    CMCC_3GCDR_CG_M_reachLimit = 3,
    CMCC_3GCDR_CG_M_onlyOneCDRGenerated = 4
} CMCC_3GCDR_CG_M_ConsolidationResult;

typedef struct CMCC_3GCDR_CG_M__seqof2 {
    unsigned int    count;
    CMCC_3GCDR_CG_M_GSNAddress *value;
} CMCC_3GCDR_CG_M__seqof2;

typedef struct CMCC_3GCDR_CG_M__seqof3 {
    unsigned int    count;
    struct CMCC_3GCDR_CG_M_ChangeOfCharCondition *value;
} CMCC_3GCDR_CG_M__seqof3;

struct CMCC_3GCDR_CG_M__seqof4 {
    unsigned int    count;
    struct CMCC_3GCDR_CG_M_SequenceList *value;
}; 

struct CMCC_3GCDR_CG_M__seqof5 {
    unsigned int    count;
    struct CMCC_3GCDR_CG_M_LocalSequenceNumberList *value;
}; 

struct CMCC_3GCDR_CG_M__seqof6 {
    unsigned int    count;
    struct CMCC_3GCDR_CG_M_RNCUnsentDownlinkVolumeList *value;
}; 

typedef struct CMCC_3GCDR_CG_M_SGSNPDPRecord {
    unsigned int    bit_mask;
#       define      CMCC_3GCDR_CG_M_SGSNPDPRecord_networkInitiation_present 0x80000000
#       define      CMCC_3GCDR_CG_M_SGSNPDPRecord_servedIMEI_present 0x40000000
#       define      CMCC_3GCDR_CG_M_SGSNPDPRecord_msNetworkCapability_present 0x20000000
#       define      CMCC_3GCDR_CG_M_SGSNPDPRecord_routingArea_present 0x10000000
#       define      CMCC_3GCDR_CG_M_SGSNPDPRecord_locationAreaCode_present 0x08000000
#       define      CMCC_3GCDR_CG_M_SGSNPDPRecord_cellIdentifier_present 0x04000000
#       define      CMCC_3GCDR_CG_M_SGSNPDPRecord_servedPDPAddress_present 0x02000000
#       define      CMCC_3GCDR_CG_M_SGSNPDPRecord_sgsnChange_present 0x01000000
#       define      CMCC_3GCDR_CG_M_SGSNPDPRecord_diagnostics_present 0x00800000
#       define      CMCC_3GCDR_CG_M_SGSNPDPRecord_recSequenceNumList_present 0x00400000
#       define      CMCC_3GCDR_CG_M_SGSNPDPRecord_nodeID_present 0x00200000
#       define      CMCC_3GCDR_CG_M_SGSNPDPRecord_recordExtensions_present 0x00100000
#       define      CMCC_3GCDR_CG_M_SGSNPDPRecord_localSequenceNumber_present 0x00080000
#       define      CMCC_3GCDR_CG_M_SGSNPDPRecord_apnSelectionMode_present 0x00040000
#       define      CMCC_3GCDR_CG_M_SGSNPDPRecord_servedMSISDN_present 0x00020000
#       define      CMCC_3GCDR_CG_M_SGSNPDPRecord_chargingCharacteristics_present 0x00010000
#       define      CMCC_3GCDR_CG_M_SGSNPDPRecord_systemType_present 0x00008000
#       define      CMCC_3GCDR_CG_M_cAMELInformationPDP_present 0x00004000
#       define      CMCC_3GCDR_CG_M_rNCUnsentDownlinkVolumeList_present 0x00002000
    CMCC_3GCDR_CG_M_CallEventRecordType recordType;
    CMCC_3GCDR_CG_M_NetworkInitiatedPDPContext networkInitiation;  /* optional;
                                   * set in bit_mask
                   * CMCC_3GCDR_CG_M_SGSNPDPRecord_networkInitiation_present if
                   * present */
    CMCC_3GCDR_CG_M_IMSI servedIMSI;
    CMCC_3GCDR_CG_M_IMEI servedIMEI;  /* optional; set in bit_mask
                          * CMCC_3GCDR_CG_M_SGSNPDPRecord_servedIMEI_present if
                          * present */
    CMCC_3GCDR_CG_M__seqof2 sgsnAddress;
    CMCC_3GCDR_CG_M_MSNetworkCapability msNetworkCapability;  /* optional; set
                                   * in bit_mask
                 * CMCC_3GCDR_CG_M_SGSNPDPRecord_msNetworkCapability_present if
                 * present */
    CMCC_3GCDR_CG_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                         * CMCC_3GCDR_CG_M_SGSNPDPRecord_routingArea_present if
                         * present */
    CMCC_3GCDR_CG_M_LocationAreaCode locationAreaCode;  /* optional; set in
                                   * bit_mask
                    * CMCC_3GCDR_CG_M_SGSNPDPRecord_locationAreaCode_present if
                    * present */
    CMCC_3GCDR_CG_M_CellId cellIdentifier;  /* optional; set in bit_mask
                      * CMCC_3GCDR_CG_M_SGSNPDPRecord_cellIdentifier_present if
                      * present */
    CMCC_3GCDR_CG_M_ChargingID chargingID;
    CMCC_3GCDR_CG_M_GSNAddress ggsnAddressUsed;
    CMCC_3GCDR_CG_M_AccessPointNameNI accessPointNameNI;
    CMCC_3GCDR_CG_M_PDPType pdpType;
    CMCC_3GCDR_CG_M_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                    * CMCC_3GCDR_CG_M_SGSNPDPRecord_servedPDPAddress_present if
                    * present */
    CMCC_3GCDR_CG_M__seqof3 listOfTrafficVolumes;
    CMCC_3GCDR_CG_M_TimeStamp recordOpeningTime;
    CMCC_3GCDR_CG_M_CallDuration duration;
    CMCC_3GCDR_CG_M_SGSNChange sgsnChange;  /* optional; set in bit_mask
                          * CMCC_3GCDR_CG_M_SGSNPDPRecord_sgsnChange_present if
                          * present */
    CMCC_3GCDR_CG_M_CauseForRecClosing causeForRecClosing;
    CMCC_3GCDR_CG_M_Diagnostics diagnostics;  /* optional; set in bit_mask
                         * CMCC_3GCDR_CG_M_SGSNPDPRecord_diagnostics_present if
                         * present */
    struct CMCC_3GCDR_CG_M__seqof4 recSequenceNumList;  /* optional; set in bit_mask
                  * CMCC_3GCDR_CG_M_SGSNPDPRecord_recSequenceNumList_present if
                  * present */
    CMCC_3GCDR_CG_M_NodeID nodeID;  /* optional; set in bit_mask
                              * CMCC_3GCDR_CG_M_SGSNPDPRecord_nodeID_present if
                              * present */
    CMCC_3GCDR_CG_M_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                    * CMCC_3GCDR_CG_M_SGSNPDPRecord_recordExtensions_present if
                    * present */
    struct CMCC_3GCDR_CG_M__seqof5 localSequenceNumber;  /* optional; set in bit_mask
                 * CMCC_3GCDR_CG_M_SGSNPDPRecord_localSequenceNumber_present if
                 * present */
    CMCC_3GCDR_CG_M_APNSelectionMode apnSelectionMode;  /* optional; set in
                                   * bit_mask
                    * CMCC_3GCDR_CG_M_SGSNPDPRecord_apnSelectionMode_present if
                    * present */
    CMCC_3GCDR_CG_M_AccessPointNameOI accessPointNameOI;
    CMCC_3GCDR_CG_M_MSISDN servedMSISDN;  /* optional; set in bit_mask
                        * CMCC_3GCDR_CG_M_SGSNPDPRecord_servedMSISDN_present if
                        * present */
    CMCC_3GCDR_CG_M_ChargingCharacteristics chargingCharacteristics;  
                                  /* optional; set in bit_mask
             * CMCC_3GCDR_CG_M_SGSNPDPRecord_chargingCharacteristics_present if
             * present */
    CMCC_3GCDR_CG_M_SystemType systemType;  /* optional; set in bit_mask
                          * CMCC_3GCDR_CG_M_SGSNPDPRecord_systemType_present if
                          * present */
    CMCC_3GCDR_CG_M_CAMELInformationPDP cAMELInformationPDP;  /* optional; set
                                   * in bit_mask
                                   * CMCC_3GCDR_CG_M_cAMELInformationPDP_present
                                   * if present */
    struct CMCC_3GCDR_CG_M__seqof6 rNCUnsentDownlinkVolumeList;  /* optional; set in bit_mask
                       * CMCC_3GCDR_CG_M_rNCUnsentDownlinkVolumeList_present if
                       * present */
    CMCC_3GCDR_CG_M_ConsolidationResult consolidationResult;
} CMCC_3GCDR_CG_M_SGSNPDPRecord;

typedef ossBoolean      CMCC_3GCDR_CG_M_DynamicAddressFlag;

typedef struct CMCC_3GCDR_CG_M__seqof7 {
    unsigned int    count;
    int             *value;
} CMCC_3GCDR_CG_M__seqof7;

typedef struct CMCC_3GCDR_CG_M_SequenceList {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_CG_M_recSequenceNumber_present 0x80
    CMCC_3GCDR_CG_M_GSNAddress sgsnAddress;
    CMCC_3GCDR_CG_M__seqof7 recSequenceNumber;  /* optional; set in bit_mask
                                 * CMCC_3GCDR_CG_M_recSequenceNumber_present if
                                 * present */
} CMCC_3GCDR_CG_M_SequenceList;

typedef struct CMCC_3GCDR_CG_M_LocalSequenceNumberList {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_CG_M_localSeqNumberList_present 0x80
    CMCC_3GCDR_CG_M_GSNAddress gsnAddress;
    CMCC_3GCDR_CG_M__seqof7 localSeqNumberList;  /* optional; set in bit_mask
                                * CMCC_3GCDR_CG_M_localSeqNumberList_present if
                                * present */
} CMCC_3GCDR_CG_M_LocalSequenceNumberList;

typedef struct CMCC_3GCDR_CG_M_PLMN_Id {
    unsigned short  length;
    unsigned char   value[3];
} CMCC_3GCDR_CG_M_PLMN_Id;

typedef struct CMCC_3GCDR_CG_M_GGSNPDPRecord {
    unsigned short  bit_mask;
#       define      CMCC_3GCDR_CG_M_GGSNPDPRecord_networkInitiation_present 0x8000
#       define      CMCC_3GCDR_CG_M_GGSNPDPRecord_servedPDPAddress_present 0x4000
#       define      CMCC_3GCDR_CG_M_dynamicAddressFlag_present 0x2000
#       define      CMCC_3GCDR_CG_M_GGSNPDPRecord_diagnostics_present 0x1000
#       define      CMCC_3GCDR_CG_M_GGSNPDPRecord_recSequenceNumList_present 0x0800
#       define      CMCC_3GCDR_CG_M_GGSNPDPRecord_nodeID_present 0x0400
#       define      CMCC_3GCDR_CG_M_GGSNPDPRecord_recordExtensions_present 0x0200
#       define      CMCC_3GCDR_CG_M_localSequenceNumberList_present 0x0100
#       define      CMCC_3GCDR_CG_M_GGSNPDPRecord_apnSelectionMode_present 0x0080
#       define      CMCC_3GCDR_CG_M_GGSNPDPRecord_servedMSISDN_present 0x0040
#       define      CMCC_3GCDR_CG_M_GGSNPDPRecord_chargingCharacteristics_present 0x0020
#       define      CMCC_3GCDR_CG_M_sgsnPLMNIdentifier_present 0x0010
    CMCC_3GCDR_CG_M_CallEventRecordType recordType;
    CMCC_3GCDR_CG_M_NetworkInitiatedPDPContext networkInitiation;  /* optional;
                                   * set in bit_mask
                   * CMCC_3GCDR_CG_M_GGSNPDPRecord_networkInitiation_present if
                   * present */
    CMCC_3GCDR_CG_M_IMSI servedIMSI;
    CMCC_3GCDR_CG_M_GSNAddress ggsnAddress;
    CMCC_3GCDR_CG_M_ChargingID chargingID;
    CMCC_3GCDR_CG_M__seqof2 sgsnAddress;
    CMCC_3GCDR_CG_M_AccessPointNameNI accessPointNameNI;
    CMCC_3GCDR_CG_M_PDPType pdpType;
    CMCC_3GCDR_CG_M_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                    * CMCC_3GCDR_CG_M_GGSNPDPRecord_servedPDPAddress_present if
                    * present */
    CMCC_3GCDR_CG_M_DynamicAddressFlag dynamicAddressFlag;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_CG_M_dynamicAddressFlag_present
                                   * if present */
    CMCC_3GCDR_CG_M__seqof3 listOfTrafficVolumes;
    CMCC_3GCDR_CG_M_TimeStamp recordOpeningTime;
    CMCC_3GCDR_CG_M_CallDuration duration;
    CMCC_3GCDR_CG_M_CauseForRecClosing causeForRecClosing;
    CMCC_3GCDR_CG_M_Diagnostics diagnostics;  /* optional; set in bit_mask
                         * CMCC_3GCDR_CG_M_GGSNPDPRecord_diagnostics_present if
                         * present */
    CMCC_3GCDR_CG_M_SequenceList recSequenceNumList;  /* optional; set in
                                   * bit_mask
                  * CMCC_3GCDR_CG_M_GGSNPDPRecord_recSequenceNumList_present if
                  * present */
    CMCC_3GCDR_CG_M_NodeID nodeID;  /* optional; set in bit_mask
                              * CMCC_3GCDR_CG_M_GGSNPDPRecord_nodeID_present if
                              * present */
    CMCC_3GCDR_CG_M_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                    * CMCC_3GCDR_CG_M_GGSNPDPRecord_recordExtensions_present if
                    * present */
    CMCC_3GCDR_CG_M_LocalSequenceNumberList localSequenceNumberList;  
                                  /* optional; set in bit_mask
                           * CMCC_3GCDR_CG_M_localSequenceNumberList_present if
                           * present */
    CMCC_3GCDR_CG_M_APNSelectionMode apnSelectionMode;  /* optional; set in
                                   * bit_mask
                    * CMCC_3GCDR_CG_M_GGSNPDPRecord_apnSelectionMode_present if
                    * present */
    CMCC_3GCDR_CG_M_MSISDN servedMSISDN;  /* optional; set in bit_mask
                        * CMCC_3GCDR_CG_M_GGSNPDPRecord_servedMSISDN_present if
                        * present */
    CMCC_3GCDR_CG_M_ChargingCharacteristics chargingCharacteristics;  
                                  /* optional; set in bit_mask
             * CMCC_3GCDR_CG_M_GGSNPDPRecord_chargingCharacteristics_present if
             * present */
    CMCC_3GCDR_CG_M_PLMN_Id sgsnPLMNIdentifier;  /* optional; set in bit_mask
                                * CMCC_3GCDR_CG_M_sgsnPLMNIdentifier_present if
                                * present */
    CMCC_3GCDR_CG_M_ConsolidationResult consolidationResult;
} CMCC_3GCDR_CG_M_GGSNPDPRecord;

typedef unsigned int    CMCC_3GCDR_CG_M_LocalSequenceNumber;

typedef struct CMCC_3GCDR_CG_M_CAMELInformationMM {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_CG_M_CAMELInformationMM_sCFAddress_present 0x80
#       define      CMCC_3GCDR_CG_M_CAMELInformationMM_serviceKey_present 0x40
#       define      CMCC_3GCDR_CG_M_CAMELInformationMM_defaultTransactionHandling_present 0x20
#       define      CMCC_3GCDR_CG_M_CAMELInformationMM_numberOfDPEncountered_present 0x10
#       define      CMCC_3GCDR_CG_M_CAMELInformationMM_levelOfCAMELService_present 0x08
#       define      CMCC_3GCDR_CG_M_CAMELInformationMM_freeFormatData_present 0x04
#       define      CMCC_3GCDR_CG_M_CAMELInformationMM_fFDAppendIndicator_present 0x02
    CMCC_3GCDR_CG_M_SCFAddress sCFAddress;  /* optional; set in bit_mask
                     * CMCC_3GCDR_CG_M_CAMELInformationMM_sCFAddress_present if
                     * present */
    CMCC_3GCDR_CG_M_ServiceKey serviceKey;  /* optional; set in bit_mask
                     * CMCC_3GCDR_CG_M_CAMELInformationMM_serviceKey_present if
                     * present */
    CMCC_3GCDR_CG_M_DefaultGPRS_Handling defaultTransactionHandling;  
                                  /* optional; set in bit_mask
     * CMCC_3GCDR_CG_M_CAMELInformationMM_defaultTransactionHandling_present if
     * present */
    CMCC_3GCDR_CG_M_NumberOfDPEncountered numberOfDPEncountered;  /* optional;
                                   * set in bit_mask
          * CMCC_3GCDR_CG_M_CAMELInformationMM_numberOfDPEncountered_present if
          * present */
    CMCC_3GCDR_CG_M_LevelOfCAMELService levelOfCAMELService;  /* optional; set
                                   * in bit_mask
            * CMCC_3GCDR_CG_M_CAMELInformationMM_levelOfCAMELService_present if
            * present */
    CMCC_3GCDR_CG_M_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                 * CMCC_3GCDR_CG_M_CAMELInformationMM_freeFormatData_present if
                 * present */
    CMCC_3GCDR_CG_M_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in
                                   * bit_mask
             * CMCC_3GCDR_CG_M_CAMELInformationMM_fFDAppendIndicator_present if
             * present */
} CMCC_3GCDR_CG_M_CAMELInformationMM;

typedef struct CMCC_3GCDR_CG_M_SGSNMMRecord {
    unsigned int    bit_mask;
#       define      CMCC_3GCDR_CG_M_SGSNMMRecord_servedIMEI_present 0x80000000
#       define      CMCC_3GCDR_CG_M_SGSNMMRecord_msNetworkCapability_present 0x40000000
#       define      CMCC_3GCDR_CG_M_SGSNMMRecord_routingArea_present 0x20000000
#       define      CMCC_3GCDR_CG_M_SGSNMMRecord_locationAreaCode_present 0x10000000
#       define      CMCC_3GCDR_CG_M_SGSNMMRecord_cellIdentifier_present 0x08000000
#       define      CMCC_3GCDR_CG_M_changeLocation_present 0x04000000
#       define      CMCC_3GCDR_CG_M_duration_present 0x02000000
#       define      CMCC_3GCDR_CG_M_SGSNMMRecord_sgsnChange_present 0x01000000
#       define      CMCC_3GCDR_CG_M_SGSNMMRecord_diagnostics_present 0x00800000
#       define      CMCC_3GCDR_CG_M_recordSequenceNumber_present 0x00400000
#       define      CMCC_3GCDR_CG_M_SGSNMMRecord_nodeID_present 0x00200000
#       define      CMCC_3GCDR_CG_M_SGSNMMRecord_recordExtensions_present 0x00100000
#       define      CMCC_3GCDR_CG_M_SGSNMMRecord_localSequenceNumber_present 0x00080000
#       define      CMCC_3GCDR_CG_M_SGSNMMRecord_servedMSISDN_present 0x00040000
#       define      CMCC_3GCDR_CG_M_SGSNMMRecord_chargingCharacteristics_present 0x00020000
#       define      CMCC_3GCDR_CG_M_cAMELInformationMM_present 0x00010000
#       define      CMCC_3GCDR_CG_M_SGSNMMRecord_systemType_present 0x00008000
    CMCC_3GCDR_CG_M_CallEventRecordType recordType;
    CMCC_3GCDR_CG_M_IMSI servedIMSI;
    CMCC_3GCDR_CG_M_IMEI servedIMEI;  /* optional; set in bit_mask
                           * CMCC_3GCDR_CG_M_SGSNMMRecord_servedIMEI_present if
                           * present */
    CMCC_3GCDR_CG_M_GSNAddress sgsnAddress;
    CMCC_3GCDR_CG_M_MSNetworkCapability msNetworkCapability;  /* optional; set
                                   * in bit_mask
                  * CMCC_3GCDR_CG_M_SGSNMMRecord_msNetworkCapability_present if
                  * present */
    CMCC_3GCDR_CG_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                          * CMCC_3GCDR_CG_M_SGSNMMRecord_routingArea_present if
                          * present */
    CMCC_3GCDR_CG_M_LocationAreaCode locationAreaCode;  /* optional; set in
                                   * bit_mask
                     * CMCC_3GCDR_CG_M_SGSNMMRecord_locationAreaCode_present if
                     * present */
    CMCC_3GCDR_CG_M_CellId cellIdentifier;  /* optional; set in bit_mask
                       * CMCC_3GCDR_CG_M_SGSNMMRecord_cellIdentifier_present if
                       * present */
    struct CMCC_3GCDR_CG_M__seqof8 {
        unsigned int    count;
        struct CMCC_3GCDR_CG_M_ChangeLocation *value;
    } changeLocation;  /* optional; set in bit_mask
                        * CMCC_3GCDR_CG_M_changeLocation_present if present */
    CMCC_3GCDR_CG_M_TimeStamp recordOpeningTime;
    CMCC_3GCDR_CG_M_CallDuration duration;  /* optional; set in bit_mask
                                             * CMCC_3GCDR_CG_M_duration_present
                                             * if present */
    CMCC_3GCDR_CG_M_SGSNChange sgsnChange;  /* optional; set in bit_mask
                           * CMCC_3GCDR_CG_M_SGSNMMRecord_sgsnChange_present if
                           * present */
    CMCC_3GCDR_CG_M_CauseForRecClosing causeForRecClosing;
    CMCC_3GCDR_CG_M_Diagnostics diagnostics;  /* optional; set in bit_mask
                          * CMCC_3GCDR_CG_M_SGSNMMRecord_diagnostics_present if
                          * present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                              * CMCC_3GCDR_CG_M_recordSequenceNumber_present if
                              * present */
    CMCC_3GCDR_CG_M_NodeID nodeID;  /* optional; set in bit_mask
                               * CMCC_3GCDR_CG_M_SGSNMMRecord_nodeID_present if
                               * present */
    CMCC_3GCDR_CG_M_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                     * CMCC_3GCDR_CG_M_SGSNMMRecord_recordExtensions_present if
                     * present */
    CMCC_3GCDR_CG_M_LocalSequenceNumber localSequenceNumber;  /* optional; set
                                   * in bit_mask
                  * CMCC_3GCDR_CG_M_SGSNMMRecord_localSequenceNumber_present if
                  * present */
    CMCC_3GCDR_CG_M_MSISDN servedMSISDN;  /* optional; set in bit_mask
                         * CMCC_3GCDR_CG_M_SGSNMMRecord_servedMSISDN_present if
                         * present */
    CMCC_3GCDR_CG_M_ChargingCharacteristics chargingCharacteristics;  
                                  /* optional; set in bit_mask
              * CMCC_3GCDR_CG_M_SGSNMMRecord_chargingCharacteristics_present if
              * present */
    CMCC_3GCDR_CG_M_CAMELInformationMM cAMELInformationMM;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_CG_M_cAMELInformationMM_present
                                   * if present */
    CMCC_3GCDR_CG_M_SystemType systemType;  /* optional; set in bit_mask
                           * CMCC_3GCDR_CG_M_SGSNMMRecord_systemType_present if
                           * present */
} CMCC_3GCDR_CG_M_SGSNMMRecord;

typedef CMCC_3GCDR_CG_M_AddressString CMCC_3GCDR_CG_M_RecordingEntity;

typedef struct CMCC_3GCDR_CG_M_MessageReference {
    unsigned int    length;
    unsigned char   *value;
} CMCC_3GCDR_CG_M_MessageReference;

typedef CMCC_3GCDR_CG_M_Diagnostics CMCC_3GCDR_CG_M_SMSResult;

typedef struct CMCC_3GCDR_CG_M_CalledNumber {
    unsigned short  length;
    unsigned char   value[10];
} CMCC_3GCDR_CG_M_CalledNumber;

typedef enum CMCC_3GCDR_CG_M_DefaultSMS_Handling {
    CMCC_3GCDR_CG_M_DefaultSMS_Handling_continueTransaction = 0,
    CMCC_3GCDR_CG_M_DefaultSMS_Handling_releaseTransaction = 1
} CMCC_3GCDR_CG_M_DefaultSMS_Handling;

typedef struct CMCC_3GCDR_CG_M_CallingNumber {
    unsigned short  length;
    unsigned char   value[10];
} CMCC_3GCDR_CG_M_CallingNumber;

typedef struct CMCC_3GCDR_CG_M_CallReferenceNumber {
    unsigned short  length;
    unsigned char   value[8];
} CMCC_3GCDR_CG_M_CallReferenceNumber;

typedef struct CMCC_3GCDR_CG_M_CAMELInformationSMS {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_CG_M_CAMELInformationSMS_sCFAddress_present 0x80
#       define      CMCC_3GCDR_CG_M_CAMELInformationSMS_serviceKey_present 0x40
#       define      CMCC_3GCDR_CG_M_defaultSMSHandling_present 0x20
#       define      CMCC_3GCDR_CG_M_cAMELCallingPartyNumber_present 0x10
#       define      CMCC_3GCDR_CG_M_cAMELDestinationSubscriberNumber_present 0x08
#       define      CMCC_3GCDR_CG_M_cAMELSMSCAddress_present 0x04
#       define      CMCC_3GCDR_CG_M_CAMELInformationSMS_freeFormatData_present 0x02
#       define      CMCC_3GCDR_CG_M_sMSReferenceNumber_present 0x01
    CMCC_3GCDR_CG_M_SCFAddress sCFAddress;  /* optional; set in bit_mask
                    * CMCC_3GCDR_CG_M_CAMELInformationSMS_sCFAddress_present if
                    * present */
    CMCC_3GCDR_CG_M_ServiceKey serviceKey;  /* optional; set in bit_mask
                    * CMCC_3GCDR_CG_M_CAMELInformationSMS_serviceKey_present if
                    * present */
    CMCC_3GCDR_CG_M_DefaultSMS_Handling defaultSMSHandling;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_CG_M_defaultSMSHandling_present
                                   * if present */
    CMCC_3GCDR_CG_M_CallingNumber cAMELCallingPartyNumber;  /* optional; set in
                                   * bit_mask
                           * CMCC_3GCDR_CG_M_cAMELCallingPartyNumber_present if
                           * present */
    CMCC_3GCDR_CG_M_CalledNumber cAMELDestinationSubscriberNumber;  
                                  /* optional; set in bit_mask
                  * CMCC_3GCDR_CG_M_cAMELDestinationSubscriberNumber_present if
                  * present */
    CMCC_3GCDR_CG_M_AddressString cAMELSMSCAddress;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_CG_M_cAMELSMSCAddress_present if
                                   * present */
    CMCC_3GCDR_CG_M_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                * CMCC_3GCDR_CG_M_CAMELInformationSMS_freeFormatData_present if
                * present */
    CMCC_3GCDR_CG_M_CallReferenceNumber sMSReferenceNumber;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_CG_M_sMSReferenceNumber_present
                                   * if present */
} CMCC_3GCDR_CG_M_CAMELInformationSMS;

typedef struct CMCC_3GCDR_CG_M_SGSNSMORecord {
    unsigned short  bit_mask;
#       define      CMCC_3GCDR_CG_M_SGSNSMORecord_servedIMEI_present 0x8000
#       define      CMCC_3GCDR_CG_M_SGSNSMORecord_servedMSISDN_present 0x4000
#       define      CMCC_3GCDR_CG_M_SGSNSMORecord_locationArea_present 0x2000
#       define      CMCC_3GCDR_CG_M_SGSNSMORecord_routingArea_present 0x1000
#       define      CMCC_3GCDR_CG_M_SGSNSMORecord_cellIdentifier_present 0x0800
#       define      CMCC_3GCDR_CG_M_SGSNSMORecord_smsResult_present 0x0400
#       define      CMCC_3GCDR_CG_M_SGSNSMORecord_recordExtensions_present 0x0200
#       define      CMCC_3GCDR_CG_M_SGSNSMORecord_nodeID_present 0x0100
#       define      CMCC_3GCDR_CG_M_SGSNSMORecord_localSequenceNumber_present 0x0080
#       define      CMCC_3GCDR_CG_M_SGSNSMORecord_chargingCharacteristics_present 0x0040
#       define      CMCC_3GCDR_CG_M_SGSNSMORecord_systemType_present 0x0020
#       define      CMCC_3GCDR_CG_M_destinationNumber_present 0x0010
#       define      CMCC_3GCDR_CG_M_cAMELInformationSMS_present 0x0008
    CMCC_3GCDR_CG_M_CallEventRecordType recordType;
    CMCC_3GCDR_CG_M_IMSI servedIMSI;
    CMCC_3GCDR_CG_M_IMEI servedIMEI;  /* optional; set in bit_mask
                          * CMCC_3GCDR_CG_M_SGSNSMORecord_servedIMEI_present if
                          * present */
    CMCC_3GCDR_CG_M_MSISDN servedMSISDN;  /* optional; set in bit_mask
                        * CMCC_3GCDR_CG_M_SGSNSMORecord_servedMSISDN_present if
                        * present */
    CMCC_3GCDR_CG_M_MSNetworkCapability msNetworkCapability;
    CMCC_3GCDR_CG_M_AddressString serviceCentre;
    CMCC_3GCDR_CG_M_RecordingEntity recordingEntity;
    CMCC_3GCDR_CG_M_LocationAreaCode locationArea;  /* optional; set in bit_mask
                        * CMCC_3GCDR_CG_M_SGSNSMORecord_locationArea_present if
                        * present */
    CMCC_3GCDR_CG_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                         * CMCC_3GCDR_CG_M_SGSNSMORecord_routingArea_present if
                         * present */
    CMCC_3GCDR_CG_M_CellId cellIdentifier;  /* optional; set in bit_mask
                      * CMCC_3GCDR_CG_M_SGSNSMORecord_cellIdentifier_present if
                      * present */
    CMCC_3GCDR_CG_M_MessageReference messageReference;
    CMCC_3GCDR_CG_M_TimeStamp originationTime;
    CMCC_3GCDR_CG_M_SMSResult smsResult;  /* optional; set in bit_mask
                           * CMCC_3GCDR_CG_M_SGSNSMORecord_smsResult_present if
                           * present */
    CMCC_3GCDR_CG_M_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                    * CMCC_3GCDR_CG_M_SGSNSMORecord_recordExtensions_present if
                    * present */
    CMCC_3GCDR_CG_M_NodeID nodeID;  /* optional; set in bit_mask
                              * CMCC_3GCDR_CG_M_SGSNSMORecord_nodeID_present if
                              * present */
    CMCC_3GCDR_CG_M_LocalSequenceNumber localSequenceNumber;  /* optional; set
                                   * in bit_mask
                 * CMCC_3GCDR_CG_M_SGSNSMORecord_localSequenceNumber_present if
                 * present */
    CMCC_3GCDR_CG_M_ChargingCharacteristics chargingCharacteristics;  
                                  /* optional; set in bit_mask
             * CMCC_3GCDR_CG_M_SGSNSMORecord_chargingCharacteristics_present if
             * present */
    CMCC_3GCDR_CG_M_SystemType systemType;  /* optional; set in bit_mask
                          * CMCC_3GCDR_CG_M_SGSNSMORecord_systemType_present if
                          * present */
    CMCC_3GCDR_CG_M_CalledNumber destinationNumber;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_CG_M_destinationNumber_present
                                   * if present */
    CMCC_3GCDR_CG_M_CAMELInformationSMS cAMELInformationSMS;  /* optional; set
                                   * in bit_mask
                                   * CMCC_3GCDR_CG_M_cAMELInformationSMS_present
                                   * if present */
} CMCC_3GCDR_CG_M_SGSNSMORecord;

typedef struct CMCC_3GCDR_CG_M_SGSNSMTRecord {
    unsigned short  bit_mask;
#       define      CMCC_3GCDR_CG_M_SGSNSMTRecord_servedIMEI_present 0x8000
#       define      CMCC_3GCDR_CG_M_SGSNSMTRecord_servedMSISDN_present 0x4000
#       define      CMCC_3GCDR_CG_M_SGSNSMTRecord_locationArea_present 0x2000
#       define      CMCC_3GCDR_CG_M_SGSNSMTRecord_routingArea_present 0x1000
#       define      CMCC_3GCDR_CG_M_SGSNSMTRecord_cellIdentifier_present 0x0800
#       define      CMCC_3GCDR_CG_M_SGSNSMTRecord_smsResult_present 0x0400
#       define      CMCC_3GCDR_CG_M_SGSNSMTRecord_recordExtensions_present 0x0200
#       define      CMCC_3GCDR_CG_M_SGSNSMTRecord_nodeID_present 0x0100
#       define      CMCC_3GCDR_CG_M_SGSNSMTRecord_localSequenceNumber_present 0x0080
#       define      CMCC_3GCDR_CG_M_SGSNSMTRecord_chargingCharacteristics_present 0x0040
#       define      CMCC_3GCDR_CG_M_SGSNSMTRecord_systemType_present 0x0020
    CMCC_3GCDR_CG_M_CallEventRecordType recordType;
    CMCC_3GCDR_CG_M_IMSI servedIMSI;
    CMCC_3GCDR_CG_M_IMEI servedIMEI;  /* optional; set in bit_mask
                          * CMCC_3GCDR_CG_M_SGSNSMTRecord_servedIMEI_present if
                          * present */
    CMCC_3GCDR_CG_M_MSISDN servedMSISDN;  /* optional; set in bit_mask
                        * CMCC_3GCDR_CG_M_SGSNSMTRecord_servedMSISDN_present if
                        * present */
    CMCC_3GCDR_CG_M_MSNetworkCapability msNetworkCapability;
    CMCC_3GCDR_CG_M_AddressString serviceCentre;
    CMCC_3GCDR_CG_M_RecordingEntity recordingEntity;
    CMCC_3GCDR_CG_M_LocationAreaCode locationArea;  /* optional; set in bit_mask
                        * CMCC_3GCDR_CG_M_SGSNSMTRecord_locationArea_present if
                        * present */
    CMCC_3GCDR_CG_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                         * CMCC_3GCDR_CG_M_SGSNSMTRecord_routingArea_present if
                         * present */
    CMCC_3GCDR_CG_M_CellId cellIdentifier;  /* optional; set in bit_mask
                      * CMCC_3GCDR_CG_M_SGSNSMTRecord_cellIdentifier_present if
                      * present */
    CMCC_3GCDR_CG_M_TimeStamp originationTime;
    CMCC_3GCDR_CG_M_SMSResult smsResult;  /* optional; set in bit_mask
                           * CMCC_3GCDR_CG_M_SGSNSMTRecord_smsResult_present if
                           * present */
    CMCC_3GCDR_CG_M_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                    * CMCC_3GCDR_CG_M_SGSNSMTRecord_recordExtensions_present if
                    * present */
    CMCC_3GCDR_CG_M_NodeID nodeID;  /* optional; set in bit_mask
                              * CMCC_3GCDR_CG_M_SGSNSMTRecord_nodeID_present if
                              * present */
    CMCC_3GCDR_CG_M_LocalSequenceNumber localSequenceNumber;  /* optional; set
                                   * in bit_mask
                 * CMCC_3GCDR_CG_M_SGSNSMTRecord_localSequenceNumber_present if
                 * present */
    CMCC_3GCDR_CG_M_ChargingCharacteristics chargingCharacteristics;  
                                  /* optional; set in bit_mask
             * CMCC_3GCDR_CG_M_SGSNSMTRecord_chargingCharacteristics_present if
             * present */
    CMCC_3GCDR_CG_M_SystemType systemType;  /* optional; set in bit_mask
                          * CMCC_3GCDR_CG_M_SGSNSMTRecord_systemType_present if
                          * present */
} CMCC_3GCDR_CG_M_SGSNSMTRecord;

typedef struct CMCC_3GCDR_CG_M_CallEventRecord {
    unsigned short  choice;
#       define      CMCC_3GCDR_CG_M_sgsnPDPRecord_chosen 1
#       define      CMCC_3GCDR_CG_M_ggsnPDPRecord_chosen 2
#       define      CMCC_3GCDR_CG_M_sgsnMMRecord_chosen 3
#       define      CMCC_3GCDR_CG_M_sgsnSMORecord_chosen 4
#       define      CMCC_3GCDR_CG_M_sgsnSMTRecord_chosen 5
    union _union {
        CMCC_3GCDR_CG_M_SGSNPDPRecord sgsnPDPRecord;  /* to choose, set choice
                                   * to CMCC_3GCDR_CG_M_sgsnPDPRecord_chosen */
        CMCC_3GCDR_CG_M_GGSNPDPRecord ggsnPDPRecord;  /* to choose, set choice
                                   * to CMCC_3GCDR_CG_M_ggsnPDPRecord_chosen */
        CMCC_3GCDR_CG_M_SGSNMMRecord sgsnMMRecord;  /* to choose, set choice to
                                       * CMCC_3GCDR_CG_M_sgsnMMRecord_chosen */
        CMCC_3GCDR_CG_M_SGSNSMORecord sgsnSMORecord;  /* to choose, set choice
                                   * to CMCC_3GCDR_CG_M_sgsnSMORecord_chosen */
        CMCC_3GCDR_CG_M_SGSNSMTRecord sgsnSMTRecord;  /* to choose, set choice
                                   * to CMCC_3GCDR_CG_M_sgsnSMTRecord_chosen */
    } u;
} CMCC_3GCDR_CG_M_CallEventRecord;

typedef enum CMCC_3GCDR_CG_M_ChangeCondition {
    CMCC_3GCDR_CG_M_qoSChange = 0,
    CMCC_3GCDR_CG_M_tariffTime = 1,
    CMCC_3GCDR_CG_M_recordClosure = 2
} CMCC_3GCDR_CG_M_ChangeCondition;

typedef enum CMCC_3GCDR_CG_M_QoSReliability {
    CMCC_3GCDR_CG_M_unspecifiedReliability = 0,
    CMCC_3GCDR_CG_M_acknowledgedGTP = 1,
    CMCC_3GCDR_CG_M_unackGTPAcknowLLC = 2,
    CMCC_3GCDR_CG_M_unackGTPLLCAcknowRLC = 3,
    CMCC_3GCDR_CG_M_unackGTPLLCRLC = 4,
    CMCC_3GCDR_CG_M_unacknowUnprotectedData = 5
} CMCC_3GCDR_CG_M_QoSReliability;

typedef enum CMCC_3GCDR_CG_M_QoSDelay {
    CMCC_3GCDR_CG_M_delayClass1 = 1,
    CMCC_3GCDR_CG_M_delayClass2 = 2,
    CMCC_3GCDR_CG_M_delayClass3 = 3,
    CMCC_3GCDR_CG_M_delayClass4 = 4
} CMCC_3GCDR_CG_M_QoSDelay;

typedef enum CMCC_3GCDR_CG_M_QoSPrecedence {
    CMCC_3GCDR_CG_M_QoSPrecedence_unspecified = 0,
    CMCC_3GCDR_CG_M_highPriority = 1,
    CMCC_3GCDR_CG_M_normalPriority = 2,
    CMCC_3GCDR_CG_M_lowPriority = 3
} CMCC_3GCDR_CG_M_QoSPrecedence;

typedef enum CMCC_3GCDR_CG_M_QoSPeakThroughput {
    CMCC_3GCDR_CG_M_QoSPeakThroughput_unspecified = 0,
    CMCC_3GCDR_CG_M_upTo1000octetPs = 1,
    CMCC_3GCDR_CG_M_upTo2000octetPs = 2,
    CMCC_3GCDR_CG_M_upTo4000octetPs = 3,
    CMCC_3GCDR_CG_M_upTo8000octetPs = 4,
    CMCC_3GCDR_CG_M_upTo16000octetPs = 5,
    CMCC_3GCDR_CG_M_upTo32000octetPs = 6,
    CMCC_3GCDR_CG_M_upTo64000octetPs = 7,
    CMCC_3GCDR_CG_M_upTo128000octetPs = 8,
    CMCC_3GCDR_CG_M_upTo256000octetPs = 9
} CMCC_3GCDR_CG_M_QoSPeakThroughput;

typedef enum CMCC_3GCDR_CG_M_QoSMeanThroughput {
    CMCC_3GCDR_CG_M_subscribedMeanThroughput = 0,
    CMCC_3GCDR_CG_M_mean100octetPh = 1,
    CMCC_3GCDR_CG_M_mean200octetPh = 2,
    CMCC_3GCDR_CG_M_mean500octetPh = 3,
    CMCC_3GCDR_CG_M_mean1000octetPh = 4,
    CMCC_3GCDR_CG_M_mean2000octetPh = 5,
    CMCC_3GCDR_CG_M_mean5000octetPh = 6,
    CMCC_3GCDR_CG_M_mean10000octetPh = 7,
    CMCC_3GCDR_CG_M_mean20000octetPh = 8,
    CMCC_3GCDR_CG_M_mean50000octetPh = 9,
    CMCC_3GCDR_CG_M_mean100000octetPh = 10,
    CMCC_3GCDR_CG_M_mean200000octetPh = 11,
    CMCC_3GCDR_CG_M_mean500000octetPh = 12,
    CMCC_3GCDR_CG_M_mean1000000octetPh = 13,
    CMCC_3GCDR_CG_M_mean2000000octetPh = 14,
    CMCC_3GCDR_CG_M_mean5000000octetPh = 15,
    CMCC_3GCDR_CG_M_mean10000000octetPh = 16,
    CMCC_3GCDR_CG_M_mean20000000octetPh = 17,
    CMCC_3GCDR_CG_M_mean50000000octetPh = 18,
    CMCC_3GCDR_CG_M_reserved = 30,
    CMCC_3GCDR_CG_M_bestEffort = 31
} CMCC_3GCDR_CG_M_QoSMeanThroughput;

typedef struct CMCC_3GCDR_CG_M_GSMQoSInformation {
    CMCC_3GCDR_CG_M_QoSReliability reliability;
    CMCC_3GCDR_CG_M_QoSDelay delay;
    CMCC_3GCDR_CG_M_QoSPrecedence precedence;
    CMCC_3GCDR_CG_M_QoSPeakThroughput peakThroughput;
    CMCC_3GCDR_CG_M_QoSMeanThroughput meanThroughput;
} CMCC_3GCDR_CG_M_GSMQoSInformation;

typedef struct CMCC_3GCDR_CG_M_QoSInformation {
    unsigned short  choice;
#       define      CMCC_3GCDR_CG_M_gsmQosInformation_chosen 1
#       define      CMCC_3GCDR_CG_M_umtsQosInformation_chosen 2
    union _union {
        CMCC_3GCDR_CG_M_GSMQoSInformation gsmQosInformation;  /* to choose, set
                                   * choice to
                                  * CMCC_3GCDR_CG_M_gsmQosInformation_chosen */
        struct CMCC_3GCDR_CG_M__octet9 {
            unsigned short  length;
            unsigned char   value[12];
        } umtsQosInformation;  /* to choose, set choice to
                                * CMCC_3GCDR_CG_M_umtsQosInformation_chosen */
    } u;
} CMCC_3GCDR_CG_M_QoSInformation;

typedef int             CMCC_3GCDR_CG_M_DataVolumeGPRS;

typedef struct CMCC_3GCDR_CG_M_ChangeOfCharCondition {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_CG_M_qosRequested_present 0x80
#       define      CMCC_3GCDR_CG_M_qosNegotiated_present 0x40
    CMCC_3GCDR_CG_M_QoSInformation qosRequested;  /* optional; set in bit_mask
                                      * CMCC_3GCDR_CG_M_qosRequested_present if
                                      * present */
    CMCC_3GCDR_CG_M_QoSInformation qosNegotiated;  /* optional; set in bit_mask
                                     * CMCC_3GCDR_CG_M_qosNegotiated_present if
                                     * present */
    CMCC_3GCDR_CG_M_DataVolumeGPRS dataVolumeGPRSUplink;
    CMCC_3GCDR_CG_M_DataVolumeGPRS dataVolumeGPRSDownlink;
    CMCC_3GCDR_CG_M_ChangeCondition changeCondition;
    CMCC_3GCDR_CG_M_TimeStamp changeTime;
} CMCC_3GCDR_CG_M_ChangeOfCharCondition;

typedef struct CMCC_3GCDR_CG_M_ChangeLocation {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_CG_M_cellId_present 0x80
    CMCC_3GCDR_CG_M_LocationAreaCode locationAreaCode;
    CMCC_3GCDR_CG_M_RoutingAreaCode routingAreaCode;
    CMCC_3GCDR_CG_M_CellId cellId;  /* optional; set in bit_mask
                                     * CMCC_3GCDR_CG_M_cellId_present if
                                     * present */
    CMCC_3GCDR_CG_M_TimeStamp changeTime;
} CMCC_3GCDR_CG_M_ChangeLocation;

typedef struct CMCC_3GCDR_CG_M_RNCUnsentDownlinkVolumeList {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_CG_M_rNCUnsentDnVolumeList_present 0x80
    CMCC_3GCDR_CG_M_GSNAddress sgsnAddress;
    CMCC_3GCDR_CG_M__seqof7 rNCUnsentDnVolumeList;  /* optional; set in bit_mask
                             * CMCC_3GCDR_CG_M_rNCUnsentDnVolumeList_present if
                             * present */
} CMCC_3GCDR_CG_M_RNCUnsentDownlinkVolumeList;

extern CMCC_3GCDR_CG_M__ObjectID CMCC_3GCDR_CG_M_gsm1205InformationModel;

extern CMCC_3GCDR_CG_M__ObjectID CMCC_3GCDR_CG_M_gsm1205ASN1Module;

extern int CMCC_3GCDR_CG_M_maxAddressLength;

extern int CMCC_3GCDR_CG_M_maxISDN_AddressLength;


extern void *CMCC_CDRF_R99_Fin_M;    /* encoder-decoder control table */
#ifdef __cplusplus
}	/* extern "C" */
#endif /* __cplusplus */
#endif /* OSS_CDRF_R99_Fin_M */
