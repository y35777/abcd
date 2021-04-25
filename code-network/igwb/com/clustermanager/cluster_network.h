#ifndef __CLUSTER_NETWORK_H__
#define __CLUSTER_NETWORK_H__

#include "cluster_struct.h"

class CHeartBeatLink;
class CComCluster;
class CClusterCommTask;

class CClusterNetwork
{
/*
    ��Ա����
*/
public:
    CClusterNetwork(CComCluster* pCluster);
    virtual ~CClusterNetwork();

    //��ʼ������
    int InitInstance();
    //��Զ˷�����Ϣ
    int Send(SClusterFrame* pFrame);
    //����Զ˷�������Ϣ
    virtual void OnReceive(SClusterFrame* pFrame);
    //��ȡ�ܵ�������
    int GetLinkCount();
    //��ȡ��ǰ���õ�����
    int GetActiveLinkCount();
    //��ACE_Event_Handler����Ķ���ע�ᵽCCommTask�����ACE_Reactor������
    int RegisterHandle(ACE_Event_Handler* pSocket, ACE_Reactor_Mask mask);
    //�붨ʱ��
    void Timer1Sec();
    //˫���¼���Ϣ������
    virtual void OnNotify(ECLUSTER_NOTIFY uMsg,
                          UINT4 uParam1 = 0,
                          UINT4 uParam2 = 0);

protected:
    //����CClusterNetwork���˳�ǰ��ɨβ����
    void ExitInstance();
    //ֹͣ����ͨѶ���߳�
    void StopCommThread();
    //��������̺߳���
    static void ThreadProc(CClusterNetwork* pNetwork);
    //ͨ��������֡��������������Ŀ�����
    void TestLink();

/*
    ��Ա����
*/
protected:
    //���������б��ָ��
    LIST <CHeartBeatLink*>* m_pHeartBeatList;

    //Cluster�����ָ��
    CComCluster*            m_pCluster;

    //CCommTask�����ָ��
    CClusterCommTask*       m_pCommTask;

    //ACE���̹߳������
    ACE_Thread_Manager      m_ThrdMgr;

    //��������߳��˳��¼�
    ACE_Event               m_ExitEvent;

    //��Ϣ����ʱ�õĻ�����
    ACE_Mutex               m_SendMutex;
};

#endif //__CLUSTER_NETWORK_H__
