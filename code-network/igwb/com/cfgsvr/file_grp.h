/******************************************************************************

  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.
  
    ******************************************************************************
    File Name     : file_grp.h
    Version       : 1.0
    Author        : dengwanqiu
    Created       : 2003/07/31
    Last Modified :
    Description   : interface for the CCfgFileGrp  class.
    Function List :
    History       :
    1.Date        : 2003-07-31
    Author      : dengwanqiu
    Modification: Created file
    
******************************************************************************/
#ifndef __FILE_GROUP_H__
#define __FILE_GROUP_H__

#include "cfg_def.h"

#include "cfg_file.h"

/*===============================================================================
描述：配置文件组类
远程配置功能支持多种文件的配置，其中对每种都有一组文件：
(1) 出厂配置:     .bas
(2) 上次配置: .lst
(3) 当前配置:     .ini
===============================================================================*/
class CCfgFileGrp  
{
public:
    CCfgFileGrp();
    virtual ~CCfgFileGrp();
    
    //初始化配置文件组
    BOOL Init(const char* szFilePath);
    //通过文件号获得此文件的所有段名
    void GetAllSectName(LIST< STRING >& SectNameList, int nFileNo);
    //根据段名获得此段的配置项列表
    CFGITEM_LIST* GetCfgListFromSectName(const char* szSectName, int nFileNo);
	
    //设置某配置文件的所有配置项
    BOOL SetAllCfgItem(const char* szAllCfgItem, int nFileNo);
    //取所有配置项
    void GetAllCfgItem(STRING &strAllCfgItem , int nFileNo);
    //更新当前配置文件
	BOOL UpdateCurrentCfg(const char* szFilePath);
    //更新上次配置文件
	BOOL UpdateLastCfg(const char* szFilePath);
    
protected:
    CCfgFile*  m_pFacFile;     //出厂的配置文件
    CCfgFile*  m_pSucFile;     //上次的配置文件
    CCfgFile*  m_pCurFile;     //当前的配置文件
};

#endif //__FILE_GROUP_H__
