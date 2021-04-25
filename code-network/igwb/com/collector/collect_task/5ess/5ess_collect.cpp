// 5ess_collect.cpp: implementation of the C5ESSCollect class.
//
//////////////////////////////////////////////////////////////////////
#include "../../../include/base_type.h"
#include "../../include/i_ftam.h"
#include "../../include/i_cmise_fun.h"
#include "../vertel_ftam.h"
#include "5ess_collect.h"
#include "../bk_link_mgr.h"
#include "../global_fun.h"

#include <algorithm>
#include <math.h>
#include "../file_sn_manager.h"

#include "../../../include/toolbox.h"
#include "../../include/adapter_include.h"
#include "../../include/res_config.h"
#include "../../collector_file_pool.h"
#include "../../resource.h"
#include <assert.h>

extern CCollectorFilePool* GetFilePool();
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
C5ESSCollect::C5ESSCollect(int nSwitchIndex, CLinkGroup* pLinkGroup)
                          :CCollectBase(nSwitchIndex,pLinkGroup)
{
    m_pFileSNMgr = NULL;
}

C5ESSCollect::~C5ESSCollect()
{
    //多种话单存放的详细目录名
    m_vBillSavePath.clear();
    //文件前缀的数组
    m_vFilePrefix.clear();

    if (m_pFileSNMgr != NULL)
    {
        delete m_pFileSNMgr;
        m_pFileSNMgr = NULL;
    }

}
//初始化采集服务, 由派生类具体实现
int  C5ESSCollect::Col_Init()
{
    int  nRet = 0;
    char szBuf[BUF_SIZE] = "";
    char szPathName[MAX_PATH];

    CINIFile ini(m_szCfgPath);
    ini.Open();
    SNPRINTF(szBuf,BUF_SIZE,"%s%d", CFG_CO_SEC_SWITCH, m_nSwitchIndex);

    //获取采集任务名
    ini.GetString(szBuf,
                  CFG_CO_NAME,
                  CFG_CO_NAME_DEFAULT,
                  m_szSwitchName,
                  sizeof(m_szSwitchName));

    // 读取配置文件:m_strLocalRootPath 本地话单存放主目录
    ini.GetString(szBuf,
                  CFG_CO_LOCAL_DIR,
                  CCollectorAdapter::GetFrontSavePath(),
                  szPathName,
                  MAX_PATH);
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

    //得到远端5ESS服务器上的采集源目录m_strRemotePath
    ini.GetString(szBuf,
                  CFG_CO_REMOTE_DIR,
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

    char  szAreaCode[20];  //区号
    char  szBillType[20];  //话单类型
    //读配置文件, 得到区号m_szAreaCode
    ini.GetString(szBuf,
                  CFG_5ESS_AREA_CODE,
                  CFG_5ESS_AREA_CODE_DEFAULT,
                  szAreaCode,
                  sizeof(szAreaCode));

    //读配置文件,话单类型m_szBillType
    ini.GetString(szBuf,
                  CFG_5ESS_BILL_TYPE,
                  CFG_5ESS_BILL_TYPE_DEFAULT,
                  szBillType,
                  sizeof(szBillType));

    //读配置文件,本地话单文件后缀m_szBillType
    //文件后缀名暂不实现
    //added by chenliangwei
    //读话单文件的保存天数
    m_nFileSaveDays = ini.GetInt(CFG_CO_SEC_SYSTEM,
                                 CFG_CO_ORIG_FILE_SAVE_DAYS,
                                 CFG_CO_ORIG_FILE_SAVE_DAYS_DEFAULT);
    m_nFileSaveDays = ini.GetInt(szBuf,
                                 CFG_CO_ORIG_FILE_SAVE_DAYS,
                                 m_nFileSaveDays);
    if(m_nFileSaveDays < 0)
    {
        m_nFileSaveDays = CFG_CO_ORIG_FILE_SAVE_DAYS_DEFAULT;
    }

    m_bCheckSN = ini.GetInt(szBuf,
                            CFG_CO_CHECK_SN,
                            CFG_CO_CHECK_SN_DEFAULT);

    //是否进行文件长度校验
    m_bFileLenVerify = ini.GetInt(CFG_CO_SEC_SYSTEM,
                                  CFG_CO_FILE_LEN_VERIFY,
                                  CFG_CO_FILE_LEN_VERIFY_DEFAULT);
    m_bFileLenVerify = ini.GetInt(szBuf,
                                  CFG_CO_FILE_LEN_VERIFY,
                                  m_bFileLenVerify);

    int nCheckSNDepth = ini.GetInt(CFG_CO_SEC_SYSTEM,
                                 CFG_CO_CHECK_SN_DEPTH,
                                 CFG_CO_CHECK_SN_DEPTH_DEFAULT);
    nCheckSNDepth = ini.GetInt(szBuf,
                                 CFG_CO_CHECK_SN_DEPTH,
                                 nCheckSNDepth);
    if(nCheckSNDepth < 0)
    {
        nCheckSNDepth = CFG_CO_CHECK_SN_DEPTH_DEFAULT;
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
    SNPRINTF(szPathName,
             sizeof(szPathName),
             "%s/file_sn_status.dat",
             szPathName);

    char szMgrInfo[BUF_SIZE];
    SNPRINTF(szMgrInfo,
        sizeof(szMgrInfo),
        S_COL_FILE_SN_MGR,
        m_szSwitchName);
    szMgrInfo[BUF_SIZE - 1] = '\0';

    m_pFileSNMgr = new CFileSNMgr(szPathName,
                                  nCheckSNDepth,
                                  LEN_OF_AMA_FILE_SN,
                                  0,
                                  GetFileNameBySN);

    m_pFileSNMgr->SetMgrInfo(szMgrInfo);

    m_pFileSNMgr->SetSNLimit(pow((float)10, LEN_OF_AMA_FILE_SN), 1);

    if (ERR_SUCCESS != m_pFileSNMgr->Init())
    {
        delete m_pFileSNMgr;
        m_pFileSNMgr = NULL;
    }

    //added end

    //读文件前缀个数
    int nPrefixNum = ini.GetInt(szBuf,
                                CFG_5ESS_PREFIX_NUM,
                                CFG_5ESS_PREFIX_NUM_DEFAULT);
    //读出文件前缀并初始化路径名，FileStore数组
    char szTmpKey[BUF_SIZE];
    char szPrefixName[50];
    char szLabel[50];
    char szPriLink[50];
    char szBakLink[50];

    STRING strBillPath;
    for(int i = 1; i <= nPrefixNum; i++)
    {
        SNPRINTF(szTmpKey,
                 sizeof(szTmpKey),
                 CFG_5ESS_SEC_PREFIX,
                 m_nSwitchIndex,
                 i);
        ini.GetString(szTmpKey,
                      CFG_5ESS_PREFIX_NAME,
                      CFG_5ESS_PREFIX_NAME_DEFAULT,
                      szPrefixName,
                      sizeof(szPrefixName));

        //标识
        ini.GetString(szTmpKey,
                      CFG_CO_MODULE_LABEL,
                      szTmpKey,
                      szLabel,
                      sizeof(szLabel));

        //读配置文件, 得到区号m_szAreaCode
        char szTempAreaCode[20];
        ini.GetString(szTmpKey,
                      CFG_5ESS_AREA_CODE,
                      szAreaCode,
                      szTempAreaCode,
                      sizeof(szTempAreaCode));

        //读配置文件,话单类型m_szBillType
        char szTempBillType[20];
        ini.GetString(szTmpKey,
                      CFG_5ESS_BILL_TYPE,
                      szBillType,
                      szTempBillType,
                      sizeof(szTempBillType));

        ini.GetString(szTmpKey,
                      CFG_CO_PRI_LINK,
                      CFG_CO_PRI_LINK_DEFAULT,
                      szPriLink,
                      sizeof(szPriLink));
        if((szPriLink[0] == '\0') || (NULL == m_pLinkGroup->GetLink(szPriLink)))
        {
            COTRACE(S_COL_MUST_CFG_PRI_LINK, szPrefixName);
            return ERR_FAIL;
        }

        ini.GetString(szTmpKey,
                      CFG_CO_BAK_LINK,
                      CFG_CO_BAK_LINK_DEFAULT,
                      szBakLink,
                      sizeof(szBakLink));

        m_vFilePrefix.push_back(STRING(szPrefixName));
        m_vAreaCode.push_back(szTempAreaCode);
        m_vBillType.push_back(szTempBillType);
        m_vLabel.push_back(szLabel);

        CLinkMgr* pLinkMgr = new CBKLinkMgr(m_pLinkGroup);
        pLinkMgr->RegisterLink(szPriLink);
        if((szBakLink[0] != '\0') && (NULL != m_pLinkGroup->GetLink(szBakLink)))
        {
            pLinkMgr->RegisterLink(szBakLink);
        }
        m_LinkMgrMap[STRING(szPrefixName)] = pLinkMgr;
    }

    SNPRINTF(szTmpKey,BUF_SIZE,"%s%d_5ess_fna",CFG_CO_SEC_SWITCH,m_nSwitchIndex);
    m_strFnaIndex = szTmpKey;
    ini.GetString(szTmpKey,
                  CFG_5ESS_FNA_NAME,
                  CFG_5ESS_FNA_NAME_DEFAULT,
                  szBuf,
                  MAX_PATH);
    m_strFnaFile = szBuf;

    //创建本地指示文件存放目录
    ini.GetString(szTmpKey,CFG_5ESS_SAVE_FNA_DIR,
                  CFG_5ESS_SAVE_FNA_DIR_DEFAULT,
                  szPathName,
                  MAX_PATH);
    m_strFNARootPath = szPathName;
    CreateDeepDir(m_strFNARootPath.c_str());

    ini.GetString(szTmpKey,
                  CFG_CO_PRI_LINK,
                  CFG_CO_PRI_LINK_DEFAULT,
                  szPriLink,
                  sizeof(szPriLink));
    if((szPriLink[0] == '\0') || (NULL == m_pLinkGroup->GetLink(szPriLink)))
    {
        COTRACE(S_COL_MUST_CFG_PRI_LINK, "FNA");
        return ERR_FAIL;
    }

    ini.GetString(szTmpKey,
                  CFG_CO_BAK_LINK,
                  CFG_CO_BAK_LINK_DEFAULT,
                  szBakLink,
                  sizeof(szBakLink));

    CLinkMgr* pLinkMgr = new CBKLinkMgr(m_pLinkGroup);
    pLinkMgr->RegisterLink(szPriLink);
    if((szBakLink[0] != '\0') && (NULL != m_pLinkGroup->GetLink(szBakLink)))
    {
        pLinkMgr->RegisterLink(szBakLink);
    }
    m_LinkMgrMap[m_strFnaIndex] = pLinkMgr;

    return nRet;  //程序返回
}

/*
    机制是先在非空前缀中匹配，如果不成功，则属于空前缀
    即是说配置为空的前缀是其它非空前缀的补集
*/
int C5ESSCollect::GetPrefixIndex(const char* const szSrcFile)
{
    int nIndex = -1;
    int i;
    for(i = 0; i < m_vFilePrefix.size(); i++)
    {
        if((m_vFilePrefix[i] != "")
           && strstr(szSrcFile, m_vFilePrefix[i].c_str()) != NULL)
        {
            nIndex = i;
            break;
        }
    }

    if(nIndex == -1)
    {
        for(i = 0; i < m_vFilePrefix.size(); i++)
        {
            if(m_vFilePrefix[i] == "")
            {
                nIndex = i;
                break;
            }
        }
    }

    return nIndex;
}

int C5ESSCollect::GetFileListIndex(const char* const szSrcFile)
{
    m_MutexForList.acquire();

    int nFileListIndex = -1;
    for(int i = 0; i < m_FileList.size(); i++)
    {
        SFileInfo& fi = m_FileList[i];
        if(StringiCmp(fi.szFileName, szSrcFile))
        {
            nFileListIndex = i;
            break;
        }
    }

    m_MutexForList.release();

    return nFileListIndex;
}

STRING C5ESSCollect::GetDstFileName(const char* const szSrcFile)
{
    char szDstName[MAX_PATH];

    strncpy(szDstName, szSrcFile, sizeof(szDstName));
    szDstName[sizeof(szDstName) - 1] = '\0';

    //本地文件名格式：原文件名_HHSS.话单类型
    int nIndex = GetPrefixIndex(szSrcFile);
    if(nIndex != -1)
    {
        sprintf(szDstName,
                "%s_%s.%s",
                szSrcFile,
                Get_Hour_Min().c_str(),
                m_vBillType[nIndex].c_str());
    }

    return szDstName;
}

int C5ESSCollect::MakeFileNameMsg(const char* const szSrcFile,
                                  SGetFileReq& req)
{
    if(szSrcFile[0] == '\0')
    {
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

//拷贝文件服务端的单个文件到本地, 由派生类具体实现;
int C5ESSCollect::Col_GetFile(SColFileReq& stReq)
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

        nRet = GetFile(file_req);
        stReq.nFileSize = file_req.nFileSize;
    }

    return nRet;
}

int C5ESSCollect::GetFile(SGetFileReq& stReq)
{
    if(stReq.szSrcName[0] == '\0')
    {
        COTRACE("Remote file name is not specified: failed to get file");
        return ERR_FAIL;
    }


    //检查是否属于本采集任务
    int nFileListIndex = GetFileListIndex(stReq.szSrcName);
    if(nFileListIndex == -1)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               S_COL_FILE_NOT_FOUND,
               m_szSwitchName,
               stReq.szSrcName);
        return ERR_FAIL;
    }

    //取链路
    int  nRet = 0;
    m_pLinkMgr = NULL;
    int nIndex = GetPrefixIndex(stReq.szSrcName);
    if(nIndex == -1)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               S_COL_FILE_NOT_FOUND,
               m_szSwitchName,
               stReq.szSrcName);
        return ERR_FAIL;
    }
    m_pLinkMgr = m_LinkMgrMap[m_vFilePrefix[nIndex]];

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
    m_FileList[nFileListIndex].nCollectStatus = CS_COLLECTING;
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
                         S_COL_FILE_DISMATCHED_LENGTH,
                         m_szSwitchName,
                         stReq.szSrcName,
                         stReq.nFileSize,
                         nLength);
                szTraceMsg[sizeof(szTraceMsg) - 1] = '\0';
                COTRACE(szTraceMsg);
                COMSGOUT(MSGOUT_LEVEL_URGENT, szTraceMsg);

                //由于文件被采集下来后，交换机会自动删除该文件，如果返回失败，
                //该文件则再也找不到，此处返回成功至少可减少一些损失
                nRet = ERR_SUCCESS;
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
                         S_COL_FAILED_TO_MOVE_FILE,
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
                 S_COL_FAILED_TO_COLLECT_FILE,
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
        m_MutexForList.release();
    }
    else
    {
        m_MutexForList.acquire();
        m_FileList[nFileListIndex].nCollectStatus = CS_COLLECTED;
        m_MutexForList.release();


        //将文件信息加到转换文件池中
        SSwitchFileInfo stFileInfo;
        stFileInfo.nSwitchIndex = m_nSwitchIndex;
        strncpy(stFileInfo.szModuleLabel,
                m_vLabel[nIndex].c_str(),
                sizeof(stFileInfo.szModuleLabel));
        strncpy(stFileInfo.szRemoteFile,
                stReq.szSrcName,
                sizeof(stFileInfo.szRemoteFile));
        strncpy(stFileInfo.szLocalFile,
                strDstFile.c_str(),
                sizeof(stFileInfo.szLocalFile));
        stFileInfo.nConvertInfo = nIndex + 1;

        GetFilePool()->PushBack(stFileInfo);
    }

    return  nRet;
}

//获得文件列表, 由派生类具体实现
int  C5ESSCollect::Col_GetList(SListOut& pOut)
{
    //检查参数合法性
    int  nRet ;     //各子函数返回值

    //获得指示文件fna.txt
    SGetFileReq   stGetReq; // 获取文件的请求信息
    memset(&stGetReq, 0, sizeof(stGetReq));

    stGetReq.nFileSize = 50; //不会超过50K
    strcpy(stGetReq.szSrcName, m_strFnaFile.c_str()); //源文件名
    strcpy(stGetReq.szSrcPath, m_strRemotePath.c_str()); //源文件路径

    //将本地指示文件改名为"fna"+"当前日期".txt的形式
    //FNA_FILE_NAME : "fna_%s.txt"
    sprintf(stGetReq.szDstName, FNA_FILE_NAME, get_date_time());
    strcpy( stGetReq.szDstPath, m_strFNAPath.c_str());

    m_pLinkMgr = NULL;
    if (m_LinkMgrMap.find(m_strFnaIndex) != m_LinkMgrMap.end())
    {
        m_pLinkMgr = m_LinkMgrMap[m_strFnaIndex];
    }
    if(m_pLinkMgr == NULL)
    {
        COTRACE(S_COL_FAILED_TO_GET_FILE_LIST, m_szSwitchName);
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               S_COL_FAILED_TO_GET_FILE_LIST,
               m_szSwitchName);
        return ERR_FAIL;
    }

    //调用文件传输类对象获取指示文件
    nRet  = m_pLinkMgr->GetFile( stGetReq );
    if ( nRet != ERR_SUCCESS )
    {
        //错误信息已记录
        COTRACE(S_COL_FAILED_TO_GET_FNA,
              m_szSwitchName,
              nRet);
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               S_COL_FAILED_TO_GET_FNA,
               m_szSwitchName,
               nRet);
        return nRet;  //程序错误返回
    }

    //连接文件名与路径信息
    string  strFnaName = m_strFNAPath + DIR_CHAR;
    strFnaName += stGetReq.szDstName;

    //读取指示文件中的内容，将文件名列表放到文件信息向量数组中
    FILE*   fp = fopen( strFnaName.c_str(), "r");
    if( NULL == fp )
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               S_COL_FAILED_TO_OPEN_FNA,
               m_szSwitchName,
               strFnaName.c_str(),
               errno);
        COTRACE(S_COL_FAILED_TO_OPEN_FNA,
              m_szSwitchName,
              strFnaName.c_str(),
              errno);

        return COL_LISTFAIL;  //程序错误返回
    }

    char      szFileInfo[256];  //包含一行文件信息的字符串
    SFileInfo stFile;           //文件信息结构

    /*逐行读取指示文件中的信息，每行的信息如下：
      文件名    文件大小\r\n*/
    int  nCount = 0;
    while( !feof( fp ) )
    {
        memset( szFileInfo, 0, sizeof(szFileInfo) );
        if( NULL == fgets( szFileInfo, 255, fp ) )
        {
            continue;
        }

        //从信息串得到文件名，保存到stFile
        memset( &stFile, 0, sizeof(stFile) );
        if ( !GetSubName(szFileInfo, stFile) )
        {
            continue;
        }

        int nIndex = GetPrefixIndex(stFile.szFileName);
        if(nIndex != -1)
        {
            char szRemoteFile[MAX_PATH] = "";
            if(stGetReq.szSrcPath[0] != '\0')
            {
                strncpy(szRemoteFile, stGetReq.szSrcPath, sizeof(szRemoteFile));
                if((szRemoteFile[strlen(szRemoteFile) - 1] != '\\')
                   && (szRemoteFile[strlen(szRemoteFile) - 1] != '/'))
                {
                    strncat(szRemoteFile, "/", sizeof(szRemoteFile));
                }
            }
            strncat(szRemoteFile, stFile.szFileName, sizeof(szRemoteFile));
            unsigned long uFileLen = 0;
            if(m_pLinkMgr->GetFileLen(szRemoteFile, uFileLen) == 0)
            {
                stFile.nFileSize = uFileLen;
            }
            pOut.FileList.push_back(stFile);
        }

    }
    fclose( fp );

    //added by chenliangwei
    std::sort(pOut.FileList.begin(), pOut.FileList.end(), Compare());
    //added end

    m_MutexForList.acquire();

    m_FileList.clear();
    for(int i = 0; i < pOut.FileList.size(); i++)
    {
        SFileInfo& fi = pOut.FileList[i];
        fi.nCollectStatus = CS_NOT_COLLECTED;
        m_FileList.push_back(fi);
    }

    m_MutexForList.release();

    return  ERR_SUCCESS;  //函数返回, pOut中包含文件列表信息
}

//动态创建子目录, 由派生类具体实现
BOOL C5ESSCollect::CreateLocSubDir()
{
    char szTmp[24];
    time_t tNow = time(NULL);
    tm tmNow = *localtime(&tNow);
    strftime(szTmp,24,"%Y%m%d",&tmNow);
    m_strCurDate = szTmp;
    m_strLocalPath = m_strLocalRootPath + "/" + szTmp;
    CreateDeepDir(m_strLocalPath.c_str());

    m_strFNAPath = m_strFNARootPath + "/" + szTmp;
    CreateDeepDir(m_strFNAPath.c_str());

    /*STRING strSavePath;
    for(int i = 0; i <= m_vBillSavePath; i++)
    {
        strSavePath = m_vBillSavePath[i] + DIR_CHAR + m_strCurDate;
        if(!CreateDeepDir(strSavePath.c_str()))
        {
            m_strError = strerror(errno);
            return FALSE;
        }
    }*/
    return TRUE;
}

//清除过期文件, 由派生类具体实现
BOOL C5ESSCollect::ClearOldFile()
{
    //added by chenliangwei
    int nRet1 = ClearOldDateSubDir(m_strLocalRootPath.c_str(),
                                   m_nFileSaveDays);
    int nRet2 = ClearOldDateSubDir(m_strFNARootPath.c_str(),
                                   m_nFileSaveDays);
    //added end
    return nRet1 == ERR_SUCCESS && nRet2 == ERR_SUCCESS;
}

//检查文件序列号是否连续，并处理漏采文件
BOOL C5ESSCollect::CheckFileSN(const char* szFileName, char* szPreviousFile)
{
    if ((m_pFileSNMgr ==NULL)||(m_bCheckSN == 0))
    {
        return TRUE;
    }

    if (strstr(szFileName, PREFIX_AMA_FILE) == NULL)
    {
        return TRUE;
    }


    return m_pFileSNMgr->CheckFileSN(szFileName, szPreviousFile);

}

//文件管理器的回调函数，通过文件的序列号计算漏采文件名
STRING C5ESSCollect::GetFileNameBySN(const int   nFileSN,
                                     const char* szReferName,
                                     const int   nSNLen,
                                     const int   nPostfixLen)
{

    //计算序列号
    char szFormat[20];
    sprintf(szFormat, "%%0%dd", nSNLen);
    char* szFileSN = new char[nSNLen + 1];
    SNPRINTF(szFileSN, nSNLen + 1, szFormat, nFileSN);
    szFileSN[nSNLen] = '\0';

    STRING strFileName = "";
    strFileName = strFileName + PREFIX_AMA_FILE
                 + "*." + szFileSN;

    return strFileName;

}


BOOL C5ESSCollect::GetSubName(const char *sInfoLine,
                              SFileInfo& szFileInfo )
{

    char   buf[256];
    float  fFileSize = 0;

    if( NULL == sInfoLine)      //为NULL
    {
        return  FALSE;
    }

    if( strlen(sInfoLine) == 0 ) //为""
    {
        return  FALSE;
    }

    if( strlen(sInfoLine) > sizeof(buf)-1 )
    {
        return FALSE;
    }

    strcpy( buf, sInfoLine );
    buf[strlen(buf)-1] ='\0';   //去除'\n'
    StringLRTrim(buf);
    if( strlen(buf) == 0 ) //为""
    {
        return  FALSE;
    }

    //解析出 文件名 和 文件大小;格式为：stream001.1234     1002.4       2002-10-21
    int nRet = sscanf(buf,"%s%f", szFileInfo.szFileName, &fFileSize );
    szFileInfo.nFileSize      = fFileSize * 1024;
    szFileInfo.nCollectStatus = CS_NOT_COLLECTED;

    return nRet == 2;

} //end of GetSubName

//added by chenliangwei
BOOL C5ESSCollect::Compare::operator ()(const SFileInfo &lfileinfo, const SFileInfo &rfileinfo)
{
    //如果不是AMA文件，则放到列表的尾部
    if (strstr(rfileinfo.szFileName, PREFIX_AMA_FILE) == NULL)
    {
        return TRUE;
    }

    if (strstr(lfileinfo.szFileName, PREFIX_AMA_FILE) == NULL)
    {
        return FALSE;
    }

    char szlFileSN[10], szrFileSN[10];
    int  nlLen = 0, nrLen = 0;
    UINT4 nlFileSN = 0, nrFileSN = 0;

    nlLen = strlen(lfileinfo.szFileName);
    strcpy(szlFileSN, lfileinfo.szFileName + nlLen - LEN_OF_AMA_FILE_SN);
    nlFileSN = (UINT4)atoi(szlFileSN);

    nrLen = strlen(rfileinfo.szFileName);
    strcpy(szrFileSN, rfileinfo.szFileName + nlLen - LEN_OF_AMA_FILE_SN);
    nrFileSN = (UINT4)atoi(szrFileSN);

    return (nlFileSN < nrFileSN);
}
//added end

//指定的文件是否可采
int C5ESSCollect::CanGetSpecFile(const char* const szModule,
                                 const char* const szSrcFile)
{
    m_MutexForList.acquire();

    int nRet = ERR_FILE_CANNOT_BE_FOUND;
    for(int i = 0; i < m_FileList.size(); i++)
    {
        SFileInfo& fi = m_FileList[i];
        if(strcmp(fi.szFileName, szSrcFile) == 0)
        {
            if(CS_NOT_COLLECTED == fi.nCollectStatus)
            {
                nRet = ERR_SUCCESS;
            }
            else if(CS_COLLECTING == fi.nCollectStatus)
            {
                nRet = ERR_FILE_BEING_COLLECTED;
            }
            else
            {
                nRet = ERR_NEW_FILE_CANNOT_BE_CO;
            }
            break;
        }
    }

    m_MutexForList.release();

    return nRet;
}

int C5ESSCollect::GetSpecFile(const char* const szModule,
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
               S_COL_FAILED_TO_MAN_COL_FILE,
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
        nRet = GetFile(file_req);
    }

    return nRet;
}

//获取交换机信息
int C5ESSCollect::GetSwitchInfo(LIST<STRING>& label,
                                LIST<STRING>& content)
{
    label.push_back("module");

    VECTOR<STRING>::iterator i = m_vLabel.begin();
    for(; i != m_vLabel.end(); i++)
    {
        content.push_back(*i);
    }

    return ERR_SUCCESS;
}

//获取模块信息
int C5ESSCollect::GetModuleInfo(const char* const szModule,
                                LIST<STRING>& label,
                                LIST<STRING>& content)
{
    label.push_back("文件名");
    label.push_back("文件长度");
    label.push_back("采集状态");

    int nLabelIndex = -1;
    for(int i = 0; i < m_vLabel.size(); i++)
    {
        if(m_vLabel[i] == szModule)
        {
            nLabelIndex = i;
            break;
        }
    }

    if(nLabelIndex >= 0)
    {
        m_MutexForList.acquire();

        for(int i = 0; i < m_FileList.size(); i++)
        {
            SFileInfo& fi = m_FileList[i];
            if(GetPrefixIndex(fi.szFileName) != nLabelIndex)
            {
                continue;
            }

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
    }

    if(content.size() == 0)
    {
        content.push_back("-");
        content.push_back("-");
        content.push_back("-");
    }

    return ERR_SUCCESS;
}

//获取文件长度
int C5ESSCollect::GetFileLen(const char* const szModule,
                             const char* const szSrcFile)
{
    m_MutexForList.acquire();

    int nRet = 0;
    for(int i = 0; i < m_FileList.size(); i++)
    {
        SFileInfo& fi = m_FileList[i];
        if((strcmp(fi.szFileName, szSrcFile) == 0)
           && (CS_NOT_COLLECTED == fi.nCollectStatus))
        {
            nRet = fi.nFileSize;
            break;
        }
    }

    m_MutexForList.release();

    return nRet;
}

