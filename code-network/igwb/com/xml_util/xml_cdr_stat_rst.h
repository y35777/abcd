#ifndef __XML_STAT_CFG_RST_H__
#define __XML_STAT_CFG_RST_H__

#include "../cdrstat/i_cdr_stat_rst.h"
#include "xml_file_opr.h"


class CXmlCDRStatRst: public ICDRStatRst
{
public:
	CXmlCDRStatRst();
	virtual ~CXmlCDRStatRst();

public:
	 //���ò��������г�ʼ������
    virtual int Init(UINT4 uAccessPoint);    //������

	//���ͳ�ƽ���ļ�ͷ��Ϣ
   	virtual void GetStatRstHead(SStatRstHead& statRstHead);

	//�������ͳ���������ʽ
    virtual SStatExpIndexList* GetStatExpList(void);

	//���ָ��ʱ�䷶Χ��ͳ���������ʽ�Ľ��
	virtual int GetStatExpRst(const char* const szStatExpress, time_t tBeginTime, 
								time_t tEndTime, UINT4& nCDRCount, UINT4& nValueTotal); 

	//���ͳ���������ʽ
	virtual int AddStatExpress(LIST<char* >& StatExpList);

	//���һ�������ͳ�Ʊ��ʽ��ͳ�ƽ��
	virtual int AddStatRst(const SAllStatExpRst& statExpRst, time_t statTime);

	//����ͳ�ƽ���ļ��в���ʹ�õ��������ʽ
	virtual int CleanStatExp(void);

	//���ؽ�����
	virtual UINT4 GetAccessPoint();

	//]���ؽ��������
	virtual const char* GetAccessPointName(); 

	//���ݽ����ŷ������б��ʽ
	virtual void  GetAllExpress(LIST<STRING>& ExpressList);

	//����ϴ�ͳ��ʱ��
	virtual time_t GetLastStatTime();

    //ˢ����ִ��ͳ�Ʋ�����ʱ��
    virtual int SetLastStatTime(time_t curtime);

protected:
    //���ع�����ʹ��
    DOMDocument* m_pDoc;
	//���ݱ��ʽ���������
	DOMNode* FindStatExpress(char* szExpress, UINT4& nIndex);

	//��������г���Ҫ��ع�
	BOOL RollBack(void);

	//��һ���ͳ�ƽ������������ò���ĳͳ�Ʊ��ʽ��ͳ�ƽ��
	void FindStatRst(DOMNode* pNode, UINT4 nIndex, UINT4& nCDRCount, UINT4& nValueTotal);
		
    //��ȡͷ�ڵ�
    DOMElement* GetHeadElement();
    
    //��ȡ�ļ������б�
    DOMNodeList* GetContentList();

protected:
	//������
	UINT4    m_uAccessPoint;
	//���������
	STRING   m_sAPName;
	//ͳ�������ļ�����·����
	char     m_szFileName[MAX_PATH];

	//XML�ļ�����
	CXmlFileOpr m_xmlFileRst;

private:
	//�����ļ�ͷ��Ϣ��XML�ļ���
	DOMElement* AddHeadInfo(DOMElement* pRootEle);
};
#endif //__XML_STAT_CFG_RST_H__
