#include "convert.h"
#include "128_proc.h"
#include "gsm_proc.h"
#include "stp_proc.h"
#include "wlan_proc.h"
#include "ps_proc.h"

#include "../include/msg_def.h"
#include "../include/toolbox.h"
#include "resource.h"

CBPCreator::CBPCreator()
{

}

CBPCreator::~CBPCreator()
{

}

/**********************************************************************
功能描述    根据接入点类型创建相应的CBillProcess的子类的对象
输入参数    typeID----要创建的话单处理对象的类型
输出参数    无
返回值      所创建的CBillProcess的子类的对象，如果为NULL，则表示创建失败
抛出异常    无
**********************************************************************/
CBillProcess* CBPCreator::CreateBillProcess(AP_TYPE typeID, const char* szReleaseVer)
{
    CBillProcess* pBP = NULL;

    switch (typeID)
    {
		case AP_SOFTX:	//颜文远2002.04.29添加.SoftX3000产品与128地实现一致
        case AP_128:
            pBP = new CBP128();
            break;

        case AP_GSM:
            pBP = new CBPGSM();
            break;

        case AP_STP:
            pBP = new CBPSTP();
            break;

		case AP_PS: 
		case AP_PS_RESERVED: 
            if (ACE_OS::strcasecmp(szReleaseVer, CFG_AP_KEY_RELEASE_R98) == 0)
			{
				pBP = new CPSBillProcess(PACKET_SWITCH_R98);
			}				
			else if (ACE_OS::strcasecmp(szReleaseVer, CFG_AP_KEY_RELEASE_R99) == 0)
			{
				pBP = new CPSBillProcess(PACKET_SWITCH_R99);
			}
			else if (ACE_OS::strcasecmp(szReleaseVer, CFG_AP_KEY_RELEASE_R4) == 0)
			{
				pBP = new CPSBillProcess(PACKET_SWITCH_R4);
			}
			else
			{
				TRACE(MTS_BILLPROCESSOR, S_CV_TRC_ERR_RELEASE_VERSION);
				pBP = NULL;
			}
			break; 

        case AP_WLAN:
        	pBP = new CBPWLAN();
			break;

        default:
            TRACE(MTS_BILLPROCESSOR, S_CV_TRC_ERR_AP_TYPE, typeID);
            pBP = NULL;
            break;
    }

    return pBP;
}

/**********************************************************************
功能描述    构造函数
输入参数    uAPID----本模块所在接入点进程的接入点号 
            pPacketCache----话单包缓冲区，在函数外创建和删除
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
CBillProcessor::CBillProcessor(const UINT4 uAPID, CPacketCache* pPacketCache)
                : CWorkModule()
{
    m_pPacketCache = pPacketCache;
    m_uAPID = uAPID;

    m_pBillProcess = NULL;
}

CBillProcessor::~CBillProcessor()
{

}

//获取模块类型
MODULE_TYPE CBillProcessor::GetModuleType()
{
    return MT_BILLPROCESSOR;
}

/**********************************************************************
功能描述    初始化函数
输入参数    无
输出参数    无
返回值      初始化是否成功 
                0----初始化成功 
                非0----初始化失败
抛出异常    无
**********************************************************************/
int CBillProcessor::InitInstance()
{
    int nResult = CWorkModule::InitInstance();
    if (0 != nResult)
    {
        TRACE(MTS_BILLPROCESSOR, S_CV_TRC_FAIL_INIT_BASE);
        return ERR_FAIL;
    }

    //读取配置数据，根据配置信息得到接入点类型并创建相应的CBillProcess对象
    CINIFile cfgIniFile(GetCfgFilePath());
    BOOL bSucc = cfgIniFile.Open();
    if (!bSucc)
    {
        TRACE(MTS_BILLPROCESSOR, S_CV_TRC_FAIL_OPEN_INI);
        return ERR_FAIL;
    }
    char szAPSection[25];
    sprintf(szAPSection, "%s%d", CFG_SEC_AP, m_uAPID);
    int typeID = cfgIniFile.GetInt(szAPSection, CFG_AP_KEY_APTYPE, 
                                   CFG_AP_KEY_APTYPE_DEFAULT);
	//2003-06-02 add by ggf
	char szReleaseVer[MAX_PATH];
	cfgIniFile.GetString(szAPSection, CFG_AP_KEY_RELEASE_VERSION, CFG_AP_KEY_RELEASE_VERSION_DEFAULT, 
		szReleaseVer, MAX_PATH);
	//end of add. ggf
    BOOL bNeedConvert = cfgIniFile.GetInt(szAPSection, CFG_MUST_CONVERT, 
                                          CFG_MUST_CONVERT_DEFAULT);
    BOOL bNeedMerge = cfgIniFile.GetInt(szAPSection, CFG_AP_KEY_MUST_MERGE, 
                                        CFG_AP_KEY_MUST_MERGE_DEFAULT);
	//2003-06-02 edit by ggf
    m_pBillProcess = CBPCreator::CreateBillProcess((AP_TYPE)typeID, szReleaseVer);
	//end of edit. ggf
    if (NULL == m_pBillProcess)
    {
        return ERR_FAIL;
    }
    bSucc = m_pBillProcess->Init(m_uAPID, bNeedConvert, bNeedMerge);
    if (!bSucc)
    {
        return ERR_FAIL;
    }

    //读取和话单合并相关的参数
    int nHashSize, nMValue, nMaxQwLen;
    nMaxQwLen = cfgIniFile.GetInt(szAPSection, CFG_MAX_WAIT_USERS, 
                                  CFG_MAX_WAIT_USERS_DEFAULT);  
    if(nMaxQwLen <= 0)
    {
        nMaxQwLen = CFG_MAX_WAIT_USERS_DEFAULT;
    }
    nMValue = cfgIniFile.GetInt(szAPSection, CFG_MAX_WAIT_PACKAGES, 
                                CFG_MAX_WAIT_PACKAGES_DEFAULT);
    if(nMValue <= 0)
    {
        nMValue = CFG_MAX_WAIT_PACKAGES_DEFAULT;
    }
    //HASH表的大小设置为最大用户数。
    nHashSize = nMaxQwLen;

    //进行和话单合并相关的初始化
    m_pBillProcess->InitMerge(nHashSize, nMValue, nMaxQwLen);
/*  if (!bSucc)
    {
        return ERR_FAIL;
    }
*/
    m_nChannelCount = m_pBillProcess->GetChannelCount();    

	m_nCDRCheckSpan  = cfgIniFile.GetInt(szAPSection, CFG_CDR_CHECK_SPAN, 
		                                     CFG_CDR_CHECK_SPAN_DEFAULT);
	m_nCDRCheckCount = cfgIniFile.GetInt(szAPSection, CFG_CDR_CHECK_COUNT, 
		                                     CFG_CDR_CHECK_COUNT_DEFAULT);

	m_nTimerID1Sec = SetTimer(1000);		//创建一个1秒定时器用于检查异常话单

    return ERR_SUCCESS;
}

//模块退出时的清理函数
void CBillProcessor::ExitInstance()
{
    if (NULL != m_pBillProcess)
    {
        delete m_pBillProcess;
        m_pBillProcess = NULL;
    }

    CWorkModule::ExitInstance();
}

/**********************************************************************
功能描述    消息分发函数
输入参数    pMsgParsed----待分发的消息帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillProcessor::ParseMsg(MSG_INTRA* pMsgParsed)
{
    switch (pMsgParsed->CmdCode)
    {
        case CMD_BILL_PROCESS:  //从前存盘发过来的"请求计费预处理"消息
            ProcBillProcMsg(pMsgParsed);
            break;
        case CMD_RESUME:    //从前存盘发过来的"请求启动恢复"消息
            ProcResumeMsg(pMsgParsed);
            break;
        case CMD_INFORM_STATUS: //从后存盘发过来的"通知状态"消息
            ProcSynInfoMsg(pMsgParsed);
            break;
        case CMD_AUXUPGRADE: //从前存盘发过来的"请求进行辅助升级"消息
            m_pBillProcess->SetUpdateFlag(TRUE);
            break;
        default:
            CWorkModule::ParseMsg(pMsgParsed);
            break;
    }
}

/**********************************************************************
功能描述    处理"请求计费预处理"消息
输入参数    pMessage----要处理的消息帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillProcessor::ProcBillProcMsg(MSG_INTRA* pMessage)
{
    UINT4 uPID = *(UINT4* )pMessage->Data;
    MSGOUT(MTS_BILLPROCESSOR, MSGOUT_LEVEL_SIDLY, S_CV_MSG_REC_PACKET, uPID);

    void* pPacket = NULL;
    UINT4 uFrontBillCount, uBackBillCount, uPacketLen = 0;
    UINT4 uMergeTime, uProcessTime;

    //获得包的指针和长度，如果获取话单包失败，则当作空包处理
    BOOL bSucc = m_pPacketCache->GetPacket(pPacket, uPacketLen); 
    if (!bSucc)
    {
        TRACE(MTS_BILLPROCESSOR, S_CV_TRC_FAIL_GET_PACKET, uPID);
        uPacketLen = 0;
        pPacket = NULL;
    }

    try
    {
        //处理该话单包
        m_pBillProcess->ProcessBillPacket(uPID, (BYTE* )pPacket, uPacketLen, 
                                          uFrontBillCount, uBackBillCount, 
                                          uMergeTime, uProcessTime);  
    }
    catch (char* e)
    {
        TRACE(MTS_BILLPROCESSOR, S_CV_TRC_CATCH_ERR, e);
        SendQuitToMe();
    }

    //向后存盘发送话单包
    for (int nChannelNo = 0; nChannelNo < m_nChannelCount; nChannelNo++)
    {
        //构造送往后存盘的"请求保存最终话单包"消息
        MSG_INTRA* pMsg;
        pMsg = new(sizeof(SBackPacketInfo)) MSG_INTRA;
        pMsg->CmdCode = CMD_SAVE_END_BILL;
        pMsg->SenderPid = PT_SELF;
        pMsg->SenderMid = MT_BILLPROCESSOR;
        pMsg->ReceiverPid = PT_SELF;
        pMsg->ReceiverMid = MT_BACKSAVE;
        SBackPacketInfo* pPacketInfo = (SBackPacketInfo* )pMsg->Data;
        pPacketInfo->yChannelID = nChannelNo;
        m_pBillProcess->GetChannelPacket(nChannelNo, pPacketInfo->pPacket, 
                                         pPacketInfo->uLen);
        
        SChannelSynInfo* pSynInfo 
                 = m_pBillProcess->GetChannelSynInfo(nChannelNo);
        pPacketInfo->uPID = pSynInfo->uLastPID;
        pPacketInfo->uBillCsn = pSynInfo->uBillCsn;

        SendMsg(pMsg);
    }

    delete pPacket;    //释放原始话单包
    pPacket = NULL;

    //如果辅助升级完成，则向后存盘模块发送"通知辅助升级完成"消息
    if (m_pBillProcess->IsAssistUpdateOver())
    {
        static BOOL bIsOver = FALSE;

        if(bIsOver == TRUE) return;

        bIsOver = TRUE;

        MSG_INTRA* pMsg = new(0) MSG_INTRA;
        pMsg->CmdCode = CMD_ANS_AUXUPGRADE;
        pMsg->SenderPid = PT_SELF;
        pMsg->SenderMid = MT_BILLPROCESSOR;
        pMsg->ReceiverPid = PT_SELF;
        pMsg->ReceiverMid = MT_BACKSAVE;

        SendMsg(pMsg);

        //发送停止网络模块的消息
        pMsg = new(0) MSG_INTRA;
        pMsg->CmdCode = CMD_STOP_NETWORK;
        pMsg->SenderPid = PT_SELF;
        pMsg->SenderMid = MT_BILLPROCESSOR;
        pMsg->ReceiverPid = PT_SELF;
        pMsg->ReceiverMid = MT_NETWORK;

        SendMsg(pMsg);
    }
}

/**********************************************************************
功能描述    处理"请求启动恢复"消息
输入参数    pMessage----要处理的消息帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillProcessor::ProcResumeMsg(MSG_INTRA* pMessage)
{
    //向后存盘模块发送"请求启动恢复"帧
    MSG_INTRA* pMsg = new(0) MSG_INTRA;
    pMsg->CmdCode = CMD_RESUME;
    pMsg->SenderPid = PT_SELF;
    pMsg->SenderMid = MT_BILLPROCESSOR;
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = MT_BACKSAVE;

    SendMsg(pMsg);
}

/**********************************************************************
功能描述    处理"通知状态"的消息
输入参数    pMessage----要处理的消息帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillProcessor::ProcSynInfoMsg(MSG_INTRA* pMessage)
{
    SInformStatus* pStatus = (SInformStatus* )pMessage->Data;
    UINT4 uLastPID = pStatus->uLastPID;
    m_pBillProcess->ProcessSynInfo(pMessage->Data, pMessage->Len);
    UINT4 uResumeStartPID = m_pBillProcess->GetResumePID(uLastPID);

    //向前存盘发送"通知状态"消息
    MSG_INTRA* pMsg = new(sizeof(UINT4)) MSG_INTRA;
    pMsg->CmdCode = CMD_INFORM_STATUS;
    pMsg->SenderPid = PT_SELF;
    pMsg->SenderMid = MT_BILLPROCESSOR;
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = MT_FRONTSAVE;
    *(UINT4* )pMsg->Data = uResumeStartPID;

    SendMsg(pMsg);
}

void CBillProcessor::OnTimer(const long nTimerID)
{
	static int nCDRCheckSpan = 0;

    if (nTimerID == m_nTimerID1Sec)
	{
		nCDRCheckSpan++;
		if (nCDRCheckSpan >= m_nCDRCheckSpan)
		{
			nCDRCheckSpan = 0;
			if (m_pBillProcess->GetErrorCDRCount() >= m_nCDRCheckCount)
			{
				//发送异常话单告警
				SInnerAlarm sAlarm;
				sAlarm.yAlarmType    = AT_EVENT;
				sAlarm.uAlarmID      = ALARM_ID_ABNORMAL_BILL;
				sAlarm.yAlarmPara[0] = ALARM_PARA_DECODING_ERR;
				SendAlarm(&sAlarm);
			}

			m_pBillProcess->ResetErrorCDRCount();
		}
	}
}
