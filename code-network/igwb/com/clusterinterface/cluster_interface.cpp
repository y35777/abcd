/***************************************************************************
 *产品名        ：iGateway Bill V200
 *模块名        ：双机接口模块(ClusterInterface)
 *文件名        ：cluster_interface.cpp
 *描述信息      ：该类封装了双机接口模块管理双机事务的接口。
 *版本说明      ：V200R001i00M00B00D1130A
 *版权信息      ：版权所有（C）2001-2002 华为技术有限公司
 *作者          ：邱意平
 *创建时间      ：2001-10-30
 *修改记录      ：
****************************************************************************/

#include "cluster_interface.h"
#include "resource.h"

/***************************************************************************
函数原型	public: CClusterInterface()
功能描述	构造函数，初始化成员变量。
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
****************************************************************************/
CClusterInterface:: CClusterInterface()
{
    //当产生磁盘空间不足时，双机接口模块会向所有接入点的网络
    //模块发送暂停接收话单的命令，m_bNetworkPaused指示是否发
    //送过该命令，初始化为FALSE，表示不曾发送过暂停接收话单的
    //命令
    m_bNetworkPaused = FALSE;            
    
    //m_yDiskFullMask指示是否产生过磁盘空间不足的故障，bit0为1
    //指示前存盘空间不足，bit1为1指示后存盘空间不足，初始化为
    //0表示不曾出现前、后存盘空间不足的故障
    m_yDiskFullMask   = NO_DISK_FULL;

    m_nAPCount        = 0;
    m_pFailOverCause  = NULL;
}

/***************************************************************************
函数原型	public: ~CClusterInterface()
功能描述	析构函数
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
****************************************************************************/
CClusterInterface:: ~CClusterInterface()
{
    if (m_pFailOverCause)
    {
        delete m_pFailOverCause;
        m_pFailOverCause = NULL;
    }
}

/**************************************************************************
函数原型	public: virtual int InitInstance()
功能描述	初始化函数
输入参数	无。
输出参数	无。
返回值  	返回初始化是否成功，失败返回非零值，成功返回0。
抛出异常	无。
***************************************************************************/
int CClusterInterface::InitInstance()
{
    //调用基类的初始化函数
    int nRet = CWorkModule::InitInstance();

    if (nRet != ERR_SUCCESS)
    {
        m_nStatus = CLSTRINT_FAILURE;
        return nRet;
    }

    //注册定时器，初始化状态
    m_lTimerID5Sec = SetTimer(FIVE_SECOND_IN_MILISEC);
    
    m_nStatus = CLSTRINT_INIT;      //双机接口正在初始化

    //从配置文件中读取接入点数
    CINIFile *pIniFile = new CINIFile(GetCfgFilePath());
    if (!pIniFile->Open())
    {
        pIniFile->Close();
        delete pIniFile;
        m_nStatus = CLSTRINT_FAILURE;

        return ERR_OPEN_CFG_FILE;
    }
    
    m_nAPCount = (UINT4)pIniFile->GetInt(CFG_SEC_COMMON,
					 CFG_COMMON_KEY_APCOUNT, 
					 CFG_COMMON_KEY_APCOUNT_DEFAULT);
    if (0 > m_nAPCount)
    {
		pIniFile->Close();
		delete pIniFile;
        return ERR_FAIL;
    }

    int nNoCluster = pIniFile->GetInt(
                          CFG_SEC_COMMON,
                          CFG_COMMON_KEY_NO_CLUSTER,
                          CFG_COMMON_KEY_NO_CLUSTER_DEFAULT);

    int nClusterMode = pIniFile->GetInt(
                          CFG_SEC_COMMON,
                          CFG_COMMON_KEY_CLUSTER_MODE,
                          CFG_COMMON_KEY_CLUSTER_MODE_TEST);
    if(nClusterMode == CFG_COMMON_KEY_CLUSTER_MODE_TEST)
    //ClusterMode未配置
    {
        if(nNoCluster == CFG_COMMON_KEY_NO_CLUSTER_DEFAULT)
        //NoCluster未配置
        {
            m_bSingleServer = !CFG_COMMON_KEY_CLUSTER_MODE_DEFAULT;
        }
        else
        {
            m_bSingleServer = nNoCluster;
        }
    }
    else
    {
        m_bSingleServer = !nClusterMode;
    }

    pIniFile->Close();
    delete pIniFile;

    return ERR_SUCCESS;
}

/**************************************************************************
函数原型	public: virtual void ExitInstance()
功能描述	退出时的清理工作
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
***************************************************************************/
void CClusterInterface:: ExitInstance()
{
    //注销定时器
    KillTimer(m_lTimerID5Sec);

    CWorkModule::ExitInstance();
}

/**************************************************************************
函数原型	protected: void  ParseMsg(MSG_INTRA* pMsg)
功能描述	消息分发函数
输入参数	由其它模块发送来的消息。
输出参数	无。
返回值  	无。
抛出异常	无。
***************************************************************************/
void CClusterInterface:: ParseMsg(MSG_INTRA* pMsg)
{
    switch (pMsg->CmdCode)
    {
        //监控模块发送的初始化应答
        case CMD_ANS_INIT:
        {
            //判断是否是监控模块发送的初始化应答消息
            if (MT_MONITOR == pMsg->SenderMid)
            {
                ProcInitAns(*((BOOL *)(pMsg->Data)));
            }

            break;
        }

        //监控模块发送的激活应答
        case CMD_ANS_ACTIVE:
        {
            if (MT_MONITOR == pMsg->SenderMid)
            {
                ProcActiveAns(*((BOOL *)(pMsg->Data)));
            }

            break;
        }

        //双机代理或者管理模块发送的退出命令
        case CMD_EXIT:
        {
            if ((CLSTRINT_DEACTIVING != m_nStatus) 
             && (CLSTRINT_DEACTIVE   != m_nStatus))
            {
                 TRACE(THISMODULE, S_CLUSTERINTERFACE_MSG_DEACTIVEME);
                 MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, 
                        S_CLUSTERINTERFACE_MSG_DEACTIVEME);
                 
                 DeActive();
            }

	    break;
        }

        case CMD_ANS_DEACTIVE:
        {
            if (CLSTRINT_DEACTIVING == m_nStatus)
            {
                TRACE(THISMODULE, S_CLUSTERINTERFACE_MSG_SHUTDOWN);
                MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, 
                       S_CLUSTERINTERFACE_MSG_SHUTDOWN);
                Shutdown();
            }
        
            break;
        }

         //处理双机倒换请求
         case CMD_ASK_FAILOVER:
         {
              BYTE nCause = *((BYTE *)(pMsg->Data));
              void* pData = (void*) ((BYTE *)(pMsg->Data) + sizeof(BYTE));
              UINT4 nLen = pMsg->Len - sizeof(BYTE);

              TRACE(THISMODULE, S_CLUSTERINTERFACE_MSG_RCV_ASKFAILOVER, nCause);
              MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, 
                     S_CLUSTERINTERFACE_MSG_RCV_ASKFAILOVER, nCause);

              ProcAskedFailOver((PROCESS_TYPE)(pMsg->SenderPid), 
                                (MODULE_TYPE)(pMsg->SenderMid), 
                                 nCause, 
                                 pData, 
                                 nLen);
              break;
          }
   
          //手工倒换请求
          case CMD_MANUALOVER:
          {
              //手工倒换命令由MML SERVER直接转发到双机代理部分
              //不需要经过双机接口转发
              //ProcManualOverCmd();

              break;
          }

          case CMD_DISKSPACERECOVERY:
          {
              BYTE yDiskFullMask = *((BYTE *)(pMsg->Data));

              if ((yDiskFullMask == PRIMARY_DISK_FULL) 
               || (yDiskFullMask == SECONDARY_DISK_FULL))
              {
                  //只有前后磁盘空间均正常，才对磁盘空间恢复进行处理
                  m_yDiskFullMask = m_yDiskFullMask & (~yDiskFullMask);

                  if (NO_DISK_FULL == m_yDiskFullMask) 
                  {
                      ProcDiskSpaceRecovery();
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

/***********************************************************************************
函数原型	protected: virtual MODULE_TYPE GetModuleType();
功能描述	返回模块类型
输入参数	无。
输出参数	无。
返回值  	模块类型。
抛出异常	无。
************************************************************************************/
MODULE_TYPE CClusterInterface:: GetModuleType()
{
     return MT_CLSTRINTERFACE;
}

/**********************************************************************************
函数原型	protected: void SendActiveAnsToCluster(BOOL bResult)
功能描述	向双机进程的双机代理/管理模块发激活应答消息
输入参数	bResult---激活结果。 
输出参数	无。
返回值  	无。
抛出异常	无。
***********************************************************************************/
void CClusterInterface:: SendActiveAnsToCluster(BOOL bResult)
{
    MSG_INTRA *pMsg  = new(sizeof(UINT4)) MSG_INTRA;

    pMsg->SenderPid   = GetProcessType(); 
    pMsg->SenderMid   = GetModuleType();
    pMsg->ReceiverPid = PT_CLSTR;
    pMsg->ReceiverMid = MT_CLSTR;
    pMsg->CmdCode     = CMD_ANS_ACTIVE;
    pMsg->Len         = sizeof(UINT4);

    *(UINT4 *)(pMsg->Data) = bResult;

    SendMsg(pMsg);
}

/***********************************************************************************
函数原型	protected: void ProcInitAns(BOOL bResult)
功能描述	处理初始化应答消息
输入参数	如果初始化成功，为TRUE；否则为FALSE。
输出参数	无。
返回值  	无。
抛出异常	无。
************************************************************************************/
void CClusterInterface:: ProcInitAns(BOOL bResult)
{
    //当前状态不是在初始化
    if (CLSTRINT_INIT != m_nStatus)
    {
         return;
    }
   
    if (TRUE == bResult)
    {
        //监控模块初始化成功，发起激活操作
        TRACE(THISMODULE, S_CLUSTERINTERFACE_MSG_MONITORINIT_OK, bResult);
        
        m_nStatus = CLSTRINT_ACTIVING;
        Active();
    }
    else
    {
        //系统初始化失败，向双机代理模块发送初始化失败的消息
        TRACE(THISMODULE, S_CLUSTERINTERFACE_MSG_MONITORINIT_FAIL);

        m_nStatus = CLSTRINT_FAILURE;
        SendActiveAnsToCluster(FALSE);

/*
#ifdef _PLATFORM_HPUX
#ifdef _PRODUCT_CCS
        //added by chenliangwei, 2003-04-30
        if (m_bSingleServer)
        {
            Shutdown();
        }
        //added end
#endif //_PRODUCT_CCS
#endif //_PLATFORM_HPUX
*/
    }
}

/***********************************************************************************
函数原型	protected: void ProcActiveAns(BOOL bResult)
功能描述	处理激活应答消息
输入参数	如果激活成功，为TRUE ；否则为FALSE。
输出参数	无。
返回值  	无。
抛出异常	无。
************************************************************************************/
void CClusterInterface:: ProcActiveAns(BOOL bResult)
{
    if (CLSTRINT_ACTIVING != m_nStatus)
    {
        return;
    }

    TRACE(THISMODULE, S_CLUSTERINTERFACE_MSG_RCV_ANSACTIVE, bResult);
    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, 
           S_CLUSTERINTERFACE_MSG_RCV_ANSACTIVE, bResult);

    //将自身的状态迁移，并向双机代理模块应答
    if (TRUE == bResult)
    {
        m_nStatus = CLSTRINT_ACTIVE;
        SendActiveAnsToCluster(TRUE);     
    } 
    else
    {
        m_nStatus = CLSTRINT_FAILURE;
        SendActiveAnsToCluster(FALSE); 

/*
#ifdef _PLATFORM_HPUX
#ifdef _PRODUCT_CCS
        //added by chenliangwei, 2003-04-30
        if (m_bSingleServer)
        {
            Shutdown();
        }
        //added end
#endif //_PRODUCT_CCS
#endif //_PLATFORM_HPUX
*/
    }
}

/*************************************************************************************
函数原型	protected: void Active()
功能描述	发起激活操作
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
*************************************************************************************/
void CClusterInterface:: Active()
{
    //向监控模块发送激活命令
    if ((CLSTRINT_INIT == m_nStatus) || (CLSTRINT_ACTIVING == m_nStatus))
    {
        MSG_INTRA *pMsg = new MSG_INTRA;

        pMsg->SenderPid   = PT_SELF; 
        pMsg->SenderMid   = GetModuleType();
        pMsg->ReceiverPid = PT_SELF;
        pMsg->ReceiverMid = MT_MONITOR;
        pMsg->CmdCode     = CMD_ACTIVE;

        SendMsg(pMsg);
        
        //将自身状态迁移到正在激活状态
        m_nStatus = CLSTRINT_ACTIVING;
    }
}

/*********************************************************************************
函数原型	protected: void DeActive()
功能描述	发起去激活操作
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
**********************************************************************************/
void CClusterInterface:: DeActive()
{
    //向监控模块发送去激活命令
    if (CLSTRINT_DEACTIVE != m_nStatus) 
    { 
        MSG_INTRA *pMsg   = new MSG_INTRA;

        pMsg->SenderPid   = PT_SELF; 
        pMsg->SenderMid   = GetModuleType();
        pMsg->ReceiverPid = PT_SELF;
        pMsg->ReceiverMid = MT_MONITOR;
        pMsg->CmdCode     = CMD_DEACTIVE;

        SendMsg(pMsg);

        //将自身状态迁移到去激活状态
        m_nStatus = CLSTRINT_DEACTIVING;
    }
}

/**********************************************************************************
函数原型	protected: void  Shutdown()
功能描述	发起关闭退出操作
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
***********************************************************************************/
void CClusterInterface:: Shutdown()
{
    MSG_INTRA *pMsg   = new MSG_INTRA;

    pMsg->SenderPid   = PT_SELF; 
    pMsg->SenderMid   = GetModuleType();
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = MT_MONITOR;
    pMsg->CmdCode     = CMD_EXIT;

    SendMsg(pMsg);
}

/**********************************************************************************
函数原型	protected: void ProcAskedFailOver(const PROCESS_TYPE pt, 
                                                  const MODULE_TYPE mt, 
                                                  const BYTE nCause, 
                                                  const void* pData = 0, 
                                                  const UINT4 nLen =0)
功能描述	处理其它模块发送的倒换申请
输入参数	pt---发送倒换申请的进程编号。 
                mt---发送倒换申请的模块号 
                nCause---倒换原因 
                pData---附带数据的指针 
                nLen---附带数据长度。
输出参数	无。
返回值  	无。
抛出异常	无。
***********************************************************************************/
void CClusterInterface:: ProcAskedFailOver(const PROCESS_TYPE pt, 
                                           const MODULE_TYPE mt, 
                                           const BYTE nCause, 
                                           const void* pData, 
                                           const UINT4 nLen)
{
    if (m_pFailOverCause)
    {
        delete m_pFailOverCause;
        m_pFailOverCause = NULL;
    }

    m_pFailOverCause = new (nLen)SFailOverCause;
    m_pFailOverCause->PID   = pt;
    m_pFailOverCause->MID   = mt;
    m_pFailOverCause->Cause = (EFAILOVERCAUSE_TYPE) nCause;

    memcpy(m_pFailOverCause->Para, pData, nLen);

    MSG_INTRA *pMsg = new(FAILOVERCAUSE_HEADLENTGH 
                          + m_pFailOverCause->Length)MSG_INTRA;

    pMsg->SenderPid   = GetProcessType(); 
    pMsg->SenderMid   = GetModuleType();
    pMsg->ReceiverPid = PT_CLSTR;
    pMsg->ReceiverMid = MT_CLSTR;
    pMsg->CmdCode     = CMD_ASK_FAILOVER;

    memcpy(pMsg->Data, m_pFailOverCause, pMsg->Len);

    //向双机代理发送倒换申请
    SendMsg(pMsg);

    //故障原因是前存盘空间不足
    if (ERROR_NOENOUGHDISK_PRIMARY == nCause)
    {
        if (NO_DISK_FULL == m_yDiskFullMask)
        {
           //请求各个接入点进程的网络模块暂停收发话单
           MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT,
                  S_MSG_DISKFULL_MTNETWORK_PAUSING);

           ProcDiskSpaceFull();
        }
	
        m_yDiskFullMask = m_yDiskFullMask | PRIMARY_DISK_FULL;
    }
    //故障原因是后存盘空间不足
    else if (ERROR_NOENOUGHDISK_SECONDARY == nCause)
    {
        if (NO_DISK_FULL == m_yDiskFullMask)
        {
            //请求各个接入点进程的网络模块暂停收发话单
            MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, 
                   S_MSG_DISKFULL_MTNETWORK_PAUSING);

            ProcDiskSpaceFull();
        }
 
        m_yDiskFullMask = m_yDiskFullMask | SECONDARY_DISK_FULL;
     }
     else
     {
          m_nStatus = CLSTRINT_FAILOVER;
     }
}

/*****************************************************************************************
函数原型	protected: void  ProcManualOverCmd()
功能描述	处理MML模块发来的手动倒换申请
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
******************************************************************************************/
void CClusterInterface:: ProcManualOverCmd()
{
    MSG_INTRA *pMsg   = new MSG_INTRA;

    pMsg->SenderPid   = GetProcessType(); 
    pMsg->SenderMid   = GetModuleType();
    pMsg->ReceiverPid = PT_CLSTR;
    pMsg->ReceiverMid = MT_CLSTR;
    pMsg->CmdCode     = CMD_MANUALOVER;
		
	SendMsg(pMsg);
}

/***************************************************************************************
函数原型	protected: void ProcDiskSpaceFull()
功能描述	处理磁盘空间满的通知，请求网络模块暂停收发话单
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
****************************************************************************************/
void CClusterInterface:: ProcDiskSpaceFull()
{
    int i = 0;
    for (i = 0; i < m_nAPCount; i++)
    {
        MSG_INTRA *pMsg   = new MSG_INTRA;

        pMsg->SenderPid   = GetProcessType(); 
        pMsg->SenderMid   = GetModuleType();
        pMsg->ReceiverPid = PT_AP_BASE + i;
        pMsg->ReceiverMid = MT_NETWORK;
        pMsg->CmdCode     = CMD_STOP_NETWORK;
        SendMsg(pMsg);

        pMsg              = new MSG_INTRA;
        pMsg->SenderPid   = GetProcessType(); 
        pMsg->SenderMid   = GetModuleType();
        pMsg->ReceiverPid = PT_CO_BASE + i;
        pMsg->ReceiverMid = MT_COLLECTOR;
        pMsg->CmdCode     = CMD_STOP_NETWORK;
        SendMsg(pMsg);
   }

   m_bNetworkPaused = TRUE;
}

/****************************************************************************************
函数原型	protected: void  ProcDiskSpaceRecovery()
功能描述	处理磁盘空间恢复的通知，请求曾停止的网络模块重新开始收发话单
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
*****************************************************************************************/
void CClusterInterface:: ProcDiskSpaceRecovery()
{
    //没有向网络模块发送过暂停命令
    if (m_bNetworkPaused == FALSE) 
    {
        return;
    }

    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, 
           S_MSG_DISKRECOVERY_MTNETWORK_GOINGON);

    int i = 0;
    for(i = 0; i < m_nAPCount; i++)
    {
        MSG_INTRA *pMsg   = new MSG_INTRA;
        pMsg->SenderPid   = GetProcessType(); 
        pMsg->SenderMid   = GetModuleType();
        pMsg->ReceiverPid = PT_AP_BASE + i;
        pMsg->ReceiverMid = MT_NETWORK;
        pMsg->CmdCode     = CMD_RESUME_NETWORK;
        SendMsg(pMsg);

        pMsg   = new MSG_INTRA;
        pMsg->SenderPid   = GetProcessType(); 
        pMsg->SenderMid   = GetModuleType();
        pMsg->ReceiverPid = PT_CO_BASE + i;
        pMsg->ReceiverMid = MT_COLLECTOR;
        pMsg->CmdCode     = CMD_RESUME_NETWORK;
        SendMsg(pMsg);
    }

    m_bNetworkPaused = FALSE;
}

/***************************************************************************************
函数原型	protected: void  OnTimer(const long nTimerID);
功能描述	定时器处理
输入参数	时钟ID ，只读。
输出参数	无。
返回值  	无。
抛出异常	无。
****************************************************************************************/
void CClusterInterface:: OnTimer(const long nTimerID)
{
    if (nTimerID == m_lTimerID5Sec)
    {
        Timer5Sec();
    }

    CWorkModule::OnTimer(nTimerID);
}

/*****************************************************************************************
函数原型	protected: void  Timer5Sec();
功能描述	5 秒定时器处理
输入参数	无。
输出参数	无。
返回值  	无。
抛出异常	无。
******************************************************************************************/
void CClusterInterface:: Timer5Sec()
{
    MSG_INTRA *pMsg = NULL;

    switch (m_nStatus)
    {
    case CLSTRINT_FAILOVER:
        //向双机代理/管理模块发送倒换申请命令(CMD_ASK_FAILOVER)。
        pMsg = new(FAILOVERCAUSE_HEADLENTGH 
                  + m_pFailOverCause->Length) MSG_INTRA;
        pMsg->SenderPid   = GetProcessType(); 
        pMsg->SenderMid   = GetModuleType();
        pMsg->ReceiverPid = PT_CLSTR;
        pMsg->ReceiverMid = MT_CLSTR;
        pMsg->CmdCode     = CMD_ASK_FAILOVER;
   
        memcpy(pMsg->Data, m_pFailOverCause, pMsg->Len);
        SendMsg(pMsg);
   
        break;

    case CLSTRINT_DEACTIVING:
        //向监控模块发送去激活命令
        DeActive();

        break;
	
    case CLSTRINT_ACTIVING:
        //向监控模块发送激活命令
        Active();

        break;

    case CLSTRINT_DEACTIVE:
        //向监控模块发送退出命令
        Shutdown();

        break;

    default:
	    break;
    }
}
