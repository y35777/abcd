/******************************************************************************
Copyright (C), 2003-2011, Huawei Tech. Co., Ltd.

  /****************************************************************************
  File Name       : file_grp.cpp
  Version         : 1.0
  Author          : dengwanqiu
  Created         : 2003-07-31
  Last Modified   : 
  Description     : implementation of the CCfgFileGrp class.
  Function List   : 
  History         : 
  1 Date          : 2003-07-31
  Author        : dengwanqiu
  Modification  : Created file
******************************************************************************/
#include "file_grp.h"

//////////////////////////////////////////////////////////////////////
// CCfgFileGrp Class
//////////////////////////////////////////////////////////////////////

/*******************************************************************
函数名称：CCfgFileGrp()
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
CCfgFileGrp::CCfgFileGrp() : m_pFacFile(NULL), m_pSucFile(NULL), m_pCurFile(NULL)
{
}

/*******************************************************************
函数名称：~CCfgFileGrp()
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
CCfgFileGrp::~CCfgFileGrp()
{
    if (m_pFacFile != NULL)
    {
        delete m_pFacFile;
        m_pFacFile = NULL;
    }
    if (m_pSucFile != NULL)
    {
        delete m_pSucFile;
        m_pSucFile = NULL;
    }
    if (m_pCurFile != NULL)
    {
        delete m_pCurFile;
        m_pCurFile = NULL;
    }
}

/*******************************************************************
函数名称：Init(char* szFilePath)
功能描述：初始化文件组
输入参数：
   参数1：char* szFilePath，文件组路径(不含扩展名的配置文件全路径)
输出参数：
返回内容：成功返回TRUE,失败返回FALSE；
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CCfgFileGrp::Init(const char *szFilePath)
{
	//检查参数合法性
    if (szFilePath == NULL)
    {
        return FALSE;
    }
    char szFileFullPath[MAX_PATH];

	sprintf(szFileFullPath, "%s%s", szFilePath, ".bas");
    //初始化出厂的配置文件m_pFacFile；
    if (m_pFacFile != NULL)
    {
        delete m_pFacFile;
    }
    m_pFacFile = new  CCfgFile();
    if(ACE_OS::access(szFileFullPath, F_OK) != 0)
    {
        STRING strResIni = szFilePath;
        strResIni       += ".ini";
        FileCopy(strResIni.c_str(), szFileFullPath);
    }
    if (!m_pFacFile->Init(szFileFullPath))
    {
        return FALSE;
    }
    
    sprintf(szFileFullPath, "%s%s", szFilePath, ".lst");
    //初始化上次成功的配置文件m_pSucFile；
    if (m_pSucFile != NULL)
    {
        delete m_pSucFile;
    }
    m_pSucFile = new  CCfgFile();
    if(ACE_OS::access(szFileFullPath, F_OK) != 0)
    {
        STRING strResIni = szFilePath;
        strResIni       += ".ini";
        FileCopy(strResIni.c_str(), szFileFullPath);
    }
    if (!m_pSucFile->Init(szFileFullPath))
    {
        return FALSE;
    }
    
    sprintf(szFileFullPath, "%s%s", szFilePath, ".ini");
    //初始化当前的配置文件m_pCurFile；
    if (m_pCurFile != NULL)
    {
        delete m_pCurFile;
    }
    m_pCurFile = new  CCfgFile();	
    if (!m_pCurFile->Init(szFileFullPath))
    {
        return FALSE;
    }
    
    return TRUE;
}

/*******************************************************************
函数名称：GetAllSectName(LIST<STRING>& SectNameList, int nFileNo)
功能描述：根据文件号获得所有段名
输入参数：
   参数1：int nFileNo，文件号
          注：按照约定，0表示出厂的，1表示上次成功的，2表示当前的
输出参数：
   参数1：LIST<STRING>& SectNameList，所有段名列表
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CCfgFileGrp::GetAllSectName(LIST< STRING >& SectNameList, int nFileNo)
{
    switch(nFileNo)
    {
    case FILE_TYPE_FAC:
        //出厂配置
        if (m_pFacFile != NULL)
        {
            m_pFacFile->GetAllSectName(SectNameList);
        }
        break;
    case FILE_TYPE_SUC:
        //上次配置
        if (m_pSucFile != NULL)
        {
            m_pSucFile->GetAllSectName(SectNameList);
        }
        break;
    case FILE_TYPE_CUR:
        //当前配置
        if (m_pCurFile != NULL)
        {
            m_pCurFile->GetAllSectName(SectNameList);
        }
        break;
    default:
        break;
    }
}

/*******************************************************************
函数名称：GetCfgListFromSectName( 
                            const char* szSectName, int nFileNo)
功能描述：根据文件号和段名获得此段的参数配置信息
输入参数：
   参数1：const char* szSectName，段名
   参数2：int nFileNo，文件号
输出参数：
返回内容：CFGITEM_LIST* ,返回指定段名的参数项信息列表指针
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
CFGITEM_LIST* CCfgFileGrp::GetCfgListFromSectName(const char *szSectName, int nFileNo)
{
	if (szSectName == NULL)
    {
        return FALSE;
    }
    
    switch(nFileNo)
    {
    case FILE_TYPE_FAC:
        //出厂配置
        if (m_pFacFile != NULL)
        {
            return m_pFacFile->GetCfgListFromSectName(szSectName);
        }
        return NULL;
    case FILE_TYPE_SUC:
        //上次配置
        if (m_pSucFile != NULL)
        {
            return m_pSucFile->GetCfgListFromSectName(szSectName);
        }
        return NULL;
    case FILE_TYPE_CUR:
        //当前配置
        if (m_pCurFile != NULL)
        {
            return m_pCurFile->GetCfgListFromSectName(szSectName);
        }
        return NULL;
    default:
        break;
    }

	return NULL;
}


/*******************************************************************
函数名称：SetAllCfgItem(const char* szAllCfgItem, int nFileNo)
功能描述：根据文件号设置参数配置信息
输入参数：
   参数1：int nFileNo，文件号
   参数2：const char* szAllCfgItem，所有参数配置信息
输出参数：
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CCfgFileGrp::SetAllCfgItem(const char *szAllCfgItem, int nFileNo)
{
	switch(nFileNo)
    {
    case FILE_TYPE_FAC:
        //出厂配置
        if (m_pFacFile != NULL)
        {
            return m_pFacFile->SetAllCfgItem(szAllCfgItem);
        }
		break;
    case FILE_TYPE_SUC:
        //上次配置
        if (m_pSucFile != NULL)
        {
            return m_pSucFile->SetAllCfgItem(szAllCfgItem);
        }
		break;
    case FILE_TYPE_CUR:
        //当前配置
        if (m_pCurFile != NULL)
        {
            return m_pCurFile->SetAllCfgItem(szAllCfgItem);
        }
        break;
    default:
        break;
    }

    return FALSE;
}

/*******************************************************************
函数名称：GetAllCfgItem(char* szAllCfgItem, int nFileNo)
功能描述：根据文件号获得所有参数配置信息
输入参数：
   参数1：int nFileNo，文件号
输出参数：
   参数1：STRING &szAllCfgItem，所有参数配置信息
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CCfgFileGrp::GetAllCfgItem(STRING &strAllCfgItem, int nFileNo)
{
	switch(nFileNo)
    {
    case FILE_TYPE_FAC:
        //出厂配置
        if (m_pFacFile != NULL)
        {
            m_pFacFile->GetAllCfgItem(strAllCfgItem);
        }
        break;
    case FILE_TYPE_SUC:
        //上次配置
        if (m_pSucFile != NULL)
        {
            m_pSucFile->GetAllCfgItem(strAllCfgItem);
        }
        break;
    case FILE_TYPE_CUR:
        //当前配置
        if (m_pCurFile != NULL)
        {
            m_pCurFile->GetAllCfgItem(strAllCfgItem);
        }
        break;
    default:
        break;
    }
}

/*******************************************************************
函数名称：UpdateCurrentCfg(const char* szFilePath)
功能描述：更新当前配置文件
输入参数：
   参数1：const char* szFilePath，当前配置文件全路径
输出参数：
返回内容：成功返回TRUE;失败返回FALSE;
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CCfgFileGrp::UpdateCurrentCfg(const char* szFilePath)
{
	//参数合法性检查
    if (szFilePath == NULL)
    {
        return FALSE;
    }

    //检查配置文件的合法性
    CCfgFile *pNewCurFile = new CCfgFile();
	if (!pNewCurFile->Init(szFilePath))
    {
        //如不能成功初始化手工或其它进程错误修改的配置文件，则用原配置
        //文件覆盖配置文件
        if (m_pCurFile != NULL)
        {
            m_pCurFile->WriteAllCfgItem();
        }
        return FALSE;
    }

    //配置文件合法，更新配置文件映射表内容
    if (m_pCurFile != NULL)
	{
		delete m_pCurFile;
	}
    m_pCurFile = pNewCurFile;

    return TRUE;
}

/*******************************************************************
函数名称：UpdateLastCfg(const char* szFilePath)
功能描述：更新上次配置文件
输入参数：
   参数1：const char* szFilePath，上次配置文件全路径
输出参数：
返回内容：成功返回TRUE;失败返回FALSE;
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CCfgFileGrp::UpdateLastCfg(const char* szFilePath)
{
	//参数合法性检查
    if (szFilePath == NULL)
    {
        return FALSE;
    }

    //检查配置文件的合法性
    CCfgFile *pNewLstFile = new CCfgFile();
	if (!pNewLstFile->Init(szFilePath))
    {
        //如不能成功初始化上次配置文件，则用上次配置文件映射表内容覆盖上次配置文件
        if (m_pSucFile != NULL)
        {
            m_pSucFile->WriteAllCfgItem();
        }
        return FALSE;
    }

    //配置文件合法，更新配置文件映射表内容
    if (m_pSucFile != NULL)
	{
		delete m_pSucFile;
	}
    m_pSucFile = pNewLstFile;

    return TRUE;
}