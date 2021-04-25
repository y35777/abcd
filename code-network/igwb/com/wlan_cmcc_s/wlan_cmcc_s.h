#ifndef __WLAN_CMCC_S_H__
#define __WLAN_CMCC_S_H__

#define DEFAULT_PROV_CODE        ""
#define DEFAULT_HOME_CODE        "46000000"
#define DEFAULT_FILE_VERSION    "01"
#define DEFAULT_SAVE_FILE_TIME_SPAN 7200
#define DEFAULT_START_SEQ_NO       0
#define DEFAULT_SEQNO_WIDTH       3
#define DEFAULT_PROCESS_INTERVAL 30
#define BILL_MAX_CALCULATE_LEN   11
#define REMAIN_FILE_EXIST   1
#define REMAIN_FILE_NOT_EXIST 0
//TStatusInfo结构是存储到状态文件中的结构 
struct TStatusInfo
{
    char     szWrittenTempFileName[128];  //第一份临时文件的文件名
    UINT4    dwCurrentFileNo;                 //文件序列号；
    time_t ftLastFileCreateTime;            //最新一个文件的创建时间,YYYYMMDDHHMMSS
    TStatusInfo()
    {
        memset(this, 0, sizeof(*this));        
    }
};

struct TMemStatus
{
    char     szFilePrefix[128];//注册表种前缀的描述
    UINT4     unSerialNumLen;//序列号的长度
    TStatusInfo* pStatusInfo;
} ;

struct TWlanHeadRecord
{
    char sHeadFlag[2];//头记录标志 ，填写10
    char sProvCode[8];//填空
    char sReserve1[20];//空格
    char sHomeCode[10];//左对齐，右填空
    char sFileNo[4];//左对齐，右填空
    char sReserve2[20];//空格
    char sFileProduceTime[14];//YYYYMMDDHHMMSS
    char sFileVersion[2];//01
    char sReserve3[132];//填空
    char sDivLine[2];//回车换行
} ;

struct TWlanTailRecord
{
    char sTailFlag[2];//"90"
    char sHomeCode[10];//46000000，左对齐右填空
    char sReserve1[20];//空格
    char sProvCode[8];//填空
    char sFileNo[4];//文件名中的序号，左对齐右填空
    char sReserve2[20];//空格
    char sFileProduceTime[14];//YYYYMMDDHHMISS
    char sRecordCount[9];//右对齐，左补0
    char sTotalDuration[15];//单位：秒，右对齐，左填0
    char sTotalDataFlowUp[15];//单位：秒，右对齐，左填0
    char sTotalDataFlowDown[15];//单位：秒，右对齐，左填0
    char sTotalBasicFee[9];//单位：分，右对齐，左填0
    char sTotalInfoFee[9];//单位：分，右对齐，左填0
    char sReserve3[62];//空格
    char sDivLine[2];//回车换行
} ;

struct TWlanRecord
{
    char sRecordType[2];           // Rec_type
    char sOperType[2];             //Oper_type
    char sOperID[2];               //Oper_ID
    char sServedMSISDN[24];    // Charge_Dn, 即MSISDN，不足位F填充
    char sServedIMSI[15];          // IMSI，不足位F填充
    char sUserType[1];             // User_type
    char sHomeProv[4];
    char sRoamProv[4];
    char sAuthType[2];             // Auth_type
    char sStartTime[14];           // YYYYMMDDHHMISS,该张话单的计费开始时间
    char sStopTime[14];        // YYYYMMDDHHMISS,该张话单的计费停止时间
    char sDuration[6];             // 该张话单的时长
    char sDataFlowUp[10];          // 该张话单的上行数据流量
    char sDataFlowDn[10];          // 该张话单的下行数据流量
    char sHotSpotID[16];           // Hotspot_ID
    char sACAddress[16];               // AC_address
    char sASAddress[16];               // AS_address
    char sHomeCarrier[5];
    char sRoamCarrier[5];
    char sBasicFee[6];
    char sInfoFee[6];
    char sServiceID[8];
    char sISPID[6];
    char sCloseCause[2];           // Logout_Cause，中间话单的原因值为0xFE
    char sReserve1[16];
    char sDivLine[2];
} ;  


BOOL SaveStatusInfo();
BOOL CreateEmptyFiles();
BOOL CreateAEmptyFile(STRING strFileName, STRING strFileSuffix);
STRING GetSuffixFileName(int nSerialWidth, UINT4 dwFileNumber);

BOOL IsFileExist(STRING strFilePath);

BOOL IsNumeric(const char* pSrc, UINT4 nLen);
UINT4 GetFileNextSerialNo(UINT4 ulSaveFileTimeSpan, UINT4 ulStartSeqNo,time_t t = ACE_OS::time(NULL));
UINT4 GetFileSerialNo(UINT4 ulSaveFileTimeSpan, UINT4 ulStartSeqNo, time_t t = ACE_OS::time(NULL));
//由指定时间，前缀描述得到前缀名称
STRING GetPrefixFileName(char * szNamePrefix, time_t tCurTime = ACE_OS::time(NULL));
template<class T>
BOOL Add(T& nTotal, const char* sFieldValue, UINT4 dwFieldLen, char* szBuffer);
BOOL LoadStatusInfo(char *szConfigPath, UINT4 nApID);

extern "C" BOOL Init_WLAN(char* szConfigPath, BYTE nApID, UINT4 nChannelCount);
extern "C" BOOL Process_WLAN(char* szFileName, char*& szDestFileName, UINT4 nChannelNum);
extern "C" BOOL Terminate_WLAN();
extern "C" BOOL IsFileOutPut_WLAN(UINT4 nChannelID);
extern "C" void ResetRemainTicket_WLAN();
extern "C" void GetAEmptyFilePara(char *szFileName, char *szFileSuffix, UINT4 nChannelID);

#endif