// collect_statistic.h: interface for the CCollectStatistic class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _COLLECT_STATISTIC_H
#define _COLLECT_STATISTIC_H
#include "../frame/msg_proc.h"
#include "../include/toolbox.h"
#include "resource.h"

#ifdef _PLATFORM_WIN32
#define S_ISDIR(mode)   (((mode) & S_IFMT) == S_IFDIR)
#endif

struct SFileInfo
{
    STRING strFileName;
    time_t tFileTime;
    int nFileSize;
};


class CCollectStatistic  
{
public:
	void ErrorOutput();
	int ProcStatisticCollectFiles(CParseResult *pParse);
	int Init(const char *szRoot);
	int SetFrontsaveRoot(const char *szRoot);
	CCollectStatistic();
	virtual ~CCollectStatistic();
protected:
    void SendCommonReport(CParseResult *pParse, const char *szContent, int nRes);
    void SendMMLReport(const char *szReport,CParseResult *pParse);
	int QueryAllPoint(CParseResult *pParse);
	int QuerySpecialPoint(CParseResult *pParse);
    STRING m_strFrontsaveRoot;     //ǰ���̸�Ŀ¼
    STRING m_strStartDate;         //��ʼ����
    STRING m_strEndDate;           //��������
    STRING m_strCurrentDate;       //��ǰ����
    STRING m_strColName;           //���������
    LIST<STRING> m_ColPointList;   //�ɼ����б� 
    STRING m_strError;
    int m_nPageLimit;              //ÿҳ��¼����
};

#endif // _COLLECT_STATISTIC_H
