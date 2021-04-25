//�ļ�����xml_file_opr.h
//���ܣ�  XML�ļ�������Xerces��װ

#ifndef _XML_FILE_OPR_H_
#define _XML_FILE_OPR_H_

//ʹ��igwbͷ�ļ�
#include "../include/precom.h"
#include "../include/toolbox.h"

//ʹ��Xercesͷ�ļ�
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>


//ʹ��XERCES�����ռ�
XERCES_CPP_NAMESPACE_USE

// ---------------------------------------------------------------------------
// StrX��һ���򵥹����࣬��������XMLCh* �� char *֮���ת�����ܣ�������ʹ����
//     ֮���Զ��ͷ��ڴ棻XMLChar *Ϊ���ַ���
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

//StrX֧��operator<<����
inline ostream& operator<<(ostream& target, const StrX& toDump)
{
    target << toDump.localForm();
    return target;
}

//��һ��XMLCh *�ַ���ת����char *�ַ���
#define S(str) (StrX(str).localForm())

#ifdef _PLATFORM_WIN32
#define SNWPRINTF _snwprintf
#else 
#define SNWPRINTF swprintf
#endif
// ---------------------------------------------------------------------------
// XStr��һ���򵥹����࣬��������char * �� XMLCh*֮���ת�����ܣ�������ʹ����
//     ֮���Զ��ͷ��ڴ�
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

//��һ��char *�ַ���ת����XMLCh *�ַ���
#define X(str) XStr(str).unicodeForm()


// ---------------------------------------------------------------------------
//XML�ļ�������
// CXmlFileOpr��һ����XML�ļ�ʹ��DOM�ӿڽ��в����ķ�װ�࣬��ʹ����Xereces�⡣
//     
// ---------------------------------------------------------------------------
class CXmlFileOpr
{
public:
	CXmlFileOpr();
	~CXmlFileOpr();

	//����������DOM�ĵ������ļ�������������ָ���ļ�
	BOOL InitXmlFile(const char * const szFilePath,
		             const char * const szRootName);
    
	//��DOM�ĵ�������д���ļ���
	BOOL WriteXmlFile();

	//�½����������㵽ָ��DOM�ĵ�
	DOMElement * AddRootElementToDOMDoc(DOMDocument *pDOMDoc, 
		                                const char * const pszRootName);

	//��ȡ�����
	DOMElement * GetRootElement();

	//��ָ������¼���ָ���ӽ��
	BOOL AddChildToNode(DOMElement *pNode, DOMElement *pChildNode);

	//��DOM�������ɾ��ָ����㼰���ӽ��
	BOOL  DeleteNode(DOMNode *& pDelNode);

	//��ָ����������޸�ָ������
	BOOL SetNodeAttrValue(DOMElement *pNode, const char * const pszAttrName,
		                  const char * const pszAttrValue);
    
	//��ָ����������޸�ֵ
	BOOL setNodeValue(DOMElement *pNode, const char * const pszValue);
    
	//��ȡָ������ָ������ֵ
	const XMLCh * GetNodeAttrValue(DOMElement *pNode, const char * const pszAttrName);

	//Ϊָ��DOM�ĵ�����һָ���ǵ�DOM���
	DOMElement * CreateNewElement(DOMDocument *pDOMDoc, 
		                          const char * const pszNodeName);

	//����һָ���ǵ�DOM���
	DOMElement * CreateNewElement(const char * const pszNodeName);

	//�õ�DOMDocumentָ��
    DOMDocument * GetDOMDocument();
	
	//�ɸ���㼰�ӽ���Ǻŵõ�����ָ���ӽ���б�
	DOMNodeList * GetChildListByTagName(DOMElement *pNode, const char * const pszTagName);
	
	//��ȡָ������ֵ���ӽ��ָ��
	DOMElement * GetNodeByAttrValue(DOMNodeList *pNodeList, const char * const pszAttrName,
		                            const char * const pszAttrValue);

    //���XML��DOM�ĵ�������
	BOOL ClearDOMDocument();

	//��ָ��DOM�ĵ�����������ԭDOM�ĵ�������
	BOOL SetDOMDocument(DOMDocument *pDOMDoc);

	//��XML�ļ���ʽ��ʾDOM�ĵ���
    void DisplayXmlFile();

	//����ģ������
	void SetModuleName(const char * const pszModuleName);

	//�����ַ���
    void SetFontSet(const char* szFontSet);
private:
	//XML�ļ���ȫ·����
	char m_szFilePath[MAX_PATH];
	
	//XML�ļ��������
	char m_szRootName[MAX_PATH];

	//ҵ��ģ������--���ڼ�Trace�ļ�
	char m_szModuleName[32];
	
	//�ַ���
	char m_szFontSet[100];
	
	//�Ƿ��ʼ��Xerces
	BOOL m_bInitXerces;
	
	//�ĵ��Ƿ��ɷ�����������������ɵ��ĵ����ڷ��������ɷ���������ɾ����
	//��createDocument()��cloneNode()�����ɵ��ĵ�����Ӧ��ʽ����release()ɾ��
	BOOL m_bParsedDoc;
	
	//DOM������
	DOMBuilder  *m_pDOMBuilder;
	
	//DOM�ĵ�ָ��
	DOMDocument *m_pDOMDoc;
	
	//DOMImplementationָ��
	DOMImplementation *m_pDOMimpl;

    /*
     *  ����Xerces��XMLPlatformUtils::Initialize()��XMLPlatformUtils::Terminate()
     *  ���������ڶ��߳���ͬʱ���á����Դ˴�����һʹ�ü�������ʹ�ü���С�ڵ���0ʱ��
     *  �Ž��г�ʼ�����ã�����0ʱ���н������á�������֤ͬʱʹ��Xercesʱֻ��һ����
     *  �̵���XMLPlatformUtils::Initialize()��XMLPlatformUtils::Terminate()������
     * 
     *  ͬʱʹ��ACE_Mutex�����߳�ͬ����
     *
     */

    //ʹ�ü���
    static int g_nRefCount;

    //�߳�ͬ��������
    static ACE_Mutex g_MutexRefCnt;

	//��ʼ��Xerces
	BOOL InitXMLPlatFormUtils();
	
	//�ͷ�Xerecs��Դ
	void UninitXMLPlatFormUtils();
	
	//�����ĵ���
    BOOL CreateDOMDocumentTree();

	//����һ����������XML�ļ�
	BOOL CreateNullXmlFile();

	//��ȡDOMException�Ĵ�����Ϣ
	STRING GetDOMExceptionMsg(const DOMException& tocatch);
    
	//��ȡXMLException�Ĵ�����Ϣ
	STRING GetXMLExceptionMsg(const XMLException& tocatch);

	//������
	void DoError(const char *pszFormat,...);
};

#endif //_XML_FILE_OPR_H_
