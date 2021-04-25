#include "../include/base_type.h"
#include "res_user_defined.h"
#include "cluster_toolbox.h"


/********************************************************
* 名称: CResUserDefined::CResUserDefined
* 功能: 构造函数
* 输入: const char* const szCreateName  : 执行创建操作的程序
*       const char* const szActiveName  : 执行激活操作的程序
*       const char* const szDeActiveName: 执行去激活操作的程序
*       const char* const szCheckStatus : 执行状态检查操作的程序
*       const char* const szResName     : 资源名
*       ERES_TYPE nResType              : 资源类型
*       int nMaxLocalRecovery           : 最大本地恢复次数
* 输出:
* 返回:
********************************************************/
CResUserDefined::CResUserDefined(const char* const szCreateName,
                                 const char* const szActiveName,
                                 const char* const szDeActiveName,
                                 const char* const szCheckStatus,
                                 const char* const szResName,
                                 ERES_TYPE nResType,
                                 int nMaxLocalRecovery):
CResObject(szResName, nResType, nMaxLocalRecovery)
{
    strncpy(m_szCreateName, szCreateName, MAX_PATH);
    m_szCreateName[MAX_PATH - 1] = '\0';

    strncpy(m_szActiveName, szActiveName, MAX_PATH);
    m_szActiveName[MAX_PATH - 1] = '\0';

    strncpy(m_szDeActiveName, szDeActiveName, MAX_PATH);
    m_szCreateName[MAX_PATH - 1] = '\0';

    strncpy(m_szCheckName, szCheckStatus, MAX_PATH);
    m_szCheckName[MAX_PATH - 1] = '\0';
}

/********************************************************
* 名称: CResUserDefined::~CResUserDefined
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CResUserDefined::~CResUserDefined()
{
    //
}

/********************************************************
* 名称: CResUserDefined::Create
* 功能: 资源创建函数
* 输入:
* 输出:
* 返回: int 资源创建是否成功，0表示成功，非零值表示失败
********************************************************/
int CResUserDefined::Create()
{
    if(('\0' != m_szCreateName[0]) && 
       (ERR_SUCCESS != ExecuteProcess(m_szCreateName)))
    {
        return ERR_FAIL;
    }

    return CResObject::Create();
}

/********************************************************
* 名称: CResUserDefined::CheckStatus
* 功能: 资源的状态检查函数
* 输入:
* 输出:
* 返回: int 是否存在不可恢复的故障，0表示无故障，非零值表示检测到故障
********************************************************/
int CResUserDefined::CheckStatus()
{
    if(('\0' != m_szCheckName[0]) && 
       (ERR_SUCCESS != ExecuteProcess(m_szCheckName)))
    {
        m_nStatus = ERES_STATUS_DEACTIVE;
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CResUserDefined::Active
* 功能: 资源的激活函数
* 输入:
* 输出:
* 返回: int 是否激活成功，0表示成功，非零值表示启动失败
********************************************************/
int CResUserDefined::Active()
{
    if(('\0' != m_szActiveName[0]) && 
       (ERR_SUCCESS != ExecuteProcess(m_szActiveName)))
    {
        return ERR_FAIL;
    }

    return CResObject::Active();
}

/********************************************************
* 名称: CResUserDefined::DeActive
* 功能: 资源的去激活函数
* 输入:
* 输出:
* 返回: int 是否去激活成功，0表示成功，非零值表示去激活失败
********************************************************/
int CResUserDefined::DeActive()
{
    if(('\0' != m_szDeActiveName[0]) && 
       (ERR_SUCCESS != ExecuteProcess(m_szDeActiveName)))
    {
        return ERR_FAIL;
    }

    return CResObject::DeActive();
}
