#include "../include/toolbox.h"
#include "../include/msg_def.h"
#include "../include/perf_item.h"
#include "net_status.h"
#include "data_buffer.h"
#include "network.h"
#include "gtp_related.h"
#include "slide_wnd_related.h"	//颜文远2002.04.22添加
#include "scan_app.h"
#include "wlan_as_app.h"


//工作目录
char CNetWork::m_szWorkDir[MAX_PATH] = "";

/******************************************************
// 函数名:  CNetWork::CNetWork
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    构造函数
// 输入:
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CNetWork::CNetWork()
{
    m_pNetApp                   = NULL;
    m_Timer1SecId               = 0;
    m_nLowAtTime                = 0;
    m_TimerSendPacketId         = 0;
    m_bSendOnResponse           = FALSE; 
    m_bWaitForResponse          = FALSE;
    m_bMustMerge                = FALSE;
    m_uMaxPackageSize           = 0;
    m_uSendEmptyPackageCount    = 0;
    m_uPacketId                 = 0;
    m_pNetWorkStatusInfo        = new CNetWorkStatusInfo(
                                        sizeof(SNetWorkStatusInfo));
    m_pPtclTracker              = 0;
    m_uStatus                   = NWSTATUS_INIT;
    m_szStatusFileName[0]       = '\0';
    m_uLowAtSize                = 1024 * CFG_AP_KEY_LOW_AT_SIZE_DEFAULT;
    m_szStackType[0]            = '\0';
}


/******************************************************
// 函数名:  CNetWork::~CNetWork
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    析构函数
// 输入:
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CNetWork::~CNetWork()
{
    if (m_pNetWorkStatusInfo)
    {
        delete m_pNetWorkStatusInfo;
        m_pNetWorkStatusInfo = 0;
    }
}



/******************************************************
// 函数名:  CNetWork::InitInstance
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    初始化实例函数
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CNetWork::InitInstance(void)
{
    int nRet;
    nRet = CWorkModule::InitInstance();
    if (nRet != ERR_SUCCESS) return nRet;

    //打开配置文件
    CINIFile *pIniFile;
    pIniFile = new CINIFile((char *)GetCfgFilePath());
    pIniFile->Open();

    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP, 
            GetProcessType() - PT_AP_BASE + 1);

    //如果MustMerge=1，则赋TRUE给m_bMustMerge，否则FALSE
    int nMustMerge = pIniFile->GetInt(strSec,
                                      CFG_AP_KEY_MUST_MERGE,
                                      CFG_AP_KEY_MUST_MERGE_DEFAULT);
    if(nMustMerge) m_bMustMerge = TRUE;

    //读取最大话单包长度赋给成员变量m_nMaxPackageSize
    m_uMaxPackageSize = pIniFile->GetInt(strSec,
                                  CFG_AP_KEY_MAX_PACKAGE_SIZE,
                                  CFG_AP_KEY_MAX_PACKAGE_SIZE_DEFAULT);
    m_uMaxPackageSize *= 1024;

    //读出低潮时间
    m_nLowAtTime = pIniFile->GetInt(strSec,
                                    CFG_AP_KEY_LOW_AT_TIME,
                                    CFG_AP_KEY_LOW_AT_TIME_DEFAULT);
    //获得低潮包大小
    m_uLowAtSize = pIniFile->GetInt(strSec,
                                    CFG_AP_KEY_LOW_AT_SIZE,
                                    CFG_AP_KEY_LOW_AT_SIZE_DEFAULT);
    m_uLowAtSize *= 1024;

    //获得每秒钟需要发送的最大空包数
    m_uSendEmptyPackageMaxPerSec = pIniFile->GetInt(strSec,
                                    CFG_AP_KEY_MAX_EMPTY_PACKAGE,
                                    CFG_AP_KEY_MAX_EMPTY_PACKAGE_DEFAULT);

    //从配置文件中相应接入点中读出网络模块状态文件的工作目录，
    //赋给m_szWorkDir
    pIniFile->GetString(CFG_SEC_DISK_FILE,
                        CFG_MAIN_STATUS_ROOT_DIR,
                        CFG_MAIN_STATUS_ROOT_DIR_DEFAULT,
                        (char*)m_szWorkDir,
                        MAX_PATH);
    char szAPName[255];
    pIniFile->GetString(strSec, CFG_AP_NAME, 
                        strSec, szAPName, 255);

    strcat((char*)m_szWorkDir, ACE_DIRECTORY_SEPARATOR_STR);
    strcat((char*)m_szWorkDir, szAPName);
    strcat((char*)m_szWorkDir, ACE_DIRECTORY_SEPARATOR_STR);
    strcat((char*)m_szWorkDir, "network");
    strcat((char*)m_szWorkDir, ACE_DIRECTORY_SEPARATOR_STR);

    //创建工作目录
    if(!CreateDeepDir(m_szWorkDir))
    {
        TRACE(THISMODULE, S_NET_CREATE_NET_STATUS_DIR, 
              m_szWorkDir, ACE_OS::last_error());
        pIniFile->Close();
        delete pIniFile;
        return ERR_FAIL;
    }

    //赋状态文件的全路径名
    sprintf(m_szStatusFileName, "%s%s", m_szWorkDir, "status.dat");

    //通过当前接入点号从配置文件中获得当前接入点类型；
    int nAPType = pIniFile->GetInt(strSec,
                                   CFG_AP_KEY_APTYPE,
                                   CFG_AP_KEY_APTYPE_DEFAULT);

    //通过当前接入点号从配置文件中获得当前接入点类型；
    int nAddModuleNum = pIniFile->GetInt(strSec,
                                         CFG_AP_KEY_ADD_MOD_NUM,
                                         CFG_AP_KEY_ADD_MOD_NUM_DEFAULT);

    //通过当前接入点号从配置文件中获得是否是文件扫描协议
    pIniFile->GetString(strSec,
                        CFG_AP_KEY_ADDITION_STACK,
                        CFG_AP_KEY_ADDITION_STACK_DEFAULT,
                        m_szStackType,
                        sizeof(m_szStackType));

    pIniFile->Close();
    delete pIniFile;
    
    BYTE yAPType = (BYTE)GetProcessType() - PT_AP_BASE + 1;
    if(ACE_OS::strcasecmp(m_szStackType, "ScanStack") == 0)
    {
        m_pNetApp = new CScanApp(this);
    }
    else if(ACE_OS::strcasecmp(m_szStackType, "WlanAsStack") == 0)
    {
        m_pNetApp = new CWlanAsApp(this);
    }
    else
    {
        switch(nAPType)
        {
		case AP_WLAN:
        case AP_PS:
        case AP_PS_RESERVED:
            m_pNetApp = new CGtpNetApp(this);
            m_pPtclTracker = new CGtpTrack();
            break;

		//颜文远2002.04.22修改
        case AP_128:
            if(nAddModuleNum)
            {
	            m_pNetApp = new C128NetApp(this);
            }
            else
            {
	            m_pNetApp = new CSlideWndNetApp(this);
            }
            m_pPtclTracker = new CAppFrmTrack();
            break;

        case AP_STP:
	        m_pNetApp = new CSlideWndNetApp(this);
            m_pPtclTracker = new CAppFrmTrack();
            break;

        case AP_GSM:
			m_pNetApp = new CGSMNetApp(this);
            m_pPtclTracker = new CAppFrmTrack();
			break;

		case AP_SOFTX:
			m_pNetApp = new CSoftXNetApp(this);
            m_pPtclTracker = new CSysFrmTrack();
			break;

		//颜文远2002.04.22修改结束

        default:
            TRACE(THISMODULE, S_NET_AP_TYPE_UNKNOWN, nAPType);
            break;
        }
    }

    //初始化网络应用层
    if((!m_pNetApp) || (m_pNetApp->Init() != ERR_SUCCESS))
    {
        TRACE(THISMODULE, S_NET_INIT_NETAPP);
        return ERR_FAIL;
    }
    
    return nRet; 
}



/******************************************************
// 函数名:  CNetWork::GetPacketId
// 作者:    Wangfeng
// 时期:    01-10-24
// 描述:    获取当前包号
// 输入:
// 返回值: UINT4 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
UINT4 CNetWork::GetPacketId()
{
    return m_uPacketId;
}


/******************************************************
// 函数名:  CNetWork::IsWaitForResponse
// 作者:    Wangfeng
// 时期:    01-10-26
// 描述:    网络模块是否正在等待前存盘的应答
// 输入:
// 返回值: BOOL 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-26      Wangfeng
******************************************************/
BOOL CNetWork::IsWaitForResponse()
{
    return m_bWaitForResponse;
}


/******************************************************
// 函数名:  CNetWork::ExitInstance
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    退出时的清理工作
// 输入:
//       参数1 :void
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CNetWork::ExitInstance(void)
{
    if(m_pNetApp != NULL)
    {
        m_pNetApp->DeActive();
        delete m_pNetApp;
        m_pNetApp = NULL;
    }

    if(m_pPtclTracker != NULL)
    {
        delete m_pPtclTracker;
        m_pPtclTracker = NULL;
    }

    KillTimer(m_Timer1SecId);
    KillTimer(m_TimerSendPacketId);

    CWorkModule::ExitInstance();
}


/******************************************************
// 函数名:  CNetWork::GetModuleType
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    返回模块号信息
// 输入:
// 返回值: MODULE_TYPE 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
MODULE_TYPE CNetWork::GetModuleType()
{
    return MT_NETWORK;
}


/******************************************************
// 函数名:  CNetWork::ParseMsg
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    消息处理
// 输入:
//       参数1 :MSG_INTRA* pMsgParsed
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CNetWork::ParseMsg(MSG_INTRA* pMsgParsed)
{
    MSG_INTRA *pMsg;
    int nRet = 0;
    switch(pMsgParsed->CmdCode)
    {
    case CMD_MODULE_ACTIVE:
        //收到管理模块发送的激活本模块的命令，向前存盘模块发
        //请求启动恢复的消息
        pMsg = new MSG_INTRA;
        pMsg->SenderPid     = PT_SELF; 
        pMsg->SenderMid     = MT_NETWORK;
        pMsg->ReceiverPid   = PT_SELF;
        pMsg->ReceiverMid   = MT_FRONTSAVE;
        pMsg->CmdCode       = CMD_RESUME;
        SendMsg(pMsg);

        //初始化状态文件,如果存在则装入,不存在则创建新文件
        nRet = InitStatus();
        if (ERR_SUCCESS != nRet)
        {
            TRACE(THISMODULE, S_NET_INIT_STATUS_FILE, nRet);
            SendQuitToMe();
        }
        break;

    case CMD_RESUME_OVER:
        //若为启动恢复完成消息，取出消息中前存盘返回的当前包ID，
        //激活m_pNetApp对象，并变迁状态为“激活状态”
        if(NWSTATUS_ACTIVE == m_uStatus) break;

        //调整包号
        if(AdjustStatus(*(UINT4 *)pMsgParsed->Data) != ERR_SUCCESS)
        {
            TRACE(THISMODULE, S_NET_FAIL_TO_RECOVERY);
            SendQuitToMe();
            m_uStatus = NWSTATUS_DEACTIVE;
            break;
        }

        //设置低潮定时器
        m_TimerSendPacketId = SetTimer(m_nLowAtTime);

        //设置秒事件定时器
        m_Timer1SecId = SetTimer(1000);

        //激活网络应用层
        m_pNetApp->Active();
        m_uStatus = NWSTATUS_ACTIVE;
        break;

    case CMD_HOST_DATA:
        //若为主机协议消息，调用m_pNetApp的OnReceive（协议数据
        //帧处理入口）
        m_pNetApp->OnReceive((SNetFrame*)pMsgParsed);
        break;

    case CMD_SAVE_SUCCEED:
        //若为前存盘应答消息，调用m_pNetApp的OnResponse，然后调用
        //SaveStatus更新状态文件
        ASSERT(*(UINT4 *)pMsgParsed->Data == m_uPacketId);
        
        //应答
        OnResponse(TRUE);

        //如果是辅助升级状态，且一秒内发空包数小于20，则向前存盘发空包
        switch(m_uStatus)
        {
        case NWSTATUS_AUXUPGRADE:
            if (m_uSendEmptyPackageCount < m_uSendEmptyPackageMaxPerSec)
            {
                SendEmptyPackage();
                m_uSendEmptyPackageCount++;
            }
            break;

        case NWSTATUS_ACTIVE:
            if(m_bSendOnResponse)
            {
                if(m_pNetApp->GetReadyDataSize() > 0) 
                {
                    SendBillPackage();
                }
                m_bSendOnResponse = FALSE;
            }
            else if(m_pNetApp->GetReadyDataSize() >= m_uLowAtSize)
            {
                SendBillPackage();
            }
            break;
        //by ldf 2002.4.8 对应问题单 D12457
        default:
            {
                break;
            }
        }
        break;
        //修改完毕

    case CMD_SYNC_PACKETID:
        //若为“请求同步原始话单包”，用消息中的PacketID更改m_PacketId
        //消息中的PacketID为前存盘已成功保存的包号
        //注意:消息中的PacketID为前存盘当前已成功保存的包号
        if (m_uPacketId != *(UINT4 *)pMsgParsed->Data)
        {
            BOOL bExit = FALSE;
            if (m_uPacketId > *(UINT4 *)pMsgParsed->Data)
            {
                bExit = TRUE;
            }

            m_pNetApp->OnResponse(FALSE);
            m_bWaitForResponse = FALSE;
            m_uPacketId = *(UINT4 *)pMsgParsed->Data;
            SaveStatus();
            SyncStatus();

            if (bExit)
            {
                m_pNetApp->DeActive();
                TRACE(THISMODULE, S_NET_FAIL_TO_RECOVERY);
                SendQuitToMe();
                m_uStatus = NWSTATUS_DEACTIVE;
            }
        }
        break;

    case CMD_BIN_ALARM:
		//颜文远2002.04.16添加
        //若为告警模块的告警转发消息，调用m_NetApp对象的DispatchAlarm接口
        m_pNetApp->DispatchAlarm(pMsgParsed);
        break;
		//颜文远2002.04.16添加结束

    case CMD_AUXUPGRADE:
        //若为辅助升级消息，去激活m_pNetApp对象，变迁到“辅助升
        //级状态”，并发送“通知已经开始发送空包”给管理模块；
        if (NWSTATUS_ACTIVE == m_uStatus)
        {
            m_uStatus = NWSTATUS_AUXUPGRADE;
            m_pNetApp->DeActive();
            pMsg = new MSG_INTRA;
            pMsg->SenderPid     = PT_SELF; 
            pMsg->SenderMid     = MT_NETWORK;
            pMsg->ReceiverPid   = PT_SELF;
            pMsg->ReceiverMid   = MT_MANAGER;
            pMsg->CmdCode       = CMD_SENDEMPTYPACKET;
            SendMsg(pMsg);
        }
        else
        {
            TRACE(THISMODULE, S_NET_IGNORE_AUXUPGRADE, m_uStatus);
            MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, 
                   S_NET_IGNORE_AUXUPGRADE, m_uStatus);

            //by ldf 2002.4.8 对应问题单D12433 D12459
            pMsg = new MSG_INTRA;
            pMsg->SenderPid     = GetProcessType(); 
            pMsg->SenderMid     = GetModuleType();
            pMsg->ReceiverPid   = PT_KERNEL;
            pMsg->ReceiverMid   = MT_MONITOR;
            pMsg->CmdCode       = CMD_AUXUPGRADE_FAIL;
            SendMsg(pMsg);
            //修改完毕
        }
        break;

    case CMD_STOP_NETWORK:
        //若为“磁盘空间已满”，则去激活网络应用层对象
        m_pNetApp->DeActive();
        m_uStatus = NWSTATUS_DEACTIVE;
        break;

    case CMD_RESUME_NETWORK:
        //若为“磁盘空间恢复”，则激活网络应用层对象
        m_pNetApp->Active();
        m_uStatus = NWSTATUS_ACTIVE;
        break;

    default:
        break;
    }

    CWorkModule::ParseMsg(pMsgParsed);
}



/******************************************************
// 函数名:  CNetWork::OnTimer
// 作者:    Wangfeng
// 时期:    01-10-24
// 描述:    定时器函数
// 输入:
//       参数1 :const long nTimerID
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
void CNetWork::OnTimer(const long nTimerID)
{
    if(nTimerID == m_Timer1SecId)
    {
        Timer1Sec();
    }
    else if(nTimerID == m_TimerSendPacketId)
    {
        LowAtTimer();
    }

    CWorkModule::OnTimer(nTimerID);
}




/******************************************************
// 函数名:  CNetWork::OnResponse
// 作者:    Wangfeng
// 时期:    01-10-29
// 描述:    对上次提供给前存盘模块保存的话单数据进行后续的处理
// 输入:
//       参数1 :BOOL const bSaved
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-29      Wangfeng
******************************************************/
void CNetWork::OnResponse(BOOL const bSaved)
{
    //向网络层应答
    m_pNetApp->OnResponse(bSaved);
    m_bWaitForResponse = FALSE;
    SaveStatus();
    SyncStatus();
}


/******************************************************
// 函数名:  CNetWork::Timer1Sec
// 作者:    Wangfeng
// 时期:    01-10-24
// 描述:    1秒定时器函数
// 输入:
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
void CNetWork::Timer1Sec()
{
    switch(m_uStatus)
    {
    case NWSTATUS_AUXUPGRADE:
        if (FALSE == m_bWaitForResponse)
        {
            SendEmptyPackage();
        }
        m_uSendEmptyPackageCount = 0;
        break;

    default:
        break;
    }
    m_pNetApp->Timer1Sec();
}



/******************************************************
// 函数名:  CNetWork::LowAtTimer
// 作者:    Wangfeng
// 时期:    01-10-24
// 描述:    定时打包事件处理
// 输入:
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
void CNetWork::LowAtTimer()
{
    switch(m_uStatus)
    {
    case NWSTATUS_ACTIVE:
        if (m_bWaitForResponse)
        {
            m_bSendOnResponse = TRUE;
        }
        else 
        {//如果已准备好的话单尺寸大于0，则向前存发送话单包
            if(m_pNetApp->GetReadyDataSize() > 0) 
            {
                SendBillPackage();
            }
            else if(m_bMustMerge)
            {
                //如果要进行合并，则要发空包
                SendEmptyPackage();
            }
        }
        break;

    default:
        break;
    }
}



/******************************************************
// 函数名:  CNetWork::AdjustStatus
// 作者:    Wangfeng
// 时期:    01-10-24
// 描述:    进行启动恢复处理
// 输入:
//       参数1 :UINT4 uPacketSaved--由前存盘模块发来的已
//                                  成功存盘的包号
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
int CNetWork::AdjustStatus(UINT4 uPacketSaved)
{
    int nRet = ERR_SUCCESS;

    if (m_bWaitForResponse)
    {
        //若处于等待应答状态且包号相等，则表明最后一包已成功保
        //存，但应答丢失，应补发应答
        if (m_uPacketId == uPacketSaved)  
        {
            OnResponse(TRUE);
        }
        //若处于等待应答状态且包号不等，则表明最后一包话单未到达
        //存盘模块或到达之后未成功保存，此时应修正自身的包号
        else if (m_uPacketId == uPacketSaved + 1)
        {
            m_pNetApp->OnResponse(FALSE);
            m_uPacketId = uPacketSaved;
            m_bWaitForResponse = FALSE;
            SaveStatus();
            SyncStatus();
        }
        else
        {
            //网络模块与前存盘模块的包号不正常
            TRACE(THISMODULE, 
                  S_NET_ADJUST_PACKAGE_ID,
                  m_bWaitForResponse,
                  m_uPacketId, uPacketSaved);
            m_pNetApp->OnResponse(FALSE);
            if (m_uPacketId > uPacketSaved)
            //如果当前状态文件中的包号大于前存盘的包号,这冲情况下,有可能
            //存在一些序列号文件其包号介于m_uPacketId与uPacketSaved之间,
            //这里通过重启来删除这些序列号文件.
            {
                nRet = ERR_FAIL;
            }
            m_uPacketId = uPacketSaved;
            m_bWaitForResponse = FALSE;
            SaveStatus();
            SyncStatus();
        }
    }
    else
    {
        //网络模块与前存盘模块的包号不正常
        if(uPacketSaved != m_uPacketId) 
        {
            TRACE(THISMODULE, 
                  S_NET_ADJUST_PACKAGE_ID,
                  m_bWaitForResponse,
                  m_uPacketId, uPacketSaved);

            if (m_uPacketId > uPacketSaved)
            {
                nRet = ERR_FAIL;
            }
            m_uPacketId = uPacketSaved;
            SaveStatus();
            SyncStatus();
        }
    }
    return nRet;
}



/******************************************************
// 函数名:  CNetWork::SendMsgtoHost
// 作者:    Wangfeng
// 时期:    01-10-30
// 描述:    发送消息到主机
// 输入:
//       参数1 :MSG_INTRA* pMsg
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-30      Wangfeng
******************************************************/
int CNetWork::SendMsgtoHost(MSG_INTRA* pMsg)
{
    return SendMsg(pMsg);
}



/******************************************************
// 函数名:  CNetWork::SendBillPackage
// 作者:    Wangfeng
// 时期:    01-10-24
// 描述:    发送话单数据包
// 输入:
//       参数1 :BOOL bGetNewData--是否取新的话单包数据
// 返回值: BOOL -- 是否发出话单包
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
BOOL CNetWork::SendBillPackage(BOOL bGetNewData)
{

    //取已准备好的话单尺寸
    static UINT4 uLen = 0;
    static char* pPacket = 0;
    if(bGetNewData)
    {
        CDataBuffer DBuf;

        //取话单长度
        int nReadyDataSize = m_pNetApp->GetReadyDataSize();
        if(0 == nReadyDataSize) return FALSE;
        if(nReadyDataSize > m_uMaxPackageSize) 
        {
            nReadyDataSize = m_uMaxPackageSize;
        }

        //取得话单数据
        DBuf.Alloc(nReadyDataSize);
        m_pNetApp->GetReadyData(&DBuf);
        DBuf.Get(pPacket, uLen);
    }
    //检查当前话单包是否有效
    if ((NULL == pPacket) || (0 == uLen)) return FALSE;
    
    //向前存盘发送当前话单包
    SFrontPacketInfo BillPackage;
    BillPackage.uPID = ++m_uPacketId;
    BillPackage.pPacket = (BYTE*)pPacket;
    BillPackage.uPacketLen = uLen;

    MSG_INTRA *pMsg = new(sizeof(SFrontPacketInfo)) MSG_INTRA;
    pMsg->SenderPid = PT_SELF; 
    pMsg->SenderMid = MT_NETWORK;
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = MT_FRONTSAVE;
    pMsg->CmdCode = CMD_SAVE_PACKET;
    memcpy(pMsg->Data, &BillPackage, sizeof(SFrontPacketInfo));
    SendMsg(pMsg);

    //更新状态
    m_bWaitForResponse = TRUE;
    SaveStatus();
    SyncStatus();

    return TRUE;
}



/******************************************************
// 函数名:  CNetWork::SendEmptyPackage
// 作者:    Wangfeng
// 时期:    01-10-24
// 描述:    向前存盘发送空包
// 输入:
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
void CNetWork::SendEmptyPackage()
{
    //向前存盘发送空包
    SFrontPacketInfo BillPackage;
    BillPackage.uPID = ++m_uPacketId;
    BillPackage.pPacket = 0;
    BillPackage.uPacketLen = 0;

    MSG_INTRA *pMsg = new(sizeof(SFrontPacketInfo)) MSG_INTRA;
    pMsg->SenderPid = PT_SELF; 
    pMsg->SenderMid = MT_NETWORK;
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = MT_FRONTSAVE;
    pMsg->CmdCode = CMD_SAVE_PACKET;
    memcpy(pMsg->Data, &BillPackage, sizeof(SFrontPacketInfo));
    SendMsg(pMsg);

    //更新状态
    m_bWaitForResponse = TRUE;
    SaveStatus();
    SyncStatus();
}



/******************************************************
// 函数名:  CNetWork::SyncStatus
// 作者:    Wangfeng
// 时期:    01-10-29
// 描述:    发送状态数据
// 输入:
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-29      Wangfeng
******************************************************/
void CNetWork::SyncStatus()
{
    int nBufLen = CNetStatusInfo::GetAllStatusDataLen();

    BYTE *pBuffer = new BYTE[nBufLen];

    if(CNetStatusInfo::GetAllStatusData(pBuffer, nBufLen))
    {
        WriteSyncData(GetModuleType(), pBuffer, nBufLen);
    }

    delete [] pBuffer;
}



/******************************************************
// 函数名:  CNetWork::SaveStatus
// 作者:    Wangfeng
// 时期:    01-10-24
// 描述:    保存状态
// 输入:
// 返回值: void 
// 注释:    注意：绝对不能在已提交包未应答的情况下进行
//          连续保存状态，这样会造成同一已提交包被同时
//          保存在至少两个序列号列表文件中。
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
int CNetWork::SaveStatus()
{
    SNetWorkStatusInfo StatusInfo;
    StatusInfo.uPacketId = m_uPacketId;
    StatusInfo.bWaitForResponse = m_bWaitForResponse;
    m_pNetWorkStatusInfo->Set((BYTE*)&StatusInfo, 
                              sizeof(SNetWorkStatusInfo));

    if((m_pNetApp->SaveStatus() != 0) || (CNetStatusInfo::FlushAll() != 0))
    {
        m_pNetApp->DeActive();
        SendQuitToMe();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}



/******************************************************
// 函数名:  CNetWork::LoadStatus
// 作者:    Wangfeng
// 时期:    01-10-24
// 描述:    读取状态
// 输入:
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
int CNetWork::LoadStatus()
{
    //读主状态文件的头部
    SNetWorkStatusInfo StatusInfo;
    UINT4 uInputLen = m_pNetWorkStatusInfo->Get((BYTE*)&StatusInfo, 
                                        sizeof(SNetWorkStatusInfo));
    if(uInputLen != sizeof(SNetWorkStatusInfo))
    {
        return ERR_FAIL;
    }

    m_uPacketId = StatusInfo.uPacketId;
    m_bWaitForResponse = StatusInfo.bWaitForResponse;

    //读主状态文件的协议部分
    return m_pNetApp->LoadStatus();
}


/******************************************************
// 函数名: 	CNetWork::InitStatus
// 作者:    Wangfeng
// 时期:    02-1-29
// 描述:    在初始化完成后，第一次读取状态
// 输入:
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//02-1-29       Wangfeng
******************************************************/
int CNetWork::InitStatus()
{
    //以读/写方式打开状态文件，如果成功，则装放状态文件
    //如果失败，则重新创建一个状态，并打开它
    int nRet = CNetStatusInfo::Init(m_szStatusFileName);
    if(ERR_SUCCESS == nRet)
    {
        ACE_stat st;
        nRet = ACE_OS::stat(m_szStatusFileName, &st);
        if((0 == nRet) && (st.st_size > 0))
        {
            //装入状态文件
            return LoadStatus();
        }
    }
    else //文件不存在
    {
        //创建一个新的状态
        FILE* file = fopen(m_szStatusFileName, "wb");
        if(!file)
        {
            TRACE(THISMODULE, S_NET_CRT_NET_STATUS_FILE, 
                  errno, strerror(errno));
            return ERR_FAIL;
        }
        fclose(file);
        nRet = CNetStatusInfo::Init(m_szStatusFileName);
    }

    return ERR_SUCCESS;
}

void CNetWork::TrackProtocol(MSG_INTRA* pMsg, int nProcRst)
{
    if(CMsgProcessor::g_bTrackProtocol)
    {
        CMsgProcessor::TrackProtocol(m_pPtclTracker->Analysis(pMsg, nProcRst));
    }
}
