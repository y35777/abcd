#ifndef __COLLECT_TASK_H__
#define __COLLECT_TASK_H__

#include "../include/i_collect_task.h"

class  CCollectBase;

class CCollectTask : public ICollectTask
{
public:
    CCollectTask(int nSwitchIndex);
    virtual ~CCollectTask();

    //设置配置文件全路径名
    virtual void SetCfgPath(const char* const szCfgPath);

    //初始化采集任务
    virtual int Init();

    //启动采集任务
    virtual int Collect();

    //等待退出的操作
    virtual void WaitForExit();

    virtual void DeActive();

    virtual void Active();

    //设置告警门限
    virtual int  SetAlarmLimit(const char* const szTP, int nAlarmLimit);

    //采集指定的文件
    virtual int  GetSpecFile(const char* const szModule,
                             const char* const szSrcFile, 
                             const char* const szDstFile);
    //获取交换机信息
    virtual int  GetSwitchInfo(LIST<STRING>& label,
                               LIST<STRING>& content);

    //获取模块信息
    virtual int  GetModuleInfo(const char* const szModule,
                               LIST<STRING>& label,
                               LIST<STRING>& content);

    //获取所有链路的状态
    virtual int  GetLinkStatus(LIST<SLinkStat>& LinkStat);

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

    //added by chenliangwei, 2003-08-05
    //测试连接服务端
    virtual  int  TestLink();
    //added end

    //设置授权信息
    virtual void SetLicense(BOOL bLicensed)
    {
        m_bLicensed = bLicensed;
    }

protected:

    BOOL WaitNextTime();

    //完成一次完整的采集操作
    int  CollectProcess();

    //保存一次性采集序列到配置文件中
    int SaveOnceColSchl();

    int             m_nSwitchIndex;
    char            m_szSwitchName[40];       //交换机名

    //本采集任务的配置文件全路径名
    char            m_szCfgPath[MAX_PATH];

    //采集基类对象指针，指向具体的采集类对象
    CCollectBase*   m_pCollector;

    //上次采集完成时间
    time_t          m_nLastColTime;

    //上一次成功完成采集的启动时间
    time_t          m_nLastSucColTime;

    //下次采集等待的时间
    int             m_nFailedWaitTime;

    //采集启动时间
    int             m_nColBeginTime;

    //采集结束时间
    int             m_nColEndTime;

    //采集时间序列表
    LIST <int>      m_ColListTime;

    //一次性采集时间序列表
    LIST <time_t>   m_OnceColListTime;
    BOOL            m_bOnceColAct;   
    ACE_Recursive_Thread_Mutex m_MutexForSchl;

    //collect cycle(采集成功)
    int             m_nColSucCycle;

    //collect cycle(接连失败)
    int             m_nCntFailCycle;

    //collect cycle(采集失败)
    int             m_nColFailCycle;

    //最小采集周期
    int             m_nMinColCycle;
    
    //采集失败后允许重试次数
    int             m_nColFailTimes;

    //采集对象的结构
    SCreateCollect  m_stCreate;

    //如果该对象的所有者需要本线程退出，则需要置该成员变量为TRUE
    BOOL            m_bAskForExit;

    BOOL            m_bActive;

    LIST<SColFileReq>  m_ColFileList;

    LIST<SColFileReq>  m_SpecColFileList;
    ACE_Recursive_Thread_Mutex m_MutexForList;

    time_t                      m_tTestLink;
    MAP<STRING, SLinkStatusMsg> m_LinkStatusMap;
    ACE_Recursive_Thread_Mutex m_MutexForLinkMap;   //added by chenliangwei, 2003-08-05

    //是否授权
    BOOL            m_bLicensed;
    
    //上次开始等待的时间
    time_t          m_lLastWaitTime;
};

#endif //__COLLECT_TASK_H__
