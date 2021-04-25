#ifndef __EWSD_AMA_CONVERT_FUN_H__
#define __EWSD_AMA_CONVERT_FUN_H__

int ProcessEWSDAMAFile(const char* pszSrcFile,
                       const char* pszDestFile,
                       const char* pszErrFile,
                       const int bFileLevelVerify,
                       const int bBillLevelVerify);


#endif //__EWSD_AMA_CONVERT_FUN_H__

