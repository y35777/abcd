#include "../include/base_type.h"
#include <assert.h>
#include "heartbeat_link.h"
#include "cluster_network.h"


/********************************************************
* 名称: CHeartBeatLink::CHeartBeatLink
* 功能: 构造函数
* 输入: CClusterNetwork* pNetwork    : CClusterNetwork对象的指针
*       int nLinkIndex               : 心跳序列号
*       const char* const szLinkName : 心跳名
* 输出:
* 返回:
********************************************************/
CHeartBeatLink::CHeartBeatLink(CClusterNetwork* pNetwork,
                               int nLinkIndex,
                               const char* const szLinkName)
{
    assert(NULL != pNetwork);

    m_pNetwork           = pNetwork;
    m_bIsAvailable       = TRUE;
    m_nLinkBrokenCounter = 0;
    m_nLinkIndex         = nLinkIndex;
    m_strLinkName        = szLinkName;
    m_nLinkType          = LT_UDP;
}

/********************************************************
* 名称: CHeartBeatLink::~CHeartBeatLink
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CHeartBeatLink::~CHeartBeatLink()
{
    m_pNetwork = NULL;
}

/********************************************************
* 名称: CHeartBeatLink::OnReceive
* 功能: 处理对端发来的消息
* 输入: SClusterFrame* pFrame : 双机消息帧结构的指针
* 输出:
* 返回: void
********************************************************/
void CHeartBeatLink::OnReceive(SClusterFrame* pFrame)
{
    if(pFrame->VerifyCheckSum())
    {
        if (!m_bIsAvailable)
        {
            m_pNetwork->OnNotify(NOTIFY_HEARTLINK_RECOVERY, (UINT4)this);
            m_bIsAvailable = TRUE;
        }

        m_nLinkBrokenCounter = 0;

        if((CMD_CLS_SHAKEHANDS != pFrame->nCmdCode) && (NULL != m_pNetwork))
        {
            m_pNetwork->OnReceive(pFrame);
        }
    }

    delete pFrame;
}

/********************************************************
* 名称: CHeartBeatLink::GetLinkType
* 功能: 获取心跳链路类型
* 输入:
* 输出:
* 返回: LINK_TYPE 返回心跳链路类型
********************************************************/
LINK_TYPE CHeartBeatLink::GetLinkType()
{
    return m_nLinkType;
}

/********************************************************
* 名称: CHeartBeatLink::GetLinkName
* 功能: 获取心跳链路名称
* 输入:
* 输出:
* 返回: const char* 返回心跳链路名称
********************************************************/
const char* CHeartBeatLink::GetLinkName()
{
    return m_strLinkName.c_str();
}

/********************************************************
* 名称: CHeartBeatLink::GetLinkIndex
* 功能: 获取心跳链路序列号
* 输入:
* 输出:
* 返回: int 返回心跳链路序列号
********************************************************/
int CHeartBeatLink::GetLinkIndex()
{
    return m_nLinkIndex;
}

/********************************************************
* 名称: CHeartBeatLink::GetbIsAvailable
* 功能: 获取心跳是否可用
* 输入:
* 输出:
* 返回: BOOL 心跳是否可用
********************************************************/
BOOL CHeartBeatLink::GetbIsAvailable()
{
    return m_bIsAvailable;
}

/********************************************************
* 名称: CHeartBeatLink::Disable
* 功能: 置本心跳对象不可用
* 输入:
* 输出:
* 返回: void
********************************************************/
void CHeartBeatLink::Disable()
{
    m_bIsAvailable = FALSE;
}

/********************************************************
* 名称: CHeartBeatLink::Timer1Sec
* 功能: 秒定时器
* 输入:
* 输出:
* 返回: void
********************************************************/
void CHeartBeatLink::Timer1Sec()
{
    if((m_bIsAvailable) && (++m_nLinkBrokenCounter > LINKBROKEN_TIMEOUT))
    {
        m_bIsAvailable = FALSE;
    }
}
