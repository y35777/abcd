/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : backup_server.cpp
  版 本 号   : 初稿
  作    者   : 张玉新
  生成日期   : 2002年9月8日
  最近修改   :
  功能描述   : 备份模块的实现
  函数列表   :
  修改历史   :
  1.日    期   : 2002年9月8日
    作    者   : 张玉新
    修改内容   : 创建文件

******************************************************************************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
#define THISMODULE "BACKUP"
/****** End For PC-LINT ******/
#include "backup_server.h"
#include "manual_task.h"
/*****************************************************************************
 函 数 名  : 构造函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2002年9月8日
    作    者   : 张玉新
    修改内容   : 新生成函数

*****************************************************************************/
CBackupServer::CBackupServer()
{
    m_pMMLReport = NULL;
    m_pBakTaskMgr = NULL;
}
/*****************************************************************************
 函 数 名  : InitInstance
 功能描述  : 资源初始化
 输入参数  : 无
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2002年9月8日
    作    者   : 张玉新
    修改内容   : 新生成函数

*****************************************************************************/
int CBackupServer::InitInstance()
{
    int nResult = CWorkModule::InitInstance();
    if (0 != nResult)
    {
        return nResult;
    }

    CINIFile IniFile(GetCfgFilePath());
    BOOL bSucc = IniFile.Open();
    if (!bSucc)
    {
        TRACE(MTS_BILLBROWSER, S_BACKUP_INI_OPEN_FAIL);
        return ERR_FAIL;
    }
    
    IniFile.GetString(CFG_SEC_DISK_FILE, CFG_FRONT_SAVE_ROOT_DIR,
                         CFG_FRONT_SAVE_ROOT_DIR_DEFAULT,
                         m_szOrigBasePath, MAX_PATH);
    IniFile.GetString(CFG_SEC_DISK_FILE, CFG_BACK_SAVE_ROOT_DIR,
                         CFG_BACK_SAVE_ROOT_DIR_DEFAULT,
                         m_szFinalBasePath, MAX_PATH);

   	//创建m_pBakTaskMgr指针
    m_pBakTaskMgr = new CBakTaskMgr(this);
    
	if(m_pBakTaskMgr->open())
	{
		//即使失败也可使OM正常启动，不备份而已
		TRACE(MTS_BACKUP,S_BACKUP_START_FAIL);
	}
    
    m_pMMLReport = new CMMLReport("Backup");

	return ERR_SUCCESS;
}
/*****************************************************************************
 函 数 名  : ExitInstance
 功能描述  : 退出函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2002年9月8日
    作    者   : 张玉新
    修改内容   : 新生成函数

*****************************************************************************/
void CBackupServer::ExitInstance()
{
    delete m_pMMLReport, m_pMMLReport = NULL;
	if(m_pBakTaskMgr)
		m_pBakTaskMgr->Exit();
	delete m_pBakTaskMgr, m_pBakTaskMgr = NULL;

    CWorkModule::ExitInstance();
}
/*****************************************************************************
 函 数 名  : GetModuleType
 功能描述  : 返回模块类型
 输入参数  : 无
 输出参数  : 无
 返 回 值  : MODULE_TYPE
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2002年9月8日
    作    者   : 张玉新
    修改内容   : 新生成函数

*****************************************************************************/
MODULE_TYPE CBackupServer::GetModuleType()
{
    return MT_BACKUP;
}
/*****************************************************************************
 函 数 名  : ParseMsg
 功能描述  : 处理内部消息函数
 输入参数  : MSG_INTRA *pMsg
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2004年5月20日
    作    者   : Zou Yongsheng
    修改内容   : 

*****************************************************************************/
void CBackupServer::ParseMsg(MSG_INTRA *pMsg)
{
    CParseResult* pResult = NULL;
    
    switch (pMsg->CmdCode)
    {
    case CMD_MML:
        pResult = *((CParseResult**)(pMsg->Data));
        switch(pResult->GetCmdCode())
        {
        case MML_BAK_CDR:
            ReqBakCdrProc(pMsg);
            break;
        case MML_DEL_CDR:
            ReqDelCdrProc(pMsg);
            break;
        case MML_BKP_CFGFILE:
            BkpCfgFile(pMsg);
            break;
        case MML_ULD_CFGFILE:
            UldCfgFile(pMsg);
            break;
		case MML_DLD_CFGFILE:
			DldCfgFile(pMsg);
			break;
		case MML_RTR_CFGFILE:
			RtrCfgFile(pMsg);            
        default:
            break;
        }
        break;
    default:
        CWorkModule::ParseMsg(pMsg);
        break;
    }
}


/* om modify add by yangguang ---begin*/

void CBackupServer::SendMMLReport(const char* szReport, int nClientNo)
{
    int nStrLen = strlen(szReport);
	
    MSG_INTRA* pMsg = new(nStrLen + 1) MSG_INTRA;
    pMsg->SenderPid = PT_OM;
    pMsg->SenderMid = MT_BACKUP;
    pMsg->ReceiverPid = PT_OM;
    pMsg->ReceiverMid = MT_MMLDEBUG;
    pMsg->AppType = APP_MML_NORM;
    pMsg->ClientNo = nClientNo;
	
    memcpy(pMsg->Data, szReport, nStrLen + 1);
    SendMsg(pMsg);
}

void CBackupServer::SendSimpleReport(const char* szCmd, int nRetCode,
                                     const char* szInfo,int nClientNo)
{
    m_pMMLReport->MakeReportHead();
    m_pMMLReport->MakeCmdDisplay(szCmd, 10640, nRetCode, "O&M");
	m_pMMLReport->AddString("\r\n");
    m_pMMLReport->AddString(szInfo);
    m_pMMLReport->AddString("\r\n");
	m_pMMLReport->AddString("\r\n");

	m_pMMLReport->AddString("");
    m_pMMLReport->MakeReportTail();

    const char* szReport = m_pMMLReport->GetReport();	
    SendMMLReport(szReport, nClientNo);
}


/*
[BKP CFGFILE]
macro_code = 155
macro_name = 备份配置文件
cmd_level = CL_ADMIN
*/
void CBackupServer::BkpCfgFile(MSG_INTRA* pMsg)
{
    CParseResult* pParseResult = *((CParseResult**)(pMsg->Data));
	
    char szIGWBPath[MAX_PATH];
    sprintf(szIGWBPath, "nohup tar cvf config.tar %s/%s &", GetAppPath(),"config");
#ifdef _PLATFORM_WIN32
	WriteLog(MTS_BACKUP, "this function can not run on win32!");
	SendSimpleReport(pParseResult->GetCmdLine(), 1, "Session ID = 1", pMsg->ClientNo);
#else 
	system1(szIGWBPath);
	WriteLog(MTS_BACKUP, "backup config file successed!");
	SendSimpleReport(pParseResult->GetCmdLine(), 0, "Session ID = 1", pMsg->ClientNo);	
#endif //_PLATFORM_WIN32

	//by ldf 2004-08-17 OM-M2000联调
    CMMLReport MMLReport("HUAWEI");
    MMLReport.MakeReportHead();
    MMLReport.MakeCmdDisplay("BKP CFGFILE:",0,0,"O&M");
    MMLReport.AddString(STR_NEWLINE);
    MMLReport.MakeReportItem("Report Type","Backup",24,ALIGN_RIGHT,6,ALIGN_LEFT,1);
#ifdef _PLATFORM_WIN32
    MMLReport.MakeReportItem("Status","Failure",24,ALIGN_RIGHT,7,ALIGN_LEFT,1);    
#else 
    MMLReport.MakeReportItem("Status","Success",24,ALIGN_RIGHT,7,ALIGN_LEFT,1);    
#endif //_PLATFORM_WIN32
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

/*
[RTR CFGFILE]
macro_code = 158
macro_name = 恢复配置文件
cmd_level = CL_ADMIN 
*/
void CBackupServer::RtrCfgFile(MSG_INTRA* pMsg)
{
    CParseResult* pParseResult = *((CParseResult**)(pMsg->Data));
	
    char szIGWBPath[MAX_PATH];
    sprintf(szIGWBPath, "nohup tar xvf %s/config.tar &", GetAppPath());
#ifdef _PLATFORM_WIN32
	WriteLog(MTS_BACKUP, "this function can not run on win32!");
	SendSimpleReport(pParseResult->GetCmdLine(), 1, "Session ID = 1", pMsg->ClientNo);	
#else 
	system1(szIGWBPath);
	WriteLog(MTS_BACKUP, "restore config file successed!");
	SendSimpleReport(pParseResult->GetCmdLine(), 0, "Session ID = 1", pMsg->ClientNo);	
#endif //_PLATFORM_WIN32

	//by ldf 2004-08-17 OM-M2000联调
    CMMLReport MMLReport("HUAWEI");
    MMLReport.MakeReportHead();
    MMLReport.MakeCmdDisplay("RTR CFGFILE:",0,0,"O&M");
    MMLReport.AddString(STR_NEWLINE);
    MMLReport.MakeReportItem("Report Type","Restore",24,ALIGN_RIGHT,7,ALIGN_LEFT,1);
#ifdef _PLATFORM_WIN32
    MMLReport.MakeReportItem("Status","Failure",24,ALIGN_RIGHT,7,ALIGN_LEFT,1);    
#else 
    MMLReport.MakeReportItem("Status","Success",24,ALIGN_RIGHT,7,ALIGN_LEFT,1);    
#endif //_PLATFORM_WIN32
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

/*
[ULD CFGFILE]
macro_code=156
macro_name=上载配置文件
para_num=5
1=IP (FTP server IP)
2=USR (FTP SERVER USERNAME)
3=PWD (FTP SERVER PASSWORD)
4=DIR (FTP SERVER DIRECTORY)
5=FT (备份文件类型) {0~1}
para1_datatype=2
para2_datatype=2
para3_datatype=2
para4_datatype=2
para5_datatype=0
cmd_level = CL_ADMIN
*/
void CBackupServer::UldCfgFile(MSG_INTRA* pMsg)
{
	CParseResult* pParseResult = *((CParseResult**)(pMsg->Data));

	STRING s_ip  = (const char* )pParseResult->GetStringVal(1);
	STRING s_usr = (const char* )pParseResult->GetStringVal(2); 
	STRING s_pwd =  (const char* )pParseResult->GetStringVal(3);
	STRING s_dir =  (const char* )pParseResult->GetStringVal(4);

	char szDestHostIp[MAX_HOST_LEN];

    CINIFile ini(GetCfgFilePath());
    if(ini.Open() != TRUE)
    {
        TRACE(MTS_BACKUP,S_BACKUP_INI_OPEN_FAIL);
        return;
    }

    ini.GetString(CFG_SEC_MML,CFG_MML_KEY_LOCAL_IP,
				  "127.0.0.1",  szDestHostIp,
				  MAX_HOST_LEN);

	CFtpClient m_FtpClient;
	if (m_FtpClient.Connect(s_ip.c_str(), s_usr.c_str(), s_pwd.c_str(), szDestHostIp) != 0)
	{
		SendSimpleReport(pParseResult->GetCmdLine(), 1, "connect ftp server failed!", pMsg->ClientNo);
		return;
	}
	WriteLog(MTS_BACKUP,"connect ftp server successed!");
	if (m_FtpClient.SetDirectory(s_dir.c_str()) != 0)
	{
		if (m_FtpClient.MakeDirectory(s_dir.c_str()) != 0)
		{
			SendSimpleReport(pParseResult->GetCmdLine(), 1, "make directory failed!", pMsg->ClientNo);
			return;
		}
		m_FtpClient.SetDirectory(s_dir.c_str());
	}

	WriteLog(MTS_BACKUP,"set dir for ftp server successed!");
	
	if (m_FtpClient.PutFile("config.tar") != 0)
	{
		SendSimpleReport(pParseResult->GetCmdLine(), 1, "put file failed!", pMsg->ClientNo);
		return;
	}

	WriteLog(MTS_BACKUP,"put file to ftp server successed!");
	m_FtpClient.Disconnect();
	SendSimpleReport(pParseResult->GetCmdLine(), 0, "Session ID = 1", pMsg->ClientNo);

	//by ldf 2004-08-17 OM-M2000联调
    CMMLReport MMLReport("HUAWEI");
    MMLReport.MakeReportHead();
    MMLReport.MakeCmdDisplay("ULD CFGFILE:",0,0,"O&M");
    MMLReport.AddString(STR_NEWLINE);
    MMLReport.MakeReportItem("Report Type","Upload",24,ALIGN_RIGHT,6,ALIGN_LEFT,1);
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

/*
[DLD CFGFILE]
macro_code=157
macro_name=下载配置文件
para_num=5
1=IP (FTP server IP)
2=USR (FTP SERVER USERNAME)
3=PWD (FTP SERVER PASSWORD)
4=DIR (FTP SERVER DIRECTORY)
5=FT (备份文件类型) {0~1}
para1_datatype=2
para2_datatype=2
para3_datatype=2
para4_datatype=2
para5_datatype=0
cmd_level = CL_ADMIN
*/
void CBackupServer::DldCfgFile(MSG_INTRA* pMsg)
{
	CParseResult* pParseResult = *((CParseResult**)(pMsg->Data));
	
	STRING s_ip  = (const char* )pParseResult->GetStringVal(1);
	STRING s_usr = (const char* )pParseResult->GetStringVal(2); 
	STRING s_pwd =  (const char* )pParseResult->GetStringVal(3);
	STRING s_dir =  (const char* )pParseResult->GetStringVal(4);

	char szDestHostIP[MAX_HOST_LEN];

    CINIFile ini(GetCfgFilePath());
    if(ini.Open() != TRUE)
    {
        TRACE(MTS_BACKUP,S_BACKUP_INI_OPEN_FAIL);
        return;
    }
	
    ini.GetString(CFG_SEC_MML,CFG_MML_KEY_LOCAL_IP,
		"127.0.0.1",  szDestHostIP,
		MAX_HOST_LEN);
	
	CFtpClient m_FtpClient;
	if (m_FtpClient.Connect(s_ip.c_str(), s_usr.c_str(), s_pwd.c_str(), szDestHostIP) != 0)
	{
		SendSimpleReport(pParseResult->GetCmdLine(), 1, "connect ftp server failed!", pMsg->ClientNo);
		return;
	}
	if (m_FtpClient.SetDirectory(s_dir.c_str()) != 0)
	{
		if (m_FtpClient.MakeDirectory(s_dir.c_str()) != 0)
		{
			SendSimpleReport(pParseResult->GetCmdLine(), 1, "make directory failed!", pMsg->ClientNo);
			return;
		}
		m_FtpClient.SetDirectory(s_dir.c_str());
	}
	if (m_FtpClient.GetFile("config.tar") != 0)
	{
		SendSimpleReport(pParseResult->GetCmdLine(), 1, "make directory failed!", pMsg->ClientNo);
		return;
	}
	m_FtpClient.Disconnect();
	SendSimpleReport(pParseResult->GetCmdLine(), 0, "Session ID = 1", pMsg->ClientNo);

	//by ldf 2004-08-17 OM-M2000联调
    CMMLReport MMLReport("HUAWEI");
    MMLReport.MakeReportHead();
    MMLReport.MakeCmdDisplay("DLD CFGFILE:",0,0,"O&M");
    MMLReport.AddString(STR_NEWLINE);
    MMLReport.MakeReportItem("Report Type","Download",24,ALIGN_RIGHT,8,ALIGN_LEFT,1);
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

/* om modify add by yangguang ---end*/



/*****************************************************************************
 函 数 名  : ReqModifyBackupProc
 功能描述  : 修改备份属性
 输入参数  : MSG_INTRA *pMsg
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2002年9月8日
    作    者   : 张玉新
    修改内容   : 新生成函数

*****************************************************************************/
void CBackupServer::ReqModifyBackupProc(MSG_INTRA *pMsg)
{
    
}
/*****************************************************************************
 函 数 名  : ReqQueryBackupProc
 功能描述  : 查询备份属性
 输入参数  : MSG_INTRA *pMsg
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2002年9月8日
    作    者   : 张玉新
    修改内容   : 新生成函数

*****************************************************************************/
void CBackupServer::ReqQueryBackupProc(MSG_INTRA *pMsg)
{
    
}
/*****************************************************************************
 函 数 名  : OnTimer
 功能描述  : 定时器函数
 输入参数  : const long nTimerID
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2002年9月8日
    作    者   : 张玉新
    修改内容   : 新生成函数

*****************************************************************************/
void CBackupServer::OnTimer(const long nTimerID)
{
    CWorkModule::OnTimer(nTimerID);
}

/*****************************************************************************
 函 数 名  : CBakTaskMgr
 功能描述  : 构造函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  :  
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2002年9月8日
    作    者   : 张玉新
    修改内容   : 新生成函数

*****************************************************************************/
CBakTaskMgr::CBakTaskMgr(CBackupServer* pBackupServer)
{
    //初始化成员变量
	m_ExitEvent.reset();                    //退出事件
    //m_BakMutex.release();                   //线程互斥对象
	m_pbBakFatalFlag = NULL;                //备份故障标志

	m_pBackupServer = pBackupServer;
    m_pManBakStatus = NULL;
}
/*****************************************************************************
 函 数 名  : ~CBakTaskMgr
 功能描述  : 析构函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  :  
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2002年9月8日
    作    者   : 张玉新
    修改内容   : 新生成函数

*****************************************************************************/
CBakTaskMgr::~CBakTaskMgr()
{
   //Exit();
    if(m_pManBakStatus != NULL)
    {
        delete m_pManBakStatus;
    }
}

int CBakTaskMgr::GetLicenseCount()
{
    if(m_pBackupServer)
        return m_pBackupServer->GetLicenseCount();

    return m_pBackupServer->GetLicenseCount();
}

/*****************************************************************************
 函 数 名  : InitInstance
 功能描述  : 初始化函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2002年9月8日
    作    者   : 张玉新
    修改内容   : 新生成函数

*****************************************************************************/
int CBakTaskMgr::open(void* args)
{
	CINIFile ini(GetCfgFilePath());
	if(ini.Open() != TRUE)
	{
		TRACE(MTS_BACKUP,S_BACKUP_INI_OPEN_FAIL);
		return ERR_FAIL;
	}
	char szStatusFileName[MAX_PATH];
	ini.GetString(CFG_SEC_NET_BACKUP,CFG_BACKUP_STATUS_FILE_DIR,CFG_BACKUP_STATUS_SRC_DIR_DEFAULT,szStatusFileName,MAX_PATH);
	m_szBakStatusName = szStatusFileName;
	m_szBakStatusName += "/backup_status.bsf";
	ini.Close();
	CreateDeepDir(szStatusFileName);
    //打开状态文件
	if(FALSE == m_StatusFile.Open(m_szBakStatusName.c_str()))
    {
        char szTmpBuf[BK_MAX_INFO_LEN];
        sprintf(szTmpBuf, "Errno: %d (%s)", errno, strerror(errno));
		STRING sTmpInfo = S_BACKUP_OPEN_STATUS_FAIL; 
        sTmpInfo += m_szBakStatusName.c_str();
        sTmpInfo += szTmpBuf;
        TRACE(MTS_BACKUP, sTmpInfo.c_str());

        return ERR_FAIL;
    }

    m_pManBakStatus = new CManualStatus;
    if (!m_pManBakStatus->Init())
    {
        return ERR_FAIL;
    }
       
	if (CreateBakTasks())
	{
		TRACE(MTS_BACKUP,S_BACKUP_CREATE_TASK_FAIL);
		return ERR_FAIL;
	}

	StartAllTasks();

	//启动备份监控线程
    if(activate())
	{
		TRACE(MTS_BACKUP,S_BACKUP_MANAGER_THREAD_FAIL);
		return ERR_FAIL;
	}
	return ERR_SUCCESS;
}
/*****************************************************************************
 函 数 名  : Sleep
 功能描述  : 睡觉函数
 输入参数  : 睡觉的秒数
 输出参数  : 无
 返 回 值  : BOOL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2002年9月8日
    作    者   : 张玉新
    修改内容   : 新生成函数

*****************************************************************************/
BOOL CBakTaskMgr::Sleep(int nSeconds)
{
	ACE_Time_Value Seconds(nSeconds);
	Seconds += ACE_OS::gettimeofday();
	
    int nRet = m_ExitEvent.wait(&Seconds);

	return (nRet == 0);
}
/*****************************************************************************
 函 数 名  : ExitInstance
 功能描述  : 退出函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2002年9月8日
    作    者   : 张玉新
    修改内容   : 新生成函数

*****************************************************************************/
void CBakTaskMgr::Exit()
{
    //停止检测线程
	m_ExitEvent.signal();
	wait();


	//删除m_BakTaskMap各个节点
	Clear();

	//释放m_pbBakFatalFlag
	delete[] m_pbBakFatalFlag;

	//关闭状态文件
	m_StatusFile.Close();
}
/*****************************************************************************
 函 数 名  : CBakTaskMgr::CreateBakTasks
 功能描述  : 创建备份任务
 输入参数  : 无
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2002年9月9日
    作    者   : 张玉新
    修改内容   : 新生成函数

*****************************************************************************/
int CBakTaskMgr::CreateBakTasks()
{
    //打开INI文件，读取备份任务个数参数配置；
    CINIFile ini(GetCfgFilePath());
	if(ini.Open() != TRUE)
	{
		TRACE(MTS_BACKUP,S_BACKUP_INI_OPEN_FAIL);
		return ERR_FAIL;
	}
	
	char szDefaultDestHostIP[MAX_HOST_LEN];
    char szDestHostIP[MAX_HOST_LEN];
	ini.GetString(CFG_SEC_NET_BACKUP,CFG_BACKUP_DEST_HOST_IP,
		CFG_BACKUP_DEST_HOST_IP_DEFAULT,szDefaultDestHostIP,MAX_HOST_LEN);
    int nBackupTaskCount = 0;
	nBackupTaskCount = ini.GetInt(CFG_SEC_NET_BACKUP,CFG_NET_BACKUP_TASK_COUNT,0);
	//根据备份任务个数，创建各个备份任务，分配m_pbBakFatalFlag内存，
	//并初始化为0
	if(nBackupTaskCount <= 0)
	{
		TRACE(MTS_BACKUP,S_BACKUP_TASK_ISNULL);
		return ERR_FAIL;
	}
	else if(nBackupTaskCount > 100)
	{
		nBackupTaskCount = 100;
		TRACE(MTS_BACKUP,S_BACKUP_EXCEED_LIMIT,CFG_NET_BACKUP_TASK_COUNT,100);
	}
	
    m_pbBakFatalFlag = new BOOL[nBackupTaskCount + 1];
	memset(m_pbBakFatalFlag,0,nBackupTaskCount + 1);

	CBakTask *pTask = NULL;
	char szBakSec[16];
	for(int i = 1; i <= nBackupTaskCount; i++)
	{
		SNPRINTF(szBakSec,16,"%s%d",CFG_NET_BACKUP,i);
		szBakSec[15] = 0;
		ini.GetString(szBakSec,CFG_BACKUP_DEST_HOST_IP,
			szDefaultDestHostIP,szDestHostIP,MAX_HOST_LEN);
		if(ACE_OS::strcasecmp(szDestHostIP,"Local") == 0)
		{
			pTask = new CBakTask(i,this); 
		}
		else
		{
			pTask = new CNetBakTask(i,this);
		}

        if(pTask->GetCfg())
        {
    		m_BakTaskMap[i] = pTask;
        }
        else
        {
            TRACE(MTS_BACKUP,S_BACKUP_OPEN_TASK_FAIL, i);
            delete pTask;
            pTask = NULL;
        }
	}
    
	//关闭INI文件。
	ini.Close();
	return ERR_SUCCESS;
}
/*****************************************************************************
 函 数 名  : CBakTaskMgr::Clear
 功能描述  : 清除所有备份任务
 输入参数  : 无
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2002年9月6日
    作    者   : 张玉新
    修改内容   : 新生成函数

*****************************************************************************/
void CBakTaskMgr::Clear()
{
    //清除所有备份任务
	MAP<int, CBakTask*>::iterator it = m_BakTaskMap.begin();
	for(; it != m_BakTaskMap.end(); it++)
	{
		CBakTask *pTask = it->second;
		pTask->SignalExitFlag();
	}
	
    it = m_BakTaskMap.begin();
	for(; it != m_BakTaskMap.end(); it++)
	{
		CBakTask *pTask = it->second;
		pTask->Stop();
		delete pTask , pTask = NULL;
	}
    
    ClearManBakTasks();
    ClearManDelTasks();
}
/*****************************************************************************
 函 数 名  : CBakTaskMgr::svc
 功能描述  : 备份线程检测函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2002年9月6日
    作    者   : 张玉新
    修改内容   : 新生成函数

*****************************************************************************/
int CBakTaskMgr::svc()
{
    MAP<int, CBakTask*>::iterator it;
	CBakTask *pTask = NULL;
	do
	{
		for(it = m_BakTaskMap.begin(); it != m_BakTaskMap.end(); it++)
		{
			pTask = it->second;
			if(pTask == NULL)
			{
				continue;
			}
			if(pTask->IsExit())
			{
				pTask->open();
                MSGOUT(MTS_BACKUP,MSGOUT_LEVEL_SIDLY,
                       S_BACKUP_TASK_REACT,pTask->GetTaskID());
			}
		}
	}while(!Sleep(60));
	
	TRACE(MTS_BACKUP,S_BACKUP_MANAGER_THREAD_EXIT);
	return ERR_SUCCESS;
}
/*****************************************************************************
 函 数 名  : StartAllTasks
 功能描述  : 启动所有备份线程
 输入参数  : 无
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2002年9月6日
    作    者   : 张玉新
    修改内容   : 新生成函数

*****************************************************************************/
int CBakTaskMgr::StartAllTasks()
{
    //读取m_pBakTaskMap下一个节点，调用其open()函数
    for(MAP<int, CBakTask*>::iterator it = m_BakTaskMap.begin();
	    it != m_BakTaskMap.end(); it++)
	{      
		if(it->second->open())
        {
            TRACE(MTS_BACKUP,S_BACKUP_OPEN_TASK_FAIL,it->first);
        }
	}
	return ERR_SUCCESS;
}
/*****************************************************************************
 函 数 名  : CBakTaskMgr::GetStatInfo
 功能描述  : 获取某个备份任务的状态
 输入参数  : const char* pszTaskName
             SStatusNode *pStatInfo
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2002年9月6日
    作    者   : 张玉新
    修改内容   : 新生成函数

*****************************************************************************/
int CBakTaskMgr::GetStatInfo( const char* pszTaskName, SStatusNode *pStatInfo )
{
    //获取m_BakMutex
    ACE_GUARD_RETURN(ACE_Mutex,ace_mon,m_BakMutex,-1);

	int nRet = m_StatusFile.GetStatInfo(pszTaskName,pStatInfo);
    //释放m_BakMutex

    return nRet;
}
/*****************************************************************************
 函 数 名  : CBakTaskMgr::WriteStatusFile
 功能描述  : 从文件的当前位置开始写入数据
 输入参数  : void *pData
             UINT4 uLen
 输出参数  : 无
 返 回 值  : BOOL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2002年9月6日
    作    者   : 张玉新
    修改内容   : 新生成函数

*****************************************************************************/
BOOL CBakTaskMgr::WriteStatusFile(SStatusNode &Data)
{
    //获取m_BakMutex
    ACE_GUARD_RETURN(ACE_Mutex,ace_mon,m_BakMutex,FALSE);

    int nRet = m_StatusFile.GetStatInfo(Data.szPath,NULL);
	if(-1 == nRet)
	{
		TRACE(MTS_BACKUP, S_BACKUP_STATUSFILE_OPERATE_FAIL);
		return FALSE;
	}
	
	if(-2 == nRet)
	{
		m_StatusFile.SetOffset(0, SEEK_END);
	}
	else
	{
		m_StatusFile.SetOffset(nRet);
	}
		
	//释放m_BakMutex
	return (m_StatusFile.Write(&Data,sizeof(SStatusNode)));
}



/////////////////////////////////////////////////////////////////////////////
// 手工备份新增加函数实现 

/*****************************************************************************
 函 数 名  : SendMMLReport
 功能描述  : 发送MML响应报文到客户端
 输入参数  : const char* szReport, MML响应报文
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : ParseMsg
 
 修改历史      :
  1.日    期   : 2004年5月20日
    作    者   : Z30971
    修改内容   : NULL
****************************************************************************/
/*
void CBackupServer::SendMMLReport(const char* szReport, int nClientNo)
{
    int nStrLen = strlen(szReport);

    MSG_INTRA* pMsg = new(nStrLen + 1) MSG_INTRA;
    pMsg->SenderPid = PT_OM;
    pMsg->SenderMid = MT_BACKUP;
    pMsg->ReceiverPid = PT_OM;
    pMsg->ReceiverMid = MT_MMLDEBUG;
    pMsg->AppType = APP_MML_NORM;
    pMsg->ClientNo = nClientNo;

    memcpy(pMsg->Data, szReport, nStrLen + 1);

    SendMsg(pMsg);
}
*/
/*****************************************************************************
 函 数 名  : SendSimpleInfoReport
 功能描述  : 发送简单的MML响应报文到客户端
 输入参数  : szCmd, 回显的MML命令
             nRetCode, MML命令返回码 
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2004年5月20日
    作    者   : Z30971
    修改内容   : NULL
****************************************************************************/
/*
void CBackupServer::SendSimpleReport(const char* szCmd, int nRetCode,
                                     const char* szInfo,int nClientNo)
{
    m_pMMLReport->MakeReportHead();
    m_pMMLReport->MakeCmdDisplay(szCmd, 1, nRetCode, "Backup");
    m_pMMLReport->AddString(szInfo);
    m_pMMLReport->AddString("\r\n");
    m_pMMLReport->MakeReportTail();
    const char* szReport = m_pMMLReport->GetReport();

    SendMMLReport(szReport, nClientNo);
}
*/
/*****************************************************************************
 函 数 名  : GetMatchTaskID
 功能描述  : 获得相匹配的备份任务的ID
 输入参数  : szSrcDir, 备份源路径
             nTaskID, 线程ID 
 输出参数  : nTaskID, 匹配的任务ID
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2004年5月20日
    作    者   : Z30971
    修改内容   : NULL
****************************************************************************/
BOOL CBakTaskMgr::GetMatchTaskID(const char* szSrcDir, int& nTaskID)
{
    TASKMAP::iterator it = m_BakTaskMap.begin();
    for (; it != m_BakTaskMap.end(); it++)
    {
        if(it->second->IsMatched(szSrcDir))
        {
            nTaskID = it->first;
            return TRUE;
        }
    }
    return FALSE;
}
/*****************************************************************************
 函 数 名  : ManualBakTaskProc
 功能描述  : 手工备份任务处理
 输入参数  : nClientNo, 客户端号
             szCmd,     回显命令
             nTaskId,   线程ID
 输出参数  : 无
 返 回 值  : BOOL, 成功与否
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2004年5月23日
    作    者   : Z30971
    修改内容   : NULL

*****************************************************************************/
BOOL CBakTaskMgr::ManualBakTaskProc(int nClientNo, const char* szCmd, int nTaskId,
                                const char* szBakPath, const char* szName,
                                int nFlag)
{
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, m_TaskMutex, -1);
    
    DelExitBakTasks();

    BOOL bEndCmd = FALSE;
    STRING strName = szName;
    
    int nPos = strName.find_last_of(NAMELIST_END_CHAR);
    if(nPos != (strName.length() - 1))
    {
        m_pBackupServer->SendSimpleReport(szCmd, 0, "", nClientNo);
    }
    else
    {
        strName[nPos] = 0;
        bEndCmd = TRUE;
    }
 
    SManBakTaskInfo *pTaskInfo = NULL;
    CManBakTask     *pBakTask  = NULL;
    MANUALBAKTASKMAP::iterator it = m_ManBakTaskMap.find(nClientNo);
    
    if(it != m_ManBakTaskMap.end())
    {
        pTaskInfo = m_ManBakTaskMap[nClientNo];
        
        
        //同一个客户端只允许一个任务在运行,其它的丢弃
        BOOL bErr = (pTaskInfo->bEndCmd || (pTaskInfo->nTaskId != nTaskId));

        if (bErr && bEndCmd)
        {
            m_pBackupServer->SendSimpleReport(szCmd, 0, S_BACKUP_DISCARD_TASK, nClientNo);
            return FALSE;
        }
        pTaskInfo->NameList.push_back(strName);

        pBakTask = pTaskInfo->pTask;
    }
    else
    {
        pTaskInfo = new SManBakTaskInfo;
        pTaskInfo->NameList.push_back(strName);
        
        pBakTask  = new CManBakTask(nClientNo, nTaskId, this, szBakPath);
        pTaskInfo->pTask   = pBakTask;
        pTaskInfo->nTaskId = nTaskId;
        pTaskInfo->nFlag   = nFlag;
        pTaskInfo->bEndCmd = FALSE;

        m_ManBakTaskMap[nClientNo] = pTaskInfo;
    }

    if(pBakTask && bEndCmd)
    {
        //记录最后一个命令
        STRNCPY(pTaskInfo->szCmd, szCmd, 256);
        pTaskInfo->bEndCmd = TRUE;
        
        if(pBakTask->open())
        {
            TRACE(MTS_BACKUP,S_BACKUP_OPEN_TASK_FAIL, nTaskId);
            DelManBakTask(nClientNo);
        }
    }

    return TRUE;
}

/*****************************************************************************
 函 数 名  : ManualDelTaskProc
 功能描述  : 手工删除任务处理
 输入参数  : nClientNo, 客户端号
             szCmd,     回显命令
             nTaskId,   线程ID
 输出参数  :
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2004年5月23日
    作    者   : Z30971
    修改内容   : NULL

*****************************************************************************/
void CBakTaskMgr::ManualDelTaskProc(int nClientNo, const char* szCmd, int nTaskId,
                                    const char* szDelPath, const char* szName,
                                    int nFlag)
{
    BOOL bEndCmd = FALSE;

    STRING strName = szName;
    
    int nPos = strName.find_last_of(NAMELIST_END_CHAR);
    if(nPos != (strName.length() - 1))
    {
        m_pBackupServer->SendSimpleReport(szCmd, 0, "", nClientNo);
    }
    else
    {
        //m_pBackupServer->SendSimpleReport(szCmd, 0, S_DELETE_RUNNING_NOW, nClientNo);
        
        strName[nPos] = 0;
        bEndCmd = TRUE;
    }

    SManDelTaskInfo *pTaskInfo = NULL;
    MANUALDELTASKMAP::iterator it = m_ManDelTaskMap.find(nClientNo);
    
    if(it != m_ManDelTaskMap.end())
    {
        pTaskInfo = m_ManDelTaskMap[nClientNo];
        
        //同一个客户端发送了不同的任务，则丢弃此命令
        if(pTaskInfo->nTaskId != nTaskId)
        {
            return;
        }
        pTaskInfo->NameList.push_back(strName);
    }
    else
    {
        pTaskInfo = new SManDelTaskInfo;
        
        pTaskInfo->NameList.push_back(strName);
        
        pTaskInfo->nTaskId = nTaskId;
        pTaskInfo->nFlag = nFlag;

        m_ManDelTaskMap[nClientNo] = pTaskInfo;
    }

    BOOL bSuc = FALSE;
    if (bEndCmd)
    {
        if (nFlag == DATE_DIR)
        {
            bSuc = DelDirs(szDelPath, pTaskInfo->NameList, nTaskId);
        }
        else
        {
            bSuc = DelFiles(szDelPath, pTaskInfo->NameList, nTaskId);
        }

        if (bSuc)
        {
            m_pBackupServer->SendSimpleReport(szCmd, 1, S_DELETE_SUCCESS, nClientNo);
        }
        else
        {
            m_pBackupServer->SendSimpleReport(szCmd, 0, S_DELETE_FAIL, nClientNo);
        }

        //从map中删除某个键
        DelManDelTask(nClientNo);
    }

}
/*****************************************************************************
 函 数 名  : CBakTaskMgr::DelManBakTask
 功能描述  : 删除手工备份任务信息
 输入参数  : nClientNo, 客户端号
 输出参数  : 无
 返 回 值  : BOOL, 成功与否
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2004年5月23日
    作    者   : Z30971
    修改内容   : NULL
*****************************************************************************/
BOOL CBakTaskMgr::DelManBakTask(int nClientNo)
{
    MANUALBAKTASKMAP::iterator it = m_ManBakTaskMap.find(nClientNo);
    if(it == m_ManBakTaskMap.end())
    {
        return FALSE;        
    }
   
    SManBakTaskInfo *pTaskInfo = m_ManBakTaskMap[nClientNo];
   
    delete pTaskInfo->pTask;
    delete pTaskInfo;
   
    m_ManBakTaskMap.erase(nClientNo);
    return TRUE;
}
/*****************************************************************************
 函 数 名  : CBakTaskMgr::DelManDelTask
 功能描述  : 删除手工删除任务信息
 输入参数  : nClientNo, 客户端号
 输出参数  : 无
 返 回 值  : BOOL, 成功与否
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2004年5月23日
    作    者   : Z30971
    修改内容   : NULL
*****************************************************************************/
BOOL CBakTaskMgr::DelManDelTask(int nClientNo)
{
    MANUALDELTASKMAP::iterator it = m_ManDelTaskMap.find(nClientNo);
    if(it == m_ManDelTaskMap.end())
    {
        return FALSE;        
    }
   
    SManDelTaskInfo *pTaskInfo = m_ManDelTaskMap[nClientNo];
   
    delete pTaskInfo;
   
    m_ManDelTaskMap.erase(nClientNo);
    return TRUE;
}
/*****************************************************************************
 函 数 名  : SetTaskExitFlag
 功能描述  : 设置备份任务退出标志
 输入参数  : nClientNo, 客户端号
 输出参数  : 无
 返 回 值  : BOOL, 成功与否
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2004年5月20日
    作    者   : Z30971
    修改内容   : NULL
****************************************************************************/
BOOL CBakTaskMgr::SetTaskExitFlag(int nClientNo)
{
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, m_TaskMutex, -1);
    
    MANUALBAKTASKMAP::iterator it = m_ManBakTaskMap.find(nClientNo);
    SManBakTaskInfo* pManTaskInfo;

    /*
     * 将客户端号设置为大于MAX_CLIENTNO的数值,
     * 则标识为已经退出,允许删除	
     */
    if(it != m_ManBakTaskMap.end())
    {
        pManTaskInfo = m_ManBakTaskMap[nClientNo];
        
        m_ManBakTaskMap.erase(it);
        m_ManBakTaskMap[nClientNo + MAX_CLIENTNO] = pManTaskInfo;
        return TRUE;
    }

    return FALSE;
}
/*****************************************************************************
 函 数 名  : DelExitManTask
 功能描述  : 删除退出任务
 输入参数  : 无
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2004年5月20日
    作    者   : Z30971
    修改内容   : NULL
****************************************************************************/
void CBakTaskMgr::DelExitBakTasks()
{
    SManBakTaskInfo* pManTaskInfo;
    MANUALBAKTASKMAP::iterator it = m_ManBakTaskMap.begin();

    LIST<int> ClientNoList;
    for (; it != m_ManBakTaskMap.end(); it++)
    {
        pManTaskInfo = it->second;
        if(it->first > MAX_CLIENTNO)
        {
            ClientNoList.push_back(it->first);
        }
    }

    int nClientNo;
    while (ClientNoList.size() > 0)  
    {
        nClientNo = ClientNoList.front();
        pManTaskInfo = m_ManBakTaskMap[nClientNo];

        delete pManTaskInfo->pTask;
        delete pManTaskInfo;
        m_ManBakTaskMap.erase(nClientNo);

        ClientNoList.pop_front();
    }
}
/*****************************************************************************
 函 数 名  : ClearManBakTasks
 功能描述  : 清空手工备份任务信息
 输入参数  : 无
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2004年5月20日
    作    者   : Z30971
    修改内容   : NULL
****************************************************************************/
void CBakTaskMgr::ClearManBakTasks()
{
    SManBakTaskInfo *pTaskInfo = NULL;
    MANUALBAKTASKMAP::iterator it = m_ManBakTaskMap.begin();
    
    for(; it != m_ManBakTaskMap.end(); it++)
    {
        pTaskInfo = it->second;
        if(pTaskInfo->pTask != NULL)
        {
            pTaskInfo->pTask->SignalExitFlag();
            pTaskInfo->pTask->Stop();
            
            delete pTaskInfo->pTask;
            pTaskInfo->pTask = NULL;
        }
        
        delete pTaskInfo;
        pTaskInfo = NULL;
    }
    
    m_ManBakTaskMap.clear();
}
/*****************************************************************************
 函 数 名  : ClearManDelTasks
 功能描述  : 清空手工删除任务信息
 输入参数  : 无
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2004年5月20日
    作    者   : Z30971
    修改内容   : NULL
****************************************************************************/
void CBakTaskMgr::ClearManDelTasks()
{
    m_ManDelTaskMap.clear();
}
/*****************************************************************************
 函 数 名  : AddBakedFile
 功能描述  : 增加已备份文件
 输入参数  : FileNode, 文件节点
 输出参数  : 无
 返 回 值  : BOOL, 成功与否
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2004年5月23日
    作    者   : Z30971
    修改内容   : NULL
    
*****************************************************************************/
BOOL CBakTaskMgr::AddBakedFile(SFilesListNode &FileNode)
{
    return m_pManBakStatus->AddFile(FileNode);
}
/*****************************************************************************
 函 数 名  : DelBakedFile
 功能描述  : 删除备份文件节点
 输入参数  : FileNode, 文件节点
 输出参数  : 无
 返 回 值  : BOOL, 成功与否
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2004年5月23日
    作    者   : Z30971
    修改内容   : NULL

*****************************************************************************/
BOOL CBakTaskMgr::DelBakedFile(SFilesListNode& FileNode)
{
    return m_pManBakStatus->DelFile(FileNode);
}
/*****************************************************************************
 函 数 名  : DelDirs
 功能描述  : 删除目录集合
 输入参数  : szPath, 目录集合所在的路径
             DirList,目录集合
             nTaskID,线程ID
 输出参数  : 无
 返 回 值  : BOOL, 成功与否
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2004年5月23日
    作    者   : Z30971
    修改内容   : NULL

*****************************************************************************/
BOOL CBakTaskMgr::DelDirs (const char* szPath, STRINGLIST &DirList, int nTaskID)
{
    STRING szDir = szPath;
    szDir += "/";
    
    STRING szDirFullPath;
    STRINGLIST FileList;

    STRINGLIST::iterator it= DirList.begin();
    for (; it != DirList.end(); it++)
    {
        szDirFullPath = szDir + *it;

        if (0 != GetFileList(szDirFullPath.c_str(), FileList))
        {
            if (!DelFiles(szDirFullPath.c_str(), FileList, nTaskID))
            {
                return FALSE;
            }
        }
        
        if (0 != rmdir(szDirFullPath.c_str()))
        {
            return FALSE;
        }
    }

    return TRUE;
}
/*****************************************************************************
 函 数 名  : DelFiles
 功能描述  : 删除话单文件集合
 输入参数  : szPath, 文件集合所处目录
             FileList, 文件集合
             nTaskID, 线程ID
 输出参数  : 无
 返 回 值  : BOOL, 成功失败
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2004年5月23日
    作    者   : Z30971
    修改内容   : NULL

*****************************************************************************/
BOOL CBakTaskMgr::DelFiles(const char* szPath, STRINGLIST &FileList, int nTaskID)
{
    STRING szDir = szPath;
    szDir += "/";
    
    STRING szFileFullPath;

    STRINGLIST::iterator it = FileList.begin();
    for (; it != FileList.end(); it++)
    {
        szFileFullPath = szDir + *it;
        
        if(!JudgeIsBackuped(szFileFullPath.c_str(), nTaskID))
        {
            return FALSE;
        }

        if (!FileDel(szFileFullPath.c_str()))
        {
            return FALSE;
        }
    }

    return TRUE;
}
/*****************************************************************************
 函 数 名  : IsAutoBackuped
 功能描述  : 判断是否自动备份过
 输入参数  : szFilePath, 文件全路径
             nTaskID, 线程ID
 输出参数  : 无
 返 回 值  : BOOL, 是否成功
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2004年5月23日
    作    者   : Z30971
    修改内容   : NULL

*****************************************************************************/
BOOL CBakTaskMgr::IsAutoBackuped (const char* szFilePath, int nTaskID)
{
    ACE_stat FileStat;
    ACE_OS::stat(szFilePath, &FileStat);
    
    CBakTask *pTask = NULL;
    TASKMAP::iterator itTask = m_BakTaskMap.find(nTaskID);
    if (itTask == m_BakTaskMap.end())
    {
        return FALSE;        
    }
    pTask = m_BakTaskMap[nTaskID];
    
    const char* p1 = strrchr(szFilePath, '/');
       
#ifdef _PLATFORM_WIN32
    const char* p2 = strrchr(szFilePath, '\\');
#else //_PLATFORM_WIN32
    const char* p2 = NULL;
#endif //_PLATFORM_WIN32
    const char* szFileName = max(p1, p2);
    if (szFileName == NULL)
    {
        szFileName = szFilePath;
    }
    else
    {
        szFileName++;
    }
   
    if (!pTask->JudgeIsBackupped(szFileName, FileStat.st_ctime))
    {
        return FALSE;
    }

    return TRUE;
}
/*****************************************************************************
 函 数 名  : IsManualBackuped
 功能描述  : 判断是否手工备份过
 输入参数  : szFilePath, 文件全路径
 输出参数  : 无
 返 回 值  : BOOL, 是否成功
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2004年5月20日
    作    者   : Z30971
    修改内容   : NULL
****************************************************************************/
BOOL CBakTaskMgr::IsManualBackuped(const char* szFilePath)
{
    ACE_stat FileStat;
    ACE_OS::stat(szFilePath, &FileStat);

    SFilesListNode FileNode;
    FileNode.tFileTime = FileStat.st_ctime;
    STRNCPY(FileNode.szFileName, szFilePath, MAX_PATH);
    
    return m_pManBakStatus->FindFile(FileNode);
}
/*****************************************************************************
 函 数 名  : JudgeIsBackuped
 功能描述  : 判断是否备份过
 输入参数  : szFilePath, 文件全路径
             nTaskID, 线程ID 
 输出参数  : 无
 返 回 值  : BOOL, 是否成功
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2004年5月20日
    作    者   : Z30971
    修改内容   : NULL
****************************************************************************/
BOOL CBakTaskMgr::JudgeIsBackuped(const char* szFilePath, int nTaskID)
{
    return (IsAutoBackuped(szFilePath, nTaskID) || IsManualBackuped(szFilePath)); 
}
/*****************************************************************************
 函 数 名  : GetManTaskInfo
 功能描述  : 获得手工备份信息
 输入参数  : nClientNo, 客户端口号 pTaskInfo, 备份任务指针
 输出参数  : pTaskInfo, 手工备份信息
 返 回 值  : BOOL, 成功与否
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2004年5月23日
    作    者   : Z30971
    修改内容   : NULL

*****************************************************************************/
BOOL CBakTaskMgr::GetBakTaskInfo(int nClientNo,SManBakTaskInfo* &pBakTaskInfo)
{
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, m_TaskMutex, -1);
    
    MANUALBAKTASKMAP::iterator it = m_ManBakTaskMap.find(nClientNo);
    if(it != m_ManBakTaskMap.end())
    {
        pBakTaskInfo = m_ManBakTaskMap[nClientNo];
        return TRUE;
    }
    
    pBakTaskInfo = NULL;
    return FALSE;
}
/*****************************************************************************
 函 数 名  : ReqBakCdrProc
 功能描述  : 手工备份话单
 输入参数  : MSG_INTRA *pMsg
 输出参数  : 无
 返 回 值  : void
 调用函数  : CBakTaskMgr::ReqBakCdrProc 
 被调函数  : ParseMsg
 
 修改历史      :
  1.日    期   : 2004年5月23日
    作    者   : Z30971
    修改内容   : NULL
命令如下:
BAK CDR: ap="3G", bt=1, chl="CHL_1", date="20020225",
         flg=1, name="b00000046.dat";
最后一个名称的末尾加一个特殊标志~
*****************************************************************************/
void CBackupServer::ReqBakCdrProc(MSG_INTRA *pMsg)
{
    CParseResult* pResult = *((CParseResult** )(pMsg->Data));
    
    int nCdrType, nFlg;
    const char* szAPName = pResult->GetStringVal(1);
    pResult->GetDigitVal(&nCdrType, 2);
    const char* szChlName = pResult->GetStringVal(3);
    const char* szDateName = pResult->GetStringVal(4);
    pResult->GetDigitVal(&nFlg, 5);
    const char* szName = pResult->GetStringVal(6);
  
    //根据参数获得要备份的话单的路径
    char szBakPath[MAX_PATH];
    
    //备份原始话单下多个日期目录
    if((ORIG_BILL == nCdrType) && (DATE_DIR == nFlg))
    {
        sprintf(szBakPath, "%s/%s", m_szOrigBasePath, szAPName);
    }
    //备份原始话单某日期目录下多个话单文件
    else if((ORIG_BILL == nCdrType) && (DATE_DIR != nFlg))
    {
        sprintf(szBakPath, "%s/%s/%s", m_szOrigBasePath,
                szAPName, szDateName);
    }
    //备份最终话单某通道下多个日期目录
    else if((ORIG_BILL != nCdrType) && (DATE_DIR == nFlg))
    {
        sprintf(szBakPath, "%s/%s/%s", m_szFinalBasePath,
                szAPName, szChlName);
    }
    //备份最终话单某通道某日期下多个话单文件
    else
    {
        sprintf(szBakPath, "%s/%s/%s/%s", m_szFinalBasePath, szAPName,
                szChlName, szDateName);
    }

    int nAutoTaskID;
    
    //得到相应的自动备份线程任务ID
    if(!m_pBakTaskMgr->GetMatchTaskID(szBakPath, nAutoTaskID))
    {
        SendSimpleReport(pResult->GetCmdLine(), 0, S_BACKUP_NOAUTO_TASK, pMsg->ClientNo);
        return;
    }

    //SendSimpleReport(pResult->GetCmdLine(), 0, S_BACKUP_RUNNING_NOW, pMsg->ClientNo);

    m_pBakTaskMgr->ManualBakTaskProc(pMsg->ClientNo, pResult->GetCmdLine(),
                                 nAutoTaskID, szBakPath, szName, nFlg);
}

/*****************************************************************************
 函 数 名  : ReqDelCdrProc
 功能描述  : 手工备份话单
 输入参数  : MSG_INTRA *pMsg
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : ParseMsg
 
 修改历史      :
  1.日    期   : 2004年5月14日
    作    者   : Z30971
    修改内容   : NULL
命令如下:
DEL CDR: ap="3G", bt=1, chl="CHL_1", date="20020225",
         flg=1, name="b00000046.dat", 
         er="b00000046.dat";
*****************************************************************************/
void CBackupServer::ReqDelCdrProc(MSG_INTRA *pMsg)
{
    CParseResult* pResult = *((CParseResult** )(pMsg->Data));
    
    int nCdrType, nFlg;
    const char* szAPName = pResult->GetStringVal(1);
    pResult->GetDigitVal(&nCdrType, 2);
    const char* szChlName = pResult->GetStringVal(3);
    const char* szDateName = pResult->GetStringVal(4);
    pResult->GetDigitVal(&nFlg, 5);
    const char* szName = pResult->GetStringVal(6);
  
    //根据参数获得要备份的话单的路径
    char szDelPath[MAX_PATH];
    
    //备份原始话单下多个日期目录
    if((ORIG_BILL == nCdrType) && (DATE_DIR == nFlg))
    {
        sprintf(szDelPath, "%s/%s", m_szOrigBasePath, szAPName);
    }
    //备份原始话单某日期目录下多个话单文件
    else if((ORIG_BILL == nCdrType) && (DATE_DIR != nFlg))
    {
        sprintf(szDelPath, "%s/%s/%s", m_szOrigBasePath,
                szAPName, szDateName);
    }
    //备份最终话单某通道下多个日期目录
    else if((ORIG_BILL != nCdrType) && (DATE_DIR == nFlg))
    {
        sprintf(szDelPath, "%s/%s/%s", m_szFinalBasePath,
                szAPName, szChlName);
    }
    //备份最终话单某通道某日期下多个话单文件
    else
    {
        sprintf(szDelPath, "%s/%s/%s/%s", m_szFinalBasePath, szAPName,
                szChlName, szDateName);
    }

    int nAutoTaskID;
    //得到相应的自动备份线程任务ID
    if(!m_pBakTaskMgr->GetMatchTaskID(szDelPath, nAutoTaskID))
    {
        SendSimpleReport(pResult->GetCmdLine(), 0, S_BACKUP_NOAUTO_TASK, pMsg->ClientNo);
        return;
    }

    m_pBakTaskMgr->ManualDelTaskProc(pMsg->ClientNo, pResult->GetCmdLine(),
                                     nAutoTaskID, szDelPath,
                                     szName, nFlg);
 
}
