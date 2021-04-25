#include "../../include/base_type.h"
#include "collect_include.h"
#include "collect_define.h"
#include "collect_struct.h"
#include "vertel_ftam.h"
#include "../include/adapter_include.h"
#include "../include/ftam_cmd_code.h"
#include "../../include/toolbox.h"
#include "x25_link_check.h"

ACE_Recursive_Thread_Mutex CVertelFtam::g_VertelMutex;

// 构造函数
CVertelFtam::CVertelFtam(int nTransSpeed, const char* szDevName)
{
    m_nTransSpeed = nTransSpeed * 1024;
    if(0 >= m_nTransSpeed)
    {
        m_nTransSpeed = 6 * 1024;
    }
    m_bOperate    = FALSE;

    m_LastStatusList.push_back(ERR_SUCCESS);
    m_LastStatusList.push_back(ERR_FAIL);
    m_LastStatusList.push_back(ERR_FAIL);

    m_szDevName[0] = '\0';
    if(NULL != szDevName)
    {
        strncpy(m_szDevName, szDevName, sizeof(m_szDevName));
        m_szDevName[sizeof(m_szDevName) - 1] = '\0';
        CX25LinkCheck::Instance()->RegisterLink(m_szDevName);
    }
}

//析构函数
CVertelFtam::~CVertelFtam()
{
    //
}

//初始化本地文件协议服务, 是虚函数, 由派生类具体实现该方法
int CVertelFtam::InitProtocol()
{
    return  ERR_SUCCESS;
}

//测试能否正常连接对端文件服务器, 虚函数, 由派生类具体实现
int CVertelFtam::TryConnect()
{
#ifdef _PLATFORM_HPUX
    if('\0' != m_szDevName[0])
    {
        return CX25LinkCheck::Instance()->GetLinkState(m_szDevName);
    }
    else
    {
#endif //_PLATFORM_HPUX

    if(!m_bOperate)
    {
        char szCmdLink[1024];
        SNPRINTF(szCmdLink, 
                 sizeof(szCmdLink),
                 "./vftam %s 01=%s 02=%s 03=%s 04=%s 05=%s 06=%s",
                 FTAM_TEST_LINK,
                 m_szLocalName,
                 m_szLocalUsr,
                 m_szLocalPwd,
                 m_szSvrName,
                 m_szSvrUsr,
                 m_szSvrPwd);

        m_bOperate = TRUE;
        int nRet = system(szCmdLink);
        m_bOperate = FALSE;
        m_LastStatusList.push_back(nRet);
        m_LastStatusList.pop_front();
    }

    LIST<int>::iterator i;
    for(i = m_LastStatusList.begin();
        i != m_LastStatusList.end();
        i++)
    {
        if(*i == ERR_SUCCESS)
        {
            return ERR_SUCCESS;
        }
    }

#ifdef _PLATFORM_HPUX
    }
#endif //_PLATFORM_HPUX

    return ERR_FAIL;
}

//实现拷贝文件服务端的单个文件到本地, 虚函数, 由派生类具体实现
int CVertelFtam::GetFile(const SGetFileReq &pReq)
{
    //g_VertelMutex.acquire();

    char szCmdLink[1024];

    //计算超时等待时间：文件大小(K)除以速率
    int nTimeOut = pReq.nFileSize/m_nTransSpeed;

    //等待时间为实际传输时间的2倍
    nTimeOut = nTimeOut * 2 + 20;

    SNPRINTF(szCmdLink, 
             sizeof(szCmdLink),
             "./vftam %s 01=%s 02=%s 03=%s 04=%s 05=%s 06=%s 07=%s 08=%s 09=%s 10=%s 11=%d",
             FTAM_GET_FILE,
             m_szLocalName,
             m_szLocalUsr,
             m_szLocalPwd,
             m_szSvrName,
             m_szSvrUsr,
             m_szSvrPwd,
             pReq.szSrcPath,
             pReq.szSrcName,
             pReq.szDstPath,
             pReq.szDstName,
             nTimeOut);

    m_bOperate = TRUE;
    int nRet = system(szCmdLink);
    if(ERR_SUCCESS != nRet)
    {
        COTRACE("GetFile failed.(ErrCode=%d)\n",
              nRet);
    }
    m_bOperate = FALSE;


    //g_VertelMutex.release();

    return nRet;
}

//获得文件服务器当前目录下的文件列表, 虚函数, 由派生类具体实现该类
int CVertelFtam::GetFileList( const  char* szSrcDir, SListOut &pOut )
{
    static int g_nCount = 0;

    g_VertelMutex.acquire();

    CreateDeepDir(SZ_OUTPUT_PATH);

    char szOutputFile[MAX_PATH];
    SNPRINTF(szOutputFile, 
             sizeof(szOutputFile),
             "%s/getattr_%d_%d_%d.txt",
             SZ_OUTPUT_PATH,
             time(NULL) % 100,
             ACE_OS::rand() % 10000,
             g_nCount++);
    szOutputFile[MAX_PATH - 1] = '\0';
    if(g_nCount >= 1000)
    {
        g_nCount = 0;
    }

    g_VertelMutex.release();

    char szCmdLink[1024];
    SNPRINTF(szCmdLink, 
             sizeof(szCmdLink),
             "./vftam %s 01=%s 02=%s 03=%s 04=%s 05=%s 06=%s 07=%s 08=%d",
             FTAM_GET_FILE_LIST,
             m_szLocalName,
             m_szLocalUsr,
             m_szLocalPwd,
             m_szSvrName,
             m_szSvrUsr,
             m_szSvrPwd,
             szSrcDir,
             szOutputFile);

    m_bOperate = TRUE;
    int nRet = system(szCmdLink);
    if(ERR_SUCCESS != nRet)
    {
        COTRACE("GetFileList failed.(ErrCode=%d)\n", nRet);
    }
    m_bOperate = FALSE;

    FILE* fp = fopen(szOutputFile, "r");
    if(fp == NULL)
    {
        return -1;
    }
    char szFileInfo[1024];  //包含一行文件信息的字符串
    int  nCount = 0;
    while( !feof( fp ) )
    {
        szFileInfo[0] = '\0';
        if(NULL == fgets(szFileInfo, sizeof(szFileInfo), fp))
        {
            continue;
        }

        //从信息串得到文件名
        StringLRTrim(szFileInfo);
        SFileInfo fi;
	    sscanf(szFileInfo, 
               "%s%d%d",
               fi.szFileName,
               &fi.nAttr,
               &fi.nFileSize);
        fi.nCollectStatus = CS_NOT_COLLECTED;
        pOut.FileList.push_back(fi);
    }
    fclose( fp );
    
    remove(szOutputFile);
    //g_VertelMutex.release();

    return nRet;
}

//删除文件服务端的指定文件
int CVertelFtam::DelFile( const char* szFileName )
{
    //g_VertelMutex.acquire();

    char szCmdLink[1024];

    SNPRINTF(szCmdLink, 
             sizeof(szCmdLink),
             "./vftam %s 01=%s 02=%s 03=%s 04=%s 05=%s 06=%s 07=%s",
             FTAM_DELETE_FILE,
             m_szLocalName,
             m_szLocalUsr,
             m_szLocalPwd,
             m_szSvrName,
             m_szSvrUsr,
             m_szSvrPwd,
             szFileName);

    m_bOperate = TRUE;
    int nRet = system(szCmdLink);
    if(ERR_SUCCESS != nRet)
    {
        COTRACE("DelFile failed.(ErrCode=%d)\n",
              nRet);
    }
    m_bOperate = FALSE;

    //g_VertelMutex.release();

    return nRet;
}

//取文件服务端的指定文件的长度
int CVertelFtam::GetFileLen(const char* szFileName,
                            unsigned long& uFileLen)
{
    static int g_nCount = 0;

    g_VertelMutex.acquire();

    CreateDeepDir(SZ_OUTPUT_PATH);

    char szOutputFile[MAX_PATH];
    SNPRINTF(szOutputFile, 
             sizeof(szOutputFile),
             "%s/getattr_%d_%d_%d.txt",
             SZ_OUTPUT_PATH,
             time(NULL) % 100,
             ACE_OS::rand() % 10000,
             g_nCount++);
    szOutputFile[MAX_PATH - 1] = '\0';
    if(g_nCount >= 1000)
    {
        g_nCount = 0;
    }

    g_VertelMutex.release();

    char szCmdLink[1024];

    SNPRINTF(szCmdLink, 
             sizeof(szCmdLink),
             "./vftam %s 01=%s 02=%s 03=%s 04=%s 05=%s 06=%s 07=%s 08=%s",
             FTAM_GET_FILE_ATTR,
             m_szLocalName,
             m_szLocalUsr,
             m_szLocalPwd,
             m_szSvrName,
             m_szSvrUsr,
             m_szSvrPwd,
             szFileName,
             szOutputFile);

    remove(szOutputFile);

    m_bOperate = TRUE;
    int nRet = system(szCmdLink);
    m_bOperate = FALSE;

    if(ERR_SUCCESS != nRet)
    {
        COTRACE("GetFileLen failed.(ErrCode=%d)\n",
              nRet);
    }
    FILE* fp = fopen(szOutputFile, "r");
    if(fp == NULL)
    {
        return -1;
    }

    char szFileInfo[1024] = "";  //包含一行文件信息的字符串
    char szTemp[MAX_PATH];
	time_t            tCreateTime;
	time_t            tModTime;
	time_t            tReadTime;
	time_t            tAttrModTime;
    unsigned long     nFileFutureSize;
    if(NULL != fgets(szFileInfo, sizeof(szFileInfo), fp))
    {
        //从信息串得到文件名
        StringLRTrim(szFileInfo);
	    sscanf(szFileInfo, 
               "%s%d%d%d%d%d%d",
               szTemp,
               &tCreateTime,
               &tModTime,
               &tReadTime,
               &tAttrModTime,
               &uFileLen,
               &nFileFutureSize);
    }
    fclose( fp );
    
    remove(szOutputFile);
    return  nRet;
}

//重命名指定文件
int CVertelFtam::RenameFile( const char *szOld, const char *szNew )
{
    //g_VertelMutex.acquire();

    char szCmdLink[1024];

    SNPRINTF(szCmdLink, 
             sizeof(szCmdLink),
             "./vftam %s 01=%s 02=%s 03=%s 04=%s 05=%s 06=%s 07=%s 08=%s",
             FTAM_RENAME_FILE,
             m_szLocalName,
             m_szLocalUsr,
             m_szLocalPwd,
             m_szSvrName,
             m_szSvrUsr,
             m_szSvrPwd,
             szOld,
             szNew);

    m_bOperate = TRUE;
    int nRet = system(szCmdLink);
    if(ERR_SUCCESS != nRet)
    {
        COTRACE("RenameFile failed.(ErrCode=%d)\n",
              nRet);
    }
    m_bOperate = FALSE;

    //g_VertelMutex.release();

    return nRet;
}


