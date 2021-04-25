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


//通信层是多线程的。为了避免这些线程间的访问冲突，
//需要引入一个互斥量，从而使得这些线程以串行的方
//式运行。CCommMutex抽象了该互斥量
class CCommMutex
{
public:
    //获得CCommMutex单实例对象指针
    static CCommMutex* const Instance();

    //销毁CCommMutex单实例对象
    static void Destroy();

    //申请获得互斥量的使用权
    void Acquire();

    //释放互斥量的使用权
    void Release();

protected:
    CCommMutex();
    ~CCommMutex();

    //由于CCommMutex只支持一个实例化对象，
    //所以该静态指针指向该对象
    static CCommMutex* g_pTheMutex;

    //ACE提供的递归线程锁
    ACE_Recursive_Thread_Mutex m_AceMutex;
};
//IP地址过滤类
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

    //上一次接收数据到当前时刻的时间差，单位为s
    unsigned long m_lTimeNoData;

    //从ACE_Reactor中注销事件
    void RemoveHandler();

    //当ACE_Reactor对象发现某个CSocketImpl对象上有SOCKET输入事件，
    //则调用该对象的本函数
    virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);

    //当ACE_Reactor对象发现某个CSocketImpl对象上有SOCKET输出事件，
    //调用该对象的本函数
    virtual int handle_output(ACE_HANDLE fd = ACE_INVALID_HANDLE);

    //当ACE_Reactor对象发现某个CSocketImpl对象上有SOCKET关闭事件，
    //则调用该对象的本函数
    virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);

protected:

    //向子类提供实际处理SOCKET上的输入事件的虚函数接口
    virtual int SOCK_handle_input(void) = 0;

    //向子类提供实际处理SOCKET上的输出事件的虚函数接口
    virtual int SOCK_handle_output(void);

    //向子类提供实际处理SOCKET上的关闭事件的虚函数接口
    virtual int SOCK_handle_close(void);

    friend class CCommTask;
    //指向产生事件的Task对象
    CCommTask* m_pCommTask;

    //是否是故障状态
    BOOL m_bFailure;
};


class CCommRouter;
//以数据帧为单位在SOCKET上收发流数据的基类
class CFrmSocket: public CSocketImpl
{
public:
    CFrmSocket(ACE_SOCK_Stream* pSocketStream);
    virtual ~CFrmSocket();

    //发送数据帧
    virtual int Send(const void* const pAFrame);

    //上层CComRouter对象的指针
    CCommRouter* m_pCommRouter;

    //获得对端地址
    ACE_INET_Addr& GetAddrRemote();

    //ACE_SOCK_Stream 封装了以流方式接收数据的SOCKET
    ACE_SOCK_Stream* GetSockStream();

    //获得本地地址
    inline void MakeLocalAddress()
    {
        m_pSocketStream->get_local_addr(m_addrLocal);
    }

protected:

    //完成清理工作
    void Clear(void);

    //ACE_Reactor获得产生SOCKET事件句柄的回调函数
    virtual ACE_HANDLE get_handle(void) const;

    //处理SOCKET输入事件
    virtual int SOCK_handle_input(void);

    //处理SOCKET输出事件
    virtual int SOCK_handle_output(void);

    //处理SOCKET关闭事件
    virtual int SOCK_handle_close(void);

    //从接收缓冲区中提取数据帧，并将提取出的数据帧的指针插入到
    //接收队列的队尾
    virtual void ReceiveFrames(void) = 0;

    //向上层提交一帧数据
    virtual void OnReceiveFrame(void* pAFrame);

    //以FIFO方式将发送队列中的数据帧发送出去，直到SOCKET被阻塞
    virtual int ProcessPendingWrite(void);

    //获得一帧数据的长度
    virtual long GetFrameSize(const char* const pAFrame) = 0;

    //本端的IP地址和端口
    ACE_INET_Addr m_addrLocal;

    //对端的IP地址和端口
    ACE_INET_Addr m_addrRemote;

    //接收缓冲区
    char* m_pRecvBuf;

    //接收缓冲区的长度
    long m_lRecvBufSize;

    //每次从SOCKET接收数据时，实际所接收的数据的总长
    long m_lBytesReceived;

    //接收队列
    LIST <char*> m_InFrames;

    //若接收队列队尾元素指向的数据帧已经被完整地接收，
    //则m_bLastOK 等于TRUE；否则等于FALSE
    BOOL m_bLastOK;

    //该成员变量表示接收队列队尾元素指向的数据帧的实际长度
    long m_lLastFrameLen;

    //发送队列
    LIST <char*> m_OutFrames;

    //该成员变量表示发送队列的最大长度
    long m_lOutFramesMax;

    //该指针指向当前正在发送的数据帧
    char* m_pSendFrame;

    //m_pSendFrame指向的数据帧已经被发送的字节数
    long m_lBytesSent;

    //ACE_SOCK_Stream封装了以流方式接收数据的SOCKET
    ACE_SOCK_Stream* m_pSocketStream;

};

//以二进制数据帧为单位在SOCKET上收发数据的SOCKET的基类
class CBinFrmSocket:public CFrmSocket
{
public:
    CBinFrmSocket(ACE_SOCK_Stream* pSocketStream);
    virtual ~CBinFrmSocket();

protected:

    //从接收缓冲区中提取二进制帧，并将所提取出来的二进制帧的指针
    //插入到接收队列的队尾
    virtual void ReceiveFrames(void);

    //获取数据帧的长度
    virtual long GetFrameSize(const char* const pAFrame);

    //获得一帧数据的数据区的长度
    virtual long GetDataSize(const char* const pAFrame) = 0;

    //帧头的长度
    long m_lHeadLen;

    //该缓冲区用于临时存放当前正在接收的二进制帧的帧头数据
    char* m_pFrameHead;
};

class CMemFrmSocket:public CBinFrmSocket
{
public:

    CMemFrmSocket(ACE_SOCK_Stream* pSocketStream);
    virtual ~CMemFrmSocket();

    //获得是否是主端口的信息
    BOOL GetbPrimary();

    //设置是否是主端口的信息
    void SetbPrimary(BOOL bPrimary);

    //获得主机模块号
    int GetModuleNum();

    //设置主机模块号
    void SetModuleNum(int nNum);

protected:

    //获得一帧SMemFrame实例的数据区的长度
    virtual long GetDataSize(const char* const pAFrame);

    //向管理本对象的CSlideWndCommRouter对象提交数据
    virtual void OnReceiveFrame(void* pAFrame);

    //是否是主端口
    BOOL m_bPrimary;

    //该成员变量表示本对象所属的主机模块号
    int m_nModuleNum;
};

//颜文远2002.04.08添加
class CSysFrmSocket:public CMemFrmSocket
{
public:

    CSysFrmSocket(ACE_SOCK_Stream* pSocketStream);
    virtual ~CSysFrmSocket();

protected:

    //获得一帧SSysFrame实例的数据区的长度
    virtual long GetDataSize(const char* const pAFrame);

    //向管理本对象的CSoftXCommRouter对象提交数据
    virtual void OnReceiveFrame(void* pAFrame);

};
//颜文远2002.04.08添加结束

class CMsgFrmSocket:public CBinFrmSocket
{
public:

    CMsgFrmSocket(ACE_SOCK_Stream* pSocketStream);
    virtual ~CMsgFrmSocket();

protected:

    //获得一帧MSG_INTRA实例的数据区的长度
    virtual long GetDataSize(const char* const pAFrame);

    //用于向管理本对象的CMsgSvrCommRouter对象或
    //CMsgCliCommRouter对象提交数据
    virtual void OnReceiveFrame(void* pAFrame);
};

class CMmlFrmSocket:public CFrmSocket
{
public:
	CMmlFrmSocket(ACE_SOCK_Stream* pSocketStream);
    virtual ~CMmlFrmSocket();

    //增加一个文本帧开始或结束的标志
    void AddTags(const char yTag, const BOOL bBeginTag);

    //发送MML文件数据帧
    virtual int Send(const void* const pAFrame);

protected:

    //本函数从接收缓冲区中提取文本帧，并将所提取出来的文
    //本帧的指针插入到接收队列的队尾
    virtual void ReceiveFrames(void);

    //获得数据帧长度
    virtual long GetFrameSize(const char* const pAFrame);

    //向管理本对象的CMmlCommRouter对象提交数据
    void OnReceiveFrame(void* pAFrame);

    //该成员变量是一个集合，集合中的元素为文本帧结束的标志
    SET <char> m_yEndTags;

    //该成员变量是一个集合，集合中的元素为文本帧开始的标志
    SET <char> m_yBeginTags;
};

class CGtpUdpCommRouter;

class CGtpUdpSocket:public CSocketImpl
{
public:

    #define GTP_INPUT_BUFFER_MAXLENGTH (64*1024)

    CGtpUdpSocket(CGtpUdpCommRouter* const pGtpCommRouter);
    virtual ~CGtpUdpSocket();

    //打开一个收发UDP数据报的端口
    int Open(const char* const szIPAddr, const int nPort);

    //获得本对象对应的IP地址和端口号
    void GetAddr(unsigned int& nIPAddr, int& nPort);

    //发送一个UDP数据报
    int Send(const MSG_INTRA* const pMsg);

    //指向管理本对象的CGtpUdpCommRouter对象
    CGtpUdpCommRouter *m_pGtpCommRouter;

protected:

    //当CGtpUdpSocket 对象被注册到ACE_Reactor对象后,ACE_Reactor
    //对象会回调CGtpUdpSocket对象的get_handle函数，从而获得产生
    //SOCKET事件的句柄
    virtual ACE_HANDLE get_handle(void) const;

    //处理SOCKET输入事件
    virtual int SOCK_handle_input(void);

    //处理SOCKET关闭事件
    virtual int SOCK_handle_close(void);

    //本端IP地址和端口
    ACE_INET_Addr m_addrLocal;

    //用于接收UDP数据报的缓冲区,该缓冲区的长度为64KB
    char* m_pRecvBuf;

    //ACE提供的ACE_SOCK_Dgram可用于收发UDP数据报
    ACE_SOCK_Dgram m_SocketDgram;
};

//iGWB与convert连接的socket
class CABFrmSocket: public CBinFrmSocket
{
public:
    CABFrmSocket(ACE_SOCK_Stream* pSocketStream);
    virtual ~CABFrmSocket();

protected:

    //获得一帧数据的数据区的长度
    virtual long GetDataSize(const char* const pAFrame);
    
	//向管理本对象的CAlarmBoxRouter对象提交数据
    void OnReceiveFrame(void* pAFrame);
};

class CGtpFrmSocket:public CBinFrmSocket
{
public:

    CGtpFrmSocket(ACE_SOCK_Stream* pSocketStream);
    virtual ~CGtpFrmSocket();

protected:

    //获得一帧SGtpFrame实例的数据区的长度
    virtual long GetDataSize(const char* const pAFrame);

    //向管理本对象的CGtpFrmCommRouter对象提交数据
    virtual void OnReceiveFrame(void* pAFrame);

    //处理SOCKET关闭事件
    virtual int SOCK_handle_close(void);

};

///////////////////////////////////////////////////////////////////////////////
//GTP'计费接口增加 Z30971 2004/06/10

class CGTPUdpCliCommRouter;
class CGtpUdpCliSocket: public CGtpUdpSocket
{
public:
    CGtpUdpCliSocket(CGTPUdpCliCommRouter* const pGtpCliCommRouter);
    virtual ~CGtpUdpCliSocket();
    
    virtual int Send(const MSG_INTRA* const pMsg);

    
    //指向管理本对象的CGtpUdpCliCommRouter对象
    CGTPUdpCliCommRouter *m_pCommRouter;

protected:
    //处理SOCKET输入事件
    virtual int SOCK_handle_input(void);
    //处理SOCKET关闭事件
    virtual int SOCK_handle_close(void);

};

class CGtpTcpCliSocket: public CGtpFrmSocket
{
public:
    CGtpTcpCliSocket(ACE_SOCK_Stream* pSocketStream);
    virtual ~CGtpTcpCliSocket();

protected:
    //向管理本对象的CGtpFrmCommRouter对象提交数据
    virtual void OnReceiveFrame(void* pAFrame);
};
#endif //__CSOCKETS_H_
