/********************************************************/
/* Copyright (C) 2002 OSS Nokalva.  All rights reserved.*/
/********************************************************/

/* THIS FILE IS PROPRIETARY MATERIAL OF OSS NOKALVA
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF OSS NOKALVA.
 * THIS FILE MAY NOT BE DISTRIBUTED. */

/* Generated for: HuaWei Technologies, Inc., Shenzhen, China - license 8088 for Windows NT */
/* Abstract syntax: CDRF_3GCDR */
/* Created: Mon Aug 12 12:33:13 2002 */
/* ASN.1 compiler version: 5.3 */
/* Target operating system: Windows NT/Windows 9x */
/* Target machine type: Intel x86 */
/* C compiler options required: -Zp4 (Microsoft) */
/* ASN.1 compiler options and file names specified:
 * -codefile CDRF_3GCDR.c -headerfile CDRF_3GCDR.h -errorfile
 * C:\WINDOWS\TEMP\2. -prefix CDR3G_ -nodebug -dualheader -ansi -verbose -ber
 * -root -charintegers
 * D:\application\ossasn1\win32\5.3.0\asn1dflt\asn1dflt.ms.zp4
 * D:\project\asn1app\Oss_3GCDR_z4\jinying\CDRF_3GCDR.asn
 */

#ifndef OSS_CDRF_3GCDR
#define OSS_CDRF_3GCDR

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "asn1hdr.h"
#include "asn1code.h"

#define          CDR3G_CallEventRecord_PDU 1

typedef struct CDR3G__ObjectID {
    unsigned short  length;
    unsigned char   *value;
} CDR3G__ObjectID;

typedef struct CDR3G__Any {
    unsigned long   length;
    unsigned char   *value;
} CDR3G__Any;

typedef int             CDR3G_CallEventRecordType;
#define                     CDR3G_CallEventRecordType_sgsnPDPRecord 18
#define                     CDR3G_CallEventRecordType_ggsnPDPRecord 19
#define                     CDR3G_CallEventRecordType_sgsnMMRecord 20
#define                     CDR3G_CallEventRecordType_sgsnSMORecord 21
#define                     CDR3G_CallEventRecordType_sgsnSMTRecord 22

typedef ossBoolean      CDR3G_NetworkInitiatedPDPContext;

typedef struct CDR3G_IMSI {
    unsigned short  length;
    unsigned char   value[8];
} CDR3G_IMSI;

typedef struct CDR3G_IMEI {
    unsigned short  length;
    unsigned char   value[8];
} CDR3G_IMEI;

typedef struct CDR3G_IPBinaryAddress {
    unsigned short  choice;
#       define      CDR3G_iPBinV4Address_chosen 1
#       define      CDR3G_iPBinV6Address_chosen 2
    union _union {
        struct CDR3G__octet1 {
            unsigned short  length;
            unsigned char   value[4];
        } iPBinV4Address;  /* to choose, set choice to
                            * CDR3G_iPBinV4Address_chosen */
        struct CDR3G__octet2 {
            unsigned short  length;
            unsigned char   value[16];
        } iPBinV6Address;  /* to choose, set choice to
                            * CDR3G_iPBinV6Address_chosen */
    } u;
} CDR3G_IPBinaryAddress;

typedef struct CDR3G_IPTextRepresentedAddress {
    unsigned short  choice;
#       define      CDR3G_iPTextV4Address_chosen 1
#       define      CDR3G_iPTextV6Address_chosen 2
    union _union {
        char            iPTextV4Address[16];  /* to choose, set choice to
                                              * CDR3G_iPTextV4Address_chosen */
        char            iPTextV6Address[46];  /* to choose, set choice to
                                              * CDR3G_iPTextV6Address_chosen */
    } u;
} CDR3G_IPTextRepresentedAddress;

typedef struct CDR3G_IPAddress {
    unsigned short  choice;
#       define      CDR3G_iPBinaryAddress_chosen 1
#       define      CDR3G_iPTextRepresentedAddress_chosen 2
    union _union {
        CDR3G_IPBinaryAddress iPBinaryAddress;  /* to choose, set choice to
                                              * CDR3G_iPBinaryAddress_chosen */
        CDR3G_IPTextRepresentedAddress iPTextRepresentedAddress;  /* to choose,
                                   * set choice to
                                   * CDR3G_iPTextRepresentedAddress_chosen */
    } u;
} CDR3G_IPAddress;

typedef CDR3G_IPAddress CDR3G_GSNAddress;

typedef struct CDR3G_MSNetworkCapability {
    unsigned short  length;
    unsigned char   value[1];
} CDR3G_MSNetworkCapability;

typedef struct CDR3G_RoutingAreaCode {
    unsigned short  length;
    unsigned char   value[1];
} CDR3G_RoutingAreaCode;

typedef struct CDR3G_LocationAreaCode {
    unsigned short  length;
    unsigned char   value[2];
} CDR3G_LocationAreaCode;

typedef struct CDR3G_CellId {
    unsigned short  length;
    unsigned char   value[2];
} CDR3G_CellId;

typedef unsigned int    CDR3G_ChargingID;

typedef char            CDR3G_AccessPointNameNI[64];

typedef struct CDR3G_PDPType {
    unsigned short  length;
    unsigned char   value[2];
} CDR3G_PDPType;

typedef struct CDR3G_AddressString {
    unsigned short  length;
    unsigned char   value[20];
} CDR3G_AddressString;

typedef CDR3G_AddressString CDR3G_ETSIAddress;

typedef struct CDR3G_PDPAddress {
    unsigned short  choice;
#       define      CDR3G_iPAddress_chosen 1
#       define      CDR3G_eTSIAddress_chosen 2
    union _union {
        CDR3G_IPAddress iPAddress;  /* to choose, set choice to
                                     * CDR3G_iPAddress_chosen */
        CDR3G_ETSIAddress eTSIAddress;  /* to choose, set choice to
                                         * CDR3G_eTSIAddress_chosen */
    } u;
} CDR3G_PDPAddress;

typedef struct CDR3G_TimeStamp {
    unsigned short  length;
    unsigned char   value[9];
} CDR3G_TimeStamp;

typedef int             CDR3G_CallDuration;

typedef ossBoolean      CDR3G_SGSNChange;

typedef int             CDR3G_CauseForRecClosing;
#define                     CDR3G_normalRelease 0
#define                     CDR3G_abnormalRelease 4
#define                     CDR3G_cAMELInitCallRelease 5
#define                     CDR3G_volumeLimit 16
#define                     CDR3G_timeLimit 17
#define                     CDR3G_sGSNChange 18
#define                     CDR3G_maxChangeCond 19
#define                     CDR3G_managementIntervention 20

typedef struct CDR3G_ManagementExtension {
    unsigned char   bit_mask;
#       define      CDR3G_significance_present 0x80
    CDR3G__ObjectID identifier;
    ossBoolean      significance;  /* CDR3G_significance_present not set in
                                    * bit_mask implies value is FALSE */
    CDR3G__Any      information;
} CDR3G_ManagementExtension;

typedef struct CDR3G_Diagnostics {
    unsigned short  choice;
#       define      CDR3G_gsm0408Cause_chosen 1
#       define      CDR3G_gsm0902MapErrorValue_chosen 2
#       define      CDR3G_ccittQ767Cause_chosen 3
#       define      CDR3G_networkSpecificCause_chosen 4
#       define      CDR3G_manufacturerSpecificCause_chosen 5
    union _union {
        int             gsm0408Cause;  /* to choose, set choice to
                                        * CDR3G_gsm0408Cause_chosen */
        int             gsm0902MapErrorValue;  /* to choose, set choice to
                                         * CDR3G_gsm0902MapErrorValue_chosen */
        int             ccittQ767Cause;  /* to choose, set choice to
                                          * CDR3G_ccittQ767Cause_chosen */
        CDR3G_ManagementExtension networkSpecificCause;  /* to choose, set
                                   * choice to
                                   * CDR3G_networkSpecificCause_chosen */
        CDR3G_ManagementExtension manufacturerSpecificCause;  /* to choose, set
                                   * choice to
                                   * CDR3G_manufacturerSpecificCause_chosen */
    } u;
} CDR3G_Diagnostics;

typedef char            CDR3G_NodeID[21];

typedef struct CDR3G_ManagementExtensions {
    unsigned int    count;
    CDR3G_ManagementExtension *value;
} CDR3G_ManagementExtensions;

typedef unsigned int    CDR3G_LocalSequenceNumber;

typedef enum CDR3G_APNSelectionMode {
    CDR3G_mSorNetworkProvidedSubscriptionVerified = 0,
    CDR3G_mSProvidedSubscriptionNotVerified = 1,
    CDR3G_networkProvidedSubscriptionNotVerified = 2
} CDR3G_APNSelectionMode;

typedef char            CDR3G_AccessPointNameOI[38];

typedef struct CDR3G_ISDN_AddressString {
    unsigned short  length;
    unsigned char   value[9];
} CDR3G_ISDN_AddressString;

typedef CDR3G_ISDN_AddressString CDR3G_MSISDN;

typedef struct CDR3G_ChargingCharacteristics {
    unsigned short  length;
    unsigned char   value[2];
} CDR3G_ChargingCharacteristics;

typedef enum CDR3G_SystemType {
    CDR3G_unknown = 0,
    CDR3G_iuUTRAN = 1,
    CDR3G_gERAN = 2
} CDR3G_SystemType;

typedef CDR3G_AddressString CDR3G_SCFAddress;

typedef unsigned int    CDR3G_ServiceKey;

typedef enum CDR3G_DefaultGPRS_Handling {
    CDR3G_DefaultGPRS_Handling_continueTransaction = 0,
    CDR3G_DefaultGPRS_Handling_releaseTransaction = 1
} CDR3G_DefaultGPRS_Handling;

typedef CDR3G_AccessPointNameNI CDR3G_CAMELAccessPointNameNI;

typedef CDR3G_AccessPointNameOI CDR3G_CAMELAccessPointNameOI;

typedef int             CDR3G_NumberOfDPEncountered;

typedef unsigned char   CDR3G_LevelOfCAMELService;
#define                     CDR3G_basic 0x80
#define                     CDR3G_callDurationSupervision 0x40
#define                     CDR3G_onlineCharging 0x20

typedef struct CDR3G_FreeFormatData {
    unsigned short  length;
    unsigned char   value[160];
} CDR3G_FreeFormatData;

typedef ossBoolean      CDR3G_FFDAppendIndicator;

typedef struct CDR3G_CAMELInformationPDP {
    unsigned short  bit_mask;
#       define      CDR3G_CAMELInformationPDP_sCFAddress_present 0x8000
#       define      CDR3G_CAMELInformationPDP_serviceKey_present 0x4000
#       define      CDR3G_CAMELInformationPDP_defaultTransactionHandling_present 0x2000
#       define      CDR3G_cAMELAccessPointNameNI_present 0x1000
#       define      CDR3G_cAMELAccessPointNameOI_present 0x0800
#       define      CDR3G_CAMELInformationPDP_numberOfDPEncountered_present 0x0400
#       define      CDR3G_CAMELInformationPDP_levelOfCAMELService_present 0x0200
#       define      CDR3G_CAMELInformationPDP_freeFormatData_present 0x0100
#       define      CDR3G_CAMELInformationPDP_fFDAppendIndicator_present 0x0080
    CDR3G_SCFAddress sCFAddress;  /* optional; set in bit_mask
                              * CDR3G_CAMELInformationPDP_sCFAddress_present if
                              * present */
    CDR3G_ServiceKey serviceKey;  /* optional; set in bit_mask
                              * CDR3G_CAMELInformationPDP_serviceKey_present if
                              * present */
    CDR3G_DefaultGPRS_Handling defaultTransactionHandling;  /* optional; set in
                                   * bit_mask
              * CDR3G_CAMELInformationPDP_defaultTransactionHandling_present if
              * present */
    CDR3G_CAMELAccessPointNameNI cAMELAccessPointNameNI;  /* optional; set in
                                   * bit_mask
                                   * CDR3G_cAMELAccessPointNameNI_present if
                                   * present */
    CDR3G_CAMELAccessPointNameOI cAMELAccessPointNameOI;  /* optional; set in
                                   * bit_mask
                                   * CDR3G_cAMELAccessPointNameOI_present if
                                   * present */
    CDR3G_NumberOfDPEncountered numberOfDPEncountered;  /* optional; set in
                                   * bit_mask
                   * CDR3G_CAMELInformationPDP_numberOfDPEncountered_present if
                   * present */
    CDR3G_LevelOfCAMELService levelOfCAMELService;  /* optional; set in bit_mask
                     * CDR3G_CAMELInformationPDP_levelOfCAMELService_present if
                     * present */
    CDR3G_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                          * CDR3G_CAMELInformationPDP_freeFormatData_present if
                          * present */
    CDR3G_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in bit_mask
                      * CDR3G_CAMELInformationPDP_fFDAppendIndicator_present if
                      * present */
} CDR3G_CAMELInformationPDP;

typedef int             CDR3G_DataVolumeGPRS;

typedef struct CDR3G__seqof1 {
    unsigned int    count;
    struct CDR3G_ChangeOfCharCondition *value;
} CDR3G__seqof1;

struct CDR3G__seqof3 {
     unsigned int    count;
     struct CDR3G_ChangeLocation *value;
};

typedef struct CDR3G_SGSNPDPRecord {
    unsigned int    bit_mask;
#       define      CDR3G_SGSNPDPRecord_networkInitiation_present 0x80000000
#       define      CDR3G_SGSNPDPRecord_servedIMEI_present 0x40000000
#       define      CDR3G_SGSNPDPRecord_msNetworkCapability_present 0x20000000
#       define      CDR3G_SGSNPDPRecord_routingArea_present 0x10000000
#       define      CDR3G_SGSNPDPRecord_locationAreaCode_present 0x08000000
#       define      CDR3G_SGSNPDPRecord_cellIdentifier_present 0x04000000
#       define      CDR3G_SGSNPDPRecord_servedPDPAddress_present 0x02000000
#       define      CDR3G_SGSNPDPRecord_sgsnChange_present 0x01000000
#       define      CDR3G_SGSNPDPRecord_diagnostics_present 0x00800000
#       define      CDR3G_SGSNPDPRecord_recordSequenceNumber_present 0x00400000
#       define      CDR3G_SGSNPDPRecord_nodeID_present 0x00200000
#       define      CDR3G_SGSNPDPRecord_recordExtensions_present 0x00100000
#       define      CDR3G_SGSNPDPRecord_localSequenceNumber_present 0x00080000
#       define      CDR3G_SGSNPDPRecord_apnSelectionMode_present 0x00040000
#       define      CDR3G_SGSNPDPRecord_servedMSISDN_present 0x00020000
#       define      CDR3G_SGSNPDPRecord_chargingCharacteristics_present 0x00010000
#       define      CDR3G_SGSNPDPRecord_systemType_present 0x00008000
#       define      CDR3G_cAMELInformationPDP_present 0x00004000
#       define      CDR3G_rNCUnsentDownlinkVolume_present 0x00002000
    CDR3G_CallEventRecordType recordType;
    CDR3G_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                             * CDR3G_SGSNPDPRecord_networkInitiation_present if
                             * present */
    CDR3G_IMSI      servedIMSI;
    CDR3G_IMEI      servedIMEI;  /* optional; set in bit_mask
                                  * CDR3G_SGSNPDPRecord_servedIMEI_present if
                                  * present */
    CDR3G_GSNAddress sgsnAddress;
    CDR3G_MSNetworkCapability msNetworkCapability;  /* optional; set in bit_mask
                           * CDR3G_SGSNPDPRecord_msNetworkCapability_present if
                           * present */
    CDR3G_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                   * CDR3G_SGSNPDPRecord_routingArea_present if
                                   * present */
    CDR3G_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                              * CDR3G_SGSNPDPRecord_locationAreaCode_present if
                              * present */
    CDR3G_CellId    cellIdentifier;  /* optional; set in bit_mask
                                * CDR3G_SGSNPDPRecord_cellIdentifier_present if
                                * present */
    CDR3G_ChargingID chargingID;
    CDR3G_GSNAddress ggsnAddressUsed;
    CDR3G_AccessPointNameNI accessPointNameNI;
    CDR3G_PDPType   pdpType;
    CDR3G_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                              * CDR3G_SGSNPDPRecord_servedPDPAddress_present if
                              * present */
    CDR3G__seqof1   listOfTrafficVolumes;
    CDR3G_TimeStamp recordOpeningTime;
    CDR3G_CallDuration duration;
    CDR3G_SGSNChange sgsnChange;  /* optional; set in bit_mask
                                   * CDR3G_SGSNPDPRecord_sgsnChange_present if
                                   * present */
    CDR3G_CauseForRecClosing causeForRecClosing;
    CDR3G_Diagnostics diagnostics;  /* optional; set in bit_mask
                                     * CDR3G_SGSNPDPRecord_diagnostics_present
                                     * if present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                          * CDR3G_SGSNPDPRecord_recordSequenceNumber_present if
                          * present */
    CDR3G_NodeID    nodeID;  /* optional; set in bit_mask
                              * CDR3G_SGSNPDPRecord_nodeID_present if present */
    CDR3G_ManagementExtensions recordExtensions;  /* optional; set in bit_mask
                              * CDR3G_SGSNPDPRecord_recordExtensions_present if
                              * present */
    CDR3G_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                           * CDR3G_SGSNPDPRecord_localSequenceNumber_present if
                           * present */
    CDR3G_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                              * CDR3G_SGSNPDPRecord_apnSelectionMode_present if
                              * present */
    CDR3G_AccessPointNameOI accessPointNameOI;
    CDR3G_MSISDN    servedMSISDN;  /* optional; set in bit_mask
                                    * CDR3G_SGSNPDPRecord_servedMSISDN_present
                                    * if present */
    CDR3G_ChargingCharacteristics chargingCharacteristics;  /* optional; set in
                                   * bit_mask
                       * CDR3G_SGSNPDPRecord_chargingCharacteristics_present if
                       * present */
    CDR3G_SystemType systemType;  /* optional; set in bit_mask
                                   * CDR3G_SGSNPDPRecord_systemType_present if
                                   * present */
    CDR3G_CAMELInformationPDP cAMELInformationPDP;  /* optional; set in bit_mask
                                         * CDR3G_cAMELInformationPDP_present if
                                         * present */
    CDR3G_DataVolumeGPRS rNCUnsentDownlinkVolume;  /* optional; set in bit_mask
                                     * CDR3G_rNCUnsentDownlinkVolume_present if
                                     * present */
} CDR3G_SGSNPDPRecord;

typedef ossBoolean      CDR3G_DynamicAddressFlag;

typedef struct CDR3G_GGSNPDPRecord {
    unsigned short  bit_mask;
#       define      CDR3G_GGSNPDPRecord_networkInitiation_present 0x8000
#       define      CDR3G_GGSNPDPRecord_servedPDPAddress_present 0x4000
#       define      CDR3G_dynamicAddressFlag_present 0x2000
#       define      CDR3G_GGSNPDPRecord_diagnostics_present 0x1000
#       define      CDR3G_GGSNPDPRecord_recordSequenceNumber_present 0x0800
#       define      CDR3G_GGSNPDPRecord_nodeID_present 0x0400
#       define      CDR3G_GGSNPDPRecord_recordExtensions_present 0x0200
#       define      CDR3G_GGSNPDPRecord_localSequenceNumber_present 0x0100
#       define      CDR3G_GGSNPDPRecord_apnSelectionMode_present 0x0080
#       define      CDR3G_GGSNPDPRecord_servedMSISDN_present 0x0040
#       define      CDR3G_GGSNPDPRecord_chargingCharacteristics_present 0x0020
    CDR3G_CallEventRecordType recordType;
    CDR3G_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                             * CDR3G_GGSNPDPRecord_networkInitiation_present if
                             * present */
    CDR3G_IMSI      servedIMSI;
    CDR3G_GSNAddress ggsnAddress;
    CDR3G_ChargingID chargingID;
    struct CDR3G__seqof2 {
        unsigned int    count;
        CDR3G_GSNAddress *value;
    } sgsnAddress;
    CDR3G_AccessPointNameNI accessPointNameNI;
    CDR3G_PDPType   pdpType;
    CDR3G_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                              * CDR3G_GGSNPDPRecord_servedPDPAddress_present if
                              * present */
    CDR3G_DynamicAddressFlag dynamicAddressFlag;  /* optional; set in bit_mask
                                          * CDR3G_dynamicAddressFlag_present if
                                          * present */
    CDR3G__seqof1   listOfTrafficVolumes;
    CDR3G_TimeStamp recordOpeningTime;
    CDR3G_CallDuration duration;
    CDR3G_CauseForRecClosing causeForRecClosing;
    CDR3G_Diagnostics diagnostics;  /* optional; set in bit_mask
                                     * CDR3G_GGSNPDPRecord_diagnostics_present
                                     * if present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                          * CDR3G_GGSNPDPRecord_recordSequenceNumber_present if
                          * present */
    CDR3G_NodeID    nodeID;  /* optional; set in bit_mask
                              * CDR3G_GGSNPDPRecord_nodeID_present if present */
    CDR3G_ManagementExtensions recordExtensions;  /* optional; set in bit_mask
                              * CDR3G_GGSNPDPRecord_recordExtensions_present if
                              * present */
    CDR3G_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                           * CDR3G_GGSNPDPRecord_localSequenceNumber_present if
                           * present */
    CDR3G_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                              * CDR3G_GGSNPDPRecord_apnSelectionMode_present if
                              * present */
    CDR3G_MSISDN    servedMSISDN;  /* optional; set in bit_mask
                                    * CDR3G_GGSNPDPRecord_servedMSISDN_present
                                    * if present */
    CDR3G_ChargingCharacteristics chargingCharacteristics;  /* optional; set in
                                   * bit_mask
                       * CDR3G_GGSNPDPRecord_chargingCharacteristics_present if
                       * present */
} CDR3G_GGSNPDPRecord;

typedef struct CDR3G_CAMELInformationMM {
    unsigned char   bit_mask;
#       define      CDR3G_CAMELInformationMM_sCFAddress_present 0x80
#       define      CDR3G_CAMELInformationMM_serviceKey_present 0x40
#       define      CDR3G_CAMELInformationMM_defaultTransactionHandling_present 0x20
#       define      CDR3G_CAMELInformationMM_numberOfDPEncountered_present 0x10
#       define      CDR3G_CAMELInformationMM_levelOfCAMELService_present 0x08
#       define      CDR3G_CAMELInformationMM_freeFormatData_present 0x04
#       define      CDR3G_CAMELInformationMM_fFDAppendIndicator_present 0x02
    CDR3G_SCFAddress sCFAddress;  /* optional; set in bit_mask
                                   * CDR3G_CAMELInformationMM_sCFAddress_present
                                   * if present */
    CDR3G_ServiceKey serviceKey;  /* optional; set in bit_mask
                                   * CDR3G_CAMELInformationMM_serviceKey_present
                                   * if present */
    CDR3G_DefaultGPRS_Handling defaultTransactionHandling;  /* optional; set in
                                   * bit_mask
               * CDR3G_CAMELInformationMM_defaultTransactionHandling_present if
               * present */
    CDR3G_NumberOfDPEncountered numberOfDPEncountered;  /* optional; set in
                                   * bit_mask
                    * CDR3G_CAMELInformationMM_numberOfDPEncountered_present if
                    * present */
    CDR3G_LevelOfCAMELService levelOfCAMELService;  /* optional; set in bit_mask
                      * CDR3G_CAMELInformationMM_levelOfCAMELService_present if
                      * present */
    CDR3G_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                           * CDR3G_CAMELInformationMM_freeFormatData_present if
                           * present */
    CDR3G_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in bit_mask
                       * CDR3G_CAMELInformationMM_fFDAppendIndicator_present if
                       * present */
} CDR3G_CAMELInformationMM;

typedef struct CDR3G_SGSNMMRecord {
    unsigned int    bit_mask;
#       define      CDR3G_SGSNMMRecord_servedIMEI_present 0x80000000
#       define      CDR3G_SGSNMMRecord_msNetworkCapability_present 0x40000000
#       define      CDR3G_SGSNMMRecord_routingArea_present 0x20000000
#       define      CDR3G_SGSNMMRecord_locationAreaCode_present 0x10000000
#       define      CDR3G_SGSNMMRecord_cellIdentifier_present 0x08000000
#       define      CDR3G_changeLocation_present 0x04000000
#       define      CDR3G_duration_present 0x02000000
#       define      CDR3G_SGSNMMRecord_sgsnChange_present 0x01000000
#       define      CDR3G_SGSNMMRecord_diagnostics_present 0x00800000
#       define      CDR3G_SGSNMMRecord_recordSequenceNumber_present 0x00400000
#       define      CDR3G_SGSNMMRecord_nodeID_present 0x00200000
#       define      CDR3G_SGSNMMRecord_recordExtensions_present 0x00100000
#       define      CDR3G_SGSNMMRecord_localSequenceNumber_present 0x00080000
#       define      CDR3G_SGSNMMRecord_servedMSISDN_present 0x00040000
#       define      CDR3G_SGSNMMRecord_chargingCharacteristics_present 0x00020000
#       define      CDR3G_cAMELInformationMM_present 0x00010000
#       define      CDR3G_SGSNMMRecord_systemType_present 0x00008000
    CDR3G_CallEventRecordType recordType;
    CDR3G_IMSI      servedIMSI;
    CDR3G_IMEI      servedIMEI;  /* optional; set in bit_mask
                                  * CDR3G_SGSNMMRecord_servedIMEI_present if
                                  * present */
    CDR3G_GSNAddress sgsnAddress;
    CDR3G_MSNetworkCapability msNetworkCapability;  /* optional; set in bit_mask
                            * CDR3G_SGSNMMRecord_msNetworkCapability_present if
                            * present */
    CDR3G_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                    * CDR3G_SGSNMMRecord_routingArea_present if
                                    * present */
    CDR3G_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                               * CDR3G_SGSNMMRecord_locationAreaCode_present if
                               * present */
    CDR3G_CellId    cellIdentifier;  /* optional; set in bit_mask
                                 * CDR3G_SGSNMMRecord_cellIdentifier_present if
                                 * present */
    struct CDR3G__seqof3  changeLocation;  /* optional; set in bit_mask CDR3G_changeLocation_present
                                            * if present */
    CDR3G_TimeStamp recordOpeningTime;
    CDR3G_CallDuration duration;  /* optional; set in bit_mask
                                   * CDR3G_duration_present if present */
    CDR3G_SGSNChange sgsnChange;  /* optional; set in bit_mask
                                   * CDR3G_SGSNMMRecord_sgsnChange_present if
                                   * present */
    CDR3G_CauseForRecClosing causeForRecClosing;
    CDR3G_Diagnostics diagnostics;  /* optional; set in bit_mask
                                     * CDR3G_SGSNMMRecord_diagnostics_present if
                                     * present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                           * CDR3G_SGSNMMRecord_recordSequenceNumber_present if
                           * present */
    CDR3G_NodeID    nodeID;  /* optional; set in bit_mask
                              * CDR3G_SGSNMMRecord_nodeID_present if present */
    CDR3G_ManagementExtensions recordExtensions;  /* optional; set in bit_mask
                               * CDR3G_SGSNMMRecord_recordExtensions_present if
                               * present */
    CDR3G_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                            * CDR3G_SGSNMMRecord_localSequenceNumber_present if
                            * present */
    CDR3G_MSISDN    servedMSISDN;  /* optional; set in bit_mask
                                    * CDR3G_SGSNMMRecord_servedMSISDN_present if
                                    * present */
    CDR3G_ChargingCharacteristics chargingCharacteristics;  /* optional; set in
                                   * bit_mask
                        * CDR3G_SGSNMMRecord_chargingCharacteristics_present if
                        * present */
    CDR3G_CAMELInformationMM cAMELInformationMM;  /* optional; set in bit_mask
                                          * CDR3G_cAMELInformationMM_present if
                                          * present */
    CDR3G_SystemType systemType;  /* optional; set in bit_mask
                                   * CDR3G_SGSNMMRecord_systemType_present if
                                   * present */
} CDR3G_SGSNMMRecord;

typedef CDR3G_AddressString CDR3G_RecordingEntity;

typedef struct CDR3G_MessageReference {
    unsigned int    length;
    unsigned char   *value;
} CDR3G_MessageReference;

typedef CDR3G_Diagnostics CDR3G_SMSResult;

typedef struct CDR3G_CalledNumber {
    unsigned short  length;
    unsigned char   value[10];
} CDR3G_CalledNumber;

typedef enum CDR3G_DefaultSMS_Handling {
    CDR3G_DefaultSMS_Handling_continueTransaction = 0,
    CDR3G_DefaultSMS_Handling_releaseTransaction = 1
} CDR3G_DefaultSMS_Handling;

typedef struct CDR3G_CallingNumber {
    unsigned short  length;
    unsigned char   value[10];
} CDR3G_CallingNumber;

typedef struct CDR3G_CAMELInformationSMS {
    unsigned char   bit_mask;
#       define      CDR3G_CAMELInformationSMS_sCFAddress_present 0x80
#       define      CDR3G_CAMELInformationSMS_serviceKey_present 0x40
#       define      CDR3G_defaultSMSHandling_present 0x20
#       define      CDR3G_cAMELCallingPartyNumber_present 0x10
#       define      CDR3G_cAMELDestinationSubscriberNumber_present 0x08
#       define      CDR3G_cAMELSMSCAddress_present 0x04
#       define      CDR3G_CAMELInformationSMS_freeFormatData_present 0x02
    CDR3G_SCFAddress sCFAddress;  /* optional; set in bit_mask
                              * CDR3G_CAMELInformationSMS_sCFAddress_present if
                              * present */
    CDR3G_ServiceKey serviceKey;  /* optional; set in bit_mask
                              * CDR3G_CAMELInformationSMS_serviceKey_present if
                              * present */
    CDR3G_DefaultSMS_Handling defaultSMSHandling;  /* optional; set in bit_mask
                                          * CDR3G_defaultSMSHandling_present if
                                          * present */
    CDR3G_CallingNumber cAMELCallingPartyNumber;  /* optional; set in bit_mask
                                     * CDR3G_cAMELCallingPartyNumber_present if
                                     * present */
    CDR3G_CalledNumber cAMELDestinationSubscriberNumber;  /* optional; set in
                                   * bit_mask
                            * CDR3G_cAMELDestinationSubscriberNumber_present if
                            * present */
    CDR3G_AddressString cAMELSMSCAddress;  /* optional; set in bit_mask
                                            * CDR3G_cAMELSMSCAddress_present if
                                            * present */
    CDR3G_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                          * CDR3G_CAMELInformationSMS_freeFormatData_present if
                          * present */
} CDR3G_CAMELInformationSMS;

typedef struct CDR3G_SGSNSMORecord {
    unsigned short  bit_mask;
#       define      CDR3G_SGSNSMORecord_servedIMEI_present 0x8000
#       define      CDR3G_SGSNSMORecord_servedMSISDN_present 0x4000
#       define      CDR3G_SGSNSMORecord_locationArea_present 0x2000
#       define      CDR3G_SGSNSMORecord_routingArea_present 0x1000
#       define      CDR3G_SGSNSMORecord_cellIdentifier_present 0x0800
#       define      CDR3G_SGSNSMORecord_smsResult_present 0x0400
#       define      CDR3G_SGSNSMORecord_recordExtensions_present 0x0200
#       define      CDR3G_SGSNSMORecord_nodeID_present 0x0100
#       define      CDR3G_SGSNSMORecord_localSequenceNumber_present 0x0080
#       define      CDR3G_SGSNSMORecord_chargingCharacteristics_present 0x0040
#       define      CDR3G_SGSNSMORecord_systemType_present 0x0020
#       define      CDR3G_destinationNumber_present 0x0010
#       define      CDR3G_cAMELInformationSMS_present 0x0008
    CDR3G_CallEventRecordType recordType;
    CDR3G_IMSI      servedIMSI;
    CDR3G_IMEI      servedIMEI;  /* optional; set in bit_mask
                                  * CDR3G_SGSNSMORecord_servedIMEI_present if
                                  * present */
    CDR3G_MSISDN    servedMSISDN;  /* optional; set in bit_mask
                                    * CDR3G_SGSNSMORecord_servedMSISDN_present
                                    * if present */
    CDR3G_MSNetworkCapability msNetworkCapability;
    CDR3G_AddressString serviceCentre;
    CDR3G_RecordingEntity recordingEntity;
    CDR3G_LocationAreaCode locationArea;  /* optional; set in bit_mask
                                  * CDR3G_SGSNSMORecord_locationArea_present if
                                  * present */
    CDR3G_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                   * CDR3G_SGSNSMORecord_routingArea_present if
                                   * present */
    CDR3G_CellId    cellIdentifier;  /* optional; set in bit_mask
                                * CDR3G_SGSNSMORecord_cellIdentifier_present if
                                * present */
    CDR3G_MessageReference messageReference;
    CDR3G_TimeStamp originationTime;
    CDR3G_SMSResult smsResult;  /* optional; set in bit_mask
                                 * CDR3G_SGSNSMORecord_smsResult_present if
                                 * present */
    CDR3G_ManagementExtensions recordExtensions;  /* optional; set in bit_mask
                              * CDR3G_SGSNSMORecord_recordExtensions_present if
                              * present */
    CDR3G_NodeID    nodeID;  /* optional; set in bit_mask
                              * CDR3G_SGSNSMORecord_nodeID_present if present */
    CDR3G_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                           * CDR3G_SGSNSMORecord_localSequenceNumber_present if
                           * present */
    CDR3G_ChargingCharacteristics chargingCharacteristics;  /* optional; set in
                                   * bit_mask
                       * CDR3G_SGSNSMORecord_chargingCharacteristics_present if
                       * present */
    CDR3G_SystemType systemType;  /* optional; set in bit_mask
                                   * CDR3G_SGSNSMORecord_systemType_present if
                                   * present */
    CDR3G_CalledNumber destinationNumber;  /* optional; set in bit_mask
                                            * CDR3G_destinationNumber_present if
                                            * present */
    CDR3G_CAMELInformationSMS cAMELInformationSMS;  /* optional; set in bit_mask
                                         * CDR3G_cAMELInformationSMS_present if
                                         * present */
} CDR3G_SGSNSMORecord;

typedef struct CDR3G_SGSNSMTRecord {
    unsigned short  bit_mask;
#       define      CDR3G_SGSNSMTRecord_servedIMEI_present 0x8000
#       define      CDR3G_SGSNSMTRecord_servedMSISDN_present 0x4000
#       define      CDR3G_SGSNSMTRecord_locationArea_present 0x2000
#       define      CDR3G_SGSNSMTRecord_routingArea_present 0x1000
#       define      CDR3G_SGSNSMTRecord_cellIdentifier_present 0x0800
#       define      CDR3G_SGSNSMTRecord_smsResult_present 0x0400
#       define      CDR3G_SGSNSMTRecord_recordExtensions_present 0x0200
#       define      CDR3G_SGSNSMTRecord_nodeID_present 0x0100
#       define      CDR3G_SGSNSMTRecord_localSequenceNumber_present 0x0080
#       define      CDR3G_SGSNSMTRecord_chargingCharacteristics_present 0x0040
#       define      CDR3G_SGSNSMTRecord_systemType_present 0x0020
    CDR3G_CallEventRecordType recordType;
    CDR3G_IMSI      servedIMSI;
    CDR3G_IMEI      servedIMEI;  /* optional; set in bit_mask
                                  * CDR3G_SGSNSMTRecord_servedIMEI_present if
                                  * present */
    CDR3G_MSISDN    servedMSISDN;  /* optional; set in bit_mask
                                    * CDR3G_SGSNSMTRecord_servedMSISDN_present
                                    * if present */
    CDR3G_MSNetworkCapability msNetworkCapability;
    CDR3G_AddressString serviceCentre;
    CDR3G_RecordingEntity recordingEntity;
    CDR3G_LocationAreaCode locationArea;  /* optional; set in bit_mask
                                  * CDR3G_SGSNSMTRecord_locationArea_present if
                                  * present */
    CDR3G_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                   * CDR3G_SGSNSMTRecord_routingArea_present if
                                   * present */
    CDR3G_CellId    cellIdentifier;  /* optional; set in bit_mask
                                * CDR3G_SGSNSMTRecord_cellIdentifier_present if
                                * present */
    CDR3G_TimeStamp originationTime;
    CDR3G_SMSResult smsResult;  /* optional; set in bit_mask
                                 * CDR3G_SGSNSMTRecord_smsResult_present if
                                 * present */
    CDR3G_ManagementExtensions recordExtensions;  /* optional; set in bit_mask
                              * CDR3G_SGSNSMTRecord_recordExtensions_present if
                              * present */
    CDR3G_NodeID    nodeID;  /* optional; set in bit_mask
                              * CDR3G_SGSNSMTRecord_nodeID_present if present */
    CDR3G_LocalSequenceNumber localSequenceNumber;  /* optional; set in bit_mask
                           * CDR3G_SGSNSMTRecord_localSequenceNumber_present if
                           * present */
    CDR3G_ChargingCharacteristics chargingCharacteristics;  /* optional; set in
                                   * bit_mask
                       * CDR3G_SGSNSMTRecord_chargingCharacteristics_present if
                       * present */
    CDR3G_SystemType systemType;  /* optional; set in bit_mask
                                   * CDR3G_SGSNSMTRecord_systemType_present if
                                   * present */
} CDR3G_SGSNSMTRecord;

typedef struct CDR3G_CallEventRecord {
    unsigned short  choice;
#       define      CDR3G_sgsnPDPRecord_chosen 1
#       define      CDR3G_ggsnPDPRecord_chosen 2
#       define      CDR3G_sgsnMMRecord_chosen 3
#       define      CDR3G_sgsnSMORecord_chosen 4
#       define      CDR3G_sgsnSMTRecord_chosen 5
    union _union {
        CDR3G_SGSNPDPRecord sgsnPDPRecord;  /* to choose, set choice to
                                             * CDR3G_sgsnPDPRecord_chosen */
        CDR3G_GGSNPDPRecord ggsnPDPRecord;  /* to choose, set choice to
                                             * CDR3G_ggsnPDPRecord_chosen */
        CDR3G_SGSNMMRecord sgsnMMRecord;  /* to choose, set choice to
                                           * CDR3G_sgsnMMRecord_chosen */
        CDR3G_SGSNSMORecord sgsnSMORecord;  /* to choose, set choice to
                                             * CDR3G_sgsnSMORecord_chosen */
        CDR3G_SGSNSMTRecord sgsnSMTRecord;  /* to choose, set choice to
                                             * CDR3G_sgsnSMTRecord_chosen */
    } u;
} CDR3G_CallEventRecord;

typedef enum CDR3G_ChangeCondition {
    CDR3G_qoSChange = 0,
    CDR3G_tariffTime = 1,
    CDR3G_recordClosure = 2
} CDR3G_ChangeCondition;

typedef enum CDR3G_QoSReliability {
    CDR3G_unspecifiedReliability = 0,
    CDR3G_acknowledgedGTP = 1,
    CDR3G_unackGTPAcknowLLC = 2,
    CDR3G_unackGTPLLCAcknowRLC = 3,
    CDR3G_unackGTPLLCRLC = 4,
    CDR3G_unacknowUnprotectedData = 5
} CDR3G_QoSReliability;

typedef enum CDR3G_QoSDelay {
    CDR3G_delayClass1 = 1,
    CDR3G_delayClass2 = 2,
    CDR3G_delayClass3 = 3,
    CDR3G_delayClass4 = 4
} CDR3G_QoSDelay;

typedef enum CDR3G_QoSPrecedence {
    CDR3G_QoSPrecedence_unspecified = 0,
    CDR3G_highPriority = 1,
    CDR3G_normalPriority = 2,
    CDR3G_lowPriority = 3
} CDR3G_QoSPrecedence;

typedef enum CDR3G_QoSPeakThroughput {
    CDR3G_QoSPeakThroughput_unspecified = 0,
    CDR3G_upTo1000octetPs = 1,
    CDR3G_upTo2000octetPs = 2,
    CDR3G_upTo4000octetPs = 3,
    CDR3G_upTo8000octetPs = 4,
    CDR3G_upTo16000octetPs = 5,
    CDR3G_upTo32000octetPs = 6,
    CDR3G_upTo64000octetPs = 7,
    CDR3G_upTo128000octetPs = 8,
    CDR3G_upTo256000octetPs = 9
} CDR3G_QoSPeakThroughput;

typedef enum CDR3G_QoSMeanThroughput {
    CDR3G_bestEffort = 0,
    CDR3G_mean100octetPh = 1,
    CDR3G_mean200octetPh = 2,
    CDR3G_mean500octetPh = 3,
    CDR3G_mean1000octetPh = 4,
    CDR3G_mean2000octetPh = 5,
    CDR3G_mean5000octetPh = 6,
    CDR3G_mean10000octetPh = 7,
    CDR3G_mean20000octetPh = 8,
    CDR3G_mean50000octetPh = 9,
    CDR3G_mean100000octetPh = 10,
    CDR3G_mean200000octetPh = 11,
    CDR3G_mean500000octetPh = 12,
    CDR3G_mean1000000octetPh = 13,
    CDR3G_mean2000000octetPh = 14,
    CDR3G_mean5000000octetPh = 15,
    CDR3G_mean10000000octetPh = 16,
    CDR3G_mean20000000octetPh = 17,
    CDR3G_mean50000000octetPh = 18
} CDR3G_QoSMeanThroughput;

typedef struct CDR3G_GSMQoSInformation {
    CDR3G_QoSReliability reliability;
    CDR3G_QoSDelay  delay;
    CDR3G_QoSPrecedence precedence;
    CDR3G_QoSPeakThroughput peakThroughput;
    CDR3G_QoSMeanThroughput meanThroughput;
} CDR3G_GSMQoSInformation;

typedef struct CDR3G_QoSInformation {
    unsigned short  choice;
#       define      CDR3G_gsmQosInformation_chosen 1
#       define      CDR3G_umtsQosInformation_chosen 2
    union _union {
        CDR3G_GSMQoSInformation gsmQosInformation;  /* to choose, set choice to
                                            * CDR3G_gsmQosInformation_chosen */
        struct CDR3G__octet3 {
            unsigned short  length;
            unsigned char   value[12];
        } umtsQosInformation;  /* to choose, set choice to
                                * CDR3G_umtsQosInformation_chosen */
    } u;
} CDR3G_QoSInformation;

typedef struct CDR3G_ChangeOfCharCondition {
    unsigned char   bit_mask;
#       define      CDR3G_qosRequested_present 0x80
#       define      CDR3G_qosNegotiated_present 0x40
    CDR3G_QoSInformation qosRequested;  /* optional; set in bit_mask
                                         * CDR3G_qosRequested_present if
                                         * present */
    CDR3G_QoSInformation qosNegotiated;  /* optional; set in bit_mask
                                          * CDR3G_qosNegotiated_present if
                                          * present */
    CDR3G_DataVolumeGPRS dataVolumeGPRSUplink;
    CDR3G_DataVolumeGPRS dataVolumeGPRSDownlink;
    CDR3G_ChangeCondition changeCondition;
    CDR3G_TimeStamp changeTime;
} CDR3G_ChangeOfCharCondition;

typedef struct CDR3G_ChangeLocation {
    unsigned char   bit_mask;
#       define      CDR3G_cellId_present 0x80
    CDR3G_LocationAreaCode locationAreaCode;
    CDR3G_RoutingAreaCode routingAreaCode;
    CDR3G_CellId    cellId;  /* optional; set in bit_mask CDR3G_cellId_present
                              * if present */
    CDR3G_TimeStamp changeTime;
} CDR3G_ChangeLocation;

extern CDR3G__ObjectID CDR3G_gsm1205InformationModel;

extern CDR3G__ObjectID CDR3G_gsm1205ASN1Module;

extern int CDR3G_maxAddressLength;

extern int CDR3G_maxISDN_AddressLength;


extern void *CDRF_3GCDR;    /* encoder-decoder control table */
#ifdef __cplusplus
}	/* extern "C" */
#endif /* __cplusplus */
#endif /* OSS_CDRF_3GCDR */
