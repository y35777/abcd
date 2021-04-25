#ifndef __ASYN_RESOURCE_KERNEL_H__
#define __ASYN_RESOURCE_KERNEL_H__

#include "asyn_res_object.h"

class CComCluster;
class CClusterModule;

class CAsynResKernel : public CAsynResObject
{
public:
    CAsynResKernel(const char* const szAppPath,
                   CClusterModule* pClusterModule,
                   const char* const szResName);
    virtual ~CAsynResKernel();

    void Terminate();           //ǿ��ɱ��Kernel
    void SetPidInValid();       //����PIDΪ��Ч
    
    void AcquireLock();         //���뻥����
    void ReleaseLock();         //�ͷŻ�����

    virtual int CheckStatus();  //��Դ��״̬��麯��
    virtual int Create();       //��Դ��������
    virtual int Active();       //��Դ�ļ����
    virtual int DeActive();     //��Դ��ȥ�����

    //����Զ˷��͵�˫����Ϣ
    virtual void OnReceive(SClusterFrame* pFrame);
    //��ȡ�������
    virtual int GetAttachedData(void*& pData, int& nLen);
    //�����������
    virtual void ProcAttachedData(void* pData, int nLen);

protected:
   //KERNEL���̵�ȫ·����
   char                         m_szAppPath[MAX_PATH];

   //CClusterModule�����ָ��
   CClusterModule*              m_pClusterModule;

   //����PID��
   pid_t                        m_nPid;

   //ACE�Ľ��̹������
   ACE_Process_Manager          m_PMgr;

   //���̵�ACE����
   ACE_Process_Options          m_POptions;

   //ACE�ṩ�ĵݹ��߳���
   ACE_Recursive_Thread_Mutex   m_Mutex;
};

#endif //__ASYN_RESOURCE_KERNEL_H__
