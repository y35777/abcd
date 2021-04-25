/**************************************************************************
 *��Ʒ��    : iGateway Bill V200        
 *ģ����    : ����ģ��(Perf)
 *�ļ���    : perf.h
 *������Ϣ  : ���ļ�����CPerf�Ķ���
 *�汾˵��  : V200R001i00M00B00D1130A   
 *��Ȩ��Ϣ  : ��Ȩ����(c) 2001-2002 ��Ϊ�������޹�˾
 *����      : ������
 *����ʱ��  : 2001-11-05
 *�޸ļ�¼  : 
 **************************************************************************/

#ifndef __PERF_H__
#define __PERF_H__
 
#include "../include/frame.h"
#include "../include/toolbox.h"
#include "collect_statistic.h"
#include <algorithm>
#include "perf_hdr.h"
#include "os_perf_db.h"
#include "task_manager.h"
//������ 2002-04-13����
#ifdef _PLATFORM_WIN32
#include "perf_nt/nt_perf_db.h"
#else
#include "perf_unix/unix_perf_db.h"
#endif
//���ӽ���
#include "../include/perf_item.h"

#include "perf_encap.h"
#include "perf_bin_util.h"

typedef MAP<BYTE,SPerfItem *,LESS<BYTE> > PERF_MAP;

#define AP_CONN_STATISTIC_TIMER		10 * 1000

// �����������ݽṹ��ʱ������յ�����������
typedef struct _PerfBuf
{
    int count;
    SPerfItem* pData;
} SPerfBuf, *PPerfBuf;
 
class CPerf : public CWorkModule
{
public:
    // ���캯��
    CPerf(void);
    
    // ��������
    ~CPerf(void);
    
protected:
    // ��ñ�ģ�������
    MODULE_TYPE GetModuleType(void);
    
    // ��ʼ��
    int InitInstance(void);
    
    // �˳�ʱ��������
    void ExitInstance(void);
    
    // ���ݶ�ʱ��ID�ֱ���ʱ���¼�
    virtual void OnTimer(const long nTimerID);
    
    // ��������ģ���յ�����Ϣ
    virtual void ParseMsg(MSG_INTRA* pMsgParsed);
    
    // ������ģ�鴦��״̬��ѯ��״̬
    void QueryStatus(MSG_INTRA* pMsg);
	void SetStatusInv(MSG_INTRA* pMsg);
	void LstStatusInv(MSG_INTRA* pMsg);
	//��ѯIP����
    void QueryIPAddr(MSG_INTRA *pMsg);
    // �����������������������
    void ProcConnectStatus(const BYTE nAccessPointID, 
                           SPerfItem* pData, 
                           const BYTE nCount);
                           
    // �������Ը����̵���������
    void SavePerfItem(SPerfItem* pItems, const BYTE nCount);
    
	void SetAllAPName(MSG_INTRA *pMsg);
	//��ѯ��·״̬��Ϣ
	void QueryLinkState(MSG_INTRA *pMsg);

	//��ѯ���е�AP����
	void QueryAllAPName(MSG_INTRA *pMsg);

	void DecideLinkStat(BYTE nAPId, SLinkStat& link_stat);

	BYTE GetLinkStat(BYTE nAPId, SLinkStat& link_stat);

	void SendMMLReport(const char* szReport);
	void SendSimpleInfoReport(const char* szCmd, int nRetCode, 
                                         const char* szInfo) ;
	void SendHoriTableReport(CParseResult* pParseResult, 
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList, 
                        const char* szTableHeadName);
                        
        //add by maimaoshi,2003-03-12
        //��ѯ����״̬
	void QueryProcStatus(MSG_INTRA* pMsgParsed);
	//end
    
protected:
    // ��ȡ�����ļ���Ŀ¼����������ͳ���ļ�
    int GetPerfFileList ( LIST<STRING>& PerfFileList );
    
    // ɾ��ָ������ǰ�����������ļ�
    int DeleteOutdatedFile ( void );

	//��ʱ��MML�ͻ��˷��͸�������������״̬
	void SendConnStat();
    
protected:
    // �������ͳ���ļ��ĺ�׺��
    inline const char* GetPerfFileExt ( void )
    {
        if(m_bUseXml)
        {
            return ".xml";
        }

        return ".prf";
    }

    // �������ͳ���ļ��Ķ�д��װ����
    inline IPerfUtil* CreatePerfUtilObj ( void )
    {
        if(m_bUseXml)
        {
            return m_perfUtilEncap.CreatePerfUtil();
        }
        
        return new CPerfBinUtil();
    }
    
    void CreateAlarm(CParseResult *pParse);
	void MonitorSystem();
    // ����ϵͳ�������ݿ⣬��NT�¸�ָ��ָ��һ��CNTPerfDB����;
    // ��UNIX�¸�ָ��ָ��һ��CUNIXPerfDB����
    COSPerfDB *m_pOSPerfDB;     
    
    UINT4 m_uMinDiskAlarmRoom;      //�澯��ֵ(major)
    UINT4 m_uMaxDiskAlarmRoom;      //�澯��ֵ(minor)    
    UINT4 m_uMinDiskHandoverRoom;   // ��С�����ռ�
    UINT4 m_uMinBakDeviceRoom;      // ���ݽ��ʵ���С�ռ�
    STRING m_szFrontSavePath;   // ���ԭʼ�����ļ��ĸ�Ŀ¼
    STRING m_szBackSavePath;    // ������ջ����ļ��ĸ�Ŀ¼
    STRING m_szBakDevicePath;   // ���ݽ��ʵĸ�Ŀ¼·��
    BOOL m_bFrontSaveAlarm;     // ԭʼ�����ռ䲻��澯��־
    BOOL m_bBackSaveAlarm;      // ���ջ����ռ䲻��澯��־
    BOOL m_bBakDeviceAlarm;     // ���ݽ��ʿռ䲻��澯��־
    BOOL m_bFrontSaveMinorAlarm;
    BOOL m_bBackSaveMinorAlarm;
    BOOL m_bBakDeviceMinorAlarm;    
    BOOL m_bFrontSaveHandover;  // ԭʼ�����ռ䲻�㵹����־
    BOOL m_bBackSaveHandover;   // ���ջ����ռ䲻�㵹����־
    BOOL m_bBakDeviceHandover;  // ���ݽ��ʿռ䲻�㵹����־
    UINT4 m_nTotalFrontSaveRoom;
    UINT4 m_nTotalBackSaveRoom;
    UINT4 m_nTotalBakDeviceRoom;    
    
    UINT4 m_uAPNum;				// ����������
    UINT4 m_uSavedDays;			// �����ļ���������
    
	UINT2 m_uBillsvrNo;
    
	long m_PerfStatID;			// ͳ���������ݶ�ʱ��ID
	long m_ApConnTimer;		    // ���ͽ��������״̬�Ķ�ʱ��

    PERF_MAP m_PerfMap;			// ��û���յ����н�������״̬ǰ��
								// �������յ����������������ݣ�
								// ���һ��Ԫ�ر�������״̬��Ϣ
    SPerfItem m_perfHeart[2];	// ��������,0�������,1������ø���
	
	MAP<BYTE, STRING>  m_mapAPNumToAPName; //�ӵ��������ӳ��ӵ��
	CMMLReport*        m_pMMLReport;       //�������MML����Ķ���
    int                m_nCurClientNo;     //��ǰ���ڴ���Ļ���̨��̨��
	LINKCOUNTER_LIST   m_LinkCounterList;  //��·������
	UINT4              m_nLinkMaxCounter;  //�����·����ֵ
	//���¼�ʱ��ID
    long m_Timer1SecId;
	//������ 2002-12-07
	long m_nMonitorID;          //ϵͳ��ض�ʱ��ID
	BYTE m_nCPUmaxLimit;        //CPU��������
	BYTE m_nCPUminLimit;        //CPU��������
    unsigned short m_nMemMaxLimit;        //Memeory��Ҫ�澯�ķ�ֵ
    unsigned short m_nMemMidLimit;         //memory��Ҫ�澯�ķ�ֵ
    unsigned short m_nMemMinLimit;        //Memeory�澯�ָ��ķ�ֵ    
	int m_nPeriod;              //ɨ������
	int m_nExSpan;              //��������ֵE(x)������
	
	int m_nPerfPeriod;         //����ͳ�Ƽ��
	BYTE m_nSpanCount;      //��ֵ���ڵļ���

	unsigned short *m_pCPUTotalArray;   //CPU�ۼ�ֵ
    float *m_pExCPUUsedArray;           //CPU��ƽ��ֵ
	UINT4 m_uMEMTotal;	    //Memeory���ۼ�ֵ
	//������ ���ӽ��� 2002-12-07

    CCollectStatistic m_CollectStatistic;

	//�����ļ�����·����
	char m_szFileName[MAX_PATH];

    CPerfUtilEncap  m_perfUtilEncap;
	IPerfUtil*      m_pPerfUtil;           //��������ͳ���ļ������Ķ���
    BOOL            m_bUseXml;
    //����������M2000���ܽӿ�
    CTaskManager*   m_pM2000Manager;    //M2000�������������
    long m_Timer1SecID;                 //1�붨ʱ��ID
    STRING          m_szBasePath;
};

#endif /* __PERF_H__ */

