#include "log_xml_util.h"
#include "../include/export_dll.h"

extern "C" IGWB_Svc_Export ILogUtil* CreateLogUtil()
{
    return new CLogXmlUtil();
}

//ÈÕÖ¾Ä£°å¶¨Òå
/***********************************************************************************
                          ÈÕÖ¾ÎÄ¼þYYYYMMDD.xml¸ñÊ½
 -----------------------------------------------------------------------------------
<?xml version="1.0" encoding="ISO8859-1" standalone="no" ?>
<LogRecords>
    <SLog SerialNo="1" LogType="0" UserName="LiPing" IPAddr="10.140.20.1" LogTime="2003-12-05 10:30:20" OperResult="0" OperCmd="LS_PRARSECT" CmdDesc="To get all sections name of allpara file"></SLog>
    <SLog SerialNo="2" LogType="1" UserName="WangLing" IPAddr="10.140.20.1" LogTime="2003-12-05 10:30:22" OperResult="1" OperCmd="LS_PRARINFO" CmdDesc="To get a section para info"></SLog>
</LogRecords>
 -----------------------------------------------------------------------------------

***********************************************************************************/
#define XMLTAG_LOG_ROOT		     "LogRecords"
#define XMLTAG_LOG_ONELOG        "SLog"
#define XMLTAG_LOG_SERIALNO      "SerialNo"
#define XMLTAG_LOG_LOGTYPE       "LogType"
#define XMLTAG_LOG_USERNAME      "UserName"
#define XMLTAG_LOG_IPADDR        "IPAddr"
#define XMLTAG_LOG_LOGTIME       "LogTime"
#define XMLTAG_LOG_OPERRESULT    "OperResult"
#define XMLTAG_LOG_OPERCMD       "OperCmd"
#define XMLTAG_LOG_CMDDESC       "CmdDesc"


CLogXmlUtil::CLogXmlUtil()
{
    m_nReadIndex = 0;
}

CLogXmlUtil::~CLogXmlUtil()
{

}

//ÆÊÎö²¢Éú³ÉDOMÎÄµµÊ÷£¬ÎÄ¼þ²»´æÔÚÔòÉú³ÉÖ¸¶¨ÎÄ¼þ
int CLogXmlUtil::OpenFile(const char * const szFilePath)
{
    if(szFilePath != NULL)
    {
	    //³õÊ¼»¯xmlÎÄ¼þ£¬²¢½¨Á¢ÏàÓ¦µÄDOM½áµãÊ÷
	    BOOL bRet = m_xmlFileLog.InitXmlFile(szFilePath, XMLTAG_LOG_ROOT);

        return bRet ? 0 : -1;
    }

    return -1;
}

//¹Ø±ÕÎÄ¼þ
int CLogXmlUtil::CloseFile(void)
{
    return m_xmlFileLog.ClearDOMDocument() ? 0 : -1;
}

//»ñµÃÊ£ÓàµÄÈÕÖ¾Êý
int CLogXmlUtil::GetRemainCount(void)
{
	//»ñÈ¡¸ù½áµã
	DOMElement *pRootEle = m_xmlFileLog.GetRootElement();
	if (pRootEle == 0)
	{
		return 0;
	}

	DOMNodeList *pNodeList = m_xmlFileLog.GetChildListByTagName(pRootEle,XMLTAG_LOG_ONELOG);
	if (pNodeList == 0)
	{
		return 0;
	}

    int nRemainCount = pNodeList->getLength() - m_nReadIndex - 1;
    return nRemainCount;
}

//Ð´Ò»ÌõÈÕÖ¾ÐÅÏ¢
int  CLogXmlUtil::AppendLog(UINT4       uSerialNo,
                            BYTE        LogType,
                            const char* szUserName,
                            UINT4       uIPAddr,
                            time_t      tLogTime,
                            BYTE        OperResult,
                            const char* szOperCmd,
                            const char* szCmdDesc)
{
    // ½«±¾ÌõÈÕÖ¾Êý¾ÝÐ´Èëµ±Ç°ÈÕÖ¾ÎÄ¼þÖÐ
    DOMDocument *pDOMDoc = m_xmlFileLog.GetDOMDocument();
    if (NULL == pDOMDoc)
    {
        return -1;
    }

	DOMElement *pRootEle = m_xmlFileLog.GetRootElement();
	if (NULL == pRootEle)
	{
		return -1;
	}

	DOMElement *pNewEle = m_xmlFileLog.CreateNewElement(XMLTAG_LOG_ONELOG);
	if (NULL == pNewEle)
	{
		return -1;
	}
    
	char szTmp[256];
	//ÐòÁÐºÅ
	sprintf(szTmp,"%lu",uSerialNo);
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_SERIALNO,szTmp))
	{
		pNewEle->release();
		return -1;
	}
	//ÈÕÖ¾ÀàÐÍ
	sprintf(szTmp,"%d",LogType);
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_LOGTYPE,szTmp))
	{
		pNewEle->release();
		return -1;
	}
	//²úÉúÈÕÖ¾µÄÓÃ»§Ãû
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_USERNAME,szUserName))
	{
		pNewEle->release();
		return -1;
	}
	//MML¿Í»§¶ËµÄIPµØÖ·
	struct in_addr addr;
    addr.s_addr = uIPAddr;
    if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_IPADDR,ACE_OS::inet_ntoa(addr)))
	{
		pNewEle->release();
		return -1;
	}
	//ÈÕÖ¾²úÉúÊ±¼ä
    STRING strTime = ConvertTimeToString(tLogTime);
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_LOGTIME,strTime.c_str()))
	{
		pNewEle->release();
		return -1;
	}
	//²Ù×÷½á¹û
	sprintf(szTmp,"%d",OperResult);
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_OPERRESULT,szTmp))
	{
		pNewEle->release();
		return -1;
	}
	//MMLÃüÁîÃû»òº¯ÊýÃû
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_OPERCMD,szOperCmd))
	{
		pNewEle->release();
		return -1;
	}
	//MMLÃüÁîÃû»òº¯ÊýÃûµÄÏêÏ¸ÃèÊö
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_CMDDESC,szCmdDesc))
	{
		pNewEle->release();
		return -1;
	}

	//ÏÈ±¸·ÝÔ­DOM½áµãÊ÷
	DOMDocument *pOldDOMDoc = (DOMDocument *)pDOMDoc->cloneNode(true);
	if (!m_xmlFileLog.AddChildToNode(pRootEle,pNewEle))
	{
		pNewEle->release();
		m_xmlFileLog.SetDOMDocument(pOldDOMDoc);
		return -1;
	}

	//Ð´xmlÎÄ¼þ
	if (!m_xmlFileLog.WriteXmlFile())
	{
		//Ê§°ÜÔò»Ö¸´Ô­DOMÎÄµµ
		m_xmlFileLog.SetDOMDocument(pOldDOMDoc);
		return -1;
	}

	//³É¹¦É¾³ý±¸·ÝDOMÎÄµµ
	pOldDOMDoc->release();
    
    return 0;
}

//¶ÁÒ»ÌõÈÕÖ¾ÐÅÏ¢
int  CLogXmlUtil::ReadLog(int     whence,
                          UINT4&  uSerialNo,
                          BYTE&   LogType,
                          char*   szUserName,
                          int     nUserNameSize,
                          UINT4&  uIPAddr,
                          time_t& tLogTime,
                          BYTE&   OperResult,
                          char*   szOperCmd,
                          int     nOperCmdSize,
                          char*   szCmdDesc,
                          int     nCmdDescSize)
{
	//»ñÈ¡¸ù½áµã
	DOMElement *pRootEle = m_xmlFileLog.GetRootElement();
	if (NULL == pRootEle)
	{
		return -1;
	}

    int nCount = 0;
	DOMNodeList *pNodeList = m_xmlFileLog.GetChildListByTagName(pRootEle,XMLTAG_LOG_ONELOG);
	if (NULL == pNodeList || (nCount = pNodeList->getLength()) == 0)
	{
		return -1;
	}

	//±éÀúÈÕÖ¾¼ÇÂ¼£¬²éÕÒ·ûºÏÒªÇóµÄ¼ÇÂ¼
    int nRet = -1;
    
    switch(whence) 
    {
    case EL_FIRST:
        m_nReadIndex = 0;
        nRet = ReadOneLog(m_nReadIndex, 
                          uSerialNo,
                          LogType,
                          szUserName,
                          nUserNameSize,
                          uIPAddr,
                          tLogTime,
                          OperResult,
                          szOperCmd,
                          nOperCmdSize,
                          szCmdDesc,
                          nCmdDescSize);
    	break;

    case EL_NEXT:
        if(m_nReadIndex >= nCount)
        {
            return -1;
        }
        m_nReadIndex++;
        nRet = ReadOneLog(m_nReadIndex, 
                          uSerialNo,
                          LogType,
                          szUserName,
                          nUserNameSize,
                          uIPAddr,
                          tLogTime,
                          OperResult,
                          szOperCmd,
                          nOperCmdSize,
                          szCmdDesc,
                          nCmdDescSize);
    	break;

    case EL_FINAL:
        nRet = ReadOneLog(nCount - 1, 
                          uSerialNo,
                          LogType,
                          szUserName,
                          nUserNameSize,
                          uIPAddr,
                          tLogTime,
                          OperResult,
                          szOperCmd,
                          nOperCmdSize,
                          szCmdDesc,
                          nCmdDescSize);
    	break;

    default:
        break;

    }

    return nRet;
}

//¶ÁÒ»ÌõÈÕÖ¾ÐÅÏ¢
//¶ÁÒ»ÌõÈÕÖ¾ÐÅÏ¢
int  CLogXmlUtil::ReadOneLog(int     nIndex,
                             UINT4&  uSerialNo,
                             BYTE&   LogType,
                             char*   szUserName,
                             int     nUserNameSize,
                             UINT4&  uIPAddr,
                             time_t& tLogTime,
                             BYTE&   OperResult,
                             char*   szOperCmd,
                             int     nOperCmdSize,
                             char*   szCmdDesc,
                             int     nCmdDescSize)
{
	//»ñÈ¡¸ù½áµã
	DOMElement *pRootEle = m_xmlFileLog.GetRootElement();
	if (NULL == pRootEle)
	{
		return -1;
	}

    int nCount = 0;
	DOMNodeList *pNodeList = m_xmlFileLog.GetChildListByTagName(pRootEle,XMLTAG_LOG_ONELOG);
	if (NULL == pNodeList || (nCount = pNodeList->getLength()) == 0)
	{
		return -1;
	}

	DOMElement *pNode = (DOMElement *)pNodeList->item(nIndex);
    if (NULL == pNode)
    {
        return -1;
    }

    const XMLCh* pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_LOGTYPE);
	LogType = atoi(S(pNodeValue));

    //È¡ÈÕÖ¾Ê±¼ä
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_LOGTIME);
	tLogTime = ConvertTimeToTimeT(S(pNodeValue));

    //ÓÃ»§Ãû
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_USERNAME);
	strncpy(szUserName,S(pNodeValue),nUserNameSize);
	szUserName[nUserNameSize-1] = '\0';

    //ÐòÁÐºÅ
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_SERIALNO);
	uSerialNo = atol(S(pNodeValue));
    //IPµØÖ·
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_IPADDR);
    uIPAddr = ACE_OS::inet_addr(S(pNodeValue));
    //²Ù×÷½á¹û
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_OPERRESULT);
    OperResult = atoi(S(pNodeValue));
    //MMLÃüÁîÃû»òº¯ÊýÃû
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_OPERCMD);
    strncpy(szOperCmd, S(pNodeValue), nOperCmdSize);
    szOperCmd[nOperCmdSize-1] = '\0';
    //MMLÃüÁîÃû»òº¯ÊýÃûµÄÏêÏ¸ÃèÊö
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_CMDDESC);
    strncpy(szCmdDesc, S(pNodeValue), nCmdDescSize);
    szCmdDesc[nCmdDescSize-1] = '\0'; 

    return 0;
}