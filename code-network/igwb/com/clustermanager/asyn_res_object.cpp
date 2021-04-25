#include "../include/base_type.h"
#include "com_cluster.h"
#include "asyn_res_object.h"


/********************************************************
* 名称: CAsynResObject::CAsynResObject
* 功能: 构造函数
* 输入: szResName   : 资源名
* 输出:
* 返回:
********************************************************/
CAsynResObject::CAsynResObject(const char* const szResName)
:m_nStatus(EASYNRES_DEACTIVE), m_pComCluster(NULL)
{
    strncpy(m_szResName, szResName, MAX_ASYN_RES_NAME);
    m_szResName[MAX_ASYN_RES_NAME - 1] = '\0';
}

/********************************************************
* 名称: CAsynResObject::~CAsynResObject
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CAsynResObject::~CAsynResObject()
{
    m_pComCluster = NULL;
}

/********************************************************
* 名称: CAsynResObject::GetStatus
* 功能: 获取异步资源状态
* 输入:
* 输出:
* 返回: EASYNRES_STATUS 异步资源状态
********************************************************/
EASYNRES_STATUS CAsynResObject::GetStatus()
{
    return m_nStatus;
}

/********************************************************
* 名称: CAsynResObject::SetStatus
* 功能: 设置异步资源状态
* 输入: EASYNRES_STATUS nStatus : 异步资源状态
* 输出:
* 返回: void
********************************************************/
void CAsynResObject::SetStatus(EASYNRES_STATUS nStatus)
{
    m_nStatus = nStatus;
}

/********************************************************
* 名称: CAsynResObject::GetResName
* 功能: 获取资源名称
* 输入:
* 输出:
* 返回: const char* 资源名称
********************************************************/
const char* CAsynResObject::GetResName()
{
    return m_szResName;
}

/********************************************************
* 名称: CAsynResObject::Attach
* 功能: 绑定CComCluster对象的指针
* 输入: pComCluster : CComCluster对象的指针
* 输出:
* 返回: void
********************************************************/
void CAsynResObject::Attach(CComCluster* pComCluster)
{
    m_pComCluster = pComCluster;
}

/********************************************************
* 名称: CAsynResObject::Create
* 功能: 资源创建函数
* 输入:
* 输出:
* 返回: int 资源创建是否成功，0表示成功，非零值表示失败
********************************************************/
int CAsynResObject::Create()
{
    m_nStatus = EASYNRES_DEACTIVE;

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CAsynResObject::Active
* 功能: 资源的激活函数
* 输入:
* 输出:
* 返回: int 是否激活成功，0表示成功，非零值表示启动失败
********************************************************/
int CAsynResObject::Active()
{
    m_nStatus = EASYNRES_ACTIVING;

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CAsynResObject::DeActive
* 功能: 资源的去激活函数
* 输入:
* 输出:
* 返回: int 是否去激活成功，0表示成功，非零值表示去激活失败
********************************************************/
int CAsynResObject::DeActive()
{
    m_nStatus = EASYNRES_DEACTIVING;

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CAsynResObject::OnReceive
* 功能: 处理对端发送的双机消息
* 输入: SClusterFrame* pFrame :
* 输出:
* 返回: void
********************************************************/
void CAsynResObject::OnReceive(SClusterFrame* pFrame)
{
    //
}

/********************************************************
* 名称: CAsynResObject::GetAttachedData
* 功能: 获取外带数据
* 输入:
* 输出: pData : 输出外带数据缓冲区的指针
*       nLen  : 输出外带数据缓冲区的长度
* 返回: int 返回输出外带数据缓冲区的长度
********************************************************/
int CAsynResObject::GetAttachedData(void*& pData, int& nLen)
{
    return 0;
}

/********************************************************
* 名称: CAsynResObject::ProcAttachedData
* 功能: 处理外带数据
* 输入: pData : 输入外带数据缓冲区的指针
*       nLen  : 输入外带数据缓冲区的长度
* 输出:
* 返回: void
********************************************************/
void CAsynResObject::ProcAttachedData(void* pData, int nLen)
{
    //
}

