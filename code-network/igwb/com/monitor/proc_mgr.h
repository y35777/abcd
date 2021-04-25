/***************************************************************************
 *产品名   ：iGateway Bill V200
 *模块名   ：monitor
 *文件名   ：proc_mgr.h
 *描述信息 ：类CProcessManager的定义。
 *版本说明 ：V200R001i00M00B00D1130A
 *版权信息 ：版权所有（C）2001-2002 华为技术有限公司
 *作者     ：邱意平
 *创建时间 ：2001-10-30
 *修改记录 ：
*****************************************************************************/

#ifndef _PROCESS_MANAGER_H_
#define _PROCESS_MANAGER_H_

#include "monitor_hdr.h"
#include "proc_child.h"

#define TIME_OF_PROC_INIT       60
#define TIME_OF_PROC_ACTIVE     60
#define TIME_OF_PROC_DEACTIVE   120

class CProcessChild;

class CProcessManager
{
public:
    CProcessManager();
    ~CProcessManager();

    int InitInstance();

    void ProcInitAns(PROCESS_TYPE nPType);      //处理初始化应答
    void ProcActiveAns(PROCESS_TYPE nPType);    //处理激活应答
    int Start();                                //启动所有进程
    int Active();                               //激活所有进程
    void DeActive();                            //去激活所有进程
    void Timer5Sec();
    EPROCMGRSTATUS GetStatus();                 //返回本对象的状态
    void ProcAuxUpgradeCmd();                   //处理辅助升级命令
    void ProcAuxUpgradeAns(PROCESS_TYPE nPType);//处理辅助升级应答
    EAUXUPGSTATUS GetAuxUpgStatus();            //返回当前辅助升级的状态
    //by ldf 2002.4.8 对应问题单 D12433
    void AuxUpgFailSetStatus();                 //辅助升级失败回置AUXUPG_STOPPED状态
    //修改完毕
    void ProduceCDR();   

protected:
    int CreateAllProcessChild();                //创建所有子进程对象
    int Add(CProcessChild *pChild);             //添加指定进程对象
    void ProcFailedRecovery(PROCESS_TYPE nPType);//进行本地恢复失败的处理
    CProcessChild *LookUp(PROCESS_TYPE nPType); //通过输入的进程类型查找进程对象
    CProcessChild *LookUp(pid_t nPid);          //通过输入的PID查找进程对象
    int Start(PROCESS_TYPE nPType);             //启动指定进程
    void Stop(PROCESS_TYPE nPType);             //关闭指定进程
    void Stop();                                //关闭所有进程
    int StartWatchDog();                        //启动守护线程

    static void ProcWatchDog(CProcessManager* pMgr);    
    void WatchDog();
    int ProcRecovery(PROCESS_TYPE nPType);      //进行本地恢复
    EAPPSTATUS GetStatus(PROCESS_TYPE nPType);  //返回指定进程编号的状态
    int SetStatus(PROCESS_TYPE nPType,          //设置指针进程的状态
                  EAPPSTATUS nStatus);
    pid_t WaitExitPid(UINT4 uSec);              //等待所有子进程直到超时，返回
                                                //退出的进程PID
    int Wait(UINT4 uSec);                       //等待所有子进程直到超时，返回
                                                //当前未退出的进程数
    int WaitPid(pid_t nPID, int nSec);          //等待指定进程退出，nSec超时
public:

	typedef LIST<CProcessChild *> LISTPROCCHILD;
    LISTPROCCHILD       m_ProcChild;        //业务进程对象指针list                                                

private:
    ACE_Process_Manager m_ProcMgr;          //业务进程的ACE管理者
    ACE_Thread_Manager  m_ThdMgr;           //守护线程的线程管理对象

    BOOL                m_bExitFlag;        //只当关闭守护线程时才置TRUE的标志
    UINT2               m_uInitTimeOut;     //业务进程初始化超时时间计时器,单位为秒
    UINT2               m_uActiveTimeOut;   //业务进程激活超时时间计时器,单位为秒
    EAUXUPGSTATUS       m_nAuxUpdStatus;    //辅助升级状态
    EPROCMGRSTATUS      m_nStatus;          //业务进程管理者对象当前状态
};


#endif //_PROCESS_MANAGER_H_
