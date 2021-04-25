/**************************************************************************
 *产品名    : iGateway Bill V200        
 *模块名    : 性能模块(Perf)
 *文件名    : perf.h
 *描述信息  : 本文件是类CPerf的定义
 *版本说明  : V200R001i00M00B00D1130A   
 *版权信息  : 版权所有(c) 2001-2002 华为技术有限公司
 *作者      : 方卫华
 *创建时间  : 2001-11-05
 *修改记录  : 
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
//张玉新 2002-04-13增加
#ifdef _PLATFORM_WIN32
#include "perf_nt/nt_perf_db.h"
#else
#include "perf_unix/unix_perf_db.h"
#endif
//增加结束
#include "../include/perf_item.h"

#include "perf_encap.h"
#include "perf_bin_util.h"

typedef MAP<BYTE,SPerfItem *,LESS<BYTE> > PERF_MAP;

#define AP_CONN_STATISTIC_TIMER		10 * 1000

// 定义下面数据结构暂时保存接收到的性能数据
typedef struct _PerfBuf
{
    int count;
    SPerfItem* pData;
} SPerfBuf, *PPerfBuf;
 
class CPerf : public CWorkModule
{
public:
    // 构造函数
    CPerf(void);
    
    // 析构函数
    ~CPerf(void);
    
protected:
    // 获得本模块的类型
    MODULE_TYPE GetModuleType(void);
    
    // 初始化
    int InitInstance(void);
    
    // 退出时的清理工作
    void ExitInstance(void);
    
    // 根据定时器ID分别处理定时器事件
    virtual void OnTimer(const long nTimerID);
    
    // 解析性能模块收到的消息
    virtual void ParseMsg(MSG_INTRA* pMsgParsed);
    
    // 让性能模块处于状态查询的状态
    void QueryStatus(MSG_INTRA* pMsg);
	void SetStatusInv(MSG_INTRA* pMsg);
	void LstStatusInv(MSG_INTRA* pMsg);
	//查询IP连接
    void QueryIPAddr(MSG_INTRA *pMsg);
    // 处理接入点与主机的连接情况
    void ProcConnectStatus(const BYTE nAccessPointID, 
                           SPerfItem* pData, 
                           const BYTE nCount);
                           
    // 保存来自各进程的性能数据
    void SavePerfItem(SPerfItem* pItems, const BYTE nCount);
    
	void SetAllAPName(MSG_INTRA *pMsg);
	//查询链路状态信息
	void QueryLinkState(MSG_INTRA *pMsg);

	//查询所有的AP名称
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
        //查询进程状态
	void QueryProcStatus(MSG_INTRA* pMsgParsed);
	//end
    
protected:
    // 获取性能文件根目录下所有性能统计文件
    int GetPerfFileList ( LIST<STRING>& PerfFileList );
    
    // 删除指定天数前的性能数据文件
    int DeleteOutdatedFile ( void );

	//定时向MML客户端发送各个接入点的连接状态
	void SendConnStat();
    
protected:
    // 获得性能统计文件的后缀名
    inline const char* GetPerfFileExt ( void )
    {
        if(m_bUseXml)
        {
            return ".xml";
        }

        return ".prf";
    }

    // 获得性能统计文件的读写封装对象
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
    // 操作系统性能数据库，在NT下该指针指向一个CNTPerfDB对象;
    // 在UNIX下该指针指向一个CUNIXPerfDB对象
    COSPerfDB *m_pOSPerfDB;     
    
    UINT4 m_uMinDiskAlarmRoom;      //告警阀值(major)
    UINT4 m_uMaxDiskAlarmRoom;      //告警阀值(minor)    
    UINT4 m_uMinDiskHandoverRoom;   // 最小倒换空间
    UINT4 m_uMinBakDeviceRoom;      // 备份介质的最小空间
    STRING m_szFrontSavePath;   // 存放原始话单文件的根目录
    STRING m_szBackSavePath;    // 存放最终话单文件的根目录
    STRING m_szBakDevicePath;   // 备份介质的根目录路径
    BOOL m_bFrontSaveAlarm;     // 原始话单空间不足告警标志
    BOOL m_bBackSaveAlarm;      // 最终话单空间不足告警标志
    BOOL m_bBakDeviceAlarm;     // 备份介质空间不足告警标志
    BOOL m_bFrontSaveMinorAlarm;
    BOOL m_bBackSaveMinorAlarm;
    BOOL m_bBakDeviceMinorAlarm;    
    BOOL m_bFrontSaveHandover;  // 原始话单空间不足倒换标志
    BOOL m_bBackSaveHandover;   // 最终话单空间不足倒换标志
    BOOL m_bBakDeviceHandover;  // 备份介质空间不足倒换标志
    UINT4 m_nTotalFrontSaveRoom;
    UINT4 m_nTotalBackSaveRoom;
    UINT4 m_nTotalBakDeviceRoom;    
    
    UINT4 m_uAPNum;				// 接入点进程数
    UINT4 m_uSavedDays;			// 性能文件保存天数
    
	UINT2 m_uBillsvrNo;
    
	long m_PerfStatID;			// 统计性能数据定时器ID
	long m_ApConnTimer;		    // 发送接入点连接状态的定时器

    PERF_MAP m_PerfMap;			// 在没有收到所有接入点进程状态前，
								// 保存已收到接入点进程性能数据，
								// 最后一个元素保存心跳状态信息
    SPerfItem m_perfHeart[2];	// 代表心跳,0代表个数,1代表可用个数
	
	MAP<BYTE, STRING>  m_mapAPNumToAPName; //接点进程名称映射接点号
	CMMLReport*        m_pMMLReport;       //负责产生MML报告的对象
    int                m_nCurClientNo;     //当前正在处理的话单台的台号
	LINKCOUNTER_LIST   m_LinkCounterList;  //链路计数器
	UINT4              m_nLinkMaxCounter;  //最大链路计数值
	//秒事件时钟ID
    long m_Timer1SecId;
	//张玉新 2002-12-07
	long m_nMonitorID;          //系统监控定时器ID
	BYTE m_nCPUmaxLimit;        //CPU过载上限
	BYTE m_nCPUminLimit;        //CPU过载下限
    unsigned short m_nMemMaxLimit;        //Memeory重要告警的阀值
    unsigned short m_nMemMidLimit;         //memory次要告警的阀值
    unsigned short m_nMemMinLimit;        //Memeory告警恢复的阀值    
	int m_nPeriod;              //扫描周期
	int m_nExSpan;              //计算期望值E(x)的周期
	
	int m_nPerfPeriod;         //性能统计间隔
	BYTE m_nSpanCount;      //均值周期的计数

	unsigned short *m_pCPUTotalArray;   //CPU累计值
    float *m_pExCPUUsedArray;           //CPU的平均值
	UINT4 m_uMEMTotal;	    //Memeory的累计值
	//张玉新 增加结束 2002-12-07

    CCollectStatistic m_CollectStatistic;

	//性能文件绝对路径名
	char m_szFileName[MAX_PATH];

    CPerfUtilEncap  m_perfUtilEncap;
	IPerfUtil*      m_pPerfUtil;           //用于性能统计文件操作的对象
    BOOL            m_bUseXml;
    //张玉新增加M2000性能接口
    CTaskManager*   m_pM2000Manager;    //M2000性能任务管理器
    long m_Timer1SecID;                 //1秒定时器ID
    STRING          m_szBasePath;
};

#endif /* __PERF_H__ */

