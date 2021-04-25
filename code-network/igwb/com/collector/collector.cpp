/**************************************************************************
 *产品名    : iGateway Bill V200
 *模块名    : 采集模块(collector)
 *文件名    : collector.cpp
 *描述信息  : 本文件是类CCollectorModule的实现
 *版本说明  : V200R001i00M00B00D1130A
 *版权信息  : 版权所有（C）2001-2002 华为技术有限公司
 *作者      : 王锋
 *创建时间  : 2002-10-26
 *修改记录  :
 **************************************************************************/
#include "collector.h"
#include "../utility/qb_report.h"
#include "../utility/mml_para.h"
#include "../include/toolbox.h"
#include "collect_task/global_fun.h"

//added by chenliangwei
CCollectorFilePool* CCollectorModule::g_pFilePool = NULL;
char CCollectorModule::g_szColAppName[MAX_PATH] = "";

CCollectorFilePool* GetFilePool()
{
    if (CCollectorModule::g_pFilePool != NULL)
    {
        return CCollectorModule::g_pFilePool;
    }

    //获得状态文件的路径
    char szPathName[MAX_PATH];
#ifdef _PLATFORM_WIN32
    SNPRINTF(szPathName,
             sizeof(szPathName),
             "d:/other/collect/%s",
             CCollectorModule::g_szColAppName);
#else
    SNPRINTF(szPathName,
             sizeof(szPathName),
             "/var/other/collect/%s",
             CCollectorModule::g_szColAppName);
#endif
    CreateDeepDir(szPathName);

    SNPRINTF(szPathName,
             sizeof(szPathName),
             "%s/col_filepool_status.dat",
             szPathName);

    CCollectorModule::g_pFilePool =
        new CCollectorFilePool(szPathName);

    if (ERR_SUCCESS != CCollectorModule::g_pFilePool->Init())
    {
        delete CCollectorModule::g_pFilePool;
        CCollectorModule::g_pFilePool = NULL;
    }

    return CCollectorModule::g_pFilePool;
}
//added end

/**************************************************************************
函数原型    public: CCollectorModule (void);
功能描述    构造函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
CCollectorModule::CCollectorModule(void)
{
    m_lTimerID5Sec = 0;
    m_nStatus      = CM_DEACTIVE;
    m_bDiskFull    = FALSE;

    //added by chenliangwei
    m_nConvertStatus = CO_CVT_STATUS_INIT;
    m_nFileNum = 0;
    m_bFileIsSended = FALSE;

    m_bFileNeedReSend = FALSE;
    m_nFileReSendTime = 0;
    m_nFileReSendInterval = 30;
    //added end

    m_szStatusFile[0] = '\0';
    m_pStatusFile = NULL;
}

/**************************************************************************
函数原型    public: ~CCollectorModule ();
功能描述    构造函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
CCollectorModule::~CCollectorModule()
{
    //
}

/**************************************************************************
函数原型    protected: void ExitInstance (void);
功能描述    退出清理
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CCollectorModule::ExitInstance (void)
{
    if(NULL != m_pTaskList)
    {
        CCollectTaskObj* pTask = NULL;
        while(m_pTaskList->size() > 0)
        {
            pTask = m_pTaskList->front();
            pTask->WaitForExit();
            delete pTask;
            m_pTaskList->erase(m_pTaskList->begin());
        }

        delete m_pTaskList;
        m_pTaskList = NULL;
    }

    KillTimer(m_lTimerID5Sec);

    //added by chenliangwei
    KillTimer(m_lTimerID1Sec);
    //added end

    CWorkModule::ExitInstance();
}
/**************************************************************************
函数原型    protected: int InitInstance (void);
功能描述    初始化
输入参数    无
输出参数    无
返回值      初始化成功时返回ERR_SUCCESS
抛出异常    无
***************************************************************************/
int CCollectorModule::InitInstance (void)
{
    int nRet = ERR_FAIL;
    nRet = CWorkModule::InitInstance();
    if( nRet != ERR_SUCCESS )
    {
        return nRet;
    }

    // 打开配置文件
    CINIFile IniFile(GetCfgFilePath());
    nRet = IniFile.Open();

    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP, GetProcessType() - PT_CO_BASE + 1);
    int nCollectType = IniFile.GetInt(strSec,
                                      CFG_AP_KEY_COLTYPE,
                                      CFG_AP_KEY_COLTYPE_DEFAULT);

    if(nCollectType == 0)
    {
        TRACE(THISMODULE, S_AP_COTYPE_DISMATCHED, GetProcessType() - PT_CO_BASE + 1);
        return ERR_FAIL;
    }

    IniFile.GetString(strSec, CFG_AP_NAME, strSec, 
                      g_szColAppName, sizeof(g_szColAppName));

    char szCfgPath[MAX_PATH];
    IniFile.GetString(strSec, CFG_COT_CFG_PATH, "", szCfgPath, MAX_PATH);
    szCfgPath[MAX_PATH - 1] = '\0';

    // 关闭配置文件
    IniFile.Close();

    //获得状态文件的路径
    char szPathName[MAX_PATH];
#ifdef _PLATFORM_WIN32
    SNPRINTF(szPathName,
             sizeof(szPathName),
             "d:/other/collect/%s",
             CCollectorModule::g_szColAppName);
#else
    SNPRINTF(szPathName,
             sizeof(szPathName),
             "/var/other/collect/%s",
             CCollectorModule::g_szColAppName);
#endif
    CreateDeepDir(szPathName);

    SNPRINTF(m_szStatusFile,
             sizeof(m_szStatusFile),
             "%s/col_module_status.dat",
             szPathName);

    //added by chenliangwei
    CCollectorFilePool* pFilePool = GetFilePool();
    if (pFilePool == NULL)
    {
        return ERR_FAIL;
    }
    //added end

    m_pTaskList = CCollectTaskObjFactory::CreateCollectTaskObj(szCfgPath);

    if(NULL == m_pTaskList)
    {
        TRACE(THISMODULE, S_FAILED_TO_CREATE_CO_TASK_OBJ);
        return ERR_FAIL;
    }

    m_lTimerID5Sec = SetTimer(FIVE_SECOND_IN_MILISEC);

    //added by chenliangwei
    m_lTimerID1Sec = SetTimer(ONE_SECOND_IN_MILISEC);
    //added

    return ERR_SUCCESS;
}

/**************************************************************************
函数原型    protected: MODULE_TYPE GetModuleType ( void );
功能描述    获得本模块类型
输入参数    无
输出参数    无
返回值      返回本模块类型
抛出异常    无
***************************************************************************/
MODULE_TYPE CCollectorModule::GetModuleType ( void )
{
    return MT_COLLECTOR;
}


/**************************************************************************
函数原型    protected: void ParseMsg ( MSG_INTRA* pMsgParsed )
功能描述    命令解析与分发
输入参数    消息指针
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CCollectorModule::ParseMsg ( MSG_INTRA* pMsgParsed )
{
    VECTOR<CCollectTaskObj*>::iterator iter;
    MSG_INTRA* pMsg = NULL;
    switch(pMsgParsed->CmdCode)
    {
    case CMD_MODULE_ACTIVE:
        //收到管理模块发送的激活本模块的命令
        iter = m_pTaskList->begin();
        for (; iter != m_pTaskList->end(); iter++)
        {
            CCollectTaskObj* pTask = *iter;
            if(pTask->activate() != ERR_SUCCESS)
            {
                TRACE(THISMODULE, S_FAIL_TO_OPEN_COTASK, g_szColAppName, pTask->GetTaskID());
                SendQuitToMe();
                return;
            }
        }
        m_nStatus = CM_ACTIVE;

        pMsg   = new(strlen(CMsgProcessor::g_szPtDes) + 1) MSG_INTRA;
        pMsg->SenderPid   = GetProcessType();
        pMsg->SenderMid   = GetModuleType();
        pMsg->ReceiverPid = PT_OM;
        pMsg->ReceiverMid = MT_PERF;
        pMsg->CmdCode     = CMD_AP_NAME;

        memcpy(pMsg->Data,
               CMsgProcessor::g_szPtDes,
               strlen(CMsgProcessor::g_szPtDes) + 1);

        SendMsg(pMsg);

        //added by chenliangwei
        pMsg = new MSG_INTRA;
        pMsg->SenderPid     = PT_SELF;
        pMsg->SenderMid     = MT_COLLECTOR;
        pMsg->ReceiverPid   = PT_SELF;
        pMsg->ReceiverMid   = MT_COLCONVERTOR;
        pMsg->CmdCode       = CMD_RESUME;
        SendMsg(pMsg);

        m_nConvertStatus = CO_CVT_STATUS_RESUME;
        //added

        break;

    //added by chenliangwei
    case CMD_RESUME_OVER:
        {
            if (m_nConvertStatus == CO_CVT_STATUS_RESUME)
            {
                OnResumeOver((SCollectConvertResult *)pMsgParsed->Data);
            }
        }
        break;

    case CMD_ANS_CONVERT_BILLFILE:
        {
            if (m_nConvertStatus == CO_CVT_STATUS_CONVERT)
            {
                SCollectConvertResult* pResult =
                               (SCollectConvertResult*)pMsgParsed->Data;

                OnResponse(pResult->nConvertResult);
            }
        }
        break;
    //added end

    case CMD_STOP_NETWORK:
        if(m_nStatus == CM_AUXUPGRADE)
        {
            break;
        }
        //若为“磁盘空间已满”，则去激活网络应用层对象
        iter = m_pTaskList->begin();
        for (; iter != m_pTaskList->end(); iter++)
        {
            CCollectTaskObj* pTask = *iter;
            pTask->DeActive();
        }
        m_nStatus   = CM_DEACTIVE;
        m_bDiskFull = TRUE;
        break;

    case CMD_RESUME_NETWORK:
        if(m_nStatus == CM_AUXUPGRADE)
        {
            break;
        }
        //若为“磁盘空间恢复”，则激活网络应用层对象
        iter = m_pTaskList->begin();
        for (; iter != m_pTaskList->end(); iter++)
        {
            CCollectTaskObj* pTask = *iter;
            if(!pTask->GetManualStop())
            {
                pTask->Active();
            }
        }
        m_nStatus   = CM_ACTIVE;
        m_bDiskFull = FALSE;
        break;

    case CMD_AUXUPGRADE:
        iter = m_pTaskList->begin();
        for (; iter != m_pTaskList->end(); iter++)
        {
            CCollectTaskObj* pTask = *iter;
            pTask->SetFlagForExit();
        }
        m_nStatus = CM_AUXUPGRADE;
        break;

    case CMD_MML:
        {
            CParseResult ParseResult(pMsgParsed);
            ParseResult.CreateParseResult(pMsgParsed);

            switch (ParseResult.GetCmdCode())
            {
            case MML_LST_COL_INFO:
                ProcListColInfo(pMsgParsed);
                break;

            case MML_SET_COL_STATUS:
                if(m_nStatus == CM_AUXUPGRADE)
                {
                    break;
                }
                ProcSetColStatus(pMsgParsed);
                break;

            case MML_SET_ALARM_LIMIT:
                if(m_nStatus == CM_AUXUPGRADE)
                {
                    break;
                }
                ProcSetALarmLimit(pMsgParsed);
                break;

            case MML_LST_MODULE_INFO:
                ProcListModuleInfo(pMsgParsed);
                break;

            case MML_GET_FILE:
                if(m_nStatus == CM_AUXUPGRADE)
                {
                    break;
                }
                ProcGetFile(pMsgParsed);
                break;

            //查询采集计划
            case MML_LST_COL_SCHDL:
                ProcListColSch(pMsgParsed);
                break;

            //设置采集计划
            case MML_SET_COL_SCHDL:
                if(m_nStatus == CM_AUXUPGRADE)
                {
                    break;
                }
                ProcSetColSch(pMsgParsed);
                break;

            //查询单次采集计划
            case MML_LST_A_COL_SCHDL:
                ProcListOnceColSch(pMsgParsed);
                break;

            //设置单次采集计划
            case MML_SET_A_COL_SCHDL:
                if(m_nStatus == CM_AUXUPGRADE)
                {
                    break;
                }
                ProcSetOnceColSch(pMsgParsed);
                break;

            default:
                break;
            }
        }
        break;

    default:
        break;
    }
    
    CWorkModule::ParseMsg(pMsgParsed);
}

/*********************************************************************************************
函数原型        protected: void  OnTimer(const long nTimerID);
功能描述        定时器处理
输入参数        时钟ID ，只读。
输出参数        无。
返回值          无。
抛出异常        无。
**********************************************************************************************/
void CCollectorModule::OnTimer(const long nTimerID)
{
    if (nTimerID == m_lTimerID5Sec)
    {
        Timer5Sec();
    }

    //added by chenliangwei
    if (nTimerID == m_lTimerID1Sec)
    {
        Timer1Sec();
    }
    //added end

    CWorkModule::OnTimer(nTimerID);
}

/********************************************************************************************
函数原型        protected: void  Timer5Sec();
功能描述        5秒定时器处理
输入参数        无。
输出参数        无。
返回值          无。
抛出异常        无。
*********************************************************************************************/
void CCollectorModule::Timer5Sec()
{

/*  commented by chenliangwei, 2003-05-19
    if(m_nStatus == CM_DEACTIVE)
    {
        return;
    }
*/

    MSG_INTRA* pMsg = NULL;

    pMsg   = new(strlen(CMsgProcessor::g_szPtDes) + 1) MSG_INTRA;
    pMsg->SenderPid   = GetProcessType();
    pMsg->SenderMid   = GetModuleType();
    pMsg->ReceiverPid = PT_OM;
    pMsg->ReceiverMid = MT_PERF;
    pMsg->CmdCode     = CMD_AP_NAME;

    memcpy(pMsg->Data,
           CMsgProcessor::g_szPtDes,
           strlen(CMsgProcessor::g_szPtDes) + 1);

    SendMsg(pMsg);


    CCollectTaskObj* pTask = NULL;
    VECTOR<CCollectTaskObj*>::iterator i = m_pTaskList->begin();

    int j = 0;
    while(i != m_pTaskList->end())
    {
        pTask = *i;

        if(j++ < m_nLicenseCount)
        {
            pTask->SetLicense(TRUE);
        }
        else
        {
            pTask->SetLicense(FALSE);
        }

        //modifyed by chenliangwei, 2003-05-19
        if (m_nStatus == CM_ACTIVE)
        {
            if(!pTask->GetActiveValue())
            {
                if(!pTask->GetFailureValue())
                {
                    if(pTask->CanRestart())
                    {
                        TRACE(THISMODULE, S_REOPEN_COTASK, pTask->GetTaskID());
                        if(pTask->open() != ERR_SUCCESS)
                        {
                            TRACE(THISMODULE, S_FAIL_TO_REOPEN_COTASK, pTask->GetTaskID());
                        }
                    }
                    else
                    {
                        TRACE(THISMODULE, S_CO_TASK_FAILED, pTask->GetTaskID());
                    }
                }
            }
        }
        //modifyed end

        int nActive = pTask->GetActiveValue();
        if(!pTask->GetLicense())
        {
            nActive = CS_LIC_EXPIRED; //未授权
        }
        const char* szColLabel = pTask->GetColLabel();
        int nStrBufLen = strlen(szColLabel) + 1;
        pMsg   = new(sizeof(nActive) + nStrBufLen) MSG_INTRA;
        pMsg->SenderPid   = GetProcessType();
        pMsg->SenderMid   = GetModuleType();
        pMsg->ReceiverPid = PT_OM;
        pMsg->ReceiverMid = MT_COLAGENT;
        pMsg->CmdCode     = CMD_REPORT_COLLECTOR_STATUS;

        memcpy(pMsg->Data, &nActive, sizeof(nActive));
        memcpy(pMsg->Data + sizeof(nActive),
               szColLabel,
               nStrBufLen);

        SendMsg(pMsg);

        //发链路状态信息
        LIST<SLinkStat> link_list;
        pTask->GetLinkStatus(link_list);
        LIST<SLinkStat>::iterator iter = link_list.begin();
        for(; iter != link_list.end(); iter++)
        {
            pMsg   = new(sizeof(SLinkStat)) MSG_INTRA;
            pMsg->SenderPid   = CMsgProcessor::GetProcessType();
            pMsg->SenderMid   = MT_COLLECTOR;
            pMsg->ReceiverPid = PT_OM;
            pMsg->ReceiverMid = MT_PERF;
            pMsg->CmdCode     = CMD_GET_APLINK_STATE;

            memcpy(pMsg->Data, &iter->szLocalAddr, sizeof(SLinkStat));

            SendMsg(pMsg);
        }

        i++;
    }
}

//added by chenliangwei
//秒定时器
void CCollectorModule::Timer1Sec()
{
    if (m_nConvertStatus == CO_CVT_STATUS_WAIT)
    {
        if (!m_bFileNeedReSend)
        {
            //不是文件重发，则照常发送
            SendFileToConvert();
        }
        else   
        {   
            //是文件重发，则一段时间后发送
            if (m_nFileReSendTime++ >= m_nFileReSendInterval)
            {
                SendFileToConvert();
                m_nFileReSendTime = 0;
            }
        }


        CCollectorFilePool* pFilePool = GetFilePool();
        if ((m_nStatus == CM_AUXUPGRADE) && (pFilePool->GetSize() == 0))
        {
            BOOL bExit = TRUE;
            VECTOR<CCollectTaskObj*>::iterator iter;
            iter = m_pTaskList->begin();
            for (; iter != m_pTaskList->end(); iter++)
            {
                CCollectTaskObj* pTask = *iter;
                if(pTask->GetActiveValue())
                {
                    bExit = FALSE;
                }
            }
            if(bExit)
            {
                MSG_INTRA *pMsg     = new MSG_INTRA;
                pMsg->SenderPid     = PT_SELF; 
                pMsg->SenderMid     = MT_COLLECTOR;
                pMsg->ReceiverPid   = PT_SELF;
                pMsg->ReceiverMid   = MT_MANAGER;
                pMsg->CmdCode       = CMD_ANS_AUXUPGRADE;
                SendMsg(pMsg);
                m_nStatus = CM_DEACTIVE;
            }
        }
    }
}

void CCollectorModule::OnResumeOver(SCollectConvertResult* pResult)
{
    //如果文件号相等，处理转换应答
    if ((pResult->nFileNum == m_nFileNum)
        && (m_bFileIsSended))
    {
        OnResponse(pResult->nConvertResult);
        return;
    }

    //如果文件号不相等，更新状态，发送文件，
    if (m_bFileIsSended)
    {
        m_bFileIsSended = FALSE;
        SaveStatus();
    }

    SendFileToConvert();

}

void CCollectorModule::OnResponse(int nConvertResult)
{
    CCollectorFilePool* pFilePool = GetFilePool();

    if (pFilePool->GetSize() == 0)
    {
        m_bFileIsSended = FALSE;
        m_nConvertStatus = CO_CVT_STATUS_WAIT;
        SaveStatus();
        return;
    }

    if (!m_bFileIsSended)
    {
        return;
    }

    //如果是写最终文件失败，过一段时间后重发此文件
    if(nConvertResult == COL_WRITE_BILL_FAIL)
    {
        m_bFileIsSended = FALSE;
        m_nConvertStatus = CO_CVT_STATUS_WAIT;
        m_bFileNeedReSend = TRUE;
        SaveStatus();

        return;
    }


    //如果要求重采
    if(nConvertResult == COL_RECOL_BILL)
    {
        SSwitchFileInfo& stSwitch = pFilePool->Front();

        if(strcmp(stSwitch.szLocalFile + strlen(stSwitch.szLocalFile) - 6, ".recol") != 0)
        {
            int nRet = ERR_FAIL;
            CCollectTaskObj* pTask = NULL;
            VECTOR<CCollectTaskObj*>::iterator iter = m_pTaskList->begin();
            for (; iter != m_pTaskList->end(); iter++)
            {
                pTask = *iter;
                if(pTask->GetTaskID() == stSwitch.nSwitchIndex)
                {
                    strncat(stSwitch.szLocalFile, ".recol", sizeof(stSwitch.szLocalFile));
                    nRet = pTask->GetSpecFile(stSwitch.szModuleLabel,
                                              stSwitch.szRemoteFile,
                                              stSwitch.szLocalFile);
                    break;
                }
            }
            if(nRet == ERR_SUCCESS)
            {
                MSGOUT(THISMODULE,
                       MSGOUT_LEVEL_IMPORTANT,
                       S_COL_AUTO_RECOL_FILE,
                       stSwitch.szRemoteFile,
                       stSwitch.szLocalFile);
            }
            else
            {
                MSGOUT(THISMODULE,
                       MSGOUT_LEVEL_URGENT,
                       S_COL_AUTO_RECOL_FILE_FAILED,
                       stSwitch.szRemoteFile,
                       stSwitch.szLocalFile);
                TRACE(S_COL_AUTO_RECOL_FILE_FAILED,
                      stSwitch.szRemoteFile,
                      stSwitch.szLocalFile);
            }
        }
        else
        {
            MSGOUT(THISMODULE,
                   MSGOUT_LEVEL_URGENT,
                   S_COL_FILE_CANNOT_BE_RECOL,
                   stSwitch.szRemoteFile);

            char szMsg[1024];
            SNPRINTF(szMsg, 1024,
                     S_COL_FILE_CANNOT_BE_RECOL,
                     stSwitch.szRemoteFile);
            szMsg[1024 - 1] = '\0';

            CMsgProcessor::WriteLog(MTS_COLLECTOR, szMsg, SLog::OPER_FAIL);

        }
    }

    pFilePool->PopFront();
    m_bFileIsSended = FALSE;
    m_bFileNeedReSend = FALSE;
    SaveStatus();

    SendFileToConvert();
}


void CCollectorModule::SendFileToConvert()
{
    //已经发送过，就不能发送
    if (m_bFileIsSended)
    {
        return;
    }

    CCollectorFilePool* pFilePool = GetFilePool();

    if (pFilePool->GetSize() == 0)
    {
        m_nConvertStatus = CO_CVT_STATUS_WAIT;
    }
    else
    {
        m_nFileNum++;
        SSwitchFileInfo& stSwitch = pFilePool->Front();

        SCollectConvertFileInfo stFileInfo;
        stFileInfo.nFileNum = m_nFileNum;
        memcpy(&stFileInfo.info, &stSwitch, sizeof(SSwitchFileInfo));

        MSG_INTRA *pMsg = new(sizeof(SCollectConvertFileInfo)) MSG_INTRA;
        pMsg->SenderPid = PT_SELF;
        pMsg->SenderMid = MT_COLLECTOR;
        pMsg->ReceiverPid = PT_SELF;
        pMsg->ReceiverMid = MT_COLCONVERTOR;
        pMsg->CmdCode = CMD_CONVERT_BILLFILE;
        memcpy(pMsg->Data, &stFileInfo, sizeof(SCollectConvertFileInfo));

        SendMsg(pMsg);

        //更新状态
        m_bFileIsSended = TRUE;
        m_nConvertStatus = CO_CVT_STATUS_CONVERT;

        SaveStatus();
    }

}
//added end

//查询指定采集任务的信息
void CCollectorModule::ProcListColInfo(MSG_INTRA* pMsgParsed)
{
    CParseResult ParseResult(pMsgParsed);
    ParseResult.CreateParseResult(pMsgParsed);

    int nRet = -1;
    LIST<STRING> label;
    LIST<STRING> content;

    CCollectTaskObj* pTask = NULL;
    VECTOR<CCollectTaskObj*>::iterator iter = m_pTaskList->begin();
    for (; iter != m_pTaskList->end(); iter++)
    {
        pTask = *iter;
        if(StringiCmp(pTask->GetColLabel(), ParseResult.GetStringVal(1)))
        {
            nRet = pTask->GetSwitchInfo(label, content);
            break;
        }
    }

    //返回报告
    CMMLReport Report("COLLECTOR");
    Report.MakeReportHead();
    if(nRet == ERR_SUCCESS)
    {
        Report.MakeCmdDisplay(ParseResult.GetCmdLine(), 0, 0, "ListTaskInfo");
        Report.AddHoriTable(label, content);
    }
    else
    {
        Report.MakeCmdDisplay(ParseResult.GetCmdLine(), 0, 1, "ListTaskInfo");
        Report.AddString(S_COLLECTOR_NOT_BE_FOUND);
    }
    Report.AddString(STR_NEWLINE);
    Report.MakeReportTail();

    const char* pMMLReport = Report.GetReport();
    SendMMLReport(pMMLReport, pMsgParsed->AppType, pMsgParsed->ClientNo);
}

//设置采集任务的状态
void CCollectorModule::ProcSetColStatus(MSG_INTRA* pMsgParsed)
{
    CParseResult ParseResult(pMsgParsed);
    ParseResult.CreateParseResult(pMsgParsed);

    int nRet = -1;

    STRING strErrInfo = S_COLLECTOR_NOT_BE_FOUND;
    CCollectTaskObj* pTask = NULL;
    VECTOR<CCollectTaskObj*>::iterator iter = m_pTaskList->begin();
    for (; iter != m_pTaskList->end(); iter++)
    {
        pTask = *iter;
        if(StringiCmp(pTask->GetColLabel(), ParseResult.GetStringVal(1)))
        {
            break;
        }
        pTask = NULL;
    }

    if(pTask != NULL)
    {
        BOOL bManualStop = pTask->GetManualStop();
        int  nActive = 0;
        ParseResult.GetDigitVal(&nActive, 2);
        if(nActive == 1)
        {
            pTask->SetManualStop(FALSE);
            if(!m_bDiskFull)
            {
                pTask->Active();
                nRet = 0;
                strErrInfo = S_COL_TO_BE_RUNNING_AND_ACT;
            }
            else
            {
                strErrInfo = S_COL_TO_BE_RUNNING_BUT_NOT_ACT;
            }
        }
        else
        {
            pTask->SetManualStop(TRUE);
            pTask->DeActive();
            nRet = 0;
            strErrInfo = S_COLLECTOR_TO_BE_STOPPED;
        }
    }

    //写日志
    SLog log;
    log.LogType = SLog::LOG_OPER;
    log.uIPAddr = pMsgParsed->IPAddr;
    strncpy(log.szUserName, pMsgParsed->UserName, sizeof(log.szUserName));
    log.szUserName[sizeof(log.szUserName) - 1] = 0;

    strncpy(log.szOperCmd, ParseResult.GetCmdName(), sizeof(log.szOperCmd));
    log.szOperCmd[sizeof(log.szOperCmd) - 1] = 0;
    strncpy(log.szCmdDesc, ParseResult.GetCmdDspName(), sizeof(log.szCmdDesc));
    log.szCmdDesc[sizeof(log.szCmdDesc) - 1] = 0;

    if(nRet == 0)
    {
        log.OperResult = SLog::OPER_SUCCESS;
    }
    else
    {
        log.OperResult = SLog::OPER_FAIL;
    }
    
    WriteLog(&log);

    //返回报告
    CMMLReport Report("COLLECTOR");
    Report.MakeReportHead();
    if(nRet < 0)
    {
        nRet = -nRet;
    }
    Report.MakeCmdDisplay(ParseResult.GetCmdLine(), 0, nRet, "SetTaskStatus");
    Report.AddString(strErrInfo.c_str());
    Report.AddString(STR_NEWLINE);
    Report.MakeReportTail();

    const char* pMMLReport  = Report.GetReport();
    SendMMLReport(pMMLReport, pMsgParsed->AppType, pMsgParsed->ClientNo);
}

//设置告警门限
void CCollectorModule::ProcSetALarmLimit(MSG_INTRA* pMsgParsed)
{
    CParseResult ParseResult(pMsgParsed);
    ParseResult.CreateParseResult(pMsgParsed);

    int nRet = -1;

    STRING strErrInfo = S_COLLECTOR_NOT_BE_FOUND;
    CCollectTaskObj* pTask = NULL;
    VECTOR<CCollectTaskObj*>::iterator iter = m_pTaskList->begin();
    for (; iter != m_pTaskList->end(); iter++)
    {
        pTask = *iter;
        if(StringiCmp(pTask->GetColLabel(), ParseResult.GetStringVal(1)))
        {
            int  nAlarmLimit;
            ParseResult.GetDigitVal(&nAlarmLimit, 3);
            if(nAlarmLimit != -1)
            {
                nRet = pTask->SetAlarmLimit(ParseResult.GetStringVal(2), nAlarmLimit);
                if(nRet != ERR_SUCCESS)
                {
                    strErrInfo = S_COL_FAILED_TO_SET_ALARMLIMIT;
                }
                else
                {
                    strErrInfo = S_COL_SET_ALARMLIMIT;
                }
            }
            break;
        }
    }

    //写日志
    SLog log;
    log.LogType = SLog::LOG_OPER;
    log.uIPAddr = pMsgParsed->IPAddr;
    strncpy(log.szUserName, pMsgParsed->UserName, sizeof(log.szUserName));
    log.szUserName[sizeof(log.szUserName) - 1] = 0;

    strncpy(log.szOperCmd, ParseResult.GetCmdName(), sizeof(log.szOperCmd));
    log.szOperCmd[sizeof(log.szOperCmd) - 1] = 0;
    strncpy(log.szCmdDesc, ParseResult.GetCmdDspName(), sizeof(log.szCmdDesc));
    log.szCmdDesc[sizeof(log.szCmdDesc) - 1] = 0;

    if(nRet == 0)
    {
        log.OperResult = SLog::OPER_SUCCESS;
    }
    else
    {
        log.OperResult = SLog::OPER_FAIL;
    }
    
    WriteLog(&log);
    
    //返回报告
    CMMLReport Report("COLLECTOR");
    Report.MakeReportHead();
    if(nRet < 0)
    {
        nRet = -nRet;
    }
    Report.MakeCmdDisplay(ParseResult.GetCmdLine(), 0, nRet, "SetAlarmLimit");
    Report.AddString(strErrInfo.c_str());
    Report.AddString(STR_NEWLINE);
    Report.MakeReportTail();

    const char* pMMLReport  = Report.GetReport();
    SendMMLReport(pMMLReport,
                  pMsgParsed->AppType,
                  pMsgParsed->ClientNo);
}

//查询指定模块的计费信息
void CCollectorModule::ProcListModuleInfo(MSG_INTRA* pMsgParsed)
{
    CParseResult ParseResult(pMsgParsed);
    ParseResult.CreateParseResult(pMsgParsed);

    int nRet = -1;
    LIST<STRING> label;
    LIST<STRING> content;

    CCollectTaskObj* pTask = NULL;
    VECTOR<CCollectTaskObj*>::iterator iter = m_pTaskList->begin();
    for (; iter != m_pTaskList->end(); iter++)
    {
        pTask = *iter;
        if(StringiCmp(pTask->GetColLabel(), ParseResult.GetStringVal(1)))
        {
            nRet = pTask->GetModuleInfo(ParseResult.GetStringVal(2), label, content);
            if((nRet != 0) || (label.size() == 0))
            {
                //返回报告
                CMMLReport Report("COLLECTOR");
                Report.MakeReportHead();
                Report.MakeCmdDisplay(ParseResult.GetCmdLine(), 0, 1, "ListModuleInfo");
                Report.AddString(S_COL_FAIL_TO_GET_MOD_INFO);
                Report.AddString(STR_NEWLINE);
                Report.MakeReportTail();

                const char* pMMLReport  = Report.GetReport();
                SendMMLReport(pMMLReport,
                              pMsgParsed->AppType,
                              pMsgParsed->ClientNo);
                return;
            }
        }
    }

    if(content.size() > 0)
    {
        int nSubPageNo = 1;
        const char* szReport;
        CQBMMLReport* pReport = new CQBMMLReport;
        while (!content.empty() || (1 == nSubPageNo))
        {
            pReport->MakeQBReportHead(ParseResult.GetCmdLine());
            pReport->AddQBHoriTable(label, content,
                                    "ListModuleInfo", nSubPageNo);
            pReport->MakeReportTail();

            szReport = pReport->GetReport();
            SendMMLReport(szReport,
                          pMsgParsed->AppType,
                          pMsgParsed->ClientNo);
        }
    }
    else
    {
        CMMLReport Report("COLLECTOR");
        Report.MakeReportHead();
        Report.MakeCmdDisplay(ParseResult.GetCmdLine(), 0, 1, "ListModuleInfo");
        Report.AddString(S_COLLECTOR_NOT_BE_FOUND);
        Report.AddString(STR_NEWLINE);
        Report.MakeReportTail();

        const char* pMMLReport  = Report.GetReport();
        SendMMLReport(pMMLReport,
                      pMsgParsed->AppType,
                      pMsgParsed->ClientNo);
    }
}

//手动采集文件
void CCollectorModule::ProcGetFile(MSG_INTRA* pMsgParsed)
{
    CParseResult ParseResult(pMsgParsed);
    ParseResult.CreateParseResult(pMsgParsed);

    int nRet = ERR_FAIL;

    STRING strErrInfo = S_COLLECTOR_NOT_BE_FOUND;

    //判断目标文件名是否合法
    const char* pszDstFile = ParseResult.GetStringVal(4);
    if(pszDstFile != NULL && pszDstFile[0] != '\0')
    {
        STRING strPath;
        STRING strFile;
        divide_file_path_name(pszDstFile,
                              strPath,
                              strFile);
        if((strFile.find('*') != STRING::npos)
           || (strFile.find('\\') != STRING::npos)
           || (strFile.find('/') != STRING::npos)
           || (strFile.find('?') != STRING::npos)
           || (strFile.find('>') != STRING::npos)
           || (strFile.find('<') != STRING::npos)
           || (strFile.find(':') != STRING::npos)
           || (strFile.find('"') != STRING::npos)
           || (strFile.find('|') != STRING::npos))
        {
            strErrInfo = S_COL_FILE_NAME_ILLEGAL;
            nRet = ERR_FILE_NAME_NOT_CORRECT;
        }
        else
        {
            CreateDeepDir(strPath.c_str());
        }
    }

    if(ERR_FILE_NAME_NOT_CORRECT != nRet)
    {
        CCollectTaskObj* pTask = NULL;
        VECTOR<CCollectTaskObj*>::iterator iter = m_pTaskList->begin();
        for (; iter != m_pTaskList->end(); iter++)
        {
            pTask = *iter;
            if(StringiCmp(pTask->GetColLabel(), ParseResult.GetStringVal(1)))
            {
                nRet = pTask->GetSpecFile(ParseResult.GetStringVal(2),
                                          ParseResult.GetStringVal(3),
                                          ParseResult.GetStringVal(4));
                switch(nRet)
                {
                case ERR_SUCCESS:
                    strErrInfo = S_COL_FILE_BE_PUT_IN_LIST;
                    break;

                case ERR_FILE_BE_DELETED:
                    strErrInfo = S_COL_FILE_HAVE_BE_REMOVED;
                    break;

                case ERR_FILE_BEING_COLLECTED:
                    strErrInfo = S_COL_FILE_BE_COLLECTING;
                    break;

                case ERR_NEW_FILE_CANNOT_BE_CO:
                    strErrInfo = S_COL_FILE_BE_NOT_CLOSE;
                    break;

                case ERR_FILE_CANNOT_BE_FOUND:
                    strErrInfo = S_COL_FILE_BE_NOT_FOUND;
                    break;

                case ERR_FILE_STATUS_NOT_CORRECT:
                    strErrInfo = S_COL_FILE_STATUS_ERROR;
                    break;

                case ERR_FILE_BE_PUT_IN_LIST:
                    strErrInfo = S_COL_FILE_BE_PUT_IN_LIST;
                    break;

                default:
                    strErrInfo = S_COL_FILE_CANNOT_BE_MAN_COL;
                    break;
                }
                break;
            }
        }
    }

    //返回报告
    CMMLReport Report("COLLECTOR");
    Report.MakeReportHead();
    if(nRet < 0)
    {
        nRet = -nRet;
    }
    Report.MakeCmdDisplay(ParseResult.GetCmdLine(), 0, nRet, "CollectFile");
    Report.AddString(strErrInfo.c_str());
    Report.AddString(STR_NEWLINE);
    Report.MakeReportTail();

    const char* pMMLReport  = Report.GetReport();
    SendMMLReport(pMMLReport, pMsgParsed->AppType, pMsgParsed->ClientNo);
}

void CCollectorModule::SendMMLReport(const char* szReport,
                                     int nAppType,
                                     int nClientNo)
{
    int nStrLen = strlen(szReport);

    MSG_INTRA* pMsg = new(nStrLen + 1) MSG_INTRA;
    pMsg->SenderPid   = GetProcessType();
    pMsg->SenderMid   = GetModuleType();
    pMsg->ReceiverPid = PT_OM;
    pMsg->ReceiverMid = MT_MMLDEBUG;
    pMsg->AppType     = nAppType;
    pMsg->ClientNo    = nClientNo;

    memcpy(pMsg->Data, szReport, nStrLen + 1);

    SendMsg(pMsg);
}

//added by chenliangwei
//加载状态文件
int CCollectorModule::LoadStatus()
{
    m_pStatusFile = fopen(m_szStatusFile, "rb+");
    if(NULL == m_pStatusFile)
    {
        //创建新文件
        m_pStatusFile = fopen(m_szStatusFile, "wb+");

        if (NULL == m_pStatusFile)
        {
            TRACE(S_CMU_FAILED_TO_OPEN_STATUS, m_szStatusFile);
            return ERR_FAIL;
        }
        else
        {
            fclose(m_pStatusFile);
            m_pStatusFile = NULL;

            return ERR_SUCCESS;
        }
    }

    int nFileLength = FileSize(m_pStatusFile);
    if(nFileLength < 2 * sizeof(int))
    {
        fclose(m_pStatusFile);
        m_pStatusFile = NULL;

        return ERR_SUCCESS;
    }

    int nItemCount = nFileLength / sizeof(int);
    int *pReadBuffer = new int[nItemCount];
    int nRet = fread(pReadBuffer,
                 nFileLength,
                 1,
                 m_pStatusFile);
    fclose(m_pStatusFile);
    m_pStatusFile = NULL;

    if(1 != nRet)
    {
        delete []pReadBuffer;

        TRACE(S_CMU_FAILED_TO_READ_STATUS, m_szStatusFile);
        return ERR_FAIL;
    }

    m_nFileNum = pReadBuffer[0];

    m_bFileIsSended = (pReadBuffer[1] == 1);


    delete []pReadBuffer;

    return ERR_SUCCESS;
}

//保存状态文件
int CCollectorModule::SaveStatus()
{
    m_pStatusFile = fopen(m_szStatusFile, "wb");
    if(NULL == m_pStatusFile)
    {
        TRACE(S_CMU_FAILED_TO_OPEN_STATUS, m_szStatusFile);
        return ERR_FAIL;

    }

    int nRet = 0;
    int nItemCount = 2;
    int *pWriteBuffer = new int[nItemCount];

    pWriteBuffer[0] = m_nFileNum;
    pWriteBuffer[1] = (m_bFileIsSended ? 1 : 0);

    int nFileLength = nItemCount * sizeof(int);

    nRet = fwrite(pWriteBuffer,
                  nFileLength,
                  1,
                  m_pStatusFile);

    if(1 != nRet)
    {
        fclose(m_pStatusFile);
        m_pStatusFile = NULL;
        delete []pWriteBuffer;

        TRACE(S_CMU_FAILED_TO_WRITE_STATUS, m_szStatusFile);
        return ERR_FAIL;
    }


    nRet = fflush(m_pStatusFile);
    fclose(m_pStatusFile);
    m_pStatusFile = NULL;

    delete []pWriteBuffer;

    ACE_OS::truncate(m_szStatusFile, nFileLength);

    return nRet;
}
//added end

//查询采集计划
void CCollectorModule::ProcListColSch(MSG_INTRA* pMsgParsed)
{
    CParseResult ParseResult(pMsgParsed);
    ParseResult.CreateParseResult(pMsgParsed);

    int nRet = -1;
    LIST<STRING> label;
    LIST<STRING> content;
    int nBegin;
    int nEnd;
    int nCycle;
    int nCntFailCycle;
    int nColFailCycle;
    int nFileRetryTimes;

    label.push_back(S_COL_SCHEDULE_TYPE);
    label.push_back(S_COL_SCHEDULE_INFO);
    label.push_back(S_CNT_RETRY_CYCLE);
    label.push_back(S_RECOL_RETRY_CYCLE);
    label.push_back(S_RECOL_RETRY_TIMES);
    CCollectTaskObj* pTask = NULL;
    VECTOR<CCollectTaskObj*>::iterator iter = m_pTaskList->begin();
    for (; iter != m_pTaskList->end(); iter++)
    {
        pTask = *iter;
        if(StringiCmp(pTask->GetColLabel(), ParseResult.GetStringVal(1)))
        {
            char szBuf[50];
            LIST<int> time_list;
            nRet = pTask->GetColRetryPara(nCntFailCycle,
                                          nColFailCycle,
                                          nFileRetryTimes);
            nRet = pTask->GetColCyclePara(nBegin,
                                          nEnd,
                                          nCycle);
            nRet = pTask->GetColTimeList(time_list);
            if(ERR_SUCCESS != nRet)
            {
                CMMLReport Report("COLLECTOR");
                Report.MakeReportHead();
                Report.MakeCmdDisplay(ParseResult.GetCmdLine(),
                                      0, 1, "ListColSchdule");
                Report.AddString(S_FAILED_TO_QUERY_COL_SCH);
                Report.AddString(STR_NEWLINE);
                Report.MakeReportTail();

                const char* pMMLReport  = Report.GetReport();
                SendMMLReport(pMMLReport,
                              pMsgParsed->AppType,
                              pMsgParsed->ClientNo);
                break;
            }

            if(time_list.size() > 0)
            {
                //采集计划类型为1表示时间序列
                content.push_back("1");

                szBuf[0] = '\0';
                STRING strTimeList;
                LIST<int>::iterator it = time_list.begin();
                for(; it != time_list.end(); it++)
                {
                    //转换为分钟
                    int nTime = *it / 60;
                    SNPRINTF(szBuf,
                             sizeof(szBuf),
                             "%02d:%02d&",
                             nTime/60,
                             nTime%60);
                    strTimeList += szBuf;
                }
                content.push_back(strTimeList);
            }
            else
            {
                content.push_back("0");

                //采集计划信息
                nBegin /= 60;
                nEnd   /= 60;
                SNPRINTF(szBuf,
                         sizeof(szBuf),
                         "%d&%02d:%02d&%02d:%02d&",
                         nCycle,
                         nBegin/60,
                         nBegin%60,
                         nEnd/60,
                         nEnd%60);
                content.push_back(szBuf);
            }

            //连接失败重试时间
            SNPRINTF(szBuf,
                     sizeof(szBuf),
                     "%d",
                     nCntFailCycle);
            content.push_back(szBuf);

            //采集失败重试时间
            SNPRINTF(szBuf,
                     sizeof(szBuf),
                     "%d",
                     nColFailCycle);
            content.push_back(szBuf);

            //采集失败重试次数
            SNPRINTF(szBuf,
                     sizeof(szBuf),
                     "%d",
                     nFileRetryTimes);
            content.push_back(szBuf);

            break;
        }
    }

    if(content.size() > 0)
    {
        int nSubPageNo = 1;
        const char* szReport;
        CQBMMLReport* pReport = new CQBMMLReport;
        while (!content.empty() || (1 == nSubPageNo))
        {
            pReport->MakeQBReportHead(ParseResult.GetCmdLine());
            pReport->AddQBHoriTable(label, content,
                                    "ListColSchdule",
                                    nSubPageNo);
            pReport->MakeReportTail();

            szReport = pReport->GetReport();
            SendMMLReport(szReport,
                          pMsgParsed->AppType,
                          pMsgParsed->ClientNo);
        }
    }
    else
    {
        CMMLReport Report("COLLECTOR");
        Report.MakeReportHead();
        Report.MakeCmdDisplay(ParseResult.GetCmdLine(),
                              0, 1, "ListColSchdule");
        Report.AddString(S_COLLECTOR_NOT_BE_FOUND);
        Report.AddString(STR_NEWLINE);
        Report.MakeReportTail();

        const char* pMMLReport  = Report.GetReport();
        SendMMLReport(pMMLReport,
                      pMsgParsed->AppType,
                      pMsgParsed->ClientNo);
    }
}

//设置采集计划
void CCollectorModule::ProcSetColSch(MSG_INTRA* pMsgParsed)
{
    CParseResult ParseResult(pMsgParsed);
    ParseResult.CreateParseResult(pMsgParsed);

    int nRet = -1;
    STRING strErrInfo = S_COLLECTOR_NOT_BE_FOUND;

    VECTOR<CCollectTaskObj*>::iterator iter = m_pTaskList->begin();
    for (; iter != m_pTaskList->end(); iter++)
    {
        CCollectTaskObj* pTask = *iter;
        if(StringiCmp(pTask->GetColLabel(), ParseResult.GetStringVal(1)))
        {
            int nColSchType;
            ParseResult.GetDigitVal(&nColSchType, 2);

            char szColSchTime[1024];
            strncpy(szColSchTime, ParseResult.GetStringVal(3), sizeof(szColSchTime));
            szColSchTime[sizeof(szColSchTime) - 1] = '\0';
            if(nColSchType == 0)
            {
                strErrInfo = S_COL_SCH_INFO_INVALID;
                const char *pToken = strtok(szColSchTime, "&");
                if(pToken == NULL)
                {
                    break;
                }
                int nCycle = atoi(pToken);

                pToken = strtok(NULL, "&");
                if(pToken == NULL)
                {
                    break;
                }
                int nBegin = GetTimeFromString(pToken);

                pToken = strtok(NULL, "&");
                if(pToken == NULL)
                {
                    break;
                }
                int nEnd = GetTimeFromString(pToken);

                nRet = pTask->SetColCyclePara(nBegin,
                                              nEnd,
                                              nCycle);
                if(ERR_SUCCESS == nRet)
                {
                    LIST<int> time_list;
                    nRet = pTask->SetColTimeList(time_list);
                }
            }
            else if(nColSchType == 1)
            {
                const char *pToken = strtok(szColSchTime, "&");
                LIST<int> time_list;
                while(pToken)
                {
                    int nTime = GetTimeFromString(pToken);

                    BOOL bFind = FALSE;
                    LIST<int>::iterator it = time_list.begin();
                    for(; it != time_list.end(); it++)
                    {
                        if(nTime == (*it))
                        {
                            bFind = TRUE;
                            break;
                        }
                    }
                    if(!bFind) time_list.push_back(nTime);
                    pToken = strtok(NULL, "&");
                }
                time_list.sort();
                nRet = pTask->SetColTimeList(time_list);
            }

            int nCntFailCycle;
            ParseResult.GetDigitVal(&nCntFailCycle, 4);
            int nColFailCycle;
            ParseResult.GetDigitVal(&nColFailCycle, 5);
            int nFileRetryTimes;
            ParseResult.GetDigitVal(&nFileRetryTimes, 6);
            if(ERR_SUCCESS == nRet)
            {
                nRet = pTask->SetColRetryPara(nCntFailCycle,
                                              nColFailCycle,
                                              nFileRetryTimes);
            }

            if(ERR_SUCCESS != nRet)
            {
                strErrInfo = S_FAILED_TO_SET_COL_SCH;
            }
            else
            {
                strErrInfo = S_SUC_SET_COL_SCH;
            }

            break;
        }
    }

    //写日志
    SLog log;
    log.LogType = SLog::LOG_OPER;
    log.uIPAddr = pMsgParsed->IPAddr;
    strncpy(log.szUserName, pMsgParsed->UserName, sizeof(log.szUserName));
    log.szUserName[sizeof(log.szUserName) - 1] = 0;

    strncpy(log.szOperCmd, ParseResult.GetCmdName(), sizeof(log.szOperCmd));
    log.szOperCmd[sizeof(log.szOperCmd) - 1] = 0;
    strncpy(log.szCmdDesc, ParseResult.GetCmdDspName(), sizeof(log.szCmdDesc));
    log.szCmdDesc[sizeof(log.szCmdDesc) - 1] = 0;

    if(nRet == 0)
    {
        log.OperResult = SLog::OPER_SUCCESS;
    }
    else
    {
        log.OperResult = SLog::OPER_FAIL;
    }
    
    WriteLog(&log);
    
    //返回报告
    CMMLReport Report("COLLECTOR");
    Report.MakeReportHead();
    if(nRet < 0)
    {
        nRet = -nRet;
    }
    Report.MakeCmdDisplay(ParseResult.GetCmdLine(),
                          0, nRet, "SetColSchdule");
    Report.AddString(strErrInfo.c_str());
    Report.AddString(STR_NEWLINE);
    Report.MakeReportTail();

    const char* pMMLReport  = Report.GetReport();
    SendMMLReport(pMMLReport,
                  pMsgParsed->AppType,
                  pMsgParsed->ClientNo);

}


//查询单次采集计划
void CCollectorModule::ProcListOnceColSch(MSG_INTRA* pMsgParsed)
{
    CParseResult ParseResult(pMsgParsed);
    ParseResult.CreateParseResult(pMsgParsed);

    int nRet = -1;
    LIST<STRING> label;
    LIST<STRING> content;

    label.push_back(S_COL_SCH_INFO);
    CCollectTaskObj* pTask = NULL;
    VECTOR<CCollectTaskObj*>::iterator iter = m_pTaskList->begin();
    for (; iter != m_pTaskList->end(); iter++)
    {
        pTask = *iter;
        if(StringiCmp(pTask->GetColLabel(), ParseResult.GetStringVal(1)))
        {
            char szBuf[50];
            LIST<time_t> time_list;
            nRet = pTask->GetOnceColSchl(time_list);
            if(ERR_SUCCESS != nRet)
            {
                CMMLReport Report("COLLECTOR");
                Report.MakeReportHead();
                Report.MakeCmdDisplay(ParseResult.GetCmdLine(),
                                      0, 1, "ListOnceColSchdule");
                Report.AddString(S_FAILED_TO_QUERY_ONCE_SCH);
                Report.AddString(STR_NEWLINE);
                Report.MakeReportTail();

                const char* pMMLReport  = Report.GetReport();
                SendMMLReport(pMMLReport,
                              pMsgParsed->AppType,
                              pMsgParsed->ClientNo);
                break;
            }

            if(time_list.size() > 0)
            {
                LIST<time_t>::iterator it = time_list.begin();
                for(; it != time_list.end(); it++)
                {
                    time_t tTime = *it;
                    tm    tmTime = *localtime(&tTime);
                    strftime(szBuf,
                             sizeof(szBuf),
                             "%Y&%m&%d&%H&%M&",
                             &tmTime);
                    content.push_back(szBuf);
                }
            }
            else
            {
                content.push_back(" ");
            }

            break;
        }
    }

    if(content.size() > 0)
    {
        int nSubPageNo = 1;
        const char* szReport;
        CQBMMLReport* pReport = new CQBMMLReport;
        while (!content.empty() || (1 == nSubPageNo))
        {
            pReport->MakeQBReportHead(ParseResult.GetCmdLine());
            pReport->AddQBHoriTable(label, content,
                                    "ListOnceColSchdule",
                                    nSubPageNo);
            pReport->MakeReportTail();

            szReport = pReport->GetReport();
            SendMMLReport(szReport,
                          pMsgParsed->AppType,
                          pMsgParsed->ClientNo);
        }
    }
    else
    {
        CMMLReport Report("COLLECTOR");
        Report.MakeReportHead();
        Report.MakeCmdDisplay(ParseResult.GetCmdLine(),
                              0, 1, "ListOnceColSchdule");
        Report.AddString(S_COLLECTOR_NOT_BE_FOUND);
        Report.AddString(STR_NEWLINE);
        Report.MakeReportTail();

        const char* pMMLReport  = Report.GetReport();
        SendMMLReport(pMMLReport,
                      pMsgParsed->AppType,
                      pMsgParsed->ClientNo);
    }
}


//设置单次采集计划
void CCollectorModule::ProcSetOnceColSch(MSG_INTRA* pMsgParsed)
{
    CParseResult ParseResult(pMsgParsed);
    ParseResult.CreateParseResult(pMsgParsed);
    
    int nRet = -1;
    STRING strErrInfo = S_COLLECTOR_NOT_BE_FOUND;
    
    VECTOR<CCollectTaskObj*>::iterator iter = m_pTaskList->begin();
    for (; iter != m_pTaskList->end(); iter++)
    {
        CCollectTaskObj* pTask = *iter;
        if(StringiCmp(pTask->GetColLabel(), ParseResult.GetStringVal(1)))
        {
            char szColSchTime[1024] = "";
            strncpy(szColSchTime, ParseResult.GetStringVal(2), sizeof(szColSchTime));
            szColSchTime[sizeof(szColSchTime) - 1] = '\0';

            StringLRTrim(szColSchTime);

            LIST<time_t> time_list;
            if (szColSchTime[0] != '\0')
            {
                //在时间序列的最后加上'^'
                int nstrColLen = strlen(szColSchTime);
                if (szColSchTime[nstrColLen - 1] != '^')
                {
                    szColSchTime[nstrColLen] = '^';
                    szColSchTime[nstrColLen + 1] = '\0';
                }
            
                //拆分一次性时间序列，注:约定时间序列分隔符为'^'
                char *ss0 = NULL;
                char *ss1 = NULL;
                ss0 = szColSchTime;
                while ((ss1 = strchr(ss0, '^')) != NULL)
                {
                    *ss1 = '\0';
                
                    //将时间格式(Y&M&D&H&m&S)转换为time_t
                    tm tmTime = {0};
                    const char *pToken = strtok(ss0, "&");
                    ss0 = ss1+1;
                
                    strErrInfo = S_COL_SCH_TIME_INVALID;
                    if(pToken == NULL)
                    {
                        continue;
                    }
                    tmTime.tm_year = atoi(pToken) - 1900;
                
                    pToken = strtok(NULL, "&");
                    if(pToken == NULL)
                    {
                        continue;
                    }
                    tmTime.tm_mon = atoi(pToken) - 1;
                
                    pToken = strtok(NULL, "&");
                    if(pToken == NULL)
                    {
                        continue;
                    }
                    tmTime.tm_mday = atoi(pToken);
                
                    pToken = strtok(NULL, "&");
                    if(pToken == NULL)
                    {
                        continue;
                    }
                    tmTime.tm_hour = atoi(pToken);
                
                    pToken = strtok(NULL, "&");
                    if(pToken == NULL)
                    {
                        continue;
                    }
                    tmTime.tm_min = atoi(pToken);
                    tmTime.tm_sec = 0;
                
                    time_t tTime = mktime(&tmTime);
                
                    BOOL bFind = FALSE;
                    LIST<time_t>::iterator it = time_list.begin();
                    for(; it != time_list.end(); it++)
                    {
                        if(tTime == (*it))
                        {
                            bFind = TRUE;
                            break;
                        }
                    }
                
                    if(!bFind) 
                    {
                        time_list.push_back(tTime);
                    }
                
                }
            }
            
            time_list.sort();
            nRet = pTask->SetOnceColSchl(time_list);
            
            if(ERR_SUCCESS == nRet)
            {
                strErrInfo = S_SUC_SET_COL_SCH;
            }
            else
            {
                strErrInfo = S_FAILED_TO_SET_COL_SCH;
            }
            
            break;
        }
    }
    
    //写日志
    SLog log;
    log.LogType = SLog::LOG_OPER;
    log.uIPAddr = pMsgParsed->IPAddr;
    strncpy(log.szUserName, pMsgParsed->UserName, sizeof(log.szUserName));
    log.szUserName[sizeof(log.szUserName) - 1] = 0;
    
    strncpy(log.szOperCmd, ParseResult.GetCmdName(), sizeof(log.szOperCmd));
    log.szOperCmd[sizeof(log.szOperCmd) - 1] = 0;
    strncpy(log.szCmdDesc, ParseResult.GetCmdDspName(), sizeof(log.szCmdDesc));
    log.szCmdDesc[sizeof(log.szCmdDesc) - 1] = 0;
    
    if(nRet == 0)
    {
        log.OperResult = SLog::OPER_SUCCESS;
    }
    else
    {
        log.OperResult = SLog::OPER_FAIL;
    }
    
    WriteLog(&log);
    
    //返回报告
    CMMLReport Report("COLLECTOR");
    Report.MakeReportHead();
    if(nRet < 0)
    {
        nRet = -nRet;
    }
    Report.MakeCmdDisplay(ParseResult.GetCmdLine(),
        0, nRet, "SetOnceColSchdule");
    Report.AddString(strErrInfo.c_str());
    Report.AddString(STR_NEWLINE);
    Report.MakeReportTail();
    
    const char* pMMLReport  = Report.GetReport();
    SendMMLReport(pMMLReport,
        pMsgParsed->AppType,
        pMsgParsed->ClientNo);
}



