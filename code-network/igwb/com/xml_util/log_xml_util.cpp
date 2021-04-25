#include "log_xml_util.h"
#include "../include/export_dll.h"

extern "C" IGWB_Svc_Export ILogUtil* CreateLogUtil()
{
    return new CLogXmlUtil();
}

//��־ģ�嶨��
/***********************************************************************************
                          ��־�ļ�YYYYMMDD.xml��ʽ
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

//����������DOM�ĵ������ļ�������������ָ���ļ�
int CLogXmlUtil::OpenFile(const char * const szFilePath)
{
    if(szFilePath != NULL)
    {
	    //��ʼ��xml�ļ�����������Ӧ��DOM�����
	    BOOL bRet = m_xmlFileLog.InitXmlFile(szFilePath, XMLTAG_LOG_ROOT);

        return bRet ? 0 : -1;
    }

    return -1;
}

//�ر��ļ�
int CLogXmlUtil::CloseFile(void)
{
    return m_xmlFileLog.ClearDOMDocument() ? 0 : -1;
}

//���ʣ�����־��
int CLogXmlUtil::GetRemainCount(void)
{
	//��ȡ�����
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

//дһ����־��Ϣ
int  CLogXmlUtil::AppendLog(UINT4       uSerialNo,
                            BYTE        LogType,
                            const char* szUserName,
                            UINT4       uIPAddr,
                            time_t      tLogTime,
                            BYTE        OperResult,
                            const char* szOperCmd,
                            const char* szCmdDesc)
{
    // ��������־����д�뵱ǰ��־�ļ���
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
	//���к�
	sprintf(szTmp,"%lu",uSerialNo);
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_SERIALNO,szTmp))
	{
		pNewEle->release();
		return -1;
	}
	//��־����
	sprintf(szTmp,"%d",LogType);
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_LOGTYPE,szTmp))
	{
		pNewEle->release();
		return -1;
	}
	//������־���û���
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_USERNAME,szUserName))
	{
		pNewEle->release();
		return -1;
	}
	//MML�ͻ��˵�IP��ַ
	struct in_addr addr;
    addr.s_addr = uIPAddr;
    if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_IPADDR,ACE_OS::inet_ntoa(addr)))
	{
		pNewEle->release();
		return -1;
	}
	//��־����ʱ��
    STRING strTime = ConvertTimeToString(tLogTime);
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_LOGTIME,strTime.c_str()))
	{
		pNewEle->release();
		return -1;
	}
	//�������
	sprintf(szTmp,"%d",OperResult);
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_OPERRESULT,szTmp))
	{
		pNewEle->release();
		return -1;
	}
	//MML������������
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_OPERCMD,szOperCmd))
	{
		pNewEle->release();
		return -1;
	}
	//MML����������������ϸ����
	if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,XMLTAG_LOG_CMDDESC,szCmdDesc))
	{
		pNewEle->release();
		return -1;
	}

	//�ȱ���ԭDOM�����
	DOMDocument *pOldDOMDoc = (DOMDocument *)pDOMDoc->cloneNode(true);
	if (!m_xmlFileLog.AddChildToNode(pRootEle,pNewEle))
	{
		pNewEle->release();
		m_xmlFileLog.SetDOMDocument(pOldDOMDoc);
		return -1;
	}

	//дxml�ļ�
	if (!m_xmlFileLog.WriteXmlFile())
	{
		//ʧ����ָ�ԭDOM�ĵ�
		m_xmlFileLog.SetDOMDocument(pOldDOMDoc);
		return -1;
	}

	//�ɹ�ɾ������DOM�ĵ�
	pOldDOMDoc->release();
    
    return 0;
}

//��һ����־��Ϣ
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
	//��ȡ�����
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

	//������־��¼�����ҷ���Ҫ��ļ�¼
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

//��һ����־��Ϣ
//��һ����־��Ϣ
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
	//��ȡ�����
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

    //ȡ��־ʱ��
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_LOGTIME);
	tLogTime = ConvertTimeToTimeT(S(pNodeValue));

    //�û���
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_USERNAME);
	strncpy(szUserName,S(pNodeValue),nUserNameSize);
	szUserName[nUserNameSize-1] = '\0';

    //���к�
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_SERIALNO);
	uSerialNo = atol(S(pNodeValue));
    //IP��ַ
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_IPADDR);
    uIPAddr = ACE_OS::inet_addr(S(pNodeValue));
    //�������
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_OPERRESULT);
    OperResult = atoi(S(pNodeValue));
    //MML������������
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_OPERCMD);
    strncpy(szOperCmd, S(pNodeValue), nOperCmdSize);
    szOperCmd[nOperCmdSize-1] = '\0';
    //MML����������������ϸ����
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,XMLTAG_LOG_CMDDESC);
    strncpy(szCmdDesc, S(pNodeValue), nCmdDescSize);
    szCmdDesc[nCmdDescSize-1] = '\0'; 

    return 0;
}