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

    void Terminate();           //强行杀掉Kernel
    void SetPidInValid();       //设置PID为无效
    
    void AcquireLock();         //申请互斥锁
    void ReleaseLock();         //释放互斥锁

    virtual int CheckStatus();  //资源的状态检查函数
    virtual int Create();       //资源创建函数
    virtual int Active();       //资源的激活函数
    virtual int DeActive();     //资源的去激活函数

    //处理对端发送的双机消息
    virtual void OnReceive(SClusterFrame* pFrame);
    //获取外带数据
    virtual int GetAttachedData(void*& pData, int& nLen);
    //处理外带数据
    virtual void ProcAttachedData(void* pData, int nLen);

protected:
   //KERNEL进程的全路径名
   char                         m_szAppPath[MAX_PATH];

   //CClusterModule对象的指针
   CClusterModule*              m_pClusterModule;

   //进程PID号
   pid_t                        m_nPid;

   //ACE的进程管理对象
   ACE_Process_Manager          m_PMgr;

   //进程的ACE环境
   ACE_Process_Options          m_POptions;

   //ACE提供的递归线程锁
   ACE_Recursive_Thread_Mutex   m_Mutex;
};

#endif //__ASYN_RESOURCE_KERNEL_H__
