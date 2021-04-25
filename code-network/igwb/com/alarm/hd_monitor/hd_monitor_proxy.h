/*     
    ��Ʒ��:iGateway Bill V200
    ģ����:�澯
    �ļ���:hd_monitor_proxy.h
    ������Ϣ:
    
    ����CHDMonitorProxy��Ķ���,������ΪӲ����⹦�ܵĴ���
    ���ڽ�Ӳ���澯ת��������ģ�鴦��.
  ������ֻ�ṩһ��ת���澯�Ľӿڣ������ʵ�ֽ������࣮

  ������Ӧ���ܹ����ã���ʹ����ͺ�Ӧ�Ǳ�׼�ģ����Ʒ�޹أ�

    �޸ļ�¼:
    ������,��2003-05 ,    ����.
    ������,  2003-11 ,    �Ż����룬����ע��.   
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

//SnmpӲ�����澯������
class CHDMonitorProxy  
{  
public:

    //ģ�庯�������ڴ�����ͬ��CHDMonitorProxy��
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

    //��ʼ��
    virtual int Init(const char *szCfgPath, int nPort = 162);

    //�˳��̣߳�������ʵ��
    virtual void Exit();

    //�澯����ʱ�Ļص�����
    static int AsyncNotify(IHardwareAlarm *pHDAlarm);

    const char *GetLastError()
    {
        return m_strError.c_str();
    }

protected:

    CHDMonitorProxy();
    virtual ~CHDMonitorProxy();

    //�澯������
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
