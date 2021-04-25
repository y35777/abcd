#include "../include/toolbox.h"
#include "license_file.h"
#include "license_manager.h"
#include "resource.h"
#include "../utility/mml_para.h"
#include "../utility/mml_report.h"

CLicenseManager::CLicenseManager()
{
    //��Ա������ʼ��
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
 ����ԭ��    protected:  int InitInstance(void);
 ��������    ��ʼ���������̳и�����麯����
 �������    ��
 �������    ��
 ����ֵ      ��ʼ���Ƿ�ɹ���
 �׳��쳣    ��
********************************************************************/
int CLicenseManager::InitInstance()
{
    //���ȵ��û����InitInstance()
    CWorkModule::InitInstance();

    time(&m_tStartTime);

    CINIFile* pINIFile = new CINIFile(GetCfgFilePath());
    pINIFile->Open();

    //�������н�������ͣ������AP_PS���ͣ��ٶ���Э�����ͣ�
    //��Ϊ��AP_PS���͵Ľ���㣬License���ƾ�ȷ��R98��R99��
    //R4�Ⱦ����Э��
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
            //�������AP_PS���͵Ľ���㣬�򲻱ع���Э������
            strcpy(ap.szRelease, "NONE");
        }

        m_mapAPRelease[ap.nAPID] = ap;
	}

    //��ȡLicense�ļ���ŵ�Ŀ¼����
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

    //��License��ʱ�������Ĭ��30��
    m_nTimerInterval = pINIFile->GetInt(CFG_SEC_LICENSE, CFG_LICENSE_TIMER_INTERVAL,
                                        DEFAULT_LICENSE_TIMER_INTERVAL);

    //License��ʱ���������С��10�룬Ҳ���ô���10����
    if ((m_nTimerInterval < 10) || (m_nTimerInterval > 600))
    {
        m_nTimerInterval = DEFAULT_LICENSE_TIMER_INTERVAL;
    }

    //���뻻��ɺ���
    m_nTimerInterval *= 1000;


    m_pLicenseFile = new CLicenseFile(m_szLicenseDir);

    if (0 != m_pLicenseFile->Init())
    {
        //License��ʼ��ʧ�ܣ�LicenseMangerӦTRACE���󣬵���Ӧ�˳�
        TRACE(MTS_LICENSE_MANAGER, S_INIT_LICENSE_FAILED);
    }
    else
    {
        TRACE(MTS_LICENSE_MANAGER, S_INIT_LICENSE_SUCCESS);
    }

	for(int j = 0; j < nAPCount; j++)
	{
		//if �ǲɼ������
		char szAPSection[64];
		
		sprintf(szAPSection, "%s%d", CFG_SEC_AP, j+1);
		int nCollectType = pINIFile->GetInt(szAPSection, CFG_AP_KEY_COLTYPE,
			CFG_AP_KEY_COLTYPE_DEFAULT);
		
		//��CollectType����0,��ʾ�ǲɼ������
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
    //����һ����ʱ��
    m_nTimerIDCheckLic = CWorkModule::SetTimer(m_nTimerInterval);

	time(&m_tLastRefreshTime);
    return ERR_SUCCESS;
}

/********************************************************************
 ����ԭ��    protected:  void ExitInstance(void);
 ��������    ģ���˳�ǰ�����������̳���CWorkModule��
 �������    ��
 �������    ��
 ����ֵ      �ޡ�
 �׳��쳣    ��
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
 ����ԭ��    protected:  MODULE_TYPE GetModuleType(void);
 ��������    ���ر�ҵ��ģ���ģ�����͡�
 �������    ��
 �������    ��
 ����ֵ      ��ҵ��ģ���ģ�����͡�
 �׳��쳣    ��
********************************************************************/
MODULE_TYPE CLicenseManager::GetModuleType()
{
    return MT_LICENSE_MANAGER;
}

/********************************************************************
 ����ԭ��    protected:  void ParseMsg(MSG_INTRA* pMsg);
 ��������    ��Ϣ��������
 �������
         pMsg : ���յ���Ϣ֡��
 �������    ��
 ����ֵ      �ޡ�
 �׳��쳣    ��
********************************************************************/
void CLicenseManager::ParseMsg(MSG_INTRA* pMsg)
{
    switch (pMsg->CmdCode)
    {
        //����License
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
		//����License
                case MML_REFRESH_LICENSE:           
		{
                    RefreshLicense(pMsg);
                    time(&m_tLastRefreshTime);
                    TRACE(MTS_LICENSE_MANAGER, S_REFRESH_LICENSE_SUCCESS);
                    break;            
                }
                case MML_SHOW_LICENSE: 
                {     	
                    SendLicenseToClient(pMsg); //��ͻ��˷���License	
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
 ����ԭ��    private:  LICENSE_ID GetLicenseID(PROCESS_TYPE apt,
                                               MODULE_TYPE mt);
 ��������    ���ĳһ��ҵ��ģ���License ID��
 �������
            apt : �������� (����ID)
 �������   mt  : ģ������ (ģ��ID)
 ����ֵ     ��ҵ��ģ���Ӧ��License ID��
 �׳��쳣   ��
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
                //�����PS(Packet Switch)��������ͣ���Ҫ�ж�Э��汾
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

        //��License����ģ�飬ͳһ����INVALID_LICENSE_ID;
        default:
        {
            nLicenseID = INVALID_LICENSE_ID;
            break;
        }
    }

    return nLicenseID;
}

/********************************************************************
 ����ԭ��    protected:  void OnTimer(const long nTimerID);
 ��������    ��ʱ��������
 �������
            nTimerID : ��ʱ��ID
 �������   �ޡ�
 ����ֵ     �ޡ�
 �׳��쳣   ��
********************************************************************/
void CLicenseManager::OnTimer(const long nTimerID)
{
    if (nTimerID == m_nTimerIDCheckLic)
    {
        if (m_pLicenseFile)
        {
			time_t tNow;
			time(&tNow);
			if ((tNow - m_tLastRefreshTime) > 3600) //1Сʱˢ��һ��.
			{
				m_pLicenseFile->Refresh();
				m_tLastRefreshTime = tNow;
			}
            //���License���ڻ���License�Ƿ������͸澯
            if (!m_pLicenseFile->IsValid() || m_pLicenseFile->IsExpired())
            {
                SInnerAlarm alm;
                alm.yAlarmType    = AT_EVENT;
                alm.uAlarmID      = ALARM_ID_LICENSE_MANAGEMENT;
                alm.yAlarmPara[0] = ALARM_PARA_LICENSE_INVALID;//by lx SWPD04546
                SNPRINTF((char *)(&(alm.yAlarmPara[1])), MAX_ALARM_PARA - 1, "%s", S_INVALID_LICENSE);
                SendAlarm(&alm);
            }

            //�������ʱLicense������"License��������"�ĸ澯
            if (m_pLicenseFile->IsTemporary() && !m_pLicenseFile->IsExpired())
            {
                time_t tExpireDate = m_pLicenseFile->GetExpireDate();
                time_t tNow;
                time(&tNow);

                //����Licenseʣ�����Ч����
                int nDay = (tExpireDate - tNow)/(24*3600);
                if ((nDay >= 0) && (nDay <= 7))  //��ǰ7��Ԥ��
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
 ����ԭ��    private:  void ResponseLicense(MSG_INTRA* pMsg);
 ��������    ��Ӧҵ��ģ���License����
 �������
            pMsg : License������Ϣ��
 �������   �ޡ�
 ����ֵ     �ޡ�
 �׳��쳣   ��
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

    //��License�ܿ�ģ��
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
        //���License
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


    //���������/�ɼ�ģ�飬Ӧ�����ٱ�֤60���License����
    if ((pLicResp->nLicenseCount == 0) && (nSenderMid == MT_NETWORK))
    {
        time_t tNow;
        time(&tNow);

        int nDay = (tNow - m_tStartTime)/(24*3600);
        if (nDay < 60)
        {
            pLicResp->nLicenseCount = 1;        //License��������
        }
    }

    //��Ӧҵ��ģ���License����
    CWorkModule::SendMsg(pRes);
}

/********************************************************************
 ����ԭ��    private:  void RefreshLicense(MSG_INTRA* pMsg);
 ��������    ����License�ļ��������
 �������
            pMsg : License�ļ�������Ϣ��
 �������   �ޡ�
 ����ֵ     �ޡ�
 �׳��쳣   ��
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

    Report.AddString(STR_NEWLINE);  //����
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
 ����ԭ��    private:  void SendLicenseToClient(MSG_INTRA* pMsg);
 ��������    ��ͻ��˷���License�ļ���
 �������    pMsg : �ͻ��˻�ȡLicense�ļ���Ϣ��
 �������   �ޡ�
 ����ֵ     �ޡ�
 �׳��쳣   ��
********************************************************************/
void CLicenseManager::SendLicenseToClient(MSG_INTRA* pMsg)
{
    CParseResult pr(pMsg);
    pr.CreateParseResult(pMsg);
    CMMLReport Report(THISMODULE);
    LIST<STRING> InfoTitleList;    //���ı���
    LIST<STRING> InfoContentList;  //��������
    //��������
    InfoTitleList.push_back("  FUNCTION ");
    InfoTitleList.push_back("  ACCOUNT  ");
    InfoTitleList.push_back("EXPIRED DATE");
    
    m_pLicenseFile->GetLicenseToClient(InfoContentList);//���License����
	
    Report.MakeReportHead(); 
    Report.MakeCmdDisplay(pr.GetCmdLine(), 0, 0, THISMODULE);
    Report.AddHoriTable(InfoTitleList, InfoContentList, S_QB_TH_PAGE_INFO);
    Report.AddString(STR_NEWLINE);  //����
    Report.MakeReportTail();	

    //���ͱ���
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
