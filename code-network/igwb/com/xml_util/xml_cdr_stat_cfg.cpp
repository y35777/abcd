#include "../include/toolbox.h"
#include "../include/frame.h"
#include "xml_cdr_stat_cfg.h"
#include "../include/export_dll.h"

extern "C" IGWB_Svc_Export ICDRStatCfg* CreateCDRStatCfg()
{
    return new CXmlCDRStatCfg();
}

//»°µ¥Í³¼ÆÄ£¿é¶¨Òå
/***********************************************************************************
                          Í³¼ÆÅäÖÃÎÄ¼þstat_cfg_x.xml¸ñÊ½
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
//¶¨Òåstat_cfg_xxx.xmlÎÄ¼þ(Í³¼ÆÅäÖÃÏî)ÖÐÊ¹ÓÃµÄXML TAG±ê¼Ç
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
º¯ÊýÃû³Æ£ºCCDRStatCfg()
¹¦ÄÜÃèÊö£º¹¹Ôìº¯Êý
ÊäÈë²ÎÊý£ºÎÞ
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£ºÎÞ
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
CXmlCDRStatCfg::CXmlCDRStatCfg()
{
    m_uAccessPoint  = 0;
    m_szFileName[0] = '\0';
}
/*******************************************************************
º¯ÊýÃû³Æ£º~CXmlCDRStatCfg()
¹¦ÄÜÃèÊö£ºÎö¹¹º¯Êý
ÊäÈë²ÎÊý£ºÎÞ
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£ºÎÞ
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
CXmlCDRStatCfg::~CXmlCDRStatCfg()
{
    
}


/*******************************************************************
º¯ÊýÃû³Æ£ºInit(UINT4 uAccessPoint)
¹¦ÄÜÃèÊö£ºÉèÖÃ²ÎÊý£¬½øÐÐ³õÊ¼»¯²Ù×÷
ÊäÈë²ÎÊý£ºUINT4 uAccessPoint,½ÓÈëµãºÅ
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£ºÎÞ
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



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

	//´ÓÅäÖÃÎÄ¼þÖÐ¶ÁÈ¡µ±Ç°½ÓÈëµãÃû³Æ
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

    //ÅäÖÃÎÄ¼þµÄÎÄ¼þÃû
	sprintf(m_szFileName, "%s/%s%d%s", szPath, "cdr_stat_", m_uAccessPoint, ".xml");
  
	//ÅÐ¶ÏÎÄ¼þÊÇ·ñ´æÔÚ
	BOOL bFileExist = (access(m_szFileName, 0) == 0);
	//³õÊ¼»¯XMLFile,ÈçÖ¸¶¨µÄXMLÎÄ¼þ²»´æÔÚ£¬ÔòÐÂ½¨Ò»¸ö½ö°üº¬Ö¸¶¨¸ù½áµãµÄXMLÎÄ¼þ
	if (!m_xmlFileCfg.InitXmlFile(m_szFileName,XMLTAG_STATCFG_ROOT))
	{
		return ERR_FAIL;
	}

	//ÈçXMLÎÄ¼þÊÇÒ»ÐÂ½¨ÇÒ²»º¬Í·ÐÅÏ¢ÎÄ¼þ£¬ÔòÐè¼ÓÉÏÍ·ÐÅÏ¢
	if (!bFileExist)
	{
		//¼ÓÈëÍ·ÐÅÏ¢
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
º¯ÊýÃû³Æ£ºGetStatItemNameList(LIST<STRING>& listStatItemName)
¹¦ÄÜÃèÊö£º»ñµÃËùÓÐÍ³¼ÆÏîµÄÏîÄ¿Ãû³Æ
ÊäÈë²ÎÊý£ºLIST<STRING>& listStatItemName,Í³¼ÆÏîÃû³ÆÁÐ±í
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£ºÎÞ
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
void CXmlCDRStatCfg::GetStatItemNameList(LIST<STRING>& listStatItemName)
{
    //»ñÈ¡XMLµÄDOMÎÄµµ£¬²¢¼ì²éÆäºÏ·¨ÐÔ
	DOMDocument *pDOMDoc = m_xmlFileCfg.GetDOMDocument();
	if (pDOMDoc == 0)
	{	
		return;
	}
	//ÅäÖÃÏî½áµã¸öÊý
	int nNodesCount;
	
	//»ñÈ¡ËùÓÐÅäÖÃÏî½áµãÁÐ±íÖ¸Õë,Èç²»´æÔÚÔò·µ»Ø
	DOMNodeList *pDOMNodeList = pDOMDoc->getElementsByTagName(X(XMLTAG_STATCFG_ITEMCFG));
	if (pDOMNodeList == 0 || (nNodesCount = pDOMNodeList->getLength()) <= 0)
	{
		return;
	}
	
	DOMElement *pNode = 0;
	for(int i=0; i<nNodesCount; i++)
	{
		//È¡µ¥¸öÅäÖÃÏî½áµã
		pNode = (DOMElement *)pDOMNodeList->item(i);
		if (pNode != 0)
		{	
			//È¡µ¥¸öÅäÖÃÏîÃû³Æ£¬²¢¼ÓÈëÁÐ±í
			const XMLCh *pValue = pNode->getAttribute(X(XMLTAG_STATCFG_ITEMNAME));	
			listStatItemName.push_back(STRING(S(pValue)));
		}
	}
}

/*******************************************************************
º¯ÊýÃû³Æ£ºGetStatItem(const char* const szStatItemName)
¹¦ÄÜÃèÊö£º»ñµÃÖ¸¶¨Í³¼ÆÏîÄ¿ÃûµÄÍ³¼ÆÅäÖÃÐÅÏ¢
ÊäÈë²ÎÊý£ºconst char* const szStatItemName,Í³¼ÆÏîÃû³Æ
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£ºÎÞ
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
SStatItemCfg* CXmlCDRStatCfg::GetStatItem(const char* const szStatItemName)
{
	//²ÎÊýºÏ·¨ÐÔ¼ì²é
    if (szStatItemName == 0 || szStatItemName[0] == '\0')
	{
		return NULL;
	}
    
	//»ñÈ¡XMLÎÄ¼þµÄDOMÎÄµµ
    DOMDocument *pDOMDoc = m_xmlFileCfg.GetDOMDocument();
	if (pDOMDoc == 0)
	{
		return NULL;
	}
    
	//ÅäÖÃÏî½áµã¸öÊý
	int nNodesCount;

	//»ñÈ¡ËùÓÐÅäÖÃÏî½áµãÁÐ±íÖ¸Õë
	DOMNodeList *pDOMNodeList = pDOMDoc->getElementsByTagName(X(XMLTAG_STATCFG_ITEMCFG));
	if (pDOMNodeList == 0 || (nNodesCount = pDOMNodeList->getLength()) <= 0)
	{
		return NULL;
	}
	
	SStatItemCfg* pStatItemCfg = NULL;
	DOMElement *pNode = 0;

	//±éÀúËùÓÐÅäÖÃÏî½áµãÁÐ±í£¬²¢²éÕÒÖ¸¶¨ÅäÖÃÃû³Æ½áµã
	for(int i=0; i<nNodesCount; i++)
	{
		//È¡µ¥¸ö½áµã
		pNode = (DOMElement *)pDOMNodeList->item(i);
		if (pNode == 0) 
		{
			continue;
		}
		
		//»ñÈ¡½áµãµÄÅäÖÃÏîÃû³Æ
		const XMLCh *pItemName = pNode->getAttribute(X(XMLTAG_STATCFG_ITEMNAME));
		if (XMLString::compareString(X(szStatItemName),pItemName) != 0)
		{
			//·ÇÖ¸¶¨ÅäÖÃÏîÃû³Æ£¬¼ÌÐø±éÀúÏÂÒ»½áµã
			continue;
		}

		//È¡½áµãµÄ×ÓÅäÖÃÏî¸öÊý
		const XMLCh *pSubItemCnt = pNode->getAttribute(X(XMLTAG_STATCFG_SUBITEMCOUT));
		int nSubItemCnt = atoi(S(pSubItemCnt));

		//Îª·µ»ØµÄÅäÖÃÏîÐÅÏ¢·ÖÅäÄÚ´æ£¬ÒÔ±£´æÅäÖÃÏî¼°Æä×ÓÏîÐÅÏ¢
		pStatItemCfg = new (nSubItemCnt)SStatItemCfg;
		sprintf(pStatItemCfg->szStatItemName, "%s", szStatItemName);
		pStatItemCfg->uStatItemCount = nSubItemCnt;
			
		//È¡×ÓÅäÖÃÏî½áµãÁÐ±íÖ¸Õë
		DOMNodeList *pChildList = pNode->getElementsByTagName(X(XMLTAG_STATCFG_SUBITEMCFG));
		if (pChildList != 0)
		{
			//DOMÎÄµµÊ÷ÖÐµÄ×ÓÅäÖÃÏî½áµã¸öÊý
			int nCount = pChildList->getLength();

	        //Èç×ÓÅäÖÃÏî½áµã¸öÊýÓëÅäÖÃÏîÖÐµÄ×ÓÅäÖÃÏî¸öÊý²»µÈ£¬ÔòÈ¡×îÐ¡Öµ
			if (nCount > nSubItemCnt)
			{
				nCount = nSubItemCnt;
			}
			DOMElement *pChildNode = 0;

			//±éÀúËùÓÐ×ÓÅäÖÃ
			for(int nIndex=0; nIndex < nCount; nIndex++)
			{
				//µÚnIndex×ÓÅäÖÃÏîÐÅÏ¢Ö¸Õë
				SStatSubItemCfg *pSubItemCfg = &(pStatItemCfg->subItemCfg[nIndex]);
				//µÚnIndex×ÓÅäÖÃÏî½áµã
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
		//ÒÑÈ¡µ½ÏàÓ¦ÅäÖÃÏîÐÅÏ¢£¬·µ»Ø
		return pStatItemCfg; 	
	}
	
	return NULL;
}

/*******************************************************************
º¯ÊýÃû³Æ£ºGetStatItem(const char* const szStatItemName, 
                      LIST<SOneStatSubItemCfg*>& subItemList)
¹¦ÄÜÃèÊö£º»ñµÃÖ¸¶¨Í³¼ÆÏîÄ¿ÃûµÄÍ³¼ÆÅäÖÃÐÅÏ¢
ÊäÈë²ÎÊý£ºconst char* const szStatItemName,Í³¼ÆÏîÃû³Æ 
		  LIST<SOneStatSubItemCfg*>& subItemList,Í³¼ÆÏîÁÐ±í
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£ºÎÞ
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
void CXmlCDRStatCfg::GetStatItem(const char* const szStatItemName, 
							  LIST<SOneStatSubItemCfg*>& subItemList)
{
	//²ÎÊýºÏ·¨ÐÔ¼ì²é
	if (szStatItemName == 0 || szStatItemName[0] == '\0')
	{
		return;
	}

	//È¡Ö¸¶¨ÅäÖÃÏîÃû³ÆµÄÅäÖÃÏîÐÅÏ¢
	SStatItemCfg* pStatItemCfg = GetStatItem(szStatItemName);
	if (pStatItemCfg == 0)
	{
		return;
	}

	//×ÓÅäÖÃÏî¸öÊý
	UINT4 nCount = pStatItemCfg->uStatItemCount;

	//Ìî³ä·µ»ØÁÐ±ísubItemList	
	SStatSubItemCfg* pTmp;
	for( UINT4 i = 0; i < nCount; i++)
	{
		//µÚi¸ö×ÓÅäÖÃÏîÐÅÏ¢
		pTmp = &(pStatItemCfg->subItemCfg[i]);

        //Éú³É×ÓÅäÖÃÏîÐÅÏ¢
		SOneStatSubItemCfg* pOneCfg = new SOneStatSubItemCfg;
		memcpy(pOneCfg->szStatItemName, pStatItemCfg->szStatItemName, 64);
		memcpy(pOneCfg->szStatSubItemName, pTmp->szStatSubItemName, 64);
		memcpy(pOneCfg->szStatSubItemCond, pTmp->szStatSubItemCond, 512);
        
		//¼ÓÈëÁÐ±í
		subItemList.push_back(pOneCfg);
	}  

	delete pStatItemCfg;
}


/*******************************************************************
º¯ÊýÃû³Æ£ºSetStatCfg(LIST<SOneStatSubItemCfg*> subItemList)
¹¦ÄÜÃèÊö£ºÉèÖÃËùÓÐÍ³¼Æ×ÓÏîµÄÅäÖÃÐÅÏ¢
ÊäÈë²ÎÊý£ºLIST<SOneStatSubItemCfg*> subItemList,Í³¼ÆÏîÁÐ±í
          ×¢£ºÁÐ±íÒÑ°´ÅäÖÃÏîÃû³ÆÅÅÐòºÃ 
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£ºÎÞ
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
int CXmlCDRStatCfg::SetStatCfg(LIST<SOneStatSubItemCfg*> subItemList)
{
    //»ñÈ¡XMLµÄDOMÎÄµµ 
    DOMDocument *pDOMDoc = m_xmlFileCfg.GetDOMDocument();
    if (pDOMDoc == 0)
	{
		return ERR_FAIL;
	}
    //ÏÈ±£´æDOMÎÄµµ£¬Ê§°ÜÔò»Ö¸´Ô­DOMÎÄµµ
	DOMDocument *pOldDOMDoc = (DOMDocument *)pDOMDoc->cloneNode(true);

	//Çå¿ÕDOMÎÄµµÄÚÈÝ
	if (!m_xmlFileCfg.ClearDOMDocument())
	{
		//Ê§°ÜÔò»Ö¸´Ô­DOMÎÄµµ
		m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
		return ERR_FAIL;
	}
	
	//ÐÂ½¨DOM¸ù½áµã
    DOMElement *pRoot = m_xmlFileCfg.AddRootElementToDOMDoc(pDOMDoc,XMLTAG_STATCFG_ROOT);
	if (pRoot == 0)
	{
		//Ê§°ÜÔò»Ö¸´Ô­DOMÎÄµµ
		m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
		return ERR_FAIL;
	}

	//ÔÚ¸ù½áµãÏÂ¼ÓÈëÐÂÉú³ÉÎÄ¼þÍ·½áµã
	DOMElement  *pHeadNode = AddHeadInfo(pRoot);
	if (pHeadNode == 0)
	{
		//Ê§°ÜÔò»Ö¸´Ô­DOMÎÄµµ
		m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
		pHeadNode->release();
		return ERR_FAIL; 
	}
    
	//ÉÏÒ»ÅäÖÃÏîÃû³Æ
	char szOldItem[256] = "";
	//ÅäÖÃÏîÊý
	int  nItemCnt = 0;
	//×ÓÅäÖÃÏîÊý
	int  nSubItemCnt = 0;   
	DOMElement *pNewNode = 0;
	char szTmp[256];
	LIST<SOneStatSubItemCfg*>::iterator it = subItemList.begin();
	
	//½«ËùÓÐÅäÖÃÏîÐÅÏ¢¼ÓÈëDOM½áµãÊ÷
	for(;it != subItemList.end(); it++)
	{
		SOneStatSubItemCfg *pCfgInfo = (*it);
		//ÅÐ¶ÏÊÇ·ñÎªÐÂÅäÖÃÏî
		if (strcmp(szOldItem, pCfgInfo->szStatItemName) != 0)
		{
			//ÕâÊÇÒ»ÏîÎ´¼ÓÈë½áµãÊ÷µÄÐÂÅäÖÃÏî	
			if (pNewNode != 0)
			{
				//ÎªÉÏÒ»ÅäÖÃÏî¼ÓÉÏ×ÓÅäÖÃÏîÊýÊôÐÔ
				sprintf(szTmp,"%d",nSubItemCnt);
				if (!m_xmlFileCfg.SetNodeAttrValue(pNewNode,XMLTAG_STATCFG_SUBITEMCOUT,szTmp))
				{
					//Ê§°ÜÔò»Ö¸´Ô­DOMÎÄµµ
		            m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
					return ERR_FAIL;
				}
			}
			//ÕâÊÇÐÂÅäÖÃÏîµÄµÚÒ»¸ö×ÓÅäÖÃÏî
			nSubItemCnt = 1;  
			//ÕâÊÇÒ»ÐÂÅäÖÃÏî£¬ËùÒÔÅäÖÃÏî¸öÊýÐè¼ÓÒ»
			++nItemCnt;
			strcpy(szOldItem,pCfgInfo->szStatItemName);
			//Éú³ÉÒ»ÐÂÅäÖÃÏîÐÅÏ¢£¬²¢¼ÓÈëDOM½áµãÊ÷
			pNewNode = m_xmlFileCfg.CreateNewElement(pDOMDoc,XMLTAG_STATCFG_ITEMCFG);
			if (pNewNode == 0)
			{
				//Ê§°ÜÔò»Ö¸´Ô­DOMÎÄµµ
		        m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
				return ERR_FAIL;
			}
			//ÎªÐÂÅäÖÃÏî¼ÓÈëÅäÖÃÏîÃûÊôÐÔ
			if (!m_xmlFileCfg.SetNodeAttrValue(pNewNode,XMLTAG_STATCFG_ITEMNAME,
				pCfgInfo->szStatItemName))
			{
				//Ê§°ÜÔò»Ö¸´Ô­DOMÎÄµµ
		        m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
				return ERR_FAIL;
			}
			//½«ÐÂÅäÖÃÏî½áµã¼Óµ½DOM¸ù½áµãÏÂ
			if (!m_xmlFileCfg.AddChildToNode(pRoot,pNewNode))
			{
				//Ê§°ÜÔò»Ö¸´Ô­DOMÎÄµµ
		        m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
				pNewNode->release();
                return ERR_FAIL;
			}
		}
		else
		{
			//ÕâÊÇÒ»ÏîÒÑ¼ÓÈëµÄÅäÖÃÏîµÄ×ÓÏî
			++nSubItemCnt;
		}
		//Éú³É×ÓÅäÖÃÏî½áµã
		DOMElement *pNewSubNode = m_xmlFileCfg.CreateNewElement(pDOMDoc,XMLTAG_STATCFG_SUBITEMCFG);
		if (pNewSubNode == 0)
		{
			return ERR_FAIL;
		}
		//×ÓÅäÖÃÏîÃû
		if (!m_xmlFileCfg.SetNodeAttrValue(pNewSubNode,XMLTAG_STATCFG_SUBITEMNAME,
			pCfgInfo->szStatSubItemName))
		{
			//Ê§°ÜÔò»Ö¸´Ô­DOMÎÄµµ
		    m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
			pNewSubNode->release();
			return ERR_FAIL;
		}
		//×ÓÅäÖÃÏîÌõ¼þ
		if (!m_xmlFileCfg.SetNodeAttrValue(pNewSubNode,XMLTAG_STATCFG_SUBITEMCOND,
			pCfgInfo->szStatSubItemCond))
		{
			//Ê§°ÜÔò»Ö¸´Ô­DOMÎÄµµ
		    m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
			pNewSubNode->release();
			return ERR_FAIL;
		}
		//ÔÚÅäÖÃÏî½áµãÏÂ¼ÓÈë×ÓÅäÖÃÏî½áµã
		if (!m_xmlFileCfg.AddChildToNode(pNewNode,pNewSubNode))
		{
			//Ê§°ÜÔò»Ö¸´Ô­DOMÎÄµµ
		    m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
			pNewSubNode->release();
			return ERR_FAIL;
		}
	}

	//¼ÓÉÏ×îºóÒ»ÅäÖÃÏîµÄÏîÊý
	if (pNewNode != 0)
	{
		sprintf(szTmp,"%d",nSubItemCnt);
		if (!m_xmlFileCfg.SetNodeAttrValue(pNewNode,XMLTAG_STATCFG_SUBITEMCOUT,szTmp))
		{
			//Ê§°ÜÔò»Ö¸´Ô­DOMÎÄµµ
		    m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
			return ERR_FAIL;
		}
	}

	//ÐÞ¸ÄÎÄ¼þÍ·½áµãµÄÅäÖÃÏîÊý
	sprintf(szTmp,"%d",nItemCnt);
	if (!m_xmlFileCfg.SetNodeAttrValue(pHeadNode,XMLTAG_STATCFG_ITEMCOUNT,szTmp))
	{
		//Ê§°ÜÔò»Ö¸´Ô­DOMÎÄµµ
		m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
		return ERR_FAIL;
	}

	//Ð´xmlÎÄ¼þ
	if (!m_xmlFileCfg.WriteXmlFile())
	{
		//Ê§°ÜÔò»Ö¸´Ô­DOMÎÄµµ
		m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
		//ÖØÐ´»ØÔ­ÎÄ¼þ
		m_xmlFileCfg.WriteXmlFile();
		return ERR_FAIL;
	}

	//³É¹¦É¾³ý±¸·ÝDOMÎÄµµ
	pOldDOMDoc->release();

	return ERR_SUCCESS;
}


/*******************************************************************
º¯ÊýÃû³Æ£ºGetAccessPoint()
¹¦ÄÜÃèÊö£º·µ»Ø½ÓÈëµãºÅ
ÊäÈë²ÎÊý£ºÎÞ
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£ºÎÞ
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
UINT4 CXmlCDRStatCfg::GetAccessPoint()
{
	return m_uAccessPoint;
}


/*******************************************************************
º¯ÊýÃû³Æ£ºGetAccessPointName()
¹¦ÄÜÃèÊö£º»ñµÃ½ÓÈëµãÃû³Æ
ÊäÈë²ÎÊý£ºÎÞ
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£ºÎÞ
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
const char* CXmlCDRStatCfg::GetAccessPointName()
{
	return m_sAPName.c_str();
}


/*******************************************************************
º¯ÊýÃû³Æ£ºDOMElement * CXmlCDRStatCfg::AddHeadInfo(DOMElement *pRootEle)
¹¦ÄÜÃèÊö£ºÔÚÍ³¼ÆÅäÖÃXMLÎÄ¼þµÄDOMÊ÷µÄ¸ù½áµã´¦¼ÓÈëÍ·ÐÅÏ¢½áµã
ÊäÈë²ÎÊý£ºDOMElement *pRootEle£¬DOMÊ÷µÄ¸ù½áµã
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£º³É¹¦¼ÓÈëÍ·ÐÅÏ¢½áµã£¬·µ»ØÍ·½áµãÖ¸Õë£»·ñÔò·µ»ØNULL
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
DOMElement * CXmlCDRStatCfg::AddHeadInfo(DOMElement *pRootEle)
{
	if (pRootEle == 0)
	{
		return NULL;
	}
	//¼ì²éDOMÎÄµµÊÇ·ñ´æÔÚ
	DOMDocument *pDOMDoc = m_xmlFileCfg.GetDOMDocument();
	if (pDOMDoc == 0)
	{
		return NULL;
	}

	//Éú³ÉÍ·½áµã
	DOMElement  *pHeadNode = m_xmlFileCfg.CreateNewElement(pDOMDoc,XMLTAG_STATCFG_HEAD);
	if (pHeadNode == 0)
	{
		return NULL;
	}
	//°æ±¾ÊôÐÔ
	if (!m_xmlFileCfg.SetNodeAttrValue(pHeadNode,XMLTAG_STATCFG_VERSION,"0x00000001"))
	{
        pHeadNode->release();
		return NULL; 
	}
	//½ÓÈëµãºÅ
	char szTmp[256];
	sprintf(szTmp,"%d",m_uAccessPoint);
	if (!m_xmlFileCfg.SetNodeAttrValue(pHeadNode,XMLTAG_STATCFG_AP, szTmp))
	{
        pHeadNode->release();
		return NULL; 
	}
	//ÅäÖÃÏîÊý
	if (!m_xmlFileCfg.SetNodeAttrValue(pHeadNode,XMLTAG_STATCFG_ITEMCOUNT,"0"))
	{
        pHeadNode->release();
		return NULL; 
	}
	//±£Áô×Ö½Ú
	if (!m_xmlFileCfg.SetNodeAttrValue(pHeadNode,XMLTAG_STATCFG_RESERVED,"00000000"))
	{
        pHeadNode->release();
		return NULL; 
	}

	//¼ÓÈëÍ·½áµãµ½¸ù½áµãÏÂ
	if (!m_xmlFileCfg.AddChildToNode(pRootEle,pHeadNode))
	{
		pHeadNode->release();
		return NULL; 
	}

	return pHeadNode;
}
