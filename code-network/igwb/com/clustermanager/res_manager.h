#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include "res_object.h"
#include "RaidHDCheck.h"

enum RESOURCE_TYPE
{
    RT_DYNAMIC      = 1,    //动态资源
    RT_STATIC       = 2,    //静态资源
    RT_HARDDISK     = 4     //磁盘
};

class CResManager
{
public:
    CResManager();
    virtual ~CResManager();

    //初始化函数
    int InitInstance();
    //资源的状态检查函数
    UINT4 CheckStatus(int nCheckResMask);
    //资源的激活函数
    int Active(int nCheckResMask);
    //资源的去激活函数
    int DeActive(int nCheckResMask);
    //磁盘阵列硬盘的故障检查函数
    int CheckRaidHD();
    //返回指定CResObject资源对象的指针
    CResObject* GetObject(int nIndex);
    //检查指定的资源ID所在的资源组是否都已失效
    BOOL VerifyGroupFailed(int nIndex);

protected:

    //退出前的进行清理工作
    void ExitInstance();

protected:

    //动态同步资源的Map指针
    MAP<int, CResObject*>      *m_pDynamResMap;

    //静态同步资源的Map指针
    MAP<int, CResObject*>      *m_pStaticResMap;

    //磁盘阵列硬盘状态对象的指针
    CRaidHDCheck*               m_pRaidHDChecker;

};

#endif //__RESOURCE_MANAGER_H__
