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

    //根据*pMsg帧头中的相关信息，选择适当的链路将*pMsg的Data域的
    //数据发送出去
    virtual int Send(const MSG_INTRA* const pMsg) = 0;

    //向CCommRouterMan对象提交从低层的链路上接收上来的消息
    virtual void OnReceive(MSG_INTRA* pMsg);

    //处理来自*pFrmSocket对象接收到的数据帧
    virtual void OnReceiveEx(CFrmSocket* pFrmSocket, void* pFrame);

    //为本端口增加一个SOCKET连接
    virtual int AddFrmSocket(CFrmSocket* pFrmSocket);

    //为本端口注销一个SOCKET连接
    virtual void DelFrmSocket(CFrmSocket* pFrmSocket);

    //秒事件处理函数
    virtual void Timer1Sec(void);

    //注册CSocketImpl子类的对象
    virtual int RegisterHandle(CSocketImpl* pSocket, ACE_Reactor_Mask mask);

protected:

    //用于集中管理所有的CCommRouter对象
    CCommRouterMan* m_pCommRouterMan;

    //对应的应用类型
    unsigned long m_uAppType;

};

class CCommRouterMan
{
public:
    CCommRouterMan(CCommLayer* const pCommLayer);
    virtual ~CCommRouterMan();

    //注册CCommRouter子类的对象
    int RegisterCommRouter(CCommRouter* pCommRouter,
                            unsigned long uAppType);

    //为应用类型为uAppType的端口增加一个新连接
    int AddFrmSocket(CFrmSocket* pFrmSocket,
                    unsigned long uAppType);

    //根据*pMsg帧头中的相关信息，选择适当的CCommRouter对象，
    //并调用该对象的Send函数
    int Send(const MSG_INTRA* const pMsg);

    //向作为通信层的CCommLayer对象提交下层的CCommRouter子类
    //对象提交上来的消息
    void OnReceive(MSG_INTRA* pMsg);

    //秒定时器事件
    void Timer1Sec(void);

    //注册CSocketImpl子类的对象
    int RegisterHandle(CSocketImpl* pSocket, ACE_Reactor_Mask mask);

protected:

    //该指针指向代表消息中转的CCommRouter对象
    CCommRouter* m_pMsgCommRouter;

    typedef MAP<unsigned long, CCommRouter*> MAP_APT_TO_PCOMMROUTER;

    //被管理的CCommRouter对象指针的索引
    MAP_APT_TO_PCOMMROUTER m_CommRouterMap;

    //该指针指向作为通信层的CCommLayer子类的对象
    CCommLayer* m_pCommLayer;
};

//对GTP’协议UDP方式的服务器端的抽象
class CGtpUdpCommRouter:public CCommRouter
{
public:
    CGtpUdpCommRouter(CCommRouterMan* const pCommRouterMan,
                        const unsigned long uAppType);
    virtual ~CGtpUdpCommRouter();

    //根据配置创建、初始化相应的CGtpUdpSocket对象，并将这些对象
    //注册到CCommTaskMan对象中
    int Init(unsigned char const nAPID);

    //根据*pMsg帧头中的相关信息，选择适当的CGtpUdpSocket对象将
    //*pMsg的Data域的数据发送出去
    virtual int Send(const MSG_INTRA* const pMsg);

    //为本端口删除一个SOCKET连接；被管理的CGtpUdpSocket子类对象的
    //SOCK_handle_close函数会调用本函数
    virtual void DelGtpSocket(CGtpUdpSocket* pGtpSocket);

protected:

    typedef LIST <CGtpUdpSocket*> LIST_PGTPSOCKET;
  
    //被管理的GTPSOCKET对象指针的链表
    LIST_PGTPSOCKET m_SocketList;
};

//对GTP’协议TCP方式的服务器端的抽象
class CGtpFrmCommRouter:public CCommRouter
{
public:
    CGtpFrmCommRouter(CCommRouterMan* const pCommRouterMan,
                        const unsigned long uAppType);
    virtual ~CGtpFrmCommRouter();

    //为本端口增加一个SOCKET连接
    virtual int AddFrmSocket(CFrmSocket* pFrmSocket);

    //初始化m_GsnMultiIPMap及m_GsnMultiIPTable
    int Init(unsigned char const nAPID);

    //根据*pMsg帧头中的相关信息，选择适当的CGtpFrmSocket对象将
    //*pMsg的Data域的数据发送出去
    virtual int Send(const MSG_INTRA* const pMsg);

    //为本端口删除一个SOCKET连接；被管理的CGtpFrmSocket子类对象的
    //SOCK_handle_close函数会调用本函数
    virtual void DelGtpSocket(CSocketImpl* pGtpSocket);

protected:

    //被管理的TCP方式的CGtpFrmSocket对象指针的MAP,索引为对端IP地址
    MAP<UINT4, CGtpFrmSocket*>  m_GtpFrmSocketMap;

    //各IP地址对应于m_GsnMultiIPTable中GSN的索引号
    MAP<UINT4, int>        m_GsnMultiIPMap;

    //所有多地址GSN的IP地址表
    VECTOR<VECTOR<UINT4> > m_GsnMultiIPTable;

};

class CMscLinkPair
{
public:
    CMscLinkPair();
    virtual ~CMscLinkPair();

    //设置代表主连接的CMemFrmSocket对象
    void SetPrimaryLink(CMemFrmSocket* pLink);

    //设置代表备连接的CMemFrmSocket对象
    void SetSecondaryLink(CMemFrmSocket* pLink);

    //获得代表主连接的CMemFrmSocket对象
    CMemFrmSocket* const GetPrimaryLink(void);

    //获得代表备连接的CMemFrmSocket对象
    CMemFrmSocket* const GetSecondaryLink(void);

protected:

    //该指针指向的对象代表主机模块与计费网关之间的主连接
    CMemFrmSocket* m_pPrimaryLink;

    //该指针指向的对象代表主机模块与计费网关之间的备连接
    CMemFrmSocket* m_pSecondaryLink;
};

class CLinkPairs
{
public:
    CLinkPairs();
    virtual ~CLinkPairs();

    //处理*pLink对应的主机模块号的改变
    void OnLinkModuleNumberChanged(CFrmSocket* const pLink,
                                    int const nOldModNum,
                                    int const nNewModNum);

    //根据主机模块号获得对应的CMscLinkPair对象的指针
    CMscLinkPair* const GetLinkPairByModuleNumber(int const nModuleNumber);

    //为本端口删除一个SOCKET连接
    void DelFrmSocket(CFrmSocket* pFrmSocket);

	//颜文远2002.04.08添加
    //根据pMsg帧中的相关信息，选择适当的链路将*pMsg的Data域
    //的数据发送出去
    virtual int Send(const MSG_INTRA* const pMsg);

    //秒事件处理函数
    void Timer1Sec(void);

	//设置心跳超时限制
	void SetLinkTimeOut(int nLinkTimeOut);

protected:

    typedef MAP <int, CMscLinkPair*> MAP_MODNUM_TO_PCMSCLINKPAIR;

    //以主机模块号为键值的CMscLinkPair指针MAP
    MAP_MODNUM_TO_PCMSCLINKPAIR m_MapLinks;

	//颜文远2002.04.08添加
	//socket的超时限制，以秒为单位
	int	m_nLinkTimeOut;
	//颜文远2002.04.08添加结束
};

class CSoftXLinkPairs
{
public:
    CSoftXLinkPairs();
    virtual ~CSoftXLinkPairs();

    //根据主机模块号获得对应的CMscLinkPair对象的指针
    CMscLinkPair* const GetLinkPairByModuleNumber(int const nModuleNumber);

    //根据pMsg帧中的相关信息，选择适当的链路将*pMsg的Data域
    //的数据发送出去
    virtual int Send(const MSG_INTRA* const pMsg);

    //为本端口删除一个SOCKET连接
    virtual void DelFrmSocket(CFrmSocket* pFrmSocket);

    //注册模块号
    int RegisterModule(CFrmSocket* pLink, int nModNum);

protected:
    typedef MAP <int, CMscLinkPair*> MAP_MODNUM_TO_PCMSCLINKPAIR;

    //以主机模块号为键值的CMscLinkPair指针MAP
    MAP_MODNUM_TO_PCMSCLINKPAIR m_MapLinks;
};

//CSlideWndCommRouter抽象了滑动窗口协议服务器端的端口（包含主备两个端口）
class CSlideWndCommRouter : public CCommRouter
{
public:
    CSlideWndCommRouter(CCommRouterMan* const pCommRouterMan,
                        const unsigned long uAppType);
    virtual ~CSlideWndCommRouter();

    //为本端口增加一个SOCKET连接
    virtual int AddFrmSocket(CFrmSocket* pFrmSocket);

    //为本端口删除一个SOCKET连接
    virtual void DelFrmSocket(CFrmSocket* pFrmSocket);

    //处理来自*pFrmSocket对象接收到的数据帧
    virtual void OnReceiveEx(CFrmSocket* pFrmSocket,
                             void* pFrame);

    //根据*pMsg帧头中的相关信息，选择适当的链路将*pMsg的Data域
    //的数据发送出去
    virtual int Send(const MSG_INTRA* const pMsg);

    //秒事件处理函数
    virtual void Timer1Sec(void);

	//颜文远2002.04.08添加
	//设置心跳超时限制
	void SetLinkTimeOut(int nLinkTimeOut);
	//颜文远2002.04.08添加结束


protected:

    //代表所接入的MSC主机节点与计费网关的所有连接
    CLinkPairs m_LinkPairs;

    typedef MAP <CMemFrmSocket*, int>  MAP_PCMEMFRMSOCKET_TO_INT;

    //未与主机号关联的Socket连接的MAP
    MAP_PCMEMFRMSOCKET_TO_INT m_SlideWndSocketMap;
};


//颜文远2002.04.08添加
class CSoftXCommRouter : public CCommRouter
{
public:
    CSoftXCommRouter(CCommRouterMan* const pCommRouterMan,
                        const unsigned long uAppType);
    virtual ~CSoftXCommRouter();

    //为本端口增加一个SOCKET连接
    virtual int AddFrmSocket(CFrmSocket* pFrmSocket);

    //为本端口删除一个SOCKET连接
    virtual void DelFrmSocket(CFrmSocket* pFrmSocket);

    //处理来自*pFrmSocket对象接收到的数据帧
    virtual void OnReceiveEx(CFrmSocket* pFrmSocket,
                             void* pFrame);

    //根据*pMsg帧头中的相关信息，选择适当的链路将*pMsg的Data域
    //的数据发送出去
    virtual int Send(const MSG_INTRA* const pMsg);

    //秒事件处理函数
    virtual void Timer1Sec(void);

	//设置心跳超时限制
	void SetLinkTimeOut(int nLinkTimeOut);


protected:
    //代表所接入的MSC主机节点与计费网关的所有连接
    CSoftXLinkPairs m_LinkPairs;

	//所有的Socket对象指针数组
	VECTOR<CSysFrmSocket* >		m_AllFrmSockets;

	//socket的超时限制，以秒为单位
	int	m_nLinkTimeOut;
};
//颜文远2002.04.08添加结束


class CMsgSvrCommRouter:public CCommRouter
{
public:
    CMsgSvrCommRouter(CCommRouterMan* const pCommRouterMan,
                        const unsigned long uAppType);
    virtual ~CMsgSvrCommRouter();

    //为本端口增加一个SOCKET连接
    virtual int AddFrmSocket(CFrmSocket* pFrmSocket);

    //为本端口删除一个SOCKET连接
    virtual void DelFrmSocket(CFrmSocket* pFrmSocket);

    //处理来自*pFrmSocket对象接收到的数据帧
    virtual void OnReceiveEx(CFrmSocket* pFrmSocket,
                             void* pFrame);

    //根据*pMsg帧头中的相关信息，选择适当的链路将*pMsg的Data域
    //的数据发送出去
    virtual int Send(const MSG_INTRA* const pMsg);

protected:

    typedef MAP <int,CMsgFrmSocket*> MAP_PT_TO_PMSGFRMSOCKET;

    //以进程编号为键值，被索引值为CMsgFrmSocket对象的指针
    MAP_PT_TO_PMSGFRMSOCKET m_SocketMap;

    typedef LIST <CMsgFrmSocket*> LIST_PMSGFRMSOCKET;

    //本链表中的元素所指向的CMsgFrmSocket对象暂时没有与进程编号绑定
    LIST_PMSGFRMSOCKET m_SocketList;
};

//抽象了计费网关中除KERNEL进程之外其他进程的消息中转的客户端
class CMsgCliCommRouter:public CCommRouter
{
public:
    CMsgCliCommRouter(CCommRouterMan* const pCommRouterMan,
                        const unsigned long uAppType,
                        PROCESS_TYPE const nProcessType);
    virtual ~CMsgCliCommRouter();

    //为本端口删除一个SOCKET连接
    virtual void DelFrmSocket(CFrmSocket* pFrmSocket);

    //秒事件处理函数
    virtual void Timer1Sec(void);

    //发送pMsg指向的数据帧
    virtual int Send(const MSG_INTRA* const pMsg);

protected:

    //与Kernel进程进行连接
    int Connect(void);

    //该指针指向的对象表示所在进程与KERNEL进程的消息中转服务器
    //端之间的连接
    CMsgFrmSocket* m_pMsgFrmSocket;

    //该成员变量表示本对象所在进程的进程编号
    PROCESS_TYPE m_nProcessType;

    //从上次没有成功连上KERNEL进程的消息中转服务器端开始所经历
    //的时间，单位为S
    int m_nReConnectTimer;

};

//抽象了操作维护进程通信层中的MML端口
class CMmlCommRouter:public CCommRouter
{
public:
    enum
    {
        MIN_CLIENT_NO = 1,      //最小客户端号
        MAX_CLIENT_NO = 255     //最大客户端号
    };

    CMmlCommRouter(CCommRouterMan* const pCommRouterMan,
                        const unsigned long uAppType);
    virtual ~CMmlCommRouter();

    //为本端口增加一个SOCKET连接
    virtual int AddFrmSocket(CFrmSocket* pFrmSocket);

    //为本端口删除一个SOCKET连接
    virtual void DelFrmSocket(CFrmSocket* pFrmSocket);

    //秒事件处理函数
    virtual void Timer1Sec(void);

    //处理来自*pFrmSocket对象接收到的数据帧
    void OnReceiveEx(CFrmSocket* pFrmSocket, void* pFrame);

    //发送pMsg指向的数据帧
    virtual int Send(const MSG_INTRA* const pMsg);

protected:
    void SendMsgToMMLServer(const char *szCmd, UINT2 nClientNo, UINT4 uIp);

    typedef MAP <unsigned long, CMmlFrmSocket*> MAP_CN_TO_PCMMLFRMSOCKET;

    //该MAP的元素为CMmlFrmSocket对象的指针，一个
    //CMmlFrmSocket对象表示一个MML客户端与计费网
    //关之间的连接，每个连接都具有一个唯一的编号
    MAP_CN_TO_PCMMLFRMSOCKET m_SocketMap;
};

//抽象了计费网关OM进程与convert之间的链路管理
class CAlarmBoxRouter:public CCommRouter
{
public:
    CAlarmBoxRouter(CCommRouterMan* const pCommRouterMan,
                        const unsigned long uAppType);
    virtual ~CAlarmBoxRouter();

    //为本端口增加一个SOCKET连接
    virtual int AddFrmSocket(CFrmSocket* pFrmSocket);

    //为本端口删除一个SOCKET连接
    virtual void DelFrmSocket(CFrmSocket* pFrmSocket);

    //处理来自*pFrmSocket对象接收到的数据帧
    virtual void OnReceiveEx(CFrmSocket* pFrmSocket, void* pFrame);

    //将从告警模块发来的数据转发到convert
    virtual int Send(const MSG_INTRA* const pMsg);

protected:

    //代表convert模块与计费网关的连接
	CABFrmSocket* m_pABFrmSocket;
};

///////////////////////////////////////////////////////////////////////////////
//GTP'计费接口增加 Z30971 2004/06/10

//对GTP'协议TCP方式的客户端的抽象
class CGTPTcpCliCommRouter:public CCommRouter
{
public:
    CGTPTcpCliCommRouter(CCommRouterMan* const pCommRouterMan,
                         const unsigned long uAppType,
                         PROCESS_TYPE const nProcessType);
    virtual ~CGTPTcpCliCommRouter();

    //为本端口删除一个SOCKET连接
    virtual void DelFrmSocket(CFrmSocket* pFrmSocket);

    //秒事件处理函数
    virtual void Timer1Sec(void);

    //发送pMsg指向的数据帧
    virtual int Send(const MSG_INTRA* const pMsg);

protected:

    //与BS服务器进程进行连接
    int Connect(void);

    //该指针指向的对象表示计费接口进程与BS的连接
    CGtpTcpCliSocket* m_pGtpTcpCliSocket;

    //该成员变量表示本对象所在进程的进程编号
    PROCESS_TYPE m_nProcessType;

    //从上次没有成功连上BS服务器端开始所经历的时间，单位为S
    int m_nReConnectTimer;

};

//对GTP'协议UDP方式的客户端的抽象
class CGTPUdpCliCommRouter:public CCommRouter
{
public:
    CGTPUdpCliCommRouter(CCommRouterMan* const pCommRouterMan,
                         const unsigned long uAppType,
                         PROCESS_TYPE const nProcessType);
    virtual ~CGTPUdpCliCommRouter();

    //为本端口删除一个SOCKET连接；被管理的CGtpUdpSocket子类对象的
    //SOCK_handle_close函数会调用本函数
    virtual void DelGtpSocket(CGtpUdpCliSocket* pGtpSocket);

    //秒事件处理函数
    virtual void Timer1Sec(void);

    //发送pMsg指向的数据帧
    virtual int Send(const MSG_INTRA* const pMsg);

protected:
    //初始化SOCKET
    int Init();

    //该指针指向的对象表示计费接口进程与BS的连接
    CGtpUdpCliSocket* m_pGtpUdpCliSocket;

    //该成员变量表示本对象所在进程的进程编号
    PROCESS_TYPE m_nProcessType;
};
#endif //__CCOMMROUTER
