#ifndef _CDR_STAT_CFG_ENCAP_H_
#define _CDR_STAT_CFG_ENCAP_H_

#include "../include/base_type.h"
#include "i_cdr_stat_cfg.h"
#include "ace/DLL.h"

extern "C" {

typedef ICDRStatCfg* (*CREATECDRSTATCFG)();
}

class CStatCfgEncap
{
public:
    CStatCfgEncap();
    ~CStatCfgEncap();

    ICDRStatCfg* CreateCDRStatCfg();

protected:
    ACE_DLL*     m_pDll;
};

#endif //_CDR_STAT_CFG_ENCAP_H_
