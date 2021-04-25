/******************************************************************************

                  版权所有 (C), 2004-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : manual_task.cpp
  版 本 号   : iGWBV200R002B02
  作    者   : Zou Yongsheng
  生成日期   : 2004年5月20日
  最近修改   :
  功能描述   : 备份任务
  函数列表   :
  修改历史   :
    日    期   : 2004年5月20日
    作    者   : Zou Yongsheng
    修改内容   : 创建文件

******************************************************************************/
#include "manual_task.h"
#include "backup_server.h"

CManBakTask::CManBakTask(int nClientNo, int nTaskID, CBakTaskMgr *pBakTaskMgr, 
                         const char* szPath)                   
                         : CNetBakTask(nTaskID, pBakTaskMgr)
{
    STRNCPY(m_szManBakDir, szPath, MAX_PATH);
    m_bLocal = FALSE;
    m_nClientNo = nClientNo;
}

CManBakTask::~CManBakTask()
{
}
/*****************************************************************************
函 数 名  : BOOL CManBakTask::GetCfg()
功能描述  : 获得配置文件信息
输入参数  : 无
输出参数  : 无
返 回 值  : BOOL 是否成功
调用函数  :
被调函数  :

  修改历史   :
  日    期   : 2004年5月21日
  作    者   : Zou Yongsheng
  修改内容   : 新生成函数

*****************************************************************************/
BOOL CManBakTask::GetCfg()
{
    if(!CNetBakTask::GetCfg())
    {
        return FALSE;
    }

    CINIFile ini(GetCfgFilePath());
    if(ini.Open() != TRUE)
    {
        TRACE(MTS_BACKUP,S_BACKUP_INI_OPEN_FAIL);
        return FALSE;
    }

    char szDestHostIP[MAX_HOST_LEN];
    ini.GetString(CFG_SEC_NET_BACKUP,CFG_BACKUP_DEST_HOST_IP,
		CFG_BACKUP_DEST_HOST_IP_DEFAULT, szDestHostIP, MAX_HOST_LEN);

    char szBakSec[16];
	SNPRINTF(szBakSec,16,"%s%d",CFG_NET_BACKUP, m_nTaskID);
	szBakSec[15] = 0;
	
    ini.GetString(szBakSec,CFG_BACKUP_DEST_HOST_IP,
			      szDestHostIP, szDestHostIP, MAX_HOST_LEN);
    if(ACE_OS::strcasecmp(szDestHostIP, "Local") == 0)
    {
        m_bLocal = TRUE;
    }

    return TRUE;
}
/*****************************************************************************
函 数 名  : CManBakTask::open
功能描述  : 设置备份信息,启动备份任务
输入参数  : args , 参数信息
输出参数  : 无
返 回 值  : void
调用函数  :
被调函数  :

  修改历史   :
  日    期   : 2004年5月18日
  作    者   : Zou Yongsheng
  修改内容   : 新生成函数

*****************************************************************************/
int  CManBakTask::open(void* args)
{
    if(!GetCfg())
    {
        TRACE(MTS_BACKUP,S_BACKUP_OPEN_TASK_FAIL, m_nTaskID);
        return ERR_FAIL;
    }
    
    SManBakTaskInfo* pTaskInfo = NULL;
    if(!m_pBakTaskMgr->GetBakTaskInfo(m_nClientNo, pTaskInfo))
    {
        return ERR_FAIL;
    }
    
    m_nNameFlag = pTaskInfo->nFlag;

    if(m_nNameFlag == DATE_DIR)
    {
        m_DirList = pTaskInfo->NameList;
    }
    else
    {
        m_FileList = pTaskInfo->NameList;
    }
    STRNCPY(m_szCmd, pTaskInfo->szCmd, 256);
        
    //设置m_bIsExit为FALSE
    m_bIsExit = FALSE;

    //启动备份线程，如果失败返回ERR_FAIL
    if(this->activate())
    {
        m_bIsExit = TRUE;
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}
/*****************************************************************************
函 数 名  : CManBakTask::Backup
功能描述  : 手工备份线程执行函数
输入参数  : 无
输出参数  : 无
返 回 值  : void
调用函数  :
被调函数  :

  修改历史   :
  日    期   : 2004年5月18日
  作    者   : Zou Yongsheng
  修改内容   : 新生成函数

*****************************************************************************/
void CManBakTask::Backup()
{
    int nBakRetCode = ERR_SUCCESS;

    while(1)
    {
        if (m_pBakTaskMgr && m_pBakTaskMgr->GetLicenseCount() == 0)
        {
            TRACE(MTS_BACKUP, S_NO_BACKUP_LICENSE);
            nBakRetCode = ERR_LICENSE_EXPIRED;
            break;
        }
        
        //网络手工备份,需要连接目标机器
        if (!m_bLocal) 
        {
            if(!Connect())
            {
                TRACE(MTS_BACKUP,S_BACKUP_CONNECT_FAIL,m_nTaskID);
                nBakRetCode = ERR_CONNECT_FAILED;
                break;
            }
        }
        
        PrepareDirs(m_szManBakDir);
        while (!m_DirList.empty())
        {
            if(Sleep(0))
            {
                nBakRetCode = ERR_EVT_EXIT;
                break;
            }
     
            STRING& szCurDir = m_DirList.front();
            PrepareFiles(szCurDir.c_str());
            if (m_FileList.empty())
            {
                m_DirList.pop_front();
                continue;
            }
            char szDesDir[MAX_PATH];
            SNPRINTF(szDesDir,MAX_PATH,"%s%s",m_szDestDir,szCurDir.substr(strlen(m_szSrcDir)).c_str());
            szDesDir[MAX_PATH - 1] = 0;
            
            if (m_bLocal)
            {
                if (!CreateDeepDir(szDesDir))
                {
                    TRACE(MTS_BACKUP,S_BACKUP_CREATE_DIR_FAIL, m_nTaskID, szDesDir);
                    nBakRetCode = ERR_CRE_DIR;
                    break;
                }
            }
            else
            {
                if (m_pFtpClient[m_nActiveIndex]->SetDirectory(szDesDir))
                {
                    if (!MakeDir(szDesDir))
                    {
                        TRACE(MTS_BACKUP,S_BACKUP_CREATE_DIR_FAIL, m_nTaskID, szDesDir);
                        nBakRetCode = ERR_CRE_DIR;
                        break;
                    }
                    
                    if (m_pFtpClient[m_nActiveIndex]->SetDirectory(szDesDir))
                    {
                        nBakRetCode = ERR_SET_DIR;
                        break;
                    }
                }
            }
            
            int nRet = PutFiles(szCurDir.c_str(), szDesDir);
            if (nRet != ERR_SUCCESS)
            {
                nBakRetCode = nRet;
                break;
            }
            
            m_DirList.pop_front();
            m_FileList.clear();
            
        }//end while
        
        break;
    }
    
    if(nBakRetCode != ERR_EVT_EXIT)
    {
        SendSimpleReport(nBakRetCode);
    }

    m_bIsExit = TRUE;
    
    m_pBakTaskMgr->SetTaskExitFlag(m_nClientNo);
} 
/*****************************************************************************
函 数 名  : CManBakTask::PutFiles
功能描述  : 传送话单文件
输入参数  : szSrcDir, 传送话单源目录 szDstDir, 目标目录
输出参数  : 无
返 回 值  : int, 返回错误码
调用函数  :
被调函数  :

  修改历史   :
  日    期   : 2004年5月18日
  作    者   : Zou Yongsheng
  修改内容   : 新生成函数

*****************************************************************************/
int CManBakTask::PutFiles(const char* szSrcDir, const char* szDstDir)
{
    char szSrcFilePath[MAX_PATH];
    char szDstFilePath[MAX_PATH];

    while (m_FileList.size())
    {
        //等待退出事件
        if(Sleep(0))
        {
            return ERR_EVT_EXIT;
        }
        
        STRING &szFileName = m_FileList.front();
        SNPRINTF(szSrcFilePath, MAX_PATH, "%s/%s", szSrcDir, szFileName.c_str());
        szSrcFilePath[MAX_PATH - 1] = 0;

        SNPRINTF(szDstFilePath, MAX_PATH, "%s/%s", szDstDir, szFileName.c_str());
        szDstFilePath[MAX_PATH - 1] = 0;

        int nFlg = FALSE;
        
        //通过本地标志来调用不同的函数
        if (m_bLocal)
        {
            nFlg = CBakTask::PutAFile(szSrcFilePath, szDstFilePath);
        }
        else
        {
            nFlg = CNetBakTask::PutAFile(szSrcFilePath, szDstFilePath);
        }
        
        if(!nFlg)
        {
            return ERR_TRANS_FILE;
        }
        
        //是否自动备份已经备份过
        if(!m_pBakTaskMgr->IsAutoBackuped(szSrcFilePath, m_nTaskID))
        {
            ACE_stat FileStat;
            ACE_OS::stat(szSrcFilePath, &FileStat);
            
            SFilesListNode FileNode;
            STRNCPY(FileNode.szFileName, szSrcFilePath, MAX_PATH);
            FileNode.tFileTime = FileStat.st_ctime;

            m_pBakTaskMgr->AddBakedFile(FileNode);
        }
        
        m_FileList.pop_front();
    }

    return ERR_SUCCESS;
}
/*****************************************************************************
函 数 名  : CManBakTask::PrepareFiles
功能描述  : 准备目录szDir下的文件列表
输入参数  : szDir,当前目录路径
输出参数  : 文件列表m_FileList
返 回 值  : void
调用函数  :
被调函数  :

  修改历史   :
  日    期   : 2004年5月18日
  作    者   : Zou Yongsheng
  修改内容   : 新生成函数

*****************************************************************************/
void CManBakTask::PrepareFiles(const char* szDir)
{
    if(m_nNameFlag == FILE_NAME)
    {
        return;
    }
    
    GetFileList(szDir, m_FileList);
}
/*****************************************************************************
函 数 名  : CManBakTask::PrepareDirs
功能描述  : 准备szDir目录下的子目录
输入参数  : szDir,当前目录路径
输出参数  : 目录列表m_DirList
返 回 值  : void
调用函数  :
被调函数  :

  修改历史   :
  日    期   : 2004年5月18日
  作    者   : Zou Yongsheng
  修改内容   : 新生成函数

*****************************************************************************/
void CManBakTask::PrepareDirs(const char* szDir)
{
    if(m_nNameFlag == FILE_NAME)
    {
        m_DirList.clear();
        m_DirList.push_back(szDir);
        return;
    }

    STRING strPath = szDir;
    strPath += "/";
    
    STRINGLIST::iterator it = m_DirList.begin();
    for (; it != m_DirList.end(); it++)
    {
        STRING& strDir = *it;
        strDir = strPath + strDir;
    }
    return; 
}
/*****************************************************************************
函 数 名  : CManBakTask::SendSimpleReport
功能描述  : 向客户端发送简单信息
输入参数  : nBakRetCode,备份返回码
输出参数  : 无
返 回 值  : void
调用函数  :
被调函数  :

  修改历史   :
  日    期   : 2004年5月18日
  作    者   : Zou Yongsheng
  修改内容   : 新生成函数

*****************************************************************************/
void CManBakTask::SendSimpleReport(int nBakRetCode)
{
    char szInfo[256];
    int nRetCode = 1;
    
    switch(nBakRetCode)
    {
    case ERR_LICENSE_EXPIRED:
        STRNCPY(szInfo, S_NO_BACKUP_LICENSE, 256);
        break;
    case ERR_CONNECT_FAILED:
        STRNCPY(szInfo, S_BACKUP_CONNECT_FAILED, 256);
        break;
    case ERR_CRE_DIR:
        STRNCPY(szInfo, S_BACKUP_CRE_DIR_FAILED, 256);
        break;
    case ERR_SET_DIR:
        STRNCPY(szInfo, S_BACKUP_SET_DIR_FAILED, 256);
        break;
    case ERR_TRANS_FILE:
        STRNCPY(szInfo, S_BACKUP_TRANS_FAILED, 256);
        break;
    case ERR_SUCCESS:
        STRNCPY(szInfo, S_BACKUP_RUNNING_NOW, 256);
        nRetCode = 0;
        break;
    default:
        break;
    }

    m_pBakTaskMgr->m_pBackupServer->SendSimpleReport(m_szCmd, nRetCode, szInfo,
                                                     m_nClientNo);
}

/*****************************************************************************
函 数 名  : CManualStatus::Init
功能描述  : 初始化状态文件
输入参数  : 无
输出参数  : 无
返 回 值  : BOOL, 成功与否
调用函数  :
被调函数  :

  修改历史   :
  日    期   : 2004年5月23日
  作    者   : Zou Yongsheng
  修改内容   : 新生成函数

*****************************************************************************/
BOOL CManualStatus::Init()
{
    //获取状态路径信息
    CINIFile ini(GetCfgFilePath());
	if(ini.Open() != TRUE)
	{
		TRACE(MTS_BACKUP,S_BACKUP_INI_OPEN_FAIL);
		return ERR_FAIL;
	}
	char szStatusFileName[MAX_PATH];
	ini.GetString(CFG_SEC_NET_BACKUP,CFG_BACKUP_STATUS_FILE_DIR,
                  CFG_BACKUP_STATUS_SRC_DIR_DEFAULT,szStatusFileName,MAX_PATH);
	m_szFilePath = szStatusFileName;
	m_szFilePath += "/manual_backup.bsf";
	ini.Close();
	CreateDeepDir(szStatusFileName);

     //读取已备份文件列表
    FILE *fp;
    SFilesListNode FileNode;
    
    m_BakedFileList.clear();
    
    if ((fp = fopen(m_szFilePath.c_str(), "rb")) != NULL)
    {
        while(!feof(fp))
        {
            if(fread(&FileNode, sizeof(SFilesListNode), 1, fp) != 1)
            {
                fclose(fp);
                return ERR_FAIL;
            }

            m_BakedFileList.push_back(FileNode);
        }

        fclose(fp);
    }

    return TRUE;
}
/*****************************************************************************
函 数 名  : CManualStatus::RefreshFile
功能描述  : 刷新状态文件
输入参数  : 无
输出参数  : 无
返 回 值  : BOOL, 成功与否
调用函数  :
被调函数  :

  修改历史   :
  日    期   : 2004年5月23日
  作    者   : Zou Yongsheng
  修改内容   : 新生成函数

*****************************************************************************/
BOOL CManualStatus::RefreshFile()
{
    STRING szStatusFileB = m_szFilePath;
    szStatusFileB += ".bak";

    BOOL bExist = TRUE;

    if (!FileMove(m_szFilePath.c_str(), szStatusFileB.c_str()))
    {
       if (!ACE_OS::access(szStatusFileB.c_str(), F_OK))
       {
           return FALSE;
       }
       bExist = FALSE;
    }
    
    FILE* fp = fopen(szStatusFileB.c_str(), "wb");
    if (fp == NULL) 
    {
        return FALSE;
    }

    SFilesListNode FileNode;

    LISTFILELISTNODE::iterator it = m_BakedFileList.begin();
    for (; it != m_BakedFileList.end(); it++)
    {
        FileNode = *it;
        if (fwrite(&FileNode, sizeof(SFilesListNode), 1, fp) != 1)
        {
            fclose(fp);
            if(bExist)
            {
                FileCopy(szStatusFileB.c_str(), m_szFilePath.c_str());
            }
                 
            return FALSE;
        }
    }

    if(!RefreshFile())
    {
        return FALSE;
    }
    return TRUE;
}
/*****************************************************************************
函 数 名  : CManualStatus::AddFile
功能描述  : 增加备份文件
输入参数  : 无
输出参数  : 无
返 回 值  : BOOL, 成功与否
调用函数  :
被调函数  :

  修改历史   :
  日    期   : 2004年5月23日
  作    者   : Zou Yongsheng
  修改内容   : 新生成函数

*****************************************************************************/
BOOL CManualStatus::AddFile(SFilesListNode &FileNode)
{
    ACE_GUARD_RETURN(ACE_Mutex, ace_mon, m_Mutex, -1);
    
    m_BakedFileList.push_back(FileNode);
    return TRUE;
}
/*****************************************************************************
函 数 名  : CManualStatus::DelFile
功能描述  : 删除备份文件
输入参数  : 无
输出参数  : 无
返 回 值  : BOOL, 成功与否
调用函数  :
被调函数  :

  修改历史   :
  日    期   : 2004年5月23日
  作    者   : Zou Yongsheng
  修改内容   : 新生成函数

*****************************************************************************/
BOOL CManualStatus::DelFile(SFilesListNode& FileNode)
{
    ACE_GUARD_RETURN(ACE_Mutex, ace_mon, m_Mutex, -1);
    
    LISTFILELISTNODE::iterator it = m_BakedFileList.begin();
    for (; it != m_BakedFileList.end(); it++)
    {
        if(FileNode == *it)
        {
            m_BakedFileList.erase(it);
            
            if(!RefreshFile())
            {
                return FALSE;
            }
            return TRUE;
        }
    }
    return FALSE;
}


BOOL CManualStatus::FindFile(SFilesListNode &FileNode)
{
    ACE_GUARD_RETURN(ACE_Mutex, ace_mon, m_Mutex, -1);

    LISTFILELISTNODE::iterator it = m_BakedFileList.begin();
    for (; it != m_BakedFileList.end(); it++)
    {
        if (FileNode == *it)
        {
            return TRUE;
        }
    }
    
    return FALSE;
}
