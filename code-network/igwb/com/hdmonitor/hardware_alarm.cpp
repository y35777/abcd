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
//  ������:     CHardwareAlarm::StartRcvThread����
//  ����:       ����Ӳ���澯�ļ��ӵ��߳�
//  �������:   ��
//  �������:   ��
//  ���ú�����  
//  ����ֵ:     BOOL
//              FALSE �����߳��������ɹ�
//              TRUE  �����߳������ɹ�
//----------------------------------------------------
bool CHardwareAlarm::StartRcvThread(const char *szCfgPath, int nPort)
{	
	//�����澯�����߳�
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
//  ������:     CSnmpAlarm::ExitRcvThread����
//  ����:       �˳������̣߳�����Trap�̺߳�Schedule�̣߳�
//  �������:   ��
//  �������:   ��
//  ���ú�����  ��
//  ����ֵ:     BOOL
//----------------------------------------------------
bool CHardwareAlarm::ExitRcvThread()
{
	if (m_nScheduleRunning)
	{
		m_IsExit = true; //�����˳��̱߳�־

	    //�ر�m_hTrapThread���
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
//  ������:     CSnmpAlarm::RegisterFunc����
//  ����:       ����ע�ắ��
//  �������:   FUNC p
//  �������:   ��
//  ���ú�����  ��
//  ����ֵ:     int
//----------------------------------------------------
bool CHardwareAlarm::RegisterFunc(FUNC p)
{
   	m_pGlobalFunc = p;
	return true;
}

bool CHardwareAlarm::SetAlarm(SWarnData& TempWarnData, int iParasLength)
{
    // �жϲ��������Ƿ���Ч
	if ( (iParasLength > PARAM_MAX_LEN)
		|| (iParasLength < 0) )
	{
		return false;
	}	

	//������д���50����ͷ��ɾ��Ԫ�أ�ʹ����Ԫ�ظ���С�ڵ���50
	while(m_WarnList.size() >= BUFFERLIST_MAX_SIZE)
	{
		m_WarnList.pop_front();
	}

	//���һ��Ԫ�ص�����β��
	m_WarnList.AddElement(TempWarnData);
	return true;
}

bool CHardwareAlarm::GetAlarm(SWarnData& TempWarnData)
{    
    if (m_WarnList.size() <= 0)
    {
        return false; 
    }

    // ��������ͷԪ�ص�TempWarnData   
    TempWarnData = m_WarnList.front();  //������ͷԪ�ؿ�����TempWarnData��
    m_WarnList.pop_front();             //�Ӷ���ͷ������һ��Ԫ��    
    return true;
}

//---------------------------------------------------
//  ������:     CHardwareAlarm::ScheduleExecutionThread����
//  ����:       �澯�����߳�ִ�к���
//  �������:   ��
//  �������:   ��
//  ���ú�����  
//  ����ֵ:     DWORD
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
//  ������:     CHardwareAlarm::Schedule(��
//  ����:       �澯��������
//              �ȴ��и澯��Ϣд����У�����и澯��Ϣ
//              д�룬m_hEvent����Ϊsignal״̬������
//              m_pGlobalFunc������֪ͨӦ�ó���Ӷ�����
//              ȡ�����ݡ����⣬������30��ĳ�ʱ�ȴ�����Ϊ���ܹ�
//              �����̵߳����У�����Ҫʱ�˳��̣߳���ֹ�߳�����
//  �������:   ��
//  �������:   ��
//  ���ú�����  ��
//  ����ֵ:     void
//----------------------------------------------------
void  CHardwareAlarm::Schedule()
{
    ACE_Time_Value tv;    
    while (!m_IsExit)
    { 
		//�ȴ��и澯д����У���ʱ����5��
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
//  �� �� ��:   CHardwareAlarm::GetLastError()
//  ��    ��:   ��ȡ������Ϣ
//  �������:   ��
//  �������:   ��
//  ���ú���:   ��
//  �� �� ֵ:   char*
//*****************************************************
const char* CHardwareAlarm::GetLastError() const
{
    return m_strError.c_str();
}