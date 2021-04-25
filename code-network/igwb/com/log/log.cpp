/**************************************************************************
 *��Ʒ��    : iGateway Bill V200        
 *ģ����    : ��־ģ��(Log)
 *�ļ���    : log.cpp
 *������Ϣ  : ���ļ�����CLog��ʵ��
 *�汾˵��  : V200R001i00M00B00D1130A   
 *��Ȩ��Ϣ  : ��Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����      : ������
 *����ʱ��  : 2001-10-30
 *�޸ļ�¼  : 
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
����ԭ��    public: CLog (void);
��������    ���캯��
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
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
����ԭ��    public: ~CLog ();
��������    ���캯��
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
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
����ԭ��    protected: void ExitInstance (void);
��������    �˳�����
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
void CLog::ExitInstance (void)
{
     
}

    
/**************************************************************************
����ԭ��    protected: int InitInstance (void);
��������    ��ʼ��
�������    ��
�������    ��
����ֵ      ��ʼ���ɹ�ʱ����ERR_SUCCESS
�׳��쳣    ��
***************************************************************************/
int CLog::InitInstance (void)
{ 
    int nRet = ERR_FAIL;
    nRet = CWorkModule::InitInstance();
    if( nRet != ERR_SUCCESS )
    {
        return nRet;
    }
       
    // ��ʼ����Ա����m_tLastDayΪ��ǰʱ��
    time(&m_tLastDay);
    
    // �������ļ�
    CINIFile ConfigFile(GetCfgFilePath());
    nRet = ConfigFile.Open();
    if ( nRet != TRUE)
    {
        TRACE(THISMODULE,S_LOG_OPENINIFILEFAIL);
        return ERR_FAIL;
    }
    else
    {
        // �����ÿ����ȡ��־��������
        m_uSavedDays = ConfigFile.GetInt(CFG_SEC_LOG, 
                                         CFG_LOG_SAVE_DAY, 
                                         CFG_LOG_SAVE_DAY_DEFAULT);
    
        // ��ȡ��־�ļ���ŵĸ�·��
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

        // �����ÿ����ȡ��־��������
        m_bUseXml = ConfigFile.GetInt(CFG_SEC_LOG, 
                                      CFG_USE_XML, 
                                      CFG_USE_XML_DEFAULT);
        // �ر������ļ�
        ConfigFile.Close();
    }
   
    m_pLogUtil = CreateLogUtilObj();
    if(NULL == m_pLogUtil)
    {
        return FALSE;
    }

    //������ 2002-04-25  ���ⵥD13709
    // �򿪵������־�ļ�
    nRet = OpenWriteFile(); 
    if ( nRet != TRUE)
    {
        TRACE(THISMODULE,S_LOG_OPENLOGFILEFAIL);
        return ERR_FAIL;
    }

    // ��ʼ����ǰ��־���к�
    m_uCurrentSN = GetCurrentSN();
	//����

	m_lTimerID5Sec = CWorkModule::SetTimer(FIVE_SECOND_IN_MILISEC);
    m_lTimerID1Day = CWorkModule::SetTimer(6 * 3600 * 1000);
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
MODULE_TYPE CLog::GetModuleType ( void )
{
    return MT_LOG;
}

/**************************************************************************
����ԭ��    protected: BOOL OpenWriteFile ( void );
��������    ����־�ļ�
�������    ��
�������    ��
����ֵ      ��־�ļ��򿪳ɹ�ʱ����TRUE�����򷵻�FALSE
�׳��쳣    ��
***************************************************************************/
BOOL CLog::OpenWriteFile ( void )
{
    // ��ȡ��ǰʱ�䣬ȡ�������պ���־�ļ��ĸ�·�������־�ļ�ȫ·����
    // ��־�ļ�����ʽΪyyyymmdd.xml
    time_t curtime = ACE_OS::time(NULL);
    char filename[9];
    ACE_OS::strftime(filename, 9, "%Y%m%d", ACE_OS::localtime(&curtime));
    filename[8] = '\0';

    STRING szLogFile = MakePath(m_szBasePath, filename);
    szLogFile += GetLogFileExt();

	//�����־����Ŀ¼�����ڣ������ɴ�Ŀ¼
	if (ACE_OS::access(m_szBasePath.c_str(),0) != 0)
	{
		if (!CreateDeepDir(m_szBasePath.c_str()))
		{
			return FALSE;
		}
	}

	// ����m_tLastDayΪ��ǰʱ��
    m_tLastDay = curtime;

	//��ʼ��xml�ļ�����������Ӧ��DOM�����
	int nRet = m_pLogUtil->OpenFile(szLogFile.c_str());

    return nRet == 0;
}

/**************************************************************************
����ԭ��    protected: void CloseWriteFile ( void );
��������    �ر���־�ļ�
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
void CLog::CloseWriteFile ( void )
{
	m_pLogUtil->CloseFile();
}


/**************************************************************************
����ԭ��    protected: BOOL WriteLog ( SLog *pBuf );
��������    ������־
�������    ��־����
�������    ��
����ֵ      ������־�ɹ�����TRUE�����򷵻�FALSE
�׳��쳣    ��
***************************************************************************/
BOOL CLog::WriteLog ( SLog *pBuf )
{
    if(pBuf == NULL)
    {
        TRACE(THISMODULE, S_LOG_LOGBUF_NULL);
        return FALSE;
    }

    // ��ȡ��ǰʱ��
    time_t curtime;
    time(&curtime);
    
    // ��ǰ�������ϴ�д��־�����ڲ�ͬ����µ���־�ļ�
    int CurDay = localtime(&curtime)->tm_mday;
    int LastDay = localtime(&m_tLastDay)->tm_mday;
	//������ 2002-06-04 ���ⵥD14236
    if (CurDay != LastDay)
    {        
        // ���µ���־�ļ�
		if(OpenWriteFile() != TRUE)
        {
            return FALSE;
        }
    }
    
    pBuf->tLogTime  = curtime;
    pBuf->uSerialNo = ++m_uCurrentSN;

    // ��������־����д�뵱ǰ��־�ļ���
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
����ԭ��    protected: UINT4 GetCurrentSN ( void );
��������    ��ȡ��ǰ�ļ����к�
�������    ��
�������    ��
����ֵ      ���ص�ǰ�ļ����к�
�׳��쳣    ��
***************************************************************************/

UINT4 CLog::GetCurrentSN ( void )
{
    UINT4 CurSN = 0;
    
    // ��ȡ��ǰʱ�䣬ȡ�������պ���־�ļ��ĸ�·�������־�ļ�ȫ·����
    time_t curtime;
    tm *ptm;
    time(&curtime);
    ptm = localtime(&curtime);

    // ��־�ļ�����ʽΪyyyymmdd.xml
    char filename[9];
    sprintf(filename,"%04d%02d%02d", ptm->tm_year + 1900, ptm->tm_mon + 1,ptm->tm_mday);
    filename[8] = '\0';
    //��ǰ��־�ļ�
    STRING szCurLogFile = MakePath(m_szBasePath, filename);
    szCurLogFile += GetLogFileExt();
    
    // �ӵ�ǰ���ڵ���־�ļ����ȡ��־���к�
    if (GetSNFromFile(szCurLogFile.c_str(), CurSN))
    {
        return CurSN;
    }

    // ��ȡ��־��Ŀ¼����־�ļ����б�
    LIST<STRING> LogFileList;
    if (GetLogFileList(LogFileList) != ERR_SUCCESS) 
    {
        return 0;
    }
    
    // ������־�ļ�������������
    LogFileList.sort();
    
    STRING szLogFile;

    LIST<STRING>::reverse_iterator riter = LogFileList.rbegin();
    while (riter != LogFileList.rend())
    {
        szLogFile = MakePath(m_szBasePath, (*riter).c_str());       
        //ǰ���ѶԵ�ǰ��־�ļ�����������ʧ�ܣ����ﲻ�ٶ����������
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
����ԭ��    protected: BOOL GetSNFromFile ( const char * szPathFile, 
                                            UINT4& uSN );
��������    ��һ����־�ļ����ȡ��־���к�
�������    ��־�ļ�·��
�������    ��־���к�
����ֵ      �ɹ��򷵻�TRUE�����򷵻�FALSE
�׳��쳣    ��
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

    //�ȶ�ȡָ����־�ļ���������DOM�����
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
����ԭ��    private: int GetLogFileList ( LIST<STRING>& LogFileList );
��������    ��ȡ��־��Ŀ¼��������־�ļ�
�������    ��
�������    ��־�ļ��б�
����ֵ      �ɹ��򷵻�ERR_SUCCESS������ERR_FAIL
�׳��쳣    ��
***************************************************************************/
int CLog::GetLogFileList ( LIST<STRING>& LogFileList )
{
    // ������־Ŀ¼�µ����е���־�ļ�����
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
        
        // �������з�".xml"�ļ���Ŀ¼
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
����ԭ��    protected: void ParseMsg ( MSG_INTRA* pMsgParsed )
��������    ���������ַ�
�������    ��Ϣָ��
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
void CLog::ParseMsg ( MSG_INTRA* pMsgParsed )
{
    switch ( pMsgParsed->CmdCode )
    {
        case CMD_WRITE_LOG:                     //д��־����
        {
            // ����Ϣ֡��Data����ȡ����Ҫ�������־����
            SLog *pData = (SLog *) &(pMsgParsed->Data[0]);
            // ����ð���־����
            WriteLog(pData);
            
            break;
        }    
        case CMD_MML:
        {
            // ����Ϣ֡��Data����ȡ��CParseResult����
            CParseResult *pParse = *(CParseResult **)pMsgParsed->Data;
            UINT2 MMLCmdCode = pParse->GetCmdCode();
            if ( MML_LST_LOG == MMLCmdCode)     //��־��ѯ����
            {
                QueryLog(pMsgParsed);           // ʵʩ��־��ѯ����
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
			//������ 2002-04-25 ���ⵥD12535
            //TRACE(THISMODULE, S_LOG_RECV_OTHER_MSG, pMsgParsed->CmdCode);
            
            break;
    }
}

void CLog::GetLogSavingDays(MSG_INTRA* pMsg)
{
    // ����Ϣ֡��Data����ȡ��CParseResult����
    CParseResult *pParse = *(CParseResult **)pMsg->Data;
    char szTmp[64];
    
    //����MML���ģ���ӦMML�ͻ���
    CMMLReport Report(THISMODULE);
    
    Report.MakeReportHead();            //���챨��ͷ
    //�����������
    Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, ERR_SUCCESS, THISMODULE);
    //���췵�ر�������
    SNPRINTF(szTmp,64,"%s  =  %d","days",m_uSavedDays);
    szTmp[63] = 0;
    Report.AddString(szTmp);
    //��������
    Report.AddString(STR_NEWLINE);
    //���챨��β 
    Report.MakeReportTail();
    
    SendMsgToMML(Report.GetReport(),pMsg);
}
/**************************************************************************
����ԭ��    protected: void QueryLog ( MSG_INTRA* pMsg )
��������    ������ѯҪ��ʵʩ��ѯ����
�������    ��Ϣָ��
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
void CLog::QueryLog ( MSG_INTRA* pMsg )
{
    // ��ŷ��ظ�MML�ͻ��˵ı�����Ϣ
    m_nNetStatus = 0;
    LIST <STRING> retMML;
    m_nQueryCount = 0;
    int nQueryResult = LOG_REP_MORE;
    //������ 2003-07-16 SWPD00397
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
    
    // ����Ϣ֡��Data����ȡ��CParseResult����
    CParseResult *pParse = *(CParseResult **)pMsg->Data;
    
    strncpy(log.szOperCmd,pParse->GetCmdName(),20);
    log.szOperCmd[LOG_OPER_CMD_LEN-1] = 0;
    strncpy(log.szCmdDesc,pParse->GetCmdDspName(),128);
    log.szCmdDesc[LOG_CMD_DESC_LEN-1] = 0;
    //�޸Ľ���
    // ��CParseResult��������ȡ��MML���������
    const char *szOperUser = pMsg->UserName;        // MML��ѯ�����û�
    int LogType = 0;
    BOOL bRet = pParse->GetDigitVal(&LogType, 2);   //��־����
    const char *szLogUser = pParse->GetStringVal(1);// ����ѯ�û�

    tm QStartDate;
    tm QDefault;
    memset(&QDefault,0,sizeof(tm));
    GetDateValue(pParse,3,&QStartDate,&QDefault);   // ��ѯ��ʼ����
    GetTimeValue(pParse,5,&QStartDate,&QDefault);   // ��ѯ��ʼʱ��
    tm QEndDate;
    GetDateValue(pParse,4,&QEndDate,&QDefault);     // ��ѯ��������
    GetTimeValue(pParse,6,&QEndDate,&QDefault);     // ��ѯ��ֹʱ��

    // ����ȥ��������ȷ��������ʱ�Ȳ�д
    // �ֱ�Ѳ�ѯ����ʼ�������ѯ��ʼʱ�䡢
    // ��ѯ�����������ѯ����ʱ����֯��tm�ṹ��
    // �ֱ�ͳ��Ϊ��ѯ��ʼʱ��Ͳ�ѯ����ʱ��
 
    time_t st,et;
    st = mktime(&QStartDate);
    et = mktime(&QEndDate);
    
    if ( difftime(et,st) < 0)
    {
        // ���ظ�MML�ı�����ϢΪ����ѯ����ʼʱ�䲻�ܴ��ڽ���ʱ�䡱
        SendErrorReport(S_LOG_TIME_NOT_MATCH,pMsg);
        log.OperResult = SLog::OPER_FAIL;
        WriteLog(&log);
    }
    else if(0 ==  ACE_OS::strcasecmp(szOperUser, ADMIN_USER))  // �����û�ΪAdmin
    {
        while(nQueryResult == LOG_REP_MORE && m_nNetStatus == 0)
        {
            if ( SLog::LOG_OPER == LogType )        // ��־����Ϊ������־
            {
                nQueryResult = QueryOperLog(&QStartDate, &QEndDate, szLogUser, retMML);
            }
            else if ( SLog::LOG_SYS == LogType )    // ��־����Ϊϵͳ��־
            {
                nQueryResult = QuerySysLog(&QStartDate, &QEndDate, retMML);
            }
            AssembleReport(nQueryResult,retMML,pMsg);
        }
        log.OperResult = SLog::OPER_SUCCESS;
        WriteLog(&log);
    }
    else      //�����û�����ͨ�û�
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
            // ���ظ�MML�ı�����ϢΪ������ѯ����־���ͷǷ���
            SendErrorReport(S_LOG_TYPE_INVALID,pMsg);
            log.OperResult = SLog::OPER_FAIL;
            WriteLog(&log);
        }
        else  //����ѯ���û��������ڲ���Ա�û���
        {
            // ���ظ�MML�ı�����ϢΪ������ѯ���û����Ƿ���
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
    // ��װMML���Ĳ����͸�MML�ͻ���
    if(m_nQueryCount == 0) //���û�в�ѯ����Ӧ���ʧ�ܱ���
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
����ԭ��    protected: UINT4 QuerySysLog ( ACE_Time_Value* pStartTime, 
                                           ACE_Time_Value* pEndTime, 
                                           LIST<STRING>& qResult );
��������    ϵͳ��־��ѯ����
�������    ��־��ѯ��ֹʱ���
�������    MML�����б�
����ֵ      LOG_REP_MORE����ʾ���б���
            LOG_REP_NONE����ʾ���һҳ����
            LOG_REP_ERR����ʾ����
�׳��쳣    ��
***************************************************************************/
UINT4 CLog::QuerySysLog ( tm *pStartTime, 
                          tm *pEndTime, 
                          LIST <STRING> &qResult)
{    
    return QueryLog_impl(pStartTime,pEndTime,qResult);
}
/**************************************************************************
����ԭ��    protected: UINT4 QueryOperLog ( ACE_Time_Value* pStartTime,
                                            ACE_Time_Value* pEndTime, 
                                            char *pUserName,
                                            LIST<STRING>& qResult );
��������    ������־��ѯ����
�������    ��־��ѯ��ֹʱ��Σ�����ѯ���û���
�������    MML�����б�
����ֵ      LOG_REP_MORE����ʾ���б���
            LOG_REP_NONE����ʾ���һҳ����
            LOG_REP_ERR����ʾ����
�׳��쳣    ��
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
    // ���Ĵ�С������
    long LenOfMML = 0;
    // ���һ��MML���ĵĻ�����
    char buf[MAX_LOG_LEN];
    buf[0] = '\0';
    time_t tStartTime = mktime(pStartTime);
    time_t tEndTime = mktime(pEndTime);
    
    qResult.clear();//ensure result list is empty
    if(m_bFirst)
    {              
        // ��ʼ��־�ļ���
        char filename[9];
        sprintf(filename, "%04d%02d%02d", pStartTime->tm_year+1900,
            pStartTime->tm_mon+1, pStartTime->tm_mday);
        filename[8] = '\0';
        STRING StartLogFile(filename);
        StartLogFile += GetLogFileExt();
        
        // ������־�ļ���
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
            // ����ѯ���ڿ�ʼ�ļ����ƺͽ����ļ�����֮�����־�ļ�
            if ( (*iter < StartLogFile) || (*iter > EndLogFile) )
            {
                iter = m_LogFileList.erase(iter);
                continue;
            }
            iter++;
        }
        
        // ������־�ļ���������
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

            //��־�ļ���
            szLogFile = MakePath(m_szBasePath, m_LogFileList.front().c_str());

            //�ȶ�ȡָ����־�ļ���������DOM�����
	        if (m_pQueryLogUtil->OpenFile(szLogFile.c_str()) != 0)
	        {
                TRACE(THISMODULE, S_LOG_OPEN_LOG_FAIL, szLogFile.c_str());
                delete m_pQueryLogUtil;
                m_pQueryLogUtil = NULL;
		        continue;
	        }
        }
	    
		//������־��¼�����ҷ���Ҫ��ļ�¼
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
                if(SLog::LOG_SYS != log.LogType )   //������־��Ϊϵͳ��־
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
                if(SLog::LOG_OPER != log.LogType )  //��־��Ϊ������־
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
                // ����ѯ���û�����Ϊ��
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
����ԭ��    private: int PrintLog(char *buf, SLog *pLog);
��������    ��ӡ��־��Ϣ
�������    ��־��Ϣ��������һ����־��¼����־�����ʽ����MML���ĸ�ʽ������
-------------------------------------------------------------------------------------
���к� ��־���� �û��� �����ص�   ����       �������   ��������    ������Դ    
  1     ����    Admin   IP��ַ   2001-08-03      �ɹ�   LST ALMLOG     -      
-------------------------------------------------------------------------------------
���к�  �û���  ����         �����ص�      ��־��Դ      �������� ������ϸ����   �������  ��־���� 
1        Admin  2001-08-03    -               -          LST LOG   ��ѯ��־      �ɹ�      ����
�������   ��
����ֵ     ��־��Ϣ����
�׳��쳣   ��
***************************************************************************/
int CLog::PrintLog(char *buf, SLog *pLog)
{
    char temp[24];
    int len = 0;
    //���к� 
    len += SNPRINTF(buf + len, MAX_LOG_LEN - len, "  %-9d", pLog->uSerialNo);

    //�û���
    if (pLog->szUserName[0] == '\0' || SLog::LOG_SYS == pLog->LogType)
    {
        len += SNPRINTF(buf + len, MAX_LOG_LEN - len, "%-50.48s", "-");
    }
    else
    {
		StringToLower(pLog->szUserName);
        len += SNPRINTF(buf + len, MAX_LOG_LEN - len, "%-50.48s", pLog->szUserName);
    }

    //����
    ACE_OS::strftime(temp, 24, "%Y-%m-%d %H:%M:%S", ACE_OS::localtime(&pLog->tLogTime));
    len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-24.22s",temp);

    //�����ص�
    struct in_addr Addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
    Addr.s_addr = pLog->uIPAddr;
#else 
    Addr.S_un.S_addr = pLog->uIPAddr;
#endif//_PLATFORM_HPUX
    len += SNPRINTF(buf + len, MAX_LOG_LEN - len, "%-20.18s",  ACE_OS::inet_ntoa(Addr));

    
    //��־��Դ
    if (pLog->szUserName[0] == '\0' || SLog::LOG_OPER == pLog->LogType)
    {
        len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-50.48s", "-");
    }
    else
    {
        len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-50.48s", pLog->szUserName);
    }
	
    //��������
    if (pLog->szOperCmd[0] == '\0')
    {
        len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-22.20s","-");
    }
    else
    {
        len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-22.20s",pLog->szOperCmd);
    }
    
    //����������ϸ����
    if (pLog->szCmdDesc[0] == '\0')
    {
        len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-256.254s","-");
    }
    else
    {
        len += SNPRINTF(buf + len, MAX_LOG_LEN - len,"%-256.254s",pLog->szCmdDesc);
    }

    //�������
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
         
    //��־����
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
����ԭ��    protected: BOOL DeleteOutdatedLog ( void );
��������    ɾ��ָ������ǰ����־
�������    ��
�������    ��
����ֵ      �ɹ��򷵻�TRUE������FALSE
�׳��쳣    ��
***************************************************************************/
BOOL CLog::DeleteOutdatedLog ( void )
{
    STRING szLogFile;
    int nRet;
    // ��ȡ��ǰʱ��
    time_t curtime,tFileTime;
    time(&curtime);
    //������ 2002-04-25 ���ⵥ12476 ɾ��ԭ���Ĵ���    
    // ��ȡ��־��Ŀ¼����־�ļ����б�
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
            //û��.log
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
        {//�޸Ľ���
            // ɾ������־�ļ�
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
����ԭ��    protected: int SendMsgToMML(char *pszMMLReport,MSG_INTRA *pFrame)
��������    ����MML���ĵ��ͻ���
�������    ��
�������    ��
����ֵ      �ɹ��򷵻�0���������
�׳��쳣    ��
***************************************************************************/
int CLog::SendMsgToMML(const char *pszMMLReport, MSG_INTRA *pFrame)
{
    // ����MSG_INTRA��Ϣͷ
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
    //�����ݲ���
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
    //����·ݲ���
    pcTmp = pParaResult->GetStringVal(paraID,2);
    if( pcTmp != NULL)
    {
        pDate->tm_mon = atoi(pcTmp) - 1;
    }
    else
    {
        pDate->tm_mon = pDefault->tm_mon;
    }
    //������ڲ���
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
    //���hour����
    
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
    //���min����
    pcTmp = pParaResult->GetStringVal(paraID,2);
    if( pcTmp != NULL)
    {
        pDate->tm_min = atoi(pcTmp);
    }
    else
    {
        pDate->tm_min = pDefault->tm_min;
    }
    //���sec����
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
	// ����Ϣ֡��Data����ȡ��CParseResult����
    CParseResult *pParse = *(CParseResult **)pMsg->Data;
    
	int nSavingDays = 30;
    // ��CParseResult��������ȡ��MML���������
    BOOL bRet = pParse->GetDigitVal(&nSavingDays, 1);   //��־����
    //ʡ���˴�����
    CINIFile ConfigFile(GetCfgFilePath());
    ConfigFile.Open();
	m_uSavedDays = nSavingDays;
    ConfigFile.WriteInt(CFG_SEC_LOG,
                        CFG_LOG_SAVE_DAY, 
                        m_uSavedDays);
    ConfigFile.Close();

	//д��־
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

	//����MML���ģ���ӦMML�ͻ���
    CMMLReport Report(THISMODULE);

    Report.MakeReportHead();            //���챨��ͷ
    //�����������
    Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, ERR_SUCCESS, THISMODULE);
    //���췵�ر�������
    Report.AddString(STR_OPRSUCCESS);
    //��������
    Report.AddString(STR_NEWLINE);
    //���챨��β 
    Report.MakeReportTail();
               
    SendMsgToMML(Report.GetReport(),pMsg);
}

void CLog::RemoveLogByClient(MSG_INTRA* pMsg)
{
	// ����Ϣ֡��Data����ȡ��CParseResult����
    CParseResult *pParse = *(CParseResult **)pMsg->Data;

    tm sd;	//ɾ������ʼ����
	tm ed;	//ɾ������ֹ����
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
	   // ��ʼ��־�ļ���
		char szFileName[9];
		sprintf(szFileName, "%04d%02d%02d", sd.tm_year+1900, sd.tm_mon+1, sd.tm_mday);
		szFileName[8] = '\0';
		STRING StartLogFile(szFileName);
		StartLogFile += GetLogFileExt();
    
		// ������־�ļ���
		sprintf(szFileName, "%04d%02d%02d", ed.tm_year+1900, ed.tm_mon+1, ed.tm_mday);
		szFileName[8] = '\0';
		STRING EndLogFile(szFileName);
		EndLogFile += GetLogFileExt();
        char szTempFilePath[MAX_PATH];
		// ��ȡ��־��Ŀ¼����־�ļ����б�
		LIST<STRING> LogFileList;
		if (GetLogFileList(LogFileList) != ERR_FAIL)
		{       
			LIST<STRING>::iterator iter = LogFileList.begin();    
			while(iter != LogFileList.end())
			{
				// ����ѯ���ڿ�ʼ�ļ����ƺͽ����ļ�����֮�����־�ļ�
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

	// ��MML�ͻ�����Ӧ����
	//д��־
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

    Report.MakeReportHead();            //���챨��ͷ

    //�����������

	Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, nRet, THISMODULE);
	if (ERR_SUCCESS == nRet)
	{
		//���췵�ر�������
		Report.AddString(STR_OPRSUCCESS);
	}
	else
	{
		//���췵�ر�������
		Report.AddString(STR_OPRFAIL);
	}

    //��������
    Report.AddString(STR_NEWLINE);
    //���챨��β 
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
        //��ʼ�鵵��־
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

    //��ʹ��ͬ���ļ�tar�ǻḲ�Ǻ󷵻سɹ���gzip��ͬ
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
        else //ɾ��ԭ�е�.gz����ѹ��һ��
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

    //д��־
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

    Report.MakeReportHead();            //���챨��ͷ

    //�����������

	Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, nRet, THISMODULE);
	if (ERR_SUCCESS == nRet)
	{
		//���췵�ر�������
		Report.AddString(STR_OPRSUCCESS);
	}
	else
	{
		//���췵�ر�������
		Report.AddString(STR_OPRFAIL);
	}

    //��������
    Report.AddString(STR_NEWLINE);
    //���챨��β 
    Report.MakeReportTail();
               
    SendMsgToMML(Report.GetReport(),pMsg);
}

void CLog::SetLogArchiveTime(MSG_INTRA* pMsg)
{
	// ����Ϣ֡��Data����ȡ��CParseResult����
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

    // �������ļ�
    CINIFile ConfigFile(GetCfgFilePath());

	if (TRUE == ConfigFile.Open())
	{
		ConfigFile.WriteString(CFG_SEC_LOG,CFG_LOG_ARCTIME,szTime);    
       
        // �ر������ļ�
        ConfigFile.Close();

		nRet = ERR_SUCCESS;
    }
	else
	{
		nRet = ERR_FAIL;
	}

	//д��־
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

    Report.MakeReportHead();            //���챨��ͷ

    //�����������
	Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, nRet, THISMODULE);
	if (ERR_SUCCESS == nRet)
	{
		//���췵�ر�������
		Report.AddString(STR_OPRSUCCESS);
	}
	else
	{
		//���췵�ر�������
		Report.AddString(STR_OPRFAIL);
	}

    //��������
    Report.AddString(STR_NEWLINE);
    //���챨��β 
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
        // ɾ�����ڵ���־�ļ�
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
        //���һ���ļ�����д״̬����������
        if(LogFileList.size() > 0)
        {
            LogFileList.pop_back();
        }
        
        //��ʼ�鵵��־
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


    //��ʹ��ͬ���ļ�tar�ǻḲ�Ǻ󷵻سɹ�,��gzip��ͬ
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
        else //ɾ��ԭ�е�.gz����ѹ��һ��
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

	//д��־
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
    // ����Ϣ֡��Data����ȡ��CParseResult����
    CParseResult *pParse = *(CParseResult **)pMsg->Data;
    char szTmp[64];
    
    //����MML���ģ���ӦMML�ͻ���
    CMMLReport Report(THISMODULE);
    
    Report.MakeReportHead();            //���챨��ͷ
    //�����������
    Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, ERR_SUCCESS, THISMODULE);
    //���췵�ر�������
    char szArcTime[64];
    strftime(szArcTime,64,"%H:%M:%S",&m_tArchive);
    SNPRINTF(szTmp,64,"%s  =  %s","archive time",szArcTime);
    szTmp[63] = 0;
    Report.AddString(szTmp);
    //��������
    Report.AddString(STR_NEWLINE);
    //���챨��β 
    Report.MakeReportTail();
    
    SendMsgToMML(Report.GetReport(),pMsg);
}

//������־
void CLog::ExportLog(MSG_INTRA* pMsg)
{
    CParseResult* pParse = *((CParseResult**)pMsg->Data);

    //�����������

    //��ʼʱ��
    tm tSTime;
    memset(&tSTime,0,sizeof(tm));
    const char *pcTmp;
    
     //��ȡ���
    pcTmp = pParse->GetStringVal(1,1);
    if (pcTmp != NULL)
    {
        tSTime.tm_year = atoi(pcTmp);
        if (tSTime.tm_year > 1900)
        {
            tSTime.tm_year = tSTime.tm_year - 1900;
        }
    }    

    //��ȡ�·�
    pcTmp = pParse->GetStringVal(1,2);    
    if (pcTmp != NULL)
    {
        tSTime.tm_mon = atoi(pcTmp);
        if (tSTime.tm_mon > 1)
        {
            tSTime.tm_mon = tSTime.tm_mon - 1;
        }
    }    
    
    //��ȡ����
    pcTmp = pParse->GetStringVal(1,3);
    if (pcTmp != NULL)
    {
        tSTime.tm_mday = atoi(pcTmp);
    }    
      
    //��ȡСʱ
    pcTmp = pParse->GetStringVal(3,1);
    if (pcTmp != NULL)
    {
        tSTime.tm_hour = atoi(pcTmp);
    }    

    //��ȡ����
    pcTmp = pParse->GetStringVal(3,2);
    if (pcTmp != NULL)
    {
        tSTime.tm_min = atoi(pcTmp);
    }    

    //��ȡ��
    pcTmp = pParse->GetStringVal(3,3);
    if (pcTmp != NULL)
    {
        tSTime.tm_sec = atoi(pcTmp);
    }    

    //����ʱ��   
    // ���tEimeû��ȡֵ����ȡ2038��       
    //mktime handles dates in any time zone from midnight, January 1, 1970, to January 18, 19:14:07, 2038. 
    tm    tETime;
    memset(&tETime,0,sizeof(tm));
    //��ȡ��� 
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
    
    //��ȡ�·�
    pcTmp = pParse->GetStringVal(2,2);    
    if (pcTmp != NULL)
    {
        tETime.tm_mon = atoi(pcTmp);
        if (tETime.tm_mon > 1)
        {
            tETime.tm_mon = tETime.tm_mon - 1;
        }
    }    
    
    //��ȡ����
    pcTmp = pParse->GetStringVal(2,3);
    if (pcTmp != NULL)
    {
        tETime.tm_mday = atoi(pcTmp);
    }    
    
    //��ȡСʱ
    pcTmp = pParse->GetStringVal(4,1);
    if (pcTmp != NULL)
    {
        tETime.tm_hour = atoi(pcTmp);
    }    
    
    //��ȡ����
    pcTmp = pParse->GetStringVal(4,2);
    if (pcTmp != NULL)
    {
        tETime.tm_min = atoi(pcTmp);
    }    
    
    //��ȡ��
    pcTmp = pParse->GetStringVal(4,3);
    if (pcTmp != NULL)
    {
        tETime.tm_sec = atoi(pcTmp);
    }
    
    //��ȡ��־����
    int    nLogType = 1; 
    pParse->GetDigitVal(&nLogType,5);

    //��ȡ�ļ����� 
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
        // ���ظ�MML�ı�����ϢΪ����ѯ����ʼʱ�䲻�ܴ��ڽ���ʱ�䡱
        SendSimpleReport(pParse->GetCmdLine(),nErrCode,"Query Time error",pMsg); 
        return;
    }
    
    //�����ʼʱ��ȱʡ,��Ϊ��һ����־��ʱ��
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

        // ������־�ļ���������
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
    
    //�������ʱ��ȱʡ����Ϊ���һ����־��ʱ��
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
        
        // ������־�ļ���������
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

    //��ѯ��־
    BOOL bCreateFile = TRUE;

    //��ѯ��������������־
    STRING szLogFile;
    SLog Log;
    FILE* fpLog = NULL;
    
    // ��ʼ��־�ļ���
    char filename[9];
    sprintf(filename, "%04d%02d%02d", tSTime.tm_year+1900,
        tSTime.tm_mon+1, tSTime.tm_mday);
    filename[8] = '\0';
    STRING StartLogFile(filename);
    StartLogFile += GetLogFileExt();
    
    // ������־�ļ���
    sprintf(filename, "%04d%02d%02d", tETime.tm_year+1900,
        tETime.tm_mon+1,tETime.tm_mday);
    filename[8] = '\0';
    STRING EndLogFile(filename);
    EndLogFile += GetLogFileExt();

    // ��ȡ��־��Ŀ¼����־�ļ����б�
    LIST<STRING> LogFileList;
    if (ERR_SUCCESS != GetLogFileList(LogFileList))
    {
        SendSimpleReport(pParse->GetCmdLine(),nErrCode,"Get Log File Failed",pMsg);
        return;
    }
    
    LIST<STRING>::iterator iter = LogFileList.begin();    
    while(iter != LogFileList.end())
    {
        // ����ѯ���ڿ�ʼ�ļ����ƺͽ����ļ�����֮�����־�ļ�
        if ( (*iter < StartLogFile) || (*iter > EndLogFile) )
        {
            iter = LogFileList.erase(iter);
            continue;
        }
        iter++;
    }

    // ������־�ļ���������
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
                //����һ��.xml����־�ļ�
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
                //����һ��.xml����־�ļ�
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
    
    //���ر���
    //SendSimpleReport(pParse->GetCmdLine(),nErrCode,"Success",pMsg);

    SendSimpleReport(pParse->GetCmdLine(),nErrCode,"Session ID = 1",pMsg);
	//by ldf 2004-08-26 OM-M2000����
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
	
	//���͵��ͻ���
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

//�ϴ���־
void CLog::UploadLog(MSG_INTRA* pMsg)
{
    CParseResult* pParse = *((CParseResult**)pMsg->Data);
    
    //�����������
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
    
    //�ϴ���־
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
    
    //������Ҫ���ص�ԭ�ļ���
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
    
    //���ر���
    //SendSimpleReport(pParse->GetCmdLine(),0,"Success",pMsg);
    SendSimpleReport(pParse->GetCmdLine(),0,"Session ID = 1",pMsg);
	
    //by ldf 2004-08-26 OM-M2000����
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
	
	//���͵��ͻ���
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
