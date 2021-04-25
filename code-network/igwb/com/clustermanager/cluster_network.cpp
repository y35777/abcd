#include "../include/frame.h"
#include "com_cluster.h"
#include "cluster_network.h"
#include "heartbeat_link.h"
#include "cluster_mutex.h"
#include "cluster_commtask.h"
#include "heartbeat_creator.h"

/********************************************************
* 名称: CClusterNetwork::CClusterNetwork
* 功能: 构造函数
* 输入: CComCluster* pCluster : CComCluster对象的指针
* 输出:
* 返回:
********************************************************/
CClusterNetwork::CClusterNetwork(CComCluster* pCluster)
{
    assert(NULL != pCluster);

    m_pHeartBeatList    = NULL;
    m_pCluster          = pCluster;
    m_pCommTask         = NULL;
}

/********************************************************
* 名称: CClusterNetwork::~CClusterNetwork
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CClusterNetwork::~CClusterNetwork()
{
    ExitInstance();
}

/********************************************************
* 名称: CClusterNetwork::InitInstance
* 功能: 初始化函数
* 输入:
* 输出:
* 返回: int 返回初始化是否成功，失败返回非零值，成功返回0
********************************************************/
int CClusterNetwork::InitInstance()
{
    m_pCommTask = new CClusterCommTask(this);

    if(!m_pCluster->GetbSingleServer())
    {//双机模式

        //创建并初始化所有心跳对象
        m_pHeartBeatList = CHeartBeatCreator::CreateHeartBeatList(this);
        if(NULL == m_pHeartBeatList)
        {
            return ERR_HEARTBEAT_CREATE;
        }
    }

    //激活m_pCommTask对象
    int nRet = m_pCommTask->open();
    if(ERR_SUCCESS != nRet)
    {
        return ERR_COMMTASK_OPEN;
    }

    if(!m_pCluster->GetbSingleServer())
    {//双机模式

        //启动心跳检测线程。
        nRet = m_ThrdMgr.spawn((ACE_THR_FUNC)ThreadProc, (void *)this);
        if(-1 == nRet)
        {
            return ERR_HB_THREAD_SPAWN;
        }
    }

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CClusterNetwork::ExitInstance
* 功能: 进行CClusterNetwork类退出前的扫尾工作
* 输入:
* 输出:
* 返回: void
********************************************************/
void CClusterNetwork::ExitInstance()
{
    //使所有通讯子线程退出
    StopCommThread();

    if(NULL != m_pCommTask)
    {
        delete m_pCommTask;
        m_pCommTask = NULL;
    }

    if(NULL != m_pHeartBeatList)
    {
        LIST <CHeartBeatLink*>::iterator i;
        for(i = m_pHeartBeatList->begin();
            i != m_pHeartBeatList->end();
            i++)
        {
            delete *i;
        }
        delete m_pHeartBeatList;
        m_pHeartBeatList = NULL;
    }

    m_pCluster = NULL;
}

/********************************************************
* 名称: CClusterNetwork::StopCommThread
* 功能: 停止所有通讯子线程
* 输入:
* 输出:
* 返回: void
********************************************************/
void CClusterNetwork::StopCommThread()
{
    m_ExitEvent.signal();

    //其中timeout为绝对时间，等待心跳检测线程退出
    ACE_Time_Value timeout = ACE_OS::gettimeofday();
    timeout += ACE_Time_Value(10);
    m_ThrdMgr.wait(&timeout);

    if(NULL != m_pCommTask)
    {
        //等待CCommTask的事件检测线程退出
        m_pCommTask->QuitCommTask();
    }
}

/********************************************************
* 名称: CClusterNetwork::Send
* 功能: 向对端发送消息
* 输入: SClusterFrame* pFrame : 双机消息帧结构的指针
* 输出:
* 返回: int 发送是否成功，0表示成功，非零值表示失败
********************************************************/
int CClusterNetwork::Send(SClusterFrame* pFrame)
{
    //pFrame长度大于最大发送帧长度
    if(pFrame->nLen > CLUSTER_FRAME_MAXSIZE - CLUSTER_FRAME_HEADLENGTH)
    {
        return ERR_SEND_LENGTH_EXCCEED;
    }

    if(NULL != m_pHeartBeatList)
    {
        //在m_pHeartBeatList中查找第一个指针不为空，
        //且其GetbIsAvailable()返回为TRUE的心跳对象;
        CHeartBeatLink* pLink = NULL;
        LIST <CHeartBeatLink*>::iterator i;
        for(i = m_pHeartBeatList->begin();
            i != m_pHeartBeatList->end();
            i++)
        {
            pLink = *i;
            if((NULL != pLink) && (pLink->GetbIsAvailable()))
            {
                pFrame->FillCheckSum();
                m_SendMutex.acquire();
                int nRet = pLink->Send(pFrame);
                m_SendMutex.release();

                return nRet;
            }
        }
    }

    return ERR_NO_AVAILABLE_HB;
}

/********************************************************
* 名称: CClusterNetwork::OnReceive
* 功能: 处理对端发来的消息
* 输入: SClusterFrame* pFrame : 双机消息帧结构的指针
* 输出:
* 返回: void
********************************************************/
void CClusterNetwork::OnReceive(SClusterFrame* pFrame)
{
    if(NULL != m_pCluster)
    {
        //向双机对象提交对端发来的消息
        m_pCluster->OnReceive(pFrame);
    }
}

/********************************************************
* 名称: CClusterNetwork::GetLinkCount
* 功能: 获取总的心跳数
* 输入:
* 输出:
* 返回: int 总的心跳数
********************************************************/
int CClusterNetwork::GetLinkCount()
{
    int nLinkCount = 0;
    if(NULL != m_pHeartBeatList)
    {
        nLinkCount = m_pHeartBeatList->size();
    }

    return nLinkCount;
}

/********************************************************
* 名称: CClusterNetwork::GetActiveLinkCount
* 功能: 获取当前可用的心跳
* 输入:
* 输出:
* 返回: int 当前可用的心跳数
********************************************************/
int CClusterNetwork::GetActiveLinkCount()
{
    int nLinkCount = 0;

    if(NULL != m_pHeartBeatList)
    {
        //在m_pHeartBeatList中查找指针不为空，且其GetbIsAvailable()
        //返回为TRUE的心跳对象，置nLinkCount为满足条件的心跳数;
        LIST <CHeartBeatLink*>::iterator i;
        for(i = m_pHeartBeatList->begin();
            i != m_pHeartBeatList->end();
            i++)
        {
            if((NULL != *i) && (*i)->GetbIsAvailable())
            {
                nLinkCount++;
            }
        }
    }

    return nLinkCount;
}

/********************************************************
* 名称: CClusterNetwork::ThreadProc
* 功能: 心跳检测线程函数
* 输入: CClusterNetwork* pNetwork : CClusterNetwork对象的指针
* 输出:
* 返回: void
********************************************************/
void CClusterNetwork::ThreadProc(CClusterNetwork* pNetwork)
{
    pNetwork->TestLink();
}

/********************************************************
* 名称: CClusterNetwork::TestLink
* 功能: 通过发握手帧，检测所有心跳的可用性
* 输入:
* 输出:
* 返回: void
********************************************************/
void CClusterNetwork::TestLink()
{
    if((NULL == m_pHeartBeatList) || (m_pHeartBeatList->size() == 0))
    {
        return;
    }

    SClusterFrame* pFrame  = new SClusterFrame;
    pFrame->nCmdCode       = CMD_CLS_SHAKEHANDS;
    pFrame->FillCheckSum();
    ACE_Time_Value abstime = ACE_OS::gettimeofday();
    abstime               += ACE_Time_Value(1);

    while(m_ExitEvent.wait(&abstime) == -1)
    //表示超时
    {
        m_SendMutex.acquire();
        //依次调用m_pHeartBeatList中所有心跳对象的Send函数，发送握手帧;
        LIST <CHeartBeatLink*>::iterator i;
        for(i = m_pHeartBeatList->begin();
            i != m_pHeartBeatList->end();
            i++)
        {
            (*i)->Send(pFrame);
        }
        m_SendMutex.release();
        abstime  = ACE_OS::gettimeofday();
        abstime += ACE_Time_Value(1);
    }

    delete pFrame;
}

/********************************************************
* 名称: CClusterNetwork::RegisterHandle
* 功能: 将ACE_Event_Handler子类的对象注册到CCommTask对象的
*       ACE_Reactor对象中
* 输入: ACE_Event_Handler* pSocket : 待注册的Socket对象的指针
*       ACE_Reactor_Mask mask      : 待注册的事件
* 输出:
* 返回: int 注册是否成功，0表示成功，非零值表示错误码
********************************************************/
int CClusterNetwork::RegisterHandle(ACE_Event_Handler* pSocket,
                                    ACE_Reactor_Mask mask)
{
    int nRet = ERR_FAIL;

    //调用m_pCommTask对象的RegisterHandle接口注册Socket对象的事件
    if(NULL != m_pCommTask)
    {
        nRet = m_pCommTask->RegisterHandle(pSocket, mask);
    }

    return nRet;
}

/********************************************************
* 名称: CClusterNetwork::Timer1Sec
* 功能: 秒定时器
* 输入:
* 输出:
* 返回: void
********************************************************/
void CClusterNetwork::Timer1Sec()
{
    CClusterMutex::Instance()->Acquire();

    BOOL bAvailable;

    //判断心跳链路是否中断
    if(NULL != m_pHeartBeatList)
    {
        CHeartBeatLink* pLink = NULL;
        LIST <CHeartBeatLink*>::iterator i;

        for(i = m_pHeartBeatList->begin();
            i != m_pHeartBeatList->end();
            i++)
        {
            pLink = (*i);
            bAvailable = pLink->GetbIsAvailable();
            pLink->Timer1Sec();

            if ((bAvailable) && (bAvailable != pLink->GetbIsAvailable()))
            {
                OnNotify(NOTIFY_HEARTLINK_BROKEN, (UINT4)pLink);
            }
        }
    }

    //触发CComCluster的秒事件处理函数
    if (NULL != m_pCluster)
    {
        m_pCluster->Timer1Sec();
    }

    CClusterMutex::Instance()->Release();
}

/********************************************************
* 名称: CClusterNetwork::OnNotify
* 功能: 双机事件信息处理函数
* 输入: ECLUSTER_NOTIFY uMsg : 事件信息码
*       UINT4 uParam1        : 参数一，可能是故障对象的指针
*       UINT4 uParam2        : 参数二，保留参数
* 输出:
* 返回:
********************************************************/
void CClusterNetwork::OnNotify(ECLUSTER_NOTIFY uMsg,
                               UINT4 uParam1,
                               UINT4 uParam2)
{
    if(NULL != m_pCluster)
    {
        m_pCluster->OnNotify(uMsg, uParam1, uParam2);
    }
}
