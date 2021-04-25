/********************************************************/
/* Copyright (C) 2003 OSS Nokalva.  All rights reserved.*/
/********************************************************/

/* THIS FILE IS PROPRIETARY MATERIAL OF OSS NOKALVA
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF OSS NOKALVA.
 * THIS FILE MAY NOT BE DISTRIBUTED. */

/* Generated for: HuaWei Technologies, Inc., Shenzhen, China - license 8088 for Windows NT */
/* Abstract syntax: CDRF_2GCDR_Org */
/* Created: Mon Mar 24 10:41:06 2003 */
/* ASN.1 compiler version: 5.3 */
/* Target operating system: Windows NT/Windows 9x */
/* Target machine type: Intel x86 */
/* C compiler options required: -Zp4 (Microsoft) */
/* ASN.1 compiler options and file names specified:
 * -codefile CDRF_2GCDR_Org.c -headerfile CDRF_2GCDR_Org.h -errorfile
 * C:\WINDOWS\TEMP\2. -prefix Org_ -dualheader -ansi -verbose -ber -root
 * D:\application\ossasn1\win32\5.3.0\asn1dflt\asn1dflt.ms.zp4
 * D:\project\530\asn1app\maimaoshi\CDRF_2GCDR_Org.asn
 */

#ifndef OSS_CDRF_2GCDR_Org
#define OSS_CDRF_2GCDR_Org

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "asn1hdr.h"
#include "asn1code.h"

#define          Org_CallEventRecord_PDU 1

typedef struct Org__ObjectID {
    unsigned short  length;
    unsigned char   *value;
} Org__ObjectID;

typedef struct Org__Any {
    unsigned long   length;
    unsigned char   *value;
} Org__Any;

typedef int             Org_CallEventRecordType;
#define                     Org_CallEventRecordType_sgsnPDPRecord 18
#define                     Org_CallEventRecordType_ggsnPDPRecord 19
#define                     Org_CallEventRecordType_sgsnMMRecord 20
#define                     Org_CallEventRecordType_sgsnSMORecord 21
#define                     Org_CallEventRecordType_sgsnSMTRecord 22

typedef ossBoolean      Org_NetworkInitiatedPDPContext;

typedef struct Org_IMSI {
    unsigned short  length;
    unsigned char   value[8];
} Org_IMSI;

typedef struct Org_IMEI {
    unsigned short  length;
    unsigned char   value[8];
} Org_IMEI;
struct Org__octet1 {
    unsigned short  length;
    unsigned char   value[4];
};
struct Org__octet2 {
    unsigned short  length;
    unsigned char   value[16];
};
        

typedef struct Org_IPBinaryAddress {
    unsigned short  choice;
#       define      Org_iPBinV4Address_chosen 1
#       define      Org_iPBinV6Address_chosen 2
    union _union {
        struct Org__octet1 iPBinV4Address;  /* to choose, set choice to
                            * Org_iPBinV4Address_chosen */
        struct Org__octet2 iPBinV6Address;  /* to choose, set choice to
                            * Org_iPBinV6Address_chosen */
    } u;
} Org_IPBinaryAddress;

typedef struct Org_IPTextRepresentedAddress {
    unsigned short  choice;
#       define      Org_iPTextV4Address_chosen 1
#       define      Org_iPTextV6Address_chosen 2
    union _union {
        char            iPTextV4Address[16];  /* to choose, set choice to
                                               * Org_iPTextV4Address_chosen */
        char            iPTextV6Address[46];  /* to choose, set choice to
                                               * Org_iPTextV6Address_chosen */
    } u;
} Org_IPTextRepresentedAddress;

typedef struct Org_IPAddress {
    unsigned short  choice;
#       define      Org_iPBinaryAddress_chosen 1
#       define      Org_iPTextRepresentedAddress_chosen 2
    union _union {
        Org_IPBinaryAddress iPBinaryAddress;  /* to choose, set choice to
                                               * Org_iPBinaryAddress_chosen */
        Org_IPTextRepresentedAddress iPTextRepresentedAddress;  /* to choose,
                                   * set choice to
                                   * Org_iPTextRepresentedAddress_chosen */
    } u;
} Org_IPAddress;

typedef Org_IPAddress   Org_GSNAddress;

typedef struct Org_MSNetworkCapability {
    unsigned short  length;
    unsigned char   value[1];
} Org_MSNetworkCapability;

typedef struct Org_RoutingAreaCode {
    unsigned short  length;
    unsigned char   value[1];
} Org_RoutingAreaCode;

typedef struct Org_LocationAreaCode {
    unsigned short  length;
    unsigned char   value[2];
} Org_LocationAreaCode;

typedef struct Org_CellId {
    unsigned short  length;
    unsigned char   value[2];
} Org_CellId;

typedef unsigned int    Org_ChargingID;

typedef char            Org_AccessPointNameNI[64];

typedef struct Org_PDPType {
    unsigned short  length;
    unsigned char   value[2];
} Org_PDPType;

typedef struct Org_AddressString {
    unsigned short  length;
    unsigned char   value[20];
} Org_AddressString;

typedef Org_AddressString Org_ETSIAddress;

typedef struct Org_PDPAddress {
    unsigned short  choice;
#       define      Org_iPAddress_chosen 1
#       define      Org_eTSIAddress_chosen 2
    union _union {
        Org_IPAddress   iPAddress;  /* to choose, set choice to
                                     * Org_iPAddress_chosen */
        Org_ETSIAddress eTSIAddress;  /* to choose, set choice to
                                       * Org_eTSIAddress_chosen */
    } u;
} Org_PDPAddress;

typedef enum Org_QoSReliability {
    Org_unspecifiedReliability = 0,
    Org_acknowledgedGTP = 1,
    Org_unackGTPAcknowLLC = 2,
    Org_unackGTPLLCAcknowRLC = 3,
    Org_unackGTPLLCRLC = 4,
    Org_unacknowUnprotectedData = 5
} Org_QoSReliability;

typedef enum Org_QoSDelay {
    Org_delayClass1 = 1,
    Org_delayClass2 = 2,
    Org_delayClass3 = 3,
    Org_delayClass4 = 4
} Org_QoSDelay;

typedef enum Org_QoSPrecedence {
    Org_QoSPrecedence_unspecified = 0,
    Org_highPriority = 1,
    Org_normalPriority = 2,
    Org_lowPriority = 3
} Org_QoSPrecedence;

typedef enum Org_QoSPeakThroughput {
    Org_QoSPeakThroughput_unspecified = 0,
    Org_upTo100OctetPs = 1,
    Org_upTo200OctetPs = 2,
    Org_upTo400OctetPs = 3,
    Org_upTo800OctetPs = 4,
    Org_upTo1600OctetPs = 5,
    Org_upTo3200OctetPs = 6,
    Org_upTo6400OctetPs = 7,
    Org_upTo12800OctetPs = 8,
    Org_upTo25600OctetPs = 9
} Org_QoSPeakThroughput;

typedef enum Org_QoSMeanThroughput {
    Org_bestEffort = 0,
    Org_mean100octetPh = 1,
    Org_mean200octetPh = 2,
    Org_mean500octetPh = 3,
    Org_mean1000octetPh = 4,
    Org_mean2000octetPh = 5,
    Org_mean5000octetPh = 6,
    Org_mean10000octetPh = 7,
    Org_mean20000octetPh = 8,
    Org_mean50000octetPh = 9,
    Org_mean100000octetPh = 10,
    Org_mean200000octetPh = 11,
    Org_mean500000octetPh = 12,
    Org_mean1000000octetPh = 13,
    Org_mean2000000octetPh = 14,
    Org_mean5000000octetPh = 15,
    Org_mean10000000octetPh = 16,
    Org_mean20000000octetPh = 17,
    Org_mean50000000octetPh = 18
} Org_QoSMeanThroughput;

typedef struct Org_QoSInformation {
    Org_QoSReliability reliability;
    Org_QoSDelay    delay;
    Org_QoSPrecedence precedence;
    Org_QoSPeakThroughput peakThroughput;
    Org_QoSMeanThroughput meanThroughput;
} Org_QoSInformation;

typedef int             Org_DataVolumeGPRS;

typedef enum Org_ChangeCondition {
    Org_qoSChange = 0,
    Org_tariffTime = 1,
    Org_recordClosure = 2
} Org_ChangeCondition;

typedef struct Org_TimeStamp {
    unsigned short  length;
    unsigned char   value[9];
} Org_TimeStamp;

typedef struct Org_ChangeOfCharCondition {
    unsigned char   bit_mask;
#       define      Org_qosRequested_present 0x80
#       define      Org_qosNegotiated_present 0x40
    Org_QoSInformation qosRequested;  /* optional; set in bit_mask
                                       * Org_qosRequested_present if present */
    Org_QoSInformation qosNegotiated;  /* optional; set in bit_mask
                                        * Org_qosNegotiated_present if
                                        * present */
    Org_DataVolumeGPRS dataVolumeGPRSUplink;
    Org_DataVolumeGPRS dataVolumeGPRSDownlink;
    Org_ChangeCondition changeCondition;
    Org_TimeStamp   changeTime;
} Org_ChangeOfCharCondition;

typedef int             Org_CallDuration;

typedef ossBoolean      Org_SGSNChange;

typedef int             Org_CauseForRecClosing;
#define                     Org_normalRelease 0
#define                     Org_abnormalRelease 4
#define                     Org_volumeLimit 16
#define                     Org_timeLimit 17
#define                     Org_sGSNChange 18
#define                     Org_maxChangeCond 19
#define                     Org_managementIntervention 20

typedef struct Org_ManagementExtension {
    unsigned char   bit_mask;
#       define      Org_significance_present 0x80
    Org__ObjectID   identifier;
    ossBoolean      significance;  /* Org_significance_present not set in
                                    * bit_mask implies value is FALSE */
    Org__Any        information;
} Org_ManagementExtension;

typedef struct Org_Diagnostics {
    unsigned short  choice;
#       define      Org_gsm0408Cause_chosen 1
#       define      Org_gsm0902MapErrorValue_chosen 2
#       define      Org_ccittQ767Cause_chosen 3
#       define      Org_networkSpecificCause_chosen 4
#       define      Org_manufacturerSpecificCause_chosen 5
    union _union {
        int             gsm0408Cause;  /* to choose, set choice to
                                        * Org_gsm0408Cause_chosen */
        int             gsm0902MapErrorValue;  /* to choose, set choice to
                                           * Org_gsm0902MapErrorValue_chosen */
        int             ccittQ767Cause;  /* to choose, set choice to
                                          * Org_ccittQ767Cause_chosen */
        Org_ManagementExtension networkSpecificCause;  /* to choose, set choice
                                        * to Org_networkSpecificCause_chosen */
        Org_ManagementExtension manufacturerSpecificCause;  /* to choose, set
                                   * choice to
                                   * Org_manufacturerSpecificCause_chosen */
    } u;
} Org_Diagnostics;

typedef char            Org_NodeID[21];

typedef unsigned int    Org_LocalSequenceNumber;

typedef enum Org_APNSelectionMode {
    Org_mSorNetworkProvidedSubscriptionVerified = 0,
    Org_mSProvidedSubscriptionNotVerified = 1,
    Org_networkProvidedSubscriptionNotVerified = 2
} Org_APNSelectionMode;

typedef char            Org_AccessPointNameOI[38];

typedef struct Org_ISDN_AddressString {
    unsigned short  length;
    unsigned char   value[10];
} Org_ISDN_AddressString;

typedef Org_ISDN_AddressString Org_MSISDN;

struct Org__seqof1_ {
    struct Org__seqof1_ *next;
    Org_ChangeOfCharCondition value;
};

typedef struct Org_SGSNPDPRecord {
    unsigned short  bit_mask;
#       define      Org_SGSNPDPRecord_networkInitiation_present 0x8000
#       define      Org_SGSNPDPRecord_anonymousAccessIndicator_present 0x4000
#       define      Org_SGSNPDPRecord_servedIMEI_present 0x2000
#       define      Org_SGSNPDPRecord_msNetworkCapability_present 0x1000
#       define      Org_SGSNPDPRecord_routingArea_present 0x0800
#       define      Org_SGSNPDPRecord_locationAreaCode_present 0x0400
#       define      Org_SGSNPDPRecord_cellIdentity_present 0x0200
#       define      Org_SGSNPDPRecord_sgsnChange_present 0x0100
#       define      Org_SGSNPDPRecord_diagnostics_present 0x0080
#       define      Org_SGSNPDPRecord_recordSequenceNumber_present 0x0040
#       define      Org_SGSNPDPRecord_nodeID_present 0x0020
#       define      Org_SGSNPDPRecord_recordExtensions_present 0x0010
#       define      Org_SGSNPDPRecord_localSequenceNumber_present 0x0008
#       define      Org_SGSNPDPRecord_apnSelectionMode_present 0x0004
#       define      Org_SGSNPDPRecord_servedMSISDN_present 0x0002
    Org_CallEventRecordType recordType;
    Org_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                                   * Org_SGSNPDPRecord_networkInitiation_present
                                   * if present */
    ossBoolean      anonymousAccessIndicator;  /* optional; set in bit_mask
                        * Org_SGSNPDPRecord_anonymousAccessIndicator_present if
                        * present */
    Org_IMSI        servedIMSI;
    Org_IMEI        servedIMEI;  /* optional; set in bit_mask
                                  * Org_SGSNPDPRecord_servedIMEI_present if
                                  * present */
    Org_GSNAddress  sgsnAddress;
    Org_MSNetworkCapability msNetworkCapability;  /* optional; set in bit_mask
                             * Org_SGSNPDPRecord_msNetworkCapability_present if
                             * present */
    Org_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                       * Org_SGSNPDPRecord_routingArea_present
                                       * if present */
    Org_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                                * Org_SGSNPDPRecord_locationAreaCode_present if
                                * present */
    Org_CellId      cellIdentity;  /* optional; set in bit_mask
                                    * Org_SGSNPDPRecord_cellIdentity_present if
                                    * present */
    Org_ChargingID  chargingID;
    Org_GSNAddress  ggsnAddressUsed;
    Org_AccessPointNameNI accessPointNameNI;
    Org_PDPType     pdpType;
    Org_PDPAddress  servedPDPAddress;
    struct Org__seqof1_ *listOfTrafficVolumes;
    Org_TimeStamp   recordOpeningTime;
    Org_CallDuration duration;
    Org_SGSNChange  sgsnChange;  /* optional; set in bit_mask
                                  * Org_SGSNPDPRecord_sgsnChange_present if
                                  * present */
    Org_CauseForRecClosing causeForRecClosing;
    Org_Diagnostics diagnostics;  /* optional; set in bit_mask
                                   * Org_SGSNPDPRecord_diagnostics_present if
                                   * present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                            * Org_SGSNPDPRecord_recordSequenceNumber_present if
                            * present */
    Org_NodeID      nodeID;  /* optional; set in bit_mask
                              * Org_SGSNPDPRecord_nodeID_present if present */
    struct Org_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                                   * Org_SGSNPDPRecord_recordExtensions_present
                                   * if present */
    Org_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                             * Org_SGSNPDPRecord_localSequenceNumber_present if
                             * present */
    Org_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                                * Org_SGSNPDPRecord_apnSelectionMode_present if
                                * present */
    Org_AccessPointNameOI accessPointNameOI;
    Org_MSISDN      servedMSISDN;  /* optional; set in bit_mask
                                    * Org_SGSNPDPRecord_servedMSISDN_present if
                                    * present */
} Org_SGSNPDPRecord;

typedef ossBoolean      Org_DynamicAddressFlag;

struct Org__seqof2 {
    struct Org__seqof2 *next;
    Org_GSNAddress  value;
};

struct Org__seqof3 {
    struct Org__seqof3 *next;
    Org_PDPAddress  value;
};
    

typedef struct Org_GGSNPDPRecord {
    unsigned short  bit_mask;
#       define      Org_GGSNPDPRecord_networkInitiation_present 0x8000
#       define      Org_GGSNPDPRecord_anonymousAccessIndicator_present 0x4000
#       define      Org_remotePDPAddress_present 0x2000
#       define      Org_dynamicAddressFlag_present 0x1000
#       define      Org_GGSNPDPRecord_diagnostics_present 0x0800
#       define      Org_GGSNPDPRecord_recordSequenceNumber_present 0x0400
#       define      Org_GGSNPDPRecord_nodeID_present 0x0200
#       define      Org_GGSNPDPRecord_recordExtensions_present 0x0100
#       define      Org_GGSNPDPRecord_localSequenceNumber_present 0x0080
#       define      Org_GGSNPDPRecord_apnSelectionMode_present 0x0040
#       define      Org_GGSNPDPRecord_servedMSISDN_present 0x0020
    Org_CallEventRecordType recordType;
    Org_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                                   * Org_GGSNPDPRecord_networkInitiation_present
                                   * if present */
    ossBoolean      anonymousAccessIndicator;  /* optional; set in bit_mask
                        * Org_GGSNPDPRecord_anonymousAccessIndicator_present if
                        * present */
    Org_IMSI        servedIMSI;
    Org_GSNAddress  ggsnAddress;
    Org_ChargingID  chargingID;
    struct Org__seqof2 *sgsnAddress;
    Org_AccessPointNameNI accessPointNameNI;
    Org_PDPType     pdpType;
    Org_PDPAddress  servedPDPAddress;
    struct Org__seqof3 *remotePDPAddress;  /* optional; set in bit_mask
                           * Org_remotePDPAddress_present if present */
    Org_DynamicAddressFlag dynamicAddressFlag;  /* optional; set in bit_mask
                                            * Org_dynamicAddressFlag_present if
                                            * present */
    struct Org__seqof1_ *listOfTrafficVolumes;
    Org_TimeStamp   recordOpeningTime;
    Org_CallDuration duration;
    Org_CauseForRecClosing causeForRecClosing;
    Org_Diagnostics diagnostics;  /* optional; set in bit_mask
                                   * Org_GGSNPDPRecord_diagnostics_present if
                                   * present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                            * Org_GGSNPDPRecord_recordSequenceNumber_present if
                            * present */
    Org_NodeID      nodeID;  /* optional; set in bit_mask
                              * Org_GGSNPDPRecord_nodeID_present if present */
    struct Org_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                                   * Org_GGSNPDPRecord_recordExtensions_present
                                   * if present */
    Org_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                             * Org_GGSNPDPRecord_localSequenceNumber_present if
                             * present */
    Org_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                                * Org_GGSNPDPRecord_apnSelectionMode_present if
                                * present */
    Org_MSISDN      servedMSISDN;  /* optional; set in bit_mask
                                    * Org_GGSNPDPRecord_servedMSISDN_present if
                                    * present */
} Org_GGSNPDPRecord;

typedef struct Org_ChangeLocation {
    unsigned char   bit_mask;
#       define      Org_cellId_present 0x80
    Org_LocationAreaCode locationAreaCode;
    Org_RoutingAreaCode routingAreaCode;
    Org_CellId      cellId;  /* optional; set in bit_mask Org_cellId_present if
                              * present */
    Org_TimeStamp   changeTime;
} Org_ChangeLocation;

struct Org__seqof4 {
    struct Org__seqof4 *next;
    Org_ChangeLocation value;
};

typedef struct Org_SGSNMMRecord {
    unsigned short  bit_mask;
#       define      Org_SGSNMMRecord_servedIMEI_present 0x8000
#       define      Org_SGSNMMRecord_msNetworkCapability_present 0x4000
#       define      Org_SGSNMMRecord_routingArea_present 0x2000
#       define      Org_SGSNMMRecord_locationAreaCode_present 0x1000
#       define      Org_SGSNMMRecord_cellIdentity_present 0x0800
#       define      Org_changeLocation_present 0x0400
#       define      Org_duration_present 0x0200
#       define      Org_SGSNMMRecord_sgsnChange_present 0x0100
#       define      Org_SGSNMMRecord_diagnostics_present 0x0080
#       define      Org_SGSNMMRecord_recordSequenceNumber_present 0x0040
#       define      Org_SGSNMMRecord_nodeID_present 0x0020
#       define      Org_SGSNMMRecord_recordExtensions_present 0x0010
#       define      Org_SGSNMMRecord_localSequenceNumber_present 0x0008
    Org_CallEventRecordType recordType;
    Org_IMSI        servedIMSI;
    Org_IMEI        servedIMEI;  /* optional; set in bit_mask
                                  * Org_SGSNMMRecord_servedIMEI_present if
                                  * present */
    Org_GSNAddress  sgsnAddress;
    Org_MSNetworkCapability msNetworkCapability;  /* optional; set in bit_mask
                              * Org_SGSNMMRecord_msNetworkCapability_present if
                              * present */
    Org_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                       * Org_SGSNMMRecord_routingArea_present if
                                       * present */
    Org_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                                 * Org_SGSNMMRecord_locationAreaCode_present if
                                 * present */
    Org_CellId      cellIdentity;  /* optional; set in bit_mask
                                    * Org_SGSNMMRecord_cellIdentity_present if
                                    * present */
    struct Org__seqof4 *changeLocation;  /* optional; set in bit_mask Org_changeLocation_present
                         * if present */
    Org_TimeStamp   recordOpeningTime;
    Org_CallDuration duration;  /* optional; set in bit_mask
                                 * Org_duration_present if present */
    Org_SGSNChange  sgsnChange;  /* optional; set in bit_mask
                                  * Org_SGSNMMRecord_sgsnChange_present if
                                  * present */
    Org_CauseForRecClosing causeForRecClosing;
    Org_Diagnostics diagnostics;  /* optional; set in bit_mask
                                   * Org_SGSNMMRecord_diagnostics_present if
                                   * present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                             * Org_SGSNMMRecord_recordSequenceNumber_present if
                             * present */
    Org_NodeID      nodeID;  /* optional; set in bit_mask
                              * Org_SGSNMMRecord_nodeID_present if present */
    struct Org_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                                   * Org_SGSNMMRecord_recordExtensions_present
                                   * if present */
    Org_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                              * Org_SGSNMMRecord_localSequenceNumber_present if
                              * present */
} Org_SGSNMMRecord;

typedef Org_AddressString Org_RecordingEntity;

typedef struct Org_MessageReference {
    unsigned short  length;
    unsigned char   value[9];
} Org_MessageReference;

typedef Org_Diagnostics Org_SMSResult;

typedef struct Org_SGSNSMORecord {
    unsigned short  bit_mask;
#       define      Org_SGSNSMORecord_servedIMEI_present 0x8000
#       define      Org_SGSNSMORecord_servedMSISDN_present 0x4000
#       define      Org_SGSNSMORecord_locationArea_present 0x2000
#       define      Org_SGSNSMORecord_routingArea_present 0x1000
#       define      Org_SGSNSMORecord_cellIdentity_present 0x0800
#       define      Org_SGSNSMORecord_smsResult_present 0x0400
#       define      Org_SGSNSMORecord_recordExtensions_present 0x0200
#       define      Org_SGSNSMORecord_nodeID_present 0x0100
#       define      Org_SGSNSMORecord_localSequenceNumber_present 0x0080
    Org_CallEventRecordType recordType;
    Org_IMSI        servedIMSI;
    Org_IMEI        servedIMEI;  /* optional; set in bit_mask
                                  * Org_SGSNSMORecord_servedIMEI_present if
                                  * present */
    Org_MSISDN      servedMSISDN;  /* optional; set in bit_mask
                                    * Org_SGSNSMORecord_servedMSISDN_present if
                                    * present */
    Org_MSNetworkCapability msNetworkCapability;
    Org_AddressString serviceCentre;
    Org_RecordingEntity recordingEntity;
    Org_LocationAreaCode locationArea;  /* optional; set in bit_mask
                                    * Org_SGSNSMORecord_locationArea_present if
                                    * present */
    Org_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                       * Org_SGSNSMORecord_routingArea_present
                                       * if present */
    Org_CellId      cellIdentity;  /* optional; set in bit_mask
                                    * Org_SGSNSMORecord_cellIdentity_present if
                                    * present */
    Org_MessageReference messageReference;
    Org_TimeStamp   originationTime;
    Org_SMSResult   smsResult;  /* optional; set in bit_mask
                                 * Org_SGSNSMORecord_smsResult_present if
                                 * present */
    struct Org_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                                   * Org_SGSNSMORecord_recordExtensions_present
                                   * if present */
    Org_NodeID      nodeID;  /* optional; set in bit_mask
                              * Org_SGSNSMORecord_nodeID_present if present */
    Org_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                             * Org_SGSNSMORecord_localSequenceNumber_present if
                             * present */
} Org_SGSNSMORecord;

typedef struct Org_SGSNSMTRecord {
    unsigned short  bit_mask;
#       define      Org_SGSNSMTRecord_servedIMEI_present 0x8000
#       define      Org_SGSNSMTRecord_servedMSISDN_present 0x4000
#       define      Org_SGSNSMTRecord_locationArea_present 0x2000
#       define      Org_SGSNSMTRecord_routingArea_present 0x1000
#       define      Org_SGSNSMTRecord_cellIdentity_present 0x0800
#       define      Org_SGSNSMTRecord_smsResult_present 0x0400
#       define      Org_SGSNSMTRecord_recordExtensions_present 0x0200
#       define      Org_SGSNSMTRecord_nodeID_present 0x0100
#       define      Org_SGSNSMTRecord_localSequenceNumber_present 0x0080
    Org_CallEventRecordType recordType;
    Org_IMSI        servedIMSI;
    Org_IMEI        servedIMEI;  /* optional; set in bit_mask
                                  * Org_SGSNSMTRecord_servedIMEI_present if
                                  * present */
    Org_MSISDN      servedMSISDN;  /* optional; set in bit_mask
                                    * Org_SGSNSMTRecord_servedMSISDN_present if
                                    * present */
    Org_MSNetworkCapability msNetworkCapability;
    Org_AddressString serviceCentre;
    Org_RecordingEntity recordingEntity;
    Org_LocationAreaCode locationArea;  /* optional; set in bit_mask
                                    * Org_SGSNSMTRecord_locationArea_present if
                                    * present */
    Org_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                       * Org_SGSNSMTRecord_routingArea_present
                                       * if present */
    Org_CellId      cellIdentity;  /* optional; set in bit_mask
                                    * Org_SGSNSMTRecord_cellIdentity_present if
                                    * present */
    Org_TimeStamp   originationTime;
    Org_SMSResult   smsResult;  /* optional; set in bit_mask
                                 * Org_SGSNSMTRecord_smsResult_present if
                                 * present */
    struct Org_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                                   * Org_SGSNSMTRecord_recordExtensions_present
                                   * if present */
    Org_NodeID      nodeID;  /* optional; set in bit_mask
                              * Org_SGSNSMTRecord_nodeID_present if present */
    Org_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                             * Org_SGSNSMTRecord_localSequenceNumber_present if
                             * present */
} Org_SGSNSMTRecord;

typedef struct Org_CallEventRecord {
    unsigned short  choice;
#       define      Org_sgsnPDPRecord_chosen 1
#       define      Org_ggsnPDPRecord_chosen 2
#       define      Org_sgsnMMRecord_chosen 3
#       define      Org_sgsnSMORecord_chosen 4
#       define      Org_sgsnSMTRecord_chosen 5
    union _union {
        Org_SGSNPDPRecord sgsnPDPRecord;  /* to choose, set choice to
                                           * Org_sgsnPDPRecord_chosen */
        Org_GGSNPDPRecord ggsnPDPRecord;  /* to choose, set choice to
                                           * Org_ggsnPDPRecord_chosen */
        Org_SGSNMMRecord sgsnMMRecord;  /* to choose, set choice to
                                         * Org_sgsnMMRecord_chosen */
        Org_SGSNSMORecord sgsnSMORecord;  /* to choose, set choice to
                                           * Org_sgsnSMORecord_chosen */
        Org_SGSNSMTRecord sgsnSMTRecord;  /* to choose, set choice to
                                           * Org_sgsnSMTRecord_chosen */
    } u;
} Org_CallEventRecord;

typedef struct Org_ManagementExtensions_ {
    struct Org_ManagementExtensions_ *next;
    Org_ManagementExtension value;
} *Org_ManagementExtensions;

extern Org__ObjectID Org_gsm1205InformationModel;

extern Org__ObjectID Org_gsm1205ASN1Module;

extern Org__ObjectID Org_gsm1205ManagedObjectClass;

extern Org__ObjectID Org_gsm1205Package;

extern Org__ObjectID Org_gsm1205NameBinding;

extern Org__ObjectID Org_gsm1205Attribute;

extern Org__ObjectID Org_gsm1205Action;

extern Org__ObjectID Org_gsm1205Notification;

extern int Org_maxAddressLength;

extern int Org_maxISDN_AddressLength;


extern void *CDRF_2GCDR_Org;    /* encoder-decoder control table */
#ifdef __cplusplus
}	/* extern "C" */
#endif /* __cplusplus */
#endif /* OSS_CDRF_2GCDR_Org */
