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
* ����: CComCluster::CComCluster
* ����: ���캯��
* ����:
* ���:
* ����:
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

    //��������ָ���ʼ��ʱ���븳Ϊ��Чֵ������Ϊ��
    m_pNetwork          = NULL;
    m_pResManager       = NULL;

    //��������ָ������ʱ��һ����Ч����ʹ��ʱ��Ҫ���
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
* ����: CComCluster::~CComCluster
* ����: ����������
* ����:
* ���:
* ����:
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
* ����: CComCluster::InitInstance
* ����: ����CComCluster��ĳ�ʼ������
* ����:
* ���:
* ����: int ������г�ʼ�������ɹ����򷵻�0�����򷵻ط���ֵ
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
                   Ϊ�����ϴ�δ�����˳��ı������нϸߵļ������ȼ���
                   ������ϴμ���ļ��ʱ���޸�Ϊ���
                */
                m_tLastActiveTime      = 0;
                m_LocalNodeMsg.lLastActiveGapTime = time(NULL);
                m_LocalNodeMsg.nNodeID = -1;
            }
        }
    }

    //��������ʼ��m_pResManager����
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

    //����ģʽ
    if(GetbSingleServer())
    {
        m_bHeartBeatBroken = TRUE;
        m_nMaxHBBrokenTime = -1;
    }

    //��������ʼ��m_pNetwork����
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

    //��Զ˷��ͽڵ���Ϣ��������ΪCMD_CLS_HEARTBEAT������Ϊm_LocalNodeMsg���ڴ濽��
    SClusterFrame* pFrame = new(sizeof(m_LocalNodeMsg)) SClusterFrame;
    pFrame->nCmdCode      = CMD_CLS_HEARTBEAT;
    memcpy(pFrame->Data, (BYTE*)&m_LocalNodeMsg, sizeof(m_LocalNodeMsg));
    Send(pFrame);

    return ERR_SUCCESS;
}

/********************************************************
* ����: CComCluster::Instance
* ����: ����CComCluster��ĵ�ʵ��������
* ����: CAsynResObject* pObject : �첽�����ָ��
*       CClusterAlarm* pAlarm   : �澯�����ָ��
* ���:
* ����: CComCluster* ����CComCluster�ĵ�ʵ�������ָ��
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

    //����CComCluster�ĵ�ʵ��ָ��g_pComCluster
    return g_pComCluster;
}

/********************************************************
* ����: CComCluster::Destroy
* ����: ����CComCluster��ĵ�ʵ�����ٹ���
* ����:
* ���:
* ����:
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
* ����: CComCluster::ReportMsg
* ����: ���浱ǰ��Ϣ
* ����: 
* ���: 
* ����: void 
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
* ����: CComCluster::DoExit
* ����: �����˳�����
* ����:
* ���:
* ����:
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

        //��������ȥ�������
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
* ����: CComCluster::ActiveMe
* ����: �������
* ����:
* ���:
* ����: int �����Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CComCluster::ActiveMe()
{
    //����ͬ����Դ
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

    //�����첽��Դ
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
* ����: CComCluster::OnAsynResActived
* ����: �첽��Դ������ɵĽӿ�
* ����: bSucc : �첽��Դ�����Ƿ�ɹ���TRUE��ʾ�ɹ���FALSE��ʾʧ��
* ���:
* ����:
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
* ����: CComCluster::DeActiveMe
* ����: ȥ�������
* ����:
* ���:
* ����:
********************************************************/
void CComCluster::DeActiveMe()
{
    if(NULL != m_pAsynResObject)
    {
        m_pAsynResObject->DeActive();
    }

    //����ϴεĶ�̬��Դ״̬
    m_nLastDynResStatus = 0;
}

/********************************************************
* ����: CComCluster::EndDeActiveMe
* ����: ȥ���������ɨβ����
* ����:
* ���:
* ����:
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
        //����CAsynResObject����ռ��������ݵĽӿ�
        void *pData = 0;
        int  nLen   = 0;
        if (m_pAsynResObject != NULL)
        {
            m_pAsynResObject->GetAttachedData(pData, nLen);
        }

        /*
          ��֤�Զ˲����򱾶��޲�����Ϊȥ����״̬��������������
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
* ����: CComCluster::OnAsynResDeActived
* ����: �첽��Դȥ������ɵĽӿ�
* ����:
* ���:
* ����:
********************************************************/
void CComCluster::OnAsynResDeActived()
{
    int nRet = 0;
    if(NULL != m_pResManager)
    {
        //ȥ���̬��Դ
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
* ����: CComCluster::BeginHandOverInit
* ����: ��ʼ����HandOver����˵Ĳ���
* ����:
* ���:
* ����:
********************************************************/
void CComCluster::BeginHandOverInit()
{
    CLogicalCluster::BeginHandOverInit();

    //��Զ˷���HandOver���CMD_CLS_HANDOVER��
    SClusterFrame *pFrame = new(sizeof(SNODE_MSG)) SClusterFrame;
    pFrame->uSeqNum       = ++m_uInitSeqNum;
    pFrame->nCmdCode      = CMD_CLS_HANDOVER;
    memcpy(pFrame->Data, (BYTE*)&m_LocalNodeMsg, sizeof(SNODE_MSG));
    Send(pFrame);

    OnNotify(NOTIFY_HANDOVER);
}

/********************************************************
* ����: CComCluster::EndHandOverResp
* ����: HandOver��Ӧ�˽��������Ľӿ�
* ����:
* ���:
* ����:
********************************************************/
void CComCluster::EndHandOverResp()
{
    CLogicalCluster::EndHandOverResp();

    //����CAsynResObject����ռ��������ݵĽӿ�
    void *pData = 0;
    int  nLen   = 0;
    if (m_pAsynResObject != NULL)
    {
        m_pAsynResObject->GetAttachedData(pData, nLen);
    }

    //������״̬���ռ��ĸ������ݸ��ӵ�HandOverӦ��֡�У���Զ˷��ͳ�ȥ
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
* ����: CComCluster::ProcHandOverAns
* ����: HandOver����˴���HandOverӦ��Ľӿ�
* ����: pData   : ������ݵ�ָ��
*       uLength : ������ݵĳ���
* ���:
* ����:
********************************************************/
void CComCluster::ProcHandOverAns(void* pData, UINT4 uLength)
{
    //��CAsynResObject����HandOver�������;
    if((NULL != m_pAsynResObject) && (uLength > 0))
    {
        m_pAsynResObject->ProcAttachedData(pData, uLength);
    }

    //���ø����ProcHandOverAns����;
    CLogicalCluster::ProcHandOverAns(pData, uLength);
}

/********************************************************
* ����: CComCluster::EndFailOverInitDeActive
* ����: FailOver��������ȥ��������Ľӿ�
* ����:
* ���:
* ����:
********************************************************/
void CComCluster::EndFailOverInitDeActive()
{
    //���ø����EndFailOverInitDeActive����;
    CLogicalCluster::EndFailOverInitDeActive();

    //����CAsynResObject����ռ��������ݵĽӿ�;
    void* pData = NULL;
    int  nLen = 0;
    if(m_pAsynResObject != NULL)
    {
        m_pAsynResObject->GetAttachedData(pData, nLen);
    }

    //������״̬���ռ��ĸ������ݸ��ӵ�FailOver����֡��CMD_CLS_FAILOVER���У�
    //��Զ˷��ͳ�ȥ
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
* ����: CComCluster::BeginFailOverResp
* ����: FailOver��Ӧ�˿�ʼ�����Ľӿ�
* ����: pData   : ������ݵ�ָ��
*       uLength : ������ݵĳ���
* ���:
* ����:
********************************************************/
void CComCluster::BeginFailOverResp(void* pData, UINT4 uLength)
{
    //��CAsynResObject����FailOver�������
    if((NULL != m_pAsynResObject) && (uLength > 0))
    {
        m_pAsynResObject->ProcAttachedData(pData, uLength);
    }

    //���ø����BeginFailOverResp����
    CLogicalCluster::BeginFailOverResp(pData, uLength);
}

/********************************************************
* ����: CComCluster::EndFailOverResp
* ����: FailOver��Ӧ�˽��������Ľӿ�
* ����: BOOL bSucc : �����Ƿ�ɹ�
* ���:
* ����:
********************************************************/
void CComCluster::EndFailOverResp(BOOL bSucc)
{
    //���ø����EndFailOverResp����
    CLogicalCluster::EndFailOverResp(bSucc);

    //��Զ˷�����״̬��FailOver�Ƿ�ɹ���Ӧ��
    //��CMD_CLS_ANS_FAILOVER������ΪBOOL�ͣ�
    SClusterFrame *pFrame = new(sizeof(SNODE_MSG) +
                                sizeof(BOOL)) SClusterFrame;
    pFrame->uSeqNum       = m_uRespSeqNum;
    pFrame->nCmdCode      = CMD_CLS_ANS_FAILOVER;
    memcpy(pFrame->Data, (BYTE*)&m_LocalNodeMsg, sizeof(SNODE_MSG));
    memcpy(pFrame->Data + sizeof(SNODE_MSG), (BYTE*)&bSucc, sizeof(BOOL));

    Send(pFrame);
}

/********************************************************
* ����: CComCluster::CanFailOver
* ����: �ж��Ƿ��ܽ��й��ϵ���
* ����: EFAILOVERCAUSE_TYPE nCause : ����ԭ��
* ���:
* ����: int �Ƿ��ܷ���FailOver���ϵ�������
********************************************************/
int CComCluster::CanFailOver(EFAILOVERCAUSE_TYPE nCause)
{
    //���ø����CanFailOver����
    int nRet = CLogicalCluster::CanFailOver(nCause);
    if(ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    //��������жϣ��򷵻�ERR_HEART_BEAT_BROKEN;
    if(m_bHeartBeatBroken)
    {
        return ERR_HEART_BEAT_BROKEN;
    }

    //����Ǵ�����������У��ҹ���ԭ���Ǵ��������д����𻵣��򷵻�ʧ��
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
* ����: CComCluster::GetbSingleServer
* ����: �ж��Ƿ��ǵ���ģʽ
* ����:
* ���:
* ����: const BOOL �Ƿ��ǵ���
********************************************************/
const BOOL CComCluster::GetbSingleServer()
{
    return m_bSingleServer;
}

/********************************************************
* ����: CComCluster::GetbWaitForExit
* ����: �����Ƿ��ڵȴ��˳���״̬
* ����:
* ���:
* ����: const BOOL �����Ƿ��ڵȴ��˳���״̬�����Ƿ���TRUE��
*                  ���򷵻�FALSE
********************************************************/
const BOOL CComCluster::GetbWaitForExit()
{
    return  m_bWaitForExit;
}

/********************************************************
* ����: CComCluster::GetHeartBeatStatus
* ����: ����������״̬
* ����:
* ���:
* ����: UINT2 ����������״̬�����ֽ�Ϊ�������������ֽ�Ϊ
*             ��ǰ���õ�������
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
* ����: CComCluster::Timer1Sec
* ����: ���¼�������
* ����:
* ���:
* ����:
********************************************************/
void CComCluster::Timer1Sec()
{
    if(m_bWaitForExit)
    {
        return;
    }

    if(!m_bHeartBeatBroken)
    {
        //���m_pNetwork��ǰ�Ƿ��п��õ�����
        int nActiveLink = m_pNetwork->GetActiveLinkCount();
        if(0 == nActiveLink)
        {
            if(time(NULL) - m_tHBBrokenTime >= m_nMaxHBBrokenTime)
            //��Ϊ�����ж�
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

    //�����жϼ���
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
        //����첽��Դ��״̬�Ƿ����Ѽ���״̬
        if (NULL != m_pAsynResObject)
        {
            int nStatus = m_pAsynResObject->GetStatus();
            switch(nStatus)
            {
            case EASYNRES_ACTIVE:
            //�첽��Դ��״̬���Ѽ���״̬
                OnAsynResActived(TRUE);
                break;

            case EASYNRES_DEACTIVE:
            case EASYNRES_FAILURE:
            //�첽��Դ��״̬����ȥ����״̬�����״̬
                OnAsynResActived(FALSE);
                break;

            default:
                break;
            }
        }
        else
        //�������첽��Դ
        {
            OnAsynResActived(TRUE);
        }
        break;

    case CLUSTER_DEACTIVING:
        //����첽��Դ��״̬�Ƿ�����ȥ����״̬;
        if (NULL != m_pAsynResObject)
        {
            int nStatus = m_pAsynResObject->GetStatus();
            switch(nStatus)
            {
            case EASYNRES_DEACTIVE:
            case EASYNRES_FAILURE:
            //�첽��Դ��״̬����ȥ����״̬�����״̬
                OnAsynResDeActived();
                break;

            default:
                break;
            }
        }
        else
        //�������첽��Դ
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
                //��Զ˷�HandOver����������ط��������m_uInitSeqNum����
                SClusterFrame *pFrame = new(sizeof(SNODE_MSG)) SClusterFrame;
                pFrame->uSeqNum       = m_uInitSeqNum;
                pFrame->nCmdCode      = CMD_CLS_HANDOVER;
                memcpy(pFrame->Data, (BYTE*)&m_LocalNodeMsg, sizeof(SNODE_MSG));
                Send(pFrame);
            }
        }
    //ע��˴�û��break,��˵����FailOver����˴���ȥ��������״̬ʱ,��Ҫ�������²���
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
                //��Զ˷�FailOver����������ط��������m_uInitSeqNum����
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
    
    //���ø����Timer1Sec����
    CLogicalCluster::Timer1Sec();

    //��Զ˷��ͽڵ���Ϣ��������ΪCMD_CLS_HEARTBEAT������Ϊm_LocalNodeMsg���ڴ濽��
    SClusterFrame* pFrame = new(sizeof(m_LocalNodeMsg)) SClusterFrame;
    pFrame->nCmdCode      = CMD_CLS_HEARTBEAT;
    memcpy(pFrame->Data, (BYTE*)&m_LocalNodeMsg, sizeof(m_LocalNodeMsg));
    Send(pFrame);

    //��m_pResManager����һ����һ�ε�״̬��⼰�������м��
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

    //����CClusterAlarm������붨ʱ������
    if(NULL != m_pClusterAlarm)
    {
        m_pClusterAlarm->Timer1Sec();
    }
}

/********************************************************
* ����: CComCluster::CheckResStatus
* ����: ����Դ״̬״̬
* ����: 
* ���:
* ����: int �Ƿ���������0��ʾδ����������ֵ��ʾ����
********************************************************/
int CComCluster::CheckResStatus()
{
    int nResStatus = 0;
    if(CLUSTER_ACTIVE == m_LocalNodeMsg.nStatus)
    {
        //��⶯̬��Դ
        nResStatus = m_pResManager->CheckStatus(RT_DYNAMIC);

        if(m_nLastDynResStatus != nResStatus)
        {
            if(NULL != m_pClusterAlarm)
            {
                for(int i = 1; i <= MAX_RES_NUM; i++)
                {
                    //��ԴʧЧ�澯
                    if(((1 << (i - 1)) & nResStatus) && !((1 << (i - 1)) & m_nLastDynResStatus))
                    {
                        CResObject* pResObject = m_pResManager->GetObject(i);
                        m_pClusterAlarm->OnNotify(NOTIFY_RESOURCE_FAIL, 
                                                  (UINT4)pResObject);
                    }
                    //��Դ�ָ��澯
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

        //����Ƿ���Ҫ����
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

        //����첽��Դ
        if ((NULL != m_pAsynResObject) && 
            (m_pAsynResObject->CheckStatus() != ERR_SUCCESS))
        {
            OnNotify(NOTIFY_ASYN_RES_FAIL);
            return 1;
        }
    }

    //��⾲̬��Դ
    nResStatus = m_pResManager->CheckStatus(RT_STATIC);

    if(m_nLastStaticResStatus != nResStatus)
    {
        if(NULL != m_pClusterAlarm)
        {
            for(int i = 1; i <= MAX_RES_NUM; i++)
            {
                //��ԴʧЧ�澯
                if(((1 << (i - 1)) & nResStatus) && !((1 << (i - 1)) & m_nLastStaticResStatus))
                {
                    CResObject* pResObject = m_pResManager->GetObject(i);
                    m_pClusterAlarm->OnNotify(NOTIFY_RESOURCE_FAIL, 
                                              (UINT4)pResObject);
                }
                //��Դ�ָ��澯
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
        //����Ƿ���Ҫ����
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

    //���Ӳ��
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
* ����: CComCluster::Send
* ����: ��Զ˷�����Ϣ
* ����: SClusterFrame* pFrame : ˫����Ϣ֡
* ���:
* ����: int �����Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
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
* ����: CComCluster::OnReceive
* ����: ����Զ˷��͵�˫����Ϣ
* ����: SClusterFrame* pFrame : ˫����Ϣ֡
* ���:
* ����: void
********************************************************/
void CComCluster::OnReceive(SClusterFrame* pFrame)
{
    if(m_bWaitForExit)
    {
        return;
    }

    //���¶Զ���Ϣ
    if((CMD_CLS_LOGICAL_MINCODE <= pFrame->nCmdCode) &&
       (CMD_CLS_LOGICAL_MAXCODE >= pFrame->nCmdCode))
    {
        memcpy((BYTE*)&m_PeerNodeMsg, pFrame->Data, sizeof(SNODE_MSG));
        switch(m_PeerNodeMsg.nStatus)
        {
        case CLUSTER_ACTIVING:
        case CLUSTER_ACTIVE:
            /*
                ����Զ˴��ڼ�������ڼ���״̬�����ұ��˴��ڳ�ʼ��״̬�����
                    1.�Ե�ģʽ��,
                    2.�ǶԵ�ģʽ�£�������˽ڵ�Ŵ��ڶԶ�,
                �򽫱���״̬Ǩ�Ƶ�ȥ����״̬��
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

    //��������Ƿ��ѻָ�
    if(m_bHeartBeatBroken)
    {
        OnNotify(NOTIFY_HEARTBEAT_RECOVERY);
        m_bHeartBeatBroken = FALSE;
    }
    m_tHBBrokenTime = time(NULL);

    //�Զ�״̬Ϊ��ʼ��״̬
    if(CLUSTER_INITIAL == m_PeerNodeMsg.nStatus)
    {
        OnNotify(NOTIFY_PEERNODE_RESTART);
    }

    switch(pFrame->nCmdCode)
    {
    case CMD_CLS_HANDOVER:
    //�յ�HandOver����
        /*
            �ڷǶԵ�ģʽ�£����ڵ㴦�ڳ�ʼ��״̬ʱ������Ǩ�Ƶ�HandOver��Ӧ��״̬
        */
        if((SM_AUTO_BACK == m_SwitchBackMode) && 
           (CLUSTER_INITIAL == m_LocalNodeMsg.nStatus) &&
           (m_LocalNodeMsg.nNodeID < m_PeerNodeMsg.nNodeID))
        {
            break;
        }

        /*
            ������Խ��в���״̬Ǩ�ƵĿ����������֣�
            1������״̬Ǩ�ƾ�������Ǩ�ƣ�
            2����������£�����ͬʱ����HandOver������
               ��ڵ�Žϴ��һ��Ǩ�Ƶ���Ӧ��״̬
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
    //�յ�HandOverӦ��
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
    //�յ�FailOver����
        if(g_nOprStatusArray[m_LocalNodeMsg.nOperation][OPR_FAILOVER_RESP])
        {
            m_uRespSeqNum = pFrame->uSeqNum;
            TRACE(MTS_CLSTR, S_RECV_FAILOVER_CMD);
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, S_RECV_FAILOVER_CMD);
            BeginFailOverResp(pFrame->Data + sizeof(SNODE_MSG),
                              pFrame->nLen - sizeof(SNODE_MSG));
        }
        //��������޲������Ѽ��˵�������ϴη���Ӧ���ˣ�����ֱ�ӷ���Ӧ��
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
    //�յ�FailOverӦ��
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
    //������Ϣ֡����ǰ����������
        break;

    case CMD_CLS_ASKFOR_HANDOVER:
    //�յ�CMD_CLS_ASKFOR_HANDOVER,�����ǰ����״̬Ϊ��ʼ����ȥ����״̬,
    //����״̬ΪOPR_NONE,�����BeginHandOverInit����,����HandOver����
        if((g_nOprStatusArray[m_LocalNodeMsg.nOperation][OPR_HANDOVER_INIT]) &&
           ((CLUSTER_DEACTIVE == m_LocalNodeMsg.nStatus) ||
           (CLUSTER_INITIAL == m_LocalNodeMsg.nStatus)))
        {
            BeginHandOverInit();
        }
        break;

    case CMD_CLS_ASKFOR_ACTIVE:
    //�յ�CMD_CLS_ASKFOR_ACTIVE,���ʾ�Զ˼����˳�,�����ǰ����״̬Ϊ
    //��ʼ����ȥ����״̬,�򴥷������������
        if((CLUSTER_DEACTIVE == m_LocalNodeMsg.nStatus) ||
           (CLUSTER_INITIAL == m_LocalNodeMsg.nStatus))
        {
            //��CAsynResObject����FailOver�������
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
* ����: CComCluster::DoManualOver
* ����: �����ֶ�����
* ����:
* ���:
* ����: int �Ƿ��ܼ��������ֶ�����
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
* ����: CComCluster::OnNotify
* ����: ˫���¼���Ϣ������
* ����: ECLUSTER_NOTIFY uMsg : �¼���Ϣ��
*       UINT4 uParam1        : ����һ�������ǹ��϶����ָ��
*       UINT4 uParam2        : ����������������
* ���:
* ����:
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
    //ֹͣ��Դʧ��
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
            //��������ϵͳ
            Reboot();
        }
        break;
    }
    case NOTIFY_RESOURCE_FAIL:
    //��Դʧ�ܵ�֪ͨ������
        DoFailOver(ERROR_RESOURCE);
        break;

    case NOTIFY_ASYN_RES_FAIL:
    //�첽��Դʧ�ܵ�֪ͨ������
        if (ERR_SUCCESS != DoFailOver(ERROR_PROCESS_EXCEPTION))
        {
            m_LocalNodeMsg.nOperation = OPR_AGGRESSIVE_DEACTIVE;

            OnNotify(NOTIFY_AGGRESSIVE_DEACTIVE);

            //ִ������ȥ��������
            BeginDeActiveMe();
        }
        break;

    case NOTIFY_HARDDISK_FAIL:
    //����Ӳ�̹���
        DoFailOver(ERROR_HARDDISK);
        break;

    default:
        break;
    }
}
