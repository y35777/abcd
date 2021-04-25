#ifndef __S1240_TP_COLLECT_H__
#define __S1240_TP_COLLECT_H__

#include "../collect_base.h"
#include "../../include/i_cmise_fun.h"

#define S1240_DS_DUMPED         "dumped"
#define S1240_DS_RELEASED       "released"

#define S1240_FILE_SN_LIMIT      200
#define S1240_FILE_SN_LEN        3
#define BUF_SIZE                 256

#define S1240_FILE_PREFIX        "TP-FILE-"

class CFileSNMgr;


//SS1240FileStatus����ÿ��S1240���������ļ���Ϣ
struct SS1240FileStatus
{
    SS1240FileMsg file_msg;     //�ļ�Ŀǰ��״̬��Ϣ
    int  nLastFileCounter;      //�ϴβɼ�ʱ���ļ���������
    int  nLastSetStatus;        //�������ϴ����õ�״̬
    int  nFinalStatus;          //Ŀ��״̬(-1:null|dumped|release)
    int  nStatus;               //�Ƿ����ڲɼ���״̬ E_COLLECT_STATUS
    BOOL bAdditional;           //�Ƿ��Ǹ���Ҫ��ɼ����ļ�
    char szDstFile[MAX_PATH];   //Ŀ���ļ���
};


class CS1240TPCollect
{
public:
    CS1240TPCollect(CLinkMgr *pPriLinkMgr,
                    CLinkMgr *pBakLinkMgr,
                    CS1240CmiseMgr* pCmiseMgr,
                    int nSwitchIndex,
                    int nTPIndex);
    virtual ~CS1240TPCollect();

    //��ʼ���ɼ�����
    int  Init();

    const SS1240TPInfo& GetTPInfo()
    {
        return m_TPInfo;
    }

    int MakeFileNameMsg(const char* const szSrcFile,
                        SGetFileReq& req);

    //ȡ�ɲɼ��ļ��б�
    int GetList(VECTOR<SS1240FileMsg>& pOut);

    //�����ļ�����˵ĵ����ļ������أ������ļ�״̬Ǩ��;
    int  GetFile(SColFileReq& stReq);

    //�����ļ�����˵ĵ����ļ������أ�ֻ��ȡ�ļ�;
    int  DoGetAFile(SColFileReq& stReq);

    //����ļ��б�
    int  GetAllFileList();

    //���ļ��б��в���ָ���ļ�
    int  FindFileInList(const char* const szFileName);

    //��������ļ��б�
    int  GetAllList(MAP<STRING, SS1240FileStatus>& Out);

    //���alarmlimit
    int  GetFilePool(SS1240FilePoolMsg& pool_msg);

    //���alarmlimit
    int  GetAlarmLimit();

    //����alarmlimit
    int  SetAlarmLimit(int nAlarmLimit);

    //��̬������Ŀ¼
    BOOL CreateLocSubDir();

    //��������ļ�
    BOOL ClearOldFile();

    //����ļ����к��Ƿ�����
    BOOL CheckFileSN(const char* szFileName, char* szPreviousFile);


    void SetCfgPath(const char* const szCfgPath);

    void SetFileSaveDays(int nFileSaveDays);

    inline void SetSwitchName(const char* const szSwitchName)
    {
        strncpy(m_szSwitchName, szSwitchName, sizeof(m_szSwitchName));
        m_szSwitchName[sizeof(m_szSwitchName) - 1] = '\0';
    }

    //�������ӷ����
    virtual int  TestLink(MAP<STRING, SLinkStatusMsg>& LinkStatus);

    //ָ�����ļ��Ƿ�ɽ����ֶ��ɼ�
    virtual int  CanGetSpecFile(const char* const szSrcFile);

    //ָ�����ļ��Ƿ�ɽ����Զ��ɼ�
    virtual int  CanAutoGet(const char* const szFileName);

    //��ȡ�ļ�����
    virtual int  GetFileLen(const char* const szSrcFile);

    //�ļ��������Ļص�������ͨ���ļ������кż���©���ļ���
    static STRING GetFileNameBySN(const int   nFileSN,
                                  const char* szReferName,
                                  const int   nSNLen,
                                  const int   nPostfixLen);

protected:

    int LoadStatus();

    int SaveStatus();

    STRING GetDstFileName(const char* const szSrcFile);

    int RestoreOperatingFile(const char* const szFileName);

    CLinkMgr*               m_pPriLinkMgr;
    CLinkMgr*               m_pBakLinkMgr;
    CS1240CmiseMgr*         m_pCmiseMgr;
    int                     m_nSwitchIndex;
    int                     m_nTPIndex;
    int                     m_nCoFailAlarmLimit;

    SS1240TPInfo            m_TPInfo;

    MAP<STRING, SS1240FileStatus>   m_FileList;
    ACE_Recursive_Thread_Mutex      m_MutexForMap;
    int                             m_nCollectingFile;

    char  m_szCfgPath[MAX_PATH];    //��ǰ�����ļ�������·����Ϣ

    //������ŵĵ�ǰĿ¼
    char  m_szLocalPath[MAX_PATH];

    //��ʱ�����ļ���ŵ�Ŀ¼
    STRING m_strTmpPath;

    char  m_szStatusFile[MAX_PATH];
    FILE* m_pStatusFile;

    //�����õ�Զ��Ŀ¼
    char  m_szRemotePath[MAX_PATH];

    //�����ļ���������
    int   m_nFileSaveDays;   //added by chenliangwei
    char  m_szSwitchName[40];       //��������

    int   m_bCheckSN;

    CFileSNMgr* m_pFileSNMgr;

    int   m_nAlarmLimit;

    class CTPFileCompare
    {
    public:
        BOOL operator ()(const SS1240FileMsg &lfileinfo, const SS1240FileMsg &rfileinfo)
        {
            //�������AMA�ļ�����ŵ��б��β��
            if (strcmp(rfileinfo.szFileId, lfileinfo.szFileId) > 0)
            {
                return TRUE;
            }

            return FALSE;
        }
    };

    //�Ƿ�����ļ�����У��
    BOOL m_bFileLenVerify;
};


#endif //__S1240_TP_COLLECT_H__


