#include "../include/base_type.h"
#include <assert.h>
#include "heartbeat_link.h"
#include "cluster_network.h"


/********************************************************
* ����: CHeartBeatLink::CHeartBeatLink
* ����: ���캯��
* ����: CClusterNetwork* pNetwork    : CClusterNetwork�����ָ��
*       int nLinkIndex               : �������к�
*       const char* const szLinkName : ������
* ���:
* ����:
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
* ����: CHeartBeatLink::~CHeartBeatLink
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CHeartBeatLink::~CHeartBeatLink()
{
    m_pNetwork = NULL;
}

/********************************************************
* ����: CHeartBeatLink::OnReceive
* ����: ����Զ˷�������Ϣ
* ����: SClusterFrame* pFrame : ˫����Ϣ֡�ṹ��ָ��
* ���:
* ����: void
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
* ����: CHeartBeatLink::GetLinkType
* ����: ��ȡ������·����
* ����:
* ���:
* ����: LINK_TYPE ����������·����
********************************************************/
LINK_TYPE CHeartBeatLink::GetLinkType()
{
    return m_nLinkType;
}

/********************************************************
* ����: CHeartBeatLink::GetLinkName
* ����: ��ȡ������·����
* ����:
* ���:
* ����: const char* ����������·����
********************************************************/
const char* CHeartBeatLink::GetLinkName()
{
    return m_strLinkName.c_str();
}

/********************************************************
* ����: CHeartBeatLink::GetLinkIndex
* ����: ��ȡ������·���к�
* ����:
* ���:
* ����: int ����������·���к�
********************************************************/
int CHeartBeatLink::GetLinkIndex()
{
    return m_nLinkIndex;
}

/********************************************************
* ����: CHeartBeatLink::GetbIsAvailable
* ����: ��ȡ�����Ƿ����
* ����:
* ���:
* ����: BOOL �����Ƿ����
********************************************************/
BOOL CHeartBeatLink::GetbIsAvailable()
{
    return m_bIsAvailable;
}

/********************************************************
* ����: CHeartBeatLink::Disable
* ����: �ñ��������󲻿���
* ����:
* ���:
* ����: void
********************************************************/
void CHeartBeatLink::Disable()
{
    m_bIsAvailable = FALSE;
}

/********************************************************
* ����: CHeartBeatLink::Timer1Sec
* ����: �붨ʱ��
* ����:
* ���:
* ����: void
********************************************************/
void CHeartBeatLink::Timer1Sec()
{
    if((m_bIsAvailable) && (++m_nLinkBrokenCounter > LINKBROKEN_TIMEOUT))
    {
        m_bIsAvailable = FALSE;
    }
}
