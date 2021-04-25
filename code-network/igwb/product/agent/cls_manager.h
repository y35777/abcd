
#ifndef _CLUSTER_MANAGER_H
#define _CLUSTER_MANAGER_H

#include "../../com/frame/wrk_manager.h"
#include "../../com/include/i_frame_adapter.h"

class CClstrMgr:public CModuleManager
{
public:
    CClstrMgr();
    ~CClstrMgr();

protected:

    virtual int InitInstance(void);
    virtual void ExitInstance(void);

    virtual int CreateWorkModules(void);

    int handle_signal(int signum, 
                      siginfo_t* st,
                      ucontext_t* ut);
	int m_nRecvSignal;            //收到的信号
    virtual void OnTimer(const long nTimerID);
    virtual void ParseMsg(MSG_INTRA* pMsgParsed);

    IFrameAdapter* m_pFrameAdapter;
};

#endif
