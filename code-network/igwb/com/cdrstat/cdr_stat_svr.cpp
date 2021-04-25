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
�������ƣ�CCDRStatSvr()
�������������캯��,��ʼ����Ա����
���������
�����������
�������ݣ�
���ú�����
����������
����ʱ�䣺maimaoshi,2003/01/25
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



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
�������ƣ�~CCDRStatSvr()
������������������,ɾ��ָ���Ա����
���������
�����������
�������ݣ�
���ú�����
����������
����ʱ�䣺maimaoshi,2003/01/25
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
CCDRStatSvr::~CCDRStatSvr()
{

}
/*******************************************************************
�������ƣ�InitInstance()
������������ʼ������
���������
�����������
�������ݣ����ش�����(ERR_SUCCESS��ʾ�ɹ�,ERR_FAIL��ʾʧ��)
���ú�����
����������
����ʱ�䣺maimaoshi,2003/01/25
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



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
    //��ȡ�������
    m_uAPNum = IniFile.GetInt(CFG_SEC_COMMON,
                              CFG_COMMON_KEY_APCOUNT,
                              CFG_COMMON_KEY_APCOUNT_DEFAULT);
    //��ȡ���н���������
    char szAPSec[25], szAPName[25];
    for (UINT4 i = 1; i <=  m_uAPNum; i++)
    {
        //���������
        SNPRINTF(szAPSec, sizeof(szAPSec), "%s%d", CFG_SEC_AP, i);
        szAPSec[sizeof(szAPSec) - 1] = '\0';

        IniFile.GetString(szAPSec, CFG_AP_NAME, szAPSec, szAPName, sizeof(szAPName));
        szAPName[sizeof(szAPName) - 1] = '\0';

        m_APNameList.push_back(szAPName);
    }

    //��ȡÿ�쿪ʼͳ�Ƶ�ʱ��"HH:MM:SS"
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

    //ע�ᶨʱ��
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
�������ƣ�ExitInstance()
�����������˳�������
���������
�����������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE
���ú�����
����������
����ʱ�䣺maimaoshi,2003/01/25
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



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

    //ע����ʱ��
    KillTimer(m_lTimerID);

    CWorkModule::ExitInstance();
}

/*******************************************************************
�������ƣ�GetModuleType()
������������ȡ��ģ������
���������
�����������
�������ݣ���ģ�����ͺ�
���ú�����
����������
����ʱ�䣺maimaoshi,2003/01/25
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
MODULE_TYPE CCDRStatSvr::GetModuleType()
{
    return MT_CDRSTAT;
}

/*******************************************************************
�������ƣ�ParseMsg(MSG_INTRA* pMsgParsed)
�������������������ַ�
���������MSG_INTRA* pMsgParsed,����֡
�����������
�������ݣ���
���ú�����1��ProcListQueryPara(),��ѯͳ�ƿ�ѡ����Ϣ
          2��ProcListStatItem(),��ѯͳ����Ŀ������Ϣ
          3��ProcSetStatItem(),����ͳ����Ŀ������Ϣ
          4��ProcBrowseStatRst(),��ѯͳ�ƽ����Ϣ
����������
����ʱ�䣺maimaoshi,2003/01/25
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void CCDRStatSvr::ParseMsg(MSG_INTRA* pMsgParsed)
{
    //����CMD_MML����
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
                //��ѯͳ�ƿ�ѡ����Ϣ
                ProcListQueryPara(pMsgParsed);
                break;
            case MML_LST_STATITEM:
                //��ѯͳ����Ŀ������Ϣ
                ProcListStatItem(pMsgParsed);
                break;
            case MML_SET_STATITEM:
                //����ͳ����Ŀ������Ϣ
                ProcSetStatItem(pMsgParsed);
                break;
            case MML_BRW_STATINFO:
                //��ѯͳ�ƽ����Ϣ
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
�������ƣ�ProcListStatItem(MSG_INTRA* pMsgParsed)
��������������ͳ����Ŀ��ѯ����
���������MSG_INTRA* pMsgParsed,����֡
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/01/27
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void CCDRStatSvr::ProcListStatItem(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));

    //���첢����MML����
    //��ȡ�����͵ı��ı���
    LIST<STRING> TitleList;
    TitleList.push_back(s_CDRSTAT_AP_NAME);
    TitleList.push_back(s_CDRSTAT_STATITEM_NAME);
    TitleList.push_back(s_CDRSTAT_SUBITEM_NAME);
    TitleList.push_back(s_CDRSTAT_SUBITEM_EXPRESS);

    LIST<STRING> ContentList;

    STRING sAPName;
    //dwq �޸Ľӿ� 2003-11-18
    //LIST<char* > listStatItemName;
    LIST<STRING> listStatItemName;
    //dwq �޸Ľ��� 2003-11-18

    LIST<SOneStatSubItemCfg*> subItemList;
    LIST<ICDRStatCfg*> listStatCfg;

    m_pCDRStatCfgMgr->GetStatCfgList(listStatCfg);

    if (listStatCfg.size() == 0)
    {
        //û���κ�������Ϣ�������޷�������ýӿڵı���
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1, S_CDRSTAT_FAIL_GET_CFGINTERFACE);
        return;
    }

    //��װ���ر���
    LIST<ICDRStatCfg*>::iterator it_cfg = listStatCfg.begin();

    while (it_cfg != listStatCfg.end())
    {
        //��ÿ���������д���
        ICDRStatCfg* pTmpCfg = *it_cfg;

        //��ý��������
        sAPName = pTmpCfg->GetAccessPointName();

        //��ô˽�����ͳ�����б�
        pTmpCfg->GetStatItemNameList(listStatItemName);

        //����˽���㻹û���κ�������Ϣ
        if (listStatItemName.size() == 0)
        {
            //ֻ�ѽ�������Ʒ���
            //ap   item      subitem       exp
            //3G    -           -           -
            ContentList.push_back(sAPName);
            ContentList.push_back("-");
            ContentList.push_back("-");
            ContentList.push_back("-");
        }
        else
        {
            //����ͳ�����ҳ���������ͳ������ͱ��ʽ

            //dwq �޸Ľӿ� 2003-11-18
            //LIST<char* >::iterator it_name = listStatItemName.begin();
            LIST<STRING>::iterator it_name = listStatItemName.begin();
            //dwq �޸Ľ��� 2003-11-18

            while (it_name != listStatItemName.end())
            {
                //�����ҳ�ÿ��ͳ����������б�

                pTmpCfg->GetStatItem(it_name->c_str(), subItemList);

                LIST<SOneStatSubItemCfg*>::iterator it_onesub = subItemList.begin();

                //�ҳ�ÿһ��ͳ���������Ϣ
                while (it_onesub != subItemList.end())
                {
                    //ap   item      subitem       exp
                    //3G  ͳ����    ͳ������1      a>1
                    ContentList.push_back(sAPName);
                    ContentList.push_back((*it_onesub)->szStatItemName);
                    ContentList.push_back((*it_onesub)->szStatSubItemName);
                    ContentList.push_back((*it_onesub)->szStatSubItemCond);

                    //���subItemList
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
�������ƣ�ProcListQueryPara(MSG_INTRA* pMsgParsed)
��������������ͳ�ƿ�ѡ����Ϣ��ѯ����
���������MSG_INTRA* pMsgParsed,����֡
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/01/27
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void CCDRStatSvr::ProcListQueryPara(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult**)(pMsgParsed->Data));

    //��MML�����еõ����������
    STRING sAPName = pParseResult->GetStringVal(1);

    //���첢����MML���棬���������ķ��ر���
    LIST<STRING> TitleList, ContentList, RecHeadList, ParaFieldList;

    TitleList.push_back(S_CDRSTAT_FIELD_NAME);
    TitleList.push_back(S_CDRSTAT_FIELD_DESC);
    TitleList.push_back(S_CDRSTAT_FIELD_TYPE);
    TitleList.push_back(S_CDRSTAT_FIELD_OPERATOR);
    TitleList.push_back(S_CDRSTAT_FIELD_UNIT);
    TitleList.push_back(S_CDRSTAT_ENUM_FLAG);
    TitleList.push_back(S_CDRSTAT_ENUM_VALUE);

    ContentList = m_mapAPToParaContent[sAPName];

    //������������ÿ����¼�ı���
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
�������ƣ�ProcSetStatItem(MSG_INTRA* pMsgParsed)
��������������ͳ����Ŀ��������
���������MSG_INTRA* pMsgParsed,����֡
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/01/27
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void CCDRStatSvr::ProcSetStatItem(MSG_INTRA* pMsgParsed)
{
    CParseResult* pParseResult = *((CParseResult**)(pMsgParsed->Data));
    //��MML�����еõ�ͳ��������Ϣ
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
        //��ÿ���������д���
        ICDRStatCfg* pTmpCfg = *it_cfg;

        const char* szAPName = pTmpCfg->GetAccessPointName();

        //ͨ������������ҵ���ӳ���LIST<SOneStatSubItemCfg*>
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
        //��ÿ���������д���
        ICDRStatRst* pTmpRst = *it_rst;

        const char* szAPName = pTmpRst->GetAccessPointName();

        //ͨ������������ҵ���ӳ���LIST<SOneStatSubItemRst*>
        subItemList = mapAPNameStatCfg[szAPName];

        LIST<char* > StatExpList;
        //��subItemListȡ�����еı��ʽ
        LIST<SOneStatSubItemCfg*>::iterator it_onesub;
        for(it_onesub = subItemList.begin(); it_onesub != subItemList.end(); it_onesub++)
        {
            StatExpList.push_back((*it_onesub)->szStatSubItemCond);
        }

        //�������б��ʽ
        pTmpRst->AddStatExpress(StatExpList);

        //����CCDRStatTask::BuildMap_AP_FMT_EXP
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
�������ƣ�ProcBrowseStatRst(MSG_INTRA* pMsgParsed)
��������������ͳ�������������
���������MSG_INTRA* pMsgParsed,����֡
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/01/27
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void CCDRStatSvr::ProcBrowseStatRst(MSG_INTRA* pMsgParsed)
{
    int nPeriod;
    CParseResult* pParseResult = *((CParseResult**)(pMsgParsed->Data));

    //���������
    char* szAPName    = (char* )pParseResult->GetStringVal(1);
    //ͳ������
    pParseResult->GetDigitVal(&nPeriod, 2);
    //ͳ�����ڿ�ʼʱ��
    char* szStartTime = (char* )pParseResult->GetStringVal(3);
    //ͳ�����ڽ���ʱ��
    char* szEndTime   = (char* )pParseResult->GetStringVal(4);
    //ͳ��������
    char* szStatItem  = (char* )pParseResult->GetStringVal(5);

    //ͨ��ͳ���������ҵ�ͳ����������ƺͱ��ʽ
    ICDRStatCfg* pStatCfg = NULL;
    pStatCfg = m_pCDRStatCfgMgr->GetStatCfgInterface(szAPName);
    LIST<SOneStatSubItemCfg*> subItemList;
    pStatCfg->GetStatItem(szStatItem, subItemList);

    if (subItemList.size() == 0)
    {
        //û���κ�������Ϣ�������޷�������ýӿڵı���
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 1, S_CDRSTAT_FAIL_GET_CFGINTERFACE);
        return;
    }

    //����ͬһ��ͳ��������б��ʽ
    LIST<char*> ExpressList;
    //��ȡ�����͵ı��ı���
    LIST<STRING> TitleList;

    //ͳ������
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

    //���ͳ�������ƣ��ҳ�����������������ƣ��ӵ�������
    LIST<SOneStatSubItemCfg*>::iterator it_onecfg = subItemList.begin();
    while (it_onecfg != subItemList.end())
    {
        TitleList.push_back((*it_onecfg)->szStatSubItemName);

        ExpressList.push_back((*it_onecfg)->szStatSubItemCond);
        it_onecfg++;
    }

    VECTOR<STRING> CycleName;
    VECTOR<time_t> StartTime, EndTime;

    //��ú�������ص���Ϣ
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

    //����ѭ��
    for (UINT4 i = 0; i < CycleName.size(); i++)
    {
        ContentList.push_back(CycleName[i]);

        it_exp  = ExpressList.begin();
        //ȡ��ÿ�����ʽ
        while (it_exp != ExpressList.end())
        {
            nCDRCount = 0;
            nValueTotal = 0;

            //ȡ��ͳ��ֵ
            if (ERR_FAIL == (pTmpRst->GetStatExpRst((const char*)(*it_exp),
                                                    StartTime[i],
                                                    EndTime[i],
                                                    nCDRCount,
                                                    nValueTotal)))
            {
                nCDRCount = 0;
            }
            //��ӵ�ͳ�������б�
            sprintf(szCDRCount, "%d", nCDRCount);
            ContentList.push_back(szCDRCount);
            it_exp++;
        }
    }

    SendHoriTableReport(pParseResult, TitleList, ContentList, S_CDRSTAT_TH_LIST_STAT_RESULT);

}

/*******************************************************************
�������ƣ�OnTimer(const long nTimerID)
������������ʱ���¼�������
���������const long nTimerID,
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/14
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void CCDRStatSvr::OnTimer(const long nTimerID)
{
    if (nTimerID == m_lTimerID)
    {
        //����ͳ�������ͳ�ƹ���
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
�������ƣ�GetBillFiles(const char* szPath, LIST<STRING>& qResult)
�������������ĳĿ¼�������ļ���
���������const char* szPath, ·����
���������LIST<STRING>& qResult,��·���µ����л����ļ��б�
�������ݣ������ļ�����
���ú�����
����������
����ʱ�䣺maimaoshi,2003/01/27
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
int CCDRStatSvr::GetBillFiles(const char* szPath, LIST<STRING>& qResult)
{
    //Ϊ���ڻ���̨�ϰ���ʱ����ȷ����ʾ�����ҵ����ļ����밴���޸�ʱ���������,
    //Ϊ���Ƚ��ҵ��Ļ����ļ���Ϣ���뵽FileInfoList�б�Ȼ���FileInfoList
    //�����޸�ʱ��������򣬴Ӷ��õ������޸�ʱ������Ļ����ļ������б�
    LIST<SFileNameNode> FileInfoList;
    SFileNameNode TmpNode;

    int nCount = 0;
    ACE_Dirent FindDir((const ACE_TCHAR* )szPath);

    //����ָ����Ŀ¼���ҳ�Ŀ¼�����е��ļ��������뵽qResult
    dirent* pDirent;
    ACE_stat FileInfo;
    char szFileFullPath[MAX_PATH], szFileName[MAX_PATH];
    pDirent = FindDir.read();
    while(pDirent)
    {
        strcpy(szFileName, pDirent->d_name);
        sprintf(szFileFullPath, "%s/%s", szPath, szFileName);
        ACE_OS::stat(szFileFullPath, &FileInfo);

        //����ҵ�����һ���ļ��������Ҫ��
        if (S_IFDIR != (FileInfo.st_mode & S_IFMT))
        {
            strcpy(TmpNode.szFileName, szFileName);
            TmpNode.tFileTime = FileInfo.st_mtime;
            FileInfoList.push_back(TmpNode);
            nCount++;
        }

        pDirent = FindDir.read();
    }

    //�����޸�ʱ���FileInfoList��������Ȼ��ݴ˵õ������Ļ����ļ����б�
    FileInfoList.sort();
    LIST<SFileNameNode>::iterator i;
    for (i = FileInfoList.begin(); i != FileInfoList.end(); i++)
    {
        qResult.push_back((*i).szFileName);
    }

    return nCount;
}

/*******************************************************************
�������ƣ�ConvertCycle(int nPeriod, char* szStartTime, char* szEndTime, VECTOR<STRING>& CycleName, VECTOR<time_t>& StartTime, VECTOR<time_t>& EndTime)
������������ú�������ص���Ϣ
���������int nPeriod, char* szStartTime, char* szEndTime
���������VECTOR<STRING>& CycleName, VECTOR<time_t>& StartTime, VECTOR<time_t>& EndTime
�������ݣ�����0��ʾ�ɹ�������-1��ʾʧ��
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



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
        //����ǰ���������ȡͳ�ƽ��
        for (nYear = startTm.tm_year; nYear <=  endTm.tm_year; nYear++)
        {
            sprintf(szCycle, "%04d", nYear + 1900);
            CycleName.push_back(szCycle);

            //���ڿ�ʼʱ��
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

            //���ڽ���ʱ��
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
        //����ǰ���������ȡͳ�ƽ��
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

                //���ڿ�ʼʱ��
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

                //���ڽ���ʱ��
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
        //����ǰ���������ȡͳ�ƽ��
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
�������ƣ�StrToTm(const char* szSrc, tm &tmTar)
�������������ִ�ת��Ϊtm�ṹ
���������const char* szSrc, ����������Ϣ���ַ���
���������tm &tmTar,ת��������ڽṹ
�������ݣ�����0��ʾ�ɹ�������-1��ʾʧ��
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



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

    //�ֽ�����ƥ�䣬�򷵻�ʧ��
    if(strlen(szSrc) != 8)
    {
        return -1;
    }

    //ȡ��
    memcpy(szTemp, szSrc, 4);
    szTemp[4] = 0;
    nYear = atoi(szTemp);

    //ȡ��
    memcpy(szTemp, szSrc+4, 2);
    szTemp[2] = 0;
    nMon = atoi(szTemp);

    //ȡ��
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
�������ƣ�GetDay(int nYear, int nMonth)
�������������ꡢ�»������
���������int nYear,��
          int nMonth,��
�����������
�������ݣ������յ���Ŀ
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



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
�������ƣ�ParseCfgInfo(char* szCfgInfo, MAP<STRING, ONESTATSUBITEMCFG_LIST>& mapAPNameStatCfg)
�����������ֽ�������Ϣ"3G,item2,item21,exp21;3G,item2,item21,exp21;GPRS,item1,item11,exp11;GPRS,item2,item21,exp21;"
���������char* szCfgInfo,������Ϣ
���������MAP<STRING, ONESTATSUBITEMCFG_LIST>& mapAPNameStatCfg,�����������������б�ӳ��
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



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
            //����szStatCfg
            memcpy(szStatCfg, szCfgInfo + nOffset, i - nOffset + 1);
            GetCfgItem(szStatCfg, szAPName, szItemName, szSubItemName, szExpress);

            if (mapAPNameStatCfg.find(STRING(szAPName)) !=  mapAPNameStatCfg.end())
            {
                SubItemList = mapAPNameStatCfg[STRING(szAPName)];

                //�����ͳ���ͳ������ͱ��ʽ�����κ�һ���ǿյľͲ�����
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
                    //дTrace()
                    continue;
                }

            }else
            {
                //�����������Ʋ�ͬ
                if (strcmp(szAPName, sAPName.c_str()) != 0)
                {
                    //��һ�ν���
                    if (pSubItemList == NULL)
                    {
                        pSubItemList = new ONESTATSUBITEMCFG_LIST;
                    }
                    else
                    {
                        //�ύʱҪ���List�Ƿ���Ԫ��
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

                //�����ͳ���ͳ������ͱ��ʽ�����κ�һ���ǿյľͲ�����
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
                    //дTrace()
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
        //�ύʱҪ���List�Ƿ���Ԫ��
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
�������ƣ�GetCfgItem(char* szStatCfg, char* szApName, char* szItemName, char* szSubItemName, char* szExpress)
�����������ֽ�һ��������Ϣ
���������char* szStatCfg,һ��������Ϣ
���������char*& szApName, ������Ϣ�еĽ��������
          char*& szItemName, ������Ϣ��ͳ��������
          char*& szSubItemName, ������Ϣ��ͳ����������
          char*& szExpress,������Ϣ�б��ʽ
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



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
                //ȡ���������
                memcpy(szApName, szStatCfg + nOffset, i - nOffset);
                break;
            case 1:
                //ȡͳ������
                nOffset++;
                memcpy(szItemName, szStatCfg + nOffset, i - nOffset);
                break;
            case 2:
                //ȡͳ����������
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
    //���ȡ���ʽ
    nOffset++;
    memcpy(szExpress, szStatCfg + nOffset, i - nOffset);
}

/*******************************************************************
�������ƣ�SendSimpleInfoReport(const char* szCmd, int nRetCode, const char* szInfo)
�������������ͱ�������ֻ�м򵥵���Ϣ��MML����
���������
   ����1��const char* szCmd,MML�����Ӧ������
   ����2��int nRetCode,����Ĵ�����
   ����3��const char* szInfo,�������еļ���Ϣ
�����������
�������ݣ�
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



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
�������ƣ�SendHoriTableReport(CParseResult* pParseResult,
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList,
                        const char* szTableHeadName)
�������������ͱ�����Ϊһ���������MML����
���������
   ����1��CParseResult* pParseResult,MML���������Ķ���
   ����2��LIST<STRING>& TitleList,���ı���
   ����3��LIST<STRING>& ContentList,��������
   ����4��const char* szTableHeadName,������������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void CCDRStatSvr::SendHoriTableReport(CParseResult* pParseResult,
                                      LIST<STRING>& TitleList,
                                      LIST<STRING>& ContentList,
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

/*******************************************************************
�������ƣ�SendVertTableReport(CParseResult* pParseResult,
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList,
                        LIST<STRING>& RecHeadList
�������������ͱ�����Ϊһ���������MML����
���������
   ����1��CParseResult* pParseResult,MML���������Ķ���
   ����2��LIST<STRING>& TitleList,���ı���
   ����3��LIST<STRING>& ContentList,��������
   ����4��LIST<STRING>& RecHeadList,ͷ�б�
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void CCDRStatSvr::SendVertTableReport(CParseResult* pParseResult,
                                      LIST<STRING>& TitleList,
                                      LIST<STRING>& ContentList,
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

/*******************************************************************
�������ƣ�SendMMLReport(const char* szReport)
������������MML����ת����MSG_INTRA����ʽ���ͳ�ȥ
���������const char* szReport,�����͵�MML����
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



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
�������ƣ�SetParaInfoContentList(STRING sAPName, INFO_LIST ContentList)
�������������ò�ѯ������Ϣ����
���������
    ����1��STRING sAPName,���������
    ����2��INFO_LIST ContentList,�����б�
�����������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void CCDRStatSvr::SetParaInfoContentList(STRING sAPName,
                                         INFO_LIST ContentList)
{
    m_mapAPToParaContent[sAPName] = ContentList;
}
