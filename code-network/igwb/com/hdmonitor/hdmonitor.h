#ifndef _HARDWARE_MONITOR_H_
#define _HARDWARE_MONITOR_H_


#define PARAM_MAX_LEN 80

struct SWarnData
{
    unsigned short	AlarmId;			  //告警编号
    unsigned char	AlarmType;			  //5-恢复,6-故障,7-事件
    unsigned short	Len;				  //参数长度
    unsigned char	Paras[PARAM_MAX_LEN]; //故障模块送给告警模块的告警参数
    
public:
    SWarnData()
    {
        memset(this, 0, sizeof(SWarnData)); // 用0初始化告警帧
    }
    
};

class IHardwareAlarm;
typedef int (*FUNC)(IHardwareAlarm *pHDAlarm); // 定义注册函数的空指针

class IHardwareAlarm
{
public:
    IHardwareAlarm(){};
    virtual ~IHardwareAlarm(){};       //析构函数 
    
    //创建硬件告警的监视的线程
    virtual bool StartRcvThread(const char *szCfgPath, 
                                int nPort = 162) = 0;  
    //退出硬件告警的监视的线程
    virtual bool ExitRcvThread() = 0;  
    
    //从队列中取出告警帧
    virtual bool GetAlarm(SWarnData& alarm) = 0;   
    
    //注册函数
    virtual bool RegisterFunc(FUNC p) = 0;  
    
    //获取出错信息
    virtual const char* GetLastError(void) const = 0;  
                  
};

#endif //_HARDWARE_MONITOR_H_
