#ifndef __5ESS_CONVERT_FUN_H__
#define __5ESS_CONVERT_FUN_H__

int Process5essAMAFile(const char*   pszSrcFile,
                     VECTOR<STRING>& FinalFileNameList,
                     VECTOR<int>&    CvtResultList,
                     const int bFileLevelVerify,
                     const int bBillLevelVerify,
                     void* pIBillProcessor = NULL,
                     const int nOrigBillLen = 0);

int Process5essMMFile(const char*    pszSrcFile,
                     VECTOR<STRING>& FinalFileNameList,
                     VECTOR<int>&    CvtResultList,
                     const int bFileLevelVerify,
                     const int bBillLevelVerify,
                     void* pIBillProcessor = NULL,
                     const int nOrigBillLen = 0); 


#endif //__5ESS_CONVERT_FUN_H__

