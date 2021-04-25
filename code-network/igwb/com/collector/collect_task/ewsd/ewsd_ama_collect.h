#ifndef _EWSD_AMA_COLLECT_H_
#define _EWSD_AMA_COLLECT_H_

#include "../collect_base.h"


class CEWSD_AMACollect
{
public:
    CEWSD_AMACollect(CLinkMgr *pLinkMgr,
                   int nSwitchIndex,
                   int nAMAIndex);
    virtual ~CEWSD_AMACollect();

    //��ʼ���ɼ�����
    int  Init();

    //�����ļ�����˵ĵ����ļ�������;
    int  GetFile(SColFileReq& stReq);


    //���ļ��б��в���ָ���ļ�
    int  FindFileInList(const char* const szFileName);

    //����ļ��б�
    int  GetList(SListOut& pOut);


    //��̬������Ŀ¼
    BOOL CreateLocSubDir();

    //��������ļ�
    BOOL ClearOldFile();

    //����ļ����к��Ƿ�����
    BOOL CheckFileSN(const char* szFileName, char* szPreviousFile);


    inline void SetCfgPath(const char* const szCfgPath)
    {
        strncpy(m_szCfgPath, szCfgPath, MAX_PATH);
        m_szCfgPath[MAX_PATH - 1] = '\0';
    }

    inline void SetFileSaveDays(const int nFileSaveDays)
    {
        m_nFileSaveDays = nFileSaveDays;
    }

    inline void SetSwitchName(const char* const szSwitchName)
    {
        strncpy(m_szSwitchName, szSwitchName, sizeof(m_szSwitchName));
        m_szSwitchName[sizeof(m_szSwitchName) - 1] = '\0';
    }

    //ָ�����ļ��Ƿ�ɲ�
    virtual int  CanGetSpecFile(const char* const szSrcFile);
    //�ɼ�ָ�����ļ�
    virtual int  GetSpecFile(const char* const szSrcFile,
                             const char* const szDstFile);
    //��ȡģ����Ϣ
    virtual int  GetModuleInfo(LIST<STRING>& label,
                               LIST<STRING>& content);

    //��ȡ�ļ�����
    virtual int  GetFileLen(const char* const szSrcFile);

    inline const char* GetModuleLabel()
    {
        return m_strModuleLabel.c_str();
    }

    int MakeFileNameMsg(const char* const szSrcFile,
                        SGetFileReq& req);

    int DoGetAFile(SGetFileReq& stReq);
protected:

    int LoadStatus();

    int SaveStatus();

    int IncreaseFileSN();

    char  m_szStatusFile[MAX_PATH];
    FILE* m_pStatusFile;


    STRING GetDstFileName(const char* const szSrcFile);

    //Ҫ�ɼ���EWSD��������AMA�ļ���
    STRING m_strAMAFile;

    //AMA�ļ��Ļ�������
    STRING m_strBillType;

    //AMA�ļ����ļ����к�
    UINT4  m_nFileSN;

    int    m_nFileSNLen;
    UINT4  m_nFileSNLimit;

    char m_szAreaCode[50]; //����
    char m_szBillType[50]; //��������


    int                   m_nSwitchIndex;
    int                   m_nAMAIndex;

    //������ŵĸ�Ŀ¼
    STRING m_strLocalRootPath;
    //������ŵĵ�ǰĿ¼
    STRING m_strLocalPath;

    //��ʱ�����ļ���ŵ�Ŀ¼
    STRING m_strTmpPath;

    //Զ�˽�������ԴĿ¼
    STRING m_strRemotePath;

    //�����ļ���������
    int    m_nFileSaveDays;   //added by chenliangwei


    CLinkMgr*               m_pLinkMgr;


    VECTOR<SFileInfo>       m_FileList;

    char  m_szCfgPath[MAX_PATH];    //��ǰ�����ļ�������·����Ϣ

    char  m_szSwitchName[40];       //��������

    //�Ƿ�����ļ�����У��
    BOOL m_bFileLenVerify;

    //ģ���ʶ��
    STRING      m_strModuleLabel;

    ACE_Mutex   m_MutexForList;
};


#endif //_EWSD_AMA_COLLECT_H_


