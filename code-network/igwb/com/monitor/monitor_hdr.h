/***************************************************************************
 *产品名        ：iGateway Bill V200
 *模块名        ：monitor
 *文件名        ：monitor_hdr.h
 *描述信息      ：本文件包含监控模块需要使用的头文件和数据结构。
 *版本说明      ：V200R001i00M00B00D1130A
 *版权信息      ：版权所有（C）2001-2002 华为技术有限公司
 *作者          ：邱意平
 *创建时间      ：2001-10-30
 *修改记录      ：
****************************************************************************/

#ifndef _MONITOR_HEADER_H_
#define _MONITOR_HEADER_H_

#include "../include/precom.h"
#include "../include/resource.h"
#include "../include/toolbox.h"
#include "../include/frame.h"

extern long g_lDebugVersion;

enum EAPPSTATUS
{
    APP_STOPPED,                //停止状态
    APP_READY,                  //已收到业务进程管理模块的初始化应答
    APP_SCHEDULED_START,        //处于启动前的等待状态
    APP_START_PENDING,          //正在启动状态
    APP_STOP_PENDING,           //正在停止状态
    APP_ACTIVING,               //正在激活状态
    APP_ACTIVE                  //运行并激活状态
};

enum EAUXUPGSTATUS
{
    AUXUPG_STOPPED,             //未启动辅助升级状态
    AUXUPG_START,               //正在执行辅助升级状态
    AUXUPG_COMPLETED            //辅助升级已完成状态
};

struct SMappedAppInfo
{
    PROCESS_TYPE nPType;        //业务进程编号
    BOOL bStart;                //是否启动该模块
    BOOL bCanRestart;           //该进程在异常退出时是否可以重启该进程
    char szAppName[100];        //该业务进程的Name
    char szPathName[MAX_PATH];  //该业务进程的程序路径（全路径）
};

enum EPROCMGRSTATUS
{
    PROCMGR_STOPPED,            //停止状态
    PROCMGR_READY,              //已收到业务进程管理模块的初始化应答
    PROCMGR_START_PENDING,      //正在启动状态
    PROCMGR_STOP_PENDING,       //正在停止状态
    PROCMGR_ACTIVING,           //正在激活状态
    PROCMGR_ACTIVE,             //运行并激活状态
    PROCMGR_FAILURE             //故障状态
};

enum EKERNELSTATUS
{
    KERNEL_INIT_PENDING,        //Kernel处于初始化状态，启动并等待业务进程返回
                                //初始化成功的消息。
    KERNEL_READY,               //Kernel初始化成功，正在等待激活命令。
    KERNEL_ACTIVING,            //Kernel正在执行激活的状态。
    KERNEL_ACTIVE,              //Kernel处于激活状态。
    KERNEL_DEACTIVING,          //Kernel正在执行去激活的状态。
    KERNEL_DEACTIVE,            //Kernel处于停止状态
    KERNEL_FAILURE,             //Kernel处于故障的状态。
    KERNEL_FAILOVER,            //Kernel处于正在申请故障倒换的状态
    KERNEL_SHUTDOWNING          //Kernel处于正在退出的状态
};

#endif //_MONITOR_HEADER_H_
