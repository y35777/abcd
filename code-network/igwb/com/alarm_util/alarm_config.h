#ifndef _ALARM_CONFIG
#define _ALARM_CONFIG
// AlarmConfig.h: interface for the CAlarmConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "../include/base_type.h"


struct SAlarmExplain;
typedef MAP<UINT2, SAlarmExplain*,LESS <int> > ALARM_EXPLAIN_MAP;

class CAlarmCfgVisitor;
class IGWB_Svc_Export CAlarmConfig
{
public:

    BYTE GetAlarmType(UINT2  uAlarmID);
    SAlarmExplain *GetAlarmExplain(UINT2  uAlarmID);
    int LoadConfig();
    //constructor and destructor
    CAlarmConfig(const char* szCfgFile);
    virtual ~CAlarmConfig();
    
    void AlarmVisit(CAlarmCfgVisitor visitor); //by lx SWPD04479
    void GetAlarmExplainList(LIST<SAlarmExplain>& AlarmExplainList);    
    
private:

    ALARM_EXPLAIN_MAP m_AlarmExplainMap;
    UINT2 m_uMaxAlarmID;
    char m_szCfgFile[MAX_PATH];
};

#endif
