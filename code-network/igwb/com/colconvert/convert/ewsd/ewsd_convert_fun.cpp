//转换S1240详细话单到文本格式

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../comm_convert_fun.h"
#include "../include/collect_convert_define.h"
#include "../../include/collect_convert_include.h"
#include "../../../include/toolbox.h"

//西门子话单文件中一个Block的长度
#define SIEMENS_BLOCK_SIZE	8192

//AMA文件在交换机硬盘和磁带上按照一个个Block存储，一个Block地长度是8192字节。若正常
//话单的长度不足8192字节，在记录的末尾会填充一些填充话单以达到8192字节的长度。

//每条记录的头结构
typedef struct
{
	unsigned char Phv;				//记录指示，为0x84表示话单长度可变
	unsigned char L_Rec_Len;		//L_Rec_Len和H_Rec_Len表示包括Header(SiemensP84H)
	unsigned char H_Rec_Len: 4;		//在内的整张话单的长度，12bits,低位在前
	unsigned char Res	   : 4;		//
	unsigned char Bill_Flag1 : 7;	//计费特性
	unsigned char F18 : 1;			//1-Record Owner Type in Fixed Part, 0-Record Owner DN in Fixed Part
	unsigned char Bill_Flag2[2];		//计费特性
	unsigned char PartialID : 4;	//部分记录指示(0－保留，1－单条，2－首张，3－中间，4－保留，5－末张)
	unsigned char ChargeFlag: 4;	//计费标记(0－未定义，1－计费，2－免费，3－被叫付费)
	unsigned char Rec_Owner_Type;
} SiemensP84H_Rec_Owner_Type;

//每条记录的头结构
typedef struct
{
	unsigned char Phv;				//记录指示，为0x84表示话单长度可变
	unsigned char L_Rec_Len;		//L_Rec_Len和H_Rec_Len表示包括Header(SiemensP84H)
	unsigned char H_Rec_Len: 4;		//在内的整张话单的长度，12bits,低位在前
	unsigned char Res	   : 4;		//
	unsigned char Bill_Flag1 : 7;	//计费特性
	unsigned char F18 : 1;			//1-Record Owner Type in Fixed Part, 0-Record Owner DN in Fixed Part
	unsigned char Bill_Flag2[2];		//计费特性
	unsigned char PartialID : 4;	//部分记录指示(0－保留，1－单条，2－首张，3－中间，4－保留，5－末张)
	unsigned char ChargeFlag: 4;	//计费标记(0－未定义，1－计费，2－免费，3－被叫付费)
	unsigned char Caller_Len: 5;	//主叫号码长度
	unsigned char LAC_Len   : 3;	//LAC长度
	//unsigned char Value[];		//主叫号码，长度由Caller_Len决定
} SiemensP84H_Rec_Owner_DN;

//通话时间
typedef struct 
{
	unsigned char Phv;			//值为0x64表示通话时间
	unsigned char Year;			//年，00－99
	unsigned char Month;		//月，1－12
	unsigned char Day;			//日，1－31
	unsigned char Hour;			//时，0－23
	unsigned char Minute;		//分，0－59
	unsigned char Second;		//秒，0－59
	unsigned char YearFlag : 1; //年份指示，0－1900至1999，1－2000～
	unsigned char Res      : 5;	//保留
	unsigned char Se_Flag  : 1;	//0－可靠时间，1－不可靠时间
	unsigned char Time_Flag: 1;	//0－通话开始时间，1－通话结束时间
	unsigned char Duration[3];	//通话时长
} SiemensP64H;

//被叫号码
typedef struct 
{
	unsigned char Phv;			//值为0x65表示被叫号码
	unsigned char Called_Len: 5;//被叫号码长度
	unsigned char Res       : 3;//保留
	//unsigned char Value[];	//被叫号码，长度由Called_Len决定
} SiemensP65H;

//业务信息
typedef struct 
{
    unsigned char Phv;			//0x66表示业务信息
	unsigned char ServiceInd;	//业务信息指示，Binary
	unsigned char AddInfo;		//附加信息
	unsigned char Flag : 1;		//
	unsigned char Res  : 7;		//保留
} SiemensP66H;

//呼叫计费单元
typedef struct
{
	unsigned char Phv;			//值为0x67表示计费单元
	unsigned char Ccu[3];		//计费单元(Call Charge Units)
} SiemensP67H;

//FAU计费单元
typedef struct
{
	unsigned char Phv;			//值为0x68表示FAU计费单元
	unsigned char Ccu[2];		//FAU计费单元
} SiemensP68H;

//入中继
typedef struct
{
	unsigned char Phv;				//值为0x69表示入中继
	unsigned char Tgrp_Name[6];		//中继群名称
	unsigned char Trunk_No_In[2];	//入中继群号，数值型，低位在前
} SiemensP69H;

//出中继
typedef struct
{
	unsigned char Phv;				//值为0x6A表示出中继
	unsigned char Tgrp_Name[6];		//中继群名称
	unsigned char Trunk_No_Out[2];	//出中继群号，数值型，低位在前
} SiemensP6AH;

//CIC入中继格式结构
typedef struct
{   
   unsigned char Phv;				//0x6B表示CIC入中继格式结构
   unsigned char Tgrp_Name[6];		//Tgrp_Name：中继群名称
   unsigned char CIC_Pn[2];			//CICpcm号
   unsigned char CIC_Cn;			//CIC计费号
} SiemensP6BH;

//CIC出中继格式结构
typedef struct
{   
   unsigned char Phv;				//0x6C表示CIC出中继格式结构
   unsigned char Tgrp_Name[6];		//中继群名称
   unsigned char CIC_Pn[2];			//CICpcm号	
   unsigned char CIC_Cn;			//CIC计费号
} SiemensP6CH;

typedef struct 
{
	unsigned char Phv;				//0x6E表示Connection Identifier Number
	unsigned char Len;				//长度，固定为0x06
	unsigned char ConnNum[4];		//Connection Number
} SiemensP6EH;

typedef struct 
{
	unsigned char Phv;				//0x6F表示Facilities Used By Owner
	unsigned char Len;				//
	//unsigned char OFac[];			//
} SiemensP6FH;

typedef struct 
{
	unsigned char Phv;				//0x70表示Facilities Used By Partner
	unsigned char Len;				//
	//unsigned char PFac[];			//
} SiemensP70H;

typedef struct 
{
	unsigned char Phv;			//值为0x74表示通话时间
	unsigned char Year;			//年，00－99
	unsigned char Month;		//月，1－12
	unsigned char Day;			//日，1－31
	unsigned char Hour;			//时，0－23
	unsigned char Minute;		//分，0－59
	unsigned char Second;		//秒，0－59
	unsigned char YearFlag : 1; //年份指示，0－1900至1999，1－2000～
	unsigned char Res      : 5;	//保留
	unsigned char Se_Flag  : 1;	//0－可靠时间，1－不可靠时间
	unsigned char Time_Flag: 1;	//0－通话开始时间，1－通话结束时间
} SiemensP74H;

typedef struct
{
	unsigned char Phv;			//0x75-Project Specific Data
	unsigned char Package_Len;	//表示该Package的长度，包括所有字段
} SiemensP75H;

typedef struct 
{
	unsigned char Phv;			//0x76,Digit String
	unsigned char Len;			//
	//unsigned char Digit[];	//
} SiemensP76H;

typedef struct 
{
	unsigned char Phv;			//0x77,Transmission Medium Requirement
	unsigned char Trans;		//0-3.1KHz, 1-Speech, 2-64K unrestricted, 3-64K unrestricted with fallback
} SiemensP77H;

typedef struct 
{
	unsigned char Phv;			//0x78,calling party category
	unsigned char CPC;			//
} SiemensP78H;

//计费区
typedef struct
{
	unsigned char Phv;			//值为0x7A表示区计费结构
	unsigned char Zone[2];		//计费区
} SiemensP7AH;

//计费区
typedef struct
{
	unsigned char Phv;			//值为0x7D Account Code
	unsigned char Len;			//
	unsigned char F1 : 1;		
	unsigned char Res: 7;
	unsigned char DigitNum;		//

	//unsigned char Digits[];	//
} SiemensP7DH;

//New Destination
typedef struct
{
	unsigned char Phv;			//0x80--New Destination
	unsigned char Len;			
	unsigned char NADI : 7;		//
	unsigned char Res1 : 1;
	unsigned char Res2 : 4;
	unsigned char NPI  : 3;
	unsigned char Res3 : 1;	
	unsigned char DigitNum : 5;
	unsigned char res4 : 3;
	//unsigned char Digits[];
} SiemensP80H;

//通话质量
typedef struct 
{
	unsigned char Phv;				//值为0x82表示通话质量
	unsigned char Package_Len;		//表示该Package的长度，包括所有字段
	unsigned char Cause_Value[2];	//释放原因(参见CCITT(Q.931)，共127种，16为正常释放)
	unsigned char Location : 4;		//位置指示(参见CCITT(Q.931)，共16种，2为本地公共网)
	unsigned char Res1 : 1;
	unsigned char CodingStandard : 2;
	unsigned char res2 : 1;
} SiemensP82H;

//Duration Before Answer
typedef struct
{
	unsigned char Phv;				//0x86-Duration Before Answer
	unsigned char Package_Len;		//表示该Package的长度，包括所有字段
	unsigned char Duration1[2];		//Duration from dial tone till address complete
	unsigned char Duration2[2];		//Duration from address complete till answer
} SiemensP86H;

//Chargeband
typedef struct
{
	unsigned char Phv;				//0x87-Chargeband
	unsigned char Package_Len;
	unsigned char Chargeband[2];
	unsigned char ChargebandMsg : 1; //0-No chargeband message, 1- chargeband message received
	unsigned char Res1 :7;
} SiemensP87H;

//UUS3 Counters
typedef struct
{
	unsigned char Phv;				//0x88-UUS3 Counters
	unsigned char Package_Len;
	unsigned char MsgReceived[2];
	unsigned char MsgSent[2];
} SiemensP88H;

//B Meter Pulses
typedef struct
{
	unsigned char Phv;				//0x8A-B Meter Pulse
	unsigned char Package_Len;
	unsigned char B_Meter_Pulses[2];
} SiemensP8AH;

//Release Indication
typedef struct
{
	unsigned char Phv;
	unsigned char Package_Len;
	unsigned char Release_Indication : 2;
	unsigned char Res1 : 6;
} SiemensP8BH;

//Incoming Route ID
typedef struct
{
	unsigned char Phv;
	unsigned char Package_Len;
	unsigned char RouteIndication : 2;
	unsigned char Res : 6;
	//subPackages
} SiemensP8CH;

//Outgoing Route ID
typedef struct
{
	unsigned char Phv;
	unsigned char Package_Len;
	unsigned char RouteIndication : 2;
	unsigned char Res : 6;
	unsigned char TrunkGroupName[6];
	unsigned char TrunkGroupLine[2];
} SiemensP8DH;

//主叫号码
typedef struct
{
	unsigned char Phv;				//值为0x8E表示主叫号码
	unsigned char Package_Len;		//表示该Package的长度，包括所有字段
	unsigned char NADI : 7   ;		//Nature of Address Indicator
	unsigned char Res1 : 1;
	unsigned char Screening : 2;
	unsigned char Present_Indicator : 2;
	unsigned char NPI : 3;			//Numbering Plan Indicator
	unsigned char Res2 : 1;
	unsigned char DigitNum : 5;
	unsigned char LAC_Len  : 3;
	//unsigned char packedDigits[];
} SiemensP8EH;

//IP/SN Charge Data
typedef struct
{
	unsigned char Phv;				//0x8F
	unsigned char Package_Len;
	//unsigned char Data[]; max 20 bytes
} SiemensP8FH;

//Billing CAC
typedef struct
{
	unsigned char Phv;				//0x91
	unsigned char Package_Len;
	unsigned char CACDigitsNum : 3;
	unsigned char PrefixDigitsNum : 2;
	unsigned char CACType : 3;
	//unsigned char packedDigits[]
} SiemensP91H;

//Third Party Number
typedef struct
{
	unsigned char Phv;				//0x92
	unsigned char Package_Len;
	unsigned char Type : 4;
	unsigned char Res1 : 4;
	unsigned char Res2;
	unsigned char NADI : 7;
	unsigned char Res3 : 1;
} SiemensP92H;

//Tariff Info
typedef struct
{
	unsigned char Phv;				//0x93
	unsigned char Package_Len;
	unsigned char CtrIndictr;
	unsigned char Currency;
	//subPackage
} SiemensP93H;

//CAC New Destination
typedef struct
{
	unsigned char Phv;				//0x94
	unsigned char Package_Len;
	unsigned char DigitsNum : 3;
	unsigned char Res1 : 5;
	//unsigned char packedDigits
} SiemensP94H;

//Global Charge Reference
typedef struct
{
	unsigned char Phv;				//0x95
	unsigned char Package_Len;
	unsigned char Res1;
	//unsigned char ChargeReference[];
} SimensP95H;

//Audit Trail
typedef struct
{
	unsigned char Phv;				//0x96
	unsigned char Package_Len;
	unsigned char SequenceNum[4];
	unsigned char CheckSum1;
	unsigned char CheckSum2;
} SimensP96H;

//Business Group Identification
typedef struct
{
	unsigned char Phv;				//0x97
	unsigned char Package_Len;
	unsigned char BusinessGrpID[3];
} SimensP97H;

//Centrex Numbers
typedef struct
{
	unsigned char Phv;				//0x98
	unsigned char Package_Len;
	unsigned char Res1;
	unsigned char DigitsNum;
	//unsigned char packedDigits[];
} SiemensP98H;

//Received Called Party Number
typedef struct
{
	unsigned char Phv;				//0x99
	unsigned char Package_Len;
	unsigned char NADI : 7;
	unsigned char Res1 : 1;
	unsigned char Res2 : 4;
	unsigned char NPI : 3;
	unsigned char Res3 : 1;
	unsigned char DigitsNum : 5;
	unsigned char CACDigitsNum : 3;
	//unsigned char packedDigits[];
} SiemensP99H;

//Additional Call Indicators
typedef struct
{
	unsigned char Phv;				//0x9A
	unsigned char Package_Len;
	unsigned char Res[4];
} SiemensP9AH;

//Add On Tariff
typedef struct
{
	unsigned char Phv;				//0x9B
	unsigned char Package_Len;
	unsigned char CtlIndictr;
	unsigned char Currency;
	//subPackages
} SiemensP9BH;

//Advised Cumulated Charges
typedef struct
{
	unsigned char Phv;				//0x97
	unsigned char Package_Len;
	unsigned char CtlFlag : 4;
	unsigned char AdviseType : 2;
	unsigned char Res1 : 2;
	unsigned char CurrencyOrPulses[3];
	unsigned char Multiplier;
} SiemensP9CH;

//Outgoing PA Identification
typedef struct
{
	unsigned char Phv;			//0x9D
	unsigned char Package_Len;
	unsigned char DigitsNum : 5;
	unsigned char LACDigitsNum : 3;
	//unsigned char packedDigits[];
} SiemensP9DH;

//Additional Calling Party Number
typedef struct
{
	unsigned char Phv;			//0x9F
	unsigned char Package_Len;
	unsigned char NADI : 7;
	unsigned char Res1 : 1;
	unsigned char Screening : 2;
	unsigned char Pres_Indictr : 2;
	unsigned char NPI : 3;
	unsigned char Res2 : 1;
	unsigned char DigitsNum : 5;
	unsigned char Res3 : 3;
} SiemensP9FH;

//Additional Numbers
typedef struct
{
	unsigned char Phv;			//0xA7
	unsigned char Package_Len;
	unsigned char StringsNum;
	unsigned char NumType : 4;
	unsigned char NumQualifier : 4;
	unsigned char NADI : 7;
	unsigned char Res1 : 1;
	unsigned char Res2 : 4;
	unsigned char NPI : 3;
	unsigned char Res3 : 1;
	unsigned char DigitsNum : 5;
	unsigned char Res4 : 3;
} SiemensPA7H;

//Called Party Number
typedef struct
{
	unsigned char Phv;				//0xA8
	unsigned char Package_Len;		//表示该Package的长度，包括所有字段
	unsigned char NADI : 7;			//
	unsigned char Res1 : 1;
	unsigned char Res2 : 4;
	unsigned char NPI : 3;
	unsigned char Res3 : 1;
	unsigned char DigitsNum : 5;
	unsigned char LAC_Len : 3;
	//unsigned char packedDigits[];
} SiemensPA8H;

//Redirecting Num
typedef struct
{
	unsigned char Phv;				//0xAA;
	unsigned char Package_Len;
	unsigned char NADI : 7;
	unsigned char Res1 : 1;
	unsigned char Res2 : 2;
	unsigned char Pres_Indictr : 2;
	unsigned char NPI : 3;
	unsigned char Res3 : 1;
	unsigned char DigitsNum : 5;
	unsigned char LACLen : 3;
	//unsigned char packedDigits[];
} SiemensPAAH;

//Sent Called Party Number
typedef struct
{
	unsigned char Phv;				//0xAB
	unsigned char Package_Len;		//表示该Package的长度，包括所有字段
	unsigned char NADI : 7;			//
	unsigned char Res1 : 1;
	unsigned char Res2 : 4;
	unsigned char NPI : 3;
	unsigned char Res3 : 1;
	unsigned char DigitsNum : 5;
	unsigned char LAC_Len : 3;
	//unsigned char packedDigits[];
} SiemensPABH;

//System Provided Number
typedef struct
{
	unsigned char Phv;				//0xAC
	unsigned char Package_Len;
	unsigned char NumType : 4;
	unsigned char NumQulifier : 4;
	unsigned char NADI : 7;
	unsigned char Res1 : 1;
	unsigned char Res2 : 4;
	unsigned char NPI : 3;
	unsigned char Res3 : 1;
	unsigned char DigitsNum : 5;
	unsigned char LACNum : 3;
	//unsigned char packedDigits[];
} SiemensPACH;

//ITU Calling Party Category
typedef struct
{
	unsigned char Phv;
	unsigned char Package_Len;
	unsigned char ITU_Calling_Party_Category;
} SiemensPB0H;

//Project Specific Service 
typedef struct
{
	unsigned char Phv;
	unsigned char Package_Len;
	unsigned char TeleService;
	unsigned char BearerCapability;
} SiemensPB1H;

//Exchange Info
typedef struct
{
	unsigned char Phv;				//0xB2
	unsigned char Package_Len;
	unsigned char Va0 : 1;
	unsigned char Va1 : 1;
	unsigned char Res1 : 6;
	unsigned char VerID;
	unsigned ExchangeID_Len : 4;
	unsigned char Res2 : 4;
	//unsigned char ExchangeID[], max 11 bytes;
} SiemensPB2H;

//Service Subscriber Number
typedef struct
{
	unsigned char Phv;				//0xB3
	unsigned char Package_Len;
	unsigned char Qualifier : 4;
	unsigned char Res1 : 4;
	unsigned char NotApplicable;
	unsigned char DigitsNum : 6;
	unsigned char Coding : 2;
	//unsigned char packedDigits[]
} SiemensPB3H;

//Internet Session Info
typedef struct
{
	unsigned char Phv;
	unsigned char Pacakge_Len;
	//subPackages
} SiemensPB5H;

//Operator Connection Duration
typedef struct
{
	unsigned char Phv;				//0xB6
	unsigned char Package_Len;
	unsigned char Duration[2];
} SiemensPB6H;

//Sent CAC
typedef struct
{
	unsigned char Phv;				//0xB7
	unsigned char Package_Len;
	unsigned char CACDigitsNum : 3;
	unsigned char Res : 5;
	//unsigned char packedDigits[];
} SiemensPB7H;

//MLPP Level
typedef struct
{
	unsigned char Phv;				//0xB8
	unsigned char Package_Len;
	unsigned char PrecedenceLevel : 4;
	unsigned char Res1 : 1;
	unsigned char LFB : 2;
	unsigned char Res2 : 1;
	unsigned char NetworkID[2];
	unsigned char ServiceDomain[3];
} SiemensPB8H;

//IN Service Key
typedef struct
{
	unsigned char Phv;				//0xBA
	unsigned char Package_Len;
	unsigned char Key[4];
} SiemensPBAH;

//Received CAC
typedef struct
{
	unsigned char Phv;
	unsigned char Package_Len;
	unsigned char CACDigitsNum : 3;
	unsigned char Res1 : 5;
	//unsigned char packedDigits[];
} SiemensPBBH;

//Signalling Point Code
typedef struct
{
	unsigned char Phv;
	unsigned char Package_Len;
	unsigned char NetworkIndictr : 2;
	unsigned char Res : 6;
	unsigned char SignallingPointCode[3];
} SiemensPBFH;

//Sent Calling Party Number
typedef struct
{
	unsigned char Phv;
	unsigned char Package_Len;
	unsigned char NADI : 7;
	unsigned char Res1 : 1;
	unsigned char Screening : 2;
	unsigned char PresIndictr : 2;
	unsigned char NPI : 3;
	unsigned char Res2 : 1;
	unsigned char DigitsNum : 5;
	unsigned char LACDigitsNum : 3;
	//unsigned char packedDigits[];
} SiemensPC0H;

//Sent Redirecting Number
typedef struct
{
	unsigned char Phv;
	unsigned char Package_Len;
	unsigned char NADI : 7;
	unsigned char Res1 : 1;
	unsigned char Res2 : 4;
	unsigned char NPI : 3;
	unsigned char Res3 : 1;
	unsigned char DigitsNum : 5;
	unsigned char LACDigitsNum : 3;
	//unsigned char packedDigits[];
} SiemensPC1H;

//Sent Calling Party Category
typedef struct
{
	unsigned char Phv;
	unsigned char Package_Len;
	unsigned char Category;
} SiemensPC2H;

//Original Calling Party Category
typedef struct
{
	unsigned char Phv;
	unsigned char Package_Len;
	unsigned char Category;
} SiemensPC3H;

//Trunk Info
typedef struct
{
	unsigned char Phv;
	unsigned char Package_Len;
	//subPackages
} SiemensPC4H;

//Durations in 100ms
typedef struct
{
	unsigned char Phv;
	unsigned char Package_Len;
	//subPackages
} SiemensPC6H;

//Received Charged Determination Information
typedef struct
{
	unsigned char Phv;
	unsigned char Package_Len;
	unsigned char CDI : 1;
	unsigned char Res : 7;
} SiemensPC7H;

//若不足的字节数是1个或者两个字节，就用0x80填充，他只有一个字节，结构如下:
typedef struct
{
   unsigned char Phv;
} SiemensP80H_Fill_Rec1;

//若不足的字节数超过两个字节，就用0x81类型的话单填充，结构如下:
typedef struct
{
   unsigned char Phv;			//0x81
   unsigned char Rec_Len[2];	//填充记录的长度，包括所有字段
   //unsigned char Filler[];		//填充字节，任意值
} SiemensP81H_Fill_Rec2;

//填充32个bytes
typedef struct
{
	unsigned char Phv;			//0x00
	unsigned char Rec_Value[31];
} SiemensP00H_Fill_Rec3;

//转换成文本格式的西门子话单结构
struct SiemensDestFmt
{
	char szTime[14];		//通话时间YYYYMMDDhhmmss
	char szCallerNum[15];	//主叫号码
	char szCalledNum[15];	//被叫号码
	char szDuration[6];		//通话时长
	char szPartialID[3];	//部分记录指示
	char szChargeFlag[3];	//计费免费指示
	char szSeFlag[3];		//可靠/不可靠时间指示
	char szTimeFlag[3];		//起始/终止时间指示
	char szInTrunk[6];		//入中继
	char szOutTrunk[6];		//出中继
	char szInCicPn[6];		//入CIC pcm
	char szInCicCn[6];		//入CIC计费号码
	char szOutCicPn[6];		//出CIC pcm
	char szOutCicCn[6];		//出CIC计费号码	

	//char szCcu[5];		//呼叫计费
	//char szFau[5];		//FAU计费
	//char szZone[5];		//计费区
	//char szCause[4];		//呼叫释放原因
	//char szLoc[3];		//位置指示

	char szReturn[1];
};


int ConvertAEWSDAMACDR(unsigned char*& pRec, SiemensDestFmt& CDR, int& nLeftBytes, int& nRecordLen);
int ValideEWSDAMACDR(SiemensDestFmt& DestCDR);

int ValidateEWSDDateTime(const char* pszDateTime);

int ProcessEWSDAMAFile(const char* pszSrcFile,
                       const char* pszDestFile,
                       const char* pszErrFile,
                       const int bFileLevelVerify,
                       const int bBillLevelVerify)
{

    FILE* fpIn;
    FILE* fpOut;
    FILE* fpErr;

    if ((fpIn = fopen(pszSrcFile, "rb")) == NULL)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "打开话单文件(%s)失败",
               pszSrcFile);

        WRITELOG_F("打开话单文件(%s)失败",
                   pszSrcFile);

        return ERR_FAIL;
    }

    if ((fpOut = fopen(pszDestFile, "wb")) == NULL)
    {
        fclose(fpIn);

        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "打开最终话单文件(%s)失败",
               pszDestFile);

        WRITELOG_F("打开最终话单文件(%s)失败",
                   pszDestFile);

        return ERR_FAIL;
    }

    if ((fpErr = fopen(pszErrFile, "wb")) == NULL)
    {
        fclose(fpIn);
        fclose(fpOut);
        FileDel(pszDestFile);

        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "打开错误话单文件(%s)失败",
               pszErrFile);

        WRITELOG_F("打开错误话单文件(%s)失败",
                   pszErrFile);

        return ERR_FAIL;
    }

    //进行文件级校验。
    long nSrcFileSize = FileSize(fpIn);

    if (bFileLevelVerify)
    {
        if (nSrcFileSize % SIEMENS_BLOCK_SIZE != 0)
        {
            fclose(fpIn);
            fclose(fpOut);
            fclose(fpErr);

            FileDel(pszDestFile);
            FileDel(pszErrFile);

            if (!FileCopy(pszSrcFile, pszErrFile))
            {
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                       "话单文件(%s)拷贝到错误文件(%s)失败",
                       pszSrcFile,
                       pszErrFile);

                WRITELOG_F("话单文件(%s)拷贝到错误文件(%s)失败",
                           pszSrcFile,
                           pszErrFile);
            }

            return ERR_FAIL;
        }
    }


    /*
     * 开始进行话单转换。
     */
    int nResult = ERR_SUCCESS;
    int nErrBillCount = 0;
    int nBillCount = 0;

	//计算一个文件中包含的Block数量
	long nBlock = nSrcFileSize / SIEMENS_BLOCK_SIZE;

    unsigned char Block[SIEMENS_BLOCK_SIZE];
	SiemensDestFmt CDR;

	while (nBlock)
	{
		fread(&Block, sizeof(Block), 1, fpIn);
		unsigned char* pB = &Block[0];
		
		int nLeftBytes = SIEMENS_BLOCK_SIZE;

		while (nLeftBytes > 0)
		{
			switch (pB[0])
			{
				case 0x84:
				{
                    int nRecordLen = 0;
					int nResult = ConvertAEWSDAMACDR(pB, CDR, nLeftBytes, nRecordLen);

                    if (nResult != ERR_SUCCESS)
                    {
                        fwrite(pB - nRecordLen, nRecordLen, 1, fpErr);
                        nErrBillCount++;
                    }
                    else
                    {
                        if (bBillLevelVerify)
                        {
                            //进行话单级校验。
                            nResult = ValideEWSDAMACDR(CDR);
                            if (nResult != ERR_SUCCESS)
                            {
                                fwrite(pB - nRecordLen, nRecordLen, 1, fpErr);
                                nErrBillCount++;
                            }
                            else
                            {
						        fwrite(&CDR, sizeof(CDR), 1, fpOut);
                            }
                        }
                        else
                        {
						    fwrite(&CDR, sizeof(CDR), 1, fpOut);
                        }
					}

                    nBillCount++;

					break;
				}
				
				//填充字节
				case 0x80:
				{
					pB		   += 1;
					nLeftBytes -= 1;

					break;
				}

				//填充字节
				case 0x81:
				{
					SiemensP81H_Fill_Rec2 P81H;
					memcpy(&P81H, pB, sizeof(pB));

					int nPadding = P81H.Rec_Len[1];
					nPadding += (nPadding*256 + P81H.Rec_Len[0]);

					pB         += nPadding;
					nLeftBytes -= nPadding;

					break;
				}
				//填充32字节
				case 0x00:
				{
					pB += sizeof(SiemensP00H_Fill_Rec3);
					nLeftBytes -= sizeof(SiemensP00H_Fill_Rec3);

					break;
				}
				default :
				{
					pB += nLeftBytes - 1;
					nLeftBytes = 0;

					break;
				}
			}
		}

		nBlock--;
	}

    fclose(fpIn);
    fclose(fpOut);
    fclose(fpErr);

    /*
     * 对转换结果进行后续处理。
     */
    if (nErrBillCount == 0)
    {
        FileDel(pszErrFile);
        nResult = ERR_SUCCESS;
    }
    else if (nErrBillCount == nBillCount)
    {
        FileDel(pszDestFile);
        nResult = ERR_FAIL;
    }
    else
    {
        nResult = COL_CONVERT_PART_SUCC;
    }


    return nResult;
}

int ConvertAEWSDAMACDR(unsigned char*& pB, SiemensDestFmt& CDR, int& nLeftBytes, int& nRecordLen)
{
	memset(&CDR, ' ', sizeof(CDR));
	CDR.szReturn[0] = '\n';

	if (0 == nLeftBytes)
	{
		return ERR_FAIL;
	}

	//计算一条记录的长度
	short nRecLen = pB[2] & 0x0F;
	nRecLen = nRecLen * 256 + pB[1];

	if (0 == nRecLen)
	{
        nRecordLen = 0;
		return ERR_FAIL;
	}

	if (nLeftBytes < nRecLen)
	{
        nRecordLen = nLeftBytes;
		return ERR_FAIL;
	}

    nRecordLen = nRecLen;
	nLeftBytes -= nRecLen;

	char szTemp[128];

	//记录头
	if (0x84 !=  pB[0])	
	{
		printf("Error CDR!\n");
		return ERR_FAIL;
	}
		
	SiemensP84H_Rec_Owner_DN P84H_Rec_Owner_DN;
	memcpy(&P84H_Rec_Owner_DN, pB, sizeof(P84H_Rec_Owner_DN));

	//部分记录指示
	sprintf(szTemp, "%d", P84H_Rec_Owner_DN.PartialID);
	memcpy(CDR.szPartialID, szTemp, strlen(szTemp));
			
	//计费免费指示
	sprintf(szTemp, "%d", P84H_Rec_Owner_DN.ChargeFlag);
	memcpy(CDR.szChargeFlag, szTemp, strlen(szTemp));

	if (0 == P84H_Rec_Owner_DN.F18)
	{
		//主叫号码
		int nCallerLen = P84H_Rec_Owner_DN.Caller_Len;
		pB += sizeof(P84H_Rec_Owner_DN);
		nRecLen -= sizeof(P84H_Rec_Owner_DN);
		
		if (0 != nCallerLen)
		{
			BCD2String((char*)pB, szTemp, nCallerLen);
			if (0 != strlen(szTemp))
			{
				memcpy(CDR.szCallerNum, szTemp, strlen(szTemp));
			}
		}

		pB += (nCallerLen + 1)/2;
		nRecLen -= (nCallerLen + 1)/2;	
	}
	else if (1 == P84H_Rec_Owner_DN.F18)
	{
		pB += sizeof(P84H_Rec_Owner_DN);
		nRecLen -= sizeof(P84H_Rec_Owner_DN);
	}

	while (nRecLen > 0)
	{
		switch (pB[0])
		{
			//通话时间结构
			case 0x64:
			{
				SiemensP64H P64H;
				memcpy(&P64H, pB, sizeof(P64H));
				
				//转换通话时间
				sprintf(szTemp, "%4d%02d%02d%02d%02d%02d", 
					    P64H.Year+2000, P64H.Month, P64H.Day,
						P64H.Hour, P64H.Minute, P64H.Second);
				memcpy(CDR.szTime, szTemp, strlen(szTemp));

				sprintf(szTemp, "%d", P64H.Se_Flag);
				memcpy(CDR.szSeFlag, szTemp, strlen(szTemp));

				sprintf(szTemp, "%d", P64H.Time_Flag);
				memcpy(CDR.szTimeFlag, szTemp, strlen(szTemp));

				//通话时长
				int n1 = P64H.Duration[2];
				int n2 = P64H.Duration[1];
				int n3 = P64H.Duration[0];
				int nDuration = n1 * 256 * 256 + n2 * 256 + n3;
				sprintf(szTemp, "%d", nDuration);
				memcpy(CDR.szDuration, szTemp, strlen(szTemp));

				pB += sizeof(P64H);
				nRecLen -= sizeof(P64H);

				break;	
			}

			//被叫号码结构
			case 0x65:
			{
				SiemensP65H P65H;
				memcpy(&P65H, pB, sizeof(P65H));
				int nBCD = P65H.Called_Len;
				pB += sizeof(P65H);  //指向被叫号码起始位置
				nRecLen -= sizeof(P65H);

				BCD2String((char*)pB, szTemp, nBCD);

				if (strlen(szTemp))
				{
					memcpy(CDR.szCalledNum, szTemp, strlen(szTemp));	
				}
				
				pB += (nBCD + 1)/2;
				nRecLen -= (nBCD + 1)/2;
			
				break;
			}

			//呼叫计费单元结构
			case 0x67:
			{
				SiemensP67H P67H;
				memcpy(&P67H, pB, sizeof(P67H));

				pB += sizeof(P67H);
				nRecLen -= sizeof(P67H);
			
				break;
			}

			//FAU计费单元
			case 0x68:
			{
				SiemensP68H P68H;
				memcpy(&P68H, pB, sizeof(P68H));

				pB += sizeof(P68H);
				nRecLen -= sizeof(P68H);
			
				break;
			}

			//入中继
			case 0x69: 
			{
				SiemensP69H P69H;
				memcpy(&P69H, pB, sizeof(P69H));

				int nTrunk = P69H.Trunk_No_In[1];
				nTrunk += nTrunk * 256 + P69H.Trunk_No_In[0];

				sprintf(szTemp, "%d", nTrunk);
				memcpy(CDR.szInTrunk, szTemp, strlen(szTemp));

				pB += sizeof(P69H);
				nRecLen -= sizeof(P69H);

				break;
			}

			//出中继
			case 0x6A:
			{
				SiemensP6AH P6AH;
				memcpy(&P6AH, pB, sizeof(P6AH));

				int nTrunk = P6AH.Trunk_No_Out[1];
				nTrunk += nTrunk * 256 + P6AH.Trunk_No_Out[0];

				sprintf(szTemp, "%d", nTrunk);
				memcpy(CDR.szOutTrunk, szTemp, strlen(szTemp));

				pB += sizeof(P6AH);
				nRecLen -= sizeof(P6AH);

				break;
			}

			//CIC入中继
			case 0x6B:
			{
				SiemensP6BH P6BH;
				memcpy(&P6BH, pB, sizeof(P6BH));

				int nCICpn = P6BH.CIC_Pn[1];
				nCICpn += (nCICpn*256 + P6BH.CIC_Pn[0]);
				sprintf(szTemp, "%d", nCICpn);
				memcpy(CDR.szInCicPn, szTemp, strlen(szTemp));
			
				int nCICcn = P6BH.CIC_Cn;
				sprintf(szTemp, "%d", nCICcn);
				memcpy(CDR.szInCicCn, szTemp, strlen(szTemp));

				pB += sizeof(P6BH);
				nRecLen -= sizeof(P6BH);

				break;
			}

			//CIC出中继
			case 0x6C:
			{
				SiemensP6CH P6CH;
				memcpy(&P6CH, pB, sizeof(P6CH));

				int nCICpn = P6CH.CIC_Pn[1];
				nCICpn += (nCICpn*256 + P6CH.CIC_Pn[0]);
				sprintf(szTemp, "%d", nCICpn);
				memcpy(CDR.szInCicPn, szTemp, strlen(szTemp));
			
				int nCICcn = P6CH.CIC_Cn;
				sprintf(szTemp, "%d", nCICcn);
				memcpy(CDR.szInCicCn, szTemp, strlen(szTemp));

				pB += sizeof(P6CH);
				nRecLen -= sizeof(P6CH);

				break;
			}

			//Connection Identifier Number
			case 0x6E:
			{
				//SiemensP6EH P6EH;
				
				pB += sizeof(SiemensP6EH);
				nRecLen -= sizeof(SiemensP6EH);
				
				break;
			}

			//Project Specific Data
			case 0x75:
			{
				SiemensP75H P75H;
				memcpy(&P75H, pB, sizeof(P75H));

				pB += P75H.Package_Len;
				nRecLen -= P75H.Package_Len;

				break;
			}

			//计费区域
			case 0x7A:
			{
				SiemensP7AH P7AH;
				memcpy(&P7AH, pB, sizeof(P7AH));

				pB += sizeof(P7AH);
				nRecLen -= sizeof(P7AH);

				break;
			}			

			//通话质量
			case 0x82:
			{
				SiemensP82H P82H;
				memcpy(&P82H, pB, sizeof(P82H));

				pB += P82H.Package_Len;
				nRecLen -= P82H.Package_Len;

				break;
			}

			//Duration Before Answer
			case 0x86:
			{
				SiemensP86H P86H;
				memcpy(&P86H, pB, sizeof(P86H));
				pB += sizeof(SiemensP86H);
				nRecLen -= sizeof(SiemensP86H);

				break;
			}

			//Chargeband
			case 0x87:
			{
				SiemensP87H P87H;
				memcpy(&P87H, pB, sizeof(P87H));

				pB += P87H.Package_Len;
				nRecLen -= P87H.Package_Len;

				break;
			}

			//UUS3 Counters
			case 0x88:
			{
				SiemensP88H P88H;
				memcpy(&P88H, pB, sizeof(P88H));

				pB += P88H.Package_Len;
				nRecLen -= P88H.Package_Len;

				break;
			}

			//B Meter Pulses
			case 0x8A:
			{
				SiemensP8AH P8AH;
				memcpy(&P8AH, pB, sizeof(P8AH));

				pB += P8AH.Package_Len;
				nRecLen -= P8AH.Package_Len;
				
				break;
			}

			//Release Indication
			case 0x8B:
			{
				SiemensP8BH P8BH;
				memcpy(&P8BH, pB, sizeof(P8BH));

				pB += P8BH.Package_Len;
				nRecLen -= P8BH.Package_Len;

				break;
			}

			//Incoming Route ID
			case 0x8C:
			{
				SiemensP8CH P8CH;
				memcpy(&P8CH, pB, sizeof(P8CH));

				pB += P8CH.Package_Len;
				nRecLen -= P8CH.Package_Len;

				break;
			}

			//Outgoing Route ID
			case 0x8D:
			{
				SiemensP8DH P8DH;
				memcpy(&P8DH, pB, sizeof(P8DH));

				pB += P8DH.Package_Len;
				nRecLen -= P8DH.Package_Len;

				break;
			}
			
			//主叫号码
			case 0x8E:
			{
				SiemensP8EH P8EH;
				memcpy(&P8EH, pB, sizeof(P8EH));
				int nCallerLen = P8EH.DigitNum;
				
				if (0 != nCallerLen)
				{
					BCD2String((char*)pB, szTemp, nCallerLen);
					if (0 != strlen(szTemp))
					{
						memcpy(CDR.szCallerNum, szTemp, strlen(szTemp));
					}
				}

				pB += P8EH.Package_Len;
				nRecLen -= P8EH.Package_Len;
				
				break;
			}

			//IP/SN Charge Data
			case 0x8F:
			{
				SiemensP8FH P8FH;
				memcpy(&P8FH, pB, sizeof(P8FH));

				pB += P8FH.Package_Len;
				nRecLen -= P8FH.Package_Len;

				break;
			}

			case 0x91:
			{
				SiemensP91H P91H;
				memcpy(&P91H, pB, sizeof(P91H));

				pB += P91H.Package_Len;
				nRecLen -= P91H.Package_Len;

				break;
			}

			//Third Party Number
			case 0x92:
			{
				SiemensP92H P92H;
				memcpy(&P92H, pB, sizeof(P92H));

				pB += P92H.Package_Len;
				nRecLen -= P92H.Package_Len;

				break;
			}

			//Tariff Info
			case 0x93:
			{
				SiemensP93H P93H;
				memcpy(&P93H, pB, sizeof(P93H));

				pB += P93H.Package_Len;
				nRecLen -= P93H.Package_Len;

				break;
			}

			//CAC New Destinatin
			case 0x94:
			{
				SiemensP94H P94H;
				memcpy(&P94H, pB, sizeof(P94H));

				pB += P94H.Package_Len;
				nRecLen -= P94H.Package_Len;

				break;
			}

			//Global Charge Reference
			case 0x95:
			{
				SimensP95H P95H;
				memcpy(&P95H, pB, sizeof(P95H));

				pB += P95H.Package_Len;
				nRecLen -= P95H.Package_Len;

				break;
			}

			//Audit Trail
			case 0x96:
			{
				SimensP96H P96H;
				memcpy(&P96H, pB, sizeof(P96H));

				pB += P96H.Package_Len;
				nRecLen -= P96H.Package_Len;

				break;
			}

			//Business Group ID
			case 0x97:
			{
				SimensP97H P97H;
				memcpy(&P97H, pB, sizeof(P97H));

				pB += P97H.Package_Len;
				nRecLen -= P97H.Package_Len;

				break;
			}
			
			//Centrex Numbers
			case 0x98:
			{
				SiemensP98H P98H;
				memcpy(&P98H, pB, sizeof(P98H));

				pB += P98H.Package_Len;
				nRecLen -= P98H.Package_Len;

				break;
			}

			//Received Called Party Number
			case 0x99:
			{
				SiemensP99H P99H;
				memcpy(&P99H, pB, sizeof(P99H));

				pB += P99H.Package_Len;
				nRecLen -= P99H.Package_Len;

				break;
			}

			//Additional Call Indicators
			case 0x9A:
			{
				SiemensP9AH P9AH;
				memcpy(&P9AH, pB, sizeof(P9AH));

				pB += P9AH.Package_Len;
				nRecLen -= P9AH.Package_Len;

				break;
			}
			
			//被叫号码
			case 0xA8:
			{
				SiemensPA8H PA8H;
				memcpy(&PA8H, pB, sizeof(PA8H));
				int nCalledLen = PA8H.DigitsNum;

				if (0 != nCalledLen)
				{
					char* pszCalledNumber = (char*)(pB + sizeof(PA8H));
					BCD2String(pszCalledNumber, szTemp, nCalledLen);
					if (0 != strlen(szTemp))
					{
						memcpy(CDR.szCalledNum, szTemp, strlen(szTemp));
					}
				}

				pB += PA8H.Package_Len;
				nRecLen -= PA8H.Package_Len;

				break;
			}

			//Add On Tariff
			case 0x9B:
			{
				SiemensP9BH P9BH;
				memcpy(&P9BH, pB, sizeof(P9BH));

				pB += P9BH.Package_Len;
				nRecLen -= P9BH.Package_Len;

				break;
			}

			//Advised Cumulated Charges
			case 0x9C:
			{
				SiemensP9CH P9CH;
				memcpy(&P9CH, pB, sizeof(P9CH));

				pB += P9CH.Package_Len;
				nRecLen -= P9CH.Package_Len;

				break;
			}

			//Outgoing PA Identification
			case 0x9D:
			{
				SiemensP9DH P9DH;
				memcpy(&P9DH, pB, sizeof(P9DH));

				pB += P9DH.Package_Len;
				nRecLen -= P9DH.Package_Len;

				break;
			}

			//Additional Calling Party Number
			case 0x9F:
			{
				SiemensP9FH P9FH;
				memcpy(&P9FH, pB, sizeof(P9FH));

				pB += P9FH.Package_Len;
				nRecLen -= P9FH.Package_Len;

				break;
			}

			//Additional Numbers
			case 0xA7:
			{
				SiemensPA7H PA7H;
				memcpy(&PA7H, pB, sizeof(PA7H));

				pB += PA7H.Package_Len;
				nRecLen -= PA7H.Package_Len;

				break;
			}

			//Redirecting Number
			case 0xAA:
			{
				SiemensPAAH PAAH;
				memcpy(&PAAH, pB, sizeof(PAAH));

				pB += PAAH.Package_Len;
				nRecLen -= PAAH.Package_Len;

				break;
			}

			//Sent Called Party Number
			case 0xAB:
			{
				SiemensPABH PABH;
				memcpy(&PABH, pB, sizeof(PABH));

				pB += PABH.Package_Len;
				nRecLen -= PABH.Package_Len;

				break;
			}
			
			//System Provided Number
			case 0xAC:
			{
				SiemensPACH PACH;
				memcpy(&PACH, pB, sizeof(PACH));

				pB += PACH.Package_Len;
				nRecLen -= PACH.Package_Len;

				break;
			}

			//ITU Calling Party Category
			case 0xB0:
			{
				SiemensPB0H PB0H;
				memcpy(&PB0H, pB, sizeof(PB0H));

				pB += PB0H.Package_Len;
				nRecLen -= PB0H.Package_Len;

				break;
			}

			//System Info
			case 0xB2:
			{
				SiemensPB2H PB2H;
				memcpy(&PB2H, pB, sizeof(PB2H));

				pB += PB2H.Package_Len;
				nRecLen -= PB2H.Package_Len;

				break;
			}

			//Service Subscriber Number
			case 0xB3:
			{
				SiemensPB3H PB3H;
				memcpy(&PB3H, pB, sizeof(PB3H));

				pB += PB3H.Package_Len;
				nRecLen -= PB3H.Package_Len;

				break;
			}

			//Operator Connection Duration
			case 0xB6:
			{
				SiemensPB6H PB6H;
				memcpy(&PB6H, pB, sizeof(PB6H));

				pB += PB6H.Package_Len;
				nRecLen -= PB6H.Package_Len;

				break;
			}

			//Sent CAC
			case 0xB7:
			{
				SiemensPB7H PB7H;
				memcpy(&PB7H, pB, sizeof(PB7H));

				pB += PB7H.Package_Len;
				nRecLen -= PB7H.Package_Len;

				break;
			}

			//MLPP Level
			case 0xB8:
			{
				SiemensPB8H PB8H;
				memcpy(&PB8H, pB, sizeof(PB8H));

				pB += PB8H.Package_Len;
				nRecLen -= PB8H.Package_Len;

				break;
			}

			//IN Service Key
			case 0xBA:
			{
				SiemensPBAH PBAH;
				memcpy(&PBAH, pB, sizeof(PBAH));

				pB += PBAH.Package_Len;
				nRecLen -= PBAH.Package_Len;

				break;
			}

			//Received CAC
			case 0xBB:
			{
				SiemensPBBH PBBH;
				memcpy(&PBBH, pB, sizeof(PBBH));

				pB += PBBH.Package_Len;
				nRecLen -= PBBH.Package_Len;

				break;
			}

			//Project Specific Service
			case 0xB1:
			{
				SiemensPB1H PB1H;
				memcpy(&PB1H, pB, sizeof(PB1H));
				
				pB += PB1H.Package_Len;
				nRecLen -= PB1H.Package_Len;

				break;
			}

			//Internet Session Info
			case 0xB5:
			{
				SiemensPB5H PB5H;
				memcpy(&PB5H, pB, sizeof(PB5H));

				pB += PB5H.Pacakge_Len;
				nRecLen -= PB5H.Pacakge_Len;

				break;
			}

			//Signalling Point Code
			case 0xBF:
			{
				SiemensPBFH PBFH;
				memcpy(&PBFH, pB, sizeof(PBFH));

				pB += PBFH.Package_Len;
				nRecLen -= PBFH.Package_Len;

				break;
			}
			
			//Sent Calling Party Number
			case 0xC0:
			{
				SiemensPC0H PC0H;
				memcpy(&PC0H, pB, sizeof(PC0H));

				pB += PC0H.Package_Len;
				nRecLen -= PC0H.Package_Len;

				break;
			}

			//Sent Redirecting Number
			case 0xC1:
			{
				SiemensPC1H PC1H;
				memcpy(&PC1H, pB, sizeof(PC1H));

				pB += PC1H.Package_Len;
				nRecLen -= PC1H.Package_Len;

				break;
			}

			//Sent Calling Party Category
			case 0xC2:
			{
				SiemensPC2H PC2H;
				memcpy(&PC2H, pB, sizeof(PC2H));

				pB += PC2H.Package_Len;
				nRecLen -= PC2H.Package_Len;

				break;
			}

			//Original Calling Party Category
			case 0xC3:
			{
				SiemensPC3H PC3H;
				memcpy(&PC3H, pB, sizeof(PC3H));

				pB += PC3H.Package_Len;
				nRecLen -= PC3H.Package_Len;

				break;
			}

			//Trunk Info
			case 0xC4:
			{
				SiemensPC4H PC4H;
				memcpy(&PC4H, pB, sizeof(PC4H));

				pB += PC4H.Package_Len;
				nRecLen -= PC4H.Package_Len;

				break;
			}

			//Durations in 100ms
			case 0xC6:
			{
				SiemensPC6H PC6H;
				memcpy(&PC6H, pB, sizeof(PC6H));

				pB += PC6H.Package_Len;
				nRecLen -= PC6H.Package_Len;

				break;
			}

			//Received Charged Determination Information
			case 0xC7:
			{
				SiemensPC7H PC7H;
				memcpy(&PC7H, pB, sizeof(PC7H));

				pB += PC7H.Package_Len;
				nRecLen -= PC7H.Package_Len;

				break;
			}
			
			default:
			{
				pB += nRecLen;
				nRecLen = 0; 

				break;
			}
		}
	}

	return ERR_SUCCESS;
}


int ValideEWSDAMACDR(SiemensDestFmt& DestCDR)
{
    int nRet = ERR_SUCCESS;

    //校验日期时间
    nRet = ValidateEWSDDateTime((char*)DestCDR.szTime);
    if ( nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    return nRet;

}

int ValidateEWSDDateTime(const char* pszDateTime)
{
    /*
     *校验日期。
     */
    char szYear[5];
    char szMonth[3];
    char szDay[3];

    memcpy(szMonth, pszDateTime, 4);
    szYear[4] = '\0';

    memcpy(szMonth, pszDateTime + 4, 2);
    szMonth[2] = '\0';

    memcpy(szDay, pszDateTime + 6, 2);
    szDay[2] = '\0';

    int nYear, nDay, nMonth;

    nYear = atoi(szYear);
    if ((nYear < 1950)||(nYear > 2049))
    {
        return ERR_FAIL;
    }

    nMonth = atoi(szMonth);
    if ((nMonth < 1)||(nMonth > 12))
    {
        return ERR_FAIL;
    }

    nDay = atoi(szDay);
    if ((nDay < 1)||(nDay > 31))
    {
        return ERR_FAIL;
    }

    /*
     *校验时间。
     */
    char szHour[3];
    char szMinute[3];
    char szSecond[3];

    memcpy(szHour, pszDateTime + 8, 2);
    szHour[2] = '\0';

    memcpy(szMinute, pszDateTime + 10, 2);
    szMinute[2] = '\0';

    memcpy(szSecond, pszDateTime + 12, 2);
    szSecond[2] = '\0';

    if ((strcmp(szHour, "00") < 0)||(strcmp(szHour, "23") > 0))
    {
        return ERR_FAIL;
    }

    if ((strcmp(szMinute, "00") < 0)||(strcmp(szMinute, "59") > 0))
    {
        return ERR_FAIL;
    }

    if ((strcmp(szSecond, "00") < 0)||(strcmp(szSecond, "59") > 0))
    {
        return ERR_FAIL;
    }

    return ERR_SUCCESS;

}
