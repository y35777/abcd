/***************************************************************************
 *产品名        ：iGateway Bill V200
 *模块名        ：monitor
 *文件名        ：proc_child.cpp
 *描述信息      ：类CProcessChild的实现。该类抽象了一个进程对象，例如接入点进程
                  操作维护进程等。
 *版本说明      ：V200R001i00M00B00D1130A
 *版权信息      ：版权所有（C）2001-2002 华为技术有限公司
 *作者          ：邱意平
 *创建时间      ：2001-10-30
 *修改记录      ：
***************************************************************************/

#include "proc_child.h"
#include "monitor.h"

/**************************************************************************
函数原型	public: CProcessChild(SMappedAppInfo* pAppInfo)
功能描述	构造函数
输入参数	业务进程信息对象指针。
输出参数	无。
返回值  	无。
抛出异常	无。
***************************************************************************/
CProcessChild:: CProcessChild(SMappedAppInfo *pAppInfo)
{
    //业务进程信息结构的指针赋到成品变量m_pInfo中。
    m_pInfo = pAppInfo;

    //初始化成员变量
    m_nStatus = APP_STOPPED;
    m_PID     = 0;
    m_uRestartCount  = 0;
    m_uLastStartTime = 0;
    m_bBaseProc      = FALSE;

    //初始化命令行参数，将该业务进程编号作为命令行参数
    if ((pAppInfo->nPType >= PT_CO_BASE) && (pAppInfo->nPType <= MAX_PT_CO_TYPE))
    {
        m_POptions.command_line("%s %d", pAppInfo->szPathName, 
                                pAppInfo->nPType - PT_CO_BASE + 1);
    }
    else if ((pAppInfo->nPType >= PT_AP_BASE) && (pAppInfo->nPType <= MAX_PT_AP_TYPE))
    {
        m_POptions.command_line("%s %d", pAppInfo->szPathName, 
                                pAppInfo->nPType - PT_AP_BASE + 1);
    }
    else
    {
        m_POptions.command_line("%s", pAppInfo->szPathName);
    }
}

/****************************************************************************
函数原型	public: ~CProcessChild()
功能描述	析构函数
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
*****************************************************************************/
CProcessChild:: ~CProcessChild()
{
    if (m_pInfo) 
    {
        delete m_pInfo;
        m_pInfo = NULL;
    }
}

/*****************************************************************************
函数原型	public: EAPPSTATUS GetStatus();
功能描述	返回该进程状态
输入参数	无。
输出参数	无。
返回值  	返回该进程的状态值。
抛出异常	无。
******************************************************************************/
EAPPSTATUS CProcessChild:: GetStatus()
{
    return m_nStatus;
}

/***************************************************************************
函数原型	public: void SetStatus(EAPPSTATUS nStatus)
功能描述	设置该进程的状态
输入参数	进程的状态值。
输出参数	无。
返回值  	无。
抛出异常	无。
****************************************************************************/
void CProcessChild:: SetStatus(EAPPSTATUS nStatus)
{
    m_nStatus = nStatus;
}

/***************************************************************************
函数原型	public: time_t GetLastStartTime()
功能描述	获得最近成功启动该业务进程的绝对时间
输入参数	无。
输出参数	无。
返回值  	返回最近成功启动该业务进程的绝对时间。
抛出异常	无。
****************************************************************************/
time_t CProcessChild:: GetLastStartTime()
{
    return m_uLastStartTime;
}

/***************************************************************************
函数原型	public: PROCESS_TYPE GetProcessType()
功能描述	获得该业务进程编号
输入参数	无。
输出参数	无。
返回值  	返回该业务进程编号。
抛出异常	无。
****************************************************************************/
PROCESS_TYPE CProcessChild:: GetProcessType()
{
    return m_pInfo->nPType;
}

/***************************************************************************
函数原型	public: pid_t GetPid()
功能描述	获得该业务进程的进程ID 号。
输入参数	无。
输出参数	无。
返回值  	返回该业务进程的进程ID 号。
抛出异常	无。
****************************************************************************/
pid_t CProcessChild:: GetPid()
{
    return m_PID;
}

/****************************************************************************
函数原型	public: EAUXUPGSTATUS GetAuxUpgStatus()
功能描述	获得该业务进程的辅助升级状态
输入参数	无。
输出参数	无。
返回值  	返回该业务进程的辅助升级状态。
抛出异常	无。
*****************************************************************************/
EAUXUPGSTATUS CProcessChild:: GetAuxUpgStatus()
{
    return m_nAuxUpgStatus;
}

/****************************************************************************
函数原型	public: void SetAuxUpgStatus(EAUXUPGSTATUS nStatus)
功能描述	设置该业务进程的辅助升级操作是否完成的状态
输入参数	该业务进程的辅助升级状态。
输出参数	无。
返回值  	无。
抛出异常	无。
*****************************************************************************/
void CProcessChild:: SetAuxUpgStatus(EAUXUPGSTATUS nStatus)
{
    m_nAuxUpgStatus = nStatus;
}

/*****************************************************************************
函数原型	public: int Active()
功能描述	激活该业务进程
输入参数	无。
输出参数	无。
返回值  	激活该业务进程是否成功，如果成功返回ERR_SUCCESS，否则返回
                ERR_FAIL。
抛出异常	无。
******************************************************************************/
int CProcessChild:: Active()
{
    switch (m_nStatus)
    {
        //已经激活
        case APP_ACTIVE:
            return ERR_SUCCESS;

        case APP_READY:
        case APP_ACTIVING:
            //通过监控模块向管理模块发送激活命令
            CMonitor::SendMsgToManager(m_pInfo->nPType, CMD_ACTIVE);
            m_nStatus = APP_ACTIVING;
            return ERR_SUCCESS;

        default:
            return ERR_FAIL;
    }
}

/********************************************************************************
函数原型	public: BOOL CanRestart()
功能描述	该业务进程是支持本地恢复
输入参数	无。
输出参数	无。
返回值  	返回该业务进程是支持本地恢复。
抛出异常	无。
********************************************************************************/
BOOL CProcessChild:: CanRestart()
{
    //本进程不支持重启恢复
    if (!m_pInfo->bCanRestart)
    {
        return FALSE;
    }
 
    time_t TimeNow;
    time(&TimeNow);
   
    //如果距离上一次启动时间超过5分钟，则认为可以重启
    if ((TimeNow - m_uLastStartTime) > FIVE_MINUTE_IN_SECOND)
    {
        return TRUE;
    }

    //已重启次数小于2次，则直接重启
    if (m_uRestartCount < 2)
    {
        return TRUE;
    }

	//已重启次数等于2次，则延迟250秒再重启
    if (m_uRestartCount == 2)
    {
        m_nStartDelay = 250;
        m_nStatus = APP_SCHEDULED_START;
        return TRUE;
    } 
    
    return FALSE;  
}

/********************************************************************************
函数原型	public: void  Stop()
功能描述	强行关闭指定的业务进程
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
*********************************************************************************/
void CProcessChild:: Stop()
{
    if (APP_STOPPED == m_nStatus)
    {
        return;
    }
    
    m_nStatus = APP_STOP_PENDING;
    
    //调用ACE的静态函数，强行杀死指定进程
    ACE::terminate_process(m_PID);
   
    m_nStatus = APP_STOPPED;
}

/*******************************************************************************
函数原型	public: int Start(ACE_Process_Manager *pMgr)
功能描述	启动该业务进程
输入参数	无。
输出参数	无。
返回值  	启动该业务进程是否成功，如果成功返回ERR_SUCCESS，否则返回ERR_FAIL。
抛出异常	无。
********************************************************************************/
int CProcessChild:: Start(ACE_Process_Manager* pMgr)
{
    if (pMgr == NULL)
    {
        m_pInfo->bCanRestart = FALSE;
        return ERR_FAIL;
    }

    //因为在UNIX下，如果子进程文件不存在，SPAWN还是会返回成功，
    //在CMonitor启动子进程前先检查文件是否存在
    if(ACE_OS::access(m_pInfo->szPathName, F_OK) != 0)
    {
        m_pInfo->bCanRestart = FALSE;
        TRACE(MTS_MONITOR, SZ_KNL_PROC_NOT_EXIST, m_pInfo->szPathName);
        return ERR_FAIL;
    }

    m_PID = pMgr->spawn(m_POptions);
    
    //进程PID为-1表示创建进程失败
    if (-1 == m_PID) 
    {
        m_pInfo->bCanRestart = FALSE;
        return ERR_FAIL;
    }

    m_nStatus = APP_START_PENDING;
   
    time_t TimeNow;
    time(&TimeNow);

    if (m_uLastStartTime > 0)
    {
        //如果距离上次启动时间在5分钟内，则认为是多次重启，并将重启计数器
        //累加
        if (((long)TimeNow - m_uLastStartTime) <= FIVE_MINUTE_IN_SECOND)
        {
            m_uRestartCount++;
        }
        else
        {
            m_uRestartCount = 1;
        }
    } 
 
    m_uLastStartTime = TimeNow;
    
    return ERR_SUCCESS;
}

/***********************************************************************************
函数原型	public: char * GetAppName()
功能描述	获取该业务进程的名称
输入参数	无。
输出参数	无。
返回值  	返回该业务进程的名称。
抛出异常	无。
************************************************************************************/
const char* CProcessChild:: GetAppName()
{
    return m_pInfo->szAppName;
}

int CProcessChild:: GetLeftStartDelay(int nDiv)
{
    m_nStartDelay -= nDiv;
    return m_nStartDelay;
}
