#ifndef __STAT_RST_MGR_H__
#define __STAT_RST_MGR_H__

#include "../include/toolbox.h"
#include "stat_rst_encap.h"

#ifdef _PLATFORM_WIN32
#pragma warning(disable:4503)
#endif

class ICDRStatRst;

class CCDRStatRstMgr
{
public:
    CCDRStatRstMgr();
    virtual ~CCDRStatRstMgr();

    //进行初始化操作
    int Init(UINT4 nApCount);

    //返回所有ICDRStatRst接口类指针的接口
    void GetStatRstList(LIST<ICDRStatRst*>& listStatRst);

    //通过接入点号查找对应的ICDRStatRst接口类指针
    ICDRStatRst* GetStatRstInterface(int nAccessPoint);

    //通过接入点名称查找对应的ICDRStatRst接口类指针
    ICDRStatRst* GetStatRstInterface(STRING sAPName);

protected:
    // 获得统计结果文件的读写封装对象
    ICDRStatRst* CreateCDRStatRst ( void );

    //ICDRStatRst对象的指针列表
    LIST<ICDRStatRst*>   m_statRstList;
    BOOL                 m_bUseXml;

    CStatRstEncap        m_StatRstEncap;
};

#endif //__STAT_RST_MGR_H__
