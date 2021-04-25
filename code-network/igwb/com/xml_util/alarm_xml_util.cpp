#include "../include/toolbox.h"
#include "alarm_xml_util.h"
#include "../include/export_dll.h"

extern "C" IGWB_Svc_Export IAlarmUtil* CreateAlarmUtil()
{
    return new CAlarmXmlUtil();
}

CAlarmXmlUtil::CAlarmXmlUtil()
{
    //
}

CAlarmXmlUtil::~CAlarmXmlUtil()
{

}

/***********************************************************************************
                          ÀúÊ·¸æ¾¯ÎÄ¼þalarmhistory.xml¸ñÊ½
 -----------------------------------------------------------------------------------
<?xml version="1.0" encoding="ISO8859-1" standalone="no" ?>
<AlarmHistory>
  <Head CSN="11" SYN="11" BASEID="11"/>
  <Comment CSN="10" SYN="10" ModuleNO="1" AlarmID="2" AlarmType="3" FuncID="4" AlarmLevel="5" SubSysID="6" SDate="2002-01-01 12:12:12" CPUPos="1" PID="7" Para="010BAB" ResType="8" RDate="2002-01-02 12:12:12" FAckOptr="A" FAckDate="2002-01-03 12:12:12" RAckOptr="B" RAckDate="2002-01-04 12:12:12"/>
  <Comment CSN="11" SYN="11" ModuleNO="1" AlarmID="2" AlarmType="3" FuncID="4" AlarmLevel="5" SubSysID="6" SDate="2003-01-01 12:12:12" CPUPos="1" PID="7" Para="010BAB" ResType="8" RDate="2003-01-02 12:12:12" FAckOptr="A" FAckDate="2003-01-03 12:12:12" RAckOptr="B" RAckDate="2003-01-04 12:12:12"/>
</AlarmHistory>
 -----------------------------------------------------------------------------------

***********************************************************************************/
#define XMLTAG_ALARM_ALARMHISTORY           "AlarmHistory"
#define XMLTAG_ALARM_HEAD                   "Head"
#define XMLTAG_ALARM_HEAD_CSN               "CSN"
#define XMLTAG_ALARM_HEAD_SYN               "SYN"
#define XMLTAG_ALARM_HEAD_BASEID            "BASEID"
#define XMLTAG_ALARM_CONTENT                "Comment"
#define XMLTAG_ALARM_CONTENT_CSN            "CSN"
#define XMLTAG_ALARM_CONTENT_SYN            "SYN"
#define XMLTAG_ALARM_CONTENT_MODULENO       "ModuleNO"
#define XMLTAG_ALARM_CONTENT_ALARMID        "AlarmID"
#define XMLTAG_ALARM_CONTENT_ALARMTYPE      "AlarmType"
#define XMLTAG_ALARM_CONTENT_FUNCID         "FuncID"
#define XMLTAG_ALARM_CONTENT_ALARMLEVEL     "AlarmLevel"
#define XMLTAG_ALARM_CONTENT_SUBSYSID       "SubSysID"
#define XMLTAG_ALARM_CONTENT_SDATE          "SDate"
#define XMLTAG_ALARM_CONTENT_CPUPOS         "CPUPos"
#define XMLTAG_ALARM_CONTENT_PID            "PID"
#define XMLTAG_ALARM_CONTENT_PARA           "Para"
#define XMLTAG_ALARM_CONTENT_RESTYPE        "ResType"
#define XMLTAG_ALARM_CONTENT_RDATE          "RDate"
#define XMLTAG_ALARM_CONTENT_FACKOPTR       "FAckOptr"
#define XMLTAG_ALARM_CONTENT_FACKDATE       "FAckDate"
#define XMLTAG_ALARM_CONTENT_RACKOPTR       "RAckOptr"
#define XMLTAG_ALARM_CONTENT_RACKDATE       "RAckDate"

//ÆÊÎö²¢Éú³ÉDOMÎÄµµÊ÷£¬ÎÄ¼þ²»´æÔÚÔòÉú³ÉÖ¸¶¨ÎÄ¼þ
int CAlarmXmlUtil::OpenFile(const char * const szFilePath)
{
    if(szFilePath != NULL)
    {
        //³õÊ¼»¯xmlÎÄ¼þ£¬²¢½¨Á¢ÏàÓ¦µÄDOM½áµãÊ÷
        BOOL bRet = m_xmlFileAlarm.InitXmlFile(szFilePath, XMLTAG_ALARM_ALARMHISTORY);

        return bRet ? 0 : -1;
    }

    return -1;
}

//¹Ø±ÕÎÄ¼þ
int CAlarmXmlUtil::CloseFile(void)
{
    return m_xmlFileAlarm.ClearDOMDocument() ? 0 : -1;
}

//»ñµÃÊ£ÓàµÄ¸æ¾¯Êý
int CAlarmXmlUtil::GetCount(void)
{
    //»ñÈ¡DOC¶ÔÏó
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
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

    DOMNodeList *pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle,XMLTAG_ALARM_CONTENT);
    if (NULL == pNodeList)
    {
        return 0;
    }

    int nCount = pNodeList->getLength();

    return nCount;
}

//±£´æÎÄ¼þ
int CAlarmXmlUtil::Flush(void)
{
    //Ð´xmlÎÄ¼þ
    if (!m_xmlFileAlarm.WriteXmlFile())
    {
        return -1;
    }

    return 0;
}

//É¾³ýÄ³Ìõ¸æ¾¯¼ÇÂ¼
int CAlarmXmlUtil::Delete(int nIndex)
{
    //»ñÈ¡DOC¶ÔÏó
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
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

    DOMNodeList *pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle,XMLTAG_ALARM_CONTENT);
    if (NULL == pNodeList)
    {
        return 0;
    }

    DOMNode* pNode = pNodeList->item(nIndex);
    //É¾³ý¼ÇÂ¼
    if (!m_xmlFileAlarm.DeleteNode(pNode))
    {
        //É¾³ýÊ§°Ü
        return ERR_FAIL;
    }

    return 0;
}

//»ñµÃÎÄ¼þÍ·¼ÇÂ¼
int CAlarmXmlUtil::ReadHeadElement(UINT4& uCSN)
{
    //»ñÈ¡DOC¶ÔÏó
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
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

    //°´Í·ÐÅÏ¢¹Ø¼ü×Ö²éÕÒ
    DOMNodeList* pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_HEAD);
    if (pNodeList && pNodeList->getLength() > 0)
    {
        DOMElement* pEleHead = (DOMElement*)pNodeList->item(0);
        const XMLCh* chr = NULL;
        chr = m_xmlFileAlarm.GetNodeAttrValue(pEleHead, XMLTAG_ALARM_HEAD_CSN);
        if (!chr)
        {
            return ERR_FAIL;
        }
        uCSN = atoi(S(chr));
        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

//Ìí¼Ó/Ð´ÎÄ¼þÍ·¼ÇÂ¼
int CAlarmXmlUtil::WriteHeadElement(UINT4 uCSN)
{
    //»ñÈ¡DOC¶ÔÏó
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
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

    //°´Í·ÐÅÏ¢¹Ø¼ü×Ö²éÕÒ
    DOMNodeList* pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_HEAD);
    if (NULL == pNodeList || pNodeList->getLength() == 0)
    {
        //Éú³ÉÍ·½áµã
        DOMElement* pHeadNode = m_xmlFileAlarm.CreateNewElement(pDoc, XMLTAG_ALARM_HEAD);
        if (pHeadNode == NULL)
        {
            return ERR_FAIL;
        }
        //ÔÚÍ·½áµãÖÐ¼ÓÈëÊôÐÔ
        if (!m_xmlFileAlarm.SetNodeAttrValue(pHeadNode, XMLTAG_ALARM_HEAD_CSN, "99"))
        {
            pHeadNode->release();
            return ERR_FAIL;
        }

        //Ìí¼ÓÍ·½Úµã
        if (!m_xmlFileAlarm.AddChildToNode(pRootEle, pHeadNode))
        {
            pHeadNode->release();
            return ERR_FAIL;
        }
        pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_HEAD);
    }

    if (NULL != pNodeList && pNodeList->getLength() > 0)
    {
        DOMElement* pEleHead = (DOMElement*)pNodeList->item(0);

        //ÉèÖÃÐÂ¸æ¾¯Á÷Ë®ºÅ
        char szBuf[1024];
        sprintf(szBuf, "%ld", uCSN);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleHead, XMLTAG_ALARM_HEAD_CSN, szBuf))
        {
            return ERR_FAIL;
        }

        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

//Ð´Ò»ÌõÈÕÖ¾ÐÅÏ¢
int CAlarmXmlUtil::SaveHisAlarm(int            nIndex,
                                SHistoryAlarm& hisAlarm)
{
    //»ñÈ¡DOC¶ÔÏó
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
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
    DOMNodeList* pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_CONTENT);
    if (pNodeList == NULL)
    {
        return ERR_FAIL;
    }

    DOMElement* pEleAlarm = NULL;
    if (nIndex > pNodeList->getLength())
    {
        return ERR_FAIL;
    }
    else if (nIndex == pNodeList->getLength())
    {
        //´´½¨ÐÂµÄ¼ÇÂ¼¶ÔÏó
        pEleAlarm = m_xmlFileAlarm.CreateNewElement(pDoc, XMLTAG_ALARM_CONTENT);
        if (pEleAlarm == NULL)
        {
            return ERR_FAIL;
        }
        //Ìí¼Ó½Úµã
        if (!m_xmlFileAlarm.AddChildToNode(pRootEle, pEleAlarm))
        {
            pEleAlarm->release();
            return ERR_FAIL;
        }
    }
    else
    {
        pEleAlarm = (DOMElement*)pNodeList->item(nIndex);
    }

    if(NULL != pEleAlarm)
    {
        char szBuf[1024];
        char szDate[20];

        //¿ªÊ¼ÉèÖÃÖµ
        //¸æ¾¯Á÷Ë®ºÅ,Ë³Ðò·ÖÅä,»Ö¸´¸æ¾¯ÓëÆä¶ÔÓ¦µÄ¹ÊÕÏ¸æ¾¯uCsnÏàÍ¬
        sprintf(szBuf, "%ld", hisAlarm.uCsn);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_CSN, szBuf))
        {
            return ERR_FAIL;
        }

        //¸æ¾¯Í¬²½ºÅ
        sprintf(szBuf, "%ld", hisAlarm.uSyn);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_SYN, szBuf))
        {
            return ERR_FAIL;
        }

        //·¢ËÍ¸æ¾¯µÄÖ÷»úÄ£¿éºÅ,¶ÔÓÚiGateway bill,Ö÷Îª0,±¸Îª1
        sprintf(szBuf, "%ld", hisAlarm.yModuleNO);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_MODULENO, szBuf))
        {
            return ERR_FAIL;
        }

        //¸æ¾¯±àºÅ
        sprintf(szBuf, "%ld", hisAlarm.uAlarmID);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_ALARMID, szBuf))
        {
            return ERR_FAIL;
        }

        //¸æ¾¯ÀàÐÍ,ALARM_TYPEÃ¶¾ÙÐÍ
        sprintf(szBuf, "%ld", hisAlarm.yAlarmType);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_ALARMTYPE, szBuf))
        {
            return ERR_FAIL;
        }

        //¸æ¾¯Ïä¸æ¾¯·ÖÀà,ALARM_FUNCTION_IDÃ¶¾ÙÐÍ
        sprintf(szBuf, "%ld", hisAlarm.yFuncID);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_FUNCID, szBuf))
        {
            return ERR_FAIL;
        }

        //¸æ¾¯¼¶±ð,ALARM_LEVELÃ¶¾ÙÐÍ
        sprintf(szBuf, "%ld", hisAlarm.yAlarmLevel);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_ALARMLEVEL, szBuf))
        {
            return ERR_FAIL;
        }

        //Íø¹Ü¸æ¾¯·ÖÀà,ALARM_SUBSYSTEMÃ¶¾ÙÐÍ
        sprintf(szBuf, "%ld", hisAlarm.ySubSysID);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_SUBSYSID, szBuf))
        {
            return ERR_FAIL;
        }

        //¸æ¾¯²úÉúµÄÈÕÆÚ,Ê±¼ä
        szDate[0] = '\0';
        if (hisAlarm.uSYear + hisAlarm.ySMonth + hisAlarm.ySDay
            + hisAlarm.ySHour + hisAlarm.ySMin + hisAlarm.ySSec != 0)
        {
            sprintf(szDate,"%04d-%02d-%02d %02d:%02d:%02d",
                hisAlarm.uSYear, hisAlarm.ySMonth, hisAlarm.ySDay
                , hisAlarm.ySHour, hisAlarm.ySMin, hisAlarm.ySSec);
        }
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_SDATE, szDate))
        {
            return ERR_FAIL;
        }

        //A(0),B(1)»úÖ¸Ê¾,¶ÔÓÚiGateway bill,Ö÷Îª0,±¸Îª1
        sprintf(szBuf, "%ld", hisAlarm.yCPUPos);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_CPUPOS, szBuf))
        {
            return ERR_FAIL;
        }

        //¸æ¾¯²úÉúµÄ½ø³Ì±êÊ¶
        sprintf(szBuf, "%ld", hisAlarm.yPID);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_PID, szBuf))
        {
            return ERR_FAIL;
        }

        //¸æ¾¯²ÎÊý
        for (int nLoop = 0; nLoop < MAX_ALARM_PARA; nLoop++)
        {
            sprintf(szBuf + nLoop*2, "%02X", hisAlarm.yPara[nLoop]);
        }
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_PARA, szBuf))
        {
            return ERR_FAIL;
        }

        //¸æ¾¯»Ö¸´±êÖ¾,ALARM_RECOVER_TYPEÃ¶¾ÙÐÍ
        sprintf(szBuf, "%ld", hisAlarm.yResType);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_RESTYPE, szBuf))
        {
            return ERR_FAIL;
        }

        //¸æ¾¯»Ö¸´µÄÈÕÆÚ,Ê±¼ä
        szDate[0] = '\0';
        if (hisAlarm.uRYear + hisAlarm.yRMonth + hisAlarm.yRDay
            + hisAlarm.yRHour + hisAlarm.yRMin + hisAlarm.yRSec != 0)
        {
            sprintf(szDate,"%04d-%02d-%02d %02d:%02d:%02d",
                hisAlarm.uRYear, hisAlarm.yRMonth, hisAlarm.yRDay
                , hisAlarm.yRHour, hisAlarm.yRMin, hisAlarm.yRSec);
        }
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_RDATE, szDate))
        {
            return ERR_FAIL;
        }

        //¸æ¾¯È·ÈÏµÄ²Ù×÷Ô±,ÈÕÆÚ,Ê±¼ä
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_FACKOPTR, hisAlarm.szFAckOptr))
        {
            return ERR_FAIL;
        }

        szDate[0] = '\0';
        if (hisAlarm.uFAckYear + hisAlarm.yFAckMonth + hisAlarm.yFAckDay
            + hisAlarm.yFAckHour + hisAlarm.yFAckMin + hisAlarm.yFAckSec != 0)
        {
            sprintf(szDate,"%04d-%02d-%02d %02d:%02d:%02d",
                hisAlarm.uFAckYear, hisAlarm.yFAckMonth, hisAlarm.yFAckDay
                , hisAlarm.yFAckHour, hisAlarm.yFAckMin, hisAlarm.yFAckSec);
        }
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_FACKDATE, szDate))
        {
            return ERR_FAIL;
        }

        //¸æ¾¯»Ö¸´È·ÈÏµÄ²Ù×÷Ô±,ÈÕÆÚ,Ê±¼ä
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_RACKOPTR, hisAlarm.szRAckOptr))
        {
            return ERR_FAIL;
        }

        szDate[0] = '\0';
        if (hisAlarm.uRAckYear + hisAlarm.yRAckMonth + hisAlarm.yRAckDay
            + hisAlarm.yRAckHour + hisAlarm.yRAckMin + hisAlarm.yRAckSec != 0)
        {
            sprintf(szDate,"%04d-%02d-%02d %02d:%02d:%02d",
                hisAlarm.uRAckYear, hisAlarm.yRAckMonth, hisAlarm.yRAckDay
                , hisAlarm.yRAckHour, hisAlarm.yRAckMin, hisAlarm.yRAckSec);
        }
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_RACKDATE, szDate))
        {
            return ERR_FAIL;
        }

        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

//¶ÁÒ»Ìõ¸æ¾¯ÐÅÏ¢
int CAlarmXmlUtil::ReadOneAlarm(int            nIndex,
                                SHistoryAlarm& hisAlarm)
{
    //»ñÈ¡DOC¶ÔÏó
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
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
    DOMNodeList* pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_CONTENT);
    if (pNodeList == NULL)
    {
        return ERR_FAIL;
    }

    if (nIndex >= pNodeList->getLength())
    {
        return ERR_FAIL;
    }

    DOMElement* pEleAlarm = (DOMElement*)pNodeList->item(nIndex);
    if(NULL != pEleAlarm)
    {
        const XMLCh* pChr = NULL;
        UINT2 uYear = 0;        //4Î»Äê
        BYTE yMonth = 0;        //2Î»ÔÂ
        BYTE yDay   = 0;        //2Î»ÈÕ
        BYTE yHour  = 0;        //2Î»Ê±
        BYTE yMin   = 0;        //2Î»·Ö
        BYTE ySec   = 0;        //2Î»Ãë

        //¿ªÊ¼»ñÈ¡Êý¾Ý

        //¸æ¾¯Á÷Ë®ºÅ,Ë³Ðò·ÖÅä,»Ö¸´¸æ¾¯ÓëÆä¶ÔÓ¦µÄ¹ÊÕÏ¸æ¾¯uCsnÏàÍ¬
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_CSN);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.uCsn = atoi(S(pChr));

        //¸æ¾¯Í¬²½ºÅ
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_SYN);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.uSyn = atoi(S(pChr));

        //·¢ËÍ¸æ¾¯µÄÖ÷»úÄ£¿éºÅ,¶ÔÓÚiGateway bill,Ö÷Îª0,±¸Îª1
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_MODULENO);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.yModuleNO = atoi(S(pChr));

        //¸æ¾¯±àºÅ
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_ALARMID);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.uAlarmID = atoi(S(pChr));

        //¸æ¾¯ÀàÐÍ,ALARM_TYPEÃ¶¾ÙÐÍ
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_ALARMTYPE);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.yAlarmType = atoi(S(pChr));

        //¸æ¾¯Ïä¸æ¾¯·ÖÀà,ALARM_FUNCTION_IDÃ¶¾ÙÐÍ
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_FUNCID);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.yFuncID = atoi(S(pChr));

        //¸æ¾¯¼¶±ð,ALARM_LEVELÃ¶¾ÙÐÍ
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_ALARMLEVEL);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.yAlarmLevel = atoi(S(pChr));

        //Íø¹Ü¸æ¾¯·ÖÀà,ALARM_SUBSYSTEMÃ¶¾ÙÐÍ
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_SUBSYSID);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.ySubSysID = atoi(S(pChr));

        //¸æ¾¯²úÉúµÄÈÕÆÚ,Ê±¼ä
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_SDATE);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        //¸ñÊ½×ª»»
        ProcessDate(pChr, uYear, yMonth, yDay, yHour, yMin, ySec);
        hisAlarm.uSYear  = uYear;
        hisAlarm.ySMonth = yMonth;
        hisAlarm.ySDay   = yDay;
        hisAlarm.ySHour  = yHour;
        hisAlarm.ySMin   = yMin;
        hisAlarm.ySSec   = ySec;

        //A(0),B(1)»úÖ¸Ê¾,¶ÔÓÚiGateway bill,Ö÷Îª0,±¸Îª1
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_CPUPOS);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.yCPUPos = atoi(S(pChr));

        //¸æ¾¯²úÉúµÄ½ø³Ì±êÊ¶
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_PID);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.yPID = atoi(S(pChr));

        //¸æ¾¯²ÎÊý
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_PARA);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        BYTE byValue = 0;
        char chr[3];
        for (int nLoop = 0, nCount = 0; nLoop < MAX_ALARM_PARA; nLoop++)
        {
            memcpy(chr, S(pChr) + nCount, 2);
            if (chr[0] == '\0' || chr[1] == '\0')
            {
                break;
            }
            chr[2] = '\0';
            //»ñÈ¡Ò»¸öBYTEµÄÊýÖµ
            byValue = GetHexCharValue(chr);
            hisAlarm.yPara[nLoop] = byValue;
            //Ã¿´Î´¦Àí2¸ö×Ö·û£¬¼´Ò»¸öBYTE
            nCount += 2;
        }

        //¸æ¾¯»Ö¸´±êÖ¾,ALARM_RECOVER_TYPEÃ¶¾ÙÐÍ
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_RESTYPE);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.yResType = atoi(S(pChr));

        //¸æ¾¯»Ö¸´µÄÈÕÆÚ,Ê±¼ä
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_RDATE);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        //¸ñÊ½×ª»»
        ProcessDate(pChr, uYear, yMonth, yDay, yHour, yMin, ySec);
        hisAlarm.uRYear  = uYear;
        hisAlarm.yRMonth = yMonth;
        hisAlarm.yRDay   = yDay;
        hisAlarm.yRHour  = yHour;
        hisAlarm.yRMin   = yMin;
        hisAlarm.yRSec   = ySec;

        //¸æ¾¯È·ÈÏµÄ²Ù×÷Ô±,ÈÕÆÚ,Ê±¼ä
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_FACKOPTR);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        memcpy(hisAlarm.szFAckOptr, S(pChr), 78);
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_FACKDATE);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        //¸ñÊ½×ª»»
        ProcessDate(pChr, uYear, yMonth, yDay, yHour, yMin, ySec);
        hisAlarm.uFAckYear  = uYear;
        hisAlarm.yFAckMonth = yMonth;
        hisAlarm.yFAckDay   = yDay;
        hisAlarm.yFAckHour  = yHour;
        hisAlarm.yFAckMin   = yMin;
        hisAlarm.yFAckSec   = ySec;

        //¸æ¾¯»Ö¸´È·ÈÏµÄ²Ù×÷Ô±,ÈÕÆÚ,Ê±¼ä
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_RACKOPTR);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        memcpy(hisAlarm.szRAckOptr, S(pChr), 78);
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_RACKDATE);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        //¸ñÊ½×ª»»
        ProcessDate(pChr, uYear, yMonth, yDay, yHour, yMin, ySec);
        hisAlarm.uRAckYear  = uYear;
        hisAlarm.yRAckMonth = yMonth;
        hisAlarm.yRAckDay   = yDay;
        hisAlarm.yRAckHour  = yHour;
        hisAlarm.yRAckMin   = yMin;
        hisAlarm.yRAckSec   = ySec;

        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

/*******************************************************************
º¯ÊýÃû³Æ£ºProcessDate(const XMLCh* pChr, UINT2& uYear, BYTE& yMonth, BYTE& yDay
                                , BYTE& yHour, BYTE& yMin, BYTE& ySec);
¹¦ÄÜÃèÊö£º´¦ÀíÄêÔÂÈÕÊ±·ÖÃë¸ñÊ½
ÊäÈë²ÎÊý£ºpChr´ý´¦ÀíµÄÈÕÆÚ×Ö·û´®
Êä³ö²ÎÊý£ºuYearÄê£¬yMonthÔÂ£¬yDayÈÕ£¬yHourÊ±£¬yMin·Ö£¬ySecÃë
·µ»ØÄÚÈÝ£ºÎÞ
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£º
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ



********************************************************************/
void CAlarmXmlUtil::ProcessDate(const XMLCh* pChr,
                                UINT2& uYear,
                                BYTE& yMonth,
                                BYTE& yDay,
                                BYTE& yHour,
                                BYTE& yMin,
                                BYTE& ySec)
{
    if (pChr == NULL || pChr[0] == '\0')    //¿Õ´®
    {
        uYear = 0;
        yMonth = 0;
        yDay = 0;
        yHour = 0;
        yMin = 0;
        ySec = 0;
    }
    else    //°´ÕÕ yyyy-mm-dd hh:mm:ss¸ñÊ½·Ö½â
    {
        char pTemp[5];
        memcpy(pTemp, S(pChr), 4);
        pTemp[4] = '\0';
        uYear = atoi(pTemp);

        memcpy(pTemp, S(pChr) + 5, 2);
        pTemp[2] = '\0';
        yMonth = atoi(pTemp);

        memcpy(pTemp, S(pChr) + 8, 2);
        pTemp[2] = '\0';
        yDay = atoi(pTemp);

        memcpy(pTemp, S(pChr) + 11, 2);
        pTemp[2] = '\0';
        yHour = atoi(pTemp);

        memcpy(pTemp, S(pChr) + 14, 2);
        pTemp[2] = '\0';
        yMin = atoi(pTemp);

        memcpy(pTemp, S(pChr) + 17, 2);
        pTemp[2] = '\0';
        ySec = atoi(pTemp);
    }
}

/*******************************************************************
º¯ÊýÃû³Æ£ºGetHexCharValue(char chr[2])
¹¦ÄÜÃèÊö£º×ª»»Ò»¸öÊ®Áù½øÖÆµÄ×Ö·û´®ÎªÊýÖµ£¬Èç°Ñ"AC"×ª»»³É172
ÊäÈë²ÎÊý£ºchr´ý´¦ÀíµÄÊ®Áù½øÖÆ×Ö·û´®
Êä³ö²ÎÊý£ºÎÞ
·µ»ØÄÚÈÝ£º×ª»»ºóµÄÊýÖµ
µ÷ÓÃº¯Êý£º
±»µ÷º¯Êý£º
×÷ÕßÊ±¼ä£º
------------------------------ÐÞ¸Ä¼ÇÂ¼------------------------------
ÐÞ¸ÄÈË          ÐÞ¸ÄÊ±¼ä           ÐÞ¸ÄÄÚÈÝ



********************************************************************/
BYTE CAlarmXmlUtil::GetHexCharValue(char chr[2])
{
    BYTE byValue = 0;
    int nValueTemp = 0;
    for(int i = 0, j = 1; i < 2; i++, j--)
    {
        switch(chr[i])
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            {
                nValueTemp = chr[i] - '0';
                break;
            }
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            {
                nValueTemp = chr[i] - 'A' + 10;
                break;
            }
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            {
                nValueTemp = chr[i] - 'a' + 10;
                break;
            }
        default://·ÇÊ®Áù½øÖÆ×Ö·û
            {
                return 255;
            }
        }

        if (j == 1)
        {
            byValue += nValueTemp*16;
        }
        else
        {
            byValue += nValueTemp;
        }
    }

    return byValue;
}

int CAlarmXmlUtil::ReadHeadElementSyn(UINT4& uSYN)
{
    //»ñÈ¡DOC¶ÔÏó
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
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

    //°´Í·ÐÅÏ¢¹Ø¼ü×Ö²éÕÒ
    DOMNodeList* pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_HEAD);
    if (pNodeList && pNodeList->getLength() > 0)
    {
        DOMElement* pEleHead = (DOMElement*)pNodeList->item(0);
        const XMLCh* chr = NULL;
        chr = m_xmlFileAlarm.GetNodeAttrValue(pEleHead, XMLTAG_ALARM_HEAD_SYN);
        if (!chr)
        {
            return ERR_FAIL;
        }
        uSYN = atoi(S(chr));
        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

int CAlarmXmlUtil::ReadHeadElementMMLBaseID(UINT4& uMMLBaseID)
{
    //»ñÈ¡DOC¶ÔÏó
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
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

    //°´Í·ÐÅÏ¢¹Ø¼ü×Ö²éÕÒ
    DOMNodeList* pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_HEAD);
    if (pNodeList && pNodeList->getLength() > 0)
    {
        DOMElement* pEleHead = (DOMElement*)pNodeList->item(0);
        const XMLCh* chr = NULL;
        chr = m_xmlFileAlarm.GetNodeAttrValue(pEleHead, XMLTAG_ALARM_HEAD_BASEID);
        if (!chr)
        {
            return ERR_FAIL;
        }
        uMMLBaseID = atoi(S(chr));
        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

int CAlarmXmlUtil::WriteHeadElementSyn(UINT4 uSYN)
{
    //»ñÈ¡DOC¶ÔÏó
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
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

    //°´Í·ÐÅÏ¢¹Ø¼ü×Ö²éÕÒ
    DOMNodeList* pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_HEAD);
    if (NULL == pNodeList || pNodeList->getLength() == 0)
    {
        //Éú³ÉÍ·½áµã
        DOMElement* pHeadNode = m_xmlFileAlarm.CreateNewElement(pDoc, XMLTAG_ALARM_HEAD);
        if (pHeadNode == NULL)
        {
            return ERR_FAIL;
        }
        //ÔÚÍ·½áµãÖÐ¼ÓÈëÊôÐÔ
        if (!m_xmlFileAlarm.SetNodeAttrValue(pHeadNode, XMLTAG_ALARM_HEAD_SYN, "0"))
        {
            pHeadNode->release();
            return ERR_FAIL;
        }

        //Ìí¼ÓÍ·½Úµã
        if (!m_xmlFileAlarm.AddChildToNode(pRootEle, pHeadNode))
        {
            pHeadNode->release();
            return ERR_FAIL;
        }
        pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_HEAD);
    }

    if (NULL != pNodeList && pNodeList->getLength() > 0)
    {
        DOMElement* pEleHead = (DOMElement*)pNodeList->item(0);

        //ÉèÖÃÐÂ¸æ¾¯Á÷Ë®ºÅ
        char szBuf[1024];
        sprintf(szBuf, "%ld", uSYN);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleHead, XMLTAG_ALARM_HEAD_SYN, szBuf))
        {
            return ERR_FAIL;
        }

        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

int CAlarmXmlUtil::WriteHeadElementMMLBaseID(UINT4 uBaseID)
{
    //»ñÈ¡DOC¶ÔÏó
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
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

    //°´Í·ÐÅÏ¢¹Ø¼ü×Ö²éÕÒ
    DOMNodeList* pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_HEAD);
    if (NULL == pNodeList || pNodeList->getLength() == 0)
    {
        //Éú³ÉÍ·½áµã
        DOMElement* pHeadNode = m_xmlFileAlarm.CreateNewElement(pDoc, XMLTAG_ALARM_HEAD);
        if (pHeadNode == NULL)
        {
            return ERR_FAIL;
        }
        //ÔÚÍ·½áµãÖÐ¼ÓÈëÊôÐÔ
        if (!m_xmlFileAlarm.SetNodeAttrValue(pHeadNode, XMLTAG_ALARM_HEAD_BASEID, "0"))
        {
            pHeadNode->release();
            return ERR_FAIL;
        }

        //Ìí¼ÓÍ·½Úµã
        if (!m_xmlFileAlarm.AddChildToNode(pRootEle, pHeadNode))
        {
            pHeadNode->release();
            return ERR_FAIL;
        }
        pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_HEAD);
    }

    if (NULL != pNodeList && pNodeList->getLength() > 0)
    {
        DOMElement* pEleHead = (DOMElement*)pNodeList->item(0);

        //ÉèÖÃÐÂ¸æ¾¯Á÷Ë®ºÅ
        char szBuf[1024];
        sprintf(szBuf, "%ld", uBaseID);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleHead, XMLTAG_ALARM_HEAD_BASEID, szBuf))
        {
            return ERR_FAIL;
        }

        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

