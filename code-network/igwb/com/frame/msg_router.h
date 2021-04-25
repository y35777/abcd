/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:msg_router.h
    描述信息:
    
    包含CMsgRouter类的定义,该类用于在整个进程中交换消息.
    CMsgProcessor类包含该类的对象,以支持消息交换.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-16,创建.
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
    MAPROUTE m_RouteTab;            //路由表,采用STL的map模板类
    ACE_RW_Mutex m_RouterRWMutex;   //用于保护路由表的线程读写锁

    PROCESS_TYPE m_pt;              //所属进程的编号
    BOOL m_bTrack;                  //TRUE表示要进行业务流程跟踪

    BOOL SendTrackedMsg(MSG_INTRA* pMsgTracked);
    void ProcessOwnMsg(MSG_INTRA* pMsgOwn);
};

#endif
