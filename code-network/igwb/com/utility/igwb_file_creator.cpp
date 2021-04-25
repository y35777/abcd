// igwb_file.cpp: implementation of the CIGWBFile class.
//
//////////////////////////////////////////////////////////////////////
#include "../include/toolbox.h"
#include "../include/frame.h"
#include "igwb_file_creator.h"
#include "igwb_file.h"
#include "ps_file.h"
#include "cc08_file.h"
#include "5ess_file.h"
#include "s1240_file.h"
#include "zxj_file.h"
#include "ewsd_file.h"
#include "text_file.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIGWBFileCreator::	CIGWBFileCreator()
{
    //
}

CIGWBFileCreator::~CIGWBFileCreator()
{
    //
}

CIGWBFile* CIGWBFileCreator::CreateIGWBFile(UINT4 uBillType, 
                                            UINT4 uAPType, 
                                            UINT4 uBillLen,
                                            int nAPID)
{
    switch (uAPType)
    {
        case AP_PS:
        case AP_PS_RESERVED:
            return new CPSFile(uBillType, uBillLen, nAPID);

        case AP_CC08:
			return new CCc08File(uBillType, uBillLen, nAPID);

        case AP_5ESS:
			return new C5essFile(uBillType, uBillLen, nAPID);

        case AP_S1240:
			return new CS1240File(uBillType, uBillLen, nAPID);

        case AP_ZXJ:
			return new CZxjFile(uBillType, uBillLen, nAPID);

        case AP_EWSD:
			return new CEwsdFile(uBillType, uBillLen, nAPID);

        case AP_TEXT_IN:
			return new CTextFile(uBillType, uBillLen, nAPID);

        default:
			return new CIGWBFile(uBillType, uBillLen, nAPID);
    }

    return NULL;
}
