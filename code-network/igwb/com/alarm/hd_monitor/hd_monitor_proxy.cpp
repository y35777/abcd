/*     
    ��Ʒ��:iGateway Bill V200
    ģ����:�澯
    �ļ���:hd_monitor_proxy.h
    ������Ϣ:
    
    ����CHDMonitorProxy���ʵ��,������ΪӲ����⹦�ܵĴ���
    ���ڽ�Ӳ���澯ת��������ģ�鴦��.
    ����ֻ�ṩһ��ת���澯�Ľӿڣ������ʵ�ֽ������࣮

    �޸ļ�¼:
    ������,��2003-05 ,    ����.
    ������,  2003-11 ,    �Ż����룬����ע��.   
*/

#include "hd_monitor_proxy.h"

#ifdef _PLATFORM_WIN32
    #define HD_MONITOR_DLL       "HDmonitor.dll"
#elif defined(_PLATFORM_SOLARIS) || defined (_PLATFORM_LINUX) 
    #define HD_MONITOR_DLL       "libHDmonitor.so"
#elif defined(_PLATFORM_HPUX)
    #define HD_MONITOR_DLL       "libHDmonitor.sl"
#else
    #error macro _PLATFORM_??? undefined.
#endif

CHDMonitorProxy* CHDMonitorProxy::s_pInstance = NULL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHDMonitorProxy::CHDMonitorProxy()
{
    m_pHDAlarm = NULL;
    m_nErrno = 0;
}

CHDMonitorProxy::~CHDMonitorProxy()
{
    m_pHDAlarm = NULL;
}

//��ʼ��
int CHDMonitorProxy::Init(const char *szCfgPath, int nPort)
{
#ifndef USE_WINDOWS_DLL

    //����Ӳ����⶯̬��
    int nRet = m_dll.open(HD_MONITOR_DLL);
    if(nRet != 0)
    {
        m_strError = ACE_OS::strerror(ACE_OS::last_error());
        m_nErrno = ERR_LOAD_DLL_FAIL;  //m_dll.open() occur error
        return m_nErrno;
    }

    //��λ��ȡIHardwareAlarm����
    IHDAlarmGet pGet = NULL;
    pGet = (IHDAlarmGet)m_dll.symbol("GetHDAlarm");
    if(pGet == NULL)
    {
        m_strError = ACE_OS::strerror(ACE_OS::last_error());
        m_nErrno = ERR_FIND_FUN_IN_DLL_FAIL; 
        m_dll.close();
        return m_nErrno;
    }

    //�õ�IHardwareAlarm����
    m_pHDAlarm = pGet();
    if(m_pHDAlarm == NULL)
    {
        m_strError = "Get HDAlarm Error";
        m_nErrno = ERR_GET_SNMP_HD_MONITOR_FAIL;
        m_dll.close();
        return m_nErrno;
    }

    //ע�ắ��
    if(m_pHDAlarm->RegisterFunc(AsyncNotify))
    {
        //�����߳�
        if(m_pHDAlarm->StartRcvThread(szCfgPath, nPort))
        {
            m_strError = "Succeed in starting thread";
            m_nErrno = 0;
            return m_nErrno;
        }
    }
    
    //�������
    m_strError = m_pHDAlarm->GetLastError();
    Destructor Destruct = (Destructor)m_dll.symbol("Destruct");
    if(Destruct)
    {
        Destruct();
    }
    else
    {
        m_strError += ACE_OS::strerror(ACE_OS::last_error());
    }
    m_dll.close();

    m_nErrno = ERR_OTHER;  //other error
    return m_nErrno;

#else
    return 0;
#endif

}

//�˳��̣߳�������ʵ��
void CHDMonitorProxy::Exit()
{
#ifndef USE_WINDOWS_DLL    
    if(s_pInstance)
    {
        if(m_nErrno == 0) //��ʼ���ɹ�������߳�ʧ��
        {
            m_pHDAlarm->ExitRcvThread();
            Destructor Destruct = (Destructor)m_dll.symbol("Destruct");
            if(Destruct)
            {
                Destruct();
            }
            else
            {
                //m_strError = m_dll.error();
                m_strError = ACE_OS::strerror(ACE_OS::last_error());
            }
            m_dll.close();
        }
        delete this;
        s_pInstance = NULL;
    }
#endif
}


//�澯����ʱ�Ļص�����
int CHDMonitorProxy::AsyncNotify(IHardwareAlarm *pHDAlarm)
{
    SWarnData Alarm;
    int nRet = pHDAlarm->GetAlarm(Alarm);
    if(nRet)
    {
        CHDMonitorProxy *pInst = CHDMonitorProxy::Instance();
        if(pInst != NULL)
        {
            return pInst->ProcessAlarm(Alarm);
        }
        else
        {
            return ERR_OTHER;
        }
    }

    return 0;
}

//�澯������
int CHDMonitorProxy::ProcessAlarm(SWarnData &Alarm)
{
    return 0;
}