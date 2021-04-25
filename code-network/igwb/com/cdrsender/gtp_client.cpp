     /**************************************************************************
 *��Ʒ��    ��iGateway Bill V200        
 *ģ����    ��GTP'�Ʒѽӿ�
 *�ļ���    : gtp_client.cpp
 *������Ϣ  �����ļ�����CGtpClient��ʵ��
 *�汾˵��  ��V200R002B02   
 *��Ȩ��Ϣ  ����Ȩ���У�C��2004-2005 ��Ϊ�������޹�˾
 *����      ��Z30971
 *����ʱ��  ��2004-06-19
 *�޸ļ�¼  ��
 **************************************************************************/
#include "../frame/msg_proc.h"
#include "gtp_client.h"
#include "cdrsnd_svr.h"

//////////////////////////////////////////////////////////////////////////
/**************************************************************************
 *����ԭ��  : CGtpClient(CCDRBuffer* pBuffer, CSenderStatusFile* pStatusFile)
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
CGtpClient::CGtpClient(CCDRBuffer* pBuffer, CSenderStatusFile* pStatusFile)
{
    m_fpSendStatus = pStatusFile;
    m_pCdrBuffer = pBuffer;
    m_fpGtpStatus = NULL;
    m_nEchoReqInvCounter = 0;
}
/**************************************************************************
 *����ԭ��  : CGtpClient::~CGtpClient()
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
CGtpClient::~CGtpClient()
{
    Clear();
    if (m_fpGtpStatus!= NULL)
    {
        fseek(m_fpGtpStatus, 0, SEEK_SET);
        fwrite(&m_GtpNodeInfo, 
              GTP_NODE_INFO_LENGTH,
              1,
              m_fpGtpStatus
              );
        fclose(m_fpGtpStatus);
        m_fpGtpStatus = NULL;
    }
}
/**************************************************************************
 *����ԭ��  : BOOL CGtpClient::GetGtpInfo()
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
BOOL CGtpClient::GetGtpInfo()
{
    //���GTP��Ϣ
    CINIFile IniFile(GetCfgFilePath());
    if (IniFile.Open() != TRUE)
    {
        TRACE(MTS_CDRSENDER,S_PROCESS_OPEN_INIFAIL);
        return ERR_FAIL;
    }
    
    //��ȡ״̬�ļ�·��
    char szFileName[MAX_PATH];
	IniFile.GetString(CFG_SEC_BS, 
                      CFG_BS_KEY_GTP_STATUS_DIR,
                      CFG_MAIN_STATUS_ROOT_DIR_DEFAULT,
                      szFileName,
                      MAX_PATH
                      );
	
    STRNCAT(szFileName, "/bs", MAX_PATH);
    CreateDeepDir(szFileName);

    STRNCAT(szFileName, "/gtp.sf", MAX_PATH);

    m_fpGtpStatus = fopen(szFileName, "rb+");
    if (m_fpGtpStatus == NULL)
    {
        m_fpGtpStatus = fopen(szFileName, "wb+");
    }

    if (m_fpGtpStatus == NULL) 
    {
        return FALSE;
    }

    if (fread(&m_GtpNodeInfo, GTP_NODE_INFO_LENGTH, 1, m_fpGtpStatus) != 1)
    {
        memset(&m_GtpNodeInfo, 0, GTP_NODE_INFO_LENGTH);
    }

    
    //1���������ļ��ж�ȡGTP' Ӧ������, ȱʡΪUDP
    int nGtpMode = IniFile.GetInt(CFG_SEC_BS, 
                                  CFG_BS_KEY_GTP_MODE,
                                  CFG_BS_KEY_GTP_MODE_DEFAULT
                                  );//1 ��ʾUDP
    
    UINT4 nAppType = APP_BS_GTP_UDP;
    if (nGtpMode != CFG_BS_KEY_GTP_MODE_DEFAULT)
    {
        nAppType = APP_BS_GTP_TCP;
    }
    m_GtpNodeInfo.uAppType = nAppType;

        
    //2���������ļ��ж�ȡGTP' ����IP�Ͷ˿ںţ�ȱʡֵΪ0.0.0.0��3386
    char szLocalIp[20];
    IniFile.GetString(CFG_SEC_BS, 
                      CFG_BS_KEY_GTP_CLI_IP, 
                      CFG_BS_KEY_GTP_CLI_IP_DEFAULT,
                      szLocalIp, 
                      sizeof(szLocalIp));

    int nLocalPort = IniFile.GetInt(CFG_SEC_BS, 
                                    CFG_BS_KEY_GTP_CLI_PORT,
                                    CFG_BS_KEY_GTP_CLI_PORT_DEFAULT);

    m_GtpNodeInfo.uLocalIp = ACE_OS::inet_addr(szLocalIp);
    m_GtpNodeInfo.uLocalPort = nLocalPort;

    //3���������ļ��л�ȡGTP'�İ汾��
    int nGtpVersion = IniFile.GetInt(CFG_SEC_BS,
                                     CFG_BS_KEY_GTP_VERSION,
                                     CFG_BS_KEY_GTP_VERSION_DEFAULT
                                     );
    int nHeadLen = IniFile.GetInt(CFG_SEC_BS,
                                  CFG_BS_KEY_GTP_HEADLEN,
                                  CFG_BS_KEY_GTP_HEADLEN_DEFAULT
                                 );

    switch (nGtpVersion) 
    {
    case 0:
        m_GtpNodeInfo.uGtpVersion = GTP_V06_TAG1;
        if (nHeadLen == GTP_FRAME_HEAD_LENGTH_V1)
        {
            m_GtpNodeInfo.uGtpVersion = GTP_V0_TAG1;
        }
        break;
    case 1:
        m_GtpNodeInfo.uGtpVersion = GTP_V16_TAG1;
        if (nHeadLen == GTP_FRAME_HEAD_LENGTH_V1)
        {
            m_GtpNodeInfo.uGtpVersion = GTP_V1_TAG1;
        }
    	break;
    case 2:
    default:
        m_GtpNodeInfo.uGtpVersion = GTP_V2_TAG1;
        break;
    }

    m_nGtpHeadLen = SGtpFrame::GetHeadLen(m_GtpNodeInfo.uGtpVersion);
   

    //4���������ļ��л�ȡGTP'��������IP�Ͷ˿ں�
    char szSvrIp[20];
    IniFile.GetString(CFG_SEC_BS, 
                      CFG_BS_KEY_GTP_SVR_IP, 
                      CFG_BS_KEY_GTP_SVR_IP_DEFAULT,
                      szSvrIp, 
                      sizeof(szSvrIp)
                      );

    if (strcmp(szSvrIp, CFG_BS_KEY_GTP_SVR_IP_DEFAULT) == 0)
    {
        return FALSE;
    }
    
    //�������ļ��ж�ȡGTP Svr�Ķ˿ںţ�ȱʡֵΪ3386
    int nGTPSvrPort = IniFile.GetInt(CFG_SEC_BS, 
                                     CFG_BS_KEY_GTP_SVR_PORT,
                                     CFG_BS_KEY_GTP_SVR_PORT_DEFAULT
                                     );
    m_GtpNodeInfo.nPeerActivedIP = ACE_OS::inet_addr(szSvrIp);
    m_GtpNodeInfo.uPeerPort = nGTPSvrPort;

    
    m_GtpNodeInfo.uLocaRestartCount++;
    m_nSeqNumber = m_GtpNodeInfo.uSeqNum;

    
    SendNodeAliveRequest(m_GtpNodeInfo.uLocalIp);

    return TRUE;
}
/**************************************************************************
 *����ԭ��  : CGtpClient::~CGtpClient()
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
void CGtpClient::Timer1Sec()
{
    /* ��ʱ����ECHO REQUEST ��Ϣ*/
    if (m_GtpNodeInfo.uAppType == APP_BS_GTP_UDP)
    {
        if(++m_nEchoReqInvCounter >= ECHO_REQ_INV)
        {
            SendEchoRequest();
            m_nEchoReqInvCounter = 0;
        }
    }

    /* �ط�δ���ͳ�ȥ��NODE ALIVE REQEST��Ϣ*/
    if (m_NodeAliveReq.pFrame != NULL)
    {
        if (m_NodeAliveReq.yTimeOut >= REQ_TIMEOUT)
        {
            m_NodeAliveReq.yTimeOut = 0;
            m_NodeAliveReq.yRepeatTimes++;
            SendFrame(m_NodeAliveReq.pFrame);

            if (m_NodeAliveReq.yRepeatTimes > TIMEOUT_RPT)
            {
                delete m_NodeAliveReq.pFrame;
                m_NodeAliveReq.pFrame = NULL;
                m_NodeAliveReq.yRepeatTimes = 0;
                m_NodeAliveReq.yTimeOut = 0;
            }
        }
    }
    
    /* ������ݷ��Ͷ������Ƿ����δӦ������� */
    /* 1��ɾ��ͷ���Ѿ������ĵõ�Ӧ���GTP����֡ */
    /* 2��д״̬����*/
    /* 3���ӻ������л�û���,�������β��*/
    /* 4������δӦ��Ļ����� */

    //TDataFrameReqInfo* pDataFrameInfo;
    TDataFrameReqInfo* pLastDataFrame;
    TDataFrameReqInfo* pCurDataFrame;
    TBillPacket*       pPacket;
    TStatusRecord      StatusRecord;
    UINT4  nOffset;
    BOOL   bExistAck = FALSE;

    if (!m_QueueDataReq.empty())
    {
        pCurDataFrame = m_QueueDataReq.front();
    }
    else
    {
        pCurDataFrame = NULL;
    }

    while ((pCurDataFrame!= NULL) && (pCurDataFrame->m_bRecvAck))
    {
        m_QueueDataReq.pop_front();
        pLastDataFrame = pCurDataFrame;
              
        if (!m_QueueDataReq.empty())
        {
             pCurDataFrame = m_QueueDataReq.front();
             if (!pCurDataFrame->m_bRecvAck)
             {
                 pCurDataFrame = NULL;
             }
        }
        else
        {
            pCurDataFrame = NULL;
        }

        if ((pCurDataFrame != NULL )
           && (pLastDataFrame->m_pBillPacket->m_nTaskID
               == pCurDataFrame->m_pBillPacket->m_nTaskID
               ))
        {
            /* 1��ɾ��ͷ���Ѿ������ĵõ�Ӧ���GTP����֡ */
            m_GtpNodeInfo.uSeqNum = pLastDataFrame->m_nSeqNumber;
            
            TRACE(MTS_CDRSENDER, 
                  S_BS_SEND_SUCC,
                  pLastDataFrame->m_pBillPacket->m_nTaskID,
                  pLastDataFrame->m_pBillPacket->m_szFileName,
                  pLastDataFrame->m_pBillPacket->m_nCurReadPos
                  );
            
            delete pLastDataFrame->m_pBillPacket;
            delete pLastDataFrame;

            pLastDataFrame = NULL;
         }
        else
        {
            //����״̬��¼
            StatusRecord.m_nTaskID = pLastDataFrame->m_pBillPacket->m_nTaskID;
            STRNCPY(StatusRecord.m_szFileName,
                pLastDataFrame->m_pBillPacket->m_szFileName,
                MAX_PATH
                );
            StatusRecord.m_tCreTime = pLastDataFrame->m_pBillPacket->m_tCreTime;
            StatusRecord.m_nCurPos = pLastDataFrame->m_pBillPacket->m_nCurReadPos;
            
            /* 2��д״̬����*/
            nOffset = m_fpSendStatus->GetOffset(StatusRecord.m_nTaskID);
            m_fpSendStatus->SetOffset(nOffset);
            m_fpSendStatus->Write(&StatusRecord);

            m_GtpNodeInfo.uSeqNum = pLastDataFrame->m_nSeqNumber; 
            if (m_fpGtpStatus != NULL)
            {
                fseek(m_fpGtpStatus, 0, SEEK_SET);
                int nWriteLen = fwrite(&m_GtpNodeInfo, 
                                        GTP_NODE_INFO_LENGTH,
                                        1,
                                        m_fpGtpStatus
                                       );
                int nRet = fflush(m_fpGtpStatus);
                if (nWriteLen!= 1 || nRet != 0)
                {
                    TRACE(MTS_CDRSENDER, S_BS_WRITE_STATUS_ERR);
                }
            }
            
                
            delete pLastDataFrame->m_pBillPacket;
            delete pLastDataFrame;
            pLastDataFrame = NULL;
          
        }
    }
    
    /* 3���ӻ������л�û���,�������β�� */
    int nQueueLen = m_QueueDataReq.size();
    while (nQueueLen < QUEUE_SENDER_LEN)
    {
        pPacket = (TBillPacket *)m_pCdrBuffer->GetPacket();
        if (pPacket == NULL)
        {
            break;
        }
        
        //����GTP'Э��֡
        SGtpFrame *pGTPFrame;
        pGTPFrame = CreateGTPFrame(pPacket->m_nLength 
            + TIETRANSFER_CMD_LENGTH
            + TIEBILL_BACKAGE_HEAD_LENGTH
            );
        
        if (pGTPFrame != NULL)
        {
            pGTPFrame->yType(DATA_TRANSFER_REQUEST);
            pGTPFrame->uSeqNum(++m_nSeqNumber);
            
            TIETransferCommand* pIECmd = (TIETransferCommand*)pGTPFrame->Data();
            pIECmd->yType(TIETransferCommand::type);
            pIECmd->yCmd(TC_SENDDATA);
            
            TIEBillPackage* pIEPacket = (TIEBillPackage*)(pGTPFrame->Data() + TIETRANSFER_CMD_LENGTH);
            pIEPacket->yType(TIEBillPackage::type);
            pIEPacket->uLen(pPacket->m_nLength + 4);

            pIEPacket->yCount(pPacket->m_nCdrCount);
            memcpy(pIEPacket->Data(), pPacket->m_pData, pPacket->m_nLength);
        }
        
        //���ȴ�Ӧ��֡���뵽m_QueueDataReq������
        pCurDataFrame = new TDataFrameReqInfo;
        pCurDataFrame->m_pFrame = pGTPFrame;
        pCurDataFrame->m_bRecvAck = FALSE;
        pCurDataFrame->m_nSeqNumber = pGTPFrame->uSeqNum();
        pCurDataFrame->m_pBillPacket = pPacket;

        m_QueueDataReq.push_back(pCurDataFrame);
        
        nQueueLen++;
        
        //���������������Ѿ����ã�ɾ��       
        delete[] pPacket->m_pData;
        pPacket->m_pData = NULL;
    }

     /* 4������δӦ��Ļ����� */
    QUEUE_DATA_GTPREQ::iterator it = m_QueueDataReq.begin(); 
    while (it != m_QueueDataReq.end())
    {
        pCurDataFrame = *it;
        
        if (!pCurDataFrame->m_bRecvAck)
        {
            SendDataTransferRequest(pCurDataFrame->m_pFrame);
        }

        it++;
    }
}
/**************************************************************************
 *����ԭ��  : SGtpFrame* CGtpClient::CreateGTPFrame(UINT2 uLen)
 *�������  : 
 *�������  : 
 *��������  : ����GTP'Э��֡
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
SGtpFrame* CGtpClient::CreateGTPFrame(UINT2 uLen)
{
    int nSize = m_nGtpHeadLen + uLen;
    SGtpFrame *pFrame = (SGtpFrame *)new BYTE[nSize];

    if (pFrame)
    {
        memset(pFrame, 0, nSize);
        pFrame->yTag(m_GtpNodeInfo.uGtpVersion);
        pFrame->FillV0HeadEx();
        pFrame->uLen(uLen);
    }
    return pFrame;
}
/**************************************************************************
 *����ԭ��  : SGtpFrame* CGtpClient::CopyGTPFrame(SGtpFrame *pFrame)
 *�������  : 
 *�������  : 
 *��������  : ����һ��GTP֡
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
SGtpFrame* CGtpClient::CopyGTPFrame(SGtpFrame *pFrame)
{
    SGtpFrame *pResult;
    pResult = (SGtpFrame *)new BYTE[pFrame->GetSize()];
    if (pResult != NULL)
    {
        memcpy(pResult, pFrame, pFrame->GetSize());
    }

    return pResult;
}
/**************************************************************************
 *����ԭ��  : int CGtpClient::SendFrame(SGtpFrame* pFrame)
 *�������  : 
 *�������  : 
 *��������  : ��GTP��������������֡
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
int CGtpClient::SendFrame(SGtpFrame* pFrame)
{
    //����һ��SNetFrame�ṹ��ʵ��*pMsg
    MSG_INTRA* pMsg = new(pFrame->GetSize()) MSG_INTRA;
    if (!pMsg) return ERR_FAIL;

    //��*pFrame�е����ݿ�����*pMsg����������
    memcpy(pMsg->Data, pFrame, pMsg->Len);
    pMsg->ReceiverPid = PT_OUTSIDE;
    pMsg->ReceiverMid = MT_UNKNOWN;
    pMsg->SenderPid   = CMsgProcessor::GetProcessType();
    pMsg->SenderMid   = MT_CDRSENDER;
    pMsg->AppType     = m_GtpNodeInfo.uAppType;
    ((SNetFrame*)pMsg)->SNetAddrPair.ClientIp   = m_GtpNodeInfo.uLocalIp;
    ((SNetFrame*)pMsg)->SNetAddrPair.ClientPort = m_GtpNodeInfo.uLocalPort;
    ((SNetFrame*)pMsg)->SNetAddrPair.ServerIp   = m_GtpNodeInfo.nPeerActivedIP;
    ((SNetFrame*)pMsg)->SNetAddrPair.ServerPort = m_GtpNodeInfo.uPeerPort;

    return m_pCDRSndSvr->SendMsgtoBS(pMsg);
}
/**************************************************************************
 *����ԭ��  : void CGtpClient::OnReceive(void* pFrame)
 *�������  : 
 *�������  : 
 *��������  : �����GTP'���������յ�������
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
void CGtpClient::OnReceive(void* pFrame)
{
    SNetFrame* pNetFrame = (SNetFrame *)pFrame;
    SGtpFrame* pGTPFrame = (SGtpFrame *)(pNetFrame->Data);

    //�����Ϣ�����������Ȳ���һ��GTP'��֡ͷ���ȣ����˳�
    if(pNetFrame->Len < GTP_FRAME_HEAD_LENGTH_V2)
    {
        return;
    }

    //Ŀǰ��������ַ���ǵ�ǰ��ĵ�ַ,�򷵻�
    //�Ժ�������Ӳ������õķ��������б���,�򷵻�
    if (m_GtpNodeInfo.nPeerActivedIP != pNetFrame->SNetAddrPair.ServerIp)
    {
        return;
    }

    //�жϰ汾�Ƿ�֧��
    if (!pGTPFrame->CheckGTPFrame())
    {
        SendVersionNotSupported(pGTPFrame);
        return;
    }

    switch (pGTPFrame->yType())
    {
    //��������,ֱ��Ӧ�����������������
    case ECHO_REQUEST:
        OnEchoRequest(pGTPFrame);
        break;

    //����Ӧ�𡣴�����ȡ�������������Ӧ����
    case ECHO_RESPONSE:
        OnEchoResponse(pGTPFrame);
        break;

    //���ݴ���Ӧ��
    case DATA_TRANSFER_RESPONSE:
        OnDataTransferResponse(pGTPFrame);
        break;

    //�ڵ�����Ӧ��
    case NODE_ALIVE_RESPONSE:
        OnNodeAliveResponse(pGTPFrame);
        break;

    //�ڵ�����
    case NODE_ALIVE_REQUEST:
        OnNodeAliveRequest(pGTPFrame);
        break;

    //�ض�������
    case REDIRECTION_REQUEST:
        OnRedirectRequest(pGTPFrame);
        break;

    //�汾��֧��
    case VERSION_NOT_SUPPORTED:
        break;

    default:
        MSGOUT(MTS_CDRSENDER, MSGOUT_LEVEL_IMPORTANT,
               S_NET_UNRECOGNIZED_FRAME,
               pGTPFrame->yType());
    }
}
/**************************************************************************
 *����ԭ��  : void CGtpClient::SendVersionNotSupported(SGtpFrame *pFrame)
 *�������  : 
 *�������  : 
 *��������  : ���Ͱ汾��֧��: �յ���֧�ֵ���Ϣ����
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/    
void CGtpClient::SendVersionNotSupported(SGtpFrame *pFrame)
{
    SGtpFrame* pGtpFrame = CreateGTPFrame(0);
    ASSERT(NULL != pGtpFrame);

    //���GTPͷ
    pGtpFrame->yType(VERSION_NOT_SUPPORTED);
    pGtpFrame->uSeqNum(pFrame->uSeqNum());

    SendFrame(pGtpFrame);
    
    delete pGtpFrame;
}
/**************************************************************************
 *����ԭ��  : void CGtpClient::SendEchoRequest();
 *�������  : 
 *�������  : 
 *��������  : ����Echo Request
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/  
void CGtpClient::SendEchoRequest()
{
    SGtpFrame *pFrame = CreateGTPFrame(0);
    if (pFrame != NULL)
    {
        //���GTPͷ
        pFrame->yType(ECHO_REQUEST);
        pFrame->uSeqNum(0);
        
        SendFrame(pFrame);
    }
}
/**************************************************************************
 *����ԭ��  : void CGtpClient::SendEchoResponse(SGtpFrame *pFrame)
 *�������  : 
 *�������  : 
 *��������  : ����Response
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/  
void CGtpClient::SendEchoResponse(SGtpFrame *pFrame)
{
    SGtpFrame *pRespFrame;
    TIERecovery *pRecovery;

    pRespFrame = CreateGTPFrame(TIERECOVERY_LENGTH);
    if (pRespFrame != NULL)
    {
        //���GTPͷ
        pRespFrame->yType(ECHO_RESPONSE);
        pRespFrame->uSeqNum(pFrame->uSeqNum());

        //���IE�ṹ
        pRecovery = (TIERecovery *)pRespFrame->Data();
        pRecovery->yType(TIERecovery::type);
        pRecovery->yRestartCount(m_GtpNodeInfo.uLocaRestartCount-1);
        
        SendFrame(pRespFrame);
        
        delete pRespFrame;
    }
}
/**************************************************************************
 *����ԭ��  : void CGtpClient::SendGTPRequest(SGtpFrame *pFrame)
 *�������  : 
 *�������  : 
 *��������  : ����GTP����֡������֡��,�������δӦ�����
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/  
void CGtpClient::SendGTPRequest(SGtpFrame *pFrame)
{
    SendFrame(pFrame);
}
/**************************************************************************
 *����ԭ��  : void CGtpClient::SendGTPResponse(SGtpFrame *pFrame)
 *�������  : 
 *�������  : 
 *��������  : ����GTPӦ��֡
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/      
void CGtpClient::SendGTPResponse(SGtpFrame *pFrame)
{
    SendFrame(pFrame);
}
/**************************************************************************
 *����ԭ��  : void CGtpClient::SendNodeAliveRequest(UINT4 uLocalAddr)
 *�������  : 
 *�������  : 
 *��������  : ����Node Alive Request/Response
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/       
void CGtpClient::SendNodeAliveRequest(UINT4 uLocalAddr)
{
    SGtpFrame *pFrame;
    TIENodeAddress *pAddress;

    pFrame = CreateGTPFrame(TIENODE_ADDRESS_LENGTH);
    if (pFrame != NULL)
    {
        //���GTPͷ
        pFrame->yType(NODE_ALIVE_REQUEST);
        pFrame->uSeqNum(0);

        //���IE�ṹ
        pAddress = (TIENodeAddress *)pFrame->Data();
        pAddress->yType(TIENodeAddress::type);
        pAddress->uLen(4);
        pAddress->uAddress(uLocalAddr);
        
        m_NodeAliveReq.pFrame = pFrame;
        m_NodeAliveReq.yRepeatTimes = 0;
        m_NodeAliveReq.yTimeOut = 0;
        
        SendFrame(pFrame);
    }
}
/**************************************************************************
 *����ԭ��  : void CGtpClient::SendNodeAliveResponse(SGtpFrame *pFrame)
 *�������  : 
 *�������  : 
 *��������  : ����Node Alive Request/Response
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/   
void CGtpClient::SendNodeAliveResponse(SGtpFrame *pFrame)
{
    //����Ӧ��
    SGtpFrame *pRespFrame;
    pRespFrame = CreateGTPFrame(0);
    if (pRespFrame != NULL)
    {
        //���GTPͷ
        pRespFrame->yType(NODE_ALIVE_RESPONSE);
        pRespFrame->uSeqNum(pFrame->uSeqNum());

        SendFrame(pRespFrame);
        delete pRespFrame;
    }
}
/**************************************************************************
 *����ԭ��  : void CGtpClient::OnNodeAliveRequest(SGtpFrame *pFrame)
 *�������  : 
 *�������  : 
 *��������  : ��ӦNode ALIVE REQUEST/RESPONSE֡
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/   
void CGtpClient::OnNodeAliveRequest(SGtpFrame *pFrame)
{
    //���IP��ַ��ʽΪipv6���򷵻�Versoin Not Supported��Ϣ
    TIENodeAddress* pIpAddress = (TIENodeAddress* )(pFrame->Data());
    if (pFrame->uLen() < TIENODE_ADDRESS_LENGTH || pIpAddress->uLen() != 4)
    {
        SendVersionNotSupported(pFrame);
        return;
    }

    //���ü���
    if (m_NodeAliveReq.pFrame != NULL)
    {
        m_NodeAliveReq.yRepeatTimes = 0;
        m_NodeAliveReq.yTimeOut = 0;
    }

    SendNodeAliveResponse(pFrame);
}
/**************************************************************************
 *����ԭ��  : void CGtpClient::OnNodeAliveResponse(SGtpFrame *pFrame)
 *�������  : 
 *�������  : 
 *��������  : ����Node Alive Request/Response
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/   
void CGtpClient::OnNodeAliveResponse(SGtpFrame *pFrame)
{
    if (m_NodeAliveReq.pFrame != NULL)
    {
        delete m_NodeAliveReq.pFrame;
        m_NodeAliveReq.pFrame = NULL;
        m_NodeAliveReq.yRepeatTimes = 0;
        m_NodeAliveReq.yTimeOut = 0;
    }
}

/**************************************************************************
 *����ԭ��  : void CGtpClient::SendRedirectResponse()
 *�������  : 
 *�������  : 
 *��������  : ����Redirection Response
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/   
void CGtpClient::SendRedirectResponse()
{
    //
}
 /**************************************************************************
 *����ԭ��  : void CGtpClient::SendDataTransferRequest(SGtpFrame *pFrame)
 *�������  : 
 *�������  : 
 *��������  : ����Data TransferR equest
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/      
void CGtpClient::SendDataTransferRequest(SGtpFrame *pFrame)
{
    SendFrame(pFrame);
}
/**************************************************************************
 *����ԭ��  : void CGtpClient::SendNormalData(SGtpFrame *pFrame)
 *�������  : 
 *�������  : 
 *��������  : ����Node Alive Request/Response
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/   
void CGtpClient::SendNormalData(SGtpFrame *pFrame)
{
}
/**************************************************************************
 *����ԭ��  : void CGtpClient::SendTestEmptyFrame(SGtpFrame *pFrame)
 *�������  : 
 *�������  : 
 *��������  : ����Node Alive Request/Response
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/   
void CGtpClient::SendTestEmptyFrame(SGtpFrame *pFrame)
{
}
/**************************************************************************
 *����ԭ��  : void CGtpClient::SendMaybeDupData(SGtpFrame *pFrame)
 *�������  : 
 *�������  : 
 *��������  : ����Node Alive Request/Response
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/   
void CGtpClient::SendMaybeDupData(SGtpFrame *pFrame)
{
}
/**************************************************************************
 *����ԭ��  : void CGtpClient::SendCancelData(SGtpFrame *pFrame)
 *�������  : 
 *�������  : 
 *��������  : ����Node Alive Request/Response
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/   
void CGtpClient::SendCancelData(SGtpFrame *pFrame)
{
}
/**************************************************************************
 *����ԭ��  : void CGtpClient::SendReleaseData(SGtpFrame *pFrame)
 *�������  : 
 *�������  : 
 *��������  : ����Node Alive Request/Response
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/   
void CGtpClient::SendReleaseData(SGtpFrame *pFrame)
{
}

/**************************************************************************
 *����ԭ��  : void CGtpClient::Clear()
 *�������  : ��
 *�������  : ��
 *��������  : ���������Ϣ
 *����      : Z30971
 *�޸ļ�¼  : ��
***************************************************************************/   
void CGtpClient::Clear()
{
    if (m_NodeAliveReq.pFrame != NULL)
    {
        delete m_NodeAliveReq.pFrame;
        m_NodeAliveReq.pFrame = NULL;
    }

    QUEUE_DATA_GTPREQ::iterator it2 = m_QueueDataReq.begin();
    while (it2 != m_QueueDataReq.end())
    {
        TDataFrameReqInfo* pDataFrame = *it2;
        delete pDataFrame->m_pBillPacket;
        pDataFrame->m_pBillPacket = NULL;
        delete pDataFrame->m_pFrame;
        pDataFrame->m_pFrame = NULL;
        delete pDataFrame;
        pDataFrame = NULL;
        it2++;
    }
    m_QueueDataReq.clear();
}
/**************************************************************************
 *����ԭ��  : void CGtpClient::OnEchoResponse(pGTPFrame)
 *�������  : ��
 *�������  : ��
 *��������  : ���������Ϣ
 *����      : Z30971
 *�޸ļ�¼  : ��
***************************************************************************/  
void CGtpClient::OnEchoResponse(SGtpFrame *pFrame)
{
     TIERecovery *pRecovery;
    int nChkRst = CHECK_TV(pFrame, "TIERecovery", 
                           TIERECOVERY_LENGTH,
                           pRecovery, 0);
    if(nChkRst != 0)
    {
        return;
    }

    if (m_GtpNodeInfo.uPeerRestartCount != pRecovery->yRestartCount())
    {
        m_GtpNodeInfo.uPeerRestartCount = pRecovery->yRestartCount();
        if (m_NodeAliveReq.pFrame != NULL)
        {
            m_NodeAliveReq.yRepeatTimes = 0;
            m_NodeAliveReq.yTimeOut = 0;
        }
    }
}
/**************************************************************************
 *����ԭ��  : void CGtpClient::OnDataTransferResponse(SGtpFrame *pFrame)
 *�������  : ��
 *�������  : ��
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : ��
***************************************************************************/  
void CGtpClient::OnDataTransferResponse(SGtpFrame *pFrame)
{
    if (m_QueueDataReq.empty())
    {
        return;
    }

    TIECause* pCause = (TIECause*)pFrame->Data();
    if (pCause->yCause() == CAUSE_REQUEST_ACCEPTED)
    {
        TIESNResponse* pResponse = (TIESNResponse*)(pFrame->Data() + TIECAUSE_LENGTH);
        UINT2 nRecordCount = pResponse->uLen()/2;

        for (int i = 0; i < nRecordCount; i++)
        {
            QUEUE_DATA_GTPREQ::iterator it = m_QueueDataReq.begin();
            while (it != m_QueueDataReq.end())
            {
                if ((*it)->m_nSeqNumber == pResponse->uSeqNums(i))
                {
                    delete (*it)->m_pFrame;
                    (*it)->m_pFrame = NULL;
                    (*it)->m_bRecvAck = TRUE;
                    break;
                }

                it++;
            }//End while
        }//End for
    }//End if
}

void CGtpClient::OnRedirectRequest(SGtpFrame *pFrame)
{
}
void CGtpClient::OnEchoRequest(SGtpFrame *pFrame)
{
    SendEchoResponse(pFrame);
}
   
