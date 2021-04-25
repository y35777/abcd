#ifndef _PERF_UTIL_ENCAP_H_
#define _PERF_UTIL_ENCAP_H_

#include "../include/base_type.h"
#include "../include/i_perf_util.h"
#include "ace/DLL.h"

extern "C" {

typedef IPerfUtil* (*CREATEPERFUTIL)();
}

class CPerfUtilEncap
{
public:
    CPerfUtilEncap();
    ~CPerfUtilEncap();

    IPerfUtil* CreatePerfUtil();

protected:
    ACE_DLL*     m_pDll;
};

#endif //_PERF_UTIL_ENCAP_H_
