//文件名：xml_file_opr.h
//功能：  XML文件操作的Xerces封装

#ifndef _XML_FILE_OPR_H_
#define _XML_FILE_OPR_H_

//使用igwb头文件
#include "../include/precom.h"
#include "../include/toolbox.h"

//使用Xerces头文件
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>


//使用XERCES命名空间
XERCES_CPP_NAMESPACE_USE

// ---------------------------------------------------------------------------
// StrX是一个简单工具类，它简化了由XMLCh* 与 char *之间的转化功能，并能在使用完
//     之后自动释放内存；XMLChar *为宽字符串
// ---------------------------------------------------------------------------
class StrX
{
public :
    StrX(const XMLCh* const toTranscode)
    {
        fLocalForm = XMLString::transcode(toTranscode);
        if(NULL == fLocalForm)
        {
            fLocalForm = new char[1];
            fLocalForm[0] = '\0';
        }
    }

    ~StrX()
    {
        XMLString::release(&fLocalForm);
    }

    const char* localForm() const
    {
        return fLocalForm;
    }

private :
    char*   fLocalForm;
};

//StrX支持operator<<方法
inline ostream& operator<<(ostream& target, const StrX& toDump)
{
    target << toDump.localForm();
    return target;
}

//将一个XMLCh *字符串转化成char *字符串
#define S(str) (StrX(str).localForm())

#ifdef _PLATFORM_WIN32
#define SNWPRINTF _snwprintf
#else 
#define SNWPRINTF swprintf
#endif
// ---------------------------------------------------------------------------
// XStr是一个简单工具类，它简化了由char * 与 XMLCh*之间的转化功能，并能在使用完
//     之后自动释放内存
// ---------------------------------------------------------------------------
class XStr
{
public :
    XStr(const char* const toTranscode)
    {
#ifdef _PLATFORM_WIN32
        if (toTranscode == NULL || toTranscode[0] == '\0')
        {
            fUnicodeForm = new XMLCh[1];
            fUnicodeForm[0] = 0;
        }
        else
        {
            int neededLen = strlen(toTranscode);
            fUnicodeForm = new XMLCh[neededLen + 1];
            SNWPRINTF((wchar_t *)fUnicodeForm, neededLen + 1, L"%S", toTranscode);
            fUnicodeForm[neededLen] = 0;
        }
#else //_PLATFORM_WIN32
        fUnicodeForm = XMLString::transcode(toTranscode);
#endif //_PLATFORM_WIN32
    }

    ~XStr()
    {
#ifdef _PLATFORM_WIN32
        delete [] fUnicodeForm;
#else //_PLATFORM_WIN32
        XMLString::release(&fUnicodeForm);
#endif //_PLATFORM_WIN32
    }

    const XMLCh* unicodeForm() const
    {
        return fUnicodeForm;
    }

private :
    XMLCh*   fUnicodeForm;
};

//将一个char *字符串转化成XMLCh *字符串
#define X(str) XStr(str).unicodeForm()


// ---------------------------------------------------------------------------
//XML文件操作类
// CXmlFileOpr是一个对XML文件使用DOM接口进行操作的封装类，它使用了Xereces库。
//     
// ---------------------------------------------------------------------------
class CXmlFileOpr
{
public:
	CXmlFileOpr();
	~CXmlFileOpr();

	//剖析并生成DOM文档树，文件不存在则生成指定文件
	BOOL InitXmlFile(const char * const szFilePath,
		             const char * const szRootName);
    
	//将DOM文档树内容写入文件中
	BOOL WriteXmlFile();

	//新建并加入根结点到指定DOM文档
	DOMElement * AddRootElementToDOMDoc(DOMDocument *pDOMDoc, 
		                                const char * const pszRootName);

	//获取根结点
	DOMElement * GetRootElement();

	//在指定结点下加入指定子结点
	BOOL AddChildToNode(DOMElement *pNode, DOMElement *pChildNode);

	//在DOM结点树中删除指定结点及其子结点
	BOOL  DeleteNode(DOMNode *& pDelNode);

	//给指定结点加入或修改指定属性
	BOOL SetNodeAttrValue(DOMElement *pNode, const char * const pszAttrName,
		                  const char * const pszAttrValue);
    
	//给指定结点加入或修改值
	BOOL setNodeValue(DOMElement *pNode, const char * const pszValue);
    
	//获取指定结点的指定属性值
	const XMLCh * GetNodeAttrValue(DOMElement *pNode, const char * const pszAttrName);

	//为指定DOM文档生成一指点标记的DOM结点
	DOMElement * CreateNewElement(DOMDocument *pDOMDoc, 
		                          const char * const pszNodeName);

	//生成一指点标记的DOM结点
	DOMElement * CreateNewElement(const char * const pszNodeName);

	//得到DOMDocument指针
    DOMDocument * GetDOMDocument();
	
	//由父结点及子结点标记号得到所有指定子结点列表
	DOMNodeList * GetChildListByTagName(DOMElement *pNode, const char * const pszTagName);
	
	//获取指定属性值的子结点指针
	DOMElement * GetNodeByAttrValue(DOMNodeList *pNodeList, const char * const pszAttrName,
		                            const char * const pszAttrValue);

    //清空XML的DOM文档树内容
	BOOL ClearDOMDocument();

	//用指定DOM文档树内容重设原DOM文档树内容
	BOOL SetDOMDocument(DOMDocument *pDOMDoc);

	//用XML文件格式显示DOM文档树
    void DisplayXmlFile();

	//设置模块名称
	void SetModuleName(const char * const pszModuleName);

	//设置字符集
    void SetFontSet(const char* szFontSet);
private:
	//XML文件的全路径名
	char m_szFilePath[MAX_PATH];
	
	//XML文件根结点名
	char m_szRootName[MAX_PATH];

	//业务模块名称--用于记Trace文件
	char m_szModuleName[32];
	
	//字符集
	char m_szFontSet[100];
	
	//是否初始化Xerces
	BOOL m_bInitXerces;
	
	//文档是否由分析而来，因分析生成的文档属于分析器，由分析器负责删除，
	//而createDocument()或cloneNode()等生成的文档，则应显式调用release()删除
	BOOL m_bParsedDoc;
	
	//DOM分析器
	DOMBuilder  *m_pDOMBuilder;
	
	//DOM文档指针
	DOMDocument *m_pDOMDoc;
	
	//DOMImplementation指针
	DOMImplementation *m_pDOMimpl;

    /*
     *  由于Xerces的XMLPlatformUtils::Initialize()及XMLPlatformUtils::Terminate()
     *  函数不能在多线程中同时调用。所以此处采用一使用计数，当使用计数小于等于0时，
     *  才进行初始化调用，等于0时进行结束调用。这样保证同时使用Xerces时只有一个线
     *  程调用XMLPlatformUtils::Initialize()及XMLPlatformUtils::Terminate()方法。
     * 
     *  同时使用ACE_Mutex进行线程同步。
     *
     */

    //使用计数
    static int g_nRefCount;

    //线程同步互斥量
    static ACE_Mutex g_MutexRefCnt;

	//初始化Xerces
	BOOL InitXMLPlatFormUtils();
	
	//释放Xerecs资源
	void UninitXMLPlatFormUtils();
	
	//生成文档树
    BOOL CreateDOMDocumentTree();

	//生成一仅含根结点的XML文件
	BOOL CreateNullXmlFile();

	//获取DOMException的错误信息
	STRING GetDOMExceptionMsg(const DOMException& tocatch);
    
	//获取XMLException的错误信息
	STRING GetXMLExceptionMsg(const XMLException& tocatch);

	//错误处理
	void DoError(const char *pszFormat,...);
};

#endif //_XML_FILE_OPR_H_
