#ifndef __CCOMMLAYER
#define __CCOMMLAYER

#include "comm_router.h"
#include "ace/Task.h"

class CCommListener:public CSocketImpl
{
public:
    CCommListener(unsigned long const uAppTpye,
                    CCommLayer* const pCommLayer);
    virtual ~CCommListener();

    //��ʼ��ָ����IP��ַ�Ͷ˿�������
    int Open(const char* const szIPAddr, const int nPort, const BOOL bReuse = 0);
protected:

    //��CCommListener����ע�ᵽACE_Reactor�����ACE_Reactor����
    //��ص�CCommListener�����get_handle����,�Ӷ���ò���SOCKET�¼�
    //�ľ��
    virtual ACE_HANDLE get_handle(void) const;

    //����SOCKET�������¼�
    virtual int SOCK_handle_input(void);

    //�������Ӳ�����m_uAppType��ȡֵ����һ����ӦCFrmSocket����Ķ���
    virtual CFrmSocket* CreateFrmSocket(void);

    //һ��CCommListener�����Ӧһ��Ӧ������
    unsigned long m_uAppType;

    //ACE�ṩ��ACE_SOCK_Acceptor����������SOCKET
    ACE_SOCK_Acceptor m_Acceptor;

    //��ָ��ָ����Ϊͨ�Ų��CCommLayer����
    CCommLayer* m_pCommLayer;

};

class CCommTaskMan;
class CCommTask:public ACE_Task_Base
{
public:
    CCommTask(CCommTaskMan* pCommTaskMan);
    virtual ~CCommTask();

    //������������һ�����ڼ��Reactor�¼����߳�
    virtual int open(void* args = 0);

    //�̺߳���,���������ṩһ��Reactor���¼�ѭ��
    virtual int svc(void);

    //��CSocketImpl����Ķ���ע�ᵽ�������ACE_Reactor������
    virtual int RegisterHandle(CSocketImpl* pSocket, ACE_Reactor_Mask mask);

    //�����߳�
    void QuitComTask(void);

    //��pSocketָ�������ɾ���б�
    void ScheduleRemove(CSocketImpl* pSocket);

    //��pSocketָ�������ע���б�
    int ScheduleRegister(CSocketImpl* pSocket, ACE_Reactor_Mask mask);

    //��pSocketָ�������ע��output�¼����б�
    void ScheduleWakeup(CSocketImpl* pSocket);

protected:

    //ɾ����ɾ���б��е�Socket����
    void DelScheduleSocket();

    //ע���ע���б��е�Socket����
    void RegScheduleSocket();

    //ע��m_SocketWillRmOptHdl��Socket�����output�¼�
    void SleepSocketOutput();

    ACE_Reactor m_Reactor;

    //��Ҫɾ����Socketָ���б�
    typedef LIST <CSocketImpl*> LIST_SOCKET;
    LIST_SOCKET m_SocketWillBeDel;

    //��Ҫע���Socketָ���б�
    typedef MAP <CSocketImpl*, ACE_Reactor_Mask> MAP_SOCKET_TO_MASK;
    MAP_SOCKET_TO_MASK m_SocketWillReg;

    //��Ҫע��output�¼���Socketָ���б�
    LIST_SOCKET m_SocketWillRmOptHdl;

    //Reactor�Ƿ�����
    BOOL m_bReactorFull;

    CCommTaskMan* m_pCommTaskMan;
};

class CCommTaskMan
{
public:
    CCommTaskMan(CCommLayer* pCommLayer);
    virtual ~CCommTaskMan();

    //��ʼ������
    int Init(void);

    //ע��CSocketImpl����Ķ���
    int RegisterHandle(CSocketImpl* pSocket, ACE_Reactor_Mask mask);

    //��������CCommTask����
    int Active();

    //ȥ��������CCommTask����
    void DeActive();

    CCommLayer* GetCommLayer();

protected:

    typedef LIST <CCommTask*> LIST_PCOMMTASK;

    //�������CCommTask����ָ�������
    LIST_PCOMMTASK m_CommTaskList;

    CCommLayer* m_pCommLayer;
};



//CCommLayer������ͨ�Ų㣬ÿ������ֻ����һ��CCommLayer����Ķ���
class CCommLayer
{
public:
    CCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CCommLayer();

    //��ʼ������
    virtual int Init(void);

    //��ɱ�Ҫ��������
    void ExitInstance(void);

    //���ݽ��̵ı�Ż����Ӧ��ͨ�Ų����
    static CCommLayer* const Instance(unsigned long const nProcessID =
                                        PT_UNKNOWN);

    //���ٶ���
    static void Destroy();

    //ΪӦ������ΪuAppType�Ķ˿�����һ��������
    int AddFrmSocket(CFrmSocket* pFrmSocket, unsigned long uAppType);

    //ע��CSocketImpl����Ķ���
    int RegisterHandle(CSocketImpl* pSocket, ACE_Reactor_Mask mask);

    //������Ϣ
    int Send(const MSG_INTRA* const pMsg);

    //�ύ�ӵͲ����·�Ͻ�����������Ϣ
    virtual void OnReceive(MSG_INTRA* pMsg);

    //���¼�������
    virtual void Timer1Sec(void);

protected:

    //CCommLayer��֧��Singleton���ģʽ����Ҫһ����̬��Ա���浥ʵ��
    //�����ָ��
    static CCommLayer* m_pCommLayer;

    //�ó�Ա������ʾ���������ڽ��̵ı��
    PROCESS_TYPE m_nProcessID;

    //�������е�CCommRouter����
    CCommRouterMan *m_pCommRouterMan;

    //��ָ��ָ��Ķ���������е�CCommTask����
    CCommTaskMan* m_pCommTaskMan;
};

//������֧��TCP/IP�������˵�ͨ�Ų�
class CTcpSvrCommLayer:public CCommLayer
{
public:
    CTcpSvrCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CTcpSvrCommLayer();

protected:

    typedef LIST <CCommListener*> LIST_PCOMMLISTENER;

    LIST_PCOMMLISTENER m_CommListenerList;
};

//������֧��GTP��Э��Ľ������̵�ͨ�Ų�
class CGtpCommLayer:public CTcpSvrCommLayer
{
public:
    CGtpCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CGtpCommLayer();

    //��ʼ������
    virtual int Init(void);
};

//������֧�ֻ�������Э��Ľ��������е�ͨ�Ų�
class CSlideWndCommLayer:public CTcpSvrCommLayer
{
public:
    CSlideWndCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CSlideWndCommLayer();

    //��ʼ������
    virtual int Init(void);

protected:
};


//����Զ2002.04.08���
class CSoftXCommLayer : public CTcpSvrCommLayer
{
public:
    CSoftXCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CSoftXCommLayer();

    //��ʼ������
    virtual int Init(void);
};
//����Զ2002.04.08��ӽ���


//������KERNEL�����е�ͨ�Ų�
class CMsgSvrCommLayer:public CTcpSvrCommLayer
{
public:
    CMsgSvrCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CMsgSvrCommLayer();

    //��ʼ������
    virtual int Init(void);
};

//CMmlCommLayer������CTcpSvrCommLayer,�����˲���ά�����̵�ͨ�Ų�
class CMmlCommLayer:public CTcpSvrCommLayer
{
public:
    CMmlCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CMmlCommLayer();

    //��ʼ������
    virtual int Init(void);

    //�ύ�ӵͲ����·�Ͻ�����������Ϣ
    virtual void OnReceive(MSG_INTRA* pMsg);
};

//CClstrCommLayer������CTcpSvrCommLayer,������˫��ά�����̵�ͨ�Ų�
class CClstrCommLayer:public CTcpSvrCommLayer
{
public:
    CClstrCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CClstrCommLayer();

    //��ʼ������
    virtual int Init(void);
};

//������CTcpSvrCommLayer,������Զ�����ý��̵�ͨ�Ų�
class CCfgSvrCommLayer:public CTcpSvrCommLayer
{
public:
    CCfgSvrCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CCfgSvrCommLayer();

    //��ʼ������
    virtual int Init(void);
};

//������CTcpSvrCommLayer,������Զ�����ý��̵�ͨ�Ų�
class CMeterAccCommLayer:public CTcpSvrCommLayer
{
public:
    CMeterAccCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CMeterAccCommLayer();

    //��ʼ������
    virtual int Init(void);
};


///////////////////////////////////////////////////////////////////////////////
//GTP'�Ʒѽӿ����� Z30971 2004/06/10

//������CCommlayer, ����Ʒѽӿڽ��̵�ͨ�Ų�
class CBSCommlayer:public CCommLayer
{
public:
    CBSCommlayer(PROCESS_TYPE const nProcessID);
    virtual ~CBSCommlayer();

    //��ʼ������
    virtual int Init(void);
};

#endif //__CCOMMLAYER
