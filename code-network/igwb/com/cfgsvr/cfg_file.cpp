/******************************************************************************
Copyright (C), 2003-2011, Huawei Tech. Co., Ltd.

  /****************************************************************************
  File Name       : cfg_file.cpp
  Version         : 1.0
  Author          : dengwanqiu
  Created         : 2003-07-31
  Last Modified   : 
  Description     : implementation of the CCfgFile class.
  Function List   : 
  History         : 
  1 Date          : 2003-07-31
  Author        : dengwanqiu
  Modification  : Created file
******************************************************************************/
#include "cfg_file.h"
#include "resource.h"
#include "../include/toolbox.h"

//记录设置配置参数失败原因，当失败时，将此信息反馈给客户端; 定义在cfg_svr.cpp中
extern char g_szErrorReason[20*1024];

//////////////////////////////////////////////////////////////////////
// CCfgFile Class
//////////////////////////////////////////////////////////////////////

/*******************************************************************
函数名称：CCfgFile()
功能描述：构造函数
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
CCfgFile::CCfgFile() :  m_pCurItemList(NULL)
{
    m_szFilePath[0] = '\0';
	m_szCurSectName[0] = '\0';
}

/*******************************************************************
函数名称：~CCfgFile()
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
CCfgFile::~CCfgFile()
{
    //清除m_mapCfgSect映射表内容
    ClearCfgSectMap();
}

/*******************************************************************
函数名称：Init(const char* szFilePath)
功能描述：初始化操作，从配置文件中读取段名、配置项及配置项值构建配置
          段映射表
输入参数：
   参数1：const char* szFilePath，配置文件全路径
输出参数：
返回内容：初始化成功返回TRUE, 失败则返回FALSE
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CCfgFile::Init(const char *szFileName)
{
    //存取配置文件的全路径名
	SNPRINTF(m_szFilePath, MAX_PATH, "%s", szFileName);
    m_szFilePath[MAX_PATH-1] = '\0';
    
    //打开配置文件；
    FILE *pFile = fopen(m_szFilePath,"rt");
    if (pFile == NULL)
    {
        TRACE(MTS_CFGSVR,
              "%s(%s)",
              S_CFG_OPEN_CFGFILE_ERR,
              m_szFilePath);
        return FALSE;
    }
    
    char szLine[MAX_LINE+1];
    //循环读取每一行内容；
    while (!feof(pFile))
    {
        szLine[0] = '\0';
        //读取配置文件的一行
        if (fgets(szLine, MAX_LINE, pFile) == NULL)
        {
            //文件已读取文件尾
            if (feof(pFile)) break;
            //读文件错误
            TRACE(MTS_CFGSVR,
                  "%s(%s)",
                  S_CFG_READ_CFGFILE_ERR,
                  m_szFilePath);
            fclose(pFile);
            return FALSE;
        }
        //去掉回车、换行及前后空格
        char* pszDiv = strchr(szLine, ';');
        if(NULL != pszDiv)
        {
            pszDiv[0] = '\0';
        }
		StringLRTrim(szLine);
		if (szLine[0] != '\0')
		{
			//调用函数ParseALine()分析每一行内容；
			if (!ParseALine(szLine))
			{
				fclose(pFile);
				return FALSE;
			}
		}
    }
    //加入最后一个配置段
    if (!AddItemListToCfgSectMap())
    {
        fclose(pFile);
        return FALSE;
    }
    //关闭文件；
    fclose(pFile);
    return TRUE;
}

/*******************************************************************
函数名称：AddItemListToCfgSectMap()
功能描述：将一配置段的所有配置信息列表加入配置映射表
输入参数：
输出参数：
返回内容：加入映射表成功返回TRUE,否则返回FALSE
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CCfgFile::AddItemListToCfgSectMap()
{
    //配置项列表不为NULL且找到新段名表明上一配置段结束，
    //需要利用m_szCurSectName和m_pCurItemList在m_mapCfgSect中建立映射关系；
    if (m_pCurItemList != NULL)
    {
        //对INI文件看有没有大小写不一致的,如不一致以ini中配置的第一个为主
        CFGSECT_MAP::iterator it; //add by zkq 2004-09-02
        for(it=m_mapCfgSect.begin(); it != m_mapCfgSect.end(); it++)
        {
            if(StringiCmp(m_szCurSectName,(*it).first.c_str()))
            {
                strcpy(m_szCurSectName,(*it).first.c_str());
                break;
            }
        }//end
	PAIR< CFGSECT_MAP::iterator, bool > pairRet;
        pairRet = m_mapCfgSect.insert( PAIR< STRING, CFGITEM_LIST >(STRING(m_szCurSectName),
            *m_pCurItemList) );
        if (!pairRet.second)
        {
            //在同一映射表中不允许存在两个相同的段名，如有请合并配置文件
            TRACE(MTS_CFGSVR,
                  "%s(%s:%s)",
                  S_CFG_SECTION_REPEAT_ERR,
                  m_szFilePath,
                  m_szCurSectName);

           // return FALSE; //delete by zkq 2004-08-31
        }
        //清除 m_pCurItemList 空间//add by zkq 2004-08-31
        /*while(!m_pCurItemList->empty())
        {
            SCfgItem *pCfgItem = m_pCurItemList->front();
            m_pCurItemList->pop_front();
            delete pCfgItem;
        }*/
        delete m_pCurItemList;
        //end
        m_pCurItemList = NULL;
	m_szCurSectName[0] = '\0';
    }
    return TRUE;
}

/*******************************************************************
函数名称：ParseALine(const char* szLine)
功能描述：分析一行
输入参数：
   参数1：const char* szLine，一行数据(已去掉回车、换行及前后空格)
输出参数：
返回内容：如此行为注释行，或合法的段名，或合法的配置项名、配置项值，
          则成功返回TRUE，否则失败返回FALSE
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CCfgFile::ParseALine(const char *szLine)
{
    //假如行第1个字符为“[”，继续找“]”，如果找不到，返回失败；
    if (szLine[0] == '[')
    {
        const char *pdest = strchr(szLine,']');
        if (pdest == NULL)
        {
            TRACE(MTS_CFGSVR,
                  "%s(%s:%s)",
                  S_CFG_SECTION_INVALID,
                  m_szFilePath,
                  szLine);
            return FALSE;
        }
        //如果找到说明此行“[]”内的是段名, 并取出段名
        char szSection[MAX_LINE+1];
        szSection[0] = '\0';
        int  nLength = pdest - szLine -1;
        
        strncpy(szSection, &szLine[1], nLength);
        szSection[nLength] = '\0';
        
		StringLRTrim(szSection);
        nLength = strlen(szSection);
        if (nLength >= MAX_SECTNAME_LEN || nLength == 0) //段名最大长度硬编码
        {
            //段名过长或为空
            TRACE(MTS_CFGSVR,
                  "%s(%s:%s)",
                  S_CFG_SECTION_INVALID,
                  m_szFilePath,
                  szSection);
            return FALSE;
        }
        
		//如配置项列表不为NULL且找到新段名表明上一配置段结束,加入映射表
        if (!AddItemListToCfgSectMap())
        {
            return FALSE;
        }
		//设置当前配置段名
		strcpy(m_szCurSectName,szSection);

        return TRUE;
    }
    //如为配置项行，调用分析配置行的函数ParseCfgLine()继续处理
    return ParseCfgLine(szLine);
}

/*******************************************************************
函数名称：ParseCfgLine(const char* szLine)
功能描述：分析一配置项行
输入参数：
   参数1：const char* szLine，配置项行(已去掉回车、换行及前后空格)
输出参数：
返回内容：成功取得合法的配置项名及配置项值，则返回TRUE;
          否则返回FALSE;
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CCfgFile::ParseCfgLine(const char *szLine)
{   
	//查找“=”，找不到或"="为第一个字符，返回FALSE；
    const char *pdest = strchr(szLine, '=');
    if (pdest == NULL || pdest == szLine)
    {
        TRACE(MTS_CFGSVR,
              "%s(%s:%s:%s)",
              S_CFG_ITEM_INVALID,
              m_szFilePath,
              m_szCurSectName,
              szLine);
        return FALSE;
    }
    char szItemName[MAX_LINE+1];
    char szItemValue[MAX_LINE+1];
    int  nLength = pdest - szLine;
    
    //取参数项名字符串
    strncpy(szItemName, szLine, nLength);
    szItemName[nLength] = '\0';
    StringLRTrim(szItemName);
    nLength = strlen(szItemName);
    //参数项名长硬编码
    if (nLength >= MAX_ITEMNAME_LEN || nLength == 0) 
    {
        //参数项名过长或为空
        TRACE(MTS_CFGSVR,
              "%s(%s:%s:%s)",
              S_CFG_ITEMNAME_INVALID,
              m_szFilePath,
              m_szCurSectName,
              szItemName);
        return FALSE;
    }
    
    //取参数项值字符串
    strcpy(szItemValue, pdest + 1);
    StringLRTrim(szItemValue);
    nLength = strlen(szItemValue);
    //参数项值长硬编码
    if (nLength >= MAX_ITEMVALUE_LEN) 
    {
        //参数项值过长或为空
        TRACE(MTS_CFGSVR,
              "%s(%s:%s:%s:%s)",
              S_CFG_ITEMVALUE_INVALID,
              m_szFilePath,
              m_szCurSectName,
              szItemName,
              szItemValue);
        return FALSE;
    }
    
    if (m_pCurItemList != NULL)
    {
        //判断是否有重复的配置项名在同一配置段中
        CFGITEM_LIST::iterator it;
        for(it=m_pCurItemList->begin(); it != m_pCurItemList->end(); it++)
        {
            if (StringiCmp((*it)->szItemName, szItemName))
            {
                TRACE(MTS_CFGSVR,
                      "%s(%s:%s:%s)",
                      S_CFG_ITEMNAME_REP_IN_ONESECT,
                      m_szFilePath,
                      m_szCurSectName,
                      szItemName);

                //return FALSE;
               return TRUE; //有重复的配置项名在同一配置段中
            }
        }
    }
    else
    {
        m_pCurItemList = new CFGITEM_LIST();
    }
    //新增一配置项信息
    SCfgItem *pCfgItem = new SCfgItem();
    strcpy(pCfgItem->szItemName, szItemName);
    strcpy(pCfgItem->szItemValue, szItemValue);
    
    m_pCurItemList->push_back(pCfgItem);
    
    return TRUE;
}

/*******************************************************************
函数名称：GetAllSectName(LIST<STRING>& SectNameList)
功能描述：获得所有段名
输入参数：
输出参数：
   参数1：LIST<STRING>& SectNameList，所有段名列表
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CCfgFile::GetAllSectName(LIST< STRING >& SectNameList)
{
    //遍历映射表m_mapCfgSect，把段名全部取出来，加到SectNameList
    CFGSECT_MAP::iterator it;
    
    SectNameList.clear();
    
    for( it=m_mapCfgSect.begin(); it != m_mapCfgSect.end(); it++ )
    {
        SectNameList.push_back( (*it).first );
    }
}

/*******************************************************************
函数名称：GetCfgListFromSectName(const char* szSectName)
功能描述：从段名获得此段所有配置信息
输入参数：
   参数1：const char* szSectName，段名
输出参数：
返回内容：CFGITEM_LIST* ,返回指定段名的参数项信息列表指针 
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
CFGITEM_LIST* CCfgFile::GetCfgListFromSectName(const char *szSectName)
{
    //检查参数合法性
    if (szSectName == NULL) 
    {
        return NULL;
    }
    
    CFGSECT_MAP::iterator it;

    //在映射表m_mapCfgSect中查找段名为szSectName的映射；
    if ( (it=m_mapCfgSect.find( STRING(szSectName) )) != m_mapCfgSect.end() )
    {
        //找到，返回列表指针
        return &((*it).second);
    }

	return NULL;
}


/*******************************************************************
函数名称：SetAllCfgItem(const char* szAllCfgItem)
功能描述：设置参数配置信息，如"段名^配置项名^配置项值|段名^配置项名^配置项值",
          并写入相应的配置文件中
输入参数：
   参数1：const char* szAllCfgItem，参数配置信息
输出参数：
返回内容：成功写入配置文件返回TRUE,否则返回FALSE
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CCfgFile::SetAllCfgItem(const char *szAllCfgItem)
{
    //参数合法性检查
    if (szAllCfgItem == NULL)
    {
        return FALSE;
    }
    //首先清空原来的映射表
    ClearCfgSectMap();
    
    m_pCurItemList = NULL;
    m_szCurSectName[0] = '\0';
    
    int nPos = 0;    //当前指针位置
    int nNextPos;
    int nLen;        //一条配置信息长度
    nLen = strlen(szAllCfgItem);
	//循环取出每个配置项
    while (szAllCfgItem[nPos] != '\0')
    {
        const char *pdest = strchr(&szAllCfgItem[nPos],'|');
        if (pdest != NULL)
        {
            nLen = pdest - szAllCfgItem - nPos;
            nNextPos = nPos + nLen + 1;
        }
        else
        {
            nLen = strlen(&szAllCfgItem[nPos]);
            nNextPos = nPos + nLen;
        }
        if (nLen > 0)
        {
            //分配存储一空信息的空间
			char *pszCfgItem = new  char[nLen+1];
            //取出类似“段名^配置项名^配置项值”的一个配置项的内容
            strncpy(pszCfgItem, szAllCfgItem + nPos, nLen);
            pszCfgItem[nLen] = '\0';
            StringLRTrim(pszCfgItem);
            
            if (pszCfgItem[0] != '\0')
            {
                //调用函数ParseCfgItem()，分析类似“段名^配置项名^配置项值”的一个配置项的内容；
                if ( !ParseCfgItem(pszCfgItem) )
				{
					//更新失败，则恢复原列表内容
                    ClearCfgSectMap();
                    Init(m_szFilePath);
                    //释放临时空间
                    delete [] pszCfgItem;
                    return FALSE;
				}
            }
            //释放临时空间
            delete [] pszCfgItem;
        }
        
        nPos = nNextPos;  //移动指针位置
    }

	//写配置文件
	return WriteAllCfgItem();
}

/*******************************************************************
函数名称：ParseCfgItem(const char* szCfgItem)
功能描述：分析配置项，如"段名^配置项名^配置项值"
输入参数：
   参数1：const char* szCfgItem，配置项信息
输出参数：
返回内容：成功取得合法的段名、配置项名、配置项值返回TRUE;
          否则返回FALSE;
调用函数：
被调函数：
作者时间：denqwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CCfgFile::ParseCfgItem(const char *szCfgItem)
{  
    char szSectName[MAX_LINE+1]  = "";
    char szItemName[MAX_LINE+1]  = "";
    char szItemValue[MAX_LINE+1] = "";

    //判断配置项是否为合法形式："段名^配置项名^配置项值"
    const char *pSectDiv = strchr(szCfgItem,'^');
    if (pSectDiv == NULL || pSectDiv == szCfgItem)
    {
        //记录错误原因
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s)",
                 S_CFG_SET_CFGINFO_INVALID,
                 szCfgItem);
        //写Trace文件 
        TRACE(MTS_CFGSVR, g_szErrorReason);
        return FALSE;
    }

    //判断段名是否太长或为空
    int nCopyLen = int(pSectDiv - szCfgItem);
    memcpy(szSectName, szCfgItem, nCopyLen);
    szSectName[nCopyLen] = '\0';
    StringLRTrim(szSectName);
    if(szSectName[0] == '\0')
    {
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s)",
                 S_CFG_SET_SECTNAME_NULL,
                 szCfgItem);
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }
    if(strlen(szSectName) >= MAX_SECTNAME_LEN)
    {
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s)",
                 S_CFG_SET_SECTNAME_TOOLONG,
                 szCfgItem);
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }

    //判断域名是否太长或为空
    const char *pItemName = pSectDiv + 1;
    const char* pItemDiv = strchr(pItemName, '^');
    if (pItemDiv == NULL || pItemDiv == pItemName)
    {
        //记录错误原因
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s)",
                 S_CFG_SET_CFGINFO_INVALID,
                 szCfgItem);
        //写Trace文件 
        TRACE(MTS_CFGSVR, g_szErrorReason);
        return FALSE;
    }
    nCopyLen = int(pItemDiv - pItemName);
    memcpy(szItemName, pItemName, nCopyLen);
    szItemName[nCopyLen] = '\0';
    StringLRTrim(szItemName);
    if(szItemName[0] == '\0')
    {
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s)",
                 S_CFG_SET_ITEMNAME_NULL,
                 szCfgItem);
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }
    if(strlen(szItemName) >= MAX_ITEMNAME_LEN)
    {
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s:%s)",
                 S_CFG_SET_ITEMNAME_TOOLONG,
                 szSectName,
                 szItemName);
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }

    //判断域值是否太长或为空
    const char *pItemValue = pItemDiv + 1;
    strcpy(szItemValue, pItemValue);
    StringLRTrim(szItemValue);
    if (szItemValue[0] == '\0')
    {
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s:%s)",
                 S_CFG_SET_ITEMVALUE_NULL,
                 szSectName,
                 szItemName);
        //写Trace文件 
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }
    if(strlen(szItemValue) >= MAX_ITEMVALUE_LEN)
    {
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s:%s)",
                 S_CFG_SET_ITEMVALUE_TOOLONG,
                 szSectName,
                 szItemName);
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }
/*
    
    //分析出段名szSectName；
    const char *pdest = strchr(szCfgItem,'^');
    if (pdest == NULL || pdest == szCfgItem)
    {   
        //记录错误原因
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s)"
                 S_CFG_SET_CFGINFO_INVALID,
                 szCfgItem);
        //写Trace文件 
        TRACE(MTS_CFGSVR, g_szErrorReason);
        return FALSE;
    }
    int nSectNameLen = pdest - szCfgItem;    //段名长度  
    strncpy(szSectName,szCfgItem,nSectNameLen);
    szSectName[nSectNameLen] = '\0';
    StringLRTrim(szSectName);
    int nLen = strlen(szSectName);
    //判断段名是否太长或为空
    if ( nLen >= MAX_SECTNAME_LEN  || nLen == 0 )
    {   
        //记录错误原因
        if (nLen == 0)
        {
            SNPRINTF(g_szErrorReason,
                     sizeof(g_szErrorReason),
                     "%s(%s)"
                     S_CFG_SET_SECTNAME_NULL,
                     szCfgItem);
        }
        else
        {
            SNPRINTF(g_szErrorReason,
                     sizeof(g_szErrorReason),
                     "%s(%s)"
                     S_CFG_SET_SECTNAME_TOOLONG,
                     szCfgItem);
        }
        //写Trace文件 
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }
	//已到字符串尾，表明无参数项名 
    if (szCfgItem[nSectNameLen+1] == '\0')
    {
        //记录错误原因
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s)"
                 S_CFG_SET_NO_ITEMNAME,
                 szCfgItem);
        //写Trace文件 
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }
    
	//取参数项名
    szItemName[0] = '\0';
    pdest = strchr(szCfgItem + nSectNameLen + 1,'^');
    //判断参数项名是否为空
    if (pdest == NULL || pdest == szCfgItem + nSectNameLen + 1)
    {
        //取参数项名，为了记录日志需要
        if (pdest == NULL)
        {
            strcpy(szItemName, szCfgItem + nSectNameLen + 1);
            StringLRTrim(szItemName);
        }
        //记录错误原因: 无参数项值
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s:%s)"
                 S_CFG_SET_NO_ITEMVALUE,
                 szSectName,
                 szItemName);
        //写Trace文件 
        TRACE(MTS_CFGSVR,g_szErrorReason);

		return FALSE;
    }

    int nItemNameLen = pdest - szCfgItem - nSectNameLen -1;
    strncpy(szItemName,szCfgItem + nSectNameLen + 1,nItemNameLen);
    szItemName[nItemNameLen] = '\0';
    StringLRTrim(szItemName);
    nLen = strlen(szItemName);
    //判断参数项名是否太长或为空
    if (nLen >= MAX_ITEMNAME_LEN || nLen == 0)
    {
        //记录错误原因
        if (nLen == 0)
        {
            SNPRINTF(g_szErrorReason,
                     sizeof(g_szErrorReason),
                     "%s(%s:%s)"
                     S_CFG_SET_ITEMNAME_NULL,
                     szSectName,
                     szItemName);
        }
        else
        {
            SNPRINTF(g_szErrorReason,
                     sizeof(g_szErrorReason),
                     "%s(%s:%s)"
                     S_CFG_SET_ITEMNAME_TOOLONG,
                     szSectName,
                     szItemName);
        }
        //写Trace文件 
        TRACE(MTS_CFGSVR,
              "%s(%s:%s:%s)",
              S_CFG_SET_ITEMNAME_INVALID,
              m_szFilePath,
              szSectName,
              szItemName);
        return FALSE;
    }
    //已到字符串尾，表明无参数项值
	if (szCfgItem[nSectNameLen + nItemNameLen + 2] == '\0')
    {
        //记录错误原因
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s:%s)"
                 S_CFG_SET_ITEMVALUE_NULL,
                 szSectName,
                 szItemName);
        //写Trace文件 
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }
    
    //取参数项值
    strncpy(szItemValue, &szCfgItem[nSectNameLen + nItemNameLen + 2], MAX_LINE);
    szItemValue[MAX_LINE] = '\0';
    StringLRTrim(szItemValue);
    nLen = strlen(szItemValue);
    //判断参数项值是否太长或为空
    if (nLen >= MAX_ITEMVALUE_LEN || nLen == 0)
    {
        //记录错误原因
        if (nLen == 0)
        {
            SNPRINTF(g_szErrorReason,
                     sizeof(g_szErrorReason),
                     "%s(%s:%s)"
                     S_CFG_SET_ITEMVALUE_NULL,
                     szSectName,
                     szItemName);
        }
        else
        {
            SNPRINTF(g_szErrorReason,
                     sizeof(g_szErrorReason),
                     "%s(%s:%s)"
                     S_CFG_SET_ITEMVALUE_TOOLONG,
                     szSectName,
                     szItemName);
        }
        //写Trace文件 
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    } */

    
    SCfgItem *pCfgItem = new  SCfgItem;
    strcpy(pCfgItem->szItemName, szItemName);
    strcpy(pCfgItem->szItemValue, szItemValue);
    
	//如果此段名已在映射表中，则将此配置项加入或修改此段对应的配置项列表
	CFGSECT_MAP::iterator it = m_mapCfgSect.find( STRING(szSectName) );
	if (it != m_mapCfgSect.end())
	{
		CFGITEM_LIST &ItemList = (*it).second;
		CFGITEM_LIST::iterator it_list;
		for (it_list = ItemList.begin(); it_list != ItemList.end(); it_list++)
		{
			//此配置项在列表中，只修改其值。这表明如有相同的“段名^项名^项值”，
            //则取最后的值
			if (strcmp((*it_list)->szItemName, szItemName) == 0)
			{
				strcpy((*it_list)->szItemValue, szItemValue);
				return TRUE;
			}
		}	
		ItemList.push_back(pCfgItem);
		return TRUE;
	}
	//否则，加入新段名及配置项列表到映射表中
	else
	{
		CFGITEM_LIST *pItemList = new CFGITEM_LIST();
        pItemList->push_back(pCfgItem);
		m_mapCfgSect.insert(PAIR< STRING, CFGITEM_LIST >(STRING(szSectName), 
            *pItemList) );
		return TRUE;
	}
    
	return TRUE;
}

/*******************************************************************
函数名称：WriteAllCfgItem()
功能描述：写所有参数配置信息
输入参数：
输出参数：
返回内容：成功返回TRUE,失败返回FALSE；
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
#ifdef _PLATFORM_WIN32
#define S_LN_CR "\r\n"
#else
#define S_LN_CR "\n"
#endif
BOOL CCfgFile::WriteAllCfgItem()
{
    //打开配置文件，设置为可写和创建模式；
    FILE *pFile = fopen(m_szFilePath,"wb");//by zkq  2004-0713 SWPD06718
    if (pFile == NULL)
    {
        //记录错误原因
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s)",
                 S_CFG_SET_OPENFILE_ERROR,
                 m_szFilePath);
        //写Trace文件
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }
    
    //遍历m_mapCfgSect映射表，依次取出一个段和此段的配置项；
    CFGSECT_MAP::iterator it;
    for(it=m_mapCfgSect.begin(); it != m_mapCfgSect.end(); it++)
    {
        //写入一个段名
        char szOutBuf[2048];
        SNPRINTF(szOutBuf,
                 sizeof(szOutBuf),
                 "[%s]%s",
                 ((*it).first).c_str(),
                 S_LN_CR);
        int nLen = strlen(szOutBuf);
        if ( fwrite(szOutBuf, nLen, 1, pFile) != 1 )
        { 
            //记录错误原因
            fclose(pFile);
            SNPRINTF(g_szErrorReason,
                     sizeof(g_szErrorReason),
                     "%s(%s)",
                     S_CFG_SET_WRITEFILE_ERROR,
                     m_szFilePath);
            //写Trace文件
            TRACE(MTS_CFGSVR,g_szErrorReason);
            return FALSE;
        }
        //写一个此段的所有配置项；
        CFGITEM_LIST *pItemList = &((*it).second);
        
        CFGITEM_LIST::iterator itItem;
        for( itItem=pItemList->begin(); itItem != pItemList->end(); itItem++ )
        {
            SCfgItem *Item = (*itItem);
            if (Item != NULL)
            {
                SNPRINTF(szOutBuf,
                         sizeof(szOutBuf),
                         "%s=%s%s",
                         Item->szItemName,
                         Item->szItemValue,
                         S_LN_CR);
                int nItemLen = strlen(szOutBuf);
                if ( fwrite(szOutBuf, nItemLen, 1, pFile) != 1 )
                {
                    //记录错误原因
                    fclose(pFile);
                    SNPRINTF(g_szErrorReason,
                             sizeof(g_szErrorReason),
                             "%s(%s)",
                             S_CFG_SET_WRITEFILE_ERROR,
                             m_szFilePath);
                    //写Trace文件
                    TRACE(MTS_CFGSVR,g_szErrorReason);
                    return FALSE;
                }
            }
        }
        //每段间加入一空行
        fprintf(pFile, S_LN_CR);
    }
    fclose(pFile);
    return TRUE;
}

/*******************************************************************
函数名称：GetAllCfgItem(STRING &strAllCfgItem)
功能描述：获得所有参数配置信息
输入参数：
输出参数：
   参数1：STRING &strAllCfgItem，参数配置信息 
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CCfgFile::GetAllCfgItem(STRING &strAllCfgItem)
{    
    //先清空字符内容
    strAllCfgItem = "";
    
    //从映射表中取出所有配置信息，组装为"段名^配置项名^配置项值|"的字符串；
    CFGSECT_MAP::iterator it;
    for( it=m_mapCfgSect.begin(); it != m_mapCfgSect.end(); it++)
    {
        CFGITEM_LIST &ItemList = (*it).second;
        CFGITEM_LIST::iterator itItem;
        
        char szItemBuf[MAX_LINE];
        for(itItem=ItemList.begin(); itItem != ItemList.end(); itItem++)
        {
            sprintf(szItemBuf, "%s^%s^%s|", ((*it).first).c_str(), 
                (*itItem)->szItemName,(*itItem)->szItemValue); 
            strAllCfgItem.append(szItemBuf); 
        }
    }

    //去掉最后一个"|"
    if (strAllCfgItem != "")
    {
        strAllCfgItem[strAllCfgItem.size()-1] = '\0';
    }
}

/*******************************************************************
函数名称：ClearCfgSectMap()
功能描述：清空配置映射表
输入参数：
输出参数：
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CCfgFile::ClearCfgSectMap()
{
    //清除映射表
    CFGSECT_MAP::iterator it;
    for(it=m_mapCfgSect.begin(); it != m_mapCfgSect.end(); it++)
    {
        //清除列表内容
        CFGITEM_LIST &ItemList = (*it).second;
        while( !ItemList.empty() )
        {
            SCfgItem *pCfgItem = ItemList.front();
            ItemList.pop_front();
            delete pCfgItem;
        }
    }
    m_mapCfgSect.clear();
    //清除未加入的配置列表 
    if (m_pCurItemList != NULL)
    {
        while(!m_pCurItemList->empty())
        {
            SCfgItem *pCfgItem = m_pCurItemList->front();
            m_pCurItemList->pop_front();
            delete pCfgItem;
        }
        delete m_pCurItemList;
        m_pCurItemList = NULL;
    }
}
