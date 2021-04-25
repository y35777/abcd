/******************************************************************************
��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
******************************************************************************
    �� �� ��   : backup_task.cpp
    �� �� ��   : ����
    ��    ��   : ������
    ��������   : 2002��9��6��
    ����޸�   :
    ��������   : ���������ʵ��
    �����б�   :
    �޸���ʷ   :
    1.��    ��   : 2002��9��6��
    ��    ��   : ������
    �޸�����   : �����ļ�

******************************************************************************/
#include "backup_task.h"
#include "backup_file_pool.h"

ACE_Mutex CBakTask::g_LogFileMutex;
/*****************************************************************************
�� �� ��  : CBakTask::CBakTask
��������  : ���캯��
�������  : int nTaskID, CBakTaskMgr* pBakTaskMgr
�������  : ��
�� �� ֵ  :
���ú���  :
��������  :

  �޸���ʷ      :
  1.��    ��   : 2002��9��6��
  ��    ��   : ������
  �޸�����   : �����ɺ���

*****************************************************************************/
CBakTask::CBakTask(int nTaskID, CBakTaskMgr* pBakTaskMgr)
{
    //��ʼ��m_nTaskID = nTaskID
    m_nTaskID = nTaskID;
    //��ʼ��m_pBakTaskMgr = pBakTaskMgr
    m_pBakTaskMgr = pBakTaskMgr;
    //��ʼ��������Ա����
    m_nSearchFilesInterval = 0;
    m_szSrcDir[0] = '\0';
    m_szDestDir[0] = '\0';
    m_strDestHost  = "Local";
    m_bDelAfterBak = FALSE;
    m_bBakLastFile = FALSE;
    m_nDesFileSavedDays = 0;
    m_nBakAlmTimeout = 0;
    m_bIsExit = TRUE;
    m_nReStartTimes = 0;
    m_BakThdExitEvt.reset();
    m_nCurOffset = 0;
    memset(&m_CurStatus,0,sizeof(SStatusNode));
    m_LastBakOKTime     = time(NULL);
    m_bActivateLog      = FALSE;
    m_strTaskLabel      = "";
    m_strLogSaveDir     = "";
    m_strDestLogSaveDir = "";

    m_bIsJudged = FALSE;
    m_bIsSecond = FALSE;
}
/*****************************************************************************
�� �� ��  : WriteStatusFile
��������  : ����ǰ״̬д��״̬�ļ�
�������  : SStatusNode  CurStatus
�������  : ��
�� �� ֵ  : int
���ú���  :
��������  :

  �޸���ʷ      :
  1.��    ��   : 2002��9��6��
  ��    ��   : ������
  �޸�����   : �����ɺ���

*****************************************************************************/
BOOL CBakTask::WriteStatusFile(SStatusNode &CurStatus)
{
    return m_pBakTaskMgr->WriteStatusFile(CurStatus);
}
/*****************************************************************************
�� �� ��  : CBakTask::GetStatInfo
��������  : ��ȡĳ�����������״̬��Ϣ
�������  : const char* pszTaskName
SStatusNode *pStatInfo
�������  : ��
�� �� ֵ  : int
���ú���  :
��������  :

  �޸���ʷ      :
  1.��    ��   : 2002��9��6��
  ��    ��   : ������
  �޸�����   : �����ɺ���

*****************************************************************************/
int CBakTask::GetStatInfo( const char* pszTaskName, SStatusNode *pStatInfo)
{
    return m_pBakTaskMgr->GetStatInfo(pszTaskName, pStatInfo);
}
/*****************************************************************************
�� �� ��  : CBakTask::open
��������  : �����̺߳���
�������  : void* args
�������  : ��
�� �� ֵ  : int
���ú���  :
��������  :

  �޸���ʷ      :
  1.��    ��   : 2002��9��6��
  ��    ��   : ������
  �޸�����   : �����ɺ���

*****************************************************************************/
int  CBakTask::open(void* args)
{
    //���m_nReStartTimes > 3 ������ERR_START_TOOMANY_TIMES
    if(m_nReStartTimes++ > 3)
        return ERR_START_TOOMANY_TIMES;

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
�� �� ��  : CBakTask::Stop
��������  : ֹͣ�̺߳���
�������  : u_long flags
�������  : ��
�� �� ֵ  : void
���ú���  :
��������  :

  �޸���ʷ      :
  1.��    ��   : 2002��9��6��
  ��    ��   : ������
  �޸�����   : �����ɺ���

*****************************************************************************/
void CBakTask::Stop()
{
    //����m_BakThdExitEvt�¼�
    //m_BakThdExitEvt.signal();

    //ͨ��m_BakThdMgr�˳��߳�
    TRACE(MTS_BACKUP,S_BACKUP_END_TASK,m_nTaskID);
    wait();

}
/*****************************************************************************
�� �� ��  : CBakTask::svc
��������  : �����̺߳���
�������  : ��
�������  : ��
�� �� ֵ  : int
���ú���  :
��������  :

  �޸���ʷ      :
  1.��    ��   : 2002��9��6��
  ��    ��   : ������
  �޸�����   : �����ɺ���

*****************************************************************************/
int CBakTask::svc()
{
    Backup();
    return ERR_SUCCESS;
}

//�ж��Ƿ��ѱ��ݹ�
BOOL CBakTask::JudgeIsBackupped(const char* szFileName, time_t tFileTime)
{
    if(tFileTime < m_CurStatus.tFileTime)
    {
        return TRUE;
    }
    else if(tFileTime == m_CurStatus.tFileTime)
    {
        for(int i = 0; i < m_CurStatus.nFileCount; i++)
        {
            if(strcmp(m_CurStatus.szFileName[i], szFileName) == 0)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/*****************************************************************************
�� �� ��  : CBakTask::PrepareFiles
��������  : ׼�����������Ĵ������ļ��б�
�������  : const char* szPath - �����ļ����ڵ�·��
time_t tFileTime - ����״̬�ļ�ʱ��
BOOL bRmvLastFile
�������  : m_FileList
�� �� ֵ  : int
���ú���  :
��������  :

  �޸���ʷ      :
  1.��    ��   : 2002��9��6��
  ��    ��   : ������
  �޸�����   : �����ɺ���

*****************************************************************************/
#define READ_COUNT_WAIT_ONCE    10      //ÿ�εȴ�֮ǰ��ȡ���ļ���Ŀ
#define READ_FILE_INTERVAL      1000    //ʱ��,΢�뼶
 
int CBakTask::PrepareFiles( const char* szPath, time_t tFileTime, BOOL bRmvLastFile )
{
    //�ж��Ƿ��Ǻ���̵ڶ��ݻ���: ������(8λ����)Ŀ¼
    if (!m_bIsJudged)
    {
        const char* p1 = strrchr(szPath, '\\');
        const char* p2 = strrchr(szPath, '/');
        const char* p = max(p1, p2);
        
        if (p != NULL && !(IsDigitValue(++p) && (strlen(p) == 8)))
        {
            m_bIsSecond = TRUE;
        }

        m_bIsJudged = TRUE;
    }
    
    ACE_DIR *pDir = NULL;
    dirent *pDirent = NULL;
    ACE_stat sDirStat;

    ClearList();
    //���ļ�Ŀ¼,��szPathĿ¼
    pDir = ACE_OS::opendir(szPath);
    if(pDir == NULL)
    {
		//by ����	2004-5-29   ��Ӧ���ⵥSWPD05427
        char szTmpBuf[BK_MAX_INFO_LEN];
        sprintf(szTmpBuf, "Errno: %d (%s)", errno, strerror(errno));
        STRING sTmpInfo = S_BACKUP_OPEN_DIR_FAIL;//��·��ʧ��(231)
        sTmpInfo += STRING(" ") + szPath + STRING(" ");
        MSGOUT(MTS_BACKUP, MSGOUT_LEVEL_SIDLY, sTmpInfo.c_str());
        sTmpInfo += szTmpBuf;
        TRACE(MTS_BACKUP, sTmpInfo.c_str());
		//the end	2004-5-19	��Ӧ���ⵥSWPD05427

        return ERR_FAIL;
    }
    //MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,            //Commented by ZhengYuqun 2003-10-21
    //    S_BACKUP_SEARCH_FILE_IN,szPath,m_nTaskID);
    char szSrcFilePath[MAX_PATH];
    //�ѱ����������ļ�
    SFilesListNode Node;
    
    int nFileCount = 0;
    while((pDirent = ACE_OS::readdir(pDir)) != NULL)
    {
        ACE_Time_Value timeval(0);
        if (++nFileCount > READ_COUNT_WAIT_ONCE)
        {
            //����: 1
            timeval.set(0, READ_FILE_INTERVAL);
            nFileCount = 0;
        }

        timeval += ACE_OS::gettimeofday();
        int nRet = m_BakThdExitEvt.wait(&timeval);
        if(nRet == 0)
        {
            m_BakThdExitEvt.signal();
            break;
        }
        //�γ�Դ�ļ�·��
        SNPRINTF(szSrcFilePath,MAX_PATH,"%s/%s",szPath,pDirent->d_name);
        szSrcFilePath[MAX_PATH - 1] = 0;

        ACE_OS::stat(szSrcFilePath, &sDirStat);
        if(S_ISDIR(sDirStat.st_mode))
        {
            continue;
        }

        //�ļ���,ʱ��
        Node.tFileTime = sDirStat.st_ctime;
        strncpy(Node.szFileName,pDirent->d_name,MAX_PATH);
        Node.szFileName[MAX_PATH-1] = 0;
        /*for test
        char szTimeToDisplay[24];
        strftime(szTimeToDisplay,24,"%Y%m%d",localtime(&tFileTime));
        printf("Last Backup Time = %s\n",szTimeToDisplay);
        strftime(szTimeToDisplay,24,"%Y%m%d",localtime(&(Node.tFileTime)));
        printf("Current Create Time = %s\n",szTimeToDisplay);
        test over*/
        if(JudgeIsBackupped(Node.szFileName, Node.tFileTime))
        {
            //������ 2003-04-07
            //������ݹ������ӵ�ɾ���б�
            BOOL bIsBackupped = CNetBackupFilePool::GetInstance()->IsFileBackupped(
                                                                    m_nFilePoolID,
                                                                    m_nTaskID,
                                                                    sDirStat.st_ctime,
                                                                    Node.szFileName);
            if (m_bIsSecond  && (m_uDelFileDays > 0)
               && bIsBackupped
               && (time(NULL) >= sDirStat.st_ctime + m_uDelFileDays * ONE_DAY_IN_SEC))
            {
                m_DelFileList.push_back(Node);
            }
        }
        else
        {
            m_FileList.push_back(Node);
        }
        //MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,    //Commented by ZhengYuqun 2003-10-21  SWPD01995
        //       S_BACKUP_FIND_FILE_IN,
        //       szPath,Node.szFileName,m_nTaskID);

    }
    ACE_OS::closedir(pDir);

    m_FileList.sort();

    if(bRmvLastFile && m_FileList.size())
    {
        Node = m_FileList.back();
        m_FileList.pop_back();
        MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
               S_BACKUP_NO_BAK_LAST_FILE,
               Node.szFileName,m_nTaskID);
    }

    return ERR_SUCCESS;
}
/*****************************************************************************
�� �� ��  : PrepareSubDirs
��������  : ��pszDir�����е�Ŀ¼����SubDirList��
�������  : const char* pszDir
LIST2<STRING>& SubDirList
�������  : ��
�� �� ֵ  : void
���ú���  :
��������  :

  �޸���ʷ      :
  1.��    ��   : 2002��9��6��
  ��    ��   : ������
  �޸�����   : �����ɺ���

*****************************************************************************/
void CBakTask::PrepareSubDirs(const char* pszDir, LIST2<STRING> &SubDirList)
{
    PrepareSubDirs_Impl(pszDir,SubDirList);
    //��Ŀ¼����
    if(SubDirList.empty())
    {
        SubDirList.push_back(pszDir);
        MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
               S_BACKUP_PUSH_ROOT_DIR, pszDir, m_nTaskID);
    }
    std::sort(SubDirList.begin(),SubDirList.end(),Compare());
}

//PrepareSubDirs�ľ���ʵ��
void CBakTask::PrepareSubDirs_Impl(const char* pszDir, LIST2<STRING>& SubDirList )
{
    //��pszDir���ڵ�Ŀ¼���õ�ָ��pszDirĿ¼��ָ��pDir
    //��szPathĿ¼
    ACE_DIR *pDir = NULL;
    dirent *pDirent = NULL;
    ACE_stat sDirStat;

    //���ļ�Ŀ¼
    pDir = ACE_OS::opendir(pszDir);
    if(pDir == NULL)
    {
		//by ����	2004-5-29   ��Ӧ���ⵥSWPD05427	
        char szTmpBuf[BK_MAX_INFO_LEN];
        sprintf(szTmpBuf, "Errno: %d (%s)", errno, strerror(errno));
        STRING sTmpInfo = S_BACKUP_OPEN_DIR_FAIL;//��·��ʧ��(231)
        sTmpInfo += STRING(" ") + pszDir + STRING(" ");
        MSGOUT(MTS_BACKUP, MSGOUT_LEVEL_SIDLY, sTmpInfo.c_str());
        sTmpInfo += szTmpBuf;
        TRACE(MTS_BACKUP, sTmpInfo.c_str());
		//the end	2004-5-19	��Ӧ���ⵥSWPD05427

        return;
    }
    MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
           S_BACKUP_SEARCH_DIR,pszDir,m_nTaskID);
    //����pszDir���ļ�(��),������ļ��в��Ҳ��ǡ�.�����ߡ�..��
    //(������tmpDir),�������SubDirListβ��,����ͨ��
    //SNPRINTF(pszDir, MAX_PATH,"s%s" psz, tmpDir)
    //����pszDir��,�ݹ����PrepareSubDirs(pszDir,SubDirList)
    char szSrcFilePath[MAX_PATH];
    while((pDirent = ACE_OS::readdir(pDir)) != NULL)
    {
        ACE_Time_Value Seconds(0);
        Seconds += ACE_OS::gettimeofday ();
        int nRet = m_BakThdExitEvt.wait(&Seconds);
        if(nRet == 0)
        {
            m_BakThdExitEvt.signal();
            break;
        }

        SNPRINTF(szSrcFilePath,MAX_PATH,"%s/%s",pszDir,pDirent->d_name);
        szSrcFilePath[MAX_PATH - 1] = 0;

        ACE_OS::stat(szSrcFilePath, &sDirStat);
        if(!S_ISDIR(sDirStat.st_mode)
            || strcmp(pDirent->d_name,".") == 0
            || strcmp(pDirent->d_name,"..") == 0
            || !IsDigitValue(pDirent->d_name))
        {
            continue;
        }
        SubDirList.push_back(szSrcFilePath);
        MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
               S_BACKUP_FIND_DIR,szSrcFilePath,m_nTaskID);
        PrepareSubDirs_Impl(szSrcFilePath,SubDirList);
    }
    //�ر�pszDir����Ŀ¼
    ACE_OS::closedir(pDir);
}
/*****************************************************************************
�� �� ��  : PutFiles
��������  : ��m_FileList�е��ļ����η��͵�FTP Server�ˣ���֧��Դ�ļ�ɾ��
�������  : const char* szSrcPath
const char* szDestPath
�������  : ��
�� �� ֵ  : int
���ú���  :
��������  :

  �޸���ʷ      :
  1.��    ��   : 2002��9��6��
  ��    ��   : ������
  �޸�����   : �����ɺ���

*****************************************************************************/
int CBakTask::PutFiles( const char* szSrcPath, const char* szDestPath )
{
    SStatusNode NodeBak = m_CurStatus;
    int nLen = 2 * MAX_PATH + 64;
    char szLog[2 * MAX_PATH + 64];
    char szDesFileName[MAX_PATH];
    char szSrcFileName[MAX_PATH];
    while(m_FileList.size() > 0)
    {
        BOOL bRet = TRUE;
        SFilesListNode &Node = m_FileList.front();

        SNPRINTF(szSrcFileName,MAX_PATH,"%s/%s",szSrcPath,Node.szFileName);
        szSrcFileName[MAX_PATH - 1] = 0;
        SNPRINTF(szDesFileName,MAX_PATH,"%s/%s",szDestPath,Node.szFileName);
        szDesFileName[MAX_PATH - 1] = 0;
        //�޸����ⵥ18680
        for(int i = 0; i < 1; i++)
        {
            bRet = PutAFile(szSrcFileName,szDesFileName);
            if(bRet)
            {
                ACE_stat FileStat;
                ACE_OS::stat(szSrcFileName, &FileStat);
                
                SFilesListNode FileNode;
                STRNCPY(FileNode.szFileName, szSrcFileName, MAX_PATH);
                FileNode.tFileTime = FileStat.st_ctime;
                m_pBakTaskMgr->DelBakedFile(FileNode);
                
                if(m_CurStatus.tFileTime != Node.tFileTime)
                {
                    m_CurStatus.nFileCount = 0;
                    m_CurStatus.tFileTime = Node.tFileTime;
                }

                if(m_CurStatus.nFileCount < MAX_FILE_COUNT)
                {
                    strncpy(m_CurStatus.szFileName[m_CurStatus.nFileCount++],
                            Node.szFileName,
                            MAX_PATH);
                }

                if(!WriteStatusFile(m_CurStatus))
                {
                    //��ԭ��ԭ����״̬
                    m_CurStatus = NodeBak;
                    TRACE(MTS_BACKUP,S_BACKUP_WRITE_STATUS_FAIL,m_nTaskID,szSrcFileName);
                    SNPRINTF(szLog,nLen,S_BACKUP_WRITE_STATUS_FAIL,
                        m_nTaskID,szSrcFileName);
                    szLog[nLen - 1] = '\0';
                    CWorkModule::WriteLog(MTS_BACKUP,szLog,SLog::OPER_FAIL);
                    MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,szLog);
                    bRet = FALSE;
                    break; //���ⵥ18680�޸�
                }
                SET<STRING> file_list;
                for(int j = 0; j < m_CurStatus.nFileCount; j++)
                {
                    file_list.insert(m_CurStatus.szFileName[j]);
                }
                CNetBackupFilePool::GetInstance()->UpdateFilePoolTime(
                                                   m_nFilePoolID,
                                                   m_nTaskID,
                                                   m_CurStatus.tFileTime,
                                                   file_list);
                //���±���״̬
                NodeBak = m_CurStatus;
                SNPRINTF(szLog,
                         nLen,
                         S_BACKUP_COPY_BILL,
                         m_nTaskID,
                         szSrcFileName,
                         m_strDestHost.c_str(),
                         szDesFileName);
                szLog[nLen - 1] = '\0';
                CWorkModule::WriteLog(MTS_BACKUP,szLog);
                MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,szLog);

                //д���ݻ�����־
                if(m_bActivateLog)
                {
                    ACE_Guard< ACE_Mutex > ace_mon (g_LogFileMutex);
                    int nRet = SaveCheckLog(Node.szFileName,
                                            szSrcPath,
                                            szDestPath);
                    if(nRet != ERR_SUCCESS)
                    {
                        return ERR_BAK_FAIL;
                    }
                }

                if(m_bDelAfterBak)
                {
                    BOOL nRes = FileDel(szSrcFileName);
                    if(!nRes)
                    {
                        SNPRINTF(szLog,nLen,
                            S_BACKUP_DELETE_FILE_FAILED,
                            m_nTaskID,szSrcFileName);
                        szLog[nLen - 1] = '\0';
                        CWorkModule::WriteLog(MTS_BACKUP,szLog,SLog::OPER_FAIL);
                        TRACE(MTS_BACKUP,szLog);
                        MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,szLog);
                    }
                    else
                    {
                        SNPRINTF(szLog,nLen,
                            S_BACKUP_DELETE_BILL,
                            m_nTaskID,szSrcFileName);
                        szLog[nLen - 1] = '\0';
                        CWorkModule::WriteLog(MTS_BACKUP,szLog);
                        MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,szLog);
                    }
                }

                m_FileList.pop_front();


                if(Sleep(1))
                    return ERR_EVT_EXIT;
                //���ⵥ18680��ɾ��break;
            }
        }
        if(!bRet)
        {
            SInnerAlarm sAlarm;
            sAlarm.uAlarmID = ALARM_ID_BACKUP_FAIL; //ALARM_SEND_FAIL
            sAlarm.yAlarmType = AT_EVENT;
            sAlarm.yAlarmPara[0] = m_nTaskID;
#ifdef _UNIT_TEST
            printf("Send Alarm(ID=%d,Type=%d,Para=%d)",sAlarm.uAlarmID,sAlarm.yAlarmType,sAlarm.yAlarmPara);
#else
            CWorkModule::SendAlarm(&sAlarm);
#endif
            TRACE(MTS_BACKUP,
                  S_BACKUP_FILE_FAILED,
                  m_nTaskID,
                  szSrcFileName,
                  m_strDestHost.c_str(),
                  szDesFileName);
            SNPRINTF(szLog,
                     nLen,
                     S_BACKUP_FILE_FAILED,
                     m_nTaskID,
                     szSrcFileName,
                     m_strDestHost.c_str(),
                     szDesFileName);
            szLog[nLen - 1] = '\0';
            CWorkModule::WriteLog(MTS_BACKUP,szLog,SLog::OPER_FAIL);
            MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_URGENT,szLog);  //�����Լ���Ӵ�Ҫ��Ϊ���� ZhengYuqun SWPD01995 2003-10-21
            return ERR_BAK_FAIL;
        }
    }
    return ERR_SUCCESS;
}

int CNetBakTask::MakeNewCheckLogFile()
{
    //���ɻ�����־�ļ���
    time_t tNow;
    time(&tNow);
    tm tmTime = *localtime(&tNow);
    char szDateString[MAX_PATH];
    strftime(szDateString,
             sizeof(szDateString),
             "%Y%m%d",
             &tmTime);

    STRING strLogFileName = m_strLogSaveDir;
    strLogFileName += "/";
    strLogFileName += m_strTaskLabel;
    strLogFileName += "_";
    strLogFileName += szDateString;
    strLogFileName += ".log";

    FILE* f = fopen(strLogFileName.c_str(), "rb+");
    if(NULL == f)
    //�򲻿�������ļ���˵���ѿ���
    {
        //ȡ�������־�ļ���
        time_t tTemp = tNow - 24L*3600;
        tm tmYesTime = *localtime(&(tTemp));
        strftime(szDateString,
                 sizeof(szDateString),
                 "%Y%m%d",
                 &tmYesTime);
        STRING strYesLogFileName = m_strLogSaveDir;
        strYesLogFileName += "/";
        strYesLogFileName += m_strTaskLabel;
        strYesLogFileName += "_";
        strYesLogFileName += szDateString;
        strYesLogFileName += ".log";

        FILE* fLast = fopen(strYesLogFileName.c_str(), "rb+");
        if(NULL != fLast)
        {
            //ȡ��ʼʱ����Ϣ�������ļ��ܳ���
            char szOutMsg[4096]       = "";
            char szCurTime[100]       = "";
            char szBeginTime[100]     = "";
            char szFileName[MAX_PATH] = "";
            UINT4 uTotalFileLen       = 0;
            UINT4 uFileCount          = 0;
            BOOL  bHasSumLogMsg       = FALSE;

            fseek(fLast, 0, SEEK_SET);
            while(!feof(fLast))
            {
                long lFileLen = 0;

                szOutMsg[0] = '\0';
                fgets(szOutMsg, sizeof(szOutMsg), fLast);
                if((szOutMsg[0] == '\0')
                   || (szOutMsg[0] == '\n')
                   || (szOutMsg[0] == '\r'))
                {
                    continue;
                }

                lFileLen     = 0;
                szCurTime[0] = '\0';
                sscanf(szOutMsg,
                       "%s\t%s\t%d",
                       szCurTime,
                       szFileName,
                       &lFileLen);

                if(szCurTime[0] == '\0')
                {
                    tmYesTime.tm_hour = 0;
                    tmYesTime.tm_min  = 0;
                    tmYesTime.tm_sec  = 0;
                    strftime(szCurTime,
                             sizeof(szCurTime),
                             "%Y%m%d%H%M%S",
                             &tmYesTime);
                }

                if(szBeginTime[0] == '\0')
                {
                    strncpy(szBeginTime,
                            szCurTime,
                            sizeof(szBeginTime));
                    szBeginTime[sizeof(szBeginTime) - 1] = '\0';
                }

                //�ж��Ƿ����л�����־��Ϣ
                if(strlen(szCurTime) < 14)
                {
                    bHasSumLogMsg = TRUE;
                    break;
                }

                if(lFileLen > 0)
                {
                    uTotalFileLen += lFileLen;
                }

                uFileCount++;
            }

            if(szBeginTime[0] == '\0')
            {
                tmYesTime.tm_hour = 0;
                tmYesTime.tm_min  = 0;
                tmYesTime.tm_sec  = 0;
                strftime(szBeginTime,
                         sizeof(szBeginTime),
                         "%Y%m%d%H%M%S",
                         &tmYesTime);
            }

            if(szCurTime[0] == '\0')
            {
                tmYesTime.tm_hour = 23;
                tmYesTime.tm_min  = 59;
                tmYesTime.tm_sec  = 59;
                strftime(szCurTime,
                         sizeof(szCurTime),
                         "%Y%m%d%H%M%S",
                         &tmYesTime);
            }

            if(!bHasSumLogMsg)
            {
                //д��־�ܽ���Ϣ
                char szSumLogMsg[4096];
                SNPRINTF(szSumLogMsg,
                         sizeof(szSumLogMsg),
                         "%u\t%u\t%s\t%s\t%s\t%s\n",
                         uFileCount,
                         uTotalFileLen,
                         szBeginTime,
                         szCurTime,
                         m_strLogSaveDir.c_str(),
                         m_strDestLogSaveDir.c_str());
                fseek(fLast, 0, SEEK_END);
                fputs(szSumLogMsg, fLast);
                fflush(fLast);
            }
            fclose(fLast);

            //�����ļ�������ԴĿ¼
            STRING strDesLogFile = m_strDestLogSaveDir;
            strDesLogFile += "/";
            strDesLogFile += m_strTaskLabel;
            strDesLogFile += "_";
            strDesLogFile += szDateString;
            strDesLogFile += ".log";

            int nRet = m_pFtpClient[m_nActiveIndex]->SetDirectory(m_strDestLogSaveDir.c_str());
            if(nRet != ERR_SUCCESS)
            {
                BOOL bRes = MakeDir(m_strDestLogSaveDir.c_str());
                if(!bRes)
                {
                    TRACE(MTS_BACKUP,S_BACKUP_CREATE_DIR_FAIL,m_nTaskID,m_strDestLogSaveDir.c_str());
                    MSGOUT(MTS_BACKUP,
                           MSGOUT_LEVEL_URGENT,        //�����Լ���Ӵ�Ҫ��Ϊ���� ZhengYuqun SWPD01995 2003-10-21
                           S_BACKUP_CREATE_DIR_FAIL,
                           m_nTaskID,
                           m_strDestLogSaveDir.c_str());
                }
            }
            BOOL bRet = PutAFile(strYesLogFileName.c_str(), strDesLogFile.c_str());
            if(!bRet)
            {
                return ERR_FAIL;
            }
        }

        //ɾ��90��ǰ����־�ļ�
        tTemp = tNow - 24L*3600*90;
        tm tmDelLogTime = *localtime(&(tTemp));
        strftime(szDateString,
                 sizeof(szDateString),
                 "%Y%m%d",
                 &tmDelLogTime);
        STRING strDelLogFileName = m_strLogSaveDir;
        strDelLogFileName += "/";
        strDelLogFileName += m_strTaskLabel;
        strDelLogFileName += "_";
        strDelLogFileName += szDateString;
        strDelLogFileName += ".log";
        remove(strDelLogFileName.c_str());

        //�����������־�ļ�
        f = fopen(strLogFileName.c_str(), "wb+");
    }

    fclose(f);

    return ERR_SUCCESS;
}

int CNetBakTask::SaveCheckLog(const char* szFileName,
                              const char* szSrcPath,
                              const char* szDestPath)
{
    const char SZ_LOG_FILE_POSTFIX[] = ".log";

    //���ɻ�����־�ļ���
    time_t tNow;
    time(&tNow);
    tm tmTime = *localtime(&tNow);
    char szDateString[MAX_PATH];
    strftime(szDateString,
             sizeof(szDateString),
             "%Y%m%d",
             &tmTime);

    STRING strLogFileName = m_strLogSaveDir;
    strLogFileName += "/";
    strLogFileName += m_strTaskLabel;
    strLogFileName += "_";
    strLogFileName += szDateString;
    strLogFileName += SZ_LOG_FILE_POSTFIX;

    //д������־
    char szLogMsg[4096];
    strftime(szDateString,
             sizeof(szDateString),
             "%Y%m%d%H%M%S",
             &tmTime);
    char szSrcFileName[MAX_PATH];
    SNPRINTF(szSrcFileName,MAX_PATH,"%s/%s",szSrcPath,szFileName);
    szSrcFileName[MAX_PATH - 1] = 0;
    SNPRINTF(szLogMsg,
             sizeof(szLogMsg),
             "%s\t%s\t%d\t%s\t%s\t%s\n",
             szDateString,
             szFileName,
             FileSize(szSrcFileName),
             m_strTaskLabel.c_str(),
             szSrcPath,
             szDestPath);
    long lStrLen = strlen(szLogMsg);

    FILE* f = fopen(strLogFileName.c_str(), "rb+");
    if(NULL == f)
    //�򲻿�������ļ���˵���ѿ���
    {
        int nRet = MakeNewCheckLogFile();
        if(nRet != ERR_SUCCESS)
        {
            return nRet;
        }

        //�����������־�ļ�
        f = fopen(strLogFileName.c_str(), "wb+");
    }

    if(NULL != f)
    {
        if(ACE_OS::strcasecmp(&szFileName[strlen(szFileName) - strlen(SZ_LOG_FILE_POSTFIX)],
                              SZ_LOG_FILE_POSTFIX) != 0)
        {
            //ȡ���һ����־��Ϣ
            char szOutMsg[4096] = "";
            fseek(f, -(lStrLen), SEEK_END);
            fgets(szOutMsg, sizeof(szOutMsg), f);

            //�Ƚ����һ����־���������ͬ��׷��
            const char* p1 = strchr(szOutMsg, '\t');
            const char* p2 = strchr(szLogMsg, '\t');

            if((p1 != NULL) && (p2 != NULL))
            {
                p1++;
                p2++;
                if(strcmp(p1, p2) != 0)
                {
                    fseek(f, 0, SEEK_END);
                    fputs(szLogMsg, f);
                    fflush(f);
                }
            }
            else
            {
                fseek(f, 0, SEEK_END);
                fputs(szLogMsg, f);
                fflush(f);
            }
        }
        fclose(f);

        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

int CBakTask::MakeNewCheckLogFile()
{
    return ERR_SUCCESS;
}

int CBakTask::SaveCheckLog(const char* szFileName,
                           const char* szSrcPath,
                           const char* szDestPath)
{
    return ERR_SUCCESS;
}

/*****************************************************************************
�� �� ��  : CBakTask::Backup
��������  : ����FTP Server�������������ļ�
�������  : ��
�������  : ��
�� �� ֵ  : void
���ú���  :
��������  :

  �޸���ʷ      :
  1.��    ��   : 2002��9��6��
  ��    ��   : ������
  �޸�����   : �����ɺ���

*****************************************************************************/
void CBakTask::Backup()
{
    while(1)
    {
        if (m_pBakTaskMgr && m_pBakTaskMgr->GetLicenseCount() == 0)
        {
            TRACE(MTS_BACKUP, S_NO_BACKUP_LICENSE);
            if (Sleep(30))  //��License�����ߣ�����
            {
                break;
            }
            continue;
        }

        try{
            m_LastBakOKTime = time(NULL);
            LIST2<STRING> SubDirList;
            DelDestDirOrFile();
            PrepareSubDirs(m_szSrcDir, SubDirList);
            while(!SubDirList.empty())
            {
                STRING szCurDir = SubDirList.front();
                int nRet = ERR_SUCCESS;
                if(SubDirList.size() == 1 && m_bBakLastFile == FALSE)
                {
                    nRet = PrepareFiles(szCurDir.c_str(),m_CurStatus.tFileTime,TRUE);
                }
                else
                {
                    nRet = PrepareFiles(szCurDir.c_str(),m_CurStatus.tFileTime,FALSE);
                }
                if(nRet != ERR_SUCCESS || m_FileList.empty())
                {
                    MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
                        S_BACKUP_NO_FILE_IN_DIR,szCurDir,
                        m_nTaskID);
                    if(m_FileList.empty())
                        SubDirList.pop_front();

                    continue;
                }

                char szDesDir[MAX_PATH];
                SNPRINTF(szDesDir,MAX_PATH,"%s%s",m_szDestDir,szCurDir.substr(strlen(m_szSrcDir)).c_str());
                szDesDir[MAX_PATH - 1] = 0;
                BOOL bRet = TRUE;
                bRet = CreateDeepDir(szDesDir);
                if(bRet == FALSE)
                {
                    TRACE(MTS_BACKUP,S_BACKUP_CREATE_DIR_FAIL,m_nTaskID,szDesDir);
                    SleepThrowException(m_nSearchFilesInterval);
                }
                MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
                    S_BACKUP_CRE_DEST_DIR,szDesDir,m_nTaskID);
                nRet = PutFiles(szCurDir.c_str(),szDesDir);
                if(nRet == ERR_SUCCESS)
                {
                    SubDirList.pop_front();
                    ClearList();
                    continue;
                }
                else if(nRet == ERR_EVT_EXIT)
                {
                    throw ExitException("ERR_EVT_EXIT");
                }
                else if(nRet == ERR_BAK_FAIL)
                {
                    throw ContinueException("ERR_BAK_FAIL");
                }
                else
                {
                    throw ExitException("Unknown Error");
                }
            }//while
            //ɾ��Դ�ļ��Ĺ����Ѿ��Ƶ�ǰ�������
            SleepThrowException(m_nSearchFilesInterval);
        }//try
        catch(ContinueException)
        {
            DelOutDateFile();
            ClearList();
        }
        catch(...)
        {
            MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
                S_BACKUP_END_TASK,m_nTaskID);
            ClearList();
            m_bIsExit = TRUE;
            return;
        }
    }//while(1)
}
/*****************************************************************************
�� �� ��  : CBakTask::Sleep
��������  : ˯��nSeconds�룬��˯���еȴ��˳��¼�
�������  : int nSeconds
�������  : ��
�� �� ֵ  : BOOL
���ú���  :
��������  :

�޸���ʷ  :
      1.��    ��   : 2002��9��6��
      ��    ��   : ������
      �޸�����   : �����ɺ���

*****************************************************************************/
BOOL CBakTask::Sleep(int nSeconds)
{
    ACE_Time_Value Seconds(nSeconds);
    Seconds += ACE_OS::gettimeofday ();
    //MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
    //    S_BACKUP_SLEEP_INTERVAL,
    //    nSeconds,m_nTaskID);
    int nRet = m_BakThdExitEvt.wait(&Seconds);

    return (nRet == 0);
}
void CBakTask::SleepThrowException(int nSeconds)
{
    if(Sleep(nSeconds))
    {
        throw ExitException("Sleep return TRUE");
    }
    else
    {
        throw ContinueException("Sleep return FALSE");
    }
}
/*****************************************************************************
�� �� ��  : CBakTask::PutAFile
��������  : ����һ���ļ�
�������  : const char* pszLocalFile
            const char* pszRemoteFile
�������  : ��
�� �� ֵ  : BOOL
���ú���  :
��������  :

�޸���ʷ  :
      1.��    �� : 2002��9��6��
        ��    �� : ������
        �޸����� : �����ɺ���

*****************************************************************************/
BOOL CBakTask::PutAFile(const char* pszLocalFile, const char* pszRemoteFile)
{
    return FileCopy(pszLocalFile,pszRemoteFile);
}
/*****************************************************************************
�� �� ��  : CBakTask::DelDestDirOrFile
��������  : ɾ�����ڱ����ļ�����Ŀ¼
�������  : ��
�������  : ��
�� �� ֵ  : int
���ú���  :
��������  :

�޸���ʷ  :
      1.��    ��   : 2002��9��6��
        ��    ��   : ������
        �޸�����   : �����ɺ���

*****************************************************************************/
int CBakTask::DelDestDirOrFile()
{
    return DelDirOrFile_Impl(m_szDestDir,m_nDesFileSavedDays);
}

int CBakTask::DelDirOrFile_Impl(const char *pszName,int nSaveDays)
{
    if(nSaveDays <= 0)
    {
        return ERR_SUCCESS;
    }
    //��szPathĿ¼
    ACE_DIR *pDir = NULL;
    dirent *pDirent = NULL;
    ACE_stat sDirStat;
    //���ļ�Ŀ¼
    pDir = ACE_OS::opendir(pszName);
    if(pDir == NULL)
    {
		//by ����	2004-5-29   ��Ӧ���ⵥSWPD05427
        char szTmpBuf[BK_MAX_INFO_LEN];
        sprintf(szTmpBuf, "Errno: %d (%s)", errno, strerror(errno));
        STRING sTmpInfo = S_BACKUP_OPEN_DIR_FAIL;
        sTmpInfo += STRING(" ") + pszName + STRING(" ");
        MSGOUT(MTS_BACKUP, MSGOUT_LEVEL_SIDLY, sTmpInfo.c_str());
        sTmpInfo += szTmpBuf;
        TRACE(MTS_BACKUP, sTmpInfo.c_str());
		//the end	2004-5-19	��Ӧ���ⵥSWPD05427

        return ERR_FAIL;
    }
    MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
        S_BACKUP_BEGIN_DEL_DEST_FILE,
        pszName,m_nTaskID);
    char szPath[MAX_PATH];
    char szCurTime[32];
    char szFileTime[32];
    time_t curTime,DirTime;
    tm tTmp;
    memset(&tTmp,0,sizeof(tm));

    while((pDirent = ACE_OS::readdir(pDir)) != NULL)
    {
        SNPRINTF(szPath,MAX_PATH,"%s/%s",pszName,pDirent->d_name);
        szPath[MAX_PATH - 1] = 0;

        if(strcmp(pDirent->d_name,".") == 0
            || strcmp(pDirent->d_name,"..") == 0)
        {
            continue;
        }

        ACE_OS::stat(szPath, &sDirStat);
        int nRes = ERR_SUCCESS;
        int nElapsed = 0;
        if(S_ISDIR(sDirStat.st_mode) && IsDigitValue(pDirent->d_name))
        {
            int nDateNum = ACE_OS::atoi(pDirent->d_name);
            tTmp.tm_year = (nDateNum / 10000) - 1900;
            tTmp.tm_mon = (nDateNum % 10000)/100 - 1;
            tTmp.tm_mday = nDateNum %100;
            DirTime = mktime(&tTmp); //�õ�Ŀ¼ʱ��Ļ�׼ֵ
            //ȡ��ǰʱ��
            time(&curTime);
            tTmp = *localtime(&curTime);
            tTmp.tm_hour = 0;
            tTmp.tm_min = 0;
            tTmp.tm_sec = 0;
            curTime = mktime(&tTmp); //�õ���ǰ�����ڣ�ʱ���붼Ϊ0
            nElapsed = difftime(curTime,DirTime);
            strftime(szCurTime,32,"%Y-%m-%d",&tTmp);
            MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
                S_BACKUP_CUR_TIM_DIR_NAME,
                szCurTime,pDirent->d_name,m_nTaskID);
            if(nElapsed >= (nSaveDays * ONE_DAY_IN_SEC))
            {
                if(SweepDirectory(szPath))
                {
                    MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
                        S_BACKUP_DEL_DIR,
                        szPath,S_BACKUP_FAILURE,
                        m_nTaskID);
                    nRes = ERR_FAIL;
                }
                else
                {
                    MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
                        S_BACKUP_DEL_DIR,
                        szPath,S_BACKUP_SUCCESS,
                        m_nTaskID);
                }
            }
        }
        else if(!S_ISDIR(sDirStat.st_mode))
        {
            time(&curTime);
            tTmp = *localtime(&curTime);
            strftime(szCurTime,32,"%Y-%m-%d",&tTmp);
            tTmp = *localtime(&(sDirStat.st_ctime));
            strftime(szFileTime,32,"%Y-%m-%d",&tTmp);
            nElapsed = difftime(curTime,sDirStat.st_ctime);
            MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
                S_BACKUP_CUR_TIM_DIR_NAME,
                szCurTime,szFileTime,m_nTaskID);
            if(nElapsed >= (nSaveDays * ONE_DAY_IN_SEC))
            {
                if(!FileDel(szPath))
                {
                    MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
                        S_BACKUP_DEL_DIR,
                        szPath,S_BACKUP_FAILURE,m_nTaskID);
                    nRes = ERR_FAIL;
                }
                else
                {
                    MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
                        S_BACKUP_DEL_DIR,
                        szPath,S_BACKUP_SUCCESS,m_nTaskID);
                }
            }
        }
        if(nRes)
        {
            ACE_OS::closedir(pDir);
            return nRes;
        }

    }
    //�ر�Ŀ¼
    ACE_OS::closedir(pDir);
    return ERR_SUCCESS;
}
/*****************************************************************************
�� �� ��  : CBakTask::GetCfg
��������  : �ļ��л�ȡ������������Ĳ���
�������  : ��
�������  : ��
�� �� ֵ  : BOOL
���ú���  :
��������  :

�޸���ʷ  :
   1.��    ��   : 2002��9��6��
     ��    ��   : ������
     �޸�����   : �����ɺ���

*****************************************************************************/
BOOL CBakTask::GetCfg()
{
    //��INI�ļ���
    CINIFile ini(GetCfgFilePath());
    if(ini.Open() != TRUE)
    {
        TRACE(MTS_BACKUP,S_BACKUP_INI_OPEN_FAIL);
        return FALSE;
    }
    //��INI�ļ���[NetBackup]�������ζ�ȡ���²���������
    m_nDesFileSavedDays = ini.GetInt(CFG_SEC_NET_BACKUP,
        CFG_NET_BACKUP_DES_SAVE_DAYS,
        CFG_NET_BACKUP_DES_SAVE_DAYS_DEFAULT);

    m_nSearchFilesInterval = ini.GetInt(CFG_SEC_NET_BACKUP,
        CFG_NET_BACKUP_SEARCH_INTERVAL,
        CFG_NET_BACKUP_SEARCH_INTERVAL_DEFAULT);

    ini.GetString(CFG_SEC_NET_BACKUP,CFG_NET_BACKUP_SRC_ROOT_DIR,
        CFG_NET_BACKUP_SRC_ROOT_DIR_DEFAULT,
        m_szSrcDir,MAX_PATH);
    ini.GetString(CFG_SEC_NET_BACKUP,CFG_NET_BACKUP_DES_ROOT_DIR,
        CFG_NET_BACKUP_DES_ROOT_DIR_DEFAULT,
        m_szDestDir,MAX_PATH);

    m_bDelAfterBak = ini.GetInt(CFG_SEC_NET_BACKUP,
        CFG_NET_BACKUP_DEL_AFTER_BAK,
        CFG_NET_BACKUP_DEL_AFTER_BAK_DEFAULT);

    m_bBakLastFile = ini.GetInt(CFG_SEC_NET_BACKUP,
        CFG_NET_BACKUP_LAST_FILE,
        CFG_NET_BACKUP_LAST_FILE_DEFAULT);

    m_nBakAlmTimeout = ini.GetInt(CFG_SEC_NET_BACKUP,
        CFG_NET_BACKUP_ALARM_TIME_OUT,
        CFG_NET_BACKUP_ALARM_TIME_OUT_DEFAULT);
    
//----------------------------add  timer backup
    char szBeginBuf[20];
    ini.GetString(CFG_SEC_NET_BACKUP,
        CFG_NET_BEGIN_TIME,
        CFG_NET_BEGIN_TIME_DEFAULT,
        szBeginBuf,
        sizeof(szBeginBuf));
    m_nBeginTime = GetTimeFromString(szBeginBuf);
    
    char szEndBuf[20];
    ini.GetString(CFG_SEC_NET_BACKUP,
        CFG_NET_END_TIME,
        CFG_NET_END_TIME_DEFAULT,
        szEndBuf,
        sizeof(szEndBuf));
    m_nEndTime   = GetTimeFromString(szEndBuf);
       
    //added by chenliangwei
    char szListTimeBuf[1024];
    ini.GetString(CFG_SEC_NET_BACKUP,
        CFG_NET_LIST_TIME,
        CFG_NET_LIST_TIME_DEFAULT,
        szListTimeBuf,
        sizeof(szListTimeBuf));
    //add end
    
    //added by chenliangwei, 2003-08-05
    char szOnceListTimeBuf[1024];
    ini.GetString(CFG_SEC_NET_BACKUP,
        CFG_NET_ONCE_LIST_TIME,
        CFG_NET_ONCE_LIST_TIME_DEFAULT,
        szOnceListTimeBuf,
        sizeof(szOnceListTimeBuf));
    //add end
//-----------------------------------------add timer backup 
    //������ 2003-04-07
    m_uDelFileDays = ini.GetInt(CFG_SEC_NET_BACKUP,
                                CFG_NET_BACKUP_DEL_FILE_DAYS,
                                CFG_NET_BACKUP_DEL_FILE_DAYS_DEFAULT);
    char szBackupSec[16];
    //��INI�ļ���[BackupTaskN]��NΪ��������ID��
    //�������ζ�ȡ������������������
    //�������Ĭ�����ã�������������������Ϊ׼
    SNPRINTF(szBackupSec,16,"%s%d",CFG_NET_BACKUP,m_nTaskID);
    szBackupSec[15] = 0;
    //������ 2003-04-07

//-----------------------------------------------------add timer backup
    ini.GetString(szBackupSec,
                  CFG_NET_BEGIN_TIME,
                  szBeginBuf,
                  szBeginBuf,
                  sizeof(szBeginBuf));
    m_nBeginTime = GetTimeFromString(szBeginBuf);
    
    ini.GetString(szBackupSec,
                  CFG_NET_END_TIME,
                  szEndBuf,
                  szEndBuf,
                  sizeof(szEndBuf));
    m_nEndTime   = GetTimeFromString(szEndBuf);
    
    //added by chenliangwei
    ini.GetString(szBackupSec,
                  CFG_NET_LIST_TIME,
                  szListTimeBuf,
                  szListTimeBuf,
                  sizeof(szListTimeBuf));
    //add end

    if (szListTimeBuf[0] != '\0')
    {
        char *pToken = strtok(szListTimeBuf, ",");
        while(pToken)
        {
            int nTime = GetTimeFromString(pToken);
            
            bool bFind = false;
            LIST<int>::iterator iter = m_ListTime.begin();
            for(; iter != m_ListTime.end(); iter++)
            {
                if(nTime == (*iter))
                {
                    bFind = true;
                    break;
                }
            }
            if(!bFind) m_ListTime.push_back(nTime);
            pToken = strtok(NULL, ",");
        }
    }
    
    m_ListTime.sort();    
    
    //added by chenliangwei, 2003-08-05
    ini.GetString(szBackupSec,
                  CFG_NET_ONCE_LIST_TIME,
                  szOnceListTimeBuf,
                  szOnceListTimeBuf,
                  sizeof(szOnceListTimeBuf));
    //add end
    
    //����һ��������
    if (szOnceListTimeBuf[0] != '\0')
    {    
        //��ʱ�����е�������','
        int nstrColLen = strlen(szOnceListTimeBuf);
        if (szOnceListTimeBuf[nstrColLen - 1] != ',')
        {
            szOnceListTimeBuf[nstrColLen] = ',';
            szOnceListTimeBuf[nstrColLen + 1] = '\0';
        }
        
        //���һ����ʱ�����У�ע:Լ��ʱ�����зָ���Ϊ'^'
        char *ss0 = NULL;
        char *ss1 = NULL;
        ss0 = szOnceListTimeBuf;
        while ((ss1 = strchr(ss0, ',')) != NULL)
        {
            *ss1 = '\0';
            
            //��ʱ���ʽ(YYYY-MM-DD HH:mm:SS)ת��Ϊtime_t
            tm tmTime = {0};
            const char *pToken = strtok(ss0, "-");
            ss0 = ss1+1;
            
            if(pToken == NULL)
            {
                continue;
            }
            tmTime.tm_year = atoi(pToken) - 1900;
            
            pToken = strtok(NULL, "-");
            if(pToken == NULL)
            {
                continue;
            }
            tmTime.tm_mon = atoi(pToken) - 1;
            
            pToken = strtok(NULL, " ");
            if(pToken == NULL)
            {
                continue;
            }
            tmTime.tm_mday = atoi(pToken);
            
            pToken = strtok(NULL, ":");
            if(pToken == NULL)
            {
                continue;
            }
            tmTime.tm_hour = atoi(pToken);
            
            pToken = strtok(NULL, ":");
            if(pToken == NULL)
            {
                continue;
            }
            tmTime.tm_min = atoi(pToken);
            tmTime.tm_sec = 0;
            
            time_t tTime = mktime(&tmTime);
            
            //m_MutexForSchl.acquire();
            
            bool bFind = false;
            LIST<time_t>::iterator it = m_OnceListTime.begin();
            for(; it != m_OnceListTime.end(); it++)
            {
                if(tTime == (*it))
                {
                    bFind = true;
                    break;
                }
            }
            
            if(!bFind) 
            {
                //����ǹ�ʱ��ʱ�䣬�����뵽һ���Բɼ�������
                if (tTime >= time(NULL))
                {
                    m_OnceListTime.push_back(tTime);
                }
            }
            
            //m_MutexForSchl.release();
        }
        
        //m_MutexForSchl.acquire();
        m_OnceListTime.sort();
        SaveOnceColSchl();
        //m_MutexForSchl.release();
    }
    //added end

//-----------------------------------add timer backup

    m_uDelFileDays = ini.GetInt(szBackupSec,
                                CFG_NET_BACKUP_DEL_FILE_DAYS,
                                m_uDelFileDays);
    if(m_uDelFileDays > 180)
    {
        m_uDelFileDays = 180;
    }
    //���ӽ���

    m_nFilePoolID = ini.GetInt(szBackupSec,
                               CFG_NET_BACKUP_FILE_POOL_ID,
                               CFG_NET_BACKUP_FILE_POOL_ID_DEFAULT);

    m_nDesFileSavedDays = ini.GetInt(szBackupSec,
        CFG_NET_BACKUP_DES_SAVE_DAYS,
        m_nDesFileSavedDays);
    if(m_nDesFileSavedDays > 365)
    {
        m_nDesFileSavedDays = 365;
        TRACE(MTS_BACKUP,S_BACKUP_EXCEED_LIMIT,m_nTaskID,CFG_NET_BACKUP_DES_SAVE_DAYS,365);
    }

    m_nSearchFilesInterval = ini.GetInt(szBackupSec,
        CFG_NET_BACKUP_SEARCH_INTERVAL,
        m_nSearchFilesInterval);
    if(m_nSearchFilesInterval > 604800) //7 days
    {
        m_nSearchFilesInterval = 604800;
        TRACE(MTS_BACKUP,S_BACKUP_EXCEED_LIMIT,m_nTaskID,CFG_NET_BACKUP_SEARCH_INTERVAL,604800);
    }
    else if(m_nSearchFilesInterval < 1)
    {
        m_nSearchFilesInterval = 1;
        TRACE(MTS_BACKUP,S_BACKUP_EXCEED_LIMIT,m_nTaskID,CFG_NET_BACKUP_SEARCH_INTERVAL,30);
    }

    ini.GetString(szBackupSec,CFG_NET_BACKUP_SRC_ROOT_DIR,
        m_szSrcDir,
        m_szSrcDir,MAX_PATH);
    ini.GetString(szBackupSec,CFG_NET_BACKUP_DES_ROOT_DIR,
        m_szDestDir,
        m_szDestDir,MAX_PATH);

    if(m_szDestDir[0] == '\0')
    {
        TRACE(MTS_BACKUP,
             S_BACKUP_DEST_DIR_NOT_CONF,
             m_nTaskID);

        return FALSE;
    }

    m_bDelAfterBak = ini.GetInt(szBackupSec,
        CFG_NET_BACKUP_DEL_AFTER_BAK,
        m_bDelAfterBak);
    if(m_bDelAfterBak != 0 && m_bDelAfterBak != 1)
    {
        m_bDelAfterBak = 1;
        TRACE(MTS_BACKUP,S_BACKUP_EXCEED_LIMIT,m_nTaskID,CFG_NET_BACKUP_DEL_AFTER_BAK,1);
    }

    m_bBakLastFile = ini.GetInt(szBackupSec,
        CFG_NET_BACKUP_LAST_FILE,
        m_bBakLastFile);
    if(m_bBakLastFile != 0 && m_bBakLastFile != 1)
    {
        m_bBakLastFile = 1;
        TRACE(MTS_BACKUP,S_BACKUP_EXCEED_LIMIT,m_nTaskID,CFG_NET_BACKUP_LAST_FILE,1);
    }

    m_nBakAlmTimeout = ini.GetInt(szBackupSec,
        CFG_NET_BACKUP_ALARM_TIME_OUT,
        m_nBakAlmTimeout);
    if(m_nBakAlmTimeout > 605000)
    {
        m_nBakAlmTimeout = 605000;
        TRACE(MTS_BACKUP,S_BACKUP_EXCEED_LIMIT,m_nTaskID,CFG_NET_BACKUP_ALARM_TIME_OUT,605000);
    }
    else if(m_nBakAlmTimeout < 60)
    {
        m_nBakAlmTimeout = 60;
        TRACE(MTS_BACKUP,S_BACKUP_EXCEED_LIMIT,m_nTaskID,CFG_NET_BACKUP_ALARM_TIME_OUT,60);
    }

    char szID[16];
    SNPRINTF(szID,16,"%d",m_nTaskID);

    m_bActivateLog = ini.GetInt(CFG_SEC_NET_BACKUP,
                                CFG_CHECK_LOG_ENABLE,
                                CFG_CHECK_LOG_ENABLE_DEFAULT);
    m_bActivateLog = ini.GetInt(szBackupSec,
                                CFG_CHECK_LOG_ENABLE,
                                m_bActivateLog);
    if(m_bActivateLog)
    {
        char szTemp[MAX_PATH] = "";
        ini.GetString(CFG_SEC_NET_BACKUP,
                      CFG_CHECK_TASK_LABEL,
                      CFG_CHECK_TASK_LABEL_DEFAULT,
                      szTemp,
                      sizeof(szTemp));
        ini.GetString(szBackupSec,
                      CFG_CHECK_TASK_LABEL,
                      szTemp,
                      szTemp,
                      sizeof(szTemp));

        m_strTaskLabel = szTemp;

        ini.GetString(CFG_SEC_NET_BACKUP,
                      CFG_CHECK_LOG_DEST_PATH,
                      CFG_CHECK_LOG_DEST_PATH_DEFAULT,
                      szTemp,
                      sizeof(szTemp));
        ini.GetString(szBackupSec,
                      CFG_CHECK_LOG_DEST_PATH,
                      szTemp,
                      szTemp,
                      sizeof(szTemp));
        if(szTemp[0] == '\0')
        {
            TRACE(MTS_BACKUP,
                 S_BACKUP_DEST_DIR_NOT_CONF,
                 m_nTaskID);

            return FALSE;
        }
        m_strDestLogSaveDir = szTemp;

        char szStatusFileName[MAX_PATH];
        ini.GetString(CFG_SEC_NET_BACKUP,
                      CFG_BACKUP_STATUS_FILE_DIR,
                      CFG_BACKUP_STATUS_SRC_DIR_DEFAULT,
                      szStatusFileName,
                      sizeof(szStatusFileName));
        szStatusFileName[sizeof(szStatusFileName) - 1] = '\0';
        m_strLogSaveDir = szStatusFileName;

        if(m_strTaskLabel == "")
        {
            m_strTaskLabel = szID;
        }
        m_strLogSaveDir += "/";
        m_strLogSaveDir += m_strTaskLabel;
        CreateDeepDir(m_strLogSaveDir.c_str());
    }

    GetStatInfo(szID,&m_CurStatus);
    m_CurStatus.nFileCount = min(m_CurStatus.nFileCount, MAX_FILE_COUNT);
    if(m_CurStatus.nFileCount < 0)
    {
        m_CurStatus.nFileCount = 0;
    }
    strncpy(m_CurStatus.szPath,szID,16);
    //�ر�INI�ļ�
    ini.Close();

    SET<STRING> file_list;
    for(int j = 0; j < m_CurStatus.nFileCount; j++)
    {
        file_list.insert(m_CurStatus.szFileName[j]);
    }
    CNetBackupFilePool::GetInstance()->UpdateFilePoolTime(
                                       m_nFilePoolID,
                                       m_nTaskID,
                                       m_CurStatus.tFileTime,
                                       file_list);

    return TRUE;
}
void CBakTask::ClearList()
{
    m_DelFileList.clear();
    m_FileList.clear();
}
void CBakTask::DelOutDateFile()
{
    char szDelFileName[MAX_PATH];
    //ɾ�������ļ�
    for(; !m_DelFileList.empty(); m_DelFileList.pop_front())
    {
        SFilesListNode &Node = m_DelFileList.front();

        SNPRINTF(szDelFileName,MAX_PATH,"%s/%s",m_szSrcDir,Node.szFileName);
        szDelFileName[MAX_PATH - 1] = 0;
        if(!FileDel(szDelFileName))
            TRACE(MTS_BACKUP,S_BACKUP_DEL_FILE_FAILURE,szDelFileName);
        else
            MSGOUT(MTS_BACKUP,
                   MSGOUT_LEVEL_SIDLY,
                   S_BACKUP_DEL_OUTDATE_FILE,
                   szDelFileName);
    }
}

/*****************************************************************************
�� �� ��  : CNetBakTask::CNetBakTask
��������  : ���캯��
�������  : int nTaskID
CBakTaskMgr* pBakTaskMgr
�������  : ��
�� �� ֵ  :
���ú���  :
��������  :

  �޸���ʷ      :
  1.��    ��   : 2002��9��6��
  ��    ��   : ������
  �޸�����   : �����ɺ���

*****************************************************************************/
CNetBakTask::CNetBakTask( int nTaskID, CBakTaskMgr* pBakTaskMgr ):CBakTask(nTaskID,pBakTaskMgr)
{
    m_bIsAvailable = FALSE;
    m_LastConnTime = 0;
    m_nReConnInterval = 300;
    memset(m_szDestHostIP[0],0,MAX_HOST_LEN);      //Ŀ��������Ip��Local�����أ�
    memset(m_szDestHostIP[1],0,MAX_HOST_LEN);      //Ŀ��������Ip��Local�����أ�
    memset(m_szLocalHostIP[0],0,MAX_HOST_LEN);    //������IP
    memset(m_szLocalHostIP[1],0,MAX_HOST_LEN);    //������IP
    memset(m_szUserName,0,MAX_USERNAME_LEN);   //��¼�û���
    memset(m_szPassword,0,MAX_PASSWORD_LEN);   //����
    m_nActiveIndex    = 0;
    m_pFtpClient[0]   = new CFtpClient;
    m_pFtpClient[1]   = new CFtpClient;
    m_strLastSendFile = "";
    m_nResendCount    = 0;
    m_bSendAlarm      = FALSE;   
    m_bTransErrFlg    = FALSE;
    m_nRetryTimesCount  = 0;
}

CNetBakTask::~CNetBakTask()
{
    if(m_pFtpClient[0] != NULL)
    {
        delete m_pFtpClient[0];
        m_pFtpClient[0] = NULL;
    }

    if(m_pFtpClient[1] != NULL)
    {
        delete m_pFtpClient[1];
        m_pFtpClient[1] = NULL;
    }
}


/*****************************************************************************
�� �� ��  : CNetBakTask::Backup
��������  : ����FTP Server�������������ļ�
�������  : ��
�������  : ��
�� �� ֵ  : void
���ú���  :
��������  :

      �޸���ʷ      :
      1.��    ��   : 2002��9��6��
      ��    ��   : ������
      �޸�����   : �����ɺ���

*****************************************************************************/
void CNetBakTask::Backup()
{
    BOOL bRes = TRUE;
    int nRet = 0;
	int nFlag = 0;
    while(WaitNextTime())
    {
        if (m_pBakTaskMgr && m_pBakTaskMgr->GetLicenseCount() == 0)
        {
            TRACE(MTS_BACKUP, S_NO_BACKUP_LICENSE);
            if (Sleep(30))  //��License�����ߣ�����
                break;
            continue;
        }
        try{
            nRet = ERR_SUCCESS;
            bRes = Connect();
            if(!bRes)
            {
                if(!m_bSendAlarm)
                {
                    SInnerAlarm sInner;
                    sInner.uAlarmID = ALARM_ID_BACKUP_CONNECT_FAIL;//����ʧ��
                    sInner.yAlarmPara[0] = m_nTaskID;
                    sInner.yAlarmType = AT_FAULT;
#ifdef _UNIT_TEST
                    printf("Send Alarm(ID=%d,Type=%d,Para=%d)",sInner.uAlarmID,sInner.yAlarmType,sInner.yAlarmPara);
#else
                    CWorkModule::SendAlarm(&sInner);
#endif
                    m_bSendAlarm = TRUE;
                    TRACE(MTS_BACKUP,S_BACKUP_CONNECT_FAIL,m_nTaskID);
                    MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_URGENT,           //�����Լ���Ӵ�Ҫ��Ϊ���� ZhengYuqun SWPD01995 2003-10-21
                        S_BACKUP_CONNECT_FAIL,m_nTaskID);
                }
                
                throw ContinueException("Failed to connect ftp server");
            }
            else
            {
                if(m_bSendAlarm)
                {
                    SInnerAlarm sInner;
                    sInner.uAlarmID = ALARM_ID_BACKUP_CONNECT_FAIL;//����ʧ��
                    sInner.yAlarmPara[0] = m_nTaskID;
                    sInner.yAlarmType = AT_RESTORE;
                    CWorkModule::SendAlarm(&sInner);
                    
                    m_bSendAlarm = FALSE;
                }
            }            

            if(m_bActivateLog)
            {
                //����������־�ļ�
                ACE_Guard< ACE_Mutex > ace_mon (g_LogFileMutex);
                MakeNewCheckLogFile();
            }

            while(m_bIsAvailable == TRUE)
            {
                LIST2<STRING> SubDirList;
                PrepareSubDirs(m_szSrcDir,SubDirList);
                char szRemoteDir[MAX_PATH];
                while(!SubDirList.empty())
                {
                    STRING &strTmpDir = SubDirList.front();
                    if(SubDirList.size() == 1 && m_bBakLastFile == FALSE)
                    {//���ⵥ18680�޸ģ�������2002-20-26
                        nRet = PrepareFiles(strTmpDir.c_str(),m_CurStatus.tFileTime,TRUE);
                    }
                    else
                    {
                        nRet= PrepareFiles(strTmpDir.c_str(),m_CurStatus.tFileTime,FALSE);
                    }
                    //�޸Ľ���
                    if(nRet != ERR_SUCCESS || m_FileList.empty())
                    {
                        if(m_FileList.empty())
                            SubDirList.pop_front();
                        continue;
                    }

                    SNPRINTF(szRemoteDir,MAX_PATH,"%s%s",m_szDestDir,strTmpDir.substr(strlen(m_szSrcDir)).c_str());
                    szRemoteDir[MAX_PATH - 1] = 0;

                    nRet = m_pFtpClient[m_nActiveIndex]->SetDirectory(szRemoteDir);
                    if(nRet)
                    {
                        bRes = MakeDir(szRemoteDir);
                        if(!bRes)
                        {
							if (nFlag == 0)
							{
								TRACE(MTS_BACKUP,S_BACKUP_CREATE_DIR_FAIL,m_nTaskID,szRemoteDir);
								MSGOUT(MTS_BACKUP,
									   MSGOUT_LEVEL_URGENT,        //�����Լ���Ӵ�Ҫ��Ϊ���� ZhengYuqun SWPD01995 2003-10-21
									   S_BACKUP_CREATE_DIR_FAIL,
									   m_nTaskID,szRemoteDir);
								char szLog[200];
								sprintf(szLog, S_BACKUP_CREATE_DIR_FAIL, m_nTaskID, szRemoteDir);
								szLog[199] = '\0';
								CWorkModule::WriteLog(MTS_BACKUP, szLog, SLog::OPER_FAIL);

								SInnerAlarm sAlarm;
								sAlarm.uAlarmID = ALARM_ID_BACKUP_FAIL; 
								sAlarm.yAlarmType = AT_EVENT;
								sAlarm.yAlarmPara[0] = m_nTaskID;
								CWorkModule::SendAlarm(&sAlarm);
								nFlag = 1;
							}

                            throw ContinueException("Failed to make directory");
                        }
						nFlag = 0;
                        nRet = m_pFtpClient[m_nActiveIndex]->SetDirectory(szRemoteDir);
                        if(nRet)
                        {
                            throw ContinueException("Failed to set directory");
                        }
                    }

                    nRet = PutFiles(strTmpDir.c_str(),szRemoteDir);
                    if(nRet == ERR_EVT_EXIT)
                    {
                        throw ExitException("PutFile return ERR_EVT_EXIT");
                    }
                    else if(nRet == ERR_BAK_FAIL)
                    {
                        throw ContinueException("PutFile return ERR_BAK_FAIL");
                    }
                    
                    m_bTransErrFlg = FALSE;
              
                    SubDirList.pop_front();
                } //while
                //ɾ��Դ�ļ��Ĵ����Ѿ��ŵ�ǰ�������
                DelDestDirOrFile();
                SleepThrowException(0);
    
            }//while
        }//try
        catch(ContinueException m){
            if(strcmp(m.what(), "Sleep return FALSE") == 0)
            {
                m_bTransErrFlg = FALSE;
            }
            //�����������
            else 
            {
                //Z30971�޸����ⵥ��SWPD05706 2004/08/23
                TRACE(MTS_BACKUP, "%s(ID=%d)", m.what(), m_nTaskID);
                if (++m_nRetryTimesCount > m_nReTryTimes)
                {
                    m_bTransErrFlg = FALSE;
                    m_nRetryTimesCount = 0;
                    
                    TRACE(MTS_BACKUP,S_BACKUP_CONTINUE_LATER, m_nTaskID, m_nReTryTimes);
                    MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
                        S_BACKUP_CONTINUE_LATER, m_nTaskID, m_nReTryTimes);
					char szLog[200];
					sprintf(szLog, S_BACKUP_CONTINUE_LATER, m_nTaskID, m_nReTryTimes);
					szLog[199] = '\0';
					CWorkModule::WriteLog(MTS_BACKUP, szLog, SLog::OPER_FAIL);
                }
                else
                {
                    m_bTransErrFlg = TRUE;
                    TRACE(MTS_BACKUP,S_BACKUP_RETRY, m_nTaskID,
                        m_nRetryTimesCount, m_nRetryTimesCount - 1);
                    MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY, S_BACKUP_RETRY, m_nTaskID,
                        m_nRetryTimesCount, m_nRetryTimesCount - 1);
					char szLog[200];
					sprintf(szLog, S_BACKUP_RETRY, m_nTaskID, m_nRetryTimesCount, m_nRetryTimesCount-1);
					szLog[199] = '\0';
					CWorkModule::WriteLog(MTS_BACKUP, szLog, SLog::OPER_FAIL);
                }
            }

            DelOutDateFile();
            ClearList();
            m_bIsAvailable = FALSE;
        }
        catch(...){
            ClearList();
            m_bIsExit = TRUE;
            return;
        }
    }//while 1
}

/*****************************************************************************
�� �� ��  : CNetBakTask::Connect
��������  : ����FTP Server
�������  : ��
�������  : ��
�� �� ֵ  : BOOL
���ú���  :
��������  :

  �޸���ʷ      :
  1.��    ��   : 2002��9��6��
  ��    ��   : ������
  �޸�����   : �����ɺ���

*****************************************************************************/
BOOL CNetBakTask::Connect()
{
    //����m_LastConnTimeΪ��ǰʱ��
    m_LastConnTime = time(NULL);
    //����m_pFtpClient[m_nActiveIndex]->Connect()������ʧ�ܷ���FALSE
    int bRes = m_pFtpClient[m_nActiveIndex]->Connect(m_szDestHostIP[m_nActiveIndex],
                                                     m_szUserName,
                                                     m_szPassword,
                                                     m_szLocalHostIP[m_nActiveIndex]);
    if(bRes != ERR_SUCCESS && m_szDestHostIP[1][0] != '\0')
    {
        MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
               S_BACKUP_CONNECT,
               m_szLocalHostIP[m_nActiveIndex],
               m_szUserName,
               m_szDestHostIP[m_nActiveIndex],
               S_BACKUP_FAILURE,
               m_nTaskID);
        m_nActiveIndex++;
        if(m_nActiveIndex >= 2)
        {
            m_nActiveIndex = 0;
        }
        bRes = m_pFtpClient[m_nActiveIndex]->Connect(m_szDestHostIP[m_nActiveIndex],
                                                     m_szUserName,
                                                     m_szPassword,
                                                     m_szLocalHostIP[m_nActiveIndex]);
    }

    if(bRes != ERR_SUCCESS)
    {
        m_bIsAvailable = FALSE;
        MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
               S_BACKUP_CONNECT,
               m_szLocalHostIP[m_nActiveIndex],
               m_szUserName,
               m_szDestHostIP[m_nActiveIndex],
               S_BACKUP_FAILURE,
               m_nTaskID);
        return FALSE;
    }

    m_strDestHost = m_szDestHostIP[m_nActiveIndex];

    //����m_bIsAvailableΪTRUE
    m_bIsAvailable = TRUE;
    MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
        S_BACKUP_CONNECT,
        m_szLocalHostIP[m_nActiveIndex],
        m_szUserName,
        m_szDestHostIP[m_nActiveIndex],
        S_BACKUP_SUCCESS,
        m_nTaskID);

    return TRUE;
}
/*****************************************************************************
�� �� ��  : CNetBakTask::SetDir
��������  : ������FTP Server�ϵĵ�ǰĿ¼
�������  : const char* pszDirectory
�������  : ��
�� �� ֵ  : BOOL
���ú���  :
��������  :

   �޸���ʷ      :
    1.��    ��   : 2002��9��6��
      ��    ��   : ������
      �޸�����   : �����ɺ���

*****************************************************************************/
BOOL CNetBakTask::SetDir( const char* pszDirectory )
{
    return (m_pFtpClient[m_nActiveIndex]->SetDirectory(pszDirectory) == 0);
}
/*****************************************************************************
�� �� ��  : CNetBakTask::MakeDir
��������  : ��FTP Server�ϴ���һ��Ŀ¼
�������  : const char* pszDirectory
�������  : ��
�� �� ֵ  : BOOL
���ú���  :
��������  :

    �޸���ʷ      :
     1.��    ��   : 2002��9��6��
       ��    ��   : ������
       �޸�����   : �����ɺ���

*****************************************************************************/
BOOL CNetBakTask::MakeDir( const char* pszDirectory )
{
    if (m_pFtpClient[m_nActiveIndex]->MakeDirectory(pszDirectory) != 0)
    {
        LIST<STRING> DirNameList;
        char pTmpDir[MAX_PATH];
        strncpy(pTmpDir,pszDirectory,MAX_PATH);
        char *pToken = strtok(pTmpDir,"/\\");
        while(pToken)
        {
            DirNameList.push_back(pToken);
            pToken = strtok(NULL,"/\\");
        }
        char szPath[MAX_PATH];
        int nLen = 0;
        int nRet = 0;
        //һ��һ������Ŀ¼
        while(!DirNameList.empty())
        {
            SNPRINTF(&szPath[nLen],MAX_PATH,"/%s",DirNameList.front().c_str());
            if(!SetDir(szPath))
            {
                nRet = m_pFtpClient[m_nActiveIndex]->MakeDirectory(szPath);
                if(nRet)
                {
                    MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
                           S_BACKUP_CRE_DIR,
                           szPath,S_BACKUP_FAILURE,m_nTaskID);
                    return FALSE;
                }
                MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
                       S_BACKUP_CRE_DIR,
                       szPath,S_BACKUP_SUCCESS,m_nTaskID);
            }
            nLen += DirNameList.front().size();
            nLen++; //"/" �ĳ��� �� 1
            DirNameList.pop_front();
        }
    }
    return TRUE;
}
/*****************************************************************************
�� �� ��  : CNetBakTask::PutAFile
��������  : ��FTP Server�Ϸ���һ���ļ�
�������  : const char* pszLocalFile
            const char* pszRemoteFile
�������  : ��
�� �� ֵ  : BOOL
���ú���  :
��������  :

   �޸���ʷ      :
    1.��    ��   : 2002��9��6��
      ��    ��   : ������
      �޸�����   : �����ɺ���

*****************************************************************************/
BOOL CNetBakTask::PutAFile( const char* pszLocalFile, const char* pszRemoteFile )
{
    char szTmpFile[MAX_PATH];
    SNPRINTF(szTmpFile,MAX_PATH,"%s%s",pszRemoteFile,".tmp");
    szTmpFile[MAX_PATH - 1] = 0;

    if(m_strLastSendFile == pszRemoteFile)
    {
        SNPRINTF(szTmpFile,
                 MAX_PATH,
                 "%s%d%s",
                 pszRemoteFile,
                 m_nResendCount++,
                 ".tmp");
        szTmpFile[MAX_PATH - 1] = 0;
        if(m_nResendCount > 9)
        {
            m_nResendCount = 0;
        }
    }
    m_strLastSendFile = pszRemoteFile;

    int nRet = m_pFtpClient[m_nActiveIndex]->PutFile(pszLocalFile,szTmpFile);
    if(nRet)
    {
        //Z30971�޸����ⵥ�� SWPD07859 2004/08/24
        //�����е�FTP SERVER����ֱ�Ӹ��������ļ�,��ɾ֮
        if (0 != m_pFtpClient[m_nActiveIndex]->DeleteFile(szTmpFile))
        {
            return FALSE;
        }
        
        //���±���
        if (0 != m_pFtpClient[m_nActiveIndex]->PutFile(pszLocalFile,szTmpFile))
        {
            return FALSE;
        }

    }
    m_nResendCount = 0;
    m_strLastSendFile = "";

    const char* p1 = strrchr(szTmpFile, '/');
#ifdef _PLATFORM_WIN32
    const char* p2 = strrchr(szTmpFile, '\\');
#else //_PLATFORM_WIN32
    const char* p2 = NULL;
#endif //_PLATFORM_WIN32
    const char* szTmpFile1 = max(p1, p2);
    if(szTmpFile1 == NULL)
    {
        szTmpFile1 = szTmpFile;
    }
    else
    {
        szTmpFile1++;
    }

    p1 = strrchr(pszRemoteFile, '/');
#ifdef _PLATFORM_WIN32
    p2 = strrchr(pszRemoteFile, '\\');
#else //_PLATFORM_WIN32
    p2 = NULL;
#endif //_PLATFORM_WIN32
    const char* pszRemoteFile1 = max(p1, p2);
    if(pszRemoteFile1 == NULL)
    {
        pszRemoteFile1 = pszRemoteFile;
    }
    else
    {
        pszRemoteFile1++;
    }

    nRet = m_pFtpClient[m_nActiveIndex]->RenameFile(szTmpFile1,pszRemoteFile1);
    if(nRet)
    {
        //Z30971�޸����ⵥ�� SWPD07859 2004/08/24
        //�п�������Ϊ��һ�δ�����һ�����������ļ������
        //�����һ�������ļ���ʹ�ĸ������ɹ�,��ɾ��ԭ�е�Ŀ���ļ�,Ȼ���ٳ��Ը���
        if(0 != m_pFtpClient[m_nActiveIndex]->DeleteFile(pszRemoteFile))
        {
            TRACE(MTS_BACKUP,S_BACKUP_RENAME_DELETE_FAIL,m_nTaskID,pszRemoteFile);
            MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
                   S_BACKUP_RENAME_DELETE_FAIL,m_nTaskID,pszRemoteFile);

            return FALSE;
        }

        if (0 != m_pFtpClient[m_nActiveIndex]->RenameFile(szTmpFile1,pszRemoteFile1))
        {
            TRACE(MTS_BACKUP,S_BACKUP_RENAME_FAIL,m_nTaskID,szTmpFile);
            MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
                   S_BACKUP_RENAME_FAIL,m_nTaskID,szTmpFile);
           
            m_pFtpClient[m_nActiveIndex]->DeleteFile(szTmpFile);
            return FALSE;
        }
    }

    return TRUE;
}
/*****************************************************************************
�� �� ��  : CNetBakTask::DelDestDirOrFile
��������  : ɾ�����ڱ����ļ�����Ŀ¼
�������  : ��
�������  : ��
�� �� ֵ  : int
���ú���  :
��������  :

   �޸���ʷ      :
    1.��    ��   : 2002��9��6��
      ��    ��   : ������
      �޸�����   : �����ɺ���

*****************************************************************************/
int CNetBakTask::DelDestDirOrFile()
{
    if(m_nDesFileSavedDays <= 0)
        return ERR_SUCCESS;

    FTP_FILELIST FtpFileList;
    int nRet = ERR_SUCCESS;
    char szDesName[MAX_PATH];

    m_pFtpClient[m_nActiveIndex]->ListDirectory(m_szDestDir,FtpFileList);
    while(!FtpFileList.empty())
    {
        SFtpFile FtpFile = FtpFileList.front();
        //�����Ŀ¼����Ŀ¼�����ֲ���Ҫ������־�����
        if(FtpFile.nAttr == FTP_FILE_ATTRIBUTE_DIRECTORY
            && !IsDigitValue(FtpFile.szFileName))
        {
            FtpFileList.pop_front();
            continue;
        }

        SNPRINTF(szDesName,MAX_PATH,"%s/%s",m_szDestDir,FtpFile.szFileName);
        szDesName[MAX_PATH - 1] = '\0';
        time_t tTime,curTime;
        tm tFileDate,tTmp;
        memset(&tFileDate,0,sizeof(tm));
        memset(&tTmp,0,sizeof(tm));
        int nDate = 0;

        if(FtpFile.nAttr == FTP_FILE_ATTRIBUTE_NORMAL)
        {
            nDate = atoi(FtpFile.szDate);
            tFileDate.tm_year = nDate/10000 - 1900;
            tFileDate.tm_mon = (nDate%10000)/100 - 1;
            tFileDate.tm_mday = nDate%100;
            tTime = mktime(&tFileDate);
            if(difftime(time(NULL),tTime) >= (m_nDesFileSavedDays * 86400))
            {
                nRet = m_pFtpClient[m_nActiveIndex]->DeleteFile(szDesName);
                if(!nRet)
                    MSGOUT(MTS_BACKUP,
                           MSGOUT_LEVEL_SIDLY,
                           S_BACKUP_DEL_DIR,
                           szDesName,S_BACKUP_SUCCESS,
                           m_nTaskID);
                else
                    MSGOUT(MTS_BACKUP,
                           MSGOUT_LEVEL_SIDLY,
                           S_BACKUP_DEL_DIR,
                           szDesName,S_BACKUP_FAILURE,
                           m_nTaskID);
            }
        }
        else if(FtpFile.nAttr == FTP_FILE_ATTRIBUTE_DIRECTORY)
        {
            //ȡĿ¼ʱ��
            nDate = atoi(FtpFile.szFileName);
            tFileDate.tm_year = nDate/10000 - 1900;
            tFileDate.tm_mon = (nDate%10000)/100 - 1;
            tFileDate.tm_mday = nDate%100;
            tTime = mktime(&tFileDate);
            //ȡ��ǰʱ��
            time(&curTime);
            tTmp = *localtime(&curTime);
            tTmp.tm_hour = 0;
            tTmp.tm_min = 0;
            tTmp.tm_sec = 0;
            curTime = mktime(&tTmp); //�õ���ǰ�����ڣ�ʱ���붼Ϊ0
            if(difftime(curTime,tTime) >= (m_nDesFileSavedDays * 86400))
            {
                nRet = m_pFtpClient[m_nActiveIndex]->RemoveDirectory(szDesName);
                if(!nRet)
                    MSGOUT(MTS_BACKUP,
                           MSGOUT_LEVEL_SIDLY,
                           S_BACKUP_DEL_DIR,
                           szDesName,S_BACKUP_SUCCESS,
                           m_nTaskID);
                else
                    MSGOUT(MTS_BACKUP,
                           MSGOUT_LEVEL_SIDLY,
                           S_BACKUP_DEL_DIR,
                           szDesName,S_BACKUP_FAILURE,
                           m_nTaskID);
            }
            else
            {
                nRet = ERR_SUCCESS;
            }
        }
        else
        {
            TRACE(MTS_BACKUP,S_BACKUP_FILE_ATTR_ERR,m_nTaskID,szDesName);
            MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
                   S_BACKUP_FILE_ATTR_ERR,m_nTaskID,szDesName);
            nRet = ERR_FAIL;
        }

        if(nRet)
        {
            TRACE(MTS_BACKUP,S_BACKUP_DELETE_FAILED,m_nTaskID,szDesName);
            MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
                   S_BACKUP_DELETE_FAILED,m_nTaskID,szDesName);
        }

        FtpFileList.pop_front();
    }
    return nRet;
}
/*****************************************************************************
�� �� ��  : CNetBAkTask::GetCfg
��������  : ��INI�ļ��л�ȡ����FTP��������Ĳ���
�������  : ��
�������  : ��
�� �� ֵ  : BOOL
���ú���  :
��������  :

   �޸���ʷ      :
    1.��    ��   : 2002��9��6��
      ��    ��   : ������
      �޸�����   : �����ɺ���

*****************************************************************************/
BOOL CNetBakTask::GetCfg()
{
    //���ø����ͬ������
    if(!CBakTask::GetCfg())
    {
        return FALSE;
    }

    //��INI�ļ�
    CINIFile ini(GetCfgFilePath());
    if(ini.Open() != TRUE)
    {
        TRACE(MTS_BACKUP,S_BACKUP_INI_OPEN_FAIL);
        return FALSE;
    }
    //��INI�ļ���[NetBackup]�������ζ�ȡ���²���������
    //m_nReConnInterval��m_szUserName��m_szLocalIp
    //����¼�����BeforeEncode��Password
    //����bBeforeEncode1��szPassword1
    m_nReConnInterval = ini.GetInt(CFG_SEC_NET_BACKUP,
        CFG_NET_BACKUP_RECONNECT_INTERVAL,
        CFG_NET_BACKUP_RECONNECT_INTERVAL_DEFAULT);
    ini.GetString(CFG_SEC_NET_BACKUP,CFG_NET_BACKUP_USER_NAME,
        CFG_NET_BACKUP_USER_NAME_DEFAULT,
        m_szUserName,MAX_USERNAME_LEN);
    ini.GetString(CFG_SEC_NET_BACKUP,
                  CFG_BACKUP_DEST_HOST_IP,
                  CFG_BACKUP_DEST_HOST_IP_DEFAULT,
                  m_szDestHostIP[0],
                  MAX_HOST_LEN);
    ini.GetString(CFG_SEC_NET_BACKUP,
                  CFG_BACKUP_DEST_HOST_IP_BAK,
                  "",
                  m_szDestHostIP[1],
                  MAX_HOST_LEN);
    ini.GetString(CFG_SEC_NET_BACKUP,
                  CFG_NET_BACKUP_LOCAL_IP,
                  CFG_NET_BACKUP_LOCAL_IP_DEFAULT,
                  m_szLocalHostIP[0],
                  MAX_HOST_LEN);
    ini.GetString(CFG_SEC_NET_BACKUP,
                  CFG_NET_BACKUP_LOCAL_IP_BAK,
                  CFG_NET_BACKUP_LOCAL_IP_DEFAULT,
                  m_szLocalHostIP[1],
                  sizeof(m_szLocalHostIP[1]));
    m_nReTryTimes = ini.GetInt(CFG_SEC_NET_BACKUP,
        CFG_NET_BACKUP_RETRY_TIMES,
        CFG_NET_BACKUP_RETRY_TIMES_DEFAULT);
    if(m_nReTryTimes <= 0 || m_nReTryTimes > 5)
    {
        m_nReTryTimes = CFG_NET_BACKUP_RETRY_TIMES_DEFAULT;
    }
    BOOL bBeforeEncode1 = ini.GetInt(CFG_SEC_NET_BACKUP,
        CFG_NET_BACKUP_BEFORE_ENCODE,
        CFG_NET_BACKUP_BEFORE_ENCODE_DEFAULT);
    char szPassword1[MAX_PASSWORD_LEN];
    ini.GetString(CFG_SEC_NET_BACKUP,CFG_NET_BACKUP_PASSWORD,
        CFG_NET_BACKUP_PASSWORD_DEFAULT,
        szPassword1,MAX_PASSWORD_LEN);

	
	m_nFtpIsPassive = ini.GetInt(CFG_SEC_COMMON,
								CFG_NET_BACKUP_FTP_IS_PASSIVE,
								0);
	char szSec[20];
#ifdef _PRODUCT_CCS
	sprintf(szSec, "%s%d", CFG_SEC_AP, CMsgProcessor::GetProcessType() - PT_CO_BASE+ 1);
#else
	sprintf(szSec, "%s%d", CFG_SEC_AP, CMsgProcessor::GetProcessType() - PT_AP_BASE+ 1);
#endif

	m_nFtpIsPassive = ini.GetInt(szSec,
								 CFG_NET_BACKUP_FTP_IS_PASSIVE,
								 m_nFtpIsPassive);

	m_nFtpIsPassive = ini.GetInt(CFG_SEC_NET_BACKUP,
		                         CFG_NET_BACKUP_FTP_IS_PASSIVE,
								 m_nFtpIsPassive);
	

    char szBackupSec[16];
    BOOL bBeforeEncode2 = TRUE;
    char szPassword2[MAX_PASSWORD_LEN];
    //��INI�ļ���[BackupTaskN]��NΪ��������ID���������ζ�ȡ
    //���²��������ã� m_nReConnInterval��m_szUserName��m_szLocalIp��
    //�������Ĭ�����ã����������������Դ�Ϊ׼��
    //����¼�����BeforeEncode��Password����bBeforeEncod2��szPassword2
    SNPRINTF(szBackupSec,16,"%s%d",CFG_NET_BACKUP,m_nTaskID);
    szBackupSec[15] = 0;
    m_nReConnInterval = ini.GetInt(szBackupSec,
        CFG_NET_BACKUP_RECONNECT_INTERVAL,
        m_nReConnInterval);
    if(m_nReConnInterval > 3600)
    {
        m_nReConnInterval = 3600;
        TRACE(MTS_BACKUP,S_BACKUP_EXCEED_LIMIT,m_nTaskID,CFG_NET_BACKUP_RECONNECT_INTERVAL,3600);
    }
    else if(m_nReConnInterval < 1)
    {
        m_nReConnInterval = 1;
        TRACE(MTS_BACKUP,S_BACKUP_EXCEED_LIMIT,m_nTaskID,CFG_NET_BACKUP_RECONNECT_INTERVAL,1);
    }

	m_nFtpIsPassive = ini.GetInt(szBackupSec,
								 CFG_NET_BACKUP_FTP_IS_PASSIVE,
								 m_nFtpIsPassive);

    ini.GetString(szBackupSec,CFG_NET_BACKUP_USER_NAME,
        m_szUserName,m_szUserName,MAX_USERNAME_LEN);
    ini.GetString(szBackupSec,
                  CFG_BACKUP_DEST_HOST_IP,
                  m_szDestHostIP[0],
                  m_szDestHostIP[0],
                  MAX_HOST_LEN);
    ini.GetString(szBackupSec,
                  CFG_BACKUP_DEST_HOST_IP_BAK,
                  m_szDestHostIP[1],
                  m_szDestHostIP[1],
                  MAX_HOST_LEN);
    ini.GetString(szBackupSec,
                  CFG_NET_BACKUP_LOCAL_IP,
                  m_szLocalHostIP[0],
                  m_szLocalHostIP[0],
                  MAX_HOST_LEN);
    ini.GetString(szBackupSec,
                  CFG_NET_BACKUP_LOCAL_IP_BAK,
                  m_szLocalHostIP[1],
                  m_szLocalHostIP[1],
                  MAX_HOST_LEN);
    bBeforeEncode2 = ini.GetInt(szBackupSec,
        CFG_NET_BACKUP_BEFORE_ENCODE,bBeforeEncode1);
    ini.GetString(szBackupSec,CFG_NET_BACKUP_PASSWORD,
        szPassword1,szPassword2,MAX_PASSWORD_LEN);

    //��bBeforeEncod2=FALSEת5������������m_szPassword=szPassword2��
    //����[BackupTaskN]��NΪm_nTaskID�����е�BeforeEncodeΪ��Ч��
    //�޸�PasswordΪ���ܺ�����룬���bBeforeEncod1=TRUE
    //����szPassword1= szPassword2���޸�[NetBackup]
    //���BeforeEncode��Password��
    memset(m_szPassword,0,MAX_PASSWORD_LEN);
    if (bBeforeEncode2)
    {
        char szCode[MAX_PASSWORD_LEN];
        memset(szCode,0,MAX_PASSWORD_LEN);
        strncpy(m_szPassword,szPassword2,MAX_PASSWORD_LEN);
        encode(szPassword2,szCode);
        if(bBeforeEncode1 && strcmp(szPassword2,szPassword1) == 0)
        {
            ini.WriteInt(CFG_SEC_NET_BACKUP,CFG_NET_BACKUP_BEFORE_ENCODE,0);
            ini.WriteString(CFG_SEC_NET_BACKUP,CFG_NET_BACKUP_PASSWORD,szCode);
        }
        else
        {
            ini.WriteInt(szBackupSec,CFG_NET_BACKUP_BEFORE_ENCODE,0);
            ini.WriteString(szBackupSec,CFG_NET_BACKUP_PASSWORD,szCode);
        }
    }
    //��bBeforeEncod2=FALSE����m_szPassword��Ϊ������ܺ������
    else
    {
        decode(szPassword2,m_szPassword);
    }

    //�ر�INI�ļ�
    ini.Close();

    m_pFtpClient[0]->SetPassive(m_nFtpIsPassive);
	m_pFtpClient[1]->SetPassive(m_nFtpIsPassive);   
    
    return true;
}

int CBakTask::GetTimeFromString(const char* const szTime) //add by yangguang copy from clw
{
    if(szTime == 0 || szTime[0] == '\0')
    {
        return 0;
    }
    
    int nTime = 0;
    char* p = (char*)szTime;
    char* p1 = strchr(p, ':');
    if(p1 == NULL)
        //���ֻ��ʱ
    {
        if(IsDigitValue(p))
        {
            nTime = atoi(p) * 3600;
        }
    }
    else
    {
        char szBuf[10];
        strncpy(szBuf, p, p1 - p);
        szBuf[p1 - p] = '\0';
        if(IsDigitValue(szBuf))
        {
            nTime = atoi(szBuf) * 3600;
        }
        
        p = p1 + 1;
        p1 = strchr(p, ':');
        if(p1 == NULL)
        {
            if(IsDigitValue(p))
            {
                nTime += atoi(p) * 60;
            }
        }
        else
        {
            char szBuf[10];
            strncpy(szBuf, p, p1 - p);
            szBuf[p1 - p] = '\0';
            if(IsDigitValue(szBuf))
            {
                nTime += atoi(szBuf) * 60;
            }
        }
    }
    
    return nTime;    
}

//added by chenliangwei, 2003-08-05
int CBakTask::SaveOnceColSchl()
{
    //m_MutexForSchl.acquire();
    
    CINIFile INIFile(GetCfgFilePath());
    if (!INIFile.Open())
    {
        //m_MutexForSchl.release();
        return ERR_FAIL;
    }    
    
    char szColTime[1024] = "";
    LIST<time_t>::iterator i;
    for(i = m_OnceListTime.begin(); i != m_OnceListTime.end(); i++)
    {
        tm tmTime = *localtime(&(*i));
        SNPRINTF(szColTime,
            sizeof(szColTime),
            "%s%04d-%02d-%02d %02d:%02d:%02d,",
            szColTime,
            tmTime.tm_year + 1900,
            tmTime.tm_mon + 1,
            tmTime.tm_mday,
            tmTime.tm_hour,
            tmTime.tm_min,
            tmTime.tm_sec);
        
        szColTime[sizeof(szColTime) - 1] = '\0';
    }
    
    if(strlen(szColTime) > 0)
    {
        szColTime[strlen(szColTime) - 1] = '\0';
    }
    
    char szBackupSec[16];
    SNPRINTF(szBackupSec, sizeof(szBackupSec), "%s%d", CFG_NET_BACKUP, m_nTaskID);
    szBackupSec[15] = 0;
    INIFile.WriteString(szBackupSec, CFG_NET_ONCE_LIST_TIME, szColTime);
    
    //m_MutexForSchl.release();
    
    return 0;
}
//added end

//����һ�βɼ�����,ֻ����ʱ���Ƿ���,�����Ǳ��籸�ݳɹ���ʧ��,����������
bool CBakTask::WaitNextTime()
{
    return true;
}

bool CNetBakTask::WaitNextTime()
{
    tm tmLastNow = *localtime(&m_LastBakOKTime);
    long tLastTime = tmLastNow.tm_hour * 3600
                     + tmLastNow.tm_min * 60
                     + tmLastNow.tm_sec;

    //m_bOnceColAct = false;
    while(!IsExit())
    {
        time_t tNow = time(NULL);
        tm tmNow = *localtime(&tNow);
        long tNowTime = tmNow.tm_hour * 3600
                        + tmNow.tm_min * 60
                        + tmNow.tm_sec;

        //m_MutexForSchl.acquire();

        //Z30971�޸����ⵥ��SWPD05706 2004/08/23
        //�ϴδ�����̷�������,�����ļ����ɹ�
        if (m_bTransErrFlg) 
        {
            //��������ʱ��Ϊm_nReConnInterval
            if ((tNow - m_LastConnTime >=  m_nReConnInterval))
            {
                return true;
            }
    
        }
        else //��������
        {        
            //ʱ�����е����ȼ��ϸ�
            if(m_ListTime.size() > 0)
            {
                LIST<int>::iterator i;
                for(i = m_ListTime.begin(); i != m_ListTime.end(); i++)
                {
                    //���￼�ǵ��ǿ�������
                    if(tNowTime < tLastTime)
                    {
                        tLastTime = 0;
                    }
                    if((tNowTime >= *i) && (tLastTime < *i))
                    {
                        m_LastBakOKTime = time(NULL); //added by chenliangwei, 2003-05-20
                        //m_MutexForSchl.release();
                        return true;
                    }
                }
            }
            
            //modifyed by chenliangwei, 2003-08-05,����else
            //ʱ�䷶Χ
            else if(((tNowTime >= m_nBeginTime) && (tNowTime <= m_nEndTime))
                || ((0 == m_nBeginTime) && (0 == m_nEndTime)))
            {
                //added by chenliangwei, 2003-05-20
                //����ɼ��ɹ�����Ϊ0, ֱ�ӷ���
                if (m_nSearchFilesInterval == 0)
                {
                    m_LastBakOKTime = time(NULL);
                    //m_MutexForSchl.release();
                    return true;
                }
                //added end
                
                if(tNow - m_LastBakOKTime >=  m_nSearchFilesInterval)
                {
                    while(m_LastBakOKTime + m_nSearchFilesInterval <= tNow)
                    {
                        m_LastBakOKTime += m_nSearchFilesInterval;
                    }
                    //m_MutexForSchl.release();
                    return true;
                }
            }
            
            //����ٿ��ǵ����ɼ�����
            if(m_OnceListTime.size() > 0)
            {
                time_t tActTime = m_OnceListTime.front();
                if(tNow >= tActTime)
                {
                    m_OnceListTime.pop_front();
                    SaveOnceColSchl();
                    //m_bOnceColAct = true;
                    //m_MutexForSchl.release();
                    return true;
                }
            }
        }

        //m_MutexForSchl.release();
        //ACE_OS::sleep(1);
        if (this->Sleep(1))
            m_bIsExit = true;
        else
            m_bIsExit = false;
    }
 
    return false;
}


