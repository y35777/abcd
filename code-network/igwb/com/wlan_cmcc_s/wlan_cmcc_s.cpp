#include "../include/frame.h"
#include "../include/toolbox.h"
#include "wlan_cmcc_s.h"
#include "../include/export_dll.h"

#define min(a, b)  (((a) < (b)) ? (a) : (b)) 
char g_sProvCode[20];
char g_sHomeCode[20];
char g_sFileVersion[20];
UINT4 g_ulSaveFileTimeSpan;
BYTE g_nSeqNoWidth;
UINT4 g_ulStartSeqNo;
UINT4 g_dwMAXSeqNo;
UINT4  g_dwRemainTicket;
int  *g_pMarkRemainFile;

const UINT4 MAX_PROCESS_RECORDS = 10000;//����һ�δ�������¼��

//ָ��TStatusInfo ��ָ�룬ʵ������һ�����飬
//����ָ����ڴ������������ͨ������ͨ����״̬�����Ĵ�����Ϊ�����ڶ�״̬�ļ����ж�д
TStatusInfo* g_pStatusArray;

//ָ�� TMemStatus ��ָ�룬ʵ������һ�����飬
//����ָ����ڴ������������ͨ������ͨ���ĵ�ǰ״̬��
TMemStatus* g_pMemStatusArray;

int g_nChannelCount = 0; 

FILE *g_pfileStatus;

STRING g_strTmpFilePath;
STRING g_strBackDataBakPath;
STRING g_strSecondPath;
STRING g_strStatusFilePath;


extern "C" IGWB_Svc_Export BOOL PC_Init(char* szConfigPath, BYTE nApID, UINT4 nChannelCount)
{
    return Init_WLAN(szConfigPath, nApID, nChannelCount);
}
extern "C" IGWB_Svc_Export BOOL PC_Process(char* szFileName, char*& szDestFileName, UINT4 nChannelNum)
{
    return Process_WLAN(szFileName, szDestFileName, nChannelNum);
}
extern "C" IGWB_Svc_Export BOOL PC_Terminate()
{
    return Terminate_WLAN();
}
//added by lucy 2004-6-11 wlan backsave patch
extern "C" IGWB_Svc_Export BOOL PC_IsFileOutPut_WLAN(UINT4 nChannelID)
{
    return IsFileOutPut_WLAN(nChannelID);
}
extern "C" IGWB_Svc_Export void PC_ResetRemainTicket_WLAN()
{
    ResetRemainTicket_WLAN();
}
extern "C" IGWB_Svc_Export void PC_GetAEmptyFilePara(char *szFileName, char *szFileSuffix, UINT4 nChannelID)
{
    GetAEmptyFilePara(szFileName, szFileSuffix, nChannelID);
}
extern "C" IGWB_Svc_Export BOOL PC_CreateAEmptyFile(char *szFileName, char *szFileSuffix)
{
    return CreateAEmptyFile(szFileName, szFileSuffix);
}
//end

/*******************************************************************************
Function        : Init_WLAN
Description     : ��̬���ʼ������Ľӿ�ʵ�ֺ���
Input           : char* szConfigPath
: BYTE nApID
Output          : 
Return          : extern
Calls           : 
Called by       : 

  History         : 
  1 Date          : 2003-4-10
  Author        : Gan Zhiliang
  Modification  : Created file
*******************************************************************************/
extern "C" BOOL Init_WLAN(char* szConfigPath, BYTE nApID, UINT4 nChannelCount )
{
    
    
    g_nChannelCount = nChannelCount - 1;
    
    if(g_nChannelCount <= 0)
    {
        return FALSE;
    }
    
    /*�õ�״̬�ļ��е������Լ�ͨ����*/
    if(!LoadStatusInfo(szConfigPath, nApID))
    {
        if(g_pStatusArray)
        {
            delete []g_pStatusArray;
            g_pStatusArray = NULL;
        }
        if(g_pMemStatusArray)
        {
            delete []g_pMemStatusArray;
            g_pMemStatusArray = NULL;
        }
        return FALSE;
    }
    
    //�������ϴγ����쳣��ֹ�����Ŀ��ļ�
    if(!CreateEmptyFiles())
    {
        return FALSE;
    }
    
    time_t t = ACE_OS::time(NULL);
    tm *tNow = ACE_OS::localtime(&t);
    UINT4 dwSerialNo = GetFileNextSerialNo(g_ulSaveFileTimeSpan, g_ulStartSeqNo);
    
    if(dwSerialNo == g_dwMAXSeqNo)
    {
        g_dwRemainTicket = 24 * 3600 - tNow->tm_hour * 3600 - tNow->tm_min - tNow->tm_sec ;
        
    }
    else
    {
        g_dwRemainTicket = g_ulSaveFileTimeSpan * (dwSerialNo - g_ulStartSeqNo + 1) - tNow->tm_hour * 3600 - tNow->tm_min * 60 - tNow->tm_sec;
    }
    
    
    return TRUE;
}

/*******************************************************************************
  Function        : Process_WLAN
  Description     : ��������Ľӿں���
  Input           : char* szFileName
                  : char*& szDestFileName
                  : UINT4 nChannelNum
  Output          : 
  Return          : extern
  Calls           : 
  Called by       : 
  
  History         : 
  1 Date          : 2003-5-6
    Author        : Gan Zhiliang
    Modification  : Created file
*******************************************************************************/
extern "C" BOOL Process_WLAN(char* szFileName, char*& szDestFileName, UINT4 nChannelNum)
{    //ע�⴫�����Ĳ���ֻ�ܹ�nChannelNum�Ǵ�1��ʼ�ģ����磺ͨ��1,nChannelNum =1     
    //�ж�״̬�ļ��б�����ļ������Ƿ�͵�һ����ʱ�ļ���������ͬ�������˵���Ѿ�
    //д�����ʱ�ļ��У����򷵻�,һ����һ����ʱ�ļ���һ���ڶ�����ʱ�ļ����Լ���ʽ
    //�����ļ�����һһ��Ӧ�ģ���ͳ����Ϻ󣬸���״̬�ļ�
    
    STRING     strPrefix;
    STRING     strTempFileName;
    STRING     strSuffix;
    STRING     strFileName;
    
    if(nChannelNum < 1)
    {
        throw("nChannelNum  is illegal");
        return FALSE;
    }
    
    if(0 == strcmp(szFileName, g_pMemStatusArray[nChannelNum - 1].pStatusInfo->szWrittenTempFileName))
    {
        return TRUE;
    }
    
    //�õ�strPrefix = sz20030317    
    strTempFileName = szFileName;
    int nPos1 = strTempFileName.rfind('/');
    strTempFileName = strTempFileName.substr(nPos1 + 1, strTempFileName.length());
    int nPos2 = strTempFileName.rfind('.');
    strPrefix = strTempFileName.substr(0, nPos2);   
    
    
    //�򿪵�һ����ʱ�����ļ������ѵ�һ����ʱ�����ļ�������д���ڶ�����ʱ�����ļ���ȥ��
    FILE *pFirstTempFile = NULL;
    pFirstTempFile = fopen(szFileName , "rb");
    if (NULL == pFirstTempFile)
    {   
        fclose(pFirstTempFile);
        return FALSE;
    } 
    
    //�õ��ڶ�����ʱ�ļ�������
    //ȡ���ļ�����ǰ׺���������Ŀ����ļ�����
    char szFileSerialNo[10];
    UINT4 ulFileSerialNo = GetFileSerialNo(g_ulSaveFileTimeSpan, g_ulStartSeqNo);    
    sprintf(szFileSerialNo, "%03d", ulFileSerialNo);
    STRING strFilePath;
    strFilePath = g_strTmpFilePath;
    strFilePath += strPrefix;
    strFilePath += ".";
    strFilePath += szFileSerialNo;
    
    /*
    *  �����ڶ�����ʱ�ļ��ļ�����ͷβ��¼�Լ���������д���ļ�����Ϊͷ��¼Ԥ���ռ䡣
    */
    FILE *pTmpDestFile = NULL;
    pTmpDestFile = fopen(strFilePath.c_str(), "wb+");
    if(pTmpDestFile == NULL)
    {
        return FALSE;
    }
    
    //Ϊ�ڶ�����ʱ�ļ�Ԥ��ͷ��¼�Ŀռ�
    UINT4 ulDestFileOffset = sizeof(TWlanHeadRecord);
    //�Ѽ�¼д���ڶ�����ʱ�ļ���ȥ
    if(0 != fseek(pTmpDestFile, ulDestFileOffset, SEEK_SET))
    {
        fclose(pTmpDestFile);
        return FALSE;
    }
    
    
    
    /*
    * �ֶ�ΰ����������ļ������ڴ棬����ͳ��
    */
    UINT4  dwRecordLen = sizeof(TWlanRecord);
    if(fseek(pFirstTempFile, 0, SEEK_END) != 0) 
    {
        
        return FALSE;
    }
    UINT4 dwFileLen = ftell(pFirstTempFile);
    UINT4 dwRecordCount = dwFileLen / dwRecordLen; //��ü�¼��
    UINT4 dwProcessTimes = (dwRecordCount + MAX_PROCESS_RECORDS - 1)/ MAX_PROCESS_RECORDS;
    UINT4 dwLastProcessRecords = dwRecordCount % MAX_PROCESS_RECORDS;
    
    UINT4   nOffset = 0;
    INT8 nTotalDuration = 0, nTotalDataFlowUp = 0, nTotalDataFlowDown = 0;
    UINT4   nTotalBasicFee = 0, nTotalInfoFee = 0;
    TWlanRecord* pAWlanRecord = NULL;
    UINT4 dwFieldLen = 0;
    char szBuffer[BILL_MAX_CALCULATE_LEN];
    UINT4 ulFileStartPosition = 0;
    char* pBillFile = new char[dwRecordLen * MAX_PROCESS_RECORDS];  
    
    if ( pBillFile == NULL )
    {
        return FALSE;
    }
    UINT4 dwBufferLen;
    for(UINT4 dwProcess = 0;  dwProcess < dwProcessTimes; dwProcess++)
    {
        try
        {
            if((dwLastProcessRecords != 0 ) && (dwProcess == (dwProcessTimes -1)))//���һ��
            {
                dwBufferLen = dwRecordLen * dwLastProcessRecords;                   
            }
            else
            {
                dwBufferLen = dwRecordLen * MAX_PROCESS_RECORDS;
            }
            
            if(0 != fseek(pFirstTempFile, ulFileStartPosition, SEEK_SET))
            {
                fclose(pFirstTempFile);
                return FALSE;
            }
            
            UINT4 tmpLen = fread(pBillFile, 1, dwBufferLen,pFirstTempFile); 
            
            //û�ж���
            if( tmpLen != dwBufferLen )  
            {   
                delete []pBillFile;
                pBillFile = NULL;
                fclose(pFirstTempFile);             
                return FALSE;
            }
            
            //�ѻ�������д���ڶ�����ʱ�ļ���ȥ
            tmpLen  = fwrite(pBillFile, 1, dwBufferLen,pTmpDestFile); 
            
            //û��д��
            if( tmpLen != dwBufferLen )  
            {   
                delete []pBillFile;
                pBillFile = NULL;
                fclose(pTmpDestFile);             
                return FALSE;
            }
            
            
            ulFileStartPosition += dwBufferLen;
            
        }
        catch(const char *e)
        {
            e++;//Ϊ������"����δ����"�ĸ澯.
            delete []pBillFile;
            pBillFile = NULL;
            
            return FALSE;
        }
        
        
        /*
        * ͳ��ͨ��ʱ��������������������������������ͨ�ŷѡ���Ϣ�ѡ�
        */
        
        nOffset = 0;
        UINT4 dwRecordStatsCout = dwBufferLen / dwRecordLen;
        for (UINT4 i = 0; i < dwRecordStatsCout; i++)
        {
            pAWlanRecord = (TWlanRecord*)(pBillFile + nOffset);
            
            //ͳ��ͨ��ʱ����
            dwFieldLen = sizeof(pAWlanRecord->sDuration);
            if (!(Add(nTotalDuration, pAWlanRecord->sDuration, dwFieldLen, szBuffer)))
            {
                throw("Bill file: %s, record: %d, Field: %s is wrong", szFileName, i, "Duration");
            }
            
            //ͳ����������������
            dwFieldLen = sizeof(pAWlanRecord->sDataFlowUp);
            if (!(Add(nTotalDataFlowUp, pAWlanRecord->sDataFlowUp, dwFieldLen, szBuffer)))
            {
                throw("Bill file: %s, record: %d, Field: %s is wrong", szFileName, i, "DataFlowUp");
            }
            
            //ͳ����������������
            dwFieldLen = sizeof(pAWlanRecord->sDataFlowDn);
            if (!(Add(nTotalDataFlowDown, pAWlanRecord->sDataFlowDn, dwFieldLen, szBuffer)))
            {
                throw("Bill file: %s, record: %d, Field: %s is wrong", szFileName, i, "DataFlowDn");
            }
            
            //ͳ��ͨ�ŷѡ�
            dwFieldLen = sizeof(pAWlanRecord->sBasicFee);
            if (!(Add(nTotalBasicFee, pAWlanRecord->sBasicFee, dwFieldLen, szBuffer)))
            {
                throw("Bill file: %s, record: %d, Field: %s is wrong", szFileName, i, "BasicFee");
            }
            
            //ͳ����Ϣ�ѡ�
            dwFieldLen = sizeof(pAWlanRecord->sInfoFee);
            if (!(Add(nTotalInfoFee, pAWlanRecord->sInfoFee, dwFieldLen, szBuffer)))
            {
                throw("Bill file: %s, record: %d, Field: %s is wrong", szFileName, i, "sInfoFee");
            }
            
            nOffset += dwRecordLen;
        }
        ACE_OS::sleep(0);
        
    }//����ͳ�����
    
     /*
     * ���ͷ��¼��β��¼��
     */
     TWlanHeadRecord headRecord;
     TWlanTailRecord tailRecord;
     memset(&headRecord, ' ', sizeof(TWlanHeadRecord));
     memset(&tailRecord, ' ', sizeof(TWlanTailRecord));
     
     /*
     * ���ͷ��¼��
     */
     
     //���ͷ��¼��ǡ�
     strncpy(headRecord.sHeadFlag, "10", sizeof(headRecord.sHeadFlag));
     
     //�ϴ��ļ���ʡ���롣
     strncpy(headRecord.sProvCode, g_sProvCode, sizeof(headRecord.sProvCode));
     
     //�������ִ��롣
     strncpy(headRecord.sHomeCode, g_sHomeCode, sizeof(headRecord.sHomeCode));
     
     //����ļ����кš�
    memcpy(headRecord.sFileNo, szFileSerialNo , 3);
     
     //����ļ���������,�ļ���������ʼ���ǵ�ǰ���µ�ʱ��
     STRING  strFileProduceTime;
     time_t tNow = time(NULL);
     tm *tt = localtime(&tNow);
     char szTime[20];
     sprintf(szTime, "%04d%02d%02d%02d%02d%02d",tt->tm_year + 1900, tt->tm_mon + 1, tt->tm_mday, tt->tm_hour, tt->tm_min, tt->tm_sec);
     strFileProduceTime = szTime;       
     memcpy(headRecord.sFileProduceTime, strFileProduceTime.c_str(), sizeof(headRecord.sFileProduceTime));
     
     //����ļ��汾�š�
     strncpy(headRecord.sFileVersion, g_sFileVersion, sizeof(headRecord.sFileVersion));
     
     //���س��ͻ��С�
     strncpy(headRecord.sDivLine, "\r\n", sizeof(headRecord.sDivLine));
     
     
     /*
     * ���β��¼��
     */
     char szCalculateField[1024];
     memset(szCalculateField, ' ', 1024);
     
     //���β��¼��ǡ�
     strncpy(tailRecord.sTailFlag, "90", sizeof(tailRecord.sTailFlag));
     
     
     //�������ִ��룬��ͷ��¼�е���Ӧ�ֶ���䡣
     memcpy(tailRecord.sHomeCode, headRecord.sHomeCode, sizeof(headRecord.sHomeCode));
     
     //�ϴ��ļ���ʡ���룬��ͷ��¼�е���Ӧ�ֶ���䡣
     memcpy(tailRecord.sProvCode, headRecord.sProvCode , sizeof(headRecord.sProvCode));
     
     //����ļ����кţ���ͷ��¼�е���Ӧ�ֶ���䡣
     memcpy(tailRecord.sFileNo, headRecord.sFileNo , sizeof(headRecord.sFileNo));
     
     //����ļ��������ڣ���ͷ��¼�е���Ӧ�ֶ���䡣
     memcpy(tailRecord.sFileProduceTime, headRecord.sFileProduceTime, sizeof(headRecord.sFileProduceTime));
     
     //��仰��������
     sprintf(szCalculateField, "%09u", dwRecordCount);
     strncpy(tailRecord.sRecordCount, szCalculateField, sizeof(tailRecord.sRecordCount));
     
     #ifdef _PLATFORM_WIN32
     //�����ʱ���ϼơ�
     sprintf(szCalculateField, "%015I64u", nTotalDuration);
     strncpy(tailRecord.sTotalDuration, szCalculateField, sizeof(tailRecord.sTotalDuration));
     
     //����������������ϼơ�
     sprintf(szCalculateField, "%015I64u", nTotalDataFlowUp);
     strncpy(tailRecord.sTotalDataFlowUp, szCalculateField, sizeof(tailRecord.sTotalDataFlowUp));
     
     //����������������ϼơ�
     sprintf(szCalculateField, "%015I64u", nTotalDataFlowDown);
     strncpy(tailRecord.sTotalDataFlowDown, szCalculateField, sizeof(tailRecord.sTotalDataFlowDown));
     #else
	 //�����ʱ���ϼơ�
     sprintf(szCalculateField, "%015Illu", nTotalDuration);
     strncpy(tailRecord.sTotalDuration, szCalculateField, sizeof(tailRecord.sTotalDuration));
     
     //����������������ϼơ�
     sprintf(szCalculateField, "%015Illu", nTotalDataFlowUp);
     strncpy(tailRecord.sTotalDataFlowUp, szCalculateField, sizeof(tailRecord.sTotalDataFlowUp));
     
     //����������������ϼơ�
     sprintf(szCalculateField, "%015Illu", nTotalDataFlowDown);
     strncpy(tailRecord.sTotalDataFlowDown, szCalculateField, sizeof(tailRecord.sTotalDataFlowDown));
     #endif

     //���ͨ�ŷѺϼơ�
     sprintf(szCalculateField, "%09u", nTotalBasicFee);
     strncpy(tailRecord.sTotalBasicFee, szCalculateField, sizeof(tailRecord.sTotalBasicFee));
     
     //�����Ϣ�Ѻϼơ�
     sprintf(szCalculateField, "%09u", nTotalInfoFee);
     strncpy(tailRecord.sTotalInfoFee, szCalculateField, sizeof(tailRecord.sTotalInfoFee));
     
     //���س��ͻ��С�
     strncpy(tailRecord.sDivLine, "\r\n", sizeof(tailRecord.sDivLine));
     
     
     try
     {
         //��ָ���ƶ����ڶ�����ʱ�ļ���ͷ����д��ͷ��¼
         if(0 != fseek(pTmpDestFile, 0, SEEK_SET))
         {
             fclose(pTmpDestFile);
             return FALSE;
         }
         fwrite(&headRecord, 1, sizeof(headRecord), pTmpDestFile);
         ulDestFileOffset += dwFileLen;
         
         //��ָ���ƶ����ڶ�����ʱ�ļ���β����д��β��¼
         if(0 != fseek(pTmpDestFile, ulDestFileOffset, SEEK_SET))
         {
             fclose(pTmpDestFile);
             return FALSE;
         }
         
         fwrite(&tailRecord, 1, sizeof(tailRecord), pTmpDestFile);
         
         delete []pBillFile;
         pBillFile = NULL;
         fflush(pTmpDestFile);       
         //ɾ����ʱ�ļ�
         fclose(pFirstTempFile);
         
     }
     catch(const char * e)
     {
         e++; //�˾���������������������δ���á��ĸ澯
         //tmpDestFile.Close(); 
         fclose(pTmpDestFile);
         return FALSE;
     }
     
     //tmpDestFile.Close();
     fclose(pTmpDestFile);
     
     /*
     * ���»����ļ��ƶ�����ʽĿ¼�������ļ�����¼�������ļ��С�������״̬�ļ��е��ļ���š�
     */
     STRING strDstFilePath = szDestFileName;
     nPos1 = strDstFilePath.rfind('/');      
     strDstFilePath = strDstFilePath.substr(0, nPos1 + 1);
     //˵���� �������к�Ϊ���ֵ�Ļ����ļ����ڵڶ�����賿����Ժ�����ģ����Ե�
	 //���к�Ϊ���ֵ��ʱ��ǰ׺�е�ʱ������ȡֵӦ����ǰһ���ʱ��ȡֵ��
	 time_t tPrefix = tNow;
	 if(ulFileSerialNo == g_dwMAXSeqNo)
	 {
		 tPrefix = tNow - 24 * 3600;
	 }
	 strPrefix = GetPrefixFileName(g_pMemStatusArray[nChannelNum - 1].szFilePrefix, tPrefix);
     strDstFilePath += strPrefix;    
     strDstFilePath += ".";
     strDstFilePath += szFileSerialNo;
     
     if(!FileMove(strFilePath.c_str(), strDstFilePath.c_str()))
     {
          throw("special move to back file error");
		  return FALSE;
     }
     g_pStatusArray[nChannelNum - 1].ftLastFileCreateTime = tNow;  
     g_pMemStatusArray[nChannelNum - 1].pStatusInfo->dwCurrentFileNo = ulFileSerialNo;
     strcpy(g_pStatusArray[nChannelNum - 1].szWrittenTempFileName, szFileName);    
     
     SaveStatusInfo();
     if(pBillFile)
     {
         delete []pBillFile;
         pBillFile = NULL;
     }
     
     
     
     return TRUE;
     
     
}


/*******************************************************************************
  Function        : Terminate_WLAN
  Description     : �������Ľӿ�ʵ�ֺ���
  Input           : void
  Output          : 
  Return          : extern
  Calls           : 
  Called by       : 
  
  History         : 
  1 Date          : 2003-5-6
    Author        : Gan Zhiliang
    Modification  : Created file
*******************************************************************************/
extern "C" BOOL Terminate_WLAN()
{
	
	if(g_pMarkRemainFile)
	{
		delete []g_pMarkRemainFile;
	}
	if(g_pStatusArray)
	{
		delete []g_pStatusArray;
	}
	if(g_pMemStatusArray)
	{
		delete []g_pMemStatusArray;
	}
	
	fclose(g_pfileStatus);   
	
	return TRUE;
	
	
}

/*******************************************************************************
Function        : LoadChannelStatusInfo
Description     : ��״̬�ļ��ж����¼��ͨ���ļ�����ʱ�䡢�ļ����кš�״̬�ļ���
Input           : void
Output          : 
Return          : BOOL
Calls           : 
Called by       : 

  History         : 
  1 Date          : 2003-3-15
  Author        : Gan Zhiliang
  Modification  : Created file
  ��ע��          Ϊ���ܹ���ʵ�ʿ����е����ù���ͬʱ�ﵽ����Ե�Ҫ�󣬺����
  �в��ö༶���õķ�ʽ�����ò��������ȼ��ӵ͵����ܹ���Ϊ
  ����/�����/ͨ��3����������߼���Ĳ���û�����ã�
  ��ȡ�ͼ�������ò���
*******************************************************************************/
BOOL LoadStatusInfo(char *szConfigPath, UINT4 nApID)
{
	
	
	STRING strKeyChannel;
	STRING strFilePrefix;
	UINT4 unSerialNumLen = 0;
	STRING strAPSelection;
	
	g_pStatusArray = new TStatusInfo[g_nChannelCount];
	g_pMemStatusArray = new TMemStatus[g_nChannelCount];
	
	//�������ļ�  
	CINIFile *cfgIniFile;
	cfgIniFile = new CINIFile(szConfigPath);
	if(cfgIniFile->Open() == FALSE)
	{
		delete cfgIniFile;
		return FALSE;
	}
	
	//��ȡ�����̶�Ӧ�Ľ�����
	char strSec[20];
	sprintf(strSec, "%s%d", CFG_SEC_AP, nApID);
	strAPSelection = strSec;
	
	//��ȡʡ����
	char szProvCode[20];
	cfgIniFile->GetString(strAPSelection.c_str(), "ProvCode", DEFAULT_PROV_CODE, szProvCode, 20);
	strcpy(g_sProvCode, szProvCode);
	
	//��ȡ�����ִ���
	char szHomeCode[20];
	cfgIniFile->GetString(strAPSelection.c_str(), "HomeCode", DEFAULT_HOME_CODE, szHomeCode, 20);
	strcpy(g_sHomeCode, szHomeCode);
	
	//�����ļ��汾����
	char szFileVer[20];
	cfgIniFile->GetString(strAPSelection.c_str(), "FileVersion", DEFAULT_FILE_VERSION, szFileVer, 20);
	strcpy(g_sFileVersion, szFileVer);
	
	
	//�����ļ�����ʱ�������������кſ�ȣ�������ʼ���к�
	g_ulSaveFileTimeSpan = cfgIniFile->GetInt(strAPSelection.c_str(), "SaveFileTimeSpan", DEFAULT_SAVE_FILE_TIME_SPAN);
	g_nSeqNoWidth = cfgIniFile->GetInt(strAPSelection.c_str(), "SeqNoWidth", DEFAULT_SEQNO_WIDTH);
	g_ulStartSeqNo = cfgIniFile->GetInt(strAPSelection.c_str(), "StartSeqNo", DEFAULT_START_SEQ_NO);       
	
	//�õ��������
	char szAPName[255];
	cfgIniFile->GetString(strAPSelection.c_str(), CFG_AP_NAME, "WLAN", szAPName, 255);
	
	//�õ���״̬�ļ��ĸ�Ŀ¼
	char szMainStatusRootDir[MAX_PATH];
	cfgIniFile->GetString(CFG_SEC_DISK_FILE, CFG_MAIN_STATUS_ROOT_DIR, 
		CFG_MAIN_STATUS_ROOT_DIR_DEFAULT, szMainStatusRootDir, MAX_PATH);
	g_strStatusFilePath = szMainStatusRootDir;
	g_strStatusFilePath = g_strStatusFilePath + "/" + szAPName;
	g_strStatusFilePath += "/BackSavePatch";
	CreateDeepDir(g_strStatusFilePath.c_str());
	g_strStatusFilePath += "/PatchInfo.dat";
	
	
	//�õ����ջ����ļ��ĸ�Ŀ¼
	char szMainBackSaveRootDir[MAX_PATH];
	cfgIniFile->GetString(CFG_SEC_DISK_FILE, CFG_BACK_SAVE_ROOT_DIR, 
		CFG_BACK_SAVE_ROOT_DIR_DEFAULT, 
		szMainBackSaveRootDir, MAX_PATH);
	
	//�õ���ʱ�ļ��ĸ�Ŀ¼
	g_strTmpFilePath = szMainBackSaveRootDir;
	g_strTmpFilePath = g_strTmpFilePath + "/" + szAPName;
	g_strTmpFilePath += "/temp/";
	
	//�õ��ڶ����ļ��ĸ�Ŀ¼
	g_strSecondPath = szMainBackSaveRootDir;
	g_strSecondPath += "/Second/";
	g_strSecondPath += szAPName;
	g_strSecondPath += "/";     
	
	g_pfileStatus = fopen(g_strStatusFilePath.c_str(), "rb+");
	if(NULL == g_pfileStatus)
	{
		g_pfileStatus = fopen(g_strStatusFilePath.c_str(), "wb+");
		if(NULL == g_pfileStatus)
		{
			return FALSE;
		}              
	}
	else
	{
		fclose(g_pfileStatus);
		g_pfileStatus = fopen(g_strStatusFilePath.c_str(), "rb+");
		if(NULL == g_pfileStatus)
		{
			return FALSE;
		}             
		
	}
	
	//ȡ��״̬�ļ���С
	if(fseek(g_pfileStatus, 0, SEEK_END) != 0) 
		return FALSE;
	
	UINT4 uFileLen = ftell(g_pfileStatus);
	
	//����õ��˲���ȷ��״̬�ļ������ļ�������Ϊ0��
	if (0 != uFileLen % sizeof(TStatusInfo))
	{
		int nResult = ACE_OS::truncate(g_strStatusFilePath.c_str(), 0);
		if (0 != nResult)
		{
			return FALSE;
		}
		
		fflush(g_pfileStatus);
	}
	
	//����ļ����Ȳ�Ϊ0����״̬�ļ��ж���״̬��Ϣ��
	if (0 !=  uFileLen)
	{
		UINT4 dwReadCount = min(uFileLen, g_nChannelCount * sizeof(TStatusInfo));
		fseek(g_pfileStatus, 0, SEEK_SET);
		//��״̬�ļ��ж���״̬��Ϣ
		fread(g_pStatusArray, 1, dwReadCount, g_pfileStatus);
		
	}
	else
	{
		//�������Ϊ������쳣��Ҫ�õ���ǰ�ļ������к���
		for(int j = 0; j < g_nChannelCount; j++)
		{
			g_pStatusArray[j].dwCurrentFileNo = GetFileSerialNo(g_ulSaveFileTimeSpan, g_ulStartSeqNo);
			
		}
		
	}
	
	
	char szChlSection[40];
	char szFormalPrefix[512];
	int nCsnLen = 0;
	//��״̬�ļ��б������Ϣ��ʼ�����ڴ���ȥ  
	for (int i = 0 ; i < g_nChannelCount; i++)
	{
		g_pMemStatusArray[i].pStatusInfo = g_pStatusArray + i;
		
		
		sprintf(szChlSection, CFG_SEC_CHANNEL, nApID, i + 1);
		
		
		//�������ļ��ж�ȡ��ʽ�ļ���ǰ׺�����ò����Ķ�ȡԭ�������ͷ�еı�ע
		cfgIniFile->GetString(CFG_SEC_DISK_FILE, CFG_PREFIX, CFG_PREFIX_DEFAULT,
			szFormalPrefix, 512);
		cfgIniFile->GetString(strAPSelection.c_str(), CFG_PREFIX, szFormalPrefix,
			szFormalPrefix, 512);
		cfgIniFile->GetString(szChlSection, CFG_PREFIX, szFormalPrefix,
			szFormalPrefix, 512);
		
		//�������ļ��ж�ȡ�ļ����кų��ȣ����ò����Ķ�ȡԭ�������ͷ�еı�ע
		nCsnLen = cfgIniFile->GetInt(CFG_SEC_DISK_FILE,
			CFG_CSN_LEN, CFG_CSN_LEN_DEFAULT);
		nCsnLen = cfgIniFile->GetInt(strAPSelection.c_str(), CFG_CSN_LEN, nCsnLen);
		nCsnLen = cfgIniFile->GetInt(szChlSection, CFG_CSN_LEN, nCsnLen);
		
		strcpy(g_pMemStatusArray[i].szFilePrefix, szFormalPrefix);
		g_pMemStatusArray[i].unSerialNumLen = nCsnLen;              
		
	}
	
	
	
	g_dwMAXSeqNo = 24 * 3600 / g_ulSaveFileTimeSpan - 1 + g_ulStartSeqNo;       
	
	g_pMarkRemainFile = new int[g_nChannelCount];
	if(g_pMarkRemainFile == NULL)
	{
		return FALSE;
	}
	
	cfgIniFile->Close();
	delete cfgIniFile;
	
	return TRUE;
	
}


/*******************************************************************************
Function        : SaveStatusInfo
Description     : ����״̬�ļ���
Input           : void
Output          : 
Return          : BOOL
Calls           : 
Called by       : 

  History         : 
  1 Date          : 2003-3-15
  Author        : Gan Zhiliang
  Modification  : Created file
*******************************************************************************/
  BOOL SaveStatusInfo()
  {
      UINT4 dwWriteCount = g_nChannelCount * sizeof(TStatusInfo);
      try
      {
          fseek(g_pfileStatus, 0, SEEK_SET);
          fwrite((void*)g_pStatusArray, 1, dwWriteCount, g_pfileStatus);
          fflush(g_pfileStatus);
          
          //���ԭ���ж��ͨ�������������ˣ���ô״̬�ļ����Ⱦ�Ӧ�ø���ʵ�ʱ仯
          int nResult = ACE_OS::truncate(g_strStatusFilePath.c_str(), dwWriteCount);
          if (0 != nResult)
          {
              return FALSE;
          }
          
      }
      catch(...)
      {
          return FALSE;
      }
      return TRUE;   
      
  }
  
  
/*******************************************************************************
  Function        : IsNumeric
  Description     : �ж�һ���ַ��ǲ�����ֵ�ַ�
  Input           : const char* pSrc
  : UINT4 nLen
  Output          : 
  Return          : BOOL
  Calls           : 
  Called by       : 
  
    History         : 
    1 Date          : 2003-3-18
    Author        : Gan Zhiliang
    Modification  : Created file
*******************************************************************************/
  BOOL IsNumeric(const char* pSrc, UINT4 nLen)
  {
      for (UINT4 i = 0; i < nLen; i++)
      {
          if ((pSrc[i] < '0')||(pSrc[i] > '9'))
          {
              return FALSE;
          }
      }
      
      return TRUE;
  }
  
template<class T>
      BOOL Add(T& nTotal, const char* sFieldValue, UINT4 dwFieldLen, char* szBuffer)
  {
      memcpy(szBuffer, sFieldValue, dwFieldLen);
      szBuffer[dwFieldLen] = '\0';
      
      if (!IsNumeric(szBuffer, dwFieldLen))
      {
          return FALSE;
      }
	  #ifdef _PLATFORM_WIN32
      nTotal += _atoi64(szBuffer);
      #else
	  nTotal += atoll(szBuffer);
      #endif
      return TRUE;
  }
  
  
/*******************************************************************************
  Function        : GetFileSerialNo
  Description     : ����ʼ���к�����ļ�����ʱ�����õ�ĳһ�ƶ�ʱ�����Ӧ�ñ���
  �ļ������к���
  ���磺 Ӧ���Ѿ�������ļ������к��룬���Ǳ����ļ���ʱ����2����1�֣����ʱ��õ���
  ���к���Ӧ���Ǳ������к�Ϊ0�Ļ����ļ���
  Input           : UINT4 ulSaveFileTimeSpan
  : UINT4 ulStartSeqNo
  : CTime t
  Output          : 
  Return          : UINT4
  Calls           : 
  Called by       : 
  
    History         : 
    1 Date          : 2003-3-13
    Author        : Gan Zhiliang
    Modification  : Created file
*******************************************************************************/
  UINT4 GetFileSerialNo(UINT4 ulSaveFileTimeSpan, UINT4 ulStartSeqNo,time_t t)
  {
      UINT4 ulSerialNo;
      tm *tt = ACE_OS::localtime(&t);
      
      if((tt->tm_hour * 3600 + tt->tm_min * 60 + tt->tm_sec) < ulSaveFileTimeSpan)
      {
          ulSerialNo =  24 * 3600 / ulSaveFileTimeSpan - 1 + ulStartSeqNo;
      }
      else
      {
          ulSerialNo = (tt->tm_hour * 3600 + tt->tm_min * 60 + tt->tm_sec)/ ulSaveFileTimeSpan - 1 + ulStartSeqNo;          
      }
      
      
      return ulSerialNo;
      
  }
  
/*******************************************************************************
  Function        : GetFileNextSerialNo
  Description     : ��һ��ʱ��Ӧ�ñ�����ļ������к���
  �ͣ�GetFileSerialNo��ͬ���ǣ����磺0��30�֣����õ�����ˮ������0��GetFileSerialNo
  �õ�����ˮ������11��������0��
  Input           : UINT4 ulSaveFileTimeSpan
  : UINT4 ulStartSeqNo
  : CTime t
  Output          : 
  Return          : UINT4
  Calls           : 
  Called by       : 
  
    History         : 
    1 Date          : 2003-3-22
    Author        : Gan Zhiliang
    Modification  : Created file
*******************************************************************************/
  UINT4 GetFileNextSerialNo(UINT4 ulSaveFileTimeSpan, UINT4 ulStartSeqNo,time_t t)
  {
      UINT4 ulSerialNo;
      tm *tt = ACE_OS::localtime(&t);
      if((tt->tm_hour * 3600 + tt->tm_min * 60 + tt->tm_sec) < ulSaveFileTimeSpan)
      {
          ulSerialNo = ulStartSeqNo;
      }
      else
      {
          ulSerialNo = (tt->tm_hour * 3600 + tt->tm_min * 60 + tt->tm_sec) / ulSaveFileTimeSpan  + ulStartSeqNo;          
      }
      
      
      return ulSerialNo;
      
  }
/*******************************************************************************
  Function        : CreateEmptyFiles
  Description     : �ڳ���������ʱ�򴴽����ϴ�崻�����������Ӧ�����ɵ��ļ�
  Input           : void
  Output          : 
  Return          : BOOL
  Calls           : 
  Called by       : 
  
    History         : 
    1 Date          : 2003-3-15
    Author        : Gan Zhiliang
    Modification  : Created file
*******************************************************************************/
  BOOL CreateEmptyFiles()
  {
      int i = 0;
      STRING strFileName;
      STRING strFilePrefix;
      STRING strFileSuffix;
      
      for(i = 0; i < g_nChannelCount; i++)
      {
          g_pMarkRemainFile[i] = REMAIN_FILE_NOT_EXIST;
          if(!(0 == g_pMemStatusArray[i].pStatusInfo->ftLastFileCreateTime))
          {
              
              time_t stimeNow = time(NULL);
              tm tmNow = *(localtime(&stimeNow)); 
			  UINT4 ulSerialNoNow,ulSerialNoLast;
              ulSerialNoNow = GetFileSerialNo(g_ulSaveFileTimeSpan, g_ulStartSeqNo, stimeNow);
              time_t stimeFile = g_pMemStatusArray[i].pStatusInfo->ftLastFileCreateTime;
              tm tmFile = *(localtime(&stimeFile));              
			  ulSerialNoLast = g_pStatusArray[i].dwCurrentFileNo;
              UINT4 ulSerialNo;
              
              if(stimeNow > stimeFile)
              {
              /*
              *���ʱ����ͬһ��
                  */
                  
                  if((tmNow.tm_year == tmFile.tm_year) 
                      && (tmNow.tm_yday == tmFile.tm_yday))
                      
                  {
                      if(ulSerialNoNow == ulSerialNoLast) 
                      {
                          continue;
                      }                     
                      
                      strFilePrefix = GetPrefixFileName( g_pMemStatusArray[i].szFilePrefix, stimeNow);
                      for( ulSerialNo = ulSerialNoLast + 1; ulSerialNo <= ulSerialNoNow ; ulSerialNo++ )
                      {
                          strFileSuffix = GetSuffixFileName(g_nSeqNoWidth, ulSerialNo);
                          char szFileName[10];
                          sprintf(szFileName, "chl%d/", i + 1);
                          strFileName = g_strSecondPath;
                          strFileName += szFileName;
                          strFileName = strFileName + strFilePrefix + "." + strFileSuffix;
                          if((ulSerialNo == ulSerialNoNow ) && (IsFileExist(g_strTmpFilePath)))
                          {
                              g_pMarkRemainFile[i] = REMAIN_FILE_EXIST;
                              
                          }
                          else
                          {
                              if(CreateAEmptyFile(strFileName, strFileSuffix))
                              {
                                  g_pStatusArray[i].dwCurrentFileNo = ulSerialNo;
                                  g_pStatusArray[i].ftLastFileCreateTime = time(NULL);
                                  ACE_OS::sleep(1);
                              }
                          }
                          
                      }
                      
                  }
                  else//���ʱ�䲻��ͬһ��
                  {     
                      strFilePrefix = GetPrefixFileName( g_pMemStatusArray[i].szFilePrefix, stimeFile);
                      UINT4 ulLastSerialNumOneDay = 24 * 3600/g_ulSaveFileTimeSpan - 1 + g_ulStartSeqNo;
                      for(ulSerialNo = ulSerialNoLast + 1; ulSerialNo <= ulLastSerialNumOneDay; ulSerialNo++)
                      {
                          strFileSuffix = GetSuffixFileName(g_nSeqNoWidth, ulSerialNo);
                          char szFileName[10];
                          sprintf(szFileName, "chl%d/", i + 1);
                          strFileName = g_strSecondPath;
                          strFileName += szFileName;
                          strFileName = strFileName + strFilePrefix + "." + strFileSuffix;
                          if(CreateAEmptyFile(strFileName, strFileSuffix))
                          {
                              g_pStatusArray[i].dwCurrentFileNo = ulSerialNo;
                              g_pStatusArray[i].ftLastFileCreateTime = time(NULL);
                              ACE_OS::sleep(1);
                              
                          }
                      }
                      
                      if(ulSerialNoNow < ulLastSerialNumOneDay)
                      {
                          strFilePrefix = GetPrefixFileName( g_pMemStatusArray[i].szFilePrefix, stimeNow);
                          for(ulSerialNo = g_ulStartSeqNo; ulSerialNo <= ulSerialNoNow; ulSerialNo++)
                          {
							  strFileSuffix = GetSuffixFileName(g_nSeqNoWidth, ulSerialNo);
							  char szFileName[20];
							  sprintf(szFileName, "chl%d/",i + 1);
							  strFileName = g_strSecondPath;
							  strFileName += szFileName;							  
							  strFileName = strFileName + strFilePrefix + "." + strFileSuffix;
							  if((ulSerialNo == ulSerialNoNow ) && (IsFileExist(g_strTmpFilePath)))
							  {
								  g_pMarkRemainFile[i] = REMAIN_FILE_EXIST;
								  
							  }
							  else
							  {
								  if(CreateAEmptyFile(strFileName, strFileSuffix))
								  {
									  g_pStatusArray[i].dwCurrentFileNo = ulSerialNo;
									  g_pStatusArray[i].ftLastFileCreateTime = time(NULL);
									  ACE_OS::sleep(1);                             
								  }
							  }
						  }
                      }
                      
                  }
              }
                           
              
          }
          
      }
      
      SaveStatusInfo();
      return TRUE;
      
  }
  
/*******************************************************************************
  Function        : CreateAEmptyFile
  Description     : ����ָ���ļ����ƵĽ������ļ�ͷ��¼��β��¼�Ŀ��ļ���
  ����ʱ��Ϊϵͳ��ǰʱ��
  Input           : STRING strFileName(�ļ�����ȫ·��), strFileSuffix����׺����Ҫ������¼�е����к��룩
  Output          : 
  Return          : BOOL
  Calls           : 
  Called by       : 
  
    History         : 
    1 Date          : 2003-3-17
    Author        : Gan Zhiliang
    Modification  : Created file
*******************************************************************************/
BOOL CreateAEmptyFile(STRING strFileName, STRING strFileSuffix)
{
    
    FILE *pFileEmpty = NULL;
    STRING strTempFileName;
    int nPos;
    strTempFileName = strFileName;
    nPos = strTempFileName.rfind('/');
    strTempFileName = strTempFileName.substr(nPos + 1, strTempFileName.length());
    strTempFileName = g_strTmpFilePath + strTempFileName;
    
    pFileEmpty = fopen(strTempFileName.c_str(), "ab+");
    if(pFileEmpty == NULL)
    {
        throw("Error open file %s ,line %d ",strTempFileName, __LINE__);
        return FALSE;
    }
    
    //����ջ����ļ���ͷ��¼��β��¼��ŵ�����
    TWlanHeadRecord headRecord;
    TWlanTailRecord tailRecord;
    
    memset(&headRecord, ' ', sizeof(TWlanHeadRecord));
    memset(&tailRecord, ' ', sizeof(TWlanTailRecord));
    
    
    /*
    * ���ͷ��¼��
    */
    
    //���ͷ��¼��ǡ�
    strncpy(headRecord.sHeadFlag, "10", sizeof(headRecord.sHeadFlag));
    
    //�ϴ��ļ���ʡ���롣
    strncpy(headRecord.sProvCode, g_sProvCode, sizeof(headRecord.sProvCode));
    
    //�������ִ��롣
    strncpy(headRecord.sHomeCode, g_sHomeCode, sizeof(headRecord.sHomeCode));
    
    //����ļ����кš�
    memcpy(headRecord.sFileNo, strFileSuffix.c_str() , g_nSeqNoWidth);
    
    time_t tNow = ACE_OS::time(NULL);
    tm *tt = ACE_OS::localtime(&tNow);
    char szTime[20];
    sprintf(szTime, "%04d%02d%02d%02d%02d%02d",tt->tm_year + 1900, tt->tm_mon + 1, tt->tm_mday, tt->tm_hour, tt->tm_min, tt->tm_sec);
    
    memcpy(headRecord.sFileProduceTime, szTime, sizeof(headRecord.sFileProduceTime));
    
    //����ļ��汾�š�
    strncpy(headRecord.sFileVersion, g_sFileVersion, sizeof(headRecord.sFileVersion));
    
    //���س��ͻ��С�
    strncpy(headRecord.sDivLine, "\r\n", sizeof(headRecord.sDivLine));
    
    
    /*
    * ���β��¼��
    */
    
    //���β��¼��ǡ�
    strncpy(tailRecord.sTailFlag, "90", sizeof(tailRecord.sTailFlag));
    
    
    //�������ִ��룬��ͷ��¼�е���Ӧ�ֶ���䡣
    memcpy(tailRecord.sHomeCode, headRecord.sHomeCode, sizeof(headRecord.sHomeCode));
    
    //�ϴ��ļ���ʡ���룬��ͷ��¼�е���Ӧ�ֶ���䡣
    memcpy(tailRecord.sProvCode, headRecord.sProvCode , sizeof(headRecord.sProvCode));
    
    //����ļ����кţ���ͷ��¼�е���Ӧ�ֶ���䡣
    memcpy(tailRecord.sFileNo, headRecord.sFileNo , sizeof(headRecord.sFileNo));
    
    //����ļ��������ڣ���ͷ��¼�е���Ӧ�ֶ���䡣
    memcpy(tailRecord.sFileProduceTime, headRecord.sFileProduceTime, sizeof(headRecord.sFileProduceTime));
    
    //��仰������,��ʱ���ϼ�,�������������ϼ�,�������������ϼ�,ͨ�ŷѺϼ�,��Ϣ�Ѻϼ�
    //��Ϊ���ڿջ�����Щ���ݵ�ֵ����Ϊ�ַ�'0'
    memset(tailRecord.sRecordCount, '0', offsetof(TWlanTailRecord, sReserve3) - offsetof(TWlanTailRecord, sRecordCount));
    
    //���س��ͻ��С�
    strncpy(tailRecord.sDivLine, "\r\n", sizeof(tailRecord.sDivLine));
    
    
    try
    {
        fwrite(&headRecord, 1, sizeof(TWlanHeadRecord), pFileEmpty);
        fwrite(&tailRecord, 1, sizeof(TWlanTailRecord), pFileEmpty);
        
        fclose(pFileEmpty);
        FileMove(strTempFileName.c_str(), strFileName.c_str());  
        
    }
    catch(const char * e)
    {
        e++;
        if(pFileEmpty)
        {
            fclose(pFileEmpty);
        }
        return FALSE;
    }
    
    return TRUE;    
}

/*******************************************************************************
Function        : GetSuffixFile
Description     : �����к��Լ����кŵĿ�ȵõ����кŵĸ�ʽ
Input           : int nSerialLen
: UINT4 dwFileNumber
Output          : 
Return          : STRING
Calls           : 
Called by       : 

  History         : 
  1 Date          : 2003-3-15
  Author        : Gan Zhiliang
  Modification  : Created file
*******************************************************************************/
STRING GetSuffixFileName(int nSerialWidth, UINT4 dwFileNumber)
{
    char szFormat[100], szFileCsn[100];
    SNPRINTF(szFormat, 100, "%%0%dd", nSerialWidth);
    szFormat[99] = 0;
    SNPRINTF(szFileCsn, 100, szFormat, dwFileNumber);
    szFileCsn[99] = 0;
    return szFileCsn;
    
}

/*******************************************************************************
Function        : GetPrefixFileName
Description     : ��ǰ׺�����ַ����õ��ļ���ǰ׺������ҪĿ�����������%��ǰ׺
Input           : STRING szNamePrefix
Output          : 
Return          : STRING
Calls           : 
Called by       : 

  History         : 
  1 Date          : 2003-3-15
  Author        : Gan Zhiliang
  Modification  : Created file
*******************************************************************************/
STRING GetPrefixFileName(char *szNamePrefix, time_t tCurTime)
{
    tm *tt = ACE_OS::localtime(&tCurTime);
    
    char szYear[10], szMonth[10], szDay[10];
    
    sprintf(szYear, "%04d", tt->tm_year + 1900);
    sprintf(szMonth, "%02d", tt->tm_mon + 1);
    sprintf(szDay, "%02d", tt->tm_mday);
    
    char szPrefix[512], szPrefix1[512], szPrefix2[512], szPrefix3[512], 
        szPrefix4[512], szPrefix5[512]; 
    ReplaceSubStr(szNamePrefix, szPrefix1, "%y", szYear);
    ReplaceSubStr(szPrefix1, szPrefix2, "%Y", szYear);
    ReplaceSubStr(szPrefix2, szPrefix3, "%m", szMonth);
    ReplaceSubStr(szPrefix3, szPrefix4, "%M", szMonth);
    ReplaceSubStr(szPrefix4, szPrefix5, "%d", szDay);
    ReplaceSubStr(szPrefix5, szPrefix, "%D", szDay);
    
    return szPrefix;
}



/*******************************************************************************
Function        : IsFileExist
Description     : �ú����ж���strFilePath·���´��ڲ����ں�׺��ΪstrFileSuffix ���ļ�
Input           : STRING strFilePath
Output          : 
Return          : BOOL
Calls           : 
Called by       : 

  History         : 
  1 Date          : 2003-3-22
  Author        : Gan Zhiliang
  Modification  : Created file
*******************************************************************************/
BOOL IsFileExist(STRING strFilePath)
{
    BOOL bResult = TRUE;
    FILE *pFile = NULL;
    pFile = fopen(strFilePath.c_str(), "rb+");
    if(NULL == pFile )
    {
        bResult = FALSE;
    }
    if(bResult)
    {
        fclose(pFile);
    }
    
    return bResult;     
    
}

/*******************************************************************************
Function        : IsFileOutPut_WLAN()
Description     : �������ж϶�ʱ��ʱ���Ƿ��Ѿ������˻����ļ������ʱ��
Input           : void
Output          : 
Return          : BOOL
Calls           : 
Called by       : 

  History         : 
  1 Date          : 2003-4-23
  Author        : Gan Zhiliang
  Modification  : Created file
  Comments        :�������ж�remailtickets�Ƿ񵽴�WLAN�ȴ���ʱ��
*******************************************************************************/
BOOL IsFileOutPut_WLAN(UINT4 nChannelID)
{
    if(g_dwRemainTicket > 1)
    {
        if(g_pMarkRemainFile[nChannelID - 1] == REMAIN_FILE_EXIST)
        {               
            g_pMarkRemainFile[nChannelID - 1] = REMAIN_FILE_NOT_EXIST;
            return TRUE;
        }
        
        g_dwRemainTicket--;         
        return  FALSE;
    }
    
    return TRUE;  
    
}


/*******************************************************************************
Function        : ResetRemainTicket_WLAN
Description     : �����趨��Ҫ�ȴ���tickets
Input           : void
Output          : 
Return          : void
Calls           : 
Called by       : 

History         : 
1 Date          : 2003-4-23
Author        : Gan Zhiliang
Modification  : Created file
*******************************************************************************/
void ResetRemainTicket_WLAN()
{
    time_t stimeNow = time(NULL);
    tm *tmNow = localtime(&stimeNow);  
    
    UINT4 dwSerialNo = GetFileNextSerialNo(g_ulSaveFileTimeSpan, g_ulStartSeqNo);
    
    
    if(dwSerialNo == g_dwMAXSeqNo)
    {
        g_dwRemainTicket = 24 * 3600 - tmNow->tm_hour * 3600 - tmNow->tm_min * 60 - tmNow->tm_sec;
        
    }
    else
    {
        g_dwRemainTicket = g_ulSaveFileTimeSpan * (dwSerialNo - g_ulStartSeqNo + 1) - tmNow->tm_hour * 3600 - tmNow->tm_min * 60 - tmNow->tm_sec;
    }
    
    return;
}

void GetAEmptyFilePara(char *szFileName, char *szFileSuffix, UINT4 nChannelID)
{
    STRING strFileName, strFileSuffix;
    UINT4 dwSerialNo =  GetFileSerialNo(g_ulSaveFileTimeSpan, g_ulStartSeqNo);
    time_t tNow = time(NULL);
    if(dwSerialNo == g_dwMAXSeqNo)
    {
        tNow = tNow - 24 * 3600;
    }
    STRING strPrefix = GetPrefixFileName(g_pMemStatusArray[nChannelID - 1].szFilePrefix, tNow);
    char szChannelNum[10];
    sprintf(szChannelNum, "chl%d",nChannelID);
    strFileName = g_strSecondPath;
    strFileName += szChannelNum;
    strFileName += "/";
    strFileName  += strPrefix;
    strFileName += ".";     
    strFileSuffix = GetSuffixFileName(g_nSeqNoWidth, dwSerialNo);
    strFileName += strFileSuffix; 
    strcpy(szFileName, strFileName.c_str());
    strcpy(szFileSuffix, strFileSuffix.c_str());
    return ;
    
}
