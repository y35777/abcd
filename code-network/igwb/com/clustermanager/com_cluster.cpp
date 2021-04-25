#include "../include/frame.h"
#include <assert.h>
#include "cluster_mutex.h"
#include "com_cluster.h"
#include "cluster_creator.h"
#include "cluster_network.h"
#include "asyn_res_object.h"
#include "cluster_toolbox.h"
#include "resource.h"

CComCluster* CComCluster::g_pComCluster = NULL;

/********************************************************
* 名称: CComCluster::CComCluster
* 功能: 构造函数
* 输入:
* 输出:
* 返回:
********************************************************/
CComCluster::CComCluster()
{
    m_uInitSeqNum       = 0;
    m_uRespSeqNum       = 0;
    m_tHBBrokenTime     = time(NULL);
    m_nMaxHBBrokenTime  = MAX_HB_BROKEN_TIME_DEFAULT;
    m_bInstallArray     = TRUE;
    m_bHeartBeatBroken  = FALSE;
    m_bSingleServer     = FALSE;

    m_bWaitForExit      = FALSE;
    m_bExit             = FALSE;

    //以下两个指针初始化时必须赋为有效值，不能为空
    m_pNetwork          = NULL;
    m_pResManager       = NULL;

    //以下两个指针运行时不一定有效，在使用时需要检查
    m_pAsynResObject    = NULL;
    m_pClusterAlarm     = NULL;

    m_nLastStaticResStatus = 0;
    m_nLastDynResStatus    = 0;
    m_szStatusFile[0]      = '\0';

    m_stClusterStatus.uVersion       = 0;
    m_stClusterStatus.uSafeDeActived = 1;
    m_bLastActived                   = FALSE;
}

/********************************************************
* 名称: CComCluster::~CComCluster
* 功能: 析构函函数
* 输入:
* 输出:
* 返回:
********************************************************/
CComCluster::~CComCluster()
{
    if(NULL != m_pNetwork)
    {
        delete m_pNetwork;
        m_pNetwork = NULL;
    }

    if(NULL != m_pResManager)
    {
        delete m_pResManager;
        m_pResManager = NULL;
    }

    if(NULL != m_pClusterAlarm)
    {
        delete m_pClusterAlarm;
        m_pClusterAlarm = NULL;
    }

    m_pAsynResObject = NULL;
}

void CComCluster::SetStatusFile(const char* szStatusFile)
{
    if(szStatusFile != NULL)
    {
        strncpy(m_szStatusFile, szStatusFile, sizeof(m_szStatusFile));
        m_szStatusFile[sizeof(m_szStatusFile) - 1] = '\0';
    }
}

/********************************************************
* 名称: CComCluster::InitInstance
* 功能: 进行CComCluster类的初始化工作
* 输入:
* 输出:
* 返回: int 如果所有初始化操作成功，则返回0，否则返回非零值
********************************************************/
int CComCluster::InitInstance()
{
    int nRet = ERR_SUCCESS;

    if(m_szStatusFile[0] !='\0')
    {
        FILE* f = fopen(m_szStatusFile, "rb");
        if(NULL != f)
        {
            fread(&m_stClusterStatus, sizeof(m_stClusterStatus), 1, f);
            fclose(f);

            if(m_stClusterStatus.uSafeDeActived == 0)
            {
                /*
                   为了让上次未正常退出的备机能有较高的激活优先级，
                   将其距上次激活的间隔时间修改为最大
                */
                m_tLastActiveTime      = 0;
                m_LocalNodeMsg.lLastActiveGapTime = time(NULL);
                m_LocalNodeMsg.nNodeID = -1;
            }
        }
    }

    //创建并初始化m_pResManager对象
    assert(NULL == m_pResManager);
    m_pResManager = new CResManager;
    if(NULL == m_pResManager)
    {
        return ERR_INIT_CLUSTER_RECOURSE;
    }
    else
    {
        nRet = m_pResManager->InitInstance();
        if(ERR_SUCCESS != nRet)
        {
            return nRet;
        }
    }

    //单机模式
    if(GetbSingleServer())
    {
        m_bHeartBeatBroken = TRUE;
        m_nMaxHBBrokenTime = -1;
    }

    //创建并初始化m_pNetwork对象
    assert(NULL == m_pNetwork);
    m_pNetwork = new CClusterNetwork(this);
    if(NULL == m_pNetwork)
    {
        return ERR_INIT_CLUSTER_NETWORK;
    }
    else
    {
        nRet = m_pNetwork->InitInstance();
        if(ERR_SUCCESS != nRet)
        {
            return nRet;
        }
    }

    //向对端发送节点信息，命令码为CMD_CLS_HEARTBEAT，参数为m_LocalNodeMsg的内存拷贝
    SClusterFrame* pFrame = new(sizeof(m_LocalNodeMsg)) SClusterFrame;
    pFrame->nCmdCode      = CMD_CLS_HEARTBEAT;
    memcpy(pFrame->Data, (BYTE*)&m_LocalNodeMsg, sizeof(m_LocalNodeMsg));
    Send(pFrame);

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CComCluster::Instance
* 功能: 进行CComCluster类的单实例化工作
* 输入: CAsynResObject* pObject : 异步对象的指针
*       CClusterAlarm* pAlarm   : 告警对象的指针
* 输出:
* 返回: CComCluster* 返回CComCluster的单实例对象的指针
********************************************************/
CComCluster* CComCluster::Instance(CAsynResObject* pObject,
                                   CClusterAlarm* pAlarm)
{
    if(NULL == g_pComCluster)
    {
        g_pComCluster = CClusterCreator::CreateComCluster();
        if(NULL != g_pComCluster)
        {
            g_pComCluster->m_pAsynResObject = pObject;
            g_pComCluster->m_pClusterAlarm  = pAlarm;
            pObject->Attach(g_pComCluster);
            int nRet = g_pComCluster->InitInstance();
            if (ERR_SUCCESS != nRet)
            {
                TRACE(MTS_CLSTR, S_CLUSTER_INIT_FAILED, nRet);
                Destroy();
            }
        }
        else
        {
            delete pAlarm;
        }
    }

    //返回CComCluster的单实例指针g_pComCluster
    return g_pComCluster;
}

/********************************************************
* 名称: CComCluster::Destroy
* 功能: 进行CComCluster类的单实例销毁工作
* 输入:
* 输出:
* 返回:
********************************************************/
void CComCluster::Destroy()
{
    if(NULL != g_pComCluster)
    {
        delete g_pComCluster;
        g_pComCluster = NULL;
    }
}

/********************************************************
* 名称: CComCluster::ReportMsg
* 功能: 报告当前信息
* 输入: 
* 输出: 
* 返回: void 
********************************************************/
void CComCluster::ReportMsg()
{
    MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, S_LOCAL_STATUS_NOTIFY, 
           g_szStatusDesp[m_LocalNodeMsg.nStatus],
           g_szOperDesp[m_LocalNodeMsg.nOperation]);
    MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, S_PEER_STATUS_NOTIFY, 
           g_szStatusDesp[m_PeerNodeMsg.nStatus],
           g_szOperDesp[m_PeerNodeMsg.nOperation]);
    MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, S_HEARTBEAT_STATUS_NOTIFY, 
           m_pNetwork->GetLinkCount(),
           m_pNetwork->GetActiveLinkCount());

#ifdef _ONLY_FOR_TEST
    TRACE(MTS_CLSTR, S_LOCAL_STATUS_NOTIFY, 
           g_szStatusDesp[m_LocalNodeMsg.nStatus],
           g_szOperDesp[m_LocalNodeMsg.nOperation]);
    TRACE(MTS_CLSTR, S_PEER_STATUS_NOTIFY, 
           g_szStatusDesp[m_PeerNodeMsg.nStatus],
           g_szOperDesp[m_PeerNodeMsg.nOperation]);
    TRACE(MTS_CLSTR, S_HEARTBEAT_STATUS_NOTIFY, 
           m_pNetwork->GetLinkCount(),
           m_pNetwork->GetActiveLinkCount());
#endif //_ONLY_FOR_TEST

}

/********************************************************
* 名称: CComCluster::DoExit
* 功能: 处理退出命令
* 输入:
* 输出:
* 返回:
********************************************************/
void CComCluster::DoExit()
{
    CClusterMutex::Instance()->Acquire();

    m_bExit = TRUE;

    m_LocalNodeMsg.nOperation = OPR_AGGRESSIVE_DEACTIVE;
    switch(m_LocalNodeMsg.nStatus)
    {
    case CLUSTER_ACTIVE:
    case CLUSTER_ACTIVING:
        OnNotify(NOTIFY_AGGRESSIVE_DEACTIVE);

        //发起主动去激活操作
        BeginDeActiveMe();
        break;

    case CLUSTER_DEACTIVING:
        break;

    default:
        EndDeActiveMe();
        break;
    }

    CClusterMutex::Instance()->Release();
}

/********************************************************
* 名称: CComCluster::ActiveMe
* 功能: 激活操作
* 输入:
* 输出:
* 返回: int 激活是否成功，0表示成功，非零值表示失败
********************************************************/
int CComCluster::ActiveMe()
{
    //激活同步资源
    int nRet = ERR_SUCCESS;
    if(NULL != m_pResManager)
    {
        nRet = m_pResManager->Active(RT_DYNAMIC);
        if (0 != nRet)
        {
            CResObject* pResObject = m_pResManager->GetObject(nRet);
            for(int i = 1; i <= MAX_RES_NUM; i++)
            {
                if((1 << (i - 1)) & nRet)
                {
                    CResObject* pResObject = m_pResManager->GetObject(i);
                    OnNotify(NOTIFY_STARTRESOURCE_FAIL, (UINT4)pResObject);
                }
            }
            return ERR_FAIL;
        }
    }

    //激活异步资源
    if(NULL != m_pAsynResObject)
    {
        nRet = m_pAsynResObject->Active();
    }

    return nRet;
}

void CComCluster::EndActiveMe(BOOL bSucc)
{
    CLogicalCluster::EndActiveMe(bSucc);

    if(bSucc)
    {
        m_bLastActived = TRUE;
        if(m_szStatusFile[0] !='\0')
        {
            m_stClusterStatus.uSafeDeActived = 0;
            m_LocalNodeMsg.nNodeID = 1;
            FILE* f = fopen(m_szStatusFile, "wb");
            if(NULL != f)
            {
                fwrite(&m_stClusterStatus, sizeof(m_stClusterStatus), 1, f);
                fclose(f);
            }
        }
    }
}


/********************************************************
* 名称: CComCluster::OnAsynResActived
* 功能: 异步资源激活完成的接口
* 输入: bSucc : 异步资源激活是否成功，TRUE表示成功，FALSE表示失败
* 输出:
* 返回:
********************************************************/
void CComCluster::OnAsynResActived(BOOL bSucc)
{
    if(!bSucc)
    {
        OnNotify(NOTIFY_START_ASYN_RES_FAIL);
    }

    EndActiveMe(bSucc);
}

/********************************************************
* 名称: CComCluster::DeActiveMe
* 功能: 去激活操作
* 输入:
* 输出:
* 返回:
********************************************************/
void CComCluster::DeActiveMe()
{
    if(NULL != m_pAsynResObject)
    {
        m_pAsynResObject->DeActive();
    }

    //清除上次的动态资源状态
    m_nLastDynResStatus = 0;
}

/********************************************************
* 名称: CComCluster::EndDeActiveMe
* 功能: 去激活操作的扫尾工作
* 输入:
* 输出:
* 返回:
********************************************************/
void CComCluster::EndDeActiveMe()
{
    CLogicalCluster::EndDeActiveMe();

    if(m_szStatusFile[0] !='\0')
    {
        if(m_LocalNodeMsg.nNodeID == -1)
        {
            m_LocalNodeMsg.nNodeID = 1;
        }

        if(m_bLastActived)
        {
            m_stClusterStatus.uSafeDeActived = 1;
            FILE* f = fopen(m_szStatusFile, "wb");
            if(NULL != f)
            {
                fwrite(&m_stClusterStatus, sizeof(m_stClusterStatus), 1, f);
                fclose(f);
            }
        }
    }
    m_bLastActived = FALSE;

    if(m_bExit)
    {
        //调用CAsynResObject类的收集附带数据的接口
        void *pData = 0;
        int  nLen   = 0;
        if (m_pAsynResObject != NULL)
        {
            m_pAsynResObject->GetAttachedData(pData, nLen);
        }

        /*
          保证对端不会因本端无操作且为去激活状态而立即激活起来
        */
        m_LocalNodeMsg.nOperation = OPR_AGGRESSIVE_DEACTIVE;
        SClusterFrame *pFrame = new(sizeof(SNODE_MSG) + nLen) SClusterFrame;
        pFrame->nCmdCode      = CMD_CLS_ASKFOR_ACTIVE;
        memcpy(pFrame->Data, (BYTE*)&m_LocalNodeMsg, sizeof(SNODE_MSG));
        if((nLen > 0) && (NULL != pData))
        {
            memcpy(pFrame->Data + sizeof(SNODE_MSG), (BYTE*)pData, nLen);
        }
        delete [] pData;
        Send(pFrame);

        m_bWaitForExit        = TRUE;
    }
}

/********************************************************
* 名称: CComCluster::OnAsynResDeActived
* 功能: 异步资源去激活完成的接口
* 输入:
* 输出:
* 返回:
********************************************************/
void CComCluster::OnAsynResDeActived()
{
    int nRet = 0;
    if(NULL != m_pResManager)
    {
        //去激活动态资源
        nRet = m_pResManager->DeActive(RT_DYNAMIC);
        if (nRet != 0)
        {
            CResObject* pResObject = m_pResManager->GetObject(nRet);
            OnNotify(NOTIFY_STOPRESOURCE_FAIL, (UINT4)pResObject);
        }
    }

    EndDeActiveMe();
}

/********************************************************
* 名称: CComCluster::BeginHandOverInit
* 功能: 开始进行HandOver发起端的操作
* 输入:
* 输出:
* 返回:
********************************************************/
void CComCluster::BeginHandOverInit()
{
    CLogicalCluster::BeginHandOverInit();

    //向对端发送HandOver命令（CMD_CLS_HANDOVER）
    SClusterFrame *pFrame = new(sizeof(SNODE_MSG)) SClusterFrame;
    pFrame->uSeqNum       = ++m_uInitSeqNum;
    pFrame->nCmdCode      = CMD_CLS_HANDOVER;
    memcpy(pFrame->Data, (BYTE*)&m_LocalNodeMsg, sizeof(SNODE_MSG));
    Send(pFrame);

    OnNotify(NOTIFY_HANDOVER);
}

/********************************************************
* 名称: CComCluster::EndHandOverResp
* 功能: HandOver响应端结束操作的接口
* 输入:
* 输出:
* 返回:
********************************************************/
void CComCluster::EndHandOverResp()
{
    CLogicalCluster::EndHandOverResp();

    //调用CAsynResObject类的收集附带数据的接口
    void *pData = 0;
    int  nLen   = 0;
    if (m_pAsynResObject != NULL)
    {
        m_pAsynResObject->GetAttachedData(pData, nLen);
    }

    //将本端状态及收集的附带数据附加到HandOver应答帧中，向对端发送出去
    SClusterFrame *pFrame = new(sizeof(SNODE_MSG) + nLen) SClusterFrame;
    pFrame->uSeqNum       = m_uRespSeqNum;
    pFrame->nCmdCode      = CMD_CLS_ANS_HANDOVER;
    memcpy(pFrame->Data, (BYTE*)&m_LocalNodeMsg, sizeof(SNODE_MSG));
    if((nLen > 0) && (NULL != pData))
    {
        memcpy(pFrame->Data + sizeof(SNODE_MSG), (BYTE*)pData, nLen);
    }
    delete [] pData;
    Send(pFrame);
}

/********************************************************
* 名称: CComCluster::ProcHandOverAns
* 功能: HandOver发起端处理HandOver应答的接口
* 输入: pData   : 外带数据的指针
*       uLength : 外带数据的长度
* 输出:
* 返回:
********************************************************/
void CComCluster::ProcHandOverAns(void* pData, UINT4 uLength)
{
    //向CAsynResObject传递HandOver外带数据;
    if((NULL != m_pAsynResObject) && (uLength > 0))
    {
        m_pAsynResObject->ProcAttachedData(pData, uLength);
    }

    //调用父类的ProcHandOverAns函数;
    CLogicalCluster::ProcHandOverAns(pData, uLength);
}

/********************************************************
* 名称: CComCluster::EndFailOverInitDeActive
* 功能: FailOver发起端完成去激活操作的接口
* 输入:
* 输出:
* 返回:
********************************************************/
void CComCluster::EndFailOverInitDeActive()
{
    //调用父类的EndFailOverInitDeActive函数;
    CLogicalCluster::EndFailOverInitDeActive();

    //调用CAsynResObject类的收集附带数据的接口;
    void* pData = NULL;
    int  nLen = 0;
    if(m_pAsynResObject != NULL)
    {
        m_pAsynResObject->GetAttachedData(pData, nLen);
    }

    //将本端状态及收集的附带数据附加到FailOver命令帧（CMD_CLS_FAILOVER）中，
    //向对端发送出去
    SClusterFrame *pFrame = new(sizeof(SNODE_MSG) + nLen) SClusterFrame;
    pFrame->uSeqNum       = ++m_uInitSeqNum;
    pFrame->nCmdCode      = CMD_CLS_FAILOVER;
    memcpy(pFrame->Data, (BYTE*)&m_LocalNodeMsg, sizeof(SNODE_MSG));
    if((nLen > 0) && (NULL != pData))
    {
        memcpy(pFrame->Data + sizeof(SNODE_MSG), (BYTE*)pData, nLen);
    }
    delete [] pData;
    Send(pFrame);
}

/********************************************************
* 名称: CComCluster::BeginFailOverResp
* 功能: FailOver响应端开始操作的接口
* 输入: pData   : 外带数据的指针
*       uLength : 外带数据的长度
* 输出:
* 返回:
********************************************************/
void CComCluster::BeginFailOverResp(void* pData, UINT4 uLength)
{
    //向CAsynResObject传递FailOver外带数据
    if((NULL != m_pAsynResObject) && (uLength > 0))
    {
        m_pAsynResObject->ProcAttachedData(pData, uLength);
    }

    //调用父类的BeginFailOverResp函数
    CLogicalCluster::BeginFailOverResp(pData, uLength);
}

/********************************************************
* 名称: CComCluster::EndFailOverResp
* 功能: FailOver响应端结束操作的接口
* 输入: BOOL bSucc : 激活是否成功
* 输出:
* 返回:
********************************************************/
void CComCluster::EndFailOverResp(BOOL bSucc)
{
    //调用父类的EndFailOverResp函数
    CLogicalCluster::EndFailOverResp(bSucc);

    //向对端发本端状态及FailOver是否成功的应答
    //（CMD_CLS_ANS_FAILOVER，参数为BOOL型）
    SClusterFrame *pFrame = new(sizeof(SNODE_MSG) +
                                sizeof(BOOL)) SClusterFrame;
    pFrame->uSeqNum       = m_uRespSeqNum;
    pFrame->nCmdCode      = CMD_CLS_ANS_FAILOVER;
    memcpy(pFrame->Data, (BYTE*)&m_LocalNodeMsg, sizeof(SNODE_MSG));
    memcpy(pFrame->Data + sizeof(SNODE_MSG), (BYTE*)&bSucc, sizeof(BOOL));

    Send(pFrame);
}

/********************************************************
* 名称: CComCluster::CanFailOver
* 功能: 判断是否能进行故障倒换
* 输入: EFAILOVERCAUSE_TYPE nCause : 故障原因
* 输出:
* 返回: int 是否能发起FailOver故障倒换操作
********************************************************/
int CComCluster::CanFailOver(EFAILOVERCAUSE_TYPE nCause)
{
    //调用父类的CanFailOver函数
    int nRet = CLogicalCluster::CanFailOver(nCause);
    if(ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    //如果心跳中断，则返回ERR_HEART_BEAT_BROKEN;
    if(m_bHeartBeatBroken)
    {
        return ERR_HEART_BEAT_BROKEN;
    }

    //如果是带共享磁盘阵列，且故障原因是磁盘阵列有磁盘损坏，则返回失败
    if((m_bInstallArray) &&
       ((nCause == ERROR_HARDDISK) ||
        (nCause == ERROR_NOENOUGHDISK_PRIMARY) ||
        (nCause == ERROR_NOENOUGHDISK_SECONDARY)))
    {
        return ERR_SHARED_DISK_FAIL;
    }

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CComCluster::GetbSingleServer
* 功能: 判断是否是单机模式
* 输入:
* 输出:
* 返回: const BOOL 是否是单机
********************************************************/
const BOOL CComCluster::GetbSingleServer()
{
    return m_bSingleServer;
}

/********************************************************
* 名称: CComCluster::GetbWaitForExit
* 功能: 返回是否处于等待退出的状态
* 输入:
* 输出:
* 返回: const BOOL 返回是否处于等待退出的状态，若是返回TRUE，
*                  否则返回FALSE
********************************************************/
const BOOL CComCluster::GetbWaitForExit()
{
    return  m_bWaitForExit;
}

/********************************************************
* 名称: CComCluster::GetHeartBeatStatus
* 功能: 返回心跳的状态
* 输入:
* 输出:
* 返回: UINT2 返回心跳的状态，低字节为心跳总数，高字节为
*             当前可用的心跳数
********************************************************/
UINT2 CComCluster::GetHeartBeatStatus()
{
    CClusterMutex::Instance()->Acquire();

    UINT2 uRet = 0;
    *((BYTE*)&uRet)     = m_pNetwork->GetLinkCount();
    *((BYTE*)&uRet + 1) = m_pNetwork->GetActiveLinkCount();

    CClusterMutex::Instance()->Release();

    return uRet;
}

/********************************************************
* 名称: CComCluster::Timer1Sec
* 功能: 秒事件处理函数
* 输入:
* 输出:
* 返回:
********************************************************/
void CComCluster::Timer1Sec()
{
    if(m_bWaitForExit)
    {
        return;
    }

    if(!m_bHeartBeatBroken)
    {
        //检测m_pNetwork当前是否有可用的心跳
        int nActiveLink = m_pNetwork->GetActiveLinkCount();
        if(0 == nActiveLink)
        {
            if(time(NULL) - m_tHBBrokenTime >= m_nMaxHBBrokenTime)
            //认为心跳中断
            {
                memset(&m_PeerNodeMsg, 0, sizeof(m_LocalNodeMsg));
                m_PeerNodeMsg.nNodeID    = INVALID_NODE_ID;
                m_PeerNodeMsg.nStatus    = CLUSTER_UNKNOWN;
                m_PeerNodeMsg.nOperation = OPR_NONE;
                m_bHeartBeatBroken       = TRUE;
                OnNotify(NOTIFY_HEARTBEAT_BROKEN);
            }
        }
    }

    //心跳中断激活
    if((m_bHeartBeatBroken) &&
       (OPR_NONE == m_LocalNodeMsg.nOperation) &&
       (CLUSTER_ACTIVING != m_LocalNodeMsg.nStatus) &&
       (CLUSTER_ACTIVE != m_LocalNodeMsg.nStatus) &&
       ((time(NULL) - m_tLastActiveTime) >= m_lActiveInterval))
    {
        m_LocalNodeMsg.nOperation = OPR_AGGRESSIVE_ACTIVE;
        OnNotify(NOTIFY_AGGRESSIVE_ACTIVE);
        BeginActiveMe();
    }

    switch(m_LocalNodeMsg.nStatus)
    {
    case CLUSTER_ACTIVING:
        //检测异步资源的状态是否是已激活状态
        if (NULL != m_pAsynResObject)
        {
            int nStatus = m_pAsynResObject->GetStatus();
            switch(nStatus)
            {
            case EASYNRES_ACTIVE:
            //异步资源的状态是已激活状态
                OnAsynResActived(TRUE);
                break;

            case EASYNRES_DEACTIVE:
            case EASYNRES_FAILURE:
            //异步资源的状态是已去激活状态或故障状态
                OnAsynResActived(FALSE);
                break;

            default:
                break;
            }
        }
        else
        //不存在异步资源
        {
            OnAsynResActived(TRUE);
        }
        break;

    case CLUSTER_DEACTIVING:
        //检测异步资源的状态是否是已去激活状态;
        if (NULL != m_pAsynResObject)
        {
            int nStatus = m_pAsynResObject->GetStatus();
            switch(nStatus)
            {
            case EASYNRES_DEACTIVE:
            case EASYNRES_FAILURE:
            //异步资源的状态是已去激活状态或故障状态
                OnAsynResDeActived();
                break;

            default:
                break;
            }
        }
        else
        //不存在异步资源
        {
            OnAsynResDeActived();
        }
        break;

    case CLUSTER_DEACTIVE:
        if(OPR_HANDOVER_INIT == m_LocalNodeMsg.nOperation)
        {
            if(m_bHeartBeatBroken)
            {
                m_LocalNodeMsg.nOperation = OPR_NONE;
                OnNotify(NOTIFY_HANDOVER_FAIL);
            }
            else
            {
                //向对端发HandOver命令，这里是重发命令，所以m_uInitSeqNum不变
                SClusterFrame *pFrame = new(sizeof(SNODE_MSG)) SClusterFrame;
                pFrame->uSeqNum       = m_uInitSeqNum;
                pFrame->nCmdCode      = CMD_CLS_HANDOVER;
                memcpy(pFrame->Data, (BYTE*)&m_LocalNodeMsg, sizeof(SNODE_MSG));
                Send(pFrame);
            }
        }
    //注意此处没有break,这说明当FailOver发起端处于去激活或故障状态时,都要进行以下操作
    case CLUSTER_FAILURE:
        if(OPR_FAILOVER_INIT == m_LocalNodeMsg.nOperation)
        {
            if(m_bHeartBeatBroken)
            {
                m_LocalNodeMsg.nOperation = OPR_NONE;
                OnNotify(NOTIFY_FAILOVER_FAIL);
            }
            else
            {
                //向对端发FailOver命令，这里是重发命令，所以m_uInitSeqNum不变
                SClusterFrame *pFrame = new(sizeof(SNODE_MSG)) SClusterFrame;
                pFrame->uSeqNum       = m_uInitSeqNum;
                pFrame->nCmdCode      = CMD_CLS_FAILOVER;
                memcpy(pFrame->Data, (BYTE*)&m_LocalNodeMsg, sizeof(SNODE_MSG));
                Send(pFrame);
            }
        }
        break;

    default:
        break;
    }
    
    //调用父类的Timer1Sec函数
    CLogicalCluster::Timer1Sec();

    //向对端发送节点信息，命令码为CMD_CLS_HEARTBEAT，参数为m_LocalNodeMsg的内存拷贝
    SClusterFrame* pFrame = new(sizeof(m_LocalNodeMsg)) SClusterFrame;
    pFrame->nCmdCode      = CMD_CLS_HEARTBEAT;
    memcpy(pFrame->Data, (BYTE*)&m_LocalNodeMsg, sizeof(m_LocalNodeMsg));
    Send(pFrame);

    //对m_pResManager进行一分钟一次的状态检测及磁盘阵列检测
    static time_t tCheckStatusTime = 0;
    if(time(NULL) - tCheckStatusTime >= ONE_MINUTE_IN_SECOND)
    {
        tCheckStatusTime = time(NULL);
        ReportMsg();
        if(CheckResStatus() != 0)
        {
            return;
        }
    }

    //调用CClusterAlarm对象的秒定时器函数
    if(NULL != m_pClusterAlarm)
    {
        m_pClusterAlarm->Timer1Sec();
    }
}

/********************************************************
* 名称: CComCluster::CheckResStatus
* 功能: 检资源状态状态
* 输入: 
* 输出:
* 返回: int 是否发生倒换，0表示未发生，非零值表示发生
********************************************************/
int CComCluster::CheckResStatus()
{
    int nResStatus = 0;
    if(CLUSTER_ACTIVE == m_LocalNodeMsg.nStatus)
    {
        //检测动态资源
        nResStatus = m_pResManager->CheckStatus(RT_DYNAMIC);

        if(m_nLastDynResStatus != nResStatus)
        {
            if(NULL != m_pClusterAlarm)
            {
                for(int i = 1; i <= MAX_RES_NUM; i++)
                {
                    //资源失效告警
                    if(((1 << (i - 1)) & nResStatus) && !((1 << (i - 1)) & m_nLastDynResStatus))
                    {
                        CResObject* pResObject = m_pResManager->GetObject(i);
                        m_pClusterAlarm->OnNotify(NOTIFY_RESOURCE_FAIL, 
                                                  (UINT4)pResObject);
                    }
                    //资源恢复告警
                    else if(!((1 << (i - 1)) & nResStatus) && ((1 << (i - 1)) & m_nLastDynResStatus))
                    {
                        CResObject* pResObject = m_pResManager->GetObject(i);
                        m_pClusterAlarm->OnNotify(NOTIFY_RESOURCE_RECOVERY, 
                                                  (UINT4)pResObject);
                    }
                }
            }

            m_nLastDynResStatus = nResStatus;
        }

        //检查是否需要倒换
        for(int i = 1; i <= MAX_RES_NUM; i++)
        {
            if((1 << (i - 1)) & nResStatus)
            {
                if(m_pResManager->VerifyGroupFailed(i))
                {
                    CResObject* pResObject = m_pResManager->GetObject(i);
                    OnNotify(NOTIFY_RESOURCE_FAIL, 
                             (UINT4)pResObject,
                             NOT_NEED_ALARM);
                    return 1;
                }
            }
        }

        //检测异步资源
        if ((NULL != m_pAsynResObject) && 
            (m_pAsynResObject->CheckStatus() != ERR_SUCCESS))
        {
            OnNotify(NOTIFY_ASYN_RES_FAIL);
            return 1;
        }
    }

    //检测静态资源
    nResStatus = m_pResManager->CheckStatus(RT_STATIC);

    if(m_nLastStaticResStatus != nResStatus)
    {
        if(NULL != m_pClusterAlarm)
        {
            for(int i = 1; i <= MAX_RES_NUM; i++)
            {
                //资源失效告警
                if(((1 << (i - 1)) & nResStatus) && !((1 << (i - 1)) & m_nLastStaticResStatus))
                {
                    CResObject* pResObject = m_pResManager->GetObject(i);
                    m_pClusterAlarm->OnNotify(NOTIFY_RESOURCE_FAIL, 
                                              (UINT4)pResObject);
                }
                //资源恢复告警
                else if(!((1 << (i - 1)) & nResStatus) && ((1 << (i - 1)) & m_nLastStaticResStatus))
                {
                    CResObject* pResObject = m_pResManager->GetObject(i);
                    m_pClusterAlarm->OnNotify(NOTIFY_RESOURCE_RECOVERY, 
                                              (UINT4)pResObject);
                }
            }
        }
        m_nLastStaticResStatus = nResStatus;
    }

    if(CLUSTER_ACTIVE == m_LocalNodeMsg.nStatus)
    {
        //检查是否需要倒换
        for(int i = 1; i <= MAX_RES_NUM; i++)
        {
            if((1 << (i - 1)) & nResStatus)
            {
                if(m_pResManager->VerifyGroupFailed(i))
                {
                    CResObject* pResObject = m_pResManager->GetObject(i);
                    OnNotify(NOTIFY_RESOURCE_FAIL, 
                             (UINT4)pResObject,
                             NOT_NEED_ALARM);
                    return 1;
                }
            }
        }
    }

    //检测硬盘
    static BOOL bHardDiskFailed = FALSE;
    static int nHDCheckInv      = 0;
    if(nHDCheckInv++ >= 10)
    {
        nResStatus = m_pResManager->CheckStatus(RT_HARDDISK);
        if((nResStatus > 0) && (!bHardDiskFailed))
        {
            bHardDiskFailed = TRUE;
            OnNotify(NOTIFY_HARDDISK_FAIL, (UINT4)nResStatus);
        }
        else if((0 == nResStatus) && (bHardDiskFailed))
        {
            bHardDiskFailed = FALSE;
            OnNotify(NOTIFY_HARDDISK_RECOVERY);
        }
        nHDCheckInv = 0;
    }

    return 0;
}


/********************************************************
* 名称: CComCluster::Send
* 功能: 向对端发送消息
* 输入: SClusterFrame* pFrame : 双机消息帧
* 输出:
* 返回: int 发送是否成功，0表示成功，非零值表示失败
********************************************************/
int CComCluster::Send(SClusterFrame* pFrame)
{
    int nRet = ERR_SEND_FORBIDDED;

    CClusterMutex::Instance()->Acquire();

    if(!m_bWaitForExit)
    {
        nRet = m_pNetwork->Send(pFrame);
    }

    CClusterMutex::Instance()->Release();

    delete pFrame;
    return nRet;
}

/********************************************************
* 名称: CComCluster::OnReceive
* 功能: 处理对端发送的双机消息
* 输入: SClusterFrame* pFrame : 双机消息帧
* 输出:
* 返回: void
********************************************************/
void CComCluster::OnReceive(SClusterFrame* pFrame)
{
    if(m_bWaitForExit)
    {
        return;
    }

    //更新对端信息
    if((CMD_CLS_LOGICAL_MINCODE <= pFrame->nCmdCode) &&
       (CMD_CLS_LOGICAL_MAXCODE >= pFrame->nCmdCode))
    {
        memcpy((BYTE*)&m_PeerNodeMsg, pFrame->Data, sizeof(SNODE_MSG));
        switch(m_PeerNodeMsg.nStatus)
        {
        case CLUSTER_ACTIVING:
        case CLUSTER_ACTIVE:
            /*
                如果对端处于激活或正在激活状态，并且本端处于初始化状态，如果
                    1.对等模式下,
                    2.非对等模式下，如果本端节点号大于对端,
                则将本端状态迁移到去激活状态。
            */
            if((CLUSTER_INITIAL == m_LocalNodeMsg.nStatus) &&
               ((SM_NONE_BACK == m_SwitchBackMode) || 
                ((SM_AUTO_BACK == m_SwitchBackMode) && 
                 (m_PeerNodeMsg.nNodeID < m_LocalNodeMsg.nNodeID))))
            {
                m_LocalNodeMsg.nStatus = CLUSTER_DEACTIVE;
            }
            break;

        default:
            break;
        }
    }

    //检查心跳是否已恢复
    if(m_bHeartBeatBroken)
    {
        OnNotify(NOTIFY_HEARTBEAT_RECOVERY);
        m_bHeartBeatBroken = FALSE;
    }
    m_tHBBrokenTime = time(NULL);

    //对端状态为初始化状态
    if(CLUSTER_INITIAL == m_PeerNodeMsg.nStatus)
    {
        OnNotify(NOTIFY_PEERNODE_RESTART);
    }

    switch(pFrame->nCmdCode)
    {
    case CMD_CLS_HANDOVER:
    //收到HandOver命令
        /*
            在非对等模式下，主节点处于初始化状态时，不能迁移到HandOver响应端状态
        */
        if((SM_AUTO_BACK == m_SwitchBackMode) && 
           (CLUSTER_INITIAL == m_LocalNodeMsg.nStatus) &&
           (m_LocalNodeMsg.nNodeID < m_PeerNodeMsg.nNodeID))
        {
            break;
        }

        /*
            这里可以进行操作状态迁移的可能性有两种：
            1、操作状态迁移矩阵允许迁移；
            2、极端情况下，两边同时发起HandOver操作，
               则节点号较大的一方迁移到响应端状态
        */
        if((g_nOprStatusArray[m_LocalNodeMsg.nOperation][OPR_HANDOVER_RESP]) ||
           ((OPR_HANDOVER_INIT == m_LocalNodeMsg.nOperation) && 
           (m_LocalNodeMsg.nNodeID > m_PeerNodeMsg.nNodeID)))
        {
            m_uRespSeqNum = pFrame->uSeqNum;
            TRACE(MTS_CLSTR, S_RECV_HANDOVER_CMD);
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, S_RECV_HANDOVER_CMD);
            BeginHandOverResp();
        }
        break;

    case CMD_CLS_ANS_HANDOVER:
    //收到HandOver应答
        if((OPR_HANDOVER_INIT == m_LocalNodeMsg.nOperation) &&
           (CLUSTER_DEACTIVE == m_LocalNodeMsg.nStatus) && 
           (pFrame->uSeqNum == m_uInitSeqNum))
        {
            TRACE(MTS_CLSTR, S_RECV_HANDOVER_ANS);
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, S_RECV_HANDOVER_ANS);
            ProcHandOverAns(pFrame->Data + sizeof(SNODE_MSG),
                            pFrame->nLen - sizeof(SNODE_MSG));
        }
        break;

    case CMD_CLS_FAILOVER:
    //收到FailOver命令
        if(g_nOprStatusArray[m_LocalNodeMsg.nOperation][OPR_FAILOVER_RESP])
        {
            m_uRespSeqNum = pFrame->uSeqNum;
            TRACE(MTS_CLSTR, S_RECV_FAILOVER_CMD);
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, S_RECV_FAILOVER_CMD);
            BeginFailOverResp(pFrame->Data + sizeof(SNODE_MSG),
                              pFrame->nLen - sizeof(SNODE_MSG));
        }
        //如果本端无操作且已激活，说明本端上次发的应答丢了，这里直接返回应答
        else if(OPR_NONE == m_LocalNodeMsg.nOperation
                && CLUSTER_ACTIVE == m_LocalNodeMsg.nStatus)
        {
            BOOL bSucc = TRUE;
            SClusterFrame *pFrame = new(sizeof(SNODE_MSG) +
                                        sizeof(BOOL)) SClusterFrame;
            pFrame->uSeqNum       = pFrame->uSeqNum;
            pFrame->nCmdCode      = CMD_CLS_ANS_FAILOVER;
            memcpy(pFrame->Data, (BYTE*)&m_LocalNodeMsg, sizeof(SNODE_MSG));
            memcpy(pFrame->Data + sizeof(SNODE_MSG), (BYTE*)&bSucc, sizeof(BOOL));

            Send(pFrame);
        }
        break;

    case CMD_CLS_ANS_FAILOVER:
    //收到FailOver应答
        if((OPR_FAILOVER_INIT == m_LocalNodeMsg.nOperation) &&
           (pFrame->uSeqNum == m_uInitSeqNum))
        {
            BOOL uSucc = 0;
            memcpy((BYTE*)&uSucc, pFrame->Data + sizeof(SNODE_MSG), sizeof(BOOL));
            TRACE(MTS_CLSTR, S_RECV_FAILOVER_ANS, uSucc);
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, S_RECV_FAILOVER_ANS, uSucc);
            ProcFailOverAns(uSucc);
        }
        break;

    case CMD_CLS_HEARTBEAT:
    //心跳信息帧，在前面已做处理
        break;

    case CMD_CLS_ASKFOR_HANDOVER:
    //收到CMD_CLS_ASKFOR_HANDOVER,如果当前本端状态为初始化及去激活状态,
    //操作状态为OPR_NONE,则调用BeginHandOverInit函数,触发HandOver操作
        if((g_nOprStatusArray[m_LocalNodeMsg.nOperation][OPR_HANDOVER_INIT]) &&
           ((CLUSTER_DEACTIVE == m_LocalNodeMsg.nStatus) ||
           (CLUSTER_INITIAL == m_LocalNodeMsg.nStatus)))
        {
            BeginHandOverInit();
        }
        break;

    case CMD_CLS_ASKFOR_ACTIVE:
    //收到CMD_CLS_ASKFOR_ACTIVE,则表示对端即将退出,如果当前本端状态为
    //初始化及去激活状态,则触发主动激活操作
        if((CLUSTER_DEACTIVE == m_LocalNodeMsg.nStatus) ||
           (CLUSTER_INITIAL == m_LocalNodeMsg.nStatus))
        {
            //向CAsynResObject传递FailOver外带数据
            BYTE* pData   = pFrame->Data + sizeof(SNODE_MSG);
            UINT4 uLength = pFrame->nLen - sizeof(SNODE_MSG); 
            if((NULL != m_pAsynResObject) && (uLength > 0))
            {
                m_pAsynResObject->ProcAttachedData(pData, uLength);
            }

            if(m_bInstallArray)
            {
                m_LocalNodeMsg.nOperation = OPR_AGGRESSIVE_ACTIVE;
                OnNotify(NOTIFY_AGGRESSIVE_ACTIVE);
                BeginActiveMe();
            }
        }
        break;

    default:
        if((CMD_CLS_ASYNRES_MINCODE <= pFrame->nCmdCode) &&
           (CMD_CLS_ASYNRES_MAXCODE >= pFrame->nCmdCode) &&
           (NULL != m_pAsynResObject))
        {
            m_pAsynResObject->OnReceive(pFrame);
        }
        break;
    }
}

/********************************************************
* 名称: CComCluster::DoManualOver
* 功能: 申请手动倒换
* 输入:
* 输出:
* 返回: int 是否能继续进行手动倒换
********************************************************/
int CComCluster::DoManualOver(STRING& strErrInfo, STRING& strDetailInfo)
{
    int nRet = ERR_FAIL;
    CClusterMutex::Instance()->Acquire();
    char szErrInfo[1024];
    char szTemp[100];

    strDetailInfo = "";
    strErrInfo    = "";
    if(m_bHeartBeatBroken || (CLUSTER_UNKNOWN == m_PeerNodeMsg.nStatus))
    {
        strErrInfo    = S_MANSWCH_HEARTBEAT_NO_RECOV;
        strDetailInfo = strErrInfo;
    }
    else if(OPR_NONE != m_PeerNodeMsg.nOperation)
    {
        SNPRINTF(szErrInfo, 
                 sizeof(szErrInfo),
                 S_MANSWCH_OPERATION_IS_DOING,
                 "");
        szErrInfo[sizeof(szErrInfo) - 1] = '\0';
        strErrInfo = szErrInfo;

        SNPRINTF(szTemp, 
                 sizeof(szTemp),
                 "(%s)",
                 g_szOperDesp[m_PeerNodeMsg.nOperation]);

        SNPRINTF(szErrInfo, 
                 sizeof(szErrInfo),
                 S_MANSWCH_OPERATION_IS_DOING,
                 szTemp);
        szErrInfo[sizeof(szErrInfo) - 1] = '\0';
        strDetailInfo = szErrInfo;
    }
    else if((CLUSTER_DEACTIVING == m_PeerNodeMsg.nStatus) ||
            (CLUSTER_ACTIVING == m_PeerNodeMsg.nStatus))
    {
        SNPRINTF(szErrInfo, 
                 sizeof(szErrInfo),
                 S_MANSWCH_STATUS_INSTABLE,
                 "");
        szErrInfo[sizeof(szErrInfo) - 1] = '\0';
        strErrInfo = szErrInfo;

        SNPRINTF(szTemp, 
                 sizeof(szTemp),
                 "(%s)",
                 g_szStatusDesp[m_PeerNodeMsg.nStatus]);

        SNPRINTF(szErrInfo, 
                 sizeof(szErrInfo),
                 S_MANSWCH_STATUS_INSTABLE,
                 szTemp);
        szErrInfo[sizeof(szErrInfo) - 1] = '\0';
        strDetailInfo = szErrInfo;
    }
    else
    {
        SClusterFrame *pFrame = new(sizeof(SNODE_MSG)) SClusterFrame;
        pFrame->nCmdCode = CMD_CLS_ASKFOR_HANDOVER;
        memcpy(pFrame->Data, (BYTE*)&m_LocalNodeMsg, sizeof(SNODE_MSG));
        nRet = Send(pFrame);
        if(ERR_SUCCESS != nRet)
        {
            SNPRINTF(szErrInfo, 
                     sizeof(szErrInfo),
                     S_MANSWCH_SEND_CMD,
                     nRet);
            szErrInfo[sizeof(szErrInfo) - 1] = '\0';
            strErrInfo    = szErrInfo;
            strDetailInfo = strErrInfo;
        }
    }

    CClusterMutex::Instance()->Release();

    return nRet;
}

/********************************************************
* 名称: CComCluster::OnNotify
* 功能: 双机事件信息处理函数
* 输入: ECLUSTER_NOTIFY uMsg : 事件信息码
*       UINT4 uParam1        : 参数一，可能是故障对象的指针
*       UINT4 uParam2        : 参数二，保留参数
* 输出:
* 返回:
********************************************************/
void CComCluster::OnNotify(ECLUSTER_NOTIFY uMsg,
                           UINT4 uParam1, UINT4 uParam2)
{
    if((NULL != m_pClusterAlarm) && (uParam2 != NOT_NEED_ALARM))
    {
        m_pClusterAlarm->OnNotify(uMsg, uParam1, uParam2);
    }

    switch (uMsg)
    {
    case NOTIFY_STOPRESOURCE_FAIL:
    //停止资源失败
    {
        CResObject* pResObject = (CResObject*) uParam1;
        if((pResObject != NULL) && 
           (pResObject->GetResType() == ERES_VIRTUAL_IP))
        {
            if(m_pAsynResObject != NULL)
            {
                m_pAsynResObject->DeActive();
                ACE_OS::sleep(10);
            }
            //重启操作系统
            Reboot();
        }
        break;
    }
    case NOTIFY_RESOURCE_FAIL:
    //资源失败的通知命令码
        DoFailOver(ERROR_RESOURCE);
        break;

    case NOTIFY_ASYN_RES_FAIL:
    //异步资源失败的通知命令码
        if (ERR_SUCCESS != DoFailOver(ERROR_PROCESS_EXCEPTION))
        {
            m_LocalNodeMsg.nOperation = OPR_AGGRESSIVE_DEACTIVE;

            OnNotify(NOTIFY_AGGRESSIVE_DEACTIVE);

            //执行主动去激活流程
            BeginDeActiveMe();
        }
        break;

    case NOTIFY_HARDDISK_FAIL:
    //阵列硬盘故障
        DoFailOver(ERROR_HARDDISK);
        break;

    default:
        break;
    }
}
