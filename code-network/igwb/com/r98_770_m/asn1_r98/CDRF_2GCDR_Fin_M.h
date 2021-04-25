/********************************************************/
/* Copyright (C) 2003 OSS Nokalva.  All rights reserved.*/
/********************************************************/

/* THIS FILE IS PROPRIETARY MATERIAL OF OSS NOKALVA
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF OSS NOKALVA.
 * THIS FILE MAY NOT BE DISTRIBUTED. */

/* Generated for: HuaWei Technologies, Inc., Shenzhen, China - license 8088 for Windows NT */
/* Abstract syntax: CDRF_R98_Fin_M */
/* Created: Fri Dec 19 10:58:59 2003 */
/* ASN.1 compiler version: 5.3 */
/* Target operating system: Windows NT/Windows 9x */
/* Target machine type: Intel x86 */
/* C compiler options required: -Zp4 (Microsoft) */
/* ASN.1 compiler options and file names specified:
 * -codefile CDRF_2GCDR_Fin_M.c -headerfile CDRF_2GCDR_Fin_M.h -errorfile
 * C:\WINDOWS\TEMP\2. -prefix Fin_M_ -dualheader -ansi -verbose -ber -root
 * D:\application\ossasn1\win32\5.3.0\asn1dflt\asn1dflt.ms.zp4
 * D:\yutemp\lidufang\1218\R98\r98_fin_m\CDRF_R98_Fin_M.asn
 */

#ifndef OSS_CDRF_R98_Fin_M
#define OSS_CDRF_R98_Fin_M

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "asn1hdr.h"
#include "asn1code.h"

#define          Fin_M_BSRecord_PDU 1

typedef struct Fin_M__ObjectID {
    unsigned short  length;
    unsigned char   *value;
} Fin_M__ObjectID;

typedef struct Fin_M__octet1 {
    unsigned short  length;
    unsigned char   value[15];
} Fin_M__octet1;

typedef struct Fin_M__octet3 {
    unsigned short  length;
    unsigned char   value[14];
} Fin_M__octet3;

typedef struct Fin_M__octet4 {
    unsigned short  length;
    unsigned char   value[10];
} Fin_M__octet4;

typedef struct Fin_M_BSXferSummaryRecord {
    Fin_M__octet1   cgnID;
    struct Fin_M__octet2 {
        unsigned short  length;
        unsigned char   value[34];
    } bsXferFile;
    Fin_M__octet3   openTime;
    Fin_M__octet3   closeTime;
    Fin_M__octet4   lrsnStart;
    Fin_M__octet4   lrsnEnd;
    struct Fin_M__octet5 {
        unsigned short  length;
        unsigned char   value[3];
    } cdrType;
    Fin_M__octet4   totalCdr;
    Fin_M__octet4   missingCdr;
    Fin_M__octet4   duplicateCdr;
    Fin_M__octet1   cgID;
} Fin_M_BSXferSummaryRecord;

typedef int             Fin_M_CallEventRecordType;
#define                     Fin_M_CallEventRecordType_sgsnPDPRecord 18
#define                     Fin_M_CallEventRecordType_ggsnPDPRecord 19
#define                     Fin_M_CallEventRecordType_sgsnMMRecord 20
#define                     Fin_M_CallEventRecordType_sgsnSMORecord 21
#define                     Fin_M_CallEventRecordType_sgsnSMTRecord 22

typedef ossBoolean      Fin_M_NetworkInitiatedPDPContext;

typedef struct Fin_M_IMSI {
    unsigned short  length;
    unsigned char   value[8];
} Fin_M_IMSI;

typedef struct Fin_M_IMEI {
    unsigned short  length;
    unsigned char   value[8];
} Fin_M_IMEI;

typedef struct Fin_M_IPBinaryAddress {
    unsigned short  choice;
#       define      Fin_M_iPBinV4Address_chosen 1
#       define      Fin_M_iPBinV6Address_chosen 2
    union _union {
        struct Fin_M__octet6 {
            unsigned short  length;
            unsigned char   value[4];
        } iPBinV4Address;  /* to choose, set choice to
                            * Fin_M_iPBinV4Address_chosen */
        struct Fin_M__octet7 {
            unsigned short  length;
            unsigned char   value[16];
        } iPBinV6Address;  /* to choose, set choice to
                            * Fin_M_iPBinV6Address_chosen */
    } u;
} Fin_M_IPBinaryAddress;

typedef struct Fin_M_IPTextRepresentedAddress {
    unsigned short  choice;
#       define      Fin_M_iPTextV4Address_chosen 1
#       define      Fin_M_iPTextV6Address_chosen 2
    union _union {
        char            iPTextV4Address[16];  /* to choose, set choice to
                                              * Fin_M_iPTextV4Address_chosen */
        char            iPTextV6Address[46];  /* to choose, set choice to
                                              * Fin_M_iPTextV6Address_chosen */
    } u;
} Fin_M_IPTextRepresentedAddress;

typedef struct Fin_M_IPAddress {
    unsigned short  choice;
#       define      Fin_M_iPBinaryAddress_chosen 1
#       define      Fin_M_iPTextRepresentedAddress_chosen 2
    union _union {
        Fin_M_IPBinaryAddress iPBinaryAddress;  /* to choose, set choice to
                                              * Fin_M_iPBinaryAddress_chosen */
        Fin_M_IPTextRepresentedAddress iPTextRepresentedAddress;  /* to choose,
                                   * set choice to
                                   * Fin_M_iPTextRepresentedAddress_chosen */
    } u;
} Fin_M_IPAddress;

typedef Fin_M_IPAddress Fin_M_GSNAddress;

typedef struct Fin_M_MSNetworkCapability {
    unsigned short  length;
    unsigned char   value[1];
} Fin_M_MSNetworkCapability;

typedef struct Fin_M_RoutingAreaCode {
    unsigned short  length;
    unsigned char   value[1];
} Fin_M_RoutingAreaCode;

typedef struct Fin_M_LocationAreaCode {
    unsigned short  length;
    unsigned char   value[2];
} Fin_M_LocationAreaCode;

typedef struct Fin_M_CellId {
    unsigned short  length;
    unsigned char   value[2];
} Fin_M_CellId;

typedef unsigned int    Fin_M_ChargingID;

typedef char            Fin_M_AccessPointNameNI[64];

typedef struct Fin_M_PDPType {
    unsigned short  length;
    unsigned char   value[2];
} Fin_M_PDPType;

typedef struct Fin_M_AddressString {
    unsigned short  length;
    unsigned char   value[20];
} Fin_M_AddressString;

typedef Fin_M_AddressString Fin_M_ETSIAddress;

typedef struct Fin_M_PDPAddress {
    unsigned short  choice;
#       define      Fin_M_iPAddress_chosen 1
#       define      Fin_M_eTSIAddress_chosen 2
    union _union {
        Fin_M_IPAddress iPAddress;  /* to choose, set choice to
                                     * Fin_M_iPAddress_chosen */
        Fin_M_ETSIAddress eTSIAddress;  /* to choose, set choice to
                                         * Fin_M_eTSIAddress_chosen */
    } u;
} Fin_M_PDPAddress;

typedef enum Fin_M_QoSReliability {
    Fin_M_unspecifiedReliability = 0,
    Fin_M_acknowledgedGTP = 1,
    Fin_M_unackGTPAcknowLLC = 2,
    Fin_M_unackGTPLLCAcknowRLC = 3,
    Fin_M_unackGTPLLCRLC = 4,
    Fin_M_unacknowUnprotectedData = 5
} Fin_M_QoSReliability;

typedef enum Fin_M_QoSDelay {
    Fin_M_delayClass1 = 1,
    Fin_M_delayClass2 = 2,
    Fin_M_delayClass3 = 3,
    Fin_M_delayClass4 = 4
} Fin_M_QoSDelay;

typedef enum Fin_M_QoSPrecedence {
    Fin_M_QoSPrecedence_unspecified = 0,
    Fin_M_highPriority = 1,
    Fin_M_normalPriority = 2,
    Fin_M_lowPriority = 3
} Fin_M_QoSPrecedence;

typedef enum Fin_M_QoSPeakThroughput {
    Fin_M_QoSPeakThroughput_unspecified = 0,
    Fin_M_upTo100OctetPs = 1,
    Fin_M_upTo200OctetPs = 2,
    Fin_M_upTo400OctetPs = 3,
    Fin_M_upTo800OctetPs = 4,
    Fin_M_upTo1600OctetPs = 5,
    Fin_M_upTo3200OctetPs = 6,
    Fin_M_upTo6400OctetPs = 7,
    Fin_M_upTo12800OctetPs = 8,
    Fin_M_upTo25600OctetPs = 9
} Fin_M_QoSPeakThroughput;

typedef enum Fin_M_QoSMeanThroughput {
    Fin_M_bestEffort = 0,
    Fin_M_mean100octetPh = 1,
    Fin_M_mean200octetPh = 2,
    Fin_M_mean500octetPh = 3,
    Fin_M_mean1000octetPh = 4,
    Fin_M_mean2000octetPh = 5,
    Fin_M_mean5000octetPh = 6,
    Fin_M_mean10000octetPh = 7,
    Fin_M_mean20000octetPh = 8,
    Fin_M_mean50000octetPh = 9,
    Fin_M_mean100000octetPh = 10,
    Fin_M_mean200000octetPh = 11,
    Fin_M_mean500000octetPh = 12,
    Fin_M_mean1000000octetPh = 13,
    Fin_M_mean2000000octetPh = 14,
    Fin_M_mean5000000octetPh = 15,
    Fin_M_mean10000000octetPh = 16,
    Fin_M_mean20000000octetPh = 17,
    Fin_M_mean50000000octetPh = 18
} Fin_M_QoSMeanThroughput;

typedef struct Fin_M_QoSInformation {
    Fin_M_QoSReliability reliability;
    Fin_M_QoSDelay  delay;
    Fin_M_QoSPrecedence precedence;
    Fin_M_QoSPeakThroughput peakThroughput;
    Fin_M_QoSMeanThroughput meanThroughput;
} Fin_M_QoSInformation;

typedef int             Fin_M_DataVolumeGPRS;

typedef enum Fin_M_ChangeCondition {
    Fin_M_qoSChange = 0,
    Fin_M_tariffTime = 1,
    Fin_M_recordClosure = 2
} Fin_M_ChangeCondition;

typedef struct Fin_M_TimeStamp {
    unsigned short  length;
    unsigned char   value[9];
} Fin_M_TimeStamp;

typedef struct Fin_M_ChangeOfCharCondition {
    unsigned char   bit_mask;
#       define      Fin_M_qosRequested_present 0x80
#       define      Fin_M_qosNegotiated_present 0x40
    Fin_M_QoSInformation qosRequested;  /* optional; set in bit_mask
                                         * Fin_M_qosRequested_present if
                                         * present */
    Fin_M_QoSInformation qosNegotiated;  /* optional; set in bit_mask
                                          * Fin_M_qosNegotiated_present if
                                          * present */
    Fin_M_DataVolumeGPRS dataVolumeGPRSUplink;
    Fin_M_DataVolumeGPRS dataVolumeGPRSDownlink;
    Fin_M_ChangeCondition changeCondition;
    Fin_M_TimeStamp changeTime;
} Fin_M_ChangeOfCharCondition;

typedef int             Fin_M_CallDuration;

typedef ossBoolean      Fin_M_SGSNChange;

typedef int             Fin_M_CauseForRecClosing;
#define                     Fin_M_normalRelease 0
#define                     Fin_M_abnormalRelease 4
#define                     Fin_M_volumeLimit 16
#define                     Fin_M_timeLimit 17
#define                     Fin_M_sGSNChange 18
#define                     Fin_M_maxChangeCond 19
#define                     Fin_M_managementIntervention 20

typedef struct Fin_M_RECORDSequenceNumber {
    Fin_M_GSNAddress gsnAddress;
    struct Fin_M_RecordNumberList_ *recordNumberList;
} Fin_M_RECORDSequenceNumber;

typedef char            Fin_M_NodeID[21];

typedef enum Fin_M_APNSelectionMode {
    Fin_M_mSorNetworkProvidedSubscriptionVerified = 0,
    Fin_M_mSProvidedSubscriptionNotVerified = 1,
    Fin_M_networkProvidedSubscriptionNotVerified = 2
} Fin_M_APNSelectionMode;

typedef char            Fin_M_AccessPointNameOI[38];

typedef struct Fin_M_ISDN_AddressString {
    unsigned short  length;
    unsigned char   value[10];
} Fin_M_ISDN_AddressString;

typedef Fin_M_ISDN_AddressString Fin_M_MSISDN;

typedef enum Fin_M_ConsolidationResult {
    Fin_M_normal = 0,
    Fin_M_notNormal = 1,
    Fin_M_forInterSGSNConsolidation = 2,
    Fin_M_reachLimit = 3,
    Fin_M_onlyOneCDRGenerated = 4
} Fin_M_ConsolidationResult;

typedef struct Fin_M__seqof1_ {
    struct Fin_M__seqof1_ *next;
    Fin_M_GSNAddress value;
} *Fin_M__seqof1;

typedef struct Fin_M__seqof2_ {
    struct Fin_M__seqof2_ *next;
    Fin_M_ChangeOfCharCondition value;
} *Fin_M__seqof2;

struct Fin_M__seqof3 {
    struct Fin_M__seqof3 *next;
    Fin_M_RECORDSequenceNumber value;
};

typedef struct Fin_M_SGSNPDPRecord {
    unsigned short  bit_mask;
#       define      Fin_M_SGSNPDPRecord_networkInitiation_present 0x8000
#       define      Fin_M_SGSNPDPRecord_anonymousAccessIndicator_present 0x4000
#       define      Fin_M_SGSNPDPRecord_servedIMEI_present 0x2000
#       define      Fin_M_SGSNPDPRecord_msNetworkCapability_present 0x1000
#       define      Fin_M_SGSNPDPRecord_routingArea_present 0x0800
#       define      Fin_M_SGSNPDPRecord_locationAreaCode_present 0x0400
#       define      Fin_M_SGSNPDPRecord_cellIdentity_present 0x0200
#       define      Fin_M_SGSNPDPRecord_sgsnChange_present 0x0100
#       define      Fin_M_SGSNPDPRecord_recordSequenceNumber_present 0x0080
#       define      Fin_M_SGSNPDPRecord_nodeID_present 0x0040
#       define      Fin_M_SGSNPDPRecord_recordExtensions_present 0x0020
#       define      Fin_M_SGSNPDPRecord_apnSelectionMode_present 0x0010
#       define      Fin_M_SGSNPDPRecord_servedMSISDN_present 0x0008
    Fin_M_CallEventRecordType recordType;
    Fin_M_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                             * Fin_M_SGSNPDPRecord_networkInitiation_present if
                             * present */
    ossBoolean      anonymousAccessIndicator;  /* optional; set in bit_mask
                      * Fin_M_SGSNPDPRecord_anonymousAccessIndicator_present if
                      * present */
    Fin_M_IMSI      servedIMSI;
    Fin_M_IMEI      servedIMEI;  /* optional; set in bit_mask
                                  * Fin_M_SGSNPDPRecord_servedIMEI_present if
                                  * present */
    struct Fin_M__seqof1_ *sgsnAddress;
    Fin_M_MSNetworkCapability msNetworkCapability;  /* optional; set in bit_mask
                           * Fin_M_SGSNPDPRecord_msNetworkCapability_present if
                           * present */
    Fin_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                   * Fin_M_SGSNPDPRecord_routingArea_present if
                                   * present */
    Fin_M_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                              * Fin_M_SGSNPDPRecord_locationAreaCode_present if
                              * present */
    Fin_M_CellId    cellIdentity;  /* optional; set in bit_mask
                                    * Fin_M_SGSNPDPRecord_cellIdentity_present
                                    * if present */
    Fin_M_ChargingID chargingID;
    Fin_M_GSNAddress ggsnAddressUsed;
    Fin_M_AccessPointNameNI accessPointNameNI;
    Fin_M_PDPType   pdpType;
    Fin_M_PDPAddress servedPDPAddress;
    struct Fin_M__seqof2_ *listOfTrafficVolumes;
    Fin_M_TimeStamp recordOpeningTime;
    Fin_M_CallDuration duration;
    Fin_M_SGSNChange sgsnChange;  /* optional; set in bit_mask
                                   * Fin_M_SGSNPDPRecord_sgsnChange_present if
                                   * present */
    Fin_M_CauseForRecClosing causeForRecClosing;
    struct Fin_M__seqof3 *recordSequenceNumber;  /* optional; set in bit_mask
                          * Fin_M_SGSNPDPRecord_recordSequenceNumber_present if
                          * present */
    Fin_M_NodeID    nodeID;  /* optional; set in bit_mask
                              * Fin_M_SGSNPDPRecord_nodeID_present if present */
    struct Fin_M_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                              * Fin_M_SGSNPDPRecord_recordExtensions_present if
                              * present */
    Fin_M_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                              * Fin_M_SGSNPDPRecord_apnSelectionMode_present if
                              * present */
    Fin_M_AccessPointNameOI accessPointNameOI;
    Fin_M_MSISDN    servedMSISDN;  /* optional; set in bit_mask
                                    * Fin_M_SGSNPDPRecord_servedMSISDN_present
                                    * if present */
    Fin_M_ConsolidationResult consolidationResult;
} Fin_M_SGSNPDPRecord;

typedef ossBoolean      Fin_M_DynamicAddressFlag;

struct Fin_M__seqof4 {
    struct Fin_M__seqof4 *next;
    Fin_M_PDPAddress value;
};

typedef struct Fin_M_PLMN_Id {
    unsigned short  length;
    unsigned char   value[3];
} Fin_M_PLMN_Id;

typedef struct Fin_M_GGSNPDPRecord {
    unsigned short  bit_mask;
#       define      Fin_M_GGSNPDPRecord_networkInitiation_present 0x8000
#       define      Fin_M_GGSNPDPRecord_anonymousAccessIndicator_present 0x4000
#       define      Fin_M_remotePDPAddress_present 0x2000
#       define      Fin_M_dynamicAddressFlag_present 0x1000
#       define      Fin_M_GGSNPDPRecord_recordSequenceNumber_present 0x0800
#       define      Fin_M_GGSNPDPRecord_nodeID_present 0x0400
#       define      Fin_M_GGSNPDPRecord_recordExtensions_present 0x0200
#       define      Fin_M_GGSNPDPRecord_apnSelectionMode_present 0x0100
#       define      Fin_M_sgsnPLMNIdentifier_present 0x0080
#       define      Fin_M_GGSNPDPRecord_servedMSISDN_present 0x0040
    Fin_M_CallEventRecordType recordType;
    Fin_M_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                             * Fin_M_GGSNPDPRecord_networkInitiation_present if
                             * present */
    ossBoolean      anonymousAccessIndicator;  /* optional; set in bit_mask
                      * Fin_M_GGSNPDPRecord_anonymousAccessIndicator_present if
                      * present */
    Fin_M_IMSI      servedIMSI;
    Fin_M_GSNAddress ggsnAddress;
    Fin_M_ChargingID chargingID;
    struct Fin_M__seqof1_ *sgsnAddress;
    Fin_M_AccessPointNameNI accessPointNameNI;
    Fin_M_PDPType   pdpType;
    Fin_M_PDPAddress servedPDPAddress;
    struct Fin_M__seqof4 *remotePDPAddress;  /* optional; set in bit_mask
                           * Fin_M_remotePDPAddress_present if present */
    Fin_M_DynamicAddressFlag dynamicAddressFlag;  /* optional; set in bit_mask
                                          * Fin_M_dynamicAddressFlag_present if
                                          * present */
    struct Fin_M__seqof2_ *listOfTrafficVolumes;
    Fin_M_TimeStamp recordOpeningTime;
    Fin_M_CallDuration duration;
    Fin_M_CauseForRecClosing causeForRecClosing;
    Fin_M_RECORDSequenceNumber recordSequenceNumber;  /* optional; set in
                                   * bit_mask
                          * Fin_M_GGSNPDPRecord_recordSequenceNumber_present if
                          * present */
    Fin_M_NodeID    nodeID;  /* optional; set in bit_mask
                              * Fin_M_GGSNPDPRecord_nodeID_present if present */
    struct Fin_M_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                              * Fin_M_GGSNPDPRecord_recordExtensions_present if
                              * present */
    Fin_M_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                              * Fin_M_GGSNPDPRecord_apnSelectionMode_present if
                              * present */
    Fin_M_PLMN_Id   sgsnPLMNIdentifier;  /* optional; set in bit_mask
                                          * Fin_M_sgsnPLMNIdentifier_present if
                                          * present */
    Fin_M_MSISDN    servedMSISDN;  /* optional; set in bit_mask
                                    * Fin_M_GGSNPDPRecord_servedMSISDN_present
                                    * if present */
    Fin_M_ConsolidationResult consolidationResult;
} Fin_M_GGSNPDPRecord;

typedef struct Fin_M_ChangeLocation {
    unsigned char   bit_mask;
#       define      Fin_M_cellId_present 0x80
    Fin_M_LocationAreaCode locationAreaCode;
    Fin_M_RoutingAreaCode routingAreaCode;
    Fin_M_CellId    cellId;  /* optional; set in bit_mask Fin_M_cellId_present
                              * if present */
    Fin_M_TimeStamp changeTime;
} Fin_M_ChangeLocation;

struct Fin_M__octet8 {
    unsigned int    length;
    unsigned char   *value;
}; 
    
typedef struct Fin_M_ManagementExtension {
    unsigned char   bit_mask;
#       define      Fin_M_significance_present 0x80
    Fin_M__ObjectID identifier;
    ossBoolean      significance;  /* Fin_M_significance_present not set in
                                    * bit_mask implies value is FALSE */
    struct Fin_M__octet8 information;
} Fin_M_ManagementExtension;

typedef struct Fin_M_Diagnostics {
    unsigned short  choice;
#       define      Fin_M_gsm0408Cause_chosen 1
#       define      Fin_M_gsm0902MapErrorValue_chosen 2
#       define      Fin_M_ccittQ767Cause_chosen 3
#       define      Fin_M_networkSpecificCause_chosen 4
#       define      Fin_M_manufacturerSpecificCause_chosen 5
    union _union {
        int             gsm0408Cause;  /* to choose, set choice to
                                        * Fin_M_gsm0408Cause_chosen */
        int             gsm0902MapErrorValue;  /* to choose, set choice to
                                         * Fin_M_gsm0902MapErrorValue_chosen */
        int             ccittQ767Cause;  /* to choose, set choice to
                                          * Fin_M_ccittQ767Cause_chosen */
        Fin_M_ManagementExtension networkSpecificCause;  /* to choose, set
                                   * choice to
                                   * Fin_M_networkSpecificCause_chosen */
        Fin_M_ManagementExtension manufacturerSpecificCause;  /* to choose, set
                                   * choice to
                                   * Fin_M_manufacturerSpecificCause_chosen */
    } u;
} Fin_M_Diagnostics;

typedef unsigned int    Fin_M_LocalSequenceNumber;

struct Fin_M__seqof5 {
    struct Fin_M__seqof5 *next;
    Fin_M_ChangeLocation value;
}; 

typedef struct Fin_M_SGSNMMRecord {
    unsigned short  bit_mask;
#       define      Fin_M_SGSNMMRecord_servedIMEI_present 0x8000
#       define      Fin_M_SGSNMMRecord_msNetworkCapability_present 0x4000
#       define      Fin_M_SGSNMMRecord_routingArea_present 0x2000
#       define      Fin_M_SGSNMMRecord_locationAreaCode_present 0x1000
#       define      Fin_M_SGSNMMRecord_cellIdentity_present 0x0800
#       define      Fin_M_changeLocation_present 0x0400
#       define      Fin_M_duration_present 0x0200
#       define      Fin_M_SGSNMMRecord_sgsnChange_present 0x0100
#       define      Fin_M_diagnostics_present 0x0080
#       define      Fin_M_SGSNMMRecord_recordSequenceNumber_present 0x0040
#       define      Fin_M_SGSNMMRecord_nodeID_present 0x0020
#       define      Fin_M_SGSNMMRecord_recordExtensions_present 0x0010
#       define      Fin_M_SGSNMMRecord_localSequenceNumber_present 0x0008
    Fin_M_CallEventRecordType recordType;
    Fin_M_IMSI      servedIMSI;
    Fin_M_IMEI      servedIMEI;  /* optional; set in bit_mask
                                  * Fin_M_SGSNMMRecord_servedIMEI_present if
                                  * present */
    Fin_M_GSNAddress sgsnAddress;
    Fin_M_MSNetworkCapability msNetworkCapability;  /* optional; set in bit_mask
                            * Fin_M_SGSNMMRecord_msNetworkCapability_present if
                            * present */
    Fin_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                    * Fin_M_SGSNMMRecord_routingArea_present if
                                    * present */
    Fin_M_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                               * Fin_M_SGSNMMRecord_locationAreaCode_present if
                               * present */
    Fin_M_CellId    cellIdentity;  /* optional; set in bit_mask
                                    * Fin_M_SGSNMMRecord_cellIdentity_present if
                                    * present */
    struct Fin_M__seqof5 *changeLocation;  /* optional; set in bit_mask
                         * Fin_M_changeLocation_present if present */
    Fin_M_TimeStamp recordOpeningTime;
    Fin_M_CallDuration duration;  /* optional; set in bit_mask
                                   * Fin_M_duration_present if present */
    Fin_M_SGSNChange sgsnChange;  /* optional; set in bit_mask
                                   * Fin_M_SGSNMMRecord_sgsnChange_present if
                                   * present */
    Fin_M_CauseForRecClosing causeForRecClosing;
    Fin_M_Diagnostics diagnostics;  /* optional; set in bit_mask
                                     * Fin_M_diagnostics_present if present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                           * Fin_M_SGSNMMRecord_recordSequenceNumber_present if
                           * present */
    Fin_M_NodeID    nodeID;  /* optional; set in bit_mask
                              * Fin_M_SGSNMMRecord_nodeID_present if present */
    struct Fin_M_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                                   * Fin_M_SGSNMMRecord_recordExtensions_present
                                   * if present */
    Fin_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                            * Fin_M_SGSNMMRecord_localSequenceNumber_present if
                            * present */
} Fin_M_SGSNMMRecord;

typedef Fin_M_AddressString Fin_M_RecordingEntity;

typedef struct Fin_M_MessageReference {
    unsigned short  length;
    unsigned char   value[9];
} Fin_M_MessageReference;

typedef Fin_M_Diagnostics Fin_M_SMSResult;

typedef struct Fin_M_SGSNSMORecord {
    unsigned short  bit_mask;
#       define      Fin_M_SGSNSMORecord_servedIMEI_present 0x8000
#       define      Fin_M_SGSNSMORecord_servedMSISDN_present 0x4000
#       define      Fin_M_SGSNSMORecord_locationArea_present 0x2000
#       define      Fin_M_SGSNSMORecord_routingArea_present 0x1000
#       define      Fin_M_SGSNSMORecord_cellIdentity_present 0x0800
#       define      Fin_M_SGSNSMORecord_smsResult_present 0x0400
#       define      Fin_M_SGSNSMORecord_recordExtensions_present 0x0200
#       define      Fin_M_SGSNSMORecord_nodeID_present 0x0100
#       define      Fin_M_SGSNSMORecord_localSequenceNumber_present 0x0080
    Fin_M_CallEventRecordType recordType;
    Fin_M_IMSI      servedIMSI;
    Fin_M_IMEI      servedIMEI;  /* optional; set in bit_mask
                                  * Fin_M_SGSNSMORecord_servedIMEI_present if
                                  * present */
    Fin_M_MSISDN    servedMSISDN;  /* optional; set in bit_mask
                                    * Fin_M_SGSNSMORecord_servedMSISDN_present
                                    * if present */
    Fin_M_MSNetworkCapability msNetworkCapability;
    Fin_M_AddressString serviceCentre;
    Fin_M_RecordingEntity recordingEntity;
    Fin_M_LocationAreaCode locationArea;  /* optional; set in bit_mask
                                  * Fin_M_SGSNSMORecord_locationArea_present if
                                  * present */
    Fin_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                   * Fin_M_SGSNSMORecord_routingArea_present if
                                   * present */
    Fin_M_CellId    cellIdentity;  /* optional; set in bit_mask
                                    * Fin_M_SGSNSMORecord_cellIdentity_present
                                    * if present */
    Fin_M_MessageReference messageReference;
    Fin_M_TimeStamp originationTime;
    Fin_M_SMSResult smsResult;  /* optional; set in bit_mask
                                 * Fin_M_SGSNSMORecord_smsResult_present if
                                 * present */
    struct Fin_M_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                              * Fin_M_SGSNSMORecord_recordExtensions_present if
                              * present */
    Fin_M_NodeID    nodeID;  /* optional; set in bit_mask
                              * Fin_M_SGSNSMORecord_nodeID_present if present */
    Fin_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                           * Fin_M_SGSNSMORecord_localSequenceNumber_present if
                           * present */
} Fin_M_SGSNSMORecord;

typedef struct Fin_M_SGSNSMTRecord {
    unsigned short  bit_mask;
#       define      Fin_M_SGSNSMTRecord_servedIMEI_present 0x8000
#       define      Fin_M_SGSNSMTRecord_servedMSISDN_present 0x4000
#       define      Fin_M_SGSNSMTRecord_locationArea_present 0x2000
#       define      Fin_M_SGSNSMTRecord_routingArea_present 0x1000
#       define      Fin_M_SGSNSMTRecord_cellIdentity_present 0x0800
#       define      Fin_M_SGSNSMTRecord_smsResult_present 0x0400
#       define      Fin_M_SGSNSMTRecord_recordExtensions_present 0x0200
#       define      Fin_M_SGSNSMTRecord_nodeID_present 0x0100
#       define      Fin_M_SGSNSMTRecord_localSequenceNumber_present 0x0080
    Fin_M_CallEventRecordType recordType;
    Fin_M_IMSI      servedIMSI;
    Fin_M_IMEI      servedIMEI;  /* optional; set in bit_mask
                                  * Fin_M_SGSNSMTRecord_servedIMEI_present if
                                  * present */
    Fin_M_MSISDN    servedMSISDN;  /* optional; set in bit_mask
                                    * Fin_M_SGSNSMTRecord_servedMSISDN_present
                                    * if present */
    Fin_M_MSNetworkCapability msNetworkCapability;
    Fin_M_AddressString serviceCentre;
    Fin_M_RecordingEntity recordingEntity;
    Fin_M_LocationAreaCode locationArea;  /* optional; set in bit_mask
                                  * Fin_M_SGSNSMTRecord_locationArea_present if
                                  * present */
    Fin_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                   * Fin_M_SGSNSMTRecord_routingArea_present if
                                   * present */
    Fin_M_CellId    cellIdentity;  /* optional; set in bit_mask
                                    * Fin_M_SGSNSMTRecord_cellIdentity_present
                                    * if present */
    Fin_M_TimeStamp originationTime;
    Fin_M_SMSResult smsResult;  /* optional; set in bit_mask
                                 * Fin_M_SGSNSMTRecord_smsResult_present if
                                 * present */
    struct Fin_M_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                              * Fin_M_SGSNSMTRecord_recordExtensions_present if
                              * present */
    Fin_M_NodeID    nodeID;  /* optional; set in bit_mask
                              * Fin_M_SGSNSMTRecord_nodeID_present if present */
    Fin_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                           * Fin_M_SGSNSMTRecord_localSequenceNumber_present if
                           * present */
} Fin_M_SGSNSMTRecord;

typedef struct Fin_M_CallEventRecord {
    unsigned short  choice;
#       define      Fin_M_sgsnPDPRecord_chosen 1
#       define      Fin_M_ggsnPDPRecord_chosen 2
#       define      Fin_M_sgsnMMRecord_chosen 3
#       define      Fin_M_sgsnSMORecord_chosen 4
#       define      Fin_M_sgsnSMTRecord_chosen 5
    union _union {
        Fin_M_SGSNPDPRecord sgsnPDPRecord;  /* to choose, set choice to
                                             * Fin_M_sgsnPDPRecord_chosen */
        Fin_M_GGSNPDPRecord ggsnPDPRecord;  /* to choose, set choice to
                                             * Fin_M_ggsnPDPRecord_chosen */
        Fin_M_SGSNMMRecord sgsnMMRecord;  /* to choose, set choice to
                                           * Fin_M_sgsnMMRecord_chosen */
        Fin_M_SGSNSMORecord sgsnSMORecord;  /* to choose, set choice to
                                             * Fin_M_sgsnSMORecord_chosen */
        Fin_M_SGSNSMTRecord sgsnSMTRecord;  /* to choose, set choice to
                                             * Fin_M_sgsnSMTRecord_chosen */
    } u;
} Fin_M_CallEventRecord;

typedef struct Fin_M__seqof6 {
     struct Fin_M__seqof6 *next;
     Fin_M_CallEventRecord value;
}Fin_M_listOfCDR;

typedef struct Fin_M_BSRecord {
    Fin_M_BSXferSummaryRecord bsXferRecord;
    Fin_M_listOfCDR *listOfCDR;
} Fin_M_BSRecord;

typedef struct Fin_M_ManagementExtensions_ {
    struct Fin_M_ManagementExtensions_ *next;
    Fin_M_ManagementExtension value;
} *Fin_M_ManagementExtensions;

typedef int             Fin_M_RecordNumber;

typedef struct Fin_M_RecordNumberList_ {
    struct Fin_M_RecordNumberList_ *next;
    Fin_M_RecordNumber value;
} *Fin_M_RecordNumberList;

extern int Fin_M_maxAddressLength;

extern int Fin_M_maxISDN_AddressLength;


extern void *CDRF_R98_Fin_M;    /* encoder-decoder control table */
#ifdef __cplusplus
}	/* extern "C" */
#endif /* __cplusplus */
#endif /* OSS_CDRF_R98_Fin_M */
