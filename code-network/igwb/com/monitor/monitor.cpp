/***************************************************************************
 *��Ʒ��    ��iGateway Bill V200
 *ģ����    ��monitor
 *�ļ���    ��monitor.h
 *������Ϣ  ����CMonitor�Ķ��塣
 *�汾˵��  ��V200R001i00M00B00D1130A
 *��Ȩ��Ϣ  ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����      ������ƽ
 *����ʱ��  ��2001-10-30
 *�޸ļ�¼  ��
*****************************************************************************/

#include "monitor.h"
#include "proc_child.h"
#include "../utility/mml_para.h"
#include "../utility/mml_report.h"
#include "resource.h"

long g_lDebugVersion = 0;

/****************************************************************************
����ԭ��	public: CMonitor()
��������	���캯��
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
*****************************************************************************/
CMonitor:: CMonitor()
{
}

/****************************************************************************
����ԭ��	public: ~CMonitor()
��������	��������
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
*****************************************************************************/
CMonitor:: ~CMonitor()
{
    //
}

/****************************************************************************
����ԭ��	public: virtual int InitInstance()
��������	��ʼ������
�������	�ޡ�
�������	�ޡ�
����ֵ  	���س�ʼ���Ƿ�ɹ���ERR_SUCCESS��0����ʾ�ɹ�������ֵ��ʾʧ�ܡ�
�׳��쳣	�ޡ�
*****************************************************************************/
int CMonitor::InitInstance()
{
    //���û���ĳ�ʼ������
    int nRet = CWorkModule::InitInstance();
	
    if (ERR_SUCCESS != nRet)
    {
        m_nStatus = KERNEL_FAILURE;
        return nRet;
    }

    //�������ļ��ж�ȡ�Ƿ������ڵ��Ե���Ϣ
    CINIFile *pIniFile = new CINIFile(GetCfgFilePath());
    if (!pIniFile->Open()) 
    {
        delete pIniFile;
        m_nStatus = KERNEL_FAILURE;

        return ERR_OPEN_CFG_FILE;
    }

    //ȷ���Ƿ�Ϊ���԰汾
    g_lDebugVersion = pIniFile->GetInt(CFG_SEC_KERNEL, 
                                       CFG_KERNEL_KEY_DEBUGVERSION, 
                                       CFG_KERNEL_KEY_DEBUGVERSION_DEFAULT);
    pIniFile->Close();
    delete pIniFile;
	
    //ע�ᶨʱ������ʼ��״̬
    m_lTimerID5Sec = SetTimer(FIVE_SECOND_IN_MILISEC);
    m_nStatus = KERNEL_INIT_PENDING;

    //��ʼ��ҵ����̹����߶����ڸú����д������еĽ������̺Ͳ���
    //ά�����̣�����������Щ�ӽ���
    nRet = m_ProcMgr.InitInstance();
    if (ERR_SUCCESS != nRet)
    {
        m_nStatus = KERNEL_FAILURE;
        return nRet;
    }

    m_nCounter = 0;
	m_bClusterprocIsExist = FALSE;
    m_lTimerID1Sec = SetTimer(ONE_SECOND_IN_MILISEC);
    //�ڷǵ��԰汾�������������̺Ͳ���ά������
    if (0 == g_lDebugVersion)
    {
       //�����ӽ���
       nRet = m_ProcMgr.Start();
       if (ERR_SUCCESS != nRet)
       {
           m_nStatus = KERNEL_FAILURE;
           return nRet;
       }
    }
	
    return ERR_SUCCESS;
}

/**********************************************************************************
����ԭ��	public: virtual void ExitInstance()
��������	�˳�ʱ��������
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
***********************************************************************************/
void CMonitor::ExitInstance()
{
    DeActive();

    //ע����ʱ��
    KillTimer(m_lTimerID5Sec);
    
    //ע����ʱ��
    KillTimer(m_lTimerID1Sec);

    //���û����ExitInstance����
    CWorkModule::ExitInstance();
}

/*********************************************************************************
����ԭ��	protected: void  ParseMsg(MSG_INTRA* pMsg)
��������	��Ϣ�ַ�����
�������	������ģ�鷢��������Ϣ��
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
**********************************************************************************/
void CMonitor::ParseMsg(MSG_INTRA* pMsg)
{
    //by ldf 2002.4.9 ��Ӧ���ⵥ D12433
    static UINT2   LastAppType;                    //��¼Ӧ������
    static UINT2   LastClientNo;                   //��¼ĳ�����͵�MML�ͻ��˵ı��
    static UINT4   LastIPAddr;                     //��¼MML�ͻ��˵�IP��ַ
    static char    LastUserName[48];               //��¼MML�û���
    //�޸����

    switch (pMsg->CmdCode)
    {
        //ҵ����̷����ĳ�ʼ��Ӧ��
        case CMD_ANS_INIT:
        {
            m_ProcMgr.ProcInitAns((PROCESS_TYPE)(pMsg->SenderPid));
		
            if (g_lDebugVersion && (KERNEL_ACTIVE != m_nStatus)) 
            {
                m_nStatus = KERNEL_ACTIVING;
            }

            break;
        }

        //˫����ת�����ļ�������
        case CMD_ACTIVE:
        {
            //ֻ���ڷǵ��԰汾�ҵ�ǰ״̬ΪREADY״̬��,���ܼ�������
            if ((0 == g_lDebugVersion) && (KERNEL_READY == m_nStatus)) 
            {
                 MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, SZ_MSG_BEGIN_ACTIVE);
                 TRACE(THISMODULE, SZ_MSG_BEGIN_ACTIVE);

                 Active();
            }

            break;
        }

        //ҵ����̷����ļ���Ӧ��
        case CMD_ANS_ACTIVE:
        {
            m_ProcMgr.ProcActiveAns((PROCESS_TYPE)(pMsg->SenderPid));

            break;
        }

        //˫����ת������ȥ��������
        case CMD_DEACTIVE:
        {
            if ((KERNEL_DEACTIVING != m_nStatus) 
             && (KERNEL_DEACTIVE != m_nStatus))
            {
                MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, SZ_MSG_BEGIN_DEACTIVE);

                TRACE(THISMODULE, SZ_MSG_BEGIN_DEACTIVE);
             
                DeActive();

                SendAnsToClstrInterface(CMD_ANS_DEACTIVE, TRUE);
            }

            break;
        }

        //˫����ת�������˳�����
        case CMD_EXIT:
        {
            m_nStatus = KERNEL_SHUTDOWNING;
            SendMsgToManager(PT_SELF, CMD_QUIT);

            SLog log;
 
            log.LogType      = SLog::LOG_SYS;
            log.OperResult   = SLog::OPER_SUCCESS;
            log.uIPAddr      = 0;
            log.szOperCmd[0] = '\0';
            strcpy(log.szCmdDesc, SZ_CMD_EXIT);
            strcpy(log.szUserName, THISMODULE);
            
            WriteLog(&log);

            break;
        }

        //����MML SERVERת����MML�ͻ��˵���Ϣ��Ŀǰֻ����һ������
        //��"��������"
        case CMD_MML:
        {
            CParseResult pr(pMsg);
            pr.CreateParseResult(pMsg);

            if(MML_PRODUCE_CDR == pr.GetCmdCode())
            {
                m_ProcMgr.ProduceCDR();
                
                MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, GetStringFromID(5002));
                //��Ҫ������д��־
                SLog log;
                log.LogType    = SLog::LOG_OPER;
                log.uIPAddr    = pMsg->IPAddr;
                log.OperResult = SLog::OPER_SUCCESS;
                
                strcpy(log.szUserName, pMsg->UserName);
                strcpy(log.szOperCmd, pr.GetCmdName());
                strcpy(log.szCmdDesc, pr.GetCmdDspName());
                strcpy(LastUserName, pMsg->UserName);                
                WriteLog(&log); 
                
                //���췵�صı���
                CMMLReport Report(THISMODULE);
                Report.MakeReportHead(); 
                Report.MakeCmdDisplay(pr.GetCmdLine(), 0, ERR_SUCCESS, THISMODULE);
                //���챨��β 
                Report.MakeReportTail();
                
                const char* pChar = Report.GetReport();
                int nLen = strlen(pChar); 
                //��MML�ͻ���Ӧ��
                MSG_INTRA* pAns = new(nLen + 1)MSG_INTRA;
                strcpy((char *)(pAns->Data), (const char *)pChar);
                
                pAns->SenderPid     = GetProcessType();
                pAns->SenderMid     = GetModuleType();
                pAns->ReceiverPid   = PT_OM; 
                pAns->ReceiverMid   = MT_MMLDEBUG;
                pAns->AppType       = pMsg->AppType;
                pAns->ClientNo      = pMsg->ClientNo;
                pAns->IPAddr        = pMsg->IPAddr; 
                
                LastAppType         = pAns->AppType;                    //��¼Ӧ������
                LastClientNo        = pAns->ClientNo;                   //��¼ĳ�����͵�MML�ͻ��˵ı��
                LastIPAddr          = pAns->IPAddr;                     //��¼MML�ͻ��˵�IP��ַ              
                
                SendMsg(pAns);    //������Ϣ
            }            

            if (MML_AUP == pr.GetCmdCode())  
            {
                // ������ 2002-03-13 add ���ⵥD11733
                if(m_ProcMgr.GetAuxUpgStatus() == AUXUPG_START)
                {
                    WriteLog(MTS_MONITOR,S_MONITOR_REVMOREAUXUPG,SLog::OPER_SUCCESS);
                    break;
                }
                //���ӽ���

                //������ 2002-03-13 add ���ⵥD11733
                CMMLReport Report(THISMODULE);
                Report.MakeReportHead();            //���챨��ͷ
                //�����������
                Report.MakeCmdDisplay(pr.GetCmdLine(), 0, ERR_SUCCESS, THISMODULE);
                if (KERNEL_ACTIVE == m_nStatus && m_ProcMgr.GetAuxUpgStatus() == AUXUPG_STOPPED)
                {
                    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, SZ_AUP_IS_GOINGON);
                    m_ProcMgr.ProcAuxUpgradeCmd();
                    
                    //���췵�ر�������
                    Report.AddString(SZ_AUP_IS_GOINGON);
                   
                    //��Ҫ������д��־
                    SLog log;
                    log.LogType    = SLog::LOG_OPER;
                    log.uIPAddr    = pMsg->IPAddr;
                    log.OperResult = SLog::OPER_SUCCESS;

                    strcpy(log.szUserName, pMsg->UserName);
                    strcpy(log.szOperCmd, SZ_MML_CMD_AUP);
                    strcpy(log.szCmdDesc, SZ_MML_CMDDESC_AUP);

                    //by ldf 2002.4.9 ��Ӧ���ⵥD12433
                    strcpy(LastUserName, pMsg->UserName);
                    //�޸����

                    WriteLog(&log); 
                }
                else if(m_ProcMgr.GetAuxUpgStatus() == AUXUPG_COMPLETED)
                {
                    //���췵�ر�������
                    Report.AddString(SZ_AUP_IS_COMPLETE);
                }
                //��������
                Report.AddString(STR_NEWLINE);
                //���챨��β 
                Report.MakeReportTail();
                
                const char* pChar = Report.GetReport();
                int nLen = strlen(pChar); 
                
                //��MML�ͻ���Ӧ������������
                MSG_INTRA* pAns = new(nLen + 1)MSG_INTRA;
                strcpy((char *)(pAns->Data), (const char *)pChar);
                
                pAns->SenderPid     = GetProcessType();
                pAns->SenderMid     = GetModuleType();
                pAns->ReceiverPid   = PT_OM; 
                pAns->ReceiverMid   = MT_MMLDEBUG;
                pAns->AppType       = pMsg->AppType;
                pAns->ClientNo      = pMsg->ClientNo;
                pAns->IPAddr        = pMsg->IPAddr; 

                //by ldf 2002.4.9 ��Ӧ���ⵥ D12433
                LastAppType         = pAns->AppType;                    //��¼Ӧ������
                LastClientNo        = pAns->ClientNo;                   //��¼ĳ�����͵�MML�ͻ��˵ı��
                LastIPAddr          = pAns->IPAddr;                     //��¼MML�ͻ��˵�IP��ַ
                //�޸����
 
                SendMsg(pAns);    //������Ϣ
            } //�޸Ľ���

            break;
        }

        //by ldf 2002.4.8 ��Ӧ���ⵥ D12433
       case CMD_AUXUPGRADE_FAIL:
        {
            if(m_ProcMgr.GetAuxUpgStatus() == AUXUPG_START)
            {

            MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, SZ_AUP_IS_FAILED);
	
            CMMLReport Report(THISMODULE);
			
            Report.MakeReportHead();            //���챨��ͷ
			
            //�����������
            Report.MakeCmdDisplay(SZ_MML_CMD_AUP_REDISPLAY, 0, ERR_FAIL, THISMODULE);
            //���췵�ر�������
            Report.AddString(SZ_AUP_IS_FAILED);
            //��������
            Report.AddString(STR_NEWLINE);
            //���챨��β 
            Report.MakeReportTail();
			
            const char* pChar = Report.GetReport();
            int nLen = strlen(pChar); 
			
            //��MML�ͻ���Ӧ������������
            MSG_INTRA* pAns = new(nLen + 1)MSG_INTRA;
            strcpy((char *)(pAns->Data), (const char *)pChar);

            pAns->SenderPid     = GetProcessType();
            pAns->SenderPid     = GetModuleType();
            pAns->ReceiverPid   = PT_OM; 
            pAns->ReceiverMid   = MT_MMLDEBUG;
            pAns->AppType       = LastAppType;
            pAns->ClientNo      = LastClientNo;
            pAns->IPAddr        = LastIPAddr; 

            SendMsg(pAns);    //������Ϣ

            //��Ҫ������д��־
            SLog log;
            log.LogType    = SLog::LOG_OPER;
            log.uIPAddr    = LastIPAddr;
            log.OperResult = SLog::OPER_FAIL;

            strcpy(log.szUserName, LastUserName);
            strcpy(log.szOperCmd, SZ_MML_CMD_AUP);
            strcpy(log.szCmdDesc, SZ_AUP_IS_FAILED);
			
            WriteLog(&log); 

            //�Ѹ���������״̬ ��AUXUPG_START �û� AUXUPG_STOPPED
            m_ProcMgr.AuxUpgFailSetStatus();

            }

            break;
        }

        case CMD_ANS_AUXUPGRADE:
        {
            m_ProcMgr.ProcAuxUpgradeAns((PROCESS_TYPE)(pMsg->SenderPid));
        
            break;
        }
        case CMD_ANS_PROC_STATUS:
		{
			SendAnsProcStatus();
			break;
		}
		case CMD_ANS_CLSTR_STATUS:
		{
			m_nCounter = 0;
			m_bClusterprocIsExist = TRUE;			
			break;
		}


        default:
        {
            break;
        }
    }
}

/**************************************************************************************
����ԭ��	protected: virtual MODULE_TYPE GetModuleType();
��������	����ģ������
�������	�ޡ�
�������	�ޡ�
����ֵ  	ģ�����͡�
�׳��쳣	�ޡ�
***************************************************************************************/
MODULE_TYPE CMonitor::GetModuleType()
{
    return MT_MONITOR;
}

/**************************************************************************************
����ԭ��	protected: int Active()
��������	���𼤻����
�������	�ޡ�
�������	�ޡ�
����ֵ  	������𼤻�����ɹ����򷵻�ERR_SUCCESS �����򷵻ط���ֵ��
�׳��쳣	�ޡ�
***************************************************************************************/
int CMonitor::Active()
{
    switch (m_nStatus)
    {
        //������ڼ�������Ѿ������ֱ�ӷ���
        case KERNEL_ACTIVING:
        case KERNEL_ACTIVE: 
        {
            return ERR_SUCCESS;
        }

        case KERNEL_READY:
        {
            if (m_ProcMgr.Active() != ERR_SUCCESS) 
            {
                m_nStatus = KERNEL_FAILURE;
                return ERR_FAIL;
            }

            m_nStatus = KERNEL_ACTIVING;
            return ERR_SUCCESS;
        }

        default:
        {
            return ERR_FAIL;
        }
    }
}

/*********************************************************************************************
����ԭ��	protected: void  DeActive()
��������	ִ��ȥ������������������϶��ɹ�
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
**********************************************************************************************/
void CMonitor:: DeActive()
{
    if(KERNEL_DEACTIVE == m_nStatus)
    {
        return;
    }

    m_nStatus = KERNEL_DEACTIVING;

    m_ProcMgr.DeActive();     //ͨ�����̹�����ȥ��������ӽ���

    m_nStatus = KERNEL_DEACTIVE;
}

/*********************************************************************************************
����ԭ��	protected: void  OnTimer(const long nTimerID);
��������	��ʱ������
�������	ʱ��ID ��ֻ����
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
**********************************************************************************************/
void CMonitor::OnTimer(const long nTimerID)
{ 
    if (nTimerID == m_lTimerID5Sec)
    {
        Timer5Sec();
    }
    //added by maimaoshi,2003-03-15
    if (nTimerID == m_lTimerID1Sec)
	{
		m_nCounter++;

		if (m_nCounter > 6)
		{
			m_nCounter = 6;
			m_bClusterprocIsExist = FALSE;
		}

    }
    //end

    CWorkModule::OnTimer(nTimerID);
}

/********************************************************************************************
����ԭ��	protected: void  Timer5Sec();
��������	5�붨ʱ������
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
*********************************************************************************************/
void CMonitor::Timer5Sec()
{
    if (0 == g_lDebugVersion)
    //�ǵ��԰汾ʱ��
    {
        m_ProcMgr.Timer5Sec();
    }

    //added by maimaoshi,2003-03-15
    //ÿ5����˫������������״̬
    MSG_INTRA *pMsg = new MSG_INTRA;
    pMsg->SenderPid   = GetProcessType(); 
    pMsg->SenderMid   = GetModuleType();
    pMsg->ReceiverPid = PT_CLSTR;
    pMsg->ReceiverMid = MT_MANAGER;
    pMsg->CmdCode     = CMD_ANS_CLSTR_STATUS;
    SendMsg(pMsg);
    //end
    
    EPROCMGRSTATUS nProcStatus = m_ProcMgr.GetStatus();
	
    switch (m_nStatus)
    {
        case KERNEL_INIT_PENDING:
        {
            switch (nProcStatus)
            {
                case PROCMGR_READY:
                {
                    m_nStatus = KERNEL_READY;
                    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, SZ_KNL_GET_READY);
                    SendAnsToClstrInterface(CMD_ANS_INIT, TRUE);

                    break;
                }

                case PROCMGR_FAILURE:
                {
                    m_nStatus = KERNEL_FAILURE;
                    MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, SZ_KNL_INIT_FAILURE);
                    
                    //���ڳ�ʼ��ʧ�ܵ���������м�¼
                    TRACE(THISMODULE, SZ_KNL_INIT_FAILURE);

                    SendAnsToClstrInterface(CMD_ANS_INIT, FALSE);
            
                    break;
                }

                case PROCMGR_ACTIVE:
                {
                    if (g_lDebugVersion)
                    {
                        MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, SZ_KNL_GET_READY);
                        SendAnsToClstrInterface(CMD_ANS_INIT, TRUE);
                        
                        m_nStatus = KERNEL_ACTIVE;
                        MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, 
                               SZ_KNL_IS_ACTIVE);
                        TRACE(THISMODULE, SZ_KNL_IS_ACTIVE);

                        SendAnsToClstrInterface(CMD_ANS_ACTIVE, TRUE);
                    }
                    break;
                }
                
                default:
                {
                    break;
                }
            } //end of  switch(nProcStatus)

            break;
        }

        case KERNEL_ACTIVING:
        {
            switch (nProcStatus)
            {
                case PROCMGR_ACTIVE:
                {
                    m_nStatus = KERNEL_ACTIVE;
                    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, 
                           SZ_KNL_IS_ACTIVE);
                    TRACE(THISMODULE, SZ_KNL_IS_ACTIVE);

                    SendAnsToClstrInterface(CMD_ANS_ACTIVE, TRUE);
					SendInfoToAlarm();

                    break;
                }

                case PROCMGR_FAILURE:
                {
                    m_nStatus = KERNEL_FAILURE;
                    MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, 
                           SZ_KNL_ACTIVE_FAILURE);
                    TRACE(THISMODULE, SZ_KNL_ACTIVE_FAILURE);

                    SendAnsToClstrInterface(CMD_ANS_ACTIVE, FALSE);

                    break;
                }

                default:
                {
                    break;
                }
            }  // end of switch (nProcStatus)

            break;
        }

        case KERNEL_ACTIVE:
        {
            switch (nProcStatus)
            {
                case PROCMGR_FAILURE:
                {
                    m_nStatus = KERNEL_FAILOVER;

                    break;
                }

                default:
                {
                    break;
                }
            } //end of switch (nProcStatus)

            break;
        }

        case KERNEL_FAILOVER:
        {
            MSGOUT(MTS_MONITOR, MSGOUT_LEVEL_URGENT, SZ_MSG_ASKFOR_FAILOVER);
            AskForFailOver(GetModuleType(), ERROR_PROCESS_EXCEPTION, NULL, 0);
            
            break;
        }
        
        default:
        {
            break;
        }
    }
}

/********************************************************************************************
����ԭ��	public: static int SendMsgToManager(PROCESS_TYPE nPType, UINT4 uCmdCode)
��������	��ָ�����̵Ĺ���ģ�鷢��Ϣ����̬����������ģ���������ʹ��
�������	nPType---ҵ����̱�š� uCmdCode---��Ϣ���
�������	�ޡ�
����ֵ  	���������Ϣ�ɹ����򷵻�ERR_SUCCESS �����򷵻�ERR_FAIL��
�׳��쳣	�ޡ�
*********************************************************************************************/
int CMonitor:: SendMsgToManager(PROCESS_TYPE nPType, UINT4 uCmdCode)
{
    MSG_INTRA *pMsg = new MSG_INTRA;
   
    pMsg->SenderPid   = GetProcessType(); 
    pMsg->SenderMid   = MT_UNKNOWN;
    pMsg->ReceiverPid = nPType;      //ָ���Ľ���ID
    pMsg->ReceiverMid = MT_MANAGER;

    pMsg->CmdCode = uCmdCode;

    int nRet = SendMsg(pMsg);

    return nRet;
}

/********************************************************************************************
����ԭ��	protected: int SendAnsToClstrInterface(UINT4 uCmdCode, UINT4 uResult)
��������	�򱾽��̵�˫���ӿ�ģ�鷢Ӧ����Ϣ
�������	uCmdCode---��Ϣ��� uResult---����� 
�������	�ޡ�
����ֵ  	���������Ϣ�ɹ����򷵻�ERR_SUCCESS �����򷵻�ERR_FAIL��
�׳��쳣	�ޡ�
*********************************************************************************************/
int CMonitor::SendAnsToClstrInterface(UINT4 uCmdCode, UINT4 uResult)
{
    MSG_INTRA *pMsg = new(sizeof(UINT4)) MSG_INTRA;

    pMsg->SenderPid   = PT_SELF; 
    pMsg->SenderMid   = MT_MONITOR;
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = MT_CLSTRINTERFACE;
    pMsg->CmdCode     = uCmdCode;

    *(UINT4*)(pMsg->Data) = uResult;

    int nRet = SendMsg(pMsg);
   
    return nRet;
}

/********************************************************************************************
����ԭ��	protected: void SendAnsToClstrInterface()
��������	��澯ģ�鷢�͸澯��Ϣ��������������£�
1���Ȼ����������Ա���û����Լ�����ԭ��
2�����͸澯��Ϣ
3������������Ա����Ϣ��Ϊ"_"
�������	uCmdCode---��Ϣ��� uResult---����� 
�������	�ޡ�
����ֵ  	��
�׳��쳣	�ޡ�
*********************************************************************************************/
void CMonitor::SendInfoToAlarm()
{
    
    char szResetCfgPath[MAX_PATH];
    SNPRINTF(szResetCfgPath, sizeof(szResetCfgPath),
             "%s/%s", GetAppPath(), S_RESET_INFO_FILE_NAME);
    szResetCfgPath[sizeof(szResetCfgPath) - 1] = '\0';
    
    CINIFileEx iniFile(szResetCfgPath);
    BOOL  bSucc = iniFile.Open();
	
    //��ȡ��������Ա���û���
    char szUserName[USER_NAME_LEN];    
    memset(szUserName, 0, USER_NAME_LEN);
	if(!bSucc)
	{		
		strcpy(szUserName,S_RESET_DEFAULT);
        
	}
	else
	{
		iniFile.GetString(CFG_SEC_OPERATOR,CFG_OPERATOR_KEY_NAME,S_RESET_DEFAULT,szUserName,USER_NAME_LEN);
		
	}
	
    //��ȡ����ԭ��
    BYTE yResetCausedID;
    int nRet = memcmp(szUserName,S_RESET_DEFAULT,strlen(szUserName));
    if(0 == nRet)
    {
		//TRACE(THISMODULE,S_ALARM_INI_OPEN_FAIL);
        yResetCausedID = (BYTE)0;      //ϵͳ����
    }
    else
    {
        yResetCausedID = (BYTE)1;       //�ֹ�����
		
    }
	
    //���͸澯��Ϣ   
    SInnerAlarm Alarm;
    Alarm.yAlarmType = AT_EVENT;
    Alarm.uAlarmID = ALARM_ID_RESET;
    Alarm.yAlarmPara[0] = yResetCausedID;
    memcpy(&Alarm.yAlarmPara[1], szUserName, MAX_ALARM_PARA - 1);
    CMsgProcessor::SendAlarm(&Alarm);
	
    //����������Ա����Ϣ��Ϊ"_"
    iniFile.WriteString(CFG_SEC_OPERATOR,CFG_OPERATOR_KEY_NAME,S_RESET_DEFAULT);
	
    iniFile.Close();
    
}
//add by maimaoshi,2003-03-15
/********************************************************************************************
����ԭ��	protected: SendAnsProcStatus()
��������	��OM���̵�Perf��Ӧ����Ϣ
�������	�ޡ� 
�������	�ޡ�
����ֵ  	���������Ϣ�ɹ����򷵻�ERR_SUCCESS �����򷵻�ERR_FAIL��
�׳��쳣	�ޡ�
*********************************************************************************************/
void CMonitor::SendAnsProcStatus()
{
	SProcStatus* pItems = NULL;
	CProcessChild* pProcessChild = NULL;
	int nItemCount = 0;
	nItemCount = m_ProcMgr.m_ProcChild.size();
	int nLen = sizeof(SProcStatus);

	MSG_INTRA *pMsg = new(nLen * (nItemCount + 2)) MSG_INTRA;

	//����cluster����״̬��Ϣ
	pItems = new SProcStatus;
	pItems->nPid = PT_CLSTR;
	strncpy(pItems->szProcName, PTS_CLSTR, 32);
	pItems->szProcName[31] = '\0';
	if (m_bClusterprocIsExist)
	{
		sprintf(pItems->szStatus ,"%s", "ONLINE");
	}
	else
	{
	sprintf(pItems->szStatus ,"%s", "OFFLINE");
	}

	memcpy(pMsg->Data, pItems, nLen);
	delete pItems;

	//����kernel����״̬��Ϣ
	pItems = new SProcStatus;
	pItems->nPid = PT_KERNEL;
	strncpy(pItems->szProcName, PTS_KERNEL, 32);
	pItems->szProcName[31] = '\0';
	sprintf(pItems->szStatus ,"%s", "ONLINE");
	memcpy(pMsg->Data + nLen, pItems, nLen);
	delete pItems;

	//����OM���������̵�״̬��Ϣ
	int i = 2;
	LIST<CProcessChild *>::iterator it;
	for(it = m_ProcMgr.m_ProcChild.begin(); it != m_ProcMgr.m_ProcChild.end(); it++)
	{
		pProcessChild = *it;
		
		pItems = new SProcStatus;
		pItems->nPid = pProcessChild->GetProcessType();
		strncpy(pItems->szProcName , pProcessChild->GetAppName(), 32);
		pItems->szProcName[31] = '\0';
		
		if (pProcessChild->GetStatus() == APP_ACTIVE )
		{
			sprintf(pItems->szStatus ,"%s", "ONLINE");
		}
		else
		{
			sprintf(pItems->szStatus ,"%s", "OFFLINE");
		}

		memcpy(pMsg->Data + nLen * i, pItems, nLen);
		delete pItems;

		i++;
	}

	pMsg->SenderPid = GetProcessType(); 
    pMsg->SenderMid = GetModuleType();
    pMsg->ReceiverPid = PT_OM;
    pMsg->ReceiverMid = MT_PERF;
    pMsg->CmdCode =  CMD_ANS_PROC_STATUS;
            
    SendMsg(pMsg);
}
//end
