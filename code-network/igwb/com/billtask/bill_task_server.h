#ifndef __BILL_TASK_SERVER_H__
#define __BILL_TASK_SERVER_H__

#include "../include/frame.h"
#include "bill_task.h"

class CBillTaskServer : public CWorkModule
{
public:

    CBillTaskServer();
    ~CBillTaskServer();

protected:

    //取模块号
    virtual MODULE_TYPE GetModuleType();

    //初始化
    virtual int InitInstance();

    //扫尾工作
    virtual void ExitInstance();

    //命令解析和分发
    virtual void ParseMsg(MSG_INTRA* pMsgParsed);

    //定时器事件的处理入口函数
    virtual void OnTimer(const long nTimerID);

    //5秒定时器事件的处理入口函数
    void Timer5Sec();

    //SCP话单恢复处理函数
    void ProcScpRecovery(MSG_INTRA* pMsgParsed);

    //SCP关闭告警请求处理函数
    void ProcScpCloseAlarm(MSG_INTRA* pMsgParsed);

    //话单恢复任务队列
    LIST<CBillTask*>	    m_TaskList;

    //网际标识及信令点号到接入点号的MAP
    MAP<SScpApplyKey, int>	m_MapDpctoAP;

    //5秒定时器ID
    long	                m_lTimer5Sec;

    //当前任务ID号
    int                 m_nTaskID;
};

#endif //__BILL_TASK_SERVER_H__
