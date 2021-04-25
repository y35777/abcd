#ifndef _PERF_UTIL_ENCAP_H_
#define _PERF_UTIL_ENCAP_H_

#include "../include/base_type.h"
#include "i_wlan_as_oci.h"
#include "ace/DLL.h"

extern "C" {

typedef IWlanAsOci* (*CREATEWLANASOCI)();
}

class CWlanUtilEncap
{
public:
    CWlanUtilEncap();
    ~CWlanUtilEncap();

    IWlanAsOci* CreateWlanAsOci(const char* szFmtDll);

protected:
    ACE_DLL*     m_pDll;
};

#endif //_PERF_UTIL_ENCAP_H_
