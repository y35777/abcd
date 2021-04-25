// ActiveAlarmList.h: interface for the CActiveAlarmList class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _ALARM_LIST
#define _ALARM_LIST

#include "../alarm_util/alarm_config.h"
#include "../alarm_util/alarm_struct.h"

class CActiveAlarmList:public LIST <SActiveAlarm *>
{
public:

    int InitList(SActiveAlarm& sActiveAlarm,CAlarmConfig *pConfig);

    SActiveAlarm * AddFatalAlarm(BYTE yAlarmType, UINT2 uAlarmID, BYTE* yAlarmPara);
    void RemoveAlarm(BYTE yAlarmType, UINT2 uAlarmID, BYTE* yAlarmPara);
    void RemoveAlarm(SActiveAlarm* pActiveAlarm);

    SActiveAlarm * Find(SActiveAlarm* pActiveAlarm);
    SActiveAlarm *Find(BYTE yAlarmType,UINT2 uAlarmID, BYTE* yAlarmPara);
    SActiveAlarm *Find(SInnerAlarm* pInnerAlarm);

    int AddAlarm(SInnerAlarm* pInnerAlarm,SActiveAlarm **ppActive, int  nRecoverType = NOT_RECOVER);


    //constructor and destructor
    CActiveAlarmList();
    explicit CActiveAlarmList(SActiveAlarm& sActiveAlarm);
    virtual ~CActiveAlarmList();

protected:

    SActiveAlarm m_ActiveAlarmTmpl;
    CAlarmConfig *m_pAlmCfg;

    SActiveAlarm * AddRestoreAlarm(BYTE yAlarmType, UINT2 uAlarmID, BYTE* yAlarmPara,int nRecoverType = AUTO_RECOVER);

    SActiveAlarm * AddEventAlarm(BYTE yAlarmType, UINT2 uAlarmID, BYTE* yAlarmPara);

    SActiveAlarm * CreateActiveAlarm(BYTE yAlarmType, UINT2 uAlarmID, BYTE* yAlarmPara,int nRecoverType = NOT_RECOVER);
};

#endif
