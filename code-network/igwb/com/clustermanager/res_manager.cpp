#include "../include/frame.h"
#include "res_manager.h"
#include "res_object_creator.h"
#include "RaidHDChkCreator.h"
#include "resource.h"

/********************************************************
* 名称: CResManager::CResManager
* 功能: 构造函数
* 输入:
* 输出:
* 返回:
********************************************************/
CResManager::CResManager()
{
    m_pDynamResMap   = NULL;
    m_pStaticResMap  = NULL;
    m_pRaidHDChecker = NULL;
}

/********************************************************
* 名称: CResManager::~CResManager
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CResManager::~CResManager()
{
    ExitInstance();
}

/********************************************************
* 名称: CResManager::InitInstance
* 功能: 初始化函数
* 输入:
* 输出:
* 返回: int 初始化是否成功，0表示成功，非零值表示失败
********************************************************/
int CResManager::InitInstance()
{
    //用CResObjectCreator类创建动态同步资源的函数，返回动态同步资源Map的指针
    m_pDynamResMap = CResObjectCreator::CreateResObject(RT_DYNAMIC);
    if((NULL == m_pDynamResMap) && (ERR_CREATE_RES_OBJECT == errno))
    {
        return ERR_CREATE_RES_OBJECT;
    }

    //用CResObjectCreator类创建静态同步资源的函数，返回静态同步资源Map的指针
    m_pStaticResMap = CResObjectCreator::CreateResObject(RT_STATIC);
    if(NULL != m_pStaticResMap)
    {
        Active(RT_STATIC);
    }
    else if(ERR_CREATE_RES_OBJECT == errno)
    {
        return ERR_CREATE_RES_OBJECT;
    }

    //调用CRaidHDChkCreator::CreatRaidHDCheck函数，
    //创建CRaidHDCheck对象，初始化成员变量m_pRaidHDChecker
    m_pRaidHDChecker = CRaidHDChkCreator::CreatRaidHDCheck();

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CResManager::ExitInstance
* 功能: 退出前的进行清理工作
* 输入:
* 输出:
* 返回: void
********************************************************/
void CResManager::ExitInstance()
{
    //停止并释放动态同步资源Map的资源对象
    DeActive(RT_DYNAMIC);
    if(NULL != m_pDynamResMap)
    {
        MAP<int, CResObject*>::iterator i;
        for (i = m_pDynamResMap->begin(); i != m_pDynamResMap->end(); i++)
        {
            (*i).second->Remove();
            delete (*i).second;
        }
        m_pDynamResMap->clear();
        delete m_pDynamResMap;
        m_pDynamResMap = NULL; 
    }

    //停止并释放静态同步资源Map的资源对象
    DeActive(RT_STATIC);
    if(NULL != m_pStaticResMap)
    {
        MAP<int, CResObject*>::iterator i;
        for (i = m_pStaticResMap->begin(); i != m_pStaticResMap->end(); i++)
        {
            (*i).second->Remove();
            delete (*i).second;
        }
        m_pStaticResMap->clear();
        delete m_pStaticResMap;
        m_pStaticResMap = NULL; 
    }

    //释放硬盘检测对象
    if(NULL != m_pRaidHDChecker)
    {
        delete m_pRaidHDChecker;
        m_pRaidHDChecker = NULL;
    }
}

/********************************************************
* 名称: CResManager::CheckStatus
* 功能: 资源的状态检查函数
* 输入: nCheckResMask : nCheckResMask---被检查资源类型的掩码，
*       1表示动态资源,2表示静态资源,3表示硬盘资源
* 输出:
* 返回: UINT4 是否存在不可恢复的故障，0表示无故障，非零值
*       表示发生故障的资源ID,具体分配如下：
*       当资源发生故障时，返回资源ID的掩码
*       当磁盘阵列发生故障时，返回故障磁盘ID的掩码
********************************************************/
UINT4 CResManager::CheckStatus(int nCheckResMask)
{
    CResObject* pResObject = NULL;

    //如果nCheckResMask等于RT_DYNAMIC，
    //则调用m_pDynamResMap中所有资源对象的CheckStatus函数，
    //检查资源状态，如果存在不可恢复的故障，则返回该资源
    //在m_pDynamResMap中的索引
    if ((RT_DYNAMIC == nCheckResMask) && (NULL != m_pDynamResMap))
    {
        MAP<int, CResObject*>::iterator i;
        int nFailedResIDMask = 0;
        for (i = m_pDynamResMap->begin(); i != m_pDynamResMap->end(); i++)
        {
            pResObject = (*i).second;
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, 
                   S_BEGIN_CHECK_RESOURCE, pResObject->GetResName());
            if(pResObject->CheckStatus() != ERR_SUCCESS)
            {
                MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, 
                       S_RES_LOCAL_RECOVERY, 
                       pResObject->GetResName());
                if(pResObject->LocalRecovery() != ERR_SUCCESS)
                {
                    nFailedResIDMask |= 1 << ((*i).first - 1);
                }
                else
                {
                    MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, 
                           S_RES_LOCAL_RECOVERY_SUCCESS, 
                           pResObject->GetResName());
                }
            }
            else
            {
                MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, 
                       S_CHECK_RESOURCE_OK, pResObject->GetResName());
            }
        }

        return nFailedResIDMask;
    }

    //检测磁盘阵列硬盘
    if(RT_HARDDISK == nCheckResMask)
    {
        int nHardDisk = CheckRaidHD();
        if(0 != nHardDisk)
        {
            return nHardDisk;
        }
    }

    //如果nCheckResMask等于RT_STATIC，
    //则调用m_pStaticResMap中所有资源对象的CheckStatus函数，
    //检查资源状态，如果存在不可恢复的故障，则返回该资源在
    //m_pStaticResMap中的索引
    if ((RT_STATIC == nCheckResMask) && (NULL != m_pStaticResMap))
    {
        MAP<int, CResObject*>::iterator i;
        int nFailedResIDMask = 0;
        for (i = m_pStaticResMap->begin(); i != m_pStaticResMap->end(); i++)
        {
            pResObject = (*i).second;
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, 
                   S_BEGIN_CHECK_RESOURCE, pResObject->GetResName());
            if(pResObject->CheckStatus() != ERR_SUCCESS)
            {
                MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, 
                       S_RES_LOCAL_RECOVERY, 
                       pResObject->GetResName());
                if(pResObject->LocalRecovery() != ERR_SUCCESS)
                {
                    nFailedResIDMask |= 1 << ((*i).first - 1);
                }
                else
                {
                    MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, 
                           S_RES_LOCAL_RECOVERY_SUCCESS, 
                           pResObject->GetResName());
                }
            }
            else
            {
                MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, 
                       S_CHECK_RESOURCE_OK, pResObject->GetResName());
            }
        }

        return nFailedResIDMask;
    }

    return 0;
}

/********************************************************
* 名称: CResManager::Active
* 功能: 资源的激活函数
* 输入: nCheckResMask : nCheckResMask---被激活资源类型的掩码，
*       第一位（即最低位）表示动态资源，第二位表示静态资源
* 输出:
* 返回: int 是否激活成功，0表示成功，非零值表示启动失败的
*       资源ID，具体分配如下：
*       所返回的int的最高位为0；剩余位为资源ID
********************************************************/
int CResManager::Active(int nCheckResMask)
{
    CResObject* pResObject = NULL;

    //如果nCheckResMask位与RT_DYNAMIC不为零，则调用
    //m_pDynamResMap中所有资源对象的Active函数，激活这些资源，
    //如果激活失败，则返回该资源在m_pDynamResMap中的索引
    int nFailedResIDMask = 0;
    if ((nCheckResMask & RT_DYNAMIC) && (NULL != m_pDynamResMap))
    {
        MAP<int, CResObject*>::iterator i;
        for (i = m_pDynamResMap->begin(); i != m_pDynamResMap->end(); i++)
        {
            pResObject = (*i).second;
            if(pResObject->Active() != ERR_SUCCESS)
            {
                nFailedResIDMask |= 1 << ((*i).first - 1);
                int nSwitchGroup = pResObject->GetSwitchGroup();
                if(nSwitchGroup == 0)
                //如果倒换组号为0，则返回当前失效资源的ID掩码
                {
                    return nFailedResIDMask;
                }
                else if(nSwitchGroup > 0)
                //如果倒换组号大于0，则要检查其它同组号的资源是否也失效
                {
                    //寻找后绪资源中是否存在同组号的资源
                    MAP<int, CResObject*>::iterator j = i;
                    BOOL bExistSameGroupRes = FALSE;
                    while(++j != m_pDynamResMap->end())
                    {
                        if((*j).second->GetSwitchGroup() == nSwitchGroup)
                        {
                            bExistSameGroupRes = TRUE;
                            break;
                        }
                    }

                    //当后绪资源中存在同组号的资源，则继续激活下一个资源
                    //如果不存在，则检查前面的同组资源中是否存在已激活的资源
                    if(!bExistSameGroupRes && VerifyGroupFailed((*i).first))
                    {
                        return nFailedResIDMask;
                    }
                }
            }
        }
    }

    //如果nCheckResMask位与RT_STATIC不为零，则调用
    //m_pStaticResMap中所有资源对象的Active函数，激活这些资源，
    //如果激活失败，则返回该资源在m_pStaticResMap中的索引
    if ((nCheckResMask & RT_STATIC) && (NULL != m_pStaticResMap))
    {
        MAP<int, CResObject*>::iterator i;
        for (i = m_pStaticResMap->begin(); i != m_pStaticResMap->end(); i++)
        {
            pResObject = (*i).second;
            if(pResObject->Active() != ERR_SUCCESS)
            {
                nFailedResIDMask |= 1 << ((*i).first - 1);
                int nSwitchGroup = pResObject->GetSwitchGroup();
                if(nSwitchGroup == 0)
                //如果倒换组号为0，则返回当前失效资源的ID掩码
                {
                    return nFailedResIDMask;
                }
                else if(nSwitchGroup > 0)
                //如果倒换组号大于0，则要检查其它同组号的资源是否也失效
                {
                    //寻找后绪资源中是否存在同组号的资源
                    MAP<int, CResObject*>::iterator j = i;
                    BOOL bExistSameGroupRes = FALSE;
                    while(++j != m_pStaticResMap->end())
                    {
                        if((*j).second->GetSwitchGroup() == nSwitchGroup)
                        {
                            bExistSameGroupRes = TRUE;
                            break;
                        }
                    }

                    //当后绪资源中存在同组号的资源，则继续激活下一个资源
                    //如果不存在，则检查前面的同组资源中是否存在已激活的资源
                    if(!bExistSameGroupRes && VerifyGroupFailed((*i).first))
                    {
                        return nFailedResIDMask;
                    }
                }
            }
        }
    }

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CResManager::DeActive
* 功能: 资源的去激活函数
* 输入: nCheckResMask : nCheckResMask---被激活资源类型的掩码，
*       第一位（即最低位）表示动态资源，第二位表示静态资源
* 输出:
* 返回: int 是否去激活成功，0表示成功，非零值表示启动失败的
*       资源ID，具体分配如下：
*       所返回的UINT4的最高位为0；剩余位为资源ID
********************************************************/
int CResManager::DeActive(int nCheckResMask)
{
    CResObject* pResObject = NULL;
    int nFailedResObject = 0;

    //如果nCheckResMask位与RT_DYNAMIC不为零，则调用
    //m_pDynamResMap中所有资源对象的DeActive函数，去激活这些资源，
    //如果激活失败，则返回该资源在m_pDynamResMap中的索引
    if ((nCheckResMask & RT_DYNAMIC) && (NULL != m_pDynamResMap))
    {
        MAP<int, CResObject*>::iterator i;
        for (i = m_pDynamResMap->begin(); i != m_pDynamResMap->end(); i++)
        {
            pResObject = (*i).second;
            if((pResObject->DeActive() != ERR_SUCCESS) && (0 == nFailedResObject))
            {
                nFailedResObject = (*i).first;
            }
        }
    }

    //如果nCheckResMask位与RT_STATIC不为零，则调用
    //m_pStaticResMap中所有资源对象的DeActive函数，去激活这些资源，
    //如果激活失败，则返回该资源在m_pStaticResMap中的索引
    if ((nCheckResMask & RT_STATIC) && (NULL != m_pStaticResMap))
    {
        MAP<int, CResObject*>::iterator i;
        for (i = m_pStaticResMap->begin(); i != m_pStaticResMap->end(); i++)
        {
            pResObject = (*i).second;
            if((pResObject->DeActive() != ERR_SUCCESS) && (0 == nFailedResObject))
            {
                nFailedResObject = (*i).first;
            }
        }
    }

    return nFailedResObject;
}

/********************************************************
* 名称: CResManager::CheckRaidHD
* 功能: 磁盘阵列硬盘的故障检查函数
* 输入:
* 输出:
* 返回: int 发生故障的硬盘ID掩码
********************************************************/
int CResManager::CheckRaidHD()
{
    //调用m_pRaidHDChecker对象的检测接口，实施对磁盘阵列
    //硬盘的检测，返回发生故障的硬盘掩码
    if(NULL != m_pRaidHDChecker)
    {
        m_pRaidHDChecker->CheckRaidHD();

        return m_pRaidHDChecker->GetFailedHDID();
    }

    return 0;
}

/********************************************************
* 名称: CResManager::GetObject
* 功能: 返回指定CResObject资源对象的指针
* 输入: nIndex : 资源的索引
* 输出:
* 返回: CResObject* 对应的CResObject资源对象指针
********************************************************/
CResObject* CResManager::GetObject(int nIndex)
{
    if((NULL != m_pDynamResMap) &&
       (m_pDynamResMap->find(nIndex) != m_pDynamResMap->end()))
    {
        return (*m_pDynamResMap)[nIndex];
    }

    if((NULL != m_pStaticResMap) &&
       (m_pStaticResMap->find(nIndex) != m_pStaticResMap->end()))
    {
        return (*m_pStaticResMap)[nIndex];
    }

    return NULL;
}

/********************************************************
* 名称: CResManager::VerifyGroupFailed
* 功能: 检查指定的资源ID所在的资源组是否都已失效
* 输入: nIndex : 资源的索引
* 输出:
* 返回: CResObject* 对应的CResObject资源对象指针
********************************************************/
BOOL CResManager::VerifyGroupFailed(int nIndex)
{
    if((NULL != m_pDynamResMap) &&
       (m_pDynamResMap->find(nIndex) != m_pDynamResMap->end()))
    {
        int nSwitchGroup = (*m_pDynamResMap)[nIndex]->GetSwitchGroup();
        if(nSwitchGroup < 0)
        {
            return FALSE;
        }
        else if(nSwitchGroup == 0)
        {
            return TRUE;
        }

        MAP<int, CResObject*>::iterator i;
        for (i = m_pDynamResMap->begin(); i != m_pDynamResMap->end(); i++)
        {
            if(((*i).second->GetStatus() == ERES_STATUS_ACTIVE) &&
               ((*i).second->GetSwitchGroup() == nSwitchGroup))
            {
                return FALSE;
            }
        }

        return TRUE;
    }

    if((NULL != m_pStaticResMap) &&
       (m_pStaticResMap->find(nIndex) != m_pStaticResMap->end()))
    {
        int nSwitchGroup = (*m_pStaticResMap)[nIndex]->GetSwitchGroup();
        if(nSwitchGroup < 0)
        {
            return FALSE;
        }
        else if(nSwitchGroup == 0)
        {
            return TRUE;
        }

        MAP<int, CResObject*>::iterator i;
        for (i = m_pStaticResMap->begin(); i != m_pStaticResMap->end(); i++)
        {
            if(((*i).second->GetStatus() == ERES_STATUS_ACTIVE) &&
               ((*i).second->GetSwitchGroup() == nSwitchGroup))
            {
                return FALSE;
            }
        }

        return TRUE;
    }

    return FALSE;
}
