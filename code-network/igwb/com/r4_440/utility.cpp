#pragma warning(disable:4706)

#include "../include/type_def.h"
#include "../toolbase/field_filter.h"
#include "../format/format_def.h"
#include "asn1_r4/CDRF_3GPPR4_Org.h"
#include "asn1_r4/CDRF_3GPPR4_Fin.h"
#include "../include/export_dll.h"
#include "r4_cdr_browse.h"
#include "../include/toolbox.h"

extern "C" IGWB_Svc_Export 
int GetOrginalCDRType(const void* pABill, 
                      PS_CDR_TYPE& cdr_type)
{
    //不符合本格式
    if(pABill == NULL)
    {
        return -1;
    }

    R4_Org_CallEventRecord* pFrontCDR = (R4_Org_CallEventRecord*)pABill;
    switch(pFrontCDR->choice)
    {
    case R4_Org_sgsnPDPRecord_chosen:
        {
            R4_Org_SGSNPDPRecord& cdr = pFrontCDR->u.sgsnPDPRecord;
            if(cdr.bit_mask & R4_Org_SGSNPDPRecord_recordSequenceNumber_present) 
            {
                cdr_type = PS_PARTIAL_SCDR;
            }
            else
            {
                cdr_type = PS_SCDR;
            }
        }
        break;

    case R4_Org_ggsnPDPRecord_chosen:
        {
            R4_Org_GGSNPDPRecord& cdr = pFrontCDR->u.ggsnPDPRecord;
            if(cdr.bit_mask & R4_Org_GGSNPDPRecord_recordSequenceNumber_present)
            {
                cdr_type = PS_PARTIAL_GCDR;
            }
            else
            {
                cdr_type = PS_GCDR;
            }
        }
        break;

    case R4_Org_sgsnMMRecord_chosen :
        {
            R4_Org_SGSNMMRecord& cdr = pFrontCDR->u.sgsnMMRecord;
            if(cdr.bit_mask & R4_Org_SGSNMMRecord_recordSequenceNumber_present)
            {
                cdr_type = PS_PARTIAL_MCDR;
            }
            else
            {
                cdr_type = PS_MCDR;
            }
        }
        break;

    case R4_Org_sgsnSMORecord_chosen:
        cdr_type = PS_SMOCDR;
        break;

    case R4_Org_sgsnSMTRecord_chosen:
        cdr_type = PS_SMTCDR;
        break;

    case R4_Org_sgsnLCTRecord_chosen:
        cdr_type = PS_LCSMTCDR;
        break;

    case R4_Org_sgsnLCORecord_chosen:
        cdr_type = PS_LCSMOCDR;
        break;

    case R4_Org_sgsnLCNRecord_chosen:
        cdr_type = PS_LCSNICDR;
        break;

    default:
        return -1;
    }

    return 0;
}

#define CHECK_OM_FIELD(mask, section, field) \
    ((cdr.bit_mask & mask == 0) \
     && CCDRFieldFilter::GetInstance(szCfgFile)->GetField(section, #field))
     /*&& (&cdr.field != NULL)此行用来验证域名是否存在，编绎时应去掉 \*/

extern "C" IGWB_Svc_Export 
int CheckOMField(const char* szCfgFile,
                 const void* pABill,
                 BOOL        bMerged)
{
    if(!bMerged)
    {
        R4_Org_CallEventRecord* pFrontCDR = (R4_Org_CallEventRecord*)pABill;
        switch(pFrontCDR->choice)
        {
        case R4_Org_sgsnPDPRecord_chosen:
            {
                R4_Org_SGSNPDPRecord& cdr = pFrontCDR->u.sgsnPDPRecord;

                if(CHECK_OM_FIELD(R4_Org_SGSNPDPRecord_sgsnAddress_present,
                                  S_CDR_DESC,
                                  sgsnAddress))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNPDPRecord_msNetworkCapability_present,
                                  S_CDR_DESC,
                                  msNetworkCapability))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNPDPRecord_routingArea_present,
                                  S_CDR_DESC,
                                  routingArea))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNPDPRecord_locationAreaCode_present,
                                  S_CDR_DESC,
                                  locationAreaCode))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNPDPRecord_cellIdentifier_present,
                                  S_CDR_DESC,
                                  cellIdentifier))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNPDPRecord_accessPointNameNI_present,
                                  S_CDR_DESC,
                                  accessPointNameNI))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNPDPRecord_pdpType_present,
                                  S_CDR_DESC,
                                  pdpType))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNPDPRecord_listOfTrafficVolumes_present,
                                  S_CDR_DESC,
                                  listOfTrafficVolumes))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNPDPRecord_diagnostics_present,
                                  S_CDR_DESC,
                                  diagnostics))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNPDPRecord_nodeID_present,
                                  S_CDR_DESC,
                                  nodeID))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNPDPRecord_localSequenceNumber_present,
                                  S_CDR_DESC,
                                  localSequenceNumber))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNPDPRecord_apnSelectionMode_present,
                                  S_CDR_DESC,
                                  apnSelectionMode))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_accessPointNameOI_present,
                                  S_CDR_DESC,
                                  accessPointNameOI))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNPDPRecord_servedMSISDN_present,
                                  S_CDR_DESC,
                                  servedMSISDN))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNPDPRecord_chChSelectionMode_present,
                                  S_CDR_DESC,
                                  chChSelectionMode))
                {
                    return -1;
                }
            }
            break;

        case R4_Org_ggsnPDPRecord_chosen:
            {
                R4_Org_GGSNPDPRecord& cdr = pFrontCDR->u.ggsnPDPRecord;

                if(CHECK_OM_FIELD(R4_Org_GGSNPDPRecord_accessPointNameNI_present,
                                  G_CDR_DESC,
                                  accessPointNameNI))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_GGSNPDPRecord_pdpType_present,
                                  G_CDR_DESC,
                                  pdpType))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_GGSNPDPRecord_listOfTrafficVolumes_present,
                                  G_CDR_DESC,
                                  listOfTrafficVolumes))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_GGSNPDPRecord_diagnostics_present,
                                  G_CDR_DESC,
                                  diagnostics))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_GGSNPDPRecord_nodeID_present,
                                  G_CDR_DESC,
                                  nodeID))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_GGSNPDPRecord_localSequenceNumber_present,
                                  G_CDR_DESC,
                                  localSequenceNumber))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_GGSNPDPRecord_apnSelectionMode_present,
                                  G_CDR_DESC,
                                  apnSelectionMode))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_GGSNPDPRecord_servedMSISDN_present,
                                  G_CDR_DESC,
                                  servedMSISDN))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_GGSNPDPRecord_chChSelectionMode_present,
                                  G_CDR_DESC,
                                  chChSelectionMode))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_sgsnPLMNIdentifier_present,
                                  G_CDR_DESC,
                                  sgsnPLMNIdentifier))
                {
                    return -1;
                }
            }
            break;

        case R4_Org_sgsnMMRecord_chosen :
            {
                R4_Org_SGSNMMRecord& cdr = pFrontCDR->u.sgsnMMRecord;
                if(CHECK_OM_FIELD(R4_Org_SGSNMMRecord_sgsnAddress_present,
                                  M_CDR_DESC,
                                  sgsnAddress))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNMMRecord_msNetworkCapability_present,
                                  M_CDR_DESC,
                                  msNetworkCapability))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNMMRecord_routingArea_present,
                                  M_CDR_DESC,
                                  routingArea))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNMMRecord_locationAreaCode_present,
                                  M_CDR_DESC,
                                  locationAreaCode))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNMMRecord_cellIdentifier_present,
                                  M_CDR_DESC,
                                  cellIdentifier))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_duration_present,
                                  M_CDR_DESC,
                                  duration))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNMMRecord_diagnostics_present,
                                  M_CDR_DESC,
                                  diagnostics))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNMMRecord_localSequenceNumber_present,
                                  M_CDR_DESC,
                                  localSequenceNumber))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNMMRecord_servedMSISDN_present,
                                  M_CDR_DESC,
                                  servedMSISDN))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNMMRecord_nodeID_present,
                                  M_CDR_DESC,
                                  nodeID))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNMMRecord_chChSelectionMode_present,
                                  M_CDR_DESC,
                                  chChSelectionMode))
                {
                    return -1;
                }
            }
            break;

        case R4_Org_sgsnSMORecord_chosen:
            {
                R4_Org_SGSNSMORecord& cdr = pFrontCDR->u.sgsnSMORecord;
                if(CHECK_OM_FIELD(R4_Org_SGSNSMORecord_servedMSISDN_present,
                                  S_SMO_CDR_DESC,
                                  servedMSISDN))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNSMORecord_msNetworkCapability_present,
                                  S_SMO_CDR_DESC,
                                  msNetworkCapability))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNSMORecord_serviceCentre_present,
                                  S_SMO_CDR_DESC,
                                  serviceCentre))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNSMORecord_recordingEntity_present,
                                  S_SMO_CDR_DESC,
                                  recordingEntity))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNSMORecord_locationArea_present,
                                  S_SMO_CDR_DESC,
                                  locationArea))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNSMORecord_routingArea_present,
                                  S_SMO_CDR_DESC,
                                  routingArea))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNSMORecord_cellIdentifier_present,
                                  S_SMO_CDR_DESC,
                                  cellIdentifier))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNSMORecord_nodeID_present,
                                  S_SMO_CDR_DESC,
                                  nodeID))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNSMORecord_localSequenceNumber_present,
                                  S_SMO_CDR_DESC,
                                  localSequenceNumber))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_destinationNumber_present,
                                  S_SMO_CDR_DESC,
                                  destinationNumber))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNSMORecord_chChSelectionMode_present,
                                  S_SMO_CDR_DESC,
                                  chChSelectionMode))
                {
                    return -1;
                }
            }
            break;

        case R4_Org_sgsnSMTRecord_chosen:
            {
                R4_Org_SGSNSMTRecord& cdr = pFrontCDR->u.sgsnSMTRecord;
                if(CHECK_OM_FIELD(R4_Org_SGSNSMTRecord_servedMSISDN_present,
                                  S_SMT_CDR_DESC,
                                  servedMSISDN))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNSMTRecord_msNetworkCapability_present,
                                  S_SMT_CDR_DESC,
                                  msNetworkCapability))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNSMTRecord_serviceCentre_present,
                                  S_SMT_CDR_DESC,
                                  serviceCentre))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNSMTRecord_recordingEntity_present,
                                  S_SMT_CDR_DESC,
                                  recordingEntity))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNSMTRecord_locationArea_present,
                                  S_SMT_CDR_DESC,
                                  locationArea))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNSMTRecord_routingArea_present,
                                  S_SMT_CDR_DESC,
                                  routingArea))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNSMTRecord_cellIdentifier_present,
                                  S_SMT_CDR_DESC,
                                  cellIdentifier))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNSMTRecord_nodeID_present,
                                  S_SMT_CDR_DESC,
                                  nodeID))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNSMTRecord_localSequenceNumber_present,
                                  S_SMT_CDR_DESC,
                                  localSequenceNumber))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNSMTRecord_chChSelectionMode_present,
                                  S_SMT_CDR_DESC,
                                  chChSelectionMode))
                {
                    return -1;
                }
            }
            break;

        case R4_Org_sgsnLCTRecord_chosen:
            {
                R4_Org_SGSNLCTRecord& cdr = pFrontCDR->u.sgsnLCTRecord;
                if(CHECK_OM_FIELD(R4_Org_SGSNLCTRecord_servedMSISDN_present,
                                  S_LCS_MT_CDR_DESC,
                                  servedMSISDN))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCTRecord_sgsnAddress_present,
                                  S_LCS_MT_CDR_DESC,
                                  sgsnAddress))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCTRecord_measurementDuration_present,
                                  S_LCS_MT_CDR_DESC,
                                  measurementDuration))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCTRecord_location_present,
                                  S_LCS_MT_CDR_DESC,
                                  location))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCTRecord_routingArea_present,
                                  S_LCS_MT_CDR_DESC,
                                  routingArea))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCTRecord_nodeID_present,
                                  S_LCS_MT_CDR_DESC,
                                  nodeID))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCTRecord_localSequenceNumber_present,
                                  S_LCS_MT_CDR_DESC,
                                  localSequenceNumber))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCTRecord_chChSelectionMode_present,
                                  S_LCS_MT_CDR_DESC,
                                  chChSelectionMode))
                {
                    return -1;
                }
            }
            break;

        case R4_Org_sgsnLCORecord_chosen:
            {
                R4_Org_SGSNLCORecord& cdr = pFrontCDR->u.sgsnLCORecord;

                if(CHECK_OM_FIELD(R4_Org_SGSNLCORecord_servedMSISDN_present,
                                  S_LCS_MT_CDR_DESC,
                                  servedMSISDN))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCORecord_sgsnAddress_present,
                                  S_LCS_MT_CDR_DESC,
                                  sgsnAddress))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCORecord_measurementDuration_present,
                                  S_LCS_MT_CDR_DESC,
                                  measurementDuration))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCORecord_location_present,
                                  S_LCS_MT_CDR_DESC,
                                  location))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCORecord_routingArea_present,
                                  S_LCS_MT_CDR_DESC,
                                  routingArea))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCORecord_nodeID_present,
                                  S_LCS_MT_CDR_DESC,
                                  nodeID))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCORecord_localSequenceNumber_present,
                                  S_LCS_MT_CDR_DESC,
                                  localSequenceNumber))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCORecord_chChSelectionMode_present,
                                  S_LCS_MT_CDR_DESC,
                                  chChSelectionMode))
                {
                    return -1;
                }
            }
            break;

        case R4_Org_sgsnLCNRecord_chosen:
            {
                R4_Org_SGSNLCNRecord& cdr = pFrontCDR->u.sgsnLCNRecord;
                
                if(CHECK_OM_FIELD(R4_Org_SGSNLCNRecord_sgsnAddress_present,
                                  S_LCS_MT_CDR_DESC,
                                  sgsnAddress))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCNRecord_measurementDuration_present,
                                  S_LCS_MT_CDR_DESC,
                                  measurementDuration))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCNRecord_location_present,
                                  S_LCS_MT_CDR_DESC,
                                  location))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCNRecord_routingArea_present,
                                  S_LCS_MT_CDR_DESC,
                                  routingArea))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCNRecord_nodeID_present,
                                  S_LCS_MT_CDR_DESC,
                                  nodeID))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCNRecord_localSequenceNumber_present,
                                  S_LCS_MT_CDR_DESC,
                                  localSequenceNumber))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Org_SGSNLCNRecord_chChSelectionMode_present,
                                  S_LCS_MT_CDR_DESC,
                                  chChSelectionMode))
                {
                    return -1;
                }
            }
            break;

        default:
            break;
        }
    }
    else
    {
        R4_Fin_CallEventRecord* pBackCDR = (R4_Fin_CallEventRecord*)pABill;
        switch(pBackCDR->choice)
        {
        case R4_Fin_sgsnPDPRecord_chosen:
            {
                R4_Fin_SGSNPDPRecord& cdr = pBackCDR->u.sgsnPDPRecord;

                if(CHECK_OM_FIELD(R4_Fin_SGSNPDPRecord_sgsnAddress_present,
                                  S_CDR_DESC,
                                  sgsnAddress))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNPDPRecord_msNetworkCapability_present,
                                  S_CDR_DESC,
                                  msNetworkCapability))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNPDPRecord_routingArea_present,
                                  S_CDR_DESC,
                                  routingArea))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNPDPRecord_locationAreaCode_present,
                                  S_CDR_DESC,
                                  locationAreaCode))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNPDPRecord_cellIdentifier_present,
                                  S_CDR_DESC,
                                  cellIdentifier))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNPDPRecord_accessPointNameNI_present,
                                  S_CDR_DESC,
                                  accessPointNameNI))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNPDPRecord_pdpType_present,
                                  S_CDR_DESC,
                                  pdpType))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNPDPRecord_listOfTrafficVolumes_present,
                                  S_CDR_DESC,
                                  listOfTrafficVolumes))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNPDPRecord_diagnostics_present,
                                  S_CDR_DESC,
                                  diagnostics))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNPDPRecord_nodeID_present,
                                  S_CDR_DESC,
                                  nodeID))
                {
                    return -1;
                }
                //if(CHECK_OM_FIELD(R4_Fin_SGSNPDPRecord_localSequenceNumber_present,
                //                 S_CDR_DESC,
                //                  localSequenceNumber))
                //{
                //    return -1;
                //}
                if(CHECK_OM_FIELD(R4_Fin_SGSNPDPRecord_apnSelectionMode_present,
                                  S_CDR_DESC,
                                  apnSelectionMode))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_accessPointNameOI_present,
                                  S_CDR_DESC,
                                  accessPointNameOI))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNPDPRecord_servedMSISDN_present,
                                  S_CDR_DESC,
                                  servedMSISDN))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNPDPRecord_chChSelectionMode_present,
                                  S_CDR_DESC,
                                  chChSelectionMode))
                {
                    return -1;
                }
            }
            break;

        case R4_Fin_ggsnPDPRecord_chosen:
            {
                R4_Fin_GGSNPDPRecord& cdr = pBackCDR->u.ggsnPDPRecord;

                if(CHECK_OM_FIELD(R4_Fin_GGSNPDPRecord_accessPointNameNI_present,
                                  G_CDR_DESC,
                                  accessPointNameNI))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_GGSNPDPRecord_pdpType_present,
                                  G_CDR_DESC,
                                  pdpType))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_GGSNPDPRecord_listOfTrafficVolumes_present,
                                  G_CDR_DESC,
                                  listOfTrafficVolumes))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_GGSNPDPRecord_diagnostics_present,
                                  G_CDR_DESC,
                                  diagnostics))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_GGSNPDPRecord_nodeID_present,
                                  G_CDR_DESC,
                                  nodeID))
                {
                    return -1;
                }
                //if(CHECK_OM_FIELD(R4_Fin_GGSNPDPRecord_localSequenceNumber_present,
                //                  G_CDR_DESC,
                //                  localSequenceNumber))
                //{
                //    return -1;
                //}
                if(CHECK_OM_FIELD(R4_Fin_GGSNPDPRecord_apnSelectionMode_present,
                                  G_CDR_DESC,
                                  apnSelectionMode))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_GGSNPDPRecord_servedMSISDN_present,
                                  G_CDR_DESC,
                                  servedMSISDN))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_GGSNPDPRecord_chChSelectionMode_present,
                                  G_CDR_DESC,
                                  chChSelectionMode))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_sgsnPLMNIdentifier_present,
                                  G_CDR_DESC,
                                  sgsnPLMNIdentifier))
                {
                    return -1;
                }
            }
            break;

        case R4_Fin_sgsnMMRecord_chosen :
            {
                R4_Fin_SGSNMMRecord& cdr = pBackCDR->u.sgsnMMRecord;
                if(CHECK_OM_FIELD(R4_Fin_SGSNMMRecord_sgsnAddress_present,
                                  M_CDR_DESC,
                                  sgsnAddress))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNMMRecord_msNetworkCapability_present,
                                  M_CDR_DESC,
                                  msNetworkCapability))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNMMRecord_routingArea_present,
                                  M_CDR_DESC,
                                  routingArea))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNMMRecord_locationAreaCode_present,
                                  M_CDR_DESC,
                                  locationAreaCode))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNMMRecord_cellIdentifier_present,
                                  M_CDR_DESC,
                                  cellIdentifier))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_duration_present,
                                  M_CDR_DESC,
                                  duration))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNMMRecord_diagnostics_present,
                                  M_CDR_DESC,
                                  diagnostics))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNMMRecord_localSequenceNumber_present,
                                  M_CDR_DESC,
                                  localSequenceNumber))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNMMRecord_servedMSISDN_present,
                                  M_CDR_DESC,
                                  servedMSISDN))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNMMRecord_nodeID_present,
                                  M_CDR_DESC,
                                  nodeID))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNMMRecord_chChSelectionMode_present,
                                  M_CDR_DESC,
                                  chChSelectionMode))
                {
                    return -1;
                }
            }
            break;

        case R4_Fin_sgsnSMORecord_chosen:
            {
                R4_Fin_SGSNSMORecord& cdr = pBackCDR->u.sgsnSMORecord;
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMORecord_servedMSISDN_present,
                                  S_SMO_CDR_DESC,
                                  servedMSISDN))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMORecord_msNetworkCapability_present,
                                  S_SMO_CDR_DESC,
                                  msNetworkCapability))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMORecord_serviceCentre_present,
                                  S_SMO_CDR_DESC,
                                  serviceCentre))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMORecord_recordingEntity_present,
                                  S_SMO_CDR_DESC,
                                  recordingEntity))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMORecord_locationArea_present,
                                  S_SMO_CDR_DESC,
                                  locationArea))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMORecord_routingArea_present,
                                  S_SMO_CDR_DESC,
                                  routingArea))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMORecord_cellIdentifier_present,
                                  S_SMO_CDR_DESC,
                                  cellIdentifier))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMORecord_nodeID_present,
                                  S_SMO_CDR_DESC,
                                  nodeID))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMORecord_localSequenceNumber_present,
                                  S_SMO_CDR_DESC,
                                  localSequenceNumber))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_destinationNumber_present,
                                  S_SMO_CDR_DESC,
                                  destinationNumber))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMORecord_chChSelectionMode_present,
                                  S_SMO_CDR_DESC,
                                  chChSelectionMode))
                {
                    return -1;
                }
            }
            break;

        case R4_Fin_sgsnSMTRecord_chosen:
            {
                R4_Fin_SGSNSMTRecord& cdr = pBackCDR->u.sgsnSMTRecord;
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMTRecord_servedMSISDN_present,
                                  S_SMT_CDR_DESC,
                                  servedMSISDN))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMTRecord_msNetworkCapability_present,
                                  S_SMT_CDR_DESC,
                                  msNetworkCapability))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMTRecord_serviceCentre_present,
                                  S_SMT_CDR_DESC,
                                  serviceCentre))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMTRecord_recordingEntity_present,
                                  S_SMT_CDR_DESC,
                                  recordingEntity))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMTRecord_locationArea_present,
                                  S_SMT_CDR_DESC,
                                  locationArea))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMTRecord_routingArea_present,
                                  S_SMT_CDR_DESC,
                                  routingArea))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMTRecord_cellIdentifier_present,
                                  S_SMT_CDR_DESC,
                                  cellIdentifier))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMTRecord_nodeID_present,
                                  S_SMT_CDR_DESC,
                                  nodeID))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMTRecord_localSequenceNumber_present,
                                  S_SMT_CDR_DESC,
                                  localSequenceNumber))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNSMTRecord_chChSelectionMode_present,
                                  S_SMT_CDR_DESC,
                                  chChSelectionMode))
                {
                    return -1;
                }
            }
            break;

        case R4_Fin_sgsnLCTRecord_chosen:
            {
                R4_Fin_SGSNLCTRecord& cdr = pBackCDR->u.sgsnLCTRecord;
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCTRecord_servedMSISDN_present,
                                  S_LCS_MT_CDR_DESC,
                                  servedMSISDN))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCTRecord_sgsnAddress_present,
                                  S_LCS_MT_CDR_DESC,
                                  sgsnAddress))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCTRecord_measurementDuration_present,
                                  S_LCS_MT_CDR_DESC,
                                  measurementDuration))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCTRecord_location_present,
                                  S_LCS_MT_CDR_DESC,
                                  location))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCTRecord_routingArea_present,
                                  S_LCS_MT_CDR_DESC,
                                  routingArea))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCTRecord_nodeID_present,
                                  S_LCS_MT_CDR_DESC,
                                  nodeID))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCTRecord_localSequenceNumber_present,
                                  S_LCS_MT_CDR_DESC,
                                  localSequenceNumber))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCTRecord_chChSelectionMode_present,
                                  S_LCS_MT_CDR_DESC,
                                  chChSelectionMode))
                {
                    return -1;
                }
            }
            break;

        case R4_Fin_sgsnLCORecord_chosen:
            {
                R4_Fin_SGSNLCORecord& cdr = pBackCDR->u.sgsnLCORecord;

                if(CHECK_OM_FIELD(R4_Fin_SGSNLCORecord_servedMSISDN_present,
                                  S_LCS_MT_CDR_DESC,
                                  servedMSISDN))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCORecord_sgsnAddress_present,
                                  S_LCS_MT_CDR_DESC,
                                  sgsnAddress))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCORecord_measurementDuration_present,
                                  S_LCS_MT_CDR_DESC,
                                  measurementDuration))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCORecord_location_present,
                                  S_LCS_MT_CDR_DESC,
                                  location))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCORecord_routingArea_present,
                                  S_LCS_MT_CDR_DESC,
                                  routingArea))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCORecord_nodeID_present,
                                  S_LCS_MT_CDR_DESC,
                                  nodeID))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCORecord_localSequenceNumber_present,
                                  S_LCS_MT_CDR_DESC,
                                  localSequenceNumber))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCORecord_chChSelectionMode_present,
                                  S_LCS_MT_CDR_DESC,
                                  chChSelectionMode))
                {
                    return -1;
                }
            }
            break;

        case R4_Fin_sgsnLCNRecord_chosen:
            {
                R4_Fin_SGSNLCNRecord& cdr = pBackCDR->u.sgsnLCNRecord;
                
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCNRecord_sgsnAddress_present,
                                  S_LCS_MT_CDR_DESC,
                                  sgsnAddress))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCNRecord_measurementDuration_present,
                                  S_LCS_MT_CDR_DESC,
                                  measurementDuration))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCNRecord_location_present,
                                  S_LCS_MT_CDR_DESC,
                                  location))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCNRecord_routingArea_present,
                                  S_LCS_MT_CDR_DESC,
                                  routingArea))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCNRecord_nodeID_present,
                                  S_LCS_MT_CDR_DESC,
                                  nodeID))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCNRecord_localSequenceNumber_present,
                                  S_LCS_MT_CDR_DESC,
                                  localSequenceNumber))
                {
                    return -1;
                }
                if(CHECK_OM_FIELD(R4_Fin_SGSNLCNRecord_chChSelectionMode_present,
                                  S_LCS_MT_CDR_DESC,
                                  chChSelectionMode))
                {
                    return -1;
                }
            }
            break;

        default:
            break;
        }
    }

    return 0;
}

extern "C" IGWB_Svc_Export 
int CDRFilterAPNNI(const void* pABill)
{
    static LIST<STRING> APNNIList;
    static LIST<STRING> SGSNAddressList;
    static LIST<STRING> GGSNAddressList;

    char szAddress[128];
    LIST<STRING>::iterator itAPNNI;
    LIST<STRING>::iterator itSGSNAddress;
    LIST<STRING>::iterator itGGSNAddress;

    if(0 == APNNIList.size())
    {
        FILE* fp = NULL;
        fp = fopen("./config/distination/APNNI.lst", "rt");
        if (fp != NULL)
        {
            char szLine[1024] = "";
            while (!feof(fp))
            {
                szLine[0] = '\0';
                fgets(szLine, sizeof(szLine), fp);
                StringLRTrim(szLine);
                if (strlen(szLine))
                {
                    APNNIList.push_back(szLine);
                }
            }
            fclose(fp);
            fp = NULL;
        }
        
        fp = fopen("./config/distination/SGSNAddress.lst", "rt");
        if (fp != NULL)
        {
            char szLine[1024] = "";
            while (!feof(fp))
            {
                szLine[0] = '\0';
                fgets(szLine, sizeof(szLine), fp);
                StringLRTrim(szLine);
                if (strlen(szLine))
                {
                    SGSNAddressList.push_back(szLine);
                }
            }
            fclose(fp);
            fp = NULL;
        }
        
        fp = fopen("./config/distination/GGSNAddress.lst", "rt");
        if (fp != NULL)
        {
            char szLine[1024] = "";
            while (!feof(fp))
            {
                szLine[0] = '\0';
                fgets(szLine, sizeof(szLine), fp);
                StringLRTrim(szLine);
                {
                    
                    GGSNAddressList.push_back(szLine);
                }
            }
            fclose(fp);
            fp = NULL;
        }        
    }
    
    R4_Org_CallEventRecord* pR4Bill;
    pR4Bill = (R4_Org_CallEventRecord*)pABill;
    
    //对于S-CDR，如果GGSNAddress不为本地GGSN，则表示为漫游情况
    if (R4_Org_sgsnPDPRecord_chosen == pR4Bill->choice)
    {
        itAPNNI = APNNIList.begin();
        while (itAPNNI != APNNIList.end())
        {
            if (StringiCmp(pR4Bill->u.sgsnPDPRecord.accessPointNameNI, (*itAPNNI).c_str()))
            {
                if (0 < R43GIPAddrToStr(&pR4Bill->u.sgsnPDPRecord.ggsnAddressUsed, 
                    szAddress, 
                    sizeof(szAddress)))
                {
                    itGGSNAddress = GGSNAddressList.begin();
                    while (itGGSNAddress != GGSNAddressList.end())
                    {
                        //非漫游话单，则免费
                        if (0 == strcmp(szAddress, (*itGGSNAddress).c_str()))
                        {
                            return -1;
                        }
                        itGGSNAddress++;
                    }
                }
                
                break;
            }
            
            itAPNNI++;
        }
    }
    //对于G-CDR，如果SGSNAddress不为本地SGSN，则表示为漫游情况
    else if(R4_Org_ggsnPDPRecord_chosen == pR4Bill->choice)
    {
        itAPNNI = APNNIList.begin();
        while (itAPNNI != APNNIList.end())
        {
            if (StringiCmp(pR4Bill->u.ggsnPDPRecord.accessPointNameNI, (*itAPNNI).c_str()))
            {
                //尽管G-CDR中的SGSNAddress为链表，但是暂时只考虑一个节点的情况
                if (0 < R43GIPAddrToStr(&pR4Bill->u.ggsnPDPRecord.sgsnAddress->value, 
                    szAddress, 
                    sizeof(szAddress)))
                {
                    itSGSNAddress = SGSNAddressList.begin();
                    while (itSGSNAddress != SGSNAddressList.end())
                    {
                        //非漫游话单，则免费
                        if (0 == strcmp(szAddress, (*itSGSNAddress).c_str()))
                        {
                            return -1;
                        }
                        itSGSNAddress++;
                    }
                }
                
                break;
            }
            
            itAPNNI++;
        }
    }
    
    return 0;
}