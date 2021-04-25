
#ifndef _COLLECTOR_MANAGER_H
#define _COLLECTOR_MANAGER_H

#include "../../com/frame/wrk_manager.h"
#include "../../com/include/i_frame_adapter.h"

class CCollectorMgr:public CModuleManager
{
public:
    CCollectorMgr();
    ~CCollectorMgr();
    int handle_signal(int signum, siginfo_t* st, ucontext_t* ut);

protected:

    virtual int InitInstance(void);
    virtual void ExitInstance(void);
    virtual int CreateWorkModules(void);
    virtual void ParseMsg(MSG_INTRA* pMsgParsed);
    virtual void OnTimer(const long nTimerID);

    void BeginAuxUpgrade(void);
    void EndAuxUpgrade(void);
    
    int m_nRecvSignal;           //�յ����ź�
    IFrameAdapter* m_pFrameAdapter;
};

#endif //_COLLECTOR_MANAGER_H
