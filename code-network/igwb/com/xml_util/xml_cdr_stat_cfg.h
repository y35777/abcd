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
	 //设置参数，进行初始化操作
    virtual int Init(UINT4 uAccessPoint);

	//获得所有统计项的项目名
	virtual void GetStatItemNameList(LIST<STRING>& listStatItemName);

	//获得指定统计项目名的统计配置信息
	virtual  SStatItemCfg* GetStatItem(const char* const szStatItemName);	

	//获得指定统计项目名的统计配置信息
	virtual void GetStatItem(const char* const szStatItemName, 
		                     LIST<SOneStatSubItemCfg*>& subItemList);
	
	//设置所有统计子项的配置信息
	virtual int SetStatCfg(LIST<SOneStatSubItemCfg*> subItemList);

	//获得接入点号
	virtual UINT4 GetAccessPoint();

	//获得接入点名称
	virtual const char* GetAccessPointName();

protected:
	//接入点号
	UINT4    m_uAccessPoint;
	//接入点名称
	STRING   m_sAPName;
	//统计配置文件绝对路径名
	char     m_szFileName[MAX_PATH];
	//XML文件操作
	CXmlFileOpr m_xmlFileCfg;

private:
	//在统计配置XML文件的DOM树的根结点处加入头信息结点
	DOMElement * AddHeadInfo(DOMElement *pRootEle);
};

#endif  //__XML_CDR_STAT_CFG_H__

