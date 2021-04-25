#include "co_agent.h"
#include "../include/toolbox.h"
#include "../utility/mml_para.h"
#include "../utility/mml_report.h"

/**************************************************************************
函数原型    public: CCollectorAgent (void);
功能描述    构造函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
CCollectorAgent::CCollectorAgent(void)
{
    m_lTimerID5Sec = 0;
    m_lSHKTimeOut  = 60;
}

/**************************************************************************
函数原型    public: ~CCollectorAgent ();
功能描述    构造函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
CCollectorAgent::~CCollectorAgent()
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
void CCollectorAgent::ExitInstance (void)
{
    KillTimer(m_lTimerID5Sec);

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
int CCollectorAgent::InitInstance (void)
{
    int nRet = ERR_FAIL;
    nRet = CWorkModule::InitInstance();
    if( nRet != ERR_SUCCESS )
    {
        return nRet;
    }

    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();
    char szCfgPath[MAX_PATH];
    IniFile.GetString(CFG_SEC_COLLECT,
                      CFG_CCS_CFG_DIR,
                      CFG_CCS_CFG_DIR_DEFAULT,
                      szCfgPath,
                      sizeof(szCfgPath));

    SNPRINTF(m_szCfgFile,
             sizeof(m_szCfgFile),
             "%s/ccs_cfg_info.dat",
             szCfgPath);

    m_lTimerID5Sec = SetTimer(FIVE_SECOND_IN_MILISEC);

    LoadCCSConfig();

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
MODULE_TYPE CCollectorAgent::GetModuleType ( void )
{
    return MT_COLAGENT;
}

/**************************************************************************
函数原型    protected: void ParseMsg ( MSG_INTRA* pMsgParsed )
功能描述    命令解析与分发
输入参数    消息指针
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CCollectorAgent::ParseMsg ( MSG_INTRA* pMsgParsed )
{
    CParseResult* pParseResult = NULL;
    const char* szColLabel;
    int nActive;

    switch(pMsgParsed->CmdCode)
    {
    case CMD_REPORT_COLLECTOR_STATUS:
        //各个采集模块上报状态
        memcpy(&nActive, pMsgParsed->Data, sizeof(nActive));
        szColLabel = (char*)(pMsgParsed->Data + sizeof(nActive));
        m_ColToStatus[szColLabel]   = nActive;
        m_ColToProcType[szColLabel] = pMsgParsed->SenderPid;
        m_ColToSHKTime[szColLabel]  = time(NULL);
        break;

    case CMD_MML:
        pParseResult = *((CParseResult** )(pMsgParsed->Data));
        switch (pParseResult->GetCmdCode())
        {
        case MML_LST_COL_INFO:
        case MML_SET_COL_STATUS:
        case MML_SET_ALARM_LIMIT:
        case MML_LST_MODULE_INFO:
        case MML_GET_FILE:
        case MML_LST_COL_SCHDL:
        case MML_SET_COL_SCHDL:
        case MML_LST_A_COL_SCHDL:
        case MML_SET_A_COL_SCHDL:
            if(m_ColToStatus.find(pParseResult->GetStringVal(1)) != m_ColToStatus.end())
            {
                UINT4 nSerialLen = pParseResult->GetParseLength();
                MSG_INTRA* pMsg = new(nSerialLen) MSG_INTRA;

                pParseResult->Serialize((char*)pMsg->Data,nSerialLen);

                pMsg->SenderPid   = GetProcessType();
                pMsg->SenderMid   = GetModuleType();
                pMsg->ReceiverPid = m_ColToProcType[pParseResult->GetStringVal(1)];
                pMsg->ReceiverMid = MT_COLLECTOR;
                pMsg->CmdCode     = CMD_MML;

                pMsg->AppType     = pMsgParsed->AppType;
                pMsg->ClientNo    = pMsgParsed->ClientNo;
                pMsg->IPAddr      = pMsgParsed->IPAddr;
                strncpy(pMsg->UserName,
                        pParseResult->GetUserName(),
                        sizeof(pMsg->UserName));
                pMsg->UserName[sizeof(pMsg->UserName) - 1] = 0;

                SendMsg(pMsg);
                return;
            }
            break;

        //查询各个采集任务的状态
        case MML_LST_COL_STATUS:
            if(m_ColToStatus.find(pParseResult->GetStringVal(1)) != m_ColToStatus.end())
            {
                //返回失败报告
                CMMLReport Report("COLLECTOR");
                Report.MakeReportHead();
                Report.MakeCmdDisplay(pParseResult->GetCmdLine(), 0, 0, "COLLECTOR");
                LIST<STRING> title;
                title.push_back("Task");
                title.push_back("状态");

                LIST<STRING> content;
                content.push_back(pParseResult->GetStringVal(1));
                char szBuf[10];
                SNPRINTF(szBuf, sizeof(szBuf), "%d", m_ColToStatus[pParseResult->GetStringVal(1)]);
                content.push_back(szBuf);
                Report.AddHoriTable(title, content, "List TaskStatus");
                Report.AddString(STR_NEWLINE);
                Report.MakeReportTail();

                const char* pMMLReport  = Report.GetReport();
                SendMMLReport(pMMLReport, pMsgParsed->AppType, pMsgParsed->ClientNo);
                return;
            }
            break;

        case MML_SET_CCSCFGINFO:        //设置采集配置信息
            AddCCSCfgItem(pMsgParsed);
            return;

        case MML_GET_CCSCFGINFO:        //获取采集配置信息
            GetCCSCfgInfo(pMsgParsed);
            return;

        case MML_DEL_CCSCFGITEM:        //删除采集配置信息
            RemoveCCSCfgItem(pMsgParsed);
            return;

        case MML_MOD_CCSCFGINFO:        //修改采集配置信息
            ModifyCCSCfgItem(pMsgParsed);
            return;

        default:
            break;
        }
        {
            //写日志
            SLog log;
            log.LogType = SLog::LOG_OPER;
            log.uIPAddr = pMsgParsed->IPAddr;
            strncpy(log.szUserName, pMsgParsed->UserName, sizeof(log.szUserName));
            log.szUserName[sizeof(log.szUserName) - 1] = 0;

            strncpy(log.szOperCmd, pParseResult->GetCmdName(), sizeof(log.szOperCmd));
            log.szOperCmd[sizeof(log.szOperCmd) - 1] = 0;
            strncpy(log.szCmdDesc, pParseResult->GetCmdDspName(), sizeof(log.szCmdDesc));
            log.szCmdDesc[sizeof(log.szCmdDesc) - 1] = 0;
            log.OperResult = SLog::OPER_FAIL;

            WriteLog(&log);

            //返回失败报告
            CMMLReport Report("COLLECTOR");
            Report.MakeReportHead();
            Report.MakeCmdDisplay(pParseResult->GetCmdLine(), 0, 1, "COLLECTOR");
            Report.AddString("目标对象不存在");
            Report.AddString(STR_NEWLINE);
            Report.MakeReportTail();

            const char* pMMLReport  = Report.GetReport();
            SendMMLReport(pMMLReport, pMsgParsed->AppType, pMsgParsed->ClientNo);
        }
        break;

    default:
        CWorkModule::ParseMsg(pMsgParsed);
        break;
    }
}

/*********************************************************************************************
函数原型        protected: void  OnTimer(const long nTimerID);
功能描述        定时器处理
输入参数        时钟ID ，只读。
输出参数        无。
返回值          无。
抛出异常        无。
**********************************************************************************************/
void CCollectorAgent::OnTimer(const long nTimerID)
{
    if (nTimerID == m_lTimerID5Sec)
    {
        Timer5Sec();
    }

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
void CCollectorAgent::Timer5Sec()
{
    MAP<STRING, time_t>::iterator i = m_ColToSHKTime.begin();

    time_t tNow = time(NULL);
    while(i != m_ColToSHKTime.end())
    {
        if(tNow - i->second > m_lSHKTimeOut)
        {
            m_ColToStatus.erase(i->first);
            m_ColToProcType.erase(i->first);
            m_ColToSHKTime.erase(i->first);
            i = m_ColToSHKTime.begin();
        }
        else
        {
            i++;
        }
    }
}

void CCollectorAgent::SendMMLReport(const char* szReport,
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

BOOL CCollectorAgent::LoadCCSConfig(void)
{
    char szCfgItem[2048];

    //注意：采集任务配置信息最开始可能并不存在
    FILE* fp = fopen(m_szCfgFile, "r");
    if (fp != NULL)
    {
        m_CfgInfo.clear();
        while (!feof(fp))
        {
            fgets(szCfgItem, sizeof(szCfgItem), fp);
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            StringLRTrim(szCfgItem);
            if(strchr(szCfgItem, '/') != NULL)
            {
                m_CfgInfo.push_back(szCfgItem);
            }
            szCfgItem[0] = '\0';
        }

        fclose(fp);
    }

    m_CfgInfo.sort();

    //added by chenliangwei, 2003-07-08
    UpdateUserRight();
    //added end

    return TRUE;

}

//added by chenliangwei, 2003-07-08
BOOL CCollectorAgent::UpdateUserRight(void)
{
    m_UserRight.clear();

    //构建用户权限表
    LIST<STRING>::iterator it;
    it = m_CfgInfo.begin();
    while (it != m_CfgInfo.end())
    {
        STRING& strCfgItem = *it;
        const char* pszTmp  = strCfgItem.c_str();
        const char* pszSep1 = strchr(pszTmp, ':');
        const char* pszSep2 = strchr(pszTmp, '/');
        if((pszSep2 != NULL) && (pszSep1 != NULL) && (pszSep2 < pszSep1 - 1) )
        {
            pszSep2++;
            char szCfgItem[2048];
            const char* pszSep3 = strchr(pszSep2 + 1, '/');
            if(pszSep3 == NULL || pszSep3 > pszSep1)
            {
                pszSep3 = pszSep1;
                int nStrLen = pszSep3 - pszSep2;
                strncpy(szCfgItem, pszSep2, nStrLen);
                szCfgItem[nStrLen] = '\0';
                //printf("szCfgItem:%s\r\n", szCfgItem);

                const char* pszList = strstr(pszSep1, "LIST(");
                if(pszList != NULL && strlen(pszList) > strlen("LIST("))
                {
                    pszList += strlen("LIST(");
                    const char* pszEnd = strchr(pszList, ')');
                    if(pszEnd != NULL && pszList <= pszEnd)
                    {
                        char szUserMsg[2048];
                        strncpy(szUserMsg, pszList, pszEnd - pszList);
                        szUserMsg[pszEnd - pszList] = '\0';

                        MAP<STRING, int> user_vertor;
                        char *pToken = strtok(szUserMsg, "^");
                        while(pToken)
                        {
                            //printf("user:%s\r\n", pToken);
                            StringToLower(pToken);
                            user_vertor[pToken] = 0;
                            pToken = strtok(NULL, "^");
                        }

                        //管理员必定有管理任务的权限
                        user_vertor["admin"] = 0;
                        m_UserRight[szCfgItem] = user_vertor;
                    }
                }
            }
        }
        it++;
    }

    return TRUE;
}
//added end


BOOL CCollectorAgent::WriteCCSConfig(void)
{
    //然后保存配置信息
    m_CfgInfo.sort();

    FILE* fp = fopen(m_szCfgFile, "w");
    if (fp != NULL)
    {
        LIST<STRING>::iterator it = m_CfgInfo.begin();
        while (it != m_CfgInfo.end())
        {
            const char* szCfgItem = it->c_str();
            fprintf(fp, "%s\n", szCfgItem);
            it++;
        }

        fclose(fp);

        return TRUE;
    }

    return FALSE;
}

//删除一条采集任务信息
void CCollectorAgent::RemoveCCSCfgItem(MSG_INTRA* pMsg)
{
    //进程内消息，直接强制类型转换
    CParseResult *pParse = *(CParseResult **)pMsg->Data;
    char szCfgItem[2048];

    //写日志
    SLog log;
    log.LogType = SLog::LOG_OPER;
    log.uIPAddr = pMsg->IPAddr;
    strncpy(log.szUserName, pMsg->UserName, sizeof(log.szUserName));
    log.szUserName[sizeof(log.szUserName) - 1] = 0;
    strncpy(log.szOperCmd, pParse->GetCmdName(), sizeof(log.szOperCmd));
    log.szOperCmd[sizeof(log.szOperCmd) - 1] = 0;
    strncpy(log.szCmdDesc, pParse->GetCmdDspName(), sizeof(log.szCmdDesc));
    log.szCmdDesc[sizeof(log.szCmdDesc) - 1] = 0;

    CMMLReport Report(THISMODULE);
    Report.MakeReportHead();

    int nRet = 1;
    const char* pszTmp = pParse->GetStringVal(1);
    STRING strRetMsg = STR_OPRFAIL;
    int nStrLen = strlen(pszTmp);
    if((nStrLen > 0) && (nStrLen < sizeof(szCfgItem) - 1))
    {
        //added by chenliangwei, 2003-07-08
        //进行用户权限的鉴别
        BOOL  bAuditSuccess = TRUE;
        const char* pszSep2 = strchr(pszTmp, '/');
        if(pszSep2 != NULL) 
        {
            pszSep2++;
            const char* pszSep3 = strchr(pszSep2 + 1, '/');
            if(pszSep3 == NULL)
            {
                pszSep3 = pszTmp + nStrLen;
            }

            int nTaskLen = pszSep3 - pszSep2;
            char szTask[2048];
            strncpy(szTask, pszSep2, nTaskLen);
            szTask[nTaskLen] = '\0';

            //将用户名转换为小写
            char  szUserName[48];
            strncpy(szUserName, pMsg->UserName, sizeof(szUserName));
            szUserName[sizeof(szUserName) - 1] = '\0';
            StringToLower(szUserName);

            if(m_UserRight.find(szTask) != m_UserRight.end())
            {
                MAP<STRING, int>& user_vertor = m_UserRight[szTask];
                if(user_vertor.find(szUserName) == user_vertor.end())
                {
                    log.OperResult = SLog::OPER_FAIL;
                    strRetMsg      = "删除采集任务配置信息失败:用户无权限"; //构造返回报文正文
                    bAuditSuccess = FALSE;
                }

            }
            else
            {
                log.OperResult = SLog::OPER_FAIL;
                strRetMsg      = "删除采集任务配置信息失败:删除的配置信息指定了不存在的采集任务"; //构造返回报文正文
                bAuditSuccess = FALSE;
            }

        }
        //added end

        if (bAuditSuccess)
        {
            //查找szCfgItem是否有两个'/'，如果只有一个说明是采集任务
            const char* pszDiv = strchr(szCfgItem, '/');
            if((pszDiv != NULL) && (pszDiv = strchr(pszDiv + 1, '/')) == NULL)
            {
                log.OperResult = SLog::OPER_FAIL;
                strRetMsg      = "删除采集任务配置信息失败:不能删除采集任务";       //构造返回报文正文
            }
            else
            {
                LIST<STRING>::iterator it = m_CfgInfo.begin();
                while (it != m_CfgInfo.end())
                {
                    STRING& sCfgItem = *it;

                    //按字符串最大匹配的方法找到对应的配置项
                    if (strncmp(pszTmp, sCfgItem.c_str(), nStrLen) == 0)
                    {
                        m_CfgInfo.erase(it);
                        it = m_CfgInfo.begin();
                        continue;
                    }

                    it++;
                }

                //然后保存配置信息
                if (WriteCCSConfig())
                {
                    log.OperResult = SLog::OPER_SUCCESS;
                    strRetMsg      = STR_OPRSUCCESS;    //构造返回报文正文
                    nRet = 0;
                }
                else
                {
                    log.OperResult = SLog::OPER_FAIL;
                    strRetMsg      = "删除采集任务配置信息失败:文件更新失败";       //构造返回报文正文
                }
            }
        }
    }
    else
    {
        log.OperResult = SLog::OPER_FAIL;
        strRetMsg      = "删除采集任务配置信息失败:信息格式不合法";           //构造返回报文正文
    }

    WriteLog(&log);

    Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, nRet, THISMODULE);
    Report.AddString(strRetMsg.c_str());
    Report.AddString(STR_NEWLINE);
    Report.MakeReportTail();

    const char* pChar = Report.GetReport();
    SendMMLReport(pChar, pMsg->AppType, pMsg->ClientNo);
}

void CCollectorAgent::AddCCSCfgItem(MSG_INTRA* pMsg)
{
    //进程内消息，直接强制类型转换
    CParseResult *pParse = *(CParseResult **)pMsg->Data;

    //写日志
    SLog log;
    log.LogType = SLog::LOG_OPER;
    log.uIPAddr = pMsg->IPAddr;
    strncpy(log.szUserName, pMsg->UserName, sizeof(log.szUserName));
    log.szUserName[sizeof(log.szUserName) - 1] = 0;
    strncpy(log.szOperCmd, pParse->GetCmdName(), sizeof(log.szOperCmd));
    log.szOperCmd[sizeof(log.szOperCmd) - 1] = 0;
    strncpy(log.szCmdDesc, pParse->GetCmdDspName(), sizeof(log.szCmdDesc));
    log.szCmdDesc[sizeof(log.szCmdDesc) - 1] = 0;

    CMMLReport Report(THISMODULE);
    Report.MakeReportHead();

    int nRet = 1;
    //检查配置项是否已经存在，如果存在，则认为重复
    //此功能可作为配置信息完备性检查
    const char* pszTmp = pParse->GetStringVal(1);
    const char* pszSep = strchr(pszTmp, ':');
    char szCfgItem[2048];
    STRING strRetMsg = "添加采集任务配置信息失败:格式不正确";
    if((pszSep != NULL) && (pszSep - pszTmp < sizeof(szCfgItem) - 2))
    {
        //added by chenliangwei, 2003-07-08
        //进行用户权限的鉴别
        BOOL  bAuditSuccess = TRUE;
        const char* pszSep2 = strchr(pszTmp, '/');
        if((pszSep2 != NULL) && (pszSep2 < pszSep - 1))
        {
            pszSep2++;
            const char* pszSep3 = strchr(pszSep2 + 1, '/');
            if(pszSep3 == NULL || pszSep3 > pszSep)
            {
                pszSep3 = pszSep;
            }

            int nTaskLen = pszSep3 - pszSep2;
            char szTask[2048];
            strncpy(szTask, pszSep2, nTaskLen);
            szTask[nTaskLen] = '\0';

            //将用户名转换为小写
            char  szUserName[48];
            strncpy(szUserName, pMsg->UserName, sizeof(szUserName));
            szUserName[sizeof(szUserName) - 1] = '\0';
            StringToLower(szUserName);

            if(m_UserRight.find(szTask) != m_UserRight.end())
            {
                MAP<STRING, int>& user_vertor = m_UserRight[szTask];
                if(user_vertor.find(szUserName) == user_vertor.end())
                {
                    log.OperResult = SLog::OPER_FAIL;
                    strRetMsg      = "添加采集任务配置信息失败:用户无权限"; //构造返回报文正文
                    bAuditSuccess = FALSE;
                }

            }
            else
            {
                log.OperResult = SLog::OPER_FAIL;
                strRetMsg      = "添加采集任务配置信息失败:增加的配置信息指定了不存在的采集任务"; //构造返回报文正文
                bAuditSuccess = FALSE;
            }

        }
        //added end

        if (bAuditSuccess)
        {
            int nStrLen = pszSep - pszTmp + 1;
            strncpy(szCfgItem, pszTmp, nStrLen);
            szCfgItem[nStrLen] = '\0';

            const char* pszDiv = strchr(szCfgItem, '/');
            if(pszDiv != NULL)
            {
                const char* pszDiv2 = strchr(pszDiv + 1, '/');
                if(pszDiv2 == NULL)
                //查找szCfgItem是否有两个'/'，如果只有一个说明是采集任务
                {
                    BOOL bFound = FALSE;
                    LIST<STRING>::iterator it = m_CfgInfo.begin();
                    while (it != m_CfgInfo.end())
                    {
                        const char* pszCfgItem = it->c_str();

                        //按字符串最大匹配的方法找到对应的配置项
                        if (strncmp(szCfgItem, pszCfgItem, nStrLen) == 0)
                        {
                            bFound = TRUE;
                            m_CfgInfo.erase(it);
                            break;
                        }

                        it++;
                    }

                    if (!bFound)
                    {
                        log.OperResult = SLog::OPER_FAIL;
                        strRetMsg      = "添加采集任务配置信息失败:不能添加采集任务";
                    }
                    else
                    {
                        m_CfgInfo.push_back(pszTmp);        //添加到尾部

                        //added by chenliangwei, 2003-07-08
                        UpdateUserRight();
                        //added end

                        if(WriteCCSConfig())
                        {
                            log.OperResult = SLog::OPER_SUCCESS;
                            strRetMsg      = STR_OPRSUCCESS;    //构造返回报文正文
                            nRet           = 0;
                        }
                        else
                        {
                            log.OperResult = SLog::OPER_FAIL;
                            strRetMsg      = "添加采集任务配置信息失败:保存配置项失败";
                        }
                    }
                }
                else
                {
                    int nCfgItem1Len = pszDiv2 - szCfgItem;
                    BOOL bFound = FALSE;
                    LIST<STRING>::iterator it = m_CfgInfo.begin();
                    while (it != m_CfgInfo.end())
                    {
                        const char* pszCfgItem = it->c_str();

                        //按字符串最大匹配的方法找到对应的配置项
                        if ((strncmp(szCfgItem, pszCfgItem, nCfgItem1Len) == 0)
                            && (pszCfgItem[nCfgItem1Len]) == ':')
                        {
                            bFound = TRUE;
                            break;
                        }

                        it++;
                    }

                    if (!bFound)
                    {
                        log.OperResult = SLog::OPER_FAIL;
                        strRetMsg      = "添加采集任务配置信息失败:增加的配置信息指定了不存在的采集任务";
                    }
                    else
                    {
                        LIST<STRING>::iterator it = m_CfgInfo.begin();
                        while (it != m_CfgInfo.end())
                        {
                            const char* pszCfgItem = it->c_str();

                            //按字符串最大匹配的方法找到对应的配置项
                            if (strncmp(szCfgItem, pszCfgItem, nStrLen) == 0)
                            {
                                m_CfgInfo.erase(it);
                                break;
                            }

                            it++;
                        }

                        m_CfgInfo.push_back(pszTmp);        //添加到尾部
                        if(WriteCCSConfig())
                        {
                            log.OperResult = SLog::OPER_SUCCESS;
                            strRetMsg      = STR_OPRSUCCESS;    //构造返回报文正文
                            nRet           = 0;
                        }
                        else
                        {
                            log.OperResult = SLog::OPER_FAIL;
                            strRetMsg      = "添加采集任务配置信息失败:保存配置项失败";
                        }
                    }
                }
            }
        }
    }

    WriteLog(&log);

    Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, nRet, THISMODULE);
    Report.AddString(strRetMsg.c_str());        //构造返回报文正文
    Report.AddString(STR_NEWLINE);
    Report.MakeReportTail();

    const char* pChar = Report.GetReport();
    SendMMLReport(pChar, pMsg->AppType, pMsg->ClientNo);
}

void CCollectorAgent::ModifyCCSCfgItem(MSG_INTRA* pMsg)
{
    //进程内消息，直接强制类型转换
    CParseResult *pParse = *(CParseResult **)pMsg->Data;

    //写日志
    SLog log;
    log.LogType = SLog::LOG_OPER;
    log.uIPAddr = pMsg->IPAddr;
    strncpy(log.szUserName, pMsg->UserName, sizeof(log.szUserName));
    log.szUserName[sizeof(log.szUserName) - 1] = 0;
    strncpy(log.szOperCmd, pParse->GetCmdName(), sizeof(log.szOperCmd));
    log.szOperCmd[sizeof(log.szOperCmd) - 1] = 0;
    strncpy(log.szCmdDesc, pParse->GetCmdDspName(), sizeof(log.szCmdDesc));
    log.szCmdDesc[sizeof(log.szCmdDesc) - 1] = 0;

    CMMLReport Report(THISMODULE);
    Report.MakeReportHead();

    int nRet = 1;
    //检查配置项是否已经存在，如果存在，则认为重复
    //此功能可作为配置信息完备性检查
    const char* pszSrc = pParse->GetStringVal(1);
    const char* pszDst = pParse->GetStringVal(2);
    const char* pszSep = strchr(pszSrc, ':');
    char szCfgItem[2048];
    STRING strRetMsg = STR_OPRFAIL;
    if((pszSep != NULL) && (pszSep - pszSrc < sizeof(szCfgItem) - 2))
    {

        //added by chenliangwei, 2003-07-08
        //进行用户权限的鉴别
        BOOL bAuditSuccess = TRUE;
        const char* pszSep2 = strchr(pszSrc, '/');
        if((pszSep2 != NULL) && (pszSep2 < pszSep - 1))
        {
            pszSep2++;
            const char* pszSep3 = strchr(pszSep2 + 1, '/');
            if(pszSep3 == NULL || pszSep3 > pszSep)
            {
                pszSep3 = pszSep;
            }

            int nTaskLen = pszSep3 - pszSep2;
            char szTask[2048];
            strncpy(szTask, pszSep2, nTaskLen);
            szTask[nTaskLen] = '\0';

            //将用户名转换为小写
            char  szUserName[48];
            strncpy(szUserName, pMsg->UserName, sizeof(szUserName));
            szUserName[sizeof(szUserName) - 1] = '\0';
            StringToLower(szUserName);

            if(m_UserRight.find(szTask) != m_UserRight.end())
            {
                MAP<STRING, int>& user_vertor = m_UserRight[szTask];
                if(user_vertor.find(szUserName) == user_vertor.end())
                {
                    log.OperResult = SLog::OPER_FAIL;
                    strRetMsg      = "修改采集任务配置信息失败:用户无权限"; //构造返回报文正文
                    bAuditSuccess = FALSE;
                }

            }
            else
            {
                log.OperResult = SLog::OPER_FAIL;
                strRetMsg      = "修改采集任务配置信息失败:找不到匹配的配置项"; //构造返回报文正文
                bAuditSuccess = FALSE;
            }

        }
        //added end

        if (bAuditSuccess)
        {
            int nStrLen = pszSep - pszSrc + 1;
            strncpy(szCfgItem, pszSrc, nStrLen);
            szCfgItem[nStrLen] = '\0';
            //此时szCfgItem的内容样例为"Huawei CCS/CC08/交换机信息:"

            //查找szCfgItem是否有两个'/'，如果只有一个说明是采集任务
            BOOL bTask = FALSE;
            const char* pszDiv = strchr(szCfgItem, '/');
            if((pszDiv != NULL)
               && ((pszDiv = strchr(pszDiv + 1, '/')) == NULL))
            {
                bTask = TRUE;
            }

            if ((bTask) && (strncmp(szCfgItem, pszDst, nStrLen) != 0))
            {
                //如果修改了采集任务名，则返回失败
                log.OperResult = SLog::OPER_FAIL;
                strRetMsg      = "修改采集任务配置信息失败:不能修改采集任务的名称";
            }
            else
            {
                //从后面找第一个'/'
                pszDiv = strrchr(szCfgItem, '/');
                if(pszDiv != NULL)
                {
                    int nPerSecLen = pszDiv - szCfgItem;
                    if(strncmp(szCfgItem, pszDst, nPerSecLen) == 0)
                    //保存修改前与修改后在同一个父节点中
                    {
                        log.OperResult = SLog::OPER_FAIL;
                        strRetMsg      = "修改采集任务配置信息失败:找不到匹配的配置项"; //构造返回报文正文
                        bool bFound = false;
                        if(strncmp(szCfgItem, pszDst, nStrLen) == 0)
                        //如果不修改节点名
                        {
                            LIST<STRING>::iterator it = m_CfgInfo.begin();
                            while (it != m_CfgInfo.end())
                            {
                                const char* pszCfgItem = it->c_str();

                                //按字符串最大匹配的方法找到对应的配置项
                                if (strncmp(szCfgItem, pszCfgItem, nStrLen) == 0)
                                {
                                    m_CfgInfo.erase(it);
                                    m_CfgInfo.push_back(pszDst);        //添加到尾部
                                    bFound = true;

                                    if (bTask)
                                    {
                                        UpdateUserRight();
                                    }

                                    break;
                                }

                                it++;
                            }
                        }
                        else
                        //如果修改了节点名
                        {
                            int nDstItemNameLen = int(strchr(pszDst, ':') - pszDst) - nPerSecLen - 1;
                            if(nDstItemNameLen > 0)
                            {
                                STRING strDstItem = pszDst;
                                STRING strNewItemName = strDstItem.substr(nPerSecLen + 1, nDstItemNameLen);
                                LIST<STRING>::iterator it = m_CfgInfo.begin();
                                while (it != m_CfgInfo.end())
                                {
                                    STRING& strCfgItem = *it;

                                    if (strncmp(szCfgItem, strCfgItem.c_str(), nStrLen) == 0)
                                    //找到该节点项
                                    {
                                        strCfgItem = pszDst;
                                        bFound = true;
                                    }
                                    else if (strncmp(szCfgItem, strCfgItem.c_str(), nStrLen - 1) == 0)
                                    //该节点的子项
                                    {
                                        strCfgItem.replace(nPerSecLen + 1,
                                                           nStrLen - nPerSecLen - 2,
                                                           strNewItemName);
                                        bFound = true;
                                    }

                                    it++;
                                }
                            }
                        }

                        if(bFound)
                        {
                            //保存配置信息
                            if(WriteCCSConfig())
                            {
                                log.OperResult = SLog::OPER_SUCCESS;
                                strRetMsg      = STR_OPRSUCCESS;    //构造返回报文正文
                                nRet           = 0;
                            }
                            else
                            {
                                log.OperResult = SLog::OPER_FAIL;
                                strRetMsg      = "修改采集任务配置信息失败:保存配置项失败"; //构造返回报文正文
                            }
                        }
                    }
                    else
                    {
                        log.OperResult = SLog::OPER_FAIL;
                        strRetMsg      = "修改采集任务配置信息失败:不能同时修改两个节点的名称";
                    }
                }
                else
                {
                    log.OperResult = SLog::OPER_FAIL;
                    strRetMsg      = "修改采集任务配置信息失败:格式不正确";
                }
            }
        }
    }

    WriteLog(&log);

    Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, nRet, THISMODULE);
    Report.AddString(strRetMsg.c_str());        //构造返回报文正文
    Report.AddString(STR_NEWLINE);
    Report.MakeReportTail();

    const char* pChar = Report.GetReport();
    SendMMLReport(pChar, pMsg->AppType, pMsg->ClientNo);
}

void CCollectorAgent::GetCCSCfgInfo(MSG_INTRA* pMsg)
{
    CParseResult *pParse = *(CParseResult **)pMsg->Data;

    //写日志
    SLog log;
    log.LogType    = SLog::LOG_OPER;
    log.OperResult = SLog::OPER_SUCCESS;
    log.uIPAddr    = pMsg->IPAddr;
    strncpy(log.szUserName, pMsg->UserName, sizeof(log.szUserName));
    log.szUserName[sizeof(log.szUserName) - 1] = 0;
    strncpy(log.szOperCmd, pParse->GetCmdName(), sizeof(log.szOperCmd));
    log.szOperCmd[sizeof(log.szOperCmd) - 1] = 0;
    strncpy(log.szCmdDesc, pParse->GetCmdDspName(), sizeof(log.szCmdDesc));
    log.szCmdDesc[sizeof(log.szCmdDesc) - 1] = 0;
    WriteLog(&log);

    CMMLReport Report(THISMODULE);
    Report.MakeReportHead();        //构造报文头
    LIST<STRING>::iterator it;

    //为没有配置信息的采集任务配置缺省项
    MAP<STRING, int>::iterator iter;
    for(iter = m_ColToStatus.begin(); iter != m_ColToStatus.end(); iter++)
    {
        char szCfgItem[2048];
        SNPRINTF(szCfgItem,
                 sizeof(szCfgItem),
                 "Huawei CCS/%s:",
                 iter->first.c_str());
        szCfgItem[sizeof(szCfgItem) - 1] = '\0';
        int nStrLen = strlen(szCfgItem);

        bool bFound = false;
        it = m_CfgInfo.begin();
        while (it != m_CfgInfo.end())
        {
            const char* pszCfgItem = it->c_str();
            if(strncmp(szCfgItem, pszCfgItem, nStrLen) == 0)
            {
                bFound = true;
                break;
            }
            it++;
        }

        if(!bFound)
        {
            //采集任务名
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s:LIST()",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //交换机信息
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/交换机信息:STRING(交换机信息)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //交换机信息/交换机名称
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/交换机信息/交换机名称:STRING(交换机名称)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //交换机信息/交换机版本
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/交换机信息/交换机名称:STRING(交换机版本)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //交换机信息/交换局名称
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/交换机信息/交换局名称:STRING(交换局名称)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //交换机信息/交换局类别
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/交换机信息/交换局类别:STRING(交换局类别)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //交换机信息/号码段
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/交换机信息/号码段:UINT(0755)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //交换机信息/所属采集机
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/交换机信息/所属采集机:STRING(所属采集机)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //交换机信息/采集端口数
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/交换机信息/采集端口数:UINT(2)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //交换机信息/采集端口1
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/交换机信息/采集端口1:STRING(采集端口1)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //交换机信息/采集端口1/端口名称
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/交换机信息/采集端口1/端口名称:STRING(端口名称)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //交换机信息/采集端口1/端口类型
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/交换机信息/采集端口1/端口类型:STRING(端口类型)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //交换机信息/采集端口1/端口速率
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/交换机信息/采集端口1/端口速率:STRING(64Kbps)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //交换机信息/采集端口2
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/交换机信息/采集端口2:STRING(采集端口2)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //交换机信息/采集端口2/端口名称
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/交换机信息/采集端口2/端口名称:STRING(端口名称)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //交换机信息/采集端口2/端口类型
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/交换机信息/采集端口2/端口类型:STRING(端口类型)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //交换机信息/采集端口2/端口速率
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/交换机信息/采集端口2/端口速率:STRING(64Kbps)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //前置采集机信息
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/前置采集机信息:STRING(前置采集机信息)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //前置采集机信息/采集机名称
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/前置采集机信息/采集机名称:STRING(采集机名称)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //前置采集机信息/采集端口数
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/前置采集机信息/采集端口数:UINT(2)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //前置采集机信息/采集端口1
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/前置采集机信息/采集端口1:STRING(采集端口1)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //前置采集机信息/采集端口1/端口名称
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/前置采集机信息/采集端口1/端口名称:STRING(端口名称)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //前置采集机信息/采集端口1/端口类型
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/前置采集机信息/采集端口1/端口类型:STRING(端口类型)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //前置采集机信息/采集端口1/端口速率
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/前置采集机信息/采集端口1/端口速率:STRING(64Kbps)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //前置采集机信息/采集端口2
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/前置采集机信息/采集端口2:STRING(采集端口2)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //前置采集机信息/采集端口2/端口名称
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/前置采集机信息/采集端口2/端口名称:STRING(端口名称)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //前置采集机信息/采集端口2/端口类型
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/前置采集机信息/采集端口2/端口类型:STRING(端口类型)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //前置采集机信息/采集端口2/端口速率
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/前置采集机信息/采集端口2/端口速率:STRING(64Kbps)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //网管系统接口信息
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/网管系统接口信息:STRING(网管系统接口信息)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //网管系统接口信息/接口机名称
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/网管系统接口信息/接口机名称:STRING(接口机名称)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //网管系统接口信息/网络地址
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/网管系统接口信息/网络地址:STRING(网络地址)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //网管系统接口信息/网络带宽
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/网管系统接口信息/网络带宽:STRING(网络带宽)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //帐务系统接口信息
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/帐务系统接口信息:STRING(帐务系统接口信息)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //帐务系统接口信息/接口机名称
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/帐务系统接口信息/接口机名称:STRING(接口机名称)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //帐务系统接口信息/网络地址
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/帐务系统接口信息/网络地址:STRING(网络地址)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //帐务系统接口信息/网络带宽
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/帐务系统接口信息/网络带宽:STRING(网络带宽)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //added by chenliangwei, 2003-07-08
            //在权限表中增加此任务的缺省权限
            MAP<STRING, int> user_vertor;
            user_vertor["admin"] = 0;
            m_UserRight[iter->first.c_str()] = user_vertor;
            //added end

            WriteCCSConfig();
        }
    }

    LIST<STRING> label;
    LIST<STRING> content;

    it = m_CfgInfo.begin();
    while (it != m_CfgInfo.end())
    {
        STRING& strCfgItem = *it;
        const char* pszTmp  = strCfgItem.c_str();
        const char* pszSep1 = strchr(pszTmp, ':');
        const char* pszSep2 = strchr(pszTmp, '/');
        if((pszSep2 != NULL) && (pszSep2 < pszSep1 - 1))
        {
            pszSep2++;
            const char* pszSep3 = strchr(pszSep2 + 1, '/');
            if(pszSep3 == NULL || pszSep3 > pszSep1)
            {
                pszSep3 = pszSep1;
            }

            int nStrLen = pszSep3 - pszSep2;
            char szCfgItem[2048];
            strncpy(szCfgItem, pszSep2, nStrLen);
            szCfgItem[nStrLen] = '\0';
            //printf("szCfgItem:%s\r\n", szCfgItem);

            //将用户名转换为小写
            char  szUserName[48];
            strncpy(szUserName, pMsg->UserName, sizeof(szUserName));
            szUserName[sizeof(szUserName) - 1] = '\0';
            StringToLower(szUserName);

            if(m_UserRight.find(szCfgItem) != m_UserRight.end())
            {
                MAP<STRING, int>& user_vertor = m_UserRight[szCfgItem];
                if(user_vertor.find(szUserName) != user_vertor.end())
                {
                    content.push_back(strCfgItem);
                }
            }
        }
        it++;
    }

    if(content.size() == 0)
    {
        content.push_back("-");
    }

    label.push_back("采集配置信息");
    Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, 0, "ListTaskInfo");
    Report.AddHoriTable(label, content);
    Report.AddString(STR_NEWLINE);
    Report.MakeReportTail();

    const char* pChar = Report.GetReport();
    SendMMLReport(pChar, pMsg->AppType, pMsg->ClientNo);
}






