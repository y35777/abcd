#ifndef _MML_ALARM_HANDLER
#define _MML_ALARM_HANDLER
// MMLAlarmHandler.h: interface for the CMMLAlarmHandler class.
//
//////////////////////////////////////////////////////////////////////
#include "alarm_handler.h"
#include "../include/base_type.h"   // Added by ClassView
#include "../utility/mml_para.h"
#include "alarm_box.h"
#include "history_alarm.h"
#include "alarm_mml_report.h"

#define  ONE_DAY            (24*60*60)    //1天所包含的秒数
#define  THIRTY_DAY         (30*ONE_DAY)  //30天所包含的秒数
#define  ALARM_DEL_NUM      300           //告警条目超过上限时，一次性删除的告警个数
#define  ALARM_UPPER_LIMIT  5000          //告警条目上限
#define  DEFAULT_VALUE      -1

#define LATEST_ALARM_HIS_VERSION 1

class CSnmpAgentObj;

class CMMLAlarmHandler : public CAlarmHandler
{
public:
    //virtual void TestAckAlm(SHistoryAlarm *pHistory);
    void RECOVERY_FAULTALARM(SHistoryAlarm *pHistoryAlarm, CHistoryAlarm *pHistoryAlarmLib,
                             int  nRecoverType = AUTO_RECOVER);

    /////////////////////----判断函数 ----////////////////////////////////////////////////////
    int IsInTimeRange(SHistoryAlarm *pHistory,tm *pStart,tm*pEnd);
    int IsInDateRange(SHistoryAlarm *pHistory,tm *pStart,tm*pEnd);
    int IsTime(tm *pDate);
    int IsDate(tm *pDate);
    int AlmDateEqual(SHistoryAlarm *pHistory,tm *pCommit);
    int AlmLevelTell(UINT2 uAlmLevel,int *pAlmLevelArray);

    ////////////////////----取参数函数----/////////////////////////////////////////////
    void GetIntArrayValue(CParseResult *pParaResult, int nParaID,
                          int *pArray, int nArraySize, int nDefPos);

    void GetTimeValue(CParseResult *pParaResult, int nParaID, tm *pTime);
    void GetDateValue(CParseResult *pParaResult, int nParaID, tm *pDate);

    //by ldf 2004-01-30 OM整改
    SHistoryAlarm * CreateHistoryAlarm(SActiveAlarm *pActive,int  nRecoverType = AUTO_RECOVER);
    //end
    void SendMessageToMML(const char *pMMLReport,UINT2 uAppType, UINT4 uIPAddr, UINT2 uClientNo);    
    int RestoreAllFaultAlarm();
    void OnTimer1Day();
    void BroadCastMMLReport(const char* szReport);
    int Init(const char *szCfgPath = S_ALARM_CONFIG_PATH, 
             int nConnectedMode = NO_ALARM_BOX,
             STRING szPortName = S_ALARM_BOX_PORT_NAME_DEFAULT); 
	//by	杨善	2004-5-19	  对应问题单SWPD05267
	//往活动告警队列中加入一条新的告警
    virtual ALARM_ADD_RESULT AddAlarm(SInnerAlarm* pNewAlarm, SActiveAlarm** ppActiveAlarm, int nRecoverType = NOT_RECOVER);
	//the end	2004-5-19	  对应问题单SWPD05267

    ///////////////////----重载基类的函数----//////////////////////////////////////////
    UINT4 RollBackCsn(void);
    UINT4 CreateCsn(void);
    //by ldf 2004-01-30 OM整改
    UINT4 RollBackSyn(void);
    UINT4 CreateSyn(void);
    //end

    void HandleFrameFromMML(MSG_INTRA *pFrame);
    void HandleFrameFromApp(MSG_INTRA *pFrame);
    void Timer1Sec();

    ///////////////////----构造/析构函数----///////////////////////////////////////////
    CMMLAlarmHandler(int BillsvrNo, CAlarmServer* pAlarmServer, const char* szAlarmShield, char *pcHisPath = S_ALARM_ALMHISTORY);    
    virtual ~CMMLAlarmHandler();
    int QueryAlmLog_Impl(SAlarmQueryCond &condition, LIST<SHistoryAlarm> &FoundAlarmList);

protected:

    BOOL ChangeIntoLatesetAlarmHis();
    BOOL FormatChange(UINT4 uVer, FILE* pAlarmFile, FILE* pAlarmFileTmp);
    
    int HandleCmdDefault(MSG_INTRA *pFrame);
    int HandleCmdAlarmAckEVT(MSG_INTRA *pFrame);
    int HandleCmdAlarmAck(MSG_INTRA *pFrame);
    int HandleCmdAlarmDelete(MSG_INTRA *pFrame);
    int HandleCmdAlarmClear(MSG_INTRA *pFrame);
    void HandleCmdAlarmDetail(MSG_INTRA *pFrame);
    void HandleCmdAlarmQueryLOG(MSG_INTRA *pFrame);
    void HandleCmdAlarmQuery(MSG_INTRA *pFrame);
    //by ldf 2004-01-30 OM整改
    int HandleCmdAlarmCHK(MSG_INTRA *pFrame);
    //end

    void HandleCmdIGWAlarmQuery(MSG_INTRA *pFrame);
    void HandleCmdIGWAlarmQueryLOG(MSG_INTRA *pFrame);
    int HandleCmdIGWAlarmDelete(MSG_INTRA *pFrame);


    CHistoryAlarm *m_pHistoryAlarm;
    CAlarmBox *m_pAlarmBoxByCG, *m_pAlarmBoxByLMT; //张玉新 告警箱添加代码    
    UINT4 m_nActiveTime;
    UINT4 m_nTimer1Day;
    UINT2 m_nPageSize;
    UINT4 m_nReportCsn;
    STRING sHisPath;

protected:
    class CFindListNodeByCsn
    {
        friend class CMMLAlarmHandler;
    public:
        int operator ()(SActiveAlarm *pActive);
        UINT4 m_uCsn;
    } m_nFindNode;

    int m_nTimer1DayCount;
    UINT4 Diff(SActiveAlarm *pAlarm);
    CAlarmMMLReport *m_pAlarmMMLReport;

    //by ldf 2002-10-29 对应问题单 D18685
    LIST<STRING> CsnAcked ;
    //修改完毕

    //by ldf 2004-01-30 OM整改
    int m_nMMLBaseID; //从M2000
    //end
	
    CSnmpAgentObj* m_pSNMPAgent;  //SNMP告警代理
};

#endif
