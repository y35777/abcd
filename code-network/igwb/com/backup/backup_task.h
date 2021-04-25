#ifndef _BACKUP_TASK
#define _BACKUP_TASK
/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : backup_task.h
  版 本 号   : 初稿
  作    者   : 张玉新
  生成日期   : 2002年9月8日
  最近修改   :
  功能描述   : 备份任务
  函数列表   :
  修改历史   :
  1.日    期   : 2002年9月8日
    作    者   : 张玉新
    修改内容   : 创建文件

******************************************************************************/
#include "../toolbase/ftp_client.h"
#include "../utility/encrypt.h"
#include "backup_server.h"
#include <algorithm>
#define LIST2 std::deque

class CBakTaskMgr;

class CBakTask : public ACE_Task_Base
{
protected:
    //add by yangguang
    int             m_nBeginTime;
    int             m_nEndTime;
    LIST <int>      m_ListTime;
    LIST <time_t>   m_OnceListTime;
    //ACE_Recursive_Thread_Mutex m_MutexForSchl;
    //add end   
  
public:
    inline BOOL IsMatched(const char* szSrcDir) const
    {
        STRING szLong = szSrcDir;
        STRING szShort = m_szSrcDir;

        std::replace(szLong.begin(), szLong.end(), '\\', '/');
        std::replace(szShort.begin(), szShort.end(), '\\', '/');

		StringToUpper((char*)szLong.c_str());
		StringToUpper((char*)szShort.c_str());
        return (NULL != strstr(szLong.c_str(), szShort.c_str()));
    };


protected:
    int m_nTaskID;	             //备份任务ID
    int m_nSearchFilesInterval;	 //搜索文件的间隔
    STRING m_strDestHost;        //目标主机名
    char m_szSrcDir[MAX_PATH];	 //备份的源路径
    char m_szDestDir[MAX_PATH];	 //备份的目的路径
    BOOL m_bDelAfterBak;	     //备份完成是否删除源文件
    BOOL m_bBakLastFile;	     //是否备份最新一个文件
    int m_nDesFileSavedDays;	 //远端Ftp Server文件保留天数
    int m_nBakAlmTimeout;	     //备份告警超时时间
    BOOL m_bIsExit;	             //备份线程是否已经退出
    int m_nReStartTimes;	     //备份线程当前已经重新启动次数
    ACE_Event m_BakThdExitEvt;	 //事件对象，用于停止备份线程
    int m_nCurOffset;	         //当前状态信息所在文件中的位置
    SStatusNode m_CurStatus;	 //备份状态信息
    LISTFILELISTNODE m_FileList; //当前扫描到需要备份的文件列表
    CBakTaskMgr *m_pBakTaskMgr;	 //备份任务管理对象指针
    time_t m_LastBakOKTime;	     //上次备份成功时间，成功发送或者没有新文件都算成功
    //张玉新 2003-04-07
    UINT4 m_uDelFileDays;        //删除日期期限
    LISTFILELISTNODE m_DelFileList; //待删除文件列表
    //增加结束

    int m_nFilePoolID;           //文件池ID，用于备份源的删除时间仲裁

    //传送点标识
    STRING  m_strTaskLabel;

    //是否启动日志特性
    BOOL    m_bActivateLog;

    //日志存放目录
    STRING  m_strLogSaveDir;

    //在目标机器上日志存放的目录
    STRING  m_strDestLogSaveDir;

    //保存日志文件的线程锁
    static ACE_Mutex g_LogFileMutex;

    //是否判断过是第二份话单,在准备话单文件时只判断一次
    BOOL m_bIsJudged;         
    
    //第二份话单标识,在准备话单文件时判断
    BOOL m_bIsSecond;         

public: 
    CBakTask(int nTaskID, CBakTaskMgr *pBakTaskMgr);
    virtual ~CBakTask(){}

    virtual int open(void* args = 0);
    virtual int svc();
    virtual void Stop ();
	
    inline const int GetTaskID()
    {	return m_nTaskID; }
	
    inline const BOOL IsExit()
    {	return m_bIsExit; }

    inline void SignalExitFlag()
    {	m_BakThdExitEvt.signal(); }

    inline const BOOL IsBakTimeout()
    {	//如果m_nBakAlmTimeout小于等于0,返回FALSE
        if(m_nBakAlmTimeout <= 0)
            return FALSE;

        //如果m_LastBakOKTime与当前时间差大于m_nBakAlmTimeout
        //返回TRUE,否则返回FALSE
        if(time(NULL) - m_LastBakOKTime > m_nBakAlmTimeout)
            return TRUE;
        else
            return FALSE;
    }
    virtual BOOL GetCfg();
	
    //判断是否已备份过
    BOOL JudgeIsBackupped(const char* szFileName, time_t tFileTime);
 
    
protected:
	void DelOutDateFile();
    BOOL WriteStatusFile(SStatusNode &CurStatus);
    int GetStatInfo (const char* pszTaskName, SStatusNode *pStatInfo);
    int PrepareFiles(const char* szPath, time_t tFileTime, BOOL bRmvLastFile);
    inline void PrepareSubDirs(const char* pszDir, LIST2<STRING> &SubDirList);
    void PrepareSubDirs_Impl(const char* pszDir, LIST2<STRING> &SubDirList);	
    int PutFiles(const char* szSrcPath, const char* szDestPath);
    virtual BOOL PutAFile(const char *pszLocalFile, const char *pszRemoteFile);   
	virtual void  Backup();
    BOOL Sleep(int nSeconds);
    inline void SleepThrowException(int nSeconds);
    virtual int DelDestDirOrFile();
    int DelDirOrFile_Impl(const char *pszName,int nSaveDays);

//add by yangguang
protected:
    virtual bool WaitNextTime();
    virtual int GetTimeFromString(const char* const szTime); //copy from clw
    virtual int SaveOnceColSchl();//copy from ccs clw
	//virtual int DelSrcDirOrFile();

    //保存稽核日志信息
	virtual int SaveCheckLog(const char* szFileName,
                             const char* szSrcPath,
                             const char* szDestPath);

    //更换稽核日志文件
    virtual int MakeNewCheckLogFile();

    void ClearList();	
    class Compare
    {    
    public:
        bool operator ()(const STRING &lhs, const STRING &rhs)
        {        
            return (strcmp(lhs.c_str(),rhs.c_str()) < 0);
        }
    };//定义结束	
};

class CNetBakTask : public CBakTask
{	
protected:
	char m_szLocalHostIP[2][MAX_HOST_LEN];  //本机的IP
    char m_szDestHostIP[2][MAX_HOST_LEN];	//目标主机的Ip（Local代表本地）
	char m_szUserName[MAX_USERNAME_LEN];    //登录用户名
	char m_szPassword[MAX_PASSWORD_LEN];    //密码
	CFtpClient*     m_pFtpClient[2];        //FTP客户端对象
	int             m_nActiveIndex;         //当前激活FTP客户端对象的指针
	time_t          m_LastConnTime;	        //上次与FTP Server连接的时间
	BOOL            m_bIsAvailable;	        //当前文件传输状态是否正常
	STRING          m_strLastSendFile;      //上一次送的文件名
    int             m_nResendCount;         //同一个文件的传输重试次数
    BOOL            m_bSendAlarm;           //发送连接失败告警的标志
	int             m_nFtpIsPassive;
    int             m_nReTryTimes;          //一次文件传输失败后重新连接的次数
    int             m_nReConnInterval;	    //重新连接FTP Server的时间间隔
    
    //Z30971修改问题单：SWPD05706 2004/08/23
    int             m_nRetryTimesCount;     //重试次数计数器 、
    BOOL            m_bTransErrFlg;         //传输故障标志
    
public:
	CNetBakTask(int nTaskID,CBakTaskMgr *pBakTaskMgr);

	virtual ~CNetBakTask();
protected:
	virtual void  Backup();
	BOOL Connect();
	BOOL SetDir(const char* pszDirectory);
	BOOL MakeDir(const char* pszDirectory);
	virtual BOOL PutAFile(const char* pszLocalFile, const char* pszRemoteFile);
	virtual int DelDestDirOrFile();
	virtual BOOL GetCfg();
    //保存稽核日志信息
	virtual int SaveCheckLog(const char* szFileName,
                             const char* szSrcPath,
                             const char* szDestPath);

    //更换稽核日志文件
    virtual int MakeNewCheckLogFile();
    
    virtual bool WaitNextTime();
};

//异常定义
class Exception {
	char szException[80];
public:
    Exception(const char *szInfo){
		strncpy(szException,szInfo,80);
		szException[79] = 0;
	}
    virtual ~Exception() throw(){ }
    virtual const char *what(){
		return szException;
	}
};

class ExitException:public Exception{
public:
	ExitException(const char *szInfo):Exception(szInfo){ }
};

class ContinueException : public Exception{
public:
	ContinueException(const char *szInfo):Exception(szInfo){ }
};

#endif /* _BACKUP_TASK */
