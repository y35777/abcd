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

    //开始在指定的IP地址和端口上侦听
    int Open(const char* const szIPAddr, const int nPort, const BOOL bReuse = 0);
protected:

    //当CCommListener对象被注册到ACE_Reactor对象后，ACE_Reactor对象
    //会回调CCommListener对象的get_handle函数,从而获得产生SOCKET事件
    //的句柄
    virtual ACE_HANDLE get_handle(void) const;

    //处理SOCKET的输入事件
    virtual int SOCK_handle_input(void);

    //接收连接并根据m_uAppType的取值创建一个相应CFrmSocket子类的对象
    virtual CFrmSocket* CreateFrmSocket(void);

    //一个CCommListener对象对应一种应用类型
    unsigned long m_uAppType;

    //ACE提供的ACE_SOCK_Acceptor抽象了侦听SOCKET
    ACE_SOCK_Acceptor m_Acceptor;

    //该指针指向作为通信层的CCommLayer对象
    CCommLayer* m_pCommLayer;

};

class CCommTaskMan;
class CCommTask:public ACE_Task_Base
{
public:
    CCommTask(CCommTaskMan* pCommTaskMan);
    virtual ~CCommTask();

    //本函数会启动一个用于检测Reactor事件的线程
    virtual int open(void* args = 0);

    //线程函数,本函数是提供一个Reactor的事件循环
    virtual int svc(void);

    //将CSocketImpl子类的对象注册到本对象的ACE_Reactor对象中
    virtual int RegisterHandle(CSocketImpl* pSocket, ACE_Reactor_Mask mask);

    //结束线程
    void QuitComTask(void);

    //将pSocket指针列入待删除列表
    void ScheduleRemove(CSocketImpl* pSocket);

    //将pSocket指针列入待注册列表
    int ScheduleRegister(CSocketImpl* pSocket, ACE_Reactor_Mask mask);

    //将pSocket指针列入待注销output事件的列表
    void ScheduleWakeup(CSocketImpl* pSocket);

protected:

    //删除待删除列表中的Socket对象
    void DelScheduleSocket();

    //注册待注册列表中的Socket对象
    void RegScheduleSocket();

    //注销m_SocketWillRmOptHdl中Socket对象的output事件
    void SleepSocketOutput();

    ACE_Reactor m_Reactor;

    //需要删除的Socket指针列表
    typedef LIST <CSocketImpl*> LIST_SOCKET;
    LIST_SOCKET m_SocketWillBeDel;

    //需要注册的Socket指针列表
    typedef MAP <CSocketImpl*, ACE_Reactor_Mask> MAP_SOCKET_TO_MASK;
    MAP_SOCKET_TO_MASK m_SocketWillReg;

    //需要注销output事件的Socket指针列表
    LIST_SOCKET m_SocketWillRmOptHdl;

    //Reactor是否已满
    BOOL m_bReactorFull;

    CCommTaskMan* m_pCommTaskMan;
};

class CCommTaskMan
{
public:
    CCommTaskMan(CCommLayer* pCommLayer);
    virtual ~CCommTaskMan();

    //初始化函数
    int Init(void);

    //注册CSocketImpl子类的对象
    int RegisterHandle(CSocketImpl* pSocket, ACE_Reactor_Mask mask);

    //激活所有CCommTask对象
    int Active();

    //去激活所有CCommTask对象
    void DeActive();

    CCommLayer* GetCommLayer();

protected:

    typedef LIST <CCommTask*> LIST_PCOMMTASK;

    //被管理的CCommTask对象指针的链表
    LIST_PCOMMTASK m_CommTaskList;

    CCommLayer* m_pCommLayer;
};



//CCommLayer抽象了通信层，每个进程只包含一个CCommLayer子类的对象
class CCommLayer
{
public:
    CCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CCommLayer();

    //初始化函数
    virtual int Init(void);

    //完成必要的清理工作
    void ExitInstance(void);

    //根据进程的编号获得相应的通信层对象
    static CCommLayer* const Instance(unsigned long const nProcessID =
                                        PT_UNKNOWN);

    //销毁对象
    static void Destroy();

    //为应用类型为uAppType的端口增加一个新连接
    int AddFrmSocket(CFrmSocket* pFrmSocket, unsigned long uAppType);

    //注册CSocketImpl子类的对象
    int RegisterHandle(CSocketImpl* pSocket, ACE_Reactor_Mask mask);

    //发送消息
    int Send(const MSG_INTRA* const pMsg);

    //提交从低层的链路上接收上来的消息
    virtual void OnReceive(MSG_INTRA* pMsg);

    //秒事件处理函数
    virtual void Timer1Sec(void);

protected:

    //CCommLayer类支持Singleton设计模式，需要一个静态成员保存单实例
    //对象的指针
    static CCommLayer* m_pCommLayer;

    //该成员变量表示本对象所在进程的编号
    PROCESS_TYPE m_nProcessID;

    //管理所有的CCommRouter对象
    CCommRouterMan *m_pCommRouterMan;

    //该指针指向的对象管理所有的CCommTask对象
    CCommTaskMan* m_pCommTaskMan;
};

//抽象了支持TCP/IP服务器端的通信层
class CTcpSvrCommLayer:public CCommLayer
{
public:
    CTcpSvrCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CTcpSvrCommLayer();

protected:

    typedef LIST <CCommListener*> LIST_PCOMMLISTENER;

    LIST_PCOMMLISTENER m_CommListenerList;
};

//抽象了支持GTP’协议的接入点进程的通信层
class CGtpCommLayer:public CTcpSvrCommLayer
{
public:
    CGtpCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CGtpCommLayer();

    //初始化函数
    virtual int Init(void);
};

//抽象了支持滑动窗口协议的接入点进程中的通信层
class CSlideWndCommLayer:public CTcpSvrCommLayer
{
public:
    CSlideWndCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CSlideWndCommLayer();

    //初始化函数
    virtual int Init(void);

protected:
};


//颜文远2002.04.08添加
class CSoftXCommLayer : public CTcpSvrCommLayer
{
public:
    CSoftXCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CSoftXCommLayer();

    //初始化函数
    virtual int Init(void);
};
//颜文远2002.04.08添加结束


//抽象了KERNEL进程中的通信层
class CMsgSvrCommLayer:public CTcpSvrCommLayer
{
public:
    CMsgSvrCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CMsgSvrCommLayer();

    //初始化函数
    virtual int Init(void);
};

//CMmlCommLayer派生于CTcpSvrCommLayer,抽象了操作维护进程的通信层
class CMmlCommLayer:public CTcpSvrCommLayer
{
public:
    CMmlCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CMmlCommLayer();

    //初始化函数
    virtual int Init(void);

    //提交从低层的链路上接收上来的消息
    virtual void OnReceive(MSG_INTRA* pMsg);
};

//CClstrCommLayer派生于CTcpSvrCommLayer,抽象了双机维护进程的通信层
class CClstrCommLayer:public CTcpSvrCommLayer
{
public:
    CClstrCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CClstrCommLayer();

    //初始化函数
    virtual int Init(void);
};

//派生于CTcpSvrCommLayer,抽象了远程配置进程的通信层
class CCfgSvrCommLayer:public CTcpSvrCommLayer
{
public:
    CCfgSvrCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CCfgSvrCommLayer();

    //初始化函数
    virtual int Init(void);
};

//派生于CTcpSvrCommLayer,抽象了远程配置进程的通信层
class CMeterAccCommLayer:public CTcpSvrCommLayer
{
public:
    CMeterAccCommLayer(PROCESS_TYPE const nProcessID);
    virtual ~CMeterAccCommLayer();

    //初始化函数
    virtual int Init(void);
};


///////////////////////////////////////////////////////////////////////////////
//GTP'计费接口增加 Z30971 2004/06/10

//派生于CCommlayer, 抽象计费接口进程的通信层
class CBSCommlayer:public CCommLayer
{
public:
    CBSCommlayer(PROCESS_TYPE const nProcessID);
    virtual ~CBSCommlayer();

    //初始化函数
    virtual int Init(void);
};

#endif //__CCOMMLAYER
