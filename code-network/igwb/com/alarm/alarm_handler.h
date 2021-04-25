#ifndef _ALARM_HANDLER
#define _ALARM_HANDLER
// AlarmHandler.h: interface for the CAlarmHandler class.
//
//////////////////////////////////////////////////////////////////////
#include "../include/frame.h"
#include "../utility/mml_report.h"
#include "../utility/mml_para.h"
#include "active_alarm_list.h"
#include "../include/resource.h"
#include "resource.h"
#include "alarm_server.h"

enum ALARM_PRODUCT_TYPE
{
    ALARM_PRODUCT_CG = 65535,
    ALARM_PRODUCT_CCS = 1
};

class CAlarmHandler
{
public:
    void SetCfgPath(const char *pcCfgPath);

    virtual void HandleFrameFromMML(MSG_INTRA *pFrame);
    virtual void HandleFrameFromApp(MSG_INTRA *pFrame);

    virtual void AlarmReceived(MSG_INTRA *pFrame);
    virtual UINT4  RollBackCsn(void);
    virtual UINT4 CreateCsn(void);

    //by ldf 2004-01-30 OM整改
    virtual UINT4  RollBackSyn(void);
    virtual UINT4 CreateSyn(void);

    //unit test change * ==> ** 这样才能改变指针的值
    virtual ALARM_ADD_RESULT AddAlarm(SInnerAlarm* pNewAlarm, SActiveAlarm** ppActiveAlarm, int  nRecoverType = NOT_RECOVER);
    //end

    void DispatchFrame(MSG_INTRA *pFrame);
    virtual void Timer1Sec();
    virtual int Init(const char* szCfgPath = S_ALARM_CONFIG_PATH, 
                     int nConnectedMode = NO_ALARM_BOX,
                     STRING szPortName = S_ALARM_BOX_PORT_NAME_DEFAULT);    
    CAlarmHandler(int BillsvrNo, CAlarmServer* pAlarmServer);
    virtual ~CAlarmHandler();
    
    void HandleCmdLstAlm(MSG_INTRA *pFrame);
    void HandleCmdSetShield(MSG_INTRA *pFrame);   
    CActiveAlarmList* GetActiveAlarmList();    

protected:
    void TRACEDebug(const char *szModule,const char * format_str,...);
    BOOL m_bTraceDebug; //告警TRACE输出开关 张玉新联调增加
    CAlarmServer* m_pAlarmServer;    
    CActiveAlarmList m_ActiveAlarmList;
    UINT4 m_uCsn;
    CAlarmConfig* m_pAlarmCfg;
    UINT4 m_uBillsvrNo;
    STRING m_sAlarmCfgPath;
    STRING m_sINIPath;
    LIST<STRING> m_AlarmItems;       
    ACE_Mutex m_Mutex;
    char  m_szAlarmShield[128]; //本接入点的告警屏蔽码
    
};

#endif
