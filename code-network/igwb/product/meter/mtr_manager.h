#ifndef __METER_ACC_MANAGER_H__
#define __METER_ACC_MANAGER_H__

#include "../../com/frame/wrk_manager.h"
#include "../../com/include/i_frame_adapter.h"

class CMeterAccMgr : public CModuleManager
{
public:
    CMeterAccMgr();
    ~CMeterAccMgr();
    int handle_signal(int signum, siginfo_t* st, ucontext_t* ut);

protected:
    virtual int InitInstance(void);
    virtual void ExitInstance(void);
    virtual int CreateWorkModules(void);
    virtual void OnTimer(const long nTimerID);

	int m_nRecvSignal;           //�յ����ź�
    IFrameAdapter* m_pFrameAdapter;
};

#endif //__METER_ACC_MANAGER_H__
