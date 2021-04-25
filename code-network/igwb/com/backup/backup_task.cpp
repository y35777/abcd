/******************************************************************************
版权所有 (C), 2001-2011, 华为技术有限公司
******************************************************************************
    文 件 名   : backup_task.cpp
    版 本 号   : 初稿
    作    者   : 张玉新
    生成日期   : 2002年9月6日
    最近修改   :
    功能描述   : 备份任务的实现
    函数列表   :
    修改历史   :
    1.日    期   : 2002年9月6日
    作    者   : 张玉新
    修改内容   : 创建文件

******************************************************************************/
#include "backup_task.h"
#include "backup_file_pool.h"

ACE_Mutex CBakTask::g_LogFileMutex;
/*****************************************************************************
函 数 名  : CBakTask::CBakTask
功能描述  : 构造函数
输入参数  : int nTaskID, CBakTaskMgr* pBakTaskMgr
输出参数  : 无
返 回 值  :
调用函数  :
被调函数  :

  修改历史      :
  1.日    期   : 2002年9月6日
  作    者   : 张玉新
  修改内容   : 新生成函数

*****************************************************************************/
CBakTask::CBakTask(int nTaskID, CBakTaskMgr* pBakTaskMgr)
{
    //初始化m_nTaskID = nTaskID
    m_nTaskID = nTaskID;
    //初始化m_pBakTaskMgr = pBakTaskMgr
    m_pBakTaskMgr = pBakTaskMgr;
    //初始化其他成员变量
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
函 数 名  : WriteStatusFile
功能描述  : 将当前状态写入状态文件
输入参数  : SStatusNode  CurStatus
输出参数  : 无
返 回 值  : int
调用函数  :
被调函数  :

  修改历史      :
  1.日    期   : 2002年9月6日
  作    者   : 张玉新
  修改内容   : 新生成函数

*****************************************************************************/
BOOL CBakTask::WriteStatusFile(SStatusNode &CurStatus)
{
    return m_pBakTaskMgr->WriteStatusFile(CurStatus);
}
/*****************************************************************************
函 数 名  : CBakTask::GetStatInfo
功能描述  : 获取某个备份任务的状态信息
输入参数  : const char* pszTaskName
SStatusNode *pStatInfo
输出参数  : 无
返 回 值  : int
调用函数  :
被调函数  :

  修改历史      :
  1.日    期   : 2002年9月6日
  作    者   : 张玉新
  修改内容   : 新生成函数

*****************************************************************************/
int CBakTask::GetStatInfo( const char* pszTaskName, SStatusNode *pStatInfo)
{
    return m_pBakTaskMgr->GetStatInfo(pszTaskName, pStatInfo);
}
/*****************************************************************************
函 数 名  : CBakTask::open
功能描述  : 启动线程函数
输入参数  : void* args
输出参数  : 无
返 回 值  : int
调用函数  :
被调函数  :

  修改历史      :
  1.日    期   : 2002年9月6日
  作    者   : 张玉新
  修改内容   : 新生成函数

*****************************************************************************/
int  CBakTask::open(void* args)
{
    //如果m_nReStartTimes > 3 ，返回ERR_START_TOOMANY_TIMES
    if(m_nReStartTimes++ > 3)
        return ERR_START_TOOMANY_TIMES;

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
函 数 名  : CBakTask::Stop
功能描述  : 停止线程函数
输入参数  : u_long flags
输出参数  : 无
返 回 值  : void
调用函数  :
被调函数  :

  修改历史      :
  1.日    期   : 2002年9月6日
  作    者   : 张玉新
  修改内容   : 新生成函数

*****************************************************************************/
void CBakTask::Stop()
{
    //产生m_BakThdExitEvt事件
    //m_BakThdExitEvt.signal();

    //通过m_BakThdMgr退出线程
    TRACE(MTS_BACKUP,S_BACKUP_END_TASK,m_nTaskID);
    wait();

}
/*****************************************************************************
函 数 名  : CBakTask::svc
功能描述  : 备份线程函数
输入参数  : 无
输出参数  : 无
返 回 值  : int
调用函数  :
被调函数  :

  修改历史      :
  1.日    期   : 2002年9月6日
  作    者   : 张玉新
  修改内容   : 新生成函数

*****************************************************************************/
int CBakTask::svc()
{
    Backup();
    return ERR_SUCCESS;
}

//判断是否已备份过
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
函 数 名  : CBakTask::PrepareFiles
功能描述  : 准备符合条件的待备份文件列表
输入参数  : const char* szPath - 备份文件所在的路径
time_t tFileTime - 备份状态文件时间
BOOL bRmvLastFile
输出参数  : m_FileList
返 回 值  : int
调用函数  :
被调函数  :

  修改历史      :
  1.日    期   : 2002年9月6日
  作    者   : 张玉新
  修改内容   : 新生成函数

*****************************************************************************/
#define READ_COUNT_WAIT_ONCE    10      //每次等待之前读取的文件数目
#define READ_FILE_INTERVAL      1000    //时间,微秒级
 
int CBakTask::PrepareFiles( const char* szPath, time_t tFileTime, BOOL bRmvLastFile )
{
    //判断是否是后存盘第二份话单: 非日期(8位数字)目录
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
    //打开文件目录,打开szPath目录
    pDir = ACE_OS::opendir(szPath);
    if(pDir == NULL)
    {
		//by 杨善	2004-5-29   对应问题单SWPD05427
        char szTmpBuf[BK_MAX_INFO_LEN];
        sprintf(szTmpBuf, "Errno: %d (%s)", errno, strerror(errno));
        STRING sTmpInfo = S_BACKUP_OPEN_DIR_FAIL;//打开路径失败(231)
        sTmpInfo += STRING(" ") + szPath + STRING(" ");
        MSGOUT(MTS_BACKUP, MSGOUT_LEVEL_SIDLY, sTmpInfo.c_str());
        sTmpInfo += szTmpBuf;
        TRACE(MTS_BACKUP, sTmpInfo.c_str());
		//the end	2004-5-19	对应问题单SWPD05427

        return ERR_FAIL;
    }
    //MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,            //Commented by ZhengYuqun 2003-10-21
    //    S_BACKUP_SEARCH_FILE_IN,szPath,m_nTaskID);
    char szSrcFilePath[MAX_PATH];
    //已遍历完所有文件
    SFilesListNode Node;
    
    int nFileCount = 0;
    while((pDirent = ACE_OS::readdir(pDir)) != NULL)
    {
        ACE_Time_Value timeval(0);
        if (++nFileCount > READ_COUNT_WAIT_ONCE)
        {
            //毫秒: 1
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
        //形成源文件路径
        SNPRINTF(szSrcFilePath,MAX_PATH,"%s/%s",szPath,pDirent->d_name);
        szSrcFilePath[MAX_PATH - 1] = 0;

        ACE_OS::stat(szSrcFilePath, &sDirStat);
        if(S_ISDIR(sDirStat.st_mode))
        {
            continue;
        }

        //文件名,时间
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
            //张玉新 2003-04-07
            //如果备份过就增加到删除列表
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
函 数 名  : PrepareSubDirs
功能描述  : 将pszDir下所有的目录放在SubDirList中
输入参数  : const char* pszDir
LIST2<STRING>& SubDirList
输出参数  : 无
返 回 值  : void
调用函数  :
被调函数  :

  修改历史      :
  1.日    期   : 2002年9月6日
  作    者   : 张玉新
  修改内容   : 新生成函数

*****************************************************************************/
void CBakTask::PrepareSubDirs(const char* pszDir, LIST2<STRING> &SubDirList)
{
    PrepareSubDirs_Impl(pszDir,SubDirList);
    //对目录排序
    if(SubDirList.empty())
    {
        SubDirList.push_back(pszDir);
        MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
               S_BACKUP_PUSH_ROOT_DIR, pszDir, m_nTaskID);
    }
    std::sort(SubDirList.begin(),SubDirList.end(),Compare());
}

//PrepareSubDirs的具体实现
void CBakTask::PrepareSubDirs_Impl(const char* pszDir, LIST2<STRING>& SubDirList )
{
    //打开pszDir所在的目录，得到指向pszDir目录的指针pDir
    //打开szPath目录
    ACE_DIR *pDir = NULL;
    dirent *pDirent = NULL;
    ACE_stat sDirStat;

    //打开文件目录
    pDir = ACE_OS::opendir(pszDir);
    if(pDir == NULL)
    {
		//by 杨善	2004-5-29   对应问题单SWPD05427	
        char szTmpBuf[BK_MAX_INFO_LEN];
        sprintf(szTmpBuf, "Errno: %d (%s)", errno, strerror(errno));
        STRING sTmpInfo = S_BACKUP_OPEN_DIR_FAIL;//打开路径失败(231)
        sTmpInfo += STRING(" ") + pszDir + STRING(" ");
        MSGOUT(MTS_BACKUP, MSGOUT_LEVEL_SIDLY, sTmpInfo.c_str());
        sTmpInfo += szTmpBuf;
        TRACE(MTS_BACKUP, sTmpInfo.c_str());
		//the end	2004-5-19	对应问题单SWPD05427

        return;
    }
    MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
           S_BACKUP_SEARCH_DIR,pszDir,m_nTaskID);
    //遍历pszDir下文件(夹),如果是文件夹并且不是“.”或者“..”
    //(假设是tmpDir),将其加入SubDirList尾部,并且通过
    //SNPRINTF(pszDir, MAX_PATH,"s%s" psz, tmpDir)
    //更新pszDir后,递归调用PrepareSubDirs(pszDir,SubDirList)
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
    //关闭pszDir所在目录
    ACE_OS::closedir(pDir);
}
/*****************************************************************************
函 数 名  : PutFiles
功能描述  : 将m_FileList中的文件依次发送到FTP Server端，并支持源文件删除
输入参数  : const char* szSrcPath
const char* szDestPath
输出参数  : 无
返 回 值  : int
调用函数  :
被调函数  :

  修改历史      :
  1.日    期   : 2002年9月6日
  作    者   : 张玉新
  修改内容   : 新生成函数

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
        //修改问题单18680
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
                    //还原到原来的状态
                    m_CurStatus = NodeBak;
                    TRACE(MTS_BACKUP,S_BACKUP_WRITE_STATUS_FAIL,m_nTaskID,szSrcFileName);
                    SNPRINTF(szLog,nLen,S_BACKUP_WRITE_STATUS_FAIL,
                        m_nTaskID,szSrcFileName);
                    szLog[nLen - 1] = '\0';
                    CWorkModule::WriteLog(MTS_BACKUP,szLog,SLog::OPER_FAIL);
                    MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,szLog);
                    bRet = FALSE;
                    break; //问题单18680修改
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
                //更新备份状态
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

                //写备份稽核日志
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
                //问题单18680，删掉break;
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
            MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_URGENT,szLog);  //将调试级别从次要改为紧急 ZhengYuqun SWPD01995 2003-10-21
            return ERR_BAK_FAIL;
        }
    }
    return ERR_SUCCESS;
}

int CNetBakTask::MakeNewCheckLogFile()
{
    //生成稽核日志文件名
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
    //打不开今天的文件，说明已跨天
    {
        //取昨天的日志文件名
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
            //取开始时间信息及话单文件总长度
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

                //判断是否已有汇总日志信息
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
                //写日志总结信息
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

            //复制文件到备份源目录
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
                           MSGOUT_LEVEL_URGENT,        //将调试级别从次要改为紧急 ZhengYuqun SWPD01995 2003-10-21
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

        //删除90天前的日志文件
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

        //创建今天的日志文件
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

    //生成稽核日志文件名
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

    //写稽核日志
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
    //打不开今天的文件，说明已跨天
    {
        int nRet = MakeNewCheckLogFile();
        if(nRet != ERR_SUCCESS)
        {
            return nRet;
        }

        //创建今天的日志文件
        f = fopen(strLogFileName.c_str(), "wb+");
    }

    if(NULL != f)
    {
        if(ACE_OS::strcasecmp(&szFileName[strlen(szFileName) - strlen(SZ_LOG_FILE_POSTFIX)],
                              SZ_LOG_FILE_POSTFIX) != 0)
        {
            //取最后一条日志信息
            char szOutMsg[4096] = "";
            fseek(f, -(lStrLen), SEEK_END);
            fgets(szOutMsg, sizeof(szOutMsg), f);

            //比较最后一条日志，如果不相同才追加
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
函 数 名  : CBakTask::Backup
功能描述  : 连接FTP Server，搜索并备份文件
输入参数  : 无
输出参数  : 无
返 回 值  : void
调用函数  :
被调函数  :

  修改历史      :
  1.日    期   : 2002年9月6日
  作    者   : 张玉新
  修改内容   : 新生成函数

*****************************************************************************/
void CBakTask::Backup()
{
    while(1)
    {
        if (m_pBakTaskMgr && m_pBakTaskMgr->GetLicenseCount() == 0)
        {
            TRACE(MTS_BACKUP, S_NO_BACKUP_LICENSE);
            if (Sleep(30))  //无License，休眠，继续
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
            //删除源文件的功能已经移到前后存盘了
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
函 数 名  : CBakTask::Sleep
功能描述  : 睡眠nSeconds秒，在睡眠中等待退出事件
输入参数  : int nSeconds
输出参数  : 无
返 回 值  : BOOL
调用函数  :
被调函数  :

修改历史  :
      1.日    期   : 2002年9月6日
      作    者   : 张玉新
      修改内容   : 新生成函数

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
函 数 名  : CBakTask::PutAFile
功能描述  : 发送一个文件
输入参数  : const char* pszLocalFile
            const char* pszRemoteFile
输出参数  : 无
返 回 值  : BOOL
调用函数  :
被调函数  :

修改历史  :
      1.日    期 : 2002年9月6日
        作    者 : 张玉新
        修改内容 : 新生成函数

*****************************************************************************/
BOOL CBakTask::PutAFile(const char* pszLocalFile, const char* pszRemoteFile)
{
    return FileCopy(pszLocalFile,pszRemoteFile);
}
/*****************************************************************************
函 数 名  : CBakTask::DelDestDirOrFile
功能描述  : 删除过期备份文件或者目录
输入参数  : 无
输出参数  : 无
返 回 值  : int
调用函数  :
被调函数  :

修改历史  :
      1.日    期   : 2002年9月6日
        作    者   : 张玉新
        修改内容   : 新生成函数

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
    //打开szPath目录
    ACE_DIR *pDir = NULL;
    dirent *pDirent = NULL;
    ACE_stat sDirStat;
    //打开文件目录
    pDir = ACE_OS::opendir(pszName);
    if(pDir == NULL)
    {
		//by 杨善	2004-5-29   对应问题单SWPD05427
        char szTmpBuf[BK_MAX_INFO_LEN];
        sprintf(szTmpBuf, "Errno: %d (%s)", errno, strerror(errno));
        STRING sTmpInfo = S_BACKUP_OPEN_DIR_FAIL;
        sTmpInfo += STRING(" ") + pszName + STRING(" ");
        MSGOUT(MTS_BACKUP, MSGOUT_LEVEL_SIDLY, sTmpInfo.c_str());
        sTmpInfo += szTmpBuf;
        TRACE(MTS_BACKUP, sTmpInfo.c_str());
		//the end	2004-5-19	对应问题单SWPD05427

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
            DirTime = mktime(&tTmp); //得到目录时间的基准值
            //取当前时间
            time(&curTime);
            tTmp = *localtime(&curTime);
            tTmp.tm_hour = 0;
            tTmp.tm_min = 0;
            tTmp.tm_sec = 0;
            curTime = mktime(&tTmp); //得到当前的日期，时分秒都为0
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
    //关闭目录
    ACE_OS::closedir(pDir);
    return ERR_SUCCESS;
}
/*****************************************************************************
函 数 名  : CBakTask::GetCfg
功能描述  : 文件中获取各个备份任务的参数
输入参数  : 无
输出参数  : 无
返 回 值  : BOOL
调用函数  :
被调函数  :

修改历史  :
   1.日    期   : 2002年9月6日
     作    者   : 张玉新
     修改内容   : 新生成函数

*****************************************************************************/
BOOL CBakTask::GetCfg()
{
    //打开INI文件；
    CINIFile ini(GetCfgFilePath());
    if(ini.Open() != TRUE)
    {
        TRACE(MTS_BACKUP,S_BACKUP_INI_OPEN_FAIL);
        return FALSE;
    }
    //从INI文件中[NetBackup]域中依次读取以下参数的配置
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
    //张玉新 2003-04-07
    m_uDelFileDays = ini.GetInt(CFG_SEC_NET_BACKUP,
                                CFG_NET_BACKUP_DEL_FILE_DAYS,
                                CFG_NET_BACKUP_DEL_FILE_DAYS_DEFAULT);
    char szBackupSec[16];
    //从INI文件中[BackupTaskN]（N为备份任务ID）
    //域中依次读取上述各个参数的配置
    //如果不是默认配置，参数的最终配置依次为准
    SNPRINTF(szBackupSec,16,"%s%d",CFG_NET_BACKUP,m_nTaskID);
    szBackupSec[15] = 0;
    //张玉新 2003-04-07

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
    
    //解析一次性序列
    if (szOnceListTimeBuf[0] != '\0')
    {    
        //在时间序列的最后加上','
        int nstrColLen = strlen(szOnceListTimeBuf);
        if (szOnceListTimeBuf[nstrColLen - 1] != ',')
        {
            szOnceListTimeBuf[nstrColLen] = ',';
            szOnceListTimeBuf[nstrColLen + 1] = '\0';
        }
        
        //拆分一次性时间序列，注:约定时间序列分隔符为'^'
        char *ss0 = NULL;
        char *ss1 = NULL;
        ss0 = szOnceListTimeBuf;
        while ((ss1 = strchr(ss0, ',')) != NULL)
        {
            *ss1 = '\0';
            
            //将时间格式(YYYY-MM-DD HH:mm:SS)转换为time_t
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
                //如果是过时的时间，不加入到一次性采集序列中
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
    //增加结束

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
    //关闭INI文件
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
    //删除过期文件
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
函 数 名  : CNetBakTask::CNetBakTask
功能描述  : 构造函数
输入参数  : int nTaskID
CBakTaskMgr* pBakTaskMgr
输出参数  : 无
返 回 值  :
调用函数  :
被调函数  :

  修改历史      :
  1.日    期   : 2002年9月6日
  作    者   : 张玉新
  修改内容   : 新生成函数

*****************************************************************************/
CNetBakTask::CNetBakTask( int nTaskID, CBakTaskMgr* pBakTaskMgr ):CBakTask(nTaskID,pBakTaskMgr)
{
    m_bIsAvailable = FALSE;
    m_LastConnTime = 0;
    m_nReConnInterval = 300;
    memset(m_szDestHostIP[0],0,MAX_HOST_LEN);      //目标主机的Ip（Local代表本地）
    memset(m_szDestHostIP[1],0,MAX_HOST_LEN);      //目标主机的Ip（Local代表本地）
    memset(m_szLocalHostIP[0],0,MAX_HOST_LEN);    //本机的IP
    memset(m_szLocalHostIP[1],0,MAX_HOST_LEN);    //本机的IP
    memset(m_szUserName,0,MAX_USERNAME_LEN);   //登录用户名
    memset(m_szPassword,0,MAX_PASSWORD_LEN);   //密码
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
函 数 名  : CNetBakTask::Backup
功能描述  : 连接FTP Server，搜索并备份文件
输入参数  : 无
输出参数  : 无
返 回 值  : void
调用函数  :
被调函数  :

      修改历史      :
      1.日    期   : 2002年9月6日
      作    者   : 张玉新
      修改内容   : 新生成函数

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
            if (Sleep(30))  //无License，休眠，继续
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
                    sInner.uAlarmID = ALARM_ID_BACKUP_CONNECT_FAIL;//连接失败
                    sInner.yAlarmPara[0] = m_nTaskID;
                    sInner.yAlarmType = AT_FAULT;
#ifdef _UNIT_TEST
                    printf("Send Alarm(ID=%d,Type=%d,Para=%d)",sInner.uAlarmID,sInner.yAlarmType,sInner.yAlarmPara);
#else
                    CWorkModule::SendAlarm(&sInner);
#endif
                    m_bSendAlarm = TRUE;
                    TRACE(MTS_BACKUP,S_BACKUP_CONNECT_FAIL,m_nTaskID);
                    MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_URGENT,           //将调试级别从次要改为紧急 ZhengYuqun SWPD01995 2003-10-21
                        S_BACKUP_CONNECT_FAIL,m_nTaskID);
                }
                
                throw ContinueException("Failed to connect ftp server");
            }
            else
            {
                if(m_bSendAlarm)
                {
                    SInnerAlarm sInner;
                    sInner.uAlarmID = ALARM_ID_BACKUP_CONNECT_FAIL;//连接失败
                    sInner.yAlarmPara[0] = m_nTaskID;
                    sInner.yAlarmType = AT_RESTORE;
                    CWorkModule::SendAlarm(&sInner);
                    
                    m_bSendAlarm = FALSE;
                }
            }            

            if(m_bActivateLog)
            {
                //更换稽核日志文件
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
                    {//问题单18680修改，张玉新2002-20-26
                        nRet = PrepareFiles(strTmpDir.c_str(),m_CurStatus.tFileTime,TRUE);
                    }
                    else
                    {
                        nRet= PrepareFiles(strTmpDir.c_str(),m_CurStatus.tFileTime,FALSE);
                    }
                    //修改结束
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
									   MSGOUT_LEVEL_URGENT,        //将调试级别从次要改为紧急 ZhengYuqun SWPD01995 2003-10-21
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
                //删除源文件的代码已经放到前后存盘了
                DelDestDirOrFile();
                SleepThrowException(0);
    
            }//while
        }//try
        catch(ContinueException m){
            if(strcmp(m.what(), "Sleep return FALSE") == 0)
            {
                m_bTransErrFlg = FALSE;
            }
            //发生故障情况
            else 
            {
                //Z30971修改问题单：SWPD05706 2004/08/23
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
函 数 名  : CNetBakTask::Connect
功能描述  : 连接FTP Server
输入参数  : 无
输出参数  : 无
返 回 值  : BOOL
调用函数  :
被调函数  :

  修改历史      :
  1.日    期   : 2002年9月6日
  作    者   : 张玉新
  修改内容   : 新生成函数

*****************************************************************************/
BOOL CNetBakTask::Connect()
{
    //设置m_LastConnTime为当前时间
    m_LastConnTime = time(NULL);
    //调用m_pFtpClient[m_nActiveIndex]->Connect()函数，失败返回FALSE
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

    //设置m_bIsAvailable为TRUE
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
函 数 名  : CNetBakTask::SetDir
功能描述  : 设置在FTP Server上的当前目录
输入参数  : const char* pszDirectory
输出参数  : 无
返 回 值  : BOOL
调用函数  :
被调函数  :

   修改历史      :
    1.日    期   : 2002年9月6日
      作    者   : 张玉新
      修改内容   : 新生成函数

*****************************************************************************/
BOOL CNetBakTask::SetDir( const char* pszDirectory )
{
    return (m_pFtpClient[m_nActiveIndex]->SetDirectory(pszDirectory) == 0);
}
/*****************************************************************************
函 数 名  : CNetBakTask::MakeDir
功能描述  : 在FTP Server上创建一个目录
输入参数  : const char* pszDirectory
输出参数  : 无
返 回 值  : BOOL
调用函数  :
被调函数  :

    修改历史      :
     1.日    期   : 2002年9月6日
       作    者   : 张玉新
       修改内容   : 新生成函数

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
        //一级一级创建目录
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
            nLen++; //"/" 的长度 ＝ 1
            DirNameList.pop_front();
        }
    }
    return TRUE;
}
/*****************************************************************************
函 数 名  : CNetBakTask::PutAFile
功能描述  : 往FTP Server上发送一个文件
输入参数  : const char* pszLocalFile
            const char* pszRemoteFile
输出参数  : 无
返 回 值  : BOOL
调用函数  :
被调函数  :

   修改历史      :
    1.日    期   : 2002年9月6日
      作    者   : 张玉新
      修改内容   : 新生成函数

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
        //Z30971修改问题单： SWPD07859 2004/08/24
        //可能有的FTP SERVER不能直接覆盖已有文件,则删之
        if (0 != m_pFtpClient[m_nActiveIndex]->DeleteFile(szTmpFile))
        {
            return FALSE;
        }
        
        //重新备份
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
        //Z30971修改问题单： SWPD07859 2004/08/24
        //有可能是因为上一次传送了一个不完整的文件后断连
        //造成有一个遗留文件，使的改名不成功,则删除原有的目标文件,然后再尝试改名
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
函 数 名  : CNetBakTask::DelDestDirOrFile
功能描述  : 删除过期备份文件或者目录
输入参数  : 无
输出参数  : 无
返 回 值  : int
调用函数  :
被调函数  :

   修改历史      :
    1.日    期   : 2002年9月6日
      作    者   : 张玉新
      修改内容   : 新生成函数

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
        //如果是目录但是目录的名字不是要求的数字就跳过
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
            //取目录时间
            nDate = atoi(FtpFile.szFileName);
            tFileDate.tm_year = nDate/10000 - 1900;
            tFileDate.tm_mon = (nDate%10000)/100 - 1;
            tFileDate.tm_mday = nDate%100;
            tTime = mktime(&tFileDate);
            //取当前时间
            time(&curTime);
            tTmp = *localtime(&curTime);
            tTmp.tm_hour = 0;
            tTmp.tm_min = 0;
            tTmp.tm_sec = 0;
            curTime = mktime(&tTmp); //得到当前的日期，时分秒都为0
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
函 数 名  : CNetBAkTask::GetCfg
功能描述  : 从INI文件中获取各个FTP备份任务的参数
输入参数  : 无
输出参数  : 无
返 回 值  : BOOL
调用函数  :
被调函数  :

   修改历史      :
    1.日    期   : 2002年9月6日
      作    者   : 张玉新
      修改内容   : 新生成函数

*****************************************************************************/
BOOL CNetBakTask::GetCfg()
{
    //调用父类的同名函数
    if(!CBakTask::GetCfg())
    {
        return FALSE;
    }

    //打开INI文件
    CINIFile ini(GetCfgFilePath());
    if(ini.Open() != TRUE)
    {
        TRACE(MTS_BACKUP,S_BACKUP_INI_OPEN_FAIL);
        return FALSE;
    }
    //从INI文件中[NetBackup]域中依次读取以下参数的配置
    //m_nReConnInterval、m_szUserName、m_szLocalIp
    //并记录该域的BeforeEncode、Password
    //配置bBeforeEncode1、szPassword1
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
    //从INI文件中[BackupTaskN]（N为备份任务ID）域中依次读取
    //以下参数的配置： m_nReConnInterval、m_szUserName、m_szLocalIp，
    //如果不是默认配置，参数的最终配置以此为准，
    //并记录该域的BeforeEncode、Password配置bBeforeEncod2、szPassword2
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

    //若bBeforeEncod2=FALSE转5），否则：设置m_szPassword=szPassword2，
    //设置[BackupTaskN]（N为m_nTaskID）域中的BeforeEncode为无效，
    //修改Password为加密后的密码，如果bBeforeEncod1=TRUE
    //并且szPassword1= szPassword2，修改[NetBackup]
    //域的BeforeEncode、Password；
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
    //若bBeforeEncod2=FALSE，将m_szPassword改为自身解密后的密码
    else
    {
        decode(szPassword2,m_szPassword);
    }

    //关闭INI文件
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
        //如果只有时
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

//等下一次采集任务,只考虑时间是否到了,不考虑比如备份成功或失败,重联等其他
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

        //Z30971修改问题单：SWPD05706 2004/08/23
        //上次传输过程发生故障,传输文件不成功
        if (m_bTransErrFlg) 
        {
            //重新连接时间为m_nReConnInterval
            if ((tNow - m_LastConnTime >=  m_nReConnInterval))
            {
                return true;
            }
    
        }
        else //传输正常
        {        
            //时间序列的优先级较高
            if(m_ListTime.size() > 0)
            {
                LIST<int>::iterator i;
                for(i = m_ListTime.begin(); i != m_ListTime.end(); i++)
                {
                    //这里考虑的是跨天的情况
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
            
            //modifyed by chenliangwei, 2003-08-05,增加else
            //时间范围
            else if(((tNowTime >= m_nBeginTime) && (tNowTime <= m_nEndTime))
                || ((0 == m_nBeginTime) && (0 == m_nEndTime)))
            {
                //added by chenliangwei, 2003-05-20
                //如果采集成功周期为0, 直接返回
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
            
            //最后再考虑单个采集任务
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


