#ifndef __HEART_BEAT_LINK_H__
#define __HEART_BEAT_LINK_H__

#include "cluster_struct.h"

enum LINK_TYPE
{
    LT_COM,     //������������
    LT_UDP      //UDP��������
};

class CClusterNetwork;

class CHeartBeatLink
{
/*
    ��Ա����
*/
public:
    CHeartBeatLink(CClusterNetwork* pNetwork,
                   int nLinkIndex,
                   const char* const szLinkName = 0);
    virtual ~CHeartBeatLink();

    //���������˿�
    virtual int Create() = 0;
    //��Զ˷�����Ϣ
    virtual int Send(SClusterFrame* pFrame) = 0;
    //����Զ˷�������Ϣ
    virtual void OnReceive(SClusterFrame* pFrame);
    //��ȡ������·����
    LINK_TYPE GetLinkType();
    //��ȡ������·����
    const char* GetLinkName();
    //��ȡ������·���к�
    int GetLinkIndex();
    //��ȡ�����Ƿ����
    BOOL GetbIsAvailable();
    //�ñ��������󲻿���
    void Disable();
    //�붨ʱ��
    virtual void Timer1Sec();

/*
    ��Ա����
*/
protected:
   //˫��ͨѶ�����ָ��
   CClusterNetwork *    m_pNetwork;

   //������·����
   LINK_TYPE            m_nLinkType;

   //������·�Ƿ����
   BOOL                 m_bIsAvailable;

   //������·�жϼ���ʱ��
   int                  m_nLinkBrokenCounter;
#define LINKBROKEN_TIMEOUT  15

   //������
   STRING               m_strLinkName;

   //�������к�
   int                  m_nLinkIndex;

};

#endif //__HEART_BEAT_LINK_H__
