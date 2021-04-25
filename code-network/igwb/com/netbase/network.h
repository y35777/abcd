#ifndef  __NETWORK_H__
#define  __NETWORK_H__

#include "../include/frame.h"
#include "protocol_track.h"

class CNetApp;
class CNetWorkStatusInfo;

class CNetWork:public CWorkModule
{
public:
    enum NWSTATUS
    {
        NWSTATUS_INIT,          //��ʼ��״̬
        NWSTATUS_ACTIVE,        //����״̬
        NWSTATUS_DEACTIVE,      //ȥ����״̬
        NWSTATUS_AUXUPGRADE     //��������״̬
    };  

    CNetWork();
    virtual ~CNetWork();

    //��ȡ��ǰ����
    UINT4 GetPacketId();

    //����ģ���Ƿ����ڵȴ�ǰ���̵�Ӧ��
    BOOL IsWaitForResponse();

    //����Ŀ¼,��"D:\\statusfile\\network\\"
    static char m_szWorkDir[MAX_PATH];

    //������Ϣ������
    int SendMsgtoHost(MSG_INTRA* pMsg);

    void TrackProtocol(MSG_INTRA* pMsg, int nProcRst = 0);

    //��ȡ����ģ�鵱ǰ״̬
    inline UINT4 GetCurStatus()
    {
        return m_uStatus;
    };
 
protected:

    //��ʼ��ʵ������
    virtual int InitInstance(void);

    //�˳�ʱ��������
    virtual void ExitInstance(void);

    //����ģ�����Ϣ
    virtual MODULE_TYPE GetModuleType();

    //��Ϣ�ַ�����
    virtual void ParseMsg(MSG_INTRA* pMsgParsed);

    //��ʱ������
    virtual void OnTimer(const long nTimerID);

    //1�붨ʱ������
    void Timer1Sec();

    //��ʱ����¼�����
    void LowAtTimer();

    //���������ָ�����
    int AdjustStatus(UINT4 uPacketSaved);

    //���ͻ������ݰ�
    BOOL SendBillPackage(BOOL bGetNewData = TRUE);

    //��ǰ���̷��Ϳհ�
    void SendEmptyPackage();

    //���ϴ��ṩ��ǰ����ģ�鱣��Ļ������ݽ��к����Ĵ���
    void OnResponse(BOOL const bSaved);

    //����״̬����
    void SyncStatus();

    //����״̬
    int SaveStatus();

    //��ȡ״̬
    int LoadStatus();

    //�ڳ�ʼ����ɺ󣬵�һ�ζ�ȡ״̬
    int InitStatus();

    //CNetWork��Э��ʵ���йصĲ��ֶ�ί��CNetApp����������
    CNetApp* m_pNetApp;

    //Э����ٶ����ָ��
    CProtocolTrack* m_pPtclTracker;

    //����ģ���״̬
    UINT4 m_uStatus;

    //��״̬�ļ���
    char m_szStatusFileName[MAX_PATH];

    //�Ƿ�Ҫ���кϲ��ı�־�����Ҫ���кϲ������޻���ʱ��Ҫ���հ�
    BOOL m_bMustMerge;

    //���¼�ʱ��ID
    long m_Timer1SecId;

    //�ͳ���ʱ��ʱ��
    int m_nLowAtTime;

    //��ʱ���ʱ��ID
    long m_TimerSendPacketId;

    //��״̬�ļ���д����
    CNetWorkStatusInfo* m_pNetWorkStatusInfo;

    //�ѷ��͵����һ���İ�ID
    UINT4 m_uPacketId;

    //�Ƿ����ڵȴ�ǰ����Ӧ��
    BOOL m_bWaitForResponse;

    //�Ƿ���Ҫ���յ�ǰ����Ӧ����������ͻ���
    BOOL m_bSendOnResponse;

    //��󻰵�������
    UINT4 m_uMaxPackageSize;

    //�ѷ��հ���
    UINT4 m_uSendEmptyPackageCount;

    //ÿ������Ҫ���͵����հ���
    UINT4 m_uSendEmptyPackageMaxPerSec;

    //�ͳ�����С
    UINT4 m_uLowAtSize;

    //���ӵ�Э������
    char m_szStackType[40];

};
#endif //__NETWORK_H__
