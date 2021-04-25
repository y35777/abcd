/***************************************************************************
 *产品名    ：iGateway Bill V200
 *模块名    ：monitor
 *文件名    ：monitor.h
 *描述信息  ：类CMonitor的定义。
 *版本说明  ：V200R001i00M00B00D1130A
 *版权信息  ：版权所有（C）2001-2002 华为技术有限公司
 *作者      ：邱意平
 *创建时间  ：2001-10-30
 *修改记录  ：
*****************************************************************************/

#ifndef _PROCESS_MONITOR_H_
#define _PROCESS_MONITOR_H_

#include "monitor_hdr.h"
#include "proc_mgr.h"
#include "../toolbase/ini_file_ex.h"

class CMonitor:public CWorkModule
{
public:
    CMonitor();
    ~CMonitor();
    
    static int SendMsgToManager(PROCESS_TYPE nPType,    //向指定进程的管理模块发送命令 
                                UINT4 uCmdCode);

protected: 
    virtual int InitInstance();     
    virtual void ExitInstance();
    virtual void ParseMsg(MSG_INTRA* pMsgParsed);
    virtual MODULE_TYPE GetModuleType();
    int Active();                                       //激活
    void DeActive();                                    //去激活
    virtual void OnTimer(const long nTimerID);
    void Timer5Sec();
    int SendAnsToClstrInterface(UINT4 uCmdCode,         //向双机接口模块发应答
                                UINT4 uResult);
	void SendInfoToAlarm();                             //向告警模块发告警
    void SendAnsProcStatus();

private:
    CProcessManager m_ProcMgr;                          //进程管理者对象
    EKERNELSTATUS m_nStatus;                            //Kernel模块当前状态
    long m_lTimerID5Sec;                                //5秒定时器ID
    
    //add by maimaoshi,2003-03-15
    long m_lTimerID1Sec;                                //1秒定时器
	BOOL m_bClusterprocIsExist;                         //双机进程存在标志
	long m_nCounter;
	//end
};

#endif //_MONITOR_H_

