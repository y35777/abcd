
#ifndef __PERF_ITEM_H__
#define __PERF_ITEM_H__

#include "base_type.h"

// SPerfItem�ṹ����һ��ʱ���ڵ���������
struct SPerfItem
{
    BYTE yAPID;         //���̺�
    BYTE yModuleID;     //ģ���
    UINT2 uID;          //����ͳ����ı��
    time_t timeBegin;   //ͳ�ƿ�ʼʱ��
    time_t timeEnd;     //ͳ�ƽ���ʱ��
    UINT4 uValue;       //����ͳ�����ȡֵ
};

//����ͳ�Ʋ��ԣ�ͳ�ƾ�ֵ�����ۼ�ֵ
enum PERF_STATS_STRATEGY
{
    PERF_STATS_AVERAGE_VALUE    = 0,    //ͳ�ƾ�ֵ
    PERF_STATS_TOTAL_VALUE      = 1     //ͳ���ۼ�ֵ
};

class CPerfItem {
protected:
    // ���캯����ͨ����̬����CPerfItem::GetItem����CPerfItem����
    CPerfItem(const BYTE yAPID, const BYTE yModuleID, const UINT2 uItemID, 
              const PERF_STATS_STRATEGY nStatsStrategy = PERF_STATS_AVERAGE_VALUE);

public:
    // ������������
    void Set(const UINT4 uNewValue);

    // ׷����������
    void Add(const UINT4 uValueAdded);

    PERF_STATS_STRATEGY GetStatsStrategy(void);

    // ���CPerfItemʵ��
    static CPerfItem* GetItem(const BYTE yAPID, const BYTE yModuleID, const UINT2 uItemID,
                              const PERF_STATS_STRATEGY nStatsStrategy = PERF_STATS_AVERAGE_VALUE);

    // �ͷ�����ʵ��
    static void ReleaseAllItems(void);

    // ��������ͳ�����ڵĿ�ʼʱ��
    static void BeginStatistic(void);

    // ��������ͳ�����ڵĽ���ʱ��
    static void EndStatistic(SPerfItem*& pAllItems, UINT2& uItemCount);

	static void SetPerfOn(BOOL bOption);

protected:
    SPerfItem m_Data;       // ��������
    BOOL m_bNewPeriod;      // ��ͳ�����ڿ�ʼ�ı�־
    static LIST<CPerfItem*> g_Items;    // CPerfItem���󼯺�

	static BOOL g_bPerfOn;
	static ACE_Mutex g_Mutex;
    PERF_STATS_STRATEGY m_nStatsStrategy;
};

//added by maimaoshi,2002-11-22
struct SLinkCounter
{
    BYTE  nApID;              //�ӵ�
    char  szModuleLabel[50];  //ģ��ţ��Ի�������Э����ģ��ţ���GTP'Э����ȱʡ1
    char  szLinkLabel[50];    //��·��,����BAM��,IP��ַ
    UINT4 nCounter;           //����״̬��������
};

typedef LIST<SLinkCounter>       LINKCOUNTER_LIST;	

typedef LIST<SLinkStat>          LINKSTAT_LIST;	

typedef MAP<BYTE,LINKSTAT_LIST*>  LINKSTAT_MAP;

class CLinkStatItem
{
protected:
    // ���캯��
    CLinkStatItem();
	//��������
	~CLinkStatItem();
public:

	//�ύ��·״̬
	static void CommitLinkStat(const BYTE yAPID,SLinkStat LinkStat);

	//�����·״̬
    static void GetLinkStatData(LINKSTAT_LIST*& pLinkList,const BYTE yAPID);

	// �ͷ�����ʵ��
    static void ReleaseAllItems();   

public:
	
	static LINKSTAT_MAP  g_LinkStat;
    static ACE_Mutex     g_LinkStat_Mutex;
};
//end

#endif /* __PERF_H__ */    
