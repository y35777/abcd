#ifndef _ALARM_MML
#define _ALARM_MML
// AlarmMMLReport.h: interface for the CAlarmMMLReport class.
//
//////////////////////////////////////////////////////////////////////

#include "../utility/mml_report.h"
#include "resource.h"
#include "../alarm_util/alarm_config.h"
#include "../alarm_util/alarm_struct.h"

class CAlarmConfig;
class CAlarmMMLReport : public CMMLReport
{
public:
    void MakeDeleteAlarmBody(char*start_date,char*end_date,int MH);

    void MakeRecoverAlarmBody(int start_csn,int end_csn,int MH);

    void MakeQueryAlarmBody (SActiveAlarm *pAlarm,CAlarmConfig *pConfig);

    void MakeQueryAlarmBody (SHistoryAlarm *pAlarm,CAlarmConfig *pConfig);

    void MakeDetailAlarmBody(SHistoryAlarm *pAlarm, CAlarmConfig *pConfig);

    //by ldf 2002-10-29 对应问题单 D18685
    //by ldf 2002.6.7 对应问题单D14393
    void MakeAlarmEventAckBody (const char* szConfirmOpr, const char* szAlarmType, const char* szCsn);
    //修改完毕
    void MakeAlarmFaultAckBody (const char* szConfirmOpr, const char* szAlarmType, LIST <STRING>& CsnAcked);
    //修改完毕

    //by ldf 2004-01-30 OM整改
    void MakeAlarmBroad (SActiveAlarm* pAlarm,CAlarmConfig *pConfig,int  nRecoverType = NOT_RECOVER);
    //end

    void MakeAlarmHead(SAlarmDetail *pAlarm);

    void MakeReportHead2(SActiveAlarm *pActive);
    //by ldf 2002-10-29 对应问题单 D18685
    virtual void MakeReportItem(const char* szTitle,
                                const char* szContent,
                                int nFirstFormatLen = 16,
                                int nFirstAign = ALIGN_LEFT,
                                int nSecondFormatLen = 55,
                                int nSecondAlign = ALIGN_LEFT);
    //修改完毕
    virtual void MultiLine(const char* sInput,
                           STRING &sOutput,
                           int LEFT_MARGIN = 10,
                           int ROW_LENGTH = 80);


    CAlarmMMLReport(const char* szName);
    virtual ~CAlarmMMLReport();

//by ldf 2004-01-30 OM整改
    void SetMMLBaseAlarmID(UINT4 uMMLBaseID);
private:
    unsigned int m_uMMLBaseAlarmID;		//MML的告警基准同步号
//end

};
#endif

