#pragma warning(disable:4706)

#include "../include/type_def.h"
#include "asn1_r99/CDRF_CMCC_3GCDR.h"
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

    CMCC_3GCDR_CallEventRecord* pFrontCDR = (CMCC_3GCDR_CallEventRecord*)pABill;
    switch(pFrontCDR->choice)
    {
    case CMCC_3GCDR_sgsnPDPRecord_chosen:
        {
            CMCC_3GCDR_SGSNPDPRecord& cdr = pFrontCDR->u.sgsnPDPRecord;
            if(cdr.bit_mask & CMCC_3GCDR_SGSNPDPRecord_recordSequenceNumber_present) 
            {
                cdr_type = PS_PARTIAL_SCDR;
            }
            else
            {
                cdr_type = PS_SCDR;
            }
        }
        break;

    case CMCC_3GCDR_ggsnPDPRecord_chosen:
        {
            CMCC_3GCDR_GGSNPDPRecord& cdr = pFrontCDR->u.ggsnPDPRecord;
            if(cdr.bit_mask & CMCC_3GCDR_GGSNPDPRecord_localSequenceNumber_present) 
            {
                cdr_type = PS_PARTIAL_GCDR;
            }
            else
            {
                cdr_type = PS_GCDR;
            }
        }
        break;

    case CMCC_3GCDR_sgsnMMRecord_chosen :
        {
            CMCC_3GCDR_SGSNMMRecord& cdr = pFrontCDR->u.sgsnMMRecord;
            if(cdr.bit_mask & CMCC_3GCDR_SGSNMMRecord_recordSequenceNumber_present) 
            {
                cdr_type = PS_PARTIAL_MCDR;
            }
            else
            {
                cdr_type = PS_MCDR;
            }
        }
        break;

    case CMCC_3GCDR_sgsnSMORecord_chosen:
        cdr_type = PS_SMOCDR;
        break;

    case CMCC_3GCDR_sgsnSMTRecord_chosen:
        cdr_type = PS_SMTCDR;
        break;

    default:
        return -1;
    }

    return 0;
}