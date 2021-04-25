//转换S1240详细话单到文本格式

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../comm_convert_fun.h"
#include "../include/collect_convert_define.h"
#include "../../include/collect_convert_include.h"
#include "../../../include/toolbox.h"

//S1240详单的话单格式
struct S1240DetailSrcFmt
{
    char CallType[1];               //呼叫类型 BCD码，例如0x01、0x02、0x03等
    char CallerNum[5];              //主叫号码，BCD码，不足位用16进制的E填充
    char LocAttr[1];                //位置码属性，固定为1
    char CalledNum[8];              //被叫号码，BCD码，不足位用16进制的E填充
    char StartHour[1];              //起始时，BCD码
    char StartMinute[1];            //起始分，BCD码
    char StartSecond[1];            //起始秒，BCD码
    char EndHour[1];                //终止时，BCD码
    char EndMinute[1];              //终止分，BCD码
    char EndSecond[1];              //终止秒，BCD码
    char EndMonth[1];               //终止月，BCD码
    char EndDay[1];                 //终止日，BCD码
    char Duration[3];               //通话时长，BCD码
    char PulseNum[2];               //脉冲次数
    char OutTrunk[2];               //出中继群
};

struct S1240DetailCDRBlock
{
    unsigned char           nCDRType;                       //话单类型，固定为0x02
    unsigned char           BlockCsn[3];            //块顺序号
    S1240DetailSrcFmt       DetailCDR[68];          //每个块含
    unsigned char           Padding[4];                     //填充符，0xEE
};

//转换成文本格式的S1240详单格式
struct S1240DetailDestFmt
{
    char            szCallType[3];                          //呼叫类型
    char            szCallerNum[11];                        //主叫号码
    char            szLocAttr[3];                           //位置码属性
    char            szCalledNum[17];                        //被叫号码
    char            szStartHour[3];                         //起始时
    char            szStartMinute[3];                       //起始分
    char            szStartSecond[3];                       //起始秒
    char            szEndHour[3];                           //终止时
    char            szEndMinute[3];                         //终止分
    char            szEndSecond[3];                         //终止秒
    char            szEndMonth[3];                          //终止月
    char            szEndDay[3];                            //终止日
    char            szDuration[7];                              //通话时长
    char            szPulseNum[5];                          //脉冲计数
    char            szOutTrunk[5];                          //出中继
    char            szReturn[1];                            //回车
};

struct S1240MeterSrcFmt
{
    char    Meter1[2];
    char    Meter2[2];
    char    Meter3[2];
};

struct S1240MeterCDRBlock
{
    char                    CallType[1];            //呼叫类型，取值0x01或者0x03
    char                    BlockCsn[3];            //Block序号，BCD码
    char                    Titile1[4];             //字冠1，BCD码
    S1240MeterSrcFmt        Record1[100];           //百号组1
    char                    Titile2[4];             //字冠2，BCD码
    S1240MeterSrcFmt        Record2[100];           //百号组2
    char                    Titile3[4];             //字冠3，BCD码
    S1240MeterSrcFmt        Record3[100];           //百号组3
};

struct S1240MeterDestFmt
{
    char    szTitle[9];             //字冠
    char    szMeter1[5];    //计次表1
    char    szMeter2[5];    //计次表2
    char    szMeter3[5];    //计次表3
    char    szReturn[1];    //回车
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
               "打开话单文件(%s)失败",
               pszSrcFile);

        WRITELOG_F("打开话单文件(%s)失败",
                   pszSrcFile);

        return ERR_FAIL;
    }

    if ((fpOut = fopen(pszDestFile, "wb")) == NULL)
    {
        fclose(fpIn);

        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "打开最终话单文件(%s)失败",
               pszDestFile);

        WRITELOG_F("打开最终话单文件(%s)失败",
                   pszDestFile);

        return -2;
    }

    if ((fpErr = fopen(pszErrFile, "wb")) == NULL)
    {
        fclose(fpIn);
        fclose(fpOut);
        FileDel(pszDestFile);

        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "打开错误话单文件(%s)失败",
               pszErrFile);

        WRITELOG_F("打开错误话单文件(%s)失败",
                   pszErrFile);

        return -2;
    }


    //进行文件级校验。
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
                       "话单文件(%s)拷贝到错误文件(%s)失败",
                       pszSrcFile,
                       pszErrFile);

                WRITELOG_F("话单文件(%s)拷贝到错误文件(%s)失败",
                           pszSrcFile,
                           pszErrFile);
            }

            return ERR_FAIL;
        }
    }

    /*
     * 开始进行话单转换。
     */
    int nResult = ERR_SUCCESS;
    int nErrBillCount = 0;
    int nBillCount = 0;

    long nBlockNum = nSrcFileSize / sizeof(S1240DetailCDRBlock);

    S1240DetailCDRBlock CDRBlock;
    S1240DetailDestFmt  DestCDR;

    while (nBlockNum)
    {
        //读一个块的数据
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
                //进行话单级校验。
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
     * 对转换结果进行后续处理。
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
               "打开话单文件(%s)失败",
               pszSrcFile);

        WRITELOG_F("打开话单文件(%s)失败",
                   pszSrcFile);

        return -2;
    }

    if ((fpOut = fopen(pszDestFile, "wb")) == NULL)
    {
        fclose(fpIn);

        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "打开最终话单文件(%s)失败",
               pszDestFile);

        WRITELOG_F("打开最终话单文件(%s)失败",
                   pszDestFile);

        return -2;
    }

    if ((fpErr = fopen(pszErrFile, "wb")) == NULL)
    {
        fclose(fpIn);
        fclose(fpOut);
        FileDel(pszDestFile);

        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "打开错误话单文件(%s)失败",
               pszErrFile);

        WRITELOG_F("打开错误话单文件(%s)失败",
                   pszErrFile);

        return -2;
    }


    //进行文件级校验。
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
                       "话单文件(%s)拷贝到错误文件(%s)失败",
                       pszSrcFile,
                       pszErrFile);

                WRITELOG_F("话单文件(%s)拷贝到错误文件(%s)失败",
                           pszSrcFile,
                           pszErrFile);
            }

            return ERR_FAIL;
        }
    }

    /*
     * 开始进行话单转换。
     */
    long nBlockNum = nSrcFileSize / sizeof(S1240MeterCDRBlock);
    S1240MeterCDRBlock      CDRBlock;
    S1240MeterDestFmt   DestCDR;

    while (nBlockNum)
    {
        char szTitle[128];
        int n;

        //读一个块的数据
        fread(&CDRBlock, sizeof(S1240MeterCDRBlock), 1, fpIn);

        if (CDRBlock.CallType[0] == 0xee)
        {
            nBlockNum--;
            continue;
        }

        //转换第1个百分组
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

        //转换第2个百分组
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

        //转换第3个百分组
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



//字符串递增
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

    //校验结束日期
    nRet = ValidateMDDate((char*)DestCDR.szEndMonth);
    if ( nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //校验开始时间
    nRet = ValidateTime((char*)DestCDR.szStartHour);
    if ( nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //校验结束时间
    nRet = ValidateTime((char*)DestCDR.szEndHour);
    if ( nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    return nRet;

}
