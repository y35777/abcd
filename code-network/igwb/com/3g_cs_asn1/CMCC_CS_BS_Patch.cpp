#include "../include/frame.h"
#include "../include/toolbox.h"
#include "CMCC_CS_BS_Patch.h"
#include "MSCV9BillFormat.h"
#include "event_bill_file.h"
#include "oss_asn1_cmcc_cs.h"
#include "CMCC_CS_R4_CDR.h"
#include "CMCC_CS_CDR_Convert.h"
#include "resource.h"
#include "StaticMemMnger.h"
#include "../include/export_dll.h"

#define CMCC_EVENT_FILE_NAME   "cmcc_event_file.dat"
#define CMCC_READ_BILL_COUNT    1000 
#define CMCC_ASN1_FILE_SIZE     1.5 * 1024 * 1024

int CBSPatchForCMCC::s_nBillLen = 0;
int CBSPatchForCMCC::s_nChannelCount = 0; 
CEventBillFile* CBSPatchForCMCC::s_pEventFile = NULL; 
char* CBSPatchForCMCC::s_pBillBuf = NULL;
int CBSPatchForCMCC::s_nBillBufSize = 0;
void* CBSPatchForCMCC::s_pASN1Global = NULL;
char* CBSPatchForCMCC::s_pMemFile = NULL;
int CBSPatchForCMCC::s_nMemFileLen = 0;

SET<int> CBSPatchForCMCC::s_LCSSet;

#define CMCC_BLOCK_BUFF_SIZE          1 * 1024 * 1024
CStaticMemMnger* g_pAsnBuffForCMCC = NULL;         //ASN1编码前的缓冲区

extern "C" IGWB_Svc_Export BOOL PC_Init(char* szConfigPath, BYTE nApID, UINT4 nChannelCount)
{
    return CBSPatchForCMCC::Init(szConfigPath, nApID, nChannelCount);
}
extern "C" IGWB_Svc_Export BOOL PC_Process(char* szFileName, char*& szDestFileName, UINT4 nChannelNum)
{
    return CBSPatchForCMCC::Process(szFileName, szDestFileName, nChannelNum);
}
extern "C" IGWB_Svc_Export BOOL PC_Terminate()
{
    return CBSPatchForCMCC::Terminate();
}

//by	杨善	2004-6-1	问题单SWPD05564
extern "C" IGWB_Svc_Export BOOL PC_CanClose(char* szFileName)
{
	return CBSPatchForCMCC::CanClose(szFileName);
}
//the end	问题单SWPD05564
/******************************************************
// 函数名: 	CBSPatchForCMCC::Init
// 作者:    chenliangwei
// 时期:    02-10-28
// 描述:    CMCC后存盘补丁初始化    
// 输入:
//       参数1 :char* szConfigPath
//       参数2 :BYTE nApID
//       参数3 :UINT4 nChannelCount
// 返回值: BOOL 
******************************************************/
BOOL CBSPatchForCMCC::Init(char* szConfigPath, BYTE nApID, UINT4 nChannelCount)
{
    s_nChannelCount = nChannelCount;
    if(s_nChannelCount <= 0)
        return FALSE;

    //构建LCS话单类型集合
    s_LCSSet.insert(SS_ACT);
    s_LCSSet.insert(MT_SMS);
    s_LCSSet.insert(MO_SMS);
    s_LCSSet.insert(TRANSIT);
    s_LCSSet.insert(GATEWAY_INCOMING);
    s_LCSSet.insert(GATEWAY_OUTGOING);
    s_LCSSet.insert(ROAM);
    s_LCSSet.insert(MOI);
    s_LCSSet.insert(MO_HO);
    s_LCSSet.insert(MT_HO);
    s_LCSSet.insert(TCAMEL);
    s_LCSSet.insert(FCI);
    s_LCSSet.insert(EC);
    s_LCSSet.insert(LCS);
    s_LCSSet.insert(Check_IMEI);
    s_LCSSet.insert(QUERY_HLR);
    s_LCSSet.insert(CommonEquip);

    //读取INI文件
    CINIFile IniFile(szConfigPath);
    BOOL bSucc = IniFile.Open();
    if (!bSucc)
    {
        TRACE(MTS_BACKSAVE_PATCH, S_BS_TRC_FAIL_OPEN_INI);
        return FALSE;
    }
    
	//读取本进程对应的接入点名称
	char szSec[20];
	sprintf(szSec, "%s%d", CFG_SEC_AP, nApID);

    char szAPName[20];
    IniFile.GetString(szSec,
                  CFG_AP_NAME,
                  szSec,
                  szAPName,
                  sizeof(szAPName));

    //得到并创建主状态文件的根目录
    char szMainStatusRootDir[MAX_PATH];
    char szMainStatusPath[MAX_PATH];
    IniFile.GetString(CFG_SEC_DISK_FILE, 
                         CFG_MAIN_STATUS_ROOT_DIR, 
                         CFG_MAIN_STATUS_ROOT_DIR_DEFAULT, 
                         szMainStatusRootDir, MAX_PATH);

    sprintf(szMainStatusPath, "%s/%s/%s", 
            szMainStatusRootDir, szAPName, "save");
    CreateDeepDir(szMainStatusPath);

    sprintf(szMainStatusPath, "%s/%s", 
            szMainStatusPath, CMCC_EVENT_FILE_NAME);

    //创建CEventBillFile对象
    s_pEventFile = new CEventBillFile;
    if (!s_pEventFile->Init(szMainStatusPath))
    {
        TRACE(MTS_BACKSAVE_PATCH, "Init event file fail");
        delete s_pEventFile;
        s_pEventFile = NULL;
        return FALSE;
    }

    //创建读话单缓冲区
    int nBillCount = IniFile.GetInt(szSec, "CMCCReadBillCount", 
                       CMCC_READ_BILL_COUNT);
    s_nBillLen = sizeof(MSC_Bill_Report);
    s_nBillBufSize = s_nBillLen * nBillCount;
    s_pBillBuf = new char[s_nBillBufSize];
    if (s_pBillBuf == NULL)
    {
        TRACE(MTS_BACKSAVE_PATCH, "Allocate memery space of read bill buffer fail");
        delete s_pEventFile;
        s_pEventFile = NULL;
        return FALSE;
    }
    
    //创建编码文件缓冲区
    s_nMemFileLen = IniFile.GetInt(szSec, "CMCCAsn1FileSize", 
                       CMCC_ASN1_FILE_SIZE);

    s_pMemFile = new char[s_nMemFileLen];
    if (s_pMemFile == NULL)
    {
        TRACE(MTS_BACKSAVE_PATCH, "Allocate memery space of ASN1 file buffer fail");

        delete []s_pBillBuf;
        s_pBillBuf = NULL;

        delete s_pEventFile;
        s_pEventFile = NULL;

        return FALSE;
    }

    //初始化ASN1编码前的缓冲区
    size_t nBlockSize = IniFile.GetInt(szSec, "CMCCBlockBuffSize", 
                        CMCC_BLOCK_BUFF_SIZE);

    g_pAsnBuffForCMCC = new CStaticMemMnger(nBlockSize);
    if (g_pAsnBuffForCMCC == NULL)
    {
        TRACE(MTS_BACKSAVE_PATCH, "Allocate memery space of ASN1 buffer fail");
        
        delete []s_pBillBuf;
        s_pBillBuf = NULL;
        
        delete s_pEventFile;
        s_pEventFile = NULL;

        delete s_pMemFile;
        s_pMemFile = NULL;
        
        return FALSE;
    }    

    //初始化OSS库
    s_pASN1Global = Init_OSS_Lib_Final_CMCC_CS();
    
    return TRUE;
}


/******************************************************
// 函数名: 	CBSPatchForCMCC::Terminate
// 作者:    chenliangwei
// 时期:    02-10-29
// 描述:    清除函数
// 输入:
// 返回值: BOOL 
******************************************************/
BOOL CBSPatchForCMCC::Terminate()
{
    s_LCSSet.clear();

    if (s_pEventFile != NULL)
    {
        delete s_pEventFile;
        s_pEventFile = NULL;
    }

    if (s_pBillBuf != NULL)
    {
        delete []s_pBillBuf;
        s_pBillBuf = NULL;
    }

    if (s_pMemFile != NULL)
    {
        delete []s_pMemFile;
        s_pMemFile = NULL;
    }

    if (g_pAsnBuffForCMCC != NULL)
    {
        delete []g_pAsnBuffForCMCC;
        g_pAsnBuffForCMCC = NULL;
    }
    
    //释放OSS库
    Release_OSS_Lib_Final_CMCC_CS(s_pASN1Global);

    return TRUE;
}

/******************************************************
// 函数名: 	CBSPatchForCMCC::Process
// 作者:    chenliangwei
// 时期:    02-10-28
// 描述:    处理CMCC的话单文件：
            1.合并Event话单；
            2.将话单进行ASN.1编码
// 输入:
//       参数1 :char* szFileName
//       参数2 :char*& szDestFileName
//       参数3 :UINT4 nChannelNum       从1开始的，比如：通道1,nChannelNum =1     

// 返回值: BOOL 
******************************************************/
BOOL CBSPatchForCMCC::Process(char* szFileName, char*& szDestFileName, UINT4 nChannelNum)
{ 
    if(nChannelNum < 1)
    {
        TRACE(MTS_BACKSAVE_PATCH, "nChannelNum  is illegal");
        return FALSE;
    }
    
    /*
     * 分多次把整个话单文件读入内存，进行转换。
     */
    FILE*  pSrcFile = fopen(szFileName, "rb");
    if (NULL == pSrcFile)
    {
        TRACE(MTS_BACKSAVE_PATCH, "Open Source File %s fail", szFileName);
        return FALSE;
    }
    
    s_pEventFile->SetSrcFileName(szFileName);
    g_pAsnBuffForCMCC->Deallocate();
    
    UINT4 nRecordCnt = 0;
    UINT4 nFileOffset = 0;
    UINT4 nFileLen = FileSize(pSrcFile);
    MSC_Bill_Report* pCurrentBill = NULL;
    CMCC_CS_ObservedIMEITicketFile sDestIMEIFile;
    CMCC_CS_ObservedIMEITicketFile::CMCC_CS__seqof9* pIMEITail = NULL;
    CMCC_CS_CallEventDataFile sDestFile;
    CMCC_CS_CallEventDataFile::CMCC_CS__seqof8* pRecordTail = NULL;

    //是否初始化sDestIMEIFile
    BOOL bInitDestFile = TRUE;
    BOOL bDestIMEIFile = FALSE;
    /*
	 * 从文件中依次读出缓冲区大小的话单，进行转换。
	 */
	while (nFileOffset < nFileLen)
	{
		//把话单读进缓冲区
		size_t nBytesRead = fread(s_pBillBuf, 1, s_nBillBufSize, pSrcFile);
		nFileOffset += nBytesRead;
	
		/*
		 * 对缓冲区中的每条话单进行转换。
		 */
		INT4 nOffset = 0;
		while ( (nOffset + s_nBillLen) <= nBytesRead)
		{
			pCurrentBill = (MSC_Bill_Report*)(s_pBillBuf + nOffset);
			nOffset += s_nBillLen;
            
            //初始化文件 
            if (bInitDestFile) 
            {
                if (pCurrentBill->bill_type == Check_IMEI) 
                {
                    memset(&sDestIMEIFile,0,sizeof(sDestIMEIFile));
                    sDestIMEIFile.extensions = NULL;
                    sDestIMEIFile.productionDateTime.length = 9;
                    sDestIMEIFile.noOfRecords = 0;

                    bDestIMEIFile = TRUE;
                }
                else
                {
                    memset(&sDestFile, 0, sizeof(sDestFile));
                    sDestFile.extensions = NULL; 
                    /*new CMCC_CS_ManagementExtensions_;
                    sDestFile.extensions->next = NULL;
                    sDestFile.extensions->value.bit_mask = 0;
                    sDestFile.extensions->value.bit_mask |= CMCC_CS_significance_present;
                    sDestFile.extensions->value.significance = 1;
                    sDestFile.extensions->value.information.value = new (g_pAsnBuffForCMCC)BYTE[1];
                    sDestFile.extensions->value.information.length = 1;
                    sDestFile.extensions->value.identifier.value = new (g_pAsnBuffForCMCC)BYTE[1];
                    sDestFile.extensions->value.identifier.length = 1;*/

                    sDestFile.headerRecord.extensions = NULL;
                /*
                    sDestFile.headerRecord.extensions = new (g_pAsnBuffForCMCC)CMCC_CS_ManagementExtensions_;
                    sDestFile.headerRecord.extensions->next = NULL;
                    sDestFile.headerRecord.extensions->value.bit_mask = 0;
                    sDestFile.headerRecord.extensions->value.bit_mask |= CMCC_CS_significance_present;
                    sDestFile.headerRecord.extensions->value.significance = 0;
                    sDestFile.headerRecord.extensions->value.information.value = new (g_pAsnBuffForCMCC)BYTE[2];
                    memset(sDestFile.headerRecord.extensions->value.information.value, 0, 2);
                    sDestFile.headerRecord.extensions->value.information.length = 2;
                    sDestFile.headerRecord.extensions->value.identifier.value = new (g_pAsnBuffForCMCC)BYTE[2];
                    sDestFile.headerRecord.extensions->value.identifier.length = 2;*/

                    sDestFile.headerRecord.productionDateTime.length = 9;
                    sDestFile.headerRecord.recordingEntity.length = 1;
    
                    sDestFile.trailerRecord.firstCallDateTime.length = 9;
                    sDestFile.trailerRecord.extensions = NULL; /*new CMCC_CS_ManagementExtensions_;
                    sDestFile.trailerRecord.extensions->next = NULL;
                    sDestFile.trailerRecord.extensions->value.bit_mask = 0;
                    sDestFile.trailerRecord.extensions->value.bit_mask |= CMCC_CS_significance_present;
                    sDestFile.trailerRecord.extensions->value.significance = 1;
                    sDestFile.trailerRecord.extensions->value.information.value = new (g_pAsnBuffForCMCC)BYTE[2];
                    memset(sDestFile.trailerRecord.extensions->value.information.value, 0, 2);
                    sDestFile.trailerRecord.extensions->value.information.length = 1;
                    sDestFile.trailerRecord.extensions->value.identifier.value = new (g_pAsnBuffForCMCC)BYTE[1];
                    sDestFile.trailerRecord.extensions->value.identifier.length = 1;*/
    
                    sDestFile.trailerRecord.lastCallDateTime.length = 9;
                    sDestFile.trailerRecord.noOfRecords = 0;
                    sDestFile.trailerRecord.productionDateTime.length = 9;
                    sDestFile.trailerRecord.recordingEntity.length = 1;
                }

                bInitDestFile = FALSE;
            }

            if (bDestIMEIFile) 
            {  
                CMCC_CS_ObservedIMEITicketFile::CMCC_CS__seqof9* pObservedIMEITickets = 
                                            new (g_pAsnBuffForCMCC)CMCC_CS_ObservedIMEITicketFile::CMCC_CS__seqof9;
                pObservedIMEITickets->next = NULL;
                
                if (CMCC_CS_R4_CDR_CONVERT::ConvertABill_CHECK_IMEI((BYTE*)pCurrentBill, sizeof(MSC_Bill_Report), &pObservedIMEITickets->value))
                {
                    //将转换后的话单链入编码文件列表
                    if(sDestIMEIFile.observedIMEITickets == NULL)
                        sDestIMEIFile.observedIMEITickets = pObservedIMEITickets;
                    else
                        pIMEITail->next = pObservedIMEITickets;

                    pIMEITail = pObservedIMEITickets;
                    
                    nRecordCnt++;
                }
                else
                {
                    TRACE(MTS_BACKSAVE_PATCH, "Bill convert failed in Source File %s, offset = %d", 
                          szFileName, nFileOffset - nBytesRead + nOffset - s_nBillLen);
                    pObservedIMEITickets = NULL;
                }
            }
            else
            {
                //事件话单
                //by    杨善    2004-6-2    对应问题单SWPD05614
                if (pCurrentBill->bill_type == EVENT_BILL)
                //the end  对应问题单SWPD05614
                {
                    s_pEventFile->AddAEventBill(pCurrentBill, 
                                      nFileOffset - nBytesRead + nOffset - s_nBillLen);
                    continue;
                }

                MSC_Bill_Report* pEvent = NULL;
                int nEventCnt = 0 ;

                //普通话单
                //by    杨善    2004-6-2    对应问题单SWPD05614
                if (s_LCSSet.find(pCurrentBill->bill_type) == s_LCSSet.end())
                {
                    //只有单张话单和最后一张话单需要查找事件话单
                    MSC_Bill_Report* pGnlBill = (MSC_Bill_Report*)pCurrentBill;
                    if ((pGnlBill->record_type == SINGLE_BILL) || (pGnlBill->record_type == LAST_BILL))
                    {
                        s_pEventFile->FindEventBills(pCurrentBill, pEvent, nEventCnt);
                    }
                }
                //the end  对应问题单SWPD05614

               //进行转换
                CMCC_CS_CallEventDataFile::CMCC_CS__seqof8* pCallEventRecords = 
                                           new (g_pAsnBuffForCMCC)CMCC_CS_CallEventDataFile::CMCC_CS__seqof8;
                pCallEventRecords->next = NULL;

                if (CMCC_CS_R4_CDR_CONVERT::ConvertABill((BYTE*)pCurrentBill, sizeof(MSC_Bill_Report), (BYTE*)pEvent, nEventCnt, &pCallEventRecords->value))
                {
                    //将转换后的话单链入编码文件列表
                    if (sDestFile.callEventRecords == NULL)
                        sDestFile.callEventRecords = pCallEventRecords;
                    else
                        pRecordTail->next = pCallEventRecords;
                
                    pRecordTail = pCallEventRecords;

                    nRecordCnt++;
                }
                else
                {
                    TRACE(MTS_BACKSAVE_PATCH, "Bill convert failed in Source File %s, offset = %d", 
                           szFileName, nFileOffset - nBytesRead + nOffset - s_nBillLen);
                    //delete pCallEventRecords;
                    pCallEventRecords = NULL;
                }

                //清除已经合并的事件话单
                if (pEvent != NULL)
                {
                    s_pEventFile->RemoveEventBills(pCurrentBill);
                    delete []pEvent;
                    pEvent = NULL;
                }
            }
		}
    }

    fclose(pSrcFile);
    pSrcFile = NULL;

    /*
     *	填充文件的头尾记录
     */
    //填充文件头记录, 以源文件的创建时间为头时间
    ACE_stat FileStat;
    ACE_OS::stat(szFileName, &FileStat);
    tm* ptmCreateTime = gmtime(&FileStat.st_ctime);
    tm  tmCreateTime = *ptmCreateTime;

    BYTE byTime[6];
    byTime[0] = tmCreateTime.tm_year + 1900 - 2000;
    byTime[1] = tmCreateTime.tm_mon + 1;
    byTime[2] = tmCreateTime.tm_mday;
    byTime[3] = tmCreateTime.tm_hour;
    byTime[4] = tmCreateTime.tm_min;
    byTime[5] = tmCreateTime.tm_sec;
    
    char* pEncodedData = NULL;
    int   nBufferLen   = s_nMemFileLen;
    int nPDU;
    
    if (bDestIMEIFile)
    {
        CMCC_CS_R4_CDR_CONVERT::ConvertTimeStamp(sDestIMEIFile.productionDateTime,byTime);
        
        sDestIMEIFile.noOfRecords = nRecordCnt;

        nPDU = CMCC_CS_ObservedIMEITicketFile_PDU;
        pEncodedData = EnCodeCMCCCSBill_final_mt(s_pASN1Global, (char*)&sDestIMEIFile,
                                                 s_pMemFile, &nBufferLen,nPDU);       
    }
    else
    {
        CMCC_CS_R4_CDR_CONVERT::ConvertTimeStamp(sDestFile.headerRecord.productionDateTime, 
            byTime);

        //填充文件尾记录, 以当前时间为尾时间
        time_t tColseTime = time(NULL);
        tm* ptmColseTime = gmtime(&tColseTime);
        tm  tmColseTime = *ptmColseTime;
    
        byTime[0] = tmColseTime.tm_year + 1900 - 2000;
        byTime[1] = tmColseTime.tm_mon + 1;
        byTime[2] = tmColseTime.tm_mday;
        byTime[3] = tmColseTime.tm_hour;
        byTime[4] = tmColseTime.tm_min;
        byTime[5] = tmColseTime.tm_sec;
    
        CMCC_CS_R4_CDR_CONVERT::ConvertTimeStamp(sDestFile.trailerRecord.productionDateTime, 
                         byTime);

        sDestFile.trailerRecord.noOfRecords = nRecordCnt;

        nPDU = CMCC_CS_CallEventDataFile_PDU;
        pEncodedData = EnCodeCMCCCSBill_final_mt(s_pASN1Global, (char*)&sDestFile,
                                                 s_pMemFile, &nBufferLen,nPDU);        
    }
  
    //存入文件
    BOOL bRet = TRUE;
    if (pEncodedData != NULL)
    {
        FILE* pAsn1File = fopen(szDestFileName, "wb");
        if (pAsn1File == NULL)
        {
            TRACE(MTS_BACKSAVE_PATCH, "Open Asn1 dest file %s failed", szDestFileName);
            bRet = FALSE;
        }
        else
        {
            if (fwrite(pEncodedData, nBufferLen, 1, pAsn1File) != 1)
            {
                TRACE(MTS_BACKSAVE_PATCH, "Write Asn1 dest file %s failed", szDestFileName);
                bRet = FALSE;
            }
 
            fclose(pAsn1File);
            pAsn1File = NULL;
        }
    }
    else
    {
        TRACE(MTS_BACKSAVE_PATCH, "Source File %s Encode Asn1 failed", szFileName);
        bRet = FALSE;
   
    }

    //删除sDestFile中的话单记录
    //Clear_Memory(sDestFile);
    g_pAsnBuffForCMCC->Deallocate();

    s_pEventFile->SaveToFile();
     
    return bRet;
}

//清除所有编码后的记录
/*void CBSPatchForCMCC::Clear_Memory(CMCC_CS_CallEventDataFile& DataFile)
{
    CMCC_CS_CallEventDataFile::CMCC_CS__seqof6* q = DataFile.callEventRecords,* p = NULL;
    while(q)
    {
        p = q;
        q = q->next;
        CMCC_CS_R4_CDR_CONVERT::DeleteABill(&(p->value));
        delete p;
    }
    
    if(DataFile.extensions)
    {
		std::list<CMCC_CS_ManagementExtensions_ *> ListOfrecordExtensions;
		CMCC_CS_ManagementExtensions_ *pNextrecordExtensions = DataFile.extensions;

		while (pNextrecordExtensions)
		{
			ListOfrecordExtensions.push_back(pNextrecordExtensions);
			pNextrecordExtensions = pNextrecordExtensions->next;
		}

		std::list<CMCC_CS_ManagementExtensions_ *>::iterator it = ListOfrecordExtensions.begin();
		while (it != ListOfrecordExtensions.end())
		{
			if((*it)->value.information.value)
            {
                delete (*it)->value.information.value;
            }
			if((*it)->value.identifier.value)
            {
                delete (*it)->value.identifier.value;
            }
            delete (*it);
			it++;
		}
		
        DataFile.extensions = NULL;
    }

    if(DataFile.headerRecord.extensions)
    {
		std::list<CMCC_CS_ManagementExtensions_ *> ListOfrecordExtensions;
		CMCC_CS_ManagementExtensions_ *pNextrecordExtensions = DataFile.headerRecord.extensions;

		while (pNextrecordExtensions)
		{
			ListOfrecordExtensions.push_back(pNextrecordExtensions);
			pNextrecordExtensions = pNextrecordExtensions->next;
		}

		std::list<CMCC_CS_ManagementExtensions_ *>::iterator it = ListOfrecordExtensions.begin();
		while (it != ListOfrecordExtensions.end())
		{
			if((*it)->value.information.value)
            {
                delete (*it)->value.information.value;
            }
			if((*it)->value.identifier.value)
            {
                delete (*it)->value.identifier.value;
            }
            delete (*it);
			it++;
		}
		
        DataFile.headerRecord.extensions = NULL;
    }

    if(DataFile.trailerRecord.extensions)
    {
		std::list<CMCC_CS_ManagementExtensions_ *> ListOfrecordExtensions;
		CMCC_CS_ManagementExtensions_ *pNextrecordExtensions = DataFile.trailerRecord.extensions;

		while (pNextrecordExtensions)
		{
			ListOfrecordExtensions.push_back(pNextrecordExtensions);
			pNextrecordExtensions = pNextrecordExtensions->next;
		}

		std::list<CMCC_CS_ManagementExtensions_ *>::iterator it = ListOfrecordExtensions.begin();
		while (it != ListOfrecordExtensions.end())
		{
			if((*it)->value.information.value)
            {
                delete (*it)->value.information.value;
            }
			if((*it)->value.identifier.value)
            {
                delete (*it)->value.identifier.value;
            }
            delete (*it);
			it++;
		}
		
        DataFile.trailerRecord.extensions = NULL;
    }
}
*/
//by 杨善	2004-6-1	对应问题单SWPD05564
BOOL CBSPatchForCMCC::CanClose(char* szFileName)
{
	FILE*  pSrcFile = fopen(szFileName, "rb");
    if (NULL == pSrcFile)
    {
        TRACE(MTS_BACKSAVE_PATCH, "Open Source File %s fail", szFileName);
        return FALSE;
    }
	
    UINT4 nFileOffset = 0;
    UINT4 nFileLen = FileSize(pSrcFile);
    MSC_Bill_Report* pCurrentBill = NULL;
	
	while (nFileOffset < nFileLen)
	{
		//把话单读进缓冲区
		size_t nBytesRead = fread(s_pBillBuf, 1, s_nBillBufSize, pSrcFile);
		nFileOffset += nBytesRead;
		
		INT4 nOffset = 0;
		while ( (nOffset + s_nBillLen) <= nBytesRead)
		{
			pCurrentBill = (MSC_Bill_Report*)(s_pBillBuf + nOffset);
			nOffset += s_nBillLen;	
			
            if (pCurrentBill->bill_type != EVENT_BILL)
            {
				fclose(pSrcFile);
				pSrcFile = NULL;
				return TRUE;
            }
		}
	}
	
	fclose(pSrcFile);
	pSrcFile = NULL;
	
    if (nFileLen == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;           
    }
}
//the end	 对应问题单SWPD05564
