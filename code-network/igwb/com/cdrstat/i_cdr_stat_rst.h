#ifndef __I_STAT_CFG_RST_H__
#define __I_STAT_CFG_RST_H__

#include "stat_cfg_info.h"

class ICDRStatRst
{
public:
    ICDRStatRst(){};
    virtual  ~ICDRStatRst(){};
public:
    //设置参数，进行初始化操作
    virtual int Init(UINT4 uAccessPoint) = 0;

    //获得统计结果文件头信息
    virtual void GetStatRstHead(SStatRstHead& statRstHead) = 0;

    //获得所有统计条件表达式
    virtual SStatExpIndexList* GetStatExpList(void) = 0;

    //获得指定时间范围内统计条件表达式的结果
    virtual int GetStatExpRst(const char* const szStatExpress, time_t tBeginTime,
                            time_t tEndTime, UINT4& nCDRCount, UINT4& nValueTotal) = 0;

    //添加统计条件表达式
    virtual int AddStatExpress(LIST<char* >& StatExpList) = 0;

    //添加一天的所有统计表达式的统计结果
    virtual int AddStatRst(const SAllStatExpRst& statExpRst, time_t statTime) = 0;

    //返回接入点号
    virtual UINT4 GetAccessPoint() = 0;

    //]返回接入点名称
    virtual const char* GetAccessPointName() = 0;

    //根据接入点号返回所有表达式
    virtual void  GetAllExpress(LIST<STRING>& ExpressList) = 0;

    //获得上次统计时间
    virtual time_t GetLastStatTime() = 0;
    
    //刷新已执行统计操作的时间
    virtual int SetLastStatTime(time_t curtime) = 0;
};


#endif //__I_STAT_CFG_RST_H__
