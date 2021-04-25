#ifndef _HISTORY
#define _HISTORY
// HistoryAlarm.h: interface for the CHistoryAlarm class.
//
//////////////////////////////////////////////////////////////////////
#include "../alarm_util/alarm_config.h"
#include "../alarm_util/alarm_struct.h"
#include "alarm_encap.h"
#include "alarm_bin_util.h"

//�ڵ�ǰλ��д��һ��������¼
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
    //by ldf 2004-01-30 OM����
    int FindRecordBySyn(UINT4 uSyn);
    //end

    int AddRecord(SHistoryAlarm  pAlarm[], UINT4 uNum);
    int AddRecord(SHistoryAlarm  *pAlarm);

    UINT4 GetCsn();
    int WriteCsn(UINT4 csn);
    UINT4 CreateCsn();
    UINT4 RollBackCsn();

    //by ldf 2004-01-30 OM����
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
    // ��ø澯�ļ��ĺ�׺��
    inline const char* GetAlarmFile( void )
    {
        if(m_bUseXml)
        {
            return "alarmhistory.xml";
        }

        return "alarmhistory.his";
    }

    // ��ø澯�ļ��Ķ�д��װ����
    inline IAlarmUtil* CreateAlarmUtilObj ( void )
    {
        if(m_bUseXml)
        {
            return m_alarmUtilEncap.CreateAlarmUtil();
        }
        
        return new CAlarmBinUtil();
    }

    char            m_szAlarmFile[MAX_PATH];//��Ա����m_szAlarmFile������ʷ�澯�Ĵ洢·��
    UINT4           m_uCsn;                 //�澯��ˮ��,��Ч��Χ��100~2147483646
    //by ldf 2004-01-30 OM����
    UINT4	 m_uSyn;                        //�澯ͬ����,��Ч��Χ��100~2147483646


    int             m_nAlarmUpperLimit;     //�澯�ļ��д�Ÿ澯��Ŀ�����ޡ�
    int             m_nAlarmDelNum;         //�澯��Ŀ��������ʱ��һ����ɾ���ĸ澯����
    int             m_nRecordNum;           //��¼��ǰ�澯���ݿ��ļ��еĸ澯��¼����
    int             m_nReadIndex;           //
    CAlarmUtilEncap m_alarmUtilEncap;
	IAlarmUtil*     m_pAlarmUtil;           //���ڸ澯�ļ������Ķ���
    BOOL            m_bUseXml;

    //by ldf 2004-01-30 OM����
    UINT4  m_uMMLAlmBaseID; //M2000ͨ��CHK ALM��������Ļ�׼ֵ,ֻ��¼��һ��
                          //��Ĭ��Ϊ0����0ʱ��ʾ�Ѿ����չ��ˣ����ٽ��գ�
    //end

};

#endif
