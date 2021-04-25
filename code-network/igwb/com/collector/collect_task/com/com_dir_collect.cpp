#include "../../../include/base_type.h"
#include "../global_fun.h"
#include "com_dir_collect.h"

#include "../file_sn_manager.h"

#include "../../../include/toolbox.h"
#include "../../include/adapter_include.h"
#include "../../include/res_config.h"
#include "../../collector_file_pool.h"
#include <assert.h>

extern CCollectorFilePool* GetFilePool();

CComDirCollect::CComDirCollect(CLinkMgr *pLinkMgr,
                               int nSwitchIndex,
                               int nDirIndex)
{
    assert(pLinkMgr  != NULL);
    m_pLinkMgr        = pLinkMgr;


    m_nSwitchIndex    = nSwitchIndex;
    m_szSwitchName[0] = '\0';
    m_nDirIndex       = nDirIndex;
    m_bFileLenVerify  = FALSE;
    m_pFileSNMgr      = NULL;
}

CComDirCollect::~CComDirCollect()
{
    m_pLinkMgr = NULL;

    m_FileList.clear();

    if (NULL != m_pFileSNMgr)
    {
        delete m_pFileSNMgr;
        m_pFileSNMgr = NULL;
    }

}
int CComDirCollect::LoadStatus()
{
    //modify by chenliangwei, 2003-4-28
    m_pStatusFile = fopen(m_szStatusFile, "rb+");
    if(NULL == m_pStatusFile)
    {
        //�������ļ�
        m_pStatusFile = fopen(m_szStatusFile, "wb+");

        if (NULL == m_pStatusFile)
        {
            COTRACE("�ɼ�����(%s, DIR=%d)��/����״̬�ļ�(%s)ʧ��", 
                  m_szSwitchName,
                  m_nDirIndex,
                  m_szStatusFile);
            return ERR_FAIL;
        }
        else
        {
            fclose(m_pStatusFile);
            m_pStatusFile = NULL;

            return ERR_SUCCESS;
        }
    }


    int nFileLength = FileSize(m_pStatusFile);
    if(nFileLength == 0)
    {
        fclose(m_pStatusFile);
        m_pStatusFile = NULL;
        return ERR_SUCCESS;
    }

    SFileInfo status_info;
    int nRet = 0;
    for(int i = 0; i < nFileLength / sizeof(status_info); i++)
    {
        nRet = fread(&status_info, 
                     sizeof(status_info),
                     1, 
                     m_pStatusFile);
        if(1 != nRet)
        {
            fclose(m_pStatusFile);
            m_pStatusFile = NULL;

            return ERR_FAIL;
        }
        m_FileList.push_back(status_info);
    }

    fclose(m_pStatusFile);
    m_pStatusFile = NULL;

    return ERR_SUCCESS;
}

int CComDirCollect::SaveStatus()
{
    STRING strPath;
    STRING strFile;
    divide_file_path_name(m_szStatusFile,
                          strPath,
                          strFile);
    CreateDeepDir(strPath.c_str());

    m_pStatusFile = fopen(m_szStatusFile, "wb");
    if(NULL == m_pStatusFile)
    {
        COTRACE("�ɼ�����(%s, DIR=%d)��״̬�ļ�(%s)ʧ��", 
              m_szSwitchName,
              m_nDirIndex,
              m_szStatusFile);

        return ERR_FAIL;
    }

    int nRet;
    int nFileLength = 0;
    char* pBuf = new char[sizeof(SFileInfo) * m_FileList.size()];
    VECTOR<SFileInfo>::iterator i;
    for(i = m_FileList.begin(); i != m_FileList.end(); i++)
    {
        memcpy(pBuf + nFileLength, 
               &(*i), 
               sizeof(SFileInfo));
        nFileLength += sizeof(SFileInfo);
    }
    nRet = fwrite(pBuf, 
                  nFileLength,
                  1, 
                  m_pStatusFile);
    delete pBuf;
    if(nRet != 1)
    {
        fclose(m_pStatusFile);
        m_pStatusFile = NULL;

        return ERR_FAIL;
    }

    nRet = fflush(m_pStatusFile);
    fclose(m_pStatusFile);
    m_pStatusFile = NULL;

    ACE_OS::truncate(m_szStatusFile, nFileLength);

    return nRet;
}


//��ʼ���ɼ�����
int  CComDirCollect::Init()
{
    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        return ERR_FAIL;
    }

    /*
        ���Switch����Ϣ
    */

    char szSecName[COM_CFG_LEN];
    SNPRINTF(szSecName,
             sizeof(szSecName),
             "%s%d",
             CFG_CO_SEC_SWITCH,
             m_nSwitchIndex);
    szSecName[COM_CFG_LEN - 1] = '\0';

    //�õ��ɼ�����
    INIFile.GetString(szSecName, CFG_CO_TYPE,
                      CFG_CO_TYPE_DEFAULT,
                      m_szCollectId,
                      sizeof(m_szCollectId));

    //���switch�µĴ���ģʽ
    char szTransMode[TRANS_MODE_LEN];
    INIFile.GetString(szSecName, CFG_CO_COL_TRANS_MODE,
                      CFG_CO_COL_TRANS_MODE_DEFAULT,
                      szTransMode,
                      sizeof(szTransMode));
//���ظ��ɼ�ͬ���ļ�
	m_nForceColAllFile = INIFile.GetInt(szSecName,
							CFG_CO_FORCE_COL_ALL_FILE,
							CFG_CO_FORCE_COL_ALL_FILE_DEFAULT);

	if (m_nForceColAllFile<0) 
	{
		m_nForceColAllFile = CFG_CO_FORCE_COL_ALL_FILE_DEFAULT;
	}

    //�õ�����ļ����к���Ϣ
    m_nFileSNLen = INIFile.GetInt(szSecName,
                            CFG_CO_FILE_SN_LEN,
                            CFG_CO_FILE_SN_LEN_DEFAULT);
    if(m_nFileSNLen < 0)
    {
        m_nFileSNLen = CFG_CO_FILE_SN_LEN_DEFAULT;
    }

    m_nFilePostfixLen = INIFile.GetInt(szSecName,
                            CFG_CO_FILE_POSTFIX_LEN,
                            CFG_CO_FILE_POSTFIX_LEN_DEFAULT);
    if(m_nFilePostfixLen < 0)
    {
        m_nFilePostfixLen = CFG_CO_FILE_POSTFIX_LEN_DEFAULT;
    }

    m_bCheckSN = INIFile.GetInt(szSecName,
                            CFG_CO_CHECK_SN,
                            CFG_CO_CHECK_SN_DEFAULT);


    int nCheckSNDepth = INIFile.GetInt(CFG_CO_SEC_SYSTEM,
                                 CFG_CO_CHECK_SN_DEPTH,
                                 CFG_CO_CHECK_SN_DEPTH_DEFAULT);
    nCheckSNDepth = INIFile.GetInt(szSecName,
                                 CFG_CO_CHECK_SN_DEPTH,
                                 nCheckSNDepth);
    if(nCheckSNDepth < 0)
    {
        nCheckSNDepth = CFG_CO_CHECK_SN_DEPTH_DEFAULT;
    }

    m_nFileSNLimit = INIFile.GetInt(szSecName,
                                    CFG_CO_FILE_SN_UP_LMT,
                                    CFG_CO_FILE_SN_UP_LMT_DEFAULT);
    if(m_nFileSNLimit < 0)
    {
        m_nFileSNLimit = CFG_CO_FILE_SN_UP_LMT_DEFAULT;
    }

    m_nFileSNDownLimit = INIFile.GetInt(szSecName,
                                        CFG_CO_FILE_SN_DOWN_LMT,
                                        CFG_CO_FILE_SN_DOWN_LMT_DEFAULT);
    if(m_nFileSNDownLimit < 0)
    {
        m_nFileSNDownLimit = CFG_CO_FILE_SN_DOWN_LMT_DEFAULT;
    }

    //�Ƿ�����ļ�����У��
    m_bFileLenVerify = INIFile.GetInt(CFG_CO_SEC_SYSTEM,
                                      CFG_CO_FILE_LEN_VERIFY,
                                      CFG_CO_FILE_LEN_VERIFY_DEFAULT);
    m_bFileLenVerify = INIFile.GetInt(szSecName,
                                      CFG_CO_FILE_LEN_VERIFY,
                                      m_bFileLenVerify);


    //�Ƿ��ڲɼ���ɾ���������ļ�
    m_bDelRemoteFile = INIFile.GetInt(CFG_CO_SEC_SYSTEM,
                                      CFG_CO_DEL_REMOTE_FILE,
                                      CFG_CO_DEL_REMOTE_FILE_DEFAULT);
    m_bDelRemoteFile = INIFile.GetInt(szSecName,
                                      CFG_CO_DEL_REMOTE_FILE,
                                      m_bDelRemoteFile);

    //added by chenliangwei, 2003-05-16
    //�Ƿ�ɼ��������ϵ����һ���ļ�
    m_bCollectLastFile = INIFile.GetInt(CFG_CO_SEC_SYSTEM,
                                      CFG_CO_COL_LAST_FILE,
                                      CFG_CO_COL_LAST_FILE_DEFAULT);
    m_bCollectLastFile = INIFile.GetInt(szSecName,
                                      CFG_CO_COL_LAST_FILE,
                                      m_bCollectLastFile);
    //added end

    //���������ļ���׺��
    char szFilePostfix[1024];
    INIFile.GetString(szSecName, CFG_CO_FILE_POSTFIX,
                      CFG_CO_FILE_POSTFIX_DEFAULT,
                      szFilePostfix,
                      sizeof(szFilePostfix));
    //��׺������ģʽ
    m_nPostfixFltMode = INIFile.GetInt(szSecName,
                                      CFG_CO_POSTFIX_FILTER_MODE,
                                      CFG_CO_POSTFIX_FILTER_MODE_DEFAULT);

    //���������ļ�ǰ׺��
    char szFilePrefix[1024];
    INIFile.GetString(szSecName, CFG_CO_FILE_PREFIX,
                      CFG_CO_FILE_PREFIX_DEFAULT,
                      szFilePrefix,
                      sizeof(szFilePrefix));
    //ǰ׺������ģʽ
    m_nPrefixFltMode = INIFile.GetInt(szSecName,
                                      CFG_CO_PREFIX_FILTER_MODE,
                                      CFG_CO_PREFIX_FILTER_MODE_DEFAULT);


    /*
        ���DirCollect����Ϣ
    */

    SNPRINTF(szSecName, sizeof(szSecName),
             "%s%d_%s%d", //"switch*_col_dir*"
             CFG_CO_SEC_SWITCH, m_nSwitchIndex,
             CFG_CO_SEC_COL_DIR, m_nDirIndex);
    szSecName[COM_CFG_LEN - 1] = '\0';

    //��ʶ
    char szModuleLabel[100];
    INIFile.GetString(szSecName,
                      CFG_CO_MODULE_LABEL,
                      szSecName,
                      szModuleLabel,
                      sizeof(szModuleLabel));
    m_strModuleLabel = szModuleLabel;

    //���switch*_col_dir*�µĴ���ģʽ
    INIFile.GetString(szSecName, CFG_CO_COL_TRANS_MODE,
                      szTransMode,
                      szTransMode,
                      sizeof(szTransMode));

    if (StringiCmp(szTransMode, TANS_MODE_ASC))
    {
        m_nMode = FILE_TANS_MODE_ASC;
    }
    else if (StringiCmp(szTransMode, TANS_MODE_BIN))
    {
        m_nMode = FILE_TANS_MODE_BIN;
    }
    else
    {
        COTRACE("�ɼ�����(%s)��⵽��Ч�Ĵ�������(%s)",
              m_szSwitchName,
              szTransMode);
        return ERR_FAIL;
    }
    //�ظ��ɼ�
	m_nForceColAllFile = INIFile.GetInt(szSecName,
							CFG_CO_FORCE_COL_ALL_FILE,
							m_nForceColAllFile);

	if (m_nForceColAllFile<0) 
	{
		m_nForceColAllFile = CFG_CO_FORCE_COL_ALL_FILE_DEFAULT;
	}	
	
    //��ȡ��������Ӧ�ı��ظ�Ŀ¼��
    char szPathName[MAX_PATH];
    INIFile.GetString(szSecName,
                      CFG_CO_LOCAL_DIR,
                      CCollectorAdapter::GetFrontSavePath(),
                      szPathName,
                      sizeof(szPathName));
    int nLen = strlen(szPathName) - 1;
    if(nLen > 0)
    {
        if((szPathName[nLen] == '\\')
           || (szPathName[nLen] == '/'))
        {
            szPathName[nLen] = '\0';
        }
    }
    m_strLocalRootPath = szPathName;

    //�������ػ��������Ŀ¼
    if (!CreateDeepDir(m_strLocalRootPath.c_str()))
    {
        COTRACE("�ɼ�����(%s)ԭʼ����Ŀ¼(%s)�޷������ɹ�",
            m_szSwitchName,
            m_strLocalRootPath.c_str());

        return ERR_FAIL;
    }

    //�������������ʱĿ¼
    m_strTmpPath = szPathName;
    m_strTmpPath += "/temp";
    CreateDeepDir(m_strTmpPath.c_str());

    //�õ�Զ�˷������ϵĲɼ�ԴĿ¼m_strRemotePath
    INIFile.GetString(szSecName, CFG_CO_REMOTE_DIR,
                  CFG_CO_REMOTE_DIR_DEFAULT,
                  szPathName,
                  MAX_PATH);
    nLen = strlen(szPathName) - 1;
    if(nLen > 0)
    {
        if((szPathName[nLen] == '\\')
           || (szPathName[nLen] == '/'))
        {
            szPathName[nLen] = '\0';
        }
    }
    m_strRemotePath  = szPathName;

    m_nFileSaveDays = INIFile.GetInt(szSecName,
                                 CFG_CO_ORIG_FILE_SAVE_DAYS,
                                 m_nFileSaveDays);
    if(m_nFileSaveDays < 0)
    {
        m_nFileSaveDays = CFG_CO_ORIG_FILE_SAVE_DAYS_DEFAULT;
    }

    m_bDelRemoteFile = INIFile.GetInt(szSecName,
                                      CFG_CO_DEL_REMOTE_FILE,
                                      m_bDelRemoteFile);

    //added by chenliangwei, 2003-05-16
    m_bCollectLastFile = INIFile.GetInt(szSecName,
                                      CFG_CO_COL_LAST_FILE,
                                      m_bCollectLastFile);
    //added end

    //���������ļ���׺��
    INIFile.GetString(szSecName, CFG_CO_FILE_POSTFIX,
                      szFilePostfix,
                      szFilePostfix,
                      sizeof(szFilePostfix));

    //��׺������ģʽ
    m_nPostfixFltMode = INIFile.GetInt(szSecName,
                                      CFG_CO_POSTFIX_FILTER_MODE,
                                      m_nPostfixFltMode);

    if (szFilePostfix[0] != '\0')
    {
        char *pToken = strtok(szFilePostfix, ",");
        while(pToken != NULL)
        {
            BOOL bFind = FALSE;
            LIST<STRING>::iterator iter = m_FilePostfixList.begin();
            for(; iter != m_FilePostfixList.end(); iter++)
            {
                if((*iter) == pToken)
                {
                    bFind = TRUE;
                    break;
                }
            }
            if(!bFind) m_FilePostfixList.push_back(pToken);
            pToken = strtok(NULL, ",");
        }
    }


    //���������ļ�ǰ׺��
    INIFile.GetString(szSecName, CFG_CO_FILE_PREFIX,
                      szFilePrefix,
                      szFilePrefix,
                      sizeof(szFilePrefix));

    //ǰ׺������ģʽ
    m_nPrefixFltMode = INIFile.GetInt(szSecName,
                                      CFG_CO_PREFIX_FILTER_MODE,
                                      m_nPrefixFltMode);


    if (szFilePrefix[0] != '\0')
    {
        char *pToken = strtok(szFilePrefix, ",");
        while(pToken != NULL)
        {
            BOOL bFind = FALSE;
            LIST<STRING>::iterator iter = m_FilePrefixList.begin();
            for(; iter != m_FilePrefixList.end(); iter++)
            {
                if((*iter) == pToken)
                {
                    bFind = TRUE;
                    break;
                }
            }
            if(!bFind) m_FilePrefixList.push_back(pToken);
            pToken = strtok(NULL, ",");
        }
    }

    //�õ�����ļ����к���Ϣ
    m_nFileSNLen = INIFile.GetInt(szSecName,
                            CFG_CO_FILE_SN_LEN,
                            m_nFileSNLen);
    if(m_nFileSNLen < 0)
    {
        m_nFileSNLen = CFG_CO_FILE_SN_LEN_DEFAULT;
    }

    m_nFilePostfixLen = INIFile.GetInt(szSecName,
                            CFG_CO_FILE_POSTFIX_LEN,
                            m_nFilePostfixLen);
    if(m_nFilePostfixLen < 0)
    {
        m_nFilePostfixLen = CFG_CO_FILE_POSTFIX_LEN_DEFAULT;
    }

    m_bCheckSN = INIFile.GetInt(szSecName,
                            CFG_CO_CHECK_SN,
                            m_bCheckSN);

    nCheckSNDepth = INIFile.GetInt(szSecName,
                                 CFG_CO_CHECK_SN_DEPTH,
                                 nCheckSNDepth);
    if(nCheckSNDepth < 0)
    {
        nCheckSNDepth = CFG_CO_CHECK_SN_DEPTH_DEFAULT;
    }

    m_nFileSNLimit = INIFile.GetInt(szSecName,
                                    CFG_CO_FILE_SN_UP_LMT,
                                    m_nFileSNLimit);
    if(m_nFileSNLimit < 0)
    {
        m_nFileSNLimit = CFG_CO_FILE_SN_UP_LMT_DEFAULT;
    }

    m_nFileSNDownLimit = INIFile.GetInt(szSecName,
                                        CFG_CO_FILE_SN_DOWN_LMT,
                                        m_nFileSNDownLimit);
    if(m_nFileSNDownLimit < 0)
    {
        m_nFileSNDownLimit = CFG_CO_FILE_SN_DOWN_LMT_DEFAULT;
    }

    //���״̬�ļ���·��
#ifdef _PLATFORM_WIN32
    SNPRINTF(szPathName,
             sizeof(szPathName),
             "d:/other/collect/%s/%s",
             CCollectorAdapter::GetAPName(),
             m_szSwitchName);
#else
    SNPRINTF(szPathName,
             sizeof(szPathName),
             "/var/other/collect/%s/%s",
             CCollectorAdapter::GetAPName(),
             m_szSwitchName);
#endif
    CreateDeepDir(szPathName);

/*
    ����Ҫע�����״̬�ļ��������ظ��������������⡣
*/

    //�ɼ�Ŀ¼�����״̬�ļ���
    SNPRINTF(m_szStatusFile,
             sizeof(m_szStatusFile),
             "%s/dir%d_status.dat",
             szPathName,
             m_nDirIndex);

    if(LoadStatus() != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
               "����:�ɼ�����(%s,DIR=%d)��״̬�ļ�(%s)ʧ��", 
               m_szSwitchName,
               m_nDirIndex,
               m_szStatusFile);
    }

    char szFileSNStatusFile[MAX_PATH];

    //�ļ����кŵ�״̬�ļ�
    SNPRINTF(szFileSNStatusFile,
             sizeof(szFileSNStatusFile),
             "%s/dir%d_file_sn_status.dat",
             szPathName,
             m_nDirIndex);


    char szMgrInfo[BUF_SIZE];
    SNPRINTF(szMgrInfo,
        sizeof(szMgrInfo),
        "�ɼ�����(%s,DIR=%d)�ļ����кŹ�����",
        m_szSwitchName,
        m_nDirIndex);

    szMgrInfo[BUF_SIZE - 1] = '\0';

    m_pFileSNMgr = new CFileSNMgr(szFileSNStatusFile,
                                nCheckSNDepth,
                                m_nFileSNLen,
                                m_nFilePostfixLen + 1);

    if (m_nFileSNLimit > 0)
    {
        m_pFileSNMgr->SetSNLimit(m_nFileSNLimit, m_nFileSNDownLimit);
    }

    m_pFileSNMgr->SetMgrInfo(szMgrInfo);

    if (ERR_SUCCESS != m_pFileSNMgr->Init())
    {
        delete m_pFileSNMgr;
        m_pFileSNMgr = NULL;
    }


    return ERR_SUCCESS;
}

//�����ļ�����˵ĵ����ļ�������, �����������ʵ��;
int CComDirCollect::GetFile(SColFileReq& stReq)
{
    SGetFileReq file_req;
    int nRet = MakeFileNameMsg(stReq.szSrcName, file_req);
    if(nRet == ERR_SUCCESS)
    {
        file_req.nFileSize = stReq.nFileSize;

        if((stReq.szDstName != NULL)
           &&(stReq.szDstName[0] != 0))
        {
            STRING strPath;
            STRING strFile;
            divide_file_path_name(stReq.szDstName,
                                  strPath,
                                  strFile);
            if(strPath == "")
            {
                strPath = m_strLocalPath;
            }
#ifdef _PLATFORM_WIN32
            else if((strPath[1] != ':')
                    && (strPath[0] != '/')
                    && (strPath[0] != '\\'))
#else
            else if(strPath[0] != '/')
#endif
            {
                strPath = m_strLocalPath + "/" + strPath;
            }

            strncpy(file_req.szDstPath,
                    strPath.c_str(),
                    sizeof(file_req.szDstPath));
            strncpy(file_req.szDstName,
                    strFile.c_str(),
                    sizeof(file_req.szDstName));
            CreateDeepDir(file_req.szDstPath);
        }

        SNPRINTF(stReq.szDstName,
                 sizeof(stReq.szDstName),
                 "%s/%s",
                 file_req.szDstPath,
                 file_req.szDstName);

        nRet = DoGetAFile(file_req);
        stReq.nFileSize = file_req.nFileSize;
    }

    return nRet;
}

//�����ļ�����˵ĵ����ļ�������;
int CComDirCollect::DoGetAFile(SGetFileReq& stReq)
{

    if(stReq.szSrcName[0] == '\0')
    {
        COTRACE("�ɼ��ļ�������Ϊ�գ������������.");
        //COTRACE("Remote file name is not specified: failed to get file");
        return ERR_FAIL;
    }

    int nFileListIndex = -1;
    int i = 0;
    for(i = 0; i < m_FileList.size(); i++)
    {
        SFileInfo& file_info = m_FileList[i];
        if(strcmp(file_info.szFileName, stReq.szSrcName) == 0)
        {
            nFileListIndex = i;
            break;
        }
    }

    if(nFileListIndex == -1)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�ɼ�����(%s)�ɼ��ļ�(%s)ʧ��:�ļ�������",
               m_szSwitchName,
               stReq.szSrcName);
        return ERR_FAIL;
    }

    int nRet = ERR_SUCCESS;

    //���ô���ģʽ
    char szTraceMsg[1024];

    nRet = m_pLinkMgr->SetMode(m_nMode);
    if (nRet != ERR_SUCCESS)
    {
        SNPRINTF(szTraceMsg,
                 sizeof(szTraceMsg),
                 //"Collector(ID=%d) failed to collect file:%s to %s",
                 "�ɼ�����(%s,DIRID=%d)���ô���ģʽʧ��",
                 m_szSwitchName,
                 m_nDirIndex);
        szTraceMsg[sizeof(szTraceMsg) - 1] = '\0';
        COTRACE(szTraceMsg);
        COMSGOUT(MSGOUT_LEVEL_URGENT, szTraceMsg);

        return nRet;
    }

    //������ʱ�ļ���
    string strSrcFile = m_strTmpPath;
    strSrcFile += "/";
    strSrcFile += stReq.szDstName;

    //������ʽ�ļ���
    string  strDstFile = stReq.szDstPath;
    if((stReq.szDstPath[0] != 0) &&
       ((stReq.szDstPath[strlen(stReq.szDstPath) - 1] != '\\') &&
        (stReq.szDstPath[strlen(stReq.szDstPath) - 1] != '/')))
    {
        strDstFile += "/";
    }
    strDstFile += stReq.szDstName;

    //���ԭ·����
    STRING strPath;
    STRING strFile;
    divide_file_path_name(stReq.szSrcName,
                          strPath,
                          strFile);

    strncpy(stReq.szSrcName, strFile.c_str(), sizeof(stReq.szSrcName));
    stReq.szSrcName[sizeof(stReq.szSrcName) - 1] = '\0';
    strncpy(stReq.szSrcPath, strPath.c_str(), sizeof(stReq.szSrcPath));
    stReq.szSrcPath[sizeof(stReq.szSrcPath) - 1] = '\0';
    strncpy(stReq.szDstPath, m_strTmpPath.c_str(), sizeof(stReq.szDstPath));
    stReq.szDstPath[sizeof(stReq.szDstPath) - 1] = '\0';

    m_MutexForList.acquire();
    m_FileList[nFileListIndex].nCollectStatus = CS_COLLECTING;
    if(SaveStatus() != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "����:�ɼ�����(%s,DIR=%d)����״̬�ļ�(%s)ʧ��", 
               m_szSwitchName,
               m_nDirIndex,
               m_szStatusFile);

        COTRACE("����:�ɼ�����(%s,DIR=%d)����״̬�ļ�(%s)ʧ��", 
               m_szSwitchName,
               m_nDirIndex,
               m_szStatusFile);

    }
    m_MutexForList.release();

    //ȡ�ļ�
    nRet = m_pLinkMgr->GetFile(stReq);//���Ż�Ϊ����ȫ·������

    //��¼������־
    if( nRet == ERR_SUCCESS )
    {
        //�����ļ�����
        nRet = ERR_FAIL;
        FILE* f = fopen(strSrcFile.c_str(), "rb");
        if(f != NULL)
        {
            int nLength = FileSize(f);
            fclose(f);

            if(m_bFileLenVerify && stReq.nFileSize != nLength)
            {
                SNPRINTF(szTraceMsg,
                         sizeof(szTraceMsg),
                         "�ɼ�����(%s)�ɼ����ļ�(%s)���Ȳ�һ��(Զ��:%d, ����:%d)",
                         m_szSwitchName,
                         stReq.szSrcName,
                         stReq.nFileSize,
                         nLength);
                szTraceMsg[sizeof(szTraceMsg) - 1] = '\0';
                COTRACE(szTraceMsg);
                COMSGOUT(MSGOUT_LEVEL_URGENT, szTraceMsg);
            }
            else
            {
                nRet = ERR_SUCCESS;
                stReq.nFileSize = nLength;
            }
        }

        if(nRet == ERR_SUCCESS)
        {
            //�ƶ���ʱ�ļ�Ϊ��ʽ�ļ�
            BOOL bRet = FileCopy(strSrcFile.c_str(), strDstFile.c_str() );
            if(!bRet)
            {
                SNPRINTF(szTraceMsg,
                         sizeof(szTraceMsg),
                         //"Collector(ID=%d) failed to move file:%s to %s",
                         "�ɼ�����(%s)�ƶ��ļ�ʧ��:%s to %s",
                         m_szSwitchName,
                         strSrcFile.c_str(),
                         strDstFile.c_str());
                szTraceMsg[sizeof(szTraceMsg) - 1] = '\0';
                COTRACE(szTraceMsg);
                COMSGOUT(MSGOUT_LEVEL_URGENT, szTraceMsg);
                nRet = ERR_FAIL;
            }
            else 
            {
                FileDel(strSrcFile.c_str());
                if (m_bDelRemoteFile)
                {
                    char szFileName[MAX_PATH] = "";
                    if(stReq.szSrcPath[0] != '\0')
                    {
                        SNPRINTF(szFileName, sizeof(szFileName), "%s/", stReq.szSrcPath);
                    }
                    strncat(szFileName, stReq.szSrcName, sizeof(szFileName));
                    szFileName[sizeof(szFileName) - 1] = '\0';

                    nRet = m_pLinkMgr->DelFile(szFileName);

                    if(nRet != ERR_SUCCESS)
                    {
                        SNPRINTF(szTraceMsg,
                                 sizeof(szTraceMsg),
                                 //"Collector(ID=%d) failed to collect file:%s to %s",
                                 "�ɼ�����(%s)ɾ���ļ�ʧ��:%s",
                                 m_szSwitchName,
                                 szFileName);
                        szTraceMsg[sizeof(szTraceMsg) - 1] = '\0';
                        COTRACE(szTraceMsg);
                        COMSGOUT(MSGOUT_LEVEL_URGENT, szTraceMsg);
                    }
                }
            }
        }
    }
    else
    {
        SNPRINTF(szTraceMsg,
                 sizeof(szTraceMsg),
                 //"Collector(ID=%d) failed to collect file:%s to %s",
                 "�ɼ�����(%s)�ɼ��ļ�ʧ��:%s to %s",
                 m_szSwitchName,
                 stReq.szSrcName,
                 strSrcFile.c_str());
        szTraceMsg[sizeof(szTraceMsg) - 1] = '\0';
        COTRACE(szTraceMsg);
        COMSGOUT(MSGOUT_LEVEL_URGENT, szTraceMsg);
    }

    if(nRet != ERR_SUCCESS)
    {
        remove(strSrcFile.c_str());
        remove(strDstFile.c_str());

        m_MutexForList.acquire();
        m_FileList[nFileListIndex].nCollectStatus = CS_NOT_COLLECTED;
        if(SaveStatus() != ERR_SUCCESS)
        {
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "����:�ɼ�����(%s,DIR=%d)����״̬�ļ�(%s)ʧ��", 
                   m_szSwitchName,
                   m_nDirIndex,
                   m_szStatusFile);

            COTRACE("����:�ɼ�����(%s,DIR=%d)����״̬�ļ�(%s)ʧ��", 
                   m_szSwitchName,
                   m_nDirIndex,
                   m_szStatusFile);

        }

        m_MutexForList.release();
    }
    else
    {
        m_MutexForList.acquire();
        m_FileList[nFileListIndex].nCollectStatus = CS_COLLECTED;
        if(SaveStatus() != ERR_SUCCESS)
        {
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "����:�ɼ�����(%s,DIR=%d)����״̬�ļ�(%s)ʧ��", 
                   m_szSwitchName,
                   m_nDirIndex,
                   m_szStatusFile);

            COTRACE("����:�ɼ�����(%s,DIR=%d)����״̬�ļ�(%s)ʧ��", 
                   m_szSwitchName,
                   m_nDirIndex,
                   m_szStatusFile);

        }

        m_MutexForList.release();


        //���ļ���Ϣ�ӵ�ת���ļ�����
        SSwitchFileInfo stFileInfo;
        stFileInfo.nSwitchIndex = m_nSwitchIndex;
        strncpy(stFileInfo.szModuleLabel,
                m_strModuleLabel.c_str(),
                sizeof(stFileInfo.szModuleLabel));
        SNPRINTF(stFileInfo.szRemoteFile,
               sizeof(stFileInfo.szRemoteFile),
               "%s/%s",
                stReq.szSrcPath,
                stReq.szSrcName);
        strncpy(stFileInfo.szLocalFile,
                strDstFile.c_str(),
                sizeof(stFileInfo.szLocalFile));
        stFileInfo.nConvertInfo = m_nDirIndex;

        GetFilePool()->PushBack(stFileInfo);
        //added end
    }

    return  nRet;

}


//���ļ��б��в���ָ���ļ�
int CComDirCollect::FindFileInList(const char* const szFileName)
{
    VECTOR<SFileInfo>::iterator i;
    for(i = m_FileList.begin(); i != m_FileList.end(); i++)
    {
        SFileInfo& file_msg = *i;
        if(strcmp(file_msg.szFileName, szFileName) == 0)
        {
            return ERR_SUCCESS;
        }
    }

    return ERR_FAIL;
}

//����ļ��б�
int CComDirCollect::GetList(SListOut& pOut)
{
    SListOut file_list_new;
    int nRet = m_pLinkMgr->GetFileList(m_strRemotePath.c_str(), file_list_new);

    if(nRet != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�ɼ�����(%s,DIRID=%d)��ȡ�ļ��б�ʧ��(������=%d)",
               m_szSwitchName,
               m_nDirIndex,
               nRet);
        COTRACE("�ɼ�����(%s,DIRID=%d)��ȡ�ļ��б�ʧ��(������=%d)",
               m_szSwitchName,
               m_nDirIndex,
               nRet);
        return nRet;
    }

    //���ļ�����С��������
    std::sort(file_list_new.FileList.begin(),
              file_list_new.FileList.end(),
              Compare());

    VECTOR<SFileInfo>::iterator i;
    
    //��׺����
    if (m_FilePostfixList.size() > 0)
    {
        i = file_list_new.FileList.begin();

        while(i != file_list_new.FileList.end())
        {
            BOOL bFind = FALSE;
            LIST<STRING>::iterator j;
            for(j = m_FilePostfixList.begin(); j != m_FilePostfixList.end(); j++)
            {
                char* pszPost = (*i).szFileName + strlen((*i).szFileName) 
                                - (*j).length();

                if (strcmp(pszPost, (*j).c_str()) == 0)
                {
                    bFind = TRUE;
                    break;
                }
            }

            //ģʽ��Ϊ0, ���˵������Ϻ�׺�����ļ�
            if (m_nPostfixFltMode != 0)
            {
                if (!bFind) 
                {
                    i = file_list_new.FileList.erase(i);
                    continue;
                }
            }
            else   //ģʽΪ0, ���˵����Ϻ�׺�����ļ�
            {
                if (bFind)
                {
                    i = file_list_new.FileList.erase(i);
                    continue;
                }
            }
            i++;

        }
    }

    //ǰ׺����
    if (m_FilePrefixList.size() > 0)
    {
        STRING strPath;
        STRING strFile;

        i = file_list_new.FileList.begin();

        while(i != file_list_new.FileList.end())
        {
            //����ļ���
            divide_file_path_name((*i).szFileName,
                                  strPath,
                                  strFile);

            BOOL bFind = FALSE;
            LIST<STRING>::iterator j;
            for(j = m_FilePrefixList.begin(); j != m_FilePrefixList.end(); j++)
            {
                if (strncmp(strFile.c_str(), (*j).c_str(), (*j).length()) == 0)
                {
                    bFind = TRUE;
                    break;
                }
            }

            //ģʽ��Ϊ0, ���˵�������ǰ׺�����ļ�
            if (m_nPrefixFltMode != 0)
            {
                if (!bFind) 
                {
                    i = file_list_new.FileList.erase(i);
                    continue;
                }
            }
            else   //ģʽΪ0, ���˵�����ǰ׺�����ļ�
            {
                if (bFind)
                {
                    i = file_list_new.FileList.erase(i);
                    continue;
                }
            }
            i++;

        }
    }


    if (file_list_new.FileList.size() == 0)
    {
        return ERR_SUCCESS;
    }

    //��ȡ���Ѿ��ɼ����ļ�
    VECTOR<SFileInfo> file_list;
    for(i = m_FileList.begin(); i != m_FileList.end(); i++)
    {
        SFileInfo& file_info = *i;

        if(CS_COLLECTED == file_info.nCollectStatus)
        {
            VECTOR<SFileInfo>::iterator j;
            j = file_list_new.FileList.begin();
            while(j != file_list_new.FileList.end())
            {
                if (strcmp((*j).szFileName, file_info.szFileName) == 0)
                {
                    file_list.push_back(file_info);
                    break;
                }
                j++;
            }
        }
    }

    //�����ļ��б��Ҫ�ɼ����ļ��б�
    m_MutexForList.acquire();

    m_FileList.clear();

    VECTOR<SFileInfo>::iterator j;
        for(i = file_list_new.FileList.begin();
        i != file_list_new.FileList.end();
        i++)
    {
        BOOL bFound = FALSE;
        SFileInfo& file_info_i = *i;
        file_info_i.nCollectStatus = CS_NOT_COLLECTED;
        for(j = file_list.begin(); j != file_list.end(); j++)
        {
            SFileInfo& file_info_j = *j;
            if(strcmp(file_info_i.szFileName, file_info_j.szFileName) == 0)
            {
                bFound = TRUE;
                break;
            }
        }

        if((!bFound)||m_nForceColAllFile)
        {
            m_FileList.push_back(*i);
            pOut.FileList.push_back(*i);
        }
        else
        {
            m_FileList.push_back(*j);
        }
    }

    //modify by chenliangwei, 2003-05-16, ֻ�е��ļ�����0ʱ��ȥ��
    if ((!m_bCollectLastFile) && (pOut.FileList.size() > 0))
    {
        pOut.FileList.pop_back();
    }

    if(SaveStatus() != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "����:�ɼ�����(%s,DIR=%d)����״̬�ļ�(%s)ʧ��", 
               m_szSwitchName,
               m_nDirIndex,
               m_szStatusFile);

        COTRACE("����:�ɼ�����(%s,DIR=%d)����״̬�ļ�(%s)ʧ��", 
               m_szSwitchName,
               m_nDirIndex,
               m_szStatusFile);

    }

    m_MutexForList.release();

    return ERR_SUCCESS;
}

//��̬������Ŀ¼
BOOL CComDirCollect::CreateLocSubDir()
{
    char szTmp[24];
    time_t tNow = time(NULL);
    tm tmNow = *localtime(&tNow);
    strftime(szTmp,24,"%Y%m%d",&tmNow);
    m_strLocalPath = m_strLocalRootPath + "/" + szTmp;
    CreateDeepDir(m_strLocalPath.c_str());

    return TRUE;
}

//��������ļ�
BOOL CComDirCollect::ClearOldFile()
{
    //added by chenliangwei
    int nRet = CCollectBase::ClearOldDateSubDir(m_strLocalRootPath.c_str(),
                                  m_nFileSaveDays);

    return nRet == ERR_SUCCESS;
    //added end
}

//����ļ����к��Ƿ�����
BOOL CComDirCollect::CheckFileSN(const char* szFileName, char* szPreviousFile)
{
    if ((m_pFileSNMgr ==NULL)||(m_bCheckSN == 0))
    {
        return TRUE;
    }

    return m_pFileSNMgr->CheckFileSN(szFileName, szPreviousFile);

}

/***********************************************
�����ж�ָ�����ļ��Ƿ���ֹ��ɼ�
�����ļ������ֹ��ɼ�
//1.Ŀ¼�б�֮����ļ�������ɼ�
//2.Ŀ¼�б�֮�ڵ��ļ������Ѿ����ɼ���ɾ�����ļ�
************************************************/
int CComDirCollect::CanGetSpecFile(const char* const szSrcFile)
{
    m_MutexForList.acquire();

    int nRet = ERR_FILE_CANNOT_BE_FOUND;
    for(int i = 0; i < m_FileList.size(); i++)
    {
        SFileInfo& fi = m_FileList[i];

        if(strcmp(fi.szFileName, szSrcFile) == 0)
        {
            nRet = ERR_SUCCESS;

            //�����Ҫɾ��Զ���ļ������ļ��ѱ��ɼ����ܲɼ�
            if ((m_bDelRemoteFile) && (CS_COLLECTED == fi.nCollectStatus))
            {
                m_MutexForList.release();
                //��ʾ�����ѱ�ɾ���������ٲ�
                return ERR_FILE_BE_DELETED;
            }
            else if(CS_COLLECTING == fi.nCollectStatus)
            {
                m_MutexForList.release();
                //�����ļ����ڱ��ɼ��������ٲ�
                return ERR_FILE_BEING_COLLECTED;
            }
            else if(!m_bCollectLastFile)
            {
                BOOL bIsNew = TRUE;
                for(int j = 0; j < m_FileList.size(); j++)
                {
                    //���ʱ�䲻�����£�������ѭ��
                    if(strcmp(m_FileList[j].szFileName, fi.szFileName) > 0)
                    {
                        bIsNew = FALSE;
                        break;
                    }
                }

                if(bIsNew)
                {
                    m_MutexForList.release();
                    //��ʾϵͳ������ɼ����µĿ���δ�رյĻ����ļ�
                    return ERR_NEW_FILE_CANNOT_BE_CO;
                }
            }
        }
    }

    m_MutexForList.release();

    return nRet;
}

int CComDirCollect::GetSpecFile(const char* const szModule,
                                const char* const szSrcFile,
                                const char* const szDstFile)
{
    int nFileListIndex = -1;
    for(int i = 0; i < m_FileList.size(); i++)
    {
        SFileInfo& fi = m_FileList[i];
        if(strcmp(szSrcFile, fi.szFileName) == 0)
        {
            nFileListIndex = i;
            break;
        }
    }

    if(nFileListIndex == -1)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�ɼ�����(%s)�ֶ��ɼ��ļ�(%s)ʧ��:�ļ�������",
               m_szSwitchName,
               szSrcFile);
        return ERR_FAIL;
    }

    SGetFileReq file_req;
    int nRet = MakeFileNameMsg(szSrcFile, file_req);
    if(nRet == ERR_SUCCESS)
    {
        file_req.nFileSize = m_FileList[nFileListIndex].nFileSize;
        if((szDstFile != NULL) && (szDstFile[0] != 0))
        {
            STRING strPath;
            STRING strFile;
            divide_file_path_name(szDstFile, strPath, strFile);
            strncpy(file_req.szDstPath,
                    strPath.c_str(),
                    sizeof(file_req.szDstPath));
            strncpy(file_req.szDstName,
                    strPath.c_str(),
                    sizeof(file_req.szDstName));
        }
        nRet = DoGetAFile(file_req);
    }

    return nRet;
}

//��ȡģ����Ϣ
int CComDirCollect::GetModuleInfo(LIST<STRING>& label,
                                  LIST<STRING>& content)
{
    label.push_back("�ļ���");
    label.push_back("�ļ�����");
    label.push_back("�ɼ�״̬");

    m_MutexForList.acquire();

    for(int i = 0; i < m_FileList.size(); i++)
    {
        SFileInfo& fi = m_FileList[i];

        content.push_back(fi.szFileName);

        char szBuf[20];
        SNPRINTF(szBuf, sizeof(szBuf), "%d", fi.nFileSize);
        content.push_back(szBuf);
        switch (fi.nCollectStatus)
        {
        case CS_COLLECTING:
            content.push_back(S_CS_COLLECTING);
            break;

        case CS_COLLECTED:
            content.push_back(S_CS_COLLECTED);
            break;

        case CS_NOT_COLLECTED:
        default:
            content.push_back(S_CS_NOT_COLLECTED);
            break;
        }
    }

    m_MutexForList.release();

    if(content.size() == 0)
    {
        content.push_back("-");
        content.push_back("-");
        content.push_back("-");
    }

    return ERR_SUCCESS;
}

//��ȡ�ļ�����
int CComDirCollect::GetFileLen(const char* const szSrcFile)
{
    m_MutexForList.acquire();

    int nRet = 0;
    VECTOR<SFileInfo>::iterator i;
    for(i = m_FileList.begin(); i != m_FileList.end(); i++)
    {
        SFileInfo& fi = *i;
        if(strcmp(fi.szFileName, szSrcFile) == 0)
        {
            nRet = fi.nFileSize;
            break;
        }
    }

    m_MutexForList.release();

    return nRet;
}

int CComDirCollect::MakeFileNameMsg(const char* const szSrcFile,
                                  SGetFileReq& req)
{
    if(szSrcFile[0] == '\0')
    {
        //COTRACE("�ɼ��ļ�������Ϊ�գ������������.");
        COTRACE("Remote file name is not specified: failed to get file");
        return ERR_FAIL;
    }

    SGetFileReq stGetReq;
    memset(&stGetReq,0, sizeof(SGetFileReq));

    //5ESS�ϵ�Դ�ļ�·��
    strncpy(stGetReq.szSrcPath,
            m_strRemotePath.c_str(),
            sizeof(stGetReq.szSrcPath));

    //Դ�ļ���
    strncpy(stGetReq.szSrcName, szSrcFile, sizeof(stGetReq.szSrcName));

    //��ǰĿ�Ļ����ļ�·��
    strcpy(stGetReq.szDstPath, m_strLocalPath.c_str());

    //szBuf����µ�Ŀ���ļ���
    strncpy(stGetReq.szDstName,
            GetDstFileName(stGetReq.szSrcName).c_str(),
            sizeof(stGetReq.szDstName));

    req = stGetReq;

    return ERR_SUCCESS;
}

STRING CComDirCollect::GetDstFileName(const char* const szSrcFile)
{
    STRING strPath;
    STRING strFile;
    divide_file_path_name(szSrcFile,
                          strPath,
                          strFile);

    return strFile;
}
