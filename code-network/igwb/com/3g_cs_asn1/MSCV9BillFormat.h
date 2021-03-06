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
    ACCESS_BSC  = 0,   // 2G网络
    ACCESS_RNC  = 1,   // 3G网络
    ACCESS_UNKNOWN = 2, //未知网络 
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
    MOC					= 0,	//移动始发呼叫
    MTC					= 1,	//移动终结呼叫
    GATEWAY_INCOMING	= 2,	//入关口局话单
    EC					= 3,	//紧急呼叫
    TRANSIT				= 4,	//中转，汇接呼叫
    SS_ACT				= 5,	//补充业务编程动作，注册、注销、激活、去话
    MT_SMS				= 6,	//移动台终结短消息业务
    MO_SMS				= 7,    //移动台始发短消息业务
    LOCATION_FREASH		= 8,    //位置更新
    CALL_ATTEMPT		= 9,    //试呼话单
	GATEWAY_OUTGOING	= 10,   //出关口局话单  
    ISDN_OC             = 11,   
    ISDN_TC             = 12,   
    ROAM                = 13,   //漫游话单
    CommonEquip         = 14,   //公共设备话单
    MO_FREECALL         = 17,   //主叫智能用户免费话单类型
    CFW					= 18,   //前转话单
    MOI					= 19,   //代做话单
    MO_HO               = 20,   //主叫切换话单
    MT_HO               = 21,   //被叫切换话单
    MT_FREECALL         = 22,   //被叫智能用户免费话单类型
    CFW_FREECALL        = 23,   // 智能前转时原被叫智能用户免费话单类型 
    TCAMEL				= 24,   //终结CAMEL访问记录
    FCI					= 25,   //自由格式数据话单
 	LCS					= 26,   //位置业务话单 
    Check_IMEI			= 27,   //Check IMEI话单类型
    QUERY_HLR			= 28,   //关口局HLR问询话单类型
    EVENT_BILL          = 30
};

enum HOT_BILLING_MARK
{
    NORMAL_BILLING     = 0x00,        //普通计费
    HOT_BILLING        = 0x01         //热计费
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
    SINGLE_BILL,            //本次呼叫只产生一张话单
    FIRST_BILL,             //本次呼叫第一张话单
    INTERMEDIATE_BILL,      //中间话单
    LAST_BILL               //本次呼叫最后一张话单
};

enum MBILL_TBS
{
    MBILL_TELECOM_SERVICE,  //电信业务
    MBILL_BEAR_SERVICE,     //承载业务
    MBILL_ISDN_SERVICE,     // ISDN业务
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

//by    杨善    2004-6-2    对应问题单SWPD05614
struct MSC_Bill_Report
{
   	// 公共域部分, 基本区:
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
//the end  对应问题单SWPD05614

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

//by    杨善    2004-6-2    对应问题单SWPD05614
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
//the end  对应问题单SWPD05614

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
// SS_code2字段表示如下补充业务：
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

// SS_code3字段表示如下补充业务：
struct MBILL_SS_code3
{
        BYTE   reserve4      :1;
        BYTE   reserve3      :1;
        
        //by    杨善    2004-6-2    对应问题单SWPD05614
        BYTE   CLIR			 :1;
        BYTE   CLIP          :1;
        //the end  对应问题单SWPD05614

        BYTE   ALLMOLR       :1;
        BYTE   UUS3          :1;
        BYTE   UUS2          :1;
        BYTE   UUS1          :1;
};
// SS_code4字段表示如下补充业务：
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
// SS_code5字段表示如下补充业务：
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
// SS_code2字段表示如下补充业务：
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

// SS_code3字段表示如下补充业务：
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
// SS_code4字段表示如下补充业务：
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
// SS_code5字段表示如下补充业务：
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

//1.1	MOC_Bill_Report（包括MOC(moCallRecord) /CFW(forwardCallRecord:接口同MOCallRecord)/MOI）
struct MOC_Bill_Report
 {
   	// 公共域部分, 基本区:
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
	//  扩展区:
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

    BYTE		TBS:4;//TBS:4;        //  表明该次业务为电话业务（为“0”）或承载业务（为“1”）    
	BYTE		ROAM_mark       :4;    
    BYTE		ServiceCode;//tele_bearer_code;  //  依据TBS填电话业务码或承业务码。

    BYTE		transparencyIndicator:2;  //transparency_indicator :2;  // 透明非透明指示   
	BYTE		s9         :6;

    struct MBILL_SS_code1     SS_code1;          
    struct MBILL_SS_code2     SS_code2;          
    struct MBILL_SS_code3     SS_code3;          
    struct MBILL_SS_code4     SS_code4;          
    struct MBILL_SS_code5     SS_code5;

	AOC_E1_E7	aocParameters;

	BYTE		MSClassmark[3];// 即原来的struct  Classmark initial_served_ms_classmark，被计费移动用户初始CLASSMARK，struct  Classmark不符合协议改为BYTE
	BYTE		changeOfClassmark[3];//struct  Classmark last_served_ms_classmark;  //被计费移动用户改变后的CLASSMARK

	BYTE		initial_year;           // 通话初始时刻
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            
    BYTE		initial_minute;
    BYTE		initial_second;

	BYTE		end_year; // 通话结束时间
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

	UINT4		callDuration;//conversation_time;                 // 通话时长
	
    BYTE		radioChanRequested:4;//USER_Radio_channelUINT2ed :4;                //   全速率或半速率 
    BYTE		radioChanUsed:2; 	
	BYTE		s10           :2; 
	   
	BYTE		causeForTerm;//cause_for_termination;        //  通话终止原因
	
	SDiagnostics     diagnostics;//uDiagnostics;
	BYTE		call_reference_len;
	BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; 用于同一移动台发生的不同话务的本地识别号

    BYTE		chargeIndicator:4;//free_indicator :4;  // 计费免费标志    
  	BYTE		s11          :4;

    
    //(MOCallRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 费率指示
    UINT2		money_per_count; // 费率
    UINT2		add_fee;
	
	BYTE		chargedParty;

    BYTE		CAMEL_GsmSCF_Address_npi : 4;
    BYTE		CAMEL_GsmSCF_Address_nai :  3;
	BYTE		s12            : 1;    
	
    BYTE		CAMEL_GsmSCF_Address_BCDlen;
    BYTE		CAMEL_GsmSCF_Address[MAX_SCF_LEN]; 

	UINT4		ServiceKey;                //业务键

	BYTE		NetworkCallReferenceLen;
    BYTE		NetworkcallReferenceNumber[maxCallReferenceNumberLength];

	//(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mSCAddress (跟NetworkCallReferenceLen配合使用)
    BYTE		MSCAddress_npi : 4;
    BYTE		MSCAddress_nai : 3;
    BYTE		s13            : 1;   
    
    BYTE		MSCAddress_BCDlen;
    BYTE		MSCAddress[MAX_MSC_NUM_IN_BYTES];

	BYTE		CAMELInitCFIndicator:1;
	BYTE		defaultCallHandling:2;
	BYTE		s14            : 5;
	
	BYTE		fnur;//ucFunr;           //数据业务用户速率,enum FNUR
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
    UINT4		ServiceKey2;                //业务键
	
	BYTE		freeFormatDataAppend2:2;
	BYTE		freeFormatDataAppend:2;
	BYTE		defaultCallHandling2:2 ;
	BYTE		systemType:2;
	BYTE		rateIndication;

    BYTE		originalCalledNumber_npi  :4;                 // 号码计划
	BYTE		originalCalledNumber_nai  :3; 
	BYTE		s18               :1;    
                // 号码类型
    BYTE		originalCalledNumber_BCDlen;                 // 号码长度
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

	BYTE		callingChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];

	BYTE		calledChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];
	
	UINT2		mscOutgoingCircuit;
    UINT2		mscIncomingCircuit;
	
    BYTE		rnc_bsc_id_len:3;
    BYTE		msc_id_len:3;//orgMSCId长度指示
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
	
	BYTE	    CFW_invoked_times:4;    // 前转发生的次数    
	BYTE		CH_invoked_times:4;        // 呼叫保持业务发生的次数
    BYTE		CH_retrieve_times:4;       // 呼叫保持业务恢复的次数    
    BYTE		CW_invoked_times:4;        // 呼叫等待业务发生的次数
    BYTE		MPTY_Build_times:4;        // 多方通话业务建立发生的次数    
    BYTE		MPTY_CH_times:4;           // 多方通话业务保持发生的次数
    BYTE		MPTY_CH_retrieve_times:4;   // 多方通话业务保持恢复发生的次数   
    BYTE		MPTY_Split_times:4;        // 多方通话业务隔离发生的次数
	
	BYTE		HO_invoked_times;         // 切换发生的次数
	BYTE		AOCParmsChangedTimes:4; 
	BYTE		ClassmarkChangedTimes:4;
    BYTE		RadioChanChangedTimes:4;	
	BYTE		transmission_mode:4;   // 2G话单使用的  全速率或半速率 标志。

	BYTE		Info_trans_capability;             // 承载能力
    BillClassmark3 Classmark3Information;    
	
	BYTE		DTMF_indicator :2;  // 是否使用DTMF
	BYTE		user_type :5;  //   移动台优先级  
	BYTE		s8 :1;  //   0 表示本地用户，1 表示漫游用户�
	
  
    BYTE		EmergencyCallFlag     :1;          // 紧急呼叫标�   
    BYTE		PromptAndCollectCharge :1;               //资源计费话单标志以前的s6标志
    BYTE		SORIndication:1;
 	BYTE		s21:1;
	BYTE		EarlyForwardSORIndication	:1;
	BYTE		LateForwardSORIndication	:1; 	
    BYTE		s20                   :2;
    // BYTE		UssdCallBackIndication:1; 
    
    BYTE		ModemType:7;
	BYTE		Classmark3Indication:1;   
    
    BYTE		cause_for_partial_record;//cause_for_partial_record ;      //  出中间话单原因
    BYTE		transaction_id;// 事务ID,用于同一移动台发生的不同话务的本地识别号
    BYTE		gsm_gsvn;                          // 1业务类别
    BYTE		B_ch_number;       // 该次呼叫所占用的B信道数
	
    BYTE		scfID[MAX_SCFID_LEN]; 

    BYTE		served_msc_num_npi : 4;
    BYTE		served_msc_num_nai :  3;
	BYTE		s22            : 1;
    BYTE		served_msc_num_BCDlen ;
	BYTE		served_msc_num[MAX_MSC_NUM_IN_BYTES];  
	
	BYTE		GuaranteedBitRate;  //保证比特率
	BYTE		MaximumBitRate;     //最大比特率

    //新增字段
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

	//可变部分
	
	BYTE		fcidata_len;
    BYTE		fcidata2_len;
    BYTE     VarPart[MAX_TRANSFER_FCI_LEN * 2];
   
	/*
    VarPart由以下两部分组成
    BYTE FCIData[MAX_TRANSFER_FCI_LEN];
    BYTE FCIData2[MAX_TRANSFER_FCI_LEN];
    */

};

//1.2	MTC_Bill_Report（包括MTC(mtCallRecord)）
struct MTC_Bill_Report
{
	// 公共域部分, 基本区:
	BYTE		validflag; // 有效性标志,　“AA”为有效，“55”为无效
    UINT2		size; // 本张话单长度
    UINT2		CRCCode; // CRC校验结果
    BYTE		bill_type; //  enum   GSM_BILL_TYPE, 协议中的recordType域
    UINT4		bill_sequence; // 话单流水号，唯一区分一张话单。
    BYTE		ModuleNum; // 产生话单的模块号
    UINT4		count; // GSM 计费当一次呼叫产生多张话单时count 是该次呼叫首话单流水号
    BYTE		sequence; // 单张话单为0，首张话单为1，其余类推
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 单张首张中间末张话单标识
    BYTE		HotBillingTag:1; //为cmcc测试增加的热计费标志
    BYTE		CRC_Way:2; // CRC校验方式
    BYTE		ChargeLevel:1;//计费精度指示:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//公共域部分的扩展空间
    BYTE		Comm_reserved;//公共域部分的扩展空间
	
	//  扩展区:
	
	BYTE		servedIMSI[MAX_IMSI_LEN];  //  被计费移动用户IMSI码

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  被计费移动用户移动台设备识别码

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;  
    BYTE		servedMSISDN_nai :  3;
    BYTE		s1            : 1;
    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // 被计费移动用户MSISDN号
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
        
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callingNumber
    BYTE		callingNumber_npi  :4;                 // 主叫号码计划
    BYTE		callingNumber_nai  :3;                 // 主叫号码类型    
    BYTE		s2               :1;

    BYTE		callingNumber_BCDlen ;                 // 主叫号码长度
    BYTE		callingNumber[MAX_NUM_LEN/2];     // 主叫号码

    //(MOCallRecord)(TermCAMELRecord)calledNumber
    BYTE		calledNumber_npi  :4;//dialed_isdn_npi  :4;                 // 被叫号码计划
    BYTE		calledNumber_nai  :3;//dialed_isdn_nai  :3;                 // 被叫号码类型
    BYTE		s3               :1;
   
    BYTE		calledNumber_BCDlen ;//dialed_isdn_num  :5;                 // 被叫号码长度
    BYTE		calledNumber[MAX_NUM_LEN/2];  //dialed_num[MAX_NUM_LEN/2];     // 被叫号码

    //(MOCallRecord)(MTCallRecord)connectedNumber
    BYTE		connectedNumber_npi :4;               //第三方号码计划
    BYTE		connectedNumber_nai :3;               //第三方号码类型   
    BYTE		s4                 :1;               //unuseful    

    BYTE		connectedNumber_BCDlen;               //第三方号码长度
    BYTE		connectedNumber[MAX_NUM_LEN/2];  // 第三方号码

    //(MOCallRecord)(MTCallRecord)(roamingRecord)roamingNumber
    BYTE		roamingNumber_npi : 4;
    BYTE		roamingNumber_nai :  3;    
    BYTE		s5            : 1;    
    BYTE		roamingNumber_BCDlen ;
    BYTE		roamingNumber[MAX_NUM_LEN/2] ;    //MSRN               //  移动被叫漫游号码

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)originalCalledNumber 目前没有填写，预留
    BYTE		originalCalledNumber_npi  :4;                 // 号码计划
    BYTE		originalCalledNumber_nai  :3;                 // 号码类型
    BYTE		s6               :1;    
    BYTE		originalCalledNumber_BCDlen;                 // 号码长度
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

    //(MTCallRecord)redirectingnumber 目前没有填写，预留
    BYTE		redirectingnumber_npi  :4;                 // 号码计划
    BYTE		redirectingnumber_nai  :3;                 // 号码类型   
    BYTE		s7               :1;    
    BYTE		redirectingnumber_BCDlen;                 // 号码长度
    BYTE		redirectingnumber[MAX_NUM_LEN/2]; 

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;  
    BYTE		s8            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  本局MSC号

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mscIncomingROUTE/mscOutgoingROUTE
    BYTE		mscIncomingROUTE_ind : 4;//IT_trunk_group_ind : 4;       
    BYTE		mscOutgoingROUTE_ind : 4;//OT_trunk_group_ind : 4; 
    UINT2		mscIncomingROUTE_ROUTENumber;//IT_trunk_group;                     // 入中继群
    UINT2		mscOutgoingROUTE_ROUTENumber;//OT_trunk_group;                    // 出中继群
    BYTE		mscIncomingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//IT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];
    BYTE		mscOutgoingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//OT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];

    //(MTCallRecord)location/changeOfLocation 
    BYTE		caller_location_LAI[MAX_LOCAL_LAI_LEN/2];//caller_org_LAC[MAX_LOCAL_LAC_LEN/2];  //  被计费移动用户所处初始位置区号
    BYTE		caller_location_cellID[MAX_LOCAL_CI_LEN/2];//caller_org_cell_ID[MAX_LOCAL_CI_LEN/2];   // 被计费移动用户所处的初始小区号
    BYTE		caller_changeOflocation_LAI[MAX_LOCAL_LAI_LEN/2]; //caller_LAC[MAX_LOCAL_LAC_LEN/2];     // 被计费移动用户所处位置区号
    BYTE		caller_changeOflocation_cellID[MAX_LOCAL_CI_LEN/2];//caller_cell_ID[MAX_LOCAL_CI_LEN/2];  // 被计费移动用户所处的小区号

    BYTE		called_location_LAI[MAX_LOCAL_LAI_LEN/2];//called_org_LAC[MAX_LOCAL_LAC_LEN/2];    // 被叫用户所处初始位置区号
    BYTE		called_location_cellID[MAX_LOCAL_CI_LEN/2]; //called_org_cell_ID[MAX_LOCAL_CI_LEN/2];    // 被叫用户所处的初始小区号
    BYTE		called_changeOflocation_LAI[MAX_LOCAL_LAI_LEN/2];//called_LAC[MAX_LOCAL_LAC_LEN/2];          // 被叫用户所处位置区号
    BYTE		called_changeOflocation_cellID[MAX_LOCAL_CI_LEN/2]; //called_cell_ID[MAX_LOCAL_CI_LEN/2];       // 被叫用户所处的小区号

    //(MOCallRecord)(MTCallRecord)(roamingRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  表明该次业务为电话业务（为“0”）或承载业务（为“1”）
    BYTE		ROAM_mark        :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  依据TBS填电话业务码或承业务码。
    
    //(MOCallRecord)(MTCallRecord)(roamingRecord)transparencyIndicator
    BYTE		transparencyIndicator:2;  //transparency_indicator :2;  // 透明非透明指示    
    BYTE		s10          :6;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)changeOfService 用事件话单或中间话单的方式出

    //(MOCallRecord)(MTCallRecord)(roamingRecord)supplServicesUsed(呼叫无关的补充业务用事件话单方式出,BAU合并)
    struct MBILL_SS_code1     SS_code1;          
    struct MBILL_SS_code2     SS_code2;          
    struct MBILL_SS_code3     SS_code3;          
    struct MBILL_SS_code4     SS_code4;          
    struct MBILL_SS_code5     SS_code5;

    //(MOCallRecord)(MTCallRecord)aocParameters/changeOfAOCParms
    AOC_E1_E7    aocParameters;  

    //(MOCallRecord)(MTCallRecord)MSClassmark[3]
    BYTE  MSClassmark[3];// 即原来的struct  Classmark initial_served_ms_classmark，被计费移动用户初始CLASSMARK，struct  Classmark不符合协议改为BYTE

    //(MOCallRecord)(MTCallRecord)changeOfClassmark[3]  
    BYTE   changeOfClassmark[3];//struct  Classmark last_served_ms_classmark;  //被计费移动用户改变后的CLASSMARK

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)seizureTime 
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)answerTime
    BYTE		initial_year;           // 通话初始时刻
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            
    BYTE		initial_minute;
    BYTE		initial_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)releaseTime
    BYTE		end_year; // 通话结束时间
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callDuration
    UINT4		callDuration;//conversation_time;                 // 通话时长
        
    //(MOCallRecord)(MTCallRecord)radioChanUsed 
    //(MOCallRecord)(MTCallRecord)radioChanRequested  
    BYTE		radioChanRequested:4;//USER_Radio_channelUINT2ed :4;                // 1  全速率或半速率    

    BYTE		radioChanUsed:2; 

    BYTE		s11    :2;    

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)causeForTerm
    BYTE		causeForTerm;//cause_for_termination;        //  通话终止原因

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)SDiagnostics
    SDiagnostics     diagnostics;//uDiagnostics;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; 用于同一移动台发生的不同话务的本地识别号

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)sequenceNumber: 中间话单序列号,放在公共域部分, 基本区:sequence域

    //(MOCallRecord)(MTCallRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // 计费免费标志    
    BYTE		s12         :4;

    //(MOCallRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 费率指示
    UINT2		money_per_count; // 费率
    UINT2		add_fee;                 //附加费

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)chargedParty 目前没有填写，先预留比特位
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)networkCallReference
    //当使用CAMEL业务时,同一话务所经的各网元产生的话单将含有此标识码(NCR),用以把这些话单关联到一起. 
    BYTE		NetworkCallReferenceLen;
    BYTE		NetworkcallReferenceNumber[maxCallReferenceNumberLength]; // 由NetworkCallReferenceLen指明长度(2G旧有的呼叫参考号)

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mSCAddress (跟NetworkCallReferenceLen配合使用)
    BYTE		MSCAddress_npi : 4;
    BYTE		MSCAddress_nai :  3;
    BYTE		s13            : 1;    
    BYTE		MSCAddress_BCDlen;
    BYTE		MSCAddress[MAX_MSC_NUM_IN_BYTES];

    //(MOCallRecord)(MTCallRecord)fnur
    BYTE		fnur;//ucFunr;           //数据业务用户速率,enum FNUR

    //(MOCallRecord)(MTCallRecord)aiurRequested
    BYTE		aiurRequested;//BYTE SMSResult;(B03中域名使用的是SMSResult但含义是aiurRequested，R02改名)

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
    UINT4		ServiceKey;                //业务键

    //(MOCallRecord)(MTCallRecord)rateIndication
    BYTE		rateIndication;//ucRateIndication; //数据业务速率适配

     //(MOCallRecord)(MTCallRecord)systemType
    BYTE		systemType:2;//USER_NETWORK_TYPE :4; 

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)freeFormatDataAppend
    BYTE		freeFormatDataAppend:2;//CAMEL_FCI_Data_Append_Ind :2; //表示是第一个FCI消息，0表示不是第一个

      //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)defaultCallHandling
    BYTE		defaultCallHandling:2;//CAMEL_default_Call_Handling;   
    BYTE		s15       :2;   
   

    //(MOCallRecord)(MTCallRecord)callingChargeAreaCode 目前没有填写，预留//泰国特殊域    BYTE    CAC_num[MAX_CAC_LEN/2+1];
    BYTE		callingChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];

    //(MOCallRecord)(MTCallRecord)calledChargeAreaCode 目前没有填写，预留//泰国特殊域    BYTE    CAC_num[MAX_CAC_LEN/2+1];
    BYTE		calledChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)numberOfDPEncountered
    BYTE		numberOfDPEncountered;//CAMEL_num_of_DP;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)levelOfCAMELService
    BYTE		levelOfCAMELService;//CAMEL_service_level;

    //(MTCallRecord)(roamingRecord)mscIncomingCircuit
    UINT2		mscOutgoingCircuit;
    UINT2		mscIncomingCircuit;

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId长度指示
    BYTE		msc_id_len:3;//orgMSCId长度指示
    BYTE		s23            : 2;

    //(MOCallRecord)(MTCallRecord)orgRNCorBSCId
    BYTE		orgRNCorBSCId[MAX_RNCBSC_ID_LEN];//Rnc_Bsc_Id[3];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(MOCallRecord)(MTCallRecord)callEmlppPriority
    BYTE		callEmlppPriority:4;//CallEmlpp;//呼叫最终EMLPP优先级 （取值0-7)

    //(MTCallRecord)calledDefaultEmlppPriority
    BYTE		calledDefaultEmlppPriority:4;//calledDefaultEmlpp;//被叫用户签约默认EMLPP优先级（取值0-7)

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

    //(MOCallRecord)(MTCallRecord)hotBillingTag放在基本区HotBillingTag

    //(MTCallRecord)redirectingcounter 
    BYTE		CFW_invoked_times:4;       // 前转发生的次数    

    //(MOCallRecord)(MTCallRecord)recordExtensions
    BYTE		CH_invoked_times:4;        // 呼叫保持业务发生的次数
    BYTE		CH_retrieve_times:4;       // 呼叫保持业务恢复的次数    
    BYTE		CW_invoked_times:4;        // 呼叫等待业务发生的次数
    BYTE		MPTY_Build_times:4;        // 多方通话业务建立发生的次数    
    BYTE		MPTY_CH_times:4;           // 多方通话业务保持发生的次数
    BYTE		MPTY_CH_retrieve_times:4;   // 多方通话业务保持恢复发生的次数
    BYTE		MPTY_Split_times:4;        // 多方通话业务隔离发生的次数

    BYTE		HO_invoked_times;         // 切换发生的次数
    
    //BYTE     ServiceChangedTimes:4;

    BYTE		AOCParmsChangedTimes:4;    
    BYTE		ClassmarkChangedTimes:4;    
    BYTE		RadioChanChangedTimes:4;
    BYTE		transmission_mode:4;   // 2G话单使用的  全速率或半速率 标志。

    BYTE		Info_trans_capability;             // 承载能力

    struct  BillClassmark3 Classmark3Information;    

    BYTE		DTMF_indicator :2;  // 是否使用DTMF
    BYTE		user_type :5;  //   移动台优先级    
    BYTE		s9 :1;  //   0 表示本地用户，1 表示漫游用户。
    BYTE		initialCallAttemptFlag :1;	
    BYTE		reserved :1; 
    BYTE		SORIndication:1;    
    BYTE		UssdCallBackIndication:1;// 只在CAMEL MTC话单中才记录
    BYTE		ucTcsiVtcsiFlag : 4;//0为TCSI，1为VTCSI,0xff无效 
	
    //为自动波特率Autobauding Support新增
    BYTE		ModemType:7;    
    BYTE		Classmark3Indication:1;
    
    BYTE		cause_for_partial_record;//cause_for_partial_record ;      //  出中间话单原因    
    BYTE		transaction_id;// 事务ID,用于同一移动台发生的不同话务的本地识别号
    BYTE		gsm_gsvn;                          // 1业务类别
    BYTE		B_ch_number;       // 该次呼叫所占用的B信道数

    // GSM 特殊域
    BYTE		scfID[MAX_SCFID_LEN];      //SCF索引号

    //  当前被计费移动用户所处MSC
    BYTE		served_msc_num_npi : 4;
    BYTE		served_msc_num_nai :  3;
    BYTE		s18            : 1;
    BYTE		served_msc_num_BCDlen ;
    BYTE		served_msc_num[MAX_MSC_NUM_IN_BYTES];       

    //数据业务比特率定义
    BYTE     	GuaranteedBitRate;//保证比特率
    BYTE    	MaximumBitRate;//最大比特率
    
    //新增容器用作后续扩展
    BYTE		container[MAX_CONTAINER_MTC_LEN];

    // FCI 放在可变部分
    BYTE		fcidata_len;
    BYTE        VarPart[MAX_TRANSFER_FCI_LEN];
    BYTE		FCIData[MAX_TRANSFER_FCI_LEN];
	
};

//1.3	TCAMEL_Bill_Report（包括TCAMEL （TermCAMELRecord））
struct TCAMEL_Bill_Report
{
	// 公共域部分, 基本区:
	BYTE		validflag; // 有效性标志,　“AA”为有效，“55”为无效
    UINT2		size; // 本张话单长度
    UINT2		CRCCode; // CRC校验结果
    BYTE		bill_type; //  enum   GSM_BILL_TYPE, 协议中的recordType域
    UINT4		bill_sequence; // 话单流水号，唯一区分一张话单。
    BYTE		ModuleNum; // 产生话单的模块号
    UINT4		count; // GSM 计费当一次呼叫产生多张话单时count 是该次呼叫首话单流水号
    BYTE		sequence; // 单张话单为0，首张话单为1，其余类推
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 单张首张中间末张话单标识
    BYTE		HotBillingTag:1; //为cmcc测试增加的热计费标志
    BYTE		CRC_Way:2; // CRC校验方式
    BYTE		ChargeLevel:1;//计费精度指示:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//公共域部分的扩展空间
    BYTE		Comm_reserved;//公共域部分的扩展空间
	
	//  扩展区:

	BYTE		servedIMSI[MAX_IMSI_LEN];  //  被计费移动用户IMSI码

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  被计费移动用户移动台设备识别码

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // 被计费移动用户MSISDN号
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callingNumber
    BYTE		callingNumber_npi  :4;                 // 主叫号码计划
    BYTE		callingNumber_nai  :3;                 // 主叫号码类型   
    BYTE		s2               :1;    
    BYTE		callingNumber_BCDlen ;                 // 主叫号码长度
    BYTE		callingNumber[MAX_NUM_LEN/2];     // 主叫号码

    //(MOCallRecord)(TermCAMELRecord)calledNumber
    BYTE		calledNumber_npi  :4;//dialed_isdn_npi  :4;                 // 被叫号码计划
    BYTE		calledNumber_nai  :3;//dialed_isdn_nai  :3;                 // 被叫号码类型    
    BYTE		s3               :1;    
    BYTE		calledNumber_BCDlen ;//dialed_isdn_num  :5;                 // 被叫号码长度
    BYTE		calledNumber[MAX_NUM_LEN/2];  //dialed_num[MAX_NUM_LEN/2];     // 被叫号码

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;   
    BYTE		s4            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  本局MSC号

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
    UINT4		ServiceKey;                //业务键

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)networkCallReference
    //当使用CAMEL业务时,同一话务所经的各网元产生的话单将含有此标识码(NCR),用以把这些话单关联到一起. 
    BYTE		NetworkCallReferenceLen;
    BYTE		NetworkcallReferenceNumber[maxCallReferenceNumberLength]; // 由NetworkCallReferenceLen指明长度(2G旧有的呼叫参考号)

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mSCAddress (跟NetworkCallReferenceLen配合使用)
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
    UINT2		mscIncomingROUTE_ROUTENumber;//IT_trunk_group;                     // 入中继群
    UINT2		mscOutgoingROUTE_ROUTENumber;//OT_trunk_group;                    // 出中继群
    BYTE		mscIncomingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//IT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];
    BYTE		mscOutgoingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//OT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)seizureTime 
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)answerTime
    BYTE		initial_year;           // 通话初始时刻
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            //3
    BYTE		initial_minute;
    BYTE		initial_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)releaseTime
    BYTE		end_year; // 通话结束时间
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callDuration
    UINT4		callDuration;//conversation_time;                 // 通话时长

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)causeForTerm
    BYTE		causeForTerm;//cause_for_termination;        //  通话终止原因

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)SDiagnostics
    SDiagnostics     diagnostics;//uDiagnostics;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; 用于同一移动台发生的不同话务的本地识别号

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)numberOfDPEncountered
    BYTE		numberOfDPEncountered;//CAMEL_num_of_DP;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)levelOfCAMELService
    BYTE		levelOfCAMELService;//CAMEL_service_level;


    MBILL_CHANGEFLAGS     CAMELModificationChangeFlags;

	BYTE		ModifiedCallingPartyNumber_npi  :4;                 // 号码计划
    BYTE		ModifiedCallingPartyNumber_nai  :3;                 // 号码类型   
    BYTE		s9               :1;    
    BYTE		ModifiedCallingPartyNumber_BCDlen;                 // 号码长度
    BYTE		ModifiedCallingPartyNumber[MAX_NUM_LEN/2];     // 改变后的主叫号码

    BYTE		ModifiedCallingPartyCategory;// 修改后的主叫用户类别

    BYTE		ModifiedOriginalCalledPartyNumber_npi  :4;                 // 号码计划
    BYTE		ModifiedOriginalCalledPartyNumber_nai  :3;                 // 号码类型   
    BYTE		s10               :1;       
    BYTE		ModifiedOriginalCalledPartyNumber_BCDlen;                 // 号码长度
    BYTE		ModifiedOriginalCalledPartyNumber[MAX_NUM_LEN/2]; // 改变后的原被叫号码

    BYTE		ModifiedGenericNumber_npi  :4;                 // 号码计划
    BYTE		ModifiedGenericNumber_nai  :3;                 // 号码类型 
    BYTE		s11               :1;    
    BYTE		ModifiedGenericNumber_BCDlen;                 // 号码长度
    BYTE		ModifiedGenericNumber[MAX_NUM_LEN/2];   // 改变后的GenericNumber,参见CapEncode_TGenericNumbers , TGenericNumbers

    BYTE		ModifiedRedirectingPartyNumber_npi  :4;                 // 号码计划
    BYTE		ModifiedRedirectingPartyNumber_nai  :3;                 // 号码类型
    BYTE		s12               :1;    
    BYTE		ModifiedRedirectingPartyNumber_BCDlen;                 // 号码长度
    BYTE		ModifiedRedirectingPartyNumber[MAX_NUM_LEN/2];     // 改变后的RedirectingPartyNumber
    
    BYTE		ModifiedRedirectionCounter; 

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)freeFormatDataAppend
    BYTE		freeFormatDataAppend:2;//CAMEL_FCI_Data_Append_Ind :2; //表示是第一个FCI消息，0表示不是第一个  

    //(TermCAMELRecord)mscServerIndication//目前没有填写，预留比特位
    BYTE		mscServerIndication:1;    

    //(MOCallRecord)(TermCAMELRecord)defaultCallHandling-2 
    BYTE		defaultCallHandling2:2 ;//CAMEL_default_Call_Handling2;   
    BYTE		s13       :3;    
      
    //(MOCallRecord)(TermCAMELRecord)gsm-SCFAddress-2放在可变部分
    BYTE		CAMEL_GsmSCF_Address2_npi : 4;
    BYTE		CAMEL_GsmSCF_Address2_nai :  3; 
    BYTE		s14            : 1;   
    BYTE		CAMEL_GsmSCF_Address2_BCDlen;
    BYTE		CAMEL_GsmSCF_Address2[MAX_SCF_LEN]; 

    //(MOCallRecord)(TermCAMELRecord)serviceKey-2放在可变部分
    UINT4		ServiceKey2;                //业务键

    //(MOCallRecord)(TermCAMELRecord)freeFormatData-2放在最后长度可变

    //(MOCallRecord)(TermCAMELRecord)freeFormatDataAppend-2
    BYTE		freeFormatDataAppend2:2;//CAMEL_FCI_Data_Append_Ind2 :2;    
    BYTE		s15         :6;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)(TermCAMELRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  表明该次业务为电话业务（为“0”）或承载业务（为“1”）   
    BYTE		ROAM_mark       :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  依据TBS填电话业务码或承业务码。
    
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // 计费免费标志   
	BYTE		s17             :4;
    
    //(MOCallRecord)(TermCAMELRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 费率指示
    UINT2		money_per_count; // 费率
    UINT2		add_fee;                 //附加费

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)chargedParty 目前没有填写，先预留比特位
    BYTE        chargedParty; //enum MBILL_CHARGED_PARTY
    
    BYTE        UssdCallBackIndication:1;// ucb呼叫的真实主叫才记录此域
    BYTE        msc_id_len:3;//orgMSCId长度指示
    BYTE        initialCallAttemptFlag:1;//ICA呼叫标志
    BYTE        s18         :3;
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)originalCalledNumber 目前没有填写，预留
    BYTE		originalCalledNumber_npi  :4;                 // 号码计划    
    BYTE		originalCalledNumber_nai  :3;                 // 号码类型
    BYTE		s19               :1;    
    BYTE		originalCalledNumber_BCDlen;                 // 号码长度
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;

    BYTE		cause_for_partial_record;//cause_for_partial_record ;      //  出中间话单原因

    //新增容器用作后续扩展
    BYTE		container[MAX_CONTAINER_TCAMEL_LEN];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)freeFormatData FCI放在可变部分
    //(MOCallRecord)(TermCAMELRecord)freeFormatData-2 FCI放在可变部分
    BYTE		fcidata_len;
    BYTE		fcidata2_len;
    BYTE     VarPart[MAX_TRANSFER_FCI_LEN * 2];
    
	// BYTE		FCIData[MAX_TRANSFER_FCI_LEN];
    // BYTE		FCIData2[MAX_TRANSFER_FCI_LEN];

	
};

//1.4	ROAM_Bill_Report（包括ROAM（roamingRecord））
struct ROAM_Bill_Report
{
	// 公共域部分, 基本区:

    BYTE		validflag; // 有效性标志,　“AA”为有效，“55”为无效
    UINT2		size; // 本张话单长度
    UINT2		CRCCode; // CRC校验结果
    BYTE		bill_type; //  enum   GSM_BILL_TYPE, 协议中的recordType域
    UINT4		bill_sequence; // 话单流水号，唯一区分一张话单。
    BYTE		ModuleNum; // 产生话单的模块号
    UINT4		count; // GSM 计费当一次呼叫产生多张话单时count 是该次呼叫首话单流水号
    BYTE		sequence; // 单张话单为0，首张话单为1，其余类推
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 单张首张中间末张话单标识
    BYTE		HotBillingTag:1; //为cmcc测试增加的热计费标志
    BYTE		CRC_Way:2; // CRC校验方式
    BYTE		ChargeLevel:1;//计费精度指示:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//公共域部分的扩展空间
    BYTE		Comm_reserved;//公共域部分的扩展空间
	
	//  扩展区:
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  被计费移动用户IMSI码

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // 被计费移动用户MSISDN号
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callingNumber
    BYTE		callingNumber_npi  :4;                 // 主叫号码计划
    BYTE		callingNumber_nai  :3;                 // 主叫号码类型
    BYTE		s2               :1;    
    BYTE		callingNumber_BCDlen ;                 // 主叫号码长度
    BYTE		callingNumber[MAX_NUM_LEN/2];     // 主叫号码

    //(MOCallRecord)(MTCallRecord)(roamingRecord)roamingNumber
    BYTE		roamingNumber_npi : 4;
    BYTE		roamingNumber_nai :  3;    
    BYTE		s3            : 1;    
    BYTE		roamingNumber_BCDlen ;
    BYTE		roamingNumber[MAX_NUM_LEN/2] ;    //MSRN               //  移动被叫漫游号码

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;    
    BYTE		s4            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  本局MSC号

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mscIncomingROUTE/mscOutgoingROUTE
    BYTE		mscIncomingROUTE_ind : 4;//IT_trunk_group_ind : 4;    
    BYTE		mscOutgoingROUTE_ind : 4;//OT_trunk_group_ind : 4;    
    UINT2		mscIncomingROUTE_ROUTENumber;//IT_trunk_group;                     // 入中继群
    UINT2		mscOutgoingROUTE_ROUTENumber;//OT_trunk_group;                    // 出中继群
    BYTE		mscIncomingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//IT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];
    BYTE		mscOutgoingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//OT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];

    //(MOCallRecord)(MTCallRecord)(roamingRecord)(TermCAMELRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  表明该次业务为电话业务（为“0”）或承载业务（为“1”）    
    BYTE		ROAM_mark          :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  依据TBS填电话业务码或承业务码。

    BYTE		transparencyIndicator:2;  //transparency_indicator :2;  // 透明非透明指示   
    
    //(MOCallRecord)(MTCallRecord)(roamingRecord)transparencyIndicator
	BYTE        SORIndication	:1;//优选路由标志 
    BYTE		s6          :5;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)changeOfService 用事件话单或中间话单的方式出

    //(MOCallRecord)(MTCallRecord)(roamingRecord)supplServicesUsed(呼叫无关的补充业务用事件话单方式出,BAU合并)
    struct MBILL_SS_code1     SS_code1;          
    struct MBILL_SS_code2     SS_code2;          
    struct MBILL_SS_code3     SS_code3;          
    struct MBILL_SS_code4     SS_code4;          
    struct MBILL_SS_code5     SS_code5;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)seizureTime 
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)answerTime
    BYTE		initial_year;           // 通话初始时刻
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            //3
    BYTE		initial_minute;
    BYTE		initial_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)releaseTime
    BYTE		end_year; // 通话结束时间
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callDuration
    UINT4		callDuration;//conversation_time;                 // 通话时长

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)causeForTerm
    BYTE		causeForTerm;//cause_for_termination;        //  通话终止原因

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)SDiagnostics
    SDiagnostics     diagnostics;//uDiagnostics;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; 用于同一移动台发生的不同话务的本地识别号

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)sequenceNumber: 中间话单序列号,放在公共域部分, 基本区:sequence域

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)networkCallReference
    //当使用CAMEL业务时,同一话务所经的各网元产生的话单将含有此标识码(NCR),用以把这些话单关联到一起. 
    BYTE		NetworkCallReferenceLen;
    BYTE		NetworkcallReferenceNumber[maxCallReferenceNumberLength]; // 由NetworkCallReferenceLen指明长度(2G旧有的呼叫参考号)

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mSCAddress (跟NetworkCallReferenceLen配合使用)
    BYTE		MSCAddress_npi : 4;
    BYTE		MSCAddress_nai :  3;
    BYTE		s7            : 1;   
    BYTE		MSCAddress_BCDlen;
    BYTE		MSCAddress[MAX_MSC_NUM_IN_BYTES];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // 计费免费标志
    BYTE		callEmlppPriority   :4;
    
    //(MOCallRecord)(TermCAMELRecord)(roamingRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 费率指示
    UINT2		money_per_count; // 费率
    UINT2		add_fee;                 //附加费

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)chargedParty 目前没有填写，先预留比特位
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)originalCalledNumber 目前没有填写，预留
    BYTE		originalCalledNumber_npi  :4;                 // 号码计划
    BYTE		originalCalledNumber_nai  :3;                 // 号码类型    
    BYTE		s9               :1;    
    BYTE		originalCalledNumber_BCDlen;                 // 号码长度
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

    //(MOCallRecord)(MTCallRecord)(roamingRecord)callingChargeAreaCode 目前没有填写，预留//泰国特殊域    BYTE    CAC_num[MAX_CAC_LEN/2+1];
    BYTE		callingChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];

    //(MOCallRecord)(MTCallRecord)(roamingRecord)calledChargeAreaCode 目前没有填写，预留//泰国特殊域    BYTE    CAC_num[MAX_CAC_LEN/2+1];
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
    BYTE		msc_id_len:3;//orgMSCId长度指示
    BYTE		s10          :1;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)cUGInterlockCode
    UINT4		CUGInterlockCode;//CUGInterlockCode;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)cUGIndex
    UINT2		CUGIndex;

    BYTE		cause_for_partial_record;//cause_for_partial_record ;      //  出中间话单原因

    BYTE        eaSubscriberInfo[MAX_NAEA_CODE_IN_BYTE + 1];
    BYTE        selectedCIC[MAX_NAEA_CODE_IN_BYTE + 1];
    //新增容器用作后续扩展
    BYTE     container[MAX_CONTAINER_ROAM_LEN];
	
};

//1.5	Gateway_Bill_Report（包括GWO(outGatewayRecord)、GWI(incGatewayRecord)、Transit(TransitCallRecord)）
struct Gateway_Bill_Report
{
	// 公共域部分, 基本区:
	
    BYTE		validflag; // 有效性标志,　“AA”为有效，“55”为无效
    UINT2		size; // 本张话单长度
    UINT2		CRCCode; // CRC校验结果
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // 话单流水号，唯一区分一张话单。
    BYTE		ModuleNum; // 产生话单的模块号
    UINT4		count; // GSM 计费当一次呼叫产生多张话单时count 是该次呼叫首话单流水号
    BYTE		sequence; // 单张话单为0，首张话单为1，其余类推
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 单张首张中间末张话单标识
    BYTE		HotBillingTag:1; //为cmcc测试增加的热计费标志
    BYTE		CRC_Way:2; // CRC校验方式
    BYTE		ChargeLevel:1;//计费精度指示:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//公共域部分的扩展空间
    BYTE		Comm_reserved;//公共域部分的扩展空间	
	
	//  扩展区:
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  被计费移动用户IMSI码

    //servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  被计费移动用户移动台设备识别码

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)callingNumber
    BYTE		callingNumber_npi  :4;                 // 主叫号码计划
    BYTE		callingNumber_nai  :3;                 // 主叫号码类型 
    BYTE		s1               :1;    
    BYTE		callingNumber_BCDlen ;                 // 主叫号码长度
    BYTE		callingNumber[MAX_NUM_LEN/2];     // 主叫号码

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)calledNumber
    BYTE		calledNumber_npi  :4;//dialed_isdn_npi  :4;                 // 被叫号码计划
    BYTE		calledNumber_nai  :3;//dialed_isdn_nai  :3;                 // 被叫号码类型 
    BYTE		s2               :1;  
    BYTE		calledNumber_BCDlen ;//dialed_isdn_num  :5;                 // 被叫号码长度
    BYTE		calledNumber[MAX_NUM_LEN/2];  //dialed_num[MAX_NUM_LEN/2];     // 被叫号码
    
    //(incGatewayRecord)(OutGatewayRecord)roamingNumber
    BYTE		roamingNumber_npi : 4;
    BYTE		roamingNumber_nai :  3;   
    BYTE		s3            : 1;   
    BYTE		roamingNumber_BCDlen ;
    BYTE		roamingNumber[MAX_NUM_LEN/2] ;    //MSRN               //  移动被叫漫游号码

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;    
    BYTE		s4            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  本局MSC号

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)mscIncomingROUTE/mscOutgoingROUTE
    BYTE		mscIncomingROUTE_ind : 4;//IT_trunk_group_ind : 4;    
    BYTE		mscOutgoingROUTE_ind : 4;//OT_trunk_group_ind : 4;    
    UINT2		mscIncomingROUTE_ROUTENumber;//IT_trunk_group;                     // 入中继群
    UINT2		mscOutgoingROUTE_ROUTENumber;//OT_trunk_group;                    // 出中继群
    BYTE		mscIncomingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//IT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];
    BYTE		mscOutgoingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//OT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)seizureTime 
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)answerTime
    BYTE		initial_year; // 通话初始时刻
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            //3
    BYTE		initial_minute;
    BYTE		initial_second;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)releaseTime
    BYTE		end_year; // 通话结束时间
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)callDuration
    UINT4		callDuration;//conversation_time;                 // 通话时长

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)causeForTerm
    BYTE		causeForTerm;//cause_for_termination;        //  通话终止原因

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)SDiagnostics
    SDiagnostics     diagnostics;//uDiagnostics;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; 用于同一移动台发生的不同话务的本地识别号

    //(MOCallRecord)(MTCallRecord)(roamingRecord)(TermCAMELRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  表明该次业务为电话业务（为“0”）或承载业务（为“1”）   
    BYTE		s5       :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  依据TBS填电话业务码或承业务码。
    
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)sequenceNumber: 中间话单序列号,放在公共域部分, 基本区:sequence域

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // 计费免费标志   
    BYTE		s6          :4;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 费率指示
    UINT2		money_per_count; // 费率
    UINT2		add_fee;                 //附加费

    //(incGatewayRecord)(OutGatewayRecord)chargedParty 目前没有填写，先预留比特位
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)originalCalledNumber 目前没有填写，预留
    BYTE		originalCalledNumber_npi  :4;                 // 号码计划
    BYTE		originalCalledNumber_nai  :3;                 // 号码类型  
    BYTE		s7               :1;   
    BYTE		originalCalledNumber_BCDlen;                 // 号码长度
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

    //(OutGatewayRecord)(TransitCallRecord)mscOutgoingCircuit
    UINT2		mscOutgoingCircuit;

    //(incGatewayRecord)(TransitCallRecord)mscIncomingCircuit
    UINT2		mscIncomingCircuit;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)callEmlppPriority
    BYTE		callEmlppPriority:4;//CallEmlpp;//呼叫最终EMLPP优先级 （取值0-7)
    BYTE		reserved:4;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)eaSubscriberInfo 目前没有填写，预留
    BYTE		eaSubscriberInfo[MAX_NAEA_CODE_IN_BYTE + 1];
    
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)selectedCIC 目前没有填写，预留
    BYTE		selectedCIC[MAX_CIC_LEN_IN_BYTE];

     //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)cUGOutgoingAccessIndicator
    BYTE		CUGOutgoingAccessIndicator:2;//CUG_call_ind         :2; 
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)cUGOutgoingAccessUsed
    BYTE		CUGOutgoingAccessUsed:2;//CUGOutgoingAccessUsed   :2;
    BYTE		msc_id_len:3;//orgMSCId长度指示
    BYTE		s8        :1;    

   
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)cUGInterlockCode
    UINT4		CUGInterlockCode;//CUGInterlockCode;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)cUGIndex
    UINT2		CUGIndex;

    //recordExtensions
    BYTE		cause_for_partial_record ;      //  出中间话单原因
    BYTE		gsm_gsvn;                          // 1业务类别
    BYTE		SORIndication:1;
    BYTE		transparencyIndicator:2;   //透明非透明指示    

    BYTE		PromptAndCollectCharge:1;
    BYTE		transmission_mode:4;  // 2G话单使用的全速率半速率标志

    BYTE		subscriberCategory;

    BYTE        isdn_bc_len;
    BYTE        isdn_bc[MAX_ISDN_BC_LEN];
    BYTE        LLC_len;
    BYTE        LLC[MAX_LLC_LEN];
    BYTE        HLC_len;
    BYTE        HLC[MAX_HLC_LEN];

    
    //新增容器用作后续扩展
    BYTE		container[MAX_CONTAINER_GATEWAY_LEN];
	
};

//1.6	SMSMT_Bill_Report（包括SMS MT(MTSMSRecord)）
struct SMSMT_Bill_Report
{
	// 公共域部分, 基本区:

	BYTE		validflag; // 有效性标志,　“AA”为有效，“55”为无效
    UINT2		size; // 本张话单长度
    UINT2		CRCCode; // CRC校验结果
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // 话单流水号，唯一区分一张话单。
    BYTE		ModuleNum; // 产生话单的模块号
    UINT4		count; // GSM 计费当一次呼叫产生多张话单时count 是该次呼叫首话单流水号
    BYTE		sequence; // 单张话单为0，首张话单为1，其余类推
    BYTE		record_type :3; //enum GSM_RECORD_TY
    BYTE		HotBillingTag:1; //为cmcc测试增加的热计费标志
    BYTE		CRC_Way:2; // CRC校验方式
    BYTE		ChargeLevel:1;//计费精度指示:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//公共域部分的扩展空间
	BYTE		Comm_reserved;//公共域部分的扩展空间
	
	//  扩展区:	
    BYTE		serviceCentre_npi : 4;//SMS_address_npi : 4;
    BYTE		serviceCentre_nai :  3;//SMS_address_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		serviceCentre_BCDlen;//SMS_address_num : 5;
    BYTE		serviceCentre[MAX_MSC_NUM_IN_BYTES];  //SMS_address[8];    //短消息中心地址
    
    //(MOSMSRecord)(MTSMSRecord)servedIMSI
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  被计费移动用户IMSI码

    //(MOSMSRecord)(MTSMSRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  被计费移动用户移动台设备识别码

    //(MOSMSRecord)(MTSMSRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s2            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // 被计费移动用户MSISDN号
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MOSMSRecord)(MTSMSRecord)MSClassmark[3] 
    BYTE   MSClassmark[3];//struct  Classmark last_served_ms_classmark;      // 被计费移动用户当前CLASSMARK
    BillClassmark3 Classmark3Information;
    
    //(MOSMSRecord)(MTSMSRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;    
    BYTE		s3            : 1;   
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  本局MSC号

    //(MOSMSRecord)(MTSMSRecord)location 主被叫的此域合一
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
    SDiagnostics  smsResult;//参考map定义的SMSResult
    BYTE		actionResult;//操作结果，取SM_transmission_success,或 SM_transmission_failed 

    //(MOSMSRecord)(MTSMSRecord)systemType
    BYTE		systemType:2;//USER_NETWORK_TYPE :4;     
    BYTE		s4        :6;
    
    //(MOSMSRecord)(MTSMSRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  表明该次业务为电话业务（为“0”）或承载业务（为“1”）    
    BYTE		ROAM_mark         :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  依据TBS填电话业务码或承业务码。
    
    //(MOSMSRecord)(MTSMSRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // 计费免费标志
    BYTE		Classmark3Indication:1;
    BYTE		s6           :3;
    
    //(MOSMSRecord)(MTSMSRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 费率指示
    UINT2		money_per_count; // 费率
    UINT2		add_fee;                 //附加费

    //(MOSMSRecord)(MTSMSRecord)chargedParty  目前没有填写，先预留比特位
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId长度指示
    BYTE		msc_id_len:3;//orgMSCId长度指示
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

    //(MOSMSRecord)(MTSMSRecord)hotBillingTag放在基本区HotBillingTag

    //(MTSMSRecord)origination
    BYTE		callingNumber_npi  :4; //caller_isdn_npi                // 主叫号码计划
    BYTE		callingNumber_nai  :3;  //caller_isdn_nai               // 主叫号码类型
    BYTE		s7               :1;    
    BYTE		callingNumber_BCDlen ; //caller_isdn_num                // 主叫号码长度
    BYTE		callingNumber[MAX_NUM_LEN/2]; //caller_num    // 主叫号码

    //(MOSMSRecord)(MTSMSRecord)recordExtensions
    //  当前被计费移动用户所处MSC
    BYTE		served_msc_num_npi : 4;
    BYTE		served_msc_num_nai :  3;
    BYTE		s8            : 1;
    BYTE		served_msc_num_BCDlen ;
	BYTE		served_msc_num[MAX_MSC_NUM_IN_BYTES];       

    //新增容器用作后续扩展
    BYTE		container[MAX_CONTAINER_SMSMT_LEN];
    
    //(MOSMSRecord)(MTSMSRecord)smstext 
    BYTE		smstext_len;
    BYTE     	VarPart[MAX_SMSTEXT_LEN];
    // BYTE		smstext[MAX_SMSTEXT_LEN];

	
};

//1.7	SMSMO_Bill_Report（包括SMS MO(MOSMSRecord)）
struct SMSMO_Bill_Report
{
	// 公共域部分, 基本区:

	BYTE		validflag; // 有效性标志,　“AA”为有效，“55”为无效
    UINT2		size; // 本张话单长度
    UINT2		CRCCode; // CRC校验结果
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // 话单流水号，唯一区分一张话单。
    BYTE		ModuleNum; // 产生话单的模块号
    UINT4		count; // GSM 计费当一次呼叫产生多张话单时count 是该次呼叫首话单流水号
    BYTE		sequence; // 单张话单为0，首张话单为1，其余类推
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 单张首张中间末张话单标识
    BYTE		HotBillingTag:1; //为cmcc测试增加的热计费标志
    BYTE		CRC_Way:2; // CRC校验方式
    BYTE		ChargeLevel:1;//计费精度指示:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//公共域部分的扩展空间
    BYTE		Comm_reserved;//公共域部分的扩展空间
	
	//  扩展区:
    BYTE		serviceCentre_npi : 4;//SMS_address_npi : 4;
    BYTE		serviceCentre_nai :  3;//SMS_address_nai :  3;   
	BYTE		s1            : 1;    
    BYTE		serviceCentre_BCDlen;//SMS_address_num : 5;
    BYTE		serviceCentre[MAX_MSC_NUM_IN_BYTES];  //SMS_address[8];    //短消息中心地址
    
    //(MOSMSRecord)(MTSMSRecord)servedIMSI
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  被计费移动用户IMSI码

    //(MOSMSRecord)(MTSMSRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  被计费移动用户移动台设备识别码

    //(MOSMSRecord)(MTSMSRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;   
    BYTE		s2            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // 被计费移动用户MSISDN号
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MOSMSRecord)(MTSMSRecord)MSClassmark[3] 
    BYTE   MSClassmark[3];//struct  Classmark last_served_ms_classmark;      // 被计费移动用户当前CLASSMARK
    struct  BillClassmark3 Classmark3Information;
    
    //(MOSMSRecord)(MTSMSRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;    
    BYTE		s3            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  本局MSC号

    //(MOSMSRecord)(MTSMSRecord)location 主被叫的此域合一
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
    SDiagnostics  smsResult;//参考map定义的SMSResult
    BYTE		actionResult;//操作结果，取SM_transmission_success,或 SM_transmission_failed

    //(MOSMSRecord)destinationNumber
    BYTE		destinationNumber_npi :4;//dialed_isdn_npi  :4;                 // 被叫号码计划
    BYTE		destinationNumber_nai:3;//dialed_isdn_nai  :3;                 // 被叫号码类型 
    BYTE		s4               :1;    
    BYTE		destinationNumber_BCDlen; //dialed_isdn_num  :5;                 // 被叫号码长度
    BYTE		destinationNumber[MAX_NUM_LEN/2];//dialed_num[MAX_NUM_LEN/2];     //12 被叫号码

    //(MOSMSRecord)(MTSMSRecord)systemType
    BYTE		systemType:2;//USER_NETWORK_TYPE :4; 
    BYTE		s5               :6;

    //(MOSMSRecord)(MTSMSRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  表明该次业务为电话业务（为“0”）或承载业务（为“1”）    
    BYTE		ROAM_mark               :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  依据TBS填电话业务码或承业务码。
    
    //(MOSMSRecord)(MTSMSRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // 计费免费标志
    BYTE		Classmark3Indication:1;
    BYTE		s7                :3;
   
    //(MOSMSRecord)(MTSMSRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 费率指示
    UINT2		money_per_count; // 费率
    UINT2		add_fee;                 //附加费

    //(MOSMSRecord)(MTSMSRecord)chargedParty  目前没有填写，先预留比特位
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId长度指示
    BYTE		msc_id_len:3;//orgMSCId长度指示
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

    //(MOSMSRecord)(MTSMSRecord)hotBillingTag放在基本区HotBillingTag

    //CAMEL SMS的智能信息
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

    //CAMELSMSInformation:smsReferenceNumber 目前没有填写，预留
    BYTE		smsReferenceNumberLen;
    BYTE		smsReferenceNumber[maxCallReferenceNumberLength];
    
    //(MOSMSRecord)(MTSMSRecord)recordExtensions
    //  当前被计费移动用户所处MSC
    BYTE		served_msc_num_npi : 4;
    BYTE		served_msc_num_nai :  3;
    BYTE		s12            : 1;
    BYTE		served_msc_num_BCDlen ;
    BYTE		served_msc_num[MAX_MSC_NUM_IN_BYTES];       

    //(MOSMSRecord)(MTSMSRecord)recordExtensions
    //新增容器用作后续扩展
    BYTE		container[MAX_CONTAINER_SMSMO_LEN];

    //CAMELSMSInformation:freeFormatData
    //(MOSMSRecord)(MTSMSRecord)smstext 目前没有填写，预留
    BYTE		fcidata_len;
    BYTE		smstext_len;
    BYTE		VarPart[MAX_TRANSFER_FCI_LEN + MAX_SMSTEXT_LEN];
    // BYTE		FCIData[MAX_TRANSFER_FCI_LEN];
    // BYTE		smstext[MAX_SMSTEXT_LEN];	
};

//1.8	HLR_Query_Bill_Report（包括HLR Query(HLRIntRecord)）
struct HLR_Query_Bill_Report
{
	// 公共域部分, 基本区:

	BYTE		validflag; // 有效性标志,　“AA”为有效，“55”为无效
    UINT2		size; // 本张话单长度
    UINT2		CRCCode; // CRC校验结果
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // 话单流水号，唯一区分一张话单。
    BYTE		ModuleNum; // 产生话单的模块号
    UINT4		count; // GSM 计费当一次呼叫产生多张话单时count 是该次呼叫首话单流水号
    BYTE		sequence; // 单张话单为0，首张话单为1，其余类推
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 单张首张中间末张话单标识
    BYTE		HotBillingTag:1; //为cmcc测试增加的热计费标志
    BYTE		CRC_Way:2; // CRC校验方式
    BYTE		ChargeLevel:1;//计费精度指示:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//公共域部分的扩展空间
    BYTE		Comm_reserved;//公共域部分的扩展空间
	
	//  扩展区:
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  被计费移动用户IMSI码
	
    //(HLRIntRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // 被计费移动用户MSISDN号
	BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(HLRIntRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//served_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//served_msc_num_nai :  3;   
    BYTE		s2            : 1;   
    BYTE		recordingEntity_BCDlen ;//served_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //served_msc_num[8];         //8  当前被计费移动用户所处MSC
	
    //(HLRIntRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  表明该次业务为电话业务（为“0”）或承载业务（为“1”）    
    BYTE		s3            :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  依据TBS填电话业务码或承业务码。
	
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
	
    //(HLRIntRecord)interrogationResult 没有填写，先预留字节
    SDiagnostics     interrogationResult;
	
    //(HLRIntRecord)orgMSCId
    BYTE		msc_id_len:3;//orgMSCId长度指示
    BYTE		s6        :5;
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];
    
    //(HLRIntRecord)recordExtensions
    //新增容器用作后续扩展
    BYTE		container[MAX_CONTAINER_HLRQUERY_LEN];
	
};

//1.9	SS_Action_Bill_Report（包括SS Action(SSActionRecord)、EVENT）
struct SS_Action_Bill_Report
{
	// 公共域部分, 基本区:

	BYTE		validflag; // 有效性标志,　“AA”为有效，“55”为无效
    UINT2		size; // 本张话单长度
    UINT2		CRCCode; // CRC校验结果
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // 话单流水号，唯一区分一张话单。
    BYTE		ModuleNum; // 产生话单的模块号
    UINT4		count; // GSM 计费当一次呼叫产生多张话单时count 是该次呼叫首话单流水号
    BYTE		sequence; // 单张话单为0，首张话单为1，其余类推
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 单张首张中间末张话单标识
    BYTE		HotBillingTag:1; //为cmcc测试增加的热计费标志
    BYTE		CRC_Way:2; // CRC校验方式
    BYTE		ChargeLevel:1;//计费精度指示:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//公共域部分的扩展空间
    BYTE		Comm_reserved;//公共域部分的扩展空间
	
	//  扩展区:
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  被计费移动用户IMSI码

    //(SSActionRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  被计费移动用户移动台设备识别码

    //(SSActionRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;    
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // 被计费移动用户MSISDN号
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(SSActionRecord)MSClassmark[3]
    BYTE   MSClassmark[3];//BYTE  ClassMark2;   
    struct  BillClassmark3 Classmark3Information;
    
    //(SSActionRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//    BYTE     served_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//    BYTE     served_msc_num_nai :  3;    
    BYTE		s2            : 1;       
    BYTE		recordingEntity_BCDlen ;//    BYTE     served_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //    BYTE    served_msc_num[8];       //8  当前被计费移动用户所处MSC

    //(SSActionRecord)location 主被叫的此域合一
    BYTE		served_location_LAI[MAX_LOCAL_LAI_LEN/2];//caller_LAC[MAX_LOCAL_LAC_LEN/2];called_LAC[MAX_LOCAL_LAC_LEN/2]; 
    BYTE		served_location_cellID[MAX_LOCAL_CI_LEN/2];//caller_cell_ID[MAX_LOCAL_CI_LEN/2]; called_cell_ID[MAX_LOCAL_CI_LEN/2]; 

    //(SSActionRecord)BasicServices
    BYTE		TBS:4;//TBS:4;        //  表明该次业务为电话业务（为“0”）或承载业务（为“1”）    
    BYTE		s3          :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  依据TBS填电话业务码或承业务码。

    //(SSActionRecord)supplService
    BYTE		SSCode;

    //(SSActionRecord)ssAction
    BYTE		ssAction;//SSAction; //SSActionType

    //(SSActionRecord)ssActionTime
    BYTE		ssActionTime_year; //补充业务动作发生的时间戳
    BYTE		ssActionTime_month;
    BYTE		ssActionTime_date;
    BYTE		ssActionTime_hour;            //3
    BYTE		ssActionTime_minute;
    BYTE		ssActionTime_second;

    //(SSActionRecord)ssActionResult
    SDiagnostics		ssActionResult;//SSResult;

    //(SSActionRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; 用于同一移动台发生的不同话务的本地识别号

   //(SSActionRecord)systemType
    BYTE		systemType:2;//AccessNetworkType;     
    //(SSActionRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // 计费免费标志
    BYTE		Classmark3Indication:1;
    BYTE		s4 :1;    
     
    //(SSActionRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 费率指示
    UINT2		money_per_count; // 费率
    UINT2		add_fee;                 //附加费

    //(SSActionRecord)chargedParty 目前没有填写，先预留比特位
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId长度指示
    BYTE		msc_id_len:3;//orgMSCId长度指示
    BYTE		s23            : 2;
    //(SSActionRecord)orgRNCorBSCId
    BYTE		orgRNCorBSCId[MAX_RNCBSC_ID_LEN];//Rnc_Bsc_Id[3];

    //(SSActionRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(SSActionRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;

    //(SSActionRecord)recordExtensions
    BYTE		event_type;  //记录产生事件话单的事件类型:CH,CW,MPTY,HO: enum EVENT_TYPE    

    //ussd相关信息,参考西门子的接口，预留
    BYTE		UssdAccessCode[MAX_USSDACCESSCODE_LEN];
    BYTE		UssdCodingScheme;
    BYTE		UssdNotifyCounter ;
    BYTE		UssdRequestCounter;  

    // Deleted by zengsonglin for R002C01 2004-04-07
    // BYTE     UssdString[MAXUINT2SDSTRING_LEN];

    //changeofAOC采用事件话单处理，为此增加下述话单域
    AOC_E1_E7    aocParameters;  

    //新增容器用作后续扩展
    BYTE     container[MAX_CONTAINER_SSACT_LEN];

    //(SSActionRecord)ssParameters 
    SSParameters   SSParameter;
    /*VarPart
    注意SSParameters本身就是可变长度的
    即SS_Para_len为可变部分长度指示
    可变部分最大长度为40字节
    */	
};

//1.10	CEU_Bill_Report（包括CEU(CommonEquipRecord)）
struct CEU_Bill_Report
{
	// 公共域部分, 基本区:
	BYTE		validflag; // 有效性标志,　“AA”为有效，“55”为无效
    UINT2		size; // 本张话单长度
    UINT2		CRCCode; // CRC校验结果
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // 话单流水号，唯一区分一张话单。
    BYTE		ModuleNum; // 产生话单的模块号
    UINT4		count; // GSM 计费当一次呼叫产生多张话单时count 是该次呼叫首话单流水号
    BYTE		sequence; // 单张话单为0，首张话单为1，其余类推
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 单张首张中间末张话单标识
    BYTE		HotBillingTag:1; //为cmcc测试增加的热计费标志
    BYTE		CRC_Way:2; // CRC校验方式
    BYTE		ChargeLevel:1;//计费精度指示:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//公共域部分的扩展空间
    BYTE		Comm_reserved;//公共域部分的扩展空间
	
	//  扩展区:
	
    BYTE		equipmentType;//Equipment_type;

    //(CommonEquipRecord)equipmentId
    UINT2		equipmentId;//Equipment_Id;

    //(CommonEquipRecord)servedIMSI
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  被计费移动用户IMSI码

    BYTE		servedIMEI[MAX_IMEI_LEN];  //  被计费移动用户移动台设备识别码

    //(CommonEquipRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // 被计费移动用户MSISDN号
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(CommonEquipRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//served_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//served_msc_num_nai :  3;   
    BYTE		s2            : 1;    
    BYTE		recordingEntity_BCDlen ;//served_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //served_msc_num[8];          //8  当前被计费移动用户所处MSC

    //(CommonEquipRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  表明该次业务为电话业务（为“0”）或承载业务（为“1”）    
    BYTE		s3     :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  依据TBS填电话业务码或承业务码。
   //(CommonEquipRecord)supplServicesUsed
    BYTE		SSCode;
    //(CommonEquipRecord)seizureTime
    BYTE		initial_year;    // 通话初始时刻
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            //3
    BYTE		initial_minute;
    BYTE		initial_second;

    //(CommonEquipRecord)releaseTime 
    BYTE		end_year;               // 通话结束时间
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(CommonEquipRecord)callDuration
    UINT4		callDuration;//Call_Duration; 通话时长

    //(CommonEquipRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; 用于同一移动台发生的不同话务的本地识别号

    //(CommonEquipRecord)sequenceNumber: 中间话单序列号,放在公共域部分, 基本区:sequence域

    //(CommonEquipRecord)systemType 
    BYTE		systemType:2;//AccessNetworkType     
    BYTE		s4       :6;
    
    //(CommonEquipRecord)rateIndication
    BYTE		rateIndication;//ucRateIndication; //数据业务速率适配

    //(CommonEquipRecord)fnur
    BYTE		fnur;//ucFunr;           //数据业务用户速率,enum FNUR

    //(CommonEquipRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // 计费免费标志    
 	BYTE		s5         :4;
   
    //(CommonEquipRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 费率指示
    UINT2		money_per_count; // 费率
    UINT2		add_fee;                 //附加费

    //(CommonEquipRecord)chargedParty 目前没有填写，先预留比特位
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId长度指示
    BYTE		msc_id_len:3;//orgMSCId长度指示
    BYTE		s23            : 2;
    //(CommonEquipRecord)orgRNCorBSCId
    BYTE		orgRNCorBSCId[MAX_RNCBSC_ID_LEN];//Rnc_Bsc_Id[3];

    //(CommonEquipRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(CommonEquipRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;


    BYTE		causeForTerm;//cause_for_termination;        //  通话终止原因
    SDiagnostics     diagnostics;//uDiagnostics;
    BYTE		cause_for_partial_record ;      //  出中间话单原因

    //新增容器用作后续扩展
    BYTE		container[MAX_CONTAINER_CEU_LEN];
	
};

//1.11	LCS_Bill_Report（包括MO LCS(MOLCSRecord)、MT LCS(MTLCSRecord)、NI LCS(NILCSRecord)）
struct LCS_Bill_Report
{
	// 公共域部分, 基本区:
	BYTE		validflag; // 有效性标志,　“AA”为有效，“55”为无效
    UINT2		size; // 本张话单长度
    UINT2		CRCCode; // CRC校验结果
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // 话单流水号，唯一区分一张话单。
    BYTE		ModuleNum; // 产生话单的模块号
    UINT4		count; // GSM 计费当一次呼叫产生多张话单时count 是该次呼叫首话单流水号
    BYTE		sequence; // 单张话单为0，首张话单为1，其余类推
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 单张首张中间末张话单标识
    BYTE		HotBillingTag:1; //为cmcc测试增加的热计费标志
    BYTE		CRC_Way:2; // CRC校验方式
    BYTE		ChargeLevel:1;//计费精度指示:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//公共域部分的扩展空间
    BYTE		Comm_reserved;//公共域部分的扩展空间
	
	//  扩展区:
	
    BYTE		lcs_bill_type;//location_type;             // 1 enum LOCATION_TYPE: MT、MO、NI
    
    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//served_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//served_msc_num_nai :  3;
    BYTE		s1            : 1;    
    BYTE		recordingEntity_BCDlen ;//served_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //served_msc_num[8];         //8  当前被计费移动用户所处MSC

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)lcsClientType
    BYTE		lcsClientType;//LCSClientType;  //enum LCSClientType

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)lcsClientIdentity  
    BYTE		lcsClientIdentity_npi : 4;//lcs_client : 4;
    BYTE		lcsClientIdentity_nai : 3;//lcs_client :  3; 
    BYTE		s2            : 1;    
    BYTE		lcsClientIdentity_BCDlen ;//lcs_client : 5;
    BYTE		lcsClientIdentity[MAX_LCS_CLIENT_EXTERNAL_ID_LEN];//    BYTE    lcs_client[MAX_LCS_CLIENT_EXTERNAL_ID_LEN]; //20 外部的LCS-CLIENT编码

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)servedIMSI
    BYTE		servedIMSI[MAX_IMSI_LEN];  //    BYTE    target_IMSI[MAX_IMSI_LEN];          //目标IMSI号码，8字节

    //(NILCSRecord)servedIMEI  
    BYTE		servedIMEI[MAX_IMEI_LEN];//    BYTE    target_IMEI[MAX_IMEI_LEN];          //目标IMEI号码，8字节

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;//target_num_npi : 4;
    BYTE		servedMSISDN_nai :  3;//target_num_nai :  3;    
    BYTE		s3            : 1;    
    BYTE		servedMSISDN_BCDlen;//target_num_num : 5;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//target_num [MAX_NUM_LEN/2+1]; //目标号码，13字节
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MTLCSRecord)locationType
    BYTE		locationType;//LocationEstimateType;

    //(MOLCSRecord)molr-Type
    BYTE		molr_Type;//MOLRType;  //enum MOLR_Type

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)lcsQos 
    //QOS 定位精度要求，响应时间等
    BYTE		horizontal_accuracy; //水平精度
    BYTE		verticalCoordinateRequest; //0不要求，1要求
    BYTE		vertical_accuracy;   //垂直方向精度
    BYTE		responseTime; //定位响应时间

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)lcsPriority
    BYTE		lcsPriority;//lcs_priority;                 //  定位优先级

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)mlc-Number
    BYTE		GMLC_Number_npi : 4;//GMLC_address_npi : 4;
    BYTE		GMLC_Number_nai :  3;//GMLC_address_nai :  3;    
    BYTE		s4            : 1;    
    BYTE		GMLC_Number_BCDlen;//GMLC_address_num : 5;
    BYTE		GMLC_Number[MAX_GMLC_LEN + 1];//GMLC_address[9]; //9 GMLC编码    

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)eventTimeStamp
    BYTE		eventTimeStamp_year;     //initial_year; // 通话初始时刻
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
    BYTE		served_location_LAI[MAX_LOCAL_LAI_LEN/2]; //target_LAC[MAX_LOCAL_LAC_LEN/2];    // 目标位置区号，2字节
    BYTE		served_location_cellID[MAX_LOCAL_CI_LEN/2];//target_cell_ID[MAX_LOCAL_CI_LEN/2]; // 目标小区号，2字节

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
    BYTE		chargeIndicator:4;//free_indicator :4;  // 计费免费标志   
    BYTE		s6            :4;

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // 费率指示
    UINT2		money_per_count; // 费率
    UINT2		add_fee;                 //附加费

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)chargedParty 目前没有填写，先预留比特位
    BYTE	    chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId长度指示
    BYTE		msc_id_len:3;//orgMSCId长度指示
    BYTE		s23            : 2;
    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)orgRNCorBSCId
    BYTE		orgRNCorBSCId[MAX_RNCBSC_ID_LEN];//Rnc_Bsc_Id[3];

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;

    //(MOLCSRecord)location_method
    BYTE		location_method;                 //定位方法 enum _locaton_method
    
    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)recordExtensions

    //(NILCSRecord)emsDigits目前没有填写，预留
    BYTE		emsDigits_npi : 4;
    BYTE		emsDigits_nai :  3;   
    BYTE		s7            : 1;    
    BYTE		emsDigits_BCDlen;
    BYTE		emsDigits[MAX_NUM_LEN/2];

    //(NILCSRecord)emsKey目前没有填写，预留
    BYTE		emsKey_npi : 4;
    BYTE		emsKey_nai :  3;  
    BYTE		s8            : 1;    
    BYTE		emsKey_BCDlen;
    BYTE		emsKey[MAX_NUM_LEN/2];

    //新增容器用作后续扩展
    BYTE		container[MAX_CONTAINER_LCS_LEN];

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)positioningData 
    BYTE		positioningDataLen;//    BYTE     shapeinfoLen;
    
    BYTE        call_reference_len;
    BYTE        call_reference[maxCallReferenceNumberLength];
    
    BYTE		VarPart[maxShapeInfoLength];//BYTE     shapeInfo[maxShapeInfoLength];
    /*注意
    VarPart: positioningData就是可变部分,最大长度为maxShapeInfoLength
    */
	
};

//1.12	CheckIMEI_Bill_Report（包括CHECK IMEI）
struct CheckIMEI_Bill_Report
{
	// 公共域部分, 基本区:

	BYTE		validflag; // 有效性标志,　“AA”为有效，“55”为无效
    UINT2		size; // 本张话单长度
    UINT2		CRCCode; // CRC校验结果
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // 话单流水号，唯一区分一张话单。
    BYTE		ModuleNum; // 产生话单的模块号
    UINT4		count; // GSM 计费当一次呼叫产生多张话单时count 是该次呼叫首话单流水号
    BYTE		sequence; // 单张话单为0，首张话单为1，其余类推
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, 单张首张中间末张话单标识
    BYTE		HotBillingTag:1; //为cmcc测试增加的热计费标志
    BYTE		CRC_Way:2; // CRC校验方式
    BYTE		ChargeLevel:1;//计费精度指示:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//公共域部分的扩展空间
    BYTE		Comm_reserved;//公共域部分的扩展空间
	
	//  扩展区:
	
    BYTE		checkIMEI_year;          //initial_year; // 通话初始时刻
    BYTE		checkIMEI_month;       //initial_month;
    BYTE		checkIMEI_date;       //initial_date;
    BYTE		checkIMEI_hour;          //initial_hour;            
    BYTE		checkIMEI_minute;       //initial_minute;
    BYTE		checkIMEI_second;       //initial_second;
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  被计费移动用户IMSI码
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  被计费移动用户移动台设备识别码
	
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;   
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // 被计费移动用户MSISDN号
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    BYTE		served_location_LAI[MAX_LOCAL_LAI_LEN/2]; //target_LAC[MAX_LOCAL_LAC_LEN/2];    // 目标位置区号，5字节
    BYTE		served_location_cellID[MAX_LOCAL_CI_LEN/2]; //target_cell_ID[MAX_LOCAL_CI_LEN/2]; // 目标小区号，2字节
    
    BYTE		IMEIStatus;  //enum _Equipment_Status
    BYTE		Event;  //enum IMEICheckEvent
    
    BYTE		recordingEntity_npi : 4;//served_msc_npi : 4;
    BYTE		recordingEntity_nai : 3;//served_msc_nai :  3;  
    BYTE		s2            : 1;    
    BYTE		recordingEntity_BCDlen ;//served_msc_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //served_msc[8];         //服务MSC编码
	
    BYTE		msc_id_len:3;//orgMSCId长度指示
    BYTE		s3            : 5;
    BYTE		orgMSCId[MAX_MSCID_LEN];
	
    BYTE		CallReferenceLen;
    BYTE		CallReference[maxCallReferenceNumberLength];
	
    //新增容器用作后续扩展
    BYTE		container[MAX_CONTAINER_CHECKIMEI_LEN];
	
};

#pragma pack()

#endif  //__MSCV9_BILL_FORMAT_H__
