#ifndef __RESOURCE_USER_DEFINED_H__
#define __RESOURCE_USER_DEFINED_H__

#include "res_object.h"

class CResUserDefined : public CResObject
{
public:

    CResUserDefined(const char* const szCreateName,
                    const char* const szActiveName,
                    const char* const szDeActiveName,
                    const char* const szCheckStatus,
                    const char* const szResName,
                    ERES_TYPE nResType,
                    int nMaxLocalRecovery);
    virtual ~CResUserDefined();

    virtual int Create();       //资源创建函数
    virtual int CheckStatus();  //资源的状态检查函数
    virtual int Active();       //资源的激活函数
    virtual int DeActive();     //资源的去激活函数

protected:

    //执行激活操作的程序
    char m_szActiveName[MAX_PATH];

    //执行去激活操作的程序
    char m_szDeActiveName[MAX_PATH];

    //执行状态检查操作的程序
    char m_szCheckName[MAX_PATH];

    //执行创建操作的程序
    char m_szCreateName[MAX_PATH];
};


#endif //__RESOURCE_USER_DEFINED_H__
