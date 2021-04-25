/*     
    ��Ʒ��:iGateway Bill V200
    ģ����:�澯
    �ļ���:i_alarm_snmp_agent.h
    ������Ϣ:
    
    ����IAlarmSnmpAgent��Ķ���,���ඨ����Snmp�澯����Ľӿڣ�
    �и澯��ѯ���澯�ϱ��ӿڡ�


    �޸ļ�¼:
    �������� 2003-11 ��  ����.
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
    IAlarmSnmpAgent(){};                //���캯��

    //����Agent�߳�
    virtual int StartThread(const char *szUdp, const char *szFilePathRoot) = 0;    

    //�˳�Agent�߳�
    virtual int ExitThread() = 0;      
    
    //ע�ắ��
    virtual int RegisterFunc(QUERYFUNC p) = 0;

    //�ϱ�һ���澯
    virtual int ReportAlarm(SHistoryAlarm  *pAlarm,const char *szIpAddr="", int nPort = 0) = 0;
    
    //�ϱ�һ��������Ϣ
    virtual int SendErrorInfo(const char *szError, const char *szIpAddr, int nPort) = 0;

    //�õ����һ��������Ϣ
    virtual const char* GetLastError(void) = 0;  

protected:
    virtual ~IAlarmSnmpAgent(){};       //��������     
};
#endif //_I_ALARM_SNMP_AGENT_H_
