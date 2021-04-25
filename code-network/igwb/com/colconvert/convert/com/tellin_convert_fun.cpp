//ת��TELLIN�������ı���ʽ

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../comm_convert_fun.h"
#include "../include/collect_convert_define.h"
#include "../../include/collect_convert_include.h"
#include "../../../include/toolbox.h"
#include "../../../include/format_out.h"


int ProcTELLINFile(const char*   pszSrcFile,
                   VECTOR<STRING>& FinalFileNameList,
                   VECTOR<int>&    CvtResultList,
                   const int bFileLevelVerify,
                   const int bBillLevelVerify,
                   void* pIBillProcessor,
                   const int nOrigBillLen)
{
    IBillProcessor* pBillProcessor = (IBillProcessor*)pIBillProcessor;

    FILE* pSrcFile = fopen(pszSrcFile, "rt");
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
        /*if (nFileSize < 180)
        {
            nFileValidResult = ERR_FAIL;
        }*/


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
    char szLine[4096];
    int nResult = ERR_SUCCESS;

    while (!feof(pSrcFile))
    {
        const char* p = fgets(szLine, sizeof(szLine), pSrcFile);
        if (p == NULL)
        {
            if(!feof(pSrcFile))
            {
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                           "��ԭʼ�����ļ�(%s)ʧ��(%s)",
                           pszSrcFile,
                           strerror(errno));

                WRITELOG_F("��ԭʼ�����ļ�(%s)ʧ��(%s)",
                           pszSrcFile,
                           strerror(errno));

                nRet = ERR_FAIL;
            }
            break;
        }

        //�ø�ʽ�����ת��
        UINT4 uChlID;
        CFormat* pDestFmt = NULL;
        void* pDestBill = NULL;
        UINT4 uLen;

        int nDistingLen = strlen(szLine);

        //ȥ������0x0A 0x0D
        while(nDistingLen > 0)
        {
            if(szLine[nDistingLen - 1] == 0x0A || szLine[nDistingLen - 1] == 0x0D)
                nDistingLen--;
            else
                break;
        }

        //��ȡ�ּ�ͨ���ź�ת����ʽ���󣬲����л����Ĵ���
        pBillProcessor->Distribute(szLine, nDistingLen,
                                    uChlID, pDestFmt);

        if ((pDestFmt != NULL) && (uChlID != 0))
        {
            nResult = pDestFmt->ConvertABill(szLine,
                                             nDistingLen,
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

            fwrite(szLine, nDistingLen + 1, 1, ChlFiles[0]);
            CvtResultList[0] = 1;
        }
    }

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

int SaveSecondTELLIN(const char* szFirstFile,
                      const char* szSecondFile)
{
    if(szFirstFile == NULL || szFirstFile[0] == '\0')
    {
        COTRACE("����ڶ��ݻ����ļ�ʧ��:Դ�ļ�����Ч");
        return ERR_FAIL;
    }
    if(szSecondFile == NULL || szSecondFile[0] == '\0')
    {
        COTRACE("����ڶ��ݻ����ļ�ʧ��:Ŀ���ļ�����Ч");
        return ERR_FAIL;
    }

    FILE* fFirst = fopen(szFirstFile, "rb");
    if(NULL == fFirst)
    {
        COTRACE("����ڶ��ݻ����ļ�ʧ��:��Դ�ļ�(%s)ʧ��(%s)",
              szFirstFile,
              strerror(errno));
        return ERR_FAIL;
    }
        
    FILE* fSecond = fopen(szSecondFile, "wb");
    if(NULL == fSecond)
    {
        COTRACE("����ڶ��ݻ����ļ�ʧ��:��Ŀ���ļ�(%s)ʧ��(%s)",
              szSecondFile,
              strerror(errno));
        return ERR_FAIL;
    }

    int  nRet = ERR_SUCCESS;
    int  nRead = 0;
    char szBuffer[4096];
    char szOutput[4096];

    while(!feof(fFirst))
    {
        nRead = fread(szBuffer, 1, sizeof(szBuffer), fFirst);
        if (nRead < 0)
        {
            COTRACE("����ڶ��ݻ����ļ�ʧ��:��Դ�ļ�(%s)ʧ��(%s)",
                  szFirstFile,
                  strerror(errno));
            nRet = ERR_FAIL;
            break;
        }
        else if (nRead == 0)
        {
            break;
        }

        int  nWritePointor = 0;
        const char* pReadPointor = &szBuffer[0];
        while(nRead-- > 0)
        {
            if(pReadPointor[0] == ' ')
            {
                pReadPointor++;
                continue;
            }
            szOutput[nWritePointor++] = pReadPointor[0];
            pReadPointor++;
        }

        if(nWritePointor > 0)
        {
            int nWriteBytes = fwrite(szOutput, 1, nWritePointor, fSecond);
            if(nWriteBytes < nWritePointor)
            {
                COTRACE("����ڶ��ݻ����ļ�ʧ��:дĿ���ļ�(%s)ʧ��(%s)",
                      szSecondFile,
                      strerror(errno));
                nRet = ERR_FAIL;
                break;
            }
        }
    }

    fclose(fFirst);
    fflush(fSecond);
    fclose(fSecond);
    if (nRet == ERR_FAIL)
    {
        remove(szSecondFile);
    }

    return nRet;
}
