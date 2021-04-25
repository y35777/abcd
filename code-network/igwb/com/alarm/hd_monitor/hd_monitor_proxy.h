/*     
    产品名:iGateway Bill V200
    模块名:告警
    文件名:hd_monitor_proxy.h
    描述信息:
    
    包含CHDMonitorProxy类的定义,该类作为硬件检测功能的代理，
    用于将硬件告警转发给调用模块处理.
  　此类只提供一个转发告警的接口，具体的实现交给子类．

  　此类应该能够重用，所使用类和宏应是标准的．与产品无关．

    修改记录:
    张玉新,　2003-05 ,    创建.
    陈凉威,  2003-11 ,    优化代码，增加注释.   
*/

#ifndef _HD_MONITOR_PROXY_H
#define _HD_MONITOR_PROXY_H

#include <stdio.h>
#include <string>
#include "hdmonitor.h"

#ifndef USE_WINDOWS_DLL
#include "ace/DLL.h"
#endif

#ifndef STRING
#define STRING  std::string
#endif


#define ERR_OTHER                     -1
#define ERR_LOAD_DLL_FAIL             -2
#define ERR_FIND_FUN_IN_DLL_FAIL      -3
#define ERR_GET_SNMP_HD_MONITOR_FAIL  -4

enum WARN_TYPE
{
    WARN_RESTORE = 5,
    WARN_FAULT   = 6,
    WARN_EVENT   = 7
};

typedef IHardwareAlarm* (*IHDAlarmGet)(void);
typedef void (*Destructor)();

//Snmp硬件检测告警代理类
class CHDMonitorProxy  
{  
public:

    //模板函数，用于创建不同的CHDMonitorProxy类
    template<class TYPE> static CHDMonitorProxy *Instance(TYPE *p)
    {
        if(s_pInstance == NULL)
            s_pInstance = new TYPE;
        return s_pInstance;
    }

    static CHDMonitorProxy *Instance()
    {
        if(s_pInstance)
            return s_pInstance;
        return NULL;
    }

    //初始化
    virtual int Init(const char *szCfgPath, int nPort = 162);

    //退出线程，并销毁实例
    virtual void Exit();

    //告警发生时的回调函数
    static int AsyncNotify(IHardwareAlarm *pHDAlarm);

    const char *GetLastError()
    {
        return m_strError.c_str();
    }

protected:

    CHDMonitorProxy();
    virtual ~CHDMonitorProxy();

    //告警处理函数
    virtual int ProcessAlarm(SWarnData &Alarm);

protected:
    static CHDMonitorProxy *s_pInstance; 
    
#ifndef USE_WINDOWS_DLL
    ACE_DLL m_dll;
#endif
    
    IHardwareAlarm *m_pHDAlarm;
    
    STRING      m_strError;
    int         m_nErrno;
};

#endif // _SNMP_ALARM_PROXY_H
