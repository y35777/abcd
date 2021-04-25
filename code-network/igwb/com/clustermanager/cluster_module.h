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
    ��Ա����
*/
public:

    CClusterModule();
    virtual ~CClusterModule();

    //��ȡ״̬ͬ������
    int GetSyncData(void*& pData, int& nLen, BOOL bReadFile=TRUE);
    //ʵʩ״̬���ݵ�ͬ������
    void OnDoSyncData(void* pData, int nLen);
    //��������ӿ�
    int Active();
    //ȥ��������ӿ�
    int DeActive();

protected:

    //��ʼ������
    virtual int InitInstance();
    //�˳�ǰ���������
    virtual void ExitInstance();
    //��ȡģ������
    virtual MODULE_TYPE GetModuleType();
    //��Ϣ�ַ�����
    virtual void ParseMsg(MSG_INTRA* pMsgParsed);
    //�����ҵ��ģ�鷢����ͬ������
    void OnRecvSyncData(SSyncFrame* pFrame);
    //�����ֶ��л�����
    void OnManualOver(MSG_INTRA* pMsgParsed);
	//��������������
    void OnReset(MSG_INTRA* pMsgParsed);
    //������ϵ�������
    void OnAskFailOver(SFailOverCause* pCause);
    //����˫��״̬������Ϣ
    void OnReqStatus();
    //��ʱ������
    virtual void OnTimer(const long nTimerID);
    //1���¼�������
    void Timer1Sec();
    //��Զ˷���ͬ������֡
    void SendSyncData(BOOL bReadFile);

/*
    ��Ա����
*/
protected:

    //˫�������ָ��
    CComCluster*        m_pComCluster;

    //�첽��Դ�����ָ��
    CAsynResKernel*     m_pAsynResKernel;

    //�������е�Сͼ������ָ��
    CShellShow*         m_pShellShow;

    //ͬ����������ߵ�ָ��
    CSyncManager*       m_pSyncManager;

    //��ʼ����KERNEL��ʱ��
    time_t              m_tBeginActiveTime;

    //��󼤻�ʱ��
    long                m_lMaxActiveTime;

    //��ʼȥ����KERNEL��ʱ��
    time_t              m_tBeginDeActiveTime;

    //���ȥ����ʱ��
    long                m_lMaxDeActiveTime;

    //1�붨ʱ��ID
    long                m_lTimer1Sec;

    //˫�������˳����Ƶ�����������("iGateway bill V200 Cluster Exit Mutex")��
    //����û����ø���������������1���ӣ�˫�����̽�ִ���˳�����
    ACE_Process_Mutex*  m_pExitMutex;

    //˫��������ʾͼ����Ƶ�����������("iGateway bill V200 Cluster ShowIcon Mutex")��
    //����û����ø���������������1���ӣ�˫�����̽�������ʾIcon
    ACE_Process_Mutex*  m_pShowIconMutex;

    //�ڵ��
    int                 m_nNodeID;

    //�˳��ı�־
    BOOL                m_bExit;

    //����ͬ�����ݶ�����̰߳�ȫ��
    ACE_Mutex           m_SyncDataMutex;

    //�Ƿ񼤻�
    BOOL                m_bActive;

};

#endif //__CLUSTER_MODULE_H__
