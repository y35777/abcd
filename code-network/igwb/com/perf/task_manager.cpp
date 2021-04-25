// task_manager.cpp: implementation of the CTaskManager class.
//
//////////////////////////////////////////////////////////////////////
#include "task_manager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTaskManager::CTaskManager()
{
    m_pTaskCreator = NULL;
    m_pQueryExecutive = NULL;
}
CTaskManager::~CTaskManager()
{
    if(m_pTaskCreator)
        delete m_pTaskCreator,m_pTaskCreator = NULL;
    if(m_pQueryExecutive)
        delete m_pQueryExecutive, m_pQueryExecutive = NULL;
}
/***********************************************************
//初始化函数，创建m_pTaskCreator和m_pQueryExecutive成员变量
//并且利用M_pTaskCreator创建gTaskMap和gTaskIdToStringMap变量
************************************************************/
int CTaskManager::Init(STRING szPerfRoot)
{	
    m_pTaskCreator = new CTaskCreator;
    if(m_pTaskCreator->Init(szPerfRoot) != TRUE)
    {
		delete m_pTaskCreator,m_pTaskCreator = NULL;
        return FALSE; //如果初始化失败就返回FALSE；
    }
    m_pQueryExecutive = new CQueryExecutive;

    if(m_pQueryExecutive->Init(szPerfRoot,this) != TRUE)
    {
		delete m_pTaskCreator, m_pTaskCreator = NULL;
		delete m_pQueryExecutive, m_pQueryExecutive = NULL;
		return FALSE; //如果初始化失败就返回FALSE，并删掉m_pTaskCreator；
	}
    CINIFile ini(GetCfgFilePath());
    if(ini.Open())
    {
        m_pTaskCreator->m_nPageLimit = ini.GetInt(CFG_SEC_PERF,"M2000PageSize",48*1024);
        m_pQueryExecutive->m_nPageLimit = m_pTaskCreator->m_nPageLimit;
        ini.Close();
    }
    return TRUE;
}
//分发M2000发送过来的MML命令。
void CTaskManager::Dispatch(MSG_INTRA *pMsg)
{
    //如果是MML命令，提取CParseResult对象，然后根据pParse->GetCmdCode() 
    //调用处理MML命令的5个命令接口。
    //获得MML命令数据
    CParseResult *pParaResult = *(CParseResult **)(pMsg->Data);
    //根据MML命令处理消息
    switch (pParaResult->GetCmdCode())
    {
    case MML_CRE_PRFTSK:
        ProcCreateTask(pParaResult);
        break;
    case MML_RMV_PRFTSK:
        ProcDeleteTask(pParaResult);
        break;
    case MML_LST_PRFTSK:
        ProcListTaskInfo(pParaResult);
        break;
    case MML_ACT_PRFTSK:
        ProcResumeTask(pParaResult);
        break;
    case MML_SPD_PRFTSK:
        ProcSuspendTask(pParaResult);
        break;
    case MML_LST_TSKRST:
        ProcListTaskResult(pParaResult);
        break;
    default:
        break;
    }
}

int CTaskManager::ProcCreateTask(CParseResult *pParse)
{
    return m_pTaskCreator->CreateTask(pParse);
}

int CTaskManager::ProcListTaskInfo(CParseResult *pParse)
{
    return m_pTaskCreator->ListTaskInfo(pParse);
}

int CTaskManager::ProcDeleteTask(CParseResult *pParse)
{
    return m_pTaskCreator->DeleteTask(pParse);
}
int CTaskManager::ProcResumeTask(CParseResult *pParse)
{
    //在参数中取得任务名称,即第一个参数
    STRING szTaskName = pParse->GetStringVal(1);
    //在gTaskIdToStringMap中根据任务名称查询任务ID，
    //如果没有相应的ID则说明任务不存在，返回出错的报文。
	TaskMap::iterator it = g_TaskInfo.g_TaskMap.find(STRING(szTaskName));
	//如果能够查到任务ID，这说明存在相应的任务，
    //根据得到的任务ID在gTaskMap中取得的任务对象的指针，
    //将该任务的状态（m_nStatus）置为正常。
	char szResultDes[32];
    char szTmp[16];
    g_TaskInfo.g_pReport->MakeReportHead();
    if(it != g_TaskInfo.g_TaskMap.end())
    {
        STRING strTaskName = it->first;
        CPerfTask *pTask = it->second;
		pTask->m_pTaskInfo->m_nStatus = TASK_NORMAL;
        SNPRINTF(szTmp,16,"%d",TASK_NORMAL);
        m_pTaskCreator->SetTaskFieldValue(strTaskName,STRING("RUN"),STRING(szTmp));
        int nRet = m_pTaskCreator->SaveTask(strTaskName);
        if(!nRet)
        {
            g_TaskInfo.g_pReport->MakeCmdDisplay(pParse->GetCmdLine(),
                                                 g_TaskInfo.nReportCsn++,
                                                 -1,
                                                 "O&M");
            g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
            g_TaskInfo.g_pReport->AddString(S_PERF_ACT_PERF_TASK);
            g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
			
			//by ldf 
			int nStrLen = strlen(S_PERF_ACT_PERF_TASK);
			for(int j = 0; j < nStrLen; j++)
			{
				g_TaskInfo.g_pReport->AddString("-");
			}
			//g_TaskInfo.g_pReport->AddString(STR_TITLE_SPIN);
			//end
            
            g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
            g_TaskInfo.g_pReport->MakeReportItem(S_PERF_TASK_NAME,
                                                 szTaskName.c_str(),
                                                 24,
                                                 ALIGN_RIGHT);
            g_TaskInfo.g_pReport->MakeReportItem(S_PERF_OP_RESULT,
                                                 strerror(errno),
                                                 24,
                                                 ALIGN_RIGHT);
            SNPRINTF(szResultDes,32,S_PERF_RESULT_NUM,0);   
            g_TaskInfo.g_pReport->AddString(szResultDes);
            g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
            g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
            g_TaskInfo.g_pReport->MakeReportTail();
            SendMMLReport(g_TaskInfo.g_pReport,APP_MML_NORM,pParse->GetIP());
            return FALSE;
        }
        /*
		+++    HUAWEI iGWB        2002-05-25 14:51:04
        TASKCMD #0
        %%ACT TSK: TSKN="TSK";%%
        RETCODE = 0  执行成功

        激活性能测量任务
        ----------------
        任务名 =  10
        结果  =  操作成功  
        (结果个数 = 1)


        ---    END
		*/
		
        g_TaskInfo.g_pReport->MakeCmdDisplay(pParse->GetCmdLine(),
                                             g_TaskInfo.nReportCsn++,
                                             0,
                                             "O&M");
        g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
        g_TaskInfo.g_pReport->AddString(S_PERF_ACT_PERF_TASK);
        g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
		
		//by ldf 
		int nStrLen = strlen(S_PERF_ACT_PERF_TASK);
		for(int j = 0; j < nStrLen; j++)
		{
			g_TaskInfo.g_pReport->AddString("-");
		}
		//g_TaskInfo.g_pReport->AddString(STR_TITLE_SPIN);
		//end
        
        g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
        g_TaskInfo.g_pReport->MakeReportItem(S_PERF_TASK_NAME,
                                             szTaskName.c_str(),
                                             24,
                                             ALIGN_RIGHT);
        g_TaskInfo.g_pReport->MakeReportItem(S_PERF_OP_RESULT,
                                             S_PERF_ACT_SUCCESS,
                                             24,
                                             ALIGN_RIGHT);
        SNPRINTF(szResultDes,32,S_PERF_RESULT_NUM,1);       
    }
    else
    {
        g_TaskInfo.g_pReport->MakeCmdDisplay(pParse->GetCmdLine(),
                                             g_TaskInfo.nReportCsn++,
                                             -1,"O&M");
        g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
        g_TaskInfo.g_pReport->AddString(S_PERF_ACT_PERF_TASK);
        g_TaskInfo.g_pReport->AddString(STR_NEWLINE);

		//by ldf 
		int nStrLen = strlen(S_PERF_ACT_PERF_TASK);
		for(int j = 0; j < nStrLen; j++)
		{
			g_TaskInfo.g_pReport->AddString("-");
		}
		//g_TaskInfo.g_pReport->AddString(STR_TITLE_SPIN);
		//end
       
        g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
        g_TaskInfo.g_pReport->MakeReportItem(S_PERF_TASK_NAME,
                                             szTaskName.c_str(),
                                             24,
                                             ALIGN_RIGHT);
        g_TaskInfo.g_pReport->MakeReportItem(S_PERF_OP_RESULT,
                                             S_PERF_ACT_FAIL,
                                             24,
                                             ALIGN_RIGHT);
        SNPRINTF(szResultDes,32,S_PERF_RESULT_NUM,0);   
    }
    g_TaskInfo.g_pReport->AddString(szResultDes);
    g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
    g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
    g_TaskInfo.g_pReport->MakeReportTail();
	SendMMLReport(g_TaskInfo.g_pReport,APP_MML_NORM,pParse->GetIP());
	return TRUE;
}

int CTaskManager::ProcSuspendTask(CParseResult *pParse)
{
    //在参数中取得任务名称,即第一个参数
    STRING szTaskName = pParse->GetStringVal(1);
    //在gTaskIdToStringMap中根据任务名称查询任务ID，
    //如果没有相应的ID则说明任务不存在，返回出错的报文。
	TaskMap::iterator it = (g_TaskInfo.g_TaskMap).find(szTaskName);
	//如果能够查到任务ID，这说明存在相应的任务，
    //根据得到的任务ID在gTaskMap中取得的任务对象的指针，
    //将该任务的状态（m_nStatus）置为挂起。
	char szResultDes[32];
    char szTmp[16];
    g_TaskInfo.g_pReport->MakeReportHead();
    if(it != g_TaskInfo.g_TaskMap.end())
    {
        STRING strTaskName = it->first;
        CPerfTask *pTask = it->second;
		pTask->m_pTaskInfo->m_nStatus = TASK_SUSPEND;
        //by ldf 2004-04-23 M2000联调
        SNPRINTF(szTmp,16,"%d",TASK_SUSPEND);
        //end

        m_pTaskCreator->SetTaskFieldValue(strTaskName,STRING("RUN"),STRING(szTmp));
        int nRet = m_pTaskCreator->SaveTask(strTaskName);
        if(!nRet)
        {
            g_TaskInfo.g_pReport->MakeCmdDisplay(pParse->GetCmdLine(),
                                                 g_TaskInfo.nReportCsn++,
                                                 -1,"O&M");
            g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
            g_TaskInfo.g_pReport->AddString(S_PERF_SUSPEND_PERF_TASK);
            g_TaskInfo.g_pReport->AddString(STR_NEWLINE);

			//by ldf 
			int nStrLen = strlen(S_PERF_SUSPEND_PERF_TASK);
			for(int j = 0; j < nStrLen; j++)
			{
				g_TaskInfo.g_pReport->AddString("-");
			}
			//g_TaskInfo.g_pReport->AddString(STR_TITLE_SPIN);
			//end
            
            g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
            g_TaskInfo.g_pReport->MakeReportItem(S_PERF_TASK_NAME,
                                                 szTaskName.c_str(),
                                                 24,
                                                 ALIGN_RIGHT);
            g_TaskInfo.g_pReport->MakeReportItem(S_PERF_OP_RESULT,
                                                 strerror(errno),
                                                 24,
                                                 ALIGN_RIGHT);
            SNPRINTF(szResultDes,32,S_PERF_RESULT_NUM,0);   
            g_TaskInfo.g_pReport->AddString(szResultDes);
            g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
            g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
            g_TaskInfo.g_pReport->MakeReportTail();
            SendMMLReport(g_TaskInfo.g_pReport,APP_MML_NORM,pParse->GetIP());
            return FALSE;
        }
        
        /*
		+++    HUAWEI iGWB        2002-05-25 14:51:04
        TASKCMD #0
        %%ACT TSK: TSKN="TSK";%%
        RETCODE = 0  执行成功

        激活性能测量任务
        ----------------
        任务名 =  10
        结果  =  操作成功  
        (结果个数 = 1)


        ---    END
		*/
		
        g_TaskInfo.g_pReport->MakeCmdDisplay(pParse->GetCmdLine(),
                                             g_TaskInfo.nReportCsn++,
                                             0,
                                             "O&M");
        g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
        g_TaskInfo.g_pReport->AddString(S_PERF_SUSPEND_PERF_TASK);
        g_TaskInfo.g_pReport->AddString(STR_NEWLINE);

		//by ldf 
		int nStrLen = strlen(S_PERF_SUSPEND_PERF_TASK);
		for(int j = 0; j < nStrLen; j++)
		{
			g_TaskInfo.g_pReport->AddString("-");
		}
		//g_TaskInfo.g_pReport->AddString(STR_TITLE_SPIN);
		//end
        
        g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
        g_TaskInfo.g_pReport->MakeReportItem(S_PERF_TASK_NAME,
                                             szTaskName.c_str(),
                                             24,
                                             ALIGN_RIGHT);
        g_TaskInfo.g_pReport->MakeReportItem(S_PERF_OP_RESULT,
                                             S_PERF_SUSPEND_SUCCESS,
                                             24,
                                             ALIGN_RIGHT);
        SNPRINTF(szResultDes,32,S_PERF_RESULT_NUM,1);       
    }
    else
    {
        g_TaskInfo.g_pReport->MakeCmdDisplay(pParse->GetCmdLine(),
                                             g_TaskInfo.nReportCsn++,
                                             -1,"O&M");
        g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
        g_TaskInfo.g_pReport->AddString(S_PERF_SUSPEND_PERF_TASK);
        g_TaskInfo.g_pReport->AddString(STR_NEWLINE);

		//by ldf 
		int nStrLen = strlen(S_PERF_SUSPEND_PERF_TASK);
		for(int j = 0; j < nStrLen; j++)
		{
			g_TaskInfo.g_pReport->AddString("-");
		}
		//g_TaskInfo.g_pReport->AddString(STR_TITLE_SPIN);
		//end
        
        g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
        g_TaskInfo.g_pReport->MakeReportItem(S_PERF_TASK_NAME,
                                             szTaskName.c_str(),
                                             24,
                                             ALIGN_RIGHT);
        g_TaskInfo.g_pReport->MakeReportItem(S_PERF_OP_RESULT,
                                             S_PERF_SUSPEND_FAIL,
                                             24,
                                             ALIGN_RIGHT);
        SNPRINTF(szResultDes,32,S_PERF_RESULT_NUM,0);   
    }
    g_TaskInfo.g_pReport->AddString(szResultDes);
    g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
    g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
    g_TaskInfo.g_pReport->MakeReportTail();
	SendMMLReport(g_TaskInfo.g_pReport,APP_MML_NORM,pParse->GetIP());
	return TRUE;
}

int CTaskManager::ProcStatistic(SPerfItem *pPerfs, int nCount)
{
    //得到gTaskMap实例，然后遍历调用CPerfTask的
    //ProcStatistic(SPerfItem *pPerfs,int nCount)
	TaskMap::iterator it = g_TaskInfo.g_TaskMap.begin();
	CPerfTask *pTask = NULL;
	for( ; it != g_TaskInfo.g_TaskMap.end(); it ++)
	{
		pTask = it->second;
        pTask->ProcStatistic(pPerfs,nCount);
	}
    return TRUE;
}

int CTaskManager::ProcListTaskResult(CParseResult *pParse)
{
    return m_pQueryExecutive->ListTaskResult(pParse);
}

void CTaskManager::QueryCounterInfo(STRING &TaskName, LIST<STRING> &listCounter)
{
    m_pTaskCreator->QueryCounterInfo(TaskName,listCounter);
}

STRING CTaskManager::QueryTaskInfo(STRING &TaskName, STRING &Field)
{
    return m_pTaskCreator->QueryTaskInfo(TaskName,Field);
}

void CTaskManager::DelOutDatedPerfFile()
{
    //得到gTaskMap实例，然后遍历调用CPerfTask的
    //ProcStatistic(SPerfItem *pPerfs,int nCount)
	TaskMap::iterator it = g_TaskInfo.g_TaskMap.begin();
	CPerfTask *pTask = NULL;
	for( ; it != g_TaskInfo.g_TaskMap.end(); it ++)
	{
		pTask = it->second;
        pTask->DelPerf();
	}
    return;
}

void CTaskManager::Timer1Sec()
{
    //得到gTaskMap实例，然后遍历调用CPerfTask的
    //ProcStatistic(SPerfItem *pPerfs,int nCount)
	TaskMap::iterator it = g_TaskInfo.g_TaskMap.begin();
	CPerfTask *pTask = NULL;
	for( ; it != g_TaskInfo.g_TaskMap.end(); it ++)
	{
		pTask = it->second;
        pTask->Timer1Sec();
	}
    return;
}
