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
    CQueryProxy    m_Query;       //�����ѯ���������ļ���������
    STRING         m_szPerfRoot;  //�������ݵĸ�Ŀ¼
    LIST<STRING>   m_listFileName;//�ҵ��ķ������ڵ��ļ��б�
    CDdaSet	       m_DdaSet;      //�����ļ���������
    int            m_nPageLimit;  //һҳ���ĵĳ���

};

#endif //_QUERY_EXECUTIVE_H_
