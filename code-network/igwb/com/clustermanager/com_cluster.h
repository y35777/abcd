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
    ��Ա����
*/
public:

    //����CComCluster��ĵ�ʵ��������
    static CComCluster* Instance(CAsynResObject* pObject = NULL,
                                 CClusterAlarm* pAlarm = NULL);
    //����CComCluster��ĵ�ʵ�����ٹ���
    static void Destroy();

    //�����˳�����
    void DoExit();
    //�ж��Ƿ��ǵ���ģʽ
    const BOOL GetbSingleServer();
    //�����Ƿ��ڵȴ��˳���״̬
    const BOOL GetbWaitForExit();
    //����������״̬
    UINT2 GetHeartBeatStatus();
    //���¼�������
    virtual void Timer1Sec();
    //��Զ˷�����Ϣ
    int Send(SClusterFrame* pFrame);
    //����Զ˷��͵�˫����Ϣ
    virtual void OnReceive(SClusterFrame* pFrame);
    //�����ֶ�����
    int DoManualOver(STRING& strErrInfo, STRING& strDetailInfo);
    //˫���¼���Ϣ������
#define NOT_NEED_ALARM     0xFFFFFFFF
    virtual void OnNotify(ECLUSTER_NOTIFY uMsg,
                          UINT4 uParam1 = 0,
                          UINT4 uParam2 = 0);

    void SetStatusFile(const char* szStatusFile);
protected:

    CComCluster();
    virtual ~CComCluster();

    //����CComCluster��ĳ�ʼ������
    virtual int InitInstance();

    //���浱ǰ��Ϣ
    void ReportMsg();

    //�������
    virtual int ActiveMe();
    //���������ɨβ����
    virtual void EndActiveMe(BOOL bSucc);
    //�첽��Դ������ɵĽӿ�
    virtual void OnAsynResActived(BOOL bSucc);
    //ȥ�������
    virtual void DeActiveMe();
    //ȥ���������ɨβ����
    virtual void EndDeActiveMe();
    //�첽��Դȥ������ɵĽӿ�
    virtual void OnAsynResDeActived();
    //��ʼ����HandOver����˵Ĳ���
    virtual void BeginHandOverInit();
    //HandOver��Ӧ�˽��������Ľӿ�
    virtual void EndHandOverResp();
    //HandOver����˴���HandOverӦ��Ľӿ�
    virtual void ProcHandOverAns(void* pData, UINT4 uLength);
    //FailOver��������ȥ��������Ľӿ�
    virtual void EndFailOverInitDeActive();
    //FailOver ��Ӧ�˿�ʼ�����Ľӿ�
    virtual void BeginFailOverResp(void* pData, UINT4 uLength);
    //FailOver ��Ӧ�˽��������Ľӿ�
    virtual void EndFailOverResp(BOOL bSucc);
    //�ж��Ƿ��ܽ��й��ϵ���
    virtual int CanFailOver(EFAILOVERCAUSE_TYPE nCause);
    //����Դ״̬״̬
    virtual int CheckResStatus();

/*
    ��Ա����
*/
protected:

    //�����Ƿ��ж�
    BOOL                m_bHeartBeatBroken;
#define MAX_HB_BROKEN_TIME_DEFAULT  300

    //�������һ�δ��ڵ�ʱ��
    int                 m_tHBBrokenTime;

    //�ж������жϵ�ʱ�䷧ֵ
    int                 m_nMaxHBBrokenTime;

    /*
        ��������ָ���ʼ��ʱ���븳Ϊ��Чֵ������Ϊ��
    */
    //˫����ͨѶ�����ָ��
    CClusterNetwork*    m_pNetwork;

    //ͬ����Դ��������ָ��
    CResManager*        m_pResManager;

    /*
        ��������ָ������ʱ��һ����Ч����ʹ��ʱ��Ҫ���
    */
    //�첽��Դ�����ָ��
    CAsynResObject*     m_pAsynResObject;

    //˫�����ϴ�������ָ��
    CClusterAlarm*      m_pClusterAlarm;

    //CComCluster�ĵ�ʵ��ָ��
    static CComCluster* g_pComCluster;

    //�Ƿ�������������
    BOOL                m_bInstallArray;

    //�Ƿ��ǵ�����ʽ����
    BOOL                m_bSingleServer;

    //�Ƿ��˳��ı�־
    BOOL                m_bExit;

    /*
        �Ƿ����������ȥ����������ȴ��˳���������״̬�½���������
        �ƶ�˫��״̬Ǩ�ƵĽӿڣ��û�������˫������ǰ�����뱣֤���
        ����Ϊ�档
    */
    BOOL                m_bWaitForExit;

    //����˲���֡���к�
    UINT4               m_uInitSeqNum;

    //��Ӧ�˲���֡���к�  
    UINT4               m_uRespSeqNum;

    //�ϴζ�̬��Դ��״̬
    int                 m_nLastDynResStatus;

    //�ϴξ�̬��Դ��״̬
    int                 m_nLastStaticResStatus;

    friend class CClusterCreator;

    char                m_szStatusFile[MAX_PATH];

    SCLUSTERSTATUS      m_stClusterStatus;

    //��ȥ����ǰ�Ƿ��Ǽ���״̬
    BOOL                m_bLastActived;
};

#endif //__COM_CLUSTER_H__