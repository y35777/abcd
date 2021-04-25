#ifndef __MSCV9_BILL_FORMAT_H__
#define __MSCV9_BILL_FORMAT_H__

#include "../include/base_type.h"

#pragma pack (1)

#define  FALSE           0
#define  TRUE            1
#define  NULL            0

typedef  int				BOOL;
typedef  long               INT4;
typedef  short              INT2;
typedef  int                INT;

typedef  unsigned long      UINT4;
typedef  unsigned short     UINT2;
typedef  unsigned char      BYTE;

#define SMSTEXT_LENGTH  147

struct  BillClassmark3
{
    // Fixed 2 Bytes
    BYTE  BAND3Supported   : 1 ;
    BYTE  BAND2Supported   : 1 ;
    BYTE  BAND1Supported   : 1 ;
    // Volatile Field
    BYTE UMTSFDDRATC       :1;// UMTS FDD Radio Access Technology Capability (1 bit field)
    BYTE UMTS384McpsTDDRATC:1;// UMTS 3.84 Mcps TDD Radio Access Technology Capability (1 bit field)
    BYTE UMTS128McpsTDDRATC:1;// UMTS 1.28 Mcps TDD Radio Access Technology Capability (1 bit field)
    BYTE  GSM_400_Bands_Supported:2;
    BYTE  GSM_Band         :4;
    BYTE  reserved         :4;
};

struct MBILL_CHANGEFLAGS
{
	BYTE     CAMELChangeFlags_reserved           :2;
	BYTE     redirectionCounterModified          :1; 
	BYTE     redirectingPartyNumberModified      :1;
	BYTE     genericNumbersModified              :1;
	BYTE     originalCalledPartyNumberModified   :1;
	BYTE     callingPartyCategoryModified        :1;
	BYTE     callingPartyNumberModified          :1;
};

enum AppendIndication
{
    OverWrite,
    Append
};

enum ACCESS_NETWORK_TYPE
{
    ACCESS_BSC  = 0,   // 2GÍøÂç
    ACCESS_RNC  = 1,   // 3GÍøÂç
    ACCESS_UNKNOWN = 2, //Î´ÖªÍøÂç 
    ACCESS_BUTT = 3
};

enum EVENT_TYPE
{
       Hold_Call = 0,
       Retrieve_Call,
       CallWait,
       Build_MPTY,
       Split_MPTY,
       Hold_MPTY,
       Retrieve_MPTY,
       HO,
       EVENT_TYPE_BUTT      
};

enum GSM_SS_CODE
{
    SS_CLIP					=0x11,
    SS_CLIR					=0x12,
    SS_CFU					=0x21,
    SS_CD					=0x24,
    SS_CFB					=0x29,
    SS_CFNRY				=0x2a,
    SS_CFNRC				=0x2b,
    SS_ECT					=0x31,
    SS_CWAIT				=0x41,
    SS_CHOLD				=0x42,
    SS_CCBSA    			=0x43,
    SS_CCBSB				=0x44,
    SS_MC					=0x45,
    SS_CMPTY				=0x51,
    SS_CUG					=0x61,
    SS_AOCI					=0x71,
    SS_AOCC					=0x72,
    SS_UUS1					=0x81,
    SS_UUS2					=0x82,
    SS_UUS3					=0x83,
    SS_EMLPP				=0xa1,
    SS_ALLMOLR				=0xc0,
    SS_PLMN_SPECIFICSS0  	=0xf0,
    SS_PLMN_SPECIFICSS1  	=0xf1,
    SS_PLMN_SPECIFICSS2  	=0xf2,
    SS_PLMN_SPECIFICSS3  	=0xf3,
    SS_PLMN_SPECIFICSS4  	=0xf4,
    SS_PLMN_SPECIFICSS5  	=0xf5,
    SS_PLMN_SPECIFICSS6  	=0xf6,
    SS_PLMN_SPECIFICSS7  	=0xf7,
    SS_PLMN_SPECIFICSS8  	=0xf8,
    SS_PLMN_SPECIFICSS9  	=0xf9,
    SS_PLMN_SPECIFICSS10  	=0xfa,
    SS_PLMN_SPECIFICSS11  	=0xfb,
    SS_PLMN_SPECIFICSS12  	=0xfc,
    SS_PLMN_SPECIFICSS13  	=0xfd,
    SS_PLMN_SPECIFICSS14  	=0xfe,
    SS_PLMN_SPECIFICSS15  	=0xff    
};

enum  Action_Result {
      GSM_NO_ACTION, 
      Register_SS,  
      Erase_SS, 
      Active_SS,   
      Deactive_SS,   
      Interrogate_SS, 
      Invoke_SS, 
      GSM_HOT_BILL_SEND_OK,    
      SM_transmission_success,    
      SM_transmission_failed      
};


typedef struct
{
    BYTE       type;
    BYTE       SS_Para_len;
    union 
    {
        BYTE    forwardedToNumber[24];         // [0] ForwardToNumber
        BYTE    unstructuredData[40];          // [1] OCTET STRING
    }data;
}SSParameters;

typedef struct
{
    BYTE RFC       :3;
    BYTE en_alg    :2;
    BYTE rev_level :3;
    BYTE FC        :3;
    BYTE SMC       :1;
    BYTE ss_ind    :2;
    BYTE ps_cap    :1;
    BYTE s2        :1;
    BYTE A5_2      :1;
    BYTE A5_3      :1;
    BYTE s3        :5;
    BYTE cm_3      :1;
} Classmark;

typedef struct
{
    BYTE    spare1              : 1;
    BYTE    RevisionLevel       : 2;
    BYTE    ES_IND              : 1;
    BYTE    A5_bit1             : 1;
    BYTE    RFpowerCapability   : 3;
    BYTE    spare2              : 1;
    BYTE    psCapability        : 1;
    BYTE    ssScreenIndicator   : 2;
    BYTE    smCapability        : 1;
    BYTE    VBS                 : 1;
    BYTE    VGCS                : 1;
    BYTE    FC                  : 1;
    BYTE    CM3                 : 1;
    BYTE    spare3              : 1;
    BYTE    lcsvacap            : 1;
    BYTE    ucs2                : 1;
    BYTE    solsa               : 1;
    BYTE    cmsp                : 1;
    BYTE    A5_bit3             : 1;
    BYTE    A5_bit2             : 1;
} ClassMark2;

typedef struct
{
    BYTE   CauseType;
    union
    {
        UINT4     gsm0408Cause;                // CauseType == 0
        UINT4     gsm0902MapErrorValue;        // CauseType == 1
        UINT4     ccittQ767Cause;              // CauseType == 2
    } Cause;
} SDiagnostics;

typedef struct
{
    BYTE    aoc;
    UINT2   e[7];
    UINT2   tariff_rate;
} AOC_E1_E7;

enum B02_GSM_BILL_TYPE
{
    MOC					= 0,	//ÒÆ¶¯Ê¼·¢ºô½Ð
    MTC					= 1,	//ÒÆ¶¯ÖÕ½áºô½Ð
    GATEWAY_INCOMING	= 2,	//Èë¹Ø¿Ú¾Ö»°µ¥
    EC					= 3,	//½ô¼±ºô½Ð
    TRANSIT				= 4,	//ÖÐ×ª£¬»ã½Óºô½Ð
    SS_ACT				= 5,	//²¹³äÒµÎñ±à³Ì¶¯×÷£¬×¢²á¡¢×¢Ïú¡¢¼¤»î¡¢È¥»°
    MT_SMS				= 6,	//ÒÆ¶¯Ì¨ÖÕ½á¶ÌÏûÏ¢ÒµÎñ
    MO_SMS				= 7,    //ÒÆ¶¯Ì¨Ê¼·¢¶ÌÏûÏ¢ÒµÎñ
    LOCATION_FREASH		= 8,    //Î»ÖÃ¸üÐÂ
    CALL_ATTEMPT		= 9,    //ÊÔºô»°µ¥
	GATEWAY_OUTGOING	= 10,   //³ö¹Ø¿Ú¾Ö»°µ¥  
    ISDN_OC             = 11,   
    ISDN_TC             = 12,   
    ROAM                = 13,   //ÂþÓÎ»°µ¥
    CommonEquip         = 14,   //¹«¹²Éè±¸»°µ¥
    MO_FREECALL         = 17,   //Ö÷½ÐÖÇÄÜÓÃ»§Ãâ·Ñ»°µ¥ÀàÐÍ
    CFW					= 18,   //Ç°×ª»°µ¥
    MOI					= 19,   //´ú×ö»°µ¥
    MO_HO               = 20,   //Ö÷½ÐÇÐ»»»°µ¥
    MT_HO               = 21,   //±»½ÐÇÐ»»»°µ¥
    MT_FREECALL         = 22,   //±»½ÐÖÇÄÜÓÃ»§Ãâ·Ñ»°µ¥ÀàÐÍ
    CFW_FREECALL        = 23,   // ÖÇÄÜÇ°×ªÊ±Ô­±»½ÐÖÇÄÜÓÃ»§Ãâ·Ñ»°µ¥ÀàÐÍ 
    TCAMEL				= 24,   //ÖÕ½áCAMEL·ÃÎÊ¼ÇÂ¼
    FCI					= 25,   //×ÔÓÉ¸ñÊ½Êý¾Ý»°µ¥
 	LCS					= 26,   //Î»ÖÃÒµÎñ»°µ¥ 
    Check_IMEI			= 27,   //Check IMEI»°µ¥ÀàÐÍ
    QUERY_HLR			= 28,   //¹Ø¿Ú¾ÖHLRÎÊÑ¯»°µ¥ÀàÐÍ
    EVENT_BILL          = 30
};

enum HOT_BILLING_MARK
{
    NORMAL_BILLING     = 0x00,        //ÆÕÍ¨¼Æ·Ñ
    HOT_BILLING        = 0x01         //ÈÈ¼Æ·Ñ
};

enum LOCATION_TYPE 
{
    MT_LR,
    MO_LR,    
    NI_LR,    
    LT_BUTT 
};

enum GSM_RECORD_TYPE
{
    SINGLE_BILL,            //±¾´Îºô½ÐÖ»²úÉúÒ»ÕÅ»°µ¥
    FIRST_BILL,             //±¾´Îºô½ÐµÚÒ»ÕÅ»°µ¥
    INTERMEDIATE_BILL,      //ÖÐ¼ä»°µ¥
    LAST_BILL               //±¾´Îºô½Ð×îºóÒ»ÕÅ»°µ¥
};

enum MBILL_TBS
{
    MBILL_TELECOM_SERVICE,  //µçÐÅÒµÎñ
    MBILL_BEAR_SERVICE,     //³ÐÔØÒµÎñ
    MBILL_ISDN_SERVICE,     // ISDNÒµÎñ
};

typedef struct
{
    BYTE     b_gsmSCFAddress         :1;
    BYTE     b_ServiceKey            :1;
    BYTE     b_DefaultSmsHandling    :1;
    BYTE     b_FreeFormatData        :1;
    BYTE     b_CallingNumber         :1;
    BYTE     b_CalledNumber          :1;
    BYTE     b_cAMELSMSCAddress      :1;
    BYTE     reserved                :1;
}CamelSMSInfoInd;


enum ROUTE_INDICATOR
{
    SHOW_ROUTE_NO = 0,
    SHOW_ROUTE_NAME = 1,
    SHOW_ROUTE_NULL = 2
};


enum CAUSE_FOR_PARTIAL_RECORDS
{
    normalRelease                    = 0x00,
    partialRecord	                 = 0x01,
    partialRecordCallReestablishment = 0x02,
    unsuccessfulCallAttempt	         = 0x03,
    stableCallAbnormalTermination    = 0x04,
    cAMELInitCallRelease	         = 0x05
};

//by    ÑîÉÆ    2004-6-2    ¶ÔÓ¦ÎÊÌâµ¥SWPD05614
struct MSC_Bill_Report
{
   	// ¹«¹²Óò²¿·Ö, »ù±¾Çø:
	BYTE		ValidFlag;
	UINT2		Size;
	UINT2		CRCCode;
	BYTE		bill_type;
	UINT4		bill_sequence;
	BYTE		ModuleNum;
	UINT4		FirstSequence;
	BYTE		sequence;
	BYTE		record_type		: 3;
	BYTE		HotBillingTag	: 1;
	BYTE		CRC_Way			: 2;
	BYTE		ChargeLevel		: 1;
	BYTE		Comm_reserved_bit	: 1;
	BYTE		Comm_reserved;

	BYTE		DataBlock4[812];
};
//the end  ¶ÔÓ¦ÎÊÌâµ¥SWPD05614

enum SPEECH_VERSION
{
    GSM_SPEECH_FULL_RATE_VERSION_1   =   1,
    GSM_SPEECH_HALF_HALF_VERSION_1   =   5,
    GSM_SPEECH_FULL_RATE_VERSION_2   =   17,
    GSM_SPEECH_HALF_HALF_VERSION_2   =   21,
    GSM_SPEECH_FULL_RATE_VERSION_3   =   33,
    GSM_SPEECH_HALF_HALF_VERSION_3   =   37
};

#define MAX_IMEI_LEN	8
#define MAX_IMSI_LEN	8
#define	MAX_NUM_LEN		32
#define MAX_MSC_NUM_IN_BYTES	8
#define MAX_TRUNK_GROUP_NAME_LENGHT		10
#define MAX_LOCAL_LAI_LEN	10
#define	MAX_LOCAL_CI_LEN	4
#define	maxCallReferenceNumberLength	8
#define	MAX_SCF_LEN		8
#define	MAX_CHARGE_AREA_CODE_LEN	3
#define	MAX_RNCBSC_ID_LEN	3
#define MAX_MSCID_LEN	3
#define MAX_NAEA_CODE_IN_BYTE	2
#define MAX_CIC_LEN_IN_BYTE		3
#define MAX_SCFID_LEN	5

#define MAX_TRANSFER_FCI_LEN	160
#define MAX_MSC_NUM_IN_BYTES 	8
#define MAX_CHARGE_AREA_CODE_LEN	3
#define MAX_SMSTEXT_LEN		164
#define	maxShapeInfoLength	200
#define MAX_LCS_CLIENT_EXTERNAL_ID_LEN	20
#define MAX_GMLC_LEN	8
#define MAX_MSC_CODE_LEN	6 
#define MAX_USSDACCESSCODE_LEN	2

#define MAX_ISDN_BC_LEN     12
#define MAX_LLC_LEN         18
#define MAX_HLC_LEN         5

//by    ÑîÉÆ    2004-6-2    ¶ÔÓ¦ÎÊÌâµ¥SWPD05614
#define	MAX_CONTAINER_MOC_LEN	39
#define MAX_CONTAINER_MTC_LEN	44
#define MAX_CONTAINER_TCAMEL_LEN 45
#define MAX_CONTAINER_ROAM_LEN	45
#define MAX_CONTAINER_GATEWAY_LEN	19
#define MAX_CONTAINER_HLRQUERY_LEN	6
#define MAX_CONTAINER_SMSMO_LEN	11
#define MAX_CONTAINER_SMSMT_LEN	3
#define MAX_CONTAINER_LCS_LEN	10
#define MAX_CONTAINER_SSACT_LEN	7
#define MAX_CONTAINER_CEU_LEN	7
#define MAX_CONTAINER_CHECKIMEI_LEN	6
//the end  ¶ÔÓ¦ÎÊÌâµ¥SWPD05614

enum MBILL_CHARGED_PARTY
{
	MBILL_CALLINGPARTY  = 0,
		MBILL_CALLEDPARTY   = 1,
		MBILL_CHARGED_PARTY_BUTT = 0xFF
};


#ifdef BIG_ENDIAN
struct MBILL_SS_code1
{
      BYTE   ECT        :1;
      BYTE   CMPTY      :1;
      BYTE   CWAIT      :1;
      BYTE   CHOLD      :1;
      BYTE   CFNRC      :1;
      BYTE   CFNRY      :1;
      BYTE   CFB        :1;
      BYTE   CFU        :1;
};
// SS_code2×Ö¶Î±íÊ¾ÈçÏÂ²¹³äÒµÎñ£º
struct MBILL_SS_code2
{
       BYTE    AOCC       :1;
       BYTE    AOCI        :1;
       BYTE    CUG         :1; 
       BYTE    EMLPP       :1;
       BYTE    MC           :1;
       BYTE    CCBSB     :1;
       BYTE    CCBSA     :1;
       BYTE    CD           :1;
};

// SS_code3×Ö¶Î±íÊ¾ÈçÏÂ²¹³äÒµÎñ£º
struct MBILL_SS_code3
{
        BYTE   reserve4      :1;
        BYTE   reserve3      :1;
        
        //by    ÑîÉÆ    2004-6-2    ¶ÔÓ¦ÎÊÌâµ¥SWPD05614
        BYTE   CLIR			 :1;
        BYTE   CLIP          :1;
        //the end  ¶ÔÓ¦ÎÊÌâµ¥SWPD05614

        BYTE   ALLMOLR       :1;
        BYTE   UUS3          :1;
        BYTE   UUS2          :1;
        BYTE   UUS1          :1;
};
// SS_code4×Ö¶Î±íÊ¾ÈçÏÂ²¹³äÒµÎñ£º
struct MBILL_SS_code4
{
        BYTE    plmn_specificSS7      :1;
        BYTE    plmn_specificSS6      :1;
        BYTE    plmn_specificSS5      :1;
        BYTE    plmn_specificSS4      :1;
        BYTE    plmn_specificSS3      :1;
        BYTE    plmn_specificSS2      :1;
        BYTE    plmn_specificSS1      :1;
        BYTE    plmn_specificSS0      :1;
};
// SS_code5×Ö¶Î±íÊ¾ÈçÏÂ²¹³äÒµÎñ£º
struct MBILL_SS_code5
{
       BYTE     plmn_specificSS15        :1;
       BYTE     plmn_specificSS14        :1;
       BYTE     plmn_specificSS13        :1;
       BYTE     plmn_specificSS12        :1;
       BYTE     plmn_specificSS11        :1;
       BYTE     plmn_specificSS10        :1;
       BYTE     plmn_specificSS9         :1;
       BYTE     plmn_specificSS8         :1;
};

#else
struct MBILL_SS_code1
{
      BYTE   CFU        :1;
      BYTE   CFB        :1;
      BYTE   CFNRY      :1;
      BYTE   CFNRC      :1;
      BYTE   CHOLD      :1;
      BYTE   CWAIT      :1;
      BYTE   CMPTY      :1;
      BYTE   ECT        :1;
};
// SS_code2×Ö¶Î±íÊ¾ÈçÏÂ²¹³äÒµÎñ£º
struct MBILL_SS_code2
{
       BYTE    CD        :1;
       BYTE    CCBSA     :1;
       BYTE    CCBSB     :1;
       BYTE    MC        :1;
       BYTE    EMLPP     :1;
       BYTE    CUG       :1; 
       BYTE    AOCI      :1;
       BYTE    AOCC      :1;
};

// SS_code3×Ö¶Î±íÊ¾ÈçÏÂ²¹³äÒµÎñ£º
struct MBILL_SS_code3
{
        BYTE   UUS1           :1;
        BYTE   UUS2           :1;
        BYTE   UUS3           :1;
        BYTE   ALLMOLR        :1;
        BYTE   CLIP		      :1;
        BYTE   CLIR           :1;
        BYTE   reserve3       :1;
        BYTE   reserve4       :1;
};
// SS_code4×Ö¶Î±íÊ¾ÈçÏÂ²¹³äÒµÎñ£º
struct MBILL_SS_code4
{
        BYTE    plmn_specificSS0      :1;
        BYTE    plmn_specificSS1      :1;
        BYTE    plmn_specificSS2      :1;
        BYTE    plmn_specificSS3      :1;
        BYTE    plmn_specificSS4      :1;
        BYTE    plmn_specificSS5      :1;
        BYTE    plmn_specificSS6      :1;
        BYTE    plmn_specificSS7      :1;
};
// SS_code5×Ö¶Î±íÊ¾ÈçÏÂ²¹³äÒµÎñ£º
struct MBILL_SS_code5
{
       BYTE     plmn_specificSS8         :1;
       BYTE     plmn_specificSS9         :1;
       BYTE     plmn_specificSS10        :1;
       BYTE     plmn_specificSS11        :1;
       BYTE     plmn_specificSS12        :1;
       BYTE     plmn_specificSS13        :1;
       BYTE     plmn_specificSS14        :1;
       BYTE     plmn_specificSS15        :1;
};
#endif

//1.1	MOC_Bill_Report£¨°üÀ¨MOC(moCallRecord) /CFW(forwardCallRecord:½Ó¿ÚÍ¬MOCallRecord)/MOI£©
struct MOC_Bill_Report
 {
   	// ¹«¹²Óò²¿·Ö, »ù±¾Çø:
	BYTE		ValidFlag;
	UINT2		Size;
	UINT2		CRCCode;
	BYTE		bill_type;
	UINT4		bill_sequence;
	BYTE		ModuleNum;
	UINT4		count;
	BYTE		sequence;
	BYTE		record_type		: 3;
	BYTE		HotBillingTag	: 1;
	BYTE		CRC_Way			: 2;
	BYTE		ChargeLevel		: 1;
	BYTE		Comm_reserved_bit	: 1;
	BYTE		Comm_reserved;
	//  À©Õ¹Çø:
	BYTE		servedIMSI[MAX_IMSI_LEN];
	BYTE		servedIMEI[MAX_IMEI_LEN];
	BYTE		servedMSISDN_npi	:4;
	BYTE		servedMSISDN_nai	:3;
	BYTE		s1					:1;
	BYTE		servedMSISDN_BCDlen;
	BYTE		servedMSISDN[MAX_NUM_LEN/4];
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];     
	
	BYTE		callingNumber_npi	:4;
	BYTE		callingNumber_nai	:3;
	BYTE		s2					:1;
	BYTE		callingNumber_BCDlen;
	BYTE		callingNumber[MAX_NUM_LEN/2];
	
	BYTE		calledNumber_npi	:4;
	BYTE		calledmber_nai		:3;
	BYTE		s3					:1;
	BYTE		calledNumber_BCDlen;
	BYTE		calledNumber[MAX_NUM_LEN/2];
	
	BYTE		translatedNumber_npi	:4;
	BYTE		translatedNumber_nai	:3;
	BYTE		s4						:1;
	BYTE		translatedNumberTYPElen;
	BYTE		translatedNumber[MAX_NUM_LEN/2];

	BYTE		connectedNumber_npi		:4;
	BYTE		connectedNumber_nai		:3;
	BYTE		s5						:1;
	BYTE		connectedNumber_BCDlen;
	BYTE		connectedNumber[MAX_NUM_LEN/2];

	BYTE		roamingNumber_npi	:4;
	BYTE		roamingNumber_nai	:3;
	BYTE		s6					:1;
	BYTE		roamingNumber_BCDlen;
	BYTE		roamingNumber[MAX_NUM_LEN/2];

	BYTE		recordingEntity_npi		:4;
	BYTE		recordingEntity_nai		:3;
	BYTE		s7						:1;
	BYTE		recordingEntity_BCDlen;
	BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];
	
	BYTE		mscIncomingROUTE_ind	:4;
	BYTE		mscOutgoingROUT_ind		:4;
	UINT2		mscIncomingROUTE_ROUTENumber;
	UINT2		mscOutgoingROUTE_ROUTENumber;
	BYTE		mscIncomingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];
	BYTE		mscOutgoingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];
	BYTE		caller_location_LAI[MAX_LOCAL_LAI_LEN/2];
	BYTE		caller_location_cellID[MAX_LOCAL_CI_LEN/2];
    BYTE		caller_changeOflocation_LAI[MAX_LOCAL_LAI_LEN/2];
    BYTE		caller_changeOflocation_cellID[MAX_LOCAL_CI_LEN/2];
	
	BYTE	    called_location_LAI[MAX_LOCAL_LAI_LEN/2];
    BYTE		called_location_cellID[MAX_LOCAL_CI_LEN/2]; 
    BYTE		called_changeOflocation_LAI[MAX_LOCAL_LAI_LEN/2];
    BYTE		called_changeOflocation_cellID[MAX_LOCAL_CI_LEN/2];

    BYTE		TBS:4;//TBS:4;        //  ±íÃ÷¸Ã´ÎÒµÎñÎªµç»°ÒµÎñ£¨Îª¡°0¡±£©»ò³ÐÔØÒµÎñ£¨Îª¡°1¡±£©    
	BYTE		ROAM_mark       :4;    
    BYTE		ServiceCode;//tele_bearer_code;  //  ÒÀ¾ÝTBSÌîµç»°ÒµÎñÂë»ò³ÐÒµÎñÂë¡£

    BYTE		transparencyIndicator:2;  //transparency_indicator :2;  // Í¸Ã÷·ÇÍ¸Ã÷Ö¸Ê¾   
	BYTE		s9         :6;

    struct MBILL_SS_code1     SS_code1;          
    struct MBILL_SS_code2     SS_code2;          
    struct MBILL_SS_code3     SS_code3;          
    struct MBILL_SS_code4     SS_code4;          
    struct MBILL_SS_code5     SS_code5;

	AOC_E1_E7	aocParameters;

	BYTE		MSClassmark[3];// ¼´Ô­À´µÄstruct  Classmark initial_served_ms_classmark£¬±»¼Æ·ÑÒÆ¶¯ÓÃ»§³õÊ¼CLASSMARK£¬struct  Classmark²»·ûºÏÐ­Òé¸ÄÎªBYTE
	BYTE		changeOfClassmark[3];//struct  Classmark last_served_ms_classmark;  //±»¼Æ·ÑÒÆ¶¯ÓÃ»§¸Ä±äºóµÄCLASSMARK

	BYTE		initial_year;           // Í¨»°³õÊ¼Ê±¿Ì
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            
    BYTE		initial_minute;
    BYTE		initial_second;

	BYTE		end_year; // Í¨»°½áÊøÊ±¼ä
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

	UINT4		callDuration;//conversation_time;                 // Í¨»°Ê±³¤
	
    BYTE		radioChanRequested:4;//USER_Radio_channelUINT2ed :4;                //   È«ËÙÂÊ»ò°ëËÙÂÊ 
    BYTE		radioChanUsed:2; 	
	BYTE		s10           :2; 
	   
	BYTE		causeForTerm;//cause_for_termination;        //  Í¨»°ÖÕÖ¹Ô­Òò
	
	SDiagnostics     diagnostics;//uDiagnostics;
	BYTE		call_reference_len;
	BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; ÓÃÓÚÍ¬Ò»ÒÆ¶¯Ì¨·¢ÉúµÄ²»Í¬»°ÎñµÄ±¾µØÊ¶±ðºÅ

    BYTE		chargeIndicator:4;//free_indicator :4;  // ¼Æ·ÑÃâ·Ñ±êÖ¾    
  	BYTE		s11          :4;

    
    //(MOCallRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ·ÑÂÊÖ¸Ê¾
    UINT2		money_per_count; // ·ÑÂÊ
    UINT2		add_fee;
	
	BYTE		chargedParty;

    BYTE		CAMEL_GsmSCF_Address_npi : 4;
    BYTE		CAMEL_GsmSCF_Address_nai :  3;
	BYTE		s12            : 1;    
	
    BYTE		CAMEL_GsmSCF_Address_BCDlen;
    BYTE		CAMEL_GsmSCF_Address[MAX_SCF_LEN]; 

	UINT4		ServiceKey;                //ÒµÎñ¼ü

	BYTE		NetworkCallReferenceLen;
    BYTE		NetworkcallReferenceNumber[maxCallReferenceNumberLength];

	//(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mSCAddress (¸úNetworkCallReferenceLenÅäºÏÊ¹ÓÃ)
    BYTE		MSCAddress_npi : 4;
    BYTE		MSCAddress_nai : 3;
    BYTE		s13            : 1;   
    
    BYTE		MSCAddress_BCDlen;
    BYTE		MSCAddress[MAX_MSC_NUM_IN_BYTES];

	BYTE		CAMELInitCFIndicator:1;
	BYTE		defaultCallHandling:2;
	BYTE		s14            : 5;
	
	BYTE		fnur;//ucFunr;           //Êý¾ÝÒµÎñÓÃ»§ËÙÂÊ,enum FNUR
	BYTE		aiurRequested;
	BYTE		speechVersionSupported:4;	
	BYTE		speechVersionUsed:4;
	BYTE		numberOfDPEncountered;
	BYTE		levelOfCAMELService;
	
    BYTE		CAMELDestinationNumber_npi : 4;
	BYTE		CAMELDestinationNumber_nai :  3;
	BYTE		s15            : 1;   
    BYTE		CAMELDestinationNumber_BCDlen ;
    BYTE		CAMELDestinationNumber[MAX_NUM_LEN/2]; 

	MBILL_CHANGEFLAGS     CAMELModificationChangeFlags;
	
	BYTE		CAMEL_GsmSCF_Address2_npi : 4;
	BYTE		CAMEL_GsmSCF_Address2_nai :  3;
	BYTE		s16            : 1;

	BYTE		CAMEL_GsmSCF_Address2_BCDlen;
	BYTE		CAMEL_GsmSCF_Address2[MAX_SCF_LEN]; 
	
    //(MOCallRecord)(TermCAMELRecord)serviceKey-2
    UINT4		ServiceKey2;                //ÒµÎñ¼ü
	
	BYTE		freeFormatDataAppend2:2;
	BYTE		freeFormatDataAppend:2;
	BYTE		defaultCallHandling2:2 ;
	BYTE		systemType:2;
	BYTE		rateIndication;

    BYTE		originalCalledNumber_npi  :4;                 // ºÅÂë¼Æ»®
	BYTE		originalCalledNumber_nai  :3; 
	BYTE		s18               :1;    
                // ºÅÂëÀàÐÍ
    BYTE		originalCalledNumber_BCDlen;                 // ºÅÂë³¤¶È
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

	BYTE		callingChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];

	BYTE		calledChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];
	
	UINT2		mscOutgoingCircuit;
    UINT2		mscIncomingCircuit;
	
    BYTE		rnc_bsc_id_len:3;
    BYTE		msc_id_len:3;//orgMSCId³¤¶ÈÖ¸Ê¾
    BYTE		s23            : 2;


	BYTE		orgRNCorBSCId[MAX_RNCBSC_ID_LEN];

	BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];
	BYTE		callEmlppPriority:4;
	BYTE		callerDefaultEmlppPriority:4;
	BYTE		eaSubscriberInfo[MAX_NAEA_CODE_IN_BYTE + 1];
	BYTE		selectedCIC[MAX_CIC_LEN_IN_BYTE];
	BYTE		portedflag;
	BYTE		subscriberCategory;
	
	BYTE		CUGOutgoingAccessIndicator:2;
	BYTE		CUGOutgoingAccessUsed:2;
	BYTE		s19:4;
	
	UINT4		CUGInterlockCode;
	UINT2		CUGIndex;
	
	BYTE	    CFW_invoked_times:4;    // Ç°×ª·¢ÉúµÄ´ÎÊý    
	BYTE		CH_invoked_times:4;        // ºô½Ð±£³ÖÒµÎñ·¢ÉúµÄ´ÎÊý
    BYTE		CH_retrieve_times:4;       // ºô½Ð±£³ÖÒµÎñ»Ö¸´µÄ´ÎÊý    
    BYTE		CW_invoked_times:4;        // ºô½ÐµÈ´ýÒµÎñ·¢ÉúµÄ´ÎÊý
    BYTE		MPTY_Build_times:4;        // ¶à·½Í¨»°ÒµÎñ½¨Á¢·¢ÉúµÄ´ÎÊý    
    BYTE		MPTY_CH_times:4;           // ¶à·½Í¨»°ÒµÎñ±£³Ö·¢ÉúµÄ´ÎÊý
    BYTE		MPTY_CH_retrieve_times:4;   // ¶à·½Í¨»°ÒµÎñ±£³Ö»Ö¸´·¢ÉúµÄ´ÎÊý   
    BYTE		MPTY_Split_times:4;        // ¶à·½Í¨»°ÒµÎñ¸ôÀë·¢ÉúµÄ´ÎÊý
	
	BYTE		HO_invoked_times;         // ÇÐ»»·¢ÉúµÄ´ÎÊý
	BYTE		AOCParmsChangedTimes:4; 
	BYTE		ClassmarkChangedTimes:4;
    BYTE		RadioChanChangedTimes:4;	
	BYTE		transmission_mode:4;   // 2G»°µ¥Ê¹ÓÃµÄ  È«ËÙÂÊ»ò°ëËÙÂÊ ±êÖ¾¡£

	BYTE		Info_trans_capability;             // ³ÐÔØÄÜÁ¦
    BillClassmark3 Classmark3Information;    
	
	BYTE		DTMF_indicator :2;  // ÊÇ·ñÊ¹ÓÃDTMF
	BYTE		user_type :5;  //   ÒÆ¶¯Ì¨ÓÅÏÈ¼¶  
	BYTE		s8 :1;  //   0 ±íÊ¾±¾µØÓÃ»§£¬1 ±íÊ¾ÂþÓÎÓÃ»§¡
	
  
    BYTE		EmergencyCallFlag     :1;          // ½ô¼±ºô½Ð±êÖ   
    BYTE		PromptAndCollectCharge :1;               //×ÊÔ´¼Æ·Ñ»°µ¥±êÖ¾ÒÔÇ°µÄs6±êÖ¾
    BYTE		SORIndication:1;
 	BYTE		s21:1;
	BYTE		EarlyForwardSORIndication	:1;
	BYTE		LateForwardSORIndication	:1; 	
    BYTE		s20                   :2;
    // BYTE		UssdCallBackIndication:1; 
    
    BYTE		ModemType:7;
	BYTE		Classmark3Indication:1;   
    
    BYTE		cause_for_partial_record;//cause_for_partial_record ;      //  ³öÖÐ¼ä»°µ¥Ô­Òò
    BYTE		transaction_id;// ÊÂÎñID,ÓÃÓÚÍ¬Ò»ÒÆ¶¯Ì¨·¢ÉúµÄ²»Í¬»°ÎñµÄ±¾µØÊ¶±ðºÅ
    BYTE		gsm_gsvn;                          // 1ÒµÎñÀà±ð
    BYTE		B_ch_number;       // ¸Ã´Îºô½ÐËùÕ¼ÓÃµÄBÐÅµÀÊý
	
    BYTE		scfID[MAX_SCFID_LEN]; 

    BYTE		served_msc_num_npi : 4;
    BYTE		served_msc_num_nai :  3;
	BYTE		s22            : 1;
    BYTE		served_msc_num_BCDlen ;
	BYTE		served_msc_num[MAX_MSC_NUM_IN_BYTES];  
	
	BYTE		GuaranteedBitRate;  //±£Ö¤±ÈÌØÂÊ
	BYTE		MaximumBitRate;     //×î´ó±ÈÌØÂÊ

    //ÐÂÔö×Ö¶Î
    BYTE        ip_route_number_npi  :4;
    BYTE        ip_route_number_nai  :3;
    BYTE        S111                 :1;    
    BYTE        ip_route_number[MAX_NUM_LEN/2 - 1];
    BYTE        resource_charge_initial_year;
    BYTE        resource_charge_initial_month;
    BYTE        resource_charge_initial_date;
    BYTE        resource_charge_initial_hour;
    BYTE        resource_charge_initial_minute;
    BYTE        resource_charge_initial_second;
    UINT4       resource_charge_conversation_time;

    BYTE        calledIMSI[MAX_IMSI_LEN];
        
	BYTE		container[MAX_CONTAINER_MOC_LEN];

	//¿É±ä²¿·Ö
	
	BYTE		fcidata_len;
    BYTE		fcidata2_len;
    BYTE     VarPart[MAX_TRANSFER_FCI_LEN * 2];
   
	/*
    VarPartÓÉÒÔÏÂÁ½²¿·Ö×é³É
    BYTE FCIData[MAX_TRANSFER_FCI_LEN];
    BYTE FCIData2[MAX_TRANSFER_FCI_LEN];
    */

};

//1.2	MTC_Bill_Report£¨°üÀ¨MTC(mtCallRecord)£©
struct MTC_Bill_Report
{
	// ¹«¹²Óò²¿·Ö, »ù±¾Çø:
	BYTE		validflag; // ÓÐÐ§ÐÔ±êÖ¾,¡¡¡°AA¡±ÎªÓÐÐ§£¬¡°55¡±ÎªÎÞÐ§
    UINT2		size; // ±¾ÕÅ»°µ¥³¤¶È
    UINT2		CRCCode; // CRCÐ£Ñé½á¹û
    BYTE		bill_type; //  enum   GSM_BILL_TYPE, Ð­ÒéÖÐµÄrecordTypeÓò
    UINT4		bill_sequence; // »°µ¥Á÷Ë®ºÅ£¬Î¨Ò»Çø·ÖÒ»ÕÅ»°µ¥¡£
    BYTE		ModuleNum; // ²úÉú»°µ¥µÄÄ£¿éºÅ
    UINT4		count; // GSM ¼Æ·Ñµ±Ò»´Îºô½Ð²úÉú¶àÕÅ»°µ¥Ê±count ÊÇ¸Ã´Îºô½ÐÊ×»°µ¥Á÷Ë®ºÅ
    BYTE		sequence; // µ¥ÕÅ»°µ¥Îª0£¬Ê×ÕÅ»°µ¥Îª1£¬ÆäÓàÀàÍÆ
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, µ¥ÕÅÊ×ÕÅÖÐ¼äÄ©ÕÅ»°µ¥±êÊ¶
    BYTE		HotBillingTag:1; //Îªcmcc²âÊÔÔö¼ÓµÄÈÈ¼Æ·Ñ±êÖ¾
    BYTE		CRC_Way:2; // CRCÐ£Ñé·½Ê½
    BYTE		ChargeLevel:1;//¼Æ·Ñ¾«¶ÈÖ¸Ê¾:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
    BYTE		Comm_reserved;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
	
	//  À©Õ¹Çø:
	
	BYTE		servedIMSI[MAX_IMSI_LEN];  //  ±»¼Æ·ÑÒÆ¶¯ÓÃ»§IMSIÂë

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  ±»¼Æ·ÑÒÆ¶¯ÓÃ»§ÒÆ¶¯Ì¨Éè±¸Ê¶±ðÂë

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;  
    BYTE		servedMSISDN_nai :  3;
    BYTE		s1            : 1;
    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // ±»¼Æ·ÑÒÆ¶¯ÓÃ»§MSISDNºÅ
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
        
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callingNumber
    BYTE		callingNumber_npi  :4;                 // Ö÷½ÐºÅÂë¼Æ»®
    BYTE		callingNumber_nai  :3;                 // Ö÷½ÐºÅÂëÀàÐÍ    
    BYTE		s2               :1;

    BYTE		callingNumber_BCDlen ;                 // Ö÷½ÐºÅÂë³¤¶È
    BYTE		callingNumber[MAX_NUM_LEN/2];     // Ö÷½ÐºÅÂë

    //(MOCallRecord)(TermCAMELRecord)calledNumber
    BYTE		calledNumber_npi  :4;//dialed_isdn_npi  :4;                 // ±»½ÐºÅÂë¼Æ»®
    BYTE		calledNumber_nai  :3;//dialed_isdn_nai  :3;                 // ±»½ÐºÅÂëÀàÐÍ
    BYTE		s3               :1;
   
    BYTE		calledNumber_BCDlen ;//dialed_isdn_num  :5;                 // ±»½ÐºÅÂë³¤¶È
    BYTE		calledNumber[MAX_NUM_LEN/2];  //dialed_num[MAX_NUM_LEN/2];     // ±»½ÐºÅÂë

    //(MOCallRecord)(MTCallRecord)connectedNumber
    BYTE		connectedNumber_npi :4;               //µÚÈý·½ºÅÂë¼Æ»®
    BYTE		connectedNumber_nai :3;               //µÚÈý·½ºÅÂëÀàÐÍ   
    BYTE		s4                 :1;               //unuseful    

    BYTE		connectedNumber_BCDlen;               //µÚÈý·½ºÅÂë³¤¶È
    BYTE		connectedNumber[MAX_NUM_LEN/2];  // µÚÈý·½ºÅÂë

    //(MOCallRecord)(MTCallRecord)(roamingRecord)roamingNumber
    BYTE		roamingNumber_npi : 4;
    BYTE		roamingNumber_nai :  3;    
    BYTE		s5            : 1;    
    BYTE		roamingNumber_BCDlen ;
    BYTE		roamingNumber[MAX_NUM_LEN/2] ;    //MSRN               //  ÒÆ¶¯±»½ÐÂþÓÎºÅÂë

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)originalCalledNumber Ä¿Ç°Ã»ÓÐÌîÐ´£¬Ô¤Áô
    BYTE		originalCalledNumber_npi  :4;                 // ºÅÂë¼Æ»®
    BYTE		originalCalledNumber_nai  :3;                 // ºÅÂëÀàÐÍ
    BYTE		s6               :1;    
    BYTE		originalCalledNumber_BCDlen;                 // ºÅÂë³¤¶È
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

    //(MTCallRecord)redirectingnumber Ä¿Ç°Ã»ÓÐÌîÐ´£¬Ô¤Áô
    BYTE		redirectingnumber_npi  :4;                 // ºÅÂë¼Æ»®
    BYTE		redirectingnumber_nai  :3;                 // ºÅÂëÀàÐÍ   
    BYTE		s7               :1;    
    BYTE		redirectingnumber_BCDlen;                 // ºÅÂë³¤¶È
    BYTE		redirectingnumber[MAX_NUM_LEN/2]; 

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;  
    BYTE		s8            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  ±¾¾ÖMSCºÅ

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mscIncomingROUTE/mscOutgoingROUTE
    BYTE		mscIncomingROUTE_ind : 4;//IT_trunk_group_ind : 4;       
    BYTE		mscOutgoingROUTE_ind : 4;//OT_trunk_group_ind : 4; 
    UINT2		mscIncomingROUTE_ROUTENumber;//IT_trunk_group;                     // ÈëÖÐ¼ÌÈº
    UINT2		mscOutgoingROUTE_ROUTENumber;//OT_trunk_group;                    // ³öÖÐ¼ÌÈº
    BYTE		mscIncomingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//IT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];
    BYTE		mscOutgoingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//OT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];

    //(MTCallRecord)location/changeOfLocation 
    BYTE		caller_location_LAI[MAX_LOCAL_LAI_LEN/2];//caller_org_LAC[MAX_LOCAL_LAC_LEN/2];  //  ±»¼Æ·ÑÒÆ¶¯ÓÃ»§Ëù´¦³õÊ¼Î»ÖÃÇøºÅ
    BYTE		caller_location_cellID[MAX_LOCAL_CI_LEN/2];//caller_org_cell_ID[MAX_LOCAL_CI_LEN/2];   // ±»¼Æ·ÑÒÆ¶¯ÓÃ»§Ëù´¦µÄ³õÊ¼Ð¡ÇøºÅ
    BYTE		caller_changeOflocation_LAI[MAX_LOCAL_LAI_LEN/2]; //caller_LAC[MAX_LOCAL_LAC_LEN/2];     // ±»¼Æ·ÑÒÆ¶¯ÓÃ»§Ëù´¦Î»ÖÃÇøºÅ
    BYTE		caller_changeOflocation_cellID[MAX_LOCAL_CI_LEN/2];//caller_cell_ID[MAX_LOCAL_CI_LEN/2];  // ±»¼Æ·ÑÒÆ¶¯ÓÃ»§Ëù´¦µÄÐ¡ÇøºÅ

    BYTE		called_location_LAI[MAX_LOCAL_LAI_LEN/2];//called_org_LAC[MAX_LOCAL_LAC_LEN/2];    // ±»½ÐÓÃ»§Ëù´¦³õÊ¼Î»ÖÃÇøºÅ
    BYTE		called_location_cellID[MAX_LOCAL_CI_LEN/2]; //called_org_cell_ID[MAX_LOCAL_CI_LEN/2];    // ±»½ÐÓÃ»§Ëù´¦µÄ³õÊ¼Ð¡ÇøºÅ
    BYTE		called_changeOflocation_LAI[MAX_LOCAL_LAI_LEN/2];//called_LAC[MAX_LOCAL_LAC_LEN/2];          // ±»½ÐÓÃ»§Ëù´¦Î»ÖÃÇøºÅ
    BYTE		called_changeOflocation_cellID[MAX_LOCAL_CI_LEN/2]; //called_cell_ID[MAX_LOCAL_CI_LEN/2];       // ±»½ÐÓÃ»§Ëù´¦µÄÐ¡ÇøºÅ

    //(MOCallRecord)(MTCallRecord)(roamingRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  ±íÃ÷¸Ã´ÎÒµÎñÎªµç»°ÒµÎñ£¨Îª¡°0¡±£©»ò³ÐÔØÒµÎñ£¨Îª¡°1¡±£©
    BYTE		ROAM_mark        :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  ÒÀ¾ÝTBSÌîµç»°ÒµÎñÂë»ò³ÐÒµÎñÂë¡£
    
    //(MOCallRecord)(MTCallRecord)(roamingRecord)transparencyIndicator
    BYTE		transparencyIndicator:2;  //transparency_indicator :2;  // Í¸Ã÷·ÇÍ¸Ã÷Ö¸Ê¾    
    BYTE		s10          :6;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)changeOfService ÓÃÊÂ¼þ»°µ¥»òÖÐ¼ä»°µ¥µÄ·½Ê½³ö

    //(MOCallRecord)(MTCallRecord)(roamingRecord)supplServicesUsed(ºô½ÐÎÞ¹ØµÄ²¹³äÒµÎñÓÃÊÂ¼þ»°µ¥·½Ê½³ö,BAUºÏ²¢)
    struct MBILL_SS_code1     SS_code1;          
    struct MBILL_SS_code2     SS_code2;          
    struct MBILL_SS_code3     SS_code3;          
    struct MBILL_SS_code4     SS_code4;          
    struct MBILL_SS_code5     SS_code5;

    //(MOCallRecord)(MTCallRecord)aocParameters/changeOfAOCParms
    AOC_E1_E7    aocParameters;  

    //(MOCallRecord)(MTCallRecord)MSClassmark[3]
    BYTE  MSClassmark[3];// ¼´Ô­À´µÄstruct  Classmark initial_served_ms_classmark£¬±»¼Æ·ÑÒÆ¶¯ÓÃ»§³õÊ¼CLASSMARK£¬struct  Classmark²»·ûºÏÐ­Òé¸ÄÎªBYTE

    //(MOCallRecord)(MTCallRecord)changeOfClassmark[3]  
    BYTE   changeOfClassmark[3];//struct  Classmark last_served_ms_classmark;  //±»¼Æ·ÑÒÆ¶¯ÓÃ»§¸Ä±äºóµÄCLASSMARK

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)seizureTime 
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)answerTime
    BYTE		initial_year;           // Í¨»°³õÊ¼Ê±¿Ì
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            
    BYTE		initial_minute;
    BYTE		initial_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)releaseTime
    BYTE		end_year; // Í¨»°½áÊøÊ±¼ä
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callDuration
    UINT4		callDuration;//conversation_time;                 // Í¨»°Ê±³¤
        
    //(MOCallRecord)(MTCallRecord)radioChanUsed 
    //(MOCallRecord)(MTCallRecord)radioChanRequested  
    BYTE		radioChanRequested:4;//USER_Radio_channelUINT2ed :4;                // 1  È«ËÙÂÊ»ò°ëËÙÂÊ    

    BYTE		radioChanUsed:2; 

    BYTE		s11    :2;    

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)causeForTerm
    BYTE		causeForTerm;//cause_for_termination;        //  Í¨»°ÖÕÖ¹Ô­Òò

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)SDiagnostics
    SDiagnostics     diagnostics;//uDiagnostics;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; ÓÃÓÚÍ¬Ò»ÒÆ¶¯Ì¨·¢ÉúµÄ²»Í¬»°ÎñµÄ±¾µØÊ¶±ðºÅ

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)sequenceNumber: ÖÐ¼ä»°µ¥ÐòÁÐºÅ,·ÅÔÚ¹«¹²Óò²¿·Ö, »ù±¾Çø:sequenceÓò

    //(MOCallRecord)(MTCallRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // ¼Æ·ÑÃâ·Ñ±êÖ¾    
    BYTE		s12         :4;

    //(MOCallRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ·ÑÂÊÖ¸Ê¾
    UINT2		money_per_count; // ·ÑÂÊ
    UINT2		add_fee;                 //¸½¼Ó·Ñ

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)chargedParty Ä¿Ç°Ã»ÓÐÌîÐ´£¬ÏÈÔ¤Áô±ÈÌØÎ»
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)networkCallReference
    //µ±Ê¹ÓÃCAMELÒµÎñÊ±,Í¬Ò»»°ÎñËù¾­µÄ¸÷ÍøÔª²úÉúµÄ»°µ¥½«º¬ÓÐ´Ë±êÊ¶Âë(NCR),ÓÃÒÔ°ÑÕâÐ©»°µ¥¹ØÁªµ½Ò»Æð. 
    BYTE		NetworkCallReferenceLen;
    BYTE		NetworkcallReferenceNumber[maxCallReferenceNumberLength]; // ÓÉNetworkCallReferenceLenÖ¸Ã÷³¤¶È(2G¾ÉÓÐµÄºô½Ð²Î¿¼ºÅ)

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mSCAddress (¸úNetworkCallReferenceLenÅäºÏÊ¹ÓÃ)
    BYTE		MSCAddress_npi : 4;
    BYTE		MSCAddress_nai :  3;
    BYTE		s13            : 1;    
    BYTE		MSCAddress_BCDlen;
    BYTE		MSCAddress[MAX_MSC_NUM_IN_BYTES];

    //(MOCallRecord)(MTCallRecord)fnur
    BYTE		fnur;//ucFunr;           //Êý¾ÝÒµÎñÓÃ»§ËÙÂÊ,enum FNUR

    //(MOCallRecord)(MTCallRecord)aiurRequested
    BYTE		aiurRequested;//BYTE SMSResult;(B03ÖÐÓòÃûÊ¹ÓÃµÄÊÇSMSResultµ«º¬ÒåÊÇaiurRequested£¬R02¸ÄÃû)

    //(MOCallRecord)(MTCallRecord)speechVersionUsed
    BYTE		speechVersionUsed:4;//USER_speech_versionUINT2ed :4;

    //(MOCallRecord)(MTCallRecord)speechVersionSupported
    BYTE		speechVersionSupported:4;//USER_speech_version_supported :4;    

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)gsm-SCFAddress
    BYTE		CAMEL_GsmSCF_Address_npi : 4;    
    BYTE		CAMEL_GsmSCF_Address_nai :  3;
    BYTE		s14            : 1;    
    BYTE		CAMEL_GsmSCF_Address_BCDlen;
    BYTE		CAMEL_GsmSCF_Address[MAX_SCF_LEN]; 

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)serviceKey
    UINT4		ServiceKey;                //ÒµÎñ¼ü

    //(MOCallRecord)(MTCallRecord)rateIndication
    BYTE		rateIndication;//ucRateIndication; //Êý¾ÝÒµÎñËÙÂÊÊÊÅä

     //(MOCallRecord)(MTCallRecord)systemType
    BYTE		systemType:2;//USER_NETWORK_TYPE :4; 

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)freeFormatDataAppend
    BYTE		freeFormatDataAppend:2;//CAMEL_FCI_Data_Append_Ind :2; //±íÊ¾ÊÇµÚÒ»¸öFCIÏûÏ¢£¬0±íÊ¾²»ÊÇµÚÒ»¸ö

      //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)defaultCallHandling
    BYTE		defaultCallHandling:2;//CAMEL_default_Call_Handling;   
    BYTE		s15       :2;   
   

    //(MOCallRecord)(MTCallRecord)callingChargeAreaCode Ä¿Ç°Ã»ÓÐÌîÐ´£¬Ô¤Áô//Ì©¹úÌØÊâÓò    BYTE    CAC_num[MAX_CAC_LEN/2+1];
    BYTE		callingChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];

    //(MOCallRecord)(MTCallRecord)calledChargeAreaCode Ä¿Ç°Ã»ÓÐÌîÐ´£¬Ô¤Áô//Ì©¹úÌØÊâÓò    BYTE    CAC_num[MAX_CAC_LEN/2+1];
    BYTE		calledChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)numberOfDPEncountered
    BYTE		numberOfDPEncountered;//CAMEL_num_of_DP;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)levelOfCAMELService
    BYTE		levelOfCAMELService;//CAMEL_service_level;

    //(MTCallRecord)(roamingRecord)mscIncomingCircuit
    UINT2		mscOutgoingCircuit;
    UINT2		mscIncomingCircuit;

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId³¤¶ÈÖ¸Ê¾
    BYTE		msc_id_len:3;//orgMSCId³¤¶ÈÖ¸Ê¾
    BYTE		s23            : 2;

    //(MOCallRecord)(MTCallRecord)orgRNCorBSCId
    BYTE		orgRNCorBSCId[MAX_RNCBSC_ID_LEN];//Rnc_Bsc_Id[3];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(MOCallRecord)(MTCallRecord)callEmlppPriority
    BYTE		callEmlppPriority:4;//CallEmlpp;//ºô½Ð×îÖÕEMLPPÓÅÏÈ¼¶ £¨È¡Öµ0-7)

    //(MTCallRecord)calledDefaultEmlppPriority
    BYTE		calledDefaultEmlppPriority:4;//calledDefaultEmlpp;//±»½ÐÓÃ»§Ç©Ô¼Ä¬ÈÏEMLPPÓÅÏÈ¼¶£¨È¡Öµ0-7)

    //(MOCallRecord)(MTCallRecord)eaSubscriberInfo 
    BYTE		eaSubscriberInfo[MAX_NAEA_CODE_IN_BYTE + 1];
    
    //(MOCallRecord)(MTCallRecord)selectedCIC 
    BYTE		selectedCIC[MAX_CIC_LEN_IN_BYTE];
    
    //(MOCallRecord)(MTCallRecord)portedflag 
    BYTE		portedflag;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;

     //(MOCallRecord)(MTCallRecord)(roamingRecord)cUGOutgoingAccessIndicator
    BYTE		CUGOutgoingAccessIndicator:2;//CUG_call_ind         :2; 

    //(MOCallRecord)(MTCallRecord)cUGOutgoingAccessUsed
    BYTE		CUGOutgoingAccessUsed:2;//CUGOutgoingAccessUsed   :2;    
    BYTE		s16        :4;    

    //(MOCallRecord)(MTCallRecord)(roamingRecord)cUGInterlockCode
    UINT4		CUGInterlockCode;//CUGInterlockCode;

    //(MOCallRecord)(MTCallRecord)cUGIndex
    UINT2		CUGIndex;

    //(MOCallRecord)(MTCallRecord)hotBillingTag·ÅÔÚ»ù±¾ÇøHotBillingTag

    //(MTCallRecord)redirectingcounter 
    BYTE		CFW_invoked_times:4;       // Ç°×ª·¢ÉúµÄ´ÎÊý    

    //(MOCallRecord)(MTCallRecord)recordExtensions
    BYTE		CH_invoked_times:4;        // ºô½Ð±£³ÖÒµÎñ·¢ÉúµÄ´ÎÊý
    BYTE		CH_retrieve_times:4;       // ºô½Ð±£³ÖÒµÎñ»Ö¸´µÄ´ÎÊý    
    BYTE		CW_invoked_times:4;        // ºô½ÐµÈ´ýÒµÎñ·¢ÉúµÄ´ÎÊý
    BYTE		MPTY_Build_times:4;        // ¶à·½Í¨»°ÒµÎñ½¨Á¢·¢ÉúµÄ´ÎÊý    
    BYTE		MPTY_CH_times:4;           // ¶à·½Í¨»°ÒµÎñ±£³Ö·¢ÉúµÄ´ÎÊý
    BYTE		MPTY_CH_retrieve_times:4;   // ¶à·½Í¨»°ÒµÎñ±£³Ö»Ö¸´·¢ÉúµÄ´ÎÊý
    BYTE		MPTY_Split_times:4;        // ¶à·½Í¨»°ÒµÎñ¸ôÀë·¢ÉúµÄ´ÎÊý

    BYTE		HO_invoked_times;         // ÇÐ»»·¢ÉúµÄ´ÎÊý
    
    //BYTE     ServiceChangedTimes:4;

    BYTE		AOCParmsChangedTimes:4;    
    BYTE		ClassmarkChangedTimes:4;    
    BYTE		RadioChanChangedTimes:4;
    BYTE		transmission_mode:4;   // 2G»°µ¥Ê¹ÓÃµÄ  È«ËÙÂÊ»ò°ëËÙÂÊ ±êÖ¾¡£

    BYTE		Info_trans_capability;             // ³ÐÔØÄÜÁ¦

    struct  BillClassmark3 Classmark3Information;    

    BYTE		DTMF_indicator :2;  // ÊÇ·ñÊ¹ÓÃDTMF
    BYTE		user_type :5;  //   ÒÆ¶¯Ì¨ÓÅÏÈ¼¶    
    BYTE		s9 :1;  //   0 ±íÊ¾±¾µØÓÃ»§£¬1 ±íÊ¾ÂþÓÎÓÃ»§¡£
    BYTE		initialCallAttemptFlag :1;	
    BYTE		reserved :1; 
    BYTE		SORIndication:1;    
    BYTE		UssdCallBackIndication:1;// Ö»ÔÚCAMEL MTC»°µ¥ÖÐ²Å¼ÇÂ¼
    BYTE		ucTcsiVtcsiFlag : 4;//0ÎªTCSI£¬1ÎªVTCSI,0xffÎÞÐ§ 
	
    //Îª×Ô¶¯²¨ÌØÂÊAutobauding SupportÐÂÔö
    BYTE		ModemType:7;    
    BYTE		Classmark3Indication:1;
    
    BYTE		cause_for_partial_record;//cause_for_partial_record ;      //  ³öÖÐ¼ä»°µ¥Ô­Òò    
    BYTE		transaction_id;// ÊÂÎñID,ÓÃÓÚÍ¬Ò»ÒÆ¶¯Ì¨·¢ÉúµÄ²»Í¬»°ÎñµÄ±¾µØÊ¶±ðºÅ
    BYTE		gsm_gsvn;                          // 1ÒµÎñÀà±ð
    BYTE		B_ch_number;       // ¸Ã´Îºô½ÐËùÕ¼ÓÃµÄBÐÅµÀÊý

    // GSM ÌØÊâÓò
    BYTE		scfID[MAX_SCFID_LEN];      //SCFË÷ÒýºÅ

    //  µ±Ç°±»¼Æ·ÑÒÆ¶¯ÓÃ»§Ëù´¦MSC
    BYTE		served_msc_num_npi : 4;
    BYTE		served_msc_num_nai :  3;
    BYTE		s18            : 1;
    BYTE		served_msc_num_BCDlen ;
    BYTE		served_msc_num[MAX_MSC_NUM_IN_BYTES];       

    //Êý¾ÝÒµÎñ±ÈÌØÂÊ¶¨Òå
    BYTE     	GuaranteedBitRate;//±£Ö¤±ÈÌØÂÊ
    BYTE    	MaximumBitRate;//×î´ó±ÈÌØÂÊ
    
    //ÐÂÔöÈÝÆ÷ÓÃ×÷ºóÐøÀ©Õ¹
    BYTE		container[MAX_CONTAINER_MTC_LEN];

    // FCI ·ÅÔÚ¿É±ä²¿·Ö
    BYTE		fcidata_len;
    BYTE        VarPart[MAX_TRANSFER_FCI_LEN];
    BYTE		FCIData[MAX_TRANSFER_FCI_LEN];
	
};

//1.3	TCAMEL_Bill_Report£¨°üÀ¨TCAMEL £¨TermCAMELRecord£©£©
struct TCAMEL_Bill_Report
{
	// ¹«¹²Óò²¿·Ö, »ù±¾Çø:
	BYTE		validflag; // ÓÐÐ§ÐÔ±êÖ¾,¡¡¡°AA¡±ÎªÓÐÐ§£¬¡°55¡±ÎªÎÞÐ§
    UINT2		size; // ±¾ÕÅ»°µ¥³¤¶È
    UINT2		CRCCode; // CRCÐ£Ñé½á¹û
    BYTE		bill_type; //  enum   GSM_BILL_TYPE, Ð­ÒéÖÐµÄrecordTypeÓò
    UINT4		bill_sequence; // »°µ¥Á÷Ë®ºÅ£¬Î¨Ò»Çø·ÖÒ»ÕÅ»°µ¥¡£
    BYTE		ModuleNum; // ²úÉú»°µ¥µÄÄ£¿éºÅ
    UINT4		count; // GSM ¼Æ·Ñµ±Ò»´Îºô½Ð²úÉú¶àÕÅ»°µ¥Ê±count ÊÇ¸Ã´Îºô½ÐÊ×»°µ¥Á÷Ë®ºÅ
    BYTE		sequence; // µ¥ÕÅ»°µ¥Îª0£¬Ê×ÕÅ»°µ¥Îª1£¬ÆäÓàÀàÍÆ
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, µ¥ÕÅÊ×ÕÅÖÐ¼äÄ©ÕÅ»°µ¥±êÊ¶
    BYTE		HotBillingTag:1; //Îªcmcc²âÊÔÔö¼ÓµÄÈÈ¼Æ·Ñ±êÖ¾
    BYTE		CRC_Way:2; // CRCÐ£Ñé·½Ê½
    BYTE		ChargeLevel:1;//¼Æ·Ñ¾«¶ÈÖ¸Ê¾:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
    BYTE		Comm_reserved;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
	
	//  À©Õ¹Çø:

	BYTE		servedIMSI[MAX_IMSI_LEN];  //  ±»¼Æ·ÑÒÆ¶¯ÓÃ»§IMSIÂë

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  ±»¼Æ·ÑÒÆ¶¯ÓÃ»§ÒÆ¶¯Ì¨Éè±¸Ê¶±ðÂë

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // ±»¼Æ·ÑÒÆ¶¯ÓÃ»§MSISDNºÅ
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callingNumber
    BYTE		callingNumber_npi  :4;                 // Ö÷½ÐºÅÂë¼Æ»®
    BYTE		callingNumber_nai  :3;                 // Ö÷½ÐºÅÂëÀàÐÍ   
    BYTE		s2               :1;    
    BYTE		callingNumber_BCDlen ;                 // Ö÷½ÐºÅÂë³¤¶È
    BYTE		callingNumber[MAX_NUM_LEN/2];     // Ö÷½ÐºÅÂë

    //(MOCallRecord)(TermCAMELRecord)calledNumber
    BYTE		calledNumber_npi  :4;//dialed_isdn_npi  :4;                 // ±»½ÐºÅÂë¼Æ»®
    BYTE		calledNumber_nai  :3;//dialed_isdn_nai  :3;                 // ±»½ÐºÅÂëÀàÐÍ    
    BYTE		s3               :1;    
    BYTE		calledNumber_BCDlen ;//dialed_isdn_num  :5;                 // ±»½ÐºÅÂë³¤¶È
    BYTE		calledNumber[MAX_NUM_LEN/2];  //dialed_num[MAX_NUM_LEN/2];     // ±»½ÐºÅÂë

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;   
    BYTE		s4            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  ±¾¾ÖMSCºÅ

    //(TermCAMELRecord)interrogationTime
    BYTE		GMSC_query_hlr_year ;       //CAMEL T
    BYTE		GMSC_query_hlr_month ;
    BYTE		GMSC_query_hlr_date ;
    BYTE		GMSC_query_hlr_hour ;
    BYTE		GMSC_query_hlr_minute ;
    BYTE		GMSC_query_hlr_second ;

    //cAMELCallLegInformation:cAMELDestinationNumber 
    //(TermCAMELRecord)destinationRoutingAddress 
    BYTE		CAMELDestinationNumber_npi : 4;
    BYTE		CAMELDestinationNumber_nai :  3;    
    BYTE		s5            : 1;    
    BYTE		CAMELDestinationNumber_BCDlen;
    BYTE		CAMELDestinationNumber[MAX_NUM_LEN/2]; 
        
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)gsm-SCFAddress
    BYTE		CAMEL_GsmSCF_Address_npi : 4;
    BYTE		CAMEL_GsmSCF_Address_nai :  3;    
    BYTE		s6            : 1;    
    BYTE		CAMEL_GsmSCF_Address_BCDlen;
    BYTE		CAMEL_GsmSCF_Address[MAX_SCF_LEN]; 

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)serviceKey,
    UINT4		ServiceKey;                //ÒµÎñ¼ü

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)networkCallReference
    //µ±Ê¹ÓÃCAMELÒµÎñÊ±,Í¬Ò»»°ÎñËù¾­µÄ¸÷ÍøÔª²úÉúµÄ»°µ¥½«º¬ÓÐ´Ë±êÊ¶Âë(NCR),ÓÃÒÔ°ÑÕâÐ©»°µ¥¹ØÁªµ½Ò»Æð. 
    BYTE		NetworkCallReferenceLen;
    BYTE		NetworkcallReferenceNumber[maxCallReferenceNumberLength]; // ÓÉNetworkCallReferenceLenÖ¸Ã÷³¤¶È(2G¾ÉÓÐµÄºô½Ð²Î¿¼ºÅ)

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mSCAddress (¸úNetworkCallReferenceLenÅäºÏÊ¹ÓÃ)
    BYTE		MSCAddress_npi : 4;
    BYTE		MSCAddress_nai :  3;
    BYTE		s7            : 1;    
    BYTE		MSCAddress_BCDlen;
    BYTE		MSCAddress[MAX_MSC_NUM_IN_BYTES];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)defaultCallHandling
    BYTE		defaultCallHandling:2;//CAMEL_default_Call_Handling;    
    BYTE		s8            :6;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mscIncomingROUTE/mscOutgoingROUTE
    BYTE		mscIncomingROUTE_ind : 4;//IT_trunk_group_ind : 4;    
    BYTE		mscOutgoingROUTE_ind : 4;//OT_trunk_group_ind : 4;    
    UINT2		mscIncomingROUTE_ROUTENumber;//IT_trunk_group;                     // ÈëÖÐ¼ÌÈº
    UINT2		mscOutgoingROUTE_ROUTENumber;//OT_trunk_group;                    // ³öÖÐ¼ÌÈº
    BYTE		mscIncomingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//IT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];
    BYTE		mscOutgoingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//OT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)seizureTime 
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)answerTime
    BYTE		initial_year;           // Í¨»°³õÊ¼Ê±¿Ì
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            //3
    BYTE		initial_minute;
    BYTE		initial_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)releaseTime
    BYTE		end_year; // Í¨»°½áÊøÊ±¼ä
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callDuration
    UINT4		callDuration;//conversation_time;                 // Í¨»°Ê±³¤

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)causeForTerm
    BYTE		causeForTerm;//cause_for_termination;        //  Í¨»°ÖÕÖ¹Ô­Òò

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)SDiagnostics
    SDiagnostics     diagnostics;//uDiagnostics;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; ÓÃÓÚÍ¬Ò»ÒÆ¶¯Ì¨·¢ÉúµÄ²»Í¬»°ÎñµÄ±¾µØÊ¶±ðºÅ

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)numberOfDPEncountered
    BYTE		numberOfDPEncountered;//CAMEL_num_of_DP;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)levelOfCAMELService
    BYTE		levelOfCAMELService;//CAMEL_service_level;


    MBILL_CHANGEFLAGS     CAMELModificationChangeFlags;

	BYTE		ModifiedCallingPartyNumber_npi  :4;                 // ºÅÂë¼Æ»®
    BYTE		ModifiedCallingPartyNumber_nai  :3;                 // ºÅÂëÀàÐÍ   
    BYTE		s9               :1;    
    BYTE		ModifiedCallingPartyNumber_BCDlen;                 // ºÅÂë³¤¶È
    BYTE		ModifiedCallingPartyNumber[MAX_NUM_LEN/2];     // ¸Ä±äºóµÄÖ÷½ÐºÅÂë

    BYTE		ModifiedCallingPartyCategory;// ÐÞ¸ÄºóµÄÖ÷½ÐÓÃ»§Àà±ð

    BYTE		ModifiedOriginalCalledPartyNumber_npi  :4;                 // ºÅÂë¼Æ»®
    BYTE		ModifiedOriginalCalledPartyNumber_nai  :3;                 // ºÅÂëÀàÐÍ   
    BYTE		s10               :1;       
    BYTE		ModifiedOriginalCalledPartyNumber_BCDlen;                 // ºÅÂë³¤¶È
    BYTE		ModifiedOriginalCalledPartyNumber[MAX_NUM_LEN/2]; // ¸Ä±äºóµÄÔ­±»½ÐºÅÂë

    BYTE		ModifiedGenericNumber_npi  :4;                 // ºÅÂë¼Æ»®
    BYTE		ModifiedGenericNumber_nai  :3;                 // ºÅÂëÀàÐÍ 
    BYTE		s11               :1;    
    BYTE		ModifiedGenericNumber_BCDlen;                 // ºÅÂë³¤¶È
    BYTE		ModifiedGenericNumber[MAX_NUM_LEN/2];   // ¸Ä±äºóµÄGenericNumber,²Î¼ûCapEncode_TGenericNumbers , TGenericNumbers

    BYTE		ModifiedRedirectingPartyNumber_npi  :4;                 // ºÅÂë¼Æ»®
    BYTE		ModifiedRedirectingPartyNumber_nai  :3;                 // ºÅÂëÀàÐÍ
    BYTE		s12               :1;    
    BYTE		ModifiedRedirectingPartyNumber_BCDlen;                 // ºÅÂë³¤¶È
    BYTE		ModifiedRedirectingPartyNumber[MAX_NUM_LEN/2];     // ¸Ä±äºóµÄRedirectingPartyNumber
    
    BYTE		ModifiedRedirectionCounter; 

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)freeFormatDataAppend
    BYTE		freeFormatDataAppend:2;//CAMEL_FCI_Data_Append_Ind :2; //±íÊ¾ÊÇµÚÒ»¸öFCIÏûÏ¢£¬0±íÊ¾²»ÊÇµÚÒ»¸ö  

    //(TermCAMELRecord)mscServerIndication//Ä¿Ç°Ã»ÓÐÌîÐ´£¬Ô¤Áô±ÈÌØÎ»
    BYTE		mscServerIndication:1;    

    //(MOCallRecord)(TermCAMELRecord)defaultCallHandling-2 
    BYTE		defaultCallHandling2:2 ;//CAMEL_default_Call_Handling2;   
    BYTE		s13       :3;    
      
    //(MOCallRecord)(TermCAMELRecord)gsm-SCFAddress-2·ÅÔÚ¿É±ä²¿·Ö
    BYTE		CAMEL_GsmSCF_Address2_npi : 4;
    BYTE		CAMEL_GsmSCF_Address2_nai :  3; 
    BYTE		s14            : 1;   
    BYTE		CAMEL_GsmSCF_Address2_BCDlen;
    BYTE		CAMEL_GsmSCF_Address2[MAX_SCF_LEN]; 

    //(MOCallRecord)(TermCAMELRecord)serviceKey-2·ÅÔÚ¿É±ä²¿·Ö
    UINT4		ServiceKey2;                //ÒµÎñ¼ü

    //(MOCallRecord)(TermCAMELRecord)freeFormatData-2·ÅÔÚ×îºó³¤¶È¿É±ä

    //(MOCallRecord)(TermCAMELRecord)freeFormatDataAppend-2
    BYTE		freeFormatDataAppend2:2;//CAMEL_FCI_Data_Append_Ind2 :2;    
    BYTE		s15         :6;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)(TermCAMELRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  ±íÃ÷¸Ã´ÎÒµÎñÎªµç»°ÒµÎñ£¨Îª¡°0¡±£©»ò³ÐÔØÒµÎñ£¨Îª¡°1¡±£©   
    BYTE		ROAM_mark       :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  ÒÀ¾ÝTBSÌîµç»°ÒµÎñÂë»ò³ÐÒµÎñÂë¡£
    
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // ¼Æ·ÑÃâ·Ñ±êÖ¾   
	BYTE		s17             :4;
    
    //(MOCallRecord)(TermCAMELRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ·ÑÂÊÖ¸Ê¾
    UINT2		money_per_count; // ·ÑÂÊ
    UINT2		add_fee;                 //¸½¼Ó·Ñ

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)chargedParty Ä¿Ç°Ã»ÓÐÌîÐ´£¬ÏÈÔ¤Áô±ÈÌØÎ»
    BYTE        chargedParty; //enum MBILL_CHARGED_PARTY
    
    BYTE        UssdCallBackIndication:1;// ucbºô½ÐµÄÕæÊµÖ÷½Ð²Å¼ÇÂ¼´ËÓò
    BYTE        msc_id_len:3;//orgMSCId³¤¶ÈÖ¸Ê¾
    BYTE        initialCallAttemptFlag:1;//ICAºô½Ð±êÖ¾
    BYTE        s18         :3;
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)originalCalledNumber Ä¿Ç°Ã»ÓÐÌîÐ´£¬Ô¤Áô
    BYTE		originalCalledNumber_npi  :4;                 // ºÅÂë¼Æ»®    
    BYTE		originalCalledNumber_nai  :3;                 // ºÅÂëÀàÐÍ
    BYTE		s19               :1;    
    BYTE		originalCalledNumber_BCDlen;                 // ºÅÂë³¤¶È
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;

    BYTE		cause_for_partial_record;//cause_for_partial_record ;      //  ³öÖÐ¼ä»°µ¥Ô­Òò

    //ÐÂÔöÈÝÆ÷ÓÃ×÷ºóÐøÀ©Õ¹
    BYTE		container[MAX_CONTAINER_TCAMEL_LEN];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)freeFormatData FCI·ÅÔÚ¿É±ä²¿·Ö
    //(MOCallRecord)(TermCAMELRecord)freeFormatData-2 FCI·ÅÔÚ¿É±ä²¿·Ö
    BYTE		fcidata_len;
    BYTE		fcidata2_len;
    BYTE     VarPart[MAX_TRANSFER_FCI_LEN * 2];
    
	// BYTE		FCIData[MAX_TRANSFER_FCI_LEN];
    // BYTE		FCIData2[MAX_TRANSFER_FCI_LEN];

	
};

//1.4	ROAM_Bill_Report£¨°üÀ¨ROAM£¨roamingRecord£©£©
struct ROAM_Bill_Report
{
	// ¹«¹²Óò²¿·Ö, »ù±¾Çø:

    BYTE		validflag; // ÓÐÐ§ÐÔ±êÖ¾,¡¡¡°AA¡±ÎªÓÐÐ§£¬¡°55¡±ÎªÎÞÐ§
    UINT2		size; // ±¾ÕÅ»°µ¥³¤¶È
    UINT2		CRCCode; // CRCÐ£Ñé½á¹û
    BYTE		bill_type; //  enum   GSM_BILL_TYPE, Ð­ÒéÖÐµÄrecordTypeÓò
    UINT4		bill_sequence; // »°µ¥Á÷Ë®ºÅ£¬Î¨Ò»Çø·ÖÒ»ÕÅ»°µ¥¡£
    BYTE		ModuleNum; // ²úÉú»°µ¥µÄÄ£¿éºÅ
    UINT4		count; // GSM ¼Æ·Ñµ±Ò»´Îºô½Ð²úÉú¶àÕÅ»°µ¥Ê±count ÊÇ¸Ã´Îºô½ÐÊ×»°µ¥Á÷Ë®ºÅ
    BYTE		sequence; // µ¥ÕÅ»°µ¥Îª0£¬Ê×ÕÅ»°µ¥Îª1£¬ÆäÓàÀàÍÆ
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, µ¥ÕÅÊ×ÕÅÖÐ¼äÄ©ÕÅ»°µ¥±êÊ¶
    BYTE		HotBillingTag:1; //Îªcmcc²âÊÔÔö¼ÓµÄÈÈ¼Æ·Ñ±êÖ¾
    BYTE		CRC_Way:2; // CRCÐ£Ñé·½Ê½
    BYTE		ChargeLevel:1;//¼Æ·Ñ¾«¶ÈÖ¸Ê¾:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
    BYTE		Comm_reserved;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
	
	//  À©Õ¹Çø:
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  ±»¼Æ·ÑÒÆ¶¯ÓÃ»§IMSIÂë

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // ±»¼Æ·ÑÒÆ¶¯ÓÃ»§MSISDNºÅ
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callingNumber
    BYTE		callingNumber_npi  :4;                 // Ö÷½ÐºÅÂë¼Æ»®
    BYTE		callingNumber_nai  :3;                 // Ö÷½ÐºÅÂëÀàÐÍ
    BYTE		s2               :1;    
    BYTE		callingNumber_BCDlen ;                 // Ö÷½ÐºÅÂë³¤¶È
    BYTE		callingNumber[MAX_NUM_LEN/2];     // Ö÷½ÐºÅÂë

    //(MOCallRecord)(MTCallRecord)(roamingRecord)roamingNumber
    BYTE		roamingNumber_npi : 4;
    BYTE		roamingNumber_nai :  3;    
    BYTE		s3            : 1;    
    BYTE		roamingNumber_BCDlen ;
    BYTE		roamingNumber[MAX_NUM_LEN/2] ;    //MSRN               //  ÒÆ¶¯±»½ÐÂþÓÎºÅÂë

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;    
    BYTE		s4            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  ±¾¾ÖMSCºÅ

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mscIncomingROUTE/mscOutgoingROUTE
    BYTE		mscIncomingROUTE_ind : 4;//IT_trunk_group_ind : 4;    
    BYTE		mscOutgoingROUTE_ind : 4;//OT_trunk_group_ind : 4;    
    UINT2		mscIncomingROUTE_ROUTENumber;//IT_trunk_group;                     // ÈëÖÐ¼ÌÈº
    UINT2		mscOutgoingROUTE_ROUTENumber;//OT_trunk_group;                    // ³öÖÐ¼ÌÈº
    BYTE		mscIncomingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//IT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];
    BYTE		mscOutgoingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//OT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];

    //(MOCallRecord)(MTCallRecord)(roamingRecord)(TermCAMELRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  ±íÃ÷¸Ã´ÎÒµÎñÎªµç»°ÒµÎñ£¨Îª¡°0¡±£©»ò³ÐÔØÒµÎñ£¨Îª¡°1¡±£©    
    BYTE		ROAM_mark          :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  ÒÀ¾ÝTBSÌîµç»°ÒµÎñÂë»ò³ÐÒµÎñÂë¡£

    BYTE		transparencyIndicator:2;  //transparency_indicator :2;  // Í¸Ã÷·ÇÍ¸Ã÷Ö¸Ê¾   
    
    //(MOCallRecord)(MTCallRecord)(roamingRecord)transparencyIndicator
	BYTE        SORIndication	:1;//ÓÅÑ¡Â·ÓÉ±êÖ¾ 
    BYTE		s6          :5;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)changeOfService ÓÃÊÂ¼þ»°µ¥»òÖÐ¼ä»°µ¥µÄ·½Ê½³ö

    //(MOCallRecord)(MTCallRecord)(roamingRecord)supplServicesUsed(ºô½ÐÎÞ¹ØµÄ²¹³äÒµÎñÓÃÊÂ¼þ»°µ¥·½Ê½³ö,BAUºÏ²¢)
    struct MBILL_SS_code1     SS_code1;          
    struct MBILL_SS_code2     SS_code2;          
    struct MBILL_SS_code3     SS_code3;          
    struct MBILL_SS_code4     SS_code4;          
    struct MBILL_SS_code5     SS_code5;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)seizureTime 
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)answerTime
    BYTE		initial_year;           // Í¨»°³õÊ¼Ê±¿Ì
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            //3
    BYTE		initial_minute;
    BYTE		initial_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)releaseTime
    BYTE		end_year; // Í¨»°½áÊøÊ±¼ä
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callDuration
    UINT4		callDuration;//conversation_time;                 // Í¨»°Ê±³¤

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)causeForTerm
    BYTE		causeForTerm;//cause_for_termination;        //  Í¨»°ÖÕÖ¹Ô­Òò

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)SDiagnostics
    SDiagnostics     diagnostics;//uDiagnostics;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; ÓÃÓÚÍ¬Ò»ÒÆ¶¯Ì¨·¢ÉúµÄ²»Í¬»°ÎñµÄ±¾µØÊ¶±ðºÅ

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)sequenceNumber: ÖÐ¼ä»°µ¥ÐòÁÐºÅ,·ÅÔÚ¹«¹²Óò²¿·Ö, »ù±¾Çø:sequenceÓò

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)networkCallReference
    //µ±Ê¹ÓÃCAMELÒµÎñÊ±,Í¬Ò»»°ÎñËù¾­µÄ¸÷ÍøÔª²úÉúµÄ»°µ¥½«º¬ÓÐ´Ë±êÊ¶Âë(NCR),ÓÃÒÔ°ÑÕâÐ©»°µ¥¹ØÁªµ½Ò»Æð. 
    BYTE		NetworkCallReferenceLen;
    BYTE		NetworkcallReferenceNumber[maxCallReferenceNumberLength]; // ÓÉNetworkCallReferenceLenÖ¸Ã÷³¤¶È(2G¾ÉÓÐµÄºô½Ð²Î¿¼ºÅ)

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mSCAddress (¸úNetworkCallReferenceLenÅäºÏÊ¹ÓÃ)
    BYTE		MSCAddress_npi : 4;
    BYTE		MSCAddress_nai :  3;
    BYTE		s7            : 1;   
    BYTE		MSCAddress_BCDlen;
    BYTE		MSCAddress[MAX_MSC_NUM_IN_BYTES];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // ¼Æ·ÑÃâ·Ñ±êÖ¾
    BYTE		callEmlppPriority   :4;
    
    //(MOCallRecord)(TermCAMELRecord)(roamingRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ·ÑÂÊÖ¸Ê¾
    UINT2		money_per_count; // ·ÑÂÊ
    UINT2		add_fee;                 //¸½¼Ó·Ñ

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)chargedParty Ä¿Ç°Ã»ÓÐÌîÐ´£¬ÏÈÔ¤Áô±ÈÌØÎ»
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)originalCalledNumber Ä¿Ç°Ã»ÓÐÌîÐ´£¬Ô¤Áô
    BYTE		originalCalledNumber_npi  :4;                 // ºÅÂë¼Æ»®
    BYTE		originalCalledNumber_nai  :3;                 // ºÅÂëÀàÐÍ    
    BYTE		s9               :1;    
    BYTE		originalCalledNumber_BCDlen;                 // ºÅÂë³¤¶È
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

    //(MOCallRecord)(MTCallRecord)(roamingRecord)callingChargeAreaCode Ä¿Ç°Ã»ÓÐÌîÐ´£¬Ô¤Áô//Ì©¹úÌØÊâÓò    BYTE    CAC_num[MAX_CAC_LEN/2+1];
    BYTE		callingChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];

    //(MOCallRecord)(MTCallRecord)(roamingRecord)calledChargeAreaCode Ä¿Ç°Ã»ÓÐÌîÐ´£¬Ô¤Áô//Ì©¹úÌØÊâÓò    BYTE    CAC_num[MAX_CAC_LEN/2+1];
    BYTE		calledChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];

    //(MOCallRecord)(roamingRecord)mscOutgoingCircuit
    UINT2		mscOutgoingCircuit;

    //(MTCallRecord)(roamingRecord)mscIncomingCircuit
    UINT2		mscIncomingCircuit;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;

     //(MOCallRecord)(MTCallRecord)(roamingRecord)cUGOutgoingAccessIndicator
    BYTE		CUGOutgoingAccessIndicator:2;//CUG_call_ind         :2; 
    BYTE		CUGOutgoingAccessUsed:2;//CUGOutgoingAccessUsed   :2;    
    BYTE		msc_id_len:3;//orgMSCId³¤¶ÈÖ¸Ê¾
    BYTE		s10          :1;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)cUGInterlockCode
    UINT4		CUGInterlockCode;//CUGInterlockCode;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)cUGIndex
    UINT2		CUGIndex;

    BYTE		cause_for_partial_record;//cause_for_partial_record ;      //  ³öÖÐ¼ä»°µ¥Ô­Òò

    BYTE        eaSubscriberInfo[MAX_NAEA_CODE_IN_BYTE + 1];
    BYTE        selectedCIC[MAX_NAEA_CODE_IN_BYTE + 1];
    //ÐÂÔöÈÝÆ÷ÓÃ×÷ºóÐøÀ©Õ¹
    BYTE     container[MAX_CONTAINER_ROAM_LEN];
	
};

//1.5	Gateway_Bill_Report£¨°üÀ¨GWO(outGatewayRecord)¡¢GWI(incGatewayRecord)¡¢Transit(TransitCallRecord)£©
struct Gateway_Bill_Report
{
	// ¹«¹²Óò²¿·Ö, »ù±¾Çø:
	
    BYTE		validflag; // ÓÐÐ§ÐÔ±êÖ¾,¡¡¡°AA¡±ÎªÓÐÐ§£¬¡°55¡±ÎªÎÞÐ§
    UINT2		size; // ±¾ÕÅ»°µ¥³¤¶È
    UINT2		CRCCode; // CRCÐ£Ñé½á¹û
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // »°µ¥Á÷Ë®ºÅ£¬Î¨Ò»Çø·ÖÒ»ÕÅ»°µ¥¡£
    BYTE		ModuleNum; // ²úÉú»°µ¥µÄÄ£¿éºÅ
    UINT4		count; // GSM ¼Æ·Ñµ±Ò»´Îºô½Ð²úÉú¶àÕÅ»°µ¥Ê±count ÊÇ¸Ã´Îºô½ÐÊ×»°µ¥Á÷Ë®ºÅ
    BYTE		sequence; // µ¥ÕÅ»°µ¥Îª0£¬Ê×ÕÅ»°µ¥Îª1£¬ÆäÓàÀàÍÆ
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, µ¥ÕÅÊ×ÕÅÖÐ¼äÄ©ÕÅ»°µ¥±êÊ¶
    BYTE		HotBillingTag:1; //Îªcmcc²âÊÔÔö¼ÓµÄÈÈ¼Æ·Ñ±êÖ¾
    BYTE		CRC_Way:2; // CRCÐ£Ñé·½Ê½
    BYTE		ChargeLevel:1;//¼Æ·Ñ¾«¶ÈÖ¸Ê¾:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
    BYTE		Comm_reserved;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä	
	
	//  À©Õ¹Çø:
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  ±»¼Æ·ÑÒÆ¶¯ÓÃ»§IMSIÂë

    //servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  ±»¼Æ·ÑÒÆ¶¯ÓÃ»§ÒÆ¶¯Ì¨Éè±¸Ê¶±ðÂë

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)callingNumber
    BYTE		callingNumber_npi  :4;                 // Ö÷½ÐºÅÂë¼Æ»®
    BYTE		callingNumber_nai  :3;                 // Ö÷½ÐºÅÂëÀàÐÍ 
    BYTE		s1               :1;    
    BYTE		callingNumber_BCDlen ;                 // Ö÷½ÐºÅÂë³¤¶È
    BYTE		callingNumber[MAX_NUM_LEN/2];     // Ö÷½ÐºÅÂë

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)calledNumber
    BYTE		calledNumber_npi  :4;//dialed_isdn_npi  :4;                 // ±»½ÐºÅÂë¼Æ»®
    BYTE		calledNumber_nai  :3;//dialed_isdn_nai  :3;                 // ±»½ÐºÅÂëÀàÐÍ 
    BYTE		s2               :1;  
    BYTE		calledNumber_BCDlen ;//dialed_isdn_num  :5;                 // ±»½ÐºÅÂë³¤¶È
    BYTE		calledNumber[MAX_NUM_LEN/2];  //dialed_num[MAX_NUM_LEN/2];     // ±»½ÐºÅÂë
    
    //(incGatewayRecord)(OutGatewayRecord)roamingNumber
    BYTE		roamingNumber_npi : 4;
    BYTE		roamingNumber_nai :  3;   
    BYTE		s3            : 1;   
    BYTE		roamingNumber_BCDlen ;
    BYTE		roamingNumber[MAX_NUM_LEN/2] ;    //MSRN               //  ÒÆ¶¯±»½ÐÂþÓÎºÅÂë

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;    
    BYTE		s4            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  ±¾¾ÖMSCºÅ

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)mscIncomingROUTE/mscOutgoingROUTE
    BYTE		mscIncomingROUTE_ind : 4;//IT_trunk_group_ind : 4;    
    BYTE		mscOutgoingROUTE_ind : 4;//OT_trunk_group_ind : 4;    
    UINT2		mscIncomingROUTE_ROUTENumber;//IT_trunk_group;                     // ÈëÖÐ¼ÌÈº
    UINT2		mscOutgoingROUTE_ROUTENumber;//OT_trunk_group;                    // ³öÖÐ¼ÌÈº
    BYTE		mscIncomingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//IT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];
    BYTE		mscOutgoingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//OT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)seizureTime 
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)answerTime
    BYTE		initial_year; // Í¨»°³õÊ¼Ê±¿Ì
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            //3
    BYTE		initial_minute;
    BYTE		initial_second;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)releaseTime
    BYTE		end_year; // Í¨»°½áÊøÊ±¼ä
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)callDuration
    UINT4		callDuration;//conversation_time;                 // Í¨»°Ê±³¤

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)causeForTerm
    BYTE		causeForTerm;//cause_for_termination;        //  Í¨»°ÖÕÖ¹Ô­Òò

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)SDiagnostics
    SDiagnostics     diagnostics;//uDiagnostics;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; ÓÃÓÚÍ¬Ò»ÒÆ¶¯Ì¨·¢ÉúµÄ²»Í¬»°ÎñµÄ±¾µØÊ¶±ðºÅ

    //(MOCallRecord)(MTCallRecord)(roamingRecord)(TermCAMELRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  ±íÃ÷¸Ã´ÎÒµÎñÎªµç»°ÒµÎñ£¨Îª¡°0¡±£©»ò³ÐÔØÒµÎñ£¨Îª¡°1¡±£©   
    BYTE		s5       :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  ÒÀ¾ÝTBSÌîµç»°ÒµÎñÂë»ò³ÐÒµÎñÂë¡£
    
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)sequenceNumber: ÖÐ¼ä»°µ¥ÐòÁÐºÅ,·ÅÔÚ¹«¹²Óò²¿·Ö, »ù±¾Çø:sequenceÓò

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // ¼Æ·ÑÃâ·Ñ±êÖ¾   
    BYTE		s6          :4;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ·ÑÂÊÖ¸Ê¾
    UINT2		money_per_count; // ·ÑÂÊ
    UINT2		add_fee;                 //¸½¼Ó·Ñ

    //(incGatewayRecord)(OutGatewayRecord)chargedParty Ä¿Ç°Ã»ÓÐÌîÐ´£¬ÏÈÔ¤Áô±ÈÌØÎ»
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)originalCalledNumber Ä¿Ç°Ã»ÓÐÌîÐ´£¬Ô¤Áô
    BYTE		originalCalledNumber_npi  :4;                 // ºÅÂë¼Æ»®
    BYTE		originalCalledNumber_nai  :3;                 // ºÅÂëÀàÐÍ  
    BYTE		s7               :1;   
    BYTE		originalCalledNumber_BCDlen;                 // ºÅÂë³¤¶È
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

    //(OutGatewayRecord)(TransitCallRecord)mscOutgoingCircuit
    UINT2		mscOutgoingCircuit;

    //(incGatewayRecord)(TransitCallRecord)mscIncomingCircuit
    UINT2		mscIncomingCircuit;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)callEmlppPriority
    BYTE		callEmlppPriority:4;//CallEmlpp;//ºô½Ð×îÖÕEMLPPÓÅÏÈ¼¶ £¨È¡Öµ0-7)
    BYTE		reserved:4;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)eaSubscriberInfo Ä¿Ç°Ã»ÓÐÌîÐ´£¬Ô¤Áô
    BYTE		eaSubscriberInfo[MAX_NAEA_CODE_IN_BYTE + 1];
    
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)selectedCIC Ä¿Ç°Ã»ÓÐÌîÐ´£¬Ô¤Áô
    BYTE		selectedCIC[MAX_CIC_LEN_IN_BYTE];

     //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)cUGOutgoingAccessIndicator
    BYTE		CUGOutgoingAccessIndicator:2;//CUG_call_ind         :2; 
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)cUGOutgoingAccessUsed
    BYTE		CUGOutgoingAccessUsed:2;//CUGOutgoingAccessUsed   :2;
    BYTE		msc_id_len:3;//orgMSCId³¤¶ÈÖ¸Ê¾
    BYTE		s8        :1;    

   
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)cUGInterlockCode
    UINT4		CUGInterlockCode;//CUGInterlockCode;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)cUGIndex
    UINT2		CUGIndex;

    //recordExtensions
    BYTE		cause_for_partial_record ;      //  ³öÖÐ¼ä»°µ¥Ô­Òò
    BYTE		gsm_gsvn;                          // 1ÒµÎñÀà±ð
    BYTE		SORIndication:1;
    BYTE		transparencyIndicator:2;   //Í¸Ã÷·ÇÍ¸Ã÷Ö¸Ê¾    

    BYTE		PromptAndCollectCharge:1;
    BYTE		transmission_mode:4;  // 2G»°µ¥Ê¹ÓÃµÄÈ«ËÙÂÊ°ëËÙÂÊ±êÖ¾

    BYTE		subscriberCategory;

    BYTE        isdn_bc_len;
    BYTE        isdn_bc[MAX_ISDN_BC_LEN];
    BYTE        LLC_len;
    BYTE        LLC[MAX_LLC_LEN];
    BYTE        HLC_len;
    BYTE        HLC[MAX_HLC_LEN];

    
    //ÐÂÔöÈÝÆ÷ÓÃ×÷ºóÐøÀ©Õ¹
    BYTE		container[MAX_CONTAINER_GATEWAY_LEN];
	
};

//1.6	SMSMT_Bill_Report£¨°üÀ¨SMS MT(MTSMSRecord)£©
struct SMSMT_Bill_Report
{
	// ¹«¹²Óò²¿·Ö, »ù±¾Çø:

	BYTE		validflag; // ÓÐÐ§ÐÔ±êÖ¾,¡¡¡°AA¡±ÎªÓÐÐ§£¬¡°55¡±ÎªÎÞÐ§
    UINT2		size; // ±¾ÕÅ»°µ¥³¤¶È
    UINT2		CRCCode; // CRCÐ£Ñé½á¹û
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // »°µ¥Á÷Ë®ºÅ£¬Î¨Ò»Çø·ÖÒ»ÕÅ»°µ¥¡£
    BYTE		ModuleNum; // ²úÉú»°µ¥µÄÄ£¿éºÅ
    UINT4		count; // GSM ¼Æ·Ñµ±Ò»´Îºô½Ð²úÉú¶àÕÅ»°µ¥Ê±count ÊÇ¸Ã´Îºô½ÐÊ×»°µ¥Á÷Ë®ºÅ
    BYTE		sequence; // µ¥ÕÅ»°µ¥Îª0£¬Ê×ÕÅ»°µ¥Îª1£¬ÆäÓàÀàÍÆ
    BYTE		record_type :3; //enum GSM_RECORD_TY
    BYTE		HotBillingTag:1; //Îªcmcc²âÊÔÔö¼ÓµÄÈÈ¼Æ·Ñ±êÖ¾
    BYTE		CRC_Way:2; // CRCÐ£Ñé·½Ê½
    BYTE		ChargeLevel:1;//¼Æ·Ñ¾«¶ÈÖ¸Ê¾:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
	BYTE		Comm_reserved;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
	
	//  À©Õ¹Çø:	
    BYTE		serviceCentre_npi : 4;//SMS_address_npi : 4;
    BYTE		serviceCentre_nai :  3;//SMS_address_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		serviceCentre_BCDlen;//SMS_address_num : 5;
    BYTE		serviceCentre[MAX_MSC_NUM_IN_BYTES];  //SMS_address[8];    //¶ÌÏûÏ¢ÖÐÐÄµØÖ·
    
    //(MOSMSRecord)(MTSMSRecord)servedIMSI
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  ±»¼Æ·ÑÒÆ¶¯ÓÃ»§IMSIÂë

    //(MOSMSRecord)(MTSMSRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  ±»¼Æ·ÑÒÆ¶¯ÓÃ»§ÒÆ¶¯Ì¨Éè±¸Ê¶±ðÂë

    //(MOSMSRecord)(MTSMSRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s2            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // ±»¼Æ·ÑÒÆ¶¯ÓÃ»§MSISDNºÅ
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MOSMSRecord)(MTSMSRecord)MSClassmark[3] 
    BYTE   MSClassmark[3];//struct  Classmark last_served_ms_classmark;      // ±»¼Æ·ÑÒÆ¶¯ÓÃ»§µ±Ç°CLASSMARK
    BillClassmark3 Classmark3Information;
    
    //(MOSMSRecord)(MTSMSRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;    
    BYTE		s3            : 1;   
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  ±¾¾ÖMSCºÅ

    //(MOSMSRecord)(MTSMSRecord)location Ö÷±»½ÐµÄ´ËÓòºÏÒ»
    BYTE		served_location_LAI[MAX_LOCAL_LAI_LEN/2];//caller_LAC[MAX_LOCAL_LAC_LEN/2]; called_LAC[MAX_LOCAL_LAC_LEN/2];
    BYTE		served_location_cellID[MAX_LOCAL_CI_LEN/2];//caller_cell_ID[MAX_LOCAL_CI_LEN/2]; called_cell_ID[MAX_LOCAL_CI_LEN/2]; 

    //(MOSMSRecord)originationTime/(MTSMSRecord)deliveryTime
    BYTE		OriginationDeliveryTime_year; 
    BYTE		OriginationDeliveryTime_month;
    BYTE		OriginationDeliveryTime_date;
    BYTE		OriginationDeliveryTime_hour;
    BYTE		OriginationDeliveryTime_minute;
    BYTE		OriginationDeliveryTime_second;

    //(MOSMSRecord)(MTSMSRecord)smsResult
    SDiagnostics  smsResult;//²Î¿¼map¶¨ÒåµÄSMSResult
    BYTE		actionResult;//²Ù×÷½á¹û£¬È¡SM_transmission_success,»ò SM_transmission_failed 

    //(MOSMSRecord)(MTSMSRecord)systemType
    BYTE		systemType:2;//USER_NETWORK_TYPE :4;     
    BYTE		s4        :6;
    
    //(MOSMSRecord)(MTSMSRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  ±íÃ÷¸Ã´ÎÒµÎñÎªµç»°ÒµÎñ£¨Îª¡°0¡±£©»ò³ÐÔØÒµÎñ£¨Îª¡°1¡±£©    
    BYTE		ROAM_mark         :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  ÒÀ¾ÝTBSÌîµç»°ÒµÎñÂë»ò³ÐÒµÎñÂë¡£
    
    //(MOSMSRecord)(MTSMSRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // ¼Æ·ÑÃâ·Ñ±êÖ¾
    BYTE		Classmark3Indication:1;
    BYTE		s6           :3;
    
    //(MOSMSRecord)(MTSMSRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ·ÑÂÊÖ¸Ê¾
    UINT2		money_per_count; // ·ÑÂÊ
    UINT2		add_fee;                 //¸½¼Ó·Ñ

    //(MOSMSRecord)(MTSMSRecord)chargedParty  Ä¿Ç°Ã»ÓÐÌîÐ´£¬ÏÈÔ¤Áô±ÈÌØÎ»
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId³¤¶ÈÖ¸Ê¾
    BYTE		msc_id_len:3;//orgMSCId³¤¶ÈÖ¸Ê¾
    BYTE		s23            : 2;
    //(MOSMSRecord)(MTSMSRecord)orgRNCorBSCId
    BYTE		orgRNCorBSCId[MAX_RNCBSC_ID_LEN];//Rnc_Bsc_Id[3];

    //(MOSMSRecord)(MTSMSRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(MOSMSRecord)(MTSMSRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;

    //(MOSMSRecord)(MTSMSRecord)smsUserDataType
    BYTE		smsUserDataType;// C SmsUserDataType, 0xFF indicads a normal single text SMS

    //(MOSMSRecord)(MTSMSRecord)maximumNumberOfSMSInTheConcatenatedSMS
    // C Total number of concatenatedSMS  0 is invalid
    BYTE		maximumNumberOfSMSInTheConcatenatedSMS;

    //(MOSMSRecord)(MTSMSRecord)concatenatedSMSReferenceNumber
    // C it is invalid if maximumNumberOfSMSInTheConcatenatedSMS = 0
    UINT2		concatenatedSMSReferenceNumber;

    //(MOSMSRecord)(MTSMSRecord)sequenceNumberOfTheCurrentSMS
    // C it is invalid if maximumNumberOfSMSInTheConcatenatedSMS = 0
    BYTE		sequenceNumberOfTheCurrentSMS;

    //(MOSMSRecord)(MTSMSRecord)hotBillingTag·ÅÔÚ»ù±¾ÇøHotBillingTag

    //(MTSMSRecord)origination
    BYTE		callingNumber_npi  :4; //caller_isdn_npi                // Ö÷½ÐºÅÂë¼Æ»®
    BYTE		callingNumber_nai  :3;  //caller_isdn_nai               // Ö÷½ÐºÅÂëÀàÐÍ
    BYTE		s7               :1;    
    BYTE		callingNumber_BCDlen ; //caller_isdn_num                // Ö÷½ÐºÅÂë³¤¶È
    BYTE		callingNumber[MAX_NUM_LEN/2]; //caller_num    // Ö÷½ÐºÅÂë

    //(MOSMSRecord)(MTSMSRecord)recordExtensions
    //  µ±Ç°±»¼Æ·ÑÒÆ¶¯ÓÃ»§Ëù´¦MSC
    BYTE		served_msc_num_npi : 4;
    BYTE		served_msc_num_nai :  3;
    BYTE		s8            : 1;
    BYTE		served_msc_num_BCDlen ;
	BYTE		served_msc_num[MAX_MSC_NUM_IN_BYTES];       

    //ÐÂÔöÈÝÆ÷ÓÃ×÷ºóÐøÀ©Õ¹
    BYTE		container[MAX_CONTAINER_SMSMT_LEN];
    
    //(MOSMSRecord)(MTSMSRecord)smstext 
    BYTE		smstext_len;
    BYTE     	VarPart[MAX_SMSTEXT_LEN];
    // BYTE		smstext[MAX_SMSTEXT_LEN];

	
};

//1.7	SMSMO_Bill_Report£¨°üÀ¨SMS MO(MOSMSRecord)£©
struct SMSMO_Bill_Report
{
	// ¹«¹²Óò²¿·Ö, »ù±¾Çø:

	BYTE		validflag; // ÓÐÐ§ÐÔ±êÖ¾,¡¡¡°AA¡±ÎªÓÐÐ§£¬¡°55¡±ÎªÎÞÐ§
    UINT2		size; // ±¾ÕÅ»°µ¥³¤¶È
    UINT2		CRCCode; // CRCÐ£Ñé½á¹û
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // »°µ¥Á÷Ë®ºÅ£¬Î¨Ò»Çø·ÖÒ»ÕÅ»°µ¥¡£
    BYTE		ModuleNum; // ²úÉú»°µ¥µÄÄ£¿éºÅ
    UINT4		count; // GSM ¼Æ·Ñµ±Ò»´Îºô½Ð²úÉú¶àÕÅ»°µ¥Ê±count ÊÇ¸Ã´Îºô½ÐÊ×»°µ¥Á÷Ë®ºÅ
    BYTE		sequence; // µ¥ÕÅ»°µ¥Îª0£¬Ê×ÕÅ»°µ¥Îª1£¬ÆäÓàÀàÍÆ
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, µ¥ÕÅÊ×ÕÅÖÐ¼äÄ©ÕÅ»°µ¥±êÊ¶
    BYTE		HotBillingTag:1; //Îªcmcc²âÊÔÔö¼ÓµÄÈÈ¼Æ·Ñ±êÖ¾
    BYTE		CRC_Way:2; // CRCÐ£Ñé·½Ê½
    BYTE		ChargeLevel:1;//¼Æ·Ñ¾«¶ÈÖ¸Ê¾:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
    BYTE		Comm_reserved;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
	
	//  À©Õ¹Çø:
    BYTE		serviceCentre_npi : 4;//SMS_address_npi : 4;
    BYTE		serviceCentre_nai :  3;//SMS_address_nai :  3;   
	BYTE		s1            : 1;    
    BYTE		serviceCentre_BCDlen;//SMS_address_num : 5;
    BYTE		serviceCentre[MAX_MSC_NUM_IN_BYTES];  //SMS_address[8];    //¶ÌÏûÏ¢ÖÐÐÄµØÖ·
    
    //(MOSMSRecord)(MTSMSRecord)servedIMSI
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  ±»¼Æ·ÑÒÆ¶¯ÓÃ»§IMSIÂë

    //(MOSMSRecord)(MTSMSRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  ±»¼Æ·ÑÒÆ¶¯ÓÃ»§ÒÆ¶¯Ì¨Éè±¸Ê¶±ðÂë

    //(MOSMSRecord)(MTSMSRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;   
    BYTE		s2            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // ±»¼Æ·ÑÒÆ¶¯ÓÃ»§MSISDNºÅ
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MOSMSRecord)(MTSMSRecord)MSClassmark[3] 
    BYTE   MSClassmark[3];//struct  Classmark last_served_ms_classmark;      // ±»¼Æ·ÑÒÆ¶¯ÓÃ»§µ±Ç°CLASSMARK
    struct  BillClassmark3 Classmark3Information;
    
    //(MOSMSRecord)(MTSMSRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;    
    BYTE		s3            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  ±¾¾ÖMSCºÅ

    //(MOSMSRecord)(MTSMSRecord)location Ö÷±»½ÐµÄ´ËÓòºÏÒ»
    BYTE		served_location_LAI[MAX_LOCAL_LAI_LEN/2];//caller_LAC[MAX_LOCAL_LAC_LEN/2]; called_LAC[MAX_LOCAL_LAC_LEN/2];
    BYTE		served_location_cellID[MAX_LOCAL_CI_LEN/2];//caller_cell_ID[MAX_LOCAL_CI_LEN/2]; called_cell_ID[MAX_LOCAL_CI_LEN/2]; 

    //(MOSMSRecord)MessageReference
    BYTE		MessageReference;

    //(MOSMSRecord)originationTime/(MTSMSRecord)deliveryTime
    BYTE		OriginationDeliveryTime_year; 
    BYTE		OriginationDeliveryTime_month;
    BYTE		OriginationDeliveryTime_date;
    BYTE		OriginationDeliveryTime_hour;
    BYTE		OriginationDeliveryTime_minute;
    BYTE		OriginationDeliveryTime_second;

    //(MOSMSRecord)(MTSMSRecord)smsResult
    SDiagnostics  smsResult;//²Î¿¼map¶¨ÒåµÄSMSResult
    BYTE		actionResult;//²Ù×÷½á¹û£¬È¡SM_transmission_success,»ò SM_transmission_failed

    //(MOSMSRecord)destinationNumber
    BYTE		destinationNumber_npi :4;//dialed_isdn_npi  :4;                 // ±»½ÐºÅÂë¼Æ»®
    BYTE		destinationNumber_nai:3;//dialed_isdn_nai  :3;                 // ±»½ÐºÅÂëÀàÐÍ 
    BYTE		s4               :1;    
    BYTE		destinationNumber_BCDlen; //dialed_isdn_num  :5;                 // ±»½ÐºÅÂë³¤¶È
    BYTE		destinationNumber[MAX_NUM_LEN/2];//dialed_num[MAX_NUM_LEN/2];     //12 ±»½ÐºÅÂë

    //(MOSMSRecord)(MTSMSRecord)systemType
    BYTE		systemType:2;//USER_NETWORK_TYPE :4; 
    BYTE		s5               :6;

    //(MOSMSRecord)(MTSMSRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  ±íÃ÷¸Ã´ÎÒµÎñÎªµç»°ÒµÎñ£¨Îª¡°0¡±£©»ò³ÐÔØÒµÎñ£¨Îª¡°1¡±£©    
    BYTE		ROAM_mark               :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  ÒÀ¾ÝTBSÌîµç»°ÒµÎñÂë»ò³ÐÒµÎñÂë¡£
    
    //(MOSMSRecord)(MTSMSRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // ¼Æ·ÑÃâ·Ñ±êÖ¾
    BYTE		Classmark3Indication:1;
    BYTE		s7                :3;
   
    //(MOSMSRecord)(MTSMSRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ·ÑÂÊÖ¸Ê¾
    UINT2		money_per_count; // ·ÑÂÊ
    UINT2		add_fee;                 //¸½¼Ó·Ñ

    //(MOSMSRecord)(MTSMSRecord)chargedParty  Ä¿Ç°Ã»ÓÐÌîÐ´£¬ÏÈÔ¤Áô±ÈÌØÎ»
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId³¤¶ÈÖ¸Ê¾
    BYTE		msc_id_len:3;//orgMSCId³¤¶ÈÖ¸Ê¾
    BYTE		s23            : 2;
    //(MOSMSRecord)(MTSMSRecord)orgRNCorBSCId
    BYTE		orgRNCorBSCId[MAX_RNCBSC_ID_LEN];//Rnc_Bsc_Id[3];

    //(MOSMSRecord)(MTSMSRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(MOSMSRecord)(MTSMSRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;

    //(MOSMSRecord)(MTSMSRecord)firstmccmnc
    BYTE		smsUserDataType;// C SmsUserDataType, 0xFF indicads a normal single text SMS

    //(MOSMSRecord)(MTSMSRecord)maximumNumberOfSMSInTheConcatenatedSMS
    // C Total number of concatenatedSMS  0 is invalid
    BYTE		maximumNumberOfSMSInTheConcatenatedSMS;

    //(MOSMSRecord)(MTSMSRecord)concatenatedSMSReferenceNumber
    // C it is invalid if maximumNumberOfSMSInTheConcatenatedSMS = 0
    UINT2		concatenatedSMSReferenceNumber;

    //(MOSMSRecord)(MTSMSRecord)sequenceNumberOfTheCurrentSMS
    // C it is invalid if maximumNumberOfSMSInTheConcatenatedSMS = 0
    BYTE		sequenceNumberOfTheCurrentSMS;

    //(MOSMSRecord)(MTSMSRecord)hotBillingTag·ÅÔÚ»ù±¾ÇøHotBillingTag

    //CAMEL SMSµÄÖÇÄÜÐÅÏ¢
    //(MOSMSRecord)CAMELSMSInformation 
    //CAMELSMSInformation:gsm-SCFAddress
    BYTE		CAMEL_GsmSCF_Address_npi : 4;
    BYTE		CAMEL_GsmSCF_Address_nai :  3;    
    BYTE		s8            : 1;    
    BYTE		CAMEL_GsmSCF_Address_BCDlen;
    BYTE		CAMEL_GsmSCF_Address[MAX_SCF_LEN]; 

    //CAMELSMSInformation:serviceKey
    UINT4		ServiceKey;

    //CAMELSMSInformation:defaultSMSHandling
    BYTE		defaultSMSHandling;

    //CAMELSMSInformation:callingPartyNumber
    BYTE		CAMELSMSCallingNumber_npi : 4;
    BYTE		CAMELSMSCallingNumber_nai :  3;   
    BYTE		s9            : 1;    
    BYTE		CAMELSMSCallingNumber_BCDlen;
    BYTE		CAMELSMSCallingNumber[MAX_NUM_LEN/2];
  
    //CAMELSMSInformation:destinationSubscriberNumber
    BYTE		CAMELSMSCalledNumber_npi : 4;
    BYTE		CAMELSMSCalledNumber_nai :  3;    
    BYTE		s10            : 1;    
    BYTE		CAMELSMSCalledNumber_BCDlen ;
    BYTE		CAMELSMSCalledNumber[MAX_NUM_LEN/2];

    //CAMELSMSInformation:cAMELSMSCAddress
    BYTE		CAMELSMSAddress_npi : 4;
    BYTE		CAMELSMSAddress_nai :  3;   
    BYTE		s11            : 1;    
    BYTE		CAMELSMSAddress_BCDlen;
    BYTE		CAMELSMSAddress[MAX_MSC_NUM_IN_BYTES];

    //CAMELSMSInformation:smsReferenceNumber Ä¿Ç°Ã»ÓÐÌîÐ´£¬Ô¤Áô
    BYTE		smsReferenceNumberLen;
    BYTE		smsReferenceNumber[maxCallReferenceNumberLength];
    
    //(MOSMSRecord)(MTSMSRecord)recordExtensions
    //  µ±Ç°±»¼Æ·ÑÒÆ¶¯ÓÃ»§Ëù´¦MSC
    BYTE		served_msc_num_npi : 4;
    BYTE		served_msc_num_nai :  3;
    BYTE		s12            : 1;
    BYTE		served_msc_num_BCDlen ;
    BYTE		served_msc_num[MAX_MSC_NUM_IN_BYTES];       

    //(MOSMSRecord)(MTSMSRecord)recordExtensions
    //ÐÂÔöÈÝÆ÷ÓÃ×÷ºóÐøÀ©Õ¹
    BYTE		container[MAX_CONTAINER_SMSMO_LEN];

    //CAMELSMSInformation:freeFormatData
    //(MOSMSRecord)(MTSMSRecord)smstext Ä¿Ç°Ã»ÓÐÌîÐ´£¬Ô¤Áô
    BYTE		fcidata_len;
    BYTE		smstext_len;
    BYTE		VarPart[MAX_TRANSFER_FCI_LEN + MAX_SMSTEXT_LEN];
    // BYTE		FCIData[MAX_TRANSFER_FCI_LEN];
    // BYTE		smstext[MAX_SMSTEXT_LEN];	
};

//1.8	HLR_Query_Bill_Report£¨°üÀ¨HLR Query(HLRIntRecord)£©
struct HLR_Query_Bill_Report
{
	// ¹«¹²Óò²¿·Ö, »ù±¾Çø:

	BYTE		validflag; // ÓÐÐ§ÐÔ±êÖ¾,¡¡¡°AA¡±ÎªÓÐÐ§£¬¡°55¡±ÎªÎÞÐ§
    UINT2		size; // ±¾ÕÅ»°µ¥³¤¶È
    UINT2		CRCCode; // CRCÐ£Ñé½á¹û
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // »°µ¥Á÷Ë®ºÅ£¬Î¨Ò»Çø·ÖÒ»ÕÅ»°µ¥¡£
    BYTE		ModuleNum; // ²úÉú»°µ¥µÄÄ£¿éºÅ
    UINT4		count; // GSM ¼Æ·Ñµ±Ò»´Îºô½Ð²úÉú¶àÕÅ»°µ¥Ê±count ÊÇ¸Ã´Îºô½ÐÊ×»°µ¥Á÷Ë®ºÅ
    BYTE		sequence; // µ¥ÕÅ»°µ¥Îª0£¬Ê×ÕÅ»°µ¥Îª1£¬ÆäÓàÀàÍÆ
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, µ¥ÕÅÊ×ÕÅÖÐ¼äÄ©ÕÅ»°µ¥±êÊ¶
    BYTE		HotBillingTag:1; //Îªcmcc²âÊÔÔö¼ÓµÄÈÈ¼Æ·Ñ±êÖ¾
    BYTE		CRC_Way:2; // CRCÐ£Ñé·½Ê½
    BYTE		ChargeLevel:1;//¼Æ·Ñ¾«¶ÈÖ¸Ê¾:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
    BYTE		Comm_reserved;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
	
	//  À©Õ¹Çø:
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  ±»¼Æ·ÑÒÆ¶¯ÓÃ»§IMSIÂë
	
    //(HLRIntRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // ±»¼Æ·ÑÒÆ¶¯ÓÃ»§MSISDNºÅ
	BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(HLRIntRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//served_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//served_msc_num_nai :  3;   
    BYTE		s2            : 1;   
    BYTE		recordingEntity_BCDlen ;//served_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //served_msc_num[8];         //8  µ±Ç°±»¼Æ·ÑÒÆ¶¯ÓÃ»§Ëù´¦MSC
	
    //(HLRIntRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  ±íÃ÷¸Ã´ÎÒµÎñÎªµç»°ÒµÎñ£¨Îª¡°0¡±£©»ò³ÐÔØÒµÎñ£¨Îª¡°1¡±£©    
    BYTE		s3            :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  ÒÀ¾ÝTBSÌîµç»°ÒµÎñÂë»ò³ÐÒµÎñÂë¡£
	
    //(HLRIntRecord)routingNumber
    BYTE		routingNumber_npi: 4;//RoutingNumber_npi : 4;
    BYTE		routingNumber_nai: 3;//RoutingNumber_nai :  3;   
    BYTE		s4            : 1;    
    BYTE		routingNumber_BCDlen ;//RoutingNumber_num : 5;
    BYTE		routingNumber[MAX_NUM_LEN/2];//RoutingNumber[MAX_MSRN_LEN];
	
    //(HLRIntRecord)interrogationTime
    BYTE		interrogationTime_year; 
    BYTE		interrogationTime_month;
    BYTE		interrogationTime_date;
    BYTE		interrogationTime_hour;
    BYTE		interrogationTime_minute;
    BYTE		interrogationTime_second;
	
    //(HLRIntRecord)numberOfForwarding
    BYTE		numbertypeind :2;    
    BYTE		s5          :2;
    BYTE		numberOfForwarding:4; //NumofForwarding;
	
    //(HLRIntRecord)interrogationResult Ã»ÓÐÌîÐ´£¬ÏÈÔ¤Áô×Ö½Ú
    SDiagnostics     interrogationResult;
	
    //(HLRIntRecord)orgMSCId
    BYTE		msc_id_len:3;//orgMSCId³¤¶ÈÖ¸Ê¾
    BYTE		s6        :5;
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];
    
    //(HLRIntRecord)recordExtensions
    //ÐÂÔöÈÝÆ÷ÓÃ×÷ºóÐøÀ©Õ¹
    BYTE		container[MAX_CONTAINER_HLRQUERY_LEN];
	
};

//1.9	SS_Action_Bill_Report£¨°üÀ¨SS Action(SSActionRecord)¡¢EVENT£©
struct SS_Action_Bill_Report
{
	// ¹«¹²Óò²¿·Ö, »ù±¾Çø:

	BYTE		validflag; // ÓÐÐ§ÐÔ±êÖ¾,¡¡¡°AA¡±ÎªÓÐÐ§£¬¡°55¡±ÎªÎÞÐ§
    UINT2		size; // ±¾ÕÅ»°µ¥³¤¶È
    UINT2		CRCCode; // CRCÐ£Ñé½á¹û
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // »°µ¥Á÷Ë®ºÅ£¬Î¨Ò»Çø·ÖÒ»ÕÅ»°µ¥¡£
    BYTE		ModuleNum; // ²úÉú»°µ¥µÄÄ£¿éºÅ
    UINT4		count; // GSM ¼Æ·Ñµ±Ò»´Îºô½Ð²úÉú¶àÕÅ»°µ¥Ê±count ÊÇ¸Ã´Îºô½ÐÊ×»°µ¥Á÷Ë®ºÅ
    BYTE		sequence; // µ¥ÕÅ»°µ¥Îª0£¬Ê×ÕÅ»°µ¥Îª1£¬ÆäÓàÀàÍÆ
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, µ¥ÕÅÊ×ÕÅÖÐ¼äÄ©ÕÅ»°µ¥±êÊ¶
    BYTE		HotBillingTag:1; //Îªcmcc²âÊÔÔö¼ÓµÄÈÈ¼Æ·Ñ±êÖ¾
    BYTE		CRC_Way:2; // CRCÐ£Ñé·½Ê½
    BYTE		ChargeLevel:1;//¼Æ·Ñ¾«¶ÈÖ¸Ê¾:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
    BYTE		Comm_reserved;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
	
	//  À©Õ¹Çø:
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  ±»¼Æ·ÑÒÆ¶¯ÓÃ»§IMSIÂë

    //(SSActionRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  ±»¼Æ·ÑÒÆ¶¯ÓÃ»§ÒÆ¶¯Ì¨Éè±¸Ê¶±ðÂë

    //(SSActionRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;    
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // ±»¼Æ·ÑÒÆ¶¯ÓÃ»§MSISDNºÅ
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(SSActionRecord)MSClassmark[3]
    BYTE   MSClassmark[3];//BYTE  ClassMark2;   
    struct  BillClassmark3 Classmark3Information;
    
    //(SSActionRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//    BYTE     served_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//    BYTE     served_msc_num_nai :  3;    
    BYTE		s2            : 1;       
    BYTE		recordingEntity_BCDlen ;//    BYTE     served_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //    BYTE    served_msc_num[8];       //8  µ±Ç°±»¼Æ·ÑÒÆ¶¯ÓÃ»§Ëù´¦MSC

    //(SSActionRecord)location Ö÷±»½ÐµÄ´ËÓòºÏÒ»
    BYTE		served_location_LAI[MAX_LOCAL_LAI_LEN/2];//caller_LAC[MAX_LOCAL_LAC_LEN/2];called_LAC[MAX_LOCAL_LAC_LEN/2]; 
    BYTE		served_location_cellID[MAX_LOCAL_CI_LEN/2];//caller_cell_ID[MAX_LOCAL_CI_LEN/2]; called_cell_ID[MAX_LOCAL_CI_LEN/2]; 

    //(SSActionRecord)BasicServices
    BYTE		TBS:4;//TBS:4;        //  ±íÃ÷¸Ã´ÎÒµÎñÎªµç»°ÒµÎñ£¨Îª¡°0¡±£©»ò³ÐÔØÒµÎñ£¨Îª¡°1¡±£©    
    BYTE		s3          :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  ÒÀ¾ÝTBSÌîµç»°ÒµÎñÂë»ò³ÐÒµÎñÂë¡£

    //(SSActionRecord)supplService
    BYTE		SSCode;

    //(SSActionRecord)ssAction
    BYTE		ssAction;//SSAction; //SSActionType

    //(SSActionRecord)ssActionTime
    BYTE		ssActionTime_year; //²¹³äÒµÎñ¶¯×÷·¢ÉúµÄÊ±¼ä´Á
    BYTE		ssActionTime_month;
    BYTE		ssActionTime_date;
    BYTE		ssActionTime_hour;            //3
    BYTE		ssActionTime_minute;
    BYTE		ssActionTime_second;

    //(SSActionRecord)ssActionResult
    SDiagnostics		ssActionResult;//SSResult;

    //(SSActionRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; ÓÃÓÚÍ¬Ò»ÒÆ¶¯Ì¨·¢ÉúµÄ²»Í¬»°ÎñµÄ±¾µØÊ¶±ðºÅ

   //(SSActionRecord)systemType
    BYTE		systemType:2;//AccessNetworkType;     
    //(SSActionRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // ¼Æ·ÑÃâ·Ñ±êÖ¾
    BYTE		Classmark3Indication:1;
    BYTE		s4 :1;    
     
    //(SSActionRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ·ÑÂÊÖ¸Ê¾
    UINT2		money_per_count; // ·ÑÂÊ
    UINT2		add_fee;                 //¸½¼Ó·Ñ

    //(SSActionRecord)chargedParty Ä¿Ç°Ã»ÓÐÌîÐ´£¬ÏÈÔ¤Áô±ÈÌØÎ»
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId³¤¶ÈÖ¸Ê¾
    BYTE		msc_id_len:3;//orgMSCId³¤¶ÈÖ¸Ê¾
    BYTE		s23            : 2;
    //(SSActionRecord)orgRNCorBSCId
    BYTE		orgRNCorBSCId[MAX_RNCBSC_ID_LEN];//Rnc_Bsc_Id[3];

    //(SSActionRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(SSActionRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;

    //(SSActionRecord)recordExtensions
    BYTE		event_type;  //¼ÇÂ¼²úÉúÊÂ¼þ»°µ¥µÄÊÂ¼þÀàÐÍ:CH,CW,MPTY,HO: enum EVENT_TYPE    

    //ussdÏà¹ØÐÅÏ¢,²Î¿¼Î÷ÃÅ×ÓµÄ½Ó¿Ú£¬Ô¤Áô
    BYTE		UssdAccessCode[MAX_USSDACCESSCODE_LEN];
    BYTE		UssdCodingScheme;
    BYTE		UssdNotifyCounter ;
    BYTE		UssdRequestCounter;  

    // Deleted by zengsonglin for R002C01 2004-04-07
    // BYTE     UssdString[MAXUINT2SDSTRING_LEN];

    //changeofAOC²ÉÓÃÊÂ¼þ»°µ¥´¦Àí£¬Îª´ËÔö¼ÓÏÂÊö»°µ¥Óò
    AOC_E1_E7    aocParameters;  

    //ÐÂÔöÈÝÆ÷ÓÃ×÷ºóÐøÀ©Õ¹
    BYTE     container[MAX_CONTAINER_SSACT_LEN];

    //(SSActionRecord)ssParameters 
    SSParameters   SSParameter;
    /*VarPart
    ×¢ÒâSSParameters±¾Éí¾ÍÊÇ¿É±ä³¤¶ÈµÄ
    ¼´SS_Para_lenÎª¿É±ä²¿·Ö³¤¶ÈÖ¸Ê¾
    ¿É±ä²¿·Ö×î´ó³¤¶ÈÎª40×Ö½Ú
    */	
};

//1.10	CEU_Bill_Report£¨°üÀ¨CEU(CommonEquipRecord)£©
struct CEU_Bill_Report
{
	// ¹«¹²Óò²¿·Ö, »ù±¾Çø:
	BYTE		validflag; // ÓÐÐ§ÐÔ±êÖ¾,¡¡¡°AA¡±ÎªÓÐÐ§£¬¡°55¡±ÎªÎÞÐ§
    UINT2		size; // ±¾ÕÅ»°µ¥³¤¶È
    UINT2		CRCCode; // CRCÐ£Ñé½á¹û
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // »°µ¥Á÷Ë®ºÅ£¬Î¨Ò»Çø·ÖÒ»ÕÅ»°µ¥¡£
    BYTE		ModuleNum; // ²úÉú»°µ¥µÄÄ£¿éºÅ
    UINT4		count; // GSM ¼Æ·Ñµ±Ò»´Îºô½Ð²úÉú¶àÕÅ»°µ¥Ê±count ÊÇ¸Ã´Îºô½ÐÊ×»°µ¥Á÷Ë®ºÅ
    BYTE		sequence; // µ¥ÕÅ»°µ¥Îª0£¬Ê×ÕÅ»°µ¥Îª1£¬ÆäÓàÀàÍÆ
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, µ¥ÕÅÊ×ÕÅÖÐ¼äÄ©ÕÅ»°µ¥±êÊ¶
    BYTE		HotBillingTag:1; //Îªcmcc²âÊÔÔö¼ÓµÄÈÈ¼Æ·Ñ±êÖ¾
    BYTE		CRC_Way:2; // CRCÐ£Ñé·½Ê½
    BYTE		ChargeLevel:1;//¼Æ·Ñ¾«¶ÈÖ¸Ê¾:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
    BYTE		Comm_reserved;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
	
	//  À©Õ¹Çø:
	
    BYTE		equipmentType;//Equipment_type;

    //(CommonEquipRecord)equipmentId
    UINT2		equipmentId;//Equipment_Id;

    //(CommonEquipRecord)servedIMSI
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  ±»¼Æ·ÑÒÆ¶¯ÓÃ»§IMSIÂë

    BYTE		servedIMEI[MAX_IMEI_LEN];  //  ±»¼Æ·ÑÒÆ¶¯ÓÃ»§ÒÆ¶¯Ì¨Éè±¸Ê¶±ðÂë

    //(CommonEquipRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // ±»¼Æ·ÑÒÆ¶¯ÓÃ»§MSISDNºÅ
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(CommonEquipRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//served_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//served_msc_num_nai :  3;   
    BYTE		s2            : 1;    
    BYTE		recordingEntity_BCDlen ;//served_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //served_msc_num[8];          //8  µ±Ç°±»¼Æ·ÑÒÆ¶¯ÓÃ»§Ëù´¦MSC

    //(CommonEquipRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  ±íÃ÷¸Ã´ÎÒµÎñÎªµç»°ÒµÎñ£¨Îª¡°0¡±£©»ò³ÐÔØÒµÎñ£¨Îª¡°1¡±£©    
    BYTE		s3     :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  ÒÀ¾ÝTBSÌîµç»°ÒµÎñÂë»ò³ÐÒµÎñÂë¡£
   //(CommonEquipRecord)supplServicesUsed
    BYTE		SSCode;
    //(CommonEquipRecord)seizureTime
    BYTE		initial_year;    // Í¨»°³õÊ¼Ê±¿Ì
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            //3
    BYTE		initial_minute;
    BYTE		initial_second;

    //(CommonEquipRecord)releaseTime 
    BYTE		end_year;               // Í¨»°½áÊøÊ±¼ä
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(CommonEquipRecord)callDuration
    UINT4		callDuration;//Call_Duration; Í¨»°Ê±³¤

    //(CommonEquipRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; ÓÃÓÚÍ¬Ò»ÒÆ¶¯Ì¨·¢ÉúµÄ²»Í¬»°ÎñµÄ±¾µØÊ¶±ðºÅ

    //(CommonEquipRecord)sequenceNumber: ÖÐ¼ä»°µ¥ÐòÁÐºÅ,·ÅÔÚ¹«¹²Óò²¿·Ö, »ù±¾Çø:sequenceÓò

    //(CommonEquipRecord)systemType 
    BYTE		systemType:2;//AccessNetworkType     
    BYTE		s4       :6;
    
    //(CommonEquipRecord)rateIndication
    BYTE		rateIndication;//ucRateIndication; //Êý¾ÝÒµÎñËÙÂÊÊÊÅä

    //(CommonEquipRecord)fnur
    BYTE		fnur;//ucFunr;           //Êý¾ÝÒµÎñÓÃ»§ËÙÂÊ,enum FNUR

    //(CommonEquipRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // ¼Æ·ÑÃâ·Ñ±êÖ¾    
 	BYTE		s5         :4;
   
    //(CommonEquipRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ·ÑÂÊÖ¸Ê¾
    UINT2		money_per_count; // ·ÑÂÊ
    UINT2		add_fee;                 //¸½¼Ó·Ñ

    //(CommonEquipRecord)chargedParty Ä¿Ç°Ã»ÓÐÌîÐ´£¬ÏÈÔ¤Áô±ÈÌØÎ»
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId³¤¶ÈÖ¸Ê¾
    BYTE		msc_id_len:3;//orgMSCId³¤¶ÈÖ¸Ê¾
    BYTE		s23            : 2;
    //(CommonEquipRecord)orgRNCorBSCId
    BYTE		orgRNCorBSCId[MAX_RNCBSC_ID_LEN];//Rnc_Bsc_Id[3];

    //(CommonEquipRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(CommonEquipRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;


    BYTE		causeForTerm;//cause_for_termination;        //  Í¨»°ÖÕÖ¹Ô­Òò
    SDiagnostics     diagnostics;//uDiagnostics;
    BYTE		cause_for_partial_record ;      //  ³öÖÐ¼ä»°µ¥Ô­Òò

    //ÐÂÔöÈÝÆ÷ÓÃ×÷ºóÐøÀ©Õ¹
    BYTE		container[MAX_CONTAINER_CEU_LEN];
	
};

//1.11	LCS_Bill_Report£¨°üÀ¨MO LCS(MOLCSRecord)¡¢MT LCS(MTLCSRecord)¡¢NI LCS(NILCSRecord)£©
struct LCS_Bill_Report
{
	// ¹«¹²Óò²¿·Ö, »ù±¾Çø:
	BYTE		validflag; // ÓÐÐ§ÐÔ±êÖ¾,¡¡¡°AA¡±ÎªÓÐÐ§£¬¡°55¡±ÎªÎÞÐ§
    UINT2		size; // ±¾ÕÅ»°µ¥³¤¶È
    UINT2		CRCCode; // CRCÐ£Ñé½á¹û
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // »°µ¥Á÷Ë®ºÅ£¬Î¨Ò»Çø·ÖÒ»ÕÅ»°µ¥¡£
    BYTE		ModuleNum; // ²úÉú»°µ¥µÄÄ£¿éºÅ
    UINT4		count; // GSM ¼Æ·Ñµ±Ò»´Îºô½Ð²úÉú¶àÕÅ»°µ¥Ê±count ÊÇ¸Ã´Îºô½ÐÊ×»°µ¥Á÷Ë®ºÅ
    BYTE		sequence; // µ¥ÕÅ»°µ¥Îª0£¬Ê×ÕÅ»°µ¥Îª1£¬ÆäÓàÀàÍÆ
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, µ¥ÕÅÊ×ÕÅÖÐ¼äÄ©ÕÅ»°µ¥±êÊ¶
    BYTE		HotBillingTag:1; //Îªcmcc²âÊÔÔö¼ÓµÄÈÈ¼Æ·Ñ±êÖ¾
    BYTE		CRC_Way:2; // CRCÐ£Ñé·½Ê½
    BYTE		ChargeLevel:1;//¼Æ·Ñ¾«¶ÈÖ¸Ê¾:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
    BYTE		Comm_reserved;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
	
	//  À©Õ¹Çø:
	
    BYTE		lcs_bill_type;//location_type;             // 1 enum LOCATION_TYPE: MT¡¢MO¡¢NI
    
    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//served_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//served_msc_num_nai :  3;
    BYTE		s1            : 1;    
    BYTE		recordingEntity_BCDlen ;//served_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //served_msc_num[8];         //8  µ±Ç°±»¼Æ·ÑÒÆ¶¯ÓÃ»§Ëù´¦MSC

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)lcsClientType
    BYTE		lcsClientType;//LCSClientType;  //enum LCSClientType

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)lcsClientIdentity  
    BYTE		lcsClientIdentity_npi : 4;//lcs_client : 4;
    BYTE		lcsClientIdentity_nai : 3;//lcs_client :  3; 
    BYTE		s2            : 1;    
    BYTE		lcsClientIdentity_BCDlen ;//lcs_client : 5;
    BYTE		lcsClientIdentity[MAX_LCS_CLIENT_EXTERNAL_ID_LEN];//    BYTE    lcs_client[MAX_LCS_CLIENT_EXTERNAL_ID_LEN]; //20 Íâ²¿µÄLCS-CLIENT±àÂë

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)servedIMSI
    BYTE		servedIMSI[MAX_IMSI_LEN];  //    BYTE    target_IMSI[MAX_IMSI_LEN];          //Ä¿±êIMSIºÅÂë£¬8×Ö½Ú

    //(NILCSRecord)servedIMEI  
    BYTE		servedIMEI[MAX_IMEI_LEN];//    BYTE    target_IMEI[MAX_IMEI_LEN];          //Ä¿±êIMEIºÅÂë£¬8×Ö½Ú

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;//target_num_npi : 4;
    BYTE		servedMSISDN_nai :  3;//target_num_nai :  3;    
    BYTE		s3            : 1;    
    BYTE		servedMSISDN_BCDlen;//target_num_num : 5;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//target_num [MAX_NUM_LEN/2+1]; //Ä¿±êºÅÂë£¬13×Ö½Ú
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MTLCSRecord)locationType
    BYTE		locationType;//LocationEstimateType;

    //(MOLCSRecord)molr-Type
    BYTE		molr_Type;//MOLRType;  //enum MOLR_Type

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)lcsQos 
    //QOS ¶¨Î»¾«¶ÈÒªÇó£¬ÏìÓ¦Ê±¼äµÈ
    BYTE		horizontal_accuracy; //Ë®Æ½¾«¶È
    BYTE		verticalCoordinateRequest; //0²»ÒªÇó£¬1ÒªÇó
    BYTE		vertical_accuracy;   //´¹Ö±·½Ïò¾«¶È
    BYTE		responseTime; //¶¨Î»ÏìÓ¦Ê±¼ä

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)lcsPriority
    BYTE		lcsPriority;//lcs_priority;                 //  ¶¨Î»ÓÅÏÈ¼¶

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)mlc-Number
    BYTE		GMLC_Number_npi : 4;//GMLC_address_npi : 4;
    BYTE		GMLC_Number_nai :  3;//GMLC_address_nai :  3;    
    BYTE		s4            : 1;    
    BYTE		GMLC_Number_BCDlen;//GMLC_address_num : 5;
    BYTE		GMLC_Number[MAX_GMLC_LEN + 1];//GMLC_address[9]; //9 GMLC±àÂë    

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)eventTimeStamp
    BYTE		eventTimeStamp_year;     //initial_year; // Í¨»°³õÊ¼Ê±¿Ì
    BYTE		eventTimeStamp_month;  //initial_month;
    BYTE		eventTimeStamp_date;     //initial_date;
    BYTE		eventTimeStamp_hour;     //initial_hour;            //3
    BYTE		eventTimeStamp_minute; //initial_minute;
    BYTE		eventTimeStamp_second; //initial_second;

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)measureDuration
     UINT4		measureDuration;//Call_Duration;    

    //(MTLCSRecord)notificationToMSUser
    BYTE		notificationToMSUser; //enum NotificationToMSUser

    //(MTLCSRecord)privacyOverride
    BYTE		privacyOverride;//PrivacyOverride;

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)location
    BYTE		served_location_LAI[MAX_LOCAL_LAI_LEN/2]; //target_LAC[MAX_LOCAL_LAC_LEN/2];    // Ä¿±êÎ»ÖÃÇøºÅ£¬2×Ö½Ú
    BYTE		served_location_cellID[MAX_LOCAL_CI_LEN/2];//target_cell_ID[MAX_LOCAL_CI_LEN/2]; // Ä¿±êÐ¡ÇøºÅ£¬2×Ö½Ú

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)locationEstimate
    //BYTE		locationEstimate;//

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)lcsCause  
    BYTE		lcsCause;//    BYTE     location_result;           // 1 enum LOCATION_RESULT  

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)SDiagnostics
    SDiagnostics diagnostics;//uDiagnostics;

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)systemType  
    BYTE		systemType:2;//    BYTE     AccessNetworkType;    
    BYTE		s5           :6;
    
    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)causeForTerm
    BYTE		causeForTerm;//CauseforTerm;   //enum CauseForTerm

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // ¼Æ·ÑÃâ·Ñ±êÖ¾   
    BYTE		s6            :4;

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ·ÑÂÊÖ¸Ê¾
    UINT2		money_per_count; // ·ÑÂÊ
    UINT2		add_fee;                 //¸½¼Ó·Ñ

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)chargedParty Ä¿Ç°Ã»ÓÐÌîÐ´£¬ÏÈÔ¤Áô±ÈÌØÎ»
    BYTE	    chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId³¤¶ÈÖ¸Ê¾
    BYTE		msc_id_len:3;//orgMSCId³¤¶ÈÖ¸Ê¾
    BYTE		s23            : 2;
    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)orgRNCorBSCId
    BYTE		orgRNCorBSCId[MAX_RNCBSC_ID_LEN];//Rnc_Bsc_Id[3];

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;

    //(MOLCSRecord)location_method
    BYTE		location_method;                 //¶¨Î»·½·¨ enum _locaton_method
    
    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)recordExtensions

    //(NILCSRecord)emsDigitsÄ¿Ç°Ã»ÓÐÌîÐ´£¬Ô¤Áô
    BYTE		emsDigits_npi : 4;
    BYTE		emsDigits_nai :  3;   
    BYTE		s7            : 1;    
    BYTE		emsDigits_BCDlen;
    BYTE		emsDigits[MAX_NUM_LEN/2];

    //(NILCSRecord)emsKeyÄ¿Ç°Ã»ÓÐÌîÐ´£¬Ô¤Áô
    BYTE		emsKey_npi : 4;
    BYTE		emsKey_nai :  3;  
    BYTE		s8            : 1;    
    BYTE		emsKey_BCDlen;
    BYTE		emsKey[MAX_NUM_LEN/2];

    //ÐÂÔöÈÝÆ÷ÓÃ×÷ºóÐøÀ©Õ¹
    BYTE		container[MAX_CONTAINER_LCS_LEN];

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)positioningData 
    BYTE		positioningDataLen;//    BYTE     shapeinfoLen;
    
    BYTE        call_reference_len;
    BYTE        call_reference[maxCallReferenceNumberLength];
    
    BYTE		VarPart[maxShapeInfoLength];//BYTE     shapeInfo[maxShapeInfoLength];
    /*×¢Òâ
    VarPart: positioningData¾ÍÊÇ¿É±ä²¿·Ö,×î´ó³¤¶ÈÎªmaxShapeInfoLength
    */
	
};

//1.12	CheckIMEI_Bill_Report£¨°üÀ¨CHECK IMEI£©
struct CheckIMEI_Bill_Report
{
	// ¹«¹²Óò²¿·Ö, »ù±¾Çø:

	BYTE		validflag; // ÓÐÐ§ÐÔ±êÖ¾,¡¡¡°AA¡±ÎªÓÐÐ§£¬¡°55¡±ÎªÎÞÐ§
    UINT2		size; // ±¾ÕÅ»°µ¥³¤¶È
    UINT2		CRCCode; // CRCÐ£Ñé½á¹û
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // »°µ¥Á÷Ë®ºÅ£¬Î¨Ò»Çø·ÖÒ»ÕÅ»°µ¥¡£
    BYTE		ModuleNum; // ²úÉú»°µ¥µÄÄ£¿éºÅ
    UINT4		count; // GSM ¼Æ·Ñµ±Ò»´Îºô½Ð²úÉú¶àÕÅ»°µ¥Ê±count ÊÇ¸Ã´Îºô½ÐÊ×»°µ¥Á÷Ë®ºÅ
    BYTE		sequence; // µ¥ÕÅ»°µ¥Îª0£¬Ê×ÕÅ»°µ¥Îª1£¬ÆäÓàÀàÍÆ
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, µ¥ÕÅÊ×ÕÅÖÐ¼äÄ©ÕÅ»°µ¥±êÊ¶
    BYTE		HotBillingTag:1; //Îªcmcc²âÊÔÔö¼ÓµÄÈÈ¼Æ·Ñ±êÖ¾
    BYTE		CRC_Way:2; // CRCÐ£Ñé·½Ê½
    BYTE		ChargeLevel:1;//¼Æ·Ñ¾«¶ÈÖ¸Ê¾:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
    BYTE		Comm_reserved;//¹«¹²Óò²¿·ÖµÄÀ©Õ¹¿Õ¼ä
	
	//  À©Õ¹Çø:
	
    BYTE		checkIMEI_year;          //initial_year; // Í¨»°³õÊ¼Ê±¿Ì
    BYTE		checkIMEI_month;       //initial_month;
    BYTE		checkIMEI_date;       //initial_date;
    BYTE		checkIMEI_hour;          //initial_hour;            
    BYTE		checkIMEI_minute;       //initial_minute;
    BYTE		checkIMEI_second;       //initial_second;
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  ±»¼Æ·ÑÒÆ¶¯ÓÃ»§IMSIÂë
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  ±»¼Æ·ÑÒÆ¶¯ÓÃ»§ÒÆ¶¯Ì¨Éè±¸Ê¶±ðÂë
	
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;   
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // ±»¼Æ·ÑÒÆ¶¯ÓÃ»§MSISDNºÅ
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    BYTE		served_location_LAI[MAX_LOCAL_LAI_LEN/2]; //target_LAC[MAX_LOCAL_LAC_LEN/2];    // Ä¿±êÎ»ÖÃÇøºÅ£¬5×Ö½Ú
    BYTE		served_location_cellID[MAX_LOCAL_CI_LEN/2]; //target_cell_ID[MAX_LOCAL_CI_LEN/2]; // Ä¿±êÐ¡ÇøºÅ£¬2×Ö½Ú
    
    BYTE		IMEIStatus;  //enum _Equipment_Status
    BYTE		Event;  //enum IMEICheckEvent
    
    BYTE		recordingEntity_npi : 4;//served_msc_npi : 4;
    BYTE		recordingEntity_nai : 3;//served_msc_nai :  3;  
    BYTE		s2            : 1;    
    BYTE		recordingEntity_BCDlen ;//served_msc_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //served_msc[8];         //·þÎñMSC±àÂë
	
    BYTE		msc_id_len:3;//orgMSCId³¤¶ÈÖ¸Ê¾
    BYTE		s3            : 5;
    BYTE		orgMSCId[MAX_MSCID_LEN];
	
    BYTE		CallReferenceLen;
    BYTE		CallReference[maxCallReferenceNumberLength];
	
    //ÐÂÔöÈÝÆ÷ÓÃ×÷ºóÐøÀ©Õ¹
    BYTE		container[MAX_CONTAINER_CHECKIMEI_LEN];
	
};

#pragma pack()

#endif  //__MSCV9_BILL_FORMAT_H__
