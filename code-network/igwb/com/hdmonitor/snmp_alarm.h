#ifndef _SNMP_ALARM_INTERFACE_H_
#define _SNMP_ALARM_INTERFACE_H_

#include "read_config.h"
#include "hardware_alarm.h"

#include "notifyqueue.h"
#include "snmp_pp.h"


const int SERVER_NAME_LEN       = 15;   //��������������ֽ���
const int DELLDSK_MAX_LEN       = 30;   //DELL ���������ϱ���������ֽ���
const int HP_STATUS_MAX_LEN     = 50;   //HP ״̬�������
const int DELLSER_MAX_LEN       = 60;   //DELL������Ӳ����������ֽ���

#define BAM 255 
#define  S_SNMP_HD_ALARM_FILE      "SnmpHDAlarm.ini"

class CSnmpAlarm : public CHardwareAlarm
{
public:
    CSnmpAlarm();                 //���캯��
    virtual ~CSnmpAlarm();       //��������                       

    //��������TRAP���߳�
    virtual bool StartRcvThread(const char *szCfgPath,
                                int nPort = 162); 
    
    //�˳�����TRAP���߳�
    virtual bool ExitRcvThread();                       

protected:

    //TRAP�����߳�ִ�к���
    static void *TrapExecutionThread(void *param);         

    bool trap();
    static void trap_callback(int reason, Snmp *session, 
                              Pdu &pdu, SnmpTarget &target, void *cbd);
    
protected:
	ACE_thread_t     m_hTrapThread;               //TRAP�߳̾��
	bool             m_nTrapRunning;              //��ʶ����Trap�߳��Ƿ�����
    int              m_trap_port;
    
    CAlarmProperty   m_AlarmProperty;
};

#endif //_SNMP_ALARM_INTERFACE_H_
