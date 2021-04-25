#include "../include/toolbox.h"
#include "../r4_440/asn1_r4/CDRF_3GPPR4_Org.h"
#include "../r4_440/asn1_r4/CDRF_3GPPR4_Fin.h"
#include "../r4_440/asn1_r4/oss_asn1_r4.h"
#include "asn1_r4/CDRF_3GPPR4_Org_M.h"
#include "asn1_r4/CDRF_3GPPR4_Fin_M.h"
#include "asn1_r4/oss_asn1_r4.h"
#include "r4_440_m.h"
#include "../include/export_dll.h"

#define R4_BILL_FILE_SIZE       (1024 * 1024 * 1.5) //用于存放话单文件的内存空间的大小
#define R4_BUFF_FILE_SIZE       (1024 * 1024 * 1)   //用于存放话单Value数据的内存空间的大小

char  CR4_440PatchForMoto::g_szCgID[15];           //CG的地址
char  CR4_440PatchForMoto::g_szGsnID[15];          //Gsn的地址
BOOL  CR4_440PatchForMoto::g_bMustMerged;          //话单是否合并
BOOL  CR4_440PatchForMoto::g_bCMCC;                //Motorola是否用在CMCC

BYTE* CR4_440PatchForMoto::g_pMemFile;             //内存映射文件指针
UINT4 CR4_440PatchForMoto::g_nMemFileLen;          //内存映射文件的长度
UINT4 CR4_440PatchForMoto::g_nMemDataLen;          //内存映射文件有效数据的长度

void* CR4_440PatchForMoto::g_pASN1Global;          //用于进行对合并的话单进行解码的环境变量
void* CR4_440PatchForMoto::g_pASN1Global_M;        //用于进行对不合并的话单进行解码的环境变量

void* CR4_440PatchForMoto::g_pBSRecord;            //M的话单文件结构
void* CR4_440PatchForMoto::g_pBSSummaryRecord;     //M的话单文件结构的表头部分

//add by zkq 2004-07-08
BYTE * CR4_440PatchForMoto::g_pMemDestFile;        //解码内存映射文件指针 
int    CR4_440PatchForMoto::g_nMemDestFileLen;          //解码内存映射文件的长度
//end
SMEM_LINK* CR4_440PatchForMoto::g_pMemLink = NULL;

extern "C" IGWB_Svc_Export BOOL PC_Init(const char* szConfigPath,
                                        BYTE nApID,
                                        UINT4 nChannelCount)
{
    return CR4_440PatchForMoto::Init(szConfigPath, nApID, nChannelCount);
}
extern "C" IGWB_Svc_Export BOOL PC_Process(const char* szFileName,
                                           char*& szDestFileName,
                                           UINT4 nChannelNum)
{
    return CR4_440PatchForMoto::Process(szFileName, szDestFileName, nChannelNum);
}
extern "C" IGWB_Svc_Export BOOL PC_Terminate()
{
    return CR4_440PatchForMoto::Terminate();
}

/*******************************************************************
函数名称：extern "C" BOOL Init()
功能描述：动态库初始化输出的接口实现函数
输入参数：
输出参数：无
返回内容：成功返回TRUE,失败返回FALSE
调用函数：
被调函数：
作者时间：Guofeng, Guan, 2003-06-02
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
BOOL CR4_440PatchForMoto::Init(const char* szConfigPath,
                               BYTE nApID,
                               UINT4 nChannelCount)
{
    g_bMustMerged        = TRUE;

    g_pMemFile           = NULL;

    g_pASN1Global        = NULL;
    g_pASN1Global_M      = NULL;

    g_pBSRecord          = NULL;
    g_pBSSummaryRecord   = NULL;

    g_pMemDestFile       = NULL; //add by zkq 2004-07-08
    g_nMemDestFileLen    = 0; //add by zkq 2004-07-08
    //打开配置文件
    CINIFile *pIniFile;
    pIniFile = new CINIFile(szConfigPath);
    if(pIniFile->Open() == FALSE)
    {
        delete pIniFile;
        return FALSE;
    }

    //读取本进程对应的接入点号
    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP, nApID);

    //读取CG的IP地址
    char szLocalIp[20];
    pIniFile->GetString(strSec,
                        CFG_AP_KEY_LOCAL_IP_TO_PM,
                        CFG_AP_KEY_LOCAL_IP_TO_PM_DEFAULT,
                        szLocalIp,
                        sizeof(szLocalIp));
    strcpy(g_szCgID, szLocalIp);

    //读取Gsn的IP地址
    char szGsnIp[20];
    pIniFile->GetString(strSec,
                        CFG_AP_KEY_GSNIP,
                        CFG_AP_KEY_GSNIP_DEFAULT,
                        szGsnIp,
                        sizeof(szGsnIp));
    strcpy(g_szGsnID, szGsnIp);

    //读取是否合并标志
    g_bMustMerged = pIniFile->GetInt(strSec,
                                     CFG_AP_KEY_MUST_MERGE,
                                     CFG_AP_KEY_MUST_MERGE_DEFAULT);

    //读取是针对CMCC与否
    g_bCMCC = pIniFile->GetInt(strSec,
                               CFG_AP_KEY_USEIN_CMCC,
                               CFG_AP_KEY_USEIN_CMCC_DEFAULT);

    pIniFile->Close();
    delete pIniFile;

    g_nMemFileLen          = R4_BILL_FILE_SIZE;
    g_pMemFile             = new BYTE[R4_BILL_FILE_SIZE];
    memset(g_pMemFile, 0, R4_BILL_FILE_SIZE);

    g_pMemLink             = new SMEM_LINK;
    g_pMemLink->pNext      = NULL;
    g_pMemLink->nLength    = R4_BUFF_FILE_SIZE;
    g_pMemLink->pValue     = new BYTE[R4_BUFF_FILE_SIZE];

    if (g_bMustMerged)
    {
        g_pASN1Global        = Init_OSS_Lib_Final();
        g_pASN1Global_M      = Init_OSS_Lib_Final_M();

        g_pBSRecord          = (void*)new R4_Fin_M_BSRecord;
        g_pBSSummaryRecord   = (void*)new R4_Fin_M_BSXferSummaryRecord;
    }
    else
    {
        g_pASN1Global        = Init_OSS_Lib_Ini();
        g_pASN1Global_M      = Init_OSS_Lib_Ini_M();

        g_pBSRecord          = (void*)new R4_Org_M_BSRecord;
        g_pBSSummaryRecord   = (void*)new R4_Org_M_BSXferSummaryRecord;
    }

    return TRUE;
}

/*******************************************************************
函数名称：BOOL PreProcess(char* szTmpFileName, STRING& sDestFileName)
功能描述：预处理操作
输入参数：
   参数1：char*   sTmpFileName,需要转换的话单文件名
   参数2: STRING& sDestFileName,缺省目标路径
输出参数：
   参数1: STRING& sDestFileName,实际目标路径
返回内容：成功返回TRUE,失败返回FALSE
调用函数：
被调函数：
作者时间：Guofeng, Guan, 2003-06-02
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
BOOL CR4_440PatchForMoto::PreProcess(const char* szTmpFileName,
                                     STRING& sDestFileName)
{
    UINT4  nCurOffset  = 0;
    FILE*  pTmpFile    = NULL;

    //打开文件，获得合适的处理话单的缓冲区
    pTmpFile = fopen(szTmpFileName, "rb");
    if (NULL == pTmpFile)
    {
        return FALSE;
    }
    else
    {
        //处理缓冲区不够情况
        g_nMemDataLen = FileSize(pTmpFile);
        if(g_nMemFileLen - sizeof(R4_Fin_M_BSXferSummaryRecord) < g_nMemDataLen)
        {
            g_nMemFileLen = g_nMemDataLen + sizeof(R4_Fin_M_BSXferSummaryRecord);
            delete [] g_pMemFile;
            g_pMemFile = new BYTE[g_nMemFileLen];
        }
        size_t nBytesRead = fread(g_pMemFile, g_nMemDataLen, 1, pTmpFile);
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
函数名称：BOOL ProcessNotMerged(char* szTmpFileName, char*& szDestFileName, UINT4 nChannelID)
功能描述：处理不合并的情况
输入参数：
   参数1：char*   szTmpFileName,需要转换的话单文件名
   参数2: char*&  szDestFileName,缺省目标文件名
   参数3：UINT4   nChannelID,话单文件所处的通道号
输出参数：
   参数1: char*&  szDestFileName,实际目标文件名
返回内容：成功返回TRUE,失败返回FALSE
调用函数：
被调函数：
作者时间：Guofeng, Guan, 2003-06-02
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
BOOL CR4_440PatchForMoto::ProcessNotMerged(const char* szTmpFileName,
                                           char*& szDestFileName,
                                           UINT4 nChannelID)
{
    //进行预处理
    STRING sDestFileName = szDestFileName;
    if (!PreProcess(szTmpFileName, sDestFileName))
    {
        return FALSE;
    }

    //填充表头需要收集的内容
    char  szCDRType[4];
    UINT4 nLrsnStart      = 0;
    UINT4 nLrsnEnd        = 0;
    UINT4 nTotalCount     = 0;
    UINT4 nMissingCount   = 0;
    UINT4 nDuplicateCount = 0;
    UINT4 nSequence  = 0;

    //对临时文件解码分析
    UINT4 nDataLen        = 0;
    UINT4 nRecordLen      = 0;
    int   nCurOffset      = 0;
    BOOL  bIsFirst        = TRUE;
    UINT4 nFinBufUsed     = 0;
    int  nDecodeFailTime = 0;
    SMEM_LINK* pMemLink                           = g_pMemLink;
    R4_Org_M_BSRecord::R4_Org_M__seqof4* pCurrent = (R4_Org_M_BSRecord::R4_Org_M__seqof4*)pMemLink->pValue;
    R4_Org_M_BSRecord::R4_Org_M__seqof4* pLast    = NULL;
    pCurrent->next = NULL;

    time_t openTime;
    char   szOpenTime[14];
    //获得当前时间,作为文件打开时间
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

    while (nCurOffset < g_nMemDataLen)
    {
        //读4个字节的长度
        memcpy(&nRecordLen, (BYTE*)g_pMemFile + nCurOffset, 4);
        nRecordLen = NTOHL(nRecordLen);
        //如果读出的话单长度为0，后面的话单无效
        if (0 == nRecordLen)
        {
            break;
        }
        else if ((g_nMemDataLen - nCurOffset - 4) < nRecordLen)
        //剩余的数据不够一张话单，则后面的数据无效
        {
           break;
        }
        nCurOffset += 4;

        //对每张进行解码
        nDataLen = nRecordLen;
        //对R4最终话单进行ASN.1进行解码，在nDataLen中返回解码后的长度
        int nRet = DeCodeBill_ini_mt(g_pASN1Global,
                                     (char*)g_pMemFile + nCurOffset,
                                     (int*)&nDataLen,
                                     (char*)&pCurrent->value,
                                     pMemLink->nLength - nFinBufUsed - 4);
        nCurOffset += nRecordLen;

        //如果解码失败
        if (ERR_SUCCESS != nRet)
        {
            if((nFinBufUsed != 0) && (nDecodeFailTime == 0))
            {
                //切换缓冲区
                pMemLink->pNext  = new SMEM_LINK;
                pMemLink         = pMemLink->pNext;

                pMemLink->nLength = 1024*1024;
                pMemLink->pValue  = new BYTE[pMemLink->nLength];
                pMemLink->pNext   = NULL;

                pCurrent          = (R4_Org_M_BSRecord::R4_Org_M__seqof4*)(pMemLink->pValue);
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

        pLast          = pCurrent;
        pLast->next    = NULL;

        pCurrent       = (R4_Org_M_BSRecord::R4_Org_M__seqof4*)(pMemLink->pValue + nFinBufUsed);
        pCurrent->next = NULL;


        //处理每张话单
        R4_Org_M_CallEventRecord* pTemp = (R4_Org_M_CallEventRecord*)&pLast->value;

        switch (pTemp->choice)
        {
        case R4_Org_M_sgsnPDPRecord_chosen:
            {
                R4_Org_M_SGSNPDPRecord* p = &pTemp->u.sgsnPDPRecord;
                //如果是第一张话单
                if (bIsFirst)
                {
                    //获得第一张话单的序列号nLrsnStart
                    if(p->bit_mask & R4_Org_M_SGSNPDPRecord_localSequenceNumber_present)
                    {
                        nLrsnStart = p->localSequenceNumber;
                    }
                    //获得第一张话单类型szCDRType
                    strcpy(szCDRType, "S");
                    bIsFirst = FALSE;
                }
                if(p->bit_mask & R4_Org_M_SGSNPDPRecord_localSequenceNumber_present)
                {
                    nSequence = p->localSequenceNumber;
                }

                break;
            }
        case R4_Org_M_ggsnPDPRecord_chosen:
            {
                R4_Org_M_GGSNPDPRecord* p = &pTemp->u.ggsnPDPRecord;
                //如果是第一张话单
                if (bIsFirst)
                {
                    if(p->bit_mask & R4_Org_M_GGSNPDPRecord_localSequenceNumber_present)
                    {
                        nLrsnStart = p->localSequenceNumber;
                    }
                    //获得第一张话单类型szCDRType
                    strcpy(szCDRType, "G");
                    bIsFirst = FALSE;
                }

                if(p->bit_mask & R4_Org_M_GGSNPDPRecord_localSequenceNumber_present)
                {
                    nSequence = p->localSequenceNumber;
                }
                break;
            }
        case R4_Org_M_sgsnMMRecord_chosen:
            {
                R4_Org_M_SGSNMMRecord* p = &pTemp->u.sgsnMMRecord;

                //如果是第一张话单
                if (bIsFirst)
                {
                    if(p->bit_mask & R4_Org_M_SGSNMMRecord_localSequenceNumber_present)
                    {
                        nLrsnStart = p->localSequenceNumber;
                    }
                    //获得第一张话单类型szCDRType
                    strcpy(szCDRType, "M");
                    bIsFirst = FALSE;
                }

                if(p->bit_mask & R4_Org_M_SGSNMMRecord_localSequenceNumber_present)
                {
                    nSequence = p->localSequenceNumber;
                }
                break;
            }
        case R4_Org_M_sgsnSMORecord_chosen:
            {
                R4_Org_M_SGSNSMORecord* p = &pTemp->u.sgsnSMORecord;
                //如果是第一张话单
                if (bIsFirst)
                {
                   if(p->bit_mask & R4_Org_M_SGSNSMORecord_localSequenceNumber_present)
                    {
                        nLrsnStart = p->localSequenceNumber;
                    }
                    nLrsnStart = pTemp->u.sgsnSMORecord.localSequenceNumber;
                    //获得第一张话单类型szCDRType
                    strcpy(szCDRType, "SMO");
                    bIsFirst = FALSE;
                }
                if(p->bit_mask & R4_Org_M_SGSNSMORecord_localSequenceNumber_present)
                {
                    nSequence = p->localSequenceNumber;
                }
                break;
            }
        case R4_Org_M_sgsnSMTRecord_chosen:
            {
                R4_Org_M_SGSNSMTRecord* p = &pTemp->u.sgsnSMTRecord;
                //如果是第一张话单
                if (bIsFirst)
                {
                    if(p->bit_mask & R4_Org_M_SGSNSMTRecord_localSequenceNumber_present)
                    {
                        nLrsnStart = p->localSequenceNumber;
                    }
                    //获得第一张话单类型szCDRType
                    strcpy(szCDRType, "SMT");
                    bIsFirst = FALSE;
                }

                if(p->bit_mask & R4_Org_M_SGSNSMTRecord_localSequenceNumber_present)
                {
                    nSequence = p->localSequenceNumber;
                }
                break;
            }
        case R4_Org_M_sgsnLCTRecord_chosen:
            {
                R4_Org_M_SGSNLCTRecord* p = &pTemp->u.sgsnLCTRecord;
                //如果是第一张话单
                if (bIsFirst)
                {
                    if(p->bit_mask & R4_Org_M_SGSNLCTRecord_localSequenceNumber_present)
                    {
                        nLrsnStart = p->localSequenceNumber;
                    }
                    //获得第一张话单类型szCDRType
                    strcpy(szCDRType, "LMT");
                    bIsFirst = FALSE;
                }

                if(p->bit_mask & R4_Org_M_SGSNLCTRecord_localSequenceNumber_present)
                {
                    nSequence = p->localSequenceNumber;
                }
                break;
            }
        case R4_Org_M_sgsnLCORecord_chosen:
            {
                R4_Org_M_SGSNLCORecord* p = &pTemp->u.sgsnLCORecord;
                //如果是第一张话单
                if (bIsFirst)
                {
                    if(p->bit_mask & R4_Org_M_SGSNLCORecord_localSequenceNumber_present)
                    {
                        nLrsnStart = p->localSequenceNumber;
                    }
                    //获得第一张话单类型szCDRType
                    strcpy(szCDRType, "LMO");
                    bIsFirst = FALSE;
                }

                if(p->bit_mask & R4_Org_M_SGSNLCORecord_localSequenceNumber_present)
                {
                    nSequence = p->localSequenceNumber;
                }
                break;
            }
        case R4_Org_M_sgsnLCNRecord_chosen:
            {
                R4_Org_M_SGSNLCNRecord* p = &pTemp->u.sgsnLCNRecord;
                //如果是第一张话单
                if (bIsFirst)
                {
                    if(p->bit_mask & R4_Org_M_SGSNLCNRecord_localSequenceNumber_present)
                    {
                        nLrsnStart = p->localSequenceNumber;
                    }
                    //获得第一张话单类型szCDRType
                    strcpy(szCDRType, "LNI");
                    bIsFirst = FALSE;
                }

                if(p->bit_mask & R4_Org_M_SGSNLCNRecord_localSequenceNumber_present)
                {
                    nSequence = p->localSequenceNumber;
                }
                break;
            }
        default:
            break;
        }
        nTotalCount++;
    }

    nLrsnEnd = nSequence;

    //获得实际文件名
    //by ldf 对应问题单D20358
    char szBsXferFile[34];
    char szIPAddr[40];
    sprintf(szIPAddr, "%x", inet_addr(g_szGsnID));

    if( g_bCMCC )
    {
        //by ldf 2003-08-07  对应问题单SWPD00468
        char szSeqenceNumber[10];
        STRING  sTmpFileName = szTmpFileName;
        UINT2 uPos1 = sTmpFileName.find_first_of("_") ;
        UINT2 uPos2 = sTmpFileName.find_last_of(".") ;
        UINT2 uLen = uPos2 - uPos1;
        if (uLen > 6)
        {
            //substr(1,uPos+1),得到去掉最前面的b和.后面的问题
            //(这里传入的是临时文件，例如01_00001.tmp)
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
            sprintf( szSeqenceNumber,"%s%s",sTmpZero.c_str(),
                        sTmpFileName.substr(uPos1+1,uLen-1).c_str());

        }

        sprintf(szBsXferFile, "%s_%s_%s.btf",szIPAddr,szOpenTime,szSeqenceNumber);
    }
    else
    {
        sprintf(szBsXferFile, "%s_%s.btf", szIPAddr, szOpenTime);
    }
    //end

    //实际话单的路径
    sDestFileName = sDestFileName + szBsXferFile;

    szDestFileName = new char[sDestFileName.length() + 1];
    strcpy(szDestFileName, sDestFileName.c_str());

    R4_Org_M_BSXferSummaryRecord* pSummary =
        (R4_Org_M_BSXferSummaryRecord*)g_pBSSummaryRecord;

    //***********************填写表头信息*********************************//
    //填写GSN地址:cgnID
    sprintf((char*)pSummary->cgnID.value, "%s", g_szGsnID);
    pSummary->cgnID.length = strlen((char*)pSummary->cgnID.value);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->cgnID.value, (15-pSummary->cgnID.length));
    pSummary->cgnID.length = 15;
    //end

    //填写文件名:bsXferFile
    strcpy((char*)pSummary->bsXferFile.value, szBsXferFile);
    pSummary->bsXferFile.length = strlen(szBsXferFile);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->bsXferFile.value, (34-pSummary->bsXferFile.length));
    pSummary->bsXferFile.length = 34;
    //end

    //填写文件创建时间:openTime
    strcpy((char*)pSummary->openTime.value, szOpenTime);
    pSummary->openTime.length = strlen(szOpenTime);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->openTime.value, (14-pSummary->openTime.length));
    pSummary->openTime.length = 14;
    //end

    time_t closeTime;
    char   szCloseTime[14];
    //获得当前时间,作为文件打开时间
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

    //填写文件关闭时间:closeTime
    strcpy((char*)pSummary->closeTime.value, szCloseTime);
    pSummary->closeTime.length = strlen(szCloseTime);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->closeTime.value, (14-pSummary->closeTime.length));
    pSummary->closeTime.length = 14;
    //end

    //填写开始的本地记录序列号:lrsnStart
    sprintf((char*)pSummary->lrsnStart.value, "%u", nLrsnStart);
    pSummary->lrsnStart.length = strlen((char*)pSummary->lrsnStart.value);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->lrsnStart.value, (10-pSummary->lrsnStart.length));
    pSummary->lrsnStart.length = 10;
    //end

    //填写结束的本地记录序列号:lrsnEnd
    sprintf((char*)pSummary->lrsnEnd.value, "%u", nLrsnEnd);
    pSummary->lrsnEnd.length = strlen((char*)pSummary->lrsnEnd.value);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->lrsnEnd.value, (10-pSummary->lrsnEnd.length));
    pSummary->lrsnEnd.length = 10;
    //end

    //填写话单类型:cdrType
    sprintf((char*)pSummary->cdrType.value, "%s", szCDRType);
    pSummary->cdrType.length = strlen((char*)pSummary->cdrType.value);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->cdrType.value, (3-pSummary->cdrType.length));
    pSummary->cdrType.length = 3;
    //end

    //填写所有话单总数:totalCdr
    sprintf((char*)pSummary->totalCdr.value, "%u", nTotalCount);
    pSummary->totalCdr.length = strlen((char*)pSummary->totalCdr.value);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->totalCdr.value, (10-pSummary->totalCdr.length));
    pSummary->totalCdr.length = 10;
    //end

    //填写丢失的话单总数:missingCdr
    sprintf((char*)pSummary->missingCdr.value, "%u", nMissingCount);
    pSummary->missingCdr.length = strlen((char*)pSummary->missingCdr.value);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->missingCdr.value, (10-pSummary->missingCdr.length));
    pSummary->missingCdr.length = 10;
    //end

    //填写重复的话单总数:duplicateCdr
    sprintf((char*)pSummary->duplicateCdr.value, "%u", nDuplicateCount);
    pSummary->duplicateCdr.length = strlen((char*)pSummary->duplicateCdr.value);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->duplicateCdr.value, (10-pSummary->duplicateCdr.length));
    pSummary->duplicateCdr.length = 10;
    //end

    //填写CG地址:cgID
    sprintf((char*)pSummary->cgID.value, "%s", g_szCgID);
    pSummary->cgID.length = strlen((char*)pSummary->cgID.value);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->cgID.value, (15-pSummary->cgID.length));
    pSummary->cgID.length = 15;
    //end

    //填充结构表头
    ((R4_Org_M_BSRecord*)g_pBSRecord)->bsXferRecord = *pSummary;
    ((R4_Org_M_BSRecord*)g_pBSRecord)->listOfCDR    = (R4_Org_M_BSRecord::R4_Org_M__seqof4*)g_pMemLink->pValue;

    //编码
    char* pEncodedData = NULL;
    int   nBufferLen = g_nMemFileLen;

    if ((pEncodedData = EnCodeBill_ini_M_mt(g_pASN1Global_M,
                                            (char*)g_pBSRecord,
                                            (char*)g_pMemFile,
                                            &nBufferLen)) != NULL)
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
函数名称：BOOL ProcessMerged(char* szTmpFileName, char*& szDestFileName, UINT4 nChannelID)
功能描述：处理合并的情况
输入参数：
   参数1：char*   szTmpFileName,需要转换的话单文件名
   参数2: char*&  szDestFileName,缺省目标文件名
   参数3：UINT4   nChannelID,话单文件所处的通道号
输出参数：
   参数1: char*&  szDestFileName,实际目标文件名
返回内容：成功返回TRUE,失败返回FALSE
调用函数：
被调函数：
作者时间：Guofeng, Guan, 2003-06-02
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
BOOL CR4_440PatchForMoto::ProcessMerged(const char* szTmpFileName,
                                        char*& szDestFileName,
                                        UINT4 nChannelID)
{
    //进行预处理
    STRING sDestFileName = szDestFileName;
    if (!PreProcess(szTmpFileName, sDestFileName))
    {
        return FALSE;
    }

    //填充表头需要收集的内容
    char  szCDRType[4];
    UINT4 nLrsnStart      = 0;
    UINT4 nLrsnEnd        = 0;
    UINT4 nTotalCount     = 0;
    UINT4 nMissingCount   = 0;
    UINT4 nDuplicateCount = 0;
    UINT4 nSequence  = 0;

    //对临时文件解码分析
    UINT4 nDataLen        = 0;
    UINT4 nRecordLen      = 0;
    int   nCurOffset      = 0;
    BOOL  bIsFirst        = TRUE;
    UINT4 nFinBufUsed     = 0;
    int  nDecodeFailTime = 0;

    SMEM_LINK* pMemLink       = g_pMemLink;
    R4_Fin_M_BSRecord::R4_Fin_M__seqof8* pLast    = NULL;
    R4_Fin_M_BSRecord::R4_Fin_M__seqof8* pCurrent = (R4_Fin_M_BSRecord::R4_Fin_M__seqof8*)(pMemLink->pValue);
    pCurrent->next            = NULL;

	//added by zkq  2004-06-10
	//判断是否localSequenceNumber为"exclude,excluded"
	int nStart = -1;  
	int nEnd   = -1;
	int nSeq   = -1;
	//获得目标目录的文件夹名判断计费特性
	char szTmp[MAX_PATH];
	char* szPos = NULL;
	strcpy(szTmp,szDestFileName);	
	szCDRType[0] = 0;
    
    if ((szPos = strrchr(szTmp,'/')) != NULL)
    {
       szPos[0] = 0;  //把最后一个'/' 改为 0
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
    //获得当前时间,作为文件打开时间
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


    while (nCurOffset < g_nMemDataLen)
    {
        //读4个字节的长度
        memcpy(&nRecordLen, (BYTE*)g_pMemFile + nCurOffset, 4);
        nRecordLen = NTOHL(nRecordLen);
        //如果读出的话单长度为0，后面的话单无效
        if (0 == nRecordLen)
        {
            break;
        }
        else if ((g_nMemDataLen - nCurOffset - 4) < nRecordLen)
        //剩余的数据不够一张话单，则后面的数据无效
        {
           break;
        }
        nCurOffset += 4;

        //对每张进行解码
        nDataLen = nRecordLen;
        //对R4最终话单进行ASN.1进行解码，在nDataLen中返回解码后的长度
        int nRet = DeCodeBill_final_mt(g_pASN1Global,
                                       (char*)g_pMemFile + nCurOffset,
                                       (int*)&nDataLen,
                                       (char*)&pCurrent->value,
                                       pMemLink->nLength - nFinBufUsed - 4);

        nCurOffset += nRecordLen;

        //如果解码失败
        if (ERR_SUCCESS != nRet)
        {
            if((nFinBufUsed != 0) && (nDecodeFailTime == 0))
            {
                //切换缓冲区
                pMemLink->pNext  = new SMEM_LINK;
                pMemLink         = pMemLink->pNext;

                pMemLink->nLength = 1024*1024;
                pMemLink->pValue  = new BYTE[pMemLink->nLength];
                pMemLink->pNext   = NULL;

                pCurrent          = (R4_Fin_M_BSRecord::R4_Fin_M__seqof8*)(pMemLink->pValue);
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

        pCurrent = (R4_Fin_M_BSRecord::R4_Fin_M__seqof8*)(pMemLink->pValue + nFinBufUsed);
        pCurrent->next = NULL;

        //处理每张话单g_pASN1RecContent
        R4_Fin_M_CallEventRecord* pTemp = (R4_Fin_M_CallEventRecord*)&pLast->value;
        switch (pTemp->choice)
        {
        case R4_Fin_M_sgsnPDPRecord_chosen:
            {
                R4_Fin_M_SGSNPDPRecord* p = &pTemp->u.sgsnPDPRecord;

                //如果是第一张话单
                if (bIsFirst)
                {
                    //获得第一张话单的序列号nLrsnStart
                    //added by zkq 2004-06-09//处理lrstart					
                    if(p->bit_mask & R4_Fin_M_SGSNPDPRecord_localSequenceNumberList_present)
                    {					
                        if(p->localSequenceNumberList->value.bit_mask & R4_Fin_M_localSeqNumberList_present)
                        {
                            nStart = 1;  //added by zkq 2004-06-10
                            nLrsnStart = p->localSequenceNumberList->value.localSeqNumberList->value;
                        }					
                    }				
                    //获得第一张话单类型szCDRType区分是否非计费特性 
                    if(0 == szCDRType[0])
                    {
                        strcpy(szCDRType, "S");
                    }				
                    //end                    
                    bIsFirst = FALSE;
                }
                //added by zkq 2004-06-09//处理lrsnEnd					
                if(p->bit_mask & R4_Fin_M_SGSNPDPRecord_localSequenceNumberList_present)
                {				
                    R4_Fin_M_SGSNPDPRecord::R4_Fin_M__seqof5 *q = p->localSequenceNumberList; 
                    while (q->next != NULL)
                    {
                        q = q->next;
                    }
                    
                    if (q->value.bit_mask & R4_Fin_M_localSeqNumberList_present)
                    {
                        R4_Fin_M__seqof1_ * r = q->value.localSeqNumberList;
                        while (r->next != NULL)
                        {
                            r= r->next;
                        }
                        nSeq = 1;  
                        nSequence = r->value;
                    }					
                }			
                //end  
                break;
            }
        case R4_Fin_M_ggsnPDPRecord_chosen:
            {
                R4_Fin_M_GGSNPDPRecord* p = &pTemp->u.ggsnPDPRecord;

                //如果是第一张话单
                if (bIsFirst)
                {
                    //获得第一张话单类型szCDRType
					//added by zkq 2004-06-09//处理lrstart					
					if(pTemp->u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_localSequenceNumberList_present)
					{					
						if(p->localSequenceNumberList.bit_mask & R4_Fin_M_localSeqNumberList_present)
						{
							nStart = 1;  //added by zkq 2004-06-10
							nLrsnStart = p->localSequenceNumberList.localSeqNumberList->value;
						}								
					}				
					//获得第一张话单类型szCDRType区分是否非计费特性 
					if(0 == szCDRType[0])
					{
                    strcpy(szCDRType, "G");
					}					
					//end             
                    bIsFirst = FALSE;
                }
                //added by zkq 2004-06-09//处理lrsnEnd		   
                
                if(p->bit_mask & R4_Fin_M_GGSNPDPRecord_localSequenceNumberList_present)
                {				
                    if(p->localSequenceNumberList.bit_mask & R4_Fin_M_localSeqNumberList_present)
                    {
                        R4_Fin_M__seqof1_ * r = p->localSequenceNumberList.localSeqNumberList;
                        while (r->next != NULL)
                        {
                            r = r->next;
                        }
                        nSeq = 1;  //added by zkq 2004-06-10
                        nSequence = r->value;
                    }					
                }			
                //end  		
                break;
            }
        case R4_Fin_M_sgsnMMRecord_chosen:
            {
                R4_Fin_M_SGSNMMRecord* p = &pTemp->u.sgsnMMRecord;

                //如果是第一张话单
                if (bIsFirst)
                {
                    if(p->bit_mask & R4_Fin_M_SGSNMMRecord_localSequenceNumber_present )
                    {
						nStart = 1;  //added by zkq 2004-06-10
                        nLrsnStart = p->localSequenceNumber;
                    }
                    //获得第一张话单类型szCDRType区分是否非计费特性 
                    if(0 == szCDRType[0])
                    {
                        strcpy(szCDRType, "M");
                    }
					//end                    
                    bIsFirst = FALSE;
                }

                if(p->bit_mask &  R4_Fin_M_SGSNMMRecord_localSequenceNumber_present)
                {
					nSeq = 1;  //added by zkq 2004-06-10
                    nSequence = p->localSequenceNumber;
                }
                break;
            }
        case R4_Fin_M_sgsnSMORecord_chosen:
            {
                R4_Fin_M_SGSNSMORecord* p = &pTemp->u.sgsnSMORecord;
                //如果是第一张话单
                if (bIsFirst)
                {
                    //获得第一张话单的序列号nLrsnStart
                    if(p->bit_mask & R4_Fin_M_SGSNSMORecord_localSequenceNumber_present)
                    {
						nStart = 1;  //added by zkq 2004-06-10
                        nLrsnStart = p->localSequenceNumber;
                    }
                    //获得第一张话单类型szCDRType区分是否非计费特性 
                    if(0 == szCDRType[0])
                    {
                        strcpy(szCDRType, "SMO");
                    }				
					//end                    
                    bIsFirst = FALSE;
                }

                if(p->bit_mask & R4_Fin_M_SGSNSMORecord_localSequenceNumber_present)
                {
					nSeq = 1;  //added by zkq 2004-06-10
                    nSequence = p->localSequenceNumber;
                }
                break;
            }
        case R4_Fin_M_sgsnSMTRecord_chosen:
            {
                R4_Fin_M_SGSNSMTRecord* p = &pTemp->u.sgsnSMTRecord;
                //如果是第一张话单
                if (bIsFirst)
                {
                    //获得第一张话单的序列号nLrsnStart
                    if(p->bit_mask & R4_Fin_M_SGSNSMTRecord_localSequenceNumber_present)
                    {
						nStart = 1;  //added by zkq 2004-06-10
                        nLrsnStart = p->localSequenceNumber;
                    }
                   	//获得第一张话单类型szCDRType区分是否非计费特性 
                    if(0 == szCDRType[0])
                    {
                        strcpy(szCDRType, "SMT");
                    }					
					//end                  
                    bIsFirst = FALSE;
                }

                if(p->bit_mask & R4_Fin_M_SGSNSMTRecord_localSequenceNumber_present)
                {
					nSeq = 1;  //added by zkq 2004-06-10
                    nSequence = p->localSequenceNumber;
                }
                break;
            }
            case R4_Fin_M_sgsnLCTRecord_chosen:
            {
                R4_Fin_M_SGSNLCTRecord* p = &pTemp->u.sgsnLCTRecord;
                //如果是第一张话单
                if (bIsFirst)
                {
                    if(p->bit_mask & R4_Fin_M_SGSNLCTRecord_localSequenceNumber_present)
                    {
						nStart = 1;  //added by zkq 2004-06-10
                        nLrsnStart = p->localSequenceNumber;
                    }
                   	//获得第一张话单类型szCDRType区分是否非计费特性 
                    if(0 == szCDRType[0])
                    {
                        strcpy(szCDRType, "LMT");
                    }					
					//end                    
                    bIsFirst = FALSE;
                }

                if(p->bit_mask & R4_Fin_M_SGSNLCTRecord_localSequenceNumber_present)
                {
					nSeq = 1;  //added by zkq 2004-06-10
                    nSequence = p->localSequenceNumber;
                }
                break;
            }
        case R4_Fin_M_sgsnLCORecord_chosen:
            {
                R4_Fin_M_SGSNLCORecord* p = &pTemp->u.sgsnLCORecord;
                //如果是第一张话单
                if (bIsFirst)
                {
                    if(p->bit_mask & R4_Fin_M_SGSNLCORecord_localSequenceNumber_present)
                    {
						nStart = 1;  //added by zkq 2004-06-10
                        nLrsnStart = p->localSequenceNumber;
                    }
                    //获得第一张话单类型szCDRType区分是否非计费特性 
                    if(0 == szCDRType[0])
                    {
                        strcpy(szCDRType, "LMO");
                    }	
					//end               
                    bIsFirst = FALSE;
                }

                if(p->bit_mask & R4_Fin_M_SGSNLCORecord_localSequenceNumber_present)
                {
					nSeq = 1;  //added by zkq 2004-06-10
                    nSequence = p->localSequenceNumber;
                }
                break;
            }
        case R4_Fin_M_sgsnLCNRecord_chosen:
            {
                R4_Fin_M_SGSNLCNRecord* p = &pTemp->u.sgsnLCNRecord;
                //如果是第一张话单
                if (bIsFirst)
                {
                    if(p->bit_mask & R4_Fin_M_SGSNLCNRecord_localSequenceNumber_present)
                    {
						nStart = 1;  //added by zkq 2004-06-10
                        nLrsnStart = p->localSequenceNumber;
                    }
                   	//获得第一张话单类型szCDRType区分是否非计费特性 
                    if(0 == szCDRType[0])
                    {
                        strcpy(szCDRType, "LNI");
                    }		
					//end                   
                    bIsFirst = FALSE;
                }

                if(p->bit_mask & R4_Fin_M_SGSNLCNRecord_localSequenceNumber_present)
                {
					nSeq = 1;  //added by zkq 2004-06-10
                    nSequence = p->localSequenceNumber;
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

    //获得实际文件名
    //by ldf 对应问题单D20358
    char szBsXferFile[34];
    char szIPAddr[40];
    sprintf(szIPAddr, "%x", inet_addr(g_szGsnID));

    if( g_bCMCC )
    {
        //by ldf 2003-08-07 对应问题单SWPD00468
        char szSeqenceNumber[10];
        STRING  sTmpFileName = szTmpFileName;
        UINT2 uPos1 = sTmpFileName.find_first_of("_") ;
        UINT2 uPos2 = sTmpFileName.find_last_of(".") ;
        UINT2 uLen = uPos2 - uPos1;
        if (uLen > 6)
        {
            //substr(1,uPos+1),得到去掉最前面的b和.后面的问题
            //(这里传入的是临时文件，例如01_00001.tmp)
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
            sprintf( szSeqenceNumber,"%s%s",sTmpZero.c_str(),
                        sTmpFileName.substr(uPos1+1,uLen-1).c_str());

        }

        sprintf(szBsXferFile, "%s_%s_%s.btf",szIPAddr,szOpenTime,szSeqenceNumber);
    }
    else
    {
        sprintf(szBsXferFile, "%s_%s.btf", szIPAddr, szOpenTime);
    }
    //end

    //实际话单的路径
    sDestFileName = sDestFileName + szBsXferFile;

    szDestFileName = new char[sDestFileName.length() + 1];
    strcpy(szDestFileName, sDestFileName.c_str());

    R4_Fin_M_BSXferSummaryRecord* pSummary = (R4_Fin_M_BSXferSummaryRecord*)g_pBSSummaryRecord;

    //***********************填写表头信息*********************************//
    //填写GSN地址:cgnID
    sprintf((char*)pSummary->cgnID.value, "%s", g_szGsnID);
    pSummary->cgnID.length = strlen((char*)pSummary->cgnID.value);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->cgnID.value, (15-pSummary->cgnID.length));
    pSummary->cgnID.length = 15;
    //end

    //填写文件名:bsXferFile
    strcpy((char*)pSummary->bsXferFile.value, szBsXferFile);
    pSummary->bsXferFile.length = strlen(szBsXferFile);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->bsXferFile.value, (34-pSummary->bsXferFile.length));
    pSummary->bsXferFile.length = 34;
    //end

    //填写文件创建时间:openTime
    strcpy((char*)pSummary->openTime.value, szOpenTime);
    pSummary->openTime.length = strlen(szOpenTime);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->openTime.value, (14-pSummary->openTime.length));
    pSummary->openTime.length = 14;
    //end

    time_t closeTime;
    char   szCloseTime[14];
    //获得当前时间,作为文件打开时间
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

    //填写文件关闭时间:closeTime
    strcpy((char*)pSummary->closeTime.value, szCloseTime);
    pSummary->closeTime.length = strlen(szCloseTime);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->closeTime.value, (14-pSummary->closeTime.length));
    pSummary->closeTime.length = 14;
    //end

    //填写开始的本地记录序列号:lrsnStart
    if(nStart == -1)   //added by zkq 2004-06-10
    {
        sprintf((char*)pSummary->lrsnStart.value, "%s", "exclude");
    }
    else
    {
        sprintf((char*)pSummary->lrsnStart.value, "%u", nLrsnStart);
    }
    pSummary->lrsnStart.length = strlen((char*)pSummary->lrsnStart.value);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->lrsnStart.value, (10-pSummary->lrsnStart.length));
    pSummary->lrsnStart.length = 10;
    //end

    //填写结束的本地记录序列号:lrsnEnd
    if(nEnd == -1)   //added by zkq 2004-06-10
    {
        sprintf((char*)pSummary->lrsnEnd.value, "%s", "excluded");
    }
    else
    {
        sprintf((char*)pSummary->lrsnEnd.value, "%u", nLrsnEnd);
    }
    pSummary->lrsnEnd.length = strlen((char*)pSummary->lrsnEnd.value);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->lrsnEnd.value, (10-pSummary->lrsnEnd.length));
    pSummary->lrsnEnd.length = 10;
    //end

    //填写话单类型:cdrType
    sprintf((char*)pSummary->cdrType.value, "%s", szCDRType);
    pSummary->cdrType.length = strlen((char*)pSummary->cdrType.value);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->cdrType.value, (3-pSummary->cdrType.length));
    pSummary->cdrType.length = 3;
    //end

    //填写所有话单总数:totalCdr
    sprintf((char*)pSummary->totalCdr.value, "%u", nTotalCount);
    pSummary->totalCdr.length = strlen((char*)pSummary->totalCdr.value);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->totalCdr.value, (10-pSummary->totalCdr.length));
    pSummary->totalCdr.length = 10;
    //end

    //填写丢失的话单总数:missingCdr
    sprintf((char*)pSummary->missingCdr.value, "%u", nMissingCount);
    pSummary->missingCdr.length = strlen((char*)pSummary->missingCdr.value);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->missingCdr.value, (10-pSummary->missingCdr.length));
    pSummary->missingCdr.length = 10;
    //end

    //填写重复的话单总数:duplicateCdr
    sprintf((char*)pSummary->duplicateCdr.value, "%u", nDuplicateCount);
    pSummary->duplicateCdr.length = strlen((char*)pSummary->duplicateCdr.value);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->duplicateCdr.value, (10-pSummary->duplicateCdr.length));
    pSummary->duplicateCdr.length = 10;
    //end

    //填写CG地址:cgID
    sprintf((char*)pSummary->cgID.value, "%s", g_szCgID);
    pSummary->cgID.length = strlen((char*)pSummary->cgID.value);
    //by ldf 对应问题单D20379  2003-02-21
    FillChar((char*)pSummary->cgID.value, (15-pSummary->cgID.length));
    pSummary->cgID.length = 15;
    //end

    //填充结构表头
    ((R4_Fin_M_BSRecord*)g_pBSRecord)->bsXferRecord = *pSummary;
    ((R4_Fin_M_BSRecord*)g_pBSRecord)->listOfCDR    = (R4_Fin_M_BSRecord::R4_Fin_M__seqof8*)g_pMemLink->pValue;

    //编码
    char* pEncodedData = NULL;
   
    //modify by zkq 2004-07-08
    int nBufferLen = g_nMemDestFileLen; 
    if ((pEncodedData = EnCodeBill_final_M_mt(g_pASN1Global_M,
                                              (char*)g_pBSRecord,
                                              (char*)g_pMemDestFile,
                                              &nBufferLen))  == NULL)
    {
    	delete [] g_pMemDestFile;
        g_pMemDestFile = NULL;
        g_nMemDestFileLen = 0;        
        nBufferLen = 0;
        if ((pEncodedData = EnCodeBill_final_M_mt(g_pASN1Global_M,
                                               (char*)g_pBSRecord,
                                               (char*)g_pMemDestFile,
                                                     &nBufferLen))== NULL)
        {//编码失败      
            
            TRACE(MTS_BACKSAVE, S_ENCODE_FAILED);
/*
                MSGOUT(MTS_BACKSAVE, MSGOUT_LEVEL_URGENT, S_ENCODE_FAILED);
*/
            return FALSE;
        }
        g_pMemDestFile = (BYTE *)pEncodedData;
        g_nMemDestFileLen = nBufferLen; //保存g_pMemDestFile的分配长度
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
函数名称：BOOL Process(char* szTmpFileName, char*& szDsetFileName, UINT4 nChannelID)
功能描述：动态库处理输出的接口实现函数
输入参数：
   参数1：char*   szTmpFileName,需要转换的话单文件名
   参数2: char*&  szDsetFileName,缺省目标文件名
   参数3：UINT4   nChannelID,话单文件所处的通道号
输出参数：
   参数1: char*& sDsetFileName,实际目标文件名
返回内容：成功返回TRUE,失败返回FALSE
调用函数：
   函数1：ProcessMerged(),处理合并的情况;
   函数2：ProcessMerged(),处理不合并的情况;
被调函数：
作者时间：Guofeng, Guan, 2003-06-02
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
BOOL CR4_440PatchForMoto::Process(const char* szTmpFileName,
                                  char*& szDsetFileName,
                                  UINT4 nChannelID)
{
    if (g_bMustMerged)
    {
        return ProcessMerged(szTmpFileName,szDsetFileName,nChannelID);
    }
    else
    {
        return ProcessNotMerged(szTmpFileName,szDsetFileName,nChannelID);
    }

}
/*******************************************************************
函数名称：extern "C" BOOL Terminate()
功能描述：动态库清除输出的接口实现函数
输入参数：
输出参数：无
返回内容：成功返回TRUE,失败返回FALSE
调用函数：
被调函数：
作者时间：Guofeng, Guan, 2003-06-02
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
BOOL CR4_440PatchForMoto::Terminate()
{
    if (g_pMemFile != NULL)
    {
        delete[] g_pMemFile;
    }

    if (g_pMemLink != NULL)
    {
        DeleteMemLink(g_pMemLink);
    }

    if (g_pBSSummaryRecord != NULL)
    {
        delete[] g_pBSSummaryRecord;
        g_pBSSummaryRecord = NULL;
    }

    if (g_pBSRecord != NULL)
    {
        delete[] g_pBSRecord;
        g_pBSRecord = NULL;
    }

    if(g_pMemDestFile != NULL)  //add by zkq 2004-07-08
    {
        delete[] g_pMemDestFile;
        g_pMemDestFile = NULL;
    }//end           
    if (g_bMustMerged)
    {
        Release_OSS_Lib_Final(g_pASN1Global);
        g_pASN1Global = NULL;

        Release_OSS_Lib_Final_M(g_pASN1Global_M);
        g_pASN1Global_M = NULL;
    }
    else
    {
        Release_OSS_Lib_Ini(g_pASN1Global);
        g_pASN1Global = NULL;

        Release_OSS_Lib_Ini_M(g_pASN1Global_M);
        g_pASN1Global_M = NULL;
    }

    return TRUE;
}

void CR4_440PatchForMoto::DeleteMemLink(SMEM_LINK* p)
{
    if(p->pNext != NULL)
        DeleteMemLink(p->pNext);
    else
    {
        delete [] p->pValue;
        delete p;
    }
}
