#ifndef __XML_CDR_STAT_CFG_H__
#define __XML_CDR_STAT_CFG_H__

#include "../cdrstat/i_cdr_stat_cfg.h"
#include "xml_file_opr.h"


class CXmlCDRStatCfg: public ICDRStatCfg
{
public:
	CXmlCDRStatCfg();
	virtual ~CXmlCDRStatCfg();

public:
	 //���ò��������г�ʼ������
    virtual int Init(UINT4 uAccessPoint);

	//�������ͳ�������Ŀ��
	virtual void GetStatItemNameList(LIST<STRING>& listStatItemName);

	//���ָ��ͳ����Ŀ����ͳ��������Ϣ
	virtual  SStatItemCfg* GetStatItem(const char* const szStatItemName);	

	//���ָ��ͳ����Ŀ����ͳ��������Ϣ
	virtual void GetStatItem(const char* const szStatItemName, 
		                     LIST<SOneStatSubItemCfg*>& subItemList);
	
	//��������ͳ�������������Ϣ
	virtual int SetStatCfg(LIST<SOneStatSubItemCfg*> subItemList);

	//��ý�����
	virtual UINT4 GetAccessPoint();

	//��ý��������
	virtual const char* GetAccessPointName();

protected:
	//������
	UINT4    m_uAccessPoint;
	//���������
	STRING   m_sAPName;
	//ͳ�������ļ�����·����
	char     m_szFileName[MAX_PATH];
	//XML�ļ�����
	CXmlFileOpr m_xmlFileCfg;

private:
	//��ͳ������XML�ļ���DOM���ĸ���㴦����ͷ��Ϣ���
	DOMElement * AddHeadInfo(DOMElement *pRootEle);
};

#endif  //__XML_CDR_STAT_CFG_H__

