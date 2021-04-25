#ifndef __METER_ACCUMULATION_H__
#define __METER_ACCUMULATION_H__

#include "../include/frame.h"
#include "../include/toolbox.h"

class CMeterAccTask;

class CMeterAcc : public CWorkModule
{
public:
    CMeterAcc();
    virtual ~CMeterAcc();

//以下函数继承于CWorkModule
protected:
    void ParseMsg(MSG_INTRA* pMsg);      
    int InitInstance(void);    
    void ExitInstance(void);        
    MODULE_TYPE GetModuleType(void);
    void OnTimer(const long nTimerID); 
    
private:
    long            m_nTimerID;             //10秒定时器ID
    CMeterAccTask*  m_pMeterAccTask;        //计次表累加任务
    int             m_nServerNo;            //主备标识
};

#endif //__METER_ACCUMULATION_H__