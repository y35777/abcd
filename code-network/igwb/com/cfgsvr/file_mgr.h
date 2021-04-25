/******************************************************************************

  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.
  
    ******************************************************************************
    File Name     : file_mgr.h
    Version       : 1.0
    Author        : dengwanqiu
    Created       : 2003/07/31
    Last Modified :
    Description   : interface for the CCfgFileMgr  class.
    Function List :
    History       :
    1.Date        : 2003-07-31
    Author      : dengwanqiu
    Modification: Created file
    
******************************************************************************/
#ifndef __FILE_MGR_H__
#define __FILE_MGR_H__

#include "cfg_def.h"

#include "cfg_file.h"

class CCfgFileGrp;
/*===============================================================================
描述：配置文件管理类
对于多个参数配置文件，就相应对应多个配置文件组。而CCfgFileMgr
用于管理这些配置文件组。
===============================================================================*/
class CCfgFileMgr  
{
public:
    CCfgFileMgr();
    virtual ~CCfgFileMgr();
    
    //增加一个配置组
    BOOL AddCfgFileGrp(int nGrpNo, const char* szFilePath);
    //根据组号和文件号获得某配置文件的所有段名
    void GetAllSectName(LIST< STRING >& SectNameList, int nGrpNo, int nFileNo);
    //根据组号、文件号和段名获得某配置文件某段的所有配置项
    CFGITEM_LIST * GetCfgListFromSectName(const char* szSectName,
                                       int nGrpNo, int nFileNo);
    //根据组号和文件号设置某文件的所有配置项
    BOOL SetAllCfgItem(const char* szAllCfgItem, int nGrpNo, int nFileNo);
    
    //获得文件组名
    void GetCfgFileClass(LIST< STRING >& GrpNameList);
    //取得所有配置项信息
    void GetAllCfgItem(STRING &strAllCfgItem, int nGrpNo, int nFileNo);
    //更新指定组的当前配置文件
	BOOL UpdateCurrentCfg(int nGrpNo, const char* szFilePath);
    //更新指定组的上次配置文件
	BOOL UpdateLastCfg(int nGrpNo, const char* szFilePath);
    
protected:
    MAP< int, CCfgFileGrp* >  m_mapCfgFileGrp;    //配置文件组号和配置文件组的映射表
    MAP< int, STRING >        m_mapCfgFileClass;  //配置文件组号和配置文件组名的映射表
};


#endif //__FILE_MGR_H__
