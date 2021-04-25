#include "comm_struct.h"
#include "comm_layer.h"
#include "../include/toolbox.h"

/******************************************************
// ������:  CCommRouter::CCommRouter
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ���캯��
// ����:
//       ����1 :CCommRouterMan* const pCommRouterMan
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
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
// ������: 	CCommRouter::~CCommRouter
// ����:    Wangfeng
// ʱ��:    01-10-20
// ����:    ��������
// ����:
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
CCommRouter::~CCommRouter()
{
    m_pCommRouterMan = 0;
}


/******************************************************
// ������:  CCommRouter::OnReceive
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��CCommRouterMan�����ύ�ӵͲ����·�Ͻ�����������Ϣ
// ����:
//       ����1 :MSG_INTRA* pMsg
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CCommRouter::OnReceive(MSG_INTRA* pMsg)
{
    //�����ύ��Ϣ
    m_pCommRouterMan->OnReceive(pMsg);
}


/******************************************************
// ������: 	CCommRouter::AddFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-20
// ����:    Ϊ���˿�����һ��SOCKET����
// ����:
//       ����1 :CFrmSocket* pFrmSocket
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
int CCommRouter::AddFrmSocket(CFrmSocket* pFrmSocket)
{
    return 0;
}


/******************************************************
// ������: 	CCommRouter::DelFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-20
// ����:    Ϊ���˿�ע��һ��SOCKET����
// ����:
//       ����1 :CFrmSocket* pFrmSocket
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
void CCommRouter::DelFrmSocket(CFrmSocket* pFrmSocket)
{
    //
}


/******************************************************
// ������: 	CCommRouter::Timer1Sec
// ����:    Wangfeng
// ʱ��:    01-10-20
// ����:    ���¼�������
// ����:
//       ����1 :void
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
void CCommRouter::Timer1Sec(void)
{
    //
}


/******************************************************
// ������:  CCommRouter::OnReceive
// ����:    Wangfeng
// ʱ��:    01-10-10
// ����:    ��������*pFrmSocket������յ�������֡
// ����:
//       ����1 :CFrmSocket* pFrmSocket
//       ����2 :void* pFrame
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-10      Wangfeng
******************************************************/
void CCommRouter::OnReceiveEx(CFrmSocket* pFrmSocket,
                              void* pFrame)
{
    //�����ύ��Ϣ
    m_pCommRouterMan->OnReceive((MSG_INTRA*) pFrame);
}


/******************************************************
// ������:  CCommRouter::RegisterHandle
// ����:    Wangfeng
// ʱ��:    01-10-10
// ����:    ע��CSocketImpl����Ķ���
// ����:
//       ����1 :CSocketImpl* pSocket
//       ����2 :ACE_Reactor_Mask mask
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-10      Wangfeng
******************************************************/
int CCommRouter::RegisterHandle(CSocketImpl* pSocket,
                                ACE_Reactor_Mask mask)
{
    //����*m_pCommRouterMan�����RegisterHandle������
    //�����ظú����ķ���ֵ
    return m_pCommRouterMan->RegisterHandle(pSocket, mask);
}


/******************************************************
// ������:  CCommRouterMan::CCommRouterMan
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ���캯��
// ����:
//       ����1 :CCommLayer* const pCommLayer
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
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
// ������:  CCommRouterMan::~CCommRouterMan
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��������
// ����:
//       ����1 :void
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
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

    //�ͷ�m_CommRouterMap������Ԫ��
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
// ������:  CCommRouterMan::RegisterCommRouter
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ע��CCommRouter����Ķ���
// ����:
//       ����1 :CCommRouter* pCommRouter
//       ����2 :unsigned long uAppType
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CCommRouterMan::RegisterCommRouter(CCommRouter* pCommRouter,
                                       unsigned long uAppType)
{
    if (uAppType == APP_MSG)
    //��Ӧ������Ϊ��Ϣ��ת�ķ������˻�ͻ���
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
            //��pCommRouter���뵽m_CommRouerMap��
            m_CommRouterMap[uAppType] = pCommRouter;
            return ERR_SUCCESS;
        }
    }
}


/******************************************************
// ������:  CCommRouterMan::AddFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ΪӦ������ΪuAppType�Ķ˿�����һ��������
// ����:
//       ����1 :CFrmSocket* pFrmSocket
//       ����2 :unsigned long uAppType
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CCommRouterMan::AddFrmSocket(CFrmSocket* pFrmSocket,
                                 unsigned long uAppType)
{
    if (uAppType == APP_MSG)
    //��Ӧ������Ϊ��Ϣ��ת�ķ������˻�ͻ���
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
        //�ж���m_CommRouterMap���Ƿ���ڼ�ֵΪuAppType��CCommRouter
        //����ָ�룬�������ڣ��򷵻�ERR_COMMROUTER_NOT_FOUND,������ø�
        //�����AddFrmSocket������������ERR_SUCCESS
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
// ������:  CCommRouterMan::Send
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ����*pMsg֡ͷ�е������Ϣ��ѡ���ʵ���CCommRouter
//          ���󣬲����øö����Send����
// ����:
//       ����1 :const MSG_INTRA* const pMsg
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CCommRouterMan::Send(const MSG_INTRA* const pMsg)
{
    if (pMsg->ReceiverPid == PT_OUTSIDE)
    //����ϢҪ�����Ʒ�����ϵͳ֮��
    {
        //�ж���m_CommRouterMap���Ƿ���ڼ�ֵΪpMsg->AppType��
        //CCommRouter�����ָ�룬�������ڣ��򷵻�ERR_FAIL������
        //���øö����Send�����������ظú����ķ���ֵ
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
// ������:  CCommRouterMan::OnReceive
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ����Ϊͨ�Ų��CCommLayer�����ύ�²��
//          CCommRouter��������ύ��������Ϣ
// ����:
//       ����1 :MSG_INTRA* pMsg
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CCommRouterMan::OnReceive(MSG_INTRA* pMsg)
{
    //��ͨ�Ų��ύ��Ϣ
    m_pCommLayer->OnReceive(pMsg);
}


/******************************************************
// ������:  CCommRouterMan::Timer1Sec
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    �붨ʱ���¼�
// ����:
//       ����1 :void
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
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
// ������:  CCommRouterMan::RegisterHandle
// ����:    Wangfeng
// ʱ��:    01-10-10
// ����:    ע��CSocketImpl����Ķ���
// ����:
//       ����1 :CSocketImpl* pSocket
//       ����2 :ACE_Reactor_Mask mask
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-10      Wangfeng
******************************************************/
int CCommRouterMan::RegisterHandle(CSocketImpl* pSocket,
                                   ACE_Reactor_Mask mask)
{
    //����*m_pCommLayer�����RegisterHandle�����������ظú����ķ���ֵ
    return m_pCommLayer->RegisterHandle(pSocket, mask);
}


/******************************************************
// ������:  CCommRouter
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ���캯��
// ����:
//       ����1 :pCommRouterMan
// ����ֵ: :
// ע��:
// ����:
// ����            ����                  ����
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
// ������:  CGtpUdpCommRouter::~CGtpUdpCommRouter
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��������
// ����:
//       ����1 :void
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
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
// ������:  CGtpUdpCommRouter::Init
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��ʼ������
// ����:
//       ����1 :unsigned char const nAPID
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CGtpUdpCommRouter::Init(unsigned char const nAPID)
{
    //�������ļ��ж�ȡ���ΪnAPID�Ľ������̵�GTP��Э��˿ڵ�����
    //nCount�������ļ��ж�ȡÿ��GTP��Э��˿ڵ�IP��ַ�Ͷ˿ںţ�
    //����nCount��CGtpUdpSocket����ÿ����һ�������øö����Open������
    //����*m_pCommTaskMan�����RegisterHandle�����Ե�ǰ��CGtpUdpSocket
    //�������ע�ᣬ����ǰ��CGtpUdpSocket����ָ����뵽m_SocketList�Ķ�β

    //�������ļ�
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    if(!pIniFile->Open())
    {
        delete pIniFile;
        return ERR_OPEN_CFG_FILE;
    }

    //��ȡ�����̶�Ӧ�Ľ�����
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
     	��UDP��TCP��ͬ�ĵ�ַ������Ϣ
    */
    //��ȡUDP��TCP�Ŀ���������
    int nUdpTcpMask = pIniFile->GetInt(strSec,
                                       CFG_AP_KEY_UDP_TCP_MASK,
                                       CFG_AP_KEY_UDP_TCP_MASK_DEFAULT);

    //��ȡ��GSN����������·IP��ַ
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

    //��ȡ��GSN����������·�Ķ˿ں�
    INT4 nPMLocalPort = pIniFile->GetInt(strSec,
                                         CFG_AP_KEY_LOCAL_PORT_TO_PM,
                                         CFG_AP_KEY_LOCAL_PORT_TO_PM_GSN_DEFAULT);
    
    //��ȡ��GSN�����ı���·IP��ַ
    char szFMLocalIp[20];
    pIniFile->GetString(strSec,
                        CFG_AP_KEY_LOCAL_IP_TO_FM,
                        CFG_AP_KEY_LOCAL_IP_TO_FM_GSN_DEFAULT,
                        szFMLocalIp,
                        sizeof(szFMLocalIp));

    //��ȡ��GSN�����ı���·�Ķ˿ں�
    INT4 nFMLocalPort = pIniFile->GetInt(strSec,
                                         CFG_AP_KEY_LOCAL_PORT_TO_FM,
                                         CFG_AP_KEY_LOCAL_PORT_TO_FM_GSN_DEFAULT);

    /*
     	��UDP���еĵ�ַ������Ϣ
    */
    //��ȡ��GSN����������·IP��ַ(UDP)
    char szPMLocalIpUdp[20];
    pIniFile->GetString(strSec,
                        CFG_AP_KEY_LOCAL_IP_TO_PM_UDP,
                        szPMLocalIp,
                        szPMLocalIpUdp,
                        sizeof(szPMLocalIpUdp));

    //��ȡ��GSN����������·�Ķ˿ں�(UDP)
    INT4 nPMLocalPortUdp = pIniFile->GetInt(strSec,
                                            CFG_AP_KEY_LOCAL_PORT_TO_PM_UDP,
                                            nPMLocalPort);
    
    //��ȡ��GSN�����ı���·IP��ַ(UDP)
    char szFMLocalIpUdp[20];
    pIniFile->GetString(strSec,
                        CFG_AP_KEY_LOCAL_IP_TO_FM_UDP,
                        szFMLocalIp,
                        szFMLocalIpUdp,
                        sizeof(szFMLocalIpUdp));

    //��ȡ��GSN�����ı���·�Ķ˿ں�(UDP)
    INT4 nFMLocalPortUdp = pIniFile->GetInt(strSec,
                                            CFG_AP_KEY_LOCAL_PORT_TO_FM_UDP,
                                            nFMLocalPort);
    
    //�ر������ļ��������� 
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

        //ע��GtpSocket���¼�
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

            //ע��GtpSocket���¼�
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
// ������:  CGtpUdpCommRouter::Send
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    Ϊ���˿�ɾ��һ��SOCKET���ӣ��������CGtpUdpSocket��������
//          SOCK_handle_close��������ñ�����
// ����:
//       ����1 :const MSG_INTRA* const pMsg
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CGtpUdpCommRouter::Send(const MSG_INTRA* const pMsg)
{
    //��pMsgǿ��ת����SNetFrame*���ͣ�pMsg->SNetAddrPair������������
    //����UDP���ݱ��ı��˵�IP��ַ�Ͷ˿ڣ�
    //����m_SocketList�е�CGtpUdpSocket���󣬵���CGtpUdpSocket�����GetAddr
    //�������Ӷ��жϵ�ǰ���ʵ�CGtpUdpSocket�����Ƿ��Ƕ�Ӧ��pMsg->
    //SNetAddrPairָ����IP��ַ�Ͷ˿ںţ���m_SocketList��û�ж�Ӧ��
    //CGtpUdpSocket�����򷵻�ERR_FAIL�������ڶ�Ӧ��CGtpUdpSocket������
    //���øö����Send�����������ظú����ķ���ֵ��
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
// ������:  CGtpUdpCommRouter::DelGtpSocket
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    Ϊ���˿�ɾ��һ��SOCKET���ӣ��������CGtpUdpSocket
//          ��������SOCK_handle_close��������ñ�����
// ����:
//       ����1 :CGtpUdpSocket* pGtpSocket
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CGtpUdpCommRouter::DelGtpSocket(CGtpUdpSocket* pGtpSocket)
{
    //��pGtpSocket��m_SocketList��ɾ��
    m_SocketList.remove(pGtpSocket);
}


/******************************************************
// ������:  CCommRouter
// ����:    Wangfeng
// ʱ��:    03-06-28
// ����:    ���캯��
// ����:
//       ����1 :pCommRouterMan
// ����ֵ: :
// ע��:
// ����:
// ����            ����                  ����
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
// ������:  CGtpFrmCommRouter::~CGtpFrmCommRouter
// ����:    Wangfeng
// ʱ��:    03-06-28
// ����:    ��������
// ����:
//       ����1 :void
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
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
// ������:  CGtpFrmCommRouter::Init
// ����:    Wangfeng
// ʱ��:    03-06-28
// ����:    ��ʼ������
// ����:
//       ����1 :unsigned char const nAPID
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//03-06-28       Wangfeng
******************************************************/
int CGtpFrmCommRouter::Init(unsigned char const nAPID)
{

    //�������ļ�
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    if(pIniFile->Open() == FALSE)
    {
        delete pIniFile;
        return ERR_OPEN_CFG_FILE;
    }

    //��ȡ�����̶�Ӧ�Ľ�����
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
        ����ЧIP��ַ��	
    */
    //����ЧIP��ַ��ĸ��������м���GSN����˫ƽ�����
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
    
    //�ر������ļ��������� 
    pIniFile->Close();
    delete pIniFile;

    return ERR_SUCCESS;
}


/******************************************************
// ������:  CGtpFrmCommRouter::AddFrmSocket
// ����:    Wangfeng
// ʱ��:    03-06-28
// ����:    Ϊ���˿�����һ��SOCKET����
// ����:
//       ����1 :CFrmSocket* pFrmSocket
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
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
            //ע����SOCKET������Ҫ�ø�SOCKET�ڹر��¼��лص�����
            pGtpFrmSocket->m_pCommRouter = NULL;
            pGtpFrmSocket->RemoveHandler();
        }
    }
    
    m_GtpFrmSocketMap[uIPAddr] = (CGtpFrmSocket*)pFrmSocket;
	pFrmSocket->m_pCommRouter = this;
    
    return ERR_SUCCESS;
}


/******************************************************
// ������:  CGtpFrmCommRouter::Send
// ����:    Wangfeng
// ʱ��:    03-06-28
// ����:    Ϊ���˿�ɾ��һ��SOCKET���ӣ��������CGtpUdpSocket��������
//          SOCK_handle_close��������ñ�����
// ����:
//       ����1 :const MSG_INTRA* const pMsg
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//03-06-28       Wangfeng
******************************************************/
int CGtpFrmCommRouter::Send(const MSG_INTRA* const pMsg)
{

    int nRet = ERR_SOCKET_NOT_PRESENT;

    //��pMsgǿ��ת����SNetFrame*���ͣ�pMsg->SNetAddrPair������������
    //����UDP���ݱ��ı��˵�IP��ַ�Ͷ˿ڣ�
    UINT4 uClientIP = ((_NetFrame *) pMsg)->SNetAddrPair.ClientIp;

    //���ȴ�MAP�ҿ��õ�SOCKET
    if(m_GtpFrmSocketMap.find(uClientIP) != m_GtpFrmSocketMap.end())
    {
        CGtpFrmSocket* pGtpFrmSocket = m_GtpFrmSocketMap[uClientIP];
        if(NULL != pGtpFrmSocket)
        {
            nRet = pGtpFrmSocket->Send(pMsg->Data);
        }
    }

    //���ʧ�ܣ����������·����
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
// ������:  CGtpFrmCommRouter::DelGtpSocket
// ����:    Wangfeng
// ʱ��:    03-06-28
// ����:    Ϊ���˿�ɾ��һ��SOCKET���ӣ��������CGtpUdpSocket
//          ��������SOCK_handle_close��������ñ�����
// ����:
//       ����1 :CGtpUdpSocket* pGtpSocket
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
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
// ������:  CMscLinkPair::CMscLinkPair
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    ���캯��
// ����:
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
CMscLinkPair::CMscLinkPair()
{
    m_pPrimaryLink = 0;
    m_pSecondaryLink = 0;
}


/******************************************************
// ������:  CMscLinkPair::~CMscLinkPair
// ����:    Wangfeng
// ʱ��:    01-10-10
// ����:    ��������
// ����:
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-10      Wangfeng
******************************************************/
CMscLinkPair::~CMscLinkPair()
{
    if(m_pPrimaryLink)
    {
        //����Socket��m_pCommRouter���㣬�ɱ�����ע��Socket
        //ʱ������ɾ����m_pCommRouter����ĺ���
        m_pPrimaryLink->m_pCommRouter = 0;
        m_pPrimaryLink = 0;
    }

    if(m_pSecondaryLink)
    {
        //����Socket��m_pCommRouter���㣬�ɱ�����ע��Socket
        //ʱ������ɾ����m_pCommRouter����ĺ���
        m_pSecondaryLink->m_pCommRouter = 0;
        m_pSecondaryLink = 0;
    }
}


/******************************************************
// ������:  CMscLinkPair::SetPrimaryLink
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    ���ô��������ӵ�CMemFrmSocket����
// ����:
//       ����1 :CMemFrmSocket* pLink
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
void CMscLinkPair::SetPrimaryLink(CMemFrmSocket* pLink)
{
    m_pPrimaryLink = pLink;
}


/******************************************************
// ������:  CMscLinkPair::SetSecondaryLink
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    ���ô������ӵ�CMemFrmSocket����
// ����:
//       ����1 :CMemFrmSocket* pLink
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
void CMscLinkPair::SetSecondaryLink(CMemFrmSocket* pLink)
{
    m_pSecondaryLink = pLink;
}


/******************************************************
// ������:  CMscLinkPair::GetPrimaryLink
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    ��ô��������ӵ�CMemFrmSocket����
// ����:
//       ����1 :void
// ����ֵ: CMemFrmSocket* const
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
CMemFrmSocket* const CMscLinkPair::GetPrimaryLink(void)
{
    return m_pPrimaryLink;
}


/******************************************************
// ������:  CMscLinkPair::GetSecondaryLink
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    ��ô������ӵ�CMemFrmSocket����
// ����:
//       ����1 :void
// ����ֵ: CMemFrmSocket* const
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
CMemFrmSocket* const CMscLinkPair::GetSecondaryLink(void)
{
    return m_pSecondaryLink;
}


/******************************************************
// ������:  CLinkPairs::CLinkPairs
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    ���캯��
// ����:
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
CLinkPairs::CLinkPairs()
{
	m_nLinkTimeOut = 10;
}


/******************************************************
// ������:  CLinkPairs::~CLinkPairs
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    ��������
// ����:
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
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
// ������:  CLinkPairs::OnLinkModuleNumberChanged
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:
// ����:
//       ����1 :(CFrmSocket* const pLink
//       ����2 :int const nOldModNum
//       ����3 :int const nNewModNum
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
void CLinkPairs::OnLinkModuleNumberChanged(CFrmSocket* const pLink,
                                           int const nOldModNum,
                                           int const nNewModNum)
{
    MAP_MODNUM_TO_PCMSCLINKPAIR::iterator i;
    //��pLink ǿ��ת����CMemFrmSocket*���ͣ�
    if (nOldModNum == nNewModNum)
    {
        return;
    }
    else
    {
        if (nOldModNum != APP_MID_NULL)
        //*pLink��������������ģ��Ű�
        {
/*          ��m_MapLinks�в���ģ���ΪnOldModNum��CMscLinkPair����
            �����ҵ��������pLink->m_bPrimary��ȡֵ���ø�CMscLinkPair
            �����SetPrimaryLink��SetSecondaryLink������ʵ��Ϊ0������
            CMscLinkPair�����GetPrimaryLink��GetSecondaryLink������
            ����ֵ��Ϊ0���򽫸�CMscLinkPair�����m_MapLinks��ɾ����
*/
            DelFrmSocket(pLink);
        }
        if (nNewModNum != APP_MID_NULL)
        //�µ�����ģ�������Ч��
        {
/*          ��m_MapLinks�в�������ģ���ΪnNewModNum��CMscLinkPair
            ���������ҵ��������pLink->m_bPrimary��ȡֵ����ע����
            CMscLinkPair�����m_pPrimaryLink����ע����CMscLinkPair
            �����m_pSecondaryLink����m_MapLinks�в���������ģ���
            ΪnNewModNum��CMscLinkPair�����򴴽�һ��CMscLinkPair
            ���󣬲�����pLink->m_bPrimary��ȡֵ���øö����
            SetPrimaryLink��SetSecondaryLink������ʵ��ΪpLink��Ȼ��
            ���´�����CMscLinkPair�����ָ����뵽m_MapLinks�У���ֵ
            ΪnNewModNum��
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
// ������:  CLinkPairs::GetLinkPairByModuleNumber
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    ��������ģ��Ż�ö�Ӧ��CMscLinkPair�����ָ��
// ����:
//       ����1 :int const nModuleNumber
// ����ֵ: CMscLinkPair* const
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
CMscLinkPair* const CLinkPairs::GetLinkPairByModuleNumber(
                                int const nModuleNumber)
{
/*  ��m_MapLinks�в�������ģ���ΪnModuleNumber��CMscLinkPair
    ���������ڣ��򷵻ظö����ָ�룬���򷵻�0
*/
    if(m_MapLinks.find(nModuleNumber) != m_MapLinks.end())
    {
        return m_MapLinks[nModuleNumber];
    }
    return 0;
}


/******************************************************
// ������:  CLinkPairs::DelFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-17
// ����:    Ϊ���˿�ɾ��һ��SOCKET����
// ����:
//       ����1 :CFrmSocket* pFrmSocket
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-17      Wangfeng
******************************************************/
void CLinkPairs::DelFrmSocket(CFrmSocket* pFrmSocket)
{
    //��pFrmSocketǿ��ת����CMemFrmSocket*���ͣ�
    int nModNum = ((CMemFrmSocket *)pFrmSocket)->GetModuleNum();
    if(nModNum == APP_MID_NULL)
    {
        return;
    }

    //��m_MapLinks�в�������ģ���ΪpFrmSocket->m_nModuleNum��
    //CMscLinkPair����
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
            //����CMscLinkPair�����m_MapLinks��ɾ��;
            delete m_MapLinks[nModNum];
            m_MapLinks.erase(nModNum);
        }
    }
}

//����Զ2002.04.08���
/******************************************************
// ����:    ����*pMsg֡�е������Ϣ��ѡ���ʵ�����·��
//          *pMsg��Data������ݷ��ͳ�ȥ
// ����:
//       ����1 :const MSG_INTRA* const pMsg
// ����ֵ: int
// ע��:
// ����:
******************************************************/
int CLinkPairs::Send(const MSG_INTRA* const pMsg)
{
	CMemFrmSocket* pSocket = NULL;
	int nResult = ERR_SOCKET_NOT_PRESENT;

	SMemFrame* pMemFrame = (SMemFrame* )(pMsg->Data);
    SAppFrame* pAppFrame = (SAppFrame* )(pMemFrame->Data());
	UINT4 uModu = pAppFrame->receiver_mid();

	if (APP_MID_NULL == uModu)
	//����ģ���ΪAPP_MID_NULL����������ģ��㲥
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

			//��������Ӳ����ڻ�������ӷ���ʧ�ܣ���ӱ����ӷ���
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
	else	//������ָ����ģ�鷢��
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

		    //��������Ӳ����ڻ�������ӷ���ʧ�ܣ���ӱ����ӷ���
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
// ����:    ���¼�������
// ����:
//       ����1 :void
// ����ֵ: void
// ע��:
// ����:
******************************************************/
void CLinkPairs::Timer1Sec(void)
{
	SMemFrame* pHandShakeFrm = new(_MEM_HANDSHAKE_LENGTH) SMemFrame;
	pHandShakeFrm->nCmd(MEM_CMD_HANDSHAKE);

	//�������е�socket�������ʱ������ɾ����
	//�������䷢������֡
	MAP_MODNUM_TO_PCMSCLINKPAIR::iterator i;
	CMemFrmSocket* pSocket = NULL;
	for (i = m_MapLinks.begin(); i != m_MapLinks.end(); i++)
	{
		//��������֡�е�ģ���
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

//����������ʱ����
void CLinkPairs::SetLinkTimeOut(int nLinkTimeOut)
{
	m_nLinkTimeOut = nLinkTimeOut;
}

//����Զ2002.04.08��ӽ���

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
// ����:    ����*pMsg֡�е������Ϣ��ѡ���ʵ�����·��
//          *pMsg��Data������ݷ��ͳ�ȥ
// ����:
//       ����1 :const MSG_INTRA* const pMsg
// ����ֵ: int
// ע��:
// ����:
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

	    //��������Ӳ����ڻ�������ӷ���ʧ�ܣ���ӱ����ӷ���
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
/*  ��m_MapLinks�в�������ģ���ΪnModuleNumber��CMscLinkPair
    ���������ڣ��򷵻ظö����ָ�룬���򷵻�0
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
                //����CMscLinkPair�����m_MapLinks��ɾ��;
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
// ������:  CSlideWndCommRouter::CSlideWndCommRouter
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    ���캯��
// ����:
//       ����1 :CCommRouterMan* const pCommRouterMan
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
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
// ������:  CSlideWndCommRouter::~CSlideWndCommRouter
// ����:    Wangfeng
// ʱ��:    01-10-10
// ����:    ��������
// ����:
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-10      Wangfeng
******************************************************/
CSlideWndCommRouter::~CSlideWndCommRouter()
{
    //�ͷ�m_SocketMap�еĶ���
    MAP_PCMEMFRMSOCKET_TO_INT::iterator i;
    for(i = m_SlideWndSocketMap.begin();
        i != m_SlideWndSocketMap.end();
        i++)
    {
        //����Socket��m_pCommRouter���㣬�ɱ�����ע��Socket
        //ʱ������ɾ����m_pCommRouter����ĺ���
        (*i).first->m_pCommRouter = 0;
    }
    m_SlideWndSocketMap.clear();
}


/******************************************************
// ������:  CSlideWndCommRouter::AddFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    Ϊ���˿�����һ��SOCKET����
// ����:
//       ����1 :CFrmSocket* pFrmSocket
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
int CSlideWndCommRouter::AddFrmSocket(CFrmSocket* pFrmSocket)
{
    //��pFrmSocketǿ��ת����CMemFrmSocket*����
    //��pFrmSocket���뵽m_SlideWndSocketMap��
    m_SlideWndSocketMap[(CMemFrmSocket *)pFrmSocket] = 
        ((CMemFrmSocket *)pFrmSocket)->GetModuleNum();
    pFrmSocket->m_pCommRouter = this;
    return 0;
}


/******************************************************
// ������:  CSlideWndCommRouter::DelFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    Ϊ���˿�ɾ��һ��SOCKET����
// ����:
//       ����1 :CFrmSocket* pFrmSocket
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
void CSlideWndCommRouter::DelFrmSocket(CFrmSocket* pFrmSocket)
{
    //��pFrmSocketǿ��ת����CMemFrmSocket*���ͣ�
    //��pFrmSocket��m_SlideWndSocketMap��ɾ��;
    if(((CMemFrmSocket*)pFrmSocket)->GetModuleNum() != APP_MID_NULL)
    {
        m_LinkPairs.DelFrmSocket((CMemFrmSocket*)pFrmSocket);
    }

    m_SlideWndSocketMap.erase((CMemFrmSocket*)pFrmSocket);

    pFrmSocket->m_pCommRouter = NULL;
}


/******************************************************
// ������:  CSlideWndCommRouter::OnReceiveEx
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    ��������*pFrmSocket������յ�������֡
// ����:
//       ����1 :CFrmSocket* pFrmSocket
//       ����2 :void* pFrame
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
void CSlideWndCommRouter::OnReceiveEx(CFrmSocket* pFrmSocket,
                                      void* pFrame)
{

    SMemFrame* pMsg = (SMemFrame *)pFrame;
    //��pFrmSocket����ĳ�ʱ����������

    if(pMsg->nCmd() == MEM_CMD_HANDSHAKE)
    //��*pMsgΪ����֡
    {
        //������֡������ģ�����Ϣ������pFrmSocket->m_nModuleNum��
        //����m_LinkPairs�����OnLinkModuleNumberChanged������
        //���ı��pFrmSocket���������ģ�����Ϣ��ͬʱҪ��Ӧ��
        //m_SlideWndSocketMapɾ����pFrmSocket��
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
		//����Զ2002.04.19�޸�
		//һ��SMemFrame�������п��ܰ������SAppFrame
		MSG_INTRA* pMsgIntra = NULL;
		BYTE* pAFrame = pMsg->Data();
		SAppFrame* pAppFrame = NULL;
		while (pMsg->Data() + pMsg->nLen() - pAFrame > _APPFRAME_EXHEAD_LENGTH)
		{
			//���ʣ������ݻ�����һ��SAppFrame�����˳�
			pAppFrame = (SAppFrame* )pAFrame;
			if (pMsg->Data() + pMsg->nLen() - pAFrame < pAppFrame->size())
			{
				break;
			}

			//��pAppFrame�����һ��MSG_INTRAʵ����Data�򣬵��û���CCommRouter::
			//OnReceive�ӿ��ύ����֡
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

		//���MemFrame�е����ݴ��ڴ�����д��TRACE
		if (pAFrame < pMsg->Data() + pMsg->nLen())
		{
			int nRemain = pMsg->Data() + pMsg->nLen() - pAFrame;
			MSGOUT(MTS_COMM, MSGOUT_LEVEL_URGENT, S_COMM_MEM_LEN_ERR, nRemain);
			TRACE(MTS_COMM, S_COMM_MEM_LEN_ERR, nRemain);
		}
    }
	//����Զ2002.04.19�޸����



    //�������Ϣ֡�ɵ����ߵ���CFrmSocket::OnReceiveFrameɾ��

}


/******************************************************
// ������:  CSlideWndCommRouter::Send
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    ����*pMsg֡ͷ�е������Ϣ��ѡ���ʵ�����·��
//          *pMsg��Data������ݷ��ͳ�ȥ
// ����:
//       ����1 :const MSG_INTRA* const pMsg
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
int CSlideWndCommRouter::Send(const MSG_INTRA* const pMsg)
{
	//����Զ2002.04.08���
	return m_LinkPairs.Send(pMsg);
	//����Զ2002.04.08��ӽ���
}

/******************************************************
// ������:  CSlideWndCommRouter::Timer1Sec
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    ���¼�������
// ����:
//       ����1 :void
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
void CSlideWndCommRouter::Timer1Sec(void)
{
	//����Զ2002.04.08���
	m_LinkPairs.Timer1Sec();
	//����Զ2002.04.08��ӽ���
}

//����Զ2002.04.08���
//����������ʱ����
void CSlideWndCommRouter::SetLinkTimeOut(int nLinkTimeOut)
{
	m_LinkPairs.SetLinkTimeOut(nLinkTimeOut);
}
//����Զ2002.04.08��ӽ���


//����Զ2002.04.08���
//���캯��
CSoftXCommRouter::CSoftXCommRouter(CCommRouterMan* const pCommRouterMan,
									const unsigned long uAppType)
						: CCommRouter(pCommRouterMan, uAppType)
{
	m_nLinkTimeOut       = 10;
}

//��������
CSoftXCommRouter::~CSoftXCommRouter()
{
    //����m_AllFrmSockets�еĶ���
    VECTOR<CSysFrmSocket* >::iterator i;
    for(i = m_AllFrmSockets.begin(); i != m_AllFrmSockets.end(); i++)
    {
        //����Socket��m_pCommRouter���㣬�ɱ�����ע��Socket
        //ʱ������ɾ����m_pCommRouter����ĺ���
        (*i)->m_pCommRouter = NULL;
    }
    m_AllFrmSockets.clear();
}

/******************************************************
// ����:    Ϊ���˿�����һ��SOCKET����
// ����:
//       ����1 :CFrmSocket* pFrmSocket
// ����ֵ: void
// ע��:
******************************************************/
int CSoftXCommRouter::AddFrmSocket(CFrmSocket* pFrmSocket)
{
	m_AllFrmSockets.push_back((CSysFrmSocket* )pFrmSocket);
	pFrmSocket->m_pCommRouter = this;

	return 0;
}

/******************************************************
// ����:    Ϊ���˿�ɾ��һ��SOCKET����
// ����:
//       ����1 :CFrmSocket* pFrmSocket
// ����ֵ: void
// ע��:
******************************************************/
void CSoftXCommRouter::DelFrmSocket(CFrmSocket* pFrmSocket)
{
	//��m_AllFrmSockets��ɾ��pFrmSocket
	VECTOR<CSysFrmSocket* >::iterator j;
	for (j = m_AllFrmSockets.begin(); j != m_AllFrmSockets.end(); j++)
	{
		if (*j == pFrmSocket)
		{
			m_AllFrmSockets.erase(j);
			break;
		}
	}

	//��m_LinkPairs��ɾ��pFrmSocket
    m_LinkPairs.DelFrmSocket(pFrmSocket);

    //����ص�
    pFrmSocket->m_pCommRouter = NULL;
}

/******************************************************
// ����:    ��������*pFrmSocket������յ�������֡
// ����:
//       ����1 :CFrmSocket* pFrmSocket
//       ����2 :void* pFrame
// ����ֵ: void
// ע��:
******************************************************/
void CSoftXCommRouter::OnReceiveEx(CFrmSocket* pFrmSocket, void* pFrame)
{
	SSysFrame* pMsg = (SSysFrame* )pFrame;
	UINT4 uModu = pMsg->sender_mid();

	//������Ϣ�е�ģ���������ģ��Ŷ�Ӧ��Socket
	if (SYS_MID_BAM != uModu)
	{
        m_LinkPairs.RegisterModule(pFrmSocket, uModu);
	}

	//pFrameΪ������Ϣ�������ϲ㴫�ͣ�ֱ�ӷ���
	if (SYS_CMD_SYS_HAND_SHAKE == pMsg->ucCmdCode())
	{
		return;
	}

	//pFrame����������Ϣʱ������MSG_INTRA�ṹ���ύ������ģ��
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
// ����:    ����*pMsg֡�е������Ϣ��ѡ���ʵ�����·��
//          *pMsg��Data������ݷ��ͳ�ȥ
// ����:
//       ����1 :const MSG_INTRA* const pMsg
// ����ֵ: int
// ע��:
******************************************************/
int CSoftXCommRouter::Send(const MSG_INTRA* const pMsg)
{
	SSysFrame* pFrame = (SSysFrame* )(pMsg->Data);
	CFrmSocket* pSocket = NULL;

	if (SYS_MID_BAM == pFrame->receiver_mid())
	//��������BAM����Ϣ����˳���m_AllFrmSocketsȡһ��socket���з���
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
	else	//������ݽ���ģ���ѡȡsocket���з���
	{
	    return m_LinkPairs.Send(pMsg);
	}

	return ERR_SOCKET_NOT_PRESENT;
}

/******************************************************
// ����:    ���¼�������
// ����:
//       ����1 :void
// ����ֵ: void
// ע��:
******************************************************/
void CSoftXCommRouter::Timer1Sec(void)
{
	//��������֡
	SSysFrame* pHandShakeFrm = new(0) SSysFrame;
	pHandShakeFrm->sender_mid(SYS_MID_BAU);
	pHandShakeFrm->sender_pid(SYS_PID_HTCOMM);
	pHandShakeFrm->receiver_pid(SYS_PID_HTCOMM);
	pHandShakeFrm->ucCmdCode(SYS_CMD_BAU_HAND_SHAKE);

	VECTOR<CSysFrmSocket* >::iterator i;

	//�������е�socket�������ʱ������ɾ����
	//�������䷢������֡
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

//����������ʱ����
void CSoftXCommRouter::SetLinkTimeOut(int nLinkTimeOut)
{
	m_nLinkTimeOut = nLinkTimeOut;
}

//����Զ2002.04.08��ӽ���



/******************************************************
// ������:  CMsgSvrCommRouter::CMsgSvrCommRouter
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    ���캯��
// ����:
//       ����1 :CCommRouterMan* const pCommRouterMan
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
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
// ������:  CMsgSvrCommRouter::~CMsgSvrCommRouter
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    ��������
// ����:
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
CMsgSvrCommRouter::~CMsgSvrCommRouter()
{
    //�ͷ�m_SocketMap�еĶ���
    MAP_PT_TO_PMSGFRMSOCKET::iterator i;
    for(i = m_SocketMap.begin(); i != m_SocketMap.end(); i++)
    {
        //����Socket��m_pCommRouter���㣬�ɱ�����ע��Socket
        //ʱ������ɾ����m_pCommRouter����ĺ���
        (*i).second->m_pCommRouter = 0;
    }
    m_SocketMap.clear();

    //�ͷ�m_SocketList�еĶ���
    while(m_SocketList.size() > 0)
    {
        CMsgFrmSocket* pMsgFrmSocket = m_SocketList.front();

        //����Socket��m_pCommRouter���㣬�ɱ�����ע��Socket
        //ʱ������ɾ����m_pCommRouter����ĺ���
        pMsgFrmSocket->m_pCommRouter = 0;
        m_SocketList.pop_front();
    }
}



/******************************************************
// ������:  CMsgSvrCommRouter::AddFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    Ϊ���˿�����һ��SOCKET����
// ����:
//       ����1 :CFrmSocket* pFrmSocket
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
int CMsgSvrCommRouter::AddFrmSocket(CFrmSocket* pFrmSocket)
{
    //��pFrmSocketǿ��ת����CMsgFrmSocket*����
    //��pFrmSocket���뵽m_SocketList�Ķ�β
    m_SocketList.push_back((CMsgFrmSocket*)pFrmSocket);
    pFrmSocket->m_pCommRouter = this;
    return 0;
}



/******************************************************
// ������:  CMsgSvrCommRouter::DelFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    Ϊ���˿�ɾ��һ��SOCKET����
// ����:
//       ����1 :CFrmSocket* pFrmSocket
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
void CMsgSvrCommRouter::DelFrmSocket(CFrmSocket* pFrmSocket)
{
    //��pFrmSocketǿ��ת����CMsgFrmSocket*����
    //��pFrmSocket��m_SocketList��m_SocketMap��ɾ��
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
// ������:  CMsgSvrCommRouter::OnReceiveEx
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    ��������*pFrmSocket������յ�������֡
// ����:
//       ����1 :CFrmSocket* pFrmSocket
//       ����2 :void* pFrame
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
void CMsgSvrCommRouter::OnReceiveEx(CFrmSocket* pFrmSocket,
                                    void* pFrame)
{
/*  ��1 �����pMsg�Ƿ��ǡ��ͻ���ע����Ϣ������pMsg�ǡ��ͻ���
    ע����Ϣ��������ע����Ϣ�е�SenderPid��m_SocketMap�н���
    pFrmSocket��������̱�ŵ�ӳ���ϵ�����m_SocketMap���Ѵ�
    �ڸý��̺Ŷ�Ӧ��CMsgFrmSocket��������ע����CMsgFrmSocket
    �����ٴ�map��ɾ���ö���Ȼ������µĶ��󣬲��Ҵ�
    m_SocketList��ɾ��CMsgFrmSocket*��ָ�Ķ��󣬲�ɾ��pMsg��
    ����ִ�в��裨2 ����*/
    MSG_INTRA* pMsg = (MSG_INTRA *) pFrame;

    if(pMsg->CmdCode == CMD_REG_PROCESS)
    //�ͻ���ע����Ϣ
    {
        if(m_SocketMap.find(pMsg->SenderPid) != m_SocketMap.end())
        //����Ѵ���һ����pMsg->SenderPidƥ���Socket��ע����Socket
        {
            if (m_SocketMap[pMsg->SenderPid] && 
                (m_SocketMap[pMsg->SenderPid] != pFrmSocket))
            {
                m_SocketMap[pMsg->SenderPid]->RemoveHandler();
                m_SocketMap[pMsg->SenderPid]->m_pCommRouter = 0;
            }
        }
        
        TRACE(MTS_COMM, S_COMM_SHAKE_HAND, pMsg->SenderPid);

        //��Map����ӽ��̱�ŵ�Socketָ�������
        m_SocketMap[pMsg->SenderPid] = (CMsgFrmSocket*)pFrmSocket;

        //��m_SocketList��ɾ����Socketָ��
        m_SocketList.remove((CMsgFrmSocket*)pFrmSocket);

        //���������ύ����Ϣ֡�������ڴ˴�ɾ������Ϣ֡
        delete pMsg;
    }
    else
    {
        /*�ж�MSG_INTRA��ReceivePid�Ƿ�Ϊ������ID�����Ǳ�����ID�������
        CCommRouter::OnReceiveֱ�����ϲ��ύpMsg�������PT_BROADCAST����
        �㲥����Ϣ������ֱ��ת������Ϣ��*/
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
            //ת������Ϣ֡
            int nRet = Send(pMsg);
            //������ 2002-03-13 delete old code for ���ⵥD11689
            //�ڴ˴�����ɾ������Ϣ֡
            delete pMsg;
        }
    }
}


/******************************************************
// ������:  CMsgSvrCommRouter::Send
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    ����*pMsg֡ͷ�е������Ϣ��ѡ���ʵ�����·��
//          *pMsg��Data������ݷ��ͳ�ȥ
// ����:
//       ����1 :const MSG_INTRA* const pMsg
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
int CMsgSvrCommRouter::Send(const MSG_INTRA* const pMsg)
{
    //����pMsg->ReceiverPid����m_SocketMap,��������ReceiverPid
    //��Ӧ��CMsgFrmSocket����,����CMsgFrmSocket��������Ϣ,
    //�����ظ�CMsgFrmSocket�����Send�����ķ���ֵ,���򷵻�ʧ��
    if(m_SocketMap.find(pMsg->ReceiverPid) != m_SocketMap.end())
    {
        return m_SocketMap[pMsg->ReceiverPid]->Send(pMsg);
    }
    return ERR_SOCKET_NOT_PRESENT;
}


/******************************************************
// ������:  CMsgCliCommRouter::CMsgCliCommRouter
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    ���캯��
// ����:
//       ����1 :CCommRouterMan* const pCommRouterMan
//       ����2 :PROCESS_TYPE const nProcessType
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
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

    //��Kernel��������
    Connect();
}


/******************************************************
// ������:  CMsgCliCommRouter::~CMsgCliCommRouter
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    ��������
// ����:
//       ����1 :void
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
CMsgCliCommRouter::~CMsgCliCommRouter(void)
{
    if(m_pMsgFrmSocket)
    {
        //����Socket��m_pCommRouter���㣬�ɱ�����ע��Socket
        //ʱ������ɾ����m_pCommRouter����ĺ���
        m_pMsgFrmSocket->m_pCommRouter = 0;
        m_pMsgFrmSocket = NULL;
    }
}



/******************************************************
// ������:  CMsgCliCommRouter::DelFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    Ϊ���˿�ɾ��һ��SOCKET����
// ����:
//       ����1 :CFrmSocket* pFrmSocket
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
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
// ������:  CMsgCliCommRouter::Connect
// ����:    Wangfeng
// ʱ��:    01-10-17
// ����:    ��Kernel���̽�������
// ����:
//       ����1 :void
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-17      Wangfeng
******************************************************/
int CMsgCliCommRouter::Connect(void)
{
    //ʹ��ACE_SOCK_Connector����KENERL���̵���Ϣ��ת����
    //���ˣ������ӳɹ����򴴽�һ��CMsgFrmSocket���󣬽���
    //�����ָ�븳ֵ��m_pMsgFrmSocket�����øö����Send��
    //����Զ˷��͡�ע����Ϣ���������Ͳ��ɹ��������
    //m_pMsgFrmSocket�ĳ�Ա����RemoveHandler()��ACE_Reactor
    //��ע���¼�����ʹm_pMsgFrmSocket����0

    //�������ļ��ж�ȡMsgSvr�Ķ˿ںţ�ȱʡֵΪ9999
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    pIniFile->Open();
    int nMsgSvrPort = pIniFile->GetInt(CFG_SEC_COMMON,
                                CFG_COMMON_KEY_MSGSVR_PORT,
                                CFG_COMMON_KEY_MSGSVR_PORT_DEFAULT);
    pIniFile->Close();
    delete pIniFile;

    //��Kernel���̽�������
    ACE_SOCK_Connector Connector;
    ACE_SOCK_Stream *pstream = new ACE_SOCK_Stream();
    ACE_INET_Addr remote_sap;
    remote_sap.set(nMsgSvrPort, LOCAL_ADDR);

    ACE_Time_Value timeout(2);
    int nRet = ERR_FAIL;

    if(Connector.connect(*pstream, remote_sap, &timeout) != 0)
    //�������ɹ�
    {
        delete pstream;
        return nRet;
    }

    //�����ɹ���
    m_pMsgFrmSocket = new CMsgFrmSocket(pstream);
    m_pMsgFrmSocket->m_pCommRouter = this;

    //ע��Socket�Ķ�д�¼�
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

    //��������Ϣ��Kernelע��
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
// ������:  CMsgCliCommRouter::Timer1Sec
// ����:    Wangfeng
// ʱ��:    01-10-8
// ����:    ���¼�������
// ����:
//       ����1 :void
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-8       Wangfeng
******************************************************/
void CMsgCliCommRouter::Timer1Sec(void)
{
    if (m_pMsgFrmSocket == 0)
    {
        if (m_nReConnectTimer >= 5)
        {
            //��������������
            m_nReConnectTimer = 0;

            //��Kernel��������
            Connect();
        }
        else
        {
            m_nReConnectTimer++;
        }
    }
}



/******************************************************
// ������:  CMsgCliCommRouter::Send
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ����pMsgָ�������֡
// ����:
//       ����1 :const MSG_INTRA* const pMsg
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
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
// ������:  CMmlCommRouter::CMmlCommRouter
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ���캯��
// ����:
//       ����1 :CCommRouterMan* const pCommRouterMan
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
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
// ������:  CMmlCommRouter::~CMmlCommRouter
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ��������
// ����:
//       ����1 :void
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CMmlCommRouter::~CMmlCommRouter(void)
{
    //�ͷ�m_SocketMap�еĶ���
    MAP_CN_TO_PCMMLFRMSOCKET::iterator i;
    for(i = m_SocketMap.begin(); i != m_SocketMap.end(); i++)
    {
        //����Socket��m_pCommRouter���㣬�ɱ�����ע��Socket
        //ʱ������ɾ����m_pCommRouter����ĺ���
        (*i).second->m_pCommRouter = 0;
    }
    m_SocketMap.clear();
}



/******************************************************
// ������:  CMmlCommRouter::AddFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    Ϊ���˿�����һ��SOCKET����
// ����:
//       ����1 :CFrmSocket* pFrmSocket
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
int CMmlCommRouter::AddFrmSocket(CFrmSocket* pFrmSocket)
{
    //��pFrmSocketǿ��ת����CMmlFrmSocket*����
    //��m_SocketID��Ϊ��ֵ��pFrmSocket���뵽m_SocketMap��

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
// ������:  CMmlCommRouter::DelFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    Ϊ���˿�ɾ��һ��SOCKET����
// ����:
//       ����1 :CFrmSocket* pFrmSocket
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
void CMmlCommRouter::DelFrmSocket(CFrmSocket* pFrmSocket)
{
    //��pFrmSocketǿ��ת����CMmlFrmSocket*���ͣ�
    //��pFrmSocket��m_SocketMap��ɾ��;
    MAP_CN_TO_PCMMLFRMSOCKET::iterator i;
    for(i = m_SocketMap.begin(); i != m_SocketMap.end(); i++)
    {
        if((*i).second == (CMmlFrmSocket*)pFrmSocket)
        {
            //������ 2003-04-01
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
// ������:  CMmlCommRouter::Timer1Sec
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ���¼�������
// ����:
//       ����1 :void
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
void CMmlCommRouter::Timer1Sec(void)
{
    //����m_SocketMap�����е�CMmlFrmSocket����,��������
    //RemoveHandler()��Ա����ע����ʱδ���յ����ݵ�
    //CMmlFrmSocket����ʹ����CMemFrmSocket�����
    //m_lTimeNoData��һ��
    //���������ֳ�ʱȥ������Ϊ��������
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
// ������:  CMmlCommRouter::OnReceiveEx
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ��������*pFrmSocket������յ�������֡
// ����:
//       ����1 :CFrmSocket* pFrmSocket
//       ����2 :void* pFrame
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
void CMmlCommRouter::OnReceiveEx(CFrmSocket* pFrmSocket,
                                 void* pFrame)
{
    //��pFrmSocket����ĳ�ʱ����������;
    //��m_SocketMap�л��pFrmSocket��Ӧ�ı��,��pMsg���
    //��һ��MSG_INTRAʵ��,����CCommRouter::OnReceive����
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

    //�������Ϣ֡�ɵ�����ɾ��
}


/******************************************************
// ������:  CMmlCommRouter::Send
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ����pMsgָ�������֡
// ����:
//       ����1 :const MSG_INTRA* const pMsg
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
int CMmlCommRouter::Send(const MSG_INTRA* const pMsg)
{
    //�ж�pMsg->ClientNo�Ƿ�Ϊ0,��ClientNoΪ0,����m_SocketMap������
    //CMmlFrmSocket�����Send����,�����к����ķ���ֵΪERR_SUCCESS,
    //�򷵻�ERR_SUCCESS,���򷵻�ERR_FAIL
    int nRet = ERR_SUCCESS;
    
    //�����ͣ��ĳ��Op�Ͳ��ط�����
    if(pMsg->CmdCode == CMD_BREAK_OPERATOR)
    {
        MAP_CN_TO_PCMMLFRMSOCKET::iterator it = m_SocketMap.find(pMsg->ClientNo);
        if(it == m_SocketMap.end())
            return ERR_SUCCESS;
        //������ 2003-02-18
        CMmlFrmSocket *pSocket = it->second;
        
        pSocket->RemoveHandler();
        //����
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
    //�������pMsg->ClientNo��m_SocketMap�в��Ҷ�Ӧ��CMmlFrmSocket��
    //��,��������,�򷵻�ERR_FAIL,������øö����Send����,�����ظú�
    //���ķ���ֵ
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
// ������:  CAlarmBoxRouter::CAlarmBoxRouter
// ����:    Wangzhaoyang
// ʱ��:    02-07-15
// ����:    ���캯��
// ����:
//       ����1 :CCommRouterMan* const pCommRouterMan
//       ����2 :const unsigned long uAppType
// ����ֵ: 
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//02-07-15       Wangzhaoyang            ����
******************************************************/
CAlarmBoxRouter::CAlarmBoxRouter(CCommRouterMan* const pCommRouterMan,
								 const unsigned long uAppType)
								 :CCommRouter(pCommRouterMan, uAppType)
{
	m_pABFrmSocket = NULL;
}

/******************************************************
// ������:  CAlarmBoxRouter::~CAlarmBoxRouter
// ����:    Wangzhaoyang
// ʱ��:    02-07-15
// ����:    ��������
// ����:
// ����ֵ: 
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//02-07-15       Wangzhaoyang            ����
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
// ������:  CAlarmBoxRouter::AddFrmSocket
// ����:    Wangzhaoyang
// ʱ��:    02-07-15
// ����:    Ϊ���˿�����һ��SOCKET����
// ����:
//       ����1 :CFrmSocket* pFrmSocket
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//02-07-15       Wangzhaoyang            ����
******************************************************/
int CAlarmBoxRouter::AddFrmSocket(CFrmSocket* pFrmSocket)
{
	if(NULL == m_pABFrmSocket)
	{
		m_pABFrmSocket = (CABFrmSocket*)pFrmSocket;
	}
	else //ɾ��ԭ�ȵ�socket�����½���
	{
        m_pABFrmSocket->m_pCommRouter = NULL;
		m_pABFrmSocket->RemoveHandler();
		m_pABFrmSocket = (CABFrmSocket*)pFrmSocket;
	}

	pFrmSocket->m_pCommRouter = this;
	return 0;
}

/******************************************************
// ������:  CAlarmBoxRouter::DelFrmSocket
// ����:    Wangzhaoyang
// ʱ��:    02-07-15
// ����:    Ϊ���˿�ɾ��һ��SOCKET����
// ����:
//       ����1 :CFrmSocket* pFrmSocket
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//02-07-15       Wangzhaoyang            ����
******************************************************/
void CAlarmBoxRouter::DelFrmSocket(CFrmSocket* pFrmSocket)
{
	ASSERT(pFrmSocket == m_pABFrmSocket);
	ASSERT(pFrmSocket != NULL);
	
    m_pABFrmSocket->m_pCommRouter = NULL;
	m_pABFrmSocket = NULL;
}

/******************************************************
// ������:  CAlarmBoxRouter::OnReceive
// ����:    Wangzhaoyang
// ʱ��:    02-07-15
// ����:    ��������*pFrmSocket������յ�������֡
// ����:
//       ����1 :void* pFrame
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//02-07-15       Wangzhaoyang            ����
******************************************************/
void CAlarmBoxRouter::OnReceiveEx(CFrmSocket* pFrmSocket,void* pFrame)
{

	SAppConvertFrame* pMsg = (SAppConvertFrame*)pFrame;
  
	MSG_INTRA* pMsgIntra = NULL;
	if(pMsg->byCmd == ALARM_CMD_CONVERT_TO_BAU)  //convert��BAU�ĸ澯����
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
// ������:  CAlarmBoxRouter::Send
// ����:    Wangzhaoyang
// ʱ��:    02-07-15
// ����:    ���Ӹ澯ģ�鷢��������ת����convert
// ����:
//       ����1 :const MSG_INTRA* const pMsg
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//02-07-15       Wangzhaoyang            ����
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
//GTP'�Ʒѽӿ����� Z30971 2004/06/10

//��GTP'Э��TCP��ʽ�Ŀͻ��˵ĳ���
//���캯��
CGTPTcpCliCommRouter::CGTPTcpCliCommRouter(CCommRouterMan* const pCommRouterMan,
                                           const unsigned long uAppType,
                                           PROCESS_TYPE const nProcessType)
                                           :CCommRouter(pCommRouterMan, uAppType)
{
    m_nProcessType = nProcessType;
    m_pGtpTcpCliSocket = 0;
    m_nReConnectTimer = 0;

    //��BS��������
    Connect();
}

//��������
CGTPTcpCliCommRouter::~CGTPTcpCliCommRouter(void)
{
    if(m_pGtpTcpCliSocket)
    {
        //����Socket��m_pCommRouter���㣬�ɱ�����ע��Socket
        //ʱ������ɾ����m_pCommRouter����ĺ���
        m_pGtpTcpCliSocket->m_pCommRouter = 0;
        m_pGtpTcpCliSocket = NULL;
    }
}

//ɾ��GTP TCP SOCKET
void CGTPTcpCliCommRouter::DelFrmSocket(CFrmSocket* pFrmSocket)
{
    ASSERT(m_pGtpTcpCliSocket == pFrmSocket);

    if(m_pGtpTcpCliSocket && m_pGtpTcpCliSocket == pFrmSocket)
    {
        m_pGtpTcpCliSocket->m_pCommRouter = 0;
        m_pGtpTcpCliSocket = NULL;
    }
}

//����BS
int CGTPTcpCliCommRouter::Connect(void)
{
    //ʹ��ACE_SOCK_Connector����BS����, ��GTP TCP����
    //���ˣ������ӳɹ����򴴽�һ��Socket���󣬽���
    //�����ָ�븳ֵ��m_pGtpTcpCliSocket.

    
    //�������ļ��ж�ȡGTP Svr��IP��ַ
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    pIniFile->Open();

    char szSvrIp[20];
    pIniFile->GetString(CFG_SEC_BS, 
                        CFG_BS_KEY_GTP_SVR_IP, 
                        CFG_BS_KEY_GTP_SVR_IP_DEFAULT,
                        szSvrIp, 
                        sizeof(szSvrIp));
    
    //�������ļ��ж�ȡGTP Svr�Ķ˿ںţ�ȱʡֵΪ3386
    int nGTPSvrPort = pIniFile->GetInt(CFG_SEC_BS, 
                                       CFG_BS_KEY_GTP_SVR_PORT,
                                       CFG_BS_KEY_GTP_SVR_PORT_DEFAULT
                                       );

    //��ȡ���ؿͻ��˵�IP��ַ
    char szLocalIp[20];
    pIniFile->GetString(CFG_SEC_BS, 
                        CFG_BS_KEY_GTP_CLI_IP, 
                        CFG_BS_KEY_GTP_CLI_IP_DEFAULT,
                        szLocalIp, 
                        sizeof(szLocalIp)
                        );
    
    //��ȡ���ؿͻ��˵ĵĶ˿ںţ�ȱʡֵΪ3386
    int nLocalPort = pIniFile->GetInt(CFG_SEC_BS, 
                                      CFG_BS_KEY_GTP_CLI_PORT,
                                      CFG_BS_KEY_GTP_CLI_PORT_DEFAULT
                                      );
    
    pIniFile->Close();
    delete pIniFile;

    //��BS��������
    ACE_SOCK_Connector Connector;
    ACE_SOCK_Stream *pstream = new ACE_SOCK_Stream();
    ACE_INET_Addr remote_sap;
    remote_sap.set(nGTPSvrPort, szSvrIp);

    ACE_INET_Addr local_sap;
    local_sap.set(nLocalPort,szLocalIp);

    ACE_Time_Value timeout(2);
    int nRet = ERR_FAIL;

    if(Connector.connect(*pstream, remote_sap, &timeout, local_sap) != 0)
    //���Ӳ��ɹ�
    {
        TRACE(MTS_COMM, S_COMM_CONNECT_BS, szSvrIp, nGTPSvrPort);
        delete pstream;
        return nRet;
    }

    //���ӳɹ���
    m_pGtpTcpCliSocket = new CGtpTcpCliSocket(pstream);
    m_pGtpTcpCliSocket->m_pCommRouter = this;

    //ע��Socket�Ķ�д�¼�
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

//һ�붨ʱ��
void CGTPTcpCliCommRouter::Timer1Sec(void)
{
    if (m_pGtpTcpCliSocket == 0)
    {
        if (m_nReConnectTimer >= 5)
        {
            //��������������
            m_nReConnectTimer = 0;

            //��BS��������
            Connect();
        }
        else
        {
            m_nReConnectTimer++;
        }
    }
}

//��������
int CGTPTcpCliCommRouter::Send(const MSG_INTRA* const pMsg)
{
    if (m_pGtpTcpCliSocket != 0)
    {
        return m_pGtpTcpCliSocket->Send(pMsg->Data);
    }

    return ERR_SOCKET_NOT_PRESENT;
}

//��GTP'Э��UDP��ʽ�Ŀͻ��˵ĳ���
//���캯��
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

//��������
CGTPUdpCliCommRouter::~CGTPUdpCliCommRouter()
{
    if(m_pGtpUdpCliSocket)
    {
        //����Socket��m_pCommRouter���㣬�ɱ�����ע��Socket
        //ʱ������ɾ����m_pCommRouter����ĺ���
        m_pGtpUdpCliSocket->m_pCommRouter = 0;
        m_pGtpUdpCliSocket = NULL;
    }
}

//ɾ��SOCKET
void CGTPUdpCliCommRouter::DelGtpSocket(CGtpUdpCliSocket* pGtpSocket)
{
    ASSERT(m_pGtpUdpCliSocket == pGtpSocket);

    if(m_pGtpUdpCliSocket && m_pGtpUdpCliSocket == pGtpSocket)
    {
        m_pGtpUdpCliSocket->m_pCommRouter = 0;
        m_pGtpUdpCliSocket = NULL;
    }
}

//��ʼ��SOCKET
int CGTPUdpCliCommRouter::Init()
{
    //�������ļ��ж�ȡGTP' ���ص�IP��ַ
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    pIniFile->Open();

    char szLocalIp[20];
    pIniFile->GetString(CFG_SEC_BS, 
                        CFG_BS_KEY_GTP_CLI_IP, 
                        CFG_BS_KEY_GTP_CLI_IP_DEFAULT,
                        szLocalIp, 
                        sizeof(szLocalIp));
    
    //�������ļ��ж�ȡGTP' ���صĶ˿ںţ�ȱʡֵΪ3386
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

    //ע��Socket�¼�
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

//��������
int CGTPUdpCliCommRouter::Send(const MSG_INTRA* const pMsg)
{
    if (m_pGtpUdpCliSocket != 0)
    {
        return m_pGtpUdpCliSocket->Send(pMsg);
    }

    return ERR_SOCKET_NOT_PRESENT;
}

//���¼�
void CGTPUdpCliCommRouter::Timer1Sec(void)
{
}

