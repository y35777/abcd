#include "../include/toolbox.h"
#include "../include/frame.h"
#include "xml_cdr_stat_cfg.h"
#include "../include/export_dll.h"

extern "C" IGWB_Svc_Export ICDRStatCfg* CreateCDRStatCfg()
{
    return new CXmlCDRStatCfg();
}

//话单统计模块定义
/***********************************************************************************
                          统计配置文件stat_cfg_x.xml格式
 -----------------------------------------------------------------------------------
<?xml version="1.0" encoding="iso-8859-1"?>
<StatisticConfig>
	<StatCfgHead version="0x00000001" AccessPoint="1" StatItemCount="2" Reserved="0xFFFFFFFFFFFFFFFF"></StatCfgHead>

	<StatItemCfg StatItemName="3G" StatSubItemCount="2">
		<StatSubItemCfg StatSubItemName="3G1Name" StatSubItemCond="&amp;time &lt;= 10:20:30"> </StatSubItemCfg>
		<StatSubItemCfg StatSubItemName="3G2Name" StatSubItemCond="date &gt; 03-10-20"> </StatSubItemCfg>
	</StatItemCfg> 	

	<StatItemCfg StatItemName="WCDMA" StatSubItemCount="4">
		<StatSubItemCfg StatSubItemName="WCDMA1" StatSubItemCond="name = 'abc'" > </StatSubItemCfg>
		<StatSubItemCfg StatSubItemName="WCDMA2" StatSubItemCond="time = '12:10:30'"> </StatSubItemCfg>
		<StatSubItemCfg StatSubItemName="WCDMA3" StatSubItemCond="day = '50'"> </StatSubItemCfg>
		<StatSubItemCfg StatSubItemName="WCDMA4" StatSubItemCond="day &lt;= 30"> </StatSubItemCfg>
	</StatItemCfg> 	
</StatisticConfig>
 -----------------------------------------------------------------------------------

 ***********************************************************************************/
//定义stat_cfg_xxx.xml文件(统计配置项)中使用的XML TAG标记
#define XMLTAG_STATCFG_ROOT           "StatisticConfig"
#define XMLTAG_STATCFG_HEAD           "StatCfgHead"
#define XMLTAG_STATCFG_VERSION        "version"
#define XMLTAG_STATCFG_AP             "AccessPoint"
#define XMLTAG_STATCFG_ITEMCOUNT      "StatItemCount"
#define XMLTAG_STATCFG_RESERVED       "Reserved"
#define XMLTAG_STATCFG_ITEMCFG        "StatItemCfg"
#define XMLTAG_STATCFG_ITEMNAME       "StatItemName"
#define XMLTAG_STATCFG_SUBITEMCOUT    "StatSubItemCount"
#define XMLTAG_STATCFG_SUBITEMCFG     "StatSubItemCfg"
#define XMLTAG_STATCFG_SUBITEMNAME    "StatSubItemName"
#define XMLTAG_STATCFG_SUBITEMCOND    "StatSubItemCond"


/*******************************************************************
函数名称：CCDRStatCfg()
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
CXmlCDRStatCfg::CXmlCDRStatCfg()
{
    m_uAccessPoint  = 0;
    m_szFileName[0] = '\0';
}
/*******************************************************************
函数名称：~CXmlCDRStatCfg()
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
CXmlCDRStatCfg::~CXmlCDRStatCfg()
{
    
}


/*******************************************************************
函数名称：Init(UINT4 uAccessPoint)
功能描述：设置参数，进行初始化操作
输入参数：UINT4 uAccessPoint,接入点号
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
int CXmlCDRStatCfg::Init(UINT4 uAccessPoint)
{
    m_uAccessPoint = uAccessPoint;
	
    CINIFile iniFile(GetCfgFilePath());
    char szPath[MAX_PATH];
    iniFile.Open();
    iniFile.GetString(CFG_SEC_CDRSTAT, CFG_CDRSTAT_ROOT_DIR, 
                            CFG_CDRSTAT_ROOT_DIR_DEFAULT, szPath, MAX_PATH);
    szPath[MAX_PATH -1 ] = '\0';

	//从配置文件中读取当前接入点名称
	char szAPSec[25];
    SNPRINTF(szAPSec, 25, "%s%d", CFG_SEC_AP, m_uAccessPoint);
    szAPSec[24] = '\0';

	char szAPName[255];
    iniFile.GetString(szAPSec, CFG_AP_NAME, szAPSec, szAPName, 255);
	m_sAPName = szAPName;

	if (!CreateDeepDir(szPath))
	{
		return ERR_FAIL;
	}

    //配置文件的文件名
	sprintf(m_szFileName, "%s/%s%d%s", szPath, "cdr_stat_", m_uAccessPoint, ".xml");
  
	//判断文件是否存在
	BOOL bFileExist = (access(m_szFileName, 0) == 0);
	//初始化XMLFile,如指定的XML文件不存在，则新建一个仅包含指定根结点的XML文件
	if (!m_xmlFileCfg.InitXmlFile(m_szFileName,XMLTAG_STATCFG_ROOT))
	{
		return ERR_FAIL;
	}

	//如XML文件是一新建且不含头信息文件，则需加上头信息
	if (!bFileExist)
	{
		//加入头信息
		DOMDocument *pDOMDoc = m_xmlFileCfg.GetDOMDocument();
		if (pDOMDoc == 0)
		{
			return ERR_FAIL;
		}
		if (!AddHeadInfo(pDOMDoc->getDocumentElement()))
		{
			return ERR_FAIL;
		}
	}
	 	
	return ERR_SUCCESS;
}



/*******************************************************************
函数名称：GetStatItemNameList(LIST<STRING>& listStatItemName)
功能描述：获得所有统计项的项目名称
输入参数：LIST<STRING>& listStatItemName,统计项名称列表
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CXmlCDRStatCfg::GetStatItemNameList(LIST<STRING>& listStatItemName)
{
    //获取XML的DOM文档，并检查其合法性
	DOMDocument *pDOMDoc = m_xmlFileCfg.GetDOMDocument();
	if (pDOMDoc == 0)
	{	
		return;
	}
	//配置项结点个数
	int nNodesCount;
	
	//获取所有配置项结点列表指针,如不存在则返回
	DOMNodeList *pDOMNodeList = pDOMDoc->getElementsByTagName(X(XMLTAG_STATCFG_ITEMCFG));
	if (pDOMNodeList == 0 || (nNodesCount = pDOMNodeList->getLength()) <= 0)
	{
		return;
	}
	
	DOMElement *pNode = 0;
	for(int i=0; i<nNodesCount; i++)
	{
		//取单个配置项结点
		pNode = (DOMElement *)pDOMNodeList->item(i);
		if (pNode != 0)
		{	
			//取单个配置项名称，并加入列表
			const XMLCh *pValue = pNode->getAttribute(X(XMLTAG_STATCFG_ITEMNAME));	
			listStatItemName.push_back(STRING(S(pValue)));
		}
	}
}

/*******************************************************************
函数名称：GetStatItem(const char* const szStatItemName)
功能描述：获得指定统计项目名的统计配置信息
输入参数：const char* const szStatItemName,统计项名称
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
SStatItemCfg* CXmlCDRStatCfg::GetStatItem(const char* const szStatItemName)
{
	//参数合法性检查
    if (szStatItemName == 0 || szStatItemName[0] == '\0')
	{
		return NULL;
	}
    
	//获取XML文件的DOM文档
    DOMDocument *pDOMDoc = m_xmlFileCfg.GetDOMDocument();
	if (pDOMDoc == 0)
	{
		return NULL;
	}
    
	//配置项结点个数
	int nNodesCount;

	//获取所有配置项结点列表指针
	DOMNodeList *pDOMNodeList = pDOMDoc->getElementsByTagName(X(XMLTAG_STATCFG_ITEMCFG));
	if (pDOMNodeList == 0 || (nNodesCount = pDOMNodeList->getLength()) <= 0)
	{
		return NULL;
	}
	
	SStatItemCfg* pStatItemCfg = NULL;
	DOMElement *pNode = 0;

	//遍历所有配置项结点列表，并查找指定配置名称结点
	for(int i=0; i<nNodesCount; i++)
	{
		//取单个结点
		pNode = (DOMElement *)pDOMNodeList->item(i);
		if (pNode == 0) 
		{
			continue;
		}
		
		//获取结点的配置项名称
		const XMLCh *pItemName = pNode->getAttribute(X(XMLTAG_STATCFG_ITEMNAME));
		if (XMLString::compareString(X(szStatItemName),pItemName) != 0)
		{
			//非指定配置项名称，继续遍历下一结点
			continue;
		}

		//取结点的子配置项个数
		const XMLCh *pSubItemCnt = pNode->getAttribute(X(XMLTAG_STATCFG_SUBITEMCOUT));
		int nSubItemCnt = atoi(S(pSubItemCnt));

		//为返回的配置项信息分配内存，以保存配置项及其子项信息
		pStatItemCfg = new (nSubItemCnt)SStatItemCfg;
		sprintf(pStatItemCfg->szStatItemName, "%s", szStatItemName);
		pStatItemCfg->uStatItemCount = nSubItemCnt;
			
		//取子配置项结点列表指针
		DOMNodeList *pChildList = pNode->getElementsByTagName(X(XMLTAG_STATCFG_SUBITEMCFG));
		if (pChildList != 0)
		{
			//DOM文档树中的子配置项结点个数
			int nCount = pChildList->getLength();

	        //如子配置项结点个数与配置项中的子配置项个数不等，则取最小值
			if (nCount > nSubItemCnt)
			{
				nCount = nSubItemCnt;
			}
			DOMElement *pChildNode = 0;

			//遍历所有子配置
			for(int nIndex=0; nIndex < nCount; nIndex++)
			{
				//第nIndex子配置项信息指针
				SStatSubItemCfg *pSubItemCfg = &(pStatItemCfg->subItemCfg[nIndex]);
				//第nIndex子配置项结点
				pChildNode = (DOMElement *)pChildList->item(nIndex);
				if (pChildNode != 0)
				{
					const XMLCh * pSubItemName = pChildNode->getAttribute(X(XMLTAG_STATCFG_SUBITEMNAME));
					const XMLCh * pSubCond = pChildNode->getAttribute(X(XMLTAG_STATCFG_SUBITEMCOND));
					strncpy(pSubItemCfg->szStatSubItemName,S(pSubItemName),64);
					strncpy(pSubItemCfg->szStatSubItemCond,S(pSubCond),512);
				}
			}
		}
		//已取到相应配置项信息，返回
		return pStatItemCfg; 	
	}
	
	return NULL;
}

/*******************************************************************
函数名称：GetStatItem(const char* const szStatItemName, 
                      LIST<SOneStatSubItemCfg*>& subItemList)
功能描述：获得指定统计项目名的统计配置信息
输入参数：const char* const szStatItemName,统计项名称 
		  LIST<SOneStatSubItemCfg*>& subItemList,统计项列表
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CXmlCDRStatCfg::GetStatItem(const char* const szStatItemName, 
							  LIST<SOneStatSubItemCfg*>& subItemList)
{
	//参数合法性检查
	if (szStatItemName == 0 || szStatItemName[0] == '\0')
	{
		return;
	}

	//取指定配置项名称的配置项信息
	SStatItemCfg* pStatItemCfg = GetStatItem(szStatItemName);
	if (pStatItemCfg == 0)
	{
		return;
	}

	//子配置项个数
	UINT4 nCount = pStatItemCfg->uStatItemCount;

	//填充返回列表subItemList	
	SStatSubItemCfg* pTmp;
	for( UINT4 i = 0; i < nCount; i++)
	{
		//第i个子配置项信息
		pTmp = &(pStatItemCfg->subItemCfg[i]);

        //生成子配置项信息
		SOneStatSubItemCfg* pOneCfg = new SOneStatSubItemCfg;
		memcpy(pOneCfg->szStatItemName, pStatItemCfg->szStatItemName, 64);
		memcpy(pOneCfg->szStatSubItemName, pTmp->szStatSubItemName, 64);
		memcpy(pOneCfg->szStatSubItemCond, pTmp->szStatSubItemCond, 512);
        
		//加入列表
		subItemList.push_back(pOneCfg);
	}  

	delete pStatItemCfg;
}


/*******************************************************************
函数名称：SetStatCfg(LIST<SOneStatSubItemCfg*> subItemList)
功能描述：设置所有统计子项的配置信息
输入参数：LIST<SOneStatSubItemCfg*> subItemList,统计项列表
          注：列表已按配置项名称排序好 
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
int CXmlCDRStatCfg::SetStatCfg(LIST<SOneStatSubItemCfg*> subItemList)
{
    //获取XML的DOM文档 
    DOMDocument *pDOMDoc = m_xmlFileCfg.GetDOMDocument();
    if (pDOMDoc == 0)
	{
		return ERR_FAIL;
	}
    //先保存DOM文档，失败则恢复原DOM文档
	DOMDocument *pOldDOMDoc = (DOMDocument *)pDOMDoc->cloneNode(true);

	//清空DOM文档内容
	if (!m_xmlFileCfg.ClearDOMDocument())
	{
		//失败则恢复原DOM文档
		m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
		return ERR_FAIL;
	}
	
	//新建DOM根结点
    DOMElement *pRoot = m_xmlFileCfg.AddRootElementToDOMDoc(pDOMDoc,XMLTAG_STATCFG_ROOT);
	if (pRoot == 0)
	{
		//失败则恢复原DOM文档
		m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
		return ERR_FAIL;
	}

	//在根结点下加入新生成文件头结点
	DOMElement  *pHeadNode = AddHeadInfo(pRoot);
	if (pHeadNode == 0)
	{
		//失败则恢复原DOM文档
		m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
		pHeadNode->release();
		return ERR_FAIL; 
	}
    
	//上一配置项名称
	char szOldItem[256] = "";
	//配置项数
	int  nItemCnt = 0;
	//子配置项数
	int  nSubItemCnt = 0;   
	DOMElement *pNewNode = 0;
	char szTmp[256];
	LIST<SOneStatSubItemCfg*>::iterator it = subItemList.begin();
	
	//将所有配置项信息加入DOM结点树
	for(;it != subItemList.end(); it++)
	{
		SOneStatSubItemCfg *pCfgInfo = (*it);
		//判断是否为新配置项
		if (strcmp(szOldItem, pCfgInfo->szStatItemName) != 0)
		{
			//这是一项未加入结点树的新配置项	
			if (pNewNode != 0)
			{
				//为上一配置项加上子配置项数属性
				sprintf(szTmp,"%d",nSubItemCnt);
				if (!m_xmlFileCfg.SetNodeAttrValue(pNewNode,XMLTAG_STATCFG_SUBITEMCOUT,szTmp))
				{
					//失败则恢复原DOM文档
		            m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
					return ERR_FAIL;
				}
			}
			//这是新配置项的第一个子配置项
			nSubItemCnt = 1;  
			//这是一新配置项，所以配置项个数需加一
			++nItemCnt;
			strcpy(szOldItem,pCfgInfo->szStatItemName);
			//生成一新配置项信息，并加入DOM结点树
			pNewNode = m_xmlFileCfg.CreateNewElement(pDOMDoc,XMLTAG_STATCFG_ITEMCFG);
			if (pNewNode == 0)
			{
				//失败则恢复原DOM文档
		        m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
				return ERR_FAIL;
			}
			//为新配置项加入配置项名属性
			if (!m_xmlFileCfg.SetNodeAttrValue(pNewNode,XMLTAG_STATCFG_ITEMNAME,
				pCfgInfo->szStatItemName))
			{
				//失败则恢复原DOM文档
		        m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
				return ERR_FAIL;
			}
			//将新配置项结点加到DOM根结点下
			if (!m_xmlFileCfg.AddChildToNode(pRoot,pNewNode))
			{
				//失败则恢复原DOM文档
		        m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
				pNewNode->release();
                return ERR_FAIL;
			}
		}
		else
		{
			//这是一项已加入的配置项的子项
			++nSubItemCnt;
		}
		//生成子配置项结点
		DOMElement *pNewSubNode = m_xmlFileCfg.CreateNewElement(pDOMDoc,XMLTAG_STATCFG_SUBITEMCFG);
		if (pNewSubNode == 0)
		{
			return ERR_FAIL;
		}
		//子配置项名
		if (!m_xmlFileCfg.SetNodeAttrValue(pNewSubNode,XMLTAG_STATCFG_SUBITEMNAME,
			pCfgInfo->szStatSubItemName))
		{
			//失败则恢复原DOM文档
		    m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
			pNewSubNode->release();
			return ERR_FAIL;
		}
		//子配置项条件
		if (!m_xmlFileCfg.SetNodeAttrValue(pNewSubNode,XMLTAG_STATCFG_SUBITEMCOND,
			pCfgInfo->szStatSubItemCond))
		{
			//失败则恢复原DOM文档
		    m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
			pNewSubNode->release();
			return ERR_FAIL;
		}
		//在配置项结点下加入子配置项结点
		if (!m_xmlFileCfg.AddChildToNode(pNewNode,pNewSubNode))
		{
			//失败则恢复原DOM文档
		    m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
			pNewSubNode->release();
			return ERR_FAIL;
		}
	}

	//加上最后一配置项的项数
	if (pNewNode != 0)
	{
		sprintf(szTmp,"%d",nSubItemCnt);
		if (!m_xmlFileCfg.SetNodeAttrValue(pNewNode,XMLTAG_STATCFG_SUBITEMCOUT,szTmp))
		{
			//失败则恢复原DOM文档
		    m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
			return ERR_FAIL;
		}
	}

	//修改文件头结点的配置项数
	sprintf(szTmp,"%d",nItemCnt);
	if (!m_xmlFileCfg.SetNodeAttrValue(pHeadNode,XMLTAG_STATCFG_ITEMCOUNT,szTmp))
	{
		//失败则恢复原DOM文档
		m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
		return ERR_FAIL;
	}

	//写xml文件
	if (!m_xmlFileCfg.WriteXmlFile())
	{
		//失败则恢复原DOM文档
		m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
		//重写回原文件
		m_xmlFileCfg.WriteXmlFile();
		return ERR_FAIL;
	}

	//成功删除备份DOM文档
	pOldDOMDoc->release();

	return ERR_SUCCESS;
}


/*******************************************************************
函数名称：GetAccessPoint()
功能描述：返回接入点号
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
UINT4 CXmlCDRStatCfg::GetAccessPoint()
{
	return m_uAccessPoint;
}


/*******************************************************************
函数名称：GetAccessPointName()
功能描述：获得接入点名称
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
const char* CXmlCDRStatCfg::GetAccessPointName()
{
	return m_sAPName.c_str();
}


/*******************************************************************
函数名称：DOMElement * CXmlCDRStatCfg::AddHeadInfo(DOMElement *pRootEle)
功能描述：在统计配置XML文件的DOM树的根结点处加入头信息结点
输入参数：DOMElement *pRootEle，DOM树的根结点
输出参数：无
返回内容：成功加入头信息结点，返回头结点指针；否则返回NULL
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
DOMElement * CXmlCDRStatCfg::AddHeadInfo(DOMElement *pRootEle)
{
	if (pRootEle == 0)
	{
		return NULL;
	}
	//检查DOM文档是否存在
	DOMDocument *pDOMDoc = m_xmlFileCfg.GetDOMDocument();
	if (pDOMDoc == 0)
	{
		return NULL;
	}

	//生成头结点
	DOMElement  *pHeadNode = m_xmlFileCfg.CreateNewElement(pDOMDoc,XMLTAG_STATCFG_HEAD);
	if (pHeadNode == 0)
	{
		return NULL;
	}
	//版本属性
	if (!m_xmlFileCfg.SetNodeAttrValue(pHeadNode,XMLTAG_STATCFG_VERSION,"0x00000001"))
	{
        pHeadNode->release();
		return NULL; 
	}
	//接入点号
	char szTmp[256];
	sprintf(szTmp,"%d",m_uAccessPoint);
	if (!m_xmlFileCfg.SetNodeAttrValue(pHeadNode,XMLTAG_STATCFG_AP, szTmp))
	{
        pHeadNode->release();
		return NULL; 
	}
	//配置项数
	if (!m_xmlFileCfg.SetNodeAttrValue(pHeadNode,XMLTAG_STATCFG_ITEMCOUNT,"0"))
	{
        pHeadNode->release();
		return NULL; 
	}
	//保留字节
	if (!m_xmlFileCfg.SetNodeAttrValue(pHeadNode,XMLTAG_STATCFG_RESERVED,"00000000"))
	{
        pHeadNode->release();
		return NULL; 
	}

	//加入头结点到根结点下
	if (!m_xmlFileCfg.AddChildToNode(pRootEle,pHeadNode))
	{
		pHeadNode->release();
		return NULL; 
	}

	return pHeadNode;
}
