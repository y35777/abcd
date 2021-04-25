#include "../include/toolbox.h"
#include "../include/frame.h"
#include "xml_cdr_stat_rst.h"
#include "../include/export_dll.h"

extern "C" IGWB_Svc_Export ICDRStatRst* CreateCDRStatRst()
{
    return new CXmlCDRStatRst();
}

/***********************************************************************************
                          Í³¼Æ½á¹ûÎÄ¼þstat_rst_x.xml¸ñÊ½
 -----------------------------------------------------------------------------------
<?xml version="0x00000001" encoding="ISO8859-1" standalone="no" ?>
<CDRStatRst>
  <StatRstHead AccessPoint="16" Reserved="00000000" StatEndTime="2003-12-05" StatExpCount="4" version="0x00000001">
	<StatExpIndexMsg InUsed="1" StatExp="express1" StatExpIndex="1"/>
	<StatExpIndexMsg InUsed="1" StatExp="express2" StatExpIndex="2"/>
	<StatExpIndexMsg InUsed="1" StatExp="express3" StatExpIndex="3"/>
	<StatExpIndexMsg InUsed="1" StatExp="xyz > 2003-01-01 12:00" StatExpIndex="4"/>
  </StatRstHead>

  <StatRstContent ItemStatEndTime="2003-12-05" StatSubItemCount="2">
	<OneStatExpRst CDRCount="4" ItemStatExpIndex="1" ValueTotal="5"/>
	<OneStatExpRst CDRCount="7" ItemStatExpIndex="2" ValueTotal="8"/>
  </StatRstContent>
</CDRStatRst>
 -----------------------------------------------------------------------------------
************************************************************************************/
//¶¨Òåstat_rst_xxx.xmlÎÄ¼þ(Í³¼Æ½á¹û)ÖÐÊ¹ÓÃµÄXML TAG±ê¼Ç
#define XMLTAG_STATRST_ROOT             "CDRStatRst"
#define XMLTAG_STATRST_HEAD             "StatRstHead"
#define XMLTAG_STATRST_ENDTIME          "StatEndTime"
#define XMLTAG_STATRST_RESERVED         "Reserved"
#define XMLTAG_STATRST_EXPCOUNT         "StatExpCount"
#define XMLTAG_STATRST_VERSION          "version"
#define XMLTAG_STATRST_AP               "AccessPoint"
#define XMLTAG_STATRST_EXPINDEXMSG      "StatExpIndexMsg"
#define XMLTAG_STATRST_EXP              "StatExp"
#define XMLTAG_STATRST_INURSED          "InUsed"
#define XMLTAG_STATRST_EXPINDEX         "StatExpIndex"
#define XMLTAG_STATRST_CONTENT          "StatRstContent"
#define XMLTAG_STATRST_STATENDTIME      "ItemStatEndTime"
#define XMLTAG_STATRST_SUBITEMCOUT      "StatSubItemCount"
#define XMLTAG_STATRST_ONESTATEXPRST    "OneStatExpRst"
#define XMLTAG_STATRST_ITEMSTATEXPINDEX "ItemStatExpIndex"
#define XMLTAG_STATRST_CDRCOUNT         "CDRCount"
#define XMLTAG_STATRST_VALUETOTAL       "ValueTotal"

/*******************************************************************
º¯ÊýÃû³Æ£ºCCDRStatRst()
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
CXmlCDRStatRst::CXmlCDRStatRst()
{
	m_uAccessPoint  = 0;
    m_szFileName[0] = '\0';    
    m_pDoc = NULL;
}

/*******************************************************************
º¯ÊýÃû³Æ£º~CXmlCDRStatRst()
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
CXmlCDRStatRst::~CXmlCDRStatRst()
{
}

/*******************************************************************
º¯ÊýÃû³Æ£ºInit(UINT4 uAccessPoint)
¹¦ÄÜÃèÊö£ºÉèÖÃ²ÎÊý£¬½øÐÐ³õÊ¼»¯²Ù×÷
ÊäÈë²ÎÊý£ºUINT4 uAccessPoint,½ÓÈëµãºÅ
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£º³É¹¦·µ»ØERR_SUCCESS,·ñÔò·µ»ØERR_FAIL
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
int CXmlCDRStatRst::Init(UINT4 uAccessPoint)
{
	m_uAccessPoint = uAccessPoint;

    CINIFile iniFile(GetCfgFilePath());
    char szPath[MAX_PATH];
    iniFile.Open();
    iniFile.GetString(CFG_SEC_CDRSTAT, CFG_CDRSTAT_ROOT_DIR, 
                            CFG_CDRSTAT_ROOT_DIR_DEFAULT, szPath, MAX_PATH);

    szPath[MAX_PATH -1] = '\0';

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
    sprintf(m_szFileName, "%s/%s%d%s", szPath, "stat_rst_", m_uAccessPoint, ".xml");

	//ÅÐ¶ÏÎÄ¼þÊÇ·ñ´æÔÚ
	BOOL bFileExist = (access(m_szFileName, 0) == 0);

	//³õÊ¼»¯XMLFile,ÈçÖ¸¶¨µÄXMLÎÄ¼þ²»´æÔÚ£¬ÔòÐÂ½¨Ò»¸öÄÚÈÝÎª¿ÕÇÒ²»º¬Í·ÐÅÏ¢µÄXMLÎÄ¼þ
	if (!m_xmlFileRst.InitXmlFile(m_szFileName, XMLTAG_STATRST_ROOT))
	{
		return ERR_FAIL;
	}

	//XMLÎÄ¼þÊÇÒ»ÐÂ½¨ÇÒ²»º¬Í·ÐÅÏ¢ÎÄ¼þ
	if (!bFileExist)
	{
		DOMDocument *pDOMDoc = m_xmlFileRst.GetDOMDocument();
		if (pDOMDoc == 0)
		{
            FileDel(m_szFileName);
			return ERR_FAIL;
		}

		//¼ÓÈëÍ·ÐÅÏ¢
		if (!AddHeadInfo(pDOMDoc->getDocumentElement()))
		{
            //Ìí¼ÓÍ·ÐÅÏ¢Ê§°ÜºóÉ¾³ýÎÄ¼þ
            FileDel(m_szFileName);
			return ERR_FAIL;
		}
	}

	return ERR_SUCCESS;
}


/*******************************************************************
º¯ÊýÃû³Æ£ºGetStatRstHead(SStatRstHead& statRstHead)
¹¦ÄÜÃèÊö£º»ñµÃÍ³¼Æ½á¹ûÎÄ¼þÍ·ÐÅÏ¢
ÊäÈë²ÎÊý£ºÎÞ
Êä³ö²ÎÊý£ºSStatRstHead& statRstHead,Í³¼Æ½á¹û½á¹¹Í·ÐÅÏ¢
·µ»ØÄÚÈÝ£ºÎÞ
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
void CXmlCDRStatRst::GetStatRstHead(SStatRstHead& statRstHead)
{
    DOMElement* pEleHead = GetHeadElement();
    if (!pEleHead)
    {
        return;
    }

    const XMLCh* chr = NULL;

    //°æ±¾
    chr = m_xmlFileRst.GetNodeAttrValue(pEleHead, XMLTAG_STATRST_VERSION);
    statRstHead.uVersion = atoi(S(chr));

    //½ÓÈëµã
    chr = m_xmlFileRst.GetNodeAttrValue(pEleHead, XMLTAG_STATRST_AP);
    statRstHead.uAccessPoint = atoi(S(chr));

    //Í³¼ÆÊ±¼ä        
    chr = m_xmlFileRst.GetNodeAttrValue(pEleHead, XMLTAG_STATRST_ENDTIME);
    const char* pszTime = XMLString::transcode(chr);
    statRstHead.tStatEndTime = ConvertTimeToTimeT_YMD(pszTime);

    //±£Áô×Ö½Ú
    chr = m_xmlFileRst.GetNodeAttrValue(pEleHead, XMLTAG_STATRST_RESERVED);
    strncpy((char*)statRstHead.yReserved, S(chr), 8);
}


/*******************************************************************
º¯ÊýÃû³Æ£ºGetStatExpList(SStatExpIndexList& statExpIndexList)
¹¦ÄÜÃèÊö£º»ñµÃËùÓÐÍ³¼ÆÌõ¼þ±í´ïÊ½
ÊäÈë²ÎÊý£ºÎÞ
Êä³ö²ÎÊý£ºSStatExpIndexList& statExpIndexList,ËùÓÐÍ³¼ÆÌõ¼þ±í´ïÊ½
·µ»ØÄÚÈÝ£ºÎÞ
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
SStatExpIndexList* CXmlCDRStatRst::GetStatExpList(void)
{    
	SStatExpIndexList* pStatExpIndexList = NULL;

    //»ñÈ¡XMLÎÄ¼þÖÐµÄÍ·ÐÅÏ¢
    DOMElement* pEleHead = GetHeadElement();
    if (!pEleHead)
    {
        return NULL;
    }

    //»ñÈ¡Ìõ¼þ±í´ïÊ½¸öÊý
    const XMLCh* chr = m_xmlFileRst.GetNodeAttrValue(pEleHead, XMLTAG_STATRST_EXPCOUNT);
    int nCount = atoi(S(chr));
    if (nCount == 0)
    {
        //¸öÊýÎª0
		pStatExpIndexList = new SStatExpIndexList;
		pStatExpIndexList->uStatExpCount = 0;
		return pStatExpIndexList;
    }
    else
    {
        //·ÖÅäÖ¸¶¨¸öÊý´óÐ¡µÄÄÚ´æ
		pStatExpIndexList = new (nCount)SStatExpIndexList;
    }

    //ÉèÖÃÌõ¼þ±í´ïÊ½¸öÊý
	pStatExpIndexList->uStatExpCount = nCount;

    //»ñÈ¡XMLÖÐÌõ¼þ±í´ïÊ½¶ÔÏóµÄ½ÚµãÁÐ±í
	DOMNodeList* pNodeList = m_xmlFileRst.GetChildListByTagName(pEleHead, XMLTAG_STATRST_EXPINDEXMSG);
    DOMElement* pEle = NULL;
    if (pNodeList)
    {
        int nLoop = min(nCount, pNodeList->getLength());
        //Ñ­»·¸³Öµ
        for (int i = 0; i < nLoop; i++)
        {
            pEle = (DOMElement*)pNodeList->item(i);
            if (!pEle)
            {
                continue;
            }

            //±í´ïÊ½
            chr = m_xmlFileRst.GetNodeAttrValue(pEle, XMLTAG_STATRST_EXP);
            strncpy(pStatExpIndexList->expIndex[i].szStatExp, S(chr), 512);

            //ÊÇ·ñÔÚÓÃ
            chr = m_xmlFileRst.GetNodeAttrValue(pEle, XMLTAG_STATRST_INURSED);
            pStatExpIndexList->expIndex[i].bInUsed = (BOOL)(atoi(S(chr)));

            //ÐòºÅ
            chr = m_xmlFileRst.GetNodeAttrValue(pEle, XMLTAG_STATRST_EXPINDEX);
            pStatExpIndexList->expIndex[i].nStatExpIndex = atoi(S(chr));
        }
    }

	return pStatExpIndexList; 
}


/*******************************************************************
º¯ÊýÃû³Æ£ºGetStatExpRst(const char* const szStatExpress, time_t tBeginTime, 
						time_t tEndTime, UINT4& nCDRCount, UINT4& nValueTotal)
¹¦ÄÜÃèÊö£º»ñµÃÖ¸¶¨Ê±¼ä·¶Î§ÄÚÍ³¼ÆÌõ¼þ±í´ïÊ½µÄ½á¹û
ÊäÈë²ÎÊý£ºconst char* const szStatExpress,±í´ïÊ½ 
		  time_t tBeginTime, ¿ªÊ¼Ê±¼ä
		  time_t tEndTime, ½áÊøÊ±¼ä
Êä³ö²ÎÊý£ºUINT4& nCDRCount, »°µ¥ÕÅÊý
          UINT4& nValueTotal,ÀÛ¼ÓÖµ
·µ»ØÄÚÈÝ£º³É¹¦·µ»ØERR_SUCCESS,·ñÔò·µ»ØERR_FAIL
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
int CXmlCDRStatRst::GetStatExpRst(const char* const szStatExpress, time_t tBeginTime, 
						time_t tEndTime, UINT4& nCDRCount, UINT4& nValueTotal)
{
    //¶¨ÒåÖ¸¶¨±í´ïÊ½¶ÔÓ¦µÄÐòºÅ
	UINT4 nCurrentIndex = 0;
	if (!FindStatExpress((char*)szStatExpress, nCurrentIndex))
	{
		return ERR_FAIL;
	}

    //»ñÈ¡XMLÎÄ¼þÄÚÈÝ¶ÔÏóµÄ½ÚµãÁÐ±í
    DOMNodeList* pNodeList = GetContentList();
    if (!pNodeList)
	{
		return ERR_FAIL;
	}

    int nListCount = pNodeList->getLength();
    DOMNode* pNode = NULL;
    UINT4 nEndTime = 0;
    const XMLCh* chr = NULL;
    const char* pszTime = NULL;

    //Ñ­»·Ã¿Ò»¸ö½á¹ûÁÐ±í¶ÔÏó
    for (int i = 0; i < nListCount; i++)
	{
        pNode = pNodeList->item(i);
        if (!pNode)
        {
            continue;
        }

        chr = m_xmlFileRst.GetNodeAttrValue((DOMElement*)pNode, XMLTAG_STATRST_STATENDTIME);
        pszTime = XMLString::transcode(chr);
        nEndTime = ConvertTimeToTimeT_YMD(pszTime);

        //ÅÐ¶Ï¸Ã½ÚµãµÄÊ±¼äÊÇ·ñÏà·û
		if (nEndTime >= tBeginTime && nEndTime <= tEndTime)
		{
            //·ûºÏÌõ¼þµÄ½Úµã¶ÔÏó£¬ÀÛ¼ÓÊý¾Ý
			FindStatRst(pNode, nCurrentIndex, nCDRCount, nValueTotal);
		}
	}

	return ERR_SUCCESS;
}


/*******************************************************************
º¯ÊýÃû³Æ£ºAddStatExpress(LIST<char* >& StatExpList)
¹¦ÄÜÃèÊö£ºÌí¼ÓÍ³¼ÆÌõ¼þ±í´ïÊ½
ÊäÈë²ÎÊý£ºLIST<char* >& StatExpList,±í´ïÊ½ÁÐ±í
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£º³É¹¦·µ»ØERR_SUCCESS,·ñÔò·µ»ØERR_FAIL
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
int CXmlCDRStatRst::AddStatExpress(LIST<char* >& StatExpList)
{
	DOMDocument* pDoc = m_xmlFileRst.GetDOMDocument();
    if (!pDoc)
    {
        return ERR_FAIL;
    }
    m_pDoc = (DOMDocument*)pDoc->cloneNode(true);

    //»ñÈ¡XMLÎÄ¼þÍ·ÐÅÏ¢¶ÔÏó
    DOMElement* pEleHead = GetHeadElement();
    if (!pEleHead)
    {
        m_pDoc->release();
        return ERR_FAIL;
    }
    
    //Ä¿Ç°Ìõ¼þ±í´ïÊ½¸öÊý
    UINT4 nStatExpCount = 0;
    //×îºóÒ»¸öË÷ÒýºÅ£¬ÓÃÓÚË÷ÒýÔö¼Ó
    UINT4 nLastExpMsgIndex = 0;
    UINT4 nTemp = 0;
    char chr[64] ;
    
    //»ñÈ¡Í·ÐÅÏ¢ÖÐÄ¿Ç°Ìõ¼þ±í´ïÊ½¶ÔÏóÁÐ±í
    DOMNodeList* pNodeList = m_xmlFileRst.GetChildListByTagName(pEleHead, XMLTAG_STATRST_EXPINDEXMSG); 
    if (pNodeList)
    {
        //´æÔÚ±í´ïÊ½£¬°ÑËùÓÐInUsedÖÃÎª0
        nStatExpCount = pNodeList->getLength();
        DOMNode* pNode = NULL;
        for (int i = 0; i < nStatExpCount; i++)
        {
            pNode = pNodeList->item(i);
            if (!pNode)
            {
                continue;
            }
            
            //ÖÃÎª0
            if (!m_xmlFileRst.SetNodeAttrValue((DOMElement*)pNode, XMLTAG_STATRST_INURSED, "0"))
            {
                RollBack();
                return ERR_FAIL;
            }
                //×îºóÒ»¸öÒ»¸ö±í´ïÊ½µÄË÷ÒýºÅ
            nTemp = atoi(S(m_xmlFileRst.GetNodeAttrValue((DOMElement*)pNode, XMLTAG_STATRST_EXPINDEX)));
            nLastExpMsgIndex = max(nLastExpMsgIndex, nTemp);
        }
    }
    
    UINT4 nIndex = 0;
    DOMNode* pNode = NULL;
    char chrExp[512];
    
    LIST<char* >::iterator it = StatExpList.begin();
    while(it != StatExpList.end())
    {
        strcpy(chrExp, *it);
        chrExp[511] = '\0';
        pNode = FindStatExpress(*it, nIndex);
        if (pNode)
        {
            //¶ÔÓÚÔ­À´´æÔÚÕÒµ½µÄ£¬ÐÞ¸ÄbInUsedÎª1
            if (!m_xmlFileRst.SetNodeAttrValue((DOMElement*)pNode, XMLTAG_STATRST_INURSED, "1"))
            {
                RollBack();
                return ERR_FAIL;
            }
        }
        else
        {
            //¶ÔÓÚÔ­À´²»´æÔÚµÄ£¬Ôö¼ÓÒ»¸ö            
            DOMElement* pElement = m_xmlFileRst.CreateNewElement(pDoc, XMLTAG_STATRST_EXPINDEXMSG);
            if (!pElement)
            {
                RollBack();
                return ERR_FAIL;
            }
            
            //±í´ïÊ½ÄÚÈÝ
            if (!m_xmlFileRst.SetNodeAttrValue(pElement, XMLTAG_STATRST_EXP, *it))
            {
                pElement->release();
                RollBack();
                return ERR_FAIL;
            }
            //ÊÇ·ñÔÚÓÃ
            if (!m_xmlFileRst.SetNodeAttrValue(pElement, XMLTAG_STATRST_INURSED, "1"))
            {
                pElement->release();
                RollBack();
                return ERR_FAIL;
            }
            //ÐòºÅ
            nLastExpMsgIndex++;
            sprintf(chr, "%d", nLastExpMsgIndex);
            if (!m_xmlFileRst.SetNodeAttrValue(pElement, XMLTAG_STATRST_EXPINDEX, chr))
            {
                pElement->release();
                RollBack();
                return ERR_FAIL;
            }

            //Ìí¼Ó
            if (!m_xmlFileRst.AddChildToNode(pEleHead, pElement))
            {
                pElement->release();
                RollBack();
                return ERR_FAIL;
            }            
        }

        it++;
    }
    
    //¸ÄÐ´±í´ïÊ½µÄ×ÜÊý,ÖØÐÂÐ´½øÎÄ¼þ    
    UINT4 nCount = 0;
    pNodeList = m_xmlFileRst.GetChildListByTagName(pEleHead, XMLTAG_STATRST_EXPINDEXMSG); 
    if (pNodeList)
    {
        nCount = pNodeList->getLength();
    }
    sprintf(chr, "%d", nCount);
    if (!m_xmlFileRst.SetNodeAttrValue(pEleHead, XMLTAG_STATRST_EXPCOUNT, chr))
    {
        RollBack();
        return ERR_FAIL;
    }
        
    //ÊÍ·Å±¸·Ý×ÊÔ´
    m_pDoc->release();

    //Ìá½»µ½ÎÄ¼þÖÐ
	if (!m_xmlFileRst.WriteXmlFile())
	{
		return ERR_FAIL;
    }
    
    return ERR_SUCCESS;
}


/*******************************************************************
º¯ÊýÃû³Æ£ºAddStatRst(const SAllStatExpRst& statExpRst, time_t statTime)
¹¦ÄÜÃèÊö£ºÌí¼ÓÒ»ÌìµÄËùÓÐÍ³¼Æ±í´ïÊ½µÄÍ³¼Æ½á¹û
ÊäÈë²ÎÊý£ºconst SAllStatExpRst& statExpRst,Ò»ÌìµÄÍ³¼Æ½á¹û
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£º³É¹¦·µ»ØERR_SUCCESS,·ñÔò·µ»ØERR_FAIL
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
int CXmlCDRStatRst::AddStatRst(const SAllStatExpRst& statExpRst, time_t statTime)
{
	DOMDocument* pDoc = m_xmlFileRst.GetDOMDocument();
    if (!pDoc)
    {
        return ERR_FAIL;
    }
	DOMElement* pRootEle = pDoc->getDocumentElement();
	if (pRootEle == 0)
	{
		return ERR_FAIL;
	}
    m_pDoc = (DOMDocument*)pDoc->cloneNode(true);

	//»ñÈ¡Í·ÐÅÏ¢¶ÔÏó
	DOMElement* pHeadEle = GetHeadElement();
	if (pHeadEle == 0)
	{
        m_pDoc->release();
		return ERR_FAIL;
	}
	char szBuf[1024];

    STRING strTime = ConvertTimeToString_YMD(statTime);
    //ÉèÖÃÍ³¼ÆÊ±¼ä
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadEle, XMLTAG_STATRST_ENDTIME, strTime.c_str()))
	{
        RollBack();
		return ERR_FAIL;
	}
	
	//Ôö¼ÓÍ³¼ÆÄÚÈÝ
    //´´½¨ÐÂµÄÍ³¼Æ½á¹û¶ÔÏó
	DOMElement* pNewEle = m_xmlFileRst.CreateNewElement(pDoc, XMLTAG_STATRST_CONTENT);
	if (pNewEle == 0)
	{
        RollBack();
		return ERR_FAIL;
	}
	//Í³¼Æ½ØÖ¹Ê±¼ä
	//sprintf(szBuf, "%ld", statExpRst.uStatEndTime);
    strTime = ConvertTimeToString_YMD(statExpRst.uStatEndTime);
	if (!m_xmlFileRst.SetNodeAttrValue(pNewEle, XMLTAG_STATRST_STATENDTIME, strTime.c_str()))
	{
        RollBack();
		pNewEle->release();
		return ERR_FAIL;
	}
	//ºóÐøÍ³¼Æ×ÓÏîÊý
	UINT4 nSubCnt = statExpRst.uStatSubItemCount;
	//UINT4 nSubCnt = sizeof(statExpRst)/sizeof(SOneStatExpRst);
	sprintf(szBuf, "%ld", nSubCnt);
	if (!m_xmlFileRst.SetNodeAttrValue(pNewEle, XMLTAG_STATRST_SUBITEMCOUT, szBuf))
	{
        RollBack();
		pNewEle->release();
		return ERR_FAIL;
    }
    //È¡Êµ¼ÊÊý×é¸öÊý
    //nSubCnt = sizeof(statExpRst.ItemRst)/sizeof(SOneStatExpRst);
    //Ôö¼ÓÍ³¼Æ×ÓÏî
    DOMElement* pSubEle = 0;
    for(UINT4 i = 0; i < nSubCnt; i++)
    {
        pSubEle = m_xmlFileRst.CreateNewElement(pDoc, XMLTAG_STATRST_ONESTATEXPRST);
        if (pSubEle == 0)
        {
            RollBack();
            pNewEle->release();
            return ERR_FAIL;
        }
        //Í³¼Æ±í´ïÊ½Ë÷ÒýºÅ
        sprintf(szBuf, "%ld", statExpRst.ItemRst[i].nStatExpIndex);
        if (!m_xmlFileRst.SetNodeAttrValue(pSubEle, XMLTAG_STATRST_ITEMSTATEXPINDEX, szBuf))
        {
            RollBack();
            pSubEle->release();
            pNewEle->release();
            return ERR_FAIL;
        }
        //»°µ¥ÕÅÊý
        sprintf(szBuf, "%ld", statExpRst.ItemRst[i].nCDRCount);
        if (!m_xmlFileRst.SetNodeAttrValue(pSubEle, XMLTAG_STATRST_CDRCOUNT, szBuf))
        {
            RollBack();
            pSubEle->release();
            pNewEle->release();
            return ERR_FAIL;
        }
        //ÓòµÄÀÛ¼ÆÖµ
        sprintf(szBuf, "%ld", statExpRst.ItemRst[i].nValueTotal);
        if (!m_xmlFileRst.SetNodeAttrValue(pSubEle, XMLTAG_STATRST_VALUETOTAL, szBuf))
        {
            RollBack();
            pSubEle->release();
            pNewEle->release();
            return ERR_FAIL;
        }
        
        //Ôö¼ÓÕâ¸öÍ³¼Æ×Ó½Úµã
        if (!m_xmlFileRst.AddChildToNode(pNewEle, pSubEle))
        {
            RollBack();
            pSubEle->release();
            pNewEle->release();
            return ERR_FAIL;
        }
    }
    
    //Ôö¼ÓÕâ¸öÍ³¼Æ½Úµã
    if (!m_xmlFileRst.AddChildToNode(pRootEle, pNewEle))
    {
        RollBack();
        pNewEle->release();
		return ERR_FAIL;
	}

    //ÊÍ·Å±¸·Ý×ÊÔ´
    m_pDoc->release();

    //Ð´Èëµ½XMLÎÄ¼þ
	if (!m_xmlFileRst.WriteXmlFile())
	{
		return ERR_FAIL;
	}
	
	return ERR_SUCCESS;
}

/*******************************************************************
º¯ÊýÃû³Æ£ºCleanStatExp(void)
¹¦ÄÜÃèÊö£ºÇåÀíÍ³¼Æ½á¹ûÎÄ¼þÖÐ²»ÔÙÊ¹ÓÃµÄÌõ¼þ±í´ïÊ½
ÊäÈë²ÎÊý£ºÎÞ
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£ºÎÞ
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
int CXmlCDRStatRst::CleanStatExp(void)
{
    //¸Ãº¯ÊýÄ¿Ç°Ã»ÓÐÓ¦ÓÃ
	return ERR_SUCCESS;
}

/*******************************************************************
º¯ÊýÃû³Æ£ºFindStatExpress(char* szExpress, UINT4& nIndex)
¹¦ÄÜÃèÊö£º¸ù¾Ý±í´ïÊ½²éÕÒÆäÐòºÅ
ÊäÈë²ÎÊý£ºchar* szExpress, ´ýÕÒµÄ±í´ïÊ½
Êä³ö²ÎÊý£ºUINT4& nIndex,±í´ïÊ½ÐòºÅÖµ
·µ»ØÄÚÈÝ£ºÕÒµ½·µ»ØNODE¶ÔÏó,·ñÔò·µ»ØNULL
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
DOMNode* CXmlCDRStatRst::FindStatExpress(char* szExpress, UINT4& nIndex)
{
    BOOL bReturn = FALSE;
	SStatExpIndexList* pStatExpIndexList = NULL;

    //»ñÈ¡Í·½Úµã
    DOMElement* pEleHead = GetHeadElement();
    if (!pEleHead)
    {
        return NULL;
    }

    //»ñÈ¡Ìõ¼þ±í´ïÊ½ÁÐ±í
    DOMNodeList* pNodeList = m_xmlFileRst.GetChildListByTagName(pEleHead, XMLTAG_STATRST_EXPINDEXMSG); 
    if (!pNodeList)
    {
        return NULL;
    }

    int nListCount = pNodeList->getLength();
    const XMLCh* pCh = NULL;
    DOMNode* pNode = NULL;
    //Ñ­»·²éÕÒÃ¿Ò»¸öÌõ¼þ±í´ïÊ½
    for (int i = 0; i < nListCount; i++)
    {
        pNode = pNodeList->item(i);
        pCh = m_xmlFileRst.GetNodeAttrValue((DOMElement*)pNode, XMLTAG_STATRST_EXP);
        if (strcmp(S(pCh), szExpress) == 0)
        {
            nIndex = atoi(S(m_xmlFileRst.GetNodeAttrValue((DOMElement*)pNode, XMLTAG_STATRST_EXPINDEX)));
            return pNode;
        }
    }

    return NULL;
}

/*******************************************************************
º¯ÊýÃû³Æ£ºRollBack(void)
¹¦ÄÜÃèÊö£ºÈç¹ûÐ´Ê§°Ü£¬»Ø¹öÊý¾Ý
ÊäÈë²ÎÊý£ºÎÞ
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£ºÎÞ
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
BOOL CXmlCDRStatRst::RollBack(void)
{    
    if (!m_pDoc)
    {
        return FALSE;
    }
	
	return m_xmlFileRst.SetDOMDocument(m_pDoc);
}


/*******************************************************************
º¯ÊýÃû³Æ£ºFindStatRst(SAllStatExpRst* pAllRst, UINT4 nIndex, UINT4& nCDRCount, UINT4& nValueTotal)
¹¦ÄÜÃèÊö£ºÔÚËùÓÐµÄÍ³¼Æ½á¹ûÖÐ¸ù¾ÝË÷ÒýºÅ²éÕÒÆäÍ³¼Æ½á¹ûÖµ
ÊäÈë²ÎÊý£ºSAllStatExpRst* pAllRst, ËùÓÐÍ³¼Æ½á¹û½á¹¹
          UINT4 nIndex, Ë÷ÒýºÅ
Êä³ö²ÎÊý£ºUINT4& nCDRCount, »°µ¥ÕÅÊý
          UINT4& nValueTotal,ÀÛ¼ÓÖµ
·µ»ØÄÚÈÝ£ºÎÞ
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
void CXmlCDRStatRst::FindStatRst(DOMNode* pNode, UINT4 nIndex, UINT4& nCDRCount, UINT4& nValueTotal)
{
	if (pNode == 0)
	{
		return;
	}
	
    //»ñÈ¡½á¹ûÁÐ±í
	DOMNodeList* pDOMChilds = m_xmlFileRst.GetChildListByTagName((DOMElement*)pNode, XMLTAG_STATRST_ONESTATEXPRST);	
	if (!pDOMChilds)
	{
		return;
	}
    
	DOMElement* pChild = NULL;
    const XMLCh* chExpIndex = NULL;

    int nCount = pDOMChilds->getLength();
    //Ñ­»·²éÕÒÃ¿Ò»¸ö½á¹û¶ÔÏó
	for(int i = 0; i < nCount; i++)
	{
		pChild = (DOMElement*)pDOMChilds->item(i);
		if (!pChild)
		{
            continue;
        }
        
        chExpIndex = m_xmlFileRst.GetNodeAttrValue(pChild,XMLTAG_STATRST_ITEMSTATEXPINDEX);
        //ÐòºÅÏàÍ¬£¬ÕÒµ½¶ÔÓ¦½á¹û¼ÇÂ¼
        if (atoi(S(chExpIndex)) == nIndex)
        {
            nCDRCount += atoi(S(m_xmlFileRst.GetNodeAttrValue(pChild, XMLTAG_STATRST_CDRCOUNT)));
            nValueTotal += atoi(S(m_xmlFileRst.GetNodeAttrValue(pChild, XMLTAG_STATRST_VALUETOTAL)));
            return;
        }
    }	
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
UINT4 CXmlCDRStatRst::GetAccessPoint()
{
	return m_uAccessPoint;
}


/*******************************************************************
º¯ÊýÃû³Æ£ºGetAccessPointName()
¹¦ÄÜÃèÊö£º·µ»Ø½ÓÈëµãÃû³Æ
ÊäÈë²ÎÊý£ºÎÞ
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£ºÎÞ
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
const char* CXmlCDRStatRst::GetAccessPointName()
{
	return m_sAPName.c_str();
}

/*******************************************************************
º¯ÊýÃû³Æ£ºGetAllExpress(LIST<STRING>& ExpressList)
¹¦ÄÜÃèÊö£º¸ù¾Ý½ÓÈëµãºÅ·µ»ØËùÓÐ±í´ïÊ½
ÊäÈë²ÎÊý£ºÎÞ
Êä³ö²ÎÊý£ºLIST<STRING>& ExpressList,»ñµÃËùÓÐ±í´ïÊ½ÁÐ±í
·µ»ØÄÚÈÝ£ºÎÞ
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
void  CXmlCDRStatRst::GetAllExpress(LIST<STRING>& ExpressList)
{
	SStatExpIndexList* pExpList = NULL;
	pExpList = GetStatExpList();

    if (!pExpList)
    {
        return;
    }
	for( UINT4 i = 0; i < pExpList->uStatExpCount; i++)
	{
        if((pExpList->expIndex[i]).bInUsed)
        {
            ExpressList.push_back((pExpList->expIndex[i]).szStatExp);
        }
	}
	delete pExpList;
}

/*******************************************************************
º¯ÊýÃû³Æ£ºGetLastStatTime()
¹¦ÄÜÃèÊö£º»ñµÃÉÏ´ÎÍ³¼ÆÊ±¼ä
ÊäÈë²ÎÊý£ºÎÞ
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£ºÉÏ´ÎÍ³¼ÆÊ±¼ä
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£ºmaimaoshi,2003/02/13
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
time_t CXmlCDRStatRst::GetLastStatTime()
{
    //»ñÈ¡Í·ÐÅÏ¢¶ÔÏó
	DOMElement* pHeadEle = GetHeadElement();
	if (!pHeadEle)
	{
	    return 0;
	}
	else
	{
        //·µ»ØÉÏ´ÎÍ³¼ÆÊ±¼ä
        const XMLCh* chr = m_xmlFileRst.GetNodeAttrValue(pHeadEle, XMLTAG_STATRST_ENDTIME);
        const char* pszTime = XMLString::transcode(chr);
		return ConvertTimeToTimeT_YMD(pszTime);
	}
}

/*******************************************************************
º¯ÊýÃû³Æ£ºAddHeadInfo(DOMElement *pRootEle)
¹¦ÄÜÃèÊö£º¼ÓÈëÎÄ¼þÍ·ÐÅÏ¢µ½XMLÎÄ¼þÖÐ
ÊäÈë²ÎÊý£ºpRootEle£¬DOMÎÄ¼þ¶ÔÏó
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£ºDOMElementÌí¼Ó³É¹¦·µ»ØÍ·ÐÅÏ¢¶ÔÏó£¬·ñÔò·µ»ØNULL
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£º 
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
DOMElement * CXmlCDRStatRst::AddHeadInfo(DOMElement *pRootEle)
{
	if (pRootEle == 0)
	{
		return NULL;
	}
	DOMDocument* pDOMDoc = m_xmlFileRst.GetDOMDocument();
	if (pDOMDoc == 0)
	{
		return NULL;
	}
    m_pDoc = (DOMDocument*)pDOMDoc->cloneNode(true);

	//Éú³ÉÍ·½áµã
	DOMElement* pHeadNode = m_xmlFileRst.CreateNewElement(pDOMDoc, XMLTAG_STATRST_HEAD);
	if (pHeadNode == 0)
	{
        m_pDoc->release();
		return NULL;
	}
	//ÔÚÍ·½áµãÖÐ¼ÓÈëÊôÐÔ
	//°æ±¾ÊôÐÔ
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadNode, XMLTAG_STATRST_VERSION, "0x00000001"))
	{
        RollBack();
        pHeadNode->release();
		return NULL; 
	}
	//½ÓÈëµãºÅ
	char szTmp[256];
	sprintf(szTmp, "%d", m_uAccessPoint);
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadNode, XMLTAG_STATRST_AP, szTmp))
	{
        RollBack();
        pHeadNode->release();
		return NULL; 
	}
    //Í³¼Æ½ØÖ¹Ê±¼ä
    time_t curtime = ACE_OS::time(NULL);
    STRING strTime = ConvertTimeToString_YMD(curtime);
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadNode, XMLTAG_STATRST_ENDTIME, strTime.c_str()))
	{
        RollBack();
        pHeadNode->release();
		return NULL; 
	}
	//±£Áô×Ö½Ú
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadNode, XMLTAG_STATRST_RESERVED, "00000000"))
	{
        RollBack();
        pHeadNode->release();
		return NULL; 
	}
    //Í³¼Æ±í´ïÊ½¸öÊý
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadNode, XMLTAG_STATRST_EXPCOUNT, "0"))
	{
        RollBack();
        pHeadNode->release();
		return NULL; 
	}

	//¼ÓÈëÍ·½áµãµ½¸ù½áµãÏÂ
	if (!m_xmlFileRst.AddChildToNode(pRootEle, pHeadNode))
	{
        RollBack();
		pHeadNode->release();
		return NULL; 
	}

    //Ìá½»µ½ÎÄ¼þ
    if (!m_xmlFileRst.WriteXmlFile())
    {
		pHeadNode->release();
		return NULL; 
    }
	return pHeadNode;
}

/*******************************************************************
º¯ÊýÃû³Æ£ºGetHeadElement()
¹¦ÄÜÃèÊö£º»ñÈ¡Í·½Úµã
ÊäÈë²ÎÊý£ºÎÞ
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£ºDOMElement»ñÈ¡³É¹¦·µ»ØÍ·ÐÅÏ¢¶ÔÏó£¬·ñÔò·µ»ØNULL
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£º 
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
DOMElement* CXmlCDRStatRst::GetHeadElement()
{
    //»ñÈ¡DOC¶ÔÏó
    DOMDocument* pDoc = m_xmlFileRst.GetDOMDocument();
    if (!pDoc)
    {
        return NULL;
    }
    //»ñÈ¡XMLÎÄ¼þµÄ¶¥²ãÔªËØ¶ÔÏó
    DOMElement* pEle = pDoc->getDocumentElement();
    if (!pEle)
    {
        return NULL;
    }

    //°´Í·ÐÅÏ¢¹Ø¼ü×Ö²éÕÒ
    DOMNodeList* pNodeList = m_xmlFileRst.GetChildListByTagName(pEle, XMLTAG_STATRST_HEAD);
    if (pNodeList && pNodeList->getLength() > 0)
    {
        return (DOMElement*)pNodeList->item(0);
    }

    return NULL;
}

/*******************************************************************
º¯ÊýÃû³Æ£ºGetContentList()
¹¦ÄÜÃèÊö£º»ñÈ¡ÎÄ¼þÄÚÈÝÁÐ±í
ÊäÈë²ÎÊý£ºÎÞ
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£ºDOMNodeList»ñÈ¡³É¹¦·µ»ØÎÄ¼þÄÚÈÝÁÐ±íÖ¸Õë£¬·ñÔò·µ»ØNULL
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£º 
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ      



********************************************************************/
DOMNodeList* CXmlCDRStatRst::GetContentList()
{
    //»ñÈ¡DOC¶ÔÏó
    DOMDocument* pDoc = m_xmlFileRst.GetDOMDocument();
    if (!pDoc)
    {
        return NULL;
    }
    DOMElement* pEle = pDoc->getDocumentElement();
    if (!pEle)
    {
        return NULL;
    }

    //·µ»ØÎÄ¼þÄÚÈÝÁÐ±í
    return m_xmlFileRst.GetChildListByTagName(pEle, XMLTAG_STATRST_CONTENT);
}

//Ë¢ÐÂÒÑÖ´ÐÐÍ³¼Æ²Ù×÷µÄÊ±¼ä
int CXmlCDRStatRst::SetLastStatTime(time_t curtime)
{
	DOMDocument* pDoc = m_xmlFileRst.GetDOMDocument();
    if (!pDoc)
    {
        return ERR_FAIL;
    }
	DOMElement* pRootEle = pDoc->getDocumentElement();
	if (pRootEle == 0)
	{
		return ERR_FAIL;
	}
    m_pDoc = (DOMDocument*)pDoc->cloneNode(true);

	//»ñÈ¡Í·ÐÅÏ¢¶ÔÏó
	DOMElement* pHeadEle = GetHeadElement();
	if (pHeadEle == 0)
	{
        m_pDoc->release();
		return ERR_FAIL;
	}

    STRING strTime = ConvertTimeToString_YMD(curtime);
    //ÉèÖÃÍ³¼ÆÊ±¼ä
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadEle, XMLTAG_STATRST_ENDTIME, strTime.c_str()))
	{
        RollBack();
		return ERR_FAIL;
	}
	
    //ÊÍ·Å±¸·Ý×ÊÔ´
    m_pDoc->release();

    //Ð´Èëµ½XMLÎÄ¼þ
	if (!m_xmlFileRst.WriteXmlFile())
	{
		return ERR_FAIL;
	}

    return ERR_SUCCESS;
}
