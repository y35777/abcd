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

const int BUFFERLIST_MAX_SIZE   = 50;   // ���巢�ͻ����������Ԫ�ظ���Ϊ50

class CHardwareAlarm : public IHardwareAlarm
{
public:
    CHardwareAlarm();
    virtual ~CHardwareAlarm();       //�������� 
    
    FUNC m_pGlobalFunc;             //ע�ắ��

    //����Ӳ���澯�ļ��ӵ��߳�
    virtual bool StartRcvThread(const char *szCfgPath, 
                                int nPort = 162); 

    //�˳�Ӳ���澯�ļ��ӵ��߳�
    virtual bool ExitRcvThread();   
    

    //�Ӷ�����ȡ���澯֡
    virtual bool GetAlarm(SWarnData& alarm);
    
    //ע�ắ��
    virtual bool RegisterFunc(FUNC p);
    
    //��ȡ������Ϣ
    virtual const char* GetLastError(void) const;

protected:
    //�Ѹ澯֡��ӵ�������
    virtual bool SetAlarm(SWarnData&, int iParasLength); 

    //�澯�����߳�ִ�к���
	static void *ScheduleExecutionThread(void *param); 

    //�澯������ 
	void Schedule();                                         

public:
	bool            m_IsExit;                        //��ʶ�߳��Ƿ��˳�
	bool            m_nScheduleRunning;              //��ʶSchedule�߳��Ƿ�����

protected:
    ACE_Thread_Manager        m_ThreadManager;    //�̹߳�����
    ACE_thread_t              m_hScheduleThread;   //�澯�����߳̾��

    CBufferList<SWarnData>    m_WarnList;          //�澯֡����
    std::string               m_strError;          // ������Ϣ��¼
};

#endif //_HARDWARE_MONITOR_H_
