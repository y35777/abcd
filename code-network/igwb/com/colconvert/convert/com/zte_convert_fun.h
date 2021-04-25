 #ifndef __ZTE_CONVERT_FUN_H__
#define __ZTE_CONVERT_FUN_H__

int ProcessZTEFile(const char*   pszSrcFile,
                     VECTOR<STRING>& FinalFileNameList,
                     VECTOR<int>&    CvtResultList,
                     const int bFileLevelVerify,
                     const int bBillLevelVerify,
                     void* pIBillProcessor = NULL,
                     const int nOrigBillLen = 0);


int ProcessZTEMeterFile(const char*   pszSrcFile,
                     VECTOR<STRING>& FinalFileNameList,
                     VECTOR<int>&    CvtResultList,
                     const int bFileLevelVerify,
                     const int bBillLevelVerify,
                     void* pIBillProcessor = NULL,
                     const int nOrigBillLen = 0);



#endif //__ZTE_CONVERT_FUN_H__

