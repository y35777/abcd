#ifndef __UDP_LINK_H__
#define __UDP_LINK_H__

#include "heartbeat_link.h"

class CClusterNetwork;
class CUdpSocket;

class CUdpLink : public CHeartBeatLink
{
/*
    成员函数
*/
public:

    CUdpLink(const char* const szLocalIP, 
             const char* const szPeerIP, 
             UINT nPort, 
             CClusterNetwork* pNetwork, 
             int nLinkIndex, 
             const char* const szLinkName = 0);

    virtual ~CUdpLink();

    //创建心跳的UDP端口
    virtual int Create();
    //向对端发送消息
    int Send(SClusterFrame* pFrame);

/*
    成员变量
*/
protected:
    //异步Socket对象的指针
    CUdpSocket*     m_pUdpSocket;

#define IP_LENGTH   16
    //存放本地心跳IP的字符串
    char            m_szLocalIP[IP_LENGTH];

    //存放对端心跳IP的字符串
    char            m_szPeerIP[IP_LENGTH];

    //心跳所使用的端口
    UINT            m_uSockPort;

};

#endif //__UDP_LINK_H__
