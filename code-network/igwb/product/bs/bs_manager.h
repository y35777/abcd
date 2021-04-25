///////////////////////////////////////////////////////////////////////////////
//GTP'计费接口增加 Z30971 2004/06
#ifndef __BS_MANAGER_H__
#define __BS_MANAGER_H__

#include "../../com/frame/wrk_manager.h"

class CBSMger:public CModuleManager
{
public:
    CBSMger();
    ~CBSMger();
    int handle_signal(int signum, siginfo_t* st, ucontext_t* ut);

protected:
    virtual int InitInstance(void);
    virtual int CreateWorkModules(void);

	int m_nRecvSignal;           //收到的信号
    virtual void OnTimer(const long nTimerID);
};

#endif //__BS_MANAGER_H__
