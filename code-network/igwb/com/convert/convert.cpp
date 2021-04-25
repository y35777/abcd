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
��������    ���ݽ�������ʹ�����Ӧ��CBillProcess������Ķ���
�������    typeID----Ҫ�����Ļ���������������
�������    ��
����ֵ      ��������CBillProcess������Ķ������ΪNULL�����ʾ����ʧ��
�׳��쳣    ��
**********************************************************************/
CBillProcess* CBPCreator::CreateBillProcess(AP_TYPE typeID, const char* szReleaseVer)
{
    CBillProcess* pBP = NULL;

    switch (typeID)
    {
		case AP_SOFTX:	//����Զ2002.04.29���.SoftX3000��Ʒ��128��ʵ��һ��
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
��������    ���캯��
�������    uAPID----��ģ�����ڽ������̵Ľ����� 
            pPacketCache----���������������ں����ⴴ����ɾ��
�������    ��
����ֵ      ��
�׳��쳣    ��
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

//��ȡģ������
MODULE_TYPE CBillProcessor::GetModuleType()
{
    return MT_BILLPROCESSOR;
}

/**********************************************************************
��������    ��ʼ������
�������    ��
�������    ��
����ֵ      ��ʼ���Ƿ�ɹ� 
                0----��ʼ���ɹ� 
                ��0----��ʼ��ʧ��
�׳��쳣    ��
**********************************************************************/
int CBillProcessor::InitInstance()
{
    int nResult = CWorkModule::InitInstance();
    if (0 != nResult)
    {
        TRACE(MTS_BILLPROCESSOR, S_CV_TRC_FAIL_INIT_BASE);
        return ERR_FAIL;
    }

    //��ȡ�������ݣ�����������Ϣ�õ���������Ͳ�������Ӧ��CBillProcess����
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

    //��ȡ�ͻ����ϲ���صĲ���
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
    //HASH��Ĵ�С����Ϊ����û�����
    nHashSize = nMaxQwLen;

    //���кͻ����ϲ���صĳ�ʼ��
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

	m_nTimerID1Sec = SetTimer(1000);		//����һ��1�붨ʱ�����ڼ���쳣����

    return ERR_SUCCESS;
}

//ģ���˳�ʱ��������
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
��������    ��Ϣ�ַ�����
�������    pMsgParsed----���ַ�����Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillProcessor::ParseMsg(MSG_INTRA* pMsgParsed)
{
    switch (pMsgParsed->CmdCode)
    {
        case CMD_BILL_PROCESS:  //��ǰ���̷�������"����Ʒ�Ԥ����"��Ϣ
            ProcBillProcMsg(pMsgParsed);
            break;
        case CMD_RESUME:    //��ǰ���̷�������"���������ָ�"��Ϣ
            ProcResumeMsg(pMsgParsed);
            break;
        case CMD_INFORM_STATUS: //�Ӻ���̷�������"֪ͨ״̬"��Ϣ
            ProcSynInfoMsg(pMsgParsed);
            break;
        case CMD_AUXUPGRADE: //��ǰ���̷�������"������и�������"��Ϣ
            m_pBillProcess->SetUpdateFlag(TRUE);
            break;
        default:
            CWorkModule::ParseMsg(pMsgParsed);
            break;
    }
}

/**********************************************************************
��������    ����"����Ʒ�Ԥ����"��Ϣ
�������    pMessage----Ҫ�������Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillProcessor::ProcBillProcMsg(MSG_INTRA* pMessage)
{
    UINT4 uPID = *(UINT4* )pMessage->Data;
    MSGOUT(MTS_BILLPROCESSOR, MSGOUT_LEVEL_SIDLY, S_CV_MSG_REC_PACKET, uPID);

    void* pPacket = NULL;
    UINT4 uFrontBillCount, uBackBillCount, uPacketLen = 0;
    UINT4 uMergeTime, uProcessTime;

    //��ð���ָ��ͳ��ȣ������ȡ������ʧ�ܣ������հ�����
    BOOL bSucc = m_pPacketCache->GetPacket(pPacket, uPacketLen); 
    if (!bSucc)
    {
        TRACE(MTS_BILLPROCESSOR, S_CV_TRC_FAIL_GET_PACKET, uPID);
        uPacketLen = 0;
        pPacket = NULL;
    }

    try
    {
        //����û�����
        m_pBillProcess->ProcessBillPacket(uPID, (BYTE* )pPacket, uPacketLen, 
                                          uFrontBillCount, uBackBillCount, 
                                          uMergeTime, uProcessTime);  
    }
    catch (char* e)
    {
        TRACE(MTS_BILLPROCESSOR, S_CV_TRC_CATCH_ERR, e);
        SendQuitToMe();
    }

    //�����̷��ͻ�����
    for (int nChannelNo = 0; nChannelNo < m_nChannelCount; nChannelNo++)
    {
        //������������̵�"���󱣴����ջ�����"��Ϣ
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

    delete pPacket;    //�ͷ�ԭʼ������
    pPacket = NULL;

    //�������������ɣ���������ģ�鷢��"֪ͨ�����������"��Ϣ
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

        //����ֹͣ����ģ�����Ϣ
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
��������    ����"���������ָ�"��Ϣ
�������    pMessage----Ҫ�������Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillProcessor::ProcResumeMsg(MSG_INTRA* pMessage)
{
    //������ģ�鷢��"���������ָ�"֡
    MSG_INTRA* pMsg = new(0) MSG_INTRA;
    pMsg->CmdCode = CMD_RESUME;
    pMsg->SenderPid = PT_SELF;
    pMsg->SenderMid = MT_BILLPROCESSOR;
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = MT_BACKSAVE;

    SendMsg(pMsg);
}

/**********************************************************************
��������    ����"֪ͨ״̬"����Ϣ
�������    pMessage----Ҫ�������Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillProcessor::ProcSynInfoMsg(MSG_INTRA* pMessage)
{
    SInformStatus* pStatus = (SInformStatus* )pMessage->Data;
    UINT4 uLastPID = pStatus->uLastPID;
    m_pBillProcess->ProcessSynInfo(pMessage->Data, pMessage->Len);
    UINT4 uResumeStartPID = m_pBillProcess->GetResumePID(uLastPID);

    //��ǰ���̷���"֪ͨ״̬"��Ϣ
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
				//�����쳣�����澯
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
