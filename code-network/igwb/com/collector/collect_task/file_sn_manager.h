#ifndef _FILE_SN_MANAGER_H__
#define _FILE_SN_MANAGER_H__

#include "../../include/base_type.h"


#define LEN_OF_FILESNMGR_INFO   256
#define LEN_OF_FILE_NAME        60

typedef STRING (*GETFILENAMEBYSNFUN)(const int   nFileSN,
                                     const char* szReferName,
                                     const int   nSNLen,
                                     const int   nPostfixLen);
 
//以下是文件序列号管理器CFileSNMgr的定义
class  CFileSNMgr
{

public:

    CFileSNMgr(const char* szStatusFile, 
                int nCheckSNDepth, 
                int nSNLen,
                int nPostfixLen = 0,
                GETFILENAMEBYSNFUN pGetNameFun = NULL);
    ~CFileSNMgr();

    int Init();

    //对当前文件的序列号进行检查
    BOOL CheckFileSN(const char* szFile, char* szPreviousFile);

    //设置当前文件名
    void SetFileName(const char* szFileName);


    //设置状态文件名
    inline void SetStatusFileName(const char* szStatusFile)
    {
        strcpy(m_szStatusFile, szStatusFile);
    }
    //得到状态文件名
    inline char* GetStatusFileName()
    {
        return m_szStatusFile;
    }

    //设置检查序列号深
    inline void SetCheckSNDepth(const int& nCheckSNDepth)
    {
        m_nCheckSNDepth = nCheckSNDepth;
    }
    //得到检查序列号深
    inline int GetCheckSNDepth()
    {
        return m_nCheckSNDepth;
    }

    //设置文件后缀的长度
    inline void SetPostfixLen(const int& nPostfixLen)
    {
        m_nPostfixLen = nPostfixLen;
    }

    //设置序列号的长度
    void SetSNLen(const int& nSNLen);
    //设置序列号的上限
    inline void SetSNLimit(int nSNUpLimit,int nSNDownLimit = 0)
    {
        m_nSNUpLimit   = nSNUpLimit;
        m_nSNDownLimit = nSNDownLimit;
    }


    inline void SetMgrInfo(const char* szMgrInfo)
    {
        strcpy(m_szMgrInfo, szMgrInfo);
    }

    inline char* GetMgrInfo()
    {
        return m_szMgrInfo;
    }

    inline void SetGetNameFun(GETFILENAMEBYSNFUN pGetNameFun)
    {
        m_pGetNameFun = pGetNameFun;
    }
    

    //通过文件的序列号计算漏采文件名
    static STRING GetFileNameBySN(const int   nFileSN,
                                  const char* szReferName,
                                  const int   nSNLen,
                                  const int   nPostfixLen);

protected:
    //得到文件序列号
    int GetFileSNByName(const char* szFileName);

    int LoadStatus();

    int SaveStatus();

    //是否新状态文件标志
    BOOL m_bNewStatusFile;

    char m_szMgrInfo[LEN_OF_FILESNMGR_INFO];

    //检查序列号深度
    int m_nCheckSNDepth;

    //文件序列号的下限
    int m_nSNDownLimit;
    //文件序列号的上限
    int m_nSNUpLimit;

    int m_nSNLen;
    int m_nPostfixLen;

    //当前已采集文件的序列号
    UINT4    m_nFileSN;

    //当前已采集的文件名
    char     m_szFileName[LEN_OF_FILE_NAME];

    //漏采文件列表
    VECTOR<UINT4> m_LostFileSNlist;

    GETFILENAMEBYSNFUN m_pGetNameFun;

    char  m_szStatusFile[MAX_PATH];
    FILE* m_pStatusFile;

};

#endif //_FILE_SN_MANAGER_H__

