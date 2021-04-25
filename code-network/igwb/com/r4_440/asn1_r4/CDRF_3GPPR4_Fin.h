/********************************************************/
/* Copyright (C) 2004 OSS Nokalva.  All rights reserved.*/
/********************************************************/

/* THIS FILE IS PROPRIETARY MATERIAL OF OSS NOKALVA
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF OSS NOKALVA.
 * THIS FILE MAY NOT BE DISTRIBUTED. */

/* Generated for: HuaWei Technologies, Inc., Shenzhen, China - license 8088 for Windows NT */
/* Abstract syntax: CDRF_R4_Fin */
/* Created: Fri Mar 05 11:02:04 2004 */
/* ASN.1 compiler version: 5.3 */
/* Target operating system: Windows NT/Windows 9x */
/* Target machine type: Intel x86 */
/* C compiler options required: -Zp4 (Microsoft) */
/* ASN.1 compiler options and file names specified:
 * -codefile CDRF_3GPPR4_Fin.c -headerfile CDRF_3GPPR4_Fin.h -errorfile
 * C:\WINDOWS\TEMP\2. -prefix R4_Fin_ -nodebug -dualheader -ansi -verbose -ber
 * -root D:\application\ossasn1\win32\5.3.0\asn1dflt\asn1dflt.ms.zp4
 * D:\yutemp\lidufang\0305\R4\r4_fin\CDRF_R4_Fin.asn
 */

#ifndef OSS_CDRF_R4_Fin
#define OSS_CDRF_R4_Fin

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "asn1hdr.h"
#include "asn1code.h"

#define          R4_Fin_CallEventRecord_PDU 1
#define          R4_Fin_Horizontal_Accuracy_PDU 2
#define          R4_Fin_ResponseTime_PDU 3
#define          R4_Fin_Vertical_Accuracy_PDU 4

typedef struct R4_Fin__ObjectID {
    unsigned short  length;
    unsigned char   *value;
} R4_Fin__ObjectID;

typedef int             R4_Fin_CallEventRecordType;
#define                     R4_Fin_CallEventRecordType_sgsnPDPRecord 18
#define                     R4_Fin_CallEventRecordType_ggsnPDPRecord 19
#define                     R4_Fin_CallEventRecordType_sgsnMMRecord 20
#define                     R4_Fin_CallEventRecordType_sgsnSMORecord 21
#define                     R4_Fin_CallEventRecordType_sgsnSMTRecord 22
#define                     R4_Fin_sgsnMtLCSRecord 26
#define                     R4_Fin_sgsnMoLCSRecord 27
#define                     R4_Fin_sgsnNiLCSRecord 28

typedef ossBoolean      R4_Fin_NetworkInitiatedPDPContext;

typedef struct R4_Fin_IMSI {
    unsigned short  length;
    unsigned char   value[8];
} R4_Fin_IMSI;

typedef struct R4_Fin_IMEI {
    unsigned short  length;
    unsigned char   value[8];
} R4_Fin_IMEI;

typedef struct R4_Fin_IPBinaryAddress {
    unsigned short  choice;
#       define      R4_Fin_iPBinV4Address_chosen 1
#       define      R4_Fin_iPBinV6Address_chosen 2
    union _union {
        struct R4_Fin__octet1 {
            unsigned short  length;
            unsigned char   value[4];
        } iPBinV4Address;  /* to choose, set choice to
                            * R4_Fin_iPBinV4Address_chosen */
        struct R4_Fin__octet2 {
            unsigned short  length;
            unsigned char   value[16];
        } iPBinV6Address;  /* to choose, set choice to
                            * R4_Fin_iPBinV6Address_chosen */
    } u;
} R4_Fin_IPBinaryAddress;

typedef struct R4_Fin_IPTextRepresentedAddress {
    unsigned short  choice;
#       define      R4_Fin_iPTextV4Address_chosen 1
#       define      R4_Fin_iPTextV6Address_chosen 2
    union _union {
        char            iPTextV4Address[16];  /* to choose, set choice to
                                             * R4_Fin_iPTextV4Address_chosen */
        char            iPTextV6Address[46];  /* to choose, set choice to
                                             * R4_Fin_iPTextV6Address_chosen */
    } u;
} R4_Fin_IPTextRepresentedAddress;

typedef struct R4_Fin_IPAddress {
    unsigned short  choice;
#       define      R4_Fin_iPBinaryAddress_chosen 1
#       define      R4_Fin_iPTextRepresentedAddress_chosen 2
    union _union {
        R4_Fin_IPBinaryAddress iPBinaryAddress;  /* to choose, set choice to
                                             * R4_Fin_iPBinaryAddress_chosen */
        R4_Fin_IPTextRepresentedAddress iPTextRepresentedAddress;  /* to choose,
                                   * set choice to
                                   * R4_Fin_iPTextRepresentedAddress_chosen */
    } u;
} R4_Fin_IPAddress;

typedef R4_Fin_IPAddress R4_Fin_GSNAddress;

typedef struct R4_Fin_MSNetworkCapability {
    unsigned short  length;
    unsigned char   value[8];
} R4_Fin_MSNetworkCapability;

typedef struct R4_Fin_RoutingAreaCode {
    unsigned short  length;
    unsigned char   value[1];
} R4_Fin_RoutingAreaCode;

typedef struct R4_Fin_LocationAreaCode {
    unsigned short  length;
    unsigned char   value[2];
} R4_Fin_LocationAreaCode;

typedef struct R4_Fin_CellId {
    unsigned short  length;
    unsigned char   value[2];
} R4_Fin_CellId;

typedef unsigned int    R4_Fin_ChargingID;

typedef char            R4_Fin_AccessPointNameNI[64];

typedef struct R4_Fin_PDPType {
    unsigned short  length;
    unsigned char   value[2];
} R4_Fin_PDPType;

typedef struct R4_Fin_AddressString {
    unsigned short  length;
    unsigned char   value[20];
} R4_Fin_AddressString;

typedef R4_Fin_AddressString R4_Fin_ETSIAddress;

typedef struct R4_Fin_PDPAddress {
    unsigned short  choice;
#       define      R4_Fin_iPAddress_chosen 1
#       define      R4_Fin_eTSIAddress_chosen 2
    union _union {
        R4_Fin_IPAddress iPAddress;  /* to choose, set choice to
                                      * R4_Fin_iPAddress_chosen */
        R4_Fin_ETSIAddress eTSIAddress;  /* to choose, set choice to
                                          * R4_Fin_eTSIAddress_chosen */
    } u;
} R4_Fin_PDPAddress;

typedef struct R4_Fin_QoSInformation {
    unsigned short  length;
    unsigned char   value[12];
} R4_Fin_QoSInformation;

typedef int             R4_Fin_DataVolumeGPRS;

typedef enum R4_Fin_ChangeCondition {
    R4_Fin_qoSChange = 0,
    R4_Fin_tariffTime = 1,
    R4_Fin_recordClosure = 2
} R4_Fin_ChangeCondition;

typedef struct R4_Fin_TimeStamp {
    unsigned short  length;
    unsigned char   value[9];
} R4_Fin_TimeStamp;

typedef struct R4_Fin_ChangeOfCharCondition {
    unsigned char   bit_mask;
#       define      R4_Fin_qosRequested_present 0x80
#       define      R4_Fin_qosNegotiated_present 0x40
    R4_Fin_QoSInformation qosRequested;  /* optional; set in bit_mask
                                          * R4_Fin_qosRequested_present if
                                          * present */
    R4_Fin_QoSInformation qosNegotiated;  /* optional; set in bit_mask
                                           * R4_Fin_qosNegotiated_present if
                                           * present */
    R4_Fin_DataVolumeGPRS dataVolumeGPRSUplink;
    R4_Fin_DataVolumeGPRS dataVolumeGPRSDownlink;
    R4_Fin_ChangeCondition changeCondition;
    R4_Fin_TimeStamp changeTime;
} R4_Fin_ChangeOfCharCondition;

typedef int             R4_Fin_CallDuration;

typedef ossBoolean      R4_Fin_SGSNChange;

typedef int             R4_Fin_CauseForRecClosing;
#define                     R4_Fin_normalRelease 0
#define                     R4_Fin_abnormalRelease 4
#define                     R4_Fin_cAMELInitCallRelease 5
#define                     R4_Fin_volumeLimit 16
#define                     R4_Fin_timeLimit 17
#define                     R4_Fin_sGSNChange 18
#define                     R4_Fin_maxChangeCond 19
#define                     R4_Fin_managementIntervention 20
#define                     R4_Fin_intraSGSNIntersystemChange 21
#define                     R4_Fin_unauthorizedRequestingNetwork 52
#define                     R4_Fin_unauthorizedLCSClient 53
#define                     R4_Fin_positionMethodFailure 54
#define                     R4_Fin_unknownOrUnreachableLCSClient 58

typedef struct R4_Fin_ManagementExtension {
    unsigned char   bit_mask;
#       define      R4_Fin_significance_present 0x80
    R4_Fin__ObjectID identifier;
    ossBoolean      significance;  /* R4_Fin_significance_present not set in
                                    * bit_mask implies value is FALSE */
    struct R4_Fin__octet3 {
        unsigned int    length;
        unsigned char   *value;
    } information;
} R4_Fin_ManagementExtension;

typedef enum R4_Fin_PositionMethodFailure_Diagnostic {
    R4_Fin_congestion = 0,
    R4_Fin_insufficientResources = 1,
    R4_Fin_insufficientMeasurementData = 2,
    R4_Fin_inconsistentMeasurementData = 3,
    R4_Fin_locationProcedureNotCompleted = 4,
    R4_Fin_locationProcedureNotSupportedByTargetMS = 5,
    R4_Fin_qoSNotAttainable = 6,
    R4_Fin_positionMethodNotAvailableInNetwork = 7,
    R4_Fin_positionMethodNotAvailableInLocationArea = 8
} R4_Fin_PositionMethodFailure_Diagnostic;

typedef enum R4_Fin_UnauthorizedLCSClient_Diagnostic {
    R4_Fin_noAdditionalInformation = 0,
    R4_Fin_clientNotInMSPrivacyExceptionList = 1,
    R4_Fin_callToClientNotSetup = 2,
    R4_Fin_privacyOverrideNotApplicable = 3,
    R4_Fin_disallowedByLocalRegulatoryRequirements = 4
} R4_Fin_UnauthorizedLCSClient_Diagnostic;

typedef struct R4_Fin_Diagnostics {
    unsigned short  choice;
#       define      R4_Fin_gsm0408Cause_chosen 1
#       define      R4_Fin_gsm0902MapErrorValue_chosen 2
#       define      R4_Fin_ccittQ767Cause_chosen 3
#       define      R4_Fin_networkSpecificCause_chosen 4
#       define      R4_Fin_manufacturerSpecificCause_chosen 5
#       define      R4_Fin_positionMethodFailureCause_chosen 6
#       define      R4_Fin_unauthorizedLCSClientCause_chosen 7
    union _union {
        int             gsm0408Cause;  /* to choose, set choice to
                                        * R4_Fin_gsm0408Cause_chosen */
        int             gsm0902MapErrorValue;  /* to choose, set choice to
                                        * R4_Fin_gsm0902MapErrorValue_chosen */
        int             ccittQ767Cause;  /* to choose, set choice to
                                          * R4_Fin_ccittQ767Cause_chosen */
        R4_Fin_ManagementExtension networkSpecificCause;  /* to choose, set
                                   * choice to
                                   * R4_Fin_networkSpecificCause_chosen */
        R4_Fin_ManagementExtension manufacturerSpecificCause;  /* to choose, set
                                   * choice to
                                   * R4_Fin_manufacturerSpecificCause_chosen */
        R4_Fin_PositionMethodFailure_Diagnostic positionMethodFailureCause;  
                                        /* to choose, set choice to
                                  * R4_Fin_positionMethodFailureCause_chosen */
        R4_Fin_UnauthorizedLCSClient_Diagnostic unauthorizedLCSClientCause;  
                                        /* to choose, set choice to
                                  * R4_Fin_unauthorizedLCSClientCause_chosen */
    } u;
} R4_Fin_Diagnostics;

typedef struct R4_Fin__seqof1_ {
    struct R4_Fin__seqof1_ *next;
    int             value;
} *R4_Fin__seqof1;

typedef struct R4_Fin_SequenceList {
    unsigned char   bit_mask;
#       define      R4_Fin_recSequenceNumberList_present 0x80
    R4_Fin_GSNAddress sgsnAddress;
    struct R4_Fin__seqof1_ *recSequenceNumberList;  /* optional; set in bit_mask
                                      * R4_Fin_recSequenceNumberList_present if
                                      * present */
} R4_Fin_SequenceList;

typedef char            R4_Fin_NodeID[21];

typedef struct R4_Fin_LocalSequenceNumberList {
    unsigned char   bit_mask;
#       define      R4_Fin_localSeqNumberList_present 0x80
    R4_Fin_GSNAddress gsnAddress;
    struct R4_Fin__seqof1_ *localSeqNumberList;  /* optional; set in bit_mask
                                         * R4_Fin_localSeqNumberList_present if
                                         * present */
} R4_Fin_LocalSequenceNumberList;

typedef enum R4_Fin_APNSelectionMode {
    R4_Fin_mSorNetworkProvidedSubscriptionVerified = 0,
    R4_Fin_mSProvidedSubscriptionNotVerified = 1,
    R4_Fin_networkProvidedSubscriptionNotVerified = 2
} R4_Fin_APNSelectionMode;

typedef char            R4_Fin_AccessPointNameOI[38];

typedef struct R4_Fin_ISDN_AddressString {
    unsigned short  length;
    unsigned char   value[9];
} R4_Fin_ISDN_AddressString;

typedef R4_Fin_ISDN_AddressString R4_Fin_MSISDN;

typedef struct R4_Fin_ChargingCharacteristics {
    unsigned short  length;
    unsigned char   value[2];
} R4_Fin_ChargingCharacteristics;

typedef enum R4_Fin_SystemType {
    R4_Fin_unknown = 0,
    R4_Fin_iuUTRAN = 1,
    R4_Fin_gERAN = 2
} R4_Fin_SystemType;

typedef R4_Fin_AddressString R4_Fin_SCFAddress;

typedef unsigned int    R4_Fin_ServiceKey;

typedef enum R4_Fin_DefaultGPRS_Handling {
    R4_Fin_DefaultGPRS_Handling_continueTransaction = 0,
    R4_Fin_DefaultGPRS_Handling_releaseTransaction = 1
} R4_Fin_DefaultGPRS_Handling;

typedef R4_Fin_AccessPointNameNI R4_Fin_CAMELAccessPointNameNI;

typedef R4_Fin_AccessPointNameOI R4_Fin_CAMELAccessPointNameOI;

typedef int             R4_Fin_NumberOfDPEncountered;

typedef unsigned char   R4_Fin_LevelOfCAMELService;
#define                     R4_Fin_basic 0x80
#define                     R4_Fin_callDurationSupervision 0x40
#define                     R4_Fin_onlineCharging 0x20

typedef struct R4_Fin_FreeFormatData {
    unsigned short  length;
    unsigned char   value[160];
} R4_Fin_FreeFormatData;

typedef ossBoolean      R4_Fin_FFDAppendIndicator;

typedef struct R4_Fin_CAMELInformationPDP {
    unsigned short  bit_mask;
#       define      R4_Fin_CAMELInformationPDP_sCFAddress_present 0x8000
#       define      R4_Fin_CAMELInformationPDP_serviceKey_present 0x4000
#       define      R4_Fin_CAMELInformationPDP_defaultTransactionHandling_present 0x2000
#       define      R4_Fin_cAMELAccessPointNameNI_present 0x1000
#       define      R4_Fin_cAMELAccessPointNameOI_present 0x0800
#       define      R4_Fin_CAMELInformationPDP_numberOfDPEncountered_present 0x0400
#       define      R4_Fin_CAMELInformationPDP_levelOfCAMELService_present 0x0200
#       define      R4_Fin_CAMELInformationPDP_freeFormatData_present 0x0100
#       define      R4_Fin_CAMELInformationPDP_fFDAppendIndicator_present 0x0080
    R4_Fin_SCFAddress sCFAddress;  /* optional; set in bit_mask
                             * R4_Fin_CAMELInformationPDP_sCFAddress_present if
                             * present */
    R4_Fin_ServiceKey serviceKey;  /* optional; set in bit_mask
                             * R4_Fin_CAMELInformationPDP_serviceKey_present if
                             * present */
    R4_Fin_DefaultGPRS_Handling defaultTransactionHandling;  /* optional; set in
                                   * bit_mask
             * R4_Fin_CAMELInformationPDP_defaultTransactionHandling_present if
             * present */
    R4_Fin_CAMELAccessPointNameNI cAMELAccessPointNameNI;  /* optional; set in
                                   * bit_mask
                                   * R4_Fin_cAMELAccessPointNameNI_present if
                                   * present */
    R4_Fin_CAMELAccessPointNameOI cAMELAccessPointNameOI;  /* optional; set in
                                   * bit_mask
                                   * R4_Fin_cAMELAccessPointNameOI_present if
                                   * present */
    R4_Fin_NumberOfDPEncountered numberOfDPEncountered;  /* optional; set in
                                   * bit_mask
                  * R4_Fin_CAMELInformationPDP_numberOfDPEncountered_present if
                  * present */
    R4_Fin_LevelOfCAMELService levelOfCAMELService;  /* optional; set in
                                   * bit_mask
                    * R4_Fin_CAMELInformationPDP_levelOfCAMELService_present if
                    * present */
    R4_Fin_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                         * R4_Fin_CAMELInformationPDP_freeFormatData_present if
                         * present */
    R4_Fin_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in bit_mask
                     * R4_Fin_CAMELInformationPDP_fFDAppendIndicator_present if
                     * present */
} R4_Fin_CAMELInformationPDP;

typedef struct R4_Fin_RNCUnsentDownlinkVolumeList {
    unsigned char   bit_mask;
#       define      R4_Fin_rNCUnsentDnVolumeList_present 0x80
    R4_Fin_GSNAddress sgsnAddress;
    struct R4_Fin__seqof1_ *rNCUnsentDnVolumeList;  /* optional; set in bit_mask
                                      * R4_Fin_rNCUnsentDnVolumeList_present if
                                      * present */
} R4_Fin_RNCUnsentDownlinkVolumeList;

typedef enum R4_Fin_ChChSelectionMode {
    R4_Fin_sGSNSupplied = 0,
    R4_Fin_subscriptionSpecific = 1,
    R4_Fin_aPNSpecific = 2,
    R4_Fin_homeDefault = 3,
    R4_Fin_roamingDefault = 4,
    R4_Fin_visitingDefault = 5
} R4_Fin_ChChSelectionMode;

typedef ossBoolean      R4_Fin_DynamicAddressFlag;

typedef enum R4_Fin_ConsolidationResult {
    R4_Fin_normal = 0,
    R4_Fin_notNormal = 1,
    R4_Fin_forInterSGSNConsolidation = 2,
    R4_Fin_reachLimit = 3,
    R4_Fin_onlyOneCDRGenerated = 4
} R4_Fin_ConsolidationResult;

typedef struct R4_Fin__seqof2_ {
    struct R4_Fin__seqof2_ *next;
    R4_Fin_GSNAddress value;
} *R4_Fin__seqof2;

typedef struct R4_Fin__seqof3_ {
    struct R4_Fin__seqof3_ *next;
    R4_Fin_ChangeOfCharCondition value;
} *R4_Fin__seqof3;

typedef struct R4_Fin_SGSNPDPRecord {
    unsigned int    bit_mask;
#       define      R4_Fin_SGSNPDPRecord_networkInitiation_present 0x80000000
#       define      R4_Fin_SGSNPDPRecord_servedIMEI_present 0x40000000
#       define      R4_Fin_SGSNPDPRecord_sgsnAddress_present 0x20000000
#       define      R4_Fin_SGSNPDPRecord_msNetworkCapability_present 0x10000000
#       define      R4_Fin_SGSNPDPRecord_routingArea_present 0x08000000
#       define      R4_Fin_SGSNPDPRecord_locationAreaCode_present 0x04000000
#       define      R4_Fin_SGSNPDPRecord_cellIdentifier_present 0x02000000
#       define      R4_Fin_SGSNPDPRecord_accessPointNameNI_present 0x01000000
#       define      R4_Fin_SGSNPDPRecord_pdpType_present 0x00800000
#       define      R4_Fin_SGSNPDPRecord_servedPDPAddress_present 0x00400000
#       define      R4_Fin_SGSNPDPRecord_listOfTrafficVolumes_present 0x00200000
#       define      R4_Fin_SGSNPDPRecord_sgsnChange_present 0x00100000
#       define      R4_Fin_SGSNPDPRecord_diagnostics_present 0x00080000
#       define      R4_Fin_SGSNPDPRecord_recSequenceNumList_present 0x00040000
#       define      R4_Fin_SGSNPDPRecord_nodeID_present 0x00020000
#       define      R4_Fin_SGSNPDPRecord_recordExtensions_present 0x00010000
#       define      R4_Fin_SGSNPDPRecord_localSequenceNumberList_present 0x00008000
#       define      R4_Fin_SGSNPDPRecord_apnSelectionMode_present 0x00004000
#       define      R4_Fin_accessPointNameOI_present 0x00002000
#       define      R4_Fin_SGSNPDPRecord_servedMSISDN_present 0x00001000
#       define      R4_Fin_SGSNPDPRecord_systemType_present 0x00000800
#       define      R4_Fin_cAMELInformationPDP_present 0x00000400
#       define      R4_Fin_rNCUnsentDownlinkVolumeList_present 0x00000200
#       define      R4_Fin_SGSNPDPRecord_chChSelectionMode_present 0x00000100
#       define      R4_Fin_SGSNPDPRecord_dynamicAddressFlag_present 0x00000080
    R4_Fin_CallEventRecordType recordType;
    R4_Fin_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                            * R4_Fin_SGSNPDPRecord_networkInitiation_present if
                            * present */
    R4_Fin_IMSI     servedIMSI;
    R4_Fin_IMEI     servedIMEI;  /* optional; set in bit_mask
                                  * R4_Fin_SGSNPDPRecord_servedIMEI_present if
                                  * present */
    struct R4_Fin__seqof2_ *sgsnAddress;  /* optional; set in bit_mask
                                  * R4_Fin_SGSNPDPRecord_sgsnAddress_present if
                                  * present */
    R4_Fin_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                          * R4_Fin_SGSNPDPRecord_msNetworkCapability_present if
                          * present */
    R4_Fin_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                  * R4_Fin_SGSNPDPRecord_routingArea_present if
                                  * present */
    R4_Fin_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                             * R4_Fin_SGSNPDPRecord_locationAreaCode_present if
                             * present */
    R4_Fin_CellId   cellIdentifier;  /* optional; set in bit_mask
                               * R4_Fin_SGSNPDPRecord_cellIdentifier_present if
                               * present */
    R4_Fin_ChargingID chargingID;
    R4_Fin_GSNAddress ggsnAddressUsed;
    R4_Fin_AccessPointNameNI accessPointNameNI;  /* optional; set in bit_mask
                            * R4_Fin_SGSNPDPRecord_accessPointNameNI_present if
                            * present */
    R4_Fin_PDPType  pdpType;  /* optional; set in bit_mask
                               * R4_Fin_SGSNPDPRecord_pdpType_present if
                               * present */
    R4_Fin_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                             * R4_Fin_SGSNPDPRecord_servedPDPAddress_present if
                             * present */
    struct R4_Fin__seqof3_ *listOfTrafficVolumes;  /* optional; set in bit_mask
                         * R4_Fin_SGSNPDPRecord_listOfTrafficVolumes_present if
                         * present */
    R4_Fin_TimeStamp recordOpeningTime;
    R4_Fin_CallDuration duration;
    R4_Fin_SGSNChange sgsnChange;  /* optional; set in bit_mask
                                    * R4_Fin_SGSNPDPRecord_sgsnChange_present if
                                    * present */
    R4_Fin_CauseForRecClosing causeForRecClosing;
    R4_Fin_Diagnostics diagnostics;  /* optional; set in bit_mask
                                      * R4_Fin_SGSNPDPRecord_diagnostics_present
                                      * if present */
    struct R4_Fin__seqof4 {
        struct R4_Fin__seqof4 *next;
        R4_Fin_SequenceList value;
    } *recSequenceNumList;  /* optional; set in bit_mask
                             * R4_Fin_SGSNPDPRecord_recSequenceNumList_present
                             * if present */
    R4_Fin_NodeID   nodeID;  /* optional; set in bit_mask
                              * R4_Fin_SGSNPDPRecord_nodeID_present if
                              * present */
    struct R4_Fin_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                             * R4_Fin_SGSNPDPRecord_recordExtensions_present if
                             * present */
    struct R4_Fin__seqof5 {
        struct R4_Fin__seqof5 *next;
        R4_Fin_LocalSequenceNumberList value;
    } *localSequenceNumberList;  /* optional; set in bit_mask
                      * R4_Fin_SGSNPDPRecord_localSequenceNumberList_present if
                      * present */
    R4_Fin_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                             * R4_Fin_SGSNPDPRecord_apnSelectionMode_present if
                             * present */
    R4_Fin_AccessPointNameOI accessPointNameOI;  /* optional; set in bit_mask
                                          * R4_Fin_accessPointNameOI_present if
                                          * present */
    R4_Fin_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * R4_Fin_SGSNPDPRecord_servedMSISDN_present
                                    * if present */
    R4_Fin_ChargingCharacteristics chargingCharacteristics;
    R4_Fin_SystemType systemType;  /* optional; set in bit_mask
                                    * R4_Fin_SGSNPDPRecord_systemType_present if
                                    * present */
    R4_Fin_CAMELInformationPDP cAMELInformationPDP;  /* optional; set in
                                   * bit_mask R4_Fin_cAMELInformationPDP_present
                                   * if present */
    struct R4_Fin__seqof6 {
        struct R4_Fin__seqof6 *next;
        R4_Fin_RNCUnsentDownlinkVolumeList value;
    } *rNCUnsentDownlinkVolumeList;  /* optional; set in bit_mask
                                * R4_Fin_rNCUnsentDownlinkVolumeList_present if
                                * present */
    R4_Fin_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                            * R4_Fin_SGSNPDPRecord_chChSelectionMode_present if
                            * present */
    R4_Fin_DynamicAddressFlag dynamicAddressFlag;  /* optional; set in bit_mask
                           * R4_Fin_SGSNPDPRecord_dynamicAddressFlag_present if
                           * present */
    R4_Fin_ConsolidationResult consolidationResult;
} R4_Fin_SGSNPDPRecord;

typedef struct R4_Fin_PLMN_Id {
    unsigned short  length;
    unsigned char   value[3];
} R4_Fin_PLMN_Id;

typedef struct R4_Fin_GGSNPDPRecord {
    unsigned short  bit_mask;
#       define      R4_Fin_GGSNPDPRecord_networkInitiation_present 0x8000
#       define      R4_Fin_GGSNPDPRecord_accessPointNameNI_present 0x4000
#       define      R4_Fin_GGSNPDPRecord_pdpType_present 0x2000
#       define      R4_Fin_GGSNPDPRecord_servedPDPAddress_present 0x1000
#       define      R4_Fin_GGSNPDPRecord_dynamicAddressFlag_present 0x0800
#       define      R4_Fin_GGSNPDPRecord_listOfTrafficVolumes_present 0x0400
#       define      R4_Fin_GGSNPDPRecord_diagnostics_present 0x0200
#       define      R4_Fin_GGSNPDPRecord_recSequenceNumList_present 0x0100
#       define      R4_Fin_GGSNPDPRecord_nodeID_present 0x0080
#       define      R4_Fin_GGSNPDPRecord_recordExtensions_present 0x0040
#       define      R4_Fin_GGSNPDPRecord_localSequenceNumberList_present 0x0020
#       define      R4_Fin_GGSNPDPRecord_apnSelectionMode_present 0x0010
#       define      R4_Fin_GGSNPDPRecord_servedMSISDN_present 0x0008
#       define      R4_Fin_GGSNPDPRecord_chChSelectionMode_present 0x0004
#       define      R4_Fin_sgsnPLMNIdentifier_present 0x0002
    R4_Fin_CallEventRecordType recordType;
    R4_Fin_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                            * R4_Fin_GGSNPDPRecord_networkInitiation_present if
                            * present */
    R4_Fin_IMSI     servedIMSI;
    R4_Fin_GSNAddress ggsnAddress;
    R4_Fin_ChargingID chargingID;
    struct R4_Fin__seqof2_ *sgsnAddress;
    R4_Fin_AccessPointNameNI accessPointNameNI;  /* optional; set in bit_mask
                            * R4_Fin_GGSNPDPRecord_accessPointNameNI_present if
                            * present */
    R4_Fin_PDPType  pdpType;  /* optional; set in bit_mask
                               * R4_Fin_GGSNPDPRecord_pdpType_present if
                               * present */
    R4_Fin_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                             * R4_Fin_GGSNPDPRecord_servedPDPAddress_present if
                             * present */
    R4_Fin_DynamicAddressFlag dynamicAddressFlag;  /* optional; set in bit_mask
                           * R4_Fin_GGSNPDPRecord_dynamicAddressFlag_present if
                           * present */
    struct R4_Fin__seqof3_ *listOfTrafficVolumes;  /* optional; set in bit_mask
                         * R4_Fin_GGSNPDPRecord_listOfTrafficVolumes_present if
                         * present */
    R4_Fin_TimeStamp recordOpeningTime;
    R4_Fin_CallDuration duration;
    R4_Fin_CauseForRecClosing causeForRecClosing;
    R4_Fin_Diagnostics diagnostics;  /* optional; set in bit_mask
                                      * R4_Fin_GGSNPDPRecord_diagnostics_present
                                      * if present */
    R4_Fin_SequenceList recSequenceNumList;  /* optional; set in bit_mask
                           * R4_Fin_GGSNPDPRecord_recSequenceNumList_present if
                           * present */
    R4_Fin_NodeID   nodeID;  /* optional; set in bit_mask
                              * R4_Fin_GGSNPDPRecord_nodeID_present if
                              * present */
    struct R4_Fin_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                             * R4_Fin_GGSNPDPRecord_recordExtensions_present if
                             * present */
    R4_Fin_LocalSequenceNumberList localSequenceNumberList;  /* optional; set in
                                   * bit_mask
                      * R4_Fin_GGSNPDPRecord_localSequenceNumberList_present if
                      * present */
    R4_Fin_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                             * R4_Fin_GGSNPDPRecord_apnSelectionMode_present if
                             * present */
    R4_Fin_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * R4_Fin_GGSNPDPRecord_servedMSISDN_present
                                    * if present */
    R4_Fin_ChargingCharacteristics chargingCharacteristics;
    R4_Fin_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                            * R4_Fin_GGSNPDPRecord_chChSelectionMode_present if
                            * present */
    R4_Fin_PLMN_Id  sgsnPLMNIdentifier;  /* optional; set in bit_mask
                                          * R4_Fin_sgsnPLMNIdentifier_present if
                                          * present */
    R4_Fin_ConsolidationResult consolidationResult;
} R4_Fin_GGSNPDPRecord;

typedef struct R4_Fin_ChangeLocation {
    unsigned char   bit_mask;
#       define      R4_Fin_cellId_present 0x80
    R4_Fin_LocationAreaCode locationAreaCode;
    R4_Fin_RoutingAreaCode routingAreaCode;
    R4_Fin_CellId   cellId;  /* optional; set in bit_mask R4_Fin_cellId_present
                              * if present */
    R4_Fin_TimeStamp changeTime;
} R4_Fin_ChangeLocation;

typedef unsigned int    R4_Fin_LocalSequenceNumber;

typedef struct R4_Fin_CAMELInformationMM {
    unsigned char   bit_mask;
#       define      R4_Fin_CAMELInformationMM_sCFAddress_present 0x80
#       define      R4_Fin_CAMELInformationMM_serviceKey_present 0x40
#       define      R4_Fin_CAMELInformationMM_defaultTransactionHandling_present 0x20
#       define      R4_Fin_CAMELInformationMM_numberOfDPEncountered_present 0x10
#       define      R4_Fin_CAMELInformationMM_levelOfCAMELService_present 0x08
#       define      R4_Fin_CAMELInformationMM_freeFormatData_present 0x04
#       define      R4_Fin_CAMELInformationMM_fFDAppendIndicator_present 0x02
    R4_Fin_SCFAddress sCFAddress;  /* optional; set in bit_mask
                              * R4_Fin_CAMELInformationMM_sCFAddress_present if
                              * present */
    R4_Fin_ServiceKey serviceKey;  /* optional; set in bit_mask
                              * R4_Fin_CAMELInformationMM_serviceKey_present if
                              * present */
    R4_Fin_DefaultGPRS_Handling defaultTransactionHandling;  /* optional; set in
                                   * bit_mask
              * R4_Fin_CAMELInformationMM_defaultTransactionHandling_present if
              * present */
    R4_Fin_NumberOfDPEncountered numberOfDPEncountered;  /* optional; set in
                                   * bit_mask
                   * R4_Fin_CAMELInformationMM_numberOfDPEncountered_present if
                   * present */
    R4_Fin_LevelOfCAMELService levelOfCAMELService;  /* optional; set in
                                   * bit_mask
                     * R4_Fin_CAMELInformationMM_levelOfCAMELService_present if
                     * present */
    R4_Fin_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                          * R4_Fin_CAMELInformationMM_freeFormatData_present if
                          * present */
    R4_Fin_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in bit_mask
                      * R4_Fin_CAMELInformationMM_fFDAppendIndicator_present if
                      * present */
} R4_Fin_CAMELInformationMM;

typedef struct R4_Fin_SGSNMMRecord {
    unsigned int    bit_mask;
#       define      R4_Fin_SGSNMMRecord_servedIMEI_present 0x80000000
#       define      R4_Fin_SGSNMMRecord_sgsnAddress_present 0x40000000
#       define      R4_Fin_SGSNMMRecord_msNetworkCapability_present 0x20000000
#       define      R4_Fin_SGSNMMRecord_routingArea_present 0x10000000
#       define      R4_Fin_SGSNMMRecord_locationAreaCode_present 0x08000000
#       define      R4_Fin_SGSNMMRecord_cellIdentifier_present 0x04000000
#       define      R4_Fin_changeLocation_present 0x02000000
#       define      R4_Fin_duration_present 0x01000000
#       define      R4_Fin_SGSNMMRecord_sgsnChange_present 0x00800000
#       define      R4_Fin_SGSNMMRecord_diagnostics_present 0x00400000
#       define      R4_Fin_recordSequenceNumber_present 0x00200000
#       define      R4_Fin_SGSNMMRecord_nodeID_present 0x00100000
#       define      R4_Fin_SGSNMMRecord_recordExtensions_present 0x00080000
#       define      R4_Fin_SGSNMMRecord_localSequenceNumber_present 0x00040000
#       define      R4_Fin_SGSNMMRecord_servedMSISDN_present 0x00020000
#       define      R4_Fin_cAMELInformationMM_present 0x00010000
#       define      R4_Fin_SGSNMMRecord_systemType_present 0x00008000
#       define      R4_Fin_SGSNMMRecord_chChSelectionMode_present 0x00004000
    R4_Fin_CallEventRecordType recordType;
    R4_Fin_IMSI     servedIMSI;
    R4_Fin_IMEI     servedIMEI;  /* optional; set in bit_mask
                                  * R4_Fin_SGSNMMRecord_servedIMEI_present if
                                  * present */
    R4_Fin_GSNAddress sgsnAddress;  /* optional; set in bit_mask
                                     * R4_Fin_SGSNMMRecord_sgsnAddress_present
                                     * if present */
    R4_Fin_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                           * R4_Fin_SGSNMMRecord_msNetworkCapability_present if
                           * present */
    R4_Fin_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                   * R4_Fin_SGSNMMRecord_routingArea_present if
                                   * present */
    R4_Fin_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                              * R4_Fin_SGSNMMRecord_locationAreaCode_present if
                              * present */
    R4_Fin_CellId   cellIdentifier;  /* optional; set in bit_mask
                                * R4_Fin_SGSNMMRecord_cellIdentifier_present if
                                * present */
    struct R4_Fin__seqof7 {
        struct R4_Fin__seqof7 *next;
        R4_Fin_ChangeLocation value;
    } *changeLocation;  /* optional; set in bit_mask
                         * R4_Fin_changeLocation_present if present */
    R4_Fin_TimeStamp recordOpeningTime;
    R4_Fin_CallDuration duration;  /* optional; set in bit_mask
                                    * R4_Fin_duration_present if present */
    R4_Fin_SGSNChange sgsnChange;  /* optional; set in bit_mask
                                    * R4_Fin_SGSNMMRecord_sgsnChange_present if
                                    * present */
    R4_Fin_CauseForRecClosing causeForRecClosing;
    R4_Fin_Diagnostics diagnostics;  /* optional; set in bit_mask
                                      * R4_Fin_SGSNMMRecord_diagnostics_present
                                      * if present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                                       * R4_Fin_recordSequenceNumber_present if
                                       * present */
    R4_Fin_NodeID   nodeID;  /* optional; set in bit_mask
                              * R4_Fin_SGSNMMRecord_nodeID_present if present */
    struct R4_Fin_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                              * R4_Fin_SGSNMMRecord_recordExtensions_present if
                              * present */
    R4_Fin_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                           * R4_Fin_SGSNMMRecord_localSequenceNumber_present if
                           * present */
    R4_Fin_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * R4_Fin_SGSNMMRecord_servedMSISDN_present
                                    * if present */
    R4_Fin_ChargingCharacteristics chargingCharacteristics;
    R4_Fin_CAMELInformationMM cAMELInformationMM;  /* optional; set in bit_mask
                                         * R4_Fin_cAMELInformationMM_present if
                                         * present */
    R4_Fin_SystemType systemType;  /* optional; set in bit_mask
                                    * R4_Fin_SGSNMMRecord_systemType_present if
                                    * present */
    R4_Fin_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                             * R4_Fin_SGSNMMRecord_chChSelectionMode_present if
                             * present */
} R4_Fin_SGSNMMRecord;

typedef R4_Fin_AddressString R4_Fin_RecordingEntity;

typedef struct R4_Fin_MessageReference {
    unsigned int    length;
    unsigned char   *value;
} R4_Fin_MessageReference;

typedef R4_Fin_Diagnostics R4_Fin_SMSResult;

typedef struct R4_Fin_CalledNumber {
    unsigned short  length;
    unsigned char   value[12];
} R4_Fin_CalledNumber;

typedef enum R4_Fin_DefaultSMS_Handling {
    R4_Fin_DefaultSMS_Handling_continueTransaction = 0,
    R4_Fin_DefaultSMS_Handling_releaseTransaction = 1
} R4_Fin_DefaultSMS_Handling;

typedef struct R4_Fin_CallingNumber {
    unsigned short  length;
    unsigned char   value[10];
} R4_Fin_CallingNumber;

typedef struct R4_Fin_CallReferenceNumber {
    unsigned short  length;
    unsigned char   value[8];
} R4_Fin_CallReferenceNumber;

typedef struct R4_Fin_CAMELInformationSMS {
    unsigned char   bit_mask;
#       define      R4_Fin_CAMELInformationSMS_sCFAddress_present 0x80
#       define      R4_Fin_CAMELInformationSMS_serviceKey_present 0x40
#       define      R4_Fin_defaultSMSHandling_present 0x20
#       define      R4_Fin_cAMELCallingPartyNumber_present 0x10
#       define      R4_Fin_cAMELDestinationSubscriberNumber_present 0x08
#       define      R4_Fin_cAMELSMSCAddress_present 0x04
#       define      R4_Fin_CAMELInformationSMS_freeFormatData_present 0x02
#       define      R4_Fin_smsReferenceNumber_present 0x01
    R4_Fin_SCFAddress sCFAddress;  /* optional; set in bit_mask
                             * R4_Fin_CAMELInformationSMS_sCFAddress_present if
                             * present */
    R4_Fin_ServiceKey serviceKey;  /* optional; set in bit_mask
                             * R4_Fin_CAMELInformationSMS_serviceKey_present if
                             * present */
    R4_Fin_DefaultSMS_Handling defaultSMSHandling;  /* optional; set in bit_mask
                                         * R4_Fin_defaultSMSHandling_present if
                                         * present */
    R4_Fin_CallingNumber cAMELCallingPartyNumber;  /* optional; set in bit_mask
                                    * R4_Fin_cAMELCallingPartyNumber_present if
                                    * present */
    R4_Fin_CalledNumber cAMELDestinationSubscriberNumber;  /* optional; set in
                                   * bit_mask
                           * R4_Fin_cAMELDestinationSubscriberNumber_present if
                           * present */
    R4_Fin_AddressString cAMELSMSCAddress;  /* optional; set in bit_mask
                                             * R4_Fin_cAMELSMSCAddress_present
                                             * if present */
    R4_Fin_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                         * R4_Fin_CAMELInformationSMS_freeFormatData_present if
                         * present */
    R4_Fin_CallReferenceNumber smsReferenceNumber;  /* optional; set in bit_mask
                                         * R4_Fin_smsReferenceNumber_present if
                                         * present */
} R4_Fin_CAMELInformationSMS;

typedef struct R4_Fin_SGSNSMORecord {
    unsigned short  bit_mask;
#       define      R4_Fin_SGSNSMORecord_servedIMEI_present 0x8000
#       define      R4_Fin_SGSNSMORecord_servedMSISDN_present 0x4000
#       define      R4_Fin_SGSNSMORecord_msNetworkCapability_present 0x2000
#       define      R4_Fin_SGSNSMORecord_serviceCentre_present 0x1000
#       define      R4_Fin_SGSNSMORecord_recordingEntity_present 0x0800
#       define      R4_Fin_SGSNSMORecord_locationArea_present 0x0400
#       define      R4_Fin_SGSNSMORecord_routingArea_present 0x0200
#       define      R4_Fin_SGSNSMORecord_cellIdentifier_present 0x0100
#       define      R4_Fin_SGSNSMORecord_smsResult_present 0x0080
#       define      R4_Fin_SGSNSMORecord_recordExtensions_present 0x0040
#       define      R4_Fin_SGSNSMORecord_nodeID_present 0x0020
#       define      R4_Fin_SGSNSMORecord_localSequenceNumber_present 0x0010
#       define      R4_Fin_SGSNSMORecord_systemType_present 0x0008
#       define      R4_Fin_destinationNumber_present 0x0004
#       define      R4_Fin_SGSNSMORecord_cAMELInformationSMS_present 0x0002
#       define      R4_Fin_SGSNSMORecord_chChSelectionMode_present 0x0001
    R4_Fin_CallEventRecordType recordType;
    R4_Fin_IMSI     servedIMSI;
    R4_Fin_IMEI     servedIMEI;  /* optional; set in bit_mask
                                  * R4_Fin_SGSNSMORecord_servedIMEI_present if
                                  * present */
    R4_Fin_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * R4_Fin_SGSNSMORecord_servedMSISDN_present
                                    * if present */
    R4_Fin_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                          * R4_Fin_SGSNSMORecord_msNetworkCapability_present if
                          * present */
    R4_Fin_AddressString serviceCentre;  /* optional; set in bit_mask
                                * R4_Fin_SGSNSMORecord_serviceCentre_present if
                                * present */
    R4_Fin_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                              * R4_Fin_SGSNSMORecord_recordingEntity_present if
                              * present */
    R4_Fin_LocationAreaCode locationArea;  /* optional; set in bit_mask
                                 * R4_Fin_SGSNSMORecord_locationArea_present if
                                 * present */
    R4_Fin_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                  * R4_Fin_SGSNSMORecord_routingArea_present if
                                  * present */
    R4_Fin_CellId   cellIdentifier;  /* optional; set in bit_mask
                               * R4_Fin_SGSNSMORecord_cellIdentifier_present if
                               * present */
    R4_Fin_MessageReference messageReference;
    R4_Fin_TimeStamp eventTimeStamp;
    R4_Fin_SMSResult smsResult;  /* optional; set in bit_mask
                                  * R4_Fin_SGSNSMORecord_smsResult_present if
                                  * present */
    struct R4_Fin_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                             * R4_Fin_SGSNSMORecord_recordExtensions_present if
                             * present */
    R4_Fin_NodeID   nodeID;  /* optional; set in bit_mask
                              * R4_Fin_SGSNSMORecord_nodeID_present if
                              * present */
    R4_Fin_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                          * R4_Fin_SGSNSMORecord_localSequenceNumber_present if
                          * present */
    R4_Fin_ChargingCharacteristics chargingCharacteristics;
    R4_Fin_SystemType systemType;  /* optional; set in bit_mask
                                    * R4_Fin_SGSNSMORecord_systemType_present if
                                    * present */
    R4_Fin_CalledNumber destinationNumber;  /* optional; set in bit_mask
                                             * R4_Fin_destinationNumber_present
                                             * if present */
    R4_Fin_CAMELInformationSMS cAMELInformationSMS;  /* optional; set in
                                   * bit_mask
                          * R4_Fin_SGSNSMORecord_cAMELInformationSMS_present if
                          * present */
    R4_Fin_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                            * R4_Fin_SGSNSMORecord_chChSelectionMode_present if
                            * present */
} R4_Fin_SGSNSMORecord;

typedef struct R4_Fin_SGSNSMTRecord {
    unsigned short  bit_mask;
#       define      R4_Fin_SGSNSMTRecord_servedIMEI_present 0x8000
#       define      R4_Fin_SGSNSMTRecord_servedMSISDN_present 0x4000
#       define      R4_Fin_SGSNSMTRecord_msNetworkCapability_present 0x2000
#       define      R4_Fin_SGSNSMTRecord_serviceCentre_present 0x1000
#       define      R4_Fin_SGSNSMTRecord_recordingEntity_present 0x0800
#       define      R4_Fin_SGSNSMTRecord_locationArea_present 0x0400
#       define      R4_Fin_SGSNSMTRecord_routingArea_present 0x0200
#       define      R4_Fin_SGSNSMTRecord_cellIdentifier_present 0x0100
#       define      R4_Fin_SGSNSMTRecord_smsResult_present 0x0080
#       define      R4_Fin_SGSNSMTRecord_recordExtensions_present 0x0040
#       define      R4_Fin_SGSNSMTRecord_nodeID_present 0x0020
#       define      R4_Fin_SGSNSMTRecord_localSequenceNumber_present 0x0010
#       define      R4_Fin_SGSNSMTRecord_systemType_present 0x0008
#       define      R4_Fin_SGSNSMTRecord_chChSelectionMode_present 0x0004
#       define      R4_Fin_SGSNSMTRecord_cAMELInformationSMS_present 0x0002
    R4_Fin_CallEventRecordType recordType;
    R4_Fin_IMSI     servedIMSI;
    R4_Fin_IMEI     servedIMEI;  /* optional; set in bit_mask
                                  * R4_Fin_SGSNSMTRecord_servedIMEI_present if
                                  * present */
    R4_Fin_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * R4_Fin_SGSNSMTRecord_servedMSISDN_present
                                    * if present */
    R4_Fin_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                          * R4_Fin_SGSNSMTRecord_msNetworkCapability_present if
                          * present */
    R4_Fin_AddressString serviceCentre;  /* optional; set in bit_mask
                                * R4_Fin_SGSNSMTRecord_serviceCentre_present if
                                * present */
    R4_Fin_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                              * R4_Fin_SGSNSMTRecord_recordingEntity_present if
                              * present */
    R4_Fin_LocationAreaCode locationArea;  /* optional; set in bit_mask
                                 * R4_Fin_SGSNSMTRecord_locationArea_present if
                                 * present */
    R4_Fin_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                  * R4_Fin_SGSNSMTRecord_routingArea_present if
                                  * present */
    R4_Fin_CellId   cellIdentifier;  /* optional; set in bit_mask
                               * R4_Fin_SGSNSMTRecord_cellIdentifier_present if
                               * present */
    R4_Fin_TimeStamp eventTimeStamp;
    R4_Fin_SMSResult smsResult;  /* optional; set in bit_mask
                                  * R4_Fin_SGSNSMTRecord_smsResult_present if
                                  * present */
    struct R4_Fin_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                             * R4_Fin_SGSNSMTRecord_recordExtensions_present if
                             * present */
    R4_Fin_NodeID   nodeID;  /* optional; set in bit_mask
                              * R4_Fin_SGSNSMTRecord_nodeID_present if
                              * present */
    R4_Fin_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                          * R4_Fin_SGSNSMTRecord_localSequenceNumber_present if
                          * present */
    R4_Fin_ChargingCharacteristics chargingCharacteristics;
    R4_Fin_SystemType systemType;  /* optional; set in bit_mask
                                    * R4_Fin_SGSNSMTRecord_systemType_present if
                                    * present */
    R4_Fin_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                            * R4_Fin_SGSNSMTRecord_chChSelectionMode_present if
                            * present */
    R4_Fin_CAMELInformationSMS cAMELInformationSMS;  /* optional; set in
                                   * bit_mask
                          * R4_Fin_SGSNSMTRecord_cAMELInformationSMS_present if
                          * present */
} R4_Fin_SGSNSMTRecord;

typedef enum R4_Fin_LCSClientType {
    R4_Fin_emergencyServices = 0,
    R4_Fin_valueAddedServices = 1,
    R4_Fin_plmnOperatorServices = 2,
    R4_Fin_lawfulInterceptServices = 3
} R4_Fin_LCSClientType;

typedef struct R4_Fin_LCSClientExternalID {
    unsigned char   bit_mask;
#       define      R4_Fin_externalAddress_present 0x80
    R4_Fin_AddressString externalAddress;  /* optional; set in bit_mask
                                            * R4_Fin_externalAddress_present if
                                            * present */
} R4_Fin_LCSClientExternalID;

typedef enum R4_Fin_LCSClientInternalID {
    R4_Fin_broadcastService = 0,
    R4_Fin_o_andM_HPLMN = 1,
    R4_Fin_o_andM_VPLMN = 2,
    R4_Fin_anonymousLocation = 3,
    R4_Fin_targetMSsubscribedService = 4
} R4_Fin_LCSClientInternalID;

typedef struct R4_Fin_LCSClientIdentity {
    unsigned char   bit_mask;
#       define      R4_Fin_lcsClientExternalID_present 0x80
#       define      R4_Fin_lcsClientDialedByMS_present 0x40
#       define      R4_Fin_lcsClientInternalID_present 0x20
    R4_Fin_LCSClientExternalID lcsClientExternalID;  /* optional; set in
                                   * bit_mask R4_Fin_lcsClientExternalID_present
                                   * if present */
    R4_Fin_AddressString lcsClientDialedByMS;  /* optional; set in bit_mask
                                        * R4_Fin_lcsClientDialedByMS_present if
                                        * present */
    R4_Fin_LCSClientInternalID lcsClientInternalID;  /* optional; set in
                                   * bit_mask R4_Fin_lcsClientInternalID_present
                                   * if present */
} R4_Fin_LCSClientIdentity;

typedef enum R4_Fin_LocationEstimateType {
    R4_Fin_currentLocation = 0,
    R4_Fin_currentOrLastKnownLocation = 1,
    R4_Fin_initialLocation = 2,
    R4_Fin_activateDeferredLocation = 3,
    R4_Fin_cancelDeferredLocation = 4
} R4_Fin_LocationEstimateType;

typedef unsigned short  R4_Fin_DeferredLocationEventType;
#define                     R4_Fin_msAvailable 0x8000

typedef struct R4_Fin_LocationType {
    unsigned char   bit_mask;
#       define      R4_Fin_deferredLocationEventType_present 0x80
    R4_Fin_LocationEstimateType locationEstimateType;
    R4_Fin_DeferredLocationEventType deferredLocationEventType;  /* optional;
                                   * set in bit_mask
                                   * R4_Fin_deferredLocationEventType_present if
                                   * present */
} R4_Fin_LocationType;

typedef struct R4_Fin_LCSQoSInfo {
    unsigned short  length;
    unsigned char   value[4];
} R4_Fin_LCSQoSInfo;

typedef struct R4_Fin_LCS_Priority {
    unsigned short  length;
    unsigned char   value[1];
} R4_Fin_LCS_Priority;

typedef enum R4_Fin_NotificationToMSUser {
    R4_Fin_notifyLocationAllowed = 0,
    R4_Fin_notifyAndVerify_LocationAllowedIfNoResponse = 1,
    R4_Fin_notifyAndVerify_LocationNotAllowedIfNoResponse = 2,
    R4_Fin_locationNotAllowed = 3
} R4_Fin_NotificationToMSUser;

typedef struct R4_Fin_LocationAreaAndCell {
    R4_Fin_LocationAreaCode locationAreaCode;
    R4_Fin_CellId   cellId;
} R4_Fin_LocationAreaAndCell;

typedef struct R4_Fin_Ext_GeographicalInformation {
    unsigned short  length;
    unsigned char   value[20];
} R4_Fin_Ext_GeographicalInformation;

typedef struct R4_Fin_LCSCause {
    unsigned short  length;
    unsigned char   value[1];
} R4_Fin_LCSCause;

typedef struct R4_Fin_SGSNLCTRecord {
    unsigned int    bit_mask;
#       define      R4_Fin_SGSNLCTRecord_servedMSISDN_present 0x80000000
#       define      R4_Fin_SGSNLCTRecord_sgsnAddress_present 0x40000000
#       define      R4_Fin_SGSNLCTRecord_lcsQos_present 0x20000000
#       define      R4_Fin_SGSNLCTRecord_lcsPriority_present 0x10000000
#       define      R4_Fin_SGSNLCTRecord_measurementDuration_present 0x08000000
#       define      R4_Fin_notificationToMSUser_present 0x04000000
#       define      R4_Fin_privacyOverride_present 0x02000000
#       define      R4_Fin_SGSNLCTRecord_location_present 0x01000000
#       define      R4_Fin_SGSNLCTRecord_routingArea_present 0x00800000
#       define      R4_Fin_SGSNLCTRecord_locationEstimate_present 0x00400000
#       define      R4_Fin_SGSNLCTRecord_lcsCause_present 0x00200000
#       define      R4_Fin_SGSNLCTRecord_diagnostics_present 0x00100000
#       define      R4_Fin_SGSNLCTRecord_nodeID_present 0x00080000
#       define      R4_Fin_SGSNLCTRecord_localSequenceNumber_present 0x00040000
#       define      R4_Fin_SGSNLCTRecord_chChSelectionMode_present 0x00020000
#       define      R4_Fin_SGSNLCTRecord_systemType_present 0x00010000
#       define      R4_Fin_SGSNLCTRecord_recordExtensions_present 0x00008000
    R4_Fin_CallEventRecordType recordType;
    R4_Fin_RecordingEntity recordingEntity;
    R4_Fin_LCSClientType lcsClientType;
    R4_Fin_LCSClientIdentity lcsClientIdentity;
    R4_Fin_IMSI     servedIMSI;
    R4_Fin_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * R4_Fin_SGSNLCTRecord_servedMSISDN_present
                                    * if present */
    R4_Fin_GSNAddress sgsnAddress;  /* optional; set in bit_mask
                                     * R4_Fin_SGSNLCTRecord_sgsnAddress_present
                                     * if present */
    R4_Fin_LocationType locationType;
    R4_Fin_LCSQoSInfo lcsQos;  /* optional; set in bit_mask
                                * R4_Fin_SGSNLCTRecord_lcsQos_present if
                                * present */
    R4_Fin_LCS_Priority lcsPriority;  /* optional; set in bit_mask
                                  * R4_Fin_SGSNLCTRecord_lcsPriority_present if
                                  * present */
    R4_Fin_ISDN_AddressString mlcNumber;
    R4_Fin_TimeStamp eventTimeStamp;
    R4_Fin_CallDuration measurementDuration;  /* optional; set in bit_mask
                          * R4_Fin_SGSNLCTRecord_measurementDuration_present if
                          * present */
    R4_Fin_NotificationToMSUser notificationToMSUser;  /* optional; set in
                                   * bit_mask
                                   * R4_Fin_notificationToMSUser_present if
                                   * present */
    Nulltype        privacyOverride;  /* optional; set in bit_mask
                                       * R4_Fin_privacyOverride_present if
                                       * present */
    R4_Fin_LocationAreaAndCell location;  /* optional; set in bit_mask
                                     * R4_Fin_SGSNLCTRecord_location_present if
                                     * present */
    R4_Fin_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                  * R4_Fin_SGSNLCTRecord_routingArea_present if
                                  * present */
    R4_Fin_Ext_GeographicalInformation locationEstimate;  /* optional; set in
                                   * bit_mask
                             * R4_Fin_SGSNLCTRecord_locationEstimate_present if
                             * present */
    R4_Fin_LCSCause lcsCause;  /* optional; set in bit_mask
                                * R4_Fin_SGSNLCTRecord_lcsCause_present if
                                * present */
    R4_Fin_Diagnostics diagnostics;  /* optional; set in bit_mask
                                      * R4_Fin_SGSNLCTRecord_diagnostics_present
                                      * if present */
    R4_Fin_NodeID   nodeID;  /* optional; set in bit_mask
                              * R4_Fin_SGSNLCTRecord_nodeID_present if
                              * present */
    R4_Fin_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                          * R4_Fin_SGSNLCTRecord_localSequenceNumber_present if
                          * present */
    R4_Fin_ChargingCharacteristics chargingCharacteristics;
    R4_Fin_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                            * R4_Fin_SGSNLCTRecord_chChSelectionMode_present if
                            * present */
    R4_Fin_SystemType systemType;  /* optional; set in bit_mask
                                    * R4_Fin_SGSNLCTRecord_systemType_present if
                                    * present */
    struct R4_Fin_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                             * R4_Fin_SGSNLCTRecord_recordExtensions_present if
                             * present */
    R4_Fin_CauseForRecClosing causeForRecClosing;
} R4_Fin_SGSNLCTRecord;

typedef enum R4_Fin_LocationMethod {
    R4_Fin_msBasedEOTD = 0,
    R4_Fin_msAssistedEOTD = 1,
    R4_Fin_assistedGPS = 2,
    R4_Fin_msBasedOTDOA = 3,
    R4_Fin_msAssistedOTDOA = 4
} R4_Fin_LocationMethod;

typedef struct R4_Fin_SGSNLCORecord {
    unsigned int    bit_mask;
#       define      R4_Fin_SGSNLCORecord_lcsClientType_present 0x80000000
#       define      R4_Fin_SGSNLCORecord_lcsClientIdentity_present 0x40000000
#       define      R4_Fin_SGSNLCORecord_servedMSISDN_present 0x20000000
#       define      R4_Fin_SGSNLCORecord_sgsnAddress_present 0x10000000
#       define      R4_Fin_SGSNLCORecord_lcsQos_present 0x08000000
#       define      R4_Fin_SGSNLCORecord_lcsPriority_present 0x04000000
#       define      R4_Fin_SGSNLCORecord_mlcNumber_present 0x02000000
#       define      R4_Fin_SGSNLCORecord_measurementDuration_present 0x01000000
#       define      R4_Fin_SGSNLCORecord_location_present 0x00800000
#       define      R4_Fin_SGSNLCORecord_routingArea_present 0x00400000
#       define      R4_Fin_SGSNLCORecord_locationEstimate_present 0x00200000
#       define      R4_Fin_SGSNLCORecord_lcsCause_present 0x00100000
#       define      R4_Fin_SGSNLCORecord_diagnostics_present 0x00080000
#       define      R4_Fin_SGSNLCORecord_nodeID_present 0x00040000
#       define      R4_Fin_SGSNLCORecord_localSequenceNumber_present 0x00020000
#       define      R4_Fin_SGSNLCORecord_chChSelectionMode_present 0x00010000
#       define      R4_Fin_SGSNLCORecord_systemType_present 0x00008000
#       define      R4_Fin_SGSNLCORecord_recordExtensions_present 0x00004000
    R4_Fin_CallEventRecordType recordType;
    R4_Fin_RecordingEntity recordingEntity;
    R4_Fin_LCSClientType lcsClientType;  /* optional; set in bit_mask
                                * R4_Fin_SGSNLCORecord_lcsClientType_present if
                                * present */
    R4_Fin_LCSClientIdentity lcsClientIdentity;  /* optional; set in bit_mask
                            * R4_Fin_SGSNLCORecord_lcsClientIdentity_present if
                            * present */
    R4_Fin_IMSI     servedIMSI;
    R4_Fin_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * R4_Fin_SGSNLCORecord_servedMSISDN_present
                                    * if present */
    R4_Fin_GSNAddress sgsnAddress;  /* optional; set in bit_mask
                                     * R4_Fin_SGSNLCORecord_sgsnAddress_present
                                     * if present */
    R4_Fin_LocationMethod locationMethod;
    R4_Fin_LCSQoSInfo lcsQos;  /* optional; set in bit_mask
                                * R4_Fin_SGSNLCORecord_lcsQos_present if
                                * present */
    R4_Fin_LCS_Priority lcsPriority;  /* optional; set in bit_mask
                                  * R4_Fin_SGSNLCORecord_lcsPriority_present if
                                  * present */
    R4_Fin_ISDN_AddressString mlcNumber;  /* optional; set in bit_mask
                                    * R4_Fin_SGSNLCORecord_mlcNumber_present if
                                    * present */
    R4_Fin_TimeStamp eventTimeStamp;
    R4_Fin_CallDuration measurementDuration;  /* optional; set in bit_mask
                          * R4_Fin_SGSNLCORecord_measurementDuration_present if
                          * present */
    R4_Fin_LocationAreaAndCell location;  /* optional; set in bit_mask
                                     * R4_Fin_SGSNLCORecord_location_present if
                                     * present */
    R4_Fin_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                  * R4_Fin_SGSNLCORecord_routingArea_present if
                                  * present */
    R4_Fin_Ext_GeographicalInformation locationEstimate;  /* optional; set in
                                   * bit_mask
                             * R4_Fin_SGSNLCORecord_locationEstimate_present if
                             * present */
    R4_Fin_LCSCause lcsCause;  /* optional; set in bit_mask
                                * R4_Fin_SGSNLCORecord_lcsCause_present if
                                * present */
    R4_Fin_Diagnostics diagnostics;  /* optional; set in bit_mask
                                      * R4_Fin_SGSNLCORecord_diagnostics_present
                                      * if present */
    R4_Fin_NodeID   nodeID;  /* optional; set in bit_mask
                              * R4_Fin_SGSNLCORecord_nodeID_present if
                              * present */
    R4_Fin_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                          * R4_Fin_SGSNLCORecord_localSequenceNumber_present if
                          * present */
    R4_Fin_ChargingCharacteristics chargingCharacteristics;
    R4_Fin_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                            * R4_Fin_SGSNLCORecord_chChSelectionMode_present if
                            * present */
    R4_Fin_SystemType systemType;  /* optional; set in bit_mask
                                    * R4_Fin_SGSNLCORecord_systemType_present if
                                    * present */
    struct R4_Fin_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                             * R4_Fin_SGSNLCORecord_recordExtensions_present if
                             * present */
    R4_Fin_CauseForRecClosing causeForRecClosing;
} R4_Fin_SGSNLCORecord;

typedef struct R4_Fin_SGSNLCNRecord {
    unsigned int    bit_mask;
#       define      R4_Fin_SGSNLCNRecord_lcsClientType_present 0x80000000
#       define      R4_Fin_SGSNLCNRecord_lcsClientIdentity_present 0x40000000
#       define      R4_Fin_servedIMSI_present 0x20000000
#       define      R4_Fin_SGSNLCNRecord_servedMSISDN_present 0x10000000
#       define      R4_Fin_SGSNLCNRecord_sgsnAddress_present 0x08000000
#       define      R4_Fin_SGSNLCNRecord_servedIMEI_present 0x04000000
#       define      R4_Fin_SGSNLCNRecord_lcsQos_present 0x02000000
#       define      R4_Fin_SGSNLCNRecord_lcsPriority_present 0x01000000
#       define      R4_Fin_SGSNLCNRecord_mlcNumber_present 0x00800000
#       define      R4_Fin_SGSNLCNRecord_measurementDuration_present 0x00400000
#       define      R4_Fin_SGSNLCNRecord_location_present 0x00200000
#       define      R4_Fin_SGSNLCNRecord_routingArea_present 0x00100000
#       define      R4_Fin_SGSNLCNRecord_locationEstimate_present 0x00080000
#       define      R4_Fin_SGSNLCNRecord_lcsCause_present 0x00040000
#       define      R4_Fin_SGSNLCNRecord_diagnostics_present 0x00020000
#       define      R4_Fin_SGSNLCNRecord_nodeID_present 0x00010000
#       define      R4_Fin_SGSNLCNRecord_localSequenceNumber_present 0x00008000
#       define      R4_Fin_SGSNLCNRecord_chChSelectionMode_present 0x00004000
#       define      R4_Fin_SGSNLCNRecord_systemType_present 0x00002000
#       define      R4_Fin_SGSNLCNRecord_recordExtensions_present 0x00001000
    R4_Fin_CallEventRecordType recordType;
    R4_Fin_RecordingEntity recordingEntity;
    R4_Fin_LCSClientType lcsClientType;  /* optional; set in bit_mask
                                * R4_Fin_SGSNLCNRecord_lcsClientType_present if
                                * present */
    R4_Fin_LCSClientIdentity lcsClientIdentity;  /* optional; set in bit_mask
                            * R4_Fin_SGSNLCNRecord_lcsClientIdentity_present if
                            * present */
    R4_Fin_IMSI     servedIMSI;  /* optional; set in bit_mask
                                  * R4_Fin_servedIMSI_present if present */
    R4_Fin_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * R4_Fin_SGSNLCNRecord_servedMSISDN_present
                                    * if present */
    R4_Fin_GSNAddress sgsnAddress;  /* optional; set in bit_mask
                                     * R4_Fin_SGSNLCNRecord_sgsnAddress_present
                                     * if present */
    R4_Fin_IMEI     servedIMEI;  /* optional; set in bit_mask
                                  * R4_Fin_SGSNLCNRecord_servedIMEI_present if
                                  * present */
    R4_Fin_LCSQoSInfo lcsQos;  /* optional; set in bit_mask
                                * R4_Fin_SGSNLCNRecord_lcsQos_present if
                                * present */
    R4_Fin_LCS_Priority lcsPriority;  /* optional; set in bit_mask
                                  * R4_Fin_SGSNLCNRecord_lcsPriority_present if
                                  * present */
    R4_Fin_ISDN_AddressString mlcNumber;  /* optional; set in bit_mask
                                    * R4_Fin_SGSNLCNRecord_mlcNumber_present if
                                    * present */
    R4_Fin_TimeStamp eventTimeStamp;
    R4_Fin_CallDuration measurementDuration;  /* optional; set in bit_mask
                          * R4_Fin_SGSNLCNRecord_measurementDuration_present if
                          * present */
    R4_Fin_LocationAreaAndCell location;  /* optional; set in bit_mask
                                     * R4_Fin_SGSNLCNRecord_location_present if
                                     * present */
    R4_Fin_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                  * R4_Fin_SGSNLCNRecord_routingArea_present if
                                  * present */
    R4_Fin_Ext_GeographicalInformation locationEstimate;  /* optional; set in
                                   * bit_mask
                             * R4_Fin_SGSNLCNRecord_locationEstimate_present if
                             * present */
    R4_Fin_LCSCause lcsCause;  /* optional; set in bit_mask
                                * R4_Fin_SGSNLCNRecord_lcsCause_present if
                                * present */
    R4_Fin_Diagnostics diagnostics;  /* optional; set in bit_mask
                                      * R4_Fin_SGSNLCNRecord_diagnostics_present
                                      * if present */
    R4_Fin_NodeID   nodeID;  /* optional; set in bit_mask
                              * R4_Fin_SGSNLCNRecord_nodeID_present if
                              * present */
    R4_Fin_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                          * R4_Fin_SGSNLCNRecord_localSequenceNumber_present if
                          * present */
    R4_Fin_ChargingCharacteristics chargingCharacteristics;
    R4_Fin_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                            * R4_Fin_SGSNLCNRecord_chChSelectionMode_present if
                            * present */
    R4_Fin_SystemType systemType;  /* optional; set in bit_mask
                                    * R4_Fin_SGSNLCNRecord_systemType_present if
                                    * present */
    struct R4_Fin_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                             * R4_Fin_SGSNLCNRecord_recordExtensions_present if
                             * present */
    R4_Fin_CauseForRecClosing causeForRecClosing;
} R4_Fin_SGSNLCNRecord;

typedef struct R4_Fin_CallEventRecord {
    unsigned short  choice;
#       define      R4_Fin_sgsnPDPRecord_chosen 1
#       define      R4_Fin_ggsnPDPRecord_chosen 2
#       define      R4_Fin_sgsnMMRecord_chosen 3
#       define      R4_Fin_sgsnSMORecord_chosen 4
#       define      R4_Fin_sgsnSMTRecord_chosen 5
#       define      R4_Fin_sgsnLCTRecord_chosen 6
#       define      R4_Fin_sgsnLCORecord_chosen 7
#       define      R4_Fin_sgsnLCNRecord_chosen 8
    union _union {
        R4_Fin_SGSNPDPRecord sgsnPDPRecord;  /* to choose, set choice to
                                              * R4_Fin_sgsnPDPRecord_chosen */
        R4_Fin_GGSNPDPRecord ggsnPDPRecord;  /* to choose, set choice to
                                              * R4_Fin_ggsnPDPRecord_chosen */
        R4_Fin_SGSNMMRecord sgsnMMRecord;  /* to choose, set choice to
                                            * R4_Fin_sgsnMMRecord_chosen */
        R4_Fin_SGSNSMORecord sgsnSMORecord;  /* to choose, set choice to
                                              * R4_Fin_sgsnSMORecord_chosen */
        R4_Fin_SGSNSMTRecord sgsnSMTRecord;  /* to choose, set choice to
                                              * R4_Fin_sgsnSMTRecord_chosen */
        R4_Fin_SGSNLCTRecord sgsnLCTRecord;  /* to choose, set choice to
                                              * R4_Fin_sgsnLCTRecord_chosen */
        R4_Fin_SGSNLCORecord sgsnLCORecord;  /* to choose, set choice to
                                              * R4_Fin_sgsnLCORecord_chosen */
        R4_Fin_SGSNLCNRecord sgsnLCNRecord;  /* to choose, set choice to
                                              * R4_Fin_sgsnLCNRecord_chosen */
    } u;
} R4_Fin_CallEventRecord;

typedef struct R4_Fin_ManagementExtensions_ {
    struct R4_Fin_ManagementExtensions_ *next;
    R4_Fin_ManagementExtension value;
} *R4_Fin_ManagementExtensions;

typedef struct R4_Fin_Horizontal_Accuracy {
    unsigned short  length;
    unsigned char   value[1];
} R4_Fin_Horizontal_Accuracy;

typedef enum R4_Fin_ResponseTimeCategory {
    R4_Fin_lowdelay = 0,
    R4_Fin_delaytolerant = 1
} R4_Fin_ResponseTimeCategory;

typedef struct R4_Fin_ResponseTime {
    R4_Fin_ResponseTimeCategory responseTimeCategory;
} R4_Fin_ResponseTime;

typedef struct R4_Fin_Vertical_Accuracy {
    unsigned short  length;
    unsigned char   value[1];
} R4_Fin_Vertical_Accuracy;

extern int R4_Fin_maxAddressLength;

extern int R4_Fin_maxISDN_AddressLength;

extern int R4_Fin_maxExt_GeographicalInformation;


extern void *CDRF_R4_Fin;    /* encoder-decoder control table */
#ifdef __cplusplus
}	/* extern "C" */
#endif /* __cplusplus */
#endif /* OSS_CDRF_R4_Fin */
