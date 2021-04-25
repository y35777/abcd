// dda_proxy.cpp: implementation of the CCreateProxy class.
//
//////////////////////////////////////////////////////////////////////

#include "dda_proxy.h"
#include "perf_task.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCreateProxy::CCreateProxy()
{

}

CCreateProxy::~CCreateProxy()
{

}
CDdaSet* CCreateProxy::Init(STRING &szDdaFilePath, TaskMap *pmTaskMap)
{
 m_pDdaSet = new CDdaSet;
 m_szFilePath = szDdaFilePath;
 try
 {
    //1.利用状态文件的全路径名szDdaFilePath，创建m_pDdaSet对象
    //调用m_pDdaSet->LoadFromFile(szDdaFileName);
    if(m_pDdaSet->LoadFromFile(szDdaFilePath) != TRUE)
    {//2.如果返回失败，就执行第3步。如果成功就执行6。      
        //3.按照m_pDdaSet>Open("d:/PerfStatus.Dda", IOS_BASE::in | IOS_BASE::out | IOS_BASE::binary | IOS_BASE::trunc )
        //格式创建状态文件.
		//by ldf 2003-12
		BOOL bRetOpen = m_pDdaSet->Open(szDdaFilePath.c_str(),"wb");
		m_pDdaSet->m_strSetName = szDdaFilePath;

        if( bRetOpen != TRUE)
            throw strerror(errno);
        
        //第1个域
        DB_FIELD *pField = new DB_FIELD;
        pField->Name = "TSKN";
        pField->Len = TSKN_LEN;
        pField->Dot = 0;
        pField->Type = dbChar;
        m_pDdaSet->AddField(pField);
        //第2个域
        pField = new DB_FIELD;
        pField->Name = "TSKDES";
        pField->Len = TSKDES_LEN;
        pField->Dot = 0;
        pField->Type = dbChar;
        m_pDdaSet->AddField(pField);
        //第3个域
        pField = new DB_FIELD;
        pField->Name = "USER";
        pField->Len = USER_LEN;
        pField->Dot = 0;
        pField->Type = dbChar;
        m_pDdaSet->AddField(pField);
        //第4个域
        pField = new DB_FIELD;
        pField->Name = "MU";
        pField->Len = MU_LEN;
        pField->Dot = 0;
        pField->Type = dbChar;
        m_pDdaSet->AddField(pField);
        //第5个域
        pField = new DB_FIELD;
        pField->Name = "RUN";
        pField->Len = RUN_LEN;
        pField->Dot = 0;
        pField->Type = dbNumber;
        m_pDdaSet->AddField(pField);
        //第6个域
        pField = new DB_FIELD;
        pField->Name = "PRD";
        pField->Len = PRD_LEN;
        pField->Dot = 0;
        pField->Type = dbNumber;
        m_pDdaSet->AddField(pField);
        //第7个域
        pField = new DB_FIELD;
        pField->Name = "SD";
        pField->Len = SD_LEN;
        pField->Dot = 0;
        pField->Type = dbData;
        m_pDdaSet->AddField(pField);
        //第8个域
        pField = new DB_FIELD;
        pField->Name = "SPD";
        pField->Len = SPD_LEN;
        pField->Dot = 0;
        pField->Type = dbNumber;
        m_pDdaSet->AddField(pField);
        //第9个域
        pField = new DB_FIELD;
        pField->Name = "SP1S";
        pField->Len = SP_LEN;
        pField->Dot = 0;
        pField->Type = dbChar;
        m_pDdaSet->AddField(pField);
        //第10个域
        pField = new DB_FIELD;
        pField->Name = "SP1E";
        pField->Len = SP_LEN;
        pField->Dot = 0;
        pField->Type = dbChar;
        m_pDdaSet->AddField(pField);
        //第11个域
        pField = new DB_FIELD;
        pField->Name = "SP2S";
        pField->Len = SP_LEN;
        pField->Dot = 0;
        pField->Type = dbChar;
        m_pDdaSet->AddField(pField);
        //第12个域
        pField = new DB_FIELD;
        pField->Name = "SP2E";
        pField->Len = SP_LEN;
        pField->Dot = 0;
        pField->Type = dbChar;
        m_pDdaSet->AddField(pField);
        //第13个域
        pField = new DB_FIELD;
        pField->Name = "SP3S";
        pField->Len = SP_LEN;
        pField->Dot = 0;
        pField->Type = dbChar;
        m_pDdaSet->AddField(pField);
        //第14个域
        pField = new DB_FIELD;
        pField->Name = "SP3E";
        pField->Len = SP_LEN;
        pField->Dot = 0;
        pField->Type = dbChar;
        m_pDdaSet->AddField(pField);
        //第15个域
        pField = new DB_FIELD;
        pField->Name = "TSKTP";
        pField->Len = TSKTP_LEN;
        pField->Dot = 0;
        pField->Type = dbNumber;
        m_pDdaSet->AddField(pField);
        //第16个域
        pField = new DB_FIELD;
        pField->Name = "LPT";
        pField->Len = LPT_LEN;
        pField->Dot = 0;
        pField->Type = dbNumber;
        m_pDdaSet->AddField(pField);
        //第17个域
        pField = new DB_FIELD;
        pField->Name = "TT";
        pField->Len = TT_LEN;
        pField->Dot = 0;
        pField->Type = dbNumber;
        m_pDdaSet->AddField(pField);
        //第18个域
        pField = new DB_FIELD;
        pField->Name = "STS";
        pField->Len = STS_LEN;
        pField->Dot = 0;
        pField->Type = dbChar;
        m_pDdaSet->AddField(pField);
        //第19个域
        pField = new DB_FIELD;
        pField->Name = "SRST";
        pField->Len = SRST_LEN;
        pField->Dot = 0;
        pField->Type = dbNumber;
        m_pDdaSet->AddField(pField);
        //第20个域
        pField = new DB_FIELD;
        pField->Name = "ITM";
        pField->Len = ITM_LEN;
        pField->Dot = 0;
        pField->Type = dbChar;
        m_pDdaSet->AddField(pField);
        //第21个域
        pField = new DB_FIELD;
        pField->Name = "OIV";
        pField->Len = OIV_LEN;
        pField->Dot = 0;
        pField->Type = dbChar;
        m_pDdaSet->AddField(pField);
        //最后调用m_pDdaSet->SyncToFile()。
        if(!m_pDdaSet->SyncToFile())
            throw strerror(errno);
        return m_pDdaSet;
    }
    //6.调用数据库文件成功，循环m_pDdaSet->GetRecordCount（）次，
    //调用m_pDdaSet->GetFieldStringValue(int nItem, const char* lpszFieldName)，
    //获得每一个域的字符串值，按照SInformation的各个域的要求
    //初始化CPerfTask，对于型为“xx&yy&zz”的字符串使用strtok
    //函数分割，对于m_mCouterMap中指标的转换请参照base_type.h
    //中的性能统计项ID进行转换注意，Perf模块要修改一下，因为
    //内存等指标没有ID，也没有发送。生成TaskMap。
    const char *pcTmp = NULL;
	SInformation *pInformation = NULL;
    CPerfTask *pTask= NULL; 
    try
    {
      for(int i = 0; i < m_pDdaSet->GetRecordCount(); i++)
      {
        pInformation = new SInformation;
        //TSKN(任务名称)：字符类型16个字符
        pInformation->m_szTaskName = m_pDdaSet->GetFieldStringValue(i,"TSKN");
        //TSKDES(任务描述)： 字符类型48个字符
        pInformation->m_szTaskDes = m_pDdaSet->GetFieldStringValue(i,"TSKDES");
        pInformation->m_szUser = m_pDdaSet->GetFieldStringValue(i,"USER");
        //MU(测量单元)： 字符类型16个字符
        STRING sMU = m_pDdaSet->GetFieldStringValue(i,"MU");
        AnalysisMU(pInformation,sMU.c_str());
        //RUN(是否立即运行)：整型 1
        pInformation->m_nStatus = atoi(m_pDdaSet->GetFieldStringValue(i,"RUN").c_str());
        //PRD(统计周期)：整型 2
        pInformation->m_nPeriod = atoi(m_pDdaSet->GetFieldStringValue(i,"PRD").c_str());
        //SD(起始日期)： 日期型，例如“YYYY/MM/DD”	8
        STRING sDate = m_pDdaSet->GetFieldStringValue(i,"SD");
        int nDate = atoi(sDate.c_str());
        tm tmDate;
        memset(&tmDate,0,sizeof(tm));
        tmDate.tm_year = nDate/10000 - 1900;
        tmDate.tm_mon = (nDate % 10000) / 100 - 1;
        tmDate.tm_mday = nDate % 100;
        pInformation->m_tStartTime = mktime(&tmDate);
        //SPD(统计时间段数)：整型 1
        int nSpanCount = atoi(m_pDdaSet->GetFieldStringValue(i,"SPD").c_str());
        char szFieldName[8];
        sTimeSpan sTime;
        memset(&sTime,0,sizeof(sTimeSpan));
        STRING szTime;
        //SP1S(起始时间1)：字符型，例如“HH:MM”5*3
        for(int j = 1; j <= nSpanCount; j++)
        {
            SNPRINTF(szFieldName,8,"SP%1dS",j);
            szTime = m_pDdaSet->GetFieldStringValue(i,szFieldName);
            sTime.tStartTime.tm_hour = atoi(szTime.substr(0,2).c_str());
            sTime.tStartTime.tm_min = atoi(szTime.substr(3,2).c_str());
            SNPRINTF(szFieldName,8,"SP%1dE",j);
            szTime = m_pDdaSet->GetFieldStringValue(i,szFieldName);
            sTime.tEndTime.tm_hour = atoi(szTime.substr(0,2).c_str());
            sTime.tEndTime.tm_min = atoi(szTime.substr(3,2).c_str());
            pInformation->m_vTimeSpan.push_back(sTime);
        }
        //TSKTP(任务类型)： 整型 1
        pInformation->m_nTaskType = atoi(m_pDdaSet->GetFieldStringValue(i,"TSKTP").c_str());
        //LPT(有限周期天数)： 整型 3
        pInformation->m_nDayLimit = atoi(m_pDdaSet->GetFieldStringValue(i,"LPT").c_str());
        //TT（时间类型）： 整型 1
        pInformation->m_nTimeType = atoi(m_pDdaSet->GetFieldStringValue(i,"TT").c_str());
        //STS（统计时间集合）：	 字符型，例如“1&2&3&4” 90
        AnalysisTimeSet(pInformation,m_pDdaSet->GetFieldStringValue(i,"STS").c_str());
        //SRST（保留结果天数）： 整型 3
        pInformation->m_nResultSaveDays = atoi(m_pDdaSet->GetFieldStringValue(i,"SRST").c_str());
        //ITM（测量指标）： 字符型，例如“1&2&3&4”20
        AnalysisITM(pInformation,m_pDdaSet->GetFieldStringValue(i,"ITM").c_str());
        //OIV （测量对象实例值）：字符型，例如“1&1”5
        AnalysisOIV(pInformation,m_pDdaSet->GetFieldStringValue(i,"OIV").c_str());
        
        pTask = new CPerfTask;
        if(!pTask->Init(pInformation,m_szFilePath))
        {
            TRACE(MTS_PERF,"Failed to init Task(%s)",
                  pInformation->m_szTaskName.c_str());
            continue;
        }
        pmTaskMap->insert(TaskMap::value_type(pInformation->m_szTaskName,pTask));
      } //生成 Information    
      //返回m_pDdaSet
      return m_pDdaSet;
    }
    catch (const char *pError) 
    {
       //消去warning C4101: 'pError' : unreferenced local variable
       const char *p = pError;
       delete pInformation,pInformation = NULL;
       throw;
    }
   //生成任务信息
 }
 catch(const char *pError)
 {
    m_szError = pError;
    delete m_pDdaSet,m_pDdaSet = NULL;
    return NULL;
 }
}

void CCreateProxy::AnalysisMU(SInformation *pInfo, const char *pMU)
{
    char szMU[256];
    strncpy(szMU,pMU,256);
    char *pTok = strtok(szMU,"&");
    int nVal = 0;
    while(pTok)
    {
        nVal = atoi(pTok);
        if(nVal > 2 || nVal < 1)
            throw "MU overflow";
        pInfo->m_vMU.push_back(nVal);
        pTok = strtok(NULL,"&"); 
    }
}

void CCreateProxy::AnalysisTimeSet(SInformation *pInfo, const char *pTimeSet)
{
    char szTimeSet[512];
    strncpy(szTimeSet,pTimeSet,512);
    char *pTok = strtok(szTimeSet,"&");
    int nVal = 0;
    while(pTok)
    {
        nVal = atoi(pTok);
        if(nVal > 38 || nVal < 0)
            throw "TimeSet error";
        pInfo->m_vTimeSet.push_back(nVal);
        pTok = strtok(NULL,"&"); 
    }
}

void CCreateProxy::AnalysisITM(SInformation *pInfo, const char *pITM)
{
    char szITM[256];
    strncpy(szITM,pITM,256);
    char *pTok = strtok(szITM,"&");
    int nVal = 0;
    while(pTok)
    {
        nVal = atoi(pTok);
		//by ldf 2004-05-15 Perf合入CDR统计，单元测试修改，此处的19为统计项的个数
        if(nVal > 19 || nVal < 1)
            throw "ITM error";
        pInfo->m_mCouterMap[nVal] = 0;
        pTok = strtok(NULL,"&"); 
    }
}

void CCreateProxy::AnalysisOIV(SInformation *pInfo, const char *pOIV)
{
    //测试单元为2&1,测试对象实例为4&1
    //那么 m_vMU[0] = 2, m_vMU[1] = 1
    //则m_mObjectInstance[2] = 4, m_mObjectInstance[1] = 1
    //m_mObjectInstance的下标表示单元ID
    char szOIV[256];
    strncpy(szOIV,pOIV,256);
    char *pTok = strtok(szOIV,"&");
    int nInstance = 0;
    int nMu = 0; //
    for(int i = 0; pTok ; i++)
    {
        nInstance = atoi(pTok);
#ifdef _PLATFORM_LINUX
        if(pInfo->m_vMU.size() <= i)
        {
            throw "OIV != MU";
        }
        nMu = pInfo->m_vMU[i];
#else        
        try{
            nMu = pInfo->m_vMU[i];
        }
        catch(std::out_of_range)
        {
            throw "OIV != MU";
        }
#endif
        pInfo->m_mObjectInstance[nMu] = nInstance;
        pTok = strtok(NULL,"&"); 
    }
}
CPerfTask* CCreateProxy::CreateTask(SInformation *pInfo)
{
    if(m_pDdaSet == NULL)
    {
        m_szError = "DdaSet is NULL";
        return NULL;
    }
    //返回新创建的CPerfTask，创建失败返回NULL；
    CPerfTask *pTask = new CPerfTask;
    DB_RECORD *pRecord = new(m_pDdaSet->m_nRecordLength) DB_RECORD;
    //调用CPerfTask的Init函数(参数为m_szFilePath)，
    //如果成功就返回指针，否则删除这个指针返回NULL;
    try{
        //根据参数中的信息创建CPerfTask
        if(pTask->Init(pInfo,m_szFilePath) != TRUE)
            throw pTask->GetLastError();    
        //一条记录有220个字节。
        char *pBuf = pRecord->data;
        //TSKN(任务名称)：字符类型16个字符
        SNPRINTF(pBuf,TSKN_LEN,StrFormat(TSKN_LEN,"s"),(pInfo->m_szTaskName).c_str());
        pBuf += TSKN_LEN;
        //TSKDES(任务描述)： 字符类型48个字符
        SNPRINTF(pBuf,TSKDES_LEN,StrFormat(TSKDES_LEN,"s"),(pInfo->m_szTaskDes).c_str());
        pBuf += TSKDES_LEN;
        SNPRINTF(pBuf,TSKDES_LEN,StrFormat(USER_LEN,"s"),(pInfo->m_szUser).c_str());
        pBuf += USER_LEN;
        //MU(测量单元)： 字符类型16字符
        pBuf = PrintIntVector(pBuf,pInfo->m_vMU,MU_LEN);
        //RUN(是否立即运行)：整型 1
        SNPRINTF(pBuf,RUN_LEN,StrFormat(RUN_LEN,"d"),pInfo->m_nStatus);
        pBuf += RUN_LEN;
        //PRD(统计周期)：整型 4
        SNPRINTF(pBuf,PRD_LEN,StrFormat(PRD_LEN,"d"),pInfo->m_nPeriod);
        pBuf += PRD_LEN;
        //SD(起始日期)： 日期型，例如“YYYY/MM/DD”	8
        tm tmDate = *localtime(&(pInfo->m_tStartTime));
        strftime(pBuf,SD_LEN + 2,"%Y%m%d",&tmDate);
        pBuf += SD_LEN;
        //SPD(统计时间段数)：整型 1
        SNPRINTF(pBuf,SPD_LEN,StrFormat(SPD_LEN,"d"),(pInfo->m_vTimeSpan).size());
        pBuf += SPD_LEN;
        //SP1S(起始时间1)：字符型，例如“HH:MM”5
        //SP1E(结束时间1)：字符型，例如“HH:MM”5
        //SP2S(起始时间2)：字符型，例如“HH:MM”5
        //SP2E(结束时间2)：字符型，例如“HH:MM”5
        //SP3S(起始时间3)：字符型，例如“HH:MM”5
        //SP3E(结束时间3)：字符型，例如“HH:MM”5 /total 30
        pBuf = PrintTimeSpan(pBuf,pInfo);
        //TSKTP(任务类型)： 整型 1
        SNPRINTF(pBuf,TSKTP_LEN,StrFormat(TSKTP_LEN,"d"),pInfo->m_nTaskType);
        pBuf += TSKTP_LEN;
        //LPT(有限周期天数)： 整型 4
        SNPRINTF(pBuf,LPT_LEN,StrFormat(LPT_LEN,"d"),pInfo->m_nDayLimit);
        pBuf += LPT_LEN;
        //TT（时间类型）： 整型 1
        SNPRINTF(pBuf,TT_LEN,StrFormat(TT_LEN,"d"),pInfo->m_nTimeType);
        pBuf += TT_LEN;
        //STS（统计时间集合）：	 字符型，例如“1&2&3&4” 90
        pBuf = PrintIntVector(pBuf,pInfo->m_vTimeSet,STS_LEN);
        //SRST（保留结果天数）： 整型 2
        SNPRINTF(pBuf,SRST_LEN,StrFormat(SRST_LEN,"d"),pInfo->m_nResultSaveDays);
        pBuf += SRST_LEN;
        //ITM（测量指标）： 字符型，例如“1&2&3&4”20
        pBuf = PrintMapKey(pBuf,pInfo->m_mCouterMap,ITM_LEN);
        //OIV （测量对象实例值）：字符型，例如“1&1”5
        //对象实例的排列要对应测量单元
        OIVMap::iterator it = pInfo->m_mObjectInstance.begin();
        char szTmp[256];
        char *szBuf = &szTmp[0];
        int nMu = 0;
        int nObj = 0;
        for(int j = 0;j < pInfo->m_vMU.size(); j++)
        {
            nMu = pInfo->m_vMU[j];
            nObj = pInfo->m_mObjectInstance[nMu];
            SNPRINTF(szBuf,5,"%-d&",nObj);
            if(nObj >= 10)
                szBuf += 3;
            else
                szBuf += 2;
        }
        szBuf--;
        memset(szBuf,' ',&szTmp[OIV_LEN] - szBuf);
        memcpy(pBuf,szTmp,OIV_LEN);
        pBuf += OIV_LEN;
        if(!m_pDdaSet->AppendRecordToDisk(pRecord))
            throw strerror(errno);
    }
    catch(const char *pError)
    {
        m_szError = pError;
        delete pTask, pTask = NULL;
        delete pRecord,pRecord = NULL;
    }
    return pTask;
}

char *CCreateProxy::PrintIntVector(char *pOut, VECTOR<int> &IntVec, int nLen, char cSep)
{
    char szTmp[256];
    char *pBuf = &szTmp[0];
    for(int i = 0; i < IntVec.size(); i++)
    {
        SNPRINTF(pBuf,5,"%-d%-c",IntVec[i],cSep);
        if(IntVec[i] >= 10)
            pBuf += 3;
        else
            pBuf += 2;
        
    }
    pBuf --; //因为会多一个'&'，所以要将最后一个'&'变为' '
    memset(pBuf,' ',&szTmp[nLen] - pBuf);
    //*pBuf = ' ';
    memcpy(pOut,szTmp,nLen);
    return pOut + nLen;
}

char *CCreateProxy::PrintTimeSpan(char *pOut, SInformation *pInfo)
{
    char *pBuf = pOut;
    int nSpan = pInfo->m_vTimeSpan.size();
    sTimeSpan *pTS = NULL;
    for(int i = 0; i < nSpan; i++)
    {
        pTS = &(pInfo->m_vTimeSpan[i]);
        strftime(pBuf,SP_LEN + 2,"%H:%M",&(pTS->tStartTime));
        pBuf += SP_LEN;
        strftime(pBuf,SP_LEN + 2,"%H:%M",&(pTS->tEndTime));
        pBuf += SP_LEN;
    }
    char *pMax = pOut + (6 * SP_LEN);
    memset(pBuf,' ',pMax-pBuf);
    pBuf = pMax;
    return pBuf;
}
int CCreateProxy::DeleteTask(STRING szTaskName)
{
    //从nItem = 0到最后一条记录，利用
    int nItem = 0;
    int nRet = FALSE;
    //m_pDdaSet->GetFieldStringValue(int nItem,“任务名称”)
    for(;nItem < m_pDdaSet->GetRecordCount(); nItem ++)
    {
        //依次比较返回值与szTaskName，如果相等
        if(szTaskName.compare(m_pDdaSet->GetFieldStringValue(nItem,"TSKN")) == 0)
        {
            //调用m_pDdaSet->DeleteRecord(nItem)将记录删除
            if(!m_pDdaSet->DeleteRecord(nItem))
            {
                m_szError = strerror(errno);
                return FALSE;  
            }
            m_pDdaSet->SyncToFile();
            nRet = TRUE;
            break;
        }
    }
    if(nRet == FALSE)
    {
        //表示没有这个任务
        //by ldf 2004-04-26 M2000V2B02联调
        m_szError = S_PERF_NOTHISTASK; //"no this task"
        //end
    }
    return nRet;
	
}
//////////////////////////////////////////////////////////////////////
// CSaveProxy Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSaveProxy::CSaveProxy()
{
    m_pDdaSet = new CDdaSet;
}

CSaveProxy::~CSaveProxy()
{
    if(m_pDdaSet)
        delete m_pDdaSet,m_pDdaSet = NULL;
}

int CSaveProxy::Init(STRING &szTaskName, STRING &szRootPath,CounterMap *pmCounterMap)
{
    m_szTaskName = szTaskName;
    m_pCounterMap = pmCounterMap;
    m_nRecordLen = 1 + 2*SETIME_LEN + m_pCounterMap->size() * COUNTER_LEN; 
    //首先看看对应某个性能任务的子目录是否存在
    m_szFilePath = szRootPath;
    CreateDeepDir(m_szFilePath.c_str());
    
    //看看文件是否存在 任务名_年月日.prf
    time_t tLastSaveTime;
    time(&tLastSaveTime);
    m_tLastSaveTime = *localtime(&tLastSaveTime);
    m_szFileName = GenerateFileName(tLastSaveTime);
    return CreatePerfFile(m_szFileName);
}
int CSaveProxy::Save(const time_t &tStart, const time_t &tEnd)
{ 
    tm tmEndDate = *localtime(&tEnd);
    if(tmEndDate.tm_mday != m_tLastSaveTime.tm_mday)
    {
        //by ldf  否则，跨天后前一天的信息丢失,因为CreatePerfFile(szFileName)中会
        //生成文件，在同步文件时，还利用的是旧的m_szFileName
        STRING szFileName = GenerateFileName(tEnd);
        m_szFileName = szFileName;

        if(!CreatePerfFile(szFileName))
        {
            m_szError = strerror(errno);
            return FALSE;
        }
	}

    //by ldf 2003-12
    BOOL bRetOpen = m_pDdaSet->Open(m_szFileName.c_str(),"rb+");
    m_pDdaSet->m_strSetName = m_szFileName;

    if(!bRetOpen)
    {
        m_szError = strerror(errno);
        return FALSE;
    }
    //1.首先DB_RECORD结构，然后按照前面要求的格式生成一条Dda格式
    //记录： 统计的起始时分秒  统计的结束时分秒  
    //性能指标1的值 性能指标2的值  性能指标3的值....
    DB_RECORD *pRecord = new(m_nRecordLen) DB_RECORD;
    char *pBuf = pRecord->data;
    tm tmStartDate = *localtime(&tStart);
    char szDate[16];
    strftime(szDate,16,"%H:%M:%S",&tmStartDate);
    memcpy(pBuf,szDate,SETIME_LEN);
    pBuf += SETIME_LEN;
    strftime(szDate,16,"%H:%M:%S",&tmEndDate);
    memcpy(pBuf,szDate,SETIME_LEN);
    pBuf += SETIME_LEN;
    int nVal = 0;
    for(CounterMap::iterator it = m_pCounterMap->begin(); it != m_pCounterMap->end(); it++)
    {
        nVal = int(it->second * 100);
        SNPRINTF(pBuf,COUNTER_LEN,"%-20.2f",float(nVal)/100);
        pBuf += COUNTER_LEN;
    }
    //3.m_pDdaSet->AppendRecordToDisk(DB_RECORD)；
    //将这个记录加到文件里
    if(m_pDdaSet->AppendRecordToDisk(pRecord))
    {
        m_tLastSaveTime = tmEndDate;
        return TRUE;
    }
    m_szError = strerror(errno);
    return FALSE;
}
STRING CSaveProxy::GenerateFileName(time_t tTime)
{
	char szFileName[MAX_PATH];
	tm tmTime = *localtime(&tTime);
	char szDate[32];
	strftime(szDate,32,"%Y%m%d",&tmTime);
	SNPRINTF(szFileName,MAX_PATH,"%s/%s_%s.dbf",
             m_szFilePath.c_str(),szDate,m_szTaskName.c_str());
    return STRING(szFileName);
}
int CSaveProxy::CreatePerfFile(STRING &szFileName)
{
    //by ldf 2003-12
    BOOL bRetOpen = m_pDdaSet->Open(szFileName.c_str());
    m_pDdaSet->m_strSetName = m_szFileName;

    if(!bRetOpen)
    {   
        //否则就要生成合适的文件头
        int nRes = m_pDdaSet->Open(szFileName.c_str(),"wb+");
        if(!nRes)
        {
            m_szError = strerror(errno);
            return FALSE;
        }
        m_pDdaSet->Clear();
        DB_FIELD *pField = new DB_FIELD;
        pField->Name = "STIME";
        pField->Type = dbChar;
        pField->Len = SETIME_LEN;
        pField->Dot = 0;
        m_pDdaSet->AddField(pField);
        pField = new DB_FIELD;
        pField->Name = "ETIME";
        pField->Type = dbChar;
        pField->Len = SETIME_LEN;
        pField->Dot = 0;
        m_pDdaSet->AddField(pField);
        char szTmp[MAX_PATH];
        for(int i = 1; i <= m_pCounterMap->size(); i++)
        {
            pField = new DB_FIELD;
            SNPRINTF(szTmp,MAX_PATH,"COUNTER%d",i);
            pField->Name = szTmp;
            pField->Type = dbNumber;
            pField->Len = COUNTER_LEN;
            pField->Dot = 2;
            m_pDdaSet->AddField(pField);
        }
        if(!m_pDdaSet->SyncToFile())
        {
            m_szError = strerror(errno);
            nRes = FALSE;
        }
        else
            nRes = TRUE;
        
        m_pDdaSet->Close(0);
        return nRes;
    }
    else
    {
        //如果成功，证明文件存在，关闭后返回true;
        m_pDdaSet->Close(0);
        return TRUE;
    }
}	
//////////////////////////////////////////////////////////////////////
// QueryProxy Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQueryProxy::CQueryProxy()
{
    m_szError = "No";
}

CQueryProxy::~CQueryProxy()
{

}
const char *StrFormat(int nLen, const char *pszEnd)
{
    static char format_string[16];
    SNPRINTF(&format_string[1],16,"-%d%s",nLen,pszEnd);
    format_string[0] = '%';
    return format_string;
}
