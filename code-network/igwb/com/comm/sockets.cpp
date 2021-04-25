#include "sockets.h"
#include "comm_router.h"
#include "comm_layer.h"
#include "../include/toolbox.h"
#include <algorithm>
CCommMutex* CCommMutex::g_pTheMutex = 0;

/******************************************************
// ������:  CCommMutex::CCommMutex
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ���캯��
// ����:
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CCommMutex::CCommMutex()
{
    //
}


/******************************************************
// ������:  CCommMutex::~CCommMutex
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��������
// ����:
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CCommMutex::~CCommMutex()
{
    //
}


/******************************************************
// ������:  CCommMutex::Instance
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��õ�ʵ�������ָ��
// ����:
// ����ֵ: CCommMutex* const
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CCommMutex* const CCommMutex::Instance()
{
    if(g_pTheMutex == NULL)
    {
        //����һ��CCommMutex����
        g_pTheMutex = new CCommMutex;
    }
    return g_pTheMutex;
}


/******************************************************
// ������:  CCommMutex::Destroy
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ɾ����ʵ������
// ����:
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CCommMutex::Destroy()
{
    if(g_pTheMutex != NULL)
    {
        delete g_pTheMutex;
        g_pTheMutex = 0;
    }
}


/******************************************************
// ������:  CCommMutex::Acquire
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��û�����
// ����:
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CCommMutex::Acquire()
{
    m_AceMutex.acquire();
}


/******************************************************
// ������:  CCommMutex::Release
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    �ͷŻ�����
// ����:
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CCommMutex::Release()
{
    m_AceMutex.release();
}

//////////////////////////////////////////////////////////
int CIPFilter::Init()
{
	//�������ļ�
	static BYTE bInitFirst = 0;
	if(CMsgProcessor::g_pt < PT_AP_BASE 
        && CMsgProcessor::g_pt != PT_OM)
	{
		return ERR_FAIL;
	}
	if(bInitFirst)
	{
		return ERR_SUCCESS;
	}
	char szAPName[24];
    if(CMsgProcessor::g_pt == PT_OM)
    {
        strncpy(szAPName,CFG_SEC_MML,24);
    }
    else
    {
        int nAPID = CMsgProcessor::g_pt + 1 - PT_AP_BASE;
        SNPRINTF(szAPName,24,"%s%d",CFG_SEC_AP,nAPID);
    }

    CINIFile ini(GetCfgFilePath());
	ini.Open();
	m_nMode = (BYTE)ini.GetInt(szAPName,CFG_AP_KEY_IP_MODE,CFG_AP_KEY_IP_MODE_DEFAULT);
	if(m_nMode == 2)
		return ERR_SUCCESS;
    int nNumber = ini.GetInt(szAPName,CFG_AP_IP_LIST_NUM,0); 
    char szIPName[16]; 
	char szIP[24];
	ACE_INET_Addr addr;
    for(int i = 1; i <= nNumber; i++)
	{
        SNPRINTF(szIPName,16,"%s%d",CFG_AP_IP_LIST_PREFIX,i);
        ini.GetString(szAPName,szIPName,"127.0.0.1",szIP,24);
		addr.set((u_short)0,szIP);
		m_IPList.push_back((UINT4)(addr.get_ip_address()));
	}
	//�ر������ļ��������� 
    ini.Close();
    bInitFirst = 1;
	return ERR_SUCCESS;
}
int CIPFilter::Filter(UINT4 uIP)
{
	LIST<UINT4>::iterator it = std::find(m_IPList.begin(),
                                    m_IPList.end(), uIP);
	if(m_nMode == 0 || m_nMode == 1)
	{
		if(m_nMode == 0) //����ģʽ
		{
			if(it == m_IPList.end())
			    return 0;
			else
			    return 1;
		}
		else //����ģʽ
		{
			if(it == m_IPList.end())
				return 1;
			else
				return 0;
		}
	}
	else
	{
		return 0;
	}
		    
}
/******************************************************
// ������: 	CSocketImpl::CSocketImpl
// ����:    Wangfeng
// ʱ��:    01-10-20
// ����:    ���캯��
// ����:
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
CSocketImpl::CSocketImpl()
{
    m_lTimeNoData = 0;
    m_pCommTask   = 0;
    m_bFailure    = FALSE;
}


/******************************************************
// ������: 	CSocketImpl::~CSocketImpl
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
CSocketImpl::~CSocketImpl()
{
    m_pCommTask = 0;
}


/******************************************************
// ������:  CSocketImpl::RemoveHandler
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��ACE_Reactor��ע���¼�
// ����:
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CSocketImpl::RemoveHandler()
{
    //������������CommTask����Ĵ�ɾ���б���CommTask��
    //��һ���¼�ѭ�����ٻص�handle_close()��������������
    //ԭ����UNIX��NT��handle_close()�����Ļص���ʽ������
    //��ͬ����NT�����첽�ص���������һ���¼�ѭ�����ٵ���
    //����UNIX����ͬ����ע���������¼����������DONT_CALL
    //����ACE��ͬ���ص�handle_close()���������������
    if(m_pCommTask)
    {
        m_pCommTask->ScheduleRemove(this);
    }

    m_bFailure = TRUE;
}



/******************************************************
// ������: 	CSocketImpl::SOCK_handle_output
// ����:    Wangfeng
// ʱ��:    01-10-20
// ����:    �������ṩʵ�ʴ���SOCKET�ϵ�����¼����麯���ӿ�
// ����:
//       ����1 :void
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
int CSocketImpl::SOCK_handle_output(void)
{ 
    return 0;
}


/******************************************************
// ������: 	CSocketImpl::SOCK_handle_close
// ����:    Wangfeng
// ʱ��:    01-10-20
// ����:    �������ṩʵ�ʴ���SOCKET�ϵĹر��¼����麯���ӿ�
// ����:
//       ����1 :void
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
int CSocketImpl::SOCK_handle_close(void)
{ 
    return 0;
}


/******************************************************
// ������:  CSocketImpl::handle_input
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��ACE_Reactor������ĳ��CSocketImpl��������
//          SOCKET�����¼�������øö���ı�����
// ����:
//       ����1 :ACE_HANDLE fd = ACE_INVALID_HANDLE
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CSocketImpl::handle_input(ACE_HANDLE fd)
{
    //����ֵ
    int nRetVal = 0;

    //��λ��ʱ������
    m_lTimeNoData = 0;

    //���뻥������ʹ��Ȩ
    CCommMutex::Instance()->Acquire();

    if (!m_bFailure)
    {
        //����SOCKET�ϵ������¼�
        //�������غ��SOCK_handle_input������ɶ������¼���ʵ�ʴ���
        nRetVal = SOCK_handle_input();
        if(nRetVal != ERR_SUCCESS)
        {
            RemoveHandler();
        }
    }

    //�ͷŻ�������ʹ��Ȩ
    CCommMutex::Instance()->Release();

    return 0;
}


/******************************************************
// ������:  CSocketImpl::handle_output
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��ACE_Reactor������ĳ��CSocketImpl��������
//          SOCKET����¼�������øö���ı�����
// ����:
//       ����1 :ACE_HANDLE fd
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CSocketImpl::handle_output(ACE_HANDLE fd)
{
    //����ֵ
    int nRetVal = 0;

    //��λ��ʱ������
    m_lTimeNoData = 0;

    //���뻥������ʹ��Ȩ
    CCommMutex::Instance()->Acquire();

    reactor()->cancel_wakeup(this, ACE_Event_Handler::WRITE_MASK);

    if(!m_bFailure)
    {
        //����SOCKET�ϵ������¼�
        //�������غ��SOCK_handle_output������ɶ������¼���ʵ�ʴ���
        nRetVal = SOCK_handle_output();
        if(nRetVal != ERR_SUCCESS)
        {
            RemoveHandler();
        }
    }

    //�ͷŻ�������ʹ��Ȩ
    CCommMutex::Instance()->Release();

    return 0;
}


/******************************************************
// ������:  CSocketImpl::handle_close
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��ACE_Reactor������ĳ��CSocketImpl��������
//          SOCKET�ر��¼�������øö���ı�����
// ����:
//       ����1 :ACE_HANDLE handle
//       ����2 :ACE_Reactor_Mask close_mask
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CSocketImpl::handle_close(ACE_HANDLE handle,
                              ACE_Reactor_Mask close_mask)
{
    //����ֵ
    int nRetVal = 0;

    //��λ��ʱ������
    m_lTimeNoData = 0;

    //���뻥������ʹ��Ȩ
    CCommMutex::Instance()->Acquire();

    //����SOCKET�ϵ������¼�
    //�������غ��SOCK_handle_close ������ɶԹر��¼���ʵ�ʴ���
    nRetVal = SOCK_handle_close();

    //�ͷŻ�������ʹ��Ȩ
    CCommMutex::Instance()->Release();

    //ɾ��������
    //����ζ��SOCKET���ر�ʱ��ʹ���߲�����ʽ��ɾ��CSocketImpl�������
    delete this;

    return nRetVal;
}


/******************************************************
// ������:  CFrmSocket::CFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ���캯��
// ����:
//       ����1 :ACE_SOCK_Stream* pSocketStream
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CFrmSocket::CFrmSocket(ACE_SOCK_Stream* pSocketStream)
{
    ASSERT(pSocketStream != 0);

    m_pSocketStream = pSocketStream;

    //����һ������Ϊm_lRecvBufSize���ڴ������������ڴ���
    //��ָ�븳ֵ��m_pRecvBuf
    m_lRecvBufSize = 4 * 1024;
    m_pRecvBuf = new char[m_lRecvBufSize];
    m_lBytesReceived = 0;

    m_bLastOK = TRUE;
    m_lLastFrameLen = 0;

    m_lOutFramesMax = 1000;
    m_pSendFrame = 0;
    m_lBytesSent = 0;

    m_pCommRouter = 0;
}


/******************************************************
// ������:  CFrmSocket::~CFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��������
// ����:
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CFrmSocket::~CFrmSocket()
{
    //���������
    Clear();
}

/******************************************************
// ������:  CFrmSocket::Clear
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ���������
// ����:
//       ����1 :void
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CFrmSocket::Clear(void)
{
    //�ر�SOCKET����ɾ��*m_pSocketStream����
    if (m_pSocketStream != 0)
    {
        m_pSocketStream->close();
        delete m_pSocketStream;
        m_pSocketStream = 0;
    }

    //�ͷŽ��ջ�����
    if (m_pRecvBuf != 0)
    {
        delete [] m_pRecvBuf;
        m_pRecvBuf = 0;
    }

    //�ͷŽ��ն���
    while (m_InFrames.size() > 0)
    {
        char* pFrame = m_InFrames.front();
        m_InFrames.pop_front();
        delete pFrame;
    }

    //�ͷŷ��ͻ�����
    if (m_pSendFrame != 0)
    {
        delete m_pSendFrame;
        m_pSendFrame = 0;
    }

    //�ͷŷ��Ͷ���
    while (m_OutFrames.size() > 0)
    {
        char* pFrame = m_OutFrames.front();
        m_OutFrames.pop_front();
        delete pFrame;
    }
}

/******************************************************
// ������:  CFrmSocket::get_handle
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ACE_Reactor��ò���SOCKET�¼�����Ļص�����
//          ��CFrmSocket����ע�ᵽACE_Reactor�����
//          ACE_Reactor�����ص�CFrmSocket�����get_handle
//          �������Ӷ���ò���SOCKET�¼��ľ�����������Ƕ�
//          ACE_Event_Handler�ṩ���麯��get_handle������
// ����:
//       ����1 :void
// ����ֵ: ACE_HANDLE ����SOCKET�¼���ACE_SOCK_Stream���ľ��
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
ACE_HANDLE CFrmSocket::get_handle(void) const
{
    ASSERT(m_pSocketStream != 0);

    //����*m_SocketStream �����еľ��
    return m_pSocketStream->get_handle();
}


/******************************************************
// ������:  CFrmSocket::SOCK_handle_input
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ����SOCKET�����¼�
//          �������Ƕ�CSocketImpl�ṩ���麯��SOCK_handle_input
//          �����أ��ڱ������У����Ƚ����ݽ��յ��������У�
//          Ȼ��ӻ���������ȡ����֡����������֡���뵽��
//          �ն����У������ն������Ѿ���������������֡
//          �ύ���ϲ㴦��
// ����:
//       ����1 :void
// ����ֵ: int 0�����ɹ���
//            -1����ʧ�ܣ��⽫����ע��������
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CFrmSocket::SOCK_handle_input(void)
{
    //�������ݣ���ʱΪ��
    ACE_Time_Value timeout(0);
    int nRet = m_pSocketStream->recv_n((void *)m_pRecvBuf,
                                        m_lRecvBufSize,
                                        &timeout,
                                        (size_t *)(&m_lBytesReceived));

    if(0 >= m_lBytesReceived)
    {
        // * - On timeout, -1 is returned, errno == ETIME.
        if(-1 == nRet && ETIME == errno)
        {
            //TRACE(MTS_COMM, "nRet = %d, errno = %d", nRet, errno);
            return 0;
        }

        int nErr = ACE_OS::last_error();
        switch(nErr)
        {
        case EWOULDBLOCK:
            return 0;
            
        default:
            //�����Զ��ѹر�Socket����
            TRACE(MTS_COMM, S_COMM_RECV_TCPIP_PACKAGE, 
                  m_addrRemote.get_host_addr(), 
                  m_addrRemote.get_port_number(), nErr, 
                  ACE_OS::strerror(nErr));
            return -1;
        }
    }

    //�ӽ��ջ���������ȡ����֡��������ȡ��������֡��ָ�����
    //�����ն��еĶ�β
    ReceiveFrames();

    //���ϲ��ύ����
    int nSize;
    while ((nSize = m_InFrames.size()) > 0)
    {
        if((1 == nSize) && (FALSE == m_bLastOK))
        //������һ֡����δ������ϣ����˳�
        {
            break;
        }
        char* pFrame = m_InFrames.front();
        m_InFrames.pop_front();
        OnReceiveFrame(pFrame);
    }
    return 0;
}


/******************************************************
// ������: 	CFrmSocket::GetAddrRemote
// ����:    Wangfeng
// ʱ��:    01-10-20
// ����:    ��öԶ˵�ַ
// ����:
// ����ֵ: ACE_INET_Addr& 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
ACE_INET_Addr& CFrmSocket::GetAddrRemote()
{
    return m_addrRemote;
}


/******************************************************
// ������: 	CFrmSocket::GetSockStream
// ����:    Wangfeng
// ʱ��:    01-10-20
// ����:    ACE_SOCK_Stream��װ��������ʽ�������ݵ�SOCKET
// ����:
// ����ֵ: ACE_SOCK_Stream* 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
ACE_SOCK_Stream* CFrmSocket::GetSockStream()
{
    return m_pSocketStream;
}


/******************************************************
// ������:  CFrmSocket::OnReceiveFrame
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ���ϲ��ύһ֡����
// ����:
//       ����1 :void* pAFrame
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CFrmSocket::OnReceiveFrame(void* pAFrame)
{
    ASSERT(pAFrame != 0);
    delete pAFrame;
}

/******************************************************
// ������:  CFrmSocket::Send
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��������֡
// ����:
//       ����1 :const void* const pAFrame
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CFrmSocket::Send(const void* const pAFrame)
{
    ASSERT(pAFrame != 0);

    if (m_bFailure) 
    {
        return ERR_PEER_NOT_REACHABLE;
    }

    if (m_OutFrames.size() < m_lOutFramesMax)
    {
        //��õ�ǰ����֡�ĳ���
        long lFrameLen = GetFrameSize((char*)pAFrame);
        //����һ������Ϊ��ǰ����֡���ȵ��ڴ���
        char* pBuf = new char[lFrameLen];

        //����ACE_OS::memcpy��������pAFrameָ������ݿ�
        //�����´������ڴ�����
        ACE_OS::memcpy(pBuf, pAFrame, lFrameLen);

        //���´������ڴ�����ָ����뵽���Ͷ��еĶ�β
        m_OutFrames.push_back(pBuf);

        int nRet = ProcessPendingWrite();
        if (ERR_SUCCESS != nRet)
        {
            RemoveHandler();
        }

        return nRet;
    }

    return ERR_SEND_QUEUE_FULL;
}

/******************************************************
// ������:  CFrmSocket::SOCK_handle_output
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ����SOCKET����¼�
// ����:
//       ����1 :void
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CFrmSocket::SOCK_handle_output(void)
{
    return ProcessPendingWrite();
}

/******************************************************
// ������:  CFrmSocket::ProcessPendingWrite
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��FIFO��ʽ�����Ͷ����е�����֡���ͳ�ȥ��
//          ֱ��SOCKET������
// ����:
//       ����1 :void
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CFrmSocket::ProcessPendingWrite(void)
{
    while ((m_pSendFrame != 0) || (m_OutFrames.size() > 0))
    {
        if (m_pSendFrame != 0)
        {
            //�����麯��GetFrameSize,���m_pSendFrameָ��
            //������֡�ĳ���
            long lFrameLen = GetFrameSize(m_pSendFrame);
            //�����m_pSendFrameָ�������֡�д������ֽ���
            long lLeftLen = lFrameLen - m_lBytesSent;
            //�����m_pSendFrameָ�������֡�д����͵ĵ�һ
            //���ֽڵ�λ��
            char* pBytesLeft = m_pSendFrame + m_lBytesSent;
            while (lLeftLen > 0)
            {
                size_t lWrittenLen = 0;
                ACE_Time_Value timeout(0);
                int nRet = m_pSocketStream->send_n(pBytesLeft,
                                                    lLeftLen,
                                                    &timeout,
                                                    &lWrittenLen);
                if (lWrittenLen == 0)
                {
                    int nRetErr = ACE_OS::last_error();
                    switch(nRetErr)
                    {
                    case 0:
                    case EWOULDBLOCK:   //��������ʱSOCKET������
                    case ETIME:         //�������ݳ�ʱ
                        m_pCommTask->ScheduleWakeup(this);
                        return 0;

                    default:            //��������ʱ�����˴���
                        TRACE(MTS_COMM, S_COMM_SEND_TCPIP_PACKAGE, 
                              m_addrRemote.get_host_addr(),
                              m_addrRemote.get_port_number(),
                              ACE_OS::last_error(), 
                              ACE_OS::strerror(ACE_OS::last_error()));
                        return ERR_SOCKET_SEND_MSG;
                    }
                }
                else
                {
                    //���¼����m_pSendFrameָ�������֡�д������ֽ�����
                    m_lBytesSent += lWrittenLen;
                    lLeftLen -= lWrittenLen;
                    //���¼����m_pSendFrameָ�������֡�д����͵ĵ�һ
                    //���ֽڵ�λ�ã�
                    pBytesLeft += lWrittenLen;

                    //��λ��ʱ������
                    m_lTimeNoData = 0;
                }
            }
            //ɾ��m_pSendFrameָ�������֡
            delete m_pSendFrame;
            m_pSendFrame = NULL;
            m_lBytesSent = 0;
        }

        //��÷��Ͷ��е�ͷԪ��ָ�������֡
        if (m_OutFrames.size() > 0)
        {
            m_pSendFrame = m_OutFrames.front();
            m_OutFrames.pop_front();
            m_lBytesSent = 0;
        }
    }

    return 0;
}


/******************************************************
// ������:  CFrmSocket::SOCK_handle_close
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ����SOCKET�ر��¼�
// ����:
//       ����1 :void
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CFrmSocket::SOCK_handle_close(void)
{
    if(m_pCommRouter)
    {
        m_pCommRouter->DelFrmSocket(this);
    }
    return 0;
}


/******************************************************
// ������:  CBinFrmSocket::CBinFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ���캯��
// ����:
//       ����1 :ACE_SOCK_Stream* pSocketStream
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CBinFrmSocket::CBinFrmSocket(ACE_SOCK_Stream* pSocketStream)
                             :CFrmSocket(pSocketStream)
{
    m_lHeadLen = 0;
    m_pFrameHead = 0;
}


/******************************************************
// ������:  CBinFrmSocket::~CBinFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��������
// ����:
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CBinFrmSocket::~CBinFrmSocket()
{
    //�ͷ���ʱ��ŵ�ǰ���ڽ��յĶ�����֡��֡ͷ����
    if (m_pFrameHead != 0)
    {
        delete [] m_pFrameHead;
        m_pFrameHead = 0;
    }
}

/******************************************************
// ������:  CBinFrmSocket::ReceiveFrames
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    �ӽ��ջ���������ȡ������֡����������ȡ������
//          ������֡��ָ����뵽���ն��еĶ�β
// ����:
//       ����1 :void
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CBinFrmSocket::ReceiveFrames(void)
{
    //��ʼ��һ��ָ�룬��ָ��ָ����ջ������п�������ȡ������
    //֡�ĵ�һ���ֽ�
    char* pValidBuf = m_pRecvBuf;
    //��Ҫ�������ֽ���
    long lBytesNeeded = 0;
    //ʵ�ʿ������ֽ���
    long lBytesCopied = 0;
    char* pLastFrame = 0;

    while (m_lBytesReceived > 0)
    {
        pLastFrame = 0;

        //by ldf 2002-12-09 ��Ӧ���ⵥ D19623
        if (m_lLastFrameLen >= m_lHeadLen)
        //���յ������ݴ���֡ͷ����ʱ�����ݷ��ڽ��ն����е�β��
        //��ʱm_pFrameHead���Ա����˵�ǰ֡��֡ͷ����
        {
            ASSERT(FALSE == m_bLastOK);

            //��ý��ն��еĶ�βԪ��
            pLastFrame = m_InFrames.back();
        }
        else if (m_lLastFrameLen < m_lHeadLen)
        //�Ƚ�����֡ͷ���ݣ��ŵ�m_pFrameHead��
        //���յ�������С�ڵ���֡ͷ����ʱ�����ݷ���m_pFrameHead��
        {
            m_bLastOK = FALSE;

            //����Ϊ�������ؽ���֡ͷ����Ҫ�������ֽ���
            lBytesNeeded = m_lHeadLen - m_lLastFrameLen;

            //����Ϊ�������ؽ���֡ͷ��ʵ�ʿɿ������ֽ���
            lBytesCopied = min(lBytesNeeded, m_lBytesReceived);

            ACE_OS::memcpy(m_pFrameHead + m_lLastFrameLen,
                           pValidBuf,
                           lBytesCopied);

            //�������ջ������п����ֽڵ���ʼλ�ã��ѽ����������һ֡�ĳ���
            pValidBuf += lBytesCopied;
            m_lBytesReceived -= lBytesCopied;
            m_lLastFrameLen += lBytesCopied;

            //�����ջ������е������Ѿ��������꣬���������֡����ȡ����
            if ((m_lBytesReceived == 0) && (m_lLastFrameLen < m_lHeadLen))
            {
                return;
            }
        }
        //�޸����

        /*�������������ϴ�û�н���������֡������������*/
        /*�ڴˣ��϶����յ�����֡ͷ���ȵ�����          */
        /*��m_lLastFrameLen>=m_lHeadLen               */
        /*��m_pFrameHead�ѿ϶�����˵�ǰ֡��֡ͷ����  */

        //��ȡ��ǰ֡�ĳ���
        long lFrameSize = GetFrameSize(m_pFrameHead);

        //����յ�����֡����ע����Socket�¼������˳�
        if(lFrameSize > MSG_INTRA_MAXSIZE - MSG_INTRA_HEADLENGTH)
        {
            TRACE(MTS_COMM, S_COMM_RECV_EXCEED_FRAME, 
                  m_addrRemote.get_host_addr(), 
                  m_addrRemote.get_port_number(),
                  lFrameSize);
            RemoveHandler();
            return;
        }

        //���ոպ�����һ��֡ͷʱ��Ϊ��ǰ֡����һ���ڴ�����
        if(pLastFrame == 0)
        {
            ASSERT(m_lLastFrameLen == m_lHeadLen);

            pLastFrame = new char[lFrameSize];
            ASSERT(pLastFrame != 0);
            ACE_OS::memcpy(pLastFrame, m_pFrameHead, m_lHeadLen);

            //����ǰ֡��ָ����뵽���ն��еĶ�β
            m_InFrames.push_back(pLastFrame);

            m_bLastOK = FALSE;
        }

        //����Ϊ���յ�ǰ֡������������Ҫ���ֽ���
        lBytesNeeded = lFrameSize - m_lLastFrameLen;

        //����Ϊ���յ�ǰ֡����������ʵ�ʿɿ������ֽ���
        lBytesCopied = min(lBytesNeeded, m_lBytesReceived);

        //����pValidBuf��ʼ��lBytesCopied���ֽ�׷�ӵ�pLastFrameָ��Ļ�������
        ACE_OS::memcpy(pLastFrame + m_lLastFrameLen,
                        pValidBuf,
                        lBytesCopied);

        //�������ջ������п����ֽڵ���ʼλ�ã��ѽ����������һ֡�ĳ���
        pValidBuf += lBytesCopied;
        m_lBytesReceived -= lBytesCopied;
        m_lLastFrameLen += lBytesCopied;

        //�ж��Ƿ������ؽ����˵�ǰ֡
        if (lBytesNeeded == lBytesCopied)
        {
            m_bLastOK = TRUE;
            m_lLastFrameLen = 0;
        }
    }
}

/******************************************************
// ������:  CBinFrmSocket::GetFrameSize
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��ȡ����֡�ĳ���
// ����:
//       ����1 :const char* const pAFrame
// ����ֵ: long
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
long CBinFrmSocket::GetFrameSize(const char* const pAFrame)
{
    //֡ͷ�ĳ���m_lHeadLen�����������ĳ��ȣ�
    return GetDataSize(pAFrame) + m_lHeadLen;
}

/******************************************************
// ������:  CMemFrmSocket::CMemFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ���캯��
// ����:
//       ����1 :ACE_SOCK_Stream* pSocketStream
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CMemFrmSocket::CMemFrmSocket(ACE_SOCK_Stream* pSocketStream)
                             :CBinFrmSocket(pSocketStream)
{
    //��ʼ��֡ͷ�ĳ���
    m_lHeadLen = _MEMFRAME_HEAD_LENGTH;

    //������ʱ���֡ͷ���ݵĻ�����
    m_pFrameHead = new char[m_lHeadLen];

    m_bPrimary   = FALSE;
    m_nModuleNum = APP_MID_NULL;
}


/******************************************************
// ������: 	CMemFrmSocket::~CMemFrmSocket
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
CMemFrmSocket::~CMemFrmSocket()
{
    //
}


/******************************************************
// ������: 	CMemFrmSocket::GetbPrimary
// ����:    Wangfeng
// ʱ��:    01-10-20
// ����:    ����Ƿ������˿ڵ���Ϣ
// ����:
// ����ֵ: BOOL 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
BOOL CMemFrmSocket::GetbPrimary()
{
    return m_bPrimary;
}


/******************************************************
// ������: 	CMemFrmSocket::SetbPrimary
// ����:    Wangfeng
// ʱ��:    01-10-20
// ����:    �����Ƿ������˿ڵ���Ϣ
// ����:
//       ����1 :BOOL bPrimary
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
void CMemFrmSocket::SetbPrimary(BOOL bPrimary)
{
    m_bPrimary = bPrimary;
}


/******************************************************
// ������: 	CMemFrmSocket::GetModuleNum
// ����:    Wangfeng
// ʱ��:    01-10-20
// ����:    �������ģ���
// ����:
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
int CMemFrmSocket::GetModuleNum()
{
    return m_nModuleNum;
}


/******************************************************
// ������: 	CMemFrmSocket::SetModuleNum
// ����:    Wangfeng
// ʱ��:    01-10-20
// ����:    ��������ģ���
// ����:
//       ����1 :int nNum
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
void CMemFrmSocket::SetModuleNum(int nNum)
{
    m_nModuleNum = nNum;
}


/******************************************************
// ������:  CMemFrmSocket::GetDataSize
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ���һ֡SMemFrameʵ�����������ĳ���
// ����:
//       ����1 :const char* const pAFrame
// ����ֵ: long
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
long CMemFrmSocket::GetDataSize(const char* const pAFrame)
{
    return ((SMemFrame*)pAFrame)->nLen();
}


/******************************************************
// ������:  CMemFrmSocket::OnReceiveFrame
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ����������CSlideWndCommRouter�����ύ����
// ����:
//       ����1 :void* pAFrame
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CMemFrmSocket::OnReceiveFrame(void* pAFrame)
{
	ASSERT(pAFrame != 0);
    if(m_pCommRouter != 0)
    {
        m_pCommRouter->OnReceiveEx(this, pAFrame);
    }

    //ɾ��������֡
    delete pAFrame;
}


//����Զ2002.04.08���
//���캯��
CSysFrmSocket::CSysFrmSocket(ACE_SOCK_Stream* pSocketStream)
					: CMemFrmSocket(pSocketStream)
{
    //��ʼ��֡ͷ�ĳ���
    m_lHeadLen = _SYSFRAME_HEAD_LENGTH;

    if(NULL != m_pFrameHead)
    {
        delete[] m_pFrameHead;
    }

    //������ʱ���֡ͷ���ݵĻ�����
    m_pFrameHead = new char[m_lHeadLen];
}

//��������
CSysFrmSocket::~CSysFrmSocket()
{
}

//���һ֡SSysFrameʵ�����������ĳ���
long CSysFrmSocket::GetDataSize(const char* const pAFrame)
{
    return ((SSysFrame* )pAFrame)->nLen();
}

//����������CSoftXCommRouter�����ύ����
void CSysFrmSocket::OnReceiveFrame(void* pAFrame)
{
	if(m_pCommRouter != NULL)
	{
		m_pCommRouter->OnReceiveEx(this, pAFrame);
	}

	//ɾ��������֡
	delete pAFrame;
}
//����Զ2002.04.08��ӽ���


/******************************************************
// ������:  CMsgFrmSocket::CMsgFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ���캯��
// ����:
//       ����1 :ACE_SOCK_Stream* pSocketStream
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CMsgFrmSocket::CMsgFrmSocket(ACE_SOCK_Stream* pSocketStream)
                             :CBinFrmSocket(pSocketStream)
{
    //��ʼ��֡ͷ�ĳ���
    m_lHeadLen = MSG_INTRA_HEADLENGTH;

    //������ʱ���֡ͷ���ݵĻ�����
    m_pFrameHead = new char[m_lHeadLen];
}


/******************************************************
// ������:  CMsgFrmSocket::~CMsgFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��������
// ����:
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CMsgFrmSocket::~CMsgFrmSocket()
{
    //
}


/******************************************************
// ������:  CMsgFrmSocket::GetDataSize
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ���һ֡MSG_INTRAʵ�����������ĳ���
// ����:
//       ����1 :const char* const pAFrame
// ����ֵ: long
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
long CMsgFrmSocket::GetDataSize(const char* const pAFrame)
{
    return ((MSG_INTRA*)pAFrame)->Len;
}


/******************************************************
// ������:  CMsgFrmSocket::OnReceiveFrame
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ����������CMsgSvrCommRouter�����
//          CMsgCliCommRouter�����ύ����
// ����:
//       ����1 :void* pAFrame
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CMsgFrmSocket::OnReceiveFrame(void* pAFrame)
{
    if(m_pCommRouter != 0)
    {
        m_pCommRouter->OnReceiveEx(this, pAFrame);
    }
    else
    {
        //ֻ�ڲ����ύʱɾ��������֡
        delete pAFrame;
    }
}


/******************************************************
// ������:  CMmlFrmSocket::CMmlFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ���캯��
// ����:
//       ����1 :ACE_SOCK_Stream* pSocketStream
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CMmlFrmSocket::CMmlFrmSocket(ACE_SOCK_Stream* pSocketStream)
                             :CFrmSocket(pSocketStream)
{
    //
}


/******************************************************
// ������:  CMmlFrmSocket::~CMmlFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��������
// ����:
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CMmlFrmSocket::~CMmlFrmSocket()
{
    //
}


/******************************************************
// ������:  CMmlFrmSocket::AddTags
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ����һ���ı�֡��ʼ������ı�־
// ����:
//       ����1 :const char yTag�����ı�֡�Ŀ�ʼ�������־
//       ����2 :const BOOL bBeginTag����������TRUE�����ʾ
//              yTagΪ�ı�֡�Ŀ�ʼ��־�������ʾ
//              yTagΪ�ı�֡�Ľ�����־
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CMmlFrmSocket::AddTags(const char yTag, const BOOL bBeginTag)
{
    if (bBeginTag == TRUE)
    {
        m_yBeginTags.insert(yTag);
    }
    else
    {
        m_yEndTags.insert(yTag);
    }
}



/******************************************************
// ������: 	CMmlFrmSocket::Send
// ����:    Wangfeng
// ʱ��:    01-11-15
// ����:    ����MML�ļ�����֡
// ����:
//       ����1 :const void* const pAFrame
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-11-15      Wangfeng
******************************************************/
int CMmlFrmSocket::Send(const void* const pAFrame)
{
    ASSERT(pAFrame != 0);

    if (m_bFailure) 
    {
        return ERR_PEER_NOT_REACHABLE;
    }
    if (m_OutFrames.size() < m_lOutFramesMax)
    {
        //��õ�ǰ����֡�ĳ���
        long lFrameLen = GetFrameSize((char*)pAFrame);
        //����һ������Ϊ��ǰ����֡���ȵ��ڴ���
        char* pBuf = new char[lFrameLen + 1];

        //����ACE_OS::memcpy��������pAFrameָ������ݿ�
        //�����´������ڴ�����
        strcpy(pBuf, (char*)pAFrame);

        //���´������ڴ�����ָ����뵽���Ͷ��еĶ�β
        m_OutFrames.push_back(pBuf);

        int nRet = ProcessPendingWrite();
        if (ERR_SUCCESS != nRet)
        {
            RemoveHandler();
        }

        return nRet;
    }
    else
    {
        return ERR_SOCKET_SEND_MSG;
    }
}


/******************************************************
// ������:  CMmlFrmSocket::ReceiveFrames
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    �ӽ��ջ���������ȡ�ı�֡����������ȡ��������
//          ��֡��ָ����뵽���ն��еĶ�β
// ����:
//       ����1 :void
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CMmlFrmSocket::ReceiveFrames(void)
{
    //��ʼ��һ��ָ����ջ�������ָ��
    char* pValidBuf = m_pRecvBuf;
    long lBytesNeed = 0;            //��Ҫ�������ֽ���

    while (m_lBytesReceived > 0)
    {
        //���˿�ʼ����
        if(m_yBeginTags.find(pValidBuf[0]) != m_yBeginTags.end())
        {
            pValidBuf++;
            m_lBytesReceived--;
            lBytesNeed = 0;
            if(!m_bLastOK)
            {
                if(m_InFrames.size())
                {
                    delete m_InFrames.back();
                    m_InFrames.pop_back();
                }
                m_bLastOK = TRUE;
            }
            continue;
        }

        /*
            ���˴������������Ѵ���һ�������Ϊ��ʼ���ŵ��ַ�
        */

        //������Ҳ�����������
        if (m_yEndTags.find(pValidBuf[lBytesNeed]) == m_yEndTags.end())
        {
            lBytesNeed++; 
            m_lBytesReceived--;
            if(m_lBytesReceived > 0) continue;
        }

        /*
            ���˴����������������
            1����ǰ��������ֻ��һ���������ţ����������ֻ
               Ҫ�����һ֡��Ϊ��������
            2����ǰ�����������ҵ�һ���������ţ����������
               m_lBytesReceived > 0
            3����ǰ�������Ҳ����������ţ������������
               m_lBytesReceived == 0
        */

        //���ֻ�յ�һ���������ţ��������һ֡Ϊ����
        if(0 == lBytesNeed) 
        {
            m_bLastOK = TRUE;
            pValidBuf++;
            m_lBytesReceived--;
            continue;
        }

        //ƴ֡
        char* pBuf;
        if(!m_bLastOK)
        //��һ�ν���ʱ�����һ֡û�б������ؽ���
        {
            char* pLastFrame = m_InFrames.back();
            m_InFrames.pop_back();
            long lNewFrameSize = GetFrameSize(pLastFrame) + lBytesNeed;
            pBuf = new char[lNewFrameSize + 1];
            strcpy(pBuf, pLastFrame);
            memcpy(pBuf + GetFrameSize(pLastFrame),
                   pValidBuf,
                   lBytesNeed);
            pBuf[lNewFrameSize] = '\0';
            delete pLastFrame;
        }
        else
        {
            pBuf = new char[lBytesNeed + 1];
            memcpy(pBuf, pValidBuf, lBytesNeed);
            pBuf[lBytesNeed] = '\0';
        }

        //���½��ն��е�β��
        m_InFrames.push_back(pBuf);

        //���»�����ʣ���ֽ���
        if(0 == m_lBytesReceived)
        //���ն����в����ڽ�������
        {
            m_bLastOK = FALSE;
            pValidBuf += lBytesNeed;
        }
        else
        {
            m_bLastOK = TRUE;
            m_lBytesReceived--;
            pValidBuf += lBytesNeed + 1;
        }
        lBytesNeed = 0;
    }
}


/******************************************************
// ������:  CMmlFrmSocket::GetFrameSize
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    �������֡����
// ����:
//       ����1 :const char* const pAFrame
// ����ֵ: long
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
long CMmlFrmSocket::GetFrameSize(const char* const pAFrame)
{
    return ACE_OS::strlen(pAFrame);
}


/******************************************************
// ������:  CMmlFrmSocket::OnReceiveFrame
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ����������CMmlCommRouter�����ύ����
// ����:
//       ����1 :void* pAFrame
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CMmlFrmSocket::OnReceiveFrame(void* pAFrame)
{
    if(m_pCommRouter != 0)
    {
        m_pCommRouter->OnReceiveEx(this, pAFrame);
    }

    //ɾ��������֡
    delete pAFrame;
}


/******************************************************
// ������:  CGtpUdpSocket::CGtpUdpSocket
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ���캯��
// ����:
//       ����1 :CGtpUdpCommRouter* const pGtpCommRouter
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CGtpUdpSocket::CGtpUdpSocket(CGtpUdpCommRouter* const pGtpCommRouter)
{
    //GTP'�Ʒѽӿ����� Z30971 2004/06/10,ע�͵�ASSERT
    //ASSERT(pGtpCommRouter != 0);
    m_pGtpCommRouter = pGtpCommRouter;
    //�������ջ�����
    m_pRecvBuf = new char[GTP_INPUT_BUFFER_MAXLENGTH];
}


/******************************************************
// ������:  CGtpUdpSocket::~CGtpUdpSocket
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
CGtpUdpSocket::~CGtpUdpSocket(void)
{
    m_SocketDgram.close();

    //�ͷŽ��ջ�����
    if(m_pRecvBuf)
    {
        delete[] m_pRecvBuf;
        m_pRecvBuf = 0;
    }

    m_pGtpCommRouter = 0;
}


/******************************************************
// ������:  CGtpUdpSocket::Open
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��һ���շ�UDP���ݱ��Ķ˿�
// ����:
//       ����1 :const char* const szIPAddr
//       ����2 :const int nPort
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CGtpUdpSocket::Open(const char* const szIPAddr, const int nPort)
{
	//����IP��ַ�Ͷ˿�
    m_addrLocal.set(nPort, szIPAddr);

    return m_SocketDgram.open(m_addrLocal);
}


/******************************************************
// ������:  CGtpUdpSocket::get_handle
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ����¼����
// ����:
//       ����1 :void
// ����ֵ: ACE_HANDLE
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
ACE_HANDLE CGtpUdpSocket::get_handle(void) const
{
    //����m_SocketDgram �����еľ��
    return m_SocketDgram.get_handle();
}


/******************************************************
// ������:  CGtpUdpSocket::SOCK_handle_input
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ����SOCKET�����¼�
// ����:
//       ����1 :void
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CGtpUdpSocket::SOCK_handle_input(void)
{
    ACE_INET_Addr AddrPeer;
    int nRetBytes = m_SocketDgram.recv(m_pRecvBuf,
                                GTP_INPUT_BUFFER_MAXLENGTH,
                                AddrPeer);
	//������ 2002-12-26 IP��ַ����
	CIPFilter *pFilter = ACE_Singleton<CIPFilter,ACE_Null_Mutex>::instance();
	UINT4 uIP = HTONL(AddrPeer.get_ip_address());

	//by ldf 2003-10-30 ��Ӧ���ⵥ SWPD02155
	UINT4 uIPTmp = (UINT4)(AddrPeer.get_ip_address());
	if(pFilter->Filter(uIPTmp))
	{
		return 0;
	}
	//������
	
    if (nRetBytes <= 0)
    {
        return 0;
    }

    if(m_pGtpCommRouter != 0)
    {
        //����һ��MSG_INTRA��ʵ���������յ������ݱ���������ʵ����Data��
        MSG_INTRA* pMsg   = new(nRetBytes) MSG_INTRA;
        pMsg->CmdCode     = CMD_HOST_DATA;
        pMsg->SenderPid   = PT_OUTSIDE;
        pMsg->SenderMid   = MT_UNKNOWN;
        pMsg->ReceiverPid = CMsgProcessor::GetProcessType();
        pMsg->ReceiverMid = MT_NETWORK;
        pMsg->AppType     = APP_GTP_UDP;
        ACE_OS::memcpy(pMsg->Data, m_pRecvBuf, nRetBytes);

        //���ø�ʵ����SNetAddrPair��
        ((_NetFrame *) pMsg)->SNetAddrPair.ServerIp = HTONL(m_addrLocal.get_ip_address());
        ((_NetFrame *) pMsg)->SNetAddrPair.ServerPort = m_addrLocal.get_port_number();
        ((_NetFrame *) pMsg)->SNetAddrPair.ClientIp = uIP;
        ((_NetFrame *) pMsg)->SNetAddrPair.ClientPort = AddrPeer.get_port_number();

        m_pGtpCommRouter->OnReceive(pMsg);
    }
    return 0;
}


/******************************************************
// ������:  CGtpUdpSocket::SOCK_handle_close
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ����SOCKET�ر��¼�
// ����:
//       ����1 :void
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CGtpUdpSocket::SOCK_handle_close(void)
{
    if(m_pGtpCommRouter)
    {
        m_pGtpCommRouter->DelGtpSocket(this);
    }
    return 0;
}


/******************************************************
// ������:  CGtpUdpSocket::GetAddr
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��ñ������Ӧ��IP��ַ�Ͷ˿ں�
// ����:
//       ����1 :unsigned int& nIPAddr
//       ����2 :int& nPort
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CGtpUdpSocket::GetAddr(unsigned int& nIPAddr, int& nPort)
{
    nIPAddr = HTONL(m_addrLocal.get_ip_address());
    nPort = m_addrLocal.get_port_number();
}


/******************************************************
// ������:  CGtpUdpSocket::Send
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ����һ��UDP���ݱ�
// ����:
//       ����1 :MSG_INTRA* const pMsg
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CGtpUdpSocket::Send(const MSG_INTRA* const pMsg)
{
    ACE_INET_Addr AddrPeer;
    AddrPeer.set(((_NetFrame *)pMsg)->SNetAddrPair.ClientPort,
                 NTOHL(((_NetFrame *)pMsg)->SNetAddrPair.ClientIp));

	CIPFilter *pFilter = ACE_Singleton<CIPFilter,ACE_Null_Mutex>::instance();
	UINT4 uIPTmp = (UINT4)(AddrPeer.get_ip_address());
	
	if(pFilter->Filter(uIPTmp))
	{
		return -1;
	}

    int nSendBytes = m_SocketDgram.send(pMsg->Data,
                                        pMsg->Len,
                                        AddrPeer);

    if(nSendBytes != pMsg->Len)
    {
        TRACE(MTS_COMM, S_COMM_SEND_UPD_PACKAGE, ACE_OS::last_error(), 
              ACE_OS::strerror(ACE_OS::last_error()));
        return ERR_FAIL;
    }
    return ERR_SUCCESS;
}

/******************************************************
// ������:  CABFrmSocket::CABFrmSocket
// ����:    Wangzhaoyang
// ʱ��:    02-07-15
// ����:    ���캯��
// ����:
//       ����1 :ACE_SOCK_Stream* pSocketStream
// ����ֵ: 
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//02-07-15       Wangzhaoyang            ����
******************************************************/
CABFrmSocket::CABFrmSocket(ACE_SOCK_Stream* pSocketStream)
                             :CBinFrmSocket(pSocketStream)
{
    m_lHeadLen = _APPCVTFRM_HEAD_LENGTH;
    m_pFrameHead = new char[m_lHeadLen];
}

/******************************************************
// ������:  CABFrmSocket::~CABFrmSocket
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
CABFrmSocket::~CABFrmSocket()
{

}

/******************************************************
// ������:  CABFrmSocket::GetDataSize
// ����:    Wangzhaoyang
// ʱ��:    02-07-15
// ����:    �������֡��data���ֳ���
// ����:
//       ����1 :const char *const pAFrame
// ����ֵ: 
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//02-07-15       Wangzhaoyang            ����
******************************************************/
long CABFrmSocket::GetDataSize(const char *const pAFrame)
{
	return ntohs(((SAppConvertFrame*)pAFrame)->wLen);
}

/******************************************************
// ������:  CABFrmSocket::OnReceiveFrame
// ����:    Wangzhaoyang
// ʱ��:    02-07-15
// ����:    ����������CAlarmBoxRouter�����ύ����
// ����:
//       ����1 :const char *const pAFrame
// ����ֵ: 
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//02-07-15       Wangzhaoyang            ����
******************************************************/
void CABFrmSocket::OnReceiveFrame(void* pAFrame)
{
	if(m_pCommRouter != NULL)
    {
        m_pCommRouter->OnReceiveEx(this, pAFrame);
    }

    //ɾ��������֡
    delete pAFrame;
}

/******************************************************
// ������:  CGtpFrmSocket::CGtpFrmSocket
// ����:    Wangfeng
// ʱ��:    03-06-28
// ����:    ���캯��
// ����:
//       ����1 :ACE_SOCK_Stream* pSocketStream
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//03-06-28       Wangfeng
******************************************************/
CGtpFrmSocket::CGtpFrmSocket(ACE_SOCK_Stream* pSocketStream)
                             :CBinFrmSocket(pSocketStream)
{
    //��ʼ��֡ͷ�ĳ���
    m_lHeadLen = GTP_FRAME_HEAD_LENGTH_V2;

    //������ʱ���֡ͷ���ݵĻ�����
    m_pFrameHead = new char[m_lHeadLen];
}


/******************************************************
// ������: 	CGtpFrmSocket::~CGtpFrmSocket
// ����:    Wangfeng
// ʱ��:    03-06-28
// ����:    ��������
// ����:
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//03-06-28      Wangfeng
******************************************************/
CGtpFrmSocket::~CGtpFrmSocket()
{
    //
}


/******************************************************
// ������:  CGtpFrmSocket::GetDataSize
// ����:    Wangfeng
// ʱ��:    03-06-28
// ����:    ���һ֡SGtpFrameʵ�����������ĳ���
// ����:
//       ����1 :const char* const pAFrame
// ����ֵ: long
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//03-06-28       Wangfeng
******************************************************/
long CGtpFrmSocket::GetDataSize(const char* const pAFrame)
{
    return ((SGtpFrame*)pAFrame)->GetSize() - m_lHeadLen;
}


/******************************************************
// ������:  CGtpFrmSocket::OnReceiveFrame
// ����:    Wangfeng
// ʱ��:    03-06-28
// ����:    ����������CGtpFrmCommRouter�����ύ����
// ����:
//       ����1 :void* pAFrame
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//03-06-28       Wangfeng
******************************************************/
void CGtpFrmSocket::OnReceiveFrame(void* pAFrame)
{
	ASSERT(pAFrame != 0);
    if(m_pCommRouter != NULL)
    {
        //����һ��MSG_INTRA��ʵ���������յ������ݱ���������ʵ����Data��
        MSG_INTRA* pMsg   = new(((SGtpFrame*)pAFrame)->GetSize()) MSG_INTRA;
        pMsg->CmdCode     = CMD_HOST_DATA;
        pMsg->SenderPid   = PT_OUTSIDE;
        pMsg->SenderMid   = MT_UNKNOWN;
        pMsg->ReceiverPid = CMsgProcessor::GetProcessType();
        pMsg->ReceiverMid = MT_NETWORK;
        pMsg->AppType     = APP_GTP_TCP;
        ACE_OS::memcpy(pMsg->Data, pAFrame, pMsg->Len);

        //���ø�ʵ����SNetAddrPair��
        ((_NetFrame *) pMsg)->SNetAddrPair.ServerIp     = HTONL(m_addrLocal.get_ip_address());
        ((_NetFrame *) pMsg)->SNetAddrPair.ServerPort   = m_addrLocal.get_port_number();
        ((_NetFrame *) pMsg)->SNetAddrPair.ClientIp     = HTONL(m_addrRemote.get_ip_address());
        ((_NetFrame *) pMsg)->SNetAddrPair.ClientPort   = m_addrRemote.get_port_number();

        m_pCommRouter->OnReceive(pMsg);
    }

    //ɾ��������֡
    delete pAFrame;
}

/******************************************************
// ������:  CGtpFrmSocket::SOCK_handle_close
// ����:    Wangfeng
// ʱ��:    03-06-28
// ����:    ����SOCKET�ر��¼�
// ����:
//       ����1 :void
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//03-06-28       Wangfeng
******************************************************/
int CGtpFrmSocket::SOCK_handle_close(void)
{
    if(m_pCommRouter)
    {
        ((CGtpFrmCommRouter*) m_pCommRouter)->DelGtpSocket(this);
    }

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//GTP'�Ʒѽӿ����� Z30971 2004/06/10

//GTP' UDP SOCKET���캯��
CGtpUdpCliSocket::CGtpUdpCliSocket(CGTPUdpCliCommRouter* const pGtpCliCommRouter)
                    :CGtpUdpSocket(NULL)  
         
{
    m_pCommRouter = pGtpCliCommRouter;
}
//GTP' UDP SOCKET��������
CGtpUdpCliSocket::~CGtpUdpCliSocket()
{
    m_pCommRouter = NULL;
}

int CGtpUdpCliSocket::Send(const MSG_INTRA* const pMsg)
{
    ACE_INET_Addr AddrPeer;
    AddrPeer.set(((_NetFrame *)pMsg)->SNetAddrPair.ServerPort,
                 NTOHL(((_NetFrame *)pMsg)->SNetAddrPair.ServerIp));

	CIPFilter *pFilter = ACE_Singleton<CIPFilter,ACE_Null_Mutex>::instance();
	UINT4 uIPTmp = (UINT4)(AddrPeer.get_ip_address());
	
	if(pFilter->Filter(uIPTmp))
	{
		return -1;
	}

    int nSendBytes = m_SocketDgram.send(pMsg->Data,
                                        pMsg->Len,
                                        AddrPeer);

    if(nSendBytes != pMsg->Len)
    {
        TRACE(MTS_COMM, S_COMM_SEND_UPD_PACKAGE, ACE_OS::last_error(), 
              ACE_OS::strerror(ACE_OS::last_error()));
        return ERR_FAIL;
    }
    return ERR_SUCCESS;
}

//����SOCKET�¼�
int CGtpUdpCliSocket::SOCK_handle_input(void)
{
    ACE_INET_Addr AddrPeer;
    int nRetBytes = m_SocketDgram.recv(m_pRecvBuf,
                                GTP_INPUT_BUFFER_MAXLENGTH,
                                AddrPeer);
	
	//IP����
    CIPFilter *pFilter = ACE_Singleton<CIPFilter,ACE_Null_Mutex>::instance();
	UINT4 uIP = HTONL(AddrPeer.get_ip_address());

	UINT4 uIPTmp = (UINT4)(AddrPeer.get_ip_address());
	if(pFilter->Filter(uIPTmp))
	{
		return 0;
	}
	
    if (nRetBytes <= 0)
    {
        return 0;
    }

    if(m_pCommRouter != 0)
    {
        //�����ݷ��͵�ģ�� MT_CDRSENDER
        MSG_INTRA* pMsg   = new(nRetBytes) MSG_INTRA;
        pMsg->CmdCode     = CMD_BS_DATA;
        pMsg->SenderPid   = PT_OUTSIDE;
        pMsg->SenderMid   = MT_UNKNOWN;
        pMsg->ReceiverPid = CMsgProcessor::GetProcessType();
        pMsg->ReceiverMid = MT_CDRSENDER; 
        pMsg->AppType     = APP_BS_GTP_UDP;
        ACE_OS::memcpy(pMsg->Data, m_pRecvBuf, nRetBytes);

        //���ø�ʵ����SNetAddrPair��
        ((_NetFrame *) pMsg)->SNetAddrPair.ServerIp = uIP;
        ((_NetFrame *) pMsg)->SNetAddrPair.ServerPort = AddrPeer.get_port_number();
        ((_NetFrame *) pMsg)->SNetAddrPair.ClientIp = HTONL(m_addrLocal.get_ip_address());
        ((_NetFrame *) pMsg)->SNetAddrPair.ClientPort = m_addrLocal.get_port_number();

        m_pCommRouter->OnReceive(pMsg);
    }
    return 0;
}

int CGtpUdpCliSocket::SOCK_handle_close(void)
{
    if(m_pCommRouter)
    {
        m_pCommRouter->DelGtpSocket(this);
    }
    return 0;
}


//GTP' TCP SOCKET���캯��
CGtpTcpCliSocket::CGtpTcpCliSocket(ACE_SOCK_Stream* pSocketStream)
                   : CGtpFrmSocket(pSocketStream)
{
    m_pSocketStream->get_local_addr(m_addrLocal);
    m_pSocketStream->get_remote_addr(m_addrRemote);
}
//GTP' TCP SOCKET��������
CGtpTcpCliSocket::~CGtpTcpCliSocket()
{
}

//��������֡
void CGtpTcpCliSocket::OnReceiveFrame(void* pAFrame)
{
    ASSERT(pAFrame != 0);
    if(m_pCommRouter != NULL)
    {
        //����һ��MSG_INTRA��ʵ���������յ������ݱ���������ʵ����Data��
        MSG_INTRA* pMsg   = new(((SGtpFrame*)pAFrame)->GetSize()) MSG_INTRA;
        pMsg->CmdCode     = CMD_BS_DATA;
        pMsg->SenderPid   = PT_OUTSIDE;
        pMsg->SenderMid   = MT_UNKNOWN;
        pMsg->ReceiverPid = CMsgProcessor::GetProcessType();
        pMsg->ReceiverMid = MT_CDRSENDER;
        pMsg->AppType     = APP_BS_GTP_TCP;
        ACE_OS::memcpy(pMsg->Data, pAFrame, pMsg->Len);

        //���ø�ʵ����SNetAddrPair��
        ((_NetFrame *) pMsg)->SNetAddrPair.ServerIp     = HTONL(m_addrRemote.get_ip_address());
        ((_NetFrame *) pMsg)->SNetAddrPair.ServerPort   = m_addrRemote.get_port_number();
        ((_NetFrame *) pMsg)->SNetAddrPair.ClientIp     = HTONL(m_addrLocal.get_ip_address());
        ((_NetFrame *) pMsg)->SNetAddrPair.ClientPort   = m_addrLocal.get_port_number();

        m_pCommRouter->OnReceive(pMsg);
    }

    //ɾ��������֡
    delete pAFrame;
}
