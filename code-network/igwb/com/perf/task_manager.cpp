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
//��ʼ������������m_pTaskCreator��m_pQueryExecutive��Ա����
//��������M_pTaskCreator����gTaskMap��gTaskIdToStringMap����
************************************************************/
int CTaskManager::Init(STRING szPerfRoot)
{	
    m_pTaskCreator = new CTaskCreator;
    if(m_pTaskCreator->Init(szPerfRoot) != TRUE)
    {
		delete m_pTaskCreator,m_pTaskCreator = NULL;
        return FALSE; //�����ʼ��ʧ�ܾͷ���FALSE��
    }
    m_pQueryExecutive = new CQueryExecutive;

    if(m_pQueryExecutive->Init(szPerfRoot,this) != TRUE)
    {
		delete m_pTaskCreator, m_pTaskCreator = NULL;
		delete m_pQueryExecutive, m_pQueryExecutive = NULL;
		return FALSE; //�����ʼ��ʧ�ܾͷ���FALSE����ɾ��m_pTaskCreator��
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
//�ַ�M2000���͹�����MML���
void CTaskManager::Dispatch(MSG_INTRA *pMsg)
{
    //�����MML�����ȡCParseResult����Ȼ�����pParse->GetCmdCode() 
    //���ô���MML�����5������ӿڡ�
    //���MML��������
    CParseResult *pParaResult = *(CParseResult **)(pMsg->Data);
    //����MML�������Ϣ
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
    //�ڲ�����ȡ����������,����һ������
    STRING szTaskName = pParse->GetStringVal(1);
    //��gTaskIdToStringMap�и����������Ʋ�ѯ����ID��
    //���û����Ӧ��ID��˵�����񲻴��ڣ����س���ı��ġ�
	TaskMap::iterator it = g_TaskInfo.g_TaskMap.find(STRING(szTaskName));
	//����ܹ��鵽����ID����˵��������Ӧ������
    //���ݵõ�������ID��gTaskMap��ȡ�õ���������ָ�룬
    //���������״̬��m_nStatus����Ϊ������
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
        RETCODE = 0  ִ�гɹ�

        �������ܲ�������
        ----------------
        ������ =  10
        ���  =  �����ɹ�  
        (������� = 1)


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
    //�ڲ�����ȡ����������,����һ������
    STRING szTaskName = pParse->GetStringVal(1);
    //��gTaskIdToStringMap�и����������Ʋ�ѯ����ID��
    //���û����Ӧ��ID��˵�����񲻴��ڣ����س���ı��ġ�
	TaskMap::iterator it = (g_TaskInfo.g_TaskMap).find(szTaskName);
	//����ܹ��鵽����ID����˵��������Ӧ������
    //���ݵõ�������ID��gTaskMap��ȡ�õ���������ָ�룬
    //���������״̬��m_nStatus����Ϊ����
	char szResultDes[32];
    char szTmp[16];
    g_TaskInfo.g_pReport->MakeReportHead();
    if(it != g_TaskInfo.g_TaskMap.end())
    {
        STRING strTaskName = it->first;
        CPerfTask *pTask = it->second;
		pTask->m_pTaskInfo->m_nStatus = TASK_SUSPEND;
        //by ldf 2004-04-23 M2000����
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
        RETCODE = 0  ִ�гɹ�

        �������ܲ�������
        ----------------
        ������ =  10
        ���  =  �����ɹ�  
        (������� = 1)


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
    //�õ�gTaskMapʵ����Ȼ���������CPerfTask��
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
    //�õ�gTaskMapʵ����Ȼ���������CPerfTask��
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
    //�õ�gTaskMapʵ����Ȼ���������CPerfTask��
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
