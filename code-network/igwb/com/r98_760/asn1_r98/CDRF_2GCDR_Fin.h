/********************************************************/
/* Copyright (C) 2003 OSS Nokalva.  All rights reserved.*/
/********************************************************/

/* THIS FILE IS PROPRIETARY MATERIAL OF OSS NOKALVA
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF OSS NOKALVA.
 * THIS FILE MAY NOT BE DISTRIBUTED. */

/* Generated for: HuaWei Technologies, Inc., Shenzhen, China - license 8088 for Windows NT */
/* Abstract syntax: CDRF_2GCDR_Fin */
/* Created: Mon Mar 24 10:40:39 2003 */
/* ASN.1 compiler version: 5.3 */
/* Target operating system: Windows NT/Windows 9x */
/* Target machine type: Intel x86 */
/* C compiler options required: -Zp4 (Microsoft) */
/* ASN.1 compiler options and file names specified:
 * -codefile CDRF_2GCDR_Fin.c -headerfile CDRF_2GCDR_Fin.h -errorfile
 * C:\WINDOWS\TEMP\2. -prefix Fin_ -dualheader -ansi -verbose -ber -root
 * D:\application\ossasn1\win32\5.3.0\asn1dflt\asn1dflt.ms.zp4
 * D:\project\530\asn1app\maimaoshi\CDRF_2GCDR_Fin.asn
 */

#ifndef OSS_CDRF_2GCDR_Fin
#define OSS_CDRF_2GCDR_Fin

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "asn1hdr.h"
#include "asn1code.h"

#define          Fin_CallEventRecord_PDU 1

typedef struct Fin__ObjectID {
    unsigned short  length;
    unsigned char   *value;
} Fin__ObjectID;

typedef struct Fin__Any {
    unsigned long   length;
    unsigned char   *value;
} Fin__Any;

typedef int             Fin_CallEventRecordType;
#define                     Fin_CallEventRecordType_sgsnPDPRecord 18
#define                     Fin_CallEventRecordType_ggsnPDPRecord 19
#define                     Fin_CallEventRecordType_sgsnMMRecord 20
#define                     Fin_CallEventRecordType_sgsnSMORecord 21
#define                     Fin_CallEventRecordType_sgsnSMTRecord 22

typedef ossBoolean      Fin_NetworkInitiatedPDPContext;

typedef struct Fin_IMSI {
    unsigned short  length;
    unsigned char   value[8];
} Fin_IMSI;

typedef struct Fin_IMEI {
    unsigned short  length;
    unsigned char   value[8];
} Fin_IMEI;

typedef struct Fin_IPBinaryAddress {
    unsigned short  choice;
#       define      Fin_iPBinV4Address_chosen 1
#       define      Fin_iPBinV6Address_chosen 2
    union _union {
        struct Fin__octet1 {
            unsigned short  length;
            unsigned char   value[4];
        } iPBinV4Address;  /* to choose, set choice to
                            * Fin_iPBinV4Address_chosen */
        struct Fin__octet2 {
            unsigned short  length;
            unsigned char   value[16];
        } iPBinV6Address;  /* to choose, set choice to
                            * Fin_iPBinV6Address_chosen */
    } u;
} Fin_IPBinaryAddress;

typedef struct Fin_IPTextRepresentedAddress {
    unsigned short  choice;
#       define      Fin_iPTextV4Address_chosen 1
#       define      Fin_iPTextV6Address_chosen 2
    union _union {
        char            iPTextV4Address[16];  /* to choose, set choice to
                                               * Fin_iPTextV4Address_chosen */
        char            iPTextV6Address[46];  /* to choose, set choice to
                                               * Fin_iPTextV6Address_chosen */
    } u;
} Fin_IPTextRepresentedAddress;

typedef struct Fin_IPAddress {
    unsigned short  choice;
#       define      Fin_iPBinaryAddress_chosen 1
#       define      Fin_iPTextRepresentedAddress_chosen 2
    union _union {
        Fin_IPBinaryAddress iPBinaryAddress;  /* to choose, set choice to
                                               * Fin_iPBinaryAddress_chosen */
        Fin_IPTextRepresentedAddress iPTextRepresentedAddress;  /* to choose,
                                   * set choice to
                                   * Fin_iPTextRepresentedAddress_chosen */
    } u;
} Fin_IPAddress;

typedef Fin_IPAddress   Fin_GSNAddress;

typedef struct Fin_MSNetworkCapability {
    unsigned short  length;
    unsigned char   value[1];
} Fin_MSNetworkCapability;

typedef struct Fin_RoutingAreaCode {
    unsigned short  length;
    unsigned char   value[1];
} Fin_RoutingAreaCode;

typedef struct Fin_LocationAreaCode {
    unsigned short  length;
    unsigned char   value[2];
} Fin_LocationAreaCode;

typedef struct Fin_CellId {
    unsigned short  length;
    unsigned char   value[2];
} Fin_CellId;

typedef unsigned int    Fin_ChargingID;

typedef char            Fin_AccessPointNameNI[64];

typedef struct Fin_PDPType {
    unsigned short  length;
    unsigned char   value[2];
} Fin_PDPType;

typedef struct Fin_AddressString {
    unsigned short  length;
    unsigned char   value[20];
} Fin_AddressString;

typedef Fin_AddressString Fin_ETSIAddress;

typedef struct Fin_PDPAddress {
    unsigned short  choice;
#       define      Fin_iPAddress_chosen 1
#       define      Fin_eTSIAddress_chosen 2
    union _union {
        Fin_IPAddress   iPAddress;  /* to choose, set choice to
                                     * Fin_iPAddress_chosen */
        Fin_ETSIAddress eTSIAddress;  /* to choose, set choice to
                                       * Fin_eTSIAddress_chosen */
    } u;
} Fin_PDPAddress;

typedef enum Fin_QoSReliability {
    Fin_unspecifiedReliability = 0,
    Fin_acknowledgedGTP = 1,
    Fin_unackGTPAcknowLLC = 2,
    Fin_unackGTPLLCAcknowRLC = 3,
    Fin_unackGTPLLCRLC = 4,
    Fin_unacknowUnprotectedData = 5
} Fin_QoSReliability;

typedef enum Fin_QoSDelay {
    Fin_delayClass1 = 1,
    Fin_delayClass2 = 2,
    Fin_delayClass3 = 3,
    Fin_delayClass4 = 4
} Fin_QoSDelay;

typedef enum Fin_QoSPrecedence {
    Fin_QoSPrecedence_unspecified = 0,
    Fin_highPriority = 1,
    Fin_normalPriority = 2,
    Fin_lowPriority = 3
} Fin_QoSPrecedence;

typedef enum Fin_QoSPeakThroughput {
    Fin_QoSPeakThroughput_unspecified = 0,
    Fin_upTo100OctetPs = 1,
    Fin_upTo200OctetPs = 2,
    Fin_upTo400OctetPs = 3,
    Fin_upTo800OctetPs = 4,
    Fin_upTo1600OctetPs = 5,
    Fin_upTo3200OctetPs = 6,
    Fin_upTo6400OctetPs = 7,
    Fin_upTo12800OctetPs = 8,
    Fin_upTo25600OctetPs = 9
} Fin_QoSPeakThroughput;

typedef enum Fin_QoSMeanThroughput {
    Fin_bestEffort = 0,
    Fin_mean100octetPh = 1,
    Fin_mean200octetPh = 2,
    Fin_mean500octetPh = 3,
    Fin_mean1000octetPh = 4,
    Fin_mean2000octetPh = 5,
    Fin_mean5000octetPh = 6,
    Fin_mean10000octetPh = 7,
    Fin_mean20000octetPh = 8,
    Fin_mean50000octetPh = 9,
    Fin_mean100000octetPh = 10,
    Fin_mean200000octetPh = 11,
    Fin_mean500000octetPh = 12,
    Fin_mean1000000octetPh = 13,
    Fin_mean2000000octetPh = 14,
    Fin_mean5000000octetPh = 15,
    Fin_mean10000000octetPh = 16,
    Fin_mean20000000octetPh = 17,
    Fin_mean50000000octetPh = 18
} Fin_QoSMeanThroughput;

typedef struct Fin_QoSInformation {
    Fin_QoSReliability reliability;
    Fin_QoSDelay    delay;
    Fin_QoSPrecedence precedence;
    Fin_QoSPeakThroughput peakThroughput;
    Fin_QoSMeanThroughput meanThroughput;
} Fin_QoSInformation;

typedef int             Fin_DataVolumeGPRS;

typedef enum Fin_ChangeCondition {
    Fin_qoSChange = 0,
    Fin_tariffTime = 1,
    Fin_recordClosure = 2
} Fin_ChangeCondition;

typedef struct Fin_TimeStamp {
    unsigned short  length;
    unsigned char   value[9];
} Fin_TimeStamp;

typedef struct Fin_ChangeOfCharCondition {
    unsigned char   bit_mask;
#       define      Fin_qosRequested_present 0x80
#       define      Fin_qosNegotiated_present 0x40
    Fin_QoSInformation qosRequested;  /* optional; set in bit_mask
                                       * Fin_qosRequested_present if present */
    Fin_QoSInformation qosNegotiated;  /* optional; set in bit_mask
                                        * Fin_qosNegotiated_present if
                                        * present */
    Fin_DataVolumeGPRS dataVolumeGPRSUplink;
    Fin_DataVolumeGPRS dataVolumeGPRSDownlink;
    Fin_ChangeCondition changeCondition;
    Fin_TimeStamp   changeTime;
} Fin_ChangeOfCharCondition;

typedef int             Fin_CallDuration;

typedef ossBoolean      Fin_SGSNChange;

typedef int             Fin_CauseForRecClosing;
#define                     Fin_normalRelease 0
#define                     Fin_abnormalRelease 4
#define                     Fin_volumeLimit 16
#define                     Fin_timeLimit 17
#define                     Fin_sGSNChange 18
#define                     Fin_maxChangeCond 19
#define                     Fin_managementIntervention 20

typedef struct Fin_RECORDSequenceNumber {
    Fin_GSNAddress  gsnAddress;
    struct Fin_RecordNumberList_ *recordNumberList;
} Fin_RECORDSequenceNumber;

typedef char            Fin_NodeID[21];

typedef enum Fin_APNSelectionMode {
    Fin_mSorNetworkProvidedSubscriptionVerified = 0,
    Fin_mSProvidedSubscriptionNotVerified = 1,
    Fin_networkProvidedSubscriptionNotVerified = 2
} Fin_APNSelectionMode;

typedef char            Fin_AccessPointNameOI[38];

typedef struct Fin_ISDN_AddressString {
    unsigned short  length;
    unsigned char   value[10];
} Fin_ISDN_AddressString;

typedef Fin_ISDN_AddressString Fin_MSISDN;

typedef enum Fin_ConsolidationResult {
    Fin_normal = 0,
    Fin_notNormal = 1,
    Fin_forInterSGSNConsolidation = 2,
    Fin_reachLimit = 3,
    Fin_onlyOneCDRGenerated = 4
} Fin_ConsolidationResult;

struct Fin__seqof1_ {
    struct Fin__seqof1_ *next;
    Fin_GSNAddress  value;
};

struct Fin__seqof2_ {
    struct Fin__seqof2_ *next;
    Fin_ChangeOfCharCondition value;
};

struct Fin__seqof3 {
    struct Fin__seqof3 *next;
    Fin_RECORDSequenceNumber value;
};

typedef struct Fin_SGSNPDPRecord {
    unsigned short  bit_mask;
#       define      Fin_SGSNPDPRecord_networkInitiation_present 0x8000
#       define      Fin_SGSNPDPRecord_anonymousAccessIndicator_present 0x4000
#       define      Fin_SGSNPDPRecord_servedIMEI_present 0x2000
#       define      Fin_SGSNPDPRecord_msNetworkCapability_present 0x1000
#       define      Fin_SGSNPDPRecord_routingArea_present 0x0800
#       define      Fin_SGSNPDPRecord_locationAreaCode_present 0x0400
#       define      Fin_SGSNPDPRecord_cellIdentity_present 0x0200
#       define      Fin_SGSNPDPRecord_sgsnChange_present 0x0100
#       define      Fin_SGSNPDPRecord_recordSequenceNumber_present 0x0080
#       define      Fin_SGSNPDPRecord_nodeID_present 0x0040
#       define      Fin_SGSNPDPRecord_recordExtensions_present 0x0020
#       define      Fin_SGSNPDPRecord_apnSelectionMode_present 0x0010
#       define      Fin_SGSNPDPRecord_servedMSISDN_present 0x0008
    Fin_CallEventRecordType recordType;
    Fin_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                                   * Fin_SGSNPDPRecord_networkInitiation_present
                                   * if present */
    ossBoolean      anonymousAccessIndicator;  /* optional; set in bit_mask
                        * Fin_SGSNPDPRecord_anonymousAccessIndicator_present if
                        * present */
    Fin_IMSI        servedIMSI;
    Fin_IMEI        servedIMEI;  /* optional; set in bit_mask
                                  * Fin_SGSNPDPRecord_servedIMEI_present if
                                  * present */
    struct Fin__seqof1_ *sgsnAddress;
    Fin_MSNetworkCapability msNetworkCapability;  /* optional; set in bit_mask
                             * Fin_SGSNPDPRecord_msNetworkCapability_present if
                             * present */
    Fin_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                       * Fin_SGSNPDPRecord_routingArea_present
                                       * if present */
    Fin_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                                * Fin_SGSNPDPRecord_locationAreaCode_present if
                                * present */
    Fin_CellId      cellIdentity;  /* optional; set in bit_mask
                                    * Fin_SGSNPDPRecord_cellIdentity_present if
                                    * present */
    Fin_ChargingID  chargingID;
    Fin_GSNAddress  ggsnAddressUsed;
    Fin_AccessPointNameNI accessPointNameNI;
    Fin_PDPType     pdpType;
    Fin_PDPAddress  servedPDPAddress;
    struct Fin__seqof2_ *listOfTrafficVolumes;
    Fin_TimeStamp   recordOpeningTime;
    Fin_CallDuration duration;
    Fin_SGSNChange  sgsnChange;  /* optional; set in bit_mask
                                  * Fin_SGSNPDPRecord_sgsnChange_present if
                                  * present */
    Fin_CauseForRecClosing causeForRecClosing;
    struct Fin__seqof3 *recordSequenceNumber;  /* optional; set in bit_mask
                               * Fin_SGSNPDPRecord_recordSequenceNumber_present
                               * if present */
    Fin_NodeID      nodeID;  /* optional; set in bit_mask
                              * Fin_SGSNPDPRecord_nodeID_present if present */
    struct Fin_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                                   * Fin_SGSNPDPRecord_recordExtensions_present
                                   * if present */
    Fin_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                                * Fin_SGSNPDPRecord_apnSelectionMode_present if
                                * present */
    Fin_AccessPointNameOI accessPointNameOI;
    Fin_MSISDN      servedMSISDN;  /* optional; set in bit_mask
                                    * Fin_SGSNPDPRecord_servedMSISDN_present if
                                    * present */
    Fin_ConsolidationResult consolidationResult;
} Fin_SGSNPDPRecord;

typedef ossBoolean      Fin_DynamicAddressFlag;

struct Fin__seqof4 {
    struct Fin__seqof4 *next;
    Fin_PDPAddress  value;
};

typedef struct Fin_GGSNPDPRecord {
    unsigned short  bit_mask;
#       define      Fin_GGSNPDPRecord_networkInitiation_present 0x8000
#       define      Fin_GGSNPDPRecord_anonymousAccessIndicator_present 0x4000
#       define      Fin_remotePDPAddress_present 0x2000
#       define      Fin_dynamicAddressFlag_present 0x1000
#       define      Fin_GGSNPDPRecord_recordSequenceNumber_present 0x0800
#       define      Fin_GGSNPDPRecord_nodeID_present 0x0400
#       define      Fin_GGSNPDPRecord_recordExtensions_present 0x0200
#       define      Fin_GGSNPDPRecord_apnSelectionMode_present 0x0100
#       define      Fin_GGSNPDPRecord_servedMSISDN_present 0x0080
    Fin_CallEventRecordType recordType;
    Fin_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                                   * Fin_GGSNPDPRecord_networkInitiation_present
                                   * if present */
    ossBoolean      anonymousAccessIndicator;  /* optional; set in bit_mask
                        * Fin_GGSNPDPRecord_anonymousAccessIndicator_present if
                        * present */
    Fin_IMSI        servedIMSI;
    Fin_GSNAddress  ggsnAddress;
    Fin_ChargingID  chargingID;
    struct Fin__seqof1_ *sgsnAddress;
    Fin_AccessPointNameNI accessPointNameNI;
    Fin_PDPType     pdpType;
    Fin_PDPAddress  servedPDPAddress;
    struct Fin__seqof4 *remotePDPAddress;  /* optional; set in bit_mask
                           * Fin_remotePDPAddress_present if present */
    Fin_DynamicAddressFlag dynamicAddressFlag;  /* optional; set in bit_mask
                                            * Fin_dynamicAddressFlag_present if
                                            * present */
    struct Fin__seqof2_ *listOfTrafficVolumes;
    Fin_TimeStamp   recordOpeningTime;
    Fin_CallDuration duration;
    Fin_CauseForRecClosing causeForRecClosing;
    Fin_RECORDSequenceNumber recordSequenceNumber;  /* optional; set in bit_mask
                            * Fin_GGSNPDPRecord_recordSequenceNumber_present if
                            * present */
    Fin_NodeID      nodeID;  /* optional; set in bit_mask
                              * Fin_GGSNPDPRecord_nodeID_present if present */
    struct Fin_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                                   * Fin_GGSNPDPRecord_recordExtensions_present
                                   * if present */
    Fin_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                                * Fin_GGSNPDPRecord_apnSelectionMode_present if
                                * present */
    Fin_MSISDN      servedMSISDN;  /* optional; set in bit_mask
                                    * Fin_GGSNPDPRecord_servedMSISDN_present if
                                    * present */
    Fin_ConsolidationResult consolidationResult;
} Fin_GGSNPDPRecord;

typedef struct Fin_ChangeLocation {
    unsigned char   bit_mask;
#       define      Fin_cellId_present 0x80
    Fin_LocationAreaCode locationAreaCode;
    Fin_RoutingAreaCode routingAreaCode;
    Fin_CellId      cellId;  /* optional; set in bit_mask Fin_cellId_present if
                              * present */
    Fin_TimeStamp   changeTime;
} Fin_ChangeLocation;

typedef struct Fin_ManagementExtension {
    unsigned char   bit_mask;
#       define      Fin_significance_present 0x80
    Fin__ObjectID   identifier;
    ossBoolean      significance;  /* Fin_significance_present not set in
                                    * bit_mask implies value is FALSE */
    Fin__Any        information;
} Fin_ManagementExtension;

typedef struct Fin_Diagnostics {
    unsigned short  choice;
#       define      Fin_gsm0408Cause_chosen 1
#       define      Fin_gsm0902MapErrorValue_chosen 2
#       define      Fin_ccittQ767Cause_chosen 3
#       define      Fin_networkSpecificCause_chosen 4
#       define      Fin_manufacturerSpecificCause_chosen 5
    union _union {
        int             gsm0408Cause;  /* to choose, set choice to
                                        * Fin_gsm0408Cause_chosen */
        int             gsm0902MapErrorValue;  /* to choose, set choice to
                                           * Fin_gsm0902MapErrorValue_chosen */
        int             ccittQ767Cause;  /* to choose, set choice to
                                          * Fin_ccittQ767Cause_chosen */
        Fin_ManagementExtension networkSpecificCause;  /* to choose, set choice
                                        * to Fin_networkSpecificCause_chosen */
        Fin_ManagementExtension manufacturerSpecificCause;  /* to choose, set
                                   * choice to
                                   * Fin_manufacturerSpecificCause_chosen */
    } u;
} Fin_Diagnostics;

typedef unsigned int    Fin_LocalSequenceNumber;

struct Fin__seqof5 {
     struct Fin__seqof5 *next;
     Fin_ChangeLocation value;
};

typedef struct Fin_SGSNMMRecord {
    unsigned short  bit_mask;
#       define      Fin_SGSNMMRecord_servedIMEI_present 0x8000
#       define      Fin_SGSNMMRecord_msNetworkCapability_present 0x4000
#       define      Fin_SGSNMMRecord_routingArea_present 0x2000
#       define      Fin_SGSNMMRecord_locationAreaCode_present 0x1000
#       define      Fin_SGSNMMRecord_cellIdentity_present 0x0800
#       define      Fin_changeLocation_present 0x0400
#       define      Fin_duration_present 0x0200
#       define      Fin_SGSNMMRecord_sgsnChange_present 0x0100
#       define      Fin_diagnostics_present 0x0080
#       define      Fin_SGSNMMRecord_recordSequenceNumber_present 0x0040
#       define      Fin_SGSNMMRecord_nodeID_present 0x0020
#       define      Fin_SGSNMMRecord_recordExtensions_present 0x0010
#       define      Fin_SGSNMMRecord_localSequenceNumber_present 0x0008
    Fin_CallEventRecordType recordType;
    Fin_IMSI        servedIMSI;
    Fin_IMEI        servedIMEI;  /* optional; set in bit_mask
                                  * Fin_SGSNMMRecord_servedIMEI_present if
                                  * present */
    Fin_GSNAddress  sgsnAddress;
    Fin_MSNetworkCapability msNetworkCapability;  /* optional; set in bit_mask
                              * Fin_SGSNMMRecord_msNetworkCapability_present if
                              * present */
    Fin_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                       * Fin_SGSNMMRecord_routingArea_present if
                                       * present */
    Fin_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                                 * Fin_SGSNMMRecord_locationAreaCode_present if
                                 * present */
    Fin_CellId      cellIdentity;  /* optional; set in bit_mask
                                    * Fin_SGSNMMRecord_cellIdentity_present if
                                    * present */
    struct Fin__seqof5 *changeLocation;  /* optional; set in bit_mask Fin_changeLocation_present
                         * if present */
    Fin_TimeStamp   recordOpeningTime;
    Fin_CallDuration duration;  /* optional; set in bit_mask
                                 * Fin_duration_present if present */
    Fin_SGSNChange  sgsnChange;  /* optional; set in bit_mask
                                  * Fin_SGSNMMRecord_sgsnChange_present if
                                  * present */
    Fin_CauseForRecClosing causeForRecClosing;
    Fin_Diagnostics diagnostics;  /* optional; set in bit_mask
                                   * Fin_diagnostics_present if present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                             * Fin_SGSNMMRecord_recordSequenceNumber_present if
                             * present */
    Fin_NodeID      nodeID;  /* optional; set in bit_mask
                              * Fin_SGSNMMRecord_nodeID_present if present */
    struct Fin_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                                   * Fin_SGSNMMRecord_recordExtensions_present
                                   * if present */
    Fin_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                              * Fin_SGSNMMRecord_localSequenceNumber_present if
                              * present */
} Fin_SGSNMMRecord;

typedef Fin_AddressString Fin_RecordingEntity;

typedef struct Fin_MessageReference {
    unsigned short  length;
    unsigned char   value[9];
} Fin_MessageReference;

typedef Fin_Diagnostics Fin_SMSResult;

typedef struct Fin_SGSNSMORecord {
    unsigned short  bit_mask;
#       define      Fin_SGSNSMORecord_servedIMEI_present 0x8000
#       define      Fin_SGSNSMORecord_servedMSISDN_present 0x4000
#       define      Fin_SGSNSMORecord_locationArea_present 0x2000
#       define      Fin_SGSNSMORecord_routingArea_present 0x1000
#       define      Fin_SGSNSMORecord_cellIdentity_present 0x0800
#       define      Fin_SGSNSMORecord_smsResult_present 0x0400
#       define      Fin_SGSNSMORecord_recordExtensions_present 0x0200
#       define      Fin_SGSNSMORecord_nodeID_present 0x0100
#       define      Fin_SGSNSMORecord_localSequenceNumber_present 0x0080
    Fin_CallEventRecordType recordType;
    Fin_IMSI        servedIMSI;
    Fin_IMEI        servedIMEI;  /* optional; set in bit_mask
                                  * Fin_SGSNSMORecord_servedIMEI_present if
                                  * present */
    Fin_MSISDN      servedMSISDN;  /* optional; set in bit_mask
                                    * Fin_SGSNSMORecord_servedMSISDN_present if
                                    * present */
    Fin_MSNetworkCapability msNetworkCapability;
    Fin_AddressString serviceCentre;
    Fin_RecordingEntity recordingEntity;
    Fin_LocationAreaCode locationArea;  /* optional; set in bit_mask
                                    * Fin_SGSNSMORecord_locationArea_present if
                                    * present */
    Fin_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                       * Fin_SGSNSMORecord_routingArea_present
                                       * if present */
    Fin_CellId      cellIdentity;  /* optional; set in bit_mask
                                    * Fin_SGSNSMORecord_cellIdentity_present if
                                    * present */
    Fin_MessageReference messageReference;
    Fin_TimeStamp   originationTime;
    Fin_SMSResult   smsResult;  /* optional; set in bit_mask
                                 * Fin_SGSNSMORecord_smsResult_present if
                                 * present */
    struct Fin_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                                   * Fin_SGSNSMORecord_recordExtensions_present
                                   * if present */
    Fin_NodeID      nodeID;  /* optional; set in bit_mask
                              * Fin_SGSNSMORecord_nodeID_present if present */
    Fin_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                             * Fin_SGSNSMORecord_localSequenceNumber_present if
                             * present */
} Fin_SGSNSMORecord;

typedef struct Fin_SGSNSMTRecord {
    unsigned short  bit_mask;
#       define      Fin_SGSNSMTRecord_servedIMEI_present 0x8000
#       define      Fin_SGSNSMTRecord_servedMSISDN_present 0x4000
#       define      Fin_SGSNSMTRecord_locationArea_present 0x2000
#       define      Fin_SGSNSMTRecord_routingArea_present 0x1000
#       define      Fin_SGSNSMTRecord_cellIdentity_present 0x0800
#       define      Fin_SGSNSMTRecord_smsResult_present 0x0400
#       define      Fin_SGSNSMTRecord_recordExtensions_present 0x0200
#       define      Fin_SGSNSMTRecord_nodeID_present 0x0100
#       define      Fin_SGSNSMTRecord_localSequenceNumber_present 0x0080
    Fin_CallEventRecordType recordType;
    Fin_IMSI        servedIMSI;
    Fin_IMEI        servedIMEI;  /* optional; set in bit_mask
                                  * Fin_SGSNSMTRecord_servedIMEI_present if
                                  * present */
    Fin_MSISDN      servedMSISDN;  /* optional; set in bit_mask
                                    * Fin_SGSNSMTRecord_servedMSISDN_present if
                                    * present */
    Fin_MSNetworkCapability msNetworkCapability;
    Fin_AddressString serviceCentre;
    Fin_RecordingEntity recordingEntity;
    Fin_LocationAreaCode locationArea;  /* optional; set in bit_mask
                                    * Fin_SGSNSMTRecord_locationArea_present if
                                    * present */
    Fin_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                       * Fin_SGSNSMTRecord_routingArea_present
                                       * if present */
    Fin_CellId      cellIdentity;  /* optional; set in bit_mask
                                    * Fin_SGSNSMTRecord_cellIdentity_present if
                                    * present */
    Fin_TimeStamp   originationTime;
    Fin_SMSResult   smsResult;  /* optional; set in bit_mask
                                 * Fin_SGSNSMTRecord_smsResult_present if
                                 * present */
    struct Fin_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                                   * Fin_SGSNSMTRecord_recordExtensions_present
                                   * if present */
    Fin_NodeID      nodeID;  /* optional; set in bit_mask
                              * Fin_SGSNSMTRecord_nodeID_present if present */
    Fin_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                             * Fin_SGSNSMTRecord_localSequenceNumber_present if
                             * present */
} Fin_SGSNSMTRecord;

typedef struct Fin_CallEventRecord {
    unsigned short  choice;
#       define      Fin_sgsnPDPRecord_chosen 1
#       define      Fin_ggsnPDPRecord_chosen 2
#       define      Fin_sgsnMMRecord_chosen 3
#       define      Fin_sgsnSMORecord_chosen 4
#       define      Fin_sgsnSMTRecord_chosen 5
    union _union {
        Fin_SGSNPDPRecord sgsnPDPRecord;  /* to choose, set choice to
                                           * Fin_sgsnPDPRecord_chosen */
        Fin_GGSNPDPRecord ggsnPDPRecord;  /* to choose, set choice to
                                           * Fin_ggsnPDPRecord_chosen */
        Fin_SGSNMMRecord sgsnMMRecord;  /* to choose, set choice to
                                         * Fin_sgsnMMRecord_chosen */
        Fin_SGSNSMORecord sgsnSMORecord;  /* to choose, set choice to
                                           * Fin_sgsnSMORecord_chosen */
        Fin_SGSNSMTRecord sgsnSMTRecord;  /* to choose, set choice to
                                           * Fin_sgsnSMTRecord_chosen */
    } u;
} Fin_CallEventRecord;

typedef struct Fin_ManagementExtensions_ {
    struct Fin_ManagementExtensions_ *next;
    Fin_ManagementExtension value;
} *Fin_ManagementExtensions;

typedef int             Fin_RecordNumber;

typedef struct Fin_RecordNumberList_ {
    struct Fin_RecordNumberList_ *next;
    Fin_RecordNumber value;
} *Fin_RecordNumberList;

extern int Fin_maxAddressLength;

extern int Fin_maxISDN_AddressLength;


extern void *CDRF_2GCDR_Fin;    /* encoder-decoder control table */
#ifdef __cplusplus
}	/* extern "C" */
#endif /* __cplusplus */
#endif /* OSS_CDRF_2GCDR_Fin */
