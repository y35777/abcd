#ifndef _FILE_SN_MANAGER_H__
#define _FILE_SN_MANAGER_H__

#include "../../include/base_type.h"


#define LEN_OF_FILESNMGR_INFO   256
#define LEN_OF_FILE_NAME        60

typedef STRING (*GETFILENAMEBYSNFUN)(const int   nFileSN,
                                     const char* szReferName,
                                     const int   nSNLen,
                                     const int   nPostfixLen);
 
//�������ļ����кŹ�����CFileSNMgr�Ķ���
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

    //�Ե�ǰ�ļ������кŽ��м��
    BOOL CheckFileSN(const char* szFile, char* szPreviousFile);

    //���õ�ǰ�ļ���
    void SetFileName(const char* szFileName);


    //����״̬�ļ���
    inline void SetStatusFileName(const char* szStatusFile)
    {
        strcpy(m_szStatusFile, szStatusFile);
    }
    //�õ�״̬�ļ���
    inline char* GetStatusFileName()
    {
        return m_szStatusFile;
    }

    //���ü�����к���
    inline void SetCheckSNDepth(const int& nCheckSNDepth)
    {
        m_nCheckSNDepth = nCheckSNDepth;
    }
    //�õ�������к���
    inline int GetCheckSNDepth()
    {
        return m_nCheckSNDepth;
    }

    //�����ļ���׺�ĳ���
    inline void SetPostfixLen(const int& nPostfixLen)
    {
        m_nPostfixLen = nPostfixLen;
    }

    //�������кŵĳ���
    void SetSNLen(const int& nSNLen);
    //�������кŵ�����
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
    

    //ͨ���ļ������кż���©���ļ���
    static STRING GetFileNameBySN(const int   nFileSN,
                                  const char* szReferName,
                                  const int   nSNLen,
                                  const int   nPostfixLen);

protected:
    //�õ��ļ����к�
    int GetFileSNByName(const char* szFileName);

    int LoadStatus();

    int SaveStatus();

    //�Ƿ���״̬�ļ���־
    BOOL m_bNewStatusFile;

    char m_szMgrInfo[LEN_OF_FILESNMGR_INFO];

    //������к����
    int m_nCheckSNDepth;

    //�ļ����кŵ�����
    int m_nSNDownLimit;
    //�ļ����кŵ�����
    int m_nSNUpLimit;

    int m_nSNLen;
    int m_nPostfixLen;

    //��ǰ�Ѳɼ��ļ������к�
    UINT4    m_nFileSN;

    //��ǰ�Ѳɼ����ļ���
    char     m_szFileName[LEN_OF_FILE_NAME];

    //©���ļ��б�
    VECTOR<UINT4> m_LostFileSNlist;

    GETFILENAMEBYSNFUN m_pGetNameFun;

    char  m_szStatusFile[MAX_PATH];
    FILE* m_pStatusFile;

};

#endif //_FILE_SN_MANAGER_H__

