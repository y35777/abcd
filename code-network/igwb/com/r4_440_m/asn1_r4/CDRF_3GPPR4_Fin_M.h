/********************************************************/
/* Copyright (C) 2004 OSS Nokalva.  All rights reserved.*/
/********************************************************/

/* THIS FILE IS PROPRIETARY MATERIAL OF OSS NOKALVA
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF OSS NOKALVA.
 * THIS FILE MAY NOT BE DISTRIBUTED. */

/* Generated for: HuaWei Technologies, Inc., Shenzhen, China - license 8088 for Windows NT */
/* Abstract syntax: CDRF_R4_Fin_M */
/* Created: Fri Mar 05 11:02:40 2004 */
/* ASN.1 compiler version: 5.3 */
/* Target operating system: Windows NT/Windows 9x */
/* Target machine type: Intel x86 */
/* C compiler options required: -Zp4 (Microsoft) */
/* ASN.1 compiler options and file names specified:
 * -codefile CDRF_3GPPR4_Fin_M.c -headerfile CDRF_3GPPR4_Fin_M.h -errorfile
 * C:\WINDOWS\TEMP\2. -prefix R4_Fin_M_ -nodebug -dualheader -ansi -verbose
 * -ber -root D:\application\ossasn1\win32\5.3.0\asn1dflt\asn1dflt.ms.zp4
 * D:\yutemp\lidufang\0305\R4\r4_fin_M\CDRF_R4_Fin_M.asn
 */

#ifndef OSS_CDRF_R4_Fin_M
#define OSS_CDRF_R4_Fin_M

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "asn1hdr.h"
#include "asn1code.h"

#define          R4_Fin_M_BSRecord_PDU 1
#define          R4_Fin_M_Horizontal_Accuracy_PDU 2
#define          R4_Fin_M_ResponseTime_PDU 3
#define          R4_Fin_M_Vertical_Accuracy_PDU 4

typedef struct R4_Fin_M__ObjectID {
    unsigned short  length;
    unsigned char   *value;
} R4_Fin_M__ObjectID;

typedef struct R4_Fin_M__octet1 {
    unsigned short  length;
    unsigned char   value[15];
} R4_Fin_M__octet1;

typedef struct R4_Fin_M__octet3 {
    unsigned short  length;
    unsigned char   value[14];
} R4_Fin_M__octet3;

typedef struct R4_Fin_M__octet4 {
    unsigned short  length;
    unsigned char   value[10];
} R4_Fin_M__octet4;

typedef struct R4_Fin_M_BSXferSummaryRecord {
    R4_Fin_M__octet1 cgnID;
    struct R4_Fin_M__octet2 {
        unsigned short  length;
        unsigned char   value[34];
    } bsXferFile;
    R4_Fin_M__octet3 openTime;
    R4_Fin_M__octet3 closeTime;
    R4_Fin_M__octet4 lrsnStart;
    R4_Fin_M__octet4 lrsnEnd;
    struct R4_Fin_M__octet5 {
        unsigned short  length;
        unsigned char   value[3];
    } cdrType;
    R4_Fin_M__octet4 totalCdr;
    R4_Fin_M__octet4 missingCdr;
    R4_Fin_M__octet4 duplicateCdr;
    R4_Fin_M__octet1 cgID;
} R4_Fin_M_BSXferSummaryRecord;

typedef int             R4_Fin_M_CallEventRecordType;
#define                     R4_Fin_M_CallEventRecordType_sgsnPDPRecord 18
#define                     R4_Fin_M_CallEventRecordType_ggsnPDPRecord 19
#define                     R4_Fin_M_CallEventRecordType_sgsnMMRecord 20
#define                     R4_Fin_M_CallEventRecordType_sgsnSMORecord 21
#define                     R4_Fin_M_CallEventRecordType_sgsnSMTRecord 22
#define                     R4_Fin_M_sgsnMtLCSRecord 26
#define                     R4_Fin_M_sgsnMoLCSRecord 27
#define                     R4_Fin_M_sgsnNiLCSRecord 28

typedef ossBoolean      R4_Fin_M_NetworkInitiatedPDPContext;

typedef struct R4_Fin_M_IMSI {
    unsigned short  length;
    unsigned char   value[8];
} R4_Fin_M_IMSI;

typedef struct R4_Fin_M_IMEI {
    unsigned short  length;
    unsigned char   value[8];
} R4_Fin_M_IMEI;

typedef struct R4_Fin_M_IPBinaryAddress {
    unsigned short  choice;
#       define      R4_Fin_M_iPBinV4Address_chosen 1
#       define      R4_Fin_M_iPBinV6Address_chosen 2
    union _union {
        struct R4_Fin_M__octet6 {
            unsigned short  length;
            unsigned char   value[4];
        } iPBinV4Address;  /* to choose, set choice to
                            * R4_Fin_M_iPBinV4Address_chosen */
        struct R4_Fin_M__octet7 {
            unsigned short  length;
            unsigned char   value[16];
        } iPBinV6Address;  /* to choose, set choice to
                            * R4_Fin_M_iPBinV6Address_chosen */
    } u;
} R4_Fin_M_IPBinaryAddress;

typedef struct R4_Fin_M_IPTextRepresentedAddress {
    unsigned short  choice;
#       define      R4_Fin_M_iPTextV4Address_chosen 1
#       define      R4_Fin_M_iPTextV6Address_chosen 2
    union _union {
        char            iPTextV4Address[16];  /* to choose, set choice to
                                           * R4_Fin_M_iPTextV4Address_chosen */
        char            iPTextV6Address[46];  /* to choose, set choice to
                                           * R4_Fin_M_iPTextV6Address_chosen */
    } u;
} R4_Fin_M_IPTextRepresentedAddress;

typedef struct R4_Fin_M_IPAddress {
    unsigned short  choice;
#       define      R4_Fin_M_iPBinaryAddress_chosen 1
#       define      R4_Fin_M_iPTextRepresentedAddress_chosen 2
    union _union {
        R4_Fin_M_IPBinaryAddress iPBinaryAddress;  /* to choose, set choice to
                                           * R4_Fin_M_iPBinaryAddress_chosen */
        R4_Fin_M_IPTextRepresentedAddress iPTextRepresentedAddress;  /* to
                                   * choose, set choice to
                                  * R4_Fin_M_iPTextRepresentedAddress_chosen */
    } u;
} R4_Fin_M_IPAddress;

typedef R4_Fin_M_IPAddress R4_Fin_M_GSNAddress;

typedef struct R4_Fin_M_MSNetworkCapability {
    unsigned short  length;
    unsigned char   value[8];
} R4_Fin_M_MSNetworkCapability;

typedef struct R4_Fin_M_RoutingAreaCode {
    unsigned short  length;
    unsigned char   value[1];
} R4_Fin_M_RoutingAreaCode;

typedef struct R4_Fin_M_LocationAreaCode {
    unsigned short  length;
    unsigned char   value[2];
} R4_Fin_M_LocationAreaCode;

typedef struct R4_Fin_M_CellId {
    unsigned short  length;
    unsigned char   value[2];
} R4_Fin_M_CellId;

typedef unsigned int    R4_Fin_M_ChargingID;

typedef char            R4_Fin_M_AccessPointNameNI[64];

typedef struct R4_Fin_M_PDPType {
    unsigned short  length;
    unsigned char   value[2];
} R4_Fin_M_PDPType;

typedef struct R4_Fin_M_AddressString {
    unsigned short  length;
    unsigned char   value[20];
} R4_Fin_M_AddressString;

typedef R4_Fin_M_AddressString R4_Fin_M_ETSIAddress;

typedef struct R4_Fin_M_PDPAddress {
    unsigned short  choice;
#       define      R4_Fin_M_iPAddress_chosen 1
#       define      R4_Fin_M_eTSIAddress_chosen 2
    union _union {
        R4_Fin_M_IPAddress iPAddress;  /* to choose, set choice to
                                        * R4_Fin_M_iPAddress_chosen */
        R4_Fin_M_ETSIAddress eTSIAddress;  /* to choose, set choice to
                                            * R4_Fin_M_eTSIAddress_chosen */
    } u;
} R4_Fin_M_PDPAddress;

typedef struct R4_Fin_M_QoSInformation {
    unsigned short  length;
    unsigned char   value[12];
} R4_Fin_M_QoSInformation;

typedef int             R4_Fin_M_DataVolumeGPRS;

typedef enum R4_Fin_M_ChangeCondition {
    R4_Fin_M_qoSChange = 0,
    R4_Fin_M_tariffTime = 1,
    R4_Fin_M_recordClosure = 2
} R4_Fin_M_ChangeCondition;

typedef struct R4_Fin_M_TimeStamp {
    unsigned short  length;
    unsigned char   value[9];
} R4_Fin_M_TimeStamp;

typedef struct R4_Fin_M_ChangeOfCharCondition {
    unsigned char   bit_mask;
#       define      R4_Fin_M_qosRequested_present 0x80
#       define      R4_Fin_M_qosNegotiated_present 0x40
    R4_Fin_M_QoSInformation qosRequested;  /* optional; set in bit_mask
                                            * R4_Fin_M_qosRequested_present if
                                            * present */
    R4_Fin_M_QoSInformation qosNegotiated;  /* optional; set in bit_mask
                                             * R4_Fin_M_qosNegotiated_present if
                                             * present */
    R4_Fin_M_DataVolumeGPRS dataVolumeGPRSUplink;
    R4_Fin_M_DataVolumeGPRS dataVolumeGPRSDownlink;
    R4_Fin_M_ChangeCondition changeCondition;
    R4_Fin_M_TimeStamp changeTime;
} R4_Fin_M_ChangeOfCharCondition;

typedef int             R4_Fin_M_CallDuration;

typedef ossBoolean      R4_Fin_M_SGSNChange;

typedef int             R4_Fin_M_CauseForRecClosing;
#define                     R4_Fin_M_normalRelease 0
#define                     R4_Fin_M_abnormalRelease 4
#define                     R4_Fin_M_cAMELInitCallRelease 5
#define                     R4_Fin_M_volumeLimit 16
#define                     R4_Fin_M_timeLimit 17
#define                     R4_Fin_M_sGSNChange 18
#define                     R4_Fin_M_maxChangeCond 19
#define                     R4_Fin_M_managementIntervention 20
#define                     R4_Fin_M_intraSGSNIntersystemChange 21
#define                     R4_Fin_M_unauthorizedRequestingNetwork 52
#define                     R4_Fin_M_unauthorizedLCSClient 53
#define                     R4_Fin_M_positionMethodFailure 54
#define                     R4_Fin_M_unknownOrUnreachableLCSClient 58

typedef struct R4_Fin_M_ManagementExtension {
    unsigned char   bit_mask;
#       define      R4_Fin_M_significance_present 0x80
    R4_Fin_M__ObjectID identifier;
    ossBoolean      significance;  /* R4_Fin_M_significance_present not set in
                                    * bit_mask implies value is FALSE */
    struct R4_Fin_M__octet8 {
        unsigned int    length;
        unsigned char   *value;
    } information;
} R4_Fin_M_ManagementExtension;

typedef enum R4_Fin_M_PositionMethodFailure_Diagnostic {
    R4_Fin_M_congestion = 0,
    R4_Fin_M_insufficientResources = 1,
    R4_Fin_M_insufficientMeasurementData = 2,
    R4_Fin_M_inconsistentMeasurementData = 3,
    R4_Fin_M_locationProcedureNotCompleted = 4,
    R4_Fin_M_locationProcedureNotSupportedByTargetMS = 5,
    R4_Fin_M_qoSNotAttainable = 6,
    R4_Fin_M_positionMethodNotAvailableInNetwork = 7,
    R4_Fin_M_positionMethodNotAvailableInLocationArea = 8
} R4_Fin_M_PositionMethodFailure_Diagnostic;

typedef enum R4_Fin_M_UnauthorizedLCSClient_Diagnostic {
    R4_Fin_M_noAdditionalInformation = 0,
    R4_Fin_M_clientNotInMSPrivacyExceptionList = 1,
    R4_Fin_M_callToClientNotSetup = 2,
    R4_Fin_M_privacyOverrideNotApplicable = 3,
    R4_Fin_M_disallowedByLocalRegulatoryRequirements = 4
} R4_Fin_M_UnauthorizedLCSClient_Diagnostic;

typedef struct R4_Fin_M_Diagnostics {
    unsigned short  choice;
#       define      R4_Fin_M_gsm0408Cause_chosen 1
#       define      R4_Fin_M_gsm0902MapErrorValue_chosen 2
#       define      R4_Fin_M_ccittQ767Cause_chosen 3
#       define      R4_Fin_M_networkSpecificCause_chosen 4
#       define      R4_Fin_M_manufacturerSpecificCause_chosen 5
#       define      R4_Fin_M_positionMethodFailureCause_chosen 6
#       define      R4_Fin_M_unauthorizedLCSClientCause_chosen 7
    union _union {
        int             gsm0408Cause;  /* to choose, set choice to
                                        * R4_Fin_M_gsm0408Cause_chosen */
        int             gsm0902MapErrorValue;  /* to choose, set choice to
                                      * R4_Fin_M_gsm0902MapErrorValue_chosen */
        int             ccittQ767Cause;  /* to choose, set choice to
                                          * R4_Fin_M_ccittQ767Cause_chosen */
        R4_Fin_M_ManagementExtension networkSpecificCause;  /* to choose, set
                                   * choice to
                                   * R4_Fin_M_networkSpecificCause_chosen */
        R4_Fin_M_ManagementExtension manufacturerSpecificCause;  /* to choose,
                                   * set choice to
                                 * R4_Fin_M_manufacturerSpecificCause_chosen */
        R4_Fin_M_PositionMethodFailure_Diagnostic positionMethodFailureCause;                                           /* to choose, set choice to
                                * R4_Fin_M_positionMethodFailureCause_chosen */
        R4_Fin_M_UnauthorizedLCSClient_Diagnostic unauthorizedLCSClientCause;                                           /* to choose, set choice to
                                * R4_Fin_M_unauthorizedLCSClientCause_chosen */
    } u;
} R4_Fin_M_Diagnostics;

typedef struct R4_Fin_M__seqof1_ {
    struct R4_Fin_M__seqof1_ *next;
    int             value;
} *R4_Fin_M__seqof1;

typedef struct R4_Fin_M_SequenceList {
    unsigned char   bit_mask;
#       define      R4_Fin_M_recSequenceNumber_present 0x80
    R4_Fin_M_GSNAddress sgsnAddress;
    struct R4_Fin_M__seqof1_ *recSequenceNumber;  /* optional; set in bit_mask
                                        * R4_Fin_M_recSequenceNumber_present if
                                        * present */
} R4_Fin_M_SequenceList;

typedef char            R4_Fin_M_NodeID[21];

typedef struct R4_Fin_M_LocalSequenceNumberList {
    unsigned char   bit_mask;
#       define      R4_Fin_M_localSeqNumberList_present 0x80
    R4_Fin_M_GSNAddress gsnAddress;
    struct R4_Fin_M__seqof1_ *localSeqNumberList;  /* optional; set in bit_mask
                                       * R4_Fin_M_localSeqNumberList_present if
                                       * present */
} R4_Fin_M_LocalSequenceNumberList;

typedef enum R4_Fin_M_APNSelectionMode {
    R4_Fin_M_mSorNetworkProvidedSubscriptionVerified = 0,
    R4_Fin_M_mSProvidedSubscriptionNotVerified = 1,
    R4_Fin_M_networkProvidedSubscriptionNotVerified = 2
} R4_Fin_M_APNSelectionMode;

typedef char            R4_Fin_M_AccessPointNameOI[38];

typedef struct R4_Fin_M_ISDN_AddressString {
    unsigned short  length;
    unsigned char   value[9];
} R4_Fin_M_ISDN_AddressString;

typedef R4_Fin_M_ISDN_AddressString R4_Fin_M_MSISDN;

typedef struct R4_Fin_M_ChargingCharacteristics {
    unsigned short  length;
    unsigned char   value[2];
} R4_Fin_M_ChargingCharacteristics;

typedef enum R4_Fin_M_SystemType {
    R4_Fin_M_unknown = 0,
    R4_Fin_M_iuUTRAN = 1,
    R4_Fin_M_gERAN = 2
} R4_Fin_M_SystemType;

typedef R4_Fin_M_AddressString R4_Fin_M_SCFAddress;

typedef unsigned int    R4_Fin_M_ServiceKey;

typedef enum R4_Fin_M_DefaultGPRS_Handling {
    R4_Fin_M_DefaultGPRS_Handling_continueTransaction = 0,
    R4_Fin_M_DefaultGPRS_Handling_releaseTransaction = 1
} R4_Fin_M_DefaultGPRS_Handling;

typedef R4_Fin_M_AccessPointNameNI R4_Fin_M_CAMELAccessPointNameNI;

typedef R4_Fin_M_AccessPointNameOI R4_Fin_M_CAMELAccessPointNameOI;

typedef int             R4_Fin_M_NumberOfDPEncountered;

typedef unsigned char   R4_Fin_M_LevelOfCAMELService;
#define                     R4_Fin_M_basic 0x80
#define                     R4_Fin_M_callDurationSupervision 0x40
#define                     R4_Fin_M_onlineCharging 0x20

typedef struct R4_Fin_M_FreeFormatData {
    unsigned short  length;
    unsigned char   value[160];
} R4_Fin_M_FreeFormatData;

typedef ossBoolean      R4_Fin_M_FFDAppendIndicator;

typedef struct R4_Fin_M_CAMELInformationPDP {
    unsigned short  bit_mask;
#       define      R4_Fin_M_CAMELInformationPDP_sCFAddress_present 0x8000
#       define      R4_Fin_M_CAMELInformationPDP_serviceKey_present 0x4000
#       define      R4_Fin_M_CAMELInformationPDP_defaultTransactionHandling_present 0x2000
#       define      R4_Fin_M_cAMELAccessPointNameNI_present 0x1000
#       define      R4_Fin_M_cAMELAccessPointNameOI_present 0x0800
#       define      R4_Fin_M_CAMELInformationPDP_numberOfDPEncountered_present 0x0400
#       define      R4_Fin_M_CAMELInformationPDP_levelOfCAMELService_present 0x0200
#       define      R4_Fin_M_CAMELInformationPDP_freeFormatData_present 0x0100
#       define      R4_Fin_M_CAMELInformationPDP_fFDAppendIndicator_present 0x0080
    R4_Fin_M_SCFAddress sCFAddress;  /* optional; set in bit_mask
                           * R4_Fin_M_CAMELInformationPDP_sCFAddress_present if
                           * present */
    R4_Fin_M_ServiceKey serviceKey;  /* optional; set in bit_mask
                           * R4_Fin_M_CAMELInformationPDP_serviceKey_present if
                           * present */
    R4_Fin_M_DefaultGPRS_Handling defaultTransactionHandling;  /* optional; set
                                   * in bit_mask
           * R4_Fin_M_CAMELInformationPDP_defaultTransactionHandling_present if
           * present */
    R4_Fin_M_CAMELAccessPointNameNI cAMELAccessPointNameNI;  /* optional; set in
                                   * bit_mask
                                   * R4_Fin_M_cAMELAccessPointNameNI_present if
                                   * present */
    R4_Fin_M_CAMELAccessPointNameOI cAMELAccessPointNameOI;  /* optional; set in
                                   * bit_mask
                                   * R4_Fin_M_cAMELAccessPointNameOI_present if
                                   * present */
    R4_Fin_M_NumberOfDPEncountered numberOfDPEncountered;  /* optional; set in
                                   * bit_mask
                * R4_Fin_M_CAMELInformationPDP_numberOfDPEncountered_present if
                * present */
    R4_Fin_M_LevelOfCAMELService levelOfCAMELService;  /* optional; set in
                                   * bit_mask
                  * R4_Fin_M_CAMELInformationPDP_levelOfCAMELService_present if
                  * present */
    R4_Fin_M_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                       * R4_Fin_M_CAMELInformationPDP_freeFormatData_present if
                       * present */
    R4_Fin_M_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in
                                   * bit_mask
                   * R4_Fin_M_CAMELInformationPDP_fFDAppendIndicator_present if
                   * present */
} R4_Fin_M_CAMELInformationPDP;

typedef struct R4_Fin_M_RNCUnsentDownlinkVolumeList {
    unsigned char   bit_mask;
#       define      R4_Fin_M_rNCUnsentDnVolumeList_present 0x80
    R4_Fin_M_GSNAddress sgsnAddress;
    struct R4_Fin_M__seqof1_ *rNCUnsentDnVolumeList;  /* optional; set in
                                   * bit_mask
                                   * R4_Fin_M_rNCUnsentDnVolumeList_present if
                                   * present */
} R4_Fin_M_RNCUnsentDownlinkVolumeList;

typedef enum R4_Fin_M_ChChSelectionMode {
    R4_Fin_M_sGSNSupplied = 0,
    R4_Fin_M_subscriptionSpecific = 1,
    R4_Fin_M_aPNSpecific = 2,
    R4_Fin_M_homeDefault = 3,
    R4_Fin_M_roamingDefault = 4,
    R4_Fin_M_visitingDefault = 5
} R4_Fin_M_ChChSelectionMode;

typedef ossBoolean      R4_Fin_M_DynamicAddressFlag;

typedef enum R4_Fin_M_ConsolidationResult {
    R4_Fin_M_normal = 0,
    R4_Fin_M_notNormal = 1,
    R4_Fin_M_forInterSGSNConsolidation = 2,
    R4_Fin_M_reachLimit = 3,
    R4_Fin_M_onlyOneCDRGenerated = 4
} R4_Fin_M_ConsolidationResult;

typedef struct R4_Fin_M__seqof2_ {
    struct R4_Fin_M__seqof2_ *next;
    R4_Fin_M_GSNAddress value;
} *R4_Fin_M__seqof2;

typedef struct R4_Fin_M__seqof3_ {
    struct R4_Fin_M__seqof3_ *next;
    R4_Fin_M_ChangeOfCharCondition value;
} *R4_Fin_M__seqof3;

typedef struct R4_Fin_M_SGSNPDPRecord {
    unsigned int    bit_mask;
#       define      R4_Fin_M_SGSNPDPRecord_networkInitiation_present 0x80000000
#       define      R4_Fin_M_SGSNPDPRecord_servedIMEI_present 0x40000000
#       define      R4_Fin_M_SGSNPDPRecord_sgsnAddress_present 0x20000000
#       define      R4_Fin_M_SGSNPDPRecord_msNetworkCapability_present 0x10000000
#       define      R4_Fin_M_SGSNPDPRecord_routingArea_present 0x08000000
#       define      R4_Fin_M_SGSNPDPRecord_locationAreaCode_present 0x04000000
#       define      R4_Fin_M_SGSNPDPRecord_cellIdentifier_present 0x02000000
#       define      R4_Fin_M_SGSNPDPRecord_accessPointNameNI_present 0x01000000
#       define      R4_Fin_M_SGSNPDPRecord_pdpType_present 0x00800000
#       define      R4_Fin_M_SGSNPDPRecord_servedPDPAddress_present 0x00400000
#       define      R4_Fin_M_SGSNPDPRecord_listOfTrafficVolumes_present 0x00200000
#       define      R4_Fin_M_SGSNPDPRecord_sgsnChange_present 0x00100000
#       define      R4_Fin_M_SGSNPDPRecord_diagnostics_present 0x00080000
#       define      R4_Fin_M_SGSNPDPRecord_recSequenceNumList_present 0x00040000
#       define      R4_Fin_M_SGSNPDPRecord_nodeID_present 0x00020000
#       define      R4_Fin_M_SGSNPDPRecord_recordExtensions_present 0x00010000
#       define      R4_Fin_M_SGSNPDPRecord_localSequenceNumberList_present 0x00008000
#       define      R4_Fin_M_SGSNPDPRecord_apnSelectionMode_present 0x00004000
#       define      R4_Fin_M_accessPointNameOI_present 0x00002000
#       define      R4_Fin_M_SGSNPDPRecord_servedMSISDN_present 0x00001000
#       define      R4_Fin_M_SGSNPDPRecord_systemType_present 0x00000800
#       define      R4_Fin_M_cAMELInformationPDP_present 0x00000400
#       define      R4_Fin_M_rNCUnsentDownlinkVolumeList_present 0x00000200
#       define      R4_Fin_M_SGSNPDPRecord_chChSelectionMode_present 0x00000100
#       define      R4_Fin_M_SGSNPDPRecord_dynamicAddressFlag_present 0x00000080
    R4_Fin_M_CallEventRecordType recordType;
    R4_Fin_M_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                          * R4_Fin_M_SGSNPDPRecord_networkInitiation_present if
                          * present */
    R4_Fin_M_IMSI   servedIMSI;
    R4_Fin_M_IMEI   servedIMEI;  /* optional; set in bit_mask
                                  * R4_Fin_M_SGSNPDPRecord_servedIMEI_present if
                                  * present */
    struct R4_Fin_M__seqof2_ *sgsnAddress;  /* optional; set in bit_mask
                                * R4_Fin_M_SGSNPDPRecord_sgsnAddress_present if
                                * present */
    R4_Fin_M_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                        * R4_Fin_M_SGSNPDPRecord_msNetworkCapability_present if
                        * present */
    R4_Fin_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                * R4_Fin_M_SGSNPDPRecord_routingArea_present if
                                * present */
    R4_Fin_M_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                           * R4_Fin_M_SGSNPDPRecord_locationAreaCode_present if
                           * present */
    R4_Fin_M_CellId cellIdentifier;  /* optional; set in bit_mask
                             * R4_Fin_M_SGSNPDPRecord_cellIdentifier_present if
                             * present */
    R4_Fin_M_ChargingID chargingID;
    R4_Fin_M_GSNAddress ggsnAddressUsed;
    R4_Fin_M_AccessPointNameNI accessPointNameNI;  /* optional; set in bit_mask
                          * R4_Fin_M_SGSNPDPRecord_accessPointNameNI_present if
                          * present */
    R4_Fin_M_PDPType pdpType;  /* optional; set in bit_mask
                                * R4_Fin_M_SGSNPDPRecord_pdpType_present if
                                * present */
    R4_Fin_M_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                           * R4_Fin_M_SGSNPDPRecord_servedPDPAddress_present if
                           * present */
    struct R4_Fin_M__seqof3_ *listOfTrafficVolumes;  /* optional; set in
                                   * bit_mask
                       * R4_Fin_M_SGSNPDPRecord_listOfTrafficVolumes_present if
                       * present */
    R4_Fin_M_TimeStamp recordOpeningTime;
    R4_Fin_M_CallDuration duration;
    R4_Fin_M_SGSNChange sgsnChange;  /* optional; set in bit_mask
                                 * R4_Fin_M_SGSNPDPRecord_sgsnChange_present if
                                 * present */
    R4_Fin_M_CauseForRecClosing causeForRecClosing;
    R4_Fin_M_Diagnostics diagnostics;  /* optional; set in bit_mask
                                * R4_Fin_M_SGSNPDPRecord_diagnostics_present if
                                * present */
    struct R4_Fin_M__seqof4 {
        struct R4_Fin_M__seqof4 *next;
        R4_Fin_M_SequenceList value;
    } *recSequenceNumList;  /* optional; set in bit_mask
                             * R4_Fin_M_SGSNPDPRecord_recSequenceNumList_present
                             * if present */
    R4_Fin_M_NodeID nodeID;  /* optional; set in bit_mask
                              * R4_Fin_M_SGSNPDPRecord_nodeID_present if
                              * present */
    struct R4_Fin_M_ManagementExtensions_ *recordExtensions;  /* optional; set
                                   * in bit_mask
                           * R4_Fin_M_SGSNPDPRecord_recordExtensions_present if
                           * present */
    struct R4_Fin_M__seqof5 {
        struct R4_Fin_M__seqof5 *next;
        R4_Fin_M_LocalSequenceNumberList value;
    } *localSequenceNumberList;  /* optional; set in bit_mask
                    * R4_Fin_M_SGSNPDPRecord_localSequenceNumberList_present if
                    * present */
    R4_Fin_M_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                           * R4_Fin_M_SGSNPDPRecord_apnSelectionMode_present if
                           * present */
    R4_Fin_M_AccessPointNameOI accessPointNameOI;  /* optional; set in bit_mask
                                        * R4_Fin_M_accessPointNameOI_present if
                                        * present */
    R4_Fin_M_MSISDN servedMSISDN;  /* optional; set in bit_mask
                               * R4_Fin_M_SGSNPDPRecord_servedMSISDN_present if
                               * present */
    R4_Fin_M_ChargingCharacteristics chargingCharacteristics;
    R4_Fin_M_SystemType systemType;  /* optional; set in bit_mask
                                 * R4_Fin_M_SGSNPDPRecord_systemType_present if
                                 * present */
    R4_Fin_M_CAMELInformationPDP cAMELInformationPDP;  /* optional; set in
                                   * bit_mask
                                   * R4_Fin_M_cAMELInformationPDP_present if
                                   * present */
    struct R4_Fin_M__seqof6 {
        struct R4_Fin_M__seqof6 *next;
        R4_Fin_M_RNCUnsentDownlinkVolumeList value;
    } *rNCUnsentDownlinkVolumeList;  /* optional; set in bit_mask
                              * R4_Fin_M_rNCUnsentDownlinkVolumeList_present if
                              * present */
    R4_Fin_M_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                          * R4_Fin_M_SGSNPDPRecord_chChSelectionMode_present if
                          * present */
    R4_Fin_M_DynamicAddressFlag dynamicAddressFlag;  /* optional; set in
                                   * bit_mask
                         * R4_Fin_M_SGSNPDPRecord_dynamicAddressFlag_present if
                         * present */
    R4_Fin_M_ConsolidationResult consolidationResult;
} R4_Fin_M_SGSNPDPRecord;

typedef struct R4_Fin_M_PLMN_Id {
    unsigned short  length;
    unsigned char   value[3];
} R4_Fin_M_PLMN_Id;

typedef struct R4_Fin_M_GGSNPDPRecord {
    unsigned short  bit_mask;
#       define      R4_Fin_M_GGSNPDPRecord_networkInitiation_present 0x8000
#       define      R4_Fin_M_GGSNPDPRecord_accessPointNameNI_present 0x4000
#       define      R4_Fin_M_GGSNPDPRecord_pdpType_present 0x2000
#       define      R4_Fin_M_GGSNPDPRecord_servedPDPAddress_present 0x1000
#       define      R4_Fin_M_GGSNPDPRecord_dynamicAddressFlag_present 0x0800
#       define      R4_Fin_M_GGSNPDPRecord_listOfTrafficVolumes_present 0x0400
#       define      R4_Fin_M_GGSNPDPRecord_diagnostics_present 0x0200
#       define      R4_Fin_M_GGSNPDPRecord_recSequenceNumList_present 0x0100
#       define      R4_Fin_M_GGSNPDPRecord_nodeID_present 0x0080
#       define      R4_Fin_M_GGSNPDPRecord_recordExtensions_present 0x0040
#       define      R4_Fin_M_GGSNPDPRecord_localSequenceNumberList_present 0x0020
#       define      R4_Fin_M_GGSNPDPRecord_apnSelectionMode_present 0x0010
#       define      R4_Fin_M_GGSNPDPRecord_servedMSISDN_present 0x0008
#       define      R4_Fin_M_GGSNPDPRecord_chChSelectionMode_present 0x0004
#       define      R4_Fin_M_sgsnPLMNIdentifier_present 0x0002
    R4_Fin_M_CallEventRecordType recordType;
    R4_Fin_M_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                          * R4_Fin_M_GGSNPDPRecord_networkInitiation_present if
                          * present */
    R4_Fin_M_IMSI   servedIMSI;
    R4_Fin_M_GSNAddress ggsnAddress;
    R4_Fin_M_ChargingID chargingID;
    struct R4_Fin_M__seqof2_ *sgsnAddress;
    R4_Fin_M_AccessPointNameNI accessPointNameNI;  /* optional; set in bit_mask
                          * R4_Fin_M_GGSNPDPRecord_accessPointNameNI_present if
                          * present */
    R4_Fin_M_PDPType pdpType;  /* optional; set in bit_mask
                                * R4_Fin_M_GGSNPDPRecord_pdpType_present if
                                * present */
    R4_Fin_M_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                           * R4_Fin_M_GGSNPDPRecord_servedPDPAddress_present if
                           * present */
    R4_Fin_M_DynamicAddressFlag dynamicAddressFlag;  /* optional; set in
                                   * bit_mask
                         * R4_Fin_M_GGSNPDPRecord_dynamicAddressFlag_present if
                         * present */
    struct R4_Fin_M__seqof3_ *listOfTrafficVolumes;  /* optional; set in
                                   * bit_mask
                       * R4_Fin_M_GGSNPDPRecord_listOfTrafficVolumes_present if
                       * present */
    R4_Fin_M_TimeStamp recordOpeningTime;
    R4_Fin_M_CallDuration duration;
    R4_Fin_M_CauseForRecClosing causeForRecClosing;
    R4_Fin_M_Diagnostics diagnostics;  /* optional; set in bit_mask
                                * R4_Fin_M_GGSNPDPRecord_diagnostics_present if
                                * present */
    R4_Fin_M_SequenceList recSequenceNumList;  /* optional; set in bit_mask
                         * R4_Fin_M_GGSNPDPRecord_recSequenceNumList_present if
                         * present */
    R4_Fin_M_NodeID nodeID;  /* optional; set in bit_mask
                              * R4_Fin_M_GGSNPDPRecord_nodeID_present if
                              * present */
    struct R4_Fin_M_ManagementExtensions_ *recordExtensions;  /* optional; set
                                   * in bit_mask
                           * R4_Fin_M_GGSNPDPRecord_recordExtensions_present if
                           * present */
    R4_Fin_M_LocalSequenceNumberList localSequenceNumberList;  /* optional; set
                                   * in bit_mask
                    * R4_Fin_M_GGSNPDPRecord_localSequenceNumberList_present if
                    * present */
    R4_Fin_M_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                           * R4_Fin_M_GGSNPDPRecord_apnSelectionMode_present if
                           * present */
    R4_Fin_M_MSISDN servedMSISDN;  /* optional; set in bit_mask
                               * R4_Fin_M_GGSNPDPRecord_servedMSISDN_present if
                               * present */
    R4_Fin_M_ChargingCharacteristics chargingCharacteristics;
    R4_Fin_M_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                          * R4_Fin_M_GGSNPDPRecord_chChSelectionMode_present if
                          * present */
    R4_Fin_M_PLMN_Id sgsnPLMNIdentifier;  /* optional; set in bit_mask
                                           * R4_Fin_M_sgsnPLMNIdentifier_present
                                           * if present */
    R4_Fin_M_ConsolidationResult consolidationResult;
} R4_Fin_M_GGSNPDPRecord;

typedef struct R4_Fin_M_ChangeLocation {
    unsigned char   bit_mask;
#       define      R4_Fin_M_cellId_present 0x80
    R4_Fin_M_LocationAreaCode locationAreaCode;
    R4_Fin_M_RoutingAreaCode routingAreaCode;
    R4_Fin_M_CellId cellId;  /* optional; set in bit_mask
                              * R4_Fin_M_cellId_present if present */
    R4_Fin_M_TimeStamp changeTime;
} R4_Fin_M_ChangeLocation;

typedef unsigned int    R4_Fin_M_LocalSequenceNumber;

typedef struct R4_Fin_M_CAMELInformationMM {
    unsigned char   bit_mask;
#       define      R4_Fin_M_CAMELInformationMM_sCFAddress_present 0x80
#       define      R4_Fin_M_CAMELInformationMM_serviceKey_present 0x40
#       define      R4_Fin_M_CAMELInformationMM_defaultTransactionHandling_present 0x20
#       define      R4_Fin_M_CAMELInformationMM_numberOfDPEncountered_present 0x10
#       define      R4_Fin_M_CAMELInformationMM_levelOfCAMELService_present 0x08
#       define      R4_Fin_M_CAMELInformationMM_freeFormatData_present 0x04
#       define      R4_Fin_M_CAMELInformationMM_fFDAppendIndicator_present 0x02
    R4_Fin_M_SCFAddress sCFAddress;  /* optional; set in bit_mask
                            * R4_Fin_M_CAMELInformationMM_sCFAddress_present if
                            * present */
    R4_Fin_M_ServiceKey serviceKey;  /* optional; set in bit_mask
                            * R4_Fin_M_CAMELInformationMM_serviceKey_present if
                            * present */
    R4_Fin_M_DefaultGPRS_Handling defaultTransactionHandling;  /* optional; set
                                   * in bit_mask
            * R4_Fin_M_CAMELInformationMM_defaultTransactionHandling_present if
            * present */
    R4_Fin_M_NumberOfDPEncountered numberOfDPEncountered;  /* optional; set in
                                   * bit_mask
                 * R4_Fin_M_CAMELInformationMM_numberOfDPEncountered_present if
                 * present */
    R4_Fin_M_LevelOfCAMELService levelOfCAMELService;  /* optional; set in
                                   * bit_mask
                   * R4_Fin_M_CAMELInformationMM_levelOfCAMELService_present if
                   * present */
    R4_Fin_M_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                        * R4_Fin_M_CAMELInformationMM_freeFormatData_present if
                        * present */
    R4_Fin_M_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in
                                   * bit_mask
                    * R4_Fin_M_CAMELInformationMM_fFDAppendIndicator_present if
                    * present */
} R4_Fin_M_CAMELInformationMM;

typedef struct R4_Fin_M_SGSNMMRecord {
    unsigned int    bit_mask;
#       define      R4_Fin_M_SGSNMMRecord_servedIMEI_present 0x80000000
#       define      R4_Fin_M_SGSNMMRecord_sgsnAddress_present 0x40000000
#       define      R4_Fin_M_SGSNMMRecord_msNetworkCapability_present 0x20000000
#       define      R4_Fin_M_SGSNMMRecord_routingArea_present 0x10000000
#       define      R4_Fin_M_SGSNMMRecord_locationAreaCode_present 0x08000000
#       define      R4_Fin_M_SGSNMMRecord_cellIdentifier_present 0x04000000
#       define      R4_Fin_M_changeLocation_present 0x02000000
#       define      R4_Fin_M_duration_present 0x01000000
#       define      R4_Fin_M_SGSNMMRecord_sgsnChange_present 0x00800000
#       define      R4_Fin_M_SGSNMMRecord_diagnostics_present 0x00400000
#       define      R4_Fin_M_recordSequenceNumber_present 0x00200000
#       define      R4_Fin_M_SGSNMMRecord_nodeID_present 0x00100000
#       define      R4_Fin_M_SGSNMMRecord_recordExtensions_present 0x00080000
#       define      R4_Fin_M_SGSNMMRecord_localSequenceNumber_present 0x00040000
#       define      R4_Fin_M_SGSNMMRecord_servedMSISDN_present 0x00020000
#       define      R4_Fin_M_cAMELInformationMM_present 0x00010000
#       define      R4_Fin_M_SGSNMMRecord_systemType_present 0x00008000
#       define      R4_Fin_M_SGSNMMRecord_chChSelectionMode_present 0x00004000
    R4_Fin_M_CallEventRecordType recordType;
    R4_Fin_M_IMSI   servedIMSI;
    R4_Fin_M_IMEI   servedIMEI;  /* optional; set in bit_mask
                                  * R4_Fin_M_SGSNMMRecord_servedIMEI_present if
                                  * present */
    R4_Fin_M_GSNAddress sgsnAddress;  /* optional; set in bit_mask
                                 * R4_Fin_M_SGSNMMRecord_sgsnAddress_present if
                                 * present */
    R4_Fin_M_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                         * R4_Fin_M_SGSNMMRecord_msNetworkCapability_present if
                         * present */
    R4_Fin_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                 * R4_Fin_M_SGSNMMRecord_routingArea_present if
                                 * present */
    R4_Fin_M_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                            * R4_Fin_M_SGSNMMRecord_locationAreaCode_present if
                            * present */
    R4_Fin_M_CellId cellIdentifier;  /* optional; set in bit_mask
                              * R4_Fin_M_SGSNMMRecord_cellIdentifier_present if
                              * present */
    struct R4_Fin_M__seqof7 {
        struct R4_Fin_M__seqof7 *next;
        R4_Fin_M_ChangeLocation value;
    } *changeLocation;  /* optional; set in bit_mask
                         * R4_Fin_M_changeLocation_present if present */
    R4_Fin_M_TimeStamp recordOpeningTime;
    R4_Fin_M_CallDuration duration;  /* optional; set in bit_mask
                                      * R4_Fin_M_duration_present if present */
    R4_Fin_M_SGSNChange sgsnChange;  /* optional; set in bit_mask
                                      * R4_Fin_M_SGSNMMRecord_sgsnChange_present
                                      * if present */
    R4_Fin_M_CauseForRecClosing causeForRecClosing;
    R4_Fin_M_Diagnostics diagnostics;  /* optional; set in bit_mask
                                 * R4_Fin_M_SGSNMMRecord_diagnostics_present if
                                 * present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                                     * R4_Fin_M_recordSequenceNumber_present if
                                     * present */
    R4_Fin_M_NodeID nodeID;  /* optional; set in bit_mask
                              * R4_Fin_M_SGSNMMRecord_nodeID_present if
                              * present */
    struct R4_Fin_M_ManagementExtensions_ *recordExtensions;  /* optional; set
                                   * in bit_mask
                            * R4_Fin_M_SGSNMMRecord_recordExtensions_present if
                            * present */
    R4_Fin_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                         * R4_Fin_M_SGSNMMRecord_localSequenceNumber_present if
                         * present */
    R4_Fin_M_MSISDN servedMSISDN;  /* optional; set in bit_mask
                                    * R4_Fin_M_SGSNMMRecord_servedMSISDN_present
                                    * if present */
    R4_Fin_M_ChargingCharacteristics chargingCharacteristics;
    R4_Fin_M_CAMELInformationMM cAMELInformationMM;  /* optional; set in
                                   * bit_mask
                                   * R4_Fin_M_cAMELInformationMM_present if
                                   * present */
    R4_Fin_M_SystemType systemType;  /* optional; set in bit_mask
                                      * R4_Fin_M_SGSNMMRecord_systemType_present
                                      * if present */
    R4_Fin_M_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                           * R4_Fin_M_SGSNMMRecord_chChSelectionMode_present if
                           * present */
} R4_Fin_M_SGSNMMRecord;

typedef R4_Fin_M_AddressString R4_Fin_M_RecordingEntity;

typedef struct R4_Fin_M_MessageReference {
    unsigned int    length;
    unsigned char   *value;
} R4_Fin_M_MessageReference;

typedef R4_Fin_M_Diagnostics R4_Fin_M_SMSResult;

typedef struct R4_Fin_M_CalledNumber {
    unsigned short  length;
    unsigned char   value[12];
} R4_Fin_M_CalledNumber;

typedef enum R4_Fin_M_DefaultSMS_Handling {
    R4_Fin_M_DefaultSMS_Handling_continueTransaction = 0,
    R4_Fin_M_DefaultSMS_Handling_releaseTransaction = 1
} R4_Fin_M_DefaultSMS_Handling;

typedef struct R4_Fin_M_CallingNumber {
    unsigned short  length;
    unsigned char   value[10];
} R4_Fin_M_CallingNumber;

typedef struct R4_Fin_M_CallReferenceNumber {
    unsigned short  length;
    unsigned char   value[8];
} R4_Fin_M_CallReferenceNumber;

typedef struct R4_Fin_M_CAMELInformationSMS {
    unsigned char   bit_mask;
#       define      R4_Fin_M_CAMELInformationSMS_sCFAddress_present 0x80
#       define      R4_Fin_M_CAMELInformationSMS_serviceKey_present 0x40
#       define      R4_Fin_M_defaultSMSHandling_present 0x20
#       define      R4_Fin_M_cAMELCallingPartyNumber_present 0x10
#       define      R4_Fin_M_cAMELDestinationSubscriberNumber_present 0x08
#       define      R4_Fin_M_cAMELSMSCAddress_present 0x04
#       define      R4_Fin_M_CAMELInformationSMS_freeFormatData_present 0x02
#       define      R4_Fin_M_smsReferenceNumber_present 0x01
    R4_Fin_M_SCFAddress sCFAddress;  /* optional; set in bit_mask
                           * R4_Fin_M_CAMELInformationSMS_sCFAddress_present if
                           * present */
    R4_Fin_M_ServiceKey serviceKey;  /* optional; set in bit_mask
                           * R4_Fin_M_CAMELInformationSMS_serviceKey_present if
                           * present */
    R4_Fin_M_DefaultSMS_Handling defaultSMSHandling;  /* optional; set in
                                   * bit_mask
                                   * R4_Fin_M_defaultSMSHandling_present if
                                   * present */
    R4_Fin_M_CallingNumber cAMELCallingPartyNumber;  /* optional; set in
                                   * bit_mask
                                   * R4_Fin_M_cAMELCallingPartyNumber_present if
                                   * present */
    R4_Fin_M_CalledNumber cAMELDestinationSubscriberNumber;  /* optional; set in
                                   * bit_mask
                         * R4_Fin_M_cAMELDestinationSubscriberNumber_present if
                         * present */
    R4_Fin_M_AddressString cAMELSMSCAddress;  /* optional; set in bit_mask
                                         * R4_Fin_M_cAMELSMSCAddress_present if
                                         * present */
    R4_Fin_M_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                       * R4_Fin_M_CAMELInformationSMS_freeFormatData_present if
                       * present */
    R4_Fin_M_CallReferenceNumber smsReferenceNumber;  /* optional; set in
                                   * bit_mask
                                   * R4_Fin_M_smsReferenceNumber_present if
                                   * present */
} R4_Fin_M_CAMELInformationSMS;

typedef struct R4_Fin_M_SGSNSMORecord {
    unsigned short  bit_mask;
#       define      R4_Fin_M_SGSNSMORecord_servedIMEI_present 0x8000
#       define      R4_Fin_M_SGSNSMORecord_servedMSISDN_present 0x4000
#       define      R4_Fin_M_SGSNSMORecord_msNetworkCapability_present 0x2000
#       define      R4_Fin_M_SGSNSMORecord_serviceCentre_present 0x1000
#       define      R4_Fin_M_SGSNSMORecord_recordingEntity_present 0x0800
#       define      R4_Fin_M_SGSNSMORecord_locationArea_present 0x0400
#       define      R4_Fin_M_SGSNSMORecord_routingArea_present 0x0200
#       define      R4_Fin_M_SGSNSMORecord_cellIdentifier_present 0x0100
#       define      R4_Fin_M_SGSNSMORecord_smsResult_present 0x0080
#       define      R4_Fin_M_SGSNSMORecord_recordExtensions_present 0x0040
#       define      R4_Fin_M_SGSNSMORecord_nodeID_present 0x0020
#       define      R4_Fin_M_SGSNSMORecord_localSequenceNumber_present 0x0010
#       define      R4_Fin_M_SGSNSMORecord_systemType_present 0x0008
#       define      R4_Fin_M_destinationNumber_present 0x0004
#       define      R4_Fin_M_SGSNSMORecord_cAMELInformationSMS_present 0x0002
#       define      R4_Fin_M_SGSNSMORecord_chChSelectionMode_present 0x0001
    R4_Fin_M_CallEventRecordType recordType;
    R4_Fin_M_IMSI   servedIMSI;
    R4_Fin_M_IMEI   servedIMEI;  /* optional; set in bit_mask
                                  * R4_Fin_M_SGSNSMORecord_servedIMEI_present if
                                  * present */
    R4_Fin_M_MSISDN servedMSISDN;  /* optional; set in bit_mask
                               * R4_Fin_M_SGSNSMORecord_servedMSISDN_present if
                               * present */
    R4_Fin_M_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                        * R4_Fin_M_SGSNSMORecord_msNetworkCapability_present if
                        * present */
    R4_Fin_M_AddressString serviceCentre;  /* optional; set in bit_mask
                              * R4_Fin_M_SGSNSMORecord_serviceCentre_present if
                              * present */
    R4_Fin_M_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                            * R4_Fin_M_SGSNSMORecord_recordingEntity_present if
                            * present */
    R4_Fin_M_LocationAreaCode locationArea;  /* optional; set in bit_mask
                               * R4_Fin_M_SGSNSMORecord_locationArea_present if
                               * present */
    R4_Fin_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                * R4_Fin_M_SGSNSMORecord_routingArea_present if
                                * present */
    R4_Fin_M_CellId cellIdentifier;  /* optional; set in bit_mask
                             * R4_Fin_M_SGSNSMORecord_cellIdentifier_present if
                             * present */
    R4_Fin_M_MessageReference messageReference;
    R4_Fin_M_TimeStamp eventTimeStamp;
    R4_Fin_M_SMSResult smsResult;  /* optional; set in bit_mask
                                    * R4_Fin_M_SGSNSMORecord_smsResult_present
                                    * if present */
    struct R4_Fin_M_ManagementExtensions_ *recordExtensions;  /* optional; set
                                   * in bit_mask
                           * R4_Fin_M_SGSNSMORecord_recordExtensions_present if
                           * present */
    R4_Fin_M_NodeID nodeID;  /* optional; set in bit_mask
                              * R4_Fin_M_SGSNSMORecord_nodeID_present if
                              * present */
    R4_Fin_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                        * R4_Fin_M_SGSNSMORecord_localSequenceNumber_present if
                        * present */
    R4_Fin_M_ChargingCharacteristics chargingCharacteristics;
    R4_Fin_M_SystemType systemType;  /* optional; set in bit_mask
                                 * R4_Fin_M_SGSNSMORecord_systemType_present if
                                 * present */
    R4_Fin_M_CalledNumber destinationNumber;  /* optional; set in bit_mask
                                        * R4_Fin_M_destinationNumber_present if
                                        * present */
    R4_Fin_M_CAMELInformationSMS cAMELInformationSMS;  /* optional; set in
                                   * bit_mask
                        * R4_Fin_M_SGSNSMORecord_cAMELInformationSMS_present if
                        * present */
    R4_Fin_M_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                          * R4_Fin_M_SGSNSMORecord_chChSelectionMode_present if
                          * present */
} R4_Fin_M_SGSNSMORecord;

typedef struct R4_Fin_M_SGSNSMTRecord {
    unsigned short  bit_mask;
#       define      R4_Fin_M_SGSNSMTRecord_servedIMEI_present 0x8000
#       define      R4_Fin_M_SGSNSMTRecord_servedMSISDN_present 0x4000
#       define      R4_Fin_M_SGSNSMTRecord_msNetworkCapability_present 0x2000
#       define      R4_Fin_M_SGSNSMTRecord_serviceCentre_present 0x1000
#       define      R4_Fin_M_SGSNSMTRecord_recordingEntity_present 0x0800
#       define      R4_Fin_M_SGSNSMTRecord_locationArea_present 0x0400
#       define      R4_Fin_M_SGSNSMTRecord_routingArea_present 0x0200
#       define      R4_Fin_M_SGSNSMTRecord_cellIdentifier_present 0x0100
#       define      R4_Fin_M_SGSNSMTRecord_smsResult_present 0x0080
#       define      R4_Fin_M_SGSNSMTRecord_recordExtensions_present 0x0040
#       define      R4_Fin_M_SGSNSMTRecord_nodeID_present 0x0020
#       define      R4_Fin_M_SGSNSMTRecord_localSequenceNumber_present 0x0010
#       define      R4_Fin_M_SGSNSMTRecord_systemType_present 0x0008
#       define      R4_Fin_M_SGSNSMTRecord_chChSelectionMode_present 0x0004
#       define      R4_Fin_M_SGSNSMTRecord_cAMELInformationSMS_present 0x0002
    R4_Fin_M_CallEventRecordType recordType;
    R4_Fin_M_IMSI   servedIMSI;
    R4_Fin_M_IMEI   servedIMEI;  /* optional; set in bit_mask
                                  * R4_Fin_M_SGSNSMTRecord_servedIMEI_present if
                                  * present */
    R4_Fin_M_MSISDN servedMSISDN;  /* optional; set in bit_mask
                               * R4_Fin_M_SGSNSMTRecord_servedMSISDN_present if
                               * present */
    R4_Fin_M_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                        * R4_Fin_M_SGSNSMTRecord_msNetworkCapability_present if
                        * present */
    R4_Fin_M_AddressString serviceCentre;  /* optional; set in bit_mask
                              * R4_Fin_M_SGSNSMTRecord_serviceCentre_present if
                              * present */
    R4_Fin_M_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                            * R4_Fin_M_SGSNSMTRecord_recordingEntity_present if
                            * present */
    R4_Fin_M_LocationAreaCode locationArea;  /* optional; set in bit_mask
                               * R4_Fin_M_SGSNSMTRecord_locationArea_present if
                               * present */
    R4_Fin_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                * R4_Fin_M_SGSNSMTRecord_routingArea_present if
                                * present */
    R4_Fin_M_CellId cellIdentifier;  /* optional; set in bit_mask
                             * R4_Fin_M_SGSNSMTRecord_cellIdentifier_present if
                             * present */
    R4_Fin_M_TimeStamp eventTimeStamp;
    R4_Fin_M_SMSResult smsResult;  /* optional; set in bit_mask
                                    * R4_Fin_M_SGSNSMTRecord_smsResult_present
                                    * if present */
    struct R4_Fin_M_ManagementExtensions_ *recordExtensions;  /* optional; set
                                   * in bit_mask
                           * R4_Fin_M_SGSNSMTRecord_recordExtensions_present if
                           * present */
    R4_Fin_M_NodeID nodeID;  /* optional; set in bit_mask
                              * R4_Fin_M_SGSNSMTRecord_nodeID_present if
                              * present */
    R4_Fin_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                        * R4_Fin_M_SGSNSMTRecord_localSequenceNumber_present if
                        * present */
    R4_Fin_M_ChargingCharacteristics chargingCharacteristics;
    R4_Fin_M_SystemType systemType;  /* optional; set in bit_mask
                                 * R4_Fin_M_SGSNSMTRecord_systemType_present if
                                 * present */
    R4_Fin_M_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                          * R4_Fin_M_SGSNSMTRecord_chChSelectionMode_present if
                          * present */
    R4_Fin_M_CAMELInformationSMS cAMELInformationSMS;  /* optional; set in
                                   * bit_mask
                        * R4_Fin_M_SGSNSMTRecord_cAMELInformationSMS_present if
                        * present */
} R4_Fin_M_SGSNSMTRecord;

typedef enum R4_Fin_M_LCSClientType {
    R4_Fin_M_emergencyServices = 0,
    R4_Fin_M_valueAddedServices = 1,
    R4_Fin_M_plmnOperatorServices = 2,
    R4_Fin_M_lawfulInterceptServices = 3
} R4_Fin_M_LCSClientType;

typedef struct R4_Fin_M_LCSClientExternalID {
    unsigned char   bit_mask;
#       define      R4_Fin_M_externalAddress_present 0x80
    R4_Fin_M_AddressString externalAddress;  /* optional; set in bit_mask
                                              * R4_Fin_M_externalAddress_present
                                              * if present */
} R4_Fin_M_LCSClientExternalID;

typedef enum R4_Fin_M_LCSClientInternalID {
    R4_Fin_M_broadcastService = 0,
    R4_Fin_M_o_andM_HPLMN = 1,
    R4_Fin_M_o_andM_VPLMN = 2,
    R4_Fin_M_anonymousLocation = 3,
    R4_Fin_M_targetMSsubscribedService = 4
} R4_Fin_M_LCSClientInternalID;

typedef struct R4_Fin_M_LCSClientIdentity {
    unsigned char   bit_mask;
#       define      R4_Fin_M_lcsClientExternalID_present 0x80
#       define      R4_Fin_M_lcsClientDialedByMS_present 0x40
#       define      R4_Fin_M_lcsClientInternalID_present 0x20
    R4_Fin_M_LCSClientExternalID lcsClientExternalID;  /* optional; set in
                                   * bit_mask
                                   * R4_Fin_M_lcsClientExternalID_present if
                                   * present */
    R4_Fin_M_AddressString lcsClientDialedByMS;  /* optional; set in bit_mask
                                      * R4_Fin_M_lcsClientDialedByMS_present if
                                      * present */
    R4_Fin_M_LCSClientInternalID lcsClientInternalID;  /* optional; set in
                                   * bit_mask
                                   * R4_Fin_M_lcsClientInternalID_present if
                                   * present */
} R4_Fin_M_LCSClientIdentity;

typedef enum R4_Fin_M_LocationEstimateType {
    R4_Fin_M_currentLocation = 0,
    R4_Fin_M_currentOrLastKnownLocation = 1,
    R4_Fin_M_initialLocation = 2,
    R4_Fin_M_activateDeferredLocation = 3,
    R4_Fin_M_cancelDeferredLocation = 4
} R4_Fin_M_LocationEstimateType;

typedef unsigned short  R4_Fin_M_DeferredLocationEventType;
#define                     R4_Fin_M_msAvailable 0x8000

typedef struct R4_Fin_M_LocationType {
    unsigned char   bit_mask;
#       define      R4_Fin_M_deferredLocationEventType_present 0x80
    R4_Fin_M_LocationEstimateType locationEstimateType;
    R4_Fin_M_DeferredLocationEventType deferredLocationEventType;  /* optional;
                                   * set in bit_mask
                                   * R4_Fin_M_deferredLocationEventType_present
                                   * if present */
} R4_Fin_M_LocationType;

typedef struct R4_Fin_M_LCSQoSInfo {
    unsigned short  length;
    unsigned char   value[4];
} R4_Fin_M_LCSQoSInfo;

typedef struct R4_Fin_M_LCS_Priority {
    unsigned short  length;
    unsigned char   value[1];
} R4_Fin_M_LCS_Priority;

typedef enum R4_Fin_M_NotificationToMSUser {
    R4_Fin_M_notifyLocationAllowed = 0,
    R4_Fin_M_notifyAndVerify_LocationAllowedIfNoResponse = 1,
    R4_Fin_M_notifyAndVerify_LocationNotAllowedIfNoResponse = 2,
    R4_Fin_M_locationNotAllowed = 3
} R4_Fin_M_NotificationToMSUser;

typedef struct R4_Fin_M_LocationAreaAndCell {
    R4_Fin_M_LocationAreaCode locationAreaCode;
    R4_Fin_M_CellId cellId;
} R4_Fin_M_LocationAreaAndCell;

typedef struct R4_Fin_M_Ext_GeographicalInformation {
    unsigned short  length;
    unsigned char   value[20];
} R4_Fin_M_Ext_GeographicalInformation;

typedef struct R4_Fin_M_LCSCause {
    unsigned short  length;
    unsigned char   value[1];
} R4_Fin_M_LCSCause;

typedef struct R4_Fin_M_SGSNLCTRecord {
    unsigned int    bit_mask;
#       define      R4_Fin_M_SGSNLCTRecord_servedMSISDN_present 0x80000000
#       define      R4_Fin_M_SGSNLCTRecord_sgsnAddress_present 0x40000000
#       define      R4_Fin_M_SGSNLCTRecord_lcsQos_present 0x20000000
#       define      R4_Fin_M_SGSNLCTRecord_lcsPriority_present 0x10000000
#       define      R4_Fin_M_SGSNLCTRecord_measurementDuration_present 0x08000000
#       define      R4_Fin_M_notificationToMSUser_present 0x04000000
#       define      R4_Fin_M_privacyOverride_present 0x02000000
#       define      R4_Fin_M_SGSNLCTRecord_location_present 0x01000000
#       define      R4_Fin_M_SGSNLCTRecord_routingArea_present 0x00800000
#       define      R4_Fin_M_SGSNLCTRecord_locationEstimate_present 0x00400000
#       define      R4_Fin_M_SGSNLCTRecord_lcsCause_present 0x00200000
#       define      R4_Fin_M_SGSNLCTRecord_diagnostics_present 0x00100000
#       define      R4_Fin_M_SGSNLCTRecord_nodeID_present 0x00080000
#       define      R4_Fin_M_SGSNLCTRecord_localSequenceNumber_present 0x00040000
#       define      R4_Fin_M_SGSNLCTRecord_chChSelectionMode_present 0x00020000
#       define      R4_Fin_M_SGSNLCTRecord_systemType_present 0x00010000
#       define      R4_Fin_M_SGSNLCTRecord_recordExtensions_present 0x00008000
    R4_Fin_M_CallEventRecordType recordType;
    R4_Fin_M_RecordingEntity recordingEntity;
    R4_Fin_M_LCSClientType lcsClientType;
    R4_Fin_M_LCSClientIdentity lcsClientIdentity;
    R4_Fin_M_IMSI   servedIMSI;
    R4_Fin_M_MSISDN servedMSISDN;  /* optional; set in bit_mask
                               * R4_Fin_M_SGSNLCTRecord_servedMSISDN_present if
                               * present */
    R4_Fin_M_GSNAddress sgsnAddress;  /* optional; set in bit_mask
                                * R4_Fin_M_SGSNLCTRecord_sgsnAddress_present if
                                * present */
    R4_Fin_M_LocationType locationType;
    R4_Fin_M_LCSQoSInfo lcsQos;  /* optional; set in bit_mask
                                  * R4_Fin_M_SGSNLCTRecord_lcsQos_present if
                                  * present */
    R4_Fin_M_LCS_Priority lcsPriority;  /* optional; set in bit_mask
                                * R4_Fin_M_SGSNLCTRecord_lcsPriority_present if
                                * present */
    R4_Fin_M_ISDN_AddressString mlcNumber;
    R4_Fin_M_TimeStamp eventTimeStamp;
    R4_Fin_M_CallDuration measurementDuration;  /* optional; set in bit_mask
                        * R4_Fin_M_SGSNLCTRecord_measurementDuration_present if
                        * present */
    R4_Fin_M_NotificationToMSUser notificationToMSUser;  /* optional; set in
                                   * bit_mask
                                   * R4_Fin_M_notificationToMSUser_present if
                                   * present */
    Nulltype        privacyOverride;  /* optional; set in bit_mask
                                       * R4_Fin_M_privacyOverride_present if
                                       * present */
    R4_Fin_M_LocationAreaAndCell location;  /* optional; set in bit_mask
                                   * R4_Fin_M_SGSNLCTRecord_location_present if
                                   * present */
    R4_Fin_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                * R4_Fin_M_SGSNLCTRecord_routingArea_present if
                                * present */
    R4_Fin_M_Ext_GeographicalInformation locationEstimate;  /* optional; set in
                                   * bit_mask
                           * R4_Fin_M_SGSNLCTRecord_locationEstimate_present if
                           * present */
    R4_Fin_M_LCSCause lcsCause;  /* optional; set in bit_mask
                                  * R4_Fin_M_SGSNLCTRecord_lcsCause_present if
                                  * present */
    R4_Fin_M_Diagnostics diagnostics;  /* optional; set in bit_mask
                                * R4_Fin_M_SGSNLCTRecord_diagnostics_present if
                                * present */
    R4_Fin_M_NodeID nodeID;  /* optional; set in bit_mask
                              * R4_Fin_M_SGSNLCTRecord_nodeID_present if
                              * present */
    R4_Fin_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                        * R4_Fin_M_SGSNLCTRecord_localSequenceNumber_present if
                        * present */
    R4_Fin_M_ChargingCharacteristics chargingCharacteristics;
    R4_Fin_M_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                          * R4_Fin_M_SGSNLCTRecord_chChSelectionMode_present if
                          * present */
    R4_Fin_M_SystemType systemType;  /* optional; set in bit_mask
                                 * R4_Fin_M_SGSNLCTRecord_systemType_present if
                                 * present */
    struct R4_Fin_M_ManagementExtensions_ *recordExtensions;  /* optional; set
                                   * in bit_mask
                           * R4_Fin_M_SGSNLCTRecord_recordExtensions_present if
                           * present */
    R4_Fin_M_CauseForRecClosing causeForRecClosing;
} R4_Fin_M_SGSNLCTRecord;

typedef enum R4_Fin_M_LocationMethod {
    R4_Fin_M_msBasedEOTD = 0,
    R4_Fin_M_msAssistedEOTD = 1,
    R4_Fin_M_assistedGPS = 2,
    R4_Fin_M_msBasedOTDOA = 3,
    R4_Fin_M_msAssistedOTDOA = 4
} R4_Fin_M_LocationMethod;

typedef struct R4_Fin_M_SGSNLCORecord {
    unsigned int    bit_mask;
#       define      R4_Fin_M_SGSNLCORecord_lcsClientType_present 0x80000000
#       define      R4_Fin_M_SGSNLCORecord_lcsClientIdentity_present 0x40000000
#       define      R4_Fin_M_SGSNLCORecord_servedMSISDN_present 0x20000000
#       define      R4_Fin_M_SGSNLCORecord_sgsnAddress_present 0x10000000
#       define      R4_Fin_M_SGSNLCORecord_lcsQos_present 0x08000000
#       define      R4_Fin_M_SGSNLCORecord_lcsPriority_present 0x04000000
#       define      R4_Fin_M_SGSNLCORecord_mlcNumber_present 0x02000000
#       define      R4_Fin_M_SGSNLCORecord_measurementDuration_present 0x01000000
#       define      R4_Fin_M_SGSNLCORecord_location_present 0x00800000
#       define      R4_Fin_M_SGSNLCORecord_routingArea_present 0x00400000
#       define      R4_Fin_M_SGSNLCORecord_locationEstimate_present 0x00200000
#       define      R4_Fin_M_SGSNLCORecord_lcsCause_present 0x00100000
#       define      R4_Fin_M_SGSNLCORecord_diagnostics_present 0x00080000
#       define      R4_Fin_M_SGSNLCORecord_nodeID_present 0x00040000
#       define      R4_Fin_M_SGSNLCORecord_localSequenceNumber_present 0x00020000
#       define      R4_Fin_M_SGSNLCORecord_chChSelectionMode_present 0x00010000
#       define      R4_Fin_M_SGSNLCORecord_systemType_present 0x00008000
#       define      R4_Fin_M_SGSNLCORecord_recordExtensions_present 0x00004000
    R4_Fin_M_CallEventRecordType recordType;
    R4_Fin_M_RecordingEntity recordingEntity;
    R4_Fin_M_LCSClientType lcsClientType;  /* optional; set in bit_mask
                              * R4_Fin_M_SGSNLCORecord_lcsClientType_present if
                              * present */
    R4_Fin_M_LCSClientIdentity lcsClientIdentity;  /* optional; set in bit_mask
                          * R4_Fin_M_SGSNLCORecord_lcsClientIdentity_present if
                          * present */
    R4_Fin_M_IMSI   servedIMSI;
    R4_Fin_M_MSISDN servedMSISDN;  /* optional; set in bit_mask
                               * R4_Fin_M_SGSNLCORecord_servedMSISDN_present if
                               * present */
    R4_Fin_M_GSNAddress sgsnAddress;  /* optional; set in bit_mask
                                * R4_Fin_M_SGSNLCORecord_sgsnAddress_present if
                                * present */
    R4_Fin_M_LocationMethod locationMethod;
    R4_Fin_M_LCSQoSInfo lcsQos;  /* optional; set in bit_mask
                                  * R4_Fin_M_SGSNLCORecord_lcsQos_present if
                                  * present */
    R4_Fin_M_LCS_Priority lcsPriority;  /* optional; set in bit_mask
                                * R4_Fin_M_SGSNLCORecord_lcsPriority_present if
                                * present */
    R4_Fin_M_ISDN_AddressString mlcNumber;  /* optional; set in bit_mask
                                  * R4_Fin_M_SGSNLCORecord_mlcNumber_present if
                                  * present */
    R4_Fin_M_TimeStamp eventTimeStamp;
    R4_Fin_M_CallDuration measurementDuration;  /* optional; set in bit_mask
                        * R4_Fin_M_SGSNLCORecord_measurementDuration_present if
                        * present */
    R4_Fin_M_LocationAreaAndCell location;  /* optional; set in bit_mask
                                   * R4_Fin_M_SGSNLCORecord_location_present if
                                   * present */
    R4_Fin_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                * R4_Fin_M_SGSNLCORecord_routingArea_present if
                                * present */
    R4_Fin_M_Ext_GeographicalInformation locationEstimate;  /* optional; set in
                                   * bit_mask
                           * R4_Fin_M_SGSNLCORecord_locationEstimate_present if
                           * present */
    R4_Fin_M_LCSCause lcsCause;  /* optional; set in bit_mask
                                  * R4_Fin_M_SGSNLCORecord_lcsCause_present if
                                  * present */
    R4_Fin_M_Diagnostics diagnostics;  /* optional; set in bit_mask
                                * R4_Fin_M_SGSNLCORecord_diagnostics_present if
                                * present */
    R4_Fin_M_NodeID nodeID;  /* optional; set in bit_mask
                              * R4_Fin_M_SGSNLCORecord_nodeID_present if
                              * present */
    R4_Fin_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                        * R4_Fin_M_SGSNLCORecord_localSequenceNumber_present if
                        * present */
    R4_Fin_M_ChargingCharacteristics chargingCharacteristics;
    R4_Fin_M_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                          * R4_Fin_M_SGSNLCORecord_chChSelectionMode_present if
                          * present */
    R4_Fin_M_SystemType systemType;  /* optional; set in bit_mask
                                 * R4_Fin_M_SGSNLCORecord_systemType_present if
                                 * present */
    struct R4_Fin_M_ManagementExtensions_ *recordExtensions;  /* optional; set
                                   * in bit_mask
                           * R4_Fin_M_SGSNLCORecord_recordExtensions_present if
                           * present */
    R4_Fin_M_CauseForRecClosing causeForRecClosing;
} R4_Fin_M_SGSNLCORecord;

typedef struct R4_Fin_M_SGSNLCNRecord {
    unsigned int    bit_mask;
#       define      R4_Fin_M_SGSNLCNRecord_lcsClientType_present 0x80000000
#       define      R4_Fin_M_SGSNLCNRecord_lcsClientIdentity_present 0x40000000
#       define      R4_Fin_M_servedIMSI_present 0x20000000
#       define      R4_Fin_M_SGSNLCNRecord_servedMSISDN_present 0x10000000
#       define      R4_Fin_M_SGSNLCNRecord_sgsnAddress_present 0x08000000
#       define      R4_Fin_M_SGSNLCNRecord_servedIMEI_present 0x04000000
#       define      R4_Fin_M_SGSNLCNRecord_lcsQos_present 0x02000000
#       define      R4_Fin_M_SGSNLCNRecord_lcsPriority_present 0x01000000
#       define      R4_Fin_M_SGSNLCNRecord_mlcNumber_present 0x00800000
#       define      R4_Fin_M_SGSNLCNRecord_measurementDuration_present 0x00400000
#       define      R4_Fin_M_SGSNLCNRecord_location_present 0x00200000
#       define      R4_Fin_M_SGSNLCNRecord_routingArea_present 0x00100000
#       define      R4_Fin_M_SGSNLCNRecord_locationEstimate_present 0x00080000
#       define      R4_Fin_M_SGSNLCNRecord_lcsCause_present 0x00040000
#       define      R4_Fin_M_SGSNLCNRecord_diagnostics_present 0x00020000
#       define      R4_Fin_M_SGSNLCNRecord_nodeID_present 0x00010000
#       define      R4_Fin_M_SGSNLCNRecord_localSequenceNumber_present 0x00008000
#       define      R4_Fin_M_SGSNLCNRecord_chChSelectionMode_present 0x00004000
#       define      R4_Fin_M_SGSNLCNRecord_systemType_present 0x00002000
#       define      R4_Fin_M_SGSNLCNRecord_recordExtensions_present 0x00001000
    R4_Fin_M_CallEventRecordType recordType;
    R4_Fin_M_RecordingEntity recordingEntity;
    R4_Fin_M_LCSClientType lcsClientType;  /* optional; set in bit_mask
                              * R4_Fin_M_SGSNLCNRecord_lcsClientType_present if
                              * present */
    R4_Fin_M_LCSClientIdentity lcsClientIdentity;  /* optional; set in bit_mask
                          * R4_Fin_M_SGSNLCNRecord_lcsClientIdentity_present if
                          * present */
    R4_Fin_M_IMSI   servedIMSI;  /* optional; set in bit_mask
                                  * R4_Fin_M_servedIMSI_present if present */
    R4_Fin_M_MSISDN servedMSISDN;  /* optional; set in bit_mask
                               * R4_Fin_M_SGSNLCNRecord_servedMSISDN_present if
                               * present */
    R4_Fin_M_GSNAddress sgsnAddress;  /* optional; set in bit_mask
                                * R4_Fin_M_SGSNLCNRecord_sgsnAddress_present if
                                * present */
    R4_Fin_M_IMEI   servedIMEI;  /* optional; set in bit_mask
                                  * R4_Fin_M_SGSNLCNRecord_servedIMEI_present if
                                  * present */
    R4_Fin_M_LCSQoSInfo lcsQos;  /* optional; set in bit_mask
                                  * R4_Fin_M_SGSNLCNRecord_lcsQos_present if
                                  * present */
    R4_Fin_M_LCS_Priority lcsPriority;  /* optional; set in bit_mask
                                * R4_Fin_M_SGSNLCNRecord_lcsPriority_present if
                                * present */
    R4_Fin_M_ISDN_AddressString mlcNumber;  /* optional; set in bit_mask
                                  * R4_Fin_M_SGSNLCNRecord_mlcNumber_present if
                                  * present */
    R4_Fin_M_TimeStamp eventTimeStamp;
    R4_Fin_M_CallDuration measurementDuration;  /* optional; set in bit_mask
                        * R4_Fin_M_SGSNLCNRecord_measurementDuration_present if
                        * present */
    R4_Fin_M_LocationAreaAndCell location;  /* optional; set in bit_mask
                                   * R4_Fin_M_SGSNLCNRecord_location_present if
                                   * present */
    R4_Fin_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                * R4_Fin_M_SGSNLCNRecord_routingArea_present if
                                * present */
    R4_Fin_M_Ext_GeographicalInformation locationEstimate;  /* optional; set in
                                   * bit_mask
                           * R4_Fin_M_SGSNLCNRecord_locationEstimate_present if
                           * present */
    R4_Fin_M_LCSCause lcsCause;  /* optional; set in bit_mask
                                  * R4_Fin_M_SGSNLCNRecord_lcsCause_present if
                                  * present */
    R4_Fin_M_Diagnostics diagnostics;  /* optional; set in bit_mask
                                * R4_Fin_M_SGSNLCNRecord_diagnostics_present if
                                * present */
    R4_Fin_M_NodeID nodeID;  /* optional; set in bit_mask
                              * R4_Fin_M_SGSNLCNRecord_nodeID_present if
                              * present */
    R4_Fin_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                        * R4_Fin_M_SGSNLCNRecord_localSequenceNumber_present if
                        * present */
    R4_Fin_M_ChargingCharacteristics chargingCharacteristics;
    R4_Fin_M_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                          * R4_Fin_M_SGSNLCNRecord_chChSelectionMode_present if
                          * present */
    R4_Fin_M_SystemType systemType;  /* optional; set in bit_mask
                                 * R4_Fin_M_SGSNLCNRecord_systemType_present if
                                 * present */
    struct R4_Fin_M_ManagementExtensions_ *recordExtensions;  /* optional; set
                                   * in bit_mask
                           * R4_Fin_M_SGSNLCNRecord_recordExtensions_present if
                           * present */
    R4_Fin_M_CauseForRecClosing causeForRecClosing;
} R4_Fin_M_SGSNLCNRecord;

typedef struct R4_Fin_M_CallEventRecord {
    unsigned short  choice;
#       define      R4_Fin_M_sgsnPDPRecord_chosen 1
#       define      R4_Fin_M_ggsnPDPRecord_chosen 2
#       define      R4_Fin_M_sgsnMMRecord_chosen 3
#       define      R4_Fin_M_sgsnSMORecord_chosen 4
#       define      R4_Fin_M_sgsnSMTRecord_chosen 5
#       define      R4_Fin_M_sgsnLCTRecord_chosen 6
#       define      R4_Fin_M_sgsnLCORecord_chosen 7
#       define      R4_Fin_M_sgsnLCNRecord_chosen 8
    union _union {
        R4_Fin_M_SGSNPDPRecord sgsnPDPRecord;  /* to choose, set choice to
                                             * R4_Fin_M_sgsnPDPRecord_chosen */
        R4_Fin_M_GGSNPDPRecord ggsnPDPRecord;  /* to choose, set choice to
                                             * R4_Fin_M_ggsnPDPRecord_chosen */
        R4_Fin_M_SGSNMMRecord sgsnMMRecord;  /* to choose, set choice to
                                              * R4_Fin_M_sgsnMMRecord_chosen */
        R4_Fin_M_SGSNSMORecord sgsnSMORecord;  /* to choose, set choice to
                                             * R4_Fin_M_sgsnSMORecord_chosen */
        R4_Fin_M_SGSNSMTRecord sgsnSMTRecord;  /* to choose, set choice to
                                             * R4_Fin_M_sgsnSMTRecord_chosen */
        R4_Fin_M_SGSNLCTRecord sgsnLCTRecord;  /* to choose, set choice to
                                             * R4_Fin_M_sgsnLCTRecord_chosen */
        R4_Fin_M_SGSNLCORecord sgsnLCORecord;  /* to choose, set choice to
                                             * R4_Fin_M_sgsnLCORecord_chosen */
        R4_Fin_M_SGSNLCNRecord sgsnLCNRecord;  /* to choose, set choice to
                                             * R4_Fin_M_sgsnLCNRecord_chosen */
    } u;
} R4_Fin_M_CallEventRecord;

typedef struct R4_Fin_M_BSRecord {
    R4_Fin_M_BSXferSummaryRecord bsXferRecord;
    struct R4_Fin_M__seqof8 {
        struct R4_Fin_M__seqof8 *next;
        R4_Fin_M_CallEventRecord value;
    } *listOfCDR;
} R4_Fin_M_BSRecord;

typedef struct R4_Fin_M_ManagementExtensions_ {
    struct R4_Fin_M_ManagementExtensions_ *next;
    R4_Fin_M_ManagementExtension value;
} *R4_Fin_M_ManagementExtensions;

typedef struct R4_Fin_M_Horizontal_Accuracy {
    unsigned short  length;
    unsigned char   value[1];
} R4_Fin_M_Horizontal_Accuracy;

typedef enum R4_Fin_M_ResponseTimeCategory {
    R4_Fin_M_lowdelay = 0,
    R4_Fin_M_delaytolerant = 1
} R4_Fin_M_ResponseTimeCategory;

typedef struct R4_Fin_M_ResponseTime {
    R4_Fin_M_ResponseTimeCategory responseTimeCategory;
} R4_Fin_M_ResponseTime;

typedef struct R4_Fin_M_Vertical_Accuracy {
    unsigned short  length;
    unsigned char   value[1];
} R4_Fin_M_Vertical_Accuracy;

extern int R4_Fin_M_maxAddressLength;

extern int R4_Fin_M_maxISDN_AddressLength;

extern int R4_Fin_M_maxExt_GeographicalInformation;


extern void *CDRF_R4_Fin_M;    /* encoder-decoder control table */
#ifdef __cplusplus
}	/* extern "C" */
#endif /* __cplusplus */
#endif /* OSS_CDRF_R4_Fin_M */
