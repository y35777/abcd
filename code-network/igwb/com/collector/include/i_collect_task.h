#ifndef __I_COLLECT_Task_H__
#define __I_COLLECT_Task_H__

class ICollectTask
{
public:
    ICollectTask(){};
    virtual ~ICollectTask(){};

    //设置配置文件全路径名
    virtual void SetCfgPath(const char* const szCfgPath) = 0;

    //初始化采集任务
    virtual int Init() = 0;

    //启动采集任务
    virtual int Collect() = 0;

    //等待退出的操作
    virtual void WaitForExit() = 0;

    virtual void DeActive() = 0;

    virtual void Active() = 0;

    virtual int  SetAlarmLimit(const char* const szTP, 
                               int nAlarmLimit) = 0;

    virtual int  GetSpecFile(const char* const szModule,
                             const char* const szSrcFile, 
                             const char* const szDstFile) = 0;
    //获取交换机信息
    virtual int  GetSwitchInfo(LIST<STRING>& label,
                               LIST<STRING>& content) = 0;

    //获取模块信息
    virtual int  GetModuleInfo(const char* const szModule,
                               LIST<STRING>& label,
                               LIST<STRING>& content) = 0;
    //获取所有链路的状态
    virtual int  GetLinkStatus(LIST<SLinkStat>& LinkStat) = 0;

    //增加一个单次采集任务
    virtual int  SetOnceColSchl(LIST<time_t>& OnceSchlList) = 0;

    //获取单次采集任务列表
    virtual int  GetOnceColSchl(LIST<time_t>& OnceSchlList) = 0;

    //获取采集重试参数
    virtual int  GetColRetryPara(int& nCntFailCycle,
                                 int& nColFailCycle,
                                 int& nFileRetryTimes) = 0;

    //设置采集重试参数
    virtual int  SetColRetryPara(int nCntFailCycle,
                                 int nColFailCycle,
                                 int nFileRetryTimes) = 0;

    //获取周期性采集参数
    virtual int  GetColCyclePara(int& nBegin,
                                 int& nEnd, 
                                 int& nCycle) = 0;

    //设置周期性采集参数
    virtual int  SetColCyclePara(int nBegin,
                                 int nEnd,
                                 int nCycle) = 0;

    //获取序列方式采集的参数
    virtual int  GetColTimeList(LIST<int>& TimeList) = 0;

    //设置序列方式采集的参数
    virtual int  SetColTimeList(LIST<int>& TimeList) = 0;

    //设置授权信息
    virtual void SetLicense(BOOL bLicensed) = 0;
};

#endif //__I_COLLECT_Task_H__
