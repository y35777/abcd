#ifndef __COM_CLUSTER_H__
#define __COM_CLUSTER_H__

#include "logical_cluster.h"
#include "cluster_alarm.h"
#include "res_manager.h"

class CClusterNetwork;
class CAsynResObject;

struct SCLUSTERSTATUS
{
    UINT4   uVersion;
    UINT4   uSafeDeActived;
};

class CComCluster : public CLogicalCluster
{
/*
    成员函数
*/
public:

    //进行CComCluster类的单实例化工作
    static CComCluster* Instance(CAsynResObject* pObject = NULL,
                                 CClusterAlarm* pAlarm = NULL);
    //进行CComCluster类的单实例销毁工作
    static void Destroy();

    //处理退出命令
    void DoExit();
    //判断是否是单机模式
    const BOOL GetbSingleServer();
    //返回是否处于等待退出的状态
    const BOOL GetbWaitForExit();
    //返回心跳的状态
    UINT2 GetHeartBeatStatus();
    //秒事件处理函数
    virtual void Timer1Sec();
    //向对端发送消息
    int Send(SClusterFrame* pFrame);
    //处理对端发送的双机消息
    virtual void OnReceive(SClusterFrame* pFrame);
    //申请手动倒换
    int DoManualOver(STRING& strErrInfo, STRING& strDetailInfo);
    //双机事件信息处理函数
#define NOT_NEED_ALARM     0xFFFFFFFF
    virtual void OnNotify(ECLUSTER_NOTIFY uMsg,
                          UINT4 uParam1 = 0,
                          UINT4 uParam2 = 0);

    void SetStatusFile(const char* szStatusFile);
protected:

    CComCluster();
    virtual ~CComCluster();

    //进行CComCluster类的初始化工作
    virtual int InitInstance();

    //报告当前信息
    void ReportMsg();

    //激活操作
    virtual int ActiveMe();
    //激活操作的扫尾工作
    virtual void EndActiveMe(BOOL bSucc);
    //异步资源激活完成的接口
    virtual void OnAsynResActived(BOOL bSucc);
    //去激活操作
    virtual void DeActiveMe();
    //去激活操作的扫尾工作
    virtual void EndDeActiveMe();
    //异步资源去激活完成的接口
    virtual void OnAsynResDeActived();
    //开始进行HandOver发起端的操作
    virtual void BeginHandOverInit();
    //HandOver响应端结束操作的接口
    virtual void EndHandOverResp();
    //HandOver发起端处理HandOver应答的接口
    virtual void ProcHandOverAns(void* pData, UINT4 uLength);
    //FailOver发起端完成去激活操作的接口
    virtual void EndFailOverInitDeActive();
    //FailOver 响应端开始操作的接口
    virtual void BeginFailOverResp(void* pData, UINT4 uLength);
    //FailOver 响应端结束操作的接口
    virtual void EndFailOverResp(BOOL bSucc);
    //判断是否能进行故障倒换
    virtual int CanFailOver(EFAILOVERCAUSE_TYPE nCause);
    //检资源状态状态
    virtual int CheckResStatus();

/*
    成员变量
*/
protected:

    //心跳是否中断
    BOOL                m_bHeartBeatBroken;
#define MAX_HB_BROKEN_TIME_DEFAULT  300

    //心跳最后一次存在的时间
    int                 m_tHBBrokenTime;

    //判断心跳中断的时间阀值
    int                 m_nMaxHBBrokenTime;

    /*
        以下两个指针初始化时必须赋为有效值，不能为空
    */
    //双机间通讯对象的指针
    CClusterNetwork*    m_pNetwork;

    //同步资源管理对象的指针
    CResManager*        m_pResManager;

    /*
        以下两个指针运行时不一定有效，在使用时需要检查
    */
    //异步资源对象的指针
    CAsynResObject*     m_pAsynResObject;

    //双机故障处理对象的指针
    CClusterAlarm*      m_pClusterAlarm;

    //CComCluster的单实例指针
    static CComCluster* g_pComCluster;

    //是否带共享磁盘阵列
    BOOL                m_bInstallArray;

    //是否是单机方式运行
    BOOL                m_bSingleServer;

    //是否退出的标志
    BOOL                m_bExit;

    /*
        是否已完成主动去激活操作，等待退出，在这种状态下将封锁所有
        推动双机状态迁移的接口，用户在销毁双机对象前，必须保证这个
        变量为真。
    */
    BOOL                m_bWaitForExit;

    //发起端操作帧序列号
    UINT4               m_uInitSeqNum;

    //响应端操作帧序列号  
    UINT4               m_uRespSeqNum;

    //上次动态资源的状态
    int                 m_nLastDynResStatus;

    //上次静态资源的状态
    int                 m_nLastStaticResStatus;

    friend class CClusterCreator;

    char                m_szStatusFile[MAX_PATH];

    SCLUSTERSTATUS      m_stClusterStatus;

    //在去激活前是否是激活状态
    BOOL                m_bLastActived;
};

#endif //__COM_CLUSTER_H__