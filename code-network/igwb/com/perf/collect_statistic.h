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
    STRING m_strFrontsaveRoot;     //前存盘根目录
    STRING m_strStartDate;         //起始日期
    STRING m_strEndDate;           //结束日期
    STRING m_strCurrentDate;       //当前日期
    STRING m_strColName;           //接入点名称
    LIST<STRING> m_ColPointList;   //采集点列表 
    STRING m_strError;
    int m_nPageLimit;              //每页记录个数
};

#endif // _COLLECT_STATISTIC_H
