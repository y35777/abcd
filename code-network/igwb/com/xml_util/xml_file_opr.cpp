//�ļ�����xml_file_opr.cpp
//���ܣ�  ʵ��xml�ļ�������CXmlFileOpr

#include "xml_file_opr.h"
#include <iostream>
#include <locale.h>

//ʹ��Xercesͷ�ļ�
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

//���з�
static const XMLCh gNewLine[] = { chCR, chLF, chNull };

//DOM����������
static const XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };

//�����ྲ̬����
int CXmlFileOpr::g_nRefCount = 0;

ACE_Mutex CXmlFileOpr::g_MutexRefCnt;

////////////////////////////////////////////////////////////////////
//CXmlFileOpr implementation
////////////////////////////////////////////////////////////////////

/*******************************************************************
�������ƣ�CXmlFileOpr()
�������������캯��
�����������
�����������
�������ݣ��ޣ�
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
CXmlFileOpr::CXmlFileOpr()
     :m_pDOMBuilder(0), m_pDOMDoc(0), m_pDOMimpl(0), 
	  m_bInitXerces(FALSE), m_bParsedDoc(FALSE)
{
	m_szFilePath[0]   = '\0';	
	m_szRootName[0]   = '\0';
	m_szModuleName[0] = '\0';
	m_szFontSet[0]    = '\0';
}

/*******************************************************************
�������ƣ�~CXmlFileOpr()
������������������
�����������
�����������
�������ݣ��ޣ�
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
CXmlFileOpr::~CXmlFileOpr()
{
	//��DOM�ĵ����ڲ��Ҳ����ڷ��������У������ͷ����ڴ�
	if (!m_bParsedDoc && m_pDOMDoc != 0)
	{
		m_pDOMDoc->release();
	    m_pDOMDoc = 0;	
	}
    
	//�ͷŷ�������Դ�����Զ��ͷ���������DOM�ĵ�
	if (m_pDOMBuilder != 0)
	{
		m_pDOMBuilder->release();
		m_pDOMBuilder = 0;
	}

    //����Xerces����
    if (m_bInitXerces)
    {
		g_MutexRefCnt.acquire();
        
        //����ʹ�ü���
        --g_nRefCount;
        
        //ʹ�ü�������0��ʾ�������߳�����ʹ��Xerces,���ͷ�����Դ
        if (g_nRefCount == 0)
        {
			UninitXMLPlatFormUtils();

        }

        g_MutexRefCnt.release();
    }
}

/*******************************************************************
�������ƣ�DOMDocument * GetDOMDocument()
������������ȡDOM�ĵ�ָ��
�����������
�����������
�������ݣ�����m_pDOMDoc��
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
DOMDocument * CXmlFileOpr::GetDOMDocument()
{
	return m_pDOMDoc;
}


/*******************************************************************
�������ƣ�BOOL InitXMLPlatFormUtils()
������������ʼ��Xerecs
�����������
�����������
�������ݣ���ʼ���ɹ�����TRUE,ʧ�ܷ���FALSE��
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CXmlFileOpr::InitXMLPlatFormUtils()
{
	//�ѳ�ʼ��Xerces������
    if (m_bInitXerces)
    {
        return TRUE;
    }
    
	g_MutexRefCnt.acquire();
    
    //�ж�ʹ�ü����Ƿ����0������0��ʾ�����̳߳�ʼ����Xerces���������ʼ��
    if (g_nRefCount <= 0)
    {
        //��ʼ��Xerces
        try
        {
            XMLPlatformUtils::Initialize();
        }
        catch(const XMLException& tocatch)
        {
            g_MutexRefCnt.release();
            //������
		    STRING strError = GetXMLExceptionMsg(tocatch);
		    DoError("%s,%s","initialization XMLPlatFormUtils error", strError.c_str());
		    return FALSE;
        }
    }

    //����ʹ�ü���
    ++g_nRefCount;

    g_MutexRefCnt.release();

    m_bInitXerces = TRUE;

 	//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
	//�õ�DOMImplementation����ӿ�
	try
	{
		m_pDOMimpl = DOMImplementationRegistry::getDOMImplementation(gLS);
	}
	catch (...)
	{
		DoError("%s","DOMImplementationRegistry::getDOMImplementation() error");
		return FALSE;
	}
	//the end	2004-5-20	��Ӧ���ⵥSWPD05244

	return (m_pDOMimpl != 0);
}

/*******************************************************************
�������ƣ�void UninitXMLPlatFormUtils()
�����������ͷ�Xerecs��Դ
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CXmlFileOpr::UninitXMLPlatFormUtils()
{
	//�ͷ�Xercesʹ�õ���Դ
	if (m_bInitXerces)
	{
		//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
		try
		{
			XMLPlatformUtils::Terminate();
		}
		catch (...)
		{
			DoError("%s","XMLPlatformUtils::Terminate() error");
		}
		//the end	2004-5-20	��Ӧ���ⵥSWPD05244

		m_bInitXerces = FALSE;
	}
}

/*******************************************************************
�������ƣ�BOOL InitXmlFile(const char * const szFilePath,
							  const char * const szRootName)
������������ȡָ��XML�ļ���������DOM���������ָ���ļ������ڣ����½�
          һ����������XML�ļ�
���������const char * const szFilePath,  XML�ļ�ȫ·������
          const char * const szRootName,  XML�ļ��ĸ�����ǣ�
�����������
�������ݣ���ʼ���ɹ�����TRUE,ʧ�ܷ���FALSE��
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CXmlFileOpr::InitXmlFile(const char * const szFilePath,
							  const char * const szRootName)
{
    if(m_szFontSet[0] == '\0')
    {
        CINIFile iniFile(GetCfgFilePath());
        iniFile.Open();
        iniFile.GetString(CFG_SEC_COMMON, 
                          CFG_XML_FONT_SET, 
                          CFG_XML_FONT_SET_DEFAULT,
                          m_szFontSet,
                          sizeof(m_szFontSet));
        m_szFontSet[sizeof(m_szFontSet) - 1] = '\0';
    }

    const char* p = S_LOG_SERIALNO;
    if((BYTE)p[0] >= 0x80)
    {
#ifdef _PLATFORM_WIN32
        setlocale(LC_CTYPE, "chs");
#else //_PLATFORM_WIN32
        setlocale(LC_CTYPE, "china");
#endif //_PLATFORM_WIN32
    }

	//������Ч�Լ��
	if (szFilePath == 0 || szRootName == 0)
	{
		return FALSE;
	}
	//�ļ�������������Ʋ���Ϊ��
	if (szFilePath[0] == '\0' || szRootName[0] == '\0')
	{
		return FALSE;
	}

	STRNCPY(m_szFilePath, szFilePath, MAX_PATH);
	STRNCPY(m_szRootName, szRootName, MAX_PATH);

	//XML�ļ�URI��֧��'\'�ַ����滻��'/'
	int nLen = strlen(m_szFilePath);
	for(int i=0; i<nLen; i++)
	{
		if (m_szFilePath[i] == '\\')
		{
			m_szFilePath[i] = '/';
		}
	}
	
	//�ж��ļ��Ƿ����
	BOOL bFileExist = (access(m_szFilePath, 0) == 0);
    if (!bFileExist)
    {
        STRING szFilePath(m_szFilePath);
        int nPos  = szFilePath.find_last_of('/');
#ifdef _PLATFORM_WIN32
        int nPos2 = szFilePath.find_last_of('\\');
#else //_PLATFORM_WIN32
        int nPos2 = -1;
#endif //_PLATFORM_WIN32
        nPos      = max(nPos, nPos2);
        if (nPos <= 0)
        {
            return ERR_FAIL;
        }

        STRING szDirPath = szFilePath.substr(0,nPos);
        
        //�����ļ�����Ŀ¼
		BOOL bCreateDeepDir = CreateDeepDir(szDirPath.c_str());

        if (!bCreateDeepDir)
        {
            return FALSE;
        }
    }

	//��δ��ʼ��Xerces�����ȳ�ʼ��
	if (!m_bInitXerces)
	{
		if (!InitXMLPlatFormUtils())
		{
			return FALSE;
		}
	}

	BOOL bReturn = FALSE;
	//����ļ��Ƿ����
    if (!bFileExist)
	{		
		bReturn = CreateNullXmlFile();
	}
	else
	{   
		//�ļ����ڣ������XML�ļ��������ĵ������
		bReturn = CreateDOMDocumentTree();
    }

	return bReturn;
}

/*******************************************************************
�������ƣ�BOOL CreateDOMDocumentTree()
��������������XML�ļ���������DOM�����
�����������
�����������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE��
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CXmlFileOpr::CreateDOMDocumentTree()
{
	//����ļ����Ƿ�Ϊ��
	if (m_szFilePath[0] == '\0')
	{
		return FALSE;
	}
	
	//����DOM������
	if (m_pDOMBuilder == 0)
	{
		//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
		try
		{
			m_pDOMBuilder = ((DOMImplementationLS *)m_pDOMimpl)->createDOMBuilder(
				DOMImplementationLS::MODE_SYNCHRONOUS, 0);
		}
		catch (...)
		{
			DoError("%s","DOMimpl->createDOMBuilder() error");
			return FALSE;
		}
		//the end	2004-5-20	��Ӧ���ⵥSWPD05244

		if (m_pDOMBuilder == 0)
		{
			return FALSE;
		}
	}
	else
	{
		//DOM���������ڣ�����շ�����DOM�ĵ����黺��
		try
		{
			m_pDOMBuilder->resetDocumentPool(); 
		}
		catch(...)
		{
			//������
		    DoError("%s","when creating DOM tree, resetDocumentPool() error");
			
			return FALSE;
		}
	}
	//���ԭDOM�ĵ�
	if (m_pDOMDoc != 0 && !m_bParsedDoc)
	{
		m_pDOMDoc->release();
	}
	m_pDOMDoc = 0;
    //���÷���������

	//
	try
	{
		m_pDOMDoc = m_pDOMBuilder->parseURI(m_szFilePath);
		if (m_pDOMDoc == 0)
		{
			return FALSE;
		}
        //DOM�ĵ�����������У��ɷ����������ͷ�����Դ
		m_bParsedDoc = TRUE;
	}
	catch (const XMLException& tocatch)
	{
		//������
		STRING strError = GetXMLExceptionMsg(tocatch);
		DoError("%s%s,%s","Error during parsing:",m_szFilePath,strError.c_str());

		return FALSE;
	}
	catch (const DOMException& tocatch)
	{
		//������
		STRING strError = GetDOMExceptionMsg(tocatch);
		DoError("%s%s,%s","Error during parsing:",m_szFilePath,strError.c_str());

		return FALSE;
	}
	catch (...)
	{
		//������
		DoError("%s%s","Unexpected exception during parsing:",m_szFilePath);
		
		return FALSE;
	}

	return TRUE;
}

/*******************************************************************
�������ƣ�BOOL CreateNullXmlFile()
��������������һ����������XML�ļ�
�����������
�����������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE��
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CXmlFileOpr::CreateNullXmlFile()
{
	//�ļ����������������Ϊ��
	if (m_szFilePath[0] == '\0' || m_szRootName[0] == '\0')
	{
		return FALSE;
	}

	//���ԭDOM�ĵ�
	if (m_pDOMDoc != 0 && !m_bParsedDoc)
	{
		m_pDOMDoc->release();
	}
	m_pDOMDoc = 0;

    try
	{	
		//����DOM�ĵ�
		m_pDOMDoc = m_pDOMimpl->createDocument(0, X(m_szRootName),0);
		if (m_pDOMDoc == 0)
		{
			return FALSE;
		}
		//�Ƿ��������е�DOM�ĵ���ʹ�ú�����ʾ�ͷ�����Դ
		m_bParsedDoc = FALSE;
	}
	catch(...)
	{
		//������
		DoError("%s","createDocument() error");

		return FALSE;
	}
	
	//����дXML�ļ��Ƿ�ɹ�
    return WriteXmlFile();
}

/*******************************************************************
�������ƣ�BOOL WriteXmlFile(const DOMDocument *pDOMDoc)
������������DOM�ĵ�������д���ļ���
�����������
�����������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE��
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CXmlFileOpr::WriteXmlFile()
{
	if (m_pDOMDoc == 0 || m_szFilePath[0] == '\0')
	{
		return FALSE;
	}

	//����һ�ļ����л���
	//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
	DOMWriter *pWriter;
	try
	{
		pWriter = ((DOMImplementationLS *)m_pDOMimpl)->createDOMWriter();
	}
	catch (...)
	{
		DoError("%s","DOMimpl->createDOMWriter() error");
		return FALSE;
	}
	//the end	2004-5-20	��Ӧ���ⵥSWPD05244

	if (pWriter == 0)
	{
		return FALSE;
	}

	//ָ�������ʽ
	//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
	try
	{
		pWriter->setEncoding(X(m_szFontSet));
	}
	catch (...)
	{
		DoError("%s","DOMWriter->setEncoding() error");
		return FALSE;
	}
	//the end	2004-5-20	��Ӧ���ⵥSWPD05244
	
	//�趨֧�ָ�ʽ�����ԣ��������Զ�����ָ�����з�
	//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
	BOOL bCanSetFeature;
	try
	{
		bCanSetFeature = pWriter->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
	}
	catch (...)
	{
		DoError("%s","DOMWriter->canSetFeature() error");
		return FALSE;
	}
	//the end	2004-5-20	��Ӧ���ⵥSWPD05244

	if (bCanSetFeature)
	{
		//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
		try
		{
			pWriter->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
		}
		catch (...)
		{
			DoError("%s","DOMWriter->setFeature() error");
			return FALSE;
		}
		//the end	2004-5-20	��Ӧ���ⵥSWPD05244
	}

	//ָ�����з�
	//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
	try
	{
		pWriter->setNewLine(gNewLine);
	}
	catch (...)
	{
		DoError("%s","DOMWriter->setNewLine() error");
		return FALSE;
	}
	//the end	2004-5-20	��Ӧ���ⵥSWPD05244

	//дXML�ļ�ǰ���ȶ�ԭ�ļ���һ���ݣ���д�ļ�ʧ�ܣ��ñ����ļ��ָ�
	
	BOOL bRightCopy = FALSE;
	char szFileBak[MAX_PATH];
	//�ж��ļ��Ƿ����
	if (access(m_szFilePath, 0) == 0)
	{
		//���ļ����ڣ�����һ�ݱ���
		sprintf(szFileBak,"%s%s",m_szFilePath,".bak");
		bRightCopy = FileCopy(m_szFilePath, szFileBak);
	}

	//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
	LocalFileFormatTarget *pFormTarget;	
	try
	{
		pFormTarget = new LocalFileFormatTarget(m_szFilePath);
	}
	catch (...)
	{
		DoError("%s","LocalFileFormatTarget() error");
		return FALSE;
	}
	//the end	2004-5-20	��Ӧ���ⵥSWPD05244

	if (pFormTarget == 0)
	{
		pWriter->release();
		return FALSE;
	}
	
	BOOL bReturn = TRUE;
    try
	{
		pWriter->writeNode(pFormTarget,*m_pDOMDoc);	
	}
	catch(...)
	{
		//������
		DoError("%s", "DOMWriter->writeNode() error");
		//�ָ�ԭ�ļ�
		if (bRightCopy)
		{
			FileCopy(szFileBak, m_szFilePath);
		}

		bReturn = FALSE;
	}
	pWriter->release();
	delete pFormTarget;

	//ɾ�������ļ�
	if (bRightCopy)
	{
		FileDel(szFileBak);
	}

	return bReturn;
}

/*******************************************************************
�������ƣ�DOMElement * CreateNewElement(DOMDocument *pDOMDoc, 
										const char * const pszNodeName)
����������Ϊָ��DOM�ĵ�����һָ���ǵ�DOM���
���������DOMDocument *pDOMDoc��          ָ����DOM�ĵ���
          const char * const pszNodeName��ָ�����ı������
�����������
�������ݣ��ɹ����������ɵ�DOM���ָ��,ʧ�ܷ���NULL��
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
DOMElement * CXmlFileOpr::CreateNewElement(DOMDocument *pDOMDoc, 
										   const char * const pszNodeName)
{
	if (pDOMDoc == 0 || pszNodeName == 0 || pszNodeName[0] == '\0')
	{
		return NULL;
	}

	DOMElement *pNewNode = 0;
	try
	{
		 pNewNode = pDOMDoc->createElement(X(pszNodeName));
	}
	catch(const DOMException& tocatch)
	{
		//������
		STRING strError = GetDOMExceptionMsg(tocatch);
		DoError("%s,%s","createElement() error",strError.c_str());

		return NULL;
	}

	return pNewNode;
}

/*******************************************************************
�������ƣ�DOMElement * CreateNewElement(const char * const pszNodeName)
��������������һָ���ǵ�DOM���
���������const char * const pszNodeName��ָ�����ı������
�����������
�������ݣ��ɹ����������ɵ�DOM���ָ��,ʧ�ܷ���NULL��
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
DOMElement * CXmlFileOpr::CreateNewElement(const char * const pszNodeName)
{
	if (m_pDOMDoc == 0 || pszNodeName == 0 || pszNodeName[0] == '\0')
	{
		return NULL;
	}

	DOMElement *pNewNode = 0;
	try
	{
		 pNewNode = m_pDOMDoc->createElement(X(pszNodeName));
	}
	catch(const DOMException& tocatch)
	{
		//������
		STRING strError = GetDOMExceptionMsg(tocatch);
		DoError("%s,%s","createElement() error",strError.c_str());

		return NULL;
	}

	return pNewNode;
}


/*******************************************************************
�������ƣ�DOMElement * AddRootElementToDOMDoc(DOMDocument *pDOMDoc, 
									const char * const pszRootName)
�����������½����������㵽ָ��DOM�ĵ�
���������DOMDocument *pDOMDoc��          ָ����DOM�ĵ���
          const char * const pszRootName��ָ�������ı������
�����������
�������ݣ��ɹ����������ɵ�DOM�����ָ��,ʧ�ܷ���NULL��
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
DOMElement * CXmlFileOpr::AddRootElementToDOMDoc(DOMDocument *pDOMDoc, 
										const char * const pszRootName)
{
	//�ĵ�Ϊ�ջ�ָ���������Ϊ��,����NULL
	if (pDOMDoc == 0 || pszRootName == 0)
	{
		return NULL;
	}
	
	//���ڸ���㣬�򷵻ش˸����ָ��
	//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
	DOMElement *pRootEle;
	try
	{
		pRootEle = (DOMElement *)pDOMDoc->getDocumentElement();
	}
	catch (...)
	{
		DoError("%s","DOMDoc->getDocumentElement() error");
		return NULL;
	}
	//the end	2004-5-20	��Ӧ���ⵥSWPD05244

	if (pRootEle != 0)
	{
		return pRootEle;
	}

    //����ָ�����Ƶĸ����
	//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
	try
	{
		pRootEle = CreateNewElement(pDOMDoc,pszRootName);
	}
	catch (...)
	{
		DoError("%s","CreateNewElement() error");
		return NULL;
	}
	//the end	2004-5-20	��Ӧ���ⵥSWPD05244
		
	if (pRootEle == 0)
	{
		return NULL;
	}

    //�������㵽�ĵ���
    try
	{
		pDOMDoc->appendChild(pRootEle);
	}
	catch(const DOMException& tocatch)
	{
		//������
		STRING strError = GetDOMExceptionMsg(tocatch);
		DoError("%s,%s","appendChild() error",strError.c_str());
		
		pRootEle->release();
		return NULL;
	}

	return pRootEle;
}

/*******************************************************************
�������ƣ�DOMElement * GetRootElement()
������������ȡ�����
�����������
�����������
�������ݣ��ɹ����ظ����ָ��,ʧ�ܷ���NULL��
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
DOMElement * CXmlFileOpr::GetRootElement()
{
	if (m_pDOMDoc == 0)
	{
		return NULL;
	}

	return m_pDOMDoc->getDocumentElement();
}

/*******************************************************************
�������ƣ�BOOL AddChildToNode(DOMElement *pNode,DOMElement *pChildNode)
������������ָ������¼���ָ���ӽ��
���������DOMElement *pNode��          ָ����DOM�����ָ�룻
          DOMElement *pChildNode��     ָ����DOM�ӽ��ָ�룻
�����������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE��
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CXmlFileOpr::AddChildToNode(DOMElement *pNode, DOMElement *pChildNode)
{
	if (pNode == 0 || pChildNode == 0)
	{
		return FALSE;
	}

    //�������㵽�ĵ���
    try
	{
		pNode->appendChild(pChildNode);
	}
	catch(const DOMException& tocatch)
	{
		//������
		STRING strError = GetDOMExceptionMsg(tocatch);
		DoError("%s,%s","appendChild() error",strError.c_str());

		return FALSE;
	}

	return TRUE;
}

/*******************************************************************
�������ƣ�BOOL DeleteNode(DOMNode *& pDelNode)
������������DOM�������ɾ��ָ����㼰���ӽ��
���������DOMNode *pDelNode��          ָ��ɾ���Ľ��ָ�룻
�����������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE��
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CXmlFileOpr::DeleteNode(DOMNode *& pDelNode)
{
	if (pDelNode == 0)
	{
		return NULL;
	}

	//�ж�Ҫ��ɾ���Ľ���Ƿ��и����
	//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
	DOMNode *pParentNode;
	try
	{
		pParentNode = pDelNode->getParentNode();
	}
	catch (...)
	{
		DoError("%s","pDelNode->getParentNode() error");
		return FALSE;
	}
	//the end	2004-5-20	��Ӧ���ⵥSWPD05244
	
	if (pParentNode == 0)
	{
		pDelNode->release();
		pDelNode = 0;
		return TRUE;
	}
    
	DOMNode *pNodeRet = 0;

	//ɾ�����ӽ���и���㣬�ȴӸ���㽫�˽��ɾ��
	try
	{
		pNodeRet = pParentNode->removeChild(pDelNode);
	}
	catch(const DOMException& tocatch)
	{
		//������
		STRING strError = GetDOMExceptionMsg(tocatch);
		DoError("%s,%s","removeChild() error",strError.c_str());

		return FALSE;
	}

	pDelNode->release();
	pDelNode = 0;

	return TRUE;
}


/*******************************************************************
�������ƣ�BOOL SetNodeAttrValue(DOMElement *pNode, 
          const char * const pszAttrName,const char * const pszAttrValue)
������������ָ����������޸�ָ������
���������DOMElement *pNode��               ָ���Ľ��ָ�룻
          const char * const pszAttrName��  �������������
		  const char * const pszAttrValue�� ���������ֵ��
�����������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE��
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CXmlFileOpr::SetNodeAttrValue(DOMElement *pNode, 
								   const char * const pszAttrName,
								   const char * const pszAttrValue)
{
	if (pNode == 0 || pszAttrName == 0 || pszAttrValue == 0 ||
		pszAttrName[0] == '\0')
	{
		return FALSE;
	}

	//������޸�ָ���������
    try
	{
		pNode->setAttribute(X(pszAttrName), X(pszAttrValue));
	}
	catch(const DOMException& tocatch)
	{
		//������
		STRING strError = GetDOMExceptionMsg(tocatch);
		DoError("%s,%s","setAttribute() error",strError.c_str());

		return FALSE;
	}

	return TRUE;
}

BOOL CXmlFileOpr::setNodeValue(DOMElement *pNode, 
						       const char * const pszValue)
{
	if (pNode == 0 || pszValue == 0 || pszValue[0] == '\0')
	{
		return FALSE;
	}

	//������޸�ָ���������
    try
	{
        DOMText* txt1 = m_pDOMDoc->createTextNode(X(pszValue));
        pNode->appendChild(txt1);
	}
	catch(const DOMException& tocatch)
	{
		//������
		STRING strError = GetDOMExceptionMsg(tocatch);
		DoError("%s,%s","put_text() error",strError.c_str());

		return FALSE;
	}

	return TRUE;
}

/*******************************************************************
�������ƣ�const XMLCh * GetNodeAttrValue(DOMElement *pNode, 
                                         const char * const pszAttrName)
������������ȡָ������ָ������ֵ
���������DOMElement *pNode��               ָ���Ľ��ָ�룻
          const char * const pszAttrName��  ָ������������
�����������
�������ݣ��ɹ���������ֵ��Unicode�ַ���ָ��,ʧ�ܷ���NULL��
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
const XMLCh * CXmlFileOpr::GetNodeAttrValue(DOMElement *pNode, 
											const char * const pszAttrName)
{
	if (pNode == 0 || pszAttrName == 0 || pszAttrName[0] == '\0')
	{
		return NULL;
	}

	return pNode->getAttribute(X(pszAttrName));
}

/*******************************************************************
�������ƣ�DOMNodeList * GetChildListByTagName(DOMElement *pNode,
                                       const char * const pszTagName)
�����������ɸ���㼰�ӽ���Ǻŵõ�����ָ���ӽ���б�
���������DOMElement *pNode��            ָ���ĸ����ָ�룻
          const char * const pszTagName���ӽ�������ƣ�
�����������
�������ݣ��ɹ������ӽ���б�,ʧ�ܷ���NULL��
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
DOMNodeList * CXmlFileOpr::GetChildListByTagName(DOMElement *pNode, 
												 const char * const pszTagName)
{
	if (pNode == 0 || pszTagName == 0 || pszTagName[0] == '\0')
	{
		return NULL;
	}

	return pNode->getElementsByTagName(X(pszTagName));
}


/*******************************************************************
�������ƣ�DOMElement * GetNodeByAttrValue(DOMNodeList *pNodeList, 
										  const char * const pszAttrName,
								          const char * const pszAttrValue)
������������ȡָ������ֵ���ӽ��ָ��
���������DOMNodeList *pNodeList��        �ӽ���б�ָ�룻
          const char * const pszAttrName���ӽ�����Ա�����ƣ�
		  const char * const pszAttrValue���ӽ������ֵ��
�����������
�������ݣ��ɹ��ҵ�ָ���ӽ�㣬�����ӽ��ָ�룻���򷵻�NULL��
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
DOMElement * CXmlFileOpr::GetNodeByAttrValue(DOMNodeList *pNodeList, 
											 const char * const pszAttrName,
								             const char * const pszAttrValue)
{
	//������Ч���ж�
	if (pNodeList == 0 || pszAttrName == 0 || pszAttrValue == 0 ||
		pszAttrName[0] == '\0')
	{
		return NULL;
	}
	
	//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
	int nNodeCount;
	try
	{
		nNodeCount = pNodeList->getLength();
	}
	catch (...)
	{
		DoError("%s","pNodeList->getLength() error");
		return NULL;
	}
	//the end	2004-5-20	��Ӧ���ⵥSWPD05244

	if (nNodeCount <= 0)
	{
		return NULL;
	}

	//�����б�����ָ�����
	for (int i=0; i<nNodeCount; i++)
	{
		DOMElement *pEle = (DOMElement *)pNodeList->item(i);
		if (pEle != 0)
		{
			//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
			const XMLCh *pValue;
			try
			{
			pValue = pEle->getAttribute(X(pszAttrName));
			}
			catch (...)
			{
				DoError("%s","pEle->getAttribute() error");
				return NULL;
			}
			//the end	2004-5-20	��Ӧ���ⵥSWPD05244

			//�ҵ�ָ������ֵ�Ľ��
			//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
			int nCompareString;
			try
			{
				nCompareString = XMLString::compareString(pValue,X(pszAttrValue));
			}
			catch (...)
			{
				DoError("%s","XMLString::compareString() error");
				return NULL;
			}
			//the end	2004-5-20	��Ӧ���ⵥSWPD05244

			if (nCompareString == 0)
			{
				return pEle;
			}
		}
	}
	
	return NULL;
}

/*******************************************************************
�������ƣ�BOOL ClearDOMDocument()
�������������XML��DOM�ĵ�������
�����������
�����������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE��
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CXmlFileOpr::ClearDOMDocument()
{
	if (m_pDOMDoc == 0)
	{
		return FALSE;
	}
	
	//��ȡDOM������ĸ����
	//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
	DOMElement *pRootEle;
	try
	{
		pRootEle = m_pDOMDoc->getDocumentElement();
	}
	catch (...)
	{
		DoError("%s","m_pDOMDoc->getDocumentElement() error");
		return FALSE;
	}
	//the end	2004-5-20	��Ӧ���ⵥSWPD05244

	if (pRootEle == 0)
	{
		//�޸���㣬˵��DOM������Ѿ���һ�ÿ������ɹ�����
		return TRUE;
	}

	//ɾ������㼰���������е��ӽ��
	//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
	DOMNode *pDelNode;
	try
	{
		pDelNode = m_pDOMDoc->removeChild(pRootEle);
	}
	catch (...)
	{
		DoError("%s","m_pDOMDoc->removeChild() error");
		return FALSE;
	}
	//the end	2004-5-20	��Ӧ���ⵥSWPD05244

	if (pDelNode == 0)
	{
		return FALSE;
	}

	//�ͷ���Դ
	pDelNode->release();
	pDelNode = 0;

	return TRUE;
}

/*******************************************************************
�������ƣ�BOOL SetDOMDocument(DOMDocument *pDOMDoc)
������������ָ��DOM�ĵ�����������ԭDOM�ĵ�������
���������DOMDocument *pDOMDoc����DOM�ĵ���ָ�룻
�����������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE��
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CXmlFileOpr::SetDOMDocument(DOMDocument *pDOMDoc)
{
	//������Ч�Լ��
	if (pDOMDoc == 0 || m_pDOMDoc == pDOMDoc)
	{
		return FALSE;
	}
	
	//��DOM�ĵ�������������У����ͷ�����Դ
	if (!m_bParsedDoc && m_pDOMDoc != 0)
	{
		m_pDOMDoc->release();
		m_pDOMDoc = 0;
	}
	
	m_pDOMDoc = pDOMDoc;

	//����DOM�ĵ��Ƿ��������У�ʹ�ú�����ʾ�ͷ�����Դ
	m_bParsedDoc = FALSE;

	return TRUE;
}

/*******************************************************************
�������ƣ�void DisplayXmlFile()
������������XML�ļ���ʽ��ʾDOM�ĵ���
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CXmlFileOpr::DisplayXmlFile()
{
	if (m_pDOMDoc != 0)
	{
		//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
		XMLFormatTarget *pFormTarget;
		try
		{
			pFormTarget = new StdOutFormatTarget();
		}
		catch (...)
		{
			DoError("%s","StdOutFormatTarget() error");
		}
		//the end	2004-5-20	��Ӧ���ⵥSWPD05244

		//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
		DOMWriter  *pDOMWriter;
		try
		{
			pDOMWriter = ((DOMImplementationLS*)m_pDOMimpl)->createDOMWriter();
		}
		catch (...)
		{
			DoError("%s","m_pDOMimpl->createDOMWriter() error");
		}
		//the end	2004-5-20	��Ӧ���ⵥSWPD05244
		
		
		//�趨֧�ָ�ʽ������
		//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
		BOOL bCanSetFeature;
		try
		{
			bCanSetFeature = pDOMWriter->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
		}
		catch (...)
		{
			DoError("%s","DOMWriter->canSetFeature() error");
		}
		//the end	2004-5-20	��Ӧ���ⵥSWPD05244

		if (bCanSetFeature)
		{
			//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
			try
			{
				pDOMWriter->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
			}
			catch (...)
			{
				DoError("%s","DOMWriter->setFeature() error");
			}
			//the end	2004-5-20	��Ӧ���ⵥSWPD05244
		}
		//ָ�����з�
		//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
		try
		{
			pDOMWriter->setNewLine(gNewLine);
		}
		catch (...)
		{
			DoError("%s","DOMWriter->setNewLine() error");
		}
		//the end	2004-5-20	��Ӧ���ⵥSWPD05244

		try
		{
			pDOMWriter->writeNode(pFormTarget, *m_pDOMDoc);
		}
		catch(...)
		{
			//������	    
			DoError("%s","DOMWriter->writeNode() error");
		}

		delete pFormTarget;
		pDOMWriter->release();
	}
}

/*******************************************************************
�������ƣ�STRING GetDOMExceptionMsg(const DOMException & tocatch)
������������ȡDOMException�Ĵ�����Ϣ
���������const DOMException & tocatch�����񵽵�DOMException�쳣;
�����������
�������ݣ������쳣�����뼰��Ϣ�ַ���
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
STRING CXmlFileOpr::GetDOMExceptionMsg(const DOMException & tocatch)
{
	char szTmp[256];
	sprintf(szTmp,"DOMException code: %d",tocatch.code);

	STRING strReturn(szTmp);
	
	XMLCh errText[2048];

	//by	����	2004-5-20	��Ӧ���ⵥSWPD05244
	BOOL bLoadDOMExceptionMsg;
	try
	{
		bLoadDOMExceptionMsg = DOMImplementation::loadDOMExceptionMsg(tocatch.code, errText, 2047);
	}
	catch (...)
	{
		DoError("%s","DOMImplementation::loadDOMExceptionMsg() error");
	}
	//the end	2004-5-20	��Ӧ���ⵥSWPD05244

	if (bLoadDOMExceptionMsg)
	{
		strReturn += STRING(",Message: ") + S(errText);
	}

	return strReturn;
}

/*******************************************************************
�������ƣ�STRING GetXMLExceptionMsg(const XMLException& tocatch)
������������ȡXMLException�Ĵ�����Ϣ
���������const XMLException & tocatch�����񵽵�DOMException�쳣;
�����������
�������ݣ������쳣�����뼰��Ϣ�ַ���
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
STRING CXmlFileOpr::GetXMLExceptionMsg(const XMLException& tocatch)
{
	STRING strReturn("XMLException Message: ");
    strReturn += S(tocatch.getMessage());
	
	return strReturn;
}


/*******************************************************************
�������ƣ�void DoError(const char *pszFormat,...)
����������������: ����̨������Trace�ļ�
���������const char *pszFormat,     ��ʽ���ַ�����
		  ...,                       ��ѡ������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CXmlFileOpr::DoError(const char *pszFormat,...)
{
	char szBuf[1024] = "";

	int nLen = 0;
	if (m_szModuleName[0] != '\0')
	{
        sprintf(szBuf,"(%s) ",m_szModuleName);
		nLen = strlen(szBuf);
	}
	
	va_list arglist;
    va_start(arglist, pszFormat);
    VSNPRINTF(&szBuf[nLen],(1024 - nLen), pszFormat, arglist);
    szBuf[1023] = 0;
    va_end(arglist);

#ifdef _OUTPUT_CONSOLE
	//��������Ϣ���������̨
	cout<<szBuf<<endl;
#else
	//��������Ϣ�����Trace�ļ�
	TRACE("CXmlFileOpr", szBuf);
#endif

}

/*******************************************************************
�������ƣ�void SetModuleName(const char * const pszModuleName)
��������������ģ������
���������const char * const pszModuleName,   ҵ��ģ�����ƣ�
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CXmlFileOpr::SetModuleName(const char * const pszModuleName)
{
	if (pszModuleName == 0)
	{
		return;
	}

	strncpy(m_szModuleName, pszModuleName, sizeof(m_szModuleName));
}

//�����ַ���
void CXmlFileOpr::SetFontSet(const char * szFontSet)
{
	if (szFontSet == 0 || szFontSet[0] == 0)
	{
		return;
	}

	strncpy(m_szFontSet, szFontSet, sizeof(m_szFontSet));
}
