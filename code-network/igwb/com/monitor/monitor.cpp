/***************************************************************************
 *产品名    ：iGateway Bill V200
 *模块名    ：monitor
 *文件名    ：monitor.h
 *描述信息  ：类CMonitor的定义。
 *版本说明  ：V200R001i00M00B00D1130A
 *版权信息  ：版权所有（C）2001-2002 华为技术有限公司
 *作者      ：邱意平
 *创建时间  ：2001-10-30
 *修改记录  ：
*****************************************************************************/

#include "monitor.h"
#include "proc_child.h"
#include "../utility/mml_para.h"
#include "../utility/mml_report.h"
#include "resource.h"

long g_lDebugVersion = 0;

/****************************************************************************
函数原型	public: CMonitor()
功能描述	构造函数
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
*****************************************************************************/
CMonitor:: CMonitor()
{
}

/****************************************************************************
函数原型	public: ~CMonitor()
功能描述	析构函数
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
*****************************************************************************/
CMonitor:: ~CMonitor()
{
    //
}

/****************************************************************************
函数原型	public: virtual int InitInstance()
功能描述	初始化函数
输入参数	无。
输出参数	无。
返回值  	返回初始化是否成功，ERR_SUCCESS（0）表示成功，非零值表示失败。
抛出异常	无。
*****************************************************************************/
int CMonitor::InitInstance()
{
    //调用基类的初始化函数
    int nRet = CWorkModule::InitInstance();
	
    if (ERR_SUCCESS != nRet)
    {
        m_nStatus = KERNEL_FAILURE;
        return nRet;
    }

    //从配置文件中读取是否是用于调试的信息
    CINIFile *pIniFile = new CINIFile(GetCfgFilePath());
    if (!pIniFile->Open()) 
    {
        delete pIniFile;
        m_nStatus = KERNEL_FAILURE;

        return ERR_OPEN_CFG_FILE;
    }

    //确定是否为调试版本
    g_lDebugVersion = pIniFile->GetInt(CFG_SEC_KERNEL, 
                                       CFG_KERNEL_KEY_DEBUGVERSION, 
                                       CFG_KERNEL_KEY_DEBUGVERSION_DEFAULT);
    pIniFile->Close();
    delete pIniFile;
	
    //注册定时器，初始化状态
    m_lTimerID5Sec = SetTimer(FIVE_SECOND_IN_MILISEC);
    m_nStatus = KERNEL_INIT_PENDING;

    //初始化业务进程管理者对象，在该函数中创建所有的接入点进程和操作
    //维护进程，但不激活这些子进程
    nRet = m_ProcMgr.InitInstance();
    if (ERR_SUCCESS != nRet)
    {
        m_nStatus = KERNEL_FAILURE;
        return nRet;
    }

    m_nCounter = 0;
	m_bClusterprocIsExist = FALSE;
    m_lTimerID1Sec = SetTimer(ONE_SECOND_IN_MILISEC);
    //在非调试版本，启动接入点进程和操作维护进程
    if (0 == g_lDebugVersion)
    {
       //启动子进程
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
函数原型	public: virtual void ExitInstance()
功能描述	退出时的清理工作
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
***********************************************************************************/
void CMonitor::ExitInstance()
{
    DeActive();

    //注销定时器
    KillTimer(m_lTimerID5Sec);
    
    //注销定时器
    KillTimer(m_lTimerID1Sec);

    //调用基类的ExitInstance函数
    CWorkModule::ExitInstance();
}

/*********************************************************************************
函数原型	protected: void  ParseMsg(MSG_INTRA* pMsg)
功能描述	消息分发函数
输入参数	由其它模块发送来的消息。
输出参数	无。
返回值  	无。
抛出异常	无。
**********************************************************************************/
void CMonitor::ParseMsg(MSG_INTRA* pMsg)
{
    //by ldf 2002.4.9 对应问题单 D12433
    static UINT2   LastAppType;                    //记录应用类型
    static UINT2   LastClientNo;                   //记录某种类型的MML客户端的编号
    static UINT4   LastIPAddr;                     //记录MML客户端的IP地址
    static char    LastUserName[48];               //记录MML用户名
    //修改完毕

    switch (pMsg->CmdCode)
    {
        //业务进程发来的初始化应答
        case CMD_ANS_INIT:
        {
            m_ProcMgr.ProcInitAns((PROCESS_TYPE)(pMsg->SenderPid));
		
            if (g_lDebugVersion && (KERNEL_ACTIVE != m_nStatus)) 
            {
                m_nStatus = KERNEL_ACTIVING;
            }

            break;
        }

        //双机中转发来的激活命令
        case CMD_ACTIVE:
        {
            //只能在非调试版本且当前状态为READY状态下,才能激活命令
            if ((0 == g_lDebugVersion) && (KERNEL_READY == m_nStatus)) 
            {
                 MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, SZ_MSG_BEGIN_ACTIVE);
                 TRACE(THISMODULE, SZ_MSG_BEGIN_ACTIVE);

                 Active();
            }

            break;
        }

        //业务进程发来的激活应答
        case CMD_ANS_ACTIVE:
        {
            m_ProcMgr.ProcActiveAns((PROCESS_TYPE)(pMsg->SenderPid));

            break;
        }

        //双机中转发来的去激活命令
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

        //双机中转发来的退出命令
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

        //处理MML SERVER转发的MML客户端的消息，目前只处理一个命令
        //即"辅助升级"
        case CMD_MML:
        {
            CParseResult pr(pMsg);
            pr.CreateParseResult(pMsg);

            if(MML_PRODUCE_CDR == pr.GetCmdCode())
            {
                m_ProcMgr.ProduceCDR();
                
                MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, GetStringFromID(5002));
                //重要操作，写日志
                SLog log;
                log.LogType    = SLog::LOG_OPER;
                log.uIPAddr    = pMsg->IPAddr;
                log.OperResult = SLog::OPER_SUCCESS;
                
                strcpy(log.szUserName, pMsg->UserName);
                strcpy(log.szOperCmd, pr.GetCmdName());
                strcpy(log.szCmdDesc, pr.GetCmdDspName());
                strcpy(LastUserName, pMsg->UserName);                
                WriteLog(&log); 
                
                //构造返回的报文
                CMMLReport Report(THISMODULE);
                Report.MakeReportHead(); 
                Report.MakeCmdDisplay(pr.GetCmdLine(), 0, ERR_SUCCESS, THISMODULE);
                //构造报文尾 
                Report.MakeReportTail();
                
                const char* pChar = Report.GetReport();
                int nLen = strlen(pChar); 
                //向MML客户端应答
                MSG_INTRA* pAns = new(nLen + 1)MSG_INTRA;
                strcpy((char *)(pAns->Data), (const char *)pChar);
                
                pAns->SenderPid     = GetProcessType();
                pAns->SenderMid     = GetModuleType();
                pAns->ReceiverPid   = PT_OM; 
                pAns->ReceiverMid   = MT_MMLDEBUG;
                pAns->AppType       = pMsg->AppType;
                pAns->ClientNo      = pMsg->ClientNo;
                pAns->IPAddr        = pMsg->IPAddr; 
                
                LastAppType         = pAns->AppType;                    //记录应用类型
                LastClientNo        = pAns->ClientNo;                   //记录某种类型的MML客户端的编号
                LastIPAddr          = pAns->IPAddr;                     //记录MML客户端的IP地址              
                
                SendMsg(pAns);    //发送消息
            }            

            if (MML_AUP == pr.GetCmdCode())  
            {
                // 张玉新 2002-03-13 add 问题单D11733
                if(m_ProcMgr.GetAuxUpgStatus() == AUXUPG_START)
                {
                    WriteLog(MTS_MONITOR,S_MONITOR_REVMOREAUXUPG,SLog::OPER_SUCCESS);
                    break;
                }
                //增加结束

                //张玉新 2002-03-13 add 问题单D11733
                CMMLReport Report(THISMODULE);
                Report.MakeReportHead();            //构造报文头
                //构造命令回显
                Report.MakeCmdDisplay(pr.GetCmdLine(), 0, ERR_SUCCESS, THISMODULE);
                if (KERNEL_ACTIVE == m_nStatus && m_ProcMgr.GetAuxUpgStatus() == AUXUPG_STOPPED)
                {
                    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, SZ_AUP_IS_GOINGON);
                    m_ProcMgr.ProcAuxUpgradeCmd();
                    
                    //构造返回报文正文
                    Report.AddString(SZ_AUP_IS_GOINGON);
                   
                    //重要操作，写日志
                    SLog log;
                    log.LogType    = SLog::LOG_OPER;
                    log.uIPAddr    = pMsg->IPAddr;
                    log.OperResult = SLog::OPER_SUCCESS;

                    strcpy(log.szUserName, pMsg->UserName);
                    strcpy(log.szOperCmd, SZ_MML_CMD_AUP);
                    strcpy(log.szCmdDesc, SZ_MML_CMDDESC_AUP);

                    //by ldf 2002.4.9 对应问题单D12433
                    strcpy(LastUserName, pMsg->UserName);
                    //修改完毕

                    WriteLog(&log); 
                }
                else if(m_ProcMgr.GetAuxUpgStatus() == AUXUPG_COMPLETED)
                {
                    //构造返回报文正文
                    Report.AddString(SZ_AUP_IS_COMPLETE);
                }
                //增加新行
                Report.AddString(STR_NEWLINE);
                //构造报文尾 
                Report.MakeReportTail();
                
                const char* pChar = Report.GetReport();
                int nLen = strlen(pChar); 
                
                //向MML客户端应答辅助升级命令
                MSG_INTRA* pAns = new(nLen + 1)MSG_INTRA;
                strcpy((char *)(pAns->Data), (const char *)pChar);
                
                pAns->SenderPid     = GetProcessType();
                pAns->SenderMid     = GetModuleType();
                pAns->ReceiverPid   = PT_OM; 
                pAns->ReceiverMid   = MT_MMLDEBUG;
                pAns->AppType       = pMsg->AppType;
                pAns->ClientNo      = pMsg->ClientNo;
                pAns->IPAddr        = pMsg->IPAddr; 

                //by ldf 2002.4.9 对应问题单 D12433
                LastAppType         = pAns->AppType;                    //记录应用类型
                LastClientNo        = pAns->ClientNo;                   //记录某种类型的MML客户端的编号
                LastIPAddr          = pAns->IPAddr;                     //记录MML客户端的IP地址
                //修改完毕
 
                SendMsg(pAns);    //发送消息
            } //修改结束

            break;
        }

        //by ldf 2002.4.8 对应问题单 D12433
       case CMD_AUXUPGRADE_FAIL:
        {
            if(m_ProcMgr.GetAuxUpgStatus() == AUXUPG_START)
            {

            MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, SZ_AUP_IS_FAILED);
	
            CMMLReport Report(THISMODULE);
			
            Report.MakeReportHead();            //构造报文头
			
            //构造命令回显
            Report.MakeCmdDisplay(SZ_MML_CMD_AUP_REDISPLAY, 0, ERR_FAIL, THISMODULE);
            //构造返回报文正文
            Report.AddString(SZ_AUP_IS_FAILED);
            //增加新行
            Report.AddString(STR_NEWLINE);
            //构造报文尾 
            Report.MakeReportTail();
			
            const char* pChar = Report.GetReport();
            int nLen = strlen(pChar); 
			
            //向MML客户端应答辅助升级命令
            MSG_INTRA* pAns = new(nLen + 1)MSG_INTRA;
            strcpy((char *)(pAns->Data), (const char *)pChar);

            pAns->SenderPid     = GetProcessType();
            pAns->SenderPid     = GetModuleType();
            pAns->ReceiverPid   = PT_OM; 
            pAns->ReceiverMid   = MT_MMLDEBUG;
            pAns->AppType       = LastAppType;
            pAns->ClientNo      = LastClientNo;
            pAns->IPAddr        = LastIPAddr; 

            SendMsg(pAns);    //发送消息

            //重要操作，写日志
            SLog log;
            log.LogType    = SLog::LOG_OPER;
            log.uIPAddr    = LastIPAddr;
            log.OperResult = SLog::OPER_FAIL;

            strcpy(log.szUserName, LastUserName);
            strcpy(log.szOperCmd, SZ_MML_CMD_AUP);
            strcpy(log.szCmdDesc, SZ_AUP_IS_FAILED);
			
            WriteLog(&log); 

            //把辅助升级的状态 由AUXUPG_START 置回 AUXUPG_STOPPED
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
函数原型	protected: virtual MODULE_TYPE GetModuleType();
功能描述	返回模块类型
输入参数	无。
输出参数	无。
返回值  	模块类型。
抛出异常	无。
***************************************************************************************/
MODULE_TYPE CMonitor::GetModuleType()
{
    return MT_MONITOR;
}

/**************************************************************************************
函数原型	protected: int Active()
功能描述	发起激活操作
输入参数	无。
输出参数	无。
返回值  	如果发起激活操作成功，则返回ERR_SUCCESS ，否则返回非零值。
抛出异常	无。
***************************************************************************************/
int CMonitor::Active()
{
    switch (m_nStatus)
    {
        //如果正在激活或者已经激活，则直接返回
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
函数原型	protected: void  DeActive()
功能描述	执行去激活操作，本操作将肯定成功
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
**********************************************************************************************/
void CMonitor:: DeActive()
{
    if(KERNEL_DEACTIVE == m_nStatus)
    {
        return;
    }

    m_nStatus = KERNEL_DEACTIVING;

    m_ProcMgr.DeActive();     //通过进程管理器去激活各个子进程

    m_nStatus = KERNEL_DEACTIVE;
}

/*********************************************************************************************
函数原型	protected: void  OnTimer(const long nTimerID);
功能描述	定时器处理
输入参数	时钟ID ，只读。
输出参数	无。
返回值  	无。
抛出异常	无。
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
函数原型	protected: void  Timer5Sec();
功能描述	5秒定时器处理
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
*********************************************************************************************/
void CMonitor::Timer5Sec()
{
    if (0 == g_lDebugVersion)
    //非调试版本时用
    {
        m_ProcMgr.Timer5Sec();
    }

    //added by maimaoshi,2003-03-15
    //每5秒向双机进程请求其状态
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
                    
                    //对于初始化失败的情况，进行记录
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
函数原型	public: static int SendMsgToManager(PROCESS_TYPE nPType, UINT4 uCmdCode)
功能描述	向指定进程的管理模块发消息，静态函数，供本模块的其它类使用
输入参数	nPType---业务进程编号。 uCmdCode---消息命令。
输出参数	无。
返回值  	如果发送消息成功，则返回ERR_SUCCESS ，否则返回ERR_FAIL。
抛出异常	无。
*********************************************************************************************/
int CMonitor:: SendMsgToManager(PROCESS_TYPE nPType, UINT4 uCmdCode)
{
    MSG_INTRA *pMsg = new MSG_INTRA;
   
    pMsg->SenderPid   = GetProcessType(); 
    pMsg->SenderMid   = MT_UNKNOWN;
    pMsg->ReceiverPid = nPType;      //指定的进程ID
    pMsg->ReceiverMid = MT_MANAGER;

    pMsg->CmdCode = uCmdCode;

    int nRet = SendMsg(pMsg);

    return nRet;
}

/********************************************************************************************
函数原型	protected: int SendAnsToClstrInterface(UINT4 uCmdCode, UINT4 uResult)
功能描述	向本进程的双机接口模块发应答消息
输入参数	uCmdCode---消息命令。 uResult---结果。 
输出参数	无。
返回值  	如果发送消息成功，则返回ERR_SUCCESS ，否则返回ERR_FAIL。
抛出异常	无。
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
函数原型	protected: void SendAnsToClstrInterface()
功能描述	向告警模块发送告警信息，其操作步骤如下：
1、先获得重启操作员的用户名以及重启原因
2、发送告警信息
3、将重启操作员的信息置为"_"
输入参数	uCmdCode---消息命令。 uResult---结果。 
输出参数	无。
返回值  	无
抛出异常	无。
*********************************************************************************************/
void CMonitor::SendInfoToAlarm()
{
    
    char szResetCfgPath[MAX_PATH];
    SNPRINTF(szResetCfgPath, sizeof(szResetCfgPath),
             "%s/%s", GetAppPath(), S_RESET_INFO_FILE_NAME);
    szResetCfgPath[sizeof(szResetCfgPath) - 1] = '\0';
    
    CINIFileEx iniFile(szResetCfgPath);
    BOOL  bSucc = iniFile.Open();
	
    //读取重启操作员的用户名
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
	
    //获取重启原因
    BYTE yResetCausedID;
    int nRet = memcmp(szUserName,S_RESET_DEFAULT,strlen(szUserName));
    if(0 == nRet)
    {
		//TRACE(THISMODULE,S_ALARM_INI_OPEN_FAIL);
        yResetCausedID = (BYTE)0;      //系统重启
    }
    else
    {
        yResetCausedID = (BYTE)1;       //手工重启
		
    }
	
    //发送告警信息   
    SInnerAlarm Alarm;
    Alarm.yAlarmType = AT_EVENT;
    Alarm.uAlarmID = ALARM_ID_RESET;
    Alarm.yAlarmPara[0] = yResetCausedID;
    memcpy(&Alarm.yAlarmPara[1], szUserName, MAX_ALARM_PARA - 1);
    CMsgProcessor::SendAlarm(&Alarm);
	
    //将重启操作员的信息置为"_"
    iniFile.WriteString(CFG_SEC_OPERATOR,CFG_OPERATOR_KEY_NAME,S_RESET_DEFAULT);
	
    iniFile.Close();
    
}
//add by maimaoshi,2003-03-15
/********************************************************************************************
函数原型	protected: SendAnsProcStatus()
功能描述	向OM进程的Perf发应答消息
输入参数	无。 
输出参数	无。
返回值  	如果发送消息成功，则返回ERR_SUCCESS ，否则返回ERR_FAIL。
抛出异常	无。
*********************************************************************************************/
void CMonitor::SendAnsProcStatus()
{
	SProcStatus* pItems = NULL;
	CProcessChild* pProcessChild = NULL;
	int nItemCount = 0;
	nItemCount = m_ProcMgr.m_ProcChild.size();
	int nLen = sizeof(SProcStatus);

	MSG_INTRA *pMsg = new(nLen * (nItemCount + 2)) MSG_INTRA;

	//加入cluster进程状态信息
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

	//加入kernel进程状态信息
	pItems = new SProcStatus;
	pItems->nPid = PT_KERNEL;
	strncpy(pItems->szProcName, PTS_KERNEL, 32);
	pItems->szProcName[31] = '\0';
	sprintf(pItems->szStatus ,"%s", "ONLINE");
	memcpy(pMsg->Data + nLen, pItems, nLen);
	delete pItems;

	//加入OM、接入点进程的状态信息
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
