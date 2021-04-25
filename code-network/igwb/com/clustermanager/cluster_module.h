#ifndef __CLUSTER_MODULE_H__
#define __CLUSTER_MODULE_H__

class CWorkModule;
class CAsynResKernel;
class CComCluster;
class CShellShow;
class CSyncManager;
struct SClusterFrame;
struct SSyncFrame;

class CClusterModule : public CWorkModule
{
/*
    成员函数
*/
public:

    CClusterModule();
    virtual ~CClusterModule();

    //获取状态同步数据
    int GetSyncData(void*& pData, int& nLen, BOOL bReadFile=TRUE);
    //实施状态数据的同步操作
    void OnDoSyncData(void* pData, int nLen);
    //激活操作接口
    int Active();
    //去激活操作接口
    int DeActive();

protected:

    //初始化操作
    virtual int InitInstance();
    //退出前的清理操作
    virtual void ExitInstance();
    //获取模块类型
    virtual MODULE_TYPE GetModuleType();
    //消息分发函数
    virtual void ParseMsg(MSG_INTRA* pMsgParsed);
    //处理各业务模块发来的同步数据
    void OnRecvSyncData(SSyncFrame* pFrame);
    //处理手动切换请求
    void OnManualOver(MSG_INTRA* pMsgParsed);
	//处理手重启请求
    void OnReset(MSG_INTRA* pMsgParsed);
    //处理故障倒换申请
    void OnAskFailOver(SFailOverCause* pCause);
    //处理双机状态请求消息
    void OnReqStatus();
    //定时器处理
    virtual void OnTimer(const long nTimerID);
    //1秒事件处理函数
    void Timer1Sec();
    //向对端发送同步数据帧
    void SendSyncData(BOOL bReadFile);

/*
    成员变量
*/
protected:

    //双机对象的指针
    CComCluster*        m_pComCluster;

    //异步资源对象的指针
    CAsynResKernel*     m_pAsynResKernel;

    //任务栏中的小图标对象的指针
    CShellShow*         m_pShellShow;

    //同步对象管理者的指针
    CSyncManager*       m_pSyncManager;

    //开始激活KERNEL的时间
    time_t              m_tBeginActiveTime;

    //最大激活时间
    long                m_lMaxActiveTime;

    //开始去激活KERNEL的时间
    time_t              m_tBeginDeActiveTime;

    //最大去激活时间
    long                m_lMaxDeActiveTime;

    //1秒定时器ID
    long                m_lTimer1Sec;

    //双机进程退出机制的有名进程锁("iGateway bill V200 Cluster Exit Mutex")，
    //如果用户持用该有名进程锁超过1秒钟，双机进程将执行退出流程
    ACE_Process_Mutex*  m_pExitMutex;

    //双机进程显示图标机制的有名进程锁("iGateway bill V200 Cluster ShowIcon Mutex")，
    //如果用户持用该有名进程锁超过1秒钟，双机进程将重新显示Icon
    ACE_Process_Mutex*  m_pShowIconMutex;

    //节点号
    int                 m_nNodeID;

    //退出的标志
    BOOL                m_bExit;

    //保护同步数据对象的线程安全锁
    ACE_Mutex           m_SyncDataMutex;

    //是否激活
    BOOL                m_bActive;

};

#endif //__CLUSTER_MODULE_H__
