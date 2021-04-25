#pragma warning(disable:4706)

#include "../include/type_def.h"
#include "asn1_r98/CDRF_2GCDR_Org.h"
#include "../include/export_dll.h"

extern "C" IGWB_Svc_Export 
int GetOrginalCDRType(const void* pABill, 
                      PS_CDR_TYPE& cdr_type)
{
    //不符合本格式
    if(pABill == NULL)
    {
        return -1;
    }

    Org_CallEventRecord* pFrontCDR = (Org_CallEventRecord*)pABill;
    switch(pFrontCDR->choice)
    {
    case Org_sgsnPDPRecord_chosen:
        {
            Org_SGSNPDPRecord& cdr = pFrontCDR->u.sgsnPDPRecord;
            if(cdr.bit_mask & Org_SGSNPDPRecord_recordSequenceNumber_present) 
            {
                cdr_type = PS_PARTIAL_SCDR;
            }
            else
            {
                cdr_type = PS_SCDR;
            }
        }
        break;

    case Org_ggsnPDPRecord_chosen:
        {
            Org_GGSNPDPRecord& cdr = pFrontCDR->u.ggsnPDPRecord;
            if(cdr.bit_mask & Org_GGSNPDPRecord_recordSequenceNumber_present)
            {
                cdr_type = PS_PARTIAL_GCDR;
            }
            else
            {
                cdr_type = PS_GCDR;
            }
        }
        break;

    case Org_sgsnMMRecord_chosen :
        {
            Org_SGSNMMRecord& cdr = pFrontCDR->u.sgsnMMRecord;
            if(cdr.bit_mask & Org_SGSNMMRecord_recordSequenceNumber_present) 
            {
                cdr_type = PS_PARTIAL_MCDR;
            }
            else
            {
                cdr_type = PS_MCDR;
            }
        }
        break;

    case Org_sgsnSMORecord_chosen:
        cdr_type = PS_SMOCDR;
        break;

    case Org_sgsnSMTRecord_chosen:
        cdr_type = PS_SMTCDR;
        break;

    default:
        return -1;
    }

    return 0;
}
