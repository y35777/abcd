#include "../include/toolbox.h"
#include "../r98_770/asn1_r98/CDRF_2GCDR_Org.h"
#include "../r98_770/asn1_r98/CDRF_2GCDR_Fin.h"
#include "../r98_770/asn1_r98/oss_asn1_r98.h"
#include "asn1_r98/CDRF_2GCDR_Org_M.h"
#include "asn1_r98/CDRF_2GCDR_Fin_M.h"
#include "asn1_r98/oss_asn1_r98.h"
#include "gprs_moto.h"
#include "../include/export_dll.h"

#define BILL_FILE_SIZE_GRPS       (1024 * 1024 * 1.5)    //���ڴ�Ż����ļ����ڴ�ռ�Ĵ�С
#define BUFF_FILE_SIZE_GRPS       (1024 * 1024 * 4)      //���ڴ�Ż���Value���ݵ��ڴ�ռ�Ĵ�С

//CG�ĵ�ַ
char  CR98_770PatchForMoto::g_szCgID_GPRS[15] = "";

//Gsn�ĵ�ַ
char  CR98_770PatchForMoto::g_szGsnID_GPRS[15] = "";

//�����Ƿ�ϲ�
BOOL  CR98_770PatchForMoto::g_bMustMerged_GPRS = CFG_AP_KEY_MUST_MERGE_DEFAULT;

//Motorola�Ƿ�����CMCC
BOOL  CR98_770PatchForMoto::g_bCMCC_GPRS = CFG_AP_KEY_USEIN_CMCC_DEFAULT;

//�ڴ�ӳ���ļ�ָ��
BYTE* CR98_770PatchForMoto::g_pMemFile_GPRS = NULL;

//�ڴ�ӳ���ļ��ĳ���
UINT4 CR98_770PatchForMoto::g_nMemFileLen_GPRS = BILL_FILE_SIZE_GRPS;

//�ڴ�ӳ���ļ���Ч���ݵĳ���
UINT4 CR98_770PatchForMoto::g_nMemDataLen_GPRS = 0;

//���ڽ��жԺϲ��Ļ������н���Ļ�������
void* CR98_770PatchForMoto::g_pASN1Global_GPRS = 0;

//���ڽ��жԲ��ϲ��Ļ������н���Ļ�������
void* CR98_770PatchForMoto::g_pASN1Global_M_GPRS = 0;

//M�Ļ����ļ��ṹ
void* CR98_770PatchForMoto::g_pBSRecord_GPRS = 0;

//M�Ļ����ļ��ṹ�ı�ͷ����
void* CR98_770PatchForMoto::g_pBSSummaryRecord_GPRS = 0;

SMEM_LINK* CR98_770PatchForMoto::g_pMemLink_GPRS = NULL;

//add by zkq 2004-07-08
BYTE * CR98_770PatchForMoto::g_pMemDestFile;        //�����ڴ�ӳ���ļ�ָ�� 
int    CR98_770PatchForMoto::g_nMemDestFileLen;          //�����ڴ�ӳ���ļ��ĳ���
//end
extern "C" IGWB_Svc_Export BOOL PC_Init(const char* szConfigPath,
                                        BYTE nApID,
                                        UINT4 nChannelCount)
{
    return CR98_770PatchForMoto::Init(szConfigPath, nApID, nChannelCount);
}
extern "C" IGWB_Svc_Export BOOL PC_Process(const char* szFileName,
                                           char*& szDestFileName,
                                           UINT4 nChannelNum)
{
    return CR98_770PatchForMoto::Process(szFileName, szDestFileName, nChannelNum);
}
extern "C" IGWB_Svc_Export BOOL PC_Terminate()
{
    return CR98_770PatchForMoto::Terminate();
}

/*******************************************************************
�������ƣ�BOOL Init()
������������̬���ʼ������Ľӿ�ʵ�ֺ���
���������
�����������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE
���ú�����
����������
����ʱ�䣺maimaoshi,2002/12/23
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CR98_770PatchForMoto::Init(const char* szConfigPath,
                                BYTE nApID,
                                UINT4 nChannelCount)
{
	g_bMustMerged_GPRS        = TRUE;

	g_pMemFile_GPRS           = NULL;

	g_pASN1Global_GPRS        = NULL;
	g_pASN1Global_M_GPRS      = NULL;

	g_pBSRecord_GPRS          = NULL;
	g_pBSSummaryRecord_GPRS   = NULL;

	g_pMemDestFile            = NULL; //add by zkq 2004-07-08
	g_nMemDestFileLen         = 0; //add by zkq 2004-07-08
    //�������ļ�	
    CINIFile *pIniFile;
    pIniFile = new CINIFile(szConfigPath);
    if(pIniFile->Open() == FALSE)
    {
        delete pIniFile;
        return FALSE;
    }

    //��ȡ�����̶�Ӧ�Ľ�����
    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP, nApID);
        
	//��ȡCG��IP��ַ
	char szLocalIp[20];
	pIniFile->GetString(strSec, 
                        CFG_AP_KEY_LOCAL_IP_TO_PM,
                        CFG_AP_KEY_LOCAL_IP_TO_PM_DEFAULT,
                        szLocalIp,
                        sizeof(szLocalIp));
	strcpy(g_szCgID_GPRS, szLocalIp);

	//��ȡGsn��IP��ַ
	char szGsnIp[20];
	pIniFile->GetString(strSec, 
                        CFG_AP_KEY_GSNIP, 
                        CFG_AP_KEY_GSNIP_DEFAULT,
                        szGsnIp,
                        sizeof(szGsnIp));
	strcpy(g_szGsnID_GPRS, szGsnIp);
	
	//��ȡ�Ƿ�ϲ���־
	g_bMustMerged_GPRS = pIniFile->GetInt(strSec,
                                          CFG_AP_KEY_MUST_MERGE,
                                          CFG_AP_KEY_MUST_MERGE_DEFAULT);

	//by ldf ��Ӧ���ⵥD20358
	//��ȡ�����CMCC���
	g_bCMCC_GPRS = pIniFile->GetInt(strSec,
                                    CFG_AP_KEY_USEIN_CMCC,
                                    CFG_AP_KEY_USEIN_CMCC_DEFAULT);
	//end

	pIniFile->Close();
	delete pIniFile;

	g_nMemFileLen_GPRS          = BILL_FILE_SIZE_GRPS;
    g_pMemFile_GPRS             = new BYTE[BILL_FILE_SIZE_GRPS];
	memset(g_pMemFile_GPRS, 0, BILL_FILE_SIZE_GRPS);

    g_pMemLink_GPRS             = new SMEM_LINK;
    g_pMemLink_GPRS->pNext      = NULL;
    g_pMemLink_GPRS->nLength    = BUFF_FILE_SIZE_GRPS;
    g_pMemLink_GPRS->pValue     = new BYTE[BUFF_FILE_SIZE_GRPS];

	if (g_bMustMerged_GPRS)
	{
		g_pASN1Global_GPRS        = Init_OSS_Lib_Final();
		g_pASN1Global_M_GPRS      = Init_OSS_Lib_Final_M();
		
		g_pBSRecord_GPRS          = (void*)new Fin_M_BSRecord;
		g_pBSSummaryRecord_GPRS   = (void*)new Fin_M_BSXferSummaryRecord;
	}
	else
	{		
		g_pASN1Global_GPRS        = Init_OSS_Lib_Ini();		
		g_pASN1Global_M_GPRS      = Init_OSS_Lib_Ini_M();
		
		g_pBSRecord_GPRS          = (void*)new Org_M_BSRecord;
		g_pBSSummaryRecord_GPRS   = (void*)new Org_M_BSXferSummaryRecord;	
	}

	return TRUE;
}
/*******************************************************************
�������ƣ�BOOL PreProcess_GPRS(char* szTmpFileName, STRING& sDestFileName)
����������Ԥ�������
���������
   ����1��char*   sTmpFileName,��Ҫת���Ļ����ļ���
   ����2: STRING& sDestFileName,ȱʡĿ��·��
���������
   ����1: STRING& sDestFileName,ʵ��Ŀ��·��
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE
���ú�����
����������
����ʱ�䣺maimaoshi,2002/12/29
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CR98_770PatchForMoto::PreProcess(const char* szTmpFileName, STRING& sDestFileName)
{
	UINT4  nCurOffset  = 0;  
    FILE*  pTmpFile    = NULL;	
	
	//���ļ�����ú��ʵĴ������Ļ�����
	pTmpFile = fopen(szTmpFileName, "rb");
    if (NULL == pTmpFile)
    {
        return FALSE;
    }
    else
    {
		//���������������
        g_nMemDataLen_GPRS = FileSize(pTmpFile);
        if(g_nMemFileLen_GPRS - sizeof(Fin_M_BSXferSummaryRecord) < g_nMemDataLen_GPRS)
        {
            g_nMemFileLen_GPRS = g_nMemDataLen_GPRS + sizeof(Fin_M_BSXferSummaryRecord);
            delete [] g_pMemFile_GPRS;
            g_pMemFile_GPRS = new BYTE[g_nMemFileLen_GPRS];
        }
        size_t nBytesRead = fread(g_pMemFile_GPRS, g_nMemDataLen_GPRS, 1, pTmpFile);
        fclose(pTmpFile);
        if (1 != nBytesRead)
        {
            return FALSE;
        }
    }

	UINT2 uPos = 0;	
	if ((uPos = sDestFileName.find_last_of("/")) != -1) 		 
	{		
        sDestFileName = sDestFileName.substr(0, uPos + 1);
	}
	
	return TRUE;
}
/*******************************************************************
�������ƣ�BOOL ProcessNotMerged_GPRS(char* szTmpFileName, char*& szDestFileName, UINT4 nChannelID)
���������������ϲ������
���������
   ����1��char*   szTmpFileName,��Ҫת���Ļ����ļ���
   ����2: char*&  szDestFileName,ȱʡĿ���ļ���
   ����3��UINT4   nChannelID,�����ļ�������ͨ����
���������
   ����1: char*&  szDestFileName,ʵ��Ŀ���ļ���
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE
���ú�����
����������
����ʱ�䣺maimaoshi,2002/12/23
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CR98_770PatchForMoto::ProcessNotMerged(const char* szTmpFileName,
                                            char*& szDestFileName,
                                            UINT4 nChannelID)
{
	//����Ԥ����
	STRING sDestFileName = szDestFileName;
	if (!PreProcess(szTmpFileName, sDestFileName))
	{
		return FALSE;
	}

	//����ͷ��Ҫ�ռ�������
	char  szCDRType[4];	
	UINT4 nLrsnStart      = 0;
	UINT4 nLrsnEnd        = 0;
	UINT4 nTotalCount     = 0;
	UINT4 nMissingCount   = 0;
	UINT4 nDuplicateCount = 0;
	UINT4 nSequence  = 0;

	//����ʱ�ļ��������
	UINT4 nDataLen        = 0;
	UINT4 nRecordLen      = 0;
    int   nCurOffset      = 0;
	BOOL  bIsFirst        = TRUE;
    UINT4 nFinBufUsed     = 0;
	int  nDecodeFailTime  = 0;

	Org_M_listOfCDR* pCurrent = NULL;
	Org_M_listOfCDR* pLast    = NULL;
    SMEM_LINK* pMemLink       = g_pMemLink_GPRS;
    pCurrent                  = (Org_M_listOfCDR*)(pMemLink->pValue);
	pCurrent->next            = NULL;

    time_t openTime;
    char   szOpenTime[14];
    //��õ�ǰʱ��,��Ϊ�ļ���ʱ��
    time(&openTime);
    tm tTmp = *localtime(&openTime);
    sprintf(szOpenTime,
            "%04d%02d%02d%02d%02d%02d",
            tTmp.tm_year + 1900,
            tTmp.tm_mon + 1,
            tTmp.tm_mday,
            tTmp.tm_hour,
            tTmp.tm_min,
            tTmp.tm_sec);

    while (nCurOffset < g_nMemDataLen_GPRS)
	{
		//��4���ֽڵĳ���
		memcpy(&nRecordLen, (BYTE*)g_pMemFile_GPRS + nCurOffset, 4);
        nRecordLen = NTOHL(nRecordLen);
		//��������Ļ�������Ϊ0������Ļ�����Ч
        if (0 == nRecordLen)
        {
            break;
        }
        else if ((g_nMemDataLen_GPRS - nCurOffset - 4) < nRecordLen)
        //ʣ������ݲ���һ�Ż�����������������Ч
        {
           break;
        }
		nCurOffset += 4;			
		
		//��ÿ�Ž��н���
		nDataLen = nRecordLen;    
        //��GPRS���ջ�������ASN.1���н��룬��uDataLen�з��ؽ����ĳ���
        int nRet = DeCodeBill_ini_mt(g_pASN1Global_GPRS, 
                                     (char*)g_pMemFile_GPRS + nCurOffset, 
                                     (int*)&nDataLen,
                                     (char*)&pCurrent->value, 
                                     pMemLink->nLength - nFinBufUsed - 4);
    
		nCurOffset += nRecordLen;

		//�������ʧ��
		if (ERR_SUCCESS != nRet)
		{
			if((nFinBufUsed != 0) && (nDecodeFailTime == 0))
			{
				//�л�������
				pMemLink->pNext  = new SMEM_LINK;
				pMemLink         = pMemLink->pNext;

				pMemLink->nLength = 1024*1024;
				pMemLink->pValue  = new BYTE[pMemLink->nLength];
				pMemLink->pNext   = NULL;

				pCurrent          = (Org_M_listOfCDR*)(pMemLink->pValue);
				pCurrent->next    = NULL;

				nCurOffset       -= nRecordLen + 4;
				nFinBufUsed       = 0;
			}
			nDecodeFailTime++;

			continue;
		}
		else
		{
			nDecodeFailTime = 0;
		}

        nFinBufUsed += nDataLen + int((char*)&pCurrent->value - (char*)pCurrent);

        if(pLast != NULL)
        {
			pLast->next = pCurrent;
        }

		pLast       = pCurrent;
		pLast->next = NULL;

        pCurrent = (Org_M_listOfCDR*)(pMemLink->pValue + nFinBufUsed);
        pCurrent->next = NULL;
            
		//����ÿ�Ż���
		Org_M_CallEventRecord* pTemp = (Org_M_CallEventRecord*)&pLast->value;
		switch (pTemp->choice)
		{
		case Org_M_sgsnPDPRecord_chosen:
			{
				//����ǵ�һ�Ż���
				if (bIsFirst)
				{
                    //��õ�һ�Ż��������к�nLrsnStart
					if (pTemp->u.sgsnPDPRecord.bit_mask & Org_M_SGSNPDPRecord_localSequenceNumber_present)
                    {
                        nLrsnStart = pTemp->u.sgsnPDPRecord.localSequenceNumber;		
                    }
					//��õ�һ�Ż�������szCDRType
					strcpy(szCDRType, "S");
					bIsFirst = FALSE;		
				}
                if (pTemp->u.sgsnPDPRecord.bit_mask & Org_M_SGSNPDPRecord_localSequenceNumber_present)
                {
                    nSequence = pTemp->u.sgsnPDPRecord.localSequenceNumber;		
                }
								
				break;
			}
		case Org_M_ggsnPDPRecord_chosen:
			{
				//����ǵ�һ�Ż���
				if (bIsFirst)
				{
                    //��õ�һ�Ż��������к�nLrsnStart
					if (pTemp->u.ggsnPDPRecord.bit_mask & Org_M_GGSNPDPRecord_localSequenceNumber_present)
                    {
                        nLrsnStart = pTemp->u.ggsnPDPRecord.localSequenceNumber;		
                    }
					//��õ�һ�Ż�������szCDRType
					strcpy(szCDRType, "G");
					bIsFirst = FALSE;		
				}
                if (pTemp->u.ggsnPDPRecord.bit_mask & Org_M_GGSNPDPRecord_localSequenceNumber_present)
                {
                    nSequence = pTemp->u.ggsnPDPRecord.localSequenceNumber;		
                }
							
				break;
			}
		case Org_M_sgsnMMRecord_chosen:
			{
				//����ǵ�һ�Ż���
				if (bIsFirst)
				{
                    //��õ�һ�Ż��������к�nLrsnStart
					if (pTemp->u.sgsnMMRecord.bit_mask & Org_M_SGSNMMRecord_localSequenceNumber_present)
                    {
                        nLrsnStart = pTemp->u.sgsnMMRecord.localSequenceNumber;	
                    }
					//��õ�һ�Ż�������szCDRType
					strcpy(szCDRType, "M");
					bIsFirst = FALSE;		
				}

                if (pTemp->u.sgsnMMRecord.bit_mask & Org_M_SGSNMMRecord_localSequenceNumber_present)
                {
                    nSequence = pTemp->u.sgsnMMRecord.localSequenceNumber;	
                }
				
				break;
			}
		case Org_M_sgsnSMORecord_chosen:
			{
				//����ǵ�һ�Ż���
				if (bIsFirst)
				{
					//��õ�һ�Ż��������к�nLrsnStart
					if (pTemp->u.sgsnSMORecord.bit_mask & Org_M_SGSNSMORecord_localSequenceNumber_present)
                    {
                        nLrsnStart = pTemp->u.sgsnSMORecord.localSequenceNumber;	
                    }
					//��õ�һ�Ż�������szCDRType
					strcpy(szCDRType, "SMO");
					bIsFirst = FALSE;		
				}

				if (pTemp->u.sgsnSMORecord.bit_mask & Org_M_SGSNSMORecord_localSequenceNumber_present)
                {
                    nSequence = pTemp->u.sgsnSMORecord.localSequenceNumber;	
                }			
				break;
			}
		case Org_M_sgsnSMTRecord_chosen:
			{
				//����ǵ�һ�Ż���
				if (bIsFirst)
				{
					//��õ�һ�Ż��������к�nLrsnStart
					if (pTemp->u.sgsnSMTRecord.bit_mask & Org_M_SGSNSMTRecord_localSequenceNumber_present)
                    {
                        nLrsnStart = pTemp->u.sgsnSMTRecord.localSequenceNumber;	
                    }
					//��õ�һ�Ż�������szCDRType
					strcpy(szCDRType, "SMT");
					bIsFirst = FALSE;		
				}

                if (pTemp->u.sgsnSMTRecord.bit_mask & Org_M_SGSNSMTRecord_localSequenceNumber_present)
                {
                    nSequence = pTemp->u.sgsnSMTRecord.localSequenceNumber;	
                }
							
				break;
			}
		default:
			break;
		}
		nTotalCount++;
	}

	nLrsnEnd = nSequence;

	//���ʵ���ļ���
	//by ldf ��Ӧ���ⵥD20358
	char szBsXferFile[34];
	char szIPAddr[40];
	sprintf(szIPAddr, "%x", inet_addr(g_szGsnID_GPRS));
	
	if( g_bCMCC_GPRS )
	{
        //by ldf 2003-08-07  ��Ӧ���ⵥSWPD00468 
        char szSeqenceNumber[10];
        STRING  sTmpFileName = szTmpFileName;
        UINT2 uPos1 = sTmpFileName.find_first_of("_") ;
        UINT2 uPos2 = sTmpFileName.find_last_of(".") ;
        UINT2 uLen = uPos2 - uPos1;
        if (uLen > 6)
        {
            //substr(1,uPos+1),�õ�ȥ����ǰ���b��.���������
            //(���ﴫ�������ʱ�ļ�������01_00001.tmp)
            sprintf( szSeqenceNumber,"%s",(sTmpFileName.substr(uPos2-6, 6)).c_str());
        }
        else
        {
            STRING sTmpZero = "0"; 
            UINT2 uTmpZeroLen = uLen;
            for( uTmpZeroLen; uTmpZeroLen < 6 ; uTmpZeroLen++)
            {
                sTmpZero = sTmpZero + "0"; 
            }
            sprintf(szSeqenceNumber,
                    "%s%s",
                    sTmpZero.c_str(),
                    sTmpFileName.substr(uPos1+1,uLen-1).c_str());

        }

        sprintf(szBsXferFile, "%s_%s_%s.btf",szIPAddr,szOpenTime,szSeqenceNumber);
    }
    else
    {
        sprintf(szBsXferFile, "%s_%s.btf", szIPAddr, szOpenTime);
    }
    //end

	//ʵ�ʻ�����·��
	sDestFileName = sDestFileName + szBsXferFile;

    szDestFileName = new char[sDestFileName.length() + 1];
    strcpy(szDestFileName, sDestFileName.c_str());

	Org_M_BSXferSummaryRecord* pSummary = (Org_M_BSXferSummaryRecord*)g_pBSSummaryRecord_GPRS;
	
	//***********************��д��ͷ��Ϣ*********************************//
	//��дGSN��ַ:cgnID
	sprintf((char*)pSummary->cgnID.value, "%s", g_szGsnID_GPRS);
	pSummary->cgnID.length = strlen((char*)pSummary->cgnID.value);	
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->cgnID.value, (15-pSummary->cgnID.length));
	pSummary->cgnID.length = 15;
	//end
	
	//��д�ļ���:bsXferFile
	strcpy((char*)pSummary->bsXferFile.value, szBsXferFile);
	pSummary->bsXferFile.length = strlen(szBsXferFile);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->bsXferFile.value, (34-pSummary->bsXferFile.length));
	pSummary->bsXferFile.length = 34;
	//end

	//��д�ļ�����ʱ��:openTime
	strcpy((char*)pSummary->openTime.value, szOpenTime);
	pSummary->openTime.length = strlen(szOpenTime);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->openTime.value, (14-pSummary->openTime.length));
	pSummary->openTime.length = 14;
	//end

    time_t closeTime;
    char   szCloseTime[14];
    //��õ�ǰʱ��,��Ϊ�ļ���ʱ��
    time(&closeTime);
    tTmp = *localtime(&closeTime);
    sprintf(szCloseTime,
            "%04d%02d%02d%02d%02d%02d",
            tTmp.tm_year + 1900,
            tTmp.tm_mon + 1,
            tTmp.tm_mday,
            tTmp.tm_hour,
            tTmp.tm_min,
            tTmp.tm_sec);

    //��д�ļ��ر�ʱ��:closeTime
    strcpy((char*)pSummary->closeTime.value, szCloseTime);
    pSummary->closeTime.length = strlen(szCloseTime);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->closeTime.value, (14-pSummary->closeTime.length));
	pSummary->closeTime.length = 14;
	//end

	//��д��ʼ�ı��ؼ�¼���к�:lrsnStart
	sprintf((char*)pSummary->lrsnStart.value, "%u", nLrsnStart);
	pSummary->lrsnStart.length = strlen((char*)pSummary->lrsnStart.value);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->lrsnStart.value, (10-pSummary->lrsnStart.length));
	pSummary->lrsnStart.length = 10;
	//end

	//��д�����ı��ؼ�¼���к�:lrsnEnd
	sprintf((char*)pSummary->lrsnEnd.value, "%u", nLrsnEnd);
	pSummary->lrsnEnd.length = strlen((char*)pSummary->lrsnEnd.value);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->lrsnEnd.value, (10-pSummary->lrsnEnd.length));
	pSummary->lrsnEnd.length = 10;
	//end

	//��д��������:cdrType
	sprintf((char*)pSummary->cdrType.value, "%s", szCDRType);
	pSummary->cdrType.length = strlen((char*)pSummary->cdrType.value);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->cdrType.value, (3-pSummary->cdrType.length));
	pSummary->cdrType.length = 3;
	//end

	//��д���л�������:totalCdr
	sprintf((char*)pSummary->totalCdr.value, "%u", nTotalCount);
	pSummary->totalCdr.length = strlen((char*)pSummary->totalCdr.value);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->totalCdr.value, (10-pSummary->totalCdr.length));
	pSummary->totalCdr.length = 10;
	//end
	
	//��д��ʧ�Ļ�������:missingCdr
	sprintf((char*)pSummary->missingCdr.value, "%u", nMissingCount);
	pSummary->missingCdr.length = strlen((char*)pSummary->missingCdr.value);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->missingCdr.value, (10-pSummary->missingCdr.length));
	pSummary->missingCdr.length = 10;
	//end

	//��д�ظ��Ļ�������:duplicateCdr
	sprintf((char*)pSummary->duplicateCdr.value, "%u", nDuplicateCount);
	pSummary->duplicateCdr.length = strlen((char*)pSummary->duplicateCdr.value);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->duplicateCdr.value, (10-pSummary->duplicateCdr.length));
	pSummary->duplicateCdr.length = 10;
	//end
	
	//��дCG��ַ:cgID
	sprintf((char*)pSummary->cgID.value, "%s", g_szCgID_GPRS);
	pSummary->cgID.length = strlen((char*)pSummary->cgID.value);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->cgID.value, (15-pSummary->cgID.length));
	pSummary->cgID.length = 15;
	//end

	//���ṹ��ͷ
	((Org_M_BSRecord*)g_pBSRecord_GPRS)->bsXferRecord = *pSummary;
	((Org_M_BSRecord*)g_pBSRecord_GPRS)->listOfCDR    = (Org_M_listOfCDR*)g_pMemLink_GPRS->pValue;

	//����	
	char* pEncodedData = NULL;
	int   nBufferLen   = g_nMemFileLen_GPRS;

    if ((pEncodedData = EnCodeBill_ini_M_mt(g_pASN1Global_M_GPRS,
                                            (char*)g_pBSRecord_GPRS,
                                            (char*)g_pMemFile_GPRS,
                                            &nBufferLen))  != NULL)
    {
        FILE* pFile = fopen(szDestFileName, "wb");

        if (pFile == NULL)
        {
			TRACE(MTS_BACKSAVE, 
                  S_BS_FAIL_OPEN_FILE, szDestFileName);
/*
			MSGOUT(MTS_BACKSAVE, MSGOUT_LEVEL_URGENT,
                   S_BS_FAIL_OPEN_FILE, szDestFileName);
*/
            return FALSE;
        }

        if (fwrite(pEncodedData, nBufferLen, 1, pFile) != 1)
        {
			TRACE(MTS_BACKSAVE,
                  S_BS_FAIL_WRITE_FILE, szDestFileName);
/*
			MSGOUT(MTS_BACKSAVE, MSGOUT_LEVEL_URGENT,
                   S_BS_FAIL_WRITE_FILE, szDestFileName);
*/
            fclose(pFile);
            return FALSE;
        }
        fclose(pFile);
    }
    else
    {
		TRACE(MTS_BACKSAVE,
              S_ENCODE_FAILED);
/*
		MSGOUT(MTS_BACKSAVE, MSGOUT_LEVEL_URGENT,
               S_ENCODE_FAILED);
*/
        return FALSE;
    }

    return TRUE;
}

/*******************************************************************
�������ƣ�BOOL ProcessMerged_GPRS(char* szTmpFileName, char*& szDestFileName, UINT4 nChannelID)
��������������ϲ������
���������
   ����1��char*   szTmpFileName,��Ҫת���Ļ����ļ���
   ����2: char*&  szDestFileName,ȱʡĿ���ļ���
   ����3��UINT4   nChannelID,�����ļ�������ͨ����
���������
   ����1: char*&  szDestFileName,ʵ��Ŀ���ļ���
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE
���ú�����
����������
����ʱ�䣺maimaoshi,2002/12/23
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CR98_770PatchForMoto::ProcessMerged(const char* szTmpFileName,
                                         char*& szDestFileName,
                                         UINT4 nChannelID)
{
	//����Ԥ����
	STRING sDestFileName = szDestFileName;
	if (!PreProcess(szTmpFileName, sDestFileName))
	{
		return FALSE;
	}

	//����ͷ��Ҫ�ռ�������	
	char  szCDRType[4];		
	UINT4 nLrsnStart      = 0;
    UINT4  nLrsnEnd       = 0;
	UINT4 nTotalCount     = 0;
	UINT4 nMissingCount   = 0;
	UINT4 nDuplicateCount = 0;
	UINT4  nSequence      = 0;

	//����ʱ�ļ��������	
	UINT4 nDataLen        = 0;
	UINT4 nRecordLen      = 0;
    int   nCurOffset      = 0;
	BOOL  bIsFirst        = TRUE;
    UINT4 nFinBufUsed     = 0;
	int  nDecodeFailTime  = 0;

	Fin_M_listOfCDR* pCurrent = NULL;
	Fin_M_listOfCDR* pLast    = NULL;
    SMEM_LINK* pMemLink       = g_pMemLink_GPRS;
    pCurrent                  = (Fin_M_listOfCDR*)(pMemLink->pValue);
	pCurrent->next            = NULL;
	   
	 //added by zkq  2004-06-10
	//�ж��Ƿ�Ϊ"exclude,excluded"
	int nStart = -1;
	int nEnd   = -1;
	int nSeq   = -1;
	//���Ŀ��Ŀ¼���ļ������жϼƷ�����	
	char szTmp[MAX_PATH];
	char* szPos = NULL;
	strcpy(szTmp,szDestFileName);	
	szCDRType[0] = 0;
    
    if ((szPos = strrchr(szTmp,'/')) != NULL)
    {
       szPos[0] = 0;  //�����һ��'/' ��Ϊ 0
	   if ((szPos = strrchr(szTmp,'/')) != NULL)
	   {
		   if (StringiCmp(szPos + 1, "hotbilling"))
		   {
			   strcpy(szCDRType, "H");// hotbilling 
		   }
		   if (StringiCmp(szPos + 1, "flatrate"))			   
		   {
			  strcpy(szCDRType,"F");// flatrate;			  
		   }
		   if (StringiCmp(szPos + 1,"prepaidservice"))
		   {
			   strcpy(szCDRType, "P");// prepaidservice			   
		   }
		   if (StringiCmp(szPos + 1,"normalbilling"))
		   {
			   strcpy(szCDRType,"N");// normalbilling			  
		   }		   
	   }
    }  
	szPos = NULL;
	//end
    time_t openTime;
    char   szOpenTime[14];
    //��õ�ǰʱ��,��Ϊ�ļ���ʱ��
    time(&openTime);
    tm tTmp = *localtime(&openTime);
    sprintf(szOpenTime,
            "%04d%02d%02d%02d%02d%02d",
            tTmp.tm_year + 1900,
            tTmp.tm_mon + 1,
            tTmp.tm_mday,
            tTmp.tm_hour,
            tTmp.tm_min,
            tTmp.tm_sec);

    while (nCurOffset < g_nMemDataLen_GPRS)
	{
		//��4���ֽڵĳ���
		memcpy(&nRecordLen, (BYTE*)g_pMemFile_GPRS + nCurOffset, 4);
        nRecordLen = NTOHL(nRecordLen);
		//��������Ļ�������Ϊ0������Ļ�����Ч
        if (0 == nRecordLen)
        {
            break;
        }
        else if ((g_nMemDataLen_GPRS - nCurOffset - 4) < nRecordLen)
        //ʣ������ݲ���һ�Ż�����������������Ч
        {
           break;
        }
		nCurOffset += 4;			
		
		//��ÿ�Ž��н���
		nDataLen = nRecordLen;    
        //��GPRS��������ASN.1���н��룬��uDataLen�з��ؽ����ĳ���
        int nRet = DeCodeBill_final_mt(g_pASN1Global_GPRS, 
                                       (char*)g_pMemFile_GPRS + nCurOffset, 
                                       (int*)&nDataLen,
                                       (char*)&pCurrent->value, 
                                       pMemLink->nLength - nFinBufUsed - 4);
    
		nCurOffset += nRecordLen;
		
		//�������ʧ��
		if (ERR_SUCCESS != nRet)
		{
			if((nFinBufUsed != 0) && (nDecodeFailTime == 0))
			{
				//�л�������
				pMemLink->pNext  = new SMEM_LINK;
				pMemLink         = pMemLink->pNext;

				pMemLink->nLength = 1024*1024;
				pMemLink->pValue  = new BYTE[pMemLink->nLength];
				pMemLink->pNext   = NULL;

				pCurrent          = (Fin_M_listOfCDR*)(pMemLink->pValue);
				pCurrent->next    = NULL;

				nCurOffset       -= nRecordLen + 4;
				nFinBufUsed       = 0;
			}
			nDecodeFailTime++;

			continue;
		}
		else
		{
			nDecodeFailTime = 0;
		}

        nFinBufUsed += nDataLen + int((char*)&pCurrent->value - (char*)pCurrent);

        if(pLast != NULL)
        {
			pLast->next = pCurrent;
        }

		pLast       = pCurrent;
		pLast->next = NULL;

        pCurrent = (Fin_M_listOfCDR*)(pMemLink->pValue + nFinBufUsed);
        pCurrent->next = NULL;
            
		//����ÿ�Ż���
		Fin_M_CallEventRecord* pTemp = (Fin_M_CallEventRecord*)&pLast->value;
		switch (pTemp->choice)
		{
		case Fin_M_sgsnPDPRecord_chosen:
			{
				//����ǵ�һ�Ż���
				if (bIsFirst)
				{
                    //added by zkq 2004-06-10
                    //��õ�һ�Ż�������szCDRType�����Ƿ�ǼƷ����� 
                    if(0 == szCDRType[0])
                    {
                        strcpy(szCDRType, "S");
                    }			
                    bIsFirst = FALSE;		
				}				
				//end                    
				break;
			}
		case Fin_M_ggsnPDPRecord_chosen:
			{
				//����ǵ�һ�Ż���
				if (bIsFirst)
				{
                    //added by zkq 2004-06-10
                    //��õ�һ�Ż�������szCDRType�����Ƿ�ǼƷ����� 
                    if(0 == szCDRType[0])
                    {
                        strcpy(szCDRType, "G");
                    }				
                    //end   
					bIsFirst = FALSE;		
				}			
				break;
			}
		case Fin_M_sgsnMMRecord_chosen:
			{
				//����ǵ�һ�Ż���
				if (bIsFirst)
				{
					//��õ�һ�Ż��������к�nLrsnStart
                    if (pTemp->u.sgsnSMORecord.bit_mask & Fin_M_SGSNMMRecord_localSequenceNumber_present)
                    {
						nStart = 1;  //added by zkq 2004-06-10
                        nLrsnStart = pTemp->u.sgsnSMORecord.localSequenceNumber;
                    }
                    //added by zkq 2004-06-10
                    //��õ�һ�Ż�������szCDRType�����Ƿ�ǼƷ����� 
                    if(0 == szCDRType[0])
                    {
                        strcpy(szCDRType, "M");
                    }					
                    //end   				
					bIsFirst = FALSE;		
				}

                if (pTemp->u.sgsnSMORecord.bit_mask & Fin_M_SGSNMMRecord_localSequenceNumber_present)
                {
					nSeq = 1;
                    nSequence = pTemp->u.sgsnSMORecord.localSequenceNumber;
                }
				break;
			}
		case Fin_M_sgsnSMORecord_chosen:
			{
				//����ǵ�һ�Ż���
				if (bIsFirst)
				{
					//��õ�һ�Ż��������к�nLrsnStart
                    if (pTemp->u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_localSequenceNumber_present)
                    {
						nStart = 1;
                        nLrsnStart = pTemp->u.sgsnSMORecord.localSequenceNumber;
                    }
                    //added by zkq 2004-06-10
                    //��õ�һ�Ż�������szCDRType�����Ƿ�ǼƷ����� 
                    if(0 == szCDRType[0])
                    {
                        strcpy(szCDRType, "SMO");
                    }					
                    //end   
					bIsFirst = FALSE;		
				}
                if (pTemp->u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_localSequenceNumber_present)
                {
					nSeq = 1;
                    nSequence = pTemp->u.sgsnSMORecord.localSequenceNumber;
                }

                break;
			}
		case Fin_M_sgsnSMTRecord_chosen:
			{
				//����ǵ�һ�Ż���
				if (bIsFirst)
				{
					//��õ�һ�Ż��������к�nLrsnStart
					if (pTemp->u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_localSequenceNumber_present)
                    {
						nStart = 1; //aded by zkq 2004-06-10
                        nLrsnStart = pTemp->u.sgsnSMTRecord.localSequenceNumber;
                    }
                    //added by zkq 2004-06-10
                    //��õ�һ�Ż�������szCDRType�����Ƿ�ǼƷ����� 
                    if(0 == szCDRType[0])
                    {
                        strcpy(szCDRType, "SMT");
                    }					
                    //end   				
					bIsFirst = FALSE;		
				}

				if (pTemp->u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_localSequenceNumber_present)
                {
					nSeq = 1;
                    nSequence = pTemp->u.sgsnSMTRecord.localSequenceNumber;
                }				
				break;
			}
		default:
			break;
		}
		
		nTotalCount++;		
	}

    if(nSeq == 1)//added by zkq 2004-06-10
    {
        nEnd = 1;
        nLrsnEnd = nSequence;
    }//end

	//���ʵ���ļ���
	//by ldf ��Ӧ���ⵥD20358
	char szBsXferFile[34];
	char szIPAddr[40];
	sprintf(szIPAddr, "%x", inet_addr(g_szGsnID_GPRS));
	
	if( g_bCMCC_GPRS )
	{
        //by ldf 2003-08-07 ��Ӧ���ⵥSWPD00468 
        char szSeqenceNumber[10];
        STRING  sTmpFileName = szTmpFileName;
        UINT2 uPos1 = sTmpFileName.find_first_of("_") ;
        UINT2 uPos2 = sTmpFileName.find_last_of(".") ;
        UINT2 uLen = uPos2 - uPos1;
        if (uLen > 6)
        {
            //substr(1,uPos+1),�õ�ȥ����ǰ���b��.���������
            //(���ﴫ�������ʱ�ļ�������01_00001.tmp)
            sprintf( szSeqenceNumber,"%s",(sTmpFileName.substr(uPos2-6, 6)).c_str());
        }
        else
        {
            STRING sTmpZero = "0"; 
            UINT2 uTmpZeroLen = uLen;
            for( uTmpZeroLen; uTmpZeroLen < 6 ; uTmpZeroLen++)
            {
                sTmpZero = sTmpZero + "0"; 
            }
            sprintf(szSeqenceNumber,
                    "%s%s",
                    sTmpZero.c_str(),
                    sTmpFileName.substr(uPos1+1,uLen-1).c_str());

        }

        sprintf(szBsXferFile, "%s_%s_%s.btf",szIPAddr,szOpenTime,szSeqenceNumber);
    }
    else
    {
        sprintf(szBsXferFile, "%s_%s.btf", szIPAddr, szOpenTime);
    }
    //end
	
	
	//ʵ�ʻ�����·��
	sDestFileName = sDestFileName + szBsXferFile;

    szDestFileName = new char[sDestFileName.length() + 1];
    strcpy(szDestFileName, sDestFileName.c_str());

	Fin_M_BSXferSummaryRecord* pSummary = (Fin_M_BSXferSummaryRecord*)g_pBSSummaryRecord_GPRS;
	
	//***********************��д��ͷ��Ϣ*********************************//
	//��дGSN��ַ:cgnID
	sprintf((char*)pSummary->cgnID.value, "%s", g_szGsnID_GPRS);
	pSummary->cgnID.length = strlen((char*)pSummary->cgnID.value);	
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->cgnID.value, (15-pSummary->cgnID.length));
	pSummary->cgnID.length = 15;
	//end

	//��д�ļ���:bsXferFile
	strcpy((char*)pSummary->bsXferFile.value, szBsXferFile);
	pSummary->bsXferFile.length = strlen(szBsXferFile);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->bsXferFile.value, (34-pSummary->bsXferFile.length));
	pSummary->bsXferFile.length = 34;
	//end
	
	//��д�ļ�����ʱ��:openTime
	strcpy((char*)pSummary->openTime.value, szOpenTime);
	pSummary->openTime.length = strlen(szOpenTime);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->openTime.value, (14-pSummary->openTime.length));
	pSummary->openTime.length = 14;
	//end
	
    time_t closeTime;
    char   szCloseTime[14];
    //��õ�ǰʱ��,��Ϊ�ļ���ʱ��
    time(&closeTime);
    tTmp = *localtime(&closeTime);
    sprintf(szCloseTime,
            "%04d%02d%02d%02d%02d%02d",
            tTmp.tm_year + 1900,
            tTmp.tm_mon + 1,
            tTmp.tm_mday,
            tTmp.tm_hour,
            tTmp.tm_min,
            tTmp.tm_sec);

    //��д�ļ��ر�ʱ��:closeTime
    strcpy((char*)pSummary->closeTime.value, szCloseTime);
    pSummary->closeTime.length = strlen(szCloseTime);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->closeTime.value, (14-pSummary->closeTime.length));
	pSummary->closeTime.length = 14;
	//end
	
    //��д��ʼ�ı��ؼ�¼���к�:lrsnStart
    if(nStart == -1) //modify by zkq 2004-06-10
    {
        sprintf((char*)pSummary->lrsnStart.value, "%s", "Exclude");
    }
    else
    {
        sprintf((char*)pSummary->lrsnStart.value, "%u", nLrsnStart);
    }		
    pSummary->lrsnStart.length = strlen((char*)pSummary->lrsnStart.value);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
    FillChar((char*)pSummary->lrsnStart.value, (10-pSummary->lrsnStart.length));
    pSummary->lrsnStart.length = 10;
    //end
		
    //��д�����ı��ؼ�¼���к�:lrsnEnd
    if(nEnd == -1)   //modify by zkq 2004-06-10
    {
        sprintf((char*)pSummary->lrsnEnd.value, "%s", "Excluded");
    }
    else
    {
        sprintf((char*)pSummary->lrsnEnd.value, "%u", nLrsnEnd);
    }		
    pSummary->lrsnEnd.length = strlen((char*)pSummary->lrsnEnd.value);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
    FillChar((char*)pSummary->lrsnEnd.value, (10-pSummary->lrsnEnd.length));
    pSummary->lrsnEnd.length = 10;
    //end


	//��д��������:cdrType
	sprintf((char*)pSummary->cdrType.value, "%s", szCDRType);
	pSummary->cdrType.length = strlen((char*)pSummary->cdrType.value);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->cdrType.value, (3-pSummary->cdrType.length));
	pSummary->cdrType.length = 3;
	//end
	
	//��д���л�������:totalCdr
	sprintf((char*)pSummary->totalCdr.value, "%u", nTotalCount);
	pSummary->totalCdr.length = strlen((char*)pSummary->totalCdr.value);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->totalCdr.value, (10-pSummary->totalCdr.length));
	pSummary->totalCdr.length = 10;
	//end
	
	//��д��ʧ�Ļ�������:missingCdr
	sprintf((char*)pSummary->missingCdr.value, "%u", nMissingCount);
	pSummary->missingCdr.length = strlen((char*)pSummary->missingCdr.value);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->missingCdr.value, (10-pSummary->missingCdr.length));
	pSummary->missingCdr.length = 10;
	//end
	
	//��д�ظ��Ļ�������:duplicateCdr
	sprintf((char*)pSummary->duplicateCdr.value, "%u", nDuplicateCount);
	pSummary->duplicateCdr.length = strlen((char*)pSummary->duplicateCdr.value);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->duplicateCdr.value, (10-pSummary->duplicateCdr.length));
	pSummary->duplicateCdr.length = 10;
	//end
	
	//��дCG��ַ:cgID
	sprintf((char*)pSummary->cgID.value, "%s", g_szCgID_GPRS);
	pSummary->cgID.length = strlen((char*)pSummary->cgID.value);
    //by ldf ��Ӧ���ⵥD20379  2003-02-21
	FillChar((char*)pSummary->cgID.value, (15-pSummary->cgID.length));
	pSummary->cgID.length = 15;
	//end
	
	//���ṹ��ͷ
	((Fin_M_BSRecord*)g_pBSRecord_GPRS)->bsXferRecord = *pSummary;
	((Fin_M_BSRecord*)g_pBSRecord_GPRS)->listOfCDR    = (Fin_M_listOfCDR*)g_pMemLink_GPRS->pValue;

	//����
	char* pEncodedData = NULL;
	
    //modify by zkq 2004-07-08
    
    int  nBufferLen = g_nMemDestFileLen ;
    if ((pEncodedData = EnCodeBill_final_M_mt(g_pASN1Global_M_GPRS,
                                              (char*)g_pBSRecord_GPRS,
                                              (char*)g_pMemDestFile,
                                              &nBufferLen)) == NULL)
    {
    	delete [] g_pMemDestFile;
        g_pMemDestFile = NULL; 
        g_nMemDestFileLen = 0;       
        nBufferLen = 0;
        if ((pEncodedData = EnCodeBill_final_M_mt(g_pASN1Global_M_GPRS,
                                               (char*)g_pBSRecord_GPRS,
                                               (char*)g_pMemDestFile,
                                                     &nBufferLen))== NULL)
        {//����ʧ��      
            
            TRACE(MTS_BACKSAVE, S_ENCODE_FAILED);
/*
                MSGOUT(MTS_BACKSAVE, MSGOUT_LEVEL_URGENT, S_ENCODE_FAILED);
*/
            return FALSE;
        }
        g_pMemDestFile = (BYTE *)pEncodedData;
        g_nMemDestFileLen = nBufferLen; //����g_pMemDestFile�ķ��䳤��
    }
    
    FILE* pFile = fopen(szDestFileName, "wb");
    if (pFile == NULL)
    {
        TRACE(MTS_BACKSAVE, S_BS_FAIL_OPEN_FILE, szDestFileName);
/*
        MSGOUT(MTS_BACKSAVE, MSGOUT_LEVEL_URGENT, S_BS_FAIL_OPEN_FILE, szDestFileName);
*/
        return FALSE;
    }
    if (fwrite(pEncodedData, nBufferLen, 1, pFile) != 1)
    {
        TRACE(MTS_BACKSAVE, S_BS_FAIL_WRITE_FILE, szDestFileName);
/*
        MSGOUT(MTS_BACKSAVE, MSGOUT_LEVEL_URGENT, S_BS_FAIL_WRITE_FILE, szDestFileName);
*/
        fclose(pFile);
        return FALSE;
    }
    fclose(pFile);
    //end 
	return TRUE;
}

/*******************************************************************
�������ƣ�BOOL Process_GPRS(char* szTmpFileName, char*& szDsetFileName, UINT4 nChannelID)
������������̬�⴦������Ľӿ�ʵ�ֺ���
���������
   ����1��char*   szTmpFileName,��Ҫת���Ļ����ļ���
   ����2: char*&  szDsetFileName,ȱʡĿ���ļ���
   ����3��UINT4   nChannelID,�����ļ�������ͨ����
���������
   ����1: char*& sDsetFileName,ʵ��Ŀ���ļ���
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE
���ú�����
   ����1��ProcessMerged(),����ϲ������;
   ����2��ProcessMerged(),�����ϲ������;
����������
����ʱ�䣺maimaoshi,2002/12/23
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CR98_770PatchForMoto::Process(const char* szTmpFileName,
                                   char*& szDsetFileName,
                                   UINT4 nChannelID)
{
	if (g_bMustMerged_GPRS)
	{
		return ProcessMerged(szTmpFileName,szDsetFileName,nChannelID);
	}
	else
	{
		return ProcessNotMerged(szTmpFileName,szDsetFileName,nChannelID);
	}

}

void CR98_770PatchForMoto::DeleteMemLink(SMEM_LINK* p)
{
    if(p->pNext != NULL)
        DeleteMemLink(p->pNext);
    else
    {
        delete [] p->pValue;
        delete p;
    }
}

/*******************************************************************
�������ƣ�extern "C" BOOL Terminate_GPRS()
������������̬���������Ľӿ�ʵ�ֺ���
���������
�����������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE
���ú�����
����������
����ʱ�䣺maimaoshi,2002/12/23
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CR98_770PatchForMoto::Terminate()
{
    if (g_pMemFile_GPRS != NULL)
    {
        delete[] g_pMemFile_GPRS;
    }

	if (g_pMemLink_GPRS != NULL)
    {
        DeleteMemLink(g_pMemLink_GPRS);
    }

    if (g_pBSSummaryRecord_GPRS != NULL)
	{
		delete[] g_pBSSummaryRecord_GPRS;
		g_pBSSummaryRecord_GPRS = NULL;
	}

	if (g_pBSRecord_GPRS != NULL)
	{
		delete[] g_pBSRecord_GPRS;
		g_pBSRecord_GPRS = NULL;
	}	
    if(g_pMemDestFile != NULL)  //add by zkq 2004-07-08
    {
        delete[] g_pMemDestFile;
        g_pMemDestFile = NULL;
    }//end           

	if (g_bMustMerged_GPRS)
	{
		Release_OSS_Lib_Final(g_pASN1Global_GPRS);
		g_pASN1Global_GPRS = NULL;

		Release_OSS_Lib_Final_M(g_pASN1Global_M_GPRS);
		g_pASN1Global_M_GPRS = NULL;
	}
	else
	{
		Release_OSS_Lib_Ini(g_pASN1Global_GPRS);
		g_pASN1Global_GPRS = NULL;

		Release_OSS_Lib_Ini_M(g_pASN1Global_M_GPRS);
		g_pASN1Global_M_GPRS = NULL;
	}
	
	return TRUE;
}

