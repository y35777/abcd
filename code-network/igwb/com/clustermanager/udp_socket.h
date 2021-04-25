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

    //����UDP�˿�
    int Open(const char* const szLocalAddr, 
             const char* const szPeerAddr, 
             UINT nPort);

    //��Զ˷�����Ϣ
    int Send(SClusterFrame* pFrame);
    //����Socket�����¼�
    virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);
    //����Socket�ر��¼�
    virtual int handle_close(ACE_HANDLE fd, 
                             ACE_Reactor_Mask close_mask);
    //ACE_Reactorͨ���ص����������Ի��Socket�ľ��
    virtual ACE_HANDLE get_handle() const;  

/*
    ��Ա����
*/
protected:

    //���˵�ַ
    ACE_INET_Addr   m_LocalAddr;

    //�Զ˵�ַ
    ACE_INET_Addr   m_PeerAddr;

    //CUdpLink�����ָ��
    CUdpLink*       m_pUdpLink;

    //UDP���ݱ�Socket����
    ACE_SOCK_Dgram  m_SocketDgram;

    //���ڽ���UDP���ݱ��Ļ�����,�û������ĳ���Ϊ64KB
#define UDP_INPUT_BUFFER_MAXLENGTH (64 * 1024)
    char m_RecvBuf[UDP_INPUT_BUFFER_MAXLENGTH];
};

#endif //__CLUSTER_UDP_SOCKET_H__
