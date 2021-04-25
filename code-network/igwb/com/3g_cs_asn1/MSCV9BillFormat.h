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
    ACCESS_BSC  = 0,   // 2G����
    ACCESS_RNC  = 1,   // 3G����
    ACCESS_UNKNOWN = 2, //δ֪���� 
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
    MOC					= 0,	//�ƶ�ʼ������
    MTC					= 1,	//�ƶ��ս����
    GATEWAY_INCOMING	= 2,	//��ؿھֻ���
    EC					= 3,	//��������
    TRANSIT				= 4,	//��ת����Ӻ���
    SS_ACT				= 5,	//����ҵ���̶�����ע�ᡢע�������ȥ��
    MT_SMS				= 6,	//�ƶ�̨�ս����Ϣҵ��
    MO_SMS				= 7,    //�ƶ�̨ʼ������Ϣҵ��
    LOCATION_FREASH		= 8,    //λ�ø���
    CALL_ATTEMPT		= 9,    //�Ժ�����
	GATEWAY_OUTGOING	= 10,   //���ؿھֻ���  
    ISDN_OC             = 11,   
    ISDN_TC             = 12,   
    ROAM                = 13,   //���λ���
    CommonEquip         = 14,   //�����豸����
    MO_FREECALL         = 17,   //���������û���ѻ�������
    CFW					= 18,   //ǰת����
    MOI					= 19,   //��������
    MO_HO               = 20,   //�����л�����
    MT_HO               = 21,   //�����л�����
    MT_FREECALL         = 22,   //���������û���ѻ�������
    CFW_FREECALL        = 23,   // ����ǰתʱԭ���������û���ѻ������� 
    TCAMEL				= 24,   //�ս�CAMEL���ʼ�¼
    FCI					= 25,   //���ɸ�ʽ���ݻ���
 	LCS					= 26,   //λ��ҵ�񻰵� 
    Check_IMEI			= 27,   //Check IMEI��������
    QUERY_HLR			= 28,   //�ؿھ�HLR��ѯ��������
    EVENT_BILL          = 30
};

enum HOT_BILLING_MARK
{
    NORMAL_BILLING     = 0x00,        //��ͨ�Ʒ�
    HOT_BILLING        = 0x01         //�ȼƷ�
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
    SINGLE_BILL,            //���κ���ֻ����һ�Ż���
    FIRST_BILL,             //���κ��е�һ�Ż���
    INTERMEDIATE_BILL,      //�м仰��
    LAST_BILL               //���κ������һ�Ż���
};

enum MBILL_TBS
{
    MBILL_TELECOM_SERVICE,  //����ҵ��
    MBILL_BEAR_SERVICE,     //����ҵ��
    MBILL_ISDN_SERVICE,     // ISDNҵ��
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

//by    ����    2004-6-2    ��Ӧ���ⵥSWPD05614
struct MSC_Bill_Report
{
   	// �����򲿷�, ������:
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
//the end  ��Ӧ���ⵥSWPD05614

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

//by    ����    2004-6-2    ��Ӧ���ⵥSWPD05614
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
//the end  ��Ӧ���ⵥSWPD05614

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
// SS_code2�ֶα�ʾ���²���ҵ��
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

// SS_code3�ֶα�ʾ���²���ҵ��
struct MBILL_SS_code3
{
        BYTE   reserve4      :1;
        BYTE   reserve3      :1;
        
        //by    ����    2004-6-2    ��Ӧ���ⵥSWPD05614
        BYTE   CLIR			 :1;
        BYTE   CLIP          :1;
        //the end  ��Ӧ���ⵥSWPD05614

        BYTE   ALLMOLR       :1;
        BYTE   UUS3          :1;
        BYTE   UUS2          :1;
        BYTE   UUS1          :1;
};
// SS_code4�ֶα�ʾ���²���ҵ��
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
// SS_code5�ֶα�ʾ���²���ҵ��
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
// SS_code2�ֶα�ʾ���²���ҵ��
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

// SS_code3�ֶα�ʾ���²���ҵ��
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
// SS_code4�ֶα�ʾ���²���ҵ��
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
// SS_code5�ֶα�ʾ���²���ҵ��
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

//1.1	MOC_Bill_Report������MOC(moCallRecord) /CFW(forwardCallRecord:�ӿ�ͬMOCallRecord)/MOI��
struct MOC_Bill_Report
 {
   	// �����򲿷�, ������:
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
	//  ��չ��:
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

    BYTE		TBS:4;//TBS:4;        //  �����ô�ҵ��Ϊ�绰ҵ��Ϊ��0���������ҵ��Ϊ��1����    
	BYTE		ROAM_mark       :4;    
    BYTE		ServiceCode;//tele_bearer_code;  //  ����TBS��绰ҵ������ҵ���롣

    BYTE		transparencyIndicator:2;  //transparency_indicator :2;  // ͸����͸��ָʾ   
	BYTE		s9         :6;

    struct MBILL_SS_code1     SS_code1;          
    struct MBILL_SS_code2     SS_code2;          
    struct MBILL_SS_code3     SS_code3;          
    struct MBILL_SS_code4     SS_code4;          
    struct MBILL_SS_code5     SS_code5;

	AOC_E1_E7	aocParameters;

	BYTE		MSClassmark[3];// ��ԭ����struct  Classmark initial_served_ms_classmark�����Ʒ��ƶ��û���ʼCLASSMARK��struct  Classmark������Э���ΪBYTE
	BYTE		changeOfClassmark[3];//struct  Classmark last_served_ms_classmark;  //���Ʒ��ƶ��û��ı���CLASSMARK

	BYTE		initial_year;           // ͨ����ʼʱ��
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            
    BYTE		initial_minute;
    BYTE		initial_second;

	BYTE		end_year; // ͨ������ʱ��
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

	UINT4		callDuration;//conversation_time;                 // ͨ��ʱ��
	
    BYTE		radioChanRequested:4;//USER_Radio_channelUINT2ed :4;                //   ȫ���ʻ������ 
    BYTE		radioChanUsed:2; 	
	BYTE		s10           :2; 
	   
	BYTE		causeForTerm;//cause_for_termination;        //  ͨ����ֹԭ��
	
	SDiagnostics     diagnostics;//uDiagnostics;
	BYTE		call_reference_len;
	BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; ����ͬһ�ƶ�̨�����Ĳ�ͬ����ı���ʶ���

    BYTE		chargeIndicator:4;//free_indicator :4;  // �Ʒ���ѱ�־    
  	BYTE		s11          :4;

    
    //(MOCallRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ����ָʾ
    UINT2		money_per_count; // ����
    UINT2		add_fee;
	
	BYTE		chargedParty;

    BYTE		CAMEL_GsmSCF_Address_npi : 4;
    BYTE		CAMEL_GsmSCF_Address_nai :  3;
	BYTE		s12            : 1;    
	
    BYTE		CAMEL_GsmSCF_Address_BCDlen;
    BYTE		CAMEL_GsmSCF_Address[MAX_SCF_LEN]; 

	UINT4		ServiceKey;                //ҵ���

	BYTE		NetworkCallReferenceLen;
    BYTE		NetworkcallReferenceNumber[maxCallReferenceNumberLength];

	//(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mSCAddress (��NetworkCallReferenceLen���ʹ��)
    BYTE		MSCAddress_npi : 4;
    BYTE		MSCAddress_nai : 3;
    BYTE		s13            : 1;   
    
    BYTE		MSCAddress_BCDlen;
    BYTE		MSCAddress[MAX_MSC_NUM_IN_BYTES];

	BYTE		CAMELInitCFIndicator:1;
	BYTE		defaultCallHandling:2;
	BYTE		s14            : 5;
	
	BYTE		fnur;//ucFunr;           //����ҵ���û�����,enum FNUR
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
    UINT4		ServiceKey2;                //ҵ���
	
	BYTE		freeFormatDataAppend2:2;
	BYTE		freeFormatDataAppend:2;
	BYTE		defaultCallHandling2:2 ;
	BYTE		systemType:2;
	BYTE		rateIndication;

    BYTE		originalCalledNumber_npi  :4;                 // ����ƻ�
	BYTE		originalCalledNumber_nai  :3; 
	BYTE		s18               :1;    
                // ��������
    BYTE		originalCalledNumber_BCDlen;                 // ���볤��
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

	BYTE		callingChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];

	BYTE		calledChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];
	
	UINT2		mscOutgoingCircuit;
    UINT2		mscIncomingCircuit;
	
    BYTE		rnc_bsc_id_len:3;
    BYTE		msc_id_len:3;//orgMSCId����ָʾ
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
	
	BYTE	    CFW_invoked_times:4;    // ǰת�����Ĵ���    
	BYTE		CH_invoked_times:4;        // ���б���ҵ�����Ĵ���
    BYTE		CH_retrieve_times:4;       // ���б���ҵ��ָ��Ĵ���    
    BYTE		CW_invoked_times:4;        // ���еȴ�ҵ�����Ĵ���
    BYTE		MPTY_Build_times:4;        // �෽ͨ��ҵ���������Ĵ���    
    BYTE		MPTY_CH_times:4;           // �෽ͨ��ҵ�񱣳ַ����Ĵ���
    BYTE		MPTY_CH_retrieve_times:4;   // �෽ͨ��ҵ�񱣳ָֻ������Ĵ���   
    BYTE		MPTY_Split_times:4;        // �෽ͨ��ҵ����뷢���Ĵ���
	
	BYTE		HO_invoked_times;         // �л������Ĵ���
	BYTE		AOCParmsChangedTimes:4; 
	BYTE		ClassmarkChangedTimes:4;
    BYTE		RadioChanChangedTimes:4;	
	BYTE		transmission_mode:4;   // 2G����ʹ�õ�  ȫ���ʻ������ ��־��

	BYTE		Info_trans_capability;             // ��������
    BillClassmark3 Classmark3Information;    
	
	BYTE		DTMF_indicator :2;  // �Ƿ�ʹ��DTMF
	BYTE		user_type :5;  //   �ƶ�̨���ȼ�  
	BYTE		s8 :1;  //   0 ��ʾ�����û���1 ��ʾ�����û��
	
  
    BYTE		EmergencyCallFlag     :1;          // �������б��   
    BYTE		PromptAndCollectCharge :1;               //��Դ�Ʒѻ�����־��ǰ��s6��־
    BYTE		SORIndication:1;
 	BYTE		s21:1;
	BYTE		EarlyForwardSORIndication	:1;
	BYTE		LateForwardSORIndication	:1; 	
    BYTE		s20                   :2;
    // BYTE		UssdCallBackIndication:1; 
    
    BYTE		ModemType:7;
	BYTE		Classmark3Indication:1;   
    
    BYTE		cause_for_partial_record;//cause_for_partial_record ;      //  ���м仰��ԭ��
    BYTE		transaction_id;// ����ID,����ͬһ�ƶ�̨�����Ĳ�ͬ����ı���ʶ���
    BYTE		gsm_gsvn;                          // 1ҵ�����
    BYTE		B_ch_number;       // �ôκ�����ռ�õ�B�ŵ���
	
    BYTE		scfID[MAX_SCFID_LEN]; 

    BYTE		served_msc_num_npi : 4;
    BYTE		served_msc_num_nai :  3;
	BYTE		s22            : 1;
    BYTE		served_msc_num_BCDlen ;
	BYTE		served_msc_num[MAX_MSC_NUM_IN_BYTES];  
	
	BYTE		GuaranteedBitRate;  //��֤������
	BYTE		MaximumBitRate;     //��������

    //�����ֶ�
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

	//�ɱ䲿��
	
	BYTE		fcidata_len;
    BYTE		fcidata2_len;
    BYTE     VarPart[MAX_TRANSFER_FCI_LEN * 2];
   
	/*
    VarPart���������������
    BYTE FCIData[MAX_TRANSFER_FCI_LEN];
    BYTE FCIData2[MAX_TRANSFER_FCI_LEN];
    */

};

//1.2	MTC_Bill_Report������MTC(mtCallRecord)��
struct MTC_Bill_Report
{
	// �����򲿷�, ������:
	BYTE		validflag; // ��Ч�Ա�־,����AA��Ϊ��Ч����55��Ϊ��Ч
    UINT2		size; // ���Ż�������
    UINT2		CRCCode; // CRCУ����
    BYTE		bill_type; //  enum   GSM_BILL_TYPE, Э���е�recordType��
    UINT4		bill_sequence; // ������ˮ�ţ�Ψһ����һ�Ż�����
    BYTE		ModuleNum; // ����������ģ���
    UINT4		count; // GSM �Ʒѵ�һ�κ��в������Ż���ʱcount �Ǹôκ����׻�����ˮ��
    BYTE		sequence; // ���Ż���Ϊ0�����Ż���Ϊ1����������
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, ���������м�ĩ�Ż�����ʶ
    BYTE		HotBillingTag:1; //Ϊcmcc�������ӵ��ȼƷѱ�־
    BYTE		CRC_Way:2; // CRCУ�鷽ʽ
    BYTE		ChargeLevel:1;//�ƷѾ���ָʾ:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//�����򲿷ֵ���չ�ռ�
    BYTE		Comm_reserved;//�����򲿷ֵ���չ�ռ�
	
	//  ��չ��:
	
	BYTE		servedIMSI[MAX_IMSI_LEN];  //  ���Ʒ��ƶ��û�IMSI��

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  ���Ʒ��ƶ��û��ƶ�̨�豸ʶ����

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;  
    BYTE		servedMSISDN_nai :  3;
    BYTE		s1            : 1;
    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // ���Ʒ��ƶ��û�MSISDN��
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
        
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callingNumber
    BYTE		callingNumber_npi  :4;                 // ���к���ƻ�
    BYTE		callingNumber_nai  :3;                 // ���к�������    
    BYTE		s2               :1;

    BYTE		callingNumber_BCDlen ;                 // ���к��볤��
    BYTE		callingNumber[MAX_NUM_LEN/2];     // ���к���

    //(MOCallRecord)(TermCAMELRecord)calledNumber
    BYTE		calledNumber_npi  :4;//dialed_isdn_npi  :4;                 // ���к���ƻ�
    BYTE		calledNumber_nai  :3;//dialed_isdn_nai  :3;                 // ���к�������
    BYTE		s3               :1;
   
    BYTE		calledNumber_BCDlen ;//dialed_isdn_num  :5;                 // ���к��볤��
    BYTE		calledNumber[MAX_NUM_LEN/2];  //dialed_num[MAX_NUM_LEN/2];     // ���к���

    //(MOCallRecord)(MTCallRecord)connectedNumber
    BYTE		connectedNumber_npi :4;               //����������ƻ�
    BYTE		connectedNumber_nai :3;               //��������������   
    BYTE		s4                 :1;               //unuseful    

    BYTE		connectedNumber_BCDlen;               //���������볤��
    BYTE		connectedNumber[MAX_NUM_LEN/2];  // ����������

    //(MOCallRecord)(MTCallRecord)(roamingRecord)roamingNumber
    BYTE		roamingNumber_npi : 4;
    BYTE		roamingNumber_nai :  3;    
    BYTE		s5            : 1;    
    BYTE		roamingNumber_BCDlen ;
    BYTE		roamingNumber[MAX_NUM_LEN/2] ;    //MSRN               //  �ƶ��������κ���

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)originalCalledNumber Ŀǰû����д��Ԥ��
    BYTE		originalCalledNumber_npi  :4;                 // ����ƻ�
    BYTE		originalCalledNumber_nai  :3;                 // ��������
    BYTE		s6               :1;    
    BYTE		originalCalledNumber_BCDlen;                 // ���볤��
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

    //(MTCallRecord)redirectingnumber Ŀǰû����д��Ԥ��
    BYTE		redirectingnumber_npi  :4;                 // ����ƻ�
    BYTE		redirectingnumber_nai  :3;                 // ��������   
    BYTE		s7               :1;    
    BYTE		redirectingnumber_BCDlen;                 // ���볤��
    BYTE		redirectingnumber[MAX_NUM_LEN/2]; 

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;  
    BYTE		s8            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  ����MSC��

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mscIncomingROUTE/mscOutgoingROUTE
    BYTE		mscIncomingROUTE_ind : 4;//IT_trunk_group_ind : 4;       
    BYTE		mscOutgoingROUTE_ind : 4;//OT_trunk_group_ind : 4; 
    UINT2		mscIncomingROUTE_ROUTENumber;//IT_trunk_group;                     // ���м�Ⱥ
    UINT2		mscOutgoingROUTE_ROUTENumber;//OT_trunk_group;                    // ���м�Ⱥ
    BYTE		mscIncomingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//IT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];
    BYTE		mscOutgoingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//OT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];

    //(MTCallRecord)location/changeOfLocation 
    BYTE		caller_location_LAI[MAX_LOCAL_LAI_LEN/2];//caller_org_LAC[MAX_LOCAL_LAC_LEN/2];  //  ���Ʒ��ƶ��û�������ʼλ������
    BYTE		caller_location_cellID[MAX_LOCAL_CI_LEN/2];//caller_org_cell_ID[MAX_LOCAL_CI_LEN/2];   // ���Ʒ��ƶ��û������ĳ�ʼС����
    BYTE		caller_changeOflocation_LAI[MAX_LOCAL_LAI_LEN/2]; //caller_LAC[MAX_LOCAL_LAC_LEN/2];     // ���Ʒ��ƶ��û�����λ������
    BYTE		caller_changeOflocation_cellID[MAX_LOCAL_CI_LEN/2];//caller_cell_ID[MAX_LOCAL_CI_LEN/2];  // ���Ʒ��ƶ��û�������С����

    BYTE		called_location_LAI[MAX_LOCAL_LAI_LEN/2];//called_org_LAC[MAX_LOCAL_LAC_LEN/2];    // �����û�������ʼλ������
    BYTE		called_location_cellID[MAX_LOCAL_CI_LEN/2]; //called_org_cell_ID[MAX_LOCAL_CI_LEN/2];    // �����û������ĳ�ʼС����
    BYTE		called_changeOflocation_LAI[MAX_LOCAL_LAI_LEN/2];//called_LAC[MAX_LOCAL_LAC_LEN/2];          // �����û�����λ������
    BYTE		called_changeOflocation_cellID[MAX_LOCAL_CI_LEN/2]; //called_cell_ID[MAX_LOCAL_CI_LEN/2];       // �����û�������С����

    //(MOCallRecord)(MTCallRecord)(roamingRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  �����ô�ҵ��Ϊ�绰ҵ��Ϊ��0���������ҵ��Ϊ��1����
    BYTE		ROAM_mark        :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  ����TBS��绰ҵ������ҵ���롣
    
    //(MOCallRecord)(MTCallRecord)(roamingRecord)transparencyIndicator
    BYTE		transparencyIndicator:2;  //transparency_indicator :2;  // ͸����͸��ָʾ    
    BYTE		s10          :6;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)changeOfService ���¼��������м仰���ķ�ʽ��

    //(MOCallRecord)(MTCallRecord)(roamingRecord)supplServicesUsed(�����޹صĲ���ҵ�����¼�������ʽ��,BAU�ϲ�)
    struct MBILL_SS_code1     SS_code1;          
    struct MBILL_SS_code2     SS_code2;          
    struct MBILL_SS_code3     SS_code3;          
    struct MBILL_SS_code4     SS_code4;          
    struct MBILL_SS_code5     SS_code5;

    //(MOCallRecord)(MTCallRecord)aocParameters/changeOfAOCParms
    AOC_E1_E7    aocParameters;  

    //(MOCallRecord)(MTCallRecord)MSClassmark[3]
    BYTE  MSClassmark[3];// ��ԭ����struct  Classmark initial_served_ms_classmark�����Ʒ��ƶ��û���ʼCLASSMARK��struct  Classmark������Э���ΪBYTE

    //(MOCallRecord)(MTCallRecord)changeOfClassmark[3]  
    BYTE   changeOfClassmark[3];//struct  Classmark last_served_ms_classmark;  //���Ʒ��ƶ��û��ı���CLASSMARK

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)seizureTime 
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)answerTime
    BYTE		initial_year;           // ͨ����ʼʱ��
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            
    BYTE		initial_minute;
    BYTE		initial_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)releaseTime
    BYTE		end_year; // ͨ������ʱ��
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callDuration
    UINT4		callDuration;//conversation_time;                 // ͨ��ʱ��
        
    //(MOCallRecord)(MTCallRecord)radioChanUsed 
    //(MOCallRecord)(MTCallRecord)radioChanRequested  
    BYTE		radioChanRequested:4;//USER_Radio_channelUINT2ed :4;                // 1  ȫ���ʻ������    

    BYTE		radioChanUsed:2; 

    BYTE		s11    :2;    

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)causeForTerm
    BYTE		causeForTerm;//cause_for_termination;        //  ͨ����ֹԭ��

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)SDiagnostics
    SDiagnostics     diagnostics;//uDiagnostics;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; ����ͬһ�ƶ�̨�����Ĳ�ͬ����ı���ʶ���

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)sequenceNumber: �м仰�����к�,���ڹ����򲿷�, ������:sequence��

    //(MOCallRecord)(MTCallRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // �Ʒ���ѱ�־    
    BYTE		s12         :4;

    //(MOCallRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ����ָʾ
    UINT2		money_per_count; // ����
    UINT2		add_fee;                 //���ӷ�

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)chargedParty Ŀǰû����д����Ԥ������λ
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)networkCallReference
    //��ʹ��CAMELҵ��ʱ,ͬһ���������ĸ���Ԫ�����Ļ��������д˱�ʶ��(NCR),���԰���Щ����������һ��. 
    BYTE		NetworkCallReferenceLen;
    BYTE		NetworkcallReferenceNumber[maxCallReferenceNumberLength]; // ��NetworkCallReferenceLenָ������(2G���еĺ��вο���)

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mSCAddress (��NetworkCallReferenceLen���ʹ��)
    BYTE		MSCAddress_npi : 4;
    BYTE		MSCAddress_nai :  3;
    BYTE		s13            : 1;    
    BYTE		MSCAddress_BCDlen;
    BYTE		MSCAddress[MAX_MSC_NUM_IN_BYTES];

    //(MOCallRecord)(MTCallRecord)fnur
    BYTE		fnur;//ucFunr;           //����ҵ���û�����,enum FNUR

    //(MOCallRecord)(MTCallRecord)aiurRequested
    BYTE		aiurRequested;//BYTE SMSResult;(B03������ʹ�õ���SMSResult��������aiurRequested��R02����)

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
    UINT4		ServiceKey;                //ҵ���

    //(MOCallRecord)(MTCallRecord)rateIndication
    BYTE		rateIndication;//ucRateIndication; //����ҵ����������

     //(MOCallRecord)(MTCallRecord)systemType
    BYTE		systemType:2;//USER_NETWORK_TYPE :4; 

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)freeFormatDataAppend
    BYTE		freeFormatDataAppend:2;//CAMEL_FCI_Data_Append_Ind :2; //��ʾ�ǵ�һ��FCI��Ϣ��0��ʾ���ǵ�һ��

      //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)defaultCallHandling
    BYTE		defaultCallHandling:2;//CAMEL_default_Call_Handling;   
    BYTE		s15       :2;   
   

    //(MOCallRecord)(MTCallRecord)callingChargeAreaCode Ŀǰû����д��Ԥ��//̩��������    BYTE    CAC_num[MAX_CAC_LEN/2+1];
    BYTE		callingChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];

    //(MOCallRecord)(MTCallRecord)calledChargeAreaCode Ŀǰû����д��Ԥ��//̩��������    BYTE    CAC_num[MAX_CAC_LEN/2+1];
    BYTE		calledChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)numberOfDPEncountered
    BYTE		numberOfDPEncountered;//CAMEL_num_of_DP;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)levelOfCAMELService
    BYTE		levelOfCAMELService;//CAMEL_service_level;

    //(MTCallRecord)(roamingRecord)mscIncomingCircuit
    UINT2		mscOutgoingCircuit;
    UINT2		mscIncomingCircuit;

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId����ָʾ
    BYTE		msc_id_len:3;//orgMSCId����ָʾ
    BYTE		s23            : 2;

    //(MOCallRecord)(MTCallRecord)orgRNCorBSCId
    BYTE		orgRNCorBSCId[MAX_RNCBSC_ID_LEN];//Rnc_Bsc_Id[3];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(MOCallRecord)(MTCallRecord)callEmlppPriority
    BYTE		callEmlppPriority:4;//CallEmlpp;//��������EMLPP���ȼ� ��ȡֵ0-7)

    //(MTCallRecord)calledDefaultEmlppPriority
    BYTE		calledDefaultEmlppPriority:4;//calledDefaultEmlpp;//�����û�ǩԼĬ��EMLPP���ȼ���ȡֵ0-7)

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

    //(MOCallRecord)(MTCallRecord)hotBillingTag���ڻ�����HotBillingTag

    //(MTCallRecord)redirectingcounter 
    BYTE		CFW_invoked_times:4;       // ǰת�����Ĵ���    

    //(MOCallRecord)(MTCallRecord)recordExtensions
    BYTE		CH_invoked_times:4;        // ���б���ҵ�����Ĵ���
    BYTE		CH_retrieve_times:4;       // ���б���ҵ��ָ��Ĵ���    
    BYTE		CW_invoked_times:4;        // ���еȴ�ҵ�����Ĵ���
    BYTE		MPTY_Build_times:4;        // �෽ͨ��ҵ���������Ĵ���    
    BYTE		MPTY_CH_times:4;           // �෽ͨ��ҵ�񱣳ַ����Ĵ���
    BYTE		MPTY_CH_retrieve_times:4;   // �෽ͨ��ҵ�񱣳ָֻ������Ĵ���
    BYTE		MPTY_Split_times:4;        // �෽ͨ��ҵ����뷢���Ĵ���

    BYTE		HO_invoked_times;         // �л������Ĵ���
    
    //BYTE     ServiceChangedTimes:4;

    BYTE		AOCParmsChangedTimes:4;    
    BYTE		ClassmarkChangedTimes:4;    
    BYTE		RadioChanChangedTimes:4;
    BYTE		transmission_mode:4;   // 2G����ʹ�õ�  ȫ���ʻ������ ��־��

    BYTE		Info_trans_capability;             // ��������

    struct  BillClassmark3 Classmark3Information;    

    BYTE		DTMF_indicator :2;  // �Ƿ�ʹ��DTMF
    BYTE		user_type :5;  //   �ƶ�̨���ȼ�    
    BYTE		s9 :1;  //   0 ��ʾ�����û���1 ��ʾ�����û���
    BYTE		initialCallAttemptFlag :1;	
    BYTE		reserved :1; 
    BYTE		SORIndication:1;    
    BYTE		UssdCallBackIndication:1;// ֻ��CAMEL MTC�����вż�¼
    BYTE		ucTcsiVtcsiFlag : 4;//0ΪTCSI��1ΪVTCSI,0xff��Ч 
	
    //Ϊ�Զ�������Autobauding Support����
    BYTE		ModemType:7;    
    BYTE		Classmark3Indication:1;
    
    BYTE		cause_for_partial_record;//cause_for_partial_record ;      //  ���м仰��ԭ��    
    BYTE		transaction_id;// ����ID,����ͬһ�ƶ�̨�����Ĳ�ͬ����ı���ʶ���
    BYTE		gsm_gsvn;                          // 1ҵ�����
    BYTE		B_ch_number;       // �ôκ�����ռ�õ�B�ŵ���

    // GSM ������
    BYTE		scfID[MAX_SCFID_LEN];      //SCF������

    //  ��ǰ���Ʒ��ƶ��û�����MSC
    BYTE		served_msc_num_npi : 4;
    BYTE		served_msc_num_nai :  3;
    BYTE		s18            : 1;
    BYTE		served_msc_num_BCDlen ;
    BYTE		served_msc_num[MAX_MSC_NUM_IN_BYTES];       

    //����ҵ������ʶ���
    BYTE     	GuaranteedBitRate;//��֤������
    BYTE    	MaximumBitRate;//��������
    
    //������������������չ
    BYTE		container[MAX_CONTAINER_MTC_LEN];

    // FCI ���ڿɱ䲿��
    BYTE		fcidata_len;
    BYTE        VarPart[MAX_TRANSFER_FCI_LEN];
    BYTE		FCIData[MAX_TRANSFER_FCI_LEN];
	
};

//1.3	TCAMEL_Bill_Report������TCAMEL ��TermCAMELRecord����
struct TCAMEL_Bill_Report
{
	// �����򲿷�, ������:
	BYTE		validflag; // ��Ч�Ա�־,����AA��Ϊ��Ч����55��Ϊ��Ч
    UINT2		size; // ���Ż�������
    UINT2		CRCCode; // CRCУ����
    BYTE		bill_type; //  enum   GSM_BILL_TYPE, Э���е�recordType��
    UINT4		bill_sequence; // ������ˮ�ţ�Ψһ����һ�Ż�����
    BYTE		ModuleNum; // ����������ģ���
    UINT4		count; // GSM �Ʒѵ�һ�κ��в������Ż���ʱcount �Ǹôκ����׻�����ˮ��
    BYTE		sequence; // ���Ż���Ϊ0�����Ż���Ϊ1����������
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, ���������м�ĩ�Ż�����ʶ
    BYTE		HotBillingTag:1; //Ϊcmcc�������ӵ��ȼƷѱ�־
    BYTE		CRC_Way:2; // CRCУ�鷽ʽ
    BYTE		ChargeLevel:1;//�ƷѾ���ָʾ:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//�����򲿷ֵ���չ�ռ�
    BYTE		Comm_reserved;//�����򲿷ֵ���չ�ռ�
	
	//  ��չ��:

	BYTE		servedIMSI[MAX_IMSI_LEN];  //  ���Ʒ��ƶ��û�IMSI��

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  ���Ʒ��ƶ��û��ƶ�̨�豸ʶ����

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // ���Ʒ��ƶ��û�MSISDN��
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callingNumber
    BYTE		callingNumber_npi  :4;                 // ���к���ƻ�
    BYTE		callingNumber_nai  :3;                 // ���к�������   
    BYTE		s2               :1;    
    BYTE		callingNumber_BCDlen ;                 // ���к��볤��
    BYTE		callingNumber[MAX_NUM_LEN/2];     // ���к���

    //(MOCallRecord)(TermCAMELRecord)calledNumber
    BYTE		calledNumber_npi  :4;//dialed_isdn_npi  :4;                 // ���к���ƻ�
    BYTE		calledNumber_nai  :3;//dialed_isdn_nai  :3;                 // ���к�������    
    BYTE		s3               :1;    
    BYTE		calledNumber_BCDlen ;//dialed_isdn_num  :5;                 // ���к��볤��
    BYTE		calledNumber[MAX_NUM_LEN/2];  //dialed_num[MAX_NUM_LEN/2];     // ���к���

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;   
    BYTE		s4            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  ����MSC��

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
    UINT4		ServiceKey;                //ҵ���

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)networkCallReference
    //��ʹ��CAMELҵ��ʱ,ͬһ���������ĸ���Ԫ�����Ļ��������д˱�ʶ��(NCR),���԰���Щ����������һ��. 
    BYTE		NetworkCallReferenceLen;
    BYTE		NetworkcallReferenceNumber[maxCallReferenceNumberLength]; // ��NetworkCallReferenceLenָ������(2G���еĺ��вο���)

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mSCAddress (��NetworkCallReferenceLen���ʹ��)
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
    UINT2		mscIncomingROUTE_ROUTENumber;//IT_trunk_group;                     // ���м�Ⱥ
    UINT2		mscOutgoingROUTE_ROUTENumber;//OT_trunk_group;                    // ���м�Ⱥ
    BYTE		mscIncomingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//IT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];
    BYTE		mscOutgoingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//OT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)seizureTime 
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)answerTime
    BYTE		initial_year;           // ͨ����ʼʱ��
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            //3
    BYTE		initial_minute;
    BYTE		initial_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)releaseTime
    BYTE		end_year; // ͨ������ʱ��
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callDuration
    UINT4		callDuration;//conversation_time;                 // ͨ��ʱ��

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)causeForTerm
    BYTE		causeForTerm;//cause_for_termination;        //  ͨ����ֹԭ��

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)SDiagnostics
    SDiagnostics     diagnostics;//uDiagnostics;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; ����ͬһ�ƶ�̨�����Ĳ�ͬ����ı���ʶ���

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)numberOfDPEncountered
    BYTE		numberOfDPEncountered;//CAMEL_num_of_DP;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)levelOfCAMELService
    BYTE		levelOfCAMELService;//CAMEL_service_level;


    MBILL_CHANGEFLAGS     CAMELModificationChangeFlags;

	BYTE		ModifiedCallingPartyNumber_npi  :4;                 // ����ƻ�
    BYTE		ModifiedCallingPartyNumber_nai  :3;                 // ��������   
    BYTE		s9               :1;    
    BYTE		ModifiedCallingPartyNumber_BCDlen;                 // ���볤��
    BYTE		ModifiedCallingPartyNumber[MAX_NUM_LEN/2];     // �ı������к���

    BYTE		ModifiedCallingPartyCategory;// �޸ĺ�������û����

    BYTE		ModifiedOriginalCalledPartyNumber_npi  :4;                 // ����ƻ�
    BYTE		ModifiedOriginalCalledPartyNumber_nai  :3;                 // ��������   
    BYTE		s10               :1;       
    BYTE		ModifiedOriginalCalledPartyNumber_BCDlen;                 // ���볤��
    BYTE		ModifiedOriginalCalledPartyNumber[MAX_NUM_LEN/2]; // �ı���ԭ���к���

    BYTE		ModifiedGenericNumber_npi  :4;                 // ����ƻ�
    BYTE		ModifiedGenericNumber_nai  :3;                 // �������� 
    BYTE		s11               :1;    
    BYTE		ModifiedGenericNumber_BCDlen;                 // ���볤��
    BYTE		ModifiedGenericNumber[MAX_NUM_LEN/2];   // �ı���GenericNumber,�μ�CapEncode_TGenericNumbers , TGenericNumbers

    BYTE		ModifiedRedirectingPartyNumber_npi  :4;                 // ����ƻ�
    BYTE		ModifiedRedirectingPartyNumber_nai  :3;                 // ��������
    BYTE		s12               :1;    
    BYTE		ModifiedRedirectingPartyNumber_BCDlen;                 // ���볤��
    BYTE		ModifiedRedirectingPartyNumber[MAX_NUM_LEN/2];     // �ı���RedirectingPartyNumber
    
    BYTE		ModifiedRedirectionCounter; 

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)freeFormatDataAppend
    BYTE		freeFormatDataAppend:2;//CAMEL_FCI_Data_Append_Ind :2; //��ʾ�ǵ�һ��FCI��Ϣ��0��ʾ���ǵ�һ��  

    //(TermCAMELRecord)mscServerIndication//Ŀǰû����д��Ԥ������λ
    BYTE		mscServerIndication:1;    

    //(MOCallRecord)(TermCAMELRecord)defaultCallHandling-2 
    BYTE		defaultCallHandling2:2 ;//CAMEL_default_Call_Handling2;   
    BYTE		s13       :3;    
      
    //(MOCallRecord)(TermCAMELRecord)gsm-SCFAddress-2���ڿɱ䲿��
    BYTE		CAMEL_GsmSCF_Address2_npi : 4;
    BYTE		CAMEL_GsmSCF_Address2_nai :  3; 
    BYTE		s14            : 1;   
    BYTE		CAMEL_GsmSCF_Address2_BCDlen;
    BYTE		CAMEL_GsmSCF_Address2[MAX_SCF_LEN]; 

    //(MOCallRecord)(TermCAMELRecord)serviceKey-2���ڿɱ䲿��
    UINT4		ServiceKey2;                //ҵ���

    //(MOCallRecord)(TermCAMELRecord)freeFormatData-2������󳤶ȿɱ�

    //(MOCallRecord)(TermCAMELRecord)freeFormatDataAppend-2
    BYTE		freeFormatDataAppend2:2;//CAMEL_FCI_Data_Append_Ind2 :2;    
    BYTE		s15         :6;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)(TermCAMELRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  �����ô�ҵ��Ϊ�绰ҵ��Ϊ��0���������ҵ��Ϊ��1����   
    BYTE		ROAM_mark       :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  ����TBS��绰ҵ������ҵ���롣
    
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // �Ʒ���ѱ�־   
	BYTE		s17             :4;
    
    //(MOCallRecord)(TermCAMELRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ����ָʾ
    UINT2		money_per_count; // ����
    UINT2		add_fee;                 //���ӷ�

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)chargedParty Ŀǰû����д����Ԥ������λ
    BYTE        chargedParty; //enum MBILL_CHARGED_PARTY
    
    BYTE        UssdCallBackIndication:1;// ucb���е���ʵ���вż�¼����
    BYTE        msc_id_len:3;//orgMSCId����ָʾ
    BYTE        initialCallAttemptFlag:1;//ICA���б�־
    BYTE        s18         :3;
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)originalCalledNumber Ŀǰû����д��Ԥ��
    BYTE		originalCalledNumber_npi  :4;                 // ����ƻ�    
    BYTE		originalCalledNumber_nai  :3;                 // ��������
    BYTE		s19               :1;    
    BYTE		originalCalledNumber_BCDlen;                 // ���볤��
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;

    BYTE		cause_for_partial_record;//cause_for_partial_record ;      //  ���м仰��ԭ��

    //������������������չ
    BYTE		container[MAX_CONTAINER_TCAMEL_LEN];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)freeFormatData FCI���ڿɱ䲿��
    //(MOCallRecord)(TermCAMELRecord)freeFormatData-2 FCI���ڿɱ䲿��
    BYTE		fcidata_len;
    BYTE		fcidata2_len;
    BYTE     VarPart[MAX_TRANSFER_FCI_LEN * 2];
    
	// BYTE		FCIData[MAX_TRANSFER_FCI_LEN];
    // BYTE		FCIData2[MAX_TRANSFER_FCI_LEN];

	
};

//1.4	ROAM_Bill_Report������ROAM��roamingRecord����
struct ROAM_Bill_Report
{
	// �����򲿷�, ������:

    BYTE		validflag; // ��Ч�Ա�־,����AA��Ϊ��Ч����55��Ϊ��Ч
    UINT2		size; // ���Ż�������
    UINT2		CRCCode; // CRCУ����
    BYTE		bill_type; //  enum   GSM_BILL_TYPE, Э���е�recordType��
    UINT4		bill_sequence; // ������ˮ�ţ�Ψһ����һ�Ż�����
    BYTE		ModuleNum; // ����������ģ���
    UINT4		count; // GSM �Ʒѵ�һ�κ��в������Ż���ʱcount �Ǹôκ����׻�����ˮ��
    BYTE		sequence; // ���Ż���Ϊ0�����Ż���Ϊ1����������
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, ���������м�ĩ�Ż�����ʶ
    BYTE		HotBillingTag:1; //Ϊcmcc�������ӵ��ȼƷѱ�־
    BYTE		CRC_Way:2; // CRCУ�鷽ʽ
    BYTE		ChargeLevel:1;//�ƷѾ���ָʾ:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//�����򲿷ֵ���չ�ռ�
    BYTE		Comm_reserved;//�����򲿷ֵ���չ�ռ�
	
	//  ��չ��:
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  ���Ʒ��ƶ��û�IMSI��

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // ���Ʒ��ƶ��û�MSISDN��
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callingNumber
    BYTE		callingNumber_npi  :4;                 // ���к���ƻ�
    BYTE		callingNumber_nai  :3;                 // ���к�������
    BYTE		s2               :1;    
    BYTE		callingNumber_BCDlen ;                 // ���к��볤��
    BYTE		callingNumber[MAX_NUM_LEN/2];     // ���к���

    //(MOCallRecord)(MTCallRecord)(roamingRecord)roamingNumber
    BYTE		roamingNumber_npi : 4;
    BYTE		roamingNumber_nai :  3;    
    BYTE		s3            : 1;    
    BYTE		roamingNumber_BCDlen ;
    BYTE		roamingNumber[MAX_NUM_LEN/2] ;    //MSRN               //  �ƶ��������κ���

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;    
    BYTE		s4            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  ����MSC��

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mscIncomingROUTE/mscOutgoingROUTE
    BYTE		mscIncomingROUTE_ind : 4;//IT_trunk_group_ind : 4;    
    BYTE		mscOutgoingROUTE_ind : 4;//OT_trunk_group_ind : 4;    
    UINT2		mscIncomingROUTE_ROUTENumber;//IT_trunk_group;                     // ���м�Ⱥ
    UINT2		mscOutgoingROUTE_ROUTENumber;//OT_trunk_group;                    // ���м�Ⱥ
    BYTE		mscIncomingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//IT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];
    BYTE		mscOutgoingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//OT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];

    //(MOCallRecord)(MTCallRecord)(roamingRecord)(TermCAMELRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  �����ô�ҵ��Ϊ�绰ҵ��Ϊ��0���������ҵ��Ϊ��1����    
    BYTE		ROAM_mark          :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  ����TBS��绰ҵ������ҵ���롣

    BYTE		transparencyIndicator:2;  //transparency_indicator :2;  // ͸����͸��ָʾ   
    
    //(MOCallRecord)(MTCallRecord)(roamingRecord)transparencyIndicator
	BYTE        SORIndication	:1;//��ѡ·�ɱ�־ 
    BYTE		s6          :5;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)changeOfService ���¼��������м仰���ķ�ʽ��

    //(MOCallRecord)(MTCallRecord)(roamingRecord)supplServicesUsed(�����޹صĲ���ҵ�����¼�������ʽ��,BAU�ϲ�)
    struct MBILL_SS_code1     SS_code1;          
    struct MBILL_SS_code2     SS_code2;          
    struct MBILL_SS_code3     SS_code3;          
    struct MBILL_SS_code4     SS_code4;          
    struct MBILL_SS_code5     SS_code5;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)seizureTime 
    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)answerTime
    BYTE		initial_year;           // ͨ����ʼʱ��
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            //3
    BYTE		initial_minute;
    BYTE		initial_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)releaseTime
    BYTE		end_year; // ͨ������ʱ��
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callDuration
    UINT4		callDuration;//conversation_time;                 // ͨ��ʱ��

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)causeForTerm
    BYTE		causeForTerm;//cause_for_termination;        //  ͨ����ֹԭ��

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)SDiagnostics
    SDiagnostics     diagnostics;//uDiagnostics;

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; ����ͬһ�ƶ�̨�����Ĳ�ͬ����ı���ʶ���

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)sequenceNumber: �м仰�����к�,���ڹ����򲿷�, ������:sequence��

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)networkCallReference
    //��ʹ��CAMELҵ��ʱ,ͬһ���������ĸ���Ԫ�����Ļ��������д˱�ʶ��(NCR),���԰���Щ����������һ��. 
    BYTE		NetworkCallReferenceLen;
    BYTE		NetworkcallReferenceNumber[maxCallReferenceNumberLength]; // ��NetworkCallReferenceLenָ������(2G���еĺ��вο���)

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)mSCAddress (��NetworkCallReferenceLen���ʹ��)
    BYTE		MSCAddress_npi : 4;
    BYTE		MSCAddress_nai :  3;
    BYTE		s7            : 1;   
    BYTE		MSCAddress_BCDlen;
    BYTE		MSCAddress[MAX_MSC_NUM_IN_BYTES];

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // �Ʒ���ѱ�־
    BYTE		callEmlppPriority   :4;
    
    //(MOCallRecord)(TermCAMELRecord)(roamingRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ����ָʾ
    UINT2		money_per_count; // ����
    UINT2		add_fee;                 //���ӷ�

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)chargedParty Ŀǰû����д����Ԥ������λ
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    //(MOCallRecord)(MTCallRecord)(TermCAMELRecord)(roamingRecord)originalCalledNumber Ŀǰû����д��Ԥ��
    BYTE		originalCalledNumber_npi  :4;                 // ����ƻ�
    BYTE		originalCalledNumber_nai  :3;                 // ��������    
    BYTE		s9               :1;    
    BYTE		originalCalledNumber_BCDlen;                 // ���볤��
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

    //(MOCallRecord)(MTCallRecord)(roamingRecord)callingChargeAreaCode Ŀǰû����д��Ԥ��//̩��������    BYTE    CAC_num[MAX_CAC_LEN/2+1];
    BYTE		callingChargeAreaCode[MAX_CHARGE_AREA_CODE_LEN];

    //(MOCallRecord)(MTCallRecord)(roamingRecord)calledChargeAreaCode Ŀǰû����д��Ԥ��//̩��������    BYTE    CAC_num[MAX_CAC_LEN/2+1];
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
    BYTE		msc_id_len:3;//orgMSCId����ָʾ
    BYTE		s10          :1;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)cUGInterlockCode
    UINT4		CUGInterlockCode;//CUGInterlockCode;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)cUGIndex
    UINT2		CUGIndex;

    BYTE		cause_for_partial_record;//cause_for_partial_record ;      //  ���м仰��ԭ��

    BYTE        eaSubscriberInfo[MAX_NAEA_CODE_IN_BYTE + 1];
    BYTE        selectedCIC[MAX_NAEA_CODE_IN_BYTE + 1];
    //������������������չ
    BYTE     container[MAX_CONTAINER_ROAM_LEN];
	
};

//1.5	Gateway_Bill_Report������GWO(outGatewayRecord)��GWI(incGatewayRecord)��Transit(TransitCallRecord)��
struct Gateway_Bill_Report
{
	// �����򲿷�, ������:
	
    BYTE		validflag; // ��Ч�Ա�־,����AA��Ϊ��Ч����55��Ϊ��Ч
    UINT2		size; // ���Ż�������
    UINT2		CRCCode; // CRCУ����
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // ������ˮ�ţ�Ψһ����һ�Ż�����
    BYTE		ModuleNum; // ����������ģ���
    UINT4		count; // GSM �Ʒѵ�һ�κ��в������Ż���ʱcount �Ǹôκ����׻�����ˮ��
    BYTE		sequence; // ���Ż���Ϊ0�����Ż���Ϊ1����������
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, ���������м�ĩ�Ż�����ʶ
    BYTE		HotBillingTag:1; //Ϊcmcc�������ӵ��ȼƷѱ�־
    BYTE		CRC_Way:2; // CRCУ�鷽ʽ
    BYTE		ChargeLevel:1;//�ƷѾ���ָʾ:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//�����򲿷ֵ���չ�ռ�
    BYTE		Comm_reserved;//�����򲿷ֵ���չ�ռ�	
	
	//  ��չ��:
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  ���Ʒ��ƶ��û�IMSI��

    //servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  ���Ʒ��ƶ��û��ƶ�̨�豸ʶ����

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)callingNumber
    BYTE		callingNumber_npi  :4;                 // ���к���ƻ�
    BYTE		callingNumber_nai  :3;                 // ���к������� 
    BYTE		s1               :1;    
    BYTE		callingNumber_BCDlen ;                 // ���к��볤��
    BYTE		callingNumber[MAX_NUM_LEN/2];     // ���к���

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)calledNumber
    BYTE		calledNumber_npi  :4;//dialed_isdn_npi  :4;                 // ���к���ƻ�
    BYTE		calledNumber_nai  :3;//dialed_isdn_nai  :3;                 // ���к������� 
    BYTE		s2               :1;  
    BYTE		calledNumber_BCDlen ;//dialed_isdn_num  :5;                 // ���к��볤��
    BYTE		calledNumber[MAX_NUM_LEN/2];  //dialed_num[MAX_NUM_LEN/2];     // ���к���
    
    //(incGatewayRecord)(OutGatewayRecord)roamingNumber
    BYTE		roamingNumber_npi : 4;
    BYTE		roamingNumber_nai :  3;   
    BYTE		s3            : 1;   
    BYTE		roamingNumber_BCDlen ;
    BYTE		roamingNumber[MAX_NUM_LEN/2] ;    //MSRN               //  �ƶ��������κ���

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;    
    BYTE		s4            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  ����MSC��

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)mscIncomingROUTE/mscOutgoingROUTE
    BYTE		mscIncomingROUTE_ind : 4;//IT_trunk_group_ind : 4;    
    BYTE		mscOutgoingROUTE_ind : 4;//OT_trunk_group_ind : 4;    
    UINT2		mscIncomingROUTE_ROUTENumber;//IT_trunk_group;                     // ���м�Ⱥ
    UINT2		mscOutgoingROUTE_ROUTENumber;//OT_trunk_group;                    // ���м�Ⱥ
    BYTE		mscIncomingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//IT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];
    BYTE		mscOutgoingROUTE_ROUTEName[MAX_TRUNK_GROUP_NAME_LENGHT];//OT_trunk_group_name[MAX_TRUNK_GROUP_NAME_LENGHT];

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)seizureTime 
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)answerTime
    BYTE		initial_year; // ͨ����ʼʱ��
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            //3
    BYTE		initial_minute;
    BYTE		initial_second;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)releaseTime
    BYTE		end_year; // ͨ������ʱ��
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)callDuration
    UINT4		callDuration;//conversation_time;                 // ͨ��ʱ��

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)causeForTerm
    BYTE		causeForTerm;//cause_for_termination;        //  ͨ����ֹԭ��

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)SDiagnostics
    SDiagnostics     diagnostics;//uDiagnostics;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; ����ͬһ�ƶ�̨�����Ĳ�ͬ����ı���ʶ���

    //(MOCallRecord)(MTCallRecord)(roamingRecord)(TermCAMELRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  �����ô�ҵ��Ϊ�绰ҵ��Ϊ��0���������ҵ��Ϊ��1����   
    BYTE		s5       :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  ����TBS��绰ҵ������ҵ���롣
    
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)sequenceNumber: �м仰�����к�,���ڹ����򲿷�, ������:sequence��

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // �Ʒ���ѱ�־   
    BYTE		s6          :4;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ����ָʾ
    UINT2		money_per_count; // ����
    UINT2		add_fee;                 //���ӷ�

    //(incGatewayRecord)(OutGatewayRecord)chargedParty Ŀǰû����д����Ԥ������λ
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)originalCalledNumber Ŀǰû����д��Ԥ��
    BYTE		originalCalledNumber_npi  :4;                 // ����ƻ�
    BYTE		originalCalledNumber_nai  :3;                 // ��������  
    BYTE		s7               :1;   
    BYTE		originalCalledNumber_BCDlen;                 // ���볤��
    BYTE		originalCalledNumber[MAX_NUM_LEN/2]; 

    //(OutGatewayRecord)(TransitCallRecord)mscOutgoingCircuit
    UINT2		mscOutgoingCircuit;

    //(incGatewayRecord)(TransitCallRecord)mscIncomingCircuit
    UINT2		mscIncomingCircuit;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)callEmlppPriority
    BYTE		callEmlppPriority:4;//CallEmlpp;//��������EMLPP���ȼ� ��ȡֵ0-7)
    BYTE		reserved:4;

    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)eaSubscriberInfo Ŀǰû����д��Ԥ��
    BYTE		eaSubscriberInfo[MAX_NAEA_CODE_IN_BYTE + 1];
    
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)selectedCIC Ŀǰû����д��Ԥ��
    BYTE		selectedCIC[MAX_CIC_LEN_IN_BYTE];

     //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)cUGOutgoingAccessIndicator
    BYTE		CUGOutgoingAccessIndicator:2;//CUG_call_ind         :2; 
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)cUGOutgoingAccessUsed
    BYTE		CUGOutgoingAccessUsed:2;//CUGOutgoingAccessUsed   :2;
    BYTE		msc_id_len:3;//orgMSCId����ָʾ
    BYTE		s8        :1;    

   
    //(incGatewayRecord)(OutGatewayRecord)(TransitCallRecord)cUGInterlockCode
    UINT4		CUGInterlockCode;//CUGInterlockCode;

    //(MOCallRecord)(MTCallRecord)(roamingRecord)cUGIndex
    UINT2		CUGIndex;

    //recordExtensions
    BYTE		cause_for_partial_record ;      //  ���м仰��ԭ��
    BYTE		gsm_gsvn;                          // 1ҵ�����
    BYTE		SORIndication:1;
    BYTE		transparencyIndicator:2;   //͸����͸��ָʾ    

    BYTE		PromptAndCollectCharge:1;
    BYTE		transmission_mode:4;  // 2G����ʹ�õ�ȫ���ʰ����ʱ�־

    BYTE		subscriberCategory;

    BYTE        isdn_bc_len;
    BYTE        isdn_bc[MAX_ISDN_BC_LEN];
    BYTE        LLC_len;
    BYTE        LLC[MAX_LLC_LEN];
    BYTE        HLC_len;
    BYTE        HLC[MAX_HLC_LEN];

    
    //������������������չ
    BYTE		container[MAX_CONTAINER_GATEWAY_LEN];
	
};

//1.6	SMSMT_Bill_Report������SMS MT(MTSMSRecord)��
struct SMSMT_Bill_Report
{
	// �����򲿷�, ������:

	BYTE		validflag; // ��Ч�Ա�־,����AA��Ϊ��Ч����55��Ϊ��Ч
    UINT2		size; // ���Ż�������
    UINT2		CRCCode; // CRCУ����
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // ������ˮ�ţ�Ψһ����һ�Ż�����
    BYTE		ModuleNum; // ����������ģ���
    UINT4		count; // GSM �Ʒѵ�һ�κ��в������Ż���ʱcount �Ǹôκ����׻�����ˮ��
    BYTE		sequence; // ���Ż���Ϊ0�����Ż���Ϊ1����������
    BYTE		record_type :3; //enum GSM_RECORD_TY
    BYTE		HotBillingTag:1; //Ϊcmcc�������ӵ��ȼƷѱ�־
    BYTE		CRC_Way:2; // CRCУ�鷽ʽ
    BYTE		ChargeLevel:1;//�ƷѾ���ָʾ:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//�����򲿷ֵ���չ�ռ�
	BYTE		Comm_reserved;//�����򲿷ֵ���չ�ռ�
	
	//  ��չ��:	
    BYTE		serviceCentre_npi : 4;//SMS_address_npi : 4;
    BYTE		serviceCentre_nai :  3;//SMS_address_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		serviceCentre_BCDlen;//SMS_address_num : 5;
    BYTE		serviceCentre[MAX_MSC_NUM_IN_BYTES];  //SMS_address[8];    //����Ϣ���ĵ�ַ
    
    //(MOSMSRecord)(MTSMSRecord)servedIMSI
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  ���Ʒ��ƶ��û�IMSI��

    //(MOSMSRecord)(MTSMSRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  ���Ʒ��ƶ��û��ƶ�̨�豸ʶ����

    //(MOSMSRecord)(MTSMSRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s2            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // ���Ʒ��ƶ��û�MSISDN��
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MOSMSRecord)(MTSMSRecord)MSClassmark[3] 
    BYTE   MSClassmark[3];//struct  Classmark last_served_ms_classmark;      // ���Ʒ��ƶ��û���ǰCLASSMARK
    BillClassmark3 Classmark3Information;
    
    //(MOSMSRecord)(MTSMSRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;    
    BYTE		s3            : 1;   
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  ����MSC��

    //(MOSMSRecord)(MTSMSRecord)location �����еĴ����һ
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
    SDiagnostics  smsResult;//�ο�map�����SMSResult
    BYTE		actionResult;//���������ȡSM_transmission_success,�� SM_transmission_failed 

    //(MOSMSRecord)(MTSMSRecord)systemType
    BYTE		systemType:2;//USER_NETWORK_TYPE :4;     
    BYTE		s4        :6;
    
    //(MOSMSRecord)(MTSMSRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  �����ô�ҵ��Ϊ�绰ҵ��Ϊ��0���������ҵ��Ϊ��1����    
    BYTE		ROAM_mark         :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  ����TBS��绰ҵ������ҵ���롣
    
    //(MOSMSRecord)(MTSMSRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // �Ʒ���ѱ�־
    BYTE		Classmark3Indication:1;
    BYTE		s6           :3;
    
    //(MOSMSRecord)(MTSMSRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ����ָʾ
    UINT2		money_per_count; // ����
    UINT2		add_fee;                 //���ӷ�

    //(MOSMSRecord)(MTSMSRecord)chargedParty  Ŀǰû����д����Ԥ������λ
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId����ָʾ
    BYTE		msc_id_len:3;//orgMSCId����ָʾ
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

    //(MOSMSRecord)(MTSMSRecord)hotBillingTag���ڻ�����HotBillingTag

    //(MTSMSRecord)origination
    BYTE		callingNumber_npi  :4; //caller_isdn_npi                // ���к���ƻ�
    BYTE		callingNumber_nai  :3;  //caller_isdn_nai               // ���к�������
    BYTE		s7               :1;    
    BYTE		callingNumber_BCDlen ; //caller_isdn_num                // ���к��볤��
    BYTE		callingNumber[MAX_NUM_LEN/2]; //caller_num    // ���к���

    //(MOSMSRecord)(MTSMSRecord)recordExtensions
    //  ��ǰ���Ʒ��ƶ��û�����MSC
    BYTE		served_msc_num_npi : 4;
    BYTE		served_msc_num_nai :  3;
    BYTE		s8            : 1;
    BYTE		served_msc_num_BCDlen ;
	BYTE		served_msc_num[MAX_MSC_NUM_IN_BYTES];       

    //������������������չ
    BYTE		container[MAX_CONTAINER_SMSMT_LEN];
    
    //(MOSMSRecord)(MTSMSRecord)smstext 
    BYTE		smstext_len;
    BYTE     	VarPart[MAX_SMSTEXT_LEN];
    // BYTE		smstext[MAX_SMSTEXT_LEN];

	
};

//1.7	SMSMO_Bill_Report������SMS MO(MOSMSRecord)��
struct SMSMO_Bill_Report
{
	// �����򲿷�, ������:

	BYTE		validflag; // ��Ч�Ա�־,����AA��Ϊ��Ч����55��Ϊ��Ч
    UINT2		size; // ���Ż�������
    UINT2		CRCCode; // CRCУ����
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // ������ˮ�ţ�Ψһ����һ�Ż�����
    BYTE		ModuleNum; // ����������ģ���
    UINT4		count; // GSM �Ʒѵ�һ�κ��в������Ż���ʱcount �Ǹôκ����׻�����ˮ��
    BYTE		sequence; // ���Ż���Ϊ0�����Ż���Ϊ1����������
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, ���������м�ĩ�Ż�����ʶ
    BYTE		HotBillingTag:1; //Ϊcmcc�������ӵ��ȼƷѱ�־
    BYTE		CRC_Way:2; // CRCУ�鷽ʽ
    BYTE		ChargeLevel:1;//�ƷѾ���ָʾ:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//�����򲿷ֵ���չ�ռ�
    BYTE		Comm_reserved;//�����򲿷ֵ���չ�ռ�
	
	//  ��չ��:
    BYTE		serviceCentre_npi : 4;//SMS_address_npi : 4;
    BYTE		serviceCentre_nai :  3;//SMS_address_nai :  3;   
	BYTE		s1            : 1;    
    BYTE		serviceCentre_BCDlen;//SMS_address_num : 5;
    BYTE		serviceCentre[MAX_MSC_NUM_IN_BYTES];  //SMS_address[8];    //����Ϣ���ĵ�ַ
    
    //(MOSMSRecord)(MTSMSRecord)servedIMSI
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  ���Ʒ��ƶ��û�IMSI��

    //(MOSMSRecord)(MTSMSRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  ���Ʒ��ƶ��û��ƶ�̨�豸ʶ����

    //(MOSMSRecord)(MTSMSRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;   
    BYTE		s2            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // ���Ʒ��ƶ��û�MSISDN��
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MOSMSRecord)(MTSMSRecord)MSClassmark[3] 
    BYTE   MSClassmark[3];//struct  Classmark last_served_ms_classmark;      // ���Ʒ��ƶ��û���ǰCLASSMARK
    struct  BillClassmark3 Classmark3Information;
    
    //(MOSMSRecord)(MTSMSRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//local_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//local_msc_num_nai :  3;    
    BYTE		s3            : 1;    
    BYTE		recordingEntity_BCDlen ;//local_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //local_msc_num[8];          //  ����MSC��

    //(MOSMSRecord)(MTSMSRecord)location �����еĴ����һ
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
    SDiagnostics  smsResult;//�ο�map�����SMSResult
    BYTE		actionResult;//���������ȡSM_transmission_success,�� SM_transmission_failed

    //(MOSMSRecord)destinationNumber
    BYTE		destinationNumber_npi :4;//dialed_isdn_npi  :4;                 // ���к���ƻ�
    BYTE		destinationNumber_nai:3;//dialed_isdn_nai  :3;                 // ���к������� 
    BYTE		s4               :1;    
    BYTE		destinationNumber_BCDlen; //dialed_isdn_num  :5;                 // ���к��볤��
    BYTE		destinationNumber[MAX_NUM_LEN/2];//dialed_num[MAX_NUM_LEN/2];     //12 ���к���

    //(MOSMSRecord)(MTSMSRecord)systemType
    BYTE		systemType:2;//USER_NETWORK_TYPE :4; 
    BYTE		s5               :6;

    //(MOSMSRecord)(MTSMSRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  �����ô�ҵ��Ϊ�绰ҵ��Ϊ��0���������ҵ��Ϊ��1����    
    BYTE		ROAM_mark               :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  ����TBS��绰ҵ������ҵ���롣
    
    //(MOSMSRecord)(MTSMSRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // �Ʒ���ѱ�־
    BYTE		Classmark3Indication:1;
    BYTE		s7                :3;
   
    //(MOSMSRecord)(MTSMSRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ����ָʾ
    UINT2		money_per_count; // ����
    UINT2		add_fee;                 //���ӷ�

    //(MOSMSRecord)(MTSMSRecord)chargedParty  Ŀǰû����д����Ԥ������λ
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId����ָʾ
    BYTE		msc_id_len:3;//orgMSCId����ָʾ
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

    //(MOSMSRecord)(MTSMSRecord)hotBillingTag���ڻ�����HotBillingTag

    //CAMEL SMS��������Ϣ
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

    //CAMELSMSInformation:smsReferenceNumber Ŀǰû����д��Ԥ��
    BYTE		smsReferenceNumberLen;
    BYTE		smsReferenceNumber[maxCallReferenceNumberLength];
    
    //(MOSMSRecord)(MTSMSRecord)recordExtensions
    //  ��ǰ���Ʒ��ƶ��û�����MSC
    BYTE		served_msc_num_npi : 4;
    BYTE		served_msc_num_nai :  3;
    BYTE		s12            : 1;
    BYTE		served_msc_num_BCDlen ;
    BYTE		served_msc_num[MAX_MSC_NUM_IN_BYTES];       

    //(MOSMSRecord)(MTSMSRecord)recordExtensions
    //������������������չ
    BYTE		container[MAX_CONTAINER_SMSMO_LEN];

    //CAMELSMSInformation:freeFormatData
    //(MOSMSRecord)(MTSMSRecord)smstext Ŀǰû����д��Ԥ��
    BYTE		fcidata_len;
    BYTE		smstext_len;
    BYTE		VarPart[MAX_TRANSFER_FCI_LEN + MAX_SMSTEXT_LEN];
    // BYTE		FCIData[MAX_TRANSFER_FCI_LEN];
    // BYTE		smstext[MAX_SMSTEXT_LEN];	
};

//1.8	HLR_Query_Bill_Report������HLR Query(HLRIntRecord)��
struct HLR_Query_Bill_Report
{
	// �����򲿷�, ������:

	BYTE		validflag; // ��Ч�Ա�־,����AA��Ϊ��Ч����55��Ϊ��Ч
    UINT2		size; // ���Ż�������
    UINT2		CRCCode; // CRCУ����
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // ������ˮ�ţ�Ψһ����һ�Ż�����
    BYTE		ModuleNum; // ����������ģ���
    UINT4		count; // GSM �Ʒѵ�һ�κ��в������Ż���ʱcount �Ǹôκ����׻�����ˮ��
    BYTE		sequence; // ���Ż���Ϊ0�����Ż���Ϊ1����������
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, ���������м�ĩ�Ż�����ʶ
    BYTE		HotBillingTag:1; //Ϊcmcc�������ӵ��ȼƷѱ�־
    BYTE		CRC_Way:2; // CRCУ�鷽ʽ
    BYTE		ChargeLevel:1;//�ƷѾ���ָʾ:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//�����򲿷ֵ���չ�ռ�
    BYTE		Comm_reserved;//�����򲿷ֵ���չ�ռ�
	
	//  ��չ��:
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  ���Ʒ��ƶ��û�IMSI��
	
    //(HLRIntRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // ���Ʒ��ƶ��û�MSISDN��
	BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(HLRIntRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//served_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//served_msc_num_nai :  3;   
    BYTE		s2            : 1;   
    BYTE		recordingEntity_BCDlen ;//served_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //served_msc_num[8];         //8  ��ǰ���Ʒ��ƶ��û�����MSC
	
    //(HLRIntRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  �����ô�ҵ��Ϊ�绰ҵ��Ϊ��0���������ҵ��Ϊ��1����    
    BYTE		s3            :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  ����TBS��绰ҵ������ҵ���롣
	
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
	
    //(HLRIntRecord)interrogationResult û����д����Ԥ���ֽ�
    SDiagnostics     interrogationResult;
	
    //(HLRIntRecord)orgMSCId
    BYTE		msc_id_len:3;//orgMSCId����ָʾ
    BYTE		s6        :5;
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];
    
    //(HLRIntRecord)recordExtensions
    //������������������չ
    BYTE		container[MAX_CONTAINER_HLRQUERY_LEN];
	
};

//1.9	SS_Action_Bill_Report������SS Action(SSActionRecord)��EVENT��
struct SS_Action_Bill_Report
{
	// �����򲿷�, ������:

	BYTE		validflag; // ��Ч�Ա�־,����AA��Ϊ��Ч����55��Ϊ��Ч
    UINT2		size; // ���Ż�������
    UINT2		CRCCode; // CRCУ����
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // ������ˮ�ţ�Ψһ����һ�Ż�����
    BYTE		ModuleNum; // ����������ģ���
    UINT4		count; // GSM �Ʒѵ�һ�κ��в������Ż���ʱcount �Ǹôκ����׻�����ˮ��
    BYTE		sequence; // ���Ż���Ϊ0�����Ż���Ϊ1����������
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, ���������м�ĩ�Ż�����ʶ
    BYTE		HotBillingTag:1; //Ϊcmcc�������ӵ��ȼƷѱ�־
    BYTE		CRC_Way:2; // CRCУ�鷽ʽ
    BYTE		ChargeLevel:1;//�ƷѾ���ָʾ:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//�����򲿷ֵ���չ�ռ�
    BYTE		Comm_reserved;//�����򲿷ֵ���չ�ռ�
	
	//  ��չ��:
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  ���Ʒ��ƶ��û�IMSI��

    //(SSActionRecord)servedIMEI
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  ���Ʒ��ƶ��û��ƶ�̨�豸ʶ����

    //(SSActionRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;    
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // ���Ʒ��ƶ��û�MSISDN��
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(SSActionRecord)MSClassmark[3]
    BYTE   MSClassmark[3];//BYTE  ClassMark2;   
    struct  BillClassmark3 Classmark3Information;
    
    //(SSActionRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//    BYTE     served_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//    BYTE     served_msc_num_nai :  3;    
    BYTE		s2            : 1;       
    BYTE		recordingEntity_BCDlen ;//    BYTE     served_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //    BYTE    served_msc_num[8];       //8  ��ǰ���Ʒ��ƶ��û�����MSC

    //(SSActionRecord)location �����еĴ����һ
    BYTE		served_location_LAI[MAX_LOCAL_LAI_LEN/2];//caller_LAC[MAX_LOCAL_LAC_LEN/2];called_LAC[MAX_LOCAL_LAC_LEN/2]; 
    BYTE		served_location_cellID[MAX_LOCAL_CI_LEN/2];//caller_cell_ID[MAX_LOCAL_CI_LEN/2]; called_cell_ID[MAX_LOCAL_CI_LEN/2]; 

    //(SSActionRecord)BasicServices
    BYTE		TBS:4;//TBS:4;        //  �����ô�ҵ��Ϊ�绰ҵ��Ϊ��0���������ҵ��Ϊ��1����    
    BYTE		s3          :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  ����TBS��绰ҵ������ҵ���롣

    //(SSActionRecord)supplService
    BYTE		SSCode;

    //(SSActionRecord)ssAction
    BYTE		ssAction;//SSAction; //SSActionType

    //(SSActionRecord)ssActionTime
    BYTE		ssActionTime_year; //����ҵ����������ʱ���
    BYTE		ssActionTime_month;
    BYTE		ssActionTime_date;
    BYTE		ssActionTime_hour;            //3
    BYTE		ssActionTime_minute;
    BYTE		ssActionTime_second;

    //(SSActionRecord)ssActionResult
    SDiagnostics		ssActionResult;//SSResult;

    //(SSActionRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; ����ͬһ�ƶ�̨�����Ĳ�ͬ����ı���ʶ���

   //(SSActionRecord)systemType
    BYTE		systemType:2;//AccessNetworkType;     
    //(SSActionRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // �Ʒ���ѱ�־
    BYTE		Classmark3Indication:1;
    BYTE		s4 :1;    
     
    //(SSActionRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ����ָʾ
    UINT2		money_per_count; // ����
    UINT2		add_fee;                 //���ӷ�

    //(SSActionRecord)chargedParty Ŀǰû����д����Ԥ������λ
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId����ָʾ
    BYTE		msc_id_len:3;//orgMSCId����ָʾ
    BYTE		s23            : 2;
    //(SSActionRecord)orgRNCorBSCId
    BYTE		orgRNCorBSCId[MAX_RNCBSC_ID_LEN];//Rnc_Bsc_Id[3];

    //(SSActionRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(SSActionRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;

    //(SSActionRecord)recordExtensions
    BYTE		event_type;  //��¼�����¼��������¼�����:CH,CW,MPTY,HO: enum EVENT_TYPE    

    //ussd�����Ϣ,�ο������ӵĽӿڣ�Ԥ��
    BYTE		UssdAccessCode[MAX_USSDACCESSCODE_LEN];
    BYTE		UssdCodingScheme;
    BYTE		UssdNotifyCounter ;
    BYTE		UssdRequestCounter;  

    // Deleted by zengsonglin for R002C01 2004-04-07
    // BYTE     UssdString[MAXUINT2SDSTRING_LEN];

    //changeofAOC�����¼���������Ϊ����������������
    AOC_E1_E7    aocParameters;  

    //������������������չ
    BYTE     container[MAX_CONTAINER_SSACT_LEN];

    //(SSActionRecord)ssParameters 
    SSParameters   SSParameter;
    /*VarPart
    ע��SSParameters������ǿɱ䳤�ȵ�
    ��SS_Para_lenΪ�ɱ䲿�ֳ���ָʾ
    �ɱ䲿����󳤶�Ϊ40�ֽ�
    */	
};

//1.10	CEU_Bill_Report������CEU(CommonEquipRecord)��
struct CEU_Bill_Report
{
	// �����򲿷�, ������:
	BYTE		validflag; // ��Ч�Ա�־,����AA��Ϊ��Ч����55��Ϊ��Ч
    UINT2		size; // ���Ż�������
    UINT2		CRCCode; // CRCУ����
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // ������ˮ�ţ�Ψһ����һ�Ż�����
    BYTE		ModuleNum; // ����������ģ���
    UINT4		count; // GSM �Ʒѵ�һ�κ��в������Ż���ʱcount �Ǹôκ����׻�����ˮ��
    BYTE		sequence; // ���Ż���Ϊ0�����Ż���Ϊ1����������
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, ���������м�ĩ�Ż�����ʶ
    BYTE		HotBillingTag:1; //Ϊcmcc�������ӵ��ȼƷѱ�־
    BYTE		CRC_Way:2; // CRCУ�鷽ʽ
    BYTE		ChargeLevel:1;//�ƷѾ���ָʾ:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//�����򲿷ֵ���չ�ռ�
    BYTE		Comm_reserved;//�����򲿷ֵ���չ�ռ�
	
	//  ��չ��:
	
    BYTE		equipmentType;//Equipment_type;

    //(CommonEquipRecord)equipmentId
    UINT2		equipmentId;//Equipment_Id;

    //(CommonEquipRecord)servedIMSI
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  ���Ʒ��ƶ��û�IMSI��

    BYTE		servedIMEI[MAX_IMEI_LEN];  //  ���Ʒ��ƶ��û��ƶ�̨�豸ʶ����

    //(CommonEquipRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;    
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // ���Ʒ��ƶ��û�MSISDN��
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(CommonEquipRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//served_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//served_msc_num_nai :  3;   
    BYTE		s2            : 1;    
    BYTE		recordingEntity_BCDlen ;//served_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //served_msc_num[8];          //8  ��ǰ���Ʒ��ƶ��û�����MSC

    //(CommonEquipRecord)basicService 
    BYTE		TBS:4;//TBS:4;        //  �����ô�ҵ��Ϊ�绰ҵ��Ϊ��0���������ҵ��Ϊ��1����    
    BYTE		s3     :4;
    BYTE		ServiceCode;//tele_bearer_code;  //  ����TBS��绰ҵ������ҵ���롣
   //(CommonEquipRecord)supplServicesUsed
    BYTE		SSCode;
    //(CommonEquipRecord)seizureTime
    BYTE		initial_year;    // ͨ����ʼʱ��
    BYTE		initial_month;
    BYTE		initial_date;
    BYTE		initial_hour;            //3
    BYTE		initial_minute;
    BYTE		initial_second;

    //(CommonEquipRecord)releaseTime 
    BYTE		end_year;               // ͨ������ʱ��
    BYTE		end_month;
    BYTE		end_date;
    BYTE		end_hour;
    BYTE		end_minute;
    BYTE		end_second;

    //(CommonEquipRecord)callDuration
    UINT4		callDuration;//Call_Duration; ͨ��ʱ��

    //(CommonEquipRecord)callReference 
    BYTE		call_reference_len;
    BYTE		call_reference[maxCallReferenceNumberLength];    //call_reference[maxCallReferenceNumberLength]; ����ͬһ�ƶ�̨�����Ĳ�ͬ����ı���ʶ���

    //(CommonEquipRecord)sequenceNumber: �м仰�����к�,���ڹ����򲿷�, ������:sequence��

    //(CommonEquipRecord)systemType 
    BYTE		systemType:2;//AccessNetworkType     
    BYTE		s4       :6;
    
    //(CommonEquipRecord)rateIndication
    BYTE		rateIndication;//ucRateIndication; //����ҵ����������

    //(CommonEquipRecord)fnur
    BYTE		fnur;//ucFunr;           //����ҵ���û�����,enum FNUR

    //(CommonEquipRecord)additionalChgInfo
    BYTE		chargeIndicator:4;//free_indicator :4;  // �Ʒ���ѱ�־    
 	BYTE		s5         :4;
   
    //(CommonEquipRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ����ָʾ
    UINT2		money_per_count; // ����
    UINT2		add_fee;                 //���ӷ�

    //(CommonEquipRecord)chargedParty Ŀǰû����д����Ԥ������λ
    BYTE     chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId����ָʾ
    BYTE		msc_id_len:3;//orgMSCId����ָʾ
    BYTE		s23            : 2;
    //(CommonEquipRecord)orgRNCorBSCId
    BYTE		orgRNCorBSCId[MAX_RNCBSC_ID_LEN];//Rnc_Bsc_Id[3];

    //(CommonEquipRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(CommonEquipRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;


    BYTE		causeForTerm;//cause_for_termination;        //  ͨ����ֹԭ��
    SDiagnostics     diagnostics;//uDiagnostics;
    BYTE		cause_for_partial_record ;      //  ���м仰��ԭ��

    //������������������չ
    BYTE		container[MAX_CONTAINER_CEU_LEN];
	
};

//1.11	LCS_Bill_Report������MO LCS(MOLCSRecord)��MT LCS(MTLCSRecord)��NI LCS(NILCSRecord)��
struct LCS_Bill_Report
{
	// �����򲿷�, ������:
	BYTE		validflag; // ��Ч�Ա�־,����AA��Ϊ��Ч����55��Ϊ��Ч
    UINT2		size; // ���Ż�������
    UINT2		CRCCode; // CRCУ����
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // ������ˮ�ţ�Ψһ����һ�Ż�����
    BYTE		ModuleNum; // ����������ģ���
    UINT4		count; // GSM �Ʒѵ�һ�κ��в������Ż���ʱcount �Ǹôκ����׻�����ˮ��
    BYTE		sequence; // ���Ż���Ϊ0�����Ż���Ϊ1����������
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, ���������м�ĩ�Ż�����ʶ
    BYTE		HotBillingTag:1; //Ϊcmcc�������ӵ��ȼƷѱ�־
    BYTE		CRC_Way:2; // CRCУ�鷽ʽ
    BYTE		ChargeLevel:1;//�ƷѾ���ָʾ:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//�����򲿷ֵ���չ�ռ�
    BYTE		Comm_reserved;//�����򲿷ֵ���չ�ռ�
	
	//  ��չ��:
	
    BYTE		lcs_bill_type;//location_type;             // 1 enum LOCATION_TYPE: MT��MO��NI
    
    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)recordingEntity
    BYTE		recordingEntity_npi : 4;//served_msc_num_npi : 4;
    BYTE		recordingEntity_nai : 3;//served_msc_num_nai :  3;
    BYTE		s1            : 1;    
    BYTE		recordingEntity_BCDlen ;//served_msc_num_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //served_msc_num[8];         //8  ��ǰ���Ʒ��ƶ��û�����MSC

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)lcsClientType
    BYTE		lcsClientType;//LCSClientType;  //enum LCSClientType

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)lcsClientIdentity  
    BYTE		lcsClientIdentity_npi : 4;//lcs_client : 4;
    BYTE		lcsClientIdentity_nai : 3;//lcs_client :  3; 
    BYTE		s2            : 1;    
    BYTE		lcsClientIdentity_BCDlen ;//lcs_client : 5;
    BYTE		lcsClientIdentity[MAX_LCS_CLIENT_EXTERNAL_ID_LEN];//    BYTE    lcs_client[MAX_LCS_CLIENT_EXTERNAL_ID_LEN]; //20 �ⲿ��LCS-CLIENT����

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)servedIMSI
    BYTE		servedIMSI[MAX_IMSI_LEN];  //    BYTE    target_IMSI[MAX_IMSI_LEN];          //Ŀ��IMSI���룬8�ֽ�

    //(NILCSRecord)servedIMEI  
    BYTE		servedIMEI[MAX_IMEI_LEN];//    BYTE    target_IMEI[MAX_IMEI_LEN];          //Ŀ��IMEI���룬8�ֽ�

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)servedMSISDN
    BYTE		servedMSISDN_npi : 4;//target_num_npi : 4;
    BYTE		servedMSISDN_nai :  3;//target_num_nai :  3;    
    BYTE		s3            : 1;    
    BYTE		servedMSISDN_BCDlen;//target_num_num : 5;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//target_num [MAX_NUM_LEN/2+1]; //Ŀ����룬13�ֽ�
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    //(MTLCSRecord)locationType
    BYTE		locationType;//LocationEstimateType;

    //(MOLCSRecord)molr-Type
    BYTE		molr_Type;//MOLRType;  //enum MOLR_Type

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)lcsQos 
    //QOS ��λ����Ҫ����Ӧʱ���
    BYTE		horizontal_accuracy; //ˮƽ����
    BYTE		verticalCoordinateRequest; //0��Ҫ��1Ҫ��
    BYTE		vertical_accuracy;   //��ֱ���򾫶�
    BYTE		responseTime; //��λ��Ӧʱ��

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)lcsPriority
    BYTE		lcsPriority;//lcs_priority;                 //  ��λ���ȼ�

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)mlc-Number
    BYTE		GMLC_Number_npi : 4;//GMLC_address_npi : 4;
    BYTE		GMLC_Number_nai :  3;//GMLC_address_nai :  3;    
    BYTE		s4            : 1;    
    BYTE		GMLC_Number_BCDlen;//GMLC_address_num : 5;
    BYTE		GMLC_Number[MAX_GMLC_LEN + 1];//GMLC_address[9]; //9 GMLC����    

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)eventTimeStamp
    BYTE		eventTimeStamp_year;     //initial_year; // ͨ����ʼʱ��
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
    BYTE		served_location_LAI[MAX_LOCAL_LAI_LEN/2]; //target_LAC[MAX_LOCAL_LAC_LEN/2];    // Ŀ��λ�����ţ�2�ֽ�
    BYTE		served_location_cellID[MAX_LOCAL_CI_LEN/2];//target_cell_ID[MAX_LOCAL_CI_LEN/2]; // Ŀ��С���ţ�2�ֽ�

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
    BYTE		chargeIndicator:4;//free_indicator :4;  // �Ʒ���ѱ�־   
    BYTE		s6            :4;

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)additionalChgInfo:chargeParameters
    UINT2		charging_case;       // ����ָʾ
    UINT2		money_per_count; // ����
    UINT2		add_fee;                 //���ӷ�

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)chargedParty Ŀǰû����д����Ԥ������λ
    BYTE	    chargedParty; //enum MBILL_CHARGED_PARTY

    BYTE		rnc_bsc_id_len:3;//orgRNCorBSCId����ָʾ
    BYTE		msc_id_len:3;//orgMSCId����ָʾ
    BYTE		s23            : 2;
    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)orgRNCorBSCId
    BYTE		orgRNCorBSCId[MAX_RNCBSC_ID_LEN];//Rnc_Bsc_Id[3];

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)orgMSCId
    BYTE		orgMSCId[MAX_MSCID_LEN];//Msc_Id[3];

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)subscriberCategory
    BYTE		subscriberCategory;//User_category;

    //(MOLCSRecord)location_method
    BYTE		location_method;                 //��λ���� enum _locaton_method
    
    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)recordExtensions

    //(NILCSRecord)emsDigitsĿǰû����д��Ԥ��
    BYTE		emsDigits_npi : 4;
    BYTE		emsDigits_nai :  3;   
    BYTE		s7            : 1;    
    BYTE		emsDigits_BCDlen;
    BYTE		emsDigits[MAX_NUM_LEN/2];

    //(NILCSRecord)emsKeyĿǰû����д��Ԥ��
    BYTE		emsKey_npi : 4;
    BYTE		emsKey_nai :  3;  
    BYTE		s8            : 1;    
    BYTE		emsKey_BCDlen;
    BYTE		emsKey[MAX_NUM_LEN/2];

    //������������������չ
    BYTE		container[MAX_CONTAINER_LCS_LEN];

    //(MTLCSRecord)(MOLCSRecord)(NILCSRecord)positioningData 
    BYTE		positioningDataLen;//    BYTE     shapeinfoLen;
    
    BYTE        call_reference_len;
    BYTE        call_reference[maxCallReferenceNumberLength];
    
    BYTE		VarPart[maxShapeInfoLength];//BYTE     shapeInfo[maxShapeInfoLength];
    /*ע��
    VarPart: positioningData���ǿɱ䲿��,��󳤶�ΪmaxShapeInfoLength
    */
	
};

//1.12	CheckIMEI_Bill_Report������CHECK IMEI��
struct CheckIMEI_Bill_Report
{
	// �����򲿷�, ������:

	BYTE		validflag; // ��Ч�Ա�־,����AA��Ϊ��Ч����55��Ϊ��Ч
    UINT2		size; // ���Ż�������
    UINT2		CRCCode; // CRCУ����
    BYTE		bill_type; //  enum   GSM_BILL_TYPE
    UINT4		bill_sequence; // ������ˮ�ţ�Ψһ����һ�Ż�����
    BYTE		ModuleNum; // ����������ģ���
    UINT4		count; // GSM �Ʒѵ�һ�κ��в������Ż���ʱcount �Ǹôκ����׻�����ˮ��
    BYTE		sequence; // ���Ż���Ϊ0�����Ż���Ϊ1����������
    BYTE		record_type :3; //enum GSM_RECORD_TYPE, ���������м�ĩ�Ż�����ʶ
    BYTE		HotBillingTag:1; //Ϊcmcc�������ӵ��ȼƷѱ�־
    BYTE		CRC_Way:2; // CRCУ�鷽ʽ
    BYTE		ChargeLevel:1;//�ƷѾ���ָʾ:enum MBILL_CHARGE_LEVEL
    BYTE		Comm_reserved_bit:1;//�����򲿷ֵ���չ�ռ�
    BYTE		Comm_reserved;//�����򲿷ֵ���չ�ռ�
	
	//  ��չ��:
	
    BYTE		checkIMEI_year;          //initial_year; // ͨ����ʼʱ��
    BYTE		checkIMEI_month;       //initial_month;
    BYTE		checkIMEI_date;       //initial_date;
    BYTE		checkIMEI_hour;          //initial_hour;            
    BYTE		checkIMEI_minute;       //initial_minute;
    BYTE		checkIMEI_second;       //initial_second;
	
    BYTE		servedIMSI[MAX_IMSI_LEN];  //  ���Ʒ��ƶ��û�IMSI��
    BYTE		servedIMEI[MAX_IMEI_LEN];  //  ���Ʒ��ƶ��û��ƶ�̨�豸ʶ����
	
    BYTE		servedMSISDN_npi : 4;
    BYTE		servedMSISDN_nai :  3;   
    BYTE		s1            : 1;    
    BYTE		servedMSISDN_BCDlen;
    BYTE		servedMSISDN[MAX_NUM_LEN/4];//msisdn[MAX_NUM_LEN/2];           // ���Ʒ��ƶ��û�MSISDN��
    BYTE        servedMSISDN_reserved[MAX_NUM_LEN/4];
    
    BYTE		served_location_LAI[MAX_LOCAL_LAI_LEN/2]; //target_LAC[MAX_LOCAL_LAC_LEN/2];    // Ŀ��λ�����ţ�5�ֽ�
    BYTE		served_location_cellID[MAX_LOCAL_CI_LEN/2]; //target_cell_ID[MAX_LOCAL_CI_LEN/2]; // Ŀ��С���ţ�2�ֽ�
    
    BYTE		IMEIStatus;  //enum _Equipment_Status
    BYTE		Event;  //enum IMEICheckEvent
    
    BYTE		recordingEntity_npi : 4;//served_msc_npi : 4;
    BYTE		recordingEntity_nai : 3;//served_msc_nai :  3;  
    BYTE		s2            : 1;    
    BYTE		recordingEntity_BCDlen ;//served_msc_num : 5;
    BYTE		recordingEntity[MAX_MSC_NUM_IN_BYTES];  //served_msc[8];         //����MSC����
	
    BYTE		msc_id_len:3;//orgMSCId����ָʾ
    BYTE		s3            : 5;
    BYTE		orgMSCId[MAX_MSCID_LEN];
	
    BYTE		CallReferenceLen;
    BYTE		CallReference[maxCallReferenceNumberLength];
	
    //������������������չ
    BYTE		container[MAX_CONTAINER_CHECKIMEI_LEN];
	
};

#pragma pack()

#endif  //__MSCV9_BILL_FORMAT_H__
