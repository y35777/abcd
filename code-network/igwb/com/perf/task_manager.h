// task_manager.h: interface for the CTaskManager class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _TSK_MANAGER_H_
#define _TSK_MANAGER_H_
#include "../include/base_type.h"
#include "../utility/mml_para.h"
#include "../include/perf_item.h"
#include "task_creator.h"
#include "query_executive.h"
#include "perf_task_global.h"
#include "perf_task.h"
#include "resource.h"
#include <algorithm>

extern STaskInfo g_TaskInfo;
class CTaskCreator;
class CQueryExecutive;

class CTaskManager  
{
public:
    void Timer1Sec();
    int ProcStatistic(SPerfItem *pPerfs, int nCount);
	void DelOutDatedPerfFile();
	STRING QueryTaskInfo(STRING &TaskName, STRING &Field);
	void QueryCounterInfo(STRING &TaskName, LIST<STRING> &listCounter);
	void Dispatch(MSG_INTRA  *pMsg);
	int Init(STRING szPerfRoot);
    CTaskManager();
    virtual ~CTaskManager();
#ifdef UNIT_TEST
public:
#else
protected:
#endif
	int ProcListTaskResult(CParseResult *pParse);
	int ProcResumeTask(CParseResult *pParse);
	int ProcSuspendTask(CParseResult *pParse);
	int ProcDeleteTask(CParseResult *pParse);
	int ProcListTaskInfo(CParseResult *pParse);
	int ProcCreateTask(CParseResult *pParse);
    CTaskCreator *m_pTaskCreator;        //���񴴽���
                                        //���𴴽�M2000Ҫ���
                                        //����ͳ������
    CQueryExecutive *m_pQueryExecutive;  //����ͳ�ƽ���Ĳ�ѯ
};

#endif // _TSK_MANAGER_H_
