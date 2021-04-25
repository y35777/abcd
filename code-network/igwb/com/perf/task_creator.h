// task_creator.h: interface for the CTaskCreator class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TASK_CREATOR_H_
#define _TASK_CREATOR_H_

#include "../include/base_type.h"
#include "perf_task_global.h"
#include "ddaquery.h"
#include "resource.h"
#include <time.h>
#define REPORT_MORE  1
#define REPORT_NONE  0
#define REPORT_ERR  -1
#define CONVERT_CATCH_LEN 1024
#define INTMAP MAP<int,int,LESS<int> >
extern STaskInfo g_TaskInfo;

class CTaskCreator  
{
    friend class CTaskManager;
public:
   	void SetTaskFieldValue(STRING& szTskn, STRING &szField, STRING &szValue);
    int SaveTask(STRING &szTskn);
    STRING QueryTaskInfo(STRING &TaskName, STRING &Field);
	void QueryCounterInfo(STRING &TaskName,LIST<STRING> &listCounter);
	int ListTaskInfo(CParseResult *pParse);
	int DeleteTask(CParseResult *pParse);
	int CreateTask(CParseResult *pParse);
	int Init(STRING szPerfRoot);
    CTaskCreator();
    virtual ~CTaskCreator();
    const char *GetLastError(){
        return m_szError.c_str();
    }
#ifdef UNIT_TEST
public:
#else
protected:
#endif
    int GetEnumValue(CParseResult *pParse, int nID, int nMaxLen, INTMAP &EnumMap);
    void ConvertStrToVector(const char *pStr, VECTOR<int> &Vector);
    const char * ConvertCounter(const char *pCounter);
	void MakeSubTitle(CMMLReport *pReport, const char *pContent);
	const char * ConvertStatisticTime(const char *pTime);
	const char *ConvertStatisticTimeSet(const char *pSTS);
	const char *ConvertTimeType(const char *pTT);
	const char *AddDaysToDBFData(const char *pDBFData, int nDays);
	const char *ConvertDate(const char *pDate, const char *pSeparater = "-", int nNation = 1);
    int GenerateTaskInfoReport(STaskInfo *p,const char *pCmdLine, int nINFO, CDdaQuery *pQuery);
//	int GenerateCommonReport(CMMLReport *pReport,const char *pCmdLine,const char *pContent, int nRes);
	int GenerateCommonReport(STaskInfo *p,const char *pCmdLine,const char *pContent, int nRes);
	int GenerateInformation(CParseResult *pParse,SInformation *pInfo);
    CCreateProxy    m_CreateProxy;    //创建策略
    STRING          m_szPerfRoot;     //Perf的根路径
    CDdaSet         *m_pDdaSet;       //DBF操作类
    CDdaQuery       m_Query;          //DBF文件内容查询类
    STRING          m_szError;        //失败信息
    int             m_nPageLimit;     //一页尺寸
    char            m_szConvertCatch[CONVERT_CATCH_LEN]; //用于字符转换的缓冲区
};

#endif //_TASK_CREATOR_H
