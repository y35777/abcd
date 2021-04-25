#include "net_app.h"
#include "protocol_stack.h"
#include "network.h"
#include "resource.h"


/******************************************************
// 函数名:  CNetApp::CNetApp
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    构造函数
// 输入:
//       参数1 :CNetWork* const pNetWork
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CNetApp::CNetApp(CNetWork* const pNetWork)
{
    m_pNetWork = pNetWork;
    m_bActive = FALSE;
    m_nStackStatusInfoType = 0;
    m_pReceiveBillCnt = 0;
}


/******************************************************
// 函数名:  CNetApp::~CNetApp
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
CNetApp::~CNetApp()
{
    m_pNetWork = 0;
    MAP_ID_TO_PS::iterator i;
    for(i = m_StackMan.begin(); i != m_StackMan.end(); i++)
    {
        delete (*i).second;
    }
    m_StackMan.clear();
}


/******************************************************
// 函数名:  CNetApp::Init
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    初始化函数
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CNetApp::Init(void)
{
    //张玉新 2003-05-16 统计平均接收话单张数
    BYTE yAPType = (BYTE)CMsgProcessor::GetProcessType() - PT_AP_BASE + 1;
    m_pReceiveBillCnt = CPerfItem::GetItem(yAPType,
                                           MT_NETWORK,
                                           PERF_RECEIVED_CDR,
                                           PERF_STATS_TOTAL_VALUE);
    return ERR_SUCCESS;
}


/******************************************************
// 函数名:  CNetApp::Active
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    当前存盘模块通知网络模块启动恢复已经完成后，
//          本函数被调用
// 输入:
//       参数1 :void
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CNetApp::Active(void)
{
    m_bActive = TRUE;

    MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, S_NET_NETAPP_ACTIVE);
}


/******************************************************
// 函数名:  CNetApp::DeActive
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    当网络模块在接收辅助升级或去激活命令时，
//          本函数被调用
// 输入:
//       参数1 :void
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CNetApp::DeActive(void)
{
    m_bActive = FALSE;

    MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, S_NET_NETAPP_DEACTIVE);
}


/******************************************************
// 函数名:  CNetApp::LoadStatus
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    从主状态文件中读取所有协议栈的状态信息
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CNetApp::LoadStatus(void)
{
    return ERR_SUCCESS;
}


/******************************************************
// 函数名:  CNetApp::SaveStatus
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    向主状态文件中写入所有协议栈的状态信息
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CNetApp::SaveStatus(void)
{
    return ERR_SUCCESS;
}



/******************************************************
// 函数名:  CNetApp::SendFrame2Ex
// 作者:    Wangfeng
// 时期:    01-11-6
// 描述:    向对应的主机模块或GSN发送协议帧
// 输入:
//       参数1 :MSG_INTRA* pMSG
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-11-6       Wangfeng
******************************************************/
int CNetApp::SendFrame2Ex(MSG_INTRA* pMsg, CProtocolStack* pStack)
{
    if((m_Stack_To_RecvInv.size() < m_pNetWork->GetLicenseCount())
        || (pStack == NULL) //表示为NetApp发的告警信息，必须要发出去
        || (m_Stack_To_RecvInv.find((UINT4)pStack) 
            != m_Stack_To_RecvInv.end()))
    {
        MSG_INTRA* p = new(pMsg->Len) MSG_INTRA;
        memcpy(p, pMsg, pMsg->Len + MSG_INTRA_HEADLENGTH);
        int nRet = m_pNetWork->SendMsgtoHost(pMsg);
        if(nRet == ERR_SUCCESS)
            m_pNetWork->TrackProtocol(p);
        else
            m_pNetWork->TrackProtocol(p, ERR_SOCKET_SEND_MSG);
        delete p;
        return nRet;
    }
    else
    {
        //模块数超过授权数
        m_pNetWork->TrackProtocol(pMsg, ERR_LICENSE_EXPIRED);
    }

    return ERR_SUCCESS;
}


//颜文远2002.04.16添加
/**********************************************************************
功能描述    转发来自告警模块的告警帧
输入参数    pMsg——从告警模块发送过来的告警消息
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CNetApp::DispatchAlarm(MSG_INTRA* pMsg)
{
	//具体的实现必须在子类中实现
	ASSERT(FALSE);
}
//颜文远2002.04.16添加结束


/******************************************************
// 函数名:  CNetApp::OnReceive
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    处理来自网络层的协议帧
// 输入:
//       参数1 :SNetFrame* pFrame
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CNetApp::OnReceive(SNetFrame* pFrame)
{
    //
}



/******************************************************
// 函数名:  CNetApp::Timer1Sec
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    秒事件处理函数
// 输入:
//       参数1 :void
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CNetApp::Timer1Sec(void)
{
    if (m_bActive)
    {
        //调用各CProtocolStack对象的Timer1Sec函数
        MAP_ID_TO_PS::iterator i;
        for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
        {
            (*i).second->Timer1Sec();
        }

        //删除在很长时间内(15天)都未收到主机消息的CProtocolStack对象
        for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
        {
            if((*i).second->GetInertiaPeriod() > 15L * ONE_DAY_IN_SECOND)
            {
                char szMsg[100];
                sprintf(szMsg, 
                        S_NET_REMOVE_GTP_STACK,
                        (*i).second->GetStackId());
                MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, szMsg);
                TRACE(MTS_NETWORK, szMsg);
                CMsgProcessor::WriteLog(MTS_NETWORK, szMsg);

                delete (*i).second;
                m_StackMan.erase(i);
                i = m_StackMan.begin();
            }
        }

        //更新在用协议栈上一次收到协议帧的时间差
        m_Stack_To_RecvInv.clear();
        time_t tNow;
        time(&tNow);
        int nCount = 0;
        for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
        {
            if(tNow >= i->second->GetLastRecvTime() 
               && tNow - i->second->GetLastRecvTime() < 1800 //三十分钟
               && nCount < m_pNetWork->GetLicenseCount())
            {
                m_Stack_To_RecvInv[(UINT4)i->second] = tNow - i->second->GetLastRecvTime();
                nCount++;
            }
        }
    }
}


/******************************************************
// 函数名:  CNetApp::GetNetWork
// 作者:    Wangfeng
// 时期:    01-10-26
// 描述:    返回CNetWork*对象的指针
// 输入:
// 返回值: CNetWork* 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-26      Wangfeng
******************************************************/
CNetWork* CNetApp::GetNetWork()
{
    return m_pNetWork;
}

/******************************************************
// 函数名:  CNetApp::GetReadyDataSize
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    返回所有协议栈中当前已接收的且可以提交给前存
//          盘模块保存的话单数据的字节数
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CNetApp::GetReadyDataSize(void)
{
    int nSizeTotal = 0;
    //对m_StackMan中的元素进行遍历，调用每个CProtocolStack
    //对象的GetReadyDataSize函数
    MAP_ID_TO_PS::iterator i;
    for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
    {
        nSizeTotal += (*i).second->GetReadyDataSize();
    }
    return nSizeTotal; 
}


/******************************************************
// 函数名:  CNetApp::GetReadyRecordCount
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    返回所有协议栈中当前已接收的且可以提交给前存
//          盘模块保存的话单数据的记录数
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CNetApp::GetReadyRecordCount(void)
{
    int nCountTotal = 0;
    //对m_StackMan中的元素进行遍历，调用每个CProtocolStack
    //对象的GetReadyRecordCount函数
    MAP_ID_TO_PS::iterator i;
    for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
    {
        nCountTotal += (*i).second->GetReadyRecordCount();
    }
    return nCountTotal;
}



/******************************************************
// 函数名:  CNetApp::GetReadyData
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    将所有协议栈可提交给前存盘模块保存的话单数据
//          放入*pDataBuf对象中
// 输入:
//       参数1 :CDataBuffer* const pDataBuf
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CNetApp::GetReadyData(CDataBuffer* const pDataBuf)
{
    //对m_StackMan中的元素进行遍历，调用每个CProtocolStack
    //对象的GetReadyData函数
    MAP_ID_TO_PS::iterator i;
    for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
    {
        (*i).second->GetReadyData(pDataBuf);
    }
}



/******************************************************
// 函数名:  CNetApp::OnResponse
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    对上次提供给前存盘模块保存的话单数据进行后续的处理
// 输入:
//       参数1 :bool const bSaved
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CNetApp::OnResponse(BOOL const bSaved)
{
    //对m_StackMan中的元素进行遍历，调用每个CProtocolStack
    //对象的OnResponse函数
    MAP_ID_TO_PS::iterator i;
    for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
    {
        (*i).second->OnResponse(bSaved);
    }
}

