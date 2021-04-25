#ifndef __CFG_SERVICE_BASE_H__
#define __CFG_SERVICE_BASE_H__

#include "service_base.h"

class CCFGService : public CServiceBase
{
protected:
    virtual void OnRun();
    virtual void OnStop();
};

#endif //__CFG_SERVICE_BASE_H__
