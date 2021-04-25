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
	 //设置参数，进行初始化操作
    virtual int Init(UINT4 uAccessPoint);    //接入点号

	//获得统计结果文件头信息
   	virtual void GetStatRstHead(SStatRstHead& statRstHead);

	//获得所有统计条件表达式
    virtual SStatExpIndexList* GetStatExpList(void);

	//获得指定时间范围内统计条件表达式的结果
	virtual int GetStatExpRst(const char* const szStatExpress, time_t tBeginTime, 
								time_t tEndTime, UINT4& nCDRCount, UINT4& nValueTotal); 

	//添加统计条件表达式
	virtual int AddStatExpress(LIST<char* >& StatExpList);

	//添加一天的所有统计表达式的统计结果
	virtual int AddStatRst(const SAllStatExpRst& statExpRst, time_t statTime);

	//清理统计结果文件中不再使用的条件表达式
	virtual int CleanStatExp(void);

	//返回接入点号
	virtual UINT4 GetAccessPoint();

	//]返回接入点名称
	virtual const char* GetAccessPointName(); 

	//根据接入点号返回所有表达式
	virtual void  GetAllExpress(LIST<STRING>& ExpressList);

	//获得上次统计时间
	virtual time_t GetLastStatTime();

    //刷新已执行统计操作的时间
    virtual int SetLastStatTime(time_t curtime);

protected:
    //供回滚操作使用
    DOMDocument* m_pDoc;
	//根据表达式查找其序号
	DOMNode* FindStatExpress(char* szExpress, UINT4& nIndex);

	//处理过程中出错要求回滚
	BOOL RollBack(void);

	//在一天的统计结果里根据索引好查找某统计表达式的统计结果
	void FindStatRst(DOMNode* pNode, UINT4 nIndex, UINT4& nCDRCount, UINT4& nValueTotal);
		
    //获取头节点
    DOMElement* GetHeadElement();
    
    //获取文件内容列表
    DOMNodeList* GetContentList();

protected:
	//接入点号
	UINT4    m_uAccessPoint;
	//接入点名称
	STRING   m_sAPName;
	//统计配置文件绝对路径名
	char     m_szFileName[MAX_PATH];

	//XML文件操作
	CXmlFileOpr m_xmlFileRst;

private:
	//加入文件头信息到XML文件中
	DOMElement* AddHeadInfo(DOMElement* pRootEle);
};
#endif //__XML_STAT_CFG_RST_H__
