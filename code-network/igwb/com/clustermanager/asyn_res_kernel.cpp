#include "../include/frame.h"
#include "com_cluster.h"
#include "cluster_module.h"
#include "asyn_res_kernel.h"


/********************************************************
* 名称: CAsynResKernel::CAsynResKernel
* 功能: 构造函数
* 输入: szAppPath      : Kernel进程的全路径名
*       pClusterModule : CClusterModule对象的指针
*       szResName      : 资源名
*       pComCluster    : CComCluster对象的指针
* 输出:
* 返回:
********************************************************/
CAsynResKernel::CAsynResKernel(const char* const szAppPath,
                               CClusterModule* pClusterModule,
                               const char* const szResName)
                               :CAsynResObject(szResName)
{
    strncpy(m_szAppPath, szAppPath, MAX_PATH);
    m_szAppPath[MAX_PATH - 1] = '\0';

    assert(NULL != pClusterModule);
    m_pClusterModule = pClusterModule;

    m_nPid           = ACE_INVALID_PID;
}

/********************************************************
* 名称: CAsynResKernel::~CAsynResKernel
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CAsynResKernel::~CAsynResKernel()
{
    m_pClusterModule = NULL;
}

/********************************************************
* 名称: CAsynResKernel::AcquireLock
* 功能: 申请互斥锁
* 输入: 
* 输出: 
* 返回: void 
********************************************************/
void CAsynResKernel::AcquireLock()
{
    m_Mutex.acquire();
}

/********************************************************
* 名称: CAsynResKernel::ReleaseLock
* 功能: 释放互斥锁
* 输入: 
* 输出: 
* 返回: void 
********************************************************/
void CAsynResKernel::ReleaseLock()
{
    m_Mutex.release();
}

/********************************************************
* 名称: CAsynResKernel::SetPidInValid
* 功能: 设置PID为无效
* 输入: 
* 输出: 
* 返回: void 
********************************************************/
void CAsynResKernel::SetPidInValid()
{
    m_nPid = ACE_INVALID_PID;
}

/********************************************************
* 名称: CAsynResKernel::Create
* 功能: 资源创建函数
* 输入:
* 输出:
* 返回: int 资源创建是否成功，0表示成功，非零值表示失败
********************************************************/
int CAsynResKernel::Create()
{

    //调用ACE_OS::access函数检查KERNEL进程文件是否存在
    if (ACE_OS::access(m_szAppPath, F_OK) != 0)
    {
        return ERR_FILE_NOT_PRESENT;
    }

    //根据m_szAppPath进程全路径名，初始化m_POptions
    m_POptions.command_line("%s", m_szAppPath);

    m_nStatus = EASYNRES_DEACTIVE;
    return ERR_SUCCESS;
}

/********************************************************
* 名称: CAsynResKernel::Active
* 功能: 资源的激活函数
* 输入:
* 输出:
* 返回: int 是否激活成功，0表示成功，非零值表示启动失败
********************************************************/
int CAsynResKernel::Active()
{
    AcquireLock();

    int nRet = ERR_SUCCESS;
    if((ACE_INVALID_PID == m_nPid) || (CheckStatus() != ERR_SUCCESS))
    {
        m_pClusterModule->Active();

        m_nStatus = EASYNRES_ACTIVING;
        m_nPid    = m_PMgr.spawn(m_POptions);

        if(m_nPid == ACE_INVALID_PID)
        {
            nRet = ERR_FAIL;
        }
    }

    ReleaseLock();
    
    return nRet;
}

/********************************************************
* 名称: CAsynResKernel::CheckStatus
* 功能: 资源的状态检查函数
* 输入:
* 输出:
* 返回: int 是否存在不可恢复的故障，0表示无故障，非零值表示检测到故障
********************************************************/
int CAsynResKernel::CheckStatus()
{
    int nRet = m_PMgr.wait(m_nPid, ACE_Time_Value(0));

    return nRet;
}

/********************************************************
* 名称: CAsynResKernel::DeActive
* 功能: 资源的去激活函数
* 输入:
* 输出:
* 返回: int 是否去激活成功，0表示成功，非零值表示去激活失败
********************************************************/
int CAsynResKernel::DeActive()
{
    AcquireLock();

    m_nStatus = EASYNRES_DEACTIVING;
    int nRet = m_pClusterModule->DeActive();

    ReleaseLock();

    return nRet;
}

/********************************************************
* 名称: CAsynResKernel::terminate
* 功能: 强行杀掉Kernel
* 输入:
* 输出:
* 返回: void
********************************************************/
void CAsynResKernel::Terminate()
{
    ACE::terminate_process(m_nPid);
}

/********************************************************
* 名称: CAsynResKernel::OnReceive
* 功能: 处理对端发送的双机消息
* 输入: SClusterFrame* pFrame :
* 输出:
* 返回: void
********************************************************/
void CAsynResKernel::OnReceive(SClusterFrame* pFrame)
{
    switch(pFrame->nCmdCode)
    {
    case CMD_CLS_SYNC_DATA:
        //分发同步数据
        if (NULL != m_pClusterModule)
        {
            m_pClusterModule->OnDoSyncData(pFrame->Data, pFrame->nLen);
        }
        break;

    default:
        break;
    }
}

/********************************************************
* 名称: CAsynResKernel::GetAttachedData
* 功能: 获取外带数据
* 输入:
* 输出: pData : 输出外带数据缓冲区的指针
*       nLen  : 输出外带数据缓冲区的长度
* 返回: int 返回输出外带数据缓冲区的长度
********************************************************/
int CAsynResKernel::GetAttachedData(void*& pData, int& nLen)
{
    //获得外带数据
    if (NULL == m_pClusterModule)
    {
        pData = NULL;
        nLen  = 0;
        return 0;
    }

    return m_pClusterModule->GetSyncData(pData, nLen);
}

/********************************************************
* 名称: CAsynResKernel::ProcAttachedData
* 功能: 处理外带数据
* 输入: pData : 输入外带数据缓冲区的指针
*       nLen  : 输入外带数据缓冲区的长度
* 输出:
* 返回: void
********************************************************/
void CAsynResKernel::ProcAttachedData(void* pData, int nLen)
{
    //分发同步数据
    if (NULL != m_pClusterModule)
    {
        m_pClusterModule->OnDoSyncData(pData, nLen);
    }
}

