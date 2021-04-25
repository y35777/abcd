 #include "snmp_alarm.h"
#include "sun_alarm.h"

IHardwareAlarm *g_pHDAlarm = NULL;           

IHardwareAlarm *GetHDAlarm(void)
{
    if (g_pHDAlarm == NULL)
    {
        int nRet = ACE::init();
        if (nRet < 0)
        {
            return NULL;
        }

#ifdef WIN32
        g_pHDAlarm = new CSnmpAlarm();
#elif defined(_PLATFORM_SOLARIS)
        g_pHDAlarm = new CSunAlarm();
#else
        g_pHDAlarm = NULL;
#endif 
    }

    return g_pHDAlarm;
}

void Destruct()
{
    if (g_pHDAlarm)
    {
        delete g_pHDAlarm;
        g_pHDAlarm = NULL;
        //ACE::fini();
    }
}

CHardwareAlarm::CHardwareAlarm()
{
    m_IsExit           = false; 
	m_nScheduleRunning = false;  
	m_hScheduleThread  = 0;
	m_pGlobalFunc      = NULL;
    m_strError         = "";   
}  

CHardwareAlarm::~CHardwareAlarm()
{
    ExitRcvThread();
    m_pGlobalFunc = NULL;    
}
//---------------------------------------------------
//  函数名:     CHardwareAlarm::StartRcvThread（）
//  功能:       创建硬件告警的监视的线程
//  输入参数:   无
//  输出参数:   无
//  调用函数：  
//  返回值:     BOOL
//              FALSE 表明线程启动不成功
//              TRUE  表明线程启动成功
//----------------------------------------------------
bool CHardwareAlarm::StartRcvThread(const char *szCfgPath, int nPort)
{	
	//创建告警处理线程
    int nRet =m_ThreadManager.spawn((ACE_THR_FUNC)ScheduleExecutionThread,
                                     (void*)this,
                                     THR_NEW_LWP | THR_JOINABLE,
                                     &m_hScheduleThread);
    
    if(nRet < 0)
    {
        m_strError = "create ScheduleExecutionThread failed! ";
        m_strError += ACE_OS::strerror(ACE_OS::last_error());
        
        m_IsExit = true;
        ExitRcvThread();
        return false;
    }

    m_nScheduleRunning = true;
    return true;
}


//---------------------------------------------------
//  函数名:     CSnmpAlarm::ExitRcvThread（）
//  功能:       退出接收线程（包括Trap线程和Schedule线程）
//  输入参数:   无
//  输出参数:   无
//  调用函数：  无
//  返回值:     BOOL
//----------------------------------------------------
bool CHardwareAlarm::ExitRcvThread()
{
	if (m_nScheduleRunning)
	{
		m_IsExit = true; //设置退出线程标志

	    //关闭m_hTrapThread句柄
	    if (m_hScheduleThread != 0)
	    {
		    m_ThreadManager.wait();
            m_hScheduleThread = 0;
	    }

        m_nScheduleRunning = false;
    }
    
	return true;
}

//---------------------------------------------------
//  函数名:     CSnmpAlarm::RegisterFunc（）
//  功能:       调用注册函数
//  输入参数:   FUNC p
//  输出参数:   无
//  调用函数：  无
//  返回值:     int
//----------------------------------------------------
bool CHardwareAlarm::RegisterFunc(FUNC p)
{
   	m_pGlobalFunc = p;
	return true;
}

bool CHardwareAlarm::SetAlarm(SWarnData& TempWarnData, int iParasLength)
{
    // 判断参数长度是否有效
	if ( (iParasLength > PARAM_MAX_LEN)
		|| (iParasLength < 0) )
	{
		return false;
	}	

	//如果队列大于50，从头部删除元素，使队列元素个数小于等于50
	while(m_WarnList.size() >= BUFFERLIST_MAX_SIZE)
	{
		m_WarnList.pop_front();
	}

	//添加一个元素到队列尾部
	m_WarnList.AddElement(TempWarnData);
	return true;
}

bool CHardwareAlarm::GetAlarm(SWarnData& TempWarnData)
{    
    if (m_WarnList.size() <= 0)
    {
        return false; 
    }

    // 拷贝队列头元素到TempWarnData   
    TempWarnData = m_WarnList.front();  //将队列头元素拷贝到TempWarnData中
    m_WarnList.pop_front();             //从队列头部移走一个元素    
    return true;
}

//---------------------------------------------------
//  函数名:     CHardwareAlarm::ScheduleExecutionThread（）
//  功能:       告警处理线程执行函数
//  输入参数:   无
//  输出参数:   无
//  调用函数：  
//  返回值:     DWORD
//----------------------------------------------------
void *CHardwareAlarm::ScheduleExecutionThread(void *param)
{
	CHardwareAlarm *This = (CHardwareAlarm *)param;
	if(This)
	{
		This->Schedule();
        This->m_nScheduleRunning = false;
    }
    else
    {
        This->m_strError = "Param in ScheduleExecutionThread is NULL !";
    }
    
	return 0;
}
//---------------------------------------------------
//  函数名:     CHardwareAlarm::Schedule(）
//  功能:       告警处理函数。
//              等待有告警信息写入队列，如果有告警信息
//              写入，m_hEvent设置为signal状态，调用
//              m_pGlobalFunc函数，通知应用程序从队列中
//              取出数据。另外，设置了30秒的超时等待，是为了能够
//              控制线程的运行，在需要时退出线程，防止线程阻塞
//  输入参数:   无
//  输出参数:   无
//  调用函数：  无
//  返回值:     void
//----------------------------------------------------
void  CHardwareAlarm::Schedule()
{
    ACE_Time_Value tv;    
    while (!m_IsExit)
    { 
		//等待有告警写入队列，超时设置5秒
        tv = ACE_OS::gettimeofday();
        tv += ACE_Time_Value(5);
		m_WarnList.m_hEvent.wait(&tv);
        
        if (m_pGlobalFunc)
        {
            m_pGlobalFunc(this);
        }    
	}
}


//*****************************************************
//  函 数 名:   CHardwareAlarm::GetLastError()
//  功    能:   获取出错信息
//  输入参数:   无
//  输出参数:   无
//  调用函数:   无
//  返 回 值:   char*
//*****************************************************
const char* CHardwareAlarm::GetLastError() const
{
    return m_strError.c_str();
}