#include "../../../include/base_type.h"
#include "../global_fun.h"
#include "ewsd_ama_collect.h"


#include "../../../include/toolbox.h"
#include "../../include/adapter_include.h"
#include "../../include/res_config.h"
#include "../../collector_file_pool.h"
#include <assert.h>
#include <math.h>


extern CCollectorFilePool* GetFilePool();

CEWSD_AMACollect::CEWSD_AMACollect(CLinkMgr *pLinkMgr,
                               int nSwitchIndex,
                               int nAMAIndex)
{
    assert(pLinkMgr  != NULL);
    m_pLinkMgr        = pLinkMgr;


    m_nSwitchIndex    = nSwitchIndex;
    m_szSwitchName[0] = '\0';
    m_nAMAIndex       = nAMAIndex;
    m_bFileLenVerify  = FALSE;
}

CEWSD_AMACollect::~CEWSD_AMACollect()
{
    m_pLinkMgr = NULL;

    m_FileList.clear();

}

//初始化采集服务
int  CEWSD_AMACollect::Init()
{
    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        return ERR_FAIL;
    }

    /*
        相关Switch的信息
    */

    char szSecName[BUF_SIZE];
    SNPRINTF(szSecName,
             sizeof(szSecName),
             "%s%d",
             CFG_CO_SEC_SWITCH,
             m_nSwitchIndex);
    szSecName[BUF_SIZE - 1] = '\0';


    //读配置文件, 得到区号m_szAreaCode
    INIFile.GetString(szSecName,
                  CFG_5ESS_AREA_CODE,
                  CFG_5ESS_AREA_CODE_DEFAULT,
                  m_szAreaCode,
                  sizeof(m_szAreaCode));


    m_nFileSNLen = INIFile.GetInt(szSecName,
                              CFG_EWSD_AMA_FILE_SN_LEN,
                              CFG_EWSD_AMA_FILE_SN_LEN_DEFAULT);
    if(m_nFileSNLen < 0)
    {
        m_nFileSNLen = CFG_EWSD_AMA_FILE_SN_LEN_DEFAULT;
    }

    //是否进行文件长度校验
    m_bFileLenVerify = INIFile.GetInt(CFG_CO_SEC_SYSTEM,
                                      CFG_CO_FILE_LEN_VERIFY,
                                      CFG_CO_FILE_LEN_VERIFY_DEFAULT);
    m_bFileLenVerify = INIFile.GetInt(szSecName,
                                      CFG_CO_FILE_LEN_VERIFY,
                                      m_bFileLenVerify);

    /*
        相关AMA的信息
    */

    SNPRINTF(szSecName, sizeof(szSecName),
             "%s%d_%s%d", //"switch*_col_dir*"
             CFG_CO_SEC_SWITCH, m_nSwitchIndex,
             CFG_CO_SEC_AMA, m_nAMAIndex);
    szSecName[BUF_SIZE - 1] = '\0';

    //获取AMA文件对应的本地根目录名
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
    if(nLen >= 0)
    {
        if((szPathName[nLen] == '\\')
           || (szPathName[nLen] == '/'))
        {
            szPathName[nLen] = '\0';
        }
    }
    m_strRemotePath  = szPathName;

    //读配置文件, 得到AMA文件名
    char szAMAFileName[20];
    INIFile.GetString(szSecName,
                  CFG_EWSD_AMA_FILE,
                  CFG_EWSD_AMA_FILE_DEFAULT,
                  szAMAFileName,
                  sizeof(szAMAFileName));
    m_strAMAFile = szAMAFileName;
    m_FileList.clear();

    SFileInfo stFile;           //文件信息结构
    strncpy(stFile.szFileName,
            m_strAMAFile.c_str(),
            sizeof(stFile.szFileName));
    stFile.szFileName[sizeof(stFile.szFileName) - 1] = '\0';
    stFile.nCollectStatus = CS_NOT_COLLECTED;
    stFile.nFileSize      = 0;
    stFile.nAttr          = ATTR_FILE;
    m_FileList.push_back(stFile);

    m_strBillType = m_strAMAFile.substr(m_strAMAFile.length() - 3, 3);


    m_nFileSNLen = INIFile.GetInt(szSecName,
                              CFG_EWSD_AMA_FILE_SN_LEN,
                              m_nFileSNLen);
    if(m_nFileSNLen < 0)
    {
        m_nFileSNLen = CFG_EWSD_AMA_FILE_SN_LEN_DEFAULT;
    }

    m_nFileSNLimit = pow((float)10, m_nFileSNLen);

    //是否进行文件长度校验
    m_bFileLenVerify = INIFile.GetInt(szSecName,
                                      CFG_CO_FILE_LEN_VERIFY,
                                      m_bFileLenVerify);
    m_nFileSN = 0;

    //标识
    char szModuleLabel[100];
    INIFile.GetString(szSecName,
                      CFG_CO_MODULE_LABEL,
                      szSecName,
                      szModuleLabel,
                      sizeof(szModuleLabel));
    m_strModuleLabel = szModuleLabel;

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

    //文件序列号的状态文件
    SNPRINTF(m_szStatusFile,
             sizeof(m_szStatusFile),
             "%s/ama%d_local_sn_status.dat",
             szPathName,
             m_nAMAIndex);


    if(LoadStatus() != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
               "警告:采集对象(%s)读状态文件(%s)失败",
               m_szSwitchName,
               m_szStatusFile);
    }

    return ERR_SUCCESS;  //程序返回

}

STRING CEWSD_AMACollect::GetDstFileName(const char* const szSrcFile)
{
    char szDstName[MAX_PATH];

    char szFormat[20];
    sprintf(szFormat, "%%s_%%s%%0%dd.%%s", m_nFileSNLen);

    //本地文件名格式：区号_HHSS1234.话单类型
    sprintf(szDstName,
            szFormat,
            m_szAreaCode,
            Get_Hour_Min().c_str(),
            m_nFileSN,
            m_strBillType.c_str());

    return szDstName;
}

//拷贝文件服务端的单个文件到本地, 由派生类具体实现;
int CEWSD_AMACollect::GetFile(SColFileReq& stReq)
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
int CEWSD_AMACollect::DoGetAFile(SGetFileReq& stReq)
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
    SFileInfo& file_info = m_FileList[nFileListIndex];

    int  nRet = 0;
    if(m_pLinkMgr == NULL)
    {
        //aTRACE("No link available: failed to get filelist");
        COTRACE("采集对象(%s)取文件失败:无可用的链路",
              m_szSwitchName);
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "采集对象(%s)取文件失败:无可用的链路",
               m_szSwitchName);
        return ERR_FAIL;
    }

    //设置临时文件名
    string  strSrcFile = m_strTmpPath;
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

    m_MutexForList.acquire();
    file_info.nCollectStatus = CS_COLLECTING;
    m_MutexForList.release();

    strncpy(stReq.szDstPath, m_strTmpPath.c_str(), sizeof(stReq.szDstPath));
    stReq.szDstPath[sizeof(stReq.szDstPath) - 1] = '\0';
    //取文件
    nRet = m_pLinkMgr->GetFile(stReq);//可优化为传入全路径参数

    //记录操作日志
    char szTraceMsg[1024];
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
                 stReq.szDstName);
        szTraceMsg[sizeof(szTraceMsg) - 1] = '\0';
        COTRACE(szTraceMsg);
        COMSGOUT(MSGOUT_LEVEL_URGENT, szTraceMsg);
    }

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

        remove(strSrcFile.c_str());
        remove(strDstFile.c_str());

        m_MutexForList.acquire();
        file_info.nCollectStatus = CS_NOT_COLLECTED;
        m_MutexForList.release();
    }
    else
    {
        //added by chenliangwei
        strncpy(stReq.szDstName, strDstFile.c_str(), sizeof(stReq.szDstName));

        //将文件信息加到转换文件池中
        SSwitchFileInfo stFileInfo;
        stFileInfo.nSwitchIndex = m_nSwitchIndex;
        strncpy(stFileInfo.szModuleLabel,
                m_strModuleLabel.c_str(),
                sizeof(stFileInfo.szModuleLabel));
        strncpy(stFileInfo.szRemoteFile,
                stReq.szSrcName,
                sizeof(stFileInfo.szRemoteFile));
        strncpy(stFileInfo.szLocalFile,
                stReq.szDstName,
                sizeof(stFileInfo.szLocalFile));
        stFileInfo.nConvertInfo = m_nAMAIndex;

        GetFilePool()->PushBack(stFileInfo);
        //added end

        //added end

        IncreaseFileSN();

        m_MutexForList.acquire();
        file_info.nCollectStatus = CS_COLLECTED;
        m_MutexForList.release();
    }

    return  nRet;


}


//在文件列表中查找指定文件
int CEWSD_AMACollect::FindFileInList(const char* const szFileName)
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
int CEWSD_AMACollect::GetList(SListOut& pOut)
{
    int nFileListIndex = -1;
    for(int i = 0; i < m_FileList.size(); i++)
    {
        SFileInfo& file_info = m_FileList[i];
        if(m_strAMAFile == file_info.szFileName)
        {
            nFileListIndex = i;
            break;
        }
    }
    if(nFileListIndex == -1)
    {
        return ERR_FAIL;
    }
    SFileInfo& file_info = m_FileList[nFileListIndex];

    unsigned long uFileLen = 0;
    if((m_pLinkMgr->GetFileLen(file_info.szFileName, uFileLen) != 0)
       || uFileLen <= 0)
    {
        return ERR_FAIL;
    }
    file_info.nFileSize      = uFileLen;
    file_info.nCollectStatus = CS_NOT_COLLECTED;

    pOut.FileList.push_back(file_info);

    return ERR_SUCCESS;
}

//动态创建子目录
BOOL CEWSD_AMACollect::CreateLocSubDir()
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
BOOL CEWSD_AMACollect::ClearOldFile()
{
    //added by chenliangwei
    int nRet = CCollectBase::ClearOldDateSubDir(m_strLocalRootPath.c_str(),
                                  m_nFileSaveDays);

    return nRet == ERR_SUCCESS;
    //added end
}

//加载状态文件
int CEWSD_AMACollect::LoadStatus()
{
    m_pStatusFile = fopen(m_szStatusFile, "rb+");
    if(NULL == m_pStatusFile)
    {
        //创建新文件
        m_pStatusFile = fopen(m_szStatusFile, "wb+");

        if (NULL == m_pStatusFile)
        {
            COTRACE("采集对象(%s)打开/创建状态文件(%s)失败",
              m_szSwitchName,
              m_szStatusFile);
            return ERR_FAIL;
        }
        else
        {
            m_nFileSN = 0;
            fclose(m_pStatusFile);
            m_pStatusFile = NULL;

            return ERR_SUCCESS;
        }
    }

    int nFileLength = FileSize(m_pStatusFile);
    if(nFileLength < sizeof(UINT4))
    {
        m_nFileSN = 0;
        fclose(m_pStatusFile);
        m_pStatusFile = NULL;

        return ERR_SUCCESS;
    }

    int nRet = fread(&m_nFileSN,
                 sizeof(m_nFileSN),
                 1,
                 m_pStatusFile);

    if(1 != nRet)
    {
        fclose(m_pStatusFile);
        m_pStatusFile = NULL;

        return ERR_FAIL;
    }


    fclose(m_pStatusFile);
    m_pStatusFile = NULL;

    return ERR_SUCCESS;
}

//保存状态文件
int CEWSD_AMACollect::SaveStatus()
{
    m_pStatusFile = fopen(m_szStatusFile, "wb");
    if(NULL == m_pStatusFile)
    {
        COTRACE("采集对象(%s)打开/创建状态文件(%s)失败",
              m_szSwitchName,
              m_szStatusFile);
        return ERR_FAIL;

    }

    int nRet = 0;

    nRet = fwrite(&m_nFileSN,
                  sizeof(m_nFileSN),
                  1,
                  m_pStatusFile);

    if(1 != nRet)
    {
        fclose(m_pStatusFile);
        m_pStatusFile = NULL;

        return ERR_FAIL;
    }


    nRet = fflush(m_pStatusFile);
    fclose(m_pStatusFile);
    m_pStatusFile = NULL;

    ACE_OS::truncate(m_szStatusFile, sizeof(m_nFileSN));

    return nRet;
}

int CEWSD_AMACollect::IncreaseFileSN()
{
    m_nFileSN = (m_nFileSN + 1) %  m_nFileSNLimit;

    return SaveStatus();
}

BOOL CEWSD_AMACollect::CheckFileSN(const char* szFileName, char* szPreviousFile)
{
    return TRUE;
}

//指定的文件是否可采
int CEWSD_AMACollect::CanGetSpecFile(const char* const szSrcFile)
{
    m_MutexForList.acquire();
    int nRet = FindFileInList(szSrcFile);
    m_MutexForList.release();

    return nRet;
}

//采集指定的文件
int CEWSD_AMACollect::GetSpecFile(const char* const szSrcFile,
                                  const char* const szDstFile)
{
    return ERR_FAIL;
}

//获取模块信息
int CEWSD_AMACollect::GetModuleInfo(LIST<STRING>& label,
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
int CEWSD_AMACollect::GetFileLen(const char* const szSrcFile)
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

int CEWSD_AMACollect::MakeFileNameMsg(const char* const szSrcFile,
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


