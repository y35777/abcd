#include "../utility/config.h"
#include "../include/toolbox.h"
#include "comm_layer.h"
#include "ace/High_Res_Timer.h"

CCommLayer* CCommLayer::m_pCommLayer = NULL;
/******************************************************
// 函数名:  CCommListener::CCommListener
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    构造函数
// 输入:
//       参数1 :unsigned long uAppType
//       参数2 :CCommLayer* const pCommLayer
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CCommListener::CCommListener(unsigned long const uAppType,
                            CCommLayer* const pCommLayer)
{
    m_uAppType = uAppType;
    //使用框架提供的ASSERT函数
    ASSERT(pCommLayer != 0);
    m_pCommLayer = pCommLayer;
}



/******************************************************
// 函数名: 	CCommListener::~CCommListener
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    析构函数
// 输入:
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
CCommListener::~CCommListener()
{
    m_Acceptor.close();
    m_pCommLayer = 0;
}

/******************************************************
// 函数名:  CCommListener::get_handle
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    获取事件句柄
// 输入:
//       参数1 :void
// 返回值: ACE_HANDLE 返回事件句柄
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
ACE_HANDLE CCommListener::get_handle(void) const
{
    //返回m_Acceptor 对象中的句柄
    return m_Acceptor.get_handle();
}


/******************************************************
// 函数名:  CCommListener::Open
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    开始在指定的IP地址和端口上侦听
// 输入:
//       参数1 :const char* const szIPAddr
//       参数2 :const int nPort
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CCommListener::Open(const char* const szIPAddr,
                        const int nPort,
                        const BOOL bReuse)
{
    //设置IP地址和端口
    ACE_INET_Addr LocalAddr(nPort, szIPAddr);
    return m_Acceptor.open(LocalAddr, bReuse);
}


/******************************************************
// 函数名:  CCommListener::SOCK_handle_input
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    处理SOCKET输入事件
// 输入:
//       参数1 :void
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CCommListener::SOCK_handle_input(void)
{
    //接收连接并创建一个相应的CFrmSocket子类的对象
    CFrmSocket* pNewSocket = CreateFrmSocket();
    //张玉新 2002-12-25修改
	if(pNewSocket)
	//修改完毕
	{
		//通知通信层接收了一个新连接
		unsigned long uAppType = m_uAppType;
		if ((m_uAppType == APP_MSC_PRIMARY) ||
			(m_uAppType == APP_MSC_SECOND))
		{
			uAppType = APP_MAIN;
		}
		
		//颜文远2002.04.10添加
		if ((m_uAppType == APP_SOFTX_PRIMARY) ||
			(m_uAppType == APP_SOFTX_SECOND))
		{
			uAppType = APP_MAIN;
		}
		//颜文远2002.04.10添加结束
		
		int nRet = m_pCommLayer->AddFrmSocket(pNewSocket, uAppType);
	}
    return 0;
}


/******************************************************
// 函数名:  CCommListener::CreateFrmSocket
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    接收连接并根据m_uAppType的取值创建一个相应
//          CFrmSocket子类的对象
// 输入:
//       参数1 :void
// 返回值: CFrmSocket*
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CFrmSocket* CCommListener::CreateFrmSocket(void)
{
    //创建一个Socket流对象
    ACE_SOCK_Stream* pStream = new ACE_SOCK_Stream;

    //创建对应应用类型的Socket
    CFrmSocket* pFrmSocket = 0;
    switch(m_uAppType)
    {
    case APP_MSG :
    //用于IPC
        pFrmSocket = new CMsgFrmSocket(pStream);
        break;

    case APP_MSC_PRIMARY :
    //用于与主机的主MEM板通讯
        pFrmSocket = new CMemFrmSocket(pStream);
        ((CMemFrmSocket*)pFrmSocket)->SetbPrimary(TRUE);
        break;

    case APP_MSC_SECOND :
    //用于与主机的备MEM板通讯
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
    //用于MML客户端的普通维护口通讯
        pFrmSocket = new CMmlFrmSocket(pStream);
        ((CMmlFrmSocket*)pFrmSocket)->AddTags('\r', TRUE);
        ((CMmlFrmSocket*)pFrmSocket)->AddTags('\n', TRUE);
        ((CMmlFrmSocket*)pFrmSocket)->AddTags(0x02, TRUE);
        ((CMmlFrmSocket*)pFrmSocket)->AddTags(0x03, FALSE);
        ((CMmlFrmSocket*)pFrmSocket)->AddTags(';', FALSE);
        break;

    case APP_MML_WARN:
    //用于MML客户端的告警上报口通讯
    //因为告警端口上不接收数据，所以不必设置该对象接收的文本帧
    //的开始和结束标志
        pFrmSocket = new CMmlFrmSocket(pStream);
    //by ldf test 2004-08-17 对应问题单SWPD07814
    ((CMmlFrmSocket*)pFrmSocket)->AddTags('\r', TRUE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags('\n', TRUE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags(0x02, TRUE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags(0x03, FALSE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags(';', FALSE);
        break;

	//by ldf 2004-2-12
    case APP_MML_PERF:
    //用于MML客户端的性能上报口通讯
    //因为性能上报端口上不接收数据，所以不必设置该对象接收的文本帧
    //的开始和结束标志
        pFrmSocket = new CMmlFrmSocket(pStream);
    //by ldf test 2004-08-17 对应问题单SWPD07814
    ((CMmlFrmSocket*)pFrmSocket)->AddTags('\r', TRUE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags('\n', TRUE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags(0x02, TRUE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags(0x03, FALSE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags(';', FALSE);
        break;
        
    case APP_SYN_CONFIG:
        pFrmSocket = new CMmlFrmSocket(pStream);
    //by ldf test 2004-08-17 对应问题单SWPD07814
    ((CMmlFrmSocket*)pFrmSocket)->AddTags('\r', TRUE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags('\n', TRUE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags(0x02, TRUE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags(0x03, FALSE);
    ((CMmlFrmSocket*)pFrmSocket)->AddTags(';', FALSE);
        break;
        
    case APP_MML_DEBUG:
    //用于MML客户端的运行调试口通讯
    //因为运行调试端口上不接收数据，所以不必设置该对象接收的文
    //本帧的开始和结束标志
        pFrmSocket = new CMmlFrmSocket(pStream);
        break;
	
	case APP_ALARMBOX:
		//用于实现与告警箱（AlarmBox）convert模块通讯
		pFrmSocket = new CABFrmSocket(pStream);
		break;

	case APP_GTP_TCP:
		pFrmSocket = new CGtpFrmSocket(pStream);
		break;

    default:
        break;
    }

    if(pFrmSocket)
    //如果创建成功，则确认Socket连接
    {
        m_Acceptor.accept(*(pFrmSocket->GetSockStream()),
                        &(pFrmSocket->GetAddrRemote()));
        pFrmSocket->MakeLocalAddress();

        //张玉新 2002-12-24 在这里过滤IP地址
        CIPFilter *pFilter = ACE_Singleton<CIPFilter,ACE_Null_Mutex>::instance();
        if(pFilter == NULL)
        {
            return pFrmSocket;
        }
        ACE_INET_Addr &RemoteAddr = pFrmSocket->GetAddrRemote();
        //modifyed by chenliangwei, 2003-12-27, 解决MML客户端IP过滤不生效（字节序）问题，
        UINT4 uIPTmp = (UINT4)(RemoteAddr.get_ip_address());  
        if(pFilter->Filter(uIPTmp))
        {
            pFrmSocket->GetSockStream()->close();
            delete pFrmSocket;
            return NULL;
        }
        //张玉新
    } // Socket创建成功
    else
    {
        TRACE(MTS_COMM, S_COMM_CRT_SOCKET_ERR, m_uAppType);
        delete pStream;
    }

    return pFrmSocket;
}



/******************************************************
// 函数名: 	CCommTask::CCommTask
// 作者:    Wangfeng
// 时期:    01-12-31
// 描述:    
// 输入:
//       参数1 :CCommTaskMan* pCommTaskMan
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
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
// 函数名: 	CCommTask::~CCommTask
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    析构函数
// 输入:
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
CCommTask::~CCommTask()
{
    DelScheduleSocket();

    m_pCommTaskMan = 0;
}



/******************************************************
// 函数名:  CCommTask::open
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    本函数会启动一个用于检测Reactor事件的线程
// 输入:
//       参数1 :void* args = 0
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CCommTask::open(void* args)
{
    //调用ACE_Task_Base提供的activate函数
    return activate();
}


/******************************************************
// 函数名:  CCommTask::svc
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    线程函数,本函数是提供一个Reactor的事件循环
// 输入:
//       参数1 :void
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
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

        //在多个CCommTask对象时，保证只有一个线程调用秒事件函数
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

        //申请互斥量的使用权
        CCommMutex::Instance()->Acquire();

        DelScheduleSocket();

        RegScheduleSocket();

        SleepSocketOutput();

        //释放互斥量的使用权
        CCommMutex::Instance()->Release();
    }
    return 0;
}

/******************************************************
// 函数名:  CCommTask::RegisterHandle
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    将CSocketImpl子类的对象注册到本对象的ACE_Reactor
//          对象中
// 输入:
//       参数1 :CSocketImpl* pSocket
//       参数2 :ACE_Reactor_Mask mask
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
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
// 函数名: 	CCommTask::ScheduleRegister
// 作者:    Wangfeng
// 时期:    01-12-31
// 描述:    将pSocket指针列入待注册列表
// 输入:
//       参数1 :CSocketImpl* pSocket
//       参数2 :ACE_Reactor_Mask mask
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
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
// 函数名: 	CCommTask::ScheduleRemove
// 作者:    Wangfeng
// 时期:    01-11-22
// 描述:    将pSocket指针列入待删除列表
// 输入:
//       参数1 :CSocketImpl* pSocket
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
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
// 函数名: 	CCommTask::ScheduleWakeup
// 作者:    Wangfeng
// 时期:    01-12-31
// 描述:    将pSocket指针列入待注销output事件的列表
// 输入:
//       参数1 :CSocketImpl* pSocket
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
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
// 函数名: 	CCommTask::DelScheduleSocket
// 作者:    Wangfeng
// 时期:    01-11-22
// 描述:    删除待删除列表中的Socket对象
// 输入:
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-11-22      Wangfeng
******************************************************/
void CCommTask::DelScheduleSocket()
{
    //通过回调队列中所有成员的handle_close函数来删除
    //上层显式注销的Socket对象，这样避免了UNIX下因同步
    //回调handle_close函数而造成的死锁。

    while(m_SocketWillBeDel.size())
    {
        CSocketImpl* pSocket = m_SocketWillBeDel.back();

        //从ACE_Reactor中注销本对象的事件，但不需要回调
        //handle_close()函数
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
// 函数名: 	CCommTask::RegScheduleSocket
// 作者:    Wangfeng
// 时期:    01-12-31
// 描述:    注册待注册列表中的Socket对象
// 输入:
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
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
// 函数名: 	CCommTask::SleepSocketOutput
// 作者:    Wangfeng
// 时期:    01-12-31
// 描述:    暂停m_SocketWillRmOptHdl中Socket对象的output事件
// 输入:
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
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
// 函数名:  CCommTask::QuitComTask
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    结束线程
// 输入:
//       参数1 :void
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CCommTask::QuitComTask(void)
{
    m_Reactor.end_reactor_event_loop();
    //调用ACE_Task_Base提供的wait函数等待线程结束
    wait();
}


/******************************************************
// 函数名: 	CCommTaskMan::CCommTaskMan
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    构造函数
// 输入:    CCommLayer* pCommLayer
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
CCommTaskMan::CCommTaskMan(CCommLayer* pCommLayer)
{
    ASSERT(0 != pCommLayer);

    m_pCommLayer = pCommLayer;
}


/******************************************************
// 函数名:  CCommTaskMan::~CCommTaskMan
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    析构函数
// 输入:
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CCommTaskMan::~CCommTaskMan()
{
    //删除m_CommTaskList中所有的CCommTask对象
    while(m_CommTaskList.size() > 0)
    {
        CCommTask* pTask = m_CommTaskList.front();
        m_CommTaskList.pop_front();
        delete pTask;
    }

    m_pCommLayer = 0;
}


/******************************************************
// 函数名:  CCommTaskMan::Init
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    初始化函数
// 输入:
//       参数1 :void
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
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
// 函数名:  CCommTaskMan::RegisterHandle
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    注册CSocketImpl子类的对象
// 输入:
//       参数1 :CSocketImpl* pSocket
//       参数2 :ACE_Reactor_Mask mask
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CCommTaskMan::RegisterHandle(CSocketImpl* pSocket,
                                 ACE_Reactor_Mask mask)
{
    //如果m_CommTaskList为空，则创建一个CCommTask对象，
    //并将该对象指针插入到m_CommTaskList中
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

        //如果是Reactor注册数已满，则继续下一个，否则返回
        if (ERR_REACTOR_FULL != nRegResult)
        {
            return nRegResult;
        }
    }

    //已遍历完所有的CCommTask对象，也不能注册成功，
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
// 函数名: 	CCommTaskMan::Active
// 作者:    Wangfeng
// 时期:    01-11-14
// 描述:    激活所有CCommTask对象
// 输入:
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
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
// 函数名: 	CCommTaskMan::DeActive
// 作者:    Wangfeng
// 时期:    01-11-14
// 描述:    去激活所有CCommTask对象
// 输入:
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
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
// 函数名: 	CCommTaskMan::GetCommLayer
// 作者:    Wangfeng
// 时期:    01-12-31
// 描述:    取CCommLayer对象的指针
// 输入:
// 返回值: CCommLayer* 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-12-31      Wangfeng
******************************************************/
CCommLayer* CCommTaskMan::GetCommLayer()
{
    return m_pCommLayer;
}


/******************************************************
// 函数名:  CCommLayer::CCommLayer
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    构造函数
// 输入:
//       参数1 :PROCESS_TYPE const nProcessID
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
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
// 函数名:  CCommLayer::~CCommLayer
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    析构函数
// 输入:
//       参数1 :void
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CCommLayer::~CCommLayer()
{
    ExitInstance();
}


/******************************************************
// 函数名:  CCommLayer::Init
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    初始化函数
// 输入:
//       参数1 :void
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
int CCommLayer::Init(void)
{
    m_pCommRouterMan = new CCommRouterMan(this);

    m_pCommTaskMan = new CCommTaskMan(this);

	//张玉新 2002-12-26
	(ACE_Singleton<CIPFilter,ACE_Null_Mutex>::instance())->Init();
	//张玉新
    //调用m_pCommTaskMan对象的Init函数，并返回该函数的返回值
    return m_pCommTaskMan->Init();
}


/******************************************************
// 函数名:  CCommLayer::ExitInstance
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    完成必要的清理工作
// 输入:
//       参数1 :void
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
void CCommLayer::ExitInstance(void)
{
    //去激活CommTask
    if(m_pCommTaskMan)
    {
        m_pCommTaskMan->DeActive();
    }

    //释放m_pCommRouterMan指向的对象
    if(m_pCommRouterMan)
    {
        delete m_pCommRouterMan;
        m_pCommRouterMan = 0;
    }
    
    //释放m_pCommTaskMan指向的对象
    if(m_pCommTaskMan)
    {
        delete m_pCommTaskMan;
        m_pCommTaskMan = 0;
    }
}


/******************************************************
// 函数名:  CCommLayer::Instance
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    根据进程的编号获得相应的通信层对象
// 输入:
//       参数1 :unsigned long const nProcessID
// 返回值: CCommLayer* const
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CCommLayer* const CCommLayer::Instance(unsigned long const nProcessID)
{
    if (m_pCommLayer != 0)
    //如果已存在单实例对象
    {
        return m_pCommLayer;
    }

    //如果不存在单实例对象，则根据进程编号创建该单实例对象
    switch (nProcessID)
    {
    case PT_KERNEL:     //KERNEL进程
        m_pCommLayer = new CMsgSvrCommLayer((PROCESS_TYPE)nProcessID);
        break;

    case PT_OM:         //操作维护进程
        m_pCommLayer = new CMmlCommLayer((PROCESS_TYPE)nProcessID);
        break;

    case PT_CLSTR:      //双机进程
        m_pCommLayer = new CClstrCommLayer((PROCESS_TYPE)nProcessID);
        break;
    case PT_METERACC:   //计次表累加进程
        m_pCommLayer = new CMeterAccCommLayer((PROCESS_TYPE)nProcessID);
        break;
    case PT_CFGSVR:     //远程配置进程
        m_pCommLayer = new CCfgSvrCommLayer((PROCESS_TYPE)nProcessID);
        break;
    
        //GTP'计费接口增加 Z30971 2004/06
    case PT_BS:         //计费接口进程
        m_pCommLayer = new CBSCommlayer((PROCESS_TYPE)nProcessID);
        break;

    default:
        if(nProcessID >= PT_CO_BASE && nProcessID <= MAX_PT_CO_TYPE)
        {
            m_pCommLayer = new CClstrCommLayer((PROCESS_TYPE)nProcessID);
        }
        else if ((nProcessID >= PT_AP_BASE) && (nProcessID < PT_CO_BASE))//接入点进程
        {
            //读取配置，若编号为nProcessID进程的通信层支持滑动窗口协议，
            //则创建一个CSlideWndCommLayer对象，将该对象的指针赋值给
            //m_pCommLayer；若编号为nProcessID进程的通信层支持GTP’协
            //议，则创建一个CGtpCommLayer对象，将该对象的指针赋值给
            //m_pCommLayer

            //从配置文件中读取接入点类型
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

            //根据接入点类型号创建相关CCommLayer对象
            switch(nAPType)
            {
            case AP_PS:
            case AP_PS_RESERVED:
            case AP_WLAN:
                m_pCommLayer = new CGtpCommLayer(
                                    (PROCESS_TYPE)nProcessID);
                break;

			//颜文远2002.04.10添加
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
 			//颜文远2002.04.10添加结束
            }
        }
        else
        {
            return 0;
        }
    }

    //申请互斥量的使用权
    CCommMutex::Instance()->Acquire();

    //初始化通信层
    int nRet = m_pCommLayer->Init();

    //释放互斥量的使用权
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
// 函数名:  CCommLayer::Destroy
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    销毁对象
// 输入:
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
void CCommLayer::Destroy()
{
    if (m_pCommLayer != 0)
    {
        //释放m_pCommLayer->m_pCommTaskMan指向的对象；
        delete m_pCommLayer;
        m_pCommLayer = NULL;
    }

    //销毁网络层互斥锁单实例对象
    CCommMutex::Destroy();

    TRACE(MTS_COMM, S_COMM_UNINSTANCE_COMMLAYER);
}



/******************************************************
// 函数名:  CCommLayer::AddFrmSocket
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    为应用类型为uAppType的端口增加一个新连接
// 输入:
//       参数1 :CFrmSocket* pFrmSocket
//       参数2 :unsigned long uAppType
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
int CCommLayer::AddFrmSocket(CFrmSocket* pFrmSocket,
                             unsigned long uAppType)
{
    //调用*m_pCommTaskMan对象的RegisterHandle函数
    int nRet = RegisterHandle(pFrmSocket,
                            ACE_Event_Handler::READ_MASK |
                            ACE_Event_Handler::WRITE_MASK |
                            ACE_Event_Handler::ACCEPT_MASK);
    if(nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //调用*m_pCommRouterMan对象的AddFrmSocket函数，并返回
    //该函数的返回值
    return m_pCommRouterMan->AddFrmSocket(pFrmSocket, uAppType);
}


/******************************************************
// 函数名:  CCommLayer::RegisterHandle
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    注册CSocketImpl子类的对象
// 输入:
//       参数1 :CSocketImpl* pSocket
//       参数2 :ACE_Reactor_Mask mask
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
int CCommLayer::RegisterHandle(CSocketImpl* pSocket,
                               ACE_Reactor_Mask mask)
{
    //调用*m_pCommTaskMan对象的RegisterHandle函数，并返回
    //该函数的返回值
    return m_pCommTaskMan->RegisterHandle(pSocket, mask);
}


/******************************************************
// 函数名:  CCommLayer::Send
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    发送消息
// 输入:
//       参数1 :const MSG_INTRA* const pMsg
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
int CCommLayer::Send(const MSG_INTRA* const pMsg)
{
    int nRet;
    //申请互斥量的使用权
    CCommMutex::Instance()->Acquire();

    //调用*m_pCommRouterMan对象的Send函数，并返回该函数的返回值
    nRet = m_pCommRouterMan->Send(pMsg);

    //释放互斥量的使用权
    CCommMutex::Instance()->Release();

    return nRet;
}


/******************************************************
// 函数名:  CCommLayer::OnReceive
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    提交从低层的链路上接收上来的消息
// 输入:
//       参数1 :MSG_INTRA* pMsg
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
void CCommLayer::OnReceive(MSG_INTRA* pMsg)
{
    //向本进程的相关模块提交消息帧，该消息帧由框架负责释放
    CMsgProcessor::SendMsg(pMsg);
}


/******************************************************
// 函数名:  CCommLayer::Timer1Sec
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    秒事件处理函数
// 输入:
//       参数1 :void
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
void CCommLayer::Timer1Sec(void)
{
    //申请互斥量的使用权
    CCommMutex::Instance()->Acquire();

    //调用*m_pCommTaskMan对象的Timer1Sec函数
    //m_pCommTaskMan->Timer1Sec();

    //调用*m_pCommRouterMan对象的Timer1Sec函数
    m_pCommRouterMan->Timer1Sec();

    //释放互斥量的使用权
    CCommMutex::Instance()->Release();

}


/******************************************************
// 函数名:  CGtpCommLayer::CGtpCommLayer
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    构造函数
// 输入:
//       参数1 :PROCESS_TYPE const nProcessID
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CGtpCommLayer::CGtpCommLayer(PROCESS_TYPE const nProcessID)
                             :CTcpSvrCommLayer(nProcessID)
{
    //
}


/******************************************************
// 函数名: 	CGtpCommLayer::~CGtpCommLayer
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    析构函数
// 输入:
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
CGtpCommLayer::~CGtpCommLayer()
{
    //
}


/******************************************************
// 函数名:  CGtpCommLayer::Init
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    初始化函数
// 输入:
//       参数1 :void
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
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

    //创建和初始化CMsgCliCommRouter对象，并将初始化成功的
    //CMsgCliCommRouter对象加入m_pCommRouterMan
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

    //创建和初始化CGtpUdpCommRouter对象，并将初始化成功的
    //CGtpUdpCommRouter对象加入m_pCommRouterMan，返回初始化结果
    CGtpUdpCommRouter* pGtpCommRouter;
    pGtpCommRouter = new CGtpUdpCommRouter(m_pCommRouterMan, APP_GTP_UDP);
    //初始化
    nRet = pGtpCommRouter->Init(m_nProcessID - PT_AP_BASE + 1);
    if (nRet != ERR_SUCCESS)
    {
        delete pGtpCommRouter;
        return nRet;
    }

    //向RouterMan注册
    nRet = m_pCommRouterMan->RegisterCommRouter(pGtpCommRouter,
                                                APP_GTP_UDP);
    if (nRet != ERR_SUCCESS)
    {
        delete pGtpCommRouter;
        return nRet;
    }

    //创建和初始化CGtpFrmCommRouter对象，并将初始化成功的
    //CGtpFrmCommRouter对象加入m_pCommRouterMan，返回初始化结果
    CGtpFrmCommRouter* pGtpFrmCommRouter = new CGtpFrmCommRouter(m_pCommRouterMan, APP_GTP_TCP);
    //初始化
    nRet = pGtpFrmCommRouter->Init(m_nProcessID - PT_AP_BASE + 1);
    if (nRet != ERR_SUCCESS)
    {
        delete pGtpFrmCommRouter;
        return nRet;
    }

    //向RouterMan注册
    nRet = m_pCommRouterMan->RegisterCommRouter(pGtpFrmCommRouter,
                                                APP_GTP_TCP);
    if (nRet != ERR_SUCCESS)
    {
        delete pGtpFrmCommRouter;
        return nRet;
    }
    
    //创建主备CCommListener对象，从配置文件中读取主备端口的
    //IP地址和端口号，分别调用主备CCommListener对象的Open函数；
    //调用CCommLayer::RegisterHandle函数将主备CCommListener对
    //象注册到*m_pCommTaskMan对象中，将主备CCommListener对象加
    //入到m_CommListenerList中

    //打开配置文件
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    if(!pIniFile->Open())
    {
        delete pIniFile;
        return ERR_OPEN_CFG_FILE;
    }

    //读取本进程对应的接入点号
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
     	读UDP与TCP共同的地址配置信息
    */
    //读取UDP与TCP的可用性掩码
    int nUdpTcpMask = pIniFile->GetInt(strSec,
                                       CFG_AP_KEY_UDP_TCP_MASK,
                                       CFG_AP_KEY_UDP_TCP_MASK_DEFAULT);

    //读取与GSN相连的主链路IP地址
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

    //读取与GSN相连的主链路的端口号
    INT4 nPMLocalPort = pIniFile->GetInt(strSec,
                                         CFG_AP_KEY_LOCAL_PORT_TO_PM,
                                         CFG_AP_KEY_LOCAL_PORT_TO_PM_GSN_DEFAULT);
    
    //读取与GSN相连的备链路IP地址
    char szFMLocalIp[20];
    pIniFile->GetString(strSec,
                        CFG_AP_KEY_LOCAL_IP_TO_FM,
                        CFG_AP_KEY_LOCAL_IP_TO_FM_GSN_DEFAULT,
                        szFMLocalIp,
                        sizeof(szFMLocalIp));

    //读取与GSN相连的备链路的端口号
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

        //注册Socket事件
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
            //注册Socket事件
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
// 函数名:  CTcpSvrCommLayer::CTcpSvrCommLayer
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    构造函数
// 输入:
//       参数1 :PROCESS_TYPE const nProcessID
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CTcpSvrCommLayer::CTcpSvrCommLayer(PROCESS_TYPE const nProcessID)
                                   :CCommLayer(nProcessID)
{
    //
}


/******************************************************
// 函数名:  CTcpSvrCommLayer::~CTcpSvrCommLayer
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    析构函数
// 输入:
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CTcpSvrCommLayer::~CTcpSvrCommLayer()
{
    //不用删除m_CommListenerList中的CCommListener对象
    //该对象在析构管理它的ACE_Reactor对象时会自动删除
    //这里只需要清除LIST表中的元素即可
    m_CommListenerList.clear();
}


/******************************************************
// 函数名:  CSlideWndCommLayer::CSlideWndCommLayer
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    构造函数
// 输入:
//       参数1 :PROCESS_TYPE const nProcessID
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CSlideWndCommLayer::CSlideWndCommLayer(PROCESS_TYPE const nProcessID)
                                       :CTcpSvrCommLayer(nProcessID)
{
    //
}



/******************************************************
// 函数名: 	CSlideWndCommLayer::~CSlideWndCommLayer
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    析构函数
// 输入:
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
CSlideWndCommLayer::~CSlideWndCommLayer()
{
    //
}


/******************************************************
// 函数名:  CSlideWndCommLayer::Init
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    初始化函数
// 输入:
//       参数1 :void
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
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

    //创建和初始化CMsgCliCommRouter对象，并将初始化成功的
    //CMsgCliCommRouter对象加入m_pCommRouterMan
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

    //创建和初始化CSlideWndCommRouter对象，并将初始化成功的
    //CSlideWndCommRouter对象加入m_pCommRouterMan
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

    //创建主备CCommListener对象，从配置文件中读取主备端口的
    //IP地址和端口号，分别调用主备CCommListener对象的Open函数；
    //调用CCommLayer::RegisterHandle函数将主备CCommListener对
    //象注册到*m_pCommTaskMan对象中，将主备CCommListener对象加
    //入到m_CommListenerList中
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
	//颜文远2002.04.10添加
	int nLinkTimeOut = pIniFile->GetInt(strSec, CFG_AP_KEY_LINK_TIMEOUT, 
										CFG_AP_KEY_LINK_TIMEOUT_DEFAULT);
	pSlideWndCommRouter->SetLinkTimeOut(nLinkTimeOut);
	//颜文远2002.04.10添加结束
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

    //注册Socket事件
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
    //注册Socket事件
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


//颜文远2002.04.08添加
//构造函数
CSoftXCommLayer::CSoftXCommLayer(PROCESS_TYPE const nProcessID)
				: CTcpSvrCommLayer(nProcessID)
{
}

//析构函数
CSoftXCommLayer::~CSoftXCommLayer()
{
}

/******************************************************
// 描述:    初始化函数
// 输入:
//       参数1 :void
// 返回值: int
// 注释:
******************************************************/
int CSoftXCommLayer::Init(void)
{
    int nRet = CCommLayer::Init();
    if (nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //创建和初始化CMsgCliCommRouter对象，并将初始化成功的
    //CMsgCliCommRouter对象加入m_pCommRouterMan
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

    //创建和初始化CSoftXCommRouter对象，并将初始化成功的
    //CSoftXCommRouter对象加入m_pCommRouterMan
    CSoftXCommRouter* pSoftXCommRouter;
    pSoftXCommRouter = new CSoftXCommRouter(m_pCommRouterMan, APP_MAIN);
    nRet = m_pCommRouterMan->RegisterCommRouter(pSoftXCommRouter,
                                                APP_MAIN);
    if (nRet != ERR_SUCCESS)
    {
        delete pSoftXCommRouter;
        return nRet;
    }

    //创建主备CCommListener对象，从配置文件中读取主备端口的
    //IP地址和端口号，分别调用主备CCommListener对象的Open函数；
    //调用CCommLayer::RegisterHandle函数将主备CCommListener对
    //象注册到*m_pCommTaskMan对象中，将主备CCommListener对象加
    //入到m_CommListenerList中
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

    //注册Socket事件
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
    //注册Socket事件
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

//颜文远2002.04.08添加结束



/******************************************************
// 函数名:  CMsgSvrCommLayer::CMsgSvrCommLayer
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    构造函数
// 输入:
//       参数1 :PROCESS_TYPE const nProcessID
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CMsgSvrCommLayer::CMsgSvrCommLayer(PROCESS_TYPE const nProcessID)
                                   :CTcpSvrCommLayer(nProcessID)
{
    //
}


/******************************************************
// 函数名: 	CMsgSvrCommLayer::~CMsgSvrCommLayer
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    析构函数
// 输入:
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
CMsgSvrCommLayer::~CMsgSvrCommLayer()
{
    //
}


/******************************************************
// 函数名:  CMsgSvrCommLayer::Init
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    初始化函数
// 输入:
//       参数1 :void
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
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

    //从配置文件中读取MsgSvr的端口号，缺省值为9999
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

    //创建和初始化CMsgSvrCommRouter，将初始化成功的CMsgSvrCommRouter
    //加入m_pCommRouterMan
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

    //创建一个应用类型为APP_MSG的CCommListener对象，针对该对象调用
    //CCommLayer::RegisterHandle，将该对象的指针加入到m_CommListener中
    CCommListener* pCommListener = new CCommListener(APP_MSG, this);

    //打开侦听
    nRet = pCommListener->Open(LOCAL_ADDR, nMsgSvrPort, bReuseAddr);
    if(nRet != ERR_SUCCESS)
    {
        TRACE(MTS_COMM, S_COMM_OPEN_LISTEN_PORT_ERR, LOCAL_ADDR, 
              nMsgSvrPort, ACE_OS::last_error(), 
              ACE_OS::strerror(ACE_OS::last_error()));
        delete pCommListener;
        return ACE_OS::last_error();
    }

    //注册Socket事件
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
// 函数名:  CMmlCommLayer::CMmlCommLayer
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    构造函数
// 输入:
//       参数1 :PROCESS_TYPE const nProcessID
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CMmlCommLayer::CMmlCommLayer(PROCESS_TYPE const nProcessID)
                            :CTcpSvrCommLayer(nProcessID)
{
    //
}


/******************************************************
// 函数名: 	CMmlCommLayer::~CMmlCommLayer
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    析构函数
// 输入:
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
CMmlCommLayer::~CMmlCommLayer()
{
    //
}


/******************************************************
// 函数名:  CMmlCommLayer::Init
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    初始化函数
// 输入:
//       参数1 :void
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
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

    //创建和初始化CMsgCliCommRouter对象，并将初始化成功的
    //CMsgCliCommRouter对象加入m_pCommRouterMan
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

    //从配置文件中读取MML端口的个数，为每个MML端口创建和初始化一个
    //CMmlCommRouter对象，并将初始化成功的CMmlCommRouter加入
    //m_pCommRouterMan从配置文件中读取每个MML端口的IP地址和端口号
    //，为每个MML端口创建一个CCommListener对象，并调用该对象的Open
    //函数，针对每个CCommListener对象都调用CCommLayer::RegisterHandle
    //函数，并将这些CCommListener对象加入到m_CommListenerList中
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    pIniFile->Open();

    //读侦听的IP地址
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

    //读MML普通维护口的端口
    int nLocalPortToCM = pIniFile->GetInt(CFG_SEC_MML,
                                        CFG_MML_KEY_CM_PORT,
                                        CFG_MML_KEY_CM_PORT_DEFAULT);
    //读MML告警上报口的端口
    int nLocalPortToAR = pIniFile->GetInt(CFG_SEC_MML,
                                        CFG_MML_KEY_AR_PORT,
                                        CFG_MML_KEY_AR_PORT_DEFAULT);
    //by ldf 2004-02-12
    //读MML性能上报口的端口
    int nLocalPortToPF = pIniFile->GetInt(CFG_SEC_MML,
                                        CFG_MML_KEY_PF_PORT,
                                        CFG_MML_KEY_PF_PORT_DEFAULT);
    //读MML运行调试台的端口
    int nLocalPortToRD = pIniFile->GetInt(CFG_SEC_MML,
                                        CFG_MML_KEY_RD_PORT,
                                        CFG_MML_KEY_RD_PORT_DEFAULT);
    //读MML与convert连接的端口
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

    //为MML普通维护口创建CommRouter
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

    //为MML普通维护口创建侦听
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

    //注册MML普通维护口的侦听Socket事件
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

    //为MML告警上报口创建CommRouter
    pMmlCommRouter = new CMmlCommRouter(m_pCommRouterMan,
                                        APP_MML_WARN);
    nRet = m_pCommRouterMan->RegisterCommRouter(pMmlCommRouter,
                                                APP_MML_WARN);
    if (nRet != ERR_SUCCESS)
    {
        delete pMmlCommRouter;
        return nRet;
    }

    //为MML告警上报口创建侦听
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

    //注册MML告警上报口的侦听Socket事件
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
    //为MML性能上报口创建CommRouter
    pMmlCommRouter = new CMmlCommRouter(m_pCommRouterMan,
                                        APP_MML_PERF);
    nRet = m_pCommRouterMan->RegisterCommRouter(pMmlCommRouter,
                                                APP_MML_PERF);
    if (nRet != ERR_SUCCESS)
    {
        delete pMmlCommRouter;
        return nRet;
    }

    //为MML性能上报口创建侦听
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


    //注册MML性能上报口的侦听Socket事件
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


    //为MML运行调试台的上报口创建CommRouter
    pMmlCommRouter = new CMmlCommRouter(m_pCommRouterMan,
                                        APP_MML_DEBUG);
    nRet = m_pCommRouterMan->RegisterCommRouter(pMmlCommRouter,
                                                APP_MML_DEBUG);
    if (nRet != ERR_SUCCESS)
    {
        delete pMmlCommRouter;
        return nRet;
    }

    //为MML运行调试台的上报口创建侦听
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

    //注册MML运行调试台的上报口的侦听Socket事件
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

	//增加对告警箱的操作，注册AlarmBoxRouter并增加侦听socket
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

    //注册Socket事件
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

    //增加配置数据同步口的操作，创建CommRouter    
    pMmlCommRouter = new CMmlCommRouter(m_pCommRouterMan, APP_SYN_CONFIG);
    nRet = m_pCommRouterMan->RegisterCommRouter(pMmlCommRouter, APP_SYN_CONFIG);
    if (nRet != ERR_SUCCESS)
    {
        delete pMmlCommRouter;
        return nRet;
    }
    
    //为配置数据同步口创建侦听
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
    
    //注册配置数据同步口的侦听Socket事件
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
// 函数名:  CMmlCommLayer::OnReceive
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    提交从低层的链路上接收上来的消息
// 输入:
//       参数1 :MSG_INTRA* pMsg
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
void CMmlCommLayer::OnReceive(MSG_INTRA* pMsg)
{
    //若pMsg->ReceiverMid不是表示操作维护进程中任何业务模块，则调用
    //*m_pCommRouterMan对象的Send函数，从而将消息发送给MML客户端，
    //否则调用CCommLayer::OnReceive函数
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
        //如果是发给本进程内模块的消息，则向上提交消息
        CCommLayer::OnReceive(pMsg);
    }
    else
    {
        //如果是发给MML客户端的消息，则再转发
        pMsg->ReceiverPid = PT_OUTSIDE;
        m_pCommRouterMan->Send(pMsg);

        //此处一定要释放pMsg
        delete pMsg;
    }
}


/******************************************************
// 函数名:  CClstrCommLayer::CClstrCommLayer
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    构造函数
// 输入:
//       参数1 :PROCESS_TYPE const nProcessID
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CClstrCommLayer::CClstrCommLayer(PROCESS_TYPE const nProcessID)
                                 :CTcpSvrCommLayer(nProcessID)
{
    //
}



/******************************************************
// 函数名: 	CClstrCommLayer::~CClstrCommLayer
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    析构函数
// 输入:
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
CClstrCommLayer::~CClstrCommLayer()
{
    //
}


/******************************************************
// 函数名:  CClstrCommLayer::Init
// 作者:    Wangfeng
// 时期:    01-10-12
// 描述:    初始化函数
// 输入:
//       参数1 :void
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
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

    //创建和初始化CMsgCliCommRouter对象，并将初始化成功的
    //CMsgCliCommRouter对象加入m_pCommRouterMan
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
// 函数名:  CCfgSvrCommLayer::CCfgSvrCommLayer
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    构造函数
// 输入:
//       参数1 :PROCESS_TYPE const nProcessID
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-9       Wangfeng
******************************************************/
CCfgSvrCommLayer::CCfgSvrCommLayer(PROCESS_TYPE const nProcessID)
                            :CTcpSvrCommLayer(nProcessID)
{
    //
}


/******************************************************
// 函数名: 	CCfgSvrCommLayer::~CCfgSvrCommLayer
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    析构函数
// 输入:
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
CCfgSvrCommLayer::~CCfgSvrCommLayer()
{
    //
}


/******************************************************
// 函数名:  CCfgSvrCommLayer::Init
// 作者:    Wangfeng
// 时期:    01-10-9
// 描述:    初始化函数
// 输入:
//       参数1 :void
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
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

    //创建和初始化CMsgCliCommRouter对象，并将初始化成功的
    //CMsgCliCommRouter对象加入m_pCommRouterMan
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

    //从配置文件中读取MML端口的个数，为每个MML端口创建和初始化一个
    //CMmlCommRouter对象，并将初始化成功的CMmlCommRouter加入
    //m_pCommRouterMan从配置文件中读取每个MML端口的IP地址和端口号
    //，为每个MML端口创建一个CCommListener对象，并调用该对象的Open
    //函数，针对每个CCommListener对象都调用CCommLayer::RegisterHandle
    //函数，并将这些CCommListener对象加入到m_CommListenerList中
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    pIniFile->Open();

    //读侦听的IP地址
    char szLocalIp[20];
    SNPRINTF(szLocalIp, sizeof(szLocalIp), "%s",  CFG_MML_KEY_LOCAL_IP_DEFAULT);

    //读远程配置口的端口
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

    //为远程配置口创建CommRouter
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

    //为远程配置口创建侦听
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

    //注册MML普通维护口的侦听Socket事件
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

    //创建和初始化CMsgCliCommRouter对象，并将初始化成功的
    //CMsgCliCommRouter对象加入m_pCommRouterMan
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
//GTP'计费接口增加 Z30971 2004/06/10

//构造函数
CBSCommlayer::CBSCommlayer(PROCESS_TYPE const nProcessID)
                          :CCommLayer(nProcessID)
{
} 

//析构函数
CBSCommlayer::~CBSCommlayer()
{
}

//初始化
int CBSCommlayer::Init(void)
{
    int nRet = CCommLayer::Init();
    if (nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //创建和初始化CMsgCliCommRouter对象，并将初始化成功的
    //CMsgCliCommRouter对象加入m_pCommRouterMan
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

   //根据参数配置决定初始化GTPCliCommRouter的类型
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
    if (nMode == UDP_MODE) //类型为 UDP
    {
        CGTPUdpCliCommRouter *pGtpUdpCliCommRouter;
        pGtpUdpCliCommRouter = new CGTPUdpCliCommRouter(m_pCommRouterMan, APP_BS_GTP_UDP, PT_BS);
        
        //向RouterMan注册
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
        
        //向RouterMan注册
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
