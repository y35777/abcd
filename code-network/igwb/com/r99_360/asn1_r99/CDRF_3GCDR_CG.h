/********************************************************/
/* Copyright (C) 2003 OSS Nokalva.  All rights reserved.*/
/********************************************************/

/* THIS FILE IS PROPRIETARY MATERIAL OF OSS NOKALVA
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF OSS NOKALVA.
 * THIS FILE MAY NOT BE DISTRIBUTED. */

/* Generated for: HuaWei Technologies, Inc., Shenzhen, China - license 8088 for Windows NT */
/* Abstract syntax: CDRF_3GCDR_Final */
/* Created: Fri Apr 25 09:48:53 2003 */
/* ASN.1 compiler version: 5.3 */
/* Target operating system: Windows NT/Windows 9x */
/* Target machine type: Intel x86 */
/* C compiler options required: -Zp4 (Microsoft) */
/* ASN.1 compiler options and file names specified:
 * -codefile CDRF_3GCDR_CG.c -headerfile CDRF_3GCDR_CG.h -errorfile
 * C:\WINDOWS\TEMP\2. -prefix _3GCDR_ -nodebug -dualheader -ansi -verbose -ber
 * -root D:\application\ossasn1\win32\5.3.0\asn1dflt\asn1dflt.ms.zp4
 * D:\project\530\asn1app\maimaoshi\CDRF_3GCDR_Final\CDRF_3GCDR_Final.asn
 */

#ifndef OSS_CDRF_3GCDR_Final
#define OSS_CDRF_3GCDR_Final

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "asn1hdr.h"
#include "asn1code.h"

#define          _3GCDR_CallEventRecord_PDU 1

typedef struct _3GCDR__ObjectID {
    unsigned short  length;
    unsigned char   *value;
} _3GCDR__ObjectID;

typedef struct _3GCDR__Any {
    unsigned long   length;
    unsigned char   *value;
} _3GCDR__Any;

typedef int             _3GCDR_CallEventRecordType;
#define                     _3GCDR_CallEventRecordType_sgsnPDPRecord 18
#define                     _3GCDR_CallEventRecordType_ggsnPDPRecord 19
#define                     _3GCDR_CallEventRecordType_sgsnMMRecord 20
#define                     _3GCDR_CallEventRecordType_sgsnSMORecord 21
#define                     _3GCDR_CallEventRecordType_sgsnSMTRecord 22

typedef ossBoolean      _3GCDR_NetworkInitiatedPDPContext;

typedef struct _3GCDR_IMSI {
    unsigned short  length;
    unsigned char   value[8];
} _3GCDR_IMSI;

typedef struct _3GCDR_IMEI {
    unsigned short  length;
    unsigned char   value[8];
} _3GCDR_IMEI;

typedef struct _3GCDR_IPBinaryAddress {
    unsigned short  choice;
#       define      _3GCDR_iPBinV4Address_chosen 1
#       define      _3GCDR_iPBinV6Address_chosen 2
    union _union {
        struct _3GCDR__octet1 {
            unsigned short  length;
            unsigned char   value[4];
        } iPBinV4Address;  /* to choose, set choice to
                            * _3GCDR_iPBinV4Address_chosen */
        struct _3GCDR__octet2 {
            unsigned short  length;
            unsigned char   value[16];
        } iPBinV6Address;  /* to choose, set choice to
                            * _3GCDR_iPBinV6Address_chosen */
    } u;
} _3GCDR_IPBinaryAddress;

typedef struct _3GCDR_IPTextRepresentedAddress {
    unsigned short  choice;
#       define      _3GCDR_iPTextV4Address_chosen 1
#       define      _3GCDR_iPTextV6Address_chosen 2
    union _union {
        char            iPTextV4Address[16];  /* to choose, set choice to
                                             * _3GCDR_iPTextV4Address_chosen */
        char            iPTextV6Address[46];  /* to choose, set choice to
                                             * _3GCDR_iPTextV6Address_chosen */
    } u;
} _3GCDR_IPTextRepresentedAddress;

typedef struct _3GCDR_IPAddress {
    unsigned short  choice;
#       define      _3GCDR_iPBinaryAddress_chosen 1
#       define      _3GCDR_iPTextRepresentedAddress_chosen 2
    union _union {
        _3GCDR_IPBinaryAddress iPBinaryAddress;  /* to choose, set choice to
                                             * _3GCDR_iPBinaryAddress_chosen */
        _3GCDR_IPTextRepresentedAddress iPTextRepresentedAddress;  /* to choose,
                                   * set choice to
                                   * _3GCDR_iPTextRepresentedAddress_chosen */
    } u;
} _3GCDR_IPAddress;

typedef _3GCDR_IPAddress _3GCDR_GSNAddress;

typedef struct _3GCDR_MSNetworkCapability {
    unsigned short  length;
    unsigned char   value[1];
} _3GCDR_MSNetworkCapability;

typedef struct _3GCDR_RoutingAreaCode {
    unsigned short  length;
    unsigned char   value[1];
} _3GCDR_RoutingAreaCode;

typedef struct _3GCDR_LocationAreaCode {
    unsigned short  length;
    unsigned char   value[2];
} _3GCDR_LocationAreaCode;

typedef struct _3GCDR_CellId {
    unsigned short  length;
    unsigned char   value[2];
} _3GCDR_CellId;

typedef unsigned int    _3GCDR_ChargingID;

typedef char            _3GCDR_AccessPointNameNI[64];

typedef struct _3GCDR_PDPType {
    unsigned short  length;
    unsigned char   value[2];
} _3GCDR_PDPType;

typedef struct _3GCDR_AddressString {
    unsigned short  length;
    unsigned char   value[20];
} _3GCDR_AddressString;

typedef _3GCDR_AddressString _3GCDR_ETSIAddress;

typedef struct _3GCDR_PDPAddress {
    unsigned short  choice;
#       define      _3GCDR_iPAddress_chosen 1
#       define      _3GCDR_eTSIAddress_chosen 2
    union _union {
        _3GCDR_IPAddress iPAddress;  /* to choose, set choice to
                                      * _3GCDR_iPAddress_chosen */
        _3GCDR_ETSIAddress eTSIAddress;  /* to choose, set choice to
                                          * _3GCDR_eTSIAddress_chosen */
    } u;
} _3GCDR_PDPAddress;

typedef struct _3GCDR_TimeStamp {
    unsigned short  length;
    unsigned char   value[9];
} _3GCDR_TimeStamp;

typedef int             _3GCDR_CallDuration;

typedef ossBoolean      _3GCDR_SGSNChange;

typedef int             _3GCDR_CauseForRecClosing;
#define                     _3GCDR_normalRelease 0
#define                     _3GCDR_abnormalRelease 4
#define                     _3GCDR_cAMELInitCallRelease 5
#define                     _3GCDR_volumeLimit 16
#define                     _3GCDR_timeLimit 17
#define                     _3GCDR_sGSNChange 18
#define                     _3GCDR_maxChangeCond 19
#define                     _3GCDR_managementIntervention 20

typedef struct _3GCDR_ManagementExtension {
    unsigned char   bit_mask;
#       define      _3GCDR_significance_present 0x80
    _3GCDR__ObjectID identifier;
    ossBoolean      significance;  /* _3GCDR_significance_present not set in
                                    * bit_mask implies value is FALSE */
    _3GCDR__Any     information;
} _3GCDR_ManagementExtension;

typedef struct _3GCDR_Diagnostics {
    unsigned short  choice;
#       define      _3GCDR_gsm0408Cause_chosen 1
#       define      _3GCDR_gsm0902MapErrorValue_chosen 2
#       define      _3GCDR_ccittQ767Cause_chosen 3
#       define      _3GCDR_networkSpecificCause_chosen 4
#       define      _3GCDR_manufacturerSpecificCause_chosen 5
    union _union {
        int             gsm0408Cause;  /* to choose, set choice to
                                        * _3GCDR_gsm0408Cause_chosen */
        int             gsm0902MapErrorValue;  /* to choose, set choice to
                                        * _3GCDR_gsm0902MapErrorValue_chosen */
        int             ccittQ767Cause;  /* to choose, set choice to
                                          * _3GCDR_ccittQ767Cause_chosen */
        _3GCDR_ManagementExtension networkSpecificCause;  /* to choose, set
                                   * choice to
                                   * _3GCDR_networkSpecificCause_chosen */
        _3GCDR_ManagementExtension manufacturerSpecificCause;  /* to choose, set
                                   * choice to
                                   * _3GCDR_manufacturerSpecificCause_chosen */
    } u;
} _3GCDR_Diagnostics;

struct _3GCDR__seqof1 {
   unsigned int    count;
   int             *value;
};
    
typedef struct _3GCDR_SequenceList {
    unsigned char   bit_mask;
#       define      _3GCDR_recSequenceNumber_present 0x80
    _3GCDR_GSNAddress sgsnAddress;
    struct _3GCDR__seqof1 recSequenceNumber;  /* optional; set in bit_mask
                           * _3GCDR_recSequenceNumber_present if present */
} _3GCDR_SequenceList;

typedef char            _3GCDR_NodeID[21];

typedef struct _3GCDR_ManagementExtensions {
    unsigned int    count;
    _3GCDR_ManagementExtension *value;
} _3GCDR_ManagementExtensions;

typedef enum _3GCDR_APNSelectionMode {
    _3GCDR_mSorNetworkProvidedSubscriptionVerified = 0,
    _3GCDR_mSProvidedSubscriptionNotVerified = 1,
    _3GCDR_networkProvidedSubscriptionNotVerified = 2
} _3GCDR_APNSelectionMode;

typedef char            _3GCDR_AccessPointNameOI[38];

typedef struct _3GCDR_ISDN_AddressString {
    unsigned short  length;
    unsigned char   value[9];
} _3GCDR_ISDN_AddressString;

typedef _3GCDR_ISDN_AddressString _3GCDR_MSISDN;

typedef struct _3GCDR_ChargingCharacteristics {
    unsigned short  length;
    unsigned char   value[2];
} _3GCDR_ChargingCharacteristics;

typedef enum _3GCDR_SystemType {
    _3GCDR_unknown = 0,
    _3GCDR_iuUTRAN = 1,
    _3GCDR_gERAN = 2
} _3GCDR_SystemType;

typedef _3GCDR_AddressString _3GCDR_SCFAddress;

typedef unsigned int    _3GCDR_ServiceKey;

typedef enum _3GCDR_DefaultGPRS_Handling {
    _3GCDR_DefaultGPRS_Handling_continueTransaction = 0,
    _3GCDR_DefaultGPRS_Handling_releaseTransaction = 1
} _3GCDR_DefaultGPRS_Handling;

typedef _3GCDR_AccessPointNameNI _3GCDR_CAMELAccessPointNameNI;

typedef _3GCDR_AccessPointNameOI _3GCDR_CAMELAccessPointNameOI;

typedef int             _3GCDR_NumberOfDPEncountered;

typedef unsigned char   _3GCDR_LevelOfCAMELService;
#define                     _3GCDR_basic 0x80
#define                     _3GCDR_callDurationSupervision 0x40
#define                     _3GCDR_onlineCharging 0x20

typedef struct _3GCDR_FreeFormatData {
    unsigned short  length;
    unsigned char   value[160];
} _3GCDR_FreeFormatData;

typedef ossBoolean      _3GCDR_FFDAppendIndicator;

typedef struct _3GCDR_CAMELInformationPDP {
    unsigned short  bit_mask;
#       define      _3GCDR_CAMELInformationPDP_sCFAddress_present 0x8000
#       define      _3GCDR_CAMELInformationPDP_serviceKey_present 0x4000
#       define      _3GCDR_CAMELInformationPDP_defaultTransactionHandling_present 0x2000
#       define      _3GCDR_cAMELAccessPointNameNI_present 0x1000
#       define      _3GCDR_cAMELAccessPointNameOI_present 0x0800
#       define      _3GCDR_CAMELInformationPDP_numberOfDPEncountered_present 0x0400
#       define      _3GCDR_CAMELInformationPDP_levelOfCAMELService_present 0x0200
#       define      _3GCDR_CAMELInformationPDP_freeFormatData_present 0x0100
#       define      _3GCDR_CAMELInformationPDP_fFDAppendIndicator_present 0x0080
    _3GCDR_SCFAddress sCFAddress;  /* optional; set in bit_mask
                             * _3GCDR_CAMELInformationPDP_sCFAddress_present if
                             * present */
    _3GCDR_ServiceKey serviceKey;  /* optional; set in bit_mask
                             * _3GCDR_CAMELInformationPDP_serviceKey_present if
                             * present */
    _3GCDR_DefaultGPRS_Handling defaultTransactionHandling;  /* optional; set in
                                   * bit_mask
             * _3GCDR_CAMELInformationPDP_defaultTransactionHandling_present if
             * present */
    _3GCDR_CAMELAccessPointNameNI cAMELAccessPointNameNI;  /* optional; set in
                                   * bit_mask
                                   * _3GCDR_cAMELAccessPointNameNI_present if
                                   * present */
    _3GCDR_CAMELAccessPointNameOI cAMELAccessPointNameOI;  /* optional; set in
                                   * bit_mask
                                   * _3GCDR_cAMELAccessPointNameOI_present if
                                   * present */
    _3GCDR_NumberOfDPEncountered numberOfDPEncountered;  /* optional; set in
                                   * bit_mask
                  * _3GCDR_CAMELInformationPDP_numberOfDPEncountered_present if
                  * present */
    _3GCDR_LevelOfCAMELService levelOfCAMELService;  /* optional; set in
                                   * bit_mask
                    * _3GCDR_CAMELInformationPDP_levelOfCAMELService_present if
                    * present */
    _3GCDR_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                         * _3GCDR_CAMELInformationPDP_freeFormatData_present if
                         * present */
    _3GCDR_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in bit_mask
                     * _3GCDR_CAMELInformationPDP_fFDAppendIndicator_present if
                     * present */
} _3GCDR_CAMELInformationPDP;

typedef int             _3GCDR_DataVolumeGPRS;

typedef enum _3GCDR_ConsolidationResult {
    _3GCDR_consolidationResult_normal = 0,
    _3GCDR_consolidationResult_notnormal = 1,
    _3GCDR_consolidationResult_ForInterSGSNConsolidation = 2,
    _3GCDR_consolidationResult_ReachLimit = 3,
	_3GCDR_consolidationResult_onlyOneCDRGenerated = 4
} _3GCDR_ConsolidationResult;

typedef struct _3GCDR__seqof2 {
    unsigned int    count;
    struct _3GCDR_ChangeOfCharCondition *value;
} _3GCDR__seqof2;

typedef struct _3GCDR_SGSNPDPRecord {
    unsigned int    bit_mask;
#       define      _3GCDR_SGSNPDPRecord_networkInitiation_present 0x80000000
#       define      _3GCDR_SGSNPDPRecord_servedIMEI_present 0x40000000
#       define      _3GCDR_SGSNPDPRecord_msNetworkCapability_present 0x20000000
#       define      _3GCDR_SGSNPDPRecord_routingArea_present 0x10000000
#       define      _3GCDR_SGSNPDPRecord_locationAreaCode_present 0x08000000
#       define      _3GCDR_SGSNPDPRecord_cellIdentifier_present 0x04000000
#       define      _3GCDR_SGSNPDPRecord_servedPDPAddress_present 0x02000000
#       define      _3GCDR_SGSNPDPRecord_sgsnChange_present 0x01000000
#       define      _3GCDR_SGSNPDPRecord_diagnostics_present 0x00800000
#       define      _3GCDR_SGSNPDPRecord_recSequenceNumList_present 0x00400000
#       define      _3GCDR_SGSNPDPRecord_nodeID_present 0x00200000
#       define      _3GCDR_SGSNPDPRecord_recordExtensions_present 0x00100000
#       define      _3GCDR_SGSNPDPRecord_apnSelectionMode_present 0x00080000
#       define      _3GCDR_SGSNPDPRecord_servedMSISDN_present 0x00040000
#       define      _3GCDR_SGSNPDPRecord_chargingCharacteristics_present 0x00020000
#       define      _3GCDR_SGSNPDPRecord_systemType_present 0x00010000
#       define      _3GCDR_cAMELInformationPDP_present 0x00008000
#       define      _3GCDR_rNCUnsentDownlinkVolume_present 0x00004000
    _3GCDR_CallEventRecordType recordType;
    _3GCDR_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                            * _3GCDR_SGSNPDPRecord_networkInitiation_present if
                            * present */
    _3GCDR_IMSI     servedIMSI;
    _3GCDR_IMEI     servedIMEI;  /* optional; set in bit_mask
                                  * _3GCDR_SGSNPDPRecord_servedIMEI_present if
                                  * present */
    _3GCDR_GSNAddress sgsnAddress;
    _3GCDR_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                          * _3GCDR_SGSNPDPRecord_msNetworkCapability_present if
                          * present */
    _3GCDR_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                  * _3GCDR_SGSNPDPRecord_routingArea_present if
                                  * present */
    _3GCDR_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                             * _3GCDR_SGSNPDPRecord_locationAreaCode_present if
                             * present */
    _3GCDR_CellId   cellIdentifier;  /* optional; set in bit_mask
                               * _3GCDR_SGSNPDPRecord_cellIdentifier_present if
                               * present */
    _3GCDR_ChargingID chargingID;
    _3GCDR_GSNAddress ggsnAddressUsed;
    _3GCDR_AccessPointNameNI accessPointNameNI;
    _3GCDR_PDPType  pdpType;
    _3GCDR_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                             * _3GCDR_SGSNPDPRecord_servedPDPAddress_present if
                             * present */
    _3GCDR__seqof2  listOfTrafficVolumes;
    _3GCDR_TimeStamp recordOpeningTime;
    _3GCDR_CallDuration duration;
    _3GCDR_SGSNChange sgsnChange;  /* optional; set in bit_mask
                                    * _3GCDR_SGSNPDPRecord_sgsnChange_present if
                                    * present */
    _3GCDR_CauseForRecClosing causeForRecClosing;
    _3GCDR_Diagnostics diagnostics;  /* optional; set in bit_mask
                                      * _3GCDR_SGSNPDPRecord_diagnostics_present
                                      * if present */
    _3GCDR_SequenceList recSequenceNumList;  /* optional; set in bit_mask
                           * _3GCDR_SGSNPDPRecord_recSequenceNumList_present if
                           * present */
    _3GCDR_NodeID   nodeID;  /* optional; set in bit_mask
                              * _3GCDR_SGSNPDPRecord_nodeID_present if
                              * present */
    _3GCDR_ManagementExtensions recordExtensions;  /* optional; set in bit_mask
                             * _3GCDR_SGSNPDPRecord_recordExtensions_present if
                             * present */
    _3GCDR_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                             * _3GCDR_SGSNPDPRecord_apnSelectionMode_present if
                             * present */
    _3GCDR_AccessPointNameOI accessPointNameOI;
    _3GCDR_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * _3GCDR_SGSNPDPRecord_servedMSISDN_present
                                    * if present */
    _3GCDR_ChargingCharacteristics chargingCharacteristics;  /* optional; set in
                                   * bit_mask
                      * _3GCDR_SGSNPDPRecord_chargingCharacteristics_present if
                      * present */
    _3GCDR_SystemType systemType;  /* optional; set in bit_mask
                                    * _3GCDR_SGSNPDPRecord_systemType_present if
                                    * present */
    _3GCDR_CAMELInformationPDP cAMELInformationPDP;  /* optional; set in
                                   * bit_mask _3GCDR_cAMELInformationPDP_present
                                   * if present */
    _3GCDR_DataVolumeGPRS rNCUnsentDownlinkVolume;  /* optional; set in bit_mask
                                    * _3GCDR_rNCUnsentDownlinkVolume_present if
                                    * present */
    _3GCDR_ConsolidationResult consolidationResult;
} _3GCDR_SGSNPDPRecord;

typedef ossBoolean      _3GCDR_DynamicAddressFlag;

typedef struct _3GCDR_GGSNPDPRecord {
    unsigned short  bit_mask;
#       define      _3GCDR_GGSNPDPRecord_networkInitiation_present 0x8000
#       define      _3GCDR_GGSNPDPRecord_servedPDPAddress_present 0x4000
#       define      _3GCDR_dynamicAddressFlag_present 0x2000
#       define      _3GCDR_GGSNPDPRecord_diagnostics_present 0x1000
#       define      _3GCDR_GGSNPDPRecord_recSequenceNumList_present 0x0800
#       define      _3GCDR_GGSNPDPRecord_nodeID_present 0x0400
#       define      _3GCDR_GGSNPDPRecord_recordExtensions_present 0x0200
#       define      _3GCDR_GGSNPDPRecord_apnSelectionMode_present 0x0100
#       define      _3GCDR_GGSNPDPRecord_servedMSISDN_present 0x0080
#       define      _3GCDR_GGSNPDPRecord_chargingCharacteristics_present 0x0040
    _3GCDR_CallEventRecordType recordType;
    _3GCDR_NetworkInitiatedPDPContext networkInitiation;  /* optional; set in
                                   * bit_mask
                            * _3GCDR_GGSNPDPRecord_networkInitiation_present if
                            * present */
    _3GCDR_IMSI     servedIMSI;
    _3GCDR_GSNAddress ggsnAddress;
    _3GCDR_ChargingID chargingID;
    struct _3GCDR__seqof3 {
        unsigned int    count;
        _3GCDR_GSNAddress *value;
    } sgsnAddress;
    _3GCDR_AccessPointNameNI accessPointNameNI;
    _3GCDR_PDPType  pdpType;
    _3GCDR_PDPAddress servedPDPAddress;  /* optional; set in bit_mask
                             * _3GCDR_GGSNPDPRecord_servedPDPAddress_present if
                             * present */
    _3GCDR_DynamicAddressFlag dynamicAddressFlag;  /* optional; set in bit_mask
                                         * _3GCDR_dynamicAddressFlag_present if
                                         * present */
    _3GCDR__seqof2  listOfTrafficVolumes;
    _3GCDR_TimeStamp recordOpeningTime;
    _3GCDR_CallDuration duration;
    _3GCDR_CauseForRecClosing causeForRecClosing;
    _3GCDR_Diagnostics diagnostics;  /* optional; set in bit_mask
                                      * _3GCDR_GGSNPDPRecord_diagnostics_present
                                      * if present */
    _3GCDR_SequenceList recSequenceNumList;  /* optional; set in bit_mask
                           * _3GCDR_GGSNPDPRecord_recSequenceNumList_present if
                           * present */
    _3GCDR_NodeID   nodeID;  /* optional; set in bit_mask
                              * _3GCDR_GGSNPDPRecord_nodeID_present if
                              * present */
    _3GCDR_ManagementExtensions recordExtensions;  /* optional; set in bit_mask
                             * _3GCDR_GGSNPDPRecord_recordExtensions_present if
                             * present */
    _3GCDR_APNSelectionMode apnSelectionMode;  /* optional; set in bit_mask
                             * _3GCDR_GGSNPDPRecord_apnSelectionMode_present if
                             * present */
    _3GCDR_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * _3GCDR_GGSNPDPRecord_servedMSISDN_present
                                    * if present */
    _3GCDR_ChargingCharacteristics chargingCharacteristics;  /* optional; set in
                                   * bit_mask
                      * _3GCDR_GGSNPDPRecord_chargingCharacteristics_present if
                      * present */
    _3GCDR_ConsolidationResult consolidationResult;
} _3GCDR_GGSNPDPRecord;

typedef unsigned int    _3GCDR_LocalSequenceNumber;

typedef struct _3GCDR_CAMELInformationMM {
    unsigned char   bit_mask;
#       define      _3GCDR_CAMELInformationMM_sCFAddress_present 0x80
#       define      _3GCDR_CAMELInformationMM_serviceKey_present 0x40
#       define      _3GCDR_CAMELInformationMM_defaultTransactionHandling_present 0x20
#       define      _3GCDR_CAMELInformationMM_numberOfDPEncountered_present 0x10
#       define      _3GCDR_CAMELInformationMM_levelOfCAMELService_present 0x08
#       define      _3GCDR_CAMELInformationMM_freeFormatData_present 0x04
#       define      _3GCDR_CAMELInformationMM_fFDAppendIndicator_present 0x02
    _3GCDR_SCFAddress sCFAddress;  /* optional; set in bit_mask
                              * _3GCDR_CAMELInformationMM_sCFAddress_present if
                              * present */
    _3GCDR_ServiceKey serviceKey;  /* optional; set in bit_mask
                              * _3GCDR_CAMELInformationMM_serviceKey_present if
                              * present */
    _3GCDR_DefaultGPRS_Handling defaultTransactionHandling;  /* optional; set in
                                   * bit_mask
              * _3GCDR_CAMELInformationMM_defaultTransactionHandling_present if
              * present */
    _3GCDR_NumberOfDPEncountered numberOfDPEncountered;  /* optional; set in
                                   * bit_mask
                   * _3GCDR_CAMELInformationMM_numberOfDPEncountered_present if
                   * present */
    _3GCDR_LevelOfCAMELService levelOfCAMELService;  /* optional; set in
                                   * bit_mask
                     * _3GCDR_CAMELInformationMM_levelOfCAMELService_present if
                     * present */
    _3GCDR_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                          * _3GCDR_CAMELInformationMM_freeFormatData_present if
                          * present */
    _3GCDR_FFDAppendIndicator fFDAppendIndicator;  /* optional; set in bit_mask
                      * _3GCDR_CAMELInformationMM_fFDAppendIndicator_present if
                      * present */
} _3GCDR_CAMELInformationMM;

typedef struct _3GCDR_SGSNMMRecord {
    unsigned int    bit_mask;
#       define      _3GCDR_SGSNMMRecord_servedIMEI_present 0x80000000
#       define      _3GCDR_SGSNMMRecord_msNetworkCapability_present 0x40000000
#       define      _3GCDR_SGSNMMRecord_routingArea_present 0x20000000
#       define      _3GCDR_SGSNMMRecord_locationAreaCode_present 0x10000000
#       define      _3GCDR_SGSNMMRecord_cellIdentifier_present 0x08000000
#       define      _3GCDR_changeLocation_present 0x04000000
#       define      _3GCDR_duration_present 0x02000000
#       define      _3GCDR_SGSNMMRecord_sgsnChange_present 0x01000000
#       define      _3GCDR_SGSNMMRecord_diagnostics_present 0x00800000
#       define      _3GCDR_recordSequenceNumber_present 0x00400000
#       define      _3GCDR_SGSNMMRecord_nodeID_present 0x00200000
#       define      _3GCDR_SGSNMMRecord_recordExtensions_present 0x00100000
#       define      _3GCDR_SGSNMMRecord_localSequenceNumber_present 0x00080000
#       define      _3GCDR_SGSNMMRecord_servedMSISDN_present 0x00040000
#       define      _3GCDR_SGSNMMRecord_chargingCharacteristics_present 0x00020000
#       define      _3GCDR_cAMELInformationMM_present 0x00010000
#       define      _3GCDR_SGSNMMRecord_systemType_present 0x00008000
    _3GCDR_CallEventRecordType recordType;
    _3GCDR_IMSI     servedIMSI;
    _3GCDR_IMEI     servedIMEI;  /* optional; set in bit_mask
                                  * _3GCDR_SGSNMMRecord_servedIMEI_present if
                                  * present */
    _3GCDR_GSNAddress sgsnAddress;
    _3GCDR_MSNetworkCapability msNetworkCapability;  /* optional; set in
                                   * bit_mask
                           * _3GCDR_SGSNMMRecord_msNetworkCapability_present if
                           * present */
    _3GCDR_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                   * _3GCDR_SGSNMMRecord_routingArea_present if
                                   * present */
    _3GCDR_LocationAreaCode locationAreaCode;  /* optional; set in bit_mask
                              * _3GCDR_SGSNMMRecord_locationAreaCode_present if
                              * present */
    _3GCDR_CellId   cellIdentifier;  /* optional; set in bit_mask
                                * _3GCDR_SGSNMMRecord_cellIdentifier_present if
                                * present */
    struct _3GCDR__seqof4 {
        unsigned int    count;
        struct _3GCDR_ChangeLocation *value;
    } changeLocation;  /* optional; set in bit_mask
                        * _3GCDR_changeLocation_present if present */
    _3GCDR_TimeStamp recordOpeningTime;
    _3GCDR_CallDuration duration;  /* optional; set in bit_mask
                                    * _3GCDR_duration_present if present */
    _3GCDR_SGSNChange sgsnChange;  /* optional; set in bit_mask
                                    * _3GCDR_SGSNMMRecord_sgsnChange_present if
                                    * present */
    _3GCDR_CauseForRecClosing causeForRecClosing;
    _3GCDR_Diagnostics diagnostics;  /* optional; set in bit_mask
                                      * _3GCDR_SGSNMMRecord_diagnostics_present
                                      * if present */
    int             recordSequenceNumber;  /* optional; set in bit_mask
                                       * _3GCDR_recordSequenceNumber_present if
                                       * present */
    _3GCDR_NodeID   nodeID;  /* optional; set in bit_mask
                              * _3GCDR_SGSNMMRecord_nodeID_present if present */
    _3GCDR_ManagementExtensions recordExtensions;  /* optional; set in bit_mask
                              * _3GCDR_SGSNMMRecord_recordExtensions_present if
                              * present */
    _3GCDR_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                           * _3GCDR_SGSNMMRecord_localSequenceNumber_present if
                           * present */
    _3GCDR_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * _3GCDR_SGSNMMRecord_servedMSISDN_present
                                    * if present */
    _3GCDR_ChargingCharacteristics chargingCharacteristics;  /* optional; set in
                                   * bit_mask
                       * _3GCDR_SGSNMMRecord_chargingCharacteristics_present if
                       * present */
    _3GCDR_CAMELInformationMM cAMELInformationMM;  /* optional; set in bit_mask
                                         * _3GCDR_cAMELInformationMM_present if
                                         * present */
    _3GCDR_SystemType systemType;  /* optional; set in bit_mask
                                    * _3GCDR_SGSNMMRecord_systemType_present if
                                    * present */
} _3GCDR_SGSNMMRecord;

typedef _3GCDR_AddressString _3GCDR_RecordingEntity;

typedef struct _3GCDR_MessageReference {
    unsigned int    length;
    unsigned char   *value;
} _3GCDR_MessageReference;

typedef _3GCDR_Diagnostics _3GCDR_SMSResult;

typedef struct _3GCDR_CalledNumber {
    unsigned short  length;
    unsigned char   value[10];
} _3GCDR_CalledNumber;

typedef enum _3GCDR_DefaultSMS_Handling {
    _3GCDR_DefaultSMS_Handling_continueTransaction = 0,
    _3GCDR_DefaultSMS_Handling_releaseTransaction = 1
} _3GCDR_DefaultSMS_Handling;

typedef struct _3GCDR_CallingNumber {
    unsigned short  length;
    unsigned char   value[10];
} _3GCDR_CallingNumber;

typedef struct _3GCDR_CAMELInformationSMS {
    unsigned char   bit_mask;
#       define      _3GCDR_CAMELInformationSMS_sCFAddress_present 0x80
#       define      _3GCDR_CAMELInformationSMS_serviceKey_present 0x40
#       define      _3GCDR_defaultSMSHandling_present 0x20
#       define      _3GCDR_cAMELCallingPartyNumber_present 0x10
#       define      _3GCDR_cAMELDestinationSubscriberNumber_present 0x08
#       define      _3GCDR_cAMELSMSCAddress_present 0x04
#       define      _3GCDR_CAMELInformationSMS_freeFormatData_present 0x02
    _3GCDR_SCFAddress sCFAddress;  /* optional; set in bit_mask
                             * _3GCDR_CAMELInformationSMS_sCFAddress_present if
                             * present */
    _3GCDR_ServiceKey serviceKey;  /* optional; set in bit_mask
                             * _3GCDR_CAMELInformationSMS_serviceKey_present if
                             * present */
    _3GCDR_DefaultSMS_Handling defaultSMSHandling;  /* optional; set in bit_mask
                                         * _3GCDR_defaultSMSHandling_present if
                                         * present */
    _3GCDR_CallingNumber cAMELCallingPartyNumber;  /* optional; set in bit_mask
                                    * _3GCDR_cAMELCallingPartyNumber_present if
                                    * present */
    _3GCDR_CalledNumber cAMELDestinationSubscriberNumber;  /* optional; set in
                                   * bit_mask
                           * _3GCDR_cAMELDestinationSubscriberNumber_present if
                           * present */
    _3GCDR_AddressString cAMELSMSCAddress;  /* optional; set in bit_mask
                                             * _3GCDR_cAMELSMSCAddress_present
                                             * if present */
    _3GCDR_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                         * _3GCDR_CAMELInformationSMS_freeFormatData_present if
                         * present */
} _3GCDR_CAMELInformationSMS;

typedef struct _3GCDR_SGSNSMORecord {
    unsigned short  bit_mask;
#       define      _3GCDR_SGSNSMORecord_servedIMEI_present 0x8000
#       define      _3GCDR_SGSNSMORecord_servedMSISDN_present 0x4000
#       define      _3GCDR_SGSNSMORecord_locationArea_present 0x2000
#       define      _3GCDR_SGSNSMORecord_routingArea_present 0x1000
#       define      _3GCDR_SGSNSMORecord_cellIdentifier_present 0x0800
#       define      _3GCDR_SGSNSMORecord_smsResult_present 0x0400
#       define      _3GCDR_SGSNSMORecord_recordExtensions_present 0x0200
#       define      _3GCDR_SGSNSMORecord_nodeID_present 0x0100
#       define      _3GCDR_SGSNSMORecord_localSequenceNumber_present 0x0080
#       define      _3GCDR_SGSNSMORecord_chargingCharacteristics_present 0x0040
#       define      _3GCDR_SGSNSMORecord_systemType_present 0x0020
#       define      _3GCDR_destinationNumber_present 0x0010
#       define      _3GCDR_cAMELInformationSMS_present 0x0008
    _3GCDR_CallEventRecordType recordType;
    _3GCDR_IMSI     servedIMSI;
    _3GCDR_IMEI     servedIMEI;  /* optional; set in bit_mask
                                  * _3GCDR_SGSNSMORecord_servedIMEI_present if
                                  * present */
    _3GCDR_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * _3GCDR_SGSNSMORecord_servedMSISDN_present
                                    * if present */
    _3GCDR_MSNetworkCapability msNetworkCapability;
    _3GCDR_AddressString serviceCentre;
    _3GCDR_RecordingEntity recordingEntity;
    _3GCDR_LocationAreaCode locationArea;  /* optional; set in bit_mask
                                 * _3GCDR_SGSNSMORecord_locationArea_present if
                                 * present */
    _3GCDR_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                  * _3GCDR_SGSNSMORecord_routingArea_present if
                                  * present */
    _3GCDR_CellId   cellIdentifier;  /* optional; set in bit_mask
                               * _3GCDR_SGSNSMORecord_cellIdentifier_present if
                               * present */
    _3GCDR_MessageReference messageReference;
    _3GCDR_TimeStamp originationTime;
    _3GCDR_SMSResult smsResult;  /* optional; set in bit_mask
                                  * _3GCDR_SGSNSMORecord_smsResult_present if
                                  * present */
    _3GCDR_ManagementExtensions recordExtensions;  /* optional; set in bit_mask
                             * _3GCDR_SGSNSMORecord_recordExtensions_present if
                             * present */
    _3GCDR_NodeID   nodeID;  /* optional; set in bit_mask
                              * _3GCDR_SGSNSMORecord_nodeID_present if
                              * present */
    _3GCDR_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                          * _3GCDR_SGSNSMORecord_localSequenceNumber_present if
                          * present */
    _3GCDR_ChargingCharacteristics chargingCharacteristics;  /* optional; set in
                                   * bit_mask
                      * _3GCDR_SGSNSMORecord_chargingCharacteristics_present if
                      * present */
    _3GCDR_SystemType systemType;  /* optional; set in bit_mask
                                    * _3GCDR_SGSNSMORecord_systemType_present if
                                    * present */
    _3GCDR_CalledNumber destinationNumber;  /* optional; set in bit_mask
                                             * _3GCDR_destinationNumber_present
                                             * if present */
    _3GCDR_CAMELInformationSMS cAMELInformationSMS;  /* optional; set in
                                   * bit_mask _3GCDR_cAMELInformationSMS_present
                                   * if present */
} _3GCDR_SGSNSMORecord;

typedef struct _3GCDR_SGSNSMTRecord {
    unsigned short  bit_mask;
#       define      _3GCDR_SGSNSMTRecord_servedIMEI_present 0x8000
#       define      _3GCDR_SGSNSMTRecord_servedMSISDN_present 0x4000
#       define      _3GCDR_SGSNSMTRecord_locationArea_present 0x2000
#       define      _3GCDR_SGSNSMTRecord_routingArea_present 0x1000
#       define      _3GCDR_SGSNSMTRecord_cellIdentifier_present 0x0800
#       define      _3GCDR_SGSNSMTRecord_smsResult_present 0x0400
#       define      _3GCDR_SGSNSMTRecord_recordExtensions_present 0x0200
#       define      _3GCDR_SGSNSMTRecord_nodeID_present 0x0100
#       define      _3GCDR_SGSNSMTRecord_localSequenceNumber_present 0x0080
#       define      _3GCDR_SGSNSMTRecord_chargingCharacteristics_present 0x0040
#       define      _3GCDR_SGSNSMTRecord_systemType_present 0x0020
    _3GCDR_CallEventRecordType recordType;
    _3GCDR_IMSI     servedIMSI;
    _3GCDR_IMEI     servedIMEI;  /* optional; set in bit_mask
                                  * _3GCDR_SGSNSMTRecord_servedIMEI_present if
                                  * present */
    _3GCDR_MSISDN   servedMSISDN;  /* optional; set in bit_mask
                                    * _3GCDR_SGSNSMTRecord_servedMSISDN_present
                                    * if present */
    _3GCDR_MSNetworkCapability msNetworkCapability;
    _3GCDR_AddressString serviceCentre;
    _3GCDR_RecordingEntity recordingEntity;
    _3GCDR_LocationAreaCode locationArea;  /* optional; set in bit_mask
                                 * _3GCDR_SGSNSMTRecord_locationArea_present if
                                 * present */
    _3GCDR_RoutingAreaCode routingArea;  /* optional; set in bit_mask
                                  * _3GCDR_SGSNSMTRecord_routingArea_present if
                                  * present */
    _3GCDR_CellId   cellIdentifier;  /* optional; set in bit_mask
                               * _3GCDR_SGSNSMTRecord_cellIdentifier_present if
                               * present */
    _3GCDR_TimeStamp originationTime;
    _3GCDR_SMSResult smsResult;  /* optional; set in bit_mask
                                  * _3GCDR_SGSNSMTRecord_smsResult_present if
                                  * present */
    _3GCDR_ManagementExtensions recordExtensions;  /* optional; set in bit_mask
                             * _3GCDR_SGSNSMTRecord_recordExtensions_present if
                             * present */
    _3GCDR_NodeID   nodeID;  /* optional; set in bit_mask
                              * _3GCDR_SGSNSMTRecord_nodeID_present if
                              * present */
    _3GCDR_LocalSequenceNumber localSequenceNumber;  /* optional; set in
                                   * bit_mask
                          * _3GCDR_SGSNSMTRecord_localSequenceNumber_present if
                          * present */
    _3GCDR_ChargingCharacteristics chargingCharacteristics;  /* optional; set in
                                   * bit_mask
                      * _3GCDR_SGSNSMTRecord_chargingCharacteristics_present if
                      * present */
    _3GCDR_SystemType systemType;  /* optional; set in bit_mask
                                    * _3GCDR_SGSNSMTRecord_systemType_present if
                                    * present */
} _3GCDR_SGSNSMTRecord;

typedef struct _3GCDR_CallEventRecord {
    unsigned short  choice;
#       define      _3GCDR_sgsnPDPRecord_chosen 1
#       define      _3GCDR_ggsnPDPRecord_chosen 2
#       define      _3GCDR_sgsnMMRecord_chosen 3
#       define      _3GCDR_sgsnSMORecord_chosen 4
#       define      _3GCDR_sgsnSMTRecord_chosen 5
    union _union {
        _3GCDR_SGSNPDPRecord sgsnPDPRecord;  /* to choose, set choice to
                                              * _3GCDR_sgsnPDPRecord_chosen */
        _3GCDR_GGSNPDPRecord ggsnPDPRecord;  /* to choose, set choice to
                                              * _3GCDR_ggsnPDPRecord_chosen */
        _3GCDR_SGSNMMRecord sgsnMMRecord;  /* to choose, set choice to
                                            * _3GCDR_sgsnMMRecord_chosen */
        _3GCDR_SGSNSMORecord sgsnSMORecord;  /* to choose, set choice to
                                              * _3GCDR_sgsnSMORecord_chosen */
        _3GCDR_SGSNSMTRecord sgsnSMTRecord;  /* to choose, set choice to
                                              * _3GCDR_sgsnSMTRecord_chosen */
    } u;
} _3GCDR_CallEventRecord;

typedef enum _3GCDR_ChangeCondition {
    _3GCDR_qoSChange = 0,
    _3GCDR_tariffTime = 1,
    _3GCDR_recordClosure = 2
} _3GCDR_ChangeCondition;

typedef enum _3GCDR_QoSReliability {
    _3GCDR_unspecifiedReliability = 0,
    _3GCDR_acknowledgedGTP = 1,
    _3GCDR_unackGTPAcknowLLC = 2,
    _3GCDR_unackGTPLLCAcknowRLC = 3,
    _3GCDR_unackGTPLLCRLC = 4,
    _3GCDR_unacknowUnprotectedData = 5
} _3GCDR_QoSReliability;

typedef enum _3GCDR_QoSDelay {
    _3GCDR_delayClass1 = 1,
    _3GCDR_delayClass2 = 2,
    _3GCDR_delayClass3 = 3,
    _3GCDR_delayClass4 = 4
} _3GCDR_QoSDelay;

typedef enum _3GCDR_QoSPrecedence {
    _3GCDR_QoSPrecedence_unspecified = 0,
    _3GCDR_highPriority = 1,
    _3GCDR_normalPriority = 2,
    _3GCDR_lowPriority = 3
} _3GCDR_QoSPrecedence;

typedef enum _3GCDR_QoSPeakThroughput {
    _3GCDR_QoSPeakThroughput_unspecified = 0,
    _3GCDR_upTo1000octetPs = 1,
    _3GCDR_upTo2000octetPs = 2,
    _3GCDR_upTo4000octetPs = 3,
    _3GCDR_upTo8000octetPs = 4,
    _3GCDR_upTo16000octetPs = 5,
    _3GCDR_upTo32000octetPs = 6,
    _3GCDR_upTo64000octetPs = 7,
    _3GCDR_upTo128000octetPs = 8,
    _3GCDR_upTo256000octetPs = 9
} _3GCDR_QoSPeakThroughput;

typedef enum _3GCDR_QoSMeanThroughput {
    _3GCDR_bestEffort = 0,
    _3GCDR_mean100octetPh = 1,
    _3GCDR_mean200octetPh = 2,
    _3GCDR_mean500octetPh = 3,
    _3GCDR_mean1000octetPh = 4,
    _3GCDR_mean2000octetPh = 5,
    _3GCDR_mean5000octetPh = 6,
    _3GCDR_mean10000octetPh = 7,
    _3GCDR_mean20000octetPh = 8,
    _3GCDR_mean50000octetPh = 9,
    _3GCDR_mean100000octetPh = 10,
    _3GCDR_mean200000octetPh = 11,
    _3GCDR_mean500000octetPh = 12,
    _3GCDR_mean1000000octetPh = 13,
    _3GCDR_mean2000000octetPh = 14,
    _3GCDR_mean5000000octetPh = 15,
    _3GCDR_mean10000000octetPh = 16,
    _3GCDR_mean20000000octetPh = 17,
    _3GCDR_mean50000000octetPh = 18
} _3GCDR_QoSMeanThroughput;

typedef struct _3GCDR_GSMQoSInformation {
    _3GCDR_QoSReliability reliability;
    _3GCDR_QoSDelay delay;
    _3GCDR_QoSPrecedence precedence;
    _3GCDR_QoSPeakThroughput peakThroughput;
    _3GCDR_QoSMeanThroughput meanThroughput;
} _3GCDR_GSMQoSInformation;

typedef struct _3GCDR_QoSInformation {
    unsigned short  choice;
#       define      _3GCDR_gsmQosInformation_chosen 1
#       define      _3GCDR_umtsQosInformation_chosen 2
    union _union {
        _3GCDR_GSMQoSInformation gsmQosInformation;  /* to choose, set choice to
                                           * _3GCDR_gsmQosInformation_chosen */
        struct _3GCDR__octet3 {
            unsigned short  length;
            unsigned char   value[12];
        } umtsQosInformation;  /* to choose, set choice to
                                * _3GCDR_umtsQosInformation_chosen */
    } u;
} _3GCDR_QoSInformation;

typedef struct _3GCDR_ChangeOfCharCondition {
    unsigned char   bit_mask;
#       define      _3GCDR_qosRequested_present 0x80
#       define      _3GCDR_qosNegotiated_present 0x40
    _3GCDR_QoSInformation qosRequested;  /* optional; set in bit_mask
                                          * _3GCDR_qosRequested_present if
                                          * present */
    _3GCDR_QoSInformation qosNegotiated;  /* optional; set in bit_mask
                                           * _3GCDR_qosNegotiated_present if
                                           * present */
    _3GCDR_DataVolumeGPRS dataVolumeGPRSUplink;
    _3GCDR_DataVolumeGPRS dataVolumeGPRSDownlink;
    _3GCDR_ChangeCondition changeCondition;
    _3GCDR_TimeStamp changeTime;
} _3GCDR_ChangeOfCharCondition;

typedef struct _3GCDR_ChangeLocation {
    unsigned char   bit_mask;
#       define      _3GCDR_cellId_present 0x80
    _3GCDR_LocationAreaCode locationAreaCode;
    _3GCDR_RoutingAreaCode routingAreaCode;
    _3GCDR_CellId   cellId;  /* optional; set in bit_mask _3GCDR_cellId_present
                              * if present */
    _3GCDR_TimeStamp changeTime;
} _3GCDR_ChangeLocation;

extern _3GCDR__ObjectID _3GCDR_gsm1205InformationModel;

extern _3GCDR__ObjectID _3GCDR_gsm1205ASN1Module;

extern int _3GCDR_maxAddressLength;

extern int _3GCDR_maxISDN_AddressLength;


extern void *CDRF_3GCDR_Final;    /* encoder-decoder control table */
#ifdef __cplusplus
}	/* extern "C" */
#endif /* __cplusplus */
#endif /* OSS_CDRF_3GCDR_Final */
