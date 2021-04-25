/******************************************************************************

                  版权所有 (C), 2004-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : manual_task.h
  版 本 号   : iGWBV200R002B02
  作    者   : Zou Yongsheng
  生成日期   : 2004年5月20日
  最近修改   :
  功能描述   : 手工备份任务
  函数列表   :
  修改历史   :
    日    期   : 2004年5月20日
    作    者   : Zou Yongsheng
    修改内容   : 创建文件
    存在问题   : 暂不考虑文件池即一个话单文件对应多个备份任务的情况
                 前存盘最后一个文件的备份,需要在客户端屏蔽。

******************************************************************************/

#ifndef __MANUAL_TASK_H__
#define __MANUAL_TASK_H__

#include "backup_task.h"

class CManBakTask : public CNetBakTask
{
public:
    CManBakTask(int nClientNo, int nTaskID, CBakTaskMgr *pBakTaskMgr, const char* szPath);
               
    virtual ~CManBakTask();
    
    virtual int open(void* args = 0);

protected:
    virtual void Backup(); 
    virtual BOOL GetCfg();
    
    //准备目录szDir的子目录集合
    void PrepareDirs (const char* szDir);
    
    //准备目录szDir的文件集合
    void PrepareFiles(const char* szDir);
    
    //TRANSFER 话单文件
    int  PutFiles (const char* szSrcDir, const char* szDstDir);

    //根据备份命令返回的命令码,发送不同的信息
    void SendSimpleReport(int nBakRetCode);

protected:
    //范围标识,取值为DATE_RANGE和FILE_RANGE
    BYTE m_nNameFlag;
   
    char m_szManBakDir[MAX_PATH]; //备份目录
    STRINGLIST   m_FileList;      //文件列表
    STRINGLIST   m_DirList;       //目录列表

    BOOL m_bLocal;                //本地备份
    int  m_nClientNo;             //客户端号
    char m_szCmd[256];            //MML命令

};

class CManualStatus
{
public:
    //初始化函数,将文件列表记录到内存中
    BOOL Init();
   
    //查找文件节点
    BOOL FindFile(SFilesListNode &FileNode);
    
    //增加文件到备份列表中
    BOOL AddFile(SFilesListNode &FileNode);

    //从备份列表中删除文件
    BOOL DelFile(SFilesListNode& FileNode);

protected:
    //保存手工备份列表信息到文件中
    BOOL RefreshFile();

protected:
    //手工备份状态文件
    STRING           m_szFilePath;   
    
    //自动备份未备份,手工已经备份文件列表
    LISTFILELISTNODE m_BakedFileList;
    
    //互斥锁,保护已备份文件列表
    ACE_Mutex        m_Mutex;         
};
#endif//__MANUAL_TASK_H__