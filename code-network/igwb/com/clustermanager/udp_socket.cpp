#include <assert.h>
#include "../include/base_type.h"
#include "cluster_struct.h"
#include "cluster_network.h"
#include "udp_socket.h"
#include "udp_link.h"
#include "cluster_mutex.h"


/********************************************************
* ����: CUdpSocket::CUdpSocket
* ����: ���캯��
* ����: pUdpLink : CUdpLink�����ָ��
* ���: 
* ����: 
********************************************************/
CUdpSocket::CUdpSocket(CUdpLink* pUdpLink)
{
    assert(NULL != pUdpLink);

    m_pUdpLink = pUdpLink;
}

/********************************************************
* ����: CUdpSocket::~CUdpSocket
* ����: ��������
* ����: 
* ���: 
* ����: 
********************************************************/
CUdpSocket::~CUdpSocket()
{
    m_pUdpLink = NULL;
}

/********************************************************
* ����: CUdpSocket::Open
* ����: ����UDP�˿�
* ����: const char* const szLocalAddr : ���˵�ַ
*       const char* const szPeerAddr  : �Զ˵�ַ
*       UINT nPort                    : �˿ں�
* ���: 
* ����: int ����UDP�˿��Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CUdpSocket::Open(const char* const szLocalAddr, 
                     const char* const szPeerAddr, 
                     UINT nPort)
{
    //��������ı���/�Զ˵�ַ���˿ںţ���ʼ����Ա����m_LocalAddr��m_PeerAddr
    m_LocalAddr.set(nPort, szLocalAddr);
    m_PeerAddr.set(nPort, szPeerAddr);

    //��UDP�˿�
    return m_SocketDgram.open(m_LocalAddr);
}

/********************************************************
* ����: CUdpSocket::Send
* ����: ��Զ˷�����Ϣ
* ����: SClusterFrame* pFrame : ˫����Ϣ֡�ṹ��ָ��
* ���: 
* ����: int �����Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
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
* ����: CUdpSocket::handle_input
* ����: ����Socket�����¼�
* ����: ACE_HANDLE fd : �¼������ߵľ��
* ���: 
* ����: int �Ƿ���Ҫע�������¼���0��ʾ����Ҫ������ֵ��ʾ��Ҫע��
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
* ����: CUdpSocket::handle_close
* ����: ����Socket�ر��¼�
* ����: ACE_HANDLE fd : �¼������ߵľ��
*       ACE_Reactor_Mask close_mask : �����رյ��¼�
* ���: 
* ����: int �Ƿ���Ҫע���ر��¼���0��ʾ����Ҫ������ֵ��ʾ��Ҫע��
********************************************************/
int CUdpSocket::handle_close(ACE_HANDLE fd, 
                             ACE_Reactor_Mask close_mask)
{
    //�ñ�����������Ч
    if(NULL != m_pUdpLink)
    {
        m_pUdpLink->Disable();
    }

    reactor(NULL);
    return 0;
}

/********************************************************
* ����: CUdpSocket::get_handle
* ����: ACE_Reactorͨ���ص����������Ի��Socket�ľ��
* ����: 
* ���: 
* ����: ACE_HANDLE ����Socket�ľ��
********************************************************/
ACE_HANDLE CUdpSocket::get_handle() const  
{
    return m_SocketDgram.get_handle();
}