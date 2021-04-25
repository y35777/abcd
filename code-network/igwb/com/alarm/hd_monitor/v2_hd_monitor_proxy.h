/*     
    ��Ʒ��:iGateway Bill V200
    ģ����:�澯
    �ļ���:v2_hd_monitor_proxy.h
    ������Ϣ:
    
    ����CV2HDMonitorProxy��Ķ���,������ΪiGateway Bill V200Ӳ����⹦�ܵĴ���
    ���ڽ�Ӳ���澯ת����iGateway Bill V200�澯ģ�鴦��.
    
    �޸ļ�¼:
    ������,��2003-05 ,    ����.
    ������,  2003-11 ,    �Ż����룬����ע��.       
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

//iGateway Bill V200Ӳ��������
class CV2HDMonitorProxy : public CHDMonitorProxy  
{
    friend CHDMonitorProxy;

public:
    //��ʼ��
    virtual int Init(const char *szCfgPath, int nPort = 162);
    
protected:
    CV2HDMonitorProxy();
    virtual ~CV2HDMonitorProxy();

    //�澯������
    virtual int ProcessAlarm(SWarnData &Alarm);

protected:

    MAP<int, int>    m_AlarmOriginIDToFinalMap;  //�ⲿ�澯ID���ڲ��澯ID��MAP
    MAP<int, int>    m_AlarmOriginIDToParaMap;   //�ⲿ�澯ID���ڲ��澯������MAP
    MAP<int, STRING> m_AlarmIDToExplainMap;      //�ⲿ�澯ID���澯���͵ĵ�MAP

    
    MAP<int, int>    m_WarnTypeToAlarmTypeMap;  //�ⲿ�澯���͵��ڲ��澯���͵�MAP
};
#endif //_V2_SNMP_ALARM_PROXY_H_
