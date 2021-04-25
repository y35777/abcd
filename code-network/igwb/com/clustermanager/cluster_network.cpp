#include "../include/frame.h"
#include "com_cluster.h"
#include "cluster_network.h"
#include "heartbeat_link.h"
#include "cluster_mutex.h"
#include "cluster_commtask.h"
#include "heartbeat_creator.h"

/********************************************************
* ����: CClusterNetwork::CClusterNetwork
* ����: ���캯��
* ����: CComCluster* pCluster : CComCluster�����ָ��
* ���:
* ����:
********************************************************/
CClusterNetwork::CClusterNetwork(CComCluster* pCluster)
{
    assert(NULL != pCluster);

    m_pHeartBeatList    = NULL;
    m_pCluster          = pCluster;
    m_pCommTask         = NULL;
}

/********************************************************
* ����: CClusterNetwork::~CClusterNetwork
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CClusterNetwork::~CClusterNetwork()
{
    ExitInstance();
}

/********************************************************
* ����: CClusterNetwork::InitInstance
* ����: ��ʼ������
* ����:
* ���:
* ����: int ���س�ʼ���Ƿ�ɹ���ʧ�ܷ��ط���ֵ���ɹ�����0
********************************************************/
int CClusterNetwork::InitInstance()
{
    m_pCommTask = new CClusterCommTask(this);

    if(!m_pCluster->GetbSingleServer())
    {//˫��ģʽ

        //��������ʼ��������������
        m_pHeartBeatList = CHeartBeatCreator::CreateHeartBeatList(this);
        if(NULL == m_pHeartBeatList)
        {
            return ERR_HEARTBEAT_CREATE;
        }
    }

    //����m_pCommTask����
    int nRet = m_pCommTask->open();
    if(ERR_SUCCESS != nRet)
    {
        return ERR_COMMTASK_OPEN;
    }

    if(!m_pCluster->GetbSingleServer())
    {//˫��ģʽ

        //������������̡߳�
        nRet = m_ThrdMgr.spawn((ACE_THR_FUNC)ThreadProc, (void *)this);
        if(-1 == nRet)
        {
            return ERR_HB_THREAD_SPAWN;
        }
    }

    return ERR_SUCCESS;
}

/********************************************************
* ����: CClusterNetwork::ExitInstance
* ����: ����CClusterNetwork���˳�ǰ��ɨβ����
* ����:
* ���:
* ����: void
********************************************************/
void CClusterNetwork::ExitInstance()
{
    //ʹ����ͨѶ���߳��˳�
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
* ����: CClusterNetwork::StopCommThread
* ����: ֹͣ����ͨѶ���߳�
* ����:
* ���:
* ����: void
********************************************************/
void CClusterNetwork::StopCommThread()
{
    m_ExitEvent.signal();

    //����timeoutΪ����ʱ�䣬�ȴ���������߳��˳�
    ACE_Time_Value timeout = ACE_OS::gettimeofday();
    timeout += ACE_Time_Value(10);
    m_ThrdMgr.wait(&timeout);

    if(NULL != m_pCommTask)
    {
        //�ȴ�CCommTask���¼�����߳��˳�
        m_pCommTask->QuitCommTask();
    }
}

/********************************************************
* ����: CClusterNetwork::Send
* ����: ��Զ˷�����Ϣ
* ����: SClusterFrame* pFrame : ˫����Ϣ֡�ṹ��ָ��
* ���:
* ����: int �����Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CClusterNetwork::Send(SClusterFrame* pFrame)
{
    //pFrame���ȴ��������֡����
    if(pFrame->nLen > CLUSTER_FRAME_MAXSIZE - CLUSTER_FRAME_HEADLENGTH)
    {
        return ERR_SEND_LENGTH_EXCCEED;
    }

    if(NULL != m_pHeartBeatList)
    {
        //��m_pHeartBeatList�в��ҵ�һ��ָ�벻Ϊ�գ�
        //����GetbIsAvailable()����ΪTRUE����������;
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
* ����: CClusterNetwork::OnReceive
* ����: ����Զ˷�������Ϣ
* ����: SClusterFrame* pFrame : ˫����Ϣ֡�ṹ��ָ��
* ���:
* ����: void
********************************************************/
void CClusterNetwork::OnReceive(SClusterFrame* pFrame)
{
    if(NULL != m_pCluster)
    {
        //��˫�������ύ�Զ˷�������Ϣ
        m_pCluster->OnReceive(pFrame);
    }
}

/********************************************************
* ����: CClusterNetwork::GetLinkCount
* ����: ��ȡ�ܵ�������
* ����:
* ���:
* ����: int �ܵ�������
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
* ����: CClusterNetwork::GetActiveLinkCount
* ����: ��ȡ��ǰ���õ�����
* ����:
* ���:
* ����: int ��ǰ���õ�������
********************************************************/
int CClusterNetwork::GetActiveLinkCount()
{
    int nLinkCount = 0;

    if(NULL != m_pHeartBeatList)
    {
        //��m_pHeartBeatList�в���ָ�벻Ϊ�գ�����GetbIsAvailable()
        //����ΪTRUE������������nLinkCountΪ����������������;
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
* ����: CClusterNetwork::ThreadProc
* ����: ��������̺߳���
* ����: CClusterNetwork* pNetwork : CClusterNetwork�����ָ��
* ���:
* ����: void
********************************************************/
void CClusterNetwork::ThreadProc(CClusterNetwork* pNetwork)
{
    pNetwork->TestLink();
}

/********************************************************
* ����: CClusterNetwork::TestLink
* ����: ͨ��������֡��������������Ŀ�����
* ����:
* ���:
* ����: void
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
    //��ʾ��ʱ
    {
        m_SendMutex.acquire();
        //���ε���m_pHeartBeatList���������������Send��������������֡;
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
* ����: CClusterNetwork::RegisterHandle
* ����: ��ACE_Event_Handler����Ķ���ע�ᵽCCommTask�����
*       ACE_Reactor������
* ����: ACE_Event_Handler* pSocket : ��ע���Socket�����ָ��
*       ACE_Reactor_Mask mask      : ��ע����¼�
* ���:
* ����: int ע���Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾ������
********************************************************/
int CClusterNetwork::RegisterHandle(ACE_Event_Handler* pSocket,
                                    ACE_Reactor_Mask mask)
{
    int nRet = ERR_FAIL;

    //����m_pCommTask�����RegisterHandle�ӿ�ע��Socket������¼�
    if(NULL != m_pCommTask)
    {
        nRet = m_pCommTask->RegisterHandle(pSocket, mask);
    }

    return nRet;
}

/********************************************************
* ����: CClusterNetwork::Timer1Sec
* ����: �붨ʱ��
* ����:
* ���:
* ����: void
********************************************************/
void CClusterNetwork::Timer1Sec()
{
    CClusterMutex::Instance()->Acquire();

    BOOL bAvailable;

    //�ж�������·�Ƿ��ж�
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

    //����CComCluster�����¼�������
    if (NULL != m_pCluster)
    {
        m_pCluster->Timer1Sec();
    }

    CClusterMutex::Instance()->Release();
}

/********************************************************
* ����: CClusterNetwork::OnNotify
* ����: ˫���¼���Ϣ������
* ����: ECLUSTER_NOTIFY uMsg : �¼���Ϣ��
*       UINT4 uParam1        : ����һ�������ǹ��϶����ָ��
*       UINT4 uParam2        : ����������������
* ���:
* ����:
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
