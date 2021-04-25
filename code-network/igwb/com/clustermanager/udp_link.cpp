#include "../include/base_type.h"
#include <assert.h>
#include "cluster_struct.h"
#include "cluster_network.h"
#include "udp_link.h"
#include "udp_socket.h"


/********************************************************
* ����: CUdpLink::CUdpLink
* ����: ���캯��
* ����: const char* const szLocalIP  : ���˵�ַ
*       const char* const szPeerIP   : �Զ˵�ַ
*       UINT nPort                   : �˿ں�
*       CClusterNetwork* pNetwork    : CClusterNetwork�����ָ��
*       int nLinkIndex               : �������к�
*       const char* const szLinkName : ������
* ���: 
* ����: 
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
* ����: CUdpLink::~CUdpLink
* ����: ��������
* ����: 
* ���: 
* ����: 
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
* ����: CUdpLink::Create
* ����: ����������UDP�˿�
* ����: 
* ���: 
* ����: int 
********************************************************/
int CUdpLink::Create()
{
    int nRet = ERR_SUCCESS;

    if(NULL == m_pUdpSocket)
    {
        m_pUdpSocket = new CUdpSocket(this);
    }

    //�򿪶˿�
    nRet = m_pUdpSocket->Open(m_szLocalIP, m_szPeerIP, m_uSockPort);
    if(ERR_SUCCESS == nRet)
    {
        //����m_pNetwork��RegisterHandle������ע��m_pUdpSocket���������͹ر��¼�
        assert(NULL != m_pNetwork);
        nRet = m_pNetwork->RegisterHandle(m_pUdpSocket, 
                                          ACE_Event_Handler::READ_MASK);
    }
    
    return nRet;
}

/********************************************************
* ����: CUdpLink::Send
* ����: ��Զ˷�����Ϣ
* ����: SClusterFrame* pFrame : ˫����Ϣ֡�ṹ��ָ��
* ���: 
* ����: int �����Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
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

