#ifndef __COLLECT_TASK_H__
#define __COLLECT_TASK_H__

#include "../include/i_collect_task.h"

class  CCollectBase;

class CCollectTask : public ICollectTask
{
public:
    CCollectTask(int nSwitchIndex);
    virtual ~CCollectTask();

    //���������ļ�ȫ·����
    virtual void SetCfgPath(const char* const szCfgPath);

    //��ʼ���ɼ�����
    virtual int Init();

    //�����ɼ�����
    virtual int Collect();

    //�ȴ��˳��Ĳ���
    virtual void WaitForExit();

    virtual void DeActive();

    virtual void Active();

    //���ø澯����
    virtual int  SetAlarmLimit(const char* const szTP, int nAlarmLimit);

    //�ɼ�ָ�����ļ�
    virtual int  GetSpecFile(const char* const szModule,
                             const char* const szSrcFile, 
                             const char* const szDstFile);
    //��ȡ��������Ϣ
    virtual int  GetSwitchInfo(LIST<STRING>& label,
                               LIST<STRING>& content);

    //��ȡģ����Ϣ
    virtual int  GetModuleInfo(const char* const szModule,
                               LIST<STRING>& label,
                               LIST<STRING>& content);

    //��ȡ������·��״̬
    virtual int  GetLinkStatus(LIST<SLinkStat>& LinkStat);

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

    //added by chenliangwei, 2003-08-05
    //�������ӷ����
    virtual  int  TestLink();
    //added end

    //������Ȩ��Ϣ
    virtual void SetLicense(BOOL bLicensed)
    {
        m_bLicensed = bLicensed;
    }

protected:

    BOOL WaitNextTime();

    //���һ�������Ĳɼ�����
    int  CollectProcess();

    //����һ���Բɼ����е������ļ���
    int SaveOnceColSchl();

    int             m_nSwitchIndex;
    char            m_szSwitchName[40];       //��������

    //���ɼ�����������ļ�ȫ·����
    char            m_szCfgPath[MAX_PATH];

    //�ɼ��������ָ�룬ָ�����Ĳɼ������
    CCollectBase*   m_pCollector;

    //�ϴβɼ����ʱ��
    time_t          m_nLastColTime;

    //��һ�γɹ���ɲɼ�������ʱ��
    time_t          m_nLastSucColTime;

    //�´βɼ��ȴ���ʱ��
    int             m_nFailedWaitTime;

    //�ɼ�����ʱ��
    int             m_nColBeginTime;

    //�ɼ�����ʱ��
    int             m_nColEndTime;

    //�ɼ�ʱ�����б�
    LIST <int>      m_ColListTime;

    //һ���Բɼ�ʱ�����б�
    LIST <time_t>   m_OnceColListTime;
    BOOL            m_bOnceColAct;   
    ACE_Recursive_Thread_Mutex m_MutexForSchl;

    //collect cycle(�ɼ��ɹ�)
    int             m_nColSucCycle;

    //collect cycle(����ʧ��)
    int             m_nCntFailCycle;

    //collect cycle(�ɼ�ʧ��)
    int             m_nColFailCycle;

    //��С�ɼ�����
    int             m_nMinColCycle;
    
    //�ɼ�ʧ�ܺ��������Դ���
    int             m_nColFailTimes;

    //�ɼ�����Ľṹ
    SCreateCollect  m_stCreate;

    //����ö������������Ҫ���߳��˳�������Ҫ�øó�Ա����ΪTRUE
    BOOL            m_bAskForExit;

    BOOL            m_bActive;

    LIST<SColFileReq>  m_ColFileList;

    LIST<SColFileReq>  m_SpecColFileList;
    ACE_Recursive_Thread_Mutex m_MutexForList;

    time_t                      m_tTestLink;
    MAP<STRING, SLinkStatusMsg> m_LinkStatusMap;
    ACE_Recursive_Thread_Mutex m_MutexForLinkMap;   //added by chenliangwei, 2003-08-05

    //�Ƿ���Ȩ
    BOOL            m_bLicensed;
    
    //�ϴο�ʼ�ȴ���ʱ��
    time_t          m_lLastWaitTime;
};

#endif //__COLLECT_TASK_H__
