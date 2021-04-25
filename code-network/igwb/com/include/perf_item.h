
#ifndef __PERF_ITEM_H__
#define __PERF_ITEM_H__

#include "base_type.h"

// SPerfItem结构保存一段时间内的性能数据
struct SPerfItem
{
    BYTE yAPID;         //进程号
    BYTE yModuleID;     //模块号
    UINT2 uID;          //性能统计项的编号
    time_t timeBegin;   //统计开始时刻
    time_t timeEnd;     //统计结束时刻
    UINT4 uValue;       //性能统计项的取值
};

//性能统计策略：统计均值或者累计值
enum PERF_STATS_STRATEGY
{
    PERF_STATS_AVERAGE_VALUE    = 0,    //统计均值
    PERF_STATS_TOTAL_VALUE      = 1     //统计累计值
};

class CPerfItem {
protected:
    // 构造函数，通过静态函数CPerfItem::GetItem创建CPerfItem对象
    CPerfItem(const BYTE yAPID, const BYTE yModuleID, const UINT2 uItemID, 
              const PERF_STATS_STRATEGY nStatsStrategy = PERF_STATS_AVERAGE_VALUE);

public:
    // 设置性能数据
    void Set(const UINT4 uNewValue);

    // 追加性能数据
    void Add(const UINT4 uValueAdded);

    PERF_STATS_STRATEGY GetStatsStrategy(void);

    // 获得CPerfItem实例
    static CPerfItem* GetItem(const BYTE yAPID, const BYTE yModuleID, const UINT2 uItemID,
                              const PERF_STATS_STRATEGY nStatsStrategy = PERF_STATS_AVERAGE_VALUE);

    // 释放所有实例
    static void ReleaseAllItems(void);

    // 设置性能统计周期的开始时刻
    static void BeginStatistic(void);

    // 设置性能统计周期的结束时刻
    static void EndStatistic(SPerfItem*& pAllItems, UINT2& uItemCount);

	static void SetPerfOn(BOOL bOption);

protected:
    SPerfItem m_Data;       // 性能数据
    BOOL m_bNewPeriod;      // 新统计周期开始的标志
    static LIST<CPerfItem*> g_Items;    // CPerfItem对象集合

	static BOOL g_bPerfOn;
	static ACE_Mutex g_Mutex;
    PERF_STATS_STRATEGY m_nStatsStrategy;
};

//added by maimaoshi,2002-11-22
struct SLinkCounter
{
    BYTE  nApID;              //接点
    char  szModuleLabel[50];  //模块号，对滑动窗口协议是模块号，对GTP'协议是缺省1
    char  szLinkLabel[50];    //链路号,比如BAM号,IP地址
    UINT4 nCounter;           //链接状态及计数器
};

typedef LIST<SLinkCounter>       LINKCOUNTER_LIST;	

typedef LIST<SLinkStat>          LINKSTAT_LIST;	

typedef MAP<BYTE,LINKSTAT_LIST*>  LINKSTAT_MAP;

class CLinkStatItem
{
protected:
    // 构造函数
    CLinkStatItem();
	//析构函数
	~CLinkStatItem();
public:

	//提交链路状态
	static void CommitLinkStat(const BYTE yAPID,SLinkStat LinkStat);

	//获得链路状态
    static void GetLinkStatData(LINKSTAT_LIST*& pLinkList,const BYTE yAPID);

	// 释放所有实例
    static void ReleaseAllItems();   

public:
	
	static LINKSTAT_MAP  g_LinkStat;
    static ACE_Mutex     g_LinkStat_Mutex;
};
//end

#endif /* __PERF_H__ */    
