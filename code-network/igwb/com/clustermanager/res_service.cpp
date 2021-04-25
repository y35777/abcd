#include "../include/base_type.h"
#include "res_service.h"
#include "BtwService.h"


/********************************************************
* 名称: CResService::CResService
* 功能: 构造函数
* 输入: const char* const szServiceName : 服务名
*       const char* const szResName     : 资源名
*       ERES_TYPE nResType              : 资源类型
*       int nMaxLocalRecovery           : 最大本地恢复次数
* 输出:
* 返回:
********************************************************/
CResService::CResService(const char* const szServiceName,
                         const char* const szResName,
                         ERES_TYPE nResType,
                         int nMaxLocalRecovery):
CResObject(szResName, nResType, nMaxLocalRecovery)
{
    strncpy(m_szServiceName, szServiceName, MAX_SERVICE_NAME);
    m_szServiceName[MAX_SERVICE_NAME - 1] = '\0';
}

/********************************************************
* 名称: CResService::~CResService
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CResService::~CResService()
{
    //
}

/********************************************************
* 名称: CResService::Create
* 功能: 资源创建函数
* 输入:
* 输出:
* 返回: int 资源创建是否成功，0表示成功，非零值表示失败
********************************************************/
int CResService::Create()
{
    //检查服务是否存在
    if(!CtrlService(m_szServiceName, CMD_CHECK_SERVICE))
    {
        return ERR_FAIL;
    }

    //若服务存在则停止服务运行
    StopService(m_szServiceName);

    return CResObject::Create();
}

/********************************************************
* 名称: CResService::CheckStatus
* 功能: 资源的状态检查函数
* 输入:
* 输出:
* 返回: int 是否存在不可恢复的故障，0表示无故障，非零值表示检测到故障
********************************************************/
int CResService::CheckStatus()
{
    SERVICE_STATUS ServiceStatus;
    if(QueryServiceStatus(m_szServiceName, &ServiceStatus))
    {
        if(ServiceStatus.dwCurrentState != SERVICE_STOPPED)
        {
            return ERR_SUCCESS;
        }
    }

    m_nStatus = ERES_STATUS_DEACTIVE;
    return ERR_FAIL;
}

/********************************************************
* 名称: CResService::Active
* 功能: 资源的激活函数
* 输入:
* 输出:
* 返回: int 是否激活成功，0表示成功，非零值表示启动失败
********************************************************/
int CResService::Active()
{
    if (!StartService(m_szServiceName))
    {
        return ERR_FAIL;
    }

    return CResObject::Active();
}

/********************************************************
* 名称: CResService::DeActive
* 功能: 资源的去激活函数
* 输入:
* 输出:
* 返回: int 是否去激活成功，0表示成功，非零值表示去激活失败
********************************************************/
int CResService::DeActive()
{
    if (!StopService(m_szServiceName))
    {
        return ERR_FAIL;
    }

    return CResObject::DeActive();
}
