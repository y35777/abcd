#include "../include/base_type.h"
#include "cluster_struct.h"
#include "cluster_network.h"
#include "comm_link.h"


/********************************************************
* ����: CCommLink::CCommLink
* ����: ���캯��
* ����: UINT uPort                   : ���ڶ˿ں�
*       CClusterNetwork* pNetwork    : CClusterNetwork�����ָ��
*       int nLinkIndex               : �������к�
*       const char* const szLinkName : ������
* ���:
* ����:
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
* ����: CCommLink::~CCommLink
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CCommLink::~CCommLink()
{
    m_CCommPort.Close();
}

/********************************************************
* ����: CCommLink::Create
* ����: ����������COM�˿�
* ����: 
* ���: 
* ����: int �����Ƿ�ɹ�
********************************************************/
int CCommLink::Create()
{
    return m_CCommPort.Open(m_uCommPort) ? ERR_SUCCESS : ERR_FAIL; 
}

/********************************************************
* ����: CCommLink::Send
* ����: ��Զ˷�����Ϣ
* ����: SClusterFrame* pFrame : �����͵���Ϣ֡
* ���: 
* ����: int ������Ϣ�Ƿ�ɹ�
********************************************************/
int CCommLink::Send(SClusterFrame* pFrame)
{
	UINT4 uCount;

    //����frame�ĳ���
	uCount = pFrame->nLen + CLUSTER_FRAME_HEADLENGTH; 

	return m_CCommPort.Write((BYTE *)pFrame, uCount) ? ERR_SUCCESS : ERR_FAIL; 
}

/********************************************************
* ����: CCommLink::Timer1Sec
* ����: �붨ʱ��
* ����:
* ���:
* ����: void
********************************************************/
void CCommLink::Timer1Sec()
{
	UINT4 uLen;
	SClusterFrame* pFrame;
    //�Ӵ����ж�����
	while(m_CCommPort.Read((BYTE**)&pFrame, &uLen))
	{
		//����ʧ��
		if(NULL == pFrame)
		{
			continue;
		}

		//�������֡�ĳ��Ȳ���ȷ����ɾ������
		if((pFrame->nLen != (uLen - CLUSTER_FRAME_HEADLENGTH)) || 
            (pFrame->nLen > CLUSTER_FRAME_MAXSIZE))
		{
			delete pFrame;
			continue;
		}

		//�����������OnReceive��������
		OnReceive(pFrame);
	}
    
    CHeartBeatLink::Timer1Sec();
}
