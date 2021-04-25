#include "co_agent.h"
#include "../include/toolbox.h"
#include "../utility/mml_para.h"
#include "../utility/mml_report.h"

/**************************************************************************
����ԭ��    public: CCollectorAgent (void);
��������    ���캯��
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
CCollectorAgent::CCollectorAgent(void)
{
    m_lTimerID5Sec = 0;
    m_lSHKTimeOut  = 60;
}

/**************************************************************************
����ԭ��    public: ~CCollectorAgent ();
��������    ���캯��
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
CCollectorAgent::~CCollectorAgent()
{
    //
}

/**************************************************************************
����ԭ��    protected: void ExitInstance (void);
��������    �˳�����
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
void CCollectorAgent::ExitInstance (void)
{
    KillTimer(m_lTimerID5Sec);

    CWorkModule::ExitInstance();
}
/**************************************************************************
����ԭ��    protected: int InitInstance (void);
��������    ��ʼ��
�������    ��
�������    ��
����ֵ      ��ʼ���ɹ�ʱ����ERR_SUCCESS
�׳��쳣    ��
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
����ԭ��    protected: MODULE_TYPE GetModuleType ( void );
��������    ��ñ�ģ������
�������    ��
�������    ��
����ֵ      ���ر�ģ������
�׳��쳣    ��
***************************************************************************/
MODULE_TYPE CCollectorAgent::GetModuleType ( void )
{
    return MT_COLAGENT;
}

/**************************************************************************
����ԭ��    protected: void ParseMsg ( MSG_INTRA* pMsgParsed )
��������    ���������ַ�
�������    ��Ϣָ��
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
void CCollectorAgent::ParseMsg ( MSG_INTRA* pMsgParsed )
{
    CParseResult* pParseResult = NULL;
    const char* szColLabel;
    int nActive;

    switch(pMsgParsed->CmdCode)
    {
    case CMD_REPORT_COLLECTOR_STATUS:
        //�����ɼ�ģ���ϱ�״̬
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

        //��ѯ�����ɼ������״̬
        case MML_LST_COL_STATUS:
            if(m_ColToStatus.find(pParseResult->GetStringVal(1)) != m_ColToStatus.end())
            {
                //����ʧ�ܱ���
                CMMLReport Report("COLLECTOR");
                Report.MakeReportHead();
                Report.MakeCmdDisplay(pParseResult->GetCmdLine(), 0, 0, "COLLECTOR");
                LIST<STRING> title;
                title.push_back("Task");
                title.push_back("״̬");

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

        case MML_SET_CCSCFGINFO:        //���òɼ�������Ϣ
            AddCCSCfgItem(pMsgParsed);
            return;

        case MML_GET_CCSCFGINFO:        //��ȡ�ɼ�������Ϣ
            GetCCSCfgInfo(pMsgParsed);
            return;

        case MML_DEL_CCSCFGITEM:        //ɾ���ɼ�������Ϣ
            RemoveCCSCfgItem(pMsgParsed);
            return;

        case MML_MOD_CCSCFGINFO:        //�޸Ĳɼ�������Ϣ
            ModifyCCSCfgItem(pMsgParsed);
            return;

        default:
            break;
        }
        {
            //д��־
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

            //����ʧ�ܱ���
            CMMLReport Report("COLLECTOR");
            Report.MakeReportHead();
            Report.MakeCmdDisplay(pParseResult->GetCmdLine(), 0, 1, "COLLECTOR");
            Report.AddString("Ŀ����󲻴���");
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
����ԭ��        protected: void  OnTimer(const long nTimerID);
��������        ��ʱ������
�������        ʱ��ID ��ֻ����
�������        �ޡ�
����ֵ          �ޡ�
�׳��쳣        �ޡ�
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
����ԭ��        protected: void  Timer5Sec();
��������        5�붨ʱ������
�������        �ޡ�
�������        �ޡ�
����ֵ          �ޡ�
�׳��쳣        �ޡ�
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

    //ע�⣺�ɼ�����������Ϣ�ʼ���ܲ�������
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

    //�����û�Ȩ�ޱ�
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

                        //����Ա�ض��й��������Ȩ��
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
    //Ȼ�󱣴�������Ϣ
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

//ɾ��һ���ɼ�������Ϣ
void CCollectorAgent::RemoveCCSCfgItem(MSG_INTRA* pMsg)
{
    //��������Ϣ��ֱ��ǿ������ת��
    CParseResult *pParse = *(CParseResult **)pMsg->Data;
    char szCfgItem[2048];

    //д��־
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
        //�����û�Ȩ�޵ļ���
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

            //���û���ת��ΪСд
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
                    strRetMsg      = "ɾ���ɼ�����������Ϣʧ��:�û���Ȩ��"; //���췵�ر�������
                    bAuditSuccess = FALSE;
                }

            }
            else
            {
                log.OperResult = SLog::OPER_FAIL;
                strRetMsg      = "ɾ���ɼ�����������Ϣʧ��:ɾ����������Ϣָ���˲����ڵĲɼ�����"; //���췵�ر�������
                bAuditSuccess = FALSE;
            }

        }
        //added end

        if (bAuditSuccess)
        {
            //����szCfgItem�Ƿ�������'/'�����ֻ��һ��˵���ǲɼ�����
            const char* pszDiv = strchr(szCfgItem, '/');
            if((pszDiv != NULL) && (pszDiv = strchr(pszDiv + 1, '/')) == NULL)
            {
                log.OperResult = SLog::OPER_FAIL;
                strRetMsg      = "ɾ���ɼ�����������Ϣʧ��:����ɾ���ɼ�����";       //���췵�ر�������
            }
            else
            {
                LIST<STRING>::iterator it = m_CfgInfo.begin();
                while (it != m_CfgInfo.end())
                {
                    STRING& sCfgItem = *it;

                    //���ַ������ƥ��ķ����ҵ���Ӧ��������
                    if (strncmp(pszTmp, sCfgItem.c_str(), nStrLen) == 0)
                    {
                        m_CfgInfo.erase(it);
                        it = m_CfgInfo.begin();
                        continue;
                    }

                    it++;
                }

                //Ȼ�󱣴�������Ϣ
                if (WriteCCSConfig())
                {
                    log.OperResult = SLog::OPER_SUCCESS;
                    strRetMsg      = STR_OPRSUCCESS;    //���췵�ر�������
                    nRet = 0;
                }
                else
                {
                    log.OperResult = SLog::OPER_FAIL;
                    strRetMsg      = "ɾ���ɼ�����������Ϣʧ��:�ļ�����ʧ��";       //���췵�ر�������
                }
            }
        }
    }
    else
    {
        log.OperResult = SLog::OPER_FAIL;
        strRetMsg      = "ɾ���ɼ�����������Ϣʧ��:��Ϣ��ʽ���Ϸ�";           //���췵�ر�������
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
    //��������Ϣ��ֱ��ǿ������ת��
    CParseResult *pParse = *(CParseResult **)pMsg->Data;

    //д��־
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
    //����������Ƿ��Ѿ����ڣ�������ڣ�����Ϊ�ظ�
    //�˹��ܿ���Ϊ������Ϣ�걸�Լ��
    const char* pszTmp = pParse->GetStringVal(1);
    const char* pszSep = strchr(pszTmp, ':');
    char szCfgItem[2048];
    STRING strRetMsg = "��Ӳɼ�����������Ϣʧ��:��ʽ����ȷ";
    if((pszSep != NULL) && (pszSep - pszTmp < sizeof(szCfgItem) - 2))
    {
        //added by chenliangwei, 2003-07-08
        //�����û�Ȩ�޵ļ���
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

            //���û���ת��ΪСд
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
                    strRetMsg      = "��Ӳɼ�����������Ϣʧ��:�û���Ȩ��"; //���췵�ر�������
                    bAuditSuccess = FALSE;
                }

            }
            else
            {
                log.OperResult = SLog::OPER_FAIL;
                strRetMsg      = "��Ӳɼ�����������Ϣʧ��:���ӵ�������Ϣָ���˲����ڵĲɼ�����"; //���췵�ر�������
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
                //����szCfgItem�Ƿ�������'/'�����ֻ��һ��˵���ǲɼ�����
                {
                    BOOL bFound = FALSE;
                    LIST<STRING>::iterator it = m_CfgInfo.begin();
                    while (it != m_CfgInfo.end())
                    {
                        const char* pszCfgItem = it->c_str();

                        //���ַ������ƥ��ķ����ҵ���Ӧ��������
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
                        strRetMsg      = "��Ӳɼ�����������Ϣʧ��:������Ӳɼ�����";
                    }
                    else
                    {
                        m_CfgInfo.push_back(pszTmp);        //��ӵ�β��

                        //added by chenliangwei, 2003-07-08
                        UpdateUserRight();
                        //added end

                        if(WriteCCSConfig())
                        {
                            log.OperResult = SLog::OPER_SUCCESS;
                            strRetMsg      = STR_OPRSUCCESS;    //���췵�ر�������
                            nRet           = 0;
                        }
                        else
                        {
                            log.OperResult = SLog::OPER_FAIL;
                            strRetMsg      = "��Ӳɼ�����������Ϣʧ��:����������ʧ��";
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

                        //���ַ������ƥ��ķ����ҵ���Ӧ��������
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
                        strRetMsg      = "��Ӳɼ�����������Ϣʧ��:���ӵ�������Ϣָ���˲����ڵĲɼ�����";
                    }
                    else
                    {
                        LIST<STRING>::iterator it = m_CfgInfo.begin();
                        while (it != m_CfgInfo.end())
                        {
                            const char* pszCfgItem = it->c_str();

                            //���ַ������ƥ��ķ����ҵ���Ӧ��������
                            if (strncmp(szCfgItem, pszCfgItem, nStrLen) == 0)
                            {
                                m_CfgInfo.erase(it);
                                break;
                            }

                            it++;
                        }

                        m_CfgInfo.push_back(pszTmp);        //��ӵ�β��
                        if(WriteCCSConfig())
                        {
                            log.OperResult = SLog::OPER_SUCCESS;
                            strRetMsg      = STR_OPRSUCCESS;    //���췵�ر�������
                            nRet           = 0;
                        }
                        else
                        {
                            log.OperResult = SLog::OPER_FAIL;
                            strRetMsg      = "��Ӳɼ�����������Ϣʧ��:����������ʧ��";
                        }
                    }
                }
            }
        }
    }

    WriteLog(&log);

    Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, nRet, THISMODULE);
    Report.AddString(strRetMsg.c_str());        //���췵�ر�������
    Report.AddString(STR_NEWLINE);
    Report.MakeReportTail();

    const char* pChar = Report.GetReport();
    SendMMLReport(pChar, pMsg->AppType, pMsg->ClientNo);
}

void CCollectorAgent::ModifyCCSCfgItem(MSG_INTRA* pMsg)
{
    //��������Ϣ��ֱ��ǿ������ת��
    CParseResult *pParse = *(CParseResult **)pMsg->Data;

    //д��־
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
    //����������Ƿ��Ѿ����ڣ�������ڣ�����Ϊ�ظ�
    //�˹��ܿ���Ϊ������Ϣ�걸�Լ��
    const char* pszSrc = pParse->GetStringVal(1);
    const char* pszDst = pParse->GetStringVal(2);
    const char* pszSep = strchr(pszSrc, ':');
    char szCfgItem[2048];
    STRING strRetMsg = STR_OPRFAIL;
    if((pszSep != NULL) && (pszSep - pszSrc < sizeof(szCfgItem) - 2))
    {

        //added by chenliangwei, 2003-07-08
        //�����û�Ȩ�޵ļ���
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

            //���û���ת��ΪСд
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
                    strRetMsg      = "�޸Ĳɼ�����������Ϣʧ��:�û���Ȩ��"; //���췵�ر�������
                    bAuditSuccess = FALSE;
                }

            }
            else
            {
                log.OperResult = SLog::OPER_FAIL;
                strRetMsg      = "�޸Ĳɼ�����������Ϣʧ��:�Ҳ���ƥ���������"; //���췵�ر�������
                bAuditSuccess = FALSE;
            }

        }
        //added end

        if (bAuditSuccess)
        {
            int nStrLen = pszSep - pszSrc + 1;
            strncpy(szCfgItem, pszSrc, nStrLen);
            szCfgItem[nStrLen] = '\0';
            //��ʱszCfgItem����������Ϊ"Huawei CCS/CC08/��������Ϣ:"

            //����szCfgItem�Ƿ�������'/'�����ֻ��һ��˵���ǲɼ�����
            BOOL bTask = FALSE;
            const char* pszDiv = strchr(szCfgItem, '/');
            if((pszDiv != NULL)
               && ((pszDiv = strchr(pszDiv + 1, '/')) == NULL))
            {
                bTask = TRUE;
            }

            if ((bTask) && (strncmp(szCfgItem, pszDst, nStrLen) != 0))
            {
                //����޸��˲ɼ����������򷵻�ʧ��
                log.OperResult = SLog::OPER_FAIL;
                strRetMsg      = "�޸Ĳɼ�����������Ϣʧ��:�����޸Ĳɼ����������";
            }
            else
            {
                //�Ӻ����ҵ�һ��'/'
                pszDiv = strrchr(szCfgItem, '/');
                if(pszDiv != NULL)
                {
                    int nPerSecLen = pszDiv - szCfgItem;
                    if(strncmp(szCfgItem, pszDst, nPerSecLen) == 0)
                    //�����޸�ǰ���޸ĺ���ͬһ�����ڵ���
                    {
                        log.OperResult = SLog::OPER_FAIL;
                        strRetMsg      = "�޸Ĳɼ�����������Ϣʧ��:�Ҳ���ƥ���������"; //���췵�ر�������
                        bool bFound = false;
                        if(strncmp(szCfgItem, pszDst, nStrLen) == 0)
                        //������޸Ľڵ���
                        {
                            LIST<STRING>::iterator it = m_CfgInfo.begin();
                            while (it != m_CfgInfo.end())
                            {
                                const char* pszCfgItem = it->c_str();

                                //���ַ������ƥ��ķ����ҵ���Ӧ��������
                                if (strncmp(szCfgItem, pszCfgItem, nStrLen) == 0)
                                {
                                    m_CfgInfo.erase(it);
                                    m_CfgInfo.push_back(pszDst);        //��ӵ�β��
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
                        //����޸��˽ڵ���
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
                                    //�ҵ��ýڵ���
                                    {
                                        strCfgItem = pszDst;
                                        bFound = true;
                                    }
                                    else if (strncmp(szCfgItem, strCfgItem.c_str(), nStrLen - 1) == 0)
                                    //�ýڵ������
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
                            //����������Ϣ
                            if(WriteCCSConfig())
                            {
                                log.OperResult = SLog::OPER_SUCCESS;
                                strRetMsg      = STR_OPRSUCCESS;    //���췵�ر�������
                                nRet           = 0;
                            }
                            else
                            {
                                log.OperResult = SLog::OPER_FAIL;
                                strRetMsg      = "�޸Ĳɼ�����������Ϣʧ��:����������ʧ��"; //���췵�ر�������
                            }
                        }
                    }
                    else
                    {
                        log.OperResult = SLog::OPER_FAIL;
                        strRetMsg      = "�޸Ĳɼ�����������Ϣʧ��:����ͬʱ�޸������ڵ������";
                    }
                }
                else
                {
                    log.OperResult = SLog::OPER_FAIL;
                    strRetMsg      = "�޸Ĳɼ�����������Ϣʧ��:��ʽ����ȷ";
                }
            }
        }
    }

    WriteLog(&log);

    Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, nRet, THISMODULE);
    Report.AddString(strRetMsg.c_str());        //���췵�ر�������
    Report.AddString(STR_NEWLINE);
    Report.MakeReportTail();

    const char* pChar = Report.GetReport();
    SendMMLReport(pChar, pMsg->AppType, pMsg->ClientNo);
}

void CCollectorAgent::GetCCSCfgInfo(MSG_INTRA* pMsg)
{
    CParseResult *pParse = *(CParseResult **)pMsg->Data;

    //д��־
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
    Report.MakeReportHead();        //���챨��ͷ
    LIST<STRING>::iterator it;

    //Ϊû��������Ϣ�Ĳɼ���������ȱʡ��
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
            //�ɼ�������
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s:LIST()",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //��������Ϣ
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/��������Ϣ:STRING(��������Ϣ)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //��������Ϣ/����������
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/��������Ϣ/����������:STRING(����������)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //��������Ϣ/�������汾
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/��������Ϣ/����������:STRING(�������汾)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //��������Ϣ/����������
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/��������Ϣ/����������:STRING(����������)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //��������Ϣ/���������
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/��������Ϣ/���������:STRING(���������)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //��������Ϣ/�����
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/��������Ϣ/�����:UINT(0755)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //��������Ϣ/�����ɼ���
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/��������Ϣ/�����ɼ���:STRING(�����ɼ���)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //��������Ϣ/�ɼ��˿���
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/��������Ϣ/�ɼ��˿���:UINT(2)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //��������Ϣ/�ɼ��˿�1
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/��������Ϣ/�ɼ��˿�1:STRING(�ɼ��˿�1)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //��������Ϣ/�ɼ��˿�1/�˿�����
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/��������Ϣ/�ɼ��˿�1/�˿�����:STRING(�˿�����)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //��������Ϣ/�ɼ��˿�1/�˿�����
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/��������Ϣ/�ɼ��˿�1/�˿�����:STRING(�˿�����)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //��������Ϣ/�ɼ��˿�1/�˿�����
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/��������Ϣ/�ɼ��˿�1/�˿�����:STRING(64Kbps)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //��������Ϣ/�ɼ��˿�2
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/��������Ϣ/�ɼ��˿�2:STRING(�ɼ��˿�2)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //��������Ϣ/�ɼ��˿�2/�˿�����
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/��������Ϣ/�ɼ��˿�2/�˿�����:STRING(�˿�����)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //��������Ϣ/�ɼ��˿�2/�˿�����
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/��������Ϣ/�ɼ��˿�2/�˿�����:STRING(�˿�����)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //��������Ϣ/�ɼ��˿�2/�˿�����
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/��������Ϣ/�ɼ��˿�2/�˿�����:STRING(64Kbps)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //ǰ�òɼ�����Ϣ
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/ǰ�òɼ�����Ϣ:STRING(ǰ�òɼ�����Ϣ)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //ǰ�òɼ�����Ϣ/�ɼ�������
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/ǰ�òɼ�����Ϣ/�ɼ�������:STRING(�ɼ�������)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //ǰ�òɼ�����Ϣ/�ɼ��˿���
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/ǰ�òɼ�����Ϣ/�ɼ��˿���:UINT(2)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //ǰ�òɼ�����Ϣ/�ɼ��˿�1
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/ǰ�òɼ�����Ϣ/�ɼ��˿�1:STRING(�ɼ��˿�1)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //ǰ�òɼ�����Ϣ/�ɼ��˿�1/�˿�����
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/ǰ�òɼ�����Ϣ/�ɼ��˿�1/�˿�����:STRING(�˿�����)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //ǰ�òɼ�����Ϣ/�ɼ��˿�1/�˿�����
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/ǰ�òɼ�����Ϣ/�ɼ��˿�1/�˿�����:STRING(�˿�����)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //ǰ�òɼ�����Ϣ/�ɼ��˿�1/�˿�����
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/ǰ�òɼ�����Ϣ/�ɼ��˿�1/�˿�����:STRING(64Kbps)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //ǰ�òɼ�����Ϣ/�ɼ��˿�2
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/ǰ�òɼ�����Ϣ/�ɼ��˿�2:STRING(�ɼ��˿�2)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //ǰ�òɼ�����Ϣ/�ɼ��˿�2/�˿�����
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/ǰ�òɼ�����Ϣ/�ɼ��˿�2/�˿�����:STRING(�˿�����)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //ǰ�òɼ�����Ϣ/�ɼ��˿�2/�˿�����
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/ǰ�òɼ�����Ϣ/�ɼ��˿�2/�˿�����:STRING(�˿�����)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //ǰ�òɼ�����Ϣ/�ɼ��˿�2/�˿�����
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/ǰ�òɼ�����Ϣ/�ɼ��˿�2/�˿�����:STRING(64Kbps)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //����ϵͳ�ӿ���Ϣ
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/����ϵͳ�ӿ���Ϣ:STRING(����ϵͳ�ӿ���Ϣ)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //����ϵͳ�ӿ���Ϣ/�ӿڻ�����
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/����ϵͳ�ӿ���Ϣ/�ӿڻ�����:STRING(�ӿڻ�����)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //����ϵͳ�ӿ���Ϣ/�����ַ
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/����ϵͳ�ӿ���Ϣ/�����ַ:STRING(�����ַ)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //����ϵͳ�ӿ���Ϣ/�������
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/����ϵͳ�ӿ���Ϣ/�������:STRING(�������)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //����ϵͳ�ӿ���Ϣ
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/����ϵͳ�ӿ���Ϣ:STRING(����ϵͳ�ӿ���Ϣ)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //����ϵͳ�ӿ���Ϣ/�ӿڻ�����
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/����ϵͳ�ӿ���Ϣ/�ӿڻ�����:STRING(�ӿڻ�����)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //����ϵͳ�ӿ���Ϣ/�����ַ
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/����ϵͳ�ӿ���Ϣ/�����ַ:STRING(�����ַ)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //����ϵͳ�ӿ���Ϣ/�������
            SNPRINTF(szCfgItem,
                     sizeof(szCfgItem),
                     "Huawei CCS/%s/����ϵͳ�ӿ���Ϣ/�������:STRING(�������)",
                     iter->first.c_str());
            szCfgItem[sizeof(szCfgItem) - 1] = '\0';
            m_CfgInfo.push_back(szCfgItem);

            //added by chenliangwei, 2003-07-08
            //��Ȩ�ޱ������Ӵ������ȱʡȨ��
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

            //���û���ת��ΪСд
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

    label.push_back("�ɼ�������Ϣ");
    Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, 0, "ListTaskInfo");
    Report.AddHoriTable(label, content);
    Report.AddString(STR_NEWLINE);
    Report.MakeReportTail();

    const char* pChar = Report.GetReport();
    SendMMLReport(pChar, pMsg->AppType, pMsg->ClientNo);
}






