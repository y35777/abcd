/*****************************************************
*       R99_Bill.cpp
******************************************************
*Copyright 2003 by Huawei CO.,LTD.
* All rights reserved.
* Compiler:     Visual C++ 6.0
* Author:       GGF
* Date:         2003-09-28
* Description:  
* 
* Side Effects: NONE
* Functions:
* Notes:    
* 
* Update: 
* Date              Name              Description 
============== ================ ======================
## 
*****************************************************/
#include "r99_bill.h"
#include "../../merge/merge.h"
#include "../../include/export_dll.h"

extern "C" IGWB_Svc_Export void* CreateParserObject()
{
    return (void*)new CR99_BillParser();
}

CR99_BillParser::CR99_BillParser()
{
    m_pBill = NULL;
}

CR99_BillParser::~CR99_BillParser()
{
    if(m_pBill)
    {
        delete m_pBill;
    }
}

/*
*�������ɹ�������TRUE��ʧ�����׳�һ���쳣��
*���������Ͳ���ʶ���򷵻�FALSE��
*/
BOOL CR99_BillParser::ParseARecord(char* pBuf,UINT4 uBufLen)
{
    assert(pBuf != NULL);
    
    CMCC_3GCDR_CallEventRecord* pRec = (CMCC_3GCDR_CallEventRecord*)pBuf;
    switch(pRec->choice)
    {
        case CMCC_3GCDR_sgsnPDPRecord_chosen:
            m_pBill = new CR99_SCDR;

            if(m_pBill != NULL)
            {
                //�û�����¼��ʼ����������ʧ�ܽ��׳��쳣��
                if(m_pBill->Init((char*)&(pRec->u.sgsnPDPRecord),
                    uBufLen - sizeof(unsigned short)))
                {
                    return TRUE;
                }
            }

            throw "S-CDR initialize fail.";
            break;

        case CMCC_3GCDR_ggsnPDPRecord_chosen:
            m_pBill = new CR99_GCDR;
            if(m_pBill != NULL)
            {
                if(m_pBill->Init((char*)&(pRec->u.ggsnPDPRecord),
                    uBufLen - sizeof(unsigned short)))
                {
                    return TRUE;
                }
            }

            throw "G-CDR initialize fail.";
            break;
	    /*
        case CMCC_3GCDR_sgsnMMRecord_chosen:
            m_pBill = new CUpdatedR99_MCDR;
            if(m_pBill != NULL)
            {
                if(m_pBill->Init((char*)&(pRec->u.sgsnMMRecord),
                    uBufLen - sizeof(unsigned short)))
                {
                    return TRUE;
                }
            }

            throw "M-CDR initialize fail.";
            break;
        */
        //end

        default:
            return FALSE;
            break;
    }

    return TRUE;
}

CBill* CR99_BillParser::GetBillRecord()
{
    CBill* pBill = m_pBill;
    m_pBill = NULL;
    return pBill;
}

UINT4 CR99_BillParser::GetBillTypeCount()
{
    //3G��Ҫ�ϲ��Ļ����ж��֣�S-CDR,G-CDR��
    return 2;
}

UINT4 CR99_BillParser::GetBillTypeID(UINT4 uIndex)
{
    UINT4 uRet = 0;
    switch(uIndex)
    {
    case 0:
        uRet = S_CDR;
        break;
    case 1:
        uRet = G_CDR;
        break;
    /*
    case 2:
        uRet = M_CDR;
        break;
    */
    default:
        break;
    }
    return uRet;
}


/******************************************************
* ������:   CR99_BillParser::RegisterBillType
* ����:    GGF
* ����:    2003-09-28
* ����:    ��CMerger����ע�Ự�����͡�
* ����: 
*       ����1 :CMerger* pMerger
* ����ֵ: void 
* ˵��:     ���ڴ����ʧ�ܽ��׳��쳣��
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*
******************************************************/
void CR99_BillParser::RegisterBillType(CMerger* pMerger)
{
    assert(pMerger != NULL);
    
    for(UINT4 i = 0; i < GetBillTypeCount(); i++)
    {
        pMerger->RegisterBillType(GetBillTypeID(i));
    }
}

void CR99_BillParser::GetBillTypeDesc(UINT4 uBillTypeID, char* szDesc)
{
    switch(uBillTypeID)
    {
    case S_CDR:
        strcpy(szDesc, "S-CDR");
        break;
    case G_CDR:
        strcpy(szDesc, "G-CDR");
        break;
    /*
    case M_CDR:
        strcpy(szDesc, "M-CDR");
        break;
    */
    default:
        break;
    }
}


//CR99_SCDR��ʵ��(S-CDR)
CR99_SCDR::CR99_SCDR()
{
    m_pCondition = NULL;
    m_pExtension = NULL;
    m_pSequence = NULL;

    m_bPDPCompleted = FALSE;
    m_bPrepared = FALSE;

    m_nMinRecordSequence = 0;
    m_nCurRecordSequence = 0;

    m_Duration = 0;
    m_UnsentVolume = 0;
    memset(&m_Diagnostics,0,sizeof(CMCC_3GCDR_Diagnostics));
    m_CauseForRecClosing = CMCC_3GCDR_normalRelease;

	memset(&m_CAMELInformationPDP, 0, sizeof(CMCC_3GCDR_CG_CAMELInformationPDP));

    m_pOrgRec = NULL;
    m_pDestRec = NULL;

    m_pRecordSequenceList = NULL;
    m_pRecordSequenceListTail = NULL;
    m_nLen_RecordSequence_List = 0;

    m_pLocalSequenceList = NULL;
    m_pLocalSequenceListTail = NULL;
    m_nLen_LocalSequence_List = 0;

    m_pRNCDownlinkVolumeList = NULL;
    m_pRNCDownlinkVolumeListTail = NULL;
    m_nLen_RNCDownlinkVolume_List = 0;
}

CR99_SCDR::~CR99_SCDR()
{
    //����ԭʼ������¼�ռ䡣
    if(m_pOrgRec != NULL)
    {
        delete[] m_pOrgRec->listOfTrafficVolumes.value;
		
        if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_diagnostics_present)
        {
            Clear_Diagnostics(&(m_pOrgRec->diagnostics));
        }

        if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_recordExtensions_present)
        {
            //����ֶε������Ƿ���Ч��
            if((m_pOrgRec->recordExtensions.value != NULL)
                && (m_pOrgRec->recordExtensions.count > 0))
            {
                Clear_ManagementExtension(m_pOrgRec->recordExtensions.value,
                    m_pOrgRec->recordExtensions.count);
            }
        }

        delete m_pOrgRec;
        m_pOrgRec = NULL;
    }

    //�������ջ�����¼�ռ䡣
    if(m_pDestRec != NULL)
    {
		//ɾ��sgsnAddress����
		delete[] m_pDestRec->u.sgsnPDPRecord.sgsnAddress.value;

		//ɾ��listOfTrafficVolumes����
        delete[] m_pDestRec->u.sgsnPDPRecord.listOfTrafficVolumes.value;

		//ɾ��Diagnostics
        if(m_pDestRec->u.sgsnPDPRecord.bit_mask
            & CMCC_3GCDR_CG_SGSNPDPRecord_diagnostics_present)
        {
            Clear_Diagnostics((CMCC_3GCDR_Diagnostics*)&
                (m_pDestRec->u.sgsnPDPRecord.diagnostics));
        }

		//ɾ��recordExtensions
        if(m_pDestRec->u.sgsnPDPRecord.bit_mask
            & CMCC_3GCDR_CG_SGSNPDPRecord_recordExtensions_present)
        {
            if((m_pDestRec->u.sgsnPDPRecord.recordExtensions.value != NULL)
                && (m_pDestRec->u.sgsnPDPRecord.recordExtensions.count > 0))
            {
                Clear_ManagementExtension((CMCC_3GCDR_ManagementExtension*)
                    m_pDestRec->u.sgsnPDPRecord.recordExtensions.value,
                    m_pDestRec->u.sgsnPDPRecord.recordExtensions.count);
            }
        }

		//ɾ��recSequenceNumList
        if(m_pDestRec->u.sgsnPDPRecord.bit_mask
            & CMCC_3GCDR_CG_SGSNPDPRecord_recSequenceNumList_present)
        {
            delete[] m_pDestRec->u.sgsnPDPRecord.
                recSequenceNumList.value->recSequenceNumber.value;
			delete[] m_pDestRec->u.sgsnPDPRecord.recSequenceNumList.value;
        }

		//ɾ��localSequenceNumber
		if(m_pDestRec->u.sgsnPDPRecord.bit_mask
            & CMCC_3GCDR_CG_SGSNPDPRecord_localSequenceNumber_present)
        {
            delete[] m_pDestRec->u.sgsnPDPRecord.
                localSequenceNumber.value->localSeqNumberList.value;
			delete[] m_pDestRec->u.sgsnPDPRecord.localSequenceNumber.value;
        }

		//rNCUnsentDownlinkVolumeList
		if(m_pDestRec->u.sgsnPDPRecord.bit_mask
            & CMCC_3GCDR_CG_rNCUnsentDownlinkVolumeList_present)
        {
            delete[] m_pDestRec->u.sgsnPDPRecord.
                rNCUnsentDownlinkVolumeList.value->rNCUnsentDnVolumeList.value;
			delete[] m_pDestRec->u.sgsnPDPRecord.rNCUnsentDownlinkVolumeList.value;
        }

        delete m_pDestRec;
        m_pDestRec = NULL;
    }

    /*
     * �ϲ�������ʹ�õ����ݽṹ�Ŀռ��Ѿ���GetContents�л��ա�
     */
}


/******************************************************
* ������:   CR99_SCDR::Init
* ����:    GGF
* ����:    2003-09-28
* ����: ����������£�
*       1,����ԭʼ������¼����ΪIsPDPCompleted,CanSafeMerge,
*           IsPartial,IsFirstPartialRec,IsLastPartialRec�жϵĲο���
*       2,��listOfTrafficVolumes,recordExtensions,recordSequenceNumber
*           �ֶγ�ʼ������
*       3,��¼��ǰ�ϲ���ɵ������ڲ����кš�
*       4,����PDP������־��
* ����: 
*       ����1 :char* pBuf��ԭʼ������¼
*       ����2 :int nLen���������ݵĳ���
* ����ֵ: BOOL���ɹ�����TRUE���ڴ����ʧ�ܷ���FALSE��
* ˵��:
*       ���ú�������FALSE���򻰵����󽫱�ɾ����
*       �ڻ��������������������ɿռ�Ļ��ա�
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*
******************************************************/
BOOL CR99_SCDR::Init(char* pBuf,UINT4 uLen)
{
    assert(pBuf != NULL);
    
    CMCC_3GCDR_SGSNPDPRecord* p = (CMCC_3GCDR_SGSNPDPRecord*)pBuf;

    //����ԭʼ����
    m_pOrgRec = new CMCC_3GCDR_SGSNPDPRecord;
    if(m_pOrgRec == NULL)
    {
        return FALSE;
    }

    //���ƻ�����¼
    memcpy(m_pOrgRec,p,sizeof(CMCC_3GCDR_SGSNPDPRecord));

    /*
     * ���浥������ָ���ֶ�
     */

    //����listOfTrafficVolumes�ֶ�
    if((p->listOfTrafficVolumes.value != NULL)
        && (p->listOfTrafficVolumes.count > 0))
    {
        m_pOrgRec->listOfTrafficVolumes.value
            = Clone_ChangeOfCharCondition(
            p->listOfTrafficVolumes.value,
            p->listOfTrafficVolumes.count);

        m_pOrgRec->listOfTrafficVolumes.count = p->listOfTrafficVolumes.count;

        /* 
         * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
         * 
         */
        m_nLen_Condition_List = p->listOfTrafficVolumes.count;
    }

    //����diagnostics�ֶ�
    if(p->bit_mask & CMCC_3GCDR_SGSNPDPRecord_diagnostics_present)
    {
        if(p->diagnostics.choice == CMCC_3GCDR_networkSpecificCause_chosen)
        {
            CMCC_3GCDR_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(
                        &(p->diagnostics.u.networkSpecificCause),1);

            m_pOrgRec->diagnostics.choice = CMCC_3GCDR_networkSpecificCause_chosen;
            m_pOrgRec->diagnostics.u.networkSpecificCause = *q;
            
            delete[] q;
        }
        else if(p->diagnostics.choice 
            == CMCC_3GCDR_manufacturerSpecificCause_chosen)
        {
            CMCC_3GCDR_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(
                    &(p->diagnostics.u.manufacturerSpecificCause),1);

            m_pOrgRec->diagnostics.choice = CMCC_3GCDR_manufacturerSpecificCause_chosen;
            m_pOrgRec->diagnostics.u.manufacturerSpecificCause = *q;
            
            delete[] q;
        }
    }

    //����recordExtensions�ֶ�
    if(p->bit_mask & CMCC_3GCDR_SGSNPDPRecord_recordExtensions_present)
    {
        if((p->recordExtensions.value != NULL)
            && (p->recordExtensions.count > 0))
        {
            m_pOrgRec->recordExtensions.value
                = Clone_ManagementExtension(
                p->recordExtensions.value,
                p->recordExtensions.count);
            
            m_pOrgRec->recordExtensions.count = p->recordExtensions.count;

            /* 
             * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
             * 
             */
            m_nLen_Extension_List = p->recordExtensions.count;
        }
    }

	/* 
     * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
     * 
     */
	m_nLen_SGSNAddress_List = 1;

    //���õ�ǰ�������к�
    m_nMinRecordSequence = m_nCurRecordSequence = p->recordSequenceNumber;

	//ͨ��ʱ��
	m_Duration = p->duration;

    //����PDP������־
    switch(p->causeForRecClosing)
    {
    case CMCC_3GCDR_normalRelease:
    case CMCC_3GCDR_abnormalRelease:
    case CMCC_3GCDR_sGSNChange:
        m_bPDPCompleted = TRUE;
        break;
    default:
        break;
    }

    //���Ӱ�ͨ��ʱ�������ϲ� ZhengYuqun 2003-12-18
    if((UINT4)p->duration >= m_uMaxDuration)
    {
        m_bPDPCompleted = TRUE;
    }

    return TRUE;
}

BOOL CR99_SCDR::IsFirstPartialRec()
{
    return (m_pOrgRec->recordSequenceNumber == 1);
}

BOOL CR99_SCDR::IsLastPartialRec()
{
    switch(m_pOrgRec->causeForRecClosing)
    {
    case CMCC_3GCDR_normalRelease:
    case CMCC_3GCDR_abnormalRelease:
    case CMCC_3GCDR_sGSNChange:
        return TRUE;
        break;
    default:
        break;
    }

    //���Ӱ�ͨ��ʱ�������ϲ� ZhengYuqun 2003-12-18
    if((UINT4)m_pOrgRec->duration >= m_uMaxDuration)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL CR99_SCDR::IsPartial()
{
    /* 
     * ��recordSequenceNumber�ֶ���Ч���򻰵��ǲ��ֻ�����
     * 
     */
    return (m_pOrgRec->bit_mask &
         CMCC_3GCDR_SGSNPDPRecord_recordSequenceNumber_present);
}

BOOL CR99_SCDR::IsHotBilling()
{
    if((m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_chargingCharacteristics_present)
        && ((m_pOrgRec->chargingCharacteristics.value[0] & 0x0f) == 1))
    {
        return TRUE;
    }
    return FALSE;
}


/******************************************************
* ������:   CR99_SCDR::CanSafeMerge
* ����:    GGF
* ����:    2003-09-28
* ����:    �ж�pBillָ��Ļ����Ƿ��ܹ��뵱ǰ�����ϲ���
*          ����������TRUE����ǰ������Merge�����������ã�
*          ����pBill����������Qok��
* ����: 
*       ����1 :CBill* pBill�����ϲ��Ļ�����
* ����ֵ: BOOL 
* ˵��: 
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*
******************************************************/
BOOL CR99_SCDR::CanSafeMerge(CBill* pBill)
{
    assert(pBill != NULL);

    if(!m_bPrepared)
    {
        if(!PrepareData())  return FALSE;
    }

    CR99_SCDR* p = (CR99_SCDR*)pBill;

    /* added by ggf, 2003-12-24, ��ͬһSGSN�϶���л������Ļ�����������,
	 * �����ж��Ƿ������Ѿ��ϲ�������ʱ�䷶Χ�ڣ�
	 * 1.�����������ڵ�ǰ������ʱ�䷶Χ֮�⣬���ϲ�
	 * 2.�����������ڵ�ǰʱ�䷶Χ֮�ڣ����ظ��������ظ�������Merge�����ж�����
	 * 3.����������ʱ���뵱ǰ������ʱ��֮������[m_Duration, m_Duration + SGSN_CHANGE_TIME]
	 * ����Ϊ���������������кϲ�
	 */
	int nDiff = Get_Duration(&(m_pOrgRec->recordOpeningTime), &(p->m_pOrgRec->recordOpeningTime));
	if ((nDiff < 0) || (nDiff > (m_Duration + SGSN_CHANGE_TIME))) 
		return FALSE;
	else if	((nDiff >= 0) && (nDiff < m_Duration)) 
		return TRUE;

    /* 
     * ���󻰵���ǿ�����ߡ�
     * 
     */
    if(Match_gsnAddress(&(p->m_pOrgRec->sgsnAddress), m_pSGSNAddressTail->Value))
    {
        if((m_nCurRecordSequence - m_nMinRecordSequence + 2)
            > MAX_MERGED_BILLS)
        {
            return FALSE;
        }
    }
    else
    {
       //SGSN����Ѿ������л�, ��Ҫ����ϲ������Ļ���ԭ��ΪsGSNChange��
       //�ϲ�������sgsnChange�ֶ���λ��������ϲ�
       if((m_CauseForRecClosing != CMCC_3GCDR_sGSNChange)
           || (!(p->m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_sgsnChange_present))
           || (!(p->m_pOrgRec->sgsnChange)))
        {
            return FALSE;
        }
    }
    

    if((m_nLen_Condition_List + p->m_nLen_Condition_List)
        > MAX_MERGED_BILLS)
    {
        return FALSE;
    }

	if((m_nLen_SGSNAddress_List + p->m_nLen_SGSNAddress_List)
        > MAX_MERGED_BILLS)
    {
        return FALSE;
    }

    if((m_nLen_Extension_List + p->m_nLen_Extension_List)
        > MAX_MERGED_BILLS)
    {
        return FALSE;
    }

	/*
	 * �ϲ�CAMEL����freeFormatData,
	 * �ϲ��Ļ���������length�ʹ���160
	 * ǿ������
	 * 
	 */
	if((m_pOrgRec->bit_mask 
		    & CMCC_3GCDR_cAMELInformationPDP_present)
		&& (p->m_pOrgRec->bit_mask 
		    & CMCC_3GCDR_cAMELInformationPDP_present)
		&& (m_pOrgRec->cAMELInformationPDP.bit_mask
		    & CMCC_3GCDR_CAMELInformationPDP_freeFormatData_present)
		&& (p->m_pOrgRec->cAMELInformationPDP.bit_mask
		    & CMCC_3GCDR_CAMELInformationPDP_freeFormatData_present)
		&& (p->m_pOrgRec->cAMELInformationPDP.fFDAppendIndicator == TRUE) 
		&&(p->m_pOrgRec->cAMELInformationPDP.bit_mask
		   & CMCC_3GCDR_CAMELInformationPDP_freeFormatData_present)
		&& (p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length != 0))
	{
		int nfreeFormatDatalength;
		
		if(m_bPrepared == TRUE)
		{
			nfreeFormatDatalength = m_CAMELInformationPDP.freeFormatData.length
			          + p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length;
		}
		else
		{
			nfreeFormatDatalength = m_pOrgRec->cAMELInformationPDP.freeFormatData.length
			          + p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length;
		}
						
		if(nfreeFormatDatalength > 160)
		{
			return FALSE;
		}
	}
     
    return TRUE;
}

BOOL CR99_SCDR::IsPDPCompleted()
{
    return m_bPDPCompleted;
}

BOOL CR99_SCDR::NeedWaiting()
{
    //��ǰ�ĺϲ�������ü�����ʽ�ϲ�Duration�ֶ�
    //���Ըú���ֻ����FALSE(��PDP����ʱ��������ȴ�)��
    return FALSE;
}


/******************************************************
* ������:   CR99_SCDR::GetContents
* ����:    GGF
* ����:    2003-09-28
* ����:    �����������ջ�����ʽ��ϻ�����¼��
* ����: 
* ����ֵ: char*,�������ջ�����ʽ�Ļ�����¼��
* ˵��: 
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*
******************************************************/
char* CR99_SCDR::GetContents()
{
    if(m_pDestRec != NULL)
    {
        return (char*)m_pDestRec;
    }

    CMCC_3GCDR_CG_CallEventRecord* pCallEventRec = new CMCC_3GCDR_CG_CallEventRecord;
    if(pCallEventRec == NULL)
    {
        throw "Alloc memory fail.";
    }

    pCallEventRec->choice = CMCC_3GCDR_CG_sgsnPDPRecord_chosen;
    CMCC_3GCDR_CG_SGSNPDPRecord* pDest = &(pCallEventRec->u.sgsnPDPRecord);

    //��ʼ��Ϊ0��
    memset(pDest,0,sizeof(CMCC_3GCDR_CG_SGSNPDPRecord));
    m_pDestRec = pCallEventRec;

    //�������ڴ�鿪ʼ/������ַ��
    char* pBegin = NULL,*pEnd = NULL;

    /*
     * ������û�о����ϲ������ԭʼ�����й������ջ�����
     */

    //�ϲ�ǰbit_mask��4���ֽڳ����ϲ�����2���ֽڳ�������ֱ�Ӹ�ֵ��
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_networkInitiation_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_SGSNPDPRecord_networkInitiation_present;
    }
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_servedIMEI_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_SGSNPDPRecord_servedIMEI_present;
    }
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_msNetworkCapability_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_SGSNPDPRecord_msNetworkCapability_present;
    }
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_routingArea_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_SGSNPDPRecord_routingArea_present;
    }
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_locationAreaCode_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_SGSNPDPRecord_locationAreaCode_present;
    }
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_cellIdentifier_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_SGSNPDPRecord_cellIdentifier_present;
    }
	if (m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_servedPDPAddress_present)
	{
		pDest->bit_mask |= CMCC_3GCDR_CG_SGSNPDPRecord_servedPDPAddress_present;
	}
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_sgsnChange_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_SGSNPDPRecord_sgsnChange_present;
    }
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_diagnostics_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_SGSNPDPRecord_diagnostics_present;
    }
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_recordSequenceNumber_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_SGSNPDPRecord_recSequenceNumList_present;
    }
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_nodeID_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_SGSNPDPRecord_nodeID_present;
    }
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_recordExtensions_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_SGSNPDPRecord_recordExtensions_present;
    }
	if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_localSequenceNumber_present)
	{
		pDest->bit_mask |= CMCC_3GCDR_CG_SGSNPDPRecord_localSequenceNumber_present;
	}
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_apnSelectionMode_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_SGSNPDPRecord_apnSelectionMode_present;
    }
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_servedMSISDN_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_SGSNPDPRecord_servedMSISDN_present;
    }
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_chargingCharacteristics_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_SGSNPDPRecord_chargingCharacteristics_present;
    }
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_systemType_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_SGSNPDPRecord_systemType_present;
    }
	if(m_pOrgRec->bit_mask & CMCC_3GCDR_cAMELInformationPDP_present)
	{
		pDest->bit_mask |=  CMCC_3GCDR_CG_cAMELInformationPDP_present;
	}
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_rNCUnsentDownlinkVolume_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_rNCUnsentDownlinkVolumeList_present;
    }

    if(!m_bPrepared)
    {
        pBegin = (char*)(&(m_pOrgRec->recordType));
		pEnd = (char*)(&(m_pOrgRec->sgsnAddress));

		/*
         * recordType,networkInitiation,servedIMSI,
         * servedIMEI
         */

		memcpy(&(pDest->recordType), pBegin,(pEnd - pBegin));

		//����sgsnAddress�ֶ�
		pDest->sgsnAddress.value = new CMCC_3GCDR_CG_GSNAddress;
		if(pDest->sgsnAddress.value == NULL)
        {
			throw "Alloc memory fail.";
        }
		memcpy(pDest->sgsnAddress.value, &(m_pOrgRec->sgsnAddress), sizeof(CMCC_3GCDR_CG_GSNAddress));
		pDest->sgsnAddress.count = 1;

		pBegin = (char*)(&(m_pOrgRec->msNetworkCapability));
        pEnd = (char*)(&(m_pOrgRec->listOfTrafficVolumes));

        /*
         * msNetworkCapability,routingArea,
         * locationAreaCode,cellIdentifier,chargingID,ggsnAddressUsed,
         * accessPointNameNI,pdpType,servedPDPAddress
         */
        memcpy(&(pDest->msNetworkCapability),pBegin,(pEnd - pBegin));

        //����listOfTrafficVolumes�ֶ�
        if((m_pOrgRec->listOfTrafficVolumes.value != NULL)
            && (m_pOrgRec->listOfTrafficVolumes.count > 0))
        {
            pDest->listOfTrafficVolumes.value = (CMCC_3GCDR_CG_ChangeOfCharCondition*)
                Clone_ChangeOfCharCondition(
                m_pOrgRec->listOfTrafficVolumes.value,
                m_pOrgRec->listOfTrafficVolumes.count);
            
            pDest->listOfTrafficVolumes.count
                = m_pOrgRec->listOfTrafficVolumes.count;
        }

        pBegin = (char*)(&(m_pOrgRec->recordOpeningTime));
        pEnd = (char*)(&(m_pOrgRec->diagnostics));

        /*
         * recordOpeningTime,duration,sgsnChange,causeForRecClosing
         */
        memcpy(&(pDest->recordOpeningTime),pBegin,(pEnd - pBegin));

        //����diagnostics�ֶ�
        if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_diagnostics_present)
        {
            memcpy(&(pDest->diagnostics),&(m_pOrgRec->diagnostics),
                sizeof(CMCC_3GCDR_Diagnostics));

            if(m_pOrgRec->diagnostics.choice
                == CMCC_3GCDR_networkSpecificCause_chosen)
            {
                CMCC_3GCDR_ManagementExtension* q = NULL;
                q = Clone_ManagementExtension(
                    &(m_pOrgRec->diagnostics.u.networkSpecificCause),1);

                pDest->diagnostics.choice = CMCC_3GCDR_networkSpecificCause_chosen;
                pDest->diagnostics.u.networkSpecificCause
                    = *(CMCC_3GCDR_CG_ManagementExtension*)q;

                delete[] q;
            }
            else if(m_pOrgRec->diagnostics.choice
                == CMCC_3GCDR_manufacturerSpecificCause_chosen)
            {
                CMCC_3GCDR_ManagementExtension* q = NULL;
                q = Clone_ManagementExtension(
                    &(m_pOrgRec->diagnostics.u.manufacturerSpecificCause),1);

                pDest->diagnostics.choice = CMCC_3GCDR_manufacturerSpecificCause_chosen;
                pDest->diagnostics.u.manufacturerSpecificCause
                    = *(CMCC_3GCDR_CG_ManagementExtension*)q;

                delete[] q;
            }
        }

        /*
         * recordSequenceNumberת��Ϊ����
         */
        if(m_pOrgRec->bit_mask
            & CMCC_3GCDR_SGSNPDPRecord_recordSequenceNumber_present)
        {
			pDest->recSequenceNumList.value = new CMCC_3GCDR_CG_SequenceList;
			if(pDest->recSequenceNumList.value == NULL)
			{
				throw "Alloc memory fail.";
			}

            pDest->recSequenceNumList.value->recSequenceNumber.value = new int[1];
            if(pDest->recSequenceNumList.value->recSequenceNumber.value == NULL)
            {
                throw "Alloc memory fail.";
            }

            pDest->recSequenceNumList.value->recSequenceNumber.value[0]
                = m_pOrgRec->recordSequenceNumber;
            pDest->recSequenceNumList.value->recSequenceNumber.count = 1;
            memcpy(&(pDest->recSequenceNumList.value->sgsnAddress),
                   &(m_pOrgRec->sgsnAddress),sizeof(CMCC_3GCDR_GSNAddress));
			pDest->recSequenceNumList.count = 1;
            pDest->recSequenceNumList.value->bit_mask |= CMCC_3GCDR_CG_recSequenceNumber_present;
        }

        //����nodeID�ֶ�
        if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_nodeID_present)
        {
            memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(CMCC_3GCDR_NodeID));
        }

        //����recordExtensions�ֶ�
        if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_recordExtensions_present)
        {
            if((m_pOrgRec->recordExtensions.value != NULL)
                && (m_pOrgRec->recordExtensions.count > 0))
            {
                pDest->recordExtensions.value = (CMCC_3GCDR_CG_ManagementExtension*)
                    Clone_ManagementExtension(
                    m_pOrgRec->recordExtensions.value,
                    m_pOrgRec->recordExtensions.count);

                pDest->recordExtensions.count = m_pOrgRec->recordExtensions.count;
            }
        }

        /*
         * localSequenceNumberת��Ϊ����
         */
        if(m_pOrgRec->bit_mask
            & CMCC_3GCDR_SGSNPDPRecord_localSequenceNumber_present)
        {
			pDest->localSequenceNumber.value = new CMCC_3GCDR_CG_LocalSequenceNumberList;
			if(pDest->localSequenceNumber.value == NULL)
			{
				throw "Alloc memory fail.";
			}
            pDest->localSequenceNumber.value->localSeqNumberList.value = new int[1];
            if(pDest->localSequenceNumber.value->localSeqNumberList.value == NULL)
            {
                throw "Alloc memory fail.";
            }

            pDest->localSequenceNumber.value->localSeqNumberList.value[0]
                = m_pOrgRec->localSequenceNumber;
            pDest->localSequenceNumber.value->localSeqNumberList.count = 1;
            memcpy(&(pDest->localSequenceNumber.value->gsnAddress),
                   &(m_pOrgRec->sgsnAddress),sizeof(CMCC_3GCDR_GSNAddress));
			pDest->localSequenceNumber.count = 1;
            pDest->localSequenceNumber.value->bit_mask |= CMCC_3GCDR_CG_localSeqNumberList_present;
        }

        pBegin = (char*)(&(m_pOrgRec->apnSelectionMode));
        pEnd = (char*)(&(m_pOrgRec->rNCUnsentDownlinkVolume));

        /*
         * apnSelectionMode,accessPggsnPDPRecordointNameOI,servedMSISDN,
         * chargingCharacteristics,systemType, cAMELInformationPDP,
		 *
         */
		memcpy(&(pDest->apnSelectionMode),pBegin,(pEnd - pBegin));

        /*
         * rNCUnsentDownlinkVolumeת��Ϊ����
         */
        if(m_pOrgRec->bit_mask
            & CMCC_3GCDR_rNCUnsentDownlinkVolume_present)
        {
			pDest->rNCUnsentDownlinkVolumeList.value = new CMCC_3GCDR_CG_RNCUnsentDownlinkVolumeList;
			if(pDest->rNCUnsentDownlinkVolumeList.value == NULL)
			{
				throw "Alloc memory fail.";
			}

            pDest->rNCUnsentDownlinkVolumeList.value->rNCUnsentDnVolumeList.value = new int[1];
            if(pDest->rNCUnsentDownlinkVolumeList.value->rNCUnsentDnVolumeList.value == NULL)
            {
                throw "Alloc memory fail.";
            }

            pDest->rNCUnsentDownlinkVolumeList.value->rNCUnsentDnVolumeList.value[0]
                = m_pOrgRec->rNCUnsentDownlinkVolume;
            pDest->rNCUnsentDownlinkVolumeList.value->rNCUnsentDnVolumeList.count = 1;
            memcpy(&(pDest->rNCUnsentDownlinkVolumeList.value->sgsnAddress),
                   &(m_pOrgRec->sgsnAddress),sizeof(CMCC_3GCDR_GSNAddress));
			pDest->rNCUnsentDownlinkVolumeList.count = 1;
            pDest->rNCUnsentDownlinkVolumeList.value->bit_mask |= CMCC_3GCDR_CG_rNCUnsentDnVolumeList_present;
        }


        //consolidationResult�ֶ�
        switch(m_pOrgRec->causeForRecClosing)
        {
        case CMCC_3GCDR_normalRelease:
            pDest->consolidationResult = CMCC_3GCDR_CG_normal;
            break;
        case CMCC_3GCDR_sGSNChange:
            //PDP��������
            pDest->consolidationResult = CMCC_3GCDR_CG_forInterSGSNConsolidation;
            break;
        case CMCC_3GCDR_abnormalRelease:
            //PDP�쳣����
            pDest->consolidationResult = CMCC_3GCDR_CG_notnormal;
            break;
        case CMCC_3GCDR_timeLimit:
            pDest->consolidationResult = CMCC_3GCDR_CG_reachLimit;
        default:
            //PDPδ����(��ΪPDP�쳣����)
             pDest->consolidationResult = CMCC_3GCDR_CG_notnormal;
            break;
        }
        
        if (!IsPartial())
        {
            pDest->consolidationResult = CMCC_3GCDR_CG_onlyOneCDRGenerated;	
        }
         

        return (char*)m_pDestRec;
    }

    /*
     * �����Ǻϲ��󻰵�����Ϲ��̡�
     */

    pBegin = (char*)(&(m_pOrgRec->recordType));
	pEnd = (char*)(&(m_pOrgRec->sgsnAddress));

	/*
     * recordType,networkInitiation,servedIMSI,
     * servedIMEI,sgsnAddress
     */
    memcpy(&(pDest->recordType),pBegin,(pEnd - pBegin));

	//�ϲ�sgsnAddress�ֶ�
    if(m_pSGSNAddress != NULL)
    {
		typedef LINK_STRUCT2<CMCC_3GCDR_GSNAddress> link_Structs;
		unsigned int nCount = 0;
        pDest->sgsnAddress.value = (CMCC_3GCDR_CG_GSNAddress*)
            List_To_Array2<CMCC_3GCDR_GSNAddress, link_Structs>
			(m_pSGSNAddress,nCount/*in,out*/);

        pDest->sgsnAddress.count = nCount;

        Clear_List2(m_pSGSNAddress);
        m_pSGSNAddress = m_pSGSNAddressTail = NULL;
    }

	pBegin = (char*)(&(m_pOrgRec->msNetworkCapability));
	pEnd = (char*)(&(m_pOrgRec->listOfTrafficVolumes));
    /*
     * msNetworkCapability,
     * routingArea,locationAreaCode,cellIdentifier,
     * chargingID,ggsnAddressUsed,accessPointNameNI,
     * pdpType,servedPDPAddress
     */

    memcpy(&(pDest->msNetworkCapability),pBegin,(pEnd - pBegin));

    pBegin = (char*)(&(m_pOrgRec->recordOpeningTime));
    pEnd = (char*)(&(m_pOrgRec->diagnostics));

    //�ϲ�listOfTrafficVolumes�ֶ�
    if(m_pCondition != NULL)
    {
		typedef LINK_STRUCT2<CMCC_3GCDR_ChangeOfCharCondition> link_Struct2;
		unsigned int nCount = 0;
        pDest->listOfTrafficVolumes.value = 
            List_To_Array2<CMCC_3GCDR_CG_ChangeOfCharCondition, link_Struct2>
			(m_pCondition,nCount/*in,out*/);
        
        pDest->listOfTrafficVolumes.count = nCount;

        //ɾ���ϲ�������ʹ�õ���ʱ����
        Clear_List2(m_pCondition);
        m_pCondition = m_pConditionTail = NULL;
    }
    
    /* 
     * recordOpeningTime,duration,sgsnChange,
     * causeForRecClosing
     */
    memcpy(&(pDest->recordOpeningTime),pBegin,(pEnd - pBegin));

    //�ϲ�diagnostics�ֶ�
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_diagnostics_present)
    {
        memcpy(&(pDest->diagnostics),&(m_Diagnostics),
            sizeof(CMCC_3GCDR_Diagnostics));
    }

    //�ϲ�recordSequenceNumber�ֶ�
    if(m_pOrgRec->bit_mask
        & CMCC_3GCDR_SGSNPDPRecord_recordSequenceNumber_present)
    {
        CMCC_3GCDR_CG_SequenceList* pList2 = 
            new CMCC_3GCDR_CG_SequenceList[m_nLen_RecordSequence_List];
        if(pList2 == NULL)
        {
            throw "Alloc memory fail.";
        }

        LINK_STRUCT1<SGSN_DATA1>* pList1 = m_pRecordSequenceList;
        for(unsigned int i = 0; i < m_nLen_RecordSequence_List; i++)
        {
            assert(pList1 != NULL);

            memcpy(&(pList2[i].sgsnAddress), &(pList1->Value.sgsnAddress), 
                    sizeof(CMCC_3GCDR_GSNAddress));

		    typedef LINK_STRUCT1<int> link_Struct1;
            pList2[i].recSequenceNumber.value 
                   = List_To_Array1<int, link_Struct1>(pList1->Value.pList, pList1->Value.nCount/*in,out*/);
            pList2[i].recSequenceNumber.count = pList1->Value.nCount;
            pList2[i].bit_mask |= CMCC_3GCDR_CG_recSequenceNumber_present;

            pList1 = pList1->m_pNext;
        }

		pDest->recSequenceNumList.value = pList2;
        pDest->recSequenceNumList.count = m_nLen_RecordSequence_List;

        Clear_List1(m_pRecordSequenceList);
        m_pRecordSequenceList = m_pRecordSequenceListTail = NULL;
        m_pSequenceTail = NULL;
    
    }

    //nodeID�ֶ�(һ������)
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_nodeID_present)
    {
        memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(CMCC_3GCDR_NodeID));
    }

    //�ϲ�recordExtensions�ֶ�
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_recordExtensions_present)
    {
        if(m_pExtension != NULL)
        {
			typedef LINK_STRUCT2<CMCC_3GCDR_ManagementExtension> link_Struct2;
			unsigned int nCount = 0;
            pDest->recordExtensions.value = 
				List_To_Array2<CMCC_3GCDR_CG_ManagementExtension, link_Struct2>
				(m_pExtension,nCount/*in,out*/);
            
            pDest->recordExtensions.count = nCount;
            
            Clear_List2(m_pExtension);
            m_pExtension = m_pExtensionTail = NULL;
        }
    }

	//�ϲ�localSequenceNumber�ֶ�
    if(m_pOrgRec->bit_mask
        & CMCC_3GCDR_SGSNPDPRecord_localSequenceNumber_present)
    {
        CMCC_3GCDR_CG_LocalSequenceNumberList* pList2 = 
            new CMCC_3GCDR_CG_LocalSequenceNumberList[m_nLen_LocalSequence_List];
        if(pList2 == NULL)
        {
            throw "Alloc memory fail.";
        }

        LINK_STRUCT1<SGSN_DATA1>* pList1 = m_pLocalSequenceList;
        for(unsigned int i = 0; i < m_nLen_LocalSequence_List; i++)
        {
            assert(pList1 != NULL);

            memcpy(&(pList2[i].gsnAddress), &(pList1->Value.sgsnAddress), 
                    sizeof(CMCC_3GCDR_GSNAddress));

		    typedef LINK_STRUCT1<int> link_Struct1;
            pList2[i].localSeqNumberList.value 
                   = List_To_Array1<int, link_Struct1>(pList1->Value.pList, pList1->Value.nCount/*in,out*/);
            pList2[i].localSeqNumberList.count = pList1->Value.nCount;
            pList2[i].bit_mask |= CMCC_3GCDR_CG_localSeqNumberList_present;

            pList1 = pList1->m_pNext;
        }

		pDest->localSequenceNumber.value = pList2;
        pDest->localSequenceNumber.count = m_nLen_LocalSequence_List;

        Clear_List1(m_pLocalSequenceList);
        m_pLocalSequenceList = m_pLocalSequenceListTail = NULL;
        m_pLocalSequenceNumberTail = NULL;
    }

    pBegin = (char*)(&(m_pOrgRec->apnSelectionMode));
    pEnd = (char*)(&(m_pOrgRec->rNCUnsentDownlinkVolume));

    /*
     * apnSelectionMode,accessPointNameOI,
     * servedMSISDN,chargingCharacteristics,
     * systemType,cAMELInformationPDP;
	 * 
     */
	memcpy(&(pDest->apnSelectionMode),pBegin,(pEnd - pBegin));

	//�ϲ�rNCUnsentDownlinkVolume�ֶ�
    if(m_pRNCDownlinkVolumeList != NULL)
    {
        CMCC_3GCDR_CG_RNCUnsentDownlinkVolumeList* pList2 = 
            new CMCC_3GCDR_CG_RNCUnsentDownlinkVolumeList[m_nLen_RNCDownlinkVolume_List];
        if(pList2 == NULL)
        {
            throw "Alloc memory fail.";
        }

        LINK_STRUCT1<SGSN_DATA1>* pList1 = m_pRNCDownlinkVolumeList;
        for(unsigned int i = 0; i < m_nLen_RNCDownlinkVolume_List; i++)
        {
            assert(pList1 != NULL);

            memcpy(&(pList2[i].sgsnAddress), &(pList1->Value.sgsnAddress), 
                    sizeof(CMCC_3GCDR_GSNAddress));

		    typedef LINK_STRUCT1<int> link_Struct1;
            pList2[i].rNCUnsentDnVolumeList.value 
                   = List_To_Array1<int, link_Struct1>(pList1->Value.pList, pList1->Value.nCount/*in,out*/);
            pList2[i].rNCUnsentDnVolumeList.count = pList1->Value.nCount;
            pList2[i].bit_mask |= CMCC_3GCDR_CG_rNCUnsentDnVolumeList_present;

            pList1 = pList1->m_pNext;
        }
        
		pDest->rNCUnsentDownlinkVolumeList.value = pList2;
        pDest->rNCUnsentDownlinkVolumeList.count = m_nLen_RNCDownlinkVolume_List;

        Clear_List1(m_pRNCDownlinkVolumeList);
        m_pRNCDownlinkVolumeList = m_pRNCDownlinkVolumeListTail = NULL;
        m_pRNCUnsentDownlinkVolumeTail = NULL;        
    }

    //consolidationResult�ֶ�
    switch(m_CauseForRecClosing)
    {
    case CMCC_3GCDR_normalRelease:
        pDest->consolidationResult = CMCC_3GCDR_CG_normal;
        break;
    case CMCC_3GCDR_sGSNChange:
        //PDP��������
        pDest->consolidationResult = CMCC_3GCDR_CG_forInterSGSNConsolidation;
        break;
    case CMCC_3GCDR_abnormalRelease:
        //PDP�쳣����
        pDest->consolidationResult = CMCC_3GCDR_CG_notnormal;
        break;
    case CMCC_3GCDR_timeLimit:
        pDest->consolidationResult = CMCC_3GCDR_CG_reachLimit;
    default:
        //PDPδ����(��ΪPDP�쳣����)
        pDest->consolidationResult = CMCC_3GCDR_CG_notnormal;
        break;
    }
    
    if (!IsPartial())
    {
        pDest->consolidationResult = CMCC_3GCDR_CG_onlyOneCDRGenerated;	
    }
    
    pDest->duration = m_Duration;    
    pDest->causeForRecClosing = m_CauseForRecClosing;
	
	memcpy(&(pDest->cAMELInformationPDP), 
		&(m_CAMELInformationPDP), sizeof(CMCC_3GCDR_CG_CAMELInformationPDP));
    
	return (char*)m_pDestRec;
}

UINT4 CR99_SCDR::GetBillType()
{
    return m_pOrgRec->recordType;
}

CKey CR99_SCDR::CreateKey()
{
    CR99_SCDR_Key* pkey = new CR99_SCDR_Key;
    if(pkey == NULL)
    {
        throw "Alloc memory fail.";
    }
    pkey->chargingID         = m_pOrgRec->chargingID;
    pkey->ggsnAddressUsed    = m_pOrgRec->ggsnAddressUsed;
    //pkey->sgsnAddress        = m_pOrgRec->sgsnAddress;

    CKey key;
    key.m_pDelegation = pkey;

    return key;
}


/******************************************************
* ������:   CR99_SCDR::PrepareData
* ����:    GGF
* ����:    2003-09-28
* ����:    Ϊ�ϲ�׼�����ݡ�
* ����:    �ޡ�
* ����ֵ: BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE��
* ˵��:    �ϲ���������Ҫ��һЩ�ֶ������������γ�һ������
*          �ϲ���ɺ��ٰ�����ת��Ϊ���顣
*          �ú���������ת��Ϊ����
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*
******************************************************/
BOOL CR99_SCDR::PrepareData()
{
	//ת��SCDR�е�sgsnAddress
    m_pSGSNAddress = LINK_STRUCT2<CMCC_3GCDR_GSNAddress>::Alloc();
	m_pSGSNAddress->Value = new CMCC_3GCDR_GSNAddress;
	if(m_pSGSNAddress->Value == NULL)
	{
		throw "Alloc memory fail.";
	}
	memcpy(m_pSGSNAddress->Value, &(m_pOrgRec->sgsnAddress), sizeof(CMCC_3GCDR_GSNAddress));
    m_pSGSNAddress->nCount = 1;
    m_pSGSNAddressTail = m_pSGSNAddress;

    //����listOfTrafficVolumes
    if((m_pOrgRec->listOfTrafficVolumes.value != NULL)
        && (m_pOrgRec->listOfTrafficVolumes.count > 0))
    {
        m_pCondition = LINK_STRUCT2<CMCC_3GCDR_ChangeOfCharCondition>::Alloc();

        m_pCondition->Value = Clone_ChangeOfCharCondition(
            m_pOrgRec->listOfTrafficVolumes.value,
            m_pOrgRec->listOfTrafficVolumes.count);

        m_pCondition->nCount = m_pOrgRec->listOfTrafficVolumes.count;
        m_pConditionTail = m_pCondition;
    }

    //����recordExtensions
    if(m_pOrgRec->bit_mask 
        & CMCC_3GCDR_SGSNPDPRecord_recordExtensions_present)
    {
        if((m_pOrgRec->recordExtensions.value != NULL)
            && (m_pOrgRec->recordExtensions.count > 0))
        {
            m_pExtension = LINK_STRUCT2<CMCC_3GCDR_ManagementExtension>::Alloc();

            m_pExtension->Value = Clone_ManagementExtension(
                m_pOrgRec->recordExtensions.value,
                m_pOrgRec->recordExtensions.count);

            m_pExtension->nCount = m_pOrgRec->recordExtensions.count;
            m_pExtensionTail = m_pExtension;
        }
    }

    //����recordSequenceNumber
    if(m_pOrgRec->bit_mask 
        & CMCC_3GCDR_SGSNPDPRecord_recordSequenceNumber_present)
    {
        LINK_STRUCT1<int>* q = LINK_STRUCT1<int>::Alloc();
        q->Value = m_pOrgRec->recordSequenceNumber;

        LINK_STRUCT1<SGSN_DATA1>* pList = LINK_STRUCT1<SGSN_DATA1>::Alloc();
        pList->Value.pList = q;
        pList->Value.nCount++;

     	memcpy(&(pList->Value.sgsnAddress), 
               &(m_pOrgRec->sgsnAddress), 
               sizeof(CMCC_3GCDR_GSNAddress));

        m_pRecordSequenceList = pList;
        m_pRecordSequenceListTail = pList;
        m_nLen_RecordSequence_List++;
        
        m_pSequenceTail = q;
    }

	//����localSequenceNumber
    if(m_pOrgRec->bit_mask 
        & CMCC_3GCDR_SGSNPDPRecord_localSequenceNumber_present)
    {
        LINK_STRUCT1<int>* q = LINK_STRUCT1<int>::Alloc();
        q->Value = m_pOrgRec->localSequenceNumber;

        LINK_STRUCT1<SGSN_DATA1>* pList = LINK_STRUCT1<SGSN_DATA1>::Alloc();
        pList->Value.pList = q;
        pList->Value.nCount++;

     	memcpy(&(pList->Value.sgsnAddress), 
               &(m_pOrgRec->sgsnAddress), 
               sizeof(CMCC_3GCDR_GSNAddress));

        m_pLocalSequenceList = pList;
        m_pLocalSequenceListTail = pList;
        m_nLen_LocalSequence_List++;

        m_pLocalSequenceNumberTail = q;
    }

	//����rNCUnsentDownlinkVolume
    if(m_pOrgRec->bit_mask 
        & CMCC_3GCDR_rNCUnsentDownlinkVolume_present)
    {
        LINK_STRUCT1<int>* q = LINK_STRUCT1<int>::Alloc();
        q->Value = m_pOrgRec->rNCUnsentDownlinkVolume;

        LINK_STRUCT1<SGSN_DATA1>* pList = LINK_STRUCT1<SGSN_DATA1>::Alloc();
        pList->Value.pList = q;
        pList->Value.nCount++;        

     	memcpy(&(pList->Value.sgsnAddress), 
               &(m_pOrgRec->sgsnAddress), 
               sizeof(CMCC_3GCDR_GSNAddress));

        m_pRNCDownlinkVolumeList = pList;
        m_pRNCDownlinkVolumeListTail = pList;
        m_nLen_RNCDownlinkVolume_List++;

        m_pRNCUnsentDownlinkVolumeTail = q;
    }

    //����duration�ֶ�
    m_Duration = m_pOrgRec->duration;

	//����cAMELInformationPDP
	if(m_pOrgRec->bit_mask
		& CMCC_3GCDR_cAMELInformationPDP_present)
	{
		m_CAMELInformationPDP = m_pOrgRec->cAMELInformationPDP;
	}

    //����causeForRecClosing�ֶ�
    m_CauseForRecClosing = m_pOrgRec->causeForRecClosing;

    //����diagnostics�ֶ�
    if(m_pOrgRec->bit_mask
        & CMCC_3GCDR_SGSNPDPRecord_diagnostics_present)
    {
        m_Diagnostics = m_pOrgRec->diagnostics;
        
        if(m_Diagnostics.choice == CMCC_3GCDR_networkSpecificCause_chosen)
        {
            CMCC_3GCDR_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(
                &(m_pOrgRec->diagnostics.u.networkSpecificCause),1);

            m_Diagnostics.choice = CMCC_3GCDR_networkSpecificCause_chosen;
            m_Diagnostics.u.networkSpecificCause = *q;
            
            delete[] q;
        }
        else if(m_Diagnostics.choice 
            == CMCC_3GCDR_manufacturerSpecificCause_chosen)
        {
            CMCC_3GCDR_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(
                &(m_pOrgRec->diagnostics.u.manufacturerSpecificCause),1);

            m_Diagnostics.choice = CMCC_3GCDR_manufacturerSpecificCause_chosen;
            m_Diagnostics.u.manufacturerSpecificCause = *q;
            
            delete[] q;
        }
    }

    //׼�����
    m_bPrepared = TRUE;
    return TRUE;
}
BOOL CR99_SCDR::IsDuplicated(CBill* pBill)
{
    CR99_SCDR* p = (CR99_SCDR*)pBill;
    int nDiff = Get_Duration(&(m_pOrgRec->recordOpeningTime), &(p->m_pOrgRec->recordOpeningTime));
    if	((nDiff >= 0) && (nDiff < m_Duration)) 
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    
}
/******************************************************
* ������:   CR99_SCDR::Merge
* ����:    GGF
* ����:    2003-09-28
* ����:    �ѻ���pBill���뵱ǰ�����С�
* ����: 
*       ����1 :CBill* pBill�����ϲ��Ļ�����
* ����ֵ: BOOL���ϲ��ɹ�����TRUE��ʧ�ܷ���FALSE��
* ˵��: 
*       �ϲ������и��µ��ֶ���:
*       ��Ҫ���ӵ��ֶ�(sgsnAddress,listOfTrafficVolumes,recordSequenceNumber,rNCUnsentDownlinkVolume,
*       recordExtensions),ȡ���һ�Ż���ֵ���ֶ�(causeForRecClosing,
*       diagnostics),��Ҫ�ۼӵ��ֶ�(Duration)��
*       ȡ��һ�Ż���ֵ���ֶβ��䡣
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*
******************************************************/
BOOL CR99_SCDR::Merge(CBill* pBill)
{
    assert(pBill != NULL);

    if(!m_bPrepared)
    {
        if(!PrepareData())  return FALSE;
    }

    CR99_SCDR* p = (CR99_SCDR*)pBill;

    //�����ظ�������
    int nDiff = Get_Duration(&(m_pOrgRec->recordOpeningTime), &(p->m_pOrgRec->recordOpeningTime));
	if	((nDiff >= 0) && (nDiff < m_Duration)) 
		return TRUE;

    //��ͬ��:
    //m_nCurRecordSequence = p->m_pOrgRec->recordSequenceNumber;
    //m_nCurRecordSequence++;

    //����PDP������־��
    switch(p->m_pOrgRec->causeForRecClosing)
    {
    case CMCC_3GCDR_normalRelease:
    case CMCC_3GCDR_abnormalRelease:
    //case CMCC_3GCDR_sGSNChange:
        m_bPDPCompleted = TRUE;
        break;
    default:
        break;
    }

    //����causeForRecClosing�ֶΡ�
    m_CauseForRecClosing = p->m_pOrgRec->causeForRecClosing;
    if(p->m_pOrgRec->bit_mask & CMCC_3GCDR_rNCUnsentDownlinkVolume_present)
    {
        m_pOrgRec->bit_mask |= CMCC_3GCDR_rNCUnsentDownlinkVolume_present;
    }

    //�Ƿ�֧����ʱ��
	if(!m_bDSTFlg)
	{
		int nDiff = Get_Duration(&(m_pOrgRec->recordOpeningTime),
			&(p->m_pOrgRec->recordOpeningTime));
		m_Duration = nDiff;
	}
	//�ۼ�Duration�ֶΡ�
	m_Duration += p->m_pOrgRec->duration;
	
	//����sgsnAddress�ֶ�, �жϴ��ϲ�SCDR��SGSN��ַ�Ƿ����������Ѵ��ڣ���Ҫ����  
    unsigned int nCount = 1;
    LINK_STRUCT2<CMCC_3GCDR_GSNAddress>* q = LINK_STRUCT2<CMCC_3GCDR_GSNAddress>::Alloc();

    q->Value = Clone_GSNAddress(&(m_pSGSNAddressTail->Value[m_pSGSNAddressTail->nCount - 1]), &(p->m_pOrgRec->sgsnAddress),nCount/*in,out*/);

    q->nCount = nCount;

    BOOL bSgsnChange = FALSE;    
    if(q->Value == NULL)
    {
		//��ʱnCountΪ0
        delete q;
    }
    else
    {
        m_pSGSNAddressTail->m_pNext = q;
        m_pSGSNAddressTail = q;

        bSgsnChange = TRUE;
    }


    if(!bSgsnChange)
    {
        m_nCurRecordSequence++;
    }
    else
    {
        m_nMinRecordSequence = m_nCurRecordSequence = p->m_pOrgRec->recordSequenceNumber;
    }

    /* 
     * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
     * 
     */
    m_nLen_SGSNAddress_List += nCount;

    //����listOfTrafficVolumes�ֶΡ�
    if((p->m_pOrgRec->listOfTrafficVolumes.value != NULL)
        && (p->m_pOrgRec->listOfTrafficVolumes.count > 0))
    {
        LINK_STRUCT2<CMCC_3GCDR_ChangeOfCharCondition>* q 
            = LINK_STRUCT2<CMCC_3GCDR_ChangeOfCharCondition>::Alloc();

        q->Value = Clone_ChangeOfCharCondition(
                p->m_pOrgRec->listOfTrafficVolumes.value,
                p->m_pOrgRec->listOfTrafficVolumes.count);

        q->nCount = p->m_pOrgRec->listOfTrafficVolumes.count;

        if(m_pConditionTail == NULL)
        {
            m_pCondition = m_pConditionTail = q;
        }
        else
        {
            m_pConditionTail->m_pNext = q;
            m_pConditionTail = q;
        }

        /* 
         * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
         * 
         */
        m_nLen_Condition_List += q->nCount;
    }

    /*
    //����recordExtensions�ֶΡ�
    if((p->m_pOrgRec->bit_mask
        & CMCC_3GCDR_SGSNPDPRecord_recordExtensions_present)
        && (m_pOrgRec->bit_mask
        & CMCC_3GCDR_SGSNPDPRecord_recordExtensions_present))
    {
        if((p->m_pOrgRec->recordExtensions.value != NULL)
            && (p->m_pOrgRec->recordExtensions.count > 0))
        {
            LINK_STRUCT2<CMCC_3GCDR_ManagementExtension>* q
                = LINK_STRUCT2<CMCC_3GCDR_ManagementExtension>::Alloc();
            
            q->Value = Clone_ManagementExtension(
                p->m_pOrgRec->recordExtensions.value,
                p->m_pOrgRec->recordExtensions.count);
            
            q->nCount = p->m_pOrgRec->recordExtensions.count;
            
            if(m_pExtensionTail == NULL)
            {
                m_pExtension = m_pExtensionTail = q;
            }
            else
            {
                m_pExtensionTail->m_pNext = q;
                m_pExtensionTail = q;
            }

            /* 
             * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
             * 
             
            m_nLen_Extension_List += q->nCount;
        }
    }
    */

    //����recordSequenceNumber�ֶΡ�
    if((p->m_pOrgRec->bit_mask 
        & CMCC_3GCDR_SGSNPDPRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask 
        & CMCC_3GCDR_SGSNPDPRecord_recordSequenceNumber_present))
    {
        LINK_STRUCT1<int>* q
            = LINK_STRUCT1<int>::Alloc();
        
        q->Value = p->m_pOrgRec->recordSequenceNumber;

        if(!bSgsnChange)
        {
            m_pSequenceTail->m_pNext = q;
            m_pSequenceTail = q;

            m_pRecordSequenceListTail->Value.nCount++;
        }
        else
        {
            LINK_STRUCT1<SGSN_DATA1>* pList = LINK_STRUCT1<SGSN_DATA1>::Alloc();
            pList->Value.pList = q;
            pList->Value.nCount++;

            memcpy(&(pList->Value.sgsnAddress), 
                   &(p->m_pOrgRec->sgsnAddress), 
                   sizeof(CMCC_3GCDR_GSNAddress));

            m_pRecordSequenceListTail->m_pNext = pList;
            m_pRecordSequenceListTail = pList;
            m_nLen_RecordSequence_List++;

            m_pSequenceTail = q;
        }
    }

	//����localSequenceNumber�ֶΡ�
    if((p->m_pOrgRec->bit_mask 
        & CMCC_3GCDR_SGSNPDPRecord_localSequenceNumber_present)
        && (m_pOrgRec->bit_mask 
        & CMCC_3GCDR_SGSNPDPRecord_localSequenceNumber_present))
    {
        LINK_STRUCT1<int>* q
            = LINK_STRUCT1<int>::Alloc();
        
        q->Value = p->m_pOrgRec->localSequenceNumber;

        if(!bSgsnChange)
        {
            m_pLocalSequenceNumberTail->m_pNext = q;
            m_pLocalSequenceNumberTail = q;

            m_pLocalSequenceListTail->Value.nCount++;
        }
        else
        {
            LINK_STRUCT1<SGSN_DATA1>* pList = LINK_STRUCT1<SGSN_DATA1>::Alloc();
            pList->Value.pList = q;
            pList->Value.nCount++;

            memcpy(&(pList->Value.sgsnAddress), 
                   &(p->m_pOrgRec->sgsnAddress), 
                   sizeof(CMCC_3GCDR_GSNAddress));

            m_pLocalSequenceListTail->m_pNext = pList;
            m_pLocalSequenceListTail = pList;
            m_nLen_LocalSequence_List++;

            m_pLocalSequenceNumberTail = q;
        }
    }

    //����rNCUnsentDownlinkVolume�ֶΡ�
    if(p->m_pOrgRec->bit_mask & CMCC_3GCDR_rNCUnsentDownlinkVolume_present)
    {
        LINK_STRUCT1<int>* q
            = LINK_STRUCT1<int>::Alloc();
        
        q->Value = p->m_pOrgRec->rNCUnsentDownlinkVolume;

        if(m_pRNCDownlinkVolumeList == NULL)
        {
            LINK_STRUCT1<SGSN_DATA1>* pList = LINK_STRUCT1<SGSN_DATA1>::Alloc();
            pList->Value.pList = q;
            pList->Value.nCount++;

            memcpy(&(pList->Value.sgsnAddress), 
                   &(p->m_pOrgRec->sgsnAddress), 
                   sizeof(CMCC_3GCDR_GSNAddress));

            m_pRNCDownlinkVolumeList = pList;
            m_pRNCDownlinkVolumeListTail = pList;
            m_nLen_RNCDownlinkVolume_List++;

            m_pRNCUnsentDownlinkVolumeTail = q;
        }
        else
        {
            if(!bSgsnChange)
            {
                m_pRNCUnsentDownlinkVolumeTail->m_pNext = q;
                m_pRNCUnsentDownlinkVolumeTail = q;

                m_pRNCDownlinkVolumeListTail->Value.nCount++;
            }
            else
            {
                LINK_STRUCT1<SGSN_DATA1>* pList = LINK_STRUCT1<SGSN_DATA1>::Alloc();
                pList->Value.pList = q;
                pList->Value.nCount++;

                memcpy(&(pList->Value.sgsnAddress), 
                       &(p->m_pOrgRec->sgsnAddress), 
                       sizeof(CMCC_3GCDR_GSNAddress));

                m_pRNCDownlinkVolumeListTail->m_pNext = pList;
                m_pRNCDownlinkVolumeListTail = pList;
                m_nLen_RNCDownlinkVolume_List++;

                m_pRNCUnsentDownlinkVolumeTail = q;
            }
        }

    }

    //diagnostics�ֶ�ȡ���һ�Ż�����ֵ��
    if((p->m_pOrgRec->bit_mask 
        & CMCC_3GCDR_SGSNPDPRecord_diagnostics_present))
   {
        CMCC_3GCDR_ManagementExtension* q = NULL;
        if(p->m_pOrgRec->diagnostics.choice
            == CMCC_3GCDR_networkSpecificCause_chosen)
        {
            q = Clone_ManagementExtension(
                &(p->m_pOrgRec->diagnostics.u.networkSpecificCause),1);
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CMCC_3GCDR_manufacturerSpecificCause_chosen)
        {
            q = Clone_ManagementExtension(
                &(p->m_pOrgRec->diagnostics.u.manufacturerSpecificCause),1);
        }

        /*����m_Diagnostics�Ŀռ䡣*/
        if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_diagnostics_present) //����һ��if YanYong 2004-04-26 SWPD04795
        {            
            if(m_Diagnostics.choice == CMCC_3GCDR_manufacturerSpecificCause_chosen)
            {
                delete[] m_Diagnostics.u.manufacturerSpecificCause.identifier.value;
                delete[] m_Diagnostics.u.manufacturerSpecificCause.information.value;
            }
            else if(m_Diagnostics.choice == CMCC_3GCDR_networkSpecificCause_chosen)
            {
                delete[] m_Diagnostics.u.networkSpecificCause.identifier.value;
                delete[] m_Diagnostics.u.networkSpecificCause.information.value;
            }
        }
        else
        {
            m_pOrgRec->bit_mask |= CMCC_3GCDR_SGSNPDPRecord_diagnostics_present; //����һ�� YanYong 2004-04-26 SWPD04795
        }
        /************************/

        if(p->m_pOrgRec->diagnostics.choice
            == CMCC_3GCDR_networkSpecificCause_chosen)
        {
            m_Diagnostics.choice = CMCC_3GCDR_networkSpecificCause_chosen;
            
            m_Diagnostics.u.networkSpecificCause = *q;
            delete[] q;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CMCC_3GCDR_manufacturerSpecificCause_chosen)
        {
            m_Diagnostics.choice = CMCC_3GCDR_manufacturerSpecificCause_chosen;
            
            m_Diagnostics.u.manufacturerSpecificCause = *q;
            delete[] q;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CMCC_3GCDR_gsm0408Cause_chosen)
        {
            m_Diagnostics.choice = CMCC_3GCDR_gsm0408Cause_chosen;
            m_Diagnostics.u.gsm0408Cause = 
                p->m_pOrgRec->diagnostics.u.gsm0408Cause;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CMCC_3GCDR_gsm0902MapErrorValue_chosen)
        {
            m_Diagnostics.choice = CMCC_3GCDR_gsm0902MapErrorValue_chosen;
            m_Diagnostics.u.gsm0902MapErrorValue = 
                p->m_pOrgRec->diagnostics.u.gsm0902MapErrorValue;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CMCC_3GCDR_ccittQ767Cause_chosen)
        {
            m_Diagnostics.choice = CMCC_3GCDR_ccittQ767Cause_chosen;
            m_Diagnostics.u.ccittQ767Cause = 
                p->m_pOrgRec->diagnostics.u.ccittQ767Cause;
        }
    }

	//�ϲ�CAMELInformation�����Ϣ
    if((p->m_pOrgRec->bit_mask 
		& CMCC_3GCDR_cAMELInformationPDP_present)
		&& (m_pOrgRec->bit_mask
		& CMCC_3GCDR_cAMELInformationPDP_present))
	{
		/*
		 * sCFAddress,serviceKey,defaultTransactionHandling,
		 * cAMELAccessPointNameNI,cAMELAccessPointNameOI
		 * ������ȡ��һ�Ų��ֻ���������
		 */

		/*
		 * numberOfDPEncountered,levelOfCAMELService
		 * ������ȡ���һ�Ų��ֻ���������
		 */
		m_CAMELInformationPDP.numberOfDPEncountered = 
				p->m_pOrgRec->cAMELInformationPDP.numberOfDPEncountered;

		m_CAMELInformationPDP.levelOfCAMELService = 
				p->m_pOrgRec->cAMELInformationPDP.levelOfCAMELService;
				
		/*
		 * fFDAppendIndicator�ֶδ�����ֵΪTRUE��freeFormatData
		 * ��Ϣ���Ȳ�Ϊ�㣬���ǰ��Ļ����ϲ�
		 */
		//fFDAppendIndicator �ֶβ�����߸���ΪFALSE
		//freeFormatData���ڶ�����Ϣ���Ȳ�Ϊ��
		if((!(p->m_pOrgRec->cAMELInformationPDP.bit_mask 
			& CMCC_3GCDR_CAMELInformationPDP_fFDAppendIndicator_present)
			|| ((p->m_pOrgRec->cAMELInformationPDP.bit_mask 
			& CMCC_3GCDR_CAMELInformationPDP_fFDAppendIndicator_present)
            &&  (p->m_pOrgRec->cAMELInformationPDP.fFDAppendIndicator == FALSE))) 
			&& (p->m_pOrgRec->cAMELInformationPDP.bit_mask
			& CMCC_3GCDR_CAMELInformationPDP_freeFormatData_present)
			&&(p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length != 0))
		{
			memcpy(m_CAMELInformationPDP.freeFormatData.value,
				p->m_pOrgRec->cAMELInformationPDP.freeFormatData.value,
				p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length);
			
			m_CAMELInformationPDP.freeFormatData.length =
				p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length;

			m_CAMELInformationPDP.bit_mask |= 
				CMCC_3GCDR_CAMELInformationPDP_freeFormatData_present;
		}
		
		//fFDAppendIndicator �ֶδ�����ֵΪTRUE
		//freeFormatData���ڶ�����Ϣ���Ȳ�Ϊ��
		if((p->m_pOrgRec->cAMELInformationPDP.bit_mask
			& CMCC_3GCDR_CAMELInformationPDP_fFDAppendIndicator_present)
			&& (p->m_pOrgRec->cAMELInformationPDP.fFDAppendIndicator == TRUE) 
			&&(p->m_pOrgRec->cAMELInformationPDP.bit_mask
			& CMCC_3GCDR_CAMELInformationPDP_freeFormatData_present)
			&& (p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length != 0))
		{
			memcpy(m_CAMELInformationPDP.freeFormatData.value 
				+ m_CAMELInformationPDP.freeFormatData.length,
				p->m_pOrgRec->cAMELInformationPDP.freeFormatData.value,
				p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length);

			m_CAMELInformationPDP.freeFormatData.length +=
				p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length;

			m_CAMELInformationPDP.bit_mask |= 
				CMCC_3GCDR_CAMELInformationPDP_freeFormatData_present;
		}
		
	}
   
    //���Ӱ�ͨ��ʱ�������ϲ� ZhengYuqun 2003-12-18
    if((UINT4)m_Duration >= m_uMaxDuration)
    {
        m_bPDPCompleted = TRUE;
    }

    /*
     * �����ֶ�ȡ��һ�Ż�����ֵ��
     */
    return TRUE;
}


//CR99_GCDR��ʵ��(G-CDR)
CR99_GCDR::CR99_GCDR()
{
    m_pSGSNAddress = NULL;
    m_pCondition = NULL;
    m_pExtension = NULL;
    m_pSequence = NULL;

    m_bPDPCompleted = FALSE;
    m_bPrepared = FALSE;

    m_nMinRecordSequence = 0;
    m_nCurRecordSequence = 0;

    m_Duration = 0;
    memset(&m_Diagnostics,0,sizeof(CMCC_3GCDR_Diagnostics));
    m_CauseForRecClosing = CMCC_3GCDR_normalRelease;

    m_pDestRec = NULL;
    m_pOrgRec = NULL;
}

CR99_GCDR::~CR99_GCDR()
{
    //����ԭʼ������¼�ռ䡣
    if(m_pOrgRec != NULL)
    {
        delete[] m_pOrgRec->sgsnAddress.value;

        delete[] m_pOrgRec->listOfTrafficVolumes.value;

        if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_diagnostics_present)
        {
            Clear_Diagnostics(&(m_pOrgRec->diagnostics));
        }

        if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_recordExtensions_present)
        {
            if((m_pOrgRec->recordExtensions.value != NULL)
                && (m_pOrgRec->recordExtensions.count > 0))
            {
                Clear_ManagementExtension(m_pOrgRec->recordExtensions.value,
                    m_pOrgRec->recordExtensions.count);
            }
        }

        delete m_pOrgRec;
        m_pOrgRec = NULL;
    }

    //�������ջ�����¼�ռ䡣
    if(m_pDestRec != NULL)
    {
        delete[] m_pDestRec->u.ggsnPDPRecord.sgsnAddress.value;

        delete[] m_pDestRec->u.ggsnPDPRecord.listOfTrafficVolumes.value;

        if(m_pDestRec->u.ggsnPDPRecord.bit_mask
            & CMCC_3GCDR_CG_GGSNPDPRecord_diagnostics_present)
        {
            Clear_Diagnostics((CMCC_3GCDR_Diagnostics*)
                &(m_pDestRec->u.ggsnPDPRecord.diagnostics));
        }

        if(m_pDestRec->u.ggsnPDPRecord.bit_mask
            & CMCC_3GCDR_CG_GGSNPDPRecord_recordExtensions_present)
        {
            if((m_pDestRec->u.ggsnPDPRecord.recordExtensions.value != NULL)
                && (m_pDestRec->u.ggsnPDPRecord.recordExtensions.count > 0))
            {
                Clear_ManagementExtension((CMCC_3GCDR_ManagementExtension*)
                    m_pDestRec->u.ggsnPDPRecord.recordExtensions.value,
                    m_pDestRec->u.ggsnPDPRecord.recordExtensions.count);
            }
        }

        if(m_pDestRec->u.ggsnPDPRecord.bit_mask
            & CMCC_3GCDR_CG_GGSNPDPRecord_recSequenceNumList_present)
        {
            delete[] m_pDestRec->u.ggsnPDPRecord.recSequenceNumList.
                recSequenceNumber.value;
        }

		if(m_pDestRec->u.ggsnPDPRecord.bit_mask
            & CMCC_3GCDR_CG_localSequenceNumberList_present)
        {
            delete[] m_pDestRec->u.ggsnPDPRecord.localSequenceNumberList.
                localSeqNumberList.value;
        }

        delete m_pDestRec;
        m_pDestRec = NULL;
    }

    /*
     * �ϲ�������ʹ�õ����ݽṹ�Ŀռ��Ѿ���GetContents�л��ա�
     */
}

BOOL CR99_GCDR::PrepareData()
{
    //ת��sgsnAddress
    if((m_pOrgRec->sgsnAddress.value != NULL)
        && (m_pOrgRec->sgsnAddress.count > 0))
    {
        m_pSGSNAddress = LINK_STRUCT2<CMCC_3GCDR_GSNAddress>::Alloc();

        m_pSGSNAddress->Value = Clone_GSNAddress(
            m_pOrgRec->sgsnAddress.value,
            m_pOrgRec->sgsnAddress.count);

        m_pSGSNAddress->nCount = m_pOrgRec->sgsnAddress.count;
        m_pSGSNAddressTail = m_pSGSNAddress;
    }

    //����listOfTrafficVolumes
    if((m_pOrgRec->listOfTrafficVolumes.value != NULL)
        && (m_pOrgRec->listOfTrafficVolumes.count > 0))
    {
        m_pCondition =  LINK_STRUCT2<CMCC_3GCDR_ChangeOfCharCondition>::Alloc();

        m_pCondition->Value = Clone_ChangeOfCharCondition(
            m_pOrgRec->listOfTrafficVolumes.value,
            m_pOrgRec->listOfTrafficVolumes.count);

        m_pCondition->nCount = m_pOrgRec->listOfTrafficVolumes.count;
        m_pConditionTail = m_pCondition;
    }

    //����recordExtensions
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_recordExtensions_present)
    {
        if((m_pOrgRec->recordExtensions.value != NULL)
            && (m_pOrgRec->recordExtensions.count > 0))
        {
            m_pExtension = LINK_STRUCT2<CMCC_3GCDR_ManagementExtension>::Alloc();
            
            m_pExtension->Value = Clone_ManagementExtension(
                m_pOrgRec->recordExtensions.value,
                m_pOrgRec->recordExtensions.count);
            
            m_pExtension->nCount = m_pOrgRec->recordExtensions.count;
            m_pExtensionTail = m_pExtension;
        }
    }

    //����recordSequenceNumber
    if(m_pOrgRec->bit_mask 
        & CMCC_3GCDR_GGSNPDPRecord_recordSequenceNumber_present)
    {
        m_pSequence = LINK_STRUCT1<int>::Alloc();

        m_pSequence->Value = m_pOrgRec->recordSequenceNumber;
        m_pSequenceTail = m_pSequence;
    }

	//����localSequenceNumber
    if(m_pOrgRec->bit_mask 
        & CMCC_3GCDR_GGSNPDPRecord_localSequenceNumber_present)
    {
        m_pLocalSequenceNumber = LINK_STRUCT1<int>::Alloc();

        m_pLocalSequenceNumber->Value = m_pOrgRec->localSequenceNumber;
        m_pLocalSequenceNumberTail = m_pLocalSequenceNumber;
    }

    //����duration�ֶ�
    m_Duration = m_pOrgRec->duration;

    //����causeForRecClosing�ֶ�
    m_CauseForRecClosing = m_pOrgRec->causeForRecClosing;

    //����diagnostics�ֶ�
    if(m_pOrgRec->bit_mask
        & CMCC_3GCDR_GGSNPDPRecord_diagnostics_present)
    {
        m_Diagnostics = m_pOrgRec->diagnostics;
        
        if(m_Diagnostics.choice == CMCC_3GCDR_networkSpecificCause_chosen)
        {
            CMCC_3GCDR_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(
                &(m_pOrgRec->diagnostics.u.networkSpecificCause),1);

            m_Diagnostics.choice = CMCC_3GCDR_networkSpecificCause_chosen;
            m_Diagnostics.u.networkSpecificCause = *q;
            
            delete[] q;
        }
        else if(m_Diagnostics.choice 
            == CMCC_3GCDR_manufacturerSpecificCause_chosen)
        {
            CMCC_3GCDR_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(
                &(m_pOrgRec->diagnostics.u.manufacturerSpecificCause),1);

            m_Diagnostics.choice = CMCC_3GCDR_manufacturerSpecificCause_chosen;
            m_Diagnostics.u.manufacturerSpecificCause = *q;
            
            delete[] q;
        }
    }

    //׼�����
    m_bPrepared = TRUE;
    return TRUE;
}


/******************************************************
* ������:   CR99_GCDR::Init
* ����:    GGF
* ����:    2003-09-28
* ����:    
* ����: 
*       ����1 :char* pBuf��G-CDR������¼��
*       ����2 :int nLen��������¼�ĳ��ȡ�
* ����ֵ: BOOL ��ʼ���ɹ�����TRUE���ڴ����ʧ���׳��쳣��
* ˵��: 
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*
******************************************************/
BOOL CR99_GCDR::Init(char* pBuf,UINT4 uLen)
{
    assert(pBuf != NULL);

    //������¼�������ֶζ���������������ڴ����
    CMCC_3GCDR_GGSNPDPRecord* p = (CMCC_3GCDR_GGSNPDPRecord*)pBuf;

    //����ԭʼ����
    m_pOrgRec = new CMCC_3GCDR_GGSNPDPRecord;
    if(m_pOrgRec == NULL)
    {
        throw "Alloc memory fail.";
    }

    memcpy(m_pOrgRec,p,sizeof(CMCC_3GCDR_GGSNPDPRecord));

    /*
     * ���浥������ָ���ֶ�
     */

    //����sgsnAddress
    if((p->sgsnAddress.value != NULL) && (p->sgsnAddress.count > 0))
    {
        m_pOrgRec->sgsnAddress.value = Clone_GSNAddress(
            p->sgsnAddress.value,
            p->sgsnAddress.count);
        
        m_pOrgRec->sgsnAddress.count = p->sgsnAddress.count;

        /* 
         * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
         * 
         */
        m_nLen_SGSNAddress_List = p->sgsnAddress.count;
    }

    //����listOfTrafficVolumes�ֶ�
    if((p->listOfTrafficVolumes.value != NULL)
        && (p->listOfTrafficVolumes.count > 0))
    {
        m_pOrgRec->listOfTrafficVolumes.value
            = Clone_ChangeOfCharCondition(
            p->listOfTrafficVolumes.value,
            p->listOfTrafficVolumes.count);
        
        m_pOrgRec->listOfTrafficVolumes.count
            = p->listOfTrafficVolumes.count;

        /* 
         * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
         * 
         */
        m_nLen_Condition_List = p->listOfTrafficVolumes.count;
    }

    //����diagnostics�ֶ�
    if(p->bit_mask & CMCC_3GCDR_GGSNPDPRecord_diagnostics_present)
    {
        m_pOrgRec->diagnostics = p->diagnostics;
        
        if(p->diagnostics.choice == CMCC_3GCDR_networkSpecificCause_chosen)
        {
            CMCC_3GCDR_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(
                        &(p->diagnostics.u.networkSpecificCause),1);

            m_pOrgRec->diagnostics.choice
                = CMCC_3GCDR_networkSpecificCause_chosen;
            m_pOrgRec->diagnostics.u.networkSpecificCause = *q;
            
            delete[] q;
        }
        else if(p->diagnostics.choice 
            == CMCC_3GCDR_manufacturerSpecificCause_chosen)
        {
            CMCC_3GCDR_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(
                    &(p->diagnostics.u.manufacturerSpecificCause),1);

            m_pOrgRec->diagnostics.choice
                = CMCC_3GCDR_manufacturerSpecificCause_chosen;
            m_pOrgRec->diagnostics.u.manufacturerSpecificCause = *q;

            delete[] q;
        }
    }

    //����recordExtensions�ֶ�
    if(p->bit_mask & CMCC_3GCDR_GGSNPDPRecord_recordExtensions_present)
    {
        if((p->recordExtensions.value != NULL)
            && (p->recordExtensions.count > 0))
        {
            m_pOrgRec->recordExtensions.value
                = Clone_ManagementExtension(
                p->recordExtensions.value,
                p->recordExtensions.count);

            m_pOrgRec->recordExtensions.count = p->recordExtensions.count;

            /* 
             * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
             * 
             */
            m_nLen_Extension_List = p->recordExtensions.count;
        }
    }

    //���õ�ǰ�������к�
    m_nMinRecordSequence = m_nCurRecordSequence = p->recordSequenceNumber;

    //����PDP������־
    switch(p->causeForRecClosing)
    {
    case CMCC_3GCDR_normalRelease:
    case CMCC_3GCDR_abnormalRelease:
    case CMCC_3GCDR_sGSNChange:
        m_bPDPCompleted = TRUE;
        break;
    default:
        break;
    }

    //���Ӱ�ͨ��ʱ�������ϲ� ZhengYuqun 2003-12-18
    if((UINT4)p->duration >= m_uMaxDuration)
    {
        m_bPDPCompleted = TRUE;
    }

    return TRUE;
}

BOOL CR99_GCDR::IsFirstPartialRec()
{
    return (m_pOrgRec->recordSequenceNumber == 1);
}

BOOL CR99_GCDR::IsLastPartialRec()
{
    switch(m_pOrgRec->causeForRecClosing)
    {
    case CMCC_3GCDR_normalRelease:
    case CMCC_3GCDR_abnormalRelease:
    case CMCC_3GCDR_sGSNChange:
        return TRUE;
        break;
    default:
        break;
    }

    //���Ӱ�ͨ��ʱ�������ϲ� ZhengYuqun 2003-12-18
    if((UINT4)m_pOrgRec->duration >= m_uMaxDuration)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL CR99_GCDR::IsPartial()
{
    /* 
     * ��recordSequenceNumber�ֶ���Ч���򻰵��ǲ��ֻ�����
     * 
     */
    return (m_pOrgRec->bit_mask &
        CMCC_3GCDR_GGSNPDPRecord_recordSequenceNumber_present);
}

BOOL CR99_GCDR::IsHotBilling()
{
    if((m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_chargingCharacteristics_present)
        && ((m_pOrgRec->chargingCharacteristics.value[0] & 0x0f) == 1))
    {
        return TRUE;
    }
    return FALSE;
}


/******************************************************
* ������:   CR99_GCDR::CanSafeMerge
* ����:    GGF
* ����:    2003-09-28
* ����:    
* ����: 
*       ����1 :CBill* pBill,���ϲ��Ļ�������ָ�롣
* ����ֵ: BOOL ,�����Ż�������ϲ�������TRUE�����򷵻�FALSE��
* ˵��: 
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*
******************************************************/
BOOL CR99_GCDR::CanSafeMerge(CBill* pBill)
{
    assert(pBill != NULL);

    CR99_GCDR* p = (CR99_GCDR*)pBill;

    //�����������к����Ѿ��ϲ��Ļ�����Χ�ڣ�˵�����ظ�������
    //�ظ�������Merge�����ж�����
    if((p->m_pOrgRec->bit_mask
        & CMCC_3GCDR_GGSNPDPRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask
        & CMCC_3GCDR_GGSNPDPRecord_recordSequenceNumber_present))
    {
        if((p->m_nCurRecordSequence <= m_nCurRecordSequence)
            && (p->m_nCurRecordSequence >= m_nMinRecordSequence))
        {
            return TRUE;
        }
    }

    /* 
     * ���󻰵���ǿ�����ߡ�
     * 
     */
    if((m_nCurRecordSequence - m_nMinRecordSequence + 2)
        > MAX_MERGED_BILLS)
    {
        return FALSE;
    }

    if((m_nLen_Condition_List + p->m_nLen_Condition_List)
        > MAX_MERGED_BILLS)
    {
        return FALSE;
    }

    if((m_nLen_SGSNAddress_List + p->m_nLen_SGSNAddress_List)
        > MAX_MERGED_BILLS)
    {
        return FALSE;
    }

    if((m_nLen_Extension_List + p->m_nLen_Extension_List)
        > MAX_MERGED_BILLS)
    {
        return FALSE;
    }

    //�����кŲ�������������ϲ���
    return (p->m_nCurRecordSequence == (m_nCurRecordSequence + 1));
}

BOOL CR99_GCDR::IsPDPCompleted()
{
    return m_bPDPCompleted;
}

BOOL CR99_GCDR::NeedWaiting()
{
    //��ǰ�ĺϲ�������ü�����ʽ�ϲ�Duration�ֶ�
    //���Ըú���ֻ����FALSE(��PDP����ʱ��������ȴ�)��
    BOOL bTmp = FALSE;
    return bTmp;
}

char* CR99_GCDR::GetContents()
{
    if(m_pDestRec != NULL)
    {
        return (char*)m_pDestRec;
    }

    CMCC_3GCDR_CG_CallEventRecord* pCallEventRec = new CMCC_3GCDR_CG_CallEventRecord;
    if(pCallEventRec == NULL)
    {
        throw "Alloc memory fail.";
    }

    pCallEventRec->choice = CMCC_3GCDR_CG_ggsnPDPRecord_chosen;
    CMCC_3GCDR_CG_GGSNPDPRecord* pDest = &(pCallEventRec->u.ggsnPDPRecord);

    memset(pDest,0,sizeof(CMCC_3GCDR_CG_GGSNPDPRecord));
    m_pDestRec = pCallEventRec;

    //�������ڴ�鿪ʼ/������ַ��
    char* pBegin = NULL,*pEnd = NULL;

    /*
     * ������û�о����ϲ������ԭʼ�����й������ջ�����
     */

    //�ϲ�ǰbit_mask��4���ֽڳ����ϲ�����2���ֽڳ�������ֱ�Ӹ�ֵ��
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_networkInitiation_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_GGSNPDPRecord_networkInitiation_present;
    }
	if (m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_servedPDPAddress_present)
	{
		pDest->bit_mask |= CMCC_3GCDR_CG_GGSNPDPRecord_servedPDPAddress_present;
	}
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_dynamicAddressFlag_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_dynamicAddressFlag_present;
    }
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_diagnostics_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_GGSNPDPRecord_diagnostics_present;
    }
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_recordSequenceNumber_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_GGSNPDPRecord_recSequenceNumList_present;
    }
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_nodeID_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_GGSNPDPRecord_nodeID_present;
    }
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_recordExtensions_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_GGSNPDPRecord_recordExtensions_present;
    }
	//SWPD04515 CG��R99 3a0�Ļ����ϲ�������CMCC�淶 ggf 2004.4.7
	if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_localSequenceNumber_present)
	{
		pDest->bit_mask |= CMCC_3GCDR_CG_localSequenceNumberList_present;
	}
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_apnSelectionMode_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_GGSNPDPRecord_apnSelectionMode_present;
    }
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_servedMSISDN_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_GGSNPDPRecord_servedMSISDN_present;
    }
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_chargingCharacteristics_present)
    {
        pDest->bit_mask |= CMCC_3GCDR_CG_GGSNPDPRecord_chargingCharacteristics_present;
    }
	//SWPD04515 CG��R99 3a0�Ļ����ϲ�������CMCC�淶 ggf 2004.4.7
	if(m_pOrgRec->bit_mask & CMCC_3GCDR_sgsnPLMNIdentifier_present)
	{
		pDest->bit_mask |= CMCC_3GCDR_CG_sgsnPLMNIdentifier_present;
	}
    
    if(!m_bPrepared)
    {
        pBegin = (char*)(&(m_pOrgRec->recordType));
        pEnd = (char*)(&(m_pOrgRec->sgsnAddress));
        /* 
         * recordType,networkInitiation,servedIMSI,
         * ggsnAddress,chargingID
         */
        memcpy(&(pDest->recordType),pBegin,(pEnd - pBegin));

        //����sgsnAddress�ֶ�
        if((m_pOrgRec->sgsnAddress.value != NULL)
            && (m_pOrgRec->sgsnAddress.count > 0))
        {
            pDest->sgsnAddress.value = (CMCC_3GCDR_CG_GSNAddress*)
                Clone_GSNAddress(m_pOrgRec->sgsnAddress.value,
                m_pOrgRec->sgsnAddress.count);
            
            pDest->sgsnAddress.count = m_pOrgRec->sgsnAddress.count;
        }

        pBegin = (char*)(&(m_pOrgRec->accessPointNameNI));
        pEnd = (char*)(&(m_pOrgRec->listOfTrafficVolumes));
        /*
         * accessPointNameNI,pdpType,servedPDPAddress,dynamicAddressFlag
         */
        memcpy(&(pDest->accessPointNameNI),pBegin,(pEnd - pBegin));

        //����listOfTrafficVolumes�ֶ�
        if((m_pOrgRec->listOfTrafficVolumes.value != NULL)
            && (m_pOrgRec->listOfTrafficVolumes.count > 0))
        {
            pDest->listOfTrafficVolumes.value = (CMCC_3GCDR_CG_ChangeOfCharCondition*)
                Clone_ChangeOfCharCondition(
                m_pOrgRec->listOfTrafficVolumes.value,
                m_pOrgRec->listOfTrafficVolumes.count);

            pDest->listOfTrafficVolumes.count = m_pOrgRec->listOfTrafficVolumes.count;
        }

        pBegin = (char*)(&(m_pOrgRec->recordOpeningTime));
        pEnd = (char*)(&(m_pOrgRec->diagnostics));
        /*
         * recordOpeningTime,duration,causeForRecClosing
         */
        memcpy(&(pDest->recordOpeningTime),pBegin,(pEnd - pBegin));
        
        //����diagnostics�ֶ�
        if(m_pOrgRec->bit_mask & CMCC_3GCDR_CG_GGSNPDPRecord_diagnostics_present)
        {
            memcpy(&(pDest->diagnostics),&(m_pOrgRec->diagnostics),
                sizeof(CMCC_3GCDR_Diagnostics));

            if(m_pOrgRec->diagnostics.choice
                == CMCC_3GCDR_networkSpecificCause_chosen)
            {
                CMCC_3GCDR_ManagementExtension* q = NULL;
                q = Clone_ManagementExtension(
                    &(m_pOrgRec->diagnostics.u.networkSpecificCause),1);

                pDest->diagnostics.choice = CMCC_3GCDR_networkSpecificCause_chosen;
                pDest->diagnostics.u.networkSpecificCause
                    = *(CMCC_3GCDR_CG_ManagementExtension*)q;

                delete[] q;
            }
            else if(m_pOrgRec->diagnostics.choice 
                == CMCC_3GCDR_manufacturerSpecificCause_chosen)
            {
                CMCC_3GCDR_ManagementExtension* q = NULL;
                q = Clone_ManagementExtension(
                    &(m_pOrgRec->diagnostics.u.manufacturerSpecificCause),1);

                pDest->diagnostics.choice = CMCC_3GCDR_manufacturerSpecificCause_chosen;
                pDest->diagnostics.u.manufacturerSpecificCause
                    = *(CMCC_3GCDR_CG_ManagementExtension*)q;
                
                delete[] q;
            }
        }

        /*
         * recordSequenceNumberת��Ϊ����
         */
        if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_recordSequenceNumber_present)
        {
            pDest->recSequenceNumList.recSequenceNumber.value = new int[1];
            if(pDest->recSequenceNumList.recSequenceNumber.value == NULL)
            {
                throw "Alloc memory fail.";
            }
            
            pDest->recSequenceNumList.recSequenceNumber.value[0]
                = m_pOrgRec->recordSequenceNumber;
            pDest->recSequenceNumList.recSequenceNumber.count = 1;
            pDest->recSequenceNumList.bit_mask |= CMCC_3GCDR_CG_recSequenceNumber_present;
            
            
            if(m_pOrgRec->sgsnAddress.count > 0)
            {
                memcpy(&(pDest->recSequenceNumList.sgsnAddress),
                    m_pOrgRec->sgsnAddress.value, sizeof(CMCC_3GCDR_GSNAddress));
            }
            else
            {
                memcpy(&(pDest->recSequenceNumList.sgsnAddress),
                    0, sizeof(CMCC_3GCDR_GSNAddress));
            }
        }

        //����nodeID�ֶ�(һ������)
        if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_nodeID_present)
        {
            memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(CMCC_3GCDR_NodeID));
        }

        //����recordExtensions�ֶ�
        if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_recordExtensions_present)
        {
            if((m_pOrgRec->recordExtensions.value != NULL)
                && (m_pOrgRec->recordExtensions.count > 0))
            {
                pDest->recordExtensions.value = (CMCC_3GCDR_CG_ManagementExtension*)
                    Clone_ManagementExtension(
                    m_pOrgRec->recordExtensions.value,
                    m_pOrgRec->recordExtensions.count);
                
                pDest->recordExtensions.count = m_pOrgRec->recordExtensions.count;
            }
        }

		/*
         * localSequenceNumberת��Ϊ����
         */
        if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_localSequenceNumber_present)
        {
            pDest->localSequenceNumberList.localSeqNumberList.value = new int[1];
            if(pDest->localSequenceNumberList.localSeqNumberList.value == NULL)
            {
                throw "Alloc memory fail.";
            }
            
            pDest->localSequenceNumberList.localSeqNumberList.value[0]
                = m_pOrgRec->localSequenceNumber;
            pDest->localSequenceNumberList.localSeqNumberList.count = 1;
            pDest->localSequenceNumberList.bit_mask |= CMCC_3GCDR_CG_localSeqNumberList_present;
            
            memcpy(&(pDest->localSequenceNumberList.gsnAddress),
                    &(m_pOrgRec->ggsnAddress),sizeof(CMCC_3GCDR_GSNAddress));
        }

        pBegin = (char*)(&(m_pOrgRec->apnSelectionMode));
        pEnd = (char*)(&(m_pOrgRec->sgsnPLMNIdentifier));
        /* 
         * apnSelectionMode,servedMSISDN,chargingCharacteristics,sgsnPLMNIdentifier
         */
        memcpy(&(pDest->apnSelectionMode),pBegin,
            (pEnd - pBegin) + sizeof(CMCC_3GCDR_CG_PLMN_Id));
        
		//�޸Ľ���
        //by ldf 2004-07-05 ��Ӧ���ⵥSWPD06029
        //consolidationResult�ֶ�
        switch(m_pOrgRec->causeForRecClosing)
        {
        case CMCC_3GCDR_normalRelease:
            pDest->consolidationResult = CMCC_3GCDR_CG_normal;
            break;
        case CMCC_3GCDR_sGSNChange:
            //PDP��������
            pDest->consolidationResult = CMCC_3GCDR_CG_forInterSGSNConsolidation;
            break;
        case CMCC_3GCDR_abnormalRelease:
            //PDP�쳣����
            pDest->consolidationResult = CMCC_3GCDR_CG_notnormal;
            break;
        case CMCC_3GCDR_timeLimit:
            pDest->consolidationResult = CMCC_3GCDR_CG_reachLimit;
        default:
            //PDPδ����(��ΪPDP�쳣����)
			pDest->consolidationResult = CMCC_3GCDR_CG_notnormal;
            break;
        }
		//end
		
        return (char*)m_pDestRec;
    }

    /*
     * �����Ǻϲ��󻰵�����Ϲ��̡�
     */

    pBegin = (char*)(&(m_pOrgRec->recordType));
    pEnd = (char*)(&(m_pOrgRec->sgsnAddress));

    /* 
     * recordType,networkInitiation,servedIMSI,
     * ggsnAddress,chargingID
     */
    memcpy(&(pDest->recordType),pBegin,(pEnd - pBegin));
    
    unsigned int nCount = 0;
    //�ϲ�sgsnAddress�ֶ�
    if(m_pSGSNAddress != NULL)
    {
		typedef LINK_STRUCT2<CMCC_3GCDR_GSNAddress> link_Structs;
        pDest->sgsnAddress.value = (CMCC_3GCDR_CG_GSNAddress*)
            List_To_Array2<CMCC_3GCDR_GSNAddress, link_Structs>
			(m_pSGSNAddress,nCount/*in,out*/);

        pDest->sgsnAddress.count = nCount;

        Clear_List2(m_pSGSNAddress);
        m_pSGSNAddress = m_pSGSNAddressTail = NULL;
    }

    pBegin = (char*)(&(m_pOrgRec->accessPointNameNI));
    pEnd = (char*)(&(m_pOrgRec->listOfTrafficVolumes));

    /* 
     * accessPointNameNI,pdpType,servedPDPAddress,dynamicAddressFlag
     */
    memcpy(&(pDest->accessPointNameNI),pBegin,(pEnd - pBegin));

    //�ϲ�listOfTrafficVolumes�ֶ�
    if(m_pCondition != NULL)
    {
		typedef LINK_STRUCT2<CMCC_3GCDR_ChangeOfCharCondition> link_Struct2;
        pDest->listOfTrafficVolumes.value = 
            List_To_Array2<CMCC_3GCDR_CG_ChangeOfCharCondition, link_Struct2>
			(m_pCondition,nCount/*in,out*/);
        
        pDest->listOfTrafficVolumes.count = nCount;

        Clear_List2(m_pCondition);
        m_pCondition = m_pConditionTail = NULL;
    }

    pBegin = (char*)(&(m_pOrgRec->recordOpeningTime));
    pEnd = (char*)(&(m_pOrgRec->diagnostics));
    /* 
     * recordOpeningTime,duration,causeForRecClosing
     */
    memcpy(&(pDest->recordOpeningTime),pBegin,(pEnd - pBegin));

    //�ϲ�diagnostics�ֶ�
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_diagnostics_present)
    {
        memcpy(&(pDest->diagnostics),&(m_Diagnostics),
            sizeof(CMCC_3GCDR_Diagnostics));
        /*
         * m_Diagnosticsά����ָ��ת����pDest��
         */
    }

    //�ϲ�recordSequenceNumber�ֶ�
    if(m_pOrgRec->bit_mask
        & CMCC_3GCDR_GGSNPDPRecord_recordSequenceNumber_present)
    {
        
        if(m_pOrgRec->sgsnAddress.count > 0)
        {
            memcpy(&(pDest->recSequenceNumList.sgsnAddress),
                m_pOrgRec->sgsnAddress.value, sizeof(CMCC_3GCDR_GSNAddress));
        }
        else
        {
            memcpy(&(pDest->recSequenceNumList.sgsnAddress),
                0, sizeof(CMCC_3GCDR_GSNAddress));
        }

        /*
         * m_pSequence�϶���Ϊ�ա�
         */
		typedef LINK_STRUCT1<int> link_Struct1;
        pDest->recSequenceNumList.recSequenceNumber.value = 
            List_To_Array1<int, link_Struct1>
			(m_pSequence,nCount/*in,out*/);

        pDest->recSequenceNumList.bit_mask
            |= CMCC_3GCDR_CG_recSequenceNumber_present;
        pDest->recSequenceNumList.recSequenceNumber.count = nCount;

        Clear_List1(m_pSequence);
        m_pSequence = m_pSequenceTail = NULL;
    }

    //����nodeID�ֶ�(һ������)
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_nodeID_present)
    {
        memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(CMCC_3GCDR_NodeID));
    }

    //�ϲ�recordExtensions�ֶ�
    if(m_pOrgRec->bit_mask
        & CMCC_3GCDR_GGSNPDPRecord_recordExtensions_present)
    {
        if(m_pExtension != NULL)
        {
			typedef LINK_STRUCT2<CMCC_3GCDR_ManagementExtension> link_Struct2;
            pDest->recordExtensions.value = 
				List_To_Array2<CMCC_3GCDR_CG_ManagementExtension, link_Struct2>
				(m_pExtension,nCount/*in,out*/);
            
            pDest->recordExtensions.count = nCount;

            Clear_List2(m_pExtension);
            m_pExtension = m_pExtensionTail = NULL;
        }
    }

	//�ϲ�localSequenceNumber�ֶ�
    if(m_pOrgRec->bit_mask
        & CMCC_3GCDR_GGSNPDPRecord_localSequenceNumber_present)
    {
		//SWPD04515 CG��R99 3a0�Ļ����ϲ�������CMCC�淶 ggf 2004.4.7
        memcpy(&(pDest->localSequenceNumberList.gsnAddress),
                &(m_pOrgRec->ggsnAddress),sizeof(CMCC_3GCDR_GSNAddress));

        /*
         * m_pLocalSequenceNumber�϶���Ϊ�ա�
         */
		typedef LINK_STRUCT1<int> link_Struct;
        pDest->localSequenceNumberList.localSeqNumberList.value = 
            List_To_Array1<int, link_Struct>
			(m_pLocalSequenceNumber,nCount/*in,out*/);

        pDest->localSequenceNumberList.bit_mask
            |= CMCC_3GCDR_CG_localSeqNumberList_present;
        pDest->localSequenceNumberList.localSeqNumberList.count = nCount;

        Clear_List1(m_pLocalSequenceNumber);
        m_pLocalSequenceNumber = m_pLocalSequenceNumberTail = NULL;
    }

    pBegin = (char*)(&(m_pOrgRec->apnSelectionMode));
    pEnd = (char*)(&(m_pOrgRec->sgsnPLMNIdentifier));

    /* 
     * apnSelectionMode,servedMSISDN,chargingCharacteristics,sgsnPLMNIdentifier
     */
    memcpy(&(pDest->apnSelectionMode),pBegin,
            (pEnd - pBegin) + sizeof(CMCC_3GCDR_CG_PLMN_Id));

    //consolidationResult�ֶ�
    switch(m_CauseForRecClosing)
    {
    case CMCC_3GCDR_normalRelease:
        //PDP��������
        pDest->consolidationResult
            = CMCC_3GCDR_CG_normal;
        break;
    case CMCC_3GCDR_sGSNChange:
        pDest->consolidationResult
            = CMCC_3GCDR_CG_forInterSGSNConsolidation;
        break;
    case CMCC_3GCDR_abnormalRelease:
        //PDP�쳣����
        pDest->consolidationResult
            = CMCC_3GCDR_CG_notnormal;
        break;
    default:
        //PDPδ����(��ΪPDP�쳣����)
        pDest->consolidationResult
            = CMCC_3GCDR_CG_notnormal;
        break;
    }

    pDest->duration = m_Duration;
    pDest->causeForRecClosing = m_CauseForRecClosing;

    return (char*)m_pDestRec;
}

UINT4 CR99_GCDR::GetBillType()
{
    return (m_pOrgRec->recordType);
}

CKey CR99_GCDR::CreateKey()
{
    CR99_GCDR_Key* pkey = new CR99_GCDR_Key;
    if(pkey == NULL)
    {
        throw "Alloc memory fail.";
    }

    pkey->chargingID = m_pOrgRec->chargingID;
    pkey->ggsnAddress = m_pOrgRec->ggsnAddress;

    CKey key;
    key.m_pDelegation = pkey;

    return key;
}

BOOL CR99_GCDR::IsDuplicated(CBill* pBill)
{
    BOOL bSucc = FALSE;
    CR99_GCDR* p = (CR99_GCDR*)pBill;
    if((p->m_pOrgRec->bit_mask
        & CMCC_3GCDR_GGSNPDPRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask
        & CMCC_3GCDR_GGSNPDPRecord_recordSequenceNumber_present))
    {
        if((p->m_nCurRecordSequence <= m_nCurRecordSequence)
            && (p->m_nCurRecordSequence >= m_nMinRecordSequence))
        {
            bSucc = TRUE;
        }
    }
    return FALSE;
}

/******************************************************
* ������:   CR99_GCDR::Merge
* ����:    GGF
* ����:    2003-09-28
* ����:    �ѻ���pBill���뵱ǰ�����С�
* ����: 
*       ����1 :CBill* pBill�����ϲ��Ļ�����
* ����ֵ: BOOL���ϲ��ɹ�����TRUE��ʧ�ܷ���FALSE��
* ˵��: 
*       �ϲ������и��µ��ֶ���:
*       ��Ҫ���ӵ��ֶ�(sgsnAddress,listOfTrafficVolumes,
*       recordSequenceNumber,recordExtensions),
*       ȡ���һ�Ż���ֵ���ֶ�(causeForRecClosing,diagnostics),
*       ��Ҫ�ۼӵ��ֶ�(Duration)��
*       ȡ��һ�Ż���ֵ���ֶβ��䡣
*
*       �ڴ����ʧ�ܽ��׳��쳣��
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*
******************************************************/
BOOL CR99_GCDR::Merge(CBill* pBill)
{
    assert(pBill != NULL);

    if(!m_bPrepared)
    {
        if(!PrepareData())  return FALSE;
    }

    CR99_GCDR* p = (CR99_GCDR*)pBill;

    //�����ظ�������
    if((p->m_pOrgRec->bit_mask
        & CMCC_3GCDR_GGSNPDPRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask
        & CMCC_3GCDR_GGSNPDPRecord_recordSequenceNumber_present))
    {
        if((p->m_nCurRecordSequence <= m_nCurRecordSequence)
            && (p->m_nCurRecordSequence >= m_nMinRecordSequence))
        {
            return TRUE;
        }
    }

    //��ͬ��:
    //m_nCurRecordSequence = p->m_pOrgRec->recordSequenceNumber;
    m_nCurRecordSequence++;

    //����PDP������־��
    switch(p->m_pOrgRec->causeForRecClosing)
    {
    case CMCC_3GCDR_normalRelease:
    case CMCC_3GCDR_abnormalRelease:
        m_bPDPCompleted = TRUE;
        break;
    default:
        break;
    }

    //����causeForRecClosing�ֶΡ�
    m_CauseForRecClosing = p->m_pOrgRec->causeForRecClosing;

	if(!m_bDSTFlg)
	{
		int nDiff = Get_Duration(&(m_pOrgRec->recordOpeningTime),
			&(p->m_pOrgRec->recordOpeningTime));
		m_Duration = nDiff;
	}
	//�ۼ�Duration�ֶΡ�
	m_Duration += p->m_pOrgRec->duration;
	//�޸Ľ���

    //����sgsnAddress�ֶΡ�    
    if((p->m_pOrgRec->sgsnAddress.value != NULL)
        && (p->m_pOrgRec->sgsnAddress.count > 0))
    {
        unsigned int nCount = p->m_pOrgRec->sgsnAddress.count;

        if(m_pSGSNAddressTail == NULL)
        {
            //������������ж��ظ�SGSN��ַ

            LINK_STRUCT2<CMCC_3GCDR_GSNAddress>* q = LINK_STRUCT2<CMCC_3GCDR_GSNAddress>::Alloc();

            q->Value = Clone_GSNAddress(
                p->m_pOrgRec->sgsnAddress.value,nCount/*in*/);

            q->nCount = nCount;
            m_pSGSNAddressTail = m_pSGSNAddress = q;
        }
        else
        {
            //��ʱ�ж��Ƿ����ظ�SGSN��ַ

            LINK_STRUCT2<CMCC_3GCDR_GSNAddress>* q = LINK_STRUCT2<CMCC_3GCDR_GSNAddress>::Alloc();

            q->Value = Clone_GSNAddress(
                &(m_pSGSNAddressTail->Value[m_pSGSNAddressTail->nCount - 1]),
                p->m_pOrgRec->sgsnAddress.value,nCount/*in,out*/);

            q->nCount = nCount;
            if(q->Value == NULL)
            {
                //��ʱnCountΪ0
                delete q;
            }
            else
            {
                m_pSGSNAddressTail->m_pNext = q;
                m_pSGSNAddressTail = q;
            }
        }

        /* 
         * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
         * 
         */
        m_nLen_SGSNAddress_List += nCount;
    }

    //����listOfTrafficVolumes�ֶΡ�
    if((p->m_pOrgRec->listOfTrafficVolumes.value != NULL)
        && (p->m_pOrgRec->listOfTrafficVolumes.count > 0))
    {
        LINK_STRUCT2<CMCC_3GCDR_ChangeOfCharCondition>* q
            = LINK_STRUCT2<CMCC_3GCDR_ChangeOfCharCondition>::Alloc();

        q->Value = Clone_ChangeOfCharCondition(
            p->m_pOrgRec->listOfTrafficVolumes.value,
            p->m_pOrgRec->listOfTrafficVolumes.count);

        q->nCount = p->m_pOrgRec->listOfTrafficVolumes.count;

        if(m_pConditionTail == NULL)
        {
            m_pConditionTail = m_pCondition = q;
        }
        else
        {
            m_pConditionTail->m_pNext = q;
            m_pConditionTail = q;
        }

        /* 
         * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
         * 
         */
        m_nLen_Condition_List += q->nCount;
    }

    //����recordExtensions�ֶΡ�
    if((p->m_pOrgRec->bit_mask
        & CMCC_3GCDR_GGSNPDPRecord_recordExtensions_present)
        && (m_pOrgRec->bit_mask
        & CMCC_3GCDR_GGSNPDPRecord_recordExtensions_present))
    {
        if((p->m_pOrgRec->recordExtensions.value != NULL)
            && (p->m_pOrgRec->recordExtensions.count > 0))
        {
            LINK_STRUCT2<CMCC_3GCDR_ManagementExtension>* q
                = LINK_STRUCT2<CMCC_3GCDR_ManagementExtension>::Alloc();
            
            q->Value = Clone_ManagementExtension(
                p->m_pOrgRec->recordExtensions.value,
                p->m_pOrgRec->recordExtensions.count);
            
            q->nCount = p->m_pOrgRec->recordExtensions.count;

            if(m_pExtensionTail == NULL)
            {
                m_pExtensionTail = m_pExtension = q;
            }
            else
            {
                m_pExtensionTail->m_pNext = q;
                m_pExtensionTail = q;
            }

            /* 
             * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
             * 
             */
            m_nLen_Extension_List += q->nCount;
        }
    }

    //����recordSequenceNumber�ֶΡ�
    if((p->m_pOrgRec->bit_mask
        & CMCC_3GCDR_GGSNPDPRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask
        & CMCC_3GCDR_GGSNPDPRecord_recordSequenceNumber_present))
    {
        LINK_STRUCT1<int>* q
            = LINK_STRUCT1<int>::Alloc();
        if(q == NULL) return FALSE;
        
        q->Value = p->m_pOrgRec->recordSequenceNumber;

        if(m_pSequenceTail == NULL)
        {
            m_pSequenceTail = m_pSequence = q;
        }
        else
        {
            m_pSequenceTail->m_pNext = q;
            m_pSequenceTail = q;
        }
    }

	//����localSequenceNumber�ֶΡ�
    if((p->m_pOrgRec->bit_mask
        & CMCC_3GCDR_GGSNPDPRecord_localSequenceNumber_present)
        && (m_pOrgRec->bit_mask
        & CMCC_3GCDR_GGSNPDPRecord_localSequenceNumber_present))
    {
        LINK_STRUCT1<int>* q
            = LINK_STRUCT1<int>::Alloc();
        if(q == NULL) return FALSE;
        
        q->Value = p->m_pOrgRec->localSequenceNumber;

        if(m_pLocalSequenceNumberTail == NULL)
        {
            m_pLocalSequenceNumberTail = m_pLocalSequenceNumber = q;
        }
        else
        {
            m_pLocalSequenceNumberTail->m_pNext = q;
            m_pLocalSequenceNumberTail = q;
        }
    }

    //diagnostics�ֶ�ȡ���һ�Ż�����ֵ��
    if((p->m_pOrgRec->bit_mask
        & CMCC_3GCDR_GGSNPDPRecord_diagnostics_present))
   {
        CMCC_3GCDR_ManagementExtension* q = NULL;
        if(p->m_pOrgRec->diagnostics.choice
            == CMCC_3GCDR_networkSpecificCause_chosen)
        {
            q = Clone_ManagementExtension(
                &(p->m_pOrgRec->diagnostics.u.networkSpecificCause),1);
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CMCC_3GCDR_manufacturerSpecificCause_chosen)
        {
            q = Clone_ManagementExtension(
                &(p->m_pOrgRec->diagnostics.u.manufacturerSpecificCause),1);
        }

        /*����ԭdiagnostics�ֶεĿռ䡣*/
        if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_diagnostics_present) //����һ��if YanYong 2004-04-26 SWPD04795
        {
            if(m_Diagnostics.choice == CMCC_3GCDR_manufacturerSpecificCause_chosen)
            {
                delete[] m_Diagnostics.u.manufacturerSpecificCause.identifier.value;
                delete[] m_Diagnostics.u.manufacturerSpecificCause.information.value;
            }
            else if(m_Diagnostics.choice == CMCC_3GCDR_networkSpecificCause_chosen)
            {
                delete[] m_Diagnostics.u.networkSpecificCause.identifier.value;
                delete[] m_Diagnostics.u.networkSpecificCause.information.value;
            }
        }
        else
        {
            m_pOrgRec->bit_mask |= CMCC_3GCDR_GGSNPDPRecord_diagnostics_present; //����һ�� YanYong 2004-04-26 SWPD04795           
        }

        /************************/
        if(p->m_pOrgRec->diagnostics.choice
            == CMCC_3GCDR_networkSpecificCause_chosen)
        {
            m_Diagnostics.choice = CMCC_3GCDR_networkSpecificCause_chosen;
            
            m_Diagnostics.u.networkSpecificCause = *q;
            delete[] q;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CMCC_3GCDR_manufacturerSpecificCause_chosen)
        {
            m_Diagnostics.choice = CMCC_3GCDR_manufacturerSpecificCause_chosen;
            
            m_Diagnostics.u.manufacturerSpecificCause = *q;
            delete[] q;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CMCC_3GCDR_gsm0408Cause_chosen)
        {
            m_Diagnostics.choice = CMCC_3GCDR_gsm0408Cause_chosen;
            m_Diagnostics.u.gsm0408Cause = 
                p->m_pOrgRec->diagnostics.u.gsm0408Cause;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CMCC_3GCDR_gsm0902MapErrorValue_chosen)
        {
            m_Diagnostics.choice = CMCC_3GCDR_gsm0902MapErrorValue_chosen;
            m_Diagnostics.u.gsm0902MapErrorValue = 
                p->m_pOrgRec->diagnostics.u.gsm0902MapErrorValue;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CMCC_3GCDR_ccittQ767Cause_chosen)
        {
            m_Diagnostics.choice = CMCC_3GCDR_ccittQ767Cause_chosen;
            m_Diagnostics.u.ccittQ767Cause = 
                p->m_pOrgRec->diagnostics.u.ccittQ767Cause;
        }
    }

    //���Ӱ�ͨ��ʱ�������ϲ� ZhengYuqun 2003-12-18
    if((UINT4)m_Duration >= m_uMaxDuration)
    {
        m_bPDPCompleted = TRUE;
    }
    
    /*
     * �����ֶ�ȡ��һ�Ż�����ֵ��
     */
    return TRUE;
}
