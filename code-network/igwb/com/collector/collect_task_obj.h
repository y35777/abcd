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

    //初始化
    virtual int Init();

    //设置等待退出的标志
    void SetFlagForExit();

    //等待退出的操作
    void WaitForExit();

    void DeActive();

    void Active();

    //取激活状态的值，即任务是否正在运行
    inline BOOL GetActiveValue()
    {
        return m_bActive;
    }

    //取任务号
    inline int  GetTaskID(){return m_nTaskID;}

    //取任务号
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

    //采集指定的文件
    int  GetSpecFile(const char* const szModule,
                     const char* const szSrcFile, 
                     const char* const szDstFile);

    //获取交换机信息
    int  GetSwitchInfo(LIST<STRING>& label,
                       LIST<STRING>& content);

    //获取模块信息
    int  GetModuleInfo(const char* const szModule,
                       LIST<STRING>& label, 
                       LIST<STRING>& content);

    //获取所有链路的状态
    int  GetLinkStatus(LIST<SLinkStat>& LinkStat);

    //增加一个单次采集任务
    virtual int  SetOnceColSchl(LIST<time_t>& OnceSchlList);

    //获取单次采集任务列表
    virtual int  GetOnceColSchl(LIST<time_t>& OnceSchlList);

    //获取采集重试参数
    virtual int  GetColRetryPara(int& nCntFailCycle,
                                 int& nColFailCycle,
                                 int& nFileRetryTimes);

    //设置采集重试参数
    virtual int  SetColRetryPara(int nCntFailCycle,
                                 int nColFailCycle,
                                 int nFileRetryTimes);

    //获取周期性采集参数
    virtual int  GetColCyclePara(int& nBegin,
                                 int& nEnd, 
                                 int& nCycle);

    //设置周期性采集参数
    virtual int  SetColCyclePara(int nBegin,
                                 int nEnd,
                                 int nCycle);
    //获取序列方式采集的参数
    virtual int  GetColTimeList(LIST<int>& TimeList);

    //设置序列方式采集的参数
    virtual int  SetColTimeList(LIST<int>& TimeList);

    //设置授权信息
    inline void SetLicense(BOOL bLicensed)
    {
        if(NULL != m_pCollectTask)
            m_pCollectTask->SetLicense(bLicensed);

        m_bLicensed = bLicensed;
    }

    //获得授权信息
    inline BOOL GetLicense()
    {
        return m_bLicensed;
    }

protected:

    //ActiveObject对象的线程函数，通过run函数来启动
    virtual int svc();

    //是否处于激活状态
    BOOL          m_bActive;

    //是否授权
    BOOL          m_bLicensed;

    //是否已故障
    BOOL          m_bFailure;

    //本采集任务对应的配置文件全路径名
    char          m_szCfgPath[MAX_PATH];

    //采集任务对象接口指针
    ICollectTask* m_pCollectTask;

    //任务ID号，即交换机在配置文件中的编号
    int           m_nTaskID;

    //重启次数
    int           m_nRestartTimes;

    //上次激活时间
    time_t        m_tLastActiveTime;

    //可以认为已正常启动的时间间隔
    int           m_nAcceptedActiveInv;

    //可以接受的重启次数
    int           m_nAcceptedRestartTimes;

    bool          m_bManualStop;

    STRING        m_strColLabel;
};

#endif //__COLLECT_TASK_OBJ_H_
