/*     
    产品名:iGateway Bill V200
    模块名:告警
    文件名:snmp_agent_obj.h
    描述信息:
    
    包含CSnmpAgentObj类的定义,该类作为Snmp告警代理的中间类，
    用于将查询请求转发给告警模块处理 ，同时转发告警模块的告警给Snmp告警代理。


    修改记录:
    陈凉威,  2003-11 ,   创建.   
*/

#ifndef _SNMP_AGENT_OBJ_H
#define _SNMP_AGENT_OBJ_H

#include <stdio.h>
#include <string>
#include "../../include/snmp_agent_out.h"
#include "../mml_alarm_handler.h"

#ifndef USE_WINDOWS_DLL
#include "ace/DLL.h"
#endif


typedef IAlarmSnmpAgent* (*GETSNMPAGENT)(void);
typedef void (*Destructor)();


//Snmp代理类
class CSnmpAgentObj  
{  
public:
    static CSnmpAgentObj *Instance()
    {
        if(s_pInstance == NULL)
        {
            s_pInstance = new CSnmpAgentObj;
        }
        
        return s_pInstance;
    }

    //初始化
    virtual int Init(void* pHander, const char* szIPAddr, 
                     int nPort, const char* szFilePathRoot);

    //退出线程，并销毁实例
    virtual void Exit();

    //上报一条告警
    int ReportAlarm(SHistoryAlarm  *pAlarm, const char *szIpAddr="", int nPort = 0);

    //上报一个错误信息
    int SendErrorInfo(const char *szError, const char *szIpAddr, int nPort);

    //查询告警
    static int QueryAlarm(SAlarmQueryCond& condition, LIST<SHistoryAlarm> &FoundAlarmList);
    
    const char *GetLastError()
    {
        return m_strError.c_str();
    }

protected:

    CSnmpAgentObj();
    virtual ~CSnmpAgentObj();

private:
    static CSnmpAgentObj *s_pInstance;  //单件实例
    
#ifndef USE_WINDOWS_DLL
    ACE_DLL m_dll;
#endif
    
    IAlarmSnmpAgent*   m_pSnmpAgent;  //Snmp告警代理
    CMMLAlarmHandler*  m_pMMLHander;  //MML告警处理器

    STRING   m_strError;              //错误信息
    int      m_nErrno;                //错误告警码
};

#endif // _SNMP_ALARM_PROXY_H
