#include "../utility/qb_report.h"
#include "cdr_stat_svr.h"
#include "stat_cfg_mgr.h"
#include "stat_rst_mgr.h"
#include "cdr_stat_tsk.h"
#include "cdr_stat_cfg.h"
#include "cdr_stat_rst.h"
#include "resource.h"


MAP<STRING, INFO_LIST> CCDRStatSvr::m_mapAPToParaContent;
/*******************************************************************
函数名称：CCDRStatSvr()
功能描述：构造函数,初始化成员变量
输入参数：
输出参数：无
返回内容：
调用函数：
被调函数：
作者时间：maimaoshi,2003/01/25
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
CCDRStatSvr::CCDRStatSvr()
{
    m_pCDRStatCfgMgr = NULL;
    m_pCDRStatRstMgr = NULL;
    m_pMMLReport     = NULL;
    m_pCDRStatTask   = NULL;
    m_tLastStatTime  = 0;
    m_tBeginTime     = 0;
    m_tEndTime       = 0;
    m_szBeginTime[0] = '\0';
}

/*******************************************************************
函数名称：~CCDRStatSvr()
功能描述：析构函数,删除指针成员变量
输入参数：
输出参数：无
返回内容：
调用函数：
被调函数：
作者时间：maimaoshi,2003/01/25
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
CCDRStatSvr::~CCDRStatSvr()
{

}
/*******************************************************************
函数名称：InitInstance()
功能描述：初始化函数
输入参数：
输出参数：无
返回内容：返回错误码(ERR_SUCCESS表示成功,ERR_FAIL表示失败)
调用函数：
被调函数：
作者时间：maimaoshi,2003/01/25
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
int CCDRStatSvr::InitInstance()
{
    int nRet = CWorkModule::InitInstance();
    if (nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();
    //读取接入点数
    m_uAPNum = IniFile.GetInt(CFG_SEC_COMMON,
                              CFG_COMMON_KEY_APCOUNT,
                              CFG_COMMON_KEY_APCOUNT_DEFAULT);
    //读取所有接入点的名称
    char szAPSec[25], szAPName[25];
    for (UINT4 i = 1; i <=  m_uAPNum; i++)
    {
        //接入点名称
        SNPRINTF(szAPSec, sizeof(szAPSec), "%s%d", CFG_SEC_AP, i);
        szAPSec[sizeof(szAPSec) - 1] = '\0';

        IniFile.GetString(szAPSec, CFG_AP_NAME, szAPSec, szAPName, sizeof(szAPName));
        szAPName[sizeof(szAPName) - 1] = '\0';

        m_APNameList.push_back(szAPName);
    }

    //读取每天开始统计的时间"HH:MM:SS"
    IniFile.GetString(CFG_SEC_CDRSTAT,
                      CFG_CDRSTAT_BEGINTIME,
                      CFG_CDRSTAT_BEGINTIME_DEFAULT,
                      m_szBeginTime,
                      sizeof(m_szBeginTime));
    m_szBeginTime[sizeof(m_szBeginTime) - 1] = '\0';
    IniFile.Close();

    struct tm when;
    sscanf(m_szBeginTime,
           "%d:%d:%d",
           &when.tm_hour,
           &when.tm_min,
           &when.tm_sec);
    
    SNPRINTF(m_szBeginTime, 
             sizeof(m_szBeginTime),
             "%02d:%02d:%02d",
             when.tm_hour,
             when.tm_min,
             when.tm_sec);

    //注册定时器
    m_lTimerID = SetTimer(ONE_SECOND_IN_MILISEC);

    m_pCDRStatCfgMgr = new CCDRStatCfgMgr;
    m_pCDRStatCfgMgr->Init(m_uAPNum);

    m_pCDRStatTask   = new CCDRStatTask;
    m_pCDRStatTask->Init(m_uAPNum);

    m_pCDRStatRstMgr = m_pCDRStatTask->GetCCDRStatRstMgr();

    m_pMMLReport   = new CQBMMLReport;

    return ERR_SUCCESS;
}
/*******************************************************************
函数名称：ExitInstance()
功能描述：退出处理函数
输入参数：
输出参数：无
返回内容：成功返回TRUE,失败返回FALSE
调用函数：
被调函数：
作者时间：maimaoshi,2003/01/25
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatSvr::ExitInstance()
{
    if (m_pCDRStatCfgMgr != NULL)
    {
        delete m_pCDRStatCfgMgr;
        m_pCDRStatCfgMgr = NULL;
    }


    if (m_pMMLReport != NULL)
    {
        delete m_pMMLReport;
        m_pMMLReport = NULL;
    }

    if (m_pCDRStatTask != NULL)
    {
        m_pCDRStatTask->WaitForExit();
        delete m_pCDRStatTask;
        m_pCDRStatTask = NULL;
    }

    //注销定时器
    KillTimer(m_lTimerID);

    CWorkModule::ExitInstance();
}

/*******************************************************************
函数名称：GetModuleType()
功能描述：获取本模块类型
输入参数：
输出参数：无
返回内容：本模块类型号
调用函数：
被调函数：
作者时间：maimaoshi,2003/01/25
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
MODULE_TYPE CCDRStatSvr::GetModuleType()
{
    return MT_CDRSTAT;
}

/*******************************************************************
函数名称：ParseMsg(MSG_INTRA* pMsgParsed)
功能描述：命令解析与分发
输入参数：MSG_INTRA* pMsgParsed,命令帧
输出参数：无
返回内容：无
调用函数：1、ProcListQueryPara(),查询统计可选域信息
          2、ProcListStatItem(),查询统计项目配置信息
          3、ProcSetStatItem(),设置统计项目配置信息
          4、ProcBrowseStatRst(),查询统计结果信息
被调函数：
作者时间：maimaoshi,2003/01/25
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatSvr::ParseMsg(MSG_INTRA* pMsgParsed)
{
    //分析CMD_MML命令
    CParseResult* pParseResult = NULL;

    switch (pMsgParsed->CmdCode)
    {
        case CMD_MML:
        {
            pParseResult   = *((CParseResult**)(pMsgParsed->Data));
            m_nCurClientNo = pMsgParsed->ClientNo;

            switch (pParseResult->GetCmdCode())
            {
            case MML_LST_STATPARA:
                //查询统计可选域信息
                ProcListQueryPara(pMsgParsed);
                break;
            case MML_LST_STATITEM:
                //查询统计项目配置信息
                ProcListStatItem(pMsgParsed);
                break;
            case MML_SET_STATITEM:
                //设置统计项目配置信息
                ProcSetStatItem(pMsgParsed);
                break;
            case MML_BRW_STATINFO:
                //查询统计结果信息
                ProcBrowseStatRst(pMsgParsed);
                break;
            default:
                break;
            }

            break;
        }

        default:
        {
            CWorkModule::ParseMsg(pMsgParsed);
            break;
        }
    }
}

/*******************************************************************
函数名称：ProcListStatItem(MSG_INTRA* pMsgParsed)
功能描述：处理统计项目查询请求
输入参数：MSG_INTRA* pMsgParsed,命令帧
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/01/27
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatSvr::ProcListStatItem(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));

    //构造并发送MML报告
    //获取待发送的表格的标题
    LIST<STRING> TitleList;
    TitleList.push_back(s_CDRSTAT_AP_NAME);
    TitleList.push_back(s_CDRSTAT_STATITEM_NAME);
    TitleList.push_back(s_CDRSTAT_SUBITEM_NAME);
    TitleList.push_back(s_CDRSTAT_SUBITEM_EXPRESS);

    LIST<STRING> ContentList;

    STRING sAPName;
    //dwq 修改接口 2003-11-18
    //LIST<char* > listStatItemName;
    LIST<STRING> listStatItemName;
    //dwq 修改结束 2003-11-18

    LIST<SOneStatSubItemCfg*> subItemList;
    LIST<ICDRStatCfg*> listStatCfg;

    m_pCDRStatCfgMgr->GetStatCfgList(listStatCfg);

    if (listStatCfg.size() == 0)
    {
        //没有任何配置信息，返回无法获得配置接口的报文
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1, S_CDRSTAT_FAIL_GET_CFGINTERFACE);
        return;
    }

    //组装返回报文
    LIST<ICDRStatCfg*>::iterator it_cfg = listStatCfg.begin();

    while (it_cfg != listStatCfg.end())
    {
        //对每个接入点进行处理
        ICDRStatCfg* pTmpCfg = *it_cfg;

        //获得接入点名称
        sAPName = pTmpCfg->GetAccessPointName();

        //获得此接入点的统计项列表
        pTmpCfg->GetStatItemNameList(listStatItemName);

        //如果此接入点还没有任何配置信息
        if (listStatItemName.size() == 0)
        {
            //只把接入点名称返回
            //ap   item      subitem       exp
            //3G    -           -           -
            ContentList.push_back(sAPName);
            ContentList.push_back("-");
            ContentList.push_back("-");
            ContentList.push_back("-");
        }
        else
        {
            //根据统计项找出它的所有统计子项和表达式

            //dwq 修改接口 2003-11-18
            //LIST<char* >::iterator it_name = listStatItemName.begin();
            LIST<STRING>::iterator it_name = listStatItemName.begin();
            //dwq 修改结束 2003-11-18

            while (it_name != listStatItemName.end())
            {
                //依次找出每个统计项的子项列表

                pTmpCfg->GetStatItem(it_name->c_str(), subItemList);

                LIST<SOneStatSubItemCfg*>::iterator it_onesub = subItemList.begin();

                //找出每一个统计子项的信息
                while (it_onesub != subItemList.end())
                {
                    //ap   item      subitem       exp
                    //3G  统计项    统计子项1      a>1
                    ContentList.push_back(sAPName);
                    ContentList.push_back((*it_onesub)->szStatItemName);
                    ContentList.push_back((*it_onesub)->szStatSubItemName);
                    ContentList.push_back((*it_onesub)->szStatSubItemCond);

                    //清除subItemList
                    delete *it_onesub;
                    it_onesub++;
                }
                subItemList.clear();

                it_name++;
            }
            listStatItemName.clear();
        }
        it_cfg++;
    }

    SendHoriTableReport(pParseResult, TitleList, ContentList, S_CDRSTAT_TH_LIST_STAT_CFG);

}

/*******************************************************************
函数名称：ProcListQueryPara(MSG_INTRA* pMsgParsed)
功能描述：处理统计可选域信息查询请求
输入参数：MSG_INTRA* pMsgParsed,命令帧
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/01/27
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatSvr::ProcListQueryPara(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult**)(pMsgParsed->Data));

    //从MML命令中得到接入点名称
    STRING sAPName = pParseResult->GetStringVal(1);

    //构造并发送MML报告，采用纵向报文返回报告
    LIST<STRING> TitleList, ContentList, RecHeadList, ParaFieldList;

    TitleList.push_back(S_CDRSTAT_FIELD_NAME);
    TitleList.push_back(S_CDRSTAT_FIELD_DESC);
    TitleList.push_back(S_CDRSTAT_FIELD_TYPE);
    TitleList.push_back(S_CDRSTAT_FIELD_OPERATOR);
    TitleList.push_back(S_CDRSTAT_FIELD_UNIT);
    TitleList.push_back(S_CDRSTAT_ENUM_FLAG);
    TitleList.push_back(S_CDRSTAT_ENUM_VALUE);

    ContentList = m_mapAPToParaContent[sAPName];

    //设置纵向表格中每条记录的标题
    int nRecCount = ContentList.size() / TitleList.size();
    char szRecHead[30];
    for (int nRecNo = 1; nRecNo <= nRecCount; nRecNo++)
    {
        sprintf(szRecHead, S_CDRSTAT_QUERY_COND, nRecNo);
        RecHeadList.push_back(szRecHead);
    }

    SendVertTableReport(pParseResult, TitleList, ContentList, RecHeadList);

}

/*******************************************************************
函数名称：ProcSetStatItem(MSG_INTRA* pMsgParsed)
功能描述：处理统计项目设置请求
输入参数：MSG_INTRA* pMsgParsed,命令帧
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/01/27
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatSvr::ProcSetStatItem(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult**)(pMsgParsed->Data));
    //从MML命令中得到统计配置信息
    char* szCfgInfo = (char* )pParseResult->GetStringVal(1);

	char* pfind = strchr(szCfgInfo, '~');
	if (pfind)
	{
		*pfind = '^';
		m_szInfo = "";
	}
    m_szInfo += szCfgInfo;
    int nPos = m_szInfo.find('!');
    if (( nPos != STRING::npos) && (nPos >0))
    {
        m_szInfo = m_szInfo.substr(0, nPos);
        m_szInfo += "^";
    }
    else
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 0, "");
        return;
    }
    
    BOOL bIsSuccess = TRUE;
    ONESTATSUBITEMCFG_LIST subItemList;
    MAP<STRING, ONESTATSUBITEMCFG_LIST> mapAPNameStatCfg;

    char szText[1024*5];
    sprintf(szText, "%s\"", m_szInfo.c_str());
    szText[m_szInfo.size()] = 0;

	m_szInfo = "";
    ParseCfgInfo(szText, mapAPNameStatCfg);

    LIST<ICDRStatCfg*> listStatCfg;

    m_pCDRStatCfgMgr->GetStatCfgList(listStatCfg);

    LIST<ICDRStatCfg*>::iterator it_cfg = listStatCfg.begin();

    while (it_cfg != listStatCfg.end())
    {
        //对每个接入点进行处理
        ICDRStatCfg* pTmpCfg = *it_cfg;

        const char* szAPName = pTmpCfg->GetAccessPointName();

        //通过接入点名称找到其映射的LIST<SOneStatSubItemCfg*>
        subItemList = mapAPNameStatCfg[szAPName];

        if (pTmpCfg->SetStatCfg(subItemList) == ERR_FAIL)
        {
            bIsSuccess = FALSE;
        }

        it_cfg++;
    }

    if (bIsSuccess)
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 0, S_ALARM_OPRSUCCESS);
    }
    else
    {
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1, S_ALARM_OPRFAIL);
    }

    LIST<ICDRStatRst*> listStatRst;

    m_pCDRStatRstMgr->GetStatRstList(listStatRst);

    LIST<ICDRStatRst*>::iterator it_rst = listStatRst.begin();

    while (it_rst != listStatRst.end())
    {
        //对每个接入点进行处理
        ICDRStatRst* pTmpRst = *it_rst;

        const char* szAPName = pTmpRst->GetAccessPointName();

        //通过接入点名称找到其映射的LIST<SOneStatSubItemRst*>
        subItemList = mapAPNameStatCfg[szAPName];

        LIST<char* > StatExpList;
        //从subItemList取出所有的表达式
        LIST<SOneStatSubItemCfg*>::iterator it_onesub;
        for(it_onesub = subItemList.begin(); it_onesub != subItemList.end(); it_onesub++)
        {
            StatExpList.push_back((*it_onesub)->szStatSubItemCond);
        }

        //设置所有表达式
        pTmpRst->AddStatExpress(StatExpList);

        //调用CCDRStatTask::BuildMap_AP_FMT_EXP
        m_pCDRStatTask->BuildMap_AP_FMT_EXP(szAPName);

        it_rst++;
    }
}

enum Period
{
    CYCLE_YEAR = 0,
    CYCLE_MONTH,
    CYCLE_DAY
};

/*******************************************************************
函数名称：ProcBrowseStatRst(MSG_INTRA* pMsgParsed)
功能描述：处理统计数据浏览请求
输入参数：MSG_INTRA* pMsgParsed,命令帧
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/01/27
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatSvr::ProcBrowseStatRst(MSG_INTRA* pMsgParsed)
{
    int nPeriod;
    CParseResult* pParseResult = *((CParseResult**)(pMsgParsed->Data));

    //接入点名称
    char* szAPName    = (char* )pParseResult->GetStringVal(1);
    //统计周期
    pParseResult->GetDigitVal(&nPeriod, 2);
    //统计周期开始时间
    char* szStartTime = (char* )pParseResult->GetStringVal(3);
    //统计周期结束时间
    char* szEndTime   = (char* )pParseResult->GetStringVal(4);
    //统计项名称
    char* szStatItem  = (char* )pParseResult->GetStringVal(5);

    //通过统计项名称找到统计子项的名称和表达式
    ICDRStatCfg* pStatCfg = NULL;
    pStatCfg = m_pCDRStatCfgMgr->GetStatCfgInterface(szAPName);
    LIST<SOneStatSubItemCfg*> subItemList;
    pStatCfg->GetStatItem(szStatItem, subItemList);

    if (subItemList.size() == 0)
    {
        //没有任何配置信息，返回无法获得配置接口的报文
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1, S_CDRSTAT_FAIL_GET_CFGINTERFACE);
        return;
    }

    //属于同一个统计项的所有表达式
    LIST<char*> ExpressList;
    //获取待发送的表格的标题
    LIST<STRING> TitleList;

    //统计周期
    if (nPeriod == CYCLE_YEAR)
    {
        TitleList.push_back(s_CDRSTAT_STAT_CYCLE_YEAR);
    }
    else if (nPeriod == CYCLE_MONTH)
    {
        TitleList.push_back(s_CDRSTAT_STAT_CYCLE_MONTH);
    }
    else
    {
        TitleList.push_back(s_CDRSTAT_STAT_CYCLE_DAY);
    }

    //获得统计项名称，找出属于它的子项的名称，加到标题中
    LIST<SOneStatSubItemCfg*>::iterator it_onecfg = subItemList.begin();
    while (it_onecfg != subItemList.end())
    {
        TitleList.push_back((*it_onecfg)->szStatSubItemName);

        ExpressList.push_back((*it_onecfg)->szStatSubItemCond);
        it_onecfg++;
    }

    VECTOR<STRING> CycleName;
    VECTOR<time_t> StartTime, EndTime;

    //获得和周期相关的信息
    ConvertCycle(nPeriod,
                 szStartTime,
                 szEndTime,
                 CycleName,
                 StartTime,
                 EndTime);

    char szCDRCount[16];
    LIST<STRING> ContentList;
    LIST<char*>::iterator it_exp;
    ICDRStatRst* pTmpRst = NULL;
    UINT4 nCDRCount = 0, nValueTotal = 0;


    pTmpRst = m_pCDRStatRstMgr->GetStatRstInterface(szAPName);

    //周期循环
    for (UINT4 i = 0; i < CycleName.size(); i++)
    {
        ContentList.push_back(CycleName[i]);

        it_exp  = ExpressList.begin();
        //取出每个表达式
        while (it_exp != ExpressList.end())
        {
            nCDRCount = 0;
            nValueTotal = 0;

            //取得统计值
            if (ERR_FAIL == (pTmpRst->GetStatExpRst((const char*)(*it_exp),
                                                    StartTime[i],
                                                    EndTime[i],
                                                    nCDRCount,
                                                    nValueTotal)))
            {
                nCDRCount = 0;
            }
            //添加到统计内容列表
            sprintf(szCDRCount, "%d", nCDRCount);
            ContentList.push_back(szCDRCount);
            it_exp++;
        }
    }

    SendHoriTableReport(pParseResult, TitleList, ContentList, S_CDRSTAT_TH_LIST_STAT_RESULT);

}

/*******************************************************************
函数名称：OnTimer(const long nTimerID)
功能描述：定时器事件处理函数
输入参数：const long nTimerID,
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/14
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatSvr::OnTimer(const long nTimerID)
{
    if (nTimerID == m_lTimerID)
    {
        //触发统计任务的统计工作
        char szToday[20];
        time_t curtime = ACE_OS::time(NULL);
        ACE_OS::strftime(szToday, 9, "%H:%M:%S", ACE_OS::localtime(&curtime));
	char szDate[30];
	ACE_OS::strftime(szDate, 12, "%Y-%m-%d ", ACE_OS::localtime(&curtime));
	strcat(szDate, m_szBeginTime);
        szToday[19] = '\0';
	szDate[20] = '\0';

        if (strcmp(szToday, m_szBeginTime) >= 0)
        {
            if (GetLicenseCount())
            {
                if (curtime >= m_tLastStatTime + 24*3600)
                {
                    TRACE(MTS_CDRSTAT, S_START_CDRSTAT_TASK);
                    m_pCDRStatTask->ActivateNextStatOper();
		    m_tLastStatTime = ConvertTimeToTimeT(szDate);
                }
            }
            else
            {
                //TRACE(MTS_CDRSTAT, S_NO_CDRSTAT_LICENSE);
            }
        }
    }
    else
    {
        CWorkModule::OnTimer(nTimerID);
    }
}

/*******************************************************************
函数名称：GetBillFiles(const char* szPath, LIST<STRING>& qResult)
功能描述：获得某目录的所有文件名
输入参数：const char* szPath, 路径名
输出参数：LIST<STRING>& qResult,此路径下的所有话单文件列表
返回内容：返回文件个数
调用函数：
被调函数：
作者时间：maimaoshi,2003/01/27
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
int CCDRStatSvr::GetBillFiles(const char* szPath, LIST<STRING>& qResult)
{
    //为了在话单台上按照时间正确的显示，所找到的文件必须按照修改时间进行排序,
    //为此先将找到的话单文件信息放入到FileInfoList列表，然后对FileInfoList
    //按照修改时间进行排序，从而得到按照修改时间排序的话单文件名的列表
    LIST<SFileNameNode> FileInfoList;
    SFileNameNode TmpNode;

    int nCount = 0;
    ACE_Dirent FindDir((const ACE_TCHAR* )szPath);

    //搜索指定的目录，找出目录下所有的文件，并加入到qResult
    dirent* pDirent;
    ACE_stat FileInfo;
    char szFileFullPath[MAX_PATH], szFileName[MAX_PATH];
    pDirent = FindDir.read();
    while(pDirent)
    {
        strcpy(szFileName, pDirent->d_name);
        sprintf(szFileFullPath, "%s/%s", szPath, szFileName);
        ACE_OS::stat(szFileFullPath, &FileInfo);

        //如果找到的是一个文件，则符合要求
        if (S_IFDIR != (FileInfo.st_mode & S_IFMT))
        {
            strcpy(TmpNode.szFileName, szFileName);
            TmpNode.tFileTime = FileInfo.st_mtime;
            FileInfoList.push_back(TmpNode);
            nCount++;
        }

        pDirent = FindDir.read();
    }

    //按照修改时间对FileInfoList进行排序，然后据此得到排序后的话单文件名列表
    FileInfoList.sort();
    LIST<SFileNameNode>::iterator i;
    for (i = FileInfoList.begin(); i != FileInfoList.end(); i++)
    {
        qResult.push_back((*i).szFileName);
    }

    return nCount;
}

/*******************************************************************
函数名称：ConvertCycle(int nPeriod, char* szStartTime, char* szEndTime, VECTOR<STRING>& CycleName, VECTOR<time_t>& StartTime, VECTOR<time_t>& EndTime)
功能描述：获得和周期相关的信息
输入参数：int nPeriod, char* szStartTime, char* szEndTime
输出参数：VECTOR<STRING>& CycleName, VECTOR<time_t>& StartTime, VECTOR<time_t>& EndTime
返回内容：返回0表示成功，返回-1表示失败
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
int CCDRStatSvr::ConvertCycle(int nPeriod,
                              char* szStartTime,
                              char* szEndTime,
                              VECTOR<STRING>& CycleName,
                              VECTOR<time_t>& StartTime,
                              VECTOR<time_t>& EndTime)
{
    char szCycle[16];
    int nYear = 0, nMonth = 0, nDay = 0;
    struct tm startTm, endTm, tmpTm;
    StrToTm((const char*)szStartTime, startTm);
    StrToTm((const char*)szEndTime, endTm);

    //memcpy(&tmpTm, &startTm,sizeof(tm));
    switch (nPeriod)
    {
    case CYCLE_YEAR:
        //如果是按照年周期取统计结果
        for (nYear = startTm.tm_year; nYear <=  endTm.tm_year; nYear++)
        {
            sprintf(szCycle, "%04d", nYear + 1900);
            CycleName.push_back(szCycle);

            //周期开始时间
			memset(&tmpTm, 0, sizeof(tmpTm));
            tmpTm.tm_year = nYear;
            if (nYear == startTm.tm_year)
            {
                tmpTm.tm_mon  = startTm.tm_mon;
                tmpTm.tm_mday = startTm.tm_mday;
            }
            else
            {
                tmpTm.tm_mon  = 1;
                tmpTm.tm_mday = 1;
            }

            tmpTm.tm_hour = 0;
            tmpTm.tm_min  = 0;
            tmpTm.tm_sec  = 0;
            StartTime.push_back(mktime(&tmpTm));

            //周期结束时间
            if (nYear ==  endTm.tm_year)
            {
                tmpTm.tm_mon  = endTm.tm_mon;
                tmpTm.tm_mday = endTm.tm_mday;
            }
            else
            {
                tmpTm.tm_mon  = 12;
                tmpTm.tm_mday = 31;
            }
            tmpTm.tm_hour = 23;
            tmpTm.tm_min  = 59;
            tmpTm.tm_sec  = 59;
            EndTime.push_back(mktime(&tmpTm));
        }
        break;
    case CYCLE_MONTH:
        //如果是按照月周期取统计结果
        for (nYear = startTm.tm_year; nYear <=  endTm.tm_year; nYear++)
        {
            for(nMonth = 0; nMonth <= 11; nMonth++)
            {
                if ((nYear == startTm.tm_year) && (nMonth < startTm.tm_mon))
                {
                    continue;
                }

                sprintf(szCycle, "%04d-%02d", nYear + 1900, nMonth + 1);
                CycleName.push_back(szCycle);

				memset(&tmpTm, 0, sizeof(tmpTm));
                tmpTm.tm_year = nYear;
                tmpTm.tm_mon  = nMonth;

                //周期开始时间
                if ((nMonth == startTm.tm_mon) && (nYear == startTm.tm_year))
                {
                    tmpTm.tm_mday = startTm.tm_mday;
                }
                else
                {
                    tmpTm.tm_mday = 1;
                }
                tmpTm.tm_hour = 0;
                tmpTm.tm_min  = 0;
                tmpTm.tm_sec  = 0;
                StartTime.push_back(mktime(&tmpTm));

                //周期结束时间
                if ((nMonth == endTm.tm_mon) && (nYear == endTm.tm_year))
                {
                    tmpTm.tm_mday = endTm.tm_mday;
                }
                else
                {
                    tmpTm.tm_mday = GetDay(nYear, nMonth+1);
                }

                tmpTm.tm_hour = 23;
                tmpTm.tm_min  = 59;
                tmpTm.tm_sec  = 59;
                EndTime.push_back(mktime(&tmpTm));

                if ((nYear == endTm.tm_year) && (nMonth == endTm.tm_mon))
                {
                    break;
                }
            }
        }
        break;
    case CYCLE_DAY:
        //如果是按照日周期取统计结果
        for (nYear = startTm.tm_year; nYear <=  endTm.tm_year; nYear++)
        {
            for(nMonth = 0; nMonth <= 11; nMonth++)
            {
                if ((nYear == startTm.tm_year) && (nMonth < startTm.tm_mon))
                {
                    continue;
                }

                for(nDay = 1; nDay <= GetDay(nYear, nMonth+1); nDay++)
                {
                    if ((nYear == startTm.tm_year)
                        && (nMonth == startTm.tm_mon)
                        && (nDay < startTm.tm_mday))
                    {
                        continue;
                    }

                    sprintf(szCycle, "%04d-%02d-%02d", nYear + 1900, nMonth + 1, nDay);
                    CycleName.push_back(szCycle);

					memset(&tmpTm, 0, sizeof(tmpTm));
                    tmpTm.tm_year = nYear;
                    tmpTm.tm_mon  = nMonth;
                    tmpTm.tm_mday = nDay;
                    tmpTm.tm_hour = 0;
                    tmpTm.tm_min  = 0;
                    tmpTm.tm_sec  = 0;

                    StartTime.push_back(mktime(&tmpTm));

                    tmpTm.tm_hour = 23;
                    tmpTm.tm_min  = 59;
                    tmpTm.tm_sec  = 59;

                    EndTime.push_back(mktime(&tmpTm));

                    if ((nYear == endTm.tm_year)
                        && (nMonth == endTm.tm_mon)
                        && (nDay == endTm.tm_mday))
                    {
                        break;
                    }
                }

                if ((nYear == endTm.tm_year) && (nMonth == endTm.tm_mon))
                {
                    break;
                }
            }
        }
        break;
    default:
        return -1;
    }
    return 0;
}

/*******************************************************************
函数名称：StrToTm(const char* szSrc, tm &tmTar)
功能描述：把字串转换为tm结构
输入参数：const char* szSrc, 含有日期信息的字符串
输出参数：tm &tmTar,转换后的日期结构
返回内容：返回0表示成功，返回-1表示失败
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
int CCDRStatSvr::StrToTm(const char* szSrc, tm &tmTar)
{
    int nYear = 0, nMon = 0, nDay = 0;
    int nHour = 0, nMin = 0, nSec = 0;

    char szTemp[10];

    if(NULL == szSrc)
    {
        return -1;
    }

    //字节数不匹配，则返回失败
    if(strlen(szSrc) != 8)
    {
        return -1;
    }

    //取年
    memcpy(szTemp, szSrc, 4);
    szTemp[4] = 0;
    nYear = atoi(szTemp);

    //取月
    memcpy(szTemp, szSrc+4, 2);
    szTemp[2] = 0;
    nMon = atoi(szTemp);

    //取日
    memcpy(szTemp, szSrc+6, 2);
    szTemp[2] = 0;
    nDay = atoi(szTemp);

    tmTar.tm_year = nYear - 1900;
    tmTar.tm_mon  = nMon - 1;
    tmTar.tm_mday = nDay;
    tmTar.tm_hour = nHour;
    tmTar.tm_min  = nMin;
    tmTar.tm_sec  = nSec;

    return 0;
}
/*******************************************************************
函数名称：GetDay(int nYear, int nMonth)
功能描述：由年、月获得天数
输入参数：int nYear,年
          int nMonth,月
输出参数：无
返回内容：返回日的数目
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
int CCDRStatSvr::GetDay(int nYear, int nMonth)
{
    int nDay = 0;
    switch (nMonth)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        nDay = 31;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        nDay = 30;
        break;
    case 2:
        if ((nYear % 400 == 0) || ((nYear % 4 == 0) && (nYear % 100 != 0)))
        {
            nDay = 29;
        }
        else
        {
            nDay = 28;
        }
        break;
    default:
        break;
    }
    return nDay;
}

/*******************************************************************
函数名称：ParseCfgInfo(char* szCfgInfo, MAP<STRING, ONESTATSUBITEMCFG_LIST>& mapAPNameStatCfg)
功能描述：分解配置信息"3G,item2,item21,exp21;3G,item2,item21,exp21;GPRS,item1,item11,exp11;GPRS,item2,item21,exp21;"
输入参数：char* szCfgInfo,配置信息
输出参数：MAP<STRING, ONESTATSUBITEMCFG_LIST>& mapAPNameStatCfg,接入点和它配置子项列表映射
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatSvr::ParseCfgInfo(char* szCfgInfo,
                               MAP<STRING, ONESTATSUBITEMCFG_LIST>& mapAPNameStatCfg)
{
    char szStatCfg[656]    = "";
    char szAPName[16]      = "";
    char szItemName[64]    = "";
    char szSubItemName[64] = "";
    char szExpress[512]    = "";
    memset(szStatCfg, 0, sizeof(szStatCfg));
    memset(szAPName, 0, sizeof(szAPName));
    memset(szItemName,0, sizeof(szItemName));
    memset(szSubItemName, 0, sizeof(szSubItemName));
    memset(szExpress, 0, sizeof(szExpress));

    STRING sAPName = "";
    int i = 0, nOffset = 0;
    SOneStatSubItemCfg*     pSubItemCfg  = NULL;
    ONESTATSUBITEMCFG_LIST* pSubItemList = NULL;
    ONESTATSUBITEMCFG_LIST  SubItemList;

    while (szCfgInfo[i] != '\0')
    {
        if (szCfgInfo[i] == '^')
        {
            //拆离szStatCfg
            memcpy(szStatCfg, szCfgInfo + nOffset, i - nOffset + 1);
            GetCfgItem(szStatCfg, szAPName, szItemName, szSubItemName, szExpress);

            if (mapAPNameStatCfg.find(STRING(szAPName)) !=  mapAPNameStatCfg.end())
            {
                SubItemList = mapAPNameStatCfg[STRING(szAPName)];

                //如果在统计项，统计子项和表达式中有任何一个是空的就不保存
                if ((strlen(szItemName) > 0)
                    && (strlen(szSubItemName) > 0)
                    && (strlen(szExpress) > 0))
                {
                    pSubItemCfg = new SOneStatSubItemCfg;
                    memcpy(pSubItemCfg->szStatItemName, szItemName, 64);
                    memcpy(pSubItemCfg->szStatSubItemName, szSubItemName, 64);
                    memcpy(pSubItemCfg->szStatSubItemCond, szExpress, 512);

                    SubItemList.push_back(pSubItemCfg);
                    mapAPNameStatCfg[STRING(szAPName)] = SubItemList;
                }
                else
                {
                    //写Trace()
                    continue;
                }

            }else
            {
                //如果接入点名称不同
                if (strcmp(szAPName, sAPName.c_str()) != 0)
                {
                    //第一次进入
                    if (pSubItemList == NULL)
                    {
                        pSubItemList = new ONESTATSUBITEMCFG_LIST;
                    }
                    else
                    {
                        //提交时要检查List是否有元素
                        if (pSubItemList->size() > 0)
                        {
                            mapAPNameStatCfg[sAPName] = *pSubItemList;
                        }
                        else
                        {
                            delete pSubItemList;
                        }

                        pSubItemList = new ONESTATSUBITEMCFG_LIST;
                    }

                    sAPName = szAPName;
                }

                //如果在统计项，统计子项和表达式中有任何一个是空的就不保存
                if ((strlen(szItemName) > 0)
                    && (strlen(szSubItemName) > 0)
                    && (strlen(szExpress) > 0))
                {
                    pSubItemCfg = new SOneStatSubItemCfg;
                    memcpy(pSubItemCfg->szStatItemName, szItemName, 64);
                    memcpy(pSubItemCfg->szStatSubItemName, szSubItemName, 64);
                    memcpy(pSubItemCfg->szStatSubItemCond, szExpress, 512);

                    pSubItemList->push_back(pSubItemCfg);
                }
                else
                {
                    //写Trace()
                    continue;
                }
            }

            nOffset  = i + 1;

            memset(szStatCfg, 0, sizeof(szStatCfg));
            memset(szAPName, 0, sizeof(szAPName));
            memset(szItemName,0, sizeof(szItemName));
            memset(szSubItemName, 0, sizeof(szSubItemName));
            memset(szExpress, 0, sizeof(szExpress));
        }

        i++;
    }

    if (pSubItemList != NULL)
    {
        //提交时要检查List是否有元素
        if (pSubItemList->size() > 0)
        {
            mapAPNameStatCfg[sAPName] = *pSubItemList;
        }
        else
        {
            delete pSubItemList;
        }
    }
}

/*******************************************************************
函数名称：GetCfgItem(char* szStatCfg, char* szApName, char* szItemName, char* szSubItemName, char* szExpress)
功能描述：分解一条配置信息
输入参数：char* szStatCfg,一条配置信息
输出参数：char*& szApName, 配置信息中的接入点名称
          char*& szItemName, 配置信息中统计项名称
          char*& szSubItemName, 配置信息中统计子项名称
          char*& szExpress,配置信息中表达式
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatSvr::GetCfgItem(char* szStatCfg,
                             char* szApName,
                             char* szItemName,
                             char* szSubItemName,
                             char* szExpress)
{
    int i = 0, nOffset = 0, nFlag = 0;
    while (szStatCfg[i] != '^')
    {
        if (szStatCfg[i] == ',')
        {
            switch (nFlag)
            {
            case 0:
                //取接入点名称
                memcpy(szApName, szStatCfg + nOffset, i - nOffset);
                break;
            case 1:
                //取统计名称
                nOffset++;
                memcpy(szItemName, szStatCfg + nOffset, i - nOffset);
                break;
            case 2:
                //取统计子项名称
                nOffset++;
                memcpy(szSubItemName, szStatCfg + nOffset, i - nOffset);
                break;
            default:
                break;
            }

            nOffset = i;
            nFlag++;
        }
        i++;
    }
    //最后取表达式
    nOffset++;
    memcpy(szExpress, szStatCfg + nOffset, i - nOffset);
}

/*******************************************************************
函数名称：SendSimpleInfoReport(const char* szCmd, int nRetCode, const char* szInfo)
功能描述：发送报告体中只有简单的消息的MML报告
输入参数：
   参数1：const char* szCmd,MML报告对应的命令
   参数2：int nRetCode,报告的错误码
   参数3：const char* szInfo,报告体中的简单消息
输出参数：无
返回内容：
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatSvr::SendSimpleInfoReport(const char* szCmd, int nRetCode,
                                         const char* szInfo)
{
    m_pMMLReport->MakeQBReportHead(szCmd, nRetCode);
    m_pMMLReport->AddString(szInfo);
    m_pMMLReport->AddString("\r\n");
    m_pMMLReport->MakeReportTail();
    const char* szReport = m_pMMLReport->GetReport();

    SendMMLReport(szReport);
}


/*******************************************************************
函数名称：SendHoriTableReport(CParseResult* pParseResult,
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList,
                        const char* szTableHeadName)
功能描述：发送报告体为一个横向表格的MML报告
输入参数：
   参数1：CParseResult* pParseResult,MML报告解析后的对象
   参数2：LIST<STRING>& TitleList,表格的标题
   参数3：LIST<STRING>& ContentList,表格的内容
   参数4：const char* szTableHeadName,整个表格的名字
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatSvr::SendHoriTableReport(CParseResult* pParseResult,
                                      LIST<STRING>& TitleList,
                                      LIST<STRING>& ContentList,
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

/*******************************************************************
函数名称：SendVertTableReport(CParseResult* pParseResult,
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList,
                        LIST<STRING>& RecHeadList
功能描述：发送报告体为一个纵向表格的MML报告
输入参数：
   参数1：CParseResult* pParseResult,MML报告解析后的对象
   参数2：LIST<STRING>& TitleList,表格的标题
   参数3：LIST<STRING>& ContentList,表格的内容
   参数4：LIST<STRING>& RecHeadList,头列表
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatSvr::SendVertTableReport(CParseResult* pParseResult,
                                      LIST<STRING>& TitleList,
                                      LIST<STRING>& ContentList,
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

/*******************************************************************
函数名称：SendMMLReport(const char* szReport)
功能描述：将MML报告转换成MSG_INTRA的形式发送出去
输入参数：const char* szReport,待发送的MML报告
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatSvr::SendMMLReport(const char* szReport)
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


/*******************************************************************
函数名称：SetParaInfoContentList(STRING sAPName, INFO_LIST ContentList)
功能描述：设置查询参数信息内容
输入参数：
    参数1：STRING sAPName,接入点名称
    参数2：INFO_LIST ContentList,内容列表
输出参数：无
返回内容：成功返回TRUE,失败返回FALSE
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatSvr::SetParaInfoContentList(STRING sAPName,
                                         INFO_LIST ContentList)
{
    m_mapAPToParaContent[sAPName] = ContentList;
}
