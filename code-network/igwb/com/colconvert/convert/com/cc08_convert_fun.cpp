//转换CC08话单到文本格式

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../comm_convert_fun.h"
#include "../include/collect_convert_define.h"
#include "../../include/collect_convert_include.h"
#include "../../../include/toolbox.h"
#include "../../../include/format_out.h"

const int BULK_COUNT = 1000;
const int CC08_BILL_LEN_DEFAULT = 118;

int ProcCC08File(const char*   pszSrcFile,
                 VECTOR<STRING>& FinalFileNameList,
                 VECTOR<int>&    CvtResultList,
                 const int bFileLevelVerify,
                 const int bBillLevelVerify,
                 void* pIBillProcessor,
                 const int nOrigBillLen)
{
    int nBillLen = nOrigBillLen;
    if(nBillLen == 0)
    {
       nBillLen = CC08_BILL_LEN_DEFAULT;
    }

   
    IBillProcessor* pBillProcessor = (IBillProcessor*)pIBillProcessor;

    FILE* pSrcFile = fopen(pszSrcFile, "rb");
    if (NULL == pSrcFile)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "打开话单文件(%s)失败",
               pszSrcFile);

        WRITELOG_F("打开话单文件(%s)失败",
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
    //进行文件级校验。
    if (bFileLevelVerify)
    {
        int nFileValidResult = ERR_SUCCESS;
        if (nFileSize % nBillLen != 0)
        {
            nFileValidResult = ERR_FAIL;
        }


        if (nFileValidResult == ERR_FAIL)
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
    }

    /*
     * 开始进行话单转换。
     */

    int nRet = ERR_SUCCESS;

    //读入话单
    fseek(pSrcFile, 0, SEEK_SET);

    int nResult = ERR_SUCCESS;
    int nErrBillCount = 0;
    int nBuff = nFileSize;
    char* pBuff = new char[nBuff];
    char* pABill = NULL;
    int nRead = fread(pBuff, 1, nBuff, pSrcFile);
    pABill = pBuff;
    int nSeqNum = -1;

    while (nRead > 0)
    {

        //如果剩余的空间不足一张话单，
        //写入错误文件
        if (nRead < nBillLen)
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


        int nCurSeqNum;
        memcpy(&nCurSeqNum, pABill, 4);
        nCurSeqNum = ITOHL(nCurSeqNum);
        if(nSeqNum != -1)
        {
            if(nCurSeqNum != nSeqNum + 1 && bBillLevelVerify)
            {
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                       "话单文件(%s)中话单序列号不连续(%d-%d)",
                       pszSrcFile,
                       nSeqNum,
                       nCurSeqNum);
                WRITELOG_F("话单文件(%s)中话单序列号不连续(%d-%d)",
                           pszSrcFile,
                           nSeqNum,
                           nCurSeqNum);
            }
        }
        nSeqNum = nCurSeqNum;

        //用格式库进行转换
        UINT4 uChlID;
        CFormat* pDestFmt = NULL;
        void* pDestBill = NULL;
        UINT4 uLen;


        //获取分拣通道号和转换格式对象，并进行话单的处理
        pBillProcessor->Distribute(pABill, nBillLen,
                                    uChlID, pDestFmt);

        if ((pDestFmt != NULL) && (uChlID != 0))
        {
            nResult = pDestFmt->ConvertABill(pABill,
                                             nBillLen,
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

            fwrite(pABill, nBillLen, 1, ChlFiles[0]);
            CvtResultList[0] = 1;
        }

        pABill += nBillLen;
        nRead -= nBillLen;
    }

    delete pBuff;
    fclose(pSrcFile);

    for(int j = 0; j < ChlFiles.size(); j++)
    {
        if (ChlFiles[j] != NULL)
        {
            fclose(ChlFiles[j]);
        }
    }


    return nRet;
}

