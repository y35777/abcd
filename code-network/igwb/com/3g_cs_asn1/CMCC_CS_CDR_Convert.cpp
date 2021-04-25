#include "../frame/msg_proc.h"
#include "CMCC_CS_CDR_Convert.h"
#include "../toolbase/field_filter.h"

//by    杨善    2004-6-2    对应问题单SWPD05614
#include "../toolbase/frame_tool.h"
//the end  对应问题单SWPD05614

#include "../include/toolbox.h"
#include <time.h>
#include "StaticMemMnger.h"

extern CStaticMemMnger* g_pAsnBuffForCMCC;         //ASN1编码前的缓冲区

namespace CMCC_CS_R4_CDR_CONVERT
{
/*****************************************************************************************
函数功能:	转换一张话二进制话单为ASN.1编码前的话单结构
函数输入：
	pSrcBill	:	一张二进制话单指针
	nLen		:	pSrcBill指针内容长度
	pEventBill	:	多张Event话单指针
	nEventNum	:	Event话单的张数
函数输出：
	pDestBill	:	ASN.1编码前的话单指针
函数返回:
	TRUE		:	转换成功
	FALSE		:	转换失败
抛出异常:
	无
******************************************************************************************/
BOOL ConvertABill(const BYTE* pSrcBill, const int nLen, 
                  const BYTE* pEventBill, const int nEventNum, 
                  CMCC_CS_CallEventRecord* pDestBill)
{
    //by    杨善    2004-6-2    对应问题单SWPD05614
    if ((pSrcBill == NULL) || (nLen != sizeof(MSC_Bill_Report)))
	{
        /*TRACE(MTS_BACKSAVE_PATCH, "Error, Input bill len is %d, need Len is %d", 
              nLen, sizeof(Gnl_Bill_Report));
        */
        return FALSE;
	}
    //the end  对应问题单SWPD05614
    
    if (g_pAsnBuffForCMCC == NULL)
    {
        return FALSE;
    }

	BOOL bResult = FALSE;
    
    //by    杨善    2004-6-2    对应问题单SWPD05614
	switch (((MSC_Bill_Report*)pSrcBill)->bill_type)	//话单的第18个字节为BillType
    //the end  对应问题单SWPD05614
	{
		//移动始发、紧急呼叫、前转话单
		case MOC:
		case EC:
		{
			bResult = ConvertABill_MOC(pSrcBill, nLen, pEventBill, nEventNum, pDestBill);
			break;
		}

        case CFW:
        {
            bResult = ConvertABill_CFW(pSrcBill, nLen, pEventBill, nEventNum, pDestBill);
            break;
        }

		//移动终结话单
		case MTC:
		{
			bResult = ConvertABill_MTC(pSrcBill, nLen, pEventBill, nEventNum, pDestBill);
			break;
		}

		//出关口话单
		case GATEWAY_OUTGOING:
		{
			bResult = ConvertABill_GWO(pSrcBill, nLen, pEventBill, nEventNum, pDestBill);
			break;
		}

		//入关口话单
		case GATEWAY_INCOMING:
		{
			bResult = ConvertABill_GWI(pSrcBill, nLen, pEventBill, nEventNum, pDestBill);
			break;
		}

		//汇接话单
		case TRANSIT:
		{
			bResult = ConvertABill_TRANSIT(pSrcBill, nLen, pEventBill, nEventNum, pDestBill);
			break;
		}

		//补充业务话单
		case SS_ACT:
		{
			bResult = ConvertABill_SS_ACT(pSrcBill, nLen, pEventBill, nEventNum, pDestBill);
			break;
		}

		//短消息始发话单
		case MO_SMS:
		{
			bResult = ConvertABill_MO_SMS(pSrcBill, nLen, pEventBill, nEventNum, pDestBill);
			break;
		}
		
		//短消息终结话单
		case MT_SMS:
		{
			bResult = ConvertABill_MT_SMS(pSrcBill, nLen, pEventBill, nEventNum, pDestBill);
			break;
		}

		//终结CAMEL访问记录
		case TCAMEL:
		{
			bResult = ConvertABill_TCAMEL(pSrcBill, nLen, pEventBill, nEventNum, pDestBill);
			break;
		}

		//HLR访问话单
		case QUERY_HLR:
		{
			bResult = ConvertABill_QUERY_HLR(pSrcBill, nLen, pEventBill, nEventNum, pDestBill);
			break;
		}

		//漫游话单
		case ROAM:
		{
			bResult = ConvertABill_ROAM(pSrcBill, nLen, pEventBill, nEventNum, pDestBill);
			break;
		}

		//公用设备话单
		case CommonEquip:
		{
			bResult = ConvertABill_COMMON_EQUIP(pSrcBill, nLen, pEventBill, nEventNum, pDestBill);
			break;
		}

		//定位业务话单
		case LCS:
		{

			if (((LCS_Bill_Report*)pSrcBill)->lcs_bill_type == MT_LR)
			{
				bResult = ConvertABill_MT_LR(pSrcBill, nLen, pEventBill, nEventNum, pDestBill);
			}
			else if (((LCS_Bill_Report*)pSrcBill)->lcs_bill_type == MO_LR)
			{
				bResult = ConvertABill_MO_LR(pSrcBill, nLen, pEventBill, nEventNum, pDestBill);
			}
			else if (((LCS_Bill_Report*)pSrcBill)->lcs_bill_type == NI_LR)
			{
				bResult = ConvertABill_NI_LR(pSrcBill, nLen, pEventBill, nEventNum, pDestBill);
			}
			else
			{
                //Added by ZhengYuqun 2004-03-18 SWPD04113
                SInnerAlarm sAlarm;
                sAlarm.yAlarmType = AT_EVENT;
                sAlarm.uAlarmID = ALARM_ID_ABNORMAL_BILL;
                sAlarm.yAlarmPara[0] = ALARM_PARA_TYPE_ERR;

                //by    杨善    2004-6-2    对应问题单SWPD05614
                CFrameTools::SendAlarm(&sAlarm);
                //the end  对应问题单SWPD05614

                //End SWPD04113
				bResult = FALSE;
			}

			break;
		}

		//其它类型的话单不转换
		default:
		{
            //Added by ZhengYuqun 2004-03-18 SWPD04113
            SInnerAlarm sAlarm;
            sAlarm.yAlarmType = AT_EVENT;
            sAlarm.uAlarmID = ALARM_ID_ABNORMAL_BILL;
            sAlarm.yAlarmPara[0] = ALARM_PARA_TYPE_ERR;
            
            //by    杨善    2004-6-2    对应问题单SWPD05614
            CFrameTools::SendAlarm(&sAlarm);
            //the end  对应问题单SWPD05614

            //End SWPD04113
			bResult = FALSE;
			break;
		}
	}

	return bResult;
}

BOOL ConvertABill_CHECK_IMEI(const BYTE* pSrcBill, const int nLen, CMCC_CS_ObservedIMEITicket* pDestBill)
{
    CheckIMEI_Bill_Report* pCheckIMEI = (CheckIMEI_Bill_Report *)pSrcBill;
    pDestBill->bit_mask = 0;
    
    pDestBill->servedIMEI.length = BCDtoRBCD(pDestBill->servedIMEI.value, pCheckIMEI->servedIMEI, 8);

    
    pDestBill->imeiStatus = (CMCC_CS_IMEIStatus)pCheckIMEI->IMEIStatus;

    pDestBill->servedIMSI.length = BCDtoRBCD(pDestBill->servedIMSI.value,pCheckIMEI->servedIMSI, 8); 


    pDestBill->servedMSISDN.length = 0;
    if (GET_CDR_FIELD_FILTER(servedMSISDN,CheckIMEIReport))
    {
        int nMSISDNLen = (pCheckIMEI->servedMSISDN_BCDlen + 1)/2;
        if (nMSISDNLen > 0)
        {
            pDestBill->bit_mask |= CMCC_CS_ObservedIMEITicket_servedMSISDN_present;
            pDestBill->servedMSISDN.length = 
                min(sizeof(pDestBill->servedMSISDN.value),nMSISDNLen + 1);
            pDestBill->servedMSISDN.value[0] = *(pCheckIMEI->servedMSISDN - 2) | 0x80;
            BCDtoRBCD(pDestBill->servedMSISDN.value + 1, pCheckIMEI->servedMSISDN,
                pDestBill->servedMSISDN.length - 1);
        }
    }

    int nRecordingEntityLen = (pCheckIMEI->recordingEntity_BCDlen + 1)/2;
    pDestBill->recordingEntity.length =
        min(sizeof(pDestBill->recordingEntity.value),nRecordingEntityLen + 1);
    if (pDestBill->recordingEntity.length > 0)
    {      
        pDestBill->recordingEntity.value[0] = *(pCheckIMEI->recordingEntity - 2) | 0x80;
        BCDtoRBCD(pDestBill->recordingEntity.value + 1,pCheckIMEI->recordingEntity,
            pDestBill->recordingEntity.length - 1);
    }

    if (pCheckIMEI->checkIMEI_year != 0xFF)
    {
        ConvertTimeStamp(pDestBill->eventTime,&(pCheckIMEI->checkIMEI_year));
    }

    pDestBill->location.locationAreaCode.length = 2;
    memcpy(pDestBill->location.locationAreaCode.value, pCheckIMEI->served_location_LAI + 3 , 2);
    pDestBill->location.cellIdentifier.length = 2 ;
    memcpy(pDestBill->location.cellIdentifier.value, pCheckIMEI->served_location_cellID, 2);            

    if (GET_CDR_FIELD_FILTER(imeiCheckEvent,CheckIMEIReport))
    {
        if (pCheckIMEI->Event != 0xFF)
        {
            pDestBill->bit_mask |= CMCC_CS_imeiCheckEvent_present;
            pDestBill->imeiCheckEvent = pCheckIMEI->Event;
        }
    }

    if (GET_CDR_FIELD_FILTER(callReference,CheckIMEIReport))
    {
        pDestBill->bit_mask |= CMCC_CS_ObservedIMEITicket_callReference_present;
        GetCallReference(pDestBill->callReference,
            pCheckIMEI->ModuleNum, pCheckIMEI->count);
    }

    pDestBill->recordExtensions = NULL;   
    if (GET_CDR_FIELD_FILTER(recordExtensions,CheckIMEIReport)) // 暂时没有
    {
    }
    
    pDestBill->orgMSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgMSCId,CheckIMEIReport))   //orgMSCID
    {
        if (pCheckIMEI->msc_id_len != 0)
        {
             pDestBill->bit_mask |= CMCC_CS_ObservedIMEITicket_orgMSCId_present;
            pDestBill->orgMSCId.length = min(3,pCheckIMEI->msc_id_len);
            memcpy(pDestBill->orgMSCId.value,pCheckIMEI->orgMSCId,pDestBill->orgMSCId.length);
       }
    }   
    
    return TRUE;
}

BOOL ConvertABill_GWI(const BYTE* pSrcBill, const int nLen,
                      const BYTE* pEventBill, const int nEventNum, 
                      CMCC_CS_CallEventRecord* pDestBill)
{
	Gateway_Bill_Report* pGwiBill = (Gateway_Bill_Report *)pSrcBill;	

	pDestBill->choice = CMCC_CS_incGatewayRecord_chosen;
    memset(pDestBill->u.incGatewayRecord.bit_mask, 0, sizeof(pDestBill->u.incGatewayRecord.bit_mask));

    if (GET_CDR_FIELD_FILTER(recordType, IncGatewayRecord))
    {
	    pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_recordType_present_byte] 
            |= CMCC_CS_IncGatewayRecord_recordType_present;
        pDestBill->u.incGatewayRecord.recordType = CMCC_CS_CallEventRecordType_incGatewayRecord;
    }

	pDestBill->u.incGatewayRecord.callingNumber.length = 0;
    pDestBill->u.incGatewayRecord.callingNumber.value = NULL;
	if (GET_CDR_FIELD_FILTER(callingNumber, IncGatewayRecord))		//callingNumber
	{
		int nCallerLen = (pGwiBill->callingNumber_BCDlen + 1) / 2;
        if (nCallerLen > 0)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_callingNumber_present_byte] |= 
                CMCC_CS_IncGatewayRecord_callingNumber_present;
		    pDestBill->u.incGatewayRecord.callingNumber.length = nCallerLen + 1;
		    pDestBill->u.incGatewayRecord.callingNumber.value = new (g_pAsnBuffForCMCC)BYTE[nCallerLen + 1];
            pDestBill->u.incGatewayRecord.callingNumber.value[0] = *(pGwiBill->callingNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.incGatewayRecord.callingNumber.value + 1, pGwiBill->callingNumber, nCallerLen);
        }
	}

    pDestBill->u.incGatewayRecord.calledNumber.length = 0;
    pDestBill->u.incGatewayRecord.calledNumber.value = NULL;
	if (GET_CDR_FIELD_FILTER(calledNumber, IncGatewayRecord))
    {
        int nCalledLen = (pGwiBill->calledNumber_BCDlen + 1) / 2;
        if (nCalledLen > 0)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_calledNumber_present_byte] |= 
                CMCC_CS_IncGatewayRecord_calledNumber_present;
            pDestBill->u.incGatewayRecord.calledNumber.length = nCalledLen + 1;
            pDestBill->u.incGatewayRecord.calledNumber.value = new (g_pAsnBuffForCMCC)BYTE[nCalledLen + 1];
            pDestBill->u.incGatewayRecord.calledNumber.value[0] = *(pGwiBill->calledNumber - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.incGatewayRecord.calledNumber.value + 1, pGwiBill->calledNumber, nCalledLen);
        }
    }

    pDestBill->u.incGatewayRecord.recordingEntity.length = 0;
    if (GET_CDR_FIELD_FILTER(recordingEntity, IncGatewayRecord))
    {
        int nLocalMSCLen = (pGwiBill->recordingEntity_BCDlen + 1) / 2;
        if (nLocalMSCLen > 0)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_recordingEntity_present_byte] |= 
                CMCC_CS_IncGatewayRecord_recordingEntity_present;
            pDestBill->u.incGatewayRecord.recordingEntity.length = 
                min(sizeof(pDestBill->u.incGatewayRecord.recordingEntity.value), nLocalMSCLen + 1);
            pDestBill->u.incGatewayRecord.recordingEntity.value[0] = *(pGwiBill->recordingEntity - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.incGatewayRecord.recordingEntity.value + 1, pGwiBill->recordingEntity, 
                pDestBill->u.incGatewayRecord.recordingEntity.length - 1);
        }
    }

    pDestBill->u.incGatewayRecord.mscIncomingROUTE.u.rOUTEName = NULL;
	if (GET_CDR_FIELD_FILTER(mscIncomingROUTE, IncGatewayRecord))		//mscIncomingROUTE
	{        
        if (pGwiBill->mscIncomingROUTE_ind == SHOW_ROUTE_NO)
        {
            if (pGwiBill->mscIncomingROUTE_ROUTENumber != 0xFFFF)
            {
                pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_mscIncomingROUTE_present_byte] |= 
                    CMCC_CS_IncGatewayRecord_mscIncomingROUTE_present;
                pDestBill->u.incGatewayRecord.mscIncomingROUTE.choice = CMCC_CS_rOUTENumber_chosen;
                pDestBill->u.incGatewayRecord.mscIncomingROUTE.u.rOUTENumber = pGwiBill->mscIncomingROUTE_ROUTENumber;
            }
        }
        else if (pGwiBill->mscIncomingROUTE_ind == SHOW_ROUTE_NAME)
        {
            if (*(pGwiBill->mscIncomingROUTE_ROUTEName) != 0x20)
            {
                pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_mscIncomingROUTE_present_byte] |= 
                    CMCC_CS_IncGatewayRecord_mscIncomingROUTE_present;
                pDestBill->u.incGatewayRecord.mscIncomingROUTE.choice = CMCC_CS_rOUTEName_chosen;
                pDestBill->u.incGatewayRecord.mscIncomingROUTE.u.rOUTEName = new (g_pAsnBuffForCMCC)char[11];
                memcpy(pDestBill->u.incGatewayRecord.mscIncomingROUTE.u.rOUTEName, pGwiBill->mscIncomingROUTE_ROUTEName, 10);
                pDestBill->u.incGatewayRecord.mscIncomingROUTE.u.rOUTEName[10] = '\0';
            }
        }
	}

    pDestBill->u.incGatewayRecord.mscOutgoingROUTE.u.rOUTEName = NULL;
	if (GET_CDR_FIELD_FILTER(mscOutgoingROUTE, IncGatewayRecord))		//mscOutgongingTKGP
	{
		if (pGwiBill->mscOutgoingROUTE_ind == SHOW_ROUTE_NO)
        {
            if (pGwiBill->mscOutgoingROUTE_ROUTENumber != 0xFFFF)
            {
                pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_mscOutgoingROUTE_present_byte] |= 
                    CMCC_CS_IncGatewayRecord_mscOutgoingROUTE_present;
		        pDestBill->u.incGatewayRecord.mscOutgoingROUTE.choice = CMCC_CS_rOUTENumber_chosen;
		        pDestBill->u.incGatewayRecord.mscOutgoingROUTE.u.rOUTENumber = pGwiBill->mscOutgoingROUTE_ROUTENumber;
            }
        }
        else if (pGwiBill->mscOutgoingROUTE_ind == SHOW_ROUTE_NAME)
        {
            if (*(pGwiBill->mscOutgoingROUTE_ROUTEName) != 0x20)
            {
                pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_mscOutgoingROUTE_present_byte] |= 
                    CMCC_CS_IncGatewayRecord_mscOutgoingROUTE_present;
		        pDestBill->u.incGatewayRecord.mscOutgoingROUTE.choice = CMCC_CS_rOUTEName_chosen;
		        pDestBill->u.incGatewayRecord.mscOutgoingROUTE.u.rOUTEName = new (g_pAsnBuffForCMCC)char[11];
                memcpy(pDestBill->u.incGatewayRecord.mscOutgoingROUTE.u.rOUTEName, pGwiBill->mscOutgoingROUTE_ROUTEName, 10);
                pDestBill->u.incGatewayRecord.mscOutgoingROUTE.u.rOUTEName[10] = '\0';
            }
        }
	}

	if (GET_CDR_FIELD_FILTER(seizureTime, IncGatewayRecord))		//seizureTime
	{
		if (pGwiBill->causeForTerm == unsuccessfulCallAttempt)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_seizureTime_present_byte] |= 
                CMCC_CS_IncGatewayRecord_seizureTime_present;
            ConvertTimeStamp(pDestBill->u.incGatewayRecord.seizureTime, &(pGwiBill->initial_year));
        }
	}    

	if (GET_CDR_FIELD_FILTER(answerTime, IncGatewayRecord))		//answerTimestamp
	{
		if (pGwiBill->causeForTerm != unsuccessfulCallAttempt)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_answerTime_present_byte] |= 
                CMCC_CS_IncGatewayRecord_answerTime_present; 
		    ConvertTimeStamp(pDestBill->u.incGatewayRecord.answerTime, &(pGwiBill->initial_year));
        }
	}
	
	if (GET_CDR_FIELD_FILTER(releaseTime, IncGatewayRecord))	//releaseTimestamp
	{
		if (pGwiBill->end_year != 0xFF)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_releaseTime_present_byte] |= 
                CMCC_CS_IncGatewayRecord_releaseTime_present;
	    	ConvertTimeStamp(pDestBill->u.incGatewayRecord.releaseTime, &(pGwiBill->end_year));			
        }
	}

    if (GET_CDR_FIELD_FILTER(callDuration, IncGatewayRecord))
    {
        pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_callDuration_present_byte] |= 
            CMCC_CS_IncGatewayRecord_callDuration_present; 
        pDestBill->u.incGatewayRecord.callDuration = pGwiBill->callDuration;						
    }

    if (GET_CDR_FIELD_FILTER(causeForTerm, IncGatewayRecord))
    {
        pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_causeForTerm_present_byte] |= 
            CMCC_CS_IncGatewayRecord_causeForTerm_present; 
        pDestBill->u.incGatewayRecord.causeForTerm = pGwiBill->causeForTerm;
    }

	if (GET_CDR_FIELD_FILTER(diagnostics, IncGatewayRecord))			//diagnostics
	{
        pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_diagnostics_present_byte] |= 
            CMCC_CS_IncGatewayRecord_diagnostics_present;
		if (pGwiBill->diagnostics.CauseType == 0)
		{
			pDestBill->u.incGatewayRecord.diagnostics.choice = CMCC_CS_gsm0408Cause_chosen;
			pDestBill->u.incGatewayRecord.diagnostics.u.gsm0408Cause = pGwiBill->diagnostics.Cause.gsm0408Cause;
		}
		else if (pGwiBill->diagnostics.CauseType == 1)
		{
			pDestBill->u.incGatewayRecord.diagnostics.choice = CMCC_CS_gsm0902MapErrorValue_chosen;
			pDestBill->u.incGatewayRecord.diagnostics.u.gsm0902MapErrorValue = pGwiBill->diagnostics.Cause.gsm0902MapErrorValue;
		}
		else if (pGwiBill->diagnostics.CauseType == 2)
		{
			pDestBill->u.incGatewayRecord.diagnostics.choice = CMCC_CS_ccittQ767Cause_chosen;
			pDestBill->u.incGatewayRecord.diagnostics.u.ccittQ767Cause = pGwiBill->diagnostics.Cause.ccittQ767Cause;
		}
		else
		{
			pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_diagnostics_present_byte] &=
                ~CMCC_CS_IncGatewayRecord_diagnostics_present;
		}
	}

    if (GET_CDR_FIELD_FILTER(callReference, IncGatewayRecord))
    {
        if (pGwiBill->call_reference_len != 0)
        {
            int nCallReferenceLen = min(pGwiBill->call_reference_len,
                sizeof(pDestBill->u.incGatewayRecord.callReference.value));
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_callReference_present_byte] |= 
                CMCC_CS_IncGatewayRecord_callReference_present;                    
            pDestBill->u.incGatewayRecord.callReference.length = nCallReferenceLen;
            memcpy(pDestBill->u.incGatewayRecord.callReference.value, 
                &(pGwiBill->call_reference[sizeof(pGwiBill->call_reference) - pGwiBill->call_reference_len]), nCallReferenceLen);   
        }
        else
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_callReference_present_byte] |= 
                CMCC_CS_IncGatewayRecord_callReference_present;  
            GetCallReference(pDestBill->u.incGatewayRecord.callReference,
                pGwiBill ->ModuleNum, pGwiBill->count);
        }
    }

	if (GET_CDR_FIELD_FILTER(sequenceNumber, IncGatewayRecord))		//部分话单序列号
	{
		if (pGwiBill->record_type != SINGLE_BILL)
		{
			pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_sequenceNumber_present_byte] |= 
                CMCC_CS_IncGatewayRecord_sequenceNumber_present;
			pDestBill->u.incGatewayRecord.sequenceNumber = pGwiBill->sequence;
		}
	}

	pDestBill->u.incGatewayRecord.recordExtensions = NULL;
	if (GET_CDR_FIELD_FILTER(recordExtensions, IncGatewayRecord))		//recordExtensions(目前不支持)
	{
		//pDestBill->u.incGatewayRecord.bit_mask |= CMCC_CS_IncGatewayRecord_recordExtensions_present;
	}  

    pDestBill->u.incGatewayRecord.roamingNumber.length = 0;
    if (GET_CDR_FIELD_FILTER(roamingNumber, IncGatewayRecord))  //漫游号码
	{
		int nRoamingLen = (pGwiBill->roamingNumber_BCDlen + 1) / 2;
        if (nRoamingLen > 0)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_roamingNumber_present_byte] |= 
                CMCC_CS_IncGatewayRecord_roamingNumber_present;
		    pDestBill->u.incGatewayRecord.roamingNumber.length = 
                min(sizeof(pDestBill->u.incGatewayRecord.roamingNumber.value), nRoamingLen + 1);
            pDestBill->u.incGatewayRecord.roamingNumber.value[0] = *(pGwiBill->roamingNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.incGatewayRecord.roamingNumber.value + 1, pGwiBill->roamingNumber, 
                pDestBill->u.incGatewayRecord.roamingNumber.length - 1);
        }
    }

    if (GET_CDR_FIELD_FILTER(mscIncomingCircuit, IncGatewayRecord))		//mscIncomingCircuit
	{
        if (pGwiBill->mscIncomingCircuit != 0xFFFF)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_mscIncomingCircuit_present_byte] |= 
                CMCC_CS_IncGatewayRecord_mscIncomingCircuit_present;
            pDestBill->u.incGatewayRecord.mscIncomingCircuit = pGwiBill->mscIncomingCircuit;
        }
    }

    pDestBill->u.incGatewayRecord.orgMSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgMSCId, IncGatewayRecord))
	{
        if (pGwiBill->msc_id_len != 0)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_orgMSCId_present_byte] |= 
                CMCC_CS_IncGatewayRecord_orgMSCId_present;
            pDestBill->u.incGatewayRecord.orgMSCId.length = min(3,pGwiBill->msc_id_len);
            memcpy(pDestBill->u.incGatewayRecord.orgMSCId.value, pGwiBill->orgMSCId, pDestBill->u.incGatewayRecord.orgMSCId.length);
        }
    }

    pDestBill->u.incGatewayRecord.callEmlppPriority.length = 0;
    if (GET_CDR_FIELD_FILTER(callEmlppPriority, IncGatewayRecord))
	{
        if (pGwiBill->callEmlppPriority != 0xF)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_callEmlppPriority_present_byte] |= 
                CMCC_CS_IncGatewayRecord_callEmlppPriority_present;
            pDestBill->u.incGatewayRecord.callEmlppPriority.length = 1;
            pDestBill->u.incGatewayRecord.callEmlppPriority.value[0] = pGwiBill->callEmlppPriority;
        }
    }

    if (GET_CDR_FIELD_FILTER(cUGOutgoingAccessIndicator, IncGatewayRecord))
	{
        if (pGwiBill->CUGOutgoingAccessIndicator != 0)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_cUGOutgoingAccessIndicator_present_byte] |= 
                CMCC_CS_IncGatewayRecord_cUGOutgoingAccessIndicator_present;
            pDestBill->u.incGatewayRecord.cUGOutgoingAccessIndicator = CMCC_CS_cUGCall;
        }
	}

    pDestBill->u.incGatewayRecord.cUGInterlockCode.length = 0;
    if (GET_CDR_FIELD_FILTER(cUGInterlockCode, IncGatewayRecord))
	{
		if (pGwiBill->CUGInterlockCode != 0xFFFFFFFF)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_cUGInterlockCode_present_byte] |= 
                CMCC_CS_IncGatewayRecord_cUGInterlockCode_present;
            pDestBill->u.incGatewayRecord.cUGInterlockCode.length = 4;
            memcpy(pDestBill->u.incGatewayRecord.cUGInterlockCode.value, &(pGwiBill->CUGInterlockCode), 4);
        }
	}

    if (GET_CDR_FIELD_FILTER(partialRecordType, IncGatewayRecord))      //cause_for_partial_record
    {
        if (pGwiBill->cause_for_partial_record != 0xff)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_partialRecordType_present_byte] |=
                CMCC_CS_IncGatewayRecord_partialRecordType_present;
            pDestBill->u.incGatewayRecord.partialRecordType = (CMCC_CS_PartialRecordType)pGwiBill->cause_for_partial_record;
        }
     }
    
    pDestBill->u.incGatewayRecord.iSDN_BC.length = 0;
    pDestBill->u.incGatewayRecord.iSDN_BC.value = NULL;
    if (GET_CDR_FIELD_FILTER(iSDN_BC, IncGatewayRecord))      //2
    {
        int niSDNBCLen = (pGwiBill->isdn_bc_len + 1)/2;
        if (niSDNBCLen < 0)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_iSDN_BC_present_byte] |=
                CMCC_CS_iSDN_BC_present;
            pDestBill->u.incGatewayRecord.iSDN_BC.length = niSDNBCLen + 1;
            pDestBill->u.incGatewayRecord.iSDN_BC.value = new (g_pAsnBuffForCMCC)BYTE[niSDNBCLen + 1];
            pDestBill->u.incGatewayRecord.iSDN_BC.value[0] = *(pGwiBill->isdn_bc -2) |0x80;
            BCDtoRBCD(pDestBill->u.incGatewayRecord.iSDN_BC.value + 1,pGwiBill->isdn_bc,
                pDestBill->u.incGatewayRecord.iSDN_BC.length - 1);
        }
    }
    
    pDestBill->u.incGatewayRecord.lLC.length = 0;
    pDestBill->u.incGatewayRecord.lLC.value = NULL;
    if (GET_CDR_FIELD_FILTER(lLC, IncGatewayRecord))      //3
    {
        int nLLCLen = (pGwiBill->LLC_len + 1)/2;
        if (nLLCLen < 0)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_lLC_present_byte] |=
                CMCC_CS_lLC_present; 
            pDestBill->u.incGatewayRecord.lLC.length = nLLCLen + 1;
            pDestBill->u.incGatewayRecord.lLC.value = new (g_pAsnBuffForCMCC)BYTE[nLLCLen + 1];
            pDestBill->u.incGatewayRecord.lLC.value[0] = *(pGwiBill->LLC -2) |0x80;
            BCDtoRBCD(pDestBill->u.incGatewayRecord.lLC.value + 1,pGwiBill->LLC,
                pDestBill->u.incGatewayRecord.lLC.length - 1);
        }     
    }
    
    pDestBill->u.incGatewayRecord.hLC.length = 0;
    pDestBill->u.incGatewayRecord.hLC.value = NULL;
    if (GET_CDR_FIELD_FILTER(hLC, IncGatewayRecord))      //4
    {
        int nHLCLen = (pGwiBill->HLC_len + 1)/2;
        if (nHLCLen < 0)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_hLC_present_byte] |=
                CMCC_CS_hLC_present;
            pDestBill->u.incGatewayRecord.hLC.length = nHLCLen + 1;
            pDestBill->u.incGatewayRecord.hLC.value = new (g_pAsnBuffForCMCC)BYTE[nHLCLen + 1];
            pDestBill->u.incGatewayRecord.hLC.value[0] = *(pGwiBill->HLC -2) |0x80;
            BCDtoRBCD(pDestBill->u.incGatewayRecord.lLC.value + 1,pGwiBill->HLC,
                pDestBill->u.incGatewayRecord.hLC.length - 1);
        }     
    }
 
    if (GET_CDR_FIELD_FILTER(basicService, IncGatewayRecord))	//业务码
    {
        if (pGwiBill->TBS < 2 && pGwiBill->ServiceCode != 0xFF)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_basicService_present_byte] |= 
                CMCC_CS_IncGatewayRecord_basicService_present;
            ConvertBasicServiceCode(pDestBill->u.incGatewayRecord.basicService, 
                pGwiBill->TBS, pGwiBill->ServiceCode);
        }
    }
    
    if (GET_CDR_FIELD_FILTER(additionalChgInfo, IncGatewayRecord))		//附加计费信息
    {
        if (pGwiBill->chargeIndicator >= 0 && pGwiBill->chargeIndicator < 3)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_additionalChgInfo_present_byte] |=
                CMCC_CS_IncGatewayRecord_additionalChgInfo_present;
            pDestBill->u.incGatewayRecord.additionalChgInfo.bit_mask = CMCC_CS_chargeIndicator_present;
            pDestBill->u.incGatewayRecord.additionalChgInfo.chargeIndicator = pGwiBill->chargeIndicator;
        }
    }
 
    if (GET_CDR_FIELD_FILTER(chargedParty,IncGatewayRecord) )
    {
        if (pGwiBill->chargedParty == 0 || pGwiBill->chargedParty == 1)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_chargedParty_present_byte] |=
                CMCC_CS_IncGatewayRecord_chargedParty_present;
            pDestBill->u.incGatewayRecord.chargedParty = (CMCC_CS_ChargedParty)pGwiBill->chargedParty;
        }
    }

    pDestBill->u.incGatewayRecord.originalCalledNumber.length = 0;
    pDestBill->u.incGatewayRecord.originalCalledNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(originalCalledNumber,IncGatewayRecord))
    {
        int nOriginalCalledLen = (pGwiBill->originalCalledNumber_BCDlen + 1)/2;
        if (nOriginalCalledLen > 0 )
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_originalCalledNumber_present_byte] |=
                CMCC_CS_IncGatewayRecord_originalCalledNumber_present;
            pDestBill->u.incGatewayRecord.originalCalledNumber.length = nOriginalCalledLen + 1;
            pDestBill->u.incGatewayRecord.originalCalledNumber.value = new (g_pAsnBuffForCMCC)BYTE[nOriginalCalledLen + 1];
            pDestBill->u.incGatewayRecord.originalCalledNumber.value[0] = *(pGwiBill->originalCalledNumber - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.incGatewayRecord.originalCalledNumber.value + 1,pGwiBill->originalCalledNumber,
                pDestBill->u.incGatewayRecord.originalCalledNumber.length - 1);
        }
    }

    if (GET_CDR_FIELD_FILTER(rateIndication, IncGatewayRecord))		//rateIndication
    {
     /*
        if (pGwiBill->rateIndication != 0x00)
             {
                 pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_rateIndication_present_byte] |=
                     CMCC_CS_IncGatewayRecord_rateIndication_present;
                 pDestBill->u.incGatewayRecord.rateIndication.length = 1;
                 pDestBill->u.incGatewayRecord.rateIndication.value[0] = pGwiBill->rateIndication;
             }*/
     
    }   
    
    pDestBill->u.incGatewayRecord.eaSubscriberInfo.length = 0;
    if (GET_CDR_FIELD_FILTER(eaSubscriberInfo, IncGatewayRecord)) //此域无法得到
    {
        if (pGwiBill->eaSubscriberInfo[0] != 0xff &&
            pGwiBill->eaSubscriberInfo[1] != 0xff &&
            pGwiBill->eaSubscriberInfo[2] != 0xff)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_eaSubscriberInfo_present_byte] |= 
                CMCC_CS_IncGatewayRecord_eaSubscriberInfo_present; 
            pDestBill->u.incGatewayRecord.eaSubscriberInfo.length = 3;
            memcpy(pDestBill->u.incGatewayRecord.eaSubscriberInfo.value,pGwiBill->eaSubscriberInfo,3);
        }        
    }
    
    pDestBill->u.incGatewayRecord.selectedCIC.length = 0;    
    if (GET_CDR_FIELD_FILTER(selectedCIC,IncGatewayRecord))
    {
        if (pGwiBill->selectedCIC[0] != 0xff &&
            pGwiBill->selectedCIC[1] != 0xff &&
            pGwiBill->selectedCIC[2] != 0xff)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_selectedCIC_present_byte] |=
                CMCC_CS_IncGatewayRecord_selectedCIC_present; 
            pDestBill->u.incGatewayRecord.selectedCIC.length = 3;
            memcpy(pDestBill->u.incGatewayRecord.selectedCIC.value,pGwiBill->selectedCIC,3);
        }
    }
    
    if (GET_CDR_FIELD_FILTER(cUGOutgoingAccessIndicator, IncGatewayRecord))
    {
        if (pGwiBill->CUGOutgoingAccessIndicator != 0)
        {
            pDestBill->u.incGatewayRecord.bit_mask[CMCC_CS_IncGatewayRecord_cUGOutgoingAccessIndicator_present_byte] |= 
                CMCC_CS_IncGatewayRecord_cUGOutgoingAccessIndicator_present;
            pDestBill->u.incGatewayRecord.cUGOutgoingAccessIndicator = CMCC_CS_cUGCall;
        }   
    }
    
    if (GET_CDR_FIELD_FILTER(cUGIncomingAccessUsed, IncGatewayRecord))
    {
    }
    
	return TRUE;
}

BOOL ConvertABill_GWO(const BYTE* pSrcBill, const int nLen,
                      const BYTE* pEventBill, const int nEventNum, 
                      CMCC_CS_CallEventRecord* pDestBill)
{
	Gateway_Bill_Report* pGwoBill = (Gateway_Bill_Report *)pSrcBill;	

	pDestBill->choice = CMCC_CS_outGatewayRecord_chosen;
	pDestBill->u.outGatewayRecord.bit_mask = 0;

    if (GET_CDR_FIELD_FILTER(recordType, OutGatewayRecord))
    {
        pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_recordType_present;
	    pDestBill->u.outGatewayRecord.recordType = CMCC_CS_CallEventRecordType_outGatewayRecord;
    }

	pDestBill->u.outGatewayRecord.callingNumber.length = 0;
    pDestBill->u.outGatewayRecord.callingNumber.value = NULL;
	if (GET_CDR_FIELD_FILTER(callingNumber, OutGatewayRecord))		//callingNumber
	{
		int nCallerLen = (pGwoBill->callingNumber_BCDlen + 1) / 2;
        if (nCallerLen > 0)
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_callingNumber_present;
		    pDestBill->u.outGatewayRecord.callingNumber.length = nCallerLen + 1;
		    pDestBill->u.outGatewayRecord.callingNumber.value = new (g_pAsnBuffForCMCC)BYTE[nCallerLen + 1];
            pDestBill->u.outGatewayRecord.callingNumber.value[0] = *(pGwoBill->callingNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.outGatewayRecord.callingNumber.value + 1, pGwoBill->callingNumber, nCallerLen);
        }
	}

    pDestBill->u.outGatewayRecord.calledNumber.length = 0;
    pDestBill->u.outGatewayRecord.calledNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(calledNumber, OutGatewayRecord))
    {
        int nCalledLen = (pGwoBill->calledNumber_BCDlen + 1) / 2;
        if (nCalledLen > 0)
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_calledNumber_present;
            pDestBill->u.outGatewayRecord.calledNumber.length = nCalledLen + 1;
            pDestBill->u.outGatewayRecord.calledNumber.value = new (g_pAsnBuffForCMCC)BYTE[nCalledLen + 1];
            pDestBill->u.outGatewayRecord.calledNumber.value[0] = *(pGwoBill->calledNumber - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.outGatewayRecord.calledNumber.value + 1, pGwoBill->calledNumber, nCalledLen);
        }
    }

    pDestBill->u.outGatewayRecord.recordingEntity.length = 0;
    if (GET_CDR_FIELD_FILTER(recordingEntity, OutGatewayRecord))
    {
        int nLocalMSCLen = (pGwoBill->recordingEntity_BCDlen + 1) / 2;
        if (nLocalMSCLen > 0)
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_recordingEntity_present;
            pDestBill->u.outGatewayRecord.recordingEntity.length = 
                min(sizeof(pDestBill->u.outGatewayRecord.recordingEntity.value), nLocalMSCLen + 1);
            pDestBill->u.outGatewayRecord.recordingEntity.value[0] = *(pGwoBill->recordingEntity - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.outGatewayRecord.recordingEntity.value + 1, pGwoBill->recordingEntity, 
                pDestBill->u.outGatewayRecord.recordingEntity.length - 1);
        }
    }

    pDestBill->u.outGatewayRecord.mscIncomingROUTE.u.rOUTEName = NULL;
	if (GET_CDR_FIELD_FILTER(mscIncomingROUTE, OutGatewayRecord))		//mscIncomingROUTE
	{
		if (pGwoBill->mscIncomingROUTE_ind == SHOW_ROUTE_NO)
        {
            if (pGwoBill->mscIncomingROUTE_ROUTENumber != 0xFFFF)
            {
                pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_mscIncomingROUTE_present;
		        pDestBill->u.outGatewayRecord.mscIncomingROUTE.choice = CMCC_CS_rOUTENumber_chosen;
		        pDestBill->u.outGatewayRecord.mscIncomingROUTE.u.rOUTENumber = pGwoBill->mscIncomingROUTE_ROUTENumber;        
            }
        }
        else if (pGwoBill->mscIncomingROUTE_ind == SHOW_ROUTE_NAME)
        {
            if (*(pGwoBill->mscIncomingROUTE_ROUTEName) != 0x20) 
            {
                pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_mscIncomingROUTE_present;
		        pDestBill->u.outGatewayRecord.mscIncomingROUTE.choice = CMCC_CS_rOUTEName_chosen;
		        pDestBill->u.outGatewayRecord.mscIncomingROUTE.u.rOUTEName = new (g_pAsnBuffForCMCC)char[11];
                memcpy(pDestBill->u.outGatewayRecord.mscIncomingROUTE.u.rOUTEName, pGwoBill->mscIncomingROUTE_ROUTEName, 10);
                pDestBill->u.outGatewayRecord.mscIncomingROUTE.u.rOUTEName[10] = '\0';
            }
        }
	}

    pDestBill->u.outGatewayRecord.mscOutgoingROUTE.u.rOUTEName = NULL;
	if (GET_CDR_FIELD_FILTER(mscOutgoingROUTE, OutGatewayRecord))		//mscOutgongingTKGP
	{
        if (pGwoBill->mscOutgoingROUTE_ind == SHOW_ROUTE_NO)
        {
            if (pGwoBill->mscOutgoingROUTE_ROUTENumber != 0xFFFF)
            {
                pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_mscOutgoingROUTE_present;
		        pDestBill->u.outGatewayRecord.mscOutgoingROUTE.choice = CMCC_CS_rOUTENumber_chosen;
		        pDestBill->u.outGatewayRecord.mscOutgoingROUTE.u.rOUTENumber = pGwoBill->mscOutgoingROUTE_ROUTENumber;
            }
        }
        else if (pGwoBill->mscOutgoingROUTE_ind == SHOW_ROUTE_NAME)
        {
            if (*(pGwoBill->mscOutgoingROUTE_ROUTEName) != 0x20)
            {
                pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_mscOutgoingROUTE_present;
		        pDestBill->u.outGatewayRecord.mscOutgoingROUTE.choice = CMCC_CS_rOUTEName_chosen;
		        pDestBill->u.outGatewayRecord.mscOutgoingROUTE.u.rOUTEName = new (g_pAsnBuffForCMCC)char[11];
                memcpy(pDestBill->u.outGatewayRecord.mscOutgoingROUTE.u.rOUTEName, pGwoBill->mscOutgoingROUTE_ROUTEName, 10);
                pDestBill->u.outGatewayRecord.mscOutgoingROUTE.u.rOUTEName[10] = '\0';
            }
        }
	}

	if (GET_CDR_FIELD_FILTER(seizureTime, OutGatewayRecord))		//seizureTime
	{
        if (pGwoBill->causeForTerm == unsuccessfulCallAttempt)
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_seizureTime_present;
            ConvertTimeStamp(pDestBill->u.outGatewayRecord.seizureTime, &(pGwoBill->initial_year));
        }
    }

    if (GET_CDR_FIELD_FILTER(answerTime, OutGatewayRecord))		//answerTimestamp
	{
        if (pGwoBill->causeForTerm != unsuccessfulCallAttempt)
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_answerTime_present;
		    ConvertTimeStamp(pDestBill->u.outGatewayRecord.answerTime, &(pGwoBill->initial_year));
        }
	}
	
	if (GET_CDR_FIELD_FILTER(releaseTime, OutGatewayRecord))	//releaseTimestamp
	{
        if (pGwoBill->end_year != 0xFF)
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_releaseTime_present;
		    ConvertTimeStamp(pDestBill->u.outGatewayRecord.releaseTime, &(pGwoBill->end_year));			
        }
	}

    if (GET_CDR_FIELD_FILTER(callDuration, OutGatewayRecord))
    {
        pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_callDuration_present;
        pDestBill->u.outGatewayRecord.callDuration = pGwoBill->callDuration;						
    }

    if (GET_CDR_FIELD_FILTER(causeForTerm, OutGatewayRecord))
    {
        pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_causeForTerm_present;
        pDestBill->u.outGatewayRecord.causeForTerm = pGwoBill->causeForTerm;
    }

	if (GET_CDR_FIELD_FILTER(diagnostics, OutGatewayRecord))			//diagnostics
	{
        pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_diagnostics_present;
		if (pGwoBill->diagnostics.CauseType == 0)
		{
			pDestBill->u.outGatewayRecord.diagnostics.choice = CMCC_CS_gsm0408Cause_chosen;
			pDestBill->u.outGatewayRecord.diagnostics.u.gsm0408Cause = pGwoBill->diagnostics.Cause.gsm0408Cause;
		}
		else if (pGwoBill->diagnostics.CauseType == 1)
		{
			pDestBill->u.outGatewayRecord.diagnostics.choice = CMCC_CS_gsm0902MapErrorValue_chosen;
			pDestBill->u.outGatewayRecord.diagnostics.u.gsm0902MapErrorValue = pGwoBill->diagnostics.Cause.gsm0902MapErrorValue;
		}
		else if (pGwoBill->diagnostics.CauseType == 2)
		{
			pDestBill->u.outGatewayRecord.diagnostics.choice = CMCC_CS_ccittQ767Cause_chosen;
			pDestBill->u.outGatewayRecord.diagnostics.u.ccittQ767Cause = pGwoBill->diagnostics.Cause.ccittQ767Cause;
		}
		else
		{
			pDestBill->u.outGatewayRecord.bit_mask &=
                ~CMCC_CS_OutGatewayRecord_diagnostics_present;
		}
	}

    if (GET_CDR_FIELD_FILTER(callReference, OutGatewayRecord))
    {
        if (pGwoBill->call_reference_len != 0)
        {
            int nCallReferenceLen = min(pGwoBill->call_reference_len,
                sizeof(pDestBill->u.outGatewayRecord.callReference.value));
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_callReference_present;                  
            pDestBill->u.outGatewayRecord.callReference.length = nCallReferenceLen;
            memcpy(pDestBill->u.outGatewayRecord.callReference.value, 
                &(pGwoBill->call_reference[sizeof(pGwoBill->call_reference) - pGwoBill->call_reference_len]), nCallReferenceLen);   
        }
        else
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_callReference_present;
            GetCallReference(pDestBill->u.outGatewayRecord.callReference,
                pGwoBill->ModuleNum, pGwoBill->count);
        }
    }

	if (GET_CDR_FIELD_FILTER(sequenceNumber, OutGatewayRecord))		//部分话单序列号
	{
		if (pGwoBill->record_type != SINGLE_BILL)
		{
			pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_sequenceNumber_present;
			pDestBill->u.outGatewayRecord.sequenceNumber = pGwoBill->sequence;
		}
	}

	pDestBill->u.outGatewayRecord.recordExtensions = NULL;

    pDestBill->u.outGatewayRecord.roamingNumber.length = 0;
    if (GET_CDR_FIELD_FILTER(roamingNumber, OutGatewayRecord))  //漫游号码
	{
		int nRoamingLen = (pGwoBill->roamingNumber_BCDlen + 1) / 2;
        if (nRoamingLen > 0)
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_roamingNumber_present;
		    pDestBill->u.outGatewayRecord.roamingNumber.length = 
                min(sizeof(pDestBill->u.outGatewayRecord.roamingNumber.value), nRoamingLen + 1);
            pDestBill->u.outGatewayRecord.roamingNumber.value[0] = *(pGwoBill->roamingNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.outGatewayRecord.roamingNumber.value + 1, pGwoBill->roamingNumber, 
                pDestBill->u.outGatewayRecord.roamingNumber.length - 1);
        }
    }

    if (GET_CDR_FIELD_FILTER(mscOutgoingCircuit, OutGatewayRecord))
	{
        if (pGwoBill->mscOutgoingCircuit != 0xFFFF)
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_mscOutgoingCircuit_present;
            pDestBill->u.outGatewayRecord.mscOutgoingCircuit = pGwoBill->mscOutgoingCircuit;
        }
    }

    if (GET_CDR_FIELD_FILTER(orgMSCId, OutGatewayRecord))
	{
        if (pGwoBill->msc_id_len != 0)
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_orgMSCId_present;
            pDestBill->u.outGatewayRecord.orgMSCId.length =min(3,pGwoBill->msc_id_len);
            memcpy(pDestBill->u.outGatewayRecord.orgMSCId.value, pGwoBill->orgMSCId, pDestBill->u.outGatewayRecord.orgMSCId.length);
        }
    }

    pDestBill->u.outGatewayRecord.callEmlppPriority.length = 0;
    if (GET_CDR_FIELD_FILTER(callEmlppPriority, OutGatewayRecord))
	{
        if (pGwoBill->callEmlppPriority != 0xF)
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_callEmlppPriority_present;
            pDestBill->u.outGatewayRecord.callEmlppPriority.length = 1;
            pDestBill->u.outGatewayRecord.callEmlppPriority.value[0] = pGwoBill->callEmlppPriority;
        }
    }

    if (GET_CDR_FIELD_FILTER(cUGOutgoingAccessIndicator, OutGatewayRecord))
	{
		if (pGwoBill->CUGOutgoingAccessIndicator != 0)
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_cUGOutgoingAccessIndicator_present;
            pDestBill->u.outGatewayRecord.cUGOutgoingAccessIndicator = CMCC_CS_cUGCall;
        }   
    }

    pDestBill->u.outGatewayRecord.cUGInterlockCode.length = 0;
    if (GET_CDR_FIELD_FILTER(cUGInterlockCode, OutGatewayRecord))
	{
		if (pGwoBill->CUGInterlockCode != 0xFFFFFFFF)
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_cUGInterlockCode_present;
            pDestBill->u.outGatewayRecord.cUGInterlockCode.length = 4;
            memcpy(pDestBill->u.outGatewayRecord.cUGInterlockCode.value, &(pGwoBill->CUGInterlockCode), 4);
        }
	}

    if (GET_CDR_FIELD_FILTER(partialRecordType, OutGatewayRecord))      //1
    {
        if (pGwoBill->cause_for_partial_record != 0xff)
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_partialRecordType_present;
            pDestBill->u.outGatewayRecord.partialRecordType = (CMCC_CS_PartialRecordType)pGwoBill->cause_for_partial_record;
        }
    }
    pDestBill->u.outGatewayRecord.recordExtensions = NULL;

    if (GET_CDR_FIELD_FILTER(basicService, OutGatewayRecord))	//业务码
    {
        if (pGwoBill->TBS < 2 && pGwoBill->ServiceCode != 0xFF)
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_basicService_present;
            ConvertBasicServiceCode(pDestBill->u.outGatewayRecord.basicService, 
                pGwoBill->TBS, pGwoBill->ServiceCode);
        }
    }

    if (GET_CDR_FIELD_FILTER(additionalChgInfo, OutGatewayRecord))		//附加计费信息
    {
        if (pGwoBill->chargeIndicator >= 0 && pGwoBill->chargeIndicator < 3)
        {
            pDestBill->u.outGatewayRecord.bit_mask |=CMCC_CS_OutGatewayRecord_additionalChgInfo_present;
            pDestBill->u.outGatewayRecord.additionalChgInfo.bit_mask = CMCC_CS_chargeIndicator_present;
            pDestBill->u.outGatewayRecord.additionalChgInfo.chargeIndicator = pGwoBill->chargeIndicator;
        }
    }

    if (GET_CDR_FIELD_FILTER(chargedParty,OutGatewayRecord) )
    {
        if (pGwoBill->chargedParty == 0 || pGwoBill->chargedParty == 1)
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_chargedParty_present;
            pDestBill->u.outGatewayRecord.chargedParty = (CMCC_CS_ChargedParty)pGwoBill->chargedParty;
        }
    }

    pDestBill->u.outGatewayRecord.originalCalledNumber.length = 0;
    pDestBill->u.outGatewayRecord.originalCalledNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(originalCalledNumber,OutGatewayRecord))
    {
        int nOriginalCalledLen = (pGwoBill->originalCalledNumber_BCDlen + 1)/2;
        if (nOriginalCalledLen > 0 )
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_originalCalledNumber_present;
            pDestBill->u.outGatewayRecord.originalCalledNumber.length = nOriginalCalledLen + 1;
            pDestBill->u.outGatewayRecord.originalCalledNumber.value = new (g_pAsnBuffForCMCC)BYTE[nOriginalCalledLen + 1];
            pDestBill->u.outGatewayRecord.originalCalledNumber.value[0] = *(pGwoBill->originalCalledNumber - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.outGatewayRecord.originalCalledNumber.value + 1,pGwoBill->originalCalledNumber,
                pDestBill->u.outGatewayRecord.originalCalledNumber.length - 1);
        }
    }
    
    if (GET_CDR_FIELD_FILTER(rateIndication, OutGatewayRecord))		//rateIndication
    {
/*
        if (pGwoBill->rateIndication != 0x00)
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_rateIndication_present;
            pDestBill->u.outGatewayRecord.rateIndication.length = 1;
            pDestBill->u.outGatewayRecord.rateIndication.value[0] = pGwoBill->rateIndication;
        }
*/
    }
    
    pDestBill->u.outGatewayRecord.eaSubscriberInfo.length = 0;
    if (GET_CDR_FIELD_FILTER(eaSubscriberInfo, OutGatewayRecord)) //此域无法得到
    {
        if (pGwoBill->eaSubscriberInfo[0] != 0xff &&
            pGwoBill->eaSubscriberInfo[1] != 0xff &&
            pGwoBill->eaSubscriberInfo[2] != 0xff)
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_eaSubscriberInfo_present; 
            pDestBill->u.outGatewayRecord.eaSubscriberInfo.length = 3;
            memcpy(pDestBill->u.outGatewayRecord.eaSubscriberInfo.value,pGwoBill->eaSubscriberInfo,3);
        }             
    }
    
    pDestBill->u.outGatewayRecord.selectedCIC.length = 0;    
    if (GET_CDR_FIELD_FILTER(selectedCIC,OutGatewayRecord))
    {
        if (pGwoBill->selectedCIC[0] != 0xff &&
            pGwoBill->selectedCIC[1] != 0xff &&
            pGwoBill->selectedCIC[2] != 0xff)
        {
            pDestBill->u.outGatewayRecord.bit_mask |= CMCC_CS_OutGatewayRecord_selectedCIC_present; 
            pDestBill->u.outGatewayRecord.selectedCIC.length = 3;
            memcpy(pDestBill->u.outGatewayRecord.selectedCIC.value,pGwoBill->selectedCIC,3);
        }
    }
   
    if (GET_CDR_FIELD_FILTER(cUGIncomingAccessUsed, OutGatewayRecord))
    {
    }
	return TRUE;
}


//汇接话单转换
BOOL ConvertABill_TRANSIT(const BYTE* pSrcBill, const int nLen,
                          const BYTE* pEventBill, const int nEventNum, 
                          CMCC_CS_CallEventRecord* pDestBill)
{
	Gateway_Bill_Report* pTransitBill = (Gateway_Bill_Report *)pSrcBill;	
	
	pDestBill->choice = CMCC_CS_transitRecord_chosen;
	pDestBill->u.transitRecord.bit_mask = 0;

    if (GET_CDR_FIELD_FILTER(recordType, TransitCallRecord))
    {
        pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_recordType_present;
    	pDestBill->u.transitRecord.recordType = CMCC_CS_transitCallRecord;		//recordType
    }

    pDestBill->u.transitRecord.recordingEntity.length = 0;
    if (GET_CDR_FIELD_FILTER(recordingEntity, TransitCallRecord))
    {
        int nLocalMSCLen = (pTransitBill->recordingEntity_BCDlen + 1) / 2;
        if (nLocalMSCLen > 0)
        {
            pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_recordingEntity_present;
            pDestBill->u.transitRecord.recordingEntity.length = 
                min(sizeof(pDestBill->u.transitRecord.recordingEntity.value), nLocalMSCLen + 1);
            pDestBill->u.transitRecord.recordingEntity.value[0] = *(pTransitBill->recordingEntity - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.transitRecord.recordingEntity.value + 1, pTransitBill->recordingEntity, 
                pDestBill->u.transitRecord.recordingEntity.length - 1);
        }
    }

    pDestBill->u.transitRecord.mscIncomingROUTE.u.rOUTEName = NULL;    
    if (GET_CDR_FIELD_FILTER(mscIncomingROUTE, TransitCallRecord))		//mscIncomingROUTE
	{
        if (pTransitBill->mscIncomingROUTE_ind == SHOW_ROUTE_NO)
        {
            if (pTransitBill->mscIncomingROUTE_ROUTENumber != 0xFFFF) 
            {
                pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_mscIncomingROUTE_present;
		        pDestBill->u.transitRecord.mscIncomingROUTE.choice = CMCC_CS_rOUTENumber_chosen;
		        pDestBill->u.transitRecord.mscIncomingROUTE.u.rOUTENumber = pTransitBill->mscIncomingROUTE_ROUTENumber;
            }
        }
        else if (pTransitBill->mscIncomingROUTE_ind == SHOW_ROUTE_NAME)
        {
            if (*(pTransitBill->mscIncomingROUTE_ROUTEName) != 0x20) 
            {
                pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_mscIncomingROUTE_present;
		        pDestBill->u.transitRecord.mscIncomingROUTE.choice = CMCC_CS_rOUTEName_chosen;
		        pDestBill->u.transitRecord.mscIncomingROUTE.u.rOUTEName = new (g_pAsnBuffForCMCC)char[11];
                memcpy(pDestBill->u.transitRecord.mscIncomingROUTE.u.rOUTEName, pTransitBill->mscIncomingROUTE_ROUTEName, 10);
                pDestBill->u.transitRecord.mscIncomingROUTE.u.rOUTEName[10] = '\0';
            }
        }
	}

    pDestBill->u.transitRecord.mscOutgoingROUTE.u.rOUTEName = NULL;
	if (GET_CDR_FIELD_FILTER(mscOutgoingROUTE, TransitCallRecord))		//mscOutgongingTKGP
	{
        if (pTransitBill->mscOutgoingROUTE_ind == SHOW_ROUTE_NO)
        {
            if (pTransitBill->mscOutgoingROUTE_ROUTENumber != 0xFFFF)
            {
                pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_mscOutgoingROUTE_present;
		        pDestBill->u.transitRecord.mscOutgoingROUTE.choice = CMCC_CS_rOUTENumber_chosen;
		        pDestBill->u.transitRecord.mscOutgoingROUTE.u.rOUTENumber = pTransitBill->mscOutgoingROUTE_ROUTENumber;
            }
        }
        else if (pTransitBill->mscOutgoingROUTE_ind == SHOW_ROUTE_NAME)
        {
            if (*(pTransitBill->mscOutgoingROUTE_ROUTEName) != 0x20)
            {
                pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_mscOutgoingROUTE_present;
		        pDestBill->u.transitRecord.mscOutgoingROUTE.choice = CMCC_CS_rOUTEName_chosen;
		        pDestBill->u.transitRecord.mscOutgoingROUTE.u.rOUTEName = new (g_pAsnBuffForCMCC)char[11];
                memcpy(pDestBill->u.transitRecord.mscOutgoingROUTE.u.rOUTEName, pTransitBill->mscOutgoingROUTE_ROUTEName, 10);
                pDestBill->u.transitRecord.mscOutgoingROUTE.u.rOUTEName[10] = '\0';
            }
        }
	}

	pDestBill->u.transitRecord.callingNumber.length = 0;
    pDestBill->u.transitRecord.callingNumber.value = NULL;
	if (GET_CDR_FIELD_FILTER(callingNumber, TransitCallRecord))		//主叫号码
	{
		int nCallerLen = (pTransitBill->callingNumber_BCDlen + 1) / 2;
        if (nCallerLen > 0)
        {
            pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_callingNumber_present;
		    pDestBill->u.transitRecord.callingNumber.length = nCallerLen + 1;
		    pDestBill->u.transitRecord.callingNumber.value = new (g_pAsnBuffForCMCC)BYTE[nCallerLen + 1];
            pDestBill->u.transitRecord.callingNumber.value[0] = *(pTransitBill->callingNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.transitRecord.callingNumber.value + 1, pTransitBill->callingNumber, nCallerLen);
        }
	}

    pDestBill->u.transitRecord.calledNumber.length = 0;
    pDestBill->u.transitRecord.calledNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(calledNumber, TransitCallRecord))
    {
        int nCalledLen = (pTransitBill->calledNumber_BCDlen + 1) / 2;	
        if (nCalledLen > 0)
        {
            pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_calledNumber_present;
            pDestBill->u.transitRecord.calledNumber.length = nCalledLen + 1;
            pDestBill->u.transitRecord.calledNumber.value = new (g_pAsnBuffForCMCC)BYTE[nCalledLen + 1];
            pDestBill->u.transitRecord.calledNumber.value[0] = *(pTransitBill->calledNumber - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.transitRecord.calledNumber.value + 1, pTransitBill->calledNumber, nCalledLen);
        }
    }
	
	if (GET_CDR_FIELD_FILTER(seizureTimestamp, TransitCallRecord))		//answerTimestamp
	{
        if (pTransitBill->causeForTerm == unsuccessfulCallAttempt)
        {
            pDestBill->u.transitRecord.bit_mask |= CMCC_CS_seizureTimestamp_present;
            ConvertTimeStamp(pDestBill->u.transitRecord.seizureTimestamp, &(pTransitBill->initial_year));
        }
	}

	if (GET_CDR_FIELD_FILTER(answerTimestamp, TransitCallRecord))		//answerTimestamp
	{
        if (pTransitBill->causeForTerm != unsuccessfulCallAttempt)
        {
            pDestBill->u.transitRecord.bit_mask |= CMCC_CS_answerTimestamp_present;
		    ConvertTimeStamp(pDestBill->u.transitRecord.answerTimestamp, &(pTransitBill->initial_year));
        }
	}
	
	if (GET_CDR_FIELD_FILTER(releaseTimestamp, TransitCallRecord))		//releaseTimestamp
	{
        if (pTransitBill->end_year != 0xFF)
        {
            pDestBill->u.transitRecord.bit_mask |= CMCC_CS_releaseTimestamp_present;
	    	ConvertTimeStamp(pDestBill->u.transitRecord.releaseTimestamp, &(pTransitBill->end_year));			
        }
	}

    if (GET_CDR_FIELD_FILTER(callDuration, TransitCallRecord))
    {
        pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_callDuration_present;
        pDestBill->u.transitRecord.callDuration = pTransitBill->callDuration;						
    }

    if (GET_CDR_FIELD_FILTER(causeForTerm, TransitCallRecord))
    {
        pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_causeForTerm_present;
        pDestBill->u.transitRecord.causeForTerm = pTransitBill->causeForTerm;
    }
	
	if (GET_CDR_FIELD_FILTER(diagnostics, TransitCallRecord))			//diagnostics
	{
        pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_diagnostics_present;
		if (pTransitBill->diagnostics.CauseType == 0)
		{
			pDestBill->u.transitRecord.diagnostics.choice = CMCC_CS_gsm0408Cause_chosen;
			pDestBill->u.transitRecord.diagnostics.u.gsm0408Cause = pTransitBill->diagnostics.Cause.gsm0408Cause;
		}
		else if (pTransitBill->diagnostics.CauseType == 1)
		{
			pDestBill->u.transitRecord.diagnostics.choice = CMCC_CS_gsm0902MapErrorValue_chosen;
			pDestBill->u.transitRecord.diagnostics.u.gsm0902MapErrorValue = pTransitBill->diagnostics.Cause.gsm0902MapErrorValue;
		}
		else if (pTransitBill->diagnostics.CauseType == 2)
		{
			pDestBill->u.transitRecord.diagnostics.choice = CMCC_CS_ccittQ767Cause_chosen;
			pDestBill->u.transitRecord.diagnostics.u.ccittQ767Cause = pTransitBill->diagnostics.Cause.ccittQ767Cause;
		}
		else
		{
			pDestBill->u.transitRecord.bit_mask &=
                ~CMCC_CS_TransitCallRecord_diagnostics_present;
		}
	}

    if (GET_CDR_FIELD_FILTER(callReference, TransitCallRecord))
    {
        if (pTransitBill->call_reference_len != 0)
        {
            int nCallReferenceLen = min(pTransitBill->call_reference_len,
                sizeof(pDestBill->u.transitRecord.callReference.value));
            pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_callReference_present;                  
            pDestBill->u.transitRecord.callReference.length = nCallReferenceLen;
            memcpy(pDestBill->u.transitRecord.callReference.value, 
                &(pTransitBill->call_reference[sizeof(pTransitBill->call_reference) - pTransitBill->call_reference_len]), nCallReferenceLen);   
        }
        else
        {
            pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_callReference_present;
            GetCallReference(pDestBill->u.transitRecord.callReference,
                pTransitBill->ModuleNum, pTransitBill->count);
        }
    }

	if (GET_CDR_FIELD_FILTER(sequenceNumber, TransitCallRecord))		//部分话单序列号
	{
		if (pTransitBill->record_type != SINGLE_BILL)
		{
			pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_sequenceNumber_present;
			pDestBill->u.transitRecord.sequenceNumber = pTransitBill->sequence;
		}
	}

	pDestBill->u.transitRecord.recordExtensions = NULL;
	if (GET_CDR_FIELD_FILTER(recordExtensions, TransitCallRecord))		//recordExtensions(目前不支持)
	{
	}

    if (GET_CDR_FIELD_FILTER(mscOutgoingCircuit, TransitCallRecord))
	{
        if (pTransitBill->mscOutgoingCircuit != 0xFFFF)
        {
            pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_mscOutgoingCircuit_present;
            pDestBill->u.transitRecord.mscOutgoingCircuit = pTransitBill->mscOutgoingCircuit;
        }
    }

    if (GET_CDR_FIELD_FILTER(mscIncomingCircuit, TransitCallRecord))
	{
        if (pTransitBill->mscIncomingCircuit != 0xFFFF)
        {
            pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_mscIncomingCircuit_present;
            pDestBill->u.transitRecord.mscIncomingCircuit = pTransitBill->mscIncomingCircuit;
        }
    }

    pDestBill->u.transitRecord.orgMSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgMSCId, TransitCallRecord))
	{
        if (pTransitBill->msc_id_len != 0)
        {
            pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_orgMSCId_present;
            pDestBill->u.transitRecord.orgMSCId.length = min(3,pTransitBill->msc_id_len);
            memcpy(pDestBill->u.transitRecord.orgMSCId.value, pTransitBill->orgMSCId,pDestBill->u.transitRecord.orgMSCId.length);
        }
    }

    pDestBill->u.transitRecord.callEmlppPriority.length = 0;
    if (GET_CDR_FIELD_FILTER(callEmlppPriority, TransitCallRecord))
	{
        if (pTransitBill->callEmlppPriority != 0xF)
        {
            pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_callEmlppPriority_present;
            pDestBill->u.transitRecord.callEmlppPriority.length = 1;
            pDestBill->u.transitRecord.callEmlppPriority.value[0] = pTransitBill->callEmlppPriority;
        }
    }

    if (GET_CDR_FIELD_FILTER(cUGOutgoingAccessIndicator, TransitCallRecord))
	{
		if (pTransitBill->CUGOutgoingAccessIndicator != 0)
        {
            pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_cUGOutgoingAccessIndicator_present;
            pDestBill->u.transitRecord.cUGOutgoingAccessIndicator = CMCC_CS_cUGCall;
        }
	}

    pDestBill->u.transitRecord.cUGInterlockCode.length = 0;
    if (GET_CDR_FIELD_FILTER(cUGInterlockCode, TransitCallRecord))
	{
		if (pTransitBill->CUGInterlockCode != 0xFFFFFFFF)
        {
            pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_cUGInterlockCode_present;
            pDestBill->u.transitRecord.cUGInterlockCode.length = 4;
            memcpy(pDestBill->u.transitRecord.cUGInterlockCode.value, &(pTransitBill->CUGInterlockCode), 4);
        }
	}
    if (GET_CDR_FIELD_FILTER(partialRecordType, TransitCallRecord))      //1
    {
        if (pTransitBill->cause_for_partial_record != 0xff)
        {
            pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_partialRecordType_present;
            pDestBill->u.transitRecord.partialRecordType = (CMCC_CS_PartialRecordType)pTransitBill->cause_for_partial_record;
        }
    }   
    
    if (GET_CDR_FIELD_FILTER(basicService, TransitCallRecord))	//业务码
    {
        if (pTransitBill->TBS < 2 && pTransitBill->ServiceCode != 0xFF)
        {
            pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_basicService_present;
            ConvertBasicServiceCode(pDestBill->u.transitRecord.basicService, 
                pTransitBill->TBS, pTransitBill->ServiceCode);
        }
    }
    
    if (GET_CDR_FIELD_FILTER(additionalChgInfo, TransitCallRecord))		//附加计费信息
    {
        if (pTransitBill->chargeIndicator >= 0 && pTransitBill->chargeIndicator < 3)
        {
            pDestBill->u.transitRecord.bit_mask |=CMCC_CS_TransitCallRecord_additionalChgInfo_present;
            pDestBill->u.transitRecord.additionalChgInfo.bit_mask = CMCC_CS_chargeIndicator_present;
            pDestBill->u.transitRecord.additionalChgInfo.chargeIndicator = pTransitBill->chargeIndicator;
        }
    }
    
    pDestBill->u.transitRecord.originalCalledNumber.length = 0;
    pDestBill->u.transitRecord.originalCalledNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(originalCalledNumber,TransitCallRecord))
    {
        int nOriginalCalledLen = (pTransitBill->originalCalledNumber_BCDlen + 1)/2;
        if (nOriginalCalledLen > 0 )
        {
            pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_originalCalledNumber_present;
            pDestBill->u.transitRecord.originalCalledNumber.length = nOriginalCalledLen + 1;
            pDestBill->u.transitRecord.originalCalledNumber.value = new (g_pAsnBuffForCMCC)BYTE[nOriginalCalledLen + 1];
            pDestBill->u.transitRecord.originalCalledNumber.value[0] = *(pTransitBill->originalCalledNumber - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.transitRecord.originalCalledNumber.value + 1,pTransitBill->originalCalledNumber,
                pDestBill->u.transitRecord.originalCalledNumber.length - 1);
        }
    }
    
    if (GET_CDR_FIELD_FILTER(rateIndication, TransitCallRecord))		//rateIndication
    {
    }  
 
    pDestBill->u.transitRecord.eaSubscriberInfo.length = 0;
    if (GET_CDR_FIELD_FILTER(eaSubscriberInfo, TransitCallRecord)) //此域无法得到
    {
        if (pTransitBill->eaSubscriberInfo[0] != 0xff &&
            pTransitBill->eaSubscriberInfo[1] != 0xff &&
            pTransitBill->eaSubscriberInfo[2] != 0xff)
        {
            pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_eaSubscriberInfo_present; 
            pDestBill->u.transitRecord.eaSubscriberInfo.length = 3;
            memcpy(pDestBill->u.transitRecord.eaSubscriberInfo.value,pTransitBill->eaSubscriberInfo,3);
        }    
    }

    pDestBill->u.transitRecord.selectedCIC.length = 0;
    if (GET_CDR_FIELD_FILTER(selectedCIC,TransitCallRecord))
    {
        if (pTransitBill->selectedCIC[0] != 0xff &&
            pTransitBill->selectedCIC[1] != 0xff &&
            pTransitBill->selectedCIC[2] != 0xff)
        {
            pDestBill->u.transitRecord.bit_mask |= CMCC_CS_TransitCallRecord_selectedCIC_present; 
            pDestBill->u.transitRecord.selectedCIC.length = 3;
            memcpy(pDestBill->u.transitRecord.selectedCIC.value,pTransitBill->selectedCIC,3);
        }
    }
    
    if (GET_CDR_FIELD_FILTER(cUGIncomingAccessUsed, TransitCallRecord))
    {
    }
	return TRUE;
}

BOOL ConvertABill_QUERY_HLR(const BYTE* pSrcBill, const int nLen,
                            const BYTE* pEventBill, const int nEventNum, 
                            CMCC_CS_CallEventRecord* pDestBill)
{
	HLR_Query_Bill_Report* pHlrBill = (HLR_Query_Bill_Report *)pSrcBill;
	
	pDestBill->choice = CMCC_CS_hlrIntRecord_chosen;
	pDestBill->u.hlrIntRecord.bit_mask = 0;

    if (GET_CDR_FIELD_FILTER(recordType, HLRIntRecord))
    {
        pDestBill->u.hlrIntRecord.bit_mask |= CMCC_CS_HLRIntRecord_recordType_present;
	    pDestBill->u.hlrIntRecord.recordType = CMCC_CS_CallEventRecordType_hlrIntRecord;		//recordType
    }

    pDestBill->u.hlrIntRecord.servedIMSI.length = 0;
    if (GET_CDR_FIELD_FILTER(servedIMSI, HLRIntRecord))
    {
        pDestBill->u.hlrIntRecord.servedIMSI.length = 
            BCDtoRBCD(pDestBill->u.hlrIntRecord.servedIMSI.value, pHlrBill->servedIMSI, 8);
        if(pDestBill->u.hlrIntRecord.servedIMSI.length > 0)
        {
            pDestBill->u.hlrIntRecord.bit_mask |= CMCC_CS_HLRIntRecord_servedIMSI_present;        
        }
    }

    pDestBill->u.hlrIntRecord.servedMSISDN.length = 0;
    if (GET_CDR_FIELD_FILTER(servedMSISDN, HLRIntRecord))
    {
	    int nMSISDNLen = (pHlrBill->servedMSISDN_BCDlen + 1) / 2;
        if (nMSISDNLen > 0)
        {
            pDestBill->u.hlrIntRecord.bit_mask |= CMCC_CS_HLRIntRecord_servedMSISDN_present;
            pDestBill->u.hlrIntRecord.servedMSISDN.length = 
                min(sizeof(pDestBill->u.hlrIntRecord.servedMSISDN.value), nMSISDNLen + 1);
            pDestBill->u.hlrIntRecord.servedMSISDN.value[0] = *(pHlrBill->servedMSISDN - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.hlrIntRecord.servedMSISDN.value + 1, pHlrBill->servedMSISDN, 
                pDestBill->u.hlrIntRecord.servedMSISDN.length - 1);
        }
    }

    pDestBill->u.hlrIntRecord.recordingEntity.length = 0;
    if (GET_CDR_FIELD_FILTER(recordingEntity, HLRIntRecord))
    {
	    int nServedMSCLen = (pHlrBill->recordingEntity_BCDlen + 1) / 2;
        if (nServedMSCLen > 0)
        {
            pDestBill->u.hlrIntRecord.bit_mask |= CMCC_CS_HLRIntRecord_recordingEntity_present;
            pDestBill->u.hlrIntRecord.recordingEntity.length = 
                min(sizeof(pDestBill->u.hlrIntRecord.recordingEntity.value), nServedMSCLen + 1);
            pDestBill->u.hlrIntRecord.recordingEntity.value[0] = *(pHlrBill->recordingEntity - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.hlrIntRecord.recordingEntity.value + 1, pHlrBill->recordingEntity, 
                pDestBill->u.hlrIntRecord.recordingEntity.length - 1);
        }
    }

	if (GET_CDR_FIELD_FILTER(basicService, HLRIntRecord))	//业务码
	{
        if (pHlrBill->TBS < 2 && pHlrBill->ServiceCode != 0xFF)
        {
            pDestBill->u.hlrIntRecord.bit_mask |= CMCC_CS_HLRIntRecord_basicService_present;
		    ConvertBasicServiceCode(pDestBill->u.hlrIntRecord.basicService, 
                pHlrBill->TBS, pHlrBill->ServiceCode);
        }
	}

    pDestBill->u.hlrIntRecord.routingNumber.u.roaming.length = 0;
    pDestBill->u.hlrIntRecord.routingNumber.u.forwarded.length = 0;
    if (GET_CDR_FIELD_FILTER(routingNumber, HLRIntRecord))
    {
        //SWPD03519 为了防止编码失败的异常情况，这里choice填为漫游，安全起见
        pDestBill->u.hlrIntRecord.routingNumber.choice = CMCC_CS_roaming_chosen;
	    int nRoutingNumberLen = (pHlrBill->routingNumber_BCDlen + 1) / 2;
        if (nRoutingNumberLen > 0)
        {
            pDestBill->u.hlrIntRecord.bit_mask |= CMCC_CS_routingNumber_present;
            if(pHlrBill->numbertypeind == 1)  //漫游号码
            {
                pDestBill->u.hlrIntRecord.routingNumber.choice = CMCC_CS_roaming_chosen;
                pDestBill->u.hlrIntRecord.routingNumber.u.roaming.length = 
                    min(sizeof(pDestBill->u.hlrIntRecord.routingNumber.u.roaming.value), nRoutingNumberLen + 1);
                pDestBill->u.hlrIntRecord.routingNumber.u.roaming.value[0] = *(pHlrBill->routingNumber - 2) | 0x80;
		        BCDtoRBCD(pDestBill->u.hlrIntRecord.routingNumber.u.roaming.value + 1, pHlrBill->routingNumber, 
                    pDestBill->u.hlrIntRecord.routingNumber.u.roaming.length - 1);
            }
            else if(pHlrBill->numbertypeind == 2)  //前转号码
            {
                pDestBill->u.hlrIntRecord.routingNumber.choice = CMCC_CS_forwarded_chosen;
                pDestBill->u.hlrIntRecord.routingNumber.u.forwarded.length = 
                    min(sizeof(pDestBill->u.hlrIntRecord.routingNumber.u.forwarded.value), nRoutingNumberLen + 1);
                pDestBill->u.hlrIntRecord.routingNumber.u.forwarded.value[0] = *(pHlrBill->routingNumber - 2) | 0x80;
		        BCDtoRBCD(pDestBill->u.hlrIntRecord.routingNumber.u.forwarded.value + 1, pHlrBill->routingNumber, 
                    pDestBill->u.hlrIntRecord.routingNumber.u.forwarded.length - 1);        
            }
        }
    }

    if (GET_CDR_FIELD_FILTER(interrogationTime, HLRIntRecord))
    {
        pDestBill->u.hlrIntRecord.bit_mask |= CMCC_CS_HLRIntRecord_interrogationTime_present;
        ConvertTimeStamp(pDestBill->u.hlrIntRecord.interrogationTime, &(pHlrBill->interrogationTime_year));			
    }
	
	if (GET_CDR_FIELD_FILTER(numberOfForwarding, HLRIntRecord))	//前转次数
	{
        if (pHlrBill->numberOfForwarding != 0)
        {
            pDestBill->u.hlrIntRecord.bit_mask |= CMCC_CS_numberOfForwarding_present;
            pDestBill->u.hlrIntRecord.numberOfForwarding = pHlrBill->numberOfForwarding;
        }
	}

	if (GET_CDR_FIELD_FILTER(interrogationResult, HLRIntRecord))
	{
		pDestBill->u.hlrIntRecord.bit_mask |= CMCC_CS_interrogationResult_present;
		if (pHlrBill->interrogationResult.CauseType == 0 && pHlrBill->interrogationResult.Cause.gsm0408Cause != 0)
		{
			pDestBill->u.hlrIntRecord.interrogationResult.choice = CMCC_CS_gsm0408Cause_chosen;
			pDestBill->u.hlrIntRecord.interrogationResult.u.gsm0408Cause = pHlrBill->interrogationResult.Cause.gsm0408Cause;
		}
		else if (pHlrBill->interrogationResult.CauseType == 1 && pHlrBill->interrogationResult.Cause.gsm0902MapErrorValue != 0)
		{
			pDestBill->u.hlrIntRecord.interrogationResult.choice = CMCC_CS_gsm0902MapErrorValue_chosen;
			pDestBill->u.hlrIntRecord.interrogationResult.u.gsm0902MapErrorValue = pHlrBill->interrogationResult.Cause.gsm0902MapErrorValue;
		}
		else if (pHlrBill->interrogationResult.CauseType == 2 && pHlrBill->interrogationResult.Cause.ccittQ767Cause != 0)
		{
			pDestBill->u.hlrIntRecord.interrogationResult.choice = CMCC_CS_ccittQ767Cause_chosen;
			pDestBill->u.hlrIntRecord.interrogationResult.u.ccittQ767Cause = pHlrBill->interrogationResult.Cause.ccittQ767Cause;
		}
		else
		{
			pDestBill->u.hlrIntRecord.bit_mask &= ~CMCC_CS_interrogationResult_present;
		}
	}

	pDestBill->u.hlrIntRecord.recordExtensions = NULL;
	if (GET_CDR_FIELD_FILTER(recordExtensions, TransitCallRecord))		//recordExtensions(目前不支持)
	{
	}

    pDestBill->u.hlrIntRecord.orgMSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgMSCId, HLRIntRecord))
	{
        if (pHlrBill->msc_id_len != 0)
        {
            pDestBill->u.hlrIntRecord.bit_mask |= CMCC_CS_HLRIntRecord_orgMSCId_present;
            pDestBill->u.hlrIntRecord.orgMSCId.length = min(3,pHlrBill->msc_id_len);
            memcpy(pDestBill->u.hlrIntRecord.orgMSCId.value, pHlrBill->orgMSCId, pDestBill->u.hlrIntRecord.orgMSCId.length);
        }
    }

    
	return TRUE;
}

BOOL ConvertABill_SS_ACT(const BYTE* pSrcBill, const int nLen, 
                         const BYTE* pEventBill, const int nEventNum, 
                         CMCC_CS_CallEventRecord* pDestBill)
{
	SS_Action_Bill_Report* pSsactBill = (SS_Action_Bill_Report *)pSrcBill;	
	
	pDestBill->choice = CMCC_CS_ssActionRecord_chosen;
    pDestBill->u.ssActionRecord.bit_mask = 0;

    if (GET_CDR_FIELD_FILTER(recordType, SSActionRecord))
    {
        pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_recordType_present;
	    pDestBill->u.ssActionRecord.recordType = CMCC_CS_CallEventRecordType_ssActionRecord;	//recordType
    }

    pDestBill->u.ssActionRecord.servedIMSI.length = 0;
    if (GET_CDR_FIELD_FILTER(servedIMSI, SSActionRecord))
    {
        pDestBill->u.ssActionRecord.servedIMSI.length = 
            BCDtoRBCD(pDestBill->u.ssActionRecord.servedIMSI.value, pSsactBill->servedIMSI, 8);
        if(pDestBill->u.ssActionRecord.servedIMSI.length > 0)
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_servedIMSI_present;        
        }
    }

    pDestBill->u.ssActionRecord.servedIMEI.length = 0;
	if (GET_CDR_FIELD_FILTER(servedIMEI, SSActionRecord))				//ServedIMSI
	{
		pDestBill->u.ssActionRecord.servedIMEI.length = 
            BCDtoRBCD(pDestBill->u.ssActionRecord.servedIMEI.value, pSsactBill->servedIMEI, 8);
        if (pDestBill->u.ssActionRecord.servedIMEI.length > 0)
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_servedIMEI_present;
        }
	}

	pDestBill->u.ssActionRecord.servedMSISDN.length = 0;
    if (GET_CDR_FIELD_FILTER(servedMSISDN, SSActionRecord))				//ServedMSISDN
	{
	    int nMSISDNLen = (pSsactBill->servedMSISDN_BCDlen + 1) / 2;
        if (nMSISDNLen > 0)
        {
		    pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_servedMSISDN_present;
            pDestBill->u.ssActionRecord.servedMSISDN.length = 
                min(sizeof(pDestBill->u.ssActionRecord.servedMSISDN.value), nMSISDNLen + 1);
            pDestBill->u.ssActionRecord.servedMSISDN.value[0] = *(pSsactBill->servedMSISDN - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.ssActionRecord.servedMSISDN.value + 1, pSsactBill->servedMSISDN, 
                pDestBill->u.ssActionRecord.servedMSISDN.length - 1);
        }
	}

    pDestBill->u.ssActionRecord.msClassmark.length = 0;
    if (GET_CDR_FIELD_FILTER(msClassmark, SSActionRecord))
    {
        if(!(pSsactBill->MSClassmark[0] == 0
            && pSsactBill->MSClassmark[1] == 0
            && pSsactBill->MSClassmark[2] == 0)
            && !(pSsactBill->MSClassmark[0] == 0xFF
            && pSsactBill->MSClassmark[1] == 0xFF
            && pSsactBill->MSClassmark[2] == 0xFF))
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_msClassmark_present;
            pDestBill->u.ssActionRecord.msClassmark.length = 3;
	        pDestBill->u.ssActionRecord.msClassmark.value = new (g_pAsnBuffForCMCC)BYTE[3];
	        memcpy(pDestBill->u.ssActionRecord.msClassmark.value, pSsactBill->MSClassmark, 3);
        }
    }

    pDestBill->u.ssActionRecord.recordingEntity.length = 0;
    if (GET_CDR_FIELD_FILTER(recordingEntity, SSActionRecord))
    {
	    int nServedMSCLen = (pSsactBill->recordingEntity_BCDlen + 1) / 2;
        if (nServedMSCLen > 0)
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_recordingEntity_present;
            pDestBill->u.ssActionRecord.recordingEntity.length = 
                min(sizeof(pDestBill->u.ssActionRecord.recordingEntity.value), nServedMSCLen + 1);
            pDestBill->u.ssActionRecord.recordingEntity.value[0] = *(pSsactBill->recordingEntity - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.ssActionRecord.recordingEntity.value + 1, pSsactBill->recordingEntity, 
                pDestBill->u.ssActionRecord.recordingEntity.length - 1);
        }
    }
	
    pDestBill->u.ssActionRecord.location.locationAreaCode.length = 0;
    pDestBill->u.ssActionRecord.location.cellIdentifier.length = 0;
	if (GET_CDR_FIELD_FILTER(location, SSActionRecord))					//位置区、小区
	{
        if (IsLACandCellIDValid(&(pSsactBill->served_location_LAI[3]), pSsactBill->served_location_cellID))
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_location_present;
		    pDestBill->u.ssActionRecord.location.locationAreaCode.length = 2;
		    memcpy(pDestBill->u.ssActionRecord.location.locationAreaCode.value, pSsactBill->served_location_LAI + 3, 2);
		    pDestBill->u.ssActionRecord.location.cellIdentifier.length = 2;
		    memcpy(pDestBill->u.ssActionRecord.location.cellIdentifier.value, pSsactBill->served_location_cellID, 2);
        }
	}

	pDestBill->u.ssActionRecord.basicServices = NULL;
	if (GET_CDR_FIELD_FILTER(basicServices, SSActionRecord))				//基本业务
	{
		if (pSsactBill->TBS < 2 && pSsactBill->ServiceCode != 0xFF)
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_basicServices_present;
            pDestBill->u.ssActionRecord.basicServices = new (g_pAsnBuffForCMCC)CMCC_CS_BasicServices_;
            pDestBill->u.ssActionRecord.basicServices->next = NULL;
		    ConvertBasicServiceCode(pDestBill->u.ssActionRecord.basicServices->value, 
                pSsactBill->TBS, pSsactBill->ServiceCode);
        }
	}

    pDestBill->u.ssActionRecord.supplService.length = 0;
	if (GET_CDR_FIELD_FILTER(supplService, SSActionRecord))				//补充业务
	{
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_supplService_present;
		    pDestBill->u.ssActionRecord.supplService.length = 1;
		    pDestBill->u.ssActionRecord.supplService.value[0] = pSsactBill->SSCode;
	}

	if (GET_CDR_FIELD_FILTER(ssAction, SSActionRecord))					//补充业务类型
	{
        if (pSsactBill->ssAction != 0xFF)
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_ssAction_present;
		    pDestBill->u.ssActionRecord.ssAction = (CMCC_CS_SSActionType)pSsactBill->ssAction;
        }
	}

    if (GET_CDR_FIELD_FILTER(ssActionTime, SSActionRecord))
    {
	    pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_ssActionTime_present;
        ConvertTimeStamp(pDestBill->u.ssActionRecord.ssActionTime, &(pSsactBill->ssActionTime_year));
    }

	pDestBill->u.ssActionRecord.ssParameters.u.unstructuredData.length = 0;
    pDestBill->u.ssActionRecord.ssParameters.u.unstructuredData.value = NULL;
    if (GET_CDR_FIELD_FILTER(ssParameters, SSActionRecord))				//补充业务操作参数
	{
        int nSSParaType = pSsactBill->SSParameter.type;
        int nSSParaLen  = pSsactBill->SSParameter.SS_Para_len;
        if ((nSSParaType != 0xFF) && (nSSParaLen != 0))
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_ssParameters_present;
            if(nSSParaType == 0)
            {
                nSSParaLen = min(nSSParaLen, sizeof(pDestBill->u.ssActionRecord.ssParameters.u.forwardedToNumber.value));
                pDestBill->u.ssActionRecord.ssParameters.choice = CMCC_CS_forwardedToNumber_chosen;
                pDestBill->u.ssActionRecord.ssParameters.u.forwardedToNumber.length = nSSParaLen;
		        memcpy(pDestBill->u.ssActionRecord.ssParameters.u.forwardedToNumber.value, 
                       pSsactBill->SSParameter.data.forwardedToNumber, nSSParaLen);
                pDestBill->u.ssActionRecord.ssParameters.u.forwardedToNumber.value[0] |= 0x80;
            }
            else
            {
                nSSParaLen = min(nSSParaLen, sizeof(pSsactBill->SSParameter.data));
                pDestBill->u.ssActionRecord.ssParameters.choice = CMCC_CS_unstructuredData_chosen;
                pDestBill->u.ssActionRecord.ssParameters.u.unstructuredData.length = nSSParaLen;
		        pDestBill->u.ssActionRecord.ssParameters.u.unstructuredData.value = new (g_pAsnBuffForCMCC)BYTE[nSSParaLen];
		        memcpy(pDestBill->u.ssActionRecord.ssParameters.u.unstructuredData.value, 
                       pSsactBill->SSParameter.data.unstructuredData, nSSParaLen);
            }
        }
	}

	if (GET_CDR_FIELD_FILTER(ssActionResult, SSActionRecord))			//diagnostics
	{
        pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_ssActionResult_present;
		if (pSsactBill->ssActionResult.CauseType == 0 && pSsactBill->ssActionResult.Cause.gsm0408Cause != 0)
		{
			pDestBill->u.ssActionRecord.ssActionResult.choice = CMCC_CS_gsm0408Cause_chosen;
			pDestBill->u.ssActionRecord.ssActionResult.u.gsm0408Cause = pSsactBill->ssActionResult.Cause.gsm0408Cause;
		}
		else if (pSsactBill->ssActionResult.CauseType == 1 && pSsactBill->ssActionResult.Cause.gsm0902MapErrorValue != 0)
		{
			pDestBill->u.ssActionRecord.ssActionResult.choice = CMCC_CS_gsm0902MapErrorValue_chosen;
			pDestBill->u.ssActionRecord.ssActionResult.u.gsm0902MapErrorValue = pSsactBill->ssActionResult.Cause.gsm0902MapErrorValue;
		}
		else if (pSsactBill->ssActionResult.CauseType == 2 && pSsactBill->ssActionResult.Cause.ccittQ767Cause != 0)
		{
			pDestBill->u.ssActionRecord.ssActionResult.choice = CMCC_CS_ccittQ767Cause_chosen;
			pDestBill->u.ssActionRecord.ssActionResult.u.ccittQ767Cause = pSsactBill->ssActionResult.Cause.ccittQ767Cause;
		}
		else
		{
			pDestBill->u.ssActionRecord.bit_mask &= ~CMCC_CS_ssActionResult_present;
		}
	}

    if (GET_CDR_FIELD_FILTER(callReference, SSActionRecord))
    {
        if (pSsactBill->call_reference_len != 0)
        {
            int nCallReferenceLen = min(pSsactBill->call_reference_len,
                sizeof(pDestBill->u.ssActionRecord.callReference.value));
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_callReference_present;                  
            pDestBill->u.ssActionRecord.callReference.length = nCallReferenceLen;
            memcpy(pDestBill->u.ssActionRecord.callReference.value, 
                &(pSsactBill->call_reference[sizeof(pSsactBill->call_reference) - pSsactBill->call_reference_len]), nCallReferenceLen);   
        }
        else
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_callReference_present;
            GetCallReference(pDestBill->u.ssActionRecord.callReference,
                pSsactBill->ModuleNum, pSsactBill->count);
        }
    }

	pDestBill->u.ssActionRecord.recordExtensions = NULL;								//recordExtensions(目前不支持)
	if (GET_CDR_FIELD_FILTER(recordExtensions, SSActionRecord))		
	{
	}

	if (GET_CDR_FIELD_FILTER(systemType, SSActionRecord))
	{
        switch((ACCESS_NETWORK_TYPE)pSsactBill->systemType)
        {
        case ACCESS_BSC :
            pDestBill->u.ssActionRecord.systemType = CMCC_CS_gERAN;
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_systemType_present;
            break;
        case ACCESS_RNC :
            pDestBill->u.ssActionRecord.systemType = CMCC_CS_iuUTRAN;
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_systemType_present;
            break;
        case ACCESS_UNKNOWN:
            pDestBill->u.ssActionRecord.systemType = CMCC_CS_unknown;
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_systemType_present;
            break;
        }
	}

    pDestBill->u.ssActionRecord.orgRNCorBSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgRNCorBSCId, SSActionRecord))
	{
        //如果Rnc_Bsc_id全0，则不进行编码
        if (pSsactBill->rnc_bsc_id_len != 0)
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_orgRNCorBSCId_present;
            pDestBill->u.ssActionRecord.orgRNCorBSCId.length = min(3,pSsactBill->rnc_bsc_id_len);
            memcpy(pDestBill->u.ssActionRecord.orgRNCorBSCId.value, pSsactBill->orgRNCorBSCId, pDestBill->u.ssActionRecord.orgRNCorBSCId.length);
        }
    }

    pDestBill->u.ssActionRecord.orgMSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgMSCId, SSActionRecord))
	{
        if (pSsactBill->msc_id_len != 0)
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_orgMSCId_present;
            pDestBill->u.ssActionRecord.orgMSCId.length = min(3,pSsactBill->msc_id_len);
            memcpy(pDestBill->u.ssActionRecord.orgMSCId.value, pSsactBill->orgMSCId, pDestBill->u.ssActionRecord.orgMSCId.length);
        }
    }

    pDestBill->u.ssActionRecord.subscriberCategory.length = 0;
    if (GET_CDR_FIELD_FILTER(subscriberCategory, SSActionRecord))
	{
        pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_subscriberCategory_present;
        pDestBill->u.ssActionRecord.subscriberCategory.length = 1;
        pDestBill->u.ssActionRecord.subscriberCategory.value[0] = pSsactBill->subscriberCategory;
    }

    pDestBill->u.ssActionRecord.firstmccmnc.length = 0;
    if (GET_CDR_FIELD_FILTER(firstmccmnc, SSActionRecord))
	{
        if (!(pSsactBill->served_location_LAI[0] == 0xFF
            && pSsactBill->served_location_LAI[1] == 0xFF
            && pSsactBill->served_location_LAI[2] == 0xFF)
            && !(pSsactBill->served_location_LAI[0] == 0
            && pSsactBill->served_location_LAI[1] == 0
            && pSsactBill->served_location_LAI[2] == 0))
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_firstmccmnc_present;
            pDestBill->u.ssActionRecord.firstmccmnc.length = 3;
            memcpy(pDestBill->u.ssActionRecord.firstmccmnc.value, pSsactBill->served_location_LAI, 3);
        }
    }

    if (GET_CDR_FIELD_FILTER(hotBillingTag, SSActionRecord))
    {
        if (pSsactBill->HotBillingTag == HOT_BILLING)
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_hotBillingTag_present;		
            pDestBill->u.ssActionRecord.hotBillingTag = CMCC_CS_hotBilling;
        }
        else
        {
            pDestBill->u.ssActionRecord.hotBillingTag = CMCC_CS_noHotBilling;
        }
    }

    pDestBill->u.ssActionRecord.ussdCodingScheme.length = 0;
    pDestBill->u.ssActionRecord.ussdCodingScheme.value = NULL;
    if (GET_CDR_FIELD_FILTER(ussdCodingScheme, SSActionRecord))     //1
    {
        if (pSsactBill->UssdCodingScheme != 0xff)
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_ussdCodingScheme_present;
            pDestBill->u.ssActionRecord.ussdCodingScheme.length = 1;
            pDestBill->u.ssActionRecord.ussdCodingScheme.value = new (g_pAsnBuffForCMCC)BYTE[pDestBill->u.ssActionRecord.ussdCodingScheme.length];
            memcpy(pDestBill->u.ssActionRecord.ussdCodingScheme.value , &(pSsactBill->UssdCodingScheme), 1);
        }
    }   
    
    if (GET_CDR_FIELD_FILTER(ussdString, SSActionRecord))      //2
    {
        if (pSsactBill->SSParameter.SS_Para_len != 0) 
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_ussdString_present;
            CMCC_CS_SSActionRecord::CMCC_CS__seqof7* pUssdString = new (g_pAsnBuffForCMCC)CMCC_CS_SSActionRecord::CMCC_CS__seqof7;
            pUssdString->next = NULL;
            pUssdString->value.length = pSsactBill->SSParameter.SS_Para_len;
            pUssdString->value.value = new (g_pAsnBuffForCMCC)BYTE[pUssdString->value.length];
            memcpy(pUssdString->value.value, &(pSsactBill->SSParameter.data), pUssdString->value.length) ;
            pDestBill->u.ssActionRecord.ussdString = pUssdString;
        }
    }
    if (GET_CDR_FIELD_FILTER(ussdNotifyCounter, SSActionRecord))      //3
    {
        if (pSsactBill->UssdNotifyCounter != 0xff && pSsactBill->UssdNotifyCounter != 0)
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_ussdNotifyCounter_present;
            pDestBill->u.ssActionRecord.ussdNotifyCounter = pSsactBill->UssdNotifyCounter;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(ussdRequestCounter, SSActionRecord))      //4
    {
        if (pSsactBill->UssdRequestCounter != 0xff)
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_ussdRequestCounter_present;
            pDestBill->u.ssActionRecord.ussdRequestCounter = pSsactBill->UssdRequestCounter;
        }   
    }
    
    pDestBill->u.ssActionRecord.classmark3.length = 0;
    if (GET_CDR_FIELD_FILTER(classmark3, SSActionRecord))      //5
    {
        if (pSsactBill->Classmark3Indication) 
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_classmark3_present;
            pDestBill->u.ssActionRecord.classmark3.length = 2;
            memcpy(pDestBill->u.ssActionRecord.classmark3.value, &(pSsactBill->Classmark3Information), 2);
        }
    }
    
    pDestBill->u.ssActionRecord.globalAreaID.length = 0;
    if (GET_CDR_FIELD_FILTER(globalAreaID, SSActionRecord))      //6
    {
        if (IsLACandCellIDValid(&(pSsactBill->served_location_LAI[3]),pSsactBill->served_location_cellID)) 
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_globalAreaID_present;
            pDestBill->u.ssActionRecord.globalAreaID.length = 7;
            memcpy(pDestBill->u.ssActionRecord.globalAreaID.value,pSsactBill->served_location_LAI,5);
            memcpy(pDestBill->u.ssActionRecord.globalAreaID.value + 5,pSsactBill->served_location_cellID,2);    
        }
    }
    
    if (GET_CDR_FIELD_FILTER(additionalChgInfo, SSActionRecord))		//附加计费信息
    {
        if (pSsactBill->chargeIndicator >= 0 && pSsactBill->chargeIndicator < 3)
        {
            pDestBill->u.ssActionRecord.bit_mask |=CMCC_CS_SSActionRecord_additionalChgInfo_present;
            pDestBill->u.ssActionRecord.additionalChgInfo.bit_mask = CMCC_CS_chargeIndicator_present;
            pDestBill->u.ssActionRecord.additionalChgInfo.chargeIndicator = pSsactBill->chargeIndicator;
        }
    }

    if (GET_CDR_FIELD_FILTER(chargedParty,SSActionRecord) )
    {
        if (pSsactBill->chargedParty == 1)
        {
            pDestBill->u.ssActionRecord.bit_mask |= CMCC_CS_SSActionRecord_chargedParty_present;
            pDestBill->u.ssActionRecord.chargedParty = (CMCC_CS_ChargedParty)pSsactBill->chargedParty;
        }
    }
    
    
	return TRUE;
}

//漫游话单
BOOL ConvertABill_ROAM(const BYTE* pSrcBill, const int nLen,
                       const BYTE* pEventBill, const int nEventNum,                        
                       CMCC_CS_CallEventRecord* pDestBill)
{
	ROAM_Bill_Report* pRoamBill = (ROAM_Bill_Report *)pSrcBill;	
	
	pDestBill->choice = CMCC_CS_roamingRecord_chosen;
	memset(pDestBill->u.roamingRecord.bit_mask, 0, sizeof(pDestBill->u.roamingRecord.bit_mask));

    if (GET_CDR_FIELD_FILTER(recordType, RoamingRecord))
    {
        pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_recordType_present_byte]
            |= CMCC_CS_RoamingRecord_recordType_present;    
        pDestBill->u.roamingRecord.recordType = CMCC_CS_CallEventRecordType_roamingRecord;		//recordType
    }

    pDestBill->u.roamingRecord.servedIMSI.length = 0;
    if (GET_CDR_FIELD_FILTER(servedIMSI, RoamingRecord))
    {
        pDestBill->u.roamingRecord.servedIMSI.length = 
            BCDtoRBCD(pDestBill->u.roamingRecord.servedIMSI.value, pRoamBill->servedIMSI, 8);
        if(pDestBill->u.roamingRecord.servedIMSI.length > 0)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_servedIMSI_present_byte] |=
                CMCC_CS_RoamingRecord_servedIMSI_present;        
        }
    }

    pDestBill->u.roamingRecord.servedMSISDN.length = 0;
	if (GET_CDR_FIELD_FILTER(servedMSISDN, RoamingRecord))				//ServedMSISDN
	{
		int nMSISDNLen = (pRoamBill->servedMSISDN_BCDlen + 1) / 2;
        if (nMSISDNLen > 0)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_servedMSISDN_present_byte] |=
                CMCC_CS_RoamingRecord_servedMSISDN_present;
		    pDestBill->u.roamingRecord.servedMSISDN.length = 
                min(sizeof(pDestBill->u.roamingRecord.servedMSISDN.value), nMSISDNLen + 1);
            pDestBill->u.roamingRecord.servedMSISDN.value[0] = *(pRoamBill->servedMSISDN - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.roamingRecord.servedMSISDN.value + 1, pRoamBill->servedMSISDN, 
                pDestBill->u.roamingRecord.servedMSISDN.length - 1);
        }
	}

	pDestBill->u.roamingRecord.callingNumber.length = 0;
    pDestBill->u.roamingRecord.callingNumber.value = NULL;
	if (GET_CDR_FIELD_FILTER(callingNumber, RoamingRecord))				//主叫号码
	{
		int nCallerLen = (pRoamBill->callingNumber_BCDlen + 1) / 2;
        if (nCallerLen > 0)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_callingNumber_present_byte] |= 
                CMCC_CS_RoamingRecord_callingNumber_present;
		    pDestBill->u.roamingRecord.callingNumber.length = nCallerLen + 1;
		    pDestBill->u.roamingRecord.callingNumber.value = new (g_pAsnBuffForCMCC)BYTE[nCallerLen + 1];
            pDestBill->u.roamingRecord.callingNumber.value[0] = *(pRoamBill->callingNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.roamingRecord.callingNumber.value + 1, pRoamBill->callingNumber, nCallerLen);
        }
	}

    pDestBill->u.roamingRecord.roamingNumber.length = 0;
	if (GET_CDR_FIELD_FILTER(roamingNumber, RoamingRecord))				//漫游号码
	{
		int nRoamingLen = (pRoamBill->roamingNumber_BCDlen + 1) / 2;
        if (nRoamingLen > 0)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_roamingNumber_present_byte] |= 
                CMCC_CS_RoamingRecord_roamingNumber_present;
		    pDestBill->u.roamingRecord.roamingNumber.length = 
                min(sizeof(pDestBill->u.roamingRecord.roamingNumber.value), nRoamingLen + 1);
            pDestBill->u.roamingRecord.roamingNumber.value[0] = *(pRoamBill->roamingNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.roamingRecord.roamingNumber.value + 1, pRoamBill->roamingNumber, 
                pDestBill->u.roamingRecord.roamingNumber.length - 1);
        }
	}

    pDestBill->u.roamingRecord.recordingEntity.length = 0;
    if (GET_CDR_FIELD_FILTER(recordingEntity, RoamingRecord))
    {
	    int nLocalMSCLen = (pRoamBill->recordingEntity_BCDlen + 1) / 2;
        if (nLocalMSCLen > 0)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_recordingEntity_present_byte] |=
                CMCC_CS_RoamingRecord_recordingEntity_present;
            pDestBill->u.roamingRecord.recordingEntity.length = 
                min(sizeof(pDestBill->u.roamingRecord.recordingEntity.value), nLocalMSCLen + 1);
            pDestBill->u.roamingRecord.recordingEntity.value[0] = *(pRoamBill->recordingEntity - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.roamingRecord.recordingEntity.value + 1, pRoamBill->recordingEntity, 
                pDestBill->u.roamingRecord.recordingEntity.length - 1);
        }
    }

    pDestBill->u.roamingRecord.mscIncomingROUTE.u.rOUTEName = NULL;
	if (GET_CDR_FIELD_FILTER(mscIncomingROUTE, RoamingRecord))				//mscIncomingROUTE
	{
        if (pRoamBill->mscIncomingROUTE_ind == SHOW_ROUTE_NO)
        {
            if (pRoamBill->mscIncomingROUTE_ROUTENumber != 0xFFFF)
            {
                pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_mscIncomingROUTE_present_byte] |=
                    CMCC_CS_RoamingRecord_mscIncomingROUTE_present;
                pDestBill->u.roamingRecord.mscIncomingROUTE.choice = CMCC_CS_rOUTENumber_chosen;
                pDestBill->u.roamingRecord.mscIncomingROUTE.u.rOUTENumber = pRoamBill->mscIncomingROUTE_ROUTENumber;
            }
        }
        else if (pRoamBill->mscIncomingROUTE_ind == SHOW_ROUTE_NAME)
        {
            if (*(pRoamBill->mscIncomingROUTE_ROUTEName) != 0x20)
            {
                pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_mscIncomingROUTE_present_byte] |=
                    CMCC_CS_RoamingRecord_mscIncomingROUTE_present;
                pDestBill->u.roamingRecord.mscIncomingROUTE.choice = CMCC_CS_rOUTEName_chosen;
                pDestBill->u.roamingRecord.mscIncomingROUTE.u.rOUTEName = new (g_pAsnBuffForCMCC)char[11];
                memcpy(pDestBill->u.roamingRecord.mscIncomingROUTE.u.rOUTEName, pRoamBill->mscIncomingROUTE_ROUTEName, 10);
                pDestBill->u.roamingRecord.mscIncomingROUTE.u.rOUTEName[10] = '\0';
            }
        }
	}

    pDestBill->u.roamingRecord.mscOutgoingROUTE.u.rOUTEName = NULL;
	if (GET_CDR_FIELD_FILTER(mscOutgoingROUTE, RoamingRecord))				//mscOutgongingTKGP
	{
        if (pRoamBill->mscOutgoingROUTE_ind == SHOW_ROUTE_NO)
        {            
            if (pRoamBill->mscOutgoingROUTE_ROUTENumber != 0xFFFF)
            {
                pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_mscOutgoingROUTE_present_byte] |=
                    CMCC_CS_RoamingRecord_mscOutgoingROUTE_present;
                pDestBill->u.roamingRecord.mscOutgoingROUTE.choice = CMCC_CS_rOUTENumber_chosen;
                pDestBill->u.roamingRecord.mscOutgoingROUTE.u.rOUTENumber = pRoamBill->mscOutgoingROUTE_ROUTENumber;
            }
        }
        else if (pRoamBill->mscOutgoingROUTE_ind == SHOW_ROUTE_NAME)
        {
            if (*(pRoamBill->mscOutgoingROUTE_ROUTEName) != 0x20)
            {
                pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_mscOutgoingROUTE_present_byte] |=
                    CMCC_CS_RoamingRecord_mscOutgoingROUTE_present;
                pDestBill->u.roamingRecord.mscOutgoingROUTE.choice = CMCC_CS_rOUTEName_chosen;
                pDestBill->u.roamingRecord.mscOutgoingROUTE.u.rOUTEName = new (g_pAsnBuffForCMCC)char[11];
                memcpy(pDestBill->u.roamingRecord.mscOutgoingROUTE.u.rOUTEName, pRoamBill->mscOutgoingROUTE_ROUTEName, 10);
                pDestBill->u.roamingRecord.mscOutgoingROUTE.u.rOUTEName[10] = '\0';
            }
        }
	}

	if (GET_CDR_FIELD_FILTER(basicService, RoamingRecord))				//业务码
	{
        if (pRoamBill->TBS < 2)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_basicService_present_byte] |=
                CMCC_CS_RoamingRecord_basicService_present;
	        ConvertBasicServiceCode(pDestBill->u.roamingRecord.basicService, 
                pRoamBill->TBS, pRoamBill->ServiceCode);
        }
	}

	if (GET_CDR_FIELD_FILTER(transparencyIndicator, RoamingRecord))		//透明指示
	{
        if (pRoamBill->transparencyIndicator < 2)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_transparencyIndicator_present_byte] |= 
                CMCC_CS_RoamingRecord_transparencyIndicator_present;
		    pDestBill->u.roamingRecord.transparencyIndicator = (CMCC_CS_TransparencyInd)pRoamBill->transparencyIndicator;
        }
	}

	pDestBill->u.roamingRecord.changeOfService = NULL;
	if (GET_CDR_FIELD_FILTER(changeOfService, RoamingRecord))				//需要话单合并填写时间戳
	{
		//此域不提供
        //pDestBill->u.roamingRecord.bit_mask |= CMCC_CS_RoamingRecord_changeOfService_present;
	}

	pDestBill->u.roamingRecord.supplServicesUsed = NULL;
	if (GET_CDR_FIELD_FILTER(supplServicesUsed, RoamingRecord))			//需要话单合并填写时间戳
	{
        ConvertSScode((BYTE*)(&(pRoamBill->SS_code1)), &(pRoamBill->initial_year), &(pDestBill->u.roamingRecord.supplServicesUsed));
        
        if (pDestBill->u.roamingRecord.supplServicesUsed != NULL)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_supplServicesUsed_present_byte] |=
                CMCC_CS_RoamingRecord_supplServicesUsed_present;
        }
	}

	if (GET_CDR_FIELD_FILTER(seizureTime, RoamingRecord))		//answerTimestamp
	{
        if (pRoamBill->causeForTerm == unsuccessfulCallAttempt)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_seizureTime_present_byte] |= 
                CMCC_CS_RoamingRecord_seizureTime_present;
            ConvertTimeStamp(pDestBill->u.roamingRecord.seizureTime, &(pRoamBill->initial_year));			
        }
	}    

	if (GET_CDR_FIELD_FILTER(answerTime, RoamingRecord))		//answerTimestamp
	{
        if (pRoamBill->causeForTerm != unsuccessfulCallAttempt)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_answerTime_present_byte] |=
                CMCC_CS_RoamingRecord_answerTime_present;
            ConvertTimeStamp(pDestBill->u.roamingRecord.answerTime, &(pRoamBill->initial_year));			
        }
	}
	
	if (GET_CDR_FIELD_FILTER(releaseTime, RoamingRecord))		//releaseTimestamp
	{
		pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_releaseTime_present_byte] |=
            CMCC_CS_RoamingRecord_releaseTime_present;
		ConvertTimeStamp(pDestBill->u.roamingRecord.releaseTime, &(pRoamBill->end_year));			
	}

    if (GET_CDR_FIELD_FILTER(callDuration, RoamingRecord))
    {
        pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_callDuration_present_byte] |=
            CMCC_CS_RoamingRecord_callDuration_present;
        pDestBill->u.roamingRecord.callDuration = pRoamBill->callDuration;		
    }

    if (GET_CDR_FIELD_FILTER(causeForTerm, RoamingRecord))
    {
        pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_causeForTerm_present_byte] |=
            CMCC_CS_RoamingRecord_causeForTerm_present;    
        pDestBill->u.roamingRecord.causeForTerm = pRoamBill->causeForTerm;
    }

	if (GET_CDR_FIELD_FILTER(diagnostics, RoamingRecord))			//diagnostics
	{
        pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_diagnostics_present_byte] |=
            CMCC_CS_RoamingRecord_diagnostics_present;
		if (pRoamBill->diagnostics.CauseType == 0)
		{
			pDestBill->u.roamingRecord.diagnostics.choice = CMCC_CS_gsm0408Cause_chosen;
			pDestBill->u.roamingRecord.diagnostics.u.gsm0408Cause = pRoamBill->diagnostics.Cause.gsm0408Cause;
		}
		else if (pRoamBill->diagnostics.CauseType == 1)
		{
			pDestBill->u.roamingRecord.diagnostics.choice = CMCC_CS_gsm0902MapErrorValue_chosen;
			pDestBill->u.roamingRecord.diagnostics.u.gsm0902MapErrorValue = pRoamBill->diagnostics.Cause.gsm0902MapErrorValue;
		}
		else if (pRoamBill->diagnostics.CauseType == 2)
		{
			pDestBill->u.roamingRecord.diagnostics.choice = CMCC_CS_ccittQ767Cause_chosen;
			pDestBill->u.roamingRecord.diagnostics.u.ccittQ767Cause = pRoamBill->diagnostics.Cause.ccittQ767Cause;
		}
		else
		{
			pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_diagnostics_present_byte] &=
                ~CMCC_CS_RoamingRecord_diagnostics_present;
		}
	}

    if (GET_CDR_FIELD_FILTER(callReference, RoamingRecord))
    {
        if (pRoamBill->call_reference_len != 0)
        {
            int nCallReferenceLen = min(pRoamBill->call_reference_len,
                sizeof(pDestBill->u.roamingRecord.callReference.value));
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_callReference_present_byte] |=
                CMCC_CS_RoamingRecord_callReference_present;                 
            pDestBill->u.roamingRecord.callReference.length = nCallReferenceLen;
            memcpy(pDestBill->u.roamingRecord.callReference.value, 
                &(pRoamBill->call_reference[sizeof(pRoamBill->call_reference) - pRoamBill->call_reference_len]), nCallReferenceLen);   
        }
        else
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_callReference_present_byte] |=
                CMCC_CS_RoamingRecord_callReference_present;
            GetCallReference(pDestBill->u.roamingRecord.callReference,
                pRoamBill->ModuleNum, pRoamBill->count);
        }
    }

	if (GET_CDR_FIELD_FILTER(sequenceNumber, RoamingRecord))		//部分话单序列号
	{
		if (pRoamBill->record_type != SINGLE_BILL)
		{
			pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_sequenceNumber_present_byte] |=
                CMCC_CS_RoamingRecord_sequenceNumber_present;
			pDestBill->u.roamingRecord.sequenceNumber = pRoamBill->sequence;
		}
	}

	pDestBill->u.roamingRecord.recordExtensions = NULL;								//recordExtensions(目前不支持)
	if (GET_CDR_FIELD_FILTER(recordExtensions, RoamingRecord))		
	{
	}

	if (GET_CDR_FIELD_FILTER(networkCallReference, RoamingRecord))
	{
        if (pRoamBill->NetworkCallReferenceLen != 0)
        {
            int NetworkCallReferenceLen = min(pRoamBill->NetworkCallReferenceLen, 
                sizeof(pDestBill->u.roamingRecord.networkCallReference.value));
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_networkCallReference_present_byte] |=
                CMCC_CS_RoamingRecord_networkCallReference_present;
		    pDestBill->u.roamingRecord.networkCallReference.length = NetworkCallReferenceLen;
    	    memcpy(pDestBill->u.roamingRecord.networkCallReference.value, 
                &(pRoamBill->NetworkcallReferenceNumber[sizeof(pRoamBill->NetworkcallReferenceNumber) - pRoamBill->NetworkCallReferenceLen]), NetworkCallReferenceLen);
        }
	}

	pDestBill->u.roamingRecord.mSCAddress.length = 0;
    if (GET_CDR_FIELD_FILTER(mSCAddress, RoamingRecord))
	{
        int nMSCAddressLen = (pRoamBill->MSCAddress_BCDlen + 1) / 2;
        if (nMSCAddressLen > 0)
        {
		    pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_mSCAddress_present_byte] |=
                CMCC_CS_RoamingRecord_mSCAddress_present;   
            pDestBill->u.roamingRecord.mSCAddress.length = 
                min(sizeof(pDestBill->u.roamingRecord.mSCAddress.value), nMSCAddressLen + 1);
            pDestBill->u.roamingRecord.mSCAddress.value[0] = *(pRoamBill->MSCAddress - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.roamingRecord.mSCAddress.value + 1, pRoamBill->MSCAddress, 
                pDestBill->u.roamingRecord.mSCAddress.length - 1);
        }
	}

    if (GET_CDR_FIELD_FILTER(mscOutgoingCircuit, RoamingRecord))
	{
        if (pRoamBill->mscOutgoingCircuit != 0xFFFF)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_mscOutgoingCircuit_present_byte] |= 
                CMCC_CS_RoamingRecord_mscOutgoingCircuit_present;
            pDestBill->u.roamingRecord.mscOutgoingCircuit = pRoamBill->mscOutgoingCircuit;
        }
    }

    if (GET_CDR_FIELD_FILTER(mscIncomingCircuit, RoamingRecord))
	{
        if (pRoamBill->mscOutgoingCircuit != 0xFFFF)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_mscIncomingCircuit_present_byte] |=
                CMCC_CS_RoamingRecord_mscIncomingCircuit_present;
            pDestBill->u.roamingRecord.mscIncomingCircuit = pRoamBill->mscOutgoingCircuit;
        }
    }

    if (GET_CDR_FIELD_FILTER(orgMSCId, RoamingRecord))
	{
        if (pRoamBill->msc_id_len != 0)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_orgMSCId_present_byte] |=
                CMCC_CS_RoamingRecord_orgMSCId_present;
            pDestBill->u.roamingRecord.orgMSCId.length = min(3,pRoamBill->msc_id_len);
            memcpy(pDestBill->u.roamingRecord.orgMSCId.value, pRoamBill->orgMSCId, pDestBill->u.roamingRecord.orgMSCId.length);
        }
    }

    if (GET_CDR_FIELD_FILTER(subscriberCategory, RoamingRecord))
	{
        pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_subscriberCategory_present_byte] |=
            CMCC_CS_RoamingRecord_subscriberCategory_present;
        pDestBill->u.roamingRecord.subscriberCategory.length = 1;
        pDestBill->u.roamingRecord.subscriberCategory.value[0] = pRoamBill->subscriberCategory;
    }

    if (GET_CDR_FIELD_FILTER(cUGOutgoingAccessIndicator, RoamingRecord))
	{
		if (pRoamBill->CUGOutgoingAccessIndicator != 0)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_cUGOutgoingAccessIndicator_present_byte] |=
                CMCC_CS_RoamingRecord_cUGOutgoingAccessIndicator_present;
            pDestBill->u.roamingRecord.cUGOutgoingAccessIndicator = CMCC_CS_cUGCall;
        }
	}

    pDestBill->u.roamingRecord.cUGInterlockCode.length = 0;
    if (GET_CDR_FIELD_FILTER(cUGInterlockCode, RoamingRecord))
	{
		if (pRoamBill->CUGInterlockCode != 0xFFFFFFFF)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_cUGInterlockCode_present_byte] |=
                CMCC_CS_RoamingRecord_cUGInterlockCode_present;
            pDestBill->u.roamingRecord.cUGInterlockCode.length = 4;
            memcpy(pDestBill->u.roamingRecord.cUGInterlockCode.value, &(pRoamBill->CUGInterlockCode), 4);
        }
	}

    if (GET_CDR_FIELD_FILTER(hotBillingTag, RoamingRecord))
    {
        if (pRoamBill->HotBillingTag == HOT_BILLING)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_hotBillingTag_present_byte] |=
                CMCC_CS_RoamingRecord_hotBillingTag_present;		
            pDestBill->u.roamingRecord.hotBillingTag = CMCC_CS_hotBilling;
        }
        else
        {
            pDestBill->u.roamingRecord.hotBillingTag = CMCC_CS_noHotBilling;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(partialRecordType, RoamingRecord))      //1
    {
        if (pRoamBill->cause_for_partial_record !=  0xff) 
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_partialRecordType_present_byte] |=
                CMCC_CS_RoamingRecord_partialRecordType_present;
            pDestBill->u.roamingRecord.partialRecordType = (CMCC_CS_PartialRecordType)pRoamBill->cause_for_partial_record;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(optimalRoutingFlag, RoamingRecord))      //2
    {
        if (pRoamBill->SORIndication)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_optimalRoutingFlag_present_byte] |=
                CMCC_CS_RoamingRecord_optimalRoutingFlag_present;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(additionalChgInfo, RoamingRecord))		//附加计费信息
    {
        if (pRoamBill->chargeIndicator >= 0 && pRoamBill->chargeIndicator < 3)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_additionalChgInfo_present_byte] |=
                CMCC_CS_RoamingRecord_additionalChgInfo_present;
            pDestBill->u.roamingRecord.additionalChgInfo.bit_mask = CMCC_CS_chargeIndicator_present;
            pDestBill->u.roamingRecord.additionalChgInfo.chargeIndicator = pRoamBill->chargeIndicator;
        }
    }
    
   
    if (GET_CDR_FIELD_FILTER(chargedParty,RoamingRecord) )
    {
        if (pRoamBill->chargedParty ==0 || pRoamBill->chargedParty == 1)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_chargedParty_present_byte] |=
                CMCC_CS_RoamingRecord_chargedParty_present;
            pDestBill->u.roamingRecord.chargedParty = (CMCC_CS_ChargedParty)pRoamBill->chargedParty;
        }
    }
    
    pDestBill->u.roamingRecord.originalCalledNumber.length = 0;
    pDestBill->u.roamingRecord.originalCalledNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(originalCalledNumber,RoamingRecord))
    {
        int nOriginalCalledLen = (pRoamBill->originalCalledNumber_BCDlen + 1)/2;
        if (nOriginalCalledLen > 0 )
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_originalCalledNumber_present_byte] |=
                CMCC_CS_RoamingRecord_originalCalledNumber_present;
            pDestBill->u.roamingRecord.originalCalledNumber.length = nOriginalCalledLen + 1;
            pDestBill->u.roamingRecord.originalCalledNumber.value = new (g_pAsnBuffForCMCC)BYTE[nOriginalCalledLen + 1];
            pDestBill->u.roamingRecord.originalCalledNumber.value[0] = *(pRoamBill->originalCalledNumber - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.roamingRecord.originalCalledNumber.value + 1,pRoamBill->originalCalledNumber,
                pDestBill->u.roamingRecord.originalCalledNumber.length - 1);
        }
    }

    pDestBill->u.roamingRecord.callingChargeAreaCode.length = 0;
    if (GET_CDR_FIELD_FILTER(callingChargeAreaCode,RoamingRecord))
    {
        if (pRoamBill->callingChargeAreaCode[0] != 0xff &&
            pRoamBill->callingChargeAreaCode[1] != 0xff &&
            pRoamBill->callingChargeAreaCode[2] != 0xff)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_callingChargeAreaCode_present_byte] |=
                CMCC_CS_RoamingRecord_callingChargeAreaCode_present;
            pDestBill->u.roamingRecord.callingChargeAreaCode.length = 3;
            memcpy(pDestBill->u.roamingRecord.callingChargeAreaCode.value,pRoamBill->callingChargeAreaCode,3);
        }
    }
    
    pDestBill->u.roamingRecord.calledChargeAreaCode.length = 0;
    if (GET_CDR_FIELD_FILTER(calledChargeAreaCode,RoamingRecord))
    {
        if (pRoamBill->calledChargeAreaCode[0] != 0xff &&
            pRoamBill->calledChargeAreaCode[1] != 0xff &&
            pRoamBill->calledChargeAreaCode[2] != 0xff)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_calledChargeAreaCode_present_byte] |=
                CMCC_CS_RoamingRecord_calledChargeAreaCode_present;
            pDestBill->u.roamingRecord.calledChargeAreaCode.length = 3;
            memcpy(pDestBill->u.roamingRecord.calledChargeAreaCode.value,pRoamBill->calledChargeAreaCode,3);
        }
    }
  
    pDestBill->u.roamingRecord.callEmlppPriority.length = 0;
    if (GET_CDR_FIELD_FILTER(callEmlppPriority, RoamingRecord))
    {
        if (pRoamBill->callEmlppPriority != 0xF)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_callEmlppPriority_present_byte] |=
                CMCC_CS_RoamingRecord_callEmlppPriority_present;
            pDestBill->u.roamingRecord.callEmlppPriority.length = 1;
            pDestBill->u.roamingRecord.callEmlppPriority.value[0] = pRoamBill->callEmlppPriority;
        }
    }
    
    pDestBill->u.roamingRecord.eaSubscriberInfo.length = 0;
    if (GET_CDR_FIELD_FILTER(eaSubscriberInfo, RoamingRecord)) //此域无法得到
    {
        if (pRoamBill->eaSubscriberInfo[0] != 0xff &&
            pRoamBill->eaSubscriberInfo[1] != 0xff &&
            pRoamBill->eaSubscriberInfo[2] != 0xff)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_eaSubscriberInfo_present_byte] |= 
                CMCC_CS_RoamingRecord_eaSubscriberInfo_present; 
            pDestBill->u.roamingRecord.eaSubscriberInfo.length = 3;
            memcpy(pDestBill->u.roamingRecord.eaSubscriberInfo.value,pRoamBill->eaSubscriberInfo,3);
        }        
    }

    pDestBill->u.roamingRecord.selectedCIC.length = 0;
    if (GET_CDR_FIELD_FILTER(selectedCIC,RoamingRecord))
    {
        if (pRoamBill->selectedCIC[0] != 0xff &&
            pRoamBill->selectedCIC[1] != 0xff &&
            pRoamBill->selectedCIC[2] != 0xff)
        {
            pDestBill->u.roamingRecord.bit_mask[CMCC_CS_RoamingRecord_selectedCIC_present_byte] |= 
                CMCC_CS_RoamingRecord_selectedCIC_present; 
            pDestBill->u.roamingRecord.selectedCIC.length = 3;
            memcpy(pDestBill->u.roamingRecord.selectedCIC.value,pRoamBill->selectedCIC,3);
        }
    }
    
	return TRUE;
}

BOOL ConvertABill_MO_SMS(const BYTE* pSrcBill, const int nLen, 
                         const BYTE* pEventBill, const int nEventNum,                          
                         CMCC_CS_CallEventRecord* pDestBill)
{
	SMSMO_Bill_Report* pSmsmoBill = (SMSMO_Bill_Report *)pSrcBill;	
	
	pDestBill->choice = CMCC_CS_moSMSRecord_chosen;
	pDestBill->u.moSMSRecord.bit_mask = 0;

    if (GET_CDR_FIELD_FILTER(recordType, MOSMSRecord))
    {
        pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_recordType_present;
	    pDestBill->u.moSMSRecord.recordType = CMCC_CS_CallEventRecordType_moSMSRecord;
    }

    pDestBill->u.moSMSRecord.servedIMSI.length = 0;
    if (GET_CDR_FIELD_FILTER(servedIMSI, MOSMSRecord))
    {
        pDestBill->u.moSMSRecord.servedIMSI.length = 
            BCDtoRBCD(pDestBill->u.moSMSRecord.servedIMSI.value, pSmsmoBill->servedIMSI, 8);
        if(pDestBill->u.moSMSRecord.servedIMSI.length > 0)
        {
            pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_servedIMSI_present;
        }
    }

    pDestBill->u.moSMSRecord.servedIMEI.length = 0;
	if (GET_CDR_FIELD_FILTER(servedIMEI, MOSMSRecord))		//ServedIMEI
	{
		pDestBill->u.moSMSRecord.servedIMEI.length = 
            BCDtoRBCD(pDestBill->u.moSMSRecord.servedIMEI.value, pSmsmoBill->servedIMEI, 8);
		if (pDestBill->u.moSMSRecord.servedIMEI.length > 0)
        {
            pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_servedIMEI_present;
        }
	}

    pDestBill->u.moSMSRecord.servedMSISDN.length = 0;
	if (GET_CDR_FIELD_FILTER(servedMSISDN, MOSMSRecord))		//MSISDN
	{
		int nMSISDNLen = (pSmsmoBill->servedMSISDN_BCDlen + 1) / 2;
        if (nMSISDNLen > 0)
        {
            pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_servedMSISDN_present;
		    pDestBill->u.moSMSRecord.servedMSISDN.length = 
                min(sizeof(pDestBill->u.moSMSRecord.servedMSISDN.value), nMSISDNLen + 1);
            pDestBill->u.moSMSRecord.servedMSISDN.value[0] = *(pSmsmoBill->servedMSISDN - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.moSMSRecord.servedMSISDN.value + 1, pSmsmoBill->servedMSISDN, 
                pDestBill->u.moSMSRecord.servedMSISDN.length - 1);
        }
	}

    pDestBill->u.moSMSRecord.msClassmark.length = 0;
    if (GET_CDR_FIELD_FILTER(msClassmark, MOSMSRecord))
    {
        if(!(pSmsmoBill->MSClassmark[0] == 0
            && pSmsmoBill->MSClassmark[1] == 0
            && pSmsmoBill->MSClassmark[2] == 0)
            && !(pSmsmoBill->MSClassmark[0] == 0xFF
            && pSmsmoBill->MSClassmark[1] == 0xFF
            && pSmsmoBill->MSClassmark[2] == 0xFF))
        {
            pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_msClassmark_present;
            pDestBill->u.moSMSRecord.msClassmark.length = 3;
            pDestBill->u.moSMSRecord.msClassmark.value = new (g_pAsnBuffForCMCC)BYTE[3];
            memcpy(pDestBill->u.moSMSRecord.msClassmark.value, pSmsmoBill->MSClassmark, 3);
        }
    }

    pDestBill->u.moSMSRecord.serviceCentre.length = 0;
    if (GET_CDR_FIELD_FILTER(serviceCentre, MOSMSRecord))
    {
        int nSMSAddressLen = (pSmsmoBill->serviceCentre_BCDlen + 1) / 2;
        if (nSMSAddressLen > 0)
        {
            pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_serviceCentre_present;
            pDestBill->u.moSMSRecord.serviceCentre.length = 
                min(sizeof(pDestBill->u.moSMSRecord.serviceCentre.value), nSMSAddressLen + 1);
            pDestBill->u.moSMSRecord.serviceCentre.value[0] = *(pSmsmoBill->serviceCentre - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.moSMSRecord.serviceCentre.value + 1, pSmsmoBill->serviceCentre, 
                pDestBill->u.moSMSRecord.serviceCentre.length - 1);
        }
    }

    pDestBill->u.moSMSRecord.recordingEntity.length = 0;
    if (GET_CDR_FIELD_FILTER(recordingEntity, MOSMSRecord))
    {
        int nLocalMSCLen = (pSmsmoBill->recordingEntity_BCDlen + 1) / 2;
        if (nLocalMSCLen > 0)
        {
            pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_recordingEntity_present;
            pDestBill->u.moSMSRecord.recordingEntity.length = 
                min(sizeof(pDestBill->u.moSMSRecord.recordingEntity.value), nLocalMSCLen + 1);
            pDestBill->u.moSMSRecord.recordingEntity.value[0] = *(pSmsmoBill->recordingEntity - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.moSMSRecord.recordingEntity.value + 1, pSmsmoBill->recordingEntity, 
                pDestBill->u.moSMSRecord.recordingEntity.length - 1);
        }
    }

    pDestBill->u.moSMSRecord.location.locationAreaCode.length = 0;
    pDestBill->u.moSMSRecord.location.cellIdentifier.length = 0;
	if (GET_CDR_FIELD_FILTER(location, MOSMSRecord))			//位置区、小区
	{
        if (IsLACandCellIDValid(&(pSmsmoBill->served_location_LAI[3]), pSmsmoBill->served_location_cellID))
        {
		    pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_location_present;
		    pDestBill->u.moSMSRecord.location.locationAreaCode.length = 2;
		    memcpy(pDestBill->u.moSMSRecord.location.locationAreaCode.value, pSmsmoBill->served_location_LAI + 3, 2);
		    pDestBill->u.moSMSRecord.location.cellIdentifier.length = 2;
		    memcpy(pDestBill->u.moSMSRecord.location.cellIdentifier.value, pSmsmoBill->served_location_cellID, 2);
        }
	}

    pDestBill->u.moSMSRecord.messageReference.length = 0;
    if (GET_CDR_FIELD_FILTER(messageReference, MOSMSRecord))
    {
        pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_messageReference_present;
        pDestBill->u.moSMSRecord.messageReference.length = 1;
        pDestBill->u.moSMSRecord.messageReference.value = new (g_pAsnBuffForCMCC)BYTE[1];
        pDestBill->u.moSMSRecord.messageReference.value[0] = pSmsmoBill->MessageReference;
    }

    if (GET_CDR_FIELD_FILTER(originationTime, MOSMSRecord))
    {
        pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_originationTime_present;
        ConvertTimeStamp(pDestBill->u.moSMSRecord.originationTime, &(pSmsmoBill->OriginationDeliveryTime_year));
    }

	if (GET_CDR_FIELD_FILTER(smsResult, MOSMSRecord))
	{
        pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_smsResult_present;
		if (pSmsmoBill->smsResult.CauseType == 0
            && pSmsmoBill->smsResult.Cause.gsm0408Cause != 0)
		{
			pDestBill->u.moSMSRecord.smsResult.choice = CMCC_CS_gsm0408Cause_chosen;
			pDestBill->u.moSMSRecord.smsResult.u.gsm0408Cause = pSmsmoBill->smsResult.Cause.gsm0408Cause;
		}
		else if (pSmsmoBill->smsResult.CauseType == 1
            && pSmsmoBill->smsResult.Cause.gsm0902MapErrorValue != 0)
		{
			pDestBill->u.moSMSRecord.smsResult.choice = CMCC_CS_gsm0902MapErrorValue_chosen;
			pDestBill->u.moSMSRecord.smsResult.u.gsm0902MapErrorValue = pSmsmoBill->smsResult.Cause.gsm0902MapErrorValue;
		}
		else if (pSmsmoBill->smsResult.CauseType == 2
            && pSmsmoBill->smsResult.Cause.ccittQ767Cause != 0)
		{
			pDestBill->u.moSMSRecord.smsResult.choice = CMCC_CS_ccittQ767Cause_chosen;
			pDestBill->u.moSMSRecord.smsResult.u.ccittQ767Cause = pSmsmoBill->smsResult.Cause.ccittQ767Cause;
		}
		else
		{
			pDestBill->u.moSMSRecord.bit_mask &=
                ~CMCC_CS_MOSMSRecord_smsResult_present;
		}
    }

    pDestBill->u.moSMSRecord.recordExtensions = NULL;				//recordExtensions(目前不提供)

	pDestBill->u.moSMSRecord.destinationNumber.length = 0;
    pDestBill->u.moSMSRecord.destinationNumber.value = NULL;
	if (GET_CDR_FIELD_FILTER(destinationNumber, MOSMSRecord))
	{
		int nCalledLen = (pSmsmoBill->destinationNumber_BCDlen + 1) / 2;
        if (nCalledLen > 0)
        {
            pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_destinationNumber_present;
		    pDestBill->u.moSMSRecord.destinationNumber.length = nCalledLen + 1;
		    pDestBill->u.moSMSRecord.destinationNumber.value = new (g_pAsnBuffForCMCC)BYTE[nCalledLen + 1];
            pDestBill->u.moSMSRecord.destinationNumber.value[0] = *(pSmsmoBill->destinationNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.moSMSRecord.destinationNumber.value + 1, pSmsmoBill->destinationNumber, nCalledLen);
        }
	}

	pDestBill->u.moSMSRecord.cAMELSMSInformation.callingPartyNumber.length = 0;
    pDestBill->u.moSMSRecord.cAMELSMSInformation.callingPartyNumber.value = NULL;
    pDestBill->u.moSMSRecord.cAMELSMSInformation.destinationSubscriberNumber.length = 0;
    pDestBill->u.moSMSRecord.cAMELSMSInformation.destinationSubscriberNumber.value = NULL;

	if (GET_CDR_FIELD_FILTER(systemType, MOSMSRecord))		//SystemType
	{
        switch((ACCESS_NETWORK_TYPE)pSmsmoBill->systemType)
        {
        case ACCESS_BSC :
            pDestBill->u.moSMSRecord.systemType = CMCC_CS_gERAN;
            pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_systemType_present;
            break;
        case ACCESS_RNC :
            pDestBill->u.moSMSRecord.systemType = CMCC_CS_iuUTRAN;
            pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_systemType_present;
            break;
        case ACCESS_UNKNOWN:
            pDestBill->u.moSMSRecord.systemType = CMCC_CS_unknown;
            pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_systemType_present;
            break;
        }
	}

    pDestBill->u.moSMSRecord.subscriberCategory.length = 0;
    if (GET_CDR_FIELD_FILTER(subscriberCategory, MOSMSRecord))
	{
        pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_subscriberCategory_present;
        pDestBill->u.moSMSRecord.subscriberCategory.length = 1;
        pDestBill->u.moSMSRecord.subscriberCategory.value[0] = pSmsmoBill->subscriberCategory;
    }

    pDestBill->u.moSMSRecord.firstmccmnc.length = 0;
    if (GET_CDR_FIELD_FILTER(firstmccmnc, MOSMSRecord))
	{
        if (!(pSmsmoBill->served_location_LAI[0] == 0xFF
            && pSmsmoBill->served_location_LAI[1] == 0xFF
            && pSmsmoBill->served_location_LAI[2] == 0xFF)
            && !(pSmsmoBill->served_location_LAI[0] == 0
            && pSmsmoBill->served_location_LAI[1] == 0
            && pSmsmoBill->served_location_LAI[2] == 0))
        {
            pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_firstmccmnc_present;
            pDestBill->u.moSMSRecord.firstmccmnc.length = 3;
            memcpy(pDestBill->u.moSMSRecord.firstmccmnc.value, pSmsmoBill->served_location_LAI, 3);
        }
    }

    pDestBill->u.moSMSRecord.orgRNCorBSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgRNCorBSCId, MOSMSRecord))
	{
        //如果Rnc_Bsc_id全0，则不进行编码
        if (pSmsmoBill->rnc_bsc_id_len != 0)
        {
            pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_orgRNCorBSCId_present;
            pDestBill->u.moSMSRecord.orgRNCorBSCId.length = min(3,pSmsmoBill->rnc_bsc_id_len);
            memcpy(pDestBill->u.moSMSRecord.orgRNCorBSCId.value, pSmsmoBill->orgRNCorBSCId, pDestBill->u.moSMSRecord.orgRNCorBSCId.length);
        }
    }

    pDestBill->u.moSMSRecord.orgMSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgMSCId, MOSMSRecord))
	{
        if (pSmsmoBill->msc_id_len != 0)
        {
            pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_orgMSCId_present;
            pDestBill->u.moSMSRecord.orgMSCId.length = min(3,pSmsmoBill->msc_id_len);
            memcpy(pDestBill->u.moSMSRecord.orgMSCId.value, pSmsmoBill->orgMSCId, pDestBill->u.moSMSRecord.orgMSCId.length);
        }
    }

    pDestBill->u.moSMSRecord.smstext.length = 0;
    pDestBill->u.moSMSRecord.smstext.value = NULL;
    if (GET_CDR_FIELD_FILTER(smstext, MOSMSRecord))
	{
        if (pSmsmoBill->smstext_len > 0)
        {
			BYTE* pSmsText = NULL;

			pSmsText = pSmsmoBill->VarPart + pSmsmoBill->fcidata_len;

            pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_smstext_present;
            pDestBill->u.moSMSRecord.smstext.length = pSmsmoBill->smstext_len;
            if(pDestBill->u.moSMSRecord.smstext.length > SMSTEXT_LENGTH)
            {
                pDestBill->u.moSMSRecord.smstext.length = SMSTEXT_LENGTH;
            }
            pDestBill->u.moSMSRecord.smstext.value = new (g_pAsnBuffForCMCC)unsigned char[pDestBill->u.moSMSRecord.smstext.length];
            memcpy(pDestBill->u.moSMSRecord.smstext.value, pSmsText, pDestBill->u.moSMSRecord.smstext.length);
        }
    }

    pDestBill->u.moSMSRecord.smsUserDataType.length = 0;
    if (GET_CDR_FIELD_FILTER(smsUserDataType, MOSMSRecord))
    {
        pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_smsUserDataType_present;
        pDestBill->u.moSMSRecord.smsUserDataType.length = 1;
        pDestBill->u.moSMSRecord.smsUserDataType.value[0] = pSmsmoBill->smsUserDataType;
    }
    
    if (GET_CDR_FIELD_FILTER(maximumNumberOfSMSInTheConcatenatedSMS, MOSMSRecord))
    {
        if(pSmsmoBill->maximumNumberOfSMSInTheConcatenatedSMS != 0)
        {
            pDestBill->u.moSMSRecord.bit_mask |= 
                CMCC_CS_MOSMSRecord_maximumNumberOfSMSInTheConcatenatedSMS_present;
            pDestBill->u.moSMSRecord.maximumNumberOfSMSInTheConcatenatedSMS = 
                pSmsmoBill->maximumNumberOfSMSInTheConcatenatedSMS;
        }
    }

    if (GET_CDR_FIELD_FILTER(concatenatedSMSReferenceNumber, MOSMSRecord))
    {
        if(pSmsmoBill->maximumNumberOfSMSInTheConcatenatedSMS != 0)
        {
            pDestBill->u.moSMSRecord.bit_mask |= 
                CMCC_CS_MOSMSRecord_concatenatedSMSReferenceNumber_present;
            pDestBill->u.moSMSRecord.concatenatedSMSReferenceNumber = 
                pSmsmoBill->concatenatedSMSReferenceNumber;
        }
    }

    if (GET_CDR_FIELD_FILTER(sequenceNumberOfTheCurrentSMS, MOSMSRecord))
    {
        if(pSmsmoBill->maximumNumberOfSMSInTheConcatenatedSMS != 0)
        {
            pDestBill->u.moSMSRecord.bit_mask |= 
                CMCC_CS_MOSMSRecord_sequenceNumberOfTheCurrentSMS_present;
            pDestBill->u.moSMSRecord.sequenceNumberOfTheCurrentSMS = 
                pSmsmoBill->sequenceNumberOfTheCurrentSMS;
        }
    }

	if (GET_CDR_FIELD_FILTER(hotBillingTag, MOSMSRecord))
	{
		if (pSmsmoBill->HotBillingTag == HOT_BILLING)
		{
		    pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_hotBillingTag_present;		
            pDestBill->u.moSMSRecord.hotBillingTag = CMCC_CS_hotBilling;
		}
		else
		{
			pDestBill->u.moSMSRecord.hotBillingTag = CMCC_CS_noHotBilling;
		}
	}

    pDestBill->u.moSMSRecord.classmark3.length = 0;
    if (GET_CDR_FIELD_FILTER(classmark3, MOSMSRecord))     //1
    {
        if (pSmsmoBill->Classmark3Indication)
        {
            pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_classmark3_present;
            pDestBill->u.moSMSRecord.classmark3.length = 2;
            memcpy(pDestBill->u.moSMSRecord.classmark3.value,&(pSmsmoBill->Classmark3Information),2) ;
        }
    }
    pDestBill->u.moSMSRecord.globalAreaID.length = 0;
    if (GET_CDR_FIELD_FILTER(globalAreaID, MOSMSRecord))      //2
    {
        if (IsLACandCellIDValid(&(pSmsmoBill->served_location_LAI[3]),pSmsmoBill->served_location_cellID))        
        {
            pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_globalAreaID_present;
            pDestBill->u.moSMSRecord.globalAreaID.length = 7;
            memcpy(pDestBill->u.moSMSRecord.globalAreaID.value,pSmsmoBill->served_location_LAI,5);
            memcpy(pDestBill->u.moSMSRecord.globalAreaID.value + 5,pSmsmoBill->served_location_cellID,2);
        }
    }    

    if (GET_CDR_FIELD_FILTER(basicService, MOSMSRecord))				//业务码
    {
        if (pSmsmoBill->TBS < 2)
        {
            pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_basicService_present;
            ConvertBasicServiceCode(pDestBill->u.moSMSRecord.basicService, 
                pSmsmoBill->TBS, pSmsmoBill->ServiceCode);
        }
    }

    if (GET_CDR_FIELD_FILTER(additionalChgInfo, MOSMSRecord))		//附加计费信息
    {
        if (pSmsmoBill->chargeIndicator >= 0 && pSmsmoBill->chargeIndicator < 3)
        {
            pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_additionalChgInfo_present;
            pDestBill->u.moSMSRecord.additionalChgInfo.bit_mask = CMCC_CS_chargeIndicator_present;
            pDestBill->u.moSMSRecord.additionalChgInfo.chargeIndicator = pSmsmoBill->chargeIndicator;
        }
    }

    if (GET_CDR_FIELD_FILTER(chargedParty,MOSMSRecord))
    {
        if (pSmsmoBill->chargedParty == 0 || pSmsmoBill->chargedParty == 1)
        {
            pDestBill->u.moSMSRecord.bit_mask |= CMCC_CS_MOSMSRecord_chargedParty_present;
            pDestBill->u.moSMSRecord.chargedParty = (CMCC_CS_ChargedParty)pSmsmoBill->chargedParty;
        }
    }
    
	return TRUE;
}

BOOL ConvertABill_MT_SMS(const BYTE* pSrcBill, const int nLen,
                         const BYTE* pEventBill, const int nEventNum,                          
                         CMCC_CS_CallEventRecord* pDestBill)
{
	SMSMT_Bill_Report* pSmsmtBill = (SMSMT_Bill_Report *)pSrcBill;	
	
	pDestBill->choice = CMCC_CS_mtSMSRecord_chosen;
	pDestBill->u.mtSMSRecord.bit_mask = 0;

    if (GET_CDR_FIELD_FILTER(recordType, MTSMSRecord))
    {
        pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_recordType_present;
	    pDestBill->u.mtSMSRecord.recordType = CMCC_CS_CallEventRecordType_mtSMSRecord;
    }

    pDestBill->u.mtSMSRecord.serviceCentre.length = 0;
    if (GET_CDR_FIELD_FILTER(serviceCentre, MTSMSRecord))
    {
        int nSMSAddressLen = (pSmsmtBill->serviceCentre_BCDlen + 1) / 2;
        if (nSMSAddressLen > 0)
        {
            pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_serviceCentre_present;
            pDestBill->u.mtSMSRecord.serviceCentre.length = 
                min(sizeof(pDestBill->u.mtSMSRecord.serviceCentre.value), nSMSAddressLen + 1);
            pDestBill->u.mtSMSRecord.serviceCentre.value[0] = *(pSmsmtBill->serviceCentre - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.mtSMSRecord.serviceCentre.value + 1, pSmsmtBill->serviceCentre, 
                pDestBill->u.mtSMSRecord.serviceCentre.length - 1);
        }
    }


	pDestBill->u.mtSMSRecord.servedIMEI.length = 0;
    if (GET_CDR_FIELD_FILTER(servedIMEI, MTSMSRecord))		//ServedIMEI
	{
		pDestBill->u.mtSMSRecord.servedIMEI.length = 
            BCDtoRBCD(pDestBill->u.mtSMSRecord.servedIMEI.value, pSmsmtBill->servedIMEI, 8);
        if (pDestBill->u.mtSMSRecord.servedIMEI.length > 0)
        {
		    pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_servedIMEI_present;        
        }
	}

    pDestBill->u.mtSMSRecord.servedMSISDN.length = 0;
	if (GET_CDR_FIELD_FILTER(servedMSISDN, MTSMSRecord))		//MSISDN
	{
		int nMSISDNLen = (pSmsmtBill->servedMSISDN_BCDlen + 1) / 2;
        if (nMSISDNLen > 0)
        {
            pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_servedMSISDN_present;
		    pDestBill->u.mtSMSRecord.servedMSISDN.length = 
                min(sizeof(pDestBill->u.mtSMSRecord.servedMSISDN.value), nMSISDNLen + 1);
            pDestBill->u.mtSMSRecord.servedMSISDN.value[0] = *(pSmsmtBill->servedMSISDN - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.mtSMSRecord.servedMSISDN.value + 1, pSmsmtBill->servedMSISDN, 
                pDestBill->u.mtSMSRecord.servedMSISDN.length - 1);
        }
	}

    pDestBill->u.mtSMSRecord.msClassmark.length = 0;
    if (GET_CDR_FIELD_FILTER(msClassmark, MTSMSRecord))
    {
        if(!(pSmsmtBill->MSClassmark[0] == 0
            && pSmsmtBill->MSClassmark[1] == 0
            && pSmsmtBill->MSClassmark[2] == 0)
            && !(pSmsmtBill->MSClassmark[0] == 0xFF
            && pSmsmtBill->MSClassmark[1] == 0xFF
            && pSmsmtBill->MSClassmark[2] == 0xFF))
        {
            pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_msClassmark_present;
            pDestBill->u.mtSMSRecord.msClassmark.length = 3;
            pDestBill->u.mtSMSRecord.msClassmark.value = new (g_pAsnBuffForCMCC)BYTE[3];
            memcpy(pDestBill->u.mtSMSRecord.msClassmark.value, pSmsmtBill->MSClassmark, 3);
        }
    }

    pDestBill->u.mtSMSRecord.recordingEntity.length = 0;
    if (GET_CDR_FIELD_FILTER(recordingEntity, MTSMSRecord))
    {
        int nLocalMSCLen = (pSmsmtBill->recordingEntity_BCDlen + 1) / 2;
        if (nLocalMSCLen > 0)
        {
            pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_recordingEntity_present;
            pDestBill->u.mtSMSRecord.recordingEntity.length = 
                min(sizeof(pDestBill->u.mtSMSRecord.recordingEntity.value), nLocalMSCLen + 1);
            pDestBill->u.mtSMSRecord.recordingEntity.value[0] = *(pSmsmtBill->recordingEntity - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.mtSMSRecord.recordingEntity.value + 1, pSmsmtBill->recordingEntity, 
                pDestBill->u.mtSMSRecord.recordingEntity.length - 1);
        }
    }

    pDestBill->u.mtSMSRecord.location.locationAreaCode.length = 0;
    pDestBill->u.mtSMSRecord.location.cellIdentifier.length = 0;
	if (GET_CDR_FIELD_FILTER(location, MTSMSRecord))			//位置区、小区
	{
        if (IsLACandCellIDValid(&(pSmsmtBill->served_location_LAI[3]), pSmsmtBill->served_location_cellID))
        {
            pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_location_present;
		    pDestBill->u.mtSMSRecord.location.locationAreaCode.length = 2;
		    memcpy(pDestBill->u.mtSMSRecord.location.locationAreaCode.value, pSmsmtBill->served_location_LAI + 3, 2);
		    pDestBill->u.mtSMSRecord.location.cellIdentifier.length = 2;
		    memcpy(pDestBill->u.mtSMSRecord.location.cellIdentifier.value, pSmsmtBill->served_location_cellID, 2);
        }
	}

    if (GET_CDR_FIELD_FILTER(deliveryTime, MTSMSRecord))
    {
        pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_deliveryTime_present;
        ConvertTimeStamp(pDestBill->u.mtSMSRecord.deliveryTime, &(pSmsmtBill->OriginationDeliveryTime_year));
    }

	if (GET_CDR_FIELD_FILTER(smsResult, MTSMSRecord))
	{
        pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_smsResult_present;
		if (pSmsmtBill->smsResult.CauseType == 0
            && pSmsmtBill->smsResult.Cause.gsm0408Cause != 0)
		{
			pDestBill->u.mtSMSRecord.smsResult.choice = CMCC_CS_gsm0408Cause_chosen;
			pDestBill->u.mtSMSRecord.smsResult.u.gsm0408Cause = pSmsmtBill->smsResult.Cause.gsm0408Cause;
		}
		else if (pSmsmtBill->smsResult.CauseType == 1
            && pSmsmtBill->smsResult.Cause.gsm0902MapErrorValue != 0)
		{
			pDestBill->u.mtSMSRecord.smsResult.choice = CMCC_CS_gsm0902MapErrorValue_chosen;
			pDestBill->u.mtSMSRecord.smsResult.u.gsm0902MapErrorValue = pSmsmtBill->smsResult.Cause.gsm0902MapErrorValue;
		}
		else if (pSmsmtBill->smsResult.CauseType == 2
            && pSmsmtBill->smsResult.Cause.ccittQ767Cause != 0)
		{
			pDestBill->u.mtSMSRecord.smsResult.choice = CMCC_CS_ccittQ767Cause_chosen;
			pDestBill->u.mtSMSRecord.smsResult.u.ccittQ767Cause = pSmsmtBill->smsResult.Cause.ccittQ767Cause;
		}
		else
		{
			pDestBill->u.mtSMSRecord.bit_mask &=
                ~CMCC_CS_MTSMSRecord_smsResult_present;
		}
    }

    pDestBill->u.mtSMSRecord.recordExtensions = NULL;				//recordExtensions(目前不提供)
	if (GET_CDR_FIELD_FILTER(systemType, MTSMSRecord))		//SystemType
	{
        switch((ACCESS_NETWORK_TYPE)pSmsmtBill->systemType)
        {
        case ACCESS_BSC :
            pDestBill->u.mtSMSRecord.systemType = CMCC_CS_gERAN;
            pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_systemType_present;
            break;
        case ACCESS_RNC :
            pDestBill->u.mtSMSRecord.systemType = CMCC_CS_iuUTRAN;
            pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_systemType_present;
            break;
        case ACCESS_UNKNOWN:
            pDestBill->u.mtSMSRecord.systemType = CMCC_CS_unknown;
            pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_systemType_present;
            break;
        }
	}

    pDestBill->u.mtSMSRecord.subscriberCategory.length = 0;
    if (GET_CDR_FIELD_FILTER(subscriberCategory, MTSMSRecord))
	{
        pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_subscriberCategory_present;
        pDestBill->u.mtSMSRecord.subscriberCategory.length = 1;
        pDestBill->u.mtSMSRecord.subscriberCategory.value[0] = pSmsmtBill->subscriberCategory;
    }

    pDestBill->u.mtSMSRecord.firstmccmnc.length = 0;
    if (GET_CDR_FIELD_FILTER(firstmccmnc, MTSMSRecord))
	{
        if (!(pSmsmtBill->served_location_LAI[0] == 0xFF
            && pSmsmtBill->served_location_LAI[1] == 0xFF
            && pSmsmtBill->served_location_LAI[2] == 0xFF)
            && !(pSmsmtBill->served_location_LAI[0] == 0
            && pSmsmtBill->served_location_LAI[1] == 0
            && pSmsmtBill->served_location_LAI[2] == 0))
        {
            pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_firstmccmnc_present;
            pDestBill->u.mtSMSRecord.firstmccmnc.length = 3;
            memcpy(pDestBill->u.mtSMSRecord.firstmccmnc.value, pSmsmtBill->served_location_LAI, 3);
        }
    }

    pDestBill->u.mtSMSRecord.orgRNCorBSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgRNCorBSCId, MTSMSRecord))
	{
        //如果Rnc_Bsc_id全0，则不进行编码
        if (pSmsmtBill->rnc_bsc_id_len != 0)
        {
            pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_orgRNCorBSCId_present;
            pDestBill->u.mtSMSRecord.orgRNCorBSCId.length = min(3,pSmsmtBill->rnc_bsc_id_len);
            memcpy(pDestBill->u.mtSMSRecord.orgRNCorBSCId.value, pSmsmtBill->orgRNCorBSCId, pDestBill->u.mtSMSRecord.orgRNCorBSCId.length);
        }
    }

    pDestBill->u.mtSMSRecord.orgMSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgMSCId, MTSMSRecord))
	{
        if (pSmsmtBill->msc_id_len != 0)
        {
            pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_orgMSCId_present;
            pDestBill->u.mtSMSRecord.orgMSCId.length = min(3,pSmsmtBill->msc_id_len);
            memcpy(pDestBill->u.mtSMSRecord.orgMSCId.value, pSmsmtBill->orgMSCId, pDestBill->u.mtSMSRecord.orgMSCId.length);
        }
    }

    pDestBill->u.mtSMSRecord.smstext.length = 0;
    pDestBill->u.mtSMSRecord.smstext.value = NULL;
    if (GET_CDR_FIELD_FILTER(smstext, MTSMSRecord))   //此域未确定转换关系
	{
		if (pSmsmtBill->smstext_len > 0)
		{
			BYTE* pSmsText = NULL;
			pSmsText = pSmsmtBill->VarPart;
			
            pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_smstext_present;
            pDestBill->u.mtSMSRecord.smstext.length = pSmsmtBill->smstext_len;
            if(pDestBill->u.mtSMSRecord.smstext.length > SMSTEXT_LENGTH)
            {
                pDestBill->u.mtSMSRecord.smstext.length = SMSTEXT_LENGTH;
            }
            pDestBill->u.mtSMSRecord.smstext.value = new (g_pAsnBuffForCMCC)unsigned char[pDestBill->u.mtSMSRecord.smstext.length];
            memcpy(pDestBill->u.mtSMSRecord.smstext.value, pSmsText, pDestBill->u.mtSMSRecord.smstext.length);
        }
    }

	pDestBill->u.mtSMSRecord.origination.length = 0;
    pDestBill->u.mtSMSRecord.origination.value = NULL;
    if (GET_CDR_FIELD_FILTER(origination, MTSMSRecord))
	{
		int nCallerLen = (pSmsmtBill->callingNumber_BCDlen + 1) / 2;
        if (nCallerLen > 0)
        {
            pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_origination_present;
		    pDestBill->u.mtSMSRecord.origination.length = nCallerLen + 1;
		    pDestBill->u.mtSMSRecord.origination.value = new (g_pAsnBuffForCMCC)BYTE[nCallerLen + 1];
            pDestBill->u.mtSMSRecord.origination.value[0] = *(pSmsmtBill->callingNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.mtSMSRecord.origination.value + 1, pSmsmtBill->callingNumber, nCallerLen);
        }
	}

    pDestBill->u.mtSMSRecord.smsUserDataType.length = 0;
    if (GET_CDR_FIELD_FILTER(smsUserDataType, MTSMSRecord))
    {
        pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_smsUserDataType_present;
        pDestBill->u.mtSMSRecord.smsUserDataType.length = 1;
        pDestBill->u.mtSMSRecord.smsUserDataType.value[0] = pSmsmtBill->smsUserDataType;
    }
    
    if (GET_CDR_FIELD_FILTER(maximumNumberOfSMSInTheConcatenatedSMS, MTSMSRecord))
    {
        if(pSmsmtBill->maximumNumberOfSMSInTheConcatenatedSMS != 0)
        {
            pDestBill->u.mtSMSRecord.bit_mask |= 
                CMCC_CS_MTSMSRecord_maximumNumberOfSMSInTheConcatenatedSMS_present;
            pDestBill->u.mtSMSRecord.maximumNumberOfSMSInTheConcatenatedSMS = 
                pSmsmtBill->maximumNumberOfSMSInTheConcatenatedSMS;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(concatenatedSMSReferenceNumber, MTSMSRecord))
    {
        if(pSmsmtBill->maximumNumberOfSMSInTheConcatenatedSMS != 0)
        {
            pDestBill->u.mtSMSRecord.bit_mask |= 
                CMCC_CS_MTSMSRecord_concatenatedSMSReferenceNumber_present;
            pDestBill->u.mtSMSRecord.concatenatedSMSReferenceNumber = 
                pSmsmtBill->concatenatedSMSReferenceNumber;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(sequenceNumberOfTheCurrentSMS, MTSMSRecord))
    {
        if(pSmsmtBill->maximumNumberOfSMSInTheConcatenatedSMS != 0)
        {
            pDestBill->u.mtSMSRecord.bit_mask |= 
                CMCC_CS_MTSMSRecord_sequenceNumberOfTheCurrentSMS_present;
            pDestBill->u.mtSMSRecord.sequenceNumberOfTheCurrentSMS = 
                pSmsmtBill->sequenceNumberOfTheCurrentSMS;
        }
    }

	if (GET_CDR_FIELD_FILTER(hotBillingTag, MTSMSRecord))
	{
		if (pSmsmtBill->HotBillingTag == HOT_BILLING)
		{
		    pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_hotBillingTag_present;		
            pDestBill->u.mtSMSRecord.hotBillingTag = CMCC_CS_hotBilling;
		}
		else
		{
			pDestBill->u.mtSMSRecord.hotBillingTag = CMCC_CS_noHotBilling;
		}
	}

    pDestBill->u.mtSMSRecord.cAMELSMSInformation.callingPartyNumber.length = 0;
    pDestBill->u.mtSMSRecord.cAMELSMSInformation.callingPartyNumber.value = NULL;
    pDestBill->u.mtSMSRecord.cAMELSMSInformation.destinationSubscriberNumber.length = 0;
    pDestBill->u.mtSMSRecord.cAMELSMSInformation.destinationSubscriberNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(cAMELSMSInformation, MTSMSRecord))     //1
    {
        //pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_cAMELSMSInformation_present;
    }
    
    pDestBill->u.mtSMSRecord.classmark3.length = 0;
    if (GET_CDR_FIELD_FILTER(classmark3, MTSMSRecord))      //2
    {
        if (pSmsmtBill->Classmark3Indication)
        {
            pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_classmark3_present;
            pDestBill->u.mtSMSRecord.classmark3.length = 2;
            memcpy(pDestBill->u.mtSMSRecord.classmark3.value,&(pSmsmtBill->Classmark3Information),2);            
        }
    }

    pDestBill->u.mtSMSRecord.globalAreaID.length = 0; 
    if (GET_CDR_FIELD_FILTER(globalAreaID, MTSMSRecord))     //3
    {
        if (IsLACandCellIDValid(&(pSmsmtBill->served_location_LAI[3]), pSmsmtBill->served_location_cellID))
        {
            pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_globalAreaID_present;
            pDestBill->u.mtSMSRecord.globalAreaID.length = 7;            
            memcpy(pDestBill->u.mtSMSRecord.globalAreaID.value,pSmsmtBill->served_location_LAI,5);
            memcpy(pDestBill->u.mtSMSRecord.globalAreaID.value + 5,pSmsmtBill->served_location_cellID,2);
        }
    }
    
    pDestBill->u.mtSMSRecord.servedIMSI.length = 0;
    if (GET_CDR_FIELD_FILTER(servedIMSI, MTSMSRecord))
    {
        pDestBill->u.mtSMSRecord.servedIMSI.length = 
            BCDtoRBCD(pDestBill->u.mtSMSRecord.servedIMSI.value, pSmsmtBill->servedIMSI, 8);
        if(pDestBill->u.mtSMSRecord.servedIMSI.length > 0)
        {
            pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_servedIMSI_present;
        }
    }
  
    if (GET_CDR_FIELD_FILTER(basicService, MTSMSRecord))				//业务码
    {
        if (pSmsmtBill->TBS < 2)
        {
            pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_basicService_present;
            ConvertBasicServiceCode(pDestBill->u.mtSMSRecord.basicService, 
                pSmsmtBill->TBS, pSmsmtBill->ServiceCode);
        }
    }
    
    if (GET_CDR_FIELD_FILTER(additionalChgInfo, MTSMSRecord))		//附加计费信息
    {
        if (pSmsmtBill->chargeIndicator >= 0 && pSmsmtBill->chargeIndicator < 3)
        {
            pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_additionalChgInfo_present;
            pDestBill->u.mtSMSRecord.additionalChgInfo.bit_mask = CMCC_CS_chargeIndicator_present;
            pDestBill->u.mtSMSRecord.additionalChgInfo.chargeIndicator = pSmsmtBill->chargeIndicator;
        }
    }

    if (GET_CDR_FIELD_FILTER(chargedParty,MTSMSRecord) )
    {
        if (pSmsmtBill->chargedParty == 0 || pSmsmtBill->chargedParty == 1)
        {
            pDestBill->u.mtSMSRecord.bit_mask |= CMCC_CS_MTSMSRecord_chargedParty_present;
            pDestBill->u.mtSMSRecord.chargedParty = (CMCC_CS_ChargedParty)pSmsmtBill->chargedParty;
        }
    }
    
	return TRUE;
}

BOOL ConvertABill_MO_LR(const BYTE* pSrcBill, const int nLen,
                        const BYTE* pEventBill, const int nEventNum,                         
                        CMCC_CS_CallEventRecord* pDestBill)
{
	LCS_Bill_Report* pMolrBill = (LCS_Bill_Report *)pSrcBill;	
	
	pDestBill->choice = CMCC_CS_moLCSRecord_chosen;
    pDestBill->u.moLCSRecord.bit_mask = 0;

    if (GET_CDR_FIELD_FILTER(recordType, MOLCSRecord))
    {
        pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_recordType_present;
	    pDestBill->u.moLCSRecord.recordType = CMCC_CS_CallEventRecordType_moLCSRecord;
    }

    pDestBill->u.moLCSRecord.recordingEntity.length = 0;
    if (GET_CDR_FIELD_FILTER(recordingEntity, MOLCSRecord))
    {
        int nServedMSCLen = (pMolrBill->servedMSISDN_BCDlen + 1) / 2;
        if (nServedMSCLen > 0)
        {
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_recordingEntity_present;
            pDestBill->u.moLCSRecord.recordingEntity.length = 
                min(sizeof(pDestBill->u.moLCSRecord.recordingEntity.value), nServedMSCLen + 1);
            pDestBill->u.moLCSRecord.recordingEntity.value[0] = *(pMolrBill->servedMSISDN - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.moLCSRecord.recordingEntity.value + 1, pMolrBill->servedMSISDN, 
                pDestBill->u.moLCSRecord.recordingEntity.length - 1);
        }
    }

	if (GET_CDR_FIELD_FILTER(lcsClientType, MOLCSRecord))		//lcsClientType
	{
		if(pMolrBill->lcsClientType != 0xFF)
        {
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_lcsClientType_present;
		    pDestBill->u.moLCSRecord.lcsClientType = (CMCC_CS_LCSClientType)pMolrBill->lcsClientType;
        }
	}

    pDestBill->u.moLCSRecord.lcsClientIdentity.lcsClientExternalID.externalAddress.length = 0;
	if (GET_CDR_FIELD_FILTER(lcsClientIdentity, MOLCSRecord))	//lcsClientIdentity
	{
		pDestBill->u.moLCSRecord.lcsClientIdentity.bit_mask = 0;
        pDestBill->u.moLCSRecord.lcsClientIdentity.lcsClientExternalID.bit_mask = 0;

        int nLcsClientLen = (pMolrBill->lcsClientIdentity_BCDlen + 1) / 2;
        if (nLcsClientLen > 0)
        {
		    pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_lcsClientIdentity_present;
		    pDestBill->u.moLCSRecord.lcsClientIdentity.bit_mask |= CMCC_CS_lcsClientExternalID_present;
            pDestBill->u.moLCSRecord.lcsClientIdentity.lcsClientExternalID.bit_mask |= 
                CMCC_CS_externalAddress_present;
            
		    pDestBill->u.moLCSRecord.lcsClientIdentity.lcsClientExternalID.externalAddress.length = 
                min(sizeof(pDestBill->u.moLCSRecord.lcsClientIdentity.lcsClientExternalID.externalAddress.value),
                    nLcsClientLen + 1);
            pDestBill->u.moLCSRecord.lcsClientIdentity.lcsClientExternalID.externalAddress.value[0] = 
                *(pMolrBill->lcsClientIdentity - 2) | 0x80;
            BCDtoRBCD(&(pDestBill->u.moLCSRecord.lcsClientIdentity.lcsClientExternalID.externalAddress.value[1]), 
                    pMolrBill->lcsClientIdentity, 
                    pDestBill->u.moLCSRecord.lcsClientIdentity.lcsClientExternalID.externalAddress.length - 1);
        }
	}

    pDestBill->u.moLCSRecord.servedIMSI.length = 0;
    if (GET_CDR_FIELD_FILTER(servedIMSI, MOLCSRecord))
    {
        pDestBill->u.moLCSRecord.servedIMSI.length = 
            BCDtoRBCD(pDestBill->u.moLCSRecord.servedIMSI.value, pMolrBill->servedIMSI, 8);
        if(pDestBill->u.moLCSRecord.servedIMSI.length > 0)
        {
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_servedIMSI_present;
        }
    }

    pDestBill->u.moLCSRecord.servedMSISDN.length = 0;
	if (GET_CDR_FIELD_FILTER(servedMSISDN, MOLCSRecord))		//MSISDN
	{
	    int nMSISDNLen = (pMolrBill->servedMSISDN_BCDlen + 1) / 2;
        if (nMSISDNLen > 0)
        {
		    pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_servedMSISDN_present;
            pDestBill->u.moLCSRecord.servedMSISDN.length = 
                min(sizeof(pDestBill->u.moLCSRecord.servedMSISDN.value), nMSISDNLen + 1);
            pDestBill->u.moLCSRecord.servedMSISDN.value[0] = *(pMolrBill->servedMSISDN - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.moLCSRecord.servedMSISDN.value + 1, pMolrBill->servedMSISDN, 
                pDestBill->u.moLCSRecord.servedMSISDN.length - 1);
        }
	}	

    if (GET_CDR_FIELD_FILTER(molr_Type, MOLCSRecord))
    {
        pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_molr_Type_present;
        pDestBill->u.moLCSRecord.molr_Type = pMolrBill->molr_Type;
    }

	if (GET_CDR_FIELD_FILTER(lcsQos, MOLCSRecord))		//lcsQos
    {
        pDestBill->u.moLCSRecord.lcsQos.bit_mask = 0;
        if(!(pMolrBill->horizontal_accuracy & 0x80))
        {
            pDestBill->u.moLCSRecord.lcsQos.bit_mask |= CMCC_CS_horizontal_accuracy_present;
            pDestBill->u.moLCSRecord.lcsQos.horizontal_accuracy.length = 1;
            pDestBill->u.moLCSRecord.lcsQos.horizontal_accuracy.value[0] = pMolrBill->horizontal_accuracy;
        }
        if(!(pMolrBill->vertical_accuracy & 0x80))
        {
            pDestBill->u.moLCSRecord.lcsQos.bit_mask |= CMCC_CS_vertical_accuracy_present;
            pDestBill->u.moLCSRecord.lcsQos.vertical_accuracy.length = 1;
            pDestBill->u.moLCSRecord.lcsQos.vertical_accuracy.value[0] = pMolrBill->vertical_accuracy;
        }
        if(pMolrBill->verticalCoordinateRequest)
        {
            pDestBill->u.moLCSRecord.lcsQos.bit_mask |= CMCC_CS_verticalCoordinateRequest_present;
        }
        if(pMolrBill->responseTime != 0xFF)
        {
            pDestBill->u.moLCSRecord.lcsQos.bit_mask |= CMCC_CS_responseTime_present;
            pDestBill->u.moLCSRecord.lcsQos.responseTime.responseTimeCategory = 
                (CMCC_CS_ResponseTimeCategory)pMolrBill->responseTime;
        }
        if(pDestBill->u.moLCSRecord.lcsQos.bit_mask != 0)
        {
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_lcsQos_present;
        }
	}

    pDestBill->u.moLCSRecord.lcsPriority.length = 0;
	if (GET_CDR_FIELD_FILTER(lcsPriority, MOLCSRecord))		//lcsPriority
	{
		if (pMolrBill->lcsPriority != 0xFF)
        {
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_lcsPriority_present;
		    pDestBill->u.moLCSRecord.lcsPriority.length = 1;
		    pDestBill->u.moLCSRecord.lcsPriority.value[0] = pMolrBill->lcsPriority;
        }
	}

    pDestBill->u.moLCSRecord.mlc_Number.length = 0;
	if (GET_CDR_FIELD_FILTER(mlc_Number, MOLCSRecord))		//mlc_Number
	{
	    int nGMLCAddressLen = (pMolrBill->GMLC_Number_BCDlen + 1) / 2;
        if (nGMLCAddressLen > 0)
        {
		    pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_mlc_Number_present;
            pDestBill->u.moLCSRecord.mlc_Number.length = 
                min(sizeof(pDestBill->u.moLCSRecord.mlc_Number.value), nGMLCAddressLen + 1);
            pDestBill->u.moLCSRecord.mlc_Number.value[0] = *(pMolrBill->GMLC_Number - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.moLCSRecord.mlc_Number.value + 1, pMolrBill->GMLC_Number, 
                pDestBill->u.moLCSRecord.mlc_Number.length - 1);
        }
	}

    if (GET_CDR_FIELD_FILTER(eventTimeStamp, MOLCSRecord))
    {
        pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_eventTimeStamp_present;
        ConvertTimeStamp(pDestBill->u.moLCSRecord.eventTimeStamp, &(pMolrBill->eventTimeStamp_year));
    }

	if (GET_CDR_FIELD_FILTER(measureDuration, MOLCSRecord))		//measureDuration
	{
	    pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_measureDuration_present;
        pDestBill->u.moLCSRecord.measureDuration = pMolrBill->measureDuration;
	}

    pDestBill->u.moLCSRecord.location.locationAreaCode.length = 0;
    pDestBill->u.moLCSRecord.location.cellIdentifier.length = 0;
	if (GET_CDR_FIELD_FILTER(location, MOLCSRecord))					//位置区、小区
	{
        if (IsLACandCellIDValid(&(pMolrBill->served_location_LAI[3]), pMolrBill->served_location_cellID))
        {
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_location_present;
		    pDestBill->u.moLCSRecord.location.locationAreaCode.length = 2;
		    memcpy(pDestBill->u.moLCSRecord.location.locationAreaCode.value, pMolrBill->served_location_LAI + 3, 2);
		    pDestBill->u.moLCSRecord.location.cellIdentifier.length = 2;
		    memcpy(pDestBill->u.moLCSRecord.location.cellIdentifier.value, pMolrBill->served_location_cellID, 2);
        }
	}

    pDestBill->u.moLCSRecord.locationEstimate.length = 0;
	if (GET_CDR_FIELD_FILTER(locationEstimate, MOLCSRecord))
	{
        if(pMolrBill->positioningDataLen != 0)
        {
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_locationEstimate_present;
            pDestBill->u.moLCSRecord.locationEstimate.length = 
                min(sizeof(pDestBill->u.moLCSRecord.locationEstimate.value), pMolrBill->positioningDataLen);
            memcpy(pDestBill->u.moLCSRecord.locationEstimate.value,  pMolrBill->VarPart, pDestBill->u.moLCSRecord.locationEstimate.length);
        }
	}

    pDestBill->u.moLCSRecord.positioningData.length = 0;
	if (GET_CDR_FIELD_FILTER(positioningData, MOLCSRecord))
	{
        if(pMolrBill->location_method != 0xFF)
        {
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_positioningData_present;
            pDestBill->u.moLCSRecord.positioningData.length = 1;
            pDestBill->u.moLCSRecord.positioningData.value[0] = pMolrBill->location_method;
        }
	}
    
    pDestBill->u.moLCSRecord.lcsCause.length = 0;
	if (GET_CDR_FIELD_FILTER(lcsCause, MOLCSRecord))					//lcsCause
	{
        if (pMolrBill->lcsCause != 0xFF)
        {
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_lcsCause_present;
		    pDestBill->u.moLCSRecord.lcsCause.length = 1;
            pDestBill->u.moLCSRecord.lcsCause.value[0] = pMolrBill->lcsCause;
        }
	}
	
	if (GET_CDR_FIELD_FILTER(diagnostics, MOLCSRecord))						//diagnostics
	{
		pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_diagnostics_present;
		if (pMolrBill->diagnostics.CauseType == 0 && pMolrBill->diagnostics.Cause.gsm0408Cause != 0)
		{
			pDestBill->u.moLCSRecord.diagnostics.choice = CMCC_CS_gsm0408Cause_chosen;
			pDestBill->u.moLCSRecord.diagnostics.u.gsm0408Cause = pMolrBill->diagnostics.Cause.gsm0408Cause;
		}
		else if (pMolrBill->diagnostics.CauseType == 1 && pMolrBill->diagnostics.Cause.gsm0902MapErrorValue != 0)
		{
			pDestBill->u.moLCSRecord.diagnostics.choice = CMCC_CS_gsm0902MapErrorValue_chosen;
			pDestBill->u.moLCSRecord.diagnostics.u.gsm0902MapErrorValue = pMolrBill->diagnostics.Cause.gsm0902MapErrorValue;
		}
		else if (pMolrBill->diagnostics.CauseType == 2 && pMolrBill->diagnostics.Cause.ccittQ767Cause != 0)
		{
			pDestBill->u.moLCSRecord.diagnostics.choice = CMCC_CS_ccittQ767Cause_chosen;
			pDestBill->u.moLCSRecord.diagnostics.u.ccittQ767Cause = pMolrBill->diagnostics.Cause.ccittQ767Cause;
		}
		else
		{
			pDestBill->u.moLCSRecord.bit_mask &= ~CMCC_CS_MOLCSRecord_diagnostics_present;
		}
	}

	if (GET_CDR_FIELD_FILTER(systemType, MOLCSRecord))				//systemType
	{	
        switch((ACCESS_NETWORK_TYPE)pMolrBill->systemType)
        {
        case ACCESS_BSC :
            pDestBill->u.moLCSRecord.systemType = CMCC_CS_gERAN;
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_systemType_present;
            break;
        case ACCESS_RNC :
            pDestBill->u.moLCSRecord.systemType = CMCC_CS_iuUTRAN;
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_systemType_present;
            break;
        case ACCESS_UNKNOWN:
            pDestBill->u.moLCSRecord.systemType = CMCC_CS_unknown;
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_systemType_present;
            break;
        }
	}

    pDestBill->u.moLCSRecord.recordExtensions = NULL;					//recordExtensions(目前不支持)

    if (GET_CDR_FIELD_FILTER(causeForTerm, MOLCSRecord))
    {
        pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_causeForTerm_present;
        pDestBill->u.moLCSRecord.causeForTerm = pMolrBill->causeForTerm;
    }

    pDestBill->u.moLCSRecord.subscriberCategory.length = 0;
    if (GET_CDR_FIELD_FILTER(subscriberCategory, MOLCSRecord))
	{
        pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_subscriberCategory_present;
        pDestBill->u.moLCSRecord.subscriberCategory.length = 1;
        pDestBill->u.moLCSRecord.subscriberCategory.value[0] = pMolrBill->subscriberCategory;
    }

    pDestBill->u.moLCSRecord.firstmccmnc.length = 0;
    if (GET_CDR_FIELD_FILTER(firstmccmnc, MOLCSRecord))
	{
        if (!(pMolrBill->served_location_LAI[0] == 0xFF
            && pMolrBill->served_location_LAI[1] == 0xFF
            && pMolrBill->served_location_LAI[2] == 0xFF)
            && !(pMolrBill->served_location_LAI[0] == 0
            && pMolrBill->served_location_LAI[1] == 0
            && pMolrBill->served_location_LAI[2] == 0))
        {
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_firstmccmnc_present;
            pDestBill->u.moLCSRecord.firstmccmnc.length = 3;
            memcpy(pDestBill->u.moLCSRecord.firstmccmnc.value, pMolrBill->served_location_LAI, 3);
        }
    }

    pDestBill->u.moLCSRecord.orgRNCorBSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgRNCorBSCId, MOLCSRecord))
	{
        if (pMolrBill->rnc_bsc_id_len != 0)
        {
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_orgRNCorBSCId_present;
            pDestBill->u.moLCSRecord.orgRNCorBSCId.length = min(3,pMolrBill->rnc_bsc_id_len);
            memcpy(pDestBill->u.moLCSRecord.orgRNCorBSCId.value, pMolrBill->orgRNCorBSCId, pDestBill->u.moLCSRecord.orgRNCorBSCId.length);
        }
    }

    pDestBill->u.moLCSRecord.orgMSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgMSCId, MOLCSRecord))
	{
        if (pMolrBill->msc_id_len != 0)
        {
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_orgMSCId_present;
            pDestBill->u.moLCSRecord.orgMSCId.length = min(3,pMolrBill->msc_id_len);
            memcpy(pDestBill->u.moLCSRecord.orgMSCId.value, pMolrBill->orgMSCId, pDestBill->u.moLCSRecord.orgMSCId.length);
        }
    }

	if (GET_CDR_FIELD_FILTER(hotBillingTag, MOLCSRecord))
	{
		if (pMolrBill->HotBillingTag == HOT_BILLING)
		{
		    pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_hotBillingTag_present;		
			pDestBill->u.moLCSRecord.hotBillingTag = CMCC_CS_hotBilling;
		}
		else
		{
			pDestBill->u.moLCSRecord.hotBillingTag = CMCC_CS_noHotBilling;
		}
	}

    pDestBill->u.moLCSRecord.globalAreaID.length = 0;
    if (GET_CDR_FIELD_FILTER(globalAreaID, MOLCSRecord))     //1
    {
        if (IsLACandCellIDValid(&(pMolrBill->served_location_LAI[3]),pMolrBill->served_location_cellID))
        {           
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_globalAreaID_present;
            pDestBill->u.moLCSRecord.globalAreaID.length = 7;
            memcpy(pDestBill->u.moLCSRecord.globalAreaID.value,pMolrBill->served_location_LAI,5);
            memcpy(pDestBill->u.moLCSRecord.globalAreaID.value + 5,pMolrBill->served_location_cellID,2);
        }
    }

    if (GET_CDR_FIELD_FILTER(additionalChgInfo, MOLCSRecord))		//附加计费信息
    {
        if (pMolrBill->chargeIndicator >= 0 && pMolrBill->chargeIndicator < 3)
        {
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_additionalChgInfo_present;
            pDestBill->u.moLCSRecord.additionalChgInfo.bit_mask = CMCC_CS_chargeIndicator_present;
            pDestBill->u.moLCSRecord.additionalChgInfo.chargeIndicator = pMolrBill->chargeIndicator;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(chargedParty,MOLCSRecord) )
    {
        if (pMolrBill->chargedParty == 0 || pMolrBill->chargedParty == 1)
        {
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_chargedParty_present;
            pDestBill->u.moLCSRecord.chargedParty = (CMCC_CS_ChargedParty)pMolrBill->chargedParty;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(lcsCallReference,MOLCSRecord)) 
    {
        if (pMolrBill->call_reference_len != 0)
        {
            int nCallReferenceLen = min(pMolrBill->call_reference_len,
                sizeof(pDestBill->u.moLCSRecord.lcsReferenceNumber.value));
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_lcsReferenceNumber_present;
            pDestBill->u.moLCSRecord.lcsReferenceNumber.length = nCallReferenceLen;
            memcpy(pDestBill->u.moLCSRecord.lcsReferenceNumber.value, 
                &(pMolrBill->call_reference[sizeof(pMolrBill->call_reference) - pMolrBill->call_reference_len]), nCallReferenceLen);   
        }
        else
        {
            CMCC_CS_CallReference ncallreference;
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_lcsReferenceNumber_present;
            GetCallReference(ncallreference, pMolrBill->ModuleNum, pMolrBill->count);
            pDestBill->u.moLCSRecord.lcsReferenceNumber.length = ncallreference.length;
            memcpy(pDestBill->u.moLCSRecord.lcsReferenceNumber.value, ncallreference.value, ncallreference.length);
        }
    }
    
    if (GET_CDR_FIELD_FILTER(servedIMEI, MOLCSRecord))		//ServedIMEI
    {
        pDestBill->u.moLCSRecord.servedIMEI.length = 
            BCDtoRBCD(pDestBill->u.moLCSRecord.servedIMEI.value, pMolrBill->servedIMEI, 8);
        if (pDestBill->u.moLCSRecord.servedIMEI.length > 0)
        {
            pDestBill->u.moLCSRecord.bit_mask |= CMCC_CS_MOLCSRecord_servedIMEI_present;        
        }
    }
	return TRUE;
}

BOOL ConvertABill_MT_LR(const BYTE* pSrcBill, const int nLen,
                        const BYTE* pEventBill, const int nEventNum,                         
                        CMCC_CS_CallEventRecord* pDestBill)
{
	LCS_Bill_Report* pMtlrBill = (LCS_Bill_Report *)pSrcBill;	
	
	pDestBill->choice = CMCC_CS_mtLCSRecord_chosen;
    pDestBill->u.mtLCSRecord.bit_mask = 0;

    if (GET_CDR_FIELD_FILTER(recordType, MTLCSRecord))
    {
        pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_recordType_present;
        pDestBill->u.mtLCSRecord.recordType = CMCC_CS_CallEventRecordType_mtLCSRecord;
    }

    pDestBill->u.mtLCSRecord.recordingEntity.length = 0;
    if (GET_CDR_FIELD_FILTER(recordingEntity, MTLCSRecord))
    {
        int nServedMSCLen = (pMtlrBill->recordingEntity_BCDlen + 1) / 2;
        if (nServedMSCLen > 0)
        {
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_recordingEntity_present;
            pDestBill->u.mtLCSRecord.recordingEntity.length = 
                min(sizeof(pDestBill->u.mtLCSRecord.recordingEntity.value), nServedMSCLen + 1);
            pDestBill->u.mtLCSRecord.recordingEntity.value[0] = *(pMtlrBill->recordingEntity - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.mtLCSRecord.recordingEntity.value + 1, pMtlrBill->recordingEntity, 
                pDestBill->u.mtLCSRecord.recordingEntity.length - 1);
        }
    }

    if (GET_CDR_FIELD_FILTER(lcsClientType, MTLCSRecord))
    {
        if (pMtlrBill->lcsClientType != 0xFF)
        {
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_lcsClientType_present;
            pDestBill->u.mtLCSRecord.lcsClientType = (CMCC_CS_LCSClientType)pMtlrBill->lcsClientType;
        }
    }
    
    pDestBill->u.mtLCSRecord.lcsClientIdentity.bit_mask = 0;
    pDestBill->u.mtLCSRecord.lcsClientIdentity.lcsClientExternalID.bit_mask = 0;
    if (GET_CDR_FIELD_FILTER(lcsClientIdentity, MTLCSRecord))
    {
        int nLcsClientLen = (pMtlrBill->lcsClientIdentity_BCDlen + 1) / 2;
        if (nLcsClientLen > 0)
        {
	        pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_lcsClientIdentity_present;
            pDestBill->u.mtLCSRecord.lcsClientIdentity.bit_mask |= CMCC_CS_lcsClientExternalID_present;
            pDestBill->u.mtLCSRecord.lcsClientIdentity.lcsClientExternalID.bit_mask |= 
                CMCC_CS_externalAddress_present;
        
		    pDestBill->u.mtLCSRecord.lcsClientIdentity.lcsClientExternalID.externalAddress.length = 
                min(sizeof(pDestBill->u.mtLCSRecord.lcsClientIdentity.lcsClientExternalID.externalAddress.value),
                    nLcsClientLen + 1);
            pDestBill->u.mtLCSRecord.lcsClientIdentity.lcsClientExternalID.externalAddress.value[0] = 
                *(pMtlrBill->lcsClientIdentity - 2) | 0x80;
            BCDtoRBCD(&(pDestBill->u.mtLCSRecord.lcsClientIdentity.lcsClientExternalID.externalAddress.value[1]), 
                    pMtlrBill->lcsClientIdentity, 
                    pDestBill->u.mtLCSRecord.lcsClientIdentity.lcsClientExternalID.externalAddress.length - 1);
        }
    }

    pDestBill->u.mtLCSRecord.servedIMSI.length = 0;
    if (GET_CDR_FIELD_FILTER(servedIMSI, MTLCSRecord))
    {
        pDestBill->u.mtLCSRecord.servedIMSI.length = 
            BCDtoRBCD(pDestBill->u.mtLCSRecord.servedIMSI.value, pMtlrBill->servedIMSI, 8);
        if(pDestBill->u.mtLCSRecord.servedIMSI.length > 0)
        {
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_servedIMSI_present;        
        }
    }

	pDestBill->u.mtLCSRecord.servedMSISDN.length = 0;
    if (GET_CDR_FIELD_FILTER(servedMSISDN, MTLCSRecord))		//MSISDN
	{
	    int nMSISDNLen = (pMtlrBill->servedMSISDN_BCDlen + 1) / 2;
        if (nMSISDNLen > 0)
        {
		    pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_servedMSISDN_present;
            pDestBill->u.mtLCSRecord.servedMSISDN.length = 
                min(sizeof(pDestBill->u.mtLCSRecord.servedMSISDN.value), nMSISDNLen + 1);
            pDestBill->u.mtLCSRecord.servedMSISDN.value[0] = *(pMtlrBill->servedMSISDN - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.mtLCSRecord.servedMSISDN.value + 1, pMtlrBill->servedMSISDN, 
                pDestBill->u.mtLCSRecord.servedMSISDN.length - 1);
        }
	}	

    if (GET_CDR_FIELD_FILTER(locationType, MTLCSRecord))
    {
        if (pMtlrBill->locationType != 0xFF)
        {
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_locationType_present;
            pDestBill->u.mtLCSRecord.locationType.bit_mask = 0;
	        pDestBill->u.mtLCSRecord.locationType.locationEstimateType = (CMCC_CS_LocationEstimateType)pMtlrBill->locationType;
        }
    }
    
	if (GET_CDR_FIELD_FILTER(lcsQos, MTLCSRecord))		//lcsQos
	{
        pDestBill->u.mtLCSRecord.lcsQos.bit_mask = 0;
        if(!(pMtlrBill->horizontal_accuracy & 0x80))
        {
            pDestBill->u.mtLCSRecord.lcsQos.bit_mask |= CMCC_CS_horizontal_accuracy_present;
            pDestBill->u.mtLCSRecord.lcsQos.horizontal_accuracy.length = 1;
            pDestBill->u.mtLCSRecord.lcsQos.horizontal_accuracy.value[0] = pMtlrBill->horizontal_accuracy;
        }
        if(!(pMtlrBill->vertical_accuracy & 0x80))
        {
            pDestBill->u.mtLCSRecord.lcsQos.bit_mask |= CMCC_CS_vertical_accuracy_present;
            pDestBill->u.mtLCSRecord.lcsQos.vertical_accuracy.length = 1;
            pDestBill->u.mtLCSRecord.lcsQos.vertical_accuracy.value[0] = pMtlrBill->vertical_accuracy;
        }
        if(pMtlrBill->verticalCoordinateRequest)
        {
            pDestBill->u.mtLCSRecord.lcsQos.bit_mask |= CMCC_CS_verticalCoordinateRequest_present;
        }
        if(pMtlrBill->responseTime != 0xFF)
        {
            pDestBill->u.mtLCSRecord.lcsQos.bit_mask |= CMCC_CS_responseTime_present;
            pDestBill->u.mtLCSRecord.lcsQos.responseTime.responseTimeCategory = 
                (CMCC_CS_ResponseTimeCategory)pMtlrBill->responseTime;
        }
        if(pDestBill->u.mtLCSRecord.lcsQos.bit_mask != 0)
        {
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_lcsQos_present;
        }
	}

    pDestBill->u.mtLCSRecord.lcsPriority.length = 0;    
	if (GET_CDR_FIELD_FILTER(lcsPriority, MTLCSRecord))		//lcsPriority
	{
		if (pMtlrBill->lcsPriority != 0xFF)
        {
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_lcsPriority_present;
		    pDestBill->u.mtLCSRecord.lcsPriority.length = 1;
		    pDestBill->u.mtLCSRecord.lcsPriority.value[0] = pMtlrBill->lcsPriority;
        }
	}

    pDestBill->u.mtLCSRecord.mlc_Number.length = 0;
    if (GET_CDR_FIELD_FILTER(mlc_Number, MTLCSRecord))
    {
        int nGMLCAddressLen = (pMtlrBill->GMLC_Number_BCDlen + 1) / 2;
        if (nGMLCAddressLen > 0)
        {
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_mlc_Number_present;
            pDestBill->u.mtLCSRecord.mlc_Number.length = 
                min(sizeof(pDestBill->u.mtLCSRecord.mlc_Number.value), nGMLCAddressLen + 1);
            pDestBill->u.mtLCSRecord.mlc_Number.value[0] = *(pMtlrBill->GMLC_Number - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.mtLCSRecord.mlc_Number.value + 1, pMtlrBill->GMLC_Number, 
                pDestBill->u.mtLCSRecord.mlc_Number.length - 1);
        }
    }

    if (GET_CDR_FIELD_FILTER(eventTimeStamp, MTLCSRecord))
    {
        pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_eventTimeStamp_present;
        ConvertTimeStamp(pDestBill->u.mtLCSRecord.eventTimeStamp, &(pMtlrBill->eventTimeStamp_year));
    }

	if (GET_CDR_FIELD_FILTER(measureDuration, MTLCSRecord))		//measureDuration
	{
	    pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_measureDuration_present;
        pDestBill->u.mtLCSRecord.measureDuration = pMtlrBill->measureDuration;
	}

	if (GET_CDR_FIELD_FILTER(notificationToMSUser, MTLCSRecord))					//notificationToMSUser
	{
        if (pMtlrBill->notificationToMSUser != 0xFF)
        {
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_notificationToMSUser_present;
		    pDestBill->u.mtLCSRecord.notificationToMSUser = (CMCC_CS_NotificationToMSUser)pMtlrBill->notificationToMSUser;
        }
	}

	if (GET_CDR_FIELD_FILTER(privacyOverride, MTLCSRecord))					//privacyOverride
	{
		if (pMtlrBill->privacyOverride != 0)
        {
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_privacyOverride_present;
		    pDestBill->u.mtLCSRecord.privacyOverride = pMtlrBill->privacyOverride;
        }
	}

    pDestBill->u.mtLCSRecord.location.locationAreaCode.length = 0;
    pDestBill->u.mtLCSRecord.location.cellIdentifier.length = 0;
	if (GET_CDR_FIELD_FILTER(location, MTLCSRecord))					//位置区、小区
	{
        if (IsLACandCellIDValid(&(pMtlrBill->served_location_LAI[3]), pMtlrBill->served_location_cellID))
        {
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_location_present;
		    pDestBill->u.mtLCSRecord.location.locationAreaCode.length = 2;
		    memcpy(pDestBill->u.mtLCSRecord.location.locationAreaCode.value, pMtlrBill->served_location_LAI + 3, 2);
		    pDestBill->u.mtLCSRecord.location.cellIdentifier.length = 2;
		    memcpy(pDestBill->u.mtLCSRecord.location.cellIdentifier.value, pMtlrBill->served_location_cellID, 2);
        }
	}

    pDestBill->u.mtLCSRecord.locationEstimate.length = 0;
	if (GET_CDR_FIELD_FILTER(locationEstimate, MTLCSRecord))
	{
        if(pMtlrBill->positioningDataLen != 0)
        {
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_locationEstimate_present;
            pDestBill->u.mtLCSRecord.locationEstimate.length = 
                min(sizeof(pDestBill->u.mtLCSRecord.locationEstimate.value), pMtlrBill->positioningDataLen);
            memcpy(pDestBill->u.mtLCSRecord.locationEstimate.value,  pMtlrBill->VarPart, pDestBill->u.mtLCSRecord.locationEstimate.length);
        }
	}

    pDestBill->u.mtLCSRecord.positioningData.length = 0;
	if (GET_CDR_FIELD_FILTER(positioningData, MTLCSRecord))
	{
        if(pMtlrBill->location_method != 0xFF)
        {
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_positioningData_present;
            pDestBill->u.mtLCSRecord.positioningData.length = 1;
            pDestBill->u.mtLCSRecord.positioningData.value[0] = pMtlrBill->location_method;
        }
	}
	
    pDestBill->u.mtLCSRecord.lcsCause.length = 0;
	if (GET_CDR_FIELD_FILTER(lcsCause, MTLCSRecord))					//lcsCause
	{
        if (pMtlrBill->lcsCause != 0xFF)
        {
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_lcsCause_present;
		    pDestBill->u.mtLCSRecord.lcsCause.length = 1;
            pDestBill->u.mtLCSRecord.lcsCause.value[0] = pMtlrBill->lcsCause;
        }
	}
	
	if (GET_CDR_FIELD_FILTER(diagnostics, MTLCSRecord))					//diagnostics
	{
		pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_diagnostics_present;
		if (pMtlrBill->diagnostics.CauseType == 0 && pMtlrBill->diagnostics.Cause.gsm0408Cause != 0)
		{
			pDestBill->u.mtLCSRecord.diagnostics.choice = CMCC_CS_gsm0408Cause_chosen;
			pDestBill->u.mtLCSRecord.diagnostics.u.gsm0408Cause = pMtlrBill->diagnostics.Cause.gsm0408Cause;
		}
		else if (pMtlrBill->diagnostics.CauseType == 1 && pMtlrBill->diagnostics.Cause.gsm0902MapErrorValue != 0)
		{
			pDestBill->u.mtLCSRecord.diagnostics.choice = CMCC_CS_gsm0902MapErrorValue_chosen;
			pDestBill->u.mtLCSRecord.diagnostics.u.gsm0902MapErrorValue = pMtlrBill->diagnostics.Cause.gsm0902MapErrorValue;
		}
		else if (pMtlrBill->diagnostics.CauseType == 2 && pMtlrBill->diagnostics.Cause.ccittQ767Cause != 0)
		{
			pDestBill->u.mtLCSRecord.diagnostics.choice = CMCC_CS_ccittQ767Cause_chosen;
			pDestBill->u.mtLCSRecord.diagnostics.u.ccittQ767Cause = pMtlrBill->diagnostics.Cause.ccittQ767Cause;
		}
		else
		{
			pDestBill->u.mtLCSRecord.bit_mask &= ~CMCC_CS_MTLCSRecord_diagnostics_present;
		}
	}

	if (GET_CDR_FIELD_FILTER(systemType, MTLCSRecord))				//systemType
	{
        switch((ACCESS_NETWORK_TYPE)pMtlrBill->systemType)
        {
        case ACCESS_BSC :
            pDestBill->u.mtLCSRecord.systemType = CMCC_CS_gERAN;
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_systemType_present;
            break;
        case ACCESS_RNC :
            pDestBill->u.mtLCSRecord.systemType = CMCC_CS_iuUTRAN;
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_systemType_present;
            break;
        case ACCESS_UNKNOWN:
            pDestBill->u.mtLCSRecord.systemType = CMCC_CS_unknown;
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_systemType_present;
            break;
        }        
	}

    pDestBill->u.mtLCSRecord.recordExtensions = NULL;					//recordExtensions(目前不支持)

    if (GET_CDR_FIELD_FILTER(causeForTerm, MTLCSRecord))
    {
        pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_causeForTerm_present;
        pDestBill->u.mtLCSRecord.causeForTerm = pMtlrBill->causeForTerm;
    }

    pDestBill->u.mtLCSRecord.subscriberCategory.length = 0;
    if (GET_CDR_FIELD_FILTER(subscriberCategory, MTLCSRecord))
	{
        pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_subscriberCategory_present;
        pDestBill->u.mtLCSRecord.subscriberCategory.length = 1;
        pDestBill->u.mtLCSRecord.subscriberCategory.value[0] = pMtlrBill->subscriberCategory;
    }

    pDestBill->u.mtLCSRecord.firstmccmnc.length = 0;
    if (GET_CDR_FIELD_FILTER(firstmccmnc, MTLCSRecord))
	{
        if (!(pMtlrBill->served_location_LAI[0] == 0xFF
            && pMtlrBill->served_location_LAI[1] == 0xFF
            && pMtlrBill->served_location_LAI[2] == 0xFF)
            && !(pMtlrBill->served_location_LAI[0] == 0
            && pMtlrBill->served_location_LAI[1] == 0
            && pMtlrBill->served_location_LAI[2] == 0))
        {
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_firstmccmnc_present;
            pDestBill->u.mtLCSRecord.firstmccmnc.length = 3;
            memcpy(pDestBill->u.mtLCSRecord.firstmccmnc.value, pMtlrBill->served_location_LAI, 3);
        }
    }

    pDestBill->u.mtLCSRecord.orgRNCorBSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgRNCorBSCId, MTLCSRecord))
	{
        if (pMtlrBill->rnc_bsc_id_len != 0)
        {
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_orgRNCorBSCId_present;
            pDestBill->u.mtLCSRecord.orgRNCorBSCId.length = min(3,pMtlrBill->rnc_bsc_id_len);
            memcpy(pDestBill->u.mtLCSRecord.orgRNCorBSCId.value, pMtlrBill->orgRNCorBSCId, pDestBill->u.mtLCSRecord.orgRNCorBSCId.length);
        }
    }

    pDestBill->u.mtLCSRecord.orgMSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgMSCId, MTLCSRecord))
	{
        if (pMtlrBill->msc_id_len != 0)
        {
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_orgMSCId_present;
            pDestBill->u.mtLCSRecord.orgMSCId.length = min(3,pMtlrBill->msc_id_len);
            memcpy(pDestBill->u.mtLCSRecord.orgMSCId.value, pMtlrBill->orgMSCId, pDestBill->u.mtLCSRecord.orgMSCId.length);
        }
    }

	if (GET_CDR_FIELD_FILTER(hotBillingTag, MTLCSRecord))
	{
		if (pMtlrBill->HotBillingTag == HOT_BILLING)
		{
		    pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_hotBillingTag_present;		
            pDestBill->u.mtLCSRecord.hotBillingTag = CMCC_CS_hotBilling;
		}
		else
		{
			pDestBill->u.mtLCSRecord.hotBillingTag = CMCC_CS_noHotBilling;
		}
	}

    pDestBill->u.mtLCSRecord.globalAreaID.length = 0;
    if (GET_CDR_FIELD_FILTER(globalAreaID, MTLCSRecord))     //1
    {
        if (IsLACandCellIDValid(&(pMtlrBill->served_location_LAI[3]),pMtlrBill->served_location_cellID))
        {                       
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_globalAreaID_present;
            pDestBill->u.mtLCSRecord.globalAreaID.length = 7;
            memcpy(pDestBill->u.mtLCSRecord.globalAreaID.value,pMtlrBill->served_location_LAI,5);
            memcpy(pDestBill->u.mtLCSRecord.globalAreaID.value + 5,pMtlrBill->served_location_cellID,2);
        }
    }

    if (GET_CDR_FIELD_FILTER(additionalChgInfo, MTLCSRecord))		//附加计费信息
    {
        if (pMtlrBill->chargeIndicator >= 0 && pMtlrBill->chargeIndicator < 3)
        {
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_additionalChgInfo_present;
            pDestBill->u.mtLCSRecord.additionalChgInfo.bit_mask = CMCC_CS_chargeIndicator_present;
            pDestBill->u.mtLCSRecord.additionalChgInfo.chargeIndicator = pMtlrBill->chargeIndicator;
        }
    }
    
    
    if (GET_CDR_FIELD_FILTER(chargedParty,MTLCSRecord) )
    {
        if (pMtlrBill->chargedParty == 0 || pMtlrBill->chargedParty == 1)
        {
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_chargedParty_present;
            pDestBill->u.mtLCSRecord.chargedParty = (CMCC_CS_ChargedParty)pMtlrBill->chargedParty;
        }
    }

    if (GET_CDR_FIELD_FILTER(lcsCallReference,MTLCSRecord)) 
    {  
        if (pMtlrBill->call_reference_len != 0)
        {
            int nCallReferenceLen = min(pMtlrBill->call_reference_len,
                sizeof(pDestBill->u.mtLCSRecord.lcsReferenceNumber.value));
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_lcsReferenceNumber_present;
            pDestBill->u.mtLCSRecord.lcsReferenceNumber.length = nCallReferenceLen;
            memcpy(pDestBill->u.mtLCSRecord.lcsReferenceNumber.value, 
                &(pMtlrBill->call_reference[sizeof(pMtlrBill->call_reference) - pMtlrBill->call_reference_len]), nCallReferenceLen);   
        }
        else
        {
            CMCC_CS_CallReference ncallreference;
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_lcsReferenceNumber_present;
            GetCallReference(ncallreference, pMtlrBill->ModuleNum, pMtlrBill->count);
            pDestBill->u.mtLCSRecord.lcsReferenceNumber.length = ncallreference.length;
            memcpy(pDestBill->u.mtLCSRecord.lcsReferenceNumber.value, ncallreference.value, ncallreference.length);       
         }
    }
    
    if (GET_CDR_FIELD_FILTER(servedIMEI, MTLCSRecord))		//ServedIMEI
    {
        pDestBill->u.mtLCSRecord.servedIMEI.length = 
            BCDtoRBCD(pDestBill->u.mtLCSRecord.servedIMEI.value, pMtlrBill->servedIMEI, 8);
        if (pDestBill->u.mtLCSRecord.servedIMEI.length > 0)
        {
            pDestBill->u.mtLCSRecord.bit_mask |= CMCC_CS_MTLCSRecord_servedIMEI_present;        
        }
    }

	return TRUE;
}

BOOL ConvertABill_NI_LR(const BYTE* pSrcBill, const int nLen,
                        const BYTE* pEventBill, const int nEventNum,                         
                        CMCC_CS_CallEventRecord* pDestBill)
{
	LCS_Bill_Report* pNilrBill = (LCS_Bill_Report *)pSrcBill;	
	
	pDestBill->choice = CMCC_CS_niLCSRecord_chosen;
    pDestBill->u.niLCSRecord.bit_mask = 0;

    if (GET_CDR_FIELD_FILTER(recordType, NILCSRecord))
    {
        pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_recordType_present;
	    pDestBill->u.niLCSRecord.recordType = CMCC_CS_CallEventRecordType_niLCSRecord;
    }

    pDestBill->u.niLCSRecord.recordingEntity.length = 0;
    if (GET_CDR_FIELD_FILTER(recordingEntity, NILCSRecord))
    {
        int nServedMSCLen = (pNilrBill->recordingEntity_BCDlen + 1) / 2;
        if (nServedMSCLen > 0)
        {
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_recordingEntity_present; 
            pDestBill->u.niLCSRecord.recordingEntity.length = 
                min(sizeof(pDestBill->u.niLCSRecord.recordingEntity.value), nServedMSCLen + 1);
            pDestBill->u.niLCSRecord.recordingEntity.value[0] = *(pNilrBill->recordingEntity - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.niLCSRecord.recordingEntity.value + 1, pNilrBill->recordingEntity, 
                pDestBill->u.niLCSRecord.recordingEntity.length - 1);
        }
    }

	if (GET_CDR_FIELD_FILTER(lcsClientType, NILCSRecord))		//lcsClientType
	{
		if(pNilrBill->lcsClientType != 0xFF)
        {
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_lcsClientType_present;
	    	pDestBill->u.niLCSRecord.lcsClientType = (CMCC_CS_LCSClientType)pNilrBill->lcsClientType;
        }
	}

	pDestBill->u.niLCSRecord.lcsClientIdentity.lcsClientExternalID.externalAddress.length = 0;
    if (GET_CDR_FIELD_FILTER(lcsClientIdentity, NILCSRecord))	//lcsClientIdentity
	{
		pDestBill->u.niLCSRecord.lcsClientIdentity.bit_mask = 0;
        pDestBill->u.niLCSRecord.lcsClientIdentity.lcsClientExternalID.bit_mask = 0;

        int nLcsClientLen = (pNilrBill->lcsClientIdentity_BCDlen + 1) / 2;
        if (nLcsClientLen > 0)
        {
		    pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_lcsClientIdentity_present;
		    pDestBill->u.niLCSRecord.lcsClientIdentity.bit_mask |= CMCC_CS_lcsClientExternalID_present;
            pDestBill->u.niLCSRecord.lcsClientIdentity.lcsClientExternalID.bit_mask |= 
                CMCC_CS_externalAddress_present;
        
		    pDestBill->u.niLCSRecord.lcsClientIdentity.lcsClientExternalID.externalAddress.length = 
                min(sizeof(pDestBill->u.niLCSRecord.lcsClientIdentity.lcsClientExternalID.externalAddress.value),
                    nLcsClientLen + 1);
            pDestBill->u.niLCSRecord.lcsClientIdentity.lcsClientExternalID.externalAddress.value[0] = 
                *(pNilrBill->lcsClientIdentity - 2) | 0x80;
            BCDtoRBCD(&(pDestBill->u.niLCSRecord.lcsClientIdentity.lcsClientExternalID.externalAddress.value[1]), 
                    pNilrBill->lcsClientIdentity, 
                    pDestBill->u.niLCSRecord.lcsClientIdentity.lcsClientExternalID.externalAddress.length - 1);
        }
	}

	pDestBill->u.niLCSRecord.servedIMSI.length = 0;
    if (GET_CDR_FIELD_FILTER(servedIMSI, NILCSRecord))		//ServedIMSI
	{
		pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_servedIMSI_present;
		pDestBill->u.niLCSRecord.servedIMSI.length = 
            BCDtoRBCD(pDestBill->u.niLCSRecord.servedIMSI.value, pNilrBill->servedIMSI, 8);
	}

	pDestBill->u.niLCSRecord.servedMSISDN.length = 0;
    if (GET_CDR_FIELD_FILTER(servedMSISDN, NILCSRecord))		//MSISDN
	{
	    int nMSISDNLen = (pNilrBill->servedMSISDN_BCDlen + 1) / 2;
        if (nMSISDNLen > 0)
        {
		    pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_servedMSISDN_present;
            pDestBill->u.niLCSRecord.servedMSISDN.length = 
                min(sizeof(pDestBill->u.niLCSRecord.servedMSISDN.value), nMSISDNLen + 1);
            pDestBill->u.niLCSRecord.servedMSISDN.value[0] = *(pNilrBill->servedMSISDN - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.niLCSRecord.servedMSISDN.value + 1, pNilrBill->servedMSISDN, 
                pDestBill->u.niLCSRecord.servedMSISDN.length - 1);
        }
	}	

    pDestBill->u.niLCSRecord.emsDigits.length = 0;
	if (GET_CDR_FIELD_FILTER(emsDigits, NILCSRecord))		//emsDigits 无法转换
	{
        int nEmsDigitsLen = (pNilrBill->emsDigits_BCDlen + 1) / 2;
        if (nEmsDigitsLen > 0)
        {
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_emsDigits_present;
            pDestBill->u.niLCSRecord.emsDigits.length = 
                min(sizeof(pDestBill->u.niLCSRecord.emsDigits.value), nEmsDigitsLen + 1);
            pDestBill->u.niLCSRecord.emsDigits.value[0] = *(pNilrBill->emsDigits - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.niLCSRecord.emsDigits.value + 1, pNilrBill->emsDigits, 
                pDestBill->u.niLCSRecord.emsDigits.length - 1);
        }        
	}

    pDestBill->u.niLCSRecord.emsKey.length = 0;
	if (GET_CDR_FIELD_FILTER(emsKey, NILCSRecord))		//emsDigits	无法转换
	{
        int nEmsekeyLen = (pNilrBill->emsKey_BCDlen + 1) / 2;
        if (nEmsekeyLen > 0)
        {
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_emsKey_present;
            pDestBill->u.niLCSRecord.emsKey.length = 
                min(sizeof(pDestBill->u.niLCSRecord.emsKey.value), nEmsekeyLen + 1);
            pDestBill->u.niLCSRecord.emsKey.value[0] = *(pNilrBill->emsKey - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.niLCSRecord.emsKey.value + 1, pNilrBill->emsKey, 
                pDestBill->u.niLCSRecord.emsKey.length - 1);
        }  
	}

	if (GET_CDR_FIELD_FILTER(lcsQos, NILCSRecord))		//lcsQos
	{
        pDestBill->u.niLCSRecord.lcsQos.bit_mask = 0;
        if(!(pNilrBill->horizontal_accuracy & 0x80))
        {
            pDestBill->u.niLCSRecord.lcsQos.bit_mask |= CMCC_CS_horizontal_accuracy_present;
            pDestBill->u.niLCSRecord.lcsQos.horizontal_accuracy.length = 1;
            pDestBill->u.niLCSRecord.lcsQos.horizontal_accuracy.value[0] = pNilrBill->horizontal_accuracy;
        }
        if(!(pNilrBill->vertical_accuracy & 0x80))
        {
            pDestBill->u.niLCSRecord.lcsQos.bit_mask |= CMCC_CS_vertical_accuracy_present;
            pDestBill->u.niLCSRecord.lcsQos.vertical_accuracy.length = 1;
            pDestBill->u.niLCSRecord.lcsQos.vertical_accuracy.value[0] = pNilrBill->vertical_accuracy;
        }
        if(pNilrBill->verticalCoordinateRequest)
        {
            pDestBill->u.niLCSRecord.lcsQos.bit_mask |= CMCC_CS_verticalCoordinateRequest_present;
        }
        if(pNilrBill->responseTime != 0xFF)
        {
            pDestBill->u.niLCSRecord.lcsQos.bit_mask |= CMCC_CS_responseTime_present;
            pDestBill->u.niLCSRecord.lcsQos.responseTime.responseTimeCategory = 
                (CMCC_CS_ResponseTimeCategory)pNilrBill->responseTime;
        }
        if(pDestBill->u.niLCSRecord.lcsQos.bit_mask != 0)
        {
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_lcsQos_present;
        }
	}

    pDestBill->u.niLCSRecord.lcsPriority.length = 0;
	if (GET_CDR_FIELD_FILTER(lcsPriority, NILCSRecord))		//lcsPriority
	{
		if (pNilrBill->lcsPriority != 0xFF)
        {
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_lcsPriority_present;
		    pDestBill->u.niLCSRecord.lcsPriority.length = 1;
		    pDestBill->u.niLCSRecord.lcsPriority.value[0] = pNilrBill->lcsPriority;
        }
	}

    pDestBill->u.niLCSRecord.mlc_Number.length = 0 ;
	if (GET_CDR_FIELD_FILTER(mlc_Number, NILCSRecord))		//mlc_Number
	{
	    int nGMLCAddressLen = (pNilrBill->GMLC_Number_BCDlen + 1) / 2;
        if (nGMLCAddressLen > 0)
        {
		    pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_mlc_Number_present;
            pDestBill->u.niLCSRecord.mlc_Number.length = 
                min(sizeof(pDestBill->u.niLCSRecord.mlc_Number.value), nGMLCAddressLen + 1);
            pDestBill->u.niLCSRecord.mlc_Number.value[0] = *(pNilrBill->GMLC_Number - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.niLCSRecord.mlc_Number.value + 1, pNilrBill->GMLC_Number, 
                pDestBill->u.niLCSRecord.mlc_Number.length - 1);
        }
	}

    if (GET_CDR_FIELD_FILTER(eventTimeStamp, NILCSRecord))
    {
        pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_eventTimeStamp_present;
        ConvertTimeStamp(pDestBill->u.niLCSRecord.eventTimeStamp, &(pNilrBill->eventTimeStamp_year));
    }

	if (GET_CDR_FIELD_FILTER(measureDuration, NILCSRecord))		//measureDuration
	{
		pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_measureDuration_present;
        pDestBill->u.niLCSRecord.measureDuration = pNilrBill->measureDuration;
	}

    pDestBill->u.niLCSRecord.location.locationAreaCode.length = 0;
    pDestBill->u.niLCSRecord.location.cellIdentifier.length = 0;
	if (GET_CDR_FIELD_FILTER(location, NILCSRecord))					//位置区、小区
	{
        if (IsLACandCellIDValid(&(pNilrBill->served_location_LAI[3]), pNilrBill->served_location_cellID))
        {
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_location_present;
		    pDestBill->u.niLCSRecord.location.locationAreaCode.length = 2;
		    memcpy(pDestBill->u.niLCSRecord.location.locationAreaCode.value, pNilrBill->served_location_LAI + 3, 2);
		    pDestBill->u.niLCSRecord.location.cellIdentifier.length = 2;
		    memcpy(pDestBill->u.niLCSRecord.location.cellIdentifier.value, pNilrBill->served_location_cellID, 2);
        }
	}

    pDestBill->u.niLCSRecord.locationEstimate.length = 0;
	if (GET_CDR_FIELD_FILTER(locationEstimate, NILCSRecord))
	{
        if(pNilrBill->positioningDataLen != 0)
        {
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_locationEstimate_present;
            pDestBill->u.niLCSRecord.locationEstimate.length = 
                min(sizeof(pDestBill->u.niLCSRecord.locationEstimate.value), pNilrBill->positioningDataLen);
            memcpy(pDestBill->u.niLCSRecord.locationEstimate.value,  pNilrBill->VarPart, pDestBill->u.niLCSRecord.locationEstimate.length);
        }
	}

    pDestBill->u.niLCSRecord.positioningData.length = 0;
	if (GET_CDR_FIELD_FILTER(positioningData, NILCSRecord))
	{
        if(pNilrBill->location_method != 0xFF)
        {
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_positioningData_present;
            pDestBill->u.niLCSRecord.positioningData.length = 1;
            pDestBill->u.niLCSRecord.positioningData.value[0] = pNilrBill->location_method;
        }
	}

	pDestBill->u.niLCSRecord.lcsCause.length = 0;
	if (GET_CDR_FIELD_FILTER(lcsCause, NILCSRecord))					//lcsCause
	{
        if (pNilrBill->lcsCause != 0xFF)
        {
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_lcsCause_present;
		    pDestBill->u.niLCSRecord.lcsCause.length = 1;
            pDestBill->u.niLCSRecord.lcsCause.value[0] = pNilrBill->lcsCause;
        }
	}
	
	if (GET_CDR_FIELD_FILTER(diagnostics, NILCSRecord))				//diagnostics
	{
		pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_diagnostics_present;
		if (pNilrBill->diagnostics.CauseType == 0 && pNilrBill->diagnostics.Cause.gsm0408Cause != 0)
		{
			pDestBill->u.niLCSRecord.diagnostics.choice = CMCC_CS_gsm0408Cause_chosen;
			pDestBill->u.niLCSRecord.diagnostics.u.gsm0408Cause = pNilrBill->diagnostics.Cause.gsm0408Cause;
		}
		else if (pNilrBill->diagnostics.CauseType == 1 && pNilrBill->diagnostics.Cause.gsm0902MapErrorValue != 0)
		{
			pDestBill->u.niLCSRecord.diagnostics.choice = CMCC_CS_gsm0902MapErrorValue_chosen;
			pDestBill->u.niLCSRecord.diagnostics.u.gsm0902MapErrorValue = pNilrBill->diagnostics.Cause.gsm0902MapErrorValue;
		}
		else if (pNilrBill->diagnostics.CauseType == 2 && pNilrBill->diagnostics.Cause.ccittQ767Cause != 0)
		{
			pDestBill->u.niLCSRecord.diagnostics.choice = CMCC_CS_ccittQ767Cause_chosen;
			pDestBill->u.niLCSRecord.diagnostics.u.ccittQ767Cause = pNilrBill->diagnostics.Cause.ccittQ767Cause;
		}
		else
		{
			pDestBill->u.niLCSRecord.bit_mask &= ~CMCC_CS_NILCSRecord_diagnostics_present;
		}
	}

	if (GET_CDR_FIELD_FILTER(systemType, NILCSRecord))				//systemType
	{	
        switch((ACCESS_NETWORK_TYPE)pNilrBill->systemType)
        {
        case ACCESS_BSC :
            pDestBill->u.niLCSRecord.systemType = CMCC_CS_gERAN;
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_systemType_present;
            break;
        case ACCESS_RNC :
            pDestBill->u.niLCSRecord.systemType = CMCC_CS_iuUTRAN;
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_systemType_present;
            break;
        case ACCESS_UNKNOWN:
            pDestBill->u.niLCSRecord.systemType = CMCC_CS_unknown;
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_systemType_present;
            break;
        } 
	}

    pDestBill->u.niLCSRecord.recordExtensions = NULL;					//recordExtensions(目前不支持)

    if (GET_CDR_FIELD_FILTER(causeForTerm, NILCSRecord))
    {
        pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_causeForTerm_present;
        pDestBill->u.niLCSRecord.causeForTerm = pNilrBill->causeForTerm;
    }

    pDestBill->u.niLCSRecord.subscriberCategory.length = 0;
    if (GET_CDR_FIELD_FILTER(subscriberCategory, NILCSRecord))
	{
        pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_subscriberCategory_present;
        pDestBill->u.niLCSRecord.subscriberCategory.length = 1;
        pDestBill->u.niLCSRecord.subscriberCategory.value[0] = pNilrBill->subscriberCategory;
    }

    pDestBill->u.niLCSRecord.firstmccmnc.length = 0;
    if (GET_CDR_FIELD_FILTER(firstmccmnc, NILCSRecord))
	{
        if (!(pNilrBill->served_location_LAI[0] == 0xFF
            && pNilrBill->served_location_LAI[1] == 0xFF
            && pNilrBill->served_location_LAI[2] == 0xFF)
            && !(pNilrBill->served_location_LAI[0] == 0
            && pNilrBill->served_location_LAI[1] == 0
            && pNilrBill->served_location_LAI[2] == 0))
        {
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_firstmccmnc_present;
            pDestBill->u.niLCSRecord.firstmccmnc.length = 3;
            memcpy(pDestBill->u.niLCSRecord.firstmccmnc.value, pNilrBill->served_location_LAI, 3);
        }
    }

    pDestBill->u.niLCSRecord.orgRNCorBSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgRNCorBSCId, NILCSRecord))
	{
        if (pNilrBill->msc_id_len != 0)
        {
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_orgRNCorBSCId_present;
            pDestBill->u.niLCSRecord.orgRNCorBSCId.length = min(3,pNilrBill->rnc_bsc_id_len);
            memcpy(pDestBill->u.niLCSRecord.orgRNCorBSCId.value, pNilrBill->orgRNCorBSCId, pDestBill->u.niLCSRecord.orgRNCorBSCId.length);
        }
    }

    pDestBill->u.niLCSRecord.orgMSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgMSCId, NILCSRecord))
	{
        if (pNilrBill->msc_id_len != 0)
        {
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_orgMSCId_present;
            pDestBill->u.niLCSRecord.orgMSCId.length = min(3,pNilrBill->msc_id_len);
            memcpy(pDestBill->u.niLCSRecord.orgMSCId.value, pNilrBill->orgMSCId, pDestBill->u.niLCSRecord.orgMSCId.length);
        }
    }

	if (GET_CDR_FIELD_FILTER(hotBillingTag, NILCSRecord))
	{
		if (pNilrBill->HotBillingTag == HOT_BILLING)
		{
		    pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_hotBillingTag_present;		
            pDestBill->u.niLCSRecord.hotBillingTag = CMCC_CS_hotBilling;
		}
		else
		{
			pDestBill->u.niLCSRecord.hotBillingTag = CMCC_CS_noHotBilling;
		}
	}

    pDestBill->u.niLCSRecord.globalAreaID.length = 0;
    if (GET_CDR_FIELD_FILTER(globalAreaID, NILCSRecord))     //1
    {
        if (IsLACandCellIDValid(&(pNilrBill->served_location_LAI[3]),pNilrBill->served_location_cellID))
        {                       
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_globalAreaID_present;
            pDestBill->u.niLCSRecord.globalAreaID.length = 7;
            memcpy(pDestBill->u.niLCSRecord.globalAreaID.value, pNilrBill->served_location_LAI, 5);
            memcpy(pDestBill->u.niLCSRecord.globalAreaID.value + 5, pNilrBill->served_location_cellID, 2);
         }
    }

    if (GET_CDR_FIELD_FILTER(additionalChgInfo, NILCSRecord))		//附加计费信息
    {
        if (pNilrBill->chargeIndicator >= 0 && pNilrBill->chargeIndicator < 3)
        {
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_additionalChgInfo_present;
            pDestBill->u.niLCSRecord.additionalChgInfo.bit_mask = CMCC_CS_chargeIndicator_present;
            pDestBill->u.niLCSRecord.additionalChgInfo.chargeIndicator = pNilrBill->chargeIndicator;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(chargedParty,NILCSRecord) )
    {
        if (pNilrBill->chargedParty == 0 || pNilrBill->chargedParty == 1)
        {
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_chargedParty_present;
            pDestBill->u.niLCSRecord.chargedParty = (CMCC_CS_ChargedParty)pNilrBill->chargedParty;
        }
    }
    
   	pDestBill->u.niLCSRecord.servedIMEI.length = 0;
    if (GET_CDR_FIELD_FILTER(servedIMEI, NILCSRecord))		//ServedIMEI
    {
        pDestBill->u.niLCSRecord.servedIMEI.length = 
            BCDtoRBCD(pDestBill->u.niLCSRecord.servedIMEI.value, pNilrBill->servedIMEI, 8);
        if (pDestBill->u.niLCSRecord.servedIMEI.length > 0)
        {
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_servedIMEI_present;        
        }
    }

    if (GET_CDR_FIELD_FILTER(lcsCallReference,NILCSRecord)) 
    {
        if (pNilrBill->call_reference_len != 0)
        {
            int nCallReferenceLen = min(pNilrBill->call_reference_len,
                sizeof(pDestBill->u.niLCSRecord.lcsReferenceNumber.value));
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_lcsReferenceNumber_present;
            pDestBill->u.niLCSRecord.lcsReferenceNumber.length = nCallReferenceLen;
            memcpy(pDestBill->u.niLCSRecord.lcsReferenceNumber.value, 
                &(pNilrBill->call_reference[sizeof(pNilrBill->call_reference) - pNilrBill->call_reference_len]), nCallReferenceLen);   
        }
        else
        {
            CMCC_CS_CallReference ncallreference;
            pDestBill->u.niLCSRecord.bit_mask |= CMCC_CS_NILCSRecord_lcsReferenceNumber_present;
            GetCallReference(ncallreference, pNilrBill->ModuleNum, pNilrBill->count);
            pDestBill->u.niLCSRecord.lcsReferenceNumber.length = ncallreference.length;
            memcpy(pDestBill->u.niLCSRecord.lcsReferenceNumber.value,ncallreference.value,ncallreference.length);
        }

    }
    
	return TRUE;
}

BOOL ConvertABill_MOC(const BYTE* pSrcBill, const int nLen, 
                  const BYTE* pEventBill, const int nEventNum, 
                  CMCC_CS_CallEventRecord* pDestBill)
{
	MOC_Bill_Report* pMocBill = (MOC_Bill_Report *)pSrcBill;
	pDestBill->choice = CMCC_CS_moCallRecord_chosen;
	memset(pDestBill->u.moCallRecord.bit_mask, 0, sizeof(pDestBill->u.moCallRecord.bit_mask));

    if (GET_CDR_FIELD_FILTER(recordType, MOCallRecord))
    {
        pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_recordType_present_byte]
            |= CMCC_CS_MOCallRecord_recordType_present;
	    pDestBill->u.moCallRecord.recordType = CMCC_CS_CallEventRecordType_moCallRecord;		//记录类型
    }

	pDestBill->u.moCallRecord.servedIMSI.length = 0;
    if (GET_CDR_FIELD_FILTER(servedIMSI, MOCallRecord))		//servedIMSI
	{
        pDestBill->u.moCallRecord.servedIMSI.length = 
            BCDtoRBCD(pDestBill->u.moCallRecord.servedIMSI.value, pMocBill->servedIMSI, 8);
        if (pDestBill->u.moCallRecord.servedIMSI.length > 0)
        {
		    pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_servedIMSI_present_byte] |=
                CMCC_CS_MOCallRecord_servedIMSI_present;        
        }
	}

    pDestBill->u.moCallRecord.servedIMEI.length = 0;
	if (GET_CDR_FIELD_FILTER(servedIMEI, MOCallRecord))		//servedIMEI
	{
		pDestBill->u.moCallRecord.servedIMEI.length = 
            BCDtoRBCD(pDestBill->u.moCallRecord.servedIMEI.value, pMocBill->servedIMEI, 8);
        if (pDestBill->u.moCallRecord.servedIMEI.length > 0)
        {
		    pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_servedIMEI_present_byte] |=
                CMCC_CS_MOCallRecord_servedIMEI_present;        
        }
	}

	pDestBill->u.moCallRecord.servedMSISDN.length = 0;
    if (GET_CDR_FIELD_FILTER(servedMSISDN, MOCallRecord))		//servedMSISDN
	{
	    int nMSISDNLen = (pMocBill->servedMSISDN_BCDlen + 1) / 2;
        if (nMSISDNLen > 0)
        {
		    pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_servedMSISDN_present_byte] |=
                CMCC_CS_MOCallRecord_servedMSISDN_present; 
            pDestBill->u.moCallRecord.servedMSISDN.length = 
                min(sizeof(pDestBill->u.moCallRecord.servedMSISDN.value), nMSISDNLen + 1);
            pDestBill->u.moCallRecord.servedMSISDN.value[0] = *(pMocBill->servedMSISDN - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.moCallRecord.servedMSISDN.value + 1, pMocBill->servedMSISDN, 
                pDestBill->u.moCallRecord.servedMSISDN.length - 1);
        }
	}

	pDestBill->u.moCallRecord.callingNumber.length = 0;
    pDestBill->u.moCallRecord.callingNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(callingNumber, MOCallRecord))		//主叫号码
	{
		int nCallerLen = (pMocBill->callingNumber_BCDlen + 1) / 2;
        if (nCallerLen > 0)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_callingNumber_present_byte] |=
                CMCC_CS_MOCallRecord_callingNumber_present;
		    pDestBill->u.moCallRecord.callingNumber.length = nCallerLen + 1;
		    pDestBill->u.moCallRecord.callingNumber.value = new (g_pAsnBuffForCMCC)BYTE[nCallerLen + 1];
            pDestBill->u.moCallRecord.callingNumber.value[0] = *(pMocBill->callingNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.moCallRecord.callingNumber.value + 1, pMocBill->callingNumber, nCallerLen);
        }
	}

	pDestBill->u.moCallRecord.calledNumber.length = 0;
    pDestBill->u.moCallRecord.calledNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(calledNumber, MOCallRecord))		//被叫号码
	{
		int nCalledLen = (pMocBill->calledNumber_BCDlen + 1) / 2;
        if (nCalledLen > 0)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_calledNumber_present_byte] |=
                CMCC_CS_MOCallRecord_calledNumber_present;
		    pDestBill->u.moCallRecord.calledNumber.length = nCalledLen + 1;
		    pDestBill->u.moCallRecord.calledNumber.value = new (g_pAsnBuffForCMCC)BYTE[nCalledLen + 1];
            pDestBill->u.moCallRecord.calledNumber.value[0] = *(pMocBill->calledNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.moCallRecord.calledNumber.value + 1, pMocBill->calledNumber, nCalledLen);
	    }
    }

	pDestBill->u.moCallRecord.translatedNumber.length = 0;
    pDestBill->u.moCallRecord.translatedNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(translatedNumber, MOCallRecord))		//Translated Number
	{
		int nTranslatedLen = (pMocBill->translatedNumberTYPElen + 1) / 2;
        if (nTranslatedLen > 0)
        {
		    pDestBill->u.moCallRecord.bit_mask[CMCC_CS_translatedNumber_present_byte] |=
                CMCC_CS_translatedNumber_present;  
		    pDestBill->u.moCallRecord.translatedNumber.length = nTranslatedLen + 1;
		    pDestBill->u.moCallRecord.translatedNumber.value = new (g_pAsnBuffForCMCC)BYTE[nTranslatedLen + 1];
            pDestBill->u.moCallRecord.translatedNumber.value[0] = *(pMocBill->translatedNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.moCallRecord.translatedNumber.value + 1, pMocBill->translatedNumber, nTranslatedLen);
        }
	}

	pDestBill->u.moCallRecord.connectedNumber.length = 0;
    pDestBill->u.moCallRecord.connectedNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(connectedNumber, MOCallRecord))		//连接号码
	{
		int nConnectedLen = (pMocBill->connectedNumber_BCDlen + 1) / 2;
        if (nConnectedLen > 0)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_connectedNumber_present_byte] |=
                CMCC_CS_MOCallRecord_connectedNumber_present;
		    pDestBill->u.moCallRecord.connectedNumber.length = nConnectedLen + 1;
		    pDestBill->u.moCallRecord.connectedNumber.value = new (g_pAsnBuffForCMCC)BYTE[nConnectedLen + 1];
            pDestBill->u.moCallRecord.connectedNumber.value[0] = *(pMocBill->connectedNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.moCallRecord.connectedNumber.value + 1, pMocBill->connectedNumber, nConnectedLen);
        }
	}

    pDestBill->u.moCallRecord.roamingNumber.length = 0;
    if (GET_CDR_FIELD_FILTER(roamingNumber, MOCallRecord))		//漫游号码
	{
		int nRoamingLen = (pMocBill->roamingNumber_BCDlen + 1) / 2;
        if (nRoamingLen > 0)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_roamingNumber_present_byte] |=
                CMCC_CS_MOCallRecord_roamingNumber_present;
		    pDestBill->u.moCallRecord.roamingNumber.length = 
                min(sizeof(pDestBill->u.moCallRecord.roamingNumber.value), nRoamingLen + 1);
            pDestBill->u.moCallRecord.roamingNumber.value[0] = *(pMocBill->roamingNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.moCallRecord.roamingNumber.value + 1, pMocBill->roamingNumber, 
                pDestBill->u.moCallRecord.roamingNumber.length - 1);
        }
	}

    pDestBill->u.moCallRecord.recordingEntity.length = 0;
    if (GET_CDR_FIELD_FILTER(recordingEntity, MOCallRecord))
    {
        int nLocalMSCLen = (pMocBill->recordingEntity_BCDlen + 1) / 2;
        if (nLocalMSCLen > 0)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_recordingEntity_present_byte] |=
                CMCC_CS_MOCallRecord_recordingEntity_present; 
            pDestBill->u.moCallRecord.recordingEntity.length = 
                min(sizeof(pDestBill->u.moCallRecord.recordingEntity.value), nLocalMSCLen + 1);
            pDestBill->u.moCallRecord.recordingEntity.value[0] = *(pMocBill->recordingEntity - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.moCallRecord.recordingEntity.value + 1, pMocBill->recordingEntity, 
                pDestBill->u.moCallRecord.recordingEntity.length - 1);
        }
    }
    
    pDestBill->u.moCallRecord.mscIncomingROUTE.u.rOUTEName = NULL;
	if (GET_CDR_FIELD_FILTER(mscIncomingROUTE, MOCallRecord))		//入中继
	{
        if (pMocBill->mscIncomingROUTE_ind == SHOW_ROUTE_NO)
        {
            if (pMocBill->mscIncomingROUTE_ROUTENumber != 0xFFFF) 
            {
                pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_mscIncomingROUTE_present_byte] |= 
                    CMCC_CS_MOCallRecord_mscIncomingROUTE_present;
                pDestBill->u.moCallRecord.mscIncomingROUTE.choice = CMCC_CS_rOUTENumber_chosen;
                pDestBill->u.moCallRecord.mscIncomingROUTE.u.rOUTENumber = pMocBill->mscIncomingROUTE_ROUTENumber;
            }
        }
        else if (pMocBill->mscIncomingROUTE_ind == SHOW_ROUTE_NAME)
        {
            if (*(pMocBill->mscIncomingROUTE_ROUTEName) != 0x20)
            {
                pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_mscIncomingROUTE_present_byte] |= 
                    CMCC_CS_MOCallRecord_mscIncomingROUTE_present;
                pDestBill->u.moCallRecord.mscIncomingROUTE.choice = CMCC_CS_rOUTEName_chosen;
                pDestBill->u.moCallRecord.mscIncomingROUTE.u.rOUTEName = new (g_pAsnBuffForCMCC)char[11];
                memcpy(pDestBill->u.moCallRecord.mscIncomingROUTE.u.rOUTEName, pMocBill->mscIncomingROUTE_ROUTEName, 10);
                pDestBill->u.moCallRecord.mscIncomingROUTE.u.rOUTEName[10] = '\0';
            }
        }
	}
	
    pDestBill->u.moCallRecord.mscOutgoingROUTE.u.rOUTEName = NULL;
	if (GET_CDR_FIELD_FILTER(mscOutgoingROUTE, MOCallRecord))		//出中继
	{
        if (pMocBill->mscOutgoingROUT_ind == SHOW_ROUTE_NO)
        {
            if (pMocBill->mscOutgoingROUTE_ROUTENumber != 0xFFFF)
            {
                pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_mscOutgoingROUTE_present_byte] |=
                    CMCC_CS_MOCallRecord_mscOutgoingROUTE_present;
                pDestBill->u.moCallRecord.mscOutgoingROUTE.choice = CMCC_CS_rOUTENumber_chosen;
                pDestBill->u.moCallRecord.mscOutgoingROUTE.u.rOUTENumber = pMocBill->mscOutgoingROUTE_ROUTENumber;
            }
        }
        else if (pMocBill->mscOutgoingROUT_ind == SHOW_ROUTE_NAME)
        {
            if (*(pMocBill->mscOutgoingROUTE_ROUTEName) != 0x20)
            {
                pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_mscOutgoingROUTE_present_byte] |=
                    CMCC_CS_MOCallRecord_mscOutgoingROUTE_present;
                pDestBill->u.moCallRecord.mscOutgoingROUTE.choice = CMCC_CS_rOUTEName_chosen;
                pDestBill->u.moCallRecord.mscOutgoingROUTE.u.rOUTEName = new (g_pAsnBuffForCMCC)char[11];
                memcpy(pDestBill->u.moCallRecord.mscOutgoingROUTE.u.rOUTEName, pMocBill->mscOutgoingROUTE_ROUTEName, 10);
                pDestBill->u.moCallRecord.mscOutgoingROUTE.u.rOUTEName[10] = '\0';
            }
        }
	}

	if (GET_CDR_FIELD_FILTER(location, MOCallRecord))			//初始位置区小区
	{
        if (IsLACandCellIDValid(&(pMocBill->caller_location_LAI[3]), pMocBill->caller_location_cellID))
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_location_present_byte] |= 
                CMCC_CS_MOCallRecord_location_present;
		    pDestBill->u.moCallRecord.location.locationAreaCode.length = 2;
            memcpy(pDestBill->u.moCallRecord.location.locationAreaCode.value, pMocBill->caller_location_LAI + 3, 2);
            pDestBill->u.moCallRecord.location.cellIdentifier.length = 2;
            memcpy(pDestBill->u.moCallRecord.location.cellIdentifier.value, pMocBill->caller_location_cellID,2);
        }
	}

	pDestBill->u.moCallRecord.changeOfLocation = NULL;
    if (GET_CDR_FIELD_FILTER(changeOfLocation, MOCallRecord))			//changeOfLocation
	{
        if (pEventBill != NULL && nEventNum != 0 && pMocBill->HO_invoked_times != 0)
        {
            SS_Action_Bill_Report* pSingleEventBill = NULL;

            for(int i = nEventNum - 1; i >= 0; i--) //由于后面的链表是插入方式，所以这里从后往前找
            {
                pSingleEventBill = (SS_Action_Bill_Report*)((MSC_Bill_Report*)pEventBill + i);

                if (pSingleEventBill->event_type == HO)
                {
                    CMCC_CS__seqof1_* pLocation = new (g_pAsnBuffForCMCC)CMCC_CS__seqof1_;
                    ConvertTimeStamp(pLocation->value.changeTime, &(pSingleEventBill->ssActionTime_year));
		            
                    pLocation->value.location.locationAreaCode.length = 2;
                    memcpy(pLocation->value.location.locationAreaCode.value, pSingleEventBill->served_location_LAI + 3, 2);
                    pLocation->value.location.cellIdentifier.length = 2;
                    memcpy(pLocation->value.location.cellIdentifier.value, pSingleEventBill->served_location_cellID,2);

                    pLocation->next = pDestBill->u.moCallRecord.changeOfLocation;
                    pDestBill->u.moCallRecord.changeOfLocation = pLocation;
                }
            }
        }
        //如果不出事件话单但是当前位置区小区有效并且跟初始值不相同，那么还要填当前值
        else if (pMocBill->cause_for_partial_record == CMCC_CS_locationChange)
        {
            CMCC_CS__seqof1_* pLocation = new (g_pAsnBuffForCMCC)CMCC_CS__seqof1_;
            ConvertTimeStamp(pLocation->value.changeTime, &(pMocBill->end_year));  //时间取结束时间
		    
            pLocation->value.location.locationAreaCode.length = 2;
            memcpy(pLocation->value.location.locationAreaCode.value, pMocBill->caller_changeOflocation_LAI + 3, 2);
            pLocation->value.location.cellIdentifier.length = 2;
            memcpy(pLocation->value.location.cellIdentifier.value, pMocBill->caller_changeOflocation_cellID,2);

            pLocation->next = pDestBill->u.moCallRecord.changeOfLocation;
            pDestBill->u.moCallRecord.changeOfLocation = pLocation;
        }

        if (pDestBill->u.moCallRecord.changeOfLocation != NULL)
        {
		    pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_changeOfLocation_present_byte] |= 
                CMCC_CS_MOCallRecord_changeOfLocation_present;
        }
	}

	if (GET_CDR_FIELD_FILTER(basicService, MOCallRecord))			//电话或承载业务码
	{
        if (pMocBill->TBS < 2)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_basicService_present_byte] |=
                CMCC_CS_MOCallRecord_basicService_present;
            ConvertBasicServiceCode(pDestBill->u.moCallRecord.basicService, 
                pMocBill->TBS, pMocBill->ServiceCode);
        }
	}

	if (GET_CDR_FIELD_FILTER(transparencyIndicator, MOCallRecord))	//transparencyIndicator
	{
        if (pMocBill->transparencyIndicator < 2)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_transparencyIndicator_present_byte] |=
                CMCC_CS_MOCallRecord_transparencyIndicator_present;
            pDestBill->u.moCallRecord.transparencyIndicator = (CMCC_CS_TransparencyInd)pMocBill->transparencyIndicator;
        }
	}

	pDestBill->u.moCallRecord.changeOfService = NULL;

	pDestBill->u.moCallRecord.supplServicesUsed = NULL;
    if (GET_CDR_FIELD_FILTER(supplServicesUsed, MOCallRecord))		//supplServicesUsed
	{
        int nSSCount = 0;
        
        nSSCount = pMocBill->CH_invoked_times + pMocBill->CH_retrieve_times +
            pMocBill->CW_invoked_times + pMocBill->MPTY_Build_times + pMocBill->MPTY_CH_times
            + pMocBill->MPTY_CH_retrieve_times + pMocBill->MPTY_Split_times;
        
        if (pEventBill != NULL && nEventNum != 0 && nSSCount != 0)
        {
            SS_Action_Bill_Report* pSingleEventBill = NULL;
            
            for(int i = nEventNum - 1; i >= 0; i--)
            {
                pSingleEventBill = (SS_Action_Bill_Report*)((MSC_Bill_Report*)pEventBill + i);
                
                CMCC_CS__seqof3_* pSS = NULL;
                switch (pSingleEventBill->event_type)
                {
                case Hold_Call:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CHOLD;
                    break;
                case Retrieve_Call:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CHOLD;
                    break;
                case CallWait:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CWAIT;
                    break;
                case Build_MPTY:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CMPTY;
                    break;
                case Hold_MPTY:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CMPTY;
                    break;
                case Retrieve_MPTY:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CMPTY;
                    break;
                case Split_MPTY:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CMPTY;
                    break;
                default:
                    break;
                }
                
                if (pSS != NULL)
                {
                    pSS->value.bit_mask = 0;
                    pSS->value.bit_mask |= CMCC_CS_ssTime_present;
                    pSS->value.bit_mask |= CMCC_CS_ssCode_present;
                    ConvertTimeStamp(pSS->value.ssTime, &(pSingleEventBill->ssActionTime_year));
                    pSS->next = pDestBill->u.moCallRecord.supplServicesUsed;
                    pDestBill->u.moCallRecord.supplServicesUsed = pSS;
                }
            }
        }
        
        ConvertSScode((BYTE*)(&(pMocBill->SS_code1)), &(pMocBill->initial_year), &(pDestBill->u.moCallRecord.supplServicesUsed));

        if (pDestBill->u.moCallRecord.supplServicesUsed != NULL)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_supplServicesUsed_present_byte] |= 
                CMCC_CS_MOCallRecord_supplServicesUsed_present;
        }
	}

	if (GET_CDR_FIELD_FILTER(aocParameters, MOCallRecord))		//aocParameters
	{
        pDestBill->u.moCallRecord.aocParameters.bit_mask = 0;

        AOC_E1_E7* pAoc = NULL;
        pAoc = &(pMocBill->aocParameters);
        if(pAoc->aoc != 0xFF)
        {
            pDestBill->u.moCallRecord.aocParameters.bit_mask |= CMCC_CS_e1_present;
            pDestBill->u.moCallRecord.aocParameters.e1 = pAoc->e[0];

            pDestBill->u.moCallRecord.aocParameters.bit_mask |= CMCC_CS_e2_present;
            pDestBill->u.moCallRecord.aocParameters.e2 = pAoc->e[1];
            
            pDestBill->u.moCallRecord.aocParameters.bit_mask |= CMCC_CS_e3_present;
            pDestBill->u.moCallRecord.aocParameters.e3 = pAoc->e[2];

            pDestBill->u.moCallRecord.aocParameters.bit_mask |= CMCC_CS_e4_present;
            pDestBill->u.moCallRecord.aocParameters.e4 = pAoc->e[3];

            pDestBill->u.moCallRecord.aocParameters.bit_mask |= CMCC_CS_e5_present;
            pDestBill->u.moCallRecord.aocParameters.e5 = pAoc->e[4];

            pDestBill->u.moCallRecord.aocParameters.bit_mask |= CMCC_CS_e6_present;
            pDestBill->u.moCallRecord.aocParameters.e6 = pAoc->e[5];
            
            pDestBill->u.moCallRecord.aocParameters.bit_mask |= CMCC_CS_e7_present;
            pDestBill->u.moCallRecord.aocParameters.e7 = pAoc->e[6];

            if (pDestBill->u.moCallRecord.aocParameters.bit_mask != 0)
            {
                pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_aocParameters_present_byte] |=
                    CMCC_CS_MOCallRecord_aocParameters_present;    
            }
        }
    }

    pDestBill->u.moCallRecord.changeOfAOCParms = NULL;

    pDestBill->u.moCallRecord.msClassmark.length = 0;
    pDestBill->u.moCallRecord.msClassmark.value = NULL;
    if (GET_CDR_FIELD_FILTER(msClassmark, MOCallRecord))		//msClassmark
	{
		//if (pMocBill->BillType != CFW)  //CFW话单不要这个字段
        if(!(pMocBill->MSClassmark[0] == 0
            && pMocBill->MSClassmark[1] == 0
            && pMocBill->MSClassmark[2] == 0)
            && !(pMocBill->MSClassmark[0] == 0xFF
            && pMocBill->MSClassmark[1] == 0xFF
            && pMocBill->MSClassmark[2] == 0xFF))
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_msClassmark_present_byte] |=
                CMCC_CS_MOCallRecord_msClassmark_present;
		    pDestBill->u.moCallRecord.msClassmark.length = 3;
		    pDestBill->u.moCallRecord.msClassmark.value = new (g_pAsnBuffForCMCC)BYTE[3];
		    memcpy(pDestBill->u.moCallRecord.msClassmark.value, pMocBill->MSClassmark, 3);
        }
	}

    pDestBill->u.moCallRecord.changeOfClassmark.classmark.length = 0;
    pDestBill->u.moCallRecord.changeOfClassmark.classmark.value = NULL;

    if (GET_CDR_FIELD_FILTER(seizureTime, MOCallRecord))		//应答时间
	{
        if (pMocBill->causeForTerm == unsuccessfulCallAttempt)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_seizureTime_present_byte] |=
                CMCC_CS_MOCallRecord_seizureTime_present;
            ConvertTimeStamp(pDestBill->u.moCallRecord.seizureTime, &(pMocBill->initial_year));
        }
    }

    if (GET_CDR_FIELD_FILTER(answerTime, MOCallRecord))		//应答时间
	{
        if (pMocBill->causeForTerm != unsuccessfulCallAttempt)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_answerTime_present_byte] |=
                CMCC_CS_MOCallRecord_answerTime_present;
		    ConvertTimeStamp(pDestBill->u.moCallRecord.answerTime, &(pMocBill->initial_year));
        }
	}

    if (GET_CDR_FIELD_FILTER(releaseTime, MOCallRecord))		//释放时间
	{
		if (pMocBill->end_year != 0xFF)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_releaseTime_present_byte] |=
                CMCC_CS_MOCallRecord_releaseTime_present;
		    ConvertTimeStamp(pDestBill->u.moCallRecord.releaseTime, &(pMocBill->end_year));
        }
	}

    if (GET_CDR_FIELD_FILTER(callDuration, MOCallRecord))
    {
        pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_callDuration_present_byte] |=
            CMCC_CS_MOCallRecord_callDuration_present; 
        pDestBill->u.moCallRecord.callDuration = pMocBill->callDuration;	//通话时长
    }

    if (GET_CDR_FIELD_FILTER(radioChanRequested, MOCallRecord))		
	{
        if (pMocBill->radioChanRequested > 0 && pMocBill->radioChanRequested < 4)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_radioChanRequested_present_byte] |=
                CMCC_CS_MOCallRecord_radioChanRequested_present;
            pDestBill->u.moCallRecord.radioChanRequested = (CMCC_CS_RadioChanRequested)pMocBill->radioChanRequested;
	    }
    }

    if (GET_CDR_FIELD_FILTER(radioChanUsed, MOCallRecord))		//radioChanUsed
	{
            if (pMocBill->TBS == MBILL_TELECOM_SERVICE && (0 == pMocBill->radioChanUsed || 1 == pMocBill->radioChanUsed))
            {
                pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_radioChanUsed_present_byte] |=
                    CMCC_CS_MOCallRecord_radioChanUsed_present;
                if (pMocBill->radioChanUsed == 1)  //half
                {
                    pDestBill->u.moCallRecord.radioChanUsed = CMCC_CS_halfRate;
                }
                else
                {
                    pDestBill->u.moCallRecord.radioChanUsed = CMCC_CS_fullRate;
                }
            }
	}

    if (GET_CDR_FIELD_FILTER(causeForTerm, MOCallRecord))
    {
        pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_causeForTerm_present_byte] |=
            CMCC_CS_MOCallRecord_causeForTerm_present; 
        pDestBill->u.moCallRecord.causeForTerm = pMocBill->causeForTerm;
    }

    if (GET_CDR_FIELD_FILTER(diagnostics, MOCallRecord))		//diagnostics
	{
        pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_diagnostics_present_byte] |=
            CMCC_CS_MOCallRecord_diagnostics_present;
		if (pMocBill->diagnostics.CauseType == 0)
		{
			pDestBill->u.moCallRecord.diagnostics.choice = CMCC_CS_gsm0408Cause_chosen;
			pDestBill->u.moCallRecord.diagnostics.u.gsm0408Cause = pMocBill->diagnostics.Cause.gsm0408Cause;
		}
		else if (pMocBill->diagnostics.CauseType == 1)
		{
			pDestBill->u.moCallRecord.diagnostics.choice = CMCC_CS_gsm0902MapErrorValue_chosen;
			pDestBill->u.moCallRecord.diagnostics.u.gsm0902MapErrorValue = pMocBill->diagnostics.Cause.gsm0902MapErrorValue;
		}
		else if (pMocBill->diagnostics.CauseType == 2)
		{
			pDestBill->u.moCallRecord.diagnostics.choice = CMCC_CS_ccittQ767Cause_chosen;
			pDestBill->u.moCallRecord.diagnostics.u.ccittQ767Cause = pMocBill->diagnostics.Cause.ccittQ767Cause;
		}
		else
		{
			pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_diagnostics_present_byte] &=
                ~CMCC_CS_MOCallRecord_diagnostics_present;
		}
	}

    if (GET_CDR_FIELD_FILTER(callReference, MOCallRecord))
    {
        if (pMocBill->call_reference_len != 0)
        {
            int nCallReferenceLen = min(pMocBill->call_reference_len,
                sizeof(pDestBill->u.moCallRecord.callReference.value));
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_callReference_present_byte] |=
                CMCC_CS_MOCallRecord_callReference_present; 
            pDestBill->u.moCallRecord.callReference.length = nCallReferenceLen;
            memcpy(pDestBill->u.moCallRecord.callReference.value, 
                &(pMocBill->call_reference[sizeof(pMocBill->call_reference) - pMocBill->call_reference_len]), nCallReferenceLen);   
        }
        else
        {
          pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_callReference_present_byte] |=
                CMCC_CS_MOCallRecord_callReference_present; 
          GetCallReference(pDestBill->u.moCallRecord.callReference,
                pMocBill->ModuleNum, pMocBill->count);
        }
        
    }

    if (GET_CDR_FIELD_FILTER(sequenceNumber, MOCallRecord))		//中间话单流水号
	{
		if (pMocBill->record_type != SINGLE_BILL)
		{
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_sequenceNumber_present_byte] |=
                CMCC_CS_MOCallRecord_sequenceNumber_present;
		    pDestBill->u.moCallRecord.sequenceNumber = pMocBill->sequence;
		}
	}

    if (GET_CDR_FIELD_FILTER(additionalChgInfo, MOCallRecord))		//附加计费信息
	{
		if (pMocBill->chargeIndicator >= 0 && pMocBill->chargeIndicator < 3)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_additionalChgInfo_present_byte] |=
                CMCC_CS_MOCallRecord_additionalChgInfo_present;
            pDestBill->u.moCallRecord.additionalChgInfo.bit_mask = CMCC_CS_chargeIndicator_present;
		    pDestBill->u.moCallRecord.additionalChgInfo.chargeIndicator = pMocBill->chargeIndicator;
        }
	}

    pDestBill->u.moCallRecord.recordExtensions = NULL;

    if (GET_CDR_FIELD_FILTER(gsm_SCFAddress, MOCallRecord))		//gsm_SCFAddress
	{
	    int nGSMSCFAddressLen = (pMocBill->CAMEL_GsmSCF_Address_BCDlen + 1) / 2;
        if (nGSMSCFAddressLen > 0)
        {
		    pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_gsm_SCFAddress_present_byte] |=
                CMCC_CS_MOCallRecord_gsm_SCFAddress_present;
            //SCFAddress本身就是反向BCD码，而且第一个字节是多余的
            pDestBill->u.moCallRecord.gsm_SCFAddress.length = 
                min(sizeof(pDestBill->u.moCallRecord.gsm_SCFAddress.value), nGSMSCFAddressLen + 1);
            pDestBill->u.moCallRecord.gsm_SCFAddress.value[0] = *(pMocBill->CAMEL_GsmSCF_Address - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.moCallRecord.gsm_SCFAddress.value + 1, pMocBill->CAMEL_GsmSCF_Address, 
                pDestBill->u.moCallRecord.gsm_SCFAddress.length - 1);
        }
	}

    if (GET_CDR_FIELD_FILTER(serviceKey, MOCallRecord))		//业务键
	{
        if (pMocBill->ServiceKey != 0xFFFFFFFF)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_serviceKey_present_byte] |=
                CMCC_CS_MOCallRecord_serviceKey_present;
		    pDestBill->u.moCallRecord.serviceKey = pMocBill->ServiceKey;
        }
	}

    if (GET_CDR_FIELD_FILTER(networkCallReference, MOCallRecord))		//网络呼叫参考号
	{
        if (pMocBill->NetworkCallReferenceLen != 0)
        {
            int NetworkCallReferenceLen = min(pMocBill->NetworkCallReferenceLen, 
                sizeof(pDestBill->u.moCallRecord.networkCallReference.value));
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_networkCallReference_present_byte] |=
                CMCC_CS_MOCallRecord_networkCallReference_present;
		    pDestBill->u.moCallRecord.networkCallReference.length = NetworkCallReferenceLen;
    	    memcpy(pDestBill->u.moCallRecord.networkCallReference.value, 
                &(pMocBill->NetworkcallReferenceNumber[sizeof(pMocBill->NetworkcallReferenceNumber) - pMocBill->NetworkCallReferenceLen]), NetworkCallReferenceLen);
        }
	}

    if (GET_CDR_FIELD_FILTER(mSCAddress, MOCallRecord))		//MSC 地址
	{
        int nMSCAddressLen = (pMocBill->MSCAddress_BCDlen + 1) / 2;
        if (nMSCAddressLen > 0)
        {
		    pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_mSCAddress_present_byte] |=
                CMCC_CS_MOCallRecord_mSCAddress_present; 
            pDestBill->u.moCallRecord.mSCAddress.length = 
                min(sizeof(pDestBill->u.moCallRecord.mSCAddress.value), nMSCAddressLen + 1);
            pDestBill->u.moCallRecord.mSCAddress.value[0] = *(pMocBill->MSCAddress - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.moCallRecord.mSCAddress.value + 1, pMocBill->MSCAddress, 
                pDestBill->u.moCallRecord.mSCAddress.length - 1);
        }
	}

	//by 杨善	2004-5-17   对应问题单SWPD05184
    if (GET_CDR_FIELD_FILTER(cAMELInitCFIndicator, MOCallRecord))
	{
        if(pMocBill->CAMELInitCFIndicator)
        {
		    pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_cAMELInitCFIndicator_present_byte] |=
                CMCC_CS_MOCallRecord_cAMELInitCFIndicator_present;
            pDestBill->u.moCallRecord.cAMELInitCFIndicator = CMCC_CS_cAMELCallForwarding;
        }
    }
	//the end	SWPD05184

    if (GET_CDR_FIELD_FILTER(defaultCallHandling, MOCallRecord))		//defaultCallHandling
	{
		if (pMocBill->defaultCallHandling == 0 || pMocBill->defaultCallHandling == 1)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_defaultCallHandling_present_byte] |=
                CMCC_CS_MOCallRecord_defaultCallHandling_present;
		    if (pMocBill->defaultCallHandling == 0)
            {
                pDestBill->u.moCallRecord.defaultCallHandling = CMCC_CS_continueCall;
            }
            else
            {
                pDestBill->u.moCallRecord.defaultCallHandling = CMCC_CS_releaseCall;
            }
        }
	}

 
    if (GET_CDR_FIELD_FILTER(fnur, MOCallRecord))		//fnur
	{
        if (pMocBill->fnur != 0x00)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_fnur_present_byte] |=
                CMCC_CS_MOCallRecord_fnur_present;
            pDestBill->u.moCallRecord.fnur = (CMCC_CS_Fnur)pMocBill->fnur;
        }
	}

    if (GET_CDR_FIELD_FILTER(aiurRequested, MOCallRecord))		//aiurRequested
	{
        if(pMocBill->aiurRequested != 0)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_aiurRequested_present_byte] |=
                CMCC_CS_MOCallRecord_aiurRequested_present;
            pDestBill->u.moCallRecord.aiurRequested = (CMCC_CS_AiurRequested)pMocBill->aiurRequested;
        }
	}

   if (GET_CDR_FIELD_FILTER(speechVersionSupported, MOCallRecord))		//speechVersionSupported
	{
		if (pMocBill->TBS == MBILL_TELECOM_SERVICE && pMocBill->speechVersionSupported != 0xFF)
        {
            ConvertSpeechVersion(pDestBill->u.moCallRecord.speechVersionSupported.value[0],
                pMocBill->speechVersionSupported);
            if(pDestBill->u.moCallRecord.speechVersionSupported.value[0] != 0xFF)
            {
                pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_speechVersionSupported_present_byte] |=
                    CMCC_CS_MOCallRecord_speechVersionSupported_present;
                pDestBill->u.moCallRecord.speechVersionSupported.length = 1;
            }
        }
	}

    if (GET_CDR_FIELD_FILTER(speechVersionUsed, MOCallRecord))		//speechVersionUsed
	{
        if (pMocBill->TBS == MBILL_TELECOM_SERVICE && pMocBill->speechVersionUsed != 0xFF)
        {
            ConvertSpeechVersion(pDestBill->u.moCallRecord.speechVersionUsed.value[0],
                pMocBill->speechVersionUsed);
            if(pDestBill->u.moCallRecord.speechVersionUsed.value[0] != 0xFF)
            {
                pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_speechVersionUsed_present_byte] |=
                    CMCC_CS_MOCallRecord_speechVersionUsed_present;
                pDestBill->u.moCallRecord.speechVersionUsed.length = 1;
            }
        }
	}

    if (GET_CDR_FIELD_FILTER(numberOfDPEncountered, MOCallRecord))		//numberOfDPEncountered
	{
		if (pMocBill->numberOfDPEncountered != 0)  //pMocBill->UserType＝2为智能呼叫，现在不需要判断了
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_numberOfDPEncountered_present_byte] |=
                CMCC_CS_MOCallRecord_numberOfDPEncountered_present;
		    pDestBill->u.moCallRecord.numberOfDPEncountered = pMocBill->numberOfDPEncountered;
        }
	}

    if (GET_CDR_FIELD_FILTER(levelOfCAMELService, MOCallRecord))		//numberOfDPEncountered
	{
        if (pMocBill->levelOfCAMELService != 0xFF)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_levelOfCAMELService_present_byte] |=
                CMCC_CS_MOCallRecord_levelOfCAMELService_present;
            switch(pMocBill->levelOfCAMELService)
            {
            case 0:
                pDestBill->u.moCallRecord.levelOfCAMELService = CMCC_CS_basic;
                break;
            case 1:
                pDestBill->u.moCallRecord.levelOfCAMELService = CMCC_CS_callDurationSupervision;
                break;
            case 2:
                pDestBill->u.moCallRecord.levelOfCAMELService = CMCC_CS_onlineCharging;
                break;
            default:
                pDestBill->u.moCallRecord.levelOfCAMELService = CMCC_CS_basic;
                break;
            }
        }
	}

    if (GET_CDR_FIELD_FILTER(freeFormatData, MOCallRecord))		//自由格式数据
	{
        if (pMocBill->fcidata_len != 0)
        {
            int fci_len = min(sizeof(pDestBill->u.moCallRecord.freeFormatData.value), pMocBill->fcidata_len);
            memcpy(pDestBill->u.moCallRecord.freeFormatData.value, pMocBill->VarPart, fci_len);
		    pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_freeFormatData_present_byte] |=
                CMCC_CS_MOCallRecord_freeFormatData_present;
            pDestBill->u.moCallRecord.freeFormatData.length = fci_len;
        }
	}

    pDestBill->u.moCallRecord.cAMELCallLegInformation = NULL;
    if (GET_CDR_FIELD_FILTER(cAMELCallLegInformation, MOCallRecord))		//cAMELCallLegInformation
    {
        if(pMocBill->s1 && pMocBill->PromptAndCollectCharge)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_cAMELCallLegInformation_present_byte] |=
                CMCC_CS_MOCallRecord_cAMELCallLegInformation_present;
            
            CMCC_CS__seqof5_* pCAMELCallLegInformation = new (g_pAsnBuffForCMCC)CMCC_CS__seqof5_;
            pCAMELCallLegInformation->next = NULL;
            pCAMELCallLegInformation->value.bit_mask = 0;
            
            pCAMELCallLegInformation->value.bit_mask |= CMCC_CS_cAMELDestinationNumber_present;
            pCAMELCallLegInformation->value.cAMELDestinationNumber.value = 
                new (g_pAsnBuffForCMCC)BYTE[sizeof(pMocBill->ip_route_number) + 1];
            pCAMELCallLegInformation->value.cAMELDestinationNumber.value[0] = 
                *(BYTE*)(pMocBill->ip_route_number - 1);
            pCAMELCallLegInformation->value.cAMELDestinationNumber.value[0] |= 0x80;
            pCAMELCallLegInformation->value.cAMELDestinationNumber.length = 
                BCDtoRBCD(pCAMELCallLegInformation->value.cAMELDestinationNumber.value + 1, 
                pMocBill->ip_route_number, sizeof(pMocBill->ip_route_number)) + 1;
            
            pCAMELCallLegInformation->value.bit_mask |= CMCC_CS_CAMELInformation_seizureTime_present;
            ConvertTimeStamp(pCAMELCallLegInformation->value.seizureTime, &(pMocBill->resource_charge_initial_year));
            
            //下面计算releaseTime
            struct tm ts;
            struct tm *pte;
            time_t te;
            BYTE arEndTime[6];
            
            ts.tm_year = pMocBill->resource_charge_initial_year + 2000 - 1900;
            ts.tm_mon  = pMocBill->resource_charge_initial_month - 1;
            ts.tm_mday = pMocBill->resource_charge_initial_date;
            ts.tm_hour = pMocBill->resource_charge_initial_hour;
            ts.tm_min  = pMocBill->resource_charge_initial_minute;
            ts.tm_sec  = pMocBill->resource_charge_initial_second;
            
            te = mktime(&ts) + pMocBill->resource_charge_conversation_time;
            pte = localtime(&te);
            
            if(pte)
            {
                arEndTime[0] = pte->tm_year + 1900 - 2000;
                arEndTime[1] = pte->tm_mon + 1;
                arEndTime[2] = pte->tm_mday;
                arEndTime[3] = pte->tm_hour;
                arEndTime[4] = pte->tm_min;
                arEndTime[5] = pte->tm_sec;
                
                pCAMELCallLegInformation->value.bit_mask |= CMCC_CS_CAMELInformation_releaseTime_present;
                ConvertTimeStamp(pCAMELCallLegInformation->value.releaseTime, arEndTime);
            }
            
            pCAMELCallLegInformation->value.bit_mask |= CMCC_CS_CAMELInformation_callDuration_present;
            pCAMELCallLegInformation->value.callDuration = pMocBill->resource_charge_conversation_time;
            
            pDestBill->u.moCallRecord.cAMELCallLegInformation = pCAMELCallLegInformation;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(freeFormatDataAppend, MOCallRecord))		//freeFormatDataAppend
	{
        if ((AppendIndication)pMocBill->freeFormatDataAppend == Append)
        {
		    pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_freeFormatDataAppend_present_byte] |=
                CMCC_CS_MOCallRecord_freeFormatDataAppend_present;
            pDestBill->u.moCallRecord.freeFormatDataAppend = TRUE;
        }
        else
        {
		    pDestBill->u.moCallRecord.freeFormatDataAppend = FALSE;     
        }
	}

    if (GET_CDR_FIELD_FILTER(defaultCallHandling_2, MOCallRecord))		//defaultCallHandling2
	{
        if (pMocBill->defaultCallHandling2 == 0 || pMocBill->defaultCallHandling2 == 1)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_defaultCallHandling_2_present_byte] |=
                CMCC_CS_MOCallRecord_defaultCallHandling_2_present;
		    if (pMocBill->defaultCallHandling2 == 0)
            {
                pDestBill->u.moCallRecord.defaultCallHandling_2 = CMCC_CS_continueCall;
            }
            else
            {
                pDestBill->u.moCallRecord.defaultCallHandling_2 = CMCC_CS_releaseCall;
            }
        }
	}

    if (GET_CDR_FIELD_FILTER(gsm_SCFAddress_2, MOCallRecord))		//gsm_SCFAddress2
	{
	    int nGSMSCFAddress2Len = (pMocBill->CAMEL_GsmSCF_Address2_BCDlen + 1) / 2;
        if (nGSMSCFAddress2Len > 0)
        {
		    pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_gsm_SCFAddress_2_present_byte] |=
                CMCC_CS_MOCallRecord_gsm_SCFAddress_2_present;
            pDestBill->u.moCallRecord.gsm_SCFAddress_2.length = 
                min(sizeof(pDestBill->u.moCallRecord.gsm_SCFAddress_2.value), nGSMSCFAddress2Len + 1);
            pDestBill->u.moCallRecord.gsm_SCFAddress_2.value[0] = *(pMocBill->CAMEL_GsmSCF_Address2 - 2) | 0x80, 
            BCDtoRBCD(pDestBill->u.moCallRecord.gsm_SCFAddress_2.value + 1, pMocBill->CAMEL_GsmSCF_Address2, 
                pDestBill->u.moCallRecord.gsm_SCFAddress_2.length - 1);
        }
	}

    if (GET_CDR_FIELD_FILTER(serviceKey_2, MOCallRecord))		//业务键2
	{
		if (pMocBill->ServiceKey2 != 0xFFFFFFFF)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_serviceKey_2_present_byte] |=
                CMCC_CS_MOCallRecord_serviceKey_2_present;
		    pDestBill->u.moCallRecord.serviceKey_2 = pMocBill->ServiceKey2;
        }
	}

    if (GET_CDR_FIELD_FILTER(freeFormatData_2, MOCallRecord))		//自由格式数据
	{
        if (pMocBill->fcidata2_len != 0)
        {
            int fci2_len = min(sizeof(pDestBill->u.moCallRecord.freeFormatData_2.value), pMocBill->fcidata2_len);
            memcpy(pDestBill->u.moCallRecord.freeFormatData_2.value, pMocBill->VarPart + pMocBill->fcidata_len, fci2_len);
		    pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_freeFormatData_2_present_byte] |=
                CMCC_CS_MOCallRecord_freeFormatData_2_present;
            pDestBill->u.moCallRecord.freeFormatData_2.length = fci2_len;
        }
	}

    if (GET_CDR_FIELD_FILTER(freeFormatDataAppend_2, MOCallRecord))		//freeFormatDataAppend
	{
        if ((AppendIndication)pMocBill->freeFormatDataAppend2 == Append)
        {
		    pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_freeFormatDataAppend_2_present_byte] |=
                CMCC_CS_MOCallRecord_freeFormatDataAppend_2_present;
            pDestBill->u.moCallRecord.freeFormatDataAppend_2 = TRUE;
        }
        else
        {
		    pDestBill->u.moCallRecord.freeFormatDataAppend_2 = FALSE;     
        }
	}

    if (GET_CDR_FIELD_FILTER(systemType, MOCallRecord))		//系统类型
	{
        switch((ACCESS_NETWORK_TYPE)pMocBill->systemType)
        {
        case ACCESS_BSC :
            pDestBill->u.moCallRecord.systemType = CMCC_CS_gERAN;
		    pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_systemType_present_byte] |=
                CMCC_CS_MOCallRecord_systemType_present;
            break;
        case ACCESS_RNC :
            pDestBill->u.moCallRecord.systemType = CMCC_CS_iuUTRAN;
		    pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_systemType_present_byte] |=
                CMCC_CS_MOCallRecord_systemType_present;
            break;
        case ACCESS_UNKNOWN:
            pDestBill->u.moCallRecord.systemType = CMCC_CS_unknown;
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_systemType_present_byte] |= 
                CMCC_CS_MOCallRecord_systemType_present;
            break;
        } 
	}

    if (GET_CDR_FIELD_FILTER(rateIndication, MOCallRecord))		//rateIndication
	{
        if (pMocBill->rateIndication != 0x00)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_rateIndication_present_byte] |=
                CMCC_CS_MOCallRecord_rateIndication_present;
            pDestBill->u.moCallRecord.rateIndication.length = 1;
            pDestBill->u.moCallRecord.rateIndication.value[0] = pMocBill->rateIndication;
        }
    }   

    if (GET_CDR_FIELD_FILTER(mscOutgoingCircuit, MOCallRecord))		//mscOutgoingCircuit
	{
        if (pMocBill->mscOutgoingCircuit != 0xFFFF)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_mscOutgoingCircuit_present_byte] |=
                CMCC_CS_MOCallRecord_mscOutgoingCircuit_present;
            pDestBill->u.moCallRecord.mscOutgoingCircuit = pMocBill->mscOutgoingCircuit; 
        }
    }
    
    pDestBill->u.moCallRecord.orgRNCorBSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgRNCorBSCId, MOCallRecord))
	{
        if (pMocBill->rnc_bsc_id_len != 0)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_orgRNCorBSCId_present_byte] |=
                CMCC_CS_MOCallRecord_orgRNCorBSCId_present;
            pDestBill->u.moCallRecord.orgRNCorBSCId.length = min(3,pMocBill->rnc_bsc_id_len);
            memcpy(pDestBill->u.moCallRecord.orgRNCorBSCId.value, pMocBill->orgRNCorBSCId, pDestBill->u.moCallRecord.orgRNCorBSCId.length);
        }
    }

    if (GET_CDR_FIELD_FILTER(orgMSCId, MOCallRecord))
	{
        if (pMocBill->msc_id_len != 0)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_orgMSCId_present_byte] |=
                CMCC_CS_MOCallRecord_orgMSCId_present;
            pDestBill->u.moCallRecord.orgMSCId.length = min(3,pMocBill->msc_id_len);
            memcpy(pDestBill->u.moCallRecord.orgMSCId.value, pMocBill->orgMSCId, pDestBill->u.moCallRecord.orgMSCId.length);
        }
    }

    if (GET_CDR_FIELD_FILTER(callEmlppPriority, MOCallRecord))
	{
        if (pMocBill->callEmlppPriority != 0xF)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_callEmlppPriority_present_byte] |=
                CMCC_CS_MOCallRecord_callEmlppPriority_present;
            pDestBill->u.moCallRecord.callEmlppPriority.length = 1;
            pDestBill->u.moCallRecord.callEmlppPriority.value[0] = pMocBill->callEmlppPriority;
        }
    }

    if (GET_CDR_FIELD_FILTER(callerDefaultEmlppPriority, MOCallRecord))
	{
        if (pMocBill->callerDefaultEmlppPriority != 0xF)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_callerDefaultEmlppPriority_present_byte] |=
                CMCC_CS_callerDefaultEmlppPriority_present;
            pDestBill->u.moCallRecord.callerDefaultEmlppPriority.length = 1;
            pDestBill->u.moCallRecord.callerDefaultEmlppPriority.value[0] = pMocBill->callerDefaultEmlppPriority;
        }
    }

    pDestBill->u.moCallRecord.eaSubscriberInfo.length = 0;
    if (GET_CDR_FIELD_FILTER(eaSubscriberInfo, MOCallRecord)) //此域无法得到
	{
        if (pMocBill->eaSubscriberInfo[0] != 0xff &&
            pMocBill->eaSubscriberInfo[1] != 0xff &&
            pMocBill->eaSubscriberInfo[2] != 0xff)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_eaSubscriberInfo_present_byte] |= 
                CMCC_CS_MOCallRecord_eaSubscriberInfo_present; 
            pDestBill->u.moCallRecord.eaSubscriberInfo.length = 3;
            memcpy(pDestBill->u.moCallRecord.eaSubscriberInfo.value,pMocBill->eaSubscriberInfo,3);
        }        
    }

    if (GET_CDR_FIELD_FILTER(subscriberCategory, MOCallRecord))
	{
        pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_subscriberCategory_present_byte] |=
            CMCC_CS_MOCallRecord_subscriberCategory_present;
        pDestBill->u.moCallRecord.subscriberCategory.length = 1;
        pDestBill->u.moCallRecord.subscriberCategory.value[0] = pMocBill->subscriberCategory;
    }

    pDestBill->u.moCallRecord.firstmccmnc.length = 0;
    if (GET_CDR_FIELD_FILTER(firstmccmnc, MOCallRecord))
    {
        if (GET_CDR_FIELD_FILTER(firstmccmnc, MOCallRecord))
	    {
            if (!(pMocBill->caller_location_LAI[0] == 0xFF
                && pMocBill->caller_location_LAI[1] == 0xFF
                && pMocBill->caller_location_LAI[2] == 0xFF)
                && !(pMocBill->caller_location_LAI[0] == 0
                && pMocBill->caller_location_LAI[1] == 0
                && pMocBill->caller_location_LAI[2] == 0))
            {
                pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_firstmccmnc_present_byte] 
                    |= CMCC_CS_MOCallRecord_firstmccmnc_present;
                pDestBill->u.moCallRecord.firstmccmnc.length = 3;
                memcpy(pDestBill->u.moCallRecord.firstmccmnc.value, pMocBill->caller_location_LAI, 3);
            }
        }
    }

    pDestBill->u.moCallRecord.intermediatemccmnc.length = 0;
    if (GET_CDR_FIELD_FILTER(intermediatemccmnc, MOCallRecord))
	{
    }

    pDestBill->u.moCallRecord.lastmccmnc.length = 0;
    if (GET_CDR_FIELD_FILTER(lastmccmnc, MOCallRecord))
	{
        if (!(pMocBill->caller_changeOflocation_LAI[0] == 0xFF
            && pMocBill->caller_changeOflocation_LAI[1] == 0xFF
            && pMocBill->caller_changeOflocation_LAI[2] == 0xFF)
            && !(pMocBill->caller_changeOflocation_LAI[0] == 0
            && pMocBill->caller_changeOflocation_LAI[1] == 0
            && pMocBill->caller_changeOflocation_LAI[2] == 0))
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_lastmccmnc_present_byte] 
                |= CMCC_CS_MOCallRecord_lastmccmnc_present;
            pDestBill->u.moCallRecord.lastmccmnc.length = 3;
            memcpy(pDestBill->u.moCallRecord.lastmccmnc.value, pMocBill->caller_changeOflocation_LAI, 3);
        }
    }

    if (GET_CDR_FIELD_FILTER(cUGOutgoingAccessIndicator, MOCallRecord))
	{
		if (pMocBill->CUGOutgoingAccessIndicator != 0)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_cUGOutgoingAccessIndicator_present_byte] |=
                CMCC_CS_MOCallRecord_cUGOutgoingAccessIndicator_present;
            pDestBill->u.moCallRecord.cUGOutgoingAccessIndicator = CMCC_CS_cUGCall;
        }
	}

    if (GET_CDR_FIELD_FILTER(cUGInterlockCode, MOCallRecord))
	{
		if (pMocBill->CUGInterlockCode != 0xFFFFFFFF)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_cUGInterlockCode_present_byte] |=
                CMCC_CS_MOCallRecord_cUGInterlockCode_present;
            pDestBill->u.moCallRecord.cUGInterlockCode.length = 4;
            memcpy(pDestBill->u.moCallRecord.cUGInterlockCode.value, &(pMocBill->CUGInterlockCode), 4);
        }
	}

    if (GET_CDR_FIELD_FILTER(cUGOutgoingAccessUsed, MOCallRecord)) //此域无法得到
	{
        if(pMocBill->CUGOutgoingAccessUsed ==1 || pMocBill->CUGOutgoingAccessUsed == 0)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_cUGOutgoingAccessUsed_present_byte] |=
                CMCC_CS_cUGOutgoingAccessUsed_present;
            pDestBill->u.moCallRecord.cUGOutgoingAccessUsed = 
                (CMCC_CS_CUGOutgoingAccessUsed)pMocBill->CUGOutgoingAccessUsed;
        }
	}

    if (GET_CDR_FIELD_FILTER(hotBillingTag, MOCallRecord))		//热计费标识
	{
		if (pMocBill->HotBillingTag == HOT_BILLING)
		{
		    pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_hotBillingTag_present_byte] |=
                CMCC_CS_MOCallRecord_hotBillingTag_present;
            pDestBill->u.moCallRecord.hotBillingTag = CMCC_CS_hotBilling;
		}
		else
		{
			pDestBill->u.moCallRecord.hotBillingTag = CMCC_CS_noHotBilling;
		}
	}

    if (GET_CDR_FIELD_FILTER(partialRecordType, MOCallRecord))     //1
    {
        if(pMocBill->cause_for_partial_record != 0xFF)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_partialRecordType_present_byte] |=
                CMCC_CS_MOCallRecord_partialRecordType_present;
            pDestBill->u.moCallRecord.partialRecordType = (CMCC_CS_PartialRecordType)pMocBill->cause_for_partial_record;
        }
    }

    if (GET_CDR_FIELD_FILTER(guaranteedBitRate, MOCallRecord))      //2
    {
        if(pMocBill->GuaranteedBitRate > 0 && pMocBill->GuaranteedBitRate < 8 )
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_guaranteedBitRate_present_byte] |=
                CMCC_CS_MOCallRecord_guaranteedBitRate_present;
            pDestBill->u.moCallRecord.guaranteedBitRate = (CMCC_CS_GuaranteedBitRate)pMocBill->GuaranteedBitRate;
        }
    }
 
    if (GET_CDR_FIELD_FILTER(maximumBitRate, MOCallRecord))      //3
    {
        if (pMocBill->MaximumBitRate > 0 && pMocBill->MaximumBitRate < 7)
        {       
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_maximumBitRate_present_byte] |=
                CMCC_CS_MOCallRecord_maximumBitRate_present;
            pDestBill->u.moCallRecord.maximumBitRate = (CMCC_CS_MaximumBitRate)pMocBill->MaximumBitRate;
        }
    }

    if (GET_CDR_FIELD_FILTER(modemType, MOCallRecord))      //4
    {
        if(pMocBill->ModemType != 0)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_modemType_present_byte] |=
                CMCC_CS_MOCallRecord_modemType_present;
            pDestBill->u.moCallRecord.modemType = (CMCC_CS_ModemType)pMocBill->ModemType;
        }
    }

    pDestBill->u.moCallRecord.classmark3.length = 0;
    if (GET_CDR_FIELD_FILTER(classmark3, MOCallRecord))      //5
    {
        if (pMocBill->Classmark3Indication)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_classmark3_present_byte] |=
                CMCC_CS_MOCallRecord_classmark3_present;
            pDestBill->u.moCallRecord.classmark3.length = 2;
            memcpy(pDestBill->u.moCallRecord.classmark3.value,&(pMocBill->Classmark3Information),2);    
        }
    }

    if (GET_CDR_FIELD_FILTER(optimalRoutingFlag, MOCallRecord))      //6
    {
        if (pMocBill->SORIndication)
        {
           pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_optimalRoutingFlag_present_byte] |=
               CMCC_CS_MOCallRecord_optimalRoutingFlag_present;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(optimalRoutingLateForwardFlag, MOCallRecord))      //7
    {
        if (pMocBill->LateForwardSORIndication)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_optimalRoutingLateForwardFlag_present_byte] |=
                CMCC_CS_optimalRoutingLateForwardFlag_present;
        }
    }

    if (GET_CDR_FIELD_FILTER(optimalRoutingEarlyForwardFlag, MOCallRecord))      //8
    {
        if (pMocBill->EarlyForwardSORIndication)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_optimalRoutingEarlyForwardFlag_present_byte] |=
                CMCC_CS_optimalRoutingEarlyForwardFlag_present;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(globalAreaID, MOCallRecord))      //9
    {
        if (IsLACandCellIDValid(&(pMocBill->caller_location_LAI[3]),pMocBill->caller_location_cellID))
        {                       
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_globalAreaID_present_byte] |=
                CMCC_CS_MOCallRecord_globalAreaID_present;
            pDestBill->u.moCallRecord.globalAreaID.length = 7;
            memcpy(pDestBill->u.moCallRecord.globalAreaID.value,pMocBill->caller_location_LAI,5);
            memcpy(pDestBill->u.moCallRecord.globalAreaID.value + 5,pMocBill->caller_location_cellID,2);
        }
     }

    pDestBill->u.moCallRecord.changeOfglobalAreaID = NULL;
    if (GET_CDR_FIELD_FILTER(changeOfGlobalAreaID, MOCallRecord))      //10
    {
        if (pEventBill != NULL && nEventNum != 0 && pMocBill->HO_invoked_times != 0)
        {
            SS_Action_Bill_Report* pSingleEventBill = NULL;
            
            for(int i = nEventNum - 1; i >= 0; i--) //由于后面的链表是插入方式，所以这里从后往前找
            {
                pSingleEventBill = (SS_Action_Bill_Report*)((MSC_Bill_Report*)pEventBill + i);
                
                if (pSingleEventBill->event_type == HO)
                {
                    CMCC_CS__seqof6_* pGloble = new (g_pAsnBuffForCMCC)CMCC_CS__seqof6_;
                    ConvertTimeStamp(pGloble->value.changeTime, &(pSingleEventBill->ssActionTime_year));
                    
                    pGloble->value.location.length = 7;
                    memcpy(pGloble->value.location.value, pSingleEventBill->served_location_LAI, 5);
                    memcpy(pGloble->value.location.value + 5, pSingleEventBill->served_location_cellID,2);
                    
                    pGloble->next = pDestBill->u.moCallRecord.changeOfglobalAreaID;
                    pDestBill->u.moCallRecord.changeOfglobalAreaID = pGloble;
                }
            }
        }
        //如果不出事件话单但是当前位置区小区有效并且跟初始值不相同，那么还要填当前值
        else if (IsLACandCellIDValid(&(pMocBill->caller_changeOflocation_LAI[3]), pMocBill->caller_changeOflocation_cellID)
            && memcmp(pMocBill->caller_changeOflocation_LAI, pMocBill->caller_location_LAI, 2) != 0)
        {
            CMCC_CS__seqof6_* pGloble = new (g_pAsnBuffForCMCC)CMCC_CS__seqof6_;
            ConvertTimeStamp(pGloble->value.changeTime, &(pMocBill->end_year));  //时间取结束时间
            
            pGloble->value.location.length = 7;
            memcpy(pGloble->value.location.value, pMocBill->caller_changeOflocation_LAI, 5);
            memcpy(pGloble->value.location.value + 5, pMocBill->caller_changeOflocation_cellID,2);
            
            pGloble->next = pDestBill->u.moCallRecord.changeOfglobalAreaID;
            pDestBill->u.moCallRecord.changeOfglobalAreaID = pGloble;
        }
        
        if (pDestBill->u.moCallRecord.changeOfglobalAreaID != NULL)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_changeOfglobalAreaID_present_byte] |=
                CMCC_CS_MOCallRecord_changeOfglobalAreaID_present;
        }
    }

    if (GET_CDR_FIELD_FILTER(interactionWithIP, MOCallRecord))      //11
    {
        if (pMocBill ->PromptAndCollectCharge)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_interactionWithIP_present_byte] |=
                CMCC_CS_interactionWithIP_present;
        }
    }

    if (GET_CDR_FIELD_FILTER(chargedParty,MOCallRecord) )
    {
        if (pMocBill->chargedParty == 0 || pMocBill->chargedParty == 1)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_chargedParty_present_byte] |= 
                CMCC_CS_MOCallRecord_chargedParty_present;
            pDestBill->u.moCallRecord.chargedParty = (CMCC_CS_ChargedParty)pMocBill->chargedParty;
        }
    }
    
    pDestBill->u.moCallRecord.originalCalledNumber.length = 0;
    pDestBill->u.moCallRecord.originalCalledNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(originalCalledNumber,MOCallRecord))
    {
        int nOriginalCalledLen = (pMocBill->originalCalledNumber_BCDlen + 1)/2;
        if (nOriginalCalledLen > 0 )
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_originalCalledNumber_present_byte] |=
                CMCC_CS_MOCallRecord_originalCalledNumber_present;
            pDestBill->u.moCallRecord.originalCalledNumber.length = nOriginalCalledLen + 1;
            pDestBill->u.moCallRecord.originalCalledNumber.value = new (g_pAsnBuffForCMCC)BYTE[nOriginalCalledLen + 1];
            pDestBill->u.moCallRecord.originalCalledNumber.value[0] = *(pMocBill->originalCalledNumber - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.moCallRecord.originalCalledNumber.value + 1,pMocBill->originalCalledNumber,
                pDestBill->u.moCallRecord.originalCalledNumber.length - 1);
        }
    }
    
    pDestBill->u.moCallRecord.callingChargeAreaCode.length = 0;
    if (GET_CDR_FIELD_FILTER(callingChargeAreaCode,MOCallRecord))
    {
        if (pMocBill->callingChargeAreaCode[0] != 0xff &&
            pMocBill->callingChargeAreaCode[1] != 0xff &&
            pMocBill->callingChargeAreaCode[2] != 0xff)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_callingChargeAreaCode_present_byte] |=
                CMCC_CS_MOCallRecord_callingChargeAreaCode_present;
            pDestBill->u.moCallRecord.callingChargeAreaCode.length = 3;
            memcpy(pDestBill->u.moCallRecord.callingChargeAreaCode.value,pMocBill->callingChargeAreaCode,3);
        }
    }

    pDestBill->u.moCallRecord.calledChargeAreaCode.length = 0;
    if (GET_CDR_FIELD_FILTER(calledChargeAreaCode,MOCallRecord))
    {
        if (pMocBill->calledChargeAreaCode[0] != 0xff &&
            pMocBill->calledChargeAreaCode[1] != 0xff &&
            pMocBill->calledChargeAreaCode[2] != 0xff)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_calledChargeAreaCode_present_byte] |=
                CMCC_CS_MOCallRecord_calledChargeAreaCode_present;
            pDestBill->u.moCallRecord.calledChargeAreaCode.length = 3;
            memcpy(pDestBill->u.moCallRecord.calledChargeAreaCode.value,pMocBill->calledChargeAreaCode,3);
        }
    }
    
    pDestBill->u.moCallRecord.selectedCIC.length = 0;
    if (GET_CDR_FIELD_FILTER(selectedCIC,MOCallRecord))
    {
        if (pMocBill->selectedCIC[0] != 0xff &&
            pMocBill->selectedCIC[1] != 0xff &&
            pMocBill->selectedCIC[2] != 0xff)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_selectedCIC_present_byte] |= 
                CMCC_CS_MOCallRecord_selectedCIC_present; 
            pDestBill->u.moCallRecord.selectedCIC.length = 3;
            memcpy(pDestBill->u.moCallRecord.selectedCIC.value,pMocBill->selectedCIC,3);
        }
    }

    if (GET_CDR_FIELD_FILTER(portedflag,MOCallRecord))
    {
        if(pMocBill->portedflag)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_portedflag_present_byte] |=
                CMCC_CS_MOCallRecord_portedflag_present;
            pDestBill->u.moCallRecord.portedflag = (CMCC_CS_PortedFlag)pMocBill->portedflag;
        }
    }

    pDestBill->u.moCallRecord.cUGIndex.length = 0;
    if (GET_CDR_FIELD_FILTER(cUGIndex,MOCallRecord))
    {
        if (pMocBill->CUGIndex != 0xFFFF)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_MOCallRecord_cUGIndex_present_byte] |= 
                CMCC_CS_MOCallRecord_cUGIndex_present;
            pDestBill->u.moCallRecord.cUGIndex.length = 2;
            memcpy(pDestBill->u.moCallRecord.cUGIndex.value, &(pMocBill->CUGIndex), 2);
        }
    }
    
    pDestBill->u.moCallRecord.calledIMSI.length = 0;
    if(GET_CDR_FIELD_FILTER(calledIMSI,MOCallRecord))
    {
        pDestBill->u.moCallRecord.calledIMSI.length =
            BCDtoRBCD(pDestBill->u.moCallRecord.calledIMSI.value, pMocBill->calledIMSI, 8);
        if (pDestBill->u.moCallRecord.calledIMSI.length > 0)
        {
            pDestBill->u.moCallRecord.bit_mask[CMCC_CS_calledIMSI_present_byte] |=
                CMCC_CS_calledIMSI_present;        
        }
    }
    
    return TRUE;
}

BOOL ConvertABill_CFW(const BYTE* pSrcBill, const int nLen, 
                      const BYTE* pEventBill, const int nEventNum, 
                      CMCC_CS_CallEventRecord* pDestBill)
{
	MOC_Bill_Report* pCfwBill = (MOC_Bill_Report *)pSrcBill;
	
	pDestBill->choice = CMCC_CS_forwardCallRecord_chosen;
	memset(pDestBill->u.forwardCallRecord.bit_mask, 0, sizeof(pDestBill->u.forwardCallRecord.bit_mask));

    if (GET_CDR_FIELD_FILTER(recordType, ForwardCallRecord))
    {
        pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_recordType_present_byte]
            |= CMCC_CS_MOCallRecord_recordType_present;
	    pDestBill->u.forwardCallRecord.recordType = CMCC_CS_CallEventRecordType_forwardCallRecord;		//记录类型
    }

	pDestBill->u.forwardCallRecord.servedIMSI.length = 0;
    if (GET_CDR_FIELD_FILTER(servedIMSI, ForwardCallRecord))		//servedIMSI
	{
        pDestBill->u.forwardCallRecord.servedIMSI.length = 
            BCDtoRBCD(pDestBill->u.forwardCallRecord.servedIMSI.value, pCfwBill->servedIMSI, 8);
        if (pDestBill->u.forwardCallRecord.servedIMSI.length > 0)
        {
		    pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_servedIMSI_present_byte] |=
                CMCC_CS_MOCallRecord_servedIMSI_present;        
        }
	}

    pDestBill->u.forwardCallRecord.servedIMEI.length = 0;
	if (GET_CDR_FIELD_FILTER(servedIMEI, ForwardCallRecord))		//servedIMEI
	{
		pDestBill->u.forwardCallRecord.servedIMEI.length = 
            BCDtoRBCD(pDestBill->u.forwardCallRecord.servedIMEI.value, pCfwBill->servedIMEI, 8);
        if (pDestBill->u.forwardCallRecord.servedIMEI.length > 0)
        {
		    pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_servedIMEI_present_byte] |=
                CMCC_CS_MOCallRecord_servedIMEI_present;        
        }
	}

	pDestBill->u.forwardCallRecord.servedMSISDN.length = 0;
    if (GET_CDR_FIELD_FILTER(servedMSISDN, ForwardCallRecord))		//servedMSISDN
	{
	    int nMSISDNLen = (pCfwBill->servedMSISDN_BCDlen + 1) / 2;
        if (nMSISDNLen > 0)
        {
		    pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_servedMSISDN_present_byte] |=
                CMCC_CS_MOCallRecord_servedMSISDN_present; 
            pDestBill->u.forwardCallRecord.servedMSISDN.length = 
                min(sizeof(pDestBill->u.forwardCallRecord.servedMSISDN.value), nMSISDNLen + 1);
            pDestBill->u.forwardCallRecord.servedMSISDN.value[0] = *(pCfwBill->servedMSISDN - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.forwardCallRecord.servedMSISDN.value + 1, pCfwBill->servedMSISDN, 
                pDestBill->u.forwardCallRecord.servedMSISDN.length - 1);
        }
	}

	pDestBill->u.forwardCallRecord.callingNumber.length = 0;
    pDestBill->u.forwardCallRecord.callingNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(callingNumber, ForwardCallRecord))		//主叫号码
	{
		int nCallerLen = (pCfwBill->callingNumber_BCDlen + 1) / 2;
        if (nCallerLen > 0)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_callingNumber_present_byte] |=
                CMCC_CS_MOCallRecord_callingNumber_present;
		    pDestBill->u.forwardCallRecord.callingNumber.length = nCallerLen + 1;
		    pDestBill->u.forwardCallRecord.callingNumber.value = new (g_pAsnBuffForCMCC)BYTE[nCallerLen + 1];
            pDestBill->u.forwardCallRecord.callingNumber.value[0] = *(pCfwBill->callingNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.forwardCallRecord.callingNumber.value + 1, pCfwBill->callingNumber, nCallerLen);
        }
	}

	pDestBill->u.forwardCallRecord.calledNumber.length = 0;
    pDestBill->u.forwardCallRecord.calledNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(calledNumber, ForwardCallRecord))		//被叫号码
	{
		int nCalledLen = (pCfwBill->calledNumber_BCDlen + 1) / 2;
        if (nCalledLen > 0)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_calledNumber_present_byte] |=
                CMCC_CS_MOCallRecord_calledNumber_present;
		    pDestBill->u.forwardCallRecord.calledNumber.length = nCalledLen + 1;
		    pDestBill->u.forwardCallRecord.calledNumber.value = new (g_pAsnBuffForCMCC)BYTE[nCalledLen + 1];
            pDestBill->u.forwardCallRecord.calledNumber.value[0] = *(pCfwBill->calledNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.forwardCallRecord.calledNumber.value + 1, pCfwBill->calledNumber, nCalledLen);
	    }
    }

	pDestBill->u.forwardCallRecord.translatedNumber.length = 0;
    pDestBill->u.forwardCallRecord.translatedNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(translatedNumber, ForwardCallRecord))		//Translated Number
	{
		int nTranslatedLen = (pCfwBill->translatedNumberTYPElen + 1) / 2;
        if (nTranslatedLen > 0)
        {
		    pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_translatedNumber_present_byte] |=
                CMCC_CS_translatedNumber_present;  
		    pDestBill->u.forwardCallRecord.translatedNumber.length = nTranslatedLen + 1;
		    pDestBill->u.forwardCallRecord.translatedNumber.value = new (g_pAsnBuffForCMCC)BYTE[nTranslatedLen + 1];
            pDestBill->u.forwardCallRecord.translatedNumber.value[0] = *(pCfwBill->translatedNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.forwardCallRecord.translatedNumber.value + 1, pCfwBill->translatedNumber, nTranslatedLen);
        }
	}

	pDestBill->u.forwardCallRecord.connectedNumber.length = 0;
    pDestBill->u.forwardCallRecord.connectedNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(connectedNumber, ForwardCallRecord))		//连接号码
	{
		int nConnectedLen = (pCfwBill->connectedNumber_BCDlen + 1) / 2;
        if (nConnectedLen > 0)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_connectedNumber_present_byte] |=
                CMCC_CS_MOCallRecord_connectedNumber_present;
		    pDestBill->u.forwardCallRecord.connectedNumber.length = nConnectedLen + 1;
		    pDestBill->u.forwardCallRecord.connectedNumber.value = new (g_pAsnBuffForCMCC)BYTE[nConnectedLen + 1];
            pDestBill->u.forwardCallRecord.connectedNumber.value[0] = *(pCfwBill->connectedNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.forwardCallRecord.connectedNumber.value + 1, pCfwBill->connectedNumber, nConnectedLen);
        }
	}

    pDestBill->u.forwardCallRecord.roamingNumber.length = 0;
    if (GET_CDR_FIELD_FILTER(roamingNumber, ForwardCallRecord))		//漫游号码
	{
		int nRoamingLen = (pCfwBill->roamingNumber_BCDlen + 1) / 2;
        if (nRoamingLen > 0)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_roamingNumber_present_byte] |=
                CMCC_CS_MOCallRecord_roamingNumber_present;
		    pDestBill->u.forwardCallRecord.roamingNumber.length = 
                min(sizeof(pDestBill->u.forwardCallRecord.roamingNumber.value), nRoamingLen + 1);
            pDestBill->u.forwardCallRecord.roamingNumber.value[0] = *(pCfwBill->roamingNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.forwardCallRecord.roamingNumber.value + 1, pCfwBill->roamingNumber, 
                pDestBill->u.forwardCallRecord.roamingNumber.length - 1);
        }
	}

    pDestBill->u.forwardCallRecord.recordingEntity.length = 0;
    if (GET_CDR_FIELD_FILTER(recordingEntity, ForwardCallRecord))
    {
        int nLocalMSCLen = (pCfwBill->recordingEntity_BCDlen + 1) / 2;
        if (nLocalMSCLen > 0)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_recordingEntity_present_byte] |=
                CMCC_CS_MOCallRecord_recordingEntity_present; 
            pDestBill->u.forwardCallRecord.recordingEntity.length = 
                min(sizeof(pDestBill->u.forwardCallRecord.recordingEntity.value), nLocalMSCLen + 1);
            pDestBill->u.forwardCallRecord.recordingEntity.value[0] = *(pCfwBill->recordingEntity - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.forwardCallRecord.recordingEntity.value + 1, pCfwBill->recordingEntity, 
                pDestBill->u.forwardCallRecord.recordingEntity.length - 1);
        }
    }
    
    pDestBill->u.forwardCallRecord.mscIncomingROUTE.u.rOUTEName = NULL;
	if (GET_CDR_FIELD_FILTER(mscIncomingROUTE, ForwardCallRecord))		//入中继
	{
        if (pCfwBill->mscIncomingROUTE_ind == SHOW_ROUTE_NO)
        {
            if (pCfwBill->mscIncomingROUTE_ROUTENumber != 0xFFFF)
            {
                pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_mscIncomingROUTE_present_byte] |= 
                    CMCC_CS_MOCallRecord_mscIncomingROUTE_present;
                pDestBill->u.forwardCallRecord.mscIncomingROUTE.choice = CMCC_CS_rOUTENumber_chosen;
                pDestBill->u.forwardCallRecord.mscIncomingROUTE.u.rOUTENumber = pCfwBill->mscIncomingROUTE_ROUTENumber;
            }
        }
        else if (pCfwBill->mscIncomingROUTE_ind == SHOW_ROUTE_NAME)
        {
            if (*(pCfwBill->mscIncomingROUTE_ROUTEName) != 0x20)
            {
                pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_mscIncomingROUTE_present_byte] |= 
                    CMCC_CS_MOCallRecord_mscIncomingROUTE_present;
                pDestBill->u.forwardCallRecord.mscIncomingROUTE.choice = CMCC_CS_rOUTEName_chosen;
                pDestBill->u.forwardCallRecord.mscIncomingROUTE.u.rOUTEName = new (g_pAsnBuffForCMCC)char[11];
                memcpy(pDestBill->u.forwardCallRecord.mscIncomingROUTE.u.rOUTEName, pCfwBill->mscIncomingROUTE_ROUTEName, 10);
                pDestBill->u.forwardCallRecord.mscIncomingROUTE.u.rOUTEName[10] = '\0';
            }
        }
	}
	
    pDestBill->u.forwardCallRecord.mscOutgoingROUTE.u.rOUTEName = NULL;
	if (GET_CDR_FIELD_FILTER(mscOutgoingROUTE, ForwardCallRecord))		//出中继
	{
        if (pCfwBill->mscOutgoingROUT_ind == SHOW_ROUTE_NO)
        {
            if (pCfwBill->mscOutgoingROUTE_ROUTENumber != 0xFFFF) 
            {
                pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_mscOutgoingROUTE_present_byte] |=
                    CMCC_CS_MOCallRecord_mscOutgoingROUTE_present;
                pDestBill->u.forwardCallRecord.mscOutgoingROUTE.choice = CMCC_CS_rOUTENumber_chosen;
                pDestBill->u.forwardCallRecord.mscOutgoingROUTE.u.rOUTENumber = pCfwBill->mscOutgoingROUTE_ROUTENumber;
            }
        }
        else if (pCfwBill->mscOutgoingROUT_ind == SHOW_ROUTE_NAME)
        {
            if (*(pCfwBill->mscOutgoingROUTE_ROUTEName) != 0x20)
            {
                pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_mscOutgoingROUTE_present_byte] |=
                    CMCC_CS_MOCallRecord_mscOutgoingROUTE_present;
                pDestBill->u.forwardCallRecord.mscOutgoingROUTE.choice = CMCC_CS_rOUTEName_chosen;
                pDestBill->u.forwardCallRecord.mscOutgoingROUTE.u.rOUTEName = new (g_pAsnBuffForCMCC)char[11];
                memcpy(pDestBill->u.forwardCallRecord.mscOutgoingROUTE.u.rOUTEName, pCfwBill->mscOutgoingROUTE_ROUTEName, 10);
                pDestBill->u.forwardCallRecord.mscOutgoingROUTE.u.rOUTEName[10] = '\0';
            }
        }
	}

    pDestBill->u.forwardCallRecord.location.locationAreaCode.length = 0;
    pDestBill->u.forwardCallRecord.location.cellIdentifier.length = 0;
	if (GET_CDR_FIELD_FILTER(location, ForwardCallRecord))			//初始位置区小区
	{
        if (IsLACandCellIDValid(&(pCfwBill->caller_location_LAI[3]), pCfwBill->caller_location_cellID))
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_location_present_byte] |= 
                CMCC_CS_MOCallRecord_location_present;
		    pDestBill->u.forwardCallRecord.location.locationAreaCode.length = 2;
            memcpy(pDestBill->u.forwardCallRecord.location.locationAreaCode.value, pCfwBill->caller_location_LAI + 3, 2);
            pDestBill->u.forwardCallRecord.location.cellIdentifier.length = 2;
            memcpy(pDestBill->u.forwardCallRecord.location.cellIdentifier.value, pCfwBill->caller_location_cellID,2);
        }
	}

	pDestBill->u.forwardCallRecord.changeOfLocation = NULL;
    if (GET_CDR_FIELD_FILTER(changeOfLocation, ForwardCallRecord))			//changeOfLocation
	{
        if (pEventBill != NULL && nEventNum != 0 && pCfwBill->HO_invoked_times != 0)
        {
            SS_Action_Bill_Report* pSingleEventBill = NULL;

            for(int i = nEventNum - 1; i >= 0; i--) //由于后面的链表是插入方式，所以这里从后往前找
            {
                pSingleEventBill = (SS_Action_Bill_Report*)((MSC_Bill_Report*)pEventBill + i);

                if (pSingleEventBill->event_type == HO)
                {
                    CMCC_CS__seqof1_* pLocation = new (g_pAsnBuffForCMCC)CMCC_CS__seqof1_;
                    ConvertTimeStamp(pLocation->value.changeTime, &(pSingleEventBill->ssActionTime_year));
		            
                    pLocation->value.location.locationAreaCode.length = 2;
                    memcpy(pLocation->value.location.locationAreaCode.value, pSingleEventBill->served_location_LAI + 3, 2);
                    pLocation->value.location.cellIdentifier.length = 2;
                    memcpy(pLocation->value.location.cellIdentifier.value, pSingleEventBill->served_location_cellID,2);

                    pLocation->next = pDestBill->u.forwardCallRecord.changeOfLocation;
                    pDestBill->u.forwardCallRecord.changeOfLocation = pLocation;
                }
            }
        }
        //如果不出事件话单但是当前位置区小区有效并且跟初始值不相同，那么还要填当前值
        else if (pCfwBill->cause_for_partial_record == CMCC_CS_locationChange)
        {
            CMCC_CS__seqof1_* pLocation = new (g_pAsnBuffForCMCC)CMCC_CS__seqof1_;
            ConvertTimeStamp(pLocation->value.changeTime, &(pCfwBill->end_year));  //时间取结束时间
		    
            pLocation->value.location.locationAreaCode.length = 2;
            memcpy(pLocation->value.location.locationAreaCode.value, pCfwBill->caller_changeOflocation_LAI + 3, 2);
            pLocation->value.location.cellIdentifier.length = 2;
            memcpy(pLocation->value.location.cellIdentifier.value, pCfwBill->caller_changeOflocation_cellID,2);

            pLocation->next = pDestBill->u.forwardCallRecord.changeOfLocation;
            pDestBill->u.forwardCallRecord.changeOfLocation = pLocation;
        }

        if (pDestBill->u.forwardCallRecord.changeOfLocation != NULL)
        {
		    pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_changeOfLocation_present_byte] |= 
                CMCC_CS_MOCallRecord_changeOfLocation_present;
        }
	}

	if (GET_CDR_FIELD_FILTER(basicService, ForwardCallRecord))			//电话或承载业务码
	{
        if (pCfwBill->TBS < 2)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_basicService_present_byte] |=
                CMCC_CS_MOCallRecord_basicService_present;
            ConvertBasicServiceCode(pDestBill->u.forwardCallRecord.basicService, 
                pCfwBill->TBS, pCfwBill->ServiceCode);
        }
	}

	if (GET_CDR_FIELD_FILTER(transparencyIndicator, ForwardCallRecord))	//transparencyIndicator
	{
        if (pCfwBill->transparencyIndicator < 2)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_transparencyIndicator_present_byte] |=
                CMCC_CS_MOCallRecord_transparencyIndicator_present;
            pDestBill->u.forwardCallRecord.transparencyIndicator = (CMCC_CS_TransparencyInd)pCfwBill->transparencyIndicator;
        }
	}

	pDestBill->u.forwardCallRecord.changeOfService = NULL;

	pDestBill->u.forwardCallRecord.supplServicesUsed = NULL;
    if (GET_CDR_FIELD_FILTER(supplServicesUsed, ForwardCallRecord))		//supplServicesUsed
	{
        int nSSCount = 0;
        
        nSSCount = pCfwBill->CH_invoked_times + pCfwBill->CH_retrieve_times +
            pCfwBill->CW_invoked_times + pCfwBill->MPTY_Build_times + pCfwBill->MPTY_CH_times
            + pCfwBill->MPTY_CH_retrieve_times + pCfwBill->MPTY_Split_times;
        
        if (pEventBill != NULL && nEventNum != 0 && nSSCount != 0)
        {
            SS_Action_Bill_Report* pSingleEventBill = NULL;
            
            for(int i = nEventNum - 1; i >= 0; i--)
            {
                pSingleEventBill = (SS_Action_Bill_Report*)((MSC_Bill_Report*)pEventBill + i);
                
                CMCC_CS__seqof3_* pSS = NULL;
                switch (pSingleEventBill->event_type)
                {
                case Hold_Call:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CHOLD;
                    break;
                case Retrieve_Call:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CHOLD;
                    break;
                case CallWait:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CWAIT;
                    break;
                case Build_MPTY:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CMPTY;
                    break;
                case Hold_MPTY:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CMPTY;
                    break;
                case Retrieve_MPTY:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CMPTY;
                    break;
                case Split_MPTY:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CMPTY;
                    break;
                default:
                    break;
                }
                
                if (pSS != NULL)
                {
                    pSS->value.bit_mask = 0;
                    pSS->value.bit_mask |= CMCC_CS_ssTime_present;
                    pSS->value.bit_mask |= CMCC_CS_ssCode_present;
                    ConvertTimeStamp(pSS->value.ssTime, &(pSingleEventBill->ssActionTime_year));
                    pSS->next = pDestBill->u.forwardCallRecord.supplServicesUsed;
                    pDestBill->u.forwardCallRecord.supplServicesUsed = pSS;
                }
            }
        }
        ConvertSScode((BYTE*)(&(pCfwBill->SS_code1)), &(pCfwBill->initial_year), &(pDestBill->u.forwardCallRecord.supplServicesUsed));

        if (pDestBill->u.forwardCallRecord.supplServicesUsed != NULL)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_supplServicesUsed_present_byte] |= 
                CMCC_CS_MOCallRecord_supplServicesUsed_present;
        }
	}

	if (GET_CDR_FIELD_FILTER(aocParameters, ForwardCallRecord))		//aocParameters
	{
        pDestBill->u.forwardCallRecord.aocParameters.bit_mask = 0;

        AOC_E1_E7* pAoc = NULL;
        pAoc = &(pCfwBill->aocParameters);
        if(pAoc->aoc != 0xFF)
        {
            pDestBill->u.forwardCallRecord.aocParameters.bit_mask |= CMCC_CS_e1_present;
            pDestBill->u.forwardCallRecord.aocParameters.e1 = pAoc->e[0];

            pDestBill->u.forwardCallRecord.aocParameters.bit_mask |= CMCC_CS_e2_present;
            pDestBill->u.forwardCallRecord.aocParameters.e2 = pAoc->e[1];
            
            pDestBill->u.forwardCallRecord.aocParameters.bit_mask |= CMCC_CS_e3_present;
            pDestBill->u.forwardCallRecord.aocParameters.e3 = pAoc->e[2];

            pDestBill->u.forwardCallRecord.aocParameters.bit_mask |= CMCC_CS_e4_present;
            pDestBill->u.forwardCallRecord.aocParameters.e4 = pAoc->e[3];

            pDestBill->u.forwardCallRecord.aocParameters.bit_mask |= CMCC_CS_e5_present;
            pDestBill->u.forwardCallRecord.aocParameters.e5 = pAoc->e[4];

            pDestBill->u.forwardCallRecord.aocParameters.bit_mask |= CMCC_CS_e6_present;
            pDestBill->u.forwardCallRecord.aocParameters.e6 = pAoc->e[5];
            
            pDestBill->u.forwardCallRecord.aocParameters.bit_mask |= CMCC_CS_e7_present;
            pDestBill->u.forwardCallRecord.aocParameters.e7 = pAoc->e[6];

            if (pDestBill->u.forwardCallRecord.aocParameters.bit_mask != 0)
            {
                pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_aocParameters_present_byte] |=
                    CMCC_CS_MOCallRecord_aocParameters_present;    
            }
        }
    }

    pDestBill->u.forwardCallRecord.changeOfAOCParms = NULL;

    pDestBill->u.forwardCallRecord.msClassmark.length = 0;
    pDestBill->u.forwardCallRecord.msClassmark.value = NULL;
    if (GET_CDR_FIELD_FILTER(msClassmark, ForwardCallRecord))		//msClassmark
	{
		//if (pCfwBill->BillType != CFW)  //CFW话单不要这个字段
        if(!(pCfwBill->MSClassmark[0] == 0
            && pCfwBill->MSClassmark[1] == 0
            && pCfwBill->MSClassmark[2] == 0)
            && !(pCfwBill->MSClassmark[0] == 0xFF
            && pCfwBill->MSClassmark[1] == 0xFF
            && pCfwBill->MSClassmark[2] == 0xFF))
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_msClassmark_present_byte] |=
                CMCC_CS_MOCallRecord_msClassmark_present;
		    pDestBill->u.forwardCallRecord.msClassmark.length = 3;
		    pDestBill->u.forwardCallRecord.msClassmark.value = new (g_pAsnBuffForCMCC)BYTE[3];
		    memcpy(pDestBill->u.forwardCallRecord.msClassmark.value, pCfwBill->MSClassmark, 3);
        }
	}

    pDestBill->u.forwardCallRecord.changeOfClassmark.classmark.length = 0;
    pDestBill->u.forwardCallRecord.changeOfClassmark.classmark.value = NULL;

    if (GET_CDR_FIELD_FILTER(seizureTime, ForwardCallRecord))		//应答时间
	{
        if (pCfwBill->causeForTerm == unsuccessfulCallAttempt)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_seizureTime_present_byte] |=
                CMCC_CS_MOCallRecord_seizureTime_present;
            ConvertTimeStamp(pDestBill->u.forwardCallRecord.seizureTime, &(pCfwBill->initial_year));
        }
    }

    if (GET_CDR_FIELD_FILTER(answerTime, ForwardCallRecord))		//应答时间
	{
        if (pCfwBill->causeForTerm != unsuccessfulCallAttempt)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_answerTime_present_byte] |=
                CMCC_CS_MOCallRecord_answerTime_present;
		    ConvertTimeStamp(pDestBill->u.forwardCallRecord.answerTime, &(pCfwBill->initial_year));
        }
	}

    if (GET_CDR_FIELD_FILTER(releaseTime, ForwardCallRecord))		//释放时间
	{
		if (pCfwBill->end_year != 0xFF)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_releaseTime_present_byte] |=
                CMCC_CS_MOCallRecord_releaseTime_present;
                CMCC_CS_MOCallRecord_answerTime_present;
		    ConvertTimeStamp(pDestBill->u.forwardCallRecord.releaseTime, &(pCfwBill->end_year));
        }
	}

    if (GET_CDR_FIELD_FILTER(callDuration, ForwardCallRecord))
    {
        pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_callDuration_present_byte] |=
            CMCC_CS_MOCallRecord_callDuration_present; 
        pDestBill->u.forwardCallRecord.callDuration = pCfwBill->callDuration;
    }

    if (GET_CDR_FIELD_FILTER(radioChanRequested, ForwardCallRecord))		
    {
        if (pCfwBill->radioChanRequested  > 0 && pCfwBill->radioChanRequested < 4)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_radioChanRequested_present_byte] |=
                CMCC_CS_MOCallRecord_radioChanRequested_present;
            pDestBill->u.forwardCallRecord.radioChanRequested = (CMCC_CS_RadioChanRequested)pCfwBill->radioChanRequested;
        }
    }

    if (GET_CDR_FIELD_FILTER(radioChanUsed, ForwardCallRecord))		//radioChanUsed
	{
            if (pCfwBill->TBS == MBILL_TELECOM_SERVICE && (0 == pCfwBill->radioChanUsed || 1 == pCfwBill->radioChanUsed))
            {
                pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_radioChanUsed_present_byte] |=
                    CMCC_CS_MOCallRecord_radioChanUsed_present;
                if (pCfwBill->radioChanUsed == 1)  //half
                {
                    pDestBill->u.forwardCallRecord.radioChanUsed = CMCC_CS_halfRate;
                }
                else
                {
                    pDestBill->u.forwardCallRecord.radioChanUsed = CMCC_CS_fullRate;
                }
            }
	}

    if (GET_CDR_FIELD_FILTER(causeForTerm, ForwardCallRecord))
    {
        pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_causeForTerm_present_byte] |=
            CMCC_CS_MOCallRecord_causeForTerm_present; 
        pDestBill->u.forwardCallRecord.causeForTerm = pCfwBill->causeForTerm;
    }

    if (GET_CDR_FIELD_FILTER(diagnostics, ForwardCallRecord))		//diagnostics
	{
        pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_diagnostics_present_byte] |=
            CMCC_CS_MOCallRecord_diagnostics_present;
		if (pCfwBill->diagnostics.CauseType == 0)
		{
			pDestBill->u.forwardCallRecord.diagnostics.choice = CMCC_CS_gsm0408Cause_chosen;
			pDestBill->u.forwardCallRecord.diagnostics.u.gsm0408Cause = pCfwBill->diagnostics.Cause.gsm0408Cause;
		}
		else if (pCfwBill->diagnostics.CauseType == 1)
		{
			pDestBill->u.forwardCallRecord.diagnostics.choice = CMCC_CS_gsm0902MapErrorValue_chosen;
			pDestBill->u.forwardCallRecord.diagnostics.u.gsm0902MapErrorValue = pCfwBill->diagnostics.Cause.gsm0902MapErrorValue;
		}
		else if (pCfwBill->diagnostics.CauseType == 2)
		{
			pDestBill->u.forwardCallRecord.diagnostics.choice = CMCC_CS_ccittQ767Cause_chosen;
			pDestBill->u.forwardCallRecord.diagnostics.u.ccittQ767Cause = pCfwBill->diagnostics.Cause.ccittQ767Cause;
		}
		else
		{
			pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_diagnostics_present_byte] &=
                ~CMCC_CS_MOCallRecord_diagnostics_present;
		}
	}

    if (GET_CDR_FIELD_FILTER(callReference, ForwardCallRecord))
    {
        if (pCfwBill->call_reference_len != 0)
        {
            int nCallReferenceLen = min(pCfwBill->call_reference_len,
                sizeof(pDestBill->u.forwardCallRecord.callReference.value));
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_callReference_present_byte] |=
                CMCC_CS_MOCallRecord_callReference_present; 
            pDestBill->u.forwardCallRecord.callReference.length = nCallReferenceLen;
            memcpy(pDestBill->u.forwardCallRecord.callReference.value, 
                &(pCfwBill->call_reference[sizeof(pCfwBill->call_reference) - pCfwBill->call_reference_len]), nCallReferenceLen);   
        }
        else
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_callReference_present_byte] |=
                CMCC_CS_MOCallRecord_callReference_present; 
            GetCallReference(pDestBill->u.forwardCallRecord.callReference,
                pCfwBill->ModuleNum, pCfwBill->count);
        }
    }

    if (GET_CDR_FIELD_FILTER(sequenceNumber, ForwardCallRecord))		//中间话单流水号
	{
		if (pCfwBill->record_type != SINGLE_BILL)
		{
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_sequenceNumber_present_byte] |=
                CMCC_CS_MOCallRecord_sequenceNumber_present;
		    pDestBill->u.forwardCallRecord.sequenceNumber = pCfwBill->sequence;
		}
	}

    if (GET_CDR_FIELD_FILTER(additionalChgInfo, ForwardCallRecord))		//附加计费信息
	{
		if (pCfwBill->chargeIndicator >= 0 && pCfwBill->chargeIndicator < 3)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_additionalChgInfo_present_byte] |=
                CMCC_CS_MOCallRecord_additionalChgInfo_present;
            pDestBill->u.forwardCallRecord.additionalChgInfo.bit_mask = CMCC_CS_chargeIndicator_present;
		    pDestBill->u.forwardCallRecord.additionalChgInfo.chargeIndicator = pCfwBill->chargeIndicator;
        }
	}

    pDestBill->u.forwardCallRecord.recordExtensions = NULL;

    if (GET_CDR_FIELD_FILTER(gsm_SCFAddress, ForwardCallRecord))		//gsm_SCFAddress
	{
	    int nGSMSCFAddressLen = (pCfwBill->CAMEL_GsmSCF_Address_BCDlen + 1) / 2;
        if (nGSMSCFAddressLen > 0)
        {
		    pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_gsm_SCFAddress_present_byte] |=
                CMCC_CS_MOCallRecord_gsm_SCFAddress_present;
            pDestBill->u.forwardCallRecord.gsm_SCFAddress.length = 
                min(sizeof(pDestBill->u.forwardCallRecord.gsm_SCFAddress.value), nGSMSCFAddressLen + 1);
            pDestBill->u.forwardCallRecord.gsm_SCFAddress.value[0] = *(pCfwBill->CAMEL_GsmSCF_Address - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.forwardCallRecord.gsm_SCFAddress.value + 1, pCfwBill->CAMEL_GsmSCF_Address, 
                pDestBill->u.forwardCallRecord.gsm_SCFAddress.length - 1);
        }
	}

    if (GET_CDR_FIELD_FILTER(serviceKey, ForwardCallRecord))		//业务键
	{
        if (pCfwBill->ServiceKey != 0xFFFFFFFF)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_serviceKey_present_byte] |=
                CMCC_CS_MOCallRecord_serviceKey_present;
		    pDestBill->u.forwardCallRecord.serviceKey = pCfwBill->ServiceKey;
        }
	}

    if (GET_CDR_FIELD_FILTER(networkCallReference, ForwardCallRecord))		//网络呼叫参考号
	{
        if (pCfwBill->NetworkCallReferenceLen != 0)
        {
            int NetworkCallReferenceLen = min(pCfwBill->NetworkCallReferenceLen, 
                sizeof(pDestBill->u.forwardCallRecord.networkCallReference.value));
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_networkCallReference_present_byte] |=
                CMCC_CS_MOCallRecord_networkCallReference_present;
		    pDestBill->u.forwardCallRecord.networkCallReference.length = NetworkCallReferenceLen;
    	    memcpy(pDestBill->u.forwardCallRecord.networkCallReference.value, 
                &(pCfwBill->NetworkcallReferenceNumber[sizeof(pCfwBill->NetworkcallReferenceNumber) - pCfwBill->NetworkCallReferenceLen]), NetworkCallReferenceLen);
        }
	}

    if (GET_CDR_FIELD_FILTER(mSCAddress, ForwardCallRecord))		//MSC 地址
	{
        int nMSCAddressLen = (pCfwBill->MSCAddress_BCDlen + 1) / 2;
        if (nMSCAddressLen > 0)
        {
		    pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_mSCAddress_present_byte] |=
                CMCC_CS_MOCallRecord_mSCAddress_present; 
            pDestBill->u.forwardCallRecord.mSCAddress.length = 
                min(sizeof(pDestBill->u.forwardCallRecord.mSCAddress.value), nMSCAddressLen + 1);
            pDestBill->u.forwardCallRecord.mSCAddress.value[0] = *(pCfwBill->MSCAddress - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.forwardCallRecord.mSCAddress.value + 1, pCfwBill->MSCAddress, 
                pDestBill->u.forwardCallRecord.mSCAddress.length - 1);
        }
	}

	//by 杨善	2004-5-17	对应问题单SWPD05184
    if (GET_CDR_FIELD_FILTER(cAMELInitCFIndicator, ForwardCallRecord))
	{
        if(pCfwBill->CAMELInitCFIndicator)
        {
		    pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_cAMELInitCFIndicator_present_byte] |=
                CMCC_CS_MOCallRecord_cAMELInitCFIndicator_present;
            pDestBill->u.forwardCallRecord.cAMELInitCFIndicator = CMCC_CS_cAMELCallForwarding;
        }
    }
	//the end	SWPD05184

    if (GET_CDR_FIELD_FILTER(defaultCallHandling, ForwardCallRecord))		//defaultCallHandling
	{
		if (pCfwBill->defaultCallHandling == 0 || pCfwBill->defaultCallHandling == 1)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_defaultCallHandling_present_byte] |=
                CMCC_CS_MOCallRecord_defaultCallHandling_present;
		    if (pCfwBill->defaultCallHandling == 0)
            {
                pDestBill->u.forwardCallRecord.defaultCallHandling = CMCC_CS_continueCall;
            }
            else
            {
                pDestBill->u.forwardCallRecord.defaultCallHandling = CMCC_CS_releaseCall;
            }
        }
	}

    if (GET_CDR_FIELD_FILTER(fnur, ForwardCallRecord))		//fnur
	{
        if (pCfwBill->fnur != 0x00)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_fnur_present_byte] |=
                CMCC_CS_MOCallRecord_fnur_present;
            pDestBill->u.forwardCallRecord.fnur = (CMCC_CS_Fnur)pCfwBill->fnur;
        }
	}

    if (GET_CDR_FIELD_FILTER(aiurRequested, ForwardCallRecord))		//aiurRequested
	{
        if(pCfwBill->aiurRequested != 0)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_aiurRequested_present_byte] |=
                CMCC_CS_MOCallRecord_aiurRequested_present;
            pDestBill->u.forwardCallRecord.aiurRequested = (CMCC_CS_AiurRequested)pCfwBill->aiurRequested;
        }
	}


    if (GET_CDR_FIELD_FILTER(speechVersionSupported, ForwardCallRecord))		//speechVersionSupported
	{
		if (pCfwBill->TBS == MBILL_TELECOM_SERVICE && pCfwBill->speechVersionSupported != 0xFF)
        {
            ConvertSpeechVersion(pDestBill->u.forwardCallRecord.speechVersionSupported.value[0],
                pCfwBill->speechVersionSupported);
            if(pDestBill->u.forwardCallRecord.speechVersionSupported.value[0] != 0xFF)
            {
                pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_speechVersionSupported_present_byte] |=
                    CMCC_CS_MOCallRecord_speechVersionSupported_present;
                pDestBill->u.forwardCallRecord.speechVersionSupported.length = 1;
            }
        }
	}

    if (GET_CDR_FIELD_FILTER(speechVersionUsed, ForwardCallRecord))		//speechVersionUsed
	{
        if (pCfwBill->TBS == MBILL_TELECOM_SERVICE && pCfwBill->speechVersionUsed != 0xFF)
        {
            ConvertSpeechVersion(pDestBill->u.forwardCallRecord.speechVersionUsed.value[0],
                pCfwBill->speechVersionUsed);
            if(pDestBill->u.forwardCallRecord.speechVersionUsed.value[0] != 0xFF)
            {
                pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_speechVersionUsed_present_byte] |=
                    CMCC_CS_MOCallRecord_speechVersionUsed_present;
                pDestBill->u.forwardCallRecord.speechVersionUsed.length = 1;
            }
        }
	}

    if (GET_CDR_FIELD_FILTER(numberOfDPEncountered, ForwardCallRecord))		//numberOfDPEncountered
	{
		if (pCfwBill->numberOfDPEncountered != 0)  //pCfwBill->UserType＝2为智能呼叫，现在不需要判断了
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_numberOfDPEncountered_present_byte] |=
                CMCC_CS_MOCallRecord_numberOfDPEncountered_present;
		    pDestBill->u.forwardCallRecord.numberOfDPEncountered = pCfwBill->numberOfDPEncountered;
        }
	}

    if (GET_CDR_FIELD_FILTER(levelOfCAMELService, ForwardCallRecord))		//numberOfDPEncountered
	{
        if (pCfwBill->levelOfCAMELService != 0xFF)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_levelOfCAMELService_present_byte] |=
                CMCC_CS_MOCallRecord_levelOfCAMELService_present;
            switch(pCfwBill->levelOfCAMELService)
            {
            case 0:
                pDestBill->u.forwardCallRecord.levelOfCAMELService = CMCC_CS_basic;
                break;
            case 1:
                pDestBill->u.forwardCallRecord.levelOfCAMELService = CMCC_CS_callDurationSupervision;
                break;
            case 2:
                pDestBill->u.forwardCallRecord.levelOfCAMELService = CMCC_CS_onlineCharging;
                break;
            default:
                pDestBill->u.forwardCallRecord.levelOfCAMELService = CMCC_CS_basic;
                break;
            }
        }
	}

    if (GET_CDR_FIELD_FILTER(freeFormatData, ForwardCallRecord))		//自由格式数据
	{
        if (pCfwBill->fcidata_len != 0)
        {
            int fci_len = min(sizeof(pDestBill->u.forwardCallRecord.freeFormatData.value), pCfwBill->fcidata_len);
            memcpy(pDestBill->u.forwardCallRecord.freeFormatData.value, pCfwBill->VarPart, fci_len);
		    pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_freeFormatData_present_byte] |=
                CMCC_CS_MOCallRecord_freeFormatData_present;
            pDestBill->u.forwardCallRecord.freeFormatData.length = fci_len;
        }
	}

    pDestBill->u.forwardCallRecord.cAMELCallLegInformation = NULL;

    if (GET_CDR_FIELD_FILTER(freeFormatDataAppend, ForwardCallRecord))		//freeFormatDataAppend
	{
        if ((AppendIndication)pCfwBill->freeFormatDataAppend == Append)
        {
		    pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_freeFormatDataAppend_present_byte] |=
                CMCC_CS_MOCallRecord_freeFormatDataAppend_present;
            pDestBill->u.forwardCallRecord.freeFormatDataAppend = TRUE;
        }
        else
        {
		    pDestBill->u.forwardCallRecord.freeFormatDataAppend = FALSE;     
        }
	}

    if (GET_CDR_FIELD_FILTER(defaultCallHandling_2, ForwardCallRecord))		//defaultCallHandling2
	{
        if (pCfwBill->defaultCallHandling2 == 0 || pCfwBill->defaultCallHandling2 == 1)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_defaultCallHandling_2_present_byte] |=
                CMCC_CS_MOCallRecord_defaultCallHandling_2_present;
		    if (pCfwBill->defaultCallHandling2 == 0)
            {
                pDestBill->u.forwardCallRecord.defaultCallHandling_2 = CMCC_CS_continueCall;
            }
            else
            {
                pDestBill->u.forwardCallRecord.defaultCallHandling_2 = CMCC_CS_releaseCall;
            }
        }
	}

    if (GET_CDR_FIELD_FILTER(gsm_SCFAddress_2, ForwardCallRecord))		//gsm_SCFAddress2
	{
	    int nGSMSCFAddress2Len = (pCfwBill->CAMEL_GsmSCF_Address2_BCDlen + 1) / 2;
        if (nGSMSCFAddress2Len > 0)
        {
		    pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_gsm_SCFAddress_2_present_byte] |=
                CMCC_CS_MOCallRecord_gsm_SCFAddress_2_present;
            pDestBill->u.forwardCallRecord.gsm_SCFAddress_2.length = 
                min(sizeof(pDestBill->u.forwardCallRecord.gsm_SCFAddress_2.value), nGSMSCFAddress2Len + 1);
            pDestBill->u.forwardCallRecord.gsm_SCFAddress_2.value[0] = *(pCfwBill->CAMEL_GsmSCF_Address2 - 2) | 0x80, 
            BCDtoRBCD(pDestBill->u.forwardCallRecord.gsm_SCFAddress_2.value + 1, pCfwBill->CAMEL_GsmSCF_Address2, 
                pDestBill->u.forwardCallRecord.gsm_SCFAddress_2.length - 1);
        }
	}

    if (GET_CDR_FIELD_FILTER(serviceKey_2, ForwardCallRecord))		//业务键2
	{
		if (pCfwBill->ServiceKey2 != 0xFFFFFFFF)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_serviceKey_2_present_byte] |=
                CMCC_CS_MOCallRecord_serviceKey_2_present;
		    pDestBill->u.forwardCallRecord.serviceKey_2 = pCfwBill->ServiceKey2;
        }
	}

    pDestBill->u.forwardCallRecord.freeFormatData_2.length = 0;
    if (GET_CDR_FIELD_FILTER(freeFormatData_2, ForwardCallRecord))		//自由格式数据
	{
        if (pCfwBill->fcidata2_len != 0)
        {
            int fci2_len = min(sizeof(pDestBill->u.forwardCallRecord.freeFormatData_2.value), pCfwBill->fcidata2_len);
            memcpy(pDestBill->u.forwardCallRecord.freeFormatData_2.value + pCfwBill->fcidata_len, pCfwBill->VarPart, fci2_len);
		    pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_freeFormatData_2_present_byte] |=
                CMCC_CS_MOCallRecord_freeFormatData_2_present;
            pDestBill->u.forwardCallRecord.freeFormatData_2.length = fci2_len;
        }
	}

    if (GET_CDR_FIELD_FILTER(freeFormatDataAppend_2, ForwardCallRecord))		//freeFormatDataAppend
	{
        if ((AppendIndication)pCfwBill->freeFormatDataAppend2 == Append)
        {
		    pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_freeFormatDataAppend_2_present_byte] |=
                CMCC_CS_MOCallRecord_freeFormatDataAppend_2_present;
            pDestBill->u.forwardCallRecord.freeFormatDataAppend_2 = TRUE;
        }
        else
        {
		    pDestBill->u.forwardCallRecord.freeFormatDataAppend_2 = FALSE;     
        }
	}

    if (GET_CDR_FIELD_FILTER(systemType, ForwardCallRecord))		//系统类型
	{
        switch((ACCESS_NETWORK_TYPE)pCfwBill->systemType)
        {
        case ACCESS_BSC :
            pDestBill->u.forwardCallRecord.systemType = CMCC_CS_gERAN;
		    pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_systemType_present_byte] |=
                CMCC_CS_MOCallRecord_systemType_present;
            break;
        case ACCESS_RNC :
            pDestBill->u.forwardCallRecord.systemType = CMCC_CS_iuUTRAN;
		    pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_systemType_present_byte] |=
                CMCC_CS_MOCallRecord_systemType_present;
            break;
        case ACCESS_UNKNOWN:
            pDestBill->u.forwardCallRecord.systemType = CMCC_CS_unknown;
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_systemType_present_byte] |= 
                CMCC_CS_MOCallRecord_systemType_present;
            break;
        } 
	}

    if (GET_CDR_FIELD_FILTER(rateIndication, ForwardCallRecord))		//rateIndication
	{
        if (pCfwBill->rateIndication != 0x00)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_rateIndication_present_byte] |=
                CMCC_CS_MOCallRecord_rateIndication_present;
            pDestBill->u.forwardCallRecord.rateIndication.length = 1;
            pDestBill->u.forwardCallRecord.rateIndication.value[0] = pCfwBill->rateIndication;
        }
    }   

    if (GET_CDR_FIELD_FILTER(mscOutgoingCircuit, ForwardCallRecord))		//mscOutgoingCircuit
	{
        if (pCfwBill->mscOutgoingCircuit != 0xFFFF)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_mscOutgoingCircuit_present_byte] |=
                CMCC_CS_MOCallRecord_mscOutgoingCircuit_present;
            pDestBill->u.forwardCallRecord.mscOutgoingCircuit = pCfwBill->mscOutgoingCircuit; 
        }
    }
    
    pDestBill->u.forwardCallRecord.orgRNCorBSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgRNCorBSCId, ForwardCallRecord))
	{
        if (pCfwBill->rnc_bsc_id_len != 0)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_orgRNCorBSCId_present_byte] |=
                CMCC_CS_MOCallRecord_orgRNCorBSCId_present;
            pDestBill->u.forwardCallRecord.orgRNCorBSCId.length = min(3,pCfwBill->rnc_bsc_id_len);
            memcpy(pDestBill->u.forwardCallRecord.orgRNCorBSCId.value, pCfwBill->orgRNCorBSCId, pDestBill->u.forwardCallRecord.orgRNCorBSCId.length);
        }
    }

    if (GET_CDR_FIELD_FILTER(orgMSCId, ForwardCallRecord))
	{
        if (pCfwBill->msc_id_len != 0)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_orgMSCId_present_byte] |=
                CMCC_CS_MOCallRecord_orgMSCId_present;
            pDestBill->u.forwardCallRecord.orgMSCId.length = min(3,pCfwBill->msc_id_len);
            memcpy(pDestBill->u.forwardCallRecord.orgMSCId.value, pCfwBill->orgMSCId, pDestBill->u.forwardCallRecord.orgMSCId.length);
        }
    }

    if (GET_CDR_FIELD_FILTER(callEmlppPriority, ForwardCallRecord))
	{
        if (pCfwBill->callEmlppPriority != 0xF)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_callEmlppPriority_present_byte] |=
                CMCC_CS_MOCallRecord_callEmlppPriority_present;
            pDestBill->u.forwardCallRecord.callEmlppPriority.length = 1;
            pDestBill->u.forwardCallRecord.callEmlppPriority.value[0] = pCfwBill->callEmlppPriority;
        }
    }

    if (GET_CDR_FIELD_FILTER(callerDefaultEmlppPriority, ForwardCallRecord))
	{
        if (pCfwBill->callerDefaultEmlppPriority != 0xF)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_callerDefaultEmlppPriority_present_byte] |=
                CMCC_CS_callerDefaultEmlppPriority_present;
            pDestBill->u.forwardCallRecord.callerDefaultEmlppPriority.length = 1;
            pDestBill->u.forwardCallRecord.callerDefaultEmlppPriority.value[0] = pCfwBill->callerDefaultEmlppPriority;
        }
    }

    if (GET_CDR_FIELD_FILTER(subscriberCategory, ForwardCallRecord))
	{
        pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_subscriberCategory_present_byte] |=
            CMCC_CS_MOCallRecord_subscriberCategory_present;
        pDestBill->u.forwardCallRecord.subscriberCategory.length = 1;
        pDestBill->u.forwardCallRecord.subscriberCategory.value[0] = pCfwBill->subscriberCategory;
    }

    pDestBill->u.forwardCallRecord.firstmccmnc.length = 0;
    if (GET_CDR_FIELD_FILTER(firstmccmnc, ForwardCallRecord))
    {
        if (GET_CDR_FIELD_FILTER(firstmccmnc, ForwardCallRecord))
	    {
            if (!(pCfwBill->caller_location_LAI[0] == 0xFF
                && pCfwBill->caller_location_LAI[1] == 0xFF
                && pCfwBill->caller_location_LAI[2] == 0xFF)
                && !(pCfwBill->caller_location_LAI[0] == 0
                && pCfwBill->caller_location_LAI[1] == 0
                && pCfwBill->caller_location_LAI[2] == 0))
            {
                pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_firstmccmnc_present_byte] 
                    |= CMCC_CS_MOCallRecord_firstmccmnc_present;
                pDestBill->u.forwardCallRecord.firstmccmnc.length = 3;
                memcpy(pDestBill->u.forwardCallRecord.firstmccmnc.value, pCfwBill->caller_location_LAI, 3);
            }
        }
    }

    pDestBill->u.forwardCallRecord.intermediatemccmnc.length = 0;
    if (GET_CDR_FIELD_FILTER(intermediatemccmnc, ForwardCallRecord))
	{
    }

    pDestBill->u.forwardCallRecord.lastmccmnc.length = 0;
    if (GET_CDR_FIELD_FILTER(lastmccmnc, ForwardCallRecord))
	{
        if (!(pCfwBill->caller_changeOflocation_LAI[0] == 0xFF
            && pCfwBill->caller_changeOflocation_LAI[1] == 0xFF
            && pCfwBill->caller_changeOflocation_LAI[2] == 0xFF)
            && !(pCfwBill->caller_changeOflocation_LAI[0] == 0
            && pCfwBill->caller_changeOflocation_LAI[1] == 0
            && pCfwBill->caller_changeOflocation_LAI[2] == 0))
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_lastmccmnc_present_byte] 
                |= CMCC_CS_MOCallRecord_lastmccmnc_present;
            pDestBill->u.forwardCallRecord.lastmccmnc.length = 3;
            memcpy(pDestBill->u.forwardCallRecord.lastmccmnc.value, pCfwBill->caller_changeOflocation_LAI, 3);
        }
    }

    if (GET_CDR_FIELD_FILTER(cUGOutgoingAccessIndicator, ForwardCallRecord))
	{
		if (pCfwBill->CUGOutgoingAccessIndicator != 0)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_cUGOutgoingAccessIndicator_present_byte] |=
                CMCC_CS_MOCallRecord_cUGOutgoingAccessIndicator_present;
            pDestBill->u.forwardCallRecord.cUGOutgoingAccessIndicator = CMCC_CS_cUGCall;
        }
	}

    pDestBill->u.forwardCallRecord.cUGInterlockCode.length = 0;
    if (GET_CDR_FIELD_FILTER(cUGInterlockCode, ForwardCallRecord))
	{
		if (pCfwBill->CUGInterlockCode != 0xFFFFFFFF)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_cUGInterlockCode_present_byte] |=
                CMCC_CS_MOCallRecord_cUGInterlockCode_present;
            pDestBill->u.forwardCallRecord.cUGInterlockCode.length = 4;
            memcpy(pDestBill->u.forwardCallRecord.cUGInterlockCode.value, &(pCfwBill->CUGInterlockCode), 4);
        }
	}

    if (GET_CDR_FIELD_FILTER(cUGOutgoingAccessUsed, ForwardCallRecord)) //此域无法得到
	{
        if(pCfwBill->CUGOutgoingAccessUsed == 1 || pCfwBill->CUGOutgoingAccessUsed == 0)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_cUGOutgoingAccessUsed_present_byte] |=
                CMCC_CS_cUGOutgoingAccessUsed_present;
            pDestBill->u.forwardCallRecord.cUGOutgoingAccessUsed = 
                (CMCC_CS_CUGOutgoingAccessUsed)pCfwBill->CUGOutgoingAccessUsed;
        }
	}

    if (GET_CDR_FIELD_FILTER(hotBillingTag, ForwardCallRecord))		//热计费标识
	{
		if (pCfwBill->HotBillingTag == HOT_BILLING)
		{
		    pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_hotBillingTag_present_byte] |=
                CMCC_CS_MOCallRecord_hotBillingTag_present;
            pDestBill->u.forwardCallRecord.hotBillingTag = CMCC_CS_hotBilling;
		}
		else
		{
			pDestBill->u.forwardCallRecord.hotBillingTag = CMCC_CS_noHotBilling;
		}
	}

    if (GET_CDR_FIELD_FILTER(partialRecordType, ForwardCallRecord))     //1
    {
        if(pCfwBill->cause_for_partial_record != 0xFF)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_partialRecordType_present_byte] |=
                CMCC_CS_MOCallRecord_partialRecordType_present;
            pDestBill->u.forwardCallRecord.partialRecordType = (CMCC_CS_PartialRecordType)pCfwBill->cause_for_partial_record;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(guaranteedBitRate, ForwardCallRecord))      //2
    {
        if (pCfwBill->GuaranteedBitRate > 0 && pCfwBill->GuaranteedBitRate < 8)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_guaranteedBitRate_present_byte] |=
                CMCC_CS_MOCallRecord_guaranteedBitRate_present;
            pDestBill->u.forwardCallRecord.guaranteedBitRate = (CMCC_CS_GuaranteedBitRate)pCfwBill->GuaranteedBitRate;
        }
    }
    if (GET_CDR_FIELD_FILTER(maximumBitRate, ForwardCallRecord))      //3
    {
        if (pCfwBill->MaximumBitRate > 0 && pCfwBill->MaximumBitRate < 7)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_maximumBitRate_present_byte] |=
                CMCC_CS_MOCallRecord_maximumBitRate_present;
            pDestBill->u.forwardCallRecord.maximumBitRate = (CMCC_CS_MaximumBitRate)pCfwBill->MaximumBitRate ;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(modemType, ForwardCallRecord))      //4
    {
        if (pCfwBill->ModemType != 0)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_modemType_present_byte] |=
                CMCC_CS_MOCallRecord_modemType_present;
            pDestBill->u.forwardCallRecord.modemType = (CMCC_CS_ModemType)pCfwBill->ModemType;
        }
    }

    pDestBill->u.forwardCallRecord.classmark3.length =  0;
    if (GET_CDR_FIELD_FILTER(classmark3, ForwardCallRecord))      //5
    {
        if (pCfwBill->Classmark3Indication)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_classmark3_present_byte] |=
                CMCC_CS_MOCallRecord_classmark3_present;
            pDestBill->u.forwardCallRecord.classmark3.length =  2;
            memcpy(pDestBill->u.forwardCallRecord.classmark3.value,&(pCfwBill->Classmark3Information),2);
        }
    }
    if (GET_CDR_FIELD_FILTER(optimalRoutingFlag, ForwardCallRecord))      //6
    {
        if (pCfwBill->SORIndication)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_optimalRoutingFlag_present_byte] |=
                CMCC_CS_MOCallRecord_optimalRoutingFlag_present;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(optimalRoutingLateForwardFlag, ForwardCallRecord))      //7
    {
        if (pCfwBill->LateForwardSORIndication)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_optimalRoutingLateForwardFlag_present_byte] |=
                CMCC_CS_optimalRoutingLateForwardFlag_present;
        }
    }    

    if (GET_CDR_FIELD_FILTER(optimalRoutingEarlyForwardFlag, ForwardCallRecord))      //8
    {
        if (pCfwBill->EarlyForwardSORIndication)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_optimalRoutingEarlyForwardFlag_present_byte] |=
                CMCC_CS_optimalRoutingEarlyForwardFlag_present;
        }
    }
    
    pDestBill->u.forwardCallRecord.globalAreaID.length = 0;
    if (GET_CDR_FIELD_FILTER(globalAreaID, ForwardCallRecord))      //9
    {
        if (IsLACandCellIDValid(&(pCfwBill->caller_location_cellID[3]),pCfwBill->caller_location_cellID))
        {                       
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_globalAreaID_present_byte] |=
                CMCC_CS_MOCallRecord_globalAreaID_present;
            pDestBill->u.forwardCallRecord.globalAreaID.length = 7;
            memcpy(pDestBill->u.forwardCallRecord.globalAreaID.value, pCfwBill->caller_location_LAI, 5);
            memcpy(pDestBill->u.forwardCallRecord.globalAreaID.value + 5, pCfwBill->caller_location_cellID, 2);
        }
    }
    
    pDestBill->u.forwardCallRecord.changeOfglobalAreaID = NULL;
    if (GET_CDR_FIELD_FILTER(changeOfGlobalAreaID, ForwardCallRecord))      //10
    {
        if (pEventBill != NULL && nEventNum != 0 && pCfwBill->HO_invoked_times != 0)
        {
            SS_Action_Bill_Report* pSingleEventBill = NULL;
            
            for(int i = nEventNum - 1; i >= 0; i--) //由于后面的链表是插入方式，所以这里从后往前找
            {
                pSingleEventBill = (SS_Action_Bill_Report *)((MSC_Bill_Report*)pEventBill + i);
                
                if (pSingleEventBill->event_type == HO)
                {
                    CMCC_CS__seqof6_* pGloble = new (g_pAsnBuffForCMCC)CMCC_CS__seqof6_;
                    ConvertTimeStamp(pGloble->value.changeTime, &(pSingleEventBill->ssActionTime_year));
                    
                    pGloble->value.location.length = 7;
                    memcpy(pGloble->value.location.value, pSingleEventBill->served_location_LAI, 5);
                    memcpy(pGloble->value.location.value + 5, pSingleEventBill->served_location_cellID,2);
                    
                    pGloble->next = pDestBill->u.forwardCallRecord.changeOfglobalAreaID;
                    pDestBill->u.forwardCallRecord.changeOfglobalAreaID = pGloble;
                }
            }
        }
        //如果不出事件话单但是当前位置区小区有效并且跟初始值不相同，那么还要填当前值
        else if (IsLACandCellIDValid(&(pCfwBill->caller_changeOflocation_LAI[3]), pCfwBill->caller_changeOflocation_cellID)
            && memcmp(pCfwBill->caller_changeOflocation_LAI, pCfwBill->caller_location_LAI, 2) != 0)
        {
            CMCC_CS__seqof6_* pGloble = new (g_pAsnBuffForCMCC)CMCC_CS__seqof6_;
            ConvertTimeStamp(pGloble->value.changeTime, &(pCfwBill->end_year));  //时间取结束时间
            
            pGloble->value.location.length = 7;
            memcpy(pGloble->value.location.value, pCfwBill->caller_changeOflocation_LAI, 5);
            memcpy(pGloble->value.location.value + 5, pCfwBill->caller_changeOflocation_cellID,2);
            
            pGloble->next = pDestBill->u.forwardCallRecord.changeOfglobalAreaID;
            pDestBill->u.forwardCallRecord.changeOfglobalAreaID = pGloble;
        }
        
        if (pDestBill->u.forwardCallRecord.changeOfglobalAreaID != NULL)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_changeOfglobalAreaID_present_byte] |=
                CMCC_CS_MOCallRecord_changeOfglobalAreaID_present;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(interactionWithIP, ForwardCallRecord))      //11
    {
        if (pCfwBill->PromptAndCollectCharge)
        {
             pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_interactionWithIP_present_byte] |=
                 CMCC_CS_interactionWithIP_present;
        } 
     }

    if (GET_CDR_FIELD_FILTER(chargedParty,ForwardCallRecord) )
    {
        if (pCfwBill->chargedParty == 0 || pCfwBill->chargedParty == 1)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_chargedParty_present_byte] |= 
                CMCC_CS_MOCallRecord_chargedParty_present;
            pDestBill->u.forwardCallRecord.chargedParty = (CMCC_CS_ChargedParty)pCfwBill->chargedParty;
        }
    }
    
    pDestBill->u.forwardCallRecord.originalCalledNumber.length = 0;
    pDestBill->u.forwardCallRecord.originalCalledNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(originalCalledNumber,ForwardCallRecord))
    {
        int nOriginalCalledLen = (pCfwBill->originalCalledNumber_BCDlen + 1)/2;
        if (nOriginalCalledLen > 0 )
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_originalCalledNumber_present_byte] |=
                CMCC_CS_MOCallRecord_originalCalledNumber_present;
            pDestBill->u.forwardCallRecord.originalCalledNumber.length = nOriginalCalledLen + 1;
            pDestBill->u.forwardCallRecord.originalCalledNumber.value = new (g_pAsnBuffForCMCC)BYTE[nOriginalCalledLen + 1];            
            pDestBill->u.forwardCallRecord.originalCalledNumber.value[0] = *(pCfwBill->originalCalledNumber - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.forwardCallRecord.originalCalledNumber.value + 1,pCfwBill->originalCalledNumber,
                pDestBill->u.forwardCallRecord.originalCalledNumber.length - 1);
        }
    }
    
    
    pDestBill->u.forwardCallRecord.callingChargeAreaCode.length = 0;
    if (GET_CDR_FIELD_FILTER(callingChargeAreaCode,ForwardCallRecord))
    {
        if (pCfwBill->callingChargeAreaCode[0] != 0xff &&
            pCfwBill->callingChargeAreaCode[1] != 0xff &&
            pCfwBill->callingChargeAreaCode[2] != 0xff)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_callingChargeAreaCode_present_byte] |=
                CMCC_CS_MOCallRecord_callingChargeAreaCode_present;
            pDestBill->u.forwardCallRecord.callingChargeAreaCode.length = 3;
            memcpy(pDestBill->u.forwardCallRecord.callingChargeAreaCode.value,pCfwBill->callingChargeAreaCode,3);
        }
    }
    
    
    pDestBill->u.forwardCallRecord.calledChargeAreaCode.length = 0;
    if (GET_CDR_FIELD_FILTER(calledChargeAreaCode,ForwardCallRecord))
    {
        if (pCfwBill->calledChargeAreaCode[0] != 0xff &&
            pCfwBill->calledChargeAreaCode[1] != 0xff &&
            pCfwBill->calledChargeAreaCode[2] != 0xff)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_calledChargeAreaCode_present_byte] |=
                CMCC_CS_MOCallRecord_calledChargeAreaCode_present;
            pDestBill->u.forwardCallRecord.calledChargeAreaCode.length = 3;
            memcpy(pDestBill->u.forwardCallRecord.calledChargeAreaCode.value,pCfwBill->calledChargeAreaCode,3);
        }
    }
    
    pDestBill->u.forwardCallRecord.selectedCIC.length = 0;
    if (GET_CDR_FIELD_FILTER(selectedCIC,ForwardCallRecord))
    {
        if (pCfwBill->selectedCIC[0] != 0xff &&
            pCfwBill->selectedCIC[1] != 0xff &&
            pCfwBill->selectedCIC[2] != 0xff)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_selectedCIC_present_byte] |= 
                CMCC_CS_MOCallRecord_selectedCIC_present; 
            pDestBill->u.forwardCallRecord.selectedCIC.length = 3;
            memcpy(pDestBill->u.forwardCallRecord.selectedCIC.value,pCfwBill->selectedCIC,3);
        }
    }

    if (GET_CDR_FIELD_FILTER(portedflag,ForwardCallRecord))
    {
        if (pCfwBill->portedflag)
        {      
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_portedflag_present_byte] |=
                CMCC_CS_MOCallRecord_portedflag_present;
            pDestBill->u.forwardCallRecord.portedflag = (CMCC_CS_PortedFlag)pCfwBill->portedflag;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(cUGIndex,ForwardCallRecord))
    {
        if (pCfwBill->CUGIndex != 0xFFFF)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_MOCallRecord_cUGIndex_present_byte] |= 
                CMCC_CS_MOCallRecord_cUGIndex_present;
            pDestBill->u.forwardCallRecord.cUGIndex.length = 2;
            memcpy(pDestBill->u.forwardCallRecord.cUGIndex.value,&(pCfwBill->CUGIndex),2);
        }
    }

    pDestBill->u.forwardCallRecord.calledIMSI.length = 0;
    if(GET_CDR_FIELD_FILTER(calledIMSI,ForwardCallRecord))
    {
        pDestBill->u.forwardCallRecord.calledIMSI.length =
            BCDtoRBCD(pDestBill->u.forwardCallRecord.calledIMSI.value, pCfwBill->calledIMSI, 8);
        if (pDestBill->u.forwardCallRecord.calledIMSI.length > 0)
        {
            pDestBill->u.forwardCallRecord.bit_mask[CMCC_CS_calledIMSI_present_byte] |=
                CMCC_CS_calledIMSI_present;        
        }
    }
    
    return TRUE;
}

BOOL ConvertABill_MTC(const BYTE* pSrcBill, const int nLen, 
                      const BYTE* pEventBill, const int nEventNum,                       
                      CMCC_CS_CallEventRecord* pDestBill)
{
	MTC_Bill_Report* pMtcBill = (MTC_Bill_Report *)pSrcBill;
	
	pDestBill->choice = CMCC_CS_mtCallRecord_chosen;
	memset(pDestBill->u.mtCallRecord.bit_mask, 0, sizeof(pDestBill->u.mtCallRecord.bit_mask));

    if (GET_CDR_FIELD_FILTER(recordType, MTCallRecord))
    {
        pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_recordType_present_byte]
            |= CMCC_CS_MTCallRecord_recordType_present;
	    pDestBill->u.mtCallRecord.recordType = CMCC_CS_CallEventRecordType_mtCallRecord;		//记录类型
    }

    if (GET_CDR_FIELD_FILTER(servedIMSI, MTCallRecord))
    {
        pDestBill->u.mtCallRecord.servedIMSI.length = 
            BCDtoRBCD(pDestBill->u.mtCallRecord.servedIMSI.value, pMtcBill->servedIMSI, 8);
        if(pDestBill->u.mtCallRecord.servedIMSI.length > 0)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_servedIMSI_present_byte] |=
                CMCC_CS_MTCallRecord_servedIMSI_present;                
        }
    }
    
	if (GET_CDR_FIELD_FILTER(servedIMEI, MTCallRecord))		//servedIMEI
	{
		pDestBill->u.mtCallRecord.servedIMEI.length = 
            BCDtoRBCD(pDestBill->u.mtCallRecord.servedIMEI.value, pMtcBill->servedIMEI, 8);
        if (pDestBill->u.mtCallRecord.servedIMEI.length > 0)
        {
		    pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_servedIMEI_present_byte] |=
                CMCC_CS_MTCallRecord_servedIMEI_present;        
        }
	}

    pDestBill->u.mtCallRecord.servedMSISDN.length = 0;
    pDestBill->u.mtCallRecord.servedMSISDN.value = NULL;
	if (GET_CDR_FIELD_FILTER(servedMSISDN, MTCallRecord))		//servedMSISDN
	{
		int nMSISDNLen = (pMtcBill->servedMSISDN_BCDlen + 1) / 2;
        if (nMSISDNLen > 0)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_servedMSISDN_present_byte] |=
                CMCC_CS_MTCallRecord_servedMSISDN_present;
		    pDestBill->u.mtCallRecord.servedMSISDN.length = nMSISDNLen + 1;
		    pDestBill->u.mtCallRecord.servedMSISDN.value = new (g_pAsnBuffForCMCC)BYTE[nMSISDNLen + 1];
            pDestBill->u.mtCallRecord.servedMSISDN.value[0] = *(pMtcBill->servedMSISDN - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.mtCallRecord.servedMSISDN.value + 1, pMtcBill->servedMSISDN, nMSISDNLen);
        }
	}

	pDestBill->u.mtCallRecord.callingNumber.length = 0;
    pDestBill->u.mtCallRecord.callingNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(callingNumber, MTCallRecord))		//主叫号码
	{
		int nCallerLen = (pMtcBill->callingNumber_BCDlen + 1) / 2;
        if (nCallerLen > 0)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_callingNumber_present_byte] |=
                CMCC_CS_MTCallRecord_callingNumber_present;
		    pDestBill->u.mtCallRecord.callingNumber.length = nCallerLen + 1;
		    pDestBill->u.mtCallRecord.callingNumber.value = new (g_pAsnBuffForCMCC)BYTE[nCallerLen + 1];
            pDestBill->u.mtCallRecord.callingNumber.value[0] = *(pMtcBill->callingNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.mtCallRecord.callingNumber.value + 1, pMtcBill->callingNumber, nCallerLen);
        }
	}

	pDestBill->u.mtCallRecord.connectedNumber.length = 0;
    pDestBill->u.mtCallRecord.connectedNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(connectedNumber, MTCallRecord))		//连接号码
	{
		int nConnectedLen = (pMtcBill->connectedNumber_BCDlen + 1) / 2;
        if (nConnectedLen > 0)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_connectedNumber_present_byte] |=
                CMCC_CS_MTCallRecord_connectedNumber_present;
		    pDestBill->u.mtCallRecord.connectedNumber.length = nConnectedLen + 1;
		    pDestBill->u.mtCallRecord.connectedNumber.value = new (g_pAsnBuffForCMCC)BYTE[nConnectedLen + 1];
            pDestBill->u.mtCallRecord.connectedNumber.value[0] = *(pMtcBill->connectedNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.mtCallRecord.connectedNumber.value + 1, pMtcBill->connectedNumber, nConnectedLen);
        }
	}

	if (GET_CDR_FIELD_FILTER(recordingEntity, MTCallRecord))		//MSC地址
	{
		pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_recordingEntity_present_byte] |= 
            CMCC_CS_MTCallRecord_recordingEntity_present;
        pDestBill->u.mtCallRecord.recordingEntity.length = 0;
	    int nLocalMSCLen = (pMtcBill->recordingEntity_BCDlen + 1) / 2;
        if (nLocalMSCLen > 0)
        {
		    pDestBill->u.mtCallRecord.recordingEntity.length = 
                min(sizeof(pDestBill->u.mtCallRecord.recordingEntity.value), nLocalMSCLen + 1);
            pDestBill->u.mtCallRecord.recordingEntity.value[0] = *(pMtcBill->recordingEntity - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.mtCallRecord.recordingEntity.value + 1, pMtcBill->recordingEntity, 
                pDestBill->u.mtCallRecord.recordingEntity.length - 1);
        }
	}

    pDestBill->u.mtCallRecord.mscIncomingROUTE.u.rOUTEName = NULL;
	if (GET_CDR_FIELD_FILTER(mscIncomingROUTE, MTCallRecord))		//入中继
	{
        if (pMtcBill->mscIncomingROUTE_ind == SHOW_ROUTE_NO)
        {
            if (pMtcBill->mscIncomingROUTE_ROUTENumber != 0xFFFF)
            {
                pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_mscIncomingROUTE_present_byte] |= 
                    CMCC_CS_MTCallRecord_mscIncomingROUTE_present;
                pDestBill->u.mtCallRecord.mscIncomingROUTE.choice = CMCC_CS_rOUTENumber_chosen;
                pDestBill->u.mtCallRecord.mscIncomingROUTE.u.rOUTENumber = pMtcBill->mscIncomingROUTE_ROUTENumber;
            }
        }
        else if (pMtcBill->mscIncomingROUTE_ind == SHOW_ROUTE_NAME)
        {
            if (*(pMtcBill->mscIncomingROUTE_ROUTEName) != 0x20)
            {
                pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_mscIncomingROUTE_present_byte] |= 
                    CMCC_CS_MTCallRecord_mscIncomingROUTE_present;
                pDestBill->u.mtCallRecord.mscIncomingROUTE.choice = CMCC_CS_rOUTEName_chosen;
                pDestBill->u.mtCallRecord.mscIncomingROUTE.u.rOUTEName = new (g_pAsnBuffForCMCC)char[11];
                memcpy(pDestBill->u.mtCallRecord.mscIncomingROUTE.u.rOUTEName, pMtcBill->mscIncomingROUTE_ROUTEName, 10);
                pDestBill->u.mtCallRecord.mscIncomingROUTE.u.rOUTEName[10] = '\0';
            }
        }
	}
	
    pDestBill->u.mtCallRecord.mscOutgoingROUTE.u.rOUTEName = NULL;
	if (GET_CDR_FIELD_FILTER(mscOutgoingROUTE, MTCallRecord))		//出中继
	{
        if (pMtcBill->mscOutgoingROUTE_ind == SHOW_ROUTE_NO)
        {
            if (pMtcBill->mscOutgoingROUTE_ROUTENumber != 0xFFFF)
            {
                pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_mscOutgoingROUTE_present_byte] |=
                    CMCC_CS_MTCallRecord_mscOutgoingROUTE_present;
                pDestBill->u.mtCallRecord.mscOutgoingROUTE.choice = CMCC_CS_rOUTENumber_chosen;
                pDestBill->u.mtCallRecord.mscOutgoingROUTE.u.rOUTENumber = pMtcBill->mscOutgoingROUTE_ROUTENumber;
            }
        }
        else if (pMtcBill->mscOutgoingROUTE_ind == SHOW_ROUTE_NAME)
        {
            if (*(pMtcBill->mscOutgoingROUTE_ROUTEName) != 0x20)
            {
                pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_mscOutgoingROUTE_present_byte] |=
                    CMCC_CS_MTCallRecord_mscOutgoingROUTE_present;
                pDestBill->u.mtCallRecord.mscOutgoingROUTE.choice = CMCC_CS_rOUTEName_chosen;
                pDestBill->u.mtCallRecord.mscOutgoingROUTE.u.rOUTEName = new (g_pAsnBuffForCMCC)char[11];
                memcpy(pDestBill->u.mtCallRecord.mscOutgoingROUTE.u.rOUTEName, pMtcBill->mscOutgoingROUTE_ROUTEName, 10);
                pDestBill->u.mtCallRecord.mscOutgoingROUTE.u.rOUTEName[10] = '\0';
            }
        }
	}

	if (GET_CDR_FIELD_FILTER(location, MTCallRecord))			//初始位置区小区
	{
        if (IsLACandCellIDValid(&(pMtcBill->called_location_LAI[3]), pMtcBill->called_location_cellID))
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_location_present_byte] |= 
                CMCC_CS_MTCallRecord_location_present;
		    pDestBill->u.mtCallRecord.location.locationAreaCode.length = 2;
            memcpy(pDestBill->u.mtCallRecord.location.locationAreaCode.value, pMtcBill->called_location_LAI + 3, 2);
            pDestBill->u.mtCallRecord.location.cellIdentifier.length = 2;
            memcpy(pDestBill->u.mtCallRecord.location.cellIdentifier.value, pMtcBill->called_location_cellID,2);
        }
	}

	pDestBill->u.mtCallRecord.changeOfLocation = NULL;
    if (GET_CDR_FIELD_FILTER(changeOfLocation, MTCallRecord))			//changeOfLocation
	{
        if (pEventBill != NULL && nEventNum != 0 && pMtcBill->HO_invoked_times != 0)
        {
            SS_Action_Bill_Report* pSingleEventBill = NULL;

            for(int i = nEventNum - 1; i >= 0; i--) //由于后面的链表是插入方式，所以这里从后往前找
            {
                pSingleEventBill = (SS_Action_Bill_Report*)((MSC_Bill_Report*)pEventBill + i);

                if (pSingleEventBill->event_type == HO)
                {
                    CMCC_CS__seqof1_* pLocation = new (g_pAsnBuffForCMCC)CMCC_CS__seqof1_;
                    ConvertTimeStamp(pLocation->value.changeTime, &(pSingleEventBill->ssActionTime_year));
		            
                    pLocation->value.location.locationAreaCode.length = 2;
                    memcpy(pLocation->value.location.locationAreaCode.value, pSingleEventBill->served_location_LAI + 3, 2);
                    pLocation->value.location.cellIdentifier.length = 2;
                    memcpy(pLocation->value.location.cellIdentifier.value, pSingleEventBill->served_location_cellID,2);

                    pLocation->next = pDestBill->u.mtCallRecord.changeOfLocation;
                    pDestBill->u.mtCallRecord.changeOfLocation = pLocation;
                }
            }
        }
        //如果不出事件话单但是当前位置区小区有效并且跟初始值不相同，那么还要填当前值
        else if (pMtcBill->cause_for_partial_record == CMCC_CS_locationChange)
        {
            CMCC_CS__seqof1_* pLocation = new (g_pAsnBuffForCMCC)CMCC_CS__seqof1_;
            ConvertTimeStamp(pLocation->value.changeTime, &(pMtcBill->end_year));  //时间取结束时间
		    
            pLocation->value.location.locationAreaCode.length = 2;
            memcpy(pLocation->value.location.locationAreaCode.value, pMtcBill->called_changeOflocation_LAI + 3, 2);
            pLocation->value.location.cellIdentifier.length = 2;
            memcpy(pLocation->value.location.cellIdentifier.value, pMtcBill->called_changeOflocation_cellID,2);

            pLocation->next = pDestBill->u.mtCallRecord.changeOfLocation;
            pDestBill->u.mtCallRecord.changeOfLocation = pLocation;
        }

        if (pDestBill->u.mtCallRecord.changeOfLocation != NULL)
        {
		    pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_changeOfLocation_present_byte] |= 
                CMCC_CS_MTCallRecord_changeOfLocation_present;
        }
	}

	if (GET_CDR_FIELD_FILTER(basicService, MTCallRecord))			//电话或承载业务码
	{
        if (pMtcBill->TBS < 2)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_basicService_present_byte] |=
                CMCC_CS_MTCallRecord_basicService_present;
            ConvertBasicServiceCode(pDestBill->u.mtCallRecord.basicService, 
                pMtcBill->TBS, pMtcBill->ServiceCode);
        }
	}

	if (GET_CDR_FIELD_FILTER(transparencyIndicator, MTCallRecord))	//transparencyIndicator
	{
		if (pMtcBill->transparencyIndicator < 2)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_transparencyIndicator_present_byte] |=
                CMCC_CS_MTCallRecord_transparencyIndicator_present;
            pDestBill->u.mtCallRecord.transparencyIndicator = (CMCC_CS_TransparencyInd)pMtcBill->transparencyIndicator;
        }
	}

	pDestBill->u.mtCallRecord.changeOfService = NULL;

    pDestBill->u.mtCallRecord.supplServicesUsed = NULL;
    if (GET_CDR_FIELD_FILTER(supplServicesUsed, MTCallRecord))		//supplServicesUsed
	{
        int nSSCount = 0;
        
        nSSCount = pMtcBill->CH_invoked_times + pMtcBill->CH_retrieve_times +
            pMtcBill->CW_invoked_times + pMtcBill->MPTY_Build_times + pMtcBill->MPTY_CH_times
            + pMtcBill->MPTY_CH_retrieve_times + pMtcBill->MPTY_Split_times;
        
        if (pEventBill != NULL && nEventNum != 0 && nSSCount != 0)
        {
            SS_Action_Bill_Report* pSingleEventBill = NULL;
            
            for(int i = nEventNum - 1; i >= 0; i--)
            {
                pSingleEventBill = (SS_Action_Bill_Report*)((MSC_Bill_Report*)pEventBill + i);
                
                CMCC_CS__seqof3_* pSS = NULL;
                switch (pSingleEventBill->event_type)
                {
                case Hold_Call:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CHOLD;
                    break;
                case Retrieve_Call:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CHOLD;
                    break;
                case CallWait:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CWAIT;
                    break;
                case Build_MPTY:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CMPTY;
                    break;
                case Hold_MPTY:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CMPTY;
                    break;
                case Retrieve_MPTY:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CMPTY;
                    break;
                case Split_MPTY:
                    pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
                    pSS->value.ssCode.length = 1;
                    pSS->value.ssCode.value[0] = SS_CMPTY;
                    break;
                default:
                    break;
                }
                
                if (pSS != NULL)
                {
                    pSS->value.bit_mask = 0;
                    pSS->value.bit_mask |= CMCC_CS_ssTime_present;
                    pSS->value.bit_mask |= CMCC_CS_ssCode_present;
                    ConvertTimeStamp(pSS->value.ssTime, &(pSingleEventBill->ssActionTime_year));
                    pSS->next = pDestBill->u.mtCallRecord.supplServicesUsed;
                    pDestBill->u.mtCallRecord.supplServicesUsed = pSS;
                }
            }
        }

        ConvertSScode((BYTE*)(&(pMtcBill->SS_code1)), &(pMtcBill->initial_year), &(pDestBill->u.mtCallRecord.supplServicesUsed));
 
        if (pDestBill->u.mtCallRecord.supplServicesUsed != NULL)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_supplServicesUsed_present_byte] |= 
                CMCC_CS_MTCallRecord_supplServicesUsed_present;
        }
	}

	if (GET_CDR_FIELD_FILTER(aocParameters, MTCallRecord))		//aocParameters
	{
        pDestBill->u.mtCallRecord.aocParameters.bit_mask = 0;

        AOC_E1_E7* pAoc = NULL;
        pAoc = &(pMtcBill->aocParameters);
        if(pAoc->aoc != 0xFF)
        {
            pDestBill->u.mtCallRecord.aocParameters.bit_mask |= CMCC_CS_e1_present;
            pDestBill->u.mtCallRecord.aocParameters.e1 = pAoc->e[0];

            pDestBill->u.mtCallRecord.aocParameters.bit_mask |= CMCC_CS_e2_present;
            pDestBill->u.mtCallRecord.aocParameters.e2 = pAoc->e[1];

            pDestBill->u.mtCallRecord.aocParameters.bit_mask |= CMCC_CS_e3_present;
            pDestBill->u.mtCallRecord.aocParameters.e3 = pAoc->e[2];

            pDestBill->u.mtCallRecord.aocParameters.bit_mask |= CMCC_CS_e4_present;
            pDestBill->u.mtCallRecord.aocParameters.e4 = pAoc->e[3];

            pDestBill->u.mtCallRecord.aocParameters.bit_mask |= CMCC_CS_e5_present;
            pDestBill->u.mtCallRecord.aocParameters.e5 = pAoc->e[4];

            pDestBill->u.mtCallRecord.aocParameters.bit_mask |= CMCC_CS_e6_present;
            pDestBill->u.mtCallRecord.aocParameters.e6 = pAoc->e[5];
           
            pDestBill->u.mtCallRecord.aocParameters.bit_mask |= CMCC_CS_e7_present;
            pDestBill->u.mtCallRecord.aocParameters.e7 = pAoc->e[6];

            if (pDestBill->u.mtCallRecord.aocParameters.bit_mask != 0)
            {
                pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_aocParameters_present_byte] |=
                    CMCC_CS_MTCallRecord_aocParameters_present;    
            }
        }
    }

    pDestBill->u.mtCallRecord.changeOfAOCParms = NULL;

    pDestBill->u.mtCallRecord.msClassmark.length = 0;
    pDestBill->u.mtCallRecord.msClassmark.value = NULL;
    if (GET_CDR_FIELD_FILTER(msClassmark, MTCallRecord))		//msClassmark
	{
        if(!(pMtcBill->MSClassmark[0] == 0
            && pMtcBill->MSClassmark[1] == 0
            && pMtcBill->MSClassmark[2] == 0)
            && !(pMtcBill->MSClassmark[0] == 0xFF
            && pMtcBill->MSClassmark[1] == 0xFF
            && pMtcBill->MSClassmark[2] == 0xFF))
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_msClassmark_present_byte] |=
                CMCC_CS_MTCallRecord_msClassmark_present;
		    pDestBill->u.mtCallRecord.msClassmark.length = 3;
		    pDestBill->u.mtCallRecord.msClassmark.value = new (g_pAsnBuffForCMCC)BYTE[3];
		    memcpy(pDestBill->u.mtCallRecord.msClassmark.value, pMtcBill->MSClassmark, 3);
        }
    }

    pDestBill->u.mtCallRecord.changeOfClassmark.classmark.length = 0;
    pDestBill->u.mtCallRecord.changeOfClassmark.classmark.value = NULL;

    pDestBill->u.mtCallRecord.seizureTime.length = 0;
    if (GET_CDR_FIELD_FILTER(seizureTime, MTCallRecord))		//应答时间
	{
        if (pMtcBill->causeForTerm == unsuccessfulCallAttempt)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_seizureTime_present_byte] |=
                CMCC_CS_MTCallRecord_seizureTime_present;
            ConvertTimeStamp(pDestBill->u.mtCallRecord.seizureTime, &(pMtcBill->initial_year));
        }
    }

    pDestBill->u.mtCallRecord.answerTime.length = 0;
    if (GET_CDR_FIELD_FILTER(answerTime, MTCallRecord))		//应答时间
	{
        if (pMtcBill->causeForTerm != unsuccessfulCallAttempt)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_answerTime_present_byte] |=
                CMCC_CS_MTCallRecord_answerTime_present;
		    ConvertTimeStamp(pDestBill->u.mtCallRecord.answerTime, &(pMtcBill->initial_year));
        }
	}

    if (GET_CDR_FIELD_FILTER(releaseTime, MTCallRecord))		//释放时间
	{
        if (pMtcBill->end_year != 0xFF)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_releaseTime_present_byte] |=
                CMCC_CS_MTCallRecord_releaseTime_present;
                CMCC_CS_MTCallRecord_answerTime_present;
		    ConvertTimeStamp(pDestBill->u.mtCallRecord.releaseTime, &(pMtcBill->end_year));
        }
	}

    if (GET_CDR_FIELD_FILTER(callDuration, MTCallRecord))
    {
        pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_callDuration_present_byte] |=
            CMCC_CS_MTCallRecord_callDuration_present;
        pDestBill->u.mtCallRecord.callDuration = pMtcBill->callDuration;
    }

    if (GET_CDR_FIELD_FILTER(radioChanRequested, MTCallRecord))		
    {
        if (pMtcBill->radioChanRequested > 0 && pMtcBill->radioChanRequested < 4)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_radioChanRequested_present_byte] |=
                CMCC_CS_MTCallRecord_radioChanRequested_present;
            pDestBill->u.mtCallRecord.radioChanRequested = (CMCC_CS_RadioChanRequested)pMtcBill->radioChanRequested;
        }
    }    

    if (GET_CDR_FIELD_FILTER(radioChanUsed, MTCallRecord))		
	{
        if (pMtcBill->TBS == MBILL_TELECOM_SERVICE && (0 == pMtcBill->radioChanUsed || 1 == pMtcBill->radioChanUsed))
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_radioChanUsed_present_byte] |=
                CMCC_CS_MTCallRecord_radioChanUsed_present;
            if (pMtcBill->radioChanUsed == 1)  //half
            {
                pDestBill->u.mtCallRecord.radioChanUsed = CMCC_CS_halfRate;
            }
            else
            {
                pDestBill->u.mtCallRecord.radioChanUsed = CMCC_CS_fullRate;
            }
        }
	}

    if (GET_CDR_FIELD_FILTER(causeForTerm, MTCallRecord))
    {
        pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_causeForTerm_present_byte] |=
            CMCC_CS_MTCallRecord_causeForTerm_present;
        pDestBill->u.mtCallRecord.causeForTerm = pMtcBill->causeForTerm;
    }

    if (GET_CDR_FIELD_FILTER(diagnostics, MTCallRecord))		//diagnostics
	{
        pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_diagnostics_present_byte] |=
            CMCC_CS_MTCallRecord_diagnostics_present;
		if (pMtcBill->diagnostics.CauseType == 0)
		{
			pDestBill->u.mtCallRecord.diagnostics.choice = CMCC_CS_gsm0408Cause_chosen;
			pDestBill->u.mtCallRecord.diagnostics.u.gsm0408Cause = pMtcBill->diagnostics.Cause.gsm0408Cause;
		}
		else if (pMtcBill->diagnostics.CauseType == 1)
		{
			pDestBill->u.mtCallRecord.diagnostics.choice = CMCC_CS_gsm0902MapErrorValue_chosen;
			pDestBill->u.mtCallRecord.diagnostics.u.gsm0902MapErrorValue = pMtcBill->diagnostics.Cause.gsm0902MapErrorValue;
		}
		else if (pMtcBill->diagnostics.CauseType == 2)
		{
			pDestBill->u.mtCallRecord.diagnostics.choice = CMCC_CS_ccittQ767Cause_chosen;
			pDestBill->u.mtCallRecord.diagnostics.u.ccittQ767Cause = pMtcBill->diagnostics.Cause.ccittQ767Cause;
		}
		else
		{
			pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_diagnostics_present_byte] &=
                ~CMCC_CS_MTCallRecord_diagnostics_present;
		}
	}

    if (GET_CDR_FIELD_FILTER(callReference, MTCallRecord))
    {
        if (pMtcBill->call_reference_len != 0)
        {
            int nCallReferenceLen = min(pMtcBill->call_reference_len,
                sizeof(pDestBill->u.mtCallRecord.callReference.value));
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_callReference_present_byte] |=
                CMCC_CS_MTCallRecord_callReference_present;            
            pDestBill->u.mtCallRecord.callReference.length = nCallReferenceLen;
            memcpy(pDestBill->u.mtCallRecord.callReference.value, 
                &(pMtcBill->call_reference[sizeof(pMtcBill->call_reference) - pMtcBill->call_reference_len]), nCallReferenceLen);   
        }
        else
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_callReference_present_byte] |=
                CMCC_CS_MTCallRecord_callReference_present;
            GetCallReference(pDestBill->u.mtCallRecord.callReference,
                pMtcBill->ModuleNum, pMtcBill->count);
        }
    }

    if (GET_CDR_FIELD_FILTER(sequenceNumber, MTCallRecord))		//中间话单流水号
	{
		if (pMtcBill->record_type != SINGLE_BILL)
		{		
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_sequenceNumber_present_byte] |=
                CMCC_CS_MTCallRecord_sequenceNumber_present;
		    pDestBill->u.mtCallRecord.sequenceNumber = pMtcBill->sequence;
        }
	}

    if (GET_CDR_FIELD_FILTER(additionalChgInfo, MTCallRecord))		//附加计费信息
	{
		if (pMtcBill->chargeIndicator >= 0 && pMtcBill->chargeIndicator < 3)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_additionalChgInfo_present_byte] |=
                CMCC_CS_MTCallRecord_additionalChgInfo_present;
            pDestBill->u.mtCallRecord.additionalChgInfo.bit_mask = CMCC_CS_chargeIndicator_present;
		    pDestBill->u.mtCallRecord.additionalChgInfo.chargeIndicator = pMtcBill->chargeIndicator;
        }
	}

    pDestBill->u.mtCallRecord.recordExtensions = NULL;
    if (GET_CDR_FIELD_FILTER(recordExtensions, MTCallRecord))		//记录扩展
	{
		//无此域
        //pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_recordExtensions_present_byte] |=
        //    CMCC_CS_MTCallRecord_recordExtensions_present;
	}

    if (GET_CDR_FIELD_FILTER(networkCallReference, MTCallRecord))		//网络呼叫参考号
	{
        if (pMtcBill->NetworkCallReferenceLen != 0)
        {
            int NetworkCallReferenceLen = min(pMtcBill->NetworkCallReferenceLen, 
                sizeof(pDestBill->u.mtCallRecord.networkCallReference.value));
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_networkCallReference_present_byte] |=
                CMCC_CS_MTCallRecord_networkCallReference_present;
            pDestBill->u.mtCallRecord.networkCallReference.length = NetworkCallReferenceLen;
		    memcpy(pDestBill->u.mtCallRecord.networkCallReference.value, 
                &(pMtcBill->NetworkcallReferenceNumber[sizeof(pMtcBill->NetworkcallReferenceNumber) - pMtcBill->NetworkCallReferenceLen]), NetworkCallReferenceLen);
        }
	}

    if (GET_CDR_FIELD_FILTER(mSCAddress, MTCallRecord))		//MSC 地址
	{
        int nMSCAddressLen = (pMtcBill->MSCAddress_BCDlen + 1) / 2;
        if (nMSCAddressLen > 0)
        {
		    pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_mSCAddress_present_byte] |=
                CMCC_CS_MTCallRecord_mSCAddress_present; 
            pDestBill->u.mtCallRecord.mSCAddress.length = 
                min(sizeof(pDestBill->u.mtCallRecord.mSCAddress.value), nMSCAddressLen + 1);
            pDestBill->u.mtCallRecord.mSCAddress.value[0] = *(pMtcBill->MSCAddress - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.mtCallRecord.mSCAddress.value + 1, pMtcBill->MSCAddress, 
                pDestBill->u.mtCallRecord.mSCAddress.length - 1);
        }
	}

    if (GET_CDR_FIELD_FILTER(fnur, MTCallRecord))		//fnur
	{
        if (pMtcBill->fnur != 0x00)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_fnur_present_byte] |=
                CMCC_CS_MTCallRecord_fnur_present;
            pDestBill->u.mtCallRecord.fnur = (CMCC_CS_Fnur)pMtcBill->fnur;
        }
	}

    if (GET_CDR_FIELD_FILTER(aiurRequested, MTCallRecord))		//aiurRequested
	{
        if(pMtcBill->aiurRequested != 0)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_aiurRequested_present_byte] |=
                CMCC_CS_MTCallRecord_aiurRequested_present;
            pDestBill->u.mtCallRecord.aiurRequested = (CMCC_CS_AiurRequested)pMtcBill->aiurRequested;
        }
	}

    if (GET_CDR_FIELD_FILTER(speechVersionSupported, MTCallRecord))		//speechVersionSupported
	{
        if (pMtcBill->TBS == MBILL_TELECOM_SERVICE && pMtcBill->speechVersionSupported != 0xFF)
        {
            ConvertSpeechVersion(pDestBill->u.mtCallRecord.speechVersionSupported.value[0],
                pMtcBill->speechVersionSupported);
            if(pDestBill->u.mtCallRecord.speechVersionSupported.value[0] != 0xFF)
            {
                pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_speechVersionSupported_present_byte] |=
                    CMCC_CS_MTCallRecord_speechVersionSupported_present;
                pDestBill->u.mtCallRecord.speechVersionSupported.length = 1;
            }
        }
	}

    if (GET_CDR_FIELD_FILTER(speechVersionUsed, MTCallRecord))		//speechVersionUsed
	{
        if (pMtcBill->TBS == MBILL_TELECOM_SERVICE && pMtcBill->speechVersionUsed != 0xFF)
        {
            ConvertSpeechVersion(pDestBill->u.mtCallRecord.speechVersionUsed.value[0],
                pMtcBill->speechVersionUsed);
            if(pDestBill->u.mtCallRecord.speechVersionUsed.value[0] != 0xFF)
            {
                pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_speechVersionUsed_present_byte] |=
                    CMCC_CS_MTCallRecord_speechVersionUsed_present;
		        pDestBill->u.mtCallRecord.speechVersionUsed.length = 1;
            }
        }
	}

    pDestBill->u.mtCallRecord.gsm_SCFAddress.length = 0;
    if (GET_CDR_FIELD_FILTER(gsm_SCFAddress, MTCallRecord))		//gsm_SCFAddress
	{
	    int nGSMSCFAddressLen = (pMtcBill->CAMEL_GsmSCF_Address_BCDlen + 1) / 2;
        if (nGSMSCFAddressLen > 0)
        {
		    pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_gsm_SCFAddress_present_byte] |=
                CMCC_CS_MTCallRecord_gsm_SCFAddress_present;
            //SCFAddress本身就是反向BCD码，而且第一个字节是多余的
            pDestBill->u.mtCallRecord.gsm_SCFAddress.length = 
                min(sizeof(pDestBill->u.mtCallRecord.gsm_SCFAddress.value), nGSMSCFAddressLen + 1);
            pDestBill->u.mtCallRecord.gsm_SCFAddress.value[0] = *(pMtcBill->CAMEL_GsmSCF_Address - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.mtCallRecord.gsm_SCFAddress.value + 1, pMtcBill->CAMEL_GsmSCF_Address, 
                pDestBill->u.mtCallRecord.gsm_SCFAddress.length - 1);
        }
	}

    if (GET_CDR_FIELD_FILTER(serviceKey, MTCallRecord))		//业务键
	{
        if (pMtcBill->ServiceKey != 0xFFFFFFFF)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_serviceKey_present_byte] |=
                CMCC_CS_MTCallRecord_serviceKey_present;
		    pDestBill->u.mtCallRecord.serviceKey = pMtcBill->ServiceKey;
        }
	}

    if (GET_CDR_FIELD_FILTER(systemType, MTCallRecord))		//系统类型
	{
        switch((ACCESS_NETWORK_TYPE)pMtcBill->systemType)
        {
        case ACCESS_BSC :
            pDestBill->u.mtCallRecord.systemType = CMCC_CS_gERAN;
		    pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_systemType_present_byte] |=
                CMCC_CS_MTCallRecord_systemType_present;
            break;
        case ACCESS_RNC :
            pDestBill->u.mtCallRecord.systemType = CMCC_CS_iuUTRAN;
		    pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_systemType_present_byte] |=
                CMCC_CS_MTCallRecord_systemType_present;
            break;
        case ACCESS_UNKNOWN:
            pDestBill->u.mtCallRecord.systemType = CMCC_CS_unknown;
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_systemType_present_byte] |= 
                CMCC_CS_MTCallRecord_systemType_present;
            break;
        } 
	}

    pDestBill->u.mtCallRecord.rateIndication.length = 0;
    if (GET_CDR_FIELD_FILTER(rateIndication, MTCallRecord))
	{
        if (pMtcBill->rateIndication != 0x00)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_rateIndication_present_byte] |=
                CMCC_CS_MTCallRecord_rateIndication_present;        
            pDestBill->u.mtCallRecord.rateIndication.length = 1;
            pDestBill->u.mtCallRecord.rateIndication.value[0] = pMtcBill->rateIndication;
        }
    }

    if (GET_CDR_FIELD_FILTER(roamingNumber, MTCallRecord))  //漫游号码
	{
		int nRoamingLen = (pMtcBill->roamingNumber_BCDlen + 1) / 2;
        if (nRoamingLen > 0)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_roamingNumber_present_byte] |=
                CMCC_CS_MTCallRecord_roamingNumber_present;
		    pDestBill->u.mtCallRecord.roamingNumber.length = 
                min(sizeof(pDestBill->u.mtCallRecord.roamingNumber.value), nRoamingLen + 1);
            pDestBill->u.mtCallRecord.roamingNumber.value[0] = *(pMtcBill->roamingNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.mtCallRecord.roamingNumber.value + 1, pMtcBill->roamingNumber, 
                pDestBill->u.mtCallRecord.roamingNumber.length - 1);
        }
    }

    if (GET_CDR_FIELD_FILTER(mscIncomingCircuit, MTCallRecord))		//mscIncomingCircuit
	{
        if (pMtcBill->mscIncomingCircuit != 0xFFFF)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_mscIncomingCircuit_present_byte] |=
                CMCC_CS_MTCallRecord_mscIncomingCircuit_present;
            pDestBill->u.mtCallRecord.mscIncomingCircuit = pMtcBill->mscIncomingCircuit; 
        }
    }
    
    if (GET_CDR_FIELD_FILTER(orgRNCorBSCId, MTCallRecord))
	{
        if (pMtcBill->rnc_bsc_id_len != 0)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_orgRNCorBSCId_present_byte] |=
                CMCC_CS_MTCallRecord_orgRNCorBSCId_present;
            pDestBill->u.mtCallRecord.orgRNCorBSCId.length = min(3,pMtcBill->rnc_bsc_id_len);
            memcpy(pDestBill->u.mtCallRecord.orgRNCorBSCId.value, pMtcBill->orgRNCorBSCId, pDestBill->u.mtCallRecord.orgRNCorBSCId.length);
        }
    }

    pDestBill->u.mtCallRecord.orgMSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgMSCId, MTCallRecord))
	{
        if (pMtcBill->msc_id_len != 0)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_orgMSCId_present_byte] |=
                CMCC_CS_MTCallRecord_orgMSCId_present;
            pDestBill->u.mtCallRecord.orgMSCId.length = min(3,pMtcBill->msc_id_len);
            memcpy(pDestBill->u.mtCallRecord.orgMSCId.value, pMtcBill->orgMSCId, pDestBill->u.mtCallRecord.orgMSCId.length);
        }
    }

    pDestBill->u.mtCallRecord.callEmlppPriority.length = 0;
    if (GET_CDR_FIELD_FILTER(callEmlppPriority, MTCallRecord))
	{
        if (pMtcBill->callEmlppPriority != 0xF)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_callEmlppPriority_present_byte] |=
                CMCC_CS_MTCallRecord_callEmlppPriority_present;
            pDestBill->u.mtCallRecord.callEmlppPriority.length = 1;
            pDestBill->u.mtCallRecord.callEmlppPriority.value[0] = pMtcBill->callEmlppPriority;
        }
    }

    
    pDestBill->u.mtCallRecord.calledDefaultEmlppPriority.length = 0;
    if (GET_CDR_FIELD_FILTER(calledDefaultEmlppPriority, MTCallRecord))
	{
        if (pMtcBill->calledDefaultEmlppPriority != 0xF)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_calledDefaultEmlppPriority_present_byte] |=
                CMCC_CS_calledDefaultEmlppPriority_present;
            pDestBill->u.mtCallRecord.calledDefaultEmlppPriority.length = 1;
            pDestBill->u.mtCallRecord.calledDefaultEmlppPriority.value[0] = pMtcBill->calledDefaultEmlppPriority;
        }
    }

    pDestBill->u.mtCallRecord.subscriberCategory.length = 0;
    if (GET_CDR_FIELD_FILTER(subscriberCategory, MTCallRecord))
	{
        pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_subscriberCategory_present_byte] |=
            CMCC_CS_MTCallRecord_subscriberCategory_present;
        pDestBill->u.mtCallRecord.subscriberCategory.length = 1;
        pDestBill->u.mtCallRecord.subscriberCategory.value[0] = pMtcBill->subscriberCategory;
    }

    pDestBill->u.mtCallRecord.firstmccmnc.length = 0;
    if (GET_CDR_FIELD_FILTER(firstmccmnc, MTCallRecord))
	{
        if (!(pMtcBill->caller_location_LAI[0] == 0xFF
            && pMtcBill->caller_location_LAI[1] == 0xFF
            && pMtcBill->caller_location_LAI[2] == 0xFF)
            && !(pMtcBill->caller_location_LAI[0] == 0
            && pMtcBill->caller_location_LAI[1] == 0
            && pMtcBill->caller_location_LAI[2] == 0))
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_firstmccmnc_present_byte] 
                |= CMCC_CS_MTCallRecord_firstmccmnc_present;
            pDestBill->u.mtCallRecord.firstmccmnc.length = 3;
            memcpy(pDestBill->u.mtCallRecord.firstmccmnc.value, pMtcBill->caller_location_LAI, 3);
        }
    }

    pDestBill->u.mtCallRecord.intermediatemccmnc.length = 0;
    if (GET_CDR_FIELD_FILTER(intermediatemccmnc, MTCallRecord))
	{
    }

    pDestBill->u.mtCallRecord.lastmccmnc.length = 0;
    if (GET_CDR_FIELD_FILTER(lastmccmnc, MTCallRecord))
	{
        if (!(pMtcBill->caller_changeOflocation_LAI[0] == 0xFF
            && pMtcBill->caller_changeOflocation_LAI[1] == 0xFF
            && pMtcBill->caller_changeOflocation_LAI[2] == 0xFF)
            && !(pMtcBill->caller_changeOflocation_LAI[0] == 0
            && pMtcBill->caller_changeOflocation_LAI[1] == 0
            && pMtcBill->caller_changeOflocation_LAI[2] == 0))
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_lastmccmnc_present_byte] 
                |= CMCC_CS_MTCallRecord_lastmccmnc_present;
            pDestBill->u.mtCallRecord.lastmccmnc.length = 3;
            memcpy(pDestBill->u.mtCallRecord.lastmccmnc.value, pMtcBill->caller_changeOflocation_LAI, 3);
        }
    }

    if (GET_CDR_FIELD_FILTER(cUGOutgoingAccessIndicator, MTCallRecord))
	{
		if (pMtcBill->CUGOutgoingAccessIndicator != 0)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_cUGOutgoingAccessIndicator_present_byte] |=
                CMCC_CS_MTCallRecord_cUGOutgoingAccessIndicator_present;
            pDestBill->u.mtCallRecord.cUGOutgoingAccessIndicator = CMCC_CS_cUGCall;
        }
	}

    pDestBill->u.mtCallRecord.cUGInterlockCode.length = 0;
    if (GET_CDR_FIELD_FILTER(cUGInterlockCode, MTCallRecord))
	{
		if (pMtcBill->CUGInterlockCode != 0xFFFFFFFF)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_cUGInterlockCode_present_byte] |=
                CMCC_CS_MTCallRecord_cUGInterlockCode_present;
            pDestBill->u.mtCallRecord.cUGInterlockCode.length = 4;
            memcpy(pDestBill->u.mtCallRecord.cUGInterlockCode.value, &(pMtcBill->CUGInterlockCode), 4);
        }
	}

    if (GET_CDR_FIELD_FILTER(hotBillingTag, MTCallRecord))		//热计费标识
	{
		if (pMtcBill->HotBillingTag == HOT_BILLING)
		{
		    pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_hotBillingTag_present_byte] |=
                CMCC_CS_MTCallRecord_hotBillingTag_present;
            pDestBill->u.mtCallRecord.hotBillingTag = CMCC_CS_hotBilling;
		}
		else
		{
			pDestBill->u.mtCallRecord.hotBillingTag = CMCC_CS_noHotBilling;
		}
	}

    pDestBill->u.mtCallRecord.redirectingnumber.length = 0;
    pDestBill->u.mtCallRecord.redirectingnumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(redirectingnumber, MTCallRecord))		//redirectingnumber
	{
		int nTranslatedLen = (pMtcBill->redirectingnumber_BCDlen + 1) / 2;
        if (nTranslatedLen > 0)
        {
		    pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_redirectingnumber_present_byte] |=
                CMCC_CS_redirectingnumber_present;  
		    pDestBill->u.mtCallRecord.redirectingnumber.length = nTranslatedLen + 1;
		    pDestBill->u.mtCallRecord.redirectingnumber.value = new (g_pAsnBuffForCMCC)BYTE[nTranslatedLen + 1];
            pDestBill->u.mtCallRecord.redirectingnumber.value[0] = *(pMtcBill->redirectingnumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.mtCallRecord.redirectingnumber.value + 1, pMtcBill->redirectingnumber, nTranslatedLen);
        }
	}

    if (GET_CDR_FIELD_FILTER(redirectingcounter, MTCallRecord))		//redirectingcounter
	{
        if (pMtcBill->CFW_invoked_times > 0)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_redirectingcounter_present_byte] |=
                CMCC_CS_redirectingcounter_present;
            pDestBill->u.mtCallRecord.redirectingcounter = pMtcBill->CFW_invoked_times;
        }
	}

    if (GET_CDR_FIELD_FILTER(partialRecordType, MTCallRecord))      //1
    {
        if (pMtcBill->cause_for_partial_record != 0xff)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_partialRecordType_present_byte] |=
                CMCC_CS_MTCallRecord_partialRecordType_present;
            pDestBill->u.mtCallRecord.partialRecordType = (CMCC_CS_PartialRecordType)pMtcBill->cause_for_partial_record;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(guaranteedBitRate, MTCallRecord))      //2
    {
        if (pMtcBill->GuaranteedBitRate > 0 && pMtcBill->GuaranteedBitRate < 8)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_guaranteedBitRate_present_byte] |=
                CMCC_CS_MTCallRecord_guaranteedBitRate_present;
            pDestBill->u.mtCallRecord.guaranteedBitRate = (CMCC_CS_GuaranteedBitRate)pMtcBill->GuaranteedBitRate;
        }
    }    
    if (GET_CDR_FIELD_FILTER(maximumBitRate, MTCallRecord))      //3
    {
        if (pMtcBill->MaximumBitRate > 0 && pMtcBill->MaximumBitRate < 7)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_maximumBitRate_present_byte] |=
                CMCC_CS_MTCallRecord_maximumBitRate_present;
            pDestBill->u.mtCallRecord.maximumBitRate = (CMCC_CS_MaximumBitRate)pMtcBill->MaximumBitRate;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(initialCallAttemptFlag, MTCallRecord))      //4
    {
        if (pMtcBill->initialCallAttemptFlag != FALSE)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_initialCallAttemptFlag_present_byte] |=
                CMCC_CS_initialCallAttemptFlag_present;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(ussdCallBackFlag, MTCallRecord))      //5
    {
        if (pMtcBill->UssdCallBackIndication != FALSE)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_ussdCallBackFlag_present_byte] |=
                CMCC_CS_ussdCallBackFlag_present;
        }
    }
        
    if (GET_CDR_FIELD_FILTER(modemType, MTCallRecord))      //6
    {
        if (pMtcBill->ModemType != 0)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_modemType_present_byte] |=
                CMCC_CS_MTCallRecord_modemType_present;
            pDestBill->u.mtCallRecord.modemType = (CMCC_CS_ModemType)pMtcBill->ModemType;
        }
    }
    
    pDestBill->u.mtCallRecord.classmark3.length = 0;
    if (GET_CDR_FIELD_FILTER(classmark3, MTCallRecord))      //7
    {
        if (pMtcBill->Classmark3Indication)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_classmark3_present_byte] |=
                CMCC_CS_MTCallRecord_classmark3_present;
            pDestBill->u.mtCallRecord.classmark3.length = 2;
            memcpy(pDestBill->u.mtCallRecord.classmark3.value,&(pMtcBill->Classmark3Information),2);
        }
    }
    
    if (GET_CDR_FIELD_FILTER(optimalRoutingFlag, MTCallRecord))      //8
    {
        if (pMtcBill->SORIndication) 
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_optimalRoutingFlag_present_byte] |=
                CMCC_CS_MTCallRecord_optimalRoutingFlag_present;
        }
    }  

    if (GET_CDR_FIELD_FILTER(globalAreaID, MTCallRecord))      //9
    {
        if (IsLACandCellIDValid(&(pMtcBill->called_location_LAI[3]),pMtcBill->called_location_cellID))
        {            
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_globalAreaID_present_byte] |=
                CMCC_CS_MTCallRecord_globalAreaID_present;
            pDestBill->u.mtCallRecord.globalAreaID.length = 7;
            memcpy(pDestBill->u.mtCallRecord.globalAreaID.value,&(pMtcBill->called_location_LAI),5);
            memcpy(pDestBill->u.mtCallRecord.globalAreaID.value + 5,&(pMtcBill->called_location_cellID),2);
        }
    }
       
    pDestBill->u.mtCallRecord.changeOfglobalAreaID = NULL;
    if (GET_CDR_FIELD_FILTER(changeOfGlobalAreaID, MTCallRecord))      //10
    {
        if (pEventBill != NULL && nEventNum != 0 && pMtcBill->HO_invoked_times != 0)
        {
            SS_Action_Bill_Report* pSingleEventBill = NULL;
            
            for(int i = nEventNum - 1; i >= 0; i--) //由于后面的链表是插入方式，所以这里从后往前找
            {
                pSingleEventBill = (SS_Action_Bill_Report*)((MSC_Bill_Report*)pEventBill + i);
                
                if (pSingleEventBill->event_type == HO)
                {
                    CMCC_CS__seqof6_* pGloble = new (g_pAsnBuffForCMCC)CMCC_CS__seqof6_;
                    ConvertTimeStamp(pGloble->value.changeTime, &(pSingleEventBill->ssActionTime_year));
                    
                    pGloble->value.location.length = 7;
                    memcpy(pGloble->value.location.value, pSingleEventBill->served_location_LAI, 5);
                    memcpy(pGloble->value.location.value + 5, pSingleEventBill->served_location_cellID,2);
                    
                    pGloble->next = pDestBill->u.mtCallRecord.changeOfglobalAreaID;
                    pDestBill->u.mtCallRecord.changeOfglobalAreaID = pGloble;
                }
            }
        }
        //如果不出事件话单但是当前位置区小区有效并且跟初始值不相同，那么还要填当前值
        else if (IsLACandCellIDValid(&(pMtcBill->called_changeOflocation_LAI[3]), pMtcBill->called_changeOflocation_cellID)
            && memcmp(pMtcBill->called_changeOflocation_LAI, pMtcBill->called_location_LAI, 2) != 0)
        {
            CMCC_CS__seqof6_* pGloble = new (g_pAsnBuffForCMCC)CMCC_CS__seqof6_;
            ConvertTimeStamp(pGloble->value.changeTime, &(pMtcBill->end_year));  //时间取结束时间
            
            pGloble->value.location.length = 7;
            memcpy(pGloble->value.location.value, pMtcBill->called_changeOflocation_LAI, 5);
            memcpy(pGloble->value.location.value + 5, pMtcBill->called_changeOflocation_cellID,2);
            
            pGloble->next = pDestBill->u.mtCallRecord.changeOfglobalAreaID;
            pDestBill->u.mtCallRecord.changeOfglobalAreaID = pGloble;
        }
        
        if (pDestBill->u.mtCallRecord.changeOfglobalAreaID != NULL)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_changeOfglobalAreaID_present_byte] |=
                CMCC_CS_MTCallRecord_changeOfglobalAreaID_present;
        }
    }

    if (GET_CDR_FIELD_FILTER(chargedParty,MTCallRecord) )
    {
        if (pMtcBill->chargedParty == 0 || pMtcBill->chargedParty == 1)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_chargedParty_present_byte] |= 
                CMCC_CS_MTCallRecord_chargedParty_present;
            pDestBill->u.mtCallRecord.chargedParty = (CMCC_CS_ChargedParty)pMtcBill->chargedParty;
        }
    }
    
    pDestBill->u.mtCallRecord.originalCalledNumber.length = 0;
    pDestBill->u.mtCallRecord.originalCalledNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(originalCalledNumber,MTCallRecord))
    {
        int nOriginalCalledLen = (pMtcBill->originalCalledNumber_BCDlen + 1)/2;
        if (nOriginalCalledLen > 0 )
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_originalCalledNumber_present_byte] |=
                CMCC_CS_MTCallRecord_originalCalledNumber_present;
            pDestBill->u.mtCallRecord.originalCalledNumber.length = nOriginalCalledLen + 1;
            pDestBill->u.mtCallRecord.originalCalledNumber.value = new (g_pAsnBuffForCMCC)BYTE[nOriginalCalledLen + 1];                        
            pDestBill->u.mtCallRecord.originalCalledNumber.value[0] = *(pMtcBill->originalCalledNumber - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.mtCallRecord.originalCalledNumber.value + 1,pMtcBill->originalCalledNumber,
                pDestBill->u.mtCallRecord.originalCalledNumber.length - 1);
        }
    }
    
    pDestBill->u.mtCallRecord.callingChargeAreaCode.length = 0;
    if (GET_CDR_FIELD_FILTER(callingChargeAreaCode,MTCallRecord))
    {
        if (pMtcBill->callingChargeAreaCode[0] != 0xff &&
            pMtcBill->callingChargeAreaCode[1] != 0xff &&
            pMtcBill->callingChargeAreaCode[2] != 0xff)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_callingChargeAreaCode_present_byte] |=
                CMCC_CS_MTCallRecord_callingChargeAreaCode_present;
            pDestBill->u.mtCallRecord.callingChargeAreaCode.length = 3;
            memcpy(pDestBill->u.mtCallRecord.callingChargeAreaCode.value,pMtcBill->callingChargeAreaCode,3);
        }
    }
    
    pDestBill->u.mtCallRecord.calledChargeAreaCode.length = 0;
    if (GET_CDR_FIELD_FILTER(calledChargeAreaCode,MTCallRecord))
    {
        if (pMtcBill->calledChargeAreaCode[0] != 0xff &&
            pMtcBill->calledChargeAreaCode[1] != 0xff &&
            pMtcBill->calledChargeAreaCode[2] != 0xff)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_calledChargeAreaCode_present_byte] |=
                CMCC_CS_MTCallRecord_calledChargeAreaCode_present;
            pDestBill->u.mtCallRecord.calledChargeAreaCode.length = 3;
            memcpy(pDestBill->u.mtCallRecord.calledChargeAreaCode.value,pMtcBill->calledChargeAreaCode,3);
        }
    }
    
    pDestBill->u.mtCallRecord.selectedCIC.length = 0;
    if (GET_CDR_FIELD_FILTER(selectedCIC,MTCallRecord))
    {
        if (pMtcBill->selectedCIC[0] != 0xff &&
            pMtcBill->selectedCIC[1] != 0xff &&
            pMtcBill->selectedCIC[2] != 0xff)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_selectedCIC_present_byte] |= 
                CMCC_CS_MTCallRecord_selectedCIC_present; 
            pDestBill->u.mtCallRecord.selectedCIC.length = 3;
            memcpy(pDestBill->u.mtCallRecord.selectedCIC.value,pMtcBill->selectedCIC,3);
        }
    }

    if (GET_CDR_FIELD_FILTER(defaultCallHandling, MTCallRecord))		//defaultCallHandling
    {
        if (pMtcBill->defaultCallHandling == 0 || pMtcBill->defaultCallHandling == 1)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_defaultCallHandling_present_byte] |=
                CMCC_CS_MTCallRecord_defaultCallHandling_present;
            if (pMtcBill->defaultCallHandling == 0)
            {
                pDestBill->u.mtCallRecord.defaultCallHandling = CMCC_CS_continueCall;
            }
            else
            {
                pDestBill->u.mtCallRecord.defaultCallHandling = CMCC_CS_releaseCall;
            }
        }
    }
    
    pDestBill->u.mtCallRecord.freeFormatData.length = 0;
    if (GET_CDR_FIELD_FILTER(freeFormatData, MTCallRecord))		//自由格式数据
    {
        if (pMtcBill->fcidata_len != 0)
        {
            int fci_len = min(sizeof(pDestBill->u.mtCallRecord.freeFormatData.value), pMtcBill->fcidata_len);
            memcpy(pDestBill->u.mtCallRecord.freeFormatData.value, pMtcBill->VarPart, fci_len);
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_freeFormatData_present_byte] |=
                CMCC_CS_MTCallRecord_freeFormatData_present;
            pDestBill->u.mtCallRecord.freeFormatData.length = fci_len;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(freeFormatDataAppend, MTCallRecord))		//freeFormatDataAppend
    {
        if ((AppendIndication)pMtcBill->freeFormatDataAppend == Append)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_freeFormatDataAppend_present_byte] |=
                CMCC_CS_MTCallRecord_freeFormatDataAppend_present;
            pDestBill->u.mtCallRecord.freeFormatDataAppend = TRUE;
        }
        else
        {
            pDestBill->u.mtCallRecord.freeFormatDataAppend = FALSE;     
        }
    }
    
    if (GET_CDR_FIELD_FILTER(numberOfDPEncountered, MTCallRecord))		//numberOfDPEncountered
    {
        if (pMtcBill->numberOfDPEncountered != 0)  //pCfwBill->UserType＝2为智能呼叫，现在不需要判断了
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_numberOfDPEncountered_present_byte] |=
                CMCC_CS_MTCallRecord_numberOfDPEncountered_present;
            pDestBill->u.mtCallRecord.numberOfDPEncountered = pMtcBill->numberOfDPEncountered;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(levelOfCAMELService, MTCallRecord))		//numberOfDPEncountered
    {
        if (pMtcBill->levelOfCAMELService != 0xFF)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_levelOfCAMELService_present_byte] |=
                CMCC_CS_MTCallRecord_levelOfCAMELService_present;
            switch(pMtcBill->levelOfCAMELService)
            {
            case 0:
                pDestBill->u.mtCallRecord.levelOfCAMELService = CMCC_CS_basic;
                break;
            case 1:
                pDestBill->u.mtCallRecord.levelOfCAMELService = CMCC_CS_callDurationSupervision;
                break;
            case 2:
                pDestBill->u.mtCallRecord.levelOfCAMELService = CMCC_CS_onlineCharging;
                break;
            default:
                pDestBill->u.mtCallRecord.levelOfCAMELService = CMCC_CS_basic;
                break;
            }
        }
    }
    
    pDestBill->u.mtCallRecord.eaSubscriberInfo.length = 0;
    if (GET_CDR_FIELD_FILTER(eaSubscriberInfo, MTCallRecord)) //此域无法得到
    {
        if (pMtcBill->eaSubscriberInfo[0] != 0xff &&
            pMtcBill->eaSubscriberInfo[1] != 0xff &&
            pMtcBill->eaSubscriberInfo[2] != 0xff)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_eaSubscriberInfo_present_byte] |= 
                CMCC_CS_MTCallRecord_eaSubscriberInfo_present; 
            pDestBill->u.mtCallRecord.eaSubscriberInfo.length = 3;
            memcpy(pDestBill->u.mtCallRecord.eaSubscriberInfo.value,pMtcBill->eaSubscriberInfo,3);
        }
    }
    
    if (GET_CDR_FIELD_FILTER(portedflag,MTCallRecord))
    {
        if (pMtcBill->portedflag)
        {        
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_portedflag_present_byte] |=
                CMCC_CS_MTCallRecord_portedflag_present;
            pDestBill->u.mtCallRecord.portedflag = (CMCC_CS_PortedFlag)pMtcBill->portedflag;
        }
    }
   
    pDestBill->u.mtCallRecord.cUGIndex.length = 0; 
    if (GET_CDR_FIELD_FILTER(cUGIndex,MTCallRecord))
    {
        if (pMtcBill->CUGIndex != 0xFFFF)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_cUGIndex_present_byte] |= 
                CMCC_CS_MTCallRecord_cUGIndex_present;
            pDestBill->u.mtCallRecord.cUGIndex.length = 2;
            memcpy(pDestBill->u.mtCallRecord.cUGIndex.value,&(pMtcBill->CUGIndex),2);
        }
    }
    
    if (GET_CDR_FIELD_FILTER(cUGIncomingAccessUsed, MTCallRecord))
    {
        if(pMtcBill->CUGOutgoingAccessUsed ==1 || pMtcBill->CUGOutgoingAccessUsed == 0)
        {
            pDestBill->u.mtCallRecord.bit_mask[CMCC_CS_MTCallRecord_cUGIncomingAccessUsed_present_byte] |=
                CMCC_CS_MTCallRecord_cUGIncomingAccessUsed_present;
            pDestBill->u.mtCallRecord.cUGIncomingAccessUsed = 
                (CMCC_CS_CUGIncomingAccessUsed)pMtcBill->CUGOutgoingAccessUsed;
        }
    }
    
    return TRUE;
}

BOOL ConvertABill_TCAMEL(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill)
{
	TCAMEL_Bill_Report* pTcamelBill = (TCAMEL_Bill_Report *)pSrcBill;
	
	pDestBill->choice = CMCC_CS_termCAMELRecord_chosen;
	memset(pDestBill->u.termCAMELRecord.bit_mask, 0, sizeof(pDestBill->u.termCAMELRecord.bit_mask));

    if (GET_CDR_FIELD_FILTER(recordType, TermCAMELRecord))
    {
        pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_recordtype_present_byte]
            |= CMCC_CS_recordtype_present;
	    pDestBill->u.termCAMELRecord.recordtype = CMCC_CS_CallEventRecordType_termCAMELRecord;		//记录类型
    }

    if (GET_CDR_FIELD_FILTER(servedIMSI, TermCAMELRecord))
    {
        pDestBill->u.termCAMELRecord.servedIMSI.length = 
            BCDtoRBCD(pDestBill->u.termCAMELRecord.servedIMSI.value, pTcamelBill->servedIMSI, 8);
        if(pDestBill->u.termCAMELRecord.servedIMSI.length > 0)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_servedIMSI_present_byte]
                |= CMCC_CS_TermCAMELRecord_servedIMSI_present;         
        }
    }

	if (GET_CDR_FIELD_FILTER(servedMSISDN, TermCAMELRecord))		//servedMSISDN
	{
	    int nMSISDNLen = (pTcamelBill->servedMSISDN_BCDlen + 1) / 2;
        if (nMSISDNLen > 0)
        {
		    pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_servedMSISDN_present_byte]
                |= CMCC_CS_TermCAMELRecord_servedMSISDN_present; 
            pDestBill->u.termCAMELRecord.servedMSISDN.length = 
                min(sizeof(pDestBill->u.termCAMELRecord.servedMSISDN.value), nMSISDNLen + 1);
            pDestBill->u.termCAMELRecord.servedMSISDN.value[0] = *(pTcamelBill->servedMSISDN - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.termCAMELRecord.servedMSISDN.value + 1, pTcamelBill->servedMSISDN, 
                pDestBill->u.termCAMELRecord.servedMSISDN.length - 1);
        }
	}

    pDestBill->u.termCAMELRecord.recordingEntity.length = 0;
    if (GET_CDR_FIELD_FILTER(recordingEntity, TermCAMELRecord))
    {
	    int nLocalMSCLen = (pTcamelBill->recordingEntity_BCDlen + 1) / 2;
        if (nLocalMSCLen > 0)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_recordingEntity_present_byte]
                |= CMCC_CS_TermCAMELRecord_recordingEntity_present;
		    pDestBill->u.termCAMELRecord.recordingEntity.length = 
                min(sizeof(pDestBill->u.termCAMELRecord.recordingEntity.value), nLocalMSCLen + 1);
            pDestBill->u.termCAMELRecord.recordingEntity.value[0] = *(pTcamelBill->recordingEntity - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.termCAMELRecord.recordingEntity.value + 1, pTcamelBill->recordingEntity, 
                pDestBill->u.termCAMELRecord.recordingEntity.length - 1);
        }
    }

    if (GET_CDR_FIELD_FILTER(interrogationTime, TermCAMELRecord))
    {
        pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_interrogationTime_present_byte]
            |= CMCC_CS_TermCAMELRecord_interrogationTime_present;
        ConvertTimeStamp(pDestBill->u.termCAMELRecord.interrogationTime, &(pTcamelBill->GMSC_query_hlr_year));
    }

    pDestBill->u.termCAMELRecord.destinationRoutingAddress.length = 0;
    pDestBill->u.termCAMELRecord.destinationRoutingAddress.value = NULL;
    if (GET_CDR_FIELD_FILTER(destinationRoutingAddress, TermCAMELRecord))
    {
	    int nCAMELDestLen = (pTcamelBill->CAMELDestinationNumber_BCDlen + 1) / 2;
        if (nCAMELDestLen > 0)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_destinationRoutingAddress_present_byte]
                |= CMCC_CS_destinationRoutingAddress_present;
		    pDestBill->u.termCAMELRecord.destinationRoutingAddress.length = nCAMELDestLen + 1;
		    pDestBill->u.termCAMELRecord.destinationRoutingAddress.value = new (g_pAsnBuffForCMCC)BYTE[nCAMELDestLen + 1];
            pDestBill->u.termCAMELRecord.destinationRoutingAddress.value[0] =*(pTcamelBill->CAMELDestinationNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.termCAMELRecord.destinationRoutingAddress.value + 1, 
                pTcamelBill->CAMELDestinationNumber, nCAMELDestLen);
        }
    }

    pDestBill->u.termCAMELRecord.gsm_SCFAddress.length = 0;
    if (GET_CDR_FIELD_FILTER(gsm_SCFAddress, TermCAMELRecord))
    {
	    int nGSMSCFAddressLen = (pTcamelBill->CAMEL_GsmSCF_Address_BCDlen + 1) / 2;
        if (nGSMSCFAddressLen > 0)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_gsm_SCFAddress_present_byte]
                |= CMCC_CS_TermCAMELRecord_gsm_SCFAddress_present;
            pDestBill->u.termCAMELRecord.gsm_SCFAddress.length = 
                min(sizeof(pDestBill->u.termCAMELRecord.gsm_SCFAddress.value), nGSMSCFAddressLen + 1);
            pDestBill->u.termCAMELRecord.gsm_SCFAddress.value[0] = *(pTcamelBill->CAMEL_GsmSCF_Address - 2) | 0x80; 
            BCDtoRBCD(pDestBill->u.termCAMELRecord.gsm_SCFAddress.value + 1, pTcamelBill->CAMEL_GsmSCF_Address, 
                pDestBill->u.termCAMELRecord.gsm_SCFAddress.length - 1);
        }
    }

    if (GET_CDR_FIELD_FILTER(serviceKey, TermCAMELRecord))
    {
        if(pTcamelBill->ServiceKey != 0xFFFFFFFF)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_serviceKey_present_byte]
                |= CMCC_CS_TermCAMELRecord_serviceKey_present;
            pDestBill->u.termCAMELRecord.serviceKey = pTcamelBill->ServiceKey;
        }
    }

    if (GET_CDR_FIELD_FILTER(networkCallReference, TermCAMELRecord))		//网络呼叫参考号
	{
        if (pTcamelBill->NetworkCallReferenceLen != 0)
        {
            int NetworkCallReferenceLen = min(pTcamelBill->NetworkCallReferenceLen, 
                sizeof(pDestBill->u.termCAMELRecord.networkCallReference.value));
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_networkCallReference_present_byte] |=
                CMCC_CS_TermCAMELRecord_networkCallReference_present;
		    pDestBill->u.termCAMELRecord.networkCallReference.length = NetworkCallReferenceLen;
    	    memcpy(pDestBill->u.termCAMELRecord.networkCallReference.value,
                &(pTcamelBill->NetworkcallReferenceNumber[sizeof(pTcamelBill->NetworkcallReferenceNumber) - pTcamelBill->NetworkCallReferenceLen]), NetworkCallReferenceLen);
        }
	}    

    if (GET_CDR_FIELD_FILTER(mSCAddress, TermCAMELRecord))		//MSC 地址
	{
        int nMSCAddressLen = (pTcamelBill->MSCAddress_BCDlen + 1) / 2;
        if (nMSCAddressLen > 0)
        {
		    pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_mSCAddress_present_byte] |=
                CMCC_CS_TermCAMELRecord_mSCAddress_present;   
            pDestBill->u.termCAMELRecord.mSCAddress.length = 
                min(sizeof(pDestBill->u.termCAMELRecord.mSCAddress.value), nMSCAddressLen + 1);
            pDestBill->u.termCAMELRecord.mSCAddress.value[0] = *(pTcamelBill->MSCAddress - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.termCAMELRecord.mSCAddress.value + 1, pTcamelBill->MSCAddress, 
                pDestBill->u.termCAMELRecord.mSCAddress.length - 1);
        }
	}

    if (GET_CDR_FIELD_FILTER(defaultCallHandling, TermCAMELRecord))		//defaultCallHandling
	{
		if (pTcamelBill->defaultCallHandling == 0 || pTcamelBill->defaultCallHandling ==1)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_defaultCallHandling_present_byte] |= 
                CMCC_CS_TermCAMELRecord_defaultCallHandling_present;
		    if (pTcamelBill->defaultCallHandling == 0)
            {
                pDestBill->u.termCAMELRecord.defaultCallHandling = CMCC_CS_continueCall;
            }
            else
            {
                pDestBill->u.termCAMELRecord.defaultCallHandling = CMCC_CS_releaseCall;
            }
        }
	}

    pDestBill->u.termCAMELRecord.recordExtensions = NULL;
    if (GET_CDR_FIELD_FILTER(recordExtensions, TermCAMELRecord))		//记录扩展
	{
    }

    pDestBill->u.termCAMELRecord.calledNumber.length = 0;
    pDestBill->u.termCAMELRecord.calledNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(calledNumber, TermCAMELRecord))
    {
	    int nCalledLen = (pTcamelBill->calledNumber_BCDlen + 1) / 2;
        if (nCalledLen > 0)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_calledNumber_present_byte]
                |= CMCC_CS_TermCAMELRecord_calledNumber_present;
            pDestBill->u.termCAMELRecord.calledNumber.length = nCalledLen + 1;
		    pDestBill->u.termCAMELRecord.calledNumber.value = new (g_pAsnBuffForCMCC)BYTE[nCalledLen + 1];
            pDestBill->u.termCAMELRecord.calledNumber.value[0] = *(pTcamelBill->calledNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.termCAMELRecord.calledNumber.value + 1, pTcamelBill->calledNumber, nCalledLen);
        }
    }

    pDestBill->u.termCAMELRecord.callingNumber.length = 0;
    pDestBill->u.termCAMELRecord.callingNumber.value = NULL;
	if (GET_CDR_FIELD_FILTER(callingNumber, TermCAMELRecord))		//callingNumber
	{
		int nCallerLen = (pTcamelBill->callingNumber_BCDlen + 1) / 2;
        if (nCallerLen > 0)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_callingNumber_present_byte] |=
                CMCC_CS_TermCAMELRecord_callingNumber_present;
		    pDestBill->u.termCAMELRecord.callingNumber.length = nCallerLen + 1;
		    pDestBill->u.termCAMELRecord.callingNumber.value = new (g_pAsnBuffForCMCC)BYTE[nCallerLen + 1];
            pDestBill->u.termCAMELRecord.callingNumber.value[0] = *(pTcamelBill->callingNumber - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.termCAMELRecord.callingNumber.value + 1, pTcamelBill->callingNumber, nCallerLen);
        }
	}

    pDestBill->u.termCAMELRecord.mscIncomingROUTE.u.rOUTEName = NULL;
	if (GET_CDR_FIELD_FILTER(mscIncomingROUTE, TermCAMELRecord))		//mscIncomingROUTE
	{
		if (pTcamelBill->mscIncomingROUTE_ind == SHOW_ROUTE_NO)
        {
            if (pTcamelBill->mscIncomingROUTE_ROUTENumber != 0xFFFF)
            {
                pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_mscIncomingROUTE_present_byte] |=
                    CMCC_CS_TermCAMELRecord_mscIncomingROUTE_present;
		        pDestBill->u.termCAMELRecord.mscIncomingROUTE.choice = CMCC_CS_rOUTENumber_chosen;
		        pDestBill->u.termCAMELRecord.mscIncomingROUTE.u.rOUTENumber = pTcamelBill->mscIncomingROUTE_ROUTENumber;
            }
        }
        else if (pTcamelBill->mscIncomingROUTE_ind == SHOW_ROUTE_NAME)
        {
            if (*(pTcamelBill->mscIncomingROUTE_ROUTEName) != 0x20)
            {
                pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_mscIncomingROUTE_present_byte] |=
                    CMCC_CS_TermCAMELRecord_mscIncomingROUTE_present;
		        pDestBill->u.termCAMELRecord.mscIncomingROUTE.choice = CMCC_CS_rOUTEName_chosen;
		        pDestBill->u.termCAMELRecord.mscIncomingROUTE.u.rOUTEName = new (g_pAsnBuffForCMCC)char[11];
                memcpy(pDestBill->u.termCAMELRecord.mscIncomingROUTE.u.rOUTEName, pTcamelBill->mscIncomingROUTE_ROUTEName, 10);
                pDestBill->u.termCAMELRecord.mscIncomingROUTE.u.rOUTEName[10] = '\0';
            }
        }
	}

    pDestBill->u.termCAMELRecord.mscOutgoingROUTE.u.rOUTEName = NULL;
	if (GET_CDR_FIELD_FILTER(mscOutgoingROUTE, TermCAMELRecord))		//mscOutgongingTKGP
	{
        if (pTcamelBill->mscOutgoingROUTE_ind == SHOW_ROUTE_NO)
        {
            if (pTcamelBill->mscOutgoingROUTE_ROUTENumber != 0xFFFF) 
            {
                pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_mscOutgoingROUTE_present_byte] |=
                    CMCC_CS_TermCAMELRecord_mscOutgoingROUTE_present;
		        pDestBill->u.termCAMELRecord.mscOutgoingROUTE.choice = CMCC_CS_rOUTENumber_chosen;
		        pDestBill->u.termCAMELRecord.mscOutgoingROUTE.u.rOUTENumber = pTcamelBill->mscOutgoingROUTE_ROUTENumber;
            }
        }
        else if (pTcamelBill->mscOutgoingROUTE_ind == SHOW_ROUTE_NAME)
        {
            if (*(pTcamelBill->mscOutgoingROUTE_ROUTEName) != 0x20)
            {
                pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_mscOutgoingROUTE_present_byte] |=
                    CMCC_CS_TermCAMELRecord_mscOutgoingROUTE_present;
		        pDestBill->u.termCAMELRecord.mscOutgoingROUTE.choice = CMCC_CS_rOUTEName_chosen;
		        pDestBill->u.termCAMELRecord.mscOutgoingROUTE.u.rOUTEName = new (g_pAsnBuffForCMCC)char[11];
                memcpy(pDestBill->u.termCAMELRecord.mscOutgoingROUTE.u.rOUTEName, pTcamelBill->mscOutgoingROUTE_ROUTEName, 10);
                pDestBill->u.termCAMELRecord.mscOutgoingROUTE.u.rOUTEName[10] = '\0';
            }
        }
    }

	if (GET_CDR_FIELD_FILTER(seizureTime, TermCAMELRecord))		//seizureTime
	{
        if (pTcamelBill->causeForTerm == unsuccessfulCallAttempt)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_seizureTime_present_byte] |=
                CMCC_CS_TermCAMELRecord_seizureTime_present;
            ConvertTimeStamp(pDestBill->u.termCAMELRecord.seizureTime, &(pTcamelBill->initial_year));
        }
    }

    if (GET_CDR_FIELD_FILTER(answerTime, TermCAMELRecord))		//answerTimestamp
	{
        if (pTcamelBill->causeForTerm != unsuccessfulCallAttempt)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_answerTime_present_byte] |= 
                CMCC_CS_TermCAMELRecord_answerTime_present;
		    ConvertTimeStamp(pDestBill->u.termCAMELRecord.answerTime, &(pTcamelBill->initial_year));
        }
	}
	
	if (GET_CDR_FIELD_FILTER(releaseTime, TermCAMELRecord))	//releaseTimestamp
	{
        if (pTcamelBill->end_year != 0xFF)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_releaseTime_present_byte] |=
                CMCC_CS_TermCAMELRecord_releaseTime_present;
		    ConvertTimeStamp(pDestBill->u.termCAMELRecord.releaseTime, &(pTcamelBill->end_year));			
        }
	}

    if (GET_CDR_FIELD_FILTER(callDuration, TermCAMELRecord))
    {
        pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_callDuration_present_byte]
            |= CMCC_CS_TermCAMELRecord_callDuration_present;
        pDestBill->u.termCAMELRecord.callDuration = pTcamelBill->callDuration;
    }

    if (GET_CDR_FIELD_FILTER(causeForTerm, TermCAMELRecord))
    {
        pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_causeForTerm_present_byte]
            |= CMCC_CS_TermCAMELRecord_causeForTerm_present;
        pDestBill->u.termCAMELRecord.causeForTerm = pTcamelBill->causeForTerm;
    }

	if (GET_CDR_FIELD_FILTER(diagnostics, TermCAMELRecord))			//diagnostics
	{
        pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_diagnostics_present_byte] |=
            CMCC_CS_TermCAMELRecord_diagnostics_present;
		if (pTcamelBill->diagnostics.CauseType == 0)
		{
			pDestBill->u.termCAMELRecord.diagnostics.choice = CMCC_CS_gsm0408Cause_chosen;
			pDestBill->u.termCAMELRecord.diagnostics.u.gsm0408Cause = pTcamelBill->diagnostics.Cause.gsm0408Cause;
		}
		else if (pTcamelBill->diagnostics.CauseType == 1)
		{
			pDestBill->u.termCAMELRecord.diagnostics.choice = CMCC_CS_gsm0902MapErrorValue_chosen;
			pDestBill->u.termCAMELRecord.diagnostics.u.gsm0902MapErrorValue = pTcamelBill->diagnostics.Cause.gsm0902MapErrorValue;
		}
		else if (pTcamelBill->diagnostics.CauseType == 2)
		{
			pDestBill->u.termCAMELRecord.diagnostics.choice = CMCC_CS_ccittQ767Cause_chosen;
			pDestBill->u.termCAMELRecord.diagnostics.u.ccittQ767Cause = pTcamelBill->diagnostics.Cause.ccittQ767Cause;
		}
		else
		{
			pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_diagnostics_present_byte] &=
                ~CMCC_CS_TermCAMELRecord_diagnostics_present;
		}
	}

    if (GET_CDR_FIELD_FILTER(callReference, TermCAMELRecord))
    {
        if (pTcamelBill->call_reference_len != 0)
        {
            int nCallReferenceLen = min(pTcamelBill->call_reference_len,
                sizeof(pDestBill->u.termCAMELRecord.callReference.value));
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_callReference_present_byte]
                |= CMCC_CS_TermCAMELRecord_callReference_present;                       
            pDestBill->u.termCAMELRecord.callReference.length = nCallReferenceLen;
            memcpy(pDestBill->u.termCAMELRecord.callReference.value, 
                &(pTcamelBill->call_reference[sizeof(pTcamelBill->call_reference) - pTcamelBill->call_reference_len]), nCallReferenceLen);   
        }
        else
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_callReference_present_byte]
                |= CMCC_CS_TermCAMELRecord_callReference_present;            
            GetCallReference(pDestBill->u.termCAMELRecord.callReference,
                pTcamelBill->ModuleNum, pTcamelBill->count);
        }
    }

	if (GET_CDR_FIELD_FILTER(sequenceNumber, TermCAMELRecord))		//部分话单序列号
	{
		if (pTcamelBill->record_type != SINGLE_BILL)
		{
			pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_sequenceNumber_present_byte] |=
                CMCC_CS_TermCAMELRecord_sequenceNumber_present;
			pDestBill->u.termCAMELRecord.sequenceNumber = pTcamelBill->sequence;
		}
	}

    if (GET_CDR_FIELD_FILTER(numberOfDPEncountered, TermCAMELRecord))		//numberOfDPEncountered
	{
		if (pTcamelBill->numberOfDPEncountered != 0)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_numberOfDPEncountered_present_byte] |=
                CMCC_CS_TermCAMELRecord_numberOfDPEncountered_present;
		    pDestBill->u.termCAMELRecord.numberOfDPEncountered = pTcamelBill->numberOfDPEncountered;
        }
	}

    if (GET_CDR_FIELD_FILTER(levelOfCAMELService, TermCAMELRecord))		//numberOfDPEncountered
	{
        if (pTcamelBill->levelOfCAMELService != 0xFF)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_levelOfCAMELService_present_byte] |=
                CMCC_CS_TermCAMELRecord_levelOfCAMELService_present;
            switch(pTcamelBill->levelOfCAMELService)
            {
            case 0:
                pDestBill->u.termCAMELRecord.levelOfCAMELService = CMCC_CS_basic;
                break;
            case 1:
                pDestBill->u.termCAMELRecord.levelOfCAMELService = CMCC_CS_callDurationSupervision;
                break;
            case 2:
                pDestBill->u.termCAMELRecord.levelOfCAMELService = CMCC_CS_onlineCharging;
                break;
            default:
                pDestBill->u.termCAMELRecord.levelOfCAMELService = CMCC_CS_basic;
                break;
            }
        }
	}

    if (GET_CDR_FIELD_FILTER(freeFormatData, TermCAMELRecord))		//自由格式数据
	{
        if (pTcamelBill->fcidata_len != 0)
        {
            int fci_len = min(sizeof(pDestBill->u.termCAMELRecord.freeFormatData.value), pTcamelBill->fcidata_len);
            memcpy(pDestBill->u.termCAMELRecord.freeFormatData.value, pTcamelBill->VarPart, fci_len);
		    pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_freeFormatData_present_byte] |=
                CMCC_CS_TermCAMELRecord_freeFormatData_present;
            pDestBill->u.termCAMELRecord.freeFormatData.length = fci_len;
        }
	}

    pDestBill->u.termCAMELRecord.cAMELCallLegInformation = NULL;

    if (GET_CDR_FIELD_FILTER(freeFormatDataAppend, TermCAMELRecord))		//freeFormatDataAppend
	{
        if ((AppendIndication)pTcamelBill->freeFormatDataAppend == Append)
        {
		    pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_freeFormatDataAppend_present_byte] |=
                CMCC_CS_TermCAMELRecord_freeFormatDataAppend_present;
            pDestBill->u.termCAMELRecord.freeFormatDataAppend = TRUE;
        }
        else
        {
		    pDestBill->u.termCAMELRecord.freeFormatDataAppend = FALSE;     
        }
	}

    if (GET_CDR_FIELD_FILTER(defaultCallHandling_2, TermCAMELRecord))		//defaultCallHandling2
	{
        if (pTcamelBill->defaultCallHandling2 == 0 || pTcamelBill->defaultCallHandling2 == 1)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_defaultCallHandling_2_present_byte] |=
                CMCC_CS_TermCAMELRecord_defaultCallHandling_2_present;
		    if (pTcamelBill->defaultCallHandling2 == 0)
            {
                pDestBill->u.termCAMELRecord.defaultCallHandling_2 = CMCC_CS_continueCall;
            }
            else
            {
                pDestBill->u.termCAMELRecord.defaultCallHandling_2 = CMCC_CS_releaseCall;
            }
        }
	}

    if (GET_CDR_FIELD_FILTER(gsm_SCFAddress_2, TermCAMELRecord))		//gsm_SCFAddress2
	{
	    int nGSMSCFAddress2Len = (pTcamelBill->CAMEL_GsmSCF_Address2_BCDlen + 1) / 2;
        if (nGSMSCFAddress2Len > 0)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_gsm_SCFAddress_2_present_byte] |=
                CMCC_CS_TermCAMELRecord_gsm_SCFAddress_2_present;  
            pDestBill->u.termCAMELRecord.gsm_SCFAddress_2.length = 
                min(sizeof(pDestBill->u.termCAMELRecord.gsm_SCFAddress_2.value), nGSMSCFAddress2Len + 1);
            pDestBill->u.termCAMELRecord.gsm_SCFAddress_2.value[0] = *(pTcamelBill->CAMEL_GsmSCF_Address2 - 2) | 0x80; 
            BCDtoRBCD(pDestBill->u.termCAMELRecord.gsm_SCFAddress_2.value + 1, pTcamelBill->CAMEL_GsmSCF_Address2, 
                pDestBill->u.termCAMELRecord.gsm_SCFAddress_2.length - 1);
        }
	}

    if (GET_CDR_FIELD_FILTER(serviceKey_2, TermCAMELRecord))		//业务键2
	{
		if (pTcamelBill->ServiceKey2 != 0xFFFFFFFF)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_serviceKey_2_present_byte] |=
                CMCC_CS_TermCAMELRecord_serviceKey_2_present;
		    pDestBill->u.termCAMELRecord.serviceKey_2 = pTcamelBill->ServiceKey2;
        }
	}

    if (GET_CDR_FIELD_FILTER(freeFormatData_2, TermCAMELRecord))		//自由格式数据
	{
        if (pTcamelBill->fcidata2_len != 0)
        {
            int fci2_len = min(sizeof(pDestBill->u.termCAMELRecord.freeFormatData_2.value), pTcamelBill->fcidata2_len);
            memcpy(pDestBill->u.termCAMELRecord.freeFormatData_2.value + pTcamelBill->fcidata_len, &(pTcamelBill->fcidata2_len), fci2_len);
		    pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_freeFormatData_2_present_byte] |=
                CMCC_CS_TermCAMELRecord_freeFormatData_2_present;
            pDestBill->u.termCAMELRecord.freeFormatData_2.length = fci2_len;
        }
	}

    if (GET_CDR_FIELD_FILTER(freeFormatDataAppend_2, TermCAMELRecord))		//freeFormatDataAppend
	{
        if ((AppendIndication)pTcamelBill->freeFormatDataAppend2 == Append)
        {
		    pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_freeFormatDataAppend_2_present_byte] |=
                CMCC_CS_TermCAMELRecord_freeFormatDataAppend_2_present;
            pDestBill->u.termCAMELRecord.freeFormatDataAppend_2 = TRUE;
        }
        else
        {
		    pDestBill->u.termCAMELRecord.freeFormatDataAppend_2 = FALSE;     
        }
	}

    if (GET_CDR_FIELD_FILTER(orgMSCId, TermCAMELRecord))
	{
        if (pTcamelBill->msc_id_len != 0)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_orgMSCId_present_byte] |=
                CMCC_CS_TermCAMELRecord_orgMSCId_present;
            pDestBill->u.termCAMELRecord.orgMSCId.length = min(3,pTcamelBill->msc_id_len);
            memcpy(pDestBill->u.termCAMELRecord.orgMSCId.value, pTcamelBill->orgMSCId, pDestBill->u.termCAMELRecord.orgMSCId.length);
        }
    }

    if (GET_CDR_FIELD_FILTER(subscriberCategory, TermCAMELRecord))
	{
        pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_subscriberCategory_present_byte] |=
            CMCC_CS_TermCAMELRecord_subscriberCategory_present;
        pDestBill->u.termCAMELRecord.subscriberCategory.length = 1;
        pDestBill->u.termCAMELRecord.subscriberCategory.value[0] = pTcamelBill->subscriberCategory;
    }

    if (GET_CDR_FIELD_FILTER(partialRecordType, TermCAMELRecord))      //1
    {
        if (pTcamelBill->cause_for_partial_record != 0xff)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_partialRecordType_present_byte] |=
                CMCC_CS_TermCAMELRecord_partialRecordType_present;
            pDestBill->u.termCAMELRecord.partialRecordType = (CMCC_CS_PartialRecordType)pTcamelBill->cause_for_partial_record;
        }
    }    
    
    pDestBill->u.termCAMELRecord.cAMELCallLegInformation = NULL;
    
    if (GET_CDR_FIELD_FILTER(basicService, TermCAMELRecord))			//电话或承载业务码
    {
        if (pTcamelBill->TBS < 2)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_basicService_present_byte] |=
                CMCC_CS_TermCAMELRecord_basicService_present;
            ConvertBasicServiceCode(pDestBill->u.termCAMELRecord.basicService, 
                pTcamelBill->TBS, pTcamelBill->ServiceCode);
        }
    }

    if (GET_CDR_FIELD_FILTER(additionalChgInfo, TermCAMELRecord))		//附加计费信息
    {
        if (pTcamelBill->chargeIndicator >= 0 && pTcamelBill->chargeIndicator < 3)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_additionalChgInfo_present_byte] |=
                CMCC_CS_TermCAMELRecord_additionalChgInfo_present;
            pDestBill->u.termCAMELRecord.additionalChgInfo.bit_mask = CMCC_CS_chargeIndicator_present;
            pDestBill->u.termCAMELRecord.additionalChgInfo.chargeIndicator = pTcamelBill->chargeIndicator;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(chargedParty,TermCAMELRecord) )
    {
        if (pTcamelBill->chargedParty == 0 || pTcamelBill->chargedParty == 1)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_chargedParty_present_byte] |= 
                CMCC_CS_TermCAMELRecord_chargedParty_present;
            pDestBill->u.termCAMELRecord.chargedParty = (CMCC_CS_ChargedParty)pTcamelBill->chargedParty;
        }
    }
    
    pDestBill->u.termCAMELRecord.originalCalledNumber.length = 0;
    pDestBill->u.termCAMELRecord.originalCalledNumber.value = NULL;
    if (GET_CDR_FIELD_FILTER(originalCalledNumber,TermCAMELRecord))
    {
        int nOriginalCalledLen = (pTcamelBill->originalCalledNumber_BCDlen + 1)/2;
        if (nOriginalCalledLen > 0 )
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_originalCalledNumber_present_byte] |=
                CMCC_CS_TermCAMELRecord_originalCalledNumber_present;
            pDestBill->u.termCAMELRecord.originalCalledNumber.length = nOriginalCalledLen + 1;
            pDestBill->u.termCAMELRecord.originalCalledNumber.value = new (g_pAsnBuffForCMCC)BYTE[nOriginalCalledLen + 1];                                    
            pDestBill->u.termCAMELRecord.originalCalledNumber.value[0] = *(pTcamelBill->originalCalledNumber - 2) | 0x80;
            BCDtoRBCD(pDestBill->u.termCAMELRecord.originalCalledNumber.value + 1,pTcamelBill->originalCalledNumber,
                pDestBill->u.termCAMELRecord.originalCalledNumber.length - 1);
        }
    }
    
    if (GET_CDR_FIELD_FILTER(hotBillingTag, TermCAMELRecord))		//热计费标识
    {
        if (pTcamelBill->HotBillingTag == HOT_BILLING)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_TermCAMELRecord_hotBillingTag_present_byte] |=
                CMCC_CS_TermCAMELRecord_hotBillingTag_present;
            pDestBill->u.termCAMELRecord.hotBillingTag = CMCC_CS_hotBilling;
        }
        else
        {
            pDestBill->u.termCAMELRecord.hotBillingTag = CMCC_CS_noHotBilling;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(mscServerIndication,TermCAMELRecord))
    {
        if (pTcamelBill->mscServerIndication != FALSE)
        {
            pDestBill->u.termCAMELRecord.bit_mask[CMCC_CS_mscServerIndication_present_byte] |=
                CMCC_CS_mscServerIndication_present;
            pDestBill->u.termCAMELRecord.mscServerIndication = TRUE;
        }
    }
    return TRUE;
}

BOOL ConvertABill_COMMON_EQUIP(const BYTE* pSrcBill, const int nLen, const BYTE* pEventBill, 
                  const int nEventNum, CMCC_CS_CallEventRecord* pDestBill)
{
	CEU_Bill_Report* pCeuBill = (CEU_Bill_Report *)pSrcBill;
	
	pDestBill->choice = CMCC_CS_commonEquipRecord_chosen;
	pDestBill->u.commonEquipRecord.bit_mask = 0;

    if (GET_CDR_FIELD_FILTER(recordType, CommonEquipRecord))
    {
        pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_recordType_present;
	    pDestBill->u.commonEquipRecord.recordType = CMCC_CS_CallEventRecordType_commonEquipRecord;		//记录类型    
    }

    if (GET_CDR_FIELD_FILTER(equipmentType, CommonEquipRecord))
    {
        pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_equipmentType_present; 
        pDestBill->u.commonEquipRecord.equipmentType = pCeuBill->equipmentType;
    }

    if (GET_CDR_FIELD_FILTER(equipmentId, CommonEquipRecord))
    {
        pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_equipmentId_present; 
        pDestBill->u.commonEquipRecord.equipmentId = pCeuBill->equipmentId;
    }

    pDestBill->u.commonEquipRecord.servedIMSI.length = 0;
    if (GET_CDR_FIELD_FILTER(servedIMSI, CommonEquipRecord))
    {
        pDestBill->u.commonEquipRecord.servedIMSI.length = 
            BCDtoRBCD(pDestBill->u.commonEquipRecord.servedIMSI.value, pCeuBill->servedIMSI, 8);
        if(pDestBill->u.commonEquipRecord.servedIMSI.length > 0)
        {
            pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_servedIMSI_present;         
        }
    }

    pDestBill->u.commonEquipRecord.servedMSISDN.length = 0;
    if (GET_CDR_FIELD_FILTER(servedMSISDN, CommonEquipRecord))
	{
	    int nMSISDNLen = (pCeuBill->servedMSISDN_BCDlen + 1) / 2;
        if (nMSISDNLen > 0)
        {
		    pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_servedMSISDN_present; 
            pDestBill->u.commonEquipRecord.servedMSISDN.length = 
                min(sizeof(pDestBill->u.commonEquipRecord.servedMSISDN.value), nMSISDNLen + 1);
            pDestBill->u.commonEquipRecord.servedMSISDN.value[0] = *(pCeuBill->servedMSISDN - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.commonEquipRecord.servedMSISDN.value + 1, pCeuBill->servedMSISDN, 
                pDestBill->u.commonEquipRecord.servedMSISDN.length - 1);
        }
    }

    pDestBill->u.commonEquipRecord.recordingEntity.length = 0;
    if (GET_CDR_FIELD_FILTER(recordingEntity, CommonEquipRecord))
    {
	    int nServedMSCLen = (pCeuBill->recordingEntity_BCDlen + 1) / 2;
        if (nServedMSCLen > 0)
        {
	        pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_recordingEntity_present; 
		    pDestBill->u.commonEquipRecord.recordingEntity.length = 
                min(sizeof(pDestBill->u.commonEquipRecord.recordingEntity.value), nServedMSCLen + 1);
            pDestBill->u.commonEquipRecord.recordingEntity.value[0] = *(pCeuBill->recordingEntity - 2) | 0x80;
		    BCDtoRBCD(pDestBill->u.commonEquipRecord.recordingEntity.value + 1, pCeuBill->recordingEntity, 
                pDestBill->u.commonEquipRecord.recordingEntity.length - 1);
        }
    }

	if (GET_CDR_FIELD_FILTER(basicService, CommonEquipRecord))	//业务码
	{
        if (pCeuBill->TBS < 2 && pCeuBill->ServiceCode != 0xFF)
        {
            pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_basicService_present;
		    ConvertBasicServiceCode(pDestBill->u.commonEquipRecord.basicService, 
                pCeuBill->TBS, pCeuBill->ServiceCode);
        }
	}
    
	pDestBill->u.commonEquipRecord.changeOfService = NULL;
	if (GET_CDR_FIELD_FILTER(changeOfService, CommonEquipRecord))
	{
    }

	pDestBill->u.commonEquipRecord.supplServicesUsed = NULL;
	if (GET_CDR_FIELD_FILTER(supplServicesUsed, CommonEquipRecord))
	{
        if (pCeuBill->SSCode != 0xFF)
        {
            CMCC_CS__seqof3_* pSS = NULL;
            pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
            pSS->next = pDestBill->u.commonEquipRecord.supplServicesUsed;
            pSS->value.ssCode.length = 1;
            pSS->value.ssCode.value[0] = pCeuBill->SSCode;
            pSS->value.bit_mask = CMCC_CS_ssTime_present;
            pSS->value.bit_mask |= CMCC_CS_ssCode_present;
            ConvertTimeStamp(pSS->value.ssTime, &(pCeuBill->initial_year));
            pDestBill->u.commonEquipRecord.supplServicesUsed = pSS;
		    pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_supplServicesUsed_present;
        }
	}

    if (GET_CDR_FIELD_FILTER(seizureTime, CommonEquipRecord))
    {
        pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_seizureTime_present; 
        ConvertTimeStamp(pDestBill->u.commonEquipRecord.seizureTime, &(pCeuBill->initial_year));
    }

	if (GET_CDR_FIELD_FILTER(releaseTime, CommonEquipRecord))
	{
		if (pCeuBill->initial_year != 0xFF)
        {
            pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_releaseTime_present;

		    struct tm ts;
		    struct tm *pte;
            time_t te;
            BYTE arEndTime[6];

		    ts.tm_year = pCeuBill->initial_year + 2000 - 1900;
		    ts.tm_mon  = pCeuBill->initial_month - 1;
		    ts.tm_mday = pCeuBill->initial_date;
		    ts.tm_hour = pCeuBill->initial_hour;
		    ts.tm_min  = pCeuBill->initial_minute;
		    ts.tm_sec  = pCeuBill->initial_second;
            
            te = mktime(&ts) + pCeuBill->callDuration;
		    pte = localtime(&te);

            arEndTime[0] = pte->tm_year + 1900 - 2000;
            arEndTime[1] = pte->tm_mon + 1;
            arEndTime[2] = pte->tm_mday;
            arEndTime[3] = pte->tm_hour;
            arEndTime[4] = pte->tm_min;
            arEndTime[5] = pte->tm_sec;

            ConvertTimeStamp(pDestBill->u.commonEquipRecord.releaseTime, arEndTime);
        }
    }

    if (GET_CDR_FIELD_FILTER(callDuration, CommonEquipRecord))
    {
        pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_callDuration_present; 
        pDestBill->u.commonEquipRecord.callDuration = pCeuBill->callDuration;
    }

    if (GET_CDR_FIELD_FILTER(callReference, CommonEquipRecord))
    {
        if (pCeuBill->call_reference_len != 0)
        {
            int nCallReferenceLen = min(pCeuBill->call_reference_len,
                sizeof(pDestBill->u.commonEquipRecord.callReference.value));
            pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_callReference_present;                      
            pDestBill->u.commonEquipRecord.callReference.length = nCallReferenceLen;
            memcpy(pDestBill->u.commonEquipRecord.callReference.value, 
                &(pCeuBill->call_reference[sizeof(pCeuBill->call_reference) - pCeuBill->call_reference_len]), nCallReferenceLen);   
        }
        else
        {
            pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_callReference_present; 
            GetCallReference(pDestBill->u.commonEquipRecord.callReference,
                pCeuBill->ModuleNum, pCeuBill->count);
        }
    }

	if (GET_CDR_FIELD_FILTER(sequenceNumber, CommonEquipRecord))
	{
        if(pCeuBill->record_type != SINGLE_BILL)
        {
            pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_sequenceNumber_present;
            pDestBill->u.commonEquipRecord.sequenceNumber = pCeuBill->sequence;
        }        
    }

	pDestBill->u.commonEquipRecord.recordExtensions = NULL;			//recordExtensions(目前不支持)
	if (GET_CDR_FIELD_FILTER(recordExtensions, CommonEquipRecord))		
	{
	}

	if (GET_CDR_FIELD_FILTER(systemType, CommonEquipRecord))
	{
        switch((ACCESS_NETWORK_TYPE)pCeuBill->systemType)
        {
        case ACCESS_BSC :
            pDestBill->u.commonEquipRecord.systemType = CMCC_CS_gERAN;
            pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_systemType_present;
            break;
        case ACCESS_RNC :
            pDestBill->u.commonEquipRecord.systemType = CMCC_CS_iuUTRAN;
            pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_systemType_present;
            break;
        case ACCESS_UNKNOWN:
            pDestBill->u.commonEquipRecord.systemType = CMCC_CS_unknown;
            pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_systemType_present;
            break;
        }
    }

	if (GET_CDR_FIELD_FILTER(rateIndication, CommonEquipRecord))
	{
        //pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_rateIndication_present;
    }

	if (GET_CDR_FIELD_FILTER(fnur, CommonEquipRecord))
	{
        //pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_fnur_present;
    }

    pDestBill->u.commonEquipRecord.orgRNCorBSCId.length = 0;
    if (GET_CDR_FIELD_FILTER(orgRNCorBSCId, CommonEquipRecord))
    {
        //如果Rnc_Bsc_id全0，则不进行编码
        if (pCeuBill->rnc_bsc_id_len != 0)
        {
            pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_orgRNCorBSCId_present;
            pDestBill->u.commonEquipRecord.orgRNCorBSCId.length = min(3,pCeuBill->rnc_bsc_id_len);
            memcpy(pDestBill->u.commonEquipRecord.orgRNCorBSCId.value, pCeuBill->orgRNCorBSCId, pDestBill->u.commonEquipRecord.orgRNCorBSCId.length);
        }
    }

    if (GET_CDR_FIELD_FILTER(orgMSCId, CommonEquipRecord))
	{
        if (pCeuBill->msc_id_len != 0)
        {
            pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_orgMSCId_present;
            pDestBill->u.commonEquipRecord.orgMSCId.length = min(3,pCeuBill->msc_id_len);
            memcpy(pDestBill->u.commonEquipRecord.orgMSCId.value, pCeuBill->orgMSCId, pDestBill->u.commonEquipRecord.orgMSCId.length);
        }
    }

    if (GET_CDR_FIELD_FILTER(subscriberCategory, CommonEquipRecord))
	{
        pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_subscriberCategory_present;
        pDestBill->u.commonEquipRecord.subscriberCategory.length = 1;
        pDestBill->u.commonEquipRecord.subscriberCategory.value[0] = pCeuBill->subscriberCategory;
    }

    if (GET_CDR_FIELD_FILTER(partialRecordType, CommonEquipRecord))     //1
    {
        if (pCeuBill->cause_for_partial_record != 0xff)
        {
            pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_partialRecordType_present;
            pDestBill->u.commonEquipRecord.partialRecordType = (CMCC_CS_PartialRecordType)pCeuBill->cause_for_partial_record;
        }
    }      
    if (GET_CDR_FIELD_FILTER(causeForTerm, CommonEquipRecord))      //2
    {
        if (pCeuBill->causeForTerm != 0xff)
        {
            pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_causeForTerm_present ;
            pDestBill->u.commonEquipRecord.causeForTerm = pCeuBill->causeForTerm;
        }
    }
    
    if (GET_CDR_FIELD_FILTER(diagnostics, CommonEquipRecord))      //3
    {

            pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_diagnostics_present;
            if (pCeuBill->diagnostics.CauseType == 0)
            {
                pDestBill->u.commonEquipRecord.diagnostics.choice = CMCC_CS_gsm0408Cause_chosen;
                pDestBill->u.commonEquipRecord.diagnostics.u.gsm0408Cause = pCeuBill->diagnostics.Cause.gsm0408Cause;
            }
            else if (pCeuBill->diagnostics.CauseType == 1)
            {
                pDestBill->u.commonEquipRecord.diagnostics.choice = CMCC_CS_gsm0902MapErrorValue_chosen;
                pDestBill->u.commonEquipRecord.diagnostics.u.gsm0902MapErrorValue = pCeuBill->diagnostics.Cause.gsm0902MapErrorValue;
            }
            else if (pCeuBill->diagnostics.CauseType == 2)
            {
                pDestBill->u.commonEquipRecord.diagnostics.choice = CMCC_CS_ccittQ767Cause_chosen;
                pDestBill->u.commonEquipRecord.diagnostics.u.ccittQ767Cause = pCeuBill->diagnostics.Cause.ccittQ767Cause;
            }
            else
            {
                pDestBill->u.commonEquipRecord.bit_mask &= 
                    ~CMCC_CS_CommonEquipRecord_diagnostics_present;
            }
    }
    
    if (GET_CDR_FIELD_FILTER(servedIMEI, CommonEquipRecord))      //4
    {
        pDestBill->u.commonEquipRecord.servedIMEI.length = 
            BCDtoRBCD(pDestBill->u.commonEquipRecord.servedIMEI.value, pCeuBill->servedIMEI, 8);
        if (pDestBill->u.commonEquipRecord.servedIMEI.length > 0)
        {
            pDestBill->u.commonEquipRecord.bit_mask |= CMCC_CS_CommonEquipRecord_servedIMEI_present;
        }
    }
    
    return TRUE;
}

void ConvertTimeStamp(CMCC_CS_TimeStamp& ts, const BYTE* pTime)
{
	char szTemp[64]          = "";
	unsigned char szBCD[64]  = "";

	ts.length = 9;
	sprintf(szTemp, "%02d%02d%02d%02d%02d%02d", 
            pTime[0], pTime[1], pTime[2], pTime[3], pTime[4], pTime[5]);	
	StringtoBCD(szTemp, szBCD);
	memcpy(ts.value, szBCD, 6);

    //by    杨善    2004-6-9    SWPD05807
    tzset();
    ts.value[6] = '+';
#ifdef _PLATFORM_WIN32
    if (timezone > 0)
    {
        ts.value[6] = '-';       //时区差
    }
#else
    if (timezone < 0) 
    {
        ts.value[6] = '-';
    }
#endif
    ts.value[7] = abs(timezone / 3600);     
    //the end    2004-6-9   SWPD05807
    
    ts.value[8] = 0x00;
}

void ConvertCauseForTerm(int& nOutCause, const int nInCause)
{
	switch (nInCause)
	{
		case 0x90:		//正常呼叫清除
		{
			nOutCause = CMCC_CS_normalRelease;
			break;
		}

		case 0x46:		//部分话单
        case 0xFD:		//部分话单
		{
			nOutCause = CMCC_CS_partialRecord;
			break;
		}

		default:
		{
			nOutCause = CMCC_CS_stableCallAbnormalTermination;
			break;
		}
	}
}

void ConvertBasicServiceCode(CMCC_CS_BasicServiceCode& bsc, const BYTE nServiceCode, const BYTE TeleBearCode)
{
	if (nServiceCode == MBILL_TELECOM_SERVICE)
	{
		bsc.choice = CMCC_CS_BasicServiceCode_teleservice_chosen;
		bsc.u.teleservice.length   = 1;
		bsc.u.teleservice.value[0] = TeleBearCode;
	}
	else
	{
		bsc.choice = CMCC_CS_BasicServiceCode_bearerService_chosen;
		bsc.u.bearerService.length   = 1;
		bsc.u.bearerService.value[0] = TeleBearCode;
	}
}

/************************************************************************
 函数名称：BCDtoString
 函数功能：将BCD码串转换为带尾0的字符串
 输入参数：
 const BYTE* pSrcBCD  : 待转换的BCD码串
 char* szDstStr       : 转换后的字符串
 UINT4 uSrcLen        : 待转换的BCD码串的长度
 UINT4 uDstLen        : 转换后的字符串的长度，缺省为0，表示不限制
 返 回 值：转换后字符串的长度，不含尾0
 //Added by ZhengYuqun 2003-08-27 SWPD01198
*************************************************************************/
UINT4 BCDtoString(const BYTE* pSrcBCD, char* szDstStr, UINT4 uSrcLen, 
                  UINT4 uDstLen)
{
    BYTE yCurBcd;
    char cCurChar;
    UINT4 i = 0, j = 0;

    assert(NULL != pSrcBCD && NULL != szDstStr);

    for(; i < uSrcLen; i++)
    {
        yCurBcd = *(pSrcBCD + i);
        
        if (yCurBcd < 0xA0)
        {
            cCurChar = (yCurBcd >> 4) + '0';
        }
        else
        {
            break;
        }

        if (0 == uDstLen || (0 != uDstLen && j + 1 <= uDstLen)) //如果对目标串有长度限制
        {
            szDstStr[j++] = cCurChar;
        }
        else
        {
            break;
        }

        yCurBcd &= 0x0F;

        if (yCurBcd < 0x0A)
        {
            cCurChar = yCurBcd + '0';
        }
        else
        {
            break;
        }

        if (0 == uDstLen || (0 != uDstLen && j + 1 <= uDstLen)) //如果对目标串有长度限制
        {
            szDstStr[j++] = cCurChar;
        }
        else
        {
            break;
        }

    }
    szDstStr[j] = '\0';

    return j;
}


/************************************************************************
 函数名称：StringtoBCD
 函数功能：将带尾0的字符串转换为BCD码串
 输入参数：
 const char* szSrcStr   : 待转换的字符串，如果不带尾0，必须指定puSrcLen参数
 BYTE* pDstBCD          : 待转换的BCD码串
 UINT4 uDstLen          : 转换后的BCD码串的长度，缺省为0，表示不限制
 UINT4 uSrcLen          : 待转换字符串的长度，缺省为0
 返 回 值：BCD码串缓冲区被写过的字节数
 //Added by ZhengYuqun 2003-08-27 SWPD01198
*************************************************************************/
UINT4 StringtoBCD(const char* szSrcStr, BYTE* pDstBCD, UINT4 uDstLen, UINT4 uSrcLen)
{
    int i = 0, j = 0;
    char cCurChar;
    UINT4 uSrcLength;
    
    assert(NULL != szSrcStr && NULL != pDstBCD);
    
    uSrcLength = strlen(szSrcStr);
    if (0 != uSrcLen)
    {
        uSrcLength = min(uSrcLength, uSrcLen);
    }

    if (0 != uDstLen)
    {
        memset(pDstBCD, 0xFF, uDstLen); //先将结果初始化为0xFF
    }

    for(; i < uSrcLength; i++)
    {
        if (0 != uDstLen && j == uDstLen)
        {
            break;
        }
        
        if (szSrcStr[i] >= 'a' && szSrcStr[i] <= 'f')  //将小写的a到f转换成大写的
        {
            cCurChar = szSrcStr[i] + ('A' - 'a');
        }
        else
        {
            cCurChar = szSrcStr[i];
        }

        assert( (cCurChar >= '0' && cCurChar <= '9') || (cCurChar >= 'A' && cCurChar <= 'F'));

        BYTE yCurVal = 0;
        if (cCurChar >= '0' && cCurChar <= '9')
        {
            yCurVal = cCurChar - '0';
        }
        else if (cCurChar >= 'A' && cCurChar <= 'F')
        {
            yCurVal = cCurChar - 'A' + 0x0A;
        }
       
        if (i % 2 == 0)
        {
            ((BYTE*)pDstBCD)[j] = yCurVal << 4;
            ((BYTE*)pDstBCD)[j] |= 0x0F;
        }
        else
        {
            ((BYTE*)pDstBCD)[j] &= yCurVal | 0xF0;
            j++;
        }
    }

    if (i % 2 != 0)
    {
        j++;
    }
    if (0 != uDstLen)
    {
        j = uDstLen;
    }

    return j;
}

int BCDtoRBCD(BYTE* pRBCD, BYTE* pBCD, const int nLen)
{
    int i = 0;
    int nActLen = 0;

	//计算真实的长度
    for (; i < nLen; i++)
	{
        if ((pBCD[i] & 0x0F) == 0x0F || (pBCD[i] & 0xF0) == 0xF0)
        {
            break;
        }    
    }
    
    //考虑最后一个字节为 0xFF 的情况
    if ((pBCD[i] & 0xF0) == 0xF0)
    {
        nActLen = i;
    }
    else
    {
        nActLen = i + 1;
    }

    if(nActLen > nLen)
    {
        nActLen = nLen;
    }

    for (i = 0; i < nActLen; i++)
	{
        pRBCD[i] = pBCD[i] << 4 | pBCD[i] >> 4;
	}

    return nActLen;
}

void GetCallReference(CMCC_CS_CallReference& callReference, const unsigned char nModuleNum, const int nCount)
{
    //低字节为模块号，高4字节为首话单流水号
    int nTemp = HTONL(nCount);
    callReference.value[0] = nModuleNum;
    memcpy(&callReference.value[1], &nTemp, 4);
    callReference.length = 5;
}

void ConvertSpeechVersion(BYTE &nOutValue, const int nInValue)
{
    switch(nInValue)
    {
    case 0:
        nOutValue = 1;
        break;
    case 1:
        nOutValue = 5;
        break;
    case 2:
        nOutValue = 17;
        break;
    case 3:
        nOutValue = 21;
        break;
    case 4:
        nOutValue = 33;
        break;
    case 5:
        nOutValue = 37;
        break;
    default:
        nOutValue = 0xFF;
    }
    
    return;
}

BOOL IsLACandCellIDValid(BYTE* LAC, BYTE* CellID)
{
    //LAC和CellID全等于0和全等于F的时候无效
    if( (LAC[0] == 0xFF && LAC[1] == 0xFF && CellID[0] == 0xFF && CellID[1] == 0xFF)
        || (LAC[0] == 0 && LAC[1] == 0 && CellID[0] == 0 && CellID[1] == 0))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

void ConvertSScode(BYTE* pSScode, BYTE* pDateTime, CMCC_CS__seqof3_** pDest)
{
    //MBILL_SS_code1
    if (pSScode[0] != 0xFF) 
    {
        if (((MBILL_SS_code1*)pSScode)->CFU == 0) 
        {
            Link_SScode(pDateTime,pDest,SS_CFU);             
        }        
        else if (((MBILL_SS_code1*)pSScode)->CFB == 0)
        {
            Link_SScode(pDateTime,pDest,SS_CFB);       
        }
        else if (((MBILL_SS_code1*)pSScode)->CFNRY == 0) 
        {
            Link_SScode(pDateTime,pDest,SS_CFNRY);             
        } 
        else if (((MBILL_SS_code1*)pSScode)->CFNRC == 0)
        {
            Link_SScode(pDateTime,pDest,SS_CFNRC);       
        }
        else if (((MBILL_SS_code1*)pSScode)->ECT == 0)
        {
            Link_SScode(pDateTime,pDest,SS_ECT);       
        }
    }

    //MBILL_SS_code2
    if (pSScode[1] != 0xFF)
    {
        if (((MBILL_SS_code2*)pSScode + 1)->CD == 0) 
        {
            Link_SScode(pDateTime,pDest,SS_CD);             
        }
        
        if (((MBILL_SS_code2*)pSScode + 1)->CCBSA == 0)
        {
            Link_SScode(pDateTime,pDest,SS_CCBSA);       
        }
        
        if (((MBILL_SS_code2*)pSScode + 1)->CCBSB == 0) 
        {
            Link_SScode(pDateTime,pDest,SS_CCBSB);             
        }
        
        if (((MBILL_SS_code2*)pSScode + 1)->MC == 0)
        {
            Link_SScode(pDateTime,pDest,SS_MC);       
        }
        
        if (((MBILL_SS_code2*)pSScode + 1)->EMLPP == 0) 
        {
            Link_SScode(pDateTime,pDest,SS_EMLPP);             
        }
        
        if (((MBILL_SS_code2*)pSScode + 1)->CUG == 0)
        {
            Link_SScode(pDateTime,pDest,SS_CUG);       
        }
        
        if (((MBILL_SS_code2*)pSScode + 1)->AOCI == 0) 
        {
            Link_SScode(pDateTime,pDest,SS_AOCI);             
        }
        
        if (((MBILL_SS_code2*)pSScode + 1)->AOCC == 0)
        {
            Link_SScode(pDateTime,pDest,SS_AOCC);       
        }
    }
 

    //MBILL_SS_code3
    if (pSScode[2] != 0xFF) 
    {  
        if (((MBILL_SS_code3*)pSScode + 2)->UUS1 == 0) 
        {
            Link_SScode(pDateTime,pDest,SS_UUS1);             
        }
    
        if (((MBILL_SS_code3*)pSScode + 2)->UUS2 == 0)
        {
            Link_SScode(pDateTime,pDest,SS_UUS2);       
        }
    
        if (((MBILL_SS_code3*)pSScode + 2)->UUS3 == 0) 
        {
            Link_SScode(pDateTime,pDest,SS_UUS3);             
        }
    
        if (((MBILL_SS_code3*)pSScode + 2)->ALLMOLR == 0)
        {
            Link_SScode(pDateTime,pDest,SS_ALLMOLR);       
        }
    
        if (((MBILL_SS_code3*)pSScode + 2)->CLIP == 0) 
        {
            Link_SScode(pDateTime,pDest,SS_CLIP);             
        }
    
        if (((MBILL_SS_code3*)pSScode + 2)->CLIR == 0)
        {
            Link_SScode(pDateTime,pDest,SS_CLIR);       
        }
    }

    
    //MBILL_SS_code4
    if (pSScode[3] != 0xFF)
    {
        if (((MBILL_SS_code4*)pSScode + 3)->plmn_specificSS0 == 0) 
        {
            Link_SScode(pDateTime,pDest,SS_PLMN_SPECIFICSS0);             
        }
        
        if (((MBILL_SS_code4*)pSScode + 3)->plmn_specificSS1 == 0)
        {
            Link_SScode(pDateTime,pDest,SS_PLMN_SPECIFICSS1);       
        }
        
        if (((MBILL_SS_code4*)pSScode + 3)->plmn_specificSS2 == 0) 
        {
            Link_SScode(pDateTime,pDest,SS_PLMN_SPECIFICSS2);             
        }
        
        if (((MBILL_SS_code4*)pSScode + 3)->plmn_specificSS3 == 0)
        {
            Link_SScode(pDateTime,pDest,SS_PLMN_SPECIFICSS3);       
        }
        
        if (((MBILL_SS_code4*)pSScode + 3)->plmn_specificSS4 == 0) 
        {
            Link_SScode(pDateTime,pDest,SS_PLMN_SPECIFICSS4);             
        }
        
        if (((MBILL_SS_code4*)pSScode + 3)->plmn_specificSS5 == 0)
        {
            Link_SScode(pDateTime,pDest,SS_PLMN_SPECIFICSS5);       
        }
        
        if (((MBILL_SS_code4*)pSScode + 3)->plmn_specificSS6 == 0) 
        {
            Link_SScode(pDateTime,pDest,SS_PLMN_SPECIFICSS6);             
        }
        
        if (((MBILL_SS_code4*)pSScode + 3)->plmn_specificSS7 == 0)
        {
            Link_SScode(pDateTime,pDest,SS_PLMN_SPECIFICSS7);       
        }
    }

    //MBILL_SS_code5
    if (pSScode[4] != 0xFF)
    {
        if (((MBILL_SS_code5*)pSScode + 4)->plmn_specificSS8 == 0) 
        {
            Link_SScode(pDateTime,pDest,SS_PLMN_SPECIFICSS8);             
        }
        
        if (((MBILL_SS_code5*)pSScode + 4)->plmn_specificSS9 == 0)
        {
            Link_SScode(pDateTime,pDest,SS_PLMN_SPECIFICSS9);       
        }
        
        if (((MBILL_SS_code5*)pSScode + 4)->plmn_specificSS10 == 0) 
        {
            Link_SScode(pDateTime,pDest,SS_PLMN_SPECIFICSS10);             
        }
        
        if (((MBILL_SS_code5*)pSScode + 4)->plmn_specificSS11 == 0)
        {
            Link_SScode(pDateTime,pDest,SS_PLMN_SPECIFICSS11);       
        }
        
        if (((MBILL_SS_code5*)pSScode + 4)->plmn_specificSS12 == 0) 
        {
            Link_SScode(pDateTime,pDest,SS_PLMN_SPECIFICSS12);             
        }
        
        if (((MBILL_SS_code5*)pSScode + 4)->plmn_specificSS13 == 0)
        {
            Link_SScode(pDateTime,pDest,SS_PLMN_SPECIFICSS13);       
        }
        
        if (((MBILL_SS_code5*)pSScode + 4)->plmn_specificSS14== 0) 
        {
            Link_SScode(pDateTime,pDest,SS_PLMN_SPECIFICSS14);             
        }
        
        if (((MBILL_SS_code5*)pSScode + 4)->plmn_specificSS15 == 0)
        {
            Link_SScode(pDateTime,pDest,SS_PLMN_SPECIFICSS15);       
        }
    }

}

void Link_SScode(BYTE* pDateTime,CMCC_CS__seqof3_** pDest,const int nSS)
{
    CMCC_CS__seqof3_* pSS = new (g_pAsnBuffForCMCC)CMCC_CS__seqof3_;
    pSS->next = *pDest;
    pSS->value.ssCode.length = 1;
    pSS->value.ssCode.value[0] = nSS;
    pSS->value.bit_mask = CMCC_CS_ssTime_present;
    pSS->value.bit_mask |= CMCC_CS_ssCode_present;
    ConvertTimeStamp(pSS->value.ssTime, pDateTime);
    *pDest = pSS;
} 
}
