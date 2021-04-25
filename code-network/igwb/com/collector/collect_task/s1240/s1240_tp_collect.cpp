#include "../../../include/base_type.h"
#include "s1240_cmise_mgr.h"
#include "../link_mgr.h"
#include "s1240_tp_collect.h"
#include "../global_fun.h"

#include "../file_sn_manager.h"

#include "../../../include/toolbox.h"
#include "../../include/adapter_include.h"
#include "../../include/res_config.h"
#include "../../collector_file_pool.h"
#include <assert.h>
#include <algorithm>

extern CCollectorFilePool* GetFilePool();

CS1240TPCollect::CS1240TPCollect(CLinkMgr *pPriLinkMgr,
                                 CLinkMgr *pBakLinkMgr,
                                 CS1240CmiseMgr* pCmiseMgr,
                                 int nSwitchIndex,
                                 int nTPIndex)
{
    assert(pPriLinkMgr != NULL);
    m_pPriLinkMgr  = pPriLinkMgr;

    assert(pBakLinkMgr != NULL);
    m_pBakLinkMgr  = pBakLinkMgr;

    assert(pCmiseMgr != NULL);
    m_pCmiseMgr    = pCmiseMgr;

    m_nSwitchIndex = nSwitchIndex;
    m_nTPIndex     = nTPIndex;

    m_szStatusFile[0] = '\0';
    m_szRemotePath[0] = '\0';
    m_szLocalPath[0]  = '\0';
    m_szCfgPath[0]    = '\0';
    m_pStatusFile     = NULL;
    m_szSwitchName[0] = '\0';
    m_nAlarmLimit     = -1;
    m_nCollectingFile = 0;
    m_bFileLenVerify  = FALSE;

    m_nCoFailAlarmLimit = CFG_CO_FAIL_ALARM_LIMIT_DEFAULT;

    m_pFileSNMgr = NULL;
}

CS1240TPCollect::~CS1240TPCollect()
{
    m_pPriLinkMgr = NULL;
    m_pBakLinkMgr = NULL;
    if (NULL != m_pStatusFile)
    {
        fclose(m_pStatusFile);
        m_pStatusFile = NULL;
    }

    if (NULL != m_pFileSNMgr)
    {
        delete m_pFileSNMgr;
        m_pFileSNMgr = NULL;
    }

}

int CS1240TPCollect::LoadStatus()
{
    m_pStatusFile = fopen(m_szStatusFile, "rb+");
    if(NULL == m_pStatusFile)
    {
        COTRACE("�ɼ�����(%s, TP=%s)��/����״̬�ļ�(%s)ʧ��",
              m_szSwitchName,
              m_TPInfo.szTPID,
              m_szStatusFile);
        return ERR_FAIL;
    }

    int nFileLength = FileSize(m_pStatusFile);
    if(nFileLength == 0)
    {
        fclose(m_pStatusFile);
        m_pStatusFile = NULL;
        return ERR_SUCCESS;
    }

    SS1240FileStatus status_info;
    int nRet = 0;
    m_nCollectingFile = 0;
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
        m_FileList[status_info.file_msg.szFileId] = status_info;
        if(CS_COLLECTING == status_info.nStatus)
        {
            m_nCollectingFile++;
        }
    }

    fclose(m_pStatusFile);
    m_pStatusFile = NULL;

    return ERR_SUCCESS;
}

int CS1240TPCollect::SaveStatus()
{
    if(m_FileList.size() == 0)
    {
        return ERR_SUCCESS;
    }

    m_pStatusFile = fopen(m_szStatusFile, "wb");
    if(NULL == m_pStatusFile)
    {
        return ERR_FAIL;
    }

    int nRet;
    int nFileLength = 0;
    char* pBuf = new char[sizeof(SS1240FileStatus) * m_FileList.size()];
    MAP<STRING, SS1240FileStatus>::iterator i;
    for(i = m_FileList.begin(); i != m_FileList.end(); i++)
    {
        memcpy(pBuf + nFileLength,
               &i->second,
               sizeof(SS1240FileStatus));
        nFileLength += sizeof(SS1240FileStatus);
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
int  CS1240TPCollect::Init()
{
    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        return ERR_FAIL;
    }

    /*
        ����ؽ������ֶλ�ȡ��Ϣ
    */
    char szSecName[S1240_CFG_LEN];
    SNPRINTF(szSecName,
             sizeof(szSecName),
             "%s%d", //"switch*"
             CFG_CO_SEC_SWITCH,
             m_nSwitchIndex);
    szSecName[S1240_CFG_LEN - 1] = '\0';

    //��ȡ��������Nename
    char szNename[S1240_CFG_LEN];
    INIFile.GetString(szSecName,
                      CFG_CO_TP_NENAME,
                      CFG_CO_TP_NENAME_DEFAULT,
                      szNename,
                      sizeof(szNename));

    //ȡ�����õ�Զ��Ŀ¼
    INIFile.GetString(szSecName,
                      CFG_CO_REMOTE_DIR,
                      CFG_CO_REMOTE_DIR_DEFAULT,
                      m_szRemotePath,
                      sizeof(m_szRemotePath));

    //��ȡԴ�ļ�״̬�Ƿ���DUMPED
    m_TPInfo.bDumped = INIFile.GetInt(szSecName,
                                      CFG_CO_DUMPED,
                                      CFG_CO_DUMPED_DEFAULT);

    //��ȡ����ɼ�ʧ�ܵ�����ļ���
    m_nCoFailAlarmLimit = INIFile.GetInt(szSecName,
                                         CFG_CO_FAIL_ALARM_LIMIT,
                                         CFG_CO_FAIL_ALARM_LIMIT_DEFAULT);
    if(m_nCoFailAlarmLimit < 0)
    {
        m_nCoFailAlarmLimit = CFG_CO_FAIL_ALARM_LIMIT_DEFAULT;
    }

    //��ȡĿ��״̬
    char szFinalStatus[S1240_CFG_LEN];
    INIFile.GetString(szSecName,
                      CFG_CO_FINAL_STATUS,
                      CFG_CO_FINAL_STATUS_DEFAULT,
                      szFinalStatus,
                      sizeof(szFinalStatus));

    //��ȡ��������Ӧ�ı��ظ�Ŀ¼��
    char szLocalPath[MAX_PATH];
    INIFile.GetString(szSecName,
                      CFG_CO_LOCAL_DIR,
                      CCollectorAdapter::GetFrontSavePath(),
                      szLocalPath,
                      sizeof(szLocalPath));
    int nLen = strlen(szLocalPath) - 1;
    if(nLen > 0)
    {
        if((szLocalPath[nLen] == '\\')
           || (szLocalPath[nLen] == '/'))
        {
            szLocalPath[nLen] = '\0';
        }
    }

    //�õ�����ļ����к���Ϣ
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

    //�Ƿ�����ļ�����У��
    m_bFileLenVerify = INIFile.GetInt(CFG_CO_SEC_SYSTEM,
                                  CFG_CO_FILE_LEN_VERIFY,
                                  CFG_CO_FILE_LEN_VERIFY_DEFAULT);
    m_bFileLenVerify = INIFile.GetInt(szSecName,
                                  CFG_CO_FILE_LEN_VERIFY,
                                  m_bFileLenVerify);

    /*
        �����TP�ֶλ�ȡ��Ϣ
    */
    SNPRINTF(szSecName, sizeof(szSecName),
             "%s%d_%s%d", //"switch*_tp*"
             CFG_CO_SEC_SWITCH, m_nSwitchIndex,
             CFG_CO_SEC_TP, m_nTPIndex);
    szSecName[S1240_CFG_LEN - 1] = '\0';

    //��ȡ������TP��Nename
    INIFile.GetString(szSecName,
                      CFG_CO_TP_NENAME,
                      szNename,
                      m_TPInfo.szNename,
                      sizeof(m_TPInfo.szNename));
    if(strlen(szNename) == 0)
    {
        COTRACE("�ɼ�����(%s,TP=%d)��NENAMEδ����",
              m_szSwitchName,
              m_nTPIndex);
        return ERR_FAIL;
    }

    //ȡ�����õ�Զ��Ŀ¼
    INIFile.GetString(szSecName,
                      CFG_CO_REMOTE_DIR,
                      m_szRemotePath,
                      m_szRemotePath,
                      sizeof(m_szRemotePath));

    //��ȡ������TP��ID
    INIFile.GetString(szSecName,
                      CFG_CO_TP_ID,
                      CFG_CO_TP_ID_DEFAULT,
                      m_TPInfo.szTPID,
                      sizeof(m_TPInfo.szTPID));
    if(strlen(m_TPInfo.szTPID) == 0)
    {
        COTRACE("�ɼ�����(%s,TP=%d)��TPIDδ����",
              m_szSwitchName,
              m_nTPIndex);
        return ERR_FAIL;
    }

    /*
    strncat(szLocalPath, "/", sizeof(szLocalPath));
    szLocalPath[sizeof(szLocalPath) - 1] = '\0';

    strncat(szLocalPath, m_TPInfo.szTPID, sizeof(szLocalPath));
    szLocalPath[sizeof(szLocalPath) - 1] = '\0';
    */

    //��ȡ������TP��Ӧ��������
    INIFile.GetString(szSecName,
                      CFG_CO_BILL_TYPE,
                      CFG_CO_BILL_TYPE_DEFAULT,
                      m_TPInfo.szBilltype,
                      sizeof(m_TPInfo.szBilltype));

    //��ȡ����ɼ�ʧ�ܵ�����ļ���
    m_nCoFailAlarmLimit = INIFile.GetInt(szSecName,
                                         CFG_CO_FAIL_ALARM_LIMIT,
                                         m_nCoFailAlarmLimit);
    if(m_nCoFailAlarmLimit < 0)
    {
        m_nCoFailAlarmLimit = CFG_CO_FAIL_ALARM_LIMIT_DEFAULT;
    }

    m_nFileSaveDays = INIFile.GetInt(szSecName,
                                 CFG_CO_ORIG_FILE_SAVE_DAYS,
                                 m_nFileSaveDays);
    if(m_nFileSaveDays < 0)
    {
        m_nFileSaveDays = CFG_CO_ORIG_FILE_SAVE_DAYS_DEFAULT;
    }

    //��ȡԴ�ļ�״̬�Ƿ���DUMPED
    m_TPInfo.bDumped = INIFile.GetInt(szSecName,
                                      CFG_CO_DUMPED,
                                      m_TPInfo.bDumped);

    //��ȡĿ��״̬
    INIFile.GetString(szSecName,
                      CFG_CO_FINAL_STATUS,
                      szFinalStatus,
                      szFinalStatus,
                      sizeof(szFinalStatus));
    if(StringiCmp(szFinalStatus, S1240_DS_DUMPED))
    {
        m_TPInfo.nFinalStatus = CMISE_STATUS_READYTOBETRANSFERRED;
    }
    else
    {
        m_TPInfo.nFinalStatus = CMISE_STATUS_FREE;
    }

    //��ȡ������TP��Ӧ�ı��ظ�Ŀ¼��
    INIFile.GetString(szSecName,
                      CFG_CO_LOCAL_DIR,
                      szLocalPath,
                      m_TPInfo.szLocalPath,
                      sizeof(m_TPInfo.szLocalPath));
    nLen = strlen(m_TPInfo.szLocalPath) - 1;
    if(nLen > 0)
    {
        if((m_TPInfo.szLocalPath[nLen] == '\\')
           || (m_TPInfo.szLocalPath[nLen] == '/'))
        {
            m_TPInfo.szLocalPath[nLen] = '\0';
        }
    }

    //�������������ʱĿ¼
    m_strTmpPath = m_TPInfo.szLocalPath;
    m_strTmpPath += "/temp";
    CreateDeepDir(m_strTmpPath.c_str());

    //�õ�����ļ����к���Ϣ
    m_bCheckSN = INIFile.GetInt(szSecName,
                                CFG_CO_CHECK_SN,
                                m_bCheckSN);

    m_bFileLenVerify = INIFile.GetInt(szSecName,
                                  CFG_CO_FILE_LEN_VERIFY,
                                  m_bFileLenVerify);

    //���״̬�ļ���·��
    char szPathName[MAX_PATH];
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
    �����Ǽ��豾���ɼ��ĸ�����������NENAME��TPID����ϲ����ظ���
*/
    //��¼�ļ�����״̬��״̬�ļ�
    SNPRINTF(m_szStatusFile,
             sizeof(m_szStatusFile),
             "%s/%s_%s_status.dat",
             szPathName,
             m_TPInfo.szNename,
             m_TPInfo.szTPID);

    if(LoadStatus() != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
               "����:�ɼ�����(%s,TP=%s)��״̬�ļ�(%s)ʧ��",
               m_szSwitchName,
               m_TPInfo.szTPID,
               m_szStatusFile);
    }


    char szFileSNStatusFile[MAX_PATH];

    //�ļ����кŵ�״̬�ļ�
    SNPRINTF(szFileSNStatusFile,
             sizeof(szFileSNStatusFile),
             "%s/%s_%s_file_sn_status.dat",
             szPathName,
             m_TPInfo.szNename,
             m_TPInfo.szTPID);


    char szMgrInfo[BUF_SIZE];
    SNPRINTF(szMgrInfo,
        sizeof(szMgrInfo),
        "�ɼ�����(%s,TP=%s)�ļ����кŹ�����",
        m_szSwitchName,
        m_TPInfo.szTPID);

    szMgrInfo[BUF_SIZE - 1] = '\0';

    m_pFileSNMgr = new CFileSNMgr(szFileSNStatusFile,
                                nCheckSNDepth,
                                S1240_FILE_SN_LEN,
                                0,
                                GetFileNameBySN);

    m_pFileSNMgr->SetSNLimit(S1240_FILE_SN_LIMIT);
    m_pFileSNMgr->SetMgrInfo(szMgrInfo);

    if (ERR_SUCCESS != m_pFileSNMgr->Init())
    {
        delete m_pFileSNMgr;
        m_pFileSNMgr = NULL;
    }

    GetAlarmLimit();

    return ERR_SUCCESS;
}

//�������ӷ����
int CS1240TPCollect::TestLink(MAP<STRING, SLinkStatusMsg>& LinkStatus)
{
    return m_pPriLinkMgr->TestLink(LinkStatus);
}

STRING CS1240TPCollect::GetDstFileName(const char* const szSrcFile)
{
    char szTime[50];
    time_t tNow = time(NULL);
    strftime(szTime, sizeof(szTime), "%H%M", localtime(&tNow));
    return STRING(szSrcFile) + "_" + STRING(szTime) + "." + m_TPInfo.szBilltype;
}

int CS1240TPCollect::MakeFileNameMsg(const char* const szSrcFile,
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

    //Դ�ļ�·��
    stGetReq.szSrcPath[0] = 0;

    //Դ�ļ���
    strncpy(stGetReq.szSrcName, szSrcFile, sizeof(stGetReq.szSrcName));

    //��ǰĿ�Ļ����ļ�·��
    strcpy(stGetReq.szDstPath, m_szLocalPath);

    //Ŀ���ļ���
    strncpy(stGetReq.szDstName,
            GetDstFileName(stGetReq.szSrcName).c_str(),
            sizeof(stGetReq.szDstName));

    req = stGetReq;

    return ERR_SUCCESS;
}

/*
    ����ȡ�ļ������ļ�״̬�����stReq.szDstNameΪ�գ����ʾ�Զ��ɼ�
    ���stReq.szDstName��Ϊ�գ���file_status.bAdditionalΪtrue����
    ��ʾ�ֶ��ɼ������ﻹ����һ���������stReq.szDstName��Ϊ�գ���
    file_status.bAdditionalΪfalse������������ϴ��Զ��ɼ�ʧ�ܲ���
    �������
    �ֶ��ɼ���Ҫ�������������
    1��������������ʱ���û������ֶ��ɼ������δָ���ļ���������Ϊ��
       �Զ��ɼ���ֻ���û����¶���ɼ�˳�����ָ�����ļ���������Ϊ
       ���ֶ��ɼ��������Ҫ�������Զ��ɼ������Ŀ���ļ���Ϊ�ա�
    2����������������Щ�ϴν����ֶ��ɼ�δ�ɹ����ļ�����������±�
       ��Ϊfile_status.bAdditionalΪtrue����file_status.szDstFile��
       Ϊ�ա�
*/
int CS1240TPCollect::GetFile(SColFileReq& stReq)
{
    assert(NULL != m_pCmiseMgr);

    //ԭʼ�ļ��ڽ������в����ڣ��򷵻�ʧ��
    const char* szFileName = stReq.szSrcName;
    if(szFileName == NULL
       || szFileName[0] == '\0'
       || m_FileList.find(szFileName) == m_FileList.end())
    {
        return ERR_FAIL;
    }

    SS1240FileStatus& file_status = m_FileList[szFileName];

    int nRet;
    //�����Ҫ�ɼ��������ļ�״̬ΪTRANSFERRING
    if(CS_NOT_COLLECTED == file_status.nStatus)
    {
        switch(file_status.file_msg.nFileStatus)
        {
        case CMISE_STATUS_FREE:
            if(stReq.szDstName[0] != '\0')
            {
                if(!file_status.bAdditional)
                {
                    file_status.nFinalStatus = CMISE_STATUS_FREE;
                    file_status.bAdditional  = TRUE;
                }
                strncpy(file_status.szDstFile,
                        stReq.szDstName,
                        sizeof(file_status.szDstFile));
            }
            else
            {
                file_status.nFinalStatus = m_TPInfo.nFinalStatus;
                file_status.bAdditional  = FALSE;
                file_status.szDstFile[0] = '\0';
            }
            file_status.file_msg.nFileStatus = CMISE_STATUS_READYTOBETRANSFERRED;
            file_status.nLastSetStatus       = CMISE_STATUS_READYTOBETRANSFERRED;
            if(SaveStatus() != ERR_SUCCESS)
            {
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                       "����:�ɼ�����(%s,TP=%s)����״̬�ļ�(%s)ʧ��",
                       m_szSwitchName,
                       m_TPInfo.szTPID,
                       m_szStatusFile);
            }
            nRet = m_pCmiseMgr->SetCmiseFile(szFileName,
                                             CMISE_STATUS_READYTOBETRANSFERRED);
            if(ERR_SUCCESS != nRet)
            {
                file_status.file_msg.nFileStatus = CMISE_STATUS_FREE;
                file_status.nLastSetStatus       = CMISE_STATUS_FREE;
                if(SaveStatus() != ERR_SUCCESS)
                {
                    COMSGOUT(MSGOUT_LEVEL_URGENT,
                           "����:�ɼ�����(%s,TP=%s)����״̬�ļ�(%s)ʧ��",
                           m_szSwitchName,
                           m_TPInfo.szTPID,
                           m_szStatusFile);
                }
                COTRACE("����:�ɼ�����(%s,TP=%s)�ڲɼ��ļ�(%s)ʱ���ļ�״̬=%dʧ��",
                       m_szSwitchName,
                       m_TPInfo.szTPID,
                       szFileName,
                       CMISE_STATUS_READYTOBETRANSFERRED);
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                       "����:�ɼ�����(%s,TP=%s)�ڲɼ��ļ�(%s)ʱ���ļ�״̬=%dʧ��",
                       m_szSwitchName,
                       m_TPInfo.szTPID,
                       szFileName,
                       CMISE_STATUS_READYTOBETRANSFERRED);
                return nRet;
            }
        case CMISE_STATUS_READYTOBETRANSFERRED:
            //�ȴ�״̬��Ŀ���Ǳ���������״̬�ɹ�����������˳��������´�������
            //����ΪTRANSFERRING״̬�����Լ����õ�
            if(stReq.szDstName[0] != '\0')
            {
                if(!file_status.bAdditional)
                {
                    file_status.nFinalStatus = CMISE_STATUS_FREE;
                    file_status.bAdditional  = TRUE;
                }
                strncpy(file_status.szDstFile,
                        stReq.szDstName,
                        sizeof(file_status.szDstFile));
            }
            else
            {
                file_status.nFinalStatus = m_TPInfo.nFinalStatus;
                file_status.bAdditional  = FALSE;
                file_status.szDstFile[0] = '\0';
            }
            file_status.file_msg.nFileStatus = CMISE_STATUS_TRANSFERRING;
            file_status.nLastSetStatus       = CMISE_STATUS_TRANSFERRING;
            if(SaveStatus() != ERR_SUCCESS)
            {
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                       "����:�ɼ�����(%s,TP=%s)����״̬�ļ�(%s)ʧ��",
                       m_szSwitchName,
                       m_TPInfo.szTPID,
                       m_szStatusFile);
            }
            nRet = m_pCmiseMgr->SetCmiseFile(szFileName,
                                             CMISE_STATUS_TRANSFERRING);
            if(ERR_SUCCESS != nRet)
            {
                COTRACE("����:�ɼ�����(%s,TP=%s)�ڲɼ��ļ�(%s)ʱ���ļ�״̬=%dʧ��",
                       m_szSwitchName,
                       m_TPInfo.szTPID,
                       szFileName,
                       CMISE_STATUS_TRANSFERRING);
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                       "����:�ɼ�����(%s,TP=%s)�ڲɼ��ļ�(%s)ʱ���ļ�״̬=%dʧ��",
                       m_szSwitchName,
                       m_TPInfo.szTPID,
                       szFileName,
                       CMISE_STATUS_TRANSFERRING);
                file_status.file_msg.nFileStatus = CMISE_STATUS_READYTOBETRANSFERRED;
                file_status.nLastSetStatus       = CMISE_STATUS_READYTOBETRANSFERRED;
                if(SaveStatus() != ERR_SUCCESS)
                {
                    COMSGOUT(MSGOUT_LEVEL_URGENT,
                           "����:�ɼ�����(%s,TP=%s)����״̬�ļ�(%s)ʧ��",
                           m_szSwitchName,
                           m_TPInfo.szTPID,
                           m_szStatusFile);
                }
                return nRet;
            }
            break;

        case CMISE_STATUS_TRANSFERRING:
            if(CMISE_STATUS_TRANSFERRING != file_status.nLastSetStatus)
            {
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                       "����:�ɼ�����(%s,TP=%s)�ɼ��ļ�(%s)ʱ����TRANSFERRING״̬�����������õ�",
                       m_szSwitchName,
                       m_TPInfo.szTPID,
                       szFileName);
                return ERR_FAIL;
            }
            break;

        default:
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "����:�ɼ�����(%s,TP=%s)�ɼ��ļ�(%s)ʱ����״̬������",
                   m_szSwitchName,
                   m_TPInfo.szTPID,
                   szFileName);
            return ERR_FAIL;
        }
    }

    /*
        ������㣬�ж��Ƿ����ֶ��ɼ���Ψһ��׼��file_status.bAdditionalΪtrue
    */
    //���ļ����вɼ�
    if(file_status.nStatus < CS_SETTING_COLLECTED)
    {
        file_status.nStatus = CS_COLLECTING;
        if(SaveStatus() != ERR_SUCCESS)
        {
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "����:�ɼ�����(%s,TP=%s)����״̬�ļ�(%s)ʧ��",
                   m_szSwitchName,
                   m_TPInfo.szTPID,
                   m_szStatusFile);
        }

        int nRet = DoGetAFile(stReq);
        if(ERR_SUCCESS != nRet)
        {
            file_status.nStatus = CS_NOT_COLLECTED;
            return nRet;
        }

        file_status.nStatus = CS_SETTING_COLLECTED;
        if(SaveStatus() != ERR_SUCCESS)
        {
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "����:�ɼ�����(%s,TP=%s)����״̬�ļ�(%s)ʧ��",
                   m_szSwitchName,
                   m_TPInfo.szTPID,
                   m_szStatusFile);
        }
    }

    //�ɼ��ɹ�����Ŀ��״̬
    if(file_status.nStatus == CS_SETTING_COLLECTED)
    {
        switch(file_status.file_msg.nFileStatus)
        {
        case CMISE_STATUS_TRANSFERRING:
            nRet = m_pCmiseMgr->SetCmiseFile(szFileName,
                                             CMISE_STATUS_FREE);
            if(ERR_SUCCESS != nRet)
            {
                COTRACE("����:�ɼ�����(%s,TP=%s)�ڲɼ��ļ�(%s)ʱ���ļ�״̬=%dʧ��",
                       m_szSwitchName,
                       m_TPInfo.szTPID,
                       szFileName,
                       CMISE_STATUS_FREE);
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                       "����:�ɼ�����(%s,TP=%s)�ڲɼ��ļ�(%s)ʱ���ļ�״̬=%dʧ��",
                       m_szSwitchName,
                       m_TPInfo.szTPID,
                       szFileName,
                       CMISE_STATUS_FREE);
                return nRet;
            }
            file_status.file_msg.nFileStatus = CMISE_STATUS_FREE;
            file_status.nLastSetStatus       = CMISE_STATUS_FREE;
        case CMISE_STATUS_FREE:
            if(file_status.file_msg.nFileStatus == file_status.nFinalStatus)
            {
                //���ļ���Ϣ�ӵ�ת���ļ�����
                SSwitchFileInfo stFileInfo;
                stFileInfo.nSwitchIndex = m_nSwitchIndex;
                strncpy(stFileInfo.szModuleLabel,
                        m_TPInfo.szTPID,
                        sizeof(stFileInfo.szModuleLabel));
                strncpy(stFileInfo.szRemoteFile,
                        stReq.szSrcName,
                        sizeof(stFileInfo.szRemoteFile));
                strncpy(stFileInfo.szLocalFile,
                        file_status.szDstFile,
                        sizeof(stFileInfo.szLocalFile));
                stFileInfo.nConvertInfo = m_nTPIndex;
                ::GetFilePool()->PushBack(stFileInfo);

                if(!file_status.bAdditional)
                {
                    file_status.nLastFileCounter = file_status.file_msg.nFileCounter;
                }
                file_status.bAdditional  = FALSE;
                file_status.nStatus      = CS_NOT_COLLECTED;
                file_status.nFinalStatus = m_TPInfo.nFinalStatus;
                file_status.szDstFile[0] = '\0';
                if(SaveStatus() != ERR_SUCCESS)
                {
                    COMSGOUT(MSGOUT_LEVEL_URGENT,
                           "����:�ɼ�����(%s,TP=%s)����״̬�ļ�(%s)ʧ��",
                           m_szSwitchName,
                           m_TPInfo.szTPID,
                           m_szStatusFile);
                }
                break;
            }
            nRet = m_pCmiseMgr->SetCmiseFile(szFileName,
                                             CMISE_STATUS_READYTOBETRANSFERRED);
            if(ERR_SUCCESS != nRet)
            {
                COTRACE("����:�ɼ�����(%s,TP=%s)�ڲɼ��ļ�(%s)ʱ���ļ�״̬=%dʧ��",
                       m_szSwitchName,
                       m_TPInfo.szTPID,
                       szFileName,
                       CMISE_STATUS_READYTOBETRANSFERRED);
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                       "����:�ɼ�����(%s,TP=%s)�ڲɼ��ļ�(%s)ʱ���ļ�״̬=%dʧ��",
                       m_szSwitchName,
                       m_TPInfo.szTPID,
                       szFileName,
                       CMISE_STATUS_READYTOBETRANSFERRED);
                return nRet;
            }
            file_status.file_msg.nFileStatus = CMISE_STATUS_READYTOBETRANSFERRED;
            file_status.nLastSetStatus       = CMISE_STATUS_READYTOBETRANSFERRED;
        case CMISE_STATUS_READYTOBETRANSFERRED:
            if(file_status.file_msg.nFileStatus == file_status.nFinalStatus)
            {
                //���ļ���Ϣ�ӵ�ת���ļ�����
                SSwitchFileInfo stFileInfo;
                stFileInfo.nSwitchIndex = m_nSwitchIndex;
                strncpy(stFileInfo.szModuleLabel,
                        m_TPInfo.szTPID,
                        sizeof(stFileInfo.szModuleLabel));
                strncpy(stFileInfo.szRemoteFile,
                        stReq.szSrcName,
                        sizeof(stFileInfo.szRemoteFile));
                strncpy(stFileInfo.szLocalFile,
                        file_status.szDstFile,
                        sizeof(stFileInfo.szLocalFile));
                stFileInfo.nConvertInfo = m_nTPIndex;
                ::GetFilePool()->PushBack(stFileInfo);

                if(!file_status.bAdditional)
                {
                    file_status.nLastFileCounter = file_status.file_msg.nFileCounter;
                }
                file_status.bAdditional  = FALSE;
                file_status.nStatus      = CS_NOT_COLLECTED;
                file_status.nFinalStatus = m_TPInfo.nFinalStatus;
                file_status.szDstFile[0] = '\0';
                if(SaveStatus() != ERR_SUCCESS)
                {
                    COMSGOUT(MSGOUT_LEVEL_URGENT,
                           "����:�ɼ�����(%s,TP=%s)����״̬�ļ�(%s)ʧ��",
                           m_szSwitchName,
                           m_TPInfo.szTPID,
                           m_szStatusFile);
                }
                break;
            }
        default:
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "����:�ɼ�����(%s,TP=%s)�ɼ��ļ�(%s)ʱ����״̬������",
                   m_szSwitchName,
                   m_TPInfo.szTPID,
                   szFileName);
            return ERR_FAIL;
        }
    }

    return ERR_SUCCESS;
}

//�����ļ�����˵ĵ����ļ�������;
int CS1240TPCollect::DoGetAFile(SColFileReq& stReq)
{
    const char* szFileName = stReq.szSrcName;

    SGetFileReq req;
    int nRet = MakeFileNameMsg(szFileName, req);
    if(nRet != ERR_SUCCESS)
    {
        COTRACE("�ɼ�����(%s,TP=%s)���ļ��ɼ�������������ļ�������Ϊ��",
              m_szSwitchName,
              m_TPInfo.szTPID);
        return nRet;
    }
    req.nFileSize = stReq.nFileSize;
    STRING strPrimSrcFile = stReq.szDstName;

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
            strPath = m_szLocalPath;
        }
#ifdef _PLATFORM_WIN32
        else if(strPath[1] != ':')
#else
        else if(strPath[0] != '/')
#endif
        {
            strPath = STRING(m_szLocalPath) + "/" + strPath;
        }

        strncpy(req.szDstPath,
                strPath.c_str(),
                sizeof(req.szDstPath));
        strncpy(req.szDstName,
                strFile.c_str(),
                sizeof(req.szDstName));
        CreateDeepDir(req.szDstPath);
    }

    //���б��в��Ҷ�Ӧ���ļ���Ϣ
    if(m_FileList.find(szFileName) == m_FileList.end())
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "����:�ɼ�����(%s,TP=%s)�ڲɼ��ļ�ʱδ�ҵ������ļ�(%s)",
               m_szSwitchName,
               m_TPInfo.szTPID,
               szFileName);
        m_MutexForMap.release();
        return ERR_FAIL;
    }
    SS1240FileStatus& file_status = m_FileList[szFileName];

    //������ʱ�ļ���
    string  strSrcFile = m_strTmpPath;
    strSrcFile += "/";
    strSrcFile += req.szDstName;

    //������ʽ�ļ���
    if((req.szDstPath[0] != 0) &&
       ((req.szDstPath[strlen(req.szDstPath) - 1] == '\\') ||
        (req.szDstPath[strlen(req.szDstPath) - 1] == '/')))
    {
        req.szDstPath[strlen(req.szDstPath) - 1] = '\0';
    }
    string strDstPath = req.szDstPath;
    string strDstFile = strDstPath + "/" + req.szDstName;

    //��ʼ���ļ�����
    req.nFileSize = 4.5*1024*1024;//file_status.file_msg.uFileSize;

    strncpy(req.szDstPath, m_strTmpPath.c_str(), sizeof(req.szDstPath));
    req.szDstPath[sizeof(req.szDstPath) - 1] = '\0';
    
    //ȡ�ļ�
    nRet = ERR_FAIL;
    if((file_status.file_msg.nFileMode == CMISE_MODE_OK)
       || (file_status.file_msg.nFileMode == CMISE_MODE_ORIGOK))
    {
        nRet = m_pPriLinkMgr->GetFile(req);
    }
    if(ERR_SUCCESS != nRet)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�ɼ�����(%s,TP=%s)�����ļ�(%s)�ɼ�ʧ��(������=%d)",
               m_szSwitchName,
               m_TPInfo.szTPID,
               req.szSrcName,
               nRet);
        COTRACE("�ɼ�����(%s,TP=%s)�����ļ�(%s)�ɼ�ʧ��(������=%d)",
              m_szSwitchName,
              m_TPInfo.szTPID,
              req.szSrcName,
              nRet);

        //ɾ����ʱ�ļ�
        remove(strSrcFile.c_str());

        //����TP�ı�����ļ���
        strncpy(req.szSrcName,
                file_status.file_msg.szFileIdDup,
                sizeof(req.szSrcName));
        req.szSrcName[sizeof(req.szSrcName) - 1] = '\0';

        if(strPrimSrcFile == "")
        {
            strncpy(req.szDstName,
                    GetDstFileName(req.szSrcName).c_str(),
                    sizeof(req.szDstName));
            req.szDstName[sizeof(req.szDstName) - 1] = '\0';

            strSrcFile = m_strTmpPath;
            strSrcFile += "/";
            strSrcFile += req.szDstName;
            strDstFile = strDstPath;
            strDstFile += "/";
            strSrcFile += req.szDstName;
        }

        //ȡ������ļ�
        nRet = ERR_FAIL;
        if((file_status.file_msg.nFileMode == CMISE_MODE_OK)
           || (file_status.file_msg.nFileMode == CMISE_MODE_DUPOK))
        {
            nRet = m_pBakLinkMgr->GetFile(req);
        }
        if(ERR_SUCCESS != nRet)
        {
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "�ɼ�����(%s,TP=%s)�ı��ļ�(%s)�ɼ�ʧ��(������=%d)",
                   m_szSwitchName,
                   m_TPInfo.szTPID,
                   req.szSrcName,
                   nRet);
            COTRACE("�ɼ�����(%s,TP=%s)�ı��ļ�(%s)�ɼ�ʧ��(������=%d)",
                  m_szSwitchName,
                  m_TPInfo.szTPID,
                  req.szSrcName,
                  nRet);
        }
    }

    //���ȡ�ļ�ʧ�ܣ���ɾ����ʱ�ļ������ش���
    if(ERR_SUCCESS != nRet)
    {
        remove(strSrcFile.c_str());
        return nRet;
    }

    //�����ļ�����У��
    nRet = ERR_FAIL;
    FILE* f = fopen(strSrcFile.c_str(), "rb");
    if(f != NULL)
    {
        int nLength = FileSize(f);
        fclose(f);
        if(m_bFileLenVerify && file_status.file_msg.uFileSize != nLength)
        {
            char szTraceMsg[1024];
            SNPRINTF(szTraceMsg,
                     sizeof(szTraceMsg),
                     "�ɼ�����(%s,TP=%s)�ɼ����ļ�(%s)���Ȳ�һ��(Զ��:%d, ����:%d)",
                     m_szSwitchName,
                     m_TPInfo.szTPID,
                     req.szSrcName,
                     file_status.file_msg.uFileSize,
                     nLength);
            szTraceMsg[sizeof(szTraceMsg) - 1] = '\0';
            COTRACE(szTraceMsg);
            COMSGOUT(MSGOUT_LEVEL_URGENT, szTraceMsg);
        }
        else
        {
            nRet = ERR_SUCCESS;
            stReq.nFileSize = nLength;
            file_status.file_msg.uFileSize = nLength;
        }
    }

    //����ļ�У��ʧ�ܣ���ɾ����ʱ�ļ������ش���
    if(ERR_SUCCESS != nRet)
    {
        remove(strSrcFile.c_str());
        return nRet;
    }

    //�����ļ�����
    BOOL bRet = FileCopy(strSrcFile.c_str(), strDstFile.c_str());
    if(!bRet)
    {
        char szTraceMsg[1024];
        SNPRINTF(szTraceMsg,
                 sizeof(szTraceMsg),
                 //"Collector(ID=%d) failed to move file:%s to %s",
                 "�ɼ�����(%s,TP=%s)�ƶ��ļ�ʧ��:%s to %s",
                 m_szSwitchName,
                 m_TPInfo.szTPID,
                 strSrcFile.c_str(),
                 strDstFile.c_str());
        szTraceMsg[sizeof(szTraceMsg) - 1] = '\0';
        COTRACE(szTraceMsg);
        COMSGOUT(MSGOUT_LEVEL_URGENT, szTraceMsg);
        nRet = ERR_FAIL;
    }
    remove(strSrcFile.c_str());

    if(ERR_SUCCESS == nRet)
    {
        strncpy(m_FileList[szFileName].szDstFile,
                strDstFile.c_str(),
                sizeof(m_FileList[szFileName].szDstFile));
        strncpy(stReq.szDstName,
                strDstFile.c_str(),
                sizeof(stReq.szDstName));
    }

    return nRet;
}

//����ļ��б�
int  CS1240TPCollect::GetAllFileList()
{
    VECTOR<SS1240FileMsg> file_msg_list;
    int nRet;

    //��ȡSelected�ļ�
    nRet = m_pCmiseMgr->GetCmiseFileList(CMISE_STATUS_SELECTED,
                                         file_msg_list);
/*
    if(nRet != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�ɼ�����(%s,TP=%s)ȡ״̬ΪSELECTED���ļ��б�ʧ��(������=%d)",
               m_szSwitchName,
               m_TPInfo.szTPID,
               nRet);
        COTRACE("�ɼ�����(%s,TP=%s)ȡ״̬ΪSELECTED���ļ��б�ʧ��(������=%d)",
              m_szSwitchName,
              m_TPInfo.szTPID,
              nRet);
        return nRet;
    }
*/

    //��ȡRecoreding�ļ��б�
    nRet = m_pCmiseMgr->GetCmiseFileList(CMISE_STATUS_RECORDING,
                                         file_msg_list);
/*
    if(nRet != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�ɼ�����(%s,TP=%s)ȡ״̬ΪRECORDING���ļ��б�ʧ��(������=%d)",
               m_szSwitchName,
               m_TPInfo.szTPID,
               nRet);
        COTRACE("�ɼ�����(%s,TP=%s)ȡ״̬ΪRECORDING���ļ��б�ʧ��(������=%d)",
              m_szSwitchName,
              m_TPInfo.szTPID,
              nRet);
        return nRet;
    }
*/

    //��ȡ���ļ��б�
    nRet = m_pCmiseMgr->GetCmiseFileList(CMISE_STATUS_READYTOBETRANSFERRED,
                                        file_msg_list);
/*
    if(nRet != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�ɼ�����(%s,TP=%s)ȡ״̬ΪREADYTOBETRANSFERRED���ļ��б�ʧ��(������=%d)",
               m_szSwitchName,
               m_TPInfo.szTPID,
               nRet);
        COTRACE("�ɼ�����(%s,TP=%s)ȡ״̬ΪREADYTOBETRANSFERRED���ļ��б�ʧ��(������=%d)",
              m_szSwitchName,
              m_TPInfo.szTPID,
              nRet);
        return nRet;
    }
*/

    m_MutexForMap.acquire();
    int nFileCount = m_FileList.size();
    m_MutexForMap.release();

    //��ȡ���ͷ��ļ��б�
    if(nFileCount < 200)
    {
        nRet = m_pCmiseMgr->GetCmiseFileList(CMISE_STATUS_FREE,
                                             file_msg_list);
    }
/*
    if(nRet != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�ɼ�����(%s,TP=%s)ȡ״̬ΪRELEASED���ļ��б�ʧ��(������=%d)",
               m_szSwitchName,
               m_TPInfo.szTPID,
               nRet);
        COTRACE("�ɼ�����(%s,TP=%s)ȡ״̬ΪRELEASED���ļ��б�ʧ��(������=%d)",
              m_szSwitchName,
              m_TPInfo.szTPID,
              nRet);
        return nRet;
    }
*/

    //��ȡ���ڴ����ļ�
    nRet = m_pCmiseMgr->GetCmiseFileList(CMISE_STATUS_TRANSFERRING,
                                         file_msg_list);
/*
    if(nRet != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�ɼ�����(%s,TP=%s)ȡ״̬ΪTRANSFERRING���ļ��б�ʧ��(������=%d)",
               m_szSwitchName,
               m_TPInfo.szTPID,
               nRet);
        COTRACE("�ɼ�����(%s,TP=%s)ȡ״̬ΪTRANSFERRING���ļ��б�ʧ��(������=%d)",
              m_szSwitchName,
              m_TPInfo.szTPID,
              nRet);
        return nRet;
    }
*/

    m_MutexForMap.acquire();

    while(file_msg_list.size() > 0)
    {
        SS1240FileMsg& file_msg = file_msg_list.front();
        if(m_FileList.find(file_msg.szFileId) == m_FileList.end())
        {
            SS1240FileStatus file_status;
            file_status.bAdditional       = FALSE;
            file_status.nLastFileCounter  = -1;
            file_status.nLastSetStatus    = CMISE_STATUS_FREE;
            file_status.nStatus           = CS_NOT_COLLECTED;
            file_status.szDstFile[0]      = '\0';
            m_FileList[file_msg.szFileId] = file_status;
        }
        SS1240FileStatus& file_status     = m_FileList[file_msg.szFileId];
        file_status.file_msg = file_msg;
        if(CMISE_STATUS_TRANSFERRING != file_msg.nFileStatus)
        {
            file_status.nLastSetStatus    = file_msg.nFileStatus;
        }
        if(!file_status.bAdditional)
        {
            file_status.nFinalStatus      = m_TPInfo.nFinalStatus;
        }
        if(CS_COLLECTING == file_status.nStatus)
        {
            file_status.nStatus = CS_NOT_COLLECTED;
        }

        file_msg_list.erase(file_msg_list.begin());
    }

    if(SaveStatus() != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "����:�ɼ�����(%s,TP=%s)����״̬�ļ�(%s)ʧ��",
               m_szSwitchName,
               m_TPInfo.szTPID,
               m_szStatusFile);
    }

    m_MutexForMap.release();

    return ERR_SUCCESS;
}

//���ļ��б��в���ָ���ļ���ʵ�ʾ����ж�ָ���ļ��ܷ�����Զ����ֶ��ɼ�
int CS1240TPCollect::FindFileInList(const char* const szFileName)
{
    if(CanGetSpecFile(szFileName) == ERR_SUCCESS)
    {
        return ERR_SUCCESS;
    }

    if(CanAutoGet(szFileName) == ERR_SUCCESS)
    {
        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

//�ж�ָ���ļ��ܷ�����Զ��ɼ�
int CS1240TPCollect::CanAutoGet(const char* const szFileName)
{
    m_MutexForMap.acquire();
    if(m_FileList.find(szFileName) == m_FileList.end())
    {
        m_MutexForMap.release();
        return ERR_FAIL;
    }

    //�����ж��ǿ��Խ����ֶ��ɼ�
    SS1240FileStatus& file_status = m_FileList[szFileName];
    if(file_status.bAdditional)
    {
        m_MutexForMap.release();
        return ERR_FAIL;
    }

    int nRet = ERR_FAIL;
    if(file_status.nLastFileCounter != file_status.file_msg.nFileCounter)
    {

        switch(file_status.file_msg.nFileStatus)
        {
        case CMISE_STATUS_FREE:
            //�Ѳɼ���ֻ��״̬δ���óɹ�
            if(file_status.nStatus >= CS_SETTING_COLLECTED)
            {
                nRet = ERR_SUCCESS;
            }
            break;

        case CMISE_STATUS_READYTOBETRANSFERRED:
            nRet = ERR_SUCCESS;
            if(m_TPInfo.bDumped && 0 == file_status.file_msg.nFileLocalDumped)
            {
                nRet = ERR_FAIL;
            }
            break;

        case CMISE_STATUS_TRANSFERRING:
            if(CMISE_STATUS_TRANSFERRING == file_status.nLastSetStatus)
            {
                nRet = ERR_SUCCESS;
            }
            break;

        default:
            break;
        }
    }

    m_MutexForMap.release();

    return nRet;
}

//����ļ��б�
int CS1240TPCollect::GetList(VECTOR<SS1240FileMsg>& pOut)
{
    GetAllFileList();

    m_MutexForMap.acquire();

    MAP<STRING, SS1240FileStatus>::iterator j = m_FileList.begin();
    for(; j != m_FileList.end(); j++)
    {
        SS1240FileStatus& file_status = j->second;

        if(file_status.bAdditional)
        {
            pOut.push_back(file_status.file_msg);
            continue;
        }
    }

    for(j = m_FileList.begin(); j != m_FileList.end(); j++)
    {
        SS1240FileStatus& file_status = j->second;

        if(CanAutoGet(file_status.file_msg.szFileId) == ERR_SUCCESS)
        {
            pOut.push_back(file_status.file_msg);
            continue;
        }
    }

    m_MutexForMap.release();

    return ERR_SUCCESS;
}

//��������ļ��б�
int  CS1240TPCollect::GetAllList(MAP<STRING, SS1240FileStatus>& Out)
{
    m_MutexForMap.acquire();

    Out = m_FileList;

    m_MutexForMap.release();

    return ERR_SUCCESS;
}

//���alarmlimit
int  CS1240TPCollect::GetFilePool(SS1240FilePoolMsg& pool_msg)
{
    return m_pCmiseMgr->GetFilePool(pool_msg);
}

//���alarmlimit
int CS1240TPCollect::GetAlarmLimit()
{
    if(m_nAlarmLimit == -1)
    {
        SS1240FilePoolMsg pool_msg;
        int nRet = GetFilePool(pool_msg);
        if(nRet == 0)
        {
            m_nAlarmLimit = pool_msg.nAlarmLimit;
        }
    }

    if(m_nAlarmLimit < 0)
    {
        return 0;
    }

    return m_nAlarmLimit;
}

//����alarmlimit
int  CS1240TPCollect::SetAlarmLimit(int nAlarmLimit)
{

    int nRet = m_pCmiseMgr->SetFilePool(nAlarmLimit);
    if(nRet == 0)
    {
        m_nAlarmLimit = nAlarmLimit;
    }

    return nRet;
}

//��̬������Ŀ¼
BOOL CS1240TPCollect::CreateLocSubDir()
{
    char szTime[50];
    time_t tNow = time(NULL);
    strftime(szTime, sizeof(szTime), "%Y%m%d", localtime(&tNow));

    SNPRINTF(m_szLocalPath,
             sizeof(m_szLocalPath),
             "%s/%s",
             m_TPInfo.szLocalPath,
             szTime);
    BOOL nRet = CreateDeepDir(m_szLocalPath);

    return nRet != FALSE ? TRUE : FALSE;
}

//��������ļ�
BOOL CS1240TPCollect::ClearOldFile()
{
    int nRet = CCollectBase::ClearOldDateSubDir(m_TPInfo.szLocalPath,
                                                m_nFileSaveDays);

    return nRet == ERR_SUCCESS;
}

//����ļ����к��Ƿ�����
BOOL CS1240TPCollect::CheckFileSN(const char* szFileName, char* szPreviousFile)
{
    if ((m_pFileSNMgr ==NULL)||(m_bCheckSN == 0))
    {
        return TRUE;
    }

    //TP-FILE-10000
    return m_pFileSNMgr->CheckFileSN(szFileName, szPreviousFile);
}

//�ļ��������Ļص�������ͨ���ļ������кż���©���ļ���
STRING CS1240TPCollect::GetFileNameBySN(const int   nFileSN,
                                        const char* szReferName,
                                        const int   nSNLen,
                                        const int   nPostfixLen)
{

    //�������к�
    int nPrefixLen = strlen(S1240_FILE_PREFIX);

    char szFileSNHigh[3];
    memcpy(szFileSNHigh, szReferName + nPrefixLen, 2);
    szFileSNHigh[2] = '\0';

    int nFileSNHigh = atoi(szFileSNHigh);

    //����TP��A���B��ĸ�λ���к�
    int nFileSNHighTPA, nFileSNHighTPB;
    if (nFileSNHigh % 2 == 0)
    {
        nFileSNHighTPA = nFileSNHigh;
        nFileSNHighTPB = nFileSNHigh + 1;
    }
    else
    {
        nFileSNHighTPA = nFileSNHigh - 1;
        nFileSNHighTPB = nFileSNHigh;
    }

    char szFileTPA[20], szFileTPB[20];

    sprintf(szFileTPA, "A��%s%02d%03d",
            S1240_FILE_PREFIX,
            nFileSNHighTPA,
            nFileSN);
    sprintf(szFileTPB, "B��%s%02d%03d",
            S1240_FILE_PREFIX,
            nFileSNHighTPB,
            nFileSN);


    STRING strFileName = "";
    strFileName = strFileName + szFileTPA + "," + szFileTPB;

    return strFileName;

}

void CS1240TPCollect::SetCfgPath(const char* const szCfgPath)
{
    strncpy(m_szCfgPath, szCfgPath, MAX_PATH);
    m_szCfgPath[MAX_PATH - 1] = '\0';
}

void CS1240TPCollect::SetFileSaveDays(int nFileSaveDays)
{
    m_nFileSaveDays = nFileSaveDays;
}

//ָ�����ļ��Ƿ���Խ����ֹ��ɼ�
int CS1240TPCollect::CanGetSpecFile(const char* const szSrcFile)
{
    m_MutexForMap.acquire();

    if(m_FileList.find(szSrcFile) == m_FileList.end())
    {
        m_MutexForMap.release();
        return ERR_FILE_CANNOT_BE_FOUND;
    }

    SS1240FileStatus& file_status = m_FileList[szSrcFile];
    if(file_status.nStatus == CS_COLLECTING)
    {
        m_MutexForMap.release();
        return ERR_FILE_BEING_COLLECTED;
    }

    int nRet = ERR_FAIL;
    switch(file_status.file_msg.nFileStatus)
    {
    case CMISE_STATUS_FREE:
        nRet = ERR_SUCCESS;
        break;

    case CMISE_STATUS_READYTOBETRANSFERRED:
        nRet = ERR_SUCCESS;
        if(m_TPInfo.bDumped && 0 == file_status.file_msg.nFileLocalDumped)
        {
            nRet = ERR_FILE_STATUS_NOT_CORRECT;
        }
        break;

    default:
        break;
    }

    m_MutexForMap.release();

    return nRet;
}

//��ȡ�ļ�����
int CS1240TPCollect::GetFileLen(const char* const szSrcFile)
{
    m_MutexForMap.acquire();

    if(m_FileList.find(szSrcFile) == m_FileList.end())
    {
        m_MutexForMap.release();
        return 0;
    }

    SS1240FileStatus& file_status = m_FileList[szSrcFile];

    int nFileLen = file_status.file_msg.uFileSize;

    m_MutexForMap.release();

    return nFileLen;
}

