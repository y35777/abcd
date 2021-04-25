#include "perf_xml_util.h"
#include "../include/toolbox.h"
#include "../include/export_dll.h"

extern "C" IGWB_Svc_Export IPerfUtil* CreatePerfUtil()
{
    return new CPerfXmlUtil();
}

//ÐÔÄÜÄ£¿é¶¨Òå
/***********************************************************************************
                          ÐÔÄÜÎÄ¼þYYYYMMDD_orig.xml¸ñÊ½
 -----------------------------------------------------------------------------------
/*<?xml version="1.0" encoding="ISO8859-1" standalone="no" ?>
<Perf>
  <PerfContent APID="8" ModuleID="1" ItemID="1" timeBegin="2003-10-12 12:00:00" timeEnd="2003-10-11 20:01:00" val="3"/>
  <PerfContent APID="9" ModuleID="2" ItemID="2" timeBegin="2003-10-12 13:00:00" timeEnd="2003-10-11 20:01:02" val="6"/>
</Perf>
 -----------------------------------------------------------------------------------

***********************************************************************************/
#define XMLTAG_PERF                 "Perf"
#define XMLTAG_PERFCONTENT          "PerfContent"
#define XMLTAG_APID                 "APID"
#define XMLTAG_MODULEID             "ModuleID"
#define XMLTAG_ITEMID               "ItemID"
#define XMLTAG_TIMEBEGIN            "timeBegin"
#define XMLTAG_TIMEEND              "timeEnd"
#define XMLTAG_VAL                  "val"


CPerfXmlUtil::CPerfXmlUtil()
{
    //
}

CPerfXmlUtil::~CPerfXmlUtil()
{

}

//ÆÊÎö²¢Éú³ÉDOMÎÄµµÊ÷£¬ÎÄ¼þ²»´æÔÚÔòÉú³ÉÖ¸¶¨ÎÄ¼þ
int CPerfXmlUtil::OpenFile(const char * const szFilePath)
{
    if(szFilePath != NULL)
    {
        //³õÊ¼»¯xmlÎÄ¼þ£¬²¢½¨Á¢ÏàÓ¦µÄDOM½áµãÊ÷
        BOOL bRet = m_xmlFilePerf.InitXmlFile(szFilePath, XMLTAG_PERF);

        return bRet ? 0 : -1;
    }

    return -1;
}

//¹Ø±ÕÎÄ¼þ
int CPerfXmlUtil::CloseFile(void)
{
    return m_xmlFilePerf.ClearDOMDocument() ? 0 : -1;
}

//±£´æÎÄ¼þ
int CPerfXmlUtil::Flush(void)
{
    //Ð´xmlÎÄ¼þ
    if (!m_xmlFilePerf.WriteXmlFile())
    {
        return -1;
    }

    return 0;
}

//×·¼ÓÒ»ÌõÐÔÄÜÐÅÏ¢
int CPerfXmlUtil::AppendPerfItem(SPerfItem& perfItem)
{
    //»ñÈ¡DOC¶ÔÏó
    DOMDocument* pDoc = m_xmlFilePerf.GetDOMDocument();
    if (NULL == pDoc)
    {
        return ERR_FAIL;
    }
    //»ñÈ¡XMLÎÄ¼þµÄ¶¥²ãÔªËØ¶ÔÏó
    DOMElement* pRootEle = pDoc->getDocumentElement();
    if (NULL == pRootEle)
    {
        return ERR_FAIL;
    }

    //·µ»ØÎÄ¼þÄÚÈÝÁÐ±í
    DOMNodeList* pNodeList = m_xmlFilePerf.GetChildListByTagName(pRootEle, XMLTAG_PERFCONTENT);
    if (pNodeList == NULL)
    {
        return ERR_FAIL;
    }

    DOMElement* pNewEle = m_xmlFilePerf.CreateNewElement(pDoc, XMLTAG_PERFCONTENT);
    if (pNewEle == NULL)
    {
        return ERR_FAIL;
    }
    
	char szBuf[1024];
    //½ø³ÌºÅ
    sprintf(szBuf, "%ld", perfItem.yAPID);
    if (!m_xmlFilePerf.SetNodeAttrValue(pNewEle, XMLTAG_APID, szBuf))
    {
        pNewEle->release();
        return ERR_FAIL;
    }

    //Ä£¿éºÅ
    sprintf(szBuf, "%ld", perfItem.yModuleID);
    if (!m_xmlFilePerf.SetNodeAttrValue(pNewEle, XMLTAG_MODULEID, szBuf))
    {
        pNewEle->release();
        return ERR_FAIL;
    }

    //ÐÔÄÜÍ³¼ÆÏîµÄ±àºÅ
    sprintf(szBuf, "%ld", perfItem.uID);
    if (!m_xmlFilePerf.SetNodeAttrValue(pNewEle, XMLTAG_ITEMID, szBuf))
    {
        pNewEle->release();
        return ERR_FAIL;
    }

    //Í³¼Æ¿ªÊ¼Ê±¿Ì
    sprintf(szBuf, "%ld", perfItem.timeBegin);
    if (!m_xmlFilePerf.SetNodeAttrValue(pNewEle, XMLTAG_TIMEBEGIN, szBuf))
    {
        pNewEle->release();
        return ERR_FAIL;
    }

    //Í³¼Æ½áÊøÊ±¿Ì
    sprintf(szBuf, "%ld", perfItem.timeEnd);
    if (!m_xmlFilePerf.SetNodeAttrValue(pNewEle, XMLTAG_TIMEEND, szBuf))
    {
        pNewEle->release();
        return ERR_FAIL;
    }

    //ÐÔÄÜÍ³¼ÆÏîµÄÈ¡Öµ
    sprintf(szBuf, "%ld", perfItem.uValue);
    if (!m_xmlFilePerf.SetNodeAttrValue(pNewEle, XMLTAG_VAL, szBuf))
    {
        pNewEle->release();
        return ERR_FAIL;
    }

    //Ôö¼ÓÕâ¸ö½Úµã
    if (!m_xmlFilePerf.AddChildToNode(pRootEle, pNewEle))
    {
        pNewEle->release();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}
