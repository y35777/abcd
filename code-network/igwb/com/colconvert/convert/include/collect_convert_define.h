#ifndef __COLLECT_CONVERT_DEFINE_H__
#define __COLLECT_CONVERT_DEFINE_H__


//�������ݽṹ����

#define CFG_BUF_SIZE  60
#define DEFAULT_CHL   "default"

typedef int (*CONVERTFILEFUN)(const char*     szSrcFile,
                              VECTOR<STRING>& FinalFileNameList,
                              VECTOR<int>&    CvtResultList,
                              const int bFileLevelVerify,
                              const int bBillLevelVerify,
                              void* pIBillProcessor,
                              const int nOrigBillLen);

#define  COL_CONVERT_PART_SUCC  4000


#endif //__COLLECT_CONVERT_DEFINE_H__
