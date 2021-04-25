#ifndef __COM_DIR_COLLECT_H__
#define __COM_DIR_COLLECT_H__

#include "../collect_base.h"

class CFileSNMgr;

class CComDirCollect
{
public:
    CComDirCollect(CLinkMgr *pLinkMgr,
                   int nSwitchIndex,
                   int nDirIndex);
    virtual ~CComDirCollect();

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

    inline const char* GetModuleLabel()
    {
        return m_strModuleLabel.c_str();
    }

    //ָ�����ļ��Ƿ�ɲ�
    virtual int  CanGetSpecFile(const char* const szSrcFile);

    //�ɼ�ָ�����ļ�
    virtual  int  GetSpecFile(const char* const szModule,
                              const char* const szSrcFile,
                              const char* const szDstFile);

    //��ȡģ����Ϣ
    virtual int  GetModuleInfo(LIST<STRING>& label,
                               LIST<STRING>& content);

    int MakeFileNameMsg(const char* const szSrcFile,
                        SGetFileReq& req);

    //��ȡ�ļ�����
    virtual int  GetFileLen(const char* const szSrcFile);

    int DoGetAFile(SGetFileReq& stReq);
protected:


    int LoadStatus();

    int SaveStatus();

    STRING GetDstFileName(const char* const szSrcFile);

    int                   m_nSwitchIndex;
    int                   m_nDirIndex;

    char m_szCollectId[40];

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
    
    int 	m_nForceColAllFile; //add by yangguang �ɼ�ͬ���������ļ�

    int    m_nMode;

    CLinkMgr*               m_pLinkMgr;


    VECTOR<SFileInfo>       m_FileList;

    char  m_szCfgPath[MAX_PATH];    //��ǰ�����ļ�������·����Ϣ

    char  m_szSwitchName[40];       //��������

    char  m_szStatusFile[MAX_PATH];
    FILE* m_pStatusFile;


    int m_bCheckSN;

    CFileSNMgr* m_pFileSNMgr;

    int  m_nFileSNLen;

    int  m_nFilePostfixLen;

    int  m_nFileSNDownLimit;
    int  m_nFileSNLimit;
    //�Ƿ�����ļ�����У��
    BOOL m_bFileLenVerify;

    //�Ƿ��ڲɼ���ɾ���������ļ�
    BOOL m_bDelRemoteFile;

    //�Ƿ�ɼ��������ϵ����һ���ļ�
    BOOL m_bCollectLastFile;

    //���������ļ���׺��
    LIST<STRING> m_FilePostfixList;

    //��׺������ģʽ
    int m_nPostfixFltMode;

    //���������ļ�ǰ׺��
    LIST<STRING> m_FilePrefixList;

    //ǰ׺������ģʽ
    int m_nPrefixFltMode;

    //ģ���ʶ��
    STRING  m_strModuleLabel;

    ACE_Mutex          m_MutexForList;

	class Compare
	{
	public:
        BOOL operator ()(const SFileInfo &lhs, const SFileInfo &rhs)
        {
            return strcmp(lhs.szFileName,rhs.szFileName) < 0;
            /*
            if (strcmp(lhs.szModTime,rhs.szModTime) < 0)
            {
                return TRUE;
            }
            else if(strcmp(lhs.szModTime,rhs.szModTime) == 0 
                    && strcmp(lhs.szFileName,rhs.szFileName) < 0 )
            {
                return TRUE;
            }

            return FALSE;*/
		}
	};//�������	

};


#endif //__COM_DIR_COLLECT_H__


