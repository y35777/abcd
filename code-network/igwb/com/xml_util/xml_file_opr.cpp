//文件名：xml_file_opr.cpp
//功能：  实现xml文件操作类CXmlFileOpr

#include "xml_file_opr.h"
#include <iostream>
#include <locale.h>

//使用Xerces头文件
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

//换行符
static const XMLCh gNewLine[] = { chCR, chLF, chNull };

//DOM分析器名称
static const XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };

//定义类静态变量
int CXmlFileOpr::g_nRefCount = 0;

ACE_Mutex CXmlFileOpr::g_MutexRefCnt;

////////////////////////////////////////////////////////////////////
//CXmlFileOpr implementation
////////////////////////////////////////////////////////////////////

/*******************************************************************
函数名称：CXmlFileOpr()
功能描述：构造函数
输入参数：无
输出参数：无
返回内容：无；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



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
函数名称：~CXmlFileOpr()
功能描述：析构函数
输入参数：无
输出参数：无
返回内容：无；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
CXmlFileOpr::~CXmlFileOpr()
{
	//如DOM文档存在并且不属于分析器所有，则需释放其内存
	if (!m_bParsedDoc && m_pDOMDoc != 0)
	{
		m_pDOMDoc->release();
	    m_pDOMDoc = 0;	
	}
    
	//释放分析器资源，并自动释放它产生的DOM文档
	if (m_pDOMBuilder != 0)
	{
		m_pDOMBuilder->release();
		m_pDOMBuilder = 0;
	}

    //结束Xerces调用
    if (m_bInitXerces)
    {
		g_MutexRefCnt.acquire();
        
        //减少使用计数
        --g_nRefCount;
        
        //使用计数等于0表示无其它线程正在使用Xerces,需释放其资源
        if (g_nRefCount == 0)
        {
			UninitXMLPlatFormUtils();

        }

        g_MutexRefCnt.release();
    }
}

/*******************************************************************
函数名称：DOMDocument * GetDOMDocument()
功能描述：获取DOM文档指针
输入参数：无
输出参数：无
返回内容：返回m_pDOMDoc；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
DOMDocument * CXmlFileOpr::GetDOMDocument()
{
	return m_pDOMDoc;
}


/*******************************************************************
函数名称：BOOL InitXMLPlatFormUtils()
功能描述：初始化Xerecs
输入参数：无
输出参数：无
返回内容：初始化成功返回TRUE,失败返回FALSE；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CXmlFileOpr::InitXMLPlatFormUtils()
{
	//已初始化Xerces，返回
    if (m_bInitXerces)
    {
        return TRUE;
    }
    
	g_MutexRefCnt.acquire();
    
    //判断使用计数是否大于0，大于0表示已有线程初始化了Xerces；否则需初始化
    if (g_nRefCount <= 0)
    {
        //初始化Xerces
        try
        {
            XMLPlatformUtils::Initialize();
        }
        catch(const XMLException& tocatch)
        {
            g_MutexRefCnt.release();
            //错误处理
		    STRING strError = GetXMLExceptionMsg(tocatch);
		    DoError("%s,%s","initialization XMLPlatFormUtils error", strError.c_str());
		    return FALSE;
        }
    }

    //增加使用计算
    ++g_nRefCount;

    g_MutexRefCnt.release();

    m_bInitXerces = TRUE;

 	//by	杨善	2004-5-20	对应问题单SWPD05244
	//得到DOMImplementation对象接口
	try
	{
		m_pDOMimpl = DOMImplementationRegistry::getDOMImplementation(gLS);
	}
	catch (...)
	{
		DoError("%s","DOMImplementationRegistry::getDOMImplementation() error");
		return FALSE;
	}
	//the end	2004-5-20	对应问题单SWPD05244

	return (m_pDOMimpl != 0);
}

/*******************************************************************
函数名称：void UninitXMLPlatFormUtils()
功能描述：释放Xerecs资源
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CXmlFileOpr::UninitXMLPlatFormUtils()
{
	//释放Xerces使用的资源
	if (m_bInitXerces)
	{
		//by	杨善	2004-5-20	对应问题单SWPD05244
		try
		{
			XMLPlatformUtils::Terminate();
		}
		catch (...)
		{
			DoError("%s","XMLPlatformUtils::Terminate() error");
		}
		//the end	2004-5-20	对应问题单SWPD05244

		m_bInitXerces = FALSE;
	}
}

/*******************************************************************
函数名称：BOOL InitXmlFile(const char * const szFilePath,
							  const char * const szRootName)
功能描述：读取指定XML文件，并建立DOM结点树；如指定文件不存在，则新建
          一仅含根结点的XML文件
输入参数：const char * const szFilePath,  XML文件全路径名；
          const char * const szRootName,  XML文件的根结点标记；
输出参数：无
返回内容：初始化成功返回TRUE,失败返回FALSE；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



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

	//参数有效性检查
	if (szFilePath == 0 || szRootName == 0)
	{
		return FALSE;
	}
	//文件名及根结点名称不能为空
	if (szFilePath[0] == '\0' || szRootName[0] == '\0')
	{
		return FALSE;
	}

	STRNCPY(m_szFilePath, szFilePath, MAX_PATH);
	STRNCPY(m_szRootName, szRootName, MAX_PATH);

	//XML文件URI不支持'\'字符，替换成'/'
	int nLen = strlen(m_szFilePath);
	for(int i=0; i<nLen; i++)
	{
		if (m_szFilePath[i] == '\\')
		{
			m_szFilePath[i] = '/';
		}
	}
	
	//判断文件是否存在
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
        
        //创建文件所在目录
		BOOL bCreateDeepDir = CreateDeepDir(szDirPath.c_str());

        if (!bCreateDeepDir)
        {
            return FALSE;
        }
    }

	//如未初始化Xerces，则先初始化
	if (!m_bInitXerces)
	{
		if (!InitXMLPlatFormUtils())
		{
			return FALSE;
		}
	}

	BOOL bReturn = FALSE;
	//检查文件是否存在
    if (!bFileExist)
	{		
		bReturn = CreateNullXmlFile();
	}
	else
	{   
		//文件存在，则分析XML文件并建立文档结点树
		bReturn = CreateDOMDocumentTree();
    }

	return bReturn;
}

/*******************************************************************
函数名称：BOOL CreateDOMDocumentTree()
功能描述：分析XML文件，并生成DOM结点树
输入参数：无
输出参数：无
返回内容：成功返回TRUE,失败返回FALSE；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CXmlFileOpr::CreateDOMDocumentTree()
{
	//检查文件名是否为空
	if (m_szFilePath[0] == '\0')
	{
		return FALSE;
	}
	
	//生成DOM分析器
	if (m_pDOMBuilder == 0)
	{
		//by	杨善	2004-5-20	对应问题单SWPD05244
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
		//the end	2004-5-20	对应问题单SWPD05244

		if (m_pDOMBuilder == 0)
		{
			return FALSE;
		}
	}
	else
	{
		//DOM分析器存在，则清空分析器DOM文档数组缓存
		try
		{
			m_pDOMBuilder->resetDocumentPool(); 
		}
		catch(...)
		{
			//错误处理
		    DoError("%s","when creating DOM tree, resetDocumentPool() error");
			
			return FALSE;
		}
	}
	//清除原DOM文档
	if (m_pDOMDoc != 0 && !m_bParsedDoc)
	{
		m_pDOMDoc->release();
	}
	m_pDOMDoc = 0;
    //设置分析器属性

	//
	try
	{
		m_pDOMDoc = m_pDOMBuilder->parseURI(m_szFilePath);
		if (m_pDOMDoc == 0)
		{
			return FALSE;
		}
        //DOM文档归分析器所有，由分析器负责释放其资源
		m_bParsedDoc = TRUE;
	}
	catch (const XMLException& tocatch)
	{
		//错误处理
		STRING strError = GetXMLExceptionMsg(tocatch);
		DoError("%s%s,%s","Error during parsing:",m_szFilePath,strError.c_str());

		return FALSE;
	}
	catch (const DOMException& tocatch)
	{
		//错误处理
		STRING strError = GetDOMExceptionMsg(tocatch);
		DoError("%s%s,%s","Error during parsing:",m_szFilePath,strError.c_str());

		return FALSE;
	}
	catch (...)
	{
		//错误处理
		DoError("%s%s","Unexpected exception during parsing:",m_szFilePath);
		
		return FALSE;
	}

	return TRUE;
}

/*******************************************************************
函数名称：BOOL CreateNullXmlFile()
功能描述：生成一仅含根结点的XML文件
输入参数：无
输出参数：无
返回内容：成功返回TRUE,失败返回FALSE；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CXmlFileOpr::CreateNullXmlFile()
{
	//文件名及根结点名不能为空
	if (m_szFilePath[0] == '\0' || m_szRootName[0] == '\0')
	{
		return FALSE;
	}

	//清除原DOM文档
	if (m_pDOMDoc != 0 && !m_bParsedDoc)
	{
		m_pDOMDoc->release();
	}
	m_pDOMDoc = 0;

    try
	{	
		//生成DOM文档
		m_pDOMDoc = m_pDOMimpl->createDocument(0, X(m_szRootName),0);
		if (m_pDOMDoc == 0)
		{
			return FALSE;
		}
		//非分析器所有的DOM文档，使用后需显示释放其资源
		m_bParsedDoc = FALSE;
	}
	catch(...)
	{
		//错误处理
		DoError("%s","createDocument() error");

		return FALSE;
	}
	
	//返回写XML文件是否成功
    return WriteXmlFile();
}

/*******************************************************************
函数名称：BOOL WriteXmlFile(const DOMDocument *pDOMDoc)
功能描述：将DOM文档树内容写入文件中
输入参数：无
输出参数：无
返回内容：成功返回TRUE,失败返回FALSE；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CXmlFileOpr::WriteXmlFile()
{
	if (m_pDOMDoc == 0 || m_szFilePath[0] == '\0')
	{
		return FALSE;
	}

	//生成一文件序列化类
	//by	杨善	2004-5-20	对应问题单SWPD05244
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
	//the end	2004-5-20	对应问题单SWPD05244

	if (pWriter == 0)
	{
		return FALSE;
	}

	//指定编码格式
	//by	杨善	2004-5-20	对应问题单SWPD05244
	try
	{
		pWriter->setEncoding(X(m_szFontSet));
	}
	catch (...)
	{
		DoError("%s","DOMWriter->setEncoding() error");
		return FALSE;
	}
	//the end	2004-5-20	对应问题单SWPD05244
	
	//设定支持格式化属性，这样会自动加上指定换行符
	//by	杨善	2004-5-20	对应问题单SWPD05244
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
	//the end	2004-5-20	对应问题单SWPD05244

	if (bCanSetFeature)
	{
		//by	杨善	2004-5-20	对应问题单SWPD05244
		try
		{
			pWriter->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
		}
		catch (...)
		{
			DoError("%s","DOMWriter->setFeature() error");
			return FALSE;
		}
		//the end	2004-5-20	对应问题单SWPD05244
	}

	//指定换行符
	//by	杨善	2004-5-20	对应问题单SWPD05244
	try
	{
		pWriter->setNewLine(gNewLine);
	}
	catch (...)
	{
		DoError("%s","DOMWriter->setNewLine() error");
		return FALSE;
	}
	//the end	2004-5-20	对应问题单SWPD05244

	//写XML文件前，先对原文件做一备份，如写文件失败，用备份文件恢复
	
	BOOL bRightCopy = FALSE;
	char szFileBak[MAX_PATH];
	//判断文件是否存在
	if (access(m_szFilePath, 0) == 0)
	{
		//如文件存在，先做一份备份
		sprintf(szFileBak,"%s%s",m_szFilePath,".bak");
		bRightCopy = FileCopy(m_szFilePath, szFileBak);
	}

	//by	杨善	2004-5-20	对应问题单SWPD05244
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
	//the end	2004-5-20	对应问题单SWPD05244

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
		//错误处理
		DoError("%s", "DOMWriter->writeNode() error");
		//恢复原文件
		if (bRightCopy)
		{
			FileCopy(szFileBak, m_szFilePath);
		}

		bReturn = FALSE;
	}
	pWriter->release();
	delete pFormTarget;

	//删除备份文件
	if (bRightCopy)
	{
		FileDel(szFileBak);
	}

	return bReturn;
}

/*******************************************************************
函数名称：DOMElement * CreateNewElement(DOMDocument *pDOMDoc, 
										const char * const pszNodeName)
功能描述：为指定DOM文档生成一指点标记的DOM结点
输入参数：DOMDocument *pDOMDoc，          指定的DOM文档；
          const char * const pszNodeName，指定结点的标记名称
输出参数：无
返回内容：成功返回新生成的DOM结点指针,失败返回NULL；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



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
		//错误处理
		STRING strError = GetDOMExceptionMsg(tocatch);
		DoError("%s,%s","createElement() error",strError.c_str());

		return NULL;
	}

	return pNewNode;
}

/*******************************************************************
函数名称：DOMElement * CreateNewElement(const char * const pszNodeName)
功能描述：生成一指点标记的DOM结点
输入参数：const char * const pszNodeName，指定结点的标记名称
输出参数：无
返回内容：成功返回新生成的DOM结点指针,失败返回NULL；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



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
		//错误处理
		STRING strError = GetDOMExceptionMsg(tocatch);
		DoError("%s,%s","createElement() error",strError.c_str());

		return NULL;
	}

	return pNewNode;
}


/*******************************************************************
函数名称：DOMElement * AddRootElementToDOMDoc(DOMDocument *pDOMDoc, 
									const char * const pszRootName)
功能描述：新建并加入根结点到指定DOM文档
输入参数：DOMDocument *pDOMDoc，          指定的DOM文档；
          const char * const pszRootName，指定根结点的标记名称
输出参数：无
返回内容：成功返回新生成的DOM根结点指针,失败返回NULL；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
DOMElement * CXmlFileOpr::AddRootElementToDOMDoc(DOMDocument *pDOMDoc, 
										const char * const pszRootName)
{
	//文档为空或指定根结点名为空,返回NULL
	if (pDOMDoc == 0 || pszRootName == 0)
	{
		return NULL;
	}
	
	//存在根结点，则返回此根结点指针
	//by	杨善	2004-5-20	对应问题单SWPD05244
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
	//the end	2004-5-20	对应问题单SWPD05244

	if (pRootEle != 0)
	{
		return pRootEle;
	}

    //生成指定名称的根结点
	//by	杨善	2004-5-20	对应问题单SWPD05244
	try
	{
		pRootEle = CreateNewElement(pDOMDoc,pszRootName);
	}
	catch (...)
	{
		DoError("%s","CreateNewElement() error");
		return NULL;
	}
	//the end	2004-5-20	对应问题单SWPD05244
		
	if (pRootEle == 0)
	{
		return NULL;
	}

    //加入根结点到文档中
    try
	{
		pDOMDoc->appendChild(pRootEle);
	}
	catch(const DOMException& tocatch)
	{
		//错误处理
		STRING strError = GetDOMExceptionMsg(tocatch);
		DoError("%s,%s","appendChild() error",strError.c_str());
		
		pRootEle->release();
		return NULL;
	}

	return pRootEle;
}

/*******************************************************************
函数名称：DOMElement * GetRootElement()
功能描述：获取根结点
输入参数：无
输出参数：无
返回内容：成功返回根结点指针,失败返回NULL；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



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
函数名称：BOOL AddChildToNode(DOMElement *pNode,DOMElement *pChildNode)
功能描述：在指定结点下加入指定子结点
输入参数：DOMElement *pNode，          指定的DOM父结点指针；
          DOMElement *pChildNode，     指定的DOM子结点指针；
输出参数：无
返回内容：成功返回TRUE,失败返回FALSE；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CXmlFileOpr::AddChildToNode(DOMElement *pNode, DOMElement *pChildNode)
{
	if (pNode == 0 || pChildNode == 0)
	{
		return FALSE;
	}

    //加入根结点到文档中
    try
	{
		pNode->appendChild(pChildNode);
	}
	catch(const DOMException& tocatch)
	{
		//错误处理
		STRING strError = GetDOMExceptionMsg(tocatch);
		DoError("%s,%s","appendChild() error",strError.c_str());

		return FALSE;
	}

	return TRUE;
}

/*******************************************************************
函数名称：BOOL DeleteNode(DOMNode *& pDelNode)
功能描述：在DOM结点树中删除指定结点及其子结点
输入参数：DOMNode *pDelNode，          指定删除的结点指针；
输出参数：无
返回内容：成功返回TRUE,失败返回FALSE；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CXmlFileOpr::DeleteNode(DOMNode *& pDelNode)
{
	if (pDelNode == 0)
	{
		return NULL;
	}

	//判断要求删除的结点是否有父结点
	//by	杨善	2004-5-20	对应问题单SWPD05244
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
	//the end	2004-5-20	对应问题单SWPD05244
	
	if (pParentNode == 0)
	{
		pDelNode->release();
		pDelNode = 0;
		return TRUE;
	}
    
	DOMNode *pNodeRet = 0;

	//删除的子结点有父结点，先从父结点将此结点删除
	try
	{
		pNodeRet = pParentNode->removeChild(pDelNode);
	}
	catch(const DOMException& tocatch)
	{
		//错误处理
		STRING strError = GetDOMExceptionMsg(tocatch);
		DoError("%s,%s","removeChild() error",strError.c_str());

		return FALSE;
	}

	pDelNode->release();
	pDelNode = 0;

	return TRUE;
}


/*******************************************************************
函数名称：BOOL SetNodeAttrValue(DOMElement *pNode, 
          const char * const pszAttrName,const char * const pszAttrValue)
功能描述：给指定结点加入或修改指定属性
输入参数：DOMElement *pNode，               指定的结点指针；
          const char * const pszAttrName，  加入的属性名；
		  const char * const pszAttrValue， 加入的属性值；
输出参数：无
返回内容：成功返回TRUE,失败返回FALSE；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



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

	//加入或修改指定结点属性
    try
	{
		pNode->setAttribute(X(pszAttrName), X(pszAttrValue));
	}
	catch(const DOMException& tocatch)
	{
		//错误处理
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

	//加入或修改指定结点属性
    try
	{
        DOMText* txt1 = m_pDOMDoc->createTextNode(X(pszValue));
        pNode->appendChild(txt1);
	}
	catch(const DOMException& tocatch)
	{
		//错误处理
		STRING strError = GetDOMExceptionMsg(tocatch);
		DoError("%s,%s","put_text() error",strError.c_str());

		return FALSE;
	}

	return TRUE;
}

/*******************************************************************
函数名称：const XMLCh * GetNodeAttrValue(DOMElement *pNode, 
                                         const char * const pszAttrName)
功能描述：获取指定结点的指定属性值
输入参数：DOMElement *pNode，               指定的结点指针；
          const char * const pszAttrName，  指定的属性名；
输出参数：无
返回内容：成功返回属性值的Unicode字符串指针,失败返回NULL；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



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
函数名称：DOMNodeList * GetChildListByTagName(DOMElement *pNode,
                                       const char * const pszTagName)
功能描述：由父结点及子结点标记号得到所有指定子结点列表
输入参数：DOMElement *pNode，            指定的父结点指针；
          const char * const pszTagName，子结点标记名称；
输出参数：无
返回内容：成功返回子结点列表,失败返回NULL；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



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
函数名称：DOMElement * GetNodeByAttrValue(DOMNodeList *pNodeList, 
										  const char * const pszAttrName,
								          const char * const pszAttrValue)
功能描述：获取指定属性值的子结点指针
输入参数：DOMNodeList *pNodeList，        子结点列表指针；
          const char * const pszAttrName，子结点属性标记名称；
		  const char * const pszAttrValue，子结点属性值；
输出参数：无
返回内容：成功找到指定子结点，返回子结点指针；否则返回NULL；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
DOMElement * CXmlFileOpr::GetNodeByAttrValue(DOMNodeList *pNodeList, 
											 const char * const pszAttrName,
								             const char * const pszAttrValue)
{
	//参数有效性判断
	if (pNodeList == 0 || pszAttrName == 0 || pszAttrValue == 0 ||
		pszAttrName[0] == '\0')
	{
		return NULL;
	}
	
	//by	杨善	2004-5-20	对应问题单SWPD05244
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
	//the end	2004-5-20	对应问题单SWPD05244

	if (nNodeCount <= 0)
	{
		return NULL;
	}

	//遍历列表，查找指定结点
	for (int i=0; i<nNodeCount; i++)
	{
		DOMElement *pEle = (DOMElement *)pNodeList->item(i);
		if (pEle != 0)
		{
			//by	杨善	2004-5-20	对应问题单SWPD05244
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
			//the end	2004-5-20	对应问题单SWPD05244

			//找到指定属性值的结点
			//by	杨善	2004-5-20	对应问题单SWPD05244
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
			//the end	2004-5-20	对应问题单SWPD05244

			if (nCompareString == 0)
			{
				return pEle;
			}
		}
	}
	
	return NULL;
}

/*******************************************************************
函数名称：BOOL ClearDOMDocument()
功能描述：清空XML的DOM文档树内容
输入参数：无
输出参数：无
返回内容：成功返回TRUE,失败返回FALSE；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CXmlFileOpr::ClearDOMDocument()
{
	if (m_pDOMDoc == 0)
	{
		return FALSE;
	}
	
	//获取DOM结点树的根结点
	//by	杨善	2004-5-20	对应问题单SWPD05244
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
	//the end	2004-5-20	对应问题单SWPD05244

	if (pRootEle == 0)
	{
		//无根结点，说明DOM结点树已经是一棵空树，成功返回
		return TRUE;
	}

	//删除根结点及其下面所有的子结点
	//by	杨善	2004-5-20	对应问题单SWPD05244
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
	//the end	2004-5-20	对应问题单SWPD05244

	if (pDelNode == 0)
	{
		return FALSE;
	}

	//释放资源
	pDelNode->release();
	pDelNode = 0;

	return TRUE;
}

/*******************************************************************
函数名称：BOOL SetDOMDocument(DOMDocument *pDOMDoc)
功能描述：用指定DOM文档树内容重设原DOM文档树内容
输入参数：DOMDocument *pDOMDoc，新DOM文档树指针；
输出参数：无
返回内容：成功返回TRUE,失败返回FALSE；
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CXmlFileOpr::SetDOMDocument(DOMDocument *pDOMDoc)
{
	//参数有效性检查
	if (pDOMDoc == 0 || m_pDOMDoc == pDOMDoc)
	{
		return FALSE;
	}
	
	//如DOM文档不归分析器所有，先释放其资源
	if (!m_bParsedDoc && m_pDOMDoc != 0)
	{
		m_pDOMDoc->release();
		m_pDOMDoc = 0;
	}
	
	m_pDOMDoc = pDOMDoc;

	//设置DOM文档非分析器所有，使用后需显示释放其资源
	m_bParsedDoc = FALSE;

	return TRUE;
}

/*******************************************************************
函数名称：void DisplayXmlFile()
功能描述：用XML文件格式显示DOM文档树
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CXmlFileOpr::DisplayXmlFile()
{
	if (m_pDOMDoc != 0)
	{
		//by	杨善	2004-5-20	对应问题单SWPD05244
		XMLFormatTarget *pFormTarget;
		try
		{
			pFormTarget = new StdOutFormatTarget();
		}
		catch (...)
		{
			DoError("%s","StdOutFormatTarget() error");
		}
		//the end	2004-5-20	对应问题单SWPD05244

		//by	杨善	2004-5-20	对应问题单SWPD05244
		DOMWriter  *pDOMWriter;
		try
		{
			pDOMWriter = ((DOMImplementationLS*)m_pDOMimpl)->createDOMWriter();
		}
		catch (...)
		{
			DoError("%s","m_pDOMimpl->createDOMWriter() error");
		}
		//the end	2004-5-20	对应问题单SWPD05244
		
		
		//设定支持格式化属性
		//by	杨善	2004-5-20	对应问题单SWPD05244
		BOOL bCanSetFeature;
		try
		{
			bCanSetFeature = pDOMWriter->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
		}
		catch (...)
		{
			DoError("%s","DOMWriter->canSetFeature() error");
		}
		//the end	2004-5-20	对应问题单SWPD05244

		if (bCanSetFeature)
		{
			//by	杨善	2004-5-20	对应问题单SWPD05244
			try
			{
				pDOMWriter->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
			}
			catch (...)
			{
				DoError("%s","DOMWriter->setFeature() error");
			}
			//the end	2004-5-20	对应问题单SWPD05244
		}
		//指定换行符
		//by	杨善	2004-5-20	对应问题单SWPD05244
		try
		{
			pDOMWriter->setNewLine(gNewLine);
		}
		catch (...)
		{
			DoError("%s","DOMWriter->setNewLine() error");
		}
		//the end	2004-5-20	对应问题单SWPD05244

		try
		{
			pDOMWriter->writeNode(pFormTarget, *m_pDOMDoc);
		}
		catch(...)
		{
			//错误处理	    
			DoError("%s","DOMWriter->writeNode() error");
		}

		delete pFormTarget;
		pDOMWriter->release();
	}
}

/*******************************************************************
函数名称：STRING GetDOMExceptionMsg(const DOMException & tocatch)
功能描述：获取DOMException的错误信息
输入参数：const DOMException & tocatch，捕获到的DOMException异常;
输出参数：无
返回内容：返回异常错误码及信息字符串
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
STRING CXmlFileOpr::GetDOMExceptionMsg(const DOMException & tocatch)
{
	char szTmp[256];
	sprintf(szTmp,"DOMException code: %d",tocatch.code);

	STRING strReturn(szTmp);
	
	XMLCh errText[2048];

	//by	杨善	2004-5-20	对应问题单SWPD05244
	BOOL bLoadDOMExceptionMsg;
	try
	{
		bLoadDOMExceptionMsg = DOMImplementation::loadDOMExceptionMsg(tocatch.code, errText, 2047);
	}
	catch (...)
	{
		DoError("%s","DOMImplementation::loadDOMExceptionMsg() error");
	}
	//the end	2004-5-20	对应问题单SWPD05244

	if (bLoadDOMExceptionMsg)
	{
		strReturn += STRING(",Message: ") + S(errText);
	}

	return strReturn;
}

/*******************************************************************
函数名称：STRING GetXMLExceptionMsg(const XMLException& tocatch)
功能描述：获取XMLException的错误信息
输入参数：const XMLException & tocatch，捕获到的DOMException异常;
输出参数：无
返回内容：返回异常错误码及信息字符串
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
STRING CXmlFileOpr::GetXMLExceptionMsg(const XMLException& tocatch)
{
	STRING strReturn("XMLException Message: ");
    strReturn += S(tocatch.getMessage());
	
	return strReturn;
}


/*******************************************************************
函数名称：void DoError(const char *pszFormat,...)
功能描述：错误处理: 控制台输出或记Trace文件
输入参数：const char *pszFormat,     格式化字符串；
		  ...,                       可选参数；
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



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
	//将错误信息输出到控制台
	cout<<szBuf<<endl;
#else
	//将错误信息输出到Trace文件
	TRACE("CXmlFileOpr", szBuf);
#endif

}

/*******************************************************************
函数名称：void SetModuleName(const char * const pszModuleName)
功能描述：设置模块名称
输入参数：const char * const pszModuleName,   业务模块名称；
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CXmlFileOpr::SetModuleName(const char * const pszModuleName)
{
	if (pszModuleName == 0)
	{
		return;
	}

	strncpy(m_szModuleName, pszModuleName, sizeof(m_szModuleName));
}

//设置字符集
void CXmlFileOpr::SetFontSet(const char * szFontSet)
{
	if (szFontSet == 0 || szFontSet[0] == 0)
	{
		return;
	}

	strncpy(m_szFontSet, szFontSet, sizeof(m_szFontSet));
}
