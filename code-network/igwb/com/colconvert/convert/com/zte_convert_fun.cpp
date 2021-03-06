#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../comm_convert_fun.h"
#include "../include/collect_convert_define.h"
#include "../../include/collect_convert_include.h"
#include "../../../include/toolbox.h"
#include "../../../include/format_out.h"
#include "zte_convert_fun.h"

#define  ZTE_DETAIL_TYPE       1
#define  ZTE_METER_TYPE        0


#define  ZTE_V300_DETAIL_FLAG     200
#define  ZTE_V302_DETAIL_FLAG     300

#define  ZTE_V303_METER_FLAG      200
#define  ZTE_V304_METER_FLAG      201



typedef struct
{
    unsigned char Year[5];    // for example: "1994\0"
    unsigned char Date[6];    // for example: "08.31\0"
    unsigned char Time[9];    // for example: "12:53:30\0"
    unsigned char Day;        // for example: 7 - Sun, 1 - Mon
    unsigned char Ticks;      // unit : 10ms
} TJFDATE;

typedef struct
{
    unsigned char JFVersion[2];
    TJFDATE CreateTime;
    unsigned char Reserved[69];
} TZTE_V300_HEAD;

typedef struct
{
    unsigned char RecordType;
    unsigned char PartRecID;
    unsigned char NatrAddrofCallerNum;
    unsigned char CallerNum[10];
    unsigned char NatrAddrofCalledNum;
    unsigned char CalledNum[10];
    unsigned char StartTime[4];
    unsigned char StartTicks;
    unsigned char ServiceCategory;
    unsigned char EndTime[4];
    unsigned char EndTicks;
    unsigned char ReleaseReason;
    unsigned char CallerType;
    unsigned char RecValidID : 1;
    unsigned char ClockID : 1;
    unsigned char ChargeFlg : 1;
    unsigned char AttemptCallID : 1;
    unsigned char CallerAnalyseID : 1;
    unsigned char CalledAnalyseID : 1;
    unsigned char Reserved : 2;
    unsigned char IncomingTrunkGroup[2];
    unsigned char OutgoingTrunkGroup[2];
    unsigned char SupplementServicee[7];
    unsigned char ChargePartyID;
    unsigned char NatrAddrofLinkNum;
    unsigned char LinkNum[10];
    unsigned char Fee[4];
    unsigned char BearerServices;
    unsigned char TerminalServices;
    unsigned char UUS1;
    unsigned char UUS3;
    unsigned char CallerSpecialNum[5];
    unsigned char CalledSpecialNum[5];
    unsigned char CtxGroupID[2];
    unsigned char NatrAddrofBilledNum;
    unsigned char BilledNum[11];
} TZTE_V300_BIL;

typedef struct
{
    unsigned char JFVersion[2];
    TJFDATE CreateTime;
    unsigned char Reserved[99];
} TZTE_V302_HEAD;

typedef struct
{
    unsigned char RecordType;
    unsigned char PartRecID;
    unsigned char NatrAddrofCallerNum;
    unsigned char CallerNum[20];
    unsigned char NatrAddrofCalledNum;
    unsigned char CalledNum[20];
    unsigned char StartTime[4];
    unsigned char StartTicks;
    unsigned char ServiceCategory;
    unsigned char EndTime[4];
    unsigned char EndTicks;
    unsigned char ReleaseReason;
    unsigned char CallerType;
    unsigned char RecValidID : 1;
    unsigned char ClockID : 1;
    unsigned char ChargeFlg : 1;
    unsigned char AttemptCallID : 1;
    unsigned char CallerAnalyseID : 1;
    unsigned char CalledAnalyseID : 1;
    unsigned char Reserved : 2;
    unsigned char IncomingTrunkGroup[2];
    unsigned char OutgoingTrunkGroup[2];
    unsigned char SupplementService[7];
    unsigned char ChargePartyID;
    unsigned char NatrAddrofLinkNum;
    unsigned char LinkNum[20];
    unsigned char Fee[4];
    unsigned char BearerServices;
    unsigned char TerminalServices;
    unsigned char UUS1;
    unsigned char UUS3;
    unsigned char CallerSpecialNum[5];
    unsigned char CalledSpecialNum[5];
    unsigned char CtxGroupID[2];
    unsigned char NatrAddrofBilledNum[1];
    unsigned char BilledNum[11];
} TZTE_V302_BIL;

typedef struct
{
    unsigned char VerNo;        //???????????? 200    1??;
    TJFDATE ModifyTime;         //????????           22??;
    unsigned char SubCount[2];  //????????           4??;
    unsigned char CRC[2];       //CRC??????          4??;
    unsigned char Reserved[67]; //????              67??;
} TZTE_V303_METER_HEAD;

#define TZTE_V303_METER_COUNT 9

typedef struct
{
    unsigned char PhoneNo[25];      // ????????      25????
    unsigned char PhoneID;           // ????????      1????
    unsigned char Met[TZTE_V303_METER_COUNT][4];                       // ??????????    4 * 9;
    unsigned char Sig[TZTE_V303_METER_COUNT][4];                       // ??????????    4 * 9;
} TZTE_V303_METER_BIL;

typedef struct
{
    unsigned char VerNo;
    TJFDATE ModifyTime;
    unsigned char SubCount[4];
    unsigned char CRC[4];
    unsigned char Reserved[139];
} TZTE_V304_METER_HEAD;

#define TZTE_V304_METER_COUNT 18

typedef struct
{
    unsigned char PhoneNo[25]; // ????????
    unsigned char PhoneID;     // ????????
    unsigned char Met[TZTE_V304_METER_COUNT][4];             // ??????????
    unsigned char Sig[TZTE_V304_METER_COUNT][4];             // ??????????
} TZTE_V304_METER_BIL;





int ProcessZTEBillFile(const char*   pszSrcFile,
                     VECTOR<STRING>& FinalFileNameList,
                     VECTOR<int>&    CvtResultList,
                     const int bFileLevelVerify,
                     const int bBillLevelVerify,
                     void* pIBillProcessor,
                     const int nBillLen,
                     const int nFileHeadLen,
                     const BYTE nBillType);




int ProcessZTEFile(const char*   pszSrcFile,
                     VECTOR<STRING>& FinalFileNameList,
                     VECTOR<int>&    CvtResultList,
                     const int bFileLevelVerify,
                     const int bBillLevelVerify,
                     void* pIBillProcessor,
                     const int nOrigBillLen)
{
    FILE* pSrcFile = fopen(pszSrcFile, "rb");
    if (NULL == pSrcFile)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "????????????(%s)????",
               pszSrcFile);

        WRITELOG_F("????????????(%s)????",
                   pszSrcFile);

        return ERR_FAIL;
    }

    int nFileSize = FileSize(pSrcFile);
    if (nFileSize < 2)
    {
        if (bFileLevelVerify)
        {

            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "????????(%s)??????????, ????????2",
                   pszSrcFile);

            WRITELOG_F("????????(%s)??????????, ????????2",
                       pszSrcFile);
        }
        fclose(pSrcFile);

        return ERR_FAIL;
    }

    UINT2 nVersion  = 0;

    fread(&nVersion, 2, 1, pSrcFile);
    fclose(pSrcFile);

    nVersion = ITOHS(nVersion);

    int nRet = ERR_SUCCESS;

    if (nVersion == ZTE_V300_DETAIL_FLAG)
    {
        nRet = ProcessZTEBillFile(pszSrcFile,
                           FinalFileNameList,
                           CvtResultList,
                           bFileLevelVerify,
                           bBillLevelVerify,
                           pIBillProcessor,
                           sizeof(TZTE_V300_BIL),
                           sizeof(TZTE_V300_HEAD),
                           ZTE_DETAIL_TYPE);
    }
    else if (nVersion == ZTE_V302_DETAIL_FLAG)
    {
        nRet = ProcessZTEBillFile(pszSrcFile,
                           FinalFileNameList,
                           CvtResultList,
                           bFileLevelVerify,
                           bBillLevelVerify,
                           pIBillProcessor,
                           sizeof(TZTE_V302_BIL),
                           sizeof(TZTE_V302_HEAD),
                           ZTE_DETAIL_TYPE);

    }
    else
    {

        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "??????????(%s)????????????(%d)",
               pszSrcFile,
               nVersion);

        WRITELOG_F("??????????(%s)????????????(%d)",
                   pszSrcFile,
                   nVersion);

        if (!FileCopy(pszSrcFile, FinalFileNameList[0].c_str()))
        {
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "????????(%s)??????????????(%s)????",
                   pszSrcFile,
                   FinalFileNameList[0].c_str());

            WRITELOG_F("????????(%s)??????????????(%s)????",
                       pszSrcFile,
                       FinalFileNameList[0].c_str());
        }
        else
        {
            CvtResultList[0] = 1;
        }

        nRet = ERR_FAIL;
    }

    return nRet;
}


int ProcessZTEMeterFile(const char*   pszSrcFile,
                     VECTOR<STRING>& FinalFileNameList,
                     VECTOR<int>&    CvtResultList,
                     const int bFileLevelVerify,
                     const int bBillLevelVerify,
                     void* pIBillProcessor,
                     const int nOrigBillLen)
{
    FILE* pSrcFile = fopen(pszSrcFile, "rb");
    if (NULL == pSrcFile)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "????????????(%s)????",
               pszSrcFile);

        WRITELOG_F("????????????(%s)????",
                   pszSrcFile);

        return ERR_FAIL;
    }

    int nFileSize = FileSize(pSrcFile);
    if (nFileSize < 1)
    {
        if (bFileLevelVerify)
        {
            fclose(pSrcFile);

            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "????????(%s)??????????, ????????1",
                   pszSrcFile);

            WRITELOG_F("????????(%s)??????????, ????????1",
                       pszSrcFile);
        }

        return ERR_FAIL;
    }

    BYTE nVersion  = 0;

    fread(&nVersion, 1, 1, pSrcFile);
    fclose(pSrcFile);

    int nRet = ERR_SUCCESS;

    if (nVersion == ZTE_V303_METER_FLAG)
    {
        nRet = ProcessZTEBillFile(pszSrcFile,
                           FinalFileNameList,
                           CvtResultList,
                           bFileLevelVerify,
                           bBillLevelVerify,
                           pIBillProcessor,
                           sizeof(TZTE_V303_METER_BIL),
                           sizeof(TZTE_V303_METER_HEAD),
                           ZTE_METER_TYPE);
    }
    else if (nVersion == ZTE_V304_METER_FLAG)
    {
        nRet = ProcessZTEBillFile(pszSrcFile,
                           FinalFileNameList,
                           CvtResultList,
                           bFileLevelVerify,
                           bBillLevelVerify,
                           pIBillProcessor,
                           sizeof(TZTE_V304_METER_BIL),
                           sizeof(TZTE_V304_METER_HEAD),
                           ZTE_METER_TYPE);

    }
    else
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "??????????(%s)????????????(%d)",
               pszSrcFile,
               nVersion);

        WRITELOG_F("??????????(%s)????????????(%d)",
                   pszSrcFile,
                   nVersion);

        if (!FileCopy(pszSrcFile, FinalFileNameList[0].c_str()))
        {
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "????????(%s)??????????????(%s)????",
                   pszSrcFile,
                   FinalFileNameList[0].c_str());

            WRITELOG_F("????????(%s)??????????????(%s)????",
                       pszSrcFile,
                       FinalFileNameList[0].c_str());
        }
        else
        {
            CvtResultList[0] = 1;
        }

        nRet = ERR_FAIL;
    }

    return nRet;

}


int ProcessZTEBillFile(const char*   pszSrcFile,
                     VECTOR<STRING>& FinalFileNameList,
                     VECTOR<int>&    CvtResultList,
                     const int bFileLevelVerify,
                     const int bBillLevelVerify,
                     void* pIBillProcessor,
                     const int nBillLen,
                     const int nFileHeadLen,
                     const BYTE nBillType)
{
    IBillProcessor* pBillProcessor = (IBillProcessor*)pIBillProcessor;

    FILE* pSrcFile = fopen(pszSrcFile, "rb");
    if (NULL == pSrcFile)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "????????????(%s)????",
               pszSrcFile);

        WRITELOG_F("????????????(%s)????",
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

    //????????????????
    int nFileValidResult = ERR_SUCCESS;
    if (nFileSize < nFileHeadLen)
    {
        nFileValidResult = ERR_FAIL;
    }

    if (bFileLevelVerify)
    {
        if ((nFileSize - nFileHeadLen) % nBillLen != 0)
        {
            nFileValidResult = ERR_FAIL;
        }
    }

    if (nFileValidResult == ERR_FAIL)
    {
        if (!FileCopy(pszSrcFile, FinalFileNameList[0].c_str()))
        {
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "????????(%s)??????????????(%s)????",
                   pszSrcFile,
                   FinalFileNameList[0].c_str());

            WRITELOG_F("????????(%s)??????????????(%s)????",
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
     * ??????????????????
     */
    //????????
    fseek(pSrcFile, nFileHeadLen, SEEK_SET);

    int nResult = ERR_SUCCESS;
    int nErrBillCount = 0;
    int nBuff = nFileSize - nFileHeadLen;
    char* pBuff = new char[nBuff];
    char* pABill = NULL;
    int nRead = fread(pBuff, 1, nBuff, pSrcFile);
    pABill = pBuff;

    int nRet = ERR_SUCCESS;
    while (nRead > 0)
    {

        //????????????????????????????
        //????????????
        if (nRead < nBillLen)
        {
            if (ChlFiles[0] == NULL)
            {
                ChlFiles[0] = fopen(FinalFileNameList[0].c_str(), "wb");
                if (ChlFiles[0] == NULL)
                {
                    COMSGOUT(MSGOUT_LEVEL_URGENT,
                           "????????(%s)????",
                           FinalFileNameList[0].c_str());

                    WRITELOG_F("????????(%s)????",
                               FinalFileNameList[0].c_str());
                    nRet = ERR_FAIL;
                    break;
                }
            }

            fwrite(pABill, nRead, 1, ChlFiles[0]);
            CvtResultList[0] = 1;
            break;
        }

        //????????????????
        UINT4 uChlID;
        CFormat* pDestFmt = NULL;
        void* pDestBill = NULL;
        UINT4 uLen;


        //??????????????????????????????????????????????
        pBillProcessor->Distribute(pABill,
                                   nBillLen,
                                   uChlID,
                                   pDestFmt);

        if ((pDestFmt != NULL) && (uChlID != 0))
        {
            nResult = pDestFmt->ConvertABill(pABill,
                                             nBillLen,
                                             pDestBill,
                                             uLen);
            if (ERR_SUCCESS == nResult)
            {
                if (ChlFiles[uChlID] == NULL)
                {
                    FILE* pFile = fopen(FinalFileNameList[uChlID].c_str(), "wb");
                    if (pFile == NULL)
                    {
                        COMSGOUT(MSGOUT_LEVEL_URGENT,
                               "????????????????(%s)????",
                               FinalFileNameList[uChlID].c_str());

                        WRITELOG_F("????????????????(%s)????",
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

        //??????????????????????????????????????
        if (ERR_SUCCESS != nResult)
        {
            if (ChlFiles[0] == NULL)
            {
                ChlFiles[0] = fopen(FinalFileNameList[0].c_str(), "wb");
                if (ChlFiles[0] == NULL)
                {
                    COMSGOUT(MSGOUT_LEVEL_URGENT,
                           "????????(%s)????",
                           FinalFileNameList[0].c_str());

                    WRITELOG_F("????????(%s)????",
                               FinalFileNameList[0].c_str());
                    nRet = ERR_FAIL;
                    break;
                }
            }

            fwrite(pABill, nBillLen, 1, ChlFiles[0]);
            CvtResultList[0] = 1;
        }

        pABill += nBillLen;
        nRead -= nBillLen;
    }


    delete pBuff;
    pBuff = NULL;

    fclose(pSrcFile);
    pSrcFile = NULL;

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

