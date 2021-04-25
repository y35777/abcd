
#ifndef _ACCESS_POINT_MANAGER_H
#define _ACCESS_POINT_MANAGER_H

#include "../../com/include/precom.h"
#include "../../com/include/i_frame_adapter.h"

/* 话单包缓冲区类头文件 */
#include "../../com/frame/pck_cache.h"

#include "../../com/frame/wrk_manager.h"

class CAPMgr:public CModuleManager
{
public:
    CAPMgr();
    ~CAPMgr();
    int handle_signal(int signum, siginfo_t* st, ucontext_t* ut);

protected:

    virtual int InitInstance(void);
    virtual void ExitInstance(void);
    virtual int CreateWorkModules(void);
    virtual void ParseMsg(MSG_INTRA* pMsgParsed);
    virtual void OnTimer(const long nTimerID);

    void BeginAuxUpgrade(void);
    void ReqBPAuxUpgrad(void);
    void EndAuxUpgrade(void);
    void ProduceCDR(void);    
	
    CPacketCache *m_pPacketCache;
	char m_szApName[32];
	int m_nRecvSignal;           //收到的信号
    IFrameAdapter* m_pFrameAdapter;
};

#endif
