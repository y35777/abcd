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
        //创建新文件
        m_pStatusFile = fopen(m_szStatusFile, "wb+");

        if (NULL == m_pStatusFile)
        {
            COTRACE("采集对象(%s, DIR=%d)打开/创建状态文件(%s)失败", 
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
        COTRACE("采集对象(%s, DIR=%d)打开状态文件(%s)失败", 
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


//初始化采集服务
int  CComDirCollect::Init()
{
    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        return ERR_FAIL;
    }

    /*
        相关Switch的信息
    */

    char szSecName[COM_CFG_LEN];
    SNPRINTF(szSecName,
             sizeof(szSecName),
             "%s%d",
             CFG_CO_SEC_SWITCH,
             m_nSwitchIndex);
    szSecName[COM_CFG_LEN - 1] = '\0';

    //得到采集类型
    INIFile.GetString(szSecName, CFG_CO_TYPE,
                      CFG_CO_TYPE_DEFAULT,
                      m_szCollectId,
                      sizeof(m_szCollectId));

    //获得switch下的传输模式
    char szTransMode[TRANS_MODE_LEN];
    INIFile.GetString(szSecName, CFG_CO_COL_TRANS_MODE,
                      CFG_CO_COL_TRANS_MODE_DEFAULT,
                      szTransMode,
                      sizeof(szTransMode));
//能重复采集同名文件
	m_nForceColAllFile = INIFile.GetInt(szSecName,
							CFG_CO_FORCE_COL_ALL_FILE,
							CFG_CO_FORCE_COL_ALL_FILE_DEFAULT);

	if (m_nForceColAllFile<0) 
	{
		m_nForceColAllFile = CFG_CO_FORCE_COL_ALL_FILE_DEFAULT;
	}

    //得到检查文件序列号信息
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

    //是否进行文件长度校验
    m_bFileLenVerify = INIFile.GetInt(CFG_CO_SEC_SYSTEM,
                                      CFG_CO_FILE_LEN_VERIFY,
                                      CFG_CO_FILE_LEN_VERIFY_DEFAULT);
    m_bFileLenVerify = INIFile.GetInt(szSecName,
                                      CFG_CO_FILE_LEN_VERIFY,
                                      m_bFileLenVerify);


    //是否在采集后删除交换机文件
    m_bDelRemoteFile = INIFile.GetInt(CFG_CO_SEC_SYSTEM,
                                      CFG_CO_DEL_REMOTE_FILE,
                                      CFG_CO_DEL_REMOTE_FILE_DEFAULT);
    m_bDelRemoteFile = INIFile.GetInt(szSecName,
                                      CFG_CO_DEL_REMOTE_FILE,
                                      m_bDelRemoteFile);

    //added by chenliangwei, 2003-05-16
    //是否采集交换机上的最后一个文件
    m_bCollectLastFile = INIFile.GetInt(CFG_CO_SEC_SYSTEM,
                                      CFG_CO_COL_LAST_FILE,
                                      CFG_CO_COL_LAST_FILE_DEFAULT);
    m_bCollectLastFile = INIFile.GetInt(szSecName,
                                      CFG_CO_COL_LAST_FILE,
                                      m_bCollectLastFile);
    //added end

    //交换机上文件后缀名
    char szFilePostfix[1024];
    INIFile.GetString(szSecName, CFG_CO_FILE_POSTFIX,
                      CFG_CO_FILE_POSTFIX_DEFAULT,
                      szFilePostfix,
                      sizeof(szFilePostfix));
    //后缀名过滤模式
    m_nPostfixFltMode = INIFile.GetInt(szSecName,
                                      CFG_CO_POSTFIX_FILTER_MODE,
                                      CFG_CO_POSTFIX_FILTER_MODE_DEFAULT);

    //交换机上文件前缀名
    char szFilePrefix[1024];
    INIFile.GetString(szSecName, CFG_CO_FILE_PREFIX,
                      CFG_CO_FILE_PREFIX_DEFAULT,
                      szFilePrefix,
                      sizeof(szFilePrefix));
    //前缀名过滤模式
    m_nPrefixFltMode = INIFile.GetInt(szSecName,
                                      CFG_CO_PREFIX_FILTER_MODE,
                                      CFG_CO_PREFIX_FILTER_MODE_DEFAULT);


    /*
        相关DirCollect的信息
    */

    SNPRINTF(szSecName, sizeof(szSecName),
             "%s%d_%s%d", //"switch*_col_dir*"
             CFG_CO_SEC_SWITCH, m_nSwitchIndex,
             CFG_CO_SEC_COL_DIR, m_nDirIndex);
    szSecName[COM_CFG_LEN - 1] = '\0';

    //标识
    char szModuleLabel[100];
    INIFile.GetString(szSecName,
                      CFG_CO_MODULE_LABEL,
                      szSecName,
                      szModuleLabel,
                      sizeof(szModuleLabel));
    m_strModuleLabel = szModuleLabel;

    //获得switch*_col_dir*下的传输模式
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
        COTRACE("采集对象(%s)检测到无效的传输类型(%s)",
              m_szSwitchName,
              szTransMode);
        return ERR_FAIL;
    }
    //重复采集
	m_nForceColAllFile = INIFile.GetInt(szSecName,
							CFG_CO_FORCE_COL_ALL_FILE,
							m_nForceColAllFile);

	if (m_nForceColAllFile<0) 
	{
		m_nForceColAllFile = CFG_CO_FORCE_COL_ALL_FILE_DEFAULT;
	}	
	
    //获取交换机对应的本地根目录名
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

    //创建本地话单存放主目录
    if (!CreateDeepDir(m_strLocalRootPath.c_str()))
    {
        COTRACE("采集对象(%s)原始话单目录(%s)无法创建成功",
            m_szSwitchName,
            m_strLocalRootPath.c_str());

        return ERR_FAIL;
    }

    //创建话单存放临时目录
    m_strTmpPath = szPathName;
    m_strTmpPath += "/temp";
    CreateDeepDir(m_strTmpPath.c_str());

    //得到远端服务器上的采集源目录m_strRemotePath
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

    //交换机上文件后缀名
    INIFile.GetString(szSecName, CFG_CO_FILE_POSTFIX,
                      szFilePostfix,
                      szFilePostfix,
                      sizeof(szFilePostfix));

    //后缀名过滤模式
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


    //交换机上文件前缀名
    INIFile.GetString(szSecName, CFG_CO_FILE_PREFIX,
                      szFilePrefix,
                      szFilePrefix,
                      sizeof(szFilePrefix));

    //前缀名过滤模式
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

    //得到检查文件序列号信息
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

    //获得状态文件的路径
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
*/

    //采集目录对象的状态文件名
    SNPRINTF(m_szStatusFile,
             sizeof(m_szStatusFile),
             "%s/dir%d_status.dat",
             szPathName,
             m_nDirIndex);

    if(LoadStatus() != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
               "警告:采集对象(%s,DIR=%d)读状态文件(%s)失败", 
               m_szSwitchName,
               m_nDirIndex,
               m_szStatusFile);
    }

    char szFileSNStatusFile[MAX_PATH];

    //文件序列号的状态文件
    SNPRINTF(szFileSNStatusFile,
             sizeof(szFileSNStatusFile),
             "%s/dir%d_file_sn_status.dat",
             szPathName,
             m_nDirIndex);


    char szMgrInfo[BUF_SIZE];
    SNPRINTF(szMgrInfo,
        sizeof(szMgrInfo),
        "采集对象(%s,DIR=%d)文件序列号管理器",
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

//拷贝文件服务端的单个文件到本地, 由派生类具体实现;
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

//拷贝文件服务端的单个文件到本地;
int CComDirCollect::DoGetAFile(SGetFileReq& stReq)
{

    if(stReq.szSrcName[0] == '\0')
    {
        COTRACE("采集文件名不能为空：输入参数错误.");
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
               "采集对象(%s)采集文件(%s)失败:文件不存在",
               m_szSwitchName,
               stReq.szSrcName);
        return ERR_FAIL;
    }

    int nRet = ERR_SUCCESS;

    //设置传输模式
    char szTraceMsg[1024];

    nRet = m_pLinkMgr->SetMode(m_nMode);
    if (nRet != ERR_SUCCESS)
    {
        SNPRINTF(szTraceMsg,
                 sizeof(szTraceMsg),
                 //"Collector(ID=%d) failed to collect file:%s to %s",
                 "采集对象(%s,DIRID=%d)设置传输模式失败",
                 m_szSwitchName,
                 m_nDirIndex);
        szTraceMsg[sizeof(szTraceMsg) - 1] = '\0';
        COTRACE(szTraceMsg);
        COMSGOUT(MSGOUT_LEVEL_URGENT, szTraceMsg);

        return nRet;
    }

    //设置临时文件名
    string strSrcFile = m_strTmpPath;
    strSrcFile += "/";
    strSrcFile += stReq.szDstName;

    //设置正式文件名
    string  strDstFile = stReq.szDstPath;
    if((stReq.szDstPath[0] != 0) &&
       ((stReq.szDstPath[strlen(stReq.szDstPath) - 1] != '\\') &&
        (stReq.szDstPath[strlen(stReq.szDstPath) - 1] != '/')))
    {
        strDstFile += "/";
    }
    strDstFile += stReq.szDstName;

    //获得原路径名
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
               "警告:采集对象(%s,DIR=%d)保存状态文件(%s)失败", 
               m_szSwitchName,
               m_nDirIndex,
               m_szStatusFile);

        COTRACE("警告:采集对象(%s,DIR=%d)保存状态文件(%s)失败", 
               m_szSwitchName,
               m_nDirIndex,
               m_szStatusFile);

    }
    m_MutexForList.release();

    //取文件
    nRet = m_pLinkMgr->GetFile(stReq);//可优化为传入全路径参数

    //记录操作日志
    if( nRet == ERR_SUCCESS )
    {
        //进行文件更名
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
                         "采集对象(%s)采集的文件(%s)长度不一致(远端:%d, 本端:%d)",
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
            //移动临时文件为正式文件
            BOOL bRet = FileCopy(strSrcFile.c_str(), strDstFile.c_str() );
            if(!bRet)
            {
                SNPRINTF(szTraceMsg,
                         sizeof(szTraceMsg),
                         //"Collector(ID=%d) failed to move file:%s to %s",
                         "采集对象(%s)移动文件失败:%s to %s",
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
                                 "采集对象(%s)删除文件失败:%s",
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
                 "采集对象(%s)采集文件失败:%s to %s",
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
                   "警告:采集对象(%s,DIR=%d)保存状态文件(%s)失败", 
                   m_szSwitchName,
                   m_nDirIndex,
                   m_szStatusFile);

            COTRACE("警告:采集对象(%s,DIR=%d)保存状态文件(%s)失败", 
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
                   "警告:采集对象(%s,DIR=%d)保存状态文件(%s)失败", 
                   m_szSwitchName,
                   m_nDirIndex,
                   m_szStatusFile);

            COTRACE("警告:采集对象(%s,DIR=%d)保存状态文件(%s)失败", 
                   m_szSwitchName,
                   m_nDirIndex,
                   m_szStatusFile);

        }

        m_MutexForList.release();


        //将文件信息加到转换文件池中
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


//在文件列表中查找指定文件
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

//获得文件列表
int CComDirCollect::GetList(SListOut& pOut)
{
    SListOut file_list_new;
    int nRet = m_pLinkMgr->GetFileList(m_strRemotePath.c_str(), file_list_new);

    if(nRet != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "采集任务(%s,DIRID=%d)获取文件列表失败(错误码=%d)",
               m_szSwitchName,
               m_nDirIndex,
               nRet);
        COTRACE("采集任务(%s,DIRID=%d)获取文件列表失败(错误码=%d)",
               m_szSwitchName,
               m_nDirIndex,
               nRet);
        return nRet;
    }

    //按文件名从小到大排序
    std::sort(file_list_new.FileList.begin(),
              file_list_new.FileList.end(),
              Compare());

    VECTOR<SFileInfo>::iterator i;
    
    //后缀过滤
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

            //模式不为0, 过滤掉不符合后缀名的文件
            if (m_nPostfixFltMode != 0)
            {
                if (!bFind) 
                {
                    i = file_list_new.FileList.erase(i);
                    continue;
                }
            }
            else   //模式为0, 过滤掉符合后缀名的文件
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

    //前缀过滤
    if (m_FilePrefixList.size() > 0)
    {
        STRING strPath;
        STRING strFile;

        i = file_list_new.FileList.begin();

        while(i != file_list_new.FileList.end())
        {
            //获得文件名
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

            //模式不为0, 过滤掉不符合前缀名的文件
            if (m_nPrefixFltMode != 0)
            {
                if (!bFind) 
                {
                    i = file_list_new.FileList.erase(i);
                    continue;
                }
            }
            else   //模式为0, 过滤掉符合前缀名的文件
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

    //提取出已经采集的文件
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

    //更新文件列表和要采集的文件列表
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

    //modify by chenliangwei, 2003-05-16, 只有当文件大于0时才去掉
    if ((!m_bCollectLastFile) && (pOut.FileList.size() > 0))
    {
        pOut.FileList.pop_back();
    }

    if(SaveStatus() != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "警告:采集对象(%s,DIR=%d)保存状态文件(%s)失败", 
               m_szSwitchName,
               m_nDirIndex,
               m_szStatusFile);

        COTRACE("警告:采集对象(%s,DIR=%d)保存状态文件(%s)失败", 
               m_szSwitchName,
               m_nDirIndex,
               m_szStatusFile);

    }

    m_MutexForList.release();

    return ERR_SUCCESS;
}

//动态创建子目录
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

//清除过期文件
BOOL CComDirCollect::ClearOldFile()
{
    //added by chenliangwei
    int nRet = CCollectBase::ClearOldDateSubDir(m_strLocalRootPath.c_str(),
                                  m_nFileSaveDays);

    return nRet == ERR_SUCCESS;
    //added end
}

//检查文件序列号是否连续
BOOL CComDirCollect::CheckFileSN(const char* szFileName, char* szPreviousFile)
{
    if ((m_pFileSNMgr ==NULL)||(m_bCheckSN == 0))
    {
        return TRUE;
    }

    return m_pFileSNMgr->CheckFileSN(szFileName, szPreviousFile);

}

/***********************************************
函数判断指定的文件是否可手工采集
以下文件不能手工采集
//1.目录列表之外的文件不允许采集
//2.目录列表之内的文件，但已经被采集后删除的文件
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

            //如果是要删除远端文件，并文件已被采集则不能采集
            if ((m_bDelRemoteFile) && (CS_COLLECTED == fi.nCollectStatus))
            {
                m_MutexForList.release();
                //表示话单已被删除，不能再采
                return ERR_FILE_BE_DELETED;
            }
            else if(CS_COLLECTING == fi.nCollectStatus)
            {
                m_MutexForList.release();
                //话单文件正在被采集，不能再采
                return ERR_FILE_BEING_COLLECTED;
            }
            else if(!m_bCollectLastFile)
            {
                BOOL bIsNew = TRUE;
                for(int j = 0; j < m_FileList.size(); j++)
                {
                    //如果时间不是最新，则跳出循环
                    if(strcmp(m_FileList[j].szFileName, fi.szFileName) > 0)
                    {
                        bIsNew = FALSE;
                        break;
                    }
                }

                if(bIsNew)
                {
                    m_MutexForList.release();
                    //表示系统不允许采集最新的可能未关闭的话单文件
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
               "采集对象(%s)手动采集文件(%s)失败:文件不存在",
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

//获取模块信息
int CComDirCollect::GetModuleInfo(LIST<STRING>& label,
                                  LIST<STRING>& content)
{
    label.push_back("文件名");
    label.push_back("文件长度");
    label.push_back("采集状态");

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

//获取文件长度
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
        //COTRACE("采集文件名不能为空：输入参数错误.");
        COTRACE("Remote file name is not specified: failed to get file");
        return ERR_FAIL;
    }

    SGetFileReq stGetReq;
    memset(&stGetReq,0, sizeof(SGetFileReq));

    //5ESS上的源文件路径
    strncpy(stGetReq.szSrcPath,
            m_strRemotePath.c_str(),
            sizeof(stGetReq.szSrcPath));

    //源文件名
    strncpy(stGetReq.szSrcName, szSrcFile, sizeof(stGetReq.szSrcName));

    //当前目的话单文件路径
    strcpy(stGetReq.szDstPath, m_strLocalPath.c_str());

    //szBuf存放新的目的文件名
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
