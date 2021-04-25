#ifndef _CFG_MANAGER_H
#define _CFG_MANAGER_H

#include "../../com/frame/wrk_manager.h"
#include "../../com/include/i_frame_adapter.h"

class CCfgMger:public CModuleManager
{
public:
    CCfgMger();
    ~CCfgMger();
    int handle_signal(int signum, siginfo_t* st, ucontext_t* ut);

protected:
    virtual int InitInstance(void);
    virtual void ExitInstance(void);
    virtual int CreateWorkModules(void);
    virtual void OnTimer(const long nTimerID);

	int m_nRecvSignal;           //收到的信号
    IFrameAdapter* m_pFrameAdapter;
};

#endif
