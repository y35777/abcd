//���ļ�������Զ2002.04.09����


#include "slide_wnd_related.h"
#include "network.h"
#include "../utility/mml_report.h"

#define INVALID_RECORD_SEQNUMBER	-1
#define MSC_MOD_NUM_OFFSET			17

//���������ڼ���һ�Ż������ܵķ��ؽṹ
enum AddRecResult
{
	SW_ACCEPT,	//����������
	SW_CONCEDE,	//��ǿ���գ��ڻ�������ǰһ�����������ڣ���Ϊ���ظ�֡
	SW_REFUSE,	//���ܾ�
};

//���캯��
CSlideWnd::CSlideWnd()
{
    m_pData = NULL;
    m_nSize = 0;
    m_nBaseSeqNumber = 0;
    m_nExpectedSeqNumber = 0;
    m_nHead = 0;
    m_nRecordCount = 0;
	m_nMpuWindowSize = 0;
	m_nBillRecordSize = 0;
	m_nBillMsgSize = 0;
}

//��������
CSlideWnd::~CSlideWnd()
{
    if (m_pData)
    {
        delete[] m_pData;
		m_pData = NULL;
    }
}

/**********************************************************************
��������    ��ʼ����������
�������    nSize�����������ڴ�С
			nBaseSeqNumber�����������ڵ���ʼ��ˮ��
			nBillMsgSize�������������е���Ϣ��С
			nBillRecordSize���������ĳ���
			nMpuWindowSize���������Ļ������ڴ�С
�������    ��
����ֵ      ��
�׳��쳣    ������ռ�ʧ��ʱ�׳��쳣
**********************************************************************/
void CSlideWnd::Init(int nSize, int nBaseSeqNumber, int nBillMsgSize, 
					 int nBillRecordSize, int nMpuWindowSize)
{
    ASSERT(nSize > 0);
    ASSERT(nBaseSeqNumber >= 0);
	ASSERT(nBillRecordSize > 0);
	ASSERT(nBillMsgSize == nBillRecordSize + _BILLMSG_HEAD_LENGTH);
	ASSERT(nMpuWindowSize > 0);

    //�������ڲ������鷽ʽ�洢���ݡ��˴����·�������ռ�m_pData
	m_nBillMsgSize = nBillMsgSize;
	m_nBillRecordSize = nBillRecordSize;
    if (NULL != m_pData)
    {
        delete m_pData;
        m_pData = NULL;
    }
	m_pData = new BYTE[nSize * m_nBillMsgSize];
	if (NULL == m_pData)
	{
		throw "Failed to new m_pData in CSlideWnd::Init.";
	}

    //ʹ�ø�����ʾ�Ƿ����кţ������м�¼�����к�
    //��Ϊ��������ջ�������
	SBillMessage *pMsg = NULL;
    for (int i = 0; i < nSize; i++)
    {
		pMsg = (SBillMessage *)(m_pData + m_nBillMsgSize * i);
        pMsg->nSequ(INVALID_RECORD_SEQNUMBER);
    }
    m_nSize = nSize;
    m_nBaseSeqNumber = nBaseSeqNumber;
    m_nExpectedSeqNumber = nBaseSeqNumber;
    m_nRecordCount = 0;
	m_nMpuWindowSize = nMpuWindowSize;
}

/**********************************************************************
��������    ͬ����������
�������    nBaseSeqNumber�����µ���ʼ��ˮ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSlideWnd::Synchronize(int nBaseSeqNumber)
{
    ASSERT(NULL != m_pData);

	SBillMessage *pMsg = NULL;
    for (int i = 0; i < m_nSize; i++)
    {
		pMsg = (SBillMessage *)(m_pData + m_nBillMsgSize * i);
		pMsg->nSequ(INVALID_RECORD_SEQNUMBER);
    }
    m_nHead = 0;

    m_nBaseSeqNumber = nBaseSeqNumber;
    m_nExpectedSeqNumber = nBaseSeqNumber;
    m_nRecordCount = 0;
}

/**********************************************************************
��������    �򻬶����������һ����¼
�������    pMsg����������Ļ���
�������    ��
����ֵ      ��Ӽ�¼�Ľ��
				SW_ACCEPT -- ����������
				SW_CONCEDE -- ��ǿ���գ��ڻ�������ǰһ�����������ڣ�
								��Ϊ���ظ�֡��Ӧ��ǰ̨����Ӧ��֡
				SW_REFUSE -- ���ܾ���Ӧ��ǰ̨���;ܾ�֡
˵��:
			������������������Χ�����������ڣ���������ǰ��һ��
			�������ں�������Χ�������ڻ��������ڣ�����������
			�ݣ��ȴ��������ռ�->���͹��̣������ڻ�������ǰ��һ
			�����������ڣ�����Ϊ�������ظ����ͣ�Ӧ����������Ӧ
			��֡��������ΧӦ���;ܾ�֡��
�׳��쳣    ��
**********************************************************************/
int CSlideWnd::AddRecord(SBillMessage *pMsg)
{
	ASSERT(m_nBaseSeqNumber <= m_nExpectedSeqNumber);

    //�жϻ����Ƿ���Խ���
    int nFirstAcceptableSeqNumber;
    nFirstAcceptableSeqNumber = m_nBaseSeqNumber - m_nMpuWindowSize;
    if (nFirstAcceptableSeqNumber < 0)
	{
        nFirstAcceptableSeqNumber = 0;
	}
    if ((pMsg->nSequ() < nFirstAcceptableSeqNumber) ||
        (pMsg->nSequ() >= m_nBaseSeqNumber + m_nSize))
	{
        return SW_REFUSE;
	}

    //�����뻬������֮�ڵĻ������뻬������
    int nIndex = pMsg->nSequ() - m_nBaseSeqNumber;
    if (nIndex >= 0)
    {
		SBillMessage *pMsgBuf;

        nIndex = (nIndex + m_nHead) % m_nSize;
		pMsgBuf = (SBillMessage *)(m_pData + m_nBillMsgSize * nIndex);
        if (pMsgBuf->nSequ() == INVALID_RECORD_SEQNUMBER)
        {
            memcpy(pMsgBuf, pMsg, m_nBillMsgSize);
            m_nRecordCount++;
        }

		if (pMsg->nSequ() == m_nExpectedSeqNumber)
		{
			//�������m_nExpectedSeqNumber��ֵ��ֱ���ҵ�һ��δ��Ż�����λ��
			//�򳬹��������ڷ�ΧΪֹ
			do
			{
				m_nExpectedSeqNumber++;

				nIndex = m_nExpectedSeqNumber - m_nBaseSeqNumber;
				nIndex = (nIndex + m_nHead) % m_nSize;
				pMsgBuf = (SBillMessage* )(m_pData + m_nBillMsgSize * nIndex);
			}while ((INVALID_RECORD_SEQNUMBER != pMsgBuf->nSequ()) 
				    && (m_nExpectedSeqNumber - m_nBaseSeqNumber) < m_nSize);
			        //Z30971�޸����ⵥD19843 2002-12-18
		}

		return SW_ACCEPT;
    }

    return SW_CONCEDE;
}

/**********************************************************************
��������    ��ǰ������������
�������    nHops������ǰ�����Ĳ���
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSlideWnd::Slide(int nHops)
{
	ASSERT( nHops <= m_nSize);

    for (int i =0; i < nHops; i++)
    {
		SBillMessage *pMsg;

		pMsg = (SBillMessage *)(m_pData + m_nBillMsgSize * m_nHead);
        if (pMsg->nSequ() != INVALID_RECORD_SEQNUMBER)
        {
            pMsg->nSequ(INVALID_RECORD_SEQNUMBER);
            m_nRecordCount--;
        }
        if (++m_nHead >= m_nSize)
		{
            m_nHead = 0;
		}
    }

    m_nBaseSeqNumber += nHops;
    if (m_nExpectedSeqNumber < m_nBaseSeqNumber)
    {
        m_nExpectedSeqNumber = m_nBaseSeqNumber;
    }
}

/**********************************************************************
��������    ���ؿ��ύ��¼
�������    pDataBuffer������Ŵ��ύ�����Ļ�����
�������    ��
����ֵ      �ύ�Ļ�������
�׳��쳣    ��
**********************************************************************/
int CSlideWnd::GetReadyData(CDataBuffer* pDataBuffer)
{
	ASSERT(m_nBaseSeqNumber <= m_nExpectedSeqNumber);

    int nIndex = 0, nCount = 0;

    //�������ݵ�������
	int nResult = 0;
	SBillMessage *pMsg = NULL;
    for (int i = m_nBaseSeqNumber; i < m_nExpectedSeqNumber; i++)
    {
		nIndex = i - m_nBaseSeqNumber;
		nIndex = (m_nHead + nIndex) % m_nSize;
		pMsg = (SBillMessage *)(m_pData + m_nBillMsgSize * nIndex);
        if (pMsg->nSequ() == INVALID_RECORD_SEQNUMBER)
        {
            break;
        }
        else
        {
			nResult = pDataBuffer->Put((char* )pMsg->Data(), m_nBillRecordSize);
			if (0 != nResult)
			{
				break;
			}

            nCount++;
        }
    }

    return nCount;
}


//���캯��
CSlideWndStack::CSlideWndStack(CNetApp* const pNetApp, UINT4 const uStackId)
				: CProtocolStack(pNetApp, uStackId)
{
	m_nAckNumber = 0;
	memset(&m_SlideNodeInfo, 0, SLIDE_MOD_INFO_LENGTH);
	m_SlideNodeInfo.yModule = uStackId;

	//��ʼ��m_pAckFrame
	m_pAckFrame = new(_APPFRAME_EXHEAD_LENGTH + sizeof(SAckOrRejRes)) SMemFrame;
	if (NULL == m_pAckFrame)
	{
		throw "Fail to new m_pAckFrame in CSlideWndStack::CSlideWndStack";
	}
	m_pAckFrame->nCmd(MEM_CMD_DATA);
	SAppFrame* pAppFrame = (SAppFrame* )(m_pAckFrame->Data());
	pAppFrame->sender_pid(APP_PID_HTMAIL);
	pAppFrame->sender_mid(APP_MID_NULL);
    pAppFrame->receiver_pid(APP_PID_BILL);
    pAppFrame->receiver_mid(uStackId);
	pAppFrame->nLen(_APPFRAME_EXPAND_LENGTH + sizeof(SAckOrRejRes));
    pAppFrame->sender_desk(APP_DESK_NULL);
    pAppFrame->sender_app(APP_APP_NULL);
    pAppFrame->receiver_desk(APP_DESK_NULL);
    pAppFrame->receiver_app(APP_APP_NULL);

	//������Э��ջ��״̬����
	m_pStatusInfo = new CStackStatusInfo(STACK_SLIDE, uStackId);
	if (NULL == m_pStatusInfo)
	{
		throw "Failed to new m_pStatusInfo in CSlideWndStack::CSlideWndStack.";
	}

    //��ȡ�����̶�Ӧ��������ò���
    CINIFile IniFile((char *)GetCfgFilePath());
    IniFile.Open();

    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP, 
            CMsgProcessor::GetProcessType() - PT_AP_BASE + 1);
    m_nBillRecordSize = IniFile.GetInt(strSec, CFG_AP_KEY_BILL_REC_SIZE, 
                                       CFG_AP_KEY_BILL_REC_SIZE_DEFAULT);
	m_nBillMsgSize = m_nBillRecordSize + _BILLMSG_HEAD_LENGTH;
	int nMpuWindowSize = IniFile.GetInt(strSec, CFG_AP_KEY_MPU_WINDOW_SIZE, 
										CFG_AP_KEY_MPU_WINDOW_SIZE_DEFAULT);
	int nSlideWndSize = IniFile.GetInt(strSec, CFG_AP_KEY_BAU_WINDOW_SIZE, 
									   CFG_AP_KEY_BAU_WINDOW_SIZE_DEFAULT);
    IniFile.Close();
	m_SlideWnd.Init(nSlideWndSize, 0, m_nBillMsgSize, 
					m_nBillRecordSize, nMpuWindowSize);

    m_nRecvAcceptCount    = 0;
    m_nMaxRecvAcceptCount = nSlideWndSize * 1.5;
    m_bHasUnrepliedFrm    = FALSE;
}

//��������
CSlideWndStack::~CSlideWndStack()
{
	if (NULL != m_pAckFrame)
	{
		delete m_pAckFrame;
		m_pAckFrame = NULL;
	}
}

//���ر������е�ǰ�ѽ��յ��ҿ����ύ��ǰ����ģ�鱣��Ļ������ݵ��ֽ���
int CSlideWndStack::GetReadyDataSize(void)
{
	return m_SlideWnd.GetReadyRecordCount() * m_nBillRecordSize;
}

//���ر������е�ǰ�ѽ��յ��ҿ����ύ��ǰ����ģ�鱣��Ļ������ݵļ�¼��
int CSlideWndStack::GetReadyRecordCount(void)
{
	return m_SlideWnd.GetReadyRecordCount();
}

//����Э��ջ���ύ��ǰ����ģ�鱣��Ļ������ݷ���*pDataBuf������
void CSlideWndStack::GetReadyData(CDataBuffer* const pDataBuf)
{
	m_SlideNodeInfo.SSm.uSubmitRecCnt = m_SlideWnd.GetReadyData(pDataBuf);
}

/**********************************************************************
��������    ����״̬�ļ��ж�ȡ��Э��ջ��״̬��Ϣ
�������    ��
�������    ��
����ֵ      0��ʾ�ɹ�����0��ʾʧ��
�׳��쳣    ��
**********************************************************************/
int CSlideWndStack::LoadStatus(void)
{
    UINT4 uInputLen = m_pStatusInfo->Get((BYTE*)(&m_SlideNodeInfo), 
                                         SLIDE_MOD_INFO_LENGTH);
    //��״̬�ļ����ɹ��򷵻�ʧ��
    if(uInputLen != SLIDE_MOD_INFO_LENGTH) 
	{
		return ERR_FAIL;
	}

	m_SlideWnd.Synchronize(m_SlideNodeInfo.SSm.uBaseSeqNum);

	return 0;
}

/**********************************************************************
��������    ����״̬�ļ���д�뱾Э��ջ��״̬��Ϣ
�������    ��
�������    ��
����ֵ      0��ʾ�ɹ�����0��ʾʧ��
�׳��쳣    ��
**********************************************************************/
int CSlideWndStack::SaveStatus(void)
{
	m_SlideNodeInfo.SSm.uBaseSeqNum = m_SlideWnd.GetBaseSeqNumber();
	m_pStatusInfo->Set((BYTE* )&m_SlideNodeInfo, SLIDE_MOD_INFO_LENGTH);

	return 0;
}

/**********************************************************************
��������    ������뱾Э��ջ��Ӧ������ģ���GSN���͹�����һ֡����
�������    pFrame�����յ���������Ϣ
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSlideWndStack::OnReceive(void* pFrame)
{
	//��pFrame��ȡ�������Լ��������������
	int nCmd = 0, nLen = 0;
	void* pData = NULL;
    int nRet = 0;
	ExplainFrame(pFrame, nCmd, pData, nLen);

    m_bHasUnrepliedFrm = TRUE;

	switch (nCmd)
	{
	case APP_CMD_REQ_ACKNUM:	//ͬ��Ӧ��֡��
		//���֡�Ƿ���ȷ
		if (nLen != sizeof(SSyncAckNum))
		{
			TRACE(MTS_NETWORK, S_NET_RECEIVE_ERR_FRAME, nCmd, nLen);
			MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_URGENT, 
				   S_NET_RECEIVE_ERR_FRAME, nCmd, nLen);
		}
		else
		{
			m_nAckNumber = ((SSyncAckNum* )pData)->nAckNum();
			SendResAckNumFrm();
			MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, 
				   S_NET_RECEIVE_ACKNUM_FRAME, m_nAckNumber);
		}

		break;

	case APP_CMD_REQ_SYN:	//ͬ������ͷ��ˮ��
		//���֡�Ƿ���ȷ
		if (nLen != sizeof(SSyncSequ))
		{
			TRACE(MTS_NETWORK, S_NET_RECEIVE_ERR_FRAME, nCmd, nLen);
			MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_URGENT, 
				   S_NET_RECEIVE_ERR_FRAME, nCmd, nLen);
		}
		else
		{
			int nBaseSeq = ((SSyncSequ* )pData)->nSequ();
			m_SlideWnd.Synchronize(nBaseSeq);
			SendAckFrm();
			MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, 
				   S_NET_RECEIVE_SYNC_FRAME, nBaseSeq);
		}

		break;

	case APP_CMD_REQ_DATA:	//����������
        nRet = PreProcessFrame((BYTE*&)pData, nLen);
		//���֡�Ƿ���ȷ
		if (0 != nRet || 0 != nLen % m_nBillMsgSize)	//�����п��ܻ�����Ż���
		{
			//������ 2002-12-07 �����쳣�����澯 
			SInnerAlarm sAlarm;
			sAlarm.yAlarmType = AT_EVENT;
			sAlarm.uAlarmID = ALARM_ID_ABNORMAL_BILL;
			sAlarm.yAlarmPara[0] = ALARM_PARA_FRAME_ERR;
			CMsgProcessor::SendAlarm(&sAlarm);
			//������ ���ӽ���
			TRACE(MTS_NETWORK, S_NET_RECEIVE_ERR_FRAME, nCmd, nLen);
			MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_URGENT, 
				   S_NET_RECEIVE_ERR_FRAME, nCmd, nLen);
		}
		else
		{
			SBillMessage* pMsg = NULL;
			for (int i = 0; i < nLen / m_nBillMsgSize; i++)
			{
				pMsg = (SBillMessage* )((BYTE* )pData + i * m_nBillMsgSize);

				//�Ի������б�Ҫ��Ԥ����
				PreProcessBill(pMsg);
				((CSlideWndNetApp* )m_pNetApp)->m_pAllFrmPerf->Add(1);

				//���������ݷ��뻬��������
				switch (m_SlideWnd.AddRecord(pMsg))
				{
				case SW_REFUSE:	//�������ݱ��ܾ�
					SendRejFrm();
					break;

				case SW_CONCEDE:	//�������ݱ���ǿ����
					((CSlideWndNetApp* )m_pNetApp)->m_pRptBillPerf->Add(1);
					SendAckFrm();
					break;

				case SW_ACCEPT:
					((CSlideWndNetApp* )m_pNetApp)->m_pNmlBillPerf->Add(1);
                    //������ 2003-05-16
                    m_pNetApp->m_pReceiveBillCnt->Add(1);
                    if(++m_nRecvAcceptCount > m_nMaxRecvAcceptCount)
                    {
					    SendAckFrm();
                    }
					break;

				default:
					ASSERT(FALSE);
					break;
				}
			}
		}

		break;

	default:
		TRACE(MTS_NETWORK, S_NET_RECEIVE_UNKNOWN_FRAME, nCmd);
		MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_URGENT, 
			   S_NET_RECEIVE_UNKNOWN_FRAME, nCmd);
		
		break;
	}

	CProtocolStack::OnReceive(pFrame);
}

/**********************************************************************
��������    ���ϴ��ṩ��ǰ����ģ�鱣��Ļ������ݽ��к����Ĵ���
�������    bSaved�����Ƿ񱣴�ɹ�
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSlideWndStack::OnResponse(BOOL const bSaved)
{
	if (bSaved && m_SlideNodeInfo.SSm.uSubmitRecCnt > 0)
	{
		m_SlideWnd.Slide(m_SlideNodeInfo.SSm.uSubmitRecCnt);
		SendAckFrm();
	}

	//����״̬
	m_SlideNodeInfo.SSm.uSubmitRecCnt = 0;
	SaveStatus();
}

/**********************************************************************
��������    ���л���Ԥ����
�������    pMsg����������Ļ�����Ϣ����
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSlideWndStack::PreProcessBill(void* const pMsg)
{
	//��128��STP�Ȳ�Ʒ�У�ȱʡ���Ի�������Ԥ����
	//�����⴦��Ĳ�Ʒ�������������ر�����
}

int CSlideWndStack::PreProcessFrame(BYTE*& pData, int& nLen)
{
	//��GSM��STP�Ȳ�Ʒ�У�ȱʡ���Ի�������Ԥ����
	//�����⴦��Ĳ�Ʒ�������������ر�����
    return 0;
}
/**********************************************************************
��������    ���Ӧ������ģ���GSN����Э��֡
�������    pFrame���������͵���Ϣ
�������    ��
����ֵ      ���͵Ľ����0��ʾ�ɹ�����0��ʾʧ��
�׳��쳣    �ڴ�����ʧ��ʱ�׳��쳣
**********************************************************************/
int CSlideWndStack::SendFrame2Ex(void* pFrame)
{
	//����һ��SNetFrame�ṹ��ʵ��*pMsg��
    MSG_INTRA* pMsg = new(((SMemFrame* )pFrame)->size()) MSG_INTRA;
    if (NULL == pMsg) 
	{
		throw "Fail to new pMsg in CSlideWndStack::SendFrame2Ex";
	}

	pMsg->ReceiverPid = PT_OUTSIDE;
    pMsg->ReceiverMid = MT_UNKNOWN;
    pMsg->SenderPid = CMsgProcessor::GetProcessType();
    pMsg->SenderMid = MT_NETWORK;
	pMsg->AppType = APP_MAIN;

	//��pFrame������pMsg���������У�������
    memcpy(pMsg->Data, pFrame, pMsg->Len);

    return m_pNetApp->SendFrame2Ex(pMsg, this);
}

/**********************************************************************
��������    ���������͵���Ϣ����ȡ���������Լ�����������
�������    pFrame�����������͵���Ϣ
�������    nCmd������Ϣ�е�������
			pData������Ϣ������������ָ��
			nLen������Ϣ�����������ݵĳ���
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSlideWndStack::ExplainFrame(const void* pFrame, int& nCmd, 
								  void*& pData, int& nLen)
{
	//128��STP�Ȳ�Ʒ�У��������͵���Ϣ����SAppFrame�Ľṹ��
	//��������Ĳ�Ʒ����������Ӧ���������ر�����
	SAppFrame* pAppFrame = (SAppFrame* )pFrame;

	nCmd = pAppFrame->byCmd();
	pData = pAppFrame->Data();
	nLen = pAppFrame->nLen() - _APPFRAME_EXPAND_LENGTH;
}

/**********************************************************************
��������    ����ͬ��Ӧ��֡�ŵ�Ӧ��֡
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    �ڴ�����ʧ��ʱ�׳��쳣
**********************************************************************/
void CSlideWndStack::SendResAckNumFrm()
{
	SMemFrame* pAckFrame = new(_APPFRAME_EXHEAD_LENGTH) SMemFrame;
	if (NULL == pAckFrame)
	{
		throw "Fail to new pAckFrame in CSlideWndStack::SendResAckNumFrm";
	}

	pAckFrame->nCmd(MEM_CMD_DATA);
	SAppFrame* pAppFrame = (SAppFrame* )(pAckFrame->Data());
	pAppFrame->sender_pid(APP_PID_HTMAIL);
	pAppFrame->sender_mid(APP_MID_NULL);
    pAppFrame->receiver_pid(APP_PID_BILL);
    pAppFrame->receiver_mid(m_uStackId);
	pAppFrame->nLen(_APPFRAME_EXPAND_LENGTH);
    pAppFrame->sender_desk(APP_DESK_NULL);
    pAppFrame->sender_app(APP_APP_NULL);
    pAppFrame->receiver_desk(APP_DESK_NULL);
    pAppFrame->receiver_app(APP_APP_NULL);
	pAppFrame->byCmd(APP_CMD_RSP_ACKNUM);

	SendFrame2Ex(pAckFrame);

	delete pAckFrame;

    //by ldf 2002-11-19 ��Ӧ���ⵥ D19298
    m_nRecvAcceptCount = 0;
    //�޸����
}

/**********************************************************************
��������    ����ȷ��֡
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSlideWndStack::SendAckFrm()
{
    if(m_bHasUnrepliedFrm)
    {
	    SAppFrame* pAppFrame = (SAppFrame* )(m_pAckFrame->Data());
	    pAppFrame->byCmd(APP_CMD_RSP_ACK);

	    SAckOrRejRes* pAckFrm = (SAckOrRejRes* )(pAppFrame->Data());
	    pAckFrm->nSequ(m_SlideWnd.GetBaseSeqNumber());
	    pAckFrm->nAckNum(++m_nAckNumber);

	    SendFrame2Ex(m_pAckFrame);
        m_bHasUnrepliedFrm = FALSE;
    }

    //by ldf 2002-11-19 ��Ӧ���ⵥ D19298
    m_nRecvAcceptCount = 0;
    //�޸����
}

/**********************************************************************
��������    ���;ܾ�֡
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSlideWndStack::SendRejFrm()
{
	SAppFrame* pAppFrame = (SAppFrame* )(m_pAckFrame->Data());
	pAppFrame->byCmd(APP_CMD_RSP_REJ);

	SAckOrRejRes* pRejFrm = (SAckOrRejRes* )(pAppFrame->Data());
	pRejFrm->nSequ(m_SlideWnd.GetBaseSeqNumber());
	pRejFrm->nAckNum(++m_nAckNumber);

	SendFrame2Ex(m_pAckFrame);

    //by ldf 2002-11-19 ��Ӧ���ⵥ D19298
    m_nRecvAcceptCount = 0;
    //�޸����
}


//���캯��
CGSMStack::CGSMStack(CNetApp* const pNetApp, UINT4 const uStackId)
					: CSlideWndStack(pNetApp, uStackId)
{
}

//��������
CGSMStack::~CGSMStack()
{
}

/**********************************************************************
��������    ���л���Ԥ����
�������    pMsg����������Ļ�����Ϣ����
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CGSMStack::PreProcessBill(void* const pMsg)
{
	//Z30971ȡ��Ϊ�����������ģ��� 2002-12-11 D19842
	//SBillMessage* pBillMsg = (SBillMessage* )pMsg;

	//Ϊ�����������ģ���
	//pBillMsg->Data()[MSC_MOD_NUM_OFFSET] = (BYTE)GetStackId();
}

//���캯��
C128Stack::C128Stack(CNetApp* const pNetApp, UINT4 const uStackId)
					: CSlideWndStack(pNetApp, uStackId)
{
    m_nBuffLen    = 1024;
    m_pBillBuffer = new BYTE[m_nBuffLen];
}

//��������
C128Stack::~C128Stack()
{
    if(NULL != m_pBillBuffer)
    {
        delete m_pBillBuffer;
        m_pBillBuffer = NULL;
    }
}

/**********************************************************************
��������    ���л���֡��Ԥ����
�������    pMsg����������Ļ�����Ϣ����
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
int C128Stack::PreProcessFrame(BYTE*& pData, int& nLen)
{
    int nOutputLen   = 0;
    int nBamBillSize = m_nBillMsgSize - 1;
    BYTE byModule    = m_uStackId;
    if(0 != nLen % nBamBillSize)
    {
        return -1;
    }

    if(m_nBuffLen < nLen / nBamBillSize + nLen)
    {
        delete m_pBillBuffer;
        m_nBuffLen = nLen / nBamBillSize + nLen;
        m_pBillBuffer = new BYTE[m_nBuffLen];
    }
    if(NULL == m_pBillBuffer)
    {
        return -1;
    }
    
    /*Ϊ�����������ģ���*/
	SBillMessage* pBillMsg = NULL;
	for (int i = 0; i < nLen / nBamBillSize; i++)
	{
		pBillMsg = (SBillMessage* )((BYTE* )pData + i * nBamBillSize);
	    memcpy(m_pBillBuffer, pBillMsg, sizeof(INT4));
	    memcpy(m_pBillBuffer + 4, &byModule, 1);
	    memcpy(m_pBillBuffer + 5, pBillMsg->Data(), nBamBillSize);

        nOutputLen += m_nBillMsgSize;
    }

    nLen  = nOutputLen;
    pData = m_pBillBuffer;

    return 0;
}

//���캯��
CSoftXStack::CSoftXStack(CNetApp* const pNetApp, UINT4 const uStackId)
				: CSlideWndStack(pNetApp, uStackId)
{
	m_pSysAckFrame = new(sizeof(SAckOrRejRes)) SSysFrame;
    m_pSysAckFrame->receiver_pid(SYS_PID_HTCOMM);
    m_pSysAckFrame->receiver_mid(uStackId);
    m_pSysAckFrame->sender_pid(SYS_PID_BAU);
    m_pSysAckFrame->sender_mid(SYS_MID_BAU);

	if (NULL == m_pSysAckFrame)
	{
		throw "Fail to new m_pSysAckFrame in CSoftXStack::CSoftXStack.";
	}
}

//��������
CSoftXStack::~CSoftXStack()
{
	if (NULL != m_pSysAckFrame)
	{
		delete m_pSysAckFrame;
		m_pSysAckFrame = NULL;
	}
}

/**********************************************************************
��������    ������뱾Э��ջ��Ӧ������ģ���GSN���͹�����һ֡����
�������    pFrame�����յ���������Ϣ
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSoftXStack::OnReceive(void* pFrame)
{
	//�����յ�����Ϣ��дm_pSysAckFrame�е�PID�Լ�MID
	SSysFrame* pSysFrm = (SSysFrame* )pFrame;
	m_pSysAckFrame->receiver_pid(pSysFrm->sender_pid());
	m_pSysAckFrame->receiver_mid(pSysFrm->sender_mid());
	m_pSysAckFrame->sender_pid(pSysFrm->receiver_pid());
	m_pSysAckFrame->sender_mid(pSysFrm->receiver_mid());

	CSlideWndStack::OnReceive(pFrame);
}

/**********************************************************************
��������    ���Ӧ������ģ���GSN����Э��֡
�������    pFrame���������͵���Ϣ
�������    ��
����ֵ      ���͵Ľ����0��ʾ�ɹ�����0��ʾʧ��
�׳��쳣    �ڴ�����ʧ��ʱ�׳��쳣
**********************************************************************/
int CSoftXStack::SendFrame2Ex(void* pFrame)
{
	//����һ��SNetFrame�ṹ��ʵ��*pMsg��
    MSG_INTRA* pMsg = new(((SSysFrame* )pFrame)->size()) MSG_INTRA;
    if (NULL == pMsg) 
	{
		throw "Fail to new pMsg in CSoftXStack::SendFrame2Ex";
	}

	pMsg->ReceiverPid = PT_OUTSIDE;
    pMsg->ReceiverMid = MT_UNKNOWN;
    pMsg->SenderPid = CMsgProcessor::GetProcessType();
    pMsg->SenderMid = MT_NETWORK;
	pMsg->AppType = APP_MAIN;

	//��pFrame������pMsg���������У�������
    memcpy(pMsg->Data, pFrame, pMsg->Len);

    return m_pNetApp->SendFrame2Ex(pMsg, this);
}

/**********************************************************************
��������    ���������͵���Ϣ����ȡ���������Լ�����������
�������    pFrame�����������͵���Ϣ
�������    nCmd������Ϣ�е�������
			pData������Ϣ������������ָ��
			nLen������Ϣ�����������ݵĳ���
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSoftXStack::ExplainFrame(const void* pFrame, int& nCmd, 
							   void*& pData, int& nLen)
{
	SSysFrame* pSysFrm = (SSysFrame* )pFrame;

	nCmd = pSysFrm->ucCmdCode();
	pData = pSysFrm->Data();
	nLen = pSysFrm->nLen() - _SYSFRAME_EXPAND_LENGTH;
}

/**********************************************************************
��������    ����ͬ��Ӧ��֡�ŵ�Ӧ��֡
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    �ڴ�����ʧ��ʱ�׳��쳣
**********************************************************************/
void CSoftXStack::SendResAckNumFrm()
{
	SSysFrame* pAckFrame = new (0) SSysFrame;
	if (NULL == pAckFrame)
	{
		throw "Fail to new pAckFrame in CSoftXStack::SendResAckNumFrm";
	}

	pAckFrame->sender_pid(m_pSysAckFrame->sender_pid());
	pAckFrame->sender_mid(m_pSysAckFrame->sender_mid());
    pAckFrame->receiver_pid(m_pSysAckFrame->receiver_pid());
    pAckFrame->receiver_mid(m_pSysAckFrame->receiver_mid());
	pAckFrame->ucCmdCode(SYS_CMD_RSP_ACKNUM);

	SendFrame2Ex(pAckFrame);

	delete pAckFrame;

    //by ldf 2002-11-19 ��Ӧ���ⵥ D19298
    m_nRecvAcceptCount = 0;
    //�޸����
}

/**********************************************************************
��������    ����ȷ��֡
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSoftXStack::SendAckFrm()
{
    if(m_bHasUnrepliedFrm)
    {
	    m_pSysAckFrame->ucCmdCode(SYS_CMD_RSP_ACK);

	    SAckOrRejRes* pAckFrm = (SAckOrRejRes* )(m_pSysAckFrame->Data());
	    pAckFrm->nSequ(m_SlideWnd.GetBaseSeqNumber());
	    pAckFrm->nAckNum(++m_nAckNumber);

        SendFrame2Ex(m_pSysAckFrame);
        m_bHasUnrepliedFrm = FALSE;
    }

    //by ldf 2002-11-19 ��Ӧ���ⵥ D19298
    m_nRecvAcceptCount = 0;
    //�޸����
}

/**********************************************************************
��������    ���;ܾ�֡
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSoftXStack::SendRejFrm()
{
	m_pSysAckFrame->ucCmdCode(SYS_CMD_RSP_REJ);

	SAckOrRejRes* pRejFrm = (SAckOrRejRes* )(m_pSysAckFrame->Data());
	pRejFrm->nSequ(m_SlideWnd.GetBaseSeqNumber());
	pRejFrm->nAckNum(++m_nAckNumber);

	SendFrame2Ex(m_pSysAckFrame);

    //by ldf 2002-11-19 ��Ӧ���ⵥ D19298
    m_nRecvAcceptCount = 0;
    //�޸����
}


//���캯��
CSlideWndNetApp::CSlideWndNetApp(CNetWork* const pNetWork)
					: CNetApp(pNetWork)
{
	m_pRptBillPerf = NULL;
    m_pNmlBillPerf = NULL;
    m_pAllFrmPerf = NULL;
}

//��������
CSlideWndNetApp::~CSlideWndNetApp()
{
	//����ͳ������ڴ��ɿ��ͳһ�ͷ�
	m_pRptBillPerf = NULL;
    m_pNmlBillPerf = NULL;
    m_pAllFrmPerf = NULL;
}

int CSlideWndNetApp::Init(void)
{
    int nRet = CNetApp::Init();
    if(ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    BYTE yAPType = (BYTE)CMsgProcessor::GetProcessType() - PT_AP_BASE + 1;
    //ͳ�������յ����ظ���������֡��(SLD_REPEATDATA_FRAME)
	m_pRptBillPerf = CPerfItem::GetItem(yAPType,
                                        MT_NETWORK, 
                                        SLD_REPEATDATA_FRAME);
	//ͳ�������յ���������������֡��(SLD_NORMALDATA_FRAME)
	m_pNmlBillPerf = CPerfItem::GetItem(yAPType,
                                        MT_NETWORK, 
                                        SLD_NORMALDATA_FRAME);
	//ͳ�������յ������л�������֡��(SLD_ALL_FRAME)
	m_pAllFrmPerf = CPerfItem::GetItem(yAPType,
                                       MT_NETWORK, 
                                       SLD_ALL_FRAME);
    
    return ERR_SUCCESS;
}

/**********************************************************************
��������    ����״̬�ļ��ж�ȡ����Э��ջ��״̬��Ϣ
�������    ��
�������    ��
����ֵ      0��ʾ�ɹ�����0��ʾʧ��
�׳��쳣    ��
**********************************************************************/
int CSlideWndNetApp::LoadStatus(void)
{
	//����CNetStatusInfo::EnumStackId�������������Э��ջ�ı�ţ�
	//Ϊÿ����Ŵ���һ��CSlideWndStack����
	//��ӳ���ϵ<���, ��Ӧ��CSlideWndStack�����ָ��>���뵽m_StackMan�У�
	//���ε���ÿ��CSlideWndStack����LoadStatus������
    int nNum = 0;
    UINT4 uStackId;
    while((uStackId = CNetStatusInfo::EnumStackId(nNum++, STACK_SLIDE)) 
          != INVALID_STACK_ID)
    {
		//״̬�ļ��в��ܴ�������ID��ͬ��Э��ջ�������Ǳ�̴���
        ASSERT(m_StackMan.find(uStackId) == m_StackMan.end());

		//����һ����������Э��ջ����
        CProtocolStack* pStack = CreateStack(uStackId);
        ASSERT (NULL != pStack);

        m_StackMan[uStackId] = pStack;

        //װ���Э��ջ��״̬�ļ�
        if(pStack->LoadStatus() != ERR_SUCCESS)
        {
            TRACE(MTS_NETWORK, S_NET_LOAD_SLIDE_STACK, uStackId);
            return ERR_FAIL;
        }
    }

	return 0;
}

/**********************************************************************
��������    ����״̬�ļ���д������Э��ջ��״̬��Ϣ
�������    ��
�������    ��
����ֵ      0��ʾ�ɹ�����0��ʾʧ��
�׳��쳣    ��
**********************************************************************/
int CSlideWndNetApp::SaveStatus(void)
{
	//���ε���ÿ��CSlideWndStack�����SaveStatus����
	int nResult = 0;
	MAP_ID_TO_PS::iterator i;
	for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
	{
		nResult = i->second->SaveStatus();
		if (0 != nResult)
		{
            TRACE(MTS_NETWORK, S_NET_SAVE_SLIDE_STATUS);
			break;
		}
	}

	return nResult;
}

/**********************************************************************
��������    ��������������Э��֡
�������    pFrame�����յ�����Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ����ռ�ʧ��ʱ�׳��쳣
**********************************************************************/
void CSlideWndNetApp::OnReceive(SNetFrame* pFrame)
{
	//��128��STP�Ȳ�Ʒ�У�������ͨ�Ų���SAppFrame�Ľṹ��
	//��������Ĳ�Ʒ�����������������ر�����
	SAppFrame* pAppFrame = (SAppFrame* )(pFrame->Data);

	//�������ϢΪ�͸澯��ص���Ϣ���򽫱���Ϣת�����澯ģ��
	if ((APP_PID_ALARM == pAppFrame->sender_pid()) 
		|| (APP_PID_MEMMAIL == pAppFrame->sender_pid()))
	{
		MSG_INTRA* pAlarmMsg = new(pFrame->Len) MSG_INTRA;
		if (NULL == pAlarmMsg)
		{
            m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_FAIL);
			return;
		}
        m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame);
        
		pAlarmMsg->SenderPid = CMsgProcessor::GetProcessType();
		pAlarmMsg->SenderMid = MT_NETWORK;
		pAlarmMsg->ReceiverPid = PT_OM;
		pAlarmMsg->ReceiverMid = MT_ALARM;
		pAlarmMsg->CmdCode = CMD_ALARM_RESPONSE;
		memcpy(pAlarmMsg->Data, pAppFrame, pAlarmMsg->Len);
		CWorkModule::SendMsg(pAlarmMsg);

		return;
	}

	if (m_bActive)
	{
		//��m_StackMan���ҵ�ģ��Ŷ�Ӧ��CSlideWndStack���󣬲����䴦��
		//�����Ӧ��CSlideWndStack���󲻴��ڣ��򴴽���Ӧ�Ķ���
		if (m_StackMan.find(pAppFrame->sender_mid()) == m_StackMan.end())
		{
			CProtocolStack* pNewStack = CreateStack(pAppFrame->sender_mid());
			m_StackMan[pAppFrame->sender_mid()] = pNewStack;

            char szMsg[100];
            sprintf(szMsg, S_NET_CREATE_SLIDE_STACK, pAppFrame->sender_mid());
            MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, szMsg);
            TRACE (MTS_NETWORK, szMsg);
            CMsgProcessor::WriteLog(MTS_NETWORK, szMsg);
		}

        if((m_Stack_To_RecvInv.size() < m_pNetWork->GetLicenseCount())
            || (m_Stack_To_RecvInv.find((UINT4)m_StackMan[pAppFrame->sender_mid()]) 
                != m_Stack_To_RecvInv.end()))
        {
            m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame);
		    m_StackMan[pAppFrame->sender_mid()]->OnReceive(pAppFrame);
        }
        else
        {
            //ģ����������Ȩ��
            m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_LICENSE_EXPIRED);
        }

		CNetApp::OnReceive(pFrame);
        return;
	}

    m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_APP_NOT_ACTIVED);
}

/**********************************************************************
��������    ת�����Ը澯ģ��ĸ澯֡
�������    pMsg�����Ӹ澯ģ�鷢�͹�������Ϣ
�������    ��
����ֵ      ��
�׳��쳣    ����ռ�ʧ��ʱ�׳��쳣
**********************************************************************/
void CSlideWndNetApp::DispatchAlarm(MSG_INTRA* pMsg)
{
	//�����ǰû���κζ�Ӧ�����ӣ���ֱ�ӷ���
	if (0 == m_StackMan.size())
	{
		MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, 
			   S_NET_NO_LINK_TO_SEND_ALARM);
		return;
	}

	//ѭ��ѡ��һ��ģ��ţ�����ģ��Ÿ�ֵ���澯֡�Ľ���ģ��ţ�����

	static unsigned long uReceiverMid = 0;
	MAP_ID_TO_PS::iterator Index = m_StackMan.find(uReceiverMid);
	MAP_ID_TO_PS::iterator Index2 = Index;
	if (Index2 != m_StackMan.end())
	{
		Index2++;
	}
	if (m_StackMan.end() == Index2)
	{
		Index2 = m_StackMan.begin();
	}
    while(Index2 != Index)
    {
        if(m_Stack_To_RecvInv.find((UINT4)Index2->second) 
            != m_Stack_To_RecvInv.end())
        {
            break;
        }
	    if (Index2 != m_StackMan.end())
	    {
		    Index2++;
	    }
	    if (m_StackMan.end() == Index2)
	    {
		    Index2 = m_StackMan.begin();
	    }
    }

	//�����ǰû���κζ�Ӧ�����ӣ���ֱ�ӷ���
	if (Index == Index2)
	{
		MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, 
			   S_NET_NO_LINK_TO_SEND_ALARM);
		return;
	}
	uReceiverMid = Index2->first;

	//�澯ģ�鷢�͹����ĸ澯��SMemFrame�ṹ
	SMemFrame* pMemFrame = (SMemFrame* )(pMsg->Data);
	SMemAlarmInfo* pAlarmInfo = (SMemAlarmInfo* )(pMemFrame->Data());
	pAlarmInfo->receiver_mid(uReceiverMid);

	MSG_INTRA* pAlarmMsg = new(pMsg->Len) MSG_INTRA;
	if (NULL == pAlarmMsg)
	{
		throw "Fail to new pAlarmMsg in CSlideWndNetApp::DispatchAlarm.";
	}
	memcpy(pAlarmMsg->Data, pMsg->Data, pAlarmMsg->Len);
    pAlarmMsg->ReceiverPid = PT_OUTSIDE;
    pAlarmMsg->ReceiverMid = MT_UNKNOWN;
    pAlarmMsg->SenderPid   = CMsgProcessor::GetProcessType();
    pAlarmMsg->SenderMid   = MT_NETWORK;
    pAlarmMsg->AppType     = APP_MAIN;

	SendFrame2Ex(pAlarmMsg, Index2->second);
}

/**********************************************************************
��������    ����һ���µ�Э��ջ
�������    uStackId������Э��ջ��ID
�������    ��
����ֵ      �´�����Э��ջ
�׳��쳣    ����ռ�ʧ��ʱ�׳��쳣
**********************************************************************/
CProtocolStack* CSlideWndNetApp::CreateStack(UINT4 uStackId)
{
	CSlideWndStack* pNewStack = new CSlideWndStack(this, uStackId);
	if (NULL == pNewStack)
	{
		throw "Fail to new pNewStack in CSlideWndNetApp::CreateStack.";
	}

	return pNewStack;
}

//by ldf 2003-03-24 ��Ӧ���ⵥD07718
void CSlideWndStack::Timer1Sec()
{
    if ((m_SlideWnd.GetRecordCount() > 0) &&
        (m_SlideWnd.GetExpectedSeqNumber() == m_SlideWnd.GetBaseSeqNumber()))
    {
        SendRejFrm();
    }
}
//change end

//���캯��
CGSMNetApp::CGSMNetApp(CNetWork* const pNetWork)
	: CSlideWndNetApp(pNetWork)
{
}

//��������
CGSMNetApp::~CGSMNetApp()
{
}

/**********************************************************************
��������    ����һ���µ�Э��ջ
�������    uStackId������Э��ջ��ID
�������    ��
����ֵ      �´�����Э��ջ
�׳��쳣    ����ռ�ʧ��ʱ�׳��쳣
**********************************************************************/
CProtocolStack* CGSMNetApp::CreateStack(UINT4 uStackId)
{
	CGSMStack* pNewStack = new CGSMStack(this, uStackId);
	if (NULL == pNewStack)
	{
		throw "Fail to new pNewStack in CGSMNetApp::CreateStack.";
	}

	return pNewStack;
}

//���캯��
C128NetApp::C128NetApp(CNetWork* const pNetWork)
	: CSlideWndNetApp(pNetWork)
{
}

//��������
C128NetApp::~C128NetApp()
{
}

/**********************************************************************
��������    ����һ���µ�Э��ջ
�������    uStackId������Э��ջ��ID
�������    ��
����ֵ      �´�����Э��ջ
�׳��쳣    ����ռ�ʧ��ʱ�׳��쳣
**********************************************************************/
CProtocolStack* C128NetApp::CreateStack(UINT4 uStackId)
{
	CProtocolStack* pNewStack = new C128Stack(this, uStackId);
	if (NULL == pNewStack)
	{
		throw "Fail to new pNewStack in C128NetApp::CreateStack.";
	}

	return pNewStack;
}

//���캯��
CSoftXNetApp::CSoftXNetApp(CNetWork* const pNetWork)
				: CSlideWndNetApp(pNetWork)
{
}

//��������
CSoftXNetApp::~CSoftXNetApp()
{
}

/**********************************************************************
��������    ��������������Э��֡
�������    pFrame�����յ�����Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ����ռ�ʧ��ʱ�׳��쳣
**********************************************************************/
void CSoftXNetApp::OnReceive(SNetFrame* pFrame)
{
	//�յ�����Ϣ��ʽΪSSysFrame
	SSysFrame* pSysFrame = (SSysFrame* )(pFrame->Data);

	//�������ϢΪ�͸澯��ص���Ϣ���򽫱���Ϣת�����澯ģ��
	if (SYS_MID_BAM == pSysFrame->sender_mid())
	{
		MSG_INTRA* pAlarmMsg = new(pFrame->Len) MSG_INTRA;
		if (NULL == pAlarmMsg)
		{
            m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_FAIL);
			return;
		}
        m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame);
        
		pAlarmMsg->SenderPid = CMsgProcessor::GetProcessType();
		pAlarmMsg->SenderMid = MT_NETWORK;
		pAlarmMsg->ReceiverPid = PT_OM;
		pAlarmMsg->ReceiverMid = MT_ALARM;
		pAlarmMsg->CmdCode = CMD_ALARM_RESPONSE;
		memcpy(pAlarmMsg->Data, pFrame->Data, pAlarmMsg->Len);
		CWorkModule::SendMsg(pAlarmMsg);

		return;
	}

	if (m_bActive)
	{
		//��m_StackMan���ҵ�ģ��Ŷ�Ӧ��CSoftXStack���󣬲����䴦��
		//�����Ӧ��CSoftXStack���󲻴��ڣ��򴴽���Ӧ�Ķ���
		if (m_StackMan.find(pSysFrame->sender_mid()) == m_StackMan.end())
		{
			CProtocolStack* pNewStack = CreateStack(pSysFrame->sender_mid());
			m_StackMan[pSysFrame->sender_mid()] = pNewStack;

            char szMsg[100];
            sprintf(szMsg, S_NET_CREATE_SLIDE_STACK, pSysFrame->sender_mid());
            MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, szMsg);
            TRACE (MTS_NETWORK, szMsg);
            CMsgProcessor::WriteLog(MTS_NETWORK, szMsg);
		}

        if((m_Stack_To_RecvInv.size() < m_pNetWork->GetLicenseCount())
            || (m_Stack_To_RecvInv.find((UINT4)m_StackMan[pSysFrame->sender_mid()]) 
                != m_Stack_To_RecvInv.end()))
        {
            m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame);
		    m_StackMan[pSysFrame->sender_mid()]->OnReceive(pSysFrame);
        }
        else
        {
            //ģ����������Ȩ��
            m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_LICENSE_EXPIRED);
        }

		CNetApp::OnReceive(pFrame);
        return;
	}

    m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_APP_NOT_ACTIVED);
}

/**********************************************************************
��������    ת�����Ը澯ģ��ĸ澯֡
�������    pMsg�����Ӹ澯ģ�鷢�͹�������Ϣ
�������    ��
����ֵ      ��
�׳��쳣    ����ռ�ʧ��ʱ�׳��쳣
**********************************************************************/
void CSoftXNetApp::DispatchAlarm(MSG_INTRA* pMsg)
{
	ASSERT(SYS_MID_BAM == ((SSysFrame* )(pMsg->Data))->receiver_mid());

	//���澯��Ϣֱ����ͨ�Ų�ת��
	MSG_INTRA* pAlarmMsg = new(pMsg->Len) MSG_INTRA;
	if (NULL == pAlarmMsg)
	{
		throw "Fail to new pAlarmMsg in CSoftXNetApp::DispatchAlarm.";
	}
	memcpy(pAlarmMsg->Data, pMsg->Data, pAlarmMsg->Len);
    pAlarmMsg->ReceiverPid = PT_OUTSIDE;
    pAlarmMsg->ReceiverMid = MT_UNKNOWN;
    pAlarmMsg->SenderPid   = CMsgProcessor::GetProcessType();
    pAlarmMsg->SenderMid   = MT_NETWORK;
    pAlarmMsg->AppType     = APP_MAIN;

	SendFrame2Ex(pAlarmMsg, 0);
}

/**********************************************************************
��������    ����һ���µ�Э��ջ
�������    uStackId������Э��ջ��ID
�������    ��
����ֵ      �´�����Э��ջ
�׳��쳣    ����ռ�ʧ��ʱ�׳��쳣
**********************************************************************/
CProtocolStack* CSoftXNetApp::CreateStack(UINT4 uStackId)
{
	CSoftXStack* pNewStack = new CSoftXStack(this, uStackId);
	if (NULL == pNewStack)
	{
		throw "Fail to new pNewStack in CSoftXNetApp::CreateStack.";
	}

	return pNewStack;
}


//���캯��
CSlideWndProtocolTrack::CSlideWndProtocolTrack()
{
	m_pMmlReport = new CMMLReport("BAU");
}

//��������
CSlideWndProtocolTrack::~CSlideWndProtocolTrack()
{
	if (NULL != m_pMmlReport)
	{
		delete m_pMmlReport;
		m_pMmlReport = NULL;
	}
}

/**********************************************************************
��������    ʵʩ��Э��֡�ķ���,�ɺ����ڲ������ڴ棬�����߲���Ҫ�ͷ�
�������    pFrame��������������Ϣ֡
            nProcRst--������
�������    ��
����ֵ      �������õ��ַ���
�׳��쳣    ��
**********************************************************************/
const char* CSlideWndProtocolTrack::Analysis(void* const pFrame, int nProcRst)
{
    char szBuffer[1024];

    m_pMmlReport->MakeReportHead();
    m_pMmlReport->AddString(S_PROTOCOL_TRACK_HEAD);
    m_pMmlReport->AddString(STR_NEWLINE);

    //��pFrameǿ��ת����SNetFrame*����
    SNetFrame* pMsg = (SNetFrame*) pFrame;
    if (MT_NETWORK == pMsg->ReceiverMid)
    //��ʾ����������������Ϣ
	{
        //���ͷ�
        sprintf(szBuffer, S_MODULE_I, GetSenderMid(pMsg));
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_SENDER, szBuffer);

        //���շ�
        sprintf(szBuffer, S_BAU, CMsgProcessor::g_szPtDes);
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_RECEIVER, szBuffer);

		//���к�ͳһ��Ϊ0
        m_pMmlReport->MakeReportItem2(S_TRACK_SEQ_NUM, "0");

		//�澯Ӧ����Ϣ
		if (IsAlarmResponse(pMsg))
		{
			//��Ϣ����
			m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
										 S_CMD_ALARM_RESPONSE);
			GetAlarmResponseContent(szBuffer, pMsg);
			m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, szBuffer);
		}
		else	//����������Ϣ
		{
			int nCmd, nLen;
			BYTE* pData = NULL;
			ExplainFrame(pMsg, nCmd, pData, nLen);

            STRING strContent = "";
			switch (nCmd)
			{
			case APP_CMD_REQ_ACKNUM:
				m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
											 S_CMD_REQ_ACKNUM);

				if (nLen != sizeof(SSyncAckNum))
				{
					sprintf(szBuffer, S_ERR_LENGTH, nLen);
					m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
												 szBuffer);
				}
				else
				{
					UINT4 uAckNumber = ((SSyncAckNum* )pData)->nAckNum();
					sprintf(szBuffer, S_CONTENT_REQ_ACKNUM, uAckNumber);
                    strContent = szBuffer;
                    strContent += GetProcRstExplain(nProcRst);
					m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
												  strContent.c_str());
				}

				break;

			case APP_CMD_REQ_SYN:
				m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
											 S_CMD_REQ_SYN);

				if (nLen != sizeof(SSyncSequ))
				{
					sprintf(szBuffer, S_ERR_LENGTH, nLen);
					m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, szBuffer);
				}
				else
				{
					INT4 nBaseSeq = ((SSyncSequ* )pData)->nSequ();
					sprintf(szBuffer, S_CONTENT_REQ_SYN, nBaseSeq);
                    strContent = szBuffer;
                    strContent += GetProcRstExplain(nProcRst);
					m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
												  strContent.c_str());
				}

				break;

			case APP_CMD_REQ_DATA:
				m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE,S_CMD_REQ_DATA);
				sprintf(szBuffer, S_CONTENT_REQ_DATA, nLen,((SBillMessage*)pData)->nSequ());
                strContent = szBuffer;
                strContent += GetProcRstExplain(nProcRst);
				m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
											  strContent.c_str());

				break;

			default:
				sprintf(szBuffer, S_CMD_UNKNOWN, nCmd);
				m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, szBuffer);
				m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, " ");

				break;
			}
		}
	}
	else	//��ʾ��BAU������������Ϣ
	{
        //���ͷ�
        sprintf(szBuffer, S_BAU, CMsgProcessor::g_szPtDes);
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_SENDER, szBuffer);

        //���շ�
        sprintf(szBuffer, S_MODULE_I, GetReceiverMid(pMsg));
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_RECEIVER, szBuffer);

		//���к�ͳһ��Ϊ0
        m_pMmlReport->MakeReportItem2(S_TRACK_SEQ_NUM, "0");

		//�澯��Ϣ
		if (IsBinAlarm(pMsg))
		{
			m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, S_CMD_BIN_ALARM);
			GetAlarmContent(szBuffer, pMsg);
			m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, szBuffer);
		}
		else	//����������Ϣ
		{
			int nCmd, nLen;
			BYTE* pData = NULL;
			ExplainFrame(pMsg, nCmd, pData, nLen);
			int nSequ, nAckNum;
            STRING strContent = "";

			switch (nCmd)
			{
			case APP_CMD_RSP_ACKNUM:
				m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
											 S_CMD_RSP_ACKNUM);
				if (nLen != 0)
				{
					sprintf(szBuffer, S_ERR_LENGTH, nLen);
					m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
												  szBuffer);
				}
				else
				{
                    strContent = " ";
                    if(nProcRst != ERR_SUCCESS)
                    {
                        strContent += GetProcRstExplain(nProcRst);
                    }
				    m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
											      strContent.c_str());
				}

				break;

			case APP_CMD_RSP_ACK:
				m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, S_CMD_RSP_ACK);
				if (nLen != sizeof(SAckOrRejRes))
				{
					sprintf(szBuffer, S_ERR_LENGTH, nLen);
					m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
												  szBuffer);
				}
				else
				{
					nSequ = ((SAckOrRejRes* )pData)->nSequ();
					nAckNum = ((SAckOrRejRes* )pData)->nAckNum();
					sprintf(szBuffer, S_CONTENT_RSP_ACK, nSequ, nAckNum);
                    strContent = szBuffer;
                    strContent += GetProcRstExplain(nProcRst);
				    m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
											      strContent.c_str());
				}

				break;

			case APP_CMD_RSP_REJ:
				m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, S_CMD_RSP_REJ);
				if (nLen != sizeof(SAckOrRejRes))
				{
					sprintf(szBuffer, S_ERR_LENGTH, nLen);
					m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
												 szBuffer);
				}
				else
				{
					nSequ = ((SAckOrRejRes* )pData)->nSequ();
					nAckNum = ((SAckOrRejRes* )pData)->nAckNum();
					sprintf(szBuffer, S_CONTENT_RSP_REJ, nSequ, nAckNum);
                    strContent = szBuffer;
                    strContent += GetProcRstExplain(nProcRst);
				    m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
											      strContent.c_str());
				}

				break;

			default:
				sprintf(szBuffer, S_CMD_UNKNOWN, nCmd);
				m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, szBuffer);
				m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, " ");

				break;
			}
		}
	}

    m_pMmlReport->MakeReportTail();
    return m_pMmlReport->GetReport();
}


//���캯��
CAppFrmTrack::CAppFrmTrack()
{
}

//��������
CAppFrmTrack::~CAppFrmTrack()
{
}

/**********************************************************************
��������    ��ָ������Ϣ��������ȡ���������Լ�����������
�������    pMsg��������������Ϣ֡
�������    nCmd����������
			pData��������������
			nLen�����������ݵĳ���
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CAppFrmTrack::ExplainFrame(const SNetFrame* pMsg, 
								int& nCmd, BYTE*& pData, int& nLen)
{
	SAppFrame* pAppFrame = NULL;

    if (MT_NETWORK == pMsg->ReceiverMid)
    //��ʾ����������������Ϣ��pFrame����������ΪSAppFrame�ṹ
	{
		pAppFrame = (SAppFrame* )(pMsg->Data);
	}
	else	//��ʾ�Ƿ�����������Ϣ��pFrame����������ΪSMemFrame�ṹ
	{
		SMemFrame* pMemFrame = (SMemFrame* )(pMsg->Data);
		pAppFrame = (SAppFrame* )(pMemFrame->Data());
	}

	nCmd = pAppFrame->byCmd();
	pData = pAppFrame->Data();
	nLen = pAppFrame->nLen() - _APPFRAME_EXPAND_LENGTH;
}

/**********************************************************************
��������    ��ָ������Ϣ��������ȡ������ģ���
�������    pMsg��������������Ϣ֡
�������    ��
����ֵ      ����ģ���
�׳��쳣    ��
**********************************************************************/
UINT4 CAppFrmTrack::GetSenderMid(const SNetFrame* pMsg)
{
    ASSERT(MT_NETWORK == pMsg->ReceiverMid);

	SAppFrame* pAppFrame = (SAppFrame* )(pMsg->Data);

	return pAppFrame->sender_mid();
}

/**********************************************************************
��������    ��ָ������Ϣ��������ȡ������ģ���
�������    pMsg��������������Ϣ֡
�������    ��
����ֵ      ����ģ���
�׳��쳣    ��
**********************************************************************/
UINT4 CAppFrmTrack::GetReceiverMid(const SNetFrame* pMsg)
{
    ASSERT(MT_NETWORK != pMsg->ReceiverMid);

	SMemFrame* pMemFrame = (SMemFrame* )(pMsg->Data);
	SAppFrame* pAppFrame = (SAppFrame* )(pMemFrame->Data());

	return pAppFrame->receiver_mid();
}

/**********************************************************************
��������    �ж�ָ������Ϣ�Ƿ�Ϊ�����澯Ӧ��
�������    pMsg��������������Ϣ֡
�������    ��
����ֵ      TRUEΪ�澯Ӧ��FALSE�Ǹ澯Ӧ��
�׳��쳣    ��
**********************************************************************/
BOOL CAppFrmTrack::IsAlarmResponse(const SNetFrame* pMsg)
{
    ASSERT(MT_NETWORK == pMsg->ReceiverMid);

	SAppFrame* pAppFrame = (SAppFrame* )(pMsg->Data);

	return ((APP_PID_ALARM == pAppFrame->sender_pid()) 
			|| (APP_PID_MEMMAIL == pAppFrame->sender_pid()));
}

/**********************************************************************
��������    �ж�ָ������Ϣ�Ƿ�Ϊ�����Ƹ澯��Ϣ
�������    pMsg��������������Ϣ֡
�������    ��
����ֵ      TRUEΪ�����Ƹ澯��FALSE�Ƕ����Ƹ澯
�׳��쳣    ��
**********************************************************************/
BOOL CAppFrmTrack::IsBinAlarm(const SNetFrame* pMsg)
{
    ASSERT(MT_NETWORK != pMsg->ReceiverMid);

	SMemFrame* pMemFrame = (SMemFrame* )(pMsg->Data);
	SAppFrame* pAppFrame = (SAppFrame* )(pMemFrame->Data());

	return ((APP_PID_ALARM == pAppFrame->receiver_pid()) 
			|| (APP_PID_MEMMAIL == pAppFrame->receiver_pid()));
}

/**********************************************************************
��������    ����ָ���������澯Ӧ����Ϣ��ȡ��ص�������Ϣ
�������    szBuffer�������ڴ��������Ϣ�Ļ��������ں�����������ͷ�
			pMsg�����������������澯Ӧ����Ϣ
�������    szBuffer���������������Ϣ�Ļ�����
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CAppFrmTrack::GetAlarmResponseContent(char* szBuffer, 
										   const SNetFrame* pMsg)
{
	ASSERT(IsAlarmResponse(pMsg));
    
	SMemAlarmInfo* pRspFrame = (SMemAlarmInfo* )(pMsg->Data);
	UINT2 uAlarmId;
	BYTE byPara;
	uAlarmId = pRspFrame->uAlarmID();
	if (pRspFrame->nLen() > _MEM_ALARM_INFO_EXPAND_LENGTH)
	//�и澯������ֻ��Ϊ1������
	{
		ASSERT(pRspFrame->nLen() == _MEM_ALARM_INFO_EXPAND_LENGTH + 1);

		byPara = pRspFrame->Paras()[0];
	}
	else	//û�и澯��������0xFF��ʾ��Ч�澯����
	{
		byPara = 0xFF;
	}

	sprintf(szBuffer, S_CONTENT_ALARM_RESPONSE, uAlarmId, byPara);
}

/**********************************************************************
��������    ����ָ���ĸ澯��Ϣ��ȡ��ص�������Ϣ
�������    szBuffer�������ڴ��������Ϣ�Ļ��������ں�����������ͷ�
			pMsg�����������ĸ澯��Ϣ
�������    szBuffer���������������Ϣ�Ļ�����
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CAppFrmTrack::GetAlarmContent(char* szBuffer, const SNetFrame* pMsg)
{
	ASSERT(IsBinAlarm(pMsg));

	SMemFrame* pMemFrame = (SMemFrame* )(pMsg->Data);
	SMemAlarmInfo* pAlarmFrame = (SMemAlarmInfo* )(pMemFrame->Data());
	UINT2 uAlarmId;
	BYTE byPara;
	uAlarmId = pAlarmFrame->uAlarmID();
	if (pAlarmFrame->nLen() > _MEM_ALARM_INFO_EXPAND_LENGTH)
	//�и澯������ֻ��Ϊ1������
	{
		ASSERT(pAlarmFrame->nLen() == _MEM_ALARM_INFO_EXPAND_LENGTH + 1);

		byPara = pAlarmFrame->Paras()[0];
	}
	else	//û�и澯��������0xFF��ʾ��Ч�澯����
	{
		byPara = 0xFF;
	}

	sprintf(szBuffer, S_CONTENT_BIN_ALARM, uAlarmId, byPara);
}


//���캯��
CSysFrmTrack::CSysFrmTrack()
{
}

//��������
CSysFrmTrack::~CSysFrmTrack()
{
}

/**********************************************************************
��������    ��ָ������Ϣ��������ȡ���������Լ�����������
�������    pMsg��������������Ϣ֡
�������    nCmd����������
			pData��������������
			nLen�����������ݵĳ���
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSysFrmTrack::ExplainFrame(const SNetFrame* pMsg, 
								int& nCmd, BYTE*& pData, int& nLen)
{
	SSysFrame* pSysFrm = (SSysFrame* )(pMsg->Data);

	nCmd = pSysFrm->ucCmdCode();
	pData = pSysFrm->Data();
	nLen = pSysFrm->nLen() - _SYSFRAME_EXPAND_LENGTH;
}

/**********************************************************************
��������    ��ָ������Ϣ��������ȡ������ģ���
�������    pMsg��������������Ϣ֡
�������    ��
����ֵ      ����ģ���
�׳��쳣    ��
**********************************************************************/
UINT4 CSysFrmTrack::GetSenderMid(const SNetFrame* pMsg)
{
    ASSERT(MT_NETWORK == pMsg->ReceiverMid);

	SSysFrame* pSysFrm = (SSysFrame* )(pMsg->Data);

	return pSysFrm->sender_mid();
}

/**********************************************************************
��������    ��ָ������Ϣ��������ȡ������ģ���
�������    pMsg��������������Ϣ֡
�������    ��
����ֵ      ����ģ���
�׳��쳣    ��
**********************************************************************/
UINT4 CSysFrmTrack::GetReceiverMid(const SNetFrame* pMsg)
{
    ASSERT(MT_NETWORK != pMsg->ReceiverMid);

	SSysFrame* pSysFrm = (SSysFrame* )(pMsg->Data);

	return pSysFrm->receiver_mid();
}

/**********************************************************************
��������    �ж�ָ������Ϣ�Ƿ�Ϊ�����澯Ӧ��
�������    pMsg��������������Ϣ֡
�������    ��
����ֵ      TRUEΪ�澯Ӧ��FALSE�Ǹ澯Ӧ��
�׳��쳣    ��
**********************************************************************/
BOOL CSysFrmTrack::IsAlarmResponse(const SNetFrame* pMsg)
{
    ASSERT(MT_NETWORK == pMsg->ReceiverMid);

	return (SYS_MID_BAM == GetSenderMid(pMsg));
}

/**********************************************************************
��������    �ж�ָ������Ϣ�Ƿ�Ϊ�����Ƹ澯��Ϣ
�������    pMsg��������������Ϣ֡
�������    ��
����ֵ      TRUEΪ�����Ƹ澯��FALSE�Ƕ����Ƹ澯
�׳��쳣    ��
**********************************************************************/
BOOL CSysFrmTrack::IsBinAlarm(const SNetFrame* pMsg)
{
    ASSERT(MT_NETWORK != pMsg->ReceiverMid);

	return (SYS_MID_BAM == GetReceiverMid(pMsg));
}

/**********************************************************************
��������    ����ָ���������澯Ӧ����Ϣ��ȡ��ص�������Ϣ
�������    szBuffer�������ڴ��������Ϣ�Ļ��������ں�����������ͷ�
			pMsg�����������������澯Ӧ����Ϣ
�������    szBuffer���������������Ϣ�Ļ�����
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSysFrmTrack::GetAlarmResponseContent(char* szBuffer, 
										   const SNetFrame* pMsg)
{
	ASSERT(IsAlarmResponse(pMsg));
    //������ ������������ֻ��Sys֡ͷ��û�и澯��
	SSysFrame* pSysFrm = (SSysFrame* )(pMsg->Data);
	
	sprintf(szBuffer, S_CONTENT_ALARM_RESPONSE,pSysFrm->ucHandle());
}

/**********************************************************************
��������    ����ָ���ĸ澯��Ϣ��ȡ��ص�������Ϣ
�������    szBuffer�������ڴ��������Ϣ�Ļ��������ں�����������ͷ�
			pMsg�����������ĸ澯��Ϣ
�������    szBuffer���������������Ϣ�Ļ�����
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSysFrmTrack::GetAlarmContent(char* szBuffer, const SNetFrame* pMsg)
{
	ASSERT(IsBinAlarm(pMsg));

	UINT4 uAlarmId;
	BYTE byPara;
	SSysFrame* pSysFrm = (SSysFrame* )(pMsg->Data);
	SSysAlarmInfo* pAlarmInfo = (SSysAlarmInfo* )(pSysFrm->Data());

	uAlarmId = pAlarmInfo->ulAlarmID();
	if (pAlarmInfo->uwParaLength() > 1)	//�и澯����
	{
	    //��Ȼ���������ĸ澯��������������ʾһ���͹���
		//����һ����BAU������Ϣ
		byPara = pAlarmInfo->ucPara[0];
	}
	else	//û�и澯��������0xFF��ʾ��Ч�澯����
	{
		byPara = 0xFF;
	}

	sprintf(szBuffer, S_CONTENT_BIN_ALARM, uAlarmId, byPara, pSysFrm->ucHandle());
}
