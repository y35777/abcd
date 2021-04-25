/********************************************************/
/* Copyright (C) 2003 OSS Nokalva.  All rights reserved.*/
/********************************************************/

/* THIS FILE IS PROPRIETARY MATERIAL OF OSS NOKALVA
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF OSS NOKALVA.
 * THIS FILE MAY NOT BE DISTRIBUTED. */

/* Generated for: HuaWei Technologies, Inc., Shenzhen, China - license 8088 for Windows NT */
/* Abstract syntax: CDRF_2GCDR_Org_M */
/* Created: Mon Mar 24 10:40:02 2003 */
/* ASN.1 compiler version: 5.3 */
/* Target operating system: Solaris 2.3 or later */
/* Target machine type: SPARC */
/* C compiler options required: None */
/* ASN.1 compiler options and file names specified:
 * -codefile CDRF_2GCDR_Org_M.c -headerfile CDRF_2GCDR_Org_M.h -errorfile
 * C:\WINDOWS\TEMP\2. -prefix Org_M_ -dualheader -ansi -verbose -ber -root
 * D:\application\ossasn1\solaris\asn1dflt.solaris-sparc
 * D:\project\530\asn1app\maimaoshi\CDRF_2GCDR_Org_M.asn
 */

#ifndef OSS_CDRF_2GCDR_Org_M
#define OSS_CDRF_2GCDR_Org_M

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "asn1hdr.h"
#include "asn1code.h"

#define          Org_M_BSRecord_PDU 1

typedef struct Org_M__ObjectID {
    unsigned short  length;
    unsigned char   *value;
} Org_M__ObjectID;

typedef struct Org_M__Any {
    unsigned long   length;
    unsigned char   *value;
} Org_M__Any;

typedef struct Org_M__octet1 {
    unsigned short  length;
    unsigned char   value[15];
} Org_M__octet1;

typedef struct Org_M__octet3 {
    unsigned short  length;
    unsigned char   value[14];
} Org_M__octet3;

typedef struct Org_M__octet4 {
    unsigned short  length;
    unsigned char   value[10];
} Org_M__octet4;

typedef struct Org_M_BSXferSummaryRecord {
    Org_M__octet1   cgnID;
    struct Org_M__octet2 {
        unsigned short  length;
        unsigned char   value[34];
    } bsXferFile;
    Org_M__octet3   openTime;
    Org_M__octet3   closeTime;
    Org_M__octet4   lrsnStart;
    Org_M__octet4   lrsnEnd;
    struct Org_M__octet5 {
        unsigned short  length;
        unsigned char   value[3];
    } cdrType;
    Org_M__octet4   totalCdr;
    Org_M__octet4   missingCdr;
    Org_M__octet4   duplicateCdr;
    Org_M__octet1   cgID;
} Org_M_BSXferSummaryRecord;

typedef int             Org_M_CallEventRecordType;
#define                     Org_M_CallEventRecordType_sgsnPDPRecord 18
#define                     Org_M_CallEventRecordType_ggsnPDPRecord 19
#define                     Org_M_CallEventRecordType_sgsnMMRecord 20
#define                     Org_M_CallEventRecordType_sgsnSMORecord 21
#define                     Org_M_CallEventRecordType_sgsnSMTRecord 22

typedef ossBoolean      Org_M_NetworkInitiatedPDPContext;

typedef struct Org_M_IMSI {
    unsigned short  length;
    unsigned char   value[8];
} Org_M_IMSI;

typedef struct Org_M_IMEI {
    unsigned short  length;
    unsigned char   value[8];
} Org_M_IMEI;

typedef struct Org_M_IPBinaryAddress {
    unsigned short  choice;
#       define      Org_M_iPBinV4Address_chosen 1
#       define      Org_M_iPBinV6Address_chosen 2
    union _union {
        struct Org_M__octet6 {
            unsigned short  length;
            unsigned char   value[4];
        } iPBinV4Address;  /* to choose, set choice to
                            * Org_M_iPBinV4Address_chosen */
        struct Org_M__octet7 {
            unsigned short  length;
            unsigned char   value[16];
        } iPBinV6Address;  /* to choose, set choice to
                            * Org_M_iPBinV6Address_chosen */
    } u;
} Org_M_IPBinaryAddress;

typedef struct Org_M_IPTextRepresentedAddress {
    unsigned short  choice;
#       define      Org_M_iPTextV4Address_chosen 1
#       define      Org_M_iPTextV6Address_chosen 2
    union _union {
        char            iPTextV4Address[16];  /* to choose, set choice to
                                              * Org_M_iPTextV4Address_chosen */
        char            iPTextV6Address[46];  /* to choose, set choice to
                                              * Org_M_iPTextV6Address_chosen */
    } u;
} Org_M_IPTextRepresentedAddress;

typedef struct Org_M_IPAddress {
    unsigned short  choice;
#       define      Org_M_iPBinaryAddress_chosen 1
#       define      Org_M_iPTextRepresentedAddress_chosen 2
    union _union {
        Org_M_IPBinaryAddress iPBinaryAddress;  /* to choose, set choice to
                                              * Org_M_iPBinaryAddress_chosen */
        Org_M_IPTextRepresentedAddress iPTextRepresentedAddress;  /* to choose,
                                   * set choice to
                                   * Org_M_iPTextRepresentedAddress_chosen */
    } u;
} Org_M_IPAddress;

typedef Org_M_IPAddress Org_M_GSNAddress;

typedef struct Org_M_MSNetworkCapability {
    unsigned short  length;
    unsigned char   value[1];
} Org_M_MSNetworkCapability;

typedef struct Org_M_RoutingAreaCode {
    unsigned short  length;
    unsigned char   value[1];
} Org_M_RoutingAreaCode;

typedef struct Org_M_LocationAreaCode {
    unsigned short  length;
    unsigned char   value[2];
} Org_M_LocationAreaCode;

typedef struct Org_M_CellId {
    unsigned short  length;
    unsigned char   value[2];
} Org_M_CellId;

typedef unsigned int    Org_M_ChargingID;

typedef char            Org_M_AccessPointNameNI[64];

typedef struct Org_M_PDPType {
    unsigned short  length;
    unsigned char   value[2];
} Org_M_PDPType;

typedef struct Org_M_AddressString {
    unsigned short  length;
    unsigned char   value[20];
} Org_M_AddressString;

typedef Org_M_AddressString Org_M_ETSIAddress;

typedef struct Org_M_PDPAddress {
    unsigned short  choice;
#       define      Org_M_iPAddress_chosen 1
#       define      Org_M_eTSIAddress_chosen 2
    union _union {
        Org_M_IPAddress iPAddress;  /* to choose, set choice to
                                     * Org_M_iPAddress_chosen */
        Org_M_ETSIAddress eTSIAddress;  /* to choose, set choice to
                                         * Org_M_eTSIAddress_chosen */
    } u;
} Org_M_PDPAddress;

typedef enum Org_M_QoSReliability {
    Org_M_unspecifiedReliability = 0,
    Org_M_acknowledgedGTP = 1,
    Org_M_unackGTPAcknowLLC = 2,
    Org_M_unackGTPLLCAcknowRLC = 3,
    Org_M_unackGTPLLCRLC = 4,
    Org_M_unacknowUnprotectedData = 5
} Org_M_QoSReliability;

typedef enum Org_M_QoSDelay {
    Org_M_delayClass1 = 1,
    Org_M_delayClass2 = 2,
    Org_M_delayClass3 = 3,
    Org_M_delayClass4 = 4
} Org_M_QoSDelay;

typedef enum Org_M_QoSPrecedence {
    Org_M_QoSPrecedence_unspecified = 0,
    Org_M_highPriority = 1,
    Org_M_normalPriority = 2,
    Org_M_lowPriority = 3
} Org_M_QoSPrecedence;

typedef enum Org_M_QoSPeakThroughput {
    Org_M_QoSPeakThroughput_unspecified = 0,
    Org_M_upTo100OctetPs = 1,
    Org_M_upTo200OctetPs = 2,
    Org_M_upTo400OctetPs = 3,
    Org_M_upTo800OctetPs = 4,
    Org_M_upTo1600OctetPs = 5,
    Org_M_upTo3200OctetPs = 6,
    Org_M_upTo6400OctetPs = 7,
    Org_M_upTo12800OctetPs = 8,
    Org_M_upTo25600OctetPs = 9
} Org_M_QoSPeakThroughput;

typedef enum Org_M_QoSMeanThroughput {
    Org_M_bestEffort = 0,
    Org_M_mean100octetPh = 1,
    Org_M_mean200octetPh = 2,
    Org_M_mean500octetPh = 3,
    Org_M_mean1000octetPh = 4,
    Org_M_mean2000octetPh = 5,
    Org_M_mean5000octetPh = 6,
    Org_M_mean10000octetPh = 7,
    Org_M_mean20000octetPh = 8,
    Org_M_mean50000octetPh = 9,
    Org_M_mean100000octetPh = 10,
    Org_M_mean200000octetPh = 11,
    Org_M_mean500000octetPh = 12,
    Org_M_mean1000000octetPh = 13,
    Org_M_mean2000000octetPh = 14,
    Org_M_mean5000000octetPh = 15,
    Org_M_mean10000000octetPh = 16,
    Org_M_mean20000000octetPh = 17,
    Org_M_mean50000000octetPh = 18
} Org_M_QoSMeanThroughput;

typedef struct Org_M_QoSInformation {
    Org_M_QoSReliability reliability;
    Org_M_QoSDelay  delay;
    Org_M_QoSPrecedence precedence;
    Org_M_QoSPeakThroughput peakThroughput;
    Org_M_QoSMeanThroughput meanThroughput;
} Org_M_QoSInformation;

typedef int             Org_M_DataVolumeGPRS;

typedef enum Org_M_ChangeCondition {
    Org_M_qoSChange = 0,
    Org_M_tariffTime = 1,
    Org_M_recordClosure = 2
} Org_M_ChangeCondition;

typedef struct Org_M_TimeStamp {
    unsigned short  length;
    unsigned char   value[9];
} Org_M_TimeStamp;

typedef struct Org_M_ChangeOfCharCondition {
    unsigned char   bit_mask;
#       define      Org_M_qosRequested_present 0x80
#       define      Org_M_qosNegotiated_present 0x40
    Org_M_QoSInformation qosRequested;  /* optional; set in bit_mask
                                         * Org_M_qosRequested_present if
                                         * present */
    Org_M_QoSInformation qosNegotiated;  /* optional; set in bit_mask
                                          * Org_M_qosNegotiated_present if
                                          * present */
    Org_M_DataVolumeGPRS dataVolumeGPRSUplink;
    Org_M_DataVolumeGPRS dataVolumeGPRSDownlink;
    Org_M_ChangeCondition changeCondition;
    Org_M_TimeStamp changeTime;
} Org_M_ChangeOfCharCondition;

typedef int             Org_M_CallDuration;

typedef ossBoolean      Org_M_SGSNChange;

typedef int             Org_M_CauseForRecClosing;
#define                     Org_M_normalRelease 0
#define                     Org_M_abnormalRelease 4
#define                     Org_M_volumeLimit 16
#define                     Org_M_timeLimit 17
#define                     Org_M_sGSNChange 18
#define                     Org_M_maxChangeCond 19
#define                     Org_M_managementIntervention 20

typedef struct Org_M_ManagementExtension {
    unsigned char   bit_mask;
#       define      Org_M_significance_present 0x80
    Org_M__ObjectID identifier;
    ossBoolean      significance;  /* Org_M_significance_present not set in
                                    * bit_mask implies value is FALSE */
    Org_M__Any      information;
} Org_M_ManagementExtension;

typedef struct Org_M_Diagnostics {
    unsigned short  choice;
#       define      Org_M_gsm0408Cause_chosen 1
#       define      Org_M_gsm0902MapErrorValue_chosen 2
#       define      Org_M_ccittQ767Cause_chosen 3
#       define      Org_M_networkSpecificCause_chosen 4
#       define      Org_M_manufacturerSpecificCause_chosen 5
    union _union {
        int             gsm0408Cause;  /* to choose, set choice to
                                        * Org_M_gsm0408Cause_chosen */
        int             gsm0902MapErrorValue;  /* to choose, set choice to
                                         * Org_M_gsm0902MapErrorValue_chosen */
        int             ccittQ767Cause;  /* to choose, set choice to
                                          * Org_M_ccittQ767Cause_chosen */
        Org_M_ManagementExtension networkSpecificCause;  /* to choose, set
                                   * choice to
                                   * Org_M_networkSpecificCause_chosen */
        Org_M_ManagementExtension manufacturerSpecificCause;  /* to choose, set
                                   * choice to
                                   * Org_M_manufacturerSpecificCause_chosen */
    } u;
} Org_M_Diagnostics;

typedef char            Org_M_NodeID[21];

typedef unsigned int    Org_M_LocalSequenceNumber;

typedef enum Org_M_APNSelectionMode {
    Org_M_mSorNetworkProvidedSubscriptionVerified = 0,
    Org_M_mSProvidedSubscriptionNotVerified = 1,
    Org_M_networkProvidedSubscriptionNotVerified = 2
} Org_M_APNSelectionMode;

typedef char            Org_M_AccessPointNameOI[38];

typedef struct Org_M_ISDN_AddressString {
    unsigned short  length;
    unsigned char   value[10];
} Org_M_ISDN_AddressString;

typedef Org_M_ISDN_AddressString Org_M_MSISDN;

typedef struct Org_M__seqof1_ {
    struct Org_M__seqof1_ *next;
    Org_M_ChangeOfCharCondition value;
} *Org_M__seqof1;

typedef struct Org_M_SGSNPDPRecord {
    unsigned short  bit_mask;
#       define      Org_M_SGSNPDPRecord_networkInitiation_present 0x8000
#       define      Org_M_SGSNPDPRecord_anonymousAccessIndicator_present 0x4000
#       define      Org_M_SGSNPDPRecord_servedIMEI_present 0x2000
#       define      Org_M_SGSNPDPRecord_msNetworkCapability_present 0x1000
#       define      Org_M_SGSNPDPRecord_routingArea_present 0x0800
#       define      Org_M_SGSNPDPRecord_locationAreaCode_present 0x0400
#       define      Org_M_SGSNPDPRecord_cellIdentity_present 0x0200
#       define      Org_M_SGSNPDPRecord_sgsnChange_present 0x0100
#       define      Org_M_SGSNPDPRecord_diagnostics_present 0x0080
#       define      Org_M_SGSNPDPRecord_recordSequenceNumber_present 0x0040
#       define      Org_M_SGSNPDPRecord_nodeID_present 0x0020
#       define      Org_M_SGSNPDPRecord_recordExtensions_present 0x0010
#       define      Org_M_SGSNPDPRecord_localSequenceNumber_present 0x0008
#       define      Org_M_SGSNPDPRecord_apnSelectionMode_present 0x0004
#       define      Org_M_SGSNPDPRecord_servedMSISDN_present 0x0002
    Org_M_CallEventRecordType recordType;
    Org_M_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                             * Org_M_SGSNPDPRecord_networkInitiation_present if
                             * present */
    ossBoolean      anonymousAccessIndicator;  /* optional; set in bit_mask
                      * Org_M_SGSNPDPRecord_anonymousAccessIndicator_present if
                      * present */
    Org_M_IMSI      servedIMSI;
    Org_M_IMEI      servedIMEI;  /* optional; set in bit_mask
                                  * Org_M_SGSNPDPRecord_servedIMEI_present if
                                  * present */
    Org_M_GSNAddress sgsnAddress;
    Org_M_MSNetworkCapability msNetworkCapability;  /* optional; set in bit_mask
                           * Org_M_SGSNPDPRecord_msNetworkCapability_present if
                           * present */
    Org_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                   * Org_M_SGSNPDPRecord_routingArea_present if
                                   * present */
    Org_M_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                              * Org_M_SGSNPDPRecord_locationAreaCode_present if
                              * present */
    Org_M_CellId    cellIdentity;  /* optional; set in bit_mask
                                    * Org_M_SGSNPDPRecord_cellIdentity_present
                                    * if present */
    Org_M_ChargingID chargingID;
    Org_M_GSNAddress ggsnAddressUsed;
    Org_M_AccessPointNameNI accessPointNameNI;
    Org_M_PDPType   pdpType;
    Org_M_PDPAddress servedPDPAddress;
    struct Org_M__seqof1_ *listOfTrafficVolumes;
    Org_M_TimeStamp recordOpeningTime;
    Org_M_CallDuration duration;
    Org_M_SGSNChange sgsnChange;  /* optional; set in bit_mask
                                   * Org_M_SGSNPDPRecord_sgsnChange_present if
                                   * present */
    Org_M_CauseForRecClosing causeForRecClosing;
    Org_M_Diagnostics diagnostics;  /* optional; set in bit_mask
                                     * Org_M_SGSNPDPRecord_diagnostics_present
                                     * if present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                          * Org_M_SGSNPDPRecord_recordSequenceNumber_present if
                          * present */
    Org_M_NodeID    nodeID;  /* optional; set in bit_mask
                              * Org_M_SGSNPDPRecord_nodeID_present if present */
    struct Org_M_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                              * Org_M_SGSNPDPRecord_recordExtensions_present if
                              * present */
    Org_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                           * Org_M_SGSNPDPRecord_localSequenceNumber_present if
                           * present */
    Org_M_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                              * Org_M_SGSNPDPRecord_apnSelectionMode_present if
                              * present */
    Org_M_AccessPointNameOI accessPointNameOI;
    Org_M_MSISDN    servedMSISDN;  /* optional; set in bit_mask
                                    * Org_M_SGSNPDPRecord_servedMSISDN_present
                                    * if present */
} Org_M_SGSNPDPRecord;

typedef ossBoolean      Org_M_DynamicAddressFlag;

struct Org_M__seqof2 {
    struct Org_M__seqof2 *next;
    Org_M_GSNAddress value;
};


struct Org_M__seqof3 {
    struct Org_M__seqof3 *next;
    Org_M_PDPAddress value;
};
    
typedef struct Org_M_GGSNPDPRecord {
    unsigned short  bit_mask;
#       define      Org_M_GGSNPDPRecord_networkInitiation_present 0x8000
#       define      Org_M_GGSNPDPRecord_anonymousAccessIndicator_present 0x4000
#       define      Org_M_remotePDPAddress_present 0x2000
#       define      Org_M_dynamicAddressFlag_present 0x1000
#       define      Org_M_GGSNPDPRecord_diagnostics_present 0x0800
#       define      Org_M_GGSNPDPRecord_recordSequenceNumber_present 0x0400
#       define      Org_M_GGSNPDPRecord_nodeID_present 0x0200
#       define      Org_M_GGSNPDPRecord_recordExtensions_present 0x0100
#       define      Org_M_GGSNPDPRecord_localSequenceNumber_present 0x0080
#       define      Org_M_GGSNPDPRecord_apnSelectionMode_present 0x0040
#       define      Org_M_GGSNPDPRecord_servedMSISDN_present 0x0020
    Org_M_CallEventRecordType recordType;
    Org_M_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                             * Org_M_GGSNPDPRecord_networkInitiation_present if
                             * present */
    ossBoolean      anonymousAccessIndicator;  /* optional; set in bit_mask
                      * Org_M_GGSNPDPRecord_anonymousAccessIndicator_present if
                      * present */
    Org_M_IMSI      servedIMSI;
    Org_M_GSNAddress ggsnAddress;
    Org_M_ChargingID chargingID;
    struct Org_M__seqof2 *sgsnAddress;
    Org_M_AccessPointNameNI accessPointNameNI;
    Org_M_PDPType   pdpType;
    Org_M_PDPAddress servedPDPAddress;
    struct Org_M__seqof3 *remotePDPAddress;  /* optional; set in bit_mask
                           * Org_M_remotePDPAddress_present if present */
    Org_M_DynamicAddressFlag dynamicAddressFlag;  /* optional; set in bit_mask
                                          * Org_M_dynamicAddressFlag_present if
                                          * present */
    struct Org_M__seqof1_ *listOfTrafficVolumes;
    Org_M_TimeStamp recordOpeningTime;
    Org_M_CallDuration duration;
    Org_M_CauseForRecClosing causeForRecClosing;
    Org_M_Diagnostics diagnostics;  /* optional; set in bit_mask
                                     * Org_M_GGSNPDPRecord_diagnostics_present
                                     * if present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                          * Org_M_GGSNPDPRecord_recordSequenceNumber_present if
                          * present */
    Org_M_NodeID    nodeID;  /* optional; set in bit_mask
                              * Org_M_GGSNPDPRecord_nodeID_present if present */
    struct Org_M_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                              * Org_M_GGSNPDPRecord_recordExtensions_present if
                              * present */
    Org_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                           * Org_M_GGSNPDPRecord_localSequenceNumber_present if
                           * present */
    Org_M_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                              * Org_M_GGSNPDPRecord_apnSelectionMode_present if
                              * present */
    Org_M_MSISDN    servedMSISDN;  /* optional; set in bit_mask
                                    * Org_M_GGSNPDPRecord_servedMSISDN_present
                                    * if present */
} Org_M_GGSNPDPRecord;

typedef struct Org_M_ChangeLocation {
    unsigned char   bit_mask;
#       define      Org_M_cellId_present 0x80
    Org_M_LocationAreaCode locationAreaCode;
    Org_M_RoutingAreaCode routingAreaCode;
    Org_M_CellId    cellId;  /* optional; set in bit_mask Org_M_cellId_present
                              * if present */
    Org_M_TimeStamp changeTime;
} Org_M_ChangeLocation;

struct Org_M__seqof4 {
    struct Org_M__seqof4 *next;
    Org_M_ChangeLocation value;
};

typedef struct Org_M_SGSNMMRecord {
    unsigned short  bit_mask;
#       define      Org_M_SGSNMMRecord_servedIMEI_present 0x8000
#       define      Org_M_SGSNMMRecord_msNetworkCapability_present 0x4000
#       define      Org_M_SGSNMMRecord_routingArea_present 0x2000
#       define      Org_M_SGSNMMRecord_locationAreaCode_present 0x1000
#       define      Org_M_SGSNMMRecord_cellIdentity_present 0x0800
#       define      Org_M_changeLocation_present 0x0400
#       define      Org_M_duration_present 0x0200
#       define      Org_M_SGSNMMRecord_sgsnChange_present 0x0100
#       define      Org_M_SGSNMMRecord_diagnostics_present 0x0080
#       define      Org_M_SGSNMMRecord_recordSequenceNumber_present 0x0040
#       define      Org_M_SGSNMMRecord_nodeID_present 0x0020
#       define      Org_M_SGSNMMRecord_recordExtensions_present 0x0010
#       define      Org_M_SGSNMMRecord_localSequenceNumber_present 0x0008
    Org_M_CallEventRecordType recordType;
    Org_M_IMSI      servedIMSI;
    Org_M_IMEI      servedIMEI;  /* optional; set in bit_mask
                                  * Org_M_SGSNMMRecord_servedIMEI_present if
                                  * present */
    Org_M_GSNAddress sgsnAddress;
    Org_M_MSNetworkCapability msNetworkCapability;  /* optional; set in bit_mask
                            * Org_M_SGSNMMRecord_msNetworkCapability_present if
                            * present */
    Org_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                    * Org_M_SGSNMMRecord_routingArea_present if
                                    * present */
    Org_M_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                               * Org_M_SGSNMMRecord_locationAreaCode_present if
                               * present */
    Org_M_CellId    cellIdentity;  /* optional; set in bit_mask
                                    * Org_M_SGSNMMRecord_cellIdentity_present if
                                    * present */
    struct Org_M__seqof4 *changeLocation;  /* optional; set in bit_mask
                         * Org_M_changeLocation_present if present */
    Org_M_TimeStamp recordOpeningTime;
    Org_M_CallDuration duration;  /* optional; set in bit_mask
                                   * Org_M_duration_present if present */
    Org_M_SGSNChange sgsnChange;  /* optional; set in bit_mask
                                   * Org_M_SGSNMMRecord_sgsnChange_present if
                                   * present */
    Org_M_CauseForRecClosing causeForRecClosing;
    Org_M_Diagnostics diagnostics;  /* optional; set in bit_mask
                                     * Org_M_SGSNMMRecord_diagnostics_present if
                                     * present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                           * Org_M_SGSNMMRecord_recordSequenceNumber_present if
                           * present */
    Org_M_NodeID    nodeID;  /* optional; set in bit_mask
                              * Org_M_SGSNMMRecord_nodeID_present if present */
    struct Org_M_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                                   * Org_M_SGSNMMRecord_recordExtensions_present
                                   * if present */
    Org_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                            * Org_M_SGSNMMRecord_localSequenceNumber_present if
                            * present */
} Org_M_SGSNMMRecord;

typedef Org_M_AddressString Org_M_RecordingEntity;

typedef struct Org_M_MessageReference {
    unsigned short  length;
    unsigned char   value[9];
} Org_M_MessageReference;

typedef Org_M_Diagnostics Org_M_SMSResult;

typedef struct Org_M_SGSNSMORecord {
    unsigned short  bit_mask;
#       define      Org_M_SGSNSMORecord_servedIMEI_present 0x8000
#       define      Org_M_SGSNSMORecord_servedMSISDN_present 0x4000
#       define      Org_M_SGSNSMORecord_locationArea_present 0x2000
#       define      Org_M_SGSNSMORecord_routingArea_present 0x1000
#       define      Org_M_SGSNSMORecord_cellIdentity_present 0x0800
#       define      Org_M_SGSNSMORecord_smsResult_present 0x0400
#       define      Org_M_SGSNSMORecord_recordExtensions_present 0x0200
#       define      Org_M_SGSNSMORecord_nodeID_present 0x0100
#       define      Org_M_SGSNSMORecord_localSequenceNumber_present 0x0080
    Org_M_CallEventRecordType recordType;
    Org_M_IMSI      servedIMSI;
    Org_M_IMEI      servedIMEI;  /* optional; set in bit_mask
                                  * Org_M_SGSNSMORecord_servedIMEI_present if
                                  * present */
    Org_M_MSISDN    servedMSISDN;  /* optional; set in bit_mask
                                    * Org_M_SGSNSMORecord_servedMSISDN_present
                                    * if present */
    Org_M_MSNetworkCapability msNetworkCapability;
    Org_M_AddressString serviceCentre;
    Org_M_RecordingEntity recordingEntity;
    Org_M_LocationAreaCode locationArea;  /* optional; set in bit_mask
                                  * Org_M_SGSNSMORecord_locationArea_present if
                                  * present */
    Org_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                   * Org_M_SGSNSMORecord_routingArea_present if
                                   * present */
    Org_M_CellId    cellIdentity;  /* optional; set in bit_mask
                                    * Org_M_SGSNSMORecord_cellIdentity_present
                                    * if present */
    Org_M_MessageReference messageReference;
    Org_M_TimeStamp originationTime;
    Org_M_SMSResult smsResult;  /* optional; set in bit_mask
                                 * Org_M_SGSNSMORecord_smsResult_present if
                                 * present */
    struct Org_M_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                              * Org_M_SGSNSMORecord_recordExtensions_present if
                              * present */
    Org_M_NodeID    nodeID;  /* optional; set in bit_mask
                              * Org_M_SGSNSMORecord_nodeID_present if present */
    Org_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                           * Org_M_SGSNSMORecord_localSequenceNumber_present if
                           * present */
} Org_M_SGSNSMORecord;

typedef struct Org_M_SGSNSMTRecord {
    unsigned short  bit_mask;
#       define      Org_M_SGSNSMTRecord_servedIMEI_present 0x8000
#       define      Org_M_SGSNSMTRecord_servedMSISDN_present 0x4000
#       define      Org_M_SGSNSMTRecord_locationArea_present 0x2000
#       define      Org_M_SGSNSMTRecord_routingArea_present 0x1000
#       define      Org_M_SGSNSMTRecord_cellIdentity_present 0x0800
#       define      Org_M_SGSNSMTRecord_smsResult_present 0x0400
#       define      Org_M_SGSNSMTRecord_recordExtensions_present 0x0200
#       define      Org_M_SGSNSMTRecord_nodeID_present 0x0100
#       define      Org_M_SGSNSMTRecord_localSequenceNumber_present 0x0080
    Org_M_CallEventRecordType recordType;
    Org_M_IMSI      servedIMSI;
    Org_M_IMEI      servedIMEI;  /* optional; set in bit_mask
                                  * Org_M_SGSNSMTRecord_servedIMEI_present if
                                  * present */
    Org_M_MSISDN    servedMSISDN;  /* optional; set in bit_mask
                                    * Org_M_SGSNSMTRecord_servedMSISDN_present
                                    * if present */
    Org_M_MSNetworkCapability msNetworkCapability;
    Org_M_AddressString serviceCentre;
    Org_M_RecordingEntity recordingEntity;
    Org_M_LocationAreaCode locationArea;  /* optional; set in bit_mask
                                  * Org_M_SGSNSMTRecord_locationArea_present if
                                  * present */
    Org_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                   * Org_M_SGSNSMTRecord_routingArea_present if
                                   * present */
    Org_M_CellId    cellIdentity;  /* optional; set in bit_mask
                                    * Org_M_SGSNSMTRecord_cellIdentity_present
                                    * if present */
    Org_M_TimeStamp originationTime;
    Org_M_SMSResult smsResult;  /* optional; set in bit_mask
                                 * Org_M_SGSNSMTRecord_smsResult_present if
                                 * present */
    struct Org_M_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                              * Org_M_SGSNSMTRecord_recordExtensions_present if
                              * present */
    Org_M_NodeID    nodeID;  /* optional; set in bit_mask
                              * Org_M_SGSNSMTRecord_nodeID_present if present */
    Org_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                           * Org_M_SGSNSMTRecord_localSequenceNumber_present if
                           * present */
} Org_M_SGSNSMTRecord;

typedef struct Org_M_CallEventRecord {
    unsigned short  choice;
#       define      Org_M_sgsnPDPRecord_chosen 1
#       define      Org_M_ggsnPDPRecord_chosen 2
#       define      Org_M_sgsnMMRecord_chosen 3
#       define      Org_M_sgsnSMORecord_chosen 4
#       define      Org_M_sgsnSMTRecord_chosen 5
    union _union {
        Org_M_SGSNPDPRecord sgsnPDPRecord;  /* to choose, set choice to
                                             * Org_M_sgsnPDPRecord_chosen */
        Org_M_GGSNPDPRecord ggsnPDPRecord;  /* to choose, set choice to
                                             * Org_M_ggsnPDPRecord_chosen */
        Org_M_SGSNMMRecord sgsnMMRecord;  /* to choose, set choice to
                                           * Org_M_sgsnMMRecord_chosen */
        Org_M_SGSNSMORecord sgsnSMORecord;  /* to choose, set choice to
                                             * Org_M_sgsnSMORecord_chosen */
        Org_M_SGSNSMTRecord sgsnSMTRecord;  /* to choose, set choice to
                                             * Org_M_sgsnSMTRecord_chosen */
    } u;
} Org_M_CallEventRecord;

typedef struct Org_M__seqof5 {
     struct Org_M__seqof5 *next;
     Org_M_CallEventRecord value;
}Org_M_listOfCDR;
typedef struct Org_M_BSRecord {
    Org_M_BSXferSummaryRecord bsXferRecord;
    Org_M_listOfCDR *listOfCDR;
} Org_M_BSRecord;

typedef struct Org_M_ManagementExtensions_ {
    struct Org_M_ManagementExtensions_ *next;
    Org_M_ManagementExtension value;
} *Org_M_ManagementExtensions;

extern Org_M__ObjectID Org_M_gsm1205InformationModel;

extern Org_M__ObjectID Org_M_gsm1205ASN1Module;

extern Org_M__ObjectID Org_M_gsm1205ManagedObjectClass;

extern Org_M__ObjectID Org_M_gsm1205Package;

extern Org_M__ObjectID Org_M_gsm1205NameBinding;

extern Org_M__ObjectID Org_M_gsm1205Attribute;

extern Org_M__ObjectID Org_M_gsm1205Action;

extern Org_M__ObjectID Org_M_gsm1205Notification;

extern int Org_M_maxAddressLength;

extern int Org_M_maxISDN_AddressLength;


extern void *CDRF_2GCDR_Org_M;    /* encoder-decoder control table */
#ifdef __cplusplus
}	/* extern "C" */
#endif /* __cplusplus */
#endif /* OSS_CDRF_2GCDR_Org_M */
