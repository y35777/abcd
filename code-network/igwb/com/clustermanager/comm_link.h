#ifndef __COMM_LINK_H__
#define __COMM_LINK_H__

#include "comm_port.h"
#include "heartbeat_link.h"

class CClusterNetwork;

class CCommLink : public CHeartBeatLink
{
/*
    ��Ա����
*/
public:

    CCommLink(UINT uPort,
              CClusterNetwork* pNetwork,
              int nLinkIndex,
              const char* const szLinkName = 0);
    virtual ~CCommLink();

    //����������COM�˿�
    virtual int Create();
    //��Զ˷�����Ϣ
    virtual int Send(SClusterFrame* pFrame);
    //�붨ʱ��
    virtual void Timer1Sec();

/*
    ��Ա����
*/
protected:
    //����ͨ�Ŷ���
    CCommPort     m_CCommPort;

    //����ͨѶ�ں�
    UINT          m_uCommPort;
};

#endif //__COMM_LINK_H__
