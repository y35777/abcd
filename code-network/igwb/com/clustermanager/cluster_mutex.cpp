#include "../include/base_type.h"
#include "cluster_mutex.h"

CClusterMutex* CClusterMutex::g_pTheMutex = NULL;

/********************************************************
* 名称: CClusterMutex::CClusterMutex
* 功能: 构造函数
* 输入:
* 输出:
* 返回:
********************************************************/
CClusterMutex::CClusterMutex()
{
    //
}

/********************************************************
* 名称: CClusterMutex::~CClusterMutex
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CClusterMutex::~CClusterMutex()
{
    //
}

/********************************************************
* 名称: CClusterMutex::Instance
* 功能: 获得单实例对象的指针
* 输入:
* 输出:
* 返回: CClusterMutex* const
********************************************************/
CClusterMutex* const CClusterMutex::Instance()
{
    if(g_pTheMutex == NULL)
    {
        //创建一个CClusterMutex对象
        g_pTheMutex = new CClusterMutex;
    }
    return g_pTheMutex;
}

/********************************************************
* 名称: CClusterMutex::Destroy
* 功能: 删除单实例对象
* 输入:
* 输出:
* 返回: void
********************************************************/
void CClusterMutex::Destroy()
{
    if(g_pTheMutex != NULL)
    {
        delete g_pTheMutex;
        g_pTheMutex = 0;
    }
}

/********************************************************
* 名称: CClusterMutex::Acquire
* 功能: 获得互斥量
* 输入:
* 输出:
* 返回: void
********************************************************/
void CClusterMutex::Acquire()
{
    m_AceMutex.acquire();
}

/********************************************************
* 名称: CClusterMutex::Release
* 功能: 释放互斥量
* 输入:
* 输出:
* 返回: void
********************************************************/
void CClusterMutex::Release()
{
    m_AceMutex.release();
}
