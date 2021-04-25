// query_executive.h: interface for the CQueryExecutive class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _QUERY_EXECUTIVE_H__
#define _QUERY_EXECUTIVE_H__

#include "../include/base_type.h"
#include "dda_proxy.h"
#include "task_manager.h"
extern STaskInfo g_TaskInfo;
class CTaskManager;

class CQueryExecutive  
{
    friend class CTaskManager;
public:
	int ListTaskResult(CParseResult *pParse);
	int Init(STRING &strPerfRoot,CTaskManager *pManager);
	CQueryExecutive();
	virtual ~CQueryExecutive();
#ifdef UNIT_TEST
public:
#else
protected:
#endif
	CTaskManager *m_pManager;
	void GenerateFileList(const char *szPerfTaskPath, char *szRDS, const char *szRDE);
	STRING m_szError;
    CQueryProxy    m_Query;       //具体查询性能数据文件的适配类
    STRING         m_szPerfRoot;  //性能数据的根目录
    LIST<STRING>   m_listFileName;//找到的符合日期的文件列表
    CDdaSet	       m_DdaSet;      //数据文件操作集合
    int            m_nPageLimit;  //一页报文的长度

};

#endif //_QUERY_EXECUTIVE_H_
