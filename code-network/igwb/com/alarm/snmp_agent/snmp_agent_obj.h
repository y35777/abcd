/*     
    ��Ʒ��:iGateway Bill V200
    ģ����:�澯
    �ļ���:snmp_agent_obj.h
    ������Ϣ:
    
    ����CSnmpAgentObj��Ķ���,������ΪSnmp�澯������м��࣬
    ���ڽ���ѯ����ת�����澯ģ�鴦�� ��ͬʱת���澯ģ��ĸ澯��Snmp�澯����


    �޸ļ�¼:
    ������,  2003-11 ,   ����.   
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


//Snmp������
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

    //��ʼ��
    virtual int Init(void* pHander, const char* szIPAddr, 
                     int nPort, const char* szFilePathRoot);

    //�˳��̣߳�������ʵ��
    virtual void Exit();

    //�ϱ�һ���澯
    int ReportAlarm(SHistoryAlarm  *pAlarm, const char *szIpAddr="", int nPort = 0);

    //�ϱ�һ��������Ϣ
    int SendErrorInfo(const char *szError, const char *szIpAddr, int nPort);

    //��ѯ�澯
    static int QueryAlarm(SAlarmQueryCond& condition, LIST<SHistoryAlarm> &FoundAlarmList);
    
    const char *GetLastError()
    {
        return m_strError.c_str();
    }

protected:

    CSnmpAgentObj();
    virtual ~CSnmpAgentObj();

private:
    static CSnmpAgentObj *s_pInstance;  //����ʵ��
    
#ifndef USE_WINDOWS_DLL
    ACE_DLL m_dll;
#endif
    
    IAlarmSnmpAgent*   m_pSnmpAgent;  //Snmp�澯����
    CMMLAlarmHandler*  m_pMMLHander;  //MML�澯������

    STRING   m_strError;              //������Ϣ
    int      m_nErrno;                //����澯��
};

#endif // _SNMP_ALARM_PROXY_H
