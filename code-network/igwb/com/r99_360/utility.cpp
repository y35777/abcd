#pragma warning(disable:4706)

#include "../include/type_def.h"
#include "asn1_r99/CDRF_3GCDR.h"
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

    CDR3G_CallEventRecord* pFrontCDR = (CDR3G_CallEventRecord*)pABill;
    switch(pFrontCDR->choice)
    {
    case CDR3G_sgsnPDPRecord_chosen:
        {
            CDR3G_SGSNPDPRecord& cdr = pFrontCDR->u.sgsnPDPRecord;
            if(cdr.bit_mask & CDR3G_SGSNPDPRecord_recordSequenceNumber_present) 
            {
                cdr_type = PS_PARTIAL_SCDR;
            }
            else
            {
                cdr_type = PS_SCDR;
            }
        }
        break;

    case CDR3G_ggsnPDPRecord_chosen:
        {
            CDR3G_GGSNPDPRecord& cdr = pFrontCDR->u.ggsnPDPRecord;
            if(cdr.bit_mask & CDR3G_GGSNPDPRecord_recordSequenceNumber_present) 
            {
                cdr_type = PS_PARTIAL_GCDR;
            }
            else
            {
                cdr_type = PS_GCDR;
            }
        }
        break;

    case CDR3G_sgsnMMRecord_chosen :
        {
            CDR3G_SGSNMMRecord& cdr = pFrontCDR->u.sgsnMMRecord;
            if(cdr.bit_mask & CDR3G_SGSNMMRecord_recordSequenceNumber_present) 
            {
                cdr_type = PS_PARTIAL_MCDR;
            }
            else
            {
                cdr_type = PS_MCDR;
            }
        }
        break;

    case CDR3G_sgsnSMORecord_chosen:
        cdr_type = PS_SMOCDR;
        break;

    case CDR3G_sgsnSMTRecord_chosen:
        cdr_type = PS_SMTCDR;
        break;

    default:
        return -1;
    }

    return 0;
}