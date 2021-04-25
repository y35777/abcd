#include "../include/base_type.h"
#include "res_object.h"


/********************************************************
* 名称: CResObject::CResObject
* 功能: 构造函数
* 输入: const char* const szResName : 资源名
*       ERES_TYPE nResType          : 资源类型
*       int nMaxLocalRecovery       : 最大本地恢复次数
* 输出:
* 返回:
********************************************************/
CResObject::CResObject(const char* const szResName,
                       ERES_TYPE nResType,
                       int nMaxLocalRecovery)
{
    strncpy(m_szResName, szResName, MAX_RES_NAME);
    m_szResName[MAX_RES_NAME - 1] = '\0';

    m_nResType                = nResType;
    m_nMaxLocalRecovery       = nMaxLocalRecovery;
    m_nLocalRecovery          = 0;
    m_tLastActiveTime         = time(NULL);
    m_nStatus                 = ERES_STATUS_INIT;
    m_uAlarmID                = 0;
    m_uAlarmPara              = 0;
    m_uResIndex               = 0;
    m_nSwitchGroup            = 0;
}

/********************************************************
* 名称: CResObject::~CResObject
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CResObject::~CResObject()
{
    //
}

/********************************************************
* 名称: CResObject::GetAlarmID
* 功能: 获取资源告警ID
* 输入:
* 输出:
* 返回: UINT4 资源告警ID
********************************************************/
UINT4 CResObject::GetAlarmID()
{
    return m_uAlarmID;
}

/********************************************************
* 名称: CResObject::GetAlarmPara
* 功能: 获取资源告警参数
* 输入:
* 输出:
* 返回: UINT4 资源告警参数
********************************************************/
UINT4 CResObject::GetAlarmPara()
{
    return m_uAlarmPara;
}

/********************************************************
* 名称: CResObject::GetResIndex
* 功能: 获取资源索引号
* 输入:
* 输出:
* 返回: UINT4 资源索引号
********************************************************/
UINT4 CResObject::GetResIndex()
{
    return m_uResIndex;
}

/********************************************************
* 名称: CResObject::GetResType
* 功能: 获取资源类型
* 输入:
* 输出:
* 返回: ERES_TYPE 资源类型
********************************************************/
ERES_TYPE CResObject::GetResType()
{
    return m_nResType;
}

/********************************************************
* 名称: CResObject::GetStatus
* 功能: 获取资源状态
* 输入:
* 输出:
* 返回: ERES_STATUS 资源状态
********************************************************/
ERES_STATUS CResObject::GetStatus()
{
    return m_nStatus;
}

/********************************************************
* 名称: CResObject::GetSwitchGroup
* 功能: 获取资源的倒换组号
* 输入:
* 输出:
* 返回: ERES_TYPE 资源类型
********************************************************/
int CResObject::GetSwitchGroup()
{
    return m_nSwitchGroup;
}

/********************************************************
* 名称: CResObject::GetResName
* 功能: 获取资源名称
* 输入:
* 输出:
* 返回: const char* 资源名称
********************************************************/
const char* CResObject::GetResName()
{
    return m_szResName;
}

/********************************************************
* 名称: CResObject::Create
* 功能: 资源创建函数
* 输入:
* 输出:
* 返回: int 资源创建是否成功，0表示成功，非零值表示失败
********************************************************/
int CResObject::Create()
{
    m_nStatus = ERES_STATUS_DEACTIVE;

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CResObject::Remove
* 功能: 释放资源前的清理函数
* 输入:
* 输出:
* 返回: void
********************************************************/
void CResObject::Remove()
{
    //如果状态为激活，则调用DeActive函数
    if(ERES_STATUS_ACTIVE == m_nStatus)
    {
        DeActive();
    }
}

/********************************************************
* 名称: CResObject::CheckStatus
* 功能: 资源的状态检查函数
* 输入:
* 输出:
* 返回: int 是否存在不可恢复的故障，0表示无故障，非零值表示检测到故障
********************************************************/
int CResObject::CheckStatus()
{
    return ERR_SUCCESS;
}


/********************************************************
* 名称: CResObject::LocalRecovery
* 功能: 执行本地恢复操作
* 输入: 
* 输出: 
* 返回: int 本地恢复是否成功 
********************************************************/
int CResObject::LocalRecovery()
{
    for(int i = 0; i < m_nMaxLocalRecovery; i++)
    {
        if(Active() == ERR_SUCCESS)
        {
            return ERR_SUCCESS;
        }

        if(i < m_nMaxLocalRecovery - 1)
        {
            ACE_OS::sleep(5);
        }
    }

    return ERR_FAIL;
}

/********************************************************
* 名称: CResObject::Active
* 功能: 资源的激活函数
* 输入:
* 输出:
* 返回: int 是否激活成功，0表示成功，非零值表示启动失败
********************************************************/
int CResObject::Active()
{
    m_tLastActiveTime = time(NULL);
    m_nStatus         = ERES_STATUS_ACTIVE;

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CResObject::DeActive
* 功能: 资源的去激活函数
* 输入:
* 输出:
* 返回: int 是否去激活成功，0表示成功，非零值表示去激活失败
********************************************************/
int CResObject::DeActive()
{
    m_nStatus = ERES_STATUS_DEACTIVE;

    return ERR_SUCCESS;
}
