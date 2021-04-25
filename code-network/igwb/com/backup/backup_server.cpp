/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : backup_server.cpp
  �� �� ��   : ����
  ��    ��   : ������
  ��������   : 2002��9��8��
  ����޸�   :
  ��������   : ����ģ���ʵ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2002��9��8��
    ��    ��   : ������
    �޸�����   : �����ļ�

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
 �� �� ��  : ���캯��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2002��9��8��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
CBackupServer::CBackupServer()
{
    m_pMMLReport = NULL;
    m_pBakTaskMgr = NULL;
}
/*****************************************************************************
 �� �� ��  : InitInstance
 ��������  : ��Դ��ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2002��9��8��
    ��    ��   : ������
    �޸�����   : �����ɺ���

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

   	//����m_pBakTaskMgrָ��
    m_pBakTaskMgr = new CBakTaskMgr(this);
    
	if(m_pBakTaskMgr->open())
	{
		//��ʹʧ��Ҳ��ʹOM���������������ݶ���
		TRACE(MTS_BACKUP,S_BACKUP_START_FAIL);
	}
    
    m_pMMLReport = new CMMLReport("Backup");

	return ERR_SUCCESS;
}
/*****************************************************************************
 �� �� ��  : ExitInstance
 ��������  : �˳�����
 �������  : ��
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2002��9��8��
    ��    ��   : ������
    �޸�����   : �����ɺ���

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
 �� �� ��  : GetModuleType
 ��������  : ����ģ������
 �������  : ��
 �������  : ��
 �� �� ֵ  : MODULE_TYPE
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2002��9��8��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
MODULE_TYPE CBackupServer::GetModuleType()
{
    return MT_BACKUP;
}
/*****************************************************************************
 �� �� ��  : ParseMsg
 ��������  : �����ڲ���Ϣ����
 �������  : MSG_INTRA *pMsg
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��20��
    ��    ��   : Zou Yongsheng
    �޸�����   : 

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
macro_name = ���������ļ�
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

	//by ldf 2004-08-17 OM-M2000����
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

/*
[RTR CFGFILE]
macro_code = 158
macro_name = �ָ������ļ�
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

	//by ldf 2004-08-17 OM-M2000����
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

/*
[ULD CFGFILE]
macro_code=156
macro_name=���������ļ�
para_num=5
1=IP (FTP server IP)
2=USR (FTP SERVER USERNAME)
3=PWD (FTP SERVER PASSWORD)
4=DIR (FTP SERVER DIRECTORY)
5=FT (�����ļ�����) {0~1}
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

	//by ldf 2004-08-17 OM-M2000����
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

/*
[DLD CFGFILE]
macro_code=157
macro_name=���������ļ�
para_num=5
1=IP (FTP server IP)
2=USR (FTP SERVER USERNAME)
3=PWD (FTP SERVER PASSWORD)
4=DIR (FTP SERVER DIRECTORY)
5=FT (�����ļ�����) {0~1}
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

	//by ldf 2004-08-17 OM-M2000����
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

/* om modify add by yangguang ---end*/



/*****************************************************************************
 �� �� ��  : ReqModifyBackupProc
 ��������  : �޸ı�������
 �������  : MSG_INTRA *pMsg
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2002��9��8��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
void CBackupServer::ReqModifyBackupProc(MSG_INTRA *pMsg)
{
    
}
/*****************************************************************************
 �� �� ��  : ReqQueryBackupProc
 ��������  : ��ѯ��������
 �������  : MSG_INTRA *pMsg
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2002��9��8��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
void CBackupServer::ReqQueryBackupProc(MSG_INTRA *pMsg)
{
    
}
/*****************************************************************************
 �� �� ��  : OnTimer
 ��������  : ��ʱ������
 �������  : const long nTimerID
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2002��9��8��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
void CBackupServer::OnTimer(const long nTimerID)
{
    CWorkModule::OnTimer(nTimerID);
}

/*****************************************************************************
 �� �� ��  : CBakTaskMgr
 ��������  : ���캯��
 �������  : ��
 �������  : ��
 �� �� ֵ  :  
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2002��9��8��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
CBakTaskMgr::CBakTaskMgr(CBackupServer* pBackupServer)
{
    //��ʼ����Ա����
	m_ExitEvent.reset();                    //�˳��¼�
    //m_BakMutex.release();                   //�̻߳������
	m_pbBakFatalFlag = NULL;                //���ݹ��ϱ�־

	m_pBackupServer = pBackupServer;
    m_pManBakStatus = NULL;
}
/*****************************************************************************
 �� �� ��  : ~CBakTaskMgr
 ��������  : ��������
 �������  : ��
 �������  : ��
 �� �� ֵ  :  
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2002��9��8��
    ��    ��   : ������
    �޸�����   : �����ɺ���

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
 �� �� ��  : InitInstance
 ��������  : ��ʼ������
 �������  : ��
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2002��9��8��
    ��    ��   : ������
    �޸�����   : �����ɺ���

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
    //��״̬�ļ�
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

	//�������ݼ���߳�
    if(activate())
	{
		TRACE(MTS_BACKUP,S_BACKUP_MANAGER_THREAD_FAIL);
		return ERR_FAIL;
	}
	return ERR_SUCCESS;
}
/*****************************************************************************
 �� �� ��  : Sleep
 ��������  : ˯������
 �������  : ˯��������
 �������  : ��
 �� �� ֵ  : BOOL
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2002��9��8��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
BOOL CBakTaskMgr::Sleep(int nSeconds)
{
	ACE_Time_Value Seconds(nSeconds);
	Seconds += ACE_OS::gettimeofday();
	
    int nRet = m_ExitEvent.wait(&Seconds);

	return (nRet == 0);
}
/*****************************************************************************
 �� �� ��  : ExitInstance
 ��������  : �˳�����
 �������  : ��
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2002��9��8��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
void CBakTaskMgr::Exit()
{
    //ֹͣ����߳�
	m_ExitEvent.signal();
	wait();


	//ɾ��m_BakTaskMap�����ڵ�
	Clear();

	//�ͷ�m_pbBakFatalFlag
	delete[] m_pbBakFatalFlag;

	//�ر�״̬�ļ�
	m_StatusFile.Close();
}
/*****************************************************************************
 �� �� ��  : CBakTaskMgr::CreateBakTasks
 ��������  : ������������
 �������  : ��
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2002��9��9��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
int CBakTaskMgr::CreateBakTasks()
{
    //��INI�ļ�����ȡ������������������ã�
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
	//���ݱ���������������������������񣬷���m_pbBakFatalFlag�ڴ棬
	//����ʼ��Ϊ0
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
    
	//�ر�INI�ļ���
	ini.Close();
	return ERR_SUCCESS;
}
/*****************************************************************************
 �� �� ��  : CBakTaskMgr::Clear
 ��������  : ������б�������
 �������  : ��
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2002��9��6��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
void CBakTaskMgr::Clear()
{
    //������б�������
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
 �� �� ��  : CBakTaskMgr::svc
 ��������  : �����̼߳�⺯��
 �������  : ��
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2002��9��6��
    ��    ��   : ������
    �޸�����   : �����ɺ���

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
 �� �� ��  : StartAllTasks
 ��������  : �������б����߳�
 �������  : ��
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2002��9��6��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
int CBakTaskMgr::StartAllTasks()
{
    //��ȡm_pBakTaskMap��һ���ڵ㣬������open()����
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
 �� �� ��  : CBakTaskMgr::GetStatInfo
 ��������  : ��ȡĳ�����������״̬
 �������  : const char* pszTaskName
             SStatusNode *pStatInfo
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2002��9��6��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
int CBakTaskMgr::GetStatInfo( const char* pszTaskName, SStatusNode *pStatInfo )
{
    //��ȡm_BakMutex
    ACE_GUARD_RETURN(ACE_Mutex,ace_mon,m_BakMutex,-1);

	int nRet = m_StatusFile.GetStatInfo(pszTaskName,pStatInfo);
    //�ͷ�m_BakMutex

    return nRet;
}
/*****************************************************************************
 �� �� ��  : CBakTaskMgr::WriteStatusFile
 ��������  : ���ļ��ĵ�ǰλ�ÿ�ʼд������
 �������  : void *pData
             UINT4 uLen
 �������  : ��
 �� �� ֵ  : BOOL
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2002��9��6��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
BOOL CBakTaskMgr::WriteStatusFile(SStatusNode &Data)
{
    //��ȡm_BakMutex
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
		
	//�ͷ�m_BakMutex
	return (m_StatusFile.Write(&Data,sizeof(SStatusNode)));
}



/////////////////////////////////////////////////////////////////////////////
// �ֹ����������Ӻ���ʵ�� 

/*****************************************************************************
 �� �� ��  : SendMMLReport
 ��������  : ����MML��Ӧ���ĵ��ͻ���
 �������  : const char* szReport, MML��Ӧ����
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : ParseMsg
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��20��
    ��    ��   : Z30971
    �޸�����   : NULL
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
 �� �� ��  : SendSimpleInfoReport
 ��������  : ���ͼ򵥵�MML��Ӧ���ĵ��ͻ���
 �������  : szCmd, ���Ե�MML����
             nRetCode, MML������� 
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��20��
    ��    ��   : Z30971
    �޸�����   : NULL
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
 �� �� ��  : GetMatchTaskID
 ��������  : �����ƥ��ı��������ID
 �������  : szSrcDir, ����Դ·��
             nTaskID, �߳�ID 
 �������  : nTaskID, ƥ�������ID
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��20��
    ��    ��   : Z30971
    �޸�����   : NULL
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
 �� �� ��  : ManualBakTaskProc
 ��������  : �ֹ�����������
 �������  : nClientNo, �ͻ��˺�
             szCmd,     ��������
             nTaskId,   �߳�ID
 �������  : ��
 �� �� ֵ  : BOOL, �ɹ����
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��23��
    ��    ��   : Z30971
    �޸�����   : NULL

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
        
        
        //ͬһ���ͻ���ֻ����һ������������,�����Ķ���
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
        //��¼���һ������
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
 �� �� ��  : ManualDelTaskProc
 ��������  : �ֹ�ɾ��������
 �������  : nClientNo, �ͻ��˺�
             szCmd,     ��������
             nTaskId,   �߳�ID
 �������  :
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��23��
    ��    ��   : Z30971
    �޸�����   : NULL

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
        
        //ͬһ���ͻ��˷����˲�ͬ����������������
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

        //��map��ɾ��ĳ����
        DelManDelTask(nClientNo);
    }

}
/*****************************************************************************
 �� �� ��  : CBakTaskMgr::DelManBakTask
 ��������  : ɾ���ֹ�����������Ϣ
 �������  : nClientNo, �ͻ��˺�
 �������  : ��
 �� �� ֵ  : BOOL, �ɹ����
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��23��
    ��    ��   : Z30971
    �޸�����   : NULL
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
 �� �� ��  : CBakTaskMgr::DelManDelTask
 ��������  : ɾ���ֹ�ɾ��������Ϣ
 �������  : nClientNo, �ͻ��˺�
 �������  : ��
 �� �� ֵ  : BOOL, �ɹ����
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��23��
    ��    ��   : Z30971
    �޸�����   : NULL
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
 �� �� ��  : SetTaskExitFlag
 ��������  : ���ñ��������˳���־
 �������  : nClientNo, �ͻ��˺�
 �������  : ��
 �� �� ֵ  : BOOL, �ɹ����
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��20��
    ��    ��   : Z30971
    �޸�����   : NULL
****************************************************************************/
BOOL CBakTaskMgr::SetTaskExitFlag(int nClientNo)
{
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, m_TaskMutex, -1);
    
    MANUALBAKTASKMAP::iterator it = m_ManBakTaskMap.find(nClientNo);
    SManBakTaskInfo* pManTaskInfo;

    /*
     * ���ͻ��˺�����Ϊ����MAX_CLIENTNO����ֵ,
     * ���ʶΪ�Ѿ��˳�,����ɾ��	
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
 �� �� ��  : DelExitManTask
 ��������  : ɾ���˳�����
 �������  : ��
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��20��
    ��    ��   : Z30971
    �޸�����   : NULL
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
 �� �� ��  : ClearManBakTasks
 ��������  : ����ֹ�����������Ϣ
 �������  : ��
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��20��
    ��    ��   : Z30971
    �޸�����   : NULL
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
 �� �� ��  : ClearManDelTasks
 ��������  : ����ֹ�ɾ��������Ϣ
 �������  : ��
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��20��
    ��    ��   : Z30971
    �޸�����   : NULL
****************************************************************************/
void CBakTaskMgr::ClearManDelTasks()
{
    m_ManDelTaskMap.clear();
}
/*****************************************************************************
 �� �� ��  : AddBakedFile
 ��������  : �����ѱ����ļ�
 �������  : FileNode, �ļ��ڵ�
 �������  : ��
 �� �� ֵ  : BOOL, �ɹ����
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��23��
    ��    ��   : Z30971
    �޸�����   : NULL
    
*****************************************************************************/
BOOL CBakTaskMgr::AddBakedFile(SFilesListNode &FileNode)
{
    return m_pManBakStatus->AddFile(FileNode);
}
/*****************************************************************************
 �� �� ��  : DelBakedFile
 ��������  : ɾ�������ļ��ڵ�
 �������  : FileNode, �ļ��ڵ�
 �������  : ��
 �� �� ֵ  : BOOL, �ɹ����
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��23��
    ��    ��   : Z30971
    �޸�����   : NULL

*****************************************************************************/
BOOL CBakTaskMgr::DelBakedFile(SFilesListNode& FileNode)
{
    return m_pManBakStatus->DelFile(FileNode);
}
/*****************************************************************************
 �� �� ��  : DelDirs
 ��������  : ɾ��Ŀ¼����
 �������  : szPath, Ŀ¼�������ڵ�·��
             DirList,Ŀ¼����
             nTaskID,�߳�ID
 �������  : ��
 �� �� ֵ  : BOOL, �ɹ����
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��23��
    ��    ��   : Z30971
    �޸�����   : NULL

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
 �� �� ��  : DelFiles
 ��������  : ɾ�������ļ�����
 �������  : szPath, �ļ���������Ŀ¼
             FileList, �ļ�����
             nTaskID, �߳�ID
 �������  : ��
 �� �� ֵ  : BOOL, �ɹ�ʧ��
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��23��
    ��    ��   : Z30971
    �޸�����   : NULL

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
 �� �� ��  : IsAutoBackuped
 ��������  : �ж��Ƿ��Զ����ݹ�
 �������  : szFilePath, �ļ�ȫ·��
             nTaskID, �߳�ID
 �������  : ��
 �� �� ֵ  : BOOL, �Ƿ�ɹ�
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��23��
    ��    ��   : Z30971
    �޸�����   : NULL

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
 �� �� ��  : IsManualBackuped
 ��������  : �ж��Ƿ��ֹ����ݹ�
 �������  : szFilePath, �ļ�ȫ·��
 �������  : ��
 �� �� ֵ  : BOOL, �Ƿ�ɹ�
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��20��
    ��    ��   : Z30971
    �޸�����   : NULL
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
 �� �� ��  : JudgeIsBackuped
 ��������  : �ж��Ƿ񱸷ݹ�
 �������  : szFilePath, �ļ�ȫ·��
             nTaskID, �߳�ID 
 �������  : ��
 �� �� ֵ  : BOOL, �Ƿ�ɹ�
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��20��
    ��    ��   : Z30971
    �޸�����   : NULL
****************************************************************************/
BOOL CBakTaskMgr::JudgeIsBackuped(const char* szFilePath, int nTaskID)
{
    return (IsAutoBackuped(szFilePath, nTaskID) || IsManualBackuped(szFilePath)); 
}
/*****************************************************************************
 �� �� ��  : GetManTaskInfo
 ��������  : ����ֹ�������Ϣ
 �������  : nClientNo, �ͻ��˿ں� pTaskInfo, ��������ָ��
 �������  : pTaskInfo, �ֹ�������Ϣ
 �� �� ֵ  : BOOL, �ɹ����
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��23��
    ��    ��   : Z30971
    �޸�����   : NULL

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
 �� �� ��  : ReqBakCdrProc
 ��������  : �ֹ����ݻ���
 �������  : MSG_INTRA *pMsg
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : CBakTaskMgr::ReqBakCdrProc 
 ��������  : ParseMsg
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��23��
    ��    ��   : Z30971
    �޸�����   : NULL
��������:
BAK CDR: ap="3G", bt=1, chl="CHL_1", date="20020225",
         flg=1, name="b00000046.dat";
���һ�����Ƶ�ĩβ��һ�������־~
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
  
    //���ݲ������Ҫ���ݵĻ�����·��
    char szBakPath[MAX_PATH];
    
    //����ԭʼ�����¶������Ŀ¼
    if((ORIG_BILL == nCdrType) && (DATE_DIR == nFlg))
    {
        sprintf(szBakPath, "%s/%s", m_szOrigBasePath, szAPName);
    }
    //����ԭʼ����ĳ����Ŀ¼�¶�������ļ�
    else if((ORIG_BILL == nCdrType) && (DATE_DIR != nFlg))
    {
        sprintf(szBakPath, "%s/%s/%s", m_szOrigBasePath,
                szAPName, szDateName);
    }
    //�������ջ���ĳͨ���¶������Ŀ¼
    else if((ORIG_BILL != nCdrType) && (DATE_DIR == nFlg))
    {
        sprintf(szBakPath, "%s/%s/%s", m_szFinalBasePath,
                szAPName, szChlName);
    }
    //�������ջ���ĳͨ��ĳ�����¶�������ļ�
    else
    {
        sprintf(szBakPath, "%s/%s/%s/%s", m_szFinalBasePath, szAPName,
                szChlName, szDateName);
    }

    int nAutoTaskID;
    
    //�õ���Ӧ���Զ������߳�����ID
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
 �� �� ��  : ReqDelCdrProc
 ��������  : �ֹ����ݻ���
 �������  : MSG_INTRA *pMsg
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : ParseMsg
 
 �޸���ʷ      :
  1.��    ��   : 2004��5��14��
    ��    ��   : Z30971
    �޸�����   : NULL
��������:
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
  
    //���ݲ������Ҫ���ݵĻ�����·��
    char szDelPath[MAX_PATH];
    
    //����ԭʼ�����¶������Ŀ¼
    if((ORIG_BILL == nCdrType) && (DATE_DIR == nFlg))
    {
        sprintf(szDelPath, "%s/%s", m_szOrigBasePath, szAPName);
    }
    //����ԭʼ����ĳ����Ŀ¼�¶�������ļ�
    else if((ORIG_BILL == nCdrType) && (DATE_DIR != nFlg))
    {
        sprintf(szDelPath, "%s/%s/%s", m_szOrigBasePath,
                szAPName, szDateName);
    }
    //�������ջ���ĳͨ���¶������Ŀ¼
    else if((ORIG_BILL != nCdrType) && (DATE_DIR == nFlg))
    {
        sprintf(szDelPath, "%s/%s/%s", m_szFinalBasePath,
                szAPName, szChlName);
    }
    //�������ջ���ĳͨ��ĳ�����¶�������ļ�
    else
    {
        sprintf(szDelPath, "%s/%s/%s/%s", m_szFinalBasePath, szAPName,
                szChlName, szDateName);
    }

    int nAutoTaskID;
    //�õ���Ӧ���Զ������߳�����ID
    if(!m_pBakTaskMgr->GetMatchTaskID(szDelPath, nAutoTaskID))
    {
        SendSimpleReport(pResult->GetCmdLine(), 0, S_BACKUP_NOAUTO_TASK, pMsg->ClientNo);
        return;
    }

    m_pBakTaskMgr->ManualDelTaskProc(pMsg->ClientNo, pResult->GetCmdLine(),
                                     nAutoTaskID, szDelPath,
                                     szName, nFlg);
 
}
