#ifndef _BACKUP_SERVER
#define _BACKUP_SERVER
/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : backup_server.h
  �� �� ��   : ����
  ��    ��   : ������
  ��������   : 2002��9��8��
  ����޸�   :
  ��������   : ����ģ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2002��9��8��
    ��    ��   : ������
    �޸�����   : �����ļ�
******************************************************************************/
#include "../include/base_type.h"
#include "../include/resource.h"
#include "../include/frame.h"
#include "../include/system1.h"
#include "../utility/mml_para.h"
#include "../utility/mml_report.h"
#include "../utility/config.h"
#include "../toolbase/ini_file.h"
#include "ace/Task.h"
#include "bk_status_file.h"
#include "resource.h"

class CBakTask;
class CBackupServer;
class CManualStatus;
class CManBakTask;


//�ͻ��˺ŵ��ֹ�����������Ϣ�Ķ�Ӧ��ϵ
typedef struct _ManBakTaskInfo
{
    CManBakTask* pTask;             //�ֹ���������
    STRINGLIST   NameList;          //���ڻ��ļ��б�
    BOOL         bEndCmd;           //�յ����һ�����������ʶ
    int          nTaskId;           //�Զ������߳�ID
    int          nFlag;             //�����б��ʶ
    char         szCmd[256];        //��������

} SManBakTaskInfo, *PManBakTaskInfo;

//�ͻ��˺ŵ��ֹ�ɾ��������Ϣ�Ķ�Ӧ��ϵ
typedef struct _ManDelTaskInfo
{
    STRINGLIST   NameList;          //���ڻ��ļ��б�
    int          nTaskId;           //�Զ������߳�ID
    int          nFlag;             //�����б��ʶ
    char         szCmd[256];        //��������

} SManDelTaskInfo, *PManDelTaskInfo;


typedef MAP<int, SManBakTaskInfo*> MANUALBAKTASKMAP;
typedef MAP<int, SManDelTaskInfo*> MANUALDELTASKMAP;

class CBakTaskMgr : public ACE_Task_Base
{
protected:
    typedef MAP<int, CBakTask*> TASKMAP;

    TASKMAP        m_BakTaskMap;      //��������Map,��ΪtaskID
    CBKStatusFile  m_StatusFile;      //���б���������һ��״̬�ļ����ļ�����׺Ϊbsf
    ACE_Event      m_ExitEvent;       //�˳��¼�
    ACE_Mutex      m_BakMutex;        //�̻߳������
    STRING         m_szBakStatusName; //����״̬�ļ���
    BOOL          *m_pbBakFatalFlag;  //�����������״̬��־
public:
    CBackupServer* m_pBackupServer;	  //����ģ��ָ��

public:
	CBakTaskMgr(CBackupServer* pBackupServer);
	~CBakTaskMgr();
	virtual int open(void* args = 0);
	int GetStatInfo(const char* pszTaskName, SStatusNode *pStatInfo);
	BOOL WriteStatusFile(SStatusNode &Data);
	void Exit();

    int GetLicenseCount();

protected:
	BOOL Sleep(int nSeconds);
	int  CreateBakTasks();
	void Clear();
	virtual int svc();
	int StartAllTasks();


/////////////////////////////////////////////////////////////////////////////
// �ֹ����������ӳ�Ա�����ͳ�Ա����

protected:
    MANUALBAKTASKMAP  m_ManBakTaskMap;    //�ֹ��������񼯺�
    ACE_Recursive_Thread_Mutex       m_TaskMutex;        //�ֹ��������񻥳���
    CManualStatus    *m_pManBakStatus;    //�ֹ�����״̬
   
    MANUALDELTASKMAP  m_ManDelTaskMap;    //�ֹ�ɾ������ӳ��

public:
    //�����ֹ���������
    BOOL ManualBakTaskProc(int nClientNo, const char* szCmd, int nTaskId,
                           const char* szBakPath, const char* szName,
                           int nFlag);
    //�����ֹ�ɾ������
    void ManualDelTaskProc(int nClientNo, const char* szCmd, int nTaskID,
                           const char* szDelPath, const char* szName,
                           int nFlag);
public:
    //��ȡ��Ӧ���Զ�����ID
    BOOL GetMatchTaskID(const char* szSrcDir, int& nTaskID);
    
    //�ж��Ƿ񱸷ݹ�
    BOOL JudgeIsBackuped(const char* szFilePath, int nTaskID);
    //�ж��Ƿ��Զ����ݹ�
    BOOL IsAutoBackuped (const char* szFilePath, int nTaskID);
    //�ж��Ƿ��ֹ����ݹ�
    BOOL IsManualBackuped(const char* szFilePath);

    //�����ѱ����ļ���״̬�ļ�
    BOOL AddBakedFile(SFilesListNode& FileNode);
    //��״̬�ļ���ɾ���ѱ����ļ�
    BOOL DelBakedFile(SFilesListNode& FileNode);
    
    //����������ɱ�־
    BOOL SetTaskExitFlag(int nClientNo);

    //��ȡ����������Ϣ
    BOOL GetBakTaskInfo(int nClientNo, SManBakTaskInfo* &pBakTaskInfo);

protected:
    //ɾ���ֹ�����������Ϣ
    BOOL DelManBakTask(int nClientNo);
    //ɾ���ֹ�ɾ��������Ϣ
    BOOL DelManDelTask(int nClientNo);

    //����ֹ�����������Ϣ
    void ClearManBakTasks();
    //����ֹ�ɾ��������Ϣ
    void ClearManDelTasks();

    //ɾ��������������Ϣ
    void DelExitBakTasks();

    //ɾ��Ŀ¼����
    BOOL DelDirs (const char* szPath, STRINGLIST &DirList,  int nTaskID);
    //ɾ���ļ�����
    BOOL DelFiles(const char* szPath, STRINGLIST &FileList, int nTaskID);
   
};

class CBackupServer: public CWorkModule
{
protected:
	void ParseMsg(MSG_INTRA* pMsg);
    void OnTimer(const long nTimerID);
    virtual void ExitInstance();
    virtual int InitInstance();
    virtual MODULE_TYPE GetModuleType();

	/* Ϊ����̨�����Ľӿ� */
    void ReqModifyBackupProc(MSG_INTRA *pMsg);
    void ReqQueryBackupProc(MSG_INTRA *pMsg);
    
	/* om modify add by yangguang ---begin*/

	void BkpCfgFile(MSG_INTRA* pMsg);
	void UldCfgFile(MSG_INTRA* pMsg);
	void DldCfgFile(MSG_INTRA* pMsg);
	void RtrCfgFile(MSG_INTRA* pMsg);
	void SendMMLReport(const char* szReport, int nClientNo);
public:	
	void SendSimpleReport(const char* szCmd, int nRetCode,
		  				  const char* szInfo,int nClientNo);
protected:

	/* om modify add by yangguang ---end*/    

    CMMLReport  *m_pMMLReport;          //MML���湹����
    CBakTaskMgr *m_pBakTaskMgr;         //�������������
    
public:
    CBackupServer();
    virtual ~CBackupServer(){ };


/////////////////////////////////////////////////////////////////////////////
// �ֹ����������ӳ�Ա�����ͳ�Ա����

public:
    //���ͼ���Ϣ���ͻ���
    //void SendSimpleReport(const char* szCmd, int nRetCode,const char* szInfo,
                          //int nClientNo);
protected:
    //���ͱ���
    //void SendMMLReport(const char* szReport, int nClientNo);
    
    //�ֹ����ݴ���
    void ReqBakCdrProc(MSG_INTRA *pMsg);
    //�ֹ�ɾ������
    void ReqDelCdrProc(MSG_INTRA *pMsg);

protected:
    char  m_szOrigBasePath[MAX_PATH];   //ԭʼ������Ŀ¼
    char  m_szFinalBasePath[MAX_PATH];  //���ջ�����Ŀ¼
   
};
#endif /* #ifndef _BACKUP_SERVER */
