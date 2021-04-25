#include "m2000_log_xml.h"
#include "../include/export_dll.h"

extern "C" IGWB_Svc_Export ILogUtil* CreateM2000Log()
{
    return new CM2000LogXml();
}

//��־ģ�嶨��
/***********************************************************************************
                          ��־�ļ�YYYYMMDD.xml��ʽ
 -----------------------------------------------------------------------------------
<?xml version="1.0" encoding="iso-8859-1"?>
<!-- edited with XMLSPY v5 U (http://www.xmlspy.com) by et8 (et8) -->
<LogFile xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation=".\OperationLog.xsd" StartTime="2004-02-12T12:00:00+08:00" EndTime="2004-02-12T14:00:00+08:00">
    <Log Operation="add brd:..." User="Us1" StartTime="2004-02-12T12:01:00+08:00" EndTime="2004-02-12T12:03:00+08:00" terminal="127.0.0.1" result="Success">One board is added.</Log>
    <Log Operation="crt job:..." User="Us1" StartTime="2004-02-12T12:03:01+08:00" EndTime="2004-02-12T12:03:08+08:00" terminal="127.0.0.1" result="failed">can not create job due to...</Log>
</LogFile>
 -----------------------------------------------------------------------------------

***********************************************************************************/
#define M2000_XMLTAG_LOG_ROOT          "LogFile"
#define M2000_XMLTAG_LOG_ONELOG        "Log"
#define M2000_XMLTAG_LOG_OPERCMD       "Operation"
#define M2000_XMLTAG_LOG_USERNAME      "User"
#define M2000_XMLTAG_LOG_LOGTIME       "StartTime"
#define M2000_XMLTAG_LOG_LOGTIME_E     "EndTime"
#define M2000_XMLTAG_LOG_IPADDR        "terminal"
#define M2000_XMLTAG_LOG_OPERRESULT    "result"
#define M2000_XMLTAG_LOG_XSI           "xmlns:xsi"
#define M2000_XMLTAG_LOG_XSI_VALUE     "http://www.w3.org/2001/XMLSchema-instance"
#define M2000_XMLTAG_LOG_NL            "xsi:noNamespaceSchemaLocation"
#define M2000_XMLTAG_LOG_NL_VALUE      ".\\OperationLog.xsd"


CM2000LogXml::CM2000LogXml()
{
    m_nReadIndex   = 0;
    m_strStartTime = "";
    m_strEndTime   = "";
}

CM2000LogXml::~CM2000LogXml()
{

}

//����������DOM�ĵ������ļ�������������ָ���ļ�
int CM2000LogXml::OpenFile(const char * const szFilePath)
{
    if(szFilePath != NULL)
    {
        //��ʼ��xml�ļ�����������Ӧ��DOM�����
        BOOL bRet = m_xmlFileLog.InitXmlFile(szFilePath, M2000_XMLTAG_LOG_ROOT);
        if(!bRet)
        {
            return -1;
        }

        //by ldf OM ���ģ���xml�����UTF-8��Ϊso-8859-1
        //m_xmlFileLog.SetFontSet("");

        return 0;
    }

    return -1;
}

//�ر��ļ�
int CM2000LogXml::CloseFile(void)
{
    if(m_strStartTime != "")
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

        if (!m_xmlFileLog.SetNodeAttrValue(pRootEle,
                                           M2000_XMLTAG_LOG_XSI,
                                           M2000_XMLTAG_LOG_XSI_VALUE))
        {
            pRootEle->release();
            return -1;
        }

        if (!m_xmlFileLog.SetNodeAttrValue(pRootEle,
                                           M2000_XMLTAG_LOG_NL,
                                           M2000_XMLTAG_LOG_NL_VALUE))
        {
            pRootEle->release();
            return -1;
        }

        if (!m_xmlFileLog.SetNodeAttrValue(pRootEle,
                                           M2000_XMLTAG_LOG_LOGTIME,
                                           m_strStartTime.c_str()))
        {
            pRootEle->release();
            return -1;
        }

        if (!m_xmlFileLog.SetNodeAttrValue(pRootEle,
                                           M2000_XMLTAG_LOG_LOGTIME_E,
                                           m_strEndTime.c_str()))
        {
            pRootEle->release();
            return -1;
        }

        //дxml�ļ�
        if (!m_xmlFileLog.WriteXmlFile())
        {
            pRootEle->release();
            return -1;
        }
    }

    return m_xmlFileLog.ClearDOMDocument() ? 0 : -1;
}

//���ʣ�����־��
int CM2000LogXml::GetRemainCount(void)
{
    //��ȡ�����
    DOMElement *pRootEle = m_xmlFileLog.GetRootElement();
    if (pRootEle == 0)
    {
        return 0;
    }

    DOMNodeList *pNodeList = m_xmlFileLog.GetChildListByTagName(pRootEle,M2000_XMLTAG_LOG_ONELOG);
    if (pNodeList == 0)
    {
        return 0;
    }

    int nRemainCount = pNodeList->getLength() - m_nReadIndex - 1;
    return nRemainCount;
}

//дһ����־��Ϣ
int CM2000LogXml::AppendLog(UINT4       uSerialNo,
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

    DOMElement *pNewEle = m_xmlFileLog.CreateNewElement(M2000_XMLTAG_LOG_ONELOG);
    if (NULL == pNewEle)
    {
        return -1;
    }

    char szTmp[256];
    //MML������������
    if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,M2000_XMLTAG_LOG_OPERCMD,szOperCmd))
    {
        pNewEle->release();
        return -1;
    }
    //������־���û���
    if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,M2000_XMLTAG_LOG_USERNAME,szUserName))
    {
        pNewEle->release();
        return -1;
    }
    //��־����ʱ��
    struct tm tmLocalTime = *localtime(&tLogTime);
    struct tm tmGMTime  = *gmtime(&tLogTime);
    char szTemp[2048];
    strftime(szTemp,
             sizeof(szTemp),
             "%Y-%m-%dT%H:%M:%S",
             &tmGMTime);
    szTemp[sizeof(szTemp) - 1] = '\0';
    STRING strTime = szTemp;
    long tzone = (mktime(&tmLocalTime) - mktime(&tmGMTime))/3600;
    if(tzone > 0)
    {
        SNPRINTF(szTemp,
                 sizeof(szTemp),
                 "+%02d:00",
                 tzone);
    }
    else
    {
        SNPRINTF(szTemp,
                 sizeof(szTemp),
                 "-%02d:00",
                 -tzone);
    }
    szTemp[sizeof(szTemp) - 1] = '\0';
    strTime += szTemp;

    if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,M2000_XMLTAG_LOG_LOGTIME,strTime.c_str()))
    {
        pNewEle->release();
        return -1;
    }
    if(m_strStartTime == "")
    {
        m_strStartTime = strTime;
    }

    //��־����ʱ��endtime
    if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,M2000_XMLTAG_LOG_LOGTIME_E,strTime.c_str()))
    {
        pNewEle->release();
        return -1;
    }
    m_strEndTime = strTime;

    //MML�ͻ��˵�IP��ַ
    struct in_addr addr;
    addr.s_addr = uIPAddr;
    if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,M2000_XMLTAG_LOG_IPADDR,ACE_OS::inet_ntoa(addr)))
    {
        pNewEle->release();
        return -1;
    }
    //�������
    if(OperResult == 0)
    {
        strcpy(szTmp,"success");
    }
    else
    {
        strcpy(szTmp,"failed");
    }

    if (!m_xmlFileLog.SetNodeAttrValue(pNewEle,M2000_XMLTAG_LOG_OPERRESULT,szTmp))
    {
        pNewEle->release();
        return -1;
    }

    if(OperResult == 0)
    {
        SNPRINTF(szTmp,
                 sizeof(szTmp),
                 "%s(success)",
                 szCmdDesc);
    }
    else
    {
        SNPRINTF(szTmp,
                 sizeof(szTmp),
                 "%s(failed)",
                 szCmdDesc);
    }
    if (!m_xmlFileLog.setNodeValue(pNewEle, szTmp))
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
int CM2000LogXml::ReadLog(int     whence,
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
    DOMNodeList *pNodeList = m_xmlFileLog.GetChildListByTagName(pRootEle,M2000_XMLTAG_LOG_ONELOG);
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
int  CM2000LogXml::ReadOneLog(int     nIndex,
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
    DOMNodeList *pNodeList = m_xmlFileLog.GetChildListByTagName(pRootEle,M2000_XMLTAG_LOG_ONELOG);
    if (NULL == pNodeList || (nCount = pNodeList->getLength()) == 0)
    {
        return -1;
    }

    DOMElement *pNode = (DOMElement *)pNodeList->item(nIndex);
    if (NULL == pNode)
    {
        return -1;
    }

    //const XMLCh* pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,M2000_XMLTAG_LOG_LOGTYPE);
    LogType = 0;

    //ȡ��־ʱ��
    const XMLCh* pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,M2000_XMLTAG_LOG_LOGTIME);
    tLogTime = ConvertTimeToTimeT(S(pNodeValue));

    //�û���
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,M2000_XMLTAG_LOG_USERNAME);
    strncpy(szUserName,S(pNodeValue),nUserNameSize);
    szUserName[nUserNameSize-1] = '\0';

    //���к�
    //pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,M2000_XMLTAG_LOG_SERIALNO);
    uSerialNo = 0;

    //IP��ַ
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,M2000_XMLTAG_LOG_IPADDR);
    uIPAddr = ACE_OS::inet_addr(S(pNodeValue));

    //�������
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,M2000_XMLTAG_LOG_OPERRESULT);
    if(strcmp(S(pNodeValue), "success") == 0)
    {
        OperResult = 0;//atoi(S(pNodeValue));
    }
    else
    {
        OperResult = -1;//atoi(S(pNodeValue));
    }

    //MML������������
    pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,M2000_XMLTAG_LOG_OPERCMD);
    strncpy(szOperCmd, S(pNodeValue), nOperCmdSize);
    szOperCmd[nOperCmdSize-1] = '\0';

    //MML����������������ϸ����
    //pNodeValue = m_xmlFileLog.GetNodeAttrValue(pNode,M2000_XMLTAG_LOG_CMDDESC);
    //strncpy(szCmdDesc, S(pNodeValue), nCmdDescSize);
    szCmdDesc[0] = '\0';

    return 0;
}