#ifndef __STAT_CFG_MGR_H__
#define __STAT_CFG_MGR_H__

#ifdef _PLATFORM_WIN32
#pragma warning(disable:4503)
#endif

#include "../include/toolbox.h"
#include "stat_cfg_encap.h"

class ICDRStatCfg;

class CCDRStatCfgMgr
{
public:
    CCDRStatCfgMgr();
    virtual ~CCDRStatCfgMgr();

    //进行初始化操作
    int Init(UINT4 nApCount);

    //返回所有ICDRStatCfg接口类指针的接口
    void GetStatCfgList(LIST<ICDRStatCfg*>& listStatCfg);

    //通过接入点号查找对应的ICDRStatCfg接口类指针
    ICDRStatCfg* GetStatCfgInterface(int nAccessPoint);

    //通过接入点名称查找对应的ICDRStatCfg接口类指针
    ICDRStatCfg* GetStatCfgInterface(STRING sApName);

protected:
    // 获得统计配置文件的读写封装对象
    ICDRStatCfg* CreateCDRStatCfg ( void );

    //ICDRStatCfg对象的指针列表
    LIST<ICDRStatCfg*>   m_statCfgList;
    BOOL                 m_bUseXml;

    CStatCfgEncap        m_StatCfgEncap;
};
#endif  //__STAT_CFG_MGR_H__
