#include "../include/toolbox.h"
#include "../include/frame.h"
#include "xml_cdr_stat_rst.h"
#include "../include/export_dll.h"

extern "C" IGWB_Svc_Export ICDRStatRst* CreateCDRStatRst()
{
    return new CXmlCDRStatRst();
}

/***********************************************************************************
                          ͳ�ƽ���ļ�stat_rst_x.xml��ʽ
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
//����stat_rst_xxx.xml�ļ�(ͳ�ƽ��)��ʹ�õ�XML TAG���
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
�������ƣ�CCDRStatRst()
�������������캯��
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
CXmlCDRStatRst::CXmlCDRStatRst()
{
	m_uAccessPoint  = 0;
    m_szFileName[0] = '\0';    
    m_pDoc = NULL;
}

/*******************************************************************
�������ƣ�~CXmlCDRStatRst()
������������������
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
CXmlCDRStatRst::~CXmlCDRStatRst()
{
}

/*******************************************************************
�������ƣ�Init(UINT4 uAccessPoint)
�������������ò��������г�ʼ������
���������UINT4 uAccessPoint,������
�����������
�������ݣ��ɹ�����ERR_SUCCESS,���򷵻�ERR_FAIL
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



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

	//�������ļ��ж�ȡ��ǰ���������
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

    //�����ļ����ļ���
    sprintf(m_szFileName, "%s/%s%d%s", szPath, "stat_rst_", m_uAccessPoint, ".xml");

	//�ж��ļ��Ƿ����
	BOOL bFileExist = (access(m_szFileName, 0) == 0);

	//��ʼ��XMLFile,��ָ����XML�ļ������ڣ����½�һ������Ϊ���Ҳ���ͷ��Ϣ��XML�ļ�
	if (!m_xmlFileRst.InitXmlFile(m_szFileName, XMLTAG_STATRST_ROOT))
	{
		return ERR_FAIL;
	}

	//XML�ļ���һ�½��Ҳ���ͷ��Ϣ�ļ�
	if (!bFileExist)
	{
		DOMDocument *pDOMDoc = m_xmlFileRst.GetDOMDocument();
		if (pDOMDoc == 0)
		{
            FileDel(m_szFileName);
			return ERR_FAIL;
		}

		//����ͷ��Ϣ
		if (!AddHeadInfo(pDOMDoc->getDocumentElement()))
		{
            //���ͷ��Ϣʧ�ܺ�ɾ���ļ�
            FileDel(m_szFileName);
			return ERR_FAIL;
		}
	}

	return ERR_SUCCESS;
}


/*******************************************************************
�������ƣ�GetStatRstHead(SStatRstHead& statRstHead)
�������������ͳ�ƽ���ļ�ͷ��Ϣ
�����������
���������SStatRstHead& statRstHead,ͳ�ƽ���ṹͷ��Ϣ
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CXmlCDRStatRst::GetStatRstHead(SStatRstHead& statRstHead)
{
    DOMElement* pEleHead = GetHeadElement();
    if (!pEleHead)
    {
        return;
    }

    const XMLCh* chr = NULL;

    //�汾
    chr = m_xmlFileRst.GetNodeAttrValue(pEleHead, XMLTAG_STATRST_VERSION);
    statRstHead.uVersion = atoi(S(chr));

    //�����
    chr = m_xmlFileRst.GetNodeAttrValue(pEleHead, XMLTAG_STATRST_AP);
    statRstHead.uAccessPoint = atoi(S(chr));

    //ͳ��ʱ��        
    chr = m_xmlFileRst.GetNodeAttrValue(pEleHead, XMLTAG_STATRST_ENDTIME);
    const char* pszTime = XMLString::transcode(chr);
    statRstHead.tStatEndTime = ConvertTimeToTimeT_YMD(pszTime);

    //�����ֽ�
    chr = m_xmlFileRst.GetNodeAttrValue(pEleHead, XMLTAG_STATRST_RESERVED);
    strncpy((char*)statRstHead.yReserved, S(chr), 8);
}


/*******************************************************************
�������ƣ�GetStatExpList(SStatExpIndexList& statExpIndexList)
�����������������ͳ���������ʽ
�����������
���������SStatExpIndexList& statExpIndexList,����ͳ���������ʽ
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
SStatExpIndexList* CXmlCDRStatRst::GetStatExpList(void)
{    
	SStatExpIndexList* pStatExpIndexList = NULL;

    //��ȡXML�ļ��е�ͷ��Ϣ
    DOMElement* pEleHead = GetHeadElement();
    if (!pEleHead)
    {
        return NULL;
    }

    //��ȡ�������ʽ����
    const XMLCh* chr = m_xmlFileRst.GetNodeAttrValue(pEleHead, XMLTAG_STATRST_EXPCOUNT);
    int nCount = atoi(S(chr));
    if (nCount == 0)
    {
        //����Ϊ0
		pStatExpIndexList = new SStatExpIndexList;
		pStatExpIndexList->uStatExpCount = 0;
		return pStatExpIndexList;
    }
    else
    {
        //����ָ��������С���ڴ�
		pStatExpIndexList = new (nCount)SStatExpIndexList;
    }

    //�����������ʽ����
	pStatExpIndexList->uStatExpCount = nCount;

    //��ȡXML���������ʽ����Ľڵ��б�
	DOMNodeList* pNodeList = m_xmlFileRst.GetChildListByTagName(pEleHead, XMLTAG_STATRST_EXPINDEXMSG);
    DOMElement* pEle = NULL;
    if (pNodeList)
    {
        int nLoop = min(nCount, pNodeList->getLength());
        //ѭ����ֵ
        for (int i = 0; i < nLoop; i++)
        {
            pEle = (DOMElement*)pNodeList->item(i);
            if (!pEle)
            {
                continue;
            }

            //���ʽ
            chr = m_xmlFileRst.GetNodeAttrValue(pEle, XMLTAG_STATRST_EXP);
            strncpy(pStatExpIndexList->expIndex[i].szStatExp, S(chr), 512);

            //�Ƿ�����
            chr = m_xmlFileRst.GetNodeAttrValue(pEle, XMLTAG_STATRST_INURSED);
            pStatExpIndexList->expIndex[i].bInUsed = (BOOL)(atoi(S(chr)));

            //���
            chr = m_xmlFileRst.GetNodeAttrValue(pEle, XMLTAG_STATRST_EXPINDEX);
            pStatExpIndexList->expIndex[i].nStatExpIndex = atoi(S(chr));
        }
    }

	return pStatExpIndexList; 
}


/*******************************************************************
�������ƣ�GetStatExpRst(const char* const szStatExpress, time_t tBeginTime, 
						time_t tEndTime, UINT4& nCDRCount, UINT4& nValueTotal)
�������������ָ��ʱ�䷶Χ��ͳ���������ʽ�Ľ��
���������const char* const szStatExpress,���ʽ 
		  time_t tBeginTime, ��ʼʱ��
		  time_t tEndTime, ����ʱ��
���������UINT4& nCDRCount, ��������
          UINT4& nValueTotal,�ۼ�ֵ
�������ݣ��ɹ�����ERR_SUCCESS,���򷵻�ERR_FAIL
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
int CXmlCDRStatRst::GetStatExpRst(const char* const szStatExpress, time_t tBeginTime, 
						time_t tEndTime, UINT4& nCDRCount, UINT4& nValueTotal)
{
    //����ָ�����ʽ��Ӧ�����
	UINT4 nCurrentIndex = 0;
	if (!FindStatExpress((char*)szStatExpress, nCurrentIndex))
	{
		return ERR_FAIL;
	}

    //��ȡXML�ļ����ݶ���Ľڵ��б�
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

    //ѭ��ÿһ������б����
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

        //�жϸýڵ��ʱ���Ƿ����
		if (nEndTime >= tBeginTime && nEndTime <= tEndTime)
		{
            //���������Ľڵ�����ۼ�����
			FindStatRst(pNode, nCurrentIndex, nCDRCount, nValueTotal);
		}
	}

	return ERR_SUCCESS;
}


/*******************************************************************
�������ƣ�AddStatExpress(LIST<char* >& StatExpList)
�������������ͳ���������ʽ
���������LIST<char* >& StatExpList,���ʽ�б�
�����������
�������ݣ��ɹ�����ERR_SUCCESS,���򷵻�ERR_FAIL
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
int CXmlCDRStatRst::AddStatExpress(LIST<char* >& StatExpList)
{
	DOMDocument* pDoc = m_xmlFileRst.GetDOMDocument();
    if (!pDoc)
    {
        return ERR_FAIL;
    }
    m_pDoc = (DOMDocument*)pDoc->cloneNode(true);

    //��ȡXML�ļ�ͷ��Ϣ����
    DOMElement* pEleHead = GetHeadElement();
    if (!pEleHead)
    {
        m_pDoc->release();
        return ERR_FAIL;
    }
    
    //Ŀǰ�������ʽ����
    UINT4 nStatExpCount = 0;
    //���һ�������ţ�������������
    UINT4 nLastExpMsgIndex = 0;
    UINT4 nTemp = 0;
    char chr[64] ;
    
    //��ȡͷ��Ϣ��Ŀǰ�������ʽ�����б�
    DOMNodeList* pNodeList = m_xmlFileRst.GetChildListByTagName(pEleHead, XMLTAG_STATRST_EXPINDEXMSG); 
    if (pNodeList)
    {
        //���ڱ��ʽ��������InUsed��Ϊ0
        nStatExpCount = pNodeList->getLength();
        DOMNode* pNode = NULL;
        for (int i = 0; i < nStatExpCount; i++)
        {
            pNode = pNodeList->item(i);
            if (!pNode)
            {
                continue;
            }
            
            //��Ϊ0
            if (!m_xmlFileRst.SetNodeAttrValue((DOMElement*)pNode, XMLTAG_STATRST_INURSED, "0"))
            {
                RollBack();
                return ERR_FAIL;
            }
                //���һ��һ�����ʽ��������
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
            //����ԭ�������ҵ��ģ��޸�bInUsedΪ1
            if (!m_xmlFileRst.SetNodeAttrValue((DOMElement*)pNode, XMLTAG_STATRST_INURSED, "1"))
            {
                RollBack();
                return ERR_FAIL;
            }
        }
        else
        {
            //����ԭ�������ڵģ�����һ��            
            DOMElement* pElement = m_xmlFileRst.CreateNewElement(pDoc, XMLTAG_STATRST_EXPINDEXMSG);
            if (!pElement)
            {
                RollBack();
                return ERR_FAIL;
            }
            
            //���ʽ����
            if (!m_xmlFileRst.SetNodeAttrValue(pElement, XMLTAG_STATRST_EXP, *it))
            {
                pElement->release();
                RollBack();
                return ERR_FAIL;
            }
            //�Ƿ�����
            if (!m_xmlFileRst.SetNodeAttrValue(pElement, XMLTAG_STATRST_INURSED, "1"))
            {
                pElement->release();
                RollBack();
                return ERR_FAIL;
            }
            //���
            nLastExpMsgIndex++;
            sprintf(chr, "%d", nLastExpMsgIndex);
            if (!m_xmlFileRst.SetNodeAttrValue(pElement, XMLTAG_STATRST_EXPINDEX, chr))
            {
                pElement->release();
                RollBack();
                return ERR_FAIL;
            }

            //���
            if (!m_xmlFileRst.AddChildToNode(pEleHead, pElement))
            {
                pElement->release();
                RollBack();
                return ERR_FAIL;
            }            
        }

        it++;
    }
    
    //��д���ʽ������,����д���ļ�    
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
        
    //�ͷű�����Դ
    m_pDoc->release();

    //�ύ���ļ���
	if (!m_xmlFileRst.WriteXmlFile())
	{
		return ERR_FAIL;
    }
    
    return ERR_SUCCESS;
}


/*******************************************************************
�������ƣ�AddStatRst(const SAllStatExpRst& statExpRst, time_t statTime)
�������������һ�������ͳ�Ʊ��ʽ��ͳ�ƽ��
���������const SAllStatExpRst& statExpRst,һ���ͳ�ƽ��
�����������
�������ݣ��ɹ�����ERR_SUCCESS,���򷵻�ERR_FAIL
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



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

	//��ȡͷ��Ϣ����
	DOMElement* pHeadEle = GetHeadElement();
	if (pHeadEle == 0)
	{
        m_pDoc->release();
		return ERR_FAIL;
	}
	char szBuf[1024];

    STRING strTime = ConvertTimeToString_YMD(statTime);
    //����ͳ��ʱ��
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadEle, XMLTAG_STATRST_ENDTIME, strTime.c_str()))
	{
        RollBack();
		return ERR_FAIL;
	}
	
	//����ͳ������
    //�����µ�ͳ�ƽ������
	DOMElement* pNewEle = m_xmlFileRst.CreateNewElement(pDoc, XMLTAG_STATRST_CONTENT);
	if (pNewEle == 0)
	{
        RollBack();
		return ERR_FAIL;
	}
	//ͳ�ƽ�ֹʱ��
	//sprintf(szBuf, "%ld", statExpRst.uStatEndTime);
    strTime = ConvertTimeToString_YMD(statExpRst.uStatEndTime);
	if (!m_xmlFileRst.SetNodeAttrValue(pNewEle, XMLTAG_STATRST_STATENDTIME, strTime.c_str()))
	{
        RollBack();
		pNewEle->release();
		return ERR_FAIL;
	}
	//����ͳ��������
	UINT4 nSubCnt = statExpRst.uStatSubItemCount;
	//UINT4 nSubCnt = sizeof(statExpRst)/sizeof(SOneStatExpRst);
	sprintf(szBuf, "%ld", nSubCnt);
	if (!m_xmlFileRst.SetNodeAttrValue(pNewEle, XMLTAG_STATRST_SUBITEMCOUT, szBuf))
	{
        RollBack();
		pNewEle->release();
		return ERR_FAIL;
    }
    //ȡʵ���������
    //nSubCnt = sizeof(statExpRst.ItemRst)/sizeof(SOneStatExpRst);
    //����ͳ������
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
        //ͳ�Ʊ��ʽ������
        sprintf(szBuf, "%ld", statExpRst.ItemRst[i].nStatExpIndex);
        if (!m_xmlFileRst.SetNodeAttrValue(pSubEle, XMLTAG_STATRST_ITEMSTATEXPINDEX, szBuf))
        {
            RollBack();
            pSubEle->release();
            pNewEle->release();
            return ERR_FAIL;
        }
        //��������
        sprintf(szBuf, "%ld", statExpRst.ItemRst[i].nCDRCount);
        if (!m_xmlFileRst.SetNodeAttrValue(pSubEle, XMLTAG_STATRST_CDRCOUNT, szBuf))
        {
            RollBack();
            pSubEle->release();
            pNewEle->release();
            return ERR_FAIL;
        }
        //����ۼ�ֵ
        sprintf(szBuf, "%ld", statExpRst.ItemRst[i].nValueTotal);
        if (!m_xmlFileRst.SetNodeAttrValue(pSubEle, XMLTAG_STATRST_VALUETOTAL, szBuf))
        {
            RollBack();
            pSubEle->release();
            pNewEle->release();
            return ERR_FAIL;
        }
        
        //�������ͳ���ӽڵ�
        if (!m_xmlFileRst.AddChildToNode(pNewEle, pSubEle))
        {
            RollBack();
            pSubEle->release();
            pNewEle->release();
            return ERR_FAIL;
        }
    }
    
    //�������ͳ�ƽڵ�
    if (!m_xmlFileRst.AddChildToNode(pRootEle, pNewEle))
    {
        RollBack();
        pNewEle->release();
		return ERR_FAIL;
	}

    //�ͷű�����Դ
    m_pDoc->release();

    //д�뵽XML�ļ�
	if (!m_xmlFileRst.WriteXmlFile())
	{
		return ERR_FAIL;
	}
	
	return ERR_SUCCESS;
}

/*******************************************************************
�������ƣ�CleanStatExp(void)
��������������ͳ�ƽ���ļ��в���ʹ�õ��������ʽ
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
int CXmlCDRStatRst::CleanStatExp(void)
{
    //�ú���Ŀǰû��Ӧ��
	return ERR_SUCCESS;
}

/*******************************************************************
�������ƣ�FindStatExpress(char* szExpress, UINT4& nIndex)
�������������ݱ��ʽ���������
���������char* szExpress, ���ҵı��ʽ
���������UINT4& nIndex,���ʽ���ֵ
�������ݣ��ҵ�����NODE����,���򷵻�NULL
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
DOMNode* CXmlCDRStatRst::FindStatExpress(char* szExpress, UINT4& nIndex)
{
    BOOL bReturn = FALSE;
	SStatExpIndexList* pStatExpIndexList = NULL;

    //��ȡͷ�ڵ�
    DOMElement* pEleHead = GetHeadElement();
    if (!pEleHead)
    {
        return NULL;
    }

    //��ȡ�������ʽ�б�
    DOMNodeList* pNodeList = m_xmlFileRst.GetChildListByTagName(pEleHead, XMLTAG_STATRST_EXPINDEXMSG); 
    if (!pNodeList)
    {
        return NULL;
    }

    int nListCount = pNodeList->getLength();
    const XMLCh* pCh = NULL;
    DOMNode* pNode = NULL;
    //ѭ������ÿһ���������ʽ
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
�������ƣ�RollBack(void)
�������������дʧ�ܣ��ع�����
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



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
�������ƣ�FindStatRst(SAllStatExpRst* pAllRst, UINT4 nIndex, UINT4& nCDRCount, UINT4& nValueTotal)
���������������е�ͳ�ƽ���и��������Ų�����ͳ�ƽ��ֵ
���������SAllStatExpRst* pAllRst, ����ͳ�ƽ���ṹ
          UINT4 nIndex, ������
���������UINT4& nCDRCount, ��������
          UINT4& nValueTotal,�ۼ�ֵ
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CXmlCDRStatRst::FindStatRst(DOMNode* pNode, UINT4 nIndex, UINT4& nCDRCount, UINT4& nValueTotal)
{
	if (pNode == 0)
	{
		return;
	}
	
    //��ȡ����б�
	DOMNodeList* pDOMChilds = m_xmlFileRst.GetChildListByTagName((DOMElement*)pNode, XMLTAG_STATRST_ONESTATEXPRST);	
	if (!pDOMChilds)
	{
		return;
	}
    
	DOMElement* pChild = NULL;
    const XMLCh* chExpIndex = NULL;

    int nCount = pDOMChilds->getLength();
    //ѭ������ÿһ���������
	for(int i = 0; i < nCount; i++)
	{
		pChild = (DOMElement*)pDOMChilds->item(i);
		if (!pChild)
		{
            continue;
        }
        
        chExpIndex = m_xmlFileRst.GetNodeAttrValue(pChild,XMLTAG_STATRST_ITEMSTATEXPINDEX);
        //�����ͬ���ҵ���Ӧ�����¼
        if (atoi(S(chExpIndex)) == nIndex)
        {
            nCDRCount += atoi(S(m_xmlFileRst.GetNodeAttrValue(pChild, XMLTAG_STATRST_CDRCOUNT)));
            nValueTotal += atoi(S(m_xmlFileRst.GetNodeAttrValue(pChild, XMLTAG_STATRST_VALUETOTAL)));
            return;
        }
    }	
}



/*******************************************************************
�������ƣ�GetAccessPoint()
�������������ؽ�����
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
UINT4 CXmlCDRStatRst::GetAccessPoint()
{
	return m_uAccessPoint;
}


/*******************************************************************
�������ƣ�GetAccessPointName()
�������������ؽ��������
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
const char* CXmlCDRStatRst::GetAccessPointName()
{
	return m_sAPName.c_str();
}

/*******************************************************************
�������ƣ�GetAllExpress(LIST<STRING>& ExpressList)
�������������ݽ����ŷ������б��ʽ
�����������
���������LIST<STRING>& ExpressList,������б��ʽ�б�
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



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
�������ƣ�GetLastStatTime()
��������������ϴ�ͳ��ʱ��
�����������
�����������
�������ݣ��ϴ�ͳ��ʱ��
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
time_t CXmlCDRStatRst::GetLastStatTime()
{
    //��ȡͷ��Ϣ����
	DOMElement* pHeadEle = GetHeadElement();
	if (!pHeadEle)
	{
	    return 0;
	}
	else
	{
        //�����ϴ�ͳ��ʱ��
        const XMLCh* chr = m_xmlFileRst.GetNodeAttrValue(pHeadEle, XMLTAG_STATRST_ENDTIME);
        const char* pszTime = XMLString::transcode(chr);
		return ConvertTimeToTimeT_YMD(pszTime);
	}
}

/*******************************************************************
�������ƣ�AddHeadInfo(DOMElement *pRootEle)
���������������ļ�ͷ��Ϣ��XML�ļ���
���������pRootEle��DOM�ļ�����
�����������
�������ݣ�DOMElement��ӳɹ�����ͷ��Ϣ���󣬷��򷵻�NULL
���ú�����
����������
����ʱ�䣺 
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



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

	//����ͷ���
	DOMElement* pHeadNode = m_xmlFileRst.CreateNewElement(pDOMDoc, XMLTAG_STATRST_HEAD);
	if (pHeadNode == 0)
	{
        m_pDoc->release();
		return NULL;
	}
	//��ͷ����м�������
	//�汾����
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadNode, XMLTAG_STATRST_VERSION, "0x00000001"))
	{
        RollBack();
        pHeadNode->release();
		return NULL; 
	}
	//������
	char szTmp[256];
	sprintf(szTmp, "%d", m_uAccessPoint);
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadNode, XMLTAG_STATRST_AP, szTmp))
	{
        RollBack();
        pHeadNode->release();
		return NULL; 
	}
    //ͳ�ƽ�ֹʱ��
    time_t curtime = ACE_OS::time(NULL);
    STRING strTime = ConvertTimeToString_YMD(curtime);
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadNode, XMLTAG_STATRST_ENDTIME, strTime.c_str()))
	{
        RollBack();
        pHeadNode->release();
		return NULL; 
	}
	//�����ֽ�
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadNode, XMLTAG_STATRST_RESERVED, "00000000"))
	{
        RollBack();
        pHeadNode->release();
		return NULL; 
	}
    //ͳ�Ʊ��ʽ����
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadNode, XMLTAG_STATRST_EXPCOUNT, "0"))
	{
        RollBack();
        pHeadNode->release();
		return NULL; 
	}

	//����ͷ��㵽�������
	if (!m_xmlFileRst.AddChildToNode(pRootEle, pHeadNode))
	{
        RollBack();
		pHeadNode->release();
		return NULL; 
	}

    //�ύ���ļ�
    if (!m_xmlFileRst.WriteXmlFile())
    {
		pHeadNode->release();
		return NULL; 
    }
	return pHeadNode;
}

/*******************************************************************
�������ƣ�GetHeadElement()
������������ȡͷ�ڵ�
�����������
�����������
�������ݣ�DOMElement��ȡ�ɹ�����ͷ��Ϣ���󣬷��򷵻�NULL
���ú�����
����������
����ʱ�䣺 
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
DOMElement* CXmlCDRStatRst::GetHeadElement()
{
    //��ȡDOC����
    DOMDocument* pDoc = m_xmlFileRst.GetDOMDocument();
    if (!pDoc)
    {
        return NULL;
    }
    //��ȡXML�ļ��Ķ���Ԫ�ض���
    DOMElement* pEle = pDoc->getDocumentElement();
    if (!pEle)
    {
        return NULL;
    }

    //��ͷ��Ϣ�ؼ��ֲ���
    DOMNodeList* pNodeList = m_xmlFileRst.GetChildListByTagName(pEle, XMLTAG_STATRST_HEAD);
    if (pNodeList && pNodeList->getLength() > 0)
    {
        return (DOMElement*)pNodeList->item(0);
    }

    return NULL;
}

/*******************************************************************
�������ƣ�GetContentList()
������������ȡ�ļ������б�
�����������
�����������
�������ݣ�DOMNodeList��ȡ�ɹ������ļ������б�ָ�룬���򷵻�NULL
���ú�����
����������
����ʱ�䣺 
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
DOMNodeList* CXmlCDRStatRst::GetContentList()
{
    //��ȡDOC����
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

    //�����ļ������б�
    return m_xmlFileRst.GetChildListByTagName(pEle, XMLTAG_STATRST_CONTENT);
}

//ˢ����ִ��ͳ�Ʋ�����ʱ��
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

	//��ȡͷ��Ϣ����
	DOMElement* pHeadEle = GetHeadElement();
	if (pHeadEle == 0)
	{
        m_pDoc->release();
		return ERR_FAIL;
	}

    STRING strTime = ConvertTimeToString_YMD(curtime);
    //����ͳ��ʱ��
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadEle, XMLTAG_STATRST_ENDTIME, strTime.c_str()))
	{
        RollBack();
		return ERR_FAIL;
	}
	
    //�ͷű�����Դ
    m_pDoc->release();

    //д�뵽XML�ļ�
	if (!m_xmlFileRst.WriteXmlFile())
	{
		return ERR_FAIL;
	}

    return ERR_SUCCESS;
}
