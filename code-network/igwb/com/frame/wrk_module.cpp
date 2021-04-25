/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:wrk_module.cpp
    ������Ϣ:
    
    ����CWorkModule���ʵ��,�������������������н�����Ϣ.
    CMsgProcessor���������Ķ���,��֧����Ϣ����.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
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
    ��������:ģ���˳�ʱ���������.
    ��һ��ҵ��ģ���˳�,��֪ͨCModuleManager,��ֹ����ģ��.

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/
void CWorkModule::ExitInstance()
{
    /* �򱾽��̵Ĺ���ģ�鷢�˳���Ϣ */

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

	//����һ��5��Ķ�ʱ��
	m_nTimerIDReqLicense = CWorkModule::SetTimer(5 * 1000);

    RequestLicense();

	return 0;
}

void CWorkModule::ParseMsg(MSG_INTRA* pMsg)
{
   	switch (pMsg->CmdCode)
	{
		//����Ƿ���License������Ӧ֡
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

	//ÿ30�뷢��һ��License����
	if (nTimerID == m_nTimerIDReqLicense)
	{
		RequestLicense();
	}
}

int CWorkModule::GetLicenseCount()
{
	return m_nLicenseCount;
}
