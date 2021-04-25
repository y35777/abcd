//ת��S1240��ϸ�������ı���ʽ

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../comm_convert_fun.h"
#include "../include/collect_convert_define.h"
#include "../../include/collect_convert_include.h"
#include "../../../include/toolbox.h"

//S1240�굥�Ļ�����ʽ
struct S1240DetailSrcFmt
{
    char CallType[1];               //�������� BCD�룬����0x01��0x02��0x03��
    char CallerNum[5];              //���к��룬BCD�룬����λ��16���Ƶ�E���
    char LocAttr[1];                //λ�������ԣ��̶�Ϊ1
    char CalledNum[8];              //���к��룬BCD�룬����λ��16���Ƶ�E���
    char StartHour[1];              //��ʼʱ��BCD��
    char StartMinute[1];            //��ʼ�֣�BCD��
    char StartSecond[1];            //��ʼ�룬BCD��
    char EndHour[1];                //��ֹʱ��BCD��
    char EndMinute[1];              //��ֹ�֣�BCD��
    char EndSecond[1];              //��ֹ�룬BCD��
    char EndMonth[1];               //��ֹ�£�BCD��
    char EndDay[1];                 //��ֹ�գ�BCD��
    char Duration[3];               //ͨ��ʱ����BCD��
    char PulseNum[2];               //�������
    char OutTrunk[2];               //���м�Ⱥ
};

struct S1240DetailCDRBlock
{
    unsigned char           nCDRType;                       //�������ͣ��̶�Ϊ0x02
    unsigned char           BlockCsn[3];            //��˳���
    S1240DetailSrcFmt       DetailCDR[68];          //ÿ���麬
    unsigned char           Padding[4];                     //������0xEE
};

//ת�����ı���ʽ��S1240�굥��ʽ
struct S1240DetailDestFmt
{
    char            szCallType[3];                          //��������
    char            szCallerNum[11];                        //���к���
    char            szLocAttr[3];                           //λ��������
    char            szCalledNum[17];                        //���к���
    char            szStartHour[3];                         //��ʼʱ
    char            szStartMinute[3];                       //��ʼ��
    char            szStartSecond[3];                       //��ʼ��
    char            szEndHour[3];                           //��ֹʱ
    char            szEndMinute[3];                         //��ֹ��
    char            szEndSecond[3];                         //��ֹ��
    char            szEndMonth[3];                          //��ֹ��
    char            szEndDay[3];                            //��ֹ��
    char            szDuration[7];                              //ͨ��ʱ��
    char            szPulseNum[5];                          //�������
    char            szOutTrunk[5];                          //���м�
    char            szReturn[1];                            //�س�
};

struct S1240MeterSrcFmt
{
    char    Meter1[2];
    char    Meter2[2];
    char    Meter3[2];
};

struct S1240MeterCDRBlock
{
    char                    CallType[1];            //�������ͣ�ȡֵ0x01����0x03
    char                    BlockCsn[3];            //Block��ţ�BCD��
    char                    Titile1[4];             //�ֹ�1��BCD��
    S1240MeterSrcFmt        Record1[100];           //�ٺ���1
    char                    Titile2[4];             //�ֹ�2��BCD��
    S1240MeterSrcFmt        Record2[100];           //�ٺ���2
    char                    Titile3[4];             //�ֹ�3��BCD��
    S1240MeterSrcFmt        Record3[100];           //�ٺ���3
};

struct S1240MeterDestFmt
{
    char    szTitle[9];             //�ֹ�
    char    szMeter1[5];    //�ƴα�1
    char    szMeter2[5];    //�ƴα�2
    char    szMeter3[5];    //�ƴα�3
    char    szReturn[1];    //�س�
};


int ConvertAS1240DetailCDR(S1240DetailSrcFmt SrcCDR, S1240DetailDestFmt& DestCDR);
int ValideS1240DetailCDR(S1240DetailDestFmt& DestCDR);

void StringInc(char* pszStr);



int ProcessS1240DetailFile(const char* pszSrcFile,
                           const char* pszDestFile,
                           const char* pszErrFile,
                           const int bFileLevelVerify,
                           const int bBillLevelVerify)
{

    FILE* fpIn;
    FILE* fpOut;
    FILE* fpErr;

    if ((fpIn = fopen(pszSrcFile, "rb")) == NULL)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�򿪻����ļ�(%s)ʧ��",
               pszSrcFile);

        WRITELOG_F("�򿪻����ļ�(%s)ʧ��",
                   pszSrcFile);

        return ERR_FAIL;
    }

    if ((fpOut = fopen(pszDestFile, "wb")) == NULL)
    {
        fclose(fpIn);

        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�����ջ����ļ�(%s)ʧ��",
               pszDestFile);

        WRITELOG_F("�����ջ����ļ�(%s)ʧ��",
                   pszDestFile);

        return -2;
    }

    if ((fpErr = fopen(pszErrFile, "wb")) == NULL)
    {
        fclose(fpIn);
        fclose(fpOut);
        FileDel(pszDestFile);

        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�򿪴��󻰵��ļ�(%s)ʧ��",
               pszErrFile);

        WRITELOG_F("�򿪴��󻰵��ļ�(%s)ʧ��",
                   pszErrFile);

        return -2;
    }


    //�����ļ���У�顣
    long nSrcFileSize = FileSize(fpIn);

    if (bFileLevelVerify)
    {
        if (nSrcFileSize % sizeof(S1240DetailCDRBlock) != 0)
        {
            fclose(fpIn);
            fclose(fpOut);
            fclose(fpErr);

            FileDel(pszDestFile);
            FileDel(pszErrFile);

            if (!FileCopy(pszSrcFile, pszErrFile))
            {
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                       "�����ļ�(%s)�����������ļ�(%s)ʧ��",
                       pszSrcFile,
                       pszErrFile);

                WRITELOG_F("�����ļ�(%s)�����������ļ�(%s)ʧ��",
                           pszSrcFile,
                           pszErrFile);
            }

            return ERR_FAIL;
        }
    }

    /*
     * ��ʼ���л���ת����
     */
    int nResult = ERR_SUCCESS;
    int nErrBillCount = 0;
    int nBillCount = 0;

    long nBlockNum = nSrcFileSize / sizeof(S1240DetailCDRBlock);

    S1240DetailCDRBlock CDRBlock;
    S1240DetailDestFmt  DestCDR;

    while (nBlockNum)
    {
        //��һ���������
        fread(&CDRBlock, sizeof(S1240DetailCDRBlock), 1, fpIn);
        if (CDRBlock.nCDRType == 0xee)
        {
            nBlockNum--;
            continue;
        }

        int n;
        for (n = 0; n < 68; n++)
        {
            if (CDRBlock.DetailCDR[n].CallType[0] == 0xee)
            {
                continue;
            }

            ConvertAS1240DetailCDR(CDRBlock.DetailCDR[n], DestCDR);

            if (bBillLevelVerify)
            {
                //���л�����У�顣
                nResult = ValideS1240DetailCDR(DestCDR);
                if (ERR_SUCCESS != nResult)
                {
                    nResult = fwrite(&CDRBlock.DetailCDR[n], sizeof(S1240DetailSrcFmt), 1, fpErr);

                    nErrBillCount++;
                }
                else
                {
                    fwrite(&DestCDR, sizeof(S1240DetailDestFmt), 1, fpOut);
                }
            }
            else
            {
                fwrite(&DestCDR, sizeof(S1240DetailDestFmt), 1, fpOut);
            }

            nBillCount++;
        }

        nBlockNum--;
    }

    fclose(fpIn);
    fclose(fpOut);
    fclose(fpErr);

    /*
     * ��ת��������к�������
     */
    if (nErrBillCount == 0)
    {
        FileDel(pszErrFile);
        nResult = ERR_SUCCESS;
    }
    else if (nErrBillCount == nBillCount)
    {
        FileDel(pszDestFile);
        nResult = ERR_FAIL;
    }
    else
    {
        nResult = COL_CONVERT_PART_SUCC;
    }


    return nResult;
}

int ProcessS1240MeterFile(const char* pszSrcFile,
                          const char* pszDestFile,
                          const char* pszErrFile,
                          const int bFileLevelVerify,
                          const int bBillLevelVerify)

{
    FILE* fpIn;
    FILE* fpOut;
    FILE* fpErr;

    if ((fpIn = fopen(pszSrcFile, "rb")) == NULL)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�򿪻����ļ�(%s)ʧ��",
               pszSrcFile);

        WRITELOG_F("�򿪻����ļ�(%s)ʧ��",
                   pszSrcFile);

        return -2;
    }

    if ((fpOut = fopen(pszDestFile, "wb")) == NULL)
    {
        fclose(fpIn);

        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�����ջ����ļ�(%s)ʧ��",
               pszDestFile);

        WRITELOG_F("�����ջ����ļ�(%s)ʧ��",
                   pszDestFile);

        return -2;
    }

    if ((fpErr = fopen(pszErrFile, "wb")) == NULL)
    {
        fclose(fpIn);
        fclose(fpOut);
        FileDel(pszDestFile);

        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�򿪴��󻰵��ļ�(%s)ʧ��",
               pszErrFile);

        WRITELOG_F("�򿪴��󻰵��ļ�(%s)ʧ��",
                   pszErrFile);

        return -2;
    }


    //�����ļ���У�顣
    long nSrcFileSize = FileSize(fpIn);

    if (bFileLevelVerify)
    {
        if (nSrcFileSize % sizeof(S1240MeterCDRBlock) != 0)
        {
            fclose(fpIn);
            fclose(fpOut);
            fclose(fpErr);

            FileDel(pszDestFile);
            FileDel(pszErrFile);

            if (!FileCopy(pszSrcFile, pszErrFile))
            {
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                       "�����ļ�(%s)�����������ļ�(%s)ʧ��",
                       pszSrcFile,
                       pszErrFile);

                WRITELOG_F("�����ļ�(%s)�����������ļ�(%s)ʧ��",
                           pszSrcFile,
                           pszErrFile);
            }

            return ERR_FAIL;
        }
    }

    /*
     * ��ʼ���л���ת����
     */
    long nBlockNum = nSrcFileSize / sizeof(S1240MeterCDRBlock);
    S1240MeterCDRBlock      CDRBlock;
    S1240MeterDestFmt   DestCDR;

    while (nBlockNum)
    {
        char szTitle[128];
        int n;

        //��һ���������
        fread(&CDRBlock, sizeof(S1240MeterCDRBlock), 1, fpIn);

        if (CDRBlock.CallType[0] == 0xee)
        {
            nBlockNum--;
            continue;
        }

        //ת����1���ٷ���
        BCD2String(CDRBlock.Titile1, szTitle, 4);
        strcat(szTitle, "00");
        for (n = 0; n < 100; n++)
        {
            memset(DestCDR.szTitle, ' ', sizeof(DestCDR.szTitle));
            memcpy(DestCDR.szTitle, szTitle, strlen(szTitle));

            ConvertBCDField2String(CDRBlock.Record1[n].Meter1,
                                       DestCDR.szMeter1, 2, sizeof(DestCDR.szMeter1));
            ConvertBCDField2String(CDRBlock.Record1[n].Meter2,
                                       DestCDR.szMeter2, 2, sizeof(DestCDR.szMeter2));
            ConvertBCDField2String(CDRBlock.Record1[n].Meter3,
                                       DestCDR.szMeter3, 2, sizeof(DestCDR.szMeter3));
            DestCDR.szReturn[0] = '\n';

            fwrite(&DestCDR, sizeof(S1240MeterDestFmt), 1, fpOut);

            StringInc(szTitle);
        }

        //ת����2���ٷ���
        BCD2String(CDRBlock.Titile2, szTitle, 4);
        strcat(szTitle, "00");
        for (n = 0; n < 100; n++)
        {
            memset(DestCDR.szTitle, ' ', sizeof(DestCDR.szTitle));
            memcpy(DestCDR.szTitle, szTitle, strlen(szTitle));

            ConvertBCDField2String(CDRBlock.Record2[n].Meter1,
                                       DestCDR.szMeter1, 2, sizeof(DestCDR.szMeter1));
            ConvertBCDField2String(CDRBlock.Record2[n].Meter2,
                                       DestCDR.szMeter2, 2, sizeof(DestCDR.szMeter2));
            ConvertBCDField2String(CDRBlock.Record2[n].Meter3,
                                       DestCDR.szMeter3, 2, sizeof(DestCDR.szMeter3));
            DestCDR.szReturn[0] = '\n';

            fwrite(&DestCDR, sizeof(S1240MeterDestFmt), 1, fpOut);

            StringInc(szTitle);
        }

        //ת����3���ٷ���
        BCD2String(CDRBlock.Titile3, szTitle, 4);
        strcat(szTitle, "00");
        for (n = 0; n < 100; n++)
        {
            memset(DestCDR.szTitle, ' ', sizeof(DestCDR.szTitle));
            memcpy(DestCDR.szTitle, szTitle, strlen(szTitle));

            ConvertBCDField2String(CDRBlock.Record3[n].Meter1,
                                       DestCDR.szMeter1, 2, sizeof(DestCDR.szMeter1));
            ConvertBCDField2String(CDRBlock.Record3[n].Meter2,
                                       DestCDR.szMeter2, 2, sizeof(DestCDR.szMeter2));
            ConvertBCDField2String(CDRBlock.Record3[n].Meter3,
                                       DestCDR.szMeter3, 2, sizeof(DestCDR.szMeter3));
            DestCDR.szReturn[0] = '\n';

            fwrite(&DestCDR, sizeof(S1240MeterDestFmt), 1, fpOut);

            StringInc(szTitle);
        }

        nBlockNum--;
    }

    fclose(fpIn);
    fclose(fpOut);
    fclose(fpErr);

    FileDel(pszErrFile);

    return 0;
}

int ConvertAS1240DetailCDR(S1240DetailSrcFmt SrcCDR, S1240DetailDestFmt& DestCDR)
{
    ConvertBCDField2String(SrcCDR.CallType, DestCDR.szCallType, 1, sizeof(DestCDR.szCallType));

    ConvertBCDField2String(SrcCDR.CallerNum, DestCDR.szCallerNum, 5, sizeof(DestCDR.szCallerNum));

    ConvertBCDField2String(SrcCDR.LocAttr, DestCDR.szLocAttr, 1, sizeof(DestCDR.szLocAttr));

    ConvertBCDField2String(SrcCDR.CalledNum, DestCDR.szCalledNum, 8, sizeof(DestCDR.szCalledNum));

    ConvertBCDField2String(SrcCDR.StartHour, DestCDR.szStartHour, 1, sizeof(DestCDR.szStartHour));
    DestCDR.szStartHour[2] = ':';

    ConvertBCDField2String(SrcCDR.StartMinute, DestCDR.szStartMinute, 1, sizeof(DestCDR.szStartMinute));
    DestCDR.szStartMinute[2] = ':';

    ConvertBCDField2String(SrcCDR.StartSecond, DestCDR.szStartSecond, 1, sizeof(DestCDR.szStartSecond));

    ConvertBCDField2String(SrcCDR.EndHour, DestCDR.szEndHour, 1, sizeof(DestCDR.szEndHour));
    DestCDR.szEndHour[2] = ':';

    ConvertBCDField2String(SrcCDR.EndMinute, DestCDR.szEndMinute, 1, sizeof(DestCDR.szEndMinute));
    DestCDR.szEndMinute[2] = ':';

    ConvertBCDField2String(SrcCDR.EndSecond, DestCDR.szEndSecond, 1, sizeof(DestCDR.szEndSecond));

    ConvertBCDField2String(SrcCDR.EndMonth, DestCDR.szEndMonth, 1, sizeof(DestCDR.szEndMonth));
    DestCDR.szEndMonth[2] = '-';

    ConvertBCDField2String(SrcCDR.EndDay, DestCDR.szEndDay, 1, sizeof(DestCDR.szEndDay));

    ConvertBCDField2String(SrcCDR.Duration, DestCDR.szDuration, 3, sizeof(DestCDR.szDuration));

    ConvertBCDField2String(SrcCDR.PulseNum, DestCDR.szPulseNum, 2, sizeof(DestCDR.szPulseNum));

    ConvertBCDField2String(SrcCDR.OutTrunk, DestCDR.szOutTrunk, 2, sizeof(DestCDR.szOutTrunk));

    DestCDR.szReturn[0] = '\n';

    return ERR_SUCCESS;
}



//�ַ�������
void StringInc(char* pszStr)
{
    if (!pszStr)
    {
        return;
    }

    int nLen = strlen(pszStr);
    if (0 == nLen)
    {
        return;
    }

    int nIndex = nLen - 1;
    char cOne = 49;

    while (nIndex >= 0)
    {
        if ('9' == pszStr[nIndex])
        {
            pszStr[nIndex] = '0';
            nIndex--;
        }
        else
        {
            pszStr[nIndex] ++;
            return;
        }
    }
}

int ValideS1240DetailCDR(S1240DetailDestFmt& DestCDR)
{
    int nRet = ERR_SUCCESS;

    //У���������
    nRet = ValidateMDDate((char*)DestCDR.szEndMonth);
    if ( nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //У�鿪ʼʱ��
    nRet = ValidateTime((char*)DestCDR.szStartHour);
    if ( nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //У�����ʱ��
    nRet = ValidateTime((char*)DestCDR.szEndHour);
    if ( nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    return nRet;

}
