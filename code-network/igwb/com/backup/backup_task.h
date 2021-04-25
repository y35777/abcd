#ifndef _BACKUP_TASK
#define _BACKUP_TASK
/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : backup_task.h
  �� �� ��   : ����
  ��    ��   : ������
  ��������   : 2002��9��8��
  ����޸�   :
  ��������   : ��������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2002��9��8��
    ��    ��   : ������
    �޸�����   : �����ļ�

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
    int m_nTaskID;	             //��������ID
    int m_nSearchFilesInterval;	 //�����ļ��ļ��
    STRING m_strDestHost;        //Ŀ��������
    char m_szSrcDir[MAX_PATH];	 //���ݵ�Դ·��
    char m_szDestDir[MAX_PATH];	 //���ݵ�Ŀ��·��
    BOOL m_bDelAfterBak;	     //��������Ƿ�ɾ��Դ�ļ�
    BOOL m_bBakLastFile;	     //�Ƿ񱸷�����һ���ļ�
    int m_nDesFileSavedDays;	 //Զ��Ftp Server�ļ���������
    int m_nBakAlmTimeout;	     //���ݸ澯��ʱʱ��
    BOOL m_bIsExit;	             //�����߳��Ƿ��Ѿ��˳�
    int m_nReStartTimes;	     //�����̵߳�ǰ�Ѿ�������������
    ACE_Event m_BakThdExitEvt;	 //�¼���������ֹͣ�����߳�
    int m_nCurOffset;	         //��ǰ״̬��Ϣ�����ļ��е�λ��
    SStatusNode m_CurStatus;	 //����״̬��Ϣ
    LISTFILELISTNODE m_FileList; //��ǰɨ�赽��Ҫ���ݵ��ļ��б�
    CBakTaskMgr *m_pBakTaskMgr;	 //��������������ָ��
    time_t m_LastBakOKTime;	     //�ϴα��ݳɹ�ʱ�䣬�ɹ����ͻ���û�����ļ�����ɹ�
    //������ 2003-04-07
    UINT4 m_uDelFileDays;        //ɾ����������
    LISTFILELISTNODE m_DelFileList; //��ɾ���ļ��б�
    //���ӽ���

    int m_nFilePoolID;           //�ļ���ID�����ڱ���Դ��ɾ��ʱ���ٲ�

    //���͵��ʶ
    STRING  m_strTaskLabel;

    //�Ƿ�������־����
    BOOL    m_bActivateLog;

    //��־���Ŀ¼
    STRING  m_strLogSaveDir;

    //��Ŀ���������־��ŵ�Ŀ¼
    STRING  m_strDestLogSaveDir;

    //������־�ļ����߳���
    static ACE_Mutex g_LogFileMutex;

    //�Ƿ��жϹ��ǵڶ��ݻ���,��׼�������ļ�ʱֻ�ж�һ��
    BOOL m_bIsJudged;         
    
    //�ڶ��ݻ�����ʶ,��׼�������ļ�ʱ�ж�
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
    {	//���m_nBakAlmTimeoutС�ڵ���0,����FALSE
        if(m_nBakAlmTimeout <= 0)
            return FALSE;

        //���m_LastBakOKTime�뵱ǰʱ������m_nBakAlmTimeout
        //����TRUE,���򷵻�FALSE
        if(time(NULL) - m_LastBakOKTime > m_nBakAlmTimeout)
            return TRUE;
        else
            return FALSE;
    }
    virtual BOOL GetCfg();
	
    //�ж��Ƿ��ѱ��ݹ�
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

    //���������־��Ϣ
	virtual int SaveCheckLog(const char* szFileName,
                             const char* szSrcPath,
                             const char* szDestPath);

    //����������־�ļ�
    virtual int MakeNewCheckLogFile();

    void ClearList();	
    class Compare
    {    
    public:
        bool operator ()(const STRING &lhs, const STRING &rhs)
        {        
            return (strcmp(lhs.c_str(),rhs.c_str()) < 0);
        }
    };//�������	
};

class CNetBakTask : public CBakTask
{	
protected:
	char m_szLocalHostIP[2][MAX_HOST_LEN];  //������IP
    char m_szDestHostIP[2][MAX_HOST_LEN];	//Ŀ��������Ip��Local�����أ�
	char m_szUserName[MAX_USERNAME_LEN];    //��¼�û���
	char m_szPassword[MAX_PASSWORD_LEN];    //����
	CFtpClient*     m_pFtpClient[2];        //FTP�ͻ��˶���
	int             m_nActiveIndex;         //��ǰ����FTP�ͻ��˶����ָ��
	time_t          m_LastConnTime;	        //�ϴ���FTP Server���ӵ�ʱ��
	BOOL            m_bIsAvailable;	        //��ǰ�ļ�����״̬�Ƿ�����
	STRING          m_strLastSendFile;      //��һ���͵��ļ���
    int             m_nResendCount;         //ͬһ���ļ��Ĵ������Դ���
    BOOL            m_bSendAlarm;           //��������ʧ�ܸ澯�ı�־
	int             m_nFtpIsPassive;
    int             m_nReTryTimes;          //һ���ļ�����ʧ�ܺ��������ӵĴ���
    int             m_nReConnInterval;	    //��������FTP Server��ʱ����
    
    //Z30971�޸����ⵥ��SWPD05706 2004/08/23
    int             m_nRetryTimesCount;     //���Դ��������� ��
    BOOL            m_bTransErrFlg;         //������ϱ�־
    
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
    //���������־��Ϣ
	virtual int SaveCheckLog(const char* szFileName,
                             const char* szSrcPath,
                             const char* szDestPath);

    //����������־�ļ�
    virtual int MakeNewCheckLogFile();
    
    virtual bool WaitNextTime();
};

//�쳣����
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
