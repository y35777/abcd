/*     
    ��Ʒ��:iGateway Bill V200
    ģ����:�澯
    �ļ���:alarm_snmp_agent.h
    ������Ϣ:
    
    ����CSnmpAgent��Ķ���,����Snmp�澯����Ĺ��ܣ�
    ����NMS���и澯��ѯ���Լ�����ģ����и澯�ϱ���

    ����CErrorTrap��ʵ�ִ�����ϢTrap�ϱ����ܡ�

    ����CAlarmTrap��ʵ�ָ澯�ϱ����澯����ϱ����ܡ�


    �޸ļ�¼:
    ������,  2003-08 ,   ����.
    �������� 2003-11 ��  ��֮��װΪ��̬�⣬���Ż����룬����ע��.
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

//Snmp�澯������
class CAlarmSnmpAgent : public ACE_Task_Base, public IAlarmSnmpAgent  
{
public:

    /*
     * �ӿں������֡�
     */
    //����Agent�߳�
    virtual int StartThread(const char *szUdp, const char *szFilePathRoot);    
    
    //�˳�Agent�߳�
    virtual int ExitThread();      

    //ע�ắ��
    virtual int RegisterFunc(QUERYFUNC p);

    //�ϱ�һ����ѯ�澯���
    virtual int ReportAlarm(SHistoryAlarm  *pAlarm,const char *szIpAddr="", int nPort = 0);
    
    //�ϱ�һ��������Ϣ
    virtual int SendErrorInfo(const char *szError, const char *szIpAddr, int nPort);

    //�õ����һ��������Ϣ
    virtual const char* GetLastError(void){ return m_strError.c_str(); }

    /*
     * ʵ�ֺ������֡�
     */
    //�õ�������ʵ��
    static CAlarmSnmpAgent *Instance();


    //��ʼ������
    int Init(const char *szUdp, const char *szFilePathRoot);
    
    //�������
    void Destroy();

    //��Agent�߳�
    virtual int open(void* args = 0);

    //Agent�߳�������
    virtual int svc();

    //������������
    int AddNmsInfo(SNMSInfo &nms);

    //ɾ����������
    int DeleteNMS(const char *szNMSName);

    //��������������Ϣ
    int ModifyNMSInfo(const char *szNMSName, const char *szKeyName, const char *szValue);
    int ModifyNMSInfo(const char *szNMSName, const char *szKeyName, int nVal);        


    //���Ӳ�ѯ����
    int AddQueryCondition(SQueryStatus &condition, const char *szNMSName);

    //ɾ����ѯ����
    int DelQueryCondition(const char *szNMSName);

    //���Ĳ�ѯ����
    int ModifyQueryCondition(const char *strName, const char *strKeyName, const char *strValue);
    int ModifyQueryCondition(const char *strName, const char *strKeyName, int nValue);        

    //��ʼ��ѯ�澯
    int QueryAlarm(SAlarmQueryCond &condition,const char *szIpAddr, int nPort);

protected:

    CAlarmSnmpAgent();
    virtual ~CAlarmSnmpAgent();

    static CAlarmSnmpAgent  *s_pInstance;  //��ʵ����һ������    

    QUERYFUNC   m_pQueryAlarm;          //��ѯ�澯����ָ��
    bool        m_bRun;                 //����ѭ������������
    RequestList m_ReqList;              //�������

    int         m_nNMSCnt;              //NMS����
    STRING      m_NMSFilePath;          //NMS�ļ�·��
    int         m_nQueryCondCnt;        //��ѯ��������
    STRING      m_QueryStatusFilePath;  //��ѯ����״̬�ļ�·��

    CErrorTrap  *m_pErrorTrap;          //����Trap
    CAlarmTrap  *m_pAlarmTrap;          //�澯Trap
    STRING       m_strError;            //����

    int         m_nSnmpStatus;          //SNMP��״̬
    Snmpx*      m_pSnmp;                //SNMP����
    UdpAddress  m_UdpAddr;              //SNMP�����ĵ�ַ
    Mib         m_Mib;                  //agent����


    CMMLAlarmHandler *m_pMMLHandler;    //MML�澯������


}; 

//������ϢTrap��
class CErrorTrap : public CSNMPTrap  
{
public:
    CErrorTrap(const char *szIPAddr = "0.0.0.0");
    virtual ~CErrorTrap(){}

    //���͹���Trap
    int SendError(const char *szError, const char *szIpAdder, int nPort);

protected:
    ACE_Thread_Mutex m_mutex;
};

//�澯Trap�࣬���Ϸ���ʱ����ѯ�澯ʱ���� 
class CAlarmTrap : public CSNMPTrap  
{
public:
    CAlarmTrap(const char *szIPAddr = "0.0.0.0");
    virtual ~CAlarmTrap(){}

    //���͸澯Trap
    int ReportAlarm(SHistoryAlarm  *pAlarm,const char *szIpAdder, int nPort);        

protected:
    ACE_Thread_Mutex m_mutex;
};


#endif // ALARM_PROXY_H