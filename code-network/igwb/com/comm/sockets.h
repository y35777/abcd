#ifndef __CSOCKETS_H_
#define __CSOCKETS_H_

#include "../include/base_type.h"
#include "../include/frame.h"
#include "comm_struct.h"
#include "ace/SOCK_Dgram.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/INET_Addr.h"
#include "resource.h"

#define LOCAL_ADDR     "127.0.0.1"


//ͨ�Ų��Ƕ��̵߳ġ�Ϊ�˱�����Щ�̼߳�ķ��ʳ�ͻ��
//��Ҫ����һ�����������Ӷ�ʹ����Щ�߳��Դ��еķ�
//ʽ���С�CCommMutex�����˸û�����
class CCommMutex
{
public:
    //���CCommMutex��ʵ������ָ��
    static CCommMutex* const Instance();

    //����CCommMutex��ʵ������
    static void Destroy();

    //�����û�������ʹ��Ȩ
    void Acquire();

    //�ͷŻ�������ʹ��Ȩ
    void Release();

protected:
    CCommMutex();
    ~CCommMutex();

    //����CCommMutexֻ֧��һ��ʵ��������
    //���Ըþ�ָ̬��ָ��ö���
    static CCommMutex* g_pTheMutex;

    //ACE�ṩ�ĵݹ��߳���
    ACE_Recursive_Thread_Mutex m_AceMutex;
};
//IP��ַ������
class CIPFilter{
public:
	LIST<UINT4> m_IPList;
	int m_nMode;
	CIPFilter() { m_nMode = 2;}
	~CIPFilter() {}
	int Init();
	int Filter(UINT4 uIP);

};
class CCommTask;
class CSocketImpl: public ACE_Event_Handler
{
public:
    CSocketImpl();
    virtual ~CSocketImpl();

    //��һ�ν������ݵ���ǰʱ�̵�ʱ����λΪs
    unsigned long m_lTimeNoData;

    //��ACE_Reactor��ע���¼�
    void RemoveHandler();

    //��ACE_Reactor������ĳ��CSocketImpl��������SOCKET�����¼���
    //����øö���ı�����
    virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);

    //��ACE_Reactor������ĳ��CSocketImpl��������SOCKET����¼���
    //���øö���ı�����
    virtual int handle_output(ACE_HANDLE fd = ACE_INVALID_HANDLE);

    //��ACE_Reactor������ĳ��CSocketImpl��������SOCKET�ر��¼���
    //����øö���ı�����
    virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);

protected:

    //�������ṩʵ�ʴ���SOCKET�ϵ������¼����麯���ӿ�
    virtual int SOCK_handle_input(void) = 0;

    //�������ṩʵ�ʴ���SOCKET�ϵ�����¼����麯���ӿ�
    virtual int SOCK_handle_output(void);

    //�������ṩʵ�ʴ���SOCKET�ϵĹر��¼����麯���ӿ�
    virtual int SOCK_handle_close(void);

    friend class CCommTask;
    //ָ������¼���Task����
    CCommTask* m_pCommTask;

    //�Ƿ��ǹ���״̬
    BOOL m_bFailure;
};


class CCommRouter;
//������֡Ϊ��λ��SOCKET���շ������ݵĻ���
class CFrmSocket: public CSocketImpl
{
public:
    CFrmSocket(ACE_SOCK_Stream* pSocketStream);
    virtual ~CFrmSocket();

    //��������֡
    virtual int Send(const void* const pAFrame);

    //�ϲ�CComRouter�����ָ��
    CCommRouter* m_pCommRouter;

    //��öԶ˵�ַ
    ACE_INET_Addr& GetAddrRemote();

    //ACE_SOCK_Stream ��װ��������ʽ�������ݵ�SOCKET
    ACE_SOCK_Stream* GetSockStream();

    //��ñ��ص�ַ
    inline void MakeLocalAddress()
    {
        m_pSocketStream->get_local_addr(m_addrLocal);
    }

protected:

    //���������
    void Clear(void);

    //ACE_Reactor��ò���SOCKET�¼�����Ļص�����
    virtual ACE_HANDLE get_handle(void) const;

    //����SOCKET�����¼�
    virtual int SOCK_handle_input(void);

    //����SOCKET����¼�
    virtual int SOCK_handle_output(void);

    //����SOCKET�ر��¼�
    virtual int SOCK_handle_close(void);

    //�ӽ��ջ���������ȡ����֡��������ȡ��������֡��ָ����뵽
    //���ն��еĶ�β
    virtual void ReceiveFrames(void) = 0;

    //���ϲ��ύһ֡����
    virtual void OnReceiveFrame(void* pAFrame);

    //��FIFO��ʽ�����Ͷ����е�����֡���ͳ�ȥ��ֱ��SOCKET������
    virtual int ProcessPendingWrite(void);

    //���һ֡���ݵĳ���
    virtual long GetFrameSize(const char* const pAFrame) = 0;

    //���˵�IP��ַ�Ͷ˿�
    ACE_INET_Addr m_addrLocal;

    //�Զ˵�IP��ַ�Ͷ˿�
    ACE_INET_Addr m_addrRemote;

    //���ջ�����
    char* m_pRecvBuf;

    //���ջ������ĳ���
    long m_lRecvBufSize;

    //ÿ�δ�SOCKET��������ʱ��ʵ�������յ����ݵ��ܳ�
    long m_lBytesReceived;

    //���ն���
    LIST <char*> m_InFrames;

    //�����ն��ж�βԪ��ָ�������֡�Ѿ��������ؽ��գ�
    //��m_bLastOK ����TRUE���������FALSE
    BOOL m_bLastOK;

    //�ó�Ա������ʾ���ն��ж�βԪ��ָ�������֡��ʵ�ʳ���
    long m_lLastFrameLen;

    //���Ͷ���
    LIST <char*> m_OutFrames;

    //�ó�Ա������ʾ���Ͷ��е���󳤶�
    long m_lOutFramesMax;

    //��ָ��ָ��ǰ���ڷ��͵�����֡
    char* m_pSendFrame;

    //m_pSendFrameָ�������֡�Ѿ������͵��ֽ���
    long m_lBytesSent;

    //ACE_SOCK_Stream��װ��������ʽ�������ݵ�SOCKET
    ACE_SOCK_Stream* m_pSocketStream;

};

//�Զ���������֡Ϊ��λ��SOCKET���շ����ݵ�SOCKET�Ļ���
class CBinFrmSocket:public CFrmSocket
{
public:
    CBinFrmSocket(ACE_SOCK_Stream* pSocketStream);
    virtual ~CBinFrmSocket();

protected:

    //�ӽ��ջ���������ȡ������֡����������ȡ�����Ķ�����֡��ָ��
    //���뵽���ն��еĶ�β
    virtual void ReceiveFrames(void);

    //��ȡ����֡�ĳ���
    virtual long GetFrameSize(const char* const pAFrame);

    //���һ֡���ݵ��������ĳ���
    virtual long GetDataSize(const char* const pAFrame) = 0;

    //֡ͷ�ĳ���
    long m_lHeadLen;

    //�û�����������ʱ��ŵ�ǰ���ڽ��յĶ�����֡��֡ͷ����
    char* m_pFrameHead;
};

class CMemFrmSocket:public CBinFrmSocket
{
public:

    CMemFrmSocket(ACE_SOCK_Stream* pSocketStream);
    virtual ~CMemFrmSocket();

    //����Ƿ������˿ڵ���Ϣ
    BOOL GetbPrimary();

    //�����Ƿ������˿ڵ���Ϣ
    void SetbPrimary(BOOL bPrimary);

    //�������ģ���
    int GetModuleNum();

    //��������ģ���
    void SetModuleNum(int nNum);

protected:

    //���һ֡SMemFrameʵ�����������ĳ���
    virtual long GetDataSize(const char* const pAFrame);

    //����������CSlideWndCommRouter�����ύ����
    virtual void OnReceiveFrame(void* pAFrame);

    //�Ƿ������˿�
    BOOL m_bPrimary;

    //�ó�Ա������ʾ����������������ģ���
    int m_nModuleNum;
};

//����Զ2002.04.08���
class CSysFrmSocket:public CMemFrmSocket
{
public:

    CSysFrmSocket(ACE_SOCK_Stream* pSocketStream);
    virtual ~CSysFrmSocket();

protected:

    //���һ֡SSysFrameʵ�����������ĳ���
    virtual long GetDataSize(const char* const pAFrame);

    //����������CSoftXCommRouter�����ύ����
    virtual void OnReceiveFrame(void* pAFrame);

};
//����Զ2002.04.08��ӽ���

class CMsgFrmSocket:public CBinFrmSocket
{
public:

    CMsgFrmSocket(ACE_SOCK_Stream* pSocketStream);
    virtual ~CMsgFrmSocket();

protected:

    //���һ֡MSG_INTRAʵ�����������ĳ���
    virtual long GetDataSize(const char* const pAFrame);

    //��������������CMsgSvrCommRouter�����
    //CMsgCliCommRouter�����ύ����
    virtual void OnReceiveFrame(void* pAFrame);
};

class CMmlFrmSocket:public CFrmSocket
{
public:
	CMmlFrmSocket(ACE_SOCK_Stream* pSocketStream);
    virtual ~CMmlFrmSocket();

    //����һ���ı�֡��ʼ������ı�־
    void AddTags(const char yTag, const BOOL bBeginTag);

    //����MML�ļ�����֡
    virtual int Send(const void* const pAFrame);

protected:

    //�������ӽ��ջ���������ȡ�ı�֡����������ȡ��������
    //��֡��ָ����뵽���ն��еĶ�β
    virtual void ReceiveFrames(void);

    //�������֡����
    virtual long GetFrameSize(const char* const pAFrame);

    //����������CMmlCommRouter�����ύ����
    void OnReceiveFrame(void* pAFrame);

    //�ó�Ա������һ�����ϣ������е�Ԫ��Ϊ�ı�֡�����ı�־
    SET <char> m_yEndTags;

    //�ó�Ա������һ�����ϣ������е�Ԫ��Ϊ�ı�֡��ʼ�ı�־
    SET <char> m_yBeginTags;
};

class CGtpUdpCommRouter;

class CGtpUdpSocket:public CSocketImpl
{
public:

    #define GTP_INPUT_BUFFER_MAXLENGTH (64*1024)

    CGtpUdpSocket(CGtpUdpCommRouter* const pGtpCommRouter);
    virtual ~CGtpUdpSocket();

    //��һ���շ�UDP���ݱ��Ķ˿�
    int Open(const char* const szIPAddr, const int nPort);

    //��ñ������Ӧ��IP��ַ�Ͷ˿ں�
    void GetAddr(unsigned int& nIPAddr, int& nPort);

    //����һ��UDP���ݱ�
    int Send(const MSG_INTRA* const pMsg);

    //ָ����������CGtpUdpCommRouter����
    CGtpUdpCommRouter *m_pGtpCommRouter;

protected:

    //��CGtpUdpSocket ����ע�ᵽACE_Reactor�����,ACE_Reactor
    //�����ص�CGtpUdpSocket�����get_handle�������Ӷ���ò���
    //SOCKET�¼��ľ��
    virtual ACE_HANDLE get_handle(void) const;

    //����SOCKET�����¼�
    virtual int SOCK_handle_input(void);

    //����SOCKET�ر��¼�
    virtual int SOCK_handle_close(void);

    //����IP��ַ�Ͷ˿�
    ACE_INET_Addr m_addrLocal;

    //���ڽ���UDP���ݱ��Ļ�����,�û������ĳ���Ϊ64KB
    char* m_pRecvBuf;

    //ACE�ṩ��ACE_SOCK_Dgram�������շ�UDP���ݱ�
    ACE_SOCK_Dgram m_SocketDgram;
};

//iGWB��convert���ӵ�socket
class CABFrmSocket: public CBinFrmSocket
{
public:
    CABFrmSocket(ACE_SOCK_Stream* pSocketStream);
    virtual ~CABFrmSocket();

protected:

    //���һ֡���ݵ��������ĳ���
    virtual long GetDataSize(const char* const pAFrame);
    
	//����������CAlarmBoxRouter�����ύ����
    void OnReceiveFrame(void* pAFrame);
};

class CGtpFrmSocket:public CBinFrmSocket
{
public:

    CGtpFrmSocket(ACE_SOCK_Stream* pSocketStream);
    virtual ~CGtpFrmSocket();

protected:

    //���һ֡SGtpFrameʵ�����������ĳ���
    virtual long GetDataSize(const char* const pAFrame);

    //����������CGtpFrmCommRouter�����ύ����
    virtual void OnReceiveFrame(void* pAFrame);

    //����SOCKET�ر��¼�
    virtual int SOCK_handle_close(void);

};

///////////////////////////////////////////////////////////////////////////////
//GTP'�Ʒѽӿ����� Z30971 2004/06/10

class CGTPUdpCliCommRouter;
class CGtpUdpCliSocket: public CGtpUdpSocket
{
public:
    CGtpUdpCliSocket(CGTPUdpCliCommRouter* const pGtpCliCommRouter);
    virtual ~CGtpUdpCliSocket();
    
    virtual int Send(const MSG_INTRA* const pMsg);

    
    //ָ����������CGtpUdpCliCommRouter����
    CGTPUdpCliCommRouter *m_pCommRouter;

protected:
    //����SOCKET�����¼�
    virtual int SOCK_handle_input(void);
    //����SOCKET�ر��¼�
    virtual int SOCK_handle_close(void);

};

class CGtpTcpCliSocket: public CGtpFrmSocket
{
public:
    CGtpTcpCliSocket(ACE_SOCK_Stream* pSocketStream);
    virtual ~CGtpTcpCliSocket();

protected:
    //����������CGtpFrmCommRouter�����ύ����
    virtual void OnReceiveFrame(void* pAFrame);
};
#endif //__CSOCKETS_H_
