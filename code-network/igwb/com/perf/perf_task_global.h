#ifndef _PRF_TSK_GBL__
#define _PRF_TSK_GBL__

#include "../include/base_type.h"
#include "../utility/mml_report.h"
#include "../utility/mml_para.h"
#include "../frame/wrk_module.h"
#include "perf_task.h"

#define  COUNTER_NUM 10
class CPerfTask;

typedef MAP<STRING,CPerfTask *,LESS<STRING> > TaskMap;

struct STaskInfo{
	TaskMap g_TaskMap;
	CMMLReport* g_pReport;
	int nReportCsn;
	STaskInfo(){
		//by ldf 2003-12-24 ÓëM2000Áªµ÷
		g_pReport = new CMMLReport("iGWB");
        nReportCsn = 1;
	};
	~STaskInfo(){
		if(g_pReport)
		{
			delete g_pReport,g_pReport = NULL;
		}
	};

};
void SendMMLReport(CMMLReport *pReport,UINT2 uAppType,UINT4 uIPAddr);

#endif //_PRF_TSK_GBL_
