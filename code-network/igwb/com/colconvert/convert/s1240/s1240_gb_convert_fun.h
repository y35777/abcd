#ifndef __S1240_GB_CONVERT_FUN_H__
#define __S1240_GB_CONVERT_FUN_H__

int ProcessS1240GBFile(const char*   pszSrcFile,
                       VECTOR<STRING>& FinalFileNameList,
                       VECTOR<int>&    CvtResultList,
                       const int bFileLevelVerify,
                       const int bBillLevelVerify,
                       void* pIBillProcessor = NULL,
                       const int nOrigBillLen = 0);



#endif //__COLLECT_GB_CONVERT_S1240_H__
