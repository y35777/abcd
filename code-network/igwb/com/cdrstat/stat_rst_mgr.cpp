#include "../include/toolbox.h"
#include "../include/frame.h"
#include "../utility/config.h"
#include "stat_rst_mgr.h"
#include "cdr_stat_rst.h"

/*******************************************************************
函数名称：CCDRStatRstMgr()
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
CCDRStatRstMgr::CCDRStatRstMgr()
{
    m_bUseXml = 0;
}

/*******************************************************************
函数名称：~CCDRStatRstMgr()
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
CCDRStatRstMgr::~CCDRStatRstMgr()
{
    LIST<ICDRStatRst*>::iterator it = m_statRstList.begin();
    while (it != m_statRstList.end())
    {
        delete *it;
        it++;
    }
    m_statRstList.clear();
}

//获得统计结果文件的读写封装对象
ICDRStatRst* CCDRStatRstMgr::CreateCDRStatRst ( void )
{
    if(m_bUseXml)
    {
        return m_StatRstEncap.CreateCDRStatRst();
    }

    return new CCDRStatRst();
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
int CCDRStatRstMgr::Init(UINT4 nApCount)
{
    CINIFile ConfigFile(GetCfgFilePath());
    ConfigFile.Open();
    m_bUseXml = ConfigFile.GetInt(CFG_SEC_CDRSTAT,
                                  CFG_USE_XML,
                                  CFG_USE_XML_DEFAULT);

    ICDRStatRst* pTmpRst = NULL;
    for (UINT4 i = 1; i <= nApCount; i++)
    {
        pTmpRst = CreateCDRStatRst();
        pTmpRst->Init(i);
        m_statRstList.push_back(pTmpRst);
    }

    return ERR_SUCCESS;
}

/*******************************************************************
函数名称：GetStatRstList(LIST<ICDRStatRst*>& listStatRst)
功能描述：返回所有ICDRStatRst接口类指针的接口
输入参数：无
输出参数：ICDRStatRst*>& listStatRst，统计结果接口指针列表
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatRstMgr::GetStatRstList(LIST<ICDRStatRst*>& listStatRst)
{
    LIST<ICDRStatRst*>::iterator it = m_statRstList.begin();
    while (it != m_statRstList.end())
    {
        listStatRst.push_back(*it);
        it++;
    }

}

/*******************************************************************
函数名称：GetStatRstInterface(int nAccessPoint)
功能描述：通过接入点号查找对应的ICDRStatRst接口类指针
输入参数：int nAccessPoint，接入点号
输出参数：无
返回内容：此接点的统计结果接口指针
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
ICDRStatRst* CCDRStatRstMgr::GetStatRstInterface(int nAccessPoint)
{
    LIST<ICDRStatRst*>::iterator it = m_statRstList.begin();
    while (it != m_statRstList.end())
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
函数名称：GetStatRstInterface(STRING sAPName)
功能描述：通过接入点号查找对应的ICDRStatRst接口类指针
输入参数：STRING sAPName
输出参数：无
返回内容：此接点的统计结果接口指针
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
ICDRStatRst* CCDRStatRstMgr::GetStatRstInterface(STRING sAPName)
{
    LIST<ICDRStatRst*>::iterator it = m_statRstList.begin();
    while (it != m_statRstList.end())
    {
        if (sAPName.compare((*it)->GetAccessPointName()) == 0)
        {
            return *it;
        }
        it++;
    }
    return NULL;
}
