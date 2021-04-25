#ifndef __UDP_LINK_H__
#define __UDP_LINK_H__

#include "heartbeat_link.h"

class CClusterNetwork;
class CUdpSocket;

class CUdpLink : public CHeartBeatLink
{
/*
    ��Ա����
*/
public:

    CUdpLink(const char* const szLocalIP, 
             const char* const szPeerIP, 
             UINT nPort, 
             CClusterNetwork* pNetwork, 
             int nLinkIndex, 
             const char* const szLinkName = 0);

    virtual ~CUdpLink();

    //����������UDP�˿�
    virtual int Create();
    //��Զ˷�����Ϣ
    int Send(SClusterFrame* pFrame);

/*
    ��Ա����
*/
protected:
    //�첽Socket�����ָ��
    CUdpSocket*     m_pUdpSocket;

#define IP_LENGTH   16
    //��ű�������IP���ַ���
    char            m_szLocalIP[IP_LENGTH];

    //��ŶԶ�����IP���ַ���
    char            m_szPeerIP[IP_LENGTH];

    //������ʹ�õĶ˿�
    UINT            m_uSockPort;

};

#endif //__UDP_LINK_H__
