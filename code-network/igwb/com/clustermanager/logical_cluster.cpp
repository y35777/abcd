#include "../include/frame.h"
#include <assert.h>
#include "logical_cluster.h"
#include "cluster_mutex.h"
#include "resource.h"

/*
    ˫������״̬Ǩ�ƾ���
    ����g_nClsStatusArray��һά��ʾ˫������״̬Ǩ�Ƶ�ԭʼ״̬��
    �ڶ�ά��ʾ˫������״̬Ǩ�Ƶ�Ŀ��״̬��
    ȡֵΪ0��ʾ����Ǩ�ƣ�ȡֵΪ1��ʾ��Ǩ�ƣ�
    �þ�����Ϊ���ȫ�ֳ�����ֻ�ɶ�����д���κ�д���������±�����
*/
const int CLogicalCluster::g_nClsStatusArray[MAX_CLSTR_STATUS + 1]
                                            [MAX_CLSTR_STATUS + 1] =
{
    {0, 0, 1, 1, 0, 0, 0},  //CLUSTER_INITIAL״̬
    {0, 0, 1, 0, 0, 1, 0},  //CLUSTER_DEACTIVING״̬
    {0, 0, 0, 1, 0, 0, 0},  //CLUSTER_DEACTIVE״̬
    {0, 1, 0, 0, 1, 0, 0},  //CLUSTER_ACTIVING״̬
    {0, 1, 0, 0, 0, 0, 0},  //CLUSTER_ACTIVE״̬
    {0, 0, 0, 1, 0, 0, 0},  //CLUSTER_FAILURE״̬
    {0, 0, 0, 0, 0, 0, 0}   //CLUSTER_UNKNOWN״̬
};

/*
    ˫������״̬Ǩ�ƾ���
    ����g_nOprStatusArray��һά��ʾ����״̬Ǩ�Ƶ�ԭʼ״̬��
    �ڶ�ά��ʾ����״̬Ǩ�Ƶ�Ŀ��״̬��
    ȡֵΪ0��ʾ����Ǩ�ƣ�ȡֵΪ1��ʾ��Ǩ�ƣ�
    �þ�����Ϊ���ȫ�ֳ�����ֻ�ɶ�����д���κ�д���������±�����
*/
const int CLogicalCluster::g_nOprStatusArray[MAX_CLSTR_OPERATION + 1]
                                            [MAX_CLSTR_OPERATION + 1] =
{
    {0, 1, 1, 1, 1, 1, 1},  //OPR_NONE״̬
    {1, 0, 1, 0, 0, 1, 1},  //OPR_AGGRESSIVE_ACTIVE״̬
    {1, 0, 0, 0, 0, 0, 0},  //OPR_AGGRESSIVE_DEACTIVE״̬
    {1, 0, 1, 0, 0, 0, 0},  //OPR_FAILOVER_INIT״̬
    {1, 0, 1, 0, 0, 0, 0},  //OPR_FAILOVER_RESP״̬
    {1, 0, 1, 0, 1, 0, 0},  //OPR_HANDOVER_INIT״̬
    {1, 0, 1, 0, 0, 0, 0}   //OPR_HANDOVER_RESP״̬
};

/*
    ����״̬������
*/
const char* const CLogicalCluster::g_szStatusDesp[MAX_CLSTR_STATUS + 1] = 
{
    "CLUSTER_INITIAL",          //Cluster���ڸ�������ĳ�ʼ��״̬
    "CLUSTER_DEACTIVING",       //Cluster����ȥ���������
    "CLUSTER_DEACTIVE",         //Cluster����ȥ����״̬
    "CLUSTER_ACTIVING",         //Cluster���ڼ��������
    "CLUSTER_ACTIVE",           //Cluster���ڼ���״̬
    "CLUSTER_FAILURE",          //Cluster���ڹ���״̬
    "CLUSTER_UNKNOWN"           //Cluster����δ֪״̬
};

/*
    ����״̬������
*/
const char* const CLogicalCluster::g_szOperDesp[MAX_CLSTR_OPERATION + 1] = 
{
    "OPR_NONE",                 //û�н����κβ���
    "OPR_AGGRESSIVE_ACTIVE",    //�����ж�ʱ�����������������
    "OPR_AGGRESSIVE_DEACTIVE",  //����ȥ��������������˳�ʱ
    "OPR_FAILOVER_INIT",        //ʧЧ�л������ķ����
    "OPR_FAILOVER_RESP",        //ʧЧ�л���������Ӧ��
    "OPR_HANDOVER_INIT",        //HandOver�����ķ����
    "OPR_HANDOVER_RESP"         //HandOver��������Ӧ��
};



/********************************************************
* ����: CLogicalCluster::CLogicalCluster
* ����: ���캯��
* ����:
* ���:
* ����:
********************************************************/
CLogicalCluster::CLogicalCluster()
{
    m_lActiveInterval       = ACTIVE_INTERVAL_TIME_DEFAULT;
    m_lMaxActiveFailTimes   = MAX_ACTIVE_FAIL_TIMES_DEFAULT;
    m_tLastActiveTime       = time(NULL) - m_lActiveInterval;
    m_SwitchBackMode        = SM_AUTO_BACK;

    //��ʼ��������Ϣ
    memset(&m_LocalNodeMsg, 0, sizeof(m_LocalNodeMsg));
    m_LocalNodeMsg.nStatus              = CLUSTER_INITIAL;
    m_LocalNodeMsg.nOperation           = OPR_NONE;
    m_LocalNodeMsg.lLastActiveGapTime   = m_lActiveInterval;

    //��ʼ���Զ���Ϣ
    memset(&m_PeerNodeMsg, 0, sizeof(m_LocalNodeMsg));
    m_PeerNodeMsg.nNodeID    = INVALID_NODE_ID;
    m_PeerNodeMsg.nStatus    = CLUSTER_UNKNOWN;
    m_PeerNodeMsg.nOperation = OPR_NONE;
}

/********************************************************
* ����: CLogicalCluster::~CLogicalCluster
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CLogicalCluster::~CLogicalCluster()
{
    //
}

/********************************************************
* ����: CLogicalCluster::GetNodeID
* ����: ��ȡ�ڵ��
* ����:
* ���:
* ����: int �ڵ��
********************************************************/
int CLogicalCluster::GetNodeID()
{
    return m_LocalNodeMsg.nNodeID;
}

/********************************************************
* ����: CLogicalCluster::BeginActiveMe
* ����: ���𼤻����
* ����:
* ���:
* ����:
********************************************************/
void CLogicalCluster::BeginActiveMe()
{
    int nRet            = ERR_SUCCESS;
    m_tLastActiveTime   = time(NULL);

    switch(m_LocalNodeMsg.nStatus)
    {
    case CLUSTER_ACTIVE:
    //�Ѿ�����
        EndActiveMe(TRUE);
        return;

    case CLUSTER_ACTIVING:
    //���ڼ���
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
* ����: CLogicalCluster::ActiveMe
* ����: �������
* ����:
* ���:
* ����: int ��������Ƿ�ɹ�
********************************************************/
int CLogicalCluster::ActiveMe()
{
    EndActiveMe(TRUE);

    return ERR_SUCCESS;
}

/********************************************************
* ����: CLogicalCluster::EndActiveMe
* ����: ���������ɨβ����
* ����: bSucc : ��������Ƿ�ɹ�
* ���:
* ����: void
********************************************************/
void CLogicalCluster::EndActiveMe(BOOL bSucc)
{
    if (bSucc)
    //����ɹ�
    {
        m_LocalNodeMsg.nStatus = CLUSTER_ACTIVE;
        OnNotify(NOTIFY_ACTIVE_SUCCESS);
        switch(m_LocalNodeMsg.nOperation)
        {
        case OPR_FAILOVER_RESP:
            //����FailOver��Ӧ�˲���
            EndFailOverResp(TRUE);
            break;

        case OPR_HANDOVER_INIT:
            //����HandOver����˲���
            EndHandOverInit(TRUE);
            break;

        case OPR_AGGRESSIVE_ACTIVE:
            //���������������
            OnNotify(NOTIFY_AGGRESSIVE_ACTIVE_SUCCESS);
            m_LocalNodeMsg.nOperation = OPR_NONE;
            break;

        default:
            assert(0);
            break;
        }
    }
    else
    //����ʧ��
    {
        OnNotify(NOTIFY_ACTIVE_FAIL);
        m_LocalNodeMsg.lActiveFailTimes++;
        switch(m_LocalNodeMsg.nOperation)
        {
        case OPR_FAILOVER_RESP:
            //����FailOver��Ӧ�˲���
            EndFailOverResp(FALSE);
            break;

        case OPR_HANDOVER_INIT:
            //����HandOver����˲���
            EndHandOverInit(FALSE);
            break;

        case OPR_AGGRESSIVE_ACTIVE:
            //���������������
            OnNotify(NOTIFY_AGGRESSIVE_ACTIVE_FAIL);
            break;

        default:
            assert(0);
            break;
        }
        m_LocalNodeMsg.nOperation = OPR_AGGRESSIVE_DEACTIVE;
        OnNotify(NOTIFY_AGGRESSIVE_DEACTIVE);

        //����ʧ�ܣ�ִ������ȥ��������
        BeginDeActiveMe();
    }
}

/********************************************************
* ����: CLogicalCluster::CanActiveMe
* ����: �Ƿ��ܽ��м������
* ����:
* ���:
* ����: int �Ƿ��ܽ��м��������0��ʾ���ԣ�����ֵ��ʾ������
********************************************************/
int CLogicalCluster::CanActiveMe()
{
    if(!g_nClsStatusArray[m_LocalNodeMsg.nStatus][CLUSTER_ACTIVING])
    //����״̬Ǩ�ƾ�����Ǩ��
    {
        return ERR_CLS_STATUS_TRANSFER;
    }

    if((CLUSTER_FAILURE == m_LocalNodeMsg.nStatus) &&
       (OPR_AGGRESSIVE_ACTIVE != m_LocalNodeMsg.nOperation))
    //���״̬Ϊ���ϲ�����������������������ش���
    {
        return ERR_FAILURE_STATUS;
    }

    return ERR_SUCCESS;
}

/********************************************************
* ����: CLogicalCluster::BeginDeActiveMe
* ����: ����ȥ�������
* ����:
* ���:
* ����:
********************************************************/
void CLogicalCluster::BeginDeActiveMe()
{
    switch(m_LocalNodeMsg.nStatus)
    {
    case CLUSTER_INITIAL:
    case CLUSTER_DEACTIVE:
    case CLUSTER_FAILURE:
    //����ǳ�ʼ��״̬����ȥ����״̬�����״̬����ֱ�ӽ���ȥ�������
        EndDeActiveMe();
        return;

    case CLUSTER_DEACTIVING:
    //���������ȥ����״̬�����˳�
        return;

    default:
        break;
    }

    m_LocalNodeMsg.nStatus = CLUSTER_DEACTIVING;

    DeActiveMe();
}

/********************************************************
* ����: CLogicalCluster::DeActiveMe
* ����: ȥ�������
* ����:
* ���:
* ����:
********************************************************/
void CLogicalCluster::DeActiveMe()
{
    EndDeActiveMe();
}

/********************************************************
* ����: CLogicalCluster::EndDeActiveMe
* ����: ȥ���������ɨβ����
* ����:
* ���:
* ����:
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
        //����HandOver��Ӧ�˲���
        EndHandOverResp();
        break;

    case OPR_FAILOVER_INIT:
        //����FailOver����˵�ȥ�������
        EndFailOverInitDeActive();
        break;

    case OPR_AGGRESSIVE_DEACTIVE:
        //��������ȥ�������
        m_LocalNodeMsg.nOperation = OPR_NONE;
        OnNotify(NOTIFY_AGGRESSIVE_DEACTIVE_COMPLETE);
        break;

    default:
        assert(0);
        break;
    }
}

/********************************************************
* ����: CLogicalCluster::BeginHandOverInit
* ����: ��ʼ����HandOver����˵Ĳ���
* ����:
* ���:
* ����:
********************************************************/
void CLogicalCluster::BeginHandOverInit()
{
    m_LocalNodeMsg.nOperation = OPR_HANDOVER_INIT;
}

/********************************************************
* ����: CLogicalCluster::BeginHandOverResp
* ����: ��ʼ����HandOver��Ӧ�˵Ĳ���
* ����:
* ���:
* ����:
********************************************************/
void CLogicalCluster::BeginHandOverResp()
{
    m_LocalNodeMsg.nOperation = OPR_HANDOVER_RESP;

    BeginDeActiveMe();
}

/********************************************************
* ����: CLogicalCluster::EndHandOverResp
* ����: HandOver��Ӧ�˽��������Ľӿ�
* ����:
* ���:
* ����:
********************************************************/
void CLogicalCluster::EndHandOverResp()
{
    m_LocalNodeMsg.nOperation = OPR_NONE;
}

/********************************************************
* ����: CLogicalCluster::ProcHandOverAns
* ����: HandOver����˴���HandOverӦ��Ľӿ�
* ����: pData   : ������ݵ�ָ��
*       uLength : ������ݵĳ���
* ���:
* ����:
********************************************************/
void CLogicalCluster::ProcHandOverAns(void* pData, UINT uLength)
{
    BeginActiveMe();
}

/********************************************************
* ����: CLogicalCluster::EndHandOverInit
* ����: HandOver��������HandOver�Ĺ���
* ����: bSucc : �����Ƿ�ɹ�
* ���:
* ����:
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
* ����: CLogicalCluster::BeginFailOverInit
* ����: ��ʼ����FailOver����˵Ĳ���
* ����:
* ���:
* ����:
********************************************************/
void CLogicalCluster::BeginFailOverInit()
{
    m_LocalNodeMsg.nOperation = OPR_FAILOVER_INIT;
    m_LocalNodeMsg.lActiveFailTimes++;

    BeginDeActiveMe();
}

/********************************************************
* ����: CLogicalCluster::EndFailOverInitDeActive
* ����: FailOver��������ȥ�������
* ����:
* ���:
* ����:
********************************************************/
void CLogicalCluster::EndFailOverInitDeActive()
{
    //������Ҫ�ڴ˺�����Զ˷�FAILOVER����
}

/********************************************************
* ����: CLogicalCluster::BeginFailOverResp
* ����: FailOver��Ӧ�˿�ʼ����
* ����: pData   : ������ݵ�ָ��
*       uLength : ������ݵĳ���
* ���:
* ����:
********************************************************/
void CLogicalCluster::BeginFailOverResp(void* pData, UINT4 uLength)
{
    m_LocalNodeMsg.nOperation = OPR_FAILOVER_RESP;

    BeginActiveMe();
}

/********************************************************
* ����: CLogicalCluster::EndFailOverResp
* ����: FailOver��Ӧ�˽�������
* ����: BOOL bSucc : �����Ƿ�ɹ�
* ���:
* ����:
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
* ����: CLogicalCluster::ProcFailOverAns
* ����: FailOver����˴���FailOverӦ��
* ����: bSucc : FailOver�����Ƿ�ɹ�
* ���:
* ����:
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
* ����: CLogicalCluster::CanFailOver
* ����: �ж��Ƿ��ܽ��й��ϵ���
* ����: EFAILOVERCAUSE_TYPE nCause : ����ԭ��
* ���:
* ����: int �Ƿ��ܼ�������
********************************************************/
int CLogicalCluster::CanFailOver(EFAILOVERCAUSE_TYPE nCause)
{
    //ֻ���ڼ���״̬ʱ���ܷ���FailOver����
    if(CLUSTER_ACTIVE != m_LocalNodeMsg.nStatus)
    {
         return ERR_FAIL;
    }

    //����õ�ǰ����ִ�н����Ժ���ִ��FailOver����
    if(OPR_NONE != m_LocalNodeMsg.nOperation)
    {
        return ERR_FAIL;
    }

    //����Զ�Ϊ���ϣ�����֪״̬�����ܵ���
    if((CLUSTER_FAILURE == m_PeerNodeMsg.nStatus) ||
       (CLUSTER_UNKNOWN == m_PeerNodeMsg.nStatus))
    {
         return ERR_FAIL;
    }

    //����Զ˵Ĳ���״̬Ǩ�Ʋ��Ϸ������ܵ���
    if(!g_nOprStatusArray[m_PeerNodeMsg.nOperation][OPR_FAILOVER_RESP])
    {
         return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

/********************************************************
* ����: public CLogicalCluster::DoFailOver
* ����: �����ṩ������й��ϵ����Ľӿ�
* ����: EFAILOVERCAUSE_TYPE nCause : ����ԭ��
* ���:
* ����: int �Ƿ��ѷ���FailOver���ϵ�������
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
* ����: CLogicalCluster::Timer1Sec
* ����: ���¼�������
* ����:
* ���:
* ����:
********************************************************/
void CLogicalCluster::Timer1Sec()
{
    //�����ϴμ�����ڵ�ʱ���
    m_LocalNodeMsg.lLastActiveGapTime = time(NULL) - m_tLastActiveTime;

    //��������Ѽ���ɹ�����ʱ�䳬��10���ӣ�����������ʧ�ܼ���������
    if((m_LocalNodeMsg.lLastActiveGapTime > SUCCESS_ACTIVE_INV_TIME) &&
       (CLUSTER_ACTIVE == m_LocalNodeMsg.nStatus))
    {
        m_LocalNodeMsg.lActiveFailTimes = 0;
    }

    //������������ʱ����HandOver����
    if((OPR_NONE == m_LocalNodeMsg.nOperation) &&
       (CLUSTER_INITIAL == m_LocalNodeMsg.nStatus) &&
       (SM_AUTO_BACK == m_SwitchBackMode) &&
       (m_LocalNodeMsg.nNodeID < m_PeerNodeMsg.nNodeID) &&
       (m_PeerNodeMsg.nNodeID != INVALID_NODE_ID))
    {
        //����HandOver����˲���
        m_LocalNodeMsg.nStatus = CLUSTER_DEACTIVE;
        BeginHandOverInit();
    }

    //�Զ˹���,�ұ���ȥ����,����HandOver����
    if((OPR_NONE == m_LocalNodeMsg.nOperation) &&
       ((CLUSTER_DEACTIVE == m_LocalNodeMsg.nStatus) ||
        (CLUSTER_INITIAL == m_LocalNodeMsg.nStatus)) &&
       (CLUSTER_FAILURE == m_PeerNodeMsg.nStatus) &&
       (m_LocalNodeMsg.lLastActiveGapTime > m_lActiveInterval))
    {
        //����HandOver����˲���
        m_LocalNodeMsg.nStatus = CLUSTER_DEACTIVE;
        BeginHandOverInit();
    }

    /*
      �����˶�δ�����δ���ϣ���ֻ��˭���ϴμ����ʱ�������
      �����ͬ��������������������
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
            //����HandOver����˲���
            m_LocalNodeMsg.nStatus = CLUSTER_DEACTIVE;
            BeginHandOverInit();
       }
       else if((m_LocalNodeMsg.lLastActiveGapTime ==
                m_PeerNodeMsg.lLastActiveGapTime) &&
               (m_LocalNodeMsg.nNodeID < m_PeerNodeMsg.nNodeID))
       {
            //����HandOver����˲���
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

        //ִ������ȥ��������
        BeginDeActiveMe();
    }

    //����ϵͳ���������ԣ��Ż�Ϊ��һ��Ϊ����״̬ʱ��
    //������ֶԶ˼���ɹ������Լ���ΪDeActive״̬
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
* ����: CLogicalCluster::OnNotify
* ����: ˫���¼���Ϣ������
* ����:
*       ECLUSTER_NOTIFY uMsg : �¼���Ϣ��
*       UINT4 uParam1 = 0    : ����һ�������ǹ��϶����ָ��
*       UINT4 uParam2 = 0    : ����������������
* ���:
* ����: void
********************************************************/
void CLogicalCluster::OnNotify(ECLUSTER_NOTIFY uMsg,
                               UINT4 uParam1,
                               UINT4 uParam2)
{
    //
}
