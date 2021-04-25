#include "../include/toolbox.h"
#include "license_file.h"
#include "license_manager.h"
#include "resource.h"
#include "../utility/mml_para.h"
#include "../utility/mml_report.h"

CLicenseManager::CLicenseManager()
{
    //成员变量初始化
    m_pLicenseFile     = NULL;
    m_nTimerIDCheckLic = -1;
    m_szLicenseDir[0]  = '\0';
    m_nTimerInterval   = DEFAULT_LICENSE_TIMER_INTERVAL;
}

CLicenseManager::~CLicenseManager()
{
    if (m_pLicenseFile)
    {
        delete m_pLicenseFile;
        m_pLicenseFile = NULL;
    }
}

/********************************************************************
 函数原型    protected:  int InitInstance(void);
 功能描述    初始化工作，继承父类的虚函数。
 输入参数    无
 输出参数    无
 返回值      初始化是否成功。
 抛出异常    无
********************************************************************/
int CLicenseManager::InitInstance()
{
    //首先调用基类的InitInstance()
    CWorkModule::InitInstance();

    time(&m_tStartTime);

    CINIFile* pINIFile = new CINIFile(GetCfgFilePath());
    pINIFile->Open();

    //读出所有接入点类型，如果是AP_PS类型，再读出协议类型，
    //因为对AP_PS类型的接入点，License控制精确到R98、R99、
    //R4等具体的协议
    int nAPCount = 1;
    nAPCount = pINIFile->GetInt(CFG_SEC_COMMON,
                                CFG_COMMON_KEY_APCOUNT,
                                CFG_COMMON_KEY_APCOUNT_DEFAULT);
    for (int i = 0; i < nAPCount; i++)
    {
        char szAPSection[64];
        SAPRelease ap;

        ap.nAPID = i + 1;
        sprintf(szAPSection, "AccessPoint%d", ap.nAPID);
        ap.nAPType = pINIFile->GetInt(szAPSection, CFG_AP_KEY_APTYPE,
                                           CFG_AP_KEY_APTYPE_DEFAULT);
        if (ap.nAPType == AP_PS || ap.nAPType == AP_PS_RESERVED)
        {
            char szRelease[64];
            pINIFile->GetString(szAPSection, CFG_AP_KEY_RELEASE_VERSION,
                                CFG_AP_KEY_RELEASE_R98,
                                szRelease, 64);
            strncpy(ap.szRelease, szRelease, 64);
            ap.szRelease[63] = '\0';
        }
        else
        {
            //如果不是AP_PS类型的接入点，则不必关心协议类型
            strcpy(ap.szRelease, "NONE");
        }

        m_mapAPRelease[ap.nAPID] = ap;
	}

    //读取License文件存放的目录参数
    pINIFile->GetString(CFG_SEC_LICENSE,
                        CFG_LICENSE_DIR,
                        DEFAULT_LICENSE_DIR,
                        m_szLicenseDir,
                        sizeof(m_szLicenseDir));
    if(m_szLicenseDir[0] == '\0')
    {
        SNPRINTF(m_szLicenseDir,
                 sizeof(m_szLicenseDir),
                 "%s/config/license",
                 GetAppPath());
        m_szLicenseDir[sizeof(m_szLicenseDir) - 1] = '\0';
    }

    //读License定时器间隔，默认30秒
    m_nTimerInterval = pINIFile->GetInt(CFG_SEC_LICENSE, CFG_LICENSE_TIMER_INTERVAL,
                                        DEFAULT_LICENSE_TIMER_INTERVAL);

    //License定时器间隔不得小于10秒，也不得大于10分钟
    if ((m_nTimerInterval < 10) || (m_nTimerInterval > 600))
    {
        m_nTimerInterval = DEFAULT_LICENSE_TIMER_INTERVAL;
    }

    //将秒换算成毫秒
    m_nTimerInterval *= 1000;


    m_pLicenseFile = new CLicenseFile(m_szLicenseDir);

    if (0 != m_pLicenseFile->Init())
    {
        //License初始化失败，LicenseManger应TRACE错误，但不应退出
        TRACE(MTS_LICENSE_MANAGER, S_INIT_LICENSE_FAILED);
    }
    else
    {
        TRACE(MTS_LICENSE_MANAGER, S_INIT_LICENSE_SUCCESS);
    }

	for(int j = 0; j < nAPCount; j++)
	{
		//if 是采集点进程
		char szAPSection[64];
		
		sprintf(szAPSection, "%s%d", CFG_SEC_AP, j+1);
		int nCollectType = pINIFile->GetInt(szAPSection, CFG_AP_KEY_COLTYPE,
			CFG_AP_KEY_COLTYPE_DEFAULT);
		
		//当CollectType大于0,表示是采集接入点
		if (nCollectType)
		{
			m_pLicenseFile->m_LicenseVector.push_back(LICENSE_ID_CO);
		}
		else
		{
			m_pLicenseFile->m_LicenseVector.push_back(GetLicenseID((PROCESS_TYPE)(PT_AP_BASE+j), MT_NETWORK));
		}

	}
	
    delete pINIFile, pINIFile = NULL;
    //创建一个定时器
    m_nTimerIDCheckLic = CWorkModule::SetTimer(m_nTimerInterval);

	time(&m_tLastRefreshTime);
    return ERR_SUCCESS;
}

/********************************************************************
 函数原型    protected:  void ExitInstance(void);
 功能描述    模块退出前的清理工作，继承于CWorkModule。
 输入参数    无
 输出参数    无
 返回值      无。
 抛出异常    无
********************************************************************/
void CLicenseManager::ExitInstance()
{
    if (m_nTimerIDCheckLic != -1)
    {
        CWorkModule::KillTimer(m_nTimerIDCheckLic);
    }

    CWorkModule::ExitInstance();
}

/********************************************************************
 函数原型    protected:  MODULE_TYPE GetModuleType(void);
 功能描述    返回本业务模块的模块类型。
 输入参数    无
 输出参数    无
 返回值      本业务模块的模块类型。
 抛出异常    无
********************************************************************/
MODULE_TYPE CLicenseManager::GetModuleType()
{
    return MT_LICENSE_MANAGER;
}

/********************************************************************
 函数原型    protected:  void ParseMsg(MSG_INTRA* pMsg);
 功能描述    消息处理函数。
 输入参数
         pMsg : 接收的消息帧。
 输出参数    无
 返回值      无。
 抛出异常    无
********************************************************************/
void CLicenseManager::ParseMsg(MSG_INTRA* pMsg)
{
    switch (pMsg->CmdCode)
    {
        //请求License
        case CMD_LICENSE_REQUEST:
        {
            ResponseLicense(pMsg);
            break;
        }

        case CMD_MML:
        {
            CParseResult pr(pMsg);
            pr.CreateParseResult(pMsg);
            //added by zkq 2004-6-18			
            switch (pr.GetCmdCode())
            {
		//更新License
                case MML_REFRESH_LICENSE:           
		{
                    RefreshLicense(pMsg);
                    time(&m_tLastRefreshTime);
                    TRACE(MTS_LICENSE_MANAGER, S_REFRESH_LICENSE_SUCCESS);
                    break;            
                }
                case MML_SHOW_LICENSE: 
                {     	
                    SendLicenseToClient(pMsg); //向客户端返回License	
                    break;
                }
                default:
                {
                   break;	
                }	
            }
            //end																																											            
        }
        default:
        {
           break;
        }
    }
}

/********************************************************************
 函数原型    private:  LICENSE_ID GetLicenseID(PROCESS_TYPE apt,
                                               MODULE_TYPE mt);
 功能描述    获得某一个业务模块的License ID。
 输入参数
            apt : 进程类型 (进程ID)
 输出参数   mt  : 模块类型 (模块ID)
 返回值     本业务模块对应的License ID。
 抛出异常   无
********************************************************************/
LICENSE_ID CLicenseManager::GetLicenseID(PROCESS_TYPE apt,
                                         MODULE_TYPE mt)
{
    LICENSE_ID nLicenseID = INVALID_LICENSE_ID;

    switch (mt)
    {
        case MT_BACKUP:
        {
            nLicenseID = LICENSE_ID_NETBACKUP;
            break;
        }

        case MT_CDRSTAT:
        {
            nLicenseID = LICENSE_ID_CDRSTAT;
            break;
        }

        case MT_NETWORK:
        {
            char szRelease[64] = "";
            int nAPID = apt - PT_AP_BASE + 1;
            int nAPType = -1;

            MAP_AP_RELEASE::iterator it = m_mapAPRelease.find(nAPID);
            if (it != m_mapAPRelease.end())
            {
                strncpy(szRelease, (*it).second.szRelease, 64);
                szRelease[63] = '\0';
                nAPType = it->second.nAPType;
            }

            switch (nAPType)
            {
                //如果是PS(Packet Switch)接入点类型，则还要判断协议版本
                case AP_PS:
                case AP_PS_RESERVED:
                {
                    if (strcmp(szRelease, CFG_AP_KEY_RELEASE_R98) == 0)
                    {
                        nLicenseID = LICENSE_ID_R98;
                    }
                    else if (strcmp(szRelease, CFG_AP_KEY_RELEASE_R99) == 0)
                    {
                        nLicenseID = LICENSE_ID_R99;
                    }
                    else if (strcmp(szRelease, CFG_AP_KEY_RELEASE_R4) == 0)
                    {
                        nLicenseID = LICENSE_ID_R4;
                    }
                    else
                    {
                        nLicenseID = INVALID_LICENSE_ID;
                    }

                    break;
                }

                case AP_128:
                {
                    nLicenseID = LICENSE_ID_CC08;
                    break;
                }

                case AP_GSM:
                {
                    nLicenseID = LICENSE_ID_MSC;
                    break;
                }

                case AP_STP:
                {
                    nLicenseID = LICENSE_ID_STP;
                    break;
                }

                case AP_SOFTX:
                {
                    nLicenseID = LICENSE_ID_S3K;
                    break;
                }

                case AP_WLAN:
                {
                    nLicenseID = LICENSE_ID_WLAN;
                    break;
                }

                default:
                {
                    nLicenseID = INVALID_LICENSE_ID;
                    break;
                }
            }

            break;
        }

        case MT_COLLECTOR:
        {
            nLicenseID = LICENSE_ID_CO;

            break;
        }

        //非License控制模块，统一返回INVALID_LICENSE_ID;
        default:
        {
            nLicenseID = INVALID_LICENSE_ID;
            break;
        }
    }

    return nLicenseID;
}

/********************************************************************
 函数原型    protected:  void OnTimer(const long nTimerID);
 功能描述    定时器任务处理。
 输入参数
            nTimerID : 定时器ID
 输出参数   无。
 返回值     无。
 抛出异常   无
********************************************************************/
void CLicenseManager::OnTimer(const long nTimerID)
{
    if (nTimerID == m_nTimerIDCheckLic)
    {
        if (m_pLicenseFile)
        {
			time_t tNow;
			time(&tNow);
			if ((tNow - m_tLastRefreshTime) > 3600) //1小时刷新一次.
			{
				m_pLicenseFile->Refresh();
				m_tLastRefreshTime = tNow;
			}
            //如果License过期或者License非法，则发送告警
            if (!m_pLicenseFile->IsValid() || m_pLicenseFile->IsExpired())
            {
                SInnerAlarm alm;
                alm.yAlarmType    = AT_EVENT;
                alm.uAlarmID      = ALARM_ID_LICENSE_MANAGEMENT;
                alm.yAlarmPara[0] = ALARM_PARA_LICENSE_INVALID;//by lx SWPD04546
                SNPRINTF((char *)(&(alm.yAlarmPara[1])), MAX_ALARM_PARA - 1, "%s", S_INVALID_LICENSE);
                SendAlarm(&alm);
            }

            //如果是临时License，则发送"License即将过期"的告警
            if (m_pLicenseFile->IsTemporary() && !m_pLicenseFile->IsExpired())
            {
                time_t tExpireDate = m_pLicenseFile->GetExpireDate();
                time_t tNow;
                time(&tNow);

                //计算License剩余的有效天数
                int nDay = (tExpireDate - tNow)/(24*3600);
                if ((nDay >= 0) && (nDay <= 7))  //提前7天预警
                {
                    SInnerAlarm alm;
                    alm.yAlarmType = AT_EVENT;
                    alm.uAlarmID   = ALARM_ID_LICENSE_MANAGEMENT;
                    alm.yAlarmPara[0] = ALARM_PARA_LICENSE_EXPIRE;//by lx SWPD04546
                    SNPRINTF((char *)(&(alm.yAlarmPara[1])), MAX_ALARM_PARA - 1, "%s(%s)",
                             S_TEMPORARY_LICENSE, ctime(&tExpireDate));
                    SendAlarm(&alm);
                }
            }
        }
    }
}

/********************************************************************
 函数原型    private:  void ResponseLicense(MSG_INTRA* pMsg);
 功能描述    响应业务模块的License请求。
 输入参数
            pMsg : License请求消息。
 输出参数   无。
 返回值     无。
 抛出异常   无
********************************************************************/
void CLicenseManager::ResponseLicense(MSG_INTRA* pMsg)
{
    if (!pMsg || (pMsg->CmdCode != CMD_LICENSE_REQUEST))
    {
        return;
    }
    
    MSG_INTRA* pRes   = new (sizeof(SLicenseResp)) MSG_INTRA;
    pRes->SenderPid   = GetProcessType();
    pRes->SenderMid   = GetModuleType();
    pRes->ReceiverPid = pMsg->SenderPid;
    pRes->ReceiverMid = pMsg->SenderMid;
    pRes->CmdCode     = CMD_LICENSE_RESPONSE;

    SLicenseResp* pLicResp = (SLicenseResp *)pRes->Data;

    BYTE nSenderPid = pMsg->SenderPid;
    BYTE nSenderMid = pMsg->SenderMid;

    //非License受控模块
    if ((nSenderMid != MT_COLLECTOR)
        && (nSenderMid != MT_BACKUP)
        && (nSenderMid != MT_CDRSTAT)
        && (nSenderMid != MT_NETWORK))
    {
        pLicResp->nLicenseCount   = 1;
    }
    else if (!m_pLicenseFile)
    {
        pLicResp->nLicenseCount   = 0;
    }
	else  if (!m_pLicenseFile->IsValid() || m_pLicenseFile->IsExpired())
	{
		pLicResp->nLicenseCount = 0;
	}
    else
    {
        //获得License
        LICENSE_ID nLicenseID = GetLicenseID((PROCESS_TYPE)nSenderPid,
                                             (MODULE_TYPE)nSenderMid);

        if ((nLicenseID == LICENSE_ID_CDRSTAT) || (nLicenseID == LICENSE_ID_NETBACKUP))
        {
            pLicResp->nLicenseCount = m_pLicenseFile->GetLicense(nLicenseID, ACCESS_POINT_ANY);
        }
        else
        if(LICENSE_ID_CO == nLicenseID)
        {
            int  nAPID = nSenderPid - PT_CO_BASE + 1;
            pLicResp->nLicenseCount = m_pLicenseFile->GetLicense(nLicenseID, nAPID);
        }
        else
        {
            int  nAPID = nSenderPid - PT_AP_BASE + 1;
            pLicResp->nLicenseCount = m_pLicenseFile->GetLicense(nLicenseID, nAPID);
        }
    }


    //如果是网络/采集模块，应该至少保证60天的License可用
    if ((pLicResp->nLicenseCount == 0) && (nSenderMid == MT_NETWORK))
    {
        time_t tNow;
        time(&tNow);

        int nDay = (tNow - m_tStartTime)/(24*3600);
        if (nDay < 60)
        {
            pLicResp->nLicenseCount = 1;        //License最少数量
        }
    }

    //响应业务模块的License请求
    CWorkModule::SendMsg(pRes);
}

/********************************************************************
 函数原型    private:  void RefreshLicense(MSG_INTRA* pMsg);
 功能描述    处理License文件更新命令。
 输入参数
            pMsg : License文件更新消息。
 输出参数   无。
 返回值     无。
 抛出异常   无
********************************************************************/
void CLicenseManager::RefreshLicense(MSG_INTRA* pMsg)
{
    if (NULL == m_pLicenseFile)
    {
        m_pLicenseFile = new CLicenseFile(m_szLicenseDir);
    }

    int nRet;

    nRet = m_pLicenseFile->Refresh();

    CParseResult pr(pMsg);
    pr.CreateParseResult(pMsg);

    CMMLReport Report(THISMODULE);
    Report.MakeReportHead();
    Report.MakeCmdDisplay(pr.GetCmdLine(), 0, nRet, THISMODULE);

    if (0 == nRet)
    {
        TRACE(MTS_LICENSE_MANAGER, S_REFRESH_LICENSE_SUCCESS);
        Report.AddString(S_REFRESH_LICENSE_SUCCESS);
    }
    else
    {
        TRACE(MTS_LICENSE_MANAGER, S_REFRESH_LICENSE_FAILED);
        Report.AddString(S_REFRESH_LICENSE_FAILED);
    }

    Report.AddString(STR_NEWLINE);  //换行
    Report.MakeReportTail();

    const char* pChar = Report.GetReport();

    int nLen = strlen(pChar);
    MSG_INTRA* pAns = new (nLen)MSG_INTRA;

    memcpy((void *)(pAns->Data), pChar, nLen);
    pAns->Data[nLen - 1] = '\0';

    pAns->SenderPid   = GetProcessType();
    pAns->SenderMid   = GetModuleType();
    pAns->ReceiverPid = PT_OM;
    pAns->ReceiverMid = MT_MMLDEBUG;
    pAns->AppType     = pMsg->AppType;
    pAns->ClientNo    = pMsg->ClientNo;
    pAns->IPAddr      = pMsg->IPAddr;

    SendMsg(pAns);
}
/********************************************************************
 函数原型    private:  void SendLicenseToClient(MSG_INTRA* pMsg);
 功能描述    向客户端返回License文件。
 输入参数    pMsg : 客户端获取License文件消息。
 输出参数   无。
 返回值     无。
 抛出异常   无
********************************************************************/
void CLicenseManager::SendLicenseToClient(MSG_INTRA* pMsg)
{
    CParseResult pr(pMsg);
    pr.CreateParseResult(pMsg);
    CMMLReport Report(THISMODULE);
    LIST<STRING> InfoTitleList;    //报文标题
    LIST<STRING> InfoContentList;  //报文内容
    //制作报文
    InfoTitleList.push_back("  FUNCTION ");
    InfoTitleList.push_back("  ACCOUNT  ");
    InfoTitleList.push_back("EXPIRED DATE");
    
    m_pLicenseFile->GetLicenseToClient(InfoContentList);//获得License内容
	
    Report.MakeReportHead(); 
    Report.MakeCmdDisplay(pr.GetCmdLine(), 0, 0, THISMODULE);
    Report.AddHoriTable(InfoTitleList, InfoContentList, S_QB_TH_PAGE_INFO);
    Report.AddString(STR_NEWLINE);  //换行
    Report.MakeReportTail();	

    //发送报文
    const char* szReport = Report.GetReport();
    unsigned int nStrLen = strlen(szReport);    

    MSG_INTRA* pAns = new(nStrLen + 1) MSG_INTRA; 
    strcpy((char *)(pAns->Data), szReport);
    strcpy(pAns->UserName, pMsg->UserName);
    pAns->SenderPid   = GetProcessType();
    pAns->SenderMid   = GetModuleType();
    pAns->ReceiverPid = PT_OM;
    pAns->ReceiverMid = MT_MMLDEBUG;
    pAns->AppType     = pMsg->AppType;
    pAns->ClientNo    = pMsg->ClientNo;
    pAns->IPAddr      = pMsg->IPAddr;
    
    SendMsg(pAns); 
} 
