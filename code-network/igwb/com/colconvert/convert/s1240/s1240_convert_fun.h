#ifndef __S1240_CONVERT_FUN_H__
#define __S1240_CONVERT_FUN_H__

int ProcessS1240DetailFile(const char* pszSrcFile,
                           const char* pszDestFile,
                           const char* pszErrFile,
                           const int bFileLevelVerify,
                           const int bBillLevelVerify);


int ProcessS1240MeterFile(const char* pszSrcFile,
                          const char* pszDestFile,
                           const char* pszErrFile,
                           const int bFileLevelVerify,
                           const int bBillLevelVerify);

#endif //__COLLECT_CONVERT_S1240_H__

