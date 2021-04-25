#include "sockets.h"
#include "comm_router.h"
#include "comm_layer.h"
#include "../include/toolbox.h"
#include <algorithm>
CCommMutex* CCommMutex::g_pTheMutex = 0;

/******************************************************
// 函数名:  CCommMutex::CCommMutex
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    构造函数
// 输入:
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CCommMutex::CCommMutex()
{
    //
}


/******************************************************
// 函数名:  CCommMutex::~CCommMutex
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
CCommMutex::~CCommMutex()
{
    //
}


/******************************************************
// 函数名:  CCommMutex::Instance
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    获得单实例对象的指针
// 输入:
// 返回值: CCommMutex* const
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CCommMutex* const CCommMutex::Instance()
{
    if(g_pTheMutex == NULL)
    {
        //创建一个CCommMutex对象
        g_pTheMutex = new CCommMutex;
    }
    return g_pTheMutex;
}


/******************************************************
// 函数名:  CCommMutex::Destroy
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    删除单实例对象
// 输入:
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
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
// 函数名:  CCommMutex::Acquire
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    获得互斥量
// 输入:
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CCommMutex::Acquire()
{
    m_AceMutex.acquire();
}


/******************************************************
// 函数名:  CCommMutex::Release
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    释放互斥量
// 输入:
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
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
	//打开配置文件
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
	//关闭配置文件操作对象 
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
		if(m_nMode == 0) //限制模式
		{
			if(it == m_IPList.end())
			    return 0;
			else
			    return 1;
		}
		else //允许模式
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
// 函数名: 	CSocketImpl::CSocketImpl
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    构造函数
// 输入:
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
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
// 函数名: 	CSocketImpl::~CSocketImpl
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
CSocketImpl::~CSocketImpl()
{
    m_pCommTask = 0;
}


/******************************************************
// 函数名:  CSocketImpl::RemoveHandler
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    从ACE_Reactor中注销事件
// 输入:
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CSocketImpl::RemoveHandler()
{
    //将本对象列入CommTask对象的待删除列表，让CommTask在
    //下一个事件循环中再回调handle_close()函数，这样做的
    //原因是UNIX与NT在handle_close()函数的回调方式上有所
    //不同，在NT下是异步回调，即在下一个事件循环中再调。
    //而在UNIX下则不同，当注销本对象事件后，如果不加DONT_CALL
    //，则ACE会同步回调handle_close()函数，造成死锁。
    if(m_pCommTask)
    {
        m_pCommTask->ScheduleRemove(this);
    }

    m_bFailure = TRUE;
}



/******************************************************
// 函数名: 	CSocketImpl::SOCK_handle_output
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    向子类提供实际处理SOCKET上的输出事件的虚函数接口
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
int CSocketImpl::SOCK_handle_output(void)
{ 
    return 0;
}


/******************************************************
// 函数名: 	CSocketImpl::SOCK_handle_close
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    向子类提供实际处理SOCKET上的关闭事件的虚函数接口
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
int CSocketImpl::SOCK_handle_close(void)
{ 
    return 0;
}


/******************************************************
// 函数名:  CSocketImpl::handle_input
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    当ACE_Reactor对象发现某个CSocketImpl对象上有
//          SOCKET输入事件，则调用该对象的本函数
// 输入:
//       参数1 :ACE_HANDLE fd = ACE_INVALID_HANDLE
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CSocketImpl::handle_input(ACE_HANDLE fd)
{
    //返回值
    int nRetVal = 0;

    //复位超时计数器
    m_lTimeNoData = 0;

    //申请互斥量的使用权
    CCommMutex::Instance()->Acquire();

    if (!m_bFailure)
    {
        //处理SOCKET上的输入事件
        //子类重载后的SOCK_handle_input函数完成对输入事件的实际处理
        nRetVal = SOCK_handle_input();
        if(nRetVal != ERR_SUCCESS)
        {
            RemoveHandler();
        }
    }

    //释放互斥量的使用权
    CCommMutex::Instance()->Release();

    return 0;
}


/******************************************************
// 函数名:  CSocketImpl::handle_output
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    当ACE_Reactor对象发现某个CSocketImpl对象上有
//          SOCKET输出事件，则调用该对象的本函数
// 输入:
//       参数1 :ACE_HANDLE fd
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CSocketImpl::handle_output(ACE_HANDLE fd)
{
    //返回值
    int nRetVal = 0;

    //复位超时计数器
    m_lTimeNoData = 0;

    //申请互斥量的使用权
    CCommMutex::Instance()->Acquire();

    reactor()->cancel_wakeup(this, ACE_Event_Handler::WRITE_MASK);

    if(!m_bFailure)
    {
        //处理SOCKET上的输入事件
        //子类重载后的SOCK_handle_output函数完成对输入事件的实际处理
        nRetVal = SOCK_handle_output();
        if(nRetVal != ERR_SUCCESS)
        {
            RemoveHandler();
        }
    }

    //释放互斥量的使用权
    CCommMutex::Instance()->Release();

    return 0;
}


/******************************************************
// 函数名:  CSocketImpl::handle_close
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    当ACE_Reactor对象发现某个CSocketImpl对象上有
//          SOCKET关闭事件，则调用该对象的本函数
// 输入:
//       参数1 :ACE_HANDLE handle
//       参数2 :ACE_Reactor_Mask close_mask
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CSocketImpl::handle_close(ACE_HANDLE handle,
                              ACE_Reactor_Mask close_mask)
{
    //返回值
    int nRetVal = 0;

    //复位超时计数器
    m_lTimeNoData = 0;

    //申请互斥量的使用权
    CCommMutex::Instance()->Acquire();

    //处理SOCKET上的输入事件
    //子类重载后的SOCK_handle_close 函数完成对关闭事件的实际处理
    nRetVal = SOCK_handle_close();

    //释放互斥量的使用权
    CCommMutex::Instance()->Release();

    //删除本对象
    //这意味着SOCKET被关闭时，使用者不必显式地删除CSocketImpl子类对象
    delete this;

    return nRetVal;
}


/******************************************************
// 函数名:  CFrmSocket::CFrmSocket
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    构造函数
// 输入:
//       参数1 :ACE_SOCK_Stream* pSocketStream
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CFrmSocket::CFrmSocket(ACE_SOCK_Stream* pSocketStream)
{
    ASSERT(pSocketStream != 0);

    m_pSocketStream = pSocketStream;

    //创建一个长度为m_lRecvBufSize的内存区，并将该内存区
    //的指针赋值给m_pRecvBuf
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
// 函数名:  CFrmSocket::~CFrmSocket
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
CFrmSocket::~CFrmSocket()
{
    //完成清理工作
    Clear();
}

/******************************************************
// 函数名:  CFrmSocket::Clear
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    完成清理工作
// 输入:
//       参数1 :void
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CFrmSocket::Clear(void)
{
    //关闭SOCKET，并删除*m_pSocketStream对象
    if (m_pSocketStream != 0)
    {
        m_pSocketStream->close();
        delete m_pSocketStream;
        m_pSocketStream = 0;
    }

    //释放接收缓冲区
    if (m_pRecvBuf != 0)
    {
        delete [] m_pRecvBuf;
        m_pRecvBuf = 0;
    }

    //释放接收队列
    while (m_InFrames.size() > 0)
    {
        char* pFrame = m_InFrames.front();
        m_InFrames.pop_front();
        delete pFrame;
    }

    //释放发送缓冲区
    if (m_pSendFrame != 0)
    {
        delete m_pSendFrame;
        m_pSendFrame = 0;
    }

    //释放发送队列
    while (m_OutFrames.size() > 0)
    {
        char* pFrame = m_OutFrames.front();
        m_OutFrames.pop_front();
        delete pFrame;
    }
}

/******************************************************
// 函数名:  CFrmSocket::get_handle
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    ACE_Reactor获得产生SOCKET事件句柄的回调函数
//          当CFrmSocket对象被注册到ACE_Reactor对象后，
//          ACE_Reactor对象会回调CFrmSocket对象的get_handle
//          函数，从而获得产生SOCKET事件的句柄；本函数是对
//          ACE_Event_Handler提供的虚函数get_handle的重载
// 输入:
//       参数1 :void
// 返回值: ACE_HANDLE 产生SOCKET事件的ACE_SOCK_Stream流的句柄
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
ACE_HANDLE CFrmSocket::get_handle(void) const
{
    ASSERT(m_pSocketStream != 0);

    //返回*m_SocketStream 对象中的句柄
    return m_pSocketStream->get_handle();
}


/******************************************************
// 函数名:  CFrmSocket::SOCK_handle_input
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    处理SOCKET输入事件
//          本函数是对CSocketImpl提供的虚函数SOCK_handle_input
//          的重载；在本函数中，首先将数据接收到缓冲区中，
//          然后从缓冲区中提取数据帧，并将数据帧插入到接
//          收队列中，最后接收队列中已经接收完整的数据帧
//          提交给上层处理
// 输入:
//       参数1 :void
// 返回值: int 0——成功；
//            -1——失败，这将导致注销本对象
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CFrmSocket::SOCK_handle_input(void)
{
    //接收数据，超时为零
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
            //出错或对端已关闭Socket连接
            TRACE(MTS_COMM, S_COMM_RECV_TCPIP_PACKAGE, 
                  m_addrRemote.get_host_addr(), 
                  m_addrRemote.get_port_number(), nErr, 
                  ACE_OS::strerror(nErr));
            return -1;
        }
    }

    //从接收缓冲区中提取数据帧，并将提取出的数据帧的指针插入
    //到接收队列的队尾
    ReceiveFrames();

    //向上层提交数据
    int nSize;
    while ((nSize = m_InFrames.size()) > 0)
    {
        if((1 == nSize) && (FALSE == m_bLastOK))
        //如果最后一帧数据未接收完毕，则退出
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
// 函数名: 	CFrmSocket::GetAddrRemote
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    获得对端地址
// 输入:
// 返回值: ACE_INET_Addr& 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
ACE_INET_Addr& CFrmSocket::GetAddrRemote()
{
    return m_addrRemote;
}


/******************************************************
// 函数名: 	CFrmSocket::GetSockStream
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    ACE_SOCK_Stream封装了以流方式接收数据的SOCKET
// 输入:
// 返回值: ACE_SOCK_Stream* 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
ACE_SOCK_Stream* CFrmSocket::GetSockStream()
{
    return m_pSocketStream;
}


/******************************************************
// 函数名:  CFrmSocket::OnReceiveFrame
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    向上层提交一帧数据
// 输入:
//       参数1 :void* pAFrame
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CFrmSocket::OnReceiveFrame(void* pAFrame)
{
    ASSERT(pAFrame != 0);
    delete pAFrame;
}

/******************************************************
// 函数名:  CFrmSocket::Send
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    发送数据帧
// 输入:
//       参数1 :const void* const pAFrame
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
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
        //获得当前数据帧的长度
        long lFrameLen = GetFrameSize((char*)pAFrame);
        //创建一个长度为当前数据帧长度的内存区
        char* pBuf = new char[lFrameLen];

        //调用ACE_OS::memcpy函数，将pAFrame指向的数据拷
        //贝到新创建的内存区中
        ACE_OS::memcpy(pBuf, pAFrame, lFrameLen);

        //将新创建的内存区的指针插入到发送队列的队尾
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
// 函数名:  CFrmSocket::SOCK_handle_output
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    处理SOCKET输出事件
// 输入:
//       参数1 :void
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CFrmSocket::SOCK_handle_output(void)
{
    return ProcessPendingWrite();
}

/******************************************************
// 函数名:  CFrmSocket::ProcessPendingWrite
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    以FIFO方式将发送队列中的数据帧发送出去，
//          直到SOCKET被阻塞
// 输入:
//       参数1 :void
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
int CFrmSocket::ProcessPendingWrite(void)
{
    while ((m_pSendFrame != 0) || (m_OutFrames.size() > 0))
    {
        if (m_pSendFrame != 0)
        {
            //调用虚函数GetFrameSize,获得m_pSendFrame指向
            //的数据帧的长度
            long lFrameLen = GetFrameSize(m_pSendFrame);
            //计算出m_pSendFrame指向的数据帧中待发送字节数
            long lLeftLen = lFrameLen - m_lBytesSent;
            //计算出m_pSendFrame指向的数据帧中待发送的第一
            //个字节的位置
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
                    case EWOULDBLOCK:   //发送数据时SOCKET被阻塞
                    case ETIME:         //发送数据超时
                        m_pCommTask->ScheduleWakeup(this);
                        return 0;

                    default:            //发送数据时产生了错误，
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
                    //重新计算出m_pSendFrame指向的数据帧中待发送字节数；
                    m_lBytesSent += lWrittenLen;
                    lLeftLen -= lWrittenLen;
                    //重新计算出m_pSendFrame指向的数据帧中待发送的第一
                    //个字节的位置；
                    pBytesLeft += lWrittenLen;

                    //复位超时计数器
                    m_lTimeNoData = 0;
                }
            }
            //删除m_pSendFrame指向的数据帧
            delete m_pSendFrame;
            m_pSendFrame = NULL;
            m_lBytesSent = 0;
        }

        //获得发送队列的头元素指向的数据帧
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
// 函数名:  CFrmSocket::SOCK_handle_close
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    处理SOCKET关闭事件
// 输入:
//       参数1 :void
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
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
// 函数名:  CBinFrmSocket::CBinFrmSocket
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    构造函数
// 输入:
//       参数1 :ACE_SOCK_Stream* pSocketStream
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
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
// 函数名:  CBinFrmSocket::~CBinFrmSocket
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
CBinFrmSocket::~CBinFrmSocket()
{
    //释放临时存放当前正在接收的二进制帧的帧头数据
    if (m_pFrameHead != 0)
    {
        delete [] m_pFrameHead;
        m_pFrameHead = 0;
    }
}

/******************************************************
// 函数名:  CBinFrmSocket::ReceiveFrames
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    从接收缓冲区中提取二进制帧，并将所提取出来的
//          二进制帧的指针插入到接收队列的队尾
// 输入:
//       参数1 :void
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CBinFrmSocket::ReceiveFrames(void)
{
    //初始化一个指针，该指针指向接收缓冲区中可用于提取二进制
    //帧的第一个字节
    char* pValidBuf = m_pRecvBuf;
    //需要拷贝的字节数
    long lBytesNeeded = 0;
    //实际拷贝的字节数
    long lBytesCopied = 0;
    char* pLastFrame = 0;

    while (m_lBytesReceived > 0)
    {
        pLastFrame = 0;

        //by ldf 2002-12-09 对应问题单 D19623
        if (m_lLastFrameLen >= m_lHeadLen)
        //当收到的数据大于帧头长度时，数据放在接收队列中的尾部
        //此时m_pFrameHead中仍保留了当前帧的帧头数据
        {
            ASSERT(FALSE == m_bLastOK);

            //获得接收队列的队尾元素
            pLastFrame = m_InFrames.back();
        }
        else if (m_lLastFrameLen < m_lHeadLen)
        //先接收完帧头数据，放到m_pFrameHead中
        //当收到的数据小于等于帧头长度时，数据放在m_pFrameHead中
        {
            m_bLastOK = FALSE;

            //计算为了完整地接收帧头而需要拷贝的字节数
            lBytesNeeded = m_lHeadLen - m_lLastFrameLen;

            //计算为了完整地接收帧头而实际可拷贝的字节数
            lBytesCopied = min(lBytesNeeded, m_lBytesReceived);

            ACE_OS::memcpy(m_pFrameHead + m_lLastFrameLen,
                           pValidBuf,
                           lBytesCopied);

            //调整接收缓冲区中可用字节的起始位置，已接收数及最后一帧的长度
            pValidBuf += lBytesCopied;
            m_lBytesReceived -= lBytesCopied;
            m_lLastFrameLen += lBytesCopied;

            //若接收缓冲区中的数据已经被处理完，则结束本次帧的提取工作
            if ((m_lBytesReceived == 0) && (m_lLastFrameLen < m_lHeadLen))
            {
                return;
            }
        }
        //修改完毕

        /*——继续接收上次没有接收完整的帧的数据区——*/
        /*在此，肯定已收到超过帧头长度的数据          */
        /*即m_lLastFrameLen>=m_lHeadLen               */
        /*且m_pFrameHead已肯定存放了当前帧的帧头数据  */

        //获取当前帧的长度
        long lFrameSize = GetFrameSize(m_pFrameHead);

        //如果收到超大帧，则注销本Socket事件，并退出
        if(lFrameSize > MSG_INTRA_MAXSIZE - MSG_INTRA_HEADLENGTH)
        {
            TRACE(MTS_COMM, S_COMM_RECV_EXCEED_FRAME, 
                  m_addrRemote.get_host_addr(), 
                  m_addrRemote.get_port_number(),
                  lFrameSize);
            RemoveHandler();
            return;
        }

        //当收刚好收满一个帧头时，为当前帧创建一个内存区域
        if(pLastFrame == 0)
        {
            ASSERT(m_lLastFrameLen == m_lHeadLen);

            pLastFrame = new char[lFrameSize];
            ASSERT(pLastFrame != 0);
            ACE_OS::memcpy(pLastFrame, m_pFrameHead, m_lHeadLen);

            //将当前帧的指针插入到接收队列的队尾
            m_InFrames.push_back(pLastFrame);

            m_bLastOK = FALSE;
        }

        //计算为接收当前帧的数据区而需要的字节数
        lBytesNeeded = lFrameSize - m_lLastFrameLen;

        //计算为接收当前帧的数据区而实际可拷贝的字节数
        lBytesCopied = min(lBytesNeeded, m_lBytesReceived);

        //将从pValidBuf开始的lBytesCopied个字节追加到pLastFrame指向的缓冲区中
        ACE_OS::memcpy(pLastFrame + m_lLastFrameLen,
                        pValidBuf,
                        lBytesCopied);

        //调整接收缓冲区中可用字节的起始位置，已接收数及最后一帧的长度
        pValidBuf += lBytesCopied;
        m_lBytesReceived -= lBytesCopied;
        m_lLastFrameLen += lBytesCopied;

        //判断是否完整地接收了当前帧
        if (lBytesNeeded == lBytesCopied)
        {
            m_bLastOK = TRUE;
            m_lLastFrameLen = 0;
        }
    }
}

/******************************************************
// 函数名:  CBinFrmSocket::GetFrameSize
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    获取数据帧的长度
// 输入:
//       参数1 :const char* const pAFrame
// 返回值: long
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
long CBinFrmSocket::GetFrameSize(const char* const pAFrame)
{
    //帧头的长度m_lHeadLen加上数据区的长度；
    return GetDataSize(pAFrame) + m_lHeadLen;
}

/******************************************************
// 函数名:  CMemFrmSocket::CMemFrmSocket
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    构造函数
// 输入:
//       参数1 :ACE_SOCK_Stream* pSocketStream
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CMemFrmSocket::CMemFrmSocket(ACE_SOCK_Stream* pSocketStream)
                             :CBinFrmSocket(pSocketStream)
{
    //初始化帧头的长度
    m_lHeadLen = _MEMFRAME_HEAD_LENGTH;

    //创建临时存放帧头数据的缓冲区
    m_pFrameHead = new char[m_lHeadLen];

    m_bPrimary   = FALSE;
    m_nModuleNum = APP_MID_NULL;
}


/******************************************************
// 函数名: 	CMemFrmSocket::~CMemFrmSocket
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
CMemFrmSocket::~CMemFrmSocket()
{
    //
}


/******************************************************
// 函数名: 	CMemFrmSocket::GetbPrimary
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    获得是否是主端口的信息
// 输入:
// 返回值: BOOL 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
BOOL CMemFrmSocket::GetbPrimary()
{
    return m_bPrimary;
}


/******************************************************
// 函数名: 	CMemFrmSocket::SetbPrimary
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    设置是否是主端口的信息
// 输入:
//       参数1 :BOOL bPrimary
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
void CMemFrmSocket::SetbPrimary(BOOL bPrimary)
{
    m_bPrimary = bPrimary;
}


/******************************************************
// 函数名: 	CMemFrmSocket::GetModuleNum
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    获得主机模块号
// 输入:
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
int CMemFrmSocket::GetModuleNum()
{
    return m_nModuleNum;
}


/******************************************************
// 函数名: 	CMemFrmSocket::SetModuleNum
// 作者:    Wangfeng
// 时期:    01-10-20
// 描述:    设置主机模块号
// 输入:
//       参数1 :int nNum
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-20      Wangfeng
******************************************************/
void CMemFrmSocket::SetModuleNum(int nNum)
{
    m_nModuleNum = nNum;
}


/******************************************************
// 函数名:  CMemFrmSocket::GetDataSize
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    获得一帧SMemFrame实例的数据区的长度
// 输入:
//       参数1 :const char* const pAFrame
// 返回值: long
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
long CMemFrmSocket::GetDataSize(const char* const pAFrame)
{
    return ((SMemFrame*)pAFrame)->nLen();
}


/******************************************************
// 函数名:  CMemFrmSocket::OnReceiveFrame
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    向管理本对象的CSlideWndCommRouter对象提交数据
// 输入:
//       参数1 :void* pAFrame
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
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

    //删除该数据帧
    delete pAFrame;
}


//颜文远2002.04.08添加
//构造函数
CSysFrmSocket::CSysFrmSocket(ACE_SOCK_Stream* pSocketStream)
					: CMemFrmSocket(pSocketStream)
{
    //初始化帧头的长度
    m_lHeadLen = _SYSFRAME_HEAD_LENGTH;

    if(NULL != m_pFrameHead)
    {
        delete[] m_pFrameHead;
    }

    //创建临时存放帧头数据的缓冲区
    m_pFrameHead = new char[m_lHeadLen];
}

//析构函数
CSysFrmSocket::~CSysFrmSocket()
{
}

//获得一帧SSysFrame实例的数据区的长度
long CSysFrmSocket::GetDataSize(const char* const pAFrame)
{
    return ((SSysFrame* )pAFrame)->nLen();
}

//向管理本对象的CSoftXCommRouter对象提交数据
void CSysFrmSocket::OnReceiveFrame(void* pAFrame)
{
	if(m_pCommRouter != NULL)
	{
		m_pCommRouter->OnReceiveEx(this, pAFrame);
	}

	//删除该数据帧
	delete pAFrame;
}
//颜文远2002.04.08添加结束


/******************************************************
// 函数名:  CMsgFrmSocket::CMsgFrmSocket
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    构造函数
// 输入:
//       参数1 :ACE_SOCK_Stream* pSocketStream
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CMsgFrmSocket::CMsgFrmSocket(ACE_SOCK_Stream* pSocketStream)
                             :CBinFrmSocket(pSocketStream)
{
    //初始化帧头的长度
    m_lHeadLen = MSG_INTRA_HEADLENGTH;

    //创建临时存放帧头数据的缓冲区
    m_pFrameHead = new char[m_lHeadLen];
}


/******************************************************
// 函数名:  CMsgFrmSocket::~CMsgFrmSocket
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
CMsgFrmSocket::~CMsgFrmSocket()
{
    //
}


/******************************************************
// 函数名:  CMsgFrmSocket::GetDataSize
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    获得一帧MSG_INTRA实例的数据区的长度
// 输入:
//       参数1 :const char* const pAFrame
// 返回值: long
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
long CMsgFrmSocket::GetDataSize(const char* const pAFrame)
{
    return ((MSG_INTRA*)pAFrame)->Len;
}


/******************************************************
// 函数名:  CMsgFrmSocket::OnReceiveFrame
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    向管理本对象的CMsgSvrCommRouter对象或
//          CMsgCliCommRouter对象提交数据
// 输入:
//       参数1 :void* pAFrame
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
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
        //只在不能提交时删除该数据帧
        delete pAFrame;
    }
}


/******************************************************
// 函数名:  CMmlFrmSocket::CMmlFrmSocket
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    构造函数
// 输入:
//       参数1 :ACE_SOCK_Stream* pSocketStream
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CMmlFrmSocket::CMmlFrmSocket(ACE_SOCK_Stream* pSocketStream)
                             :CFrmSocket(pSocketStream)
{
    //
}


/******************************************************
// 函数名:  CMmlFrmSocket::~CMmlFrmSocket
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
CMmlFrmSocket::~CMmlFrmSocket()
{
    //
}


/******************************************************
// 函数名:  CMmlFrmSocket::AddTags
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    增加一个文本帧开始或结束的标志
// 输入:
//       参数1 :const char yTag——文本帧的开始或结束标志
//       参数2 :const BOOL bBeginTag——若等于TRUE，则表示
//              yTag为文本帧的开始标志，否则表示
//              yTag为文本帧的结束标志
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
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
// 函数名: 	CMmlFrmSocket::Send
// 作者:    Wangfeng
// 时期:    01-11-15
// 描述:    发送MML文件数据帧
// 输入:
//       参数1 :const void* const pAFrame
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
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
        //获得当前数据帧的长度
        long lFrameLen = GetFrameSize((char*)pAFrame);
        //创建一个长度为当前数据帧长度的内存区
        char* pBuf = new char[lFrameLen + 1];

        //调用ACE_OS::memcpy函数，将pAFrame指向的数据拷
        //贝到新创建的内存区中
        strcpy(pBuf, (char*)pAFrame);

        //将新创建的内存区的指针插入到发送队列的队尾
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
// 函数名:  CMmlFrmSocket::ReceiveFrames
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    从接收缓冲区中提取文本帧，并将所提取出来的文
//          本帧的指针插入到接收队列的队尾
// 输入:
//       参数1 :void
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CMmlFrmSocket::ReceiveFrames(void)
{
    //初始化一个指向接收缓冲区的指针
    char* pValidBuf = m_pRecvBuf;
    long lBytesNeed = 0;            //需要拷贝的字节数

    while (m_lBytesReceived > 0)
    {
        //过滤开始符号
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
            到此处，缓冲区内已存在一到多个不为开始符号的字符
        */

        //如果查找不到结束符号
        if (m_yEndTags.find(pValidBuf[lBytesNeed]) == m_yEndTags.end())
        {
            lBytesNeed++; 
            m_lBytesReceived--;
            if(m_lBytesReceived > 0) continue;
        }

        /*
            到此处，存在三种情况：
            1、当前缓冲区内只有一个结束符号，这种情况下只
               要将最后一帧置为完整即可
            2、当前缓冲区内已找到一个结束符号，这种情况下
               m_lBytesReceived > 0
            3、当前缓冲区找不到结束符号，，这种情况下
               m_lBytesReceived == 0
        */

        //如果只收到一个结束符号，则置最后一帧为完整
        if(0 == lBytesNeed) 
        {
            m_bLastOK = TRUE;
            pValidBuf++;
            m_lBytesReceived--;
            continue;
        }

        //拼帧
        char* pBuf;
        if(!m_bLastOK)
        //上一次接收时，最后一帧没有被完整地接收
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

        //更新接收队列的尾项
        m_InFrames.push_back(pBuf);

        //更新缓冲区剩余字节数
        if(0 == m_lBytesReceived)
        //接收队列中不存在结束符号
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
// 函数名:  CMmlFrmSocket::GetFrameSize
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    获得数据帧长度
// 输入:
//       参数1 :const char* const pAFrame
// 返回值: long
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
long CMmlFrmSocket::GetFrameSize(const char* const pAFrame)
{
    return ACE_OS::strlen(pAFrame);
}


/******************************************************
// 函数名:  CMmlFrmSocket::OnReceiveFrame
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    向管理本对象的CMmlCommRouter对象提交数据
// 输入:
//       参数1 :void* pAFrame
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CMmlFrmSocket::OnReceiveFrame(void* pAFrame)
{
    if(m_pCommRouter != 0)
    {
        m_pCommRouter->OnReceiveEx(this, pAFrame);
    }

    //删除该数据帧
    delete pAFrame;
}


/******************************************************
// 函数名:  CGtpUdpSocket::CGtpUdpSocket
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    构造函数
// 输入:
//       参数1 :CGtpUdpCommRouter* const pGtpCommRouter
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CGtpUdpSocket::CGtpUdpSocket(CGtpUdpCommRouter* const pGtpCommRouter)
{
    //GTP'计费接口增加 Z30971 2004/06/10,注释掉ASSERT
    //ASSERT(pGtpCommRouter != 0);
    m_pGtpCommRouter = pGtpCommRouter;
    //创建接收缓冲区
    m_pRecvBuf = new char[GTP_INPUT_BUFFER_MAXLENGTH];
}


/******************************************************
// 函数名:  CGtpUdpSocket::~CGtpUdpSocket
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    析构函数
// 输入:
//       参数1 :void
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
CGtpUdpSocket::~CGtpUdpSocket(void)
{
    m_SocketDgram.close();

    //释放接收缓冲区
    if(m_pRecvBuf)
    {
        delete[] m_pRecvBuf;
        m_pRecvBuf = 0;
    }

    m_pGtpCommRouter = 0;
}


/******************************************************
// 函数名:  CGtpUdpSocket::Open
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    打开一个收发UDP数据报的端口
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
int CGtpUdpSocket::Open(const char* const szIPAddr, const int nPort)
{
	//设置IP地址和端口
    m_addrLocal.set(nPort, szIPAddr);

    return m_SocketDgram.open(m_addrLocal);
}


/******************************************************
// 函数名:  CGtpUdpSocket::get_handle
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    获得事件句柄
// 输入:
//       参数1 :void
// 返回值: ACE_HANDLE
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
ACE_HANDLE CGtpUdpSocket::get_handle(void) const
{
    //返回m_SocketDgram 对象中的句柄
    return m_SocketDgram.get_handle();
}


/******************************************************
// 函数名:  CGtpUdpSocket::SOCK_handle_input
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
int CGtpUdpSocket::SOCK_handle_input(void)
{
    ACE_INET_Addr AddrPeer;
    int nRetBytes = m_SocketDgram.recv(m_pRecvBuf,
                                GTP_INPUT_BUFFER_MAXLENGTH,
                                AddrPeer);
	//张玉新 2002-12-26 IP地址过滤
	CIPFilter *pFilter = ACE_Singleton<CIPFilter,ACE_Null_Mutex>::instance();
	UINT4 uIP = HTONL(AddrPeer.get_ip_address());

	//by ldf 2003-10-30 对应问题单 SWPD02155
	UINT4 uIPTmp = (UINT4)(AddrPeer.get_ip_address());
	if(pFilter->Filter(uIPTmp))
	{
		return 0;
	}
	//张玉新
	
    if (nRetBytes <= 0)
    {
        return 0;
    }

    if(m_pGtpCommRouter != 0)
    {
        //创建一个MSG_INTRA的实例，将接收到的数据报拷贝到该实例的Data域
        MSG_INTRA* pMsg   = new(nRetBytes) MSG_INTRA;
        pMsg->CmdCode     = CMD_HOST_DATA;
        pMsg->SenderPid   = PT_OUTSIDE;
        pMsg->SenderMid   = MT_UNKNOWN;
        pMsg->ReceiverPid = CMsgProcessor::GetProcessType();
        pMsg->ReceiverMid = MT_NETWORK;
        pMsg->AppType     = APP_GTP_UDP;
        ACE_OS::memcpy(pMsg->Data, m_pRecvBuf, nRetBytes);

        //设置该实例的SNetAddrPair域
        ((_NetFrame *) pMsg)->SNetAddrPair.ServerIp = HTONL(m_addrLocal.get_ip_address());
        ((_NetFrame *) pMsg)->SNetAddrPair.ServerPort = m_addrLocal.get_port_number();
        ((_NetFrame *) pMsg)->SNetAddrPair.ClientIp = uIP;
        ((_NetFrame *) pMsg)->SNetAddrPair.ClientPort = AddrPeer.get_port_number();

        m_pGtpCommRouter->OnReceive(pMsg);
    }
    return 0;
}


/******************************************************
// 函数名:  CGtpUdpSocket::SOCK_handle_close
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    处理SOCKET关闭事件
// 输入:
//       参数1 :void
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
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
// 函数名:  CGtpUdpSocket::GetAddr
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    获得本对象对应的IP地址和端口号
// 输入:
//       参数1 :unsigned int& nIPAddr
//       参数2 :int& nPort
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//01-10-6       Wangfeng
******************************************************/
void CGtpUdpSocket::GetAddr(unsigned int& nIPAddr, int& nPort)
{
    nIPAddr = HTONL(m_addrLocal.get_ip_address());
    nPort = m_addrLocal.get_port_number();
}


/******************************************************
// 函数名:  CGtpUdpSocket::Send
// 作者:    Wangfeng
// 时期:    01-10-6
// 描述:    发送一个UDP数据报
// 输入:
//       参数1 :MSG_INTRA* const pMsg
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
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
// 函数名:  CABFrmSocket::CABFrmSocket
// 作者:    Wangzhaoyang
// 时期:    02-07-15
// 描述:    构造函数
// 输入:
//       参数1 :ACE_SOCK_Stream* pSocketStream
// 返回值: 
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//02-07-15       Wangzhaoyang            创建
******************************************************/
CABFrmSocket::CABFrmSocket(ACE_SOCK_Stream* pSocketStream)
                             :CBinFrmSocket(pSocketStream)
{
    m_lHeadLen = _APPCVTFRM_HEAD_LENGTH;
    m_pFrameHead = new char[m_lHeadLen];
}

/******************************************************
// 函数名:  CABFrmSocket::~CABFrmSocket
// 作者:    Wangzhaoyang
// 时期:    02-07-15
// 描述:    析构函数
// 输入:
// 返回值: 
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//02-07-15       Wangzhaoyang            创建
******************************************************/
CABFrmSocket::~CABFrmSocket()
{

}

/******************************************************
// 函数名:  CABFrmSocket::GetDataSize
// 作者:    Wangzhaoyang
// 时期:    02-07-15
// 描述:    获得数据帧的data部分长度
// 输入:
//       参数1 :const char *const pAFrame
// 返回值: 
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//02-07-15       Wangzhaoyang            创建
******************************************************/
long CABFrmSocket::GetDataSize(const char *const pAFrame)
{
	return ntohs(((SAppConvertFrame*)pAFrame)->wLen);
}

/******************************************************
// 函数名:  CABFrmSocket::OnReceiveFrame
// 作者:    Wangzhaoyang
// 时期:    02-07-15
// 描述:    向管理本对象的CAlarmBoxRouter对象提交数据
// 输入:
//       参数1 :const char *const pAFrame
// 返回值: 
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//02-07-15       Wangzhaoyang            创建
******************************************************/
void CABFrmSocket::OnReceiveFrame(void* pAFrame)
{
	if(m_pCommRouter != NULL)
    {
        m_pCommRouter->OnReceiveEx(this, pAFrame);
    }

    //删除该数据帧
    delete pAFrame;
}

/******************************************************
// 函数名:  CGtpFrmSocket::CGtpFrmSocket
// 作者:    Wangfeng
// 时期:    03-06-28
// 描述:    构造函数
// 输入:
//       参数1 :ACE_SOCK_Stream* pSocketStream
// 返回值:
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//03-06-28       Wangfeng
******************************************************/
CGtpFrmSocket::CGtpFrmSocket(ACE_SOCK_Stream* pSocketStream)
                             :CBinFrmSocket(pSocketStream)
{
    //初始化帧头的长度
    m_lHeadLen = GTP_FRAME_HEAD_LENGTH_V2;

    //创建临时存放帧头数据的缓冲区
    m_pFrameHead = new char[m_lHeadLen];
}


/******************************************************
// 函数名: 	CGtpFrmSocket::~CGtpFrmSocket
// 作者:    Wangfeng
// 时期:    03-06-28
// 描述:    析构函数
// 输入:
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//03-06-28      Wangfeng
******************************************************/
CGtpFrmSocket::~CGtpFrmSocket()
{
    //
}


/******************************************************
// 函数名:  CGtpFrmSocket::GetDataSize
// 作者:    Wangfeng
// 时期:    03-06-28
// 描述:    获得一帧SGtpFrame实例的数据区的长度
// 输入:
//       参数1 :const char* const pAFrame
// 返回值: long
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//03-06-28       Wangfeng
******************************************************/
long CGtpFrmSocket::GetDataSize(const char* const pAFrame)
{
    return ((SGtpFrame*)pAFrame)->GetSize() - m_lHeadLen;
}


/******************************************************
// 函数名:  CGtpFrmSocket::OnReceiveFrame
// 作者:    Wangfeng
// 时期:    03-06-28
// 描述:    向管理本对象的CGtpFrmCommRouter对象提交数据
// 输入:
//       参数1 :void* pAFrame
// 返回值: void
// 注释:
// 更新:
// 日期            姓名                  描述
   ========== ================== ======================
//03-06-28       Wangfeng
******************************************************/
void CGtpFrmSocket::OnReceiveFrame(void* pAFrame)
{
	ASSERT(pAFrame != 0);
    if(m_pCommRouter != NULL)
    {
        //创建一个MSG_INTRA的实例，将接收到的数据报拷贝到该实例的Data域
        MSG_INTRA* pMsg   = new(((SGtpFrame*)pAFrame)->GetSize()) MSG_INTRA;
        pMsg->CmdCode     = CMD_HOST_DATA;
        pMsg->SenderPid   = PT_OUTSIDE;
        pMsg->SenderMid   = MT_UNKNOWN;
        pMsg->ReceiverPid = CMsgProcessor::GetProcessType();
        pMsg->ReceiverMid = MT_NETWORK;
        pMsg->AppType     = APP_GTP_TCP;
        ACE_OS::memcpy(pMsg->Data, pAFrame, pMsg->Len);

        //设置该实例的SNetAddrPair域
        ((_NetFrame *) pMsg)->SNetAddrPair.ServerIp     = HTONL(m_addrLocal.get_ip_address());
        ((_NetFrame *) pMsg)->SNetAddrPair.ServerPort   = m_addrLocal.get_port_number();
        ((_NetFrame *) pMsg)->SNetAddrPair.ClientIp     = HTONL(m_addrRemote.get_ip_address());
        ((_NetFrame *) pMsg)->SNetAddrPair.ClientPort   = m_addrRemote.get_port_number();

        m_pCommRouter->OnReceive(pMsg);
    }

    //删除该数据帧
    delete pAFrame;
}

/******************************************************
// 函数名:  CGtpFrmSocket::SOCK_handle_close
// 作者:    Wangfeng
// 时期:    03-06-28
// 描述:    处理SOCKET关闭事件
// 输入:
//       参数1 :void
// 返回值: int
// 注释:
// 更新:
// 日期            姓名                  描述
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
//GTP'计费接口增加 Z30971 2004/06/10

//GTP' UDP SOCKET构造函数
CGtpUdpCliSocket::CGtpUdpCliSocket(CGTPUdpCliCommRouter* const pGtpCliCommRouter)
                    :CGtpUdpSocket(NULL)  
         
{
    m_pCommRouter = pGtpCliCommRouter;
}
//GTP' UDP SOCKET析构函数
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

//处理SOCKET事件
int CGtpUdpCliSocket::SOCK_handle_input(void)
{
    ACE_INET_Addr AddrPeer;
    int nRetBytes = m_SocketDgram.recv(m_pRecvBuf,
                                GTP_INPUT_BUFFER_MAXLENGTH,
                                AddrPeer);
	
	//IP过滤
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
        //将数据发送到模块 MT_CDRSENDER
        MSG_INTRA* pMsg   = new(nRetBytes) MSG_INTRA;
        pMsg->CmdCode     = CMD_BS_DATA;
        pMsg->SenderPid   = PT_OUTSIDE;
        pMsg->SenderMid   = MT_UNKNOWN;
        pMsg->ReceiverPid = CMsgProcessor::GetProcessType();
        pMsg->ReceiverMid = MT_CDRSENDER; 
        pMsg->AppType     = APP_BS_GTP_UDP;
        ACE_OS::memcpy(pMsg->Data, m_pRecvBuf, nRetBytes);

        //设置该实例的SNetAddrPair域
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


//GTP' TCP SOCKET构造函数
CGtpTcpCliSocket::CGtpTcpCliSocket(ACE_SOCK_Stream* pSocketStream)
                   : CGtpFrmSocket(pSocketStream)
{
    m_pSocketStream->get_local_addr(m_addrLocal);
    m_pSocketStream->get_remote_addr(m_addrRemote);
}
//GTP' TCP SOCKET析构函数
CGtpTcpCliSocket::~CGtpTcpCliSocket()
{
}

//接收数据帧
void CGtpTcpCliSocket::OnReceiveFrame(void* pAFrame)
{
    ASSERT(pAFrame != 0);
    if(m_pCommRouter != NULL)
    {
        //创建一个MSG_INTRA的实例，将接收到的数据报拷贝到该实例的Data域
        MSG_INTRA* pMsg   = new(((SGtpFrame*)pAFrame)->GetSize()) MSG_INTRA;
        pMsg->CmdCode     = CMD_BS_DATA;
        pMsg->SenderPid   = PT_OUTSIDE;
        pMsg->SenderMid   = MT_UNKNOWN;
        pMsg->ReceiverPid = CMsgProcessor::GetProcessType();
        pMsg->ReceiverMid = MT_CDRSENDER;
        pMsg->AppType     = APP_BS_GTP_TCP;
        ACE_OS::memcpy(pMsg->Data, pAFrame, pMsg->Len);

        //设置该实例的SNetAddrPair域
        ((_NetFrame *) pMsg)->SNetAddrPair.ServerIp     = HTONL(m_addrRemote.get_ip_address());
        ((_NetFrame *) pMsg)->SNetAddrPair.ServerPort   = m_addrRemote.get_port_number();
        ((_NetFrame *) pMsg)->SNetAddrPair.ClientIp     = HTONL(m_addrLocal.get_ip_address());
        ((_NetFrame *) pMsg)->SNetAddrPair.ClientPort   = m_addrLocal.get_port_number();

        m_pCommRouter->OnReceive(pMsg);
    }

    //删除该数据帧
    delete pAFrame;
}
