/**********************************************************************
 *产品名    ：iGateway Bill V200        
 *模块名    ：agent
 *文件名    ：ClusterAgent.h    
 *描述信息  ：本文件是类CClusterAgent的定义文件。
 *版本说明  ：V200R002i00M00B00D1130A   
 *版权信息  ：版权所有（C）2001-2002 华为技术有限公司
 *作者      ：邱意平
 *创建时间  ：2001-10-30
 *修改记录  ：
 **********************************************************************/

#ifndef _CLUSTER_AGENT_H_
#define _CLUSTER_AGENT_H_

#include "agent_hdr.h"
#include "../include/frame.h"
#include "cluster.h"
#include "cluster2.h"
#include "cluster3.h"

class CCluster;
class CCluster2;
class CCluster3;
class CClusterAlarm;

class CClusterAgent : public CWorkModule
{
public:
    CClusterAgent(BOOL bIsSingle);        //构造函数
    ~CClusterAgent(void);                 //析构函数
    
protected:
    int  InitInstance(void);            //继承于框架的初始化函数
    MODULE_TYPE GetModuleType();          //继承于框架的虚拟函数
    
    BOOL StartKernel(void);               //启动KERNEL进程
    BOOL CheckKernel(void);               //检测KERNEL进程
    BOOL StopKernel(void);                //关闭KERNEL进程
    void KillKernel(void);                //强行杀掉KERNEL进程
    
    void DoAnsActive(BOOL bResponse);     //处理双机激活应答
    int  DoManualOver(MSG_INTRA* pMsg);   //处理手工倒换
    void DoReset(MSG_INTRA* pMsgParsed);  //处理手工重启


    void DoAskFailover(EFAILOVERCAUSE_TYPE eCause);  //处理倒换申请

    BOOL Switch(BOOL bCheckSanity);       //健康检查及切换
    
    void DoReqStatus(void);               //双机状态查询
    
    void ParseMsg(MSG_INTRA* pMsg);       //消息解析
    void OnTimer(const long nTimerID);    //定时器处理
    
    //系统信号处理
    int  handle_signal(int signum, siginfo_t* st, ucontext_t* ut);

    //by ldf 对应问题单D11862 2002-3-16
    void proc_signal(int signum);
    
private:
    CCluster*       m_ptheCluster;        //双机管理对象
    CClusterAlarm*  m_ptheClusterAlarm;   //双机告警对象 
    BOOL            m_bIsSingle;          //单机、双机指示
    pid_t           m_nPidKnl;             //KERNEL进程的PID

    //以下成员变量为超时计数器，单位均为秒
    UINT4           m_uAlarmInterval;     //告警检测间隔(单位：秒)
    UINT4           m_uKnlActTimeout;     //等待KERNEL进程激活应答超时
    UINT4           m_uKnlOffTimeout;     //等待KERNEL进程关闭超时
    UINT4           m_uKnlOffInterval;    //等待KERNEL进程关闭时间间隔
    UINT4           m_uKnlOffCount;       //等待KERNEL进程关闭定时累计
    UINT4           m_uCheckKnlInterval;  //检测KERNEL进程是否正常的时间间隔 
   
    //以下成员变量为定时器ID
    long            m_nAlarmIntervalID;   //告警检测定时器ID
    long            m_nKnlActTimeoutID;   //等待KERNEL激活应答定时器ID
    long            m_nKnlOffIntervalID;  //检测KERNEL关闭定时器ID
    long            m_nCheckKnlID;        //检测KERNEL进程的定时器ID
    long            m_nSendHeartBeatID;   //检测向PT_OM发心跳信息的定时器ID

    //by ldf 对应问题单D11862 2002-3-16
    long            m_nTimerIDBase;       //检测信号的定时器ID
    int             m_nRecvSignal;        //收到的信号

    STRING          m_sSCVer;             //SC软件版本号
};

#endif  //_CLUSTER_AGENT_H_
