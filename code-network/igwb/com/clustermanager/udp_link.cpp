#include "../include/base_type.h"
#include <assert.h>
#include "cluster_struct.h"
#include "cluster_network.h"
#include "udp_link.h"
#include "udp_socket.h"


/********************************************************
* 名称: CUdpLink::CUdpLink
* 功能: 构造函数
* 输入: const char* const szLocalIP  : 本端地址
*       const char* const szPeerIP   : 对端地址
*       UINT nPort                   : 端口号
*       CClusterNetwork* pNetwork    : CClusterNetwork对象的指针
*       int nLinkIndex               : 心跳序列号
*       const char* const szLinkName : 心跳名
* 输出: 
* 返回: 
********************************************************/
CUdpLink::CUdpLink(const char* const szLocalIP, 
                   const char* const szPeerIP, 
                   UINT nPort, 
                   CClusterNetwork* pNetwork, 
                   int nLinkIndex, 
                   const char* const szLinkName):
CHeartBeatLink(pNetwork, nLinkIndex, szLinkName)
{
    m_pUdpSocket = NULL;
    m_nLinkType  = LT_UDP;
    m_uSockPort  = nPort;
    strncpy(m_szLocalIP, szLocalIP, IP_LENGTH);
    m_szLocalIP[IP_LENGTH - 1] = '\0';

    strncpy(m_szPeerIP, szPeerIP, IP_LENGTH);
    m_szPeerIP[IP_LENGTH - 1] = '\0';

    if((NULL == szLinkName) || ('\0' == szLinkName[0]))
    {
        m_strLinkName = szLocalIP;
    }
}

/********************************************************
* 名称: CUdpLink::~CUdpLink
* 功能: 析构函数
* 输入: 
* 输出: 
* 返回: 
********************************************************/
CUdpLink::~CUdpLink()
{
    if(NULL != m_pUdpSocket)
    {
        delete m_pUdpSocket;
        m_pUdpSocket = NULL;
    }
}

/********************************************************
* 名称: CUdpLink::Create
* 功能: 创建心跳的UDP端口
* 输入: 
* 输出: 
* 返回: int 
********************************************************/
int CUdpLink::Create()
{
    int nRet = ERR_SUCCESS;

    if(NULL == m_pUdpSocket)
    {
        m_pUdpSocket = new CUdpSocket(this);
    }

    //打开端口
    nRet = m_pUdpSocket->Open(m_szLocalIP, m_szPeerIP, m_uSockPort);
    if(ERR_SUCCESS == nRet)
    {
        //调用m_pNetwork的RegisterHandle函数，注册m_pUdpSocket对象的输入和关闭事件
        assert(NULL != m_pNetwork);
        nRet = m_pNetwork->RegisterHandle(m_pUdpSocket, 
                                          ACE_Event_Handler::READ_MASK);
    }
    
    return nRet;
}

/********************************************************
* 名称: CUdpLink::Send
* 功能: 向对端发送消息
* 输入: SClusterFrame* pFrame : 双机消息帧结构的指针
* 输出: 
* 返回: int 发送是否成功，0表示成功，非零值表示失败
********************************************************/
int CUdpLink::Send(SClusterFrame* pFrame)
{
    int nRet = ERR_FAIL;

    if(NULL != m_pUdpSocket)
    {
        nRet = m_pUdpSocket->Send(pFrame);
    }
    
    return nRet;
}

