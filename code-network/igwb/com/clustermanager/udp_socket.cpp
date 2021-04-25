#include <assert.h>
#include "../include/base_type.h"
#include "cluster_struct.h"
#include "cluster_network.h"
#include "udp_socket.h"
#include "udp_link.h"
#include "cluster_mutex.h"


/********************************************************
* 名称: CUdpSocket::CUdpSocket
* 功能: 构造函数
* 输入: pUdpLink : CUdpLink对象的指针
* 输出: 
* 返回: 
********************************************************/
CUdpSocket::CUdpSocket(CUdpLink* pUdpLink)
{
    assert(NULL != pUdpLink);

    m_pUdpLink = pUdpLink;
}

/********************************************************
* 名称: CUdpSocket::~CUdpSocket
* 功能: 析构函数
* 输入: 
* 输出: 
* 返回: 
********************************************************/
CUdpSocket::~CUdpSocket()
{
    m_pUdpLink = NULL;
}

/********************************************************
* 名称: CUdpSocket::Open
* 功能: 创建UDP端口
* 输入: const char* const szLocalAddr : 本端地址
*       const char* const szPeerAddr  : 对端地址
*       UINT nPort                    : 端口号
* 输出: 
* 返回: int 创建UDP端口是否成功，0表示成功，非零值表示失败
********************************************************/
int CUdpSocket::Open(const char* const szLocalAddr, 
                     const char* const szPeerAddr, 
                     UINT nPort)
{
    //利用输入的本端/对端地址及端口号，初始化成员变量m_LocalAddr及m_PeerAddr
    m_LocalAddr.set(nPort, szLocalAddr);
    m_PeerAddr.set(nPort, szPeerAddr);

    //打开UDP端口
    return m_SocketDgram.open(m_LocalAddr);
}

/********************************************************
* 名称: CUdpSocket::Send
* 功能: 向对端发送消息
* 输入: SClusterFrame* pFrame : 双机消息帧结构的指针
* 输出: 
* 返回: int 发送是否成功，0表示成功，非零值表示失败
********************************************************/
int CUdpSocket::Send(SClusterFrame* pFrame)
{
    int nLen = pFrame->nLen + CLUSTER_FRAME_HEADLENGTH;
    int nSendBytes = m_SocketDgram.send((void*)pFrame, 
                                        nLen,
                                        m_PeerAddr);

    return (nSendBytes == nLen) ? ERR_SUCCESS : ERR_FAIL;
}

/********************************************************
* 名称: CUdpSocket::handle_input
* 功能: 处理Socket输入事件
* 输入: ACE_HANDLE fd : 事件产生者的句柄
* 输出: 
* 返回: int 是否需要注销输入事件，0表示不需要，非零值表示需要注销
********************************************************/
int CUdpSocket::handle_input(ACE_HANDLE fd)
{
    CClusterMutex::Instance()->Acquire();

    ACE_INET_Addr AddrPeer;
    int nRetBytes = m_SocketDgram.recv(m_RecvBuf,
                                       UDP_INPUT_BUFFER_MAXLENGTH,
                                       AddrPeer);

    if ((nRetBytes >= int(CLUSTER_FRAME_HEADLENGTH)) && (NULL != m_pUdpLink))
    {
        SClusterFrame* pFrame = (SClusterFrame*)new BYTE[nRetBytes];
        memcpy((BYTE*)pFrame, m_RecvBuf, nRetBytes);
        m_pUdpLink->OnReceive(pFrame);
    }

    CClusterMutex::Instance()->Release();

    return 0;
}

/********************************************************
* 名称: CUdpSocket::handle_close
* 功能: 处理Socket关闭事件
* 输入: ACE_HANDLE fd : 事件产生者的句柄
*       ACE_Reactor_Mask close_mask : 将被关闭的事件
* 输出: 
* 返回: int 是否需要注销关闭事件，0表示不需要，非零值表示需要注销
********************************************************/
int CUdpSocket::handle_close(ACE_HANDLE fd, 
                             ACE_Reactor_Mask close_mask)
{
    //置本心跳对象无效
    if(NULL != m_pUdpLink)
    {
        m_pUdpLink->Disable();
    }

    reactor(NULL);
    return 0;
}

/********************************************************
* 名称: CUdpSocket::get_handle
* 功能: ACE_Reactor通过回调本函数，以获得Socket的句柄
* 输入: 
* 输出: 
* 返回: ACE_HANDLE 返回Socket的句柄
********************************************************/
ACE_HANDLE CUdpSocket::get_handle() const  
{
    return m_SocketDgram.get_handle();
}