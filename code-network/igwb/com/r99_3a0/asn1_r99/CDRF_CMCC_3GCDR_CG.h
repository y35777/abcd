/********************************************************/
/* Copyright (C) 2003 OSS Nokalva.  All rights reserved.*/
/********************************************************/

/* THIS FILE IS PROPRIETARY MATERIAL OF OSS NOKALVA
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF OSS NOKALVA.
 * THIS FILE MAY NOT BE DISTRIBUTED. */

/* Generated for: HuaWei Technologies, Inc., Shenzhen, China - license 8088 for Windows NT */
/* Abstract syntax: CDRF_R99_Fin */
/* Created: Fri Dec 19 17:31:10 2003 */
/* ASN.1 compiler version: 5.3 */
/* Target operating system: Windows NT/Windows 9x */
/* Target machine type: Intel x86 */
/* C compiler options required: -Zp4 (Microsoft) */
/* ASN.1 compiler options and file names specified:
 * -codefile CDRF_CMCC_3GCDR_CG.C -headerfile CDRF_CMCC_3GCDR_CG.h -errorfile
 * C:\WINDOWS\TEMP\2. -prefix CMCC_3GCDR_CG_ -nodebug -dualheader -ansi
 * -verbose -ber -root
 * D:\Application\ossasn1\win32\5.3.0\asn1dflt\asn1dflt.ms.zp4
 * D:\yutemp\lidufang\1218\R99\CDRF_CMCC_3GCDR_CG\CDRF_R99_Fin.asn
 */

#ifndef OSS_CDRF_R99_Fin
#define OSS_CDRF_R99_Fin

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "asn1hdr.h"
#include "asn1code.h"

#define          CMCC_3GCDR_CG_CallEventRecord_PDU 1

typedef struct CMCC_3GCDR_CG__ObjectID {
    unsigned short  length;
    unsigned char   *value;
} CMCC_3GCDR_CG__ObjectID;

typedef int             CMCC_3GCDR_CG_CallEventRecordType;
#define                     CMCC_3GCDR_CG_CallEventRecordType_sgsnPDPRecord 18
#define                     CMCC_3GCDR_CG_CallEventRecordType_ggsnPDPRecord 19
#define                     CMCC_3GCDR_CG_CallEventRecordType_sgsnMMRecord 20
#define                     CMCC_3GCDR_CG_CallEventRecordType_sgsnSMORecord 21
#define                     CMCC_3GCDR_CG_CallEventRecordType_sgsnSMTRecord 22

typedef ossBoolean      CMCC_3GCDR_CG_NetworkInitiatedPDPContext;

typedef struct CMCC_3GCDR_CG_IMSI {
    unsigned short  length;
    unsigned char   value[8];
} CMCC_3GCDR_CG_IMSI;

typedef struct CMCC_3GCDR_CG_IMEI {
    unsigned short  length;
    unsigned char   value[8];
} CMCC_3GCDR_CG_IMEI;

typedef struct CMCC_3GCDR_CG_IPBinaryAddress {
    unsigned short  choice;
#       define      CMCC_3GCDR_CG_iPBinV4Address_chosen 1
#       define      CMCC_3GCDR_CG_iPBinV6Address_chosen 2
    union _union {
        struct CMCC_3GCDR_CG__octet1 {
            unsigned short  length;
            unsigned char   value[4];
        } iPBinV4Address;  /* to choose, set choice to
                            * CMCC_3GCDR_CG_iPBinV4Address_chosen */
        struct CMCC_3GCDR_CG__octet2 {
            unsigned short  length;
            unsigned char   value[16];
        } iPBinV6Address;  /* to choose, set choice to
                            * CMCC_3GCDR_CG_iPBinV6Address_chosen */
    } u;
} CMCC_3GCDR_CG_IPBinaryAddress;

typedef struct CMCC_3GCDR_CG_IPTextRepresentedAddress {
    unsigned short  choice;
#       define      CMCC_3GCDR_CG_iPTextV4Address_chosen 1
#       define      CMCC_3GCDR_CG_iPTextV6Address_chosen 2
    union _union {
        char            iPTextV4Address[16];  /* to choose, set choice to
                                      * CMCC_3GCDR_CG_iPTextV4Address_chosen */
        char            iPTextV6Address[46];  /* to choose, set choice to
                                      * CMCC_3GCDR_CG_iPTextV6Address_chosen */
    } u;
} CMCC_3GCDR_CG_IPTextRepresentedAddress;

typedef struct CMCC_3GCDR_CG_IPAddress {
    unsigned short  choice;
#       define      CMCC_3GCDR_CG_iPBinaryAddress_chosen 1
#       define      CMCC_3GCDR_CG_iPTextRepresentedAddress_chosen 2
    union _union {
        CMCC_3GCDR_CG_IPBinaryAddress iPBinaryAddress;  /* to choose, set choice
                                   * to CMCC_3GCDR_CG_iPBinaryAddress_chosen */
        CMCC_3GCDR_CG_IPTextRepresentedAddress iPTextRepresentedAddress;  
                                        /* to choose, set choice to
                             * CMCC_3GCDR_CG_iPTextRepresentedAddress_chosen */
    } u;
} CMCC_3GCDR_CG_IPAddress;

typedef CMCC_3GCDR_CG_IPAddress CMCC_3GCDR_CG_GSNAddress;

typedef struct CMCC_3GCDR_CG_MSNetworkCapability {
    unsigned short  length;
    unsigned char   value[8];
} CMCC_3GCDR_CG_MSNetworkCapability;

typedef struct CMCC_3GCDR_CG_RoutingAreaCode {
    unsigned short  length;
    unsigned char   value[1];
} CMCC_3GCDR_CG_RoutingAreaCode;

typedef struct CMCC_3GCDR_CG_LocationAreaCode {
    unsigned short  length;
    unsigned char   value[2];
} CMCC_3GCDR_CG_LocationAreaCode;

typedef struct CMCC_3GCDR_CG_CellId {
    unsigned short  length;
    unsigned char   value[2];
} CMCC_3GCDR_CG_CellId;

typedef unsigned int    CMCC_3GCDR_CG_ChargingID;

typedef char            CMCC_3GCDR_CG_AccessPointNameNI[64];

typedef struct CMCC_3GCDR_CG_PDPType {
    unsigned short  length;
    unsigned char   value[2];
} CMCC_3GCDR_CG_PDPType;

typedef struct CMCC_3GCDR_CG_AddressString {
    unsigned short  length;
    unsigned char   value[20];
} CMCC_3GCDR_CG_AddressString;

typedef CMCC_3GCDR_CG_AddressString CMCC_3GCDR_CG_ETSIAddress;

typedef struct CMCC_3GCDR_CG_PDPAddress {
    unsigned short  choice;
#       define      CMCC_3GCDR_CG_iPAddress_chosen 1
#       define      CMCC_3GCDR_CG_eTSIAddress_chosen 2
    union _union {
        CMCC_3GCDR_CG_IPAddress iPAddress;  /* to choose, set choice to
                                            * CMCC_3GCDR_CG_iPAddress_chosen */
        CMCC_3GCDR_CG_ETSIAddress eTSIAddress;  /* to choose, set choice to
                                          * CMCC_3GCDR_CG_eTSIAddress_chosen */
    } u;
} CMCC_3GCDR_CG_PDPAddress;

typedef struct CMCC_3GCDR_CG_TimeStamp {
    unsigned short  length;
    unsigned char   value[9];
} CMCC_3GCDR_CG_TimeStamp;

typedef int             CMCC_3GCDR_CG_CallDuration;

typedef ossBoolean      CMCC_3GCDR_CG_SGSNChange;

typedef int             CMCC_3GCDR_CG_CauseForRecClosing;
#define                     CMCC_3GCDR_CG_normalRelease 0
#define                     CMCC_3GCDR_CG_abnormalRelease 4
#define                     CMCC_3GCDR_CG_cAMELInitCallRelease 5
#define                     CMCC_3GCDR_CG_volumeLimit 16
#define                     CMCC_3GCDR_CG_timeLimit 17
#define                     CMCC_3GCDR_CG_sGSNChange 18
#define                     CMCC_3GCDR_CG_maxChangeCond 19
#define                     CMCC_3GCDR_CG_managementIntervention 20

struct CMCC_3GCDR_CG__octet3 {
    unsigned int    length;
    unsigned char   *value;
};

typedef struct CMCC_3GCDR_CG_ManagementExtension {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_CG_significance_present 0x80
    CMCC_3GCDR_CG__ObjectID identifier;
    ossBoolean      significance;  /* CMCC_3GCDR_CG_significance_present not set
                                    * in bit_mask implies value is FALSE */
    struct CMCC_3GCDR_CG__octet3 information;
} CMCC_3GCDR_CG_ManagementExtension;

typedef struct CMCC_3GCDR_CG_Diagnostics {
    unsigned short  choice;
#       define      CMCC_3GCDR_CG_gsm0408Cause_chosen 1
#       define      CMCC_3GCDR_CG_gsm0902MapErrorValue_chosen 2
#       define      CMCC_3GCDR_CG_ccittQ767Cause_chosen 3
#       define      CMCC_3GCDR_CG_networkSpecificCause_chosen 4
#       define      CMCC_3GCDR_CG_manufacturerSpecificCause_chosen 5
    union _union {
        int             gsm0408Cause;  /* to choose, set choice to
                                        * CMCC_3GCDR_CG_gsm0408Cause_chosen */
        int             gsm0902MapErrorValue;  /* to choose, set choice to
                                 * CMCC_3GCDR_CG_gsm0902MapErrorValue_chosen */
        int             ccittQ767Cause;  /* to choose, set choice to
                                       * CMCC_3GCDR_CG_ccittQ767Cause_chosen */
        CMCC_3GCDR_CG_ManagementExtension networkSpecificCause;  /* to choose,
                                   * set choice to
                                 * CMCC_3GCDR_CG_networkSpecificCause_chosen */
        CMCC_3GCDR_CG_ManagementExtension manufacturerSpecificCause;  /* to
                                   * choose, set choice to
                            * CMCC_3GCDR_CG_manufacturerSpecificCause_chosen */
    } u;
} CMCC_3GCDR_CG_Diagnostics;

typedef char            CMCC_3GCDR_CG_NodeID[21];

typedef struct CMCC_3GCDR_CG_ManagementExtensions {
    unsigned int    count;
    CMCC_3GCDR_CG_ManagementExtension *value;
} CMCC_3GCDR_CG_ManagementExtensions;

typedef enum CMCC_3GCDR_CG_APNSelectionMode {
    CMCC_3GCDR_CG_mSorNetworkProvidedSubscriptionVerified = 0,
    CMCC_3GCDR_CG_mSProvidedSubscriptionNotVerified = 1,
    CMCC_3GCDR_CG_networkProvidedSubscriptionNotVerified = 2
} CMCC_3GCDR_CG_APNSelectionMode;

typedef char            CMCC_3GCDR_CG_AccessPointNameOI[38];

typedef struct CMCC_3GCDR_CG_ISDN_AddressString {
    unsigned short  length;
    unsigned char   value[9];
} CMCC_3GCDR_CG_ISDN_AddressString;

typedef CMCC_3GCDR_CG_ISDN_AddressString CMCC_3GCDR_CG_MSISDN;

typedef struct CMCC_3GCDR_CG_ChargingCharacteristics {
    unsigned short  length;
    unsigned char   value[2];
} CMCC_3GCDR_CG_ChargingCharacteristics;

typedef enum CMCC_3GCDR_CG_SystemType {
    CMCC_3GCDR_CG_unknown = 0,
    CMCC_3GCDR_CG_iuUTRAN = 1,
    CMCC_3GCDR_CG_gERAN = 2
} CMCC_3GCDR_CG_SystemType;

typedef CMCC_3GCDR_CG_AddressString CMCC_3GCDR_CG_SCFAddress;

typedef unsigned int    CMCC_3GCDR_CG_ServiceKey;

typedef enum CMCC_3GCDR_CG_DefaultGPRS_Handling {
    CMCC_3GCDR_CG_DefaultGPRS_Handling_continueTransaction = 0,
    CMCC_3GCDR_CG_DefaultGPRS_Handling_releaseTransaction = 1
} CMCC_3GCDR_CG_DefaultGPRS_Handling;

typedef CMCC_3GCDR_CG_AccessPointNameNI CMCC_3GCDR_CG_CAMELAccessPointNameNI;

typedef CMCC_3GCDR_CG_AccessPointNameOI CMCC_3GCDR_CG_CAMELAccessPointNameOI;

typedef int             CMCC_3GCDR_CG_NumberOfDPEncountered;

typedef unsigned char   CMCC_3GCDR_CG_LevelOfCAMELService;
#define                     CMCC_3GCDR_CG_basic 0x80
#define                     CMCC_3GCDR_CG_callDurationSupervision 0x40
#define                     CMCC_3GCDR_CG_onlineCharging 0x20

typedef struct CMCC_3GCDR_CG_FreeFormatData {
    unsigned short  length;
    unsigned char   value[160];
} CMCC_3GCDR_CG_FreeFormatData;

typedef ossBoolean      CMCC_3GCDR_CG_FFDAppendIndicator;

typedef struct CMCC_3GCDR_CG_CAMELInformationPDP {
    unsigned short  bit_mask;
#       define      CMCC_3GCDR_CG_CAMELInformationPDP_sCFAddress_present 0x8000
#       define      CMCC_3GCDR_CG_CAMELInformationPDP_serviceKey_present 0x4000
#       define      CMCC_3GCDR_CG_CAMELInformationPDP_defaultTransactionHandling_present 0x2000
#       define      CMCC_3GCDR_CG_cAMELAccessPointNameNI_present 0x1000
#       define      CMCC_3GCDR_CG_cAMELAccessPointNameOI_present 0x0800
#       define      CMCC_3GCDR_CG_CAMELInformationPDP_numberOfDPEncountered_present 0x0400
#       define      CMCC_3GCDR_CG_CAMELInformationPDP_levelOfCAMELService_present 0x0200
#       define      CMCC_3GCDR_CG_CAMELInformationPDP_freeFormatData_present 0x0100
#       define      CMCC_3GCDR_CG_CAMELInformationPDP_fFDAppendIndicator_present 0x0080
    CMCC_3GCDR_CG_SCFAddress sCFAddress;  /* optional; set in bit_mask
                      * CMCC_3GCDR_CG_CAMELInformationPDP_sCFAddress_present if
                      * present */
    CMCC_3GCDR_CG_ServiceKey serviceKey;  /* optional; set in bit_mask
                      * CMCC_3GCDR_CG_CAMELInformationPDP_serviceKey_present if
                      * present */
    CMCC_3GCDR_CG_DefaultGPRS_Handling defaultTransactionHandling;  
                                  /* optional; set in bit_mask
      * CMCC_3GCDR_CG_CAMELInformationPDP_defaultTransactionHandling_present if
      * present */
    CMCC_3GCDR_CG_CAMELAccessPointNameNI cAMELAccessPointNameNI;  /* optional;
                                   * set in bit_mask
                              * CMCC_3GCDR_CG_cAMELAccessPointNameNI_present if
                              * present */
    CMCC_3GCDR_CG_CAMELAccessPointNameOI cAMELAccessPointNameOI;  /* optional;
                                   * set in bit_mask
                              * CMCC_3GCDR_CG_cAMELAccessPointNameOI_present if
                              * present */
    CMCC_3GCDR_CG_NumberOfDPEncountered numberOfDPEncountered;  /* optional; set
                                   * in bit_mask
           * CMCC_3GCDR_CG_CAMELInformationPDP_numberOfDPEncountered_present if
           * present */
    CMCC_3GCDR_CG_LevelOfCAMELService levelOfCAMELService;  /* optional; set in
                                   * bit_mask
             * CMCC_3GCDR_CG_CAMELInformationPDP_levelOfCAMELService_present if
             * present */
    CMCC_3GCDR_CG_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                  * CMCC_3GCDR_CG_CAMELInformationPDP_freeFormatData_present if
                  * present */
    CMCC_3GCDR_CG_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in
                                   * bit_mask
              * CMCC_3GCDR_CG_CAMELInformationPDP_fFDAppendIndicator_present if
              * present */
} CMCC_3GCDR_CG_CAMELInformationPDP;

typedef enum CMCC_3GCDR_CG_ConsolidationResult {
    CMCC_3GCDR_CG_normal = 0,
    CMCC_3GCDR_CG_notnormal = 1,
    CMCC_3GCDR_CG_forInterSGSNConsolidation = 2,
    CMCC_3GCDR_CG_reachLimit = 3,
    CMCC_3GCDR_CG_onlyOneCDRGenerated = 4
} CMCC_3GCDR_CG_ConsolidationResult;

typedef struct CMCC_3GCDR_CG__seqof1 {
    unsigned int    count;
    CMCC_3GCDR_CG_GSNAddress *value;
} CMCC_3GCDR_CG__seqof1;

typedef struct CMCC_3GCDR_CG__seqof2 {
    unsigned int    count;
    struct CMCC_3GCDR_CG_ChangeOfCharCondition *value;
} CMCC_3GCDR_CG__seqof2;

typedef struct CMCC_3GCDR_CG_SGSNPDPRecord {
    unsigned int    bit_mask;
#       define      CMCC_3GCDR_CG_SGSNPDPRecord_networkInitiation_present 0x80000000
#       define      CMCC_3GCDR_CG_SGSNPDPRecord_servedIMEI_present 0x40000000
#       define      CMCC_3GCDR_CG_SGSNPDPRecord_msNetworkCapability_present 0x20000000
#       define      CMCC_3GCDR_CG_SGSNPDPRecord_routingArea_present 0x10000000
#       define      CMCC_3GCDR_CG_SGSNPDPRecord_locationAreaCode_present 0x08000000
#       define      CMCC_3GCDR_CG_SGSNPDPRecord_cellIdentifier_present 0x04000000
#       define      CMCC_3GCDR_CG_SGSNPDPRecord_servedPDPAddress_present 0x02000000
#       define      CMCC_3GCDR_CG_SGSNPDPRecord_sgsnChange_present 0x01000000
#       define      CMCC_3GCDR_CG_SGSNPDPRecord_diagnostics_present 0x00800000
#       define      CMCC_3GCDR_CG_SGSNPDPRecord_recSequenceNumList_present 0x00400000
#       define      CMCC_3GCDR_CG_SGSNPDPRecord_nodeID_present 0x00200000
#       define      CMCC_3GCDR_CG_SGSNPDPRecord_recordExtensions_present 0x00100000
#       define      CMCC_3GCDR_CG_SGSNPDPRecord_localSequenceNumber_present 0x00080000
#       define      CMCC_3GCDR_CG_SGSNPDPRecord_apnSelectionMode_present 0x00040000
#       define      CMCC_3GCDR_CG_SGSNPDPRecord_servedMSISDN_present 0x00020000
#       define      CMCC_3GCDR_CG_SGSNPDPRecord_chargingCharacteristics_present 0x00010000
#       define      CMCC_3GCDR_CG_SGSNPDPRecord_systemType_present 0x00008000
#       define      CMCC_3GCDR_CG_cAMELInformationPDP_present 0x00004000
#       define      CMCC_3GCDR_CG_rNCUnsentDownlinkVolumeList_present 0x00002000
    CMCC_3GCDR_CG_CallEventRecordType recordType;
    CMCC_3GCDR_CG_NetworkInitiatedPDPContext networkInitiation;  /* optional;
                                   * set in bit_mask
                     * CMCC_3GCDR_CG_SGSNPDPRecord_networkInitiation_present if
                     * present */
    CMCC_3GCDR_CG_IMSI servedIMSI;
    CMCC_3GCDR_CG_IMEI servedIMEI;  /* optional; set in bit_mask
                            * CMCC_3GCDR_CG_SGSNPDPRecord_servedIMEI_present if
                            * present */
    CMCC_3GCDR_CG__seqof1 sgsnAddress;
    CMCC_3GCDR_CG_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                   * CMCC_3GCDR_CG_SGSNPDPRecord_msNetworkCapability_present if
                   * present */
    CMCC_3GCDR_CG_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                           * CMCC_3GCDR_CG_SGSNPDPRecord_routingArea_present if
                           * present */
    CMCC_3GCDR_CG_LocationAreaCode locationAreaCode;  /* optional; set in
                                   * bit_mask
                      * CMCC_3GCDR_CG_SGSNPDPRecord_locationAreaCode_present if
                      * present */
    CMCC_3GCDR_CG_CellId cellIdentifier;  /* optional; set in bit_mask
                        * CMCC_3GCDR_CG_SGSNPDPRecord_cellIdentifier_present if
                        * present */
    CMCC_3GCDR_CG_ChargingID chargingID;
    CMCC_3GCDR_CG_GSNAddress ggsnAddressUsed;
    CMCC_3GCDR_CG_AccessPointNameNI accessPointNameNI;
    CMCC_3GCDR_CG_PDPType pdpType;
    CMCC_3GCDR_CG_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                      * CMCC_3GCDR_CG_SGSNPDPRecord_servedPDPAddress_present if
                      * present */
    CMCC_3GCDR_CG__seqof2 listOfTrafficVolumes;
    CMCC_3GCDR_CG_TimeStamp recordOpeningTime;
    CMCC_3GCDR_CG_CallDuration duration;
    CMCC_3GCDR_CG_SGSNChange sgsnChange;  /* optional; set in bit_mask
                            * CMCC_3GCDR_CG_SGSNPDPRecord_sgsnChange_present if
                            * present */
    CMCC_3GCDR_CG_CauseForRecClosing causeForRecClosing;
    CMCC_3GCDR_CG_Diagnostics diagnostics;  /* optional; set in bit_mask
                           * CMCC_3GCDR_CG_SGSNPDPRecord_diagnostics_present if
                           * present */
    struct CMCC_3GCDR_CG__seqof3 {
        unsigned int    count;
        struct CMCC_3GCDR_CG_SequenceList *value;
    } recSequenceNumList;  /* optional; set in bit_mask
                    * CMCC_3GCDR_CG_SGSNPDPRecord_recSequenceNumList_present if
                    * present */
    CMCC_3GCDR_CG_NodeID nodeID;  /* optional; set in bit_mask
                                   * CMCC_3GCDR_CG_SGSNPDPRecord_nodeID_present
                                   * if present */
    CMCC_3GCDR_CG_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                      * CMCC_3GCDR_CG_SGSNPDPRecord_recordExtensions_present if
                      * present */
    struct CMCC_3GCDR_CG__seqof4 {
        unsigned int    count;
        struct CMCC_3GCDR_CG_LocalSequenceNumberList *value;
    } localSequenceNumber;  /* optional; set in bit_mask
                   * CMCC_3GCDR_CG_SGSNPDPRecord_localSequenceNumber_present if
                   * present */
    CMCC_3GCDR_CG_APNSelectionMode apnSelectionMode;  /* optional; set in
                                   * bit_mask
                      * CMCC_3GCDR_CG_SGSNPDPRecord_apnSelectionMode_present if
                      * present */
    CMCC_3GCDR_CG_AccessPointNameOI accessPointNameOI;
    CMCC_3GCDR_CG_MSISDN servedMSISDN;  /* optional; set in bit_mask
                          * CMCC_3GCDR_CG_SGSNPDPRecord_servedMSISDN_present if
                          * present */
    CMCC_3GCDR_CG_ChargingCharacteristics chargingCharacteristics;  
                                  /* optional; set in bit_mask
               * CMCC_3GCDR_CG_SGSNPDPRecord_chargingCharacteristics_present if
               * present */
    CMCC_3GCDR_CG_SystemType systemType;  /* optional; set in bit_mask
                            * CMCC_3GCDR_CG_SGSNPDPRecord_systemType_present if
                            * present */
    CMCC_3GCDR_CG_CAMELInformationPDP cAMELInformationPDP;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_CG_cAMELInformationPDP_present
                                   * if present */
    struct CMCC_3GCDR_CG__seqof5 {
        unsigned int    count;
        struct CMCC_3GCDR_CG_RNCUnsentDownlinkVolumeList *value;
    } rNCUnsentDownlinkVolumeList;  /* optional; set in bit_mask
                         * CMCC_3GCDR_CG_rNCUnsentDownlinkVolumeList_present if
                         * present */
    CMCC_3GCDR_CG_ConsolidationResult consolidationResult;
} CMCC_3GCDR_CG_SGSNPDPRecord;

typedef ossBoolean      CMCC_3GCDR_CG_DynamicAddressFlag;

typedef struct CMCC_3GCDR_CG__seqof6 {
    unsigned int    count;
    int             *value;
} CMCC_3GCDR_CG__seqof6;

typedef struct CMCC_3GCDR_CG_SequenceList {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_CG_recSequenceNumber_present 0x80
    CMCC_3GCDR_CG_GSNAddress sgsnAddress;
    CMCC_3GCDR_CG__seqof6 recSequenceNumber;  /* optional; set in bit_mask
                                   * CMCC_3GCDR_CG_recSequenceNumber_present if
                                   * present */
} CMCC_3GCDR_CG_SequenceList;

typedef struct CMCC_3GCDR_CG_LocalSequenceNumberList {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_CG_localSeqNumberList_present 0x80
    CMCC_3GCDR_CG_GSNAddress gsnAddress;
    CMCC_3GCDR_CG__seqof6 localSeqNumberList;  /* optional; set in bit_mask
                                  * CMCC_3GCDR_CG_localSeqNumberList_present if
                                  * present */
} CMCC_3GCDR_CG_LocalSequenceNumberList;

typedef struct CMCC_3GCDR_CG_PLMN_Id {
    unsigned short  length;
    unsigned char   value[3];
} CMCC_3GCDR_CG_PLMN_Id;

typedef struct CMCC_3GCDR_CG_GGSNPDPRecord {
    unsigned short  bit_mask;
#       define      CMCC_3GCDR_CG_GGSNPDPRecord_networkInitiation_present 0x8000
#       define      CMCC_3GCDR_CG_GGSNPDPRecord_servedPDPAddress_present 0x4000
#       define      CMCC_3GCDR_CG_dynamicAddressFlag_present 0x2000
#       define      CMCC_3GCDR_CG_GGSNPDPRecord_diagnostics_present 0x1000
#       define      CMCC_3GCDR_CG_GGSNPDPRecord_recSequenceNumList_present 0x0800
#       define      CMCC_3GCDR_CG_GGSNPDPRecord_nodeID_present 0x0400
#       define      CMCC_3GCDR_CG_GGSNPDPRecord_recordExtensions_present 0x0200
#       define      CMCC_3GCDR_CG_localSequenceNumberList_present 0x0100
#       define      CMCC_3GCDR_CG_GGSNPDPRecord_apnSelectionMode_present 0x0080
#       define      CMCC_3GCDR_CG_GGSNPDPRecord_servedMSISDN_present 0x0040
#       define      CMCC_3GCDR_CG_GGSNPDPRecord_chargingCharacteristics_present 0x0020
#       define      CMCC_3GCDR_CG_sgsnPLMNIdentifier_present 0x0010
    CMCC_3GCDR_CG_CallEventRecordType recordType;
    CMCC_3GCDR_CG_NetworkInitiatedPDPContext networkInitiation;  /* optional;
                                   * set in bit_mask
                     * CMCC_3GCDR_CG_GGSNPDPRecord_networkInitiation_present if
                     * present */
    CMCC_3GCDR_CG_IMSI servedIMSI;
    CMCC_3GCDR_CG_GSNAddress ggsnAddress;
    CMCC_3GCDR_CG_ChargingID chargingID;
    CMCC_3GCDR_CG__seqof1 sgsnAddress;
    CMCC_3GCDR_CG_AccessPointNameNI accessPointNameNI;
    CMCC_3GCDR_CG_PDPType pdpType;
    CMCC_3GCDR_CG_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                      * CMCC_3GCDR_CG_GGSNPDPRecord_servedPDPAddress_present if
                      * present */
    CMCC_3GCDR_CG_DynamicAddressFlag dynamicAddressFlag;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_CG_dynamicAddressFlag_present if
                                   * present */
    CMCC_3GCDR_CG__seqof2 listOfTrafficVolumes;
    CMCC_3GCDR_CG_TimeStamp recordOpeningTime;
    CMCC_3GCDR_CG_CallDuration duration;
    CMCC_3GCDR_CG_CauseForRecClosing causeForRecClosing;
    CMCC_3GCDR_CG_Diagnostics diagnostics;  /* optional; set in bit_mask
                           * CMCC_3GCDR_CG_GGSNPDPRecord_diagnostics_present if
                           * present */
    CMCC_3GCDR_CG_SequenceList recSequenceNumList;  /* optional; set in bit_mask
                    * CMCC_3GCDR_CG_GGSNPDPRecord_recSequenceNumList_present if
                    * present */
    CMCC_3GCDR_CG_NodeID nodeID;  /* optional; set in bit_mask
                                   * CMCC_3GCDR_CG_GGSNPDPRecord_nodeID_present
                                   * if present */
    CMCC_3GCDR_CG_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                      * CMCC_3GCDR_CG_GGSNPDPRecord_recordExtensions_present if
                      * present */
    CMCC_3GCDR_CG_LocalSequenceNumberList localSequenceNumberList;  
                                  /* optional; set in bit_mask
                             * CMCC_3GCDR_CG_localSequenceNumberList_present if
                             * present */
    CMCC_3GCDR_CG_APNSelectionMode apnSelectionMode;  /* optional; set in
                                   * bit_mask
                      * CMCC_3GCDR_CG_GGSNPDPRecord_apnSelectionMode_present if
                      * present */
    CMCC_3GCDR_CG_MSISDN servedMSISDN;  /* optional; set in bit_mask
                          * CMCC_3GCDR_CG_GGSNPDPRecord_servedMSISDN_present if
                          * present */
    CMCC_3GCDR_CG_ChargingCharacteristics chargingCharacteristics;  
                                  /* optional; set in bit_mask
               * CMCC_3GCDR_CG_GGSNPDPRecord_chargingCharacteristics_present if
               * present */
    CMCC_3GCDR_CG_PLMN_Id sgsnPLMNIdentifier;  /* optional; set in bit_mask
                                  * CMCC_3GCDR_CG_sgsnPLMNIdentifier_present if
                                  * present */
    CMCC_3GCDR_CG_ConsolidationResult consolidationResult;
} CMCC_3GCDR_CG_GGSNPDPRecord;

typedef unsigned int    CMCC_3GCDR_CG_LocalSequenceNumber;

typedef struct CMCC_3GCDR_CG_CAMELInformationMM {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_CG_CAMELInformationMM_sCFAddress_present 0x80
#       define      CMCC_3GCDR_CG_CAMELInformationMM_serviceKey_present 0x40
#       define      CMCC_3GCDR_CG_CAMELInformationMM_defaultTransactionHandling_present 0x20
#       define      CMCC_3GCDR_CG_CAMELInformationMM_numberOfDPEncountered_present 0x10
#       define      CMCC_3GCDR_CG_CAMELInformationMM_levelOfCAMELService_present 0x08
#       define      CMCC_3GCDR_CG_CAMELInformationMM_freeFormatData_present 0x04
#       define      CMCC_3GCDR_CG_CAMELInformationMM_fFDAppendIndicator_present 0x02
    CMCC_3GCDR_CG_SCFAddress sCFAddress;  /* optional; set in bit_mask
                       * CMCC_3GCDR_CG_CAMELInformationMM_sCFAddress_present if
                       * present */
    CMCC_3GCDR_CG_ServiceKey serviceKey;  /* optional; set in bit_mask
                       * CMCC_3GCDR_CG_CAMELInformationMM_serviceKey_present if
                       * present */
    CMCC_3GCDR_CG_DefaultGPRS_Handling defaultTransactionHandling;  
                                  /* optional; set in bit_mask
       * CMCC_3GCDR_CG_CAMELInformationMM_defaultTransactionHandling_present if
       * present */
    CMCC_3GCDR_CG_NumberOfDPEncountered numberOfDPEncountered;  /* optional; set
                                   * in bit_mask
            * CMCC_3GCDR_CG_CAMELInformationMM_numberOfDPEncountered_present if
            * present */
    CMCC_3GCDR_CG_LevelOfCAMELService levelOfCAMELService;  /* optional; set in
                                   * bit_mask
              * CMCC_3GCDR_CG_CAMELInformationMM_levelOfCAMELService_present if
              * present */
    CMCC_3GCDR_CG_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                   * CMCC_3GCDR_CG_CAMELInformationMM_freeFormatData_present if
                   * present */
    CMCC_3GCDR_CG_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in
                                   * bit_mask
               * CMCC_3GCDR_CG_CAMELInformationMM_fFDAppendIndicator_present if
               * present */
} CMCC_3GCDR_CG_CAMELInformationMM;

typedef struct CMCC_3GCDR_CG_SGSNMMRecord {
    unsigned int    bit_mask;
#       define      CMCC_3GCDR_CG_SGSNMMRecord_servedIMEI_present 0x80000000
#       define      CMCC_3GCDR_CG_SGSNMMRecord_msNetworkCapability_present 0x40000000
#       define      CMCC_3GCDR_CG_SGSNMMRecord_routingArea_present 0x20000000
#       define      CMCC_3GCDR_CG_SGSNMMRecord_locationAreaCode_present 0x10000000
#       define      CMCC_3GCDR_CG_SGSNMMRecord_cellIdentifier_present 0x08000000
#       define      CMCC_3GCDR_CG_changeLocation_present 0x04000000
#       define      CMCC_3GCDR_CG_duration_present 0x02000000
#       define      CMCC_3GCDR_CG_SGSNMMRecord_sgsnChange_present 0x01000000
#       define      CMCC_3GCDR_CG_SGSNMMRecord_diagnostics_present 0x00800000
#       define      CMCC_3GCDR_CG_recordSequenceNumber_present 0x00400000
#       define      CMCC_3GCDR_CG_SGSNMMRecord_nodeID_present 0x00200000
#       define      CMCC_3GCDR_CG_SGSNMMRecord_recordExtensions_present 0x00100000
#       define      CMCC_3GCDR_CG_SGSNMMRecord_localSequenceNumber_present 0x00080000
#       define      CMCC_3GCDR_CG_SGSNMMRecord_servedMSISDN_present 0x00040000
#       define      CMCC_3GCDR_CG_SGSNMMRecord_chargingCharacteristics_present 0x00020000
#       define      CMCC_3GCDR_CG_cAMELInformationMM_present 0x00010000
#       define      CMCC_3GCDR_CG_SGSNMMRecord_systemType_present 0x00008000
    CMCC_3GCDR_CG_CallEventRecordType recordType;
    CMCC_3GCDR_CG_IMSI servedIMSI;
    CMCC_3GCDR_CG_IMEI servedIMEI;  /* optional; set in bit_mask
                             * CMCC_3GCDR_CG_SGSNMMRecord_servedIMEI_present if
                             * present */
    CMCC_3GCDR_CG_GSNAddress sgsnAddress;
    CMCC_3GCDR_CG_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                    * CMCC_3GCDR_CG_SGSNMMRecord_msNetworkCapability_present if
                    * present */
    CMCC_3GCDR_CG_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                            * CMCC_3GCDR_CG_SGSNMMRecord_routingArea_present if
                            * present */
    CMCC_3GCDR_CG_LocationAreaCode locationAreaCode;  /* optional; set in
                                   * bit_mask
                       * CMCC_3GCDR_CG_SGSNMMRecord_locationAreaCode_present if
                       * present */
    CMCC_3GCDR_CG_CellId cellIdentifier;  /* optional; set in bit_mask
                         * CMCC_3GCDR_CG_SGSNMMRecord_cellIdentifier_present if
                         * present */
    struct CMCC_3GCDR_CG__seqof7 {
        unsigned int    count;
        struct CMCC_3GCDR_CG_ChangeLocation *value;
    } changeLocation;  /* optional; set in bit_mask
                        * CMCC_3GCDR_CG_changeLocation_present if present */
    CMCC_3GCDR_CG_TimeStamp recordOpeningTime;
    CMCC_3GCDR_CG_CallDuration duration;  /* optional; set in bit_mask
                                           * CMCC_3GCDR_CG_duration_present if
                                           * present */
    CMCC_3GCDR_CG_SGSNChange sgsnChange;  /* optional; set in bit_mask
                             * CMCC_3GCDR_CG_SGSNMMRecord_sgsnChange_present if
                             * present */
    CMCC_3GCDR_CG_CauseForRecClosing causeForRecClosing;
    CMCC_3GCDR_CG_Diagnostics diagnostics;  /* optional; set in bit_mask
                            * CMCC_3GCDR_CG_SGSNMMRecord_diagnostics_present if
                            * present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                                * CMCC_3GCDR_CG_recordSequenceNumber_present if
                                * present */
    CMCC_3GCDR_CG_NodeID nodeID;  /* optional; set in bit_mask
                                   * CMCC_3GCDR_CG_SGSNMMRecord_nodeID_present
                                   * if present */
    CMCC_3GCDR_CG_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                       * CMCC_3GCDR_CG_SGSNMMRecord_recordExtensions_present if
                       * present */
    CMCC_3GCDR_CG_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                    * CMCC_3GCDR_CG_SGSNMMRecord_localSequenceNumber_present if
                    * present */
    CMCC_3GCDR_CG_MSISDN servedMSISDN;  /* optional; set in bit_mask
                           * CMCC_3GCDR_CG_SGSNMMRecord_servedMSISDN_present if
                           * present */
    CMCC_3GCDR_CG_ChargingCharacteristics chargingCharacteristics;  
                                  /* optional; set in bit_mask
                * CMCC_3GCDR_CG_SGSNMMRecord_chargingCharacteristics_present if
                * present */
    CMCC_3GCDR_CG_CAMELInformationMM cAMELInformationMM;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_CG_cAMELInformationMM_present if
                                   * present */
    CMCC_3GCDR_CG_SystemType systemType;  /* optional; set in bit_mask
                             * CMCC_3GCDR_CG_SGSNMMRecord_systemType_present if
                             * present */
} CMCC_3GCDR_CG_SGSNMMRecord;

typedef CMCC_3GCDR_CG_AddressString CMCC_3GCDR_CG_RecordingEntity;

typedef struct CMCC_3GCDR_CG_MessageReference {
    unsigned int    length;
    unsigned char   *value;
} CMCC_3GCDR_CG_MessageReference;

typedef CMCC_3GCDR_CG_Diagnostics CMCC_3GCDR_CG_SMSResult;

typedef struct CMCC_3GCDR_CG_CalledNumber {
    unsigned short  length;
    unsigned char   value[10];
} CMCC_3GCDR_CG_CalledNumber;

typedef enum CMCC_3GCDR_CG_DefaultSMS_Handling {
    CMCC_3GCDR_CG_DefaultSMS_Handling_continueTransaction = 0,
    CMCC_3GCDR_CG_DefaultSMS_Handling_releaseTransaction = 1
} CMCC_3GCDR_CG_DefaultSMS_Handling;

typedef struct CMCC_3GCDR_CG_CallingNumber {
    unsigned short  length;
    unsigned char   value[10];
} CMCC_3GCDR_CG_CallingNumber;

typedef struct CMCC_3GCDR_CG_CallReferenceNumber {
    unsigned short  length;
    unsigned char   value[8];
} CMCC_3GCDR_CG_CallReferenceNumber;

typedef struct CMCC_3GCDR_CG_CAMELInformationSMS {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_CG_CAMELInformationSMS_sCFAddress_present 0x80
#       define      CMCC_3GCDR_CG_CAMELInformationSMS_serviceKey_present 0x40
#       define      CMCC_3GCDR_CG_defaultSMSHandling_present 0x20
#       define      CMCC_3GCDR_CG_cAMELCallingPartyNumber_present 0x10
#       define      CMCC_3GCDR_CG_cAMELDestinationSubscriberNumber_present 0x08
#       define      CMCC_3GCDR_CG_cAMELSMSCAddress_present 0x04
#       define      CMCC_3GCDR_CG_CAMELInformationSMS_freeFormatData_present 0x02
#       define      CMCC_3GCDR_CG_sMSReferenceNumber_present 0x01
    CMCC_3GCDR_CG_SCFAddress sCFAddress;  /* optional; set in bit_mask
                      * CMCC_3GCDR_CG_CAMELInformationSMS_sCFAddress_present if
                      * present */
    CMCC_3GCDR_CG_ServiceKey serviceKey;  /* optional; set in bit_mask
                      * CMCC_3GCDR_CG_CAMELInformationSMS_serviceKey_present if
                      * present */
    CMCC_3GCDR_CG_DefaultSMS_Handling defaultSMSHandling;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_CG_defaultSMSHandling_present if
                                   * present */
    CMCC_3GCDR_CG_CallingNumber cAMELCallingPartyNumber;  /* optional; set in
                                   * bit_mask
                             * CMCC_3GCDR_CG_cAMELCallingPartyNumber_present if
                             * present */
    CMCC_3GCDR_CG_CalledNumber cAMELDestinationSubscriberNumber;  /* optional;
                                   * set in bit_mask
                    * CMCC_3GCDR_CG_cAMELDestinationSubscriberNumber_present if
                    * present */
    CMCC_3GCDR_CG_AddressString cAMELSMSCAddress;  /* optional; set in bit_mask
                                    * CMCC_3GCDR_CG_cAMELSMSCAddress_present if
                                    * present */
    CMCC_3GCDR_CG_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                  * CMCC_3GCDR_CG_CAMELInformationSMS_freeFormatData_present if
                  * present */
    CMCC_3GCDR_CG_CallReferenceNumber sMSReferenceNumber;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_CG_sMSReferenceNumber_present if
                                   * present */
} CMCC_3GCDR_CG_CAMELInformationSMS;

typedef struct CMCC_3GCDR_CG_SGSNSMORecord {
    unsigned short  bit_mask;
#       define      CMCC_3GCDR_CG_SGSNSMORecord_servedIMEI_present 0x8000
#       define      CMCC_3GCDR_CG_SGSNSMORecord_servedMSISDN_present 0x4000
#       define      CMCC_3GCDR_CG_SGSNSMORecord_locationArea_present 0x2000
#       define      CMCC_3GCDR_CG_SGSNSMORecord_routingArea_present 0x1000
#       define      CMCC_3GCDR_CG_SGSNSMORecord_cellIdentifier_present 0x0800
#       define      CMCC_3GCDR_CG_SGSNSMORecord_smsResult_present 0x0400
#       define      CMCC_3GCDR_CG_SGSNSMORecord_recordExtensions_present 0x0200
#       define      CMCC_3GCDR_CG_SGSNSMORecord_nodeID_present 0x0100
#       define      CMCC_3GCDR_CG_SGSNSMORecord_localSequenceNumber_present 0x0080
#       define      CMCC_3GCDR_CG_SGSNSMORecord_chargingCharacteristics_present 0x0040
#       define      CMCC_3GCDR_CG_SGSNSMORecord_systemType_present 0x0020
#       define      CMCC_3GCDR_CG_destinationNumber_present 0x0010
#       define      CMCC_3GCDR_CG_cAMELInformationSMS_present 0x0008
    CMCC_3GCDR_CG_CallEventRecordType recordType;
    CMCC_3GCDR_CG_IMSI servedIMSI;
    CMCC_3GCDR_CG_IMEI servedIMEI;  /* optional; set in bit_mask
                            * CMCC_3GCDR_CG_SGSNSMORecord_servedIMEI_present if
                            * present */
    CMCC_3GCDR_CG_MSISDN servedMSISDN;  /* optional; set in bit_mask
                          * CMCC_3GCDR_CG_SGSNSMORecord_servedMSISDN_present if
                          * present */
    CMCC_3GCDR_CG_MSNetworkCapability msNetworkCapability;
    CMCC_3GCDR_CG_AddressString serviceCentre;
    CMCC_3GCDR_CG_RecordingEntity recordingEntity;
    CMCC_3GCDR_CG_LocationAreaCode locationArea;  /* optional; set in bit_mask
                          * CMCC_3GCDR_CG_SGSNSMORecord_locationArea_present if
                          * present */
    CMCC_3GCDR_CG_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                           * CMCC_3GCDR_CG_SGSNSMORecord_routingArea_present if
                           * present */
    CMCC_3GCDR_CG_CellId cellIdentifier;  /* optional; set in bit_mask
                        * CMCC_3GCDR_CG_SGSNSMORecord_cellIdentifier_present if
                        * present */
    CMCC_3GCDR_CG_MessageReference messageReference;
    CMCC_3GCDR_CG_TimeStamp originationTime;
    CMCC_3GCDR_CG_SMSResult smsResult;  /* optional; set in bit_mask
                             * CMCC_3GCDR_CG_SGSNSMORecord_smsResult_present if
                             * present */
    CMCC_3GCDR_CG_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                      * CMCC_3GCDR_CG_SGSNSMORecord_recordExtensions_present if
                      * present */
    CMCC_3GCDR_CG_NodeID nodeID;  /* optional; set in bit_mask
                                   * CMCC_3GCDR_CG_SGSNSMORecord_nodeID_present
                                   * if present */
    CMCC_3GCDR_CG_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                   * CMCC_3GCDR_CG_SGSNSMORecord_localSequenceNumber_present if
                   * present */
    CMCC_3GCDR_CG_ChargingCharacteristics chargingCharacteristics;  
                                  /* optional; set in bit_mask
               * CMCC_3GCDR_CG_SGSNSMORecord_chargingCharacteristics_present if
               * present */
    CMCC_3GCDR_CG_SystemType systemType;  /* optional; set in bit_mask
                            * CMCC_3GCDR_CG_SGSNSMORecord_systemType_present if
                            * present */
    CMCC_3GCDR_CG_CalledNumber destinationNumber;  /* optional; set in bit_mask
                                   * CMCC_3GCDR_CG_destinationNumber_present if
                                   * present */
    CMCC_3GCDR_CG_CAMELInformationSMS cAMELInformationSMS;  /* optional; set in
                                   * bit_mask
                                   * CMCC_3GCDR_CG_cAMELInformationSMS_present
                                   * if present */
} CMCC_3GCDR_CG_SGSNSMORecord;

typedef struct CMCC_3GCDR_CG_SGSNSMTRecord {
    unsigned short  bit_mask;
#       define      CMCC_3GCDR_CG_SGSNSMTRecord_servedIMEI_present 0x8000
#       define      CMCC_3GCDR_CG_SGSNSMTRecord_servedMSISDN_present 0x4000
#       define      CMCC_3GCDR_CG_SGSNSMTRecord_locationArea_present 0x2000
#       define      CMCC_3GCDR_CG_SGSNSMTRecord_routingArea_present 0x1000
#       define      CMCC_3GCDR_CG_SGSNSMTRecord_cellIdentifier_present 0x0800
#       define      CMCC_3GCDR_CG_SGSNSMTRecord_smsResult_present 0x0400
#       define      CMCC_3GCDR_CG_SGSNSMTRecord_recordExtensions_present 0x0200
#       define      CMCC_3GCDR_CG_SGSNSMTRecord_nodeID_present 0x0100
#       define      CMCC_3GCDR_CG_SGSNSMTRecord_localSequenceNumber_present 0x0080
#       define      CMCC_3GCDR_CG_SGSNSMTRecord_chargingCharacteristics_present 0x0040
#       define      CMCC_3GCDR_CG_SGSNSMTRecord_systemType_present 0x0020
    CMCC_3GCDR_CG_CallEventRecordType recordType;
    CMCC_3GCDR_CG_IMSI servedIMSI;
    CMCC_3GCDR_CG_IMEI servedIMEI;  /* optional; set in bit_mask
                            * CMCC_3GCDR_CG_SGSNSMTRecord_servedIMEI_present if
                            * present */
    CMCC_3GCDR_CG_MSISDN servedMSISDN;  /* optional; set in bit_mask
                          * CMCC_3GCDR_CG_SGSNSMTRecord_servedMSISDN_present if
                          * present */
    CMCC_3GCDR_CG_MSNetworkCapability msNetworkCapability;
    CMCC_3GCDR_CG_AddressString serviceCentre;
    CMCC_3GCDR_CG_RecordingEntity recordingEntity;
    CMCC_3GCDR_CG_LocationAreaCode locationArea;  /* optional; set in bit_mask
                          * CMCC_3GCDR_CG_SGSNSMTRecord_locationArea_present if
                          * present */
    CMCC_3GCDR_CG_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                           * CMCC_3GCDR_CG_SGSNSMTRecord_routingArea_present if
                           * present */
    CMCC_3GCDR_CG_CellId cellIdentifier;  /* optional; set in bit_mask
                        * CMCC_3GCDR_CG_SGSNSMTRecord_cellIdentifier_present if
                        * present */
    CMCC_3GCDR_CG_TimeStamp originationTime;
    CMCC_3GCDR_CG_SMSResult smsResult;  /* optional; set in bit_mask
                             * CMCC_3GCDR_CG_SGSNSMTRecord_smsResult_present if
                             * present */
    CMCC_3GCDR_CG_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                      * CMCC_3GCDR_CG_SGSNSMTRecord_recordExtensions_present if
                      * present */
    CMCC_3GCDR_CG_NodeID nodeID;  /* optional; set in bit_mask
                                   * CMCC_3GCDR_CG_SGSNSMTRecord_nodeID_present
                                   * if present */
    CMCC_3GCDR_CG_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                   * CMCC_3GCDR_CG_SGSNSMTRecord_localSequenceNumber_present if
                   * present */
    CMCC_3GCDR_CG_ChargingCharacteristics chargingCharacteristics;  
                                  /* optional; set in bit_mask
               * CMCC_3GCDR_CG_SGSNSMTRecord_chargingCharacteristics_present if
               * present */
    CMCC_3GCDR_CG_SystemType systemType;  /* optional; set in bit_mask
                            * CMCC_3GCDR_CG_SGSNSMTRecord_systemType_present if
                            * present */
} CMCC_3GCDR_CG_SGSNSMTRecord;

typedef struct CMCC_3GCDR_CG_CallEventRecord {
    unsigned short  choice;
#       define      CMCC_3GCDR_CG_sgsnPDPRecord_chosen 1
#       define      CMCC_3GCDR_CG_ggsnPDPRecord_chosen 2
#       define      CMCC_3GCDR_CG_sgsnMMRecord_chosen 3
#       define      CMCC_3GCDR_CG_sgsnSMORecord_chosen 4
#       define      CMCC_3GCDR_CG_sgsnSMTRecord_chosen 5
    union _union {
        CMCC_3GCDR_CG_SGSNPDPRecord sgsnPDPRecord;  /* to choose, set choice to
                                        * CMCC_3GCDR_CG_sgsnPDPRecord_chosen */
        CMCC_3GCDR_CG_GGSNPDPRecord ggsnPDPRecord;  /* to choose, set choice to
                                        * CMCC_3GCDR_CG_ggsnPDPRecord_chosen */
        CMCC_3GCDR_CG_SGSNMMRecord sgsnMMRecord;  /* to choose, set choice to
                                         * CMCC_3GCDR_CG_sgsnMMRecord_chosen */
        CMCC_3GCDR_CG_SGSNSMORecord sgsnSMORecord;  /* to choose, set choice to
                                        * CMCC_3GCDR_CG_sgsnSMORecord_chosen */
        CMCC_3GCDR_CG_SGSNSMTRecord sgsnSMTRecord;  /* to choose, set choice to
                                        * CMCC_3GCDR_CG_sgsnSMTRecord_chosen */
    } u;
} CMCC_3GCDR_CG_CallEventRecord;

typedef enum CMCC_3GCDR_CG_ChangeCondition {
    CMCC_3GCDR_CG_qoSChange = 0,
    CMCC_3GCDR_CG_tariffTime = 1,
    CMCC_3GCDR_CG_recordClosure = 2
} CMCC_3GCDR_CG_ChangeCondition;

typedef enum CMCC_3GCDR_CG_QoSReliability {
    CMCC_3GCDR_CG_unspecifiedReliability = 0,
    CMCC_3GCDR_CG_acknowledgedGTP = 1,
    CMCC_3GCDR_CG_unackGTPAcknowLLC = 2,
    CMCC_3GCDR_CG_unackGTPLLCAcknowRLC = 3,
    CMCC_3GCDR_CG_unackGTPLLCRLC = 4,
    CMCC_3GCDR_CG_unacknowUnprotectedData = 5
} CMCC_3GCDR_CG_QoSReliability;

typedef enum CMCC_3GCDR_CG_QoSDelay {
    CMCC_3GCDR_CG_delayClass1 = 1,
    CMCC_3GCDR_CG_delayClass2 = 2,
    CMCC_3GCDR_CG_delayClass3 = 3,
    CMCC_3GCDR_CG_delayClass4 = 4
} CMCC_3GCDR_CG_QoSDelay;

typedef enum CMCC_3GCDR_CG_QoSPrecedence {
    CMCC_3GCDR_CG_QoSPrecedence_unspecified = 0,
    CMCC_3GCDR_CG_highPriority = 1,
    CMCC_3GCDR_CG_normalPriority = 2,
    CMCC_3GCDR_CG_lowPriority = 3
} CMCC_3GCDR_CG_QoSPrecedence;

typedef enum CMCC_3GCDR_CG_QoSPeakThroughput {
    CMCC_3GCDR_CG_QoSPeakThroughput_unspecified = 0,
    CMCC_3GCDR_CG_upTo1000octetPs = 1,
    CMCC_3GCDR_CG_upTo2000octetPs = 2,
    CMCC_3GCDR_CG_upTo4000octetPs = 3,
    CMCC_3GCDR_CG_upTo8000octetPs = 4,
    CMCC_3GCDR_CG_upTo16000octetPs = 5,
    CMCC_3GCDR_CG_upTo32000octetPs = 6,
    CMCC_3GCDR_CG_upTo64000octetPs = 7,
    CMCC_3GCDR_CG_upTo128000octetPs = 8,
    CMCC_3GCDR_CG_upTo256000octetPs = 9
} CMCC_3GCDR_CG_QoSPeakThroughput;

typedef enum CMCC_3GCDR_CG_QoSMeanThroughput {
    CMCC_3GCDR_CG_subscribedMeanThroughput = 0,
    CMCC_3GCDR_CG_mean100octetPh = 1,
    CMCC_3GCDR_CG_mean200octetPh = 2,
    CMCC_3GCDR_CG_mean500octetPh = 3,
    CMCC_3GCDR_CG_mean1000octetPh = 4,
    CMCC_3GCDR_CG_mean2000octetPh = 5,
    CMCC_3GCDR_CG_mean5000octetPh = 6,
    CMCC_3GCDR_CG_mean10000octetPh = 7,
    CMCC_3GCDR_CG_mean20000octetPh = 8,
    CMCC_3GCDR_CG_mean50000octetPh = 9,
    CMCC_3GCDR_CG_mean100000octetPh = 10,
    CMCC_3GCDR_CG_mean200000octetPh = 11,
    CMCC_3GCDR_CG_mean500000octetPh = 12,
    CMCC_3GCDR_CG_mean1000000octetPh = 13,
    CMCC_3GCDR_CG_mean2000000octetPh = 14,
    CMCC_3GCDR_CG_mean5000000octetPh = 15,
    CMCC_3GCDR_CG_mean10000000octetPh = 16,
    CMCC_3GCDR_CG_mean20000000octetPh = 17,
    CMCC_3GCDR_CG_mean50000000octetPh = 18,
    CMCC_3GCDR_CG_reserved = 30,
    CMCC_3GCDR_CG_bestEffort = 31
} CMCC_3GCDR_CG_QoSMeanThroughput;

typedef struct CMCC_3GCDR_CG_GSMQoSInformation {
    CMCC_3GCDR_CG_QoSReliability reliability;
    CMCC_3GCDR_CG_QoSDelay delay;
    CMCC_3GCDR_CG_QoSPrecedence precedence;
    CMCC_3GCDR_CG_QoSPeakThroughput peakThroughput;
    CMCC_3GCDR_CG_QoSMeanThroughput meanThroughput;
} CMCC_3GCDR_CG_GSMQoSInformation;

typedef struct CMCC_3GCDR_CG_QoSInformation {
    unsigned short  choice;
#       define      CMCC_3GCDR_CG_gsmQosInformation_chosen 1
#       define      CMCC_3GCDR_CG_umtsQosInformation_chosen 2
    union _union {
        CMCC_3GCDR_CG_GSMQoSInformation gsmQosInformation;  /* to choose, set
                                   * choice to
                                   * CMCC_3GCDR_CG_gsmQosInformation_chosen */
        struct CMCC_3GCDR_CG__octet4 {
            unsigned short  length;
            unsigned char   value[12];
        } umtsQosInformation;  /* to choose, set choice to
                                * CMCC_3GCDR_CG_umtsQosInformation_chosen */
    } u;
} CMCC_3GCDR_CG_QoSInformation;

typedef int             CMCC_3GCDR_CG_DataVolumeGPRS;

typedef struct CMCC_3GCDR_CG_ChangeOfCharCondition {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_CG_qosRequested_present 0x80
#       define      CMCC_3GCDR_CG_qosNegotiated_present 0x40
    CMCC_3GCDR_CG_QoSInformation qosRequested;  /* optional; set in bit_mask
                                        * CMCC_3GCDR_CG_qosRequested_present if
                                        * present */
    CMCC_3GCDR_CG_QoSInformation qosNegotiated;  /* optional; set in bit_mask
                                       * CMCC_3GCDR_CG_qosNegotiated_present if
                                       * present */
    CMCC_3GCDR_CG_DataVolumeGPRS dataVolumeGPRSUplink;
    CMCC_3GCDR_CG_DataVolumeGPRS dataVolumeGPRSDownlink;
    CMCC_3GCDR_CG_ChangeCondition changeCondition;
    CMCC_3GCDR_CG_TimeStamp changeTime;
} CMCC_3GCDR_CG_ChangeOfCharCondition;

typedef struct CMCC_3GCDR_CG_ChangeLocation {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_CG_cellId_present 0x80
    CMCC_3GCDR_CG_LocationAreaCode locationAreaCode;
    CMCC_3GCDR_CG_RoutingAreaCode routingAreaCode;
    CMCC_3GCDR_CG_CellId cellId;  /* optional; set in bit_mask
                                   * CMCC_3GCDR_CG_cellId_present if present */
    CMCC_3GCDR_CG_TimeStamp changeTime;
} CMCC_3GCDR_CG_ChangeLocation;

typedef struct CMCC_3GCDR_CG_RNCUnsentDownlinkVolumeList {
    unsigned char   bit_mask;
#       define      CMCC_3GCDR_CG_rNCUnsentDnVolumeList_present 0x80
    CMCC_3GCDR_CG_GSNAddress sgsnAddress;
    CMCC_3GCDR_CG__seqof6 rNCUnsentDnVolumeList;  /* optional; set in bit_mask
                               * CMCC_3GCDR_CG_rNCUnsentDnVolumeList_present if
                               * present */
} CMCC_3GCDR_CG_RNCUnsentDownlinkVolumeList;

extern CMCC_3GCDR_CG__ObjectID CMCC_3GCDR_CG_gsm1205InformationModel;

extern CMCC_3GCDR_CG__ObjectID CMCC_3GCDR_CG_gsm1205ASN1Module;

extern int CMCC_3GCDR_CG_maxAddressLength;

extern int CMCC_3GCDR_CG_maxISDN_AddressLength;


extern void *CDRF_R99_Fin;    /* encoder-decoder control table */
#ifdef __cplusplus
}	/* extern "C" */
#endif /* __cplusplus */
#endif /* OSS_CDRF_R99_Fin */
