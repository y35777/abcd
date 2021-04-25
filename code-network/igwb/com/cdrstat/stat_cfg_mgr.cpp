#include "../include/toolbox.h"
#include "../include/frame.h"
#include "../utility/config.h"
#include "stat_cfg_mgr.h"
#include "cdr_stat_cfg.h"

/*******************************************************************
函数名称：CCDRStatCfgMgr()
功能描述：构造函数
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
CCDRStatCfgMgr::CCDRStatCfgMgr()
{
    m_bUseXml = 0;
}

/*******************************************************************
函数名称：~CCDRStatCfgMgr()
功能描述：析构函数
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
CCDRStatCfgMgr::~CCDRStatCfgMgr()
{
    LIST<ICDRStatCfg*>::iterator it = m_statCfgList.begin();
    while (it != m_statCfgList.end())
    {
        delete *it;
        it++;
    }
    m_statCfgList.clear();
}

// 获得统计配置文件的读写封装对象
ICDRStatCfg* CCDRStatCfgMgr::CreateCDRStatCfg ( void )
{
    if(m_bUseXml)
    {
        return m_StatCfgEncap.CreateCDRStatCfg();
    }

    return new CCDRStatCfg();
}

/*******************************************************************
函数名称：Init()
功能描述：进行初始化操作
输入参数：UINT4 nApCount
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
int CCDRStatCfgMgr::Init(UINT4 nApCount)
{
    CINIFile ConfigFile(GetCfgFilePath());
    ConfigFile.Open();
    m_bUseXml = ConfigFile.GetInt(CFG_SEC_CDRSTAT,
                                  CFG_USE_XML,
                                  CFG_USE_XML_DEFAULT);

    ICDRStatCfg* pTmpCfg = NULL;
    for (UINT4 i = 1; i <= nApCount; i++)
    {
        pTmpCfg = CreateCDRStatCfg();
        pTmpCfg->Init(i);
        m_statCfgList.push_back(pTmpCfg);
    }

    return ERR_SUCCESS;
}


/*******************************************************************
函数名称：GetStatCfgList(LIST<ICDRStatCfg*>& listStatCfg)
功能描述：返回所有ICDRStatCfg接口类指针的接口
输入参数：无
输出参数：统计配置接口指针链表
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatCfgMgr::GetStatCfgList(LIST<ICDRStatCfg*>& listStatCfg)
{
    LIST<ICDRStatCfg*>::iterator it = m_statCfgList.begin();
    while (it != m_statCfgList.end())
    {
        listStatCfg.push_back(*it);
        it++;
    }
}


/*******************************************************************
函数名称：GetStatCfgInterface(int nAccessPoint)
功能描述：通过接入点号查找对应的ICDRStatCfg接口类指针
输入参数：int nAccessPoint,接入点号
输出参数：无
返回内容：此接入点的统计配置接口指针
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
ICDRStatCfg* CCDRStatCfgMgr::GetStatCfgInterface(int nAccessPoint)
{
    LIST<ICDRStatCfg*>::iterator it = m_statCfgList.begin();
    while (it != m_statCfgList.end())
    {
        if ((*it)->GetAccessPoint() == nAccessPoint)
        {
            return *it;
        }
        it++;
    }
    return NULL;
}


/*******************************************************************
函数名称：GetStatCfgInterface(STRING sApName)
功能描述：通过接入点号查找对应的ICDRStatCfg接口类指针
输入参数：STRING sApName,接入点名称
输出参数：无
返回内容：此接入点的统计配置接口指针
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
ICDRStatCfg* CCDRStatCfgMgr::GetStatCfgInterface(STRING sApName)
{
    LIST<ICDRStatCfg*>::iterator it = m_statCfgList.begin();
    while (it != m_statCfgList.end())
    {
        if (sApName.compare((*it)->GetAccessPointName()) == 0)
        {
            return *it;
        }
        it++;
    }
    return NULL;
}