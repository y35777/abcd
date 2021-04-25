/********************************************************/
/* Copyright (C) 2003 OSS Nokalva.  All rights reserved.*/
/********************************************************/

/* THIS FILE IS PROPRIETARY MATERIAL OF OSS NOKALVA
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF OSS NOKALVA.
 * THIS FILE MAY NOT BE DISTRIBUTED. */

/* Generated for: HuaWei Technologies, Inc., Shenzhen, China - license 8088 for Windows NT */
/* Abstract syntax: CDRF_3GCDR_Final_M */
/* Created: Fri Apr 25 09:46:47 2003 */
/* ASN.1 compiler version: 5.3 */
/* Target operating system: Solaris 2.3 or later */
/* Target machine type: SPARC */
/* C compiler options required: None */
/* ASN.1 compiler options and file names specified:
 * -codefile CDRF_3GCDR_CG_M.c -headerfile CDRF_3GCDR_CG_M.h -errorfile
 * C:\WINDOWS\TEMP\2. -prefix _3GCDR_M_ -nodebug -dualheader -ansi -verbose
 * -ber -root D:\application\ossasn1\solaris\asn1dflt.solaris-sparc
 * D:\project\530\asn1app\maimaoshi\3GCDR_Final_M\CDRF_3GCDR_Final_M.asn
 */

#ifndef OSS_CDRF_3GCDR_Final_M
#define OSS_CDRF_3GCDR_Final_M

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "asn1hdr.h"
#include "asn1code.h"

#define          _3GCDR_M_BSRecord_PDU 1

typedef struct _3GCDR_M__ObjectID {
    unsigned short  length;
    unsigned char   *value;
} _3GCDR_M__ObjectID;

typedef struct _3GCDR_M__Any {
    unsigned long   length;
    unsigned char   *value;
} _3GCDR_M__Any;

typedef struct _3GCDR_M__octet1 {
    unsigned short  length;
    unsigned char   value[15];
} _3GCDR_M__octet1;

typedef struct _3GCDR_M__octet3 {
    unsigned short  length;
    unsigned char   value[14];
} _3GCDR_M__octet3;

typedef struct _3GCDR_M__octet4 {
    unsigned short  length;
    unsigned char   value[10];
} _3GCDR_M__octet4;

typedef struct _3GCDR_M_BSXferSummaryRecord {
    _3GCDR_M__octet1 cgnID;
    struct _3GCDR_M__octet2 {
        unsigned short  length;
        unsigned char   value[34];
    } bsXferFile;
    _3GCDR_M__octet3 openTime;
    _3GCDR_M__octet3 closeTime;
    _3GCDR_M__octet4 lrsnStart;
    _3GCDR_M__octet4 lrsnEnd;
    struct _3GCDR_M__octet5 {
        unsigned short  length;
        unsigned char   value[3];
    } cdrType;
    _3GCDR_M__octet4 totalCdr;
    _3GCDR_M__octet4 missingCdr;
    _3GCDR_M__octet4 duplicateCdr;
    _3GCDR_M__octet1 cgID;
} _3GCDR_M_BSXferSummaryRecord;

typedef struct _3GCDR_M_BSRecord {
    _3GCDR_M_BSXferSummaryRecord bsXferRecord;
    struct _3GCDR_M__seqof1 {
        unsigned int    count;
        struct _3GCDR_M_CallEventRecord *value;
    } listOfCDR;
} _3GCDR_M_BSRecord;

typedef int             _3GCDR_M_CallEventRecordType;
#define                     _3GCDR_M_CallEventRecordType_sgsnPDPRecord 18
#define                     _3GCDR_M_CallEventRecordType_ggsnPDPRecord 19
#define                     _3GCDR_M_CallEventRecordType_sgsnMMRecord 20
#define                     _3GCDR_M_CallEventRecordType_sgsnSMORecord 21
#define                     _3GCDR_M_CallEventRecordType_sgsnSMTRecord 22

typedef ossBoolean      _3GCDR_M_NetworkInitiatedPDPContext;

typedef struct _3GCDR_M_IMSI {
    unsigned short  length;
    unsigned char   value[8];
} _3GCDR_M_IMSI;

typedef struct _3GCDR_M_IMEI {
    unsigned short  length;
    unsigned char   value[8];
} _3GCDR_M_IMEI;

typedef struct _3GCDR_M_IPBinaryAddress {
    unsigned short  choice;
#       define      _3GCDR_M_iPBinV4Address_chosen 1
#       define      _3GCDR_M_iPBinV6Address_chosen 2
    union _union {
        struct _3GCDR_M__octet6 {
            unsigned short  length;
            unsigned char   value[4];
        } iPBinV4Address;  /* to choose, set choice to
                            * _3GCDR_M_iPBinV4Address_chosen */
        struct _3GCDR_M__octet7 {
            unsigned short  length;
            unsigned char   value[16];
        } iPBinV6Address;  /* to choose, set choice to
                            * _3GCDR_M_iPBinV6Address_chosen */
    } u;
} _3GCDR_M_IPBinaryAddress;

typedef struct _3GCDR_M_IPTextRepresentedAddress {
    unsigned short  choice;
#       define      _3GCDR_M_iPTextV4Address_chosen 1
#       define      _3GCDR_M_iPTextV6Address_chosen 2
    union _union {
        char            iPTextV4Address[16];  /* to choose, set choice to
                                           * _3GCDR_M_iPTextV4Address_chosen */
        char            iPTextV6Address[46];  /* to choose, set choice to
                                           * _3GCDR_M_iPTextV6Address_chosen */
    } u;
} _3GCDR_M_IPTextRepresentedAddress;

typedef struct _3GCDR_M_IPAddress {
    unsigned short  choice;
#       define      _3GCDR_M_iPBinaryAddress_chosen 1
#       define      _3GCDR_M_iPTextRepresentedAddress_chosen 2
    union _union {
        _3GCDR_M_IPBinaryAddress iPBinaryAddress;  /* to choose, set choice to
                                           * _3GCDR_M_iPBinaryAddress_chosen */
        _3GCDR_M_IPTextRepresentedAddress iPTextRepresentedAddress;  /* to
                                   * choose, set choice to
                                  * _3GCDR_M_iPTextRepresentedAddress_chosen */
    } u;
} _3GCDR_M_IPAddress;

typedef _3GCDR_M_IPAddress _3GCDR_M_GSNAddress;

typedef struct _3GCDR_M_MSNetworkCapability {
    unsigned short  length;
    unsigned char   value[1];
} _3GCDR_M_MSNetworkCapability;

typedef struct _3GCDR_M_RoutingAreaCode {
    unsigned short  length;
    unsigned char   value[1];
} _3GCDR_M_RoutingAreaCode;

typedef struct _3GCDR_M_LocationAreaCode {
    unsigned short  length;
    unsigned char   value[2];
} _3GCDR_M_LocationAreaCode;

typedef struct _3GCDR_M_CellId {
    unsigned short  length;
    unsigned char   value[2];
} _3GCDR_M_CellId;

typedef unsigned int    _3GCDR_M_ChargingID;

typedef char            _3GCDR_M_AccessPointNameNI[64];

typedef struct _3GCDR_M_PDPType {
    unsigned short  length;
    unsigned char   value[2];
} _3GCDR_M_PDPType;

typedef struct _3GCDR_M_AddressString {
    unsigned short  length;
    unsigned char   value[20];
} _3GCDR_M_AddressString;

typedef _3GCDR_M_AddressString _3GCDR_M_ETSIAddress;

typedef struct _3GCDR_M_PDPAddress {
    unsigned short  choice;
#       define      _3GCDR_M_iPAddress_chosen 1
#       define      _3GCDR_M_eTSIAddress_chosen 2
    union _union {
        _3GCDR_M_IPAddress iPAddress;  /* to choose, set choice to
                                        * _3GCDR_M_iPAddress_chosen */
        _3GCDR_M_ETSIAddress eTSIAddress;  /* to choose, set choice to
                                            * _3GCDR_M_eTSIAddress_chosen */
    } u;
} _3GCDR_M_PDPAddress;

typedef struct _3GCDR_M_TimeStamp {
    unsigned short  length;
    unsigned char   value[9];
} _3GCDR_M_TimeStamp;

typedef int             _3GCDR_M_CallDuration;

typedef ossBoolean      _3GCDR_M_SGSNChange;

typedef int             _3GCDR_M_CauseForRecClosing;
#define                     _3GCDR_M_normalRelease 0
#define                     _3GCDR_M_abnormalRelease 4
#define                     _3GCDR_M_cAMELInitCallRelease 5
#define                     _3GCDR_M_volumeLimit 16
#define                     _3GCDR_M_timeLimit 17
#define                     _3GCDR_M_sGSNChange 18
#define                     _3GCDR_M_maxChangeCond 19
#define                     _3GCDR_M_managementIntervention 20

typedef struct _3GCDR_M_ManagementExtension {
    unsigned char   bit_mask;
#       define      _3GCDR_M_significance_present 0x80
    _3GCDR_M__ObjectID identifier;
    ossBoolean      significance;  /* _3GCDR_M_significance_present not set in
                                    * bit_mask implies value is FALSE */
    _3GCDR_M__Any   information;
} _3GCDR_M_ManagementExtension;

typedef struct _3GCDR_M_Diagnostics {
    unsigned short  choice;
#       define      _3GCDR_M_gsm0408Cause_chosen 1
#       define      _3GCDR_M_gsm0902MapErrorValue_chosen 2
#       define      _3GCDR_M_ccittQ767Cause_chosen 3
#       define      _3GCDR_M_networkSpecificCause_chosen 4
#       define      _3GCDR_M_manufacturerSpecificCause_chosen 5
    union _union {
        int             gsm0408Cause;  /* to choose, set choice to
                                        * _3GCDR_M_gsm0408Cause_chosen */
        int             gsm0902MapErrorValue;  /* to choose, set choice to
                                      * _3GCDR_M_gsm0902MapErrorValue_chosen */
        int             ccittQ767Cause;  /* to choose, set choice to
                                          * _3GCDR_M_ccittQ767Cause_chosen */
        _3GCDR_M_ManagementExtension networkSpecificCause;  /* to choose, set
                                   * choice to
                                   * _3GCDR_M_networkSpecificCause_chosen */
        _3GCDR_M_ManagementExtension manufacturerSpecificCause;  /* to choose,
                                   * set choice to
                                 * _3GCDR_M_manufacturerSpecificCause_chosen */
    } u;
} _3GCDR_M_Diagnostics;

struct _3GCDR_M__seqof2 {
    unsigned int    count;
    int             *value;
};
    
typedef struct _3GCDR_M_SequenceList {
    unsigned char   bit_mask;
#       define      _3GCDR_M_recSequenceNumber_present 0x80
    _3GCDR_M_GSNAddress sgsnAddress;
    struct _3GCDR_M__seqof2 recSequenceNumber;  /* optional; set in bit_mask
                           * _3GCDR_M_recSequenceNumber_present if present */
} _3GCDR_M_SequenceList;

typedef char            _3GCDR_M_NodeID[21];

typedef struct _3GCDR_M_ManagementExtensions {
    unsigned int    count;
    _3GCDR_M_ManagementExtension *value;
} _3GCDR_M_ManagementExtensions;

typedef enum _3GCDR_M_APNSelectionMode {
    _3GCDR_M_mSorNetworkProvidedSubscriptionVerified = 0,
    _3GCDR_M_mSProvidedSubscriptionNotVerified = 1,
    _3GCDR_M_networkProvidedSubscriptionNotVerified = 2
} _3GCDR_M_APNSelectionMode;

typedef char            _3GCDR_M_AccessPointNameOI[38];

typedef struct _3GCDR_M_ISDN_AddressString {
    unsigned short  length;
    unsigned char   value[9];
} _3GCDR_M_ISDN_AddressString;

typedef _3GCDR_M_ISDN_AddressString _3GCDR_M_MSISDN;

typedef struct _3GCDR_M_ChargingCharacteristics {
    unsigned short  length;
    unsigned char   value[2];
} _3GCDR_M_ChargingCharacteristics;

typedef enum _3GCDR_M_SystemType {
    _3GCDR_M_unknown = 0,
    _3GCDR_M_iuUTRAN = 1,
    _3GCDR_M_gERAN = 2
} _3GCDR_M_SystemType;

typedef _3GCDR_M_AddressString _3GCDR_M_SCFAddress;

typedef unsigned int    _3GCDR_M_ServiceKey;

typedef enum _3GCDR_M_DefaultGPRS_Handling {
    _3GCDR_M_DefaultGPRS_Handling_continueTransaction = 0,
    _3GCDR_M_DefaultGPRS_Handling_releaseTransaction = 1
} _3GCDR_M_DefaultGPRS_Handling;

typedef _3GCDR_M_AccessPointNameNI _3GCDR_M_CAMELAccessPointNameNI;

typedef _3GCDR_M_AccessPointNameOI _3GCDR_M_CAMELAccessPointNameOI;

typedef int             _3GCDR_M_NumberOfDPEncountered;

typedef unsigned char   _3GCDR_M_LevelOfCAMELService;
#define                     _3GCDR_M_basic 0x80
#define                     _3GCDR_M_callDurationSupervision 0x40
#define                     _3GCDR_M_onlineCharging 0x20

typedef struct _3GCDR_M_FreeFormatData {
    unsigned short  length;
    unsigned char   value[160];
} _3GCDR_M_FreeFormatData;

typedef ossBoolean      _3GCDR_M_FFDAppendIndicator;

typedef struct _3GCDR_M_CAMELInformationPDP {
    unsigned short  bit_mask;
#       define      _3GCDR_M_CAMELInformationPDP_sCFAddress_present 0x8000
#       define      _3GCDR_M_CAMELInformationPDP_serviceKey_present 0x4000
#       define      _3GCDR_M_CAMELInformationPDP_defaultTransactionHandling_present 0x2000
#       define      _3GCDR_M_cAMELAccessPointNameNI_present 0x1000
#       define      _3GCDR_M_cAMELAccessPointNameOI_present 0x0800
#       define      _3GCDR_M_CAMELInformationPDP_numberOfDPEncountered_present 0x0400
#       define      _3GCDR_M_CAMELInformationPDP_levelOfCAMELService_present 0x0200
#       define      _3GCDR_M_CAMELInformationPDP_freeFormatData_present 0x0100
#       define      _3GCDR_M_CAMELInformationPDP_fFDAppendIndicator_present 0x0080
    _3GCDR_M_SCFAddress sCFAddress;  /* optional; set in bit_mask
                           * _3GCDR_M_CAMELInformationPDP_sCFAddress_present if
                           * present */
    _3GCDR_M_ServiceKey serviceKey;  /* optional; set in bit_mask
                           * _3GCDR_M_CAMELInformationPDP_serviceKey_present if
                           * present */
    _3GCDR_M_DefaultGPRS_Handling defaultTransactionHandling;  /* optional; set
                                   * in bit_mask
           * _3GCDR_M_CAMELInformationPDP_defaultTransactionHandling_present if
           * present */
    _3GCDR_M_CAMELAccessPointNameNI cAMELAccessPointNameNI;  /* optional; set in
                                   * bit_mask
                                   * _3GCDR_M_cAMELAccessPointNameNI_present if
                                   * present */
    _3GCDR_M_CAMELAccessPointNameOI cAMELAccessPointNameOI;  /* optional; set in
                                   * bit_mask
                                   * _3GCDR_M_cAMELAccessPointNameOI_present if
                                   * present */
    _3GCDR_M_NumberOfDPEncountered numberOfDPEncountered;  /* optional; set in
                                   * bit_mask
                * _3GCDR_M_CAMELInformationPDP_numberOfDPEncountered_present if
                * present */
    _3GCDR_M_LevelOfCAMELService levelOfCAMELService;  /* optional; set in
                                   * bit_mask
                  * _3GCDR_M_CAMELInformationPDP_levelOfCAMELService_present if
                  * present */
    _3GCDR_M_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                       * _3GCDR_M_CAMELInformationPDP_freeFormatData_present if
                       * present */
    _3GCDR_M_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in
                                   * bit_mask
                   * _3GCDR_M_CAMELInformationPDP_fFDAppendIndicator_present if
                   * present */
} _3GCDR_M_CAMELInformationPDP;

typedef int             _3GCDR_M_DataVolumeGPRS;

typedef enum _3GCDR_M_ConsolidationResult {
    _3GCDR_M_consolidationResult_normal = 0,
    _3GCDR_M_consolidationResult_notnormal = 1,
    _3GCDR_M_consolidationResult_ForInterSGSNConsolidation = 2,
    _3GCDR_M_consolidationResult_ReachLimit = 3,
    _3GCDR_M_consolidationResult_onlyOneCDRGenerated = 4
} _3GCDR_M_ConsolidationResult;

typedef struct _3GCDR_M__seqof3 {
    unsigned int    count;
    struct _3GCDR_M_ChangeOfCharCondition *value;
} _3GCDR_M__seqof3;

typedef struct _3GCDR_M_SGSNPDPRecord {
    unsigned int    bit_mask;
#       define      _3GCDR_M_SGSNPDPRecord_networkInitiation_present 0x80000000
#       define      _3GCDR_M_SGSNPDPRecord_servedIMEI_present 0x40000000
#       define      _3GCDR_M_SGSNPDPRecord_msNetworkCapability_present 0x20000000
#       define      _3GCDR_M_SGSNPDPRecord_routingArea_present 0x10000000
#       define      _3GCDR_M_SGSNPDPRecord_locationAreaCode_present 0x08000000
#       define      _3GCDR_M_SGSNPDPRecord_cellIdentifier_present 0x04000000
#       define      _3GCDR_M_SGSNPDPRecord_servedPDPAddress_present 0x02000000
#       define      _3GCDR_M_SGSNPDPRecord_sgsnChange_present 0x01000000
#       define      _3GCDR_M_SGSNPDPRecord_diagnostics_present 0x00800000
#       define      _3GCDR_M_SGSNPDPRecord_recSequenceNumList_present 0x00400000
#       define      _3GCDR_M_SGSNPDPRecord_nodeID_present 0x00200000
#       define      _3GCDR_M_SGSNPDPRecord_recordExtensions_present 0x00100000
#       define      _3GCDR_M_SGSNPDPRecord_apnSelectionMode_present 0x00080000
#       define      _3GCDR_M_SGSNPDPRecord_servedMSISDN_present 0x00040000
#       define      _3GCDR_M_SGSNPDPRecord_chargingCharacteristics_present 0x00020000
#       define      _3GCDR_M_SGSNPDPRecord_systemType_present 0x00010000
#       define      _3GCDR_M_cAMELInformationPDP_present 0x00008000
#       define      _3GCDR_M_rNCUnsentDownlinkVolume_present 0x00004000
    _3GCDR_M_CallEventRecordType recordType;
    _3GCDR_M_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                          * _3GCDR_M_SGSNPDPRecord_networkInitiation_present if
                          * present */
    _3GCDR_M_IMSI   servedIMSI;
    _3GCDR_M_IMEI   servedIMEI;  /* optional; set in bit_mask
                                  * _3GCDR_M_SGSNPDPRecord_servedIMEI_present if
                                  * present */
    _3GCDR_M_GSNAddress sgsnAddress;
    _3GCDR_M_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                        * _3GCDR_M_SGSNPDPRecord_msNetworkCapability_present if
                        * present */
    _3GCDR_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                * _3GCDR_M_SGSNPDPRecord_routingArea_present if
                                * present */
    _3GCDR_M_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                           * _3GCDR_M_SGSNPDPRecord_locationAreaCode_present if
                           * present */
    _3GCDR_M_CellId cellIdentifier;  /* optional; set in bit_mask
                             * _3GCDR_M_SGSNPDPRecord_cellIdentifier_present if
                             * present */
    _3GCDR_M_ChargingID chargingID;
    _3GCDR_M_GSNAddress ggsnAddressUsed;
    _3GCDR_M_AccessPointNameNI accessPointNameNI;
    _3GCDR_M_PDPType pdpType;
    _3GCDR_M_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                           * _3GCDR_M_SGSNPDPRecord_servedPDPAddress_present if
                           * present */
    _3GCDR_M__seqof3 listOfTrafficVolumes;
    _3GCDR_M_TimeStamp recordOpeningTime;
    _3GCDR_M_CallDuration duration;
    _3GCDR_M_SGSNChange sgsnChange;  /* optional; set in bit_mask
                                 * _3GCDR_M_SGSNPDPRecord_sgsnChange_present if
                                 * present */
    _3GCDR_M_CauseForRecClosing causeForRecClosing;
    _3GCDR_M_Diagnostics diagnostics;  /* optional; set in bit_mask
                                * _3GCDR_M_SGSNPDPRecord_diagnostics_present if
                                * present */
    _3GCDR_M_SequenceList recSequenceNumList;  /* optional; set in bit_mask
                         * _3GCDR_M_SGSNPDPRecord_recSequenceNumList_present if
                         * present */
    _3GCDR_M_NodeID nodeID;  /* optional; set in bit_mask
                              * _3GCDR_M_SGSNPDPRecord_nodeID_present if
                              * present */
    _3GCDR_M_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                           * _3GCDR_M_SGSNPDPRecord_recordExtensions_present if
                           * present */
    _3GCDR_M_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                           * _3GCDR_M_SGSNPDPRecord_apnSelectionMode_present if
                           * present */
    _3GCDR_M_AccessPointNameOI accessPointNameOI;
    _3GCDR_M_MSISDN servedMSISDN;  /* optional; set in bit_mask
                               * _3GCDR_M_SGSNPDPRecord_servedMSISDN_present if
                               * present */
    _3GCDR_M_ChargingCharacteristics chargingCharacteristics;  /* optional; set
                                   * in bit_mask
                    * _3GCDR_M_SGSNPDPRecord_chargingCharacteristics_present if
                    * present */
    _3GCDR_M_SystemType systemType;  /* optional; set in bit_mask
                                 * _3GCDR_M_SGSNPDPRecord_systemType_present if
                                 * present */
    _3GCDR_M_CAMELInformationPDP cAMELInformationPDP;  /* optional; set in
                                   * bit_mask
                                   * _3GCDR_M_cAMELInformationPDP_present if
                                   * present */
    _3GCDR_M_DataVolumeGPRS rNCUnsentDownlinkVolume;  /* optional; set in
                                   * bit_mask
                                   * _3GCDR_M_rNCUnsentDownlinkVolume_present if
                                   * present */
    _3GCDR_M_ConsolidationResult consolidationResult;
} _3GCDR_M_SGSNPDPRecord;

typedef ossBoolean      _3GCDR_M_DynamicAddressFlag;

typedef struct _3GCDR_M_GGSNPDPRecord {
    unsigned short  bit_mask;
#       define      _3GCDR_M_GGSNPDPRecord_networkInitiation_present 0x8000
#       define      _3GCDR_M_GGSNPDPRecord_servedPDPAddress_present 0x4000
#       define      _3GCDR_M_dynamicAddressFlag_present 0x2000
#       define      _3GCDR_M_GGSNPDPRecord_diagnostics_present 0x1000
#       define      _3GCDR_M_GGSNPDPRecord_recSequenceNumList_present 0x0800
#       define      _3GCDR_M_GGSNPDPRecord_nodeID_present 0x0400
#       define      _3GCDR_M_GGSNPDPRecord_recordExtensions_present 0x0200
#       define      _3GCDR_M_GGSNPDPRecord_apnSelectionMode_present 0x0100
#       define      _3GCDR_M_GGSNPDPRecord_servedMSISDN_present 0x0080
#       define      _3GCDR_M_GGSNPDPRecord_chargingCharacteristics_present 0x0040
    _3GCDR_M_CallEventRecordType recordType;
    _3GCDR_M_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                          * _3GCDR_M_GGSNPDPRecord_networkInitiation_present if
                          * present */
    _3GCDR_M_IMSI   servedIMSI;
    _3GCDR_M_GSNAddress ggsnAddress;
    _3GCDR_M_ChargingID chargingID;
    struct _3GCDR_M__seqof4 {
        unsigned int    count;
        _3GCDR_M_GSNAddress *value;
    } sgsnAddress;
    _3GCDR_M_AccessPointNameNI accessPointNameNI;
    _3GCDR_M_PDPType pdpType;
    _3GCDR_M_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                           * _3GCDR_M_GGSNPDPRecord_servedPDPAddress_present if
                           * present */
    _3GCDR_M_DynamicAddressFlag dynamicAddressFlag;  /* optional; set in
                                   * bit_mask
                                   * _3GCDR_M_dynamicAddressFlag_present if
                                   * present */
    _3GCDR_M__seqof3 listOfTrafficVolumes;
    _3GCDR_M_TimeStamp recordOpeningTime;
    _3GCDR_M_CallDuration duration;
    _3GCDR_M_CauseForRecClosing causeForRecClosing;
    _3GCDR_M_Diagnostics diagnostics;  /* optional; set in bit_mask
                                * _3GCDR_M_GGSNPDPRecord_diagnostics_present if
                                * present */
    _3GCDR_M_SequenceList recSequenceNumList;  /* optional; set in bit_mask
                         * _3GCDR_M_GGSNPDPRecord_recSequenceNumList_present if
                         * present */
    _3GCDR_M_NodeID nodeID;  /* optional; set in bit_mask
                              * _3GCDR_M_GGSNPDPRecord_nodeID_present if
                              * present */
    _3GCDR_M_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                           * _3GCDR_M_GGSNPDPRecord_recordExtensions_present if
                           * present */
    _3GCDR_M_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                           * _3GCDR_M_GGSNPDPRecord_apnSelectionMode_present if
                           * present */
    _3GCDR_M_MSISDN servedMSISDN;  /* optional; set in bit_mask
                               * _3GCDR_M_GGSNPDPRecord_servedMSISDN_present if
                               * present */
    _3GCDR_M_ChargingCharacteristics chargingCharacteristics;  /* optional; set
                                   * in bit_mask
                    * _3GCDR_M_GGSNPDPRecord_chargingCharacteristics_present if
                    * present */
    _3GCDR_M_ConsolidationResult consolidationResult;
} _3GCDR_M_GGSNPDPRecord;

typedef unsigned int    _3GCDR_M_LocalSequenceNumber;

typedef struct _3GCDR_M_CAMELInformationMM {
    unsigned char   bit_mask;
#       define      _3GCDR_M_CAMELInformationMM_sCFAddress_present 0x80
#       define      _3GCDR_M_CAMELInformationMM_serviceKey_present 0x40
#       define      _3GCDR_M_CAMELInformationMM_defaultTransactionHandling_present 0x20
#       define      _3GCDR_M_CAMELInformationMM_numberOfDPEncountered_present 0x10
#       define      _3GCDR_M_CAMELInformationMM_levelOfCAMELService_present 0x08
#       define      _3GCDR_M_CAMELInformationMM_freeFormatData_present 0x04
#       define      _3GCDR_M_CAMELInformationMM_fFDAppendIndicator_present 0x02
    _3GCDR_M_SCFAddress sCFAddress;  /* optional; set in bit_mask
                            * _3GCDR_M_CAMELInformationMM_sCFAddress_present if
                            * present */
    _3GCDR_M_ServiceKey serviceKey;  /* optional; set in bit_mask
                            * _3GCDR_M_CAMELInformationMM_serviceKey_present if
                            * present */
    _3GCDR_M_DefaultGPRS_Handling defaultTransactionHandling;  /* optional; set
                                   * in bit_mask
            * _3GCDR_M_CAMELInformationMM_defaultTransactionHandling_present if
            * present */
    _3GCDR_M_NumberOfDPEncountered numberOfDPEncountered;  /* optional; set in
                                   * bit_mask
                 * _3GCDR_M_CAMELInformationMM_numberOfDPEncountered_present if
                 * present */
    _3GCDR_M_LevelOfCAMELService levelOfCAMELService;  /* optional; set in
                                   * bit_mask
                   * _3GCDR_M_CAMELInformationMM_levelOfCAMELService_present if
                   * present */
    _3GCDR_M_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                        * _3GCDR_M_CAMELInformationMM_freeFormatData_present if
                        * present */
    _3GCDR_M_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in
                                   * bit_mask
                    * _3GCDR_M_CAMELInformationMM_fFDAppendIndicator_present if
                    * present */
} _3GCDR_M_CAMELInformationMM;

typedef struct _3GCDR_M_SGSNMMRecord {
    unsigned int    bit_mask;
#       define      _3GCDR_M_SGSNMMRecord_servedIMEI_present 0x80000000
#       define      _3GCDR_M_SGSNMMRecord_msNetworkCapability_present 0x40000000
#       define      _3GCDR_M_SGSNMMRecord_routingArea_present 0x20000000
#       define      _3GCDR_M_SGSNMMRecord_locationAreaCode_present 0x10000000
#       define      _3GCDR_M_SGSNMMRecord_cellIdentifier_present 0x08000000
#       define      _3GCDR_M_changeLocation_present 0x04000000
#       define      _3GCDR_M_duration_present 0x02000000
#       define      _3GCDR_M_SGSNMMRecord_sgsnChange_present 0x01000000
#       define      _3GCDR_M_SGSNMMRecord_diagnostics_present 0x00800000
#       define      _3GCDR_M_recordSequenceNumber_present 0x00400000
#       define      _3GCDR_M_SGSNMMRecord_nodeID_present 0x00200000
#       define      _3GCDR_M_SGSNMMRecord_recordExtensions_present 0x00100000
#       define      _3GCDR_M_SGSNMMRecord_localSequenceNumber_present 0x00080000
#       define      _3GCDR_M_SGSNMMRecord_servedMSISDN_present 0x00040000
#       define      _3GCDR_M_SGSNMMRecord_chargingCharacteristics_present 0x00020000
#       define      _3GCDR_M_cAMELInformationMM_present 0x00010000
#       define      _3GCDR_M_SGSNMMRecord_systemType_present 0x00008000
    _3GCDR_M_CallEventRecordType recordType;
    _3GCDR_M_IMSI   servedIMSI;
    _3GCDR_M_IMEI   servedIMEI;  /* optional; set in bit_mask
                                  * _3GCDR_M_SGSNMMRecord_servedIMEI_present if
                                  * present */
    _3GCDR_M_GSNAddress sgsnAddress;
    _3GCDR_M_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                         * _3GCDR_M_SGSNMMRecord_msNetworkCapability_present if
                         * present */
    _3GCDR_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                 * _3GCDR_M_SGSNMMRecord_routingArea_present if
                                 * present */
    _3GCDR_M_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                            * _3GCDR_M_SGSNMMRecord_locationAreaCode_present if
                            * present */
    _3GCDR_M_CellId cellIdentifier;  /* optional; set in bit_mask
                              * _3GCDR_M_SGSNMMRecord_cellIdentifier_present if
                              * present */
    struct _3GCDR_M__seqof5 {
        unsigned int    count;
        struct _3GCDR_M_ChangeLocation *value;
    } changeLocation;  /* optional; set in bit_mask
                        * _3GCDR_M_changeLocation_present if present */
    _3GCDR_M_TimeStamp recordOpeningTime;
    _3GCDR_M_CallDuration duration;  /* optional; set in bit_mask
                                      * _3GCDR_M_duration_present if present */
    _3GCDR_M_SGSNChange sgsnChange;  /* optional; set in bit_mask
                                      * _3GCDR_M_SGSNMMRecord_sgsnChange_present
                                      * if present */
    _3GCDR_M_CauseForRecClosing causeForRecClosing;
    _3GCDR_M_Diagnostics diagnostics;  /* optional; set in bit_mask
                                 * _3GCDR_M_SGSNMMRecord_diagnostics_present if
                                 * present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                                     * _3GCDR_M_recordSequenceNumber_present if
                                     * present */
    _3GCDR_M_NodeID nodeID;  /* optional; set in bit_mask
                              * _3GCDR_M_SGSNMMRecord_nodeID_present if
                              * present */
    _3GCDR_M_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                            * _3GCDR_M_SGSNMMRecord_recordExtensions_present if
                            * present */
    _3GCDR_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                         * _3GCDR_M_SGSNMMRecord_localSequenceNumber_present if
                         * present */
    _3GCDR_M_MSISDN servedMSISDN;  /* optional; set in bit_mask
                                    * _3GCDR_M_SGSNMMRecord_servedMSISDN_present
                                    * if present */
    _3GCDR_M_ChargingCharacteristics chargingCharacteristics;  /* optional; set
                                   * in bit_mask
                     * _3GCDR_M_SGSNMMRecord_chargingCharacteristics_present if
                     * present */
    _3GCDR_M_CAMELInformationMM cAMELInformationMM;  /* optional; set in
                                   * bit_mask
                                   * _3GCDR_M_cAMELInformationMM_present if
                                   * present */
    _3GCDR_M_SystemType systemType;  /* optional; set in bit_mask
                                      * _3GCDR_M_SGSNMMRecord_systemType_present
                                      * if present */
} _3GCDR_M_SGSNMMRecord;

typedef _3GCDR_M_AddressString _3GCDR_M_RecordingEntity;

typedef struct _3GCDR_M_MessageReference {
    unsigned int    length;
    unsigned char   *value;
} _3GCDR_M_MessageReference;

typedef _3GCDR_M_Diagnostics _3GCDR_M_SMSResult;

typedef struct _3GCDR_M_CalledNumber {
    unsigned short  length;
    unsigned char   value[10];
} _3GCDR_M_CalledNumber;

typedef enum _3GCDR_M_DefaultSMS_Handling {
    _3GCDR_M_DefaultSMS_Handling_continueTransaction = 0,
    _3GCDR_M_DefaultSMS_Handling_releaseTransaction = 1
} _3GCDR_M_DefaultSMS_Handling;

typedef struct _3GCDR_M_CallingNumber {
    unsigned short  length;
    unsigned char   value[10];
} _3GCDR_M_CallingNumber;

typedef struct _3GCDR_M_CAMELInformationSMS {
    unsigned char   bit_mask;
#       define      _3GCDR_M_CAMELInformationSMS_sCFAddress_present 0x80
#       define      _3GCDR_M_CAMELInformationSMS_serviceKey_present 0x40
#       define      _3GCDR_M_defaultSMSHandling_present 0x20
#       define      _3GCDR_M_cAMELCallingPartyNumber_present 0x10
#       define      _3GCDR_M_cAMELDestinationSubscriberNumber_present 0x08
#       define      _3GCDR_M_cAMELSMSCAddress_present 0x04
#       define      _3GCDR_M_CAMELInformationSMS_freeFormatData_present 0x02
    _3GCDR_M_SCFAddress sCFAddress;  /* optional; set in bit_mask
                           * _3GCDR_M_CAMELInformationSMS_sCFAddress_present if
                           * present */
    _3GCDR_M_ServiceKey serviceKey;  /* optional; set in bit_mask
                           * _3GCDR_M_CAMELInformationSMS_serviceKey_present if
                           * present */
    _3GCDR_M_DefaultSMS_Handling defaultSMSHandling;  /* optional; set in
                                   * bit_mask
                                   * _3GCDR_M_defaultSMSHandling_present if
                                   * present */
    _3GCDR_M_CallingNumber cAMELCallingPartyNumber;  /* optional; set in
                                   * bit_mask
                                   * _3GCDR_M_cAMELCallingPartyNumber_present if
                                   * present */
    _3GCDR_M_CalledNumber cAMELDestinationSubscriberNumber;  /* optional; set in
                                   * bit_mask
                         * _3GCDR_M_cAMELDestinationSubscriberNumber_present if
                         * present */
    _3GCDR_M_AddressString cAMELSMSCAddress;  /* optional; set in bit_mask
                                         * _3GCDR_M_cAMELSMSCAddress_present if
                                         * present */
    _3GCDR_M_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                       * _3GCDR_M_CAMELInformationSMS_freeFormatData_present if
                       * present */
} _3GCDR_M_CAMELInformationSMS;

typedef struct _3GCDR_M_SGSNSMORecord {
    unsigned short  bit_mask;
#       define      _3GCDR_M_SGSNSMORecord_servedIMEI_present 0x8000
#       define      _3GCDR_M_SGSNSMORecord_servedMSISDN_present 0x4000
#       define      _3GCDR_M_SGSNSMORecord_locationArea_present 0x2000
#       define      _3GCDR_M_SGSNSMORecord_routingArea_present 0x1000
#       define      _3GCDR_M_SGSNSMORecord_cellIdentifier_present 0x0800
#       define      _3GCDR_M_SGSNSMORecord_smsResult_present 0x0400
#       define      _3GCDR_M_SGSNSMORecord_recordExtensions_present 0x0200
#       define      _3GCDR_M_SGSNSMORecord_nodeID_present 0x0100
#       define      _3GCDR_M_SGSNSMORecord_localSequenceNumber_present 0x0080
#       define      _3GCDR_M_SGSNSMORecord_chargingCharacteristics_present 0x0040
#       define      _3GCDR_M_SGSNSMORecord_systemType_present 0x0020
#       define      _3GCDR_M_destinationNumber_present 0x0010
#       define      _3GCDR_M_cAMELInformationSMS_present 0x0008
    _3GCDR_M_CallEventRecordType recordType;
    _3GCDR_M_IMSI   servedIMSI;
    _3GCDR_M_IMEI   servedIMEI;  /* optional; set in bit_mask
                                  * _3GCDR_M_SGSNSMORecord_servedIMEI_present if
                                  * present */
    _3GCDR_M_MSISDN servedMSISDN;  /* optional; set in bit_mask
                               * _3GCDR_M_SGSNSMORecord_servedMSISDN_present if
                               * present */
    _3GCDR_M_MSNetworkCapability msNetworkCapability;
    _3GCDR_M_AddressString serviceCentre;
    _3GCDR_M_RecordingEntity recordingEntity;
    _3GCDR_M_LocationAreaCode locationArea;  /* optional; set in bit_mask
                               * _3GCDR_M_SGSNSMORecord_locationArea_present if
                               * present */
    _3GCDR_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                * _3GCDR_M_SGSNSMORecord_routingArea_present if
                                * present */
    _3GCDR_M_CellId cellIdentifier;  /* optional; set in bit_mask
                             * _3GCDR_M_SGSNSMORecord_cellIdentifier_present if
                             * present */
    _3GCDR_M_MessageReference messageReference;
    _3GCDR_M_TimeStamp originationTime;
    _3GCDR_M_SMSResult smsResult;  /* optional; set in bit_mask
                                    * _3GCDR_M_SGSNSMORecord_smsResult_present
                                    * if present */
    _3GCDR_M_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                           * _3GCDR_M_SGSNSMORecord_recordExtensions_present if
                           * present */
    _3GCDR_M_NodeID nodeID;  /* optional; set in bit_mask
                              * _3GCDR_M_SGSNSMORecord_nodeID_present if
                              * present */
    _3GCDR_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                        * _3GCDR_M_SGSNSMORecord_localSequenceNumber_present if
                        * present */
    _3GCDR_M_ChargingCharacteristics chargingCharacteristics;  /* optional; set
                                   * in bit_mask
                    * _3GCDR_M_SGSNSMORecord_chargingCharacteristics_present if
                    * present */
    _3GCDR_M_SystemType systemType;  /* optional; set in bit_mask
                                 * _3GCDR_M_SGSNSMORecord_systemType_present if
                                 * present */
    _3GCDR_M_CalledNumber destinationNumber;  /* optional; set in bit_mask
                                        * _3GCDR_M_destinationNumber_present if
                                        * present */
    _3GCDR_M_CAMELInformationSMS cAMELInformationSMS;  /* optional; set in
                                   * bit_mask
                                   * _3GCDR_M_cAMELInformationSMS_present if
                                   * present */
} _3GCDR_M_SGSNSMORecord;

typedef struct _3GCDR_M_SGSNSMTRecord {
    unsigned short  bit_mask;
#       define      _3GCDR_M_SGSNSMTRecord_servedIMEI_present 0x8000
#       define      _3GCDR_M_SGSNSMTRecord_servedMSISDN_present 0x4000
#       define      _3GCDR_M_SGSNSMTRecord_locationArea_present 0x2000
#       define      _3GCDR_M_SGSNSMTRecord_routingArea_present 0x1000
#       define      _3GCDR_M_SGSNSMTRecord_cellIdentifier_present 0x0800
#       define      _3GCDR_M_SGSNSMTRecord_smsResult_present 0x0400
#       define      _3GCDR_M_SGSNSMTRecord_recordExtensions_present 0x0200
#       define      _3GCDR_M_SGSNSMTRecord_nodeID_present 0x0100
#       define      _3GCDR_M_SGSNSMTRecord_localSequenceNumber_present 0x0080
#       define      _3GCDR_M_SGSNSMTRecord_chargingCharacteristics_present 0x0040
#       define      _3GCDR_M_SGSNSMTRecord_systemType_present 0x0020
    _3GCDR_M_CallEventRecordType recordType;
    _3GCDR_M_IMSI   servedIMSI;
    _3GCDR_M_IMEI   servedIMEI;  /* optional; set in bit_mask
                                  * _3GCDR_M_SGSNSMTRecord_servedIMEI_present if
                                  * present */
    _3GCDR_M_MSISDN servedMSISDN;  /* optional; set in bit_mask
                               * _3GCDR_M_SGSNSMTRecord_servedMSISDN_present if
                               * present */
    _3GCDR_M_MSNetworkCapability msNetworkCapability;
    _3GCDR_M_AddressString serviceCentre;
    _3GCDR_M_RecordingEntity recordingEntity;
    _3GCDR_M_LocationAreaCode locationArea;  /* optional; set in bit_mask
                               * _3GCDR_M_SGSNSMTRecord_locationArea_present if
                               * present */
    _3GCDR_M_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                * _3GCDR_M_SGSNSMTRecord_routingArea_present if
                                * present */
    _3GCDR_M_CellId cellIdentifier;  /* optional; set in bit_mask
                             * _3GCDR_M_SGSNSMTRecord_cellIdentifier_present if
                             * present */
    _3GCDR_M_TimeStamp originationTime;
    _3GCDR_M_SMSResult smsResult;  /* optional; set in bit_mask
                                    * _3GCDR_M_SGSNSMTRecord_smsResult_present
                                    * if present */
    _3GCDR_M_ManagementExtensions recordExtensions;  /* optional; set in
                                   * bit_mask
                           * _3GCDR_M_SGSNSMTRecord_recordExtensions_present if
                           * present */
    _3GCDR_M_NodeID nodeID;  /* optional; set in bit_mask
                              * _3GCDR_M_SGSNSMTRecord_nodeID_present if
                              * present */
    _3GCDR_M_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                        * _3GCDR_M_SGSNSMTRecord_localSequenceNumber_present if
                        * present */
    _3GCDR_M_ChargingCharacteristics chargingCharacteristics;  /* optional; set
                                   * in bit_mask
                    * _3GCDR_M_SGSNSMTRecord_chargingCharacteristics_present if
                    * present */
    _3GCDR_M_SystemType systemType;  /* optional; set in bit_mask
                                 * _3GCDR_M_SGSNSMTRecord_systemType_present if
                                 * present */
} _3GCDR_M_SGSNSMTRecord;

typedef struct _3GCDR_M_CallEventRecord {
    unsigned short  choice;
#       define      _3GCDR_M_sgsnPDPRecord_chosen 1
#       define      _3GCDR_M_ggsnPDPRecord_chosen 2
#       define      _3GCDR_M_sgsnMMRecord_chosen 3
#       define      _3GCDR_M_sgsnSMORecord_chosen 4
#       define      _3GCDR_M_sgsnSMTRecord_chosen 5
    union _union {
        _3GCDR_M_SGSNPDPRecord sgsnPDPRecord;  /* to choose, set choice to
                                             * _3GCDR_M_sgsnPDPRecord_chosen */
        _3GCDR_M_GGSNPDPRecord ggsnPDPRecord;  /* to choose, set choice to
                                             * _3GCDR_M_ggsnPDPRecord_chosen */
        _3GCDR_M_SGSNMMRecord sgsnMMRecord;  /* to choose, set choice to
                                              * _3GCDR_M_sgsnMMRecord_chosen */
        _3GCDR_M_SGSNSMORecord sgsnSMORecord;  /* to choose, set choice to
                                             * _3GCDR_M_sgsnSMORecord_chosen */
        _3GCDR_M_SGSNSMTRecord sgsnSMTRecord;  /* to choose, set choice to
                                             * _3GCDR_M_sgsnSMTRecord_chosen */
    } u;
} _3GCDR_M_CallEventRecord;

typedef enum _3GCDR_M_ChangeCondition {
    _3GCDR_M_qoSChange = 0,
    _3GCDR_M_tariffTime = 1,
    _3GCDR_M_recordClosure = 2
} _3GCDR_M_ChangeCondition;

typedef enum _3GCDR_M_QoSReliability {
    _3GCDR_M_unspecifiedReliability = 0,
    _3GCDR_M_acknowledgedGTP = 1,
    _3GCDR_M_unackGTPAcknowLLC = 2,
    _3GCDR_M_unackGTPLLCAcknowRLC = 3,
    _3GCDR_M_unackGTPLLCRLC = 4,
    _3GCDR_M_unacknowUnprotectedData = 5
} _3GCDR_M_QoSReliability;

typedef enum _3GCDR_M_QoSDelay {
    _3GCDR_M_delayClass1 = 1,
    _3GCDR_M_delayClass2 = 2,
    _3GCDR_M_delayClass3 = 3,
    _3GCDR_M_delayClass4 = 4
} _3GCDR_M_QoSDelay;

typedef enum _3GCDR_M_QoSPrecedence {
    _3GCDR_M_QoSPrecedence_unspecified = 0,
    _3GCDR_M_highPriority = 1,
    _3GCDR_M_normalPriority = 2,
    _3GCDR_M_lowPriority = 3
} _3GCDR_M_QoSPrecedence;

typedef enum _3GCDR_M_QoSPeakThroughput {
    _3GCDR_M_QoSPeakThroughput_unspecified = 0,
    _3GCDR_M_upTo1000octetPs = 1,
    _3GCDR_M_upTo2000octetPs = 2,
    _3GCDR_M_upTo4000octetPs = 3,
    _3GCDR_M_upTo8000octetPs = 4,
    _3GCDR_M_upTo16000octetPs = 5,
    _3GCDR_M_upTo32000octetPs = 6,
    _3GCDR_M_upTo64000octetPs = 7,
    _3GCDR_M_upTo128000octetPs = 8,
    _3GCDR_M_upTo256000octetPs = 9
} _3GCDR_M_QoSPeakThroughput;

typedef enum _3GCDR_M_QoSMeanThroughput {
    _3GCDR_M_bestEffort = 0,
    _3GCDR_M_mean100octetPh = 1,
    _3GCDR_M_mean200octetPh = 2,
    _3GCDR_M_mean500octetPh = 3,
    _3GCDR_M_mean1000octetPh = 4,
    _3GCDR_M_mean2000octetPh = 5,
    _3GCDR_M_mean5000octetPh = 6,
    _3GCDR_M_mean10000octetPh = 7,
    _3GCDR_M_mean20000octetPh = 8,
    _3GCDR_M_mean50000octetPh = 9,
    _3GCDR_M_mean100000octetPh = 10,
    _3GCDR_M_mean200000octetPh = 11,
    _3GCDR_M_mean500000octetPh = 12,
    _3GCDR_M_mean1000000octetPh = 13,
    _3GCDR_M_mean2000000octetPh = 14,
    _3GCDR_M_mean5000000octetPh = 15,
    _3GCDR_M_mean10000000octetPh = 16,
    _3GCDR_M_mean20000000octetPh = 17,
    _3GCDR_M_mean50000000octetPh = 18
} _3GCDR_M_QoSMeanThroughput;

typedef struct _3GCDR_M_GSMQoSInformation {
    _3GCDR_M_QoSReliability reliability;
    _3GCDR_M_QoSDelay delay;
    _3GCDR_M_QoSPrecedence precedence;
    _3GCDR_M_QoSPeakThroughput peakThroughput;
    _3GCDR_M_QoSMeanThroughput meanThroughput;
} _3GCDR_M_GSMQoSInformation;

typedef struct _3GCDR_M_QoSInformation {
    unsigned short  choice;
#       define      _3GCDR_M_gsmQosInformation_chosen 1
#       define      _3GCDR_M_umtsQosInformation_chosen 2
    union _union {
        _3GCDR_M_GSMQoSInformation gsmQosInformation;  /* to choose, set choice
                                      * to _3GCDR_M_gsmQosInformation_chosen */
        struct _3GCDR_M__octet8 {
            unsigned short  length;
            unsigned char   value[12];
        } umtsQosInformation;  /* to choose, set choice to
                                * _3GCDR_M_umtsQosInformation_chosen */
    } u;
} _3GCDR_M_QoSInformation;

typedef struct _3GCDR_M_ChangeOfCharCondition {
    unsigned char   bit_mask;
#       define      _3GCDR_M_qosRequested_present 0x80
#       define      _3GCDR_M_qosNegotiated_present 0x40
    _3GCDR_M_QoSInformation qosRequested;  /* optional; set in bit_mask
                                            * _3GCDR_M_qosRequested_present if
                                            * present */
    _3GCDR_M_QoSInformation qosNegotiated;  /* optional; set in bit_mask
                                             * _3GCDR_M_qosNegotiated_present if
                                             * present */
    _3GCDR_M_DataVolumeGPRS dataVolumeGPRSUplink;
    _3GCDR_M_DataVolumeGPRS dataVolumeGPRSDownlink;
    _3GCDR_M_ChangeCondition changeCondition;
    _3GCDR_M_TimeStamp changeTime;
} _3GCDR_M_ChangeOfCharCondition;

typedef struct _3GCDR_M_ChangeLocation {
    unsigned char   bit_mask;
#       define      _3GCDR_M_cellId_present 0x80
    _3GCDR_M_LocationAreaCode locationAreaCode;
    _3GCDR_M_RoutingAreaCode routingAreaCode;
    _3GCDR_M_CellId cellId;  /* optional; set in bit_mask
                              * _3GCDR_M_cellId_present if present */
    _3GCDR_M_TimeStamp changeTime;
} _3GCDR_M_ChangeLocation;

extern _3GCDR_M__ObjectID _3GCDR_M_gsm1205InformationModel;

extern _3GCDR_M__ObjectID _3GCDR_M_gsm1205ASN1Module;

extern int _3GCDR_M_maxAddressLength;

extern int _3GCDR_M_maxISDN_AddressLength;


extern void *CDRF_3GCDR_Final_M;    /* encoder-decoder control table */
#ifdef __cplusplus
}	/* extern "C" */
#endif /* __cplusplus */
#endif /* OSS_CDRF_3GCDR_Final_M */
