#ifndef _CDR_STAT_RST_ENCAP_H_
#define _CDR_STAT_RST_ENCAP_H_

#include "../include/base_type.h"
#include "i_cdr_stat_rst.h"
#include "ace/DLL.h"

extern "C" {

typedef ICDRStatRst* (*CREATECDRSTATRST)();
}

class CStatRstEncap
{
public:
    CStatRstEncap();
    ~CStatRstEncap();

    ICDRStatRst* CreateCDRStatRst();

protected:
    ACE_DLL*     m_pDll;
};

#endif //_CDR_STAT_RST_ENCAP_H_
