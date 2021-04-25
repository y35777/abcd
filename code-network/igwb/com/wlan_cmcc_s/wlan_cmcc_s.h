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
//TStatusInfo�ṹ�Ǵ洢��״̬�ļ��еĽṹ 
struct TStatusInfo
{
    char     szWrittenTempFileName[128];  //��һ����ʱ�ļ����ļ���
    UINT4    dwCurrentFileNo;                 //�ļ����кţ�
    time_t ftLastFileCreateTime;            //����һ���ļ��Ĵ���ʱ��,YYYYMMDDHHMMSS
    TStatusInfo()
    {
        memset(this, 0, sizeof(*this));        
    }
};

struct TMemStatus
{
    char     szFilePrefix[128];//ע�����ǰ׺������
    UINT4     unSerialNumLen;//���кŵĳ���
    TStatusInfo* pStatusInfo;
} ;

struct TWlanHeadRecord
{
    char sHeadFlag[2];//ͷ��¼��־ ����д10
    char sProvCode[8];//���
    char sReserve1[20];//�ո�
    char sHomeCode[10];//����룬�����
    char sFileNo[4];//����룬�����
    char sReserve2[20];//�ո�
    char sFileProduceTime[14];//YYYYMMDDHHMMSS
    char sFileVersion[2];//01
    char sReserve3[132];//���
    char sDivLine[2];//�س�����
} ;

struct TWlanTailRecord
{
    char sTailFlag[2];//"90"
    char sHomeCode[10];//46000000������������
    char sReserve1[20];//�ո�
    char sProvCode[8];//���
    char sFileNo[4];//�ļ����е���ţ�����������
    char sReserve2[20];//�ո�
    char sFileProduceTime[14];//YYYYMMDDHHMISS
    char sRecordCount[9];//�Ҷ��룬��0
    char sTotalDuration[15];//��λ���룬�Ҷ��룬����0
    char sTotalDataFlowUp[15];//��λ���룬�Ҷ��룬����0
    char sTotalDataFlowDown[15];//��λ���룬�Ҷ��룬����0
    char sTotalBasicFee[9];//��λ���֣��Ҷ��룬����0
    char sTotalInfoFee[9];//��λ���֣��Ҷ��룬����0
    char sReserve3[62];//�ո�
    char sDivLine[2];//�س�����
} ;

struct TWlanRecord
{
    char sRecordType[2];           // Rec_type
    char sOperType[2];             //Oper_type
    char sOperID[2];               //Oper_ID
    char sServedMSISDN[24];    // Charge_Dn, ��MSISDN������λF���
    char sServedIMSI[15];          // IMSI������λF���
    char sUserType[1];             // User_type
    char sHomeProv[4];
    char sRoamProv[4];
    char sAuthType[2];             // Auth_type
    char sStartTime[14];           // YYYYMMDDHHMISS,���Ż����ļƷѿ�ʼʱ��
    char sStopTime[14];        // YYYYMMDDHHMISS,���Ż����ļƷ�ֹͣʱ��
    char sDuration[6];             // ���Ż�����ʱ��
    char sDataFlowUp[10];          // ���Ż�����������������
    char sDataFlowDn[10];          // ���Ż�����������������
    char sHotSpotID[16];           // Hotspot_ID
    char sACAddress[16];               // AC_address
    char sASAddress[16];               // AS_address
    char sHomeCarrier[5];
    char sRoamCarrier[5];
    char sBasicFee[6];
    char sInfoFee[6];
    char sServiceID[8];
    char sISPID[6];
    char sCloseCause[2];           // Logout_Cause���м仰����ԭ��ֵΪ0xFE
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
//��ָ��ʱ�䣬ǰ׺�����õ�ǰ׺����
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