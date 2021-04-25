#include "../include/base_type.h"
#include "cluster_struct.h"
#include "cluster_network.h"
#include "comm_link.h"


/********************************************************
* 名称: CCommLink::CCommLink
* 功能: 构造函数
* 输入: UINT uPort                   : 串口端口号
*       CClusterNetwork* pNetwork    : CClusterNetwork对象的指针
*       int nLinkIndex               : 心跳序列号
*       const char* const szLinkName : 心跳名
* 输出:
* 返回:
********************************************************/
CCommLink::CCommLink(UINT uPort,
                     CClusterNetwork* pNetwork,
                     int nLinkIndex,
                     const char* const szLinkName):
CHeartBeatLink(pNetwork, nLinkIndex, szLinkName)
{
    m_uCommPort = uPort;
    m_nLinkType = LT_COM;

    if((NULL == szLinkName) || ('\0' == szLinkName[0]))
    {
        char szName[10];
        SNPRINTF(szName, 10, "COM%d", uPort);
        m_strLinkName = szName;
    }
}

/********************************************************
* 名称: CCommLink::~CCommLink
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CCommLink::~CCommLink()
{
    m_CCommPort.Close();
}

/********************************************************
* 名称: CCommLink::Create
* 功能: 创建心跳的COM端口
* 输入: 
* 输出: 
* 返回: int 创建是否成功
********************************************************/
int CCommLink::Create()
{
    return m_CCommPort.Open(m_uCommPort) ? ERR_SUCCESS : ERR_FAIL; 
}

/********************************************************
* 名称: CCommLink::Send
* 功能: 向对端发送消息
* 输入: SClusterFrame* pFrame : 待发送的消息帧
* 输出: 
* 返回: int 发送消息是否成功
********************************************************/
int CCommLink::Send(SClusterFrame* pFrame)
{
	UINT4 uCount;

    //计算frame的长度
	uCount = pFrame->nLen + CLUSTER_FRAME_HEADLENGTH; 

	return m_CCommPort.Write((BYTE *)pFrame, uCount) ? ERR_SUCCESS : ERR_FAIL; 
}

/********************************************************
* 名称: CCommLink::Timer1Sec
* 功能: 秒定时器
* 输入:
* 输出:
* 返回: void
********************************************************/
void CCommLink::Timer1Sec()
{
	UINT4 uLen;
	SClusterFrame* pFrame;
    //从串口中读数据
	while(m_CCommPort.Read((BYTE**)&pFrame, &uLen))
	{
		//读出失败
		if(NULL == pFrame)
		{
			continue;
		}

		//如果数据帧的长度不正确，则删除该桢
		if((pFrame->nLen != (uLen - CLUSTER_FRAME_HEADLENGTH)) || 
            (pFrame->nLen > CLUSTER_FRAME_MAXSIZE))
		{
			delete pFrame;
			continue;
		}

		//有数据则调用OnReceive处理数据
		OnReceive(pFrame);
	}
    
    CHeartBeatLink::Timer1Sec();
}
