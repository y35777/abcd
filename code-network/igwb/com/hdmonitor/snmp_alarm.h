#ifndef _SNMP_ALARM_INTERFACE_H_
#define _SNMP_ALARM_INTERFACE_H_

#include "read_config.h"
#include "hardware_alarm.h"

#include "notifyqueue.h"
#include "snmp_pp.h"


const int SERVER_NAME_LEN       = 15;   //服务器名的最大字节数
const int DELLDSK_MAX_LEN       = 30;   //DELL 磁盘阵列上报参数最大字节数
const int HP_STATUS_MAX_LEN     = 50;   //HP 状态描述语句
const int DELLSER_MAX_LEN       = 60;   //DELL服务器硬件描述最大字节数

#define BAM 255 
#define  S_SNMP_HD_ALARM_FILE      "SnmpHDAlarm.ini"

class CSnmpAlarm : public CHardwareAlarm
{
public:
    CSnmpAlarm();                 //构造函数
    virtual ~CSnmpAlarm();       //析构函数                       

    //创建接收TRAP的线程
    virtual bool StartRcvThread(const char *szCfgPath,
                                int nPort = 162); 
    
    //退出接收TRAP的线程
    virtual bool ExitRcvThread();                       

protected:

    //TRAP接收线程执行函数
    static void *TrapExecutionThread(void *param);         

    bool trap();
    static void trap_callback(int reason, Snmp *session, 
                              Pdu &pdu, SnmpTarget &target, void *cbd);
    
protected:
	ACE_thread_t     m_hTrapThread;               //TRAP线程句柄
	bool             m_nTrapRunning;              //标识接收Trap线程是否运行
    int              m_trap_port;
    
    CAlarmProperty   m_AlarmProperty;
};

#endif //_SNMP_ALARM_INTERFACE_H_
