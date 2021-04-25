#ifndef __I_COLLECT_Task_H__
#define __I_COLLECT_Task_H__

class ICollectTask
{
public:
    ICollectTask(){};
    virtual ~ICollectTask(){};

    //���������ļ�ȫ·����
    virtual void SetCfgPath(const char* const szCfgPath) = 0;

    //��ʼ���ɼ�����
    virtual int Init() = 0;

    //�����ɼ�����
    virtual int Collect() = 0;

    //�ȴ��˳��Ĳ���
    virtual void WaitForExit() = 0;

    virtual void DeActive() = 0;

    virtual void Active() = 0;

    virtual int  SetAlarmLimit(const char* const szTP, 
                               int nAlarmLimit) = 0;

    virtual int  GetSpecFile(const char* const szModule,
                             const char* const szSrcFile, 
                             const char* const szDstFile) = 0;
    //��ȡ��������Ϣ
    virtual int  GetSwitchInfo(LIST<STRING>& label,
                               LIST<STRING>& content) = 0;

    //��ȡģ����Ϣ
    virtual int  GetModuleInfo(const char* const szModule,
                               LIST<STRING>& label,
                               LIST<STRING>& content) = 0;
    //��ȡ������·��״̬
    virtual int  GetLinkStatus(LIST<SLinkStat>& LinkStat) = 0;

    //����һ�����βɼ�����
    virtual int  SetOnceColSchl(LIST<time_t>& OnceSchlList) = 0;

    //��ȡ���βɼ������б�
    virtual int  GetOnceColSchl(LIST<time_t>& OnceSchlList) = 0;

    //��ȡ�ɼ����Բ���
    virtual int  GetColRetryPara(int& nCntFailCycle,
                                 int& nColFailCycle,
                                 int& nFileRetryTimes) = 0;

    //���òɼ����Բ���
    virtual int  SetColRetryPara(int nCntFailCycle,
                                 int nColFailCycle,
                                 int nFileRetryTimes) = 0;

    //��ȡ�����Բɼ�����
    virtual int  GetColCyclePara(int& nBegin,
                                 int& nEnd, 
                                 int& nCycle) = 0;

    //���������Բɼ�����
    virtual int  SetColCyclePara(int nBegin,
                                 int nEnd,
                                 int nCycle) = 0;

    //��ȡ���з�ʽ�ɼ��Ĳ���
    virtual int  GetColTimeList(LIST<int>& TimeList) = 0;

    //�������з�ʽ�ɼ��Ĳ���
    virtual int  SetColTimeList(LIST<int>& TimeList) = 0;

    //������Ȩ��Ϣ
    virtual void SetLicense(BOOL bLicensed) = 0;
};

#endif //__I_COLLECT_Task_H__
