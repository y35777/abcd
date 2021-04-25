/**************************************************************************
 *��Ʒ��    ��iGateway Bill V200        
 *ģ����    ��GTP'�Ʒѽӿ�
 *�ļ���    : gtp_client.h
 *������Ϣ  �����ļ�����CGtpClient�Ķ���
 *�汾˵��  ��V200R002B02   
 *��Ȩ��Ϣ  ����Ȩ���У�C��2004-2005 ��Ϊ�������޹�˾
 *����      ��Z30971
 *����ʱ��  ��2004-06-19
 *�޸ļ�¼  ��
 **************************************************************************/
#ifndef __GTP_CLIENT_H__
#define __GTP_CLIENT_H__

#include "../comm/comm_struct.h"
#include "gtp_status.h"
#include "cdr_buffer.h"
#include "sender_status.h"
#include "cdr_producer.h"
#include "deque"


/* GTP'��Ϣ��ʽ */
/*
 *|_________|_____________|_____________|__...  ...___|_____________|
 *|  ��Ϣͷ |  ��Ϣ����1  |  ��Ϣ����2  |             |  ��Ϣ����n  |
 */
#define ECHO_REQ_INV      60   //��λ:�� ECHO REQUEST���
#define REQ_TIMEOUT       5    //��λ:�� REQUEST��ʱʱ��
#define TIMEOUT_RPT       5    //��λ:�� ��ʱ�ط�����
#define NODE_ALIVE_REQ    300  //��λ:�� NODE ALIVE ���

#define QUEUE_SENDER_LEN  20   //���Ͷ��еĳ���

typedef struct _TDataFrameReqInfo
{
    UINT4       m_nSeqNumber;  //֡���к�
    SGtpFrame*  m_pFrame;      //GTP֡
    BOOL        m_bRecvAck;    //�Ƿ��յ���Ӧ
    
    /*�ҽӵĻ�����,�ڷ���GTP���Ͷ���֮ǰ,����*/
    /*��������m_pData����ɾ��,���ڶ����е��� */
    /*�ݰ�������,ֻ��ͷ����*/
    TBillPacket* m_pBillPacket; 

} TDataFrameReqInfo;

class CCDRSndSvr;

class CGtpClient
{
public:
    CGtpClient(CCDRBuffer* pBuffer, CSenderStatusFile* pStatusFile);

    virtual ~CGtpClient();
    
    virtual  BOOL GetGtpInfo();

    //���¼�������
    virtual void Timer1Sec(void);
    
    //����һ��GTP֡
    SGtpFrame* CreateGTPFrame(UINT2 uLen);
    //����һ��GTP֡
    SGtpFrame* CopyGTPFrame(SGtpFrame *pFrame);
    
    //�����GTP'���������յ�������
    virtual void OnReceive(void* pFrame);
   
    
    //����GTP����
    void SendGTPRequest(SGtpFrame *pFrame);
    //����GTPӦ��֡
    void SendGTPResponse(SGtpFrame *pFrame);
    
    //����Echo Request/Response
    void SendEchoRequest();
    void SendEchoResponse(SGtpFrame *pFrame);
    
    //����Node Alive Request/Response
    void SendNodeAliveRequest(UINT4 uLocalAddr);
    void SendNodeAliveResponse(SGtpFrame *pFrame);

    //����Redirection Response
    void SendRedirectResponse();
    
    //����Data TransferR equest
    void SendDataTransferRequest(SGtpFrame *pFrame);
    void SendNormalData(SGtpFrame *pFrame);
    void SendTestEmptyFrame(SGtpFrame *pFrame);
    void SendMaybeDupData(SGtpFrame *pFrame);
    void SendCancelData(SGtpFrame *pFrame);
    void SendReleaseData(SGtpFrame *pFrame);

    
    //���Ͱ汾��֧��: �յ���֧�ֵ���Ϣ����
    void SendVersionNotSupported(SGtpFrame *pFrame);

protected:
    //��GTP��������������֡
    int SendFrame(SGtpFrame* pFrame);
    
    //����δӦ��֡������Ϣ
    void ResetReqFrameInfo();
    //���������Ϣ
    void Clear();
   
    //��ӦECHO REQUEST/RESPONSE ֡
    void OnEchoRequest(SGtpFrame *pFrame);
    void OnEchoResponse(SGtpFrame *pFrame);

    //��ӦNode ALIVE REQUEST/RESPONSE֡
    void OnNodeAliveRequest(SGtpFrame *pFrame);
    void OnNodeAliveResponse(SGtpFrame *pFrame);

    //��ӦDATA TRANSFER RESPONSE֡
    void OnDataTransferResponse(SGtpFrame *pFrame);

    //��ӦREDIRECTION REQUEST֡
    void OnRedirectRequest(SGtpFrame *pFrame);
    
    void OnResponse(SGtpFrame *pFrame);

protected:
    //GTP' ֡���к�,���ڷ��ͻ���֡,˳����,������Ϣ��Ϊ0,ʹ��
    //ʱΪ: ++m_nSeqNumber
    UINT2 m_nSeqNumber;

    //����Echo Request֡��ʱ�������
    int m_nEchoReqInvCounter;

    //Э��֡ͷ��
    int m_nGtpHeadLen;

    //GTP'Э��֡��������
    int m_nBufferLimit;

     //GTP'�ڵ�״̬��Ϣ
    SGtpInfo m_GtpNodeInfo;

    //��������ģ��ָ��
    CCDRSndSvr* m_pCDRSndSvr;

    //��Ҫ��ʱ�ط���NODE ALIVE REQEST֡
    SGtpReqFrameInfo  m_NodeAliveReq;

    //δӦ��Ļ�������֡�Ķ���,Ӧ���,�������־ΪTRUE
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
    {/*��鱾֡�Ƿ�����*/ 
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
        {/*���ietype�������Ƿ���ȷ*/
            MSGOUT(MTS_CDRSENDER, MSGOUT_LEVEL_IMPORTANT,
                   S_NET_CHECK_TV_IE_TYPE,
                   pframe->yType(), pframe->uLen(),
                   szTietype, noffset);
            return -2;
        }
    }

    return 0;
}

//CHECK_TLV���һ��TLV�ṹ���ú����Ƚ���TV�ṹ��顣Ȼ���
//��֡�����Ƿ��㹻��
template<class Tframe, class Tietype>
inline int CHECK_TLV(Tframe* pframe, const char* szTietype, int ielen, Tietype*& pie, int noffset)
{
    if (pframe->uLen() < noffset + ielen)
    {/*���ietype֡ͷ�Ƿ�����*/
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
        {/*���ietype�������Ƿ���ȷ*/
            MSGOUT(MTS_CDRSENDER, MSGOUT_LEVEL_IMPORTANT,
                   S_NET_CHECK_TV_IE_TYPE,
                   pframe->yType(), pframe->uLen(),
                   szTietype, noffset);
            return -2;
        }
        if (pframe->uLen() < noffset + IETLV_HEAD_LENGTH + pie->uLen())
        {/*��鱾֡�Ƿ�����*/
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