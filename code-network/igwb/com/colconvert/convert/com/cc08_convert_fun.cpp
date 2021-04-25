//ת��CC08�������ı���ʽ

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
               "�򿪻����ļ�(%s)ʧ��",
               pszSrcFile);

        WRITELOG_F("�򿪻����ļ�(%s)ʧ��",
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
    //�����ļ���У�顣
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
                       "�����ļ�(%s)�����������ļ�(%s)ʧ��",
                       pszSrcFile,
                       FinalFileNameList[0].c_str());

                WRITELOG_F("�����ļ�(%s)�����������ļ�(%s)ʧ��",
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
     * ��ʼ���л���ת����
     */

    int nRet = ERR_SUCCESS;

    //���뻰��
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

        //���ʣ��Ŀռ䲻��һ�Ż�����
        //д������ļ�
        if (nRead < nBillLen)
        {
            if (ChlFiles[0] == NULL)
            {
                ChlFiles[0] = fopen(FinalFileNameList[0].c_str(), "wb");
                if (ChlFiles[0] == NULL)
                {
                    COMSGOUT(MSGOUT_LEVEL_URGENT,
                           "���ļ�(%s)ʧ��",
                           FinalFileNameList[0].c_str());

                    WRITELOG_F("���ļ�(%s)ʧ��",
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
                       "�����ļ�(%s)�л������кŲ�����(%d-%d)",
                       pszSrcFile,
                       nSeqNum,
                       nCurSeqNum);
                WRITELOG_F("�����ļ�(%s)�л������кŲ�����(%d-%d)",
                           pszSrcFile,
                           nSeqNum,
                           nCurSeqNum);
            }
        }
        nSeqNum = nCurSeqNum;

        //�ø�ʽ�����ת��
        UINT4 uChlID;
        CFormat* pDestFmt = NULL;
        void* pDestBill = NULL;
        UINT4 uLen;


        //��ȡ�ּ�ͨ���ź�ת����ʽ���󣬲����л����Ĵ���
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
                               "�����ջ����ļ�(%s)ʧ��",
                               FinalFileNameList[uChlID].c_str());

                        WRITELOG_F("�����ջ����ļ�(%s)ʧ��",
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

        //���ת�����ɹ��������󻰵�д������ļ�
        if (ERR_SUCCESS != nResult)
        {
            if (ChlFiles[0] == NULL)
            {
                ChlFiles[0] = fopen(FinalFileNameList[0].c_str(), "wb");
                if (ChlFiles[0] == NULL)
                {
                    COMSGOUT(MSGOUT_LEVEL_URGENT,
                           "���ļ�(%s)ʧ��",
                           FinalFileNameList[0].c_str());

                    WRITELOG_F("���ļ�(%s)ʧ��",
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

