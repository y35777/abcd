// ewsd_mm_collect.cpp: implementation of the CEWSD_MMCollect class.
//
//////////////////////////////////////////////////////////////////////
#include "../../../include/base_type.h"
#include "../../include/i_ftam.h"
#include "../../include/i_cmise_fun.h"
#include "../vertel_ftam.h"
#include "ewsd_mm_collect.h"
#include "../bk_link_mgr.h"
#include "../global_fun.h"

#include "../../collector_file_pool.h"
#include "../../../include/toolbox.h"
#include "../../include/adapter_include.h"
#include "../../include/res_config.h"
#include <assert.h>

extern CCollectorFilePool* GetFilePool();
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEWSD_MMCollect::CEWSD_MMCollect(int nSwitchIndex, CLinkGroup* pLinkGroup)
                                 :CCollectBase(nSwitchIndex,pLinkGroup)
{
    //
}

CEWSD_MMCollect::~CEWSD_MMCollect()
{
    //
}

//初始化采集服务, 由派生类具体实现
int  CEWSD_MMCollect::Col_Init()
{
    int  nRet = 0;
    char szBuf[BUF_SIZE] = "";
    char szPathName[MAX_PATH];

    CINIFile ini(m_szCfgPath);
    ini.Open();
    SNPRINTF(szBuf,BUF_SIZE,"%s%d",CFG_CO_SEC_SWITCH,m_nSwitchIndex);

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

    //得到远端ewsd服务器上的采集源目录m_strRemotePath
    ini.GetString(szBuf,CFG_CO_REMOTE_DIR,
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

    //读配置文件, 得到区号m_szAreaCode
    ini.GetString(szBuf,
                  CFG_5ESS_AREA_CODE,
                  CFG_5ESS_AREA_CODE_DEFAULT,
                  m_szAreaCode,
                  sizeof(m_szAreaCode));

    //读配置文件,话单类型m_szBillType
    ini.GetString(szBuf,
                  CFG_5ESS_BILL_TYPE,
                  CFG_5ESS_BILL_TYPE_DEFAULT,
                  m_szBillType,
                  sizeof(m_szBillType));

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
    //added end
    //是否进行文件长度校验
    m_bFileLenVerify = ini.GetInt(CFG_CO_SEC_SYSTEM,
                                  CFG_CO_FILE_LEN_VERIFY,
                                  CFG_CO_FILE_LEN_VERIFY_DEFAULT);
    m_bFileLenVerify = ini.GetInt(szBuf,
                                  CFG_CO_FILE_LEN_VERIFY,
                                  m_bFileLenVerify);

    char szPriLink[50];
    char szBakLink[50];
    STRING strBillPath;

    ini.GetString(szBuf,
                  CFG_CO_PRI_LINK,
                  CFG_CO_PRI_LINK_DEFAULT,
                  szPriLink,
                  sizeof(szPriLink));
    if((szPriLink[0] == '\0') || (NULL == m_pLinkGroup->GetLink(szPriLink)))
    {
        COTRACE("采集对象(EWSD_MM)主链路没有配置,或配置错误");
        return ERR_FAIL;
    }

    ini.GetString(szBuf,
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
    m_strBillIndex = "EWSD_MM_BILL";
    m_LinkMgrMap[m_strBillIndex] = pLinkMgr;

    char szTmpKey[BUF_SIZE];
    SNPRINTF(szTmpKey,BUF_SIZE,"%s%d_fna",CFG_CO_SEC_SWITCH,m_nSwitchIndex);
    m_strFnaIndex = szTmpKey;
    ini.GetString(szTmpKey,
                  CFG_5ESS_FNA_NAME,
                  CFG_5ESS_FNA_NAME_DEFAULT,
                  szBuf,
                  MAX_PATH);
    m_strFnaFile = szBuf;

    //创建本地指示文件存放目录
    ini.GetString(szTmpKey,
                  CFG_5ESS_SAVE_FNA_DIR,
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
        COTRACE("采集对象(EWSD_MM)指示文件的主链路没有配置,或配置错误");
        return ERR_FAIL;
    }

    ini.GetString(szTmpKey,
                  CFG_CO_BAK_LINK,
                  CFG_CO_BAK_LINK_DEFAULT,
                  szBakLink,
                  sizeof(szBakLink));

    pLinkMgr = new CBKLinkMgr(m_pLinkGroup);
    pLinkMgr->RegisterLink(szPriLink);
    if((szBakLink[0] != '\0') && (NULL != m_pLinkGroup->GetLink(szBakLink)))
    {
        pLinkMgr->RegisterLink(szBakLink);
    }
    m_LinkMgrMap[m_strFnaIndex] = pLinkMgr;

    return nRet;  //程序返回
}

STRING CEWSD_MMCollect::GetDstFileName(const char* const szSrcFile)
{
    char szDstName[MAX_PATH];

    strncpy(szDstName, szSrcFile, sizeof(szDstName));
    szDstName[sizeof(szDstName) - 1] = '\0';

    //本地文件名格式：区号_HHSS1234.话单类型
    sprintf(szDstName,
            "%s_%s.%s",
            m_szAreaCode,
            GetFileSerial(szSrcFile).c_str(),
            m_szBillType);

    return szDstName;
}

int CEWSD_MMCollect::GetFileListIndex(const char* const szSrcFile)
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

//拷贝文件服务端的单个文件到本地, 由派生类具体实现;
int CEWSD_MMCollect::Col_GetFile(SColFileReq& stReq)
{
    if(stReq.szSrcName[0] == '\0')
    {
        COTRACE("采集文件名不能为空：输入参数错误.");
        //COTRACE("Remote file name is not specified: failed to get file");
        return ERR_FAIL;
    }

    int nFileListIndex = GetFileListIndex(stReq.szSrcName);
    if(nFileListIndex == -1)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "采集对象(%s)取文件列表失败:文件(%s)不存在",
               m_szSwitchName,
               stReq.szSrcName);
        return ERR_FAIL;
    }

    int  nRet = 0;
    m_pLinkMgr = NULL;
    if (m_LinkMgrMap.find(m_strBillIndex) != m_LinkMgrMap.end())
    {
        m_pLinkMgr = m_LinkMgrMap[m_strBillIndex];
    }
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

    SGetFileReq stGetReq;
    memset(&stGetReq,0, sizeof(SGetFileReq));
    //源文件名
    strncpy(stGetReq.szSrcName, stReq.szSrcName, sizeof(stGetReq.szSrcName));

    //EWSD上的源文件路径
    strncpy(stGetReq.szSrcPath,
            m_strRemotePath.c_str(),
            sizeof(stGetReq.szSrcPath));

    strcpy( stGetReq.szDstPath, m_strLocalPath.c_str());

    //szBuf存放新的目的文件名
    strncpy(stGetReq.szDstName,
            GetDstFileName(stGetReq.szSrcName).c_str(),
            sizeof(stGetReq.szDstName));

    stGetReq.nFileSize = stReq.nFileSize;

    //设置临时文件名
    string  strSrcFile = m_strTmpPath;
    strSrcFile += "/";
    strSrcFile += stGetReq.szDstName;

    //设置正式文件名
    string  strDstFile = stGetReq.szDstPath;
    if((stGetReq.szDstPath[0] != 0) &&
       ((stGetReq.szDstPath[strlen(stGetReq.szDstPath) - 1] != '\\') &&
        (stGetReq.szDstPath[strlen(stGetReq.szDstPath) - 1] != '/')))
    {
        strDstFile += "/";
    }
    strDstFile += stGetReq.szDstName;

    m_MutexForList.acquire();
    m_FileList[nFileListIndex].nCollectStatus = CS_COLLECTING;
    m_MutexForList.release();

    strncpy(stGetReq.szDstPath, m_strTmpPath.c_str(), sizeof(stGetReq.szDstPath));
    stGetReq.szDstPath[sizeof(stGetReq.szDstPath) - 1] = '\0';
    nRet = m_pLinkMgr->GetFile(stGetReq);//可优化为传入全路径参数

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
                         stGetReq.szSrcName,
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
                 stGetReq.szSrcName,
                 stGetReq.szDstName);
        szTraceMsg[sizeof(szTraceMsg) - 1] = '\0';
        COTRACE(szTraceMsg);
        COMSGOUT(MSGOUT_LEVEL_URGENT, szTraceMsg);
    }

    char szFileName[MAX_PATH] = "";
    if(stGetReq.szSrcPath[0] != '\0')
    {
        SNPRINTF(szFileName, sizeof(szFileName), "%s/", stGetReq.szSrcPath);
    }
    strncat(szFileName, stGetReq.szSrcName, sizeof(szFileName));
    szFileName[sizeof(szFileName) - 1] = '\0';

    nRet = m_pLinkMgr->DelFile(szFileName);
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
        //added by chenliangwei
        strncpy(stReq.szDstName, strDstFile.c_str(), sizeof(stReq.szDstName));
        //added end

        //将文件信息加到转换文件池中
        SSwitchFileInfo stFileInfo;
        stFileInfo.nSwitchIndex = m_nSwitchIndex;
        strncpy(stFileInfo.szModuleLabel,
                "MM",
                sizeof(stFileInfo.szModuleLabel));
        strncpy(stFileInfo.szRemoteFile,
                stReq.szSrcName,
                sizeof(stFileInfo.szRemoteFile));
        strncpy(stFileInfo.szLocalFile,
                stReq.szDstName,
                sizeof(stFileInfo.szLocalFile));
        stFileInfo.nConvertInfo = 0;

        GetFilePool()->PushBack(stFileInfo);
        //added end

        m_MutexForList.acquire();
        m_FileList[nFileListIndex].nCollectStatus = CS_COLLECTED;
        m_MutexForList.release();
    }

    return  nRet;
}

//获得文件列表, 由派生类具体实现
int  CEWSD_MMCollect::Col_GetList(SListOut& pOut)
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
        //aTRACE("No link available: failed to get filelist");
        COTRACE("采集对象(%s)取文件列表失败:无可用的链路",
              m_szSwitchName);
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "采集对象(%s)取文件列表失败:无可用的链路",
               m_szSwitchName);
        return ERR_FAIL;
    }

    //调用文件传输类对象获取指示文件
    nRet  = m_pLinkMgr->GetFile( stGetReq );
    if ( nRet != ERR_SUCCESS )
    {
        //错误信息已记录
        COTRACE("采集对象(%s)获取指示文件失败(错误码:%d)",
              m_szSwitchName,
              nRet);
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "采集对象(%s)获取指示文件失败(错误码:%d)",
               m_szSwitchName,
               nRet);
        //aTRACE("Failed to get fna file" );
        return   nRet;  //程序错误返回
    }

    //连接文件名与路径信息
    string  strFnaName = m_strFNAPath + DIR_CHAR;
    strFnaName += stGetReq.szDstName;

    //读取指示文件中的内容，将文件名列表放到文件信息向量数组中
    FILE*   fp = fopen( strFnaName.c_str(), "r");
    if( NULL == fp )
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "采集对象(%s)打开本地指示文件(%s)失败(错误码:%d)",
               m_szSwitchName,
               strFnaName.c_str(),
               errno);
        COTRACE("采集对象(%s)打开本地指示文件(%s)失败(错误码:%d)",
              m_szSwitchName,
              strFnaName.c_str(),
              errno);

        //aTRACE("Failed to open fna file(%s)", strFnaName.c_str());
        return COL_LISTFAIL;  //程序错误返回
    }

    char      szFileInfo[256];  //包含一行文件信息的字符串
    SFileInfo stFile;           //文件信息结构

    /*逐行读取指示文件中的信息，每行的信息如下：
      文件名    文件大小\r\n*/
    STRING strErr = "指示文件中含有错误行";
    //STRING strErr = "Any error in fna file: ";
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
            if( strlen(szFileInfo) != 0 )
            {
                //记录错误日志
//                strErr += szFileInfo;
//                COTRACE(strErr.c_str());
                COMSGOUT(MSGOUT_LEVEL_URGENT, strErr.c_str());
            }
            continue;
        }
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
    fclose( fp );

    m_MutexForList.acquire();

    m_FileList.clear();
    for(int i = 0; i < pOut.FileList.size(); i++)
    {
        SFileInfo& fi = pOut.FileList[i];
        fi.nCollectStatus = CS_NOT_COLLECTED;
        m_FileList.push_back(fi);
    }

    m_MutexForList.release();

    char szFileName[MAX_PATH] = "";
    if(stGetReq.szSrcPath[0] != '\0')
    {
        SNPRINTF(szFileName, sizeof(szFileName), "%s/", stGetReq.szSrcPath);
    }
    strncat(szFileName, stGetReq.szSrcName, sizeof(szFileName));
    szFileName[sizeof(szFileName) - 1] = '\0';

    nRet = m_pLinkMgr->DelFile(szFileName);
    return  ERR_SUCCESS;  //函数返回, pOut中包含文件列表信息
}

//动态创建子目录, 由派生类具体实现
BOOL CEWSD_MMCollect::CreateLocSubDir()
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
BOOL CEWSD_MMCollect::ClearOldFile()
{
    //added by chenliangwei
    int nRet = ClearOldDateSubDir(m_strLocalRootPath.c_str(),
                                  m_nFileSaveDays);
    //added end
    return nRet == ERR_SUCCESS;
}

BOOL CEWSD_MMCollect::GetSubName(const char *sInfoLine,
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
    szFileInfo.nFileSize = fFileSize * 1024;

    return nRet == 2;

} //end of GetSubName

int CEWSD_MMCollect::GetSpecFile(const char* const szModule,
                                 const char* const szSrcFile,
                                 const char* const szDstFile)
{

    return ERR_FAIL;
}

//获取交换机信息
int CEWSD_MMCollect::GetSwitchInfo(LIST<STRING>& label,
                                   LIST<STRING>& content)
{
    label.push_back("Module");
    content.push_back("MM");

    return ERR_SUCCESS;
}

//获取模块信息
int CEWSD_MMCollect::GetModuleInfo(const char* const szModule,
                                   LIST<STRING>& label,
                                   LIST<STRING>& content)
{
    label.push_back("文件名");
    label.push_back("长度");
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

//指定的文件是否可采
int CEWSD_MMCollect::CanGetSpecFile(const char* const szModule,
                                    const char* const szSrcFile)
{
    m_MutexForList.acquire();

    int nRet = ERR_FAIL;
    for(int i = 0; i < m_FileList.size(); i++)
    {
        SFileInfo& fi = m_FileList[i];
        if((strcmp(fi.szFileName, szSrcFile) == 0)
           && (CS_NOT_COLLECTED == fi.nCollectStatus))
        {
            nRet = ERR_SUCCESS;
        }
    }

    m_MutexForList.release();

    return nRet;
}

//获取文件长度
int CEWSD_MMCollect::GetFileLen(const char* const szModule,
                                const char* const szSrcFile)
{
    return 0;
}

