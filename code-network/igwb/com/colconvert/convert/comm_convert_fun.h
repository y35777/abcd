#ifndef __COMM_CONVERT_FUN_H__
#define __COMM_CONVERT_FUN_H__

#include "../include/collect_convert_include.h"

int ProcessOtherFile(const char*     pszSrcFile,
                     VECTOR<STRING>& FinalFileNameList,
                     VECTOR<int>&    CvtResultList,
                     const int bFileLevelVerify,
                     const int bBillLevelVerify,
                     void* pIBillProcessor = NULL,
                     const int nOrigBillLen = 0);


void ConvertBCDField2String(char* pBCD, 
                            char* pszDest, 
                            int nBCD, 
                            int nStr);

void BCD2String(char *pBCD, char *pszStr, int nBCDNum);

int BCD2Bin(char *pBCD, int nBCDNum);

//校验只有月日的日期
int ValidateMDDate(const char* pszDate);

//校验两位年的日期
int Validate2YMDDate(const char* pszDate);

//校验四位年的日期
int Validate4YMDDate(const char* pszDate);

//校验时间
int ValidateTime(const char* pszTime);

#endif //__COLLECT_CONVERT_FUN_H__

