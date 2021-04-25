// perf_task.h: interface for the CPerfTask class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _PERF_TASK_H__
#define _PERF_TASK_H__

#include "../include/base_type.h"
#include "../include/perf_item.h"
#include "dda_proxy.h"
#include "resource.h"

class CSaveProxy;
class CDdaSet;

struct sTimeSpan 
{
    tm  tStartTime;
    tm  tEndTime; 
};
#define INTMAP MAP<int,int,LESS<int> >
typedef VECTOR<sTimeSpan> TimeVector;

struct SInformation{
    int     m_nStatus;     //任务状态
    STRING  m_szTaskName;  //任务名称
    STRING  m_szTaskDes;   //任务描述
    STRING  m_szUser;       //创建该任务的用户
    IntVector   m_vMU;     //测量单元ID数组
    int     m_nPeriod;     //统计周期
    time_t  m_tStartTime;  //起始日期
    TimeVector  m_vTimeSpan; //时间段的队列
    int     m_nTaskType;     //任务类型
    int     m_nDayLimit;     //有限周期天数
    int     m_nTimeType;     //时间类型
    IntVector   m_vTimeSet;  //统计时间集合
    int	    m_nResultSaveDays;     //结果保留天数
    OIVMap   m_mObjectInstance; //对象实例哈希表
    CounterMap	m_mCouterMap;      //测量指标的哈希表
    INTMAP m_mCounterStatisticCount; //每个测量指标的统计次数
};

class CPerfTask{
public:
	int IsInPeriod();
    void Timer1Sec();
	int Expire();
	int DelPerf();
	int ProcStatistic(SPerfItem *pPerfs,int nCount);
	int Init(SInformation *pInfo, STRING szFilePath);
    const char *GetLastError(){
        return m_szError.c_str();
    }
    CPerfTask();
	virtual ~CPerfTask();
    SInformation    *m_pTaskInfo; //任务信息
#ifdef UNIT_TEST
public:
#else
protected:
#endif
	int O2IVIsEqual(int ApID);
	int IsInTimeSet(int DateVal);
	int IsInTimeSpan(tm *pCurrent);
    CSaveProxy *m_pSave;     //负责性能数据存盘
    int	    m_nStatisticCount;  //统计次数一次一秒，例如30表示统计了30次
    time_t  m_tStart;           //本次统计的起始时间
    time_t  m_tEnd;             //本次统计的结束时间
    //CDdaSet *m_pDdaSet;         //DBF文件操作类
    STRING m_szError;           //错误消息
    STRING m_szPerfFilePath;     //本任务的路径
};

#endif // _PERF_TASK_H_

