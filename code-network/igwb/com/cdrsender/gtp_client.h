/**************************************************************************
 *产品名    ：iGateway Bill V200        
 *模块名    ：GTP'计费接口
 *文件名    : gtp_client.h
 *描述信息  ：本文件是类CGtpClient的定义
 *版本说明  ：V200R002B02   
 *版权信息  ：版权所有（C）2004-2005 华为技术有限公司
 *作者      ：Z30971
 *创建时间  ：2004-06-19
 *修改记录  ：
 **************************************************************************/
#ifndef __GTP_CLIENT_H__
#define __GTP_CLIENT_H__

#include "../comm/comm_struct.h"
#include "gtp_status.h"
#include "cdr_buffer.h"
#include "sender_status.h"
#include "cdr_producer.h"
#include "deque"


/* GTP'消息格式 */
/*
 *|_________|_____________|_____________|__...  ...___|_____________|
 *|  消息头 |  信息区域1  |  信息区域2  |             |  信息区域n  |
 */
#define ECHO_REQ_INV      60   //单位:秒 ECHO REQUEST间隔
#define REQ_TIMEOUT       5    //单位:秒 REQUEST超时时间
#define TIMEOUT_RPT       5    //单位:次 超时重发次数
#define NODE_ALIVE_REQ    300  //单位:秒 NODE ALIVE 间隔

#define QUEUE_SENDER_LEN  20   //发送队列的长度

typedef struct _TDataFrameReqInfo
{
    UINT4       m_nSeqNumber;  //帧序列号
    SGtpFrame*  m_pFrame;      //GTP帧
    BOOL        m_bRecvAck;    //是否收到相应
    
    /*挂接的话单包,在放入GTP发送队列之前,数据*/
    /*包的数据m_pData将被删除,即在队列中的数 */
    /*据包无数据,只有头数据*/
    TBillPacket* m_pBillPacket; 

} TDataFrameReqInfo;

class CCDRSndSvr;

class CGtpClient
{
public:
    CGtpClient(CCDRBuffer* pBuffer, CSenderStatusFile* pStatusFile);

    virtual ~CGtpClient();
    
    virtual  BOOL GetGtpInfo();

    //秒事件处理函数
    virtual void Timer1Sec(void);
    
    //创建一个GTP帧
    SGtpFrame* CreateGTPFrame(UINT2 uLen);
    //复制一个GTP帧
    SGtpFrame* CopyGTPFrame(SGtpFrame *pFrame);
    
    //处理从GTP'服务器接收到的数据
    virtual void OnReceive(void* pFrame);
   
    
    //发送GTP请求
    void SendGTPRequest(SGtpFrame *pFrame);
    //发送GTP应答帧
    void SendGTPResponse(SGtpFrame *pFrame);
    
    //发送Echo Request/Response
    void SendEchoRequest();
    void SendEchoResponse(SGtpFrame *pFrame);
    
    //发送Node Alive Request/Response
    void SendNodeAliveRequest(UINT4 uLocalAddr);
    void SendNodeAliveResponse(SGtpFrame *pFrame);

    //发送Redirection Response
    void SendRedirectResponse();
    
    //发送Data TransferR equest
    void SendDataTransferRequest(SGtpFrame *pFrame);
    void SendNormalData(SGtpFrame *pFrame);
    void SendTestEmptyFrame(SGtpFrame *pFrame);
    void SendMaybeDupData(SGtpFrame *pFrame);
    void SendCancelData(SGtpFrame *pFrame);
    void SendReleaseData(SGtpFrame *pFrame);

    
    //发送版本不支持: 收到不支持的消息类型
    void SendVersionNotSupported(SGtpFrame *pFrame);

protected:
    //向GTP服务器发送数据帧
    int SendFrame(SGtpFrame* pFrame);
    
    //重置未应答帧记数信息
    void ResetReqFrameInfo();
    //清除所有信息
    void Clear();
   
    //相应ECHO REQUEST/RESPONSE 帧
    void OnEchoRequest(SGtpFrame *pFrame);
    void OnEchoResponse(SGtpFrame *pFrame);

    //相应Node ALIVE REQUEST/RESPONSE帧
    void OnNodeAliveRequest(SGtpFrame *pFrame);
    void OnNodeAliveResponse(SGtpFrame *pFrame);

    //相应DATA TRANSFER RESPONSE帧
    void OnDataTransferResponse(SGtpFrame *pFrame);

    //相应REDIRECTION REQUEST帧
    void OnRedirectRequest(SGtpFrame *pFrame);
    
    void OnResponse(SGtpFrame *pFrame);

protected:
    //GTP' 帧序列号,对于发送话单帧,顺序编号,其它消息均为0,使用
    //时为: ++m_nSeqNumber
    UINT2 m_nSeqNumber;

    //发送Echo Request帧的时间计数器
    int m_nEchoReqInvCounter;

    //协议帧头长
    int m_nGtpHeadLen;

    //GTP'协议帧长度限制
    int m_nBufferLimit;

     //GTP'节点状态信息
    SGtpInfo m_GtpNodeInfo;

    //话单传送模块指针
    CCDRSndSvr* m_pCDRSndSvr;

    //需要定时重发的NODE ALIVE REQEST帧
    SGtpReqFrameInfo  m_NodeAliveReq;

    //未应答的话单发送帧的队列,应答后,则置其标志为TRUE
    typedef std::deque<TDataFrameReqInfo*>   QUEUE_DATA_GTPREQ;
    QUEUE_DATA_GTPREQ  m_QueueDataReq;

protected:
    CSenderStatusFile*  m_fpSendStatus;
    CCDRBuffer*         m_pCdrBuffer;
    FILE*               m_fpGtpStatus;
};


template<class Tframe, class Tietype>
inline int CHECK_TV(Tframe* pframe, const char* szTietype, int ielen, Tietype*& pie, int noffset)
{
    if (pframe->uLen() < noffset + ielen)
    {/*检查本帧是否完整*/ 
        MSGOUT(MTS_CDRSENDER, MSGOUT_LEVEL_IMPORTANT, 
               S_NET_CHECK_TV_SIZE, 
               pframe->yType(), pframe->uLen(), 
               szTietype, noffset); 
        return -1; 
    } 
    else
    {
        pie = (Tietype *)(pframe->Data() + noffset);
        if (pie->yType() != Tietype::type)
        {/*检查ietype的类型是否正确*/
            MSGOUT(MTS_CDRSENDER, MSGOUT_LEVEL_IMPORTANT,
                   S_NET_CHECK_TV_IE_TYPE,
                   pframe->yType(), pframe->uLen(),
                   szTietype, noffset);
            return -2;
        }
    }

    return 0;
}

//CHECK_TLV检查一个TLV结构。该宏首先进行TV结构检查。然后检
//查帧长度是否足够。
template<class Tframe, class Tietype>
inline int CHECK_TLV(Tframe* pframe, const char* szTietype, int ielen, Tietype*& pie, int noffset)
{
    if (pframe->uLen() < noffset + ielen)
    {/*检查ietype帧头是否完整*/
        MSGOUT(MTS_CDRSENDER, MSGOUT_LEVEL_IMPORTANT,
               S_NET_CHECK_TV_SIZE,
               pframe->yType(), pframe->uLen(),
               szTietype, noffset);
        return -1;
    }
    else
    {
        pie = (Tietype *)(pframe->Data() + noffset);
        if (pie->yType() != Tietype::type)
        {/*检查ietype的类型是否正确*/
            MSGOUT(MTS_CDRSENDER, MSGOUT_LEVEL_IMPORTANT,
                   S_NET_CHECK_TV_IE_TYPE,
                   pframe->yType(), pframe->uLen(),
                   szTietype, noffset);
            return -2;
        }
        if (pframe->uLen() < noffset + IETLV_HEAD_LENGTH + pie->uLen())
        {/*检查本帧是否完整*/
            MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT,
                   S_NET_CHECK_TLV_SIZE,
                   pframe->yType(), pframe->uLen(),
                   szTietype, pie->uLen(), noffset);
            return -3;
        }
    }

    return 0;
}

#endif //__GTP_CLIENT_H__