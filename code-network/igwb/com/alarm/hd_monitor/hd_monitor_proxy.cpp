/*     
    产品名:iGateway Bill V200
    模块名:告警
    文件名:hd_monitor_proxy.h
    描述信息:
    
    包含CHDMonitorProxy类的实现,该类作为硬件检测功能的代理，
    用于将硬件告警转发给调用模块处理.
    此类只提供一个转发告警的接口，具体的实现交给子类．

    修改记录:
    张玉新,　2003-05 ,    创建.
    陈凉威,  2003-11 ,    优化代码，增加注释.   
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

//初始化
int CHDMonitorProxy::Init(const char *szCfgPath, int nPort)
{
#ifndef USE_WINDOWS_DLL

    //加载硬件检测动态库
    int nRet = m_dll.open(HD_MONITOR_DLL);
    if(nRet != 0)
    {
        m_strError = ACE_OS::strerror(ACE_OS::last_error());
        m_nErrno = ERR_LOAD_DLL_FAIL;  //m_dll.open() occur error
        return m_nErrno;
    }

    //定位获取IHardwareAlarm函数
    IHDAlarmGet pGet = NULL;
    pGet = (IHDAlarmGet)m_dll.symbol("GetHDAlarm");
    if(pGet == NULL)
    {
        m_strError = ACE_OS::strerror(ACE_OS::last_error());
        m_nErrno = ERR_FIND_FUN_IN_DLL_FAIL; 
        m_dll.close();
        return m_nErrno;
    }

    //得到IHardwareAlarm对象
    m_pHDAlarm = pGet();
    if(m_pHDAlarm == NULL)
    {
        m_strError = "Get HDAlarm Error";
        m_nErrno = ERR_GET_SNMP_HD_MONITOR_FAIL;
        m_dll.close();
        return m_nErrno;
    }

    //注册函数
    if(m_pHDAlarm->RegisterFunc(AsyncNotify))
    {
        //启动线程
        if(m_pHDAlarm->StartRcvThread(szCfgPath, nPort))
        {
            m_strError = "Succeed in starting thread";
            m_nErrno = 0;
            return m_nErrno;
        }
    }
    
    //处理出错
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

//退出线程，并销毁实例
void CHDMonitorProxy::Exit()
{
#ifndef USE_WINDOWS_DLL    
    if(s_pInstance)
    {
        if(m_nErrno == 0) //初始化成功或调用线程失败
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


//告警发生时的回调函数
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

//告警处理函数
int CHDMonitorProxy::ProcessAlarm(SWarnData &Alarm)
{
    return 0;
}