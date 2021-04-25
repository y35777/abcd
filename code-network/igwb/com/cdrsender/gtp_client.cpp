     /**************************************************************************
 *产品名    ：iGateway Bill V200        
 *模块名    ：GTP'计费接口
 *文件名    : gtp_client.cpp
 *描述信息  ：本文件是类CGtpClient的实现
 *版本说明  ：V200R002B02   
 *版权信息  ：版权所有（C）2004-2005 华为技术有限公司
 *作者      ：Z30971
 *创建时间  ：2004-06-19
 *修改记录  ：
 **************************************************************************/
#include "../frame/msg_proc.h"
#include "gtp_client.h"
#include "cdrsnd_svr.h"

//////////////////////////////////////////////////////////////////////////
/**************************************************************************
 *函数原型  : CGtpClient(CCDRBuffer* pBuffer, CSenderStatusFile* pStatusFile)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
CGtpClient::CGtpClient(CCDRBuffer* pBuffer, CSenderStatusFile* pStatusFile)
{
    m_fpSendStatus = pStatusFile;
    m_pCdrBuffer = pBuffer;
    m_fpGtpStatus = NULL;
    m_nEchoReqInvCounter = 0;
}
/**************************************************************************
 *函数原型  : CGtpClient::~CGtpClient()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
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
 *函数原型  : BOOL CGtpClient::GetGtpInfo()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
BOOL CGtpClient::GetGtpInfo()
{
    //获得GTP信息
    CINIFile IniFile(GetCfgFilePath());
    if (IniFile.Open() != TRUE)
    {
        TRACE(MTS_CDRSENDER,S_PROCESS_OPEN_INIFAIL);
        return ERR_FAIL;
    }
    
    //读取状态文件路径
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

    
    //1、从配置文件中读取GTP' 应用类型, 缺省为UDP
    int nGtpMode = IniFile.GetInt(CFG_SEC_BS, 
                                  CFG_BS_KEY_GTP_MODE,
                                  CFG_BS_KEY_GTP_MODE_DEFAULT
                                  );//1 表示UDP
    
    UINT4 nAppType = APP_BS_GTP_UDP;
    if (nGtpMode != CFG_BS_KEY_GTP_MODE_DEFAULT)
    {
        nAppType = APP_BS_GTP_TCP;
    }
    m_GtpNodeInfo.uAppType = nAppType;

        
    //2、从配置文件中读取GTP' 本地IP和端口号，缺省值为0.0.0.0和3386
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

    //3、从配置文件中获取GTP'的版本号
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
   

    //4、从配置文件中获取GTP'服务器的IP和端口号
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
    
    //从配置文件中读取GTP Svr的端口号，缺省值为3386
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
 *函数原型  : CGtpClient::~CGtpClient()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
void CGtpClient::Timer1Sec()
{
    /* 定时发送ECHO REQUEST 消息*/
    if (m_GtpNodeInfo.uAppType == APP_BS_GTP_UDP)
    {
        if(++m_nEchoReqInvCounter >= ECHO_REQ_INV)
        {
            SendEchoRequest();
            m_nEchoReqInvCounter = 0;
        }
    }

    /* 重发未发送出去的NODE ALIVE REQEST消息*/
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
    
    /* 检测数据发送队列中是否存在未应答的请求 */
    /* 1、删除头部已经连续的得到应答的GTP数据帧 */
    /* 2、写状态数据*/
    /* 3、从缓存区中获得话单,放入队列尾部*/
    /* 4、发送未应答的话单包 */

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
            /* 1、删除头部已经连续的得到应答的GTP数据帧 */
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
            //构建状态记录
            StatusRecord.m_nTaskID = pLastDataFrame->m_pBillPacket->m_nTaskID;
            STRNCPY(StatusRecord.m_szFileName,
                pLastDataFrame->m_pBillPacket->m_szFileName,
                MAX_PATH
                );
            StatusRecord.m_tCreTime = pLastDataFrame->m_pBillPacket->m_tCreTime;
            StatusRecord.m_nCurPos = pLastDataFrame->m_pBillPacket->m_nCurReadPos;
            
            /* 2、写状态数据*/
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
    
    /* 3、从缓存区中获得话单,放入队列尾部 */
    int nQueueLen = m_QueueDataReq.size();
    while (nQueueLen < QUEUE_SENDER_LEN)
    {
        pPacket = (TBillPacket *)m_pCdrBuffer->GetPacket();
        if (pPacket == NULL)
        {
            break;
        }
        
        //构建GTP'协议帧
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
        
        //将等待应答帧放入到m_QueueDataReq队列中
        pCurDataFrame = new TDataFrameReqInfo;
        pCurDataFrame->m_pFrame = pGTPFrame;
        pCurDataFrame->m_bRecvAck = FALSE;
        pCurDataFrame->m_nSeqNumber = pGTPFrame->uSeqNum();
        pCurDataFrame->m_pBillPacket = pPacket;

        m_QueueDataReq.push_back(pCurDataFrame);
        
        nQueueLen++;
        
        //话单包的数据区已经无用，删除       
        delete[] pPacket->m_pData;
        pPacket->m_pData = NULL;
    }

     /* 4、发送未应答的话单包 */
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
 *函数原型  : SGtpFrame* CGtpClient::CreateGTPFrame(UINT2 uLen)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 创建GTP'协议帧
 *作者      : Z30971
 *修改记录  : 
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
 *函数原型  : SGtpFrame* CGtpClient::CopyGTPFrame(SGtpFrame *pFrame)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 复制一个GTP帧
 *作者      : Z30971
 *修改记录  : 
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
 *函数原型  : int CGtpClient::SendFrame(SGtpFrame* pFrame)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 向GTP服务器发送数据帧
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
int CGtpClient::SendFrame(SGtpFrame* pFrame)
{
    //创建一个SNetFrame结构的实例*pMsg
    MSG_INTRA* pMsg = new(pFrame->GetSize()) MSG_INTRA;
    if (!pMsg) return ERR_FAIL;

    //将*pFrame中的数据拷贝到*pMsg的数据区中
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
 *函数原型  : void CGtpClient::OnReceive(void* pFrame)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 处理从GTP'服务器接收到的数据
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
void CGtpClient::OnReceive(void* pFrame)
{
    SNetFrame* pNetFrame = (SNetFrame *)pFrame;
    SGtpFrame* pGTPFrame = (SGtpFrame *)(pNetFrame->Data);

    //如果消息的数据区长度不足一个GTP'的帧头长度，则退出
    if(pNetFrame->Len < GTP_FRAME_HEAD_LENGTH_V2)
    {
        return;
    }

    //目前做法：地址不是当前活动的地址,则返回
    //以后可以增加不是配置的服务器端列表中,则返回
    if (m_GtpNodeInfo.nPeerActivedIP != pNetFrame->SNetAddrPair.ServerIp)
    {
        return;
    }

    //判断版本是否支持
    if (!pGTPFrame->CheckGTPFrame())
    {
        SendVersionNotSupported(pGTPFrame);
        return;
    }

    switch (pGTPFrame->yType())
    {
    //回响请求,直接应答该命令并返回重起计数
    case ECHO_REQUEST:
        OnEchoRequest(pGTPFrame);
        break;

    //回响应答。从中提取重起计数并做相应处理
    case ECHO_RESPONSE:
        OnEchoResponse(pGTPFrame);
        break;

    //数据传输应答
    case DATA_TRANSFER_RESPONSE:
        OnDataTransferResponse(pGTPFrame);
        break;

    //节点活动命令应答
    case NODE_ALIVE_RESPONSE:
        OnNodeAliveResponse(pGTPFrame);
        break;

    //节点活动请求，
    case NODE_ALIVE_REQUEST:
        OnNodeAliveRequest(pGTPFrame);
        break;

    //重定向请求
    case REDIRECTION_REQUEST:
        OnRedirectRequest(pGTPFrame);
        break;

    //版本不支持
    case VERSION_NOT_SUPPORTED:
        break;

    default:
        MSGOUT(MTS_CDRSENDER, MSGOUT_LEVEL_IMPORTANT,
               S_NET_UNRECOGNIZED_FRAME,
               pGTPFrame->yType());
    }
}
/**************************************************************************
 *函数原型  : void CGtpClient::SendVersionNotSupported(SGtpFrame *pFrame)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 发送版本不支持: 收到不支持的消息类型
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/    
void CGtpClient::SendVersionNotSupported(SGtpFrame *pFrame)
{
    SGtpFrame* pGtpFrame = CreateGTPFrame(0);
    ASSERT(NULL != pGtpFrame);

    //填充GTP头
    pGtpFrame->yType(VERSION_NOT_SUPPORTED);
    pGtpFrame->uSeqNum(pFrame->uSeqNum());

    SendFrame(pGtpFrame);
    
    delete pGtpFrame;
}
/**************************************************************************
 *函数原型  : void CGtpClient::SendEchoRequest();
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 发送Echo Request
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/  
void CGtpClient::SendEchoRequest()
{
    SGtpFrame *pFrame = CreateGTPFrame(0);
    if (pFrame != NULL)
    {
        //填充GTP头
        pFrame->yType(ECHO_REQUEST);
        pFrame->uSeqNum(0);
        
        SendFrame(pFrame);
    }
}
/**************************************************************************
 *函数原型  : void CGtpClient::SendEchoResponse(SGtpFrame *pFrame)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 发送Response
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/  
void CGtpClient::SendEchoResponse(SGtpFrame *pFrame)
{
    SGtpFrame *pRespFrame;
    TIERecovery *pRecovery;

    pRespFrame = CreateGTPFrame(TIERECOVERY_LENGTH);
    if (pRespFrame != NULL)
    {
        //填充GTP头
        pRespFrame->yType(ECHO_RESPONSE);
        pRespFrame->uSeqNum(pFrame->uSeqNum());

        //填充IE结构
        pRecovery = (TIERecovery *)pRespFrame->Data();
        pRecovery->yType(TIERecovery::type);
        pRecovery->yRestartCount(m_GtpNodeInfo.uLocaRestartCount-1);
        
        SendFrame(pRespFrame);
        
        delete pRespFrame;
    }
}
/**************************************************************************
 *函数原型  : void CGtpClient::SendGTPRequest(SGtpFrame *pFrame)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 发送GTP请求帧除数据帧外,将其放入未应答队列
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/  
void CGtpClient::SendGTPRequest(SGtpFrame *pFrame)
{
    SendFrame(pFrame);
}
/**************************************************************************
 *函数原型  : void CGtpClient::SendGTPResponse(SGtpFrame *pFrame)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 发送GTP应答帧
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/      
void CGtpClient::SendGTPResponse(SGtpFrame *pFrame)
{
    SendFrame(pFrame);
}
/**************************************************************************
 *函数原型  : void CGtpClient::SendNodeAliveRequest(UINT4 uLocalAddr)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 发送Node Alive Request/Response
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/       
void CGtpClient::SendNodeAliveRequest(UINT4 uLocalAddr)
{
    SGtpFrame *pFrame;
    TIENodeAddress *pAddress;

    pFrame = CreateGTPFrame(TIENODE_ADDRESS_LENGTH);
    if (pFrame != NULL)
    {
        //填充GTP头
        pFrame->yType(NODE_ALIVE_REQUEST);
        pFrame->uSeqNum(0);

        //填充IE结构
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
 *函数原型  : void CGtpClient::SendNodeAliveResponse(SGtpFrame *pFrame)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 发送Node Alive Request/Response
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/   
void CGtpClient::SendNodeAliveResponse(SGtpFrame *pFrame)
{
    //发送应答
    SGtpFrame *pRespFrame;
    pRespFrame = CreateGTPFrame(0);
    if (pRespFrame != NULL)
    {
        //填充GTP头
        pRespFrame->yType(NODE_ALIVE_RESPONSE);
        pRespFrame->uSeqNum(pFrame->uSeqNum());

        SendFrame(pRespFrame);
        delete pRespFrame;
    }
}
/**************************************************************************
 *函数原型  : void CGtpClient::OnNodeAliveRequest(SGtpFrame *pFrame)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 相应Node ALIVE REQUEST/RESPONSE帧
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/   
void CGtpClient::OnNodeAliveRequest(SGtpFrame *pFrame)
{
    //如果IP地址形式为ipv6，则返回Versoin Not Supported消息
    TIENodeAddress* pIpAddress = (TIENodeAddress* )(pFrame->Data());
    if (pFrame->uLen() < TIENODE_ADDRESS_LENGTH || pIpAddress->uLen() != 4)
    {
        SendVersionNotSupported(pFrame);
        return;
    }

    //重置记数
    if (m_NodeAliveReq.pFrame != NULL)
    {
        m_NodeAliveReq.yRepeatTimes = 0;
        m_NodeAliveReq.yTimeOut = 0;
    }

    SendNodeAliveResponse(pFrame);
}
/**************************************************************************
 *函数原型  : void CGtpClient::OnNodeAliveResponse(SGtpFrame *pFrame)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 发送Node Alive Request/Response
 *作者      : Z30971
 *修改记录  : 
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
 *函数原型  : void CGtpClient::SendRedirectResponse()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 发送Redirection Response
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/   
void CGtpClient::SendRedirectResponse()
{
    //
}
 /**************************************************************************
 *函数原型  : void CGtpClient::SendDataTransferRequest(SGtpFrame *pFrame)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 发送Data TransferR equest
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/      
void CGtpClient::SendDataTransferRequest(SGtpFrame *pFrame)
{
    SendFrame(pFrame);
}
/**************************************************************************
 *函数原型  : void CGtpClient::SendNormalData(SGtpFrame *pFrame)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 发送Node Alive Request/Response
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/   
void CGtpClient::SendNormalData(SGtpFrame *pFrame)
{
}
/**************************************************************************
 *函数原型  : void CGtpClient::SendTestEmptyFrame(SGtpFrame *pFrame)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 发送Node Alive Request/Response
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/   
void CGtpClient::SendTestEmptyFrame(SGtpFrame *pFrame)
{
}
/**************************************************************************
 *函数原型  : void CGtpClient::SendMaybeDupData(SGtpFrame *pFrame)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 发送Node Alive Request/Response
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/   
void CGtpClient::SendMaybeDupData(SGtpFrame *pFrame)
{
}
/**************************************************************************
 *函数原型  : void CGtpClient::SendCancelData(SGtpFrame *pFrame)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 发送Node Alive Request/Response
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/   
void CGtpClient::SendCancelData(SGtpFrame *pFrame)
{
}
/**************************************************************************
 *函数原型  : void CGtpClient::SendReleaseData(SGtpFrame *pFrame)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 发送Node Alive Request/Response
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/   
void CGtpClient::SendReleaseData(SGtpFrame *pFrame)
{
}

/**************************************************************************
 *函数原型  : void CGtpClient::Clear()
 *输入参数  : 无
 *输出参数  : 无
 *功能描述  : 清除所有信息
 *作者      : Z30971
 *修改记录  : 无
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
 *函数原型  : void CGtpClient::OnEchoResponse(pGTPFrame)
 *输入参数  : 无
 *输出参数  : 无
 *功能描述  : 清除所有信息
 *作者      : Z30971
 *修改记录  : 无
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
 *函数原型  : void CGtpClient::OnDataTransferResponse(SGtpFrame *pFrame)
 *输入参数  : 无
 *输出参数  : 无
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 无
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
   
