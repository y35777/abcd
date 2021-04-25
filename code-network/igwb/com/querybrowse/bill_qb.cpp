#include "bill_qb.h"

CBillQBServer::CBillQBServer()
{
    m_pFormatObj      = NULL;
    m_pIQueryBrowse   = NULL;
    m_pMMLReport      = NULL;
    m_uPageSize       = 0;
    m_bDisplayErrBill = FALSE;
}

CBillQBServer::~CBillQBServer()
{
    //
}

/**********************************************************************
��������    ��ʼ������״̬�ͳ�Ա����
�������    ��
�������    ��
����ֵ      0----�ɹ�
            1----ʧ��
            <0----ʧ�ܣ�����ֵ�ľ���ֵΪ������
�׳��쳣    ��
**********************************************************************/
int CBillQBServer::InitInstance()
{
    int nResult = CWorkModule::InitInstance();
    if (ERR_SUCCESS != nResult)
    {
        TRACE(MTS_BILLBROWSER, S_QB_TRC_FAIL_INIT_BASE);
        return ERR_FAIL;
    }

    //��ʼ����ʽ����ض���
    char szIGWBPath[MAX_PATH];
    sprintf(szIGWBPath, "%s/%s/%s", GetAppPath(),
            "config", "format");
    m_pFormatObj = new CIGWB(szIGWBPath);
    m_pIQueryBrowse
        = (IQueryBrowse* )m_pFormatObj->QueryInterface(IID_FP_IQUERYBROWSE);
    if (NULL == m_pIQueryBrowse)
    {
        TRACE(MTS_BILLBROWSER, S_QB_TRC_FAIL_INIT_QB_FMT);
        return ERR_FAIL;
    }
    nResult = m_pIQueryBrowse->Init(0);
    if (0 != nResult)
    {
        TRACE(MTS_BILLBROWSER, S_QB_TRC_FAIL_INIT_QB_FMT);
        return ERR_FAIL;
    }

    //����ǰ�����·��
    CINIFile cfgIniFile(GetCfgFilePath());
    BOOL bSucc = cfgIniFile.Open();
    if (!bSucc)
    {
        TRACE(MTS_BILLBROWSER, S_QB_TRC_FAIL_OPEN_INI);
        return ERR_FAIL;
    }

    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_FRONT_SAVE_ROOT_DIR,
                         CFG_FRONT_SAVE_ROOT_DIR_DEFAULT,
                         m_szOrigBasePath, MAX_PATH);
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_BACK_SAVE_ROOT_DIR,
                         CFG_BACK_SAVE_ROOT_DIR_DEFAULT,
                         m_szFinalBasePath, MAX_PATH);
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_QB_TMP_RESULT_DIR,
                         CFG_QB_TMP_RESULT_DEFAULT,
                         m_szQBTmpResultPath, MAX_PATH);
    

    //Added by MMS on 2002.11.21
    //���Ӷ���ɼ��ĸ�·��
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_COL_ROOT_DIR,
                         CFG_COL_ROOT_DIR_DEFAULT,
                         m_szCollectRootPath, MAX_PATH);
    //End


    //�����б�m_APInfoList
    int nAPCount = cfgIniFile.GetInt(CFG_SEC_COMMON, CFG_COMMON_KEY_APCOUNT,
                                     CFG_COMMON_KEY_APCOUNT_DEFAULT);
    char szAPName[255], szAPSection[35];
    for (int nAPNo = 1; nAPNo <= nAPCount; nAPNo++)
    {
        sprintf(szAPSection, "%s%d", CFG_SEC_AP, nAPNo);
        cfgIniFile.GetString(szAPSection, CFG_AP_NAME, szAPSection,
                             szAPName, 255);
        int nAPType = cfgIniFile.GetInt(szAPSection, CFG_AP_KEY_APTYPE,
                                        CFG_AP_KEY_APTYPE_DEFAULT);

        //��CollectType����0,��ʾ�ǲɼ������
        int nCollectType = cfgIniFile.GetInt(szAPSection, CFG_AP_KEY_COLTYPE,
                                        CFG_AP_KEY_COLTYPE_DEFAULT);
        if (nCollectType)
        {
            nAPType = AP_OTHER + nCollectType;
        }

        BOOL bMustMerge = cfgIniFile.GetInt(szAPSection, CFG_AP_KEY_MUST_MERGE,
                                            CFG_AP_KEY_MUST_MERGE_DEFAULT);

        BOOL bFinalSPFileFmt = cfgIniFile.GetInt(szAPSection, CFG_AP_KEY_FINAL_SPFILEFMT,
                                                 CFG_AP_KEY_FINAL_SPFILEFMT_DEFAULT);

        SAPInfo* pAPInfo = new SAPInfo;
        STRNCPY(pAPInfo->szAPName, szAPName, 255);
        pAPInfo->nAPType = nAPType;
        pAPInfo->nAPID   = nAPNo;
        pAPInfo->bMustMerge = bMustMerge;
        pAPInfo->bFinalSPFileFmt = bFinalSPFileFmt;
        m_APInfoList.push_back(pAPInfo);
    }

    //�������ļ��ж�ȡ�Ͳ�ѯ���������صĲ���
    UINT4 uPageSize = cfgIniFile.GetInt(CFG_SEC_OM, CFG_PAGE_SIZE,
                                        CFG_PAGE_SIZE_DEFAULT);
    if ((uPageSize > 500) || (0 == uPageSize))
    {
        TRACE(MTS_BILLBROWSER, S_QB_TRC_ERR_CFG_PAGE_SIZE, uPageSize);
        uPageSize = CFG_PAGE_SIZE_DEFAULT;
    }
    CBillQBTask::SetPageSize(uPageSize);
    m_uPageSize = uPageSize;

    m_bDisplayErrBill = cfgIniFile.GetInt(CFG_SEC_OM,
                                          CFG_DISPLAY_ERR_BILL,
                                          CFG_DISPLAY_ERR_BILL_DEFAULT);

    m_uTimeOutLimit = 60;
    m_lBaseTimer = SetTimer(1000);
    m_pMMLReport = new CQBMMLReport();

    return ERR_SUCCESS;
}

/**********************************************************************
��������    ��������������
�������    ��
�������    ��
**********************************************************************/
void CBillQBServer::ExitInstance()
{
    CWorkModule::ExitInstance();

    //ֹͣ��ɾ�����еĲ�ѯ�����������������б�
    LIST<CBillQBTask* >::iterator i;
    for (i = m_qBrowseTask.begin(); i != m_qBrowseTask.end(); i++)
    {
        DestroyTask(*i);
    }
    m_qBrowseTask.clear();
    for (i = m_qQueryTask.begin(); i != m_qQueryTask.end(); i++)
    {
        DestroyTask(*i);
    }
    m_qQueryTask.clear();

    LIST<SAPInfo* >::iterator j;
    for (j = m_APInfoList.begin(); j != m_APInfoList.end(); j++)
    {
        delete *j;
    }
    m_APInfoList.clear();

    if (NULL != m_pFormatObj)
    {
        delete m_pFormatObj;
    }
    if (NULL != m_pMMLReport)
    {
        delete m_pMMLReport;
    }
}

//��ȡ��ģ�����ͺ���
MODULE_TYPE CBillQBServer::GetModuleType()
{
    return MT_BILLBROWSER;
}

/**********************************************************************
��������    ��ʱ���¼��Ĵ�����ں���
�������    nTimerID----��ʱ��ID
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::OnTimer(const long nTimerID)
{
    if (nTimerID == m_lBaseTimer)
    {
        //�������еĲ�ѯ�������ĳ�ʱ
        LIST<CBillQBTask* >::iterator i;
        i = m_qBrowseTask.begin();
        while (i != m_qBrowseTask.end())
        {
            (*i)->IncTimeOutFlg();
            if ((*i)->IsTimeOut(m_uTimeOutLimit))
            {
                DestroyTask(*i);
                m_qBrowseTask.erase(i);

                i = m_qBrowseTask.begin();
            }
            else
            {
                i++;
            }
        }
        i = m_qQueryTask.begin();
        while (i != m_qQueryTask.end())
        {
            (*i)->IncTimeOutFlg();
            if ((*i)->IsTimeOut(m_uTimeOutLimit))
            {
                DestroyTask(*i);
                m_qQueryTask.erase(i);

                i = m_qQueryTask.begin();
            }
            else
            {
                i++;
            }
        }
    }
    else
    {
        CWorkModule::OnTimer(nTimerID);
    }
}

/**********************************************************************
��������    ��������ͷַ�
�������    pMsgParsed----������Ϣ֡�����ú���������ɾ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::ParseMsg(MSG_INTRA* pMsgParsed)
{
    if (CMD_MML == pMsgParsed->CmdCode)
    {
        CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
        m_nCurClientNo = pMsgParsed->ClientNo;
/*
        //��������������������صĴ�����Ϣ����
        if (0 != strlen(pParseResult->GetErrMsg()))
        {
            SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                                 pParseResult->GetErrMsg());
        }
        else
*/

        switch (pParseResult->GetCmdCode())
        {
            case MML_LST_AP:
                ReqAccessPointProc(pMsgParsed);
                break;
            case MML_LST_BILLTYPE:
                ReqBillTypeProc(pMsgParsed);
                break;
            case MML_LST_CHL:
                ReqChannelProc(pMsgParsed);
                break;
            case MML_LST_ORGDATE:
                ReqOriginalDateProc(pMsgParsed);
                break;
            case MML_LST_FINALDATE:
                ReqFinalDateProc(pMsgParsed);
                break;
            case MML_LST_FILES:
                ReqBillFilesProc(pMsgParsed);
                break;
            case MML_LST_FMT:
                ReqBillFormatProc(pMsgParsed);
                break;
            case MML_LST_QUERYPARA:
                ReqQueryCondProc(pMsgParsed);
                break;
            case MML_BRW_BILL:
                ReqBrowseProc(pMsgParsed);
                break;
            case MML_LST_BILL:
                ReqQueryProc(pMsgParsed);
                break;
            case MML_GET_PROGRESS:
                ReqTaskProgressProc(pMsgParsed);
                break;
            case MML_GET_PAGE_DATA:
                ReqGetPageDataProc(pMsgParsed);
                break;
            case MML_STP_TASK:
                ReqStopTaskProc(pMsgParsed);
                break;
            case MML_CLS_TASK:
                ReqCloseTaskProc(pMsgParsed);
                break;
            case MML_SHK_WND:
                ReqWndShakeHandsProc(pMsgParsed);
                break;

            //Added by MMS on 2002.11.21
            //���ӶԲɼ��������������
            case MML_LST_COLROOT:
                ReqFileRoot(pMsgParsed);
                break;
            case MML_LST_COLFILE:
                ReqBillFiles(pMsgParsed);
                break;
            //End

            default:
                CWorkModule::ParseMsg(pMsgParsed);
                TRACE(MTS_BILLBROWSER, S_QB_TRC_REC_ERR_CMD,
                      pParseResult->GetCmdCode());
                break;
        }

        //������MSG_INTRA�д�ŵ�ֻ��CParseResult��ָ�룬
        //���Ա����ڴ��ͷ�CParseResult����
        delete pParseResult;
    }
}

/**********************************************************************
��������    ����������Ϣ�Ĵ�����ں���
�������    pMsgParsed----������Ϣ֡�����ú���������ɾ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::ReqAccessPointProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    //��ȡ���н����������Ϣ
    LIST<STRING> APNameListTmp;
    m_pIQueryBrowse->GetAPNames(APNameListTmp);

    //���첢����MML����
    LIST<STRING> TitleList;
    TitleList.push_back(S_QB_PRODUCT_ANME);
    TitleList.push_back(S_QB_PRODUCT_INDEX);

    LIST<STRING> APNameList;
    LIST<STRING>::iterator iter;
    int index = 1;
    char szIndex[10];
    
    for (iter = APNameListTmp.begin(); iter != APNameListTmp.end(); iter++)
    {
        sprintf(szIndex, "%d", index);
        APNameList.push_back(*iter);
        APNameList.push_back(szIndex);
        index ++;
    }

    SendHoriTableReport(pParseResult, TitleList, APNameList,
                        S_QB_TH_LIST_PRODUCT_NAME);
}

/**********************************************************************
��������    ���󻰵����͵Ĵ�����ں���
�������    pMsgParsed----������Ϣ֡�����ú���������ɾ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::ReqBillTypeProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    char* szAPName = (char* )pParseResult->GetStringVal(1);

    //���첢����MML����

    //��ȡ�����͵ı��ı��������
    LIST<STRING> TitleList;
    TitleList.push_back(S_QB_TT_BILL_TYPE);
    TitleList.push_back(S_QB_TT_AP_NAME);
    TitleList.push_back(S_QB_TT_TYPE);
    TitleList.push_back(S_QB_TT_SIZE);
    TitleList.push_back(S_QB_TT_MTIME);
    TitleList.push_back(S_QB_TT_CTIME);

    //���ݽ��������ֱ�ӻ�ȡ��������(0��1)����д�����������
    LIST<STRING> ContentList;
    ContentList.push_back("0");
    ContentList.push_back(szAPName);
    ContentList.push_back("-");
    ContentList.push_back("-");
    ContentList.push_back("-");
    ContentList.push_back("-");

    ContentList.push_back("1");
    ContentList.push_back(szAPName);
    ContentList.push_back("-");
    ContentList.push_back("-");
    ContentList.push_back("-");
    ContentList.push_back("-");

    SendHoriTableReport(pParseResult, TitleList, ContentList,
                        S_QB_TH_LIST_BILL_TYPE);
}

/**********************************************************************
��������    ����ͨ����Ϣ�Ĵ�����ں���
�������    pMsgParsed----������Ϣ֡�����ú���������ɾ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::ReqChannelProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    //����MML�����еĽ�������õ�ͨ����
    char* szAPName = (char* )pParseResult->GetStringVal(1);
    LIST<STRING> ChlList;
    m_pIQueryBrowse->GetChlNames(szAPName, ChlList);

    //���첢����MML����

    //��ȡ�����͵ı��ı��������
    LIST<STRING> TitleList;
    TitleList.push_back(S_QB_TT_BILL_TYPE);
    TitleList.push_back(S_QB_TT_CHANNEL_NO);
    TitleList.push_back(S_QB_TT_CHANNEL_NAME);
    TitleList.push_back(S_QB_TT_PRODUCT_NAME);
    TitleList.push_back(S_QB_TT_TYPE);
    TitleList.push_back(S_QB_TT_SIZE);
    TitleList.push_back(S_QB_TT_MTIME);
    TitleList.push_back(S_QB_TT_CTIME);

    LIST<STRING> ContentList;
    int nChlNo = 1;
    char szBillType[10], szChlNo[10];
    sprintf(szBillType, "%d", 1);
    LIST<STRING>::iterator iChlIdx;
    for (iChlIdx = ChlList.begin(); iChlIdx != ChlList.end(); iChlIdx++)
    {
        char szFilePath[MAX_PATH];

        sprintf(szFilePath, "%s/%s/%s", m_szFinalBasePath, szAPName, (*iChlIdx).c_str());
        ACE_stat FileStat;
        memset(&FileStat, 0, sizeof(FileStat));
        int nTmpRet = ACE_OS::stat(szFilePath, &FileStat);
        if(0 != nTmpRet)
        {
            CreateDeepDir(szFilePath);
            ACE_OS::stat(szFilePath, &FileStat);
        }

        sprintf(szChlNo, "%d", nChlNo);
        nChlNo++;

        ContentList.push_back(szBillType);
        ContentList.push_back(szChlNo);
        ContentList.push_back(*iChlIdx);
        ContentList.push_back(szAPName);
        ContentList.push_back(S_FILE_TYPE_FOLDER);          //Ŀ¼

        char szTemp[128];
        tm* pTimeTm = NULL;

        sprintf(szTemp, "%d", FileStat.st_size);
        ContentList.push_back(szTemp);

        pTimeTm = localtime(&(FileStat.st_mtime));
        sprintf(szTemp, "%04d-%02d-%02d %02d:%02d:%02d",
                pTimeTm->tm_year + 1900, pTimeTm->tm_mon + 1, pTimeTm->tm_mday,
                pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);
        ContentList.push_back(szTemp);

        pTimeTm = localtime(&(FileStat.st_ctime));
        sprintf(szTemp, "%04d-%02d-%02d %02d:%02d:%02d",
                pTimeTm->tm_year + 1900, pTimeTm->tm_mon + 1, pTimeTm->tm_mday,
                pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);
        ContentList.push_back(szTemp);
    }

    SendHoriTableReport(pParseResult, TitleList, ContentList,
                        S_QB_TH_LIST_CHL);
}

/**********************************************************************
��������    ����ԭʼ��������Ŀ¼������ں���
�������    pMsgParsed----������Ϣ֡�����ú���������ɾ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::ReqOriginalDateProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    //����MML�����еĽ�������õ�ԭʼ����������Ŀ¼�б�
    char* szAPName = (char* )pParseResult->GetStringVal(1);
    LIST<STRING> OrigDateList;
    char szPath[MAX_PATH];
    sprintf(szPath, "%s/%s", m_szOrigBasePath, szAPName);
    GetDateDir(szPath, OrigDateList);

    //���첢����MML����

    //��ȡ�����͵ı��ı��������
    LIST<STRING> TitleList;
    TitleList.push_back(S_QB_TT_ORIG_DATE_NO);
    TitleList.push_back(S_QB_TT_ORIG_DATE_NAME);
    TitleList.push_back(S_QB_TT_BILL_TYPE);
    TitleList.push_back(S_QB_TT_PRODUCT_NAME);
    TitleList.push_back(S_QB_TT_TYPE);
    TitleList.push_back(S_QB_TT_SIZE);
    TitleList.push_back(S_QB_TT_MTIME);
    TitleList.push_back(S_QB_TT_CTIME);

    LIST<STRING> ContentList;
    char szBillType[10];    //�������ͣ��ڴ˱�����һֱΪ0��ԭʼ������
    sprintf(szBillType, "%d", 0);
    LIST<STRING>::iterator i;
    int nDateNo = 1;
    char szDateNo[10];    //����Ŀ¼�����
    for (i = OrigDateList.begin(); i != OrigDateList.end(); i++)
    {
        sprintf(szDateNo, "%d", nDateNo);
        nDateNo++;

        ContentList.push_back(szDateNo);
        ContentList.push_back(*i);
        ContentList.push_back(szBillType);
        ContentList.push_back(szAPName);
        ContentList.push_back(S_FILE_TYPE_FOLDER);          //Ŀ¼

        char szFilePath[MAX_PATH];

        sprintf(szFilePath, "%s/%s/%s", m_szOrigBasePath, szAPName, (*i).c_str());
        ACE_stat FileStat;
        memset(&FileStat, 0, sizeof(FileStat));
        int nTmpRet = ACE_OS::stat(szFilePath, &FileStat);
        if(0 != nTmpRet)
        {
            CreateDeepDir(szFilePath);
            ACE_OS::stat(szFilePath, &FileStat);
        }

        char szTemp[128];
        tm* pTimeTm = NULL;

        sprintf(szTemp, "%d", FileStat.st_size);
        ContentList.push_back(szTemp);

        pTimeTm = localtime(&(FileStat.st_mtime));
        sprintf(szTemp, "%04d-%02d-%02d %02d:%02d:%02d",
                pTimeTm->tm_year + 1900, pTimeTm->tm_mon + 1, pTimeTm->tm_mday,
                pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);
        ContentList.push_back(szTemp);

        pTimeTm = localtime(&(FileStat.st_ctime));
        sprintf(szTemp, "%04d-%02d-%02d %02d:%02d:%02d",
                pTimeTm->tm_year + 1900, pTimeTm->tm_mon + 1, pTimeTm->tm_mday,
                pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);
        ContentList.push_back(szTemp);
    }

    SendHoriTableReport(pParseResult, TitleList, ContentList,
                        S_QB_TH_LIST_ORIG_DATE);
}

/**********************************************************************
��������    �������ջ�������Ŀ¼������ں���
�������    pMsgParsed----������Ϣ֡�����ú���������ɾ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::ReqFinalDateProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    //����MML�����еĽ�������õ�ͨ�����б�
    char* szAPName = (char* )pParseResult->GetStringVal(1);
    LIST<STRING> ChlList, ChlDateList, ContentList;
    m_pIQueryBrowse->GetChlNames(szAPName, ChlList);

    //���첢����MML����

    //��ȡ�����͵ı��ı��������
    LIST<STRING> TitleList;
    TitleList.push_back(S_QB_TT_CHL_DATE_NO);
    TitleList.push_back(S_QB_TT_CHL_DATE_NAME);
    TitleList.push_back(S_QB_TT_BILL_TYPE);
    TitleList.push_back(S_QB_TT_CHANNEL_NO);
    TitleList.push_back(S_QB_TT_CHANNEL_NAME);
    TitleList.push_back(S_QB_TT_PRODUCT_NAME);
    TitleList.push_back(S_QB_TT_TYPE);
    TitleList.push_back(S_QB_TT_SIZE);
    TitleList.push_back(S_QB_TT_MTIME);
    TitleList.push_back(S_QB_TT_CTIME);

    char szBillType[10];    //�������ͣ��ڴ˱�����һֱΪ1�����ջ�����
    int nDateCount, nDateNo, nChlNo = 1;
    char szChlNo[10], szChlDir[MAX_PATH], szDateNo[10];
    sprintf(szBillType, "%d", 1);
    LIST<STRING>::iterator iChlIdx, iDateIdx;
    for (iChlIdx = ChlList.begin(); iChlIdx != ChlList.end(); iChlIdx++)
    {
        sprintf(szChlNo, "%d", nChlNo);
        nChlNo++;

        //���ݲ�Ʒ����ͨ�����õ�ͨ��·������ø�·���µ�����Ŀ¼
        ChlDateList.clear();
        sprintf(szChlDir, "%s/%s/%s", m_szFinalBasePath,
                szAPName, (*iChlIdx).c_str());
        nDateCount = GetDateDir(szChlDir, ChlDateList);

        nDateNo = 1;

        //����Ŀ¼�����б���д��صı������
        iDateIdx = ChlDateList.begin();
        for (; iDateIdx != ChlDateList.end();  iDateIdx++)
        {
            sprintf(szDateNo, "%d", nDateNo);
            nDateNo++;

            ContentList.push_back(szDateNo);
            ContentList.push_back(*iDateIdx);
            ContentList.push_back(szBillType);
            ContentList.push_back(szChlNo);
            ContentList.push_back(*iChlIdx);
            ContentList.push_back(szAPName);

            char szFilePath[MAX_PATH];

            sprintf(szFilePath, "%s/%s/%s/%s", m_szFinalBasePath, szAPName,
                    (*iChlIdx).c_str(), (*iDateIdx).c_str());
            ACE_stat FileStat;
            memset(&FileStat, 0, sizeof(FileStat));
            int nTmpRet = ACE_OS::stat(szFilePath, &FileStat);
            if(0 != nTmpRet)
            {
                CreateDeepDir(szFilePath);
                ACE_OS::stat(szFilePath, &FileStat);
            }

            ContentList.push_back(S_FILE_TYPE_FOLDER);          //Ŀ¼

            char szTemp[128];
            tm* pTimeTm = NULL;

            sprintf(szTemp, "%d", FileStat.st_size);
            ContentList.push_back(szTemp);

            pTimeTm = localtime(&(FileStat.st_mtime));
            sprintf(szTemp, "%04d-%02d-%02d %02d:%02d:%02d",
                pTimeTm->tm_year + 1900, pTimeTm->tm_mon + 1, pTimeTm->tm_mday,
                pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);
            ContentList.push_back(szTemp);

            pTimeTm = localtime(&(FileStat.st_ctime));
            sprintf(szTemp, "%04d-%02d-%02d %02d:%02d:%02d",
                pTimeTm->tm_year + 1900, pTimeTm->tm_mon + 1, pTimeTm->tm_mday,
                pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);
            ContentList.push_back(szTemp);
        }
    }

    SendHoriTableReport(pParseResult, TitleList, ContentList,
                        S_QB_TH_LIST_FINAL_DATE);
}

/**********************************************************************
��������    ���󻰵��ļ��б�����ں���
�������    pMsgParsed----������Ϣ֡�����ú���������ɾ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::ReqBillFilesProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    //��MML�����еõ������������ݴ˻�û����ļ��б�
    int nBillType;
    char* szAPName = (char* )pParseResult->GetStringVal(1);
    pParseResult->GetDigitVal(&nBillType, 2);
    char* szChlName = (char* )pParseResult->GetStringVal(3);
    char* szDateName = (char* )pParseResult->GetStringVal(4);
    char szFilePath[MAX_PATH];
    if (ORIG_BILL == nBillType) //ԭʼ����
    {
        sprintf(szFilePath, "%s/%s/%s", m_szOrigBasePath, szAPName, szDateName);
    }
    else        //���ջ���
    {
        sprintf(szFilePath, "%s/%s/%s/%s", m_szFinalBasePath, szAPName,
                szChlName, szDateName);
    }

    //��ȡָ��Ŀ¼�µ������ļ�����Ŀ¼
    LIST<SFILE_STATE> qResult;
    int nRet = GetFiles(szFilePath, qResult);

    //��ȡ�ļ�����Ŀ¼ʧ��
    if(nRet == -1)
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                                 S_QB_TT_FAILED_READDIR);
    }

    //���첢����MML����

    LIST<STRING> TitleList;
    TitleList.push_back(S_QB_TT_NAME);
    TitleList.push_back(S_QB_TT_TYPE);
    TitleList.push_back(S_QB_TT_SIZE);
    TitleList.push_back(S_QB_TT_MTIME);
    TitleList.push_back(S_QB_TT_CTIME);

    //��·�������б�
    LIST<STRING> ContentList;
    char szBuff[50];
    tm* pTimeTm = NULL;

    //�齨MML����
    LIST<SFILE_STATE>::iterator it = qResult.begin();
    while(it != qResult.end())
    {
        ContentList.push_back(it->sFileName.c_str());

        if (it->nType == 0)
        {
            ContentList.push_back(S_FILE_TYPE_FOLDER);
        }
        else
        {
            ContentList.push_back(S_FILE_TYPE_FILE);
        }

        sprintf(szBuff, "%d", it->nSize);
        ContentList.push_back(szBuff);

        pTimeTm = localtime(&(it->mtime));
        sprintf(szBuff, "%04d-%02d-%02d %02d:%02d:%02d",
                pTimeTm->tm_year + 1900, pTimeTm->tm_mon + 1, pTimeTm->tm_mday,
                pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);
        ContentList.push_back(szBuff);

        pTimeTm = localtime(&(it->ctime));
        sprintf(szBuff, "%04d-%02d-%02d %02d:%02d:%02d",
                pTimeTm->tm_year + 1900, pTimeTm->tm_mon + 1, pTimeTm->tm_mday,
                pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);
        ContentList.push_back(szBuff);

        ++it;
    }

    //����MML����
    SendHoriTableReport(pParseResult, TitleList, ContentList,
                        s_QB_TH_LIST_BILL_FILE);
}

/**********************************************************************
��������    ���󻰵���ʽ��Ϣ������ں���
�������    pMsgParsed----������Ϣ֡�����ú���������ɾ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::ReqBillFormatProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    //��MML�����еõ��������
    char* szAPName = (char* )pParseResult->GetStringVal(1);

    //���첢����MML����

    //��ȡ�����͵ı��ı��������
    LIST<STRING> TitleList;
    TitleList.push_back(S_QB_TT_FORMAT_NAME);
    TitleList.push_back(S_QB_TT_FORMAT_DESC);
    TitleList.push_back(S_QB_TT_CHL_INFO);

    LIST<STRING> ContentList;
    //���ԭʼ��������ظ�ʽ
    LIST<STRING> OrigFmtNameList, OrigFmtDescList;
    m_pIQueryBrowse->GetOrigBrowseNames(szAPName, OrigFmtNameList,
                                        OrigFmtDescList);
    LIST<STRING>::iterator iNameIdx, iDescIdx;
    iNameIdx = OrigFmtNameList.begin();
    iDescIdx = OrigFmtDescList.begin();
    for (; iNameIdx != OrigFmtNameList.end(); iNameIdx++)
    {
        ContentList.push_back(*iNameIdx);
        ContentList.push_back(*iDescIdx);
        ContentList.push_back("255");   //255��ʾԭʼ����

        iDescIdx++;
    }
    //������ջ�������ͨ���µ���ظ�ʽ
    LIST<STRING> ChlList;
    m_pIQueryBrowse->GetChlNames(szAPName, ChlList);

    int nChlNo = 1;
    char szChlNo[10];
    LIST<STRING> ChlFmtNameList, ChlFmtDescList;
    LIST<STRING>::iterator iChlIdx;
    for (iChlIdx = ChlList.begin(); iChlIdx != ChlList.end(); iChlIdx++)
    {
        sprintf(szChlNo, "%d", nChlNo);
        nChlNo++;

        ChlFmtNameList.clear();
        ChlFmtDescList.clear();
        m_pIQueryBrowse->GetFinalBrowseNames(szAPName, *iChlIdx,
                                             ChlFmtNameList, ChlFmtDescList);
        iNameIdx = ChlFmtNameList.begin();
        iDescIdx = ChlFmtDescList.begin();
        for (; iNameIdx != ChlFmtNameList.end(); iNameIdx++)
        {
            ContentList.push_back(*iNameIdx);
            ContentList.push_back(*iDescIdx);
            ContentList.push_back(szChlNo);

            iDescIdx++;
        }
    }

    SendHoriTableReport(pParseResult, TitleList, ContentList,
                        S_QB_TH_LIST_FMT);
}

/**********************************************************************
��������    �����ѯ����������ں���
�������    pMsgParsed----������Ϣ֡�����ú���������ɾ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::ReqQueryCondProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    //��MML�����л�ø�ʽ�����õ���Ӧ�ĸ�ʽ�����������
    char* szFmtName = (char* )pParseResult->GetStringVal(1);
    CFormat* pFmt = m_pIQueryBrowse->GetBrowseFmt(szFmtName);
    if (NULL == pFmt)
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                            S_QB_RI_FAIL_GET_FORMAT);
        return;
    }
    int nFieldCount = pFmt->GetFieldNum();
    const SFieldInfo** FieldArray = pFmt->GetFieldsArray();

    //���첢����MML���棬���������ķ��ر���
    LIST<STRING> TitleList, ContentList, RecHeadList;
    TitleList.push_back(S_QB_TT_FIELD_NAME);
    TitleList.push_back(S_QB_TT_FIELD_DESC);
    TitleList.push_back(S_QB_TT_FIELD_TYPE);
    TitleList.push_back(S_QB_TT_FIELD_OPERATOR);
    TitleList.push_back(S_QB_TT_FIELD_UNIT);
    TitleList.push_back(S_QB_TT_ENUM_FLAG);
    TitleList.push_back(S_QB_TT_ENUM_VALUE);

    //�������е��򣬸����������ڲ�ѯ��������֯����
    SFieldInfo* pFieldInfo = NULL;
    BYTE OperMaskArr[6] = {0x01, 0x04, 0x02, 0x10, 0x08, 0x20};
    char AllOperArr[][6] = {"==", ">", "<", ">=", "<=", "!="}; 
    char szFieldType[30], szOperator[10], szEnumFlg[5], szEnumValue[2048];
    int nEnumCount;
    ENUMINFO_LIST* pEnumList;
    ENUMINFO_LIST::iterator iEnumIdx;
    for (int nFieldIdx = 0; nFieldIdx < nFieldCount; nFieldIdx++)
    {
        pFieldInfo = (SFieldInfo* )FieldArray[nFieldIdx];

        //�������ڲ�ѯ����ֱ���Թ�
        if (0 == pFieldInfo->flgQuery)
        {
            continue;
        }

        sprintf(szFieldType, "%d", pFieldInfo->nQueryCondDataType);
        //�õ������ö�ٱ�־�Լ����е�ö��ֵ
        if (0 == pFieldInfo->flgEnum)   //������ö��ֵ
        {
            sprintf(szEnumFlg, "%d", 0);
            szEnumValue[0] = '\0';
        }
        else
        {
            //��ʱszEnumFlg������ʾö��ֵ�ĸ�����ö��ֵ����^�ŷֿ�
            szEnumValue[0] = '\0';  //��֤szEnumValueΪ���ַ���
            pEnumList = pFieldInfo->pEnumInfos;
            nEnumCount = pEnumList->size();
            iEnumIdx = pEnumList->begin();
            STRNCPY(szEnumValue, (*iEnumIdx).szEnumValue, 2048);
            iEnumIdx++;
            for (; iEnumIdx != pEnumList->end(); iEnumIdx++)
            {
                STRNCAT(szEnumValue, "^", 2048);
                STRNCAT(szEnumValue, (*iEnumIdx).szEnumValue, 2048);
            }
            sprintf(szEnumFlg, "%d", nEnumCount);
        }

        //��ȡ���ӣ�һ������ܶ�Ӧ�������
        for (int i = 0; i < 6; i++)
        {
            //�����ӵ�ֵת�����ַ���
            if (pFieldInfo->nOperator & OperMaskArr[i])
            {
                strcpy(szOperator, AllOperArr[i]);

                ContentList.push_back(pFieldInfo->szFieldName);
                ContentList.push_back(pFieldInfo->szFieldDescribe);
                ContentList.push_back(szFieldType);
                ContentList.push_back(szOperator);
                ContentList.push_back(pFieldInfo->szUnit);
                ContentList.push_back(szEnumFlg);
                ContentList.push_back(szEnumValue);
            }
        }
    }

    //������������ÿ����¼�ı���
    int nRecCount = ContentList.size() / TitleList.size();
    char szRecHead[30];
    for (int nRecNo = 1; nRecNo <= nRecCount; nRecNo++)
    {
        sprintf(szRecHead, S_QB_TH_QUERY_COND, nRecNo);
        RecHeadList.push_back(szRecHead);
    }

    SendVertTableReport(pParseResult, TitleList, ContentList, RecHeadList);
}

/**********************************************************************
��������    ���󻰵����������ں���
�������    pMsgParsed----������Ϣ֡�����ú���������ɾ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::ReqBrowseProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    int nDeskNo = pMsgParsed->ClientNo;
    int nWndNo, nPageNo, nBillType;
    pParseResult->GetDigitVal(&nWndNo, 1);
    char* szAPName = (char* )pParseResult->GetStringVal(2);
    pParseResult->GetDigitVal(&nBillType, 3);
    char* szChlName = (char* )pParseResult->GetStringVal(4);
    char* szDateName = (char* )pParseResult->GetStringVal(5);
    char* szFileName = (char* )pParseResult->GetStringVal(6);
    char* szFmtName = (char* )pParseResult->GetStringVal(7);

    pParseResult->GetDigitVal(&nPageNo, 8);

    //���ݲ����õ������ļ��Ĵ��·��
    char szFileFullPath[MAX_PATH];
    if (ORIG_BILL == nBillType)     //ԭʼ����
    {
        sprintf(szFileFullPath, "%s/%s/%s/%s", m_szOrigBasePath, szAPName,
                szDateName, szFileName);
    }
    else                            //���ջ���
    {
        sprintf(szFileFullPath, "%s/%s/%s/%s/%s", m_szFinalBasePath,
                szAPName, szChlName, szDateName, szFileName);
    }

    //���ݴ��ںź�̨�ţ�������������б��ҳ���Ӧ���������
    CBillQBTask* pTask = NULL;
    LIST<CBillQBTask* >::iterator iTskIdx;
    iTskIdx = m_qBrowseTask.begin();
    for (; iTskIdx != m_qBrowseTask.end(); iTskIdx++)
    {
        if ((*iTskIdx)->IsMatched(nDeskNo, nWndNo))
        {
            pTask = *iTskIdx;
            break;
        }
    }

    //�����Ӧ�����񲻴��ڣ��򴴽���������Ӧ������
    if (NULL == pTask)
    {
        pTask = new CBillQBTask(nDeskNo, nWndNo, nPageNo,
                                szFmtName, nBillType, m_szQBTmpResultPath);
        int nAPID = GetAPID(szAPName);
        pTask->SetDisplayErrBill(m_bDisplayErrBill);
        BOOL bSucc = pTask->Init(szFileFullPath, GetAPType(szAPName), nAPID);
        if (!bSucc)
        {
            SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                                 S_QB_RI_FAIL_PREPARE_FILE);
            delete pTask;
            return;
        }
        pTask->SetMustMerge(GetAPMustMerge(szAPName));
        pTask->SetFinalSPFileFmt(GetAPFinalSPFileFmt(szAPName));
        bSucc = pTask->Run();
        if (!bSucc)
        {
            SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                                 S_QB_RI_FAIL_RUN_TASK);
            delete pTask;
            return;
        }
        m_qBrowseTask.push_back(pTask);

    }

    SendSimpleInfoReport(pParseResult->GetCmdLine(), 0,
                         S_QB_RI_SUCCESS_START_TASK);
}

/**********************************************************************
��������    ���󻰵���ѯ������ں���
�������    pMsgParsed----������Ϣ֡�����ú���������ɾ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::ReqQueryProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    int nDeskNo = pMsgParsed->ClientNo;

    int nWndNo, nPageNo, nBillType, nFlg;
    pParseResult->GetDigitVal(&nWndNo, 1);
    char* szAPName = (char* )pParseResult->GetStringVal(2);
    pParseResult->GetDigitVal(&nBillType, 3);
    char* szChlName = (char* )pParseResult->GetStringVal(4);
    char* szDateName = (char* )pParseResult->GetStringVal(5);
    char* szFmtName = (char* )pParseResult->GetStringVal(6);
    pParseResult->GetDigitVal(&nFlg, 7);
    char* szBegin = (char* )pParseResult->GetStringVal(8);
    char* szEnd = (char* )pParseResult->GetStringVal(9);
    char* szCond = (char* )pParseResult->GetStringVal(10);
    pParseResult->GetDigitVal(&nPageNo, 11);

    
    //���ⵥ SWPD05145 YanYong 2004-05-25
    MAP<int, LIST<STRING> >::iterator it;
    LIST<STRING> queryFileList;
    if ( (it = m_ClientFileListMap.find(pMsgParsed->ClientNo)) != m_ClientFileListMap.end())
    {
        (*it).second.push_back(szBegin);
    }
    else
    {
        queryFileList.push_back(szBegin);
        m_ClientFileListMap[pMsgParsed->ClientNo] = queryFileList;
        it = m_ClientFileListMap.find(pMsgParsed->ClientNo);
    }

    if (strcmp(szEnd, "THE_LAST_FILE") !=0)
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(),
                             100,
                             "waitting for others files",
                             pMsgParsed->ClientNo);
        return;
    }
    else
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(),
                             0,
                             "Query Begining",
                             pMsgParsed->ClientNo);
    }
    //</SWPD05145>

    //���ݲ������Ҫ��ѯ�Ļ�����·��
    char szQueryPath[MAX_PATH];
    if ((ORIG_BILL == nBillType) && (DATE_RANGE == nFlg))
    //��ѯԭʼ�����µĶ������Ŀ¼
    {
        sprintf(szQueryPath, "%s/%s", m_szOrigBasePath, szAPName);
    }
    else if ((ORIG_BILL == nBillType) && (DATE_RANGE != nFlg))
    //��ѯԭʼ�����µĶ�������ļ�
    {
        sprintf(szQueryPath, "%s/%s/%s", m_szOrigBasePath,
                szAPName, szDateName);
    }
    else if ((ORIG_BILL != nBillType) && (DATE_RANGE == nFlg))
    //��ѯ���ջ����µĶ������Ŀ¼
    {
        sprintf(szQueryPath, "%s/%s/%s", m_szFinalBasePath,
                szAPName, szChlName);
    }
    else    //��ѯ���ջ����µĶ�������ļ�
    {
        sprintf(szQueryPath, "%s/%s/%s/%s", m_szFinalBasePath, szAPName,
                szChlName, szDateName);
    }

    //���ݴ��ںź�̨�ţ�������ѯ�����б��ҳ���Ӧ���������
    CBillQBTask* pTask = NULL;
    LIST<CBillQBTask* >::iterator iTskIdx;
    iTskIdx = m_qQueryTask.begin();
    for (; iTskIdx != m_qQueryTask.end(); iTskIdx++)
    {
        if ((*iTskIdx)->IsMatched(nDeskNo, nWndNo))
        {
            pTask = *iTskIdx;
            break;
        }
    }

    //�����Ӧ�����񲻴��ڣ��򴴽���������Ӧ������
    if (NULL == pTask)
    {
        //�Բ�ѯ������������Ĵ����Ա�ʹ�ӻ���̨�͹����Ĳ�ѯ����
        //�������ʽ�������
        /*char szExCond[MAX_COND_LEN];
        if (DATE_RANGE == nFlg)
        {
            TransQueryCond(szCond, szBegin, szEnd, szExCond);
        }
        else
        {
            TransQueryCond(szCond, szDateName, szDateName, szExCond);
        }
        */

        pTask = new CBillQBTask(nDeskNo, nWndNo, nPageNo, szFmtName, nBillType, m_szQBTmpResultPath);
        int nAPID = GetAPID(szAPName);
        //���ⵥ SWPD05145 YanYong 2004-05-25 
//        BOOL bSucc = pTask->Init(szQueryPath, szBegin, szEnd, nFlg,         
//                                 szCond, GetAPType(szAPName), nAPID);  // ���ⵥ SWPD05145 ɾ������ YanYong 2004-05-26
        
        BOOL bSucc = pTask->Init(szQueryPath, (*it).second, nFlg,
                                 szCond, GetAPType(szAPName), nAPID);
        //</SWPD05145>

        if (!bSucc)
        {
            SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                                 S_QB_RI_FAIL_PREPARE_FILE,pMsgParsed->ClientNo);
            delete pTask;
            return;
        }
        pTask->SetMustMerge(GetAPMustMerge(szAPName));
        pTask->SetFinalSPFileFmt(GetAPFinalSPFileFmt(szAPName));
        bSucc = pTask->Run();
        if (!bSucc)
        {
            SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                                 S_QB_RI_FAIL_RUN_TASK,pMsgParsed->ClientNo);
            delete pTask;
            return;
        }

        m_qQueryTask.push_back(pTask);
    }

    SendSimpleInfoReport(pParseResult->GetCmdLine(),
                         0,
                         S_QB_RI_SUCCESS_START_TASK,
                         pMsgParsed->ClientNo);
}

/**********************************************************************
��������    ���������������ں���
�������    pMsgParsed----������Ϣ֡�����ú���������ɾ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::ReqCloseTaskProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    int nDeskNo = pMsgParsed->ClientNo;
    int nWndNo;
    pParseResult->GetDigitVal(&nWndNo, 1);

    LIST<CBillQBTask* >* pTaskList;
    //���ݴ��ںź�̨�ţ�������ѯ��������б��ҳ���Ӧ���������
    CBillQBTask* pTask = NULL;
    LIST<CBillQBTask* >::iterator iTskIdx;
    iTskIdx = m_qQueryTask.begin();
    for (; iTskIdx != m_qQueryTask.end(); iTskIdx++)
    {
        if ((*iTskIdx)->IsMatched(nDeskNo, nWndNo))
        {
            pTask = *iTskIdx;
            pTaskList = &m_qQueryTask;
            break;
        }
    }
    if (NULL == pTask)
    {
        iTskIdx = m_qBrowseTask.begin();
        for (; iTskIdx != m_qBrowseTask.end(); iTskIdx++)
        {
            if ((*iTskIdx)->IsMatched(nDeskNo, nWndNo))
            {
                pTask = *iTskIdx;
                pTaskList = &m_qBrowseTask;
                break;
            }
        }
    }

    //�����ڶ�Ӧ�������򷵻ظ�����ҵ��̨"���񲻴���"��Ϣ
    if (NULL == pTask)
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                             S_QB_RI_TASK_NOT_EXIST);
    }
    else    //���򣬽�����ɾ������
    {
        DestroyTask(pTask);
        pTaskList->erase(iTskIdx);
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 0,
                             S_QB_RI_SUCCESS_CLOSE_TASK);
    }
}

/**********************************************************************
��������    ���������������ں���
�������    pMsgParsed----������Ϣ֡�����ú���������ɾ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::ReqStopTaskProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    int nDeskNo = pMsgParsed->ClientNo;
    int nWndNo;
    pParseResult->GetDigitVal(&nWndNo, 1);

    LIST<CBillQBTask* >* pTaskList;
    //���ݴ��ںź�̨�ţ�������ѯ��������б��ҳ���Ӧ���������
    CBillQBTask* pTask = NULL;
    LIST<CBillQBTask* >::iterator iTskIdx;
    iTskIdx = m_qQueryTask.begin();
    for (; iTskIdx != m_qQueryTask.end(); iTskIdx++)
    {
        if ((*iTskIdx)->IsMatched(nDeskNo, nWndNo))
        {
            pTask = *iTskIdx;
            pTaskList = &m_qQueryTask;
            break;
        }
    }
    if (NULL == pTask)
    {
        iTskIdx = m_qBrowseTask.begin();
        for (; iTskIdx != m_qBrowseTask.end(); iTskIdx++)
        {
            if ((*iTskIdx)->IsMatched(nDeskNo, nWndNo))
            {
                pTask = *iTskIdx;
                pTaskList = &m_qBrowseTask;
                break;
            }
        }
    }

    //�����ڶ�Ӧ�������򷵻ظ�����ҵ��̨"���񲻴���"��Ϣ
    if (NULL == pTask)
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                             S_QB_RI_TASK_NOT_EXIST);
    }
    else    //���򣬽�������
    {
        StopTask(pTask);
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 0,
                             S_QB_RI_SUCCESS_STOP_TASK);
    }
}

/**********************************************************************
��������    ���������������ں���
�������    pMsgParsed----������Ϣ֡�����ú���������ɾ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::ReqTaskProgressProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    int nDeskNo = pMsgParsed->ClientNo;
    int nWndNo;
    pParseResult->GetDigitVal(&nWndNo, 1);

    LIST<CBillQBTask* >* pTaskList;
    //���ݴ��ںź�̨�ţ�������ѯ��������б��ҳ���Ӧ���������
    CBillQBTask* pTask = NULL;
    LIST<CBillQBTask* >::iterator iTskIdx;
    iTskIdx = m_qQueryTask.begin();
    for (; iTskIdx != m_qQueryTask.end(); iTskIdx++)
    {
        if ((*iTskIdx)->IsMatched(nDeskNo, nWndNo))
        {
            pTask = *iTskIdx;
            pTaskList = &m_qQueryTask;
            break;
        }
    }
    if (NULL == pTask)
    {
        iTskIdx = m_qBrowseTask.begin();
        for (; iTskIdx != m_qBrowseTask.end(); iTskIdx++)
        {
            if ((*iTskIdx)->IsMatched(nDeskNo, nWndNo))
            {
                pTask = *iTskIdx;
                pTaskList = &m_qBrowseTask;
                break;
            }
        }
    }

    //�����ڶ�Ӧ�������򷵻ظ�����ҵ��̨"���񲻴���"��Ϣ
    if (NULL == pTask)
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                             S_QB_RI_TASK_NOT_EXIST);
        return;
    }
    else
    {
        UINT4 nRecordCount;
        int nPercent, nCurPage;
        BOOL bSucc = pTask->GetProcess(nRecordCount, nPercent, nCurPage);

        /*if(!bSucc)
        {
            SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                                 S_QB_RI_FAIL_GET_PROGRESS);
        }*/

        //�õ�����ҳ������Ϣ���ı��������
        LIST<STRING> InfoTitleList, InfoContentList;
        InfoTitleList.push_back(S_QB_TT_RECORD_COUNT);
        InfoTitleList.push_back(S_QB_TT_CUR_PERCENT);
        InfoTitleList.push_back(S_QB_TT_CUR_PAGE_NO);

        char szRecordCount[20], szPercent[20], szCurPage[20];
        sprintf(szRecordCount, "%d", nRecordCount);
        sprintf(szPercent, "%d", nPercent);
        sprintf(szCurPage, "%d", nCurPage);

        InfoContentList.push_back(szRecordCount);
        InfoContentList.push_back(szPercent);
        InfoContentList.push_back(szCurPage);

        //����MML���沢����
        m_pMMLReport->MakeQBReportHead(pParseResult->GetCmdLine());
        m_pMMLReport->AddHoriTable(InfoTitleList, InfoContentList,
                                   S_QB_TH_PROGESS_INFO);
        m_pMMLReport->MakeReportTail();

        const char* szReport = m_pMMLReport->GetReport();
        SendMMLReport(szReport);

    }
}

/**********************************************************************
��������    ����ҳ������ں���
�������    pMsgParsed----������Ϣ֡�����ú���������ɾ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::ReqGetPageDataProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    int nDeskNo = pMsgParsed->ClientNo;
    int nWndNo;
    pParseResult->GetDigitVal(&nWndNo, 1);
    int nPageNo;
    pParseResult->GetDigitVal(&nPageNo, 2);

    LIST<CBillQBTask* >* pTaskList;
    //���ݴ��ںź�̨�ţ�������ѯ��������б��ҳ���Ӧ���������
    CBillQBTask* pTask = NULL;
    LIST<CBillQBTask* >::iterator iTskIdx;
    iTskIdx = m_qQueryTask.begin();
    for (; iTskIdx != m_qQueryTask.end(); iTskIdx++)
    {
        if ((*iTskIdx)->IsMatched(nDeskNo, nWndNo))
        {
            pTask = *iTskIdx;
            pTaskList = &m_qQueryTask;
            break;
        }
    }
    if (NULL == pTask)
    {
        iTskIdx = m_qBrowseTask.begin();
        for (; iTskIdx != m_qBrowseTask.end(); iTskIdx++)
        {
            if ((*iTskIdx)->IsMatched(nDeskNo, nWndNo))
            {
                pTask = *iTskIdx;
                pTaskList = &m_qBrowseTask;
                break;
            }
        }
    }

    //�����ڶ�Ӧ�������򷵻ظ�����ҵ��̨"���񲻴���"��Ϣ
    if (NULL == pTask)
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                             S_QB_RI_TASK_NOT_EXIST);
    }
    else
    {
        //�õ��������ݱ��ı��������
        LIST<STRING> BillTitleList, BillContentList;
        pTask->GetBillTitleList(BillTitleList);
        BillTitleList.push_back("filename");        

        if (BillTitleList.empty())
        {
            SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                                 S_QB_RI_FAIL_GET_PAGE_DATA);
            return;
        }

        int nResult = pTask->GetPageData(nPageNo, BillContentList);
        if(0 == nResult)
        {
            SendSimpleInfoReport(pParseResult->GetCmdLine(), 1, S_QB_RI_FAIL_GET_PAGE_DATA);
            return;
        }

        LIST<STRING> InfoTitleList, InfoContentList;
        InfoTitleList.push_back(S_QB_TT_CUR_PAGE_NO);
        InfoTitleList.push_back(S_QB_TT_FIRST_RECORD_COUNT);
        InfoTitleList.push_back(S_QB_TT_LAST_RECORD_COUNT);

        char szCurPage[20], szFirstRecorfNo[20], szLastRecordNo[20];
        sprintf(szCurPage, "%d", nPageNo);
        int nFirstRecordNo = (nPageNo - 1)* m_uPageSize + 1;
        sprintf(szFirstRecorfNo, "%d", nFirstRecordNo);
        int nLastRecordNo = nFirstRecordNo + nResult - 1;
        sprintf(szLastRecordNo, "%d", nLastRecordNo);

        InfoContentList.push_back(szCurPage);
        InfoContentList.push_back(szFirstRecorfNo);
        InfoContentList.push_back(szLastRecordNo);

        //�������ݿ��ܱȽϴ󣬱��뿼�Ƿ�ҳ������
        int nSubPageNo = 1;     //��ҳ����ʱ����ҳ��
        while (!BillContentList.empty() || (1 == nSubPageNo))
        {
            //����MML���沢����
            m_pMMLReport->MakeQBReportHead(pParseResult->GetCmdLine());
            m_pMMLReport->AddHoriTable(InfoTitleList, InfoContentList,
                                       S_QB_TH_PAGE_INFO);
            m_pMMLReport->AddQBHoriTable(BillTitleList, BillContentList,
                                         S_QB_TH_BILL_DATA, nSubPageNo);
            m_pMMLReport->MakeReportTail();

            const char* szReport = m_pMMLReport->GetReport();
            SendMMLReport(szReport);
        }

    }
}
/**********************************************************************
��������    ���󴰿����ִ�����
�������    pMsgParsed----������Ϣ֡�����ú���������ɾ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::ReqWndShakeHandsProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    int nDeskNo = pMsgParsed->ClientNo;
    int nWndNo;
    pParseResult->GetDigitVal(&nWndNo, 1);

    //���ݴ��ںź�̨�ţ��ҳ���Ӧ�Ĳ�ѯ������������
    CBillQBTask* pTask = NULL;
    LIST<CBillQBTask* >::iterator iTskIdx;
    iTskIdx = m_qBrowseTask.begin();
    for (iTskIdx = m_qBrowseTask.begin();
            iTskIdx != m_qBrowseTask.end(); iTskIdx++)
    {
        if ((*iTskIdx)->IsMatched(nDeskNo, nWndNo))
        {
            pTask = *iTskIdx;
            break;
        }
    }
    if (NULL == pTask)
    {
        iTskIdx = m_qQueryTask.begin();
        for (; iTskIdx != m_qQueryTask.end(); iTskIdx++)
        {
            if ((*iTskIdx)->IsMatched(nDeskNo, nWndNo))
            {
                pTask = *iTskIdx;
                break;
            }
        }
    }

    //������Ӧ�������Ƿ�����򻰵�̨������Ӧ�ı��沢����Ӧ����
    if (NULL == pTask)
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                             S_QB_RI_TASK_NOT_EXIST);
    }
    else
    {
        pTask->RsetTimeOutFlg();
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 0,
                             S_QB_RI_SHAKE_CORRECT);
    }
}

/**********************************************************************
��������    ����ָ���Ĳ�ѯ�������
�������    pTask----ָ��������
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::DestroyTask(CBillQBTask* pTask)
{
    if(pTask->IsRunning())
    {
        pTask->Stop();
        pTask->WaitThreadExit();
    }

    delete pTask;
}

/**********************************************************************
��������    ָֹͣ���Ĳ�ѯ�������
�������    pTask----ָ��������
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::StopTask(CBillQBTask* pTask)
{
    if(!(pTask->IsRunning()))
    {
        pTask->Stop();
        pTask->WaitThreadExit();
    }
}
/**********************************************************************
��������    ���ͱ�������ֻ�м򵥵���Ϣ��MML����
�������    szCmd----MML�����Ӧ������
            nRetCode----����Ĵ�����
            szInfo----�������еļ���Ϣ
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::SendSimpleInfoReport(const char* szCmd, int nRetCode,
                                         const char* szInfo)
{
    m_pMMLReport->MakeQBReportHead(szCmd, nRetCode);
    m_pMMLReport->AddString(szInfo);
    m_pMMLReport->AddString("\r\n");
    m_pMMLReport->MakeReportTail();
    const char* szReport = m_pMMLReport->GetReport();

    SendMMLReport(szReport);
}

/**********************************************************************
��������    ���ͱ�����Ϊһ���������MML����
�������    pParseResult----MML���������Ķ���
            TitleList----���ı���
            ContentList----��������
            szTableHeadName----������������
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::SendHoriTableReport(CParseResult* pParseResult,
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList,
                        const char* szTableHeadName)
{
    //���ڱ������ݿ��ܻ�ܴ����Ա��뿼���Է�ҳ�ķ�ʽ����MML����
    int nSubPageNo = 1;
    const char* szReport;
    while (!ContentList.empty() || (1 == nSubPageNo))
    {
        m_pMMLReport->MakeQBReportHead(pParseResult->GetCmdLine());
        m_pMMLReport->AddQBHoriTable(TitleList, ContentList,
                                szTableHeadName, nSubPageNo);
        m_pMMLReport->MakeReportTail();

        szReport = m_pMMLReport->GetReport();
        SendMMLReport(szReport);
    }
}

/**********************************************************************
��������    ���ͱ�����Ϊһ���������MML����
�������    pParseResult----MML���������Ķ���
            TitleList----���ı���
            ContentList----��������
            RecHeadList----�������и�����¼������
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::SendVertTableReport(CParseResult* pParseResult,
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList,
                        LIST<STRING>& RecHeadList)
{
    //���ڱ������ݿ��ܻ�ܴ����Ա��뿼���Է�ҳ�ķ�ʽ����MML����
    const char* szReport;
    int nSubPageNo = 1;
    while (!ContentList.empty() || (1 == nSubPageNo))
    {
        m_pMMLReport->MakeQBReportHead(pParseResult->GetCmdLine());
        m_pMMLReport->AddQBVertTable(TitleList, ContentList,
                                    RecHeadList);
        m_pMMLReport->MakeReportTail();

        szReport = m_pMMLReport->GetReport();
        SendMMLReport(szReport);

        nSubPageNo++;
    }
}

/**********************************************************************
��������    ����û�л������ݵ�MML����
�������    pParseResult----MML���������Ķ���
            szNextPageExist----��һҳ�Ƿ����
            szPageNo----��ǰҳ��ҳ��
            szPercent----׼����ǰҳ���ݵĽ���
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::SendEmptyBillDataReport(CParseResult* pParseResult,
                            CBillQBTask* pTask,
                            const char* szNextPageExist,
                            const char* szPageNo, const char* szPercent)
{
    //��û���ҳ��Ϣ���ı��������
    LIST<STRING> InfoTitleList, InfoContentList;
    InfoTitleList.push_back(S_QB_TT_HAVE_NEXT_PAGE);
    InfoTitleList.push_back(S_QB_TT_CUR_PAGE_NO);
    InfoTitleList.push_back(S_QB_TT_CUR_PERCENT);

    InfoContentList.push_back(szNextPageExist);
    InfoContentList.push_back(szPageNo);
    InfoContentList.push_back(szPercent);

    //�õ��������ݱ��ı��⣬����Ϊ��
    LIST<STRING> BillTitleList, BillContentList;

    pTask->GetBillTitleList(BillTitleList);
    if (BillTitleList.empty())
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                             S_QB_RI_FAIL_GET_PAGE_DATA);
        return;
    }

    int nSubPageNo = 1;   //����û�л������ݵ�����£����÷�ҳ���ͣ�
                          //����ҳ��϶�Ϊ1
    m_pMMLReport->MakeQBReportHead(pParseResult->GetCmdLine());
    m_pMMLReport->AddHoriTable(InfoTitleList, InfoContentList,
                               S_QB_TH_PAGE_INFO);
    m_pMMLReport->AddQBHoriTable(BillTitleList, BillContentList,
                                 S_QB_TH_BILL_DATA, nSubPageNo);
    m_pMMLReport->MakeReportTail();

    const char* szReport = m_pMMLReport->GetReport();
    SendMMLReport(szReport);
}

/**********************************************************************
��������    ��MML����ת����MSG_INTRA����ʽ���ͳ�ȥ
�������    szReport----�����͵�MML����
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBServer::SendMMLReport(const char* szReport)
{
    int nStrLen = strlen(szReport);

    MSG_INTRA* pMsg = new(nStrLen + 1) MSG_INTRA;
    pMsg->SenderPid = PT_SELF;
    pMsg->SenderMid = MT_BILLBROWSER;
    pMsg->ReceiverPid = PT_OM;
    pMsg->ReceiverMid = MT_MMLDEBUG;
    pMsg->AppType = APP_MML_NORM;
    pMsg->ClientNo = m_nCurClientNo;

    memcpy(pMsg->Data, szReport, nStrLen + 1);

    SendMsg(pMsg);
}

/**********************************************************************
��������    �õ����������Ӧ�Ľ���������
�������    szReport----�����͵�MML����
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
int CBillQBServer::GetAPType(const char* szAPName)
{
    //�����б�m_APInfoList���ҳ���szAPName��Ӧ�Ľ���������
    LIST<SAPInfo* >::iterator i;
    for (i = m_APInfoList.begin(); i != m_APInfoList.end(); i++)
    {
        if (0 == strcmp(szAPName, (*i)->szAPName))
        {
            return (*i)->nAPType;
        }
    }

    return AP_128;
}

/**********************************************************************
��������    �õ�3G������Ӧ�Ľ�����
�������    szAPName----���������
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
int CBillQBServer::GetAPID(const char* szAPName)
{
    //�����б�m_APInfoList���ҳ���szAPName��Ӧ�Ľ���������
    LIST<SAPInfo* >::iterator i;
    for (i = m_APInfoList.begin(); i != m_APInfoList.end(); i++)
    {
        if (0 == strcmp(szAPName, (*i)->szAPName))
        {
            return (*i)->nAPID;
        }
    }

    return NULL;
}

int CBillQBServer::GetAPMustMerge(const char* szAPName)
{
    //�����б�m_APInfoList���ҳ���szAPName��Ӧ�Ľ�����
    LIST<SAPInfo* >::iterator i;
    for (i = m_APInfoList.begin(); i != m_APInfoList.end(); i++)
    {
        if (0 == strcmp(szAPName, (*i)->szAPName))
        {
            return (*i)->bMustMerge;
        }
    }

    return 0;
}
int CBillQBServer::GetAPFinalSPFileFmt(const char* szAPName)
{
    //�����б�m_APInfoList���ҳ���szAPName��Ӧ�Ľ�����
    LIST<SAPInfo* >::iterator i;
    for (i = m_APInfoList.begin(); i != m_APInfoList.end(); i++)
    {
        if (0 == strcmp(szAPName, (*i)->szAPName))
        {
            return (*i)->bFinalSPFileFmt;
        }
    }

    return CFG_AP_KEY_FINAL_SPFILEFMT_DEFAULT;
}

/**********************************************************************
��������    �Ի�����ѯ�����������⴦��
�������    szCond----�ӻ���̨�͹����Ĳ�ѯ����
            szBeginDate----�ӻ���̨�͹�������ʼ����
            szEndDate----�ӻ���̨�͹����Ľ�������
�������    szExCond----�����Ĳ�ѯ�������ռ��ں�����������ͷ�
����ֵ      ��
�׳��쳣    ��
��ע        ���ڻ���̨�������Ĳ�ѯ�����У���ʼʱ��Ĳ�ѯ����ֻ����ʱ����
            �����ڸ�ʽ����ж��У���ʼʱ��ȴ����������ں�ʱ�������ԣ�
            ���뽫����̨���͵���Ϣ�еĿ�ʼ���ںͽ������ڷֱ���뵽��ʼʱ��
            �Ĳ�ѯ������ȥ����ʼʱ�����������Ϊ"StartTime"��ʱ�����ʽ��
            2001-11-05 21:00:03��ʾ���ӻ���̨���͵���Ϣ�е�������20011105
            ��ʾ���������ڵ���ʾ��ʽ���������Ӧ��ת�������磬�ӻ���̨���ص�
            ��ѯ�����У���ʼʱ��Ĳ�ѯ����Ϊ"StartTime >= 08:05:00"��
            "StartTime <= 18:05:00"����ʼ���ںͽ������ڷֱ�Ϊ"20011101"��
            "20011103"�����޸ĺ�Ŀ�ʼʱ���ѯ����Ϊ
            "StartTime >= 2001-11-01 08:05:00"��
            "StartTime <= 2001-11-03 18:05:00"
**********************************************************************/
void CBillQBServer::TransQueryCond(const char* szCond,
                                const char* szBeginDate,
                                const char* szEndDate, char* szExCond)
{
    //�޸Ŀ�ʼ���ںͽ������ڵĸ�ʽ
    char szExBeginDate[40], szExEndDate[40], szYear[10], szMon[10], szDay[10];
    memcpy(szYear, szBeginDate, 4);
    szYear[4] = '\0';
    memcpy(szMon, szBeginDate + 4, 2);
    szMon[2] = '\0';
    memcpy(szDay, szBeginDate + 6, 2);
    szDay[2] = '\0';
    sprintf(szExBeginDate, "%s-%s-%s ", szYear, szMon, szDay);
    memcpy(szYear, szEndDate, 4);
    szYear[4] = '\0';
    memcpy(szMon, szEndDate + 4, 2);
    szMon[2] = '\0';
    memcpy(szDay, szEndDate + 6, 2);
    szDay[2] = '\0';
    sprintf(szExEndDate, "%s-%s-%s ", szYear, szMon, szDay);

    //�޸Ĳ�ѯ����������ʼ���ںͽ������ڲ���

    char* pReadPos = (char* )szCond;    //�����ַ�������ʱ�Ķ�ָ��
    szExCond[0] = '\0';
    char* pWritePos = (char* )szExCond; //�����ַ�������ʱ��дָ��
    char* pSchPos = (char* )szCond;     //�����ַ����в��ҵ�ָ��
    char cOperator;                     //StartTime�����������ӵĵ�һ���ַ�

    //��szCond�в������е�StartTime�����StartTime�������Ϊ>=��>��
    //�����������м���szExBeginDate���������szEndDate
    while (1)
    {
       pSchPos = (char* )strstr(pSchPos, "StartTime");
       if (NULL == pSchPos)
       {
           break;
       }
       else
       {
           pSchPos += 9;

           //����StartTime���������ӵĵ�һ���ַ�
           while (1)
           {
               if (' ' != *pSchPos)
               {
                   cOperator = *pSchPos;
                   break;
               }
               pSchPos++;
           }

           //����ʼ���ڻ�������ڵ���Ϣ���뵽��ѯ������
           pSchPos = strchr(pSchPos, '@');
           ASSERT(NULL != pSchPos);

           memcpy(pWritePos, pReadPos, pSchPos - pReadPos + 1);
           pWritePos += pSchPos - pReadPos + 1;
           pReadPos += pSchPos - pReadPos + 1;
           if ('>' == cOperator)
           {
               memcpy(pWritePos, szExBeginDate, strlen(szExBeginDate));
               pWritePos += strlen(szExBeginDate);
           }
           else
           {
               ASSERT('<' == cOperator);
               memcpy(pWritePos, szExEndDate, strlen(szExEndDate));
               pWritePos += strlen(szExEndDate);
           }
       }
    }

    *pWritePos = '\0';
    STRNCAT(szExCond, pReadPos, MAX_COND_LEN);
}


//Added by XDB on 2002.11.11
//���ӶԲɼ��������������
void CBillQBServer::ReqFileRoot(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));

    //���첢����MML����
    LIST<STRING> TitleList;
    TitleList.push_back(S_QB_TT_NAME);

    //��·�������б�
    LIST<STRING> ContentList;

    ContentList.push_back(m_szOrigBasePath);
    ContentList.push_back(m_szFinalBasePath);

    SendHoriTableReport(pParseResult, TitleList, ContentList,
                        s_QB_TH_LIST_ROOT_DIR);
}

void CBillQBServer::ReqBillFiles(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    char* szPath = (char* )pParseResult->GetStringVal(1);

    //��������Ƿ�
    if(szPath == NULL)
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                                 S_QB_TT_INVALID_PATH);
    }

    //��ȡָ��Ŀ¼�µ������ļ�����Ŀ¼
    LIST<SFILE_STATE> qResult;
    int nRet = GetFiles(szPath, qResult);

    //��ȡ�ļ�����Ŀ¼ʧ��
    if(nRet == -1)
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                                 S_QB_TT_FAILED_READDIR);
    }

    //���첢����MML����
    LIST<STRING> TitleList;
    TitleList.push_back(S_QB_TT_NAME);
    TitleList.push_back(S_QB_TT_TYPE);
    TitleList.push_back(S_QB_TT_SIZE);
    TitleList.push_back(S_QB_TT_MTIME);
    TitleList.push_back(S_QB_TT_CTIME);

    //��·�������б�
    LIST<STRING> ContentList;
    char szBuff[50];
    tm* pTimeTm = NULL;

    //�齨MML����
    LIST<SFILE_STATE>::iterator it = qResult.begin();
    while(it != qResult.end())
    {
        ContentList.push_back(it->sFileName.c_str());

        sprintf(szBuff, "%d", it->nType);
        ContentList.push_back(szBuff);

        sprintf(szBuff, "%d", it->nSize);
        ContentList.push_back(szBuff);

        pTimeTm = localtime(&(it->mtime));
        sprintf(szBuff, "%04d-%02d-%02d %02d:%02d:%02d",
                pTimeTm->tm_year + 1900, pTimeTm->tm_mon + 1, pTimeTm->tm_mday,
                pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);
        ContentList.push_back(szBuff);

        pTimeTm = localtime(&(it->ctime));
        sprintf(szBuff, "%04d-%02d-%02d %02d:%02d:%02d",
                pTimeTm->tm_year + 1900, pTimeTm->tm_mon + 1, pTimeTm->tm_mday,
                pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);
        ContentList.push_back(szBuff);

        ++it;
    }

    //����MML����
    SendHoriTableReport(pParseResult, TitleList, ContentList,
                        s_QB_TH_LIST_BILL_FILE);
}

int CBillQBServer::GetFiles(const char* szPath, LIST<SFILE_STATE>& qResult)
{
    if(szPath == NULL)
    {
        return -1;
    }
    LIST<SFILE_STATE>  qTmpResult;
    LIST<STRING>  qTmpName;

    ACE_DIR *pDir = NULL;
    dirent* pDirent =  NULL;

    //�򿪸�ʽ�����ļ�Ŀ¼
    pDir = ACE_OS::opendir(szPath);
    if(pDir == NULL)
    {
        return -1;
    }

    int nCount = 0;
    SFILE_STATE FileState;
    char szFileFullPath[MAX_PATH];
    ACE_stat FileStat;

    //˳���ȡ�ҵ����ļ�
    while ((pDirent = ACE_OS::readdir(pDir)) != NULL)
    {
        FileState.sFileName = pDirent->d_name;
        qTmpName.push_back(pDirent->d_name);

        SNPRINTF(szFileFullPath, MAX_PATH, "%s/%s", szPath, pDirent->d_name);
        ACE_OS::stat(szFileFullPath, &FileStat);

        FileState.nSize = FileStat.st_size;
        FileState.ctime = FileStat.st_ctime;
        FileState.mtime = FileStat.st_mtime;

        if (S_IFDIR == (FileStat.st_mode & S_IFMT))    //�ҵ�����һ��Ŀ¼
        {
            //����ȷ��Ŀ¼����"."��".."
            if ((0 != strcmp(pDirent->d_name, "."))
                && (0 != strcmp(pDirent->d_name, "..")))
            {
                FileState.nType = 0;
                qTmpResult.push_back(FileState);
                nCount++;
            }
        }
        else
        {
            FileState.nType = 1;
            qTmpResult.push_back(FileState);
            nCount++;
        }
    }

    qTmpName.sort();

    LIST<STRING>::iterator it = qTmpName.begin();

    while (it != qTmpName.end())
    {
        LIST<SFILE_STATE>::iterator it_state = qTmpResult.begin();

        while (it_state != qTmpResult.end())
        {
            if ((*it).compare((*it_state).sFileName) == 0)
            {
                qResult.push_back(*it_state);
                break;
            }
            it_state++;
        }

        it++;
    }

    ACE_OS::closedir(pDir);
    return nCount;
}
//End of Add by XDB

/********************************************************
//���ⵥ SWPD05145  add by YanYong 2004-05-25
���ܣ� ��Ӹú�����֧����ָ����clientno�ͻ��˷��ͱ���
********************************************************/
void CBillQBServer::SendMMLReport(const char *szReport, int clientNo)
{
    int nStrLen = strlen(szReport);
    
    MSG_INTRA* pMsg = new(nStrLen + 1) MSG_INTRA;
    pMsg->SenderPid = PT_SELF;
    pMsg->SenderMid = MT_BILLBROWSER;
    pMsg->ReceiverPid = PT_OM;
    pMsg->ReceiverMid = MT_MMLDEBUG;
    pMsg->AppType = APP_MML_NORM;
    pMsg->ClientNo = clientNo;
    
    memcpy(pMsg->Data, szReport, nStrLen + 1);
    
    SendMsg(pMsg);
}

/********************************************************
//���ⵥ SWPD05145  add by YanYong 2004-05-25
���ܣ� ��Ӹú�����֧����ָ����clientno�ͻ��˷��ͱ���
********************************************************/
void CBillQBServer::SendSimpleInfoReport(const char *szCmd, int nRetCode, const char *szInfo, int clientNo)
{
    m_pMMLReport->MakeQBReportHead(szCmd, nRetCode);
    m_pMMLReport->AddString(szInfo);
    m_pMMLReport->AddString("\r\n");
    m_pMMLReport->MakeReportTail();
    const char* szReport = m_pMMLReport->GetReport();
    
    SendMMLReport(szReport, clientNo);
}
