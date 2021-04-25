#include "../include/toolbox.h"
#include "../include/frame.h"
#include "xml_cdr_stat_cfg.h"
#include "../include/export_dll.h"

extern "C" IGWB_Svc_Export ICDRStatCfg* CreateCDRStatCfg()
{
    return new CXmlCDRStatCfg();
}

//����ͳ��ģ�鶨��
/***********************************************************************************
                          ͳ�������ļ�stat_cfg_x.xml��ʽ
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
//����stat_cfg_xxx.xml�ļ�(ͳ��������)��ʹ�õ�XML TAG���
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
�������ƣ�CCDRStatCfg()
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
CXmlCDRStatCfg::CXmlCDRStatCfg()
{
    m_uAccessPoint  = 0;
    m_szFileName[0] = '\0';
}
/*******************************************************************
�������ƣ�~CXmlCDRStatCfg()
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
CXmlCDRStatCfg::~CXmlCDRStatCfg()
{
    
}


/*******************************************************************
�������ƣ�Init(UINT4 uAccessPoint)
�������������ò��������г�ʼ������
���������UINT4 uAccessPoint,������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



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
	sprintf(m_szFileName, "%s/%s%d%s", szPath, "cdr_stat_", m_uAccessPoint, ".xml");
  
	//�ж��ļ��Ƿ����
	BOOL bFileExist = (access(m_szFileName, 0) == 0);
	//��ʼ��XMLFile,��ָ����XML�ļ������ڣ����½�һ��������ָ��������XML�ļ�
	if (!m_xmlFileCfg.InitXmlFile(m_szFileName,XMLTAG_STATCFG_ROOT))
	{
		return ERR_FAIL;
	}

	//��XML�ļ���һ�½��Ҳ���ͷ��Ϣ�ļ����������ͷ��Ϣ
	if (!bFileExist)
	{
		//����ͷ��Ϣ
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
�������ƣ�GetStatItemNameList(LIST<STRING>& listStatItemName)
�����������������ͳ�������Ŀ����
���������LIST<STRING>& listStatItemName,ͳ���������б�
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CXmlCDRStatCfg::GetStatItemNameList(LIST<STRING>& listStatItemName)
{
    //��ȡXML��DOM�ĵ����������Ϸ���
	DOMDocument *pDOMDoc = m_xmlFileCfg.GetDOMDocument();
	if (pDOMDoc == 0)
	{	
		return;
	}
	//�����������
	int nNodesCount;
	
	//��ȡ�������������б�ָ��,�粻�����򷵻�
	DOMNodeList *pDOMNodeList = pDOMDoc->getElementsByTagName(X(XMLTAG_STATCFG_ITEMCFG));
	if (pDOMNodeList == 0 || (nNodesCount = pDOMNodeList->getLength()) <= 0)
	{
		return;
	}
	
	DOMElement *pNode = 0;
	for(int i=0; i<nNodesCount; i++)
	{
		//ȡ������������
		pNode = (DOMElement *)pDOMNodeList->item(i);
		if (pNode != 0)
		{	
			//ȡ�������������ƣ��������б�
			const XMLCh *pValue = pNode->getAttribute(X(XMLTAG_STATCFG_ITEMNAME));	
			listStatItemName.push_back(STRING(S(pValue)));
		}
	}
}

/*******************************************************************
�������ƣ�GetStatItem(const char* const szStatItemName)
�������������ָ��ͳ����Ŀ����ͳ��������Ϣ
���������const char* const szStatItemName,ͳ��������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
SStatItemCfg* CXmlCDRStatCfg::GetStatItem(const char* const szStatItemName)
{
	//�����Ϸ��Լ��
    if (szStatItemName == 0 || szStatItemName[0] == '\0')
	{
		return NULL;
	}
    
	//��ȡXML�ļ���DOM�ĵ�
    DOMDocument *pDOMDoc = m_xmlFileCfg.GetDOMDocument();
	if (pDOMDoc == 0)
	{
		return NULL;
	}
    
	//�����������
	int nNodesCount;

	//��ȡ�������������б�ָ��
	DOMNodeList *pDOMNodeList = pDOMDoc->getElementsByTagName(X(XMLTAG_STATCFG_ITEMCFG));
	if (pDOMNodeList == 0 || (nNodesCount = pDOMNodeList->getLength()) <= 0)
	{
		return NULL;
	}
	
	SStatItemCfg* pStatItemCfg = NULL;
	DOMElement *pNode = 0;

	//�����������������б�������ָ���������ƽ��
	for(int i=0; i<nNodesCount; i++)
	{
		//ȡ�������
		pNode = (DOMElement *)pDOMNodeList->item(i);
		if (pNode == 0) 
		{
			continue;
		}
		
		//��ȡ��������������
		const XMLCh *pItemName = pNode->getAttribute(X(XMLTAG_STATCFG_ITEMNAME));
		if (XMLString::compareString(X(szStatItemName),pItemName) != 0)
		{
			//��ָ�����������ƣ�����������һ���
			continue;
		}

		//ȡ���������������
		const XMLCh *pSubItemCnt = pNode->getAttribute(X(XMLTAG_STATCFG_SUBITEMCOUT));
		int nSubItemCnt = atoi(S(pSubItemCnt));

		//Ϊ���ص���������Ϣ�����ڴ棬�Ա����������������Ϣ
		pStatItemCfg = new (nSubItemCnt)SStatItemCfg;
		sprintf(pStatItemCfg->szStatItemName, "%s", szStatItemName);
		pStatItemCfg->uStatItemCount = nSubItemCnt;
			
		//ȡ�����������б�ָ��
		DOMNodeList *pChildList = pNode->getElementsByTagName(X(XMLTAG_STATCFG_SUBITEMCFG));
		if (pChildList != 0)
		{
			//DOM�ĵ����е��������������
			int nCount = pChildList->getLength();

	        //������������������������е���������������ȣ���ȡ��Сֵ
			if (nCount > nSubItemCnt)
			{
				nCount = nSubItemCnt;
			}
			DOMElement *pChildNode = 0;

			//��������������
			for(int nIndex=0; nIndex < nCount; nIndex++)
			{
				//��nIndex����������Ϣָ��
				SStatSubItemCfg *pSubItemCfg = &(pStatItemCfg->subItemCfg[nIndex]);
				//��nIndex����������
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
		//��ȡ����Ӧ��������Ϣ������
		return pStatItemCfg; 	
	}
	
	return NULL;
}

/*******************************************************************
�������ƣ�GetStatItem(const char* const szStatItemName, 
                      LIST<SOneStatSubItemCfg*>& subItemList)
�������������ָ��ͳ����Ŀ����ͳ��������Ϣ
���������const char* const szStatItemName,ͳ�������� 
		  LIST<SOneStatSubItemCfg*>& subItemList,ͳ�����б�
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CXmlCDRStatCfg::GetStatItem(const char* const szStatItemName, 
							  LIST<SOneStatSubItemCfg*>& subItemList)
{
	//�����Ϸ��Լ��
	if (szStatItemName == 0 || szStatItemName[0] == '\0')
	{
		return;
	}

	//ȡָ�����������Ƶ���������Ϣ
	SStatItemCfg* pStatItemCfg = GetStatItem(szStatItemName);
	if (pStatItemCfg == 0)
	{
		return;
	}

	//�����������
	UINT4 nCount = pStatItemCfg->uStatItemCount;

	//��䷵���б�subItemList	
	SStatSubItemCfg* pTmp;
	for( UINT4 i = 0; i < nCount; i++)
	{
		//��i������������Ϣ
		pTmp = &(pStatItemCfg->subItemCfg[i]);

        //��������������Ϣ
		SOneStatSubItemCfg* pOneCfg = new SOneStatSubItemCfg;
		memcpy(pOneCfg->szStatItemName, pStatItemCfg->szStatItemName, 64);
		memcpy(pOneCfg->szStatSubItemName, pTmp->szStatSubItemName, 64);
		memcpy(pOneCfg->szStatSubItemCond, pTmp->szStatSubItemCond, 512);
        
		//�����б�
		subItemList.push_back(pOneCfg);
	}  

	delete pStatItemCfg;
}


/*******************************************************************
�������ƣ�SetStatCfg(LIST<SOneStatSubItemCfg*> subItemList)
������������������ͳ�������������Ϣ
���������LIST<SOneStatSubItemCfg*> subItemList,ͳ�����б�
          ע���б��Ѱ���������������� 
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
int CXmlCDRStatCfg::SetStatCfg(LIST<SOneStatSubItemCfg*> subItemList)
{
    //��ȡXML��DOM�ĵ� 
    DOMDocument *pDOMDoc = m_xmlFileCfg.GetDOMDocument();
    if (pDOMDoc == 0)
	{
		return ERR_FAIL;
	}
    //�ȱ���DOM�ĵ���ʧ����ָ�ԭDOM�ĵ�
	DOMDocument *pOldDOMDoc = (DOMDocument *)pDOMDoc->cloneNode(true);

	//���DOM�ĵ�����
	if (!m_xmlFileCfg.ClearDOMDocument())
	{
		//ʧ����ָ�ԭDOM�ĵ�
		m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
		return ERR_FAIL;
	}
	
	//�½�DOM�����
    DOMElement *pRoot = m_xmlFileCfg.AddRootElementToDOMDoc(pDOMDoc,XMLTAG_STATCFG_ROOT);
	if (pRoot == 0)
	{
		//ʧ����ָ�ԭDOM�ĵ�
		m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
		return ERR_FAIL;
	}

	//�ڸ�����¼����������ļ�ͷ���
	DOMElement  *pHeadNode = AddHeadInfo(pRoot);
	if (pHeadNode == 0)
	{
		//ʧ����ָ�ԭDOM�ĵ�
		m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
		pHeadNode->release();
		return ERR_FAIL; 
	}
    
	//��һ����������
	char szOldItem[256] = "";
	//��������
	int  nItemCnt = 0;
	//����������
	int  nSubItemCnt = 0;   
	DOMElement *pNewNode = 0;
	char szTmp[256];
	LIST<SOneStatSubItemCfg*>::iterator it = subItemList.begin();
	
	//��������������Ϣ����DOM�����
	for(;it != subItemList.end(); it++)
	{
		SOneStatSubItemCfg *pCfgInfo = (*it);
		//�ж��Ƿ�Ϊ��������
		if (strcmp(szOldItem, pCfgInfo->szStatItemName) != 0)
		{
			//����һ��δ������������������	
			if (pNewNode != 0)
			{
				//Ϊ��һ�����������������������
				sprintf(szTmp,"%d",nSubItemCnt);
				if (!m_xmlFileCfg.SetNodeAttrValue(pNewNode,XMLTAG_STATCFG_SUBITEMCOUT,szTmp))
				{
					//ʧ����ָ�ԭDOM�ĵ�
		            m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
					return ERR_FAIL;
				}
			}
			//������������ĵ�һ����������
			nSubItemCnt = 1;  
			//����һ�����������������������һ
			++nItemCnt;
			strcpy(szOldItem,pCfgInfo->szStatItemName);
			//����һ����������Ϣ��������DOM�����
			pNewNode = m_xmlFileCfg.CreateNewElement(pDOMDoc,XMLTAG_STATCFG_ITEMCFG);
			if (pNewNode == 0)
			{
				//ʧ����ָ�ԭDOM�ĵ�
		        m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
				return ERR_FAIL;
			}
			//Ϊ�����������������������
			if (!m_xmlFileCfg.SetNodeAttrValue(pNewNode,XMLTAG_STATCFG_ITEMNAME,
				pCfgInfo->szStatItemName))
			{
				//ʧ����ָ�ԭDOM�ĵ�
		        m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
				return ERR_FAIL;
			}
			//������������ӵ�DOM�������
			if (!m_xmlFileCfg.AddChildToNode(pRoot,pNewNode))
			{
				//ʧ����ָ�ԭDOM�ĵ�
		        m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
				pNewNode->release();
                return ERR_FAIL;
			}
		}
		else
		{
			//����һ���Ѽ���������������
			++nSubItemCnt;
		}
		//��������������
		DOMElement *pNewSubNode = m_xmlFileCfg.CreateNewElement(pDOMDoc,XMLTAG_STATCFG_SUBITEMCFG);
		if (pNewSubNode == 0)
		{
			return ERR_FAIL;
		}
		//����������
		if (!m_xmlFileCfg.SetNodeAttrValue(pNewSubNode,XMLTAG_STATCFG_SUBITEMNAME,
			pCfgInfo->szStatSubItemName))
		{
			//ʧ����ָ�ԭDOM�ĵ�
		    m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
			pNewSubNode->release();
			return ERR_FAIL;
		}
		//������������
		if (!m_xmlFileCfg.SetNodeAttrValue(pNewSubNode,XMLTAG_STATCFG_SUBITEMCOND,
			pCfgInfo->szStatSubItemCond))
		{
			//ʧ����ָ�ԭDOM�ĵ�
		    m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
			pNewSubNode->release();
			return ERR_FAIL;
		}
		//�����������¼�������������
		if (!m_xmlFileCfg.AddChildToNode(pNewNode,pNewSubNode))
		{
			//ʧ����ָ�ԭDOM�ĵ�
		    m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
			pNewSubNode->release();
			return ERR_FAIL;
		}
	}

	//�������һ�����������
	if (pNewNode != 0)
	{
		sprintf(szTmp,"%d",nSubItemCnt);
		if (!m_xmlFileCfg.SetNodeAttrValue(pNewNode,XMLTAG_STATCFG_SUBITEMCOUT,szTmp))
		{
			//ʧ����ָ�ԭDOM�ĵ�
		    m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
			return ERR_FAIL;
		}
	}

	//�޸��ļ�ͷ������������
	sprintf(szTmp,"%d",nItemCnt);
	if (!m_xmlFileCfg.SetNodeAttrValue(pHeadNode,XMLTAG_STATCFG_ITEMCOUNT,szTmp))
	{
		//ʧ����ָ�ԭDOM�ĵ�
		m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
		return ERR_FAIL;
	}

	//дxml�ļ�
	if (!m_xmlFileCfg.WriteXmlFile())
	{
		//ʧ����ָ�ԭDOM�ĵ�
		m_xmlFileCfg.SetDOMDocument(pOldDOMDoc);
		//��д��ԭ�ļ�
		m_xmlFileCfg.WriteXmlFile();
		return ERR_FAIL;
	}

	//�ɹ�ɾ������DOM�ĵ�
	pOldDOMDoc->release();

	return ERR_SUCCESS;
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
UINT4 CXmlCDRStatCfg::GetAccessPoint()
{
	return m_uAccessPoint;
}


/*******************************************************************
�������ƣ�GetAccessPointName()
������������ý��������
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
const char* CXmlCDRStatCfg::GetAccessPointName()
{
	return m_sAPName.c_str();
}


/*******************************************************************
�������ƣ�DOMElement * CXmlCDRStatCfg::AddHeadInfo(DOMElement *pRootEle)
������������ͳ������XML�ļ���DOM���ĸ���㴦����ͷ��Ϣ���
���������DOMElement *pRootEle��DOM���ĸ����
�����������
�������ݣ��ɹ�����ͷ��Ϣ��㣬����ͷ���ָ�룻���򷵻�NULL
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
DOMElement * CXmlCDRStatCfg::AddHeadInfo(DOMElement *pRootEle)
{
	if (pRootEle == 0)
	{
		return NULL;
	}
	//���DOM�ĵ��Ƿ����
	DOMDocument *pDOMDoc = m_xmlFileCfg.GetDOMDocument();
	if (pDOMDoc == 0)
	{
		return NULL;
	}

	//����ͷ���
	DOMElement  *pHeadNode = m_xmlFileCfg.CreateNewElement(pDOMDoc,XMLTAG_STATCFG_HEAD);
	if (pHeadNode == 0)
	{
		return NULL;
	}
	//�汾����
	if (!m_xmlFileCfg.SetNodeAttrValue(pHeadNode,XMLTAG_STATCFG_VERSION,"0x00000001"))
	{
        pHeadNode->release();
		return NULL; 
	}
	//������
	char szTmp[256];
	sprintf(szTmp,"%d",m_uAccessPoint);
	if (!m_xmlFileCfg.SetNodeAttrValue(pHeadNode,XMLTAG_STATCFG_AP, szTmp))
	{
        pHeadNode->release();
		return NULL; 
	}
	//��������
	if (!m_xmlFileCfg.SetNodeAttrValue(pHeadNode,XMLTAG_STATCFG_ITEMCOUNT,"0"))
	{
        pHeadNode->release();
		return NULL; 
	}
	//�����ֽ�
	if (!m_xmlFileCfg.SetNodeAttrValue(pHeadNode,XMLTAG_STATCFG_RESERVED,"00000000"))
	{
        pHeadNode->release();
		return NULL; 
	}

	//����ͷ��㵽�������
	if (!m_xmlFileCfg.AddChildToNode(pRootEle,pHeadNode))
	{
		pHeadNode->release();
		return NULL; 
	}

	return pHeadNode;
}
