/**************************************************************************
 *产品名    ：iGateway Bill V200        
 *模块名    ：agent
 *文件名    ：cluster_agent.cpp
 *描述信息  ：本文件是类CClusterAgent的实现。
 *版本说明  ：V200R002i00M00B00D1130A   
 *版权信息  ：版权所有（C）2001-2002 华为技术有限公司
 *作者      ：邱意平
 *创建时间  ：2001-10-30
 *修改记录  ：
 **************************************************************************/

#include "cluster_agent.h"
#include "cluster.h"
#include "cluster2.h"
#include "cluster3.h"
#include "cluster_alarm.h"
//by ldf 对应问题单D11862 2002-3-16
#include "../include/signal_tab.h"
#ifdef _PLATFORM_SOLARIS 
#include "../include/system1.h"
#endif
#include "../utility/mml_para.h"
#include "../utility/mml_report.h"

/**************************************************************************
函数原型    public: CClusterAgent(BOOL bIsSingle);
功能描述    CClusterAgent类构造函数
输入参数    bIsSingle：单、双机环境标志，单机环境下为真。
输出参数    无
返回值          无
抛出异常    无
***************************************************************************/
CClusterAgent:: CClusterAgent(BOOL bIsSingle)
{
    m_ptheCluster      = NULL;
    m_ptheClusterAlarm = NULL;
    m_bIsSingle        = bIsSingle;
    m_nPidKnl          = -1;
    //by ldf 对应问题单D11862 2002-3-16
    m_nRecvSignal      = 0;

}

/*************************************************************************
函数原型    public: ~CClusterAgent();
功能描述    对象析构函数，进行对象销毁前的清理工作。
输入参数    无
输出参数    无
返回值          无
抛出异常    无
**************************************************************************/
CClusterAgent:: ~CClusterAgent()
{
    //销毁双机告警对象
    if (m_ptheClusterAlarm)
    {
        delete m_ptheClusterAlarm;
    }
    
    //销毁双机对象 
    if (m_ptheCluster)
    {
        delete m_ptheCluster;
    }
}

/*************************************************************************
函数原型    protected:  int InitInstance(void);
功能描述    程序启动后的初始化工作，继承父类的虚函数。
输入参数    无
输出参数    无
返回值          初始化是否成功。
抛出异常    无
**************************************************************************/
int CClusterAgent:: InitInstance(void)
{
    //调用父类的初始化函数，注册路由
    CWorkModule::InitInstance();    

    //在单机环境下，不需要创建m_ptheCluster对象。
    //注意m_ptheCluster对象应先于m_ptheClusterAlarm对象创建
    if (FALSE == m_bIsSingle)
    {
        CINIFile* pINIFile = new CINIFile(GetCfgFilePath());
        pINIFile->Open();     //打开参数配置文件billsvr.ini
        
        char szClusterVer[255] = "";
        pINIFile->GetString(CFG_SEC_CLUSTER, CFG_CLUSTER_VER, DEFAULT_CLUSTER_VER, szClusterVer, 254);          
        m_sSCVer = szClusterVer;
        
        delete pINIFile;  //CINIFile类在析构函数中会关闭INI配置文件句柄    
        
        if ( strcmp(m_sSCVer.c_str(),"2.2") == 0 )
            //GetCfgFilePath()是CClusterAgent类的父类提供的
            //获得配置文件全路径名的静态函数
        {
            m_ptheCluster = new CCluster2(GetCfgFilePath());
        }
        else
        {
            m_ptheCluster = new CCluster3(GetCfgFilePath());
        }
    }
    
    //创建双机告警处理对象
    m_ptheClusterAlarm = new CClusterAlarm(m_ptheCluster); 
    
    //读配置参数
    CINIFile* pINIFile = new CINIFile(GetCfgFilePath());
    if (FALSE == pINIFile->Open())
    {
        delete pINIFile;
        return ERR_OPEN_CFG_FILE;
    }
    
    //告警检测定时间隔，设计规定Agent每隔m_nSendHeartBeatID
    //时间间隔向PT_OM发送一次心跳信息
    m_nSendHeartBeatID = SetTimer(FIVE_SECOND_IN_MILISEC);
    //告警检测定时间隔，设计规定Agent每隔m_uAlarmInterval
    //时间间隔检测一次有关的告警项
    m_uAlarmInterval = pINIFile->GetInt(CFG_SEC_ALM_AGENT, 
                                        CFG_ALM_INTERVAL, DEFAULT_ALM_INTERVAL);
    //将单位转换成毫秒
    m_uAlarmInterval *= 1000;
    //规定检测时间介于3分钟到30分钟之间
    if (((3*60*1000) > m_uAlarmInterval) || ((30*60*1000) < m_uAlarmInterval)) 
    {
        m_uAlarmInterval = DEFAULT_ALM_INTERVAL * 1000;
    }
   
    //等待KERNEL激活应答超时，设计规定当Agent成功启动
    //KERNEL进程后，如果在m_uKnlActTimeout时间间隔之后没有收到KERNEL
    //的激活应答，则执行相应的命令
    m_uKnlActTimeout = pINIFile->GetInt(CFG_SEC_ALM_AGENT, 
                                        CFG_KNL_ACT_TIMEOUT, DEFAULT_KNL_ACT_TIMEOUT);
    //将单位转换成毫秒
    m_uKnlActTimeout *= 1000;
    //规定检测时间介于1分钟到10分钟之间
    if (((60*1000) > m_uKnlActTimeout) || ((10*60*1000) < m_uKnlActTimeout))
    {
        m_uKnlActTimeout = DEFAULT_KNL_ACT_TIMEOUT * 1000;
    }

    //等待KERNEL关闭应答超时，设计规定在Agent发出关闭
    //KERNEL进程后的m_uKnlOffTimeout时间间隔后，如果KERNEL还没有退
    //出，则强行杀死KERNEL进程。
    m_uKnlOffTimeout = pINIFile->GetInt(CFG_SEC_ALM_AGENT, 
                                        CFG_KNL_OFF_TIMEOUT, DEFAULT_KNL_OFF_TIMEOUT);

    //将单位转换成毫秒
    m_uKnlOffTimeout *= 1000;

    //检测KERNEL关闭定时器，设计规定在Agent发出关闭KERNEL
    //进程的消息后，将每隔m_uKnlOffInterval间隔检测一次KERNEL进程是否
    //还存在。
    m_uKnlOffInterval = pINIFile->GetInt(CFG_SEC_ALM_AGENT, 
                                         CFG_KNL_OFF_INTERVAL, DEFAULT_KNL_OFF_INTERVAL);
    //将单位转换成毫秒
    m_uKnlOffInterval *= 1000;
    //规定检测时间介于10秒钟到1分钟之间
    if (((10*1000) > m_uKnlOffInterval) || ((60*1000) < m_uKnlOffInterval))
    {
        m_uKnlOffInterval = DEFAULT_KNL_OFF_INTERVAL * 1000;
    }

    //检测KERNEL进程定时器，设计规定Agent应定时检测KERNEL
    //进程是否正常，此定时器的时间间隔不宜过短以免影响性能。
    m_uCheckKnlInterval = pINIFile->GetInt(CFG_SEC_ALM_AGENT, 
                                           CFG_KNL_CHECK_INTERVAL, DEFAULT_KNL_CHECK_INTERVAL);
    //将单位转换成毫秒
    m_uCheckKnlInterval *= 1000;
    //规定检测时间介于10秒钟到1分钟之间
    if (((10*1000) > m_uCheckKnlInterval) || ((60*1000) < m_uCheckKnlInterval))
    {
        m_uCheckKnlInterval = DEFAULT_KNL_CHECK_INTERVAL * 1000;
    }
    
    
    //等待KERNEL进程关闭累计次数
    m_uKnlOffCount = pINIFile->GetInt(CFG_SEC_ALM_AGENT, 
                                      CFG_KNL_OFF_COUNT, DEFAULT_KNL_OFF_COUNT); 
    //规定在3-6次之间
    if ((3 > m_uKnlOffCount) || (6 < m_uKnlOffCount))
    {
        m_uKnlOffCount = DEFAULT_KNL_OFF_COUNT;
    }
    
    delete pINIFile;

    //如果启动KERNEL失败，则发起倒换 
    if (StartKernel() == FALSE)
    {
        //打印、输出启动KERNEL失败的消息
        TRACE(THISMODULE, S_AGENT_START_KNL_FAILED);
            
        Switch(FALSE);

        return ERR_FAIL; 
    }
    else
    {
        //打印、输出启动KERNEL成功的消息
        TRACE(THISMODULE, S_AGENT_START_KNL_SUCCESS);
        MSGOUT(THISMODULE, MSGOUT_LEVEL_NORMAL, 
               S_AGENT_START_KNL_SUCCESS); 
    }   

    //设置定时器，等待KERNEL激活应答
    m_nKnlActTimeoutID = SetTimer(m_uKnlActTimeout); 

    //设置定时器，定时检测KERNEL进程
    m_nCheckKnlID = SetTimer(m_uCheckKnlInterval); 

    //by ldf 对应问题单D11862 2002-3-16
    m_nTimerIDBase = SetTimer(ONE_SECOND_IN_MILISEC);

    //用ACE_Reactor的register_handler方法把本对象注册于Reactor，
    //处理SIGTERM信号
    reactor()->register_handler(SIGTERM, this);

    //注册SIGCLD信号，实时捕获KERNEL进程退出消息
    reactor()->register_handler(SIGCLD, this);

    MSGOUT(THISMODULE, MSGOUT_LEVEL_NORMAL, 
               S_AGENT_AGENT_INIT_SUCCESS);

    return ERR_SUCCESS; 
}

/************************************************************************ 
函数原型    protected:  MODULE_TYPE GetModuleType(); 
功能描述    获得CMsgProcessor对象的模块类型，继承父类的虚函数。 
输入参数    无 
输出参数    无 
返回值          本对象的模块类型
抛出异常    无
*************************************************************************/
MODULE_TYPE CClusterAgent:: GetModuleType()
{
    return MT_CLSTR;
}

/*************************************************************************
函数原型    protected:  BOOL StartKernel(void);
功能描述    启动KERNEL进程，但不等待KERNEL进程返回的激活应答。
输入参数    无
输出参数    无
返回值          如果KERNEL进程启动成功则返回TRUE；如果KERNEL进程启动失败则
                返回FALSE。
抛出异常    无
**************************************************************************/
BOOL CClusterAgent:: StartKernel(void)
{
    char szAppPath[MAX_PATH];

    //获得应用程序所在目录
    strncpy(szAppPath, GetAppPath(), MAX_PATH);
    szAppPath[MAX_PATH - 1] = '\0';

    //如果路径名最后没有带'/'，则添加'/'
    if (szAppPath[strlen(szAppPath) - 1] != '/')
    {
        strncat(szAppPath, "/", MAX_PATH);
    }
    szAppPath[MAX_PATH - 1] = '\0';

    strncat(szAppPath, S_APPNAME_KERNEL, MAX_PATH);
    szAppPath[MAX_PATH - 1] = '\0';
   
    //检查当前目录下是否存在KERNEL进程的可执行文件
    if (access(szAppPath, F_OK) < 0)
    {
        TRACE(THISMODULE, S_AGENT_CANNOT_FINDFILE, szAppPath);
        return FALSE; 
    }

    if ((m_nPidKnl = vfork()) < 0)
    {
        return FALSE;
    }    
    else if (m_nPidKnl == 0)   //子进程,Modified according to D14774
    {
        if (execl(szAppPath, szAppPath, NULL) < 0)
        {
            TRACE(THISMODULE, S_AGENT_EXECL_KNL_FAILED, szAppPath, errno);
            _exit(1);
        }
        
        return TRUE;
    }

    return TRUE;
}

/**************************************************************************
函数原型    protected:  BOOL CheckKernel(void);
功能描述    检查KERNEL进程是否正常
输入参数    无
输出参数    无
返回值          如果KERNEL进程正常则返回TRUE；如果KERNEL进程异常则返回FALSE；
抛出异常    无
***************************************************************************/
BOOL CClusterAgent:: CheckKernel(void)
{
    //在SUN Solaris操作系统中，一个进程启动后，在"/proc"目录下会对应一个
    //子目录，子目录名就是该进程的PID，Agent就用这种方法检测KERNEL进程是
    //否存在    
    if (m_nPidKnl == -1)  return FALSE;
    
    int nPid = ACE_OS::waitpid(m_nPidKnl, 0, WNOHANG);
    if(nPid == m_nPidKnl)
    {
        //提示KERNEL进程没有运行
        TRACE(THISMODULE, S_AGENT_KNL_NOT_RUNNING);
        m_nPidKnl = -1;
        return FALSE;
    }

/*
    char szDirKnl[MAX_PATH];
    sprintf(szDirKnl, "/proc/%d", m_nPidKnl);
    
    //检测目录是否存在
    if (access(szDirKnl, F_OK) < 0)
    {
        //提示KERNEL进程没有运行
        TRACE(THISMODULE, S_AGENT_KNL_NOT_RUNNING);

        return FALSE;
    }
*/


    //打印KERNEL进程在运行
    MSGOUT(THISMODULE, MSGOUT_LEVEL_NORMAL, 
               S_AGENT_KNL_IS_RUNNING);

    return TRUE;
}

/**************************************************************************
函数原型    protected:  BOOL StopKernel(void);
功能描述    向KERNEL进程发送关闭命令，但不会等待KERNEL进程退出
输入参数    无
输出参数    无
返回值          返回关闭命令发送是否成功
抛出异常    无
***************************************************************************/
BOOL CClusterAgent:: StopKernel(void)
{
    MSG_INTRA* pMsg   = new MSG_INTRA;

    pMsg->SenderPid   = PT_CLSTR;
    pMsg->SenderMid   = GetModuleType();
    pMsg->ReceiverPid = PT_KERNEL;          //发送给KERNEL进程
    pMsg->ReceiverMid = MT_CLSTRINTERFACE;  //发送给双机接口模块 
    pMsg->CmdCode     = CMD_EXIT;           //退出命令

    if (CWorkModule::SendMsg(pMsg) != ERR_SUCCESS)
    {
        return FALSE;                       //发送消息
    }
    
    TRACE(THISMODULE, S_AGENT_SEND_MSG_TO_STOP_KNL);
    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
               S_AGENT_SEND_MSG_TO_STOP_KNL);

    return TRUE;
}

/*************************************************************************
函数原型    protected:  void KillKernel(void);
功能描述    检查KERNEL进程是否还在运行，如果还在运行则强行杀掉。
输入参数    无
输出参数    无
返回值          无
抛出异常    无
**************************************************************************/
void CClusterAgent:: KillKernel(void)
{
    if (m_nPidKnl == -1)
    {
        return;
    }

    //首先检查KERNEL进程是否存在
    if (CheckKernel() == TRUE)
    {
        
        TRACE(THISMODULE, S_AGENT_SEND_SIG_TO_KILL_KNL);
        MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                   S_AGENT_SEND_SIG_TO_KILL_KNL);

        kill(m_nPidKnl, SIGTERM); //向KERNEL进程发送SIGTERM信号    
    }
}

/***************************************************************************
函数原型    protected:  void DoAnsActive(BOOL bResponse);
功能描述    处理双机接口模块返回的激活应答
输入参数    bResponse：双机接口模块返回的激活是否成功应答
输出参数    无
返回值          无
抛出异常    无
****************************************************************************/
void CClusterAgent:: DoAnsActive(BOOL bResponse)
{
    //停止KERNEL激活应答定时器
    //KERNEL进程已经应答，停止等待
    KillTimer(m_nKnlActTimeoutID);

    if (bResponse) //激活成功应答
    {
        TRACE(THISMODULE, S_AGENT_KNL_ACTIVE_SUCCESS);
        MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                   S_AGENT_KNL_ACTIVE_SUCCESS);

        //在双机环境下产生倒换告警
        if (m_bIsSingle == FALSE)
        {
            m_ptheClusterAlarm->AlarmSwitch(); 
        }
        
        //启动告警检测定时器 
        m_nAlarmIntervalID = SetTimer(m_uAlarmInterval);
    }
    else  //激活失败应答
    {
        TRACE(THISMODULE, S_AGENT_KNL_ACTIVE_FAIL);
        MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                   S_AGENT_KNL_ACTIVE_FAIL);

        StopKernel();   //向双机模块发送退出命令
        m_nKnlOffIntervalID = SetTimer(m_uKnlOffInterval);
    }
}

/*************************************************************************
函数原型    protected:  int DoManualOver(MSG_INTRA* pMsgParsed);
功能描述    处理双机接口模块转发的MML客户端的手动倒换命令，并向MML客户
                端返回能否倒换应答。
输入参数    pMsgParsed：本消息包含双机接口模块转发的MML客户端的手动倒
                换命令
输出参数    无
返回值          切换成功返回ERR_SUCCESS，否则返回ERR_FAIL； 
抛出异常    无
**************************************************************************/
int CClusterAgent:: DoManualOver(MSG_INTRA* pMsg)
{
	SInnerAlarm alm;

    struct in_addr Addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
    Addr.s_addr = pMsg->IPAddr;    
#else //_PLATFORM_HPUX
    Addr.S_un.S_addr = pMsg->IPAddr;    
#endif//_PLATFORM_HPUX

	alm.uAlarmID   = ALARM_ID_MANUAL_SWITCHOVER;
	alm.yAlarmType = AT_EVENT;
	SNPRINTF((char *)(alm.yAlarmPara), MAX_ALARM_PARA, "%s(%s)", 
		      pMsg->UserName, ACE_OS::inet_ntoa(Addr));

	SendAlarm(&alm);

    //检查对端能否接管逻辑主机
    if (Switch(TRUE) == FALSE)
    {
        return ERR_FAIL;
    }
    else //对端可以接管，则发起倒换命令
    {
       //倒换失败
       if (Switch(FALSE) == FALSE)
       {
           return ERR_FAIL;
       }
    }
    
    return ERR_SUCCESS;
}

/****************************************************************************
函数原型    protected:  BOOL Switch(BOOL bCheckSanity);
功能描述    1）如果bCheckSanity为TRUE，则调用高可用数据服务注册的FM_CHECK
                方法，检查对端是否有能力接管逻辑主机； 2）如果bCheckSanity为
                FALSE，则切换逻辑主机到对端。
输入参数    bCheckSanity：如果bCheckSanity为TRUE，则仅进行对端主机的健康
                检查；如果bCheckSanity为FALSE，则启动逻辑主机切换过程。
输出参数    无
返回值          若对端可以接管逻辑主机或启动切换成功则返回TRUE
抛出异常    无
*****************************************************************************/
BOOL CClusterAgent:: Switch(BOOL bCheckSantity)
{
    if (m_ptheCluster == NULL)   return FALSE;   //单机环境，无法切换
   
    //当前活动成员只有一个，无法切换 
    if (m_ptheCluster->GetCurrMemNum() < 2)  return FALSE;

    char szCmd[255];
    int  nStatus = 1;
    
    if ( strcmp(m_sSCVer.c_str(),"2.2") == 0 )
    {
        //获得高可用数据服务名
        const STRING sDataService = m_ptheCluster->GetDataService();
        
        //获得逻辑主机名
        const STRING sLogHostName = m_ptheCluster->GetLogicalHost();
        
        if (bCheckSantity)
        {
            //检查对端能否接管SC
            sprintf(szCmd, "hactl -n -g -s %s -l %s", 
                sDataService.c_str(), sLogHostName.c_str());
            
#ifdef _PLATFORM_SOLARIS 
            nStatus = system1(szCmd);
#else
            nStatus = system(szCmd);
#endif
        }
        else
        {
            //切换SC到对端
            sprintf(szCmd, "hactl -g -s %s -l %s", 
                sDataService.c_str(), sLogHostName.c_str());
#ifdef _PLATFORM_SOLARIS 
            nStatus = system1(szCmd);
#else
            nStatus = system(szCmd);
#endif
        }
    }
    else
    {
        //获得本主机名
        const STRING sLocalNodename = m_ptheCluster->GetLocalNodename();
        
        if (bCheckSantity)
        {
            //检查对端能否接管SC ,因为目前的判断是采用看online的节点个数，
            nStatus = 0 ;
            
        }
        else
        {
            //切换SC到对端
			//by ldf 2004-06-03 对应问题单 SWPD05528
            sprintf(szCmd, "scswitch -S -h  %s &", sLocalNodename.c_str());
#ifdef _PLATFORM_SOLARIS 
            nStatus = system1(szCmd);
#else
            nStatus = system(szCmd);
#endif
        }
        
    }
    
    return (nStatus == 0);  //system()系统调用成功时返回0
}

/****************************************************************************
函数原型    protected:  void DoReqStatus(void);
功能描述    双机代理模块检测并发送双机状态数据（心跳情况）给操作维护进程
                中的性能模块。
输入参数    无
输出参数    无
返回值          无
抛出异常    无
*****************************************************************************/
void CClusterAgent:: DoReqStatus(void)
{
    MSG_INTRA* pMsg   = new (2)MSG_INTRA;
   
    pMsg->SenderPid   = PT_CLSTR;
    pMsg->SenderMid   = GetModuleType();
    pMsg->ReceiverPid = PT_OM;
    pMsg->ReceiverMid = MT_PERF;     //发送给性能统计模块

    pMsg->CmdCode     = CMD_ANS_HB_STATUS;  //心跳状态查询应答
   
    int nPrvNet = 0;
    int nActivePrvNet = 0;    //记录活动私网数量

    if (m_bIsSingle == FALSE)
    {
        //获得私网个数
        nPrvNet = m_ptheCluster->GetNumOfPrvNet();

        if(m_ptheCluster->GetCurrMemNum() > 1)
        {
            for (int i = 0; i < nPrvNet; i++)
            {
                if (m_ptheCluster->GetPrvNetStatus(i))
                {
                    nActivePrvNet++;
                }
            }
        }
    }
     
    pMsg->Data[0] = (BYTE)nPrvNet;        //第一字节存放私网个数
    pMsg->Data[1] = (BYTE)nActivePrvNet;  //第二字节存放可用心跳个数
    
    SendMsg(pMsg);   //发送消息
}

/***************************************************************************
函数原型      protected: void DoAskFailover(EFAILOVERCAUSE_TYPE eCause);
功能描述      处理双机接口模块发给双机代理模块的倒换申请，如果对端不能接管
              逻辑主机则抛弃该消息。
输入参数      eCause：双机接口模块转发的其它模块的倒换申请原因
输出参数      无
返回值        无
抛出异常      无
****************************************************************************/
void CClusterAgent:: DoAskFailover(EFAILOVERCAUSE_TYPE eCause)
{
    char szCause[1024];
    char szMsg[1024];
    BOOL bDoFailOver = FALSE;
    
    //根据申请倒换的原因，仲裁该次倒换是否可以执行
    switch (eCause)
    {
    //由于双机系统是共享磁盘阵列，磁盘空间不足对主备机都是一样的，因此倒换无效
    case ERROR_NOENOUGHDISK_PRIMARY:    //主硬盘空间不足
        strncpy(szCause, S_ERR_NOENOUGHDISK_PRIMARY, 1024);
        break;
        
    case ERROR_NOENOUGHDISK_SECONDARY:  //副硬盘空间不足
        strncpy(szCause, S_ERR_NOENOUGHDISK_SECONDARY, 1024);
        break;
        
    case ERROR_HARDDISK:                //共享RAID的硬盘故障
        strncpy(szCause, S_ERR_HARDDISK, 1024);
        break;

    //对于写文件失败、进程异常、资源不足，则倒换可以进行
    case ERROR_WRITEFILE:
        strncpy(szCause, S_ERR_WRITEFILE, 1024);
        bDoFailOver = TRUE;
        break;
        
    case ERROR_PROCESS_EXCEPTION:
        strncpy(szCause, S_ERR_PROCESS_EXCEPTION, 1024);
        bDoFailOver = TRUE;
        break;
        
    case ERROR_ACTIVE:
        strncpy(szCause, S_ERR_ACTIVE, 1024);
        bDoFailOver = TRUE;
        break;
        
    case ERROR_RESOURCE:
        strncpy(szCause, S_ERR_RESOURCE, 1024);
        bDoFailOver = TRUE;
        break;

    default:
        SNPRINTF(szCause, 1024, "%s(CauseCode = %d)", 
                 S_ERR_UNRECOGNIZED, eCause);
        break;
    }
    szCause[1023] = 0;
    SNPRINTF(szMsg, 1024, S_AGENT_ASK_FAILOVER, szCause);
    szMsg[1023] = 0;
    
    TRACE(THISMODULE, szMsg);
    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, szMsg);


    
    if(bDoFailOver && Switch(TRUE))  //检查对端是否可以接管
    {
        WriteLog(MTS_CLSTR, szMsg);
        Switch(FALSE); //对端可以接管，进行SC切换
    }
    //by ldf 2002.3.15 对应问题单D11827
    else if(ERROR_PROCESS_EXCEPTION == eCause)
    //不能倒换，让所有进程退出
    {
        m_nRecvSignal = SIGTERM;
    }
}

/****************************************************************************
函数原型    protected:  void ParseMsg(MSG_INTRA* pMsg) ;
功能描述    处理所有的非退出消息。
输入参数    pMsg：消息队列的头消息，该消息有待于具体的模块进行具体
                的处理，消息被处理后，该消息应该被释放。
输出参数    无
返回值          无
抛出异常    无
*****************************************************************************/
void CClusterAgent:: ParseMsg(MSG_INTRA* pMsg)
{
    switch (pMsg->CmdCode)
    {
        case CMD_ANS_ACTIVE:     //KERNEL进程的激活应答
        {
            MSGOUT(THISMODULE, MSGOUT_LEVEL_NORMAL,
                       S_AGENT_RCV_KNL_ACTIVE);

            BOOL bActive = *((UINT4*)pMsg->Data);
        
            DoAnsActive(bActive);
            break;
        }
   
        case CMD_ASK_FAILOVER:   //倒换申请
        {
            SFailOverCause* pCause = (SFailOverCause *)(pMsg->Data);
            EFAILOVERCAUSE_TYPE eCause = pCause->Cause;
             
            if (m_bIsSingle == FALSE)
            {
                DoAskFailover(eCause);
            }
            else if(ERROR_PROCESS_EXCEPTION == eCause)
            //单机下不能倒换，让所有进程退出
            {
                m_nRecvSignal = SIGTERM;
            }
            break;    
        } 
      
        case CMD_REQ_STATUS:     //性能模块的双机状态查询
        {
            DoReqStatus();
            break;
        }
    
        case CMD_MML:
        {
            //从MML命令行中取命令字
            CParseResult pr(pMsg);
            pr.CreateParseResult(pMsg);
            char szAnswer[128];
            
            if (MML_RESET == pr.GetCmdCode() )
            {
                DoReset(pMsg);
               
            }


            //by ldf 2002.4.1 对应问题单D12300 D13201
            static BOOL SwitchLock = FALSE;

            if ( (MML_SWP == pr.GetCmdCode()) && (FALSE == SwitchLock) )
            {
                //修改完毕

                //只有在双机环境下，倒换才有意义
                if (m_bIsSingle == FALSE)
                {
                    //执行切换操作并返回切换结果
                    int nRet = DoManualOver(pMsg);

                    //重要操作，写日志
                    SLog log;
                    log.LogType    = SLog::LOG_OPER;
                    log.uIPAddr    = pMsg->IPAddr;
                    if (ERR_SUCCESS == nRet)
                    {
                        sprintf(szAnswer, "%s", S_AGENT_HANDOVER_SUCCESS);
                        log.OperResult = SLog::OPER_SUCCESS;

                        //by ldf 2002.4.27 对应问题单D12300 D13201
                        SwitchLock = TRUE;
                        //修改完毕
                    }
                    else
                    {
                        sprintf(szAnswer, "%s", S_AGENT_HANDOVER_FAIL);
                        log.OperResult = SLog::OPER_FAIL;
                    }

                    strcpy(log.szUserName, pMsg->UserName);
                    strcpy(log.szOperCmd, S_AGENT_MML_CMD_SWP);
                    strcpy(log.szCmdDesc, S_AGENT_MML_CMDDESC_SWP);

                    WriteLog(&log);   //写日志
                      
                    CMMLReport Report(THISMODULE);
                    Report.MakeReportHead();
                    Report.MakeCmdDisplay(pr.GetCmdLine(), 0, nRet, THISMODULE);
                    Report.AddString(szAnswer);
                    Report.AddString(STR_NEWLINE);  //换行
                    Report.MakeReportTail();
                     
                    const char* pChar = Report.GetReport();
                    
                    int nLen = strlen(pChar);
                    MSG_INTRA* pAns = new (nLen)MSG_INTRA;
    
                    memcpy((void *)(pAns->Data), pChar, nLen);
                    pAns->SenderPid   = GetProcessType();
                    pAns->SenderMid   = GetModuleType();
                    pAns->ReceiverPid = PT_OM;           
                    pAns->ReceiverMid = MT_MMLDEBUG;
                    pAns->AppType     = pMsg->AppType;
                    pAns->ClientNo    = pMsg->ClientNo;
                    pAns->IPAddr      = pMsg->IPAddr;
                     
                    SendMsg(pAns); 
 
                    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, szAnswer);
            }
               //by ldf 3-13 对应问题单D11722 2002.3.14
                else
                {
                    sprintf(szAnswer, "%s", S_AGENT_CANNOT_HANDOVER);  
                    CMMLReport Report(THISMODULE);
                    Report.MakeReportHead();
                    Report.MakeCmdDisplay(pr.GetCmdLine(), 0, ERR_FAIL, THISMODULE);
                    Report.AddString(szAnswer);
                    Report.AddString(STR_NEWLINE);  //换行
                    Report.MakeReportTail();
                     
                    const char* pChar = Report.GetReport();
                    
                    int nLen = strlen(pChar);
                    MSG_INTRA* pAns = new (nLen)MSG_INTRA;
    
                    memcpy((void *)(pAns->Data), pChar, nLen);
                    pAns->SenderPid   = GetProcessType();
                    pAns->SenderMid   = GetModuleType();
                    pAns->ReceiverPid = PT_OM;           
                    pAns->ReceiverMid = MT_MMLDEBUG;
                    pAns->AppType     = pMsg->AppType;
                    pAns->ClientNo    = pMsg->ClientNo;
                    pAns->IPAddr      = pMsg->IPAddr;
                     
                    SendMsg(pAns); 
 
                    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, szAnswer);
                }
            }
           
            break; 
        }

        default:
        {
            break;
        }
    }
}

/*************************************************************************
函数原型    protected:  void OnTimer(const long nTimerID);
功能描述    对编号为nTimerID的定时器的触发事件进行处理
输入参数    nTimerID：当前需要处理的定时器事件对应的定时器的ID
输出参数    无
返回值          无
抛出异常    无
**************************************************************************/
void CClusterAgent:: OnTimer(const long nTimerID)
{
    if (nTimerID == m_nAlarmIntervalID)
    {
        //告警定时检测
        m_ptheClusterAlarm->Alarm();
    }
    else if (nTimerID == m_nCheckKnlID)
    { 
        //定时检测KERNEL进程
        if (!CheckKernel())            //如果KERNEL已经退出，则双机代理也退出 
        {
            MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                   S_AGENT_SEND_QUIT_TO_MGR);
 
            SendQuitToMe();   //给管理模块发送退出命令
        }
    }
    else if (nTimerID == m_nKnlActTimeoutID)    
    {
        //KERNEL激活应答超时
        if (!CheckKernel())
        {
            MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                   S_AGENT_SEND_QUIT_TO_MGR);

            SendQuitToMe();   //给管理模块发送退出命令
        }
        else
        {
            KillTimer(m_nKnlActTimeoutID);
            KillTimer(m_nCheckKnlID);
            KillTimer(m_nAlarmIntervalID);
            KillTimer(m_nSendHeartBeatID);
            StopKernel();      //向KERNEL进程发送关闭命令
            
            //定时检测KERNEL是否已经关闭
            m_nKnlOffIntervalID = SetTimer(m_uKnlOffInterval);
            m_uKnlOffCount = 0;
        }
    } 
    else if (nTimerID == m_nKnlOffIntervalID)
    {
        if (!CheckKernel())
        {
            MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                S_AGENT_SEND_QUIT_TO_MGR);
            
            SendQuitToMe();     //给管理模块发退出命令
        }
        else
        {
            m_uKnlOffCount += m_uKnlOffInterval;
            if (m_uKnlOffCount >= m_uKnlOffTimeout)
            {
                KillKernel();               //强行杀死KERNEL
                
                MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                    S_AGENT_SEND_QUIT_TO_MGR);
                
                SendQuitToMe();             //向管理模块发退出命令
            } 
        }
    }
    else if ( nTimerID == m_nSendHeartBeatID )
    {
        DoReqStatus();
    }

    //by ldf 对应问题单D11862 2002-3-16
    else if(nTimerID == m_nTimerIDBase)
    {
        proc_signal(m_nRecvSignal);
        m_nRecvSignal = 0;
    }

}

/****************************************************************************
函数原型    protected:  int handle_signal(int signum, siginfo_t*, 
                                              ucontext_t* ut)
功能描述    根据不同信号执行相应处理
输入参数    signum：信号
输出参数    无
返回值          返回值意义如下： >0: 让本handler再次取得处理之前让其他handler
                可被调用, 用于handler间的合作； =0: 保持登记； <0: 调用
                handle_close并取消登记。
抛出异常    无
*****************************************************************************/
int CClusterAgent:: handle_signal(int signum, siginfo_t* st,
                                  ucontext_t* ut)

{
    //by ldf 对应问题单D11862 2002-3-16
    if(( signum > 0) && ( signum <= 36))
    {
        m_nRecvSignal = signum;
    }

    return 0;
}

//by ldf 对应问题单D11862 2002-3-16
void CClusterAgent:: proc_signal(int signum)
{
     switch (signum)
     {
     case SIGTERM:
         TRACE(THISMODULE, S_AGENT_RCV_SIGTERM);
         MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT,
                S_AGENT_RCV_SIGTERM);

         if (!CheckKernel())
         {
             MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                    S_AGENT_SEND_QUIT_TO_MGR);

             SendQuitToMe();      //向管理模块发送退出命令
         }
         else
         {
             KillTimer(m_nKnlActTimeoutID);
             KillTimer(m_nCheckKnlID);
             KillTimer(m_nAlarmIntervalID);
             KillTimer(m_nSendHeartBeatID);
             StopKernel();        //
            
             m_nKnlOffIntervalID = SetTimer(m_uKnlOffInterval);
         }
         break;
     
     //KERNEL进程退出
     case SIGCLD:
         MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                S_AGENT_RCV_SIGCLD);
         TRACE(THISMODULE, S_AGENT_RCV_SIGCLD);
         
                   
         MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                S_AGENT_SEND_QUIT_TO_MGR);

         SendQuitToMe();
 
         break;
         
     default:
         break; 
     }
}
/********************************************************
* 名称: CClusterAgent::OnReset
* 功能: 处理手工重启请求
* 输入: pMsgParsed : 消息帧
* 输出:
* 返回: void
********************************************************/
void CClusterAgent::DoReset(MSG_INTRA* pMsgParsed)
{
    //记录操作员用户名
    char szResetCfgPath[MAX_PATH];
    SNPRINTF(szResetCfgPath, sizeof(szResetCfgPath),
             "%s/%s", GetAppPath(), S_RESET_INFO_FILE_NAME);
    szResetCfgPath[sizeof(szResetCfgPath) - 1] = '\0';

    CINIFile ini(szResetCfgPath);
    BOOL bSucc = ini.Open();
    if(bSucc)
    {
        char szUserName[48];        
        strcpy(szUserName, pMsgParsed->UserName);
        
        ini.WriteString(CFG_SEC_OPERATOR,CFG_OPERATOR_KEY_NAME,szUserName);//记录用户名信息
        ini.Close();
    }
    else
    {
        TRACE(MTS_CLSTR, S_BS_FAIL_OPEN_FILE, szResetCfgPath);
    }

    //重要操作，写日志
    SLog log;
    log.LogType    = SLog::LOG_OPER; 
    log.uIPAddr    = pMsgParsed->IPAddr;
    log.OperResult = SLog::OPER_SUCCESS;
    strcpy(log.szUserName, pMsgParsed->UserName);
    strcpy(log.szOperCmd, S_AGENT_MML_CMD_RESET);
    strcpy(log.szCmdDesc, S_AGENT_MML_CMDDESC_RESET);

    WriteLog(&log);   //写日志

    //响应手工重启请求
    char szAnswer[128];
    sprintf(szAnswer, "%s", S_MANNUL_RESET_SUCCESS);

    CParseResult pr(pMsgParsed);
    pr.CreateParseResult(pMsgParsed);
    
    CMMLReport Report(THISMODULE);
    Report.MakeReportHead();
    Report.MakeCmdDisplay(pr.GetCmdLine(), 0, ERR_SUCCESS, THISMODULE);
    Report.AddString(szAnswer);
    Report.AddString(STR_NEWLINE);  //换行
    Report.MakeReportTail();
    
    const char* pChar = Report.GetReport();
    
    int nLen          = strlen(pChar);
    MSG_INTRA* pAns   = new (nLen)MSG_INTRA;
    pAns->SenderPid   = GetProcessType();
    pAns->SenderMid   = GetModuleType();
    pAns->ReceiverPid = PT_OM;
    pAns->ReceiverMid = MT_MMLDEBUG;
    pAns->AppType     = pMsgParsed->AppType;
    pAns->ClientNo    = pMsgParsed->ClientNo;
    pAns->IPAddr      = pMsgParsed->IPAddr;
    
    memcpy(pAns->Data, pChar, nLen);
    SendMsg(pAns);
   
    //双机进程退出
    StopKernel();
}

