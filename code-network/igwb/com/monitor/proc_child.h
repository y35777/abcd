/***************************************************************************
 *产品名        ：iGateway Bill V200
 *模块名        ：monitor
 *文件名        ：proc_child.h
 *描述信息      ：类CProcessChild的定义。该类抽象了一个进程对象，例如接入点进程
                  操作维护进程等。
 *版本说明      ：V200R001i00M00B00D1130A
 *版权信息      ：版权所有（C）2001-2002 华为技术有限公司
 *作者          ：邱意平
 *创建时间      ：2001-10-30
 *修改记录      ：
*****************************************************************************/

#ifndef _PROCESS_CHILD_H_
#define _PROCESS_CHILD_H_

#include "monitor_hdr.h"

class CProcessChild
{
public:
    CProcessChild(SMappedAppInfo *pAppInfo);
    ~CProcessChild();

    EAPPSTATUS GetStatus();                 //返回本进程的状态
    void SetStatus(EAPPSTATUS nStatus);     //设置本进程的状态
    time_t GetLastStartTime();              //返回上次启动的时间
    PROCESS_TYPE GetProcessType();          //返回进程编号
    pid_t GetPid();                         //返回进程PID
    EAUXUPGSTATUS GetAuxUpgStatus();        //返回辅助升级的状态
    void SetAuxUpgStatus(EAUXUPGSTATUS nStatus);    //设置辅助升级的状态
    int Active();                           //激活本进程
    BOOL CanRestart();                      //返回是否能进行本地恢复
    void Stop();                            //关闭本进程
    int Start(ACE_Process_Manager *pMgr);   //启动本进程
    const char *GetAppName();               //返回本进程名
	int GetLeftStartDelay(int nDiv);        //取当前剩余的重启延迟时间,nDiv为每次检查的间隔时间(s)

    inline BOOL GetbBaseProc(void)
    {
        return m_bBaseProc;
    }

    inline void EnableBaseProc(void)
    {
        m_bBaseProc = TRUE;
    }

    inline void DisableBaseProc(void)
    {
        m_bBaseProc = FALSE;
    }

private:
    EAPPSTATUS          m_nStatus;          //本业务进程的状态
    EAUXUPGSTATUS       m_nAuxUpgStatus;    //本业务进程的辅助升级操作是否完成
    SMappedAppInfo      *m_pInfo;           //业务进程信息  
    pid_t               m_PID;              //本业务进程的进程ID
    UINT2               m_uRestartCount;    //重启计数器，如果重启次数超过三次则倒换
    time_t              m_uLastStartTime;   //上次启动的绝对时间
    ACE_Process_Options m_POptions;         //业务进程的ACE环境
	int                 m_nStartDelay;      //当进程状态为APP_SCHEDULED_START时，延迟启动的时间(s)
    BOOL                m_bBaseProc;        //是否是基础进程（需要先进后退）
};


#endif //_PROCESS_CHILD_H_

