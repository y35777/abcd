
#ifndef _KERNEL_MANAGER_H
#define _KERNEL_MANAGER_H

#include "../../com/frame/wrk_manager.h"
#include "../../com/include/i_frame_adapter.h"

class CKenelMgr:public CModuleManager
{
public:
    CKenelMgr();
    ~CKenelMgr();

protected:
    
    virtual int InitInstance(void);
    virtual void ExitInstance(void);

    virtual int CreateWorkModules(void);
    virtual void OnTimer(const long nTimerID);

    int handle_signal(int signum, 
                      siginfo_t* st,
                      ucontext_t* ut);

	int m_nRecvSignal;            //�յ����ź�
    IFrameAdapter* m_pFrameAdapter;
};

#endif
