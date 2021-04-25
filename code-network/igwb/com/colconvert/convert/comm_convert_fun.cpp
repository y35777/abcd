#include "comm_convert_fun.h"
#include "../../include/base_type.h"
#include "../../include/toolbox.h"

int ProcessOtherFile(const char*     pszSrcFile,
                     VECTOR<STRING>& FinalFileNameList,
                     VECTOR<int>&    CvtResultList,
                     const int bFileLevelVerify,
                     const int bBillLevelVerify,
                     void* pIBillProcessor,
                     const int nOrigBillLen)

{
    if (FinalFileNameList.size() == 1)
    {
        if (!FileCopy(pszSrcFile, FinalFileNameList[0].c_str()))
        {
            return ERR_FAIL;
        }
        CvtResultList[0] = 1;
    }
    else if (FinalFileNameList.size() > 1)
    {
        if (!FileCopy(pszSrcFile, FinalFileNameList[1].c_str()))
        {
            return ERR_FAIL;
        }
        CvtResultList[1] = 1;
    }
    else
    {
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

void BCD2String(char *pBCD, char *pszStr, int nBCDNum)
{
    int             nBCDIndex = 0;
    int             nStrIndex = 0;
    unsigned char   yCurBCD;

    while (nBCDIndex < nBCDNum)
    {
        yCurBCD = pBCD[nBCDIndex++];

        //转换低4位
        if (yCurBCD < 0xA0)
        {
            pszStr[nStrIndex++] = (yCurBCD >> 4) + '0';
        }
        else
        {
            break;
        }

        //屏蔽当前BCD码数组值的高四位
        yCurBCD &= 0x0F;

        if (yCurBCD < 0x0A)
        {
            pszStr[nStrIndex++] = yCurBCD + '0';
        }
        else
        {
            break;
        }
    }

    pszStr[nStrIndex] = '\0';
}

int BCD2Bin(char *pBCD, int nBCDNum)
{
    int             nBCDIndex = 0;
    unsigned char   yCurBCD;
    int           nResult = 0;

    while (nBCDIndex < nBCDNum)
    {
        yCurBCD = pBCD[nBCDIndex++];

        //转换低4位
        if (yCurBCD < 0xA0)
        {
            nResult = nResult * 10 + (yCurBCD >> 4) ;
        }
        else
        {
            break;
        }

        //屏蔽当前BCD码数组值的高四位
        yCurBCD &= 0x0F;

        if (yCurBCD < 0x0A)
        {
            nResult = nResult * 10 + yCurBCD;
        }
        else
        {
            break;
        }
    }

    return nResult;
}

void ConvertBCDField2String(char* pBCD, char* pszDest, int nBCD, int nStr)
{
    char szTemp[128];

    memset(pszDest, ' ', nStr);
    BCD2String(pBCD, szTemp, nBCD);
    if (strlen(szTemp))
    {
        memcpy(pszDest, szTemp, strlen(szTemp));
    }
}

//校验只有月日的日期
int ValidateMDDate(const char* pszDate)
{
    char szMonth[3];
    char szDay[3];

    memcpy(szMonth, pszDate, 2);
    szMonth[2] = '\0';

    memcpy(szDay, pszDate + 3, 2);
    szDay[2] = '\0';

    int nDay, nMonth;

    //校验月
    if (!IsDigitValue(szMonth))
    {
        return ERR_FAIL;
    }

    nMonth = atoi(szMonth);
    if ((nMonth < 1)||(nMonth > 12))
    {
        return ERR_FAIL;
    }

    //校验日
    if (!IsDigitValue(szDay))
    {
        return ERR_FAIL;
    }

    nDay = atoi(szDay);
    if ((nDay < 1)||(nDay > 31))
    {
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

//校验两位年的日期
int Validate2YMDDate(const char* pszDate)
{
    char szYear[3];
    char szMonth[3];
    char szDay[3];

    memcpy(szYear, pszDate, 2);
    szYear[2] = '\0';

    memcpy(szMonth, pszDate + 3, 2);
    szMonth[2] = '\0';

    memcpy(szDay, pszDate + 6, 2);
    szDay[2] = '\0';

    int  nYear, nMonth, nDay;

    //校验年
    if (!IsDigitValue(szYear))
    {
        return ERR_FAIL;
    }

    //校验月
    if (!IsDigitValue(szMonth))
    {
        return ERR_FAIL;
    }

    nMonth = atoi(szMonth);
    if ((nMonth < 1)||(nMonth > 12))
    {
        return ERR_FAIL;
    }

    //校验日
    if (!IsDigitValue(szDay))
    {
        return ERR_FAIL;
    }

    nDay = atoi(szDay);
    if ((nDay < 1)||(nDay > 31))
    {
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

//校验四位年的日期
int Validate4YMDDate(const char* pszDate)
{
    char szYear[5];
    char szMonth[3];
    char szDay[3];

    memcpy(szYear, pszDate, 4);
    szYear[4] = '\0';

    memcpy(szMonth, pszDate + 5, 2);
    szMonth[2] = '\0';

    memcpy(szDay, pszDate + 8, 2);
    szDay[2] = '\0';

    int  nYear, nMonth, nDay;

    //校验年
    if (!IsDigitValue(szYear))
    {
        return ERR_FAIL;
    }

    nYear = atoi(szYear);
    if ((nYear < 1950)||(nYear > 2049))
    {
        return ERR_FAIL;
    }

    //校验月
    if (!IsDigitValue(szMonth))
    {
        return ERR_FAIL;
    }

    nMonth = atoi(szMonth);
    if ((nMonth < 1)||(nMonth > 12))
    {
        return ERR_FAIL;
    }

    //校验日
    if (!IsDigitValue(szDay))
    {
        return ERR_FAIL;
    }

    nDay = atoi(szDay);
    if ((nDay < 1)||(nDay > 31))
    {
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}


int ValidateTime(const char* pszTime)
{
    char szHour[3];
    char szMinute[3];
    char szSecond[3];

    memcpy(szHour, pszTime, 2);
    szHour[2] = '\0';

    memcpy(szMinute, pszTime + 3, 2);
    szMinute[2] = '\0';

    memcpy(szSecond, pszTime + 6, 2);
    szSecond[2] = '\0';


    int nHour, nMinute, nSecond;

    //校验时
    if (!IsDigitValue(szHour))
    {
        return ERR_FAIL;
    }

    nHour = atoi(szHour);
    if ((nHour < 0)||(nHour > 23))
    {
        return ERR_FAIL;
    }

     //校验分
    if (!IsDigitValue(szMinute))
    {
        return ERR_FAIL;
    }

    nMinute = atoi(szMinute);
    if ((nMinute < 0)||(nMinute > 59))
    {
        return ERR_FAIL;
    }

     //校验秒
    if (!IsDigitValue(szSecond))
    {
        return ERR_FAIL;
    }

    nSecond = atoi(szSecond);
    if ((nSecond < 0)||(nSecond > 59))
    {
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

