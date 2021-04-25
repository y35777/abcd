// task_creator.cpp: implementation of the CTaskCreator class.
//
//////////////////////////////////////////////////////////////////////
#include "task_creator.h"
CTaskCreator::CTaskCreator()
{
    m_pDdaSet = NULL; //这个只是引用
    m_nPageLimit = 48*1024;
}

CTaskCreator::~CTaskCreator()
{

}

int CTaskCreator::Init(STRING szPerfRoot)
{
    //根据szPerfRoot路径组成状态文件的全路径，
    //调用m_CreateProxy.Init()函数将返回值存在m_pDdaSet
    //中并利用此值初始化m_Query，初始化gTaskMap(以参数的形式
    //传递给m_CreateProxy）成功返回TRUE，失败返回FALSE。
	//by ldf 2003-12
    #ifdef _PLATFORM_WIN32 
     STRING szPerfName = szPerfRoot + "\\prf_status.dbf";
    #else
	    STRING szPerfName = szPerfRoot + "/prf_status.dbf";
    #endif

    TaskMap *pTaskMap = &(g_TaskInfo.g_TaskMap);
    m_pDdaSet = m_CreateProxy.Init(szPerfName,pTaskMap);
    if(!m_pDdaSet)
        return FALSE;
    m_Query.Attach(m_pDdaSet);
    return TRUE;
}

int CTaskCreator::CreateTask(CParseResult *pParse)
{
    //根据pParse参数分析出SInformation信息
    //并动态生成 一个SInformation对象，
    if(pParse->GetCmdCode() != MML_CRE_PRFTSK)
        return FALSE;
    SInformation *pInfo = new SInformation;
    int nRes = 0;
    if(GenerateInformation(pParse,pInfo))
    {
        //调用m_CreateProxy.CreateTask(SInformation*) 
        //创建统计任务，如果返回NULL，就生成创建失败的MML报文，
        //并发送。返回FALSE；创建成功的话就将这个任务加入到
        //gTaskMap中，并在gTaskIDToStringMap建立索引。
        //生成创建成功的MML报文，并发送。返回TRUE。
        CPerfTask *pTask = m_CreateProxy.CreateTask(pInfo);
        if(pTask == NULL)
        {
            nRes = -1;
            delete pInfo,pInfo = NULL;
            m_szError = m_CreateProxy.GetLastError();
        }
        else
        {
            g_TaskInfo.g_TaskMap[pInfo->m_szTaskName] = pTask;
            nRes = 0;
        }
    }
    else
    {
        delete pInfo, pInfo = NULL;
        //by ldf 2004-04-26 M2000V2B02 性能联调，增加返回错误码
        if(m_szError.compare(S_PERF_TASKNAME_EXIST) == 0)
        {
            nRes = ERROR_TASKNAME_EXIST;
        }
        else
        {
            nRes = -1;
        }
        //end
	
    }
    //by ldf 2003-12-24
	//GenerateCommonReport(g_TaskInfo.g_pReport,pParse->GetCmdLine(),S_PERF_CREATE_SUCCESS,nRes);
	GenerateCommonReport(&g_TaskInfo,pParse->GetCmdLine(),S_PERF_CREATE_SUCCESS,nRes);
    SendMMLReport(g_TaskInfo.g_pReport,APP_MML_NORM,pParse->GetIP());
    return (nRes == 0);
}

int CTaskCreator::DeleteTask(CParseResult *pParse)
{
    //从参数中得到任务名，
    //调用m_CreateProxy.DeleteTask(szTaskName)删除这个任务，
    //如果失败，返回失败的报文。删除成功，就将gTaskMap和
    //gTaskIDToStringMap中的信息删掉，返回成功的MML报文。
    //返回TRUE。
    //取第一个参数任务名称
    STRING szTaskName = pParse->GetStringVal(1);
    int nRes = 0;
    if(m_CreateProxy.DeleteTask(szTaskName))
    {
        delete g_TaskInfo.g_TaskMap[szTaskName], g_TaskInfo.g_TaskMap[szTaskName] = NULL;
        g_TaskInfo.g_TaskMap.erase(szTaskName);
        nRes = 0;
    }
    else
    {
        m_szError = m_CreateProxy.GetLastError();
        //by ldf 2004-04-26 M2000性能联调，把返回错误码和M2000匹配
        if(m_szError.compare(S_PERF_NOTHISTASK) == 0)
        {
            nRes=ERROR_NOTHISTASK;
        }
        else
        {
            nRes = -1;
        }
        //end
    }
	//by ldf 2003-12-24
    //GenerateCommonReport(g_TaskInfo.g_pReport,pParse->GetCmdLine(),
	GenerateCommonReport(&g_TaskInfo,pParse->GetCmdLine(),
                         S_PERF_DELETE_SUCCESS,nRes);
    SendMMLReport(g_TaskInfo.g_pReport,APP_MML_NORM,pParse->GetIP());
    return (nRes == 0);
}
int CTaskCreator::ListTaskInfo(CParseResult *pParse)
{
    int nRet = REPORT_MORE;
    m_Query.ClearBind();
    try{
        //取pParse的CHT参数和INFO参数，按CHT参数值选择分支：
        //1－执行分支1，2－执行分支2，3－执行分支3，4－执行分支4，
        //5－执行分支5   
        int nCHT = 0,nINFO = 0;
        //取第一个参数 CHT(查询类型) 必要参数
        if(!pParse->GetDigitVal(&nCHT,1))
            throw "failed to get the first parameter";
        //取第2个参数 INFO(显示详细信息) 可选，默认0
        if(!pParse->GetDigitVal(&nINFO,2))
            nINFO = 0;
        CDdaQuery *pQuery = NULL;
        switch(nCHT)
        {
        case 1:
            {
                //取第3个参数 TSKN参数，获得任务名称
                const char *pTaskName = pParse->GetStringVal(3);
                //GetStringVal返回可能是""
                if(strcmp(pTaskName,"") != 0)
                {
                    //如果TSKN参数指定了任务名，
                    //调用m_Query.BindAnd("任务名",“任务名称”,OpEqual)
                    //将条件帮定到查询集合上，然后调用m_Query的查询命令，
                    //得到结果记录的索引从m_pDdaSet中取到相应的记录然后
                    m_Query.AddBindAnd("TSKN",pTaskName,opEqual);
                    m_Query.DoQuery();
                    pQuery = &m_Query;
                }
                
                break;
            }
        case 2:
            {
                //取MS参数和其子参数，将子参数作为条件利用
                //m_Query.BindAnd函数绑定到m_Query中，然后取得结果数组
                //利用此数组从m_pDdaSet中取得记录，再跟据INFO参数要求
                //组成任务的基本信息和详细信息的MML报告
                INTMAP MsMap;
                INTMAP::iterator it;
                int nBufPos = 0;
                char *pBuf =  new char[ 3 * COUNTER_NUM];
                GetEnumValue(pParse,5,10,MsMap); 
                for(it = MsMap.begin(); it!=MsMap.end(); it++)
                {                    
                    SNPRINTF(pBuf + nBufPos,COUNTER_NUM,
                        "%d&",it->first);
                    if(it->first >= 10)
                        nBufPos += 3;
                    else
                        nBufPos += 2;       
                }
                pBuf[nBufPos-1] = 0;
                m_Query.AddBindAnd("ITM",STRING(pBuf),opSet);
                m_Query.DoQuery();
                pQuery = &m_Query;
                break;
            }
        case 3:
            {
                //取STAT参数，将用户状态作为条件绑定到m_Query上，
                //取得符合条件的记录根据INFO 参数要求组成任务的基本信息
                //和详细信息的MML报告
                int nSTAT = 0;
                if(!pParse->GetDigitVal(&nSTAT,6))
                    nSTAT = 0;
                m_Query.AddBindAnd("RUN",nSTAT,opEqual);
                m_Query.DoQuery();
                pQuery = &m_Query;
                break;
            }
        case 4:
            {
                //取USER参数，将用户名绑定到m_Query上然后取得符合条件
                //的记录根据INFO 参数要求组成任务的基本信息和详细信息的
                //MML报告
                const char *szUser = pParse->GetStringVal(UINT2(4));
                m_Query.AddBindAnd(STRING("USER"),STRING(szUser),opEqual);
                m_Query.DoQuery();
                pQuery = &m_Query;
                break;
            }
        case 5:
            {   
                //取RDS和RDE参数，将两个日期将用户名绑定到m_Query上然后
                //取得符合条件的记录根据INFO 参数要求组成任务的基本信息和
                //详细信息的MML报告
                int nYear = 0, nMon = 0, nDay = 0;
                if(!pParse->GetDigitVal(&nYear,7,1)
                    || !pParse->GetDigitVal(&nMon,7,2)
                    || !pParse->GetDigitVal(&nDay,7,3))
                    throw "bad start date";
                char szDate[10] = "";
                SNPRINTF(szDate,10,"%04d%02d%02d",nYear,nMon,nDay);       
                m_Query.AddBindAnd("SD",STRING(szDate),opGreater);
                
                if(!pParse->GetDigitVal(&nYear,8,1)
                    || !pParse->GetDigitVal(&nMon,8,2)
                    || !pParse->GetDigitVal(&nDay,8,3))
                    throw "bad end date";
                SNPRINTF(szDate,10,"%04d%02d%02d",nYear,nMon,nDay);       
                m_Query.AddBindAnd("SD",STRING(szDate),opLess);
                m_Query.DoQuery();
                pQuery = &m_Query;
                break;
            }
        } // switch
        
        do {
            nRet = GenerateTaskInfoReport(&g_TaskInfo,
                                          pParse->GetCmdLine(),
                                          nINFO,pQuery);
            if(nRet == REPORT_ERR)
				GenerateCommonReport(&g_TaskInfo,
                                     pParse->GetCmdLine(),
                                     "", REPORT_ERR);
            SendMMLReport(g_TaskInfo.g_pReport,APP_MML_NORM,pParse->GetIP());
        } while(nRet == REPORT_MORE);
        
    }
    catch(char *pError)
    {
        m_szError = pError;
        GenerateCommonReport(&g_TaskInfo,
                             pParse->GetCmdLine(),
                             "", REPORT_ERR);
        SendMMLReport(g_TaskInfo.g_pReport,APP_MML_NORM,pParse->GetIP());
        nRet =  REPORT_ERR;
    }
    return (nRet != REPORT_ERR);
}
int CTaskCreator::GetEnumValue(CParseResult *pParse, int nID, int nMaxLen, INTMAP &EnumMap)
{
    STRING szParameter;
    int nPos = 0;
    int nValue = 0;
    int nKeyPos = 0;
    for (int i = 1; i <= nMaxLen; i ++)
    {
        szParameter = pParse->GetStringVal(nID,i);
        if (szParameter.compare("") != 0)
        {
            nPos = szParameter.find("-");
            nKeyPos = atoi((szParameter.substr(0, nPos)).c_str());
            nValue = atoi((szParameter.substr(nPos + 1)).c_str());
            EnumMap[nKeyPos] = nValue;
        }
        else
            break;
    }
    return EnumMap.size();
}
int CTaskCreator::GenerateInformation(CParseResult *pParse,SInformation *pInfo)
{
    try{
        int i = 0;
        //取第1个参数性能测量任务名称,必选参数
        pInfo->m_szTaskName = pParse->GetStringVal(1);
        if(pInfo->m_szTaskName.compare("") == 0)
            throw "failed to get first parameter";
        if(g_TaskInfo.g_TaskMap.find(pInfo->m_szTaskName) != g_TaskInfo.g_TaskMap.end())
            throw S_PERF_TASKNAME_EXIST;
        //取第2个参数性能测量任务描述,可选参数
        pInfo->m_szTaskDes = pParse->GetStringVal(2); //""
        pInfo->m_szUser = pParse->GetUserName();

        //by ldf 2004-04-22 与M2000V2联调，周鹏19715确认，
        //特有此特殊处理，把M2000创建的任务统一只用“M2000”标识此任务的创建者
        if( ACE_OS::strncasecmp(pParse->GetUserName(),"M2000", 4) == 0 )
        {
            pInfo->m_szUser ="M2000";
        }
        //end
		
        //取第3个参数测量单元ID,必选参数
        //枚举
        INTMAP EnumMap; 
        INTMAP::iterator map_it;
        if(GetEnumValue(pParse,3,2,EnumMap))
        {
            for(map_it = EnumMap.begin(); map_it != EnumMap.end(); map_it++)
            {
                pInfo->m_vMU.push_back(map_it->first);
            }
        }
        else
            throw "failed to get the third parameter";
        //取第4个参数是否立即运行,可选参数
        if(!(pParse->GetDigitVal(&(pInfo->m_nStatus),4)))
            pInfo->m_nStatus = TASK_NORMAL;
        //取第5个参数统计周期,必选参数
        if(!(pParse->GetDigitVal(&(pInfo->m_nPeriod),5)))
            pInfo->m_nPeriod = 30; //默认:30 minute
        //取第6个参数起始日期YY&MM&DD（可选）
        tm tmDate;
        memset(&tmDate,0,sizeof(tm));
        if(pParse->GetDigitVal(&(tmDate.tm_year),6,1) 
            && pParse->GetDigitVal(&(tmDate.tm_mon),6,2)
            && pParse->GetDigitVal(&(tmDate.tm_mday),6,3))
        {
            tmDate.tm_year -= 1900;
            tmDate.tm_mon -= 1;
            pInfo->m_tStartTime = mktime(&tmDate);
        }
        else
            time(&(pInfo->m_tStartTime));
        //取第7个参数统计时间段的个数,必选参数
        int nTimeSpanNum = 0;
        if(pParse->GetDigitVal(&nTimeSpanNum,7))
        {
            if(nTimeSpanNum > 3 || nTimeSpanNum < 1)
                throw "the seventh parameter overflow limit";
            int nParaLoc = 0;
            sTimeSpan sTime;
            memset(&sTime, 0, sizeof(sTimeSpan));
            for(i = 0; i < nTimeSpanNum; i++)
            {
                //取第8-13个参数
                //SP1-3S(起始时间1-3)，必选
                //SP1-3E(结束时间1-3)，必选
                int nStartHH = 0, nStartMM = 0,nEndHH = 0, nEndMM = 0;
                nParaLoc = 8 + i*2;
                BOOL bRet1 = pParse->GetDigitVal(&nStartHH,nParaLoc,1);
                BOOL bRet2 = pParse->GetDigitVal(&nStartMM,nParaLoc,2);
                if(!bRet1 || !bRet2)
                    throw "no start time";
                
                sTime.tStartTime.tm_hour = nStartHH;
                sTime.tStartTime.tm_min = nStartMM;
                
                nParaLoc = 8 + i*2 + 1;
                bRet1 = pParse->GetDigitVal(&nEndHH,nParaLoc,1);
                bRet2 = pParse->GetDigitVal(&nEndMM,nParaLoc,2);
                if(!bRet1 || !bRet2)
                    throw "no end time";
                
                sTime.tEndTime.tm_hour = nEndHH;
                sTime.tEndTime.tm_min = nEndMM;
                
                pInfo->m_vTimeSpan.push_back(sTime);
                
            }
        }
        else
            throw "failed to get the 7th parameter";
        //取第14个参数TSKTP(任务类型)可选参数，数字型 
        //默认取值是半永久任务
        if(!pParse->GetDigitVal(&(pInfo->m_nTaskType),14))
            pInfo->m_nTaskType = TASK_PERMANENCE;
        //取第15个参数LPT(有限周期天数)
        //如果时间类型为0则不可选, 如果为1则必选
        //默认取值：7天; 注：既是为0，取7也无所谓，反正不用
        if(!pParse->GetDigitVal(&(pInfo->m_nDayLimit),15))
            pInfo->m_nDayLimit = 7;
        //取第16个参数TT（时间类型）可选参数 如果不选，则缺省为0
        if(!pParse->GetDigitVal(&(pInfo->m_nTimeType),16))
            pInfo->m_nTaskType = 0;
        //取第17个参数STS(统计时间集合）位域类型的字节数为38
        //如果时间类型是DAY,则不可选;如果是WEEK 则必选，
        //取值范围是SUN到SAT;如果是MONTH,则必选，取值范围是D1－D31;
        //可多选，用&隔开
        //枚举
        EnumMap.clear();
        if(GetEnumValue(pParse,17,38,EnumMap))
        {
            int nDay = 0;
            for(map_it = EnumMap.begin();map_it != EnumMap.end(); map_it++)
            {
                nDay = map_it->first; 
                if(pInfo->m_nTimeType == TT_WEEK
                   &&(nDay > 6 || nDay < 0))
                {
                    throw "17th parameter overflow while Time Type is TT_WEEK";
                }
                else if(pInfo->m_nTimeType == TT_MONTH
                        && (nDay < 6 || nDay >38))
                {
                    throw "17th parameter overflow while Time Type is TT_MONTH";
                }
                pInfo->m_vTimeSet.push_back(nDay);
            }
        }
        else if(pInfo->m_nTimeType == TT_DAY)
        {
            pInfo->m_vTimeSet.clear();
        }
        else
        {
            throw "17th parameter is null";
        }

        //取第18个参数SRST（保留结果天数）可选参数，默认取值：30
        if(!pParse->GetDigitVal(&(pInfo->m_nResultSaveDays),18))
            pInfo->m_nResultSaveDays = 30;
        //取第19个参数ITM（测量指标）必选参数 最多10个指标
        //枚举
        int nCounter = 0;
        EnumMap.clear();
		//by ldf 2004-05-19 最多有19个统计项
        if(GetEnumValue(pParse,19,19,EnumMap))
        {
            for(map_it = EnumMap.begin(); map_it != EnumMap.end(); map_it++)
            {
                nCounter = map_it->first;
                pInfo->m_mCouterMap[nCounter] = 0;
                pInfo->m_mCounterStatisticCount[nCounter] = 0;
            }
        }
        else
        {
            throw "19th paremeter is null";
        }
        //取20个参数OIV （测量对象实例值）
        int nKey = 0;
        char szOIV[32];
        strncpy(szOIV,pParse->GetStringVal(20),32);
        char *pTok = strtok(szOIV,"&");
        for(i = 1; pTok; i++, pTok = strtok(NULL,"&"))
        {
            nKey = pInfo->m_vMU[i-1];
            pInfo->m_mObjectInstance[nKey] = atoi(pTok);
        }
    }
    catch(const char *pError)
    {
        m_szError = pError;
        return FALSE;
    }
    return TRUE;
}

int CTaskCreator::GenerateCommonReport(STaskInfo *p,const char *pCmdLine, const char *pContent,int nRes)
{
    /*
    +++    HUAWEI iGWB        2002-05-24 09:25:13
    TASKCMD    #0
    %%CRE TSK: TSKN="CG Performance", TSKDES="CG性能测量", MU=1&&2, RUN=FALSE, SPD=1, SP1S=1, SP1E=24, TSKTP=1, LPT=10, TT=WEEK, STS=SUN&SAT, SRST=20, SRCD=10000, OIV=1&1, ITM=1&2&3&4&5&6&7;%%
    RETCODE = 0  执行成功
    创建任务成功
    ---    END
    */
    p->g_pReport->MakeReportHead();
    p->g_pReport->MakeCmdDisplay(pCmdLine,p->nReportCsn++,nRes,"O&M");

    //by ldf 2004-04-26 M2000联调
    if(nRes != 0)
    //end
    {
        p->g_pReport->MakeReportItem(S_PERF_FAIL_REASON,
                                m_szError.c_str(),20,
                                ALIGN_RIGHT);
        p->g_pReport->AddString(STR_NEWLINE);
        p->g_pReport->AddString(STR_NEWLINE);
    }
    else
    {
        p->g_pReport->AddString(pContent);
        p->g_pReport->AddString(STR_NEWLINE);

    }
    p->g_pReport->MakeReportTail();
    return TRUE;
}

int CTaskCreator::GenerateTaskInfoReport(STaskInfo *p,
                                         const char *pCmdLine, 
                                         int nINFO, 
                                         CDdaQuery *pQuery)
{
    static int nResPos = 0; //上一次的位置变量
    static int nPages  = 1; //共产生了多少张报文
    int i = 0; //循环变量;
    static UINTArray ResArray;
    UINTArray *pResArray;
    try{
        if(pQuery)
        {
            pResArray = &(pQuery->m_ResultArray);
        }
        else
        {
            if(nResPos == 0)
            {
                ResArray.clear();
                for(i = nResPos; i < m_pDdaSet->GetRecordCount(); i++)
                    ResArray.push_back(i);
            }
            pResArray = &ResArray;
        }
        /* 简要和详细的报文格式
        +++    HUAWEI iGWB        2002-06-10 16:49:53
        TASKCMD    #0
        %%LST TSK: CHT=1, INFO=0;%%
        RETCODE = 0  执行成功
        
        任务基本信息
        ------------
        任务名称  = 王峰                                                     
        任务号  =  0                                                            
        任务描述  =  2000年8月CG硬件性能
        任务开始时间 = 2002-08-01                                                
        运行状态  =  激活                                                         
        结果保留天数  =  5000                                                         
          
        任务名称  =  麦茂识                                                     
        任务号  =  1                                                            
        任务描述  =  2000年12月CG业务性能
        任务开始时间 = 2002-08-01                                              
        运行状态  =  激活                                                         
        结果保留天数  =  100
        (结果个数 = 2)
            
        ---    END
        */
        p->g_pReport->MakeReportHead();
        p->g_pReport->MakeCmdDisplay(pCmdLine,p->nReportCsn++,0,"O&M");
        STRING szTmp;
        const char *pTmp = NULL;
        int nVal = 0;
        int nItem = 0;
        for(i = nResPos; i < pResArray->size(); i++)
        {   nItem =  pResArray->at(i);
            MakeSubTitle(p->g_pReport,S_PERF_TASK_SIMPLE_INFO);
            p->g_pReport->MakeReportItem(S_PERF_TSKN,
                                         m_pDdaSet->GetFieldStringValue(nItem,"TSKN").c_str(),
                                         24,
                                         ALIGN_RIGHT);
            p->g_pReport->MakeReportItem(S_PERF_TSKDES,
                                         m_pDdaSet->GetFieldStringValue(nItem,"TSKDES").c_str(),
                                         24,
                                         ALIGN_RIGHT);
            p->g_pReport->MakeReportItem(S_PERF_CRE_TSK_USER,
                                         m_pDdaSet->GetFieldStringValue(nItem,"USER").c_str(),
                                         24,
                                         ALIGN_RIGHT);
            pTmp = ConvertDate(m_pDdaSet->GetFieldStringValue(nItem,"SD").c_str());
            p->g_pReport->MakeReportItem(S_PERF_SD,pTmp,24,ALIGN_RIGHT);
            nVal = atoi(m_pDdaSet->GetFieldStringValue(nItem,"RUN").c_str());
            if(nVal == TASK_NORMAL)
            {
                szTmp = S_PERF_STAT_ACT;
            }
            else
            {
                szTmp = S_PERF_STAT_SUS;
            }
            p->g_pReport->MakeReportItem(S_PERF_TSK_STSTUS,szTmp.c_str(),24,ALIGN_RIGHT);
            p->g_pReport->MakeReportItem(S_PERF_SRST,
                                         m_pDdaSet->GetFieldStringValue(nItem,"SRST").c_str(),
                                         24,
                                         ALIGN_RIGHT);

			//by ldf 204-04-23 M2000联调
            //p->g_pReport->AddString(STR_NEWLINE);
			//end
			
            //如果是详细格式还要添加如下信息
            if(nINFO)
            {
                /*
                时间信息
                --------
                任务类型  =  永久任务                                                       
                时间类型  =  按日统计                                                       
                起始日期  =  2001-01-01                                                     
                结束日期  =  <NULL>                                                         
                统计周期  =  60                                                             
                统计时间集合  =  <NULL>                                                         
                统计时间段  =  1                                                              
                起始时间1  =  00:00                                                          
                结束时间1  =  24:00                                                          
                起始时间2  =  <NULL>                                                         
                结束时间2  =  <NULL>                                                         
                起始时间3  =  <NULL>                                                         
                结束时间3  =  <NULL>                                                         
                结果保留时间  =  100
                */
                MakeSubTitle(p->g_pReport,S_PERF_TIME_INFO);
                int nTASKTP = atoi(m_pDdaSet->GetFieldStringValue(nItem,"TSKTP").c_str()); 
                if(nTASKTP) //如果是1就是有限周期
                {
                    p->g_pReport->MakeReportItem(S_PERF_TSKTP,S_PERF_LIMIT,24,ALIGN_RIGHT);
                }
                else
                {
                    p->g_pReport->MakeReportItem(S_PERF_TSKTP,S_PERF_PERMANENCE,24,ALIGN_RIGHT);
                }
                
                pTmp = ConvertTimeType(m_pDdaSet->GetFieldStringValue(nItem,"TT").c_str());
                p->g_pReport->MakeReportItem(S_PERF_TT,pTmp,24,ALIGN_RIGHT);

				STRING szDateTmp = m_pDdaSet->GetFieldStringValue(nItem,"SD");
                pTmp = ConvertDate(szDateTmp.c_str());
                p->g_pReport->MakeReportItem(S_PERF_SD,pTmp,24,ALIGN_RIGHT);

                if(nTASKTP)
                {
					//by ldf pTmp的内容格式已经变为了 2003-12-25，而不是20031225
                    pTmp = AddDaysToDBFData(szDateTmp.c_str(),atoi(m_pDdaSet->GetFieldStringValue(nItem,"LPT").c_str()));
                    szTmp = ConvertDate(pTmp);
                }
                else
                {
                    szTmp = S_PERF_TSK_NULL;
                }
                //by ldf 2004-04-24 M2000联调
                p->g_pReport->MakeReportItem(S_PERF_ED,szTmp.c_str(),24,ALIGN_RIGHT,50,ALIGN_LEFT,1);
                //end
                p->g_pReport->MakeReportItem(S_PERF_PRD,
                                             m_pDdaSet->GetFieldStringValue(nItem,"PRD").c_str(),
                                             24,
                                             ALIGN_RIGHT);
                //如果szTmp = ""，ConvertStatisticTimeSet返回<NULL>
                pTmp = ConvertStatisticTimeSet(m_pDdaSet->GetFieldStringValue(nItem,"STS").c_str());

                //by ldf 2004-04-24 M2000联调
                p->g_pReport->MakeReportItem(S_PERF_STS,pTmp,24,ALIGN_RIGHT,50,ALIGN_LEFT,1);
                //end
				
                p->g_pReport->MakeReportItem(S_PERF_SPD,
                                             m_pDdaSet->GetFieldStringValue(nItem,"SPD").c_str(),
                                             24,
                                             ALIGN_RIGHT);
                pTmp = ConvertStatisticTime(m_pDdaSet->GetFieldStringValue(nItem,"SP1S").c_str());
                p->g_pReport->MakeReportItem(S_PERF_SP1S,pTmp,24,ALIGN_RIGHT);
                pTmp = ConvertStatisticTime(m_pDdaSet->GetFieldStringValue(nItem,"SP1E").c_str());
                p->g_pReport->MakeReportItem(S_PERF_SP1E,pTmp,24,ALIGN_RIGHT);
                pTmp = ConvertStatisticTime(m_pDdaSet->GetFieldStringValue(nItem,"SP2S").c_str());
                p->g_pReport->MakeReportItem(S_PERF_SP2S,pTmp,24,ALIGN_RIGHT);
                pTmp = ConvertStatisticTime(m_pDdaSet->GetFieldStringValue(nItem,"SP2E").c_str());
                p->g_pReport->MakeReportItem(S_PERF_SP2E,pTmp,24,ALIGN_RIGHT);
                pTmp = ConvertStatisticTime(m_pDdaSet->GetFieldStringValue(nItem,"SP3S").c_str());
                p->g_pReport->MakeReportItem(S_PERF_SP3S,pTmp,24,ALIGN_RIGHT);
                pTmp = ConvertStatisticTime(m_pDdaSet->GetFieldStringValue(nItem,"SP3E").c_str());
                p->g_pReport->MakeReportItem(S_PERF_SP3E,pTmp,24,ALIGN_RIGHT);
                p->g_pReport->MakeReportItem(S_PERF_SRST,
                                             m_pDdaSet->GetFieldStringValue(nItem,"SRST").c_str(),
                                             24,
                                             ALIGN_RIGHT);            
                /*
                指标信息
                --------
                测量指标1  =  CPU1使用率（OFFINT）
                测量指标2  =  CPU2使用率（OFFINT）
                测量指标3  =  物理内存可用空间（OFFINT）
                测量指标4  =  物理内存总空间（OFFINT）
                测量指标5  =  前存盘可用空间（OFFINT）
                测量指标6  =  前存盘总空间（OFFINT）
                测量指标7  =  后存盘可用空间（OFFINT）
                测量指标8  =  后存盘总空间（OFFINT）
                测量指标9  =  接收话单张数（OFFINT）
                测量指标10  =  处理话单张数（OFFOUT）                                       
                */
                MakeSubTitle(p->g_pReport,S_PERF_COUNTER_INFO);
                char szCounterTitle[16];
                char szCounter[32] = "";
				//by ldf 2004-05-28
                memcpy(szCounter,m_pDdaSet->GetFieldStringValue(nItem,"ITM").c_str(),38);
                char *pSubTok = strtok(szCounter,"&");
                for(int j = 1; pSubTok; j++)
                {
                    pTmp = ConvertCounter(pSubTok);
                    SNPRINTF(szCounterTitle,16,"%s%d",S_PERF_ITM,j);
                    p->g_pReport->MakeReportItem(szCounterTitle,pTmp,24,ALIGN_RIGHT);
                    pSubTok = strtok(NULL,"&");
                }
                /*
                测量对象信息
                ------------
                测量对象实例1  =  硬件性能
                测量对象实例2  =  接入点1的业务性能                                      
                */
                MakeSubTitle(p->g_pReport,S_PERF_OBJECT_INFO);
                //先读MU的值确定是字符串
                VECTOR<int> vMu,vOIV;
                ConvertStrToVector(m_pDdaSet->GetFieldStringValue(nItem,"MU").c_str(),vMu);
                ConvertStrToVector(m_pDdaSet->GetFieldStringValue(nItem,"OIV").c_str(),vOIV);
                //要对应测试单元

                char szTitle[32];
                char szContent[32];
                int nOIV = 0;
                for(int k = 0; k < vMu.size(); k++)
                {
                    SNPRINTF(szTitle,32,S_PERF_INSTANCE,k+1);
					nOIV = vOIV[k];
					sprintf(szContent,"%d",nOIV);
                    p->g_pReport->MakeReportItem(szTitle,szContent,24,ALIGN_RIGHT);
                }
        }
        nResPos ++;
        if(p->g_pReport->GetReportLen() >= m_nPageLimit)
            break;
    }//for依次生成报文
    p->g_pReport->AddString(STR_NEWLINE);
    //by ldf 204-04-23 M2000联调
    //p->g_pReport->AddString(STR_NEWLINE);
    //end
	
    //如果是到了最后的记录,生成报文尾部
    if(nResPos >= pResArray->size())
    {
        char szTmp[32];
        SNPRINTF(szTmp,32,S_PERF_RESULT_NUM,pResArray->size());
        p->g_pReport->AddString(szTmp);
        p->g_pReport->AddString(STR_NEWLINE);
        SNPRINTF(szTmp,32,s_PERF_PAGE_NUM,nPages);
        p->g_pReport->AddString(szTmp);
        p->g_pReport->AddString(STR_NEWLINE);
        nResPos = 0;
        nPages = 1;
        p->g_pReport->AddString(MML_END);
        if(!pQuery)
        {
            ResArray.clear();
        }
        return REPORT_NONE;
    }
    else
    {
        p->g_pReport->AddString(S_PERF_REPORT_MORE);
        p->g_pReport->AddString(STR_NEWLINE);
        nPages ++;
        p->g_pReport->AddString(MML_END);
        return REPORT_MORE;
    }
  }
  catch(std::exception &e)
  {
       m_szError = e.what();
       return REPORT_ERR;
  }

}

void CTaskCreator::ConvertStrToVector(const char *pStr, VECTOR<int> &Vector)
{
    char szTmp[16];
    strncpy(szTmp,pStr,16);
    char *pTok = strtok(szTmp,"&");
    while(pTok)
    {
        Vector.push_back(atoi(pTok));
        pTok = strtok(NULL,"&");
    }
}

const char *CTaskCreator::ConvertDate(const char *pDate, const char *pSeparater, int nNation)
{
    int nDate = atoi(pDate);
    int nYear = nDate / 10000;
    int nMonth = (nDate % 10000)/100;
    int nDay = nDate % 100;
    if(nNation)
    {
        SNPRINTF(m_szConvertCatch,CONVERT_CATCH_LEN,"%04d%s%02d%s%02d",
                 nYear,pSeparater,nMonth,pSeparater,
                 nDay, pSeparater);   
    }
    else
    {
        SNPRINTF(m_szConvertCatch,CONVERT_CATCH_LEN,"%02d%s%02d%s%04d",
                 nMonth,pSeparater,nDay,pSeparater,
                 nYear,pSeparater);
    }
    return m_szConvertCatch;
}
//
const char *CTaskCreator::AddDaysToDBFData(const char *pDBFData, int nDays)
{
    int nDate = atoi(pDBFData);
    int nYear = nDate / 10000;
    int nMonth = (nDate % 10000)/100;
    int nDay = nDate % 100;
    tm tmDate;
    memset(&tmDate,0,sizeof(tm));
    tmDate.tm_year = nYear - 1900;
    tmDate.tm_mon = nMonth - 1;
    //by ldf 2004-04-24 M2000联调，应M2000要求，当天也算，所以CG少加一天
    //要少加一天才会在数字,即如果为90天的，只加89天
    tmDate.tm_mday = nDay -1;
    //end
	
    time_t tTime = mktime(&tmDate);
    tTime += nDays * 24 * 3600; 
    tmDate = *localtime(&tTime);
    strftime(m_szConvertCatch,CONVERT_CATCH_LEN,"%Y%m%d",&tmDate);
    return m_szConvertCatch;
}

const char *CTaskCreator::ConvertTimeType(const char *pTT)
{
    switch(atoi(pTT)) {
    case TT_DAY: //
        strncpy(m_szConvertCatch,S_PERF_TT0,CONVERT_CATCH_LEN);
        break;
    case TT_WEEK:
        strncpy(m_szConvertCatch,S_PERF_TT1,CONVERT_CATCH_LEN);
    	break;
    default:
        strncpy(m_szConvertCatch,S_PERF_TT2,CONVERT_CATCH_LEN);
    }
    return m_szConvertCatch;
}

const char *CTaskCreator::ConvertStatisticTimeSet(const char *pSTS)
{
    char szTmp[512] = "";
    strncpy(szTmp,pSTS,512);
    char *pTok = strtok(szTmp,"&");
    if(!pTok)
    {
        strncpy(m_szConvertCatch,S_PERF_TSK_NULL,CONVERT_CATCH_LEN);
        return m_szConvertCatch;
    }
    int nPos = 0;
    int nVal = 0;
    while(pTok)
    {
        nVal = atoi(pTok);
        switch(nVal) 
        {
        case 0:
            //by ldf 2004-04-26 M2000联调过程中发现的一个异常，
            //CONVERT_CATCH_LEN 1024,
            //这里目前个字段的和，能局限于1024的长度，不然会有内存问题
            strncpy(&m_szConvertCatch[nPos],S_PERF_SUN,CONVERT_CATCH_LEN-nPos);
            nPos += strlen(S_PERF_SUN);
        	break;
        case 1:
            strncpy(&m_szConvertCatch[nPos],S_PERF_MON,CONVERT_CATCH_LEN-nPos);
            nPos += strlen(S_PERF_MON);
        	break;
        case 2:
            strncpy(&m_szConvertCatch[nPos],S_PERF_TUE,CONVERT_CATCH_LEN-nPos);
            nPos += strlen(S_PERF_TUE);
        	break;
        case 3:
            strncpy(&m_szConvertCatch[nPos],S_PERF_WED,CONVERT_CATCH_LEN-nPos);
            nPos += strlen(S_PERF_WED);
        	break;
        case 4:
            strncpy(&m_szConvertCatch[nPos],S_PERF_THU,CONVERT_CATCH_LEN-nPos);
            nPos += strlen(S_PERF_THU);
        	break;
        case 5:
            strncpy(&m_szConvertCatch[nPos],S_PERF_FRI,CONVERT_CATCH_LEN-nPos);
            nPos += strlen(S_PERF_FRI);
        	break;
        case 6:
            strncpy(&m_szConvertCatch[nPos],S_PERF_SAT,CONVERT_CATCH_LEN-nPos);
            nPos += strlen(S_PERF_SAT);
        	break;
        default:
            //by ldf 2004-04-24 M2000联调，这里可能是Macro.ini的问题
            nVal -= 7; //月份中的日期从7开始为1号
            //end
            SNPRINTF(&m_szConvertCatch[nPos],CONVERT_CATCH_LEN - nPos - 1,
                     "%02d  ",nVal);
            nPos += 2;
        }

        pTok = strtok(NULL,"&");
		
		//by ldf 2003-12-25 在每个之间加 “,” 分开
		if(pTok)
		{
			strncpy(&m_szConvertCatch[nPos], "," , 1);
			nPos ++;
		}
    }
    return m_szConvertCatch;
}

const char *CTaskCreator::ConvertStatisticTime(const char *pTime)
{
    if(strcmp(pTime,"") == 0)
    {
        strncpy(m_szConvertCatch,S_PERF_TSK_NULL,CONVERT_CATCH_LEN);
    }
    else
    {
        strncpy(m_szConvertCatch,pTime,CONVERT_CATCH_LEN);
    }
    return m_szConvertCatch;
}
//
void CTaskCreator::MakeSubTitle(CMMLReport *pReport,const char *pContent)
{
    pReport->AddString(STR_NEWLINE);
    pReport->AddString(pContent);
    pReport->AddString(STR_NEWLINE);

	//by ldf 
	int nStrLen = strlen(pContent);
	for(int j = 0; j < nStrLen; j++)
	{
		pReport->AddString("-");
	}
	//pReport->AddString(STR_TITLE_SPIN);
	//end

    pReport->AddString(STR_NEWLINE);
}
//
const char * CTaskCreator::ConvertCounter(const char *pCounter)
{
    int nVal = atoi(pCounter);
    switch(nVal) 
    {
    case 1:
        strncpy(m_szConvertCatch,S_PERF_COUNTER_CPU1,CONVERT_CATCH_LEN);
    	break;
    case 2:
        strncpy(m_szConvertCatch,S_PERF_COUNTER_CPU2,CONVERT_CATCH_LEN);
    	break;
    case 3:
        strncpy(m_szConvertCatch,S_PERF_COUNTER_PMA,CONVERT_CATCH_LEN);
    	break;
    case 4:
        strncpy(m_szConvertCatch,S_PERF_COUNTER_PMT,CONVERT_CATCH_LEN);
    	break;
    case 5:
        strncpy(m_szConvertCatch,S_PERF_COUNTER_FSA ,CONVERT_CATCH_LEN);
    	break;
    case 6:
        strncpy(m_szConvertCatch,S_PERF_COUNTER_FST,CONVERT_CATCH_LEN);
    	break;
    case 7:
        strncpy(m_szConvertCatch,S_PERF_COUNTER_BSA,CONVERT_CATCH_LEN);
    	break;
    case 8:
        strncpy(m_szConvertCatch,S_PERF_COUNTER_BST,CONVERT_CATCH_LEN);
    	break;
	case 9:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RSCDR,CONVERT_CATCH_LEN);
    	break;
    case 10:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RPSCDR,CONVERT_CATCH_LEN);
		break;
	case 11:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RGCDR,CONVERT_CATCH_LEN);
		break;
	case 12:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RPGCDR,CONVERT_CATCH_LEN);
		break;
	case 13:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RMCDR,CONVERT_CATCH_LEN);
		break;
	case 14:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RPMCDR,CONVERT_CATCH_LEN);
		break;
	case 15:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RSMO,CONVERT_CATCH_LEN);
		break;
	case 16:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RSMT,CONVERT_CATCH_LEN);
		break;
	case 17:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RLCSMO,CONVERT_CATCH_LEN);
		break;
	case 18:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RLCSMT,CONVERT_CATCH_LEN);
		break;
	case 19:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RLCSNI,CONVERT_CATCH_LEN);
    	break;
    default:
        break;
    }
    return m_szConvertCatch;
}

void CTaskCreator::QueryCounterInfo(STRING &TaskName, LIST<STRING> &listCounter)
{
    m_Query.ClearBind();
    m_Query.AddBindAnd("TSKN",TaskName,opEqual);
    int nItem = m_Query.QueryFirst();
    STRING strCounter = m_pDdaSet->GetFieldStringValue(nItem,"ITM");
    char szTmp[32];
    strncpy(szTmp,strCounter.c_str(),32);
    char *pTok = strtok(szTmp,"&");
    while(pTok != NULL)
    {
        listCounter.push_back(ConvertCounter(pTok));
        pTok = strtok(NULL,"&");
    }
}
STRING CTaskCreator::QueryTaskInfo(STRING &TaskName, STRING &Field)
{
    m_Query.ClearBind();
    m_Query.AddBindAnd("TSKN",TaskName,opEqual);
    int nItem = m_Query.QueryFirst();
    return m_pDdaSet->GetFieldStringValue(nItem,Field.c_str());
}

int CTaskCreator::SaveTask(STRING &szTskn)
{
    m_Query.ClearBind();
    m_Query.AddBindAnd("TSKN",szTskn,opEqual);
    int nItem = m_Query.QueryFirst();
    return m_pDdaSet->UpdateRecord(nItem);
}

void CTaskCreator::SetTaskFieldValue(STRING &szTskn, STRING &szField, STRING &szValue)
{
    m_Query.ClearBind();
    m_Query.AddBindAnd("TSKN",szTskn,opEqual);
    int nItem = m_Query.QueryFirst();
    m_pDdaSet->SetFieldStringValue(nItem,szField.c_str(),szValue);
}
