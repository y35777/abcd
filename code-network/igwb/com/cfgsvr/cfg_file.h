/******************************************************************************

  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.
  
    ******************************************************************************
    File Name     : cfg_file.h
    Version       : 1.0
    Author        : dengwanqiu
    Created       : 2003/07/31
    Last Modified :
    Description   : interface for the CCfgFile.  class.
    Function List :
    History       :
    1.Date        : 2003-07-31
    Author      : dengwanqiu
    Modification: Created file
    
******************************************************************************/
#ifndef __CFG_FILE_H__
#define __CFG_FILE_H__


#include "cfg_def.h"

/*================================================================================
配置文件格式

  配置文件格式分段行和项行：
  [段名]--------------------------------------->段行格式
  配置项名=配置项值--------------------->项行格式
  
================================================================================*/

//配置文件的配置项结构
struct SCfgItem
{
    char szItemName[MAX_ITEMNAME_LEN];       //配置项名称
    char szItemValue[MAX_ITEMVALUE_LEN];     //配置项值
};

//配置项列表，用于存储配置段信息（每个配置段由多个配置项组成）
typedef LIST< SCfgItem * > CFGITEM_LIST;

//配置段映射表，用于存储配置文件信息（每个配置文件由多个配置段组成）
typedef MAP< STRING, CFGITEM_LIST > CFGSECT_MAP;

/*===============================================================================
描述：配置文件类
每个配置文件使用一个CCfgFile类进行抽象
===============================================================================*/
class CCfgFile  
{
public:
    CCfgFile();
    virtual ~CCfgFile();
    
    //初始化函数，打开文件，分析文件内容，初始化m_mapCfgSectt；
    BOOL Init(const char* szFileName);
    //获得所有段名
    void GetAllSectName(LIST< STRING >&  SectNameList);
    //通过段名获得此段的所有参数项列表
    CFGITEM_LIST* GetCfgListFromSectName(const char* szSectName);
    //设置所有配置项信息
    BOOL SetAllCfgItem(const char* szAllCfgItem);
    
    //取得所有配置项信息
    void GetAllCfgItem(STRING &strAllCfgItem); 

    //把所有配置项都写到配置文件中
    BOOL WriteAllCfgItem();    
protected:
    char          m_szFilePath[MAX_PATH];               //配置文件的路径
    char          m_szCurSectName[MAX_SECTNAME_LEN];    //当前段的名称
    CFGITEM_LIST* m_pCurItemList;                       //当前段的配置列表
    CFGSECT_MAP   m_mapCfgSect;                         //配置段结构列表
    
    //分析每一行内容
    BOOL ParseALine(const char* szLine);
    //分析配置项行内容
    BOOL ParseCfgLine(const char* szLine);
    //分析一个配置项内容
    BOOL ParseCfgItem(const char* szCfgItem);
private:
    //将一配置段的所有配置信息列表加入配置映射表
    BOOL AddItemListToCfgSectMap();
    //清空配置映射表
    void ClearCfgSectMap();
};


#endif //__CFG_FILE_H__
