#ifndef _BACKUP_SERVER
#define _BACKUP_SERVER
/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : backup_server.h
  版 本 号   : 初稿
  作    者   : 张玉新
  生成日期   : 2002年9月8日
  最近修改   :
  功能描述   : 备份模块
  函数列表   :
  修改历史   :
  1.日    期   : 2002年9月8日
    作    者   : 张玉新
    修改内容   : 创建文件
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


//客户端号到手工备份任务信息的对应关系
typedef struct _ManBakTaskInfo
{
    CManBakTask* pTask;             //手工备份任务
    STRINGLIST   NameList;          //日期或文件列表
    BOOL         bEndCmd;           //收到最后一条部分命令标识
    int          nTaskId;           //自动备份线程ID
    int          nFlag;             //名称列表标识
    char         szCmd[256];        //回显命令

} SManBakTaskInfo, *PManBakTaskInfo;

//客户端号到手工删除任务信息的对应关系
typedef struct _ManDelTaskInfo
{
    STRINGLIST   NameList;          //日期或文件列表
    int          nTaskId;           //自动备份线程ID
    int          nFlag;             //名称列表标识
    char         szCmd[256];        //回显命令

} SManDelTaskInfo, *PManDelTaskInfo;


typedef MAP<int, SManBakTaskInfo*> MANUALBAKTASKMAP;
typedef MAP<int, SManDelTaskInfo*> MANUALDELTASKMAP;

class CBakTaskMgr : public ACE_Task_Base
{
protected:
    typedef MAP<int, CBakTask*> TASKMAP;

    TASKMAP        m_BakTaskMap;      //备份任务Map,键为taskID
    CBKStatusFile  m_StatusFile;      //所有备份任务共用一个状态文件，文件名后缀为bsf
    ACE_Event      m_ExitEvent;       //退出事件
    ACE_Mutex      m_BakMutex;        //线程互斥对象
    STRING         m_szBakStatusName; //备份状态文件名
    BOOL          *m_pbBakFatalFlag;  //备份任务故障状态标志
public:
    CBackupServer* m_pBackupServer;	  //备份模块指针

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
// 手工备份新增加成员函数和成员属性

protected:
    MANUALBAKTASKMAP  m_ManBakTaskMap;    //手工备份任务集合
    ACE_Recursive_Thread_Mutex       m_TaskMutex;        //手工备份任务互斥锁
    CManualStatus    *m_pManBakStatus;    //手工备份状态
   
    MANUALDELTASKMAP  m_ManDelTaskMap;    //手工删除任务映射

public:
    //处理手工备份任务
    BOOL ManualBakTaskProc(int nClientNo, const char* szCmd, int nTaskId,
                           const char* szBakPath, const char* szName,
                           int nFlag);
    //处理手工删除任务
    void ManualDelTaskProc(int nClientNo, const char* szCmd, int nTaskID,
                           const char* szDelPath, const char* szName,
                           int nFlag);
public:
    //获取相应的自动备份ID
    BOOL GetMatchTaskID(const char* szSrcDir, int& nTaskID);
    
    //判断是否备份过
    BOOL JudgeIsBackuped(const char* szFilePath, int nTaskID);
    //判断是否自动备份过
    BOOL IsAutoBackuped (const char* szFilePath, int nTaskID);
    //判断是否手工备份过
    BOOL IsManualBackuped(const char* szFilePath);

    //增加已备份文件到状态文件
    BOOL AddBakedFile(SFilesListNode& FileNode);
    //从状态文件中删除已备份文件
    BOOL DelBakedFile(SFilesListNode& FileNode);
    
    //设置任务完成标志
    BOOL SetTaskExitFlag(int nClientNo);

    //获取备份任务信息
    BOOL GetBakTaskInfo(int nClientNo, SManBakTaskInfo* &pBakTaskInfo);

protected:
    //删除手工备份任务信息
    BOOL DelManBakTask(int nClientNo);
    //删除手工删除任务信息
    BOOL DelManDelTask(int nClientNo);

    //清除手工备份任务信息
    void ClearManBakTasks();
    //清除手工删除任务信息
    void ClearManDelTasks();

    //删除结束的任务信息
    void DelExitBakTasks();

    //删除目录集合
    BOOL DelDirs (const char* szPath, STRINGLIST &DirList,  int nTaskID);
    //删除文件集合
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

	/* 为话单台保留的接口 */
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

    CMMLReport  *m_pMMLReport;          //MML报告构造器
    CBakTaskMgr *m_pBakTaskMgr;         //备份任务管理器
    
public:
    CBackupServer();
    virtual ~CBackupServer(){ };


/////////////////////////////////////////////////////////////////////////////
// 手工备份新增加成员函数和成员属性

public:
    //发送简单信息到客户端
    //void SendSimpleReport(const char* szCmd, int nRetCode,const char* szInfo,
                          //int nClientNo);
protected:
    //发送报文
    //void SendMMLReport(const char* szReport, int nClientNo);
    
    //手工备份处理
    void ReqBakCdrProc(MSG_INTRA *pMsg);
    //手工删除处理
    void ReqDelCdrProc(MSG_INTRA *pMsg);

protected:
    char  m_szOrigBasePath[MAX_PATH];   //原始话单根目录
    char  m_szFinalBasePath[MAX_PATH];  //最终话单根目录
   
};
#endif /* #ifndef _BACKUP_SERVER */
