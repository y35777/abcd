/******************************************************************************
Copyright (C), 2003-2011, Huawei Tech. Co., Ltd.

  /****************************************************************************
  File Name       : para_file.cpp
  Version         : 1.0
  Author          : dengwanqiu
  Created         : 2003-07-31
  Last Modified   : 
  Description     : implementation of the CParaFile class.
  Function List   : 
  History         : 
  1 Date          : 2003-07-31
  Author        : dengwanqiu
  Modification  : Created file
******************************************************************************/

#include "para_file.h"
#include "resource.h"

/*******************************************************************
函数名称：CParaFile()
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
CParaFile::CParaFile() :  m_pCurParaItem(NULL)
{
    m_szFilePath[0] = '\0';
	m_szCurSectName[0] = '\0';
}

/*******************************************************************
函数名称：~CParaFile()
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
CParaFile::~CParaFile()
{
    //清除m_mapParaSectt映射表内容
    PARASECT_MAP::iterator  it;    
	
    for(it=m_mapParaSect.begin(); it != m_mapParaSect.end(); it++)
	{
		PARAITEM_LIST &ItemList = (*it).second;
        while( !ItemList.empty() )
        {
            SParaItem *pParaItem = ItemList.front();
            ItemList.pop_front();
            delete pParaItem;
        }
	}
    m_mapParaSect.clear();
    if (m_pCurParaItem != NULL)
    {
        delete m_pCurParaItem;
        m_pCurParaItem = NULL;
    }
}

/*******************************************************************
函数名称：Init(const char* szFilePath)
功能描述：初始化文件
输入参数：
   参数1：const char* szFilePath，文件路径
输出参数：
返回内容：成功返回TRUE,失败返回FALSE
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CParaFile::Init(const char *szFileName)
{
	SNPRINTF(m_szFilePath, MAX_PATH, "%s", szFileName);
	m_szFilePath[MAX_PATH - 1] = '\0';
    
    FILE *pFile = fopen(m_szFilePath,"rt");
    if (pFile == NULL)
    {
        TRACE(MTS_CFGSVR,
              "%s(%s)",
              S_CFG_OPEN_CFGFILE_ERR,
              m_szFilePath);
        return FALSE;
    }
    
    char szLineBuf[MAX_LINE+1];
    //循环读取每一行内容；
    while(!feof(pFile))
    {
        szLineBuf[0] = '\0';
        if (fgets(szLineBuf,MAX_LINE, pFile) == NULL)
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
        //去掉回车、换行符及前后空格
        StringLRTrim(szLineBuf);
		if (szLineBuf[0] != '\0')
		{
			//分析一行
			if (!ParseALine(szLineBuf))
			{
				//出错处理
				fclose(pFile);
				return FALSE;
			}
		}
    }
	//将最后一参数项信息加入到参数段结构列表
	if (!AddParaItemToParaSectMap())
	{
		fclose(pFile);
        return FALSE;
	}

    //关闭文件；
    fclose(pFile);
    
    return TRUE;
}

/*******************************************************************
函数名称：AddParaItemToParaSectMap()
功能描述：将一参数项信息加入到参数段结构列表
输入参数：
输出参数：
返回内容：成功返回TRUE,失败返回FALSE
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CParaFile::AddParaItemToParaSectMap()
{
	//判断m_pCurParaItem是否为NULL，不为NULL表示存在上一参数项信息，必须加入映射表；
    if (m_pCurParaItem != NULL)
    {
        //检查参数项信息的完整性：参数项信息必须指定所属段名及数据类型
		if (m_SectNameList.empty() ||                       //参数项的所属段名不能为空
			m_pCurParaItem->nItemType == PARATYPE_UNKNOWN)           //参数项数据类型必须指定
		{
			TRACE(MTS_CFGSVR,
                  "%s(%s:%s)",
                  S_CFG_PRARFILE_SECTINFO_INVALID,
                  m_szFilePath,
                  m_szCurSectName);
            
			return FALSE;
		}
		
        LIST< STRING >::iterator it = m_SectNameList.begin();
        LIST< STRING >::iterator itNext = m_SectNameList.begin();
        LIST< STRING >::iterator itEnd = m_SectNameList.end();
        for( ; it!=m_SectNameList.end(); it++)
        {
            PARASECT_MAP::iterator itSect; //add by zkq 2004-09-01
            for(itSect = m_mapParaSect.begin(); itSect != m_mapParaSect.end(); itSect++)
            {
                if(StringiCmp((*it).c_str(),(*itSect).first.c_str()))
                {
                    (*it) = (*itSect).first ; //大小写不同时只保留一个  
                    break;
                }                
            }           
            //end     
            itNext++;
            PARASECT_MAP::iterator itFind = m_mapParaSect.find( (*it) );
            //检查参数项所在段的段名列表是否已加入映射表
            if (itFind == m_mapParaSect.end())
            {
                //如未加入，则新增PARAITEM_LIST列表,并加入映射表
                PARAITEM_LIST* pItemList = new PARAITEM_LIST; 
                //如不是最后加入的参数项，则必须拷贝一份副本，再加入列表
                //否则直接加入列
                if (itNext != itEnd)
                {
                    SParaItem *pInsParaItem = new SParaItem;
                    *pInsParaItem = *m_pCurParaItem;
                    pItemList->push_back(pInsParaItem);
                }
                else
                {
                    pItemList->push_back(m_pCurParaItem);
                }
                
                m_mapParaSect.insert( PAIR< STRING, PARAITEM_LIST >((*it), 
                    *pItemList ));
                if(pItemList != NULL) //释放空间 add by zkq 2004-08-31
                {
                    delete pItemList;
                    pItemList = NULL;
                }
            }
            else
            {
                //如已加入
                PARAITEM_LIST& ItemList = (*itFind).second;
				PARAITEM_LIST::iterator itItem;
				//检查同一段中不能存在两个参数名相同的参数项
				for(itItem=ItemList.begin(); itItem != ItemList.end(); itItem++)
				{
					if (strcmp((*itItem)->szItemName, m_pCurParaItem->szItemName) == 0) 
					{
						//存在两个参数名相同的参数项在同一段中
                        TRACE(MTS_CFGSVR,
                              "%s(%s:%s:%s)",
                              S_CFG_ITEMINFO_REP_IN_ONERELA,
                              m_szFilePath,
                              m_pCurParaItem->szItemName,
                              (*it).c_str());
					    //两个参数名相同的参数项在同一段中,保留TRACE
                        if(m_pCurParaItem != NULL) //add by zkq 2004-08-31
                        {
                            delete m_pCurParaItem;
                            m_pCurParaItem = NULL;
                        }
                        m_SectNameList.clear();
					    return TRUE;
					}
				}
                if (itNext != itEnd)
                {
                    SParaItem *pInsParaItem = new SParaItem;
                    *pInsParaItem = *m_pCurParaItem;
                    ItemList.push_back(pInsParaItem);
                }
                else
                {
                    ItemList.push_back(m_pCurParaItem);
                }
            }
        }
        //已插入列表,则清空段名列表准确保存下一段名
        m_pCurParaItem = NULL;
        m_SectNameList.clear();
    }
    
	return TRUE;
}

/*******************************************************************
函数名称：ParseALine(char* szLine)
功能描述：分析一行
输入参数：
   参数1：char* szLine，一行数据(已去掉回车、换行符及前后空格)
输出参数：
返回内容：成功返回TRUE,失败返回FALSE
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CParaFile::ParseALine(const char *szLine)
{   
    //假如行第1个字符是“#”,表明此行为注释行，忽略；
    if (szLine[0] == '#') 
    {
        return TRUE;
    }
    //假如行第1个字符为“[”，继续找“]”，如果找不到，返回失败；
    if (szLine[0] == '[')
    {
        const char *pdest = strchr(szLine,']');
        
        if (pdest == NULL)
        {
            TRACE(MTS_CFGSVR,
                  "%s(%s:%s)",
                  S_CFG_PARAFILE_SECTNAME_INVALID, 
                  m_szFilePath,
                  szLine);
           
			return FALSE;
        }
        //如果找到说明此行“[]”内的是参数项名，
        char szSection[MAX_LINE+1];
        int  nLength = pdest - szLine -1;
        
        strncpy(szSection, &szLine[1], nLength);
        szSection[nLength] = '\0';
        StringLRTrim(szSection);
        nLength = strlen(szSection); 
		
        if (nLength == 0 || nLength >= MAX_ITEMNAME_LEN)
		{
			//参数项名为空或太长
            TRACE(MTS_CFGSVR,
                  "%s(%s:%s)",
                  S_CFG_PARAFILE_SECTNAME_INVALID,
                  m_szFilePath,
                  szSection);
            
			return FALSE;
		}
        strcpy(m_szCurSectName,szSection);
        
		//将上一参数项信息加入到参数段结构列表
		if (!AddParaItemToParaSectMap())
		{
			return FALSE;
		}
        		
        //分析出新的参数项,新建一参数项信息结构,并指定参数名
		m_pCurParaItem = new  SParaItem;
		strcpy(m_pCurParaItem->szItemName,szSection);

        return TRUE;	
    }
    //调用分析参数行的函数ParseParaLine()继续处理；
    return ParseParaLine(szLine);
}

/*******************************************************************
函数名称：ParseParaLine(const char *szLine)
功能描述：分析一参数项属性行
输入参数：
   参数1：const char* szLine，一行数据(已去掉回车、换行符及前后空格)
输出参数：
返回内容：成功返回TRUE,失败返回FALSE
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CParaFile::ParseParaLine(const char *szLine)
{
    //当参数信息正文第一行不是段名信息,而是参数信息时,m_pCurParaItem为空
    if (m_szCurSectName[0] == '\0')
    {
        TRACE(MTS_CFGSVR,
              "%s(%s)",
              S_CFG_PARAFILE_STYLE_INVALID,
              m_szFilePath);
        return FALSE;
    }

	//查找“=”，找不到或参数项属性名为空，返回FALSE；
    const char *pdest = strchr(szLine, '=');
    if (pdest == NULL || pdest == szLine)
    {
        TRACE(MTS_CFGSVR,
              "%s(%s:%s:%s)",
              S_CFG_PARAFILE_ITEM_INVALID,
              m_szFilePath,
              m_szCurSectName,
              szLine);
        return FALSE;
    }
    char szItem[MAX_LINE+1];
    char szValue[MAX_LINE+1];
    int  nLength = pdest - szLine;
    
    //取参数项名字符串
    strncpy(szItem, szLine, nLength);
    szItem[nLength] = '\0';
    StringLRTrim(szItem);
	if (szItem[0] == '\0')
	{
		//参数项属性名为空
        TRACE(MTS_CFGSVR,
              "%s(%s:%s:%s)",
              S_CFG_PARAFILE_ITEM_INVALID,
              m_szFilePath,
              m_szCurSectName,
              szLine);
 		return FALSE;
	}
    
    //取参数项值字符串
    strcpy(szValue, pdest+1);
    StringLRTrim(szValue);
	    
	//分析参数项的属性项及属性值
    if (strcmp(szItem,SECTLIST) == 0)
    {
        //如果是SECTLIST，表示是当前参数项的所属段名(如"段名1^段名2^段名3")，
        //建立此项的段名列表
        m_SectNameList.clear();
        char *token = strtok(szValue,"^");
        while (token != NULL)
        {
            StringLRTrim(token);
			if( strlen(token) >= MAX_SECTNAME_LEN )
			{
				//段名太长
                TRACE(MTS_CFGSVR,
                      "%s(%s:%s:%s)",
                      S_CFG_PARAFILE_RELASECT_TOOLEN,
                      m_szFilePath,
                      m_szCurSectName,
                      token);
				
				return FALSE;
			}
			else if (strlen(token) > 0)
			{
                //检查段名是否有重复，没有则加入当前段名列表
				LIST< STRING >::iterator it;
                it = std::find(m_SectNameList.begin(),m_SectNameList.end(),STRING(token));
				if (it == m_SectNameList.end())
				{
                    m_SectNameList.push_back(STRING(token));
				}
			}
            token = strtok(NULL,"^");
        }//end while
        //参数项名所属段名不能为空
		if (m_SectNameList.empty())
        {
            TRACE(MTS_CFGSVR,
                  "%s(%s:%s)",
                  S_CFG_PARAFILE_RELASECT_NULL,
                  m_szFilePath,
                  m_szCurSectName);

            return FALSE;
        }
        return TRUE;      
    }
    if (strcmp(szItem,RELATION) == 0)
    {
        //如果是SECTRELA，表示是相关段名
        strncpy(m_pCurParaItem->szParaRelaSect,szValue,MAX_SECTNAME_LEN-1);
        m_pCurParaItem->szParaRelaSect[MAX_SECTNAME_LEN-1] = '\0';
        return TRUE;
    }
    if (strcmp(szItem,TYPE) == 0)
    {
        //如果是TPYE，表示是类型: 类型由"NUM","STR","ENUM","IP"字符串表示 
        m_pCurParaItem->nItemType = GetParaDataType(szValue);
        if (m_pCurParaItem->nItemType == PARATYPE_UNKNOWN)
        {	//无效数据类型
            TRACE(MTS_CFGSVR,
                  "%s(%s:%s)",
                  S_CFG_PARAFILE_DATATYPE_INVALID,
                  m_szFilePath,
                  m_szCurSectName);
            
            return FALSE;
        }
        return TRUE;
    }
    if (strcmp(szItem,DESC) == 0)
    {
        //如果是DESC，表示是描述
        strncpy(m_pCurParaItem->szItemDesc,szValue,MAX_ITEMDESC_LEN-1);
        m_pCurParaItem->szItemDesc[MAX_ITEMDESC_LEN-1] = '\0';
        return TRUE;
    }
    if (strcmp(szItem,DEFAULT) == 0)
    {
        //如果是DEFAULT，表示是缺省值
        strncpy(m_pCurParaItem->szDefaultValue,szValue,MAX_ITEMVALUE_LEN-1);
        m_pCurParaItem->szDefaultValue[MAX_ITEMVALUE_LEN-1] = '\0';
        return TRUE;
    }
    if (strcmp(szItem,MUSTFLAG) == 0)
    {
        //如果是MUSTFLAG，表示是必配标志
        if (strcmp(szValue,"YES") == 0)
        {
            m_pCurParaItem->bIsMustCfg = TRUE;
        }
               
        return TRUE;
    }
    if (strcmp(szItem,UPDATEFLAG) == 0)
    {   //如果是UPDATEFLAG，表示是可改标志
        if (strcmp(szValue,"YES") == 0)
        {
            m_pCurParaItem->bIsCanChg = TRUE;
        }
        
        return TRUE;
    }
    
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
void CParaFile::GetAllSectName(LIST< STRING >& SectNameList)
{
    //遍历映射表m_mapParaSect，把段名全部取出来，加到SectNameList
    SectNameList.clear();
    
    PARASECT_MAP::iterator it;
    for(it=m_mapParaSect.begin(); it != m_mapParaSect.end(); it++)
    {
        SectNameList.push_back( (*it).first );
    }   
}

/*******************************************************************
函数名称：GetParaListFromSectName(const char* szSectName)
功能描述：从段名获得此段所有参数信息
输入参数：
   参数1：const char* szSectName，段名
输出参数：
返回内容：PARAITEM_LIST* ,返回指定段名的参数项信息列表指针
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
PARAITEM_LIST* CParaFile::GetParaListFromSectName(const char *szSectName)
{	
    //检查参数合法性
    if (szSectName == NULL) 
    {
        return NULL;
    }

    PARASECT_MAP::iterator it;
    //在映射表m_mapParaSect中查找段名为szSectName的映射；
    if ( (it = m_mapParaSect.find( STRING(szSectName) )) != m_mapParaSect.end() )
    {
        return &((*it).second);
    }
	return NULL;
}

/*******************************************************************
函数名称：GetParaDataType(const char* szValue)
功能描述：通过数据类型字符串获得数据类型
输入参数：
   参数1：const char* szValue，数据类型字符串
输出参数：
返回内容：PARATYPE ,返回数据类型
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
PARATYPE CParaFile::GetParaDataType(const char* szValue) const
{
    if (strcmp(szValue,"NUM") == 0)
    {   //数字类型
        return PARATYPE_NUM;
    }
    else if (strcmp(szValue,"STR") == 0)
    {   //字符串类型
        return PARATYPE_STR;
    }
    else if (strcmp(szValue,"ENUM") == 0)
    {
        //枚举类型
        return PARATYPE_ENUM;
    }
    else if (strcmp(szValue,"IP") == 0)
    {	//IP地址类型
        return PARATYPE_IP;
    }
    else if (strcmp(szValue,"DATE") == 0)
    {
        //日期类型
        return PARATYPE_DATE;
    }
    else if (strcmp(szValue,"TIME") == 0)
    {
        //时间类型
        return PARATYPE_TIME;
    }
    else if (strcmp(szValue,"DATETIME") == 0)
    {
        //日期时间类型
        return PARATYPE_DATETIME;
    }
    else
    {	
        //无效数据类型
        return PARATYPE_UNKNOWN;
    }
}
