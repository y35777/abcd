#ifndef __CCOMMROUTER
#define __CCOMMROUTER

#include "sockets.h"

class CCommRouterMan;
class CCommRouter
{
public:

    CCommRouter(CCommRouterMan* const pCommRouterMan,
                const unsigned long uAppType);

    virtual ~CCommRouter();

    //����*pMsg֡ͷ�е������Ϣ��ѡ���ʵ�����·��*pMsg��Data���
    //���ݷ��ͳ�ȥ
    virtual int Send(const MSG_INTRA* const pMsg) = 0;

    //��CCommRouterMan�����ύ�ӵͲ����·�Ͻ�����������Ϣ
    virtual void OnReceive(MSG_INTRA* pMsg);

    //��������*pFrmSocket������յ�������֡
    virtual void OnReceiveEx(CFrmSocket* pFrmSocket, void* pFrame);

    //Ϊ���˿�����һ��SOCKET����
    virtual int AddFrmSocket(CFrmSocket* pFrmSocket);

    //Ϊ���˿�ע��һ��SOCKET����
    virtual void DelFrmSocket(CFrmSocket* pFrmSocket);

    //���¼�������
    virtual void Timer1Sec(void);

    //ע��CSocketImpl����Ķ���
    virtual int RegisterHandle(CSocketImpl* pSocket, ACE_Reactor_Mask mask);

protected:

    //���ڼ��й������е�CCommRouter����
    CCommRouterMan* m_pCommRouterMan;

    //��Ӧ��Ӧ������
    unsigned long m_uAppType;

};

class CCommRouterMan
{
public:
    CCommRouterMan(CCommLayer* const pCommLayer);
    virtual ~CCommRouterMan();

    //ע��CCommRouter����Ķ���
    int RegisterCommRouter(CCommRouter* pCommRouter,
                            unsigned long uAppType);

    //ΪӦ������ΪuAppType�Ķ˿�����һ��������
    int AddFrmSocket(CFrmSocket* pFrmSocket,
                    unsigned long uAppType);

    //����*pMsg֡ͷ�е������Ϣ��ѡ���ʵ���CCommRouter����
    //�����øö����Send����
    int Send(const MSG_INTRA* const pMsg);

    //����Ϊͨ�Ų��CCommLayer�����ύ�²��CCommRouter����
    //�����ύ��������Ϣ
    void OnReceive(MSG_INTRA* pMsg);

    //�붨ʱ���¼�
    void Timer1Sec(void);

    //ע��CSocketImpl����Ķ���
    int RegisterHandle(CSocketImpl* pSocket, ACE_Reactor_Mask mask);

protected:

    //��ָ��ָ�������Ϣ��ת��CCommRouter����
    CCommRouter* m_pMsgCommRouter;

    typedef MAP<unsigned long, CCommRouter*> MAP_APT_TO_PCOMMROUTER;

    //�������CCommRouter����ָ�������
    MAP_APT_TO_PCOMMROUTER m_CommRouterMap;

    //��ָ��ָ����Ϊͨ�Ų��CCommLayer����Ķ���
    CCommLayer* m_pCommLayer;
};

//��GTP��Э��UDP��ʽ�ķ������˵ĳ���
class CGtpUdpCommRouter:public CCommRouter
{
public:
    CGtpUdpCommRouter(CCommRouterMan* const pCommRouterMan,
                        const unsigned long uAppType);
    virtual ~CGtpUdpCommRouter();

    //�������ô�������ʼ����Ӧ��CGtpUdpSocket���󣬲�����Щ����
    //ע�ᵽCCommTaskMan������
    int Init(unsigned char const nAPID);

    //����*pMsg֡ͷ�е������Ϣ��ѡ���ʵ���CGtpUdpSocket����
    //*pMsg��Data������ݷ��ͳ�ȥ
    virtual int Send(const MSG_INTRA* const pMsg);

    //Ϊ���˿�ɾ��һ��SOCKET���ӣ��������CGtpUdpSocket��������
    //SOCK_handle_close��������ñ�����
    virtual void DelGtpSocket(CGtpUdpSocket* pGtpSocket);

protected:

    typedef LIST <CGtpUdpSocket*> LIST_PGTPSOCKET;
  
    //�������GTPSOCKET����ָ�������
    LIST_PGTPSOCKET m_SocketList;
};

//��GTP��Э��TCP��ʽ�ķ������˵ĳ���
class CGtpFrmCommRouter:public CCommRouter
{
public:
    CGtpFrmCommRouter(CCommRouterMan* const pCommRouterMan,
                        const unsigned long uAppType);
    virtual ~CGtpFrmCommRouter();

    //Ϊ���˿�����һ��SOCKET����
    virtual int AddFrmSocket(CFrmSocket* pFrmSocket);

    //��ʼ��m_GsnMultiIPMap��m_GsnMultiIPTable
    int Init(unsigned char const nAPID);

    //����*pMsg֡ͷ�е������Ϣ��ѡ���ʵ���CGtpFrmSocket����
    //*pMsg��Data������ݷ��ͳ�ȥ
    virtual int Send(const MSG_INTRA* const pMsg);

    //Ϊ���˿�ɾ��һ��SOCKET���ӣ��������CGtpFrmSocket��������
    //SOCK_handle_close��������ñ�����
    virtual void DelGtpSocket(CSocketImpl* pGtpSocket);

protected:

    //�������TCP��ʽ��CGtpFrmSocket����ָ���MAP,����Ϊ�Զ�IP��ַ
    MAP<UINT4, CGtpFrmSocket*>  m_GtpFrmSocketMap;

    //��IP��ַ��Ӧ��m_GsnMultiIPTable��GSN��������
    MAP<UINT4, int>        m_GsnMultiIPMap;

    //���ж��ַGSN��IP��ַ��
    VECTOR<VECTOR<UINT4> > m_GsnMultiIPTable;

};

class CMscLinkPair
{
public:
    CMscLinkPair();
    virtual ~CMscLinkPair();

    //���ô��������ӵ�CMemFrmSocket����
    void SetPrimaryLink(CMemFrmSocket* pLink);

    //���ô������ӵ�CMemFrmSocket����
    void SetSecondaryLink(CMemFrmSocket* pLink);

    //��ô��������ӵ�CMemFrmSocket����
    CMemFrmSocket* const GetPrimaryLink(void);

    //��ô������ӵ�CMemFrmSocket����
    CMemFrmSocket* const GetSecondaryLink(void);

protected:

    //��ָ��ָ��Ķ����������ģ����Ʒ�����֮���������
    CMemFrmSocket* m_pPrimaryLink;

    //��ָ��ָ��Ķ����������ģ����Ʒ�����֮��ı�����
    CMemFrmSocket* m_pSecondaryLink;
};

class CLinkPairs
{
public:
    CLinkPairs();
    virtual ~CLinkPairs();

    //����*pLink��Ӧ������ģ��ŵĸı�
    void OnLinkModuleNumberChanged(CFrmSocket* const pLink,
                                    int const nOldModNum,
                                    int const nNewModNum);

    //��������ģ��Ż�ö�Ӧ��CMscLinkPair�����ָ��
    CMscLinkPair* const GetLinkPairByModuleNumber(int const nModuleNumber);

    //Ϊ���˿�ɾ��һ��SOCKET����
    void DelFrmSocket(CFrmSocket* pFrmSocket);

	//����Զ2002.04.08���
    //����pMsg֡�е������Ϣ��ѡ���ʵ�����·��*pMsg��Data��
    //�����ݷ��ͳ�ȥ
    virtual int Send(const MSG_INTRA* const pMsg);

    //���¼�������
    void Timer1Sec(void);

	//����������ʱ����
	void SetLinkTimeOut(int nLinkTimeOut);

protected:

    typedef MAP <int, CMscLinkPair*> MAP_MODNUM_TO_PCMSCLINKPAIR;

    //������ģ���Ϊ��ֵ��CMscLinkPairָ��MAP
    MAP_MODNUM_TO_PCMSCLINKPAIR m_MapLinks;

	//����Զ2002.04.08���
	//socket�ĳ�ʱ���ƣ�����Ϊ��λ
	int	m_nLinkTimeOut;
	//����Զ2002.04.08��ӽ���
};

class CSoftXLinkPairs
{
public:
    CSoftXLinkPairs();
    virtual ~CSoftXLinkPairs();

    //��������ģ��Ż�ö�Ӧ��CMscLinkPair�����ָ��
    CMscLinkPair* const GetLinkPairByModuleNumber(int const nModuleNumber);

    //����pMsg֡�е������Ϣ��ѡ���ʵ�����·��*pMsg��Data��
    //�����ݷ��ͳ�ȥ
    virtual int Send(const MSG_INTRA* const pMsg);

    //Ϊ���˿�ɾ��һ��SOCKET����
    virtual void DelFrmSocket(CFrmSocket* pFrmSocket);

    //ע��ģ���
    int RegisterModule(CFrmSocket* pLink, int nModNum);

protected:
    typedef MAP <int, CMscLinkPair*> MAP_MODNUM_TO_PCMSCLINKPAIR;

    //������ģ���Ϊ��ֵ��CMscLinkPairָ��MAP
    MAP_MODNUM_TO_PCMSCLINKPAIR m_MapLinks;
};

//CSlideWndCommRouter�����˻�������Э��������˵Ķ˿ڣ��������������˿ڣ�
class CSlideWndCommRouter : public CCommRouter
{
public:
    CSlideWndCommRouter(CCommRouterMan* const pCommRouterMan,
                        const unsigned long uAppType);
    virtual ~CSlideWndCommRouter();

    //Ϊ���˿�����һ��SOCKET����
    virtual int AddFrmSocket(CFrmSocket* pFrmSocket);

    //Ϊ���˿�ɾ��һ��SOCKET����
    virtual void DelFrmSocket(CFrmSocket* pFrmSocket);

    //��������*pFrmSocket������յ�������֡
    virtual void OnReceiveEx(CFrmSocket* pFrmSocket,
                             void* pFrame);

    //����*pMsg֡ͷ�е������Ϣ��ѡ���ʵ�����·��*pMsg��Data��
    //�����ݷ��ͳ�ȥ
    virtual int Send(const MSG_INTRA* const pMsg);

    //���¼�������
    virtual void Timer1Sec(void);

	//����Զ2002.04.08���
	//����������ʱ����
	void SetLinkTimeOut(int nLinkTimeOut);
	//����Զ2002.04.08��ӽ���


protected:

    //�����������MSC�����ڵ���Ʒ����ص���������
    CLinkPairs m_LinkPairs;

    typedef MAP <CMemFrmSocket*, int>  MAP_PCMEMFRMSOCKET_TO_INT;

    //δ�������Ź�����Socket���ӵ�MAP
    MAP_PCMEMFRMSOCKET_TO_INT m_SlideWndSocketMap;
};


//����Զ2002.04.08���
class CSoftXCommRouter : public CCommRouter
{
public:
    CSoftXCommRouter(CCommRouterMan* const pCommRouterMan,
                        const unsigned long uAppType);
    virtual ~CSoftXCommRouter();

    //Ϊ���˿�����һ��SOCKET����
    virtual int AddFrmSocket(CFrmSocket* pFrmSocket);

    //Ϊ���˿�ɾ��һ��SOCKET����
    virtual void DelFrmSocket(CFrmSocket* pFrmSocket);

    //��������*pFrmSocket������յ�������֡
    virtual void OnReceiveEx(CFrmSocket* pFrmSocket,
                             void* pFrame);

    //����*pMsg֡ͷ�е������Ϣ��ѡ���ʵ�����·��*pMsg��Data��
    //�����ݷ��ͳ�ȥ
    virtual int Send(const MSG_INTRA* const pMsg);

    //���¼�������
    virtual void Timer1Sec(void);

	//����������ʱ����
	void SetLinkTimeOut(int nLinkTimeOut);


protected:
    //�����������MSC�����ڵ���Ʒ����ص���������
    CSoftXLinkPairs m_LinkPairs;

	//���е�Socket����ָ������
	VECTOR<CSysFrmSocket* >		m_AllFrmSockets;

	//socket�ĳ�ʱ���ƣ�����Ϊ��λ
	int	m_nLinkTimeOut;
};
//����Զ2002.04.08��ӽ���


class CMsgSvrCommRouter:public CCommRouter
{
public:
    CMsgSvrCommRouter(CCommRouterMan* const pCommRouterMan,
                        const unsigned long uAppType);
    virtual ~CMsgSvrCommRouter();

    //Ϊ���˿�����һ��SOCKET����
    virtual int AddFrmSocket(CFrmSocket* pFrmSocket);

    //Ϊ���˿�ɾ��һ��SOCKET����
    virtual void DelFrmSocket(CFrmSocket* pFrmSocket);

    //��������*pFrmSocket������յ�������֡
    virtual void OnReceiveEx(CFrmSocket* pFrmSocket,
                             void* pFrame);

    //����*pMsg֡ͷ�е������Ϣ��ѡ���ʵ�����·��*pMsg��Data��
    //�����ݷ��ͳ�ȥ
    virtual int Send(const MSG_INTRA* const pMsg);

protected:

    typedef MAP <int,CMsgFrmSocket*> MAP_PT_TO_PMSGFRMSOCKET;

    //�Խ��̱��Ϊ��ֵ��������ֵΪCMsgFrmSocket�����ָ��
    MAP_PT_TO_PMSGFRMSOCKET m_SocketMap;

    typedef LIST <CMsgFrmSocket*> LIST_PMSGFRMSOCKET;

    //�������е�Ԫ����ָ���CMsgFrmSocket������ʱû������̱�Ű�
    LIST_PMSGFRMSOCKET m_SocketList;
};

//�����˼Ʒ������г�KERNEL����֮���������̵���Ϣ��ת�Ŀͻ���
class CMsgCliCommRouter:public CCommRouter
{
public:
    CMsgCliCommRouter(CCommRouterMan* const pCommRouterMan,
                        const unsigned long uAppType,
                        PROCESS_TYPE const nProcessType);
    virtual ~CMsgCliCommRouter();

    //Ϊ���˿�ɾ��һ��SOCKET����
    virtual void DelFrmSocket(CFrmSocket* pFrmSocket);

    //���¼�������
    virtual void Timer1Sec(void);

    //����pMsgָ�������֡
    virtual int Send(const MSG_INTRA* const pMsg);

protected:

    //��Kernel���̽�������
    int Connect(void);

    //��ָ��ָ��Ķ����ʾ���ڽ�����KERNEL���̵���Ϣ��ת������
    //��֮�������
    CMsgFrmSocket* m_pMsgFrmSocket;

    //�ó�Ա������ʾ���������ڽ��̵Ľ��̱��
    PROCESS_TYPE m_nProcessType;

    //���ϴ�û�гɹ�����KERNEL���̵���Ϣ��ת�������˿�ʼ������
    //��ʱ�䣬��λΪS
    int m_nReConnectTimer;

};

//�����˲���ά������ͨ�Ų��е�MML�˿�
class CMmlCommRouter:public CCommRouter
{
public:
    enum
    {
        MIN_CLIENT_NO = 1,      //��С�ͻ��˺�
        MAX_CLIENT_NO = 255     //���ͻ��˺�
    };

    CMmlCommRouter(CCommRouterMan* const pCommRouterMan,
                        const unsigned long uAppType);
    virtual ~CMmlCommRouter();

    //Ϊ���˿�����һ��SOCKET����
    virtual int AddFrmSocket(CFrmSocket* pFrmSocket);

    //Ϊ���˿�ɾ��һ��SOCKET����
    virtual void DelFrmSocket(CFrmSocket* pFrmSocket);

    //���¼�������
    virtual void Timer1Sec(void);

    //��������*pFrmSocket������յ�������֡
    void OnReceiveEx(CFrmSocket* pFrmSocket, void* pFrame);

    //����pMsgָ�������֡
    virtual int Send(const MSG_INTRA* const pMsg);

protected:
    void SendMsgToMMLServer(const char *szCmd, UINT2 nClientNo, UINT4 uIp);

    typedef MAP <unsigned long, CMmlFrmSocket*> MAP_CN_TO_PCMMLFRMSOCKET;

    //��MAP��Ԫ��ΪCMmlFrmSocket�����ָ�룬һ��
    //CMmlFrmSocket�����ʾһ��MML�ͻ�����Ʒ���
    //��֮������ӣ�ÿ�����Ӷ�����һ��Ψһ�ı��
    MAP_CN_TO_PCMMLFRMSOCKET m_SocketMap;
};

//�����˼Ʒ�����OM������convert֮�����·����
class CAlarmBoxRouter:public CCommRouter
{
public:
    CAlarmBoxRouter(CCommRouterMan* const pCommRouterMan,
                        const unsigned long uAppType);
    virtual ~CAlarmBoxRouter();

    //Ϊ���˿�����һ��SOCKET����
    virtual int AddFrmSocket(CFrmSocket* pFrmSocket);

    //Ϊ���˿�ɾ��һ��SOCKET����
    virtual void DelFrmSocket(CFrmSocket* pFrmSocket);

    //��������*pFrmSocket������յ�������֡
    virtual void OnReceiveEx(CFrmSocket* pFrmSocket, void* pFrame);

    //���Ӹ澯ģ�鷢��������ת����convert
    virtual int Send(const MSG_INTRA* const pMsg);

protected:

    //����convertģ����Ʒ����ص�����
	CABFrmSocket* m_pABFrmSocket;
};

///////////////////////////////////////////////////////////////////////////////
//GTP'�Ʒѽӿ����� Z30971 2004/06/10

//��GTP'Э��TCP��ʽ�Ŀͻ��˵ĳ���
class CGTPTcpCliCommRouter:public CCommRouter
{
public:
    CGTPTcpCliCommRouter(CCommRouterMan* const pCommRouterMan,
                         const unsigned long uAppType,
                         PROCESS_TYPE const nProcessType);
    virtual ~CGTPTcpCliCommRouter();

    //Ϊ���˿�ɾ��һ��SOCKET����
    virtual void DelFrmSocket(CFrmSocket* pFrmSocket);

    //���¼�������
    virtual void Timer1Sec(void);

    //����pMsgָ�������֡
    virtual int Send(const MSG_INTRA* const pMsg);

protected:

    //��BS���������̽�������
    int Connect(void);

    //��ָ��ָ��Ķ����ʾ�Ʒѽӿڽ�����BS������
    CGtpTcpCliSocket* m_pGtpTcpCliSocket;

    //�ó�Ա������ʾ���������ڽ��̵Ľ��̱��
    PROCESS_TYPE m_nProcessType;

    //���ϴ�û�гɹ�����BS�������˿�ʼ��������ʱ�䣬��λΪS
    int m_nReConnectTimer;

};

//��GTP'Э��UDP��ʽ�Ŀͻ��˵ĳ���
class CGTPUdpCliCommRouter:public CCommRouter
{
public:
    CGTPUdpCliCommRouter(CCommRouterMan* const pCommRouterMan,
                         const unsigned long uAppType,
                         PROCESS_TYPE const nProcessType);
    virtual ~CGTPUdpCliCommRouter();

    //Ϊ���˿�ɾ��һ��SOCKET���ӣ��������CGtpUdpSocket��������
    //SOCK_handle_close��������ñ�����
    virtual void DelGtpSocket(CGtpUdpCliSocket* pGtpSocket);

    //���¼�������
    virtual void Timer1Sec(void);

    //����pMsgָ�������֡
    virtual int Send(const MSG_INTRA* const pMsg);

protected:
    //��ʼ��SOCKET
    int Init();

    //��ָ��ָ��Ķ����ʾ�Ʒѽӿڽ�����BS������
    CGtpUdpCliSocket* m_pGtpUdpCliSocket;

    //�ó�Ա������ʾ���������ڽ��̵Ľ��̱��
    PROCESS_TYPE m_nProcessType;
};
#endif //__CCOMMROUTER
