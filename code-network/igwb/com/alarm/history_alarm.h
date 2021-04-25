#ifndef _HISTORY
#define _HISTORY
// HistoryAlarm.h: interface for the CHistoryAlarm class.
//
//////////////////////////////////////////////////////////////////////
#include "../alarm_util/alarm_config.h"
#include "../alarm_util/alarm_struct.h"
#include "alarm_encap.h"
#include "alarm_bin_util.h"

//在当前位置写入一条话单记录
#define WRITE_A_ALARM(pAlarm, pAlarmFile) (fwrite(pAlarm,SIZEOF_HISTORYALARM,1,m_pAlarmFile) == 1)

class CHistoryAlarm
{
public:
    int Sweep();

    int Seek(long offset, int origin = SEEK_SET);

    int GetRecord(SHistoryAlarm*pData,  UINT4& uNum);
    UINT4 GetRecordNum(void);

    int DeleteRecords(const UINT2  uRecordNum);
    int DeleteRecord(const UINT4  uSyn);

    int WriteRecord(SHistoryAlarm  *pAlarm, UINT4 uNum);
    int RewriteRecord(SHistoryAlarm *pAlarm);

    int FindRecord(UINT4 uCsn);
    //by ldf 2004-01-30 OM整改
    int FindRecordBySyn(UINT4 uSyn);
    //end

    int AddRecord(SHistoryAlarm  pAlarm[], UINT4 uNum);
    int AddRecord(SHistoryAlarm  *pAlarm);

    UINT4 GetCsn();
    int WriteCsn(UINT4 csn);
    UINT4 CreateCsn();
    UINT4 RollBackCsn();

    //by ldf 2004-01-30 OM整改
    UINT4 GetSyn();
    int WriteSyn(UINT4 syn);
    UINT4 CreateSyn();
    UINT4 RollBackSyn();
    int WriteMMLBaseID(UINT4 uMMLBaseID);
    UINT4 GetMMLBaseID();
    //end


    int Close();
    int Open();

    CHistoryAlarm(const char* szAlarmFile,int nAlarmDelNum,int nAlarmUpperLimit);
    virtual ~CHistoryAlarm();
private:
    // 获得告警文件的后缀名
    inline const char* GetAlarmFile( void )
    {
        if(m_bUseXml)
        {
            return "alarmhistory.xml";
        }

        return "alarmhistory.his";
    }

    // 获得告警文件的读写封装对象
    inline IAlarmUtil* CreateAlarmUtilObj ( void )
    {
        if(m_bUseXml)
        {
            return m_alarmUtilEncap.CreateAlarmUtil();
        }
        
        return new CAlarmBinUtil();
    }

    char            m_szAlarmFile[MAX_PATH];//成员变量m_szAlarmFile代表历史告警的存储路径
    UINT4           m_uCsn;                 //告警流水号,有效范围是100~2147483646
    //by ldf 2004-01-30 OM整改
    UINT4	 m_uSyn;                        //告警同步号,有效范围是100~2147483646


    int             m_nAlarmUpperLimit;     //告警文件中存放告警条目的上限。
    int             m_nAlarmDelNum;         //告警条目超过上限时，一次性删除的告警个数
    int             m_nRecordNum;           //记录当前告警数据库文件中的告警记录数。
    int             m_nReadIndex;           //
    CAlarmUtilEncap m_alarmUtilEncap;
	IAlarmUtil*     m_pAlarmUtil;           //用于告警文件操作的对象
    BOOL            m_bUseXml;

    //by ldf 2004-01-30 OM整改
    UINT4  m_uMMLAlmBaseID; //M2000通过CHK ALM命令传过来的基准值,只记录第一次
                          //（默认为0，非0时表示已经接收过了，则不再接收）
    //end

};

#endif
