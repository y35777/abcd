#ifndef __CLUSTER_NETWORK_H__
#define __CLUSTER_NETWORK_H__

#include "cluster_struct.h"

class CHeartBeatLink;
class CComCluster;
class CClusterCommTask;

class CClusterNetwork
{
/*
    成员函数
*/
public:
    CClusterNetwork(CComCluster* pCluster);
    virtual ~CClusterNetwork();

    //初始化函数
    int InitInstance();
    //向对端发送消息
    int Send(SClusterFrame* pFrame);
    //处理对端发来的消息
    virtual void OnReceive(SClusterFrame* pFrame);
    //获取总的心跳数
    int GetLinkCount();
    //获取当前可用的心跳
    int GetActiveLinkCount();
    //将ACE_Event_Handler子类的对象注册到CCommTask对象的ACE_Reactor对象中
    int RegisterHandle(ACE_Event_Handler* pSocket, ACE_Reactor_Mask mask);
    //秒定时器
    void Timer1Sec();
    //双机事件信息处理函数
    virtual void OnNotify(ECLUSTER_NOTIFY uMsg,
                          UINT4 uParam1 = 0,
                          UINT4 uParam2 = 0);

protected:
    //进行CClusterNetwork类退出前的扫尾工作
    void ExitInstance();
    //停止所有通讯子线程
    void StopCommThread();
    //心跳检测线程函数
    static void ThreadProc(CClusterNetwork* pNetwork);
    //通过发握手帧，检测所有心跳的可用性
    void TestLink();

/*
    成员变量
*/
protected:
    //心跳对象列表的指针
    LIST <CHeartBeatLink*>* m_pHeartBeatList;

    //Cluster对象的指针
    CComCluster*            m_pCluster;

    //CCommTask对象的指针
    CClusterCommTask*       m_pCommTask;

    //ACE的线程管理对象
    ACE_Thread_Manager      m_ThrdMgr;

    //心跳检测线程退出事件
    ACE_Event               m_ExitEvent;

    //消息发送时用的互斥锁
    ACE_Mutex               m_SendMutex;
};

#endif //__CLUSTER_NETWORK_H__
