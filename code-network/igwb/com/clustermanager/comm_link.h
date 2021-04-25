#ifndef __COMM_LINK_H__
#define __COMM_LINK_H__

#include "comm_port.h"
#include "heartbeat_link.h"

class CClusterNetwork;

class CCommLink : public CHeartBeatLink
{
/*
    成员函数
*/
public:

    CCommLink(UINT uPort,
              CClusterNetwork* pNetwork,
              int nLinkIndex,
              const char* const szLinkName = 0);
    virtual ~CCommLink();

    //创建心跳的COM端口
    virtual int Create();
    //向对端发送消息
    virtual int Send(SClusterFrame* pFrame);
    //秒定时器
    virtual void Timer1Sec();

/*
    成员变量
*/
protected:
    //串口通信对象
    CCommPort     m_CCommPort;

    //串口通讯口号
    UINT          m_uCommPort;
};

#endif //__COMM_LINK_H__
