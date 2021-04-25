/**************************************************************************
 *产品名    : iGateway Bill V200        
 *模块名    : 日志模块(Log)
 *文件名    : log.cpp
 *描述信息  : 本文件是类CLog的实现
 *版本说明  : V200R001i00M00B00D1130A   
 *版权信息  : 版权所有（C）2001-2002 华为技术有限公司
 *作者      : 方卫华
 *创建时间  : 2001-10-30
 *修改记录  : 
 **************************************************************************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/ 
#include "log.h"
#include "../utility/mml_report.h"
#include "../include/system1.h"
#include "../include/resource.h"
#include "../toolbase/ftp_client.h"
#define DIR_CHAR    '/'

/**************************************************************************
函数原型    public: CLog (void);
功能描述    构造函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
CLog::CLog()
{
    m_tLastDay      = 0;
    m_uCurrentSN    = 0;
    m_uSavedDays    = 0;
    m_szBasePath    = "";
    m_szXmlLogName  = "";
    m_pReport       = new CMMLReport("LOG");
    m_nPageLimit    = 48*1024; //48K
    m_bFirst        = true;
    m_bSameFile     = false;
    m_nQueryCount   = 0;
    m_nNetStatus    = 0;
    m_pLogUtil      = NULL;
    m_bUseXml       = TRUE;
    m_pQueryLogUtil = NULL;
}

/**************************************************************************
函数原型    public: ~CLog ();
功能描述    构造函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
CLog::~CLog()
{
    if(m_pReport != NULL)
    {
        delete m_pReport;
        m_pReport = NULL;
    }

    if(m_pLogUtil != NULL)
    {
        delete m_pLogUtil;
        m_pLogUtil = NULL;
    }

    if(m_pQueryLogUtil != NULL)
    {
        delete m_pQueryLogUtil;
        m_pQueryLogUtil = NULL;
    }
}
    
/**************************************************************************
函数原型    protected: void ExitInstance (void);
功能描述    退出清理
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CLog::ExitInstance (void)
{
     
}

    
/**************************************************************************
函数原型    protected: int InitInstance (void);
功能描述    初始化
输入参数    无
输出参数    无
返回值      初始化成功时返回ERR_SUCCESS
抛出异常    无
***************************************************************************/
int CLog::InitInstance (void)
{ 
    int nRet = ERR_FAIL;
    nRet = CWorkModule::InitInstance();
    if( nRet != ERR_SUCCESS )
    {
        return nRet;
    }
       
    // 初始化成员变量m_tLastDay为当前时间
    time(&m_tLastDay);
    
    // 打开配置文件
    CINIFile ConfigFile(GetCfgFilePath());
    nRet = ConfigFile.Open();
    if ( nRet != TRUE)
    {
        TRACE(THISMODULE,S_LOG_OPENINIFILEFAIL);
        return ERR_FAIL;
    }
    else
    {
        // 从配置库里获取日志保留天数
        m_uSavedDays = ConfigFile.GetInt(CFG_SEC_LOG, 
                                         CFG_LOG_SAVE_DAY, 
                                         CFG_LOG_SAVE_DAY_DEFAULT);
    
        // 获取日志文件存放的根路径
        char temp[MAX_PATH];
        
        ConfigFile.GetString(CFG_SEC_LOG, 
                             CFG_LOG_ROOT_DIR, 
                             CFG_LOG_ROOT_DIR_DEFAULT, 
                             temp, 
                             MAX_PATH);
        m_szBasePath = temp;
        
        ConfigFile.GetString(CFG_SEC_LOG, CFG_LOG_ARCHIVE_DIR, 
                                CFG_LOG_ARCHIVE_DIR_DEFAULT, 
                                temp, 
                                MAX_PATH);
        
		m_szArchDir = temp;
        CreateDeepDir(m_szArchDir.c_str());
		char szArchiveTime[12];

		ConfigFile.GetString(CFG_SEC_LOG,
                             CFG_LOG_ARCTIME,
                             CFG_LOG_ARCTIME_DEFAULT,
                             szArchiveTime,11);
		InitArchiveTime(szArchiveTime);

        // 从配置库里获取日志保留天数
        m_bUseXml = ConfigFile.GetInt(CFG_SEC_LOG, 
                                      CFG_USE_XML, 
                                      CFG_USE_XML_DEFAULT);
        // 关闭配置文件
        ConfigFile.Close();
    }
   
    m_pLogUtil = CreateLogUtilObj();
    if(NULL == m_pLogUtil)
    {
        return FALSE;
    }

    //张玉新 2002-04-25  问题单D13709
    // 打开当天的日志文件
    nRet = OpenWriteFile(); 
    if ( nRet != TRUE)
    {
        TRACE(THISMODULE,S_LOG_OPENLOGFILEFAIL);
        return ERR_FAIL;
    }

    // 初始化当前日志序列号
    m_uCurrentSN = GetCurrentSN();
	//结束

	m_lTimerID5Sec = CWorkModule::SetTimer(FIVE_SECOND_IN_MILISEC);
    m_lTimerID1Day = CWorkModule::SetTimer(6 * 3600 * 1000);
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
MODULE_TYPE CLog::GetModuleType ( void )
{
    return MT_LOG;
}

/**************************************************************************
函数原型    protected: BOOL OpenWriteFile ( void );
功能描述    打开日志文件
输入参数    无
输出参数    无
返回值      日志文件打开成功时返回TRUE，否则返回FALSE
抛出异常    无
***************************************************************************/
BOOL CLog::OpenWriteFile ( void )
{
    // 获取当前时间，取其年月日和日志文件的根路径组成日志文件全路径名
    // 日志文件名格式为yyyymmdd.xml
    time_t curtime = ACE_OS::time(NULL);
    char filename[9];
    ACE_OS::strftime(filename, 9, "%Y%m%d", ACE_OS::localtime(&curtime));
    filename[8] = '\0';

    STRING szLogFile = MakePath(m_szBasePath, filename);
    szLogFile += GetLogFileExt();

	//如果日志保存目录不存在，则生成此目录
	if (ACE_OS::access(m_szBasePath.c_str(),0) != 0)
	{
		if (!CreateDeepDir(m_szBasePath.c_str()))
		{
			return FALSE;
		}
	}

	// 设置m_tLastDay为当前时间
    m_tLastDay = curtime;

	//初始化xml文件，并建立相应的DOM结点树
	int nRet = m_pLogUtil->OpenFile(szLogFile.c_str());

    return nRet == 0;
}

/**************************************************************************
函数原型    protected: void CloseWriteFile ( void );
功能描述    关闭日志文件
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CLog::CloseWriteFile ( void )
{
	m_pLogUtil->CloseFile();
}


/**************************************************************************
函数原型    protected: BOOL WriteLog ( SLog *pBuf );
功能描述    保存日志
输入参数    日志数据
输出参数    无
返回值      保存日志成功返回TRUE，否则返回FALSE
抛出异常    无
***************************************************************************/
BOOL CLog::WriteLog ( SLog *pBuf )
{
    if(pBuf == NULL)
    {
        TRACE(THISMODULE, S_LOG_LOGBUF_NULL);
        return FALSE;
    }

    // 获取当前时间
    time_t curtime;
    time(&curtime);
    
    // 当前日期与上次写日志的日期不同则打开新的日志文件
    int CurDay = localtime(&curtime)->tm_mday;
    int LastDay = localtime(&m_tLastDay)->tm_mday;
	//张玉新 2002-06-04 问题单D14236
    if (CurDay != LastDay)
    {        
        // 打开新的日志文件
		if(OpenWriteFile() != TRUE)
        {
            return FALSE;
        }
    }
    
    pBuf->tLogTime  = curtime;
    pBuf->uSerialNo = ++m_uCurrentSN;

    // 将本条日志数据写入当前日志文件中
	int nRet = m_pLogUtil->AppendLog(pBuf->uSerialNo,
                                     pBuf->LogType,
                                     pBuf->szUserName,
                                     pBuf->uIPAddr,
                                     pBuf->tLogTime,
                                     pBuf->OperResult,
                                     pBuf->szOperCmd,
                                     pBuf->szCmdDesc);

    if(ERR_SUCCESS != nRet)
    {
        TRACE(THISMODULE, S_LOG_WRITE_LOG_FAIL);
        CloseWriteFile();
        return FALSE;
    }

    return TRUE;
}

/**************************************************************************
函数原型    protected: UINT4 GetCurrentSN ( void );
功能描述    获取当前文件序列号
输入参数    无
输出参数    无
返回值      返回当前文件序列号
抛出异常    无
***************************************************************************/

UINT4 CLog::GetCurrentSN ( void )
{
    UINT4 CurSN = 0;
    
    // 获取当前时间，取其年月日和日志文件的根路径组成日志文件全路径名
    time_t curtime;
    tm *ptm;
    time(&curtime);
    ptm = localtime(&curtime);

    // 日志文件名格式为yyyymmdd.xml
    char filename[9];
    sprintf(filename,"%04d%02d%02d", ptm->tm_year + 1900, ptm->tm_mon + 1,ptm->tm_mday);
    filename[8] = '\0';
    //当前日志文件
    STRING szCurLogFile = MakePath(m_szBasePath, filename);
    szCurLogFile += GetLogFileExt();
    
    // 从当前日期的日志文件里获取日志序列号
    if (GetSNFromFile(szCurLogFile.c_str(), CurSN))
    {
        return CurSN;
    }

    // 获取日志根目录下日志文件名列表
    LIST<STRING> LogFileList;
    if (GetLogFileList(LogFileList) != ERR_SUCCESS) 
    {
        return 0;
    }
    
    // 按照日志文件名称升序排列
    LogFileList.sort();
    
    STRING szLogFile;

    LIST<STRING>::reverse_iterator riter = LogFileList.rbegin();
    while (riter != LogFileList.rend())
    {
        szLogFile = MakePath(m_szBasePath, (*riter).c_str());       
        //前面已对当前日志文件进行搜索且失败，这里不再对其进行搜索
        if (szLogFile != szCurLogFile)
        {
            if (GetSNFromFile(szLogFile.c_str(), CurSN))
            {
                return CurSN;
            }
        }
        riter++;
    } 
    
    return 0;
}

/**************************************************************************
函数原型    protected: BOOL GetSNFromFile ( const char * szPathFile, 
                                            UINT4& uSN );
功能描述    从一个日志文件里获取日志序列号
输入参数    日志文件路径
输出参数    日志序列号
返回值      成功则返回TRUE，否则返回FALSE
抛出异常    无
***************************************************************************/
BOOL CLog::GetSNFromFile ( const char * szPathFile, UINT4& uSN )
{
    if (szPathFile == 0 || szPathFile[0] == '\0')
    {
        return FALSE;
    }

    CLogUtilEncap logUtilEncap;
    ILogUtil* pLogUtil = NULL;
    if(m_bUseXml)
    {
        pLogUtil = CreateLogUtilObj();
    }
    else
    {
        pLogUtil = new CLogBinUtil;
    }
    if(NULL == pLogUtil)
    {
        return FALSE;
    }

    //先读取指定日志文件，并建立DOM结点树
	if (pLogUtil->OpenFile(szPathFile) != 0)
	{
        delete pLogUtil;
		return FALSE;
	}
	
    SLog log;
	if (pLogUtil->ReadLog(EL_FINAL,
                          log.uSerialNo,
                          log.LogType,
                          log.szUserName,
                          sizeof(log.szUserName),
                          log.uIPAddr,
                          log.tLogTime,
                          log.OperResult,
                          log.szOperCmd,
                          sizeof(log.szOperCmd),
                          log.szCmdDesc,
                          sizeof(log.szCmdDesc)) != 0)
	{
        delete pLogUtil;
		return FALSE;
	}
	

	uSN = log.uSerialNo;
    delete pLogUtil;

	return TRUE;
}

/**************************************************************************
函数原型    private: int GetLogFileList ( LIST<STRING>& LogFileList );
功能描述    获取日志根目录下所有日志文件
输入参数    无
输出参数    日志文件列表
返回值      成功则返回ERR_SUCCESS，否则ERR_FAIL
抛出异常    无
***************************************************************************/
int CLog::GetLogFileList ( LIST<STRING>& LogFileList )
{
    // 查找日志目录下的所有的日志文件名称
    ACE_DIR *pBaseDIR = ACE_OS_Dirent::opendir(m_szBasePath.c_str());
    if ( ! pBaseDIR )
    {
        m_strError = ACE_OS::strerror(ACE_OS::last_error());
        TRACE(THISMODULE, S_LOG_OPEN_DIR_FAIL, m_szBasePath.c_str());
        MSGOUT(MTS_LOG,MSGOUT_LEVEL_IMPORTANT,
               S_LOG_OPEN_DIR_FAIL,
               m_szBasePath.c_str());
        return ERR_FAIL;
    }
    int nRet = 0;
    dirent *pDirent;
    
    while ( (pDirent = ACE_OS_Dirent::readdir(pBaseDIR)) != NULL )
    {
        if (ACE_OS::strcmp(pDirent->d_name, ".") == 0 
            || ACE_OS::strcmp(pDirent->d_name, "..") == 0)
        {
            continue;   
        }
        
        // 忽略所有非".xml"文件和目录
        char *ptr;
        ptr = pDirent->d_name;
        ptr += ACE_OS::strlen(pDirent->d_name) - 4;
        if ( ACE_OS::strncmp(ptr, GetLogFileExt(), 4) != 0 )
        {
            continue;
        }
            
        LogFileList.push_back(pDirent->d_name);
    }
    
    ACE_OS_Dirent::closedir(pBaseDIR);
    nRet = LogFileList.size();
    if(nRet == 0)
    {
        m_strError = S_LOG_FILE_NOEXIST;
        TRACE(THISMODULE, S_LOG_FILE_NOEXIST, m_szBasePath.c_str());
        MSGOUT(MTS_LOG,MSGOUT_LEVEL_IMPORTANT,
                S_LOG_FILE_NOEXIST,m_szBasePath.c_str());
        return ERR_FAIL; 
    }
    else
    {
        return ERR_SUCCESS;
    }
}

/**************************************************************************
函数原型    protected: void ParseMsg ( MSG_INTRA* pMsgParsed )
功能描述    命令解析与分发
输入参数    消息指针
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CLog::ParseMsg ( MSG_INTRA* pMsgParsed )
{
    switch ( pMsgParsed->CmdCode )
    {
        case CMD_WRITE_LOG:                     //写日志命令
        {
            // 从消息帧的Data域里取出需要保存的日志数据
            SLog *pData = (SLog *) &(pMsgParsed->Data[0]);
            // 保存该包日志数据
            WriteLog(pData);
            
            break;
        }    
        case CMD_MML:
        {
            // 从消息帧的Data域里取出CParseResult对象
            CParseResult *pParse = *(CParseResult **)pMsgParsed->Data;
            UINT2 MMLCmdCode = pParse->GetCmdCode();
            if ( MML_LST_LOG == MMLCmdCode)     //日志查询命令
            {
                QueryLog(pMsgParsed);           // 实施日志查询处理
            }
			else if (MML_SET_LOG_SDS == MMLCmdCode)
			{
				SetLogSavingDays(pMsgParsed);
			}
			else if (MML_RMV_LOG == MMLCmdCode)
			{
				RemoveLogByClient(pMsgParsed);
			}
			else if (MML_ARC_LOG == MMLCmdCode)
			{
				ArchiveLog(pMsgParsed);
			}
			else if(MML_SET_LOG_ARCTIME == MMLCmdCode)
			{
				SetLogArchiveTime(pMsgParsed);
			}
            else if(MML_GET_LOG_SDS == MMLCmdCode)
            {
                GetLogSavingDays(pMsgParsed);
            }
            else if(MML_GET_LOG_ARCTIME == MMLCmdCode)
            {
                GetLogArchiveTime(pMsgParsed);
            }
            else if (MML_EXP_LOG == MMLCmdCode)
            {
                ExportLog(pMsgParsed);
            }
            else if (MML_ULD_LOG == MMLCmdCode)
            {
                UploadLog(pMsgParsed);
            }
            break;
        }    
        default:
			//张玉新 2002-04-25 问题单D12535
            //TRACE(THISMODULE, S_LOG_RECV_OTHER_MSG, pMsgParsed->CmdCode);
            
            break;
    }
}

void CLog::GetLogSavingDays(MSG_INTRA* pMsg)
{
    // 从消息帧的Data域里取出CParseResult对象
    CParseResult *pParse = *(CParseResult **)pMsg->Data;
    char szTmp[64];
    
    //构造MML报文，响应MML客户端
    CMMLReport Report(THISMODULE);
    
    Report.MakeReportHead();            //构造报文头
    //构造命令回显
    Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, ERR_SUCCESS, THISMODULE);
    //构造返回报文正文
    SNPRINTF(szTmp,64,"%s  =  %d","days",m_uSavedDays);
    szTmp[63] = 0;
    Report.AddString(szTmp);
    //增加新行
    Report.AddString(STR_NEWLINE);
    //构造报文尾 
    Report.MakeReportTail();
    
    SendMsgToMML(Report.GetReport(),pMsg);
}
/**************************************************************************
函数原型    protected: void QueryLog ( MSG_INTRA* pMsg )
功能描述    分析查询要求，实施查询操作
输入参数    消息指针
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CLog::QueryLog ( MSG_INTRA* pMsg )
{
    // 存放返回给MML客户端的报文信息
    m_nNetStatus = 0;
    LIST <STRING> retMML;
    m_nQueryCount = 0;
    int nQueryResult = LOG_REP_MORE;
    //张玉新 2003-07-16 SWPD00397
    SLog log;
    char szTmpUser[USER_NAME_LEN];
    strncpy(szTmpUser,pMsg->UserName,USER_NAME_LEN);
    szTmpUser[USER_NAME_LEN-1] = 0;
    StringLRTrim(szTmpUser);
    if(strcmp(szTmpUser,"") == 0)
    {
        strcpy(szTmpUser,"-");
    }
    int nUserLen = strlen(szTmpUser);
    for(int i = 0; i<nUserLen; i++)
    {
        if(szTmpUser[i] == ' ')
            szTmpUser[i] = '-';
    }
    strncpy(log.szUserName,szTmpUser,LOG_USR_NAME_LEN);
    log.szUserName[LOG_USR_NAME_LEN-1] = 0;
    log.LogType = SLog::LOG_OPER;
    log.uIPAddr = pMsg->IPAddr;
    
    // 从消息帧的Data域里取出CParseResult对象
    CParseResult *pParse = *(CParseResult **)pMsg->Data;
    
    strncpy(log.szOperCmd,pParse->GetCmdName(),20);
    log.szOperCmd[LOG_OPER_CMD_LEN-1] = 0;
    strncpy(log.szCmdDesc,pParse->GetCmdDspName(),128);
    log.szCmdDesc[LOG_CMD_DESC_LEN-1] = 0;
    //修改结束
    // 从CParseResult对象中提取出MML命令参数，
    const char *szOperUser = pMsg->UserName;        // MML查询操作用户
    int LogType = 0;
    BOOL bRet = pParse->GetDigitVal(&LogType, 2);   //日志类型
    const char *szLogUser = pParse->GetStringVal(1);// 被查询用户

    tm QStartDate;
    tm QDefault;
    memset(&QDefault,0,sizeof(tm));
    GetDateValue(pParse,3,&QStartDate,&QDefault);   // 查询起始日期
    GetTimeValue(pParse,5,&QStartDate,&QDefault);   // 查询起始时间
    tm QEndDate;
    GetDateValue(pParse,4,&QEndDate,&QDefault);     // 查询结束日期
    GetTimeValue(pParse,6,&QEndDate,&QDefault);     // 查询终止时间

    // 由于去参数还不确定所以暂时先不写
    // 分别把查询的起始日期与查询起始时间、
    // 查询结束日期与查询结束时间组织成tm结构，
    // 分别统称为查询起始时间和查询结束时间
 
    time_t st,et;
    st = mktime(&QStartDate);
    et = mktime(&QEndDate);
    
    if ( difftime(et,st) < 0)
    {
        // 返回给MML的报文信息为“查询的起始时间不能大于结束时间”
        SendErrorReport(S_LOG_TIME_NOT_MATCH,pMsg);
        log.OperResult = SLog::OPER_FAIL;
        WriteLog(&log);
    }
    else if(0 ==  ACE_OS::strcasecmp(szOperUser, ADMIN_USER))  // 操作用户为Admin
    {
        while(nQueryResult == LOG_REP_MORE && m_nNetStatus == 0)
        {
            if ( SLog::LOG_OPER == LogType )        // 日志类型为操作日志
            {
                nQueryResult = QueryOperLog(&QStartDate, &QEndDate, szLogUser, retMML);
            }
            else if ( SLog::LOG_SYS == LogType )    // 日志类型为系统日志
            {
                nQueryResult = QuerySysLog(&QStartDate, &QEndDate, retMML);
            }
            AssembleReport(nQueryResult,retMML,pMsg);
        }
        log.OperResult = SLog::OPER_SUCCESS;
        WriteLog(&log);
    }
    else      //操作用户是普通用户
    {
        if ( SLog::LOG_OPER == LogType 
             && ACE_OS::strcasecmp(szLogUser, szOperUser) == 0 )
        {
            while(nQueryResult == LOG_REP_MORE && m_nNetStatus == 0)
            {
                nQueryResult = QueryOperLog(&QStartDate, &QEndDate, szLogUser, retMML);
                AssembleReport(nQueryResult,retMML,pMsg);
            }
            log.OperResult = SLog::OPER_SUCCESS;
            WriteLog(&log);
        }
        else if ( SLog::LOG_OPER != LogType )
        {
            // 返回给MML的报文信息为“被查询的日志类型非法”
            SendErrorReport(S_LOG_TYPE_INVALID,pMsg);
            log.OperResult = SLog::OPER_FAIL;
            WriteLog(&log);
        }
        else  //被查询的用户名不等于操作员用户名
        {
            // 返回给MML的报文信息为“被查询的用户名非法”
            SendErrorReport(S_LOG_QUERY_INVALID_USER,pMsg);
            log.OperResult = SLog::OPER_FAIL;
            WriteLog(&log);
        }
    }
    delete pParse;
    return;
}

void CLog::SendErrorReport(const char *szErrInfo,MSG_INTRA *pMsg,int nErrno)
{
    CParseResult *pParse = *(CParseResult **)pMsg->Data;
    m_pReport->MakeReportHead();
    m_pReport->MakeCmdDisplay(pParse->GetCmdLine(),1,nErrno,"LOG");
    m_pReport->AddString(S_LOG_TABLE,18,ALIGN_LEFT);
    m_pReport->AddString("PAGE 1");
    m_pReport->AddString(STR_NEWLINE);
    m_pReport->AddString(S_LOG_TITLE_SPIN);
    m_pReport->AddString(STR_NEWLINE);
    m_pReport->AddString("   ");
    m_pReport->AddString(S_LOG_REASON);
    m_pReport->AddString(STR_NEWLINE);
    m_pReport->AddString(STR_NEWLINE);
    m_pReport->AddString("   ");
    m_pReport->AddString(szErrInfo);
    m_pReport->AddString(STR_NEWLINE);
    m_pReport->AddString(STR_NEWLINE);
    m_pReport->MakeReportTail();
    SendMsgToMML(m_pReport->GetReport(),pMsg);
}

void CLog::AssembleReport(int nMode,LIST<STRING> &retMML,MSG_INTRA *pMsg)
{
    CParseResult *pParse = *(CParseResult **)pMsg->Data;
    // 组装MML报文并发送给MML客户端
    if(m_nQueryCount == 0) //如果没有查询到对应项，则发失败报文
    {
        SendErrorReport(S_LOG_QUERY_EMPTY,pMsg,400);
        return;
    }
    else if(nMode == LOG_REP_ERR)
    {
        SendErrorReport(m_strError.c_str(),pMsg);
        return;
    }
    m_pReport->MakeReportHead();
    m_pReport->MakeCmdDisplay(pParse->GetCmdLine(),1,0,"LOG");
    MakeTitle();
    for(; !retMML.empty(); retMML.pop_front())    
    {
        m_pReport->AddString(retMML.front().c_str());
        m_pReport->AddString(STR_NEWLINE);
    }   
    if(nMode == LOG_REP_MORE)
    {
        m_pReport->AddString("TO BE CONTINUED");
        m_pReport->AddString(STR_NEWLINE);
    }
    m_pReport->MakeReportTail();
    SendMsgToMML(m_pReport->GetReport(),pMsg);
    //ACE_Time_Value tv;
    //tv.msec(000);
    //ACE_OS::sleep(tv);
}

/**************************************************************************
函数原型    protected: UINT4 QuerySysLog ( ACE_Time_Value* pStartTime, 
                                           ACE_Time_Value* pEndTime, 
                                           LIST<STRING>& qResult );
功能描述    系统日志查询处理
输入参数    日志查询起止时间段
输出参数    MML报文列表
返回值      LOG_REP_MORE：表示还有报文
            LOG_REP_NONE：表示最后一页报文
            LOG_REP_ERR：表示出错
抛出异常    无
***************************************************************************/
UINT4 CLog::QuerySysLog ( tm *pStartTime, 
                          tm *pEndTime, 
                          LIST <STRING> &qResult)
{    
    return QueryLog_impl(pStartTime,pEndTime,qResult);
}
/**************************************************************************
函数原型    protected: UINT4 QueryOperLog ( ACE_Time_Value* pStartTime,
                                            ACE_Time_Value* pEndTime, 
                                            char *pUserName,
                                            LIST<STRING>& qResult );
功能描述    操作日志查询处理
输入参数    日志查询起止时间段，被查询的用户名
输出参数    MML报文列表
返回值      LOG_REP_MORE：表示还有报文
            LOG_REP_NONE：表示最后一页报文
            LOG_REP_ERR：表示出错
抛出异常    无
***************************************************************************/
UINT4 CLog::QueryOperLog ( tm *pStartTime, 
                           tm *pEndTime, 
                           const char *pUserName, 
                           LIST <STRING>& qResult)
{
    return QueryLog_impl(pStartTime,pEndTime,qResult,pUserName);
}

UINT4 CLog::QueryLog_impl(tm *pStartTime,
                          tm *pEndTime,
                          LIST <STRING>& qResult,
                          const char *pUserName)
{    
    // 报文大小计数器
    long LenOfMML = 0;
    // 存放一个MML报文的缓冲区
    char buf[MAX_LOG_LEN];
    buf[0] = '\0';
    time_t tStartTime = mktime(pStartTime);
    time_t tEndTime = mktime(pEndTime);
    
    qResult.clear();//ensure result list is empty
    if(m_bFirst)
    {              
        // 起始日志文件名
        char filename[9];
        sprintf(filename, "%04d%02d%02d", pStartTime->tm_year+1900,
            pStartTime->tm_mon+1, pStartTime->tm_mday);
        filename[8] = '\0';
        STRING StartLogFile(filename);
        StartLogFile += GetLogFileExt();
        
        // 结束日志文件名
        sprintf(filename, "%04d%02d%02d", pEndTime->tm_year+1900,
            pEndTime->tm_mon+1,pEndTime->tm_mday);
        filename[8] = '\0';
        STRING EndLogFile(filename);
        EndLogFile += GetLogFileExt();
        
        if (GetLogFileList(m_LogFileList) != ERR_SUCCESS)
        {
            return LOG_REP_ERR;
        }
        
        LIST<STRING>::iterator iter = m_LogFileList.begin();    
        while(iter != m_LogFileList.end())
        {
            // 仅查询介于开始文件名称和结束文件名称之间的日志文件
            if ( (*iter < StartLogFile) || (*iter > EndLogFile) )
            {
                iter = m_LogFileList.erase(iter);
                continue;
            }
            iter++;
        }
        
        // 按照日志文件名称排序
        m_LogFileList.sort();
        m_bFirst = false;
    }

    STRING szLogFile;
    SLog log;
    for (; !m_LogFileList.empty(); m_LogFileList.pop_front())
    {
        if(m_pQueryLogUtil == NULL)
        {
            m_pQueryLogUtil = CreateLogUtilObj();
            if(NULL == m_pQueryLogUtil)
            {
                m_bFirst = false;
                m_LogFileList.clear();
                return FALSE;
            }

            //日志文件名
            szLogFile = MakePath(m_szBasePath, m_LogFileList.front().c_str());

            //先读取指定日志文件，并建立DOM结点树
	        if (m_pQueryLogUtil->OpenFile(szLogFile.c_str()) != 0)
	        {
                TRACE(THISMODULE, S_LOG_OPEN_LOG_FAIL, szLogFile.c_str());
                delete m_pQueryLogUtil;
                m_pQueryLogUtil = NULL;
		        continue;
	        }
        }
	    
		//遍历日志记录，查找符合要求的记录
        int nRet;
        if (!m_bSameFile)
            nRet = m_pQueryLogUtil->ReadLog(EL_FIRST,
                                            log.uSerialNo,
                                            log.LogType,
                                            log.szUserName,
                                            sizeof(log.szUserName),
                                            log.uIPAddr,
                                            log.tLogTime,
                                            log.OperResult,
                                            log.szOperCmd,
                                            sizeof(log.szOperCmd),
                                            log.szCmdDesc,
                                            sizeof(log.szCmdDesc));
        else
            nRet = m_pQueryLogUtil->ReadLog(EL_NEXT,
                                            log.uSerialNo,
                                            log.LogType,
                                            log.szUserName,
                                            sizeof(log.szUserName),
                                            log.uIPAddr,
                                            log.tLogTime,
                                            log.OperResult,
                                            log.szOperCmd,
                                            sizeof(log.szOperCmd),
                                            log.szCmdDesc,
                                            sizeof(log.szCmdDesc));
                                            
        while(nRet == 0)                                         
        {
            if(pUserName == NULL)
            {
                if(SLog::LOG_SYS != log.LogType )   //该条日志不为系统日志
                {
                    nRet = m_pQueryLogUtil->ReadLog(EL_NEXT,
                                                    log.uSerialNo,
                                                    log.LogType,
                                                    log.szUserName,
                                                    sizeof(log.szUserName),
                                                    log.uIPAddr,
                                                    log.tLogTime,
                                                    log.OperResult,
                                                    log.szOperCmd,
                                                    sizeof(log.szOperCmd),
                                                    log.szCmdDesc,
                                                    sizeof(log.szCmdDesc));
                    continue;
                }
            }
            else
            {
                if(SLog::LOG_OPER != log.LogType )  //日志不为操作日志
                {
                    nRet = m_pQueryLogUtil->ReadLog(EL_NEXT,
                                                    log.uSerialNo,
                                                    log.LogType,
                                                    log.szUserName,
                                                    sizeof(log.szUserName),
                                                    log.uIPAddr,
                                                    log.tLogTime,
                                                    log.OperResult,
                                                    log.szOperCmd,
                                                    sizeof(log.szOperCmd),
                                                    log.szCmdDesc,
                                                    sizeof(log.szCmdDesc));
                    continue;
                }
                // 被查询的用户名不为空
                if((pUserName != NULL && pUserName[0] != '\0')
                  &&(ACE_OS::strcasecmp(pUserName, log.szUserName) != 0)) 
                {
                    nRet = m_pQueryLogUtil->ReadLog(EL_NEXT,
                                                    log.uSerialNo,
                                                    log.LogType,
                                                    log.szUserName,
                                                    sizeof(log.szUserName),
                                                    log.uIPAddr,
                                                    log.tLogTime,
                                                    log.OperResult,
                                                    log.szOperCmd,
                                                    sizeof(log.szOperCmd),
                                                    log.szCmdDesc,
                                                    sizeof(log.szCmdDesc));
                    continue;
                }
            }

            if ( difftime(tStartTime,log.tLogTime) > 0
                    || difftime(log.tLogTime,tEndTime) > 0)
            {
                nRet = m_pQueryLogUtil->ReadLog(EL_NEXT,
                                                log.uSerialNo,
                                                log.LogType,
                                                log.szUserName,
                                                sizeof(log.szUserName),
                                                log.uIPAddr,
                                                log.tLogTime,
                                                log.OperResult,
                                                log.szOperCmd,
                                                sizeof(log.szOperCmd),
                                                log.szCmdDesc,
                                                sizeof(log.szCmdDesc));
                continue;
            }
                     
            LenOfMML += PrintLog(buf, &log);
            qResult.push_back(buf);
            m_nQueryCount ++;
            if(LenOfMML >= m_nPageLimit)
            {
                if(m_LogFileList.size() == 1 && m_pQueryLogUtil->GetRemainCount() == 0)
                {
                    m_LogFileList.clear();
                    m_bFirst = true;
                    m_bSameFile = false;
                    m_pQueryLogUtil->CloseFile();
                    delete m_pQueryLogUtil;
                    m_pQueryLogUtil = NULL;
                    return LOG_REP_NONE;
                }
                else
                {
                    m_bSameFile = true;
                    return LOG_REP_MORE;
                }
            }

            nRet = m_pQueryLogUtil->ReadLog(EL_NEXT,
                                            log.uSerialNo,
                                            log.LogType,
                                            log.szUserName,
                                            sizeof(log.szUserName),
                                            log.uIPAddr,
                                            log.tLogTime,
                                            log.OperResult,
                                            log.szOperCmd,
                                            sizeof(log.szOperCmd),
                                            log.szCmdDesc,
                                            sizeof(log.szCmdDesc));
        }
        m_pQueryLogUtil->CloseFile();
        delete m_pQueryLogUtil;
        m_pQueryLogUtil = NULL;
    }

    if(NULL != m_pQueryLogUtil)
    {
        delete m_pQueryLogUtil;
        m_pQueryLogUtil = NULL;
    }
    m_LogFileList.clear();
    m_bFirst = true;
    m_bSameFile = false;
    return LOG_REP_NONE;
}

/**************************************************************************
函数原型    private: int PrintLog(char *buf, SLog *pLog);
功能描述    打印日志信息
输入参数    日志信息缓冲区和一条日志记录，日志输出格式符合MML报文格式，如下
-------------------------------------------------------------------------------------
序列号 日志类型 用户名 操作地点   日期       操作结果   操作命令    命令来源    
  1     操作    Admin   IP地址   2001-08-03      成功   LST ALMLOG     -      
-------------------------------------------------------------------------------------
序列号  用户名  日期         操作地点      日志来源      操作命令 命令详细描述   操作结果  日志类型 
1        Admin  2001-08-03    -               -          LST LOG   查询日志      成功      操作
输出参数   无
返回值     日志信息长度
抛出异常   无
***************************************************************************/
int CLog::PrintLog(char *buf, SLog *pLog)
{
    char temp[24];
    int len = 0;
    //序列号 
    len += SNPRINTF(buf + len, MAX_LOG_LEN - len, "  %-9d", pLog->uSerialNo);

    //用户名
    if (pLog->szUserName[0] == '\0' || SLog::LOG_SYS == pLog->LogType)
    {
        len += SNPRINTF(buf + len, MAX_LOG_LEN - len, "%-50.48s", "-");
    }
    else
    {
		StringToLower(pLog->szUserName);
        len += SNPRINTF(buf + len, MAX_LOG_LEN - len, "%-50.48s", pLog->szUserName);
    }

    //日期
    ACE_OS::strftime(temp, 24, "%Y-%m-%d %H:%M:%S", ACE_OS::localtime(&pLog->tLogTime));
    len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-24.22s",temp);

    //操作地点
    struct in_addr Addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
    Addr.s_addr = pLog->uIPAddr;
#else 
    Addr.S_un.S_addr = pLog->uIPAddr;
#endif//_PLATFORM_HPUX
    len += SNPRINTF(buf + len, MAX_LOG_LEN - len, "%-20.18s",  ACE_OS::inet_ntoa(Addr));

    
    //日志来源
    if (pLog->szUserName[0] == '\0' || SLog::LOG_OPER == pLog->LogType)
    {
        len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-50.48s", "-");
    }
    else
    {
        len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-50.48s", pLog->szUserName);
    }
	
    //操作命令
    if (pLog->szOperCmd[0] == '\0')
    {
        len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-22.20s","-");
    }
    else
    {
        len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-22.20s",pLog->szOperCmd);
    }
    
    //操作命令详细描述
    if (pLog->szCmdDesc[0] == '\0')
    {
        len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-256.254s","-");
    }
    else
    {
        len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-256.254s",pLog->szCmdDesc);
    }

    //操作结果
    switch ( pLog->OperResult )
    {
        case SLog::OPER_SUCCESS:
            len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-8.6s","SUC");
            break;
        case SLog::OPER_FAIL:
            len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-8.6s","FAIL");
            break;
        default:
            len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-8.6s","N/A");
    }   
         
    //日志类型
    switch ( pLog->LogType )
    {
        case SLog::LOG_SYS:
            len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-8.6s","SYS");
            break;
        case SLog::LOG_OPER:
            len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-8.6s","OPER");
            break;
        default:
            len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-8.6s", "N/A");
    }
    
    return len;
}

/**************************************************************************
函数原型    protected: BOOL DeleteOutdatedLog ( void );
功能描述    删除指定天数前的日志
输入参数    无
输出参数    无
返回值      成功则返回TRUE，否则FALSE
抛出异常    无
***************************************************************************/
BOOL CLog::DeleteOutdatedLog ( void )
{
    STRING szLogFile;
    int nRet;
    // 获取当前时间
    time_t curtime,tFileTime;
    time(&curtime);
    //张玉新 2002-04-25 问题单12476 删掉原来的代码    
    // 获取日志根目录下日志文件名列表
    LIST<STRING> LogFileList;
    nRet = GetLogFileList(LogFileList);
    if ( nRet != ERR_SUCCESS )
    {
        return FALSE;
    }
    int nPos = 0;
    int nYear = 0, nMonth = 0, nDay = 0, nTime = 0;
    tm tmFileTime,tmNow;
    memset(&tmFileTime,0,sizeof(tm));
    memset(&tmNow,0,sizeof(tm));
    char szCurTime[32];
    for (LIST<STRING>::iterator iter = LogFileList.begin(); 
         iter != LogFileList.end(); iter++)
    {
        szLogFile = MakePath(m_szBasePath, (*iter).c_str());
        nPos = (*iter).find(GetLogFileExt());

        if(nPos == std::string::npos)
        {
            //没有.log
            continue;
        }
        
        nPos -= 8;
        if(nPos < 0)
            nPos = 0;
        nTime = atoi((*iter).substr(nPos,8).c_str());
        nYear = nTime/10000;
        nMonth= (nTime%10000)/100;
        nDay  = (nTime%10000)%100;
        tmFileTime.tm_year = nYear - 1900;
        tmFileTime.tm_mon = nMonth - 1;
        tmFileTime.tm_mday = nDay;
        tFileTime = mktime(&tmFileTime);
        tmNow = *localtime(&curtime);
        strftime(szCurTime,32,"%Y-%m-%d %H:%M:%S",&tmNow);
        MSGOUT(MTS_LOG,MSGOUT_LEVEL_IMPORTANT,
               S_LOG_FIND_LOG_FILE,(*iter).c_str(),szCurTime);
        double dDiff = difftime(curtime,tFileTime);
        if (  dDiff > m_uSavedDays * 24 * 3600)
        {//修改结束
            // 删除该日志文件
            nRet = ACE_OS::unlink(szLogFile.c_str()); 
            if ( nRet != 0)
            {
                TRACE(THISMODULE, S_LOG_REMOVE_LOG_FAIL, szLogFile.c_str());
                MSGOUT(MTS_LOG,MSGOUT_LEVEL_IMPORTANT,
                       S_LOG_REMOVE_LOG_FAIL,
                       szLogFile.c_str());
                continue;
            }
            TRACE(THISMODULE, S_LOG_REMOVE_LOG_SUC, szLogFile.c_str());
            MSGOUT(THISMODULE,MSGOUT_LEVEL_IMPORTANT,
                   S_LOG_REMOVE_LOG_SUC,
                   szLogFile.c_str());
        }
    }
    
    return TRUE;
}

/**************************************************************************
函数原型    protected: int SendMsgToMML(char *pszMMLReport,MSG_INTRA *pFrame)
功能描述    发送MML报文到客户端
输入参数    无
输出参数    无
返回值      成功则返回0，否则非零
抛出异常    无
***************************************************************************/
int CLog::SendMsgToMML(const char *pszMMLReport, MSG_INTRA *pFrame)
{
    // 创建MSG_INTRA消息头
    int nLen =  strlen(pszMMLReport);
    MSG_INTRA *pRep = new (nLen + 1) MSG_INTRA;
    strcpy((char *)pRep->Data, pszMMLReport);
    
    pRep->SenderMid   = GetModuleType();
    pRep->ReceiverPid = PT_OM;           
    pRep->ReceiverMid = MT_MMLDEBUG;
    pRep->CmdCode     = CMD_MML;
    pRep->AppType     = pFrame->AppType;
    pRep->ClientNo    = pFrame->ClientNo;
    pRep->IPAddr      = pFrame->IPAddr;

    m_nNetStatus = CWorkModule::SendMsg(pRep);
    if(m_nNetStatus)
    {
        TRACE(MTS_LOG,"Send Error(%d)",m_nNetStatus);
    }
    return m_nNetStatus;
}

void CLog::MakeTitle()
{
    if(m_pReport == NULL) { return; }
    static int nPage = 1;
    char szTmp[10];
    sprintf(szTmp,"PAGE %d",nPage++);
    m_pReport->AddString(S_LOG_TABLE,18,ALIGN_LEFT);
    m_pReport->AddString(szTmp);
    m_pReport->AddString(STR_NEWLINE);
    m_pReport->AddString(S_LOG_TITLE_SPIN);
    m_pReport->AddString(STR_NEWLINE);
    m_pReport->AddString(S_LOG_SERIALNO,12,ALIGN_LEFT);
    m_pReport->AddString(S_LOG_USER_NAME,12,ALIGN_LEFT);
    m_pReport->AddString(S_LOG_DATE,14,ALIGN_LEFT);
    m_pReport->AddString(S_LOG_OPERATOR_IP,18,ALIGN_LEFT);
    m_pReport->AddString(S_LOG_CMD_SOURCE,16,ALIGN_LEFT);
    m_pReport->AddString(S_LOG_OPERATOR_CMD,14,ALIGN_LEFT);
    m_pReport->AddString(S_LOG_CMD_EXP,18,ALIGN_LEFT);
    m_pReport->AddString(S_LOG_OPERATOR_RESULT,12,ALIGN_LEFT);
    m_pReport->AddString(S_LOG_TYPE,12,ALIGN_LEFT);
    m_pReport->AddString(STR_NEWLINE);
    m_pReport->AddString(STR_NEWLINE);
    return;
}

int CLog::GetIntValue(CParseResult *pParaResult, int paraID, int nDefault)
{ 
    const char *pcTmp;
    pcTmp = pParaResult->GetStringVal(paraID);
    if( pcTmp != NULL)
    {
        return atoi(pcTmp);
    }
    else
    {
        return nDefault;
    }   
}

void CLog::GetDateValue(CParseResult *pParaResult, int paraID, tm *pDate, tm *pDefault)
{
    //获得年份参数
    memcpy(pDate,pDefault,sizeof(tm));
    const char *pcTmp;
    pcTmp = pParaResult->GetStringVal(paraID,1);
    if( pcTmp != NULL)
    {
        pDate->tm_year = atoi(pcTmp) - 1900;
    }
    else
    {
        pDate->tm_year = pDefault->tm_year;
    }
    //获得月份参数
    pcTmp = pParaResult->GetStringVal(paraID,2);
    if( pcTmp != NULL)
    {
        pDate->tm_mon = atoi(pcTmp) - 1;
    }
    else
    {
        pDate->tm_mon = pDefault->tm_mon;
    }
    //获得日期参数
    pcTmp = pParaResult->GetStringVal(paraID,3);
    if( pcTmp != NULL)
    {
        pDate->tm_mday = atoi(pcTmp);
    }
    else
    {
        pDate->tm_mday = pDefault->tm_mday;
    }
}

void CLog::GetTimeValue(CParseResult *pParaResult, int paraID, tm *pDate, tm *pDefault)
{
    //获得hour参数
    
    const char *pcTmp;
    pcTmp = pParaResult->GetStringVal(paraID,1);
    if( pcTmp != NULL)
    {
        pDate->tm_hour = atoi(pcTmp);
    }
    else
    {
        pDate->tm_hour = pDefault->tm_hour;
    }
    //获得min参数
    pcTmp = pParaResult->GetStringVal(paraID,2);
    if( pcTmp != NULL)
    {
        pDate->tm_min = atoi(pcTmp);
    }
    else
    {
        pDate->tm_min = pDefault->tm_min;
    }
    //获得sec参数
    pcTmp = pParaResult->GetStringVal(paraID,3);
    if( pcTmp != NULL)
    {
        pDate->tm_sec = atoi(pcTmp);
    }
    else
    {
        pDate->tm_sec = pDefault->tm_sec;
    }
}

STRING CLog::MakePath(const STRING& path, const char *filename)
{
    if(filename == NULL)
    {
        return STRING("");
    }
    char szTmp[MAX_PATH];  
    if( *((path).end() - 1) != DIR_CHAR )
    {
        sprintf(szTmp,"%s/%s",path.c_str(),filename);
    }
    else
    {
        sprintf(szTmp,"%s%s",path.c_str(),filename); 
    }
    return STRING(szTmp);
}

void CLog::SetLogSavingDays(MSG_INTRA* pMsg)
{
	// 从消息帧的Data域里取出CParseResult对象
    CParseResult *pParse = *(CParseResult **)pMsg->Data;
    
	int nSavingDays = 30;
    // 从CParseResult对象中提取出MML命令参数，
    BOOL bRet = pParse->GetDigitVal(&nSavingDays, 1);   //日志类型
    //省略了错误处理
    CINIFile ConfigFile(GetCfgFilePath());
    ConfigFile.Open();
	m_uSavedDays = nSavingDays;
    ConfigFile.WriteInt(CFG_SEC_LOG,
                        CFG_LOG_SAVE_DAY, 
                        m_uSavedDays);
    ConfigFile.Close();

	//写日志
	SLog log;

    log.LogType    = SLog::LOG_OPER;
    log.OperResult = SLog::OPER_SUCCESS;

    strncpy(log.szUserName, pMsg->UserName,LOG_USR_NAME_LEN);
    log.szUserName[LOG_USR_NAME_LEN-1] = 0;

    log.LogType = SLog::LOG_OPER;
    log.uIPAddr = pMsg->IPAddr;
    strncpy(log.szOperCmd,pParse->GetCmdName(), LOG_OPER_CMD_LEN);
    log.szOperCmd[LOG_OPER_CMD_LEN-1] = 0;

    strncpy(log.szCmdDesc, pParse->GetCmdDspName(), LOG_CMD_DESC_LEN);
    log.szCmdDesc[LOG_CMD_DESC_LEN-1] = 0;

	WriteLog(&log);

	//构造MML报文，响应MML客户端
    CMMLReport Report(THISMODULE);

    Report.MakeReportHead();            //构造报文头
    //构造命令回显
    Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, ERR_SUCCESS, THISMODULE);
    //构造返回报文正文
    Report.AddString(STR_OPRSUCCESS);
    //增加新行
    Report.AddString(STR_NEWLINE);
    //构造报文尾 
    Report.MakeReportTail();
               
    SendMsgToMML(Report.GetReport(),pMsg);
}

void CLog::RemoveLogByClient(MSG_INTRA* pMsg)
{
	// 从消息帧的Data域里取出CParseResult对象
    CParseResult *pParse = *(CParseResult **)pMsg->Data;

    tm sd;	//删除的起始日期
	tm ed;	//删除的终止日期
    tm dd;

    memset(&dd, 0, sizeof(tm));
    GetDateValue(pParse, 1, &sd, &dd); 
    GetDateValue(pParse, 2, &ed, &dd); 
 
    time_t st, et;
    st = mktime(&sd);
    et = mktime(&ed);
    
	int nRet = ERR_SUCCESS;

    if (difftime(et, st) < 0)
	{
		nRet = ERR_FAIL;
	}
	else 
	{
	   // 起始日志文件名
		char szFileName[9];
		sprintf(szFileName, "%04d%02d%02d", sd.tm_year+1900, sd.tm_mon+1, sd.tm_mday);
		szFileName[8] = '\0';
		STRING StartLogFile(szFileName);
		StartLogFile += GetLogFileExt();
    
		// 结束日志文件名
		sprintf(szFileName, "%04d%02d%02d", ed.tm_year+1900, ed.tm_mon+1, ed.tm_mday);
		szFileName[8] = '\0';
		STRING EndLogFile(szFileName);
		EndLogFile += GetLogFileExt();
        char szTempFilePath[MAX_PATH];
		// 获取日志根目录下日志文件名列表
		LIST<STRING> LogFileList;
		if (GetLogFileList(LogFileList) != ERR_FAIL)
		{       
			LIST<STRING>::iterator iter = LogFileList.begin();    
			while(iter != LogFileList.end())
			{
				// 仅查询介于开始文件名称和结束文件名称之间的日志文件
				if ((*iter >= StartLogFile) && (*iter <= EndLogFile))
				{
                    SNPRINTF(szTempFilePath,MAX_PATH,
                             "%s/%s",
                             m_szBasePath.c_str(),
                             (*iter).c_str());
		            ACE_OS::unlink(szTempFilePath); 
				}

				iter++;
			}
		}	
	}

	// 向MML客户端响应报文
	//写日志
	SLog log;

	log.LogType    = SLog::LOG_OPER;
	if (ERR_SUCCESS == nRet)
	{
		log.OperResult = SLog::OPER_SUCCESS;
	}
	else
	{
		log.OperResult = SLog::OPER_FAIL;
	}

    strncpy(log.szUserName, pMsg->UserName,LOG_USR_NAME_LEN);
    log.szUserName[LOG_USR_NAME_LEN-1] = 0;

    log.LogType = SLog::LOG_OPER;
    log.uIPAddr = pMsg->IPAddr;
    strncpy(log.szOperCmd,pParse->GetCmdName(), LOG_OPER_CMD_LEN);
    log.szOperCmd[LOG_OPER_CMD_LEN-1] = 0;

    strncpy(log.szCmdDesc, pParse->GetCmdDspName(), LOG_CMD_DESC_LEN);
    log.szCmdDesc[LOG_CMD_DESC_LEN-1] = 0;

	WriteLog(&log);

    CMMLReport Report(THISMODULE);

    Report.MakeReportHead();            //构造报文头

    //构造命令回显

	Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, nRet, THISMODULE);
	if (ERR_SUCCESS == nRet)
	{
		//构造返回报文正文
		Report.AddString(STR_OPRSUCCESS);
	}
	else
	{
		//构造返回报文正文
		Report.AddString(STR_OPRFAIL);
	}

    //增加新行
    Report.AddString(STR_NEWLINE);
    //构造报文尾 
    Report.MakeReportTail();
               
    SendMsgToMML(Report.GetReport(),pMsg);
}

void CLog::ArchiveLog(MSG_INTRA* pMsg)
{

    int nRet = ERR_FAIL;

#ifdef _PLATFORM_WIN32
    LIST<STRING> LogFileList;
    char szSrctFile[MAX_PATH] = "";
    char szDestFile[MAX_PATH] = "";
    
    nRet = GetLogFileList(LogFileList);
    LogFileList.sort();

    if(nRet != ERR_FAIL)
    { 
        //开始归档日志
        LIST<STRING>::iterator iter = LogFileList.begin();   
        while(iter != LogFileList.end())
        {
            SNPRINTF(szSrctFile,  MAX_PATH,
                     "%s/%s",
                     m_szBasePath.c_str(), 
                     (*iter).c_str());
            szSrctFile[sizeof(szSrctFile) - 1] = '\0';
            
            SNPRINTF(szDestFile,  MAX_PATH,
                     "%s/%s",
                     m_szArchDir.c_str(), 
                     (*iter).c_str());
            szDestFile[sizeof(szDestFile) - 1] = '\0';
            
            if(!FileCopy(szSrctFile, szDestFile))
            {
                nRet = ERR_FAIL;
                break;
            }
            
            iter++;
        }
    }
    
#else
	char szCmd[1024];
	char szLogFiles[MAX_PATH];

	char szSrcFile[MAX_PATH];
	char szDestFile[MAX_PATH];

	time_t t;
	struct tm *lt;
	time(&t);
	lt = localtime(&t);

	sprintf(szLogFiles, "%s/*%s", m_szBasePath.c_str(), GetLogFileExt());
	
	sprintf(szSrcFile, "%s/log%04d%02d%02d_manual.tar", 
		    m_szBasePath.c_str(), 
			lt->tm_year + 1900, 
			lt->tm_mon + 1, 
			lt->tm_mday);

	sprintf(szDestFile, "%s/log%04d%02d%02d_manual.tar.gz", 
		    m_szArchDir.c_str(), 
			lt->tm_year + 1900, 
			lt->tm_mon + 1, 
			lt->tm_mday);
	
	sprintf(szCmd, "tar cvf %s %s", szSrcFile, szLogFiles);

    //即使有同名文件tar是会覆盖后返回成功与gzip不同
    int nSysRet = system1(szCmd);
    if(0 == nSysRet)
	{
		sprintf(szCmd, "gzip -f %s", szSrcFile);
        nSysRet = system1(szCmd);
        if (0 == nSysRet)
		{
			strcat(szSrcFile, ".gz");
            nRet = ERR_SUCCESS;
			if (FileCopy(szSrcFile,szDestFile) != TRUE)
			{
				nRet = ERR_FAIL;
			}
            if (FileDel(szSrcFile) != TRUE)
			{
				nRet = ERR_FAIL;
			}
		}
        else //删除原有的.gz后再压缩一次
        {
            char szTmpPath[MAX_PATH];
            strncpy(szTmpPath,szSrcFile,MAX_PATH);
            strcat(szTmpPath, ".gz");
            FileDel(szTmpPath);
            nSysRet = system1(szCmd);
            if(nSysRet == 0)
            {
                strcat(szSrcFile, ".gz");
                nRet = ERR_SUCCESS;
                if (FileCopy(szSrcFile,szDestFile) != TRUE)
                {
                    nRet = ERR_FAIL;
                }
                if (FileDel(szSrcFile) != TRUE)
                {
                    nRet = ERR_FAIL;
                }
            }
        }
	}
#endif //_PLATFORM_WIN32

    //写日志
    SLog log;
	log.LogType    = SLog::LOG_OPER;
	if (ERR_SUCCESS == nRet)
	{
		log.OperResult = SLog::OPER_SUCCESS;
	}
	else
	{
		log.OperResult = SLog::OPER_FAIL;
	}

    CParseResult *pParse = *(CParseResult **)pMsg->Data;

    strncpy(log.szUserName, pMsg->UserName,LOG_USR_NAME_LEN);
    log.szUserName[LOG_USR_NAME_LEN-1] = 0;

    log.LogType = SLog::LOG_OPER;
    log.uIPAddr = pMsg->IPAddr;
    strncpy(log.szOperCmd,pParse->GetCmdName(), LOG_OPER_CMD_LEN);
    log.szOperCmd[LOG_OPER_CMD_LEN-1] = 0;

    strncpy(log.szCmdDesc, pParse->GetCmdDspName(), LOG_CMD_DESC_LEN);
    log.szCmdDesc[LOG_CMD_DESC_LEN-1] = 0;

	WriteLog(&log);

    CMMLReport Report(THISMODULE);

    Report.MakeReportHead();            //构造报文头

    //构造命令回显

	Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, nRet, THISMODULE);
	if (ERR_SUCCESS == nRet)
	{
		//构造返回报文正文
		Report.AddString(STR_OPRSUCCESS);
	}
	else
	{
		//构造返回报文正文
		Report.AddString(STR_OPRFAIL);
	}

    //增加新行
    Report.AddString(STR_NEWLINE);
    //构造报文尾 
    Report.MakeReportTail();
               
    SendMsgToMML(Report.GetReport(),pMsg);
}

void CLog::SetLogArchiveTime(MSG_INTRA* pMsg)
{
	// 从消息帧的Data域里取出CParseResult对象
    CParseResult *pParse = *(CParseResult **)pMsg->Data;
	
    tm dd;
    memset(&dd, 0, sizeof(tm));
	GetTimeValue(pParse, 1, &m_tArchive, &dd);

	char szTime[12];
	sprintf(szTime, "%02d:%02d:%02d", 
		    m_tArchive.tm_hour, 
			m_tArchive.tm_min, 
			m_tArchive.tm_sec);
    
	int nRet;

    // 打开配置文件
    CINIFile ConfigFile(GetCfgFilePath());

	if (TRUE == ConfigFile.Open())
	{
		ConfigFile.WriteString(CFG_SEC_LOG,CFG_LOG_ARCTIME,szTime);    
       
        // 关闭配置文件
        ConfigFile.Close();

		nRet = ERR_SUCCESS;
    }
	else
	{
		nRet = ERR_FAIL;
	}

	//写日志
	SLog log;

	log.LogType    = SLog::LOG_OPER;
	if (ERR_SUCCESS == nRet)
	{
		log.OperResult = SLog::OPER_SUCCESS;
	}
	else
	{
		log.OperResult = SLog::OPER_FAIL;
	}

    strncpy(log.szUserName, pMsg->UserName,LOG_USR_NAME_LEN);
    log.szUserName[LOG_USR_NAME_LEN-1] = 0;

    log.LogType = SLog::LOG_OPER;
    log.uIPAddr = pMsg->IPAddr;
    strncpy(log.szOperCmd, pParse->GetCmdName(), LOG_OPER_CMD_LEN);
    log.szOperCmd[LOG_OPER_CMD_LEN-1] = 0;

    strncpy(log.szCmdDesc, pParse->GetCmdDspName(), LOG_CMD_DESC_LEN);
    log.szCmdDesc[LOG_CMD_DESC_LEN-1] = 0;

	WriteLog(&log);

    CMMLReport Report(THISMODULE);

    Report.MakeReportHead();            //构造报文头

    //构造命令回显
	Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, nRet, THISMODULE);
	if (ERR_SUCCESS == nRet)
	{
		//构造返回报文正文
		Report.AddString(STR_OPRSUCCESS);
	}
	else
	{
		//构造返回报文正文
		Report.AddString(STR_OPRFAIL);
	}

    //增加新行
    Report.AddString(STR_NEWLINE);
    //构造报文尾 
    Report.MakeReportTail();
               
    SendMsgToMML(Report.GetReport(),pMsg);
}

void CLog::InitArchiveTime(const char* szTime)
{
	char szTemp[12];

	strncpy(szTemp, &szTime[0], 2);
	szTemp[2] = '\0';
	m_tArchive.tm_hour = atoi(szTemp);

	strncpy(szTemp, &szTime[3], 2);
	szTemp[2] = '\0';
	m_tArchive.tm_min = atoi(szTemp);

	strncpy(szTemp, &szTime[5], 2);
	szTemp[2] = '\0';
	m_tArchive.tm_sec = atoi(szTemp);	
}

void CLog::OnTimer(const long nTimerID)
{
    if(nTimerID == m_lTimerID5Sec)
    {
        time_t t;
        time(&t);
        struct tm tmNow;
        
        tmNow = *localtime(&t);
        
        if ((tmNow.tm_hour == m_tArchive.tm_hour) 
            && (tmNow.tm_min  == m_tArchive.tm_min) 
            && ((tmNow.tm_sec >= m_tArchive.tm_sec) 
            && (tmNow.tm_sec <= m_tArchive.tm_sec + 5)))
        {
            AutoArchiveLog();
        }
    }
    else if(nTimerID == m_lTimerID1Day)
    {
        // 删除过期的日志文件
        DeleteOutdatedLog();
    }
}
void CLog::AutoArchiveLog()
{

	int nRet = ERR_FAIL;
    
#ifdef _PLATFORM_WIN32
    LIST<STRING> LogFileList;
    char szSrctFile[MAX_PATH] = "";
    char szDestFile[MAX_PATH] = "";
    
    nRet = GetLogFileList(LogFileList);
    LogFileList.sort();
    
    if(nRet != ERR_FAIL)
    { 
        //最后一个文件处于写状态，并不备份
        if(LogFileList.size() > 0)
        {
            LogFileList.pop_back();
        }
        
        //开始归档日志
        LIST<STRING>::iterator iter = LogFileList.begin();   
        while(iter != LogFileList.end())
        {
            SNPRINTF(szSrctFile,  MAX_PATH,
                     "%s/%s",
                     m_szBasePath.c_str(), 
                     (*iter).c_str());
            szSrctFile[sizeof(szSrctFile) - 1] = '\0';
            
            SNPRINTF(szDestFile,  MAX_PATH,
                     "%s/%s",
                     m_szArchDir.c_str(), 
                     (*iter).c_str());
            szDestFile[sizeof(szDestFile) - 1] = '\0';
            
            if(!FileCopy(szSrctFile, szDestFile))
            {
                nRet = ERR_FAIL;
                break;
            }
            
            iter++;
        }
    }
    
#else
	char szCmd[1024];
	char szLogFiles[MAX_PATH];

	char szSrcFile[MAX_PATH];
	char szDestFile[MAX_PATH];

	time_t t;
	struct tm *lt;
	time(&t);
	lt = localtime(&t);

	sprintf(szLogFiles, "%s/*%s", m_szBasePath.c_str(), GetLogFileExt());
	
	sprintf(szSrcFile, "%s/log%04d%02d%02d.tar", 
		    m_szBasePath.c_str(), 
			lt->tm_year + 1900, 
			lt->tm_mon + 1, 
			lt->tm_mday);

	sprintf(szDestFile, "%s/log%04d%02d%02d.tar.gz", 
		    m_szArchDir.c_str(), 
			lt->tm_year + 1900, 
			lt->tm_mon + 1, 
			lt->tm_mday);
	
	sprintf(szCmd, "tar cvf %s %s", szSrcFile, szLogFiles);


    //即使有同名文件tar是会覆盖后返回成功,与gzip不同
    int nSysRet = system1(szCmd);
    if(0 == nSysRet)
    {
        sprintf(szCmd, "gzip -f %s", szSrcFile);
        nSysRet = system1(szCmd);
        if (0 == nSysRet)
        {
            strcat(szSrcFile, ".gz");
            nRet = ERR_SUCCESS;
			if (FileCopy(szSrcFile,szDestFile) != TRUE)
			{
				nRet = ERR_FAIL;
			}
            if (FileDel(szSrcFile) != TRUE)
			{
				nRet = ERR_FAIL;
			}
		}
        else //删除原有的.gz后再压缩一次
        {
            char szTmpPath[MAX_PATH];
            strncpy(szTmpPath,szSrcFile,MAX_PATH);
            strcat(szTmpPath, ".gz");
            FileDel(szTmpPath);
            nSysRet = system1(szCmd);
            if(nSysRet == 0)
            {
                strcat(szSrcFile, ".gz");
                nRet = ERR_SUCCESS;
                if (FileCopy(szSrcFile,szDestFile) != TRUE)
                {
                    nRet = ERR_FAIL;
                }
                if (FileDel(szSrcFile) != TRUE)
                {
                    nRet = ERR_FAIL;
                }
            }
        }
	}
#endif //_PLATFORM_WIN32    

	//写日志
	SLog log;

	log.LogType    = SLog::LOG_SYS;
	if (ERR_SUCCESS == nRet)
	{
		log.OperResult = SLog::OPER_SUCCESS;
	}
	else
	{
		log.OperResult = SLog::OPER_FAIL;
	}

    strncpy(log.szUserName, THISMODULE, LOG_USR_NAME_LEN);
    log.szUserName[LOG_USR_NAME_LEN-1] = 0;

    log.uIPAddr = 0;
    strncpy(log.szOperCmd, "-", LOG_OPER_CMD_LEN);
    log.szOperCmd[LOG_OPER_CMD_LEN-1] = 0;

    strncpy(log.szCmdDesc, S_LOG_AUTO_ARCHIEVE, LOG_CMD_DESC_LEN);
    log.szCmdDesc[LOG_CMD_DESC_LEN-1] = 0;

	WriteLog(&log);		
}

void CLog::GetLogArchiveTime(MSG_INTRA *pMsg)
{
    // 从消息帧的Data域里取出CParseResult对象
    CParseResult *pParse = *(CParseResult **)pMsg->Data;
    char szTmp[64];
    
    //构造MML报文，响应MML客户端
    CMMLReport Report(THISMODULE);
    
    Report.MakeReportHead();            //构造报文头
    //构造命令回显
    Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, ERR_SUCCESS, THISMODULE);
    //构造返回报文正文
    char szArcTime[64];
    strftime(szArcTime,64,"%H:%M:%S",&m_tArchive);
    SNPRINTF(szTmp,64,"%s  =  %s","archive time",szArcTime);
    szTmp[63] = 0;
    Report.AddString(szTmp);
    //增加新行
    Report.AddString(STR_NEWLINE);
    //构造报文尾 
    Report.MakeReportTail();
    
    SendMsgToMML(Report.GetReport(),pMsg);
}

//导出日志
void CLog::ExportLog(MSG_INTRA* pMsg)
{
    CParseResult* pParse = *((CParseResult**)pMsg->Data);

    //解析命令参数

    //开始时间
    tm tSTime;
    memset(&tSTime,0,sizeof(tm));
    const char *pcTmp;
    
     //获取年份
    pcTmp = pParse->GetStringVal(1,1);
    if (pcTmp != NULL)
    {
        tSTime.tm_year = atoi(pcTmp);
        if (tSTime.tm_year > 1900)
        {
            tSTime.tm_year = tSTime.tm_year - 1900;
        }
    }    

    //获取月份
    pcTmp = pParse->GetStringVal(1,2);    
    if (pcTmp != NULL)
    {
        tSTime.tm_mon = atoi(pcTmp);
        if (tSTime.tm_mon > 1)
        {
            tSTime.tm_mon = tSTime.tm_mon - 1;
        }
    }    
    
    //获取日期
    pcTmp = pParse->GetStringVal(1,3);
    if (pcTmp != NULL)
    {
        tSTime.tm_mday = atoi(pcTmp);
    }    
      
    //获取小时
    pcTmp = pParse->GetStringVal(3,1);
    if (pcTmp != NULL)
    {
        tSTime.tm_hour = atoi(pcTmp);
    }    

    //获取分钟
    pcTmp = pParse->GetStringVal(3,2);
    if (pcTmp != NULL)
    {
        tSTime.tm_min = atoi(pcTmp);
    }    

    //获取秒
    pcTmp = pParse->GetStringVal(3,3);
    if (pcTmp != NULL)
    {
        tSTime.tm_sec = atoi(pcTmp);
    }    

    //结束时间   
    // 如果tEime没有取值，则取2038年       
    //mktime handles dates in any time zone from midnight, January 1, 1970, to January 18, 19:14:07, 2038. 
    tm    tETime;
    memset(&tETime,0,sizeof(tm));
    //获取年份 
    pcTmp = pParse->GetStringVal(2,1);
    if (pcTmp != NULL)
    {
        tETime.tm_year = atoi(pcTmp);
        if (tETime.tm_year > 1900)
        {
            tETime.tm_year = tETime.tm_year - 1900;
        }
        else
        {
			tETime.tm_year = 2038-1900;
        }            
    }
    else
    {
        tETime.tm_year = 2038-1900;
    }    
    
    //获取月份
    pcTmp = pParse->GetStringVal(2,2);    
    if (pcTmp != NULL)
    {
        tETime.tm_mon = atoi(pcTmp);
        if (tETime.tm_mon > 1)
        {
            tETime.tm_mon = tETime.tm_mon - 1;
        }
    }    
    
    //获取日期
    pcTmp = pParse->GetStringVal(2,3);
    if (pcTmp != NULL)
    {
        tETime.tm_mday = atoi(pcTmp);
    }    
    
    //获取小时
    pcTmp = pParse->GetStringVal(4,1);
    if (pcTmp != NULL)
    {
        tETime.tm_hour = atoi(pcTmp);
    }    
    
    //获取分钟
    pcTmp = pParse->GetStringVal(4,2);
    if (pcTmp != NULL)
    {
        tETime.tm_min = atoi(pcTmp);
    }    
    
    //获取秒
    pcTmp = pParse->GetStringVal(4,3);
    if (pcTmp != NULL)
    {
        tETime.tm_sec = atoi(pcTmp);
    }
    
    //获取日志类型
    int    nLogType = 1; 
    pParse->GetDigitVal(&nLogType,5);

    //获取文件名称 
    int nLen = strlen(pParse->GetStringVal(6));
    char szFileName[MAX_PATH];
    szFileName[0] = '\0';
    strncpy(szFileName, pParse->GetStringVal(6), MAX_PATH);
    szFileName[MAX_PATH] = '\0';

    int nErrCode = 1;
    time_t st,et;
    st = mktime(&tSTime);
    et = mktime(&tETime);

    if ( difftime(et,st) < 0)
    {
        // 返回给MML的报文信息为“查询的起始时间不能大于结束时间”
        SendSimpleReport(pParse->GetCmdLine(),nErrCode,"Query Time error",pMsg); 
        return;
    }
    
    //如果开始时间缺省,则为第一条日志的时间
    if (-1 == st) 
    {
        STRING szLogFile;
        SLog Log;

        LIST<STRING> LogFileList;
        if (ERR_SUCCESS != GetLogFileList(LogFileList))
        {
            SendSimpleReport(pParse->GetCmdLine(),nErrCode,"Get File list Failed",pMsg);
            return;
        }

        // 按照日志文件名称排序
        LogFileList.sort();
        
        for (; !LogFileList.empty();LogFileList.pop_front())        
        {
            if (m_pQueryLogUtil == NULL)
            {
                m_pQueryLogUtil = CreateLogUtilObj();
                if (NULL == m_pQueryLogUtil)
                {
                    LogFileList.clear();
                    return;
                }
            }

            szLogFile = MakePath(m_szBasePath, LogFileList.front().c_str());
            
            if (0 != m_pQueryLogUtil->OpenFile(szLogFile.c_str()))
            {
                delete m_pQueryLogUtil;
                m_pQueryLogUtil = NULL;
                continue;
            }
            
            int nRet;
            nRet = m_pQueryLogUtil->ReadLog(EL_FIRST,
                                            Log.uSerialNo,
                                            Log.LogType,
                                            Log.szUserName,
                                            sizeof(Log.szUserName),
                                            Log.uIPAddr,
                                            Log.tLogTime,
                                            Log.OperResult,
                                            Log.szOperCmd,
                                            sizeof(Log.szOperCmd),
                                            Log.szCmdDesc,
                                            sizeof(Log.szCmdDesc));
            if ((nRet == 0 || nRet == 1) && Log.LogType == nLogType )
            {
                st = Log.tLogTime;
            }
            else
            {
                while (nRet == 0)
                {
                    nRet = m_pQueryLogUtil->ReadLog(EL_NEXT,
                                                    Log.uSerialNo,
                                                    Log.LogType,
                                                    Log.szUserName,
                                                    sizeof(Log.szUserName),
                                                    Log.uIPAddr,
                                                    Log.tLogTime,
                                                    Log.OperResult,
                                                    Log.szOperCmd,
                                                    sizeof(Log.szOperCmd),
                                                    Log.szCmdDesc,
                                                    sizeof(Log.szCmdDesc));
                    
                    if ((nRet == 0 || nRet ==1) && Log.LogType == nLogType )
                    {
                        st = Log.tLogTime;
                        break;                    
                    }
                    
                 }         
            }


            m_pQueryLogUtil->CloseFile();
            delete m_pQueryLogUtil;
            m_pQueryLogUtil = NULL;

            if (st != -1)
            {
                break;
            }           
        }

        if(NULL != m_pQueryLogUtil)
        {
            delete m_pQueryLogUtil;
            m_pQueryLogUtil = NULL;
        }
        LogFileList.clear();
      
        if (-1 == st)
        {
            SendSimpleReport(pParse->GetCmdLine(),nErrCode,"Get StartTime Failed",pMsg);
            return;
        }
        
        tSTime.tm_year = localtime(&st)->tm_year;
        tSTime.tm_mon  = localtime(&st)->tm_mon;
        tSTime.tm_mday = localtime(&st)->tm_mday;
        tSTime.tm_hour = localtime(&st)->tm_hour;
        tSTime.tm_min  = localtime(&st)->tm_min;
        tSTime.tm_sec  = localtime(&st)->tm_sec;
  
    }
    
    //如果结束时间缺省，则为最后一条日志的时间
    if (-1 == et)
    {
        STRING szLogFile;
        SLog Log;
        
        LIST<STRING> LogFileList;
        if (ERR_SUCCESS != GetLogFileList(LogFileList))
        {
            SendSimpleReport(pParse->GetCmdLine(),nErrCode,"Session ID = 2",pMsg);
            return;
        }    
        
        // 按照日志文件名称排序
        LogFileList.sort();
        
        for (; !LogFileList.empty();LogFileList.pop_front())        
        {
            if (m_pQueryLogUtil == NULL)
            {
                m_pQueryLogUtil = CreateLogUtilObj();
                if (NULL == m_pQueryLogUtil)
                {
                    LogFileList.clear();
                    return;
                }
            }
            
            szLogFile = MakePath(m_szBasePath, LogFileList.front().c_str());
            
            if (0 != m_pQueryLogUtil->OpenFile(szLogFile.c_str()))
            {
                delete m_pQueryLogUtil;
                m_pQueryLogUtil = NULL;
                continue;
            }
            
            int nRet;
            nRet = m_pQueryLogUtil->ReadLog(EL_FIRST,
                                            Log.uSerialNo,
                                            Log.LogType,
                                            Log.szUserName,
                                            sizeof(Log.szUserName),
                                            Log.uIPAddr,
                                            Log.tLogTime,
                                            Log.OperResult,
                                            Log.szOperCmd,
                                            sizeof(Log.szOperCmd),
                                            Log.szCmdDesc,
                                            sizeof(Log.szCmdDesc));

            if ((nRet == 0 || nRet ==1) && Log.LogType == nLogType )
            {
                et = Log.tLogTime;
            }

            while (nRet == 0)
            {
                nRet = m_pQueryLogUtil->ReadLog(EL_NEXT,
                                                Log.uSerialNo,
                                                Log.LogType,
                                                Log.szUserName,
                                                sizeof(Log.szUserName),
                                                Log.uIPAddr,
                                                Log.tLogTime,
                                                Log.OperResult,
                                                Log.szOperCmd,
                                                sizeof(Log.szOperCmd),
                                                Log.szCmdDesc,
                                                sizeof(Log.szCmdDesc));
                
                if ((nRet == 0 || nRet ==1) || Log.LogType == nLogType )
                {
                    et = Log.tLogTime;                    
                }
                
            }         
                                  
            m_pQueryLogUtil->CloseFile();
            delete m_pQueryLogUtil;
            m_pQueryLogUtil = NULL;
                     
        }
        
        if(NULL != m_pQueryLogUtil)
        {
            delete m_pQueryLogUtil;
            m_pQueryLogUtil = NULL;
        }
        LogFileList.clear();
        
        if (-1 == et)
        {
            SendSimpleReport(pParse->GetCmdLine(),nErrCode,"Get End Time Failed",pMsg);
            return;
        }
       
        tETime.tm_year = localtime(&et)->tm_year;
        tETime.tm_mon  = localtime(&et)->tm_mon;
        tETime.tm_mday = localtime(&et)->tm_mday;
        tETime.tm_hour = localtime(&et)->tm_hour;
        tETime.tm_min  = localtime(&et)->tm_min;
        tETime.tm_sec  = localtime(&et)->tm_sec;         

    }

    //查询日志
    BOOL bCreateFile = TRUE;

    //查询出符合条件的日志
    STRING szLogFile;
    SLog Log;
    FILE* fpLog = NULL;
    
    // 起始日志文件名
    char filename[9];
    sprintf(filename, "%04d%02d%02d", tSTime.tm_year+1900,
        tSTime.tm_mon+1, tSTime.tm_mday);
    filename[8] = '\0';
    STRING StartLogFile(filename);
    StartLogFile += GetLogFileExt();
    
    // 结束日志文件名
    sprintf(filename, "%04d%02d%02d", tETime.tm_year+1900,
        tETime.tm_mon+1,tETime.tm_mday);
    filename[8] = '\0';
    STRING EndLogFile(filename);
    EndLogFile += GetLogFileExt();

    // 获取日志根目录下日志文件名列表
    LIST<STRING> LogFileList;
    if (ERR_SUCCESS != GetLogFileList(LogFileList))
    {
        SendSimpleReport(pParse->GetCmdLine(),nErrCode,"Get Log File Failed",pMsg);
        return;
    }
    
    LIST<STRING>::iterator iter = LogFileList.begin();    
    while(iter != LogFileList.end())
    {
        // 仅查询介于开始文件名称和结束文件名称之间的日志文件
        if ( (*iter < StartLogFile) || (*iter > EndLogFile) )
        {
            iter = LogFileList.erase(iter);
            continue;
        }
        iter++;
    }

    // 按照日志文件名称排序
    LogFileList.sort();
    ILogUtil* lXml = m_logUtilEncap.CreateLogUtil();
  
    for (; !LogFileList.empty();LogFileList.pop_front())        
    {
        if (m_pQueryLogUtil == NULL)
        {
            m_pQueryLogUtil = CreateLogUtilObj();
            if (NULL == m_pQueryLogUtil)
            {
                LogFileList.clear();
                return;
            }
        }
        
        szLogFile = MakePath(m_szBasePath, LogFileList.front().c_str());
        
        if (0 != m_pQueryLogUtil->OpenFile(szLogFile.c_str()))
        {
            delete m_pQueryLogUtil;
            m_pQueryLogUtil = NULL;
            continue;
        }
        
        int nRet;
        nRet = m_pQueryLogUtil->ReadLog(EL_FIRST,
                                        Log.uSerialNo,
                                        Log.LogType,
                                        Log.szUserName,
                                        sizeof(Log.szUserName),
                                        Log.uIPAddr,
                                        Log.tLogTime,
                                        Log.OperResult,
                                        Log.szOperCmd,
                                        sizeof(Log.szOperCmd),
                                        Log.szCmdDesc,
                                        sizeof(Log.szCmdDesc));
        
        if (nRet != 0 && nRet != 1)
        {
            delete m_pQueryLogUtil;
            m_pQueryLogUtil = NULL;
            continue;
        }

        if ((difftime(Log.tLogTime,st) >= 0) && 
            (difftime(et,Log.tLogTime) >= 0) &&
            nLogType == Log.LogType)
        {
            if (bCreateFile)
            {
                if (!(strlen(szFileName)))
                {
                    sprintf(szFileName,"CG-%04d%02d%02d%02d%02d%02d-%04d%02d%02d%02d%02d%02d-LOG.XML", tSTime.tm_year + 1900,
                        tSTime.tm_mon+1, tSTime.tm_mday,tSTime.tm_hour,tSTime.tm_min,tSTime.tm_sec,
                        tETime.tm_year + 1900,tETime.tm_mon+1, tETime.tm_mday,tETime.tm_hour,tETime.tm_min,tETime.tm_sec);
                } 
                
                STRING strXmlPath = m_szBasePath + "/temp";
                STRING strXmlLogFile =  MakePath(strXmlPath, szFileName);
                //创建一个.xml的日志文件
                if (!(CreateDeepDir(strXmlPath.c_str())))
                {
                    SendSimpleReport(pParse->GetCmdLine(),nErrCode,"Create Directory Failed",pMsg);
                    return;
                }
                if (0 != lXml->OpenFile(strXmlLogFile.c_str())) 
                {
                    SendSimpleReport(pParse->GetCmdLine(),nErrCode,"Create XML File Failed",pMsg);
                    return;                        
                }
                
                m_szXmlLogName = STRING(szFileName);
                
                bCreateFile = FALSE;   
            }
            
            lXml->AppendLog(Log.uSerialNo,
                            Log.LogType,
                            Log.szUserName,
                            Log.uIPAddr,
                            Log.tLogTime,
                            Log.OperResult,
                            Log.szOperCmd,
                            Log.szCmdDesc);         
        }

        while (0 == nRet)
        {
            nRet = m_pQueryLogUtil->ReadLog(EL_NEXT,
                                            Log.uSerialNo,
                                            Log.LogType,
                                            Log.szUserName,
                                            sizeof(Log.szUserName),
                                            Log.uIPAddr,
                                            Log.tLogTime,
                                            Log.OperResult,
                                            Log.szOperCmd,
                                            sizeof(Log.szOperCmd),
                                            Log.szCmdDesc,
                                            sizeof(Log.szCmdDesc));

            if (difftime(st,Log.tLogTime) > 0 || 
                difftime(Log.tLogTime,et) > 0)
            {
                continue;
            }

            if (Log.LogType != nLogType)
            {
                continue;
            }

            if (bCreateFile)
            {
                if (!(strlen(szFileName)))
                {
                    sprintf(szFileName,"CG-%04d%02d%02d%02d%02d%02d-%04d%02d%02d%02d%02d%02d-LOG.XML", tSTime.tm_year + 1900,
                        tSTime.tm_mon+1, tSTime.tm_mday,tSTime.tm_hour,tSTime.tm_min,tSTime.tm_sec,
                        tETime.tm_year + 1900,tETime.tm_mon+1, tETime.tm_mday,tETime.tm_hour,tETime.tm_min,tETime.tm_sec);
                } 
                
                STRING strXmlPath = m_szBasePath + "/temp";
                STRING strXmlLogFile =  MakePath(strXmlPath, szFileName);
                //创建一个.xml的日志文件
                if (!(CreateDeepDir(strXmlPath.c_str())))
                {
                    SendSimpleReport(pParse->GetCmdLine(),nErrCode,"Create Directory Failed",pMsg);
                    return;
                }
                if (0 != lXml->OpenFile(strXmlLogFile.c_str())) 
                {
                    SendSimpleReport(pParse->GetCmdLine(),nErrCode,"Create XML File Failed",pMsg);
                    return;                        
                }
                
                m_szXmlLogName = STRING(szFileName);
                
                bCreateFile = FALSE;   
            }
            if (1 == nRet || 0 == nRet)
            {
                lXml->AppendLog(Log.uSerialNo,
                                Log.LogType,
                                Log.szUserName,
                                Log.uIPAddr,
                                Log.tLogTime,
                                Log.OperResult,
                                Log.szOperCmd,
                                Log.szCmdDesc);
            }
 
        }

        m_pQueryLogUtil->CloseFile();
        delete m_pQueryLogUtil;
        m_pQueryLogUtil = NULL;
     } 

     if(NULL != m_pQueryLogUtil)
     {
         delete m_pQueryLogUtil;
         m_pQueryLogUtil = NULL;
     }
     LogFileList.clear();

    if (!bCreateFile) 
    {
        lXml->CloseFile();
    }
    nErrCode = 0;
    
    //返回报文
    //SendSimpleReport(pParse->GetCmdLine(),nErrCode,"Success",pMsg);

    SendSimpleReport(pParse->GetCmdLine(),nErrCode,"Session ID = 1",pMsg);
	//by ldf 2004-08-26 OM-M2000联调
    CMMLReport MMLReport("HUAWEI");
    MMLReport.MakeReportHead();
    MMLReport.MakeCmdDisplay("EXP LOGFILE:",0,0,"O&M");
    MMLReport.AddString(STR_NEWLINE);
    MMLReport.MakeReportItem("Report Type","ExportLog",24,ALIGN_RIGHT,9,ALIGN_LEFT,1);
    MMLReport.MakeReportItem("Status","Success",24,ALIGN_RIGHT,7,ALIGN_LEFT,1);
    MMLReport.AddString(STR_NEWLINE);
    MMLReport.MakeReportTail();
	
	char* szReport;
    szReport = new char[strlen(MMLReport.GetReport()) + 1];
    sprintf(szReport,"%s",MMLReport.GetReport());
	
	//发送到客户端
	int nStrLen = strlen(szReport);
    MSG_INTRA* pMsgProcess = new(nStrLen + 1) MSG_INTRA;
    pMsgProcess->SenderPid   = GetProcessType();
    pMsgProcess->SenderMid   = GetModuleType();
    pMsgProcess->ReceiverPid = PT_OUTSIDE;
    pMsgProcess->ReceiverMid = MT_MMLDEBUG;
	
    strcpy(pMsgProcess->UserName,pMsg->UserName);
	
    pMsgProcess->AppType = APP_MML_NORM;
    pMsgProcess->ClientNo =pMsg->ClientNo;
    pMsgProcess->IPAddr      =pMsg ->IPAddr;
    pMsgProcess->CmdCode     = CMD_MML;
	
    memcpy(pMsgProcess->Data, szReport, nStrLen + 1);
	delete[] szReport;
	
    SendMsg(pMsgProcess);
	//end
	

    return;
}

//上传日志
void CLog::UploadLog(MSG_INTRA* pMsg)
{
    CParseResult* pParse = *((CParseResult**)pMsg->Data);
    
    //解析命令参数
    STRING strFTPSeverIp = pParse->GetStringVal(1);
    STRING strUserName = pParse->GetStringVal(2);
    STRING strPassword = pParse->GetStringVal(3);
    STRING strDSTFile = pParse->GetStringVal(4);
    int nLogType = 1;
    pParse->GetDigitVal(&nLogType,5);
    int nPressTag = 0;
    pParse->GetDigitVal(&nPressTag,6);
    
    if (strUserName.empty())
    {
        strUserName = "root"; 
    }
    
    if (strPassword.empty())
    {
        strPassword = "root";
    }
    
    int nPos = strDSTFile.find_last_of("/");
    STRING strDSTFilePath = "/";
    STRING strDSTFileName = strDSTFile.substr(nPos + 1,strDSTFile.length());
    if (nPos > 0)
    {
        strDSTFilePath = strDSTFile.substr(0,nPos);
    }
    
    //上传日志
    char szDestHostIp[MAX_HOST_LEN];
    
    CINIFile ini(GetCfgFilePath());
    int nRet = ini.Open();
    if ( nRet != TRUE)
    {
        SendSimpleReport(pParse->GetCmdLine(),1,"Open iGWB.ini Failed!",pMsg);
        return ;
    }
    else
    {
        
        ini.GetString(CFG_SEC_MML,CFG_MML_KEY_LOCAL_IP,
            "127.0.0.1",  szDestHostIp,
            MAX_HOST_LEN);
        ini.Close();
    }
    
    CFtpClient  FtpClient;
    
    //读出需要上载的原文件名
    STRING  strSrcFileName;
    strSrcFileName = m_szBasePath + "/temp/" + m_szXmlLogName;
    
    if (strDSTFileName.empty())
    {
        strDSTFileName = m_szXmlLogName;
    }
    
    if (0 != FtpClient.Connect(strFTPSeverIp.c_str(),strUserName.c_str(),strPassword.c_str(),szDestHostIp))
    {
        SendSimpleReport(pParse->GetCmdLine(),1,"connect ftp server failed!",pMsg);
        return;
    }
    
    if (0 != FtpClient.SetDirectory(strDSTFilePath.c_str()))
    {
        if ( 0 != FtpClient.MakeDirectory(strDSTFilePath.c_str()))
        {
            SendSimpleReport(pParse->GetCmdLine(),1,"Create the directory failed!",pMsg);
            return;
        }
        FtpClient.SetDirectory(strDSTFilePath.c_str());
    }
    
    if (0 != FtpClient.PutFile(strSrcFileName.c_str(),strDSTFileName.c_str()) )
    {
        SendSimpleReport(pParse->GetCmdLine(),1,"put file to ftp server failed!",pMsg);
        return;        
    }
    
    FtpClient.Disconnect();
    
    //返回报文
    //SendSimpleReport(pParse->GetCmdLine(),0,"Success",pMsg);
    SendSimpleReport(pParse->GetCmdLine(),0,"Session ID = 1",pMsg);
	
    //by ldf 2004-08-26 OM-M2000联调
    CMMLReport MMLReport("HUAWEI");
    MMLReport.MakeReportHead();
    MMLReport.MakeCmdDisplay("ULD LOGFILE:",0,0,"O&M");
    MMLReport.AddString(STR_NEWLINE);
    MMLReport.MakeReportItem("Report Type","Upload",24,ALIGN_RIGHT,9,ALIGN_LEFT,1);
    MMLReport.MakeReportItem("Status","Success",24,ALIGN_RIGHT,7,ALIGN_LEFT,1);
    MMLReport.AddString(STR_NEWLINE);
    MMLReport.MakeReportTail();
	
	char* szReport;
    szReport = new char[strlen(MMLReport.GetReport()) + 1];
    sprintf(szReport,"%s",MMLReport.GetReport());
	
	//发送到客户端
	int nStrLen = strlen(szReport);
    MSG_INTRA* pMsgProcess = new(nStrLen + 1) MSG_INTRA;
    pMsgProcess->SenderPid   = GetProcessType();
    pMsgProcess->SenderMid   = GetModuleType();
    pMsgProcess->ReceiverPid = PT_OUTSIDE;
    pMsgProcess->ReceiverMid = MT_MMLDEBUG;
	
    strcpy(pMsgProcess->UserName,pMsg->UserName);
	
    pMsgProcess->AppType = APP_MML_NORM;
    pMsgProcess->ClientNo =pMsg->ClientNo;
    pMsgProcess->IPAddr      =pMsg ->IPAddr;
    pMsgProcess->CmdCode     = CMD_MML;
	
    memcpy(pMsgProcess->Data, szReport, nStrLen + 1);
	delete[] szReport;
	
    SendMsg(pMsgProcess);
	//end
	

    return;
}

void CLog::SendSimpleReport(const char* szCmd, int nRetCode, const char* szInfo,MSG_INTRA *pMsg)
{
    m_pReport->MakeReportHead();
    m_pReport->MakeCmdDisplay(szCmd, 10640, nRetCode, "LOG");
    m_pReport->AddString("\r\n");
    m_pReport->AddString(szInfo);
    m_pReport->AddString("\r\n");
    m_pReport->AddString("\r\n");
    
    m_pReport->AddString("");
    m_pReport->MakeReportTail();
    
    SendMsgToMML(m_pReport->GetReport(), pMsg); 
}
