#ifndef __TELLIN_CONVERT_FUN_H__
#define __TELLIN_CONVERT_FUN_H__

int ProcTELLINFile(const char*   pszSrcFile,
                   VECTOR<STRING>& FinalFileNameList,
                   VECTOR<int>&    CvtResultList,
                   const int bFileLevelVerify,
                   const int bBillLevelVerify,
                   void* pIBillProcessor = NULL,
                   const int nOrigBillLen = 0);

int SaveSecondTELLIN(const char* szFirstFile, const char* szSecondFile);

#endif //__TELLIN_CONVERT_FUN_H__

