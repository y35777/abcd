/********************************************************/
/* Copyright (C) 2003 OSS Nokalva.  All rights reserved.*/
/********************************************************/

/* THIS FILE IS PROPRIETARY MATERIAL OF OSS NOKALVA
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF OSS NOKALVA.
 * THIS FILE MAY NOT BE DISTRIBUTED. */

/* Generated for: HuaWei Technologies, Inc., Shenzhen, China - license 8088 for Windows NT */
/* Abstract syntax: CDRF_3GCDR_M */
/* Created: Thu Jan 09 15:52:34 2003 */
/* ASN.1 compiler version: 5.3 */
/* Target operating system: Windows NT/Windows 9x */
/* Target machine type: Intel x86 */
/* C compiler options required: -Zp4 (Microsoft) */
/* ASN.1 compiler options and file names specified:
 * -codefile CDRF_3GCDR_M.c -headerfile CDRF_3GCDR_M.h -errorfile
 * C:\WINDOWS\TEMP\2. -prefix CDR3G_M_ -nodebug -dualheader -ansi -verbose -ber
 * -root -charintegers
 * D:\application\ossasn1\win32\5.3.0\asn1dflt\asn1dflt.ms.zp4
 * D:\project\asn1app\Oss_3GCDR_z4\jinying\CDRF_3GCDR_M.ASN
 */

#ifndef OSS_CDRF_3GCDR_M
#define OSS_CDRF_3GCDR_M

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "asn1hdr.h"
#include "asn1code.h"

#define          CDR3G_M_BSRecord_PDU 1

typedef struct CDR3G_M__ObjectID {
    unsigned short  length;
    unsigned char   *value;
} CDR3G_M__ObjectID;

typedef struct CDR3G_M__Any {
    unsigned long   length;
    unsigned char   *value;
} CDR3G_M__Any;

typedef struct CDR3G_M__octet1 {
    unsigned short  length;
    unsigned char   value[15];
} CDR3G_M__octet1;

typedef struct CDR3G_M__octet3 {
    unsigned short  length;
    unsigned char   value[14];
} CDR3G_M__octet3;

typedef struct CDR3G_M__octet4 {
    unsigned short  length;
    unsigned char   value[10];
} CDR3G_M__octet4;

typedef struct CDR3G_M_BSXferSummaryRecord {
    CDR3G_M__octet1 cgnID;
    struct CDR3G_M__octet2 {
        unsigned short  length;
        unsigned char   value[34];
    } bsXferFile;
    CDR3G_M__octet3 openTime;
    CDR3G_M__octet3 closeTime;
    CDR3G_M__octet4 lrsnStart;
    CDR3G_M__octet4 lrsnEnd;
    struct CDR3G_M__octet5 {
        unsigned short  length;
        unsigned char   value[3];
    } cdrType;
    CDR3G_M__octet4 totalCdr;
    CDR3G_M__octet4 missingCdr;
    CDR3G_M__octet4 duplicateCdr;
    CDR3G_M__octet1 cgID;
} CDR3G_M_BSXferSummaryRecord;

typedef struct CDR3G_M_BSRecord {
    CDR3G_M_BSXferSummaryRecord bsXferRecord;
    struct CDR3G_M__seqof1 {
        unsigned int    count;
        struct CDR3G_M_CallEventRecord *value;
    } listOfCDR;
} CDR3G_M_BSRecord;

typedef int             CDR3G_M_CallEventRecordType;
#define                     CDR3G_M_CallEventRecordType_sgsnPDPRecord 18
#define                     CDR3G_M_CallEventRecordType_ggsnPDPRecord 19
#define                     CDR3G_M_CallEventRecordType_sgsnMMRecord 20
#define                     CDR3G_M_CallEventRecordType_sgsnSMORecord 21
#define                     CDR3G_M_CallEventRecordType_sgsnSMTRecord 22

typedef ossBoolean      CDR3G_M_NetworkInitiatedPDPContext;

typedef struct CDR3G_M_IMSI {
    unsigned short  length;
    unsigned char   value[8];
} CDR3G_M_IMSI;

typedef struct CDR3G_M_IMEI {
    unsigned short  length;
    unsigned char   value[8];
} CDR3G_M_IMEI;

typedef struct CDR3G_M_IPBinaryAddress {
    unsigned short  choice;
#       define      CDR3G_M_iPBinV4Address_chosen 1
#       define      CDR3G_M_iPBinV6Address_chosen 2
    union _union {
        struct CDR3G_M__octet6 {
            unsigned short  length;
            unsigned char   value[4];
        } iPBinV4Address;  /* to choose, set choice to
                            * CDR3G_M_iPBinV4Address_chosen */
        struct CDR3G_M__octet7 {
            unsigned short  length;
            unsigned char   value[16];
        } iPBinV6Address;  /* to choose, set choice to
                            * CDR3G_M_iPBinV6Address_chosen */
    } u;
} CDR3G_M_IPBinaryAddress;

typedef struct CDR3G_M_IPTextRepresentedAddress {
    unsigned short  choice;
#       define      CDR3G_M_iPTextV4Address_chosen 1
#       define      CDR3G_M_iPTextV6Address_chosen 2
    union _union {
        char            iPTextV4Address[16];  /* to choose, set choice to
                                            * CDR3G_M_iPTextV4Address_chosen */
        char            iPTextV6Address[46];  /* to choose, set choice to
                                            * CDR3G_M_iPTextV6Address_chosen */
    } u;
} CDR3G_M_IPTextRepresentedAddress;

typedef struct CDR3G_M_IPAddress {
    unsigned short  choice;
#       define      CDR3G_M_iPBinaryAddress_chosen 1
#       define      CDR3G_M_iPTextRepresentedAddress_chosen 2
    union _union {
        CDR3G_M_IPBinaryAddress iPBinaryAddress;  /* to choose, set choice to
                                            * CDR3G_M_iPBinaryAddress_chosen */
        CDR3G_M_IPTextRepresentedAddress iPTextRepresentedAddress;  /* to
                                   * choose, set choice to
                                   * CDR3G_M_iPTextRepresentedAddress_chosen */
    } u;
} CDR3G_M_IPAddress;

typedef CDR3G_M_IPAddress CDR3G_M_GSNAddress;

typedef struct CDR3G_M_MSNetworkCapability {
    unsigned short  length;
    unsigned char   value[1];
} CDR3G_M_MSNetworkCapability;

typedef struct CDR3G_M_RoutingAreaCode {
    unsigned short  length;
    unsigned char   value[1];
} CDR3G_M_RoutingAreaCode;

typedef struct CDR3G_M_LocationAreaCode {
    unsigned short  length;
    unsigned char   value[2];
} CDR3G_M_LocationAreaCode;

typedef struct CDR3G_M_CellId {
    unsigned short  length;
    unsigned char   value[2];
} CDR3G_M_CellId;

typedef unsigned int    CDR3G_M_ChargingID;

typedef char            CDR3G_M_AccessPointNameNI[64];

typedef struct CDR3G_M_PDPType {
    unsigned short  length;
    unsigned char   value[2];
} CDR3G_M_PDPType;

typedef struct CDR3G_M_AddressString {
    unsigned short  length;
    unsigned char   value[20];
} CDR3G_M_AddressString;

typedef CDR3G_M_AddressString CDR3G_M_ETSIAddress;

typedef struct CDR3G_M_PDPAddress {
    unsigned short  choice;
#       define      CDR3G_M_iPAddress_chosen 1
#       define      CDR3G_M_eTSIAddress_chosen 2
    union _union {
        CDR3G_M_IPAddress iPAddress;  /* to choose, set choice to
                                       * CDR3G_M_iPAddress_chosen */
        CDR3G_M_ETSIAddress eTSIAddress;  /* to choose, set choice to
                                           * CDR3G_M_eTSIAddress_chosen */
    } u;
} CDR3G_M_PDPAddress;

typedef struct CDR3G_M_TimeStamp {
    unsigned short  length;
    unsigned char   value[9];
} CDR3G_M_TimeStamp;

typedef int             CDR3G_M_CallDuration;

typedef ossBoolean      CDR3G_M_SGSNChange;

typedef int             CDR3G_M_CauseForRecClosing;
#define                     CDR3G_M_normalRelease 0
#define                     CDR3G_M_abnormalRelease 4
#define                     CDR3G_M_cAMELInitCallRelease 5
#define                     CDR3G_M_volumeLimit 16
#define                     CDR3G_M_timeLimit 17
#define                     CDR3G_M_sGSNChange 18
#define                     CDR3G_M_maxChangeCond 19
#define                     CDR3G_M_managementIntervention 20

typedef struct CDR3G_M_ManagementExtension {
    unsigned char   bit_mask;
#       define      CDR3G_M_significance_present 0x80
    CDR3G_M__ObjectID identifier;
    ossBoolean      significance;  /* CDR3G_M_significance_present not set in
                                    * bit_mask implies value is FALSE */
    CDR3G_M__Any    information;
} CDR3G_M_ManagementExtension;

typedef struct CDR3G_M_Diagnostics {
    unsigned short  choice;
#       define      CDR3G_M_gsm0408Cause_chosen 1
#       define      CDR3G_M_gsm0902MapErrorValue_chosen 2
#       define      CDR3G_M_ccittQ767Cause_chosen 3
#       define      CDR3G_M_networkSpecificCause_chosen 4
#       define      CDR3G_M_manufacturerSpecificCause_chosen 5
    union _union {
        int             gsm0408Cause;  /* to choose, set choice to
                                        * CDR3G_M_gsm0408Cause_chosen */
        int             gsm0902MapErrorValue;  /* to choose, set choice to
                                       * CDR3G_M_gsm0902MapErrorValue_chosen */
        int             ccittQ767Cause;  /* to choose, set choice to
                                          * CDR3G_M_ccittQ767Cause_chosen */
        CDR3G_M_ManagementExtension networkSpecificCause;  /* to choose, set
                                   * choice to
                                   * CDR3G_M_networkSpecificCause_chosen */
        CDR3G_M_ManagementExtension manufacturerSpecificCause;  /* to choose,
                                   * set choice to
                                  * CDR3G_M_manufacturerSpecificCause_chosen */
    } u;
} CDR3G_M_Diagnostics;

typedef char            CDR3G_M_NodeID[21];

typedef struct CDR3G_M_ManagementExtensions {
    unsigned int    count;
    CDR3G_M_ManagementExtension *value;
} CDR3G_M_ManagementExtensions;

typedef unsigned int    CDR3G_M_LocalSequenceNumber;

typedef enum CDR3G_M_APNSelectionMode {
    CDR3G_M_mSorNetworkProvidedSubscriptionVerified = 0,
    CDR3G_M_mSProvidedSubscriptionNotVerified = 1,
    CDR3G_M_networkProvidedSubscriptionNotVerified = 2
} CDR3G_M_APNSelectionMode;

typedef char            CDR3G_M_AccessPointNameOI[38];

typedef struct CDR3G_M_ISDN_AddressString {
    unsigned short  length;
    unsigned char   value[9];
} CDR3G_M_ISDN_AddressString;

typedef CDR3G_M_ISDN_AddressString CDR3G_M_MSISDN;

typedef struct CDR3G_M_ChargingCharacteristics {
    unsigned short  length;
    unsigned char   value[2];
} CDR3G_M_ChargingCharacteristics;

typedef enum CDR3G_M_SystemType {
    CDR3G_M_unknown = 0,
    CDR3G_M_iuUTRAN = 1,
    CDR3G_M_gERAN = 2
} CDR3G_M_SystemType;

typedef CDR3G_M_AddressString CDR3G_M_SCFAddress;

typedef unsigned int    CDR3G_M_ServiceKey;

typedef enum CDR3G_M_DefaultGPRS_Handling {
    CDR3G_M_DefaultGPRS_Handling_continueTransaction = 0,
    CDR3G_M_DefaultGPRS_Handling_releaseTransaction = 1
} CDR3G_M_DefaultGPRS_Handling;

typedef CDR3G_M_AccessPointNameNI CDR3G_M_CAMELAccessPointNameNI;

typedef CDR3G_M_AccessPointNameOI CDR3G_M_CAMELAccessPointNameOI;

typedef int             CDR3G_M_NumberOfDPEncountered;

typedef unsigned char   CDR3G_M_LevelOfCAMELService;
#define                     CDR3G_M_basic 0x80
#define                     CDR3G_M_callDurationSupervision 0x40
#define                     CDR3G_M_onlineCharging 0x20

typedef struct CDR3G_M_FreeFormatData {
    unsigned short  length;
    unsigned char   value[160];
} CDR3G_M_FreeFormatData;

typedef ossBoolean      CDR3G_M_FFDAppendIndicator;

typedef struct CDR3G_M_CAMELInformationPDP {
    unsigned short  bit_mask;
#       define      CDR3G_M_CAMELInformationPDP_sCFAddress_present 0x8000
#       define      CDR3G_M_CAMELInformationPDP_serviceKey_present 0x4000
#       define      CDR3G_M_CAMELInformationPDP_defaultTransactionHandling_present 0x2000
#       define      CDR3G_M_cAMELAccessPointNameNI_present 0x1000
#       define      CDR3G_M_cAMELAccessPointNameOI_present 0x0800
#       define      CDR3G_M_CAMELInformationPDP_numberOfDPEncountered_present 0x0400
#       define      CDR3G_M_CAMELInformationPDP_levelOfCAMELService_present 0x0200
#       define      CDR3G_M_CAMELInformationPDP_freeFormatData_present 0x0100
#       define      CDR3G_M_CAMELInformationPDP_fFDAppendIndicator_present 0x0080
    CDR3G_M_SCFAddress sCFAddress;  /* optional; set in bit_mask
                            * CDR3G_M_CAMELInformationPDP_sCFAddress_present if
                            * present */
    CDR3G_M_ServiceKey serviceKey;  /* optional; set in bit_mask
                            * CDR3G_M_CAMELInformationPDP_serviceKey_present if
                            * present */
    CDR3G_M_DefaultGPRS_Handling defaultTransactionHandling;  /* optional; set
                                   * in bit_mask
            * CDR3G_M_CAMELInformationPDP_defaultTransactionHandling_present if
            * present */
    CDR3G_M_CAMELAccessPointNameNI cAMELAccessPointNameNI;  /* optional; set in
                                   * bit_mask
                                   * CDR3G_M_cAMELAccessPointNameNI_present if
                                   * present */
    CDR3G_M_CAMELAccessPointNameOI cAMELAccessPointNameOI;  /* optional; set in
                                   * bit_mask
                                   * CDR3G_M_cAMELAccessPointNameOI_present if
                                   * present */
    CDR3G_M_NumberOfDPEncountered numberOfDPEncountered;  /* optional; set in
                                   * bit_mask
                 * CDR3G_M_CAMELInformationPDP_numberOfDPEncountered_present if
                 * present */
    CDR3G_M_LevelOfCAMELService levelOfCAMELService;  /* optional; set in
                                   * bit_mask
                   * CDR3G_M_CAMELInformationPDP_levelOfCAMELService_present if
                   * present */
    CDR3G_M_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                        * CDR3G_M_CAMELInformationPDP_freeFormatData_present if
                        * present */
    CDR3G_M_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in bit_mask
                    * CDR3G_M_CAMELInformationPDP_fFDAppendIndicator_present if
                    * present */
} CDR3G_M_CAMELInformationPDP;

typedef int             CDR3G_M_DataVolumeGPRS;

typedef struct CDR3G_M__seqof2 {
    unsigned int    count;
    struct CDR3G_M_ChangeOfCharCondition *value;
} CDR3G_M__seqof2;

typedef struct CDR3G_M_SGSNPDPRecord {
    unsigned int    bit_mask;
#       define      CDR3G_M_SGSNPDPRecord_networkInitiation_present 0x80000000
#       define      CDR3G_M_SGSNPDPRecord_servedIMEI_present 0x40000000
#       define      CDR3G_M_SGSNPDPRecord_msNetworkCapability_present 0x20000000
#       define      CDR3G_M_SGSNPDPRecord_routingArea_present 0x10000000
#       define      CDR3G_M_SGSNPDPRecord_locationAreaCode_present 0x08000000
#       define      CDR3G_M_SGSNPDPRecord_cellIdentifier_present 0x04000000
#       define      CDR3G_M_SGSNPDPRecord_servedPDPAddress_present 0x02000000
#       define      CDR3G_M_SGSNPDPRecord_sgsnChange_present 0x01000000
#       define      CDR3G_M_SGSNPDPRecord_diagnostics_present 0x00800000
#       define      CDR3G_M_SGSNPDPRecord_recordSequenceNumber_present 0x00400000
#       define      CDR3G_M_SGSNPDPRecord_nodeID_present 0x00200000
#       define      CDR3G_M_SGSNPDPRecord_recordExtensions_present 0x00100000
#       define      CDR3G_M_SGSNPDPRecord_localSequenceNumber_present 0x00080000
#       define      CDR3G_M_SGSNPDPRecord_apnSelectionMode_present 0x00040000
#       define      CDR3G_M_SGSNPDPRecord_servedMSISDN_present 0x00020000
#       define      CDR3G_M_SGSNPDPRecord_chargingCharacteristics_present 0x00010000
#       define      CDR3G_M_SGSNPDPRecord_systemType_present 0x00008000
#       define      CDR3G_M_cAMELInformationPDP_present 0x00004000
#       define      CDR3G_M_rNCUnsentDownlinkVolume_present 0x00002000
    CDR3G_M_CallEventRecordType recordType;
    CDR3G_M_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                           * CDR3G_M_SGSNPDPRecord_networkInitiation_present if
                           * present */
    CDR3G_M_IMSI    servedIMSI;
    CDR3G_M_IMEI    servedIMEI;  /* optional; set in bit_mask
                                  * CDR3G_M_SGSNPDPRecord_servedIMEI_present if
                                  * present */
    CDR3G_M_GSNAddress sgsnAddress;
    CDR3G_M_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                         * CDR3G_M_SGSNPDPRecord_msNetworkCapability_present if
                         * present */
    CDR3G_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                 * CDR3G_M_SGSNPDPRecord_routingArea_present if
                                 * present */
    CDR3G_M_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                            * CDR3G_M_SGSNPDPRecord_locationAreaCode_present if
                            * present */
    CDR3G_M_CellId  cellIdentifier;  /* optional; set in bit_mask
                              * CDR3G_M_SGSNPDPRecord_cellIdentifier_present if
                              * present */
    CDR3G_M_ChargingID chargingID;
    CDR3G_M_GSNAddress ggsnAddressUsed;
    CDR3G_M_AccessPointNameNI accessPointNameNI;
    CDR3G_M_PDPType pdpType;
    CDR3G_M_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                            * CDR3G_M_SGSNPDPRecord_servedPDPAddress_present if
                            * present */
    CDR3G_M__seqof2 listOfTrafficVolumes;
    CDR3G_M_TimeStamp recordOpeningTime;
    CDR3G_M_CallDuration duration;
    CDR3G_M_SGSNChange sgsnChange;  /* optional; set in bit_mask
                                     * CDR3G_M_SGSNPDPRecord_sgsnChange_present
                                     * if present */
    CDR3G_M_CauseForRecClosing causeForRecClosing;
    CDR3G_M_Diagnostics diagnostics;  /* optional; set in bit_mask
                                 * CDR3G_M_SGSNPDPRecord_diagnostics_present if
                                 * present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                        * CDR3G_M_SGSNPDPRecord_recordSequenceNumber_present if
                        * present */
    CDR3G_M_NodeID  nodeID;  /* optional; set in bit_mask
                              * CDR3G_M_SGSNPDPRecord_nodeID_present if
                              * present */
    CDR3G_M_ManagementExtensions recordExtensions;  /* optional; set in bit_mask
                            * CDR3G_M_SGSNPDPRecord_recordExtensions_present if
                            * present */
    CDR3G_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                         * CDR3G_M_SGSNPDPRecord_localSequenceNumber_present if
                         * present */
    CDR3G_M_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                            * CDR3G_M_SGSNPDPRecord_apnSelectionMode_present if
                            * present */
    CDR3G_M_AccessPointNameOI accessPointNameOI;
    CDR3G_M_MSISDN  servedMSISDN;  /* optional; set in bit_mask
                                    * CDR3G_M_SGSNPDPRecord_servedMSISDN_present
                                    * if present */
    CDR3G_M_ChargingCharacteristics chargingCharacteristics;  /* optional; set
                                   * in bit_mask
                     * CDR3G_M_SGSNPDPRecord_chargingCharacteristics_present if
                     * present */
    CDR3G_M_SystemType systemType;  /* optional; set in bit_mask
                                     * CDR3G_M_SGSNPDPRecord_systemType_present
                                     * if present */
    CDR3G_M_CAMELInformationPDP cAMELInformationPDP;  /* optional; set in
                                   * bit_mask
                                   * CDR3G_M_cAMELInformationPDP_present if
                                   * present */
    CDR3G_M_DataVolumeGPRS rNCUnsentDownlinkVolume;  /* optional; set in
                                   * bit_mask
                                   * CDR3G_M_rNCUnsentDownlinkVolume_present if
                                   * present */
} CDR3G_M_SGSNPDPRecord;

typedef ossBoolean      CDR3G_M_DynamicAddressFlag;

typedef struct CDR3G_M_GGSNPDPRecord {
    unsigned short  bit_mask;
#       define      CDR3G_M_GGSNPDPRecord_networkInitiation_present 0x8000
#       define      CDR3G_M_GGSNPDPRecord_servedPDPAddress_present 0x4000
#       define      CDR3G_M_dynamicAddressFlag_present 0x2000
#       define      CDR3G_M_GGSNPDPRecord_diagnostics_present 0x1000
#       define      CDR3G_M_GGSNPDPRecord_recordSequenceNumber_present 0x0800
#       define      CDR3G_M_GGSNPDPRecord_nodeID_present 0x0400
#       define      CDR3G_M_GGSNPDPRecord_recordExtensions_present 0x0200
#       define      CDR3G_M_GGSNPDPRecord_localSequenceNumber_present 0x0100
#       define      CDR3G_M_GGSNPDPRecord_apnSelectionMode_present 0x0080
#       define      CDR3G_M_GGSNPDPRecord_servedMSISDN_present 0x0040
#       define      CDR3G_M_GGSNPDPRecord_chargingCharacteristics_present 0x0020
    CDR3G_M_CallEventRecordType recordType;
    CDR3G_M_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                           * CDR3G_M_GGSNPDPRecord_networkInitiation_present if
                           * present */
    CDR3G_M_IMSI    servedIMSI;
    CDR3G_M_GSNAddress ggsnAddress;
    CDR3G_M_ChargingID chargingID;
    struct CDR3G_M__seqof3 {
        unsigned int    count;
        CDR3G_M_GSNAddress *value;
    } sgsnAddress;
    CDR3G_M_AccessPointNameNI accessPointNameNI;
    CDR3G_M_PDPType pdpType;
    CDR3G_M_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                            * CDR3G_M_GGSNPDPRecord_servedPDPAddress_present if
                            * present */
    CDR3G_M_DynamicAddressFlag dynamicAddressFlag;  /* optional; set in bit_mask
                                        * CDR3G_M_dynamicAddressFlag_present if
                                        * present */
    CDR3G_M__seqof2 listOfTrafficVolumes;
    CDR3G_M_TimeStamp recordOpeningTime;
    CDR3G_M_CallDuration duration;
    CDR3G_M_CauseForRecClosing causeForRecClosing;
    CDR3G_M_Diagnostics diagnostics;  /* optional; set in bit_mask
                                 * CDR3G_M_GGSNPDPRecord_diagnostics_present if
                                 * present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                        * CDR3G_M_GGSNPDPRecord_recordSequenceNumber_present if
                        * present */
    CDR3G_M_NodeID  nodeID;  /* optional; set in bit_mask
                              * CDR3G_M_GGSNPDPRecord_nodeID_present if
                              * present */
    CDR3G_M_ManagementExtensions recordExtensions;  /* optional; set in bit_mask
                            * CDR3G_M_GGSNPDPRecord_recordExtensions_present if
                            * present */
    CDR3G_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                         * CDR3G_M_GGSNPDPRecord_localSequenceNumber_present if
                         * present */
    CDR3G_M_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                            * CDR3G_M_GGSNPDPRecord_apnSelectionMode_present if
                            * present */
    CDR3G_M_MSISDN  servedMSISDN;  /* optional; set in bit_mask
                                    * CDR3G_M_GGSNPDPRecord_servedMSISDN_present
                                    * if present */
    CDR3G_M_ChargingCharacteristics chargingCharacteristics;  /* optional; set
                                   * in bit_mask
                     * CDR3G_M_GGSNPDPRecord_chargingCharacteristics_present if
                     * present */
} CDR3G_M_GGSNPDPRecord;

typedef struct CDR3G_M_CAMELInformationMM {
    unsigned char   bit_mask;
#       define      CDR3G_M_CAMELInformationMM_sCFAddress_present 0x80
#       define      CDR3G_M_CAMELInformationMM_serviceKey_present 0x40
#       define      CDR3G_M_CAMELInformationMM_defaultTransactionHandling_present 0x20
#       define      CDR3G_M_CAMELInformationMM_numberOfDPEncountered_present 0x10
#       define      CDR3G_M_CAMELInformationMM_levelOfCAMELService_present 0x08
#       define      CDR3G_M_CAMELInformationMM_freeFormatData_present 0x04
#       define      CDR3G_M_CAMELInformationMM_fFDAppendIndicator_present 0x02
    CDR3G_M_SCFAddress sCFAddress;  /* optional; set in bit_mask
                             * CDR3G_M_CAMELInformationMM_sCFAddress_present if
                             * present */
    CDR3G_M_ServiceKey serviceKey;  /* optional; set in bit_mask
                             * CDR3G_M_CAMELInformationMM_serviceKey_present if
                             * present */
    CDR3G_M_DefaultGPRS_Handling defaultTransactionHandling;  /* optional; set
                                   * in bit_mask
             * CDR3G_M_CAMELInformationMM_defaultTransactionHandling_present if
             * present */
    CDR3G_M_NumberOfDPEncountered numberOfDPEncountered;  /* optional; set in
                                   * bit_mask
                  * CDR3G_M_CAMELInformationMM_numberOfDPEncountered_present if
                  * present */
    CDR3G_M_LevelOfCAMELService levelOfCAMELService;  /* optional; set in
                                   * bit_mask
                    * CDR3G_M_CAMELInformationMM_levelOfCAMELService_present if
                    * present */
    CDR3G_M_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                         * CDR3G_M_CAMELInformationMM_freeFormatData_present if
                         * present */
    CDR3G_M_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in bit_mask
                     * CDR3G_M_CAMELInformationMM_fFDAppendIndicator_present if
                     * present */
} CDR3G_M_CAMELInformationMM;

typedef struct CDR3G_M_SGSNMMRecord {
    unsigned int    bit_mask;
#       define      CDR3G_M_SGSNMMRecord_servedIMEI_present 0x80000000
#       define      CDR3G_M_SGSNMMRecord_msNetworkCapability_present 0x40000000
#       define      CDR3G_M_SGSNMMRecord_routingArea_present 0x20000000
#       define      CDR3G_M_SGSNMMRecord_locationAreaCode_present 0x10000000
#       define      CDR3G_M_SGSNMMRecord_cellIdentifier_present 0x08000000
#       define      CDR3G_M_changeLocation_present 0x04000000
#       define      CDR3G_M_duration_present 0x02000000
#       define      CDR3G_M_SGSNMMRecord_sgsnChange_present 0x01000000
#       define      CDR3G_M_SGSNMMRecord_diagnostics_present 0x00800000
#       define      CDR3G_M_SGSNMMRecord_recordSequenceNumber_present 0x00400000
#       define      CDR3G_M_SGSNMMRecord_nodeID_present 0x00200000
#       define      CDR3G_M_SGSNMMRecord_recordExtensions_present 0x00100000
#       define      CDR3G_M_SGSNMMRecord_localSequenceNumber_present 0x00080000
#       define      CDR3G_M_SGSNMMRecord_servedMSISDN_present 0x00040000
#       define      CDR3G_M_SGSNMMRecord_chargingCharacteristics_present 0x00020000
#       define      CDR3G_M_cAMELInformationMM_present 0x00010000
#       define      CDR3G_M_SGSNMMRecord_systemType_present 0x00008000
    CDR3G_M_CallEventRecordType recordType;
    CDR3G_M_IMSI    servedIMSI;
    CDR3G_M_IMEI    servedIMEI;  /* optional; set in bit_mask
                                  * CDR3G_M_SGSNMMRecord_servedIMEI_present if
                                  * present */
    CDR3G_M_GSNAddress sgsnAddress;
    CDR3G_M_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                          * CDR3G_M_SGSNMMRecord_msNetworkCapability_present if
                          * present */
    CDR3G_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                  * CDR3G_M_SGSNMMRecord_routingArea_present if
                                  * present */
    CDR3G_M_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                             * CDR3G_M_SGSNMMRecord_locationAreaCode_present if
                             * present */
    CDR3G_M_CellId  cellIdentifier;  /* optional; set in bit_mask
                               * CDR3G_M_SGSNMMRecord_cellIdentifier_present if
                               * present */
    struct CDR3G_M__seqof4 {
        unsigned int    count;
        struct CDR3G_M_ChangeLocation *value;
    } changeLocation;  /* optional; set in bit_mask
                        * CDR3G_M_changeLocation_present if present */
    CDR3G_M_TimeStamp recordOpeningTime;
    CDR3G_M_CallDuration duration;  /* optional; set in bit_mask
                                     * CDR3G_M_duration_present if present */
    CDR3G_M_SGSNChange sgsnChange;  /* optional; set in bit_mask
                                     * CDR3G_M_SGSNMMRecord_sgsnChange_present
                                     * if present */
    CDR3G_M_CauseForRecClosing causeForRecClosing;
    CDR3G_M_Diagnostics diagnostics;  /* optional; set in bit_mask
                                  * CDR3G_M_SGSNMMRecord_diagnostics_present if
                                  * present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                         * CDR3G_M_SGSNMMRecord_recordSequenceNumber_present if
                         * present */
    CDR3G_M_NodeID  nodeID;  /* optional; set in bit_mask
                              * CDR3G_M_SGSNMMRecord_nodeID_present if
                              * present */
    CDR3G_M_ManagementExtensions recordExtensions;  /* optional; set in bit_mask
                             * CDR3G_M_SGSNMMRecord_recordExtensions_present if
                             * present */
    CDR3G_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                          * CDR3G_M_SGSNMMRecord_localSequenceNumber_present if
                          * present */
    CDR3G_M_MSISDN  servedMSISDN;  /* optional; set in bit_mask
                                    * CDR3G_M_SGSNMMRecord_servedMSISDN_present
                                    * if present */
    CDR3G_M_ChargingCharacteristics chargingCharacteristics;  /* optional; set
                                   * in bit_mask
                      * CDR3G_M_SGSNMMRecord_chargingCharacteristics_present if
                      * present */
    CDR3G_M_CAMELInformationMM cAMELInformationMM;  /* optional; set in bit_mask
                                        * CDR3G_M_cAMELInformationMM_present if
                                        * present */
    CDR3G_M_SystemType systemType;  /* optional; set in bit_mask
                                     * CDR3G_M_SGSNMMRecord_systemType_present
                                     * if present */
} CDR3G_M_SGSNMMRecord;

typedef CDR3G_M_AddressString CDR3G_M_RecordingEntity;

typedef struct CDR3G_M_MessageReference {
    unsigned int    length;
    unsigned char   *value;
} CDR3G_M_MessageReference;

typedef CDR3G_M_Diagnostics CDR3G_M_SMSResult;

typedef struct CDR3G_M_CalledNumber {
    unsigned short  length;
    unsigned char   value[10];
} CDR3G_M_CalledNumber;

typedef enum CDR3G_M_DefaultSMS_Handling {
    CDR3G_M_DefaultSMS_Handling_continueTransaction = 0,
    CDR3G_M_DefaultSMS_Handling_releaseTransaction = 1
} CDR3G_M_DefaultSMS_Handling;

typedef struct CDR3G_M_CallingNumber {
    unsigned short  length;
    unsigned char   value[10];
} CDR3G_M_CallingNumber;

typedef struct CDR3G_M_CAMELInformationSMS {
    unsigned char   bit_mask;
#       define      CDR3G_M_CAMELInformationSMS_sCFAddress_present 0x80
#       define      CDR3G_M_CAMELInformationSMS_serviceKey_present 0x40
#       define      CDR3G_M_defaultSMSHandling_present 0x20
#       define      CDR3G_M_cAMELCallingPartyNumber_present 0x10
#       define      CDR3G_M_cAMELDestinationSubscriberNumber_present 0x08
#       define      CDR3G_M_cAMELSMSCAddress_present 0x04
#       define      CDR3G_M_CAMELInformationSMS_freeFormatData_present 0x02
    CDR3G_M_SCFAddress sCFAddress;  /* optional; set in bit_mask
                            * CDR3G_M_CAMELInformationSMS_sCFAddress_present if
                            * present */
    CDR3G_M_ServiceKey serviceKey;  /* optional; set in bit_mask
                            * CDR3G_M_CAMELInformationSMS_serviceKey_present if
                            * present */
    CDR3G_M_DefaultSMS_Handling defaultSMSHandling;  /* optional; set in
                                   * bit_mask CDR3G_M_defaultSMSHandling_present
                                   * if present */
    CDR3G_M_CallingNumber cAMELCallingPartyNumber;  /* optional; set in bit_mask
                                   * CDR3G_M_cAMELCallingPartyNumber_present if
                                   * present */
    CDR3G_M_CalledNumber cAMELDestinationSubscriberNumber;  /* optional; set in
                                   * bit_mask
                          * CDR3G_M_cAMELDestinationSubscriberNumber_present if
                          * present */
    CDR3G_M_AddressString cAMELSMSCAddress;  /* optional; set in bit_mask
                                              * CDR3G_M_cAMELSMSCAddress_present
                                              * if present */
    CDR3G_M_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                        * CDR3G_M_CAMELInformationSMS_freeFormatData_present if
                        * present */
} CDR3G_M_CAMELInformationSMS;

typedef struct CDR3G_M_SGSNSMORecord {
    unsigned short  bit_mask;
#       define      CDR3G_M_SGSNSMORecord_servedIMEI_present 0x8000
#       define      CDR3G_M_SGSNSMORecord_servedMSISDN_present 0x4000
#       define      CDR3G_M_SGSNSMORecord_locationArea_present 0x2000
#       define      CDR3G_M_SGSNSMORecord_routingArea_present 0x1000
#       define      CDR3G_M_SGSNSMORecord_cellIdentifier_present 0x0800
#       define      CDR3G_M_SGSNSMORecord_smsResult_present 0x0400
#       define      CDR3G_M_SGSNSMORecord_recordExtensions_present 0x0200
#       define      CDR3G_M_SGSNSMORecord_nodeID_present 0x0100
#       define      CDR3G_M_SGSNSMORecord_localSequenceNumber_present 0x0080
#       define      CDR3G_M_SGSNSMORecord_chargingCharacteristics_present 0x0040
#       define      CDR3G_M_SGSNSMORecord_systemType_present 0x0020
#       define      CDR3G_M_destinationNumber_present 0x0010
#       define      CDR3G_M_cAMELInformationSMS_present 0x0008
    CDR3G_M_CallEventRecordType recordType;
    CDR3G_M_IMSI    servedIMSI;
    CDR3G_M_IMEI    servedIMEI;  /* optional; set in bit_mask
                                  * CDR3G_M_SGSNSMORecord_servedIMEI_present if
                                  * present */
    CDR3G_M_MSISDN  servedMSISDN;  /* optional; set in bit_mask
                                    * CDR3G_M_SGSNSMORecord_servedMSISDN_present
                                    * if present */
    CDR3G_M_MSNetworkCapability msNetworkCapability;
    CDR3G_M_AddressString serviceCentre;
    CDR3G_M_RecordingEntity recordingEntity;
    CDR3G_M_LocationAreaCode locationArea;  /* optional; set in bit_mask
                                * CDR3G_M_SGSNSMORecord_locationArea_present if
                                * present */
    CDR3G_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                 * CDR3G_M_SGSNSMORecord_routingArea_present if
                                 * present */
    CDR3G_M_CellId  cellIdentifier;  /* optional; set in bit_mask
                              * CDR3G_M_SGSNSMORecord_cellIdentifier_present if
                              * present */
    CDR3G_M_MessageReference messageReference;
    CDR3G_M_TimeStamp originationTime;
    CDR3G_M_SMSResult smsResult;  /* optional; set in bit_mask
                                   * CDR3G_M_SGSNSMORecord_smsResult_present if
                                   * present */
    CDR3G_M_ManagementExtensions recordExtensions;  /* optional; set in bit_mask
                            * CDR3G_M_SGSNSMORecord_recordExtensions_present if
                            * present */
    CDR3G_M_NodeID  nodeID;  /* optional; set in bit_mask
                              * CDR3G_M_SGSNSMORecord_nodeID_present if
                              * present */
    CDR3G_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                         * CDR3G_M_SGSNSMORecord_localSequenceNumber_present if
                         * present */
    CDR3G_M_ChargingCharacteristics chargingCharacteristics;  /* optional; set
                                   * in bit_mask
                     * CDR3G_M_SGSNSMORecord_chargingCharacteristics_present if
                     * present */
    CDR3G_M_SystemType systemType;  /* optional; set in bit_mask
                                     * CDR3G_M_SGSNSMORecord_systemType_present
                                     * if present */
    CDR3G_M_CalledNumber destinationNumber;  /* optional; set in bit_mask
                                         * CDR3G_M_destinationNumber_present if
                                         * present */
    CDR3G_M_CAMELInformationSMS cAMELInformationSMS;  /* optional; set in
                                   * bit_mask
                                   * CDR3G_M_cAMELInformationSMS_present if
                                   * present */
} CDR3G_M_SGSNSMORecord;

typedef struct CDR3G_M_SGSNSMTRecord {
    unsigned short  bit_mask;
#       define      CDR3G_M_SGSNSMTRecord_servedIMEI_present 0x8000
#       define      CDR3G_M_SGSNSMTRecord_servedMSISDN_present 0x4000
#       define      CDR3G_M_SGSNSMTRecord_locationArea_present 0x2000
#       define      CDR3G_M_SGSNSMTRecord_routingArea_present 0x1000
#       define      CDR3G_M_SGSNSMTRecord_cellIdentifier_present 0x0800
#       define      CDR3G_M_SGSNSMTRecord_smsResult_present 0x0400
#       define      CDR3G_M_SGSNSMTRecord_recordExtensions_present 0x0200
#       define      CDR3G_M_SGSNSMTRecord_nodeID_present 0x0100
#       define      CDR3G_M_SGSNSMTRecord_localSequenceNumber_present 0x0080
#       define      CDR3G_M_SGSNSMTRecord_chargingCharacteristics_present 0x0040
#       define      CDR3G_M_SGSNSMTRecord_systemType_present 0x0020
    CDR3G_M_CallEventRecordType recordType;
    CDR3G_M_IMSI    servedIMSI;
    CDR3G_M_IMEI    servedIMEI;  /* optional; set in bit_mask
                                  * CDR3G_M_SGSNSMTRecord_servedIMEI_present if
                                  * present */
    CDR3G_M_MSISDN  servedMSISDN;  /* optional; set in bit_mask
                                    * CDR3G_M_SGSNSMTRecord_servedMSISDN_present
                                    * if present */
    CDR3G_M_MSNetworkCapability msNetworkCapability;
    CDR3G_M_AddressString serviceCentre;
    CDR3G_M_RecordingEntity recordingEntity;
    CDR3G_M_LocationAreaCode locationArea;  /* optional; set in bit_mask
                                * CDR3G_M_SGSNSMTRecord_locationArea_present if
                                * present */
    CDR3G_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                 * CDR3G_M_SGSNSMTRecord_routingArea_present if
                                 * present */
    CDR3G_M_CellId  cellIdentifier;  /* optional; set in bit_mask
                              * CDR3G_M_SGSNSMTRecord_cellIdentifier_present if
                              * present */
    CDR3G_M_TimeStamp originationTime;
    CDR3G_M_SMSResult smsResult;  /* optional; set in bit_mask
                                   * CDR3G_M_SGSNSMTRecord_smsResult_present if
                                   * present */
    CDR3G_M_ManagementExtensions recordExtensions;  /* optional; set in bit_mask
                            * CDR3G_M_SGSNSMTRecord_recordExtensions_present if
                            * present */
    CDR3G_M_NodeID  nodeID;  /* optional; set in bit_mask
                              * CDR3G_M_SGSNSMTRecord_nodeID_present if
                              * present */
    CDR3G_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                         * CDR3G_M_SGSNSMTRecord_localSequenceNumber_present if
                         * present */
    CDR3G_M_ChargingCharacteristics chargingCharacteristics;  /* optional; set
                                   * in bit_mask
                     * CDR3G_M_SGSNSMTRecord_chargingCharacteristics_present if
                     * present */
    CDR3G_M_SystemType systemType;  /* optional; set in bit_mask
                                     * CDR3G_M_SGSNSMTRecord_systemType_present
                                     * if present */
} CDR3G_M_SGSNSMTRecord;

typedef struct CDR3G_M_CallEventRecord {
    unsigned short  choice;
#       define      CDR3G_M_sgsnPDPRecord_chosen 1
#       define      CDR3G_M_ggsnPDPRecord_chosen 2
#       define      CDR3G_M_sgsnMMRecord_chosen 3
#       define      CDR3G_M_sgsnSMORecord_chosen 4
#       define      CDR3G_M_sgsnSMTRecord_chosen 5
    union _union {
        CDR3G_M_SGSNPDPRecord sgsnPDPRecord;  /* to choose, set choice to
                                              * CDR3G_M_sgsnPDPRecord_chosen */
        CDR3G_M_GGSNPDPRecord ggsnPDPRecord;  /* to choose, set choice to
                                              * CDR3G_M_ggsnPDPRecord_chosen */
        CDR3G_M_SGSNMMRecord sgsnMMRecord;  /* to choose, set choice to
                                             * CDR3G_M_sgsnMMRecord_chosen */
        CDR3G_M_SGSNSMORecord sgsnSMORecord;  /* to choose, set choice to
                                              * CDR3G_M_sgsnSMORecord_chosen */
        CDR3G_M_SGSNSMTRecord sgsnSMTRecord;  /* to choose, set choice to
                                              * CDR3G_M_sgsnSMTRecord_chosen */
    } u;
} CDR3G_M_CallEventRecord;

typedef enum CDR3G_M_ChangeCondition {
    CDR3G_M_qoSChange = 0,
    CDR3G_M_tariffTime = 1,
    CDR3G_M_recordClosure = 2
} CDR3G_M_ChangeCondition;

typedef enum CDR3G_M_QoSReliability {
    CDR3G_M_unspecifiedReliability = 0,
    CDR3G_M_acknowledgedGTP = 1,
    CDR3G_M_unackGTPAcknowLLC = 2,
    CDR3G_M_unackGTPLLCAcknowRLC = 3,
    CDR3G_M_unackGTPLLCRLC = 4,
    CDR3G_M_unacknowUnprotectedData = 5
} CDR3G_M_QoSReliability;

typedef enum CDR3G_M_QoSDelay {
    CDR3G_M_delayClass1 = 1,
    CDR3G_M_delayClass2 = 2,
    CDR3G_M_delayClass3 = 3,
    CDR3G_M_delayClass4 = 4
} CDR3G_M_QoSDelay;

typedef enum CDR3G_M_QoSPrecedence {
    CDR3G_M_QoSPrecedence_unspecified = 0,
    CDR3G_M_highPriority = 1,
    CDR3G_M_normalPriority = 2,
    CDR3G_M_lowPriority = 3
} CDR3G_M_QoSPrecedence;

typedef enum CDR3G_M_QoSPeakThroughput {
    CDR3G_M_QoSPeakThroughput_unspecified = 0,
    CDR3G_M_upTo1000octetPs = 1,
    CDR3G_M_upTo2000octetPs = 2,
    CDR3G_M_upTo4000octetPs = 3,
    CDR3G_M_upTo8000octetPs = 4,
    CDR3G_M_upTo16000octetPs = 5,
    CDR3G_M_upTo32000octetPs = 6,
    CDR3G_M_upTo64000octetPs = 7,
    CDR3G_M_upTo128000octetPs = 8,
    CDR3G_M_upTo256000octetPs = 9
} CDR3G_M_QoSPeakThroughput;

typedef enum CDR3G_M_QoSMeanThroughput {
    CDR3G_M_bestEffort = 0,
    CDR3G_M_mean100octetPh = 1,
    CDR3G_M_mean200octetPh = 2,
    CDR3G_M_mean500octetPh = 3,
    CDR3G_M_mean1000octetPh = 4,
    CDR3G_M_mean2000octetPh = 5,
    CDR3G_M_mean5000octetPh = 6,
    CDR3G_M_mean10000octetPh = 7,
    CDR3G_M_mean20000octetPh = 8,
    CDR3G_M_mean50000octetPh = 9,
    CDR3G_M_mean100000octetPh = 10,
    CDR3G_M_mean200000octetPh = 11,
    CDR3G_M_mean500000octetPh = 12,
    CDR3G_M_mean1000000octetPh = 13,
    CDR3G_M_mean2000000octetPh = 14,
    CDR3G_M_mean5000000octetPh = 15,
    CDR3G_M_mean10000000octetPh = 16,
    CDR3G_M_mean20000000octetPh = 17,
    CDR3G_M_mean50000000octetPh = 18
} CDR3G_M_QoSMeanThroughput;

typedef struct CDR3G_M_GSMQoSInformation {
    CDR3G_M_QoSReliability reliability;
    CDR3G_M_QoSDelay delay;
    CDR3G_M_QoSPrecedence precedence;
    CDR3G_M_QoSPeakThroughput peakThroughput;
    CDR3G_M_QoSMeanThroughput meanThroughput;
} CDR3G_M_GSMQoSInformation;

typedef struct CDR3G_M_QoSInformation {
    unsigned short  choice;
#       define      CDR3G_M_gsmQosInformation_chosen 1
#       define      CDR3G_M_umtsQosInformation_chosen 2
    union _union {
        CDR3G_M_GSMQoSInformation gsmQosInformation;  /* to choose, set choice
                                       * to CDR3G_M_gsmQosInformation_chosen */
        struct CDR3G_M__octet8 {
            unsigned short  length;
            unsigned char   value[12];
        } umtsQosInformation;  /* to choose, set choice to
                                * CDR3G_M_umtsQosInformation_chosen */
    } u;
} CDR3G_M_QoSInformation;

typedef struct CDR3G_M_ChangeOfCharCondition {
    unsigned char   bit_mask;
#       define      CDR3G_M_qosRequested_present 0x80
#       define      CDR3G_M_qosNegotiated_present 0x40
    CDR3G_M_QoSInformation qosRequested;  /* optional; set in bit_mask
                                           * CDR3G_M_qosRequested_present if
                                           * present */
    CDR3G_M_QoSInformation qosNegotiated;  /* optional; set in bit_mask
                                            * CDR3G_M_qosNegotiated_present if
                                            * present */
    CDR3G_M_DataVolumeGPRS dataVolumeGPRSUplink;
    CDR3G_M_DataVolumeGPRS dataVolumeGPRSDownlink;
    CDR3G_M_ChangeCondition changeCondition;
    CDR3G_M_TimeStamp changeTime;
} CDR3G_M_ChangeOfCharCondition;

typedef struct CDR3G_M_ChangeLocation {
    unsigned char   bit_mask;
#       define      CDR3G_M_cellId_present 0x80
    CDR3G_M_LocationAreaCode locationAreaCode;
    CDR3G_M_RoutingAreaCode routingAreaCode;
    CDR3G_M_CellId  cellId;  /* optional; set in bit_mask CDR3G_M_cellId_present
                              * if present */
    CDR3G_M_TimeStamp changeTime;
} CDR3G_M_ChangeLocation;

extern CDR3G_M__ObjectID CDR3G_M_gsm1205InformationModel;

extern CDR3G_M__ObjectID CDR3G_M_gsm1205ASN1Module;

extern int CDR3G_M_maxAddressLength;

extern int CDR3G_M_maxISDN_AddressLength;


extern void *CDRF_3GCDR_M;    /* encoder-decoder control table */
#ifdef __cplusplus
}	/* extern "C" */
#endif /* __cplusplus */
#endif /* OSS_CDRF_3GCDR_M */
