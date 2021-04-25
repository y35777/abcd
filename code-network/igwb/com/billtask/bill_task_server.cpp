#include "../include/toolbox.h"
#include "../include/format_out.h"
#include "bill_task_struct.h"
#include "bill_task_server.h"
#include "scp_rvy_task.h"
#include "../utility/mml_para.h"
#include "../utility/mml_report.h"
#include "resource.h"

/********************************************************
* ����: CBillTaskServer::CBillTaskServer
* ����: ���캯��
* ����: 
* ���: 
* ����: 
********************************************************/
CBillTaskServer::CBillTaskServer()
{
    m_lTimer5Sec = 0;
    m_nTaskID    = 0;
}

/********************************************************
* ����: CBillTaskServer::~CBillTaskServer
* ����: ��������
* ����: 
* ���: 
* ����: 
********************************************************/
CBillTaskServer::~CBillTaskServer()
{
    //
}

/********************************************************
* ����: CBillTaskServer::GetModuleType
* ����: ȡģ���
* ����: 
* ���: 
* ����: ģ���
********************************************************/
MODULE_TYPE CBillTaskServer::GetModuleType()
{
    return MT_BILLTASK;
}

/********************************************************
* ����: CBillTaskServer::InitInstance
* ����: ��ʼ��
* ����: 
* ���: 
* ����: �Ƿ�ɹ���0��ʾ�ɹ�
********************************************************/
int CBillTaskServer::InitInstance()
{
    int nRet = CWorkModule::InitInstance();
    if(nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //�������ļ��ж��������ŵ������ŵ�MAP����ʼ��m_MapDpctoAP
    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();

    //�ȴ�igwb.ini�ж������������������ʱ����int nAPCount;
    int nAPCount;
    nAPCount = IniFile.GetInt(CFG_SEC_COMMON, 
                              CFG_COMMON_KEY_APCOUNT,
                              CFG_COMMON_KEY_APCOUNT_DEFAULT);

    //�ٴ�scp_config.ini�а����¸�ʽ����ÿ���������ÿ�����ʱ�ʶ�µ�����������
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
		//�������ʱ�ʶ����������ʱ����int nNICound
        SNPRINTF(szAPSec, 25, "%s%d", CFG_SEC_AP, i);
        szAPSec[24] = '\0';
        int nNICound = CodeKeyFile.GetInt(szAPSec, 
                                          CFG_AP_KEY_NI_COUNT,
                                          CFG_AP_KEY_NI_COUNT_DEFAULT);
		int j;
		for(j = 0; j < nNICound; j++)
		{
			//����"DPC"+j�е������������ʱ����char szDpc[4096];
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
                //��pDpcPoint����ҵ���һ��','�ַ�
                p = strchr(pDpcPoint, ',');

                //���pDpcPoint��һ���ַ�����','�����ټ�������һ��','
                if(p == pDpcPoint)
                {
                    //����pDpcPointָ��
                    pDpcPoint++;

                    continue;
                }

				//��pDpcPoint����ҵ��ĵ�һ��','�ַ�ָ��֮����ַ���������ʱ����szDpcItem
                int nCopyCount = min(p - pDpcPoint, 19);
                memcpy(szDpcItem, pDpcPoint, nCopyCount);
                szDpcItem[nCopyCount] = '\0';

                //����pDpcPointָ��
                pDpcPoint = p + 1;

				//ȥ��szDpcItem���ҵĿո�
                StringLRTrim(szDpcItem);

				//���szDpcItemΪ""����continue
                if(szDpcItem[0] == '\0') continue;

				SScpApplyKey apply;
				apply.nNI = j;
                strncpy(apply.szDpc, szDpcItem, 7);
                apply.szDpc[6] = '\0';
				m_MapDpctoAP[apply] = i;
			}
		}
	}

    //ע�ᶨʱ��
    m_lTimer5Sec = SetTimer(FIVE_SECOND_IN_MILISEC);

    return 0;
}

/********************************************************
* ����: CBillTaskServer::ExitInstance
* ����: ɨβ����
* ����: 
* ���: 
* ����: 
********************************************************/
void CBillTaskServer::ExitInstance()
{
	//�ͷ�m_TaskList�����ж���
	while(m_TaskList.size())
	{
		m_TaskList.front()->WaitForExit();
		delete m_TaskList.front();
        m_TaskList.pop_front();
	}
	
    //ע����ʱ��
    KillTimer(m_lTimer5Sec);
    
    CWorkModule::ExitInstance();
}

/********************************************************
* ����: CBillTaskServer::ParseMsg
* ����: ��������ͷַ�
* ����: pMsgParsed : ������Ϣ֡�����ú���������ɾ��
* ���: 
* ����: 
********************************************************/
void CBillTaskServer::ParseMsg(MSG_INTRA* pMsgParsed)
{
	//�յ�CMD_MML����,��MML������ΪMML_SCP_RECOVERYʱ,����ProcScpRecovery����
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
* ����: CBillTaskServer::OnTimer
* ����: ��ʱ���¼��Ĵ�����ں���
* ����: nTimerID : ��ʱ��ID
* ���: 
* ����: 
********************************************************/
void CBillTaskServer::OnTimer(const long nTimerID)
{
    //���nTimerID == m_lTimer5Sec������ó�Ա����Timer5Sec()
    if(nTimerID == m_lTimer5Sec)
    {
        Timer5Sec();
    }

    CWorkModule::OnTimer(nTimerID);
}

/********************************************************
* ����: CBillTaskServer::Timer5Sec
* ����: 5�붨ʱ���¼��Ĵ�����ں�����ÿ��ֻɾ��һ�������
*       ������
* ����: 
* ���: 
* ����: 
********************************************************/
void CBillTaskServer::Timer5Sec()
{
    //����m_TaskList�е�ÿһ��CBillTask���󣬽��Ѿ���ɵ�����ɾ����
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
* ����: CBillTaskServer::ProcScpCloseAlarm
* ����: SCP�رո澯��������
* ����: pMsgParsed : ������Ϣָ֡��
* ���: 
* ����: 
********************************************************/
void CBillTaskServer::ProcScpCloseAlarm(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult**)(pMsgParsed->Data));
	//���ر���
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
* ����: CBillTaskServer::ProcScpRecovery
* ����: SCP�����ָ�������
* ����: pMsgParsed : ������Ϣָ֡��
* ���: 
* ����: 
********************************************************/
void CBillTaskServer::ProcScpRecovery(MSG_INTRA* pMsgParsed)
{
	int nRetCode = BR_ACCEPTED;
	CScpRvyTask* pTask  = NULL;

    CParseResult* pParseResult = *((CParseResult**)(pMsgParsed->Data));

	try
    {
	    //����m_TaskList�е�ÿһ��CBillTask�����жϸ������Ƿ�������
        LIST<CBillTask*>::iterator i;
	    for(i = m_TaskList.begin(); i != m_TaskList.end(); i++)
	    {
		    if((*i)->IsMatchedTask(pMsgParsed))
			{
				throw BR_PROCESSING;
			}
		}

	    //ȡ�������ָ��������
        struct SSCPBillRcvrApply LastScpApply;	//���»����ָ�����
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

        //���Ҹ������Ӧ�Ľ�����
		SScpApplyKey apply;
		apply.nNI = LastScpApply.nNI;
		strncpy(apply.szDpc, LastScpApply.szDpc, 7);
		apply.szDpc[6] = 0;

		if(m_MapDpctoAP.find(apply) == m_MapDpctoAP.end())
		{
			throw BR_DPCERR;
		}
		int nAP = m_MapDpctoAP[apply];

        //��дSRcvrCmd��Ϣ
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

	    //���������δ�������򴴽��µ�����
		pTask = new CScpRvyTask(nAP, &LastScpApply, &RcvrCmd, m_nTaskID);

		if(NULL == pTask)
        {
            delete [] RcvrCmd.szCmdLine;
            throw BR_SYSECPT;
        }

        //��ʼ��������
        nRetCode = pTask->Init();

        //���������
		pTask->SetActiveValue(TRUE);
		if(pTask->activate() != 0)
		{
			throw BR_THDFAIL;
		}

        //���������б�
		m_TaskList.push_back(pTask);
        m_nTaskID++;

	}
	catch(EBill_RVY_RETCODE nErrCode)
	//�����쳣
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

	//���ر���
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

