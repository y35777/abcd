#include "../include/frame.h"
#include "../include/toolbox.h"
#include "../include/format_out.h"
#include "scp_rvy_task.h"
#include "bill_task_struct.h"
#include "scp_file_group.h"
#include "../utility/mml_para.h"
#include "../utility/mml_report.h"
#include "resource.h"
#include "../querybrowse/qb_comm.h"

/********************************************************
* ����: CScpRvyTask::CScpRvyTask
* ����: ���캯��
* ����: nAP     : ������
*       pApply  : SCP�����ָ�������Ϣ
*       pCmd    : ���ڷ��ر������Ϣ
*       nTaskID : �����
* ���: 
* ����: 
********************************************************/
CScpRvyTask::CScpRvyTask(int nAP, 
                         const SSCPBillRcvrApply* pApply, 
                         const SRcvrCmd* pCmd, 
                         int nTaskID)
                         :CBillTask(nTaskID)
{
    memcpy(&m_ScpApply, pApply, sizeof(m_ScpApply));
    memcpy(&m_RcvrCmd, pCmd, sizeof(m_RcvrCmd));
    memset(&m_EcptBillRec, 0, sizeof(SEcptBillRec)); 
    
    m_nAP               = nAP;
    m_pIGWB             = NULL;
    m_pIQueryBrowse     = NULL;
    m_pFormat           = NULL;
    m_strBackChlPath    = "";
    m_strSCPRvyRoot     = "";
    m_strFormatName     = SCP_QUERY_DST_FMT;
    m_nTaskType         = TK_SCP_RVY;
	m_strLocalIp        = "";
}

/********************************************************
* ����: CScpRvyTask::~CScpRvyTask
* ����: ��������
* ����: 
* ���: 
* ����: 
********************************************************/
CScpRvyTask::~CScpRvyTask()
{
    m_pIQueryBrowse = NULL;
    m_pFormat       = NULL;

    //�ͷŸ�ʽ�����
    if(NULL != m_pIGWB)
    {
        delete m_pIGWB;
        m_pIGWB = NULL;
    }

    if(m_RcvrCmd.szCmdLine != NULL)
    {
        delete m_RcvrCmd.szCmdLine;
        m_RcvrCmd.szCmdLine = NULL;
    }

    //ɾ������SCP�����ָ�����Ŀ¼
    if(!m_strSCPRvyRoot.empty())
    {
        SweepDirectory((char*)m_strSCPRvyRoot.c_str());
    }
}

/********************************************************
* ����: CScpRvyTask::Init
* ����: ��ʼ��
* ����: 
* ���: 
* ����: 
********************************************************/
int CScpRvyTask::Init()
{
    //��ʼ����ʽ��ӿ�
    STRING strIGWBPath = STRING(GetAppPath()) +
                         "/config/format";
    m_pIGWB = new CIGWB(strIGWBPath);
    if(NULL == m_pIGWB)
    {
        TRACE(MTS_BILLTASK, S_QB_TRC_FAIL_INIT_FORMAT);
        throw BR_SYSECPT;
    }

    //��ø�ʽ���ѯ�ӿ�
    m_pIQueryBrowse = (IQueryBrowse*)m_pIGWB->QueryInterface(IID_FP_IQUERYBROWSE);
	if (NULL == m_pIQueryBrowse)
    {
        TRACE(MTS_BILLTASK, S_QB_TRC_FAIL_INIT_FORMAT);
        throw BR_SYSECPT;
    }
    int nResult = m_pIQueryBrowse->Init(0);
    if (0 != nResult)
    {
        TRACE(MTS_BILLBROWSER, S_QB_TRC_FAIL_INIT_FORMAT);
        throw BR_SYSECPT;
    }

    //���SCP�����ָ���ʽ��
    m_pFormat = m_pIQueryBrowse->GetBrowseFmt(m_strFormatName);
	if (NULL == m_pFormat)
    {
        TRACE(MTS_BILLBROWSER, S_QB_TRC_FAIL_INIT_FORMAT);
        throw BR_SYSECPT;
    }

    //ȡm_nAP���������������ʱ����STRING strAPName
    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();

    char szAPSec[25];
    SNPRINTF(szAPSec, 25, "%s%d", CFG_SEC_AP, m_nAP);
    szAPSec[24] = '\0';

    char szAPName[MAX_PATH];
    IniFile.GetString(szAPSec, CFG_AP_NAME, szAPSec,
                      szAPName, MAX_PATH);
    szAPName[MAX_PATH - 1] = '\0';

    //ȡm_nAP������º���������ܻ�����ͨ����
	STRING strBackChlName = "";
    STRING strSrcFmtName = "";
    m_pFormat->GetFormatGen(strBackChlName, strSrcFmtName);

    //ȡm_nAP������º���������ܻ�����ȫ·����
    char szBackRoot[MAX_PATH];
    IniFile.GetString(CFG_SEC_DISK_FILE, CFG_BACK_SAVE_ROOT_DIR, 
                      CFG_BACK_SAVE_ROOT_DIR_DEFAULT,
                      szBackRoot, MAX_PATH);
    szBackRoot[MAX_PATH - 1] = '\0';
    m_strBackChlPath = STRING(szBackRoot) + "/" + 
                       STRING(szAPName) + "/" + strBackChlName;

    //��SCP�����ָ��ڱ��صĹ���Ŀ¼ȫ·����
    char szTaskID[10];
    sprintf(szTaskID, "%d", m_nTaskID);
    m_strSCPRvyRoot = STRING(GetAppPath()) + "/scp_recovery/" + 
                      STRING(szTaskID);
    if(!CreateDeepDir(m_strSCPRvyRoot.c_str()))
    {
        TRACE(MTS_BILLTASK, S_SCP_TASK_CREATE_ROOT,
              m_strSCPRvyRoot.c_str());
        throw BR_SYSECPT;
    }

    //��SCP�쳣������ȫ·����
    m_strSCPErrBill = m_strSCPRvyRoot + "/" + STRING(m_ScpApply.szFileName);

    //��SCP�ָ�������ȫ·����
    m_strSCPRvyBill = m_strSCPRvyRoot + "/" + "OMC" + 
                      STRING((char*)(m_ScpApply.szFileName + 3));

	//��ȡ����IP
	char szLocalIp[MAX_PATH];
	IniFile.GetString(CFG_SEC_MML, 
                      CFG_MML_KEY_LOCAL_IP,
                      CFG_MML_KEY_LOCAL_IP_DEFAULT,
					  szLocalIp, MAX_PATH);
	szLocalIp[MAX_PATH - 1] = '\0';
	m_strLocalIp = STRING(szLocalIp);

    //��ʼ��m_CodeKeyList
    STRING strCodeKeyPath = STRING(GetAppPath()) + 
                            S_SCP_PATH_BASE_ON_APP_PATH;
    CINIFile CodeKeyFile(strCodeKeyPath.c_str());
    if(!CodeKeyFile.Open())
    {
        throw BR_CODEKEYFAIL;
    }

    //������m_nAP���������CodeKeyCount,������ʱ����int nCodeKeyCount
    int nCodeKeyCount;
    nCodeKeyCount = CodeKeyFile.GetInt(szAPSec, 
                                       CFG_AP_KEY_CODE_KEY_COUNT,
                                       CFG_AP_KEY_CODE_KEY_COUNT_DEFAULT);

    int i;
    char szCodeKey[100];
    char szCfgCodeKeyName[25];
	for(i = 0; i < nCodeKeyCount; i++)
	{
		SCodeKey ck;

		//����"CodeKey"+i�е������������ʱ����char szCodeKey[100];
        SNPRINTF(szCfgCodeKeyName, 25, "%s%d", CFG_AP_KEY_CODE_KEY, i);
        szCfgCodeKeyName[24] = '\0';
        CodeKeyFile.GetString(szAPSec, szCfgCodeKeyName, 
                              CFG_AP_KEY_CODE_KEY_DEFAULT,
                              szCodeKey, 100);
    
		//��szCodeKey��','ǰ���ַ���ȥ�����ҿո�󣬸���ck.szAccCode
		//��szCodeKey��','����ַ���ȥ�����ҿո�ת��Ϊ�����󣬸���ck.nServKey;
        char* p = NULL; 
        if((p = strchr(szCodeKey, ',')) != NULL) 
        {
            *p = '\0';
            StringLRTrim(p + 1);
            ck.nServKey = atoi(p + 1);
        
            StringLRTrim(szCodeKey);
            strncpy(ck.szAccCode, szCodeKey, MAX_NUM_LEN + 1);
            ck.szAccCode[MAX_NUM_LEN] = '\0';

		    m_CodeKeyList.push_back(ck);
        }
	}

    return BR_ACCEPTED;
}

/********************************************************
* ����: CScpRvyTask::IsMatchedTaskk
* ����: ͨ����Ϣ֡�ж��Ƿ���ͬһ����,ֻ��Ҫ�Ƚ����������롢
*       IP��ַ��Ŀ¼�����ļ������ɣ�ע�����Ҫ�ж������룬
*       ��Ϊ�����б��п����ж������񣬷�֮���������������
*       ���ܵ��ñ������������Ƿ���ͬһ����
* ����: pMsg : ��SCP��������������
* ���: 
* ����: 
********************************************************/
BOOL CScpRvyTask::IsMatchedTask(MSG_INTRA* pMsg)
{
    //�ж��Ƿ���MML����
    if(CMD_MML != pMsg->CmdCode)
    {
        return FALSE;
    }

    //�ж�MML�����Ƿ���MML_SCP_RECOVERY
    CParseResult* pParseResult = *((CParseResult**)(pMsg->Data));
    if(MML_SCP_RECOVERY != pParseResult->GetCmdCode())
    {
        return FALSE;
    }

    char* szDir      = (char* )pParseResult->GetStringVal(3);
    char* szFileName = (char* )pParseResult->GetStringVal(4);
    char* szIP       = (char* )pParseResult->GetStringVal(5);

    return (strcmp(szDir, m_ScpApply.szDir) == 0) &&
           (strcmp(szFileName, m_ScpApply.szFileName) == 0) &&
           (strcmp(szIP, m_ScpApply.szIP) == 0);
}

/********************************************************
* ����: CScpRvyTask::StrToTm
* ����: ��������ַ���ת��Ϊtm�ṹ
* ����: szSrc : ʱ��Դ�ַ�������ʽΪ
* ���: tmTar : Ŀ��ṹ
* ����: �Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CScpRvyTask::StrToTm(const char* szSrc, tm &tmTar)
{
	int nYear, nMon, nDay, nHour, nMin, nSec;
	char szTemp[10];

	if(NULL == szSrc)
    {
		return -1;
    }

    //�ֽ�����ƥ�䣬�򷵻�ʧ��
    if(strlen(szSrc) != 14)
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

	//ȡʱ
	memcpy(szTemp, szSrc+8, 2);
	szTemp[2] = 0;
	nHour = atoi(szTemp);

	//ȡ��
	memcpy(szTemp, szSrc+10, 2);
	szTemp[2] = 0;
	nMin = atoi(szTemp);

	//ȡ��
	memcpy(szTemp, szSrc+12, 2);
	szTemp[2] = 0;
	nSec = atoi(szTemp);

	tmTar.tm_year = nYear - 1900;
	tmTar.tm_mon  = nMon - 1;
	tmTar.tm_mday = nDay;
	tmTar.tm_hour = nHour;
	tmTar.tm_min  = nMin;
	tmTar.tm_sec  = nSec;

	return 0;
}

/********************************************************
* ����: CScpRvyTask::IsMatchedScpBill
* ����: �жϻ����Ƿ���������뼰ҵ��������Ľӿ�
* ����: pData : ��������ָ��
*       nLen  : ��������
* ���: 
* ����: �Ƿ�����������TRUE��ʾ����
********************************************************/
BOOL CScpRvyTask::IsMatchedScpBill(BYTE* pData, int nLen)
{
    //��ȡ�û����ı��к���
	char szCalledNumber[50];
	void* pFieldValue = NULL;
	UINT4 uFieldLen = 0;

    ASSERT(NULL != m_pFormat);

	m_pFormat->GetFieldValueByName(pData, TRUE,
                                   S_SCP_CALLER_NUMBER, 
                                   pFieldValue, uFieldLen);
	if(uFieldLen == 0)
	{
		return FALSE;
	}

	uFieldLen = min(uFieldLen, 50);
	memcpy(szCalledNumber, pFieldValue, uFieldLen);
	szCalledNumber[uFieldLen - 1] = '\0';
	StringLRTrim(szCalledNumber);

	//��m_CodeKeyList�в��ҽ�������ڸñ��к������
	LIST<SCodeKey>::iterator i;
	for (i = m_CodeKeyList.begin(); i != m_CodeKeyList.end(); i++)
	{
		//���ұ��к���һ�µ�SCodeKey�ṹ
	    //��������Ӧ��ҵ�������m_EcptBillRec�е�nServKey
        //�򷵻�TRUE�����򷵻�FALSE
        if(0 == ACE_OS::strncasecmp((*i).szAccCode, szCalledNumber, uFieldLen))
		{
			if(m_EcptBillRec.nServKey == (*i).nServKey)
			{
				delete pFieldValue;
				return TRUE;
			}
		}
	}

	delete pFieldValue;
	return FALSE;
}

#define ONE_LINE 512
/********************************************************
* ����: CScpRvyTask::IsMatchedScpBill
* ����: ʵʩ�ض�����
* ����: 
* ���: 
* ����: 
********************************************************/
void CScpRvyTask::ProcBillTask()
{
    int nRetCode   = 0;
    FILE* fSrcFile = NULL;
    FILE* fDstFile = NULL;

    try
    {
        int nRet = 0;

        //ͨ��FTP��SCP��ȡ�쳣����		
		nRet = m_FtpClient.Connect(m_ScpApply.szIP, 
                                   m_ScpApply.szOp, 
                                   m_ScpApply.szPwd, (char *)m_strLocalIp.c_str());
		

        //�����ļ�����ģʽ
		nRet = m_FtpClient.SetTransMode(FTP_TRANS_ASCII);
        if(nRet != 0) throw BR_FTP_FAIL;

  
        //���õ�ǰĿ¼
		nRet = m_FtpClient.SetDirectory(m_ScpApply.szDir);
        if(nRet != 0) throw BR_FTP_FAIL;
        
		//ȡ�ļ�
		nRet = m_FtpClient.GetFile(m_strSCPErrBill.c_str(),
								m_ScpApply.szFileName);
        if(nRet != 0) throw BR_FTPGETFAIL;
		

        //��Ŀ���ļ�
        fDstFile = fopen(m_strSCPRvyBill.c_str(), "w");
        if(NULL == fDstFile) throw BR_RCVRFILECRTFAIL;

        //���ļ��ж���ÿһ���쳣������Ϣ��������֮
        fSrcFile = fopen(m_strSCPErrBill.c_str(), "r");
        if(NULL == fSrcFile) throw BR_ECPTFILEOPENFAIL;
		
        char szLine[ONE_LINE];
		while(fgets(szLine, ONE_LINE, fSrcFile)	!= NULL)
		{
			//�Ӷ�����һ���ж���SEcptBillRec��Ϣ
			if(6 != sscanf(szLine, "%d %d %s %s %s %d", 
                           &m_EcptBillRec.nRecNo, 
                           &m_EcptBillRec.nServKey, 
                           m_EcptBillRec.szCallingNum, 
                           m_EcptBillRec.szTransNum, 
                           m_EcptBillRec.szDateTime, 
                           &m_EcptBillRec.nDecCost) )
			{
				continue;
			}
            m_EcptBillRec.szCallingNum[MAX_NUM_LEN + 1] = '\0';
            m_EcptBillRec.szTransNum[MAX_NUM_LEN + 1] = '\0';
            m_EcptBillRec.szDateTime[14] = '\0';
			//��ʱ����ַ���ת��Ϊtm�ṹ
			if(StrToTm(m_EcptBillRec.szDateTime, 
                       m_EcptBillRec.tmStartTime) != 0)
			{
				continue;
			}
            
            //����ÿһ���쳣����
			ProcOneQueryTask(fDstFile);
			
		}
		        
		if(FileSize(fDstFile) == 0) throw BR_NOBILLRECMATCH;

		//�ͷ���Դ
		fflush(fDstFile);
		fclose(fDstFile);
        fDstFile = NULL;

		fclose(fSrcFile);
        fSrcFile = NULL;
               
	    //��SCP�����ļ�
		nRetCode = BR_FTPPUTFAIL;
        STRING strSCPRvyBill = "OMC" + 
                               STRING((char*)(m_ScpApply.szFileName + 3));
		int nSendLoop = 3;
		while(nSendLoop-- > 0)
		{
			nRet = m_FtpClient.PutFile(m_strSCPRvyBill.c_str(),
                                       strSCPRvyBill.c_str());

            //����ɹ�����nRetCode=0������break;
            if(0 == nRet)
            {
                nRetCode = 0;
                break;
            }
            else if(nSendLoop > 0)
            {
                //���FTP���ļ�ʧ�ܣ���ȴ�5���������
                ACE_OS::sleep(5);
            }   
		}
	}
	//�����쳣
	catch(EBill_RVY_RETCODE nErrorCode)
	{
		if(NULL != fDstFile)
        {
            fclose(fDstFile);
            fDstFile = NULL;
        }

		if(NULL != fSrcFile)
        {
            fclose(fSrcFile);
            fSrcFile = NULL;
        }

		nRetCode = nErrorCode;
	}

	//��SCP��Ӧ��
    CMMLReport Report(S_BT_MML_TITLE);
    Report.MakeReportHead();
    Report.MakeCmdDisplay(m_RcvrCmd.szCmdLine, 0, nRetCode, S_BT_MML_SUBTITLE);
    Report.MakeReportItem(S_QB_MML_DPC, m_ScpApply.szDpc);
    Report.MakeReportTail();

    const char* pMMLReport  = Report.GetReport();
    int nLen          = strlen(pMMLReport) + 1;
    MSG_INTRA* pAns   = new(nLen) MSG_INTRA;
    if(NULL != pAns)
    {
        pAns->SenderPid   = PT_OM;
        pAns->SenderMid   = MT_BILLTASK;
        pAns->ReceiverPid = PT_OM;
        pAns->ReceiverMid = MT_MMLDEBUG;
        pAns->AppType     = m_RcvrCmd.uAppType;
        pAns->ClientNo    = m_RcvrCmd.uClientNo;

        memcpy(pAns->Data, pMMLReport, nLen);
        CMsgProcessor::SendMsg(pAns);
    }

    if(0 == nRetCode)
    {
        MSGOUT(MTS_BILLTASK, MSGOUT_LEVEL_IMPORTANT, 
               S_SCP_TASK_SUCCESS, this->m_nTaskID);
    }
    else
    {
        MSGOUT(MTS_BILLTASK, MSGOUT_LEVEL_IMPORTANT, 
               S_SCP_TASK_FAIL, this->m_nTaskID,
               nRetCode);
        TRACE(MTS_BILLTASK, 
              S_SCP_TASK_FAIL, this->m_nTaskID,
              nRetCode);
    }

    //���������ǰ���ȴ�10���ӣ����������������ѱ�����
    //10����SCP���յ������������ᷢ��ͬ����
    if(!m_bAskForExit)
    {
        ACE_OS::sleep(10);
    }

}

/********************************************************
* ����: CScpRvyTask::ProcOneQueryTask
* ����: ��һ���쳣�����ָ�������лָ�
* ����: fDstFile : Ŀ���ļ���ָ��
* ���: 
* ����: 
********************************************************/
void CScpRvyTask::ProcOneQueryTask(FILE* fDstFile) throw(int)
{
	//���㻰���ָ�����ֹʱ��
	STRING strStartTime;
	STRING strEndTime;
		
	//��ʼʱ����ǰ��Сʱ
	time_t tSearchTime = mktime(&m_EcptBillRec.tmStartTime);
	tSearchTime -= 30 * 60;
	tm tmSearchTime = *localtime(&tSearchTime);

	//���㻰���ָ�����ʼ���ڣ���ʽΪyyyymmdd
	char szTemp[20];
	strftime(szTemp, 20, "%Y%m%d", &tmSearchTime);
	strStartTime = szTemp;

	//���㻰���ָ�����ֹ���ڣ���ʽΪyyyymmdd
	if((tmSearchTime.tm_mday !=  m_EcptBillRec.tmStartTime.tm_mday) ||
	//�ղ���,���Խ2��,ǰһ��͵���
	   (tmSearchTime.tm_hour  >= 12))
	//����12���2��,�������һ��(�����϶��ǵ���Ļָ�����
    //û����һ��Ļ����ļ�����Ч��Ӱ�첻�����￼�Ǽ������)
	{
		tSearchTime = mktime(&tmSearchTime);
		tSearchTime += 24L * 3600L;
		strftime(szTemp, 20, "%Y%m%d", localtime(&tSearchTime));
		strEndTime = szTemp;
	}
	else
	//0:30-12:30,�鵱��
	{
		strEndTime = strStartTime;
	}

	//�����ѯ����
	char szQueryCondition[100];
	SNPRINTF(szQueryCondition, 100, 
             S_SCP_QUERY_CONDITION, 
             m_EcptBillRec.szCallingNum,
             m_EcptBillRec.szTransNum);

    //ȡm_nAP���������������ʱ����int nAPType
    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();
    char szAPSec[25];
    SNPRINTF(szAPSec, 25, "%s%d", CFG_SEC_AP, m_nAP);
    szAPSec[24] = '\0';
    int nAPType = IniFile.GetInt(szAPSec, CFG_AP_KEY_APTYPE,
                                 CFG_AP_KEY_APTYPE_DEFAULT);

	//׼���Ļ����ļ�
	CScpFileGroup* pFileGroup = new CScpFileGroup(m_pFormat, 
                                                  FINAL_BILL, 
                                                  nAPType,
                                                  this);
	if(NULL == pFileGroup) throw BR_SYSECPT;
    
	BOOL bRet = pFileGroup->PrepareFiles(m_strBackChlPath.c_str(),
                                         strStartTime.c_str(),
                                         strEndTime.c_str(),
                                         DATE_RANGE,
                                         szQueryCondition);
    if(!bRet)
    {
        delete pFileGroup;
        throw BR_SYSECPT;
    }

	//ȡ���������Ļ���
	CBillRecord* pBillRecord = NULL;
	int nStartFileNo = 0;
	int nStartOffset = 0;
	BOOL bRemain = TRUE;
	while ((pFileGroup->GetNextRecord(pBillRecord,
                                     nStartFileNo,
                                     nStartOffset,
                                     bRemain)) 
									 && bRemain)
	{
        //�����쳣�Ļ���
		if(pBillRecord == NULL ) continue;

        //�����˳�����
        if(m_bAskForExit)
        {
            delete pBillRecord;
            delete pFileGroup;
            throw BR_SYSECPT;
        }

        //ȡ�����������������
		LIST<STRING> ContentList;
        pBillRecord->GetContent(ContentList);

        //��д�ָ�����
        char szTempField[50];
        SNPRINTF(szTempField, 50, "%d", m_EcptBillRec.nRecNo);
        szTempField[49] = '\0';
		STRING strWriteLine = szTempField;
		strWriteLine += ",";

        SNPRINTF(szTempField, 50, "%d", m_EcptBillRec.nServKey);
        szTempField[49] = '\0';
		strWriteLine += szTempField;
		strWriteLine += ",";

		char szFieldContent[100];
		LIST<STRING>::iterator i;
		for(i = ContentList.begin(); i != ContentList.end(); i++)
		{
			strncpy(szFieldContent, (*i).c_str(), 100);
			szFieldContent[99] = '\0';
			StringLRTrim(szFieldContent);
			strWriteLine += szFieldContent;
			strWriteLine += ",";
		}
		delete pBillRecord;
		pBillRecord = NULL;

        SNPRINTF(szTempField, 50, "%d", m_EcptBillRec.nDecCost);
        szTempField[49] = '\0';
		strWriteLine += szTempField;
		strWriteLine += "\n";

        //д�ָ������ļ�
		if (fputs(strWriteLine.c_str(), fDstFile) < 0) 
        {
            delete pFileGroup;
            throw BR_SYSECPT;
        }
	}

	delete pFileGroup;
}

