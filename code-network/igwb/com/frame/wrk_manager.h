/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:wrk_manager.h
    描述信息:
    
    包含CModuleManager类的定义.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-16,创建.
*/

#ifndef _WORK_MODULE_MANAGER_H
#define _WORK_MODULE_MANAGER_H

#include "msg_proc.h"

class CWorkModule;

class CModuleManager:public CMsgProcessor
{
protected:
    typedef std::list<CWorkModule*> LIST_WORK_MODULE;

public:
    CModuleManager();
    virtual ~CModuleManager();

protected:
    LIST_WORK_MODULE m_WorkModules;     //业务模块指针链表
    long m_lTimerIDBase;                //基准定时器ID
    long m_lTimerCounterPerf;           //用于性能统计的定时器计数器
    EWKMGR_STATUS m_nStatus;
    UINT4 m_nModuleReady;               //当前已初始化成功的模块数

    /* 创建各个业务模块对象 */
    virtual int CreateWorkModules(void) = 0;

    virtual int InitInstance(void);
    virtual void ExitInstance(void);

    virtual void ParseMsg(MSG_INTRA* pMsgParsed);

    /* 激活各个业务模块 */
    virtual BOOL ActiveWorkModules(void);

    /* 终止各个业务模块 */
    void DeActiveWorkModules(void);

    virtual void OnTimer(const long nTimerID);
    virtual MODULE_TYPE GetModuleType(){return MT_MANAGER;};
};

#endif
