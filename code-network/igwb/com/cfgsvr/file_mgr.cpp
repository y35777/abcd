/******************************************************************************
Copyright (C), 2003-2011, Huawei Tech. Co., Ltd.

  /****************************************************************************
  File Name       : file_mgr.cpp
  Version         : 1.0
  Author          : dengwanqiu
  Created         : 2003-07-31
  Last Modified   : 
  Description     : implementation of the CCfgFileMgr class.
  Function List   : 
  History         : 
  1 Date          : 2003-07-31
  Author        : dengwanqiu
  Modification  : Created file
******************************************************************************/
#include "file_mgr.h"
#include "file_grp.h"

//记录设置配置参数失败原因，当失败时，将此信息反馈给客户端; 定义在cfg_svr.cpp中
extern char g_szErrorReason[20*1024];

//////////////////////////////////////////////////////////////////////
// CCfgFileMgr Class
//////////////////////////////////////////////////////////////////////

/*******************************************************************
函数名称：CCfgFileMgr()
功能描述：构造函数
输入参数：
输出参数：
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
CCfgFileMgr::CCfgFileMgr()
{   
}

/*******************************************************************
函数名称：~CCfgFileMgr()
功能描述：析构函数
输入参数：
输出参数：
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
CCfgFileMgr::~CCfgFileMgr()
{
    //清除映射表m_mapCfgFileGrp；
    MAP< int, CCfgFileGrp* >::iterator it;
    for(it=m_mapCfgFileGrp.begin(); it != m_mapCfgFileGrp.end(); it++)
    {
        delete (*it).second;
    }
    m_mapCfgFileGrp.clear();
    
    //清除映射表m_mapCfgFileClass
    m_mapCfgFileClass.clear();
}

/*******************************************************************
函数名称：AddCfgFileGrp(int nGrpNo, char* szFilePath)
功能描述：增加配置文件组
输入参数：
   参数1：int nGrpNo，组号
   参数2：int nFileNo，文件号
输出参数：
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CCfgFileMgr::AddCfgFileGrp(int nGrpNo, const char *szFilePath)
{
    if (szFilePath == NULL)
    {
        return FALSE;
    }

	//创建一个配置文件组pCfgFileGrp，执行初始化，pCfgFileGrp->Init()；
    CCfgFileGrp *pCfgFileGrp = new  CCfgFileGrp();
    if (!pCfgFileGrp->Init(szFilePath))
    {
        delete pCfgFileGrp;
        return FALSE;
    }
    //在m_mapCfgFileGrp建立nGrpNo和pCfgFileGrp映射关系；
    PAIR< MAP< int, CCfgFileGrp * >::iterator, bool > result;
    result = m_mapCfgFileGrp.insert( PAIR< int, CCfgFileGrp * >(nGrpNo, 
        pCfgFileGrp) );
    if (!result.second)
    {
        //文件组号重复
        delete pCfgFileGrp;
        return FALSE;
    }
    //在m_mapCfgFileClass建立nGrpNo和szFilePath的关系；
    m_mapCfgFileClass.insert(PAIR< int, STRING >(nGrpNo, STRING(szFilePath)));
    
    return TRUE;
}

/*******************************************************************
函数名称：GetAllSectName(LIST<STRING>& SectNameList, int nGrpNo, int nFileNo)
功能描述：根据组号和文件号获得段列表
输入参数：
   参数1：int nGrpNo，组号
   参数2：int nFileNo，文件号
输出参数：
   参数1：LIST<STRING>& SectNameList，段名列表
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CCfgFileMgr::GetAllSectName(LIST< STRING >& SectNameList, 
                                 int nGrpNo, int nFileNo)
{
    //根据组号在m_mapCfgFileGrp映射表中找到对应的配置文件组pCfgFileGrp；
    //调用pCfgFileGrp->GetAllSectName()
    MAP< int, CCfgFileGrp* >::iterator it = m_mapCfgFileGrp.find(nGrpNo);
    if ( it != m_mapCfgFileGrp.end() )
    {
        CCfgFileGrp *pCfgFileGrp = (*it).second;
        if (pCfgFileGrp != NULL)
        {
            pCfgFileGrp->GetAllSectName(SectNameList,nFileNo);
        }
    }
}

/*******************************************************************
函数名称：GetCfgListFromSectName(const char* szSectName, 
                                 int nGrpNo, int nFileNo)
功能描述：根据组号、文件号和段名获得某配置文件某段的所有配置项
输入参数：
   参数1：const char* szSectName，段名
   参数2：int nGrpNo，组号
   参数3：int nFileNo，文件号
输出参数：
返回内容：CFGITEM_LIST* ,返回指定段名的参数项信息列表指针
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
CFGITEM_LIST* CCfgFileMgr::GetCfgListFromSectName(const char *szSectName, 
                                                  int nGrpNo, int nFileNo)
{
	if (szSectName == NULL)
    {
        return FALSE;
    }
    
    //根据组号在m_mapCfgFileGrp映射表中找到对应的配置文件组pCfgFileGrp；
    //调用pCfgFileGrp->GetCfgListFromSectName()；
    MAP< int, CCfgFileGrp* >::iterator it = m_mapCfgFileGrp.find(nGrpNo);
    if ( it != m_mapCfgFileGrp.end() )
    {
        CCfgFileGrp *pCfgFileGrp = (*it).second;
        if (pCfgFileGrp != NULL)
        {
            return pCfgFileGrp->GetCfgListFromSectName(szSectName,nFileNo);
        }
    }
	return NULL;
}

/*******************************************************************
函数名称：SetAllCfgItem(const char* szAllCfgItem, int nGrpNo, int nFileNo)
功能描述：根据组号和文件号设置参数配置信息
输入参数：
   参数1：int nGrpNo，组号
   参数2：int nFileNo，文件号
输出参数：
   参数1：char* szAllCfgItem，参数配置信息
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CCfgFileMgr::SetAllCfgItem(const char *szAllCfgItem, int nGrpNo,
                                int nFileNo)
{
	if (szAllCfgItem == NULL)
    {
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s",
                 S_CFG_SET_ITEMINFO_INVALID);
        return FALSE;
    }

    //根据组号在m_mapCfgFileGrp映射表中找到对应的配置文件组pCfgFileGrp；
    //调用pCfgFileGrp->SetAllCfgItem()；
    MAP< int, CCfgFileGrp* >::iterator it = m_mapCfgFileGrp.find(nGrpNo) ;
    if ( it != m_mapCfgFileGrp.end() )
    {
        CCfgFileGrp *pCfgFileGrp = (*it).second;
        if (pCfgFileGrp != NULL)
        {
            return pCfgFileGrp->SetAllCfgItem(szAllCfgItem, nFileNo);
        }
    }

    SNPRINTF(g_szErrorReason,
             sizeof(g_szErrorReason),
             "%s",
             S_CFG_SET_ITEMINFO_INVALID);
    return FALSE;
}


/*******************************************************************
函数名称：GetCfgFileClass(LIST< STRING >& GrpNameList)
功能描述：获取所有文件组名
输入参数：
输出参数：
   参数1：LIST< STRING >& GrpNameList; 文件组名列表
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/

void CCfgFileMgr::GetCfgFileClass(LIST< STRING >& GrpNameList)
{
    //从m_mapCfgFileClass映射表中按照组号顺序取出对应的组名（组路径）；
    //把取出来的组名加入到GrpNameList中
    MAP< int, STRING >::iterator it;
    
    GrpNameList.clear();
    for(it=m_mapCfgFileClass.begin(); it != m_mapCfgFileClass.end(); it++)
    {
        GrpNameList.push_back( (*it).second );
    }
}


/*******************************************************************
函数名称：GetAllCfgItem(char* szAllCfgItem, int nGrpNo, int nFileNo)
功能描述：根据组号和文件号获得参数配置信息
输入参数：
   参数1：int nGrpNo，组号
   参数2：int nFileNo，文件号
输出参数：
   参数1：STRING &strAllCfgItem，参数配置信息
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CCfgFileMgr::GetAllCfgItem(STRING &strAllCfgItem , int nGrpNo, 
                                int nFileNo)
{
    //根据组号在m_mapCfgFileGrp映射表中找到对应的配置文件组pCfgFileGrp；
    MAP< int, CCfgFileGrp* >::iterator itFind = m_mapCfgFileGrp.find(nGrpNo);
    if (itFind != m_mapCfgFileGrp.end())
    {
        //调用pCfgFileGrp->GetAllCfgItem()；
        CCfgFileGrp *pCfgFileGrp = (*itFind).second;
        
        if (pCfgFileGrp != NULL)
        {
            pCfgFileGrp->GetAllCfgItem(strAllCfgItem, nFileNo);
        }
    }
}

/*******************************************************************
函数名称：UpdateCurrentCfg(int nGrpNo, char* szFilePath)
功能描述：更新某组的当前配置文件对象
输入参数：
   参数1：int nGrpNo，组号
   参数2：int nFileNo，文件号
输出参数：
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CCfgFileMgr::UpdateCurrentCfg(int nGrpNo, const char* szFilePath)
{
	if (szFilePath == NULL)
    {
        return FALSE;
    }

	MAP< int, CCfgFileGrp* >::iterator it = m_mapCfgFileGrp.find(nGrpNo);
	if (it != m_mapCfgFileGrp.end())
	{
		return ((*it).second)->UpdateCurrentCfg(szFilePath);
	}

	return FALSE;
}

/*******************************************************************
函数名称：UpdateLastCfg(int nGrpNo, char* szFilePath)
功能描述：更新某组的当前配置文件对象
输入参数：
   参数1：int nGrpNo，组号
   参数2：int nFileNo，文件号
输出参数：
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CCfgFileMgr::UpdateLastCfg(int nGrpNo, const char* szFilePath)
{
	if (szFilePath == NULL)
    {
        return FALSE;
    }

	MAP< int, CCfgFileGrp* >::iterator it = m_mapCfgFileGrp.find(nGrpNo);
	if (it != m_mapCfgFileGrp.end())
	{
		return ((*it).second)->UpdateLastCfg(szFilePath);
	}

	return FALSE;
}