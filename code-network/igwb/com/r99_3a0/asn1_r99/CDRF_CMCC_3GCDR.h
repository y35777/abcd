/********************************************************/
/* Copyright (C) 2003 OSS Nokalva.  All rights reserved.*/
/********************************************************/

/* THIS FILE IS PROPRIETARY MATERIAL OF OSS NOKALVA
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF OSS NOKALVA.
 * THIS FILE MAY NOT BE DISTRIBUTED. */

/* Generated for: HuaWei Technologies, Inc., Shenzhen, China - license 8088 for Windows NT */
/* Abstract syntax: CDRF_R99_Org */
/* Created: Fri Dec 19 17:30:01 2003 */
/* ASN.1 compiler version: 5.3 */
/* Target operating system: Windows NT/Windows 9x */
/* Target machine type: Intel x86 */
/* C compiler options required: -Zp4 (Microsoft) */
/* ASN.1 compiler options and file names specified:
 * -codefile CDRF_CMCC_3GCDR.C -headerfile CDRF_CMCC_3GCDR.h -errorfile
 * C:\WINDOWS\TEMP\2. -prefix CMCC_3GCDR_ -nodebug -dualheader -ansi -verbose
 * -ber -root -charintegers
 * D:\Application\ossasn1\win32\5.3.0\asn1dflt\asn1dflt.ms.zp4
 * D:\yutemp\lidufang\1218\R99\CDRF_CMCC_3GCDR\CDRF_R99_Org.asn
 */

#ifndef OSS_CDRF_R99_Org
#define OSS_CDRF_R99_Org

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "asn1hdr.h"
#include "asn1code.h"

#define          CMCC_3GCDR_CallEventRecord_PDU 1

typedef struct CMCC_3GCDR__ObjectID {
    unsigned short  length;
    unsigned char   *value;
} CMCC_3GCDR__ObjectID;

typedef int             CMCC_3GCDR_CallEventRecordType;
#define                     CMCC_3GCDR_CallEventRecordType_sgsnPDPRecord 18
#define                     CMCC_3GCDR_CallEventRecordType_ggsnPDPRecord 19
#define                     CMCC_3GCDR_CallEventRecordType_sgsnMMRecord 20
#define                     CMCC_3GCDR_CallEventRecordType_sgsnSMORecord 21
#define                     CMCC_3GCDR_CallEventRecordType_sgsnSMTRecord 22

typedef ossBoolean      CMCC_3GCDR_NetworkInitiatedPDPContext;

typedef struct CMCC_3GCDR_IMSI {
    unsigned short  length;
    unsigned char   value[8];
} CMCC_3GCDR_IMSI;

typedef struct CMCC_3GCDR_IMEI {
    unsigned short  length;
    unsigned char   value[8];
} CMCC_3GCDR_IMEI;

typedef struct CMCC_3GCDR_IPBinaryAddress {
    unsigned short  choice;
#       define      CMCC_3GCDR_iPBinV4Address_chosen 1
#       define      CMCC_3GCDR_iPBinV6Address_chosen 2
    union _union {
        struct CMCC_3GCDR__octet1 {
            unsigned short  length;
            unsigned char   value[4];
        } iPBinV4Address;  /* to choose, set choice to
                            * CMCC_3GCDR_iPBinV4Address_chosen */
        struct CMCC_3GCDR__octet2 {
            unsigned short  length;
            unsigned char   value[16];
        } iPBinV6Address;  /* to choose, set choice to
                            * CMCC_3GCDR_iPBinV6Address_chosen */
    } u;
} CMCC_3GCDR_IPBinaryAddress;

typedef struct CMCC_3GCDR_IPTextRepresentedAddress {
    unsigned short  choice;
#       define      CMCC_3GCDR_iPTextV4Address_chosen 1
#       define      CMCC_3GCDR_iPTextV6Address_chosen 2
    union _union {
        char            iPTextV4Address[16];  /* to choose, set choice to
                                         * CMCC_3GCDR_iPTextV4Address_chosen */
        char            iPTextV6Address[46];  /* to choose, set choice to
                                         * CMCC_3GCDR_iPTextV6Address_chosen */
    } u;
} CMCC_3GCDR_IPTextRepresentedAddress;

typedef struct CMCC_3GCDR_IPAddress {
    unsigned short  choice;
#       define      CMCC_3GCDR_iPBinaryAddress_chosen 1
#       define      CMCC_3GCDR_iPTextRepresentedAddress_chosen 2
    union _union {
        CMCC_3GCDR_IPBinaryAddress iPBinaryAddress;  /* to choose, set choice to
                                         * CMCC_3GCDR_iPBinaryAddress_chosen */
        CMCC_3GCDR_IPTextRepresentedAddress iPTextRepresentedAddress;  /* to
                                   * choose, set choice to
                                * CMCC_3GCDR_iPTextRepresentedAddress_chosen */
    } u;
} CMCC_3GCDR_IPAddress;

typedef CMCC_3GCDR_IPAddress CMCC_3GCDR_GSNAddress;

typedef struct CMCC_3GCDR_MSNetworkCapability {
    unsigned short  length;
    unsigned char   value[8];
} CMCC_3GCDR_MSNetworkCapability;

typedef struct CMCC_3GCDR_RoutingAreaCode {
    unsigned short  length;
    unsigned char   value[1];
} CMCC_3GCDR_RoutingAreaCode;

typedef struct CMCC_3GCDR_LocationAreaCode {
    unsigned short  length;
    unsigned char   value[2];
} CMCC_3GCDR_LocationAreaCode;

typedef struct CMCC_3GCDR_CellId {
    unsigned short  length;
    unsigned char   value[2];
} CMCC_3GCDR_CellId;

typedef unsigned int    CMCC_3GCDR_ChargingID;

typedef char            CMCC_3GCDR_AccessPointNameNI[64];

typedef struct CMCC_3GCDR_PDPType {
    unsigned short  length;
    unsigned char   value[2];
} CMCC_3GCDR_PDPType;

typedef struct CMCC_3GCDR_AddressString {
    unsigned short  length;
    unsigned char   value[20];
} CMCC_3GCDR_AddressString;

typedef CMCC_3GCDR_AddressString CMCC_3GCDR_ETSIAddress;

typedef struct CMCC_3GCDR_PDPAddress {
    unsigned short  choice;
#       define      CMCC_3GCDR_iPAddress_chosen 1
#       define      CMCC_3GCDR_eTSIAddress_chosen 2
    union _union {
        CMCC_3GCDR_IPAddress iPAddress;  /* to choose, set choice to
                                          * CMCC_3GCDR_iPAddress_chosen */
        CMCC_3GCDR_ETSIAddress eTSIAddress;  /* to choose, set choice to
                                             * CMCC_3GCDR_eTSIAddress_chosen */
    } u;
} CMCC_3GCDR_PDPAddress;

typedef struct CMCC_3GCDR_TimeStamp {
    unsigned short  length;
    unsigned char   value[9];
} CMCC_3GCDR_TimeStamp;

typedef int             CMCC_3GCDR_CallDuration;

typedef ossBoolean      CMCC_3GCDR_SGSNChange;

typedef int             CMCC_3GCDR_CauseForRecClosing;
#define                     CMCC_3GCDR_normalRelease 0
#define                     CMCC_3GCDR_abnormalRelease 4
#define                     CMCC_3GCDR_cAMELInitCallRelease 5
#define                     CMCC_3GCDR_volumeLimit 16
#define                     CMCC_3GCDR_timeLimit 17
#define                     CMCC_3GCDR_sGSNChange 18
#define                     CMCC_3GCDR_maxChangeCond 19
#define                     CMCC_3GCDR_managementIntervention 20

struct CMCC_3GCDR__octet3 {
    unsigned int    length;
    unsigned char   *value;
}; 

typedef struct CMCC_3GCDR_ManagementExtension {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_significance_present 0x80
    CMCC_3GCDR__ObjectID identifier;
    ossBoolean      significance;  /* CMCC_3GCDR_significance_present not set in
                                    * bit_mask implies value is FALSE */
    struct CMCC_3GCDR__octet3 information;
} CMCC_3GCDR_ManagementExtension;

typedef struct CMCC_3GCDR_Diagnostics {
    unsigned short  choice;
#       define      CMCC_3GCDR_gsm0408Cause_chosen 1
#       define      CMCC_3GCDR_gsm0902MapErrorValue_chosen 2
#       define      CMCC_3GCDR_ccittQ767Cause_chosen 3
#       define      CMCC_3GCDR_networkSpecificCause_chosen 4
#       define      CMCC_3GCDR_manufacturerSpecificCause_chosen 5
    union _union {
        int             gsm0408Cause;  /* to choose, set choice to
                                        * CMCC_3GCDR_gsm0408Cause_chosen */
        int             gsm0902MapErrorValue;  /* to choose, set choice to
                                    * CMCC_3GCDR_gsm0902MapErrorValue_chosen */
        int             ccittQ767Cause;  /* to choose, set choice to
                                          * CMCC_3GCDR_ccittQ767Cause_chosen */
        CMCC_3GCDR_ManagementExtension networkSpecificCause;  /* to choose, set
                                   * choice to
                                   * CMCC_3GCDR_networkSpecificCause_chosen */
        CMCC_3GCDR_ManagementExtension manufacturerSpecificCause;  /* to choose,
                                   * set choice to
                               * CMCC_3GCDR_manufacturerSpecificCause_chosen */
    } u;
} CMCC_3GCDR_Diagnostics;

typedef char            CMCC_3GCDR_NodeID[21];

typedef struct CMCC_3GCDR_ManagementExtensions {
    unsigned int    count;
    CMCC_3GCDR_ManagementExtension *value;
} CMCC_3GCDR_ManagementExtensions;

typedef unsigned int    CMCC_3GCDR_LocalSequenceNumber;

typedef enum CMCC_3GCDR_APNSelectionMode {
    CMCC_3GCDR_mSorNetworkProvidedSubscriptionVerified = 0,
    CMCC_3GCDR_mSProvidedSubscriptionNotVerified = 1,
    CMCC_3GCDR_networkProvidedSubscriptionNotVerified = 2
} CMCC_3GCDR_APNSelectionMode;

typedef char            CMCC_3GCDR_AccessPointNameOI[38];

typedef struct CMCC_3GCDR_ISDN_AddressString {
    unsigned short  length;
    unsigned char   value[9];
} CMCC_3GCDR_ISDN_AddressString;

typedef CMCC_3GCDR_ISDN_AddressString CMCC_3GCDR_MSISDN;

typedef struct CMCC_3GCDR_ChargingCharacteristics {
    unsigned short  length;
    unsigned char   value[2];
} CMCC_3GCDR_ChargingCharacteristics;

typedef enum CMCC_3GCDR_SystemType {
    CMCC_3GCDR_unknown = 0,
    CMCC_3GCDR_iuUTRAN = 1,
    CMCC_3GCDR_gERAN = 2
} CMCC_3GCDR_SystemType;

typedef CMCC_3GCDR_AddressString CMCC_3GCDR_SCFAddress;

typedef unsigned int    CMCC_3GCDR_ServiceKey;

typedef enum CMCC_3GCDR_DefaultGPRS_Handling {
    CMCC_3GCDR_DefaultGPRS_Handling_continueTransaction = 0,
    CMCC_3GCDR_DefaultGPRS_Handling_releaseTransaction = 1
} CMCC_3GCDR_DefaultGPRS_Handling;

typedef CMCC_3GCDR_AccessPointNameNI CMCC_3GCDR_CAMELAccessPointNameNI;

typedef CMCC_3GCDR_AccessPointNameOI CMCC_3GCDR_CAMELAccessPointNameOI;

typedef int             CMCC_3GCDR_NumberOfDPEncountered;

typedef unsigned char   CMCC_3GCDR_LevelOfCAMELService;
#define                     CMCC_3GCDR_basic 0x80
#define                     CMCC_3GCDR_callDurationSupervision 0x40
#define                     CMCC_3GCDR_onlineCharging 0x20

typedef struct CMCC_3GCDR_FreeFormatData {
    unsigned short  length;
    unsigned char   value[160];
} CMCC_3GCDR_FreeFormatData;

typedef ossBoolean      CMCC_3GCDR_FFDAppendIndicator;

typedef struct CMCC_3GCDR_CAMELInformationPDP {
    unsigned short  bit_mask;
#       define      CMCC_3GCDR_CAMELInformationPDP_sCFAddress_present 0x8000
#       define      CMCC_3GCDR_CAMELInformationPDP_serviceKey_present 0x4000
#       define      CMCC_3GCDR_CAMELInformationPDP_defaultTransactionHandling_present 0x2000
#       define      CMCC_3GCDR_cAMELAccessPointNameNI_present 0x1000
#       define      CMCC_3GCDR_cAMELAccessPointNameOI_present 0x0800
#       define      CMCC_3GCDR_CAMELInformationPDP_numberOfDPEncountered_present 0x0400
#       define      CMCC_3GCDR_CAMELInformationPDP_levelOfCAMELService_present 0x0200
#       define      CMCC_3GCDR_CAMELInformationPDP_freeFormatData_present 0x0100
#       define      CMCC_3GCDR_CAMELInformationPDP_fFDAppendIndicator_present 0x0080
    CMCC_3GCDR_SCFAddress sCFAddress;  /* optional; set in bit_mask
                         * CMCC_3GCDR_CAMELInformationPDP_sCFAddress_present if
                         * present */
    CMCC_3GCDR_ServiceKey serviceKey;  /* optional; set in bit_mask
                         * CMCC_3GCDR_CAMELInformationPDP_serviceKey_present if
                         * present */
    CMCC_3GCDR_DefaultGPRS_Handling defaultTransactionHandling;  /* optional;
                                   * set in bit_mask
         * CMCC_3GCDR_CAMELInformationPDP_defaultTransactionHandling_present if
         * present */
    CMCC_3GCDR_CAMELAccessPointNameNI cAMELAccessPointNameNI;  /* optional; set
                                   * in bit_mask
                                   * CMCC_3GCDR_cAMELAccessPointNameNI_present
                                   * if present */
    CMCC_3GCDR_CAMELAccessPointNameOI cAMELAccessPointNameOI;  /* optional; set
                                   * in bit_mask
                                   * CMCC_3GCDR_cAMELAccessPointNameOI_present
                                   * if present */
    CMCC_3GCDR_NumberOfDPEncountered numberOfDPEncountered;  /* optional; set in
                                   * bit_mask
              * CMCC_3GCDR_CAMELInformationPDP_numberOfDPEncountered_present if
              * present */
    CMCC_3GCDR_LevelOfCAMELService levelOfCAMELService;  /* optional; set in
                                   * bit_mask
                * CMCC_3GCDR_CAMELInformationPDP_levelOfCAMELService_present if
                * present */
    CMCC_3GCDR_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                     * CMCC_3GCDR_CAMELInformationPDP_freeFormatData_present if
                     * present */
    CMCC_3GCDR_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in
                                   * bit_mask
                 * CMCC_3GCDR_CAMELInformationPDP_fFDAppendIndicator_present if
                 * present */
} CMCC_3GCDR_CAMELInformationPDP;

typedef int             CMCC_3GCDR_DataVolumeGPRS;

typedef struct CMCC_3GCDR__seqof1 {
    unsigned int    count;
    struct CMCC_3GCDR_ChangeOfCharCondition *value;
} CMCC_3GCDR__seqof1;

typedef struct CMCC_3GCDR_SGSNPDPRecord {
    unsigned int    bit_mask;
#       define      CMCC_3GCDR_SGSNPDPRecord_networkInitiation_present 0x80000000
#       define      CMCC_3GCDR_SGSNPDPRecord_servedIMEI_present 0x40000000
#       define      CMCC_3GCDR_SGSNPDPRecord_msNetworkCapability_present 0x20000000
#       define      CMCC_3GCDR_SGSNPDPRecord_routingArea_present 0x10000000
#       define      CMCC_3GCDR_SGSNPDPRecord_locationAreaCode_present 0x08000000
#       define      CMCC_3GCDR_SGSNPDPRecord_cellIdentifier_present 0x04000000
#       define      CMCC_3GCDR_SGSNPDPRecord_servedPDPAddress_present 0x02000000
#       define      CMCC_3GCDR_SGSNPDPRecord_sgsnChange_present 0x01000000
#       define      CMCC_3GCDR_SGSNPDPRecord_diagnostics_present 0x00800000
#       define      CMCC_3GCDR_SGSNPDPRecord_recordSequenceNumber_present 0x00400000
#       define      CMCC_3GCDR_SGSNPDPRecord_nodeID_present 0x00200000
#       define      CMCC_3GCDR_SGSNPDPRecord_recordExtensions_present 0x00100000
#       define      CMCC_3GCDR_SGSNPDPRecord_localSequenceNumber_present 0x00080000
#       define      CMCC_3GCDR_SGSNPDPRecord_apnSelectionMode_present 0x00040000
#       define      CMCC_3GCDR_SGSNPDPRecord_servedMSISDN_present 0x00020000
#       define      CMCC_3GCDR_SGSNPDPRecord_chargingCharacteristics_present 0x00010000
#       define      CMCC_3GCDR_SGSNPDPRecord_systemType_present 0x00008000
#       define      CMCC_3GCDR_cAMELInformationPDP_present 0x00004000
#       define      CMCC_3GCDR_rNCUnsentDownlinkVolume_present 0x00002000
    CMCC_3GCDR_CallEventRecordType recordType;
    CMCC_3GCDR_NetworkInitiatedPDPContext networkInitiation;  /* optional; set
                                   * in bit_mask
                        * CMCC_3GCDR_SGSNPDPRecord_networkInitiation_present if
                        * present */
    CMCC_3GCDR_IMSI servedIMSI;
    CMCC_3GCDR_IMEI servedIMEI;  /* optional; set in bit_mask
                                  * CMCC_3GCDR_SGSNPDPRecord_servedIMEI_present
                                  * if present */
    CMCC_3GCDR_GSNAddress sgsnAddress;
    CMCC_3GCDR_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                      * CMCC_3GCDR_SGSNPDPRecord_msNetworkCapability_present if
                      * present */
    CMCC_3GCDR_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                              * CMCC_3GCDR_SGSNPDPRecord_routingArea_present if
                              * present */
    CMCC_3GCDR_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                         * CMCC_3GCDR_SGSNPDPRecord_locationAreaCode_present if
                         * present */
    CMCC_3GCDR_CellId cellIdentifier;  /* optional; set in bit_mask
                           * CMCC_3GCDR_SGSNPDPRecord_cellIdentifier_present if
                           * present */
    CMCC_3GCDR_ChargingID chargingID;
    CMCC_3GCDR_GSNAddress ggsnAddressUsed;
    CMCC_3GCDR_AccessPointNameNI accessPointNameNI;
    CMCC_3GCDR_PDPType pdpType;
    CMCC_3GCDR_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                         * CMCC_3GCDR_SGSNPDPRecord_servedPDPAddress_present if
                         * present */
    CMCC_3GCDR__seqof1 listOfTrafficVolumes;
    CMCC_3GCDR_TimeStamp recordOpeningTime;
    CMCC_3GCDR_CallDuration duration;
    CMCC_3GCDR_SGSNChange sgsnChange;  /* optional; set in bit_mask
                               * CMCC_3GCDR_SGSNPDPRecord_sgsnChange_present if
                               * present */
    CMCC_3GCDR_CauseForRecClosing causeForRecClosing;
    CMCC_3GCDR_Diagnostics diagnostics;  /* optional; set in bit_mask
                              * CMCC_3GCDR_SGSNPDPRecord_diagnostics_present if
                              * present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                     * CMCC_3GCDR_SGSNPDPRecord_recordSequenceNumber_present if
                     * present */
    CMCC_3GCDR_NodeID nodeID;  /* optional; set in bit_mask
                                * CMCC_3GCDR_SGSNPDPRecord_nodeID_present if
                                * present */
    CMCC_3GCDR_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                         * CMCC_3GCDR_SGSNPDPRecord_recordExtensions_present if
                         * present */
    CMCC_3GCDR_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                      * CMCC_3GCDR_SGSNPDPRecord_localSequenceNumber_present if
                      * present */
    CMCC_3GCDR_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                         * CMCC_3GCDR_SGSNPDPRecord_apnSelectionMode_present if
                         * present */
    CMCC_3GCDR_AccessPointNameOI accessPointNameOI;
    CMCC_3GCDR_MSISDN servedMSISDN;  /* optional; set in bit_mask
                             * CMCC_3GCDR_SGSNPDPRecord_servedMSISDN_present if
                             * present */
    CMCC_3GCDR_ChargingCharacteristics chargingCharacteristics;  /* optional;
                                   * set in bit_mask
                  * CMCC_3GCDR_SGSNPDPRecord_chargingCharacteristics_present if
                  * present */
    CMCC_3GCDR_SystemType systemType;  /* optional; set in bit_mask
                               * CMCC_3GCDR_SGSNPDPRecord_systemType_present if
                               * present */
    CMCC_3GCDR_CAMELInformationPDP cAMELInformationPDP;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_cAMELInformationPDP_present if
                                   * present */
    CMCC_3GCDR_DataVolumeGPRS rNCUnsentDownlinkVolume;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_rNCUnsentDownlinkVolume_present
                                   * if present */
} CMCC_3GCDR_SGSNPDPRecord;

typedef ossBoolean      CMCC_3GCDR_DynamicAddressFlag;

typedef struct CMCC_3GCDR_PLMN_Id {
    unsigned short  length;
    unsigned char   value[3];
} CMCC_3GCDR_PLMN_Id;

typedef struct CMCC_3GCDR_GGSNPDPRecord {
    unsigned short  bit_mask;
#       define      CMCC_3GCDR_GGSNPDPRecord_networkInitiation_present 0x8000
#       define      CMCC_3GCDR_GGSNPDPRecord_servedPDPAddress_present 0x4000
#       define      CMCC_3GCDR_dynamicAddressFlag_present 0x2000
#       define      CMCC_3GCDR_GGSNPDPRecord_diagnostics_present 0x1000
#       define      CMCC_3GCDR_GGSNPDPRecord_recordSequenceNumber_present 0x0800
#       define      CMCC_3GCDR_GGSNPDPRecord_nodeID_present 0x0400
#       define      CMCC_3GCDR_GGSNPDPRecord_recordExtensions_present 0x0200
#       define      CMCC_3GCDR_GGSNPDPRecord_localSequenceNumber_present 0x0100
#       define      CMCC_3GCDR_GGSNPDPRecord_apnSelectionMode_present 0x0080
#       define      CMCC_3GCDR_GGSNPDPRecord_servedMSISDN_present 0x0040
#       define      CMCC_3GCDR_GGSNPDPRecord_chargingCharacteristics_present 0x0020
#       define      CMCC_3GCDR_sgsnPLMNIdentifier_present 0x0010
    CMCC_3GCDR_CallEventRecordType recordType;
    CMCC_3GCDR_NetworkInitiatedPDPContext networkInitiation;  /* optional; set
                                   * in bit_mask
                        * CMCC_3GCDR_GGSNPDPRecord_networkInitiation_present if
                        * present */
    CMCC_3GCDR_IMSI servedIMSI;
    CMCC_3GCDR_GSNAddress ggsnAddress;
    CMCC_3GCDR_ChargingID chargingID;
    struct CMCC_3GCDR__seqof2 {
        unsigned int    count;
        CMCC_3GCDR_GSNAddress *value;
    } sgsnAddress;
    CMCC_3GCDR_AccessPointNameNI accessPointNameNI;
    CMCC_3GCDR_PDPType pdpType;
    CMCC_3GCDR_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                         * CMCC_3GCDR_GGSNPDPRecord_servedPDPAddress_present if
                         * present */
    CMCC_3GCDR_DynamicAddressFlag dynamicAddressFlag;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_dynamicAddressFlag_present if
                                   * present */
    CMCC_3GCDR__seqof1 listOfTrafficVolumes;
    CMCC_3GCDR_TimeStamp recordOpeningTime;
    CMCC_3GCDR_CallDuration duration;
    CMCC_3GCDR_CauseForRecClosing causeForRecClosing;
    CMCC_3GCDR_Diagnostics diagnostics;  /* optional; set in bit_mask
                              * CMCC_3GCDR_GGSNPDPRecord_diagnostics_present if
                              * present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                     * CMCC_3GCDR_GGSNPDPRecord_recordSequenceNumber_present if
                     * present */
    CMCC_3GCDR_NodeID nodeID;  /* optional; set in bit_mask
                                * CMCC_3GCDR_GGSNPDPRecord_nodeID_present if
                                * present */
    CMCC_3GCDR_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                         * CMCC_3GCDR_GGSNPDPRecord_recordExtensions_present if
                         * present */
    CMCC_3GCDR_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                      * CMCC_3GCDR_GGSNPDPRecord_localSequenceNumber_present if
                      * present */
    CMCC_3GCDR_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                         * CMCC_3GCDR_GGSNPDPRecord_apnSelectionMode_present if
                         * present */
    CMCC_3GCDR_MSISDN servedMSISDN;  /* optional; set in bit_mask
                             * CMCC_3GCDR_GGSNPDPRecord_servedMSISDN_present if
                             * present */
    CMCC_3GCDR_ChargingCharacteristics chargingCharacteristics;  /* optional;
                                   * set in bit_mask
                  * CMCC_3GCDR_GGSNPDPRecord_chargingCharacteristics_present if
                  * present */
    CMCC_3GCDR_PLMN_Id sgsnPLMNIdentifier;  /* optional; set in bit_mask
                                     * CMCC_3GCDR_sgsnPLMNIdentifier_present if
                                     * present */
} CMCC_3GCDR_GGSNPDPRecord;

typedef struct CMCC_3GCDR_CAMELInformationMM {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_CAMELInformationMM_sCFAddress_present 0x80
#       define      CMCC_3GCDR_CAMELInformationMM_serviceKey_present 0x40
#       define      CMCC_3GCDR_CAMELInformationMM_defaultTransactionHandling_present 0x20
#       define      CMCC_3GCDR_CAMELInformationMM_numberOfDPEncountered_present 0x10
#       define      CMCC_3GCDR_CAMELInformationMM_levelOfCAMELService_present 0x08
#       define      CMCC_3GCDR_CAMELInformationMM_freeFormatData_present 0x04
#       define      CMCC_3GCDR_CAMELInformationMM_fFDAppendIndicator_present 0x02
    CMCC_3GCDR_SCFAddress sCFAddress;  /* optional; set in bit_mask
                          * CMCC_3GCDR_CAMELInformationMM_sCFAddress_present if
                          * present */
    CMCC_3GCDR_ServiceKey serviceKey;  /* optional; set in bit_mask
                          * CMCC_3GCDR_CAMELInformationMM_serviceKey_present if
                          * present */
    CMCC_3GCDR_DefaultGPRS_Handling defaultTransactionHandling;  /* optional;
                                   * set in bit_mask
          * CMCC_3GCDR_CAMELInformationMM_defaultTransactionHandling_present if
          * present */
    CMCC_3GCDR_NumberOfDPEncountered numberOfDPEncountered;  /* optional; set in
                                   * bit_mask
               * CMCC_3GCDR_CAMELInformationMM_numberOfDPEncountered_present if
               * present */
    CMCC_3GCDR_LevelOfCAMELService levelOfCAMELService;  /* optional; set in
                                   * bit_mask
                 * CMCC_3GCDR_CAMELInformationMM_levelOfCAMELService_present if
                 * present */
    CMCC_3GCDR_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                      * CMCC_3GCDR_CAMELInformationMM_freeFormatData_present if
                      * present */
    CMCC_3GCDR_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in
                                   * bit_mask
                  * CMCC_3GCDR_CAMELInformationMM_fFDAppendIndicator_present if
                  * present */
} CMCC_3GCDR_CAMELInformationMM;

typedef struct CMCC_3GCDR_SGSNMMRecord {
    unsigned int    bit_mask;
#       define      CMCC_3GCDR_SGSNMMRecord_servedIMEI_present 0x80000000
#       define      CMCC_3GCDR_SGSNMMRecord_msNetworkCapability_present 0x40000000
#       define      CMCC_3GCDR_SGSNMMRecord_routingArea_present 0x20000000
#       define      CMCC_3GCDR_SGSNMMRecord_locationAreaCode_present 0x10000000
#       define      CMCC_3GCDR_SGSNMMRecord_cellIdentifier_present 0x08000000
#       define      CMCC_3GCDR_changeLocation_present 0x04000000
#       define      CMCC_3GCDR_duration_present 0x02000000
#       define      CMCC_3GCDR_SGSNMMRecord_sgsnChange_present 0x01000000
#       define      CMCC_3GCDR_SGSNMMRecord_diagnostics_present 0x00800000
#       define      CMCC_3GCDR_SGSNMMRecord_recordSequenceNumber_present 0x00400000
#       define      CMCC_3GCDR_SGSNMMRecord_nodeID_present 0x00200000
#       define      CMCC_3GCDR_SGSNMMRecord_recordExtensions_present 0x00100000
#       define      CMCC_3GCDR_SGSNMMRecord_localSequenceNumber_present 0x00080000
#       define      CMCC_3GCDR_SGSNMMRecord_servedMSISDN_present 0x00040000
#       define      CMCC_3GCDR_SGSNMMRecord_chargingCharacteristics_present 0x00020000
#       define      CMCC_3GCDR_cAMELInformationMM_present 0x00010000
#       define      CMCC_3GCDR_SGSNMMRecord_systemType_present 0x00008000
    CMCC_3GCDR_CallEventRecordType recordType;
    CMCC_3GCDR_IMSI servedIMSI;
    CMCC_3GCDR_IMEI servedIMEI;  /* optional; set in bit_mask
                                  * CMCC_3GCDR_SGSNMMRecord_servedIMEI_present
                                  * if present */
    CMCC_3GCDR_GSNAddress sgsnAddress;
    CMCC_3GCDR_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                       * CMCC_3GCDR_SGSNMMRecord_msNetworkCapability_present if
                       * present */
    CMCC_3GCDR_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                               * CMCC_3GCDR_SGSNMMRecord_routingArea_present if
                               * present */
    CMCC_3GCDR_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                          * CMCC_3GCDR_SGSNMMRecord_locationAreaCode_present if
                          * present */
    CMCC_3GCDR_CellId cellIdentifier;  /* optional; set in bit_mask
                            * CMCC_3GCDR_SGSNMMRecord_cellIdentifier_present if
                            * present */
    struct CMCC_3GCDR__seqof3 {
        unsigned int    count;
        struct CMCC_3GCDR_ChangeLocation *value;
    } changeLocation;  /* optional; set in bit_mask
                        * CMCC_3GCDR_changeLocation_present if present */
    CMCC_3GCDR_TimeStamp recordOpeningTime;
    CMCC_3GCDR_CallDuration duration;  /* optional; set in bit_mask
                                        * CMCC_3GCDR_duration_present if
                                        * present */
    CMCC_3GCDR_SGSNChange sgsnChange;  /* optional; set in bit_mask
                                * CMCC_3GCDR_SGSNMMRecord_sgsnChange_present if
                                * present */
    CMCC_3GCDR_CauseForRecClosing causeForRecClosing;
    CMCC_3GCDR_Diagnostics diagnostics;  /* optional; set in bit_mask
                               * CMCC_3GCDR_SGSNMMRecord_diagnostics_present if
                               * present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                      * CMCC_3GCDR_SGSNMMRecord_recordSequenceNumber_present if
                      * present */
    CMCC_3GCDR_NodeID nodeID;  /* optional; set in bit_mask
                                * CMCC_3GCDR_SGSNMMRecord_nodeID_present if
                                * present */
    CMCC_3GCDR_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                          * CMCC_3GCDR_SGSNMMRecord_recordExtensions_present if
                          * present */
    CMCC_3GCDR_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                       * CMCC_3GCDR_SGSNMMRecord_localSequenceNumber_present if
                       * present */
    CMCC_3GCDR_MSISDN servedMSISDN;  /* optional; set in bit_mask
                              * CMCC_3GCDR_SGSNMMRecord_servedMSISDN_present if
                              * present */
    CMCC_3GCDR_ChargingCharacteristics chargingCharacteristics;  /* optional;
                                   * set in bit_mask
                   * CMCC_3GCDR_SGSNMMRecord_chargingCharacteristics_present if
                   * present */
    CMCC_3GCDR_CAMELInformationMM cAMELInformationMM;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_cAMELInformationMM_present if
                                   * present */
    CMCC_3GCDR_SystemType systemType;  /* optional; set in bit_mask
                                * CMCC_3GCDR_SGSNMMRecord_systemType_present if
                                * present */
} CMCC_3GCDR_SGSNMMRecord;

typedef CMCC_3GCDR_AddressString CMCC_3GCDR_RecordingEntity;

typedef struct CMCC_3GCDR_MessageReference {
    unsigned int    length;
    unsigned char   *value;
} CMCC_3GCDR_MessageReference;

typedef CMCC_3GCDR_Diagnostics CMCC_3GCDR_SMSResult;

typedef struct CMCC_3GCDR_CalledNumber {
    unsigned short  length;
    unsigned char   value[10];
} CMCC_3GCDR_CalledNumber;

typedef enum CMCC_3GCDR_DefaultSMS_Handling {
    CMCC_3GCDR_DefaultSMS_Handling_continueTransaction = 0,
    CMCC_3GCDR_DefaultSMS_Handling_releaseTransaction = 1
} CMCC_3GCDR_DefaultSMS_Handling;

typedef struct CMCC_3GCDR_CallingNumber {
    unsigned short  length;
    unsigned char   value[10];
} CMCC_3GCDR_CallingNumber;

typedef struct CMCC_3GCDR_CallReferenceNumber {
    unsigned short  length;
    unsigned char   value[8];
} CMCC_3GCDR_CallReferenceNumber;

typedef struct CMCC_3GCDR_CAMELInformationSMS {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_CAMELInformationSMS_sCFAddress_present 0x80
#       define      CMCC_3GCDR_CAMELInformationSMS_serviceKey_present 0x40
#       define      CMCC_3GCDR_defaultSMSHandling_present 0x20
#       define      CMCC_3GCDR_cAMELCallingPartyNumber_present 0x10
#       define      CMCC_3GCDR_cAMELDestinationSubscriberNumber_present 0x08
#       define      CMCC_3GCDR_cAMELSMSCAddress_present 0x04
#       define      CMCC_3GCDR_CAMELInformationSMS_freeFormatData_present 0x02
#       define      CMCC_3GCDR_sMSReferenceNumber_present 0x01
    CMCC_3GCDR_SCFAddress sCFAddress;  /* optional; set in bit_mask
                         * CMCC_3GCDR_CAMELInformationSMS_sCFAddress_present if
                         * present */
    CMCC_3GCDR_ServiceKey serviceKey;  /* optional; set in bit_mask
                         * CMCC_3GCDR_CAMELInformationSMS_serviceKey_present if
                         * present */
    CMCC_3GCDR_DefaultSMS_Handling defaultSMSHandling;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_defaultSMSHandling_present if
                                   * present */
    CMCC_3GCDR_CallingNumber cAMELCallingPartyNumber;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_cAMELCallingPartyNumber_present
                                   * if present */
    CMCC_3GCDR_CalledNumber cAMELDestinationSubscriberNumber;  /* optional; set
                                   * in bit_mask
                       * CMCC_3GCDR_cAMELDestinationSubscriberNumber_present if
                       * present */
    CMCC_3GCDR_AddressString cAMELSMSCAddress;  /* optional; set in bit_mask
                                       * CMCC_3GCDR_cAMELSMSCAddress_present if
                                       * present */
    CMCC_3GCDR_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                     * CMCC_3GCDR_CAMELInformationSMS_freeFormatData_present if
                     * present */
    CMCC_3GCDR_CallReferenceNumber sMSReferenceNumber;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_sMSReferenceNumber_present if
                                   * present */
} CMCC_3GCDR_CAMELInformationSMS;

typedef struct CMCC_3GCDR_SGSNSMORecord {
    unsigned short  bit_mask;
#       define      CMCC_3GCDR_SGSNSMORecord_servedIMEI_present 0x8000
#       define      CMCC_3GCDR_SGSNSMORecord_servedMSISDN_present 0x4000
#       define      CMCC_3GCDR_SGSNSMORecord_locationArea_present 0x2000
#       define      CMCC_3GCDR_SGSNSMORecord_routingArea_present 0x1000
#       define      CMCC_3GCDR_SGSNSMORecord_cellIdentifier_present 0x0800
#       define      CMCC_3GCDR_SGSNSMORecord_smsResult_present 0x0400
#       define      CMCC_3GCDR_SGSNSMORecord_recordExtensions_present 0x0200
#       define      CMCC_3GCDR_SGSNSMORecord_nodeID_present 0x0100
#       define      CMCC_3GCDR_SGSNSMORecord_localSequenceNumber_present 0x0080
#       define      CMCC_3GCDR_SGSNSMORecord_chargingCharacteristics_present 0x0040
#       define      CMCC_3GCDR_SGSNSMORecord_systemType_present 0x0020
#       define      CMCC_3GCDR_destinationNumber_present 0x0010
#       define      CMCC_3GCDR_cAMELInformationSMS_present 0x0008
    CMCC_3GCDR_CallEventRecordType recordType;
    CMCC_3GCDR_IMSI servedIMSI;
    CMCC_3GCDR_IMEI servedIMEI;  /* optional; set in bit_mask
                                  * CMCC_3GCDR_SGSNSMORecord_servedIMEI_present
                                  * if present */
    CMCC_3GCDR_MSISDN servedMSISDN;  /* optional; set in bit_mask
                             * CMCC_3GCDR_SGSNSMORecord_servedMSISDN_present if
                             * present */
    CMCC_3GCDR_MSNetworkCapability msNetworkCapability;
    CMCC_3GCDR_AddressString serviceCentre;
    CMCC_3GCDR_RecordingEntity recordingEntity;
    CMCC_3GCDR_LocationAreaCode locationArea;  /* optional; set in bit_mask
                             * CMCC_3GCDR_SGSNSMORecord_locationArea_present if
                             * present */
    CMCC_3GCDR_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                              * CMCC_3GCDR_SGSNSMORecord_routingArea_present if
                              * present */
    CMCC_3GCDR_CellId cellIdentifier;  /* optional; set in bit_mask
                           * CMCC_3GCDR_SGSNSMORecord_cellIdentifier_present if
                           * present */
    CMCC_3GCDR_MessageReference messageReference;
    CMCC_3GCDR_TimeStamp originationTime;
    CMCC_3GCDR_SMSResult smsResult;  /* optional; set in bit_mask
                                * CMCC_3GCDR_SGSNSMORecord_smsResult_present if
                                * present */
    CMCC_3GCDR_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                         * CMCC_3GCDR_SGSNSMORecord_recordExtensions_present if
                         * present */
    CMCC_3GCDR_NodeID nodeID;  /* optional; set in bit_mask
                                * CMCC_3GCDR_SGSNSMORecord_nodeID_present if
                                * present */
    CMCC_3GCDR_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                      * CMCC_3GCDR_SGSNSMORecord_localSequenceNumber_present if
                      * present */
    CMCC_3GCDR_ChargingCharacteristics chargingCharacteristics;  /* optional;
                                   * set in bit_mask
                  * CMCC_3GCDR_SGSNSMORecord_chargingCharacteristics_present if
                  * present */
    CMCC_3GCDR_SystemType systemType;  /* optional; set in bit_mask
                               * CMCC_3GCDR_SGSNSMORecord_systemType_present if
                               * present */
    CMCC_3GCDR_CalledNumber destinationNumber;  /* optional; set in bit_mask
                                      * CMCC_3GCDR_destinationNumber_present if
                                      * present */
    CMCC_3GCDR_CAMELInformationSMS cAMELInformationSMS;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_cAMELInformationSMS_present if
                                   * present */
} CMCC_3GCDR_SGSNSMORecord;

typedef struct CMCC_3GCDR_SGSNSMTRecord {
    unsigned short  bit_mask;
#       define      CMCC_3GCDR_SGSNSMTRecord_servedIMEI_present 0x8000
#       define      CMCC_3GCDR_SGSNSMTRecord_servedMSISDN_present 0x4000
#       define      CMCC_3GCDR_SGSNSMTRecord_locationArea_present 0x2000
#       define      CMCC_3GCDR_SGSNSMTRecord_routingArea_present 0x1000
#       define      CMCC_3GCDR_SGSNSMTRecord_cellIdentifier_present 0x0800
#       define      CMCC_3GCDR_SGSNSMTRecord_smsResult_present 0x0400
#       define      CMCC_3GCDR_SGSNSMTRecord_recordExtensions_present 0x0200
#       define      CMCC_3GCDR_SGSNSMTRecord_nodeID_present 0x0100
#       define      CMCC_3GCDR_SGSNSMTRecord_localSequenceNumber_present 0x0080
#       define      CMCC_3GCDR_SGSNSMTRecord_chargingCharacteristics_present 0x0040
#       define      CMCC_3GCDR_SGSNSMTRecord_systemType_present 0x0020
    CMCC_3GCDR_CallEventRecordType recordType;
    CMCC_3GCDR_IMSI servedIMSI;
    CMCC_3GCDR_IMEI servedIMEI;  /* optional; set in bit_mask
                                  * CMCC_3GCDR_SGSNSMTRecord_servedIMEI_present
                                  * if present */
    CMCC_3GCDR_MSISDN servedMSISDN;  /* optional; set in bit_mask
                             * CMCC_3GCDR_SGSNSMTRecord_servedMSISDN_present if
                             * present */
    CMCC_3GCDR_MSNetworkCapability msNetworkCapability;
    CMCC_3GCDR_AddressString serviceCentre;
    CMCC_3GCDR_RecordingEntity recordingEntity;
    CMCC_3GCDR_LocationAreaCode locationArea;  /* optional; set in bit_mask
                             * CMCC_3GCDR_SGSNSMTRecord_locationArea_present if
                             * present */
    CMCC_3GCDR_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                              * CMCC_3GCDR_SGSNSMTRecord_routingArea_present if
                              * present */
    CMCC_3GCDR_CellId cellIdentifier;  /* optional; set in bit_mask
                           * CMCC_3GCDR_SGSNSMTRecord_cellIdentifier_present if
                           * present */
    CMCC_3GCDR_TimeStamp originationTime;
    CMCC_3GCDR_SMSResult smsResult;  /* optional; set in bit_mask
                                * CMCC_3GCDR_SGSNSMTRecord_smsResult_present if
                                * present */
    CMCC_3GCDR_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                         * CMCC_3GCDR_SGSNSMTRecord_recordExtensions_present if
                         * present */
    CMCC_3GCDR_NodeID nodeID;  /* optional; set in bit_mask
                                * CMCC_3GCDR_SGSNSMTRecord_nodeID_present if
                                * present */
    CMCC_3GCDR_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                      * CMCC_3GCDR_SGSNSMTRecord_localSequenceNumber_present if
                      * present */
    CMCC_3GCDR_ChargingCharacteristics chargingCharacteristics;  /* optional;
                                   * set in bit_mask
                  * CMCC_3GCDR_SGSNSMTRecord_chargingCharacteristics_present if
                  * present */
    CMCC_3GCDR_SystemType systemType;  /* optional; set in bit_mask
                               * CMCC_3GCDR_SGSNSMTRecord_systemType_present if
                               * present */
} CMCC_3GCDR_SGSNSMTRecord;

typedef struct CMCC_3GCDR_CallEventRecord {
    unsigned short  choice;
#       define      CMCC_3GCDR_sgsnPDPRecord_chosen 1
#       define      CMCC_3GCDR_ggsnPDPRecord_chosen 2
#       define      CMCC_3GCDR_sgsnMMRecord_chosen 3
#       define      CMCC_3GCDR_sgsnSMORecord_chosen 4
#       define      CMCC_3GCDR_sgsnSMTRecord_chosen 5
    union _union {
        CMCC_3GCDR_SGSNPDPRecord sgsnPDPRecord;  /* to choose, set choice to
                                           * CMCC_3GCDR_sgsnPDPRecord_chosen */
        CMCC_3GCDR_GGSNPDPRecord ggsnPDPRecord;  /* to choose, set choice to
                                           * CMCC_3GCDR_ggsnPDPRecord_chosen */
        CMCC_3GCDR_SGSNMMRecord sgsnMMRecord;  /* to choose, set choice to
                                            * CMCC_3GCDR_sgsnMMRecord_chosen */
        CMCC_3GCDR_SGSNSMORecord sgsnSMORecord;  /* to choose, set choice to
                                           * CMCC_3GCDR_sgsnSMORecord_chosen */
        CMCC_3GCDR_SGSNSMTRecord sgsnSMTRecord;  /* to choose, set choice to
                                           * CMCC_3GCDR_sgsnSMTRecord_chosen */
    } u;
} CMCC_3GCDR_CallEventRecord;

typedef enum CMCC_3GCDR_ChangeCondition {
    CMCC_3GCDR_qoSChange = 0,
    CMCC_3GCDR_tariffTime = 1,
    CMCC_3GCDR_recordClosure = 2
} CMCC_3GCDR_ChangeCondition;

typedef enum CMCC_3GCDR_QoSReliability {
    CMCC_3GCDR_unspecifiedReliability = 0,
    CMCC_3GCDR_acknowledgedGTP = 1,
    CMCC_3GCDR_unackGTPAcknowLLC = 2,
    CMCC_3GCDR_unackGTPLLCAcknowRLC = 3,
    CMCC_3GCDR_unackGTPLLCRLC = 4,
    CMCC_3GCDR_unacknowUnprotectedData = 5
} CMCC_3GCDR_QoSReliability;

typedef enum CMCC_3GCDR_QoSDelay {
    CMCC_3GCDR_delayClass1 = 1,
    CMCC_3GCDR_delayClass2 = 2,
    CMCC_3GCDR_delayClass3 = 3,
    CMCC_3GCDR_delayClass4 = 4
} CMCC_3GCDR_QoSDelay;

typedef enum CMCC_3GCDR_QoSPrecedence {
    CMCC_3GCDR_QoSPrecedence_unspecified = 0,
    CMCC_3GCDR_highPriority = 1,
    CMCC_3GCDR_normalPriority = 2,
    CMCC_3GCDR_lowPriority = 3
} CMCC_3GCDR_QoSPrecedence;

typedef enum CMCC_3GCDR_QoSPeakThroughput {
    CMCC_3GCDR_QoSPeakThroughput_unspecified = 0,
    CMCC_3GCDR_upTo1000octetPs = 1,
    CMCC_3GCDR_upTo2000octetPs = 2,
    CMCC_3GCDR_upTo4000octetPs = 3,
    CMCC_3GCDR_upTo8000octetPs = 4,
    CMCC_3GCDR_upTo16000octetPs = 5,
    CMCC_3GCDR_upTo32000octetPs = 6,
    CMCC_3GCDR_upTo64000octetPs = 7,
    CMCC_3GCDR_upTo128000octetPs = 8,
    CMCC_3GCDR_upTo256000octetPs = 9
} CMCC_3GCDR_QoSPeakThroughput;

typedef enum CMCC_3GCDR_QoSMeanThroughput {
    CMCC_3GCDR_subscribedMeanThroughput = 0,
    CMCC_3GCDR_mean100octetPh = 1,
    CMCC_3GCDR_mean200octetPh = 2,
    CMCC_3GCDR_mean500octetPh = 3,
    CMCC_3GCDR_mean1000octetPh = 4,
    CMCC_3GCDR_mean2000octetPh = 5,
    CMCC_3GCDR_mean5000octetPh = 6,
    CMCC_3GCDR_mean10000octetPh = 7,
    CMCC_3GCDR_mean20000octetPh = 8,
    CMCC_3GCDR_mean50000octetPh = 9,
    CMCC_3GCDR_mean100000octetPh = 10,
    CMCC_3GCDR_mean200000octetPh = 11,
    CMCC_3GCDR_mean500000octetPh = 12,
    CMCC_3GCDR_mean1000000octetPh = 13,
    CMCC_3GCDR_mean2000000octetPh = 14,
    CMCC_3GCDR_mean5000000octetPh = 15,
    CMCC_3GCDR_mean10000000octetPh = 16,
    CMCC_3GCDR_mean20000000octetPh = 17,
    CMCC_3GCDR_mean50000000octetPh = 18,
    CMCC_3GCDR_reserved = 30,
    CMCC_3GCDR_bestEffort = 31
} CMCC_3GCDR_QoSMeanThroughput;

typedef struct CMCC_3GCDR_GSMQoSInformation {
    CMCC_3GCDR_QoSReliability reliability;
    CMCC_3GCDR_QoSDelay delay;
    CMCC_3GCDR_QoSPrecedence precedence;
    CMCC_3GCDR_QoSPeakThroughput peakThroughput;
    CMCC_3GCDR_QoSMeanThroughput meanThroughput;
} CMCC_3GCDR_GSMQoSInformation;

typedef struct CMCC_3GCDR_QoSInformation {
    unsigned short  choice;
#       define      CMCC_3GCDR_gsmQosInformation_chosen 1
#       define      CMCC_3GCDR_umtsQosInformation_chosen 2
    union _union {
        CMCC_3GCDR_GSMQoSInformation gsmQosInformation;  /* to choose, set
                                   * choice to
                                   * CMCC_3GCDR_gsmQosInformation_chosen */
        struct CMCC_3GCDR__octet4 {
            unsigned short  length;
            unsigned char   value[12];
        } umtsQosInformation;  /* to choose, set choice to
                                * CMCC_3GCDR_umtsQosInformation_chosen */
    } u;
} CMCC_3GCDR_QoSInformation;

typedef struct CMCC_3GCDR_ChangeOfCharCondition {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_qosRequested_present 0x80
#       define      CMCC_3GCDR_qosNegotiated_present 0x40
    CMCC_3GCDR_QoSInformation qosRequested;  /* optional; set in bit_mask
                                              * CMCC_3GCDR_qosRequested_present
                                              * if present */
    CMCC_3GCDR_QoSInformation qosNegotiated;  /* optional; set in bit_mask
                                          * CMCC_3GCDR_qosNegotiated_present if
                                          * present */
    CMCC_3GCDR_DataVolumeGPRS dataVolumeGPRSUplink;
    CMCC_3GCDR_DataVolumeGPRS dataVolumeGPRSDownlink;
    CMCC_3GCDR_ChangeCondition changeCondition;
    CMCC_3GCDR_TimeStamp changeTime;
} CMCC_3GCDR_ChangeOfCharCondition;

typedef struct CMCC_3GCDR_ChangeLocation {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_cellId_present 0x80
    CMCC_3GCDR_LocationAreaCode locationAreaCode;
    CMCC_3GCDR_RoutingAreaCode routingAreaCode;
    CMCC_3GCDR_CellId cellId;  /* optional; set in bit_mask
                                * CMCC_3GCDR_cellId_present if present */
    CMCC_3GCDR_TimeStamp changeTime;
} CMCC_3GCDR_ChangeLocation;

extern CMCC_3GCDR__ObjectID CMCC_3GCDR_gsm1205InformationModel;

extern CMCC_3GCDR__ObjectID CMCC_3GCDR_gsm1205ASN1Module;

extern int CMCC_3GCDR_maxAddressLength;

extern int CMCC_3GCDR_maxISDN_AddressLength;


extern void *CDRF_R99_Org;    /* encoder-decoder control table */
#ifdef __cplusplus
}	/* extern "C" */
#endif /* __cplusplus */
#endif /* OSS_CDRF_R99_Org */
