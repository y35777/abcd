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
        COTRACE("采集对象(%s, TP=%s)打开/创建状态文件(%s)失败",
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

//初始化采集服务
int  CS1240TPCollect::Init()
{
    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        return ERR_FAIL;
    }

    /*
        在相关交换机字段获取信息
    */
    char szSecName[S1240_CFG_LEN];
    SNPRINTF(szSecName,
             sizeof(szSecName),
             "%s%d", //"switch*"
             CFG_CO_SEC_SWITCH,
             m_nSwitchIndex);
    szSecName[S1240_CFG_LEN - 1] = '\0';

    //获取交换机的Nename
    char szNename[S1240_CFG_LEN];
    INIFile.GetString(szSecName,
                      CFG_CO_TP_NENAME,
                      CFG_CO_TP_NENAME_DEFAULT,
                      szNename,
                      sizeof(szNename));

    //取调试用的远端目录
    INIFile.GetString(szSecName,
                      CFG_CO_REMOTE_DIR,
                      CFG_CO_REMOTE_DIR_DEFAULT,
                      m_szRemotePath,
                      sizeof(m_szRemotePath));

    //获取源文件状态是否是DUMPED
    m_TPInfo.bDumped = INIFile.GetInt(szSecName,
                                      CFG_CO_DUMPED,
                                      CFG_CO_DUMPED_DEFAULT);

    //获取允许采集失败的最大文件数
    m_nCoFailAlarmLimit = INIFile.GetInt(szSecName,
                                         CFG_CO_FAIL_ALARM_LIMIT,
                                         CFG_CO_FAIL_ALARM_LIMIT_DEFAULT);
    if(m_nCoFailAlarmLimit < 0)
    {
        m_nCoFailAlarmLimit = CFG_CO_FAIL_ALARM_LIMIT_DEFAULT;
    }

    //获取目标状态
    char szFinalStatus[S1240_CFG_LEN];
    INIFile.GetString(szSecName,
                      CFG_CO_FINAL_STATUS,
                      CFG_CO_FINAL_STATUS_DEFAULT,
                      szFinalStatus,
                      sizeof(szFinalStatus));

    //获取交换机对应的本地根目录名
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

    //得到检查文件序列号信息
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

    //是否进行文件长度校验
    m_bFileLenVerify = INIFile.GetInt(CFG_CO_SEC_SYSTEM,
                                  CFG_CO_FILE_LEN_VERIFY,
                                  CFG_CO_FILE_LEN_VERIFY_DEFAULT);
    m_bFileLenVerify = INIFile.GetInt(szSecName,
                                  CFG_CO_FILE_LEN_VERIFY,
                                  m_bFileLenVerify);

    /*
        在相关TP字段获取信息
    */
    SNPRINTF(szSecName, sizeof(szSecName),
             "%s%d_%s%d", //"switch*_tp*"
             CFG_CO_SEC_SWITCH, m_nSwitchIndex,
             CFG_CO_SEC_TP, m_nTPIndex);
    szSecName[S1240_CFG_LEN - 1] = '\0';

    //获取交换机TP的Nename
    INIFile.GetString(szSecName,
                      CFG_CO_TP_NENAME,
                      szNename,
                      m_TPInfo.szNename,
                      sizeof(m_TPInfo.szNename));
    if(strlen(szNename) == 0)
    {
        COTRACE("采集对象(%s,TP=%d)的NENAME未配置",
              m_szSwitchName,
              m_nTPIndex);
        return ERR_FAIL;
    }

    //取调试用的远端目录
    INIFile.GetString(szSecName,
                      CFG_CO_REMOTE_DIR,
                      m_szRemotePath,
                      m_szRemotePath,
                      sizeof(m_szRemotePath));

    //获取交换机TP的ID
    INIFile.GetString(szSecName,
                      CFG_CO_TP_ID,
                      CFG_CO_TP_ID_DEFAULT,
                      m_TPInfo.szTPID,
                      sizeof(m_TPInfo.szTPID));
    if(strlen(m_TPInfo.szTPID) == 0)
    {
        COTRACE("采集对象(%s,TP=%d)的TPID未配置",
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

    //获取交换机TP对应话单类型
    INIFile.GetString(szSecName,
                      CFG_CO_BILL_TYPE,
                      CFG_CO_BILL_TYPE_DEFAULT,
                      m_TPInfo.szBilltype,
                      sizeof(m_TPInfo.szBilltype));

    //获取允许采集失败的最大文件数
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

    //获取源文件状态是否是DUMPED
    m_TPInfo.bDumped = INIFile.GetInt(szSecName,
                                      CFG_CO_DUMPED,
                                      m_TPInfo.bDumped);

    //获取目标状态
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

    //获取交换机TP对应的本地根目录名
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

    //创建话单存放临时目录
    m_strTmpPath = m_TPInfo.szLocalPath;
    m_strTmpPath += "/temp";
    CreateDeepDir(m_strTmpPath.c_str());

    //得到检查文件序列号信息
    m_bCheckSN = INIFile.GetInt(szSecName,
                                CFG_CO_CHECK_SN,
                                m_bCheckSN);

    m_bFileLenVerify = INIFile.GetInt(szSecName,
                                  CFG_CO_FILE_LEN_VERIFY,
                                  m_bFileLenVerify);

    //获得状态文件的路径
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
    这里要注意各个状态文件名不能重复，否则会出现问题。
    这里是假设本机采集的各个交换机的NENAME及TPID的组合不会重复。
*/
    //记录文件传送状态的状态文件
    SNPRINTF(m_szStatusFile,
             sizeof(m_szStatusFile),
             "%s/%s_%s_status.dat",
             szPathName,
             m_TPInfo.szNename,
             m_TPInfo.szTPID);

    if(LoadStatus() != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
               "警告:采集对象(%s,TP=%s)读状态文件(%s)失败",
               m_szSwitchName,
               m_TPInfo.szTPID,
               m_szStatusFile);
    }


    char szFileSNStatusFile[MAX_PATH];

    //文件序列号的状态文件
    SNPRINTF(szFileSNStatusFile,
             sizeof(szFileSNStatusFile),
             "%s/%s_%s_file_sn_status.dat",
             szPathName,
             m_TPInfo.szNename,
             m_TPInfo.szTPID);


    char szMgrInfo[BUF_SIZE];
    SNPRINTF(szMgrInfo,
        sizeof(szMgrInfo),
        "采集对象(%s,TP=%s)文件序列号管理器",
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

//测试连接服务端
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
        //COTRACE("采集文件名不能为空：输入参数错误.");
        COTRACE("Remote file name is not specified: failed to get file");
        return ERR_FAIL;
    }

    SGetFileReq stGetReq;
    memset(&stGetReq,0, sizeof(SGetFileReq));

    //源文件路径
    stGetReq.szSrcPath[0] = 0;

    //源文件名
    strncpy(stGetReq.szSrcName, szSrcFile, sizeof(stGetReq.szSrcName));

    //当前目的话单文件路径
    strcpy(stGetReq.szDstPath, m_szLocalPath);

    //目的文件名
    strncpy(stGetReq.szDstName,
            GetDstFileName(stGetReq.szSrcName).c_str(),
            sizeof(stGetReq.szDstName));

    req = stGetReq;

    return ERR_SUCCESS;
}

/*
    负责取文件并置文件状态，如果stReq.szDstName为空，则表示自动采集
    如果stReq.szDstName不为空，且file_status.bAdditional为true，则
    表示手动采集，这里还存在一种情况，即stReq.szDstName不为空，但
    file_status.bAdditional为false，这种情况是上次自动采集失败产生
    的情况。
    手动采集主要存在两种情况：
    1、程序正常运行时，用户触发手动采集，如果未指定文件名，则认为是
       自动采集，只是用户重新定义采集顺序；如果指定了文件名，则认为
       是手动采集。这里就要求所有自动采集输入的目标文件名为空。
    2、程序重启后发现有些上次进行手动采集未成功的文件，这种情况下表
       现为file_status.bAdditional为true，且file_status.szDstFile不
       为空。
*/
int CS1240TPCollect::GetFile(SColFileReq& stReq)
{
    assert(NULL != m_pCmiseMgr);

    //原始文件在交换机中不存在，则返回失败
    const char* szFileName = stReq.szSrcName;
    if(szFileName == NULL
       || szFileName[0] == '\0'
       || m_FileList.find(szFileName) == m_FileList.end())
    {
        return ERR_FAIL;
    }

    SS1240FileStatus& file_status = m_FileList[szFileName];

    int nRet;
    //如果需要采集，则置文件状态为TRANSFERRING
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
                       "警告:采集对象(%s,TP=%s)保存状态文件(%s)失败",
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
                           "警告:采集对象(%s,TP=%s)保存状态文件(%s)失败",
                           m_szSwitchName,
                           m_TPInfo.szTPID,
                           m_szStatusFile);
                }
                COTRACE("警告:采集对象(%s,TP=%s)在采集文件(%s)时置文件状态=%d失败",
                       m_szSwitchName,
                       m_TPInfo.szTPID,
                       szFileName,
                       CMISE_STATUS_READYTOBETRANSFERRED);
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                       "警告:采集对象(%s,TP=%s)在采集文件(%s)时置文件状态=%d失败",
                       m_szSwitchName,
                       m_TPInfo.szTPID,
                       szFileName,
                       CMISE_STATUS_READYTOBETRANSFERRED);
                return nRet;
            }
        case CMISE_STATUS_READYTOBETRANSFERRED:
            //先存状态的目的是避免在设置状态成功后程序立即退出，程序下次启动后
            //会认为TRANSFERRING状态不是自己设置的
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
                       "警告:采集对象(%s,TP=%s)保存状态文件(%s)失败",
                       m_szSwitchName,
                       m_TPInfo.szTPID,
                       m_szStatusFile);
            }
            nRet = m_pCmiseMgr->SetCmiseFile(szFileName,
                                             CMISE_STATUS_TRANSFERRING);
            if(ERR_SUCCESS != nRet)
            {
                COTRACE("警告:采集对象(%s,TP=%s)在采集文件(%s)时置文件状态=%d失败",
                       m_szSwitchName,
                       m_TPInfo.szTPID,
                       szFileName,
                       CMISE_STATUS_TRANSFERRING);
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                       "警告:采集对象(%s,TP=%s)在采集文件(%s)时置文件状态=%d失败",
                       m_szSwitchName,
                       m_TPInfo.szTPID,
                       szFileName,
                       CMISE_STATUS_TRANSFERRING);
                file_status.file_msg.nFileStatus = CMISE_STATUS_READYTOBETRANSFERRED;
                file_status.nLastSetStatus       = CMISE_STATUS_READYTOBETRANSFERRED;
                if(SaveStatus() != ERR_SUCCESS)
                {
                    COMSGOUT(MSGOUT_LEVEL_URGENT,
                           "警告:采集对象(%s,TP=%s)保存状态文件(%s)失败",
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
                       "警告:采集对象(%s,TP=%s)采集文件(%s)时发现TRANSFERRING状态不是自已设置的",
                       m_szSwitchName,
                       m_TPInfo.szTPID,
                       szFileName);
                return ERR_FAIL;
            }
            break;

        default:
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "警告:采集对象(%s,TP=%s)采集文件(%s)时发现状态不正常",
                   m_szSwitchName,
                   m_TPInfo.szTPID,
                   szFileName);
            return ERR_FAIL;
        }
    }

    /*
        到这个点，判断是否是手动采集的唯一标准是file_status.bAdditional为true
    */
    //对文件进行采集
    if(file_status.nStatus < CS_SETTING_COLLECTED)
    {
        file_status.nStatus = CS_COLLECTING;
        if(SaveStatus() != ERR_SUCCESS)
        {
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "警告:采集对象(%s,TP=%s)保存状态文件(%s)失败",
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
                   "警告:采集对象(%s,TP=%s)保存状态文件(%s)失败",
                   m_szSwitchName,
                   m_TPInfo.szTPID,
                   m_szStatusFile);
        }
    }

    //采集成功，置目标状态
    if(file_status.nStatus == CS_SETTING_COLLECTED)
    {
        switch(file_status.file_msg.nFileStatus)
        {
        case CMISE_STATUS_TRANSFERRING:
            nRet = m_pCmiseMgr->SetCmiseFile(szFileName,
                                             CMISE_STATUS_FREE);
            if(ERR_SUCCESS != nRet)
            {
                COTRACE("警告:采集对象(%s,TP=%s)在采集文件(%s)时置文件状态=%d失败",
                       m_szSwitchName,
                       m_TPInfo.szTPID,
                       szFileName,
                       CMISE_STATUS_FREE);
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                       "警告:采集对象(%s,TP=%s)在采集文件(%s)时置文件状态=%d失败",
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
                //将文件信息加到转换文件池中
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
                           "警告:采集对象(%s,TP=%s)保存状态文件(%s)失败",
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
                COTRACE("警告:采集对象(%s,TP=%s)在采集文件(%s)时置文件状态=%d失败",
                       m_szSwitchName,
                       m_TPInfo.szTPID,
                       szFileName,
                       CMISE_STATUS_READYTOBETRANSFERRED);
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                       "警告:采集对象(%s,TP=%s)在采集文件(%s)时置文件状态=%d失败",
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
                //将文件信息加到转换文件池中
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
                           "警告:采集对象(%s,TP=%s)保存状态文件(%s)失败",
                           m_szSwitchName,
                           m_TPInfo.szTPID,
                           m_szStatusFile);
                }
                break;
            }
        default:
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "警告:采集对象(%s,TP=%s)采集文件(%s)时发现状态不正常",
                   m_szSwitchName,
                   m_TPInfo.szTPID,
                   szFileName);
            return ERR_FAIL;
        }
    }

    return ERR_SUCCESS;
}

//拷贝文件服务端的单个文件到本地;
int CS1240TPCollect::DoGetAFile(SColFileReq& stReq)
{
    const char* szFileName = stReq.szSrcName;

    SGetFileReq req;
    int nRet = MakeFileNameMsg(szFileName, req);
    if(nRet != ERR_SUCCESS)
    {
        COTRACE("采集对象(%s,TP=%s)的文件采集函数，输入的文件名参数为空",
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

    //在列表中查找对应的文件信息
    if(m_FileList.find(szFileName) == m_FileList.end())
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "警告:采集对象(%s,TP=%s)在采集文件时未找到输入文件(%s)",
               m_szSwitchName,
               m_TPInfo.szTPID,
               szFileName);
        m_MutexForMap.release();
        return ERR_FAIL;
    }
    SS1240FileStatus& file_status = m_FileList[szFileName];

    //设置临时文件名
    string  strSrcFile = m_strTmpPath;
    strSrcFile += "/";
    strSrcFile += req.szDstName;

    //设置正式文件名
    if((req.szDstPath[0] != 0) &&
       ((req.szDstPath[strlen(req.szDstPath) - 1] == '\\') ||
        (req.szDstPath[strlen(req.szDstPath) - 1] == '/')))
    {
        req.szDstPath[strlen(req.szDstPath) - 1] = '\0';
    }
    string strDstPath = req.szDstPath;
    string strDstFile = strDstPath + "/" + req.szDstName;

    //初始化文件长度
    req.nFileSize = 4.5*1024*1024;//file_status.file_msg.uFileSize;

    strncpy(req.szDstPath, m_strTmpPath.c_str(), sizeof(req.szDstPath));
    req.szDstPath[sizeof(req.szDstPath) - 1] = '\0';
    
    //取文件
    nRet = ERR_FAIL;
    if((file_status.file_msg.nFileMode == CMISE_MODE_OK)
       || (file_status.file_msg.nFileMode == CMISE_MODE_ORIGOK))
    {
        nRet = m_pPriLinkMgr->GetFile(req);
    }
    if(ERR_SUCCESS != nRet)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "采集对象(%s,TP=%s)的主文件(%s)采集失败(错误码=%d)",
               m_szSwitchName,
               m_TPInfo.szTPID,
               req.szSrcName,
               nRet);
        COTRACE("采集对象(%s,TP=%s)的主文件(%s)采集失败(错误码=%d)",
              m_szSwitchName,
              m_TPInfo.szTPID,
              req.szSrcName,
              nRet);

        //删除临时文件
        remove(strSrcFile.c_str());

        //重组TP的备侧的文件名
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

        //取备侧的文件
        nRet = ERR_FAIL;
        if((file_status.file_msg.nFileMode == CMISE_MODE_OK)
           || (file_status.file_msg.nFileMode == CMISE_MODE_DUPOK))
        {
            nRet = m_pBakLinkMgr->GetFile(req);
        }
        if(ERR_SUCCESS != nRet)
        {
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "采集对象(%s,TP=%s)的备文件(%s)采集失败(错误码=%d)",
                   m_szSwitchName,
                   m_TPInfo.szTPID,
                   req.szSrcName,
                   nRet);
            COTRACE("采集对象(%s,TP=%s)的备文件(%s)采集失败(错误码=%d)",
                  m_szSwitchName,
                  m_TPInfo.szTPID,
                  req.szSrcName,
                  nRet);
        }
    }

    //如果取文件失败，则删除临时文件，返回错误
    if(ERR_SUCCESS != nRet)
    {
        remove(strSrcFile.c_str());
        return nRet;
    }

    //进行文件长度校验
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
                     "采集对象(%s,TP=%s)采集的文件(%s)长度不一致(远端:%d, 本端:%d)",
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

    //如果文件校验失败，则删除临时文件，返回错误
    if(ERR_SUCCESS != nRet)
    {
        remove(strSrcFile.c_str());
        return nRet;
    }

    //进行文件更名
    BOOL bRet = FileCopy(strSrcFile.c_str(), strDstFile.c_str());
    if(!bRet)
    {
        char szTraceMsg[1024];
        SNPRINTF(szTraceMsg,
                 sizeof(szTraceMsg),
                 //"Collector(ID=%d) failed to move file:%s to %s",
                 "采集对象(%s,TP=%s)移动文件失败:%s to %s",
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

//获得文件列表
int  CS1240TPCollect::GetAllFileList()
{
    VECTOR<SS1240FileMsg> file_msg_list;
    int nRet;

    //获取Selected文件
    nRet = m_pCmiseMgr->GetCmiseFileList(CMISE_STATUS_SELECTED,
                                         file_msg_list);
/*
    if(nRet != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "采集对象(%s,TP=%s)取状态为SELECTED的文件列表失败(错误码=%d)",
               m_szSwitchName,
               m_TPInfo.szTPID,
               nRet);
        COTRACE("采集对象(%s,TP=%s)取状态为SELECTED的文件列表失败(错误码=%d)",
              m_szSwitchName,
              m_TPInfo.szTPID,
              nRet);
        return nRet;
    }
*/

    //获取Recoreding文件列表
    nRet = m_pCmiseMgr->GetCmiseFileList(CMISE_STATUS_RECORDING,
                                         file_msg_list);
/*
    if(nRet != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "采集对象(%s,TP=%s)取状态为RECORDING的文件列表失败(错误码=%d)",
               m_szSwitchName,
               m_TPInfo.szTPID,
               nRet);
        COTRACE("采集对象(%s,TP=%s)取状态为RECORDING的文件列表失败(错误码=%d)",
              m_szSwitchName,
              m_TPInfo.szTPID,
              nRet);
        return nRet;
    }
*/

    //获取满文件列表
    nRet = m_pCmiseMgr->GetCmiseFileList(CMISE_STATUS_READYTOBETRANSFERRED,
                                        file_msg_list);
/*
    if(nRet != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "采集对象(%s,TP=%s)取状态为READYTOBETRANSFERRED的文件列表失败(错误码=%d)",
               m_szSwitchName,
               m_TPInfo.szTPID,
               nRet);
        COTRACE("采集对象(%s,TP=%s)取状态为READYTOBETRANSFERRED的文件列表失败(错误码=%d)",
              m_szSwitchName,
              m_TPInfo.szTPID,
              nRet);
        return nRet;
    }
*/

    m_MutexForMap.acquire();
    int nFileCount = m_FileList.size();
    m_MutexForMap.release();

    //获取已释放文件列表
    if(nFileCount < 200)
    {
        nRet = m_pCmiseMgr->GetCmiseFileList(CMISE_STATUS_FREE,
                                             file_msg_list);
    }
/*
    if(nRet != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "采集对象(%s,TP=%s)取状态为RELEASED的文件列表失败(错误码=%d)",
               m_szSwitchName,
               m_TPInfo.szTPID,
               nRet);
        COTRACE("采集对象(%s,TP=%s)取状态为RELEASED的文件列表失败(错误码=%d)",
              m_szSwitchName,
              m_TPInfo.szTPID,
              nRet);
        return nRet;
    }
*/

    //获取正在传送文件
    nRet = m_pCmiseMgr->GetCmiseFileList(CMISE_STATUS_TRANSFERRING,
                                         file_msg_list);
/*
    if(nRet != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "采集对象(%s,TP=%s)取状态为TRANSFERRING的文件列表失败(错误码=%d)",
               m_szSwitchName,
               m_TPInfo.szTPID,
               nRet);
        COTRACE("采集对象(%s,TP=%s)取状态为TRANSFERRING的文件列表失败(错误码=%d)",
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
               "警告:采集对象(%s,TP=%s)保存状态文件(%s)失败",
               m_szSwitchName,
               m_TPInfo.szTPID,
               m_szStatusFile);
    }

    m_MutexForMap.release();

    return ERR_SUCCESS;
}

//在文件列表中查找指定文件，实际就是判断指定文件能否进行自动或手动采集
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

//判断指定文件能否进行自动采集
int CS1240TPCollect::CanAutoGet(const char* const szFileName)
{
    m_MutexForMap.acquire();
    if(m_FileList.find(szFileName) == m_FileList.end())
    {
        m_MutexForMap.release();
        return ERR_FAIL;
    }

    //下面判断是可以进行手动采集
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
            //已采集，只是状态未设置成功
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

//获得文件列表
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

//获得所有文件列表
int  CS1240TPCollect::GetAllList(MAP<STRING, SS1240FileStatus>& Out)
{
    m_MutexForMap.acquire();

    Out = m_FileList;

    m_MutexForMap.release();

    return ERR_SUCCESS;
}

//获得alarmlimit
int  CS1240TPCollect::GetFilePool(SS1240FilePoolMsg& pool_msg)
{
    return m_pCmiseMgr->GetFilePool(pool_msg);
}

//获得alarmlimit
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

//设置alarmlimit
int  CS1240TPCollect::SetAlarmLimit(int nAlarmLimit)
{

    int nRet = m_pCmiseMgr->SetFilePool(nAlarmLimit);
    if(nRet == 0)
    {
        m_nAlarmLimit = nAlarmLimit;
    }

    return nRet;
}

//动态创建子目录
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

//清除过期文件
BOOL CS1240TPCollect::ClearOldFile()
{
    int nRet = CCollectBase::ClearOldDateSubDir(m_TPInfo.szLocalPath,
                                                m_nFileSaveDays);

    return nRet == ERR_SUCCESS;
}

//检查文件序列号是否连续
BOOL CS1240TPCollect::CheckFileSN(const char* szFileName, char* szPreviousFile)
{
    if ((m_pFileSNMgr ==NULL)||(m_bCheckSN == 0))
    {
        return TRUE;
    }

    //TP-FILE-10000
    return m_pFileSNMgr->CheckFileSN(szFileName, szPreviousFile);
}

//文件管理器的回调函数，通过文件的序列号计算漏采文件名
STRING CS1240TPCollect::GetFileNameBySN(const int   nFileSN,
                                        const char* szReferName,
                                        const int   nSNLen,
                                        const int   nPostfixLen)
{

    //计算序列号
    int nPrefixLen = strlen(S1240_FILE_PREFIX);

    char szFileSNHigh[3];
    memcpy(szFileSNHigh, szReferName + nPrefixLen, 2);
    szFileSNHigh[2] = '\0';

    int nFileSNHigh = atoi(szFileSNHigh);

    //计算TP的A侧和B侧的高位序列号
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

    sprintf(szFileTPA, "A侧%s%02d%03d",
            S1240_FILE_PREFIX,
            nFileSNHighTPA,
            nFileSN);
    sprintf(szFileTPB, "B侧%s%02d%03d",
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

//指定的文件是否可以进行手工采集
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

//获取文件长度
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

