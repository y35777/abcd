//本文件由颜文远2002.04.09生成


#include "slide_wnd_related.h"
#include "network.h"
#include "../utility/mml_report.h"

#define INVALID_RECORD_SEQNUMBER	-1
#define MSC_MOD_NUM_OFFSET			17

//往滑动窗口加入一张话单可能的返回结构
enum AddRecResult
{
	SW_ACCEPT,	//被正常接收
	SW_CONCEDE,	//勉强接收，在滑动窗口前一个主机窗口内，认为是重复帧
	SW_REFUSE,	//被拒绝
};

//构造函数
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

//析构函数
CSlideWnd::~CSlideWnd()
{
    if (m_pData)
    {
        delete[] m_pData;
		m_pData = NULL;
    }
}

/**********************************************************************
功能描述    初始化滑动窗口
输入参数    nSize——滑动窗口大小
			nBaseSeqNumber——滑动窗口的起始流水号
			nBillMsgSize——滑动窗口中的消息大小
			nBillRecordSize——话单的长度
			nMpuWindowSize——主机的滑动窗口大小
输出参数    无
返回值      无
抛出异常    在申请空间失败时抛出异常
**********************************************************************/
void CSlideWnd::Init(int nSize, int nBaseSeqNumber, int nBillMsgSize, 
					 int nBillRecordSize, int nMpuWindowSize)
{
    ASSERT(nSize > 0);
    ASSERT(nBaseSeqNumber >= 0);
	ASSERT(nBillRecordSize > 0);
	ASSERT(nBillMsgSize == nBillRecordSize + _BILLMSG_HEAD_LENGTH);
	ASSERT(nMpuWindowSize > 0);

    //滑动窗口采用数组方式存储数据。此处重新分配数组空间m_pData
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

    //使用负数表示非法序列号，将所有记录的序列号
    //置为负数以清空滑动窗口
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
功能描述    同步滑动窗口
输入参数    nBaseSeqNumber——新的起始流水号
输出参数    无
返回值      无
抛出异常    无
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
功能描述    向滑动窗口中添加一条记录
输入参数    pMsg——待加入的话单
输出参数    无
返回值      添加记录的结果
				SW_ACCEPT -- 被正常接收
				SW_CONCEDE -- 勉强接收，在滑动窗口前一个主机窗口内，
								认为是重复帧，应向前台发送应答帧
				SW_REFUSE -- 被拒绝，应向前台发送拒绝帧
说明:
			话单可能落入三个范围：滑动窗口内，滑动窗口前的一个
			主机窗口和其他范围。若落在滑动窗口内，则保留话单内
			容，等待正常的收集->发送过程；若落在滑动窗口前的一
			个主机窗口内，则认为是主机重复发送，应向主机发送应
			答帧。其它范围应发送拒绝帧。
抛出异常    无
**********************************************************************/
int CSlideWnd::AddRecord(SBillMessage *pMsg)
{
	ASSERT(m_nBaseSeqNumber <= m_nExpectedSeqNumber);

    //判断话单是否可以接受
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

    //将落入滑动窗口之内的话单放入滑动窗口
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
			//往后调整m_nExpectedSeqNumber的值，直到找到一个未存放话单的位置
			//或超过滑动窗口范围为止
			do
			{
				m_nExpectedSeqNumber++;

				nIndex = m_nExpectedSeqNumber - m_nBaseSeqNumber;
				nIndex = (nIndex + m_nHead) % m_nSize;
				pMsgBuf = (SBillMessage* )(m_pData + m_nBillMsgSize * nIndex);
			}while ((INVALID_RECORD_SEQNUMBER != pMsgBuf->nSequ()) 
				    && (m_nExpectedSeqNumber - m_nBaseSeqNumber) < m_nSize);
			        //Z30971修改问题单D19843 2002-12-18
		}

		return SW_ACCEPT;
    }

    return SW_CONCEDE;
}

/**********************************************************************
功能描述    向前滑动滑动窗口
输入参数    nHops——向前滑动的步数
输出参数    无
返回值      无
抛出异常    无
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
功能描述    返回可提交记录
输入参数    pDataBuffer——存放待提交话单的缓冲区
输出参数    无
返回值      提交的话单条数
抛出异常    无
**********************************************************************/
int CSlideWnd::GetReadyData(CDataBuffer* pDataBuffer)
{
	ASSERT(m_nBaseSeqNumber <= m_nExpectedSeqNumber);

    int nIndex = 0, nCount = 0;

    //复制数据到缓冲区
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


//构造函数
CSlideWndStack::CSlideWndStack(CNetApp* const pNetApp, UINT4 const uStackId)
				: CProtocolStack(pNetApp, uStackId)
{
	m_nAckNumber = 0;
	memset(&m_SlideNodeInfo, 0, SLIDE_MOD_INFO_LENGTH);
	m_SlideNodeInfo.yModule = uStackId;

	//初始化m_pAckFrame
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

	//创建本协议栈的状态对象
	m_pStatusInfo = new CStackStatusInfo(STACK_SLIDE, uStackId);
	if (NULL == m_pStatusInfo)
	{
		throw "Failed to new m_pStatusInfo in CSlideWndStack::CSlideWndStack.";
	}

    //读取本进程对应的相关配置参数
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

//析构函数
CSlideWndStack::~CSlideWndStack()
{
	if (NULL != m_pAckFrame)
	{
		delete m_pAckFrame;
		m_pAckFrame = NULL;
	}
}

//返回本对象中当前已接收的且可以提交给前存盘模块保存的话单数据的字节数
int CSlideWndStack::GetReadyDataSize(void)
{
	return m_SlideWnd.GetReadyRecordCount() * m_nBillRecordSize;
}

//返回本对象中当前已接收的且可以提交给前存盘模块保存的话单数据的记录数
int CSlideWndStack::GetReadyRecordCount(void)
{
	return m_SlideWnd.GetReadyRecordCount();
}

//将本协议栈可提交给前存盘模块保存的话单数据放入*pDataBuf对象中
void CSlideWndStack::GetReadyData(CDataBuffer* const pDataBuf)
{
	m_SlideNodeInfo.SSm.uSubmitRecCnt = m_SlideWnd.GetReadyData(pDataBuf);
}

/**********************************************************************
功能描述    从主状态文件中读取本协议栈的状态信息
输入参数    无
输出参数    无
返回值      0表示成功，非0表示失败
抛出异常    无
**********************************************************************/
int CSlideWndStack::LoadStatus(void)
{
    UINT4 uInputLen = m_pStatusInfo->Get((BYTE*)(&m_SlideNodeInfo), 
                                         SLIDE_MOD_INFO_LENGTH);
    //读状态文件不成功则返回失败
    if(uInputLen != SLIDE_MOD_INFO_LENGTH) 
	{
		return ERR_FAIL;
	}

	m_SlideWnd.Synchronize(m_SlideNodeInfo.SSm.uBaseSeqNum);

	return 0;
}

/**********************************************************************
功能描述    向主状态文件中写入本协议栈的状态信息
输入参数    无
输出参数    无
返回值      0表示成功，非0表示失败
抛出异常    无
**********************************************************************/
int CSlideWndStack::SaveStatus(void)
{
	m_SlideNodeInfo.SSm.uBaseSeqNum = m_SlideWnd.GetBaseSeqNumber();
	m_pStatusInfo->Set((BYTE* )&m_SlideNodeInfo, SLIDE_MOD_INFO_LENGTH);

	return 0;
}

/**********************************************************************
功能描述    处理从与本协议栈对应的主机模块或GSN发送过来的一帧数据
输入参数    pFrame——收到的主机消息
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CSlideWndStack::OnReceive(void* pFrame)
{
	//从pFrame获取命令码以及所带的相关数据
	int nCmd = 0, nLen = 0;
	void* pData = NULL;
    int nRet = 0;
	ExplainFrame(pFrame, nCmd, pData, nLen);

    m_bHasUnrepliedFrm = TRUE;

	switch (nCmd)
	{
	case APP_CMD_REQ_ACKNUM:	//同步应答帧号
		//检查帧是否正确
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

	case APP_CMD_REQ_SYN:	//同步话单头流水号
		//检查帧是否正确
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

	case APP_CMD_REQ_DATA:	//处理话单数据
        nRet = PreProcessFrame((BYTE*&)pData, nLen);
		//检查帧是否正确
		if (0 != nRet || 0 != nLen % m_nBillMsgSize)	//数据中可能会带多张话单
		{
			//张玉新 2002-12-07 增加异常话单告警 
			SInnerAlarm sAlarm;
			sAlarm.yAlarmType = AT_EVENT;
			sAlarm.uAlarmID = ALARM_ID_ABNORMAL_BILL;
			sAlarm.yAlarmPara[0] = ALARM_PARA_FRAME_ERR;
			CMsgProcessor::SendAlarm(&sAlarm);
			//张玉新 增加结束
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

				//对话单进行必要的预处理
				PreProcessBill(pMsg);
				((CSlideWndNetApp* )m_pNetApp)->m_pAllFrmPerf->Add(1);

				//将话单数据放入滑动窗口中
				switch (m_SlideWnd.AddRecord(pMsg))
				{
				case SW_REFUSE:	//话单数据被拒绝
					SendRejFrm();
					break;

				case SW_CONCEDE:	//话单数据被勉强接收
					((CSlideWndNetApp* )m_pNetApp)->m_pRptBillPerf->Add(1);
					SendAckFrm();
					break;

				case SW_ACCEPT:
					((CSlideWndNetApp* )m_pNetApp)->m_pNmlBillPerf->Add(1);
                    //张玉新 2003-05-16
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
功能描述    对上次提供给前存盘模块保存的话单数据进行后续的处理
输入参数    bSaved——是否保存成功
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CSlideWndStack::OnResponse(BOOL const bSaved)
{
	if (bSaved && m_SlideNodeInfo.SSm.uSubmitRecCnt > 0)
	{
		m_SlideWnd.Slide(m_SlideNodeInfo.SSm.uSubmitRecCnt);
		SendAckFrm();
	}

	//保存状态
	m_SlideNodeInfo.SSm.uSubmitRecCnt = 0;
	SaveStatus();
}

/**********************************************************************
功能描述    进行话单预处理
输入参数    pMsg——待处理的话单消息数据
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CSlideWndStack::PreProcessBill(void* const pMsg)
{
	//在128、STP等产品中，缺省不对话单进行预处理
	//有特殊处理的产品可在子类中重载本函数
}

int CSlideWndStack::PreProcessFrame(BYTE*& pData, int& nLen)
{
	//在GSM、STP等产品中，缺省不对话单进行预处理
	//有特殊处理的产品可在子类中重载本函数
    return 0;
}
/**********************************************************************
功能描述    向对应的主机模块或GSN发送协议帧
输入参数    pFrame——待发送的消息
输出参数    无
返回值      发送的结果，0表示成功，非0表示失败
抛出异常    内存申请失败时抛出异常
**********************************************************************/
int CSlideWndStack::SendFrame2Ex(void* pFrame)
{
	//创建一个SNetFrame结构的实例*pMsg，
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

	//将pFrame拷贝到pMsg的数据区中，并发送
    memcpy(pMsg->Data, pFrame, pMsg->Len);

    return m_pNetApp->SendFrame2Ex(pMsg, this);
}

/**********************************************************************
功能描述    从主机发送的消息中提取出命令码以及所带的数据
输入参数    pFrame——主机发送的消息
输出参数    nCmd——消息中的命令码
			pData——消息中所带的数据指针
			nLen——消息中所带的数据的长度
返回值      无
抛出异常    无
**********************************************************************/
void CSlideWndStack::ExplainFrame(const void* pFrame, int& nCmd, 
								  void*& pData, int& nLen)
{
	//128、STP等产品中，主机发送的消息采用SAppFrame的结构，
	//对于特殊的产品，可以在相应子类中重载本函数
	SAppFrame* pAppFrame = (SAppFrame* )pFrame;

	nCmd = pAppFrame->byCmd();
	pData = pAppFrame->Data();
	nLen = pAppFrame->nLen() - _APPFRAME_EXPAND_LENGTH;
}

/**********************************************************************
功能描述    发送同步应答帧号的应答帧
输入参数    无
输出参数    无
返回值      无
抛出异常    内存申请失败时抛出异常
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

    //by ldf 2002-11-19 对应问题单 D19298
    m_nRecvAcceptCount = 0;
    //修改完毕
}

/**********************************************************************
功能描述    发送确认帧
输入参数    无
输出参数    无
返回值      无
抛出异常    无
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

    //by ldf 2002-11-19 对应问题单 D19298
    m_nRecvAcceptCount = 0;
    //修改完毕
}

/**********************************************************************
功能描述    发送拒绝帧
输入参数    无
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CSlideWndStack::SendRejFrm()
{
	SAppFrame* pAppFrame = (SAppFrame* )(m_pAckFrame->Data());
	pAppFrame->byCmd(APP_CMD_RSP_REJ);

	SAckOrRejRes* pRejFrm = (SAckOrRejRes* )(pAppFrame->Data());
	pRejFrm->nSequ(m_SlideWnd.GetBaseSeqNumber());
	pRejFrm->nAckNum(++m_nAckNumber);

	SendFrame2Ex(m_pAckFrame);

    //by ldf 2002-11-19 对应问题单 D19298
    m_nRecvAcceptCount = 0;
    //修改完毕
}


//构造函数
CGSMStack::CGSMStack(CNetApp* const pNetApp, UINT4 const uStackId)
					: CSlideWndStack(pNetApp, uStackId)
{
}

//析构函数
CGSMStack::~CGSMStack()
{
}

/**********************************************************************
功能描述    进行话单预处理
输入参数    pMsg——待处理的话单消息数据
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CGSMStack::PreProcessBill(void* const pMsg)
{
	//Z30971取消为话单数据添加模块号 2002-12-11 D19842
	//SBillMessage* pBillMsg = (SBillMessage* )pMsg;

	//为话单数据添加模块号
	//pBillMsg->Data()[MSC_MOD_NUM_OFFSET] = (BYTE)GetStackId();
}

//构造函数
C128Stack::C128Stack(CNetApp* const pNetApp, UINT4 const uStackId)
					: CSlideWndStack(pNetApp, uStackId)
{
    m_nBuffLen    = 1024;
    m_pBillBuffer = new BYTE[m_nBuffLen];
}

//析构函数
C128Stack::~C128Stack()
{
    if(NULL != m_pBillBuffer)
    {
        delete m_pBillBuffer;
        m_pBillBuffer = NULL;
    }
}

/**********************************************************************
功能描述    进行话单帧的预处理
输入参数    pMsg——待处理的话单消息数据
输出参数    无
返回值      无
抛出异常    无
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
    
    /*为话单数据添加模块号*/
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

//构造函数
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

//析构函数
CSoftXStack::~CSoftXStack()
{
	if (NULL != m_pSysAckFrame)
	{
		delete m_pSysAckFrame;
		m_pSysAckFrame = NULL;
	}
}

/**********************************************************************
功能描述    处理从与本协议栈对应的主机模块或GSN发送过来的一帧数据
输入参数    pFrame——收到的主机消息
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CSoftXStack::OnReceive(void* pFrame)
{
	//根据收到的消息填写m_pSysAckFrame中的PID以及MID
	SSysFrame* pSysFrm = (SSysFrame* )pFrame;
	m_pSysAckFrame->receiver_pid(pSysFrm->sender_pid());
	m_pSysAckFrame->receiver_mid(pSysFrm->sender_mid());
	m_pSysAckFrame->sender_pid(pSysFrm->receiver_pid());
	m_pSysAckFrame->sender_mid(pSysFrm->receiver_mid());

	CSlideWndStack::OnReceive(pFrame);
}

/**********************************************************************
功能描述    向对应的主机模块或GSN发送协议帧
输入参数    pFrame——待发送的消息
输出参数    无
返回值      发送的结果，0表示成功，非0表示失败
抛出异常    内存申请失败时抛出异常
**********************************************************************/
int CSoftXStack::SendFrame2Ex(void* pFrame)
{
	//创建一个SNetFrame结构的实例*pMsg，
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

	//将pFrame拷贝到pMsg的数据区中，并发送
    memcpy(pMsg->Data, pFrame, pMsg->Len);

    return m_pNetApp->SendFrame2Ex(pMsg, this);
}

/**********************************************************************
功能描述    从主机发送的消息中提取出命令码以及所带的数据
输入参数    pFrame——主机发送的消息
输出参数    nCmd——消息中的命令码
			pData——消息中所带的数据指针
			nLen——消息中所带的数据的长度
返回值      无
抛出异常    无
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
功能描述    发送同步应答帧号的应答帧
输入参数    无
输出参数    无
返回值      无
抛出异常    内存申请失败时抛出异常
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

    //by ldf 2002-11-19 对应问题单 D19298
    m_nRecvAcceptCount = 0;
    //修改完毕
}

/**********************************************************************
功能描述    发送确认帧
输入参数    无
输出参数    无
返回值      无
抛出异常    无
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

    //by ldf 2002-11-19 对应问题单 D19298
    m_nRecvAcceptCount = 0;
    //修改完毕
}

/**********************************************************************
功能描述    发送拒绝帧
输入参数    无
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CSoftXStack::SendRejFrm()
{
	m_pSysAckFrame->ucCmdCode(SYS_CMD_RSP_REJ);

	SAckOrRejRes* pRejFrm = (SAckOrRejRes* )(m_pSysAckFrame->Data());
	pRejFrm->nSequ(m_SlideWnd.GetBaseSeqNumber());
	pRejFrm->nAckNum(++m_nAckNumber);

	SendFrame2Ex(m_pSysAckFrame);

    //by ldf 2002-11-19 对应问题单 D19298
    m_nRecvAcceptCount = 0;
    //修改完毕
}


//构造函数
CSlideWndNetApp::CSlideWndNetApp(CNetWork* const pNetWork)
					: CNetApp(pNetWork)
{
	m_pRptBillPerf = NULL;
    m_pNmlBillPerf = NULL;
    m_pAllFrmPerf = NULL;
}

//析构函数
CSlideWndNetApp::~CSlideWndNetApp()
{
	//性能统计项的内存由框架统一释放
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
    //统计所接收到的重复话单数据帧数(SLD_REPEATDATA_FRAME)
	m_pRptBillPerf = CPerfItem::GetItem(yAPType,
                                        MT_NETWORK, 
                                        SLD_REPEATDATA_FRAME);
	//统计所接收到的正常话单数据帧数(SLD_NORMALDATA_FRAME)
	m_pNmlBillPerf = CPerfItem::GetItem(yAPType,
                                        MT_NETWORK, 
                                        SLD_NORMALDATA_FRAME);
	//统计所接收到的所有话单数据帧数(SLD_ALL_FRAME)
	m_pAllFrmPerf = CPerfItem::GetItem(yAPType,
                                       MT_NETWORK, 
                                       SLD_ALL_FRAME);
    
    return ERR_SUCCESS;
}

/**********************************************************************
功能描述    从主状态文件中读取所有协议栈的状态信息
输入参数    无
输出参数    无
返回值      0表示成功，非0表示失败
抛出异常    无
**********************************************************************/
int CSlideWndNetApp::LoadStatus(void)
{
	//调用CNetStatusInfo::EnumStackId函数，获得所有协议栈的编号；
	//为每个编号创建一个CSlideWndStack对象；
	//将映射关系<编号, 对应的CSlideWndStack对象的指针>加入到m_StackMan中；
	//依次调用每个CSlideWndStack对象LoadStatus函数；
    int nNum = 0;
    UINT4 uStackId;
    while((uStackId = CNetStatusInfo::EnumStackId(nNum++, STACK_SLIDE)) 
          != INVALID_STACK_ID)
    {
		//状态文件中不能存在两个ID相同的协议栈，否则是编程错误
        ASSERT(m_StackMan.find(uStackId) == m_StackMan.end());

		//创建一个滑动窗口协议栈对象
        CProtocolStack* pStack = CreateStack(uStackId);
        ASSERT (NULL != pStack);

        m_StackMan[uStackId] = pStack;

        //装入该协议栈的状态文件
        if(pStack->LoadStatus() != ERR_SUCCESS)
        {
            TRACE(MTS_NETWORK, S_NET_LOAD_SLIDE_STACK, uStackId);
            return ERR_FAIL;
        }
    }

	return 0;
}

/**********************************************************************
功能描述    向主状态文件中写入所有协议栈的状态信息
输入参数    无
输出参数    无
返回值      0表示成功，非0表示失败
抛出异常    无
**********************************************************************/
int CSlideWndNetApp::SaveStatus(void)
{
	//依次调用每个CSlideWndStack对象的SaveStatus函数
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
功能描述    处理来自网络层的协议帧
输入参数    pFrame——收到的消息帧
输出参数    无
返回值      无
抛出异常    申请空间失败时抛出异常
**********************************************************************/
void CSlideWndNetApp::OnReceive(SNetFrame* pFrame)
{
	//在128、STP等产品中，与主机通信采用SAppFrame的结构，
	//对于特殊的产品，必须在子类中重载本函数
	SAppFrame* pAppFrame = (SAppFrame* )(pFrame->Data);

	//如果本消息为和告警相关的消息，则将本消息转发给告警模块
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
		//在m_StackMan中找到模块号对应的CSlideWndStack对象，并交其处理，
		//如果对应的CSlideWndStack对象不存在，则创建相应的对象
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
            //模块数超过授权数
            m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_LICENSE_EXPIRED);
        }

		CNetApp::OnReceive(pFrame);
        return;
	}

    m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_APP_NOT_ACTIVED);
}

/**********************************************************************
功能描述    转发来自告警模块的告警帧
输入参数    pMsg——从告警模块发送过来的消息
输出参数    无
返回值      无
抛出异常    申请空间失败时抛出异常
**********************************************************************/
void CSlideWndNetApp::DispatchAlarm(MSG_INTRA* pMsg)
{
	//如果当前没有任何对应的连接，则直接返回
	if (0 == m_StackMan.size())
	{
		MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, 
			   S_NET_NO_LINK_TO_SEND_ALARM);
		return;
	}

	//循序选择一个模块号，将该模块号赋值给告警帧的接收模块号，发送

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

	//如果当前没有任何对应的连接，则直接返回
	if (Index == Index2)
	{
		MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, 
			   S_NET_NO_LINK_TO_SEND_ALARM);
		return;
	}
	uReceiverMid = Index2->first;

	//告警模块发送过来的告警是SMemFrame结构
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
功能描述    创建一个新的协议栈
输入参数    uStackId——新协议栈的ID
输出参数    无
返回值      新创建的协议栈
抛出异常    申请空间失败时抛出异常
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

//by ldf 2003-03-24 对应问题单D07718
void CSlideWndStack::Timer1Sec()
{
    if ((m_SlideWnd.GetRecordCount() > 0) &&
        (m_SlideWnd.GetExpectedSeqNumber() == m_SlideWnd.GetBaseSeqNumber()))
    {
        SendRejFrm();
    }
}
//change end

//构造函数
CGSMNetApp::CGSMNetApp(CNetWork* const pNetWork)
	: CSlideWndNetApp(pNetWork)
{
}

//析构函数
CGSMNetApp::~CGSMNetApp()
{
}

/**********************************************************************
功能描述    创建一个新的协议栈
输入参数    uStackId——新协议栈的ID
输出参数    无
返回值      新创建的协议栈
抛出异常    申请空间失败时抛出异常
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

//构造函数
C128NetApp::C128NetApp(CNetWork* const pNetWork)
	: CSlideWndNetApp(pNetWork)
{
}

//析构函数
C128NetApp::~C128NetApp()
{
}

/**********************************************************************
功能描述    创建一个新的协议栈
输入参数    uStackId——新协议栈的ID
输出参数    无
返回值      新创建的协议栈
抛出异常    申请空间失败时抛出异常
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

//构造函数
CSoftXNetApp::CSoftXNetApp(CNetWork* const pNetWork)
				: CSlideWndNetApp(pNetWork)
{
}

//析构函数
CSoftXNetApp::~CSoftXNetApp()
{
}

/**********************************************************************
功能描述    处理来自网络层的协议帧
输入参数    pFrame——收到的消息帧
输出参数    无
返回值      无
抛出异常    申请空间失败时抛出异常
**********************************************************************/
void CSoftXNetApp::OnReceive(SNetFrame* pFrame)
{
	//收到的消息格式为SSysFrame
	SSysFrame* pSysFrame = (SSysFrame* )(pFrame->Data);

	//如果本消息为和告警相关的消息，则将本消息转发给告警模块
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
		//在m_StackMan中找到模块号对应的CSoftXStack对象，并交其处理，
		//如果对应的CSoftXStack对象不存在，则创建相应的对象
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
            //模块数超过授权数
            m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_LICENSE_EXPIRED);
        }

		CNetApp::OnReceive(pFrame);
        return;
	}

    m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_APP_NOT_ACTIVED);
}

/**********************************************************************
功能描述    转发来自告警模块的告警帧
输入参数    pMsg——从告警模块发送过来的消息
输出参数    无
返回值      无
抛出异常    申请空间失败时抛出异常
**********************************************************************/
void CSoftXNetApp::DispatchAlarm(MSG_INTRA* pMsg)
{
	ASSERT(SYS_MID_BAM == ((SSysFrame* )(pMsg->Data))->receiver_mid());

	//将告警消息直接向通信层转发
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
功能描述    创建一个新的协议栈
输入参数    uStackId——新协议栈的ID
输出参数    无
返回值      新创建的协议栈
抛出异常    申请空间失败时抛出异常
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


//构造函数
CSlideWndProtocolTrack::CSlideWndProtocolTrack()
{
	m_pMmlReport = new CMMLReport("BAU");
}

//析构函数
CSlideWndProtocolTrack::~CSlideWndProtocolTrack()
{
	if (NULL != m_pMmlReport)
	{
		delete m_pMmlReport;
		m_pMmlReport = NULL;
	}
}

/**********************************************************************
功能描述    实施对协议帧的分析,由函数内部分配内存，调用者不需要释放
输入参数    pFrame——待分析的消息帧
            nProcRst--处理结果
输出参数    无
返回值      分析所得的字符串
抛出异常    无
**********************************************************************/
const char* CSlideWndProtocolTrack::Analysis(void* const pFrame, int nProcRst)
{
    char szBuffer[1024];

    m_pMmlReport->MakeReportHead();
    m_pMmlReport->AddString(S_PROTOCOL_TRACK_HEAD);
    m_pMmlReport->AddString(STR_NEWLINE);

    //将pFrame强制转换成SNetFrame*类型
    SNetFrame* pMsg = (SNetFrame*) pFrame;
    if (MT_NETWORK == pMsg->ReceiverMid)
    //表示是由主机发来的消息
	{
        //发送方
        sprintf(szBuffer, S_MODULE_I, GetSenderMid(pMsg));
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_SENDER, szBuffer);

        //接收方
        sprintf(szBuffer, S_BAU, CMsgProcessor::g_szPtDes);
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_RECEIVER, szBuffer);

		//序列号统一填为0
        m_pMmlReport->MakeReportItem2(S_TRACK_SEQ_NUM, "0");

		//告警应答消息
		if (IsAlarmResponse(pMsg))
		{
			//消息类型
			m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
										 S_CMD_ALARM_RESPONSE);
			GetAlarmResponseContent(szBuffer, pMsg);
			m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, szBuffer);
		}
		else	//滑动窗口消息
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
	else	//表示由BAU发往主机的消息
	{
        //发送方
        sprintf(szBuffer, S_BAU, CMsgProcessor::g_szPtDes);
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_SENDER, szBuffer);

        //接收方
        sprintf(szBuffer, S_MODULE_I, GetReceiverMid(pMsg));
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_RECEIVER, szBuffer);

		//序列号统一填为0
        m_pMmlReport->MakeReportItem2(S_TRACK_SEQ_NUM, "0");

		//告警消息
		if (IsBinAlarm(pMsg))
		{
			m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, S_CMD_BIN_ALARM);
			GetAlarmContent(szBuffer, pMsg);
			m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, szBuffer);
		}
		else	//滑动窗口消息
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


//构造函数
CAppFrmTrack::CAppFrmTrack()
{
}

//析构函数
CAppFrmTrack::~CAppFrmTrack()
{
}

/**********************************************************************
功能描述    从指定的消息数据中提取出命令码以及所带的数据
输入参数    pMsg——待分析的消息帧
输出参数    nCmd——命令码
			pData——所带的数据
			nLen——所带数据的长度
返回值      无
抛出异常    无
**********************************************************************/
void CAppFrmTrack::ExplainFrame(const SNetFrame* pMsg, 
								int& nCmd, BYTE*& pData, int& nLen)
{
	SAppFrame* pAppFrame = NULL;

    if (MT_NETWORK == pMsg->ReceiverMid)
    //表示是由主机发来的消息，pFrame所带的数据为SAppFrame结构
	{
		pAppFrame = (SAppFrame* )(pMsg->Data);
	}
	else	//表示是发往主机的消息，pFrame所带的数据为SMemFrame结构
	{
		SMemFrame* pMemFrame = (SMemFrame* )(pMsg->Data);
		pAppFrame = (SAppFrame* )(pMemFrame->Data());
	}

	nCmd = pAppFrame->byCmd();
	pData = pAppFrame->Data();
	nLen = pAppFrame->nLen() - _APPFRAME_EXPAND_LENGTH;
}

/**********************************************************************
功能描述    从指定的消息数据中提取出发送模块号
输入参数    pMsg——待分析的消息帧
输出参数    无
返回值      发送模块号
抛出异常    无
**********************************************************************/
UINT4 CAppFrmTrack::GetSenderMid(const SNetFrame* pMsg)
{
    ASSERT(MT_NETWORK == pMsg->ReceiverMid);

	SAppFrame* pAppFrame = (SAppFrame* )(pMsg->Data);

	return pAppFrame->sender_mid();
}

/**********************************************************************
功能描述    从指定的消息数据中提取出接收模块号
输入参数    pMsg——待分析的消息帧
输出参数    无
返回值      接收模块号
抛出异常    无
**********************************************************************/
UINT4 CAppFrmTrack::GetReceiverMid(const SNetFrame* pMsg)
{
    ASSERT(MT_NETWORK != pMsg->ReceiverMid);

	SMemFrame* pMemFrame = (SMemFrame* )(pMsg->Data);
	SAppFrame* pAppFrame = (SAppFrame* )(pMemFrame->Data());

	return pAppFrame->receiver_mid();
}

/**********************************************************************
功能描述    判断指定的消息是否为主机告警应答
输入参数    pMsg——待分析的消息帧
输出参数    无
返回值      TRUE为告警应答，FALSE非告警应答
抛出异常    无
**********************************************************************/
BOOL CAppFrmTrack::IsAlarmResponse(const SNetFrame* pMsg)
{
    ASSERT(MT_NETWORK == pMsg->ReceiverMid);

	SAppFrame* pAppFrame = (SAppFrame* )(pMsg->Data);

	return ((APP_PID_ALARM == pAppFrame->sender_pid()) 
			|| (APP_PID_MEMMAIL == pAppFrame->sender_pid()));
}

/**********************************************************************
功能描述    判断指定的消息是否为二进制告警消息
输入参数    pMsg——待分析的消息帧
输出参数    无
返回值      TRUE为二进制告警，FALSE非二进制告警
抛出异常    无
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
功能描述    根据指定的主机告警应答消息获取相关的描述信息
输入参数    szBuffer——用于存放描述信息的缓冲区，在函数外申请和释放
			pMsg——待分析的主机告警应答消息
输出参数    szBuffer——存放了描述信息的缓冲区
返回值      无
抛出异常    无
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
	//有告警参数，只能为1个参数
	{
		ASSERT(pRspFrame->nLen() == _MEM_ALARM_INFO_EXPAND_LENGTH + 1);

		byPara = pRspFrame->Paras()[0];
	}
	else	//没有告警参数，以0xFF表示无效告警参数
	{
		byPara = 0xFF;
	}

	sprintf(szBuffer, S_CONTENT_ALARM_RESPONSE, uAlarmId, byPara);
}

/**********************************************************************
功能描述    根据指定的告警消息获取相关的描述信息
输入参数    szBuffer——用于存放描述信息的缓冲区，在函数外申请和释放
			pMsg——待分析的告警消息
输出参数    szBuffer——存放了描述信息的缓冲区
返回值      无
抛出异常    无
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
	//有告警参数，只能为1个参数
	{
		ASSERT(pAlarmFrame->nLen() == _MEM_ALARM_INFO_EXPAND_LENGTH + 1);

		byPara = pAlarmFrame->Paras()[0];
	}
	else	//没有告警参数，以0xFF表示无效告警参数
	{
		byPara = 0xFF;
	}

	sprintf(szBuffer, S_CONTENT_BIN_ALARM, uAlarmId, byPara);
}


//构造函数
CSysFrmTrack::CSysFrmTrack()
{
}

//析构函数
CSysFrmTrack::~CSysFrmTrack()
{
}

/**********************************************************************
功能描述    从指定的消息数据中提取出命令码以及所带的数据
输入参数    pMsg——待分析的消息帧
输出参数    nCmd——命令码
			pData——所带的数据
			nLen——所带数据的长度
返回值      无
抛出异常    无
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
功能描述    从指定的消息数据中提取出发送模块号
输入参数    pMsg——待分析的消息帧
输出参数    无
返回值      发送模块号
抛出异常    无
**********************************************************************/
UINT4 CSysFrmTrack::GetSenderMid(const SNetFrame* pMsg)
{
    ASSERT(MT_NETWORK == pMsg->ReceiverMid);

	SSysFrame* pSysFrm = (SSysFrame* )(pMsg->Data);

	return pSysFrm->sender_mid();
}

/**********************************************************************
功能描述    从指定的消息数据中提取出接收模块号
输入参数    pMsg——待分析的消息帧
输出参数    无
返回值      接收模块号
抛出异常    无
**********************************************************************/
UINT4 CSysFrmTrack::GetReceiverMid(const SNetFrame* pMsg)
{
    ASSERT(MT_NETWORK != pMsg->ReceiverMid);

	SSysFrame* pSysFrm = (SSysFrame* )(pMsg->Data);

	return pSysFrm->receiver_mid();
}

/**********************************************************************
功能描述    判断指定的消息是否为主机告警应答
输入参数    pMsg——待分析的消息帧
输出参数    无
返回值      TRUE为告警应答，FALSE非告警应答
抛出异常    无
**********************************************************************/
BOOL CSysFrmTrack::IsAlarmResponse(const SNetFrame* pMsg)
{
    ASSERT(MT_NETWORK == pMsg->ReceiverMid);

	return (SYS_MID_BAM == GetSenderMid(pMsg));
}

/**********************************************************************
功能描述    判断指定的消息是否为二进制告警消息
输入参数    pMsg——待分析的消息帧
输出参数    无
返回值      TRUE为二进制告警，FALSE非二进制告警
抛出异常    无
**********************************************************************/
BOOL CSysFrmTrack::IsBinAlarm(const SNetFrame* pMsg)
{
    ASSERT(MT_NETWORK != pMsg->ReceiverMid);

	return (SYS_MID_BAM == GetReceiverMid(pMsg));
}

/**********************************************************************
功能描述    根据指定的主机告警应答消息获取相关的描述信息
输入参数    szBuffer——用于存放描述信息的缓冲区，在函数外申请和释放
			pMsg——待分析的主机告警应答消息
输出参数    szBuffer——存放了描述信息的缓冲区
返回值      无
抛出异常    无
**********************************************************************/
void CSysFrmTrack::GetAlarmResponseContent(char* szBuffer, 
										   const SNetFrame* pMsg)
{
	ASSERT(IsAlarmResponse(pMsg));
    //张玉新 主机发过来的只有Sys帧头，没有告警体
	SSysFrame* pSysFrm = (SSysFrame* )(pMsg->Data);
	
	sprintf(szBuffer, S_CONTENT_ALARM_RESPONSE,pSysFrm->ucHandle());
}

/**********************************************************************
功能描述    根据指定的告警消息获取相关的描述信息
输入参数    szBuffer——用于存放描述信息的缓冲区，在函数外申请和释放
			pMsg——待分析的告警消息
输出参数    szBuffer——存放了描述信息的缓冲区
返回值      无
抛出异常    无
**********************************************************************/
void CSysFrmTrack::GetAlarmContent(char* szBuffer, const SNetFrame* pMsg)
{
	ASSERT(IsBinAlarm(pMsg));

	UINT4 uAlarmId;
	BYTE byPara;
	SSysFrame* pSysFrm = (SSysFrame* )(pMsg->Data);
	SSysAlarmInfo* pAlarmInfo = (SSysAlarmInfo* )(pSysFrm->Data());

	uAlarmId = pAlarmInfo->ulAlarmID();
	if (pAlarmInfo->uwParaLength() > 1)	//有告警参数
	{
	    //虽然发往主机的告警有两个参数，显示一个就够了
		//另外一个是BAU主备信息
		byPara = pAlarmInfo->ucPara[0];
	}
	else	//没有告警参数，以0xFF表示无效告警参数
	{
		byPara = 0xFF;
	}

	sprintf(szBuffer, S_CONTENT_BIN_ALARM, uAlarmId, byPara, pSysFrm->ucHandle());
}
