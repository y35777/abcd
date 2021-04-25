#include "../include/toolbox.h"
#include "../include/format_out.h"
#include "bill_task_struct.h"
#include "bill_task_server.h"
#include "scp_rvy_task.h"
#include "../utility/mml_para.h"
#include "../utility/mml_report.h"
#include "resource.h"

/********************************************************
* 名称: CBillTaskServer::CBillTaskServer
* 功能: 构造函数
* 输入: 
* 输出: 
* 返回: 
********************************************************/
CBillTaskServer::CBillTaskServer()
{
    m_lTimer5Sec = 0;
    m_nTaskID    = 0;
}

/********************************************************
* 名称: CBillTaskServer::~CBillTaskServer
* 功能: 析构函数
* 输入: 
* 输出: 
* 返回: 
********************************************************/
CBillTaskServer::~CBillTaskServer()
{
    //
}

/********************************************************
* 名称: CBillTaskServer::GetModuleType
* 功能: 取模块号
* 输入: 
* 输出: 
* 返回: 模块号
********************************************************/
MODULE_TYPE CBillTaskServer::GetModuleType()
{
    return MT_BILLTASK;
}

/********************************************************
* 名称: CBillTaskServer::InitInstance
* 功能: 初始化
* 输入: 
* 输出: 
* 返回: 是否成功，0表示成功
********************************************************/
int CBillTaskServer::InitInstance()
{
    int nRet = CWorkModule::InitInstance();
    if(nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //从配置文件中读出信令点号到接入点号的MAP，初始化m_MapDpctoAP
    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();

    //先从igwb.ini中读出接入点数，赋给临时变量int nAPCount;
    int nAPCount;
    nAPCount = IniFile.GetInt(CFG_SEC_COMMON, 
                              CFG_COMMON_KEY_APCOUNT,
                              CFG_COMMON_KEY_APCOUNT_DEFAULT);

    //再从scp_config.ini中按如下格式读出每个接入点中每个网际标识下的所有信令点号
    STRING strCodeKeyPath = STRING(GetAppPath()) + 
                            S_SCP_PATH_BASE_ON_APP_PATH;
    CINIFile CodeKeyFile(strCodeKeyPath.c_str());
    CodeKeyFile.Open();

	int i;
    char szAPSec[25];
    char szDpcSec[25];
    char szDpc[4096];
    char szDpcItem[20];
	for(i = 1; i <= nAPCount; i++)
	{
		//读出网际标识数，赋给临时变量int nNICound
        SNPRINTF(szAPSec, 25, "%s%d", CFG_SEC_AP, i);
        szAPSec[24] = '\0';
        int nNICound = CodeKeyFile.GetInt(szAPSec, 
                                          CFG_AP_KEY_NI_COUNT,
                                          CFG_AP_KEY_NI_COUNT_DEFAULT);
		int j;
		for(j = 0; j < nNICound; j++)
		{
			//读出"DPC"+j中的配置项，赋给临时变量char szDpc[4096];
            SNPRINTF(szDpcSec, 25, "%s%d", CFG_AP_KEY_DCP, j);
            szDpcSec[24] = '\0';
            CodeKeyFile.GetString(szAPSec, szDpcSec,
                                  CFG_AP_KEY_DCP_DEFAULT,
                                  szDpc,
                                  4096);

			strcat(szDpc, ",");

			char * pDpcPoint = szDpc;			
            char * p = NULL; 
			while(pDpcPoint[0] != '\0')
			{
                //在pDpcPoint后查找到第一个','字符
                p = strchr(pDpcPoint, ',');

                //如果pDpcPoint第一个字符就是','，则再继续查找一下','
                if(p == pDpcPoint)
                {
                    //更新pDpcPoint指针
                    pDpcPoint++;

                    continue;
                }

				//将pDpcPoint与查找到的第一个','字符指针之间的字符串赋给临时变量szDpcItem
                int nCopyCount = min(p - pDpcPoint, 19);
                memcpy(szDpcItem, pDpcPoint, nCopyCount);
                szDpcItem[nCopyCount] = '\0';

                //更新pDpcPoint指针
                pDpcPoint = p + 1;

				//去掉szDpcItem左右的空格
                StringLRTrim(szDpcItem);

				//如果szDpcItem为""，则continue
                if(szDpcItem[0] == '\0') continue;

				SScpApplyKey apply;
				apply.nNI = j;
                strncpy(apply.szDpc, szDpcItem, 7);
                apply.szDpc[6] = '\0';
				m_MapDpctoAP[apply] = i;
			}
		}
	}

    //注册定时器
    m_lTimer5Sec = SetTimer(FIVE_SECOND_IN_MILISEC);

    return 0;
}

/********************************************************
* 名称: CBillTaskServer::ExitInstance
* 功能: 扫尾工作
* 输入: 
* 输出: 
* 返回: 
********************************************************/
void CBillTaskServer::ExitInstance()
{
	//释放m_TaskList中所有对象
	while(m_TaskList.size())
	{
		m_TaskList.front()->WaitForExit();
		delete m_TaskList.front();
        m_TaskList.pop_front();
	}
	
    //注销定时器
    KillTimer(m_lTimer5Sec);
    
    CWorkModule::ExitInstance();
}

/********************************************************
* 名称: CBillTaskServer::ParseMsg
* 功能: 命令解析和分发
* 输入: pMsgParsed : 命令消息帧，调用函数创建和删除
* 输出: 
* 返回: 
********************************************************/
void CBillTaskServer::ParseMsg(MSG_INTRA* pMsgParsed)
{
	//收到CMD_MML命令,且MML子命令为MML_SCP_RECOVERY时,调用ProcScpRecovery函数
    CParseResult* pParseResult = NULL;

	switch (pMsgParsed->CmdCode)
    {
    case CMD_MML:
		pParseResult = *((CParseResult**)(pMsgParsed->Data));
		switch (pParseResult->GetCmdCode())
		{
		case MML_SCP_RECOVERY:
			ProcScpRecovery(pMsgParsed);
			break;

		case MML_SCP_CLOSE_ALM:
			ProcScpCloseAlarm(pMsgParsed);
			break;

		default:
			break;
		}
        break;

	default:
		break;
	}
}

/********************************************************
* 名称: CBillTaskServer::OnTimer
* 功能: 定时器事件的处理入口函数
* 输入: nTimerID : 定时器ID
* 输出: 
* 返回: 
********************************************************/
void CBillTaskServer::OnTimer(const long nTimerID)
{
    //如果nTimerID == m_lTimer5Sec，则调用成员函数Timer5Sec()
    if(nTimerID == m_lTimer5Sec)
    {
        Timer5Sec();
    }

    CWorkModule::OnTimer(nTimerID);
}

/********************************************************
* 名称: CBillTaskServer::Timer5Sec
* 功能: 5秒定时器事件的处理入口函数，每次只删除一个已完成
*       的任务
* 输入: 
* 输出: 
* 返回: 
********************************************************/
void CBillTaskServer::Timer5Sec()
{
    //遍历m_TaskList中的每一个CBillTask对象，将已经完成的任务删除掉
    LIST<CBillTask*>::iterator i;
	for(i = m_TaskList.begin(); i != m_TaskList.end(); i++)
	{
		if(!(*i)->GetActiveValue())
		{
			delete (*i);
            m_TaskList.erase(i);
            break;
		}
	}
}

/********************************************************
* 名称: CBillTaskServer::ProcScpCloseAlarm
* 功能: SCP关闭告警请求处理函数
* 输入: pMsgParsed : 命令消息帧指针
* 输出: 
* 返回: 
********************************************************/
void CBillTaskServer::ProcScpCloseAlarm(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult**)(pMsgParsed->Data));
	//返回报告
    CMMLReport Report(S_BT_MML_TITLE);
    Report.MakeReportHead();
    Report.MakeCmdDisplay(pParseResult->GetCmdLine(), 0,
                          0, S_BT_MML_SUBTITLE);
    Report.AddString(S_SCP_MML_CLOSE_ALM_RESP);
    Report.AddString(STR_NEWLINE);
    Report.MakeReportTail();

    const char* pMMLReport  = Report.GetReport();
    int nLen          = strlen(pMMLReport) + 1;
    MSG_INTRA* pAns   = new(nLen) MSG_INTRA;
    if(NULL != pAns)
    {
        pAns->SenderPid   = PT_OM;
        pAns->SenderMid   = MT_BILLTASK;
        pAns->ReceiverPid = PT_OM;
        pAns->ReceiverMid = MT_MMLDEBUG;
        pAns->AppType     = pMsgParsed->AppType;
        pAns->ClientNo    = pMsgParsed->ClientNo;

        memcpy(pAns->Data, pMMLReport, nLen);
        CMsgProcessor::SendMsg(pAns);
    }
}

/********************************************************
* 名称: CBillTaskServer::ProcScpRecovery
* 功能: SCP话单恢复处理函数
* 输入: pMsgParsed : 命令消息帧指针
* 输出: 
* 返回: 
********************************************************/
void CBillTaskServer::ProcScpRecovery(MSG_INTRA* pMsgParsed)
{
	int nRetCode = BR_ACCEPTED;
	CScpRvyTask* pTask  = NULL;

    CParseResult* pParseResult = *((CParseResult**)(pMsgParsed->Data));

	try
    {
	    //遍历m_TaskList中的每一个CBillTask对象，判断该任务是否已启动
        LIST<CBillTask*>::iterator i;
	    for(i = m_TaskList.begin(); i != m_TaskList.end(); i++)
	    {
		    if((*i)->IsMatchedTask(pMsgParsed))
			{
				throw BR_PROCESSING;
			}
		}

	    //取出话单恢复命令参数
        struct SSCPBillRcvrApply LastScpApply;	//最新话单恢复请求
        memset(&LastScpApply, 0, sizeof(LastScpApply));

        strncpy(LastScpApply.szOp, 
                (char* )pParseResult->GetStringVal(1),
                17);
        LastScpApply.szOp[16] = '\0';

        strncpy(LastScpApply.szPwd, 
                (char* )pParseResult->GetStringVal(2),
                17);
        LastScpApply.szPwd[16] = '\0';

        strncpy(LastScpApply.szDir, 
                (char* )pParseResult->GetStringVal(3),
                33);
        LastScpApply.szDir[32] = '\0';

        strncpy(LastScpApply.szFileName, 
                (char* )pParseResult->GetStringVal(4),
                33);
        LastScpApply.szFileName[32] = '\0';

        strncpy(LastScpApply.szIP, 
                (char* )pParseResult->GetStringVal(5),
                17);
        LastScpApply.szIP[16] = '\0';

        pParseResult->GetDigitVal(&LastScpApply.nNI, 6);

        strncpy(LastScpApply.szDpc, 
                (char* )pParseResult->GetStringVal(7),
                7);
        LastScpApply.szDpc[6] = '\0';

        //查找该任务对应的接入点号
		SScpApplyKey apply;
		apply.nNI = LastScpApply.nNI;
		strncpy(apply.szDpc, LastScpApply.szDpc, 7);
		apply.szDpc[6] = 0;

		if(m_MapDpctoAP.find(apply) == m_MapDpctoAP.end())
		{
			throw BR_DPCERR;
		}
		int nAP = m_MapDpctoAP[apply];

        //填写SRcvrCmd信息
	    SRcvrCmd RcvrCmd;
	    RcvrCmd.uAppType  = pMsgParsed->AppType;
	    RcvrCmd.uClientNo = pMsgParsed->ClientNo;
	    RcvrCmd.uSerialNo = 0;
	    RcvrCmd.szCmdLine = new char[strlen(pParseResult->GetCmdLine()) + 1];
        if(NULL == RcvrCmd.szCmdLine)
        {
            throw BR_SYSECPT;
        }
	    strcpy(RcvrCmd.szCmdLine, pParseResult->GetCmdLine());

	    //如果该任务未启动，则创建新的任务
		pTask = new CScpRvyTask(nAP, &LastScpApply, &RcvrCmd, m_nTaskID);

		if(NULL == pTask)
        {
            delete [] RcvrCmd.szCmdLine;
            throw BR_SYSECPT;
        }

        //初始化该任务
        nRetCode = pTask->Init();

        //激活该任务
		pTask->SetActiveValue(TRUE);
		if(pTask->activate() != 0)
		{
			throw BR_THDFAIL;
		}

        //加入任务列表
		m_TaskList.push_back(pTask);
        m_nTaskID++;

	}
	catch(EBill_RVY_RETCODE nErrCode)
	//捕获异常
	{
		if (NULL != pTask)
		{
			delete pTask;
			pTask = NULL;
		}

        TRACE(THISMODULE, S_START_SCP_TASK_FAILED, nErrCode);
        MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
               S_START_SCP_TASK_FAILED, nErrCode);
		nRetCode = nErrCode;
	}

	//返回报告
    CMMLReport Report(S_BT_MML_TITLE);
    Report.MakeReportHead();
    Report.MakeCmdDisplay(pParseResult->GetCmdLine(), 0,
                          nRetCode, S_BT_MML_SUBTITLE);
    Report.MakeReportTail();

    const char* pMMLReport  = Report.GetReport();
    int nLen          = strlen(pMMLReport) + 1;
    MSG_INTRA* pAns   = new(nLen) MSG_INTRA;
    if(NULL != pAns)
    {
        pAns->SenderPid   = PT_OM;
        pAns->SenderMid   = MT_BILLTASK;
        pAns->ReceiverPid = PT_OM;
        pAns->ReceiverMid = MT_MMLDEBUG;
        pAns->AppType     = pMsgParsed->AppType;
        pAns->ClientNo    = pMsgParsed->ClientNo;

        memcpy(pAns->Data, pMMLReport, nLen);
        CMsgProcessor::SendMsg(pAns);
    }

	if (nRetCode == BR_ACCEPTED)
	{
		MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
           S_START_SCP_TASK_SUCCESS, m_nTaskID - 1);
	}
}

