#ifndef _LOG_UTIL_ENCAP_H_
#define _LOG_UTIL_ENCAP_H_

#include "../include/base_type.h"
#include "../include/i_log_util.h"
#include "ace/DLL.h"

extern "C" {

typedef ILogUtil* (*CREATELOGUTIL)();
}

class CLogUtilEncap
{
public:
    CLogUtilEncap();
    ~CLogUtilEncap();

    ILogUtil* CreateLogUtil();

protected:
    ACE_DLL*     m_pDll;
};

#endif //_LOG_UTIL_ENCAP_H_
