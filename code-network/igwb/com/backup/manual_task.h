/******************************************************************************

                  ��Ȩ���� (C), 2004-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : manual_task.h
  �� �� ��   : iGWBV200R002B02
  ��    ��   : Zou Yongsheng
  ��������   : 2004��5��20��
  ����޸�   :
  ��������   : �ֹ���������
  �����б�   :
  �޸���ʷ   :
    ��    ��   : 2004��5��20��
    ��    ��   : Zou Yongsheng
    �޸�����   : �����ļ�
    ��������   : �ݲ������ļ��ؼ�һ�������ļ���Ӧ���������������
                 ǰ�������һ���ļ��ı���,��Ҫ�ڿͻ������Ρ�

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
    
    //׼��Ŀ¼szDir����Ŀ¼����
    void PrepareDirs (const char* szDir);
    
    //׼��Ŀ¼szDir���ļ�����
    void PrepareFiles(const char* szDir);
    
    //TRANSFER �����ļ�
    int  PutFiles (const char* szSrcDir, const char* szDstDir);

    //���ݱ�������ص�������,���Ͳ�ͬ����Ϣ
    void SendSimpleReport(int nBakRetCode);

protected:
    //��Χ��ʶ,ȡֵΪDATE_RANGE��FILE_RANGE
    BYTE m_nNameFlag;
   
    char m_szManBakDir[MAX_PATH]; //����Ŀ¼
    STRINGLIST   m_FileList;      //�ļ��б�
    STRINGLIST   m_DirList;       //Ŀ¼�б�

    BOOL m_bLocal;                //���ر���
    int  m_nClientNo;             //�ͻ��˺�
    char m_szCmd[256];            //MML����

};

class CManualStatus
{
public:
    //��ʼ������,���ļ��б��¼���ڴ���
    BOOL Init();
   
    //�����ļ��ڵ�
    BOOL FindFile(SFilesListNode &FileNode);
    
    //�����ļ��������б���
    BOOL AddFile(SFilesListNode &FileNode);

    //�ӱ����б���ɾ���ļ�
    BOOL DelFile(SFilesListNode& FileNode);

protected:
    //�����ֹ������б���Ϣ���ļ���
    BOOL RefreshFile();

protected:
    //�ֹ�����״̬�ļ�
    STRING           m_szFilePath;   
    
    //�Զ�����δ����,�ֹ��Ѿ������ļ��б�
    LISTFILELISTNODE m_BakedFileList;
    
    //������,�����ѱ����ļ��б�
    ACE_Mutex        m_Mutex;         
};
#endif//__MANUAL_TASK_H__