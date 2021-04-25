#ifndef __COLLECT_TASK_OBJ_H_
#define __COLLECT_TASK_OBJ_H_

#include "ace/Task.h"
#include "include/i_collect_task.h"

class CCollectTaskObj : public ACE_Task_Base
{
public:
    CCollectTaskObj(int nTaskID, 
                    const char* const szColLabel,
                    const char* const szCfgPath);
    virtual ~CCollectTaskObj();

    //��ʼ��
    virtual int Init();

    //���õȴ��˳��ı�־
    void SetFlagForExit();

    //�ȴ��˳��Ĳ���
    void WaitForExit();

    void DeActive();

    void Active();

    //ȡ����״̬��ֵ���������Ƿ���������
    inline BOOL GetActiveValue()
    {
        return m_bActive;
    }

    //ȡ�����
    inline int  GetTaskID(){return m_nTaskID;}

    //ȡ�����
    inline const char*  GetColLabel(){return m_strColLabel.c_str();}

    BOOL CanRestart()
    {
        if(time(NULL) - m_tLastActiveTime > 60)
        {
            return m_nRestartTimes < m_nAcceptedRestartTimes;
        }

        return FALSE;
    }

    BOOL GetFailureValue()
    {
        return m_bFailure;
    }

    bool GetManualStop(){return m_bManualStop;}

    void SetManualStop(bool bManualStop){m_bManualStop = bManualStop;}

    int  SetAlarmLimit(const char* const szTP, int nAlarmLimit)
    {
        if(NULL != m_pCollectTask)
        {
            return m_pCollectTask->SetAlarmLimit(szTP, nAlarmLimit);
        }
        return ERR_SUCCESS;
    }

    //�ɼ�ָ�����ļ�
    int  GetSpecFile(const char* const szModule,
                     const char* const szSrcFile, 
                     const char* const szDstFile);

    //��ȡ��������Ϣ
    int  GetSwitchInfo(LIST<STRING>& label,
                       LIST<STRING>& content);

    //��ȡģ����Ϣ
    int  GetModuleInfo(const char* const szModule,
                       LIST<STRING>& label, 
                       LIST<STRING>& content);

    //��ȡ������·��״̬
    int  GetLinkStatus(LIST<SLinkStat>& LinkStat);

    //����һ�����βɼ�����
    virtual int  SetOnceColSchl(LIST<time_t>& OnceSchlList);

    //��ȡ���βɼ������б�
    virtual int  GetOnceColSchl(LIST<time_t>& OnceSchlList);

    //��ȡ�ɼ����Բ���
    virtual int  GetColRetryPara(int& nCntFailCycle,
                                 int& nColFailCycle,
                                 int& nFileRetryTimes);

    //���òɼ����Բ���
    virtual int  SetColRetryPara(int nCntFailCycle,
                                 int nColFailCycle,
                                 int nFileRetryTimes);

    //��ȡ�����Բɼ�����
    virtual int  GetColCyclePara(int& nBegin,
                                 int& nEnd, 
                                 int& nCycle);

    //���������Բɼ�����
    virtual int  SetColCyclePara(int nBegin,
                                 int nEnd,
                                 int nCycle);
    //��ȡ���з�ʽ�ɼ��Ĳ���
    virtual int  GetColTimeList(LIST<int>& TimeList);

    //�������з�ʽ�ɼ��Ĳ���
    virtual int  SetColTimeList(LIST<int>& TimeList);

    //������Ȩ��Ϣ
    inline void SetLicense(BOOL bLicensed)
    {
        if(NULL != m_pCollectTask)
            m_pCollectTask->SetLicense(bLicensed);

        m_bLicensed = bLicensed;
    }

    //�����Ȩ��Ϣ
    inline BOOL GetLicense()
    {
        return m_bLicensed;
    }

protected:

    //ActiveObject������̺߳�����ͨ��run����������
    virtual int svc();

    //�Ƿ��ڼ���״̬
    BOOL          m_bActive;

    //�Ƿ���Ȩ
    BOOL          m_bLicensed;

    //�Ƿ��ѹ���
    BOOL          m_bFailure;

    //���ɼ������Ӧ�������ļ�ȫ·����
    char          m_szCfgPath[MAX_PATH];

    //�ɼ��������ӿ�ָ��
    ICollectTask* m_pCollectTask;

    //����ID�ţ����������������ļ��еı��
    int           m_nTaskID;

    //��������
    int           m_nRestartTimes;

    //�ϴμ���ʱ��
    time_t        m_tLastActiveTime;

    //������Ϊ������������ʱ����
    int           m_nAcceptedActiveInv;

    //���Խ��ܵ���������
    int           m_nAcceptedRestartTimes;

    bool          m_bManualStop;

    STRING        m_strColLabel;
};

#endif //__COLLECT_TASK_OBJ_H_
