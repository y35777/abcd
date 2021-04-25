#ifndef _HARDWARE_ALARM_H_
#define _HARDWARE_ALARM_H_

#include <string.h>
#include <iostream.h>
#include <stdlib.h>
#include <list>

#include "ace/ACE.h"
#include "ace/Thread_Manager.h"
#include "ace/Init_ACE.h"

#include "buffer_list.h"
#include "hdmonitor.h"



extern "C" ACE_Svc_Export IHardwareAlarm *GetHDAlarm(void);
extern "C" ACE_Svc_Export void Destruct();

const int BUFFERLIST_MAX_SIZE   = 50;   // 定义发送缓冲队列最大的元素个数为50

class CHardwareAlarm : public IHardwareAlarm
{
public:
    CHardwareAlarm();
    virtual ~CHardwareAlarm();       //析构函数 
    
    FUNC m_pGlobalFunc;             //注册函数

    //创建硬件告警的监视的线程
    virtual bool StartRcvThread(const char *szCfgPath, 
                                int nPort = 162); 

    //退出硬件告警的监视的线程
    virtual bool ExitRcvThread();   
    

    //从队列中取出告警帧
    virtual bool GetAlarm(SWarnData& alarm);
    
    //注册函数
    virtual bool RegisterFunc(FUNC p);
    
    //获取出错信息
    virtual const char* GetLastError(void) const;

protected:
    //把告警帧添加到队列中
    virtual bool SetAlarm(SWarnData&, int iParasLength); 

    //告警处理线程执行函数
	static void *ScheduleExecutionThread(void *param); 

    //告警处理函数 
	void Schedule();                                         

public:
	bool            m_IsExit;                        //标识线程是否退出
	bool            m_nScheduleRunning;              //标识Schedule线程是否运行

protected:
    ACE_Thread_Manager        m_ThreadManager;    //线程管理器
    ACE_thread_t              m_hScheduleThread;   //告警处理线程句柄

    CBufferList<SWarnData>    m_WarnList;          //告警帧队列
    std::string               m_strError;          // 出错信息记录
};

#endif //_HARDWARE_MONITOR_H_
