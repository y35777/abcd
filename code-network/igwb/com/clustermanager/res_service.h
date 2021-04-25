#ifndef __RESOURCE_SERVICE_H__
#define __RESOURCE_SERVICE_H__

#include "res_object.h"

class CResService : public CResObject
{
public:

    CResService(const char* const szServiceName,
                const char* const szResName,
                ERES_TYPE nResType,
                int nMaxLocalRecovery);
    virtual ~CResService();

    virtual int Create();       //资源创建函数
    virtual int CheckStatus();  //资源的状态检查函数
    virtual int Active();       //资源的激活函数
    virtual int DeActive();     //资源的去激活函数

protected:

    //服务名
#define MAX_SERVICE_NAME    50
    char    m_szServiceName[MAX_SERVICE_NAME];

};


#endif //__RESOURCE_SERVICE_H__
