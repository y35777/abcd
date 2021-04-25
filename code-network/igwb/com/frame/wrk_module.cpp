/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:wrk_module.cpp
    描述信息:
    
    包含CWorkModule类的实现,该类用于在整个进程中交换消息.
    CMsgProcessor类包含该类的对象,以支持消息交换.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-16,创建.
*/

#include "wrk_module.h"

CWorkModule::CWorkModule()
{
	m_nLicenseCount = 0;
}

CWorkModule::~CWorkModule()
{
}

/*
    函数功能:模块退出时的清除函数.
    若一个业务模块退出,则通知CModuleManager,终止所有模块.

    修改记录:
    周拥辉,2001-10-16,创建.
*/
void CWorkModule::ExitInstance()
{
    /* 向本进程的管理模块发退出消息 */

    MSG_INTRA *pMsg = new MSG_INTRA;
    pMsg->SenderPid = PT_SELF;
    pMsg->SenderMid = GetModuleType();
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = MT_MANAGER;
    pMsg->CmdCode = CMD_QUIT;
    SendMsg(pMsg);

    CMsgProcessor::ExitInstance();
}

int CWorkModule::InitInstance()
{
	CMsgProcessor::InitInstance();

	m_nLicenseCount = 0;

	//创建一个5秒的定时器
	m_nTimerIDReqLicense = CWorkModule::SetTimer(5 * 1000);

    RequestLicense();

	return 0;
}

void CWorkModule::ParseMsg(MSG_INTRA* pMsg)
{
   	switch (pMsg->CmdCode)
	{
		//检查是否是License请求响应帧
	    case CMD_LICENSE_RESPONSE:
		{
			SLicenseResp* pLicResp = (SLicenseResp *)pMsg->Data;
			m_nLicenseCount = pLicResp->nLicenseCount;
			break;
		}

		default:
		{
			CMsgProcessor::ParseMsg(pMsg);
			break;
		}
	}
}

void CWorkModule::RequestLicense()
{
    MSG_INTRA *pMsg	= new MSG_INTRA;
    pMsg->SenderPid = GetProcessType();
    pMsg->SenderMid = GetModuleType();
    pMsg->ReceiverPid = PT_KERNEL;
    pMsg->ReceiverMid = MT_LICENSE_MANAGER;
    pMsg->CmdCode	  = CMD_LICENSE_REQUEST;
    SendMsg(pMsg);
}

void CWorkModule::OnTimer(const long nTimerID)
{
	CMsgProcessor::OnTimer(nTimerID);

	//每30秒发送一次License请求
	if (nTimerID == m_nTimerIDReqLicense)
	{
		RequestLicense();
	}
}

int CWorkModule::GetLicenseCount()
{
	return m_nLicenseCount;
}
