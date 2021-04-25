/******************************************************************************

  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.
  
    ******************************************************************************
    File Name     : para_file.h
    Version       : 1.0
    Author        : dengwanqiu
    Created       : 2003/07/31
    Last Modified :
    Description   : interface for the CParaFile class.
    Function List :
    History       :
    1.Date        : 2003-07-31
    Author      : dengwanqiu
    Modification: Created file
    
******************************************************************************/
#ifndef __PARA_FILE_H__
#define __PARA_FILE_H__

#include "cfg_def.h"

/*================================================================================
参数文件格式

  参数文件格式就是allpara文件的格式，每个参数项的属性都要列举出来，具体格式如下：
  [参数项名称] 
  SectList= 段名1^段名2^段名3（所属段）
  SectRela= &段名(段相关)
  Desc=描述
  Type=类型
  Default=缺省值
  MustFlag=必配标志 
  UpdateFlag=可改标志
  
================================================================================*/

//参数项类型枚举结构
enum  PARATYPE
{ 
    PARATYPE_UNKNOWN	= -1,      //无效数据类型
    PARATYPE_NUM		= 0,       //数值类型
    PARATYPE_STR		= 1,       //字符串类型 
    PARATYPE_ENUM		= 2,       //枚举类型
    PARATYPE_IP			= 3,       //IP地址类型
    PARATYPE_DATE		= 4,       //日期类型
    PARATYPE_TIME		= 5,       //时间类型
    PARATYPE_DATETIME	= 6        //日期时间类型
};

//参数项信息结构
struct  SParaItem
{
    char      szItemName[MAX_ITEMNAME_LEN];      //参数项名
    char      szItemDesc[MAX_ITEMDESC_LEN];      //参数项描述 
    PARATYPE  nItemType;                         //参数项类型
    char      szDefaultValue[MAX_ITEMVALUE_LEN]; //当类型是ENUM时，按照下列举：缺省值：枚举1，枚举2，
    char      szParaRelaSect[MAX_SECTNAME_LEN];  //参数项相关段名
    BOOL      bIsMustCfg;                        //是否必须配置项
    BOOL      bIsCanChg;                         //能否能修改此配置项

	//构造函数
	SParaItem()
	{
		szItemName[0] = '\0';
		szItemDesc[0] = '\0';
		nItemType = PARATYPE_UNKNOWN;       //指定非定义类型，用于判断参数信息文件是否指定参数项数据类型
		szDefaultValue[0] = '\0';
		szParaRelaSect[0] = '\0';
		bIsMustCfg = FALSE;
		bIsCanChg = FALSE;
	}
};

//段是由多个参数项组成的，需要为一个参数列表定义一个类型
typedef  LIST< SParaItem* >  PARAITEM_LIST;

//段名映射到此段的参数项列表，需要定义一个映射类型
typedef  MAP< STRING, PARAITEM_LIST >  PARASECT_MAP;

/*===============================================================================
描述：参数信息文件类
每个参数信息文件使用一个CParaFile类进行抽象
===============================================================================*/
class CParaFile  
{
public:
    CParaFile();
    virtual ~CParaFile();
    
    //初始化函数，打开文件，分析文件内容，初始化m_mapParaSect
    BOOL Init(const char *szFileName);
    //获得所有段名
    void GetAllSectName(LIST< STRING >& SectNameList);
    //通过段名获得此段的所有参数项列表
    PARAITEM_LIST* GetParaListFromSectName(const char *szSectName);

protected:
    char                m_szFilePath[MAX_PATH];             //参数文件路径
    char                m_szCurSectName[MAX_SECTNAME_LEN];  //当前段的名称
    SParaItem*          m_pCurParaItem;                     //当前参数项
    PARASECT_MAP        m_mapParaSect;                      //参数段结构列表
    LIST< STRING >      m_SectNameList;                     //段名列表
    
    //分析参数文件每一行内容
    BOOL ParseALine(const char* szLine);
    //分析参数项行内容
    BOOL ParseParaLine(const char* szLine);

//新增函数
private:
	//将一参数项信息加入到参数段结构列表
	BOOL AddParaItemToParaSectMap();
    //通过数据类型字符串获得数据类型
    PARATYPE GetParaDataType(const char *szValue) const;
};

#endif //__PARA_FILE_H__
