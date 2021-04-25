#ifndef __BILLPROCESSOR_H__
#define __BILLPROCESSOR_H__

#include "bill_proc.h"
#include "../include/frame.h"
#include "../include/perf_item.h"

class CBPCreator  
{
public:
    CBPCreator();
    virtual ~CBPCreator();

    //根据接入点类型创建相应的CBillProcess的子类的对象
	//2003-06-02 edit by ggf
    static CBillProcess* CreateBillProcess(AP_TYPE typeID, const char* szReleaseVer = "");
	//end of edit ggf
};

class CBillProcessor : public CWorkModule  
{
public:
    CBillProcessor(const UINT4 uAPID, CPacketCache* pPacketCache);
    virtual ~CBillProcessor();

    MODULE_TYPE GetModuleType(); //获取模块类型
    int InitInstance();        //初始化函数
    void ExitInstance();        //模块退出时的清理函数
    void ParseMsg(MSG_INTRA* pMsgParsed);   //消息分发函数
	
protected:
    void ProcBillProcMsg(MSG_INTRA* pMsg);  //处理"请求计费预处理"消息
    void ProcResumeMsg(MSG_INTRA* pMsg);    //处理"请求启动恢复"消息
    void ProcSynInfoMsg(MSG_INTRA* pMsg);   //处理"通知状态"的消息
    void OnTimer(const long nTimerID);		//定时器处理函数

protected:
    CPacketCache* m_pPacketCache;       //话单包缓冲区
    CBillProcess* m_pBillProcess;       //话单处理对象
    UINT4         m_uAPID;              //当前的接入点ID号
    INT4          m_nChannelCount;      //话单通道数（包括通道0）
	long          m_nTimerID1Sec;		//1秒定时器ID

	//如果在指定的时间内异常话单达到指定的个数，则产生告警
	int           m_nCDRCheckSpan;      //异常话单出现的时间限制
	int           m_nCDRCheckCount;     //异常话单出现的次数限制
};

#endif
