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

const UINT4 MAX_PROCESS_RECORDS = 10000;//定义一次处理话单记录数

//指向TStatusInfo 的指针，实际上是一个数组，
//它所指向的内存区域包含了总通道数个通道的状态，它的存在是为了易于对状态文件进行读写
TStatusInfo* g_pStatusArray;

//指向 TMemStatus 的指针，实际上是一个数组，
//它所指向的内存区域包含了总通道数个通道的当前状态。
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
Description     : 动态库初始化输出的接口实现函数
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
    
    /*得到状态文件中的内容以及通道的*/
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
    
    //创建自上次程序异常终止以来的空文件
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
  Description     : 处理输出的接口函数
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
{    //注意传进来的参数只能够nChannelNum是从1开始的，比如：通道1,nChannelNum =1     
    //判断状态文件中保存的文件名称是否和第一份临时文件的名称相同，如果是说明已经
    //写入过临时文件中，程序返回,一个第一份临时文件和一个第二份临时文件，以及正式
    //话单文件都是一一对应的，当统计完毕后，更新状态文件
    
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
    
    //得到strPrefix = sz20030317    
    strTempFileName = szFileName;
    int nPos1 = strTempFileName.rfind('/');
    strTempFileName = strTempFileName.substr(nPos1 + 1, strTempFileName.length());
    int nPos2 = strTempFileName.rfind('.');
    strPrefix = strTempFileName.substr(0, nPos2);   
    
    
    //打开第一份临时话单文件，并把第一份临时话单文件的内容写到第二份临时话单文件中去。
    FILE *pFirstTempFile = NULL;
    pFirstTempFile = fopen(szFileName , "rb");
    if (NULL == pFirstTempFile)
    {   
        fclose(pFirstTempFile);
        return FALSE;
    } 
    
    //得到第二份临时文件的名称
    //取得文件名的前缀，并计算出目标的文件名。
    char szFileSerialNo[10];
    UINT4 ulFileSerialNo = GetFileSerialNo(g_ulSaveFileTimeSpan, g_ulStartSeqNo);    
    sprintf(szFileSerialNo, "%03d", ulFileSerialNo);
    STRING strFilePath;
    strFilePath = g_strTmpFilePath;
    strFilePath += strPrefix;
    strFilePath += ".";
    strFilePath += szFileSerialNo;
    
    /*
    *  创建第二份临时文件文件，把头尾记录以及话单内容写入文件，先为头记录预留空间。
    */
    FILE *pTmpDestFile = NULL;
    pTmpDestFile = fopen(strFilePath.c_str(), "wb+");
    if(pTmpDestFile == NULL)
    {
        return FALSE;
    }
    
    //为第二份临时文件预留头记录的空间
    UINT4 ulDestFileOffset = sizeof(TWlanHeadRecord);
    //把记录写到第二份临时文件中去
    if(0 != fseek(pTmpDestFile, ulDestFileOffset, SEEK_SET))
    {
        fclose(pTmpDestFile);
        return FALSE;
    }
    
    
    
    /*
    * 分多次把整个话单文件读入内存，进行统计
    */
    UINT4  dwRecordLen = sizeof(TWlanRecord);
    if(fseek(pFirstTempFile, 0, SEEK_END) != 0) 
    {
        
        return FALSE;
    }
    UINT4 dwFileLen = ftell(pFirstTempFile);
    UINT4 dwRecordCount = dwFileLen / dwRecordLen; //获得记录数
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
            if((dwLastProcessRecords != 0 ) && (dwProcess == (dwProcessTimes -1)))//最后一次
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
            
            //没有读完
            if( tmpLen != dwBufferLen )  
            {   
                delete []pBillFile;
                pBillFile = NULL;
                fclose(pFirstTempFile);             
                return FALSE;
            }
            
            //把话单内容写到第二份临时文件中去
            tmpLen  = fwrite(pBillFile, 1, dwBufferLen,pTmpDestFile); 
            
            //没有写完
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
            e++;//为了消除"变量未引用"的告警.
            delete []pBillFile;
            pBillFile = NULL;
            
            return FALSE;
        }
        
        
        /*
        * 统计通话时长、上行数据流量、下行数据流量、通信费、信息费。
        */
        
        nOffset = 0;
        UINT4 dwRecordStatsCout = dwBufferLen / dwRecordLen;
        for (UINT4 i = 0; i < dwRecordStatsCout; i++)
        {
            pAWlanRecord = (TWlanRecord*)(pBillFile + nOffset);
            
            //统计通话时长。
            dwFieldLen = sizeof(pAWlanRecord->sDuration);
            if (!(Add(nTotalDuration, pAWlanRecord->sDuration, dwFieldLen, szBuffer)))
            {
                throw("Bill file: %s, record: %d, Field: %s is wrong", szFileName, i, "Duration");
            }
            
            //统计上行数据流量。
            dwFieldLen = sizeof(pAWlanRecord->sDataFlowUp);
            if (!(Add(nTotalDataFlowUp, pAWlanRecord->sDataFlowUp, dwFieldLen, szBuffer)))
            {
                throw("Bill file: %s, record: %d, Field: %s is wrong", szFileName, i, "DataFlowUp");
            }
            
            //统计下行数据流量。
            dwFieldLen = sizeof(pAWlanRecord->sDataFlowDn);
            if (!(Add(nTotalDataFlowDown, pAWlanRecord->sDataFlowDn, dwFieldLen, szBuffer)))
            {
                throw("Bill file: %s, record: %d, Field: %s is wrong", szFileName, i, "DataFlowDn");
            }
            
            //统计通信费。
            dwFieldLen = sizeof(pAWlanRecord->sBasicFee);
            if (!(Add(nTotalBasicFee, pAWlanRecord->sBasicFee, dwFieldLen, szBuffer)))
            {
                throw("Bill file: %s, record: %d, Field: %s is wrong", szFileName, i, "BasicFee");
            }
            
            //统计信息费。
            dwFieldLen = sizeof(pAWlanRecord->sInfoFee);
            if (!(Add(nTotalInfoFee, pAWlanRecord->sInfoFee, dwFieldLen, szBuffer)))
            {
                throw("Bill file: %s, record: %d, Field: %s is wrong", szFileName, i, "sInfoFee");
            }
            
            nOffset += dwRecordLen;
        }
        ACE_OS::sleep(0);
        
    }//话单统计完毕
    
     /*
     * 填充头记录和尾记录。
     */
     TWlanHeadRecord headRecord;
     TWlanTailRecord tailRecord;
     memset(&headRecord, ' ', sizeof(TWlanHeadRecord));
     memset(&tailRecord, ' ', sizeof(TWlanTailRecord));
     
     /*
     * 填充头记录。
     */
     
     //填充头记录标记。
     strncpy(headRecord.sHeadFlag, "10", sizeof(headRecord.sHeadFlag));
     
     //上传文件的省代码。
     strncpy(headRecord.sProvCode, g_sProvCode, sizeof(headRecord.sProvCode));
     
     //填充归属局代码。
     strncpy(headRecord.sHomeCode, g_sHomeCode, sizeof(headRecord.sHomeCode));
     
     //填充文件序列号。
    memcpy(headRecord.sFileNo, szFileSerialNo , 3);
     
     //填充文件产生日期,文件产生日期始终是当前最新的时间
     STRING  strFileProduceTime;
     time_t tNow = time(NULL);
     tm *tt = localtime(&tNow);
     char szTime[20];
     sprintf(szTime, "%04d%02d%02d%02d%02d%02d",tt->tm_year + 1900, tt->tm_mon + 1, tt->tm_mday, tt->tm_hour, tt->tm_min, tt->tm_sec);
     strFileProduceTime = szTime;       
     memcpy(headRecord.sFileProduceTime, strFileProduceTime.c_str(), sizeof(headRecord.sFileProduceTime));
     
     //填充文件版本号。
     strncpy(headRecord.sFileVersion, g_sFileVersion, sizeof(headRecord.sFileVersion));
     
     //填充回车和换行。
     strncpy(headRecord.sDivLine, "\r\n", sizeof(headRecord.sDivLine));
     
     
     /*
     * 填充尾记录。
     */
     char szCalculateField[1024];
     memset(szCalculateField, ' ', 1024);
     
     //填充尾记录标记。
     strncpy(tailRecord.sTailFlag, "90", sizeof(tailRecord.sTailFlag));
     
     
     //填充归属局代码，用头记录中的相应字段填充。
     memcpy(tailRecord.sHomeCode, headRecord.sHomeCode, sizeof(headRecord.sHomeCode));
     
     //上传文件的省代码，用头记录中的相应字段填充。
     memcpy(tailRecord.sProvCode, headRecord.sProvCode , sizeof(headRecord.sProvCode));
     
     //填充文件序列号，用头记录中的相应字段填充。
     memcpy(tailRecord.sFileNo, headRecord.sFileNo , sizeof(headRecord.sFileNo));
     
     //填充文件产生日期，用头记录中的相应字段填充。
     memcpy(tailRecord.sFileProduceTime, headRecord.sFileProduceTime, sizeof(headRecord.sFileProduceTime));
     
     //填充话单总数。
     sprintf(szCalculateField, "%09u", dwRecordCount);
     strncpy(tailRecord.sRecordCount, szCalculateField, sizeof(tailRecord.sRecordCount));
     
     #ifdef _PLATFORM_WIN32
     //填充总时长合计。
     sprintf(szCalculateField, "%015I64u", nTotalDuration);
     strncpy(tailRecord.sTotalDuration, szCalculateField, sizeof(tailRecord.sTotalDuration));
     
     //填充上行数据流量合计。
     sprintf(szCalculateField, "%015I64u", nTotalDataFlowUp);
     strncpy(tailRecord.sTotalDataFlowUp, szCalculateField, sizeof(tailRecord.sTotalDataFlowUp));
     
     //填充下行数据流量合计。
     sprintf(szCalculateField, "%015I64u", nTotalDataFlowDown);
     strncpy(tailRecord.sTotalDataFlowDown, szCalculateField, sizeof(tailRecord.sTotalDataFlowDown));
     #else
	 //填充总时长合计。
     sprintf(szCalculateField, "%015Illu", nTotalDuration);
     strncpy(tailRecord.sTotalDuration, szCalculateField, sizeof(tailRecord.sTotalDuration));
     
     //填充上行数据流量合计。
     sprintf(szCalculateField, "%015Illu", nTotalDataFlowUp);
     strncpy(tailRecord.sTotalDataFlowUp, szCalculateField, sizeof(tailRecord.sTotalDataFlowUp));
     
     //填充下行数据流量合计。
     sprintf(szCalculateField, "%015Illu", nTotalDataFlowDown);
     strncpy(tailRecord.sTotalDataFlowDown, szCalculateField, sizeof(tailRecord.sTotalDataFlowDown));
     #endif

     //填充通信费合计。
     sprintf(szCalculateField, "%09u", nTotalBasicFee);
     strncpy(tailRecord.sTotalBasicFee, szCalculateField, sizeof(tailRecord.sTotalBasicFee));
     
     //填充信息费合计。
     sprintf(szCalculateField, "%09u", nTotalInfoFee);
     strncpy(tailRecord.sTotalInfoFee, szCalculateField, sizeof(tailRecord.sTotalInfoFee));
     
     //填充回车和换行。
     strncpy(tailRecord.sDivLine, "\r\n", sizeof(tailRecord.sDivLine));
     
     
     try
     {
         //把指针移动到第二份临时文件的头部，写入头记录
         if(0 != fseek(pTmpDestFile, 0, SEEK_SET))
         {
             fclose(pTmpDestFile);
             return FALSE;
         }
         fwrite(&headRecord, 1, sizeof(headRecord), pTmpDestFile);
         ulDestFileOffset += dwFileLen;
         
         //把指针移动到第二份临时文件的尾部，写如尾记录
         if(0 != fseek(pTmpDestFile, ulDestFileOffset, SEEK_SET))
         {
             fclose(pTmpDestFile);
             return FALSE;
         }
         
         fwrite(&tailRecord, 1, sizeof(tailRecord), pTmpDestFile);
         
         delete []pBillFile;
         pBillFile = NULL;
         fflush(pTmpDestFile);       
         //删除临时文件
         fclose(pFirstTempFile);
         
     }
     catch(const char * e)
     {
         e++; //此句垃圾，就是消除“变量未引用”的告警
         //tmpDestFile.Close(); 
         fclose(pTmpDestFile);
         return FALSE;
     }
     
     //tmpDestFile.Close();
     fclose(pTmpDestFile);
     
     /*
     * 把新话单文件移动到正式目录，并把文件名记录到汇总文件中、并更新状态文件中的文件序号。
     */
     STRING strDstFilePath = szDestFileName;
     nPos1 = strDstFilePath.rfind('/');      
     strDstFilePath = strDstFilePath.substr(0, nPos1 + 1);
     //说明： 由于序列号为最大值的话单文件是在第二天的凌晨零点以后产生的，所以当
	 //序列号为最大值的时候前缀中的时间日期取值应该是前一天的时间取值。
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
  Description     : 清除输出的接口实现函数
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
Description     : 从状态文件中读入记录的通道文件生成时间、文件序列号、状态文件。
Input           : void
Output          : 
Return          : BOOL
Calls           : 
Called by       : 

  History         : 
  1 Date          : 2003-3-15
  Author        : Gan Zhiliang
  Modification  : Created file
  备注：          为了能够简化实际开局中的配置工作同时达到灵活性的要求，后存盘
  中采用多级配置的方式。配置参数的优先级从低到高总共分为
  总体/接入点/通道3个级别，如果高级别的参数没有配置，
  则取低级别的配置参数
*******************************************************************************/
BOOL LoadStatusInfo(char *szConfigPath, UINT4 nApID)
{
	
	
	STRING strKeyChannel;
	STRING strFilePrefix;
	UINT4 unSerialNumLen = 0;
	STRING strAPSelection;
	
	g_pStatusArray = new TStatusInfo[g_nChannelCount];
	g_pMemStatusArray = new TMemStatus[g_nChannelCount];
	
	//打开配置文件  
	CINIFile *cfgIniFile;
	cfgIniFile = new CINIFile(szConfigPath);
	if(cfgIniFile->Open() == FALSE)
	{
		delete cfgIniFile;
		return FALSE;
	}
	
	//读取本进程对应的接入点号
	char strSec[20];
	sprintf(strSec, "%s%d", CFG_SEC_AP, nApID);
	strAPSelection = strSec;
	
	//读取省代码
	char szProvCode[20];
	cfgIniFile->GetString(strAPSelection.c_str(), "ProvCode", DEFAULT_PROV_CODE, szProvCode, 20);
	strcpy(g_sProvCode, szProvCode);
	
	//读取归属局代码
	char szHomeCode[20];
	cfgIniFile->GetString(strAPSelection.c_str(), "HomeCode", DEFAULT_HOME_CODE, szHomeCode, 20);
	strcpy(g_sHomeCode, szHomeCode);
	
	//读入文件版本号码
	char szFileVer[20];
	cfgIniFile->GetString(strAPSelection.c_str(), "FileVersion", DEFAULT_FILE_VERSION, szFileVer, 20);
	strcpy(g_sFileVersion, szFileVer);
	
	
	//读入文件生成时间间隔，读入序列号宽度，读入起始序列号
	g_ulSaveFileTimeSpan = cfgIniFile->GetInt(strAPSelection.c_str(), "SaveFileTimeSpan", DEFAULT_SAVE_FILE_TIME_SPAN);
	g_nSeqNoWidth = cfgIniFile->GetInt(strAPSelection.c_str(), "SeqNoWidth", DEFAULT_SEQNO_WIDTH);
	g_ulStartSeqNo = cfgIniFile->GetInt(strAPSelection.c_str(), "StartSeqNo", DEFAULT_START_SEQ_NO);       
	
	//得到接入点名
	char szAPName[255];
	cfgIniFile->GetString(strAPSelection.c_str(), CFG_AP_NAME, "WLAN", szAPName, 255);
	
	//得到主状态文件的根目录
	char szMainStatusRootDir[MAX_PATH];
	cfgIniFile->GetString(CFG_SEC_DISK_FILE, CFG_MAIN_STATUS_ROOT_DIR, 
		CFG_MAIN_STATUS_ROOT_DIR_DEFAULT, szMainStatusRootDir, MAX_PATH);
	g_strStatusFilePath = szMainStatusRootDir;
	g_strStatusFilePath = g_strStatusFilePath + "/" + szAPName;
	g_strStatusFilePath += "/BackSavePatch";
	CreateDeepDir(g_strStatusFilePath.c_str());
	g_strStatusFilePath += "/PatchInfo.dat";
	
	
	//得到最终话单文件的根目录
	char szMainBackSaveRootDir[MAX_PATH];
	cfgIniFile->GetString(CFG_SEC_DISK_FILE, CFG_BACK_SAVE_ROOT_DIR, 
		CFG_BACK_SAVE_ROOT_DIR_DEFAULT, 
		szMainBackSaveRootDir, MAX_PATH);
	
	//得到临时文件的根目录
	g_strTmpFilePath = szMainBackSaveRootDir;
	g_strTmpFilePath = g_strTmpFilePath + "/" + szAPName;
	g_strTmpFilePath += "/temp/";
	
	//得到第二份文件的根目录
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
	
	//取得状态文件大小
	if(fseek(g_pfileStatus, 0, SEEK_END) != 0) 
		return FALSE;
	
	UINT4 uFileLen = ftell(g_pfileStatus);
	
	//如果得到了不正确的状态文件，把文件长度置为0。
	if (0 != uFileLen % sizeof(TStatusInfo))
	{
		int nResult = ACE_OS::truncate(g_strStatusFilePath.c_str(), 0);
		if (0 != nResult)
		{
			return FALSE;
		}
		
		fflush(g_pfileStatus);
	}
	
	//如果文件长度不为0，从状态文件中读入状态信息。
	if (0 !=  uFileLen)
	{
		UINT4 dwReadCount = min(uFileLen, g_nChannelCount * sizeof(TStatusInfo));
		fseek(g_pfileStatus, 0, SEEK_SET);
		//从状态文件中读入状态信息
		fread(g_pStatusArray, 1, dwReadCount, g_pfileStatus);
		
	}
	else
	{
		//如果长度为零或者异常，要得到当前文件的序列号码
		for(int j = 0; j < g_nChannelCount; j++)
		{
			g_pStatusArray[j].dwCurrentFileNo = GetFileSerialNo(g_ulSaveFileTimeSpan, g_ulStartSeqNo);
			
		}
		
	}
	
	
	char szChlSection[40];
	char szFormalPrefix[512];
	int nCsnLen = 0;
	//把状态文件中保存的信息初始化到内存中去  
	for (int i = 0 ; i < g_nChannelCount; i++)
	{
		g_pMemStatusArray[i].pStatusInfo = g_pStatusArray + i;
		
		
		sprintf(szChlSection, CFG_SEC_CHANNEL, nApID, i + 1);
		
		
		//从配置文件中读取正式文件的前缀，配置参数的读取原则见函数头中的备注
		cfgIniFile->GetString(CFG_SEC_DISK_FILE, CFG_PREFIX, CFG_PREFIX_DEFAULT,
			szFormalPrefix, 512);
		cfgIniFile->GetString(strAPSelection.c_str(), CFG_PREFIX, szFormalPrefix,
			szFormalPrefix, 512);
		cfgIniFile->GetString(szChlSection, CFG_PREFIX, szFormalPrefix,
			szFormalPrefix, 512);
		
		//从配置文件中读取文件序列号长度，配置参数的读取原则见函数头中的备注
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
Description     : 保存状态文件。
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
          
          //如果原来有多个通道，后来减少了，那么状态文件长度就应该根据实际变化
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
  Description     : 判断一个字符是不是数值字符
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
  Description     : 由起始序列号码和文件保存时间间隔得到某一制定时刻最近应该保存
  文件的序列号码
  比如： 应该已经保存的文件的序列号码，我们保存文件的时间在2点零1分，这个时候得到的
  序列号码应该是保存序列号为0的话单文件。
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
  Description     : 下一个时刻应该保存的文件的序列号码
  和：GetFileSerialNo不同的是，比如：0点30分，它得到的流水号码是0而GetFileSerialNo
  得到的流水号码是11，而不是0；
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
  Description     : 在程序启动的时候创建从上次宕机以来到现在应该生成的文件
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
              *如果时间在同一天
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
                  else//如果时间不在同一天
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
  Description     : 创建指定文件名称的仅含有文件头记录和尾记录的空文件，
  创建时间为系统当前时间
  Input           : STRING strFileName(文件名称全路径), strFileSuffix（后缀，主要是填充记录中的序列号码）
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
    
    //定义空话单文件的头记录和尾记录存放的内容
    TWlanHeadRecord headRecord;
    TWlanTailRecord tailRecord;
    
    memset(&headRecord, ' ', sizeof(TWlanHeadRecord));
    memset(&tailRecord, ' ', sizeof(TWlanTailRecord));
    
    
    /*
    * 填充头记录。
    */
    
    //填充头记录标记。
    strncpy(headRecord.sHeadFlag, "10", sizeof(headRecord.sHeadFlag));
    
    //上传文件的省代码。
    strncpy(headRecord.sProvCode, g_sProvCode, sizeof(headRecord.sProvCode));
    
    //填充归属局代码。
    strncpy(headRecord.sHomeCode, g_sHomeCode, sizeof(headRecord.sHomeCode));
    
    //填充文件序列号。
    memcpy(headRecord.sFileNo, strFileSuffix.c_str() , g_nSeqNoWidth);
    
    time_t tNow = ACE_OS::time(NULL);
    tm *tt = ACE_OS::localtime(&tNow);
    char szTime[20];
    sprintf(szTime, "%04d%02d%02d%02d%02d%02d",tt->tm_year + 1900, tt->tm_mon + 1, tt->tm_mday, tt->tm_hour, tt->tm_min, tt->tm_sec);
    
    memcpy(headRecord.sFileProduceTime, szTime, sizeof(headRecord.sFileProduceTime));
    
    //填充文件版本号。
    strncpy(headRecord.sFileVersion, g_sFileVersion, sizeof(headRecord.sFileVersion));
    
    //填充回车和换行。
    strncpy(headRecord.sDivLine, "\r\n", sizeof(headRecord.sDivLine));
    
    
    /*
    * 填充尾记录。
    */
    
    //填充尾记录标记。
    strncpy(tailRecord.sTailFlag, "90", sizeof(tailRecord.sTailFlag));
    
    
    //填充归属局代码，用头记录中的相应字段填充。
    memcpy(tailRecord.sHomeCode, headRecord.sHomeCode, sizeof(headRecord.sHomeCode));
    
    //上传文件的省代码，用头记录中的相应字段填充。
    memcpy(tailRecord.sProvCode, headRecord.sProvCode , sizeof(headRecord.sProvCode));
    
    //填充文件序列号，用头记录中的相应字段填充。
    memcpy(tailRecord.sFileNo, headRecord.sFileNo , sizeof(headRecord.sFileNo));
    
    //填充文件产生日期，用头记录中的相应字段填充。
    memcpy(tailRecord.sFileProduceTime, headRecord.sFileProduceTime, sizeof(headRecord.sFileProduceTime));
    
    //填充话单总数,总时长合计,上行数据流量合计,下行数据流量合计,通信费合计,信息费合计
    //因为对于空话单这些数据的值都因为字符'0'
    memset(tailRecord.sRecordCount, '0', offsetof(TWlanTailRecord, sReserve3) - offsetof(TWlanTailRecord, sRecordCount));
    
    //填充回车和换行。
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
Description     : 由序列号以及序列号的宽度得到序列号的格式
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
Description     : 从前缀描述字符串得到文件的前缀，其主要目的是满足带有%的前缀
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
Description     : 该函数判断在strFilePath路径下存在不存在后缀名为strFileSuffix 的文件
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
Description     : 本函数判断定时器时间是否已经到达了话单文件输出的时间
Input           : void
Output          : 
Return          : BOOL
Calls           : 
Called by       : 

  History         : 
  1 Date          : 2003-4-23
  Author        : Gan Zhiliang
  Modification  : Created file
  Comments        :本函数判断remailtickets是否到达WLAN等待的时间
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
Description     : 重新设定需要等待的tickets
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
