#include "../include/toolbox.h"
#include "../include/frame.h"
#include "cdr_stat_tsk.h"
#include "stat_rst_mgr.h"
#include "cdr_stat_svr.h"
#include "stat_file_group.h"
#include "cdr_stat_rst.h"

#ifdef _PLATFORM_WIN32
#pragma warning(disable:4503)
#endif

/*******************************************************************
函数名称：CCDRStatTask()
功能描述：构造函数
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
CCDRStatTask::CCDRStatTask()
{
    m_pCDRStatRstMgr    = NULL;
    m_bExit             = FALSE;
    m_pIGWB             = NULL;
    m_pIQueryBrowse     = NULL;
    m_szOrigBasePath[0] = '\0';
    m_szFinalBasePath[0]= '\0';
    m_szStatPath[0]     = '\0';
}
/*******************************************************************
函数名称：~CCDRStatTask()
功能描述：析构函数
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
CCDRStatTask::~CCDRStatTask()
{
    if (m_pCDRStatRstMgr != NULL)
    {
        delete m_pCDRStatRstMgr;
        m_pCDRStatRstMgr = NULL;
    }

    if (m_pIGWB != NULL)
    {
        delete m_pIGWB;
        m_pIGWB = NULL;
    }
    m_pIQueryBrowse = NULL;

    while(m_TaskInfoList.size() > 0)
    {
        delete (*m_TaskInfoList.begin());
        m_TaskInfoList.pop_front();
    }
}

/*******************************************************************
函数名称：Init()
功能描述：进行初始化操作
输入参数：UINT4 nApCount
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
int CCDRStatTask::Init(UINT4 nApCount)
{
    //初始化格式库相关对象
    char szIGWBPath[MAX_PATH];
    sprintf(szIGWBPath, "%s/config/format", GetAppPath());
    m_pIGWB = new CIGWB(szIGWBPath);
    m_pIQueryBrowse = (IQueryBrowse*)m_pIGWB->QueryInterface(IID_FP_IQUERYBROWSE);
    if (NULL == m_pIQueryBrowse)
    {
        return ERR_FAIL;
    }
    int nResult = m_pIQueryBrowse->Init(0);
    if (ERR_SUCCESS != nResult)
    {
        return ERR_FAIL;
    }

    //初始化统计结果管理对象
    m_pCDRStatRstMgr = new CCDRStatRstMgr;
    nResult = m_pCDRStatRstMgr->Init(nApCount);
    if (ERR_SUCCESS != nResult)
    {
        return ERR_FAIL;
    }

    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();

    IniFile.GetString(CFG_SEC_DISK_FILE,
                      CFG_FRONT_SAVE_ROOT_DIR,
                      CFG_FRONT_SAVE_ROOT_DIR_DEFAULT,
                      m_szOrigBasePath,
                      sizeof(m_szOrigBasePath));
    m_szOrigBasePath[sizeof(m_szOrigBasePath) - 1] = '\0';

    IniFile.GetString(CFG_SEC_DISK_FILE,
                      CFG_BACK_SAVE_ROOT_DIR,
                      CFG_BACK_SAVE_ROOT_DIR_DEFAULT,
                      m_szFinalBasePath,
                      sizeof(m_szFinalBasePath));
    m_szFinalBasePath[sizeof(m_szFinalBasePath) - 1] = '\0';

    //读取所有接入点的名称
    char szAPSec[25];
    char szAPName[255];
    UINT4 uBillLen = 0;

    for (UINT4 i = 1; i <=  nApCount; i++)
    {
        //接入点名称
        SNPRINTF(szAPSec,
                 sizeof(szAPSec),
                 "%s%d",
                 CFG_SEC_AP,
                 i);
        szAPSec[sizeof(szAPSec) - 1] = '\0';

        IniFile.GetString(szAPSec,
                          CFG_AP_NAME,
                          szAPSec,
                          szAPName,
                          sizeof(szAPName));
        szAPName[sizeof(szAPName) - 1] = '\0';

        //接入点的类型
        UINT4 uAPType = IniFile.GetInt(szAPSec,
                                       CFG_AP_KEY_APTYPE,
                                       CFG_AP_KEY_APTYPE_DEFAULT);
        
        //added by lucy 2004-06-09 SWPD05649
        //当CollectType大于0,表示是采集接入点
        int nCollectType = IniFile.GetInt(szAPSec, 
                                          CFG_AP_KEY_COLTYPE,
                                          CFG_AP_KEY_COLTYPE_DEFAULT);
        if (nCollectType)
        {
            uAPType = AP_OTHER + nCollectType;
        }
        //end SWPD05649
        
        //是否合并
        BOOL bMustMerge = IniFile.GetInt(szAPSec,
                                         CFG_AP_KEY_MUST_MERGE,
                                         CFG_AP_KEY_MUST_MERGE_DEFAULT);
		
		UINT4 uBillType = IniFile.GetInt(CFG_SEC_CDRSTAT,
										 CFG_AP_CDRSTAT_BILLTYPE,
										 CFG_AP_CDRSTAT_BILLTYPE_DEFAULT);

        //统计的话单类型
        uBillType = IniFile.GetInt(szAPSec,
                                   CFG_AP_CDRSTAT_BILLTYPE,
                                   uBillType);
        //是否是特殊的原始格式
        BOOL bFinalSPFileFmt = IniFile.GetInt(szAPSec,
                                              CFG_AP_KEY_FINAL_SPFILEFMT,
                                              CFG_AP_KEY_FINAL_SPFILEFMT_DEFAULT);

        char szRelease[16] = "";

        if (AP_PS == uAPType || AP_PS_RESERVED == uAPType)
        {
            IniFile.GetString(szAPSec,
                              CFG_AP_KEY_RELEASE_VERSION,
                              CFG_AP_KEY_RELEASE_R98,
                              szRelease,
                              sizeof(szRelease));
            szRelease[sizeof(szRelease) - 1] = '\0';
        }

        SCdrStatTaskInfo* pInfo = new SCdrStatTaskInfo;
        pInfo->uAPNumber        = i;
        pInfo->bMustMerge       = bMustMerge;
        pInfo->bFinalSPFileFmt  = bFinalSPFileFmt;
        pInfo->uAPType          = uAPType;
        pInfo->uBillType        = uBillType;
        strncpy(pInfo->szAPName, szAPName, sizeof(pInfo->szAPName));
        pInfo->szAPName[sizeof(pInfo->szAPName) - 1] = '\0';
        strncpy(pInfo->szRelease, szRelease, sizeof(pInfo->szRelease));
        pInfo->szRelease[sizeof(pInfo->szRelease) - 1] = '\0';

        LIST<STRING> ContentList, ParaFieldList;

        //根据浏览格式获得每个格式的可选域信息
        FORMAT_LIST  FormatList;

        //一个接入点的每个格式对应的表达式列表
        FORMAT_MAP_INFOLIST  FormatToParaInfo;

        //构建接入点和格式的列表
        LIST<STRING> FormatNameList, ChlNameList, DecsNameList;

        CFormat* pFmt = NULL;
        if(uBillType == 0)
        {
            //根据接入点名称获得所有原始浏览格式
            m_pIQueryBrowse->GetOrigBrowseNames(szAPName, FormatNameList, DecsNameList);
        }
        else
        {
            //根据接入点名称获得所有最终话单浏览格式
            //获得所有通道名字
            m_pIQueryBrowse->GetChlNames(szAPName, ChlNameList);
            m_mapAPToChlName[szAPName] = ChlNameList;

            LIST<STRING>::iterator it_chl;
            //依次取出每个通道的浏览格式
            for(it_chl = ChlNameList.begin(); it_chl != ChlNameList.end(); it_chl++)
            {
                LIST<STRING> ChlFmtNameList;

                m_pIQueryBrowse->GetFinalBrowseNames(szAPName, *it_chl, ChlFmtNameList, DecsNameList);


                if (ChlFmtNameList.size() > 0)
                {
                    pFmt = m_pIQueryBrowse->GetBrowseFmt(ChlFmtNameList.front());
                    int nLen = pFmt->GetSrcFmtLen();
                    m_mapChlNameToLen[*it_chl] = nLen;
                }

                //把每个通道的所有浏览格式加入到FormatNameList
                FormatNameList.merge(ChlFmtNameList);
            }
        }

        LIST<STRING>::iterator it_fn;
        for (it_fn = FormatNameList.begin(); it_fn != FormatNameList.end(); it_fn++)
        {
            pFmt = m_pIQueryBrowse->GetBrowseFmt(*it_fn);
            if (NULL == pFmt)
            {
                return ERR_FAIL;
            }

            //获得此接入点话单的长度
            uBillLen = pFmt->GetSrcFmtLen();

            //把格式加入到列表
            FormatList.push_back(pFmt);

            int nFieldCount = pFmt->GetFieldNum();
            const SFieldInfo** FieldArray = pFmt->GetFieldsArray();

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

                //在ParaFieldList中查找szFieldName,如果存在忽略掉，保留唯一
                if (FindField(pFieldInfo->szFieldName, ParaFieldList))
                {
                    continue;
                }

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
                        STRNCAT(szEnumValue, "^", sizeof(szEnumValue));
                        STRNCAT(szEnumValue, (*iEnumIdx).szEnumValue, sizeof(szEnumValue));
                        szEnumValue[sizeof(szEnumValue) - 1] = '\0';
                    }
                    sprintf(szEnumFlg, "%d", nEnumCount);
                }

                ParaFieldList.push_back(pFieldInfo->szFieldName);

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
            FormatToParaInfo[pFmt] = ParaFieldList;
        }


        //给CCDRStatTak设置FormatList
        m_mapAPToOrigFormat[szAPName] = FormatList;

        //给CCDRStatTak设置FormatToParaInfo
        m_mapAPToFormatParaInfo[szAPName] = FormatToParaInfo;

        //给CCDRStatSvr设置ContentList
        CCDRStatSvr::SetParaInfoContentList(szAPName, ContentList);

        BuildMap_AP_FMT_EXP(szAPName);

        pInfo->uBillLen = uBillLen;
        m_TaskInfoList.push_back(pInfo);

    }

    return ERR_SUCCESS;
}


/*******************************************************************
函数名称：WaitForExit(void)
功能描述：设置退出标志，并等待退出
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatTask::WaitForExit(void)
{
    m_bExit = TRUE;

    wait();

}


/*******************************************************************
函数名称：ActivateNextStatOper(void)
功能描述：激活下一次统计操作
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatTask::ActivateNextStatOper(void)
{
    activate();
}

/*******************************************************************
函数名称：svc()
功能描述：线程处理函数
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
int CCDRStatTask::svc()
{
    DoStatOpr();

    m_bExit = FALSE;

    return ERR_SUCCESS;
}


/*******************************************************************
函数名称：DoStatOpr(void)
功能描述：启动统计操作
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
int CCDRStatTask::DoStatOpr(void)
{
    //获得当天的时间
    char szToday[9];
    UINT4 i;
    time_t curtime = ACE_OS::time(NULL);
    ACE_OS::strftime(szToday, 9, "%Y%m%d", ACE_OS::localtime(&curtime));
    szToday[8] = '\0';

    SCdrStatTaskInfo* pInfo = NULL;

    TASKINFO_LIST::iterator it;
    FORMAT_MAP_INFOLIST FormatInfo;
    FORMAT_MAP_INFOLIST::iterator it_formatInfo;

    for(it = m_TaskInfoList.begin(); it != m_TaskInfoList.end(); it++)
    {
        pInfo = *it;
        //由接入点名称获得此接入点所有的表达式列表
        ICDRStatRst*  pTmpRst;
        pTmpRst = m_pCDRStatRstMgr->GetStatRstInterface(pInfo->szAPName);
        SStatExpIndexList* pIndexList = NULL;
        pIndexList = pTmpRst->GetStatExpList();

        time_t lasttime = pTmpRst->GetLastStatTime();

        //上次统计日期
        char szLastDate[9];
        ACE_OS::strftime(szLastDate, 9, "%Y%m%d", ACE_OS::localtime(&lasttime));
        szLastDate[8] = '\0';

        BYTE* pTmpMem = NULL;
        SAllStatExpRst*  pAllRst = NULL;

        SStatExpIndexMsg* pIndexMsg = NULL;
        int nValidStatExpCount = 0;
        for(i = 0; i < pIndexList->uStatExpCount; i++)
        {
            pIndexMsg = (SStatExpIndexMsg*)(&(pIndexList->expIndex)[i]);
            if(pIndexMsg->bInUsed)
            {
                nValidStatExpCount++;
            }
        }

        if(nValidStatExpCount == 0)
        {
            pTmpRst->SetLastStatTime(curtime);
            delete pIndexList;
            continue;
        }
        pTmpMem = new BYTE[nValidStatExpCount * ONESTATEXPRSTLENGTH + 8];
        pAllRst = (SAllStatExpRst*)pTmpMem;
        pAllRst->uStatSubItemCount = nValidStatExpCount;
        UINT4 j = 0;
        SOneStatExpRst* pOneExpRst = NULL;
        for(i = 0; i < pIndexList->uStatExpCount; i++)
        {
            pOneExpRst = (SOneStatExpRst*)(&(pAllRst->ItemRst)[j]);
            pIndexMsg = (SStatExpIndexMsg*)(&(pIndexList->expIndex)[i]);
            if(pIndexMsg->bInUsed)
            {
                pOneExpRst->nStatExpIndex = pIndexMsg->nStatExpIndex;
                j++;
            }
        }


        //每天统计的都是前一天的话单，但必须考虑到可能存在更早的话单没有统计：
        //1、每个接入点统计的时候都首先要读出上次的统计时间(LastStatTime),当天的时间(CurrentTime)
        //2、当天要统计的话单范围(X)：LastStatTime + 1 <= X < CurrentTime
        for(;;)
        {
            //如果时间在当天之前就继续统计，否则什么也不做
            if (strcmp(szLastDate, szToday) >= 0)
            {
                break;
            }

            //初始化一天所有表达式的SAllStatExpRst结构
            for(UINT4 i = 0; i < nValidStatExpCount; i++)
            {
                pOneExpRst = (SOneStatExpRst*)(&(pAllRst->ItemRst)[i]);
                pOneExpRst->nCDRCount   = 0;
                pOneExpRst->nValueTotal = 0;
            }

            if((*it)->uBillType == 0)
            {
                sprintf(m_szStatPath, "%s/%s/%s", m_szOrigBasePath, (*it)->szAPName, szLastDate);

                int nReturn = StatCDR(pInfo, m_szStatPath, szLastDate, pIndexList, pAllRst);
                if (nReturn == ERR_FAIL)
                {
                    pTmpRst->SetLastStatTime(curtime);
                    delete pIndexList;
                    delete pTmpMem;
                    return ERR_FAIL;
                }
                else if (nReturn == ERR_STAT_FAIL)
                {
                    delete pIndexList;
                    delete pTmpMem;
                    return ERR_STAT_FAIL;
                }
            }
            else
            {
                LIST<STRING> listChlName;
                listChlName = m_mapAPToChlName[(*it)->szAPName];
                LIST<STRING>::iterator it_chl;

                for(it_chl = listChlName.begin(); it_chl != listChlName.end(); it_chl++)
                {
                    sprintf(m_szStatPath,
                            "%s/%s/%s/%s",
                            m_szFinalBasePath,
                            (*it)->szAPName,
                            (*it_chl).c_str(),
                            szLastDate);

                    pInfo->uBillLen = m_mapChlNameToLen[*it_chl];
                    int nReturn = StatCDR(pInfo, m_szStatPath, szLastDate, pIndexList, pAllRst);
                    if (nReturn ==  ERR_FAIL)
                    {
                        pTmpRst->SetLastStatTime(curtime);
                        delete pIndexList;
                        delete pTmpMem;
                        return ERR_FAIL;
                    }
                    else if (nReturn == ERR_STAT_FAIL)
                    {
                        delete pIndexList;
                        delete pTmpMem;
                        return ERR_STAT_FAIL;
                    }
                }
            }
            //增加此接入点当天的所有统计结果
            pAllRst->uStatEndTime = lasttime;
            pTmpRst->AddStatRst(*pAllRst, lasttime);
            
            if (GetNextDate(szLastDate, lasttime) == -1)
            {
                break;
            }
        }

        pTmpRst->SetLastStatTime(curtime);

        delete pIndexList;
        delete pTmpMem;
    }

    return ERR_SUCCESS;
}

int CCDRStatTask::StatCDR(SCdrStatTaskInfo* pInfo,
                          char* szPath,
                          char* szLastDate,
                          SStatExpIndexList* pIndexList,
                          SAllStatExpRst*& pAllRst)
{
    UINT4 nCDRLen = 0;
    BOOL  bSucc, bRemain;
    BYTE* pCDRRecord = NULL;

    TASKINFO_LIST::iterator it;
    FORMAT_MAP_INFOLIST FormatInfo;
    FORMAT_MAP_INFOLIST::iterator it_formatInfo;

    if(m_mapAPToFormatExpress.find(pInfo->szAPName) == m_mapAPToFormatExpress.end())
    {
        return ERR_SUCCESS;
    }
    //取出此接入点的“格式-表达式列表”的映射
    FormatInfo = m_mapAPToFormatExpress[pInfo->szAPName];

    CCDRStatFileGroup* pCDRStatFileGroup = new CCDRStatFileGroup;
    pCDRStatFileGroup->Init(pInfo->uAPType,
                            pInfo->uBillType,
                            pInfo->bMustMerge,
                            pInfo->bFinalSPFileFmt,
                            pInfo->uBillLen,
                            pInfo->uAPNumber);

    if (pCDRStatFileGroup->PrepareFiles(szPath) != ERR_SUCCESS)
    {
        delete pCDRStatFileGroup;
        return ERR_STAT_OPEN_FILE_FAIL;
    }

    if (!pCDRStatFileGroup->Open(0))
    {
        delete pCDRStatFileGroup;
        return ERR_STAT_OPEN_FILE_FAIL;
    }

    //取话单数据，直到没有话单数据为止
    do
    {
        if(m_bExit)
        {
            delete pCDRStatFileGroup;
            return ERR_STAT_FAIL;
        }

        //取出一张话单
        bSucc = pCDRStatFileGroup->GetNextRecord(pCDRRecord, nCDRLen, bRemain);
        if (!bSucc)
        {
            delete pCDRStatFileGroup;
            return ERR_FAIL;
        }
        if (!bRemain)
        {
            break;
        }

        CFormat*  pTmpFormat = NULL;
        //对每张话单进行处理
        for(it_formatInfo = FormatInfo.begin(); it_formatInfo!= FormatInfo.end(); it_formatInfo++)
        {
            //取出一个格式
            pTmpFormat = (*it_formatInfo).first;
			
			STRING SFormatName, SFormatDesc;
			pTmpFormat->GetFormatGen(SFormatDesc, SFormatName);
			if (SFormatDesc.compare("ALL") == 0)
			{
				continue;
			}

            //判断是否属于本格式
            BOOL bFlag = pTmpFormat->IsTheFormat(pCDRRecord, nCDRLen);

            if (bFlag == 0)
            {
                INFO_LIST  ExpressList;
                INFO_LIST::iterator it_Info;

                //如果属于本格式，继续进行
                ExpressList = (*it_formatInfo).second;

                for(it_Info = ExpressList.begin(); it_Info != ExpressList.end(); it_Info++)
                {
                    //判断是否属于此查询条件
                    if (pTmpFormat->IsTheQueryCondition(pCDRRecord, *it_Info) == 0)
                    {
                        //根据表达式名找到表达式的索引号
                        UINT4 nIndex = FindIndexFromName((*it_Info).c_str(), pIndexList);
                        if ( nIndex != ERR_FAIL)
                        {
                            //根据索引号找到此统计子项结果的位置
                            SOneStatExpRst* pOneExpRst = NULL;

                            for(UINT4 i = 0; i < pAllRst->uStatSubItemCount; i++)
                            {
                                pOneExpRst = (SOneStatExpRst*)(&(pAllRst->ItemRst)[i]);

                                if (pOneExpRst->nStatExpIndex == nIndex)
                                {
                                    //修改统计结果值
                                    pOneExpRst->nCDRCount++;
                                    pOneExpRst->nValueTotal++;
                                }
                            }
                        }
                    }
                }
            }
        }
    } while (1);


    delete pCDRStatFileGroup;
    return ERR_SUCCESS;

}

/*******************************************************************
函数名称：BuildMap_AP_FMT_EXP(const char* const szAPName)
功能描述：构建接入点对应“格式-表达式列表”，用于初始化时和当表达式变化时
输入参数：const char* const szAPName,接入点名称
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatTask::BuildMap_AP_FMT_EXP(const char* const szAPName)
{
    UINT4 uBillLen = 0;

    //获得格式列表
    FORMAT_LIST FormatList;
    FormatList = m_mapAPToOrigFormat[szAPName];

    //由接入点名称获得此接入点所有的表达式列表
    ICDRStatRst*  pTmpRst = m_pCDRStatRstMgr->GetStatRstInterface(szAPName);
    LIST<STRING>  AllExpressList;
    pTmpRst->GetAllExpress(AllExpressList);

    //获得接入点的“格式-可选域列表”映射
    FORMAT_MAP_INFOLIST  FormatToParaInfo;
    FormatToParaInfo = m_mapAPToFormatParaInfo[szAPName];

    //一个接入点的每个格式对应的表达式列表
    FORMAT_MAP_INFOLIST  FormatToExpress;

    //根据浏览格式获得每个格式的可选域信息
    FORMAT_LIST::iterator it_Format;
    for (it_Format = FormatList.begin(); it_Format != FormatList.end(); it_Format++)
    {
        CFormat* pFmt = *it_Format;

        //获得此接入点话单的长度
        uBillLen = pFmt->GetSrcFmtLen();

        INFO_LIST ExpressList, ParaInfoList;
        //构建“格式-表达式列表”映射

        //1、获得每个格式的可选域
        ParaInfoList = FormatToParaInfo[pFmt];

        //2、循环处理{分析每个表达式，看是否属于此格式，如果属于此格式就加入到pExpressList映射表中}
        LIST<STRING>::iterator it_express;

        for(it_express = AllExpressList.begin(); it_express != AllExpressList.end(); it_express++)
        {
            INFO_LIST FieldList;
            if (pFmt->ParseExpress((*it_express).c_str(), FieldList) != 0)
            {
                continue;
            }

            BOOL bFlag = TRUE;
            INFO_LIST::iterator it_field;

            for (it_field = FieldList.begin(); it_field != FieldList.end(); it_field++)
            {
                if (!IsExist(*it_field, ParaInfoList))
                {
                    bFlag = FALSE;
                    break;
                }
            }

            if (bFlag)
            {
                ExpressList.push_back(*it_express);
            }
        }

        //3、加入“格式-表达式”映射
        if(ExpressList.size() > 0)
        {
            FormatToExpress[pFmt] = ExpressList;
        }
    }

    //接入点对“格式-表达式列表”映射
    if(FormatToExpress.size() > 0)
    {
        m_mapAPToFormatExpress[szAPName] = FormatToExpress;
    }
}


/*******************************************************************
函数名称：IsExist(STRING sField, LIST<STRING> ParaInfoList)
功能描述：判断一个域是否属于可选域列表
输入参数：STRING sField, 域名
          LIST<STRING> ParaInfoList,可选域列表
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
BOOL CCDRStatTask::IsExist(STRING sField, LIST<STRING> ParaInfoList)
{
    INFO_LIST::iterator it_parainfo;

    for (it_parainfo = ParaInfoList.begin(); it_parainfo != ParaInfoList.end(); it_parainfo++)
    {
        if (sField.compare(*it_parainfo) == 0)
        {
            return TRUE;
        }
    }

    return FALSE;
}


/*******************************************************************
函数名称：FindIndexFromName(const char* const szExpress, SStatExpIndexList* pIndexList)
功能描述：根据表达式名称查找索引号
输入参数：const char* const szExpress,表达式
          SStatExpIndexList* pIndexList,表达式列表
输出参数：无
返回内容：索引号
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
int CCDRStatTask::FindIndexFromName(const char* const szExpress, SStatExpIndexList* pIndexList)
{
    SStatExpIndexMsg* pIndexMsg = NULL;
    for(UINT4 i = 0; i < pIndexList->uStatExpCount; i++)
    {
        pIndexMsg = (SStatExpIndexMsg*)(&(pIndexList->expIndex)[i]);

        if  (strcmp(pIndexMsg->szStatExp, szExpress) == 0)
        {
            return pIndexMsg->nStatExpIndex;
        }
    }

    return ERR_FAIL;
}


/*******************************************************************
函数名称：GetNextDate(char* szDate, time_t& lastdate)
功能描述：获得下一天的日期
输入参数：char* & szDate,当天的日期
输出参数：char* & szDate,下一天的日期
          time_t& lastdate, 统计时间
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
int CCDRStatTask::GetNextDate(char* szDate, time_t& lastdate)
{
    int nYear = 0, nMon = 0, nDay = 0;
    char szTemp[10];
    if(NULL == szDate)
    {
        return -1;
    }

    //字节数不匹配，则返回失败
    if(strlen(szDate) != 8)
    {
        return -1;
    }

    //取年
    memcpy(szTemp, szDate, 4);
    szTemp[4] = 0;
    nYear = atoi(szTemp);

    //取月
    memcpy(szTemp, szDate+4, 2);
    szTemp[2] = 0;
    nMon = atoi(szTemp);

    //取日
    memcpy(szTemp, szDate+6, 2);
    szTemp[2] = 0;
    nDay = atoi(szTemp);

    switch (nMon)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
        {
            if (nDay == 31)
            {
                nMon++;
                nDay = 1;
            }
            else
            {
                nDay++;
            }
            break;
        }
    case 12:
        {
            if (nDay == 31)
            {
                nYear++;
                nMon = 1;
                nDay = 1;
            }
            else
            {
                nDay++;
            }
            break;
        }
    case 4:
    case 6:
    case 9:
    case 11:
        {
            if (nDay == 30)
            {
                nMon++;
                nDay = 1;
            }
            else
            {
                nDay++;
            }
            break;
        }
    case 2:
        {
            if ((nYear % 400 == 0) || ((nYear % 4 == 0) && (nYear % 100 != 0)))
            {
                if (nDay == 29)
                {
                    nMon++;
                    nDay = 1;
                }
                else
                {
                    nDay++;
                }

            }
            else
            {
                if (nDay == 28)
                {
                    nMon++;
                    nDay = 1;
                }
                else
                {
                    nDay++;
                }
            }
            break;
        }
    default:
        break;
    }

    tm tmTar;
    tmTar.tm_year = nYear - 1900;
    tmTar.tm_mon  = nMon - 1;
    tmTar.tm_mday = nDay;
    tmTar.tm_hour = 0;
    tmTar.tm_min  = 0;
    tmTar.tm_sec  = 0;

    lastdate = mktime(&tmTar);

    //输出下一天的日期
    sprintf(szDate, "%04d%02d%02d", nYear, nMon, nDay);
    return 0;
}

/*******************************************************************
函数名称：FindField(const char* const szFieldName, LIST<STRING> FieldList)
功能描述：查找域
输入参数：
    参数1：const char* const szFieldName,域名
    参数2：LIST<STRING> FieldList,域列表
输出参数：无
返回内容：成功返回TRUE,失败返回FALSE
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
BOOL CCDRStatTask::FindField(const char* const szFieldName, LIST<STRING> FieldList)
{
    LIST<STRING>::iterator it;
    for (it = FieldList.begin(); it != FieldList.end(); it++)
    {
        if (strcmp(szFieldName, (*it).c_str()) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}
CCDRStatRstMgr* CCDRStatTask::GetCCDRStatRstMgr()
{
    return m_pCDRStatRstMgr;
}

