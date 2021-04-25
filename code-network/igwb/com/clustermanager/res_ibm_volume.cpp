#include "../include/frame.h"
#include "res_ibm_volume.h"
#include "winioctl.h"
#include "cluster_toolbox.h"
#include "resource.h"
#include "ace/DLL.h"

#ifndef FIVE_SECOND
#define FIVE_SECOND (5*1000)
#endif

/********************************************************
* 名称: CResIBMVolume::CResIBMVolume
* 功能: 构造函数
* 输入: const char* const szDriveName : 驱动器盘符
*       const char* const szResName   : 资源名
*       ERES_TYPE nResType            : 资源类型
*       int nMaxLocalRecovery         : 最大本地恢复次数
* 输出:
* 返回:
********************************************************/
CResIBMVolume::CResIBMVolume(const char* const szDriveName,
                             const char* const szResName,
                             ERES_TYPE nResType,
                             int nMaxLocalRecovery):
CResVolume(szDriveName, szResName, nResType, nMaxLocalRecovery)
{
    //
}

/********************************************************
* 名称: CResIBMVolume::~CResIBMVolume
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CResIBMVolume::~CResIBMVolume()
{
    //
}

/********************************************************
* 名称: CResIBMVolume::Active
* 功能: 资源的激活函数
* 输入:
* 输出:
* 返回: int 是否激活成功，0表示成功，非零值表示启动失败
********************************************************/
int CResIBMVolume::Active()
{
    if(m_nStatus != ERES_STATUS_ACTIVE)
    {
        //接管IBM RAID资源
        TakeOver();

        if(g_nOSVersion >= 5)
        {
            RefreshIBMRaid f = GetRefreshIBMRaid();
            if(NULL != f)
            {
                f();
                Sleep(FIVE_SECOND);
            }
        }
    
        return CResVolume::Active();
    }

    return ERR_SUCCESS;
}

RefreshIBMRaid CResIBMVolume::GetRefreshIBMRaid()
{
    static ACE_Mutex mutex;
    static RefreshIBMRaid f = NULL;
    static ACE_DLL dll;

    mutex.acquire();

    if(NULL != f)
    {
        mutex.release();
        return f;
    }

    int nRet = dll.open("raidctl.dll");
    if(0 != nRet)
    {
        mutex.release();
        return NULL;
    }

    f = (RefreshIBMRaid)dll.symbol("RefreshIBMRaid");

    mutex.release();

    return f;
}
/********************************************************
* 名称: CResIBMVolume::TakeOver
* 功能: 接管RAID硬盘资源
* 输入:
* 输出:
* 返回: BOOL 是否接管成功
********************************************************/
BOOL CResIBMVolume::TakeOver()
{
    char szWorkDirectory[MAX_PATH];
    char szExeName[MAX_PATH];

    GetSystemDirectory(szWorkDirectory, MAX_PATH);
    sprintf(szExeName, "%s\\ipshahto.exe ", szWorkDirectory);

    int nRet = ExecuteProcess(szExeName);
    if(ERR_SUCCESS == nRet)
    {
        TRACE(MTS_CLSTR, S_EXECUTE_IPSHAHTO_SUCCESS);
    }
    else
    {
        TRACE(MTS_CLSTR, S_EXECUTE_IPSHAHTO_FAIL);
    }

    return TRUE;
}

