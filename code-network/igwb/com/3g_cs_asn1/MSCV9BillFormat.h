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
    ACCESS_BSC  = 0,   // 2G利大
    ACCESS_RNC  = 1,   // 3G利大
    ACCESS_UNKNOWN = 2, //隆岑利大 
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
    MOC					= 0,	//卞強兵窟柵出
    MTC					= 1,	//卞強嶮潤柵出
    GATEWAY_INCOMING	= 2,	//秘購笥蕉三汽
    EC					= 3,	//諸識柵出
    TRANSIT				= 4,	//嶄廬??祉俊柵出
    SS_ACT				= 5,	//温割匍暦園殻強恬??廣過、廣??、爾試、肇三
    MT_SMS				= 6,	//卞強岬嶮潤玉??連匍暦
    MO_SMS				= 7,    //卞強岬兵窟玉??連匍暦
    LOCATION_FREASH		= 8,    //了崔厚仟
    CALL_ATTEMPT		= 9,    //編柵三汽
	GATEWAY_OUTGOING	= 10,   //竃購笥蕉三汽  
    ISDN_OC             = 11,   
    ISDN_TC             = 12,   
    ROAM                = 13,   //只嗄三汽
    CommonEquip         = 14,   //巷慌譜姥三汽
    MO_FREECALL         = 17,   //麼出崘嬬喘薩窒継三汽窃侏
    CFW					= 18,   //念廬三汽
    MOI					= 19,   //旗恂三汽
    MO_HO               = 20,   //麼出俳算三汽
    MT_HO               = 21,   //瓜出俳算三汽
    MT_FREECALL         = 22,   //瓜出崘嬬喘薩窒継三汽窃侏
    CFW_FREECALL        = 23,   // 崘嬬念廬扮圻瓜出崘嬬喘薩窒継三汽窃侏 
    TCAMEL				= 24,   //嶮潤CAMEL恵諒芝村
    FCI					= 25,   //徭喇鯉塀方象三汽
 	LCS					= 26,   //了崔匍暦三汽 
    Check_IMEI			= 27,   //Check IMEI三汽窃侏
    QUERY_HLR			= 28,   //購笥蕉HLR諒儂三汽窃侏
    EVENT_BILL          = 30
};

enum HOT_BILLING_MARK
{
    NORMAL_BILLING     = 0x00,        //噸宥柴継
    HOT_BILLING        = 0x01         //犯柴継
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
    SINGLE_BILL,            //云肝柵出峪恢伏匯嫖三汽
    FIRST_BILL,             //云肝柵出及匯嫖三汽
    INTERMEDIATE_BILL,      //嶄寂三汽
    LAST_BILL               //云肝柵出恷朔匯嫖三汽
};

enum MBILL_TBS
{
    MBILL_TELECOM_SERVICE,  //窮佚匍暦
    MBILL_BEAR_SERVICE,     //覚墮匍暦
    MBILL_ISDN_SERVICE,     // ISDN匍暦
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

//by    剋鋲    2004-6-2    斤哘諒籾汽SWPD05614
struct MSC_Bill_Report
{
   	// 巷慌囃何蛍, 児云曝:
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
//the end  斤哘諒籾汽SWPD05614

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

//by    剋鋲    2004-6-2    斤哘諒籾汽SWPD05614
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
//the end  斤哘諒籾汽SWPD05614

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
// SS_code2忖粁燕幣泌和温割匍暦??
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

// SS_code3忖粁燕幣泌和温割匍暦??
struct MBILL_SS_code3
{
        BYTE   reserve4      :1;
        BYTE   reserve3      :1;
        
        //by    剋鋲    2004-6-2    斤哘諒籾汽SWPD05614
        BYTE   CLIR			 :1;
        BYTE   CLIP          :1;
        //the end  斤哘諒籾汽SWPD05614

        BYTE   ALLMOLR       :1;
        BYTE   UUS3          :1;
        BYTE   UUS2          :1;
        BYTE   UUS1          :1;
};
// SS_code4忖粁燕幣泌和温割匍暦??
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
// SS_code5忖粁燕幣泌和温割匍暦??
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
// SS_code2忖粁燕幣泌和温割匍暦??
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

// SS_code3忖粁燕幣泌和温割匍暦??
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
// SS_code4忖粁燕幣泌和温割匍暦??
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
// SS_code5忖粁燕幣泌和温割匍暦??
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

//1.1	MOC_Bill_Report?┛?凄MOC(moCallRecord) /CFW(forwardCallRecord:俊笥揖MOCallRecord)/MOI??
struct MOC_Bill_Report
 {
   	// 巷慌囃何蛍, 児云曝:
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
	//  制婢曝:
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

    BYTE		TBS:4;//TBS:4;        //  燕苧乎肝匍暦葎窮三匍暦??葎＾0￣??賜覚墮匍暦??葎＾1￣??    
	BYTE		ROAM_mark       :4;    
    BYTE		ServiceCode;//tele_bearer_code;  //  卆象TBS野窮三匍暦鷹賜覚匍暦鷹。

    BYTE		transparencyIndicator:2;  //transparency_indicator :2;  // 邑苧掲邑苧峺幣   
	BYTE		s9         :6;

    struct MBILL_SS_code1     SS_code1;          
    struct MBILL_SS_code2     SS_code2;          
    struct MBILL_SS_code3     SS_code3;          
    struct MBILL_SS_code4     SS_code4;          
    struct MBILL_SS_code5     SS_code5;

	AOC_E1_E7	aocParameters;

	BYTE		MSClassmark[3];// 軸圻栖議struct  Classmark initial_served_ms_classmark??瓜柴継卞強喘薩兜兵CLASSMARK??struct  Classmark音憲栽亅咏個葎BYTE
	BYTE		changeOfClassmark[3];//struct  Classmark last_served_ms_classmark;  //瓜柴継卞強喘薩個延朔議CLASSMARK

	BYTE		initial_year;           // 宥三兜兵扮震
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            
    BYTE		initial_minute;
    BYTE		initial_second;

	BYTE		end_year; // 宥三潤崩扮寂
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

	UINT4		callDuration;//conversation_time;                 // 宥三扮海
	
    BYTE		radioChanRequested:4;//USER_Radio_channelUINT2ed :4;                //   畠堀楕賜磯堀楕 
    BYTE		radioChanUsed:2; 	
	BYTE		s10           :2; 
	   
	BYTE		causeForTerm;//cause_for_termination;        //  宥三嶮峭圻咀
	
	SDiagnostics     diagnostics;//uDiagnostics;
	BYTE		call_reference_len;
	BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; 喘噐揖匯卞強岬窟伏議音揖三暦議云仇紛艶催

    BYTE		chargeIndicator:4;//free_indicator :4;  // 柴継窒継炎崗    
  	BYTE		s11          :4;

    
    //(MOCallRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 継楕峺幣
    UINT2		money_per_count; // 継楕
    UINT2		add_fee;
	
	BYTE		chargedParty;

    BYTE		CAMEL_GsmSCF_Address_npi : 4;
    BYTE		CAMEL_GsmSCF_Address_nai :  3;
	BYTE		s12            : 1;    
	
    BYTE		CAMEL_GsmSCF_Address_BCDlen;
    BYTE		CAMEL_GsmSCF_Address[MAX_SCF_LEN]; 

	UINT4		ServiceKey;                //匍暦囚

	BYTE		NetworkCallReferenceLen;
    BYTE		NetworkcallReferenceNumber[maxCallReferenceNumberLength];

	//(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mSCAddress (効NetworkCallReferenceLen塘栽聞喘)
    BYTE		MSCAddress_npi : 4;
    BYTE		MSCAddress_nai : 3;
    BYTE		s13            : 1;   
    
    BYTE		MSCAddress_BCDlen;
    BYTE		MSCAddress[MAX_MSC_NUM_IN_BYTES];

	BYTE		CAMELInitCFIndicator:1;
	BYTE		defaultCallHandling:2;
	BYTE		s14            : 5;
	
	BYTE		fnur;//ucFunr;           //方象匍暦喘薩堀楕,enum FNUR
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
    UINT4		ServiceKey2;                //匍暦囚
	
	BYTE		freeFormatDataAppend2:2;
	BYTE		freeFormatDataAppend:2;
	BYTE		defaultCallHandling2:2 ;
	BYTE		systemType:2;
	BYTE		rateIndication;

    BYTE		originalCalledNumber_npi  :4;                 // 催鷹柴皿
	BYTE		originalCalledNumber_nai  :3; 
	BYTE		s18               :1;    
                // 催鷹窃侏
    BYTE		originalCalledNumber_BCDlen;                 // 催鷹海業
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

	BYTE		callingChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];

	BYTE		calledChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];
	
	UINT2		mscOutgoingCircuit;
    UINT2		mscIncomingCircuit;
	
    BYTE		rnc_bsc_id_len:3;
    BYTE		msc_id_len:3;//orgMSCId海業峺幣
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
	
	BYTE	    CFW_invoked_times:4;    // 念廬窟伏議肝方    
	BYTE		CH_invoked_times:4;        // 柵出隠隔匍暦窟伏議肝方
    BYTE		CH_retrieve_times:4;       // 柵出隠隔匍暦志鹸議肝方    
    BYTE		CW_invoked_times:4;        // 柵出吉棋匍暦窟伏議肝方
    BYTE		MPTY_Build_times:4;        // 謹圭宥三匍暦秀羨窟伏議肝方    
    BYTE		MPTY_CH_times:4;           // 謹圭宥三匍暦隠隔窟伏議肝方
    BYTE		MPTY_CH_retrieve_times:4;   // 謹圭宥三匍暦隠隔志鹸窟伏議肝方   
    BYTE		MPTY_Split_times:4;        // 謹圭宥三匍暦侯宣窟伏議肝方
	
	BYTE		HO_invoked_times;         // 俳算窟伏議肝方
	BYTE		AOCParmsChangedTimes:4; 
	BYTE		ClassmarkChangedTimes:4;
    BYTE		RadioChanChangedTimes:4;	
	BYTE		transmission_mode:4;   // 2G三汽聞喘議  畠堀楕賜磯堀楕 炎崗。

	BYTE		Info_trans_capability;             // 覚墮嬬薦
    BillClassmark3 Classmark3Information;    
	
	BYTE		DTMF_indicator :2;  // 頁倦聞喘DTMF
	BYTE		user_type :5;  //   卞強岬單枠雫  
	BYTE		s8 :1;  //   0 燕幣云仇喘薩??1 燕幣只嗄喘薩?
	
  
    BYTE		EmergencyCallFlag     :1;          // 諸識柵出炎?   
    BYTE		PromptAndCollectCharge :1;               //彿坿柴継三汽炎崗參念議s6炎崗
    BYTE		SORIndication:1;
 	BYTE		s21:1;
	BYTE		EarlyForwardSORIndication	:1;
	BYTE		LateForwardSORIndication	:1; 	
    BYTE		s20                   :2;
    // BYTE		UssdCallBackIndication:1; 
    
    BYTE		ModemType:7;
	BYTE		Classmark3Indication:1;   
    
    BYTE		cause_for_partial_record;//cause_for_partial_record ;      //  竃嶄寂三汽圻咀
    BYTE		transaction_id;// 並暦ID,喘噐揖匯卞強岬窟伏議音揖三暦議云仇紛艶催
    BYTE		gsm_gsvn;                          // 1匍暦窃艶
    BYTE		B_ch_number;       // 乎肝柵出侭媼喘議B佚祇方
	
    BYTE		scfID[MAX_SCFID_LEN]; 

    BYTE		served_msc_num_npi : 4;
    BYTE		served_msc_num_nai :  3;
	BYTE		s22            : 1;
    BYTE		served_msc_num_BCDlen ;
	BYTE		served_msc_num[MAX_MSC_NUM_IN_BYTES];  
	
	BYTE		GuaranteedBitRate;  //隠屬曳蒙楕
	BYTE		MaximumBitRate;     //恷寄曳蒙楕

    //仟奐忖粁
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

	//辛延何蛍
	
	BYTE		fcidata_len;
    BYTE		fcidata2_len;
    BYTE     VarPart[MAX_TRANSFER_FCI_LEN * 2];
   
	/*
    VarPart喇參和曾何蛍怏撹
    BYTE FCIData[MAX_TRANSFER_FCI_LEN];
    BYTE FCIData2[MAX_TRANSFER_FCI_LEN];
    */

};

//1.2	MTC_Bill_Report?┛?凄MTC(mtCallRecord)??
struct MTC_Bill_Report
{
	// 巷慌囃何蛍, 児云曝:
	BYTE		validflag; // 嗤丼來炎崗,　＾AA￣葎嗤丼??＾55￣葎涙丼
    UINT2		size; // 云嫖三汽海業
    UINT2		CRCCode; // CRC丕刮潤惚
    BYTE		bill_type; //  enum   GSM_BILL_TYPE, 亅咏嶄議recordType囃
    UINT4		bill_sequence; // 三汽送邦催??率匯曝蛍匯嫖三汽。
    BYTE		ModuleNum; // 恢伏三汽議庁翠催
    UINT4		count; // GSM 柴継輝匯肝柵出恢伏謹嫖三汽扮count 頁乎肝柵出遍三汽送邦催
    BYTE		sequence; // 汽嫖三汽葎0??遍嫖三汽葎1??凪噫窃容
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 汽嫖遍嫖嶄寂挑嫖三汽炎紛
    BYTE		HotBillingTag:1; //葎cmcc霞編奐紗議犯柴継炎崗
    BYTE		CRC_Way:2; // CRC丕刮圭塀
    BYTE		ChargeLevel:1;//柴継娼業峺幣:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//巷慌囃何蛍議制婢腎寂
    BYTE		Comm_reserved;//巷慌囃何蛍議制婢腎寂
	
	//  制婢曝:
	
	BYTE		servedIMSI[MAX_IMSI_LEN];  //  瓜柴継卞強喘薩IMSI鷹

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  瓜柴継卞強喘薩卞強岬譜姥紛艶鷹

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;  
    BYTE		servedMSISDN_nai :  3;
    BYTE		s1            : 1;
    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // 瓜柴継卞強喘薩MSISDN催
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
        
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callingNumber
    BYTE		callingNumber_npi  :4;                 // 麼出催鷹柴皿
    BYTE		callingNumber_nai  :3;                 // 麼出催鷹窃侏    
    BYTE		s2               :1;

    BYTE		callingNumber_BCDlen ;                 // 麼出催鷹海業
    BYTE		callingNumber[MAX_NUM_LEN/2];     // 麼出催鷹

    //(MOCallRecord)(TermCAMELRecord)calledNumber
    BYTE		calledNumber_npi  :4;//dialed_isdn_npi  :4;                 // 瓜出催鷹柴皿
    BYTE		calledNumber_nai  :3;//dialed_isdn_nai  :3;                 // 瓜出催鷹窃侏
    BYTE		s3               :1;
   
    BYTE		calledNumber_BCDlen ;//dialed_isdn_num  :5;                 // 瓜出催鷹海業
    BYTE		calledNumber[MAX_NUM_LEN/2];  //dialed_num[MAX_NUM_LEN/2];     // 瓜出催鷹

    //(MOCallRecord)(MTCallRecord)connectedNumber
    BYTE		connectedNumber_npi :4;               //及眉圭催鷹柴皿
    BYTE		connectedNumber_nai :3;               //及眉圭催鷹窃侏   
    BYTE		s4                 :1;               //unuseful    

    BYTE		connectedNumber_BCDlen;               //及眉圭催鷹海業
    BYTE		connectedNumber[MAX_NUM_LEN/2];  // 及眉圭催鷹

    //(MOCallRecord)(MTCallRecord)(roamingRecord)roamingNumber
    BYTE		roamingNumber_npi : 4;
    BYTE		roamingNumber_nai :  3;    
    BYTE		s5            : 1;    
    BYTE		roamingNumber_BCDlen ;
    BYTE		roamingNumber[MAX_NUM_LEN/2] ;    //MSRN               //  卞強瓜出只嗄催鷹

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)originalCalledNumber 朕念短嗤野亟??圓藻
    BYTE		originalCalledNumber_npi  :4;                 // 催鷹柴皿
    BYTE		originalCalledNumber_nai  :3;                 // 催鷹窃侏
    BYTE		s6               :1;    
    BYTE		originalCalledNumber_BCDlen;                 // 催鷹海業
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

    //(MTCallRecord)redirectingnumber 朕念短嗤野亟??圓藻
    BYTE		redirectingnumber_npi  :4;                 // 催鷹柴皿
    BYTE		redirectingnumber_nai  :3;                 // 催鷹窃侏   
    BYTE		s7               :1;    
    BYTE		redirectingnumber_BCDlen;                 // 催鷹海業
    BYTE		redirectingnumber[MAX_NUM_LEN/2]; 

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;  
    BYTE		s8            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  云蕉MSC催

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mscIncomingROUTE/mscOutgoingROUTE
    BYTE		mscIncomingROUTE_ind : 4;//IT_trunk_group_ind : 4;       
    BYTE		mscOutgoingROUTE_ind : 4;//OT_trunk_group_ind : 4; 
    UINT2		mscIncomingROUTE_ROUTENumber;//IT_trunk_group;                     // 秘嶄写蛤
    UINT2		mscOutgoingROUTE_ROUTENumber;//OT_trunk_group;                    // 竃嶄写蛤
    BYTE		mscIncomingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//IT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];
    BYTE		mscOutgoingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//OT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];

    //(MTCallRecord)location/changeOfLocation 
    BYTE		caller_location_LAI[MAX_LOCAL_LAI_LEN/2];//caller_org_LAC[MAX_LOCAL_LAC_LEN/2];  //  瓜柴継卞強喘薩侭侃兜兵了崔曝催
    BYTE		caller_location_cellID[MAX_LOCAL_CI_LEN/2];//caller_org_cell_ID[MAX_LOCAL_CI_LEN/2];   // 瓜柴継卞強喘薩侭侃議兜兵弌曝催
    BYTE		caller_changeOflocation_LAI[MAX_LOCAL_LAI_LEN/2]; //caller_LAC[MAX_LOCAL_LAC_LEN/2];     // 瓜柴継卞強喘薩侭侃了崔曝催
    BYTE		caller_changeOflocation_cellID[MAX_LOCAL_CI_LEN/2];//caller_cell_ID[MAX_LOCAL_CI_LEN/2];  // 瓜柴継卞強喘薩侭侃議弌曝催

    BYTE		called_location_LAI[MAX_LOCAL_LAI_LEN/2];//called_org_LAC[MAX_LOCAL_LAC_LEN/2];    // 瓜出喘薩侭侃兜兵了崔曝催
    BYTE		called_location_cellID[MAX_LOCAL_CI_LEN/2]; //called_org_cell_ID[MAX_LOCAL_CI_LEN/2];    // 瓜出喘薩侭侃議兜兵弌曝催
    BYTE		called_changeOflocation_LAI[MAX_LOCAL_LAI_LEN/2];//called_LAC[MAX_LOCAL_LAC_LEN/2];          // 瓜出喘薩侭侃了崔曝催
    BYTE		called_changeOflocation_cellID[MAX_LOCAL_CI_LEN/2]; //called_cell_ID[MAX_LOCAL_CI_LEN/2];       // 瓜出喘薩侭侃議弌曝催

    //(MOCallRecord)(MTCallRecord)(roamingRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  燕苧乎肝匍暦葎窮三匍暦??葎＾0￣??賜覚墮匍暦??葎＾1￣??
    BYTE		ROAM_mark        :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  卆象TBS野窮三匍暦鷹賜覚匍暦鷹。
    
    //(MOCallRecord)(MTCallRecord)(roamingRecord)transparencyIndicator
    BYTE		transparencyIndicator:2;  //transparency_indicator :2;  // 邑苧掲邑苧峺幣    
    BYTE		s10          :6;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)changeOfService 喘並周三汽賜嶄寂三汽議圭塀竃

    //(MOCallRecord)(MTCallRecord)(roamingRecord)supplServicesUsed(柵出涙購議温割匍暦喘並周三汽圭塀竃,BAU栽旺)
    struct MBILL_SS_code1     SS_code1;          
    struct MBILL_SS_code2     SS_code2;          
    struct MBILL_SS_code3     SS_code3;          
    struct MBILL_SS_code4     SS_code4;          
    struct MBILL_SS_code5     SS_code5;

    //(MOCallRecord)(MTCallRecord)aocParameters/changeOfAOCParms
    AOC_E1_E7    aocParameters;  

    //(MOCallRecord)(MTCallRecord)MSClassmark[3]
    BYTE  MSClassmark[3];// 軸圻栖議struct  Classmark initial_served_ms_classmark??瓜柴継卞強喘薩兜兵CLASSMARK??struct  Classmark音憲栽亅咏個葎BYTE

    //(MOCallRecord)(MTCallRecord)changeOfClassmark[3]  
    BYTE   changeOfClassmark[3];//struct  Classmark last_served_ms_classmark;  //瓜柴継卞強喘薩個延朔議CLASSMARK

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)seizureTime 
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)answerTime
    BYTE		initial_year;           // 宥三兜兵扮震
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            
    BYTE		initial_minute;
    BYTE		initial_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)releaseTime
    BYTE		end_year; // 宥三潤崩扮寂
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callDuration
    UINT4		callDuration;//conversation_time;                 // 宥三扮海
        
    //(MOCallRecord)(MTCallRecord)radioChanUsed 
    //(MOCallRecord)(MTCallRecord)radioChanRequested  
    BYTE		radioChanRequested:4;//USER_Radio_channelUINT2ed :4;                // 1  畠堀楕賜磯堀楕    

    BYTE		radioChanUsed:2; 

    BYTE		s11    :2;    

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)causeForTerm
    BYTE		causeForTerm;//cause_for_termination;        //  宥三嶮峭圻咀

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)SDiagnostics
    SDiagnostics     diagnostics;//uDiagnostics;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; 喘噐揖匯卞強岬窟伏議音揖三暦議云仇紛艶催

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)sequenceNumber: 嶄寂三汽會双催,慧壓巷慌囃何蛍, 児云曝:sequence囃

    //(MOCallRecord)(MTCallRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // 柴継窒継炎崗    
    BYTE		s12         :4;

    //(MOCallRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 継楕峺幣
    UINT2		money_per_count; // 継楕
    UINT2		add_fee;                 //現紗継

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)chargedParty 朕念短嗤野亟??枠圓藻曳蒙了
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)networkCallReference
    //輝聞喘CAMEL匍暦扮,揖匯三暦侭将議光利圷恢伏議三汽繍根嗤緩炎紛鷹(NCR),喘參委宸乂三汽購選欺匯軟. 
    BYTE		NetworkCallReferenceLen;
    BYTE		NetworkcallReferenceNumber[maxCallReferenceNumberLength]; // 喇NetworkCallReferenceLen峺苧海業(2G症嗤議柵出歌深催)

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mSCAddress (効NetworkCallReferenceLen塘栽聞喘)
    BYTE		MSCAddress_npi : 4;
    BYTE		MSCAddress_nai :  3;
    BYTE		s13            : 1;    
    BYTE		MSCAddress_BCDlen;
    BYTE		MSCAddress[MAX_MSC_NUM_IN_BYTES];

    //(MOCallRecord)(MTCallRecord)fnur
    BYTE		fnur;//ucFunr;           //方象匍暦喘薩堀楕,enum FNUR

    //(MOCallRecord)(MTCallRecord)aiurRequested
    BYTE		aiurRequested;//BYTE SMSResult;(B03嶄囃兆聞喘議頁SMSResult徽根吶頁aiurRequested??R02個兆)

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
    UINT4		ServiceKey;                //匍暦囚

    //(MOCallRecord)(MTCallRecord)rateIndication
    BYTE		rateIndication;//ucRateIndication; //方象匍暦堀楕癖塘

     //(MOCallRecord)(MTCallRecord)systemType
    BYTE		systemType:2;//USER_NETWORK_TYPE :4; 

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)freeFormatDataAppend
    BYTE		freeFormatDataAppend:2;//CAMEL_FCI_Data_Append_Ind :2; //燕幣頁及匯倖FCI??連??0燕幣音頁及匯倖

      //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)defaultCallHandling
    BYTE		defaultCallHandling:2;//CAMEL_default_Call_Handling;   
    BYTE		s15       :2;   
   

    //(MOCallRecord)(MTCallRecord)callingChargeAreaCode 朕念短嗤野亟??圓藻//密忽蒙歩囃    BYTE    CAC_num[MAX_CAC_LEN/2+1];
    BYTE		callingChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];

    //(MOCallRecord)(MTCallRecord)calledChargeAreaCode 朕念短嗤野亟??圓藻//密忽蒙歩囃    BYTE    CAC_num[MAX_CAC_LEN/2+1];
    BYTE		calledChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)numberOfDPEncountered
    BYTE		numberOfDPEncountered;//CAMEL_num_of_DP;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)levelOfCAMELService
    BYTE		levelOfCAMELService;//CAMEL_service_level;

    //(MTCallRecord)(roamingRecord)mscIncomingCircuit
    UINT2		mscOutgoingCircuit;
    UINT2		mscIncomingCircuit;

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId海業峺幣
    BYTE		msc_id_len:3;//orgMSCId海業峺幣
    BYTE		s23            : 2;

    //(MOCallRecord)(MTCallRecord)orgRNCorBSCId
    BYTE		orgRNCorBSCId[MAX_RNCBSC_ID_LEN];//Rnc_Bsc_Id[3];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(MOCallRecord)(MTCallRecord)callEmlppPriority
    BYTE		callEmlppPriority:4;//CallEmlpp;//柵出恷嶮EMLPP單枠雫 ??函峙0-7)

    //(MTCallRecord)calledDefaultEmlppPriority
    BYTE		calledDefaultEmlppPriority:4;//calledDefaultEmlpp;//瓜出喘薩禰埃潮範EMLPP單枠雫??函峙0-7)

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

    //(MOCallRecord)(MTCallRecord)hotBillingTag慧壓児云曝HotBillingTag

    //(MTCallRecord)redirectingcounter 
    BYTE		CFW_invoked_times:4;       // 念廬窟伏議肝方    

    //(MOCallRecord)(MTCallRecord)recordExtensions
    BYTE		CH_invoked_times:4;        // 柵出隠隔匍暦窟伏議肝方
    BYTE		CH_retrieve_times:4;       // 柵出隠隔匍暦志鹸議肝方    
    BYTE		CW_invoked_times:4;        // 柵出吉棋匍暦窟伏議肝方
    BYTE		MPTY_Build_times:4;        // 謹圭宥三匍暦秀羨窟伏議肝方    
    BYTE		MPTY_CH_times:4;           // 謹圭宥三匍暦隠隔窟伏議肝方
    BYTE		MPTY_CH_retrieve_times:4;   // 謹圭宥三匍暦隠隔志鹸窟伏議肝方
    BYTE		MPTY_Split_times:4;        // 謹圭宥三匍暦侯宣窟伏議肝方

    BYTE		HO_invoked_times;         // 俳算窟伏議肝方
    
    //BYTE     ServiceChangedTimes:4;

    BYTE		AOCParmsChangedTimes:4;    
    BYTE		ClassmarkChangedTimes:4;    
    BYTE		RadioChanChangedTimes:4;
    BYTE		transmission_mode:4;   // 2G三汽聞喘議  畠堀楕賜磯堀楕 炎崗。

    BYTE		Info_trans_capability;             // 覚墮嬬薦

    struct  BillClassmark3 Classmark3Information;    

    BYTE		DTMF_indicator :2;  // 頁倦聞喘DTMF
    BYTE		user_type :5;  //   卞強岬單枠雫    
    BYTE		s9 :1;  //   0 燕幣云仇喘薩??1 燕幣只嗄喘薩。
    BYTE		initialCallAttemptFlag :1;	
    BYTE		reserved :1; 
    BYTE		SORIndication:1;    
    BYTE		UssdCallBackIndication:1;// 峪壓CAMEL MTC三汽嶄嘉芝村
    BYTE		ucTcsiVtcsiFlag : 4;//0葎TCSI??1葎VTCSI,0xff涙丼 
	
    //葎徭強襖蒙楕Autobauding Support仟奐
    BYTE		ModemType:7;    
    BYTE		Classmark3Indication:1;
    
    BYTE		cause_for_partial_record;//cause_for_partial_record ;      //  竃嶄寂三汽圻咀    
    BYTE		transaction_id;// 並暦ID,喘噐揖匯卞強岬窟伏議音揖三暦議云仇紛艶催
    BYTE		gsm_gsvn;                          // 1匍暦窃艶
    BYTE		B_ch_number;       // 乎肝柵出侭媼喘議B佚祇方

    // GSM 蒙歩囃
    BYTE		scfID[MAX_SCFID_LEN];      //SCF沫哈催

    //  輝念瓜柴継卞強喘薩侭侃MSC
    BYTE		served_msc_num_npi : 4;
    BYTE		served_msc_num_nai :  3;
    BYTE		s18            : 1;
    BYTE		served_msc_num_BCDlen ;
    BYTE		served_msc_num[MAX_MSC_NUM_IN_BYTES];       

    //方象匍暦曳蒙楕協吶
    BYTE     	GuaranteedBitRate;//隠屬曳蒙楕
    BYTE    	MaximumBitRate;//恷寄曳蒙楕
    
    //仟奐否匂喘恬朔偬制婢
    BYTE		container[MAX_CONTAINER_MTC_LEN];

    // FCI 慧壓辛延何蛍
    BYTE		fcidata_len;
    BYTE        VarPart[MAX_TRANSFER_FCI_LEN];
    BYTE		FCIData[MAX_TRANSFER_FCI_LEN];
	
};

//1.3	TCAMEL_Bill_Report?┛?凄TCAMEL ??TermCAMELRecord????
struct TCAMEL_Bill_Report
{
	// 巷慌囃何蛍, 児云曝:
	BYTE		validflag; // 嗤丼來炎崗,　＾AA￣葎嗤丼??＾55￣葎涙丼
    UINT2		size; // 云嫖三汽海業
    UINT2		CRCCode; // CRC丕刮潤惚
    BYTE		bill_type; //  enum   GSM_BILL_TYPE, 亅咏嶄議recordType囃
    UINT4		bill_sequence; // 三汽送邦催??率匯曝蛍匯嫖三汽。
    BYTE		ModuleNum; // 恢伏三汽議庁翠催
    UINT4		count; // GSM 柴継輝匯肝柵出恢伏謹嫖三汽扮count 頁乎肝柵出遍三汽送邦催
    BYTE		sequence; // 汽嫖三汽葎0??遍嫖三汽葎1??凪噫窃容
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 汽嫖遍嫖嶄寂挑嫖三汽炎紛
    BYTE		HotBillingTag:1; //葎cmcc霞編奐紗議犯柴継炎崗
    BYTE		CRC_Way:2; // CRC丕刮圭塀
    BYTE		ChargeLevel:1;//柴継娼業峺幣:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//巷慌囃何蛍議制婢腎寂
    BYTE		Comm_reserved;//巷慌囃何蛍議制婢腎寂
	
	//  制婢曝:

	BYTE		servedIMSI[MAX_IMSI_LEN];  //  瓜柴継卞強喘薩IMSI鷹

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  瓜柴継卞強喘薩卞強岬譜姥紛艶鷹

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // 瓜柴継卞強喘薩MSISDN催
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callingNumber
    BYTE		callingNumber_npi  :4;                 // 麼出催鷹柴皿
    BYTE		callingNumber_nai  :3;                 // 麼出催鷹窃侏   
    BYTE		s2               :1;    
    BYTE		callingNumber_BCDlen ;                 // 麼出催鷹海業
    BYTE		callingNumber[MAX_NUM_LEN/2];     // 麼出催鷹

    //(MOCallRecord)(TermCAMELRecord)calledNumber
    BYTE		calledNumber_npi  :4;//dialed_isdn_npi  :4;                 // 瓜出催鷹柴皿
    BYTE		calledNumber_nai  :3;//dialed_isdn_nai  :3;                 // 瓜出催鷹窃侏    
    BYTE		s3               :1;    
    BYTE		calledNumber_BCDlen ;//dialed_isdn_num  :5;                 // 瓜出催鷹海業
    BYTE		calledNumber[MAX_NUM_LEN/2];  //dialed_num[MAX_NUM_LEN/2];     // 瓜出催鷹

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;   
    BYTE		s4            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  云蕉MSC催

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
    UINT4		ServiceKey;                //匍暦囚

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)networkCallReference
    //輝聞喘CAMEL匍暦扮,揖匯三暦侭将議光利圷恢伏議三汽繍根嗤緩炎紛鷹(NCR),喘參委宸乂三汽購選欺匯軟. 
    BYTE		NetworkCallReferenceLen;
    BYTE		NetworkcallReferenceNumber[maxCallReferenceNumberLength]; // 喇NetworkCallReferenceLen峺苧海業(2G症嗤議柵出歌深催)

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mSCAddress (効NetworkCallReferenceLen塘栽聞喘)
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
    UINT2		mscIncomingROUTE_ROUTENumber;//IT_trunk_group;                     // 秘嶄写蛤
    UINT2		mscOutgoingROUTE_ROUTENumber;//OT_trunk_group;                    // 竃嶄写蛤
    BYTE		mscIncomingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//IT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];
    BYTE		mscOutgoingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//OT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)seizureTime 
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)answerTime
    BYTE		initial_year;           // 宥三兜兵扮震
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            //3
    BYTE		initial_minute;
    BYTE		initial_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)releaseTime
    BYTE		end_year; // 宥三潤崩扮寂
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callDuration
    UINT4		callDuration;//conversation_time;                 // 宥三扮海

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)causeForTerm
    BYTE		causeForTerm;//cause_for_termination;        //  宥三嶮峭圻咀

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)SDiagnostics
    SDiagnostics     diagnostics;//uDiagnostics;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; 喘噐揖匯卞強岬窟伏議音揖三暦議云仇紛艶催

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)numberOfDPEncountered
    BYTE		numberOfDPEncountered;//CAMEL_num_of_DP;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)levelOfCAMELService
    BYTE		levelOfCAMELService;//CAMEL_service_level;


    MBILL_CHANGEFLAGS     CAMELModificationChangeFlags;

	BYTE		ModifiedCallingPartyNumber_npi  :4;                 // 催鷹柴皿
    BYTE		ModifiedCallingPartyNumber_nai  :3;                 // 催鷹窃侏   
    BYTE		s9               :1;    
    BYTE		ModifiedCallingPartyNumber_BCDlen;                 // 催鷹海業
    BYTE		ModifiedCallingPartyNumber[MAX_NUM_LEN/2];     // 個延朔議麼出催鷹

    BYTE		ModifiedCallingPartyCategory;// 俐個朔議麼出喘薩窃艶

    BYTE		ModifiedOriginalCalledPartyNumber_npi  :4;                 // 催鷹柴皿
    BYTE		ModifiedOriginalCalledPartyNumber_nai  :3;                 // 催鷹窃侏   
    BYTE		s10               :1;       
    BYTE		ModifiedOriginalCalledPartyNumber_BCDlen;                 // 催鷹海業
    BYTE		ModifiedOriginalCalledPartyNumber[MAX_NUM_LEN/2]; // 個延朔議圻瓜出催鷹

    BYTE		ModifiedGenericNumber_npi  :4;                 // 催鷹柴皿
    BYTE		ModifiedGenericNumber_nai  :3;                 // 催鷹窃侏 
    BYTE		s11               :1;    
    BYTE		ModifiedGenericNumber_BCDlen;                 // 催鷹海業
    BYTE		ModifiedGenericNumber[MAX_NUM_LEN/2];   // 個延朔議GenericNumber,歌需CapEncode_TGenericNumbers , TGenericNumbers

    BYTE		ModifiedRedirectingPartyNumber_npi  :4;                 // 催鷹柴皿
    BYTE		ModifiedRedirectingPartyNumber_nai  :3;                 // 催鷹窃侏
    BYTE		s12               :1;    
    BYTE		ModifiedRedirectingPartyNumber_BCDlen;                 // 催鷹海業
    BYTE		ModifiedRedirectingPartyNumber[MAX_NUM_LEN/2];     // 個延朔議RedirectingPartyNumber
    
    BYTE		ModifiedRedirectionCounter; 

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)freeFormatDataAppend
    BYTE		freeFormatDataAppend:2;//CAMEL_FCI_Data_Append_Ind :2; //燕幣頁及匯倖FCI??連??0燕幣音頁及匯倖  

    //(TermCAMELRecord)mscServerIndication//朕念短嗤野亟??圓藻曳蒙了
    BYTE		mscServerIndication:1;    

    //(MOCallRecord)(TermCAMELRecord)defaultCallHandling-2 
    BYTE		defaultCallHandling2:2 ;//CAMEL_default_Call_Handling2;   
    BYTE		s13       :3;    
      
    //(MOCallRecord)(TermCAMELRecord)gsm-SCFAddress-2慧壓辛延何蛍
    BYTE		CAMEL_GsmSCF_Address2_npi : 4;
    BYTE		CAMEL_GsmSCF_Address2_nai :  3; 
    BYTE		s14            : 1;   
    BYTE		CAMEL_GsmSCF_Address2_BCDlen;
    BYTE		CAMEL_GsmSCF_Address2[MAX_SCF_LEN]; 

    //(MOCallRecord)(TermCAMELRecord)serviceKey-2慧壓辛延何蛍
    UINT4		ServiceKey2;                //匍暦囚

    //(MOCallRecord)(TermCAMELRecord)freeFormatData-2慧壓恷朔海業辛延

    //(MOCallRecord)(TermCAMELRecord)freeFormatDataAppend-2
    BYTE		freeFormatDataAppend2:2;//CAMEL_FCI_Data_Append_Ind2 :2;    
    BYTE		s15         :6;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)(TermCAMELRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  燕苧乎肝匍暦葎窮三匍暦??葎＾0￣??賜覚墮匍暦??葎＾1￣??   
    BYTE		ROAM_mark       :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  卆象TBS野窮三匍暦鷹賜覚匍暦鷹。
    
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // 柴継窒継炎崗   
	BYTE		s17             :4;
    
    //(MOCallRecord)(TermCAMELRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 継楕峺幣
    UINT2		money_per_count; // 継楕
    UINT2		add_fee;                 //現紗継

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)chargedParty 朕念短嗤野亟??枠圓藻曳蒙了
    BYTE        chargedParty; //enum MBILL_CHARGED_PARTY
    
    BYTE        UssdCallBackIndication:1;// ucb柵出議寔糞麼出嘉芝村緩囃
    BYTE        msc_id_len:3;//orgMSCId海業峺幣
    BYTE        initialCallAttemptFlag:1;//ICA柵出炎崗
    BYTE        s18         :3;
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)originalCalledNumber 朕念短嗤野亟??圓藻
    BYTE		originalCalledNumber_npi  :4;                 // 催鷹柴皿    
    BYTE		originalCalledNumber_nai  :3;                 // 催鷹窃侏
    BYTE		s19               :1;    
    BYTE		originalCalledNumber_BCDlen;                 // 催鷹海業
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;

    BYTE		cause_for_partial_record;//cause_for_partial_record ;      //  竃嶄寂三汽圻咀

    //仟奐否匂喘恬朔偬制婢
    BYTE		container[MAX_CONTAINER_TCAMEL_LEN];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)freeFormatData FCI慧壓辛延何蛍
    //(MOCallRecord)(TermCAMELRecord)freeFormatData-2 FCI慧壓辛延何蛍
    BYTE		fcidata_len;
    BYTE		fcidata2_len;
    BYTE     VarPart[MAX_TRANSFER_FCI_LEN * 2];
    
	// BYTE		FCIData[MAX_TRANSFER_FCI_LEN];
    // BYTE		FCIData2[MAX_TRANSFER_FCI_LEN];

	
};

//1.4	ROAM_Bill_Report?┛?凄ROAM??roamingRecord????
struct ROAM_Bill_Report
{
	// 巷慌囃何蛍, 児云曝:

    BYTE		validflag; // 嗤丼來炎崗,　＾AA￣葎嗤丼??＾55￣葎涙丼
    UINT2		size; // 云嫖三汽海業
    UINT2		CRCCode; // CRC丕刮潤惚
    BYTE		bill_type; //  enum   GSM_BILL_TYPE, 亅咏嶄議recordType囃
    UINT4		bill_sequence; // 三汽送邦催??率匯曝蛍匯嫖三汽。
    BYTE		ModuleNum; // 恢伏三汽議庁翠催
    UINT4		count; // GSM 柴継輝匯肝柵出恢伏謹嫖三汽扮count 頁乎肝柵出遍三汽送邦催
    BYTE		sequence; // 汽嫖三汽葎0??遍嫖三汽葎1??凪噫窃容
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 汽嫖遍嫖嶄寂挑嫖三汽炎紛
    BYTE		HotBillingTag:1; //葎cmcc霞編奐紗議犯柴継炎崗
    BYTE		CRC_Way:2; // CRC丕刮圭塀
    BYTE		ChargeLevel:1;//柴継娼業峺幣:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//巷慌囃何蛍議制婢腎寂
    BYTE		Comm_reserved;//巷慌囃何蛍議制婢腎寂
	
	//  制婢曝:
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  瓜柴継卞強喘薩IMSI鷹

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // 瓜柴継卞強喘薩MSISDN催
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callingNumber
    BYTE		callingNumber_npi  :4;                 // 麼出催鷹柴皿
    BYTE		callingNumber_nai  :3;                 // 麼出催鷹窃侏
    BYTE		s2               :1;    
    BYTE		callingNumber_BCDlen ;                 // 麼出催鷹海業
    BYTE		callingNumber[MAX_NUM_LEN/2];     // 麼出催鷹

    //(MOCallRecord)(MTCallRecord)(roamingRecord)roamingNumber
    BYTE		roamingNumber_npi : 4;
    BYTE		roamingNumber_nai :  3;    
    BYTE		s3            : 1;    
    BYTE		roamingNumber_BCDlen ;
    BYTE		roamingNumber[MAX_NUM_LEN/2] ;    //MSRN               //  卞強瓜出只嗄催鷹

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;    
    BYTE		s4            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  云蕉MSC催

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mscIncomingROUTE/mscOutgoingROUTE
    BYTE		mscIncomingROUTE_ind : 4;//IT_trunk_group_ind : 4;    
    BYTE		mscOutgoingROUTE_ind : 4;//OT_trunk_group_ind : 4;    
    UINT2		mscIncomingROUTE_ROUTENumber;//IT_trunk_group;                     // 秘嶄写蛤
    UINT2		mscOutgoingROUTE_ROUTENumber;//OT_trunk_group;                    // 竃嶄写蛤
    BYTE		mscIncomingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//IT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];
    BYTE		mscOutgoingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//OT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];

    //(MOCallRecord)(MTCallRecord)(roamingRecord)(TermCAMELRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  燕苧乎肝匍暦葎窮三匍暦??葎＾0￣??賜覚墮匍暦??葎＾1￣??    
    BYTE		ROAM_mark          :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  卆象TBS野窮三匍暦鷹賜覚匍暦鷹。

    BYTE		transparencyIndicator:2;  //transparency_indicator :2;  // 邑苧掲邑苧峺幣   
    
    //(MOCallRecord)(MTCallRecord)(roamingRecord)transparencyIndicator
	BYTE        SORIndication	:1;//單僉揃喇炎崗 
    BYTE		s6          :5;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)changeOfService 喘並周三汽賜嶄寂三汽議圭塀竃

    //(MOCallRecord)(MTCallRecord)(roamingRecord)supplServicesUsed(柵出涙購議温割匍暦喘並周三汽圭塀竃,BAU栽旺)
    struct MBILL_SS_code1     SS_code1;          
    struct MBILL_SS_code2     SS_code2;          
    struct MBILL_SS_code3     SS_code3;          
    struct MBILL_SS_code4     SS_code4;          
    struct MBILL_SS_code5     SS_code5;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)seizureTime 
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)answerTime
    BYTE		initial_year;           // 宥三兜兵扮震
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            //3
    BYTE		initial_minute;
    BYTE		initial_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)releaseTime
    BYTE		end_year; // 宥三潤崩扮寂
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callDuration
    UINT4		callDuration;//conversation_time;                 // 宥三扮海

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)causeForTerm
    BYTE		causeForTerm;//cause_for_termination;        //  宥三嶮峭圻咀

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)SDiagnostics
    SDiagnostics     diagnostics;//uDiagnostics;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; 喘噐揖匯卞強岬窟伏議音揖三暦議云仇紛艶催

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)sequenceNumber: 嶄寂三汽會双催,慧壓巷慌囃何蛍, 児云曝:sequence囃

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)networkCallReference
    //輝聞喘CAMEL匍暦扮,揖匯三暦侭将議光利圷恢伏議三汽繍根嗤緩炎紛鷹(NCR),喘參委宸乂三汽購選欺匯軟. 
    BYTE		NetworkCallReferenceLen;
    BYTE		NetworkcallReferenceNumber[maxCallReferenceNumberLength]; // 喇NetworkCallReferenceLen峺苧海業(2G症嗤議柵出歌深催)

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mSCAddress (効NetworkCallReferenceLen塘栽聞喘)
    BYTE		MSCAddress_npi : 4;
    BYTE		MSCAddress_nai :  3;
    BYTE		s7            : 1;   
    BYTE		MSCAddress_BCDlen;
    BYTE		MSCAddress[MAX_MSC_NUM_IN_BYTES];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // 柴継窒継炎崗
    BYTE		callEmlppPriority   :4;
    
    //(MOCallRecord)(TermCAMELRecord)(roamingRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 継楕峺幣
    UINT2		money_per_count; // 継楕
    UINT2		add_fee;                 //現紗継

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)chargedParty 朕念短嗤野亟??枠圓藻曳蒙了
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)originalCalledNumber 朕念短嗤野亟??圓藻
    BYTE		originalCalledNumber_npi  :4;                 // 催鷹柴皿
    BYTE		originalCalledNumber_nai  :3;                 // 催鷹窃侏    
    BYTE		s9               :1;    
    BYTE		originalCalledNumber_BCDlen;                 // 催鷹海業
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

    //(MOCallRecord)(MTCallRecord)(roamingRecord)callingChargeAreaCode 朕念短嗤野亟??圓藻//密忽蒙歩囃    BYTE    CAC_num[MAX_CAC_LEN/2+1];
    BYTE		callingChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];

    //(MOCallRecord)(MTCallRecord)(roamingRecord)calledChargeAreaCode 朕念短嗤野亟??圓藻//密忽蒙歩囃    BYTE    CAC_num[MAX_CAC_LEN/2+1];
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
    BYTE		msc_id_len:3;//orgMSCId海業峺幣
    BYTE		s10          :1;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)cUGInterlockCode
    UINT4		CUGInterlockCode;//CUGInterlockCode;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)cUGIndex
    UINT2		CUGIndex;

    BYTE		cause_for_partial_record;//cause_for_partial_record ;      //  竃嶄寂三汽圻咀

    BYTE        eaSubscriberInfo[MAX_NAEA_CODE_IN_BYTE + 1];
    BYTE        selectedCIC[MAX_NAEA_CODE_IN_BYTE + 1];
    //仟奐否匂喘恬朔偬制婢
    BYTE     container[MAX_CONTAINER_ROAM_LEN];
	
};

//1.5	Gateway_Bill_Report?┛?凄GWO(outGatewayRecord)、GWI(incGatewayRecord)、Transit(TransitCallRecord)??
struct Gateway_Bill_Report
{
	// 巷慌囃何蛍, 児云曝:
	
    BYTE		validflag; // 嗤丼來炎崗,　＾AA￣葎嗤丼??＾55￣葎涙丼
    UINT2		size; // 云嫖三汽海業
    UINT2		CRCCode; // CRC丕刮潤惚
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // 三汽送邦催??率匯曝蛍匯嫖三汽。
    BYTE		ModuleNum; // 恢伏三汽議庁翠催
    UINT4		count; // GSM 柴継輝匯肝柵出恢伏謹嫖三汽扮count 頁乎肝柵出遍三汽送邦催
    BYTE		sequence; // 汽嫖三汽葎0??遍嫖三汽葎1??凪噫窃容
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 汽嫖遍嫖嶄寂挑嫖三汽炎紛
    BYTE		HotBillingTag:1; //葎cmcc霞編奐紗議犯柴継炎崗
    BYTE		CRC_Way:2; // CRC丕刮圭塀
    BYTE		ChargeLevel:1;//柴継娼業峺幣:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//巷慌囃何蛍議制婢腎寂
    BYTE		Comm_reserved;//巷慌囃何蛍議制婢腎寂	
	
	//  制婢曝:
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  瓜柴継卞強喘薩IMSI鷹

    //servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  瓜柴継卞強喘薩卞強岬譜姥紛艶鷹

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)callingNumber
    BYTE		callingNumber_npi  :4;                 // 麼出催鷹柴皿
    BYTE		callingNumber_nai  :3;                 // 麼出催鷹窃侏 
    BYTE		s1               :1;    
    BYTE		callingNumber_BCDlen ;                 // 麼出催鷹海業
    BYTE		callingNumber[MAX_NUM_LEN/2];     // 麼出催鷹

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)calledNumber
    BYTE		calledNumber_npi  :4;//dialed_isdn_npi  :4;                 // 瓜出催鷹柴皿
    BYTE		calledNumber_nai  :3;//dialed_isdn_nai  :3;                 // 瓜出催鷹窃侏 
    BYTE		s2               :1;  
    BYTE		calledNumber_BCDlen ;//dialed_isdn_num  :5;                 // 瓜出催鷹海業
    BYTE		calledNumber[MAX_NUM_LEN/2];  //dialed_num[MAX_NUM_LEN/2];     // 瓜出催鷹
    
    //(incGatewayRecord)(OutGatewayRecord)roamingNumber
    BYTE		roamingNumber_npi : 4;
    BYTE		roamingNumber_nai :  3;   
    BYTE		s3            : 1;   
    BYTE		roamingNumber_BCDlen ;
    BYTE		roamingNumber[MAX_NUM_LEN/2] ;    //MSRN               //  卞強瓜出只嗄催鷹

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;    
    BYTE		s4            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  云蕉MSC催

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)mscIncomingROUTE/mscOutgoingROUTE
    BYTE		mscIncomingROUTE_ind : 4;//IT_trunk_group_ind : 4;    
    BYTE		mscOutgoingROUTE_ind : 4;//OT_trunk_group_ind : 4;    
    UINT2		mscIncomingROUTE_ROUTENumber;//IT_trunk_group;                     // 秘嶄写蛤
    UINT2		mscOutgoingROUTE_ROUTENumber;//OT_trunk_group;                    // 竃嶄写蛤
    BYTE		mscIncomingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//IT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];
    BYTE		mscOutgoingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//OT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)seizureTime 
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)answerTime
    BYTE		initial_year; // 宥三兜兵扮震
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            //3
    BYTE		initial_minute;
    BYTE		initial_second;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)releaseTime
    BYTE		end_year; // 宥三潤崩扮寂
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)callDuration
    UINT4		callDuration;//conversation_time;                 // 宥三扮海

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)causeForTerm
    BYTE		causeForTerm;//cause_for_termination;        //  宥三嶮峭圻咀

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)SDiagnostics
    SDiagnostics     diagnostics;//uDiagnostics;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; 喘噐揖匯卞強岬窟伏議音揖三暦議云仇紛艶催

    //(MOCallRecord)(MTCallRecord)(roamingRecord)(TermCAMELRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  燕苧乎肝匍暦葎窮三匍暦??葎＾0￣??賜覚墮匍暦??葎＾1￣??   
    BYTE		s5       :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  卆象TBS野窮三匍暦鷹賜覚匍暦鷹。
    
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)sequenceNumber: 嶄寂三汽會双催,慧壓巷慌囃何蛍, 児云曝:sequence囃

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // 柴継窒継炎崗   
    BYTE		s6          :4;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 継楕峺幣
    UINT2		money_per_count; // 継楕
    UINT2		add_fee;                 //現紗継

    //(incGatewayRecord)(OutGatewayRecord)chargedParty 朕念短嗤野亟??枠圓藻曳蒙了
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)originalCalledNumber 朕念短嗤野亟??圓藻
    BYTE		originalCalledNumber_npi  :4;                 // 催鷹柴皿
    BYTE		originalCalledNumber_nai  :3;                 // 催鷹窃侏  
    BYTE		s7               :1;   
    BYTE		originalCalledNumber_BCDlen;                 // 催鷹海業
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

    //(OutGatewayRecord)(TransitCallRecord)mscOutgoingCircuit
    UINT2		mscOutgoingCircuit;

    //(incGatewayRecord)(TransitCallRecord)mscIncomingCircuit
    UINT2		mscIncomingCircuit;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)callEmlppPriority
    BYTE		callEmlppPriority:4;//CallEmlpp;//柵出恷嶮EMLPP單枠雫 ??函峙0-7)
    BYTE		reserved:4;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)eaSubscriberInfo 朕念短嗤野亟??圓藻
    BYTE		eaSubscriberInfo[MAX_NAEA_CODE_IN_BYTE + 1];
    
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)selectedCIC 朕念短嗤野亟??圓藻
    BYTE		selectedCIC[MAX_CIC_LEN_IN_BYTE];

     //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)cUGOutgoingAccessIndicator
    BYTE		CUGOutgoingAccessIndicator:2;//CUG_call_ind         :2; 
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)cUGOutgoingAccessUsed
    BYTE		CUGOutgoingAccessUsed:2;//CUGOutgoingAccessUsed   :2;
    BYTE		msc_id_len:3;//orgMSCId海業峺幣
    BYTE		s8        :1;    

   
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)cUGInterlockCode
    UINT4		CUGInterlockCode;//CUGInterlockCode;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)cUGIndex
    UINT2		CUGIndex;

    //recordExtensions
    BYTE		cause_for_partial_record ;      //  竃嶄寂三汽圻咀
    BYTE		gsm_gsvn;                          // 1匍暦窃艶
    BYTE		SORIndication:1;
    BYTE		transparencyIndicator:2;   //邑苧掲邑苧峺幣    

    BYTE		PromptAndCollectCharge:1;
    BYTE		transmission_mode:4;  // 2G三汽聞喘議畠堀楕磯堀楕炎崗

    BYTE		subscriberCategory;

    BYTE        isdn_bc_len;
    BYTE        isdn_bc[MAX_ISDN_BC_LEN];
    BYTE        LLC_len;
    BYTE        LLC[MAX_LLC_LEN];
    BYTE        HLC_len;
    BYTE        HLC[MAX_HLC_LEN];

    
    //仟奐否匂喘恬朔偬制婢
    BYTE		container[MAX_CONTAINER_GATEWAY_LEN];
	
};

//1.6	SMSMT_Bill_Report?┛?凄SMS MT(MTSMSRecord)??
struct SMSMT_Bill_Report
{
	// 巷慌囃何蛍, 児云曝:

	BYTE		validflag; // 嗤丼來炎崗,　＾AA￣葎嗤丼??＾55￣葎涙丼
    UINT2		size; // 云嫖三汽海業
    UINT2		CRCCode; // CRC丕刮潤惚
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // 三汽送邦催??率匯曝蛍匯嫖三汽。
    BYTE		ModuleNum; // 恢伏三汽議庁翠催
    UINT4		count; // GSM 柴継輝匯肝柵出恢伏謹嫖三汽扮count 頁乎肝柵出遍三汽送邦催
    BYTE		sequence; // 汽嫖三汽葎0??遍嫖三汽葎1??凪噫窃容
    BYTE		record_type :3; //enum GSM_RECORD_TY
    BYTE		HotBillingTag:1; //葎cmcc霞編奐紗議犯柴継炎崗
    BYTE		CRC_Way:2; // CRC丕刮圭塀
    BYTE		ChargeLevel:1;//柴継娼業峺幣:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//巷慌囃何蛍議制婢腎寂
	BYTE		Comm_reserved;//巷慌囃何蛍議制婢腎寂
	
	//  制婢曝:	
    BYTE		serviceCentre_npi : 4;//SMS_address_npi : 4;
    BYTE		serviceCentre_nai :  3;//SMS_address_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		serviceCentre_BCDlen;//SMS_address_num : 5;
    BYTE		serviceCentre[MAX_MSC_NUM_IN_BYTES];  //SMS_address[8];    //玉??連嶄伉仇峽
    
    //(MOSMSRecord)(MTSMSRecord)servedIMSI
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  瓜柴継卞強喘薩IMSI鷹

    //(MOSMSRecord)(MTSMSRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  瓜柴継卞強喘薩卞強岬譜姥紛艶鷹

    //(MOSMSRecord)(MTSMSRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s2            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // 瓜柴継卞強喘薩MSISDN催
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MOSMSRecord)(MTSMSRecord)MSClassmark[3] 
    BYTE   MSClassmark[3];//struct  Classmark last_served_ms_classmark;      // 瓜柴継卞強喘薩輝念CLASSMARK
    BillClassmark3 Classmark3Information;
    
    //(MOSMSRecord)(MTSMSRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;    
    BYTE		s3            : 1;   
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  云蕉MSC催

    //(MOSMSRecord)(MTSMSRecord)location 麼瓜出議緩囃栽匯
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
    SDiagnostics  smsResult;//歌深map協吶議SMSResult
    BYTE		actionResult;//荷恬潤惚??函SM_transmission_success,賜 SM_transmission_failed 

    //(MOSMSRecord)(MTSMSRecord)systemType
    BYTE		systemType:2;//USER_NETWORK_TYPE :4;     
    BYTE		s4        :6;
    
    //(MOSMSRecord)(MTSMSRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  燕苧乎肝匍暦葎窮三匍暦??葎＾0￣??賜覚墮匍暦??葎＾1￣??    
    BYTE		ROAM_mark         :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  卆象TBS野窮三匍暦鷹賜覚匍暦鷹。
    
    //(MOSMSRecord)(MTSMSRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // 柴継窒継炎崗
    BYTE		Classmark3Indication:1;
    BYTE		s6           :3;
    
    //(MOSMSRecord)(MTSMSRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 継楕峺幣
    UINT2		money_per_count; // 継楕
    UINT2		add_fee;                 //現紗継

    //(MOSMSRecord)(MTSMSRecord)chargedParty  朕念短嗤野亟??枠圓藻曳蒙了
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId海業峺幣
    BYTE		msc_id_len:3;//orgMSCId海業峺幣
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

    //(MOSMSRecord)(MTSMSRecord)hotBillingTag慧壓児云曝HotBillingTag

    //(MTSMSRecord)origination
    BYTE		callingNumber_npi  :4; //caller_isdn_npi                // 麼出催鷹柴皿
    BYTE		callingNumber_nai  :3;  //caller_isdn_nai               // 麼出催鷹窃侏
    BYTE		s7               :1;    
    BYTE		callingNumber_BCDlen ; //caller_isdn_num                // 麼出催鷹海業
    BYTE		callingNumber[MAX_NUM_LEN/2]; //caller_num    // 麼出催鷹

    //(MOSMSRecord)(MTSMSRecord)recordExtensions
    //  輝念瓜柴継卞強喘薩侭侃MSC
    BYTE		served_msc_num_npi : 4;
    BYTE		served_msc_num_nai :  3;
    BYTE		s8            : 1;
    BYTE		served_msc_num_BCDlen ;
	BYTE		served_msc_num[MAX_MSC_NUM_IN_BYTES];       

    //仟奐否匂喘恬朔偬制婢
    BYTE		container[MAX_CONTAINER_SMSMT_LEN];
    
    //(MOSMSRecord)(MTSMSRecord)smstext 
    BYTE		smstext_len;
    BYTE     	VarPart[MAX_SMSTEXT_LEN];
    // BYTE		smstext[MAX_SMSTEXT_LEN];

	
};

//1.7	SMSMO_Bill_Report?┛?凄SMS MO(MOSMSRecord)??
struct SMSMO_Bill_Report
{
	// 巷慌囃何蛍, 児云曝:

	BYTE		validflag; // 嗤丼來炎崗,　＾AA￣葎嗤丼??＾55￣葎涙丼
    UINT2		size; // 云嫖三汽海業
    UINT2		CRCCode; // CRC丕刮潤惚
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // 三汽送邦催??率匯曝蛍匯嫖三汽。
    BYTE		ModuleNum; // 恢伏三汽議庁翠催
    UINT4		count; // GSM 柴継輝匯肝柵出恢伏謹嫖三汽扮count 頁乎肝柵出遍三汽送邦催
    BYTE		sequence; // 汽嫖三汽葎0??遍嫖三汽葎1??凪噫窃容
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 汽嫖遍嫖嶄寂挑嫖三汽炎紛
    BYTE		HotBillingTag:1; //葎cmcc霞編奐紗議犯柴継炎崗
    BYTE		CRC_Way:2; // CRC丕刮圭塀
    BYTE		ChargeLevel:1;//柴継娼業峺幣:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//巷慌囃何蛍議制婢腎寂
    BYTE		Comm_reserved;//巷慌囃何蛍議制婢腎寂
	
	//  制婢曝:
    BYTE		serviceCentre_npi : 4;//SMS_address_npi : 4;
    BYTE		serviceCentre_nai :  3;//SMS_address_nai :  3;   
	BYTE		s1            : 1;    
    BYTE		serviceCentre_BCDlen;//SMS_address_num : 5;
    BYTE		serviceCentre[MAX_MSC_NUM_IN_BYTES];  //SMS_address[8];    //玉??連嶄伉仇峽
    
    //(MOSMSRecord)(MTSMSRecord)servedIMSI
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  瓜柴継卞強喘薩IMSI鷹

    //(MOSMSRecord)(MTSMSRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  瓜柴継卞強喘薩卞強岬譜姥紛艶鷹

    //(MOSMSRecord)(MTSMSRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;   
    BYTE		s2            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // 瓜柴継卞強喘薩MSISDN催
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MOSMSRecord)(MTSMSRecord)MSClassmark[3] 
    BYTE   MSClassmark[3];//struct  Classmark last_served_ms_classmark;      // 瓜柴継卞強喘薩輝念CLASSMARK
    struct  BillClassmark3 Classmark3Information;
    
    //(MOSMSRecord)(MTSMSRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;    
    BYTE		s3            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  云蕉MSC催

    //(MOSMSRecord)(MTSMSRecord)location 麼瓜出議緩囃栽匯
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
    SDiagnostics  smsResult;//歌深map協吶議SMSResult
    BYTE		actionResult;//荷恬潤惚??函SM_transmission_success,賜 SM_transmission_failed

    //(MOSMSRecord)destinationNumber
    BYTE		destinationNumber_npi :4;//dialed_isdn_npi  :4;                 // 瓜出催鷹柴皿
    BYTE		destinationNumber_nai:3;//dialed_isdn_nai  :3;                 // 瓜出催鷹窃侏 
    BYTE		s4               :1;    
    BYTE		destinationNumber_BCDlen; //dialed_isdn_num  :5;                 // 瓜出催鷹海業
    BYTE		destinationNumber[MAX_NUM_LEN/2];//dialed_num[MAX_NUM_LEN/2];     //12 瓜出催鷹

    //(MOSMSRecord)(MTSMSRecord)systemType
    BYTE		systemType:2;//USER_NETWORK_TYPE :4; 
    BYTE		s5               :6;

    //(MOSMSRecord)(MTSMSRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  燕苧乎肝匍暦葎窮三匍暦??葎＾0￣??賜覚墮匍暦??葎＾1￣??    
    BYTE		ROAM_mark               :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  卆象TBS野窮三匍暦鷹賜覚匍暦鷹。
    
    //(MOSMSRecord)(MTSMSRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // 柴継窒継炎崗
    BYTE		Classmark3Indication:1;
    BYTE		s7                :3;
   
    //(MOSMSRecord)(MTSMSRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 継楕峺幣
    UINT2		money_per_count; // 継楕
    UINT2		add_fee;                 //現紗継

    //(MOSMSRecord)(MTSMSRecord)chargedParty  朕念短嗤野亟??枠圓藻曳蒙了
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId海業峺幣
    BYTE		msc_id_len:3;//orgMSCId海業峺幣
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

    //(MOSMSRecord)(MTSMSRecord)hotBillingTag慧壓児云曝HotBillingTag

    //CAMEL SMS議崘嬬佚連
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

    //CAMELSMSInformation:smsReferenceNumber 朕念短嗤野亟??圓藻
    BYTE		smsReferenceNumberLen;
    BYTE		smsReferenceNumber[maxCallReferenceNumberLength];
    
    //(MOSMSRecord)(MTSMSRecord)recordExtensions
    //  輝念瓜柴継卞強喘薩侭侃MSC
    BYTE		served_msc_num_npi : 4;
    BYTE		served_msc_num_nai :  3;
    BYTE		s12            : 1;
    BYTE		served_msc_num_BCDlen ;
    BYTE		served_msc_num[MAX_MSC_NUM_IN_BYTES];       

    //(MOSMSRecord)(MTSMSRecord)recordExtensions
    //仟奐否匂喘恬朔偬制婢
    BYTE		container[MAX_CONTAINER_SMSMO_LEN];

    //CAMELSMSInformation:freeFormatData
    //(MOSMSRecord)(MTSMSRecord)smstext 朕念短嗤野亟??圓藻
    BYTE		fcidata_len;
    BYTE		smstext_len;
    BYTE		VarPart[MAX_TRANSFER_FCI_LEN + MAX_SMSTEXT_LEN];
    // BYTE		FCIData[MAX_TRANSFER_FCI_LEN];
    // BYTE		smstext[MAX_SMSTEXT_LEN];	
};

//1.8	HLR_Query_Bill_Report?┛?凄HLR Query(HLRIntRecord)??
struct HLR_Query_Bill_Report
{
	// 巷慌囃何蛍, 児云曝:

	BYTE		validflag; // 嗤丼來炎崗,　＾AA￣葎嗤丼??＾55￣葎涙丼
    UINT2		size; // 云嫖三汽海業
    UINT2		CRCCode; // CRC丕刮潤惚
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // 三汽送邦催??率匯曝蛍匯嫖三汽。
    BYTE		ModuleNum; // 恢伏三汽議庁翠催
    UINT4		count; // GSM 柴継輝匯肝柵出恢伏謹嫖三汽扮count 頁乎肝柵出遍三汽送邦催
    BYTE		sequence; // 汽嫖三汽葎0??遍嫖三汽葎1??凪噫窃容
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 汽嫖遍嫖嶄寂挑嫖三汽炎紛
    BYTE		HotBillingTag:1; //葎cmcc霞編奐紗議犯柴継炎崗
    BYTE		CRC_Way:2; // CRC丕刮圭塀
    BYTE		ChargeLevel:1;//柴継娼業峺幣:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//巷慌囃何蛍議制婢腎寂
    BYTE		Comm_reserved;//巷慌囃何蛍議制婢腎寂
	
	//  制婢曝:
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  瓜柴継卞強喘薩IMSI鷹
	
    //(HLRIntRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // 瓜柴継卞強喘薩MSISDN催
	BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(HLRIntRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//served_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//served_msc_num_nai :  3;   
    BYTE		s2            : 1;   
    BYTE		recordingEntity_BCDlen ;//served_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //served_msc_num[8];         //8  輝念瓜柴継卞強喘薩侭侃MSC
	
    //(HLRIntRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  燕苧乎肝匍暦葎窮三匍暦??葎＾0￣??賜覚墮匍暦??葎＾1￣??    
    BYTE		s3            :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  卆象TBS野窮三匍暦鷹賜覚匍暦鷹。
	
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
	
    //(HLRIntRecord)interrogationResult 短嗤野亟??枠圓藻忖准
    SDiagnostics     interrogationResult;
	
    //(HLRIntRecord)orgMSCId
    BYTE		msc_id_len:3;//orgMSCId海業峺幣
    BYTE		s6        :5;
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];
    
    //(HLRIntRecord)recordExtensions
    //仟奐否匂喘恬朔偬制婢
    BYTE		container[MAX_CONTAINER_HLRQUERY_LEN];
	
};

//1.9	SS_Action_Bill_Report?┛?凄SS Action(SSActionRecord)、EVENT??
struct SS_Action_Bill_Report
{
	// 巷慌囃何蛍, 児云曝:

	BYTE		validflag; // 嗤丼來炎崗,　＾AA￣葎嗤丼??＾55￣葎涙丼
    UINT2		size; // 云嫖三汽海業
    UINT2		CRCCode; // CRC丕刮潤惚
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // 三汽送邦催??率匯曝蛍匯嫖三汽。
    BYTE		ModuleNum; // 恢伏三汽議庁翠催
    UINT4		count; // GSM 柴継輝匯肝柵出恢伏謹嫖三汽扮count 頁乎肝柵出遍三汽送邦催
    BYTE		sequence; // 汽嫖三汽葎0??遍嫖三汽葎1??凪噫窃容
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 汽嫖遍嫖嶄寂挑嫖三汽炎紛
    BYTE		HotBillingTag:1; //葎cmcc霞編奐紗議犯柴継炎崗
    BYTE		CRC_Way:2; // CRC丕刮圭塀
    BYTE		ChargeLevel:1;//柴継娼業峺幣:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//巷慌囃何蛍議制婢腎寂
    BYTE		Comm_reserved;//巷慌囃何蛍議制婢腎寂
	
	//  制婢曝:
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  瓜柴継卞強喘薩IMSI鷹

    //(SSActionRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  瓜柴継卞強喘薩卞強岬譜姥紛艶鷹

    //(SSActionRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;    
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // 瓜柴継卞強喘薩MSISDN催
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(SSActionRecord)MSClassmark[3]
    BYTE   MSClassmark[3];//BYTE  ClassMark2;   
    struct  BillClassmark3 Classmark3Information;
    
    //(SSActionRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//    BYTE     served_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//    BYTE     served_msc_num_nai :  3;    
    BYTE		s2            : 1;       
    BYTE		recordingEntity_BCDlen ;//    BYTE     served_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //    BYTE    served_msc_num[8];       //8  輝念瓜柴継卞強喘薩侭侃MSC

    //(SSActionRecord)location 麼瓜出議緩囃栽匯
    BYTE		served_location_LAI[MAX_LOCAL_LAI_LEN/2];//caller_LAC[MAX_LOCAL_LAC_LEN/2];called_LAC[MAX_LOCAL_LAC_LEN/2]; 
    BYTE		served_location_cellID[MAX_LOCAL_CI_LEN/2];//caller_cell_ID[MAX_LOCAL_CI_LEN/2]; called_cell_ID[MAX_LOCAL_CI_LEN/2]; 

    //(SSActionRecord)BasicServices
    BYTE		TBS:4;//TBS:4;        //  燕苧乎肝匍暦葎窮三匍暦??葎＾0￣??賜覚墮匍暦??葎＾1￣??    
    BYTE		s3          :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  卆象TBS野窮三匍暦鷹賜覚匍暦鷹。

    //(SSActionRecord)supplService
    BYTE		SSCode;

    //(SSActionRecord)ssAction
    BYTE		ssAction;//SSAction; //SSActionType

    //(SSActionRecord)ssActionTime
    BYTE		ssActionTime_year; //温割匍暦強恬窟伏議扮寂漢
    BYTE		ssActionTime_month;
    BYTE		ssActionTime_date;
    BYTE		ssActionTime_hour;            //3
    BYTE		ssActionTime_minute;
    BYTE		ssActionTime_second;

    //(SSActionRecord)ssActionResult
    SDiagnostics		ssActionResult;//SSResult;

    //(SSActionRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; 喘噐揖匯卞強岬窟伏議音揖三暦議云仇紛艶催

   //(SSActionRecord)systemType
    BYTE		systemType:2;//AccessNetworkType;     
    //(SSActionRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // 柴継窒継炎崗
    BYTE		Classmark3Indication:1;
    BYTE		s4 :1;    
     
    //(SSActionRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 継楕峺幣
    UINT2		money_per_count; // 継楕
    UINT2		add_fee;                 //現紗継

    //(SSActionRecord)chargedParty 朕念短嗤野亟??枠圓藻曳蒙了
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId海業峺幣
    BYTE		msc_id_len:3;//orgMSCId海業峺幣
    BYTE		s23            : 2;
    //(SSActionRecord)orgRNCorBSCId
    BYTE		orgRNCorBSCId[MAX_RNCBSC_ID_LEN];//Rnc_Bsc_Id[3];

    //(SSActionRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(SSActionRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;

    //(SSActionRecord)recordExtensions
    BYTE		event_type;  //芝村恢伏並周三汽議並周窃侏:CH,CW,MPTY,HO: enum EVENT_TYPE    

    //ussd?犢慚渡?,歌深廉壇徨議俊笥??圓藻
    BYTE		UssdAccessCode[MAX_USSDACCESSCODE_LEN];
    BYTE		UssdCodingScheme;
    BYTE		UssdNotifyCounter ;
    BYTE		UssdRequestCounter;  

    // Deleted by zengsonglin for R002C01 2004-04-07
    // BYTE     UssdString[MAXUINT2SDSTRING_LEN];

    //changeofAOC寡喘並周三汽侃尖??葎緩奐紗和峰三汽囃
    AOC_E1_E7    aocParameters;  

    //仟奐否匂喘恬朔偬制婢
    BYTE     container[MAX_CONTAINER_SSACT_LEN];

    //(SSActionRecord)ssParameters 
    SSParameters   SSParameter;
    /*VarPart
    廣吭SSParameters云附祥頁辛延海業議
    軸SS_Para_len葎辛延何蛍海業峺幣
    辛延何蛍恷寄海業葎40忖准
    */	
};

//1.10	CEU_Bill_Report?┛?凄CEU(CommonEquipRecord)??
struct CEU_Bill_Report
{
	// 巷慌囃何蛍, 児云曝:
	BYTE		validflag; // 嗤丼來炎崗,　＾AA￣葎嗤丼??＾55￣葎涙丼
    UINT2		size; // 云嫖三汽海業
    UINT2		CRCCode; // CRC丕刮潤惚
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // 三汽送邦催??率匯曝蛍匯嫖三汽。
    BYTE		ModuleNum; // 恢伏三汽議庁翠催
    UINT4		count; // GSM 柴継輝匯肝柵出恢伏謹嫖三汽扮count 頁乎肝柵出遍三汽送邦催
    BYTE		sequence; // 汽嫖三汽葎0??遍嫖三汽葎1??凪噫窃容
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 汽嫖遍嫖嶄寂挑嫖三汽炎紛
    BYTE		HotBillingTag:1; //葎cmcc霞編奐紗議犯柴継炎崗
    BYTE		CRC_Way:2; // CRC丕刮圭塀
    BYTE		ChargeLevel:1;//柴継娼業峺幣:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//巷慌囃何蛍議制婢腎寂
    BYTE		Comm_reserved;//巷慌囃何蛍議制婢腎寂
	
	//  制婢曝:
	
    BYTE		equipmentType;//Equipment_type;

    //(CommonEquipRecord)equipmentId
    UINT2		equipmentId;//Equipment_Id;

    //(CommonEquipRecord)servedIMSI
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  瓜柴継卞強喘薩IMSI鷹

    BYTE		servedIMEI[MAX_IMEI_LEN];  //  瓜柴継卞強喘薩卞強岬譜姥紛艶鷹

    //(CommonEquipRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // 瓜柴継卞強喘薩MSISDN催
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(CommonEquipRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//served_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//served_msc_num_nai :  3;   
    BYTE		s2            : 1;    
    BYTE		recordingEntity_BCDlen ;//served_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //served_msc_num[8];          //8  輝念瓜柴継卞強喘薩侭侃MSC

    //(CommonEquipRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  燕苧乎肝匍暦葎窮三匍暦??葎＾0￣??賜覚墮匍暦??葎＾1￣??    
    BYTE		s3     :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  卆象TBS野窮三匍暦鷹賜覚匍暦鷹。
   //(CommonEquipRecord)supplServicesUsed
    BYTE		SSCode;
    //(CommonEquipRecord)seizureTime
    BYTE		initial_year;    // 宥三兜兵扮震
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            //3
    BYTE		initial_minute;
    BYTE		initial_second;

    //(CommonEquipRecord)releaseTime 
    BYTE		end_year;               // 宥三潤崩扮寂
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(CommonEquipRecord)callDuration
    UINT4		callDuration;//Call_Duration; 宥三扮海

    //(CommonEquipRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; 喘噐揖匯卞強岬窟伏議音揖三暦議云仇紛艶催

    //(CommonEquipRecord)sequenceNumber: 嶄寂三汽會双催,慧壓巷慌囃何蛍, 児云曝:sequence囃

    //(CommonEquipRecord)systemType 
    BYTE		systemType:2;//AccessNetworkType     
    BYTE		s4       :6;
    
    //(CommonEquipRecord)rateIndication
    BYTE		rateIndication;//ucRateIndication; //方象匍暦堀楕癖塘

    //(CommonEquipRecord)fnur
    BYTE		fnur;//ucFunr;           //方象匍暦喘薩堀楕,enum FNUR

    //(CommonEquipRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // 柴継窒継炎崗    
 	BYTE		s5         :4;
   
    //(CommonEquipRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 継楕峺幣
    UINT2		money_per_count; // 継楕
    UINT2		add_fee;                 //現紗継

    //(CommonEquipRecord)chargedParty 朕念短嗤野亟??枠圓藻曳蒙了
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId海業峺幣
    BYTE		msc_id_len:3;//orgMSCId海業峺幣
    BYTE		s23            : 2;
    //(CommonEquipRecord)orgRNCorBSCId
    BYTE		orgRNCorBSCId[MAX_RNCBSC_ID_LEN];//Rnc_Bsc_Id[3];

    //(CommonEquipRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(CommonEquipRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;


    BYTE		causeForTerm;//cause_for_termination;        //  宥三嶮峭圻咀
    SDiagnostics     diagnostics;//uDiagnostics;
    BYTE		cause_for_partial_record ;      //  竃嶄寂三汽圻咀

    //仟奐否匂喘恬朔偬制婢
    BYTE		container[MAX_CONTAINER_CEU_LEN];
	
};

//1.11	LCS_Bill_Report?┛?凄MO LCS(MOLCSRecord)、MT LCS(MTLCSRecord)、NI LCS(NILCSRecord)??
struct LCS_Bill_Report
{
	// 巷慌囃何蛍, 児云曝:
	BYTE		validflag; // 嗤丼來炎崗,　＾AA￣葎嗤丼??＾55￣葎涙丼
    UINT2		size; // 云嫖三汽海業
    UINT2		CRCCode; // CRC丕刮潤惚
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // 三汽送邦催??率匯曝蛍匯嫖三汽。
    BYTE		ModuleNum; // 恢伏三汽議庁翠催
    UINT4		count; // GSM 柴継輝匯肝柵出恢伏謹嫖三汽扮count 頁乎肝柵出遍三汽送邦催
    BYTE		sequence; // 汽嫖三汽葎0??遍嫖三汽葎1??凪噫窃容
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 汽嫖遍嫖嶄寂挑嫖三汽炎紛
    BYTE		HotBillingTag:1; //葎cmcc霞編奐紗議犯柴継炎崗
    BYTE		CRC_Way:2; // CRC丕刮圭塀
    BYTE		ChargeLevel:1;//柴継娼業峺幣:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//巷慌囃何蛍議制婢腎寂
    BYTE		Comm_reserved;//巷慌囃何蛍議制婢腎寂
	
	//  制婢曝:
	
    BYTE		lcs_bill_type;//location_type;             // 1 enum LOCATION_TYPE: MT、MO、NI
    
    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//served_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//served_msc_num_nai :  3;
    BYTE		s1            : 1;    
    BYTE		recordingEntity_BCDlen ;//served_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //served_msc_num[8];         //8  輝念瓜柴継卞強喘薩侭侃MSC

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)lcsClientType
    BYTE		lcsClientType;//LCSClientType;  //enum LCSClientType

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)lcsClientIdentity  
    BYTE		lcsClientIdentity_npi : 4;//lcs_client : 4;
    BYTE		lcsClientIdentity_nai : 3;//lcs_client :  3; 
    BYTE		s2            : 1;    
    BYTE		lcsClientIdentity_BCDlen ;//lcs_client : 5;
    BYTE		lcsClientIdentity[MAX_LCS_CLIENT_EXTERNAL_ID_LEN];//    BYTE    lcs_client[MAX_LCS_CLIENT_EXTERNAL_ID_LEN]; //20 翌何議LCS-CLIENT園鷹

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)servedIMSI
    BYTE		servedIMSI[MAX_IMSI_LEN];  //    BYTE    target_IMSI[MAX_IMSI_LEN];          //朕炎IMSI催鷹??8忖准

    //(NILCSRecord)servedIMEI  
    BYTE		servedIMEI[MAX_IMEI_LEN];//    BYTE    target_IMEI[MAX_IMEI_LEN];          //朕炎IMEI催鷹??8忖准

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;//target_num_npi : 4;
    BYTE		servedMSISDN_nai :  3;//target_num_nai :  3;    
    BYTE		s3            : 1;    
    BYTE		servedMSISDN_BCDlen;//target_num_num : 5;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//target_num [MAX_NUM_LEN/2+1]; //朕炎催鷹??13忖准
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MTLCSRecord)locationType
    BYTE		locationType;//LocationEstimateType;

    //(MOLCSRecord)molr-Type
    BYTE		molr_Type;//MOLRType;  //enum MOLR_Type

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)lcsQos 
    //QOS 協了娼業勣箔???贄κ閏箋?
    BYTE		horizontal_accuracy; //邦峠娼業
    BYTE		verticalCoordinateRequest; //0音勣箔??1勣箔
    BYTE		vertical_accuracy;   //換岷圭?鮠?業
    BYTE		responseTime; //協了?贄κ閏?

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)lcsPriority
    BYTE		lcsPriority;//lcs_priority;                 //  協了單枠雫

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)mlc-Number
    BYTE		GMLC_Number_npi : 4;//GMLC_address_npi : 4;
    BYTE		GMLC_Number_nai :  3;//GMLC_address_nai :  3;    
    BYTE		s4            : 1;    
    BYTE		GMLC_Number_BCDlen;//GMLC_address_num : 5;
    BYTE		GMLC_Number[MAX_GMLC_LEN + 1];//GMLC_address[9]; //9 GMLC園鷹    

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)eventTimeStamp
    BYTE		eventTimeStamp_year;     //initial_year; // 宥三兜兵扮震
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
    BYTE		served_location_LAI[MAX_LOCAL_LAI_LEN/2]; //target_LAC[MAX_LOCAL_LAC_LEN/2];    // 朕炎了崔曝催??2忖准
    BYTE		served_location_cellID[MAX_LOCAL_CI_LEN/2];//target_cell_ID[MAX_LOCAL_CI_LEN/2]; // 朕炎弌曝催??2忖准

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
    BYTE		chargeIndicator:4;//free_indicator :4;  // 柴継窒継炎崗   
    BYTE		s6            :4;

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 継楕峺幣
    UINT2		money_per_count; // 継楕
    UINT2		add_fee;                 //現紗継

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)chargedParty 朕念短嗤野亟??枠圓藻曳蒙了
    BYTE	    chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId海業峺幣
    BYTE		msc_id_len:3;//orgMSCId海業峺幣
    BYTE		s23            : 2;
    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)orgRNCorBSCId
    BYTE		orgRNCorBSCId[MAX_RNCBSC_ID_LEN];//Rnc_Bsc_Id[3];

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;

    //(MOLCSRecord)location_method
    BYTE		location_method;                 //協了圭隈 enum _locaton_method
    
    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)recordExtensions

    //(NILCSRecord)emsDigits朕念短嗤野亟??圓藻
    BYTE		emsDigits_npi : 4;
    BYTE		emsDigits_nai :  3;   
    BYTE		s7            : 1;    
    BYTE		emsDigits_BCDlen;
    BYTE		emsDigits[MAX_NUM_LEN/2];

    //(NILCSRecord)emsKey朕念短嗤野亟??圓藻
    BYTE		emsKey_npi : 4;
    BYTE		emsKey_nai :  3;  
    BYTE		s8            : 1;    
    BYTE		emsKey_BCDlen;
    BYTE		emsKey[MAX_NUM_LEN/2];

    //仟奐否匂喘恬朔偬制婢
    BYTE		container[MAX_CONTAINER_LCS_LEN];

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)positioningData 
    BYTE		positioningDataLen;//    BYTE     shapeinfoLen;
    
    BYTE        call_reference_len;
    BYTE        call_reference[maxCallReferenceNumberLength];
    
    BYTE		VarPart[maxShapeInfoLength];//BYTE     shapeInfo[maxShapeInfoLength];
    /*廣吭
    VarPart: positioningData祥頁辛延何蛍,恷寄海業葎maxShapeInfoLength
    */
	
};

//1.12	CheckIMEI_Bill_Report?┛?凄CHECK IMEI??
struct CheckIMEI_Bill_Report
{
	// 巷慌囃何蛍, 児云曝:

	BYTE		validflag; // 嗤丼來炎崗,　＾AA￣葎嗤丼??＾55￣葎涙丼
    UINT2		size; // 云嫖三汽海業
    UINT2		CRCCode; // CRC丕刮潤惚
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // 三汽送邦催??率匯曝蛍匯嫖三汽。
    BYTE		ModuleNum; // 恢伏三汽議庁翠催
    UINT4		count; // GSM 柴継輝匯肝柵出恢伏謹嫖三汽扮count 頁乎肝柵出遍三汽送邦催
    BYTE		sequence; // 汽嫖三汽葎0??遍嫖三汽葎1??凪噫窃容
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 汽嫖遍嫖嶄寂挑嫖三汽炎紛
    BYTE		HotBillingTag:1; //葎cmcc霞編奐紗議犯柴継炎崗
    BYTE		CRC_Way:2; // CRC丕刮圭塀
    BYTE		ChargeLevel:1;//柴継娼業峺幣:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//巷慌囃何蛍議制婢腎寂
    BYTE		Comm_reserved;//巷慌囃何蛍議制婢腎寂
	
	//  制婢曝:
	
    BYTE		checkIMEI_year;          //initial_year; // 宥三兜兵扮震
    BYTE		checkIMEI_month;       //initial_month;
    BYTE		checkIMEI_date;       //initial_date;
    BYTE		checkIMEI_hour;          //initial_hour;            
    BYTE		checkIMEI_minute;       //initial_minute;
    BYTE		checkIMEI_second;       //initial_second;
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  瓜柴継卞強喘薩IMSI鷹
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  瓜柴継卞強喘薩卞強岬譜姥紛艶鷹
	
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;   
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // 瓜柴継卞強喘薩MSISDN催
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    BYTE		served_location_LAI[MAX_LOCAL_LAI_LEN/2]; //target_LAC[MAX_LOCAL_LAC_LEN/2];    // 朕炎了崔曝催??5忖准
    BYTE		served_location_cellID[MAX_LOCAL_CI_LEN/2]; //target_cell_ID[MAX_LOCAL_CI_LEN/2]; // 朕炎弌曝催??2忖准
    
    BYTE		IMEIStatus;  //enum _Equipment_Status
    BYTE		Event;  //enum IMEICheckEvent
    
    BYTE		recordingEntity_npi : 4;//served_msc_npi : 4;
    BYTE		recordingEntity_nai : 3;//served_msc_nai :  3;  
    BYTE		s2            : 1;    
    BYTE		recordingEntity_BCDlen ;//served_msc_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //served_msc[8];         //捲暦MSC園鷹
	
    BYTE		msc_id_len:3;//orgMSCId海業峺幣
    BYTE		s3            : 5;
    BYTE		orgMSCId[MAX_MSCID_LEN];
	
    BYTE		CallReferenceLen;
    BYTE		CallReference[maxCallReferenceNumberLength];
	
    //仟奐否匂喘恬朔偬制婢
    BYTE		container[MAX_CONTAINER_CHECKIMEI_LEN];
	
};

#pragma pack()

#endif  //__MSCV9_BILL_FORMAT_H__
