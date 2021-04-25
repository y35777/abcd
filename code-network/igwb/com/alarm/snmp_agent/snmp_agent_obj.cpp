/*     
    产品名:iGateway Bill V200
    模块名:告警
    文件名:snmp_agent_obj.cpp
    描述信息:
    
    包含CSnmpAgentObj类的定义,该类作为Snmp告警代理的中间类，
    用于将查询请求转发给告警模块处理 ，同时转发告警模块的告警给Snmp告警代理。


    修改记录:
    陈凉威,  2003-11 ,   创建.   
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


//初始化
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
    
    //加载SNMP代理动态库
    int nRet = m_dll.open(SNMP_AGENT_DLL);;
    if (nRet != ERR_SUCCESS)
    {
        m_strError = ACE_OS::strerror(ACE_OS::last_error());
        m_nErrno = ACE_OS::last_error();  
        return m_nErrno;
    }


    //定位获取代理函数
    GETSNMPAGENT pGet = (GETSNMPAGENT) m_dll.symbol("GetSnmpAgent");
    if(pGet == NULL)
    {
        m_strError = ACE_OS::strerror(ACE_OS::last_error());
        m_nErrno = ACE_OS::last_error(); 
        m_dll.close();
        return m_nErrno;
    }

    //得到Agent
    m_pSnmpAgent = pGet();
    if(m_pSnmpAgent == NULL)
    {
        m_strError = S_ALARM_GET_SNMP_AGENT_FAIL;
        m_nErrno = ERR_FAIL;
        m_dll.close();
        return m_nErrno;
    }

    //注册函数
    m_pSnmpAgent->RegisterFunc(QueryAlarm);

    //启动线程
    if(ERR_SUCCESS == m_pSnmpAgent->StartThread(szPort, szFilePathRoot))
    {
        m_strError = S_ALARM_START_SNMPAGENTTHD_SUCC;
        m_nErrno = ERR_SUCCESS;
        return m_nErrno;
    }

    //处理出错
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

//退出线程，并销毁实例
void CSnmpAgentObj::Exit()
{
#ifndef USE_WINDOWS_DLL    
    if (s_pInstance == NULL)
        return;

    //初始化成功
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

//上报一条告警
int CSnmpAgentObj::ReportAlarm(SHistoryAlarm  *pAlarm, const char *szIpAddr, int nPort)
{
    if (m_pSnmpAgent == NULL)
        return ERR_FAIL;

    return m_pSnmpAgent->ReportAlarm(pAlarm, szIpAddr, nPort);
}


//上报一个错误信息
int CSnmpAgentObj::SendErrorInfo(const char *szError, const char *szIpAddr, int nPort)
{
    if (m_pSnmpAgent == NULL)
        return ERR_FAIL;

    return m_pSnmpAgent->SendErrorInfo(szError, szIpAddr, nPort);
}

//查询告警
int CSnmpAgentObj::QueryAlarm(SAlarmQueryCond& condition, LIST<SHistoryAlarm> &FoundAlarmList)
{
    if((s_pInstance == NULL) || (s_pInstance->m_pMMLHander == NULL))
        return ERR_FAIL;

    return s_pInstance->m_pMMLHander->QueryAlmLog_Impl(condition, FoundAlarmList);
}