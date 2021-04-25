/*     
    产品名:iGateway Bill V200
    模块名:告警
    文件名:i_alarm_snmp_agent.h
    描述信息:
    
    包含IAlarmSnmpAgent类的定义,该类定义了Snmp告警代理的接口，
    有告警查询，告警上报接口。


    修改记录:
    陈凉威， 2003-11 ，  创建.
*/

#ifndef _I_ALARM_SNMP_AGENT_H_
#define _I_ALARM_SNMP_AGENT_H_

#include "../alarm_util/alarm_config.h"
#include "../alarm_util/alarm_struct.h"


typedef int (*QUERYFUNC)(SAlarmQueryCond& condition, 
                         LIST<SHistoryAlarm> &FoundAlarmList);


class IAlarmSnmpAgent
{
public:
    IAlarmSnmpAgent(){};                //构造函数

    //创建Agent线程
    virtual int StartThread(const char *szUdp, const char *szFilePathRoot) = 0;    

    //退出Agent线程
    virtual int ExitThread() = 0;      
    
    //注册函数
    virtual int RegisterFunc(QUERYFUNC p) = 0;

    //上报一条告警
    virtual int ReportAlarm(SHistoryAlarm  *pAlarm,const char *szIpAddr="", int nPort = 0) = 0;
    
    //上报一个错误信息
    virtual int SendErrorInfo(const char *szError, const char *szIpAddr, int nPort) = 0;

    //得到最后一条错误信息
    virtual const char* GetLastError(void) = 0;  

protected:
    virtual ~IAlarmSnmpAgent(){};       //析构函数     
};
#endif //_I_ALARM_SNMP_AGENT_H_
