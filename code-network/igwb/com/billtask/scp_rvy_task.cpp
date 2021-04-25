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
* 名称: CScpRvyTask::CScpRvyTask
* 功能: 构造函数
* 输入: nAP     : 接入点号
*       pApply  : SCP话单恢复请求信息
*       pCmd    : 用于返回报告的信息
*       nTaskID : 任务号
* 输出: 
* 返回: 
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
* 名称: CScpRvyTask::~CScpRvyTask
* 功能: 析构函数
* 输入: 
* 输出: 
* 返回: 
********************************************************/
CScpRvyTask::~CScpRvyTask()
{
    m_pIQueryBrowse = NULL;
    m_pFormat       = NULL;

    //释放格式库对象
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

    //删除本次SCP话单恢复工作目录
    if(!m_strSCPRvyRoot.empty())
    {
        SweepDirectory((char*)m_strSCPRvyRoot.c_str());
    }
}

/********************************************************
* 名称: CScpRvyTask::Init
* 功能: 初始化
* 输入: 
* 输出: 
* 返回: 
********************************************************/
int CScpRvyTask::Init()
{
    //初始化格式库接口
    STRING strIGWBPath = STRING(GetAppPath()) +
                         "/config/format";
    m_pIGWB = new CIGWB(strIGWBPath);
    if(NULL == m_pIGWB)
    {
        TRACE(MTS_BILLTASK, S_QB_TRC_FAIL_INIT_FORMAT);
        throw BR_SYSECPT;
    }

    //获得格式库查询接口
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

    //获得SCP话单恢复格式名
    m_pFormat = m_pIQueryBrowse->GetBrowseFmt(m_strFormatName);
	if (NULL == m_pFormat)
    {
        TRACE(MTS_BILLBROWSER, S_QB_TRC_FAIL_INIT_FORMAT);
        throw BR_SYSECPT;
    }

    //取m_nAP接入点名，赋给临时变量STRING strAPName
    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();

    char szAPSec[25];
    SNPRINTF(szAPSec, 25, "%s%d", CFG_SEC_AP, m_nAP);
    szAPSec[24] = '\0';

    char szAPName[MAX_PATH];
    IniFile.GetString(szAPSec, CFG_AP_NAME, szAPSec,
                      szAPName, MAX_PATH);
    szAPName[MAX_PATH - 1] = '\0';

    //取m_nAP接入点下后存盘中智能话单的通道名
	STRING strBackChlName = "";
    STRING strSrcFmtName = "";
    m_pFormat->GetFormatGen(strBackChlName, strSrcFmtName);

    //取m_nAP接入点下后存盘中智能话单的全路径名
    char szBackRoot[MAX_PATH];
    IniFile.GetString(CFG_SEC_DISK_FILE, CFG_BACK_SAVE_ROOT_DIR, 
                      CFG_BACK_SAVE_ROOT_DIR_DEFAULT,
                      szBackRoot, MAX_PATH);
    szBackRoot[MAX_PATH - 1] = '\0';
    m_strBackChlPath = STRING(szBackRoot) + "/" + 
                       STRING(szAPName) + "/" + strBackChlName;

    //置SCP话单恢复在本地的工作目录全路径名
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

    //置SCP异常话单的全路径名
    m_strSCPErrBill = m_strSCPRvyRoot + "/" + STRING(m_ScpApply.szFileName);

    //置SCP恢复话单的全路径名
    m_strSCPRvyBill = m_strSCPRvyRoot + "/" + "OMC" + 
                      STRING((char*)(m_ScpApply.szFileName + 3));

	//读取本地IP
	char szLocalIp[MAX_PATH];
	IniFile.GetString(CFG_SEC_MML, 
                      CFG_MML_KEY_LOCAL_IP,
                      CFG_MML_KEY_LOCAL_IP_DEFAULT,
					  szLocalIp, MAX_PATH);
	szLocalIp[MAX_PATH - 1] = '\0';
	m_strLocalIp = STRING(szLocalIp);

    //初始化m_CodeKeyList
    STRING strCodeKeyPath = STRING(GetAppPath()) + 
                            S_SCP_PATH_BASE_ON_APP_PATH;
    CINIFile CodeKeyFile(strCodeKeyPath.c_str());
    if(!CodeKeyFile.Open())
    {
        throw BR_CODEKEYFAIL;
    }

    //读出第m_nAP个接入点中CodeKeyCount,赋给临时变量int nCodeKeyCount
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

		//读出"CodeKey"+i中的配置项，赋给临时变量char szCodeKey[100];
        SNPRINTF(szCfgCodeKeyName, 25, "%s%d", CFG_AP_KEY_CODE_KEY, i);
        szCfgCodeKeyName[24] = '\0';
        CodeKeyFile.GetString(szAPSec, szCfgCodeKeyName, 
                              CFG_AP_KEY_CODE_KEY_DEFAULT,
                              szCodeKey, 100);
    
		//将szCodeKey中','前的字符串去掉左右空格后，赋给ck.szAccCode
		//将szCodeKey中','后的字符串去掉左右空格并转换为整数后，赋给ck.nServKey;
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
* 名称: CScpRvyTask::IsMatchedTaskk
* 功能: 通过消息帧判断是否是同一任务,只需要比较任务命令码、
*       IP地址、目录名及文件名即可，注意必须要判断命令码，
*       因为任务列表中可能有多种任务，反之，多种任务的请求
*       可能调用本函数来测试是否是同一任务。
* 输入: pMsg : 由SCP发来的请求命令
* 输出: 
* 返回: 
********************************************************/
BOOL CScpRvyTask::IsMatchedTask(MSG_INTRA* pMsg)
{
    //判断是否是MML命令
    if(CMD_MML != pMsg->CmdCode)
    {
        return FALSE;
    }

    //判断MML命令是否是MML_SCP_RECOVERY
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
* 名称: CScpRvyTask::StrToTm
* 功能: 将输入的字符串转换为tm结构
* 输入: szSrc : 时间源字符串，格式为
* 输出: tmTar : 目标结构
* 返回: 是否成功，0表示成功，非零值表示失败
********************************************************/
int CScpRvyTask::StrToTm(const char* szSrc, tm &tmTar)
{
	int nYear, nMon, nDay, nHour, nMin, nSec;
	char szTemp[10];

	if(NULL == szSrc)
    {
		return -1;
    }

    //字节数不匹配，则返回失败
    if(strlen(szSrc) != 14)
    {
        return -1;
    }

	//取年
	memcpy(szTemp, szSrc, 4);
	szTemp[4] = 0;
	nYear = atoi(szTemp);

	//取月
	memcpy(szTemp, szSrc+4, 2);
	szTemp[2] = 0;
	nMon = atoi(szTemp);

	//取日
	memcpy(szTemp, szSrc+6, 2);
	szTemp[2] = 0;
	nDay = atoi(szTemp);

	//取时
	memcpy(szTemp, szSrc+8, 2);
	szTemp[2] = 0;
	nHour = atoi(szTemp);

	//取分
	memcpy(szTemp, szSrc+10, 2);
	szTemp[2] = 0;
	nMin = atoi(szTemp);

	//取秒
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
* 名称: CScpRvyTask::IsMatchedScpBill
* 功能: 判断话单是否满足接入码及业务键条件的接口
* 输入: pData : 话单数据指针
*       nLen  : 话单长度
* 输出: 
* 返回: 是否满足条件，TRUE表示满足
********************************************************/
BOOL CScpRvyTask::IsMatchedScpBill(BYTE* pData, int nLen)
{
    //获取该话单的被叫号码
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

	//在m_CodeKeyList中查找接入码等于该被叫号码的项
	LIST<SCodeKey>::iterator i;
	for (i = m_CodeKeyList.begin(); i != m_CodeKeyList.end(); i++)
	{
		//查找被叫号码一致的SCodeKey结构
	    //如果该项对应的业务键等于m_EcptBillRec中的nServKey
        //则返回TRUE，否则返回FALSE
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
* 名称: CScpRvyTask::IsMatchedScpBill
* 功能: 实施特定操作
* 输入: 
* 输出: 
* 返回: 
********************************************************/
void CScpRvyTask::ProcBillTask()
{
    int nRetCode   = 0;
    FILE* fSrcFile = NULL;
    FILE* fDstFile = NULL;

    try
    {
        int nRet = 0;

        //通过FTP从SCP上取异常话单		
		nRet = m_FtpClient.Connect(m_ScpApply.szIP, 
                                   m_ScpApply.szOp, 
                                   m_ScpApply.szPwd, (char *)m_strLocalIp.c_str());
		

        //设置文件传输模式
		nRet = m_FtpClient.SetTransMode(FTP_TRANS_ASCII);
        if(nRet != 0) throw BR_FTP_FAIL;

  
        //设置当前目录
		nRet = m_FtpClient.SetDirectory(m_ScpApply.szDir);
        if(nRet != 0) throw BR_FTP_FAIL;
        
		//取文件
		nRet = m_FtpClient.GetFile(m_strSCPErrBill.c_str(),
								m_ScpApply.szFileName);
        if(nRet != 0) throw BR_FTPGETFAIL;
		

        //打开目标文件
        fDstFile = fopen(m_strSCPRvyBill.c_str(), "w");
        if(NULL == fDstFile) throw BR_RCVRFILECRTFAIL;

        //从文件中读出每一条异常话单信息，并处理之
        fSrcFile = fopen(m_strSCPErrBill.c_str(), "r");
        if(NULL == fSrcFile) throw BR_ECPTFILEOPENFAIL;
		
        char szLine[ONE_LINE];
		while(fgets(szLine, ONE_LINE, fSrcFile)	!= NULL)
		{
			//从读出的一行中读出SEcptBillRec信息
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
			//将时间从字符串转换为tm结构
			if(StrToTm(m_EcptBillRec.szDateTime, 
                       m_EcptBillRec.tmStartTime) != 0)
			{
				continue;
			}
            
            //处理每一条异常话单
			ProcOneQueryTask(fDstFile);
			
		}
		        
		if(FileSize(fDstFile) == 0) throw BR_NOBILLRECMATCH;

		//释放资源
		fflush(fDstFile);
		fclose(fDstFile);
        fDstFile = NULL;

		fclose(fSrcFile);
        fSrcFile = NULL;
               
	    //向SCP传送文件
		nRetCode = BR_FTPPUTFAIL;
        STRING strSCPRvyBill = "OMC" + 
                               STRING((char*)(m_ScpApply.szFileName + 3));
		int nSendLoop = 3;
		while(nSendLoop-- > 0)
		{
			nRet = m_FtpClient.PutFile(m_strSCPRvyBill.c_str(),
                                       strSCPRvyBill.c_str());

            //如果成功则置nRetCode=0，并且break;
            if(0 == nRet)
            {
                nRetCode = 0;
                break;
            }
            else if(nSendLoop > 0)
            {
                //如果FTP传文件失败，则等待5秒后再重试
                ACE_OS::sleep(5);
            }   
		}
	}
	//处理异常
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

	//向SCP发应答
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

    //在任务结束前，等待10秒钟，避免该任务结束后已被创建
    //10秒内SCP已收到处理结果，不会发相同请求
    if(!m_bAskForExit)
    {
        ACE_OS::sleep(10);
    }

}

/********************************************************
* 名称: CScpRvyTask::ProcOneQueryTask
* 功能: 对一条异常话单恢复请求进行恢复
* 输入: fDstFile : 目标文件的指针
* 输出: 
* 返回: 
********************************************************/
void CScpRvyTask::ProcOneQueryTask(FILE* fDstFile) throw(int)
{
	//计算话单恢复的起止时间
	STRING strStartTime;
	STRING strEndTime;
		
	//起始时间提前半小时
	time_t tSearchTime = mktime(&m_EcptBillRec.tmStartTime);
	tSearchTime -= 30 * 60;
	tm tmSearchTime = *localtime(&tSearchTime);

	//计算话单恢复的起始日期，格式为yyyymmdd
	char szTemp[20];
	strftime(szTemp, 20, "%Y%m%d", &tmSearchTime);
	strStartTime = szTemp;

	//计算话单恢复的终止日期，格式为yyyymmdd
	if((tmSearchTime.tm_mday !=  m_EcptBillRec.tmStartTime.tm_mday) ||
	//日不等,则跨越2天,前一天和当天
	   (tmSearchTime.tm_hour  >= 12))
	//大于12点跨2天,当天和下一天(基本上都是当天的恢复请求，
    //没有下一天的话单文件，对效率影响不大。这里考虑极端情况)
	{
		tSearchTime = mktime(&tmSearchTime);
		tSearchTime += 24L * 3600L;
		strftime(szTemp, 20, "%Y%m%d", localtime(&tSearchTime));
		strEndTime = szTemp;
	}
	else
	//0:30-12:30,查当天
	{
		strEndTime = strStartTime;
	}

	//构造查询条件
	char szQueryCondition[100];
	SNPRINTF(szQueryCondition, 100, 
             S_SCP_QUERY_CONDITION, 
             m_EcptBillRec.szCallingNum,
             m_EcptBillRec.szTransNum);

    //取m_nAP接入点名，赋给临时变量int nAPType
    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();
    char szAPSec[25];
    SNPRINTF(szAPSec, 25, "%s%d", CFG_SEC_AP, m_nAP);
    szAPSec[24] = '\0';
    int nAPType = IniFile.GetInt(szAPSec, CFG_AP_KEY_APTYPE,
                                 CFG_AP_KEY_APTYPE_DEFAULT);

	//准备的话单文件
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

	//取符合条件的话单
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
        //出现异常的话单
		if(pBillRecord == NULL ) continue;

        //处理退出请求
        if(m_bAskForExit)
        {
            delete pBillRecord;
            delete pFileGroup;
            throw BR_SYSECPT;
        }

        //取出话单各个域的内容
		LIST<STRING> ContentList;
        pBillRecord->GetContent(ContentList);

        //填写恢复话单
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

        //写恢复话单文件
		if (fputs(strWriteLine.c_str(), fDstFile) < 0) 
        {
            delete pFileGroup;
            throw BR_SYSECPT;
        }
	}

	delete pFileGroup;
}

