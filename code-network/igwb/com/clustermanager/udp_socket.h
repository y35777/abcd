#ifndef __CLUSTER_UDP_SOCKET_H__
#define __CLUSTER_UDP_SOCKET_H__

#include "ace/Event_Handler.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram.h"

class CUdpLink;

class CUdpSocket : public ACE_Event_Handler
{
public:
    CUdpSocket(CUdpLink* pUdpLink);
    virtual ~CUdpSocket();

    //创建UDP端口
    int Open(const char* const szLocalAddr, 
             const char* const szPeerAddr, 
             UINT nPort);

    //向对端发送消息
    int Send(SClusterFrame* pFrame);
    //处理Socket输入事件
    virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);
    //处理Socket关闭事件
    virtual int handle_close(ACE_HANDLE fd, 
                             ACE_Reactor_Mask close_mask);
    //ACE_Reactor通过回调本函数，以获得Socket的句柄
    virtual ACE_HANDLE get_handle() const;  

/*
    成员变量
*/
protected:

    //本端地址
    ACE_INET_Addr   m_LocalAddr;

    //对端地址
    ACE_INET_Addr   m_PeerAddr;

    //CUdpLink对象的指针
    CUdpLink*       m_pUdpLink;

    //UDP数据报Socket对象
    ACE_SOCK_Dgram  m_SocketDgram;

    //用于接收UDP数据报的缓冲区,该缓冲区的长度为64KB
#define UDP_INPUT_BUFFER_MAXLENGTH (64 * 1024)
    char m_RecvBuf[UDP_INPUT_BUFFER_MAXLENGTH];
};

#endif //__CLUSTER_UDP_SOCKET_H__
