#ifndef __RESOURCE_PROGRAM_H__
#define __RESOURCE_PROGRAM_H__

#include "res_object.h"

class CResProgram : public CResObject
{
public:

    CResProgram(const char* const szAppPath,
                const char* const szResName,
                ERES_TYPE nResType,
                int nMaxLocalRecovery);
    virtual ~CResProgram();

    virtual int Create();       //资源创建函数
    virtual int CheckStatus();  //资源的状态检查函数
    virtual int Active();       //资源的激活函数
    virtual int DeActive();     //资源的去激活函数

protected:

    //进程全路径名
    char                m_szAppPath[MAX_PATH];

    //进程PID号
    pid_t               m_nPid;

    //ACE的进程管理对象
    ACE_Process_Manager m_PMgr;

    //业务进程的ACE环境
    ACE_Process_Options m_POptions;

};


#endif //__RESOURCE_PROGRAM_H__
