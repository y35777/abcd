#include "comm_struct.h"
#include "comm_layer.h"
#include "../include/toolbox.h"

/******************************************************
// 函数名:  CCommRouter::CCommRouter
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    构造函数
// 输入:
//       参数1 :CCommRouterMan* const pCommRouterMan
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CCommRouter::CCommRouter(CCommRouterMan* const pCommRouterMan,
                        const unsigned long uAppType)
{
    ASSERT(pCommRouterMan != 0);
    m_pCommRouterMan = pCommRouterMan;

    m_uAppType = uAppType;
}


/******************************************************
// 函数名: 	CCommRouter::~CCommRouter
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    析构函数
// 输入:
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
CCommRouter::~CCommRouter()
{
    m_pCommRouterMan = 0;
}


/******************************************************
// 函数名:  CCommRouter::OnReceive
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    向CCommRouterMan对象提交从低层的链路上接收上来的消息
// 输入:
//       参数1 :MSG_INTRA* pMsg
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CCommRouter::OnReceive(MSG_INTRA* pMsg)
{
    //向上提交消息
    m_pCommRouterMan->OnReceive(pMsg);
}


/******************************************************
// 函数名: 	CCommRouter::AddFrmSocket
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    为本端口增加一个SOCKET连接
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
int CCommRouter::AddFrmSocket(CFrmSocket* pFrmSocket)
{
    return 0;
}


/******************************************************
// 函数名: 	CCommRouter::DelFrmSocket
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    为本端口注销一个SOCKET连接
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
void CCommRouter::DelFrmSocket(CFrmSocket* pFrmSocket)
{
    //
}


/******************************************************
// 函数名: 	CCommRouter::Timer1Sec
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    秒事件处理函数
// 输入:
//       参数1 :void
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
void CCommRouter::Timer1Sec(void)
{
    //
}


/******************************************************
// 函数名:  CCommRouter::OnReceive
// 作者:    Wangfeng
// 时期:    01-10-10
// 描述:    处理来自*pFrmSocket对象接收到的数据帧
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
//       参数2 :void* pFrame
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-10      Wangfeng
******************************************************/
void CCommRouter::OnReceiveEx(CFrmSocket* pFrmSocket,
                              void* pFrame)
{
    //向上提交消息
    m_pCommRouterMan->OnReceive((MSG_INTRA*) pFrame);
}


/******************************************************
// 函数名:  CCommRouter::RegisterHandle
// 作者:    Wangfeng
// 时期:    01-10-10
// 描述:    注册CSocketImpl子类的对象
// 输入:
//       参数1 :CSocketImpl* pSocket
//       参数2 :ACE_Reactor_Mask mask
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-10      Wangfeng
******************************************************/
int CCommRouter::RegisterHandle(CSocketImpl* pSocket,
                                ACE_Reactor_Mask mask)
{
    //调用*m_pCommRouterMan对象的RegisterHandle函数，
    //并返回该函数的返回值
    return m_pCommRouterMan->RegisterHandle(pSocket, mask);
}


/******************************************************
// 函数名:  CCommRouterMan::CCommRouterMan
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    构造函数
// 输入:
//       参数1 :CCommLayer* const pCommLayer
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CCommRouterMan::CCommRouterMan(CCommLayer* const pCommLayer)
{
    m_pMsgCommRouter = 0;
    ASSERT(pCommLayer != 0);
    m_pCommLayer = pCommLayer;
}


/******************************************************
// 函数名:  CCommRouterMan::~CCommRouterMan
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    析构函数
// 输入:
//       参数1 :void
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CCommRouterMan::~CCommRouterMan()
{
    if (m_pMsgCommRouter != 0)
    {
        delete m_pMsgCommRouter;
        m_pMsgCommRouter = 0;
    }

    //释放m_CommRouterMap中所有元素
    MAP_APT_TO_PCOMMROUTER::iterator i;
    for(i = m_CommRouterMap.begin();
        i != m_CommRouterMap.end();
        i++)
    {
        delete (*i).second;
    }
    m_CommRouterMap.clear();

    m_pCommLayer = 0;
}


/******************************************************
// 函数名:  CCommRouterMan::RegisterCommRouter
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    注册CCommRouter子类的对象
// 输入:
//       参数1 :CCommRouter* pCommRouter
//       参数2 :unsigned long uAppType
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CCommRouterMan::RegisterCommRouter(CCommRouter* pCommRouter,
                                       unsigned long uAppType)
{
    if (uAppType == APP_MSG)
    //即应用类型为消息中转的服务器端或客户端
    {
        if (m_pMsgCommRouter != 0)
        {
            return ERR_SAME_ROUTER_REGISTERED;
        }
        else
        {
            m_pMsgCommRouter = pCommRouter;
            return ERR_SUCCESS;
        }
    }
    else
    {
        if(m_CommRouterMap.find(uAppType) != m_CommRouterMap.end())
        {
            return ERR_SAME_ROUTER_REGISTERED;
        }
        else
        {
            //将pCommRouter插入到m_CommRouerMap中
            m_CommRouterMap[uAppType] = pCommRouter;
            return ERR_SUCCESS;
        }
    }
}


/******************************************************
// 函数名:  CCommRouterMan::AddFrmSocket
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    为应用类型为uAppType的端口增加一个新连接
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
//       参数2 :unsigned long uAppType
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CCommRouterMan::AddFrmSocket(CFrmSocket* pFrmSocket,
                                 unsigned long uAppType)
{
    if (uAppType == APP_MSG)
    //即应用类型为消息中转的服务器端或客户端
    {
        if (m_pMsgCommRouter == 0)
        {
            return ERR_COMMROUTER_NOT_FOUND;
        }
        else
        {
            return m_pMsgCommRouter->AddFrmSocket(pFrmSocket);
        }
    }
    else
    {
        //判断在m_CommRouterMap中是否存在键值为uAppType的CCommRouter
        //对象指针，若不存在，则返回ERR_COMMROUTER_NOT_FOUND,否则调用该
        //对象的AddFrmSocket函数，并返回ERR_SUCCESS
        if(m_CommRouterMap.find(uAppType) == m_CommRouterMap.end())
        {
            return ERR_COMMROUTER_NOT_FOUND;
        }
        else
        {
            return (m_CommRouterMap[uAppType])->AddFrmSocket(pFrmSocket);
        }
    }
}


/******************************************************
// 函数名:  CCommRouterMan::Send
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    根据*pMsg帧头中的相关信息，选择适当的CCommRouter
//          对象，并调用该对象的Send函数
// 输入:
//       参数1 :const MSG_INTRA* const pMsg
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CCommRouterMan::Send(const MSG_INTRA* const pMsg)
{
    if (pMsg->ReceiverPid == PT_OUTSIDE)
    //即消息要发往计费网关系统之外
    {
        //判断在m_CommRouterMap中是否存在键值为pMsg->AppType的
        //CCommRouter对象的指针，若不存在，则返回ERR_FAIL，否则
        //调用该对象的Send函数，并返回该函数的返回值
        if(m_CommRouterMap.find(pMsg->AppType) == m_CommRouterMap.end())
        {
            return ERR_APP_TYPE_UNKNOWN;
        }
        else
        {
            return (m_CommRouterMap[pMsg->AppType])->Send(pMsg);
        }
    }
    else
    {
        if (m_pMsgCommRouter != 0)
        {
            return m_pMsgCommRouter->Send(pMsg);
        }
        else
        {
            return ERR_FAIL;
        }
    }
}


/******************************************************
// 函数名:  CCommRouterMan::OnReceive
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    向作为通信层的CCommLayer对象提交下层的
//          CCommRouter子类对象提交上来的消息
// 输入:
//       参数1 :MSG_INTRA* pMsg
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CCommRouterMan::OnReceive(MSG_INTRA* pMsg)
{
    //向通信层提交消息
    m_pCommLayer->OnReceive(pMsg);
}


/******************************************************
// 函数名:  CCommRouterMan::Timer1Sec
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    秒定时器事件
// 输入:
//       参数1 :void
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
void CCommRouterMan::Timer1Sec(void)
{
    if(m_pMsgCommRouter)
    {
        m_pMsgCommRouter->Timer1Sec();
    }

    MAP_APT_TO_PCOMMROUTER::iterator i;
    for(i = m_CommRouterMap.begin(); i!= m_CommRouterMap.end(); i++)
    {
        (*i).second->Timer1Sec();
    }
}


/******************************************************
// 函数名:  CCommRouterMan::RegisterHandle
// 作者:    Wangfeng
// 时期:    01-10-10
// 描述:    注册CSocketImpl子类的对象
// 输入:
//       参数1 :CSocketImpl* pSocket
//       参数2 :ACE_Reactor_Mask mask
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-10      Wangfeng
******************************************************/
int CCommRouterMan::RegisterHandle(CSocketImpl* pSocket,
                                   ACE_Reactor_Mask mask)
{
    //调用*m_pCommLayer对象的RegisterHandle函数，并返回该函数的返回值
    return m_pCommLayer->RegisterHandle(pSocket, mask);
}


/******************************************************
// 函数名:  CCommRouter
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    构造函数
// 输入:
//       参数1 :pCommRouterMan
// 返回值: :
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CGtpUdpCommRouter::CGtpUdpCommRouter(CCommRouterMan* const pCommRouterMan,
                               const unsigned long uAppType)
                               : CCommRouter(pCommRouterMan,
                                             uAppType)
{
    //
}


/******************************************************
// 函数名:  CGtpUdpCommRouter::~CGtpUdpCommRouter
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    析构函数
// 输入:
//       参数1 :void
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CGtpUdpCommRouter::~CGtpUdpCommRouter(void)
{
    while(m_SocketList.size() > 0)
    {
        CGtpUdpSocket* pGtpSocket = m_SocketList.front();
        pGtpSocket->m_pGtpCommRouter = 0;
        m_SocketList.pop_front();
    }
}


/******************************************************
// 函数名:  CGtpUdpCommRouter::Init
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    初始化函数
// 输入:
//       参数1 :unsigned char const nAPID
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CGtpUdpCommRouter::Init(unsigned char const nAPID)
{
    //从配置文件中读取编号为nAPID的接入点进程的GTP’协议端口的数量
    //nCount从配置文件中读取每个GTP’协议端口的IP地址和端口号；
    //创建nCount个CGtpUdpSocket对象，每创建一个都调用该对象的Open函数，
    //调用*m_pCommTaskMan对象的RegisterHandle函数对当前的CGtpUdpSocket
    //对象进行注册，将当前的CGtpUdpSocket对象指针插入到m_SocketList的队尾

    //打开配置文件
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    if(!pIniFile->Open())
    {
        delete pIniFile;
        return ERR_OPEN_CFG_FILE;
    }

    //读取本进程对应的接入点号
    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP, nAPID);
    INT4 nAPType = pIniFile->GetInt(strSec,
                                    CFG_AP_KEY_APTYPE,
                                    CFG_AP_KEY_APTYPE_DEFAULT);
    if((nAPType != AP_PS) && (nAPType != AP_PS_RESERVED) && (nAPType != AP_WLAN))
    {
        TRACE(MTS_COMM, S_COMM_APTYPE_DISMATCH);
        pIniFile->Close();
        delete pIniFile;
        return ERR_APTYPE_NOT_MATCH;
    }

    /*
     	读UDP与TCP共同的地址配置信息
    */
    //读取UDP与TCP的可用性掩码
    int nUdpTcpMask = pIniFile->GetInt(strSec,
                                       CFG_AP_KEY_UDP_TCP_MASK,
                                       CFG_AP_KEY_UDP_TCP_MASK_DEFAULT);

    //读取与GSN相连的主链路IP地址
    char szPMLocalIp[20];
    if (!pIniFile->GetString(strSec,
                             CFG_AP_KEY_LOCAL_IP_TO_PM,
                             CFG_AP_KEY_LOCAL_IP_TO_PM_DEFAULT,
                             szPMLocalIp,
                             sizeof(szPMLocalIp)) || (szPMLocalIp[0] == '\0'))
    {
        TRACE(MTS_COMM,
              S_COMM_READ_GSN_IP,
              CFG_AP_KEY_LOCAL_IP_TO_PM_DEFAULT);
    }

    //读取与GSN相连的主链路的端口号
    INT4 nPMLocalPort = pIniFile->GetInt(strSec,
                                         CFG_AP_KEY_LOCAL_PORT_TO_PM,
                                         CFG_AP_KEY_LOCAL_PORT_TO_PM_GSN_DEFAULT);
    
    //读取与GSN相连的备链路IP地址
    char szFMLocalIp[20];
    pIniFile->GetString(strSec,
                        CFG_AP_KEY_LOCAL_IP_TO_FM,
                        CFG_AP_KEY_LOCAL_IP_TO_FM_GSN_DEFAULT,
                        szFMLocalIp,
                        sizeof(szFMLocalIp));

    //读取与GSN相连的备链路的端口号
    INT4 nFMLocalPort = pIniFile->GetInt(strSec,
                                         CFG_AP_KEY_LOCAL_PORT_TO_FM,
                                         CFG_AP_KEY_LOCAL_PORT_TO_FM_GSN_DEFAULT);

    /*
     	读UDP特有的地址配置信息
    */
    //读取与GSN相连的主链路IP地址(UDP)
    char szPMLocalIpUdp[20];
    pIniFile->GetString(strSec,
                        CFG_AP_KEY_LOCAL_IP_TO_PM_UDP,
                        szPMLocalIp,
                        szPMLocalIpUdp,
                        sizeof(szPMLocalIpUdp));

    //读取与GSN相连的主链路的端口号(UDP)
    INT4 nPMLocalPortUdp = pIniFile->GetInt(strSec,
                                            CFG_AP_KEY_LOCAL_PORT_TO_PM_UDP,
                                            nPMLocalPort);
    
    //读取与GSN相连的备链路IP地址(UDP)
    char szFMLocalIpUdp[20];
    pIniFile->GetString(strSec,
                        CFG_AP_KEY_LOCAL_IP_TO_FM_UDP,
                        szFMLocalIp,
                        szFMLocalIpUdp,
                        sizeof(szFMLocalIpUdp));

    //读取与GSN相连的备链路的端口号(UDP)
    INT4 nFMLocalPortUdp = pIniFile->GetInt(strSec,
                                            CFG_AP_KEY_LOCAL_PORT_TO_FM_UDP,
                                            nFMLocalPort);
    
    //关闭配置文件操作对象 
    pIniFile->Close();
    delete pIniFile;

    if(nUdpTcpMask & CFG_AP_KEY_UDP_ENABLE)
    {
        CGtpUdpSocket* pGtpSocket = new CGtpUdpSocket(this);
        if(pGtpSocket->Open(szPMLocalIpUdp, nPMLocalPortUdp) != ERR_SUCCESS)
        {
            delete pGtpSocket;
            pGtpSocket = 0;
            return ERR_BIND_PORT;
        }

        //注册GtpSocket的事件
        int nRet = RegisterHandle(pGtpSocket,
                                  ACE_Event_Handler::READ_MASK);
        if(nRet != ERR_SUCCESS)
        {
            delete pGtpSocket;
            pGtpSocket = 0;
            return nRet;
        }
        m_SocketList.push_back(pGtpSocket);

        if(szFMLocalIpUdp[0] != '\0' && nFMLocalPortUdp != 0)
        {
            pGtpSocket = new CGtpUdpSocket(this);
            if(pGtpSocket->Open(szFMLocalIpUdp, nFMLocalPortUdp) != ERR_SUCCESS)
            {
                delete pGtpSocket;
                pGtpSocket = 0;
                return ERR_BIND_PORT;
            }

            //注册GtpSocket的事件
            nRet = RegisterHandle(pGtpSocket,
                                  ACE_Event_Handler::READ_MASK);
            if(nRet != ERR_SUCCESS)
            {
                delete pGtpSocket;
                pGtpSocket = 0;
                return nRet;
            }
            m_SocketList.push_back(pGtpSocket);
        }
    }

    return ERR_SUCCESS;
}


/******************************************************
// 函数名:  CGtpUdpCommRouter::Send
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    为本端口删除一个SOCKET连接；被管理的CGtpUdpSocket子类对象的
//          SOCK_handle_close函数会调用本函数
// 输入:
//       参数1 :const MSG_INTRA* const pMsg
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CGtpUdpCommRouter::Send(const MSG_INTRA* const pMsg)
{
    //将pMsg强制转换成SNetFrame*类型，pMsg->SNetAddrPair域声明了用于
    //发送UDP数据报的本端的IP地址和端口；
    //遍历m_SocketList中的CGtpUdpSocket对象，调用CGtpUdpSocket对象的GetAddr
    //函数，从而判断当前访问的CGtpUdpSocket对象是否是对应于pMsg->
    //SNetAddrPair指定的IP地址和端口号；若m_SocketList中没有对应的
    //CGtpUdpSocket对象，则返回ERR_FAIL，若存在对应的CGtpUdpSocket对象，则
    //调用该对象的Send函数，并返回该函数的返回值；
    LIST_PGTPSOCKET::iterator i;
    unsigned int nIpAddr;
    int nPort;
    for(i = m_SocketList.begin(); i != m_SocketList.end(); i++)
    {
        (*i)->GetAddr(nIpAddr, nPort);
        if((((_NetFrame *) pMsg)->SNetAddrPair.ServerIp == nIpAddr) &&
            (((_NetFrame *) pMsg)->SNetAddrPair.ServerPort == nPort))
        {
            return (*i)->Send(pMsg);
        }
    }
    return ERR_SOCKET_NOT_PRESENT;
}
/******************************************************
// 函数名:  CGtpUdpCommRouter::DelGtpSocket
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    为本端口删除一个SOCKET连接；被管理的CGtpUdpSocket
//          子类对象的SOCK_handle_close函数会调用本函数
// 输入:
//       参数1 :CGtpUdpSocket* pGtpSocket
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CGtpUdpCommRouter::DelGtpSocket(CGtpUdpSocket* pGtpSocket)
{
    //将pGtpSocket从m_SocketList中删除
    m_SocketList.remove(pGtpSocket);
}


/******************************************************
// 函数名:  CCommRouter
// 作者:    Wangfeng
// 时期:    03-06-28
// 描述:    构造函数
// 输入:
//       参数1 :pCommRouterMan
// 返回值: :
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//03-06-28       Wangfeng
******************************************************/
CGtpFrmCommRouter::CGtpFrmCommRouter(CCommRouterMan* const pCommRouterMan,
                               const unsigned long uAppType)
                               : CCommRouter(pCommRouterMan,
                                             uAppType)
{
    //
}


/******************************************************
// 函数名:  CGtpFrmCommRouter::~CGtpFrmCommRouter
// 作者:    Wangfeng
// 时期:    03-06-28
// 描述:    析构函数
// 输入:
//       参数1 :void
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//03-06-28       Wangfeng
******************************************************/
CGtpFrmCommRouter::~CGtpFrmCommRouter(void)
{
    while(m_GtpFrmSocketMap.size() > 0)
    {
        MAP<UINT4, CGtpFrmSocket*>::iterator iter = m_GtpFrmSocketMap.begin();
        CGtpFrmSocket* pGtpFrmSocket = iter->second;
        pGtpFrmSocket->m_pCommRouter = 0;
        m_GtpFrmSocketMap.erase(iter);
    }
}


/******************************************************
// 函数名:  CGtpFrmCommRouter::Init
// 作者:    Wangfeng
// 时期:    03-06-28
// 描述:    初始化函数
// 输入:
//       参数1 :unsigned char const nAPID
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//03-06-28       Wangfeng
******************************************************/
int CGtpFrmCommRouter::Init(unsigned char const nAPID)
{

    //打开配置文件
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    if(pIniFile->Open() == FALSE)
    {
        delete pIniFile;
        return ERR_OPEN_CFG_FILE;
    }

    //读取本进程对应的接入点号
    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP, nAPID);
    INT4 nAPType = pIniFile->GetInt(strSec,
                                    CFG_AP_KEY_APTYPE,
                                    CFG_AP_KEY_APTYPE_DEFAULT);
    if((nAPType != AP_PS) && (nAPType != AP_PS_RESERVED) && (nAPType != AP_WLAN))
    {
        TRACE(MTS_COMM, S_COMM_APTYPE_DISMATCH);
        pIniFile->Close();
        delete pIniFile;
        return ERR_APTYPE_NOT_MATCH;
    }

    /*
        读等效IP地址表	
    */
    //读等效IP地址表的个数，即有几个GSN采用双平面机制
    INT4 nMultiIPGsnCount = pIniFile->GetInt(strSec,
                                             CFG_AP_KEY_MULTI_IP_GSN_COUNT,
                                             CFG_AP_KEY_MULTI_IP_GSN_COUNT_DEFAULT);

    UINT4 uIPAddr;
    char szLineContent[1024];
    for(int i = 0; i < nMultiIPGsnCount; i++)
    {
        char szIPAddrSec[50];
        SNPRINTF(szIPAddrSec,
                 sizeof(szIPAddrSec),
                 "%s%d",
                 CFG_AP_KEY_GSN_IP_TABLE,
                 i + 1);
        szIPAddrSec[sizeof(szIPAddrSec) - 1] = '\0';

        pIniFile->GetString(strSec,
                          szIPAddrSec,
                          "",
                          szLineContent,
                          sizeof(szLineContent));

        const char *p = strchr(szLineContent, ':');
        if(p != NULL)
        {
            char szGsnName[1024];
            strncpy(szGsnName, 
                    szLineContent, 
                    min(sizeof(szGsnName), (UINT4)p - (UINT4)&szLineContent[0]));
            szGsnName[min(sizeof(szGsnName), (UINT4)p - (UINT4)&szLineContent[0])] = '\0';
            memmove(szLineContent, p + 1, strlen(p + 1) + 1);
        }
        
        VECTOR<UINT4> ip_table;
        if (szLineContent[0] != '\0')
        {
            char *pToken = strtok(szLineContent, ",");
            while(pToken)
            {
                uIPAddr = inet_addr(pToken);

                if(0 == uIPAddr)
                {
                    TRACE(MTS_NETWORK, S_IP_ADDRESS_ERROR, pToken);
                    return ERR_FAIL;
                }

                if(m_GsnMultiIPMap.find(uIPAddr) != m_GsnMultiIPMap.end())
                {
                    TRACE(MTS_NETWORK, S_REDUP_IP_ADDRESS, pToken);
                    return ERR_FAIL;
                }

                bool bFind = false;
                int j ;
                for(j = 0; j < ip_table.size(); j++)
                {
                    if(uIPAddr == ip_table[j])
                    {
                        bFind = true;
                        break;
                    }
                }
                if(bFind)
                {
                    TRACE(MTS_NETWORK, S_REDUP_IP_ADDRESS, pToken);
                    return ERR_FAIL;
                }
                ip_table.push_back(uIPAddr);

                pToken = strtok(NULL, ",");
            }

            if(ip_table.size() > GSN_IP_TABLE_IPCOUNT_MAX)
            {
                TRACE(MTS_NETWORK, 
                      S_GSN_IP_TOO_MANY,
                      szIPAddrSec,
                      GSN_IP_TABLE_IPCOUNT_MAX);
                return ERR_FAIL;
            }
            else if(ip_table.size() > 0)
            {
                m_GsnMultiIPTable.push_back(ip_table);
            }

            int j ;
            for(j = 0; j < ip_table.size(); j++)
            {
                uIPAddr = ip_table[j];
                m_GsnMultiIPMap[uIPAddr] = i;
            }
        }
    }
    
    //关闭配置文件操作对象 
    pIniFile->Close();
    delete pIniFile;

    return ERR_SUCCESS;
}


/******************************************************
// 函数名:  CGtpFrmCommRouter::AddFrmSocket
// 作者:    Wangfeng
// 时期:    03-06-28
// 描述:    为本端口增加一个SOCKET连接
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//03-06-28       Wangfeng
******************************************************/
int CGtpFrmCommRouter::AddFrmSocket(CFrmSocket* pFrmSocket)
{
    UINT4 uIPAddr = HTONL(pFrmSocket->GetAddrRemote().get_ip_address());

    if(m_GtpFrmSocketMap.find(uIPAddr) != m_GtpFrmSocketMap.end())
    {
        CGtpFrmSocket* pGtpFrmSocket = m_GtpFrmSocketMap[uIPAddr];
        if(NULL != pGtpFrmSocket)
        {
            //注销该SOCKET，但不要让该SOCKET在关闭事件中回调上来
            pGtpFrmSocket->m_pCommRouter = NULL;
            pGtpFrmSocket->RemoveHandler();
        }
    }
    
    m_GtpFrmSocketMap[uIPAddr] = (CGtpFrmSocket*)pFrmSocket;
	pFrmSocket->m_pCommRouter = this;
    
    return ERR_SUCCESS;
}


/******************************************************
// 函数名:  CGtpFrmCommRouter::Send
// 作者:    Wangfeng
// 时期:    03-06-28
// 描述:    为本端口删除一个SOCKET连接；被管理的CGtpUdpSocket子类对象的
//          SOCK_handle_close函数会调用本函数
// 输入:
//       参数1 :const MSG_INTRA* const pMsg
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//03-06-28       Wangfeng
******************************************************/
int CGtpFrmCommRouter::Send(const MSG_INTRA* const pMsg)
{

    int nRet = ERR_SOCKET_NOT_PRESENT;

    //将pMsg强制转换成SNetFrame*类型，pMsg->SNetAddrPair域声明了用于
    //发送UDP数据报的本端的IP地址和端口；
    UINT4 uClientIP = ((_NetFrame *) pMsg)->SNetAddrPair.ClientIp;

    //首先从MAP找可用的SOCKET
    if(m_GtpFrmSocketMap.find(uClientIP) != m_GtpFrmSocketMap.end())
    {
        CGtpFrmSocket* pGtpFrmSocket = m_GtpFrmSocketMap[uClientIP];
        if(NULL != pGtpFrmSocket)
        {
            nRet = pGtpFrmSocket->Send(pMsg->Data);
        }
    }

    //如果失败，则从其它链路发送
    if(ERR_SUCCESS != nRet)
    {
        if(m_GsnMultiIPMap.find(uClientIP) != m_GsnMultiIPMap.end())
        {
            int nLinkIndex = m_GsnMultiIPMap[uClientIP];
            VECTOR<UINT4>& ip_table = m_GsnMultiIPTable[nLinkIndex];
              
            for(int i = 0; i < ip_table.size(); i++)
            {
                UINT4 uIPAddr = ip_table[i];
                if(uIPAddr == 0)
                {
                    break;
                }
    
                if(uIPAddr != uClientIP)
                {
                    if(m_GtpFrmSocketMap.find(uIPAddr) != m_GtpFrmSocketMap.end())
                    {
                        CGtpFrmSocket* pGtpFrmSocket = m_GtpFrmSocketMap[uIPAddr];
                        if(NULL != pGtpFrmSocket)
                        {
                            nRet = pGtpFrmSocket->Send(pMsg->Data);
                        }
                    }
                    if(ERR_SUCCESS == nRet)
                    {
                        break;
                    }
                }
            }
        }
    }
    
    return nRet;
}
/******************************************************
// 函数名:  CGtpFrmCommRouter::DelGtpSocket
// 作者:    Wangfeng
// 时期:    03-06-28
// 描述:    为本端口删除一个SOCKET连接；被管理的CGtpUdpSocket
//          子类对象的SOCK_handle_close函数会调用本函数
// 输入:
//       参数1 :CGtpUdpSocket* pGtpSocket
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//03-06-28       Wangfeng
******************************************************/
void CGtpFrmCommRouter::DelGtpSocket(CSocketImpl* pGtpSocket)
{
    MAP<UINT4, CGtpFrmSocket*>::iterator i = m_GtpFrmSocketMap.begin();
    while(i != m_GtpFrmSocketMap.end())
    {
        if(UINT4(pGtpSocket) == UINT4(i->second))
        {
            m_GtpFrmSocketMap.erase(i);
            break;
        }
        i++;
    }
}


/******************************************************
// 函数名:  CMscLinkPair::CMscLinkPair
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    构造函数
// 输入:
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
CMscLinkPair::CMscLinkPair()
{
    m_pPrimaryLink = 0;
    m_pSecondaryLink = 0;
}


/******************************************************
// 函数名:  CMscLinkPair::~CMscLinkPair
// 作者:    Wangfeng
// 时期:    01-10-10
// 描述:    析构函数
// 输入:
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-10      Wangfeng
******************************************************/
CMscLinkPair::~CMscLinkPair()
{
    if(m_pPrimaryLink)
    {
        //将该Socket的m_pCommRouter赋零，可避免在注销Socket
        //时调用已删除的m_pCommRouter对象的函数
        m_pPrimaryLink->m_pCommRouter = 0;
        m_pPrimaryLink = 0;
    }

    if(m_pSecondaryLink)
    {
        //将该Socket的m_pCommRouter赋零，可避免在注销Socket
        //时调用已删除的m_pCommRouter对象的函数
        m_pSecondaryLink->m_pCommRouter = 0;
        m_pSecondaryLink = 0;
    }
}


/******************************************************
// 函数名:  CMscLinkPair::SetPrimaryLink
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    设置代表主连接的CMemFrmSocket对象
// 输入:
//       参数1 :CMemFrmSocket* pLink
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
void CMscLinkPair::SetPrimaryLink(CMemFrmSocket* pLink)
{
    m_pPrimaryLink = pLink;
}


/******************************************************
// 函数名:  CMscLinkPair::SetSecondaryLink
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    设置代表备连接的CMemFrmSocket对象
// 输入:
//       参数1 :CMemFrmSocket* pLink
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
void CMscLinkPair::SetSecondaryLink(CMemFrmSocket* pLink)
{
    m_pSecondaryLink = pLink;
}


/******************************************************
// 函数名:  CMscLinkPair::GetPrimaryLink
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    获得代表主连接的CMemFrmSocket对象
// 输入:
//       参数1 :void
// 返回值: CMemFrmSocket* const
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
CMemFrmSocket* const CMscLinkPair::GetPrimaryLink(void)
{
    return m_pPrimaryLink;
}


/******************************************************
// 函数名:  CMscLinkPair::GetSecondaryLink
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    获得代表备连接的CMemFrmSocket对象
// 输入:
//       参数1 :void
// 返回值: CMemFrmSocket* const
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
CMemFrmSocket* const CMscLinkPair::GetSecondaryLink(void)
{
    return m_pSecondaryLink;
}


/******************************************************
// 函数名:  CLinkPairs::CLinkPairs
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    构造函数
// 输入:
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
CLinkPairs::CLinkPairs()
{
	m_nLinkTimeOut = 10;
}


/******************************************************
// 函数名:  CLinkPairs::~CLinkPairs
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    析构函数
// 输入:
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
CLinkPairs::~CLinkPairs()
{
    MAP_MODNUM_TO_PCMSCLINKPAIR::iterator i;
    for(i = m_MapLinks.begin(); i != m_MapLinks.end(); i++)
    {
        delete (*i).second;
    }
    m_MapLinks.clear();
}

/******************************************************
// 函数名:  CLinkPairs::OnLinkModuleNumberChanged
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:
// 输入:
//       参数1 :(CFrmSocket* const pLink
//       参数2 :int const nOldModNum
//       参数3 :int const nNewModNum
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
void CLinkPairs::OnLinkModuleNumberChanged(CFrmSocket* const pLink,
                                           int const nOldModNum,
                                           int const nNewModNum)
{
    MAP_MODNUM_TO_PCMSCLINKPAIR::iterator i;
    //将pLink 强制转换成CMemFrmSocket*类型；
    if (nOldModNum == nNewModNum)
    {
        return;
    }
    else
    {
        if (nOldModNum != APP_MID_NULL)
        //*pLink对象曾经与主机模块号绑定
        {
/*          在m_MapLinks中查找模块号为nOldModNum的CMscLinkPair对象，
            若查找到，则根据pLink->m_bPrimary的取值调用该CMscLinkPair
            对象的SetPrimaryLink或SetSecondaryLink函数，实参为0；若该
            CMscLinkPair对象的GetPrimaryLink和GetSecondaryLink函数的
            返回值均为0，则将该CMscLinkPair对象从m_MapLinks中删除；
*/
            DelFrmSocket(pLink);
        }
        if (nNewModNum != APP_MID_NULL)
        //新的主机模块号是有效的
        {
/*          在m_MapLinks中查找主机模块号为nNewModNum的CMscLinkPair
            对象，若查找到，则根据pLink->m_bPrimary的取值或者注销该
            CMscLinkPair对象的m_pPrimaryLink或者注销该CMscLinkPair
            对象的m_pSecondaryLink；若m_MapLinks中不存在主机模块号
            为nNewModNum的CMscLinkPair对象，则创建一个CMscLinkPair
            对象，并根据pLink->m_bPrimary的取值调用该对象的
            SetPrimaryLink或SetSecondaryLink函数，实参为pLink；然后
            将新创建的CMscLinkPair对象的指针加入到m_MapLinks中，键值
            为nNewModNum；
*/
            if(m_MapLinks.find(nNewModNum) == m_MapLinks.end())
            {
                m_MapLinks[nNewModNum] = new CMscLinkPair();
            }

            CFrmSocket* pFrmSocket;
            ((CMemFrmSocket *)pLink)->SetModuleNum(nNewModNum);
            if(((CMemFrmSocket *)pLink)->GetbPrimary())
            {
                pFrmSocket = m_MapLinks[nNewModNum]->GetPrimaryLink();
                if(pFrmSocket != 0)
                {
                    ((CMemFrmSocket *)pFrmSocket)->SetModuleNum(APP_MID_NULL);
                    pFrmSocket->RemoveHandler();
                }
                m_MapLinks[nNewModNum]->SetPrimaryLink((CMemFrmSocket*)pLink);
            }
            else
            {
                pFrmSocket = m_MapLinks[nNewModNum]->GetSecondaryLink();
                if(pFrmSocket != 0)
                {
                    ((CMemFrmSocket *)pFrmSocket)->SetModuleNum(APP_MID_NULL);
                    pFrmSocket->RemoveHandler();
                }
                m_MapLinks[nNewModNum]->SetSecondaryLink((CMemFrmSocket*)pLink);
            }
        }
    }
}


/******************************************************
// 函数名:  CLinkPairs::GetLinkPairByModuleNumber
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    根据主机模块号获得对应的CMscLinkPair对象的指针
// 输入:
//       参数1 :int const nModuleNumber
// 返回值: CMscLinkPair* const
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
CMscLinkPair* const CLinkPairs::GetLinkPairByModuleNumber(
                                int const nModuleNumber)
{
/*  在m_MapLinks中查找主机模块号为nModuleNumber的CMscLinkPair
    对象，若存在，则返回该对象的指针，否则返回0
*/
    if(m_MapLinks.find(nModuleNumber) != m_MapLinks.end())
    {
        return m_MapLinks[nModuleNumber];
    }
    return 0;
}


/******************************************************
// 函数名:  CLinkPairs::DelFrmSocket
// 作者:    Wangfeng
// 时期:    01-10-17
// 描述:    为本端口删除一个SOCKET连接
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-17      Wangfeng
******************************************************/
void CLinkPairs::DelFrmSocket(CFrmSocket* pFrmSocket)
{
    //将pFrmSocket强制转换成CMemFrmSocket*类型；
    int nModNum = ((CMemFrmSocket *)pFrmSocket)->GetModuleNum();
    if(nModNum == APP_MID_NULL)
    {
        return;
    }

    //在m_MapLinks中查找主机模块号为pFrmSocket->m_nModuleNum的
    //CMscLinkPair对象
    if(m_MapLinks.find(nModNum) != m_MapLinks.end())
    {
        if(m_MapLinks[nModNum]->GetPrimaryLink() == pFrmSocket)
        {
            m_MapLinks[nModNum]->SetPrimaryLink(0);
        }
        else if(m_MapLinks[nModNum]->GetSecondaryLink() == pFrmSocket)
        {
            m_MapLinks[nModNum]->SetSecondaryLink(0);
        }
        if((0 == m_MapLinks[nModNum]->GetPrimaryLink()) &&
           (0 == m_MapLinks[nModNum]->GetSecondaryLink()))
        {
            //将该CMscLinkPair对象从m_MapLinks中删除;
            delete m_MapLinks[nModNum];
            m_MapLinks.erase(nModNum);
        }
    }
}

//颜文远2002.04.08添加
/******************************************************
// 描述:    根据*pMsg帧中的相关信息，选择适当的链路将
//          *pMsg的Data域的数据发送出去
// 输入:
//       参数1 :const MSG_INTRA* const pMsg
// 返回值: int
// 注释:
// 更新:
******************************************************/
int CLinkPairs::Send(const MSG_INTRA* const pMsg)
{
	CMemFrmSocket* pSocket = NULL;
	int nResult = ERR_SOCKET_NOT_PRESENT;

	SMemFrame* pMemFrame = (SMemFrame* )(pMsg->Data);
    SAppFrame* pAppFrame = (SAppFrame* )(pMemFrame->Data());
	UINT4 uModu = pAppFrame->receiver_mid();

	if (APP_MID_NULL == uModu)
	//接收模块号为APP_MID_NULL，则向所有模块广播
	{
		MAP_MODNUM_TO_PCMSCLINKPAIR::iterator i;
		for (i = m_MapLinks.begin(); i != m_MapLinks.end(); i++)
		{
	        nResult = ERR_SOCKET_NOT_PRESENT;
			pSocket = (*i).second->GetPrimaryLink();
			if (NULL != pSocket)
			{
				nResult = pSocket->Send(pMemFrame);
			}

			//如果主连接不存在或从主连接发送失败，则从备连接发送
			if (0 != nResult)
			{
				pSocket = (*i).second->GetSecondaryLink();
				if (NULL != pSocket)
				{
					nResult = pSocket->Send(pMemFrame);
				}
			}
		}

		return nResult;
	}
	else	//否则向指定的模块发送
	{
		CMscLinkPair* pPair = GetLinkPairByModuleNumber(uModu);

        if(NULL != pPair)
        {
	        nResult = ERR_SOCKET_NOT_PRESENT;
		    pSocket = pPair->GetPrimaryLink();
		    if (NULL != pSocket)
		    {
			    nResult = pSocket->Send(pMemFrame);
		    }

		    //如果主连接不存在或从主连接发送失败，则从备连接发送
		    if (0 != nResult)
		    {
			    pSocket = pPair->GetSecondaryLink();
			    if (NULL != pSocket)
			    {
				    nResult = pSocket->Send(pMemFrame);
			    }
		    }
        }

		return nResult;
	}
}

/******************************************************
// 描述:    秒事件处理函数
// 输入:
//       参数1 :void
// 返回值: void
// 注释:
// 更新:
******************************************************/
void CLinkPairs::Timer1Sec(void)
{
	SMemFrame* pHandShakeFrm = new(_MEM_HANDSHAKE_LENGTH) SMemFrame;
	pHandShakeFrm->nCmd(MEM_CMD_HANDSHAKE);

	//遍历所有的socket，如果超时，则将其删除；
	//否则，向其发送握手帧
	MAP_MODNUM_TO_PCMSCLINKPAIR::iterator i;
	CMemFrmSocket* pSocket = NULL;
	for (i = m_MapLinks.begin(); i != m_MapLinks.end(); i++)
	{
		//设置握手帧中的模块号
		((SMemHandShakeData* )(pHandShakeFrm->Data()))->nModu((*i).first);

		//added by maimaoshi,2002-11-27
        /*
		SLinkStat* pLinkStat = NULL;
		MSG_INTRA* pMsg = new (sizeof(SLinkStat))MSG_INTRA;
		
		pMsg->SenderPid   = CMsgProcessor::GetProcessType(); 
		pMsg->SenderMid   = MT_NETWORK;
		pMsg->ReceiverPid = PT_OM;
		pMsg->ReceiverMid = MT_PERF;
		pMsg->CmdCode     = CMD_GET_APLINK_STATE;		
		pLinkStat         = (SLinkStat*)pMsg->Data;	
		
		//end by maimaoshi*/

		pSocket = (*i).second->GetPrimaryLink();
		if (NULL != pSocket)
		{
			pSocket->m_lTimeNoData++;
			if (pSocket->m_lTimeNoData > m_nLinkTimeOut)
			{
				pSocket->RemoveHandler();
			}
			else
			{
				pSocket->Send(pHandShakeFrm);
			}
			
            /*
			//added by maimaoshi,2002-11-27
			pLinkStat->nModelNum = (*i).first;			
			pLinkStat->nLocalIP  = pSocket->GetAddrLocal().get_ip_address();
			pLinkStat->nLinkNum  = pSocket->GetAddrRemote().get_ip_address();			
			pLinkStat->yStat     = ONLINK;

			CMsgProcessor::SendMsg(pMsg);
			//end*/
		}
			
		pSocket = (*i).second->GetSecondaryLink();
		if (NULL != pSocket)
		{
			pSocket->m_lTimeNoData++;
			if (pSocket->m_lTimeNoData > m_nLinkTimeOut)
			{
				pSocket->RemoveHandler();
			}
			else
			{
				pSocket->Send(pHandShakeFrm);
			}

            /*
			//added by maimaoshi,2002-11-27
			pLinkStat->nModelNum = (*i).first;			
			pLinkStat->nLocalIP  = pSocket->GetAddrLocal().get_ip_address();
			pLinkStat->nLinkNum  = pSocket->GetAddrRemote().get_ip_address();			
			pLinkStat->yStat     = ONLINK;

			CMsgProcessor::SendMsg(pMsg);
			//end*/
		}
		
	}

	delete pHandShakeFrm;
}

//设置心跳超时限制
void CLinkPairs::SetLinkTimeOut(int nLinkTimeOut)
{
	m_nLinkTimeOut = nLinkTimeOut;
}

//颜文远2002.04.08添加结束

CSoftXLinkPairs::CSoftXLinkPairs()
{
	//
}


CSoftXLinkPairs::~CSoftXLinkPairs()
{
    MAP_MODNUM_TO_PCMSCLINKPAIR::iterator i;
    for(i = m_MapLinks.begin(); i != m_MapLinks.end(); i++)
    {
        delete (*i).second;
    }
    m_MapLinks.clear();
}

/******************************************************
// 描述:    根据*pMsg帧中的相关信息，选择适当的链路将
//          *pMsg的Data域的数据发送出去
// 输入:
//       参数1 :const MSG_INTRA* const pMsg
// 返回值: int
// 注释:
// 更新:
******************************************************/
int CSoftXLinkPairs::Send(const MSG_INTRA* const pMsg)
{
	CMemFrmSocket* pSocket = NULL;
	int nResult = ERR_SOCKET_NOT_PRESENT;

	SSysFrame* pFrame = (SSysFrame* )(pMsg->Data);

	UINT4 uModu = pFrame->receiver_mid();
	CMscLinkPair* pPair = GetLinkPairByModuleNumber(uModu);

    if(NULL != pPair)
    {
	    pSocket = pPair->GetPrimaryLink();
	    if (NULL != pSocket)
	    {
		    nResult = pSocket->Send(pFrame);
	    }

	    //如果主连接不存在或从主连接发送失败，则从备连接发送
	    if (0 != nResult)
	    {
		    pSocket = pPair->GetSecondaryLink();
		    if (NULL != pSocket)
		    {
			    nResult = pSocket->Send(pFrame);
		    }
	    }
    }

	return nResult;
}

int CSoftXLinkPairs::RegisterModule(CFrmSocket* pLink, int nModNum)
{
    if (nModNum != APP_MID_NULL)
    {
        if(m_MapLinks.find(nModNum) == m_MapLinks.end())
        {
            m_MapLinks[nModNum] = new CMscLinkPair();
        }

        if(((CMemFrmSocket *)pLink)->GetbPrimary())
        {
            m_MapLinks[nModNum]->SetPrimaryLink((CMemFrmSocket*)pLink);
        }
        else
        {
            m_MapLinks[nModNum]->SetSecondaryLink((CMemFrmSocket*)pLink);
        }
    }

    return 0;
}

CMscLinkPair* const CSoftXLinkPairs::GetLinkPairByModuleNumber(
                                     int const nModuleNumber)
{
/*  在m_MapLinks中查找主机模块号为nModuleNumber的CMscLinkPair
    对象，若存在，则返回该对象的指针，否则返回0
*/
    if(m_MapLinks.find(nModuleNumber) != m_MapLinks.end())
    {
        return m_MapLinks[nModuleNumber];
    }
    return 0;
}

void CSoftXLinkPairs::DelFrmSocket(CFrmSocket* pFrmSocket)
{
    MAP_MODNUM_TO_PCMSCLINKPAIR::iterator i = m_MapLinks.begin();

    while(i != m_MapLinks.end())
    {
        CMscLinkPair* pLinkPair = i->second;
        if(pLinkPair != NULL)
        {
            if(pLinkPair->GetPrimaryLink() == pFrmSocket)
            {
                pLinkPair->SetPrimaryLink(0);
            }
            else if(pLinkPair->GetSecondaryLink() == pFrmSocket)
            {
                pLinkPair->SetSecondaryLink(0);
            }

            if((0 == pLinkPair->GetPrimaryLink()) &&
               (0 == pLinkPair->GetSecondaryLink()))
            {
                //将该CMscLinkPair对象从m_MapLinks中删除;
                delete pLinkPair;
                m_MapLinks.erase(i);
                i = m_MapLinks.begin();
                continue;
            }
        }
        i++;
    }
}

/******************************************************
// 函数名:  CSlideWndCommRouter::CSlideWndCommRouter
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    构造函数
// 输入:
//       参数1 :CCommRouterMan* const pCommRouterMan
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
CSlideWndCommRouter::CSlideWndCommRouter(CCommRouterMan*
                                         const pCommRouterMan,
                                         const unsigned long uAppType)
                                         :CCommRouter(pCommRouterMan,
                                                      uAppType)
{
    //
}


/******************************************************
// 函数名:  CSlideWndCommRouter::~CSlideWndCommRouter
// 作者:    Wangfeng
// 时期:    01-10-10
// 描述:    析构函数
// 输入:
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-10      Wangfeng
******************************************************/
CSlideWndCommRouter::~CSlideWndCommRouter()
{
    //释放m_SocketMap中的对象
    MAP_PCMEMFRMSOCKET_TO_INT::iterator i;
    for(i = m_SlideWndSocketMap.begin();
        i != m_SlideWndSocketMap.end();
        i++)
    {
        //将该Socket的m_pCommRouter赋零，可避免在注销Socket
        //时调用已删除的m_pCommRouter对象的函数
        (*i).first->m_pCommRouter = 0;
    }
    m_SlideWndSocketMap.clear();
}


/******************************************************
// 函数名:  CSlideWndCommRouter::AddFrmSocket
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    为本端口增加一个SOCKET连接
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
int CSlideWndCommRouter::AddFrmSocket(CFrmSocket* pFrmSocket)
{
    //将pFrmSocket强制转换成CMemFrmSocket*类型
    //将pFrmSocket加入到m_SlideWndSocketMap中
    m_SlideWndSocketMap[(CMemFrmSocket *)pFrmSocket] = 
        ((CMemFrmSocket *)pFrmSocket)->GetModuleNum();
    pFrmSocket->m_pCommRouter = this;
    return 0;
}


/******************************************************
// 函数名:  CSlideWndCommRouter::DelFrmSocket
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    为本端口删除一个SOCKET连接
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
void CSlideWndCommRouter::DelFrmSocket(CFrmSocket* pFrmSocket)
{
    //将pFrmSocket强制转换成CMemFrmSocket*类型；
    //将pFrmSocket从m_SlideWndSocketMap中删除;
    if(((CMemFrmSocket*)pFrmSocket)->GetModuleNum() != APP_MID_NULL)
    {
        m_LinkPairs.DelFrmSocket((CMemFrmSocket*)pFrmSocket);
    }

    m_SlideWndSocketMap.erase((CMemFrmSocket*)pFrmSocket);

    pFrmSocket->m_pCommRouter = NULL;
}


/******************************************************
// 函数名:  CSlideWndCommRouter::OnReceiveEx
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    处理来自*pFrmSocket对象接收到的数据帧
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
//       参数2 :void* pFrame
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
void CSlideWndCommRouter::OnReceiveEx(CFrmSocket* pFrmSocket,
                                      void* pFrame)
{

    SMemFrame* pMsg = (SMemFrame *)pFrame;
    //将pFrmSocket对象的超时计数器清零

    if(pMsg->nCmd() == MEM_CMD_HANDSHAKE)
    //若*pMsg为握手帧
    {
        //若握手帧中主机模块号信息不等于pFrmSocket->m_nModuleNum，
        //调用m_LinkPairs对象的OnLinkModuleNumberChanged函数，
        //并改变该pFrmSocket保存的主机模块号信息，同时要相应从
        //m_SlideWndSocketMap删除该pFrmSocket项
        UINT4 nModuleNum = ((PMemHandShakeData)(pMsg->Data()))->nModu();
        if(nModuleNum != ((CMemFrmSocket*)pFrmSocket)->GetModuleNum())
        {
            m_LinkPairs.OnLinkModuleNumberChanged(pFrmSocket,
                        ((CMemFrmSocket*)pFrmSocket)->GetModuleNum(),
                        nModuleNum);
            m_SlideWndSocketMap.erase(((CMemFrmSocket*)pFrmSocket));
        }
    }
    else
    {
		//颜文远2002.04.19修改
		//一个SMemFrame的数据中可能包含多个SAppFrame
		MSG_INTRA* pMsgIntra = NULL;
		BYTE* pAFrame = pMsg->Data();
		SAppFrame* pAppFrame = NULL;
		while (pMsg->Data() + pMsg->nLen() - pAFrame > _APPFRAME_EXHEAD_LENGTH)
		{
			//如果剩余的数据还不够一个SAppFrame，则退出
			pAppFrame = (SAppFrame* )pAFrame;
			if (pMsg->Data() + pMsg->nLen() - pAFrame < pAppFrame->size())
			{
				break;
			}

			//将pAppFrame打包成一个MSG_INTRA实例的Data域，调用基类CCommRouter::
			//OnReceive接口提交数据帧
			pMsgIntra = new(pAppFrame->size()) MSG_INTRA;
            if(NULL != pMsgIntra)
            {
			    pMsgIntra->CmdCode = CMD_HOST_DATA;
			    pMsgIntra->SenderPid = PT_OUTSIDE;
			    pMsgIntra->SenderMid = MT_UNKNOWN;
			    pMsgIntra->ReceiverPid = CMsgProcessor::GetProcessType();
			    pMsgIntra->ReceiverMid = MT_NETWORK;
			    memcpy(pMsgIntra->Data, pAppFrame, pMsgIntra->Len);
			    CCommRouter::OnReceive(pMsgIntra);
            }

			pAFrame += pAppFrame->size();
		}

		//如果MemFrame中的数据存在错误，则写入TRACE
		if (pAFrame < pMsg->Data() + pMsg->nLen())
		{
			int nRemain = pMsg->Data() + pMsg->nLen() - pAFrame;
			MSGOUT(MTS_COMM, MSGOUT_LEVEL_URGENT, S_COMM_MEM_LEN_ERR, nRemain);
			TRACE(MTS_COMM, S_COMM_MEM_LEN_ERR, nRemain);
		}
    }
	//颜文远2002.04.19修改完成



    //输入的消息帧由调用者调用CFrmSocket::OnReceiveFrame删除

}


/******************************************************
// 函数名:  CSlideWndCommRouter::Send
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    根据*pMsg帧头中的相关信息，选择适当的链路将
//          *pMsg的Data域的数据发送出去
// 输入:
//       参数1 :const MSG_INTRA* const pMsg
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
int CSlideWndCommRouter::Send(const MSG_INTRA* const pMsg)
{
	//颜文远2002.04.08添加
	return m_LinkPairs.Send(pMsg);
	//颜文远2002.04.08添加结束
}

/******************************************************
// 函数名:  CSlideWndCommRouter::Timer1Sec
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    秒事件处理函数
// 输入:
//       参数1 :void
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
void CSlideWndCommRouter::Timer1Sec(void)
{
	//颜文远2002.04.08添加
	m_LinkPairs.Timer1Sec();
	//颜文远2002.04.08添加结束
}

//颜文远2002.04.08添加
//设置心跳超时限制
void CSlideWndCommRouter::SetLinkTimeOut(int nLinkTimeOut)
{
	m_LinkPairs.SetLinkTimeOut(nLinkTimeOut);
}
//颜文远2002.04.08添加结束


//颜文远2002.04.08添加
//构造函数
CSoftXCommRouter::CSoftXCommRouter(CCommRouterMan* const pCommRouterMan,
									const unsigned long uAppType)
						: CCommRouter(pCommRouterMan, uAppType)
{
	m_nLinkTimeOut       = 10;
}

//析构函数
CSoftXCommRouter::~CSoftXCommRouter()
{
    //处理m_AllFrmSockets中的对象
    VECTOR<CSysFrmSocket* >::iterator i;
    for(i = m_AllFrmSockets.begin(); i != m_AllFrmSockets.end(); i++)
    {
        //将该Socket的m_pCommRouter赋零，可避免在注销Socket
        //时调用已删除的m_pCommRouter对象的函数
        (*i)->m_pCommRouter = NULL;
    }
    m_AllFrmSockets.clear();
}

/******************************************************
// 描述:    为本端口增加一个SOCKET连接
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
// 返回值: void
// 注释:
******************************************************/
int CSoftXCommRouter::AddFrmSocket(CFrmSocket* pFrmSocket)
{
	m_AllFrmSockets.push_back((CSysFrmSocket* )pFrmSocket);
	pFrmSocket->m_pCommRouter = this;

	return 0;
}

/******************************************************
// 描述:    为本端口删除一个SOCKET连接
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
// 返回值: void
// 注释:
******************************************************/
void CSoftXCommRouter::DelFrmSocket(CFrmSocket* pFrmSocket)
{
	//在m_AllFrmSockets中删除pFrmSocket
	VECTOR<CSysFrmSocket* >::iterator j;
	for (j = m_AllFrmSockets.begin(); j != m_AllFrmSockets.end(); j++)
	{
		if (*j == pFrmSocket)
		{
			m_AllFrmSockets.erase(j);
			break;
		}
	}

	//在m_LinkPairs中删除pFrmSocket
    m_LinkPairs.DelFrmSocket(pFrmSocket);

    //避免回调
    pFrmSocket->m_pCommRouter = NULL;
}

/******************************************************
// 描述:    处理来自*pFrmSocket对象接收到的数据帧
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
//       参数2 :void* pFrame
// 返回值: void
// 注释:
******************************************************/
void CSoftXCommRouter::OnReceiveEx(CFrmSocket* pFrmSocket, void* pFrame)
{
	SSysFrame* pMsg = (SSysFrame* )pFrame;
	UINT4 uModu = pMsg->sender_mid();

	//根据消息中的模块号设置与模块号对应的Socket
	if (SYS_MID_BAM != uModu)
	{
        m_LinkPairs.RegisterModule(pFrmSocket, uModu);
	}

	//pFrame为握手消息，则不向上层传送，直接返回
	if (SYS_CMD_SYS_HAND_SHAKE == pMsg->ucCmdCode())
	{
		return;
	}

	//pFrame不是握手消息时，构造MSG_INTRA结构，提交给网络模块
    MSG_INTRA* pMsgIntra = new(pMsg->size()) MSG_INTRA;
    if(NULL != pMsgIntra)
    {
        pMsgIntra->CmdCode = CMD_HOST_DATA;
        pMsgIntra->SenderPid = PT_OUTSIDE;
        pMsgIntra->SenderMid = MT_UNKNOWN;
        pMsgIntra->ReceiverPid = CMsgProcessor::GetProcessType();
        pMsgIntra->ReceiverMid = MT_NETWORK;
        memcpy(pMsgIntra->Data, pMsg, pMsg->size());
        CCommRouter::OnReceive(pMsgIntra);
    }
}

/******************************************************
// 描述:    根据*pMsg帧中的相关信息，选择适当的链路将
//          *pMsg的Data域的数据发送出去
// 输入:
//       参数1 :const MSG_INTRA* const pMsg
// 返回值: int
// 注释:
******************************************************/
int CSoftXCommRouter::Send(const MSG_INTRA* const pMsg)
{
	SSysFrame* pFrame = (SSysFrame* )(pMsg->Data);
	CFrmSocket* pSocket = NULL;

	if (SYS_MID_BAM == pFrame->receiver_mid())
	//对于送往BAM的消息，则顺序从m_AllFrmSockets取一个socket进行发送
	{
		static int nIndex = 0;
		if (m_AllFrmSockets.size() > 0)
		{
			if (nIndex >= m_AllFrmSockets.size())
			{
				nIndex = 0;
			}

            pSocket = m_AllFrmSockets[nIndex++];
	        if (NULL != pSocket)
	        {
                return pSocket->Send(pFrame);
            }
		}
	}
	else	//否则根据接收模块号选取socket进行发送
	{
	    return m_LinkPairs.Send(pMsg);
	}

	return ERR_SOCKET_NOT_PRESENT;
}

/******************************************************
// 描述:    秒事件处理函数
// 输入:
//       参数1 :void
// 返回值: void
// 注释:
******************************************************/
void CSoftXCommRouter::Timer1Sec(void)
{
	//创建握手帧
	SSysFrame* pHandShakeFrm = new(0) SSysFrame;
	pHandShakeFrm->sender_mid(SYS_MID_BAU);
	pHandShakeFrm->sender_pid(SYS_PID_HTCOMM);
	pHandShakeFrm->receiver_pid(SYS_PID_HTCOMM);
	pHandShakeFrm->ucCmdCode(SYS_CMD_BAU_HAND_SHAKE);

	VECTOR<CSysFrmSocket* >::iterator i;

	//遍历所有的socket，如果超时，则将其删除；
	//否则，向其发送握手帧
	for (i = m_AllFrmSockets.begin(); i != m_AllFrmSockets.end(); i++)
	{
		(*i)->m_lTimeNoData++;
		if ((*i)->m_lTimeNoData > m_nLinkTimeOut)
		{
			(*i)->RemoveHandler();
		}
		else
		{
			(*i)->Send(pHandShakeFrm);
		}
	}

	delete pHandShakeFrm;
}

//设置心跳超时限制
void CSoftXCommRouter::SetLinkTimeOut(int nLinkTimeOut)
{
	m_nLinkTimeOut = nLinkTimeOut;
}

//颜文远2002.04.08添加结束



/******************************************************
// 函数名:  CMsgSvrCommRouter::CMsgSvrCommRouter
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    构造函数
// 输入:
//       参数1 :CCommRouterMan* const pCommRouterMan
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
CMsgSvrCommRouter::CMsgSvrCommRouter(CCommRouterMan*
                                     const pCommRouterMan,
                                     const unsigned long uAppType)
                                     :CCommRouter(pCommRouterMan,
                                                  uAppType)
{
    //
}


/******************************************************
// 函数名:  CMsgSvrCommRouter::~CMsgSvrCommRouter
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    析构函数
// 输入:
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
CMsgSvrCommRouter::~CMsgSvrCommRouter()
{
    //释放m_SocketMap中的对象
    MAP_PT_TO_PMSGFRMSOCKET::iterator i;
    for(i = m_SocketMap.begin(); i != m_SocketMap.end(); i++)
    {
        //将该Socket的m_pCommRouter赋零，可避免在注销Socket
        //时调用已删除的m_pCommRouter对象的函数
        (*i).second->m_pCommRouter = 0;
    }
    m_SocketMap.clear();

    //释放m_SocketList中的对象
    while(m_SocketList.size() > 0)
    {
        CMsgFrmSocket* pMsgFrmSocket = m_SocketList.front();

        //将该Socket的m_pCommRouter赋零，可避免在注销Socket
        //时调用已删除的m_pCommRouter对象的函数
        pMsgFrmSocket->m_pCommRouter = 0;
        m_SocketList.pop_front();
    }
}



/******************************************************
// 函数名:  CMsgSvrCommRouter::AddFrmSocket
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    为本端口增加一个SOCKET连接
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
int CMsgSvrCommRouter::AddFrmSocket(CFrmSocket* pFrmSocket)
{
    //将pFrmSocket强制转换成CMsgFrmSocket*类型
    //将pFrmSocket插入到m_SocketList的队尾
    m_SocketList.push_back((CMsgFrmSocket*)pFrmSocket);
    pFrmSocket->m_pCommRouter = this;
    return 0;
}



/******************************************************
// 函数名:  CMsgSvrCommRouter::DelFrmSocket
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    为本端口删除一个SOCKET连接
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
void CMsgSvrCommRouter::DelFrmSocket(CFrmSocket* pFrmSocket)
{
    //将pFrmSocket强制转换成CMsgFrmSocket*类型
    //将pFrmSocket从m_SocketList及m_SocketMap中删除
    MAP_PT_TO_PMSGFRMSOCKET::iterator i;

    for(i = m_SocketMap.begin(); i != m_SocketMap.end(); i++)
    {
        if((*i).second == (CMsgFrmSocket*)pFrmSocket)
        {
            m_SocketMap.erase(i);
            return;
        }
    }

    m_SocketList.remove((CMsgFrmSocket*)pFrmSocket);
}



/******************************************************
// 函数名:  CMsgSvrCommRouter::OnReceiveEx
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    处理来自*pFrmSocket对象接收到的数据帧
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
//       参数2 :void* pFrame
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
void CMsgSvrCommRouter::OnReceiveEx(CFrmSocket* pFrmSocket,
                                    void* pFrame)
{
/*  （1 ）检查pMsg是否是“客户端注册消息”，若pMsg是“客户端
    注册消息”，根据注册消息中的SenderPid在m_SocketMap中建立
    pFrmSocket对象与进程编号的映射关系，如果m_SocketMap中已存
    在该进程号对应的CMsgFrmSocket对象，则先注销该CMsgFrmSocket
    对象，再从map中删除该对象，然后添加新的对象，并且从
    m_SocketList中删除CMsgFrmSocket*所指的对象，并删除pMsg；
    否则执行步骤（2 ）；*/
    MSG_INTRA* pMsg = (MSG_INTRA *) pFrame;

    if(pMsg->CmdCode == CMD_REG_PROCESS)
    //客户端注册消息
    {
        if(m_SocketMap.find(pMsg->SenderPid) != m_SocketMap.end())
        //如果已存在一个与pMsg->SenderPid匹配的Socket，注销该Socket
        {
            if (m_SocketMap[pMsg->SenderPid] && 
                (m_SocketMap[pMsg->SenderPid] != pFrmSocket))
            {
                m_SocketMap[pMsg->SenderPid]->RemoveHandler();
                m_SocketMap[pMsg->SenderPid]->m_pCommRouter = 0;
            }
        }
        
        TRACE(MTS_COMM, S_COMM_SHAKE_HAND, pMsg->SenderPid);

        //在Map中添加进程编号到Socket指针的索引
        m_SocketMap[pMsg->SenderPid] = (CMsgFrmSocket*)pFrmSocket;

        //从m_SocketList中删除该Socket指针
        m_SocketList.remove((CMsgFrmSocket*)pFrmSocket);

        //不用向上提交该消息帧，所以在此处删除该消息帧
        delete pMsg;
    }
    else
    {
        /*判断MSG_INTRA的ReceivePid是否为本进程ID，若是本进程ID，则调用
        CCommRouter::OnReceive直接向上层提交pMsg；如果是PT_BROADCAST，则
        广播该消息，否则直接转发该消息；*/
        if (PT_BROADCAST == pMsg->ReceiverPid)
        {
            MAP_PT_TO_PMSGFRMSOCKET::iterator i;
            for (i = m_SocketMap.begin(); i != m_SocketMap.end(); i++)
            {
                pMsg->ReceiverPid = (*i).first;
                Send(pMsg);
            }
            pMsg->ReceiverPid = PT_KERNEL;
            CCommRouter::OnReceive(pMsg);
        }
        else if(PT_KERNEL == pMsg->ReceiverPid)
        {
            CCommRouter::OnReceive(pMsg);
        }
        else
        {
            //转发该消息帧
            int nRet = Send(pMsg);
            //张玉新 2002-03-13 delete old code for 问题单D11689
            //在此处必须删除该消息帧
            delete pMsg;
        }
    }
}


/******************************************************
// 函数名:  CMsgSvrCommRouter::Send
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    根据*pMsg帧头中的相关信息，选择适当的链路将
//          *pMsg的Data域的数据发送出去
// 输入:
//       参数1 :const MSG_INTRA* const pMsg
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
int CMsgSvrCommRouter::Send(const MSG_INTRA* const pMsg)
{
    //根据pMsg->ReceiverPid查找m_SocketMap,若存在与ReceiverPid
    //对应的CMsgFrmSocket对象,请求CMsgFrmSocket对象发送消息,
    //并返回该CMsgFrmSocket对象的Send函数的返回值,否则返回失败
    if(m_SocketMap.find(pMsg->ReceiverPid) != m_SocketMap.end())
    {
        return m_SocketMap[pMsg->ReceiverPid]->Send(pMsg);
    }
    return ERR_SOCKET_NOT_PRESENT;
}


/******************************************************
// 函数名:  CMsgCliCommRouter::CMsgCliCommRouter
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    构造函数
// 输入:
//       参数1 :CCommRouterMan* const pCommRouterMan
//       参数2 :PROCESS_TYPE const nProcessType
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
CMsgCliCommRouter::CMsgCliCommRouter(CCommRouterMan* const pCommRouterMan,
                                     const unsigned long uAppType,
                                     PROCESS_TYPE const nProcessType)
                                     :CCommRouter(pCommRouterMan,
                                                  uAppType)
{
    m_nProcessType = nProcessType;
    m_pMsgFrmSocket = 0;
    m_nReConnectTimer = 0;

    //与Kernel进行连接
    Connect();
}


/******************************************************
// 函数名:  CMsgCliCommRouter::~CMsgCliCommRouter
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    析构函数
// 输入:
//       参数1 :void
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
CMsgCliCommRouter::~CMsgCliCommRouter(void)
{
    if(m_pMsgFrmSocket)
    {
        //将该Socket的m_pCommRouter赋零，可避免在注销Socket
        //时调用已删除的m_pCommRouter对象的函数
        m_pMsgFrmSocket->m_pCommRouter = 0;
        m_pMsgFrmSocket = NULL;
    }
}



/******************************************************
// 函数名:  CMsgCliCommRouter::DelFrmSocket
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    为本端口删除一个SOCKET连接
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
void CMsgCliCommRouter::DelFrmSocket(CFrmSocket* pFrmSocket)
{
    ASSERT(m_pMsgFrmSocket == pFrmSocket);

    if(m_pMsgFrmSocket && m_pMsgFrmSocket == pFrmSocket)
    {
        m_pMsgFrmSocket->m_pCommRouter = 0;
        m_pMsgFrmSocket = NULL;
    }
}


/******************************************************
// 函数名:  CMsgCliCommRouter::Connect
// 作者:    Wangfeng
// 时期:    01-10-17
// 描述:    与Kernel进程进行连接
// 输入:
//       参数1 :void
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-17      Wangfeng
******************************************************/
int CMsgCliCommRouter::Connect(void)
{
    //使用ACE_SOCK_Connector连接KENERL进程的消息中转服务
    //器端，若连接成功，则创建一个CMsgFrmSocket对象，将该
    //对象的指针赋值给m_pMsgFrmSocket，调用该对象的Send函
    //数向对端发送“注册消息”，若发送不成功，则调用
    //m_pMsgFrmSocket的成员函数RemoveHandler()从ACE_Reactor
    //中注销事件，并使m_pMsgFrmSocket等于0

    //从配置文件中读取MsgSvr的端口号，缺省值为9999
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    pIniFile->Open();
    int nMsgSvrPort = pIniFile->GetInt(CFG_SEC_COMMON,
                                CFG_COMMON_KEY_MSGSVR_PORT,
                                CFG_COMMON_KEY_MSGSVR_PORT_DEFAULT);
    pIniFile->Close();
    delete pIniFile;

    //与Kernel进程进行重连
    ACE_SOCK_Connector Connector;
    ACE_SOCK_Stream *pstream = new ACE_SOCK_Stream();
    ACE_INET_Addr remote_sap;
    remote_sap.set(nMsgSvrPort, LOCAL_ADDR);

    ACE_Time_Value timeout(2);
    int nRet = ERR_FAIL;

    if(Connector.connect(*pstream, remote_sap, &timeout) != 0)
    //重连不成功
    {
        delete pstream;
        return nRet;
    }

    //重连成功后
    m_pMsgFrmSocket = new CMsgFrmSocket(pstream);
    m_pMsgFrmSocket->m_pCommRouter = this;

    //注册Socket的读写事件
    nRet = RegisterHandle(m_pMsgFrmSocket,
                          ACE_Event_Handler::READ_MASK |
                          ACE_Event_Handler::WRITE_MASK);
    if(nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_CNT_KNL_REG_ERR, nRet);

        m_pMsgFrmSocket->m_pCommRouter = 0;
        delete m_pMsgFrmSocket;
        m_pMsgFrmSocket = NULL;
        return nRet;
    }

    //发握手消息向Kernel注册
    MSG_INTRA* pMsg = new MSG_INTRA;
    pMsg->SenderPid = CMsgProcessor::GetProcessType();
    pMsg->SenderMid = MT_NETWORK;
    pMsg->ReceiverPid = PT_KERNEL;
    pMsg->ReceiverMid = MT_NETWORK;
    pMsg->CmdCode = CMD_REG_PROCESS;
    nRet = m_pMsgFrmSocket->Send(pMsg);
    delete pMsg;
    if(nRet != 0)
    {
        TRACE(MTS_COMM, S_COMM_CNT_KNL_SEND_ERR, nRet);

        m_pMsgFrmSocket->RemoveHandler();
        DelFrmSocket(m_pMsgFrmSocket);
    }
    else
    {
        ACE_INET_Addr LocalAddr;
        pstream->get_local_addr(LocalAddr);
        TRACE(MTS_COMM, S_COMM_CONNECT_KERNEL,
              LocalAddr.get_host_addr(),
              LocalAddr.get_port_number());
        ACE_INET_Addr& addr = m_pMsgFrmSocket->GetAddrRemote();
        addr = remote_sap;
    }
    return nRet;
}



/******************************************************
// 函数名:  CMsgCliCommRouter::Timer1Sec
// 作者:    Wangfeng
// 时期:    01-10-8
// 描述:    秒事件处理函数
// 输入:
//       参数1 :void
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
void CMsgCliCommRouter::Timer1Sec(void)
{
    if (m_pMsgFrmSocket == 0)
    {
        if (m_nReConnectTimer >= 5)
        {
            //重连计数器清零
            m_nReConnectTimer = 0;

            //与Kernel进行重连
            Connect();
        }
        else
        {
            m_nReConnectTimer++;
        }
    }
}



/******************************************************
// 函数名:  CMsgCliCommRouter::Send
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    发送pMsg指向的数据帧
// 输入:
//       参数1 :const MSG_INTRA* const pMsg
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
int CMsgCliCommRouter::Send(const MSG_INTRA* const pMsg)
{
    if (m_pMsgFrmSocket != 0)
    {
        return m_pMsgFrmSocket->Send(pMsg);
    }

    return ERR_SOCKET_NOT_PRESENT;
}


/******************************************************
// 函数名:  CMmlCommRouter::CMmlCommRouter
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    构造函数
// 输入:
//       参数1 :CCommRouterMan* const pCommRouterMan
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CMmlCommRouter::CMmlCommRouter(CCommRouterMan* const pCommRouterMan,
                                const unsigned long uAppType)
                                : CCommRouter(pCommRouterMan,
                                              uAppType)
{
    //
}


/******************************************************
// 函数名:  CMmlCommRouter::~CMmlCommRouter
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    析构函数
// 输入:
//       参数1 :void
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CMmlCommRouter::~CMmlCommRouter(void)
{
    //释放m_SocketMap中的对象
    MAP_CN_TO_PCMMLFRMSOCKET::iterator i;
    for(i = m_SocketMap.begin(); i != m_SocketMap.end(); i++)
    {
        //将该Socket的m_pCommRouter赋零，可避免在注销Socket
        //时调用已删除的m_pCommRouter对象的函数
        (*i).second->m_pCommRouter = 0;
    }
    m_SocketMap.clear();
}



/******************************************************
// 函数名:  CMmlCommRouter::AddFrmSocket
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    为本端口增加一个SOCKET连接
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
int CMmlCommRouter::AddFrmSocket(CFrmSocket* pFrmSocket)
{
    //将pFrmSocket强制转换成CMmlFrmSocket*类型
    //以m_SocketID作为键值将pFrmSocket加入到m_SocketMap中

    int nRet = ERR_MML_SOCKET_LIST_FULL;
    for(int i = MIN_CLIENT_NO;
        i <= CMmlCommRouter::MAX_CLIENT_NO;
        i++)
    {
        if(m_SocketMap.find(i) == m_SocketMap.end())
        {
            m_SocketMap[i] = (CMmlFrmSocket*)pFrmSocket;
            pFrmSocket->m_pCommRouter = this;
            nRet = ERR_SUCCESS;
            break;
        }
    }
    return nRet;
}



/******************************************************
// 函数名:  CMmlCommRouter::DelFrmSocket
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    为本端口删除一个SOCKET连接
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
void CMmlCommRouter::DelFrmSocket(CFrmSocket* pFrmSocket)
{
    //将pFrmSocket强制转换成CMmlFrmSocket*类型；
    //将pFrmSocket从m_SocketMap中删除;
    MAP_CN_TO_PCMMLFRMSOCKET::iterator i;
    for(i = m_SocketMap.begin(); i != m_SocketMap.end(); i++)
    {
        if((*i).second == (CMmlFrmSocket*)pFrmSocket)
        {
            //张玉新 2003-04-01
            CMmlFrmSocket *pSocket = i->second;
            UINT4 uIP = pSocket->GetAddrRemote().get_ip_address();
            UINT2 nClientNo = i->first;
            m_SocketMap.erase(i);
            SendMsgToMMLServer("LGO:",nClientNo,uIP);
            //over
            break;
        }
    }
}



/******************************************************
// 函数名:  CMmlCommRouter::Timer1Sec
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    秒事件处理函数
// 输入:
//       参数1 :void
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
void CMmlCommRouter::Timer1Sec(void)
{
    //遍历m_SocketMap中所有的CMmlFrmSocket对象,并调用其
    //RemoveHandler()成员函数注销超时未接收到数据的
    //CMmlFrmSocket对象；使所有CMemFrmSocket对象的
    //m_lTimeNoData加一；
    //将主动握手超时去掉，改为被动握手
    //by ldf 2004-02-12
    if(APP_MML_NORM == m_uAppType || APP_MML_WARN == m_uAppType || APP_MML_PERF == m_uAppType)
    {
        MAP_CN_TO_PCMMLFRMSOCKET::iterator i;
        for(i = m_SocketMap.begin(); i != m_SocketMap.end(); i++)
        {
            (*i).second->m_lTimeNoData ++;
            if((*i).second->m_lTimeNoData >= 300)
            {
                (*i).second->Send("SHK:;");
            }
        }
    }
}

void CMmlCommRouter::SendMsgToMMLServer(const char *szCmd, UINT2 nClientNo, UINT4 uIp)
{
    MSG_INTRA *pMsg = NULL;
    char *pCmd = NULL;
    int nLen = strlen(szCmd) + 1;
    pMsg = new(nLen) MSG_INTRA;
    pMsg->SenderPid = PT_SELF;
    pMsg->SenderMid = MT_UNKNOWN;
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = MT_MMLSERVER;
    pMsg->ClientNo = nClientNo;
    pMsg->AppType = m_uAppType;
    pMsg->CmdCode = CMD_MML;
    pMsg->IPAddr  = HTONL(uIp);
    pCmd = (char *)pMsg->Data;    
    strncpy(pCmd,szCmd,nLen);
    pCmd[nLen - 1] = 0;
    
    CMsgProcessor::SendMsg(pMsg);
}

/******************************************************
// 函数名:  CMmlCommRouter::OnReceiveEx
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    处理来自*pFrmSocket对象接收到的数据帧
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
//       参数2 :void* pFrame
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
void CMmlCommRouter::OnReceiveEx(CFrmSocket* pFrmSocket,
                                 void* pFrame)
{
    //将pFrmSocket对象的超时计数器清零;
    //从m_SocketMap中获得pFrmSocket对应的编号,将pMsg打包
    //成一个MSG_INTRA实例,调用CCommRouter::OnReceive函数
    char *pMsg = (char*)pFrame;

    MAP_CN_TO_PCMMLFRMSOCKET::iterator i;
    for(i = m_SocketMap.begin(); i != m_SocketMap.end(); i++)
    {
        if((*i).second == (CMmlFrmSocket*)pFrmSocket)
        {
            MSG_INTRA* pMsgIntra = new(strlen(pMsg) + 1) MSG_INTRA;
            if(NULL != pMsgIntra)
            {
                pMsgIntra->SenderPid = PT_OUTSIDE;
                pMsgIntra->SenderMid = MT_UNKNOWN;
                pMsgIntra->ReceiverPid = PT_SELF;
                pMsgIntra->ReceiverMid = MT_MMLSERVER;
                pMsgIntra->ClientNo = (*i).first;
                pMsgIntra->AppType = m_uAppType;
                pMsgIntra->CmdCode = CMD_MML;
                pMsgIntra->IPAddr  = HTONL(pFrmSocket->GetAddrRemote().get_ip_address());
                ACE_OS::memcpy(pMsgIntra->Data, pMsg, strlen(pMsg) + 1);
                CCommRouter::OnReceive(pMsgIntra);
            }
            break;
        }
    }

    //输入的消息帧由调用者删除
}


/******************************************************
// 函数名:  CMmlCommRouter::Send
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    发送pMsg指向的数据帧
// 输入:
//       参数1 :const MSG_INTRA* const pMsg
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
int CMmlCommRouter::Send(const MSG_INTRA* const pMsg)
{
    //判断pMsg->ClientNo是否为0,若ClientNo为0,调用m_SocketMap中所有
    //CMmlFrmSocket对象的Send函数,若所有函数的返回值为ERR_SUCCESS,
    //则返回ERR_SUCCESS,否则返回ERR_FAIL
    int nRet = ERR_SUCCESS;
    
    //如果是停掉某个Op就不必发送了
    if(pMsg->CmdCode == CMD_BREAK_OPERATOR)
    {
        MAP_CN_TO_PCMMLFRMSOCKET::iterator it = m_SocketMap.find(pMsg->ClientNo);
        if(it == m_SocketMap.end())
            return ERR_SUCCESS;
        //张玉新 2003-02-18
        CMmlFrmSocket *pSocket = it->second;
        
        pSocket->RemoveHandler();
        //结束
        return ERR_SUCCESS;
    }
    else if(pMsg->ClientNo == 0)
    {
        MAP_CN_TO_PCMMLFRMSOCKET::iterator i;
        for(i = m_SocketMap.begin(); i != m_SocketMap.end(); i++)
        {
            int nRetValue = (*i).second->Send(pMsg->Data);
            if(nRetValue != ERR_SUCCESS)
            {
                nRet = nRetValue;
            }
        }
        return nRet;
    }
    else
    //否则根据pMsg->ClientNo在m_SocketMap中查找对应的CMmlFrmSocket对
    //象,若不存在,则返回ERR_FAIL,否则调用该对象的Send函数,并返回该函
    //数的返回值
    {
        MAP_CN_TO_PCMMLFRMSOCKET::iterator it = m_SocketMap.find(pMsg->ClientNo);
        CMmlFrmSocket *pSocket = NULL;
        if( it != m_SocketMap.end())
        {
            pSocket = it->second;
            return pSocket->Send(pMsg->Data);
        }
        return ERR_SOCKET_NOT_PRESENT;
    }
}


/******************************************************
// 函数名:  CAlarmBoxRouter::CAlarmBoxRouter
// 作者:    Wangzhaoyang
// 时期:    02-07-15
// 描述:    构造函数
// 输入:
//       参数1 :CCommRouterMan* const pCommRouterMan
//       参数2 :const unsigned long uAppType
// 返回值: 
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//02-07-15       Wangzhaoyang            创建
******************************************************/
CAlarmBoxRouter::CAlarmBoxRouter(CCommRouterMan* const pCommRouterMan,
								 const unsigned long uAppType)
								 :CCommRouter(pCommRouterMan, uAppType)
{
	m_pABFrmSocket = NULL;
}

/******************************************************
// 函数名:  CAlarmBoxRouter::~CAlarmBoxRouter
// 作者:    Wangzhaoyang
// 时期:    02-07-15
// 描述:    析构函数
// 输入:
// 返回值: 
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//02-07-15       Wangzhaoyang            创建
******************************************************/
CAlarmBoxRouter::~CAlarmBoxRouter()
{
	if(NULL != m_pABFrmSocket)
	{
		m_pABFrmSocket->m_pCommRouter = NULL;
		m_pABFrmSocket = NULL;
	}
}

/******************************************************
// 函数名:  CAlarmBoxRouter::AddFrmSocket
// 作者:    Wangzhaoyang
// 时期:    02-07-15
// 描述:    为本端口增加一个SOCKET连接
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//02-07-15       Wangzhaoyang            创建
******************************************************/
int CAlarmBoxRouter::AddFrmSocket(CFrmSocket* pFrmSocket)
{
	if(NULL == m_pABFrmSocket)
	{
		m_pABFrmSocket = (CABFrmSocket*)pFrmSocket;
	}
	else //删除原先的socket，重新建立
	{
        m_pABFrmSocket->m_pCommRouter = NULL;
		m_pABFrmSocket->RemoveHandler();
		m_pABFrmSocket = (CABFrmSocket*)pFrmSocket;
	}

	pFrmSocket->m_pCommRouter = this;
	return 0;
}

/******************************************************
// 函数名:  CAlarmBoxRouter::DelFrmSocket
// 作者:    Wangzhaoyang
// 时期:    02-07-15
// 描述:    为本端口删除一个SOCKET连接
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//02-07-15       Wangzhaoyang            创建
******************************************************/
void CAlarmBoxRouter::DelFrmSocket(CFrmSocket* pFrmSocket)
{
	ASSERT(pFrmSocket == m_pABFrmSocket);
	ASSERT(pFrmSocket != NULL);
	
    m_pABFrmSocket->m_pCommRouter = NULL;
	m_pABFrmSocket = NULL;
}

/******************************************************
// 函数名:  CAlarmBoxRouter::OnReceive
// 作者:    Wangzhaoyang
// 时期:    02-07-15
// 描述:    处理来自*pFrmSocket对象接收到的数据帧
// 输入:
//       参数1 :void* pFrame
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//02-07-15       Wangzhaoyang            创建
******************************************************/
void CAlarmBoxRouter::OnReceiveEx(CFrmSocket* pFrmSocket,void* pFrame)
{

	SAppConvertFrame* pMsg = (SAppConvertFrame*)pFrame;
  
	MSG_INTRA* pMsgIntra = NULL;
	if(pMsg->byCmd == ALARM_CMD_CONVERT_TO_BAU)  //convert到BAU的告警传输
	{
		if(pMsg->byError == 0x64)
		{
			pMsg->byCmd = ALARM_CMD_BAU_TO_CONVERT;
			m_pABFrmSocket->Send(pMsg);
			return;
		}

		int nLen = ntohs(pMsg->wLen);
		pMsgIntra = new(nLen) MSG_INTRA;
		pMsgIntra->CmdCode = ALARM_CMD_CONVERT_TO_BAU;
		pMsgIntra->SenderPid = PT_OUTSIDE;
		pMsgIntra->SenderMid = MT_UNKNOWN;
		pMsgIntra->ReceiverPid = PT_OM;
		pMsgIntra->ReceiverMid = MT_ALARM;
		pMsgIntra->AppType = APP_ALARMBOX;
		memcpy(pMsgIntra->Data, pMsg->Data(), pMsgIntra->Len);
		CCommRouter::OnReceive(pMsgIntra);
	}
}

/******************************************************
// 函数名:  CAlarmBoxRouter::Send
// 作者:    Wangzhaoyang
// 时期:    02-07-15
// 描述:    将从告警模块发来的数据转发到convert
// 输入:
//       参数1 :const MSG_INTRA* const pMsg
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//02-07-15       Wangzhaoyang            创建
******************************************************/
int CAlarmBoxRouter::Send(const MSG_INTRA* const pMsg)
{
	ASSERT(pMsg->CmdCode == ALARM_CMD_BAU_TO_CONVERT);

	SAppConvertFrame* pFrame = new(pMsg->Len) SAppConvertFrame;
    if(NULL == pFrame)
    {
        TRACE(MTS_COMM, S_ALARM_BOX_FRAME_EXCEED, pMsg->Len);
        return ERR_FAIL;
    }

	pFrame->byCmd = ALARM_CMD_BAU_TO_CONVERT;
	memcpy(pFrame->Data(), pMsg->Data, pMsg->Len);
	
    int nRet = ERR_SOCKET_NOT_PRESENT;
	if (NULL != m_pABFrmSocket)
	{
		nRet = m_pABFrmSocket->Send(pFrame);
	}

    delete pFrame;
    return nRet;
}


///////////////////////////////////////////////////////////////////////////////
//GTP'计费接口增加 Z30971 2004/06/10

//对GTP'协议TCP方式的客户端的抽象
//构造函数
CGTPTcpCliCommRouter::CGTPTcpCliCommRouter(CCommRouterMan* const pCommRouterMan,
                                           const unsigned long uAppType,
                                           PROCESS_TYPE const nProcessType)
                                           :CCommRouter(pCommRouterMan, uAppType)
{
    m_nProcessType = nProcessType;
    m_pGtpTcpCliSocket = 0;
    m_nReConnectTimer = 0;

    //与BS进行连接
    Connect();
}

//析构函数
CGTPTcpCliCommRouter::~CGTPTcpCliCommRouter(void)
{
    if(m_pGtpTcpCliSocket)
    {
        //将该Socket的m_pCommRouter赋零，可避免在注销Socket
        //时调用已删除的m_pCommRouter对象的函数
        m_pGtpTcpCliSocket->m_pCommRouter = 0;
        m_pGtpTcpCliSocket = NULL;
    }
}

//删除GTP TCP SOCKET
void CGTPTcpCliCommRouter::DelFrmSocket(CFrmSocket* pFrmSocket)
{
    ASSERT(m_pGtpTcpCliSocket == pFrmSocket);

    if(m_pGtpTcpCliSocket && m_pGtpTcpCliSocket == pFrmSocket)
    {
        m_pGtpTcpCliSocket->m_pCommRouter = 0;
        m_pGtpTcpCliSocket = NULL;
    }
}

//连接BS
int CGTPTcpCliCommRouter::Connect(void)
{
    //使用ACE_SOCK_Connector连接BS进程, 即GTP TCP服务
    //器端，若连接成功，则创建一个Socket对象，将该
    //对象的指针赋值给m_pGtpTcpCliSocket.

    
    //从配置文件中读取GTP Svr的IP地址
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    pIniFile->Open();

    char szSvrIp[20];
    pIniFile->GetString(CFG_SEC_BS, 
                        CFG_BS_KEY_GTP_SVR_IP, 
                        CFG_BS_KEY_GTP_SVR_IP_DEFAULT,
                        szSvrIp, 
                        sizeof(szSvrIp));
    
    //从配置文件中读取GTP Svr的端口号，缺省值为3386
    int nGTPSvrPort = pIniFile->GetInt(CFG_SEC_BS, 
                                       CFG_BS_KEY_GTP_SVR_PORT,
                                       CFG_BS_KEY_GTP_SVR_PORT_DEFAULT
                                       );

    //读取本地客户端的IP地址
    char szLocalIp[20];
    pIniFile->GetString(CFG_SEC_BS, 
                        CFG_BS_KEY_GTP_CLI_IP, 
                        CFG_BS_KEY_GTP_CLI_IP_DEFAULT,
                        szLocalIp, 
                        sizeof(szLocalIp)
                        );
    
    //读取本地客户端的的端口号，缺省值为3386
    int nLocalPort = pIniFile->GetInt(CFG_SEC_BS, 
                                      CFG_BS_KEY_GTP_CLI_PORT,
                                      CFG_BS_KEY_GTP_CLI_PORT_DEFAULT
                                      );
    
    pIniFile->Close();
    delete pIniFile;

    //与BS进行连接
    ACE_SOCK_Connector Connector;
    ACE_SOCK_Stream *pstream = new ACE_SOCK_Stream();
    ACE_INET_Addr remote_sap;
    remote_sap.set(nGTPSvrPort, szSvrIp);

    ACE_INET_Addr local_sap;
    local_sap.set(nLocalPort,szLocalIp);

    ACE_Time_Value timeout(2);
    int nRet = ERR_FAIL;

    if(Connector.connect(*pstream, remote_sap, &timeout, local_sap) != 0)
    //连接不成功
    {
        TRACE(MTS_COMM, S_COMM_CONNECT_BS, szSvrIp, nGTPSvrPort);
        delete pstream;
        return nRet;
    }

    //连接成功后
    m_pGtpTcpCliSocket = new CGtpTcpCliSocket(pstream);
    m_pGtpTcpCliSocket->m_pCommRouter = this;

    //注册Socket的读写事件
    nRet = RegisterHandle(m_pGtpTcpCliSocket,
                          ACE_Event_Handler::READ_MASK |
                          ACE_Event_Handler::WRITE_MASK);
    if(nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_CNT_BS_REG_ERR, nRet);

        m_pGtpTcpCliSocket->m_pCommRouter = 0;
        delete m_pGtpTcpCliSocket;
        m_pGtpTcpCliSocket = NULL;
        return nRet;
    }

    
    return ERR_SUCCESS;
}

//一秒定时器
void CGTPTcpCliCommRouter::Timer1Sec(void)
{
    if (m_pGtpTcpCliSocket == 0)
    {
        if (m_nReConnectTimer >= 5)
        {
            //重连计数器清零
            m_nReConnectTimer = 0;

            //与BS进行重连
            Connect();
        }
        else
        {
            m_nReConnectTimer++;
        }
    }
}

//发送数据
int CGTPTcpCliCommRouter::Send(const MSG_INTRA* const pMsg)
{
    if (m_pGtpTcpCliSocket != 0)
    {
        return m_pGtpTcpCliSocket->Send(pMsg->Data);
    }

    return ERR_SOCKET_NOT_PRESENT;
}

//对GTP'协议UDP方式的客户端的抽象
//构造函数
CGTPUdpCliCommRouter::CGTPUdpCliCommRouter(CCommRouterMan* const pCommRouterMan,
                                           const unsigned long uAppType,
                                           PROCESS_TYPE const nProcessType)
                              : CCommRouter(pCommRouterMan,
                                            uAppType)
{
    m_nProcessType = nProcessType;
    m_pGtpUdpCliSocket = 0;

    Init();
}

//析构函数
CGTPUdpCliCommRouter::~CGTPUdpCliCommRouter()
{
    if(m_pGtpUdpCliSocket)
    {
        //将该Socket的m_pCommRouter赋零，可避免在注销Socket
        //时调用已删除的m_pCommRouter对象的函数
        m_pGtpUdpCliSocket->m_pCommRouter = 0;
        m_pGtpUdpCliSocket = NULL;
    }
}

//删除SOCKET
void CGTPUdpCliCommRouter::DelGtpSocket(CGtpUdpCliSocket* pGtpSocket)
{
    ASSERT(m_pGtpUdpCliSocket == pGtpSocket);

    if(m_pGtpUdpCliSocket && m_pGtpUdpCliSocket == pGtpSocket)
    {
        m_pGtpUdpCliSocket->m_pCommRouter = 0;
        m_pGtpUdpCliSocket = NULL;
    }
}

//初始化SOCKET
int CGTPUdpCliCommRouter::Init()
{
    //从配置文件中读取GTP' 本地的IP地址
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    pIniFile->Open();

    char szLocalIp[20];
    pIniFile->GetString(CFG_SEC_BS, 
                        CFG_BS_KEY_GTP_CLI_IP, 
                        CFG_BS_KEY_GTP_CLI_IP_DEFAULT,
                        szLocalIp, 
                        sizeof(szLocalIp));
    
    //从配置文件中读取GTP' 本地的端口号，缺省值为3386
    int nLocalPort = pIniFile->GetInt(CFG_SEC_BS, 
                                       CFG_BS_KEY_GTP_CLI_PORT,
                                       CFG_BS_KEY_GTP_CLI_PORT_DEFAULT);
    pIniFile->Close();
    delete pIniFile;

    m_pGtpUdpCliSocket = new CGtpUdpCliSocket(this);
    if(m_pGtpUdpCliSocket->Open(szLocalIp, nLocalPort) != ERR_SUCCESS)
    {
        delete m_pGtpUdpCliSocket;
        m_pGtpUdpCliSocket = NULL;

        return ERR_BIND_PORT;
    }

    //注册Socket事件
    int nRet = RegisterHandle(m_pGtpUdpCliSocket,
                          ACE_Event_Handler::READ_MASK);
    if(nRet != ERR_SUCCESS)
    {
        delete m_pGtpUdpCliSocket;
        m_pGtpUdpCliSocket = NULL;
        return nRet;
    }
   
    return ERR_SUCCESS;
}

//发送数据
int CGTPUdpCliCommRouter::Send(const MSG_INTRA* const pMsg)
{
    if (m_pGtpUdpCliSocket != 0)
    {
        return m_pGtpUdpCliSocket->Send(pMsg);
    }

    return ERR_SOCKET_NOT_PRESENT;
}

//秒事件
void CGTPUdpCliCommRouter::Timer1Sec(void)
{
}

