/******************************************************************************

                  ��Ȩ���� (C), 2004-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : manual_task.cpp
  �� �� ��   : iGWBV200R002B02
  ��    ��   : Zou Yongsheng
  ��������   : 2004��5��20��
  ����޸�   :
  ��������   : ��������
  �����б�   :
  �޸���ʷ   :
    ��    ��   : 2004��5��20��
    ��    ��   : Zou Yongsheng
    �޸�����   : �����ļ�

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
�� �� ��  : BOOL CManBakTask::GetCfg()
��������  : ��������ļ���Ϣ
�������  : ��
�������  : ��
�� �� ֵ  : BOOL �Ƿ�ɹ�
���ú���  :
��������  :

  �޸���ʷ   :
  ��    ��   : 2004��5��21��
  ��    ��   : Zou Yongsheng
  �޸�����   : �����ɺ���

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
�� �� ��  : CManBakTask::open
��������  : ���ñ�����Ϣ,������������
�������  : args , ������Ϣ
�������  : ��
�� �� ֵ  : void
���ú���  :
��������  :

  �޸���ʷ   :
  ��    ��   : 2004��5��18��
  ��    ��   : Zou Yongsheng
  �޸�����   : �����ɺ���

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
        
    //����m_bIsExitΪFALSE
    m_bIsExit = FALSE;

    //���������̣߳����ʧ�ܷ���ERR_FAIL
    if(this->activate())
    {
        m_bIsExit = TRUE;
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}
/*****************************************************************************
�� �� ��  : CManBakTask::Backup
��������  : �ֹ������߳�ִ�к���
�������  : ��
�������  : ��
�� �� ֵ  : void
���ú���  :
��������  :

  �޸���ʷ   :
  ��    ��   : 2004��5��18��
  ��    ��   : Zou Yongsheng
  �޸�����   : �����ɺ���

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
        
        //�����ֹ�����,��Ҫ����Ŀ�����
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
�� �� ��  : CManBakTask::PutFiles
��������  : ���ͻ����ļ�
�������  : szSrcDir, ���ͻ���ԴĿ¼ szDstDir, Ŀ��Ŀ¼
�������  : ��
�� �� ֵ  : int, ���ش�����
���ú���  :
��������  :

  �޸���ʷ   :
  ��    ��   : 2004��5��18��
  ��    ��   : Zou Yongsheng
  �޸�����   : �����ɺ���

*****************************************************************************/
int CManBakTask::PutFiles(const char* szSrcDir, const char* szDstDir)
{
    char szSrcFilePath[MAX_PATH];
    char szDstFilePath[MAX_PATH];

    while (m_FileList.size())
    {
        //�ȴ��˳��¼�
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
        
        //ͨ�����ر�־�����ò�ͬ�ĺ���
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
        
        //�Ƿ��Զ������Ѿ����ݹ�
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
�� �� ��  : CManBakTask::PrepareFiles
��������  : ׼��Ŀ¼szDir�µ��ļ��б�
�������  : szDir,��ǰĿ¼·��
�������  : �ļ��б�m_FileList
�� �� ֵ  : void
���ú���  :
��������  :

  �޸���ʷ   :
  ��    ��   : 2004��5��18��
  ��    ��   : Zou Yongsheng
  �޸�����   : �����ɺ���

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
�� �� ��  : CManBakTask::PrepareDirs
��������  : ׼��szDirĿ¼�µ���Ŀ¼
�������  : szDir,��ǰĿ¼·��
�������  : Ŀ¼�б�m_DirList
�� �� ֵ  : void
���ú���  :
��������  :

  �޸���ʷ   :
  ��    ��   : 2004��5��18��
  ��    ��   : Zou Yongsheng
  �޸�����   : �����ɺ���

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
�� �� ��  : CManBakTask::SendSimpleReport
��������  : ��ͻ��˷��ͼ���Ϣ
�������  : nBakRetCode,���ݷ�����
�������  : ��
�� �� ֵ  : void
���ú���  :
��������  :

  �޸���ʷ   :
  ��    ��   : 2004��5��18��
  ��    ��   : Zou Yongsheng
  �޸�����   : �����ɺ���

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
�� �� ��  : CManualStatus::Init
��������  : ��ʼ��״̬�ļ�
�������  : ��
�������  : ��
�� �� ֵ  : BOOL, �ɹ����
���ú���  :
��������  :

  �޸���ʷ   :
  ��    ��   : 2004��5��23��
  ��    ��   : Zou Yongsheng
  �޸�����   : �����ɺ���

*****************************************************************************/
BOOL CManualStatus::Init()
{
    //��ȡ״̬·����Ϣ
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

     //��ȡ�ѱ����ļ��б�
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
�� �� ��  : CManualStatus::RefreshFile
��������  : ˢ��״̬�ļ�
�������  : ��
�������  : ��
�� �� ֵ  : BOOL, �ɹ����
���ú���  :
��������  :

  �޸���ʷ   :
  ��    ��   : 2004��5��23��
  ��    ��   : Zou Yongsheng
  �޸�����   : �����ɺ���

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
�� �� ��  : CManualStatus::AddFile
��������  : ���ӱ����ļ�
�������  : ��
�������  : ��
�� �� ֵ  : BOOL, �ɹ����
���ú���  :
��������  :

  �޸���ʷ   :
  ��    ��   : 2004��5��23��
  ��    ��   : Zou Yongsheng
  �޸�����   : �����ɺ���

*****************************************************************************/
BOOL CManualStatus::AddFile(SFilesListNode &FileNode)
{
    ACE_GUARD_RETURN(ACE_Mutex, ace_mon, m_Mutex, -1);
    
    m_BakedFileList.push_back(FileNode);
    return TRUE;
}
/*****************************************************************************
�� �� ��  : CManualStatus::DelFile
��������  : ɾ�������ļ�
�������  : ��
�������  : ��
�� �� ֵ  : BOOL, �ɹ����
���ú���  :
��������  :

  �޸���ʷ   :
  ��    ��   : 2004��5��23��
  ��    ��   : Zou Yongsheng
  �޸�����   : �����ɺ���

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
