#include "perf_xml_util.h"
#include "../include/toolbox.h"
#include "../include/export_dll.h"

extern "C" IGWB_Svc_Export IPerfUtil* CreatePerfUtil()
{
    return new CPerfXmlUtil();
}

//����ģ�鶨��
/***********************************************************************************
                          �����ļ�YYYYMMDD_orig.xml��ʽ
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

//����������DOM�ĵ������ļ�������������ָ���ļ�
int CPerfXmlUtil::OpenFile(const char * const szFilePath)
{
    if(szFilePath != NULL)
    {
        //��ʼ��xml�ļ�����������Ӧ��DOM�����
        BOOL bRet = m_xmlFilePerf.InitXmlFile(szFilePath, XMLTAG_PERF);

        return bRet ? 0 : -1;
    }

    return -1;
}

//�ر��ļ�
int CPerfXmlUtil::CloseFile(void)
{
    return m_xmlFilePerf.ClearDOMDocument() ? 0 : -1;
}

//�����ļ�
int CPerfXmlUtil::Flush(void)
{
    //дxml�ļ�
    if (!m_xmlFilePerf.WriteXmlFile())
    {
        return -1;
    }

    return 0;
}

//׷��һ��������Ϣ
int CPerfXmlUtil::AppendPerfItem(SPerfItem& perfItem)
{
    //��ȡDOC����
    DOMDocument* pDoc = m_xmlFilePerf.GetDOMDocument();
    if (NULL == pDoc)
    {
        return ERR_FAIL;
    }
    //��ȡXML�ļ��Ķ���Ԫ�ض���
    DOMElement* pRootEle = pDoc->getDocumentElement();
    if (NULL == pRootEle)
    {
        return ERR_FAIL;
    }

    //�����ļ������б�
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
    //���̺�
    sprintf(szBuf, "%ld", perfItem.yAPID);
    if (!m_xmlFilePerf.SetNodeAttrValue(pNewEle, XMLTAG_APID, szBuf))
    {
        pNewEle->release();
        return ERR_FAIL;
    }

    //ģ���
    sprintf(szBuf, "%ld", perfItem.yModuleID);
    if (!m_xmlFilePerf.SetNodeAttrValue(pNewEle, XMLTAG_MODULEID, szBuf))
    {
        pNewEle->release();
        return ERR_FAIL;
    }

    //����ͳ����ı��
    sprintf(szBuf, "%ld", perfItem.uID);
    if (!m_xmlFilePerf.SetNodeAttrValue(pNewEle, XMLTAG_ITEMID, szBuf))
    {
        pNewEle->release();
        return ERR_FAIL;
    }

    //ͳ�ƿ�ʼʱ��
    sprintf(szBuf, "%ld", perfItem.timeBegin);
    if (!m_xmlFilePerf.SetNodeAttrValue(pNewEle, XMLTAG_TIMEBEGIN, szBuf))
    {
        pNewEle->release();
        return ERR_FAIL;
    }

    //ͳ�ƽ���ʱ��
    sprintf(szBuf, "%ld", perfItem.timeEnd);
    if (!m_xmlFilePerf.SetNodeAttrValue(pNewEle, XMLTAG_TIMEEND, szBuf))
    {
        pNewEle->release();
        return ERR_FAIL;
    }

    //����ͳ�����ȡֵ
    sprintf(szBuf, "%ld", perfItem.uValue);
    if (!m_xmlFilePerf.SetNodeAttrValue(pNewEle, XMLTAG_VAL, szBuf))
    {
        pNewEle->release();
        return ERR_FAIL;
    }

    //��������ڵ�
    if (!m_xmlFilePerf.AddChildToNode(pRootEle, pNewEle))
    {
        pNewEle->release();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}
