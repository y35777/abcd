/*     
    产品名:iGateway Bill V200
    模块名:告警
    文件名:alarm_snmp_agent.h
    描述信息:
    
    包含CSnmpAgent类的定义,该类Snmp告警代理的功能，
    用于NMS进行告警查询，以及调用模块进行告警上报。

    包含CErrorTrap，实现错误信息Trap上报功能。

    包含CAlarmTrap，实现告警上报及告警结果上报功能。


    修改记录:
    张玉新,  2003-08 ,   创建.
    陈凉威， 2003-11 ，  将之封装为动态库，并优化代码，增加注释.
*/

#ifndef _ALARM_SNMP_AGENT_H
#define _ALARM_SNMP_AGENT_H

#ifdef _PLATFORM_WIN32
#pragma warning(disable:4786)
#endif

#include "./resource.h"
#include "snmp_alarm.h"
#include "snmp_trap.h"
#include "i_alarm_snmp_agent.h"

#include <iostream>
#include <sstream>

#include "ace/Task.h"
#include "agent++.h"
#include "snmp_group.h"
#include "snmp_pp_ext.h"
#include "system_group.h"
#include "snmp_target_mib.h"
#include "snmp_notification_mib.h"
#include "snmp_community_mib.h"
#include "notification_originator.h"
#include "oid_def.h"
#include "log.h"



extern "C" 
{
    IGWB_Svc_Export IAlarmSnmpAgent* GetSnmpAgent();
    IGWB_Svc_Export void Destruct();
}


class CMMLAlarmHandler;
class CErrorTrap;
class CAlarmTrap;

//Snmp告警代理类
class CAlarmSnmpAgent : public ACE_Task_Base, public IAlarmSnmpAgent  
{
public:

    /*
     * 接口函数部分。
     */
    //创建Agent线程
    virtual int StartThread(const char *szUdp, const char *szFilePathRoot);    
    
    //退出Agent线程
    virtual int ExitThread();      

    //注册函数
    virtual int RegisterFunc(QUERYFUNC p);

    //上报一条查询告警结果
    virtual int ReportAlarm(SHistoryAlarm  *pAlarm,const char *szIpAddr="", int nPort = 0);
    
    //上报一个错误信息
    virtual int SendErrorInfo(const char *szError, const char *szIpAddr, int nPort);

    //得到最后一条错误信息
    virtual const char* GetLastError(void){ return m_strError.c_str(); }

    /*
     * 实现函数部分。
     */
    //得到函数的实例
    static CAlarmSnmpAgent *Instance();


    //初始化函数
    int Init(const char *szUdp, const char *szFilePathRoot);
    
    //清除函数
    void Destroy();

    //打开Agent线程
    virtual int open(void* args = 0);

    //Agent线程主函数
    virtual int svc();

    //增加网管中心
    int AddNmsInfo(SNMSInfo &nms);

    //删除网管中心
    int DeleteNMS(const char *szNMSName);

    //更改网管中心信息
    int ModifyNMSInfo(const char *szNMSName, const char *szKeyName, const char *szValue);
    int ModifyNMSInfo(const char *szNMSName, const char *szKeyName, int nVal);        


    //增加查询条件
    int AddQueryCondition(SQueryStatus &condition, const char *szNMSName);

    //删除查询条件
    int DelQueryCondition(const char *szNMSName);

    //更改查询条件
    int ModifyQueryCondition(const char *strName, const char *strKeyName, const char *strValue);
    int ModifyQueryCondition(const char *strName, const char *strKeyName, int nValue);        

    //开始查询告警
    int QueryAlarm(SAlarmQueryCond &condition,const char *szIpAddr, int nPort);

protected:

    CAlarmSnmpAgent();
    virtual ~CAlarmSnmpAgent();

    static CAlarmSnmpAgent  *s_pInstance;  //本实例的一个引用    

    QUERYFUNC   m_pQueryAlarm;          //查询告警函数指针
    bool        m_bRun;                 //监听循环的条件变量
    RequestList m_ReqList;              //请求队列

    int         m_nNMSCnt;              //NMS计数
    STRING      m_NMSFilePath;          //NMS文件路径
    int         m_nQueryCondCnt;        //查询条件计数
    STRING      m_QueryStatusFilePath;  //查询条件状态文件路径

    CErrorTrap  *m_pErrorTrap;          //错误Trap
    CAlarmTrap  *m_pAlarmTrap;          //告警Trap
    STRING       m_strError;            //错误

    int         m_nSnmpStatus;          //SNMP的状态
    Snmpx*      m_pSnmp;                //SNMP对象
    UdpAddress  m_UdpAddr;              //SNMP监听的地址
    Mib         m_Mib;                  //agent引擎


    CMMLAlarmHandler *m_pMMLHandler;    //MML告警处理类


}; 

//错误信息Trap类
class CErrorTrap : public CSNMPTrap  
{
public:
    CErrorTrap(const char *szIPAddr = "0.0.0.0");
    virtual ~CErrorTrap(){}

    //发送故障Trap
    int SendError(const char *szError, const char *szIpAdder, int nPort);

protected:
    ACE_Thread_Mutex m_mutex;
};

//告警Trap类，故障发生时及查询告警时发送 
class CAlarmTrap : public CSNMPTrap  
{
public:
    CAlarmTrap(const char *szIPAddr = "0.0.0.0");
    virtual ~CAlarmTrap(){}

    //发送告警Trap
    int ReportAlarm(SHistoryAlarm  *pAlarm,const char *szIpAdder, int nPort);        

protected:
    ACE_Thread_Mutex m_mutex;
};


#endif // ALARM_PROXY_H