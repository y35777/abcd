#include "../utility/config.h"
#include "../include/toolbox.h"
#include "comm_layer.h"
#include "ace/High_Res_Timer.h"

CCommLayer* CCommLayer::m_pCommLayer = NULL;
/******************************************************
// ������:  CCommListener::CCommListener
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ���캯��
// ����:
//       ����1 :unsigned long uAppType
//       ����2 :CCommLayer* const pCommLayer
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CCommListener::CCommListener(unsigned long const uAppType,
                            CCommLayer* const pCommLayer)
{
    m_uAppType = uAppType;
    //ʹ�ÿ���ṩ��ASSERT����
    ASSERT(pCommLayer != 0);
    m_pCommLayer = pCommLayer;
}



/******************************************************
// ������: 	CCommListener::~CCommListener
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
CCommListener::~CCommListener()
{
    m_Acceptor.close();
    m_pCommLayer = 0;
}

/******************************************************
// ������:  CCommListener::get_handle
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��ȡ�¼����
// ����:
//       ����1 :void
// ����ֵ: ACE_HANDLE �����¼����
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
ACE_HANDLE CCommListener::get_handle(void) const
{
    //����m_Acceptor �����еľ��
    return m_Acceptor.get_handle();
}


/******************************************************
// ������:  CCommListener::Open
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��ʼ��ָ����IP��ַ�Ͷ˿�������
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
int CCommListener::Open(const char* const szIPAddr,
                        const int nPort,
                        const BOOL bReuse)
{
    //����IP��ַ�Ͷ˿�
    ACE_INET_Addr LocalAddr(nPort, szIPAddr);
    return m_Acceptor.open(LocalAddr, bReuse);
}


/******************************************************
// ������:  CCommListener::SOCK_handle_input
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
int CCommListener::SOCK_handle_input(void)
{
    //�������Ӳ�����һ����Ӧ��CFrmSocket����Ķ���
    CFrmSocket* pNewSocket = CreateFrmSocket();
    //������ 2002-12-25�޸�
	if(pNewSocket)
	//�޸����
	{
		//֪ͨͨ�Ų������һ��������
		unsigned long uAppType = m_uAppType;
		if ((m_uAppType == APP_MSC_PRIMARY) ||
			(m_uAppType == APP_MSC_SECOND))
		{
			uAppType = APP_MAIN;
		}
		
		//����Զ2002.04.10���
		if ((m_uAppType == APP_SOFTX_PRIMARY) ||
			(m_uAppType == APP_SOFTX_SECOND))
		{
			uAppType = APP_MAIN;
		}
		//����Զ2002.04.10��ӽ���
		
		int nRet = m_pCommLayer->AddFrmSocket(pNewSocket, uAppType);
	}
    return 0;
}


/******************************************************
// ������:  CCommListener::CreateFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    �������Ӳ�����m_uAppType��ȡֵ����һ����Ӧ
//          CFrmSocket����Ķ���
// ����:
//       ����1 :void
// ����ֵ: CFrmSocket*
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CFrmSocket* CCommListener::CreateFrmSocket(void)
{
    //����һ��Socket������
    ACE_SOCK_Stream* pStream = new ACE_SOCK_Stream;

    //������ӦӦ�����͵�Socket
    CFrmSocket* pFrmSocket = 0;
    switch(m_uAppType)
    {
    case APP_MSG :
    //����IPC
        pFrmSocket = new CMsgFrmSocket(pStream);
        break;

    case APP_MSC_PRIMARY :
    //��������������MEM��ͨѶ
        pFrmSocket = new CMemFrmSocket(pStream);
        ((CMemFrmSocket*)pFrmSocket)->SetbPrimary(TRUE);
        break;

    case APP_MSC_SECOND :
    //�����������ı�MEM��ͨѶ
        pFrmSocket = new CMemFrmSocket(pStream);
        ((CMemFrmSocket*)pFrmSocket)->SetbPrimary(FALSE);
        break;

    case APP_SOFTX_PRIMARY :
        pFrmSocket = new CSysFrmSocket(pStream);
        ((CSysFrmSocket*)pFrmSocket)->SetbPrimary(TRUE);
        break;

    case APP_SOFTX_SECOND :
        pFrmSocket = new CSysFrmSocket(pStream);
        ((CSysFrmSocket*)pFrmSocket)->SetbPrimary(FALSE);
        break;

    case APP_MML_NORM :
    //����MML�ͻ��˵���ͨά����ͨѶ
        pFrmSocket = new CMmlFrmSocket(pStream);
        ((CMmlFrmSocket*)pFrmSocket)->AddTags('\r', TRUE);
        ((CMmlFrmSocket*)pFrmSocket)->AddTags('\n', TRUE);
        ((CMmlFrmSocket*)pFrmSocket)->AddTags(0x02, TRUE);
        ((CMmlFrmSocket*)pFrmSocket)->AddTags(0x03, FALSE);
        ((CMmlFrmSocket*)pFrmSocket)->AddTags(';', FALSE);
        break;

    case APP_MML_WARN:
    //����MML�ͻ��˵ĸ澯�ϱ���ͨѶ
    //��Ϊ�澯�˿��ϲ��������ݣ����Բ������øö�����յ��ı�֡
    //�Ŀ�ʼ�ͽ�����־
        pFrmSocket = new CMmlFrmSocket(pStream);
    //by ldf test 2004-08-17 ��Ӧ���ⵥSWPD07814
    ((CMmlFrmSocket*)pFrmSocket)->AddTags('\r', TRUE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags('\n', TRUE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags(0x02, TRUE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags(0x03, FALSE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags(';', FALSE);
        break;

	//by ldf 2004-2-12
    case APP_MML_PERF:
    //����MML�ͻ��˵������ϱ���ͨѶ
    //��Ϊ�����ϱ��˿��ϲ��������ݣ����Բ������øö�����յ��ı�֡
    //�Ŀ�ʼ�ͽ�����־
        pFrmSocket = new CMmlFrmSocket(pStream);
    //by ldf test 2004-08-17 ��Ӧ���ⵥSWPD07814
    ((CMmlFrmSocket*)pFrmSocket)->AddTags('\r', TRUE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags('\n', TRUE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags(0x02, TRUE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags(0x03, FALSE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags(';', FALSE);
        break;
        
    case APP_SYN_CONFIG:
        pFrmSocket = new CMmlFrmSocket(pStream);
    //by ldf test 2004-08-17 ��Ӧ���ⵥSWPD07814
    ((CMmlFrmSocket*)pFrmSocket)->AddTags('\r', TRUE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags('\n', TRUE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags(0x02, TRUE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags(0x03, FALSE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags(';', FALSE);
        break;
        
    case APP_MML_DEBUG:
    //����MML�ͻ��˵����е��Կ�ͨѶ
    //��Ϊ���е��Զ˿��ϲ��������ݣ����Բ������øö�����յ���
    //��֡�Ŀ�ʼ�ͽ�����־
        pFrmSocket = new CMmlFrmSocket(pStream);
        break;
	
	case APP_ALARMBOX:
		//����ʵ����澯�䣨AlarmBox��convertģ��ͨѶ
		pFrmSocket = new CABFrmSocket(pStream);
		break;

	case APP_GTP_TCP:
		pFrmSocket = new CGtpFrmSocket(pStream);
		break;

    default:
        break;
    }

    if(pFrmSocket)
    //��������ɹ�����ȷ��Socket����
    {
        m_Acceptor.accept(*(pFrmSocket->GetSockStream()),
                        &(pFrmSocket->GetAddrRemote()));
        pFrmSocket->MakeLocalAddress();

        //������ 2002-12-24 ���������IP��ַ
        CIPFilter *pFilter = ACE_Singleton<CIPFilter,ACE_Null_Mutex>::instance();
        if(pFilter == NULL)
        {
            return pFrmSocket;
        }
        ACE_INET_Addr &RemoteAddr = pFrmSocket->GetAddrRemote();
        //modifyed by chenliangwei, 2003-12-27, ���MML�ͻ���IP���˲���Ч���ֽ������⣬
        UINT4 uIPTmp = (UINT4)(RemoteAddr.get_ip_address());  
        if(pFilter->Filter(uIPTmp))
        {
            pFrmSocket->GetSockStream()->close();
            delete pFrmSocket;
            return NULL;
        }
        //������
    } // Socket�����ɹ�
    else
    {
        TRACE(MTS_COMM, S_COMM_CRT_SOCKET_ERR, m_uAppType);
        delete pStream;
    }

    return pFrmSocket;
}



/******************************************************
// ������: 	CCommTask::CCommTask
// ����:    Wangfeng
// ʱ��:    01-12-31
// ����:    
// ����:
//       ����1 :CCommTaskMan* pCommTaskMan
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-12-31      Wangfeng
******************************************************/
CCommTask::CCommTask(CCommTaskMan* pCommTaskMan)
{
    ASSERT(0 != pCommTaskMan);

    m_bReactorFull = FALSE;
    m_pCommTaskMan = pCommTaskMan;

    ACE_Timer_Queue* Tq = m_Reactor.timer_queue();
    if(NULL != Tq)
    {
        ACE_High_Res_Timer::global_scale_factor (); 
        Tq->gettimeofday (&ACE_High_Res_Timer::gettimeofday_hr);
    }
}

 

/******************************************************
// ������: 	CCommTask::~CCommTask
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
CCommTask::~CCommTask()
{
    DelScheduleSocket();

    m_pCommTaskMan = 0;
}



/******************************************************
// ������:  CCommTask::open
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ������������һ�����ڼ��Reactor�¼����߳�
// ����:
//       ����1 :void* args = 0
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CCommTask::open(void* args)
{
    //����ACE_Task_Base�ṩ��activate����
    return activate();
}


/******************************************************
// ������:  CCommTask::svc
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    �̺߳���,���������ṩһ��Reactor���¼�ѭ��
// ����:
//       ����1 :void
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CCommTask::svc(void)
{
    static CCommTask* pCommTask = 0;
    static ACE_Time_Value Timer = ACE_High_Res_Timer::gettimeofday();
    ACE_Time_Value time_out = ACE_Time_Value(1);

    if(0 == pCommTask)
    {
        pCommTask = this;
        Timer = ACE_High_Res_Timer::gettimeofday();
    }

    m_Reactor.owner(ACE_Thread::self());
    BOOL bExit = FALSE;
    while(!bExit)
    {
        bExit = m_Reactor.reactor_event_loop_done();
        if (!bExit)
        {
            m_Reactor.handle_events(time_out);
        }

        //�ڶ��CCommTask����ʱ����ֻ֤��һ���̵߳������¼�����
        if(pCommTask == this)
        {
            ACE_Time_Value ace_now = ACE_High_Res_Timer::gettimeofday();
            time_out = ace_now - Timer;
            if(time_out >= ACE_Time_Value(1))
            {
                m_pCommTaskMan->GetCommLayer()->Timer1Sec();
                Timer = ace_now;
                time_out = ACE_Time_Value(1);
            }
            else
            {
                time_out = ACE_Time_Value(1) - time_out;
            }
        }
        else
        {
            time_out = ACE_Time_Value(1);
        }

        //���뻥������ʹ��Ȩ
        CCommMutex::Instance()->Acquire();

        DelScheduleSocket();

        RegScheduleSocket();

        SleepSocketOutput();

        //�ͷŻ�������ʹ��Ȩ
        CCommMutex::Instance()->Release();
    }
    return 0;
}

/******************************************************
// ������:  CCommTask::RegisterHandle
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��CSocketImpl����Ķ���ע�ᵽ�������ACE_Reactor
//          ������
// ����:
//       ����1 :CSocketImpl* pSocket
//       ����2 :ACE_Reactor_Mask mask
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CCommTask::RegisterHandle(CSocketImpl* pSocket,
                              ACE_Reactor_Mask mask)
{
    int nRegResult = m_Reactor.register_handler(pSocket, mask);
    if (nRegResult != 0)
    {
        nRegResult = (ACE_OS::last_error() == EMFILE ) ?
            ERR_REACTOR_FULL : ERR_HANDLE_INVALID;
    }
    else
    {
        pSocket->m_pCommTask = this;
    }
    return nRegResult;
}


/******************************************************
// ������: 	CCommTask::ScheduleRegister
// ����:    Wangfeng
// ʱ��:    01-12-31
// ����:    ��pSocketָ�������ע���б�
// ����:
//       ����1 :CSocketImpl* pSocket
//       ����2 :ACE_Reactor_Mask mask
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-12-31      Wangfeng
******************************************************/
int CCommTask::ScheduleRegister(CSocketImpl* pSocket, 
                                ACE_Reactor_Mask mask)
{
    if(TRUE == m_bReactorFull)
    {
        return ERR_REACTOR_FULL;
    }

    m_SocketWillReg[pSocket] = mask;
    return 0;
}


/******************************************************
// ������: 	CCommTask::ScheduleRemove
// ����:    Wangfeng
// ʱ��:    01-11-22
// ����:    ��pSocketָ�������ɾ���б�
// ����:
//       ����1 :CSocketImpl* pSocket
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-11-22      Wangfeng
******************************************************/
void CCommTask::ScheduleRemove(CSocketImpl* pSocket)
{
    LIST_SOCKET::iterator i;

    for(i = m_SocketWillBeDel.begin(); 
        i != m_SocketWillBeDel.end();
        i++)
    {
        if(*i == pSocket) 
        {
            return;
        }
    }
    m_SocketWillBeDel.push_back(pSocket);
}

/******************************************************
// ������: 	CCommTask::ScheduleWakeup
// ����:    Wangfeng
// ʱ��:    01-12-31
// ����:    ��pSocketָ�������ע��output�¼����б�
// ����:
//       ����1 :CSocketImpl* pSocket
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-12-31      Wangfeng
******************************************************/
void CCommTask::ScheduleWakeup(CSocketImpl* pSocket)
{
    LIST_SOCKET::iterator i;

    for(i = m_SocketWillRmOptHdl.begin(); 
        i != m_SocketWillRmOptHdl.end();
        i++)
    {
        if(*i == pSocket) 
        {
            return;
        }
    }
    m_SocketWillRmOptHdl.push_back(pSocket);
}


/******************************************************
// ������: 	CCommTask::DelScheduleSocket
// ����:    Wangfeng
// ʱ��:    01-11-22
// ����:    ɾ����ɾ���б��е�Socket����
// ����:
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-11-22      Wangfeng
******************************************************/
void CCommTask::DelScheduleSocket()
{
    //ͨ���ص����������г�Ա��handle_close������ɾ��
    //�ϲ���ʽע����Socket��������������UNIX����ͬ��
    //�ص�handle_close��������ɵ�������

    while(m_SocketWillBeDel.size())
    {
        CSocketImpl* pSocket = m_SocketWillBeDel.back();

        //��ACE_Reactor��ע����������¼���������Ҫ�ص�
        //handle_close()����
        m_Reactor.remove_handler(pSocket,
                    ACE_Event_Handler::ALL_EVENTS_MASK | 
                    DONT_CALL);
    
        m_SocketWillBeDel.pop_back();
        m_SocketWillReg.erase(pSocket);
        m_SocketWillRmOptHdl.remove(pSocket);
        pSocket->SOCK_handle_close();
        delete pSocket;
    }
}


/******************************************************
// ������: 	CCommTask::RegScheduleSocket
// ����:    Wangfeng
// ʱ��:    01-12-31
// ����:    ע���ע���б��е�Socket����
// ����:
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-12-31      Wangfeng
******************************************************/
void CCommTask::RegScheduleSocket()
{
    MAP_SOCKET_TO_MASK::iterator i;
    int nRet;

    for(i =  m_SocketWillReg.begin();
        i != m_SocketWillReg.end();
        i++)
    {
        CSocketImpl* pSocket = (*i).first;
        nRet = RegisterHandle(pSocket, (*i).second);
        switch (nRet)
        {
        case ERR_SUCCESS:
            break;

        case ERR_REACTOR_FULL:
            m_bReactorFull = TRUE;
            m_pCommTaskMan->RegisterHandle(pSocket, (*i).second);
            break;

        default:
            m_SocketWillBeDel.push_back(pSocket);
            break;
        }
    }
    m_SocketWillReg.clear();
}


/******************************************************
// ������: 	CCommTask::SleepSocketOutput
// ����:    Wangfeng
// ʱ��:    01-12-31
// ����:    ��ͣm_SocketWillRmOptHdl��Socket�����output�¼�
// ����:
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-12-31      Wangfeng
******************************************************/
void CCommTask::SleepSocketOutput()
{
    while(m_SocketWillRmOptHdl.size())
    {
        CSocketImpl* pSocket = m_SocketWillRmOptHdl.back();
        m_SocketWillRmOptHdl.pop_back();
        m_Reactor.schedule_wakeup(pSocket, ACE_Event_Handler::WRITE_MASK);
    }
}


/******************************************************
// ������:  CCommTask::QuitComTask
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    �����߳�
// ����:
//       ����1 :void
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CCommTask::QuitComTask(void)
{
    m_Reactor.end_reactor_event_loop();
    //����ACE_Task_Base�ṩ��wait�����ȴ��߳̽���
    wait();
}


/******************************************************
// ������: 	CCommTaskMan::CCommTaskMan
// ����:    Wangfeng
// ʱ��:    01-10-20
// ����:    ���캯��
// ����:    CCommLayer* pCommLayer
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
CCommTaskMan::CCommTaskMan(CCommLayer* pCommLayer)
{
    ASSERT(0 != pCommLayer);

    m_pCommLayer = pCommLayer;
}


/******************************************************
// ������:  CCommTaskMan::~CCommTaskMan
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
CCommTaskMan::~CCommTaskMan()
{
    //ɾ��m_CommTaskList�����е�CCommTask����
    while(m_CommTaskList.size() > 0)
    {
        CCommTask* pTask = m_CommTaskList.front();
        m_CommTaskList.pop_front();
        delete pTask;
    }

    m_pCommLayer = 0;
}


/******************************************************
// ������:  CCommTaskMan::Init
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ��ʼ������
// ����:
//       ����1 :void
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CCommTaskMan::Init(void)
{
    if(m_CommTaskList.empty())
    {
        CCommTask* pTask = new CCommTask(this);
        m_CommTaskList.push_back(pTask);
    }
    return ERR_SUCCESS;
}


/******************************************************
// ������:  CCommTaskMan::RegisterHandle
// ����:    Wangfeng
// ʱ��:    01-10-6
// ����:    ע��CSocketImpl����Ķ���
// ����:
//       ����1 :CSocketImpl* pSocket
//       ����2 :ACE_Reactor_Mask mask
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CCommTaskMan::RegisterHandle(CSocketImpl* pSocket,
                                 ACE_Reactor_Mask mask)
{
    //���m_CommTaskListΪ�գ��򴴽�һ��CCommTask����
    //�����ö���ָ����뵽m_CommTaskList��
    if(m_CommTaskList.empty())
    {
        CCommTask* pTask = new CCommTask(this);
        m_CommTaskList.push_back(pTask);
    }

    int nRegResult;
    LIST_PCOMMTASK::iterator i;
    for (i =  m_CommTaskList.begin();
         i != m_CommTaskList.end(); i++)
    {
        if(i == m_CommTaskList.begin())
        {
            nRegResult = (*i)->RegisterHandle(pSocket, mask);
        }
        else
        {
            nRegResult = (*i)->ScheduleRegister(pSocket, mask);
        }

        //�����Reactorע�����������������һ�������򷵻�
        if (ERR_REACTOR_FULL != nRegResult)
        {
            return nRegResult;
        }
    }

    //�ѱ��������е�CCommTask����Ҳ����ע��ɹ���
    CCommTask* pTask = new CCommTask(this);
    if(pTask->open() != 0)
    {
        delete pTask;
        return ERR_COMM_TASK_OPEN;
    }
    m_CommTaskList.push_back(pTask);

    pTask = m_CommTaskList.back();
    nRegResult = pTask->ScheduleRegister(pSocket, mask);
    return nRegResult;
}


/******************************************************
// ������: 	CCommTaskMan::Active
// ����:    Wangfeng
// ʱ��:    01-11-14
// ����:    ��������CCommTask����
// ����:
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-11-14      Wangfeng
******************************************************/
int CCommTaskMan::Active()
{
    LIST_PCOMMTASK::iterator i;
    for (i = m_CommTaskList.begin();
         i != m_CommTaskList.end(); i++)
    {
        if((*i)->open() != 0)
        {
            return ERR_COMM_TASK_OPEN;
        }
    }
    return ERR_SUCCESS;
}


/******************************************************
// ������: 	CCommTaskMan::DeActive
// ����:    Wangfeng
// ʱ��:    01-11-14
// ����:    ȥ��������CCommTask����
// ����:
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-11-14      Wangfeng
******************************************************/
void CCommTaskMan::DeActive()
{
    LIST_PCOMMTASK::iterator i;
    for (i = m_CommTaskList.begin();
         i != m_CommTaskList.end(); i++)
    {
        (*i)->QuitComTask();
    }
}


/******************************************************
// ������: 	CCommTaskMan::GetCommLayer
// ����:    Wangfeng
// ʱ��:    01-12-31
// ����:    ȡCCommLayer�����ָ��
// ����:
// ����ֵ: CCommLayer* 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-12-31      Wangfeng
******************************************************/
CCommLayer* CCommTaskMan::GetCommLayer()
{
    return m_pCommLayer;
}


/******************************************************
// ������:  CCommLayer::CCommLayer
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ���캯��
// ����:
//       ����1 :PROCESS_TYPE const nProcessID
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CCommLayer::CCommLayer(PROCESS_TYPE const nProcessID)
{
    m_nProcessID = nProcessID;
    m_pCommTaskMan = 0;
    m_pCommRouterMan = 0;
}


/******************************************************
// ������:  CCommLayer::~CCommLayer
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
CCommLayer::~CCommLayer()
{
    ExitInstance();
}


/******************************************************
// ������:  CCommLayer::Init
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ��ʼ������
// ����:
//       ����1 :void
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
int CCommLayer::Init(void)
{
    m_pCommRouterMan = new CCommRouterMan(this);

    m_pCommTaskMan = new CCommTaskMan(this);

	//������ 2002-12-26
	(ACE_Singleton<CIPFilter,ACE_Null_Mutex>::instance())->Init();
	//������
    //����m_pCommTaskMan�����Init�����������ظú����ķ���ֵ
    return m_pCommTaskMan->Init();
}


/******************************************************
// ������:  CCommLayer::ExitInstance
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ��ɱ�Ҫ��������
// ����:
//       ����1 :void
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
void CCommLayer::ExitInstance(void)
{
    //ȥ����CommTask
    if(m_pCommTaskMan)
    {
        m_pCommTaskMan->DeActive();
    }

    //�ͷ�m_pCommRouterManָ��Ķ���
    if(m_pCommRouterMan)
    {
        delete m_pCommRouterMan;
        m_pCommRouterMan = 0;
    }
    
    //�ͷ�m_pCommTaskManָ��Ķ���
    if(m_pCommTaskMan)
    {
        delete m_pCommTaskMan;
        m_pCommTaskMan = 0;
    }
}


/******************************************************
// ������:  CCommLayer::Instance
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ���ݽ��̵ı�Ż����Ӧ��ͨ�Ų����
// ����:
//       ����1 :unsigned long const nProcessID
// ����ֵ: CCommLayer* const
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CCommLayer* const CCommLayer::Instance(unsigned long const nProcessID)
{
    if (m_pCommLayer != 0)
    //����Ѵ��ڵ�ʵ������
    {
        return m_pCommLayer;
    }

    //��������ڵ�ʵ����������ݽ��̱�Ŵ����õ�ʵ������
    switch (nProcessID)
    {
    case PT_KERNEL:     //KERNEL����
        m_pCommLayer = new CMsgSvrCommLayer((PROCESS_TYPE)nProcessID);
        break;

    case PT_OM:         //����ά������
        m_pCommLayer = new CMmlCommLayer((PROCESS_TYPE)nProcessID);
        break;

    case PT_CLSTR:      //˫������
        m_pCommLayer = new CClstrCommLayer((PROCESS_TYPE)nProcessID);
        break;
    case PT_METERACC:   //�ƴα��ۼӽ���
        m_pCommLayer = new CMeterAccCommLayer((PROCESS_TYPE)nProcessID);
        break;
    case PT_CFGSVR:     //Զ�����ý���
        m_pCommLayer = new CCfgSvrCommLayer((PROCESS_TYPE)nProcessID);
        break;
    
        //GTP'�Ʒѽӿ����� Z30971 2004/06
    case PT_BS:         //�Ʒѽӿڽ���
        m_pCommLayer = new CBSCommlayer((PROCESS_TYPE)nProcessID);
        break;

    default:
        if(nProcessID >= PT_CO_BASE && nProcessID <= MAX_PT_CO_TYPE)
        {
            m_pCommLayer = new CClstrCommLayer((PROCESS_TYPE)nProcessID);
        }
        else if ((nProcessID >= PT_AP_BASE) && (nProcessID < PT_CO_BASE))//��������
        {
            //��ȡ���ã������ΪnProcessID���̵�ͨ�Ų�֧�ֻ�������Э�飬
            //�򴴽�һ��CSlideWndCommLayer���󣬽��ö����ָ�븳ֵ��
            //m_pCommLayer�������ΪnProcessID���̵�ͨ�Ų�֧��GTP��Э
            //�飬�򴴽�һ��CGtpCommLayer���󣬽��ö����ָ�븳ֵ��
            //m_pCommLayer

            //�������ļ��ж�ȡ���������
            CINIFile *pIniFile;
            pIniFile = new CINIFile(GetCfgFilePath());
            if(pIniFile->Open() == FALSE)
            {
                delete pIniFile;
                return 0;
            }
            char strSec[20];
            sprintf(strSec, "%s%d", CFG_SEC_AP,
                    nProcessID - PT_AP_BASE + 1);
            INT4 nAPType = pIniFile->GetInt(strSec,
                                            CFG_AP_KEY_APTYPE,
                                            -1);
            pIniFile->Close();
            delete pIniFile;

            //���ݽ�������ͺŴ������CCommLayer����
            switch(nAPType)
            {
            case AP_PS:
            case AP_PS_RESERVED:
            case AP_WLAN:
                m_pCommLayer = new CGtpCommLayer(
                                    (PROCESS_TYPE)nProcessID);
                break;

			//����Զ2002.04.10���
            case AP_GSM:
            case AP_128:
            case AP_STP:
                m_pCommLayer = new CSlideWndCommLayer(
                                    (PROCESS_TYPE)nProcessID);
                break;

			case AP_SOFTX:
                m_pCommLayer = new CSoftXCommLayer(
                                    (PROCESS_TYPE)nProcessID);
                break;

            default:
                TRACE(MTS_COMM, S_COMM_APTYPE_UNRECOGNIZED, nAPType);
				m_pCommLayer = NULL;
                return 0;
 			//����Զ2002.04.10��ӽ���
            }
        }
        else
        {
            return 0;
        }
    }

    //���뻥������ʹ��Ȩ
    CCommMutex::Instance()->Acquire();

    //��ʼ��ͨ�Ų�
    int nRet = m_pCommLayer->Init();

    //�ͷŻ�������ʹ��Ȩ
    CCommMutex::Instance()->Release();

    if((nRet != ERR_SUCCESS) || 
       ((nRet = m_pCommLayer->m_pCommTaskMan->Active()) != ERR_SUCCESS))
    {
        TRACE(MTS_COMM, S_COMM_INSTANCE_COMMLAYER_FAIL, nRet);
        Destroy();
    }
    else
    {
        TRACE(MTS_COMM, S_COMM_INSTANCE_COMMLAYER);
    }
    return m_pCommLayer;
}


/******************************************************
// ������:  CCommLayer::Destroy
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ���ٶ���
// ����:
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
void CCommLayer::Destroy()
{
    if (m_pCommLayer != 0)
    {
        //�ͷ�m_pCommLayer->m_pCommTaskManָ��Ķ���
        delete m_pCommLayer;
        m_pCommLayer = NULL;
    }

    //��������㻥������ʵ������
    CCommMutex::Destroy();

    TRACE(MTS_COMM, S_COMM_UNINSTANCE_COMMLAYER);
}



/******************************************************
// ������:  CCommLayer::AddFrmSocket
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ΪӦ������ΪuAppType�Ķ˿�����һ��������
// ����:
//       ����1 :CFrmSocket* pFrmSocket
//       ����2 :unsigned long uAppType
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
int CCommLayer::AddFrmSocket(CFrmSocket* pFrmSocket,
                             unsigned long uAppType)
{
    //����*m_pCommTaskMan�����RegisterHandle����
    int nRet = RegisterHandle(pFrmSocket,
                            ACE_Event_Handler::READ_MASK |
                            ACE_Event_Handler::WRITE_MASK |
                            ACE_Event_Handler::ACCEPT_MASK);
    if(nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //����*m_pCommRouterMan�����AddFrmSocket������������
    //�ú����ķ���ֵ
    return m_pCommRouterMan->AddFrmSocket(pFrmSocket, uAppType);
}


/******************************************************
// ������:  CCommLayer::RegisterHandle
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ע��CSocketImpl����Ķ���
// ����:
//       ����1 :CSocketImpl* pSocket
//       ����2 :ACE_Reactor_Mask mask
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
int CCommLayer::RegisterHandle(CSocketImpl* pSocket,
                               ACE_Reactor_Mask mask)
{
    //����*m_pCommTaskMan�����RegisterHandle������������
    //�ú����ķ���ֵ
    return m_pCommTaskMan->RegisterHandle(pSocket, mask);
}


/******************************************************
// ������:  CCommLayer::Send
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ������Ϣ
// ����:
//       ����1 :const MSG_INTRA* const pMsg
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
int CCommLayer::Send(const MSG_INTRA* const pMsg)
{
    int nRet;
    //���뻥������ʹ��Ȩ
    CCommMutex::Instance()->Acquire();

    //����*m_pCommRouterMan�����Send�����������ظú����ķ���ֵ
    nRet = m_pCommRouterMan->Send(pMsg);

    //�ͷŻ�������ʹ��Ȩ
    CCommMutex::Instance()->Release();

    return nRet;
}


/******************************************************
// ������:  CCommLayer::OnReceive
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    �ύ�ӵͲ����·�Ͻ�����������Ϣ
// ����:
//       ����1 :MSG_INTRA* pMsg
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
void CCommLayer::OnReceive(MSG_INTRA* pMsg)
{
    //�򱾽��̵����ģ���ύ��Ϣ֡������Ϣ֡�ɿ�ܸ����ͷ�
    CMsgProcessor::SendMsg(pMsg);
}


/******************************************************
// ������:  CCommLayer::Timer1Sec
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
void CCommLayer::Timer1Sec(void)
{
    //���뻥������ʹ��Ȩ
    CCommMutex::Instance()->Acquire();

    //����*m_pCommTaskMan�����Timer1Sec����
    //m_pCommTaskMan->Timer1Sec();

    //����*m_pCommRouterMan�����Timer1Sec����
    m_pCommRouterMan->Timer1Sec();

    //�ͷŻ�������ʹ��Ȩ
    CCommMutex::Instance()->Release();

}


/******************************************************
// ������:  CGtpCommLayer::CGtpCommLayer
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ���캯��
// ����:
//       ����1 :PROCESS_TYPE const nProcessID
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CGtpCommLayer::CGtpCommLayer(PROCESS_TYPE const nProcessID)
                             :CTcpSvrCommLayer(nProcessID)
{
    //
}


/******************************************************
// ������: 	CGtpCommLayer::~CGtpCommLayer
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
CGtpCommLayer::~CGtpCommLayer()
{
    //
}


/******************************************************
// ������:  CGtpCommLayer::Init
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ��ʼ������
// ����:
//       ����1 :void
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
int CGtpCommLayer::Init(void)
{
    int nRet = CCommLayer::Init();
    if (nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //�����ͳ�ʼ��CMsgCliCommRouter���󣬲�����ʼ���ɹ���
    //CMsgCliCommRouter�������m_pCommRouterMan
    CMsgCliCommRouter* pMsgCliCommRouter;
    pMsgCliCommRouter = new CMsgCliCommRouter(m_pCommRouterMan,
                                                APP_MSG,
                                                m_nProcessID);
    nRet = m_pCommRouterMan->RegisterCommRouter(pMsgCliCommRouter,
                                                APP_MSG);
    if (nRet != ERR_SUCCESS)
    {
        delete pMsgCliCommRouter;
        return nRet;
    }

    //�����ͳ�ʼ��CGtpUdpCommRouter���󣬲�����ʼ���ɹ���
    //CGtpUdpCommRouter�������m_pCommRouterMan�����س�ʼ�����
    CGtpUdpCommRouter* pGtpCommRouter;
    pGtpCommRouter = new CGtpUdpCommRouter(m_pCommRouterMan, APP_GTP_UDP);
    //��ʼ��
    nRet = pGtpCommRouter->Init(m_nProcessID - PT_AP_BASE + 1);
    if (nRet != ERR_SUCCESS)
    {
        delete pGtpCommRouter;
        return nRet;
    }

    //��RouterManע��
    nRet = m_pCommRouterMan->RegisterCommRouter(pGtpCommRouter,
                                                APP_GTP_UDP);
    if (nRet != ERR_SUCCESS)
    {
        delete pGtpCommRouter;
        return nRet;
    }

    //�����ͳ�ʼ��CGtpFrmCommRouter���󣬲�����ʼ���ɹ���
    //CGtpFrmCommRouter�������m_pCommRouterMan�����س�ʼ�����
    CGtpFrmCommRouter* pGtpFrmCommRouter = new CGtpFrmCommRouter(m_pCommRouterMan, APP_GTP_TCP);
    //��ʼ��
    nRet = pGtpFrmCommRouter->Init(m_nProcessID - PT_AP_BASE + 1);
    if (nRet != ERR_SUCCESS)
    {
        delete pGtpFrmCommRouter;
        return nRet;
    }

    //��RouterManע��
    nRet = m_pCommRouterMan->RegisterCommRouter(pGtpFrmCommRouter,
                                                APP_GTP_TCP);
    if (nRet != ERR_SUCCESS)
    {
        delete pGtpFrmCommRouter;
        return nRet;
    }
    
    //��������CCommListener���󣬴������ļ��ж�ȡ�����˿ڵ�
    //IP��ַ�Ͷ˿ںţ��ֱ��������CCommListener�����Open������
    //����CCommLayer::RegisterHandle����������CCommListener��
    //��ע�ᵽ*m_pCommTaskMan�����У�������CCommListener�����
    //�뵽m_CommListenerList��

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
    sprintf(strSec, "%s%d", CFG_SEC_AP, m_nProcessID - PT_AP_BASE + 1);
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
    BOOL bReuseAddr = pIniFile->GetInt(CFG_SEC_COMMON,
                                       CFG_REUSE_ADDRESS,
                                       CFG_REUSE_ADDRESS_DEFAULT);
    bReuseAddr = pIniFile->GetInt(strSec,
                                  CFG_REUSE_ADDRESS,
                                  bReuseAddr);
    pIniFile->Close();
    delete pIniFile;

    if(nUdpTcpMask & CFG_AP_KEY_TCP_ENABLE)
    {
        CCommListener* pCommListener = new CCommListener(APP_GTP_TCP, this);
        nRet = pCommListener->Open(szPMLocalIp, nPMLocalPort, bReuseAddr);
        if (nRet != ERR_SUCCESS)
        {
            TRACE(MTS_COMM, S_COMM_OPEN_LISTEN_PORT_ERR, szPMLocalIp, 
                  nPMLocalPort, ACE_OS::last_error(), 
                  ACE_OS::strerror(ACE_OS::last_error()));
            delete pCommListener;
            return nRet;
        }

        //ע��Socket�¼�
        nRet = RegisterHandle(pCommListener,
                              ACE_Event_Handler::READ_MASK |
                              ACE_Event_Handler::WRITE_MASK |
                              ACE_Event_Handler::ACCEPT_MASK);
        if(nRet != ERR_SUCCESS)
        {
            delete pCommListener;
            return nRet;
        }
        m_CommListenerList.push_back(pCommListener);

        if(szFMLocalIp[0] != '\0' && nFMLocalPort != 0)
        {
            pCommListener = new CCommListener(APP_GTP_TCP, this);
            nRet = pCommListener->Open(szFMLocalIp, nFMLocalPort, bReuseAddr);
            if (nRet != ERR_SUCCESS)
            {
                TRACE(MTS_COMM, S_COMM_OPEN_LISTEN_PORT_ERR, szFMLocalIp, 
                      nFMLocalPort, ACE_OS::last_error(), 
                      ACE_OS::strerror(ACE_OS::last_error()));
                delete pCommListener;
                return nRet;
            }
            //ע��Socket�¼�
            nRet = RegisterHandle(pCommListener,
                                ACE_Event_Handler::READ_MASK |
                                ACE_Event_Handler::WRITE_MASK |
                                ACE_Event_Handler::ACCEPT_MASK);
            if(nRet != ERR_SUCCESS)
            {
                delete pCommListener;
                return nRet;
            }
            m_CommListenerList.push_back(pCommListener);
        }
    }
    
    return nRet;
}


/******************************************************
// ������:  CTcpSvrCommLayer::CTcpSvrCommLayer
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ���캯��
// ����:
//       ����1 :PROCESS_TYPE const nProcessID
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CTcpSvrCommLayer::CTcpSvrCommLayer(PROCESS_TYPE const nProcessID)
                                   :CCommLayer(nProcessID)
{
    //
}


/******************************************************
// ������:  CTcpSvrCommLayer::~CTcpSvrCommLayer
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ��������
// ����:
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CTcpSvrCommLayer::~CTcpSvrCommLayer()
{
    //����ɾ��m_CommListenerList�е�CCommListener����
    //�ö�����������������ACE_Reactor����ʱ���Զ�ɾ��
    //����ֻ��Ҫ���LIST���е�Ԫ�ؼ���
    m_CommListenerList.clear();
}


/******************************************************
// ������:  CSlideWndCommLayer::CSlideWndCommLayer
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ���캯��
// ����:
//       ����1 :PROCESS_TYPE const nProcessID
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CSlideWndCommLayer::CSlideWndCommLayer(PROCESS_TYPE const nProcessID)
                                       :CTcpSvrCommLayer(nProcessID)
{
    //
}



/******************************************************
// ������: 	CSlideWndCommLayer::~CSlideWndCommLayer
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
CSlideWndCommLayer::~CSlideWndCommLayer()
{
    //
}


/******************************************************
// ������:  CSlideWndCommLayer::Init
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ��ʼ������
// ����:
//       ����1 :void
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
int CSlideWndCommLayer::Init(void)
{
    int nRet = CCommLayer::Init();
    if (nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //�����ͳ�ʼ��CMsgCliCommRouter���󣬲�����ʼ���ɹ���
    //CMsgCliCommRouter�������m_pCommRouterMan
    CMsgCliCommRouter* pMsgCliCommRouter;
    pMsgCliCommRouter = new CMsgCliCommRouter(m_pCommRouterMan,
                                                APP_MSG,
                                                m_nProcessID);
    nRet = m_pCommRouterMan->RegisterCommRouter(pMsgCliCommRouter,
                                                APP_MSG);
    if (nRet != ERR_SUCCESS)
    {
        delete pMsgCliCommRouter;
        return nRet;
    }

    //�����ͳ�ʼ��CSlideWndCommRouter���󣬲�����ʼ���ɹ���
    //CSlideWndCommRouter�������m_pCommRouterMan
    CSlideWndCommRouter* pSlideWndCommRouter;
    pSlideWndCommRouter = new CSlideWndCommRouter(m_pCommRouterMan,
                                                  APP_MAIN);
    nRet = m_pCommRouterMan->RegisterCommRouter(pSlideWndCommRouter,
                                                APP_MAIN);
    if (nRet != ERR_SUCCESS)
    {
        delete pSlideWndCommRouter;
        return nRet;
    }

    //��������CCommListener���󣬴������ļ��ж�ȡ�����˿ڵ�
    //IP��ַ�Ͷ˿ںţ��ֱ��������CCommListener�����Open������
    //����CCommLayer::RegisterHandle����������CCommListener��
    //��ע�ᵽ*m_pCommTaskMan�����У�������CCommListener�����
    //�뵽m_CommListenerList��
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    pIniFile->Open();

    char szLocalIpToPM[20];
    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP, m_nProcessID - PT_AP_BASE + 1);
    pIniFile->GetString(strSec, CFG_AP_KEY_LOCAL_IP_TO_PM,
                        CFG_AP_KEY_LOCAL_IP_TO_PM_DEFAULT,
                        szLocalIpToPM, 20);

    char szLocalIpToFM[20];
    pIniFile->GetString(strSec, CFG_AP_KEY_LOCAL_IP_TO_FM,
                        CFG_AP_KEY_LOCAL_IP_TO_FM_DEFAULT,
                        szLocalIpToFM, 20);

    int nLocalPortToPM = pIniFile->GetInt(strSec,
                                        CFG_AP_KEY_LOCAL_PORT_TO_PM,
                                        CFG_AP_KEY_LOCAL_PORT_TO_PM_DEFAULT);
    int nLocalPortToFM = pIniFile->GetInt(strSec,
                                        CFG_AP_KEY_LOCAL_PORT_TO_FM,
                                        CFG_AP_KEY_LOCAL_PORT_TO_FM_DEFAULT);
    BOOL bReuseAddr = pIniFile->GetInt(CFG_SEC_COMMON,
                                       CFG_REUSE_ADDRESS,
                                       CFG_REUSE_ADDRESS_DEFAULT);
    bReuseAddr = pIniFile->GetInt(strSec,
                                  CFG_REUSE_ADDRESS,
                                  bReuseAddr);
	//����Զ2002.04.10���
	int nLinkTimeOut = pIniFile->GetInt(strSec, CFG_AP_KEY_LINK_TIMEOUT, 
										CFG_AP_KEY_LINK_TIMEOUT_DEFAULT);
	pSlideWndCommRouter->SetLinkTimeOut(nLinkTimeOut);
	//����Զ2002.04.10��ӽ���
    pIniFile->Close();
    delete pIniFile;

    CCommListener* pCommListener = new CCommListener(APP_MSC_PRIMARY, this);
    nRet = pCommListener->Open(szLocalIpToPM, nLocalPortToPM, bReuseAddr);
    if (nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_OPEN_LISTEN_PORT_ERR, szLocalIpToPM, 
              nLocalPortToPM, ACE_OS::last_error(), 
              ACE_OS::strerror(ACE_OS::last_error()));
        delete pCommListener;
        return nRet;
    }

    //ע��Socket�¼�
    nRet = RegisterHandle(pCommListener,
                        ACE_Event_Handler::READ_MASK |
                        ACE_Event_Handler::WRITE_MASK |
                        ACE_Event_Handler::ACCEPT_MASK);
    if(nRet != ERR_SUCCESS)
    {
        delete pCommListener;
        return nRet;
    }
    m_CommListenerList.push_back(pCommListener);

    pCommListener = new CCommListener(APP_MSC_SECOND, this);
    nRet = pCommListener->Open(szLocalIpToFM, nLocalPortToFM, bReuseAddr);
    if (nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_OPEN_LISTEN_PORT_ERR, szLocalIpToFM, 
              nLocalPortToFM, ACE_OS::last_error(), 
              ACE_OS::strerror(ACE_OS::last_error()));
        delete pCommListener;
        return nRet;
    }
    //ע��Socket�¼�
    nRet = RegisterHandle(pCommListener,
                        ACE_Event_Handler::READ_MASK |
                        ACE_Event_Handler::WRITE_MASK |
                        ACE_Event_Handler::ACCEPT_MASK);
    if(nRet != ERR_SUCCESS)
    {
        delete pCommListener;
        return nRet;
    }
    m_CommListenerList.push_back(pCommListener);
    return nRet;
}


//����Զ2002.04.08���
//���캯��
CSoftXCommLayer::CSoftXCommLayer(PROCESS_TYPE const nProcessID)
				: CTcpSvrCommLayer(nProcessID)
{
}

//��������
CSoftXCommLayer::~CSoftXCommLayer()
{
}

/******************************************************
// ����:    ��ʼ������
// ����:
//       ����1 :void
// ����ֵ: int
// ע��:
******************************************************/
int CSoftXCommLayer::Init(void)
{
    int nRet = CCommLayer::Init();
    if (nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //�����ͳ�ʼ��CMsgCliCommRouter���󣬲�����ʼ���ɹ���
    //CMsgCliCommRouter�������m_pCommRouterMan
    CMsgCliCommRouter* pMsgCliCommRouter;
    pMsgCliCommRouter = new CMsgCliCommRouter(m_pCommRouterMan,
                                                APP_MSG,
                                                m_nProcessID);
    nRet = m_pCommRouterMan->RegisterCommRouter(pMsgCliCommRouter,
                                                APP_MSG);
    if (nRet != ERR_SUCCESS)
    {
        delete pMsgCliCommRouter;
        return nRet;
    }

    //�����ͳ�ʼ��CSoftXCommRouter���󣬲�����ʼ���ɹ���
    //CSoftXCommRouter�������m_pCommRouterMan
    CSoftXCommRouter* pSoftXCommRouter;
    pSoftXCommRouter = new CSoftXCommRouter(m_pCommRouterMan, APP_MAIN);
    nRet = m_pCommRouterMan->RegisterCommRouter(pSoftXCommRouter,
                                                APP_MAIN);
    if (nRet != ERR_SUCCESS)
    {
        delete pSoftXCommRouter;
        return nRet;
    }

    //��������CCommListener���󣬴������ļ��ж�ȡ�����˿ڵ�
    //IP��ַ�Ͷ˿ںţ��ֱ��������CCommListener�����Open������
    //����CCommLayer::RegisterHandle����������CCommListener��
    //��ע�ᵽ*m_pCommTaskMan�����У�������CCommListener�����
    //�뵽m_CommListenerList��
    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();

    char szLocalIpToPM[20];
    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP, m_nProcessID - PT_AP_BASE + 1);
    IniFile.GetString(strSec, CFG_AP_KEY_LOCAL_IP_TO_PM,
                      CFG_AP_KEY_LOCAL_IP_TO_PM_DEFAULT,
                      szLocalIpToPM, 20);

    char szLocalIpToFM[20];
    IniFile.GetString(strSec, CFG_AP_KEY_LOCAL_IP_TO_FM,
                      CFG_AP_KEY_LOCAL_IP_TO_FM_DEFAULT,
                      szLocalIpToFM, 20);
    int nLocalPortToPM = IniFile.GetInt(strSec,
                                        CFG_AP_KEY_LOCAL_PORT_TO_PM,
                                        CFG_AP_KEY_LOCAL_PORT_TO_PM_DEFAULT);
    int nLocalPortToFM = IniFile.GetInt(strSec,
                                        CFG_AP_KEY_LOCAL_PORT_TO_FM,
                                        CFG_AP_KEY_LOCAL_PORT_TO_FM_DEFAULT);
    BOOL bReuseAddr = IniFile.GetInt(CFG_SEC_COMMON,
                                     CFG_REUSE_ADDRESS,
                                     CFG_REUSE_ADDRESS_DEFAULT);
    bReuseAddr = IniFile.GetInt(strSec,
                                CFG_REUSE_ADDRESS,
                                bReuseAddr);
	int nLinkTimeOut = IniFile.GetInt(strSec, CFG_AP_KEY_LINK_TIMEOUT, 
									  CFG_AP_KEY_LINK_TIMEOUT_DEFAULT);
	pSoftXCommRouter->SetLinkTimeOut(nLinkTimeOut);
    IniFile.Close();

    CCommListener* pCommListener = new CCommListener(APP_SOFTX_PRIMARY, this);
    nRet = pCommListener->Open(szLocalIpToPM, nLocalPortToPM, bReuseAddr);
    if (nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_OPEN_LISTEN_PORT_ERR, szLocalIpToPM, 
              nLocalPortToPM, ACE_OS::last_error(), 
              ACE_OS::strerror(ACE_OS::last_error()));
        delete pCommListener;
        return nRet;
    }

    //ע��Socket�¼�
    nRet = RegisterHandle(pCommListener,
                        ACE_Event_Handler::READ_MASK |
                        ACE_Event_Handler::WRITE_MASK |
                        ACE_Event_Handler::ACCEPT_MASK);
    if(nRet != ERR_SUCCESS)
    {
        delete pCommListener;
        return nRet;
    }
    m_CommListenerList.push_back(pCommListener);

    pCommListener = new CCommListener(APP_SOFTX_SECOND, this);
    nRet = pCommListener->Open(szLocalIpToFM, nLocalPortToFM, bReuseAddr);
    if (nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_OPEN_LISTEN_PORT_ERR, szLocalIpToFM, 
              nLocalPortToFM, ACE_OS::last_error(), 
              ACE_OS::strerror(ACE_OS::last_error()));
        delete pCommListener;
        return nRet;
    }
    //ע��Socket�¼�
    nRet = RegisterHandle(pCommListener,
                        ACE_Event_Handler::READ_MASK |
                        ACE_Event_Handler::WRITE_MASK |
                        ACE_Event_Handler::ACCEPT_MASK);
    if(nRet != ERR_SUCCESS)
    {
        delete pCommListener;
        return nRet;
    }
    m_CommListenerList.push_back(pCommListener);
    return nRet;
}

//����Զ2002.04.08��ӽ���



/******************************************************
// ������:  CMsgSvrCommLayer::CMsgSvrCommLayer
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ���캯��
// ����:
//       ����1 :PROCESS_TYPE const nProcessID
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CMsgSvrCommLayer::CMsgSvrCommLayer(PROCESS_TYPE const nProcessID)
                                   :CTcpSvrCommLayer(nProcessID)
{
    //
}


/******************************************************
// ������: 	CMsgSvrCommLayer::~CMsgSvrCommLayer
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
CMsgSvrCommLayer::~CMsgSvrCommLayer()
{
    //
}


/******************************************************
// ������:  CMsgSvrCommLayer::Init
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ��ʼ������
// ����:
//       ����1 :void
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
int CMsgSvrCommLayer::Init(void)
{
    int nRet = CCommLayer::Init();
    if (nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //�������ļ��ж�ȡMsgSvr�Ķ˿ںţ�ȱʡֵΪ9999
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    pIniFile->Open();
    int nMsgSvrPort = pIniFile->GetInt(CFG_SEC_COMMON,
                                       CFG_COMMON_KEY_MSGSVR_PORT,
                                       CFG_COMMON_KEY_MSGSVR_PORT_DEFAULT);
    BOOL bReuseAddr = pIniFile->GetInt(CFG_SEC_COMMON,
                                       CFG_REUSE_ADDRESS,
                                       CFG_REUSE_ADDRESS_DEFAULT);
    bReuseAddr = pIniFile->GetInt(CFG_SEC_KERNEL,
                                  CFG_REUSE_ADDRESS,
                                  bReuseAddr);
    pIniFile->Close();
    delete pIniFile;

    //�����ͳ�ʼ��CMsgSvrCommRouter������ʼ���ɹ���CMsgSvrCommRouter
    //����m_pCommRouterMan
    CMsgSvrCommRouter* pMsgSvrCommRouter;
    pMsgSvrCommRouter = new CMsgSvrCommRouter(m_pCommRouterMan,
                                              APP_MSG);
    nRet = m_pCommRouterMan->RegisterCommRouter(pMsgSvrCommRouter,
                                                APP_MSG);
    if(nRet != ERR_SUCCESS)
    {
        delete pMsgSvrCommRouter;
        return nRet;
    }

    //����һ��Ӧ������ΪAPP_MSG��CCommListener������Ըö������
    //CCommLayer::RegisterHandle�����ö����ָ����뵽m_CommListener��
    CCommListener* pCommListener = new CCommListener(APP_MSG, this);

    //������
    nRet = pCommListener->Open(LOCAL_ADDR, nMsgSvrPort, bReuseAddr);
    if(nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_OPEN_LISTEN_PORT_ERR, LOCAL_ADDR, 
              nMsgSvrPort, ACE_OS::last_error(), 
              ACE_OS::strerror(ACE_OS::last_error()));
        delete pCommListener;
        return ACE_OS::last_error();
    }

    //ע��Socket�¼�
    nRet = RegisterHandle(pCommListener,
                        ACE_Event_Handler::READ_MASK |
                        ACE_Event_Handler::WRITE_MASK |
                        ACE_Event_Handler::ACCEPT_MASK);
    if(nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_REG_SOCKET_ERR, LOCAL_ADDR, 
              nMsgSvrPort, nRet);
        delete pCommListener;
        return nRet;
    }
    m_CommListenerList.push_back(pCommListener);
    return nRet;
}


/******************************************************
// ������:  CMmlCommLayer::CMmlCommLayer
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ���캯��
// ����:
//       ����1 :PROCESS_TYPE const nProcessID
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CMmlCommLayer::CMmlCommLayer(PROCESS_TYPE const nProcessID)
                            :CTcpSvrCommLayer(nProcessID)
{
    //
}


/******************************************************
// ������: 	CMmlCommLayer::~CMmlCommLayer
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
CMmlCommLayer::~CMmlCommLayer()
{
    //
}


/******************************************************
// ������:  CMmlCommLayer::Init
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ��ʼ������
// ����:
//       ����1 :void
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
int CMmlCommLayer::Init(void)
{
    int nRet = CCommLayer::Init();
    if (nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //�����ͳ�ʼ��CMsgCliCommRouter���󣬲�����ʼ���ɹ���
    //CMsgCliCommRouter�������m_pCommRouterMan
    CMsgCliCommRouter* pMsgCliCommRouter = new CMsgCliCommRouter(
                                                m_pCommRouterMan,
                                                APP_MSG,
                                                m_nProcessID);
    nRet = m_pCommRouterMan->RegisterCommRouter(pMsgCliCommRouter,
                                                APP_MSG);
    if (nRet != ERR_SUCCESS)
    {
        delete pMsgCliCommRouter;
        return nRet;
    }

    //�������ļ��ж�ȡMML�˿ڵĸ�����Ϊÿ��MML�˿ڴ����ͳ�ʼ��һ��
    //CMmlCommRouter���󣬲�����ʼ���ɹ���CMmlCommRouter����
    //m_pCommRouterMan�������ļ��ж�ȡÿ��MML�˿ڵ�IP��ַ�Ͷ˿ں�
    //��Ϊÿ��MML�˿ڴ���һ��CCommListener���󣬲����øö����Open
    //���������ÿ��CCommListener���󶼵���CCommLayer::RegisterHandle
    //������������ЩCCommListener������뵽m_CommListenerList��
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    pIniFile->Open();

    //��������IP��ַ
    char szLocalIp[20];
    if(!pIniFile->GetString(CFG_SEC_MML,
                            CFG_MML_KEY_LOCAL_IP,
                            CFG_MML_KEY_LOCAL_IP_DEFAULT,
                            szLocalIp,
                            20))
    {
        TRACE(MTS_COMM,
              S_COMM_READ_MML_IP, 
              CFG_MML_KEY_LOCAL_IP_DEFAULT);
    }

    //��MML��ͨά���ڵĶ˿�
    int nLocalPortToCM = pIniFile->GetInt(CFG_SEC_MML,
                                        CFG_MML_KEY_CM_PORT,
                                        CFG_MML_KEY_CM_PORT_DEFAULT);
    //��MML�澯�ϱ��ڵĶ˿�
    int nLocalPortToAR = pIniFile->GetInt(CFG_SEC_MML,
                                        CFG_MML_KEY_AR_PORT,
                                        CFG_MML_KEY_AR_PORT_DEFAULT);
    //by ldf 2004-02-12
    //��MML�����ϱ��ڵĶ˿�
    int nLocalPortToPF = pIniFile->GetInt(CFG_SEC_MML,
                                        CFG_MML_KEY_PF_PORT,
                                        CFG_MML_KEY_PF_PORT_DEFAULT);
    //��MML���е���̨�Ķ˿�
    int nLocalPortToRD = pIniFile->GetInt(CFG_SEC_MML,
                                        CFG_MML_KEY_RD_PORT,
                                        CFG_MML_KEY_RD_PORT_DEFAULT);
    //��MML��convert���ӵĶ˿�
	int nLocalPortToConvert= pIniFile->GetInt(CFG_SEC_MML,
                                        CFG_AP_KEY_LOCAL_PORT_TO_AB,
                                        CFG_AP_KEY_LOCAL_PORT_TO_AB_DEFAULT);
    int nLocalPortToSynConf = pIniFile->GetInt(CFG_SEC_MML,
                                        CFG_AP_KEY_LOCAL_PORT_TO_SC,
                                        CFG_AP_KEY_LOCAL_PORT_TO_SC_DEFAULT);
    
    BOOL bReuseAddr = pIniFile->GetInt(CFG_SEC_COMMON,
                                       CFG_REUSE_ADDRESS,
                                       CFG_REUSE_ADDRESS_DEFAULT);
    bReuseAddr = pIniFile->GetInt(CFG_SEC_MML,
                                  CFG_REUSE_ADDRESS,
                                  bReuseAddr);
    pIniFile->Close();
    delete pIniFile;

    //ΪMML��ͨά���ڴ���CommRouter
    CMmlCommRouter* pMmlCommRouter;
    pMmlCommRouter = new CMmlCommRouter(m_pCommRouterMan,
                                        APP_MML_NORM);
    nRet = m_pCommRouterMan->RegisterCommRouter(pMmlCommRouter,
                                                APP_MML_NORM);
    if (nRet != ERR_SUCCESS)
    {
        delete pMmlCommRouter;
        return nRet;
    }

    //ΪMML��ͨά���ڴ�������
    CCommListener* pCommListener;
    pCommListener = new CCommListener(APP_MML_NORM, this);
    nRet = pCommListener->Open(szLocalIp, nLocalPortToCM, bReuseAddr);
    if (nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_OPEN_LISTEN_PORT_ERR, szLocalIp, 
              nLocalPortToCM, ACE_OS::last_error(), 
              ACE_OS::strerror(ACE_OS::last_error()));
        delete pCommListener;
        return nRet;
    }

    //ע��MML��ͨά���ڵ�����Socket�¼�
    nRet = RegisterHandle(pCommListener,
                        ACE_Event_Handler::READ_MASK |
                        ACE_Event_Handler::WRITE_MASK |
                        ACE_Event_Handler::ACCEPT_MASK);
    if(nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_REG_SOCKET_ERR, szLocalIp, 
              nLocalPortToCM, nRet);
        delete pCommListener;
        return nRet;
    }
    m_CommListenerList.push_back(pCommListener);

    //ΪMML�澯�ϱ��ڴ���CommRouter
    pMmlCommRouter = new CMmlCommRouter(m_pCommRouterMan,
                                        APP_MML_WARN);
    nRet = m_pCommRouterMan->RegisterCommRouter(pMmlCommRouter,
                                                APP_MML_WARN);
    if (nRet != ERR_SUCCESS)
    {
        delete pMmlCommRouter;
        return nRet;
    }

    //ΪMML�澯�ϱ��ڴ�������
    pCommListener = new CCommListener(APP_MML_WARN, this);
    nRet = pCommListener->Open(szLocalIp, nLocalPortToAR, bReuseAddr);
    if (nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_OPEN_LISTEN_PORT_ERR, szLocalIp, 
              nLocalPortToAR, ACE_OS::last_error(), 
              ACE_OS::strerror(ACE_OS::last_error()));
        delete pCommListener;
        return nRet;
    }

    //ע��MML�澯�ϱ��ڵ�����Socket�¼�
    nRet = RegisterHandle(pCommListener,
                        ACE_Event_Handler::READ_MASK |
                        ACE_Event_Handler::WRITE_MASK |
                        ACE_Event_Handler::ACCEPT_MASK);
    if(nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_REG_SOCKET_ERR, szLocalIp, 
              nLocalPortToAR, nRet);
        delete pCommListener;
        return nRet;
    }
    m_CommListenerList.push_back(pCommListener);
    //by ldf 2004-02-12
    //ΪMML�����ϱ��ڴ���CommRouter
    pMmlCommRouter = new CMmlCommRouter(m_pCommRouterMan,
                                        APP_MML_PERF);
    nRet = m_pCommRouterMan->RegisterCommRouter(pMmlCommRouter,
                                                APP_MML_PERF);
    if (nRet != ERR_SUCCESS)
    {
        delete pMmlCommRouter;
        return nRet;
    }

    //ΪMML�����ϱ��ڴ�������
    pCommListener = new CCommListener(APP_MML_PERF, this);
    nRet = pCommListener->Open(szLocalIp, nLocalPortToPF, bReuseAddr);
    if (nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_OPEN_LISTEN_PORT_ERR, szLocalIp, 
              nLocalPortToPF, ACE_OS::last_error(), 
              ACE_OS::strerror(ACE_OS::last_error()));
        delete pCommListener;
        return nRet;
    }


    //ע��MML�����ϱ��ڵ�����Socket�¼�
    nRet = RegisterHandle(pCommListener,
                        ACE_Event_Handler::READ_MASK |
                        ACE_Event_Handler::WRITE_MASK |
                        ACE_Event_Handler::ACCEPT_MASK);
    if(nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_REG_SOCKET_ERR, szLocalIp, 
              nLocalPortToPF, nRet);
        delete pCommListener;
        return nRet;
    }
    m_CommListenerList.push_back(pCommListener);


    //ΪMML���е���̨���ϱ��ڴ���CommRouter
    pMmlCommRouter = new CMmlCommRouter(m_pCommRouterMan,
                                        APP_MML_DEBUG);
    nRet = m_pCommRouterMan->RegisterCommRouter(pMmlCommRouter,
                                                APP_MML_DEBUG);
    if (nRet != ERR_SUCCESS)
    {
        delete pMmlCommRouter;
        return nRet;
    }

    //ΪMML���е���̨���ϱ��ڴ�������
    pCommListener = new CCommListener(APP_MML_DEBUG, this);
    nRet = pCommListener->Open(szLocalIp, nLocalPortToRD, bReuseAddr);
    if (nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_OPEN_LISTEN_PORT_ERR, szLocalIp, 
              nLocalPortToRD, ACE_OS::last_error(), 
              ACE_OS::strerror(ACE_OS::last_error()));
        delete pCommListener;
        return nRet;
    }

    //ע��MML���е���̨���ϱ��ڵ�����Socket�¼�
    nRet = RegisterHandle(pCommListener,
                        ACE_Event_Handler::READ_MASK |
                        ACE_Event_Handler::WRITE_MASK |
                        ACE_Event_Handler::ACCEPT_MASK);
    if(nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_REG_SOCKET_ERR, szLocalIp, 
              nLocalPortToRD, nRet);
        delete pCommListener;
        return nRet;
    }
    m_CommListenerList.push_back(pCommListener);

	//���ӶԸ澯��Ĳ�����ע��AlarmBoxRouter����������socket
	CAlarmBoxRouter* pAlarmBoxRouter = new CAlarmBoxRouter(m_pCommRouterMan, APP_ALARMBOX);
	nRet = m_pCommRouterMan->RegisterCommRouter(pAlarmBoxRouter, APP_ALARMBOX);
	if(nRet != ERR_SUCCESS)
	{
		delete pAlarmBoxRouter;
		return nRet;
	}

    pCommListener = new CCommListener(APP_ALARMBOX, this);
    nRet = pCommListener->Open(szLocalIp, nLocalPortToConvert, bReuseAddr);
    if (nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_OPEN_LISTEN_PORT_ERR, szLocalIp, 
              nLocalPortToConvert, ACE_OS::last_error(), 
              ACE_OS::strerror(ACE_OS::last_error()));
        delete pCommListener;
        return nRet;
    }

    //ע��Socket�¼�
    nRet = RegisterHandle(pCommListener,
                        ACE_Event_Handler::READ_MASK |
                        ACE_Event_Handler::WRITE_MASK |
                        ACE_Event_Handler::ACCEPT_MASK);
    if(nRet != ERR_SUCCESS)
    {
        delete pCommListener;
        return nRet;
    }
    m_CommListenerList.push_back(pCommListener);

    //������������ͬ���ڵĲ���������CommRouter    
    pMmlCommRouter = new CMmlCommRouter(m_pCommRouterMan, APP_SYN_CONFIG);
    nRet = m_pCommRouterMan->RegisterCommRouter(pMmlCommRouter, APP_SYN_CONFIG);
    if (nRet != ERR_SUCCESS)
    {
        delete pMmlCommRouter;
        return nRet;
    }
    
    //Ϊ��������ͬ���ڴ�������
    pCommListener = new CCommListener(APP_SYN_CONFIG, this);
    nRet = pCommListener->Open(szLocalIp, nLocalPortToSynConf, bReuseAddr);
    if (nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_OPEN_LISTEN_PORT_ERR, szLocalIp, 
            nLocalPortToSynConf, ACE_OS::last_error(), 
            ACE_OS::strerror(ACE_OS::last_error()));
        delete pCommListener;
        return nRet;
    }
    
    //ע����������ͬ���ڵ�����Socket�¼�
    nRet = RegisterHandle(pCommListener,
        ACE_Event_Handler::READ_MASK |
        ACE_Event_Handler::WRITE_MASK |
        ACE_Event_Handler::ACCEPT_MASK);
    if(nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_REG_SOCKET_ERR, szLocalIp, 
            nLocalPortToSynConf, nRet);
        delete pCommListener;
        return nRet;
    }
    m_CommListenerList.push_back(pCommListener);      

    return 0;
}


/******************************************************
// ������:  CMmlCommLayer::OnReceive
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    �ύ�ӵͲ����·�Ͻ�����������Ϣ
// ����:
//       ����1 :MSG_INTRA* pMsg
// ����ֵ: void
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
void CMmlCommLayer::OnReceive(MSG_INTRA* pMsg)
{
    //��pMsg->ReceiverMid���Ǳ�ʾ����ά���������κ�ҵ��ģ�飬�����
    //*m_pCommRouterMan�����Send�������Ӷ�����Ϣ���͸�MML�ͻ��ˣ�
    //�������CCommLayer::OnReceive����
    if((pMsg->ReceiverMid == MT_MANAGER) ||
       (pMsg->ReceiverMid == MT_ALARM) ||
       (pMsg->ReceiverMid == MT_BACKUP) ||
       (pMsg->ReceiverMid == MT_MMLSERVER) ||
       (pMsg->ReceiverMid == MT_BILLBROWSER) ||
       (pMsg->ReceiverMid == MT_LOG) ||
       (pMsg->ReceiverMid == MT_MSGDIST) ||
       (pMsg->ReceiverMid == MT_OMCSERVERINTERFACE) ||
       (pMsg->ReceiverMid == MT_BILLTASK) ||
       (pMsg->ReceiverMid == MT_COLAGENT) ||
       (pMsg->ReceiverMid == MT_CDRSTAT) ||
       (pMsg->ReceiverMid == MT_PERF))
    {
        //����Ƿ�����������ģ�����Ϣ���������ύ��Ϣ
        CCommLayer::OnReceive(pMsg);
    }
    else
    {
        //����Ƿ���MML�ͻ��˵���Ϣ������ת��
        pMsg->ReceiverPid = PT_OUTSIDE;
        m_pCommRouterMan->Send(pMsg);

        //�˴�һ��Ҫ�ͷ�pMsg
        delete pMsg;
    }
}


/******************************************************
// ������:  CClstrCommLayer::CClstrCommLayer
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ���캯��
// ����:
//       ����1 :PROCESS_TYPE const nProcessID
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CClstrCommLayer::CClstrCommLayer(PROCESS_TYPE const nProcessID)
                                 :CTcpSvrCommLayer(nProcessID)
{
    //
}



/******************************************************
// ������: 	CClstrCommLayer::~CClstrCommLayer
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
CClstrCommLayer::~CClstrCommLayer()
{
    //
}


/******************************************************
// ������:  CClstrCommLayer::Init
// ����:    Wangfeng
// ʱ��:    01-10-12
// ����:    ��ʼ������
// ����:
//       ����1 :void
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-12      Wangfeng
******************************************************/
int CClstrCommLayer::Init(void)
{
    int nRet = CCommLayer::Init();
    if (nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //�����ͳ�ʼ��CMsgCliCommRouter���󣬲�����ʼ���ɹ���
    //CMsgCliCommRouter�������m_pCommRouterMan
    CMsgCliCommRouter* pMsgCliCommRouter;
    pMsgCliCommRouter = new CMsgCliCommRouter(m_pCommRouterMan,
                                                APP_MSG,
                                                m_nProcessID);
    nRet = m_pCommRouterMan->RegisterCommRouter(pMsgCliCommRouter,
                                                APP_MSG);
    if (nRet != ERR_SUCCESS)
    {
        delete pMsgCliCommRouter;
    }
    return nRet;
}

/******************************************************
// ������:  CCfgSvrCommLayer::CCfgSvrCommLayer
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ���캯��
// ����:
//       ����1 :PROCESS_TYPE const nProcessID
// ����ֵ:
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CCfgSvrCommLayer::CCfgSvrCommLayer(PROCESS_TYPE const nProcessID)
                            :CTcpSvrCommLayer(nProcessID)
{
    //
}


/******************************************************
// ������: 	CCfgSvrCommLayer::~CCfgSvrCommLayer
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
CCfgSvrCommLayer::~CCfgSvrCommLayer()
{
    //
}


/******************************************************
// ������:  CCfgSvrCommLayer::Init
// ����:    Wangfeng
// ʱ��:    01-10-9
// ����:    ��ʼ������
// ����:
//       ����1 :void
// ����ֵ: int
// ע��:
// ����:
// ����            ����                  ����
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
int CCfgSvrCommLayer::Init(void)
{
    int nRet = CCommLayer::Init();
    if (nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //�����ͳ�ʼ��CMsgCliCommRouter���󣬲�����ʼ���ɹ���
    //CMsgCliCommRouter�������m_pCommRouterMan
    CMsgCliCommRouter* pMsgCliCommRouter = new CMsgCliCommRouter(
                                                m_pCommRouterMan,
                                                APP_MSG,
                                                m_nProcessID);
    nRet = m_pCommRouterMan->RegisterCommRouter(pMsgCliCommRouter,
                                                APP_MSG);
    if (nRet != ERR_SUCCESS)
    {
        delete pMsgCliCommRouter;
        return nRet;
    }

    //�������ļ��ж�ȡMML�˿ڵĸ�����Ϊÿ��MML�˿ڴ����ͳ�ʼ��һ��
    //CMmlCommRouter���󣬲�����ʼ���ɹ���CMmlCommRouter����
    //m_pCommRouterMan�������ļ��ж�ȡÿ��MML�˿ڵ�IP��ַ�Ͷ˿ں�
    //��Ϊÿ��MML�˿ڴ���һ��CCommListener���󣬲����øö����Open
    //���������ÿ��CCommListener���󶼵���CCommLayer::RegisterHandle
    //������������ЩCCommListener������뵽m_CommListenerList��
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    pIniFile->Open();

    //��������IP��ַ
    char szLocalIp[20];
    SNPRINTF(szLocalIp, sizeof(szLocalIp), "%s",  CFG_MML_KEY_LOCAL_IP_DEFAULT);

    //��Զ�����ÿڵĶ˿�
    int nLocalPortToCS = pIniFile->GetInt(CFG_SEC_MML,
                                          CFG_MML_KEY_CS_PORT,
                                          CFG_MML_KEY_CS_PORT_DEFAULT);
    BOOL bReuseAddr = pIniFile->GetInt(CFG_SEC_COMMON,
                                       CFG_REUSE_ADDRESS,
                                       CFG_REUSE_ADDRESS_DEFAULT);
    bReuseAddr = pIniFile->GetInt(CFG_SEC_MML,
                                  CFG_REUSE_ADDRESS,
                                  bReuseAddr);
    pIniFile->Close();
    delete pIniFile;

    //ΪԶ�����ÿڴ���CommRouter
    CMmlCommRouter* pMmlCommRouter;
    pMmlCommRouter = new CMmlCommRouter(m_pCommRouterMan,
                                        APP_MML_NORM);
    nRet = m_pCommRouterMan->RegisterCommRouter(pMmlCommRouter,
                                                APP_MML_NORM);
    if (nRet != ERR_SUCCESS)
    {
        delete pMmlCommRouter;
        return nRet;
    }

    //ΪԶ�����ÿڴ�������
    CCommListener* pCommListener;
    pCommListener = new CCommListener(APP_MML_NORM, this);
    nRet = pCommListener->Open(szLocalIp, nLocalPortToCS, bReuseAddr);
    if (nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_OPEN_LISTEN_PORT_ERR, szLocalIp, 
              nLocalPortToCS, ACE_OS::last_error(), 
              ACE_OS::strerror(ACE_OS::last_error()));
        delete pCommListener;
        return nRet;
    }

    //ע��MML��ͨά���ڵ�����Socket�¼�
    nRet = RegisterHandle(pCommListener,
                        ACE_Event_Handler::READ_MASK |
                        ACE_Event_Handler::WRITE_MASK |
                        ACE_Event_Handler::ACCEPT_MASK);
    if(nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_REG_SOCKET_ERR, szLocalIp, 
              nLocalPortToCS, nRet);
        delete pCommListener;
        return nRet;
    }
    m_CommListenerList.push_back(pCommListener);

    return 0;
}

CMeterAccCommLayer::CMeterAccCommLayer(PROCESS_TYPE const nProcessID)
                            :CTcpSvrCommLayer(nProcessID)
{
    //
}


CMeterAccCommLayer::~CMeterAccCommLayer()
{
    //
}

int CMeterAccCommLayer::Init(void)
{
    int nRet = CCommLayer::Init();
    if (nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //�����ͳ�ʼ��CMsgCliCommRouter���󣬲�����ʼ���ɹ���
    //CMsgCliCommRouter�������m_pCommRouterMan
    CMsgCliCommRouter* pMsgCliCommRouter = new CMsgCliCommRouter(
                                                m_pCommRouterMan,
                                                APP_MSG,
                                                m_nProcessID);
    nRet = m_pCommRouterMan->RegisterCommRouter(pMsgCliCommRouter,
                                                APP_MSG);
    if (nRet != ERR_SUCCESS)
    {
        delete pMsgCliCommRouter;
        return nRet;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//GTP'�Ʒѽӿ����� Z30971 2004/06/10

//���캯��
CBSCommlayer::CBSCommlayer(PROCESS_TYPE const nProcessID)
                          :CCommLayer(nProcessID)
{
} 

//��������
CBSCommlayer::~CBSCommlayer()
{
}

//��ʼ��
int CBSCommlayer::Init(void)
{
    int nRet = CCommLayer::Init();
    if (nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //�����ͳ�ʼ��CMsgCliCommRouter���󣬲�����ʼ���ɹ���
    //CMsgCliCommRouter�������m_pCommRouterMan
    CMsgCliCommRouter* pMsgCliCommRouter;
    pMsgCliCommRouter = new CMsgCliCommRouter(m_pCommRouterMan,
                                              APP_MSG,
                                              m_nProcessID);
    nRet = m_pCommRouterMan->RegisterCommRouter(pMsgCliCommRouter,
                                                APP_MSG);
    if (nRet != ERR_SUCCESS)
    {
        delete pMsgCliCommRouter;
        return nRet;
    }

   //���ݲ������þ�����ʼ��GTPCliCommRouter������
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    if(!pIniFile->Open())
    {
        delete pIniFile;
        return ERR_OPEN_CFG_FILE;
    }

    int nMode = pIniFile->GetInt(CFG_SEC_BS,
                                 CFG_BS_KEY_GTP_MODE,
                                 CFG_BS_KEY_GTP_MODE_DEFAULT
                                 );
    if (nMode == UDP_MODE) //����Ϊ UDP
    {
        CGTPUdpCliCommRouter *pGtpUdpCliCommRouter;
        pGtpUdpCliCommRouter = new CGTPUdpCliCommRouter(m_pCommRouterMan, APP_BS_GTP_UDP, PT_BS);
        
        //��RouterManע��
        nRet = m_pCommRouterMan->RegisterCommRouter(pGtpUdpCliCommRouter,
                                                    APP_BS_GTP_UDP);
        if (nRet != ERR_SUCCESS)
        {
            delete pGtpUdpCliCommRouter;
            return nRet;
        }
    }
    else
    {
        CGTPTcpCliCommRouter *pGtpTcpCliCommRouter;
        pGtpTcpCliCommRouter = new CGTPTcpCliCommRouter(m_pCommRouterMan, APP_BS_GTP_TCP, PT_BS);
        
        //��RouterManע��
        nRet = m_pCommRouterMan->RegisterCommRouter(pGtpTcpCliCommRouter,
                                                    APP_BS_GTP_TCP);
        if (nRet != ERR_SUCCESS)
        {
            delete pGtpTcpCliCommRouter;
            return nRet;
        }
    }

    return ERR_SUCCESS;
}
