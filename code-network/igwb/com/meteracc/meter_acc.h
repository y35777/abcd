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

//���º����̳���CWorkModule
protected:
    void ParseMsg(MSG_INTRA* pMsg);      
    int InitInstance(void);    
    void ExitInstance(void);        
    MODULE_TYPE GetModuleType(void);
    void OnTimer(const long nTimerID); 
    
private:
    long            m_nTimerID;             //10�붨ʱ��ID
    CMeterAccTask*  m_pMeterAccTask;        //�ƴα��ۼ�����
    int             m_nServerNo;            //������ʶ
};

#endif //__METER_ACCUMULATION_H__