#include "../include/frame.h"
#include <assert.h>
#include "logical_cluster.h"
#include "cluster_mutex.h"
#include "resource.h"

/*
    双机基本状态迁移矩阵
    其中g_nClsStatusArray第一维表示双机基本状态迁移的原始状态，
    第二维表示双机基本状态迁移的目标状态，
    取值为0表示不能迁移，取值为1表示能迁移，
    该矩阵定义为类的全局常量，只可读不可写，任何写操作将导致保护错
*/
const int CLogicalCluster::g_nClsStatusArray[MAX_CLSTR_STATUS + 1]
                                            [MAX_CLSTR_STATUS + 1] =
{
    {0, 0, 1, 1, 0, 0, 0},  //CLUSTER_INITIAL状态
    {0, 0, 1, 0, 0, 1, 0},  //CLUSTER_DEACTIVING状态
    {0, 0, 0, 1, 0, 0, 0},  //CLUSTER_DEACTIVE状态
    {0, 1, 0, 0, 1, 0, 0},  //CLUSTER_ACTIVING状态
    {0, 1, 0, 0, 0, 0, 0},  //CLUSTER_ACTIVE状态
    {0, 0, 0, 1, 0, 0, 0},  //CLUSTER_FAILURE状态
    {0, 0, 0, 0, 0, 0, 0}   //CLUSTER_UNKNOWN状态
};

/*
    双机操作状态迁移矩阵
    其中g_nOprStatusArray第一维表示操作状态迁移的原始状态，
    第二维表示操作状态迁移的目标状态，
    取值为0表示不能迁移，取值为1表示能迁移，
    该矩阵定义为类的全局常量，只可读不可写，任何写操作将导致保护错
*/
const int CLogicalCluster::g_nOprStatusArray[MAX_CLSTR_OPERATION + 1]
                                            [MAX_CLSTR_OPERATION + 1] =
{
    {0, 1, 1, 1, 1, 1, 1},  //OPR_NONE状态
    {1, 0, 1, 0, 0, 1, 1},  //OPR_AGGRESSIVE_ACTIVE状态
    {1, 0, 0, 0, 0, 0, 0},  //OPR_AGGRESSIVE_DEACTIVE状态
    {1, 0, 1, 0, 0, 0, 0},  //OPR_FAILOVER_INIT状态
    {1, 0, 1, 0, 0, 0, 0},  //OPR_FAILOVER_RESP状态
    {1, 0, 1, 0, 1, 0, 0},  //OPR_HANDOVER_INIT状态
    {1, 0, 1, 0, 0, 0, 0}   //OPR_HANDOVER_RESP状态
};

/*
    基本状态的描述
*/
const char* const CLogicalCluster::g_szStatusDesp[MAX_CLSTR_STATUS + 1] = 
{
    "CLUSTER_INITIAL",          //Cluster处于刚启动后的初始化状态
    "CLUSTER_DEACTIVING",       //Cluster正在去激活过程中
    "CLUSTER_DEACTIVE",         //Cluster处于去激活状态
    "CLUSTER_ACTIVING",         //Cluster正在激活过程中
    "CLUSTER_ACTIVE",           //Cluster处于激活状态
    "CLUSTER_FAILURE",          //Cluster处于故障状态
    "CLUSTER_UNKNOWN"           //Cluster处于未知状态
};

/*
    操作状态的描述
*/
const char* const CLogicalCluster::g_szOperDesp[MAX_CLSTR_OPERATION + 1] = 
{
    "OPR_NONE",                 //没有进行任何操作
    "OPR_AGGRESSIVE_ACTIVE",    //心跳中断时引发的主动激活操作
    "OPR_AGGRESSIVE_DEACTIVE",  //主动去激活操作，如在退出时
    "OPR_FAILOVER_INIT",        //失效切换操作的发起端
    "OPR_FAILOVER_RESP",        //失效切换操作的响应端
    "OPR_HANDOVER_INIT",        //HandOver操作的发起端
    "OPR_HANDOVER_RESP"         //HandOver操作的响应端
};



/********************************************************
* 名称: CLogicalCluster::CLogicalCluster
* 功能: 构造函数
* 输入:
* 输出:
* 返回:
********************************************************/
CLogicalCluster::CLogicalCluster()
{
    m_lActiveInterval       = ACTIVE_INTERVAL_TIME_DEFAULT;
    m_lMaxActiveFailTimes   = MAX_ACTIVE_FAIL_TIMES_DEFAULT;
    m_tLastActiveTime       = time(NULL) - m_lActiveInterval;
    m_SwitchBackMode        = SM_AUTO_BACK;

    //初始化本端信息
    memset(&m_LocalNodeMsg, 0, sizeof(m_LocalNodeMsg));
    m_LocalNodeMsg.nStatus              = CLUSTER_INITIAL;
    m_LocalNodeMsg.nOperation           = OPR_NONE;
    m_LocalNodeMsg.lLastActiveGapTime   = m_lActiveInterval;

    //初始化对端信息
    memset(&m_PeerNodeMsg, 0, sizeof(m_LocalNodeMsg));
    m_PeerNodeMsg.nNodeID    = INVALID_NODE_ID;
    m_PeerNodeMsg.nStatus    = CLUSTER_UNKNOWN;
    m_PeerNodeMsg.nOperation = OPR_NONE;
}

/********************************************************
* 名称: CLogicalCluster::~CLogicalCluster
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CLogicalCluster::~CLogicalCluster()
{
    //
}

/********************************************************
* 名称: CLogicalCluster::GetNodeID
* 功能: 获取节点号
* 输入:
* 输出:
* 返回: int 节点号
********************************************************/
int CLogicalCluster::GetNodeID()
{
    return m_LocalNodeMsg.nNodeID;
}

/********************************************************
* 名称: CLogicalCluster::BeginActiveMe
* 功能: 发起激活操作
* 输入:
* 输出:
* 返回:
********************************************************/
void CLogicalCluster::BeginActiveMe()
{
    int nRet            = ERR_SUCCESS;
    m_tLastActiveTime   = time(NULL);

    switch(m_LocalNodeMsg.nStatus)
    {
    case CLUSTER_ACTIVE:
    //已经激活
        EndActiveMe(TRUE);
        return;

    case CLUSTER_ACTIVING:
    //正在激活
        return;

    default:
        nRet = CanActiveMe();
        if(nRet != ERR_SUCCESS)
        {
            TRACE(MTS_CLSTR, S_CANNOT_ACTIVE_ME, nRet);
            TRACE(MTS_CLSTR, S_LOCAL_STATUS_NOTIFY, 
                  g_szStatusDesp[m_LocalNodeMsg.nStatus],
                  g_szOperDesp[m_LocalNodeMsg.nOperation]);
            return;
        }
    }

    m_LocalNodeMsg.nStatus = CLUSTER_ACTIVING;

    ActiveMe();
}

/********************************************************
* 名称: CLogicalCluster::ActiveMe
* 功能: 激活操作
* 输入:
* 输出:
* 返回: int 激活操作是否成功
********************************************************/
int CLogicalCluster::ActiveMe()
{
    EndActiveMe(TRUE);

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CLogicalCluster::EndActiveMe
* 功能: 激活操作的扫尾工作
* 输入: bSucc : 激活操作是否成功
* 输出:
* 返回: void
********************************************************/
void CLogicalCluster::EndActiveMe(BOOL bSucc)
{
    if (bSucc)
    //激活成功
    {
        m_LocalNodeMsg.nStatus = CLUSTER_ACTIVE;
        OnNotify(NOTIFY_ACTIVE_SUCCESS);
        switch(m_LocalNodeMsg.nOperation)
        {
        case OPR_FAILOVER_RESP:
            //结束FailOver响应端操作
            EndFailOverResp(TRUE);
            break;

        case OPR_HANDOVER_INIT:
            //结束HandOver发起端操作
            EndHandOverInit(TRUE);
            break;

        case OPR_AGGRESSIVE_ACTIVE:
            //结束主动激活操作
            OnNotify(NOTIFY_AGGRESSIVE_ACTIVE_SUCCESS);
            m_LocalNodeMsg.nOperation = OPR_NONE;
            break;

        default:
            assert(0);
            break;
        }
    }
    else
    //激活失败
    {
        OnNotify(NOTIFY_ACTIVE_FAIL);
        m_LocalNodeMsg.lActiveFailTimes++;
        switch(m_LocalNodeMsg.nOperation)
        {
        case OPR_FAILOVER_RESP:
            //结束FailOver响应端操作
            EndFailOverResp(FALSE);
            break;

        case OPR_HANDOVER_INIT:
            //结束HandOver发起端操作
            EndHandOverInit(FALSE);
            break;

        case OPR_AGGRESSIVE_ACTIVE:
            //结束主动激活操作
            OnNotify(NOTIFY_AGGRESSIVE_ACTIVE_FAIL);
            break;

        default:
            assert(0);
            break;
        }
        m_LocalNodeMsg.nOperation = OPR_AGGRESSIVE_DEACTIVE;
        OnNotify(NOTIFY_AGGRESSIVE_DEACTIVE);

        //激活失败，执行主动去激活流程
        BeginDeActiveMe();
    }
}

/********************************************************
* 名称: CLogicalCluster::CanActiveMe
* 功能: 是否能进行激活操作
* 输入:
* 输出:
* 返回: int 是否能进行激活操作，0表示可以，非零值表示错误码
********************************************************/
int CLogicalCluster::CanActiveMe()
{
    if(!g_nClsStatusArray[m_LocalNodeMsg.nStatus][CLUSTER_ACTIVING])
    //基本状态迁移矩阵不能迁移
    {
        return ERR_CLS_STATUS_TRANSFER;
    }

    if((CLUSTER_FAILURE == m_LocalNodeMsg.nStatus) &&
       (OPR_AGGRESSIVE_ACTIVE != m_LocalNodeMsg.nOperation))
    //如果状态为故障并不是主动激活操作，并返回错误
    {
        return ERR_FAILURE_STATUS;
    }

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CLogicalCluster::BeginDeActiveMe
* 功能: 发起去激活操作
* 输入:
* 输出:
* 返回:
********************************************************/
void CLogicalCluster::BeginDeActiveMe()
{
    switch(m_LocalNodeMsg.nStatus)
    {
    case CLUSTER_INITIAL:
    case CLUSTER_DEACTIVE:
    case CLUSTER_FAILURE:
    //如果是初始化状态，已去激活状态或故障状态，则直接结束去激活操作
        EndDeActiveMe();
        return;

    case CLUSTER_DEACTIVING:
    //如果是正在去激活状态，则退出
        return;

    default:
        break;
    }

    m_LocalNodeMsg.nStatus = CLUSTER_DEACTIVING;

    DeActiveMe();
}

/********************************************************
* 名称: CLogicalCluster::DeActiveMe
* 功能: 去激活操作
* 输入:
* 输出:
* 返回:
********************************************************/
void CLogicalCluster::DeActiveMe()
{
    EndDeActiveMe();
}

/********************************************************
* 名称: CLogicalCluster::EndDeActiveMe
* 功能: 去激活操作的扫尾工作
* 输入:
* 输出:
* 返回:
********************************************************/
void CLogicalCluster::EndDeActiveMe()
{
    OnNotify(NOTIFY_DEACTIVE_SUCCESS);

    if(m_LocalNodeMsg.nStatus != CLUSTER_FAILURE)
    {
        if(m_LocalNodeMsg.lActiveFailTimes > m_lMaxActiveFailTimes)
        {
            OnNotify(NOTIFY_LOCALNODE_FAILURE);
            m_LocalNodeMsg.nStatus = CLUSTER_FAILURE;
        }
        else
        {
            m_LocalNodeMsg.nStatus = CLUSTER_DEACTIVE;
        }
    }

    switch(m_LocalNodeMsg.nOperation)
    {
    case OPR_HANDOVER_RESP:
        //结束HandOver响应端操作
        EndHandOverResp();
        break;

    case OPR_FAILOVER_INIT:
        //结束FailOver发起端的去激活操作
        EndFailOverInitDeActive();
        break;

    case OPR_AGGRESSIVE_DEACTIVE:
        //结束主动去激活操作
        m_LocalNodeMsg.nOperation = OPR_NONE;
        OnNotify(NOTIFY_AGGRESSIVE_DEACTIVE_COMPLETE);
        break;

    default:
        assert(0);
        break;
    }
}

/********************************************************
* 名称: CLogicalCluster::BeginHandOverInit
* 功能: 开始进行HandOver发起端的操作
* 输入:
* 输出:
* 返回:
********************************************************/
void CLogicalCluster::BeginHandOverInit()
{
    m_LocalNodeMsg.nOperation = OPR_HANDOVER_INIT;
}

/********************************************************
* 名称: CLogicalCluster::BeginHandOverResp
* 功能: 开始进行HandOver响应端的操作
* 输入:
* 输出:
* 返回:
********************************************************/
void CLogicalCluster::BeginHandOverResp()
{
    m_LocalNodeMsg.nOperation = OPR_HANDOVER_RESP;

    BeginDeActiveMe();
}

/********************************************************
* 名称: CLogicalCluster::EndHandOverResp
* 功能: HandOver响应端结束操作的接口
* 输入:
* 输出:
* 返回:
********************************************************/
void CLogicalCluster::EndHandOverResp()
{
    m_LocalNodeMsg.nOperation = OPR_NONE;
}

/********************************************************
* 名称: CLogicalCluster::ProcHandOverAns
* 功能: HandOver发起端处理HandOver应答的接口
* 输入: pData   : 外带数据的指针
*       uLength : 外带数据的长度
* 输出:
* 返回:
********************************************************/
void CLogicalCluster::ProcHandOverAns(void* pData, UINT uLength)
{
    BeginActiveMe();
}

/********************************************************
* 名称: CLogicalCluster::EndHandOverInit
* 功能: HandOver发起端完成HandOver的工作
* 输入: bSucc : 激活是否成功
* 输出:
* 返回:
********************************************************/
void CLogicalCluster::EndHandOverInit(BOOL bSucc)
{
    m_LocalNodeMsg.nOperation = OPR_NONE;

    if(bSucc)
    {
        OnNotify(NOTIFY_HANDOVER_SUCCESS);
    }
    else
    {
        OnNotify(NOTIFY_HANDOVER_FAIL);
    }
}

/********************************************************
* 名称: CLogicalCluster::BeginFailOverInit
* 功能: 开始进行FailOver发起端的操作
* 输入:
* 输出:
* 返回:
********************************************************/
void CLogicalCluster::BeginFailOverInit()
{
    m_LocalNodeMsg.nOperation = OPR_FAILOVER_INIT;
    m_LocalNodeMsg.lActiveFailTimes++;

    BeginDeActiveMe();
}

/********************************************************
* 名称: CLogicalCluster::EndFailOverInitDeActive
* 功能: FailOver发起端完成去激活操作
* 输入:
* 输出:
* 返回:
********************************************************/
void CLogicalCluster::EndFailOverInitDeActive()
{
    //子类需要在此函数向对端发FAILOVER命令
}

/********************************************************
* 名称: CLogicalCluster::BeginFailOverResp
* 功能: FailOver响应端开始操作
* 输入: pData   : 外带数据的指针
*       uLength : 外带数据的长度
* 输出:
* 返回:
********************************************************/
void CLogicalCluster::BeginFailOverResp(void* pData, UINT4 uLength)
{
    m_LocalNodeMsg.nOperation = OPR_FAILOVER_RESP;

    BeginActiveMe();
}

/********************************************************
* 名称: CLogicalCluster::EndFailOverResp
* 功能: FailOver响应端结束操作
* 输入: BOOL bSucc : 激活是否成功
* 输出:
* 返回:
********************************************************/
void CLogicalCluster::EndFailOverResp(BOOL bSucc)
{
    m_LocalNodeMsg.nOperation = OPR_NONE;

    if(bSucc)
    {
        OnNotify(NOTIFY_FAILOVER_RESP_SUCCESS);
    }
    else
    {
        OnNotify(NOTIFY_FAILOVER_RESP_FAIL);
    }
}

/********************************************************
* 名称: CLogicalCluster::ProcFailOverAns
* 功能: FailOver发起端处理FailOver应答
* 输入: bSucc : FailOver操作是否成功
* 输出:
* 返回:
********************************************************/
void CLogicalCluster::ProcFailOverAns(BOOL bSucc)
{
    m_LocalNodeMsg.nOperation = OPR_NONE;

    if(bSucc)
    {
        OnNotify(NOTIFY_FAILOVER_SUCCESS);
    }
    else
    {
        OnNotify(NOTIFY_FAILOVER_FAIL);
    }
}

/********************************************************
* 名称: CLogicalCluster::CanFailOver
* 功能: 判断是否能进行故障倒换
* 输入: EFAILOVERCAUSE_TYPE nCause : 倒换原因
* 输出:
* 返回: int 是否能继续倒换
********************************************************/
int CLogicalCluster::CanFailOver(EFAILOVERCAUSE_TYPE nCause)
{
    //只能在激活状态时才能发起FailOver操作
    if(CLUSTER_ACTIVE != m_LocalNodeMsg.nStatus)
    {
         return ERR_FAIL;
    }

    //最好让当前操作执行结束以后再执行FailOver操作
    if(OPR_NONE != m_LocalNodeMsg.nOperation)
    {
        return ERR_FAIL;
    }

    //如果对端为故障，或无知状态，则不能倒换
    if((CLUSTER_FAILURE == m_PeerNodeMsg.nStatus) ||
       (CLUSTER_UNKNOWN == m_PeerNodeMsg.nStatus))
    {
         return ERR_FAIL;
    }

    //如果对端的操作状态迁移不合法，则不能倒换
    if(!g_nOprStatusArray[m_PeerNodeMsg.nOperation][OPR_FAILOVER_RESP])
    {
         return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

/********************************************************
* 名称: public CLogicalCluster::DoFailOver
* 功能: 对外提供申请进行故障倒换的接口
* 输入: EFAILOVERCAUSE_TYPE nCause : 倒换原因
* 输出:
* 返回: int 是否已发起FailOver故障倒换操作
********************************************************/
int CLogicalCluster::DoFailOver(EFAILOVERCAUSE_TYPE nCause)
{
    CClusterMutex::Instance()->Acquire();

    int nRet = CanFailOver(nCause);
    if(ERR_SUCCESS == nRet)
    {
        OnNotify(NOTIFY_FAILOVER, (UINT4)nCause);

        BeginFailOverInit();
    }
    else
    {
        TRACE(MTS_CLSTR, S_CANNOT_START_FAILOVER, nCause, nRet);
        TRACE(MTS_CLSTR, S_LOCAL_STATUS_NOTIFY, 
               g_szStatusDesp[m_LocalNodeMsg.nStatus],
               g_szOperDesp[m_LocalNodeMsg.nOperation]);
        TRACE(MTS_CLSTR, S_PEER_STATUS_NOTIFY, 
               g_szStatusDesp[m_PeerNodeMsg.nStatus],
               g_szOperDesp[m_PeerNodeMsg.nOperation]);

        MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, 
               S_CANNOT_START_FAILOVER, nCause, nRet);
        MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, S_LOCAL_STATUS_NOTIFY, 
               g_szStatusDesp[m_LocalNodeMsg.nStatus],
               g_szOperDesp[m_LocalNodeMsg.nOperation]);
        MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, S_PEER_STATUS_NOTIFY, 
               g_szStatusDesp[m_PeerNodeMsg.nStatus],
               g_szOperDesp[m_PeerNodeMsg.nOperation]);
    }

    CClusterMutex::Instance()->Release();

    return nRet;
}

/********************************************************
* 名称: CLogicalCluster::Timer1Sec
* 功能: 秒事件处理函数
* 输入:
* 输出:
* 返回:
********************************************************/
void CLogicalCluster::Timer1Sec()
{
    //更新上次激活到现在的时间差
    m_LocalNodeMsg.lLastActiveGapTime = time(NULL) - m_tLastActiveTime;

    //如果本端已激活成功，并时间超过10分钟，则将连续激活失败计数器清零
    if((m_LocalNodeMsg.lLastActiveGapTime > SUCCESS_ACTIVE_INV_TIME) &&
       (CLUSTER_ACTIVE == m_LocalNodeMsg.nStatus))
    {
        m_LocalNodeMsg.lActiveFailTimes = 0;
    }

    //主服务器启动时发起HandOver操作
    if((OPR_NONE == m_LocalNodeMsg.nOperation) &&
       (CLUSTER_INITIAL == m_LocalNodeMsg.nStatus) &&
       (SM_AUTO_BACK == m_SwitchBackMode) &&
       (m_LocalNodeMsg.nNodeID < m_PeerNodeMsg.nNodeID) &&
       (m_PeerNodeMsg.nNodeID != INVALID_NODE_ID))
    {
        //发起HandOver发起端操作
        m_LocalNodeMsg.nStatus = CLUSTER_DEACTIVE;
        BeginHandOverInit();
    }

    //对端故障,且本端去激活,则发起HandOver操作
    if((OPR_NONE == m_LocalNodeMsg.nOperation) &&
       ((CLUSTER_DEACTIVE == m_LocalNodeMsg.nStatus) ||
        (CLUSTER_INITIAL == m_LocalNodeMsg.nStatus)) &&
       (CLUSTER_FAILURE == m_PeerNodeMsg.nStatus) &&
       (m_LocalNodeMsg.lLastActiveGapTime > m_lActiveInterval))
    {
        //发起HandOver发起端操作
        m_LocalNodeMsg.nStatus = CLUSTER_DEACTIVE;
        BeginHandOverInit();
    }

    /*
      当两端都未激活，且未故障，则只看谁距上次激活的时间更长，
      如果相同，则优先让主机先启动
    */
    if((m_LocalNodeMsg.lLastActiveGapTime > m_lActiveInterval) &&
       (OPR_NONE == m_LocalNodeMsg.nOperation) &&
       (OPR_NONE == m_PeerNodeMsg.nOperation ) &&
       ((CLUSTER_DEACTIVE == m_LocalNodeMsg.nStatus) || 
        (CLUSTER_INITIAL  == m_LocalNodeMsg.nStatus)   ) &&
       ((CLUSTER_DEACTIVE == m_PeerNodeMsg.nStatus ) ||
        (CLUSTER_INITIAL  == m_PeerNodeMsg.nStatus )   ))
    {
       if(m_LocalNodeMsg.lLastActiveGapTime >
          m_PeerNodeMsg.lLastActiveGapTime)
       {
            //发起HandOver发起端操作
            m_LocalNodeMsg.nStatus = CLUSTER_DEACTIVE;
            BeginHandOverInit();
       }
       else if((m_LocalNodeMsg.lLastActiveGapTime ==
                m_PeerNodeMsg.lLastActiveGapTime) &&
               (m_LocalNodeMsg.nNodeID < m_PeerNodeMsg.nNodeID))
       {
            //发起HandOver发起端操作
            m_LocalNodeMsg.nStatus = CLUSTER_DEACTIVE;
            BeginHandOverInit();
       }
    }


    if((m_LocalNodeMsg.nNodeID >= m_PeerNodeMsg.nNodeID) &&
       ((CLUSTER_ACTIVE == m_LocalNodeMsg.nStatus  ) ||
        (CLUSTER_ACTIVING == m_LocalNodeMsg.nStatus)   ) &&
       ((CLUSTER_ACTIVE == m_PeerNodeMsg.nStatus   ) ||
        (CLUSTER_ACTIVING == m_PeerNodeMsg.nStatus )   ))
    {
        m_LocalNodeMsg.nOperation = OPR_AGGRESSIVE_DEACTIVE;

        OnNotify(NOTIFY_AGGRESSIVE_DEACTIVE);

        //执行主动去激活流程
        BeginDeActiveMe();
    }

    //考虑系统的最大可用性，优化为当一端为故障状态时，
    //如果发现对端激活成功，则将自己置为DeActive状态
    if((OPR_NONE == m_LocalNodeMsg.nOperation) &&
       (CLUSTER_FAILURE == m_LocalNodeMsg.nStatus)&&
       (OPR_NONE == m_PeerNodeMsg.nOperation) &&
       (m_LocalNodeMsg.lLastActiveGapTime > 1800) &&
       (m_LocalNodeMsg.lLastActiveGapTime > m_PeerNodeMsg.lLastActiveGapTime))
    {
       if((CLUSTER_ACTIVE == m_PeerNodeMsg.nStatus) 
          || (CLUSTER_FAILURE == m_PeerNodeMsg.nStatus))
       {
            m_LocalNodeMsg.nStatus          = CLUSTER_DEACTIVE;
            m_LocalNodeMsg.lActiveFailTimes = 0;
       }
    }
}

/********************************************************
* 名称: CLogicalCluster::OnNotify
* 功能: 双机事件信息处理函数
* 输入:
*       ECLUSTER_NOTIFY uMsg : 事件信息码
*       UINT4 uParam1 = 0    : 参数一，可能是故障对象的指针
*       UINT4 uParam2 = 0    : 参数二，保留参数
* 输出:
* 返回: void
********************************************************/
void CLogicalCluster::OnNotify(ECLUSTER_NOTIFY uMsg,
                               UINT4 uParam1,
                               UINT4 uParam2)
{
    //
}
