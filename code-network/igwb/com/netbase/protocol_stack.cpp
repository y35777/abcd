#include "../include/toolbox.h"
#include "net_status.h"
#include "protocol_stack.h"


/******************************************************
// 函数名:  CProtocolStack::CProtocolStack
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    构造函数
// 输入:
//       参数1 :CNetApp* const pNetApp
//       参数2 :UINT4 const uStackId
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CProtocolStack::CProtocolStack(CNetApp* const pNetApp, 
                               UINT4 const uStackId)
{
    ASSERT(pNetApp != 0);
    m_pNetApp        = pNetApp;

    m_uStackId       = uStackId;
    m_uInertiaPeriod = 0;
    m_pStatusInfo    = 0;
    m_szName[0]      = '\0';
    m_lLastRecvTime  = 0;
}


/******************************************************
// 函数名:  CProtocolStack::~CProtocolStack
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    析构函数
// 输入:
//       参数1 :void
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CProtocolStack::~CProtocolStack(void)
{
    //释放m_pStatusInfo指向的对象
    if(m_pStatusInfo != 0)
    {
        delete m_pStatusInfo;
        m_pStatusInfo = 0;
    }

    m_pNetApp = 0;
}


/******************************************************
// 函数名:  CProtocolStack::GetStackId
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    获得本协议栈的编号
// 输入:
//       参数1 :void
// 返回值: UINT4 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
UINT4 CProtocolStack::GetStackId(void)
{
    return m_uStackId;
}


/******************************************************
// 函数名:  CProtocolStack::GetReadyDataSize
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    返回本对象中当前已接收的且可以提交给前存盘
//          模块保存的话单数据的字节数
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CProtocolStack::GetReadyDataSize(void)
{
    return 0;
}



/******************************************************
// 函数名:  CProtocolStack::GetReadyRecordCount
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    返回本对象中当前已接收的且可以提交给前存盘
//          模块保存的话单数据的记录数
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CProtocolStack::GetReadyRecordCount(void)
{
    return 0;
}


/******************************************************
// 函数名:  CProtocolStack::GetReadyData
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    将本协议栈可提交给前存盘模块保存的话单数据
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
void CProtocolStack::GetReadyData(CDataBuffer* const pDataBuf)
{
    //空
}


/******************************************************
// 函数名:  GetInertiaPeriod
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    获得从最近一次接收到数据开始到现在所经历的时
//          间，单位为秒
// 输入:
// 返回值: UINT4 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
UINT4 CProtocolStack::GetInertiaPeriod()
{
    return m_uInertiaPeriod;
}


/******************************************************
// 函数名:  CProtocolStack::OnReceive
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    处理从与本协议栈对应的主机模块或GSN发送过来的
//          一帧数据
// 输入:
//       参数1 :void* pFrame
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CProtocolStack::OnReceive(void* pFrame)
{
    m_uInertiaPeriod = 0;
    time(&m_lLastRecvTime);
}



/******************************************************
// 函数名:  CProtocolStack::OnResponse
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    对上次提供给前存盘模块保存的话单数据进行后续的处理
// 输入:
//       参数1 :bool const bSaved——表示上次提交给前存
//              盘模块的话单数据是否被成功保存 
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CProtocolStack::OnResponse(BOOL const bSaved)
{
    //空
}


/******************************************************
// 函数名:  CProtocolStack::LoadStatus
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    从主状态文件中读取本协议栈的状态信息
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CProtocolStack::LoadStatus(void)
{
    return ERR_SUCCESS;
}


/******************************************************
// 函数名:  CProtocolStack::SaveStatus
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    向主状态文件中写入本协议栈的状态信息
// 输入:
//       参数1 :void
// 返回值: int
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CProtocolStack::SaveStatus(void)
{
    return ERR_SUCCESS;
}


/******************************************************
// 函数名:  CProtocolStack::Timer1Sec
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
void CProtocolStack::Timer1Sec(void)
{
    m_uInertiaPeriod++;
}


