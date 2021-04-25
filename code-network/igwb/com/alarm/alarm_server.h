// AlarmServer.h: interface for the CAlarmServer class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _ALARM_SERVER
#define _ALARM_SERVER

#include "../include/frame.h"
#include "../include/resource.h"

//by ldf 2002.6.7 对应问题单D14393
#include "../include/toolbox.h"
#include "active_alarm_list.h"
//修改完毕


//by lx 2003-09-28 SWPD01787
#ifdef _PLATFORM_WIN32
#define DELIMIT_CHAR    '\\'
#else
#define DELIMIT_CHAR    '/'
#endif /* _PLATFORM_WIN32 */

class CHDMonitorProxy;

class CAlarmHandler;
typedef LIST <CAlarmHandler*>  ALARM_HANDLER_LIST;


class CAlarmServer : public CWorkModule
{
public:
    void ExitInstance(void);
    int InitInstance(void);
    CAlarmServer();
    virtual ~CAlarmServer();
    void AddBinAlarm(SInnerAlarm InnerAlarm);    

protected:
    MODULE_TYPE GetModuleType();

    void OnTimer(const long lTimerID);

    void ParseMsg(MSG_INTRA *pFrame);

    long m_nTimer1SecID;//unit Test change from Int to Long
    //张玉新 2003-04-22
    BOOL IsHex(const char* szData);    
    void HexStringToBinString(char* pHexString, char* pBinString);

private:
    ALARM_HANDLER_LIST m_AlarmHandlers;

    CHDMonitorProxy*   m_pHDMonitor;  //硬件检测器

};

#endif
