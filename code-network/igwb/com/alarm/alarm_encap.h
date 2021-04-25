#ifndef _ALARM_UTIL_ENCAP_H_
#define _ALARM_UTIL_ENCAP_H_

#include "../include/base_type.h"
#include "../include/i_alarm_util.h"
#include "ace/DLL.h"

extern "C" {

typedef IAlarmUtil* (*CREATEALARMUTIL)();
}

class CAlarmUtilEncap
{
public:
    CAlarmUtilEncap();
    ~CAlarmUtilEncap();

    IAlarmUtil* CreateAlarmUtil();

protected:
    ACE_DLL*     m_pDll;
};

#endif //_ALARM_UTIL_ENCAP_H_
