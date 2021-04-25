#ifndef __LOGICAL_CLUSTER_H__
#define __LOGICAL_CLUSTER_H__

#include "cluster_struct.h"

//双机基本状态
enum ECLSTR_STATUS
{
    CLUSTER_INITIAL     = 0,    //Cluster处于刚启动后的初始化状态
    CLUSTER_DEACTIVING,         //Cluster正在去激活过程中
    CLUSTER_DEACTIVE,           //Cluster处于去激活状态
    CLUSTER_ACTIVING,           //Cluster正在激活过程中
    CLUSTER_ACTIVE,             //Cluster处于激活状态
    CLUSTER_FAILURE,            //Cluster处于故障状态
    CLUSTER_UNKNOWN             //Cluster处于未知状态
};
#define MIN_CLSTR_STATUS    CLUSTER_INITIAL
#define MAX_CLSTR_STATUS    CLUSTER_UNKNOWN

//双机操作状态
enum ECLSTR_OPERATION
{
    OPR_NONE            = 0,    //没有进行任何操作
    OPR_AGGRESSIVE_ACTIVE,      //心跳中断时引发的主动激活操作
    OPR_AGGRESSIVE_DEACTIVE,    //主动去激活操作，如在退出时
    OPR_FAILOVER_INIT,          //失效切换操作的发起端
    OPR_FAILOVER_RESP,          //失效切换操作的响应端
    OPR_HANDOVER_INIT,          //HandOver操作的发起端
    OPR_HANDOVER_RESP           //HandOver操作的响应端
};
#define MIN_CLSTR_OPERATION     OPR_NONE
#define MAX_CLSTR_OPERATION     OPR_HANDOVER_RESP

//节点信息结构
struct SNODE_MSG
{
#define INVALID_NODE_ID     0xFFFFFFF
    int                 nNodeID;                //本节点的ID
    ECLSTR_STATUS       nStatus;                //双机基本状态
    ECLSTR_OPERATION    nOperation;             //双机操作状态
    long                lLastActiveGapTime;     //上次激活时间
    long                lActiveFailTimes;       //连接激活失败次数
};

//切换模式
enum ESWITCH_MODE
{
    SM_AUTO_BACK = 0,           //自动回切
    SM_NONE_BACK                //不进行自动回切
};

class CLogicalCluster
{
/*
    成员函数
*/
public:

    //获取节点号
    int GetNodeID();
    //对外提供申请进行故障倒换的接口
    int DoFailOver(EFAILOVERCAUSE_TYPE nCause);
    //秒事件处理函数
    virtual void Timer1Sec();
    //双机事件信息处理函数
    virtual void OnNotify(ECLUSTER_NOTIFY uMsg,
                          UINT4 uParam1 = 0,
                          UINT4 uParam2 = 0);

protected:

    CLogicalCluster();
    virtual ~CLogicalCluster();

    virtual int InitInstance() = 0;         //初始化函数

    //激活操作
    void BeginActiveMe();                   //发起激活操作
    virtual int ActiveMe();                 //激活操作
    virtual void EndActiveMe(BOOL bSucc);   //激活操作的扫尾工作
    virtual int CanActiveMe();              //是否能进行激活操作

    //去激活操作
    void BeginDeActiveMe();                 //发起去激活操作
    virtual void DeActiveMe();              //去激活操作
    virtual void EndDeActiveMe();           //去激活操作的扫尾工作

    //HandOver操作
    virtual void BeginHandOverInit();       //开始进行HandOver发起端的操作
    virtual void BeginHandOverResp();       //开始进行HandOver响应端的操作
    virtual void EndHandOverResp();         //HandOver响应端结束操作的接口
    //HandOver发起端处理HandOver应答
    virtual void ProcHandOverAns(void* pData, 
                                 UINT uLength);
    //HandOver发起端完成HandOver工作
    virtual void EndHandOverInit(BOOL bSucc);

    //FailOver操作
    virtual void BeginFailOverInit();       //开始进行FailOver发起端的操作
    virtual void EndFailOverInitDeActive(); //FailOver发起端完成去激活操作
    //FailOver响应端开始操作
    virtual void BeginFailOverResp(void* pData, UINT4 uLength);
    //FailOver响应端结束操作
    virtual void EndFailOverResp(BOOL bSucc);
    //FailOver发起端处理FailOver应答
    virtual void ProcFailOverAns(BOOL bSucc);
    //判断是否能进行故障倒换
    virtual int CanFailOver(EFAILOVERCAUSE_TYPE nCause);


/*
    成员变量
*/
    //基本状态的描述
    static const char* const g_szStatusDesp[MAX_CLSTR_STATUS + 1];

    //操作状态的描述
    static const char* const g_szOperDesp[MAX_CLSTR_OPERATION + 1];

protected:
    //本节点的信息
    SNODE_MSG       m_LocalNodeMsg;

    //对端节点的信息
    SNODE_MSG       m_PeerNodeMsg;

    //两次激活时间之差应大于等于这个值(任何主动发起的激活操作都要进行判断)
    long            m_lActiveInterval;
#define ACTIVE_INTERVAL_TIME_DEFAULT    120
#define SUCCESS_ACTIVE_INV_TIME         600

    //切换策略
    ESWITCH_MODE    m_SwitchBackMode;

    //最大激活失败次数，超过此值，状态将置为故障
    long            m_lMaxActiveFailTimes;
#define MAX_ACTIVE_FAIL_TIMES_DEFAULT   3

    //上次激活函数的时间
    time_t          m_tLastActiveTime;

    //双机基本状态迁移矩阵
    static const int g_nClsStatusArray[MAX_CLSTR_STATUS + 1]
                                      [MAX_CLSTR_STATUS + 1];

    //双机操作状态迁移矩阵
    static const int g_nOprStatusArray[MAX_CLSTR_OPERATION + 1]
                                      [MAX_CLSTR_OPERATION + 1];
};

#endif //__LOGICAL_CLUSTER_H__