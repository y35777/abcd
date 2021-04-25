#ifndef __LOGICAL_CLUSTER_H__
#define __LOGICAL_CLUSTER_H__

#include "cluster_struct.h"

//˫������״̬
enum ECLSTR_STATUS
{
    CLUSTER_INITIAL     = 0,    //Cluster���ڸ�������ĳ�ʼ��״̬
    CLUSTER_DEACTIVING,         //Cluster����ȥ���������
    CLUSTER_DEACTIVE,           //Cluster����ȥ����״̬
    CLUSTER_ACTIVING,           //Cluster���ڼ��������
    CLUSTER_ACTIVE,             //Cluster���ڼ���״̬
    CLUSTER_FAILURE,            //Cluster���ڹ���״̬
    CLUSTER_UNKNOWN             //Cluster����δ֪״̬
};
#define MIN_CLSTR_STATUS    CLUSTER_INITIAL
#define MAX_CLSTR_STATUS    CLUSTER_UNKNOWN

//˫������״̬
enum ECLSTR_OPERATION
{
    OPR_NONE            = 0,    //û�н����κβ���
    OPR_AGGRESSIVE_ACTIVE,      //�����ж�ʱ�����������������
    OPR_AGGRESSIVE_DEACTIVE,    //����ȥ��������������˳�ʱ
    OPR_FAILOVER_INIT,          //ʧЧ�л������ķ����
    OPR_FAILOVER_RESP,          //ʧЧ�л���������Ӧ��
    OPR_HANDOVER_INIT,          //HandOver�����ķ����
    OPR_HANDOVER_RESP           //HandOver��������Ӧ��
};
#define MIN_CLSTR_OPERATION     OPR_NONE
#define MAX_CLSTR_OPERATION     OPR_HANDOVER_RESP

//�ڵ���Ϣ�ṹ
struct SNODE_MSG
{
#define INVALID_NODE_ID     0xFFFFFFF
    int                 nNodeID;                //���ڵ��ID
    ECLSTR_STATUS       nStatus;                //˫������״̬
    ECLSTR_OPERATION    nOperation;             //˫������״̬
    long                lLastActiveGapTime;     //�ϴμ���ʱ��
    long                lActiveFailTimes;       //���Ӽ���ʧ�ܴ���
};

//�л�ģʽ
enum ESWITCH_MODE
{
    SM_AUTO_BACK = 0,           //�Զ�����
    SM_NONE_BACK                //�������Զ�����
};

class CLogicalCluster
{
/*
    ��Ա����
*/
public:

    //��ȡ�ڵ��
    int GetNodeID();
    //�����ṩ������й��ϵ����Ľӿ�
    int DoFailOver(EFAILOVERCAUSE_TYPE nCause);
    //���¼�������
    virtual void Timer1Sec();
    //˫���¼���Ϣ������
    virtual void OnNotify(ECLUSTER_NOTIFY uMsg,
                          UINT4 uParam1 = 0,
                          UINT4 uParam2 = 0);

protected:

    CLogicalCluster();
    virtual ~CLogicalCluster();

    virtual int InitInstance() = 0;         //��ʼ������

    //�������
    void BeginActiveMe();                   //���𼤻����
    virtual int ActiveMe();                 //�������
    virtual void EndActiveMe(BOOL bSucc);   //���������ɨβ����
    virtual int CanActiveMe();              //�Ƿ��ܽ��м������

    //ȥ�������
    void BeginDeActiveMe();                 //����ȥ�������
    virtual void DeActiveMe();              //ȥ�������
    virtual void EndDeActiveMe();           //ȥ���������ɨβ����

    //HandOver����
    virtual void BeginHandOverInit();       //��ʼ����HandOver����˵Ĳ���
    virtual void BeginHandOverResp();       //��ʼ����HandOver��Ӧ�˵Ĳ���
    virtual void EndHandOverResp();         //HandOver��Ӧ�˽��������Ľӿ�
    //HandOver����˴���HandOverӦ��
    virtual void ProcHandOverAns(void* pData, 
                                 UINT uLength);
    //HandOver��������HandOver����
    virtual void EndHandOverInit(BOOL bSucc);

    //FailOver����
    virtual void BeginFailOverInit();       //��ʼ����FailOver����˵Ĳ���
    virtual void EndFailOverInitDeActive(); //FailOver��������ȥ�������
    //FailOver��Ӧ�˿�ʼ����
    virtual void BeginFailOverResp(void* pData, UINT4 uLength);
    //FailOver��Ӧ�˽�������
    virtual void EndFailOverResp(BOOL bSucc);
    //FailOver����˴���FailOverӦ��
    virtual void ProcFailOverAns(BOOL bSucc);
    //�ж��Ƿ��ܽ��й��ϵ���
    virtual int CanFailOver(EFAILOVERCAUSE_TYPE nCause);


/*
    ��Ա����
*/
    //����״̬������
    static const char* const g_szStatusDesp[MAX_CLSTR_STATUS + 1];

    //����״̬������
    static const char* const g_szOperDesp[MAX_CLSTR_OPERATION + 1];

protected:
    //���ڵ����Ϣ
    SNODE_MSG       m_LocalNodeMsg;

    //�Զ˽ڵ����Ϣ
    SNODE_MSG       m_PeerNodeMsg;

    //���μ���ʱ��֮��Ӧ���ڵ������ֵ(�κ���������ļ��������Ҫ�����ж�)
    long            m_lActiveInterval;
#define ACTIVE_INTERVAL_TIME_DEFAULT    120
#define SUCCESS_ACTIVE_INV_TIME         600

    //�л�����
    ESWITCH_MODE    m_SwitchBackMode;

    //��󼤻�ʧ�ܴ�����������ֵ��״̬����Ϊ����
    long            m_lMaxActiveFailTimes;
#define MAX_ACTIVE_FAIL_TIMES_DEFAULT   3

    //�ϴμ������ʱ��
    time_t          m_tLastActiveTime;

    //˫������״̬Ǩ�ƾ���
    static const int g_nClsStatusArray[MAX_CLSTR_STATUS + 1]
                                      [MAX_CLSTR_STATUS + 1];

    //˫������״̬Ǩ�ƾ���
    static const int g_nOprStatusArray[MAX_CLSTR_OPERATION + 1]
                                      [MAX_CLSTR_OPERATION + 1];
};

#endif //__LOGICAL_CLUSTER_H__