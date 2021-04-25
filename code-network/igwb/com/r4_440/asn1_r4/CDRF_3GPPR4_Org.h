/********************************************************/
/* Copyright (C) 2004 OSS Nokalva.  All rights reserved.*/
/********************************************************/

/* THIS FILE IS PROPRIETARY MATERIAL OF OSS NOKALVA
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF OSS NOKALVA.
 * THIS FILE MAY NOT BE DISTRIBUTED. */

/* Generated for: HuaWei Technologies, Inc., Shenzhen, China - license 8088 for Windows NT */
/* Abstract syntax: CDRF_R4_Org */
/* Created: Fri Mar 05 11:03:35 2004 */
/* ASN.1 compiler version: 5.3 */
/* Target operating system: Windows NT/Windows 9x */
/* Target machine type: Intel x86 */
/* C compiler options required: -Zp4 (Microsoft) */
/* ASN.1 compiler options and file names specified:
 * -codefile CDRF_3GPPR4_Org.c -headerfile CDRF_3GPPR4_Org.h -errorfile
 * C:\WINDOWS\TEMP\2. -prefix R4_Org_ -nodebug -dualheader -ansi -verbose -ber
 * -root D:\application\ossasn1\win32\5.3.0\asn1dflt\asn1dflt.ms.zp4
 * D:\yutemp\lidufang\0305\R4\r4_org\CDRF_R4_Org.asn
 */

#ifndef OSS_CDRF_R4_Org
#define OSS_CDRF_R4_Org

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "asn1hdr.h"
#include "asn1code.h"

#define          R4_Org_CallEventRecord_PDU 1
#define          R4_Org_Horizontal_Accuracy_PDU 2
#define          R4_Org_ResponseTime_PDU 3
#define          R4_Org_Vertical_Accuracy_PDU 4

typedef struct R4_Org__ObjectID {
    unsigned short  length;
    unsigned char   *value;
} R4_Org__ObjectID;

typedef int             R4_Org_CallEventRecordType;
#define                     R4_Org_CallEventRecordType_sgsnPDPRecord 18
#define                     R4_Org_CallEventRecordType_ggsnPDPRecord 19
#define                     R4_Org_CallEventRecordType_sgsnMMRecord 20
#define                     R4_Org_CallEventRecordType_sgsnSMORecord 21
#define                     R4_Org_CallEventRecordType_sgsnSMTRecord 22
#define                     R4_Org_sgsnMtLCSRecord 26
#define                     R4_Org_sgsnMoLCSRecord 27
#define                     R4_Org_sgsnNiLCSRecord 28

typedef ossBoolean      R4_Org_NetworkInitiatedPDPContext;

typedef struct R4_Org_IMSI {
    unsigned short  length;
    unsigned char   value[8];
} R4_Org_IMSI;

typedef struct R4_Org_IMEI {
    unsigned short  length;
    unsigned char   value[8];
} R4_Org_IMEI;

typedef struct R4_Org_IPBinaryAddress {
    unsigned short  choice;
#       define      R4_Org_iPBinV4Address_chosen 1
#       define      R4_Org_iPBinV6Address_chosen 2
    union _union {
        struct R4_Org__octet1 {
            unsigned short  length;
            unsigned char   value[4];
        } iPBinV4Address;  /* to choose, set choice to
                            * R4_Org_iPBinV4Address_chosen */
        struct R4_Org__octet2 {
            unsigned short  length;
            unsigned char   value[16];
        } iPBinV6Address;  /* to choose, set choice to
                            * R4_Org_iPBinV6Address_chosen */
    } u;
} R4_Org_IPBinaryAddress;

typedef struct R4_Org_IPTextRepresentedAddress {
    unsigned short  choice;
#       define      R4_Org_iPTextV4Address_chosen 1
#       define      R4_Org_iPTextV6Address_chosen 2
    union _union {
        char            iPTextV4Address[16];  /* to choose, set choice to
                                             * R4_Org_iPTextV4Address_chosen */
        char            iPTextV6Address[46];  /* to choose, set choice to
                                             * R4_Org_iPTextV6Address_chosen */
    } u;
} R4_Org_IPTextRepresentedAddress;

typedef struct R4_Org_IPAddress {
    unsigned short  choice;
#       define      R4_Org_iPBinaryAddress_chosen 1
#       define      R4_Org_iPTextRepresentedAddress_chosen 2
    union _union {
        R4_Org_IPBinaryAddress iPBinaryAddress;  /* to choose, set choice to
                                             * R4_Org_iPBinaryAddress_chosen */
        R4_Org_IPTextRepresentedAddress iPTextRepresentedAddress;  /* to choose,
                                   * set choice to
                                   * R4_Org_iPTextRepresentedAddress_chosen */
    } u;
} R4_Org_IPAddress;

typedef R4_Org_IPAddress R4_Org_GSNAddress;

typedef struct R4_Org_MSNetworkCapability {
    unsigned short  length;
    unsigned char   value[8];
} R4_Org_MSNetworkCapability;

typedef struct R4_Org_RoutingAreaCode {
    unsigned short  length;
    unsigned char   value[1];
} R4_Org_RoutingAreaCode;

typedef struct R4_Org_LocationAreaCode {
    unsigned short  length;
    unsigned char   value[2];
} R4_Org_LocationAreaCode;

typedef struct R4_Org_CellId {
    unsigned short  length;
    unsigned char   value[2];
} R4_Org_CellId;

typedef unsigned int    R4_Org_ChargingID;

typedef char            R4_Org_AccessPointNameNI[64];

typedef struct R4_Org_PDPType {
    unsigned short  length;
    unsigned char   value[2];
} R4_Org_PDPType;

typedef struct R4_Org_AddressString {
    unsigned short  length;
    unsigned char   value[20];
} R4_Org_AddressString;

typedef R4_Org_AddressString R4_Org_ETSIAddress;

typedef struct R4_Org_PDPAddress {
    unsigned short  choice;
#       define      R4_Org_iPAddress_chosen 1
#       define      R4_Org_eTSIAddress_chosen 2
    union _union {
        R4_Org_IPAddress iPAddress;  /* to choose, set choice to
                                      * R4_Org_iPAddress_chosen */
        R4_Org_ETSIAddress eTSIAddress;  /* to choose, set choice to
                                          * R4_Org_eTSIAddress_chosen */
    } u;
} R4_Org_PDPAddress;

typedef struct R4_Org_QoSInformation {
    unsigned short  length;
    unsigned char   value[12];
} R4_Org_QoSInformation;

typedef int             R4_Org_DataVolumeGPRS;

typedef enum R4_Org_ChangeCondition {
    R4_Org_qoSChange = 0,
    R4_Org_tariffTime = 1,
    R4_Org_recordClosure = 2
} R4_Org_ChangeCondition;

typedef struct R4_Org_TimeStamp {
    unsigned short  length;
    unsigned char   value[9];
} R4_Org_TimeStamp;

typedef struct R4_Org_ChangeOfCharCondition {
    unsigned char   bit_mask;
#       define      R4_Org_qosRequested_present 0x80
#       define      R4_Org_qosNegotiated_present 0x40
    R4_Org_QoSInformation qosRequested;  /* optional; set in bit_mask
                                          * R4_Org_qosRequested_present if
                                          * present */
    R4_Org_QoSInformation qosNegotiated;  /* optional; set in bit_mask
                                           * R4_Org_qosNegotiated_present if
                                           * present */
    R4_Org_DataVolumeGPRS dataVolumeGPRSUplink;
    R4_Org_DataVolumeGPRS dataVolumeGPRSDownlink;
    R4_Org_ChangeCondition changeCondition;
    R4_Org_TimeStamp changeTime;
} R4_Org_ChangeOfCharCondition;

typedef int             R4_Org_CallDuration;

typedef ossBoolean      R4_Org_SGSNChange;

typedef int             R4_Org_CauseForRecClosing;
#define                     R4_Org_normalRelease 0
#define                     R4_Org_abnormalRelease 4
#define                     R4_Org_cAMELInitCallRelease 5
#define                     R4_Org_volumeLimit 16
#define                     R4_Org_timeLimit 17
#define                     R4_Org_sGSNChange 18
#define                     R4_Org_maxChangeCond 19
#define                     R4_Org_managementIntervention 20
#define                     R4_Org_intraSGSNIntersystemChange 21
#define                     R4_Org_unauthorizedRequestingNetwork 52
#define                     R4_Org_unauthorizedLCSClient 53
#define                     R4_Org_positionMethodFailure 54
#define                     R4_Org_unknownOrUnreachableLCSClient 58

typedef struct R4_Org_ManagementExtension {
    unsigned char   bit_mask;
#       define      R4_Org_significance_present 0x80
    R4_Org__ObjectID identifier;
    ossBoolean      significance;  /* R4_Org_significance_present not set in
                                    * bit_mask implies value is FALSE */
    struct R4_Org__octet3 {
        unsigned int    length;
        unsigned char   *value;
    } information;
} R4_Org_ManagementExtension;

typedef enum R4_Org_PositionMethodFailure_Diagnostic {
    R4_Org_congestion = 0,
    R4_Org_insufficientResources = 1,
    R4_Org_insufficientMeasurementData = 2,
    R4_Org_inconsistentMeasurementData = 3,
    R4_Org_locationProcedureNotCompleted = 4,
    R4_Org_locationProcedureNotSupportedByTargetMS = 5,
    R4_Org_qoSNotAttainable = 6,
    R4_Org_positionMethodNotAvailableInNetwork = 7,
    R4_Org_positionMethodNotAvailableInLocationArea = 8
} R4_Org_PositionMethodFailure_Diagnostic;

typedef enum R4_Org_UnauthorizedLCSClient_Diagnostic {
    R4_Org_noAdditionalInformation = 0,
    R4_Org_clientNotInMSPrivacyExceptionList = 1,
    R4_Org_callToClientNotSetup = 2,
    R4_Org_privacyOverrideNotApplicable = 3,
    R4_Org_disallowedByLocalRegulatoryRequirements = 4
} R4_Org_UnauthorizedLCSClient_Diagnostic;

typedef struct R4_Org_Diagnostics {
    unsigned short  choice;
#       define      R4_Org_gsm0408Cause_chosen 1
#       define      R4_Org_gsm0902MapErrorValue_chosen 2
#       define      R4_Org_ccittQ767Cause_chosen 3
#       define      R4_Org_networkSpecificCause_chosen 4
#       define      R4_Org_manufacturerSpecificCause_chosen 5
#       define      R4_Org_positionMethodFailureCause_chosen 6
#       define      R4_Org_unauthorizedLCSClientCause_chosen 7
    union _union {
        int             gsm0408Cause;  /* to choose, set choice to
                                        * R4_Org_gsm0408Cause_chosen */
        int             gsm0902MapErrorValue;  /* to choose, set choice to
                                        * R4_Org_gsm0902MapErrorValue_chosen */
        int             ccittQ767Cause;  /* to choose, set choice to
                                          * R4_Org_ccittQ767Cause_chosen */
        R4_Org_ManagementExtension networkSpecificCause;  /* to choose, set
                                   * choice to
                                   * R4_Org_networkSpecificCause_chosen */
        R4_Org_ManagementExtension manufacturerSpecificCause;  /* to choose, set
                                   * choice to
                                   * R4_Org_manufacturerSpecificCause_chosen */
        R4_Org_PositionMethodFailure_Diagnostic positionMethodFailureCause;  
                                        /* to choose, set choice to
                                  * R4_Org_positionMethodFailureCause_chosen */
        R4_Org_UnauthorizedLCSClient_Diagnostic unauthorizedLCSClientCause;  
                                        /* to choose, set choice to
                                  * R4_Org_unauthorizedLCSClientCause_chosen */
    } u;
} R4_Org_Diagnostics;

typedef char            R4_Org_NodeID[21];

typedef unsigned int    R4_Org_LocalSequenceNumber;

typedef enum R4_Org_APNSelectionMode {
    R4_Org_mSorNetworkProvidedSubscriptionVerified = 0,
    R4_Org_mSProvidedSubscriptionNotVerified = 1,
    R4_Org_networkProvidedSubscriptionNotVerified = 2
} R4_Org_APNSelectionMode;

typedef char            R4_Org_AccessPointNameOI[38];

typedef struct R4_Org_ISDN_AddressString {
    unsigned short  length;
    unsigned char   value[9];
} R4_Org_ISDN_AddressString;

typedef R4_Org_ISDN_AddressString R4_Org_MSISDN;

typedef struct R4_Org_ChargingCharacteristics {
    unsigned short  length;
    unsigned char   value[2];
} R4_Org_ChargingCharacteristics;

typedef enum R4_Org_SystemType {
    R4_Org_unknown = 0,
    R4_Org_iuUTRAN = 1,
    R4_Org_gERAN = 2
} R4_Org_SystemType;

typedef R4_Org_AddressString R4_Org_SCFAddress;

typedef unsigned int    R4_Org_ServiceKey;

typedef enum R4_Org_DefaultGPRS_Handling {
    R4_Org_DefaultGPRS_Handling_continueTransaction = 0,
    R4_Org_DefaultGPRS_Handling_releaseTransaction = 1
} R4_Org_DefaultGPRS_Handling;

typedef R4_Org_AccessPointNameNI R4_Org_CAMELAccessPointNameNI;

typedef R4_Org_AccessPointNameOI R4_Org_CAMELAccessPointNameOI;

typedef int             R4_Org_NumberOfDPEncountered;

typedef unsigned char   R4_Org_LevelOfCAMELService;
#define                     R4_Org_basic 0x80
#define                     R4_Org_callDurationSupervision 0x40
#define                     R4_Org_onlineCharging 0x20

typedef struct R4_Org_FreeFormatData {
    unsigned short  length;
    unsigned char   value[160];
} R4_Org_FreeFormatData;

typedef ossBoolean      R4_Org_FFDAppendIndicator;

typedef struct R4_Org_CAMELInformationPDP {
    unsigned short  bit_mask;
#       define      R4_Org_CAMELInformationPDP_sCFAddress_present 0x8000
#       define      R4_Org_CAMELInformationPDP_serviceKey_present 0x4000
#       define      R4_Org_CAMELInformationPDP_defaultTransactionHandling_present 0x2000
#       define      R4_Org_cAMELAccessPointNameNI_present 0x1000
#       define      R4_Org_cAMELAccessPointNameOI_present 0x0800
#       define      R4_Org_CAMELInformationPDP_numberOfDPEncountered_present 0x0400
#       define      R4_Org_CAMELInformationPDP_levelOfCAMELService_present 0x0200
#       define      R4_Org_CAMELInformationPDP_freeFormatData_present 0x0100
#       define      R4_Org_CAMELInformationPDP_fFDAppendIndicator_present 0x0080
    R4_Org_SCFAddress sCFAddress;  /* optional; set in bit_mask
                             * R4_Org_CAMELInformationPDP_sCFAddress_present if
                             * present */
    R4_Org_ServiceKey serviceKey;  /* optional; set in bit_mask
                             * R4_Org_CAMELInformationPDP_serviceKey_present if
                             * present */
    R4_Org_DefaultGPRS_Handling defaultTransactionHandling;  /* optional; set in
                                   * bit_mask
             * R4_Org_CAMELInformationPDP_defaultTransactionHandling_present if
             * present */
    R4_Org_CAMELAccessPointNameNI cAMELAccessPointNameNI;  /* optional; set in
                                   * bit_mask
                                   * R4_Org_cAMELAccessPointNameNI_present if
                                   * present */
    R4_Org_CAMELAccessPointNameOI cAMELAccessPointNameOI;  /* optional; set in
                                   * bit_mask
                                   * R4_Org_cAMELAccessPointNameOI_present if
                                   * present */
    R4_Org_NumberOfDPEncountered numberOfDPEncountered;  /* optional; set in
                                   * bit_mask
                  * R4_Org_CAMELInformationPDP_numberOfDPEncountered_present if
                  * present */
    R4_Org_LevelOfCAMELService levelOfCAMELService;  /* optional; set in
                                   * bit_mask
                    * R4_Org_CAMELInformationPDP_levelOfCAMELService_present if
                    * present */
    R4_Org_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                         * R4_Org_CAMELInformationPDP_freeFormatData_present if
                         * present */
    R4_Org_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in bit_mask
                     * R4_Org_CAMELInformationPDP_fFDAppendIndicator_present if
                     * present */
} R4_Org_CAMELInformationPDP;

typedef enum R4_Org_ChChSelectionMode {
    R4_Org_sGSNSupplied = 0,
    R4_Org_subscriptionSpecific = 1,
    R4_Org_aPNSpecific = 2,
    R4_Org_homeDefault = 3,
    R4_Org_roamingDefault = 4,
    R4_Org_visitingDefault = 5
} R4_Org_ChChSelectionMode;

typedef ossBoolean      R4_Org_DynamicAddressFlag;

typedef struct R4_Org__seqof1_ {
    struct R4_Org__seqof1_ *next;
    R4_Org_ChangeOfCharCondition value;
} *R4_Org__seqof1;

typedef struct R4_Org_SGSNPDPRecord {
    unsigned int    bit_mask;
#       define      R4_Org_SGSNPDPRecord_networkInitiation_present 0x80000000
#       define      R4_Org_SGSNPDPRecord_servedIMEI_present 0x40000000
#       define      R4_Org_SGSNPDPRecord_sgsnAddress_present 0x20000000
#       define      R4_Org_SGSNPDPRecord_msNetworkCapability_present 0x10000000
#       define      R4_Org_SGSNPDPRecord_routingArea_present 0x08000000
#       define      R4_Org_SGSNPDPRecord_locationAreaCode_present 0x04000000
#       define      R4_Org_SGSNPDPRecord_cellIdentifier_present 0x02000000
#       define      R4_Org_SGSNPDPRecord_accessPointNameNI_present 0x01000000
#       define      R4_Org_SGSNPDPRecord_pdpType_present 0x00800000
#       define      R4_Org_SGSNPDPRecord_servedPDPAddress_present 0x00400000
#       define      R4_Org_SGSNPDPRecord_listOfTrafficVolumes_present 0x00200000
#       define      R4_Org_SGSNPDPRecord_sgsnChange_present 0x00100000
#       define      R4_Org_SGSNPDPRecord_diagnostics_present 0x00080000
#       define      R4_Org_SGSNPDPRecord_recordSequenceNumber_present 0x00040000
#       define      R4_Org_SGSNPDPRecord_nodeID_present 0x00020000
#       define      R4_Org_SGSNPDPRecord_recordExtensions_present 0x00010000
#       define      R4_Org_SGSNPDPRecord_localSequenceNumber_present 0x00008000
#       define      R4_Org_SGSNPDPRecord_apnSelectionMode_present 0x00004000
#       define      R4_Org_accessPointNameOI_present 0x00002000
#       define      R4_Org_SGSNPDPRecord_servedMSISDN_present 0x00001000
#       define      R4_Org_SGSNPDPRecord_systemType_present 0x00000800
#       define      R4_Org_cAMELInformationPDP_present 0x00000400
#       define      R4_Org_rNCUnsentDownlinkVolume_present 0x00000200
#       define      R4_Org_SGSNPDPRecord_chChSelectionMode_present 0x00000100
#       define      R4_Org_SGSNPDPRecord_dynamicAddressFlag_present 0x00000080
    R4_Org_CallEventRecordType recordType;
    R4_Org_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                            * R4_Org_SGSNPDPRecord_networkInitiation_present if
                            * present */
    R4_Org_IMSI     servedIMSI;
    R4_Org_IMEI     servedIMEI;  /* optional; set in bit_mask
                                  * R4_Org_SGSNPDPRecord_servedIMEI_present if
                                  * present */
    R4_Org_GSNAddress sgsnAddress;  /* optional; set in bit_mask
                                     * R4_Org_SGSNPDPRecord_sgsnAddress_present
                                     * if present */
    R4_Org_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                          * R4_Org_SGSNPDPRecord_msNetworkCapability_present if
                          * present */
    R4_Org_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                  * R4_Org_SGSNPDPRecord_routingArea_present if
                                  * present */
    R4_Org_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                             * R4_Org_SGSNPDPRecord_locationAreaCode_present if
                             * present */
    R4_Org_CellId   cellIdentifier;  /* optional; set in bit_mask
                               * R4_Org_SGSNPDPRecord_cellIdentifier_present if
                               * present */
    R4_Org_ChargingID chargingID;
    R4_Org_GSNAddress ggsnAddressUsed;
    R4_Org_AccessPointNameNI accessPointNameNI;  /* optional; set in bit_mask
                            * R4_Org_SGSNPDPRecord_accessPointNameNI_present if
                            * present */
    R4_Org_PDPType  pdpType;  /* optional; set in bit_mask
                               * R4_Org_SGSNPDPRecord_pdpType_present if
                               * present */
    R4_Org_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                             * R4_Org_SGSNPDPRecord_servedPDPAddress_present if
                             * present */
    struct R4_Org__seqof1_ *listOfTrafficVolumes;  /* optional; set in bit_mask
                         * R4_Org_SGSNPDPRecord_listOfTrafficVolumes_present if
                         * present */
    R4_Org_TimeStamp recordOpeningTime;
    R4_Org_CallDuration duration;
    R4_Org_SGSNChange sgsnChange;  /* optional; set in bit_mask
                                    * R4_Org_SGSNPDPRecord_sgsnChange_present if
                                    * present */
    R4_Org_CauseForRecClosing causeForRecClosing;
    R4_Org_Diagnostics diagnostics;  /* optional; set in bit_mask
                                      * R4_Org_SGSNPDPRecord_diagnostics_present
                                      * if present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                         * R4_Org_SGSNPDPRecord_recordSequenceNumber_present if
                         * present */
    R4_Org_NodeID   nodeID;  /* optional; set in bit_mask
                              * R4_Org_SGSNPDPRecord_nodeID_present if
                              * present */
    struct R4_Org_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                             * R4_Org_SGSNPDPRecord_recordExtensions_present if
                             * present */
    R4_Org_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                          * R4_Org_SGSNPDPRecord_localSequenceNumber_present if
                          * present */
    R4_Org_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                             * R4_Org_SGSNPDPRecord_apnSelectionMode_present if
                             * present */
    R4_Org_AccessPointNameOI accessPointNameOI;  /* optional; set in bit_mask
                                          * R4_Org_accessPointNameOI_present if
                                          * present */
    R4_Org_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * R4_Org_SGSNPDPRecord_servedMSISDN_present
                                    * if present */
    R4_Org_ChargingCharacteristics chargingCharacteristics;
    R4_Org_SystemType systemType;  /* optional; set in bit_mask
                                    * R4_Org_SGSNPDPRecord_systemType_present if
                                    * present */
    R4_Org_CAMELInformationPDP cAMELInformationPDP;  /* optional; set in
                                   * bit_mask R4_Org_cAMELInformationPDP_present
                                   * if present */
    R4_Org_DataVolumeGPRS rNCUnsentDownlinkVolume;  /* optional; set in bit_mask
                                    * R4_Org_rNCUnsentDownlinkVolume_present if
                                    * present */
    R4_Org_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                            * R4_Org_SGSNPDPRecord_chChSelectionMode_present if
                            * present */
    R4_Org_DynamicAddressFlag dynamicAddressFlag;  /* optional; set in bit_mask
                           * R4_Org_SGSNPDPRecord_dynamicAddressFlag_present if
                           * present */
} R4_Org_SGSNPDPRecord;

typedef struct R4_Org_PLMN_Id {
    unsigned short  length;
    unsigned char   value[3];
} R4_Org_PLMN_Id;

typedef struct R4_Org_GGSNPDPRecord {
    unsigned short  bit_mask;
#       define      R4_Org_GGSNPDPRecord_networkInitiation_present 0x8000
#       define      R4_Org_GGSNPDPRecord_accessPointNameNI_present 0x4000
#       define      R4_Org_GGSNPDPRecord_pdpType_present 0x2000
#       define      R4_Org_GGSNPDPRecord_servedPDPAddress_present 0x1000
#       define      R4_Org_GGSNPDPRecord_dynamicAddressFlag_present 0x0800
#       define      R4_Org_GGSNPDPRecord_listOfTrafficVolumes_present 0x0400
#       define      R4_Org_GGSNPDPRecord_diagnostics_present 0x0200
#       define      R4_Org_GGSNPDPRecord_recordSequenceNumber_present 0x0100
#       define      R4_Org_GGSNPDPRecord_nodeID_present 0x0080
#       define      R4_Org_GGSNPDPRecord_recordExtensions_present 0x0040
#       define      R4_Org_GGSNPDPRecord_localSequenceNumber_present 0x0020
#       define      R4_Org_GGSNPDPRecord_apnSelectionMode_present 0x0010
#       define      R4_Org_GGSNPDPRecord_servedMSISDN_present 0x0008
#       define      R4_Org_GGSNPDPRecord_chChSelectionMode_present 0x0004
#       define      R4_Org_sgsnPLMNIdentifier_present 0x0002
    R4_Org_CallEventRecordType recordType;
    R4_Org_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                            * R4_Org_GGSNPDPRecord_networkInitiation_present if
                            * present */
    R4_Org_IMSI     servedIMSI;
    R4_Org_GSNAddress ggsnAddress;
    R4_Org_ChargingID chargingID;
    struct R4_Org__seqof2 {
        struct R4_Org__seqof2 *next;
        R4_Org_GSNAddress value;
    } *sgsnAddress;
    R4_Org_AccessPointNameNI accessPointNameNI;  /* optional; set in bit_mask
                            * R4_Org_GGSNPDPRecord_accessPointNameNI_present if
                            * present */
    R4_Org_PDPType  pdpType;  /* optional; set in bit_mask
                               * R4_Org_GGSNPDPRecord_pdpType_present if
                               * present */
    R4_Org_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                             * R4_Org_GGSNPDPRecord_servedPDPAddress_present if
                             * present */
    R4_Org_DynamicAddressFlag dynamicAddressFlag;  /* optional; set in bit_mask
                           * R4_Org_GGSNPDPRecord_dynamicAddressFlag_present if
                           * present */
    struct R4_Org__seqof1_ *listOfTrafficVolumes;  /* optional; set in bit_mask
                         * R4_Org_GGSNPDPRecord_listOfTrafficVolumes_present if
                         * present */
    R4_Org_TimeStamp recordOpeningTime;
    R4_Org_CallDuration duration;
    R4_Org_CauseForRecClosing causeForRecClosing;
    R4_Org_Diagnostics diagnostics;  /* optional; set in bit_mask
                                      * R4_Org_GGSNPDPRecord_diagnostics_present
                                      * if present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                         * R4_Org_GGSNPDPRecord_recordSequenceNumber_present if
                         * present */
    R4_Org_NodeID   nodeID;  /* optional; set in bit_mask
                              * R4_Org_GGSNPDPRecord_nodeID_present if
                              * present */
    struct R4_Org_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                             * R4_Org_GGSNPDPRecord_recordExtensions_present if
                             * present */
    R4_Org_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                          * R4_Org_GGSNPDPRecord_localSequenceNumber_present if
                          * present */
    R4_Org_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                             * R4_Org_GGSNPDPRecord_apnSelectionMode_present if
                             * present */
    R4_Org_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * R4_Org_GGSNPDPRecord_servedMSISDN_present
                                    * if present */
    R4_Org_ChargingCharacteristics chargingCharacteristics;
    R4_Org_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                            * R4_Org_GGSNPDPRecord_chChSelectionMode_present if
                            * present */
    R4_Org_PLMN_Id  sgsnPLMNIdentifier;  /* optional; set in bit_mask
                                          * R4_Org_sgsnPLMNIdentifier_present if
                                          * present */
} R4_Org_GGSNPDPRecord;

typedef struct R4_Org_ChangeLocation {
    unsigned char   bit_mask;
#       define      R4_Org_cellId_present 0x80
    R4_Org_LocationAreaCode locationAreaCode;
    R4_Org_RoutingAreaCode routingAreaCode;
    R4_Org_CellId   cellId;  /* optional; set in bit_mask R4_Org_cellId_present
                              * if present */
    R4_Org_TimeStamp changeTime;
} R4_Org_ChangeLocation;

typedef struct R4_Org_CAMELInformationMM {
    unsigned char   bit_mask;
#       define      R4_Org_CAMELInformationMM_sCFAddress_present 0x80
#       define      R4_Org_CAMELInformationMM_serviceKey_present 0x40
#       define      R4_Org_CAMELInformationMM_defaultTransactionHandling_present 0x20
#       define      R4_Org_CAMELInformationMM_numberOfDPEncountered_present 0x10
#       define      R4_Org_CAMELInformationMM_levelOfCAMELService_present 0x08
#       define      R4_Org_CAMELInformationMM_freeFormatData_present 0x04
#       define      R4_Org_CAMELInformationMM_fFDAppendIndicator_present 0x02
    R4_Org_SCFAddress sCFAddress;  /* optional; set in bit_mask
                              * R4_Org_CAMELInformationMM_sCFAddress_present if
                              * present */
    R4_Org_ServiceKey serviceKey;  /* optional; set in bit_mask
                              * R4_Org_CAMELInformationMM_serviceKey_present if
                              * present */
    R4_Org_DefaultGPRS_Handling defaultTransactionHandling;  /* optional; set in
                                   * bit_mask
              * R4_Org_CAMELInformationMM_defaultTransactionHandling_present if
              * present */
    R4_Org_NumberOfDPEncountered numberOfDPEncountered;  /* optional; set in
                                   * bit_mask
                   * R4_Org_CAMELInformationMM_numberOfDPEncountered_present if
                   * present */
    R4_Org_LevelOfCAMELService levelOfCAMELService;  /* optional; set in
                                   * bit_mask
                     * R4_Org_CAMELInformationMM_levelOfCAMELService_present if
                     * present */
    R4_Org_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                          * R4_Org_CAMELInformationMM_freeFormatData_present if
                          * present */
    R4_Org_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in bit_mask
                      * R4_Org_CAMELInformationMM_fFDAppendIndicator_present if
                      * present */
} R4_Org_CAMELInformationMM;

typedef struct R4_Org_SGSNMMRecord {
    unsigned int    bit_mask;
#       define      R4_Org_SGSNMMRecord_servedIMEI_present 0x80000000
#       define      R4_Org_SGSNMMRecord_sgsnAddress_present 0x40000000
#       define      R4_Org_SGSNMMRecord_msNetworkCapability_present 0x20000000
#       define      R4_Org_SGSNMMRecord_routingArea_present 0x10000000
#       define      R4_Org_SGSNMMRecord_locationAreaCode_present 0x08000000
#       define      R4_Org_SGSNMMRecord_cellIdentifier_present 0x04000000
#       define      R4_Org_changeLocation_present 0x02000000
#       define      R4_Org_duration_present 0x01000000
#       define      R4_Org_SGSNMMRecord_sgsnChange_present 0x00800000
#       define      R4_Org_SGSNMMRecord_diagnostics_present 0x00400000
#       define      R4_Org_SGSNMMRecord_recordSequenceNumber_present 0x00200000
#       define      R4_Org_SGSNMMRecord_nodeID_present 0x00100000
#       define      R4_Org_SGSNMMRecord_recordExtensions_present 0x00080000
#       define      R4_Org_SGSNMMRecord_localSequenceNumber_present 0x00040000
#       define      R4_Org_SGSNMMRecord_servedMSISDN_present 0x00020000
#       define      R4_Org_cAMELInformationMM_present 0x00010000
#       define      R4_Org_SGSNMMRecord_systemType_present 0x00008000
#       define      R4_Org_SGSNMMRecord_chChSelectionMode_present 0x00004000
    R4_Org_CallEventRecordType recordType;
    R4_Org_IMSI     servedIMSI;
    R4_Org_IMEI     servedIMEI;  /* optional; set in bit_mask
                                  * R4_Org_SGSNMMRecord_servedIMEI_present if
                                  * present */
    R4_Org_GSNAddress sgsnAddress;  /* optional; set in bit_mask
                                     * R4_Org_SGSNMMRecord_sgsnAddress_present
                                     * if present */
    R4_Org_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                           * R4_Org_SGSNMMRecord_msNetworkCapability_present if
                           * present */
    R4_Org_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                   * R4_Org_SGSNMMRecord_routingArea_present if
                                   * present */
    R4_Org_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                              * R4_Org_SGSNMMRecord_locationAreaCode_present if
                              * present */
    R4_Org_CellId   cellIdentifier;  /* optional; set in bit_mask
                                * R4_Org_SGSNMMRecord_cellIdentifier_present if
                                * present */
    struct R4_Org__seqof3 {
        struct R4_Org__seqof3 *next;
        R4_Org_ChangeLocation value;
    } *changeLocation;  /* optional; set in bit_mask
                         * R4_Org_changeLocation_present if present */
    R4_Org_TimeStamp recordOpeningTime;
    R4_Org_CallDuration duration;  /* optional; set in bit_mask
                                    * R4_Org_duration_present if present */
    R4_Org_SGSNChange sgsnChange;  /* optional; set in bit_mask
                                    * R4_Org_SGSNMMRecord_sgsnChange_present if
                                    * present */
    R4_Org_CauseForRecClosing causeForRecClosing;
    R4_Org_Diagnostics diagnostics;  /* optional; set in bit_mask
                                      * R4_Org_SGSNMMRecord_diagnostics_present
                                      * if present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                          * R4_Org_SGSNMMRecord_recordSequenceNumber_present if
                          * present */
    R4_Org_NodeID   nodeID;  /* optional; set in bit_mask
                              * R4_Org_SGSNMMRecord_nodeID_present if present */
    struct R4_Org_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                              * R4_Org_SGSNMMRecord_recordExtensions_present if
                              * present */
    R4_Org_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                           * R4_Org_SGSNMMRecord_localSequenceNumber_present if
                           * present */
    R4_Org_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * R4_Org_SGSNMMRecord_servedMSISDN_present
                                    * if present */
    R4_Org_ChargingCharacteristics chargingCharacteristics;
    R4_Org_CAMELInformationMM cAMELInformationMM;  /* optional; set in bit_mask
                                         * R4_Org_cAMELInformationMM_present if
                                         * present */
    R4_Org_SystemType systemType;  /* optional; set in bit_mask
                                    * R4_Org_SGSNMMRecord_systemType_present if
                                    * present */
    R4_Org_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                             * R4_Org_SGSNMMRecord_chChSelectionMode_present if
                             * present */
} R4_Org_SGSNMMRecord;

typedef R4_Org_AddressString R4_Org_RecordingEntity;

typedef struct R4_Org_MessageReference {
    unsigned int    length;
    unsigned char   *value;
} R4_Org_MessageReference;

typedef R4_Org_Diagnostics R4_Org_SMSResult;

typedef struct R4_Org_CalledNumber {
    unsigned short  length;
    unsigned char   value[12];
} R4_Org_CalledNumber;

typedef enum R4_Org_DefaultSMS_Handling {
    R4_Org_DefaultSMS_Handling_continueTransaction = 0,
    R4_Org_DefaultSMS_Handling_releaseTransaction = 1
} R4_Org_DefaultSMS_Handling;

typedef struct R4_Org_CallingNumber {
    unsigned short  length;
    unsigned char   value[10];
} R4_Org_CallingNumber;

typedef struct R4_Org_CallReferenceNumber {
    unsigned short  length;
    unsigned char   value[8];
} R4_Org_CallReferenceNumber;

typedef struct R4_Org_CAMELInformationSMS {
    unsigned char   bit_mask;
#       define      R4_Org_CAMELInformationSMS_sCFAddress_present 0x80
#       define      R4_Org_CAMELInformationSMS_serviceKey_present 0x40
#       define      R4_Org_defaultSMSHandling_present 0x20
#       define      R4_Org_cAMELCallingPartyNumber_present 0x10
#       define      R4_Org_cAMELDestinationSubscriberNumber_present 0x08
#       define      R4_Org_cAMELSMSCAddress_present 0x04
#       define      R4_Org_CAMELInformationSMS_freeFormatData_present 0x02
#       define      R4_Org_smsReferenceNumber_present 0x01
    R4_Org_SCFAddress sCFAddress;  /* optional; set in bit_mask
                             * R4_Org_CAMELInformationSMS_sCFAddress_present if
                             * present */
    R4_Org_ServiceKey serviceKey;  /* optional; set in bit_mask
                             * R4_Org_CAMELInformationSMS_serviceKey_present if
                             * present */
    R4_Org_DefaultSMS_Handling defaultSMSHandling;  /* optional; set in bit_mask
                                         * R4_Org_defaultSMSHandling_present if
                                         * present */
    R4_Org_CallingNumber cAMELCallingPartyNumber;  /* optional; set in bit_mask
                                    * R4_Org_cAMELCallingPartyNumber_present if
                                    * present */
    R4_Org_CalledNumber cAMELDestinationSubscriberNumber;  /* optional; set in
                                   * bit_mask
                           * R4_Org_cAMELDestinationSubscriberNumber_present if
                           * present */
    R4_Org_AddressString cAMELSMSCAddress;  /* optional; set in bit_mask
                                             * R4_Org_cAMELSMSCAddress_present
                                             * if present */
    R4_Org_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                         * R4_Org_CAMELInformationSMS_freeFormatData_present if
                         * present */
    R4_Org_CallReferenceNumber smsReferenceNumber;  /* optional; set in bit_mask
                                         * R4_Org_smsReferenceNumber_present if
                                         * present */
} R4_Org_CAMELInformationSMS;

typedef struct R4_Org_SGSNSMORecord {
    unsigned short  bit_mask;
#       define      R4_Org_SGSNSMORecord_servedIMEI_present 0x8000
#       define      R4_Org_SGSNSMORecord_servedMSISDN_present 0x4000
#       define      R4_Org_SGSNSMORecord_msNetworkCapability_present 0x2000
#       define      R4_Org_SGSNSMORecord_serviceCentre_present 0x1000
#       define      R4_Org_SGSNSMORecord_recordingEntity_present 0x0800
#       define      R4_Org_SGSNSMORecord_locationArea_present 0x0400
#       define      R4_Org_SGSNSMORecord_routingArea_present 0x0200
#       define      R4_Org_SGSNSMORecord_cellIdentifier_present 0x0100
#       define      R4_Org_SGSNSMORecord_smsResult_present 0x0080
#       define      R4_Org_SGSNSMORecord_recordExtensions_present 0x0040
#       define      R4_Org_SGSNSMORecord_nodeID_present 0x0020
#       define      R4_Org_SGSNSMORecord_localSequenceNumber_present 0x0010
#       define      R4_Org_SGSNSMORecord_systemType_present 0x0008
#       define      R4_Org_destinationNumber_present 0x0004
#       define      R4_Org_SGSNSMORecord_cAMELInformationSMS_present 0x0002
#       define      R4_Org_SGSNSMORecord_chChSelectionMode_present 0x0001
    R4_Org_CallEventRecordType recordType;
    R4_Org_IMSI     servedIMSI;
    R4_Org_IMEI     servedIMEI;  /* optional; set in bit_mask
                                  * R4_Org_SGSNSMORecord_servedIMEI_present if
                                  * present */
    R4_Org_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * R4_Org_SGSNSMORecord_servedMSISDN_present
                                    * if present */
    R4_Org_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                          * R4_Org_SGSNSMORecord_msNetworkCapability_present if
                          * present */
    R4_Org_AddressString serviceCentre;  /* optional; set in bit_mask
                                * R4_Org_SGSNSMORecord_serviceCentre_present if
                                * present */
    R4_Org_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                              * R4_Org_SGSNSMORecord_recordingEntity_present if
                              * present */
    R4_Org_LocationAreaCode locationArea;  /* optional; set in bit_mask
                                 * R4_Org_SGSNSMORecord_locationArea_present if
                                 * present */
    R4_Org_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                  * R4_Org_SGSNSMORecord_routingArea_present if
                                  * present */
    R4_Org_CellId   cellIdentifier;  /* optional; set in bit_mask
                               * R4_Org_SGSNSMORecord_cellIdentifier_present if
                               * present */
    R4_Org_MessageReference messageReference;
    R4_Org_TimeStamp eventTimeStamp;
    R4_Org_SMSResult smsResult;  /* optional; set in bit_mask
                                  * R4_Org_SGSNSMORecord_smsResult_present if
                                  * present */
    struct R4_Org_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                             * R4_Org_SGSNSMORecord_recordExtensions_present if
                             * present */
    R4_Org_NodeID   nodeID;  /* optional; set in bit_mask
                              * R4_Org_SGSNSMORecord_nodeID_present if
                              * present */
    R4_Org_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                          * R4_Org_SGSNSMORecord_localSequenceNumber_present if
                          * present */
    R4_Org_ChargingCharacteristics chargingCharacteristics;
    R4_Org_SystemType systemType;  /* optional; set in bit_mask
                                    * R4_Org_SGSNSMORecord_systemType_present if
                                    * present */
    R4_Org_CalledNumber destinationNumber;  /* optional; set in bit_mask
                                             * R4_Org_destinationNumber_present
                                             * if present */
    R4_Org_CAMELInformationSMS cAMELInformationSMS;  /* optional; set in
                                   * bit_mask
                          * R4_Org_SGSNSMORecord_cAMELInformationSMS_present if
                          * present */
    R4_Org_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                            * R4_Org_SGSNSMORecord_chChSelectionMode_present if
                            * present */
} R4_Org_SGSNSMORecord;

typedef struct R4_Org_SGSNSMTRecord {
    unsigned short  bit_mask;
#       define      R4_Org_SGSNSMTRecord_servedIMEI_present 0x8000
#       define      R4_Org_SGSNSMTRecord_servedMSISDN_present 0x4000
#       define      R4_Org_SGSNSMTRecord_msNetworkCapability_present 0x2000
#       define      R4_Org_SGSNSMTRecord_serviceCentre_present 0x1000
#       define      R4_Org_SGSNSMTRecord_recordingEntity_present 0x0800
#       define      R4_Org_SGSNSMTRecord_locationArea_present 0x0400
#       define      R4_Org_SGSNSMTRecord_routingArea_present 0x0200
#       define      R4_Org_SGSNSMTRecord_cellIdentifier_present 0x0100
#       define      R4_Org_SGSNSMTRecord_smsResult_present 0x0080
#       define      R4_Org_SGSNSMTRecord_recordExtensions_present 0x0040
#       define      R4_Org_SGSNSMTRecord_nodeID_present 0x0020
#       define      R4_Org_SGSNSMTRecord_localSequenceNumber_present 0x0010
#       define      R4_Org_SGSNSMTRecord_systemType_present 0x0008
#       define      R4_Org_SGSNSMTRecord_chChSelectionMode_present 0x0004
#       define      R4_Org_SGSNSMTRecord_cAMELInformationSMS_present 0x0002
    R4_Org_CallEventRecordType recordType;
    R4_Org_IMSI     servedIMSI;
    R4_Org_IMEI     servedIMEI;  /* optional; set in bit_mask
                                  * R4_Org_SGSNSMTRecord_servedIMEI_present if
                                  * present */
    R4_Org_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * R4_Org_SGSNSMTRecord_servedMSISDN_present
                                    * if present */
    R4_Org_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                          * R4_Org_SGSNSMTRecord_msNetworkCapability_present if
                          * present */
    R4_Org_AddressString serviceCentre;  /* optional; set in bit_mask
                                * R4_Org_SGSNSMTRecord_serviceCentre_present if
                                * present */
    R4_Org_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                              * R4_Org_SGSNSMTRecord_recordingEntity_present if
                              * present */
    R4_Org_LocationAreaCode locationArea;  /* optional; set in bit_mask
                                 * R4_Org_SGSNSMTRecord_locationArea_present if
                                 * present */
    R4_Org_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                  * R4_Org_SGSNSMTRecord_routingArea_present if
                                  * present */
    R4_Org_CellId   cellIdentifier;  /* optional; set in bit_mask
                               * R4_Org_SGSNSMTRecord_cellIdentifier_present if
                               * present */
    R4_Org_TimeStamp eventTimeStamp;
    R4_Org_SMSResult smsResult;  /* optional; set in bit_mask
                                  * R4_Org_SGSNSMTRecord_smsResult_present if
                                  * present */
    struct R4_Org_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                             * R4_Org_SGSNSMTRecord_recordExtensions_present if
                             * present */
    R4_Org_NodeID   nodeID;  /* optional; set in bit_mask
                              * R4_Org_SGSNSMTRecord_nodeID_present if
                              * present */
    R4_Org_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                          * R4_Org_SGSNSMTRecord_localSequenceNumber_present if
                          * present */
    R4_Org_ChargingCharacteristics chargingCharacteristics;
    R4_Org_SystemType systemType;  /* optional; set in bit_mask
                                    * R4_Org_SGSNSMTRecord_systemType_present if
                                    * present */
    R4_Org_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                            * R4_Org_SGSNSMTRecord_chChSelectionMode_present if
                            * present */
    R4_Org_CAMELInformationSMS cAMELInformationSMS;  /* optional; set in
                                   * bit_mask
                          * R4_Org_SGSNSMTRecord_cAMELInformationSMS_present if
                          * present */
} R4_Org_SGSNSMTRecord;

typedef enum R4_Org_LCSClientType {
    R4_Org_emergencyServices = 0,
    R4_Org_valueAddedServices = 1,
    R4_Org_plmnOperatorServices = 2,
    R4_Org_lawfulInterceptServices = 3
} R4_Org_LCSClientType;

typedef struct R4_Org_LCSClientExternalID {
    unsigned char   bit_mask;
#       define      R4_Org_externalAddress_present 0x80
    R4_Org_AddressString externalAddress;  /* optional; set in bit_mask
                                            * R4_Org_externalAddress_present if
                                            * present */
} R4_Org_LCSClientExternalID;

typedef enum R4_Org_LCSClientInternalID {
    R4_Org_broadcastService = 0,
    R4_Org_o_andM_HPLMN = 1,
    R4_Org_o_andM_VPLMN = 2,
    R4_Org_anonymousLocation = 3,
    R4_Org_targetMSsubscribedService = 4
} R4_Org_LCSClientInternalID;

typedef struct R4_Org_LCSClientIdentity {
    unsigned char   bit_mask;
#       define      R4_Org_lcsClientExternalID_present 0x80
#       define      R4_Org_lcsClientDialedByMS_present 0x40
#       define      R4_Org_lcsClientInternalID_present 0x20
    R4_Org_LCSClientExternalID lcsClientExternalID;  /* optional; set in
                                   * bit_mask R4_Org_lcsClientExternalID_present
                                   * if present */
    R4_Org_AddressString lcsClientDialedByMS;  /* optional; set in bit_mask
                                        * R4_Org_lcsClientDialedByMS_present if
                                        * present */
    R4_Org_LCSClientInternalID lcsClientInternalID;  /* optional; set in
                                   * bit_mask R4_Org_lcsClientInternalID_present
                                   * if present */
} R4_Org_LCSClientIdentity;

typedef enum R4_Org_LocationEstimateType {
    R4_Org_currentLocation = 0,
    R4_Org_currentOrLastKnownLocation = 1,
    R4_Org_initialLocation = 2,
    R4_Org_activateDeferredLocation = 3,
    R4_Org_cancelDeferredLocation = 4
} R4_Org_LocationEstimateType;

typedef unsigned short  R4_Org_DeferredLocationEventType;
#define                     R4_Org_msAvailable 0x8000

typedef struct R4_Org_LocationType {
    unsigned char   bit_mask;
#       define      R4_Org_deferredLocationEventType_present 0x80
    R4_Org_LocationEstimateType locationEstimateType;
    R4_Org_DeferredLocationEventType deferredLocationEventType;  /* optional;
                                   * set in bit_mask
                                   * R4_Org_deferredLocationEventType_present if
                                   * present */
} R4_Org_LocationType;

typedef struct R4_Org_LCSQoSInfo {
    unsigned short  length;
    unsigned char   value[4];
} R4_Org_LCSQoSInfo;

typedef struct R4_Org_LCS_Priority {
    unsigned short  length;
    unsigned char   value[1];
} R4_Org_LCS_Priority;

typedef enum R4_Org_NotificationToMSUser {
    R4_Org_notifyLocationAllowed = 0,
    R4_Org_notifyAndVerify_LocationAllowedIfNoResponse = 1,
    R4_Org_notifyAndVerify_LocationNotAllowedIfNoResponse = 2,
    R4_Org_locationNotAllowed = 3
} R4_Org_NotificationToMSUser;

typedef struct R4_Org_LocationAreaAndCell {
    R4_Org_LocationAreaCode locationAreaCode;
    R4_Org_CellId   cellId;
} R4_Org_LocationAreaAndCell;

typedef struct R4_Org_Ext_GeographicalInformation {
    unsigned short  length;
    unsigned char   value[20];
} R4_Org_Ext_GeographicalInformation;

typedef struct R4_Org_LCSCause {
    unsigned short  length;
    unsigned char   value[1];
} R4_Org_LCSCause;

typedef struct R4_Org_SGSNLCTRecord {
    unsigned int    bit_mask;
#       define      R4_Org_SGSNLCTRecord_servedMSISDN_present 0x80000000
#       define      R4_Org_SGSNLCTRecord_sgsnAddress_present 0x40000000
#       define      R4_Org_SGSNLCTRecord_lcsQos_present 0x20000000
#       define      R4_Org_SGSNLCTRecord_lcsPriority_present 0x10000000
#       define      R4_Org_SGSNLCTRecord_measurementDuration_present 0x08000000
#       define      R4_Org_notificationToMSUser_present 0x04000000
#       define      R4_Org_privacyOverride_present 0x02000000
#       define      R4_Org_SGSNLCTRecord_location_present 0x01000000
#       define      R4_Org_SGSNLCTRecord_routingArea_present 0x00800000
#       define      R4_Org_SGSNLCTRecord_locationEstimate_present 0x00400000
#       define      R4_Org_SGSNLCTRecord_lcsCause_present 0x00200000
#       define      R4_Org_SGSNLCTRecord_diagnostics_present 0x00100000
#       define      R4_Org_SGSNLCTRecord_nodeID_present 0x00080000
#       define      R4_Org_SGSNLCTRecord_localSequenceNumber_present 0x00040000
#       define      R4_Org_SGSNLCTRecord_chChSelectionMode_present 0x00020000
#       define      R4_Org_SGSNLCTRecord_systemType_present 0x00010000
#       define      R4_Org_SGSNLCTRecord_recordExtensions_present 0x00008000
    R4_Org_CallEventRecordType recordType;
    R4_Org_RecordingEntity recordingEntity;
    R4_Org_LCSClientType lcsClientType;
    R4_Org_LCSClientIdentity lcsClientIdentity;
    R4_Org_IMSI     servedIMSI;
    R4_Org_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * R4_Org_SGSNLCTRecord_servedMSISDN_present
                                    * if present */
    R4_Org_GSNAddress sgsnAddress;  /* optional; set in bit_mask
                                     * R4_Org_SGSNLCTRecord_sgsnAddress_present
                                     * if present */
    R4_Org_LocationType locationType;
    R4_Org_LCSQoSInfo lcsQos;  /* optional; set in bit_mask
                                * R4_Org_SGSNLCTRecord_lcsQos_present if
                                * present */
    R4_Org_LCS_Priority lcsPriority;  /* optional; set in bit_mask
                                  * R4_Org_SGSNLCTRecord_lcsPriority_present if
                                  * present */
    R4_Org_ISDN_AddressString mlcNumber;
    R4_Org_TimeStamp eventTimeStamp;
    R4_Org_CallDuration measurementDuration;  /* optional; set in bit_mask
                          * R4_Org_SGSNLCTRecord_measurementDuration_present if
                          * present */
    R4_Org_NotificationToMSUser notificationToMSUser;  /* optional; set in
                                   * bit_mask
                                   * R4_Org_notificationToMSUser_present if
                                   * present */
    Nulltype        privacyOverride;  /* optional; set in bit_mask
                                       * R4_Org_privacyOverride_present if
                                       * present */
    R4_Org_LocationAreaAndCell location;  /* optional; set in bit_mask
                                     * R4_Org_SGSNLCTRecord_location_present if
                                     * present */
    R4_Org_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                  * R4_Org_SGSNLCTRecord_routingArea_present if
                                  * present */
    R4_Org_Ext_GeographicalInformation locationEstimate;  /* optional; set in
                                   * bit_mask
                             * R4_Org_SGSNLCTRecord_locationEstimate_present if
                             * present */
    R4_Org_LCSCause lcsCause;  /* optional; set in bit_mask
                                * R4_Org_SGSNLCTRecord_lcsCause_present if
                                * present */
    R4_Org_Diagnostics diagnostics;  /* optional; set in bit_mask
                                      * R4_Org_SGSNLCTRecord_diagnostics_present
                                      * if present */
    R4_Org_NodeID   nodeID;  /* optional; set in bit_mask
                              * R4_Org_SGSNLCTRecord_nodeID_present if
                              * present */
    R4_Org_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                          * R4_Org_SGSNLCTRecord_localSequenceNumber_present if
                          * present */
    R4_Org_ChargingCharacteristics chargingCharacteristics;
    R4_Org_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                            * R4_Org_SGSNLCTRecord_chChSelectionMode_present if
                            * present */
    R4_Org_SystemType systemType;  /* optional; set in bit_mask
                                    * R4_Org_SGSNLCTRecord_systemType_present if
                                    * present */
    struct R4_Org_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                             * R4_Org_SGSNLCTRecord_recordExtensions_present if
                             * present */
    R4_Org_CauseForRecClosing causeForRecClosing;
} R4_Org_SGSNLCTRecord;

typedef enum R4_Org_LocationMethod {
    R4_Org_msBasedEOTD = 0,
    R4_Org_msAssistedEOTD = 1,
    R4_Org_assistedGPS = 2,
    R4_Org_msBasedOTDOA = 3,
    R4_Org_msAssistedOTDOA = 4
} R4_Org_LocationMethod;

typedef struct R4_Org_SGSNLCORecord {
    unsigned int    bit_mask;
#       define      R4_Org_SGSNLCORecord_lcsClientType_present 0x80000000
#       define      R4_Org_SGSNLCORecord_lcsClientIdentity_present 0x40000000
#       define      R4_Org_SGSNLCORecord_servedMSISDN_present 0x20000000
#       define      R4_Org_SGSNLCORecord_sgsnAddress_present 0x10000000
#       define      R4_Org_SGSNLCORecord_lcsQos_present 0x08000000
#       define      R4_Org_SGSNLCORecord_lcsPriority_present 0x04000000
#       define      R4_Org_SGSNLCORecord_mlcNumber_present 0x02000000
#       define      R4_Org_SGSNLCORecord_measurementDuration_present 0x01000000
#       define      R4_Org_SGSNLCORecord_location_present 0x00800000
#       define      R4_Org_SGSNLCORecord_routingArea_present 0x00400000
#       define      R4_Org_SGSNLCORecord_locationEstimate_present 0x00200000
#       define      R4_Org_SGSNLCORecord_lcsCause_present 0x00100000
#       define      R4_Org_SGSNLCORecord_diagnostics_present 0x00080000
#       define      R4_Org_SGSNLCORecord_nodeID_present 0x00040000
#       define      R4_Org_SGSNLCORecord_localSequenceNumber_present 0x00020000
#       define      R4_Org_SGSNLCORecord_chChSelectionMode_present 0x00010000
#       define      R4_Org_SGSNLCORecord_systemType_present 0x00008000
#       define      R4_Org_SGSNLCORecord_recordExtensions_present 0x00004000
    R4_Org_CallEventRecordType recordType;
    R4_Org_RecordingEntity recordingEntity;
    R4_Org_LCSClientType lcsClientType;  /* optional; set in bit_mask
                                * R4_Org_SGSNLCORecord_lcsClientType_present if
                                * present */
    R4_Org_LCSClientIdentity lcsClientIdentity;  /* optional; set in bit_mask
                            * R4_Org_SGSNLCORecord_lcsClientIdentity_present if
                            * present */
    R4_Org_IMSI     servedIMSI;
    R4_Org_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * R4_Org_SGSNLCORecord_servedMSISDN_present
                                    * if present */
    R4_Org_GSNAddress sgsnAddress;  /* optional; set in bit_mask
                                     * R4_Org_SGSNLCORecord_sgsnAddress_present
                                     * if present */
    R4_Org_LocationMethod locationMethod;
    R4_Org_LCSQoSInfo lcsQos;  /* optional; set in bit_mask
                                * R4_Org_SGSNLCORecord_lcsQos_present if
                                * present */
    R4_Org_LCS_Priority lcsPriority;  /* optional; set in bit_mask
                                  * R4_Org_SGSNLCORecord_lcsPriority_present if
                                  * present */
    R4_Org_ISDN_AddressString mlcNumber;  /* optional; set in bit_mask
                                    * R4_Org_SGSNLCORecord_mlcNumber_present if
                                    * present */
    R4_Org_TimeStamp eventTimeStamp;
    R4_Org_CallDuration measurementDuration;  /* optional; set in bit_mask
                          * R4_Org_SGSNLCORecord_measurementDuration_present if
                          * present */
    R4_Org_LocationAreaAndCell location;  /* optional; set in bit_mask
                                     * R4_Org_SGSNLCORecord_location_present if
                                     * present */
    R4_Org_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                  * R4_Org_SGSNLCORecord_routingArea_present if
                                  * present */
    R4_Org_Ext_GeographicalInformation locationEstimate;  /* optional; set in
                                   * bit_mask
                             * R4_Org_SGSNLCORecord_locationEstimate_present if
                             * present */
    R4_Org_LCSCause lcsCause;  /* optional; set in bit_mask
                                * R4_Org_SGSNLCORecord_lcsCause_present if
                                * present */
    R4_Org_Diagnostics diagnostics;  /* optional; set in bit_mask
                                      * R4_Org_SGSNLCORecord_diagnostics_present
                                      * if present */
    R4_Org_NodeID   nodeID;  /* optional; set in bit_mask
                              * R4_Org_SGSNLCORecord_nodeID_present if
                              * present */
    R4_Org_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                          * R4_Org_SGSNLCORecord_localSequenceNumber_present if
                          * present */
    R4_Org_ChargingCharacteristics chargingCharacteristics;
    R4_Org_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                            * R4_Org_SGSNLCORecord_chChSelectionMode_present if
                            * present */
    R4_Org_SystemType systemType;  /* optional; set in bit_mask
                                    * R4_Org_SGSNLCORecord_systemType_present if
                                    * present */
    struct R4_Org_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                             * R4_Org_SGSNLCORecord_recordExtensions_present if
                             * present */
    R4_Org_CauseForRecClosing causeForRecClosing;
} R4_Org_SGSNLCORecord;

typedef struct R4_Org_SGSNLCNRecord {
    unsigned int    bit_mask;
#       define      R4_Org_SGSNLCNRecord_lcsClientType_present 0x80000000
#       define      R4_Org_SGSNLCNRecord_lcsClientIdentity_present 0x40000000
#       define      R4_Org_servedIMSI_present 0x20000000
#       define      R4_Org_SGSNLCNRecord_servedMSISDN_present 0x10000000
#       define      R4_Org_SGSNLCNRecord_sgsnAddress_present 0x08000000
#       define      R4_Org_SGSNLCNRecord_servedIMEI_present 0x04000000
#       define      R4_Org_SGSNLCNRecord_lcsQos_present 0x02000000
#       define      R4_Org_SGSNLCNRecord_lcsPriority_present 0x01000000
#       define      R4_Org_SGSNLCNRecord_mlcNumber_present 0x00800000
#       define      R4_Org_SGSNLCNRecord_measurementDuration_present 0x00400000
#       define      R4_Org_SGSNLCNRecord_location_present 0x00200000
#       define      R4_Org_SGSNLCNRecord_routingArea_present 0x00100000
#       define      R4_Org_SGSNLCNRecord_locationEstimate_present 0x00080000
#       define      R4_Org_SGSNLCNRecord_lcsCause_present 0x00040000
#       define      R4_Org_SGSNLCNRecord_diagnostics_present 0x00020000
#       define      R4_Org_SGSNLCNRecord_nodeID_present 0x00010000
#       define      R4_Org_SGSNLCNRecord_localSequenceNumber_present 0x00008000
#       define      R4_Org_SGSNLCNRecord_chChSelectionMode_present 0x00004000
#       define      R4_Org_SGSNLCNRecord_systemType_present 0x00002000
#       define      R4_Org_SGSNLCNRecord_recordExtensions_present 0x00001000
    R4_Org_CallEventRecordType recordType;
    R4_Org_RecordingEntity recordingEntity;
    R4_Org_LCSClientType lcsClientType;  /* optional; set in bit_mask
                                * R4_Org_SGSNLCNRecord_lcsClientType_present if
                                * present */
    R4_Org_LCSClientIdentity lcsClientIdentity;  /* optional; set in bit_mask
                            * R4_Org_SGSNLCNRecord_lcsClientIdentity_present if
                            * present */
    R4_Org_IMSI     servedIMSI;  /* optional; set in bit_mask
                                  * R4_Org_servedIMSI_present if present */
    R4_Org_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * R4_Org_SGSNLCNRecord_servedMSISDN_present
                                    * if present */
    R4_Org_GSNAddress sgsnAddress;  /* optional; set in bit_mask
                                     * R4_Org_SGSNLCNRecord_sgsnAddress_present
                                     * if present */
    R4_Org_IMEI     servedIMEI;  /* optional; set in bit_mask
                                  * R4_Org_SGSNLCNRecord_servedIMEI_present if
                                  * present */
    R4_Org_LCSQoSInfo lcsQos;  /* optional; set in bit_mask
                                * R4_Org_SGSNLCNRecord_lcsQos_present if
                                * present */
    R4_Org_LCS_Priority lcsPriority;  /* optional; set in bit_mask
                                  * R4_Org_SGSNLCNRecord_lcsPriority_present if
                                  * present */
    R4_Org_ISDN_AddressString mlcNumber;  /* optional; set in bit_mask
                                    * R4_Org_SGSNLCNRecord_mlcNumber_present if
                                    * present */
    R4_Org_TimeStamp eventTimeStamp;
    R4_Org_CallDuration measurementDuration;  /* optional; set in bit_mask
                          * R4_Org_SGSNLCNRecord_measurementDuration_present if
                          * present */
    R4_Org_LocationAreaAndCell location;  /* optional; set in bit_mask
                                     * R4_Org_SGSNLCNRecord_location_present if
                                     * present */
    R4_Org_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                  * R4_Org_SGSNLCNRecord_routingArea_present if
                                  * present */
    R4_Org_Ext_GeographicalInformation locationEstimate;  /* optional; set in
                                   * bit_mask
                             * R4_Org_SGSNLCNRecord_locationEstimate_present if
                             * present */
    R4_Org_LCSCause lcsCause;  /* optional; set in bit_mask
                                * R4_Org_SGSNLCNRecord_lcsCause_present if
                                * present */
    R4_Org_Diagnostics diagnostics;  /* optional; set in bit_mask
                                      * R4_Org_SGSNLCNRecord_diagnostics_present
                                      * if present */
    R4_Org_NodeID   nodeID;  /* optional; set in bit_mask
                              * R4_Org_SGSNLCNRecord_nodeID_present if
                              * present */
    R4_Org_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                          * R4_Org_SGSNLCNRecord_localSequenceNumber_present if
                          * present */
    R4_Org_ChargingCharacteristics chargingCharacteristics;
    R4_Org_ChChSelectionMode chChSelectionMode;  /* optional; set in bit_mask
                            * R4_Org_SGSNLCNRecord_chChSelectionMode_present if
                            * present */
    R4_Org_SystemType systemType;  /* optional; set in bit_mask
                                    * R4_Org_SGSNLCNRecord_systemType_present if
                                    * present */
    struct R4_Org_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                             * R4_Org_SGSNLCNRecord_recordExtensions_present if
                             * present */
    R4_Org_CauseForRecClosing causeForRecClosing;
} R4_Org_SGSNLCNRecord;

typedef struct R4_Org_CallEventRecord {
    unsigned short  choice;
#       define      R4_Org_sgsnPDPRecord_chosen 1
#       define      R4_Org_ggsnPDPRecord_chosen 2
#       define      R4_Org_sgsnMMRecord_chosen 3
#       define      R4_Org_sgsnSMORecord_chosen 4
#       define      R4_Org_sgsnSMTRecord_chosen 5
#       define      R4_Org_sgsnLCTRecord_chosen 6
#       define      R4_Org_sgsnLCORecord_chosen 7
#       define      R4_Org_sgsnLCNRecord_chosen 8
    union _union {
        R4_Org_SGSNPDPRecord sgsnPDPRecord;  /* to choose, set choice to
                                              * R4_Org_sgsnPDPRecord_chosen */
        R4_Org_GGSNPDPRecord ggsnPDPRecord;  /* to choose, set choice to
                                              * R4_Org_ggsnPDPRecord_chosen */
        R4_Org_SGSNMMRecord sgsnMMRecord;  /* to choose, set choice to
                                            * R4_Org_sgsnMMRecord_chosen */
        R4_Org_SGSNSMORecord sgsnSMORecord;  /* to choose, set choice to
                                              * R4_Org_sgsnSMORecord_chosen */
        R4_Org_SGSNSMTRecord sgsnSMTRecord;  /* to choose, set choice to
                                              * R4_Org_sgsnSMTRecord_chosen */
        R4_Org_SGSNLCTRecord sgsnLCTRecord;  /* to choose, set choice to
                                              * R4_Org_sgsnLCTRecord_chosen */
        R4_Org_SGSNLCORecord sgsnLCORecord;  /* to choose, set choice to
                                              * R4_Org_sgsnLCORecord_chosen */
        R4_Org_SGSNLCNRecord sgsnLCNRecord;  /* to choose, set choice to
                                              * R4_Org_sgsnLCNRecord_chosen */
    } u;
} R4_Org_CallEventRecord;

typedef struct R4_Org_ManagementExtensions_ {
    struct R4_Org_ManagementExtensions_ *next;
    R4_Org_ManagementExtension value;
} *R4_Org_ManagementExtensions;

typedef struct R4_Org_Horizontal_Accuracy {
    unsigned short  length;
    unsigned char   value[1];
} R4_Org_Horizontal_Accuracy;

typedef enum R4_Org_ResponseTimeCategory {
    R4_Org_lowdelay = 0,
    R4_Org_delaytolerant = 1
} R4_Org_ResponseTimeCategory;

typedef struct R4_Org_ResponseTime {
    R4_Org_ResponseTimeCategory responseTimeCategory;
} R4_Org_ResponseTime;

typedef struct R4_Org_Vertical_Accuracy {
    unsigned short  length;
    unsigned char   value[1];
} R4_Org_Vertical_Accuracy;

extern int R4_Org_maxAddressLength;

extern int R4_Org_maxISDN_AddressLength;

extern int R4_Org_maxExt_GeographicalInformation;


extern void *CDRF_R4_Org;    /* encoder-decoder control table */
#ifdef __cplusplus
}	/* extern "C" */
#endif /* __cplusplus */
#endif /* OSS_CDRF_R4_Org */
