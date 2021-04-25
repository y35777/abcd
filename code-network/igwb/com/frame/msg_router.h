/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:msg_router.h
    ������Ϣ:
    
    ����CMsgRouter��Ķ���,�������������������н�����Ϣ.
    CMsgProcessor���������Ķ���,��֧����Ϣ����.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/

#ifndef _MESSAGE_ROUTER_H
#define _MESSAGE_ROUTER_H

#include "../include/precom.h"
#include "ace/Reactor.h"

class CMsgProcessor;

class CMsgRouter
{
protected:
    typedef std::map<int, CMsgProcessor*> MAPROUTE;

public:
    CMsgRouter(const PROCESS_TYPE pt);
    virtual ~CMsgRouter();

    BOOL Register(const MODULE_TYPE mt, CMsgProcessor* pModule);
    BOOL UnRegister(const MODULE_TYPE mt);
    int Route(MSG_INTRA* pMsgRouted);

protected:
    MAPROUTE m_RouteTab;            //·�ɱ�,����STL��mapģ����
    ACE_RW_Mutex m_RouterRWMutex;   //���ڱ���·�ɱ���̶߳�д��

    PROCESS_TYPE m_pt;              //�������̵ı��
    BOOL m_bTrack;                  //TRUE��ʾҪ����ҵ�����̸���

    BOOL SendTrackedMsg(MSG_INTRA* pMsgTracked);
    void ProcessOwnMsg(MSG_INTRA* pMsgOwn);
};

#endif
