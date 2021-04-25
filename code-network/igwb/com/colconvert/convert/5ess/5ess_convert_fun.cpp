#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../comm_convert_fun.h"
#include "../include/collect_convert_define.h"
#include "../../include/collect_convert_include.h"
#include "../../../include/toolbox.h"
#include "../../../include/format_out.h"

typedef struct
{
    char Head[40];
} TAMA_HEAD;

typedef struct
{
    char Tail[75];
} TAMA_TAIL;

//MM话单前头有3行
typedef struct
{
    char Head[3][80];
} TMM_HEAD;

typedef struct
{
    char data[40];
} TMM_RECORD;

int const RECORD_NUM_IN_BLOCK = 10;

typedef struct
{
    char BlockHead[38];
    TMM_RECORD Records[RECORD_NUM_IN_BLOCK];
} TMM_BLOCK;

typedef struct
{
    char Tail1[80];
} TMM_TAIL;


typedef struct
{
    char CallNum[9];
    char data[35];
    char Return;
}
TFINAL_MM_RECORD;

typedef struct
{
    TFINAL_MM_RECORD Records[RECORD_NUM_IN_BLOCK];
} TFINAL_MM_BLOCK;


int FormatMMBlock(TMM_BLOCK aBlock, TFINAL_MM_BLOCK& fBlock);

int Process5essAMAFile(const char*   pszSrcFile,
                     VECTOR<STRING>& FinalFileNameList,
                     VECTOR<int>&    CvtResultList,
                     const int bFileLevelVerify,
                     const int bBillLevelVerify,
                     void* pIBillProcessor,
                     const int nOrigBillLen)

{
    IBillProcessor* pBillProcessor = (IBillProcessor*)pIBillProcessor;

    FILE* pSrcFile = fopen(pszSrcFile, "rb");
    if (NULL == pSrcFile)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "转换时,打开原始话单文件(%s)失败",
               pszSrcFile);

        WRITELOG_F("转换时,打开原始话单文件(%s)失败",
                   pszSrcFile);

        return ERR_FAIL;
    }


    VECTOR<FILE*> ChlFiles;
    VECTOR<STRING>::iterator item = FinalFileNameList.begin();

    for (; item != FinalFileNameList.end(); item++)
    {
        ChlFiles.push_back(NULL);
    }


    int nFileSize = FileSize(pSrcFile);
    if (nFileSize < sizeof(TAMA_HEAD) + sizeof(TAMA_TAIL))
    {
        if (!FileCopy(pszSrcFile, FinalFileNameList[0].c_str()))
        {
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "话单文件(%s)拷贝到错误文件(%s)失败",
                   pszSrcFile,
                   FinalFileNameList[0].c_str());

            WRITELOG_F("话单文件(%s)拷贝到错误文件(%s)失败",
                       pszSrcFile,
                       FinalFileNameList[0].c_str());
        }
        else
        {
            CvtResultList[0] = 1;
        }

        fclose(pSrcFile);
        return ERR_FAIL;
    }

    /*
     * 开始进行话单转换。
     */

    int nResult = fseek(pSrcFile, sizeof(TAMA_HEAD), SEEK_SET);
    if (0 != nResult)
    {
        fclose(pSrcFile);
        return ERR_FAIL;
    }


    int nErrBillCount = 0;
    int nBuff = nFileSize - sizeof(TAMA_HEAD) - sizeof(TAMA_TAIL);
    char* pBuff = new char[nBuff];
    char* pABill = NULL;
    int nRead = fread(pBuff, 1, nBuff, pSrcFile);
    pABill = pBuff;

    int nRet = ERR_SUCCESS;
    while (nRead > 0)
    {
        int nRecordLen = BCD2Bin(pABill, 4);

        //如果剩余的空间不足一张话单，
        //写入错误文件
        if (nRead < nRecordLen)
        {
            if (ChlFiles[0] == NULL)
            {
                ChlFiles[0] = fopen(FinalFileNameList[0].c_str(), "wb");
                if (ChlFiles[0] == NULL)
                {
                    COMSGOUT(MSGOUT_LEVEL_URGENT,
                           "打开文件(%s)失败",
                           FinalFileNameList[0].c_str());

                    WRITELOG_F("打开文件(%s)失败",
                               FinalFileNameList[0].c_str());
                    nRet = ERR_FAIL;
                    break;
                }
            }

            fwrite(pABill, nRead, 1, ChlFiles[0]);
            CvtResultList[0] = 1;
            break;
        }

        //用格式库进行转换
        UINT4 uChlID;
        CFormat* pDestFmt = NULL;
        void* pDestBill = NULL;
        UINT4 uLen;


        //获取分拣通道号和转换格式对象，并进行话单的处理
        pBillProcessor->Distribute(pABill, nRecordLen,
                                    uChlID, pDestFmt);

        if ((pDestFmt != NULL) && (uChlID != 0))
        {
            nResult = pDestFmt->ConvertABill(pABill,
                                             nRecordLen,
                                             pDestBill, uLen);
            if (ERR_SUCCESS == nResult)
            {
                if (ChlFiles[uChlID] == NULL)
                {
                    FILE* pFile = fopen(FinalFileNameList[uChlID].c_str(), "wb");
                    if (pFile == NULL)
                    {
                        COMSGOUT(MSGOUT_LEVEL_URGENT,
                               "打开最终话单文件(%s)失败",
                               FinalFileNameList[uChlID].c_str());

                        WRITELOG_F("打开最终话单文件(%s)失败",
                                   FinalFileNameList[uChlID].c_str());

                        delete pDestBill;
                        nRet = ERR_FAIL;
                        break;
                    }

                    ChlFiles[uChlID] = pFile;
                }

                fwrite(pDestBill, uLen, 1, ChlFiles[uChlID]);
                CvtResultList[uChlID] = 1;

                delete pDestBill;
            }
        }
        else
        {
            nResult = ERR_FAIL;
        }

        //如果转换不成功，将错误话单写入错误文件
        if (ERR_SUCCESS != nResult)
        {
            if (ChlFiles[0] == NULL)
            {
                ChlFiles[0] = fopen(FinalFileNameList[0].c_str(), "wb");
                if (ChlFiles[0] == NULL)
                {
                    COMSGOUT(MSGOUT_LEVEL_URGENT,
                           "打开文件(%s)失败",
                           FinalFileNameList[0].c_str());

                    WRITELOG_F("打开文件(%s)失败",
                               FinalFileNameList[0].c_str());
                    nRet = ERR_FAIL;
                    break;
                }
            }

            fwrite(pABill, nRecordLen, 1, ChlFiles[0]);
            CvtResultList[0] = 1;
        }

        pABill += nRecordLen;
        nRead -= nRecordLen;
    }


    delete pBuff;
    fclose(pSrcFile);

    for(int j = 0; j < ChlFiles.size(); j++)
    {
        if (ChlFiles[j] != NULL)
        {
            fclose(ChlFiles[j]);
            ChlFiles[j] = NULL;
        }
    }


    return nRet;
}

int Process5essMMFile(const char*     pszSrcFile,
                     VECTOR<STRING>& FinalFileNameList,
                     VECTOR<int>&    CvtResultList,
                     const int bFileLevelVerify,
                     const int bBillLevelVerify,
                     void* pIBillProcessor,
                     const int nOrigBillLen)

{
    IBillProcessor* pBillProcessor = (IBillProcessor*)pIBillProcessor;

    FILE* pSrcFile = fopen(pszSrcFile, "r");
    if (NULL == pSrcFile)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "打开话单文件(%s)失败",
               pszSrcFile);

        WRITELOG_F("打开话单文件(%s)失败",
                   pszSrcFile);

        return ERR_FAIL;
    }

    LIST<STRING> ChlNames;
    pBillProcessor->GetChannelNames(ChlNames);

    int nMeterChl = 0;
    BOOL bFound = FALSE;
    LIST<STRING>::iterator item = ChlNames.begin();
    for (; item != ChlNames.end(); item++)
    {
        if (StringiCmp((*item).c_str(), "meter"))
        {
            bFound = TRUE;
            break;
        }

        nMeterChl++;
    }

    if (!bFound)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "Process5essMMFile函数中没有找到meter通道");

        COTRACE("Process5essMMFile函数中没有找到meter通道");

        fclose(pSrcFile);
        return ERR_FAIL;
    }

    VECTOR<FILE*> ChlFiles;
    item = ChlNames.begin();

    for (; item != ChlNames.end(); item++)
    {
        ChlFiles.push_back(NULL);
    }

    FILE* pMeterFile = fopen(FinalFileNameList[nMeterChl].c_str(), "wb");
    if (pMeterFile == NULL)
    {

        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "打开最终话单文件(%s)失败",
               FinalFileNameList[nMeterChl]);

        WRITELOG_F("打开最终话单文件(%s)失败",
                   FinalFileNameList[nMeterChl]);

        fclose(pSrcFile);
        return ERR_FAIL;
    }

    ChlFiles[nMeterChl] = pMeterFile;

    int i = 0;
    char* pRead = NULL;
    BOOL  bFileError = FALSE;
    //读出文件头三行
    TMM_HEAD mmhead;

    for (i = 0; i < 3; i++)
    {
        pRead = fgets(mmhead.Head[i], sizeof(mmhead.Head[i]), pSrcFile);
        if (pRead == NULL)
        {
            bFileError = TRUE;
            break;
        }
    }


    /*
     * 开始进行话单转换。
     */

    int nResult = 0;
    int nRet    = ERR_SUCCESS;
    TMM_BLOCK       aBlock;
    TFINAL_MM_BLOCK aFinalBlock;

    while (pRead != NULL)
    {
        pRead = fgets(aBlock.BlockHead,
                      sizeof(aBlock.BlockHead),
                      pSrcFile);

        if (pRead == NULL)
        {
            bFileError = TRUE;
            nRet       = ERR_FAIL;
            break;
        }

        for (i = 0; i < RECORD_NUM_IN_BLOCK; i++)
        {
            pRead = fgets(aBlock.Records[i].data,
                          sizeof(aBlock.Records[i].data),
                          pSrcFile);
            if (pRead == NULL)
            {
                //如果第一个记录为空，真好是把文件尾读完
                if (i != 0)
                {
                    bFileError = TRUE;
                }
                break;
            }
        }

        if (pRead == NULL)
        {
            nRet = ERR_FAIL;
            break;
        }

        FormatMMBlock(aBlock, aFinalBlock);

        nResult = fwrite(&aFinalBlock,
                          sizeof(aFinalBlock),
                          1,
                          ChlFiles[nMeterChl]);

        CvtResultList[nMeterChl] = 1;
        if (1 != nResult)
        {
            nRet = ERR_FAIL;
            break;
        }
        fflush(ChlFiles[nMeterChl]);
    }


    fclose(pSrcFile);
    pSrcFile = NULL;
    fclose(ChlFiles[nMeterChl]);
    ChlFiles[nMeterChl] = NULL;


    //进行文件级校验。
    if ((bFileLevelVerify) && (bFileError))
    {
        FileDel(FinalFileNameList[nMeterChl].c_str());

        if (!FileCopy(pszSrcFile, FinalFileNameList[0].c_str()))
        {
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "话单文件(%s)拷贝到错误文件(%s)失败",
                   pszSrcFile,
                   FinalFileNameList[0].c_str());

            WRITELOG_F("话单文件(%s)拷贝到错误文件(%s)失败",
                       pszSrcFile,
                       FinalFileNameList[0].c_str());
        }
        else
        {
            CvtResultList[0] = 1;
            CvtResultList[nMeterChl] = 0;
        }
    }


    return nRet;
}

int FormatMMBlock(TMM_BLOCK aBlock, TFINAL_MM_BLOCK& fBlock)
{
    memset(&fBlock, ' ', sizeof(fBlock));

    char strTemp[100];
    char* pFirstNum = (char*)(&(aBlock.BlockHead[7]));
    int nFirstNum = atoi(pFirstNum);

    int nLen = 8;
    char strLen[20];
    std::string strFormat;
    sprintf(strLen, "%d", nLen);
    strFormat = "%";
    strFormat += strLen;
    strFormat += "d";

    for (int i = 0; i < RECORD_NUM_IN_BLOCK; i++)
    {
        //转换号码
        sprintf(strTemp, strFormat.c_str(), i + nFirstNum);
        memcpy(fBlock.Records[i].CallNum,
               strTemp,
               strlen(strTemp));

        //转换计次
        memcpy(fBlock.Records[i].data,
               &aBlock.Records[i].data[2],
               sizeof(fBlock.Records[i].data));
        fBlock.Records[i].Return = '\n';
    }

    return 0;
}

