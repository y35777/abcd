/*     
    ��Ʒ��:iGateway Bill V200
    ģ����:�澯
    �ļ���:snmp_agent_obj.cpp
    ������Ϣ:
    
    ����CSnmpAgentObj��Ķ���,������ΪSnmp�澯������м��࣬
    ���ڽ���ѯ����ת�����澯ģ�鴦�� ��ͬʱת���澯ģ��ĸ澯��Snmp�澯����


    �޸ļ�¼:
    ������,  2003-11 ,   ����.   
*/


#include "snmp_agent_obj.h"

#ifdef _PLATFORM_WIN32
    #define SNMP_AGENT_DLL       "alarm_snmp_agent.dll"
#elif defined(_PLATFORM_SOLARIS) || defined (_PLATFORM_LINUX) 
    #define SNMP_AGENT_DLL       "libalarm_snmp_agent.so"
#elif defined(_PLATFORM_HPUX)
    #define SNMP_AGENT_DLL       "libalarm_snmp_agent.sl"
#else
    #error macro _PLATFORM_??? undefined.
#endif

CSnmpAgentObj* CSnmpAgentObj::s_pInstance = NULL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSnmpAgentObj::CSnmpAgentObj()
{
    m_pSnmpAgent = NULL;
    m_pMMLHander = NULL;
    m_nErrno = ERR_SUCCESS;
}

CSnmpAgentObj::~CSnmpAgentObj()
{
    m_pSnmpAgent = NULL;
}


//��ʼ��
int CSnmpAgentObj::Init(void* pHander, const char* szIPAddr, 
                        int nPort, const char* szFilePathRoot)
{
    assert(pHander != NULL);
    m_pMMLHander = (CMMLAlarmHandler*)pHander;

    if ((szIPAddr == NULL) || (szIPAddr[0] == '\0'))
        return ERR_FAIL;

    char szPort[20];
    SNPRINTF(szPort, sizeof(szPort), "%s/%d", szIPAddr, nPort);
    szPort[sizeof(szPort) - 1] = '\0';

#ifndef USE_WINDOWS_DLL
    
    //����SNMP����̬��
    int nRet = m_dll.open(SNMP_AGENT_DLL);;
    if (nRet != ERR_SUCCESS)
    {
        m_strError = ACE_OS::strerror(ACE_OS::last_error());
        m_nErrno = ACE_OS::last_error();  
        return m_nErrno;
    }


    //��λ��ȡ������
    GETSNMPAGENT pGet = (GETSNMPAGENT) m_dll.symbol("GetSnmpAgent");
    if(pGet == NULL)
    {
        m_strError = ACE_OS::strerror(ACE_OS::last_error());
        m_nErrno = ACE_OS::last_error(); 
        m_dll.close();
        return m_nErrno;
    }

    //�õ�Agent
    m_pSnmpAgent = pGet();
    if(m_pSnmpAgent == NULL)
    {
        m_strError = S_ALARM_GET_SNMP_AGENT_FAIL;
        m_nErrno = ERR_FAIL;
        m_dll.close();
        return m_nErrno;
    }

    //ע�ắ��
    m_pSnmpAgent->RegisterFunc(QueryAlarm);

    //�����߳�
    if(ERR_SUCCESS == m_pSnmpAgent->StartThread(szPort, szFilePathRoot))
    {
        m_strError = S_ALARM_START_SNMPAGENTTHD_SUCC;
        m_nErrno = ERR_SUCCESS;
        return m_nErrno;
    }

    //�������
    m_strError = m_pSnmpAgent->GetLastError();
    Destructor Destruct = (Destructor)m_dll.symbol("Destruct");
    if(Destruct != NULL)
    {
        Destruct();
    }
    else
    {
        m_strError += ACE_OS::strerror(ACE_OS::last_error());
    }

    m_dll.close();
    m_nErrno = ERR_FAIL; 
    return m_nErrno;
#else
    return ERR_SUCCESS;
#endif
}

//�˳��̣߳�������ʵ��
void CSnmpAgentObj::Exit()
{
#ifndef USE_WINDOWS_DLL    
    if (s_pInstance == NULL)
        return;

    //��ʼ���ɹ�
    if (m_nErrno == ERR_SUCCESS) 
    {
        m_pSnmpAgent->ExitThread();

        Destructor Destruct = (Destructor)m_dll.symbol("Destruct");
        if(Destruct != NULL)
        {
            Destruct();
        }
        else
        {
            m_strError = ACE_OS::strerror(ACE_OS::last_error());
        }

        m_dll.close();
    }

    delete this;
    s_pInstance = NULL;

#endif
}

//�ϱ�һ���澯
int CSnmpAgentObj::ReportAlarm(SHistoryAlarm  *pAlarm, const char *szIpAddr, int nPort)
{
    if (m_pSnmpAgent == NULL)
        return ERR_FAIL;

    return m_pSnmpAgent->ReportAlarm(pAlarm, szIpAddr, nPort);
}


//�ϱ�һ��������Ϣ
int CSnmpAgentObj::SendErrorInfo(const char *szError, const char *szIpAddr, int nPort)
{
    if (m_pSnmpAgent == NULL)
        return ERR_FAIL;

    return m_pSnmpAgent->SendErrorInfo(szError, szIpAddr, nPort);
}

//��ѯ�澯
int CSnmpAgentObj::QueryAlarm(SAlarmQueryCond& condition, LIST<SHistoryAlarm> &FoundAlarmList)
{
    if((s_pInstance == NULL) || (s_pInstance->m_pMMLHander == NULL))
        return ERR_FAIL;

    return s_pInstance->m_pMMLHander->QueryAlmLog_Impl(condition, FoundAlarmList);
}