/*     
    产品名:iGateway Bill V200
    模块名:告警
    文件名:v2_hd_monitor_proxy.h
    描述信息:
    
    包含CV2HDMonitorProxy类的定义,该类作为iGateway Bill V200硬件检测功能的代理，
    用于将硬件告警转发给iGateway Bill V200告警模块处理.
    
    修改记录:
    张玉新,　2003-05 ,    创建.
    陈凉威,  2003-11 ,    优化代码，增加注释.       
*/

#ifndef _V2_HD_MONITOR_PROXY_H_
#define _V2_HD_MONITOR_PROXY_H_

#ifdef _PLATFORM_WIN32
#pragma warning(disable:4786)
#endif

#include "../../include/toolbox.h"
#include "../../frame/msg_proc.h"
#include "hd_monitor_proxy.h"

#define  S_ALARM_HDMONITOR_CFG_FILE      "HDAlmCfg.ini"
#define  S_ALARM_HDMONITOR_ERR_FILE      "snmp_trace.txt"

#define  S_ALARM_CONVERT_CFG_FILE        "HDAlmCvt.ini"
#define  S_CFG_ALARM_ID_LIST             "AlarmIDList"
#define  S_CFG_NUMBER                    "number"
#define  S_CFG_ALARM_GROUP               "AlarmGroup"
#define  S_CFG_ALARM_ID                  "AlarmID"
#define  S_CFG_ALARM_PARA_NUM            "AlarmParaNumber"

//iGateway Bill V200硬件检测代理
class CV2HDMonitorProxy : public CHDMonitorProxy  
{
    friend CHDMonitorProxy;

public:
    //初始化
    virtual int Init(const char *szCfgPath, int nPort = 162);
    
protected:
    CV2HDMonitorProxy();
    virtual ~CV2HDMonitorProxy();

    //告警处理函数
    virtual int ProcessAlarm(SWarnData &Alarm);

protected:

    MAP<int, int>    m_AlarmOriginIDToFinalMap;  //外部告警ID到内部告警ID的MAP
    MAP<int, int>    m_AlarmOriginIDToParaMap;   //外部告警ID到内部告警参数的MAP
    MAP<int, STRING> m_AlarmIDToExplainMap;      //外部告警ID到告警解释的的MAP

    
    MAP<int, int>    m_WarnTypeToAlarmTypeMap;  //外部告警类型到内部告警类型的MAP
};
#endif //_V2_SNMP_ALARM_PROXY_H_
