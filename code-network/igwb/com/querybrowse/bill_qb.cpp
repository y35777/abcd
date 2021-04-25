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
功能描述    初始化对象状态和成员变量
输入参数    无
输出参数    无
返回值      0----成功
            1----失败
            <0----失败，返回值的绝对值为错误码
抛出异常    无
**********************************************************************/
int CBillQBServer::InitInstance()
{
    int nResult = CWorkModule::InitInstance();
    if (ERR_SUCCESS != nResult)
    {
        TRACE(MTS_BILLBROWSER, S_QB_TRC_FAIL_INIT_BASE);
        return ERR_FAIL;
    }

    //初始化格式库相关对象
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

    //设置前后存盘路径
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
    //增加读入采集的根路径
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_COL_ROOT_DIR,
                         CFG_COL_ROOT_DIR_DEFAULT,
                         m_szCollectRootPath, MAX_PATH);
    //End


    //构造列表m_APInfoList
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

        //当CollectType大于0,表示是采集接入点
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

    //从配置文件中读取和查询浏览任务相关的参数
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
功能描述    结束工作处理函数
输入参数    无
输出参数    无
**********************************************************************/
void CBillQBServer::ExitInstance()
{
    CWorkModule::ExitInstance();

    //停止并删除所有的查询和浏览任务，清空任务列表
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

//获取本模块类型函数
MODULE_TYPE CBillQBServer::GetModuleType()
{
    return MT_BILLBROWSER;
}

/**********************************************************************
功能描述    定时器事件的处理入口函数
输入参数    nTimerID----定时器ID
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillQBServer::OnTimer(const long nTimerID)
{
    if (nTimerID == m_lBaseTimer)
    {
        //处理所有的查询浏览任务的超时
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
功能描述    命令解析和分发
输入参数    pMsgParsed----命令消息帧，调用函数创建和删除
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillQBServer::ParseMsg(MSG_INTRA* pMsgParsed)
{
    if (CMD_MML == pMsgParsed->CmdCode)
    {
        CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
        m_nCurClientNo = pMsgParsed->ClientNo;
/*
        //如果报告分析错误，则发送相关的错误消息报告
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
            //增加对采集话单的浏览处理
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

        //由于在MSG_INTRA中存放的只是CParseResult的指针，
        //所以必须在此释放CParseResult对象
        delete pParseResult;
    }
}

/**********************************************************************
功能描述    请求接入点信息的处理入口函数
输入参数    pMsgParsed----命令消息帧，调用函数创建和删除
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillQBServer::ReqAccessPointProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    //获取所有接入点名称信息
    LIST<STRING> APNameListTmp;
    m_pIQueryBrowse->GetAPNames(APNameListTmp);

    //构造并发送MML报告
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
功能描述    请求话单类型的处理入口函数
输入参数    pMsgParsed----命令消息帧，调用函数创建和删除
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillQBServer::ReqBillTypeProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    char* szAPName = (char* )pParseResult->GetStringVal(1);

    //构造并发送MML报告

    //获取待发送的表格的标题和内容
    LIST<STRING> TitleList;
    TitleList.push_back(S_QB_TT_BILL_TYPE);
    TitleList.push_back(S_QB_TT_AP_NAME);
    TitleList.push_back(S_QB_TT_TYPE);
    TitleList.push_back(S_QB_TT_SIZE);
    TitleList.push_back(S_QB_TT_MTIME);
    TitleList.push_back(S_QB_TT_CTIME);

    //根据接入点名称直接获取话单类型(0和1)并填写报告表格的内容
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
功能描述    请求通道信息的处理入口函数
输入参数    pMsgParsed----命令消息帧，调用函数创建和删除
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillQBServer::ReqChannelProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    //根据MML命令中的接入点名得到通道名
    char* szAPName = (char* )pParseResult->GetStringVal(1);
    LIST<STRING> ChlList;
    m_pIQueryBrowse->GetChlNames(szAPName, ChlList);

    //构造并发送MML报告

    //获取待发送的表格的标题和内容
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
        ContentList.push_back(S_FILE_TYPE_FOLDER);          //目录

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
功能描述    请求原始话单日期目录处理入口函数
输入参数    pMsgParsed----命令消息帧，调用函数创建和删除
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillQBServer::ReqOriginalDateProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    //根据MML命令中的接入点名得到原始话单的日期目录列表
    char* szAPName = (char* )pParseResult->GetStringVal(1);
    LIST<STRING> OrigDateList;
    char szPath[MAX_PATH];
    sprintf(szPath, "%s/%s", m_szOrigBasePath, szAPName);
    GetDateDir(szPath, OrigDateList);

    //构造并发送MML报告

    //获取待发送的表格的标题和内容
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
    char szBillType[10];    //话单类型，在此报告中一直为0（原始话单）
    sprintf(szBillType, "%d", 0);
    LIST<STRING>::iterator i;
    int nDateNo = 1;
    char szDateNo[10];    //日期目录的序号
    for (i = OrigDateList.begin(); i != OrigDateList.end(); i++)
    {
        sprintf(szDateNo, "%d", nDateNo);
        nDateNo++;

        ContentList.push_back(szDateNo);
        ContentList.push_back(*i);
        ContentList.push_back(szBillType);
        ContentList.push_back(szAPName);
        ContentList.push_back(S_FILE_TYPE_FOLDER);          //目录

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
功能描述    请求最终话单日期目录处理入口函数
输入参数    pMsgParsed----命令消息帧，调用函数创建和删除
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillQBServer::ReqFinalDateProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    //根据MML命令中的接入点名得到通道名列表
    char* szAPName = (char* )pParseResult->GetStringVal(1);
    LIST<STRING> ChlList, ChlDateList, ContentList;
    m_pIQueryBrowse->GetChlNames(szAPName, ChlList);

    //构造并发送MML报告

    //获取待发送的表格的标题和内容
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

    char szBillType[10];    //话单类型，在此报告中一直为1（最终话单）
    int nDateCount, nDateNo, nChlNo = 1;
    char szChlNo[10], szChlDir[MAX_PATH], szDateNo[10];
    sprintf(szBillType, "%d", 1);
    LIST<STRING>::iterator iChlIdx, iDateIdx;
    for (iChlIdx = ChlList.begin(); iChlIdx != ChlList.end(); iChlIdx++)
    {
        sprintf(szChlNo, "%d", nChlNo);
        nChlNo++;

        //根据产品名和通道名得到通道路径并获得该路径下的日期目录
        ChlDateList.clear();
        sprintf(szChlDir, "%s/%s/%s", m_szFinalBasePath,
                szAPName, (*iChlIdx).c_str());
        nDateCount = GetDateDir(szChlDir, ChlDateList);

        nDateNo = 1;

        //根据目录日期列表填写相关的表格内容
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

            ContentList.push_back(S_FILE_TYPE_FOLDER);          //目录

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
功能描述    请求话单文件列表处理入口函数
输入参数    pMsgParsed----命令消息帧，调用函数创建和删除
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillQBServer::ReqBillFilesProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    //从MML命令中得到各个参数并据此获得话单文件列表
    int nBillType;
    char* szAPName = (char* )pParseResult->GetStringVal(1);
    pParseResult->GetDigitVal(&nBillType, 2);
    char* szChlName = (char* )pParseResult->GetStringVal(3);
    char* szDateName = (char* )pParseResult->GetStringVal(4);
    char szFilePath[MAX_PATH];
    if (ORIG_BILL == nBillType) //原始话单
    {
        sprintf(szFilePath, "%s/%s/%s", m_szOrigBasePath, szAPName, szDateName);
    }
    else        //最终话单
    {
        sprintf(szFilePath, "%s/%s/%s/%s", m_szFinalBasePath, szAPName,
                szChlName, szDateName);
    }

    //获取指定目录下的所有文件和子目录
    LIST<SFILE_STATE> qResult;
    int nRet = GetFiles(szFilePath, qResult);

    //获取文件和子目录失败
    if(nRet == -1)
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                                 S_QB_TT_FAILED_READDIR);
    }

    //构造并发送MML报告

    LIST<STRING> TitleList;
    TitleList.push_back(S_QB_TT_NAME);
    TitleList.push_back(S_QB_TT_TYPE);
    TitleList.push_back(S_QB_TT_SIZE);
    TitleList.push_back(S_QB_TT_MTIME);
    TitleList.push_back(S_QB_TT_CTIME);

    //根路径内容列表
    LIST<STRING> ContentList;
    char szBuff[50];
    tm* pTimeTm = NULL;

    //组建MML报告
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

    //发送MML报告
    SendHoriTableReport(pParseResult, TitleList, ContentList,
                        s_QB_TH_LIST_BILL_FILE);
}

/**********************************************************************
功能描述    请求话单格式信息处理入口函数
输入参数    pMsgParsed----命令消息帧，调用函数创建和删除
返回值      无
抛出异常    无
**********************************************************************/
void CBillQBServer::ReqBillFormatProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    //从MML命令中得到接入点名
    char* szAPName = (char* )pParseResult->GetStringVal(1);

    //构造并发送MML报告

    //获取待发送的表格的标题和内容
    LIST<STRING> TitleList;
    TitleList.push_back(S_QB_TT_FORMAT_NAME);
    TitleList.push_back(S_QB_TT_FORMAT_DESC);
    TitleList.push_back(S_QB_TT_CHL_INFO);

    LIST<STRING> ContentList;
    //获得原始话单的相关格式
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
        ContentList.push_back("255");   //255表示原始话单

        iDescIdx++;
    }
    //获得最终话单各个通道下的相关格式
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
功能描述    请求查询条件处理入口函数
输入参数    pMsgParsed----命令消息帧，调用函数创建和删除
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillQBServer::ReqQueryCondProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    //从MML命令中获得格式名并得到相应的格式对象的所有域
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

    //构造并发送MML报告，采用纵向报文返回报告
    LIST<STRING> TitleList, ContentList, RecHeadList;
    TitleList.push_back(S_QB_TT_FIELD_NAME);
    TitleList.push_back(S_QB_TT_FIELD_DESC);
    TitleList.push_back(S_QB_TT_FIELD_TYPE);
    TitleList.push_back(S_QB_TT_FIELD_OPERATOR);
    TitleList.push_back(S_QB_TT_FIELD_UNIT);
    TitleList.push_back(S_QB_TT_ENUM_FLAG);
    TitleList.push_back(S_QB_TT_ENUM_VALUE);

    //遍历所有的域，根据所有用于查询的域来组织报文
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

        //该域不用于查询，则直接略过
        if (0 == pFieldInfo->flgQuery)
        {
            continue;
        }

        sprintf(szFieldType, "%d", pFieldInfo->nQueryCondDataType);
        //得到该域的枚举标志以及所有的枚举值
        if (0 == pFieldInfo->flgEnum)   //该域不是枚举值
        {
            sprintf(szEnumFlg, "%d", 0);
            szEnumValue[0] = '\0';
        }
        else
        {
            //此时szEnumFlg用来表示枚举值的个数，枚举值间用^号分开
            szEnumValue[0] = '\0';  //保证szEnumValue为空字符串
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

        //获取算子，一个域可能对应多个算子
        for (int i = 0; i < 6; i++)
        {
            //将算子的值转换成字符串
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

    //设置纵向表格中每条记录的标题
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
功能描述    请求话单浏览处理入口函数
输入参数    pMsgParsed----命令消息帧，调用函数创建和删除
输出参数    无
返回值      无
抛出异常    无
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

    //根据参数得到话单文件的存放路径
    char szFileFullPath[MAX_PATH];
    if (ORIG_BILL == nBillType)     //原始话单
    {
        sprintf(szFileFullPath, "%s/%s/%s/%s", m_szOrigBasePath, szAPName,
                szDateName, szFileName);
    }
    else                            //最终话单
    {
        sprintf(szFileFullPath, "%s/%s/%s/%s/%s", m_szFinalBasePath,
                szAPName, szChlName, szDateName, szFileName);
    }

    //根据窗口号和台号，遍历浏览任务列表，找出相应的任务对象
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

    //如果相应的任务不存在，则创建并启动相应的任务
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
功能描述    请求话单查询处理入口函数
输入参数    pMsgParsed----命令消息帧，调用函数创建和删除
输出参数    无
返回值      无
抛出异常    无
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

    
    //问题单 SWPD05145 YanYong 2004-05-25
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

    //根据参数获得要查询的话单的路径
    char szQueryPath[MAX_PATH];
    if ((ORIG_BILL == nBillType) && (DATE_RANGE == nFlg))
    //查询原始话单下的多个日期目录
    {
        sprintf(szQueryPath, "%s/%s", m_szOrigBasePath, szAPName);
    }
    else if ((ORIG_BILL == nBillType) && (DATE_RANGE != nFlg))
    //查询原始话单下的多个话单文件
    {
        sprintf(szQueryPath, "%s/%s/%s", m_szOrigBasePath,
                szAPName, szDateName);
    }
    else if ((ORIG_BILL != nBillType) && (DATE_RANGE == nFlg))
    //查询最终话单下的多个日期目录
    {
        sprintf(szQueryPath, "%s/%s/%s", m_szFinalBasePath,
                szAPName, szChlName);
    }
    else    //查询最终话单下的多个话单文件
    {
        sprintf(szQueryPath, "%s/%s/%s/%s", m_szFinalBasePath, szAPName,
                szChlName, szDateName);
    }

    //根据窗口号和台号，遍历查询任务列表，找出相应的任务对象
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

    //如果相应的任务不存在，则创建并启动相应的任务
    if (NULL == pTask)
    {
        //对查询条件进行特殊的处理，以便使从话单台送过来的查询条件
        //能满足格式库的需求
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
        //问题单 SWPD05145 YanYong 2004-05-25 
//        BOOL bSucc = pTask->Init(szQueryPath, szBegin, szEnd, nFlg,         
//                                 szCond, GetAPType(szAPName), nAPID);  // 问题单 SWPD05145 删除该行 YanYong 2004-05-26
        
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
功能描述    请求结束任务处理入口函数
输入参数    pMsgParsed----命令消息帧，调用函数创建和删除
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillQBServer::ReqCloseTaskProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    int nDeskNo = pMsgParsed->ClientNo;
    int nWndNo;
    pParseResult->GetDigitVal(&nWndNo, 1);

    LIST<CBillQBTask* >* pTaskList;
    //根据窗口号和台号，遍历查询浏览任务列表，找出相应的任务对象
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

    //不存在对应的任务，则返回给话单业务台"任务不存在"信息
    if (NULL == pTask)
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                             S_QB_RI_TASK_NOT_EXIST);
    }
    else    //否则，结束并删除任务
    {
        DestroyTask(pTask);
        pTaskList->erase(iTskIdx);
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 0,
                             S_QB_RI_SUCCESS_CLOSE_TASK);
    }
}

/**********************************************************************
功能描述    请求结束任务处理入口函数
输入参数    pMsgParsed----命令消息帧，调用函数创建和删除
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillQBServer::ReqStopTaskProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    int nDeskNo = pMsgParsed->ClientNo;
    int nWndNo;
    pParseResult->GetDigitVal(&nWndNo, 1);

    LIST<CBillQBTask* >* pTaskList;
    //根据窗口号和台号，遍历查询浏览任务列表，找出相应的任务对象
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

    //不存在对应的任务，则返回给话单业务台"任务不存在"信息
    if (NULL == pTask)
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                             S_QB_RI_TASK_NOT_EXIST);
    }
    else    //否则，结束任务
    {
        StopTask(pTask);
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 0,
                             S_QB_RI_SUCCESS_STOP_TASK);
    }
}

/**********************************************************************
功能描述    请求任务处理进度入口函数
输入参数    pMsgParsed----命令消息帧，调用函数创建和删除
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillQBServer::ReqTaskProgressProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    int nDeskNo = pMsgParsed->ClientNo;
    int nWndNo;
    pParseResult->GetDigitVal(&nWndNo, 1);

    LIST<CBillQBTask* >* pTaskList;
    //根据窗口号和台号，遍历查询浏览任务列表，找出相应的任务对象
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

    //不存在对应的任务，则返回给话单业务台"任务不存在"信息
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

        //得到话单页进度信息表格的标题和数据
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

        //构造MML报告并发送
        m_pMMLReport->MakeQBReportHead(pParseResult->GetCmdLine());
        m_pMMLReport->AddHoriTable(InfoTitleList, InfoContentList,
                                   S_QB_TH_PROGESS_INFO);
        m_pMMLReport->MakeReportTail();

        const char* szReport = m_pMMLReport->GetReport();
        SendMMLReport(szReport);

    }
}

/**********************************************************************
功能描述    请求页数据入口函数
输入参数    pMsgParsed----命令消息帧，调用函数创建和删除
输出参数    无
返回值      无
抛出异常    无
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
    //根据窗口号和台号，遍历查询浏览任务列表，找出相应的任务对象
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

    //不存在对应的任务，则返回给话单业务台"任务不存在"信息
    if (NULL == pTask)
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                             S_QB_RI_TASK_NOT_EXIST);
    }
    else
    {
        //得到话单数据表格的标题和数据
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

        //话单数据可能比较大，必须考虑分页的问题
        int nSubPageNo = 1;     //分页传送时的子页号
        while (!BillContentList.empty() || (1 == nSubPageNo))
        {
            //构造MML报告并发送
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
功能描述    请求窗口握手处理函数
输入参数    pMsgParsed----命令消息帧，调用函数创建和删除
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillQBServer::ReqWndShakeHandsProc(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
    int nDeskNo = pMsgParsed->ClientNo;
    int nWndNo;
    pParseResult->GetDigitVal(&nWndNo, 1);

    //根据窗口号和台号，找出相应的查询或浏览任务对象
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

    //根据相应的任务是否存在向话单台返回相应的报告并作相应处理
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
功能描述    销毁指定的查询浏览任务
输入参数    pTask----指定的任务
输出参数    无
返回值      无
抛出异常    无
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
功能描述    停止指定的查询浏览任务
输入参数    pTask----指定的任务
输出参数    无
返回值      无
抛出异常    无
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
功能描述    发送报告体中只有简单的消息的MML报告
输入参数    szCmd----MML报告对应的命令
            nRetCode----报告的错误码
            szInfo----报告体中的简单消息
输出参数    无
返回值      无
抛出异常    无
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
功能描述    发送报告体为一个横向表格的MML报告
输入参数    pParseResult----MML报告解析后的对象
            TitleList----表格的标题
            ContentList----表格的内容
            szTableHeadName----整个表格的名字
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillQBServer::SendHoriTableReport(CParseResult* pParseResult,
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList,
                        const char* szTableHeadName)
{
    //由于表格的数据可能会很大，所以必须考虑以分页的方式发送MML报告
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
功能描述    发送报告体为一个纵向表格的MML报告
输入参数    pParseResult----MML报告解析后的对象
            TitleList----表格的标题
            ContentList----表格的内容
            RecHeadList----纵向表格中各条记录的内容
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillQBServer::SendVertTableReport(CParseResult* pParseResult,
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList,
                        LIST<STRING>& RecHeadList)
{
    //由于表格的数据可能会很大，所以必须考虑以分页的方式发送MML报告
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
功能描述    发送没有话单内容的MML报告
输入参数    pParseResult----MML报告解析后的对象
            szNextPageExist----下一页是否存在
            szPageNo----当前页的页号
            szPercent----准备当前页数据的进度
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillQBServer::SendEmptyBillDataReport(CParseResult* pParseResult,
                            CBillQBTask* pTask,
                            const char* szNextPageExist,
                            const char* szPageNo, const char* szPercent)
{
    //获得话单页信息表格的标题和数据
    LIST<STRING> InfoTitleList, InfoContentList;
    InfoTitleList.push_back(S_QB_TT_HAVE_NEXT_PAGE);
    InfoTitleList.push_back(S_QB_TT_CUR_PAGE_NO);
    InfoTitleList.push_back(S_QB_TT_CUR_PERCENT);

    InfoContentList.push_back(szNextPageExist);
    InfoContentList.push_back(szPageNo);
    InfoContentList.push_back(szPercent);

    //得到话单数据表格的标题，数据为空
    LIST<STRING> BillTitleList, BillContentList;

    pTask->GetBillTitleList(BillTitleList);
    if (BillTitleList.empty())
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                             S_QB_RI_FAIL_GET_PAGE_DATA);
        return;
    }

    int nSubPageNo = 1;   //由于没有话单内容的情况下，不用分页传送，
                          //所以页码肯定为1
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
功能描述    将MML报告转换成MSG_INTRA的形式发送出去
输入参数    szReport----待发送的MML报告
输出参数    无
返回值      无
抛出异常    无
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
功能描述    得到接入点名对应的接入点的类型
输入参数    szReport----待发送的MML报告
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
int CBillQBServer::GetAPType(const char* szAPName)
{
    //遍历列表m_APInfoList，找出与szAPName对应的接入点的类型
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
功能描述    得到3G接入点对应的接入点号
输入参数    szAPName----接入点名称
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
int CBillQBServer::GetAPID(const char* szAPName)
{
    //遍历列表m_APInfoList，找出与szAPName对应的接入点的类型
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
    //遍历列表m_APInfoList，找出与szAPName对应的接入点号
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
    //遍历列表m_APInfoList，找出与szAPName对应的接入点号
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
功能描述    对话单查询条件进行特殊处理
输入参数    szCond----从话单台送过来的查询条件
            szBeginDate----从话单台送过来的起始日期
            szEndDate----从话单台送过来的结束日期
输出参数    szExCond----处理后的查询条件，空间在函数外申请和释放
返回值      无
抛出异常    无
备注        由于话单台超过来的查询条件中，开始时间的查询条件只包含时间域，
            但是在格式库的判断中，开始时间却必须包含日期和时间域。所以，
            必须将话单台发送的消息中的开始日期和结束日期分别加入到开始时间
            的查询条件中去。开始时间的域名定死为"StartTime"，时间的形式如
            2001-11-05 21:00:03所示，从话单台发送的消息中的日期如20011105
            所示，所以日期的显示方式必须进行相应的转换。例如，从话单台返回的
            查询条件中，开始时间的查询条件为"StartTime >= 08:05:00"、
            "StartTime <= 18:05:00"，开始日期和结束日期分别为"20011101"、
            "20011103"，则修改后的开始时间查询条件为
            "StartTime >= 2001-11-01 08:05:00"和
            "StartTime <= 2001-11-03 18:05:00"
**********************************************************************/
void CBillQBServer::TransQueryCond(const char* szCond,
                                const char* szBeginDate,
                                const char* szEndDate, char* szExCond)
{
    //修改开始日期和结束日期的格式
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

    //修改查询条件，将开始日期和结束日期插入

    char* pReadPos = (char* )szCond;    //进行字符串拷贝时的读指针
    szExCond[0] = '\0';
    char* pWritePos = (char* )szExCond; //进行字符串拷贝时的写指针
    char* pSchPos = (char* )szCond;     //用于字符串中查找的指针
    char cOperator;                     //StartTime后所带的算子的第一个字符

    //在szCond中查找所有的StartTime，如果StartTime后的算子为>=或>，
    //则在其条件中加入szExBeginDate，否则加入szEndDate
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

           //查找StartTime所带的算子的第一个字符
           while (1)
           {
               if (' ' != *pSchPos)
               {
                   cOperator = *pSchPos;
                   break;
               }
               pSchPos++;
           }

           //将开始日期或结束日期的信息加入到查询条件中
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
//增加对采集话单的浏览处理
void CBillQBServer::ReqFileRoot(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));

    //构造并发送MML报告
    LIST<STRING> TitleList;
    TitleList.push_back(S_QB_TT_NAME);

    //根路径内容列表
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

    //如果参数非法
    if(szPath == NULL)
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                                 S_QB_TT_INVALID_PATH);
    }

    //获取指定目录下的所有文件和子目录
    LIST<SFILE_STATE> qResult;
    int nRet = GetFiles(szPath, qResult);

    //获取文件和子目录失败
    if(nRet == -1)
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                                 S_QB_TT_FAILED_READDIR);
    }

    //构造并发送MML报告
    LIST<STRING> TitleList;
    TitleList.push_back(S_QB_TT_NAME);
    TitleList.push_back(S_QB_TT_TYPE);
    TitleList.push_back(S_QB_TT_SIZE);
    TitleList.push_back(S_QB_TT_MTIME);
    TitleList.push_back(S_QB_TT_CTIME);

    //根路径内容列表
    LIST<STRING> ContentList;
    char szBuff[50];
    tm* pTimeTm = NULL;

    //组建MML报告
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

    //发送MML报告
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

    //打开格式配置文件目录
    pDir = ACE_OS::opendir(szPath);
    if(pDir == NULL)
    {
        return -1;
    }

    int nCount = 0;
    SFILE_STATE FileState;
    char szFileFullPath[MAX_PATH];
    ACE_stat FileStat;

    //顺序获取找到的文件
    while ((pDirent = ACE_OS::readdir(pDir)) != NULL)
    {
        FileState.sFileName = pDirent->d_name;
        qTmpName.push_back(pDirent->d_name);

        SNPRINTF(szFileFullPath, MAX_PATH, "%s/%s", szPath, pDirent->d_name);
        ACE_OS::stat(szFileFullPath, &FileStat);

        FileState.nSize = FileStat.st_size;
        FileState.ctime = FileStat.st_ctime;
        FileState.mtime = FileStat.st_mtime;

        if (S_IFDIR == (FileStat.st_mode & S_IFMT))    //找到的是一个目录
        {
            //必须确保目录不是"."和".."
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
//问题单 SWPD05145  add by YanYong 2004-05-25
功能： 添加该函数，支持向指定的clientno客户端发送报文
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
//问题单 SWPD05145  add by YanYong 2004-05-25
功能： 添加该函数，支持向指定的clientno客户端发送报文
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
