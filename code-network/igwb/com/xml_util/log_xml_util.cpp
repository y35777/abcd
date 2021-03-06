#include "log_xml_util.h"
#include "../include/export_dll.h"

extern "C" IGWB_Svc_Export ILogUtil* CreateLogUtil()
{
    return new CLogXmlUtil();
}

//日志模板定义
/***********************************************************************************
                          日志文件YYYYMMDD.xml格式
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

//剖析并生成DOM文档树，文件不存在则生成指定文件
int CLogXmlUtil::OpenFile(const char * const szFilePath)
{
    if(szFilePath != NULL)
    {
	    //初始化xml文件，并建立相应的DOM结点树
	    BOOL bRet = m_xmlFileLog.InitXmlFile(szFilePath, XMLTAG_LOG_ROOT);

        return bRet ? 0 : -1;
    }

    return -1;
}

//关闭文件
int CLogXmlUtil::CloseFile(void)
{
    return m_xmlFileLog.ClearDOMDocument() ? 0 : -1;
}

//获得剩余的日志数
int CLogXmlUtil::GetRemainCount(void)
{
	//获取根结点
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

//写一条日志信息
int  CLogXmlUtil::AppendLog(UINT4       uSerialNo,
                            BYTE        LogType,
                            const char* szUserName,
                            UINT4       uIPAddr,
                            time_t      tLogTime,
                            BYTE        OperResult,
                            const char* szOperCmd,
                            const char* szCmdDesc)
{
    // 将本条日志数据写入当前日志文件中
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
	//序列号
	sprintf(szTmp,"%lu",uSerialNo);
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_SERIALNO,szTmp))
	{
		pNewEle->release();
		return -1;
	}
	//日志类型
	sprintf(szTmp,"%d",LogType);
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_LOGTYPE,szTmp))
	{
		pNewEle->release();
		return -1;
	}
	//产生日志的用户名
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_USERNAME,szUserName))
	{
		pNewEle->release();
		return -1;
	}
	//MML客户端的IP地址
	struct in_addr addr;
    addr.s_addr = uIPAddr;
    if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_IPADDR,ACE_OS::inet_ntoa(addr)))
	{
		pNewEle->release();
		return -1;
	}
	//日志产生时间
    STRING strTime = ConvertTimeToString(tLogTime);
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_LOGTIME,strTime.c_str()))
	{
		pNewEle->release();
		return -1;
	}
	//操作结果
	sprintf(szTmp,"%d",OperResult);
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_OPERRESULT,szTmp))
	{
		pNewEle->release();
		return -1;
	}
	//MML命令名或函数名
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_OPERCMD,szOperCmd))
	{
		pNewEle->release();
		return -1;
	}
	//MML命令名或函数名的详细描述
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_CMDDESC,szCmdDesc))
	{
		pNewEle->release();
		return -1;
	}

	//先备份原DOM结点树
	DOMDocument *pOldDOMDoc = (DOMDocument *)pDOMDoc->cloneNode(true);
	if (!m_xmlFileLog.AddChildToNode(pRootEle,pNewEle))
	{
		pNewEle->release();
		m_xmlFileLog.SetDOMDocument(pOldDOMDoc);
		return -1;
	}

	//写xml文件
	if (!m_xmlFileLog.WriteXmlFile())
	{
		//失败则恢复原DOM文档
		m_xmlFileLog.SetDOMDocument(pOldDOMDoc);
		return -1;
	}

	//成功删除备份DOM文档
	pOldDOMDoc->release();
    
    return 0;
}

//读一条日志信息
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
	//获取根结点
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

	//遍历日志记录，查找符合要求的记录
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

//读一条日志信息
//读一条日志信息
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
	//获取根结点
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

    //取日志时间
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_LOGTIME);
	tLogTime = ConvertTimeToTimeT(S(pNodeValue));

    //用户名
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_USERNAME);
	strncpy(szUserName,S(pNodeValue),nUserNameSize);
	szUserName[nUserNameSize-1] = '\0';

    //序列号
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_SERIALNO);
	uSerialNo = atol(S(pNodeValue));
    //IP地址
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_IPADDR);
    uIPAddr = ACE_OS::inet_addr(S(pNodeValue));
    //操作结果
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_OPERRESULT);
    OperResult = atoi(S(pNodeValue));
    //MML命令名或函数名
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_OPERCMD);
    strncpy(szOperCmd, S(pNodeValue), nOperCmdSize);
    szOperCmd[nOperCmdSize-1] = '\0';
    //MML命令名或函数名的详细描述
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_CMDDESC);
    strncpy(szCmdDesc, S(pNodeValue), nCmdDescSize);
    szCmdDesc[nCmdDescSize-1] = '\0'; 

    return 0;
}