#ifndef __CC08_CONVERT_FUN_H__
#define __CC08_CONVERT_FUN_H__

int ProcCC08File(const char*   pszSrcFile,
                 VECTOR<STRING>& FinalFileNameList,
                 VECTOR<int>&    CvtResultList,
                 const int bFileLevelVerify,
                 const int bBillLevelVerify,
                 void* pIBillProcessor = NULL,
                 const int nOrigBillLen = 0);


#endif //__CC08_CONVERT_FUN_H__

