/*************************************************************/
/* Copyright (C) 2004 OSS Nokalva, Inc.  All rights reserved.*/
/*************************************************************/

/* THIS FILE IS PROPRIETARY MATERIAL OF OSS NOKALVA, INC.
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF OSS NOKALVA, INC.
 * THIS FILE MAY NOT BE DISTRIBUTED. */

/* Generated for: HuaWei Technologies, Inc., Shenzhen, China - license 8088 for Windows NT */
/* Abstract syntax: CMCC_CS_R4_CDR */
/* Created: Thu Aug 05 16:49:47 2004 */
/* ASN.1 compiler version: 6.1.3 */
/* Runtime version is unknown */
/* Target operating system: Windows NT/Windows 9x */
/* Target machine type: Intel x86 */
/* C compiler options required: -Zp4 (Microsoft) */
/* ASN.1 compiler options and file names specified:
 * -codefile C:\ossasn1\lidufang\yangshan0804\windows\CMCC_CS_R4_CDR.C
 * -headerfile C:\ossasn1\lidufang\yangshan0804\windows\CMCC_CS_R4_CDR.h
 * -errorfile C:\ossasn1\win32.dev\6.1.3\bin\error.txt -nolistingfile
 * -nomodlistingfile -prefix CMCC_CS_ -nodebug -dualheader -ansi -verbose -ber
 * -root -charintegers -compat v5.3.0
 * C:\ossasn1\ossasn1\win32\5.3.0\asn1dflt\asn1dflt.ms.zp4
 * C:\ossasn1\lidufang\yangshan0804\CMCC_CS_R4_CDR.asn
 */

#ifndef OSS_CMCC_CS_R4_CDR
#define OSS_CMCC_CS_R4_CDR

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "asn1hdr.h"
#include "asn1code.h"

#define          CMCC_CS_CallEventDataFile_PDU 1
#define          CMCC_CS_ObservedIMEITicketFile_PDU 2
#define          CMCC_CS_CallTypes_PDU 3
#define          CMCC_CS_Day_PDU 4
#define          CMCC_CS_EmergencyCallIndEnable_PDU 5
#define          CMCC_CS_EmergencyCallIndication_PDU 6
#define          CMCC_CS_FileType_PDU 7
#define          CMCC_CS_HSCSDParmsChange_PDU 8
#define          CMCC_CS_Location_info_PDU 9
#define          CMCC_CS_LocUpdResult_PDU 10
#define          CMCC_CS_Month_PDU 11
#define          CMCC_CS_MSPowerClasses_PDU 12
#define          CMCC_CS_NetworkSpecificServices_PDU 13
#define          CMCC_CS_ObservedIMEITicketEnable_PDU 14
#define          CMCC_CS_OriginDestCombinations_PDU 15
#define          CMCC_CS_PartialRecordTimer_PDU 16
#define          CMCC_CS_PartialRecordTypes_PDU 17
#define          CMCC_CS_RadioChannelsRequested_PDU 18
#define          CMCC_CS_RecordingMethod_PDU 19
#define          CMCC_CS_Service_PDU 20
#define          CMCC_CS_ServiceDistanceDependencies_PDU 21
#define          CMCC_CS_SimpleIntegerName_PDU 22
#define          CMCC_CS_SimpleStringName_PDU 23
#define          CMCC_CS_SupplServices_PDU 24
#define          CMCC_CS_TariffId_PDU 25
#define          CMCC_CS_TariffPeriods_PDU 26
#define          CMCC_CS_TariffSystemStatus_PDU 27
#define          CMCC_CS_TSCheckResult_PDU 28
#define          CMCC_CS_TSCopyTariffSystem_PDU 29
#define          CMCC_CS_TSNextChange_PDU 30
#define          CMCC_CS_TypeOfSubscribers_PDU 31
#define          CMCC_CS_TypeOfTransaction_PDU 32
#define          CMCC_CS_CallEmlppPriority_PDU 33
#define          CMCC_CS_EmergencyCallFlag_PDU 34
#define          CMCC_CS_DisconnectParty_PDU 35
#define          CMCC_CS_OptimalRoutingDestAddress_PDU 36

typedef struct CMCC_CS__ObjectID {
    unsigned short  length;
    unsigned char   *value;
} CMCC_CS__ObjectID;

typedef int             CMCC_CS_CallEventRecordType;
#define                     CMCC_CS_CallEventRecordType_moCallRecord 0
#define                     CMCC_CS_CallEventRecordType_mtCallRecord 1
#define                     CMCC_CS_CallEventRecordType_roamingRecord 2
#define                     CMCC_CS_CallEventRecordType_incGatewayRecord 3
#define                     CMCC_CS_CallEventRecordType_outGatewayRecord 4
#define                     CMCC_CS_transitCallRecord 5
#define                     CMCC_CS_CallEventRecordType_moSMSRecord 6
#define                     CMCC_CS_CallEventRecordType_mtSMSRecord 7
#define                     CMCC_CS_CallEventRecordType_ssActionRecord 10
#define                     CMCC_CS_CallEventRecordType_hlrIntRecord 11
#define                     CMCC_CS_CallEventRecordType_commonEquipRecord 14
#define                     CMCC_CS_moTraceRecord 15
#define                     CMCC_CS_mtTraceRecord 16
#define                     CMCC_CS_CallEventRecordType_termCAMELRecord 17
#define                     CMCC_CS_CallEventRecordType_mtLCSRecord 23
#define                     CMCC_CS_CallEventRecordType_moLCSRecord 24
#define                     CMCC_CS_CallEventRecordType_niLCSRecord 25
#define                     CMCC_CS_CallEventRecordType_forwardCallRecord 100

typedef struct CMCC_CS_IMSI {
    unsigned short  length;
    unsigned char   value[8];
} CMCC_CS_IMSI;

typedef struct CMCC_CS_IMEI {
    unsigned short  length;
    unsigned char   value[8];
} CMCC_CS_IMEI;

typedef struct CMCC_CS_ISDN_AddressString {
    unsigned short  length;
    unsigned char   value[9];
} CMCC_CS_ISDN_AddressString;

typedef CMCC_CS_ISDN_AddressString CMCC_CS_MSISDN;

typedef struct CMCC_CS_BCDDirectoryNumber {
    unsigned int    length;
    unsigned char   *value;
} CMCC_CS_BCDDirectoryNumber;

typedef CMCC_CS_BCDDirectoryNumber CMCC_CS_CallingNumber;

typedef CMCC_CS_BCDDirectoryNumber CMCC_CS_CalledNumber;

typedef CMCC_CS_BCDDirectoryNumber CMCC_CS_TranslatedNumber;

typedef CMCC_CS_BCDDirectoryNumber CMCC_CS_ConnectedNumber;

typedef CMCC_CS_ISDN_AddressString CMCC_CS_RoamingNumber;

typedef struct CMCC_CS_AddressString {
    unsigned short  length;
    unsigned char   value[20];
} CMCC_CS_AddressString;

typedef CMCC_CS_AddressString CMCC_CS_RecordingEntity;

typedef struct CMCC_CS_ROUTE {
    unsigned short  choice;
#       define      CMCC_CS_rOUTENumber_chosen 1
#       define      CMCC_CS_rOUTEName_chosen 2
    union _union {
        int             rOUTENumber;  /* to choose, set choice to
                                       * CMCC_CS_rOUTENumber_chosen */
        char            *rOUTEName;  /* to choose, set choice to
                                      * CMCC_CS_rOUTEName_chosen */
    } u;
} CMCC_CS_ROUTE;

typedef struct CMCC_CS_LocationAreaCode {
    unsigned short  length;
    unsigned char   value[2];
} CMCC_CS_LocationAreaCode;

typedef struct CMCC_CS_CellId {
    unsigned short  length;
    unsigned char   value[2];
} CMCC_CS_CellId;

typedef struct CMCC_CS_LocationAreaAndCell {
    CMCC_CS_LocationAreaCode locationAreaCode;
    CMCC_CS_CellId  cellIdentifier;
} CMCC_CS_LocationAreaAndCell;

typedef struct CMCC_CS_TimeStamp {
    unsigned short  length;
    unsigned char   value[9];
} CMCC_CS_TimeStamp;

typedef struct CMCC_CS_LocationChange {
    CMCC_CS_LocationAreaAndCell location;
    CMCC_CS_TimeStamp changeTime;
} CMCC_CS_LocationChange;

typedef struct CMCC_CS_BearerServiceCode {
    unsigned short  length;
    unsigned char   value[1];
} CMCC_CS_BearerServiceCode;

typedef struct CMCC_CS_TeleserviceCode {
    unsigned short  length;
    unsigned char   value[1];
} CMCC_CS_TeleserviceCode;

typedef struct CMCC_CS_BasicServiceCode {
    unsigned short  choice;
#       define      CMCC_CS_BasicServiceCode_bearerService_chosen 1
#       define      CMCC_CS_BasicServiceCode_teleservice_chosen 2
    union _union {
        CMCC_CS_BearerServiceCode bearerService;  /* to choose, set choice to
                             * CMCC_CS_BasicServiceCode_bearerService_chosen */
        CMCC_CS_TeleserviceCode teleservice;  /* to choose, set choice to
                               * CMCC_CS_BasicServiceCode_teleservice_chosen */
    } u;
} CMCC_CS_BasicServiceCode;

typedef enum CMCC_CS_TransparencyInd {
    CMCC_CS_transparent = 0,
    CMCC_CS_nonTransparent = 1
} CMCC_CS_TransparencyInd;

typedef struct CMCC_CS_RateIndication {
    unsigned short  length;
    unsigned char   value[1];
} CMCC_CS_RateIndication;

typedef enum CMCC_CS_Fnur {
    CMCC_CS_fnurNotApplicable = 0,
    CMCC_CS_fnur9600_BitsPerSecond = 1,
    CMCC_CS_fnur14400BitsPerSecond = 2,
    CMCC_CS_fnur19200BitsPerSecond = 3,
    CMCC_CS_fnur28800BitsPerSecond = 4,
    CMCC_CS_fnur38400BitsPerSecond = 5,
    CMCC_CS_fnur48000BitsPerSecond = 6,
    CMCC_CS_fnur56000BitsPerSecond = 7,
    CMCC_CS_fnur64000BitsPerSecond = 8,
    CMCC_CS_fnur33600BitsPerSecond = 9,
    CMCC_CS_fnur32000BitsPerSecond = 10,
    CMCC_CS_fnur31200BitsPerSecond = 11
} CMCC_CS_Fnur;

typedef struct CMCC_CS_ChangeOfService {
    unsigned char   bit_mask;
#       define      CMCC_CS_transparencyInd_present 0x80
#       define      CMCC_CS_ChangeOfService_rateIndication_present 0x40
#       define      CMCC_CS_ChangeOfService_fnur_present 0x20
    CMCC_CS_BasicServiceCode basicService;
    CMCC_CS_TransparencyInd transparencyInd;  /* optional; set in bit_mask
                                               * CMCC_CS_transparencyInd_present
                                               * if present */
    CMCC_CS_TimeStamp changeTime;
    CMCC_CS_RateIndication rateIndication;  /* optional; set in bit_mask
                            * CMCC_CS_ChangeOfService_rateIndication_present if
                            * present */
    CMCC_CS_Fnur    fnur;  /* optional; set in bit_mask
                            * CMCC_CS_ChangeOfService_fnur_present if present */
} CMCC_CS_ChangeOfService;

typedef struct CMCC_CS_SS_Code {
    unsigned short  length;
    unsigned char   value[1];
} CMCC_CS_SS_Code;

typedef struct CMCC_CS_SuppServiceUsed {
    unsigned char   bit_mask;
#       define      CMCC_CS_ssCode_present 0x80
#       define      CMCC_CS_ssTime_present 0x40
    CMCC_CS_SS_Code ssCode;  /* optional; set in bit_mask CMCC_CS_ssCode_present
                              * if present */
    CMCC_CS_TimeStamp ssTime;  /* optional; set in bit_mask
                                * CMCC_CS_ssTime_present if present */
} CMCC_CS_SuppServiceUsed;

typedef unsigned short  CMCC_CS_EParameter;

typedef struct CMCC_CS_AOCParameters {
    unsigned char   bit_mask;
#       define      CMCC_CS_e1_present 0x80
#       define      CMCC_CS_e2_present 0x40
#       define      CMCC_CS_e3_present 0x20
#       define      CMCC_CS_e4_present 0x10
#       define      CMCC_CS_e5_present 0x08
#       define      CMCC_CS_e6_present 0x04
#       define      CMCC_CS_e7_present 0x02
    CMCC_CS_EParameter e1;  /* optional; set in bit_mask CMCC_CS_e1_present if
                             * present */
    CMCC_CS_EParameter e2;  /* optional; set in bit_mask CMCC_CS_e2_present if
                             * present */
    CMCC_CS_EParameter e3;  /* optional; set in bit_mask CMCC_CS_e3_present if
                             * present */
    CMCC_CS_EParameter e4;  /* optional; set in bit_mask CMCC_CS_e4_present if
                             * present */
    CMCC_CS_EParameter e5;  /* optional; set in bit_mask CMCC_CS_e5_present if
                             * present */
    CMCC_CS_EParameter e6;  /* optional; set in bit_mask CMCC_CS_e6_present if
                             * present */
    CMCC_CS_EParameter e7;  /* optional; set in bit_mask CMCC_CS_e7_present if
                             * present */
} CMCC_CS_AOCParameters;

typedef struct CMCC_CS_AOCParmChange {
    CMCC_CS_TimeStamp changeTime;
    CMCC_CS_AOCParameters newParameters;
} CMCC_CS_AOCParmChange;

typedef struct CMCC_CS_Classmark {
    unsigned int    length;
    unsigned char   *value;
} CMCC_CS_Classmark;

typedef struct CMCC_CS_ChangeOfClassmark {
    CMCC_CS_Classmark classmark;
    CMCC_CS_TimeStamp changeTime;
} CMCC_CS_ChangeOfClassmark;

typedef int             CMCC_CS_CallDuration;

typedef enum CMCC_CS_RadioChanRequested {
    CMCC_CS_halfRateChannel = 0,
    CMCC_CS_fullRateChannel = 1,
    CMCC_CS_dualHalfRatePreferred = 2,
    CMCC_CS_dualFullRatePreferred = 3
} CMCC_CS_RadioChanRequested;

typedef enum CMCC_CS_TrafficChannel {
    CMCC_CS_fullRate = 0,
    CMCC_CS_halfRate = 1
} CMCC_CS_TrafficChannel;

typedef struct CMCC_CS_SpeechVersionIdentifier {
    unsigned short  length;
    unsigned char   value[1];
} CMCC_CS_SpeechVersionIdentifier;

typedef struct CMCC_CS_ChangeOfRadioChannel {
    unsigned char   bit_mask;
#       define      CMCC_CS_ChangeOfRadioChannel_speechVersionUsed_present 0x80
    CMCC_CS_TrafficChannel radioChannel;
    CMCC_CS_TimeStamp changeTime;
    CMCC_CS_SpeechVersionIdentifier speechVersionUsed;  /* optional; set in
                                   * bit_mask
                    * CMCC_CS_ChangeOfRadioChannel_speechVersionUsed_present if
                    * present */
} CMCC_CS_ChangeOfRadioChannel;

typedef int             CMCC_CS_CauseForTerm;
#define                     CMCC_CS_normalRelease 0
#define                     CMCC_CS_partialRecord 1
#define                     CMCC_CS_partialRecordCallReestablishment 2
#define                     CMCC_CS_unsuccessfulCallAttempt 3
#define                     CMCC_CS_stableCallAbnormalTermination 4
#define                     CMCC_CS_cAMELInitCallRelease 5
#define                     CMCC_CS_unauthorizedRequestingNetwork 52
#define                     CMCC_CS_unauthorizedLCSClient 53
#define                     CMCC_CS_positionMethodFailure 54
#define                     CMCC_CS_unknownOrUnreachableLCSClient 58

typedef struct CMCC_CS__octet1 {
    unsigned int    length;
    unsigned char   *value;
} CMCC_CS__octet1;

typedef struct CMCC_CS_ManagementExtension {
    unsigned char   bit_mask;
#       define      CMCC_CS_significance_present 0x80
    CMCC_CS__ObjectID identifier;
    ossBoolean      significance;  /* CMCC_CS_significance_present not set in
                                    * bit_mask implies value is FALSE */
    CMCC_CS__octet1 information;
} CMCC_CS_ManagementExtension;

typedef struct CMCC_CS_Diagnostics {
    unsigned short  choice;
#       define      CMCC_CS_gsm0408Cause_chosen 1
#       define      CMCC_CS_gsm0902MapErrorValue_chosen 2
#       define      CMCC_CS_ccittQ767Cause_chosen 3
#       define      CMCC_CS_networkSpecificCause_chosen 4
#       define      CMCC_CS_manufacturerSpecificCause_chosen 5
    union _union {
        int             gsm0408Cause;  /* to choose, set choice to
                                        * CMCC_CS_gsm0408Cause_chosen */
        int             gsm0902MapErrorValue;  /* to choose, set choice to
                                       * CMCC_CS_gsm0902MapErrorValue_chosen */
        int             ccittQ767Cause;  /* to choose, set choice to
                                          * CMCC_CS_ccittQ767Cause_chosen */
        CMCC_CS_ManagementExtension networkSpecificCause;  /* to choose, set
                                   * choice to
                                   * CMCC_CS_networkSpecificCause_chosen */
        CMCC_CS_ManagementExtension manufacturerSpecificCause;  /* to choose,
                                   * set choice to
                                  * CMCC_CS_manufacturerSpecificCause_chosen */
    } u;
} CMCC_CS_Diagnostics;

typedef struct CMCC_CS_CallReference {
    unsigned short  length;
    unsigned char   value[8];
} CMCC_CS_CallReference;

typedef int             CMCC_CS_SequenceNumber;

typedef int             CMCC_CS_ChargeIndicator;
#define                     CMCC_CS_noCharge 0
#define                     CMCC_CS_charge 1

typedef struct CMCC_CS_AdditionalChgInfo {
    unsigned char   bit_mask;
#       define      CMCC_CS_chargeIndicator_present 0x80
#       define      CMCC_CS_chargeParameters_present 0x40
    CMCC_CS_ChargeIndicator chargeIndicator;  /* optional; set in bit_mask
                                               * CMCC_CS_chargeIndicator_present
                                               * if present */
    CMCC_CS__octet1 chargeParameters;  /* optional; set in bit_mask
                                        * CMCC_CS_chargeParameters_present if
                                        * present */
} CMCC_CS_AdditionalChgInfo;

typedef CMCC_CS_AddressString CMCC_CS_ISDNAddressString;

typedef CMCC_CS_ISDNAddressString CMCC_CS_Gsm_SCFAddress;

typedef unsigned int    CMCC_CS_ServiceKey;

typedef struct CMCC_CS_CallReferenceNumber {
    unsigned short  length;
    unsigned char   value[8];
} CMCC_CS_CallReferenceNumber;

typedef CMCC_CS_CallReferenceNumber CMCC_CS_NetworkCallReference;

typedef CMCC_CS_AddressString CMCC_CS_MSCAddress;

typedef enum CMCC_CS_CAMELInitCFIndicator {
    CMCC_CS_noCAMELCallForwarding = 0,
    CMCC_CS_cAMELCallForwarding = 1
} CMCC_CS_CAMELInitCFIndicator;

typedef enum CMCC_CS_DefaultCallHandling {
    CMCC_CS_continueCall = 0,
    CMCC_CS_releaseCall = 1
} CMCC_CS_DefaultCallHandling;

typedef enum CMCC_CS_AiurRequested {
    CMCC_CS_aiur09600BitsPerSecond = 1,
    CMCC_CS_aiur14400BitsPerSecond = 2,
    CMCC_CS_aiur19200BitsPerSecond = 3,
    CMCC_CS_aiur28800BitsPerSecond = 5,
    CMCC_CS_aiur38400BitsPerSecond = 6,
    CMCC_CS_aiur43200BitsPerSecond = 7,
    CMCC_CS_aiur57600BitsPerSecond = 8,
    CMCC_CS_aiur38400BitsPerSecond1 = 9,
    CMCC_CS_aiur38400BitsPerSecond2 = 10,
    CMCC_CS_aiur38400BitsPerSecond3 = 11,
    CMCC_CS_aiur38400BitsPerSecond4 = 12
} CMCC_CS_AiurRequested;

typedef unsigned char   CMCC_CS_LevelOfCAMELService;
#define                     CMCC_CS_basic 0x80
#define                     CMCC_CS_callDurationSupervision 0x40
#define                     CMCC_CS_onlineCharging 0x20

typedef struct CMCC_CS_FreeFormatData {
    unsigned short  length;
    unsigned char   value[160];
} CMCC_CS_FreeFormatData;

typedef CMCC_CS_BCDDirectoryNumber CMCC_CS_DestinationRoutingAddress;

typedef CMCC_CS_DestinationRoutingAddress CMCC_CS_CAMELDestinationNumber;

typedef int             CMCC_CS_DataVolume;

typedef unsigned char   CMCC_CS_ChangeFlags;
#define                     CMCC_CS_callingPartyNumberModified 0x80
#define                     CMCC_CS_callingPartyCategoryModified 0x40
#define                     CMCC_CS_originalCalledPartyNumberModified 0x20
#define                     CMCC_CS_genericNumbersModified 0x10
#define                     CMCC_CS_redirectingPartyNumberModified 0x08
#define                     CMCC_CS_redirectionCounterModified 0x04

typedef struct CMCC_CS_Category {
    unsigned short  length;
    unsigned char   value[1];
} CMCC_CS_Category;

typedef CMCC_CS_Category CMCC_CS_CallingPartyCategory;

typedef CMCC_CS_BCDDirectoryNumber CMCC_CS_OriginalCalledNumber;

typedef CMCC_CS_BCDDirectoryNumber CMCC_CS_RedirectingNumber;

typedef unsigned char   CMCC_CS_NumberOfForwarding;

typedef struct CMCC_CS_CAMELModificationParameters {
    unsigned char   bit_mask;
#       define      CMCC_CS_CAMELModificationParameters_callingPartyNumber_present 0x80
#       define      CMCC_CS_callingPartyCategory_present 0x40
#       define      CMCC_CS_originalCalledPartyNumber_present 0x20
#       define      CMCC_CS_genericNumbers_present 0x10
#       define      CMCC_CS_redirectingPartyNumber_present 0x08
#       define      CMCC_CS_redirectionCounter_present 0x04
    CMCC_CS_CallingNumber callingPartyNumber;  /* optional; set in bit_mask
            * CMCC_CS_CAMELModificationParameters_callingPartyNumber_present if
            * present */
    CMCC_CS_CallingPartyCategory callingPartyCategory;  /* optional; set in
                                   * bit_mask
                                   * CMCC_CS_callingPartyCategory_present if
                                   * present */
    CMCC_CS_OriginalCalledNumber originalCalledPartyNumber;  /* optional; set in
                                   * bit_mask
                                   * CMCC_CS_originalCalledPartyNumber_present
                                   * if present */
    struct CMCC_CS_GenericNumbers_ *genericNumbers;  /* optional; set in
                                   * bit_mask CMCC_CS_genericNumbers_present if
                                   * present */
    CMCC_CS_RedirectingNumber redirectingPartyNumber;  /* optional; set in
                                   * bit_mask
                                   * CMCC_CS_redirectingPartyNumber_present if
                                   * present */
    CMCC_CS_NumberOfForwarding redirectionCounter;  /* optional; set in bit_mask
                                        * CMCC_CS_redirectionCounter_present if
                                        * present */
} CMCC_CS_CAMELModificationParameters;

typedef struct CMCC_CS_ChangedParameters {
    unsigned char   bit_mask;
#       define      CMCC_CS_changeList_present 0x80
    CMCC_CS_ChangeFlags changeFlags;
    CMCC_CS_CAMELModificationParameters changeList;  /* optional; set in
                                   * bit_mask CMCC_CS_changeList_present if
                                   * present */
} CMCC_CS_ChangedParameters;

typedef struct CMCC_CS_CAMELInformation {
    unsigned int    bit_mask;
#       define      CMCC_CS_cAMELDestinationNumber_present 0x80000000
#       define      CMCC_CS_CAMELInformation_connectedNumber_present 0x40000000
#       define      CMCC_CS_CAMELInformation_roamingNumber_present 0x20000000
#       define      CMCC_CS_CAMELInformation_mscOutgoingROUTE_present 0x10000000
#       define      CMCC_CS_CAMELInformation_seizureTime_present 0x08000000
#       define      CMCC_CS_CAMELInformation_answerTime_present 0x04000000
#       define      CMCC_CS_CAMELInformation_releaseTime_present 0x02000000
#       define      CMCC_CS_CAMELInformation_callDuration_present 0x01000000
#       define      CMCC_CS_dataVolume_present 0x00800000
#       define      CMCC_CS_CAMELInformation_cAMELInitCFIndicator_present 0x00400000
#       define      CMCC_CS_CAMELInformation_causeForTerm_present 0x00200000
#       define      CMCC_CS_cAMELModification_present 0x00100000
#       define      CMCC_CS_CAMELInformation_freeFormatData_present 0x00080000
#       define      CMCC_CS_CAMELInformation_diagnostics_present 0x00040000
#       define      CMCC_CS_CAMELInformation_freeFormatDataAppend_present 0x00020000
#       define      CMCC_CS_CAMELInformation_freeFormatData_2_present 0x00010000
#       define      CMCC_CS_CAMELInformation_freeFormatDataAppend_2_present 0x00008000
    CMCC_CS_CAMELDestinationNumber cAMELDestinationNumber;  /* optional; set in
                                   * bit_mask
                                   * CMCC_CS_cAMELDestinationNumber_present if
                                   * present */
    CMCC_CS_ConnectedNumber connectedNumber;  /* optional; set in bit_mask
                          * CMCC_CS_CAMELInformation_connectedNumber_present if
                          * present */
    CMCC_CS_RoamingNumber roamingNumber;  /* optional; set in bit_mask
                            * CMCC_CS_CAMELInformation_roamingNumber_present if
                            * present */
    CMCC_CS_ROUTE   mscOutgoingROUTE;  /* optional; set in bit_mask
                         * CMCC_CS_CAMELInformation_mscOutgoingROUTE_present if
                         * present */
    CMCC_CS_TimeStamp seizureTime;  /* optional; set in bit_mask
                              * CMCC_CS_CAMELInformation_seizureTime_present if
                              * present */
    CMCC_CS_TimeStamp answerTime;  /* optional; set in bit_mask
                               * CMCC_CS_CAMELInformation_answerTime_present if
                               * present */
    CMCC_CS_TimeStamp releaseTime;  /* optional; set in bit_mask
                              * CMCC_CS_CAMELInformation_releaseTime_present if
                              * present */
    CMCC_CS_CallDuration callDuration;  /* optional; set in bit_mask
                             * CMCC_CS_CAMELInformation_callDuration_present if
                             * present */
    CMCC_CS_DataVolume dataVolume;  /* optional; set in bit_mask
                                     * CMCC_CS_dataVolume_present if present */
    CMCC_CS_CAMELInitCFIndicator cAMELInitCFIndicator;  /* optional; set in
                                   * bit_mask
                     * CMCC_CS_CAMELInformation_cAMELInitCFIndicator_present if
                     * present */
    CMCC_CS_CauseForTerm causeForTerm;  /* optional; set in bit_mask
                             * CMCC_CS_CAMELInformation_causeForTerm_present if
                             * present */
    CMCC_CS_ChangedParameters cAMELModification;  /* optional; set in bit_mask
                                         * CMCC_CS_cAMELModification_present if
                                         * present */
    CMCC_CS_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                           * CMCC_CS_CAMELInformation_freeFormatData_present if
                           * present */
    CMCC_CS_Diagnostics diagnostics;  /* optional; set in bit_mask
                              * CMCC_CS_CAMELInformation_diagnostics_present if
                              * present */
    ossBoolean      freeFormatDataAppend;  /* optional; set in bit_mask
                     * CMCC_CS_CAMELInformation_freeFormatDataAppend_present if
                     * present */
    CMCC_CS_FreeFormatData freeFormatData_2;  /* optional; set in bit_mask
                         * CMCC_CS_CAMELInformation_freeFormatData_2_present if
                         * present */
    ossBoolean      freeFormatDataAppend_2;  /* optional; set in bit_mask
                   * CMCC_CS_CAMELInformation_freeFormatDataAppend_2_present if
                   * present */
} CMCC_CS_CAMELInformation;

typedef enum CMCC_CS_SystemType {
    CMCC_CS_unknown = 0,
    CMCC_CS_iuUTRAN = 1,
    CMCC_CS_gERAN = 2
} CMCC_CS_SystemType;

typedef enum CMCC_CS_PartialRecordType {
    CMCC_CS_timeLimit = 0,
    CMCC_CS_serviceChange = 1,
    CMCC_CS_locationChange = 2,
    CMCC_CS_classmarkChange = 3,
    CMCC_CS_aocParmChange = 4,
    CMCC_CS_radioChannelChange = 5,
    CMCC_CS_hSCSDParmChange = 6,
    CMCC_CS_changeOfCAMELDestination = 7,
    CMCC_CS_firstHotBill = 20,
    CMCC_CS_severalSSOperationBill = 21
} CMCC_CS_PartialRecordType;

typedef enum CMCC_CS_GuaranteedBitRate {
    CMCC_CS_gBR14400BitsPerSecond = 1,
    CMCC_CS_gBR28800BitsPerSecond = 2,
    CMCC_CS_gBR32000BitsPerSecond = 3,
    CMCC_CS_gBR33600BitsPerSecond = 4,
    CMCC_CS_gBR56000BitsPerSecond = 5,
    CMCC_CS_gBR57600BitsPerSecond = 6,
    CMCC_CS_gBR64000BitsPerSecond = 7
} CMCC_CS_GuaranteedBitRate;

typedef enum CMCC_CS_MaximumBitRate {
    CMCC_CS_mBR14400BitsPerSecond = 1,
    CMCC_CS_mBR28800BitsPerSecond = 2,
    CMCC_CS_mBR32000BitsPerSecond = 3,
    CMCC_CS_mBR33600BitsPerSecond = 4,
    CMCC_CS_mBR56000BitsPerSecond = 5,
    CMCC_CS_mBR57600BitsPerSecond = 6
} CMCC_CS_MaximumBitRate;

typedef enum CMCC_CS_ModemType {
    CMCC_CS_none_modem = 0,
    CMCC_CS_modem_v21 = 1,
    CMCC_CS_modem_v22 = 2,
    CMCC_CS_modem_v22_bis = 3,
    CMCC_CS_modem_v23 = 4,
    CMCC_CS_modem_v26_ter = 5,
    CMCC_CS_modem_v32 = 6,
    CMCC_CS_modem_undef_interface = 7,
    CMCC_CS_modem_autobauding1 = 8,
    CMCC_CS_modem_v34 = 33
} CMCC_CS_ModemType;

typedef struct CMCC_CS_Classmark3 {
    unsigned short  length;
    unsigned char   value[2];
} CMCC_CS_Classmark3;

typedef enum CMCC_CS_ChargedParty {
    CMCC_CS_callingParty = 0,
    CMCC_CS_calledParty = 1
} CMCC_CS_ChargedParty;

typedef struct CMCC_CS_ChargeAreaCode {
    unsigned short  length;
    unsigned char   value[3];
} CMCC_CS_ChargeAreaCode;

typedef unsigned short  CMCC_CS_MSCCIC;

typedef struct CMCC_CS_RNCorBSCId {
    unsigned short  length;
    unsigned char   value[3];
} CMCC_CS_RNCorBSCId;

typedef struct CMCC_CS_MSCId {
    unsigned short  length;
    unsigned char   value[3];
} CMCC_CS_MSCId;

typedef struct CMCC_CS_SubscriberEmlppPriority {
    unsigned short  length;
    unsigned char   value[1];
} CMCC_CS_SubscriberEmlppPriority;

typedef struct CMCC_CS_EASubscriberInfo {
    unsigned short  length;
    unsigned char   value[3];
} CMCC_CS_EASubscriberInfo;

typedef struct CMCC_CS_SelectedCIC {
    unsigned short  length;
    unsigned char   value[3];
} CMCC_CS_SelectedCIC;

typedef enum CMCC_CS_PortedFlag {
    CMCC_CS_numberNotPorted = 0,
    CMCC_CS_numberPorted = 1
} CMCC_CS_PortedFlag;

typedef struct CMCC_CS_GAI {
    unsigned short  length;
    unsigned char   value[7];
} CMCC_CS_GAI;

typedef struct CMCC_CS_ChangeOfglobalAreaID {
    CMCC_CS_GAI     location;
    CMCC_CS_TimeStamp changeTime;
} CMCC_CS_ChangeOfglobalAreaID;

typedef struct CMCC_CS_SubscriberCategory {
    unsigned short  length;
    unsigned char   value[1];
} CMCC_CS_SubscriberCategory;

typedef struct CMCC_CS_MCCMNC {
    unsigned short  length;
    unsigned char   value[3];
} CMCC_CS_MCCMNC;

typedef enum CMCC_CS_CUGOutgoingAccessIndicator {
    CMCC_CS_notCUGCall = 0,
    CMCC_CS_cUGCall = 1
} CMCC_CS_CUGOutgoingAccessIndicator;

typedef struct CMCC_CS_CUGInterlockCode {
    unsigned short  length;
    unsigned char   value[4];
} CMCC_CS_CUGInterlockCode;

typedef enum CMCC_CS_CUGOutgoingAccessUsed {
    CMCC_CS_CUGOutgoingAccessUsed_callInTheSameCUGGroup = 0,
    CMCC_CS_CUGOutgoingAccessUsed_callNotInTheSameCUGGroup = 1
} CMCC_CS_CUGOutgoingAccessUsed;

typedef struct CMCC_CS_CUGIndex {
    unsigned short  length;
    unsigned char   value[2];
} CMCC_CS_CUGIndex;

typedef Nulltype        CMCC_CS_InteractionWithIP;

typedef int             CMCC_CS_HotBillingTag;
#define                     CMCC_CS_noHotBilling 0
#define                     CMCC_CS_hotBilling 1

typedef struct CMCC_CS__seqof1_ {
    struct CMCC_CS__seqof1_ *next;
    CMCC_CS_LocationChange value;
} *CMCC_CS__seqof1;

typedef struct CMCC_CS__seqof2_ {
    struct CMCC_CS__seqof2_ *next;
    CMCC_CS_ChangeOfService value;
} *CMCC_CS__seqof2;

typedef struct CMCC_CS__seqof3_ {
    struct CMCC_CS__seqof3_ *next;
    CMCC_CS_SuppServiceUsed value;
} *CMCC_CS__seqof3;

typedef struct CMCC_CS__seqof4_ {
    struct CMCC_CS__seqof4_ *next;
    CMCC_CS_AOCParmChange value;
} *CMCC_CS__seqof4;

typedef struct CMCC_CS__seqof5_ {
    struct CMCC_CS__seqof5_ *next;
    CMCC_CS_CAMELInformation value;
} *CMCC_CS__seqof5;

typedef struct CMCC_CS__seqof6_ {
    struct CMCC_CS__seqof6_ *next;
    CMCC_CS_ChangeOfglobalAreaID value;
} *CMCC_CS__seqof6;

typedef struct CMCC_CS_MOCallRecord {
    unsigned char   bit_mask[12];
#       define      CMCC_CS_MOCallRecord_recordType_present 0x80
#       define      CMCC_CS_MOCallRecord_recordType_present_byte 0
#       define      CMCC_CS_MOCallRecord_servedIMSI_present 0x40
#       define      CMCC_CS_MOCallRecord_servedIMSI_present_byte 0
#       define      CMCC_CS_MOCallRecord_servedIMEI_present 0x20
#       define      CMCC_CS_MOCallRecord_servedIMEI_present_byte 0
#       define      CMCC_CS_MOCallRecord_servedMSISDN_present 0x10
#       define      CMCC_CS_MOCallRecord_servedMSISDN_present_byte 0
#       define      CMCC_CS_MOCallRecord_callingNumber_present 0x08
#       define      CMCC_CS_MOCallRecord_callingNumber_present_byte 0
#       define      CMCC_CS_MOCallRecord_calledNumber_present 0x04
#       define      CMCC_CS_MOCallRecord_calledNumber_present_byte 0
#       define      CMCC_CS_translatedNumber_present 0x02
#       define      CMCC_CS_translatedNumber_present_byte 0
#       define      CMCC_CS_MOCallRecord_connectedNumber_present 0x01
#       define      CMCC_CS_MOCallRecord_connectedNumber_present_byte 0
#       define      CMCC_CS_MOCallRecord_roamingNumber_present 0x80
#       define      CMCC_CS_MOCallRecord_roamingNumber_present_byte 1
#       define      CMCC_CS_MOCallRecord_recordingEntity_present 0x40
#       define      CMCC_CS_MOCallRecord_recordingEntity_present_byte 1
#       define      CMCC_CS_MOCallRecord_mscIncomingROUTE_present 0x20
#       define      CMCC_CS_MOCallRecord_mscIncomingROUTE_present_byte 1
#       define      CMCC_CS_MOCallRecord_mscOutgoingROUTE_present 0x10
#       define      CMCC_CS_MOCallRecord_mscOutgoingROUTE_present_byte 1
#       define      CMCC_CS_MOCallRecord_location_present 0x08
#       define      CMCC_CS_MOCallRecord_location_present_byte 1
#       define      CMCC_CS_MOCallRecord_changeOfLocation_present 0x04
#       define      CMCC_CS_MOCallRecord_changeOfLocation_present_byte 1
#       define      CMCC_CS_MOCallRecord_basicService_present 0x02
#       define      CMCC_CS_MOCallRecord_basicService_present_byte 1
#       define      CMCC_CS_MOCallRecord_transparencyIndicator_present 0x01
#       define      CMCC_CS_MOCallRecord_transparencyIndicator_present_byte 1
#       define      CMCC_CS_MOCallRecord_changeOfService_present 0x80
#       define      CMCC_CS_MOCallRecord_changeOfService_present_byte 2
#       define      CMCC_CS_MOCallRecord_supplServicesUsed_present 0x40
#       define      CMCC_CS_MOCallRecord_supplServicesUsed_present_byte 2
#       define      CMCC_CS_MOCallRecord_aocParameters_present 0x20
#       define      CMCC_CS_MOCallRecord_aocParameters_present_byte 2
#       define      CMCC_CS_MOCallRecord_changeOfAOCParms_present 0x10
#       define      CMCC_CS_MOCallRecord_changeOfAOCParms_present_byte 2
#       define      CMCC_CS_MOCallRecord_msClassmark_present 0x08
#       define      CMCC_CS_MOCallRecord_msClassmark_present_byte 2
#       define      CMCC_CS_MOCallRecord_changeOfClassmark_present 0x04
#       define      CMCC_CS_MOCallRecord_changeOfClassmark_present_byte 2
#       define      CMCC_CS_MOCallRecord_seizureTime_present 0x02
#       define      CMCC_CS_MOCallRecord_seizureTime_present_byte 2
#       define      CMCC_CS_MOCallRecord_answerTime_present 0x01
#       define      CMCC_CS_MOCallRecord_answerTime_present_byte 2
#       define      CMCC_CS_MOCallRecord_releaseTime_present 0x80
#       define      CMCC_CS_MOCallRecord_releaseTime_present_byte 3
#       define      CMCC_CS_MOCallRecord_callDuration_present 0x40
#       define      CMCC_CS_MOCallRecord_callDuration_present_byte 3
#       define      CMCC_CS_MOCallRecord_radioChanRequested_present 0x20
#       define      CMCC_CS_MOCallRecord_radioChanRequested_present_byte 3
#       define      CMCC_CS_MOCallRecord_radioChanUsed_present 0x10
#       define      CMCC_CS_MOCallRecord_radioChanUsed_present_byte 3
#       define      CMCC_CS_MOCallRecord_changeOfRadioChan_present 0x08
#       define      CMCC_CS_MOCallRecord_changeOfRadioChan_present_byte 3
#       define      CMCC_CS_MOCallRecord_causeForTerm_present 0x04
#       define      CMCC_CS_MOCallRecord_causeForTerm_present_byte 3
#       define      CMCC_CS_MOCallRecord_diagnostics_present 0x02
#       define      CMCC_CS_MOCallRecord_diagnostics_present_byte 3
#       define      CMCC_CS_MOCallRecord_callReference_present 0x01
#       define      CMCC_CS_MOCallRecord_callReference_present_byte 3
#       define      CMCC_CS_MOCallRecord_sequenceNumber_present 0x80
#       define      CMCC_CS_MOCallRecord_sequenceNumber_present_byte 4
#       define      CMCC_CS_MOCallRecord_additionalChgInfo_present 0x40
#       define      CMCC_CS_MOCallRecord_additionalChgInfo_present_byte 4
#       define      CMCC_CS_MOCallRecord_recordExtensions_present 0x20
#       define      CMCC_CS_MOCallRecord_recordExtensions_present_byte 4
#       define      CMCC_CS_MOCallRecord_gsm_SCFAddress_present 0x10
#       define      CMCC_CS_MOCallRecord_gsm_SCFAddress_present_byte 4
#       define      CMCC_CS_MOCallRecord_serviceKey_present 0x08
#       define      CMCC_CS_MOCallRecord_serviceKey_present_byte 4
#       define      CMCC_CS_MOCallRecord_networkCallReference_present 0x04
#       define      CMCC_CS_MOCallRecord_networkCallReference_present_byte 4
#       define      CMCC_CS_MOCallRecord_mSCAddress_present 0x02
#       define      CMCC_CS_MOCallRecord_mSCAddress_present_byte 4
#       define      CMCC_CS_MOCallRecord_cAMELInitCFIndicator_present 0x01
#       define      CMCC_CS_MOCallRecord_cAMELInitCFIndicator_present_byte 4
#       define      CMCC_CS_MOCallRecord_defaultCallHandling_present 0x80
#       define      CMCC_CS_MOCallRecord_defaultCallHandling_present_byte 5
#       define      CMCC_CS_MOCallRecord_fnur_present 0x40
#       define      CMCC_CS_MOCallRecord_fnur_present_byte 5
#       define      CMCC_CS_MOCallRecord_aiurRequested_present 0x20
#       define      CMCC_CS_MOCallRecord_aiurRequested_present_byte 5
#       define      CMCC_CS_MOCallRecord_speechVersionSupported_present 0x10
#       define      CMCC_CS_MOCallRecord_speechVersionSupported_present_byte 5
#       define      CMCC_CS_MOCallRecord_speechVersionUsed_present 0x08
#       define      CMCC_CS_MOCallRecord_speechVersionUsed_present_byte 5
#       define      CMCC_CS_MOCallRecord_numberOfDPEncountered_present 0x04
#       define      CMCC_CS_MOCallRecord_numberOfDPEncountered_present_byte 5
#       define      CMCC_CS_MOCallRecord_levelOfCAMELService_present 0x02
#       define      CMCC_CS_MOCallRecord_levelOfCAMELService_present_byte 5
#       define      CMCC_CS_MOCallRecord_freeFormatData_present 0x01
#       define      CMCC_CS_MOCallRecord_freeFormatData_present_byte 5
#       define      CMCC_CS_MOCallRecord_cAMELCallLegInformation_present 0x80
#       define      CMCC_CS_MOCallRecord_cAMELCallLegInformation_present_byte 6
#       define      CMCC_CS_MOCallRecord_freeFormatDataAppend_present 0x40
#       define      CMCC_CS_MOCallRecord_freeFormatDataAppend_present_byte 6
#       define      CMCC_CS_MOCallRecord_defaultCallHandling_2_present 0x20
#       define      CMCC_CS_MOCallRecord_defaultCallHandling_2_present_byte 6
#       define      CMCC_CS_MOCallRecord_gsm_SCFAddress_2_present 0x10
#       define      CMCC_CS_MOCallRecord_gsm_SCFAddress_2_present_byte 6
#       define      CMCC_CS_MOCallRecord_serviceKey_2_present 0x08
#       define      CMCC_CS_MOCallRecord_serviceKey_2_present_byte 6
#       define      CMCC_CS_MOCallRecord_freeFormatData_2_present 0x04
#       define      CMCC_CS_MOCallRecord_freeFormatData_2_present_byte 6
#       define      CMCC_CS_MOCallRecord_freeFormatDataAppend_2_present 0x02
#       define      CMCC_CS_MOCallRecord_freeFormatDataAppend_2_present_byte 6
#       define      CMCC_CS_MOCallRecord_systemType_present 0x01
#       define      CMCC_CS_MOCallRecord_systemType_present_byte 6
#       define      CMCC_CS_MOCallRecord_rateIndication_present 0x80
#       define      CMCC_CS_MOCallRecord_rateIndication_present_byte 7
#       define      CMCC_CS_MOCallRecord_partialRecordType_present 0x40
#       define      CMCC_CS_MOCallRecord_partialRecordType_present_byte 7
#       define      CMCC_CS_MOCallRecord_guaranteedBitRate_present 0x20
#       define      CMCC_CS_MOCallRecord_guaranteedBitRate_present_byte 7
#       define      CMCC_CS_MOCallRecord_maximumBitRate_present 0x10
#       define      CMCC_CS_MOCallRecord_maximumBitRate_present_byte 7
#       define      CMCC_CS_MOCallRecord_modemType_present 0x08
#       define      CMCC_CS_MOCallRecord_modemType_present_byte 7
#       define      CMCC_CS_MOCallRecord_classmark3_present 0x04
#       define      CMCC_CS_MOCallRecord_classmark3_present_byte 7
#       define      CMCC_CS_MOCallRecord_chargedParty_present 0x02
#       define      CMCC_CS_MOCallRecord_chargedParty_present_byte 7
#       define      CMCC_CS_MOCallRecord_originalCalledNumber_present 0x01
#       define      CMCC_CS_MOCallRecord_originalCalledNumber_present_byte 7
#       define      CMCC_CS_MOCallRecord_callingChargeAreaCode_present 0x80
#       define      CMCC_CS_MOCallRecord_callingChargeAreaCode_present_byte 8
#       define      CMCC_CS_MOCallRecord_calledChargeAreaCode_present 0x40
#       define      CMCC_CS_MOCallRecord_calledChargeAreaCode_present_byte 8
#       define      CMCC_CS_MOCallRecord_mscOutgoingCircuit_present 0x20
#       define      CMCC_CS_MOCallRecord_mscOutgoingCircuit_present_byte 8
#       define      CMCC_CS_MOCallRecord_orgRNCorBSCId_present 0x10
#       define      CMCC_CS_MOCallRecord_orgRNCorBSCId_present_byte 8
#       define      CMCC_CS_MOCallRecord_orgMSCId_present 0x08
#       define      CMCC_CS_MOCallRecord_orgMSCId_present_byte 8
#       define      CMCC_CS_MOCallRecord_callEmlppPriority_present 0x04
#       define      CMCC_CS_MOCallRecord_callEmlppPriority_present_byte 8
#       define      CMCC_CS_callerDefaultEmlppPriority_present 0x02
#       define      CMCC_CS_callerDefaultEmlppPriority_present_byte 8
#       define      CMCC_CS_MOCallRecord_eaSubscriberInfo_present 0x01
#       define      CMCC_CS_MOCallRecord_eaSubscriberInfo_present_byte 8
#       define      CMCC_CS_MOCallRecord_selectedCIC_present 0x80
#       define      CMCC_CS_MOCallRecord_selectedCIC_present_byte 9
#       define      CMCC_CS_MOCallRecord_optimalRoutingFlag_present 0x40
#       define      CMCC_CS_MOCallRecord_optimalRoutingFlag_present_byte 9
#       define      CMCC_CS_optimalRoutingLateForwardFlag_present 0x20
#       define      CMCC_CS_optimalRoutingLateForwardFlag_present_byte 9
#       define      CMCC_CS_optimalRoutingEarlyForwardFlag_present 0x10
#       define      CMCC_CS_optimalRoutingEarlyForwardFlag_present_byte 9
#       define      CMCC_CS_MOCallRecord_portedflag_present 0x08
#       define      CMCC_CS_MOCallRecord_portedflag_present_byte 9
#       define      CMCC_CS_calledIMSI_present 0x04
#       define      CMCC_CS_calledIMSI_present_byte 9
#       define      CMCC_CS_MOCallRecord_globalAreaID_present 0x02
#       define      CMCC_CS_MOCallRecord_globalAreaID_present_byte 9
#       define      CMCC_CS_MOCallRecord_changeOfglobalAreaID_present 0x01
#       define      CMCC_CS_MOCallRecord_changeOfglobalAreaID_present_byte 9
#       define      CMCC_CS_MOCallRecord_subscriberCategory_present 0x80
#       define      CMCC_CS_MOCallRecord_subscriberCategory_present_byte 10
#       define      CMCC_CS_MOCallRecord_firstmccmnc_present 0x40
#       define      CMCC_CS_MOCallRecord_firstmccmnc_present_byte 10
#       define      CMCC_CS_MOCallRecord_intermediatemccmnc_present 0x20
#       define      CMCC_CS_MOCallRecord_intermediatemccmnc_present_byte 10
#       define      CMCC_CS_MOCallRecord_lastmccmnc_present 0x10
#       define      CMCC_CS_MOCallRecord_lastmccmnc_present_byte 10
#       define      CMCC_CS_MOCallRecord_cUGOutgoingAccessIndicator_present 0x08
#       define      CMCC_CS_MOCallRecord_cUGOutgoingAccessIndicator_present_byte 10
#       define      CMCC_CS_MOCallRecord_cUGInterlockCode_present 0x04
#       define      CMCC_CS_MOCallRecord_cUGInterlockCode_present_byte 10
#       define      CMCC_CS_cUGOutgoingAccessUsed_present 0x02
#       define      CMCC_CS_cUGOutgoingAccessUsed_present_byte 10
#       define      CMCC_CS_MOCallRecord_cUGIndex_present 0x01
#       define      CMCC_CS_MOCallRecord_cUGIndex_present_byte 10
#       define      CMCC_CS_interactionWithIP_present 0x80
#       define      CMCC_CS_interactionWithIP_present_byte 11
#       define      CMCC_CS_MOCallRecord_hotBillingTag_present 0x40
#       define      CMCC_CS_MOCallRecord_hotBillingTag_present_byte 11
    CMCC_CS_CallEventRecordType recordType;  /* optional; set in bit_mask
                                   * CMCC_CS_MOCallRecord_recordType_present if
                                   * present */
    CMCC_CS_IMSI    servedIMSI;  /* optional; set in bit_mask
                                  * CMCC_CS_MOCallRecord_servedIMSI_present if
                                  * present */
    CMCC_CS_IMEI    servedIMEI;  /* optional; set in bit_mask
                                  * CMCC_CS_MOCallRecord_servedIMEI_present if
                                  * present */
    CMCC_CS_MSISDN  servedMSISDN;  /* optional; set in bit_mask
                                    * CMCC_CS_MOCallRecord_servedMSISDN_present
                                    * if present */
    CMCC_CS_CallingNumber callingNumber;  /* optional; set in bit_mask
                                * CMCC_CS_MOCallRecord_callingNumber_present if
                                * present */
    CMCC_CS_CalledNumber calledNumber;  /* optional; set in bit_mask
                                 * CMCC_CS_MOCallRecord_calledNumber_present if
                                 * present */
    CMCC_CS_TranslatedNumber translatedNumber;  /* optional; set in bit_mask
                                          * CMCC_CS_translatedNumber_present if
                                          * present */
    CMCC_CS_ConnectedNumber connectedNumber;  /* optional; set in bit_mask
                              * CMCC_CS_MOCallRecord_connectedNumber_present if
                              * present */
    CMCC_CS_RoamingNumber roamingNumber;  /* optional; set in bit_mask
                                * CMCC_CS_MOCallRecord_roamingNumber_present if
                                * present */
    CMCC_CS_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                              * CMCC_CS_MOCallRecord_recordingEntity_present if
                              * present */
    CMCC_CS_ROUTE   mscIncomingROUTE;  /* optional; set in bit_mask
                             * CMCC_CS_MOCallRecord_mscIncomingROUTE_present if
                             * present */
    CMCC_CS_ROUTE   mscOutgoingROUTE;  /* optional; set in bit_mask
                             * CMCC_CS_MOCallRecord_mscOutgoingROUTE_present if
                             * present */
    CMCC_CS_LocationAreaAndCell location;  /* optional; set in bit_mask
                                     * CMCC_CS_MOCallRecord_location_present if
                                     * present */
    struct CMCC_CS__seqof1_ *changeOfLocation;  /* optional; set in bit_mask
                             * CMCC_CS_MOCallRecord_changeOfLocation_present if
                             * present */
    CMCC_CS_BasicServiceCode basicService;  /* optional; set in bit_mask
                                 * CMCC_CS_MOCallRecord_basicService_present if
                                 * present */
    CMCC_CS_TransparencyInd transparencyIndicator;  /* optional; set in bit_mask
                        * CMCC_CS_MOCallRecord_transparencyIndicator_present if
                        * present */
    struct CMCC_CS__seqof2_ *changeOfService;  /* optional; set in bit_mask
                              * CMCC_CS_MOCallRecord_changeOfService_present if
                              * present */
    struct CMCC_CS__seqof3_ *supplServicesUsed;  /* optional; set in bit_mask
                            * CMCC_CS_MOCallRecord_supplServicesUsed_present if
                            * present */
    CMCC_CS_AOCParameters aocParameters;  /* optional; set in bit_mask
                                * CMCC_CS_MOCallRecord_aocParameters_present if
                                * present */
    struct CMCC_CS__seqof4_ *changeOfAOCParms;  /* optional; set in bit_mask
                             * CMCC_CS_MOCallRecord_changeOfAOCParms_present if
                             * present */
    CMCC_CS_Classmark msClassmark;  /* optional; set in bit_mask
                                     * CMCC_CS_MOCallRecord_msClassmark_present
                                     * if present */
    CMCC_CS_ChangeOfClassmark changeOfClassmark;  /* optional; set in bit_mask
                            * CMCC_CS_MOCallRecord_changeOfClassmark_present if
                            * present */
    CMCC_CS_TimeStamp seizureTime;  /* optional; set in bit_mask
                                     * CMCC_CS_MOCallRecord_seizureTime_present
                                     * if present */
    CMCC_CS_TimeStamp answerTime;  /* optional; set in bit_mask
                                    * CMCC_CS_MOCallRecord_answerTime_present if
                                    * present */
    CMCC_CS_TimeStamp releaseTime;  /* optional; set in bit_mask
                                     * CMCC_CS_MOCallRecord_releaseTime_present
                                     * if present */
    CMCC_CS_CallDuration callDuration;  /* optional; set in bit_mask
                                 * CMCC_CS_MOCallRecord_callDuration_present if
                                 * present */
    CMCC_CS_RadioChanRequested radioChanRequested;  /* optional; set in bit_mask
                           * CMCC_CS_MOCallRecord_radioChanRequested_present if
                           * present */
    CMCC_CS_TrafficChannel radioChanUsed;  /* optional; set in bit_mask
                                * CMCC_CS_MOCallRecord_radioChanUsed_present if
                                * present */
    CMCC_CS_ChangeOfRadioChannel changeOfRadioChan;  /* optional; set in
                                   * bit_mask
                            * CMCC_CS_MOCallRecord_changeOfRadioChan_present if
                            * present */
    CMCC_CS_CauseForTerm causeForTerm;  /* optional; set in bit_mask
                                 * CMCC_CS_MOCallRecord_causeForTerm_present if
                                 * present */
    CMCC_CS_Diagnostics diagnostics;  /* optional; set in bit_mask
                                  * CMCC_CS_MOCallRecord_diagnostics_present if
                                  * present */
    CMCC_CS_CallReference callReference;  /* optional; set in bit_mask
                                * CMCC_CS_MOCallRecord_callReference_present if
                                * present */
    CMCC_CS_SequenceNumber sequenceNumber;  /* optional; set in bit_mask
                               * CMCC_CS_MOCallRecord_sequenceNumber_present if
                               * present */
    CMCC_CS_AdditionalChgInfo additionalChgInfo;  /* optional; set in bit_mask
                            * CMCC_CS_MOCallRecord_additionalChgInfo_present if
                            * present */
    struct CMCC_CS_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                             * CMCC_CS_MOCallRecord_recordExtensions_present if
                             * present */
    CMCC_CS_Gsm_SCFAddress gsm_SCFAddress;  /* optional; set in bit_mask
                               * CMCC_CS_MOCallRecord_gsm_SCFAddress_present if
                               * present */
    CMCC_CS_ServiceKey serviceKey;  /* optional; set in bit_mask
                                     * CMCC_CS_MOCallRecord_serviceKey_present
                                     * if present */
    CMCC_CS_NetworkCallReference networkCallReference;  /* optional; set in
                                   * bit_mask
                         * CMCC_CS_MOCallRecord_networkCallReference_present if
                         * present */
    CMCC_CS_MSCAddress mSCAddress;  /* optional; set in bit_mask
                                     * CMCC_CS_MOCallRecord_mSCAddress_present
                                     * if present */
    CMCC_CS_CAMELInitCFIndicator cAMELInitCFIndicator;  /* optional; set in
                                   * bit_mask
                         * CMCC_CS_MOCallRecord_cAMELInitCFIndicator_present if
                         * present */
    CMCC_CS_DefaultCallHandling defaultCallHandling;  /* optional; set in
                                   * bit_mask
                          * CMCC_CS_MOCallRecord_defaultCallHandling_present if
                          * present */
    CMCC_CS_Fnur    fnur;  /* optional; set in bit_mask
                            * CMCC_CS_MOCallRecord_fnur_present if present */
    CMCC_CS_AiurRequested aiurRequested;  /* optional; set in bit_mask
                                * CMCC_CS_MOCallRecord_aiurRequested_present if
                                * present */
    CMCC_CS_SpeechVersionIdentifier speechVersionSupported;  /* optional; set in
                                   * bit_mask
                       * CMCC_CS_MOCallRecord_speechVersionSupported_present if
                       * present */
    CMCC_CS_SpeechVersionIdentifier speechVersionUsed;  /* optional; set in
                                   * bit_mask
                            * CMCC_CS_MOCallRecord_speechVersionUsed_present if
                            * present */
    int             numberOfDPEncountered;  /* optional; set in bit_mask
                        * CMCC_CS_MOCallRecord_numberOfDPEncountered_present if
                        * present */
    CMCC_CS_LevelOfCAMELService levelOfCAMELService;  /* optional; set in
                                   * bit_mask
                          * CMCC_CS_MOCallRecord_levelOfCAMELService_present if
                          * present */
    CMCC_CS_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                               * CMCC_CS_MOCallRecord_freeFormatData_present if
                               * present */
    struct CMCC_CS__seqof5_ *cAMELCallLegInformation;  /* optional; set in
                                   * bit_mask
                      * CMCC_CS_MOCallRecord_cAMELCallLegInformation_present if
                      * present */
    ossBoolean      freeFormatDataAppend;  /* optional; set in bit_mask
                         * CMCC_CS_MOCallRecord_freeFormatDataAppend_present if
                         * present */
    CMCC_CS_DefaultCallHandling defaultCallHandling_2;  /* optional; set in
                                   * bit_mask
                        * CMCC_CS_MOCallRecord_defaultCallHandling_2_present if
                        * present */
    CMCC_CS_Gsm_SCFAddress gsm_SCFAddress_2;  /* optional; set in bit_mask
                             * CMCC_CS_MOCallRecord_gsm_SCFAddress_2_present if
                             * present */
    CMCC_CS_ServiceKey serviceKey_2;  /* optional; set in bit_mask
                                 * CMCC_CS_MOCallRecord_serviceKey_2_present if
                                 * present */
    CMCC_CS_FreeFormatData freeFormatData_2;  /* optional; set in bit_mask
                             * CMCC_CS_MOCallRecord_freeFormatData_2_present if
                             * present */
    ossBoolean      freeFormatDataAppend_2;  /* optional; set in bit_mask
                       * CMCC_CS_MOCallRecord_freeFormatDataAppend_2_present if
                       * present */
    CMCC_CS_SystemType systemType;  /* optional; set in bit_mask
                                     * CMCC_CS_MOCallRecord_systemType_present
                                     * if present */
    CMCC_CS_RateIndication rateIndication;  /* optional; set in bit_mask
                               * CMCC_CS_MOCallRecord_rateIndication_present if
                               * present */
    CMCC_CS_PartialRecordType partialRecordType;  /* optional; set in bit_mask
                            * CMCC_CS_MOCallRecord_partialRecordType_present if
                            * present */
    CMCC_CS_GuaranteedBitRate guaranteedBitRate;  /* optional; set in bit_mask
                            * CMCC_CS_MOCallRecord_guaranteedBitRate_present if
                            * present */
    CMCC_CS_MaximumBitRate maximumBitRate;  /* optional; set in bit_mask
                               * CMCC_CS_MOCallRecord_maximumBitRate_present if
                               * present */
    CMCC_CS_ModemType modemType;  /* optional; set in bit_mask
                                   * CMCC_CS_MOCallRecord_modemType_present if
                                   * present */
    CMCC_CS_Classmark3 classmark3;  /* optional; set in bit_mask
                                     * CMCC_CS_MOCallRecord_classmark3_present
                                     * if present */
    CMCC_CS_ChargedParty chargedParty;  /* optional; set in bit_mask
                                 * CMCC_CS_MOCallRecord_chargedParty_present if
                                 * present */
    CMCC_CS_OriginalCalledNumber originalCalledNumber;  /* optional; set in
                                   * bit_mask
                         * CMCC_CS_MOCallRecord_originalCalledNumber_present if
                         * present */
    CMCC_CS_ChargeAreaCode callingChargeAreaCode;  /* optional; set in bit_mask
                        * CMCC_CS_MOCallRecord_callingChargeAreaCode_present if
                        * present */
    CMCC_CS_ChargeAreaCode calledChargeAreaCode;  /* optional; set in bit_mask
                         * CMCC_CS_MOCallRecord_calledChargeAreaCode_present if
                         * present */
    CMCC_CS_MSCCIC  mscOutgoingCircuit;  /* optional; set in bit_mask
                           * CMCC_CS_MOCallRecord_mscOutgoingCircuit_present if
                           * present */
    CMCC_CS_RNCorBSCId orgRNCorBSCId;  /* optional; set in bit_mask
                                * CMCC_CS_MOCallRecord_orgRNCorBSCId_present if
                                * present */
    CMCC_CS_MSCId   orgMSCId;  /* optional; set in bit_mask
                                * CMCC_CS_MOCallRecord_orgMSCId_present if
                                * present */
    CMCC_CS_SubscriberEmlppPriority callEmlppPriority;  /* optional; set in
                                   * bit_mask
                            * CMCC_CS_MOCallRecord_callEmlppPriority_present if
                            * present */
    CMCC_CS_SubscriberEmlppPriority callerDefaultEmlppPriority;  /* optional;
                                   * set in bit_mask
                                   * CMCC_CS_callerDefaultEmlppPriority_present
                                   * if present */
    CMCC_CS_EASubscriberInfo eaSubscriberInfo;  /* optional; set in bit_mask
                             * CMCC_CS_MOCallRecord_eaSubscriberInfo_present if
                             * present */
    CMCC_CS_SelectedCIC selectedCIC;  /* optional; set in bit_mask
                                  * CMCC_CS_MOCallRecord_selectedCIC_present if
                                  * present */
    Nulltype        optimalRoutingFlag;  /* optional; set in bit_mask
                           * CMCC_CS_MOCallRecord_optimalRoutingFlag_present if
                           * present */
    Nulltype        optimalRoutingLateForwardFlag;  /* optional; set in bit_mask
                             * CMCC_CS_optimalRoutingLateForwardFlag_present if
                             * present */
    Nulltype        optimalRoutingEarlyForwardFlag;  /* optional; set in
                                   * bit_mask
                            * CMCC_CS_optimalRoutingEarlyForwardFlag_present if
                            * present */
    CMCC_CS_PortedFlag portedflag;  /* optional; set in bit_mask
                                     * CMCC_CS_MOCallRecord_portedflag_present
                                     * if present */
    CMCC_CS_IMSI    calledIMSI;  /* optional; set in bit_mask
                                  * CMCC_CS_calledIMSI_present if present */
    CMCC_CS_GAI     globalAreaID;  /* optional; set in bit_mask
                                    * CMCC_CS_MOCallRecord_globalAreaID_present
                                    * if present */
    struct CMCC_CS__seqof6_ *changeOfglobalAreaID;  /* optional; set in bit_mask
                         * CMCC_CS_MOCallRecord_changeOfglobalAreaID_present if
                         * present */
    CMCC_CS_SubscriberCategory subscriberCategory;  /* optional; set in bit_mask
                           * CMCC_CS_MOCallRecord_subscriberCategory_present if
                           * present */
    CMCC_CS_MCCMNC  firstmccmnc;  /* optional; set in bit_mask
                                   * CMCC_CS_MOCallRecord_firstmccmnc_present if
                                   * present */
    CMCC_CS_MCCMNC  intermediatemccmnc;  /* optional; set in bit_mask
                           * CMCC_CS_MOCallRecord_intermediatemccmnc_present if
                           * present */
    CMCC_CS_MCCMNC  lastmccmnc;  /* optional; set in bit_mask
                                  * CMCC_CS_MOCallRecord_lastmccmnc_present if
                                  * present */
    CMCC_CS_CUGOutgoingAccessIndicator cUGOutgoingAccessIndicator;  
                                  /* optional; set in bit_mask
                   * CMCC_CS_MOCallRecord_cUGOutgoingAccessIndicator_present if
                   * present */
    CMCC_CS_CUGInterlockCode cUGInterlockCode;  /* optional; set in bit_mask
                             * CMCC_CS_MOCallRecord_cUGInterlockCode_present if
                             * present */
    CMCC_CS_CUGOutgoingAccessUsed cUGOutgoingAccessUsed;  /* optional; set in
                                   * bit_mask
                                   * CMCC_CS_cUGOutgoingAccessUsed_present if
                                   * present */
    CMCC_CS_CUGIndex cUGIndex;  /* optional; set in bit_mask
                                 * CMCC_CS_MOCallRecord_cUGIndex_present if
                                 * present */
    CMCC_CS_InteractionWithIP interactionWithIP;  /* optional; set in bit_mask
                                         * CMCC_CS_interactionWithIP_present if
                                         * present */
    CMCC_CS_HotBillingTag hotBillingTag;  /* optional; set in bit_mask
                                * CMCC_CS_MOCallRecord_hotBillingTag_present if
                                * present */
} CMCC_CS_MOCallRecord;

typedef struct CMCC_CS_EmlppPriority {
    unsigned short  length;
    unsigned char   value[1];
} CMCC_CS_EmlppPriority;

typedef enum CMCC_CS_CUGIncomingAccessUsed {
    CMCC_CS_CUGIncomingAccessUsed_callInTheSameCUGGroup = 0,
    CMCC_CS_CUGIncomingAccessUsed_callNotInTheSameCUGGroup = 1
} CMCC_CS_CUGIncomingAccessUsed;

typedef int             CMCC_CS_RedirectingCounter;

typedef struct CMCC_CS_MTCallRecord {
    unsigned char   bit_mask[11];
#       define      CMCC_CS_MTCallRecord_recordType_present 0x80
#       define      CMCC_CS_MTCallRecord_recordType_present_byte 0
#       define      CMCC_CS_MTCallRecord_servedIMSI_present 0x40
#       define      CMCC_CS_MTCallRecord_servedIMSI_present_byte 0
#       define      CMCC_CS_MTCallRecord_servedIMEI_present 0x20
#       define      CMCC_CS_MTCallRecord_servedIMEI_present_byte 0
#       define      CMCC_CS_MTCallRecord_servedMSISDN_present 0x10
#       define      CMCC_CS_MTCallRecord_servedMSISDN_present_byte 0
#       define      CMCC_CS_MTCallRecord_callingNumber_present 0x08
#       define      CMCC_CS_MTCallRecord_callingNumber_present_byte 0
#       define      CMCC_CS_MTCallRecord_connectedNumber_present 0x04
#       define      CMCC_CS_MTCallRecord_connectedNumber_present_byte 0
#       define      CMCC_CS_MTCallRecord_recordingEntity_present 0x02
#       define      CMCC_CS_MTCallRecord_recordingEntity_present_byte 0
#       define      CMCC_CS_MTCallRecord_mscIncomingROUTE_present 0x01
#       define      CMCC_CS_MTCallRecord_mscIncomingROUTE_present_byte 0
#       define      CMCC_CS_MTCallRecord_mscOutgoingROUTE_present 0x80
#       define      CMCC_CS_MTCallRecord_mscOutgoingROUTE_present_byte 1
#       define      CMCC_CS_MTCallRecord_location_present 0x40
#       define      CMCC_CS_MTCallRecord_location_present_byte 1
#       define      CMCC_CS_MTCallRecord_changeOfLocation_present 0x20
#       define      CMCC_CS_MTCallRecord_changeOfLocation_present_byte 1
#       define      CMCC_CS_MTCallRecord_basicService_present 0x10
#       define      CMCC_CS_MTCallRecord_basicService_present_byte 1
#       define      CMCC_CS_MTCallRecord_transparencyIndicator_present 0x08
#       define      CMCC_CS_MTCallRecord_transparencyIndicator_present_byte 1
#       define      CMCC_CS_MTCallRecord_changeOfService_present 0x04
#       define      CMCC_CS_MTCallRecord_changeOfService_present_byte 1
#       define      CMCC_CS_MTCallRecord_supplServicesUsed_present 0x02
#       define      CMCC_CS_MTCallRecord_supplServicesUsed_present_byte 1
#       define      CMCC_CS_MTCallRecord_aocParameters_present 0x01
#       define      CMCC_CS_MTCallRecord_aocParameters_present_byte 1
#       define      CMCC_CS_MTCallRecord_changeOfAOCParms_present 0x80
#       define      CMCC_CS_MTCallRecord_changeOfAOCParms_present_byte 2
#       define      CMCC_CS_MTCallRecord_msClassmark_present 0x40
#       define      CMCC_CS_MTCallRecord_msClassmark_present_byte 2
#       define      CMCC_CS_MTCallRecord_changeOfClassmark_present 0x20
#       define      CMCC_CS_MTCallRecord_changeOfClassmark_present_byte 2
#       define      CMCC_CS_MTCallRecord_seizureTime_present 0x10
#       define      CMCC_CS_MTCallRecord_seizureTime_present_byte 2
#       define      CMCC_CS_MTCallRecord_answerTime_present 0x08
#       define      CMCC_CS_MTCallRecord_answerTime_present_byte 2
#       define      CMCC_CS_MTCallRecord_releaseTime_present 0x04
#       define      CMCC_CS_MTCallRecord_releaseTime_present_byte 2
#       define      CMCC_CS_MTCallRecord_callDuration_present 0x02
#       define      CMCC_CS_MTCallRecord_callDuration_present_byte 2
#       define      CMCC_CS_MTCallRecord_radioChanRequested_present 0x01
#       define      CMCC_CS_MTCallRecord_radioChanRequested_present_byte 2
#       define      CMCC_CS_MTCallRecord_radioChanUsed_present 0x80
#       define      CMCC_CS_MTCallRecord_radioChanUsed_present_byte 3
#       define      CMCC_CS_MTCallRecord_changeOfRadioChan_present 0x40
#       define      CMCC_CS_MTCallRecord_changeOfRadioChan_present_byte 3
#       define      CMCC_CS_MTCallRecord_causeForTerm_present 0x20
#       define      CMCC_CS_MTCallRecord_causeForTerm_present_byte 3
#       define      CMCC_CS_MTCallRecord_diagnostics_present 0x10
#       define      CMCC_CS_MTCallRecord_diagnostics_present_byte 3
#       define      CMCC_CS_MTCallRecord_callReference_present 0x08
#       define      CMCC_CS_MTCallRecord_callReference_present_byte 3
#       define      CMCC_CS_MTCallRecord_sequenceNumber_present 0x04
#       define      CMCC_CS_MTCallRecord_sequenceNumber_present_byte 3
#       define      CMCC_CS_MTCallRecord_additionalChgInfo_present 0x02
#       define      CMCC_CS_MTCallRecord_additionalChgInfo_present_byte 3
#       define      CMCC_CS_MTCallRecord_recordExtensions_present 0x01
#       define      CMCC_CS_MTCallRecord_recordExtensions_present_byte 3
#       define      CMCC_CS_MTCallRecord_networkCallReference_present 0x80
#       define      CMCC_CS_MTCallRecord_networkCallReference_present_byte 4
#       define      CMCC_CS_MTCallRecord_mSCAddress_present 0x40
#       define      CMCC_CS_MTCallRecord_mSCAddress_present_byte 4
#       define      CMCC_CS_MTCallRecord_fnur_present 0x20
#       define      CMCC_CS_MTCallRecord_fnur_present_byte 4
#       define      CMCC_CS_MTCallRecord_aiurRequested_present 0x10
#       define      CMCC_CS_MTCallRecord_aiurRequested_present_byte 4
#       define      CMCC_CS_MTCallRecord_speechVersionSupported_present 0x08
#       define      CMCC_CS_MTCallRecord_speechVersionSupported_present_byte 4
#       define      CMCC_CS_MTCallRecord_speechVersionUsed_present 0x04
#       define      CMCC_CS_MTCallRecord_speechVersionUsed_present_byte 4
#       define      CMCC_CS_MTCallRecord_gsm_SCFAddress_present 0x02
#       define      CMCC_CS_MTCallRecord_gsm_SCFAddress_present_byte 4
#       define      CMCC_CS_MTCallRecord_serviceKey_present 0x01
#       define      CMCC_CS_MTCallRecord_serviceKey_present_byte 4
#       define      CMCC_CS_MTCallRecord_systemType_present 0x80
#       define      CMCC_CS_MTCallRecord_systemType_present_byte 5
#       define      CMCC_CS_MTCallRecord_rateIndication_present 0x40
#       define      CMCC_CS_MTCallRecord_rateIndication_present_byte 5
#       define      CMCC_CS_MTCallRecord_partialRecordType_present 0x20
#       define      CMCC_CS_MTCallRecord_partialRecordType_present_byte 5
#       define      CMCC_CS_MTCallRecord_guaranteedBitRate_present 0x10
#       define      CMCC_CS_MTCallRecord_guaranteedBitRate_present_byte 5
#       define      CMCC_CS_MTCallRecord_maximumBitRate_present 0x08
#       define      CMCC_CS_MTCallRecord_maximumBitRate_present_byte 5
#       define      CMCC_CS_initialCallAttemptFlag_present 0x04
#       define      CMCC_CS_initialCallAttemptFlag_present_byte 5
#       define      CMCC_CS_ussdCallBackFlag_present 0x02
#       define      CMCC_CS_ussdCallBackFlag_present_byte 5
#       define      CMCC_CS_MTCallRecord_modemType_present 0x01
#       define      CMCC_CS_MTCallRecord_modemType_present_byte 5
#       define      CMCC_CS_MTCallRecord_classmark3_present 0x80
#       define      CMCC_CS_MTCallRecord_classmark3_present_byte 6
#       define      CMCC_CS_MTCallRecord_chargedParty_present 0x40
#       define      CMCC_CS_MTCallRecord_chargedParty_present_byte 6
#       define      CMCC_CS_MTCallRecord_originalCalledNumber_present 0x20
#       define      CMCC_CS_MTCallRecord_originalCalledNumber_present_byte 6
#       define      CMCC_CS_MTCallRecord_callingChargeAreaCode_present 0x10
#       define      CMCC_CS_MTCallRecord_callingChargeAreaCode_present_byte 6
#       define      CMCC_CS_MTCallRecord_calledChargeAreaCode_present 0x08
#       define      CMCC_CS_MTCallRecord_calledChargeAreaCode_present_byte 6
#       define      CMCC_CS_MTCallRecord_defaultCallHandling_present 0x04
#       define      CMCC_CS_MTCallRecord_defaultCallHandling_present_byte 6
#       define      CMCC_CS_MTCallRecord_freeFormatData_present 0x02
#       define      CMCC_CS_MTCallRecord_freeFormatData_present_byte 6
#       define      CMCC_CS_MTCallRecord_freeFormatDataAppend_present 0x01
#       define      CMCC_CS_MTCallRecord_freeFormatDataAppend_present_byte 6
#       define      CMCC_CS_MTCallRecord_numberOfDPEncountered_present 0x80
#       define      CMCC_CS_MTCallRecord_numberOfDPEncountered_present_byte 7
#       define      CMCC_CS_MTCallRecord_levelOfCAMELService_present 0x40
#       define      CMCC_CS_MTCallRecord_levelOfCAMELService_present_byte 7
#       define      CMCC_CS_MTCallRecord_roamingNumber_present 0x20
#       define      CMCC_CS_MTCallRecord_roamingNumber_present_byte 7
#       define      CMCC_CS_MTCallRecord_mscIncomingCircuit_present 0x10
#       define      CMCC_CS_MTCallRecord_mscIncomingCircuit_present_byte 7
#       define      CMCC_CS_MTCallRecord_orgRNCorBSCId_present 0x08
#       define      CMCC_CS_MTCallRecord_orgRNCorBSCId_present_byte 7
#       define      CMCC_CS_MTCallRecord_orgMSCId_present 0x04
#       define      CMCC_CS_MTCallRecord_orgMSCId_present_byte 7
#       define      CMCC_CS_MTCallRecord_callEmlppPriority_present 0x02
#       define      CMCC_CS_MTCallRecord_callEmlppPriority_present_byte 7
#       define      CMCC_CS_calledDefaultEmlppPriority_present 0x01
#       define      CMCC_CS_calledDefaultEmlppPriority_present_byte 7
#       define      CMCC_CS_MTCallRecord_eaSubscriberInfo_present 0x80
#       define      CMCC_CS_MTCallRecord_eaSubscriberInfo_present_byte 8
#       define      CMCC_CS_MTCallRecord_selectedCIC_present 0x40
#       define      CMCC_CS_MTCallRecord_selectedCIC_present_byte 8
#       define      CMCC_CS_MTCallRecord_optimalRoutingFlag_present 0x20
#       define      CMCC_CS_MTCallRecord_optimalRoutingFlag_present_byte 8
#       define      CMCC_CS_MTCallRecord_portedflag_present 0x10
#       define      CMCC_CS_MTCallRecord_portedflag_present_byte 8
#       define      CMCC_CS_MTCallRecord_globalAreaID_present 0x08
#       define      CMCC_CS_MTCallRecord_globalAreaID_present_byte 8
#       define      CMCC_CS_MTCallRecord_changeOfglobalAreaID_present 0x04
#       define      CMCC_CS_MTCallRecord_changeOfglobalAreaID_present_byte 8
#       define      CMCC_CS_MTCallRecord_subscriberCategory_present 0x02
#       define      CMCC_CS_MTCallRecord_subscriberCategory_present_byte 8
#       define      CMCC_CS_MTCallRecord_firstmccmnc_present 0x01
#       define      CMCC_CS_MTCallRecord_firstmccmnc_present_byte 8
#       define      CMCC_CS_MTCallRecord_intermediatemccmnc_present 0x80
#       define      CMCC_CS_MTCallRecord_intermediatemccmnc_present_byte 9
#       define      CMCC_CS_MTCallRecord_lastmccmnc_present 0x40
#       define      CMCC_CS_MTCallRecord_lastmccmnc_present_byte 9
#       define      CMCC_CS_MTCallRecord_cUGOutgoingAccessIndicator_present 0x20
#       define      CMCC_CS_MTCallRecord_cUGOutgoingAccessIndicator_present_byte 9
#       define      CMCC_CS_MTCallRecord_cUGInterlockCode_present 0x10
#       define      CMCC_CS_MTCallRecord_cUGInterlockCode_present_byte 9
#       define      CMCC_CS_MTCallRecord_cUGIncomingAccessUsed_present 0x08
#       define      CMCC_CS_MTCallRecord_cUGIncomingAccessUsed_present_byte 9
#       define      CMCC_CS_MTCallRecord_cUGIndex_present 0x04
#       define      CMCC_CS_MTCallRecord_cUGIndex_present_byte 9
#       define      CMCC_CS_MTCallRecord_hotBillingTag_present 0x02
#       define      CMCC_CS_MTCallRecord_hotBillingTag_present_byte 9
#       define      CMCC_CS_redirectingnumber_present 0x01
#       define      CMCC_CS_redirectingnumber_present_byte 9
#       define      CMCC_CS_redirectingcounter_present 0x80
#       define      CMCC_CS_redirectingcounter_present_byte 10
    CMCC_CS_CallEventRecordType recordType;  /* optional; set in bit_mask
                                   * CMCC_CS_MTCallRecord_recordType_present if
                                   * present */
    CMCC_CS_IMSI    servedIMSI;  /* optional; set in bit_mask
                                  * CMCC_CS_MTCallRecord_servedIMSI_present if
                                  * present */
    CMCC_CS_IMEI    servedIMEI;  /* optional; set in bit_mask
                                  * CMCC_CS_MTCallRecord_servedIMEI_present if
                                  * present */
    CMCC_CS_CalledNumber servedMSISDN;  /* optional; set in bit_mask
                                 * CMCC_CS_MTCallRecord_servedMSISDN_present if
                                 * present */
    CMCC_CS_CallingNumber callingNumber;  /* optional; set in bit_mask
                                * CMCC_CS_MTCallRecord_callingNumber_present if
                                * present */
    CMCC_CS_ConnectedNumber connectedNumber;  /* optional; set in bit_mask
                              * CMCC_CS_MTCallRecord_connectedNumber_present if
                              * present */
    CMCC_CS_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                              * CMCC_CS_MTCallRecord_recordingEntity_present if
                              * present */
    CMCC_CS_ROUTE   mscIncomingROUTE;  /* optional; set in bit_mask
                             * CMCC_CS_MTCallRecord_mscIncomingROUTE_present if
                             * present */
    CMCC_CS_ROUTE   mscOutgoingROUTE;  /* optional; set in bit_mask
                             * CMCC_CS_MTCallRecord_mscOutgoingROUTE_present if
                             * present */
    CMCC_CS_LocationAreaAndCell location;  /* optional; set in bit_mask
                                     * CMCC_CS_MTCallRecord_location_present if
                                     * present */
    struct CMCC_CS__seqof1_ *changeOfLocation;  /* optional; set in bit_mask
                             * CMCC_CS_MTCallRecord_changeOfLocation_present if
                             * present */
    CMCC_CS_BasicServiceCode basicService;  /* optional; set in bit_mask
                                 * CMCC_CS_MTCallRecord_basicService_present if
                                 * present */
    CMCC_CS_TransparencyInd transparencyIndicator;  /* optional; set in bit_mask
                        * CMCC_CS_MTCallRecord_transparencyIndicator_present if
                        * present */
    struct CMCC_CS__seqof2_ *changeOfService;  /* optional; set in bit_mask
                              * CMCC_CS_MTCallRecord_changeOfService_present if
                              * present */
    struct CMCC_CS__seqof3_ *supplServicesUsed;  /* optional; set in bit_mask
                            * CMCC_CS_MTCallRecord_supplServicesUsed_present if
                            * present */
    CMCC_CS_AOCParameters aocParameters;  /* optional; set in bit_mask
                                * CMCC_CS_MTCallRecord_aocParameters_present if
                                * present */
    struct CMCC_CS__seqof4_ *changeOfAOCParms;  /* optional; set in bit_mask
                             * CMCC_CS_MTCallRecord_changeOfAOCParms_present if
                             * present */
    CMCC_CS_Classmark msClassmark;  /* optional; set in bit_mask
                                     * CMCC_CS_MTCallRecord_msClassmark_present
                                     * if present */
    CMCC_CS_ChangeOfClassmark changeOfClassmark;  /* optional; set in bit_mask
                            * CMCC_CS_MTCallRecord_changeOfClassmark_present if
                            * present */
    CMCC_CS_TimeStamp seizureTime;  /* optional; set in bit_mask
                                     * CMCC_CS_MTCallRecord_seizureTime_present
                                     * if present */
    CMCC_CS_TimeStamp answerTime;  /* optional; set in bit_mask
                                    * CMCC_CS_MTCallRecord_answerTime_present if
                                    * present */
    CMCC_CS_TimeStamp releaseTime;  /* optional; set in bit_mask
                                     * CMCC_CS_MTCallRecord_releaseTime_present
                                     * if present */
    CMCC_CS_CallDuration callDuration;  /* optional; set in bit_mask
                                 * CMCC_CS_MTCallRecord_callDuration_present if
                                 * present */
    CMCC_CS_RadioChanRequested radioChanRequested;  /* optional; set in bit_mask
                           * CMCC_CS_MTCallRecord_radioChanRequested_present if
                           * present */
    CMCC_CS_TrafficChannel radioChanUsed;  /* optional; set in bit_mask
                                * CMCC_CS_MTCallRecord_radioChanUsed_present if
                                * present */
    CMCC_CS_ChangeOfRadioChannel changeOfRadioChan;  /* optional; set in
                                   * bit_mask
                            * CMCC_CS_MTCallRecord_changeOfRadioChan_present if
                            * present */
    CMCC_CS_CauseForTerm causeForTerm;  /* optional; set in bit_mask
                                 * CMCC_CS_MTCallRecord_causeForTerm_present if
                                 * present */
    CMCC_CS_Diagnostics diagnostics;  /* optional; set in bit_mask
                                  * CMCC_CS_MTCallRecord_diagnostics_present if
                                  * present */
    CMCC_CS_CallReference callReference;  /* optional; set in bit_mask
                                * CMCC_CS_MTCallRecord_callReference_present if
                                * present */
    CMCC_CS_SequenceNumber sequenceNumber;  /* optional; set in bit_mask
                               * CMCC_CS_MTCallRecord_sequenceNumber_present if
                               * present */
    CMCC_CS_AdditionalChgInfo additionalChgInfo;  /* optional; set in bit_mask
                            * CMCC_CS_MTCallRecord_additionalChgInfo_present if
                            * present */
    struct CMCC_CS_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                             * CMCC_CS_MTCallRecord_recordExtensions_present if
                             * present */
    CMCC_CS_NetworkCallReference networkCallReference;  /* optional; set in
                                   * bit_mask
                         * CMCC_CS_MTCallRecord_networkCallReference_present if
                         * present */
    CMCC_CS_MSCAddress mSCAddress;  /* optional; set in bit_mask
                                     * CMCC_CS_MTCallRecord_mSCAddress_present
                                     * if present */
    CMCC_CS_Fnur    fnur;  /* optional; set in bit_mask
                            * CMCC_CS_MTCallRecord_fnur_present if present */
    CMCC_CS_AiurRequested aiurRequested;  /* optional; set in bit_mask
                                * CMCC_CS_MTCallRecord_aiurRequested_present if
                                * present */
    CMCC_CS_SpeechVersionIdentifier speechVersionSupported;  /* optional; set in
                                   * bit_mask
                       * CMCC_CS_MTCallRecord_speechVersionSupported_present if
                       * present */
    CMCC_CS_SpeechVersionIdentifier speechVersionUsed;  /* optional; set in
                                   * bit_mask
                            * CMCC_CS_MTCallRecord_speechVersionUsed_present if
                            * present */
    CMCC_CS_Gsm_SCFAddress gsm_SCFAddress;  /* optional; set in bit_mask
                               * CMCC_CS_MTCallRecord_gsm_SCFAddress_present if
                               * present */
    CMCC_CS_ServiceKey serviceKey;  /* optional; set in bit_mask
                                     * CMCC_CS_MTCallRecord_serviceKey_present
                                     * if present */
    CMCC_CS_SystemType systemType;  /* optional; set in bit_mask
                                     * CMCC_CS_MTCallRecord_systemType_present
                                     * if present */
    CMCC_CS_RateIndication rateIndication;  /* optional; set in bit_mask
                               * CMCC_CS_MTCallRecord_rateIndication_present if
                               * present */
    CMCC_CS_PartialRecordType partialRecordType;  /* optional; set in bit_mask
                            * CMCC_CS_MTCallRecord_partialRecordType_present if
                            * present */
    CMCC_CS_GuaranteedBitRate guaranteedBitRate;  /* optional; set in bit_mask
                            * CMCC_CS_MTCallRecord_guaranteedBitRate_present if
                            * present */
    CMCC_CS_MaximumBitRate maximumBitRate;  /* optional; set in bit_mask
                               * CMCC_CS_MTCallRecord_maximumBitRate_present if
                               * present */
    Nulltype        initialCallAttemptFlag;  /* optional; set in bit_mask
                                    * CMCC_CS_initialCallAttemptFlag_present if
                                    * present */
    Nulltype        ussdCallBackFlag;  /* optional; set in bit_mask
                                        * CMCC_CS_ussdCallBackFlag_present if
                                        * present */
    CMCC_CS_ModemType modemType;  /* optional; set in bit_mask
                                   * CMCC_CS_MTCallRecord_modemType_present if
                                   * present */
    CMCC_CS_Classmark3 classmark3;  /* optional; set in bit_mask
                                     * CMCC_CS_MTCallRecord_classmark3_present
                                     * if present */
    CMCC_CS_ChargedParty chargedParty;  /* optional; set in bit_mask
                                 * CMCC_CS_MTCallRecord_chargedParty_present if
                                 * present */
    CMCC_CS_OriginalCalledNumber originalCalledNumber;  /* optional; set in
                                   * bit_mask
                         * CMCC_CS_MTCallRecord_originalCalledNumber_present if
                         * present */
    CMCC_CS_ChargeAreaCode callingChargeAreaCode;  /* optional; set in bit_mask
                        * CMCC_CS_MTCallRecord_callingChargeAreaCode_present if
                        * present */
    CMCC_CS_ChargeAreaCode calledChargeAreaCode;  /* optional; set in bit_mask
                         * CMCC_CS_MTCallRecord_calledChargeAreaCode_present if
                         * present */
    CMCC_CS_DefaultCallHandling defaultCallHandling;  /* optional; set in
                                   * bit_mask
                          * CMCC_CS_MTCallRecord_defaultCallHandling_present if
                          * present */
    CMCC_CS_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                               * CMCC_CS_MTCallRecord_freeFormatData_present if
                               * present */
    ossBoolean      freeFormatDataAppend;  /* optional; set in bit_mask
                         * CMCC_CS_MTCallRecord_freeFormatDataAppend_present if
                         * present */
    int             numberOfDPEncountered;  /* optional; set in bit_mask
                        * CMCC_CS_MTCallRecord_numberOfDPEncountered_present if
                        * present */
    CMCC_CS_LevelOfCAMELService levelOfCAMELService;  /* optional; set in
                                   * bit_mask
                          * CMCC_CS_MTCallRecord_levelOfCAMELService_present if
                          * present */
    CMCC_CS_RoamingNumber roamingNumber;  /* optional; set in bit_mask
                                * CMCC_CS_MTCallRecord_roamingNumber_present if
                                * present */
    CMCC_CS_MSCCIC  mscIncomingCircuit;  /* optional; set in bit_mask
                           * CMCC_CS_MTCallRecord_mscIncomingCircuit_present if
                           * present */
    CMCC_CS_RNCorBSCId orgRNCorBSCId;  /* optional; set in bit_mask
                                * CMCC_CS_MTCallRecord_orgRNCorBSCId_present if
                                * present */
    CMCC_CS_MSCId   orgMSCId;  /* optional; set in bit_mask
                                * CMCC_CS_MTCallRecord_orgMSCId_present if
                                * present */
    CMCC_CS_EmlppPriority callEmlppPriority;  /* optional; set in bit_mask
                            * CMCC_CS_MTCallRecord_callEmlppPriority_present if
                            * present */
    CMCC_CS_EmlppPriority calledDefaultEmlppPriority;  /* optional; set in
                                   * bit_mask
                                   * CMCC_CS_calledDefaultEmlppPriority_present
                                   * if present */
    CMCC_CS_EASubscriberInfo eaSubscriberInfo;  /* optional; set in bit_mask
                             * CMCC_CS_MTCallRecord_eaSubscriberInfo_present if
                             * present */
    CMCC_CS_SelectedCIC selectedCIC;  /* optional; set in bit_mask
                                  * CMCC_CS_MTCallRecord_selectedCIC_present if
                                  * present */
    Nulltype        optimalRoutingFlag;  /* optional; set in bit_mask
                           * CMCC_CS_MTCallRecord_optimalRoutingFlag_present if
                           * present */
    CMCC_CS_PortedFlag portedflag;  /* optional; set in bit_mask
                                     * CMCC_CS_MTCallRecord_portedflag_present
                                     * if present */
    CMCC_CS_GAI     globalAreaID;  /* optional; set in bit_mask
                                    * CMCC_CS_MTCallRecord_globalAreaID_present
                                    * if present */
    struct CMCC_CS__seqof6_ *changeOfglobalAreaID;  /* optional; set in bit_mask
                         * CMCC_CS_MTCallRecord_changeOfglobalAreaID_present if
                         * present */
    CMCC_CS_SubscriberCategory subscriberCategory;  /* optional; set in bit_mask
                           * CMCC_CS_MTCallRecord_subscriberCategory_present if
                           * present */
    CMCC_CS_MCCMNC  firstmccmnc;  /* optional; set in bit_mask
                                   * CMCC_CS_MTCallRecord_firstmccmnc_present if
                                   * present */
    CMCC_CS_MCCMNC  intermediatemccmnc;  /* optional; set in bit_mask
                           * CMCC_CS_MTCallRecord_intermediatemccmnc_present if
                           * present */
    CMCC_CS_MCCMNC  lastmccmnc;  /* optional; set in bit_mask
                                  * CMCC_CS_MTCallRecord_lastmccmnc_present if
                                  * present */
    CMCC_CS_CUGOutgoingAccessIndicator cUGOutgoingAccessIndicator;  
                                  /* optional; set in bit_mask
                   * CMCC_CS_MTCallRecord_cUGOutgoingAccessIndicator_present if
                   * present */
    CMCC_CS_CUGInterlockCode cUGInterlockCode;  /* optional; set in bit_mask
                             * CMCC_CS_MTCallRecord_cUGInterlockCode_present if
                             * present */
    CMCC_CS_CUGIncomingAccessUsed cUGIncomingAccessUsed;  /* optional; set in
                                   * bit_mask
                        * CMCC_CS_MTCallRecord_cUGIncomingAccessUsed_present if
                        * present */
    CMCC_CS_CUGIndex cUGIndex;  /* optional; set in bit_mask
                                 * CMCC_CS_MTCallRecord_cUGIndex_present if
                                 * present */
    CMCC_CS_HotBillingTag hotBillingTag;  /* optional; set in bit_mask
                                * CMCC_CS_MTCallRecord_hotBillingTag_present if
                                * present */
    CMCC_CS_RedirectingNumber redirectingnumber;  /* optional; set in bit_mask
                                         * CMCC_CS_redirectingnumber_present if
                                         * present */
    CMCC_CS_RedirectingCounter redirectingcounter;  /* optional; set in bit_mask
                                        * CMCC_CS_redirectingcounter_present if
                                        * present */
} CMCC_CS_MTCallRecord;

typedef struct CMCC_CS_RoamingRecord {
    unsigned char   bit_mask[5];
#       define      CMCC_CS_RoamingRecord_recordType_present 0x80
#       define      CMCC_CS_RoamingRecord_recordType_present_byte 0
#       define      CMCC_CS_RoamingRecord_servedIMSI_present 0x40
#       define      CMCC_CS_RoamingRecord_servedIMSI_present_byte 0
#       define      CMCC_CS_RoamingRecord_servedMSISDN_present 0x20
#       define      CMCC_CS_RoamingRecord_servedMSISDN_present_byte 0
#       define      CMCC_CS_RoamingRecord_callingNumber_present 0x10
#       define      CMCC_CS_RoamingRecord_callingNumber_present_byte 0
#       define      CMCC_CS_RoamingRecord_roamingNumber_present 0x08
#       define      CMCC_CS_RoamingRecord_roamingNumber_present_byte 0
#       define      CMCC_CS_RoamingRecord_recordingEntity_present 0x04
#       define      CMCC_CS_RoamingRecord_recordingEntity_present_byte 0
#       define      CMCC_CS_RoamingRecord_mscIncomingROUTE_present 0x02
#       define      CMCC_CS_RoamingRecord_mscIncomingROUTE_present_byte 0
#       define      CMCC_CS_RoamingRecord_mscOutgoingROUTE_present 0x01
#       define      CMCC_CS_RoamingRecord_mscOutgoingROUTE_present_byte 0
#       define      CMCC_CS_RoamingRecord_basicService_present 0x80
#       define      CMCC_CS_RoamingRecord_basicService_present_byte 1
#       define      CMCC_CS_RoamingRecord_transparencyIndicator_present 0x40
#       define      CMCC_CS_RoamingRecord_transparencyIndicator_present_byte 1
#       define      CMCC_CS_RoamingRecord_changeOfService_present 0x20
#       define      CMCC_CS_RoamingRecord_changeOfService_present_byte 1
#       define      CMCC_CS_RoamingRecord_supplServicesUsed_present 0x10
#       define      CMCC_CS_RoamingRecord_supplServicesUsed_present_byte 1
#       define      CMCC_CS_RoamingRecord_seizureTime_present 0x08
#       define      CMCC_CS_RoamingRecord_seizureTime_present_byte 1
#       define      CMCC_CS_RoamingRecord_answerTime_present 0x04
#       define      CMCC_CS_RoamingRecord_answerTime_present_byte 1
#       define      CMCC_CS_RoamingRecord_releaseTime_present 0x02
#       define      CMCC_CS_RoamingRecord_releaseTime_present_byte 1
#       define      CMCC_CS_RoamingRecord_callDuration_present 0x01
#       define      CMCC_CS_RoamingRecord_callDuration_present_byte 1
#       define      CMCC_CS_RoamingRecord_causeForTerm_present 0x80
#       define      CMCC_CS_RoamingRecord_causeForTerm_present_byte 2
#       define      CMCC_CS_RoamingRecord_diagnostics_present 0x40
#       define      CMCC_CS_RoamingRecord_diagnostics_present_byte 2
#       define      CMCC_CS_RoamingRecord_callReference_present 0x20
#       define      CMCC_CS_RoamingRecord_callReference_present_byte 2
#       define      CMCC_CS_RoamingRecord_sequenceNumber_present 0x10
#       define      CMCC_CS_RoamingRecord_sequenceNumber_present_byte 2
#       define      CMCC_CS_RoamingRecord_recordExtensions_present 0x08
#       define      CMCC_CS_RoamingRecord_recordExtensions_present_byte 2
#       define      CMCC_CS_RoamingRecord_networkCallReference_present 0x04
#       define      CMCC_CS_RoamingRecord_networkCallReference_present_byte 2
#       define      CMCC_CS_RoamingRecord_mSCAddress_present 0x02
#       define      CMCC_CS_RoamingRecord_mSCAddress_present_byte 2
#       define      CMCC_CS_RoamingRecord_partialRecordType_present 0x01
#       define      CMCC_CS_RoamingRecord_partialRecordType_present_byte 2
#       define      CMCC_CS_RoamingRecord_additionalChgInfo_present 0x80
#       define      CMCC_CS_RoamingRecord_additionalChgInfo_present_byte 3
#       define      CMCC_CS_RoamingRecord_chargedParty_present 0x40
#       define      CMCC_CS_RoamingRecord_chargedParty_present_byte 3
#       define      CMCC_CS_RoamingRecord_originalCalledNumber_present 0x20
#       define      CMCC_CS_RoamingRecord_originalCalledNumber_present_byte 3
#       define      CMCC_CS_RoamingRecord_callingChargeAreaCode_present 0x10
#       define      CMCC_CS_RoamingRecord_callingChargeAreaCode_present_byte 3
#       define      CMCC_CS_RoamingRecord_calledChargeAreaCode_present 0x08
#       define      CMCC_CS_RoamingRecord_calledChargeAreaCode_present_byte 3
#       define      CMCC_CS_RoamingRecord_mscOutgoingCircuit_present 0x04
#       define      CMCC_CS_RoamingRecord_mscOutgoingCircuit_present_byte 3
#       define      CMCC_CS_RoamingRecord_mscIncomingCircuit_present 0x02
#       define      CMCC_CS_RoamingRecord_mscIncomingCircuit_present_byte 3
#       define      CMCC_CS_RoamingRecord_orgMSCId_present 0x01
#       define      CMCC_CS_RoamingRecord_orgMSCId_present_byte 3
#       define      CMCC_CS_RoamingRecord_callEmlppPriority_present 0x80
#       define      CMCC_CS_RoamingRecord_callEmlppPriority_present_byte 4
#       define      CMCC_CS_RoamingRecord_eaSubscriberInfo_present 0x40
#       define      CMCC_CS_RoamingRecord_eaSubscriberInfo_present_byte 4
#       define      CMCC_CS_RoamingRecord_selectedCIC_present 0x20
#       define      CMCC_CS_RoamingRecord_selectedCIC_present_byte 4
#       define      CMCC_CS_RoamingRecord_optimalRoutingFlag_present 0x10
#       define      CMCC_CS_RoamingRecord_optimalRoutingFlag_present_byte 4
#       define      CMCC_CS_RoamingRecord_subscriberCategory_present 0x08
#       define      CMCC_CS_RoamingRecord_subscriberCategory_present_byte 4
#       define      CMCC_CS_RoamingRecord_cUGOutgoingAccessIndicator_present 0x04
#       define      CMCC_CS_RoamingRecord_cUGOutgoingAccessIndicator_present_byte 4
#       define      CMCC_CS_RoamingRecord_cUGInterlockCode_present 0x02
#       define      CMCC_CS_RoamingRecord_cUGInterlockCode_present_byte 4
#       define      CMCC_CS_RoamingRecord_hotBillingTag_present 0x01
#       define      CMCC_CS_RoamingRecord_hotBillingTag_present_byte 4
    CMCC_CS_CallEventRecordType recordType;  /* optional; set in bit_mask
                                  * CMCC_CS_RoamingRecord_recordType_present if
                                  * present */
    CMCC_CS_IMSI    servedIMSI;  /* optional; set in bit_mask
                                  * CMCC_CS_RoamingRecord_servedIMSI_present if
                                  * present */
    CMCC_CS_MSISDN  servedMSISDN;  /* optional; set in bit_mask
                                    * CMCC_CS_RoamingRecord_servedMSISDN_present
                                    * if present */
    CMCC_CS_CallingNumber callingNumber;  /* optional; set in bit_mask
                               * CMCC_CS_RoamingRecord_callingNumber_present if
                               * present */
    CMCC_CS_RoamingNumber roamingNumber;  /* optional; set in bit_mask
                               * CMCC_CS_RoamingRecord_roamingNumber_present if
                               * present */
    CMCC_CS_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                             * CMCC_CS_RoamingRecord_recordingEntity_present if
                             * present */
    CMCC_CS_ROUTE   mscIncomingROUTE;  /* optional; set in bit_mask
                            * CMCC_CS_RoamingRecord_mscIncomingROUTE_present if
                            * present */
    CMCC_CS_ROUTE   mscOutgoingROUTE;  /* optional; set in bit_mask
                            * CMCC_CS_RoamingRecord_mscOutgoingROUTE_present if
                            * present */
    CMCC_CS_BasicServiceCode basicService;  /* optional; set in bit_mask
                                * CMCC_CS_RoamingRecord_basicService_present if
                                * present */
    CMCC_CS_TransparencyInd transparencyIndicator;  /* optional; set in bit_mask
                       * CMCC_CS_RoamingRecord_transparencyIndicator_present if
                       * present */
    struct CMCC_CS__seqof2_ *changeOfService;  /* optional; set in bit_mask
                             * CMCC_CS_RoamingRecord_changeOfService_present if
                             * present */
    struct CMCC_CS__seqof3_ *supplServicesUsed;  /* optional; set in bit_mask
                           * CMCC_CS_RoamingRecord_supplServicesUsed_present if
                           * present */
    CMCC_CS_TimeStamp seizureTime;  /* optional; set in bit_mask
                                     * CMCC_CS_RoamingRecord_seizureTime_present
                                     * if present */
    CMCC_CS_TimeStamp answerTime;  /* optional; set in bit_mask
                                    * CMCC_CS_RoamingRecord_answerTime_present
                                    * if present */
    CMCC_CS_TimeStamp releaseTime;  /* optional; set in bit_mask
                                     * CMCC_CS_RoamingRecord_releaseTime_present
                                     * if present */
    CMCC_CS_CallDuration callDuration;  /* optional; set in bit_mask
                                * CMCC_CS_RoamingRecord_callDuration_present if
                                * present */
    CMCC_CS_CauseForTerm causeForTerm;  /* optional; set in bit_mask
                                * CMCC_CS_RoamingRecord_causeForTerm_present if
                                * present */
    CMCC_CS_Diagnostics diagnostics;  /* optional; set in bit_mask
                                 * CMCC_CS_RoamingRecord_diagnostics_present if
                                 * present */
    CMCC_CS_CallReference callReference;  /* optional; set in bit_mask
                               * CMCC_CS_RoamingRecord_callReference_present if
                               * present */
    CMCC_CS_SequenceNumber sequenceNumber;  /* optional; set in bit_mask
                              * CMCC_CS_RoamingRecord_sequenceNumber_present if
                              * present */
    struct CMCC_CS_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                            * CMCC_CS_RoamingRecord_recordExtensions_present if
                            * present */
    CMCC_CS_NetworkCallReference networkCallReference;  /* optional; set in
                                   * bit_mask
                        * CMCC_CS_RoamingRecord_networkCallReference_present if
                        * present */
    CMCC_CS_MSCAddress mSCAddress;  /* optional; set in bit_mask
                                     * CMCC_CS_RoamingRecord_mSCAddress_present
                                     * if present */
    CMCC_CS_PartialRecordType partialRecordType;  /* optional; set in bit_mask
                           * CMCC_CS_RoamingRecord_partialRecordType_present if
                           * present */
    CMCC_CS_AdditionalChgInfo additionalChgInfo;  /* optional; set in bit_mask
                           * CMCC_CS_RoamingRecord_additionalChgInfo_present if
                           * present */
    CMCC_CS_ChargedParty chargedParty;  /* optional; set in bit_mask
                                * CMCC_CS_RoamingRecord_chargedParty_present if
                                * present */
    CMCC_CS_OriginalCalledNumber originalCalledNumber;  /* optional; set in
                                   * bit_mask
                        * CMCC_CS_RoamingRecord_originalCalledNumber_present if
                        * present */
    CMCC_CS_ChargeAreaCode callingChargeAreaCode;  /* optional; set in bit_mask
                       * CMCC_CS_RoamingRecord_callingChargeAreaCode_present if
                       * present */
    CMCC_CS_ChargeAreaCode calledChargeAreaCode;  /* optional; set in bit_mask
                        * CMCC_CS_RoamingRecord_calledChargeAreaCode_present if
                        * present */
    CMCC_CS_MSCCIC  mscOutgoingCircuit;  /* optional; set in bit_mask
                          * CMCC_CS_RoamingRecord_mscOutgoingCircuit_present if
                          * present */
    CMCC_CS_MSCCIC  mscIncomingCircuit;  /* optional; set in bit_mask
                          * CMCC_CS_RoamingRecord_mscIncomingCircuit_present if
                          * present */
    CMCC_CS_MSCId   orgMSCId;  /* optional; set in bit_mask
                                * CMCC_CS_RoamingRecord_orgMSCId_present if
                                * present */
    CMCC_CS_EmlppPriority callEmlppPriority;  /* optional; set in bit_mask
                           * CMCC_CS_RoamingRecord_callEmlppPriority_present if
                           * present */
    CMCC_CS_EASubscriberInfo eaSubscriberInfo;  /* optional; set in bit_mask
                            * CMCC_CS_RoamingRecord_eaSubscriberInfo_present if
                            * present */
    CMCC_CS_SelectedCIC selectedCIC;  /* optional; set in bit_mask
                                 * CMCC_CS_RoamingRecord_selectedCIC_present if
                                 * present */
    Nulltype        optimalRoutingFlag;  /* optional; set in bit_mask
                          * CMCC_CS_RoamingRecord_optimalRoutingFlag_present if
                          * present */
    CMCC_CS_SubscriberCategory subscriberCategory;  /* optional; set in bit_mask
                          * CMCC_CS_RoamingRecord_subscriberCategory_present if
                          * present */
    CMCC_CS_CUGOutgoingAccessIndicator cUGOutgoingAccessIndicator;  
                                  /* optional; set in bit_mask
                  * CMCC_CS_RoamingRecord_cUGOutgoingAccessIndicator_present if
                  * present */
    CMCC_CS_CUGInterlockCode cUGInterlockCode;  /* optional; set in bit_mask
                            * CMCC_CS_RoamingRecord_cUGInterlockCode_present if
                            * present */
    CMCC_CS_HotBillingTag hotBillingTag;  /* optional; set in bit_mask
                               * CMCC_CS_RoamingRecord_hotBillingTag_present if
                               * present */
} CMCC_CS_RoamingRecord;

typedef struct CMCC_CS_ISDN_BC {
    unsigned int    length;
    unsigned char   *value;
} CMCC_CS_ISDN_BC;

typedef struct CMCC_CS_LLC {
    unsigned int    length;
    unsigned char   *value;
} CMCC_CS_LLC;

typedef struct CMCC_CS_HLC {
    unsigned int    length;
    unsigned char   *value;
} CMCC_CS_HLC;

typedef struct CMCC_CS_IncGatewayRecord {
    unsigned char   bit_mask[5];
#       define      CMCC_CS_IncGatewayRecord_recordType_present 0x80
#       define      CMCC_CS_IncGatewayRecord_recordType_present_byte 0
#       define      CMCC_CS_IncGatewayRecord_callingNumber_present 0x40
#       define      CMCC_CS_IncGatewayRecord_callingNumber_present_byte 0
#       define      CMCC_CS_IncGatewayRecord_calledNumber_present 0x20
#       define      CMCC_CS_IncGatewayRecord_calledNumber_present_byte 0
#       define      CMCC_CS_IncGatewayRecord_recordingEntity_present 0x10
#       define      CMCC_CS_IncGatewayRecord_recordingEntity_present_byte 0
#       define      CMCC_CS_IncGatewayRecord_mscIncomingROUTE_present 0x08
#       define      CMCC_CS_IncGatewayRecord_mscIncomingROUTE_present_byte 0
#       define      CMCC_CS_IncGatewayRecord_mscOutgoingROUTE_present 0x04
#       define      CMCC_CS_IncGatewayRecord_mscOutgoingROUTE_present_byte 0
#       define      CMCC_CS_IncGatewayRecord_seizureTime_present 0x02
#       define      CMCC_CS_IncGatewayRecord_seizureTime_present_byte 0
#       define      CMCC_CS_IncGatewayRecord_answerTime_present 0x01
#       define      CMCC_CS_IncGatewayRecord_answerTime_present_byte 0
#       define      CMCC_CS_IncGatewayRecord_releaseTime_present 0x80
#       define      CMCC_CS_IncGatewayRecord_releaseTime_present_byte 1
#       define      CMCC_CS_IncGatewayRecord_callDuration_present 0x40
#       define      CMCC_CS_IncGatewayRecord_callDuration_present_byte 1
#       define      CMCC_CS_IncGatewayRecord_causeForTerm_present 0x20
#       define      CMCC_CS_IncGatewayRecord_causeForTerm_present_byte 1
#       define      CMCC_CS_IncGatewayRecord_diagnostics_present 0x10
#       define      CMCC_CS_IncGatewayRecord_diagnostics_present_byte 1
#       define      CMCC_CS_IncGatewayRecord_callReference_present 0x08
#       define      CMCC_CS_IncGatewayRecord_callReference_present_byte 1
#       define      CMCC_CS_IncGatewayRecord_sequenceNumber_present 0x04
#       define      CMCC_CS_IncGatewayRecord_sequenceNumber_present_byte 1
#       define      CMCC_CS_IncGatewayRecord_recordExtensions_present 0x02
#       define      CMCC_CS_IncGatewayRecord_recordExtensions_present_byte 1
#       define      CMCC_CS_IncGatewayRecord_partialRecordType_present 0x01
#       define      CMCC_CS_IncGatewayRecord_partialRecordType_present_byte 1
#       define      CMCC_CS_iSDN_BC_present 0x80
#       define      CMCC_CS_iSDN_BC_present_byte 2
#       define      CMCC_CS_lLC_present 0x40
#       define      CMCC_CS_lLC_present_byte 2
#       define      CMCC_CS_hLC_present 0x20
#       define      CMCC_CS_hLC_present_byte 2
#       define      CMCC_CS_IncGatewayRecord_basicService_present 0x10
#       define      CMCC_CS_IncGatewayRecord_basicService_present_byte 2
#       define      CMCC_CS_IncGatewayRecord_additionalChgInfo_present 0x08
#       define      CMCC_CS_IncGatewayRecord_additionalChgInfo_present_byte 2
#       define      CMCC_CS_IncGatewayRecord_chargedParty_present 0x04
#       define      CMCC_CS_IncGatewayRecord_chargedParty_present_byte 2
#       define      CMCC_CS_IncGatewayRecord_originalCalledNumber_present 0x02
#       define      CMCC_CS_IncGatewayRecord_originalCalledNumber_present_byte 2
#       define      CMCC_CS_IncGatewayRecord_rateIndication_present 0x01
#       define      CMCC_CS_IncGatewayRecord_rateIndication_present_byte 2
#       define      CMCC_CS_IncGatewayRecord_roamingNumber_present 0x80
#       define      CMCC_CS_IncGatewayRecord_roamingNumber_present_byte 3
#       define      CMCC_CS_IncGatewayRecord_mscIncomingCircuit_present 0x40
#       define      CMCC_CS_IncGatewayRecord_mscIncomingCircuit_present_byte 3
#       define      CMCC_CS_IncGatewayRecord_orgMSCId_present 0x20
#       define      CMCC_CS_IncGatewayRecord_orgMSCId_present_byte 3
#       define      CMCC_CS_IncGatewayRecord_callEmlppPriority_present 0x10
#       define      CMCC_CS_IncGatewayRecord_callEmlppPriority_present_byte 3
#       define      CMCC_CS_IncGatewayRecord_eaSubscriberInfo_present 0x08
#       define      CMCC_CS_IncGatewayRecord_eaSubscriberInfo_present_byte 3
#       define      CMCC_CS_IncGatewayRecord_selectedCIC_present 0x04
#       define      CMCC_CS_IncGatewayRecord_selectedCIC_present_byte 3
#       define      CMCC_CS_IncGatewayRecord_cUGOutgoingAccessIndicator_present 0x02
#       define      CMCC_CS_IncGatewayRecord_cUGOutgoingAccessIndicator_present_byte 3
#       define      CMCC_CS_IncGatewayRecord_cUGInterlockCode_present 0x01
#       define      CMCC_CS_IncGatewayRecord_cUGInterlockCode_present_byte 3
#       define      CMCC_CS_IncGatewayRecord_cUGIncomingAccessUsed_present 0x80
#       define      CMCC_CS_IncGatewayRecord_cUGIncomingAccessUsed_present_byte 4
    CMCC_CS_CallEventRecordType recordType;  /* optional; set in bit_mask
                               * CMCC_CS_IncGatewayRecord_recordType_present if
                               * present */
    CMCC_CS_CallingNumber callingNumber;  /* optional; set in bit_mask
                            * CMCC_CS_IncGatewayRecord_callingNumber_present if
                            * present */
    CMCC_CS_CalledNumber calledNumber;  /* optional; set in bit_mask
                             * CMCC_CS_IncGatewayRecord_calledNumber_present if
                             * present */
    CMCC_CS_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                          * CMCC_CS_IncGatewayRecord_recordingEntity_present if
                          * present */
    CMCC_CS_ROUTE   mscIncomingROUTE;  /* optional; set in bit_mask
                         * CMCC_CS_IncGatewayRecord_mscIncomingROUTE_present if
                         * present */
    CMCC_CS_ROUTE   mscOutgoingROUTE;  /* optional; set in bit_mask
                         * CMCC_CS_IncGatewayRecord_mscOutgoingROUTE_present if
                         * present */
    CMCC_CS_TimeStamp seizureTime;  /* optional; set in bit_mask
                              * CMCC_CS_IncGatewayRecord_seizureTime_present if
                              * present */
    CMCC_CS_TimeStamp answerTime;  /* optional; set in bit_mask
                               * CMCC_CS_IncGatewayRecord_answerTime_present if
                               * present */
    CMCC_CS_TimeStamp releaseTime;  /* optional; set in bit_mask
                              * CMCC_CS_IncGatewayRecord_releaseTime_present if
                              * present */
    CMCC_CS_CallDuration callDuration;  /* optional; set in bit_mask
                             * CMCC_CS_IncGatewayRecord_callDuration_present if
                             * present */
    CMCC_CS_CauseForTerm causeForTerm;  /* optional; set in bit_mask
                             * CMCC_CS_IncGatewayRecord_causeForTerm_present if
                             * present */
    CMCC_CS_Diagnostics diagnostics;  /* optional; set in bit_mask
                              * CMCC_CS_IncGatewayRecord_diagnostics_present if
                              * present */
    CMCC_CS_CallReference callReference;  /* optional; set in bit_mask
                            * CMCC_CS_IncGatewayRecord_callReference_present if
                            * present */
    CMCC_CS_SequenceNumber sequenceNumber;  /* optional; set in bit_mask
                           * CMCC_CS_IncGatewayRecord_sequenceNumber_present if
                           * present */
    struct CMCC_CS_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                         * CMCC_CS_IncGatewayRecord_recordExtensions_present if
                         * present */
    CMCC_CS_PartialRecordType partialRecordType;  /* optional; set in bit_mask
                        * CMCC_CS_IncGatewayRecord_partialRecordType_present if
                        * present */
    CMCC_CS_ISDN_BC iSDN_BC;  /* optional; set in bit_mask
                               * CMCC_CS_iSDN_BC_present if present */
    CMCC_CS_LLC     lLC;  /* optional; set in bit_mask CMCC_CS_lLC_present if
                           * present */
    CMCC_CS_HLC     hLC;  /* optional; set in bit_mask CMCC_CS_hLC_present if
                           * present */
    CMCC_CS_BasicServiceCode basicService;  /* optional; set in bit_mask
                             * CMCC_CS_IncGatewayRecord_basicService_present if
                             * present */
    CMCC_CS_AdditionalChgInfo additionalChgInfo;  /* optional; set in bit_mask
                        * CMCC_CS_IncGatewayRecord_additionalChgInfo_present if
                        * present */
    CMCC_CS_ChargedParty chargedParty;  /* optional; set in bit_mask
                             * CMCC_CS_IncGatewayRecord_chargedParty_present if
                             * present */
    CMCC_CS_OriginalCalledNumber originalCalledNumber;  /* optional; set in
                                   * bit_mask
                     * CMCC_CS_IncGatewayRecord_originalCalledNumber_present if
                     * present */
    CMCC_CS_RateIndication rateIndication;  /* optional; set in bit_mask
                           * CMCC_CS_IncGatewayRecord_rateIndication_present if
                           * present */
    CMCC_CS_RoamingNumber roamingNumber;  /* optional; set in bit_mask
                            * CMCC_CS_IncGatewayRecord_roamingNumber_present if
                            * present */
    CMCC_CS_MSCCIC  mscIncomingCircuit;  /* optional; set in bit_mask
                       * CMCC_CS_IncGatewayRecord_mscIncomingCircuit_present if
                       * present */
    CMCC_CS_MSCId   orgMSCId;  /* optional; set in bit_mask
                                * CMCC_CS_IncGatewayRecord_orgMSCId_present if
                                * present */
    CMCC_CS_EmlppPriority callEmlppPriority;  /* optional; set in bit_mask
                        * CMCC_CS_IncGatewayRecord_callEmlppPriority_present if
                        * present */
    CMCC_CS_EASubscriberInfo eaSubscriberInfo;  /* optional; set in bit_mask
                         * CMCC_CS_IncGatewayRecord_eaSubscriberInfo_present if
                         * present */
    CMCC_CS_SelectedCIC selectedCIC;  /* optional; set in bit_mask
                              * CMCC_CS_IncGatewayRecord_selectedCIC_present if
                              * present */
    CMCC_CS_CUGOutgoingAccessIndicator cUGOutgoingAccessIndicator;  
                                  /* optional; set in bit_mask
               * CMCC_CS_IncGatewayRecord_cUGOutgoingAccessIndicator_present if
               * present */
    CMCC_CS_CUGInterlockCode cUGInterlockCode;  /* optional; set in bit_mask
                         * CMCC_CS_IncGatewayRecord_cUGInterlockCode_present if
                         * present */
    CMCC_CS_CUGIncomingAccessUsed cUGIncomingAccessUsed;  /* optional; set in
                                   * bit_mask
                    * CMCC_CS_IncGatewayRecord_cUGIncomingAccessUsed_present if
                    * present */
} CMCC_CS_IncGatewayRecord;

typedef struct CMCC_CS_OutGatewayRecord {
    unsigned int    bit_mask;
#       define      CMCC_CS_OutGatewayRecord_recordType_present 0x80000000
#       define      CMCC_CS_OutGatewayRecord_callingNumber_present 0x40000000
#       define      CMCC_CS_OutGatewayRecord_calledNumber_present 0x20000000
#       define      CMCC_CS_OutGatewayRecord_recordingEntity_present 0x10000000
#       define      CMCC_CS_OutGatewayRecord_mscIncomingROUTE_present 0x08000000
#       define      CMCC_CS_OutGatewayRecord_mscOutgoingROUTE_present 0x04000000
#       define      CMCC_CS_OutGatewayRecord_seizureTime_present 0x02000000
#       define      CMCC_CS_OutGatewayRecord_answerTime_present 0x01000000
#       define      CMCC_CS_OutGatewayRecord_releaseTime_present 0x00800000
#       define      CMCC_CS_OutGatewayRecord_callDuration_present 0x00400000
#       define      CMCC_CS_OutGatewayRecord_causeForTerm_present 0x00200000
#       define      CMCC_CS_OutGatewayRecord_diagnostics_present 0x00100000
#       define      CMCC_CS_OutGatewayRecord_callReference_present 0x00080000
#       define      CMCC_CS_OutGatewayRecord_sequenceNumber_present 0x00040000
#       define      CMCC_CS_OutGatewayRecord_recordExtensions_present 0x00020000
#       define      CMCC_CS_OutGatewayRecord_partialRecordType_present 0x00010000
#       define      CMCC_CS_OutGatewayRecord_basicService_present 0x00008000
#       define      CMCC_CS_OutGatewayRecord_additionalChgInfo_present 0x00004000
#       define      CMCC_CS_OutGatewayRecord_chargedParty_present 0x00002000
#       define      CMCC_CS_OutGatewayRecord_originalCalledNumber_present 0x00001000
#       define      CMCC_CS_OutGatewayRecord_rateIndication_present 0x00000800
#       define      CMCC_CS_OutGatewayRecord_roamingNumber_present 0x00000400
#       define      CMCC_CS_OutGatewayRecord_mscOutgoingCircuit_present 0x00000200
#       define      CMCC_CS_OutGatewayRecord_orgMSCId_present 0x00000100
#       define      CMCC_CS_OutGatewayRecord_eaSubscriberInfo_present 0x00000080
#       define      CMCC_CS_OutGatewayRecord_selectedCIC_present 0x00000040
#       define      CMCC_CS_OutGatewayRecord_callEmlppPriority_present 0x00000020
#       define      CMCC_CS_OutGatewayRecord_cUGOutgoingAccessIndicator_present 0x00000010
#       define      CMCC_CS_OutGatewayRecord_cUGInterlockCode_present 0x00000008
#       define      CMCC_CS_OutGatewayRecord_cUGIncomingAccessUsed_present 0x00000004
    CMCC_CS_CallEventRecordType recordType;  /* optional; set in bit_mask
                               * CMCC_CS_OutGatewayRecord_recordType_present if
                               * present */
    CMCC_CS_CallingNumber callingNumber;  /* optional; set in bit_mask
                            * CMCC_CS_OutGatewayRecord_callingNumber_present if
                            * present */
    CMCC_CS_CalledNumber calledNumber;  /* optional; set in bit_mask
                             * CMCC_CS_OutGatewayRecord_calledNumber_present if
                             * present */
    CMCC_CS_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                          * CMCC_CS_OutGatewayRecord_recordingEntity_present if
                          * present */
    CMCC_CS_ROUTE   mscIncomingROUTE;  /* optional; set in bit_mask
                         * CMCC_CS_OutGatewayRecord_mscIncomingROUTE_present if
                         * present */
    CMCC_CS_ROUTE   mscOutgoingROUTE;  /* optional; set in bit_mask
                         * CMCC_CS_OutGatewayRecord_mscOutgoingROUTE_present if
                         * present */
    CMCC_CS_TimeStamp seizureTime;  /* optional; set in bit_mask
                              * CMCC_CS_OutGatewayRecord_seizureTime_present if
                              * present */
    CMCC_CS_TimeStamp answerTime;  /* optional; set in bit_mask
                               * CMCC_CS_OutGatewayRecord_answerTime_present if
                               * present */
    CMCC_CS_TimeStamp releaseTime;  /* optional; set in bit_mask
                              * CMCC_CS_OutGatewayRecord_releaseTime_present if
                              * present */
    CMCC_CS_CallDuration callDuration;  /* optional; set in bit_mask
                             * CMCC_CS_OutGatewayRecord_callDuration_present if
                             * present */
    CMCC_CS_CauseForTerm causeForTerm;  /* optional; set in bit_mask
                             * CMCC_CS_OutGatewayRecord_causeForTerm_present if
                             * present */
    CMCC_CS_Diagnostics diagnostics;  /* optional; set in bit_mask
                              * CMCC_CS_OutGatewayRecord_diagnostics_present if
                              * present */
    CMCC_CS_CallReference callReference;  /* optional; set in bit_mask
                            * CMCC_CS_OutGatewayRecord_callReference_present if
                            * present */
    CMCC_CS_SequenceNumber sequenceNumber;  /* optional; set in bit_mask
                           * CMCC_CS_OutGatewayRecord_sequenceNumber_present if
                           * present */
    struct CMCC_CS_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                         * CMCC_CS_OutGatewayRecord_recordExtensions_present if
                         * present */
    CMCC_CS_PartialRecordType partialRecordType;  /* optional; set in bit_mask
                        * CMCC_CS_OutGatewayRecord_partialRecordType_present if
                        * present */
    CMCC_CS_BasicServiceCode basicService;  /* optional; set in bit_mask
                             * CMCC_CS_OutGatewayRecord_basicService_present if
                             * present */
    CMCC_CS_AdditionalChgInfo additionalChgInfo;  /* optional; set in bit_mask
                        * CMCC_CS_OutGatewayRecord_additionalChgInfo_present if
                        * present */
    CMCC_CS_ChargedParty chargedParty;  /* optional; set in bit_mask
                             * CMCC_CS_OutGatewayRecord_chargedParty_present if
                             * present */
    CMCC_CS_OriginalCalledNumber originalCalledNumber;  /* optional; set in
                                   * bit_mask
                     * CMCC_CS_OutGatewayRecord_originalCalledNumber_present if
                     * present */
    CMCC_CS_RateIndication rateIndication;  /* optional; set in bit_mask
                           * CMCC_CS_OutGatewayRecord_rateIndication_present if
                           * present */
    CMCC_CS_RoamingNumber roamingNumber;  /* optional; set in bit_mask
                            * CMCC_CS_OutGatewayRecord_roamingNumber_present if
                            * present */
    CMCC_CS_MSCCIC  mscOutgoingCircuit;  /* optional; set in bit_mask
                       * CMCC_CS_OutGatewayRecord_mscOutgoingCircuit_present if
                       * present */
    CMCC_CS_MSCId   orgMSCId;  /* optional; set in bit_mask
                                * CMCC_CS_OutGatewayRecord_orgMSCId_present if
                                * present */
    CMCC_CS_EASubscriberInfo eaSubscriberInfo;  /* optional; set in bit_mask
                         * CMCC_CS_OutGatewayRecord_eaSubscriberInfo_present if
                         * present */
    CMCC_CS_SelectedCIC selectedCIC;  /* optional; set in bit_mask
                              * CMCC_CS_OutGatewayRecord_selectedCIC_present if
                              * present */
    CMCC_CS_EmlppPriority callEmlppPriority;  /* optional; set in bit_mask
                        * CMCC_CS_OutGatewayRecord_callEmlppPriority_present if
                        * present */
    CMCC_CS_CUGOutgoingAccessIndicator cUGOutgoingAccessIndicator;  
                                  /* optional; set in bit_mask
               * CMCC_CS_OutGatewayRecord_cUGOutgoingAccessIndicator_present if
               * present */
    CMCC_CS_CUGInterlockCode cUGInterlockCode;  /* optional; set in bit_mask
                         * CMCC_CS_OutGatewayRecord_cUGInterlockCode_present if
                         * present */
    CMCC_CS_CUGIncomingAccessUsed cUGIncomingAccessUsed;  /* optional; set in
                                   * bit_mask
                    * CMCC_CS_OutGatewayRecord_cUGIncomingAccessUsed_present if
                    * present */
} CMCC_CS_OutGatewayRecord;

typedef struct CMCC_CS_BasicService {
    unsigned short  length;
    unsigned char   value[1];
} CMCC_CS_BasicService;

typedef struct CMCC_CS_TransitCallRecord {
    unsigned int    bit_mask;
#       define      CMCC_CS_TransitCallRecord_recordType_present 0x80000000
#       define      CMCC_CS_TransitCallRecord_recordingEntity_present 0x40000000
#       define      CMCC_CS_TransitCallRecord_mscIncomingROUTE_present 0x20000000
#       define      CMCC_CS_TransitCallRecord_mscOutgoingROUTE_present 0x10000000
#       define      CMCC_CS_TransitCallRecord_callingNumber_present 0x08000000
#       define      CMCC_CS_TransitCallRecord_calledNumber_present 0x04000000
#       define      CMCC_CS_isdnBasicService_present 0x02000000
#       define      CMCC_CS_seizureTimestamp_present 0x01000000
#       define      CMCC_CS_answerTimestamp_present 0x00800000
#       define      CMCC_CS_releaseTimestamp_present 0x00400000
#       define      CMCC_CS_TransitCallRecord_callDuration_present 0x00200000
#       define      CMCC_CS_TransitCallRecord_causeForTerm_present 0x00100000
#       define      CMCC_CS_TransitCallRecord_diagnostics_present 0x00080000
#       define      CMCC_CS_TransitCallRecord_callReference_present 0x00040000
#       define      CMCC_CS_TransitCallRecord_sequenceNumber_present 0x00020000
#       define      CMCC_CS_TransitCallRecord_recordExtensions_present 0x00010000
#       define      CMCC_CS_TransitCallRecord_partialRecordType_present 0x00008000
#       define      CMCC_CS_TransitCallRecord_basicService_present 0x00004000
#       define      CMCC_CS_TransitCallRecord_additionalChgInfo_present 0x00002000
#       define      CMCC_CS_TransitCallRecord_originalCalledNumber_present 0x00001000
#       define      CMCC_CS_TransitCallRecord_rateIndication_present 0x00000800
#       define      CMCC_CS_TransitCallRecord_mscOutgoingCircuit_present 0x00000400
#       define      CMCC_CS_TransitCallRecord_mscIncomingCircuit_present 0x00000200
#       define      CMCC_CS_TransitCallRecord_orgMSCId_present 0x00000100
#       define      CMCC_CS_TransitCallRecord_callEmlppPriority_present 0x00000080
#       define      CMCC_CS_TransitCallRecord_eaSubscriberInfo_present 0x00000040
#       define      CMCC_CS_TransitCallRecord_selectedCIC_present 0x00000020
#       define      CMCC_CS_TransitCallRecord_cUGOutgoingAccessIndicator_present 0x00000010
#       define      CMCC_CS_TransitCallRecord_cUGInterlockCode_present 0x00000008
#       define      CMCC_CS_TransitCallRecord_cUGIncomingAccessUsed_present 0x00000004
    CMCC_CS_CallEventRecordType recordType;  /* optional; set in bit_mask
                              * CMCC_CS_TransitCallRecord_recordType_present if
                              * present */
    CMCC_CS_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                         * CMCC_CS_TransitCallRecord_recordingEntity_present if
                         * present */
    CMCC_CS_ROUTE   mscIncomingROUTE;  /* optional; set in bit_mask
                        * CMCC_CS_TransitCallRecord_mscIncomingROUTE_present if
                        * present */
    CMCC_CS_ROUTE   mscOutgoingROUTE;  /* optional; set in bit_mask
                        * CMCC_CS_TransitCallRecord_mscOutgoingROUTE_present if
                        * present */
    CMCC_CS_CallingNumber callingNumber;  /* optional; set in bit_mask
                           * CMCC_CS_TransitCallRecord_callingNumber_present if
                           * present */
    CMCC_CS_CalledNumber calledNumber;  /* optional; set in bit_mask
                            * CMCC_CS_TransitCallRecord_calledNumber_present if
                            * present */
    CMCC_CS_BasicService isdnBasicService;  /* optional; set in bit_mask
                                             * CMCC_CS_isdnBasicService_present
                                             * if present */
    CMCC_CS_TimeStamp seizureTimestamp;  /* optional; set in bit_mask
                                          * CMCC_CS_seizureTimestamp_present if
                                          * present */
    CMCC_CS_TimeStamp answerTimestamp;  /* optional; set in bit_mask
                                         * CMCC_CS_answerTimestamp_present if
                                         * present */
    CMCC_CS_TimeStamp releaseTimestamp;  /* optional; set in bit_mask
                                          * CMCC_CS_releaseTimestamp_present if
                                          * present */
    CMCC_CS_CallDuration callDuration;  /* optional; set in bit_mask
                            * CMCC_CS_TransitCallRecord_callDuration_present if
                            * present */
    CMCC_CS_CauseForTerm causeForTerm;  /* optional; set in bit_mask
                            * CMCC_CS_TransitCallRecord_causeForTerm_present if
                            * present */
    CMCC_CS_Diagnostics diagnostics;  /* optional; set in bit_mask
                             * CMCC_CS_TransitCallRecord_diagnostics_present if
                             * present */
    CMCC_CS_CallReference callReference;  /* optional; set in bit_mask
                           * CMCC_CS_TransitCallRecord_callReference_present if
                           * present */
    CMCC_CS_SequenceNumber sequenceNumber;  /* optional; set in bit_mask
                          * CMCC_CS_TransitCallRecord_sequenceNumber_present if
                          * present */
    struct CMCC_CS_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                        * CMCC_CS_TransitCallRecord_recordExtensions_present if
                        * present */
    CMCC_CS_PartialRecordType partialRecordType;  /* optional; set in bit_mask
                       * CMCC_CS_TransitCallRecord_partialRecordType_present if
                       * present */
    CMCC_CS_BasicServiceCode basicService;  /* optional; set in bit_mask
                            * CMCC_CS_TransitCallRecord_basicService_present if
                            * present */
    CMCC_CS_AdditionalChgInfo additionalChgInfo;  /* optional; set in bit_mask
                       * CMCC_CS_TransitCallRecord_additionalChgInfo_present if
                       * present */
    CMCC_CS_OriginalCalledNumber originalCalledNumber;  /* optional; set in
                                   * bit_mask
                    * CMCC_CS_TransitCallRecord_originalCalledNumber_present if
                    * present */
    CMCC_CS_RateIndication rateIndication;  /* optional; set in bit_mask
                          * CMCC_CS_TransitCallRecord_rateIndication_present if
                          * present */
    CMCC_CS_MSCCIC  mscOutgoingCircuit;  /* optional; set in bit_mask
                      * CMCC_CS_TransitCallRecord_mscOutgoingCircuit_present if
                      * present */
    CMCC_CS_MSCCIC  mscIncomingCircuit;  /* optional; set in bit_mask
                      * CMCC_CS_TransitCallRecord_mscIncomingCircuit_present if
                      * present */
    CMCC_CS_MSCId   orgMSCId;  /* optional; set in bit_mask
                                * CMCC_CS_TransitCallRecord_orgMSCId_present if
                                * present */
    CMCC_CS_EmlppPriority callEmlppPriority;  /* optional; set in bit_mask
                       * CMCC_CS_TransitCallRecord_callEmlppPriority_present if
                       * present */
    CMCC_CS_EASubscriberInfo eaSubscriberInfo;  /* optional; set in bit_mask
                        * CMCC_CS_TransitCallRecord_eaSubscriberInfo_present if
                        * present */
    CMCC_CS_SelectedCIC selectedCIC;  /* optional; set in bit_mask
                             * CMCC_CS_TransitCallRecord_selectedCIC_present if
                             * present */
    CMCC_CS_CUGOutgoingAccessIndicator cUGOutgoingAccessIndicator;  
                                  /* optional; set in bit_mask
              * CMCC_CS_TransitCallRecord_cUGOutgoingAccessIndicator_present if
              * present */
    CMCC_CS_CUGInterlockCode cUGInterlockCode;  /* optional; set in bit_mask
                        * CMCC_CS_TransitCallRecord_cUGInterlockCode_present if
                        * present */
    CMCC_CS_CUGIncomingAccessUsed cUGIncomingAccessUsed;  /* optional; set in
                                   * bit_mask
                   * CMCC_CS_TransitCallRecord_cUGIncomingAccessUsed_present if
                   * present */
} CMCC_CS_TransitCallRecord;

typedef struct CMCC_CS_MessageReference {
    unsigned int    length;
    unsigned char   *value;
} CMCC_CS_MessageReference;

typedef CMCC_CS_Diagnostics CMCC_CS_SMSResult;

typedef struct CMCC_CS_SmsTpDestinationNumber {
    unsigned int    length;
    unsigned char   *value;
} CMCC_CS_SmsTpDestinationNumber;

typedef enum CMCC_CS_DefaultSMS_Handling {
    CMCC_CS_continueTransaction = 0,
    CMCC_CS_releaseTransaction = 1
} CMCC_CS_DefaultSMS_Handling;

typedef struct CMCC_CS_CAMELSMSInformation {
    unsigned char   bit_mask;
#       define      CMCC_CS_CAMELSMSInformation_gsm_SCFAddress_present 0x80
#       define      CMCC_CS_CAMELSMSInformation_serviceKey_present 0x40
#       define      CMCC_CS_defaultSMSHandling_present 0x20
#       define      CMCC_CS_CAMELSMSInformation_freeFormatData_present 0x10
#       define      CMCC_CS_CAMELSMSInformation_callingPartyNumber_present 0x08
#       define      CMCC_CS_destinationSubscriberNumber_present 0x04
#       define      CMCC_CS_cAMELSMSCAddress_present 0x02
#       define      CMCC_CS_smsReferenceNumber_present 0x01
    CMCC_CS_Gsm_SCFAddress gsm_SCFAddress;  /* optional; set in bit_mask
                        * CMCC_CS_CAMELSMSInformation_gsm_SCFAddress_present if
                        * present */
    CMCC_CS_ServiceKey serviceKey;  /* optional; set in bit_mask
                            * CMCC_CS_CAMELSMSInformation_serviceKey_present if
                            * present */
    CMCC_CS_DefaultSMS_Handling defaultSMSHandling;  /* optional; set in
                                   * bit_mask CMCC_CS_defaultSMSHandling_present
                                   * if present */
    CMCC_CS_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                        * CMCC_CS_CAMELSMSInformation_freeFormatData_present if
                        * present */
    CMCC_CS_CallingNumber callingPartyNumber;  /* optional; set in bit_mask
                    * CMCC_CS_CAMELSMSInformation_callingPartyNumber_present if
                    * present */
    CMCC_CS_CalledNumber destinationSubscriberNumber;  /* optional; set in
                                   * bit_mask
                                   * CMCC_CS_destinationSubscriberNumber_present
                                   * if present */
    CMCC_CS_AddressString cAMELSMSCAddress;  /* optional; set in bit_mask
                                              * CMCC_CS_cAMELSMSCAddress_present
                                              * if present */
    CMCC_CS_CallReferenceNumber smsReferenceNumber;  /* optional; set in
                                   * bit_mask CMCC_CS_smsReferenceNumber_present
                                   * if present */
} CMCC_CS_CAMELSMSInformation;

typedef struct CMCC_CS_SmsUserDataType {
    unsigned short  length;
    unsigned char   value[1];
} CMCC_CS_SmsUserDataType;

typedef struct CMCC_CS_SMSTEXT {
    unsigned int    length;
    unsigned char   *value;
} CMCC_CS_SMSTEXT;

typedef unsigned char   CMCC_CS_MaximumNumberOfSMSInTheConcatenatedSMS;

typedef unsigned short  CMCC_CS_ConcatenatedSMSReferenceNumber;

typedef unsigned char   CMCC_CS_SequenceNumberOfTheCurrentSMS;

typedef struct CMCC_CS_MOSMSRecord {
    unsigned int    bit_mask;
#       define      CMCC_CS_MOSMSRecord_recordType_present 0x80000000
#       define      CMCC_CS_MOSMSRecord_servedIMSI_present 0x40000000
#       define      CMCC_CS_MOSMSRecord_servedIMEI_present 0x20000000
#       define      CMCC_CS_MOSMSRecord_servedMSISDN_present 0x10000000
#       define      CMCC_CS_MOSMSRecord_msClassmark_present 0x08000000
#       define      CMCC_CS_MOSMSRecord_serviceCentre_present 0x04000000
#       define      CMCC_CS_MOSMSRecord_recordingEntity_present 0x02000000
#       define      CMCC_CS_MOSMSRecord_location_present 0x01000000
#       define      CMCC_CS_messageReference_present 0x00800000
#       define      CMCC_CS_originationTime_present 0x00400000
#       define      CMCC_CS_MOSMSRecord_smsResult_present 0x00200000
#       define      CMCC_CS_MOSMSRecord_recordExtensions_present 0x00100000
#       define      CMCC_CS_destinationNumber_present 0x00080000
#       define      CMCC_CS_MOSMSRecord_cAMELSMSInformation_present 0x00040000
#       define      CMCC_CS_MOSMSRecord_systemType_present 0x00020000
#       define      CMCC_CS_MOSMSRecord_basicService_present 0x00010000
#       define      CMCC_CS_MOSMSRecord_additionalChgInfo_present 0x00008000
#       define      CMCC_CS_MOSMSRecord_classmark3_present 0x00004000
#       define      CMCC_CS_MOSMSRecord_chargedParty_present 0x00002000
#       define      CMCC_CS_MOSMSRecord_orgRNCorBSCId_present 0x00001000
#       define      CMCC_CS_MOSMSRecord_orgMSCId_present 0x00000800
#       define      CMCC_CS_MOSMSRecord_globalAreaID_present 0x00000400
#       define      CMCC_CS_MOSMSRecord_subscriberCategory_present 0x00000200
#       define      CMCC_CS_MOSMSRecord_firstmccmnc_present 0x00000100
#       define      CMCC_CS_MOSMSRecord_smsUserDataType_present 0x00000080
#       define      CMCC_CS_MOSMSRecord_smstext_present 0x00000040
#       define      CMCC_CS_MOSMSRecord_maximumNumberOfSMSInTheConcatenatedSMS_present 0x00000020
#       define      CMCC_CS_MOSMSRecord_concatenatedSMSReferenceNumber_present 0x00000010
#       define      CMCC_CS_MOSMSRecord_sequenceNumberOfTheCurrentSMS_present 0x00000008
#       define      CMCC_CS_MOSMSRecord_hotBillingTag_present 0x00000004
    CMCC_CS_CallEventRecordType recordType;  /* optional; set in bit_mask
                                    * CMCC_CS_MOSMSRecord_recordType_present if
                                    * present */
    CMCC_CS_IMSI    servedIMSI;  /* optional; set in bit_mask
                                  * CMCC_CS_MOSMSRecord_servedIMSI_present if
                                  * present */
    CMCC_CS_IMEI    servedIMEI;  /* optional; set in bit_mask
                                  * CMCC_CS_MOSMSRecord_servedIMEI_present if
                                  * present */
    CMCC_CS_MSISDN  servedMSISDN;  /* optional; set in bit_mask
                                    * CMCC_CS_MOSMSRecord_servedMSISDN_present
                                    * if present */
    CMCC_CS_Classmark msClassmark;  /* optional; set in bit_mask
                                     * CMCC_CS_MOSMSRecord_msClassmark_present
                                     * if present */
    CMCC_CS_AddressString serviceCentre;  /* optional; set in bit_mask
                                 * CMCC_CS_MOSMSRecord_serviceCentre_present if
                                 * present */
    CMCC_CS_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                               * CMCC_CS_MOSMSRecord_recordingEntity_present if
                               * present */
    CMCC_CS_LocationAreaAndCell location;  /* optional; set in bit_mask
                                      * CMCC_CS_MOSMSRecord_location_present if
                                      * present */
    CMCC_CS_MessageReference messageReference;  /* optional; set in bit_mask
                                          * CMCC_CS_messageReference_present if
                                          * present */
    CMCC_CS_TimeStamp originationTime;  /* optional; set in bit_mask
                                         * CMCC_CS_originationTime_present if
                                         * present */
    CMCC_CS_SMSResult smsResult;  /* optional; set in bit_mask
                                   * CMCC_CS_MOSMSRecord_smsResult_present if
                                   * present */
    struct CMCC_CS_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                              * CMCC_CS_MOSMSRecord_recordExtensions_present if
                              * present */
    CMCC_CS_SmsTpDestinationNumber destinationNumber;  /* optional; set in
                                   * bit_mask CMCC_CS_destinationNumber_present
                                   * if present */
    CMCC_CS_CAMELSMSInformation cAMELSMSInformation;  /* optional; set in
                                   * bit_mask
                           * CMCC_CS_MOSMSRecord_cAMELSMSInformation_present if
                           * present */
    CMCC_CS_SystemType systemType;  /* optional; set in bit_mask
                                     * CMCC_CS_MOSMSRecord_systemType_present if
                                     * present */
    CMCC_CS_BasicServiceCode basicService;  /* optional; set in bit_mask
                                  * CMCC_CS_MOSMSRecord_basicService_present if
                                  * present */
    CMCC_CS_AdditionalChgInfo additionalChgInfo;  /* optional; set in bit_mask
                             * CMCC_CS_MOSMSRecord_additionalChgInfo_present if
                             * present */
    CMCC_CS_Classmark3 classmark3;  /* optional; set in bit_mask
                                     * CMCC_CS_MOSMSRecord_classmark3_present if
                                     * present */
    CMCC_CS_ChargedParty chargedParty;  /* optional; set in bit_mask
                                  * CMCC_CS_MOSMSRecord_chargedParty_present if
                                  * present */
    CMCC_CS_RNCorBSCId orgRNCorBSCId;  /* optional; set in bit_mask
                                 * CMCC_CS_MOSMSRecord_orgRNCorBSCId_present if
                                 * present */
    CMCC_CS_MSCId   orgMSCId;  /* optional; set in bit_mask
                                * CMCC_CS_MOSMSRecord_orgMSCId_present if
                                * present */
    CMCC_CS_GAI     globalAreaID;  /* optional; set in bit_mask
                                    * CMCC_CS_MOSMSRecord_globalAreaID_present
                                    * if present */
    CMCC_CS_SubscriberCategory subscriberCategory;  /* optional; set in bit_mask
                            * CMCC_CS_MOSMSRecord_subscriberCategory_present if
                            * present */
    CMCC_CS_MCCMNC  firstmccmnc;  /* optional; set in bit_mask
                                   * CMCC_CS_MOSMSRecord_firstmccmnc_present if
                                   * present */
    CMCC_CS_SmsUserDataType smsUserDataType;  /* optional; set in bit_mask
                               * CMCC_CS_MOSMSRecord_smsUserDataType_present if
                               * present */
    CMCC_CS_SMSTEXT smstext;  /* optional; set in bit_mask
                               * CMCC_CS_MOSMSRecord_smstext_present if
                               * present */
    CMCC_CS_MaximumNumberOfSMSInTheConcatenatedSMS maximumNumberOfSMSInTheConcatenatedSMS;                              /* optional; set in bit_mask
        * CMCC_CS_MOSMSRecord_maximumNumberOfSMSInTheConcatenatedSMS_present if
        * present */
    CMCC_CS_ConcatenatedSMSReferenceNumber concatenatedSMSReferenceNumber;  
                                        /* optional; set in bit_mask
                * CMCC_CS_MOSMSRecord_concatenatedSMSReferenceNumber_present if
                * present */
    CMCC_CS_SequenceNumberOfTheCurrentSMS sequenceNumberOfTheCurrentSMS;  
                                        /* optional; set in bit_mask
                 * CMCC_CS_MOSMSRecord_sequenceNumberOfTheCurrentSMS_present if
                 * present */
    CMCC_CS_HotBillingTag hotBillingTag;  /* optional; set in bit_mask
                                 * CMCC_CS_MOSMSRecord_hotBillingTag_present if
                                 * present */
} CMCC_CS_MOSMSRecord;

typedef struct CMCC_CS_MTSMSRecord {
    unsigned int    bit_mask;
#       define      CMCC_CS_MTSMSRecord_recordType_present 0x80000000
#       define      CMCC_CS_MTSMSRecord_serviceCentre_present 0x40000000
#       define      CMCC_CS_MTSMSRecord_servedIMSI_present 0x20000000
#       define      CMCC_CS_MTSMSRecord_servedIMEI_present 0x10000000
#       define      CMCC_CS_MTSMSRecord_servedMSISDN_present 0x08000000
#       define      CMCC_CS_MTSMSRecord_msClassmark_present 0x04000000
#       define      CMCC_CS_MTSMSRecord_recordingEntity_present 0x02000000
#       define      CMCC_CS_MTSMSRecord_location_present 0x01000000
#       define      CMCC_CS_deliveryTime_present 0x00800000
#       define      CMCC_CS_MTSMSRecord_smsResult_present 0x00400000
#       define      CMCC_CS_MTSMSRecord_recordExtensions_present 0x00200000
#       define      CMCC_CS_MTSMSRecord_systemType_present 0x00100000
#       define      CMCC_CS_MTSMSRecord_cAMELSMSInformation_present 0x00080000
#       define      CMCC_CS_MTSMSRecord_basicService_present 0x00040000
#       define      CMCC_CS_MTSMSRecord_additionalChgInfo_present 0x00020000
#       define      CMCC_CS_MTSMSRecord_classmark3_present 0x00010000
#       define      CMCC_CS_MTSMSRecord_chargedParty_present 0x00008000
#       define      CMCC_CS_MTSMSRecord_orgRNCorBSCId_present 0x00004000
#       define      CMCC_CS_MTSMSRecord_orgMSCId_present 0x00002000
#       define      CMCC_CS_MTSMSRecord_globalAreaID_present 0x00001000
#       define      CMCC_CS_MTSMSRecord_subscriberCategory_present 0x00000800
#       define      CMCC_CS_MTSMSRecord_firstmccmnc_present 0x00000400
#       define      CMCC_CS_MTSMSRecord_smsUserDataType_present 0x00000200
#       define      CMCC_CS_MTSMSRecord_smstext_present 0x00000100
#       define      CMCC_CS_MTSMSRecord_maximumNumberOfSMSInTheConcatenatedSMS_present 0x00000080
#       define      CMCC_CS_MTSMSRecord_concatenatedSMSReferenceNumber_present 0x00000040
#       define      CMCC_CS_MTSMSRecord_sequenceNumberOfTheCurrentSMS_present 0x00000020
#       define      CMCC_CS_MTSMSRecord_hotBillingTag_present 0x00000010
#       define      CMCC_CS_origination_present 0x00000008
    CMCC_CS_CallEventRecordType recordType;  /* optional; set in bit_mask
                                    * CMCC_CS_MTSMSRecord_recordType_present if
                                    * present */
    CMCC_CS_AddressString serviceCentre;  /* optional; set in bit_mask
                                 * CMCC_CS_MTSMSRecord_serviceCentre_present if
                                 * present */
    CMCC_CS_IMSI    servedIMSI;  /* optional; set in bit_mask
                                  * CMCC_CS_MTSMSRecord_servedIMSI_present if
                                  * present */
    CMCC_CS_IMEI    servedIMEI;  /* optional; set in bit_mask
                                  * CMCC_CS_MTSMSRecord_servedIMEI_present if
                                  * present */
    CMCC_CS_MSISDN  servedMSISDN;  /* optional; set in bit_mask
                                    * CMCC_CS_MTSMSRecord_servedMSISDN_present
                                    * if present */
    CMCC_CS_Classmark msClassmark;  /* optional; set in bit_mask
                                     * CMCC_CS_MTSMSRecord_msClassmark_present
                                     * if present */
    CMCC_CS_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                               * CMCC_CS_MTSMSRecord_recordingEntity_present if
                               * present */
    CMCC_CS_LocationAreaAndCell location;  /* optional; set in bit_mask
                                      * CMCC_CS_MTSMSRecord_location_present if
                                      * present */
    CMCC_CS_TimeStamp deliveryTime;  /* optional; set in bit_mask
                                      * CMCC_CS_deliveryTime_present if
                                      * present */
    CMCC_CS_SMSResult smsResult;  /* optional; set in bit_mask
                                   * CMCC_CS_MTSMSRecord_smsResult_present if
                                   * present */
    struct CMCC_CS_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                              * CMCC_CS_MTSMSRecord_recordExtensions_present if
                              * present */
    CMCC_CS_SystemType systemType;  /* optional; set in bit_mask
                                     * CMCC_CS_MTSMSRecord_systemType_present if
                                     * present */
    CMCC_CS_CAMELSMSInformation cAMELSMSInformation;  /* optional; set in
                                   * bit_mask
                           * CMCC_CS_MTSMSRecord_cAMELSMSInformation_present if
                           * present */
    CMCC_CS_BasicServiceCode basicService;  /* optional; set in bit_mask
                                  * CMCC_CS_MTSMSRecord_basicService_present if
                                  * present */
    CMCC_CS_AdditionalChgInfo additionalChgInfo;  /* optional; set in bit_mask
                             * CMCC_CS_MTSMSRecord_additionalChgInfo_present if
                             * present */
    CMCC_CS_Classmark3 classmark3;  /* optional; set in bit_mask
                                     * CMCC_CS_MTSMSRecord_classmark3_present if
                                     * present */
    CMCC_CS_ChargedParty chargedParty;  /* optional; set in bit_mask
                                  * CMCC_CS_MTSMSRecord_chargedParty_present if
                                  * present */
    CMCC_CS_RNCorBSCId orgRNCorBSCId;  /* optional; set in bit_mask
                                 * CMCC_CS_MTSMSRecord_orgRNCorBSCId_present if
                                 * present */
    CMCC_CS_MSCId   orgMSCId;  /* optional; set in bit_mask
                                * CMCC_CS_MTSMSRecord_orgMSCId_present if
                                * present */
    CMCC_CS_GAI     globalAreaID;  /* optional; set in bit_mask
                                    * CMCC_CS_MTSMSRecord_globalAreaID_present
                                    * if present */
    CMCC_CS_SubscriberCategory subscriberCategory;  /* optional; set in bit_mask
                            * CMCC_CS_MTSMSRecord_subscriberCategory_present if
                            * present */
    CMCC_CS_MCCMNC  firstmccmnc;  /* optional; set in bit_mask
                                   * CMCC_CS_MTSMSRecord_firstmccmnc_present if
                                   * present */
    CMCC_CS_SmsUserDataType smsUserDataType;  /* optional; set in bit_mask
                               * CMCC_CS_MTSMSRecord_smsUserDataType_present if
                               * present */
    CMCC_CS_SMSTEXT smstext;  /* optional; set in bit_mask
                               * CMCC_CS_MTSMSRecord_smstext_present if
                               * present */
    CMCC_CS_MaximumNumberOfSMSInTheConcatenatedSMS maximumNumberOfSMSInTheConcatenatedSMS;                              /* optional; set in bit_mask
        * CMCC_CS_MTSMSRecord_maximumNumberOfSMSInTheConcatenatedSMS_present if
        * present */
    CMCC_CS_ConcatenatedSMSReferenceNumber concatenatedSMSReferenceNumber;  
                                        /* optional; set in bit_mask
                * CMCC_CS_MTSMSRecord_concatenatedSMSReferenceNumber_present if
                * present */
    CMCC_CS_SequenceNumberOfTheCurrentSMS sequenceNumberOfTheCurrentSMS;  
                                        /* optional; set in bit_mask
                 * CMCC_CS_MTSMSRecord_sequenceNumberOfTheCurrentSMS_present if
                 * present */
    CMCC_CS_HotBillingTag hotBillingTag;  /* optional; set in bit_mask
                                 * CMCC_CS_MTSMSRecord_hotBillingTag_present if
                                 * present */
    CMCC_CS_CallingNumber origination;  /* optional; set in bit_mask
                                         * CMCC_CS_origination_present if
                                         * present */
} CMCC_CS_MTSMSRecord;

typedef enum CMCC_CS_SSActionType {
    CMCC_CS_registration = 0,
    CMCC_CS_erasure = 1,
    CMCC_CS_activation = 2,
    CMCC_CS_deactivation = 3,
    CMCC_CS_interrogation = 4,
    CMCC_CS_invocation = 5,
    CMCC_CS_passwordRegistration = 6,
    CMCC_CS_ussdInvocation = 7
} CMCC_CS_SSActionType;

typedef CMCC_CS_AddressString CMCC_CS_ForwardToNumber;

typedef struct CMCC_CS_SSParameters {
    unsigned short  choice;
#       define      CMCC_CS_forwardedToNumber_chosen 1
#       define      CMCC_CS_unstructuredData_chosen 2
    union _union {
        CMCC_CS_ForwardToNumber forwardedToNumber;  /* to choose, set choice to
                                          * CMCC_CS_forwardedToNumber_chosen */
        CMCC_CS__octet1 unstructuredData;  /* to choose, set choice to
                                           * CMCC_CS_unstructuredData_chosen */
    } u;
} CMCC_CS_SSParameters;

typedef CMCC_CS_Diagnostics CMCC_CS_SSActionResult;

typedef struct CMCC_CS_UssdCodingScheme {
    unsigned int    length;
    unsigned char   *value;
} CMCC_CS_UssdCodingScheme;

typedef struct CMCC_CS_UssdString {
    unsigned int    length;
    unsigned char   *value;
} CMCC_CS_UssdString;

typedef unsigned char   CMCC_CS_UssdNotifyCounter;

typedef unsigned char   CMCC_CS_UssdRequestCounter;

typedef struct CMCC_CS_SSActionRecord {
    unsigned int    bit_mask;
#       define      CMCC_CS_SSActionRecord_recordType_present 0x80000000
#       define      CMCC_CS_SSActionRecord_servedIMSI_present 0x40000000
#       define      CMCC_CS_SSActionRecord_servedIMEI_present 0x20000000
#       define      CMCC_CS_SSActionRecord_servedMSISDN_present 0x10000000
#       define      CMCC_CS_SSActionRecord_msClassmark_present 0x08000000
#       define      CMCC_CS_SSActionRecord_recordingEntity_present 0x04000000
#       define      CMCC_CS_SSActionRecord_location_present 0x02000000
#       define      CMCC_CS_basicServices_present 0x01000000
#       define      CMCC_CS_supplService_present 0x00800000
#       define      CMCC_CS_ssAction_present 0x00400000
#       define      CMCC_CS_ssActionTime_present 0x00200000
#       define      CMCC_CS_ssParameters_present 0x00100000
#       define      CMCC_CS_ssActionResult_present 0x00080000
#       define      CMCC_CS_SSActionRecord_callReference_present 0x00040000
#       define      CMCC_CS_SSActionRecord_recordExtensions_present 0x00020000
#       define      CMCC_CS_SSActionRecord_systemType_present 0x00010000
#       define      CMCC_CS_ussdCodingScheme_present 0x00008000
#       define      CMCC_CS_ussdString_present 0x00004000
#       define      CMCC_CS_ussdNotifyCounter_present 0x00002000
#       define      CMCC_CS_ussdRequestCounter_present 0x00001000
#       define      CMCC_CS_SSActionRecord_additionalChgInfo_present 0x00000800
#       define      CMCC_CS_SSActionRecord_classmark3_present 0x00000400
#       define      CMCC_CS_SSActionRecord_chargedParty_present 0x00000200
#       define      CMCC_CS_SSActionRecord_orgRNCorBSCId_present 0x00000100
#       define      CMCC_CS_SSActionRecord_orgMSCId_present 0x00000080
#       define      CMCC_CS_SSActionRecord_globalAreaID_present 0x00000040
#       define      CMCC_CS_SSActionRecord_subscriberCategory_present 0x00000020
#       define      CMCC_CS_SSActionRecord_firstmccmnc_present 0x00000010
#       define      CMCC_CS_SSActionRecord_hotBillingTag_present 0x00000008
    CMCC_CS_CallEventRecordType recordType;  /* optional; set in bit_mask
                                 * CMCC_CS_SSActionRecord_recordType_present if
                                 * present */
    CMCC_CS_IMSI    servedIMSI;  /* optional; set in bit_mask
                                  * CMCC_CS_SSActionRecord_servedIMSI_present if
                                  * present */
    CMCC_CS_IMEI    servedIMEI;  /* optional; set in bit_mask
                                  * CMCC_CS_SSActionRecord_servedIMEI_present if
                                  * present */
    CMCC_CS_MSISDN  servedMSISDN;  /* optional; set in bit_mask
                               * CMCC_CS_SSActionRecord_servedMSISDN_present if
                               * present */
    CMCC_CS_Classmark msClassmark;  /* optional; set in bit_mask
                                * CMCC_CS_SSActionRecord_msClassmark_present if
                                * present */
    CMCC_CS_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                            * CMCC_CS_SSActionRecord_recordingEntity_present if
                            * present */
    CMCC_CS_LocationAreaAndCell location;  /* optional; set in bit_mask
                                   * CMCC_CS_SSActionRecord_location_present if
                                   * present */
    struct CMCC_CS_BasicServices_ *basicServices;  /* optional; set in bit_mask
                                             * CMCC_CS_basicServices_present if
                                             * present */
    CMCC_CS_SS_Code supplService;  /* optional; set in bit_mask
                                    * CMCC_CS_supplService_present if present */
    CMCC_CS_SSActionType ssAction;  /* optional; set in bit_mask
                                     * CMCC_CS_ssAction_present if present */
    CMCC_CS_TimeStamp ssActionTime;  /* optional; set in bit_mask
                                      * CMCC_CS_ssActionTime_present if
                                      * present */
    CMCC_CS_SSParameters ssParameters;  /* optional; set in bit_mask
                                         * CMCC_CS_ssParameters_present if
                                         * present */
    CMCC_CS_SSActionResult ssActionResult;  /* optional; set in bit_mask
                                             * CMCC_CS_ssActionResult_present if
                                             * present */
    CMCC_CS_CallReference callReference;  /* optional; set in bit_mask
                              * CMCC_CS_SSActionRecord_callReference_present if
                              * present */
    struct CMCC_CS_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                           * CMCC_CS_SSActionRecord_recordExtensions_present if
                           * present */
    CMCC_CS_SystemType systemType;  /* optional; set in bit_mask
                                     * CMCC_CS_SSActionRecord_systemType_present
                                     * if present */
    CMCC_CS_UssdCodingScheme ussdCodingScheme;  /* optional; set in bit_mask
                                          * CMCC_CS_ussdCodingScheme_present if
                                          * present */
    struct CMCC_CS__seqof7 {
        struct CMCC_CS__seqof7 *next;
        CMCC_CS_UssdString value;
    } *ussdString;  /* optional; set in bit_mask CMCC_CS_ussdString_present if
                     * present */
    CMCC_CS_UssdNotifyCounter ussdNotifyCounter;  /* optional; set in bit_mask
                                         * CMCC_CS_ussdNotifyCounter_present if
                                         * present */
    CMCC_CS_UssdRequestCounter ussdRequestCounter;  /* optional; set in bit_mask
                                        * CMCC_CS_ussdRequestCounter_present if
                                        * present */
    CMCC_CS_AdditionalChgInfo additionalChgInfo;  /* optional; set in bit_mask
                          * CMCC_CS_SSActionRecord_additionalChgInfo_present if
                          * present */
    CMCC_CS_Classmark3 classmark3;  /* optional; set in bit_mask
                                     * CMCC_CS_SSActionRecord_classmark3_present
                                     * if present */
    CMCC_CS_ChargedParty chargedParty;  /* optional; set in bit_mask
                               * CMCC_CS_SSActionRecord_chargedParty_present if
                               * present */
    CMCC_CS_RNCorBSCId orgRNCorBSCId;  /* optional; set in bit_mask
                              * CMCC_CS_SSActionRecord_orgRNCorBSCId_present if
                              * present */
    CMCC_CS_MSCId   orgMSCId;  /* optional; set in bit_mask
                                * CMCC_CS_SSActionRecord_orgMSCId_present if
                                * present */
    CMCC_CS_GAI     globalAreaID;  /* optional; set in bit_mask
                               * CMCC_CS_SSActionRecord_globalAreaID_present if
                               * present */
    CMCC_CS_SubscriberCategory subscriberCategory;  /* optional; set in bit_mask
                         * CMCC_CS_SSActionRecord_subscriberCategory_present if
                         * present */
    CMCC_CS_MCCMNC  firstmccmnc;  /* optional; set in bit_mask
                                   * CMCC_CS_SSActionRecord_firstmccmnc_present
                                   * if present */
    CMCC_CS_HotBillingTag hotBillingTag;  /* optional; set in bit_mask
                              * CMCC_CS_SSActionRecord_hotBillingTag_present if
                              * present */
} CMCC_CS_SSActionRecord;

typedef struct CMCC_CS_RoutingNumber {
    unsigned short  choice;
#       define      CMCC_CS_roaming_chosen 1
#       define      CMCC_CS_forwarded_chosen 2
    union _union {
        CMCC_CS_RoamingNumber roaming;  /* to choose, set choice to
                                         * CMCC_CS_roaming_chosen */
        CMCC_CS_ForwardToNumber forwarded;  /* to choose, set choice to
                                             * CMCC_CS_forwarded_chosen */
    } u;
} CMCC_CS_RoutingNumber;

typedef CMCC_CS_Diagnostics CMCC_CS_HLRIntResult;

typedef struct CMCC_CS_HLRIntRecord {
    unsigned short  bit_mask;
#       define      CMCC_CS_HLRIntRecord_recordType_present 0x8000
#       define      CMCC_CS_HLRIntRecord_servedIMSI_present 0x4000
#       define      CMCC_CS_HLRIntRecord_servedMSISDN_present 0x2000
#       define      CMCC_CS_HLRIntRecord_recordingEntity_present 0x1000
#       define      CMCC_CS_HLRIntRecord_basicService_present 0x0800
#       define      CMCC_CS_routingNumber_present 0x0400
#       define      CMCC_CS_HLRIntRecord_interrogationTime_present 0x0200
#       define      CMCC_CS_numberOfForwarding_present 0x0100
#       define      CMCC_CS_interrogationResult_present 0x0080
#       define      CMCC_CS_HLRIntRecord_recordExtensions_present 0x0040
#       define      CMCC_CS_HLRIntRecord_orgMSCId_present 0x0020
    CMCC_CS_CallEventRecordType recordType;  /* optional; set in bit_mask
                                   * CMCC_CS_HLRIntRecord_recordType_present if
                                   * present */
    CMCC_CS_IMSI    servedIMSI;  /* optional; set in bit_mask
                                  * CMCC_CS_HLRIntRecord_servedIMSI_present if
                                  * present */
    CMCC_CS_MSISDN  servedMSISDN;  /* optional; set in bit_mask
                                    * CMCC_CS_HLRIntRecord_servedMSISDN_present
                                    * if present */
    CMCC_CS_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                              * CMCC_CS_HLRIntRecord_recordingEntity_present if
                              * present */
    CMCC_CS_BasicServiceCode basicService;  /* optional; set in bit_mask
                                 * CMCC_CS_HLRIntRecord_basicService_present if
                                 * present */
    CMCC_CS_RoutingNumber routingNumber;  /* optional; set in bit_mask
                                           * CMCC_CS_routingNumber_present if
                                           * present */
    CMCC_CS_TimeStamp interrogationTime;  /* optional; set in bit_mask
                            * CMCC_CS_HLRIntRecord_interrogationTime_present if
                            * present */
    CMCC_CS_NumberOfForwarding numberOfForwarding;  /* optional; set in bit_mask
                                        * CMCC_CS_numberOfForwarding_present if
                                        * present */
    CMCC_CS_HLRIntResult interrogationResult;  /* optional; set in bit_mask
                                       * CMCC_CS_interrogationResult_present if
                                       * present */
    struct CMCC_CS_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                             * CMCC_CS_HLRIntRecord_recordExtensions_present if
                             * present */
    CMCC_CS_MSCId   orgMSCId;  /* optional; set in bit_mask
                                * CMCC_CS_HLRIntRecord_orgMSCId_present if
                                * present */
} CMCC_CS_HLRIntRecord;

typedef int             CMCC_CS_EquipmentType;
#define                     CMCC_CS_conferenceBridge 0

typedef int             CMCC_CS_EquipmentId;

typedef struct CMCC_CS_CommonEquipRecord {
    unsigned int    bit_mask;
#       define      CMCC_CS_CommonEquipRecord_recordType_present 0x80000000
#       define      CMCC_CS_equipmentType_present 0x40000000
#       define      CMCC_CS_equipmentId_present 0x20000000
#       define      CMCC_CS_CommonEquipRecord_servedIMSI_present 0x10000000
#       define      CMCC_CS_CommonEquipRecord_servedMSISDN_present 0x08000000
#       define      CMCC_CS_CommonEquipRecord_recordingEntity_present 0x04000000
#       define      CMCC_CS_CommonEquipRecord_basicService_present 0x02000000
#       define      CMCC_CS_CommonEquipRecord_changeOfService_present 0x01000000
#       define      CMCC_CS_CommonEquipRecord_supplServicesUsed_present 0x00800000
#       define      CMCC_CS_CommonEquipRecord_seizureTime_present 0x00400000
#       define      CMCC_CS_CommonEquipRecord_releaseTime_present 0x00200000
#       define      CMCC_CS_CommonEquipRecord_callDuration_present 0x00100000
#       define      CMCC_CS_CommonEquipRecord_callReference_present 0x00080000
#       define      CMCC_CS_CommonEquipRecord_sequenceNumber_present 0x00040000
#       define      CMCC_CS_CommonEquipRecord_recordExtensions_present 0x00020000
#       define      CMCC_CS_CommonEquipRecord_systemType_present 0x00010000
#       define      CMCC_CS_CommonEquipRecord_rateIndication_present 0x00008000
#       define      CMCC_CS_CommonEquipRecord_fnur_present 0x00004000
#       define      CMCC_CS_CommonEquipRecord_partialRecordType_present 0x00002000
#       define      CMCC_CS_CommonEquipRecord_causeForTerm_present 0x00001000
#       define      CMCC_CS_CommonEquipRecord_diagnostics_present 0x00000800
#       define      CMCC_CS_CommonEquipRecord_servedIMEI_present 0x00000400
#       define      CMCC_CS_CommonEquipRecord_additionalChgInfo_present 0x00000200
#       define      CMCC_CS_CommonEquipRecord_orgRNCorBSCId_present 0x00000100
#       define      CMCC_CS_CommonEquipRecord_orgMSCId_present 0x00000080
#       define      CMCC_CS_CommonEquipRecord_subscriberCategory_present 0x00000040
#       define      CMCC_CS_CommonEquipRecord_hotBillingTag_present 0x00000020
    CMCC_CS_CallEventRecordType recordType;  /* optional; set in bit_mask
                              * CMCC_CS_CommonEquipRecord_recordType_present if
                              * present */
    CMCC_CS_EquipmentType equipmentType;  /* optional; set in bit_mask
                                           * CMCC_CS_equipmentType_present if
                                           * present */
    CMCC_CS_EquipmentId equipmentId;  /* optional; set in bit_mask
                                       * CMCC_CS_equipmentId_present if
                                       * present */
    CMCC_CS_IMSI    servedIMSI;  /* optional; set in bit_mask
                                  * CMCC_CS_CommonEquipRecord_servedIMSI_present
                                  * if present */
    CMCC_CS_MSISDN  servedMSISDN;  /* optional; set in bit_mask
                            * CMCC_CS_CommonEquipRecord_servedMSISDN_present if
                            * present */
    CMCC_CS_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                         * CMCC_CS_CommonEquipRecord_recordingEntity_present if
                         * present */
    CMCC_CS_BasicServiceCode basicService;  /* optional; set in bit_mask
                            * CMCC_CS_CommonEquipRecord_basicService_present if
                            * present */
    struct CMCC_CS__seqof2_ *changeOfService;  /* optional; set in bit_mask
                         * CMCC_CS_CommonEquipRecord_changeOfService_present if
                         * present */
    struct CMCC_CS__seqof3_ *supplServicesUsed;  /* optional; set in bit_mask
                       * CMCC_CS_CommonEquipRecord_supplServicesUsed_present if
                       * present */
    CMCC_CS_TimeStamp seizureTime;  /* optional; set in bit_mask
                             * CMCC_CS_CommonEquipRecord_seizureTime_present if
                             * present */
    CMCC_CS_TimeStamp releaseTime;  /* optional; set in bit_mask
                             * CMCC_CS_CommonEquipRecord_releaseTime_present if
                             * present */
    CMCC_CS_CallDuration callDuration;  /* optional; set in bit_mask
                            * CMCC_CS_CommonEquipRecord_callDuration_present if
                            * present */
    CMCC_CS_CallReference callReference;  /* optional; set in bit_mask
                           * CMCC_CS_CommonEquipRecord_callReference_present if
                           * present */
    CMCC_CS_SequenceNumber sequenceNumber;  /* optional; set in bit_mask
                          * CMCC_CS_CommonEquipRecord_sequenceNumber_present if
                          * present */
    struct CMCC_CS_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                        * CMCC_CS_CommonEquipRecord_recordExtensions_present if
                        * present */
    CMCC_CS_SystemType systemType;  /* optional; set in bit_mask
                              * CMCC_CS_CommonEquipRecord_systemType_present if
                              * present */
    CMCC_CS_RateIndication rateIndication;  /* optional; set in bit_mask
                          * CMCC_CS_CommonEquipRecord_rateIndication_present if
                          * present */
    CMCC_CS_Fnur    fnur;  /* optional; set in bit_mask
                            * CMCC_CS_CommonEquipRecord_fnur_present if
                            * present */
    CMCC_CS_PartialRecordType partialRecordType;  /* optional; set in bit_mask
                       * CMCC_CS_CommonEquipRecord_partialRecordType_present if
                       * present */
    CMCC_CS_CauseForTerm causeForTerm;  /* optional; set in bit_mask
                            * CMCC_CS_CommonEquipRecord_causeForTerm_present if
                            * present */
    CMCC_CS_Diagnostics diagnostics;  /* optional; set in bit_mask
                             * CMCC_CS_CommonEquipRecord_diagnostics_present if
                             * present */
    CMCC_CS_IMEI    servedIMEI;  /* optional; set in bit_mask
                                  * CMCC_CS_CommonEquipRecord_servedIMEI_present
                                  * if present */
    CMCC_CS_AdditionalChgInfo additionalChgInfo;  /* optional; set in bit_mask
                       * CMCC_CS_CommonEquipRecord_additionalChgInfo_present if
                       * present */
    CMCC_CS_RNCorBSCId orgRNCorBSCId;  /* optional; set in bit_mask
                           * CMCC_CS_CommonEquipRecord_orgRNCorBSCId_present if
                           * present */
    CMCC_CS_MSCId   orgMSCId;  /* optional; set in bit_mask
                                * CMCC_CS_CommonEquipRecord_orgMSCId_present if
                                * present */
    CMCC_CS_SubscriberCategory subscriberCategory;  /* optional; set in bit_mask
                      * CMCC_CS_CommonEquipRecord_subscriberCategory_present if
                      * present */
    CMCC_CS_HotBillingTag hotBillingTag;  /* optional; set in bit_mask
                           * CMCC_CS_CommonEquipRecord_hotBillingTag_present if
                           * present */
} CMCC_CS_CommonEquipRecord;

typedef struct CMCC_CS_TermCAMELRecord {
    unsigned char   bit_mask[6];
#       define      CMCC_CS_recordtype_present 0x80
#       define      CMCC_CS_recordtype_present_byte 0
#       define      CMCC_CS_TermCAMELRecord_servedIMSI_present 0x40
#       define      CMCC_CS_TermCAMELRecord_servedIMSI_present_byte 0
#       define      CMCC_CS_TermCAMELRecord_servedMSISDN_present 0x20
#       define      CMCC_CS_TermCAMELRecord_servedMSISDN_present_byte 0
#       define      CMCC_CS_TermCAMELRecord_recordingEntity_present 0x10
#       define      CMCC_CS_TermCAMELRecord_recordingEntity_present_byte 0
#       define      CMCC_CS_TermCAMELRecord_interrogationTime_present 0x08
#       define      CMCC_CS_TermCAMELRecord_interrogationTime_present_byte 0
#       define      CMCC_CS_destinationRoutingAddress_present 0x04
#       define      CMCC_CS_destinationRoutingAddress_present_byte 0
#       define      CMCC_CS_TermCAMELRecord_gsm_SCFAddress_present 0x02
#       define      CMCC_CS_TermCAMELRecord_gsm_SCFAddress_present_byte 0
#       define      CMCC_CS_TermCAMELRecord_serviceKey_present 0x01
#       define      CMCC_CS_TermCAMELRecord_serviceKey_present_byte 0
#       define      CMCC_CS_TermCAMELRecord_networkCallReference_present 0x80
#       define      CMCC_CS_TermCAMELRecord_networkCallReference_present_byte 1
#       define      CMCC_CS_TermCAMELRecord_mSCAddress_present 0x40
#       define      CMCC_CS_TermCAMELRecord_mSCAddress_present_byte 1
#       define      CMCC_CS_TermCAMELRecord_defaultCallHandling_present 0x20
#       define      CMCC_CS_TermCAMELRecord_defaultCallHandling_present_byte 1
#       define      CMCC_CS_TermCAMELRecord_recordExtensions_present 0x10
#       define      CMCC_CS_TermCAMELRecord_recordExtensions_present_byte 1
#       define      CMCC_CS_TermCAMELRecord_calledNumber_present 0x08
#       define      CMCC_CS_TermCAMELRecord_calledNumber_present_byte 1
#       define      CMCC_CS_TermCAMELRecord_callingNumber_present 0x04
#       define      CMCC_CS_TermCAMELRecord_callingNumber_present_byte 1
#       define      CMCC_CS_TermCAMELRecord_mscIncomingROUTE_present 0x02
#       define      CMCC_CS_TermCAMELRecord_mscIncomingROUTE_present_byte 1
#       define      CMCC_CS_TermCAMELRecord_mscOutgoingROUTE_present 0x01
#       define      CMCC_CS_TermCAMELRecord_mscOutgoingROUTE_present_byte 1
#       define      CMCC_CS_TermCAMELRecord_seizureTime_present 0x80
#       define      CMCC_CS_TermCAMELRecord_seizureTime_present_byte 2
#       define      CMCC_CS_TermCAMELRecord_answerTime_present 0x40
#       define      CMCC_CS_TermCAMELRecord_answerTime_present_byte 2
#       define      CMCC_CS_TermCAMELRecord_releaseTime_present 0x20
#       define      CMCC_CS_TermCAMELRecord_releaseTime_present_byte 2
#       define      CMCC_CS_TermCAMELRecord_callDuration_present 0x10
#       define      CMCC_CS_TermCAMELRecord_callDuration_present_byte 2
#       define      CMCC_CS_TermCAMELRecord_causeForTerm_present 0x08
#       define      CMCC_CS_TermCAMELRecord_causeForTerm_present_byte 2
#       define      CMCC_CS_TermCAMELRecord_diagnostics_present 0x04
#       define      CMCC_CS_TermCAMELRecord_diagnostics_present_byte 2
#       define      CMCC_CS_TermCAMELRecord_callReference_present 0x02
#       define      CMCC_CS_TermCAMELRecord_callReference_present_byte 2
#       define      CMCC_CS_TermCAMELRecord_sequenceNumber_present 0x01
#       define      CMCC_CS_TermCAMELRecord_sequenceNumber_present_byte 2
#       define      CMCC_CS_TermCAMELRecord_numberOfDPEncountered_present 0x80
#       define      CMCC_CS_TermCAMELRecord_numberOfDPEncountered_present_byte 3
#       define      CMCC_CS_TermCAMELRecord_levelOfCAMELService_present 0x40
#       define      CMCC_CS_TermCAMELRecord_levelOfCAMELService_present_byte 3
#       define      CMCC_CS_TermCAMELRecord_freeFormatData_present 0x20
#       define      CMCC_CS_TermCAMELRecord_freeFormatData_present_byte 3
#       define      CMCC_CS_TermCAMELRecord_cAMELCallLegInformation_present 0x10
#       define      CMCC_CS_TermCAMELRecord_cAMELCallLegInformation_present_byte 3
#       define      CMCC_CS_TermCAMELRecord_freeFormatDataAppend_present 0x08
#       define      CMCC_CS_TermCAMELRecord_freeFormatDataAppend_present_byte 3
#       define      CMCC_CS_mscServerIndication_present 0x04
#       define      CMCC_CS_mscServerIndication_present_byte 3
#       define      CMCC_CS_TermCAMELRecord_defaultCallHandling_2_present 0x02
#       define      CMCC_CS_TermCAMELRecord_defaultCallHandling_2_present_byte 3
#       define      CMCC_CS_TermCAMELRecord_gsm_SCFAddress_2_present 0x01
#       define      CMCC_CS_TermCAMELRecord_gsm_SCFAddress_2_present_byte 3
#       define      CMCC_CS_TermCAMELRecord_serviceKey_2_present 0x80
#       define      CMCC_CS_TermCAMELRecord_serviceKey_2_present_byte 4
#       define      CMCC_CS_TermCAMELRecord_freeFormatData_2_present 0x40
#       define      CMCC_CS_TermCAMELRecord_freeFormatData_2_present_byte 4
#       define      CMCC_CS_TermCAMELRecord_freeFormatDataAppend_2_present 0x20
#       define      CMCC_CS_TermCAMELRecord_freeFormatDataAppend_2_present_byte 4
#       define      CMCC_CS_TermCAMELRecord_partialRecordType_present 0x10
#       define      CMCC_CS_TermCAMELRecord_partialRecordType_present_byte 4
#       define      CMCC_CS_TermCAMELRecord_basicService_present 0x08
#       define      CMCC_CS_TermCAMELRecord_basicService_present_byte 4
#       define      CMCC_CS_TermCAMELRecord_additionalChgInfo_present 0x04
#       define      CMCC_CS_TermCAMELRecord_additionalChgInfo_present_byte 4
#       define      CMCC_CS_TermCAMELRecord_chargedParty_present 0x02
#       define      CMCC_CS_TermCAMELRecord_chargedParty_present_byte 4
#       define      CMCC_CS_TermCAMELRecord_originalCalledNumber_present 0x01
#       define      CMCC_CS_TermCAMELRecord_originalCalledNumber_present_byte 4
#       define      CMCC_CS_TermCAMELRecord_orgMSCId_present 0x80
#       define      CMCC_CS_TermCAMELRecord_orgMSCId_present_byte 5
#       define      CMCC_CS_TermCAMELRecord_subscriberCategory_present 0x40
#       define      CMCC_CS_TermCAMELRecord_subscriberCategory_present_byte 5
#       define      CMCC_CS_TermCAMELRecord_hotBillingTag_present 0x20
#       define      CMCC_CS_TermCAMELRecord_hotBillingTag_present_byte 5
    CMCC_CS_CallEventRecordType recordtype;  /* optional; set in bit_mask
                                              * CMCC_CS_recordtype_present if
                                              * present */
    CMCC_CS_IMSI    servedIMSI;  /* optional; set in bit_mask
                                  * CMCC_CS_TermCAMELRecord_servedIMSI_present
                                  * if present */
    CMCC_CS_MSISDN  servedMSISDN;  /* optional; set in bit_mask
                              * CMCC_CS_TermCAMELRecord_servedMSISDN_present if
                              * present */
    CMCC_CS_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                           * CMCC_CS_TermCAMELRecord_recordingEntity_present if
                           * present */
    CMCC_CS_TimeStamp interrogationTime;  /* optional; set in bit_mask
                         * CMCC_CS_TermCAMELRecord_interrogationTime_present if
                         * present */
    CMCC_CS_DestinationRoutingAddress destinationRoutingAddress;  /* optional;
                                   * set in bit_mask
                                   * CMCC_CS_destinationRoutingAddress_present
                                   * if present */
    CMCC_CS_Gsm_SCFAddress gsm_SCFAddress;  /* optional; set in bit_mask
                            * CMCC_CS_TermCAMELRecord_gsm_SCFAddress_present if
                            * present */
    CMCC_CS_ServiceKey serviceKey;  /* optional; set in bit_mask
                                * CMCC_CS_TermCAMELRecord_serviceKey_present if
                                * present */
    CMCC_CS_NetworkCallReference networkCallReference;  /* optional; set in
                                   * bit_mask
                      * CMCC_CS_TermCAMELRecord_networkCallReference_present if
                      * present */
    CMCC_CS_MSCAddress mSCAddress;  /* optional; set in bit_mask
                                * CMCC_CS_TermCAMELRecord_mSCAddress_present if
                                * present */
    CMCC_CS_DefaultCallHandling defaultCallHandling;  /* optional; set in
                                   * bit_mask
                       * CMCC_CS_TermCAMELRecord_defaultCallHandling_present if
                       * present */
    struct CMCC_CS_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                          * CMCC_CS_TermCAMELRecord_recordExtensions_present if
                          * present */
    CMCC_CS_CalledNumber calledNumber;  /* optional; set in bit_mask
                              * CMCC_CS_TermCAMELRecord_calledNumber_present if
                              * present */
    CMCC_CS_CallingNumber callingNumber;  /* optional; set in bit_mask
                             * CMCC_CS_TermCAMELRecord_callingNumber_present if
                             * present */
    CMCC_CS_ROUTE   mscIncomingROUTE;  /* optional; set in bit_mask
                          * CMCC_CS_TermCAMELRecord_mscIncomingROUTE_present if
                          * present */
    CMCC_CS_ROUTE   mscOutgoingROUTE;  /* optional; set in bit_mask
                          * CMCC_CS_TermCAMELRecord_mscOutgoingROUTE_present if
                          * present */
    CMCC_CS_TimeStamp seizureTime;  /* optional; set in bit_mask
                               * CMCC_CS_TermCAMELRecord_seizureTime_present if
                               * present */
    CMCC_CS_TimeStamp answerTime;  /* optional; set in bit_mask
                                    * CMCC_CS_TermCAMELRecord_answerTime_present
                                    * if present */
    CMCC_CS_TimeStamp releaseTime;  /* optional; set in bit_mask
                               * CMCC_CS_TermCAMELRecord_releaseTime_present if
                               * present */
    CMCC_CS_CallDuration callDuration;  /* optional; set in bit_mask
                              * CMCC_CS_TermCAMELRecord_callDuration_present if
                              * present */
    CMCC_CS_CauseForTerm causeForTerm;  /* optional; set in bit_mask
                              * CMCC_CS_TermCAMELRecord_causeForTerm_present if
                              * present */
    CMCC_CS_Diagnostics diagnostics;  /* optional; set in bit_mask
                               * CMCC_CS_TermCAMELRecord_diagnostics_present if
                               * present */
    CMCC_CS_CallReference callReference;  /* optional; set in bit_mask
                             * CMCC_CS_TermCAMELRecord_callReference_present if
                             * present */
    CMCC_CS_SequenceNumber sequenceNumber;  /* optional; set in bit_mask
                            * CMCC_CS_TermCAMELRecord_sequenceNumber_present if
                            * present */
    int             numberOfDPEncountered;  /* optional; set in bit_mask
                     * CMCC_CS_TermCAMELRecord_numberOfDPEncountered_present if
                     * present */
    CMCC_CS_LevelOfCAMELService levelOfCAMELService;  /* optional; set in
                                   * bit_mask
                       * CMCC_CS_TermCAMELRecord_levelOfCAMELService_present if
                       * present */
    CMCC_CS_FreeFormatData freeFormatData;  /* optional; set in bit_mask
                            * CMCC_CS_TermCAMELRecord_freeFormatData_present if
                            * present */
    struct CMCC_CS__seqof5_ *cAMELCallLegInformation;  /* optional; set in
                                   * bit_mask
                   * CMCC_CS_TermCAMELRecord_cAMELCallLegInformation_present if
                   * present */
    ossBoolean      freeFormatDataAppend;  /* optional; set in bit_mask
                      * CMCC_CS_TermCAMELRecord_freeFormatDataAppend_present if
                      * present */
    ossBoolean      mscServerIndication;  /* optional; set in bit_mask
                                           * CMCC_CS_mscServerIndication_present
                                           * if present */
    CMCC_CS_DefaultCallHandling defaultCallHandling_2;  /* optional; set in
                                   * bit_mask
                     * CMCC_CS_TermCAMELRecord_defaultCallHandling_2_present if
                     * present */
    CMCC_CS_Gsm_SCFAddress gsm_SCFAddress_2;  /* optional; set in bit_mask
                          * CMCC_CS_TermCAMELRecord_gsm_SCFAddress_2_present if
                          * present */
    CMCC_CS_ServiceKey serviceKey_2;  /* optional; set in bit_mask
                              * CMCC_CS_TermCAMELRecord_serviceKey_2_present if
                              * present */
    CMCC_CS_FreeFormatData freeFormatData_2;  /* optional; set in bit_mask
                          * CMCC_CS_TermCAMELRecord_freeFormatData_2_present if
                          * present */
    ossBoolean      freeFormatDataAppend_2;  /* optional; set in bit_mask
                    * CMCC_CS_TermCAMELRecord_freeFormatDataAppend_2_present if
                    * present */
    CMCC_CS_PartialRecordType partialRecordType;  /* optional; set in bit_mask
                         * CMCC_CS_TermCAMELRecord_partialRecordType_present if
                         * present */
    CMCC_CS_BasicServiceCode basicService;  /* optional; set in bit_mask
                              * CMCC_CS_TermCAMELRecord_basicService_present if
                              * present */
    CMCC_CS_AdditionalChgInfo additionalChgInfo;  /* optional; set in bit_mask
                         * CMCC_CS_TermCAMELRecord_additionalChgInfo_present if
                         * present */
    CMCC_CS_ChargedParty chargedParty;  /* optional; set in bit_mask
                              * CMCC_CS_TermCAMELRecord_chargedParty_present if
                              * present */
    CMCC_CS_OriginalCalledNumber originalCalledNumber;  /* optional; set in
                                   * bit_mask
                      * CMCC_CS_TermCAMELRecord_originalCalledNumber_present if
                      * present */
    CMCC_CS_MSCId   orgMSCId;  /* optional; set in bit_mask
                                * CMCC_CS_TermCAMELRecord_orgMSCId_present if
                                * present */
    CMCC_CS_SubscriberCategory subscriberCategory;  /* optional; set in bit_mask
                        * CMCC_CS_TermCAMELRecord_subscriberCategory_present if
                        * present */
    CMCC_CS_HotBillingTag hotBillingTag;  /* optional; set in bit_mask
                             * CMCC_CS_TermCAMELRecord_hotBillingTag_present if
                             * present */
} CMCC_CS_TermCAMELRecord;

typedef enum CMCC_CS_LCSClientType {
    CMCC_CS_emergencyServices = 0,
    CMCC_CS_valueAddedServices = 1,
    CMCC_CS_plmnOperatorServices = 2,
    CMCC_CS_lawfulInterceptServices = 3
} CMCC_CS_LCSClientType;

typedef struct CMCC_CS_LCSClientExternalID {
    unsigned char   bit_mask;
#       define      CMCC_CS_externalAddress_present 0x80
    CMCC_CS_AddressString externalAddress;  /* optional; set in bit_mask
                                             * CMCC_CS_externalAddress_present
                                             * if present */
} CMCC_CS_LCSClientExternalID;

typedef enum CMCC_CS_LCSClientInternalID {
    CMCC_CS_broadcastService = 0,
    CMCC_CS_o_andM_HPLMN = 1,
    CMCC_CS_o_andM_VPLMN = 2,
    CMCC_CS_anonymousLocation = 3,
    CMCC_CS_targetMSsubscribedService = 4
} CMCC_CS_LCSClientInternalID;

typedef struct CMCC_CS_LCSClientIdentity {
    unsigned char   bit_mask;
#       define      CMCC_CS_lcsClientExternalID_present 0x80
#       define      CMCC_CS_lcsClientDialedByMS_present 0x40
#       define      CMCC_CS_lcsClientInternalID_present 0x20
    CMCC_CS_LCSClientExternalID lcsClientExternalID;  /* optional; set in
                                   * bit_mask
                                   * CMCC_CS_lcsClientExternalID_present if
                                   * present */
    CMCC_CS_AddressString lcsClientDialedByMS;  /* optional; set in bit_mask
                                       * CMCC_CS_lcsClientDialedByMS_present if
                                       * present */
    CMCC_CS_LCSClientInternalID lcsClientInternalID;  /* optional; set in
                                   * bit_mask
                                   * CMCC_CS_lcsClientInternalID_present if
                                   * present */
} CMCC_CS_LCSClientIdentity;

typedef enum CMCC_CS_LocationEstimateType {
    CMCC_CS_currentLocation = 0,
    CMCC_CS_currentOrLastKnownLocation = 1,
    CMCC_CS_initialLocation = 2,
    CMCC_CS_activateDeferredLocation = 3,
    CMCC_CS_cancelDeferredLocation = 4
} CMCC_CS_LocationEstimateType;

typedef unsigned short  CMCC_CS_DeferredLocationEventType;
#define                     CMCC_CS_msAvailable 0x8000

typedef struct CMCC_CS_LocationType {
    unsigned char   bit_mask;
#       define      CMCC_CS_deferredLocationEventType_present 0x80
    CMCC_CS_LocationEstimateType locationEstimateType;
    CMCC_CS_DeferredLocationEventType deferredLocationEventType;  /* optional;
                                   * set in bit_mask
                                   * CMCC_CS_deferredLocationEventType_present
                                   * if present */
} CMCC_CS_LocationType;

typedef struct CMCC_CS_Horizontal_Accuracy {
    unsigned short  length;
    unsigned char   value[1];
} CMCC_CS_Horizontal_Accuracy;

typedef struct CMCC_CS_Vertical_Accuracy {
    unsigned short  length;
    unsigned char   value[1];
} CMCC_CS_Vertical_Accuracy;

typedef enum CMCC_CS_ResponseTimeCategory {
    CMCC_CS_lowdelay = 0,
    CMCC_CS_delaytolerant = 1
} CMCC_CS_ResponseTimeCategory;

typedef struct CMCC_CS_ResponseTime {
    CMCC_CS_ResponseTimeCategory responseTimeCategory;
} CMCC_CS_ResponseTime;

typedef struct CMCC_CS_LCSQoSInfo {
    unsigned char   bit_mask;
#       define      CMCC_CS_horizontal_accuracy_present 0x80
#       define      CMCC_CS_verticalCoordinateRequest_present 0x40
#       define      CMCC_CS_vertical_accuracy_present 0x20
#       define      CMCC_CS_responseTime_present 0x10
    CMCC_CS_Horizontal_Accuracy horizontal_accuracy;  /* optional; set in
                                   * bit_mask
                                   * CMCC_CS_horizontal_accuracy_present if
                                   * present */
    Nulltype        verticalCoordinateRequest;  /* optional; set in bit_mask
                                 * CMCC_CS_verticalCoordinateRequest_present if
                                 * present */
    CMCC_CS_Vertical_Accuracy vertical_accuracy;  /* optional; set in bit_mask
                                         * CMCC_CS_vertical_accuracy_present if
                                         * present */
    CMCC_CS_ResponseTime responseTime;  /* optional; set in bit_mask
                                         * CMCC_CS_responseTime_present if
                                         * present */
} CMCC_CS_LCSQoSInfo;

typedef struct CMCC_CS_LCS_Priority {
    unsigned short  length;
    unsigned char   value[1];
} CMCC_CS_LCS_Priority;

typedef enum CMCC_CS_NotificationToMSUser {
    CMCC_CS_notifyLocationAllowed = 0,
    CMCC_CS_notifyAndVerify_LocationAllowedIfNoResponse = 1,
    CMCC_CS_notifyAndVerify_LocationNotAllowedIfNoResponse = 2,
    CMCC_CS_locationNotAllowed = 3
} CMCC_CS_NotificationToMSUser;

typedef struct CMCC_CS_Ext_GeographicalInformation {
    unsigned short  length;
    unsigned char   value[20];
} CMCC_CS_Ext_GeographicalInformation;

typedef struct CMCC_CS_PositioningData {
    unsigned short  length;
    unsigned char   value[33];
} CMCC_CS_PositioningData;

typedef struct CMCC_CS_LCSCause {
    unsigned short  length;
    unsigned char   value[1];
} CMCC_CS_LCSCause;

typedef struct CMCC_CS_MTLCSRecord {
    unsigned int    bit_mask;
#       define      CMCC_CS_MTLCSRecord_recordType_present 0x80000000
#       define      CMCC_CS_MTLCSRecord_recordingEntity_present 0x40000000
#       define      CMCC_CS_MTLCSRecord_lcsClientType_present 0x20000000
#       define      CMCC_CS_MTLCSRecord_lcsClientIdentity_present 0x10000000
#       define      CMCC_CS_MTLCSRecord_servedIMSI_present 0x08000000
#       define      CMCC_CS_MTLCSRecord_servedMSISDN_present 0x04000000
#       define      CMCC_CS_locationType_present 0x02000000
#       define      CMCC_CS_MTLCSRecord_lcsQos_present 0x01000000
#       define      CMCC_CS_MTLCSRecord_lcsPriority_present 0x00800000
#       define      CMCC_CS_MTLCSRecord_mlc_Number_present 0x00400000
#       define      CMCC_CS_MTLCSRecord_eventTimeStamp_present 0x00200000
#       define      CMCC_CS_MTLCSRecord_measureDuration_present 0x00100000
#       define      CMCC_CS_notificationToMSUser_present 0x00080000
#       define      CMCC_CS_privacyOverride_present 0x00040000
#       define      CMCC_CS_MTLCSRecord_location_present 0x00020000
#       define      CMCC_CS_MTLCSRecord_locationEstimate_present 0x00010000
#       define      CMCC_CS_MTLCSRecord_positioningData_present 0x00008000
#       define      CMCC_CS_MTLCSRecord_lcsCause_present 0x00004000
#       define      CMCC_CS_MTLCSRecord_diagnostics_present 0x00002000
#       define      CMCC_CS_MTLCSRecord_systemType_present 0x00001000
#       define      CMCC_CS_MTLCSRecord_recordExtensions_present 0x00000800
#       define      CMCC_CS_MTLCSRecord_causeForTerm_present 0x00000400
#       define      CMCC_CS_MTLCSRecord_lcsReferenceNumber_present 0x00000200
#       define      CMCC_CS_MTLCSRecord_servedIMEI_present 0x00000100
#       define      CMCC_CS_MTLCSRecord_additionalChgInfo_present 0x00000080
#       define      CMCC_CS_MTLCSRecord_chargedParty_present 0x00000040
#       define      CMCC_CS_MTLCSRecord_orgRNCorBSCId_present 0x00000020
#       define      CMCC_CS_MTLCSRecord_orgMSCId_present 0x00000010
#       define      CMCC_CS_MTLCSRecord_globalAreaID_present 0x00000008
#       define      CMCC_CS_MTLCSRecord_subscriberCategory_present 0x00000004
#       define      CMCC_CS_MTLCSRecord_firstmccmnc_present 0x00000002
#       define      CMCC_CS_MTLCSRecord_hotBillingTag_present 0x00000001
    CMCC_CS_CallEventRecordType recordType;  /* optional; set in bit_mask
                                    * CMCC_CS_MTLCSRecord_recordType_present if
                                    * present */
    CMCC_CS_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                               * CMCC_CS_MTLCSRecord_recordingEntity_present if
                               * present */
    CMCC_CS_LCSClientType lcsClientType;  /* optional; set in bit_mask
                                 * CMCC_CS_MTLCSRecord_lcsClientType_present if
                                 * present */
    CMCC_CS_LCSClientIdentity lcsClientIdentity;  /* optional; set in bit_mask
                             * CMCC_CS_MTLCSRecord_lcsClientIdentity_present if
                             * present */
    CMCC_CS_IMSI    servedIMSI;  /* optional; set in bit_mask
                                  * CMCC_CS_MTLCSRecord_servedIMSI_present if
                                  * present */
    CMCC_CS_MSISDN  servedMSISDN;  /* optional; set in bit_mask
                                    * CMCC_CS_MTLCSRecord_servedMSISDN_present
                                    * if present */
    CMCC_CS_LocationType locationType;  /* optional; set in bit_mask
                                         * CMCC_CS_locationType_present if
                                         * present */
    CMCC_CS_LCSQoSInfo lcsQos;  /* optional; set in bit_mask
                                 * CMCC_CS_MTLCSRecord_lcsQos_present if
                                 * present */
    CMCC_CS_LCS_Priority lcsPriority;  /* optional; set in bit_mask
                                   * CMCC_CS_MTLCSRecord_lcsPriority_present if
                                   * present */
    CMCC_CS_ISDN_AddressString mlc_Number;  /* optional; set in bit_mask
                                    * CMCC_CS_MTLCSRecord_mlc_Number_present if
                                    * present */
    CMCC_CS_TimeStamp eventTimeStamp;  /* optional; set in bit_mask
                                * CMCC_CS_MTLCSRecord_eventTimeStamp_present if
                                * present */
    CMCC_CS_CallDuration measureDuration;  /* optional; set in bit_mask
                               * CMCC_CS_MTLCSRecord_measureDuration_present if
                               * present */
    CMCC_CS_NotificationToMSUser notificationToMSUser;  /* optional; set in
                                   * bit_mask
                                   * CMCC_CS_notificationToMSUser_present if
                                   * present */
    Nulltype        privacyOverride;  /* optional; set in bit_mask
                                       * CMCC_CS_privacyOverride_present if
                                       * present */
    CMCC_CS_LocationAreaAndCell location;  /* optional; set in bit_mask
                                      * CMCC_CS_MTLCSRecord_location_present if
                                      * present */
    CMCC_CS_Ext_GeographicalInformation locationEstimate;  /* optional; set in
                                   * bit_mask
                              * CMCC_CS_MTLCSRecord_locationEstimate_present if
                              * present */
    CMCC_CS_PositioningData positioningData;  /* optional; set in bit_mask
                               * CMCC_CS_MTLCSRecord_positioningData_present if
                               * present */
    CMCC_CS_LCSCause lcsCause;  /* optional; set in bit_mask
                                 * CMCC_CS_MTLCSRecord_lcsCause_present if
                                 * present */
    CMCC_CS_Diagnostics diagnostics;  /* optional; set in bit_mask
                                       * CMCC_CS_MTLCSRecord_diagnostics_present
                                       * if present */
    CMCC_CS_SystemType systemType;  /* optional; set in bit_mask
                                     * CMCC_CS_MTLCSRecord_systemType_present if
                                     * present */
    struct CMCC_CS_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                              * CMCC_CS_MTLCSRecord_recordExtensions_present if
                              * present */
    CMCC_CS_CauseForTerm causeForTerm;  /* optional; set in bit_mask
                                  * CMCC_CS_MTLCSRecord_causeForTerm_present if
                                  * present */
    CMCC_CS_CallReferenceNumber lcsReferenceNumber;  /* optional; set in
                                   * bit_mask
                            * CMCC_CS_MTLCSRecord_lcsReferenceNumber_present if
                            * present */
    CMCC_CS_IMEI    servedIMEI;  /* optional; set in bit_mask
                                  * CMCC_CS_MTLCSRecord_servedIMEI_present if
                                  * present */
    CMCC_CS_AdditionalChgInfo additionalChgInfo;  /* optional; set in bit_mask
                             * CMCC_CS_MTLCSRecord_additionalChgInfo_present if
                             * present */
    CMCC_CS_ChargedParty chargedParty;  /* optional; set in bit_mask
                                  * CMCC_CS_MTLCSRecord_chargedParty_present if
                                  * present */
    CMCC_CS_RNCorBSCId orgRNCorBSCId;  /* optional; set in bit_mask
                                 * CMCC_CS_MTLCSRecord_orgRNCorBSCId_present if
                                 * present */
    CMCC_CS_MSCId   orgMSCId;  /* optional; set in bit_mask
                                * CMCC_CS_MTLCSRecord_orgMSCId_present if
                                * present */
    CMCC_CS_GAI     globalAreaID;  /* optional; set in bit_mask
                                    * CMCC_CS_MTLCSRecord_globalAreaID_present
                                    * if present */
    CMCC_CS_SubscriberCategory subscriberCategory;  /* optional; set in bit_mask
                            * CMCC_CS_MTLCSRecord_subscriberCategory_present if
                            * present */
    CMCC_CS_MCCMNC  firstmccmnc;  /* optional; set in bit_mask
                                   * CMCC_CS_MTLCSRecord_firstmccmnc_present if
                                   * present */
    CMCC_CS_HotBillingTag hotBillingTag;  /* optional; set in bit_mask
                                 * CMCC_CS_MTLCSRecord_hotBillingTag_present if
                                 * present */
} CMCC_CS_MTLCSRecord;

typedef int             CMCC_CS_MOLR_Type;

typedef struct CMCC_CS_MOLCSRecord {
    unsigned int    bit_mask;
#       define      CMCC_CS_MOLCSRecord_recordType_present 0x80000000
#       define      CMCC_CS_MOLCSRecord_recordingEntity_present 0x40000000
#       define      CMCC_CS_MOLCSRecord_lcsClientType_present 0x20000000
#       define      CMCC_CS_MOLCSRecord_lcsClientIdentity_present 0x10000000
#       define      CMCC_CS_MOLCSRecord_servedIMSI_present 0x08000000
#       define      CMCC_CS_MOLCSRecord_servedMSISDN_present 0x04000000
#       define      CMCC_CS_molr_Type_present 0x02000000
#       define      CMCC_CS_MOLCSRecord_lcsQos_present 0x01000000
#       define      CMCC_CS_MOLCSRecord_lcsPriority_present 0x00800000
#       define      CMCC_CS_MOLCSRecord_mlc_Number_present 0x00400000
#       define      CMCC_CS_MOLCSRecord_eventTimeStamp_present 0x00200000
#       define      CMCC_CS_MOLCSRecord_measureDuration_present 0x00100000
#       define      CMCC_CS_MOLCSRecord_location_present 0x00080000
#       define      CMCC_CS_MOLCSRecord_locationEstimate_present 0x00040000
#       define      CMCC_CS_MOLCSRecord_positioningData_present 0x00020000
#       define      CMCC_CS_MOLCSRecord_lcsCause_present 0x00010000
#       define      CMCC_CS_MOLCSRecord_diagnostics_present 0x00008000
#       define      CMCC_CS_MOLCSRecord_systemType_present 0x00004000
#       define      CMCC_CS_MOLCSRecord_recordExtensions_present 0x00002000
#       define      CMCC_CS_MOLCSRecord_causeForTerm_present 0x00001000
#       define      CMCC_CS_MOLCSRecord_lcsReferenceNumber_present 0x00000800
#       define      CMCC_CS_MOLCSRecord_servedIMEI_present 0x00000400
#       define      CMCC_CS_MOLCSRecord_additionalChgInfo_present 0x00000200
#       define      CMCC_CS_MOLCSRecord_chargedParty_present 0x00000100
#       define      CMCC_CS_MOLCSRecord_orgRNCorBSCId_present 0x00000080
#       define      CMCC_CS_MOLCSRecord_orgMSCId_present 0x00000040
#       define      CMCC_CS_MOLCSRecord_globalAreaID_present 0x00000020
#       define      CMCC_CS_MOLCSRecord_subscriberCategory_present 0x00000010
#       define      CMCC_CS_MOLCSRecord_firstmccmnc_present 0x00000008
#       define      CMCC_CS_MOLCSRecord_hotBillingTag_present 0x00000004
    CMCC_CS_CallEventRecordType recordType;  /* optional; set in bit_mask
                                    * CMCC_CS_MOLCSRecord_recordType_present if
                                    * present */
    CMCC_CS_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                               * CMCC_CS_MOLCSRecord_recordingEntity_present if
                               * present */
    CMCC_CS_LCSClientType lcsClientType;  /* optional; set in bit_mask
                                 * CMCC_CS_MOLCSRecord_lcsClientType_present if
                                 * present */
    CMCC_CS_LCSClientIdentity lcsClientIdentity;  /* optional; set in bit_mask
                             * CMCC_CS_MOLCSRecord_lcsClientIdentity_present if
                             * present */
    CMCC_CS_IMSI    servedIMSI;  /* optional; set in bit_mask
                                  * CMCC_CS_MOLCSRecord_servedIMSI_present if
                                  * present */
    CMCC_CS_MSISDN  servedMSISDN;  /* optional; set in bit_mask
                                    * CMCC_CS_MOLCSRecord_servedMSISDN_present
                                    * if present */
    CMCC_CS_MOLR_Type molr_Type;  /* optional; set in bit_mask
                                   * CMCC_CS_molr_Type_present if present */
    CMCC_CS_LCSQoSInfo lcsQos;  /* optional; set in bit_mask
                                 * CMCC_CS_MOLCSRecord_lcsQos_present if
                                 * present */
    CMCC_CS_LCS_Priority lcsPriority;  /* optional; set in bit_mask
                                   * CMCC_CS_MOLCSRecord_lcsPriority_present if
                                   * present */
    CMCC_CS_ISDN_AddressString mlc_Number;  /* optional; set in bit_mask
                                    * CMCC_CS_MOLCSRecord_mlc_Number_present if
                                    * present */
    CMCC_CS_TimeStamp eventTimeStamp;  /* optional; set in bit_mask
                                * CMCC_CS_MOLCSRecord_eventTimeStamp_present if
                                * present */
    CMCC_CS_CallDuration measureDuration;  /* optional; set in bit_mask
                               * CMCC_CS_MOLCSRecord_measureDuration_present if
                               * present */
    CMCC_CS_LocationAreaAndCell location;  /* optional; set in bit_mask
                                      * CMCC_CS_MOLCSRecord_location_present if
                                      * present */
    CMCC_CS_Ext_GeographicalInformation locationEstimate;  /* optional; set in
                                   * bit_mask
                              * CMCC_CS_MOLCSRecord_locationEstimate_present if
                              * present */
    CMCC_CS_PositioningData positioningData;  /* optional; set in bit_mask
                               * CMCC_CS_MOLCSRecord_positioningData_present if
                               * present */
    CMCC_CS_LCSCause lcsCause;  /* optional; set in bit_mask
                                 * CMCC_CS_MOLCSRecord_lcsCause_present if
                                 * present */
    CMCC_CS_Diagnostics diagnostics;  /* optional; set in bit_mask
                                       * CMCC_CS_MOLCSRecord_diagnostics_present
                                       * if present */
    CMCC_CS_SystemType systemType;  /* optional; set in bit_mask
                                     * CMCC_CS_MOLCSRecord_systemType_present if
                                     * present */
    struct CMCC_CS_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                              * CMCC_CS_MOLCSRecord_recordExtensions_present if
                              * present */
    CMCC_CS_CauseForTerm causeForTerm;  /* optional; set in bit_mask
                                  * CMCC_CS_MOLCSRecord_causeForTerm_present if
                                  * present */
    CMCC_CS_CallReferenceNumber lcsReferenceNumber;  /* optional; set in
                                   * bit_mask
                            * CMCC_CS_MOLCSRecord_lcsReferenceNumber_present if
                            * present */
    CMCC_CS_IMEI    servedIMEI;  /* optional; set in bit_mask
                                  * CMCC_CS_MOLCSRecord_servedIMEI_present if
                                  * present */
    CMCC_CS_AdditionalChgInfo additionalChgInfo;  /* optional; set in bit_mask
                             * CMCC_CS_MOLCSRecord_additionalChgInfo_present if
                             * present */
    CMCC_CS_ChargedParty chargedParty;  /* optional; set in bit_mask
                                  * CMCC_CS_MOLCSRecord_chargedParty_present if
                                  * present */
    CMCC_CS_RNCorBSCId orgRNCorBSCId;  /* optional; set in bit_mask
                                 * CMCC_CS_MOLCSRecord_orgRNCorBSCId_present if
                                 * present */
    CMCC_CS_MSCId   orgMSCId;  /* optional; set in bit_mask
                                * CMCC_CS_MOLCSRecord_orgMSCId_present if
                                * present */
    CMCC_CS_GAI     globalAreaID;  /* optional; set in bit_mask
                                    * CMCC_CS_MOLCSRecord_globalAreaID_present
                                    * if present */
    CMCC_CS_SubscriberCategory subscriberCategory;  /* optional; set in bit_mask
                            * CMCC_CS_MOLCSRecord_subscriberCategory_present if
                            * present */
    CMCC_CS_MCCMNC  firstmccmnc;  /* optional; set in bit_mask
                                   * CMCC_CS_MOLCSRecord_firstmccmnc_present if
                                   * present */
    CMCC_CS_HotBillingTag hotBillingTag;  /* optional; set in bit_mask
                                 * CMCC_CS_MOLCSRecord_hotBillingTag_present if
                                 * present */
} CMCC_CS_MOLCSRecord;

typedef struct CMCC_CS_NILCSRecord {
    unsigned int    bit_mask;
#       define      CMCC_CS_NILCSRecord_recordType_present 0x80000000
#       define      CMCC_CS_NILCSRecord_recordingEntity_present 0x40000000
#       define      CMCC_CS_NILCSRecord_lcsClientType_present 0x20000000
#       define      CMCC_CS_NILCSRecord_lcsClientIdentity_present 0x10000000
#       define      CMCC_CS_NILCSRecord_servedIMSI_present 0x08000000
#       define      CMCC_CS_NILCSRecord_servedMSISDN_present 0x04000000
#       define      CMCC_CS_NILCSRecord_servedIMEI_present 0x02000000
#       define      CMCC_CS_emsDigits_present 0x01000000
#       define      CMCC_CS_emsKey_present 0x00800000
#       define      CMCC_CS_NILCSRecord_lcsQos_present 0x00400000
#       define      CMCC_CS_NILCSRecord_lcsPriority_present 0x00200000
#       define      CMCC_CS_NILCSRecord_mlc_Number_present 0x00100000
#       define      CMCC_CS_NILCSRecord_eventTimeStamp_present 0x00080000
#       define      CMCC_CS_NILCSRecord_measureDuration_present 0x00040000
#       define      CMCC_CS_NILCSRecord_location_present 0x00020000
#       define      CMCC_CS_NILCSRecord_locationEstimate_present 0x00010000
#       define      CMCC_CS_NILCSRecord_positioningData_present 0x00008000
#       define      CMCC_CS_NILCSRecord_lcsCause_present 0x00004000
#       define      CMCC_CS_NILCSRecord_diagnostics_present 0x00002000
#       define      CMCC_CS_NILCSRecord_systemType_present 0x00001000
#       define      CMCC_CS_NILCSRecord_recordExtensions_present 0x00000800
#       define      CMCC_CS_NILCSRecord_causeForTerm_present 0x00000400
#       define      CMCC_CS_NILCSRecord_lcsReferenceNumber_present 0x00000200
#       define      CMCC_CS_NILCSRecord_additionalChgInfo_present 0x00000100
#       define      CMCC_CS_NILCSRecord_chargedParty_present 0x00000080
#       define      CMCC_CS_NILCSRecord_orgRNCorBSCId_present 0x00000040
#       define      CMCC_CS_NILCSRecord_orgMSCId_present 0x00000020
#       define      CMCC_CS_NILCSRecord_globalAreaID_present 0x00000010
#       define      CMCC_CS_NILCSRecord_subscriberCategory_present 0x00000008
#       define      CMCC_CS_NILCSRecord_firstmccmnc_present 0x00000004
#       define      CMCC_CS_NILCSRecord_hotBillingTag_present 0x00000002
    CMCC_CS_CallEventRecordType recordType;  /* optional; set in bit_mask
                                    * CMCC_CS_NILCSRecord_recordType_present if
                                    * present */
    CMCC_CS_RecordingEntity recordingEntity;  /* optional; set in bit_mask
                               * CMCC_CS_NILCSRecord_recordingEntity_present if
                               * present */
    CMCC_CS_LCSClientType lcsClientType;  /* optional; set in bit_mask
                                 * CMCC_CS_NILCSRecord_lcsClientType_present if
                                 * present */
    CMCC_CS_LCSClientIdentity lcsClientIdentity;  /* optional; set in bit_mask
                             * CMCC_CS_NILCSRecord_lcsClientIdentity_present if
                             * present */
    CMCC_CS_IMSI    servedIMSI;  /* optional; set in bit_mask
                                  * CMCC_CS_NILCSRecord_servedIMSI_present if
                                  * present */
    CMCC_CS_MSISDN  servedMSISDN;  /* optional; set in bit_mask
                                    * CMCC_CS_NILCSRecord_servedMSISDN_present
                                    * if present */
    CMCC_CS_IMEI    servedIMEI;  /* optional; set in bit_mask
                                  * CMCC_CS_NILCSRecord_servedIMEI_present if
                                  * present */
    CMCC_CS_ISDN_AddressString emsDigits;  /* optional; set in bit_mask
                                            * CMCC_CS_emsDigits_present if
                                            * present */
    CMCC_CS_ISDN_AddressString emsKey;  /* optional; set in bit_mask
                                         * CMCC_CS_emsKey_present if present */
    CMCC_CS_LCSQoSInfo lcsQos;  /* optional; set in bit_mask
                                 * CMCC_CS_NILCSRecord_lcsQos_present if
                                 * present */
    CMCC_CS_LCS_Priority lcsPriority;  /* optional; set in bit_mask
                                   * CMCC_CS_NILCSRecord_lcsPriority_present if
                                   * present */
    CMCC_CS_ISDN_AddressString mlc_Number;  /* optional; set in bit_mask
                                    * CMCC_CS_NILCSRecord_mlc_Number_present if
                                    * present */
    CMCC_CS_TimeStamp eventTimeStamp;  /* optional; set in bit_mask
                                * CMCC_CS_NILCSRecord_eventTimeStamp_present if
                                * present */
    CMCC_CS_CallDuration measureDuration;  /* optional; set in bit_mask
                               * CMCC_CS_NILCSRecord_measureDuration_present if
                               * present */
    CMCC_CS_LocationAreaAndCell location;  /* optional; set in bit_mask
                                      * CMCC_CS_NILCSRecord_location_present if
                                      * present */
    CMCC_CS_Ext_GeographicalInformation locationEstimate;  /* optional; set in
                                   * bit_mask
                              * CMCC_CS_NILCSRecord_locationEstimate_present if
                              * present */
    CMCC_CS_PositioningData positioningData;  /* optional; set in bit_mask
                               * CMCC_CS_NILCSRecord_positioningData_present if
                               * present */
    CMCC_CS_LCSCause lcsCause;  /* optional; set in bit_mask
                                 * CMCC_CS_NILCSRecord_lcsCause_present if
                                 * present */
    CMCC_CS_Diagnostics diagnostics;  /* optional; set in bit_mask
                                       * CMCC_CS_NILCSRecord_diagnostics_present
                                       * if present */
    CMCC_CS_SystemType systemType;  /* optional; set in bit_mask
                                     * CMCC_CS_NILCSRecord_systemType_present if
                                     * present */
    struct CMCC_CS_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                              * CMCC_CS_NILCSRecord_recordExtensions_present if
                              * present */
    CMCC_CS_CauseForTerm causeForTerm;  /* optional; set in bit_mask
                                  * CMCC_CS_NILCSRecord_causeForTerm_present if
                                  * present */
    CMCC_CS_CallReferenceNumber lcsReferenceNumber;  /* optional; set in
                                   * bit_mask
                            * CMCC_CS_NILCSRecord_lcsReferenceNumber_present if
                            * present */
    CMCC_CS_AdditionalChgInfo additionalChgInfo;  /* optional; set in bit_mask
                             * CMCC_CS_NILCSRecord_additionalChgInfo_present if
                             * present */
    CMCC_CS_ChargedParty chargedParty;  /* optional; set in bit_mask
                                  * CMCC_CS_NILCSRecord_chargedParty_present if
                                  * present */
    CMCC_CS_RNCorBSCId orgRNCorBSCId;  /* optional; set in bit_mask
                                 * CMCC_CS_NILCSRecord_orgRNCorBSCId_present if
                                 * present */
    CMCC_CS_MSCId   orgMSCId;  /* optional; set in bit_mask
                                * CMCC_CS_NILCSRecord_orgMSCId_present if
                                * present */
    CMCC_CS_GAI     globalAreaID;  /* optional; set in bit_mask
                                    * CMCC_CS_NILCSRecord_globalAreaID_present
                                    * if present */
    CMCC_CS_SubscriberCategory subscriberCategory;  /* optional; set in bit_mask
                            * CMCC_CS_NILCSRecord_subscriberCategory_present if
                            * present */
    CMCC_CS_MCCMNC  firstmccmnc;  /* optional; set in bit_mask
                                   * CMCC_CS_NILCSRecord_firstmccmnc_present if
                                   * present */
    CMCC_CS_HotBillingTag hotBillingTag;  /* optional; set in bit_mask
                                 * CMCC_CS_NILCSRecord_hotBillingTag_present if
                                 * present */
} CMCC_CS_NILCSRecord;

typedef struct CMCC_CS_CallEventRecord {
    unsigned short  choice;
#       define      CMCC_CS_moCallRecord_chosen 1
#       define      CMCC_CS_mtCallRecord_chosen 2
#       define      CMCC_CS_roamingRecord_chosen 3
#       define      CMCC_CS_incGatewayRecord_chosen 4
#       define      CMCC_CS_outGatewayRecord_chosen 5
#       define      CMCC_CS_transitRecord_chosen 6
#       define      CMCC_CS_moSMSRecord_chosen 7
#       define      CMCC_CS_mtSMSRecord_chosen 8
#       define      CMCC_CS_ssActionRecord_chosen 9
#       define      CMCC_CS_hlrIntRecord_chosen 10
#       define      CMCC_CS_commonEquipRecord_chosen 11
#       define      CMCC_CS_recTypeExtensions_chosen 12
#       define      CMCC_CS_termCAMELRecord_chosen 13
#       define      CMCC_CS_mtLCSRecord_chosen 14
#       define      CMCC_CS_moLCSRecord_chosen 15
#       define      CMCC_CS_niLCSRecord_chosen 16
#       define      CMCC_CS_forwardCallRecord_chosen 17
    union _union {
        CMCC_CS_MOCallRecord moCallRecord;  /* to choose, set choice to
                                             * CMCC_CS_moCallRecord_chosen */
        CMCC_CS_MTCallRecord mtCallRecord;  /* to choose, set choice to
                                             * CMCC_CS_mtCallRecord_chosen */
        CMCC_CS_RoamingRecord roamingRecord;  /* to choose, set choice to
                                              * CMCC_CS_roamingRecord_chosen */
        CMCC_CS_IncGatewayRecord incGatewayRecord;  /* to choose, set choice to
                                           * CMCC_CS_incGatewayRecord_chosen */
        CMCC_CS_OutGatewayRecord outGatewayRecord;  /* to choose, set choice to
                                           * CMCC_CS_outGatewayRecord_chosen */
        CMCC_CS_TransitCallRecord transitRecord;  /* to choose, set choice to
                                              * CMCC_CS_transitRecord_chosen */
        CMCC_CS_MOSMSRecord moSMSRecord;  /* to choose, set choice to
                                           * CMCC_CS_moSMSRecord_chosen */
        CMCC_CS_MTSMSRecord mtSMSRecord;  /* to choose, set choice to
                                           * CMCC_CS_mtSMSRecord_chosen */
        CMCC_CS_SSActionRecord ssActionRecord;  /* to choose, set choice to
                                             * CMCC_CS_ssActionRecord_chosen */
        CMCC_CS_HLRIntRecord hlrIntRecord;  /* to choose, set choice to
                                             * CMCC_CS_hlrIntRecord_chosen */
        CMCC_CS_CommonEquipRecord commonEquipRecord;  /* to choose, set choice
                                       * to CMCC_CS_commonEquipRecord_chosen */
        struct CMCC_CS_ManagementExtensions_ *recTypeExtensions;  /* to choose,
                                   * set choice to
                                   * CMCC_CS_recTypeExtensions_chosen */
        CMCC_CS_TermCAMELRecord termCAMELRecord;  /* to choose, set choice to
                                            * CMCC_CS_termCAMELRecord_chosen */
        CMCC_CS_MTLCSRecord mtLCSRecord;  /* to choose, set choice to
                                           * CMCC_CS_mtLCSRecord_chosen */
        CMCC_CS_MOLCSRecord moLCSRecord;  /* to choose, set choice to
                                           * CMCC_CS_moLCSRecord_chosen */
        CMCC_CS_NILCSRecord niLCSRecord;  /* to choose, set choice to
                                           * CMCC_CS_niLCSRecord_chosen */
        CMCC_CS_MOCallRecord forwardCallRecord;  /* to choose, set choice to
                                          * CMCC_CS_forwardCallRecord_chosen */
    } u;
} CMCC_CS_CallEventRecord;

typedef enum CMCC_CS_IMEIStatus {
    CMCC_CS_greyListedMobileEquipment = 0,
    CMCC_CS_blackListedMobileEquipment = 1,
    CMCC_CS_nonWhiteListedMobileEquipment = 2
} CMCC_CS_IMEIStatus;

typedef int             CMCC_CS_IMEICheckEvent;
#define                     CMCC_CS_mobileOriginatedCall 0
#define                     CMCC_CS_mobileTerminatedCall 1
#define                     CMCC_CS_smsMobileOriginating 2
#define                     CMCC_CS_smsMobileTerminating 3
#define                     CMCC_CS_IMEICheckEvent_ssAction 4
#define                     CMCC_CS_locationUpdate 5

typedef struct CMCC_CS_ObservedIMEITicket {
    unsigned char   bit_mask;
#       define      CMCC_CS_ObservedIMEITicket_servedMSISDN_present 0x80
#       define      CMCC_CS_imeiCheckEvent_present 0x40
#       define      CMCC_CS_ObservedIMEITicket_callReference_present 0x20
#       define      CMCC_CS_ObservedIMEITicket_recordExtensions_present 0x10
#       define      CMCC_CS_ObservedIMEITicket_orgMSCId_present 0x08
    CMCC_CS_IMEI    servedIMEI;
    CMCC_CS_IMEIStatus imeiStatus;
    CMCC_CS_IMSI    servedIMSI;
    CMCC_CS_MSISDN  servedMSISDN;  /* optional; set in bit_mask
                           * CMCC_CS_ObservedIMEITicket_servedMSISDN_present if
                           * present */
    CMCC_CS_RecordingEntity recordingEntity;
    CMCC_CS_TimeStamp eventTime;
    CMCC_CS_LocationAreaAndCell location;
    CMCC_CS_IMEICheckEvent imeiCheckEvent;  /* optional; set in bit_mask
                                             * CMCC_CS_imeiCheckEvent_present if
                                             * present */
    CMCC_CS_CallReference callReference;  /* optional; set in bit_mask
                          * CMCC_CS_ObservedIMEITicket_callReference_present if
                          * present */
    struct CMCC_CS_ManagementExtensions_ *recordExtensions;  /* optional; set in
                                   * bit_mask
                       * CMCC_CS_ObservedIMEITicket_recordExtensions_present if
                       * present */
    CMCC_CS_MSCId   orgMSCId;  /* optional; set in bit_mask
                                * CMCC_CS_ObservedIMEITicket_orgMSCId_present if
                                * present */
} CMCC_CS_ObservedIMEITicket;

typedef struct CMCC_CS_HeaderRecord {
    CMCC_CS_TimeStamp productionDateTime;
    CMCC_CS_RecordingEntity recordingEntity;
    struct CMCC_CS_ManagementExtensions_ *extensions;
} CMCC_CS_HeaderRecord;

typedef struct CMCC_CS_TrailerRecord {
    CMCC_CS_TimeStamp productionDateTime;
    CMCC_CS_RecordingEntity recordingEntity;
    CMCC_CS_TimeStamp firstCallDateTime;
    CMCC_CS_TimeStamp lastCallDateTime;
    int             noOfRecords;
    struct CMCC_CS_ManagementExtensions_ *extensions;
} CMCC_CS_TrailerRecord;

typedef struct CMCC_CS_CallEventDataFile {
    CMCC_CS_HeaderRecord headerRecord;
    struct CMCC_CS__seqof8 {
        struct CMCC_CS__seqof8 *next;
        CMCC_CS_CallEventRecord value;
    } *callEventRecords;
    CMCC_CS_TrailerRecord trailerRecord;
    struct CMCC_CS_ManagementExtensions_ *extensions;
} CMCC_CS_CallEventDataFile;

typedef struct CMCC_CS_ObservedIMEITicketFile {
    CMCC_CS_TimeStamp productionDateTime;
    struct CMCC_CS__seqof9 {
        struct CMCC_CS__seqof9 *next;
        CMCC_CS_ObservedIMEITicket value;
    } *observedIMEITickets;
    int             noOfRecords;
    struct CMCC_CS_ManagementExtensions_ *extensions;
} CMCC_CS_ObservedIMEITicketFile;

typedef struct CMCC_CS_BasicServices_ {
    struct CMCC_CS_BasicServices_ *next;
    CMCC_CS_BasicServiceCode value;
} *CMCC_CS_BasicServices;

typedef int             CMCC_CS_CallType;
#define                     CMCC_CS_mobileOriginated 0
#define                     CMCC_CS_mobileTerminated 1

typedef struct CMCC_CS_CallTypes_ {
    struct CMCC_CS_CallTypes_ *next;
    CMCC_CS_CallType value;
} *CMCC_CS_CallTypes;

typedef enum CMCC_CS_ChannelCoding {
    CMCC_CS_tchF4800 = 1,
    CMCC_CS_tchF9600 = 2,
    CMCC_CS_tchF14400 = 3
} CMCC_CS_ChannelCoding;

typedef unsigned char   CMCC_CS_Day;

typedef ossBoolean      CMCC_CS_EmergencyCallIndEnable;

typedef struct CMCC_CS_IMSIorIMEI {
    unsigned short  choice;
#       define      CMCC_CS_imsi_chosen 1
#       define      CMCC_CS_imei_chosen 2
    union _union {
        CMCC_CS_IMSI    imsi;  /* to choose, set choice to
                                * CMCC_CS_imsi_chosen */
        CMCC_CS_IMEI    imei;  /* to choose, set choice to
                                * CMCC_CS_imei_chosen */
    } u;
} CMCC_CS_IMSIorIMEI;

typedef struct CMCC_CS_EmergencyCallIndication {
    CMCC_CS_CellId  cellId;
    CMCC_CS_IMSIorIMEI callerId;
} CMCC_CS_EmergencyCallIndication;

typedef int             CMCC_CS_FileType;
#define                     CMCC_CS_callRecords 1
#define                     CMCC_CS_traceRecords 9
#define                     CMCC_CS_observedIMEITicket 14

typedef CMCC_CS_BCDDirectoryNumber CMCC_CS_GenericNumber;

typedef struct CMCC_CS_GenericNumbers_ {
    struct CMCC_CS_GenericNumbers_ *next;
    CMCC_CS_GenericNumber value;
} *CMCC_CS_GenericNumbers;

typedef int             CMCC_CS_NumOfHSCSDChanAllocated;

typedef enum CMCC_CS_InitiatingParty {
    CMCC_CS_network = 0,
    CMCC_CS_subscriber = 1
} CMCC_CS_InitiatingParty;

typedef int             CMCC_CS_NumOfHSCSDChanRequested;

typedef struct CMCC_CS_HSCSDParmsChange {
    unsigned char   bit_mask;
#       define      CMCC_CS_initiatingParty_present 0x80
#       define      CMCC_CS_HSCSDParmsChange_aiurRequested_present 0x40
#       define      CMCC_CS_hSCSDChanRequested_present 0x20
    CMCC_CS_TimeStamp changeTime;
    CMCC_CS_NumOfHSCSDChanAllocated hSCSDChanAllocated;
    CMCC_CS_InitiatingParty initiatingParty;  /* optional; set in bit_mask
                                               * CMCC_CS_initiatingParty_present
                                               * if present */
    CMCC_CS_AiurRequested aiurRequested;  /* optional; set in bit_mask
                            * CMCC_CS_HSCSDParmsChange_aiurRequested_present if
                            * present */
    CMCC_CS_ChannelCoding chanCodingUsed;
    CMCC_CS_NumOfHSCSDChanRequested hSCSDChanRequested;  /* optional; set in
                                   * bit_mask CMCC_CS_hSCSDChanRequested_present
                                   * if present */
} CMCC_CS_HSCSDParmsChange;

typedef CMCC_CS_ISDN_AddressString CMCC_CS_MscNo;

typedef struct CMCC_CS_Location_info {
    unsigned char   bit_mask;
#       define      CMCC_CS_mscNumber_present 0x80
#       define      CMCC_CS_cell_identification_present 0x40
    CMCC_CS_MscNo   mscNumber;  /* optional; set in bit_mask
                                 * CMCC_CS_mscNumber_present if present */
    CMCC_CS_LocationAreaCode location_area;
    CMCC_CS_CellId  cell_identification;  /* optional; set in bit_mask
                                           * CMCC_CS_cell_identification_present
                                           * if present */
} CMCC_CS_Location_info;

typedef CMCC_CS_Diagnostics CMCC_CS_LocUpdResult;

typedef struct CMCC_CS_ManagementExtensions_ {
    struct CMCC_CS_ManagementExtensions_ *next;
    CMCC_CS_ManagementExtension value;
} *CMCC_CS_ManagementExtensions;

typedef unsigned char   CMCC_CS_Month;

typedef int             CMCC_CS_RFPowerCapability;

typedef struct CMCC_CS_MSPowerClasses_ {
    struct CMCC_CS_MSPowerClasses_ *next;
    CMCC_CS_RFPowerCapability value;
} *CMCC_CS_MSPowerClasses;

typedef int             CMCC_CS_NetworkSpecificCode;

typedef struct CMCC_CS_NetworkSpecificServices_ {
    struct CMCC_CS_NetworkSpecificServices_ *next;
    CMCC_CS_NetworkSpecificCode value;
} *CMCC_CS_NetworkSpecificServices;

typedef ossBoolean      CMCC_CS_ObservedIMEITicketEnable;

typedef struct CMCC_CS_OriginDestCombination {
    unsigned char   bit_mask;
#       define      CMCC_CS_origin_present 0x80
#       define      CMCC_CS_destination_present 0x40
    int             origin;  /* optional; set in bit_mask CMCC_CS_origin_present
                              * if present */
    int             destination;  /* optional; set in bit_mask
                                   * CMCC_CS_destination_present if present */
} CMCC_CS_OriginDestCombination;

typedef struct CMCC_CS_OriginDestCombinations_ {
    struct CMCC_CS_OriginDestCombinations_ *next;
    CMCC_CS_OriginDestCombination value;
} *CMCC_CS_OriginDestCombinations;

typedef int             CMCC_CS_PartialRecordTimer;

typedef struct CMCC_CS_PartialRecordTypes_ {
    struct CMCC_CS_PartialRecordTypes_ *next;
    CMCC_CS_PartialRecordType value;
} *CMCC_CS_PartialRecordTypes;

typedef struct CMCC_CS_RadioChannelsRequested_ {
    struct CMCC_CS_RadioChannelsRequested_ *next;
    CMCC_CS_RadioChanRequested value;
} *CMCC_CS_RadioChannelsRequested;

typedef enum CMCC_CS_RecordingMethod {
    CMCC_CS_inCallRecord = 0,
    CMCC_CS_inSSRecord = 1
} CMCC_CS_RecordingMethod;

typedef struct CMCC_CS_Service {
    unsigned short  choice;
#       define      CMCC_CS_Service_teleservice_chosen 1
#       define      CMCC_CS_Service_bearerService_chosen 2
#       define      CMCC_CS_supplementaryService_chosen 3
#       define      CMCC_CS_networkSpecificService_chosen 4
    union _union {
        CMCC_CS_TeleserviceCode teleservice;  /* to choose, set choice to
                                        * CMCC_CS_Service_teleservice_chosen */
        CMCC_CS_BearerServiceCode bearerService;  /* to choose, set choice to
                                      * CMCC_CS_Service_bearerService_chosen */
        CMCC_CS_SS_Code supplementaryService;  /* to choose, set choice to
                                       * CMCC_CS_supplementaryService_chosen */
        CMCC_CS_NetworkSpecificCode networkSpecificService;  /* to choose, set
                                   * choice to
                                   * CMCC_CS_networkSpecificService_chosen */
    } u;
} CMCC_CS_Service;

typedef struct CMCC_CS_ServiceDistanceDependency {
    unsigned char   bit_mask;
#       define      CMCC_CS_chargingZone_present 0x80
    int             aocService;
    int             chargingZone;  /* optional; set in bit_mask
                                    * CMCC_CS_chargingZone_present if present */
} CMCC_CS_ServiceDistanceDependency;

typedef struct CMCC_CS_ServiceDistanceDependencies_ {
    struct CMCC_CS_ServiceDistanceDependencies_ *next;
    CMCC_CS_ServiceDistanceDependency value;
} *CMCC_CS_ServiceDistanceDependencies;

typedef int             CMCC_CS_SimpleIntegerName;

typedef char            *CMCC_CS_SimpleStringName;

typedef struct CMCC_CS_SupplServices_ {
    struct CMCC_CS_SupplServices_ *next;
    CMCC_CS_SS_Code value;
} *CMCC_CS_SupplServices;

typedef struct CMCC_CS_SwitchoverTime {
    unsigned char   hour;
    unsigned char   minute;
    unsigned char   second;
} CMCC_CS_SwitchoverTime;

typedef int             CMCC_CS_TariffId;

typedef struct CMCC_CS_TariffPeriod {
    CMCC_CS_SwitchoverTime switchoverTime;
    int             tariffId;
} CMCC_CS_TariffPeriod;

typedef struct CMCC_CS_TariffPeriods_ {
    struct CMCC_CS_TariffPeriods_ *next;
    CMCC_CS_TariffPeriod value;
} *CMCC_CS_TariffPeriods;

typedef enum CMCC_CS_TariffSystemStatus {
    CMCC_CS_available = 0,
    CMCC_CS_checked = 1,
    CMCC_CS_standby = 2,
    CMCC_CS_active = 3
} CMCC_CS_TariffSystemStatus;

typedef struct CMCC_CS_TSChangeover {
    unsigned char   bit_mask;
#       define      CMCC_CS_authkey_present 0x80
#       define      CMCC_CS_checksum_present 0x40
#       define      CMCC_CS_versionNumber_present 0x20
    int             newActiveTS;
    int             newStandbyTS;
    CMCC_CS__octet1 authkey;  /* optional; set in bit_mask
                               * CMCC_CS_authkey_present if present */
    CMCC_CS__octet1 checksum;  /* optional; set in bit_mask
                                * CMCC_CS_checksum_present if present */
    CMCC_CS__octet1 versionNumber;  /* optional; set in bit_mask
                                     * CMCC_CS_versionNumber_present if
                                     * present */
} CMCC_CS_TSChangeover;

typedef struct CMCC_CS_TSCheckErrorId {
    unsigned short  choice;
#       define      CMCC_CS_globalForm_chosen 1
#       define      CMCC_CS_localForm_chosen 2
    union _union {
        CMCC_CS__ObjectID globalForm;  /* to choose, set choice to
                                        * CMCC_CS_globalForm_chosen */
        int             localForm;  /* to choose, set choice to
                                     * CMCC_CS_localForm_chosen */
    } u;
} CMCC_CS_TSCheckErrorId;

typedef struct CMCC_CS_TSCheckError {
    CMCC_CS_TSCheckErrorId errorId;
} CMCC_CS_TSCheckError;

typedef struct CMCC_CS_TSCheckResult {
    unsigned short  choice;
#       define      CMCC_CS_success_chosen 1
#       define      CMCC_CS_fail_chosen 2
    union _union {
        Nulltype        success;  /* to choose, set choice to
                                   * CMCC_CS_success_chosen */
        struct CMCC_CS__setof1 {
            struct CMCC_CS__setof1 *next;
            CMCC_CS_TSCheckError value;
        } *fail;  /* to choose, set choice to CMCC_CS_fail_chosen */
    } u;
} CMCC_CS_TSCheckResult;

typedef struct CMCC_CS_TSCopyTariffSystem {
    int             oldTS;
    int             newTS;
} CMCC_CS_TSCopyTariffSystem;

typedef struct CMCC_CS_TSNextChange {
    unsigned short  choice;
#       define      CMCC_CS_noChangeover_chosen 1
#       define      CMCC_CS_tsChangeover_chosen 2
    union _union {
        Nulltype        noChangeover;  /* to choose, set choice to
                                        * CMCC_CS_noChangeover_chosen */
        CMCC_CS_TSChangeover tsChangeover;  /* to choose, set choice to
                                             * CMCC_CS_tsChangeover_chosen */
    } u;
} CMCC_CS_TSNextChange;

typedef enum CMCC_CS_TypeOfSubscribers {
    CMCC_CS_home = 0,
    CMCC_CS_visiting = 1,
    CMCC_CS_TypeOfSubscribers_all = 2
} CMCC_CS_TypeOfSubscribers;

typedef enum CMCC_CS_TypeOfTransaction {
    CMCC_CS_successful = 0,
    CMCC_CS_unsuccessful = 1,
    CMCC_CS_TypeOfTransaction_all = 2
} CMCC_CS_TypeOfTransaction;

typedef struct CMCC_CS_CallEmlppPriority {
    unsigned short  length;
    unsigned char   value[1];
} CMCC_CS_CallEmlppPriority;

typedef enum CMCC_CS_EmergencyCallFlag {
    CMCC_CS_notEmergencyCall = 0,
    CMCC_CS_emergencyCall = 1
} CMCC_CS_EmergencyCallFlag;

typedef enum CMCC_CS_DisconnectParty {
    CMCC_CS_callingPartyRelease = 0,
    CMCC_CS_calledPartyRelease = 1,
    CMCC_CS_networkRelease = 2
} CMCC_CS_DisconnectParty;

typedef CMCC_CS_BCDDirectoryNumber CMCC_CS_OptimalRoutingDestAddress;

extern const int CMCC_CS_maxAddressLength;

extern const int CMCC_CS_maxExt_GeographicalInformation;

extern const int CMCC_CS_maxISDN_AddressLength;


extern void *CMCC_CS_R4_CDR;    /* encoder-decoder control table */
#ifdef __cplusplus
}	/* extern "C" */
#endif /* __cplusplus */
#endif /* OSS_CMCC_CS_R4_CDR */
