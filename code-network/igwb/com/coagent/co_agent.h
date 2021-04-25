#ifndef __COLLECTOR_AGENT_H__
#define __COLLECTOR_AGENT_H__

#include "../include/frame.h"


class CCollectorAgent : public CWorkModule
{
public:
    // ���캯��
    CCollectorAgent(void);

    // ��������
    virtual ~CCollectorAgent();

protected:

    // ���������ַ�����
    virtual void ParseMsg (MSG_INTRA* pMsgParsed);

    // ��ʼ������
    virtual int InitInstance(void);

    // ��������������
    virtual void ExitInstance(void);

    // ��ȡ��ģ�����ͺ���
    virtual MODULE_TYPE GetModuleType(void);

    virtual void OnTimer(const long nTimerID);

    void Timer5Sec();

    void SendMMLReport(const char* szReport,
                       int nAppType,
                       int nClientNo);

    BOOL WriteCCSConfig(void);

    BOOL LoadCCSConfig(void);

    BOOL UpdateUserRight(void);

    //ɾ��һ���ɼ�������Ϣ
    void RemoveCCSCfgItem(MSG_INTRA* pMsg);

    void AddCCSCfgItem(MSG_INTRA* pMsg);

    void GetCCSCfgInfo(MSG_INTRA* pMsg);

    void ModifyCCSCfgItem(MSG_INTRA* pMsg);

    //�ɼ������������̺ŵ�MAP
    MAP<STRING, int>           m_ColToProcType;

    //�ɼ����������ϴ��յ�״̬��Ϣʱ���MAP
    MAP<STRING, time_t>        m_ColToSHKTime;

    enum CO_MD_STATUS
    {
        CM_FAILURE           = -1,
        CM_DEACTIVE          = 0,
        CM_ACTIVE            = 1
    };
    //�ɼ���������״̬��MAP
    MAP<STRING, int>            m_ColToStatus;

    long                        m_lTimerID5Sec;

    long                        m_lSHKTimeOut;

    char                        m_szCfgFile[MAX_PATH];

    LIST<STRING>                m_CfgInfo;

    //�û�Ȩ�ޱ�
    MAP<STRING, MAP<STRING, int> >  m_UserRight;
};

#endif //__COLLECTOR_AGENT_H__


