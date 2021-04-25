#include "r4_bill.h"
#include "../../merge/merge.h"
#include "../../include/export_dll.h"

extern "C" IGWB_Svc_Export void* CreateParserObject()
{
    return (void*)new C3G_R4_BillParser();
}

C3G_R4_BillParser::C3G_R4_BillParser()
{
    m_pBill = NULL;
}

C3G_R4_BillParser::~C3G_R4_BillParser()
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
BOOL C3G_R4_BillParser::ParseARecord(char* pBuf,UINT4 uBufLen)
{
    assert(pBuf != NULL);
    
    R4_Org_CallEventRecord* pRec = (R4_Org_CallEventRecord*)pBuf;
    switch(pRec->choice)
    {
        case R4_Org_sgsnPDPRecord_chosen:
            m_pBill = new C3G_R4_SCDR;

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

        case R4_Org_ggsnPDPRecord_chosen:
            m_pBill = new C3G_R4_GCDR;
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

        default:
            return FALSE;
            break;
    }

    return TRUE;
}

CBill* C3G_R4_BillParser::GetBillRecord()
{
    CBill* pBill = m_pBill;
    m_pBill = NULL;
    return pBill;
}

UINT4 C3G_R4_BillParser::GetBillTypeCount()
{
    //3G R4��Ҫ�ϲ��Ļ��������֣�S-CDR,G-CDR��
    return 2;
}

UINT4 C3G_R4_BillParser::GetBillTypeID(UINT4 uIndex)
{
    UINT4 uRet = 0;
    switch(uIndex)
    {
    case 0:
        uRet = R4_S_CDR;
        break;
    case 1:
        uRet = R4_G_CDR;
        break;
    default:
        break;
    }
    return uRet;
}


/******************************************************
* ������:   C3G_R4_BillParser::RegisterBillType
* ����:    Guofeng, Guan
* ����:    2003-06-02
* ����:    ��CMerger����ע�Ự�����͡�
* ����: 
*       ����1 :CMerger* pMerger
* ����ֵ: void 
* ˵��:     ���ڴ����ʧ�ܽ��׳��쳣��
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*2003-06-02        Guofeng, Guan
******************************************************/
void C3G_R4_BillParser::RegisterBillType(CMerger* pMerger)
{
    assert(pMerger != NULL);
    
    for(UINT4 i = 0; i < GetBillTypeCount(); i++)
    {
        pMerger->RegisterBillType(GetBillTypeID(i));
    }
}

//���ָ���Ļ�������ID�Ļ�����������,szDescΪ�������
void C3G_R4_BillParser::GetBillTypeDesc(UINT4 uBillTypeID, char* szDesc)
{
    switch(uBillTypeID)
    {
    case R4_S_CDR:
        strcpy(szDesc, "S-CDR");
        break;
    case R4_G_CDR:
        strcpy(szDesc, "G-CDR");
        break;
    default:
        break;
    }
}


//C3G_R4_SCDR��ʵ��(S-CDR)
C3G_R4_SCDR::C3G_R4_SCDR()
{
    m_pCondition = NULL;
    m_pExtension = NULL;
    m_pRecordSequence = NULL;
	m_pLocalSequence = NULL;
	m_pRNCDownlinkVolume = NULL;

    m_bPDPCompleted = FALSE;
    m_bPrepared = FALSE;

    m_nMinRecordSequence = 0;
    m_nCurRecordSequence = 0;

    m_Duration = 0;
    m_UnsentVolume = 0;
    memset(&m_Diagnostics,0,sizeof(R4_Org_Diagnostics));
    m_CauseForRecClosing = R4_Org_normalRelease;

	memset(&m_CAMELInformationPDP, 0, sizeof(R4_Org_CAMELInformationPDP));

    m_pOrgRec = NULL;
    m_pDestRec = NULL;
    
    m_bIsLimit = FALSE;

    m_pRecordSequenceList = NULL;
    m_pRecordSequenceListTail = NULL;
    m_pLocalSequenceList = NULL;
    m_pLocalSequenceListTail = NULL;
    m_pRNCDownlinkVolumeList = NULL;
    m_pRNCDownlinkVolumeListTail = NULL;
}

C3G_R4_SCDR::~C3G_R4_SCDR()
{
    //����ԭʼ������¼�ռ䡣
    if(m_pOrgRec != NULL)
    {
		//2003-07-24 listOfTrafficVolumesΪ��ѡ ggf
		if (m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_listOfTrafficVolumes_present)
		{
			Clear_ChangeOfCharCondition(m_pOrgRec->listOfTrafficVolumes);
		}
		//2003-07-24 end.
		
        if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_diagnostics_present)
        {
            Clear_Diagnostics(&(m_pOrgRec->diagnostics));
        }

        if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_recordExtensions_present)
        {
            //����ֶε������Ƿ���Ч��
            if(m_pOrgRec->recordExtensions != NULL)                
            {
                Clear_ManagementExtensions(m_pOrgRec->recordExtensions);
            }
        }

        delete m_pOrgRec;
        m_pOrgRec = NULL;
    }

    //�������ջ�����¼�ռ䡣
    if(m_pDestRec != NULL)
    {
		//2003-07-24 ���ջ�����sgsnAddressΪ����
		if(m_pDestRec->u.sgsnPDPRecord.bit_mask
			& R4_Fin_SGSNPDPRecord_sgsnAddress_present)
		{
			if (m_pDestRec->u.sgsnPDPRecord.sgsnAddress != NULL)
				Clear_GsnAddressList(m_pDestRec->u.sgsnPDPRecord.sgsnAddress);
		}
		//2003-07-24 end.

		if(m_pDestRec->u.sgsnPDPRecord.bit_mask
			& R4_Fin_SGSNPDPRecord_listOfTrafficVolumes_present)
		{
			if (m_pDestRec->u.sgsnPDPRecord.listOfTrafficVolumes != NULL)
			{
				Clear_ChangeOfCharCondition((R4_Org__seqof1_*)
					m_pDestRec->u.sgsnPDPRecord.listOfTrafficVolumes);
			}
		}

        if(m_pDestRec->u.sgsnPDPRecord.bit_mask
            & R4_Fin_SGSNPDPRecord_diagnostics_present)
        {
            Clear_Diagnostics((R4_Org_Diagnostics*)&
                (m_pDestRec->u.sgsnPDPRecord.diagnostics));
        }

        if(m_pDestRec->u.sgsnPDPRecord.bit_mask
            & R4_Fin_SGSNPDPRecord_recordExtensions_present)
        {
            if(m_pDestRec->u.sgsnPDPRecord.recordExtensions != NULL)
            {
                Clear_ManagementExtensions((R4_Org_ManagementExtensions_*)
                    m_pDestRec->u.sgsnPDPRecord.recordExtensions);
            }
        }

        if(m_pDestRec->u.sgsnPDPRecord.bit_mask
            & R4_Fin_SGSNPDPRecord_recSequenceNumList_present)
        {
			if (m_pDestRec->u.sgsnPDPRecord.recSequenceNumList != NULL)
			{
				Clear_RecSequenceNumList(m_pDestRec->u.sgsnPDPRecord.recSequenceNumList);
			}

        }

		if(m_pDestRec->u.sgsnPDPRecord.bit_mask
            & R4_Fin_SGSNPDPRecord_localSequenceNumberList_present)
        {
			if (m_pDestRec->u.sgsnPDPRecord.localSequenceNumberList != NULL)
			{
				Clear_LocalSequenceNumList(m_pDestRec->u.sgsnPDPRecord.localSequenceNumberList);
			}
			
        }

		if(m_pDestRec->u.sgsnPDPRecord.bit_mask
            & R4_Fin_rNCUnsentDownlinkVolumeList_present)
        {
			if (m_pDestRec->u.sgsnPDPRecord.rNCUnsentDownlinkVolumeList != NULL)
			{
				Clear_RNCDownlinkVolumeList(m_pDestRec->u.sgsnPDPRecord.rNCUnsentDownlinkVolumeList);
			}
			
        }

        delete m_pDestRec;
        m_pDestRec = NULL;
    }

    /*
     * �ϲ�������ʹ�õ����ݽṹ�Ŀռ��Ѿ���GetContents�л��ա�
     */
}


/******************************************************
* ������:   C3G_R4_SCDR::Init
* ����:    Guofeng, Guan
* ����:    2003-06-02
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
*2003-06-02       Zhou Yonghui
******************************************************/
BOOL C3G_R4_SCDR::Init(char* pBuf,UINT4 uLen)
{
    assert(pBuf != NULL);
    
    R4_Org_SGSNPDPRecord* p = (R4_Org_SGSNPDPRecord*)pBuf;

    //����ԭʼ����
    m_pOrgRec = new R4_Org_SGSNPDPRecord;
    if(m_pOrgRec == NULL)
    {
        return FALSE;
    }

    //���ƻ�����¼
    memcpy(m_pOrgRec,p,sizeof(R4_Org_SGSNPDPRecord));

    /*
     * ���浥������ָ���ֶ�
     */

    //����listOfTrafficVolumes�ֶ�
	if(p->bit_mask & R4_Org_SGSNPDPRecord_listOfTrafficVolumes_present)
	{
		if(p->listOfTrafficVolumes != NULL)
		{
		   /* 
            * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
            * 
            */
			R4_Org__seqof1_* pTail = NULL;
			unsigned int nCount = 0;
			m_pOrgRec->listOfTrafficVolumes = Clone_ChangeOfCharCondition(
				p->listOfTrafficVolumes, pTail, nCount);
			m_nLen_Condition_List = nCount;
		}
    }

    //����diagnostics�ֶ�
    if(p->bit_mask & R4_Org_SGSNPDPRecord_diagnostics_present)
    {
        if(p->diagnostics.choice == R4_Org_networkSpecificCause_chosen)
        {
            R4_Org_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(&(p->diagnostics.u.networkSpecificCause));

            m_pOrgRec->diagnostics.choice = R4_Org_networkSpecificCause_chosen;
            m_pOrgRec->diagnostics.u.networkSpecificCause = *q;
            
            delete q;
        }
        else if(p->diagnostics.choice 
            == R4_Org_manufacturerSpecificCause_chosen)
        {
            R4_Org_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(&(p->diagnostics.u.manufacturerSpecificCause));

            m_pOrgRec->diagnostics.choice = R4_Org_manufacturerSpecificCause_chosen;
            m_pOrgRec->diagnostics.u.manufacturerSpecificCause = *q;
            
            delete q;
        }
    }

    //����recordExtensions�ֶ�
    if(p->bit_mask & R4_Org_SGSNPDPRecord_recordExtensions_present)
    {
        if(p->recordExtensions != NULL)
        {
			/* 
             * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
             * 
             */
			R4_Org_ManagementExtensions_* pTail = NULL;
			unsigned int nCount = 0;
			m_pOrgRec->recordExtensions = 
				Clone_ManagementExtensions(p->recordExtensions, pTail, nCount);       
			m_nLen_Extension_List = nCount;
				
        }
    }

    //���õ�ǰ�������к�
    m_nMinRecordSequence = m_nCurRecordSequence = p->recordSequenceNumber;

	//ͨ��ʱ��
	m_Duration = p->duration;

    //����PDP������־
    switch(p->causeForRecClosing)
    {
    case R4_Org_normalRelease:
    case R4_Org_abnormalRelease:
    case R4_Org_sGSNChange:
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

BOOL C3G_R4_SCDR::IsFirstPartialRec()
{
    return (m_pOrgRec->recordSequenceNumber == 0);
}

BOOL C3G_R4_SCDR::IsLastPartialRec()
{
    switch(m_pOrgRec->causeForRecClosing)
    {
    case R4_Org_normalRelease:
    case R4_Org_abnormalRelease:
    case R4_Org_sGSNChange:
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

BOOL C3G_R4_SCDR::IsPartial()
{
    /* 
     * ��recordSequenceNumber�ֶ���Ч���򻰵��ǲ��ֻ�����
     * 
     */
    return (m_pOrgRec->bit_mask &
        R4_Org_SGSNPDPRecord_recordSequenceNumber_present);
}

BOOL C3G_R4_SCDR::IsHotBilling()
{
    if((m_pOrgRec->chargingCharacteristics.value[0] & 0x0f) == 1)
    {
        return TRUE;
    }
    return FALSE;
}


/******************************************************
* ������:   C3G_R4_SCDR::CanSafeMerge
* ����:    Guofeng, Guan
* ����:    2003-06-02
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
*2003-06-02       Guofeng, Guan
******************************************************/
BOOL C3G_R4_SCDR::CanSafeMerge(CBill* pBill)
{
    assert(pBill != NULL);

    if(!m_bPrepared)
    {
        if(!PrepareData())  return FALSE;
    }

    C3G_R4_SCDR* p = (C3G_R4_SCDR*)pBill;

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
    if((p->m_pOrgRec->bit_mask 
        & R4_Org_SGSNPDPRecord_sgsnAddress_present)
        && (m_pOrgRec->bit_mask 
        & R4_Org_SGSNPDPRecord_sgsnAddress_present))
    {		
        if(Match_gsnAddress(&(p->m_pOrgRec->sgsnAddress), 
                             (R4_Org_GSNAddress*)(&(m_pSGSNAddressTail->value))))
        {
            if((m_nCurRecordSequence - m_nMinRecordSequence + 2)
                > MAX_MERGED_BILLS)
            {
    	        m_bIsLimit = TRUE;
                return FALSE;
            }
        }
        else
        {
           //SGSN����Ѿ������л�, ��Ҫ����ϲ������Ļ���ԭ��ΪsGSNChange��
           //�ϲ�������sgsnChange�ֶ���λ��������ϲ�
            if((m_CauseForRecClosing != R4_Org_sGSNChange)
              || (!(p->m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_sgsnChange_present))
              || (!(p->m_pOrgRec->sgsnChange)))
            {
                return FALSE;
            }
        }
    }
    else
    {
        return FALSE;
    }


    if((m_nLen_Condition_List + p->m_nLen_Condition_List)
        > MAX_MERGED_BILLS)
    {
    	m_bIsLimit = TRUE;
        return FALSE;
    }

    if((m_nLen_Extension_List + p->m_nLen_Extension_List)
        > MAX_MERGED_BILLS)
    {
    	m_bIsLimit = TRUE;
        return FALSE;
    }

	/*
	 * �ϲ�CAMEL����freeFormatData,
	 * �ϲ��Ļ���������length�ʹ���160
	 * ǿ������
	 * 
	 */
	if((m_pOrgRec->bit_mask 
		    & R4_Org_cAMELInformationPDP_present)
		&& (p->m_pOrgRec->bit_mask 
		    & R4_Org_cAMELInformationPDP_present)
		&& (m_pOrgRec->cAMELInformationPDP.bit_mask
		    & R4_Org_CAMELInformationPDP_freeFormatData_present)
		&& (p->m_pOrgRec->cAMELInformationPDP.bit_mask
		    & R4_Org_CAMELInformationPDP_freeFormatData_present)
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

BOOL C3G_R4_SCDR::IsPDPCompleted()
{
    return m_bPDPCompleted;
}

BOOL C3G_R4_SCDR::NeedWaiting()
{
    //��ǰ�ĺϲ�������ü�����ʽ�ϲ�Duration�ֶ�
    //���Ըú���ֻ����FALSE(��PDP����ʱ��������ȴ�)��
    return FALSE;
}


/******************************************************
* ������:   C3G_R4_SCDR::GetContents
* ����:    Guofeng, Guan
* ����:    2003-06-02
* ����:    �����������ջ�����ʽ��ϻ�����¼��
* ����: 
* ����ֵ: char*,�������ջ�����ʽ�Ļ�����¼��
* ˵��: 
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*2003-06-02       Guofeng, Guan
******************************************************/
char* C3G_R4_SCDR::GetContents()
{
    if(m_pDestRec != NULL)
    {
        return (char*)m_pDestRec;
    }

    R4_Fin_CallEventRecord* pCallEventRec = new R4_Fin_CallEventRecord;
    if(pCallEventRec == NULL)
    {
        throw "Alloc memory fail.";
    }

    pCallEventRec->choice = R4_Fin_sgsnPDPRecord_chosen;
    R4_Fin_SGSNPDPRecord* pDest = &(pCallEventRec->u.sgsnPDPRecord);

    //��ʼ��Ϊ0��
    memset(pDest,0,sizeof(R4_Fin_SGSNPDPRecord));
    m_pDestRec = pCallEventRec;

    //�������ڴ�鿪ʼ/������ַ��
    char* pBegin = NULL,*pEnd = NULL;

    /*
     * ������û�о����ϲ������ԭʼ�����й������ջ�����
     */
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_networkInitiation_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNPDPRecord_networkInitiation_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_servedIMEI_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNPDPRecord_servedIMEI_present;
    }
	if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_sgsnAddress_present)
	{
		pDest->bit_mask |= R4_Fin_SGSNPDPRecord_sgsnAddress_present;
	}
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_msNetworkCapability_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNPDPRecord_msNetworkCapability_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_routingArea_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNPDPRecord_routingArea_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_locationAreaCode_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNPDPRecord_locationAreaCode_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_cellIdentifier_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNPDPRecord_cellIdentifier_present;
    }
	if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_accessPointNameNI_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNPDPRecord_accessPointNameNI_present;
    }
	if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_pdpType_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNPDPRecord_pdpType_present;
    }
	if (m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_servedPDPAddress_present)
	{
		pDest->bit_mask |= R4_Fin_SGSNPDPRecord_servedPDPAddress_present;
	}
	if (m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_listOfTrafficVolumes_present)
	{
		pDest->bit_mask |= R4_Fin_SGSNPDPRecord_listOfTrafficVolumes_present;
	}
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_sgsnChange_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNPDPRecord_sgsnChange_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_diagnostics_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNPDPRecord_diagnostics_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_recordSequenceNumber_present)		
    {
        pDest->bit_mask |= R4_Fin_SGSNPDPRecord_recSequenceNumList_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_nodeID_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNPDPRecord_nodeID_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_recordExtensions_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNPDPRecord_recordExtensions_present;
    }
	if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_localSequenceNumber_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNPDPRecord_localSequenceNumberList_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_apnSelectionMode_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNPDPRecord_apnSelectionMode_present;
    }
	if(m_pOrgRec->bit_mask & R4_Org_accessPointNameOI_present)
    {
        pDest->bit_mask |= R4_Fin_accessPointNameOI_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_servedMSISDN_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNPDPRecord_servedMSISDN_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_systemType_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNPDPRecord_systemType_present;
    }
	if(m_pOrgRec->bit_mask & R4_Org_cAMELInformationPDP_present)
	{
		pDest->bit_mask |=  R4_Fin_cAMELInformationPDP_present;
	}
    if(m_pOrgRec->bit_mask & R4_Org_rNCUnsentDownlinkVolume_present)
    {
        pDest->bit_mask |= R4_Fin_rNCUnsentDownlinkVolumeList_present;
    }
	if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_chChSelectionMode_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNPDPRecord_chChSelectionMode_present;
    }
	if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_dynamicAddressFlag_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNPDPRecord_dynamicAddressFlag_present;
    }

    if(!m_bPrepared)
    {
        pBegin = (char*)(&(m_pOrgRec->recordType));
		//2003-07-24 ���ջ�����sgsnAddressΪ����Ӧ��������ǰ��һ����servedIMEIΪֹ
		pEnd = (char*)(&(m_pOrgRec->sgsnAddress));

        /*
         * recordType,networkInitiation,servedIMSI,
         * servedIMEI
         */
        memcpy(&(pDest->recordType),pBegin,(pEnd - pBegin));
		
		/*
         * sgsnAddressת��Ϊ����
         */

		if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_sgsnAddress_present)
		{
			pDest->sgsnAddress = new R4_Fin__seqof2_;
            if(pDest->sgsnAddress == NULL)
            {
                throw "Alloc memory fail.";
            }

			memcpy(&(pDest->sgsnAddress->value),
				   &(m_pOrgRec->sgsnAddress), sizeof(R4_Org_GSNAddress));          
            pDest->sgsnAddress->next = NULL;
		}

		pBegin = (char*)(&(m_pOrgRec->msNetworkCapability));
		pEnd = (char*)(&(m_pOrgRec->listOfTrafficVolumes));
		/*
		 * msNetworkCapability,routingArea,
         * locationAreaCode,cellIdentifier,chargingID,ggsnAddressUsed,
         * accessPointNameNI,pdpType,servedPDPAddress
		 */
		memcpy(&(pDest->msNetworkCapability),pBegin,(pEnd - pBegin));
		//2003-07-24 end.

        //����listOfTrafficVolumes�ֶ�
		if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_listOfTrafficVolumes_present)
		{
			if(m_pOrgRec->listOfTrafficVolumes != NULL)
			{
				R4_Org__seqof1_* pTail = NULL;
				unsigned int nCount = 0;
				pDest->listOfTrafficVolumes = (R4_Fin__seqof3_*)
					Clone_ChangeOfCharCondition(m_pOrgRec->listOfTrafficVolumes, pTail, nCount);
			}
		}

        pBegin = (char*)(&(m_pOrgRec->recordOpeningTime));
        pEnd = (char*)(&(m_pOrgRec->diagnostics));

        /*
         * recordOpeningTime,duration,sgsnChange,causeForRecClosing
         */
        memcpy(&(pDest->recordOpeningTime),pBegin,(pEnd - pBegin));

        //����diagnostics�ֶ�
        if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_diagnostics_present)
        {
            memcpy(&(pDest->diagnostics),&(m_pOrgRec->diagnostics),
                sizeof(R4_Org_Diagnostics));

            if(m_pOrgRec->diagnostics.choice
                == R4_Org_networkSpecificCause_chosen)
            {
                R4_Org_ManagementExtension* q = NULL;
                q = Clone_ManagementExtension(
                    &(m_pOrgRec->diagnostics.u.networkSpecificCause));

                pDest->diagnostics.choice = R4_Org_networkSpecificCause_chosen;
                pDest->diagnostics.u.networkSpecificCause
                    = *(R4_Fin_ManagementExtension*)q;

                delete q;
            }
            else if(m_pOrgRec->diagnostics.choice
                == R4_Org_manufacturerSpecificCause_chosen)
            {
                R4_Org_ManagementExtension* q = NULL;
                q = Clone_ManagementExtension(
                    &(m_pOrgRec->diagnostics.u.manufacturerSpecificCause));

                pDest->diagnostics.choice = R4_Org_manufacturerSpecificCause_chosen;
                pDest->diagnostics.u.manufacturerSpecificCause
                    = *(R4_Fin_ManagementExtension*)q;

                delete q;
            }
        }

        /*
         * recordSequenceNumberת��Ϊ����
         */
        if(m_pOrgRec->bit_mask
            & R4_Org_SGSNPDPRecord_recordSequenceNumber_present)			
        {
			pDest->recSequenceNumList = new R4_Fin_SGSNPDPRecord::R4_Fin__seqof4;
			if(pDest->recSequenceNumList == NULL)
			{
				throw "Alloc memory fail.";
			}

            pDest->recSequenceNumList->value.recSequenceNumberList = new R4_Fin__seqof1_;
            if(pDest->recSequenceNumList->value.recSequenceNumberList == NULL)
            {
                throw "Alloc memory fail.";
            }

            pDest->recSequenceNumList->value.recSequenceNumberList->value
                = m_pOrgRec->recordSequenceNumber;
            pDest->recSequenceNumList->value.recSequenceNumberList->next = NULL;
			pDest->recSequenceNumList->next = NULL;

            //���ⵥ SWPD05036 YanYong 2004-05-10
            pDest->recSequenceNumList->value.bit_mask |= R4_Fin_recSequenceNumberList_present;
            //</SWPD05036>

			if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_sgsnAddress_present)
			{
				memcpy(&(pDest->recSequenceNumList->value.sgsnAddress),
                   &(m_pOrgRec->sgsnAddress),sizeof(R4_Org_GSNAddress));            
			}
			else
			{				
				//��SGSN��ַ��Ч���������е�SGSN��ַ����Ϊ��
                memset(&(pDest->recSequenceNumList->value.sgsnAddress),
                    0,sizeof(R4_Org_GSNAddress));
			}
        }

        //����nodeID�ֶ�
        if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_nodeID_present)
        {
            memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(R4_Org_NodeID));
        }

        //����recordExtensions�ֶ�
        if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_recordExtensions_present)
        {
            if(m_pOrgRec->recordExtensions != NULL)
            {
				R4_Org_ManagementExtensions_* pTail = NULL;
				unsigned int nCount = 0;
                pDest->recordExtensions = (R4_Fin_ManagementExtensions_*)
                    Clone_ManagementExtensions(m_pOrgRec->recordExtensions, pTail, nCount);
            }
        }

		/*
         * localSequenceNumberת��Ϊ����
         */
        if(m_pOrgRec->bit_mask
            & R4_Org_SGSNPDPRecord_localSequenceNumber_present)			
        {
			pDest->localSequenceNumberList = new R4_Fin_SGSNPDPRecord::R4_Fin__seqof5;
			if(pDest->localSequenceNumberList == NULL)
			{
				throw "Alloc memory fail.";
			}

            pDest->localSequenceNumberList->value.localSeqNumberList = new R4_Fin__seqof1_;
            if(pDest->localSequenceNumberList->value.localSeqNumberList == NULL)
            {
                throw "Alloc memory fail.";
            }

            pDest->localSequenceNumberList->value.localSeqNumberList->value
                = m_pOrgRec->localSequenceNumber;
            pDest->localSequenceNumberList->value.localSeqNumberList->next = NULL;
			pDest->localSequenceNumberList->next = NULL;
			pDest->localSequenceNumberList->value.bit_mask |= R4_Fin_localSeqNumberList_present;
			
			if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_sgsnAddress_present)
			{
				memcpy(&(pDest->localSequenceNumberList->value.gsnAddress),
                   &(m_pOrgRec->sgsnAddress),sizeof(R4_Org_GSNAddress));            
			}
			else
			{				
				//��SGSN��ַ��Ч���������е�SGSN��ַ����Ϊ��
                memset(&(pDest->localSequenceNumberList->value.gsnAddress),
                    0,sizeof(R4_Org_GSNAddress));
				//2003-07-29 ggf ��Ҫ��choice��ȷ��λ���������ʧ��
				//pDest->recSequenceNumList->value.sgsnAddress.choice = R4_Fin_iPAddress_chosen;
				//pDest->recSequenceNumList->value.sgsnAddress.u.iPBinaryAddress.choice = R4_Fin_iPBinaryAddress_chosen;
				//2003-07-29 ggf.
			}
        }

        pBegin = (char*)(&(m_pOrgRec->apnSelectionMode));
        pEnd = (char*)(&(m_pOrgRec->cAMELInformationPDP));

        /*
         * apnSelectionMode,accessPggsnPDPRecordointNameOI,servedMSISDN,
         * chargingCharacteristics,systemType, cAMELInformationPDP,
		 * 
         */
		memcpy(&(pDest->apnSelectionMode),pBegin,
            (pEnd - pBegin) + sizeof(R4_Fin_CAMELInformationPDP));

		/*
         * rNCDownlinkVolumeת��Ϊ����
         */
        if(m_pOrgRec->bit_mask
            & R4_Org_rNCUnsentDownlinkVolume_present)			
        {
			pDest->rNCUnsentDownlinkVolumeList = new R4_Fin_SGSNPDPRecord::R4_Fin__seqof6;
			if(pDest->rNCUnsentDownlinkVolumeList == NULL)
			{
				throw "Alloc memory fail.";
			}

            pDest->rNCUnsentDownlinkVolumeList->value.rNCUnsentDnVolumeList = new R4_Fin__seqof1_;
            if(pDest->rNCUnsentDownlinkVolumeList->value.rNCUnsentDnVolumeList == NULL)
            {
                throw "Alloc memory fail.";
            }

            pDest->rNCUnsentDownlinkVolumeList->value.rNCUnsentDnVolumeList->value
                = m_pOrgRec->rNCUnsentDownlinkVolume;
            pDest->rNCUnsentDownlinkVolumeList->value.rNCUnsentDnVolumeList->next = NULL;
			pDest->rNCUnsentDownlinkVolumeList->next = NULL;
			pDest->rNCUnsentDownlinkVolumeList->value.bit_mask |= R4_Fin_rNCUnsentDnVolumeList_present;
			
			if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_sgsnAddress_present)
			{
				memcpy(&(pDest->rNCUnsentDownlinkVolumeList->value.sgsnAddress),
                   &(m_pOrgRec->sgsnAddress),sizeof(R4_Org_GSNAddress));            
			}
			else
			{				
				//��SGSN��ַ��Ч���������е�SGSN��ַ����Ϊ��
                memset(&(pDest->rNCUnsentDownlinkVolumeList->value.sgsnAddress),
                    0,sizeof(R4_Org_GSNAddress));
			}
        }

		pBegin = (char*)(&(m_pOrgRec->chChSelectionMode));
		pEnd = (char*)(&(m_pOrgRec->dynamicAddressFlag));
		/*
         * chChSelectionMode, dynamicAddressFlag,
		 * 
         */
		memcpy(&(pDest->chChSelectionMode),pBegin,
            (pEnd - pBegin) + sizeof(R4_Org_DynamicAddressFlag));
        
        //consolidationResult�ֶ�
        switch(m_pOrgRec->causeForRecClosing)
        {
        case R4_Org_normalRelease:
            pDest->consolidationResult = R4_Fin_normal;
            break;
        case R4_Org_sGSNChange:
            //PDP��������
            pDest->consolidationResult = R4_Fin_forInterSGSNConsolidation;
            break;
        case R4_Org_abnormalRelease:
            //PDP�쳣����
            pDest->consolidationResult = R4_Fin_notNormal;
            break;
        case R4_Org_timeLimit:
            pDest->consolidationResult = R4_Fin_reachLimit;
        default:
            //PDPδ����(��ΪPDP�쳣����)
			if (m_bIsLimit)
			{
        		pDest->consolidationResult = R4_Fin_reachLimit;
			}
			else
			{
        		pDest->consolidationResult = R4_Fin_notNormal;
			}
			break;
		}
    
		if (!IsPartial())
		{
    		pDest->consolidationResult = R4_Fin_onlyOneCDRGenerated;	
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
     * servedIMEI
     */
    memcpy(&(pDest->recordType),pBegin,(pEnd - pBegin));

	//�ϲ�sgsnAddress�ֶ�
	if(m_pSGSNAddress != NULL)
	{
		pDest->sgsnAddress = m_pSGSNAddress;
		m_pSGSNAddress = m_pSGSNAddressTail = NULL;
	}
    else if(m_pOrgRec->bit_mask | R4_Org_SGSNPDPRecord_sgsnAddress_present)
    {
		pDest->sgsnAddress = new R4_Fin__seqof2_;
		if (pDest->sgsnAddress == NULL)
		{
			throw "Alloc memory fail.";
		}

		memcpy(&(pDest->sgsnAddress->value),
               &(m_pOrgRec->sgsnAddress),
               sizeof(R4_Org_GSNAddress));		
		pDest->sgsnAddress->next = NULL;
    }
    else
    {
		pDest->sgsnAddress = NULL;
    }

	/* msNetworkCapability,
     * routingArea,locationAreaCode,cellIdentifier,
     * chargingID,ggsnAddressUsed,accessPointNameNI,
     * pdpType,servedPDPAddress
     */
	pBegin = (char*)(&(m_pOrgRec->msNetworkCapability));
	pEnd = (char*)(&(m_pOrgRec->listOfTrafficVolumes));
	memcpy(&(pDest->msNetworkCapability),pBegin,(pEnd - pBegin));

    //�ϲ�listOfTrafficVolumes�ֶ�
    if(m_pCondition != NULL)
    {
        pDest->listOfTrafficVolumes = (R4_Fin__seqof3_*) m_pCondition;

        m_pCondition = m_pConditionTail = NULL;
    }

	pBegin = (char*)(&(m_pOrgRec->recordOpeningTime));
    pEnd = (char*)(&(m_pOrgRec->diagnostics));
	
	/* 
     * recordOpeningTime,duration,sgsnChange,
     * causeForRecClosing
     */
    
	memcpy(&(pDest->recordOpeningTime),pBegin,(pEnd - pBegin));

    //�ϲ�diagnostics�ֶ�
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_diagnostics_present)
    {
        memcpy(&(pDest->diagnostics),&(m_Diagnostics),
            sizeof(R4_Org_Diagnostics));
    }

    //�ϲ�recordSequenceNumber�ֶ�
    if(m_pOrgRec->bit_mask
        & R4_Org_SGSNPDPRecord_recordSequenceNumber_present)		
    {		        
        /*
         * m_pSequenceһ����ΪNULL��
         */
		pDest->recSequenceNumList = m_pRecordSequenceList;
        m_pRecordSequenceList = m_pRecordSequenceListTail = NULL;
        m_pRecordSequenceTail = NULL;
    }

    //nodeID�ֶ�(һ������)
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_nodeID_present)
    {
        memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(R4_Org_NodeID));
    }

    //�ϲ�recordExtensions�ֶ�
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_recordExtensions_present)
    {
        if(m_pExtension != NULL)
        {
            pDest->recordExtensions = (R4_Fin_ManagementExtensions_*) m_pExtension;
            
            m_pExtension = m_pExtensionTail = NULL;
        }
    }

	//�ϲ�localSequenceNumber�ֶ�
    if(m_pOrgRec->bit_mask
        & R4_Org_SGSNPDPRecord_localSequenceNumber_present)		
    {		        
        /*
         * m_pSequenceһ����ΪNULL��
         */
		pDest->localSequenceNumberList = m_pLocalSequenceList;
        m_pLocalSequenceList = m_pLocalSequenceListTail = NULL;
        m_pLocalSequenceTail = NULL;
   }

    pBegin = (char*)(&(m_pOrgRec->apnSelectionMode));
    pEnd = (char*)(&(m_pOrgRec->cAMELInformationPDP));

    /*
     * apnSelectionMode,accessPointNameOI,
     * servedMSISDN,chargingCharacteristics,
     * systemType,cAMELInformationPDP;
	 * 
     */
    //ע��zouyongsheng ����CAMEL����ڴ˲�����������޸Ĵ���
	memcpy(&(pDest->apnSelectionMode),pBegin,
        (pEnd - pBegin) + sizeof(R4_Org_CAMELInformationPDP));

	//�ϲ�rNCDownlinkVolume�ֶ�
    if(m_pRNCDownlinkVolumeList != NULL)		
    {		        
        /*
         * m_pRNCDownlinkSequenceһ����ΪNULL��
         */
		pDest->rNCUnsentDownlinkVolumeList = m_pRNCDownlinkVolumeList;
        m_pRNCDownlinkVolumeList = m_pRNCDownlinkVolumeListTail = NULL;
        m_pRNCDownlinkVolumeTail = NULL;
    }

	pBegin = (char*)(&(m_pOrgRec->chChSelectionMode));
    pEnd = (char*)(&(m_pOrgRec->dynamicAddressFlag));

    /*
     * chChSelectionMode,dynamicAddressFlag,
	 * 
     */
    //ע��zouyongsheng ����CAMEL����ڴ˲�����������޸Ĵ���
	memcpy(&(pDest->chChSelectionMode),pBegin,
        (pEnd - pBegin) + sizeof(R4_Org_DynamicAddressFlag));

	//consolidationResult�ֶ�
    switch(m_CauseForRecClosing)
    {
    case R4_Org_normalRelease:
        pDest->consolidationResult = R4_Fin_normal;
        break;
    case R4_Org_sGSNChange:
        //PDP��������
        pDest->consolidationResult = R4_Fin_forInterSGSNConsolidation;
        break;
    case R4_Org_abnormalRelease:
        //PDP�쳣����
        pDest->consolidationResult = R4_Fin_notNormal;
        break;
    case R4_Org_timeLimit:
        pDest->consolidationResult = R4_Fin_reachLimit;
        break;
    default:
        //PDPδ����(��ΪPDP�쳣����)
        if (m_bIsLimit)
        {
        	pDest->consolidationResult = R4_Fin_reachLimit;
        }
        else
        {
        	pDest->consolidationResult = R4_Fin_notNormal;
        }
        break;
    }
    
    if (!IsPartial())
    {
    	pDest->consolidationResult = R4_Fin_onlyOneCDRGenerated;	
    }

    pDest->duration = m_Duration;
    pDest->causeForRecClosing = m_CauseForRecClosing;

	memcpy(&(pDest->cAMELInformationPDP), 
		&(m_CAMELInformationPDP), sizeof(R4_Org_CAMELInformationPDP));
    
	return (char*)m_pDestRec;
}

UINT4 C3G_R4_SCDR::GetBillType()
{
    return m_pOrgRec->recordType;
}

CKey C3G_R4_SCDR::CreateKey()
{
    C3G_R4_SCDR_Key* pkey = new C3G_R4_SCDR_Key;
    if(pkey == NULL)
    {
        throw "Alloc memory fail.";
    }
    pkey->chargingID         = m_pOrgRec->chargingID;
    pkey->ggsnAddressUsed    = m_pOrgRec->ggsnAddressUsed;

	//2003-07-24 S-CDR��sgsnAddressΪ��ѡ ggf
	/*if (m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_sgsnAddress_present)
	{
		pkey->sgsnAddress        = m_pOrgRec->sgsnAddress;
		pkey->m_bSgsnAddressPresent = TRUE;
	}
	else
		pkey->m_bSgsnAddressPresent = FALSE;
    */
	//2003-07-24 end.

    CKey key;
    key.m_pDelegation = pkey;

    return key;
}


/******************************************************
* ������:   C3G_R4_SCDR::PrepareData
* ����:    Guofeng, Guan
* ����:    2003-06-02
* ����:    Ϊ�ϲ�׼�����ݡ�
* ����:    �ޡ�
* ����ֵ: BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE��
* ˵��:    �ϲ���������Ҫ��һЩ�ֶ������������γ�һ������
* 
* 
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*2003-06-02       Guofeng, Guan
******************************************************/
BOOL C3G_R4_SCDR::PrepareData()
{
	//2003-07-24 ����sgsnAddress ggf
	if(m_pOrgRec->bit_mask
		& R4_Org_SGSNPDPRecord_sgsnAddress_present)
	{
		m_pSGSNAddress = new R4_Fin__seqof2_;
		if (m_pSGSNAddress == NULL)
		{
			throw "Alloc memory fail.";
		}

		memcpy(&(m_pSGSNAddress->value), &(m_pOrgRec->sgsnAddress), sizeof(R4_Org_GSNAddress));		
		m_pSGSNAddress->next = NULL;
		m_pSGSNAddressTail = m_pSGSNAddress;
	}
	//2003-07-24 end.

    //����listOfTrafficVolumes
	if(m_pOrgRec->bit_mask
		& R4_Org_SGSNPDPRecord_listOfTrafficVolumes_present)
	{
		if(m_pOrgRec->listOfTrafficVolumes != NULL)
		{
			R4_Org__seqof1_* pTail = NULL;
			unsigned int nCount = 0;
			m_pCondition = Clone_ChangeOfCharCondition(m_pOrgRec->listOfTrafficVolumes, pTail, nCount);
			m_pConditionTail = pTail;
		}
    }

    //����recordExtensions
    if(m_pOrgRec->bit_mask 
        & R4_Org_SGSNPDPRecord_recordExtensions_present)
    {
        if(m_pOrgRec->recordExtensions != NULL)
        {
			R4_Org_ManagementExtensions_* pTail = NULL;
			unsigned int nCount = 0;
            m_pExtension = 
				Clone_ManagementExtensions(m_pOrgRec->recordExtensions, pTail, nCount);                

            m_pExtensionTail = pTail;
        }
    }

    //����recordSequenceNumber
    if(m_pOrgRec->bit_mask 
        & R4_Org_SGSNPDPRecord_recordSequenceNumber_present)
    {
		R4_Fin__seqof1_* q = new R4_Fin__seqof1_;
        if (q == NULL)
		{
			throw "Alloc memory fail.";
		}

        q->value = m_pOrgRec->recordSequenceNumber;
		q->next = NULL;

        R4_Fin_SGSNPDPRecord::R4_Fin__seqof4* pList = 
                                     new R4_Fin_SGSNPDPRecord::R4_Fin__seqof4;
        if(pList == NULL)
		{
			throw "Alloc memory fail.";
		}
        pList->next = NULL;

        pList->value.recSequenceNumberList = q;
        pList->value.bit_mask |= R4_Fin_recSequenceNumberList_present;

		memcpy(&(pList->value.sgsnAddress),
           &(m_pOrgRec->sgsnAddress), sizeof(R4_Org_GSNAddress));            
        
        m_pRecordSequenceList = pList;
        m_pRecordSequenceListTail = pList;

        m_pRecordSequenceTail = q;
    }

	//����localSequenceNumber
    if(m_pOrgRec->bit_mask 
        & R4_Org_SGSNPDPRecord_localSequenceNumber_present)
    {

		R4_Fin__seqof1_* q = new R4_Fin__seqof1_;
        if (q == NULL)
		{
			throw "Alloc memory fail.";
		}
		
        q->value = m_pOrgRec->localSequenceNumber;
		q->next = NULL;

        R4_Fin_SGSNPDPRecord::R4_Fin__seqof5* pList = 
                                     new R4_Fin_SGSNPDPRecord::R4_Fin__seqof5;
        if(pList == NULL)
		{
			throw "Alloc memory fail.";
		}
        pList->next = NULL;

        pList->value.localSeqNumberList = q;
        pList->value.bit_mask |= R4_Fin_localSeqNumberList_present;

		memcpy(&(pList->value.gsnAddress),
           &(m_pOrgRec->sgsnAddress), sizeof(R4_Org_GSNAddress));            
        
        m_pLocalSequenceList = pList;
        m_pLocalSequenceListTail = pList;

        m_pLocalSequenceTail = q;
    }

    //����duration�ֶ�
    m_Duration = m_pOrgRec->duration;

	//����cAMELInformationPDP
	if(m_pOrgRec->bit_mask
		& R4_Org_cAMELInformationPDP_present)
	{
		m_CAMELInformationPDP = m_pOrgRec->cAMELInformationPDP;
	}

	//����rNCUnsentDownlinkVolume�ֶ�
    if(m_pOrgRec->bit_mask
        & R4_Org_rNCUnsentDownlinkVolume_present)
    {
		R4_Fin__seqof1_* q = new R4_Fin__seqof1_;
        if (q == NULL)
		{
			throw "Alloc memory fail.";
		}
		
        q->value = m_pOrgRec->rNCUnsentDownlinkVolume;
		q->next = NULL;

        R4_Fin_SGSNPDPRecord::R4_Fin__seqof6* pList = 
                                     new R4_Fin_SGSNPDPRecord::R4_Fin__seqof6;
        if(pList == NULL)
		{
			throw "Alloc memory fail.";
		}
        pList->next = NULL;

        pList->value.rNCUnsentDnVolumeList = q;
        pList->value.bit_mask |= R4_Fin_rNCUnsentDnVolumeList_present;

		memcpy(&(pList->value.sgsnAddress),
           &(m_pOrgRec->sgsnAddress), sizeof(R4_Org_GSNAddress));            
        
        m_pRNCDownlinkVolumeList = pList;
        m_pRNCDownlinkVolumeListTail = pList;
        
        m_pRNCDownlinkVolumeTail = q;            
    }

    //����causeForRecClosing�ֶ�
    m_CauseForRecClosing = m_pOrgRec->causeForRecClosing;

    //����diagnostics�ֶ�
    if(m_pOrgRec->bit_mask
        & R4_Org_SGSNPDPRecord_diagnostics_present)
    {
        m_Diagnostics = m_pOrgRec->diagnostics;
        
        if(m_Diagnostics.choice == R4_Org_networkSpecificCause_chosen)
        {
            R4_Org_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(&(m_pOrgRec->diagnostics.u.networkSpecificCause));

            m_Diagnostics.choice = R4_Org_networkSpecificCause_chosen;
            m_Diagnostics.u.networkSpecificCause = *q;
            
            delete[] q;
        }
        else if(m_Diagnostics.choice 
            == R4_Org_manufacturerSpecificCause_chosen)
        {
            R4_Org_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(&(m_pOrgRec->diagnostics.u.manufacturerSpecificCause));

            m_Diagnostics.choice = R4_Org_manufacturerSpecificCause_chosen;
            m_Diagnostics.u.manufacturerSpecificCause = *q;
            
            delete[] q;
        }
    }

    //׼�����
    m_bPrepared = TRUE;
    return TRUE;
}

BOOL C3G_R4_SCDR::IsDuplicated(CBill* pBill)
{
    C3G_R4_SCDR* p = (C3G_R4_SCDR*)pBill;
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
* ������:   C3G_R4_SCDR::Merge
* ����:    Guofeng, Guan
* ����:    2003-06-02
* ����:    �ѻ���pBill���뵱ǰ�����С�
* ����: 
*       ����1 :CBill* pBill�����ϲ��Ļ�����
* ����ֵ: BOOL���ϲ��ɹ�����TRUE��ʧ�ܷ���FALSE��
* ˵��: 
*       �ϲ������и��µ��ֶ���:
*       ��Ҫ���ӵ��ֶ�(sgsnAddress,listOfTrafficVolumes,recordSequenceNumber,
*       recordExtensions),ȡ���һ�Ż���ֵ���ֶ�(causeForRecClosing,
*       diagnostics),��Ҫ�ۼӵ��ֶ�(Duration,rNCUnsentDownlinkVolume)��
*       ȡ��һ�Ż���ֵ���ֶβ��䡣
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*2003-06-02       Guofeng, Guan
******************************************************/
BOOL C3G_R4_SCDR::Merge(CBill* pBill)
{
    assert(pBill != NULL);

    if(!m_bPrepared)
    {
        if(!PrepareData())  return FALSE;
    }


    C3G_R4_SCDR* p = (C3G_R4_SCDR*)pBill;

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
    case R4_Org_normalRelease:
    case R4_Org_abnormalRelease:
    //comment by chenliangwei, 2004-06-23, for diffrent sgsn merge
    //case R4_Org_sGSNChange:
        m_bPDPCompleted = TRUE;
        break;
    default:
        break;
    }

    //����causeForRecClosing�ֶΡ�
    m_CauseForRecClosing = p->m_pOrgRec->causeForRecClosing;
    if(p->m_pOrgRec->bit_mask & R4_Org_rNCUnsentDownlinkVolume_present)
    {
        m_pOrgRec->bit_mask |= R4_Org_rNCUnsentDownlinkVolume_present;
    }


	//Z30971�����޸��Ƿ�֧����ʱ��
	if(!m_bDSTFlg)
	{
		//duration���ռ�����ʽ����
        int nDiff = Get_Duration(&(m_pOrgRec->recordOpeningTime),
                                 &(p->m_pOrgRec->recordOpeningTime));
        m_Duration = nDiff;
	}
    m_Duration += p->m_pOrgRec->duration;
	//�޸Ľ���

    //Added by ZhengYuqun 2003-11-04 SWPD02238
    if((UINT4)m_Duration >= m_uMaxDuration)
    {
        m_bPDPCompleted = TRUE;
    }

	//2003-07-24 ����sgsnAddress�ֶ� ggf
    BOOL bSgsnChange = FALSE;
	if((p->m_pOrgRec->bit_mask 
        & R4_Org_SGSNPDPRecord_sgsnAddress_present)
        && (m_pOrgRec->bit_mask 
        & R4_Org_SGSNPDPRecord_sgsnAddress_present))
    {		
        //��ʱ�ж��Ƿ�SGSN�����л�			
		if(!Match_gsnAddress(&(p->m_pOrgRec->sgsnAddress), 
                             (R4_Org_GSNAddress*)(&(m_pSGSNAddressTail->value))))
        {
            bSgsnChange = TRUE;
        }

		if(bSgsnChange)
		{
			R4_Fin__seqof2_* q = new R4_Fin__seqof2_;
			if(q == NULL)
			{
				throw "Alloc memory fail.";					
			}
			
			memcpy(&(q->value), &(p->m_pOrgRec->sgsnAddress), sizeof(R4_Org_GSNAddress));			
			q->next = NULL;

			if(m_pSGSNAddressTail == NULL)
			{
				m_pSGSNAddress = m_pSGSNAddressTail = q;
			}
			else
			{
				m_pSGSNAddressTail->next = q;
				m_pSGSNAddressTail = q;
			}
            /* 
             * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
             * 
             */
            m_nLen_SGSNAddress_List++;
		}         
    }
	//2003-07-24 end.

    if(!bSgsnChange)
    {
        m_nCurRecordSequence++;
    }
    else
    {
        m_nMinRecordSequence = m_nCurRecordSequence = p->m_pOrgRec->recordSequenceNumber;
    }

    //����listOfTrafficVolumes�ֶΡ�
	if((p->m_pOrgRec->bit_mask
		& R4_Org_SGSNPDPRecord_listOfTrafficVolumes_present)
		&& (m_pOrgRec->bit_mask
		& R4_Org_SGSNPDPRecord_listOfTrafficVolumes_present))
	{
		if(p->m_pOrgRec->listOfTrafficVolumes != NULL)
		{
			R4_Org__seqof1_* q = NULL;
			R4_Org__seqof1_* pTail = NULL;

			unsigned int nCount = 0;
			q = Clone_ChangeOfCharCondition(p->m_pOrgRec->listOfTrafficVolumes, pTail, nCount);
			
			if(m_pConditionTail == NULL)
			{
				m_pCondition = q;
				m_pConditionTail = pTail;
			}
			else
			{
				m_pConditionTail->next = q;
				m_pConditionTail = pTail;
			}

			/* 
			 * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
			 * 
			 */
			m_nLen_Condition_List += nCount;
		}
	}

    /*
    //����recordExtensions�ֶΡ�
    if((p->m_pOrgRec->bit_mask
        & R4_Org_SGSNPDPRecord_recordExtensions_present)
        && (m_pOrgRec->bit_mask
        & R4_Org_SGSNPDPRecord_recordExtensions_present))
    {
        if(p->m_pOrgRec->recordExtensions != NULL)           
        {
			R4_Org_ManagementExtensions_* pTail = NULL;
            R4_Org_ManagementExtensions_* q = NULL;
            unsigned int nCount = 0;
			q = Clone_ManagementExtensions(p->m_pOrgRec->recordExtensions, pTail, nCount);                       
            
            if(m_pExtensionTail == NULL)
            {
                m_pExtension = q;
				m_pExtensionTail = pTail;
            }
            else
            {
                m_pExtensionTail->next = q;
                m_pExtensionTail = pTail;
            }

            /* 
             * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
             * 
             *
            m_nLen_Extension_List += nCount;
        }
    }
    */

    //����recordSequenceNumber�ֶΡ�
    if((p->m_pOrgRec->bit_mask 
        & R4_Org_SGSNPDPRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask 
        & R4_Org_SGSNPDPRecord_recordSequenceNumber_present))
    {
		R4_Fin__seqof1_* q = new R4_Fin__seqof1_;
        if (q == NULL)
		{
			throw "Alloc memory fail.";
		}

        q->value = p->m_pOrgRec->recordSequenceNumber;
		q->next = NULL;

        if(!bSgsnChange)
        {
            m_pRecordSequenceTail->next = q;
            m_pRecordSequenceTail = q;
        }
        else
        {
            R4_Fin_SGSNPDPRecord::R4_Fin__seqof4* pList = 
                                         new R4_Fin_SGSNPDPRecord::R4_Fin__seqof4;
            if(pList == NULL)
			{
				throw "Alloc memory fail.";
			}
            pList->next = NULL;

            pList->value.recSequenceNumberList = q;
            pList->value.bit_mask |= R4_Fin_recSequenceNumberList_present;

			memcpy(&(pList->value.sgsnAddress),
               &(p->m_pOrgRec->sgsnAddress), sizeof(R4_Org_GSNAddress));            
            
            m_pRecordSequenceListTail->next = pList;
            m_pRecordSequenceListTail = pList;

            m_pRecordSequenceTail = q;
        }
    }

    //diagnostics�ֶ�ȡ���һ�Ż�����ֵ��
    if((p->m_pOrgRec->bit_mask 
        //& R4_Org_SGSNPDPRecord_diagnostics_present)
        //&& (m_pOrgRec->bit_mask 
        & R4_Org_SGSNPDPRecord_diagnostics_present))
    {
        R4_Org_ManagementExtension* q = NULL;
        if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_networkSpecificCause_chosen)
        {
            q = Clone_ManagementExtension(&(p->m_pOrgRec->diagnostics.u.networkSpecificCause));
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_manufacturerSpecificCause_chosen)
        {
            q = Clone_ManagementExtension(&(p->m_pOrgRec->diagnostics.u.manufacturerSpecificCause));
        }

        /*����m_Diagnostics�Ŀռ䡣*/
        if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_diagnostics_present) //����һ��if ZhengYuqun 2003-11-03 SWPD02228

        {
            if(m_Diagnostics.choice == R4_Org_manufacturerSpecificCause_chosen)
            {
                delete[] m_Diagnostics.u.manufacturerSpecificCause.identifier.value;
                delete[] m_Diagnostics.u.manufacturerSpecificCause.information.value;
            }
            else if(m_Diagnostics.choice == R4_Org_networkSpecificCause_chosen)
            {
                delete[] m_Diagnostics.u.networkSpecificCause.identifier.value;
                delete[] m_Diagnostics.u.networkSpecificCause.information.value;
            }
        }
        else
        {
            m_pOrgRec->bit_mask |= R4_Org_SGSNPDPRecord_diagnostics_present; //����һ�� ZhengYuqun 2003-11-03 SWPD02228

        }
        /************************/

        if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_networkSpecificCause_chosen)
        {
            m_Diagnostics.choice = R4_Org_networkSpecificCause_chosen;
            
            m_Diagnostics.u.networkSpecificCause = *q;
            delete q;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_manufacturerSpecificCause_chosen)
        {
            m_Diagnostics.choice = R4_Org_manufacturerSpecificCause_chosen;
            
            m_Diagnostics.u.manufacturerSpecificCause = *q;
            delete q;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_gsm0408Cause_chosen)
        {
            m_Diagnostics.choice = R4_Org_gsm0408Cause_chosen;
            m_Diagnostics.u.gsm0408Cause = 
                p->m_pOrgRec->diagnostics.u.gsm0408Cause;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_gsm0902MapErrorValue_chosen)
        {
            m_Diagnostics.choice = R4_Org_gsm0902MapErrorValue_chosen;
            m_Diagnostics.u.gsm0902MapErrorValue = 
                p->m_pOrgRec->diagnostics.u.gsm0902MapErrorValue;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_ccittQ767Cause_chosen)
        {
            m_Diagnostics.choice = R4_Org_ccittQ767Cause_chosen;
            m_Diagnostics.u.ccittQ767Cause = 
                p->m_pOrgRec->diagnostics.u.ccittQ767Cause;
        }
    }

	//����localSequenceNumber�ֶΡ�
    if((p->m_pOrgRec->bit_mask 
        & R4_Org_SGSNPDPRecord_localSequenceNumber_present)
        && (m_pOrgRec->bit_mask 
        & R4_Org_SGSNPDPRecord_localSequenceNumber_present))
    {
		R4_Fin__seqof1_* q = new R4_Fin__seqof1_;
        if (q == NULL)
		{
			throw "Alloc memory fail.";
		}
		
        q->value = p->m_pOrgRec->localSequenceNumber;
		q->next = NULL;
		
        if(!bSgsnChange)
        {
            m_pLocalSequenceTail->next = q;
            m_pLocalSequenceTail = q;
        }
        else
        {
            R4_Fin_SGSNPDPRecord::R4_Fin__seqof5* pList = 
                                         new R4_Fin_SGSNPDPRecord::R4_Fin__seqof5;
            if(pList == NULL)
			{
				throw "Alloc memory fail.";
			}
            pList->next = NULL;

            pList->value.localSeqNumberList = q;
            pList->value.bit_mask |= R4_Fin_localSeqNumberList_present;

			memcpy(&(pList->value.gsnAddress),
               &(p->m_pOrgRec->sgsnAddress), sizeof(R4_Org_GSNAddress));            
            
            m_pLocalSequenceListTail->next = pList;
            m_pLocalSequenceListTail = pList;

            m_pLocalSequenceTail = q;
        }
    }

	//�ϲ�CAMELInformation�����Ϣ
    if((p->m_pOrgRec->bit_mask 
		& R4_Org_cAMELInformationPDP_present)
		&& (m_pOrgRec->bit_mask
		& R4_Org_cAMELInformationPDP_present))
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
			& R4_Org_CAMELInformationPDP_fFDAppendIndicator_present)
			|| ((p->m_pOrgRec->cAMELInformationPDP.bit_mask 
			& R4_Org_CAMELInformationPDP_fFDAppendIndicator_present)
            &&  (p->m_pOrgRec->cAMELInformationPDP.fFDAppendIndicator == FALSE))) 
			&& (p->m_pOrgRec->cAMELInformationPDP.bit_mask
			& R4_Org_CAMELInformationPDP_freeFormatData_present)
			&&(p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length != 0))
		{
			memcpy(m_CAMELInformationPDP.freeFormatData.value,
				p->m_pOrgRec->cAMELInformationPDP.freeFormatData.value,
				p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length);
			
			m_CAMELInformationPDP.freeFormatData.length =
				p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length;

			m_CAMELInformationPDP.bit_mask |= 
				R4_Org_CAMELInformationPDP_freeFormatData_present;
		}
		
		//fFDAppendIndicator �ֶδ�����ֵΪTRUE
		//freeFormatData���ڶ�����Ϣ���Ȳ�Ϊ��
		if((p->m_pOrgRec->cAMELInformationPDP.bit_mask
			& R4_Org_CAMELInformationPDP_fFDAppendIndicator_present)
			&& (p->m_pOrgRec->cAMELInformationPDP.fFDAppendIndicator == TRUE) 
			&&(p->m_pOrgRec->cAMELInformationPDP.bit_mask
			& R4_Org_CAMELInformationPDP_freeFormatData_present)
			&& (p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length != 0))
		{
			memcpy(m_CAMELInformationPDP.freeFormatData.value 
				+ m_CAMELInformationPDP.freeFormatData.length,
				p->m_pOrgRec->cAMELInformationPDP.freeFormatData.value,
				p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length);

			m_CAMELInformationPDP.freeFormatData.length +=
				p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length;

			m_CAMELInformationPDP.bit_mask |= 
				R4_Org_CAMELInformationPDP_freeFormatData_present;
		}
		
	}

	//����rNCDownlinkVolume�ֶΡ�
    if((p->m_pOrgRec->bit_mask 
        & R4_Org_rNCUnsentDownlinkVolume_present))
//        && (m_pOrgRec->bit_mask           //ɾ������ ���ⵥSWPD05164 YanYong 2004-05-12
//        & R4_Org_rNCUnsentDownlinkVolume_present)) //ɾ������ ���ⵥSWPD05164 YanYong 2004-05-12
    {
		R4_Fin__seqof1_* q = new R4_Fin__seqof1_;
        if (q == NULL)
		{
			throw "Alloc memory fail.";
		}
		
        q->value = p->m_pOrgRec->rNCUnsentDownlinkVolume;
		q->next = NULL;
		
        if(m_pRNCDownlinkVolumeList == NULL)
        {
            R4_Fin_SGSNPDPRecord::R4_Fin__seqof6* pList = 
                                         new R4_Fin_SGSNPDPRecord::R4_Fin__seqof6;
            if(pList == NULL)
			{
				throw "Alloc memory fail.";
			}
            pList->next = NULL;

            pList->value.rNCUnsentDnVolumeList = q;
            pList->value.bit_mask |= R4_Fin_rNCUnsentDnVolumeList_present;

			memcpy(&(pList->value.sgsnAddress),
               &(p->m_pOrgRec->sgsnAddress), sizeof(R4_Org_GSNAddress));            
        
            m_pRNCDownlinkVolumeList = pList;
            m_pRNCDownlinkVolumeListTail = pList;
        
            m_pRNCDownlinkVolumeTail = q;            

        }
        else
        {
            if(!bSgsnChange)
            {
                m_pRNCDownlinkVolumeTail->next = q;
                m_pRNCDownlinkVolumeTail = q;
            }
            else
            {
                R4_Fin_SGSNPDPRecord::R4_Fin__seqof6* pList = 
                                             new R4_Fin_SGSNPDPRecord::R4_Fin__seqof6;
                if(pList == NULL)
			    {
				    throw "Alloc memory fail.";
			    }
                pList->next = NULL;

                pList->value.rNCUnsentDnVolumeList = q;
                pList->value.bit_mask |= R4_Fin_rNCUnsentDnVolumeList_present;

			    memcpy(&(pList->value.sgsnAddress),
                   &(p->m_pOrgRec->sgsnAddress), sizeof(R4_Org_GSNAddress));            
            
                m_pRNCDownlinkVolumeListTail->next = pList;
                m_pRNCDownlinkVolumeListTail = pList;
            
                m_pRNCDownlinkVolumeTail = q;            
            }
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


//C3G_R4_GCDR��ʵ��(S-CDR)
C3G_R4_GCDR::C3G_R4_GCDR()
{
    m_pSGSNAddress = NULL;
    m_pCondition = NULL;
    m_pExtension = NULL;
    m_pRecordSequence = NULL;
	m_pRecordSequenceTail = NULL;

    m_bPDPCompleted = FALSE;
    m_bPrepared = FALSE;

    m_nMinRecordSequence = 0;
    m_nCurRecordSequence = 0;

    m_Duration = 0;
    memset(&m_Diagnostics,0,sizeof(R4_Org_Diagnostics));
    m_CauseForRecClosing = R4_Org_normalRelease;

    m_pDestRec = NULL;
    m_pOrgRec = NULL;
    m_bIsLimit = FALSE;
}

C3G_R4_GCDR::~C3G_R4_GCDR()
{
    //����ԭʼ������¼�ռ䡣
    if(m_pOrgRec != NULL)
    {
		if(m_pOrgRec->sgsnAddress != NULL)
		{
			Clear_GsnAddressList((R4_Fin__seqof2_*)(m_pOrgRec->sgsnAddress));
		}

		if (m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_listOfTrafficVolumes_present)
		{
			if (m_pOrgRec->listOfTrafficVolumes != NULL)
			{
				Clear_ChangeOfCharCondition(m_pOrgRec->listOfTrafficVolumes);
			}
		}

        if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_diagnostics_present)
        {
            Clear_Diagnostics(&(m_pOrgRec->diagnostics));
        }

        if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_recordExtensions_present)
        {
            if(m_pOrgRec->recordExtensions != NULL)                
            {
                Clear_ManagementExtensions(m_pOrgRec->recordExtensions);
            }
        }

        delete m_pOrgRec;
        m_pOrgRec = NULL;
    }

    //�������ջ�����¼�ռ䡣
    if(m_pDestRec != NULL)
    {
		if(m_pDestRec->u.ggsnPDPRecord.sgsnAddress != NULL)
		{
			Clear_GsnAddressList(m_pDestRec->u.ggsnPDPRecord.sgsnAddress);
		}

		if(m_pDestRec->u.ggsnPDPRecord.bit_mask 
			& R4_Fin_GGSNPDPRecord_listOfTrafficVolumes_present)
		{
			if(m_pDestRec->u.ggsnPDPRecord.listOfTrafficVolumes != NULL)
			{
				Clear_ChangeOfCharCondition((R4_Org__seqof1_*)
					m_pDestRec->u.ggsnPDPRecord.listOfTrafficVolumes);
			}
		}

        if(m_pDestRec->u.ggsnPDPRecord.bit_mask
            & R4_Fin_GGSNPDPRecord_diagnostics_present)
        {
            Clear_Diagnostics((R4_Org_Diagnostics*)
                &(m_pDestRec->u.ggsnPDPRecord.diagnostics));
        }

        if(m_pDestRec->u.ggsnPDPRecord.bit_mask
            & R4_Fin_GGSNPDPRecord_recordExtensions_present)
        {
            if(m_pDestRec->u.ggsnPDPRecord.recordExtensions != NULL)                
            {
                Clear_ManagementExtensions((R4_Org_ManagementExtensions_*)
                    m_pDestRec->u.ggsnPDPRecord.recordExtensions);
            }
        }

        if(m_pDestRec->u.ggsnPDPRecord.bit_mask
            & R4_Fin_GGSNPDPRecord_recSequenceNumList_present)
        {
			if(m_pDestRec->u.ggsnPDPRecord.recSequenceNumList.recSequenceNumberList != NULL)
			{
				Clear_SequenceNumList(
					m_pDestRec->u.ggsnPDPRecord.recSequenceNumList.recSequenceNumberList);
			}
        }

		if(m_pDestRec->u.ggsnPDPRecord.bit_mask
            & R4_Fin_GGSNPDPRecord_localSequenceNumberList_present)
        {
			if(m_pDestRec->u.ggsnPDPRecord.localSequenceNumberList.localSeqNumberList != NULL)
			{
				Clear_SequenceNumList(
					m_pDestRec->u.ggsnPDPRecord.localSequenceNumberList.localSeqNumberList);
			}
        }

        delete m_pDestRec;
        m_pDestRec = NULL;
    }

    /*
     * �ϲ�������ʹ�õ����ݽṹ�Ŀռ��Ѿ���GetContents�л��ա�
     */
}

BOOL C3G_R4_GCDR::PrepareData()
{
    //ת��sgsnAddress
    if(m_pOrgRec->sgsnAddress != NULL)
    {
		R4_Fin__seqof2_* pTail = NULL;
		unsigned int nCount = 0;
        m_pSGSNAddress = Clone_GsnAddressList((R4_Fin__seqof2_*)(m_pOrgRec->sgsnAddress), (R4_Fin__seqof2_*)pTail, nCount);

        m_pSGSNAddressTail = pTail;
    }

    //����listOfTrafficVolumes
	if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_listOfTrafficVolumes_present)
	{
		if(m_pOrgRec->listOfTrafficVolumes != NULL)
		{
			R4_Org__seqof1_* pTail = NULL;
			unsigned int nCount = 0;
			m_pCondition = 
				Clone_ChangeOfCharCondition(m_pOrgRec->listOfTrafficVolumes, pTail, nCount);
        
			m_pConditionTail = pTail;
		}
	}

    //����recordExtensions
    if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_recordExtensions_present)
    {
        if(m_pOrgRec->recordExtensions != NULL)
        {     
			R4_Org_ManagementExtensions_* pTail = NULL; 
			unsigned int nCount = 0;
            m_pExtension = Clone_ManagementExtensions(m_pOrgRec->recordExtensions, pTail, nCount);
           
            m_pExtensionTail = pTail;
        }
    }

    //����recordSequenceNumber
    if(m_pOrgRec->bit_mask 
        & R4_Org_GGSNPDPRecord_recordSequenceNumber_present)
    {
        m_pRecordSequence = new R4_Fin__seqof1_;
		if (m_pRecordSequence == NULL)
		{
			throw "Alloc memory fail.";
		}

        m_pRecordSequence->value = m_pOrgRec->recordSequenceNumber;
		m_pRecordSequence->next = NULL;

        m_pRecordSequenceTail = m_pRecordSequence;
    }

	//����localSequenceNumber
    if(m_pOrgRec->bit_mask 
        & R4_Org_GGSNPDPRecord_localSequenceNumber_present)
    {
        m_pLocalSequence = new R4_Fin__seqof1_;
		if (m_pLocalSequence == NULL)
		{
			throw "Alloc memory fail.";
		}
		
        m_pLocalSequence->value = m_pOrgRec->localSequenceNumber;
		m_pLocalSequence->next = NULL;
		
        m_pLocalSequenceTail = m_pLocalSequence;
    }

    //����duration�ֶ�
    m_Duration = m_pOrgRec->duration;

    //����causeForRecClosing�ֶ�
    m_CauseForRecClosing = m_pOrgRec->causeForRecClosing;

    //����diagnostics�ֶ�
    if(m_pOrgRec->bit_mask
        & R4_Fin_GGSNPDPRecord_diagnostics_present)
    {
        m_Diagnostics = m_pOrgRec->diagnostics;
        
        if(m_Diagnostics.choice == R4_Fin_networkSpecificCause_chosen)
        {
            R4_Org_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(
                &(m_pOrgRec->diagnostics.u.networkSpecificCause));

            m_Diagnostics.choice = R4_Org_networkSpecificCause_chosen;
            m_Diagnostics.u.networkSpecificCause = *q;
            
            delete q;
        }
        else if(m_Diagnostics.choice 
            == R4_Org_manufacturerSpecificCause_chosen)
        {
            R4_Org_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(
                &(m_pOrgRec->diagnostics.u.manufacturerSpecificCause));

            m_Diagnostics.choice = R4_Org_manufacturerSpecificCause_chosen;
            m_Diagnostics.u.manufacturerSpecificCause = *q;
            
            delete q;
        }
    }

    //׼�����
    m_bPrepared = TRUE;
    return TRUE;
}


/******************************************************
* ������:   C3G_R4_GCDR::Init
* ����:    Guofeng, Guan
* ����:    2003-06-02
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
*2003-06-02       Guofeng, Guan
******************************************************/
BOOL C3G_R4_GCDR::Init(char* pBuf,UINT4 uLen)
{
    assert(pBuf != NULL);

    //������¼�������ֶζ���������������ڴ����
    R4_Org_GGSNPDPRecord* p = (R4_Org_GGSNPDPRecord*)pBuf;

    //����ԭʼ����
    m_pOrgRec = new R4_Org_GGSNPDPRecord;
    if(m_pOrgRec == NULL)
    {
        throw "Alloc memory fail.";
    }

    memcpy(m_pOrgRec,p,sizeof(R4_Org_GGSNPDPRecord));

    /*
     * ���浥������ָ���ֶ�
     */

    //����sgsnAddress
    if(p->sgsnAddress != NULL)
    {
		R4_Fin__seqof2_* pTail = NULL;
		unsigned int nCount = 0;
        m_pOrgRec->sgsnAddress = (R4_Org_GGSNPDPRecord::R4_Org__seqof2 *)Clone_GsnAddressList((R4_Fin__seqof2_*)(p->sgsnAddress), (R4_Fin__seqof2_*)pTail, nCount);
        

        /* 
         * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
         * 
         */
        m_nLen_SGSNAddress_List = nCount;
    }

    //����listOfTrafficVolumes�ֶ�
	if(p->bit_mask & R4_Org_GGSNPDPRecord_listOfTrafficVolumes_present)
	{
		if(p->listOfTrafficVolumes != NULL)
		{
			R4_Org__seqof1_* pTail = NULL;
			unsigned int nCount = 0;
			m_pOrgRec->listOfTrafficVolumes
				= Clone_ChangeOfCharCondition(p->listOfTrafficVolumes, pTail, nCount);            
			
			/* 
			 * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
			 * 
			 */
			m_nLen_Condition_List = nCount;
		}
    }

    //����diagnostics�ֶ�
    if(p->bit_mask & R4_Org_GGSNPDPRecord_diagnostics_present)
    {
        m_pOrgRec->diagnostics = p->diagnostics;
        
        if(p->diagnostics.choice == R4_Org_networkSpecificCause_chosen)
        {
            R4_Org_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(
                        &(p->diagnostics.u.networkSpecificCause));

            m_pOrgRec->diagnostics.choice
                = R4_Org_networkSpecificCause_chosen;
            m_pOrgRec->diagnostics.u.networkSpecificCause = *q;
            
            delete q;
        }
        else if(p->diagnostics.choice 
            == R4_Org_manufacturerSpecificCause_chosen)
        {
            R4_Org_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(
                    &(p->diagnostics.u.manufacturerSpecificCause));

            m_pOrgRec->diagnostics.choice
                = R4_Org_manufacturerSpecificCause_chosen;
            m_pOrgRec->diagnostics.u.manufacturerSpecificCause = *q;

            delete q;
        }
    }

    //����recordExtensions�ֶ�
    if(p->bit_mask & R4_Org_GGSNPDPRecord_recordExtensions_present)
    {
        if(p->recordExtensions != NULL)
        {
			R4_Org_ManagementExtensions_* pTail = NULL;
			unsigned int nCount = 0;
            m_pOrgRec->recordExtensions
                = Clone_ManagementExtensions(p->recordExtensions, pTail, nCount);            

            /* 
             * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
             * 
             */
            m_nLen_Extension_List = nCount;
        }
    }

    //���õ�ǰ�������к�
    m_nMinRecordSequence = m_nCurRecordSequence = p->recordSequenceNumber;

    //����PDP������־
    switch(p->causeForRecClosing)
    {
    case R4_Org_normalRelease:
    case R4_Org_abnormalRelease:
    case R4_Org_sGSNChange:
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

BOOL C3G_R4_GCDR::IsFirstPartialRec()
{
    return (m_pOrgRec->recordSequenceNumber == 0);
}

BOOL C3G_R4_GCDR::IsLastPartialRec()
{
    switch(m_pOrgRec->causeForRecClosing)
    {
    case R4_Org_normalRelease:
    case R4_Org_abnormalRelease:
    case R4_Org_sGSNChange:
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

BOOL C3G_R4_GCDR::IsPartial()
{
    /* 
     * ��recordSequenceNumber�ֶ���Ч���򻰵��ǲ��ֻ�����
     * 
     */
    return (m_pOrgRec->bit_mask &
        R4_Org_GGSNPDPRecord_recordSequenceNumber_present);
}

BOOL C3G_R4_GCDR::IsHotBilling()
{
    if((m_pOrgRec->chargingCharacteristics.value[0] & 0x0f) == 1)
    {
        return TRUE;
    }
    return FALSE;
}

/******************************************************
* ������:   C3G_R4_GCDR::CanSafeMerge
* ����:     Guofeng, Guan
* ����:     2003-06-02
* ����:    
* ����: 
*       ����1 :CBill* pBill,���ϲ��Ļ�������ָ�롣
* ����ֵ: BOOL ,�����Ż�������ϲ�������TRUE�����򷵻�FALSE��
* ˵��: 
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*2003-06-02       Guofeng, Guan
******************************************************/
BOOL C3G_R4_GCDR::CanSafeMerge(CBill* pBill)
{
    assert(pBill != NULL);

    C3G_R4_GCDR* p = (C3G_R4_GCDR*)pBill;

    //�����������к����Ѿ��ϲ��Ļ�����Χ�ڣ�˵�����ظ�������
    //�ظ�������Merge�����ж�����
    if((p->m_pOrgRec->bit_mask
        & R4_Org_GGSNPDPRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask
        & R4_Org_GGSNPDPRecord_recordSequenceNumber_present))
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
    	m_bIsLimit = TRUE;
        return FALSE;
    }

    if((m_nLen_Condition_List + p->m_nLen_Condition_List)
        > MAX_MERGED_BILLS)
    {
    	m_bIsLimit = TRUE;
        return FALSE;
    }

    if((m_nLen_SGSNAddress_List + p->m_nLen_SGSNAddress_List)
        > MAX_MERGED_BILLS)
    {
    	m_bIsLimit = TRUE;
        return FALSE;
    }

    if((m_nLen_Extension_List + p->m_nLen_Extension_List)
        > MAX_MERGED_BILLS)
    {
    	m_bIsLimit = TRUE;
        return FALSE;
    }

    //�����кŲ�������������ϲ���
    return (p->m_nCurRecordSequence == (m_nCurRecordSequence + 1));
}

BOOL C3G_R4_GCDR::IsPDPCompleted()
{
    return m_bPDPCompleted;
}

BOOL C3G_R4_GCDR::NeedWaiting()
{
    //��ǰ�ĺϲ�������ü�����ʽ�ϲ�Duration�ֶ�
    //���Ըú���ֻ����FALSE(��PDP����ʱ��������ȴ�)��
    return FALSE;    
}

char* C3G_R4_GCDR::GetContents()
{
    if(m_pDestRec != NULL)
    {
        return (char*)m_pDestRec;
    }

    R4_Fin_CallEventRecord* pCallEventRec = new R4_Fin_CallEventRecord;
    if(pCallEventRec == NULL)
    {
        throw "Alloc memory fail.";
    }

    pCallEventRec->choice = R4_Fin_ggsnPDPRecord_chosen;
    R4_Fin_GGSNPDPRecord* pDest = &(pCallEventRec->u.ggsnPDPRecord);

    memset(pDest,0,sizeof(R4_Fin_GGSNPDPRecord));
    m_pDestRec = pCallEventRec;

    //�������ڴ�鿪ʼ/������ַ��
    char* pBegin = NULL,*pEnd = NULL;

    /*
     * ������û�о����ϲ������ԭʼ�����й������ջ�����
     */

    //�ϲ�ǰbit_mask��4���ֽڳ����ϲ�����2���ֽڳ�������ֱ�Ӹ�ֵ��
    if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_networkInitiation_present)
    {
        pDest->bit_mask |= R4_Fin_GGSNPDPRecord_networkInitiation_present;
    }
	if (m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_accessPointNameNI_present)
	{
		pDest->bit_mask |= R4_Fin_GGSNPDPRecord_accessPointNameNI_present;
	}
	if (m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_pdpType_present)
	{
		pDest->bit_mask |= R4_Fin_GGSNPDPRecord_pdpType_present;
	}
	if (m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_servedPDPAddress_present)
	{
		pDest->bit_mask |= R4_Fin_GGSNPDPRecord_servedPDPAddress_present;
	}
    if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_dynamicAddressFlag_present)
    {
        pDest->bit_mask |= R4_Fin_GGSNPDPRecord_dynamicAddressFlag_present;
    }
	if (m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_listOfTrafficVolumes_present)
	{
		pDest->bit_mask |= R4_Fin_GGSNPDPRecord_listOfTrafficVolumes_present;
	}
    if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_diagnostics_present)
    {
        pDest->bit_mask |= R4_Fin_GGSNPDPRecord_diagnostics_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_recordSequenceNumber_present)
    {
        pDest->bit_mask |= R4_Fin_GGSNPDPRecord_recSequenceNumList_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_nodeID_present)
    {
        pDest->bit_mask |= R4_Fin_GGSNPDPRecord_nodeID_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_recordExtensions_present)
    {
        pDest->bit_mask |= R4_Fin_GGSNPDPRecord_recordExtensions_present;
    }
	if (m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_localSequenceNumber_present)
	{
		pDest->bit_mask |= R4_Fin_GGSNPDPRecord_localSequenceNumberList_present;
	}
    if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_apnSelectionMode_present)
    {
        pDest->bit_mask |= R4_Fin_GGSNPDPRecord_apnSelectionMode_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_servedMSISDN_present)
    {
        pDest->bit_mask |= R4_Fin_GGSNPDPRecord_servedMSISDN_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_chChSelectionMode_present)
    {
        pDest->bit_mask |= R4_Fin_GGSNPDPRecord_chChSelectionMode_present;
    }
	//added by ggf, 2003-11-20, sgsnPLMNIdentifier
	if(m_pOrgRec->bit_mask & R4_Org_sgsnPLMNIdentifier_present)
	{
		pDest->bit_mask |= R4_Fin_sgsnPLMNIdentifier_present;
	}
	//end.

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
        if(m_pOrgRec->sgsnAddress != NULL)
        {
			R4_Fin__seqof2_* pTail = NULL;
			unsigned int nCount = 0;
            pDest->sgsnAddress = Clone_GsnAddressList((R4_Fin__seqof2_*)(m_pOrgRec->sgsnAddress), (R4_Fin__seqof2_*)pTail, nCount);
        }

        pBegin = (char*)(&(m_pOrgRec->accessPointNameNI));
        pEnd = (char*)(&(m_pOrgRec->listOfTrafficVolumes));
        /*
         * accessPointNameNI,pdpType,servedPDPAddress,dynamicAddressFlag
         */
        memcpy(&(pDest->accessPointNameNI),pBegin,(pEnd - pBegin));

        //����listOfTrafficVolumes�ֶ�
		if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_listOfTrafficVolumes_present)
		{
			if(m_pOrgRec->listOfTrafficVolumes != NULL)           
			{
				R4_Org__seqof1_* pTail = NULL;
				unsigned int nCount = 0;
				pDest->listOfTrafficVolumes = (R4_Fin__seqof3_*)
					Clone_ChangeOfCharCondition(m_pOrgRec->listOfTrafficVolumes, pTail, nCount);            
			}
        }

        pBegin = (char*)(&(m_pOrgRec->recordOpeningTime));
        pEnd = (char*)(&(m_pOrgRec->diagnostics));
        /*
         * recordOpeningTime,duration,causeForRecClosing
         */
        memcpy(&(pDest->recordOpeningTime),pBegin,(pEnd - pBegin));
        
        //����diagnostics�ֶ�
        if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_diagnostics_present)
        {
            memcpy(&(pDest->diagnostics),&(m_pOrgRec->diagnostics),
                sizeof(R4_Org_Diagnostics));

            if(m_pOrgRec->diagnostics.choice
                == R4_Org_networkSpecificCause_chosen)
            {
                R4_Org_ManagementExtension* q = NULL;
                q = Clone_ManagementExtension(
                    &(m_pOrgRec->diagnostics.u.networkSpecificCause));

                pDest->diagnostics.choice = R4_Fin_networkSpecificCause_chosen;
                pDest->diagnostics.u.networkSpecificCause
                    = *(R4_Fin_ManagementExtension*)q;

                delete q;
            }
            else if(m_pOrgRec->diagnostics.choice 
                == R4_Org_manufacturerSpecificCause_chosen)
            {
                R4_Org_ManagementExtension* q = NULL;
                q = Clone_ManagementExtension(
                    &(m_pOrgRec->diagnostics.u.manufacturerSpecificCause));

                pDest->diagnostics.choice = R4_Fin_manufacturerSpecificCause_chosen;
                pDest->diagnostics.u.manufacturerSpecificCause
                    = *(R4_Fin_ManagementExtension*)q;
                
                delete q;
            }
        }

        /*
         * recordSequenceNumberת��Ϊ����
         */
        if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_recordSequenceNumber_present)
        {
			pDest->recSequenceNumList.recSequenceNumberList = new R4_Fin__seqof1_;
            if(pDest->recSequenceNumList.recSequenceNumberList == NULL)
            {
                throw "Alloc memory fail.";
            }

            pDest->recSequenceNumList.recSequenceNumberList->value
                = m_pOrgRec->recordSequenceNumber;
            pDest->recSequenceNumList.recSequenceNumberList->next = NULL;
            
            pDest->recSequenceNumList.bit_mask |= R4_Fin_recSequenceNumberList_present;
            
            
            if(m_pOrgRec->sgsnAddress != NULL)
            {
                memcpy(&(pDest->recSequenceNumList.sgsnAddress), 
                    &(m_pOrgRec->sgsnAddress->value),sizeof(R4_Org_GSNAddress));            
            }
            else
            {
                memcpy(&(pDest->recSequenceNumList.sgsnAddress), 
                    0, sizeof(R4_Org_GSNAddress));            
            }
        }

        //����nodeID�ֶ�(һ������)
        if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_nodeID_present)
        {
            memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(R4_Org_NodeID));
        }

        //����recordExtensions�ֶ�
        if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_recordExtensions_present)
        {
            if(m_pOrgRec->recordExtensions != NULL)                
            {
				R4_Org_ManagementExtensions_* pTail = NULL;
				unsigned int nCount = 0;
                pDest->recordExtensions = (R4_Fin_ManagementExtensions_*)
                    Clone_ManagementExtensions(m_pOrgRec->recordExtensions, pTail, nCount);                              
            }
        }   
		
		/*
         * localSequenceNumberת��Ϊ����
         */
        if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_localSequenceNumber_present)
        {
			pDest->localSequenceNumberList.localSeqNumberList = new R4_Fin__seqof1_;
            if(pDest->localSequenceNumberList.localSeqNumberList == NULL)
            {
                throw "Alloc memory fail.";
            }

            pDest->localSequenceNumberList.localSeqNumberList->value
                = m_pOrgRec->localSequenceNumber;
            pDest->localSequenceNumberList.localSeqNumberList->next = NULL;
            
            pDest->localSequenceNumberList.bit_mask |= R4_Fin_localSeqNumberList_present;
            
            memcpy(&(pDest->localSequenceNumberList.gsnAddress),
                &(m_pOrgRec->ggsnAddress),sizeof(R4_Org_GSNAddress));            
        }

        pBegin = (char*)(&(m_pOrgRec->apnSelectionMode));
        pEnd = (char*)(&(m_pOrgRec->sgsnPLMNIdentifier));
        /* 
         * apnSelectionMode,servedMSISDN,chargingCharacteristics,
		 * chChSelectionMode, sgsnPLMNIdentifier
         */
        memcpy(&(pDest->apnSelectionMode),pBegin,
            (pEnd - pBegin) + sizeof(R4_Org_PLMN_Id));

        //consolidationResult�ֶ�
        switch(m_pOrgRec->causeForRecClosing)
        {
        case R4_Org_normalRelease:
            //PDP��������
            pDest->consolidationResult
                = R4_Fin_normal;
            break;
        case R4_Org_sGSNChange:
            pDest->consolidationResult
                = R4_Fin_forInterSGSNConsolidation; 
            break;
        case R4_Org_abnormalRelease:
            //PDP�쳣����
            pDest->consolidationResult
                = R4_Fin_notNormal;
            break;
        default:
            //PDPδ����(��ΪPDP�쳣����)
			if (m_bIsLimit)
			{
        		pDest->consolidationResult = R4_Fin_reachLimit;
			}
			else
			{
        		pDest->consolidationResult = R4_Fin_notNormal;
			}
			break;
		}
    
		if (!IsPartial())
		{
    		pDest->consolidationResult = R4_Fin_onlyOneCDRGenerated;	
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
     * ggsnAddress,chargingID
     */
    memcpy(&(pDest->recordType),pBegin,(pEnd - pBegin));
    
    //�ϲ�sgsnAddress�ֶ�
    if(m_pSGSNAddress != NULL)
    {
        pDest->sgsnAddress = m_pSGSNAddress;

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
        pDest->listOfTrafficVolumes = (R4_Fin__seqof3_*) m_pCondition;
                
        m_pCondition = m_pConditionTail = NULL;
    }

    pBegin = (char*)(&(m_pOrgRec->recordOpeningTime));
    pEnd = (char*)(&(m_pOrgRec->diagnostics));
    /* 
     * recordOpeningTime,duration,causeForRecClosing
     */
    memcpy(&(pDest->recordOpeningTime),pBegin,(pEnd - pBegin));

    //�ϲ�diagnostics�ֶ�
    if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_diagnostics_present)
    {
        memcpy(&(pDest->diagnostics),&(m_Diagnostics),
            sizeof(R4_Org_Diagnostics));
        /*
         * m_Diagnosticsά����ָ��ת����pDest��
         */
    }

    //�ϲ�recordSequenceNumber�ֶ�
    if(m_pOrgRec->bit_mask
        & R4_Org_GGSNPDPRecord_recordSequenceNumber_present)
    {
        if(m_pOrgRec->sgsnAddress != NULL)
        {
            memcpy(&(pDest->recSequenceNumList.sgsnAddress), 
                &(m_pOrgRec->sgsnAddress->value),sizeof(R4_Org_GSNAddress));            
        }
        else
        {
            memcpy(&(pDest->recSequenceNumList.sgsnAddress), 
                0, sizeof(R4_Org_GSNAddress));            
        }
     

        /*
         * m_pSequence�϶���Ϊ�ա�
         */
        pDest->recSequenceNumList.recSequenceNumberList = m_pRecordSequence;

        pDest->recSequenceNumList.bit_mask
            |= R4_Fin_recSequenceNumberList_present;
        
        m_pRecordSequence = m_pRecordSequenceTail = NULL;
    }

    //����nodeID�ֶ�(һ������)
    if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_nodeID_present)
    {
        memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(R4_Org_NodeID));
    }

    //�ϲ�recordExtensions�ֶ�
    if(m_pOrgRec->bit_mask
        & R4_Org_GGSNPDPRecord_recordExtensions_present)
    {
        if(m_pExtension != NULL)
        {
            pDest->recordExtensions = (R4_Fin_ManagementExtensions_*) m_pExtension;
                        
            m_pExtension = m_pExtensionTail = NULL;
        }
    }

	//�ϲ�localSequenceNumber�ֶ�
	if(m_pOrgRec->bit_mask
        & R4_Org_GGSNPDPRecord_localSequenceNumber_present)
    {
        memcpy(&(pDest->localSequenceNumberList.gsnAddress),
            &(m_pOrgRec->ggsnAddress), sizeof(R4_Org_GSNAddress));        

        /*
         * m_pSequence�϶���Ϊ�ա�
         */
        pDest->localSequenceNumberList.localSeqNumberList = m_pLocalSequence;

        pDest->localSequenceNumberList.bit_mask
            |= R4_Fin_localSeqNumberList_present;
        
        m_pLocalSequence = m_pLocalSequenceTail = NULL;
    }

    pBegin = (char*)(&(m_pOrgRec->apnSelectionMode));
    pEnd = (char*)(&(m_pOrgRec->sgsnPLMNIdentifier));

    /* 
     * apnSelectionMode,servedMSISDN,chargingCharacteristics, 
	 * sgsnPLMNIdentifier
     */
    memcpy(&(pDest->apnSelectionMode),pBegin,
            (pEnd - pBegin) + sizeof(R4_Fin_PLMN_Id));

    //consolidationResult�ֶ�
    switch(m_CauseForRecClosing)
    {
    case R4_Org_normalRelease:
    case R4_Org_sGSNChange:
        //PDP��������
        pDest->consolidationResult
            = R4_Fin_normal;
        break;
    case R4_Org_abnormalRelease:
        //PDP�쳣����
        pDest->consolidationResult
            = R4_Fin_notNormal;
        break;
    default:
        //PDPδ����(��ΪPDP�쳣����)
        if (m_bIsLimit)
        {
        	pDest->consolidationResult = R4_Fin_reachLimit;
        }
        else
        {
        	pDest->consolidationResult = R4_Fin_notNormal;
        }
        break;
    }
    
    if (!IsPartial())
    {
    	pDest->consolidationResult = R4_Fin_onlyOneCDRGenerated;	
    }

    pDest->duration = m_Duration;
    pDest->causeForRecClosing = m_CauseForRecClosing;

    return (char*)m_pDestRec;
}

UINT4 C3G_R4_GCDR::GetBillType()
{
    return (m_pOrgRec->recordType);
}

CKey C3G_R4_GCDR::CreateKey()
{
    C3G_R4_GCDR_Key* pkey = new C3G_R4_GCDR_Key;
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

BOOL C3G_R4_GCDR::IsDuplicated(CBill* pBill)
{
    BOOL bSucc = FALSE;
    C3G_R4_GCDR* p = (C3G_R4_GCDR*)pBill;
    if((p->m_pOrgRec->bit_mask
        & R4_Org_GGSNPDPRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask
        & R4_Org_GGSNPDPRecord_recordSequenceNumber_present))
    {
        if((p->m_nCurRecordSequence <= m_nCurRecordSequence)
            && (p->m_nCurRecordSequence >= m_nMinRecordSequence))
        {
            bSucc = TRUE;
        }
    }    
    return bSucc;
}

/******************************************************
* ������:   C3G_R4_GCDR::Merge
* ����:    Guofeng, Guan
* ����:    2003-06-02
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
*2003-06-02       Guofeng, Guan
******************************************************/
BOOL C3G_R4_GCDR::Merge(CBill* pBill)
{
    assert(pBill != NULL);

    if(!m_bPrepared)
    {
        if(!PrepareData())  return FALSE;
    }

    C3G_R4_GCDR* p = (C3G_R4_GCDR*)pBill;

    //�����ظ�������
    if((p->m_pOrgRec->bit_mask
        & R4_Org_GGSNPDPRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask
        & R4_Org_GGSNPDPRecord_recordSequenceNumber_present))
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
    case R4_Org_normalRelease:
    case R4_Org_abnormalRelease:
    case R4_Org_sGSNChange:
        m_bPDPCompleted = TRUE;
        break;
    default:
        break;
    }

    //����causeForRecClosing�ֶΡ�
    m_CauseForRecClosing = p->m_pOrgRec->causeForRecClosing;

	//Z30971�����޸��Ƿ�֧����ʱ��
	if(!m_bDSTFlg)
	{
		//duration���ռ�����ʽ����
        int nDiff = Get_Duration(&(m_pOrgRec->recordOpeningTime),
                                 &(p->m_pOrgRec->recordOpeningTime));
        m_Duration = nDiff;
	}
    m_Duration += p->m_pOrgRec->duration;
	//�޸Ľ���

    //Added by ZhengYuqun 2003-11-04 SWPD02238
    if((UINT4)m_Duration >= m_uMaxDuration)
    {
        m_bPDPCompleted = TRUE;
    }

    //����sgsnAddress�ֶΡ�
    
    //�޳��ظ���SGSN��ַ.

    if(p->m_pOrgRec->sgsnAddress != NULL)        
    {
        unsigned int nCount = 0;

        if(m_pSGSNAddressTail == NULL)
        {
            //������������ж��ظ�SGSN��ַ
			R4_Fin__seqof2_* pTail = NULL;
			R4_Fin__seqof2_* q = NULL;
            q = Clone_GsnAddressList((R4_Fin__seqof2_*)p->m_pOrgRec->sgsnAddress, (R4_Fin__seqof2_*)pTail, nCount);

            m_pSGSNAddress = q;
			m_pSGSNAddressTail = pTail;
        }
        else
        {
            //��ʱ�ж��Ƿ����ظ�SGSN��ַ
			R4_Fin__seqof2_* pTail = NULL;
            R4_Fin__seqof2_* q = NULL;

            q = Clone_GsnAddressList(&(m_pSGSNAddressTail->value),
                (R4_Fin__seqof2_*)(p->m_pOrgRec->sgsnAddress), (R4_Fin__seqof2_*)pTail, nCount);

            if(q != NULL)
            {
                m_pSGSNAddressTail->next = q;
                m_pSGSNAddressTail = pTail;
            }
        }

        /* 
         * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
         * 
         */
        m_nLen_SGSNAddress_List += nCount;
    }

    //����listOfTrafficVolumes�ֶΡ�
	if((p->m_pOrgRec->bit_mask 
		& R4_Org_GGSNPDPRecord_listOfTrafficVolumes_present)
		&& (m_pOrgRec->bit_mask
		& R4_Org_GGSNPDPRecord_listOfTrafficVolumes_present))
	{
		if(p->m_pOrgRec->listOfTrafficVolumes != NULL)      
		{
			R4_Org__seqof1_* pTail = NULL;
			R4_Org__seqof1_* q = NULL;
			unsigned int nCount = 0;
			q = Clone_ChangeOfCharCondition(p->m_pOrgRec->listOfTrafficVolumes, pTail, nCount);

			if(m_pConditionTail == NULL)
			{
				m_pCondition = q;
				m_pConditionTail = pTail;
			}
			else
			{
				m_pConditionTail->next = q;
				m_pConditionTail = pTail;
			}

			/* 
			 * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
			 * 
			 */
			m_nLen_Condition_List += nCount;              
		}
    }

    //����recordExtensions�ֶΡ�
    if((p->m_pOrgRec->bit_mask
        & R4_Org_GGSNPDPRecord_recordExtensions_present)
        && (m_pOrgRec->bit_mask
        & R4_Org_GGSNPDPRecord_recordExtensions_present))
    {
		if(p->m_pOrgRec->recordExtensions != NULL)           
        {
			R4_Org_ManagementExtensions_* pTail = NULL;
            R4_Org_ManagementExtensions_* q = NULL;
            unsigned int nCount = 0;
			q = Clone_ManagementExtensions(p->m_pOrgRec->recordExtensions, pTail, nCount);                       
            
            if(m_pExtensionTail == NULL)
            {
                m_pExtension = q;
				m_pExtensionTail = pTail;
            }
            else
            {
                m_pExtensionTail->next = q;
                m_pExtensionTail = pTail;
            }

            /* 
             * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
             * 
             */
            m_nLen_Extension_List += nCount;
        }
    }

    //����recordSequenceNumber�ֶΡ�
    if((p->m_pOrgRec->bit_mask
        & R4_Org_GGSNPDPRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask
        & R4_Org_GGSNPDPRecord_recordSequenceNumber_present))
    {
		R4_Fin__seqof1_* q = new R4_Fin__seqof1_;
        if (q == NULL)
		{
			throw "Alloc memory fail.";
		}

        q->value = p->m_pOrgRec->recordSequenceNumber;
		q->next = NULL;

        if(m_pRecordSequenceTail == NULL)
        {
            m_pRecordSequenceTail = m_pRecordSequence = q;
        }
        else
        {
            m_pRecordSequenceTail->next = q;
            m_pRecordSequenceTail = q;
        }
    }

	//����localSequenceNumber�ֶΡ�
    if((p->m_pOrgRec->bit_mask
        & R4_Org_GGSNPDPRecord_localSequenceNumber_present)
        && (m_pOrgRec->bit_mask
        & R4_Org_GGSNPDPRecord_localSequenceNumber_present))
    {
		R4_Fin__seqof1_* q = new R4_Fin__seqof1_;
        if (q == NULL)
		{
			throw "Alloc memory fail.";
		}
		
        q->value = p->m_pOrgRec->localSequenceNumber;
		q->next = NULL;
		
        if(m_pLocalSequenceTail == NULL)
        {
            m_pLocalSequenceTail = m_pLocalSequence = q;
        }
        else
        {
            m_pLocalSequenceTail->next = q;
            m_pLocalSequenceTail = q;
        }
    }

    //diagnostics�ֶ�ȡ���һ�Ż�����ֵ��
    if((p->m_pOrgRec->bit_mask
        //& R4_Org_GGSNPDPRecord_diagnostics_present)
        //&& (m_pOrgRec->bit_mask
        & R4_Org_GGSNPDPRecord_diagnostics_present))
    {
        R4_Org_ManagementExtension* q = NULL;
        if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_networkSpecificCause_chosen)
        {
            q = Clone_ManagementExtension(
                &(p->m_pOrgRec->diagnostics.u.networkSpecificCause));
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_manufacturerSpecificCause_chosen)
        {
            q = Clone_ManagementExtension(
                &(p->m_pOrgRec->diagnostics.u.manufacturerSpecificCause));
        }

        /*����ԭdiagnostics�ֶεĿռ䡣*/
        if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_diagnostics_present) //����һ��if ZhengYuqun 2003-11-03 SWPD02228
        {
            if(m_Diagnostics.choice == R4_Org_manufacturerSpecificCause_chosen)
            {
                delete[] m_Diagnostics.u.manufacturerSpecificCause.identifier.value;
                delete[] m_Diagnostics.u.manufacturerSpecificCause.information.value;
            }
            else if(m_Diagnostics.choice == R4_Org_networkSpecificCause_chosen)
            {
                delete[] m_Diagnostics.u.networkSpecificCause.identifier.value;
                delete[] m_Diagnostics.u.networkSpecificCause.information.value;
            }
        }
        else
        {
            m_pOrgRec->bit_mask |= R4_Org_GGSNPDPRecord_diagnostics_present; //����һ�� ZhengYuqun 2003-11-03 SWPD02228
        }

        /************************/
        if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_networkSpecificCause_chosen)
        {
            m_Diagnostics.choice = R4_Org_networkSpecificCause_chosen;
            
            m_Diagnostics.u.networkSpecificCause = *q;
            delete q;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_manufacturerSpecificCause_chosen)
        {
            m_Diagnostics.choice = R4_Org_manufacturerSpecificCause_chosen;
            
            m_Diagnostics.u.manufacturerSpecificCause = *q;
            delete q;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_gsm0408Cause_chosen)
        {
            m_Diagnostics.choice = R4_Org_gsm0408Cause_chosen;
            m_Diagnostics.u.gsm0408Cause = 
                p->m_pOrgRec->diagnostics.u.gsm0408Cause;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_gsm0902MapErrorValue_chosen)
        {
            m_Diagnostics.choice = R4_Org_gsm0902MapErrorValue_chosen;
            m_Diagnostics.u.gsm0902MapErrorValue = 
                p->m_pOrgRec->diagnostics.u.gsm0902MapErrorValue;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_ccittQ767Cause_chosen)
        {
            m_Diagnostics.choice = R4_Org_ccittQ767Cause_chosen;
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


//C3G_R4_MCDR��ʵ��(M-CDR)
C3G_R4_MCDR::C3G_R4_MCDR()
{
    m_pCondition = NULL;
    m_pExtension = NULL;
    m_pRecordSequence = NULL;
    m_bPDPCompleted = FALSE;

    m_bPrepared = FALSE;

    m_nMinRecordSequence = 0;
    m_nCurRecordSequence = 0;

    m_Duration = 0;
    memset(&m_Diagnostics,0,sizeof(R4_Org_Diagnostics));
    m_CauseForRecClosing = R4_Org_normalRelease;

	memset(&m_CAMELInformationMM, 0, sizeof(R4_Org_CAMELInformationMM));

    m_pOrgRec = NULL;
    m_pDestRec = NULL;
}

C3G_R4_MCDR::~C3G_R4_MCDR()
{
    //����ԭʼ������¼�ռ䡣
    if(m_pOrgRec != NULL)
    {
        if(m_pOrgRec->bit_mask & R4_Org_changeLocation_present)
        {
			if(m_pOrgRec->changeLocation != NULL)
			{
				Clear_ChangeLocationList(m_pOrgRec->changeLocation);
			}
        }

        if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_diagnostics_present)
        {
            Clear_Diagnostics(&(m_pOrgRec->diagnostics));
        }

        if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_recordExtensions_present)
        {
            if(m_pOrgRec->recordExtensions != NULL)              
            {
                Clear_ManagementExtensions(m_pOrgRec->recordExtensions);
            }
        }

        delete m_pOrgRec;
        m_pOrgRec = NULL;
    }

    //�������ջ�����¼�ռ䡣
    if(m_pDestRec != NULL)
    {
        if(m_pDestRec->u.sgsnMMRecord.bit_mask
            & R4_Fin_changeLocation_present)
        {
			if(m_pDestRec->u.sgsnMMRecord.changeLocation != NULL)
			{
				Clear_ChangeLocationList((R4_Org_SGSNMMRecord::R4_Org__seqof3*)m_pDestRec->u.sgsnMMRecord.changeLocation);
			}
        }

        if(m_pDestRec->u.sgsnMMRecord.bit_mask
            & R4_Fin_SGSNMMRecord_diagnostics_present)
        {
            Clear_Diagnostics((R4_Org_Diagnostics*)
                &(m_pDestRec->u.sgsnMMRecord.diagnostics));
        }

        if(m_pDestRec->u.sgsnMMRecord.bit_mask
            & R4_Fin_SGSNMMRecord_recordExtensions_present)
        {
            if(m_pDestRec->u.sgsnMMRecord.recordExtensions != NULL)                
            {
                Clear_ManagementExtensions((R4_Org_ManagementExtensions_*)
                    m_pDestRec->u.sgsnMMRecord.recordExtensions);
            }
        }

        delete m_pDestRec;
        m_pDestRec = NULL;
    }

    /*
     * �ϲ�������ʹ�õ����ݽṹ�Ŀռ��Ѿ���GetContents�л��ա�
     */
}

BOOL C3G_R4_MCDR::PrepareData()
{
    //����duration�ֶ�
    m_Duration = m_pOrgRec->duration;

    //����causeForRecClosing�ֶ�
    m_CauseForRecClosing = m_pOrgRec->causeForRecClosing;

    //ת��changeLocation
    if(m_pOrgRec->bit_mask & R4_Org_changeLocation_present)
    {
        if(m_pOrgRec->changeLocation != NULL)            
        {
			unsigned int nCount = 0;
            m_pLocation = Clone_ChangeLocationList(m_pOrgRec->changeLocation, nCount);
                    
            m_pLocationTail = m_pLocation;
        }
    }

    //ת��recordExtensions
    if(m_pOrgRec->bit_mask
        & R4_Org_SGSNMMRecord_recordExtensions_present)
    {
        if(m_pOrgRec->recordExtensions != NULL)            
        {
			R4_Org_ManagementExtensions_* pTail = NULL;
			unsigned int nCount = 0;
            m_pExtension = Clone_ManagementExtensions(m_pOrgRec->recordExtensions, pTail, nCount);                
                        
            m_pExtensionTail = pTail;
        }
    }
/*
    //ת��recordSequenceNumber
    if(m_pOrgRec->bit_mask 
        & R4_Org_SGSNMMRecord_recordSequenceNumber_present)
    {
		
    }
*/
    //����diagnostics�ֶ�
    if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_diagnostics_present)
    {
        m_Diagnostics = m_pOrgRec->diagnostics;
        
        if(m_Diagnostics.choice == R4_Org_networkSpecificCause_chosen)
        {
            R4_Org_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(&(m_pOrgRec->diagnostics.u.networkSpecificCause));

            m_Diagnostics.choice = R4_Org_networkSpecificCause_chosen;
            m_Diagnostics.u.networkSpecificCause = *q;
            
            delete q;
        }
        else if(m_Diagnostics.choice 
            == R4_Org_manufacturerSpecificCause_chosen)
        {
            R4_Org_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(&(m_pOrgRec->diagnostics.u.manufacturerSpecificCause));

            m_Diagnostics.choice = R4_Org_manufacturerSpecificCause_chosen;
            m_Diagnostics.u.manufacturerSpecificCause = *q;
            
            delete q;
        }
    }

	//����cAMELInformationMM
	if(m_pOrgRec->bit_mask
		& R4_Org_cAMELInformationMM_present)
	{
		m_CAMELInformationMM = m_pOrgRec->cAMELInformationMM;
	}

    //׼�����
    m_bPrepared = TRUE;
    return TRUE;
}


/******************************************************
* ������:   C3G_R4_MCDR::Init
* ����:    Guofeng, Guan
* ����:    2003-06-02
* ����:    
* ����: 
*       ����1 :char* pBuf��M-CDR������¼��
*       ����2 :int nLen��������¼�ĳ��ȡ�
* ����ֵ: BOOL ����ʼ���ɹ�������TRUE���ڴ����ʧ���׳��쳣��
* ˵��: 
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*2003-06-02       Guofeng, Guan
******************************************************/
BOOL C3G_R4_MCDR::Init(char* pBuf,UINT4 uLen)
{
    assert(pBuf != NULL);

    R4_Org_SGSNMMRecord* p = (R4_Org_SGSNMMRecord*)pBuf;

    //����ԭʼ����
    m_pOrgRec = new R4_Org_SGSNMMRecord;
    if(m_pOrgRec == NULL)
    {
        throw "Alloc memory fail.";
    }

    memcpy(m_pOrgRec,p,sizeof(R4_Org_SGSNMMRecord));

    /*
     * ���浥������ָ���ֶ�
     */

    //����changeLocation
    if(p->bit_mask & R4_Org_changeLocation_present)
    {
        if(p->changeLocation != NULL)           
        {
			unsigned int nCount = 0;
            m_pOrgRec->changeLocation = Clone_ChangeLocationList(p->changeLocation, nCount);                        

            /* 
             * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
             * 
             */
            m_nLen_Location_List = nCount;
        }
    }

    //ת��recordExtensions
    if(p->bit_mask & R4_Org_SGSNMMRecord_recordExtensions_present)
    {
        if(p->recordExtensions != NULL)            
        {
			R4_Org_ManagementExtensions_* pTail = NULL;
			unsigned int nCount = 0;
            m_pOrgRec->recordExtensions = 
				Clone_ManagementExtensions(p->recordExtensions, pTail, nCount);                       

            /*
             * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
             * 
             */
            m_nLen_Extension_List = nCount;
        }
    }

    //����diagnostics�ֶ�
    if(p->bit_mask & R4_Org_SGSNMMRecord_diagnostics_present)
    {
        if(p->diagnostics.choice == R4_Org_networkSpecificCause_chosen)
        {
            R4_Org_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(&(p->diagnostics.u.networkSpecificCause));

            m_pOrgRec->diagnostics.choice = R4_Org_networkSpecificCause_chosen;
            m_pOrgRec->diagnostics.u.networkSpecificCause = *q;

            delete q;
        }
        else if(p->diagnostics.choice == R4_Org_manufacturerSpecificCause_chosen)
        {
            R4_Org_ManagementExtension* q = NULL;
            q = Clone_ManagementExtension(&(p->diagnostics.u.manufacturerSpecificCause));

            m_pOrgRec->diagnostics.choice = R4_Org_manufacturerSpecificCause_chosen;
            m_pOrgRec->diagnostics.u.manufacturerSpecificCause = *q;

            delete q;
        }
    }

    //���õ�ǰ�������к�
    m_nMinRecordSequence = m_nCurRecordSequence = p->recordSequenceNumber;

    //����PDP������־
    switch(p->causeForRecClosing)
    {
    case R4_Org_normalRelease:
    case R4_Org_abnormalRelease:
    case R4_Org_sGSNChange:
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

BOOL C3G_R4_MCDR::IsFirstPartialRec()
{
    return (m_pOrgRec->recordSequenceNumber == 0);
}

BOOL C3G_R4_MCDR::IsLastPartialRec()
{
    switch(m_pOrgRec->causeForRecClosing)
    {
    case R4_Org_normalRelease:
    case R4_Org_abnormalRelease:
    case R4_Org_sGSNChange:
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

BOOL C3G_R4_MCDR::IsPartial()
{
    /* 
     * ��recordSequenceNumber�ֶ���Ч���򻰵��ǲ��ֻ�����
     *
     */
    return (m_pOrgRec->bit_mask &
        R4_Org_SGSNMMRecord_recordSequenceNumber_present);
}

BOOL C3G_R4_MCDR::IsHotBilling()
{
    if((m_pOrgRec->chargingCharacteristics.value[0] & 0x0f) == 1)
    {
        return TRUE;
    }
    return FALSE;
}


/******************************************************
* ������:   C3G_R4_MCDR::CanSafeMerge
* ����:    Guofeng, Guan
* ����:    2003-06-02
* ����:    
* ����: 
*       ����1 :CBill* pBill,���ϲ��Ļ��������ָ�롣
* ����ֵ: BOOL ,�����Ż�������ϲ�������TRUE�����򷵻�FALSE��
* ˵��: 
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*2003-06-02       Guofeng, Guan
******************************************************/
BOOL C3G_R4_MCDR::CanSafeMerge(CBill* pBill)
{
    assert(pBill != NULL);

    C3G_R4_MCDR* p = (C3G_R4_MCDR*)pBill;

    //�����������к����Ѿ��ϲ��Ļ�����Χ�ڣ�˵�����ظ�������
    //�ظ�������Merge�����ж�����
    if((p->m_pOrgRec->bit_mask 
        & R4_Org_SGSNMMRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask 
        & R4_Org_SGSNMMRecord_recordSequenceNumber_present))
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

    if((m_nLen_Location_List + p->m_nLen_Location_List)
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
		    & R4_Org_cAMELInformationMM_present)
		&& (p->m_pOrgRec->bit_mask 
		    & R4_Org_cAMELInformationMM_present)
	    &&(m_pOrgRec->cAMELInformationMM.bit_mask
		    & R4_Org_CAMELInformationMM_freeFormatData_present)
		&& (p->m_pOrgRec->cAMELInformationMM.bit_mask
		    & R4_Org_CAMELInformationMM_fFDAppendIndicator_present)
		&& (p->m_pOrgRec->cAMELInformationMM.fFDAppendIndicator == TRUE) 
		&&(p->m_pOrgRec->cAMELInformationMM.bit_mask
		   & R4_Org_CAMELInformationMM_freeFormatData_present)
		&& (p->m_pOrgRec->cAMELInformationMM.freeFormatData.length != 0))
	{
		int nfreeFormatDatalength;
		
		if(m_bPrepared == TRUE)
		{
			nfreeFormatDatalength = m_CAMELInformationMM.freeFormatData.length
			          + p->m_pOrgRec->cAMELInformationMM.freeFormatData.length;
		}
		else
		{
			nfreeFormatDatalength = m_pOrgRec->cAMELInformationMM.freeFormatData.length
			          + p->m_pOrgRec->cAMELInformationMM.freeFormatData.length;
		}
						
		if(nfreeFormatDatalength > 160)
		{
			return FALSE;
		}
	}

    return (p->m_nCurRecordSequence == (m_nCurRecordSequence + 1));
}

BOOL C3G_R4_MCDR::IsPDPCompleted()
{
    return m_bPDPCompleted;
}

BOOL C3G_R4_MCDR::NeedWaiting()
{
    //��ǰ�ĺϲ�������ü�����ʽ�ϲ�Duration�ֶ�
    //���Ըú���ֻ����FALSE(��PDP����ʱ��������ȴ�)��
    BOOL bTmp = FALSE;
    return bTmp;
}

char* C3G_R4_MCDR::GetContents()
{
    if(m_pDestRec != NULL)
    {
        return (char*)m_pDestRec;
    }

    R4_Fin_CallEventRecord* pCallEventRec = new R4_Fin_CallEventRecord;
    if(pCallEventRec == NULL)
    {
        throw "Alloc memory fail.";
    }

    pCallEventRec->choice = R4_Fin_sgsnMMRecord_chosen;
    R4_Fin_SGSNMMRecord* pDest = &(pCallEventRec->u.sgsnMMRecord);

    memset(pDest,0,sizeof(R4_Fin_SGSNMMRecord));
    m_pDestRec = pCallEventRec;

    //�������ڴ�鿪ʼ/������ַ��
    char* pBegin = NULL,*pEnd = NULL;

    /*
     * ������û�о����ϲ������ԭʼ�����й������ջ�����
     */

    //�ϲ�ǰbit_mask��4���ֽڳ����ϲ�����2���ֽڳ�������ֱ�Ӹ�ֵ��
    if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_servedIMEI_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNMMRecord_servedIMEI_present;
    }
	if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_sgsnAddress_present)
	{
		pDest->bit_mask |= R4_Fin_SGSNMMRecord_sgsnAddress_present;
	}
    if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_msNetworkCapability_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNMMRecord_msNetworkCapability_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_routingArea_present)
    {
        pDest->bit_mask |= R4_Org_SGSNMMRecord_routingArea_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_locationAreaCode_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNMMRecord_locationAreaCode_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_cellIdentifier_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNMMRecord_cellIdentifier_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_changeLocation_present)
    {
        pDest->bit_mask |= R4_Fin_changeLocation_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_duration_present)
    {
        pDest->bit_mask |= R4_Fin_duration_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_sgsnChange_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNMMRecord_sgsnChange_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_diagnostics_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNMMRecord_diagnostics_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_recordSequenceNumber_present)
    {
        pDest->bit_mask |= R4_Fin_recordSequenceNumber_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_nodeID_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNMMRecord_nodeID_present;
    }
    if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_recordExtensions_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNMMRecord_recordExtensions_present;
    }
	if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_localSequenceNumber_present)
	{
		pDest->bit_mask |= R4_Fin_SGSNMMRecord_localSequenceNumber_present;
	}
    if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_servedMSISDN_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNMMRecord_servedMSISDN_present;
    }
	if(m_pOrgRec->bit_mask & R4_Org_cAMELInformationMM_present)
	{
		pDest->bit_mask |= R4_Fin_cAMELInformationMM_present;
	}
	if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_systemType_present)
	{
		pDest->bit_mask |= R4_Fin_SGSNMMRecord_systemType_present;
	}
    if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_chChSelectionMode_present)
    {
        pDest->bit_mask |= R4_Fin_SGSNMMRecord_chChSelectionMode_present;
    }
    
    if(!m_bPrepared)
    {
        pBegin = (char*)(&(m_pOrgRec->recordType));
        pEnd = (char*)(&(m_pOrgRec->changeLocation));
        
        /* 
         * recordType,servedIMSI,servedIMEI,sgsnAddress,
         * msNetworkCapability,routingArea,locationAreaCode,
         * cellIdentifier
         */
        memcpy(&(pDest->recordType),pBegin,(pEnd - pBegin));
    
        //����changeLocation�ֶ�
        if(m_pOrgRec->bit_mask & R4_Org_changeLocation_present)
        {
            if(m_pOrgRec->changeLocation != NULL)                
            {
				unsigned int nCount = 0;
                pDest->changeLocation = (R4_Fin_SGSNMMRecord::R4_Fin__seqof7*)
                    Clone_ChangeLocationList(m_pOrgRec->changeLocation, nCount);                                
            }
        }

        pBegin = (char*)(&(m_pOrgRec->recordOpeningTime));
        pEnd = (char*)(&(m_pOrgRec->diagnostics));
        /* 
         * recordOpeningTime,duration,sgsnChange,
         * causeForRecClosing
         */
        memcpy(&(pDest->recordOpeningTime),pBegin,(pEnd - pBegin));

        //����diagnostics�ֶ�
        if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_diagnostics_present)
        {
            memcpy(&(pDest->diagnostics),&(m_pOrgRec->diagnostics),
                sizeof(R4_Org_Diagnostics));

            if(m_pOrgRec->diagnostics.choice
                == R4_Org_networkSpecificCause_chosen)
            {
                R4_Org_ManagementExtension* q = NULL;
                q = Clone_ManagementExtension(&(m_pOrgRec->diagnostics.u.networkSpecificCause));

                pDest->diagnostics.choice = R4_Fin_networkSpecificCause_chosen;
                pDest->diagnostics.u.networkSpecificCause
                                            = *(R4_Fin_ManagementExtension*)q;
                delete q;
            }
            else if(m_pOrgRec->diagnostics.choice
                == R4_Org_manufacturerSpecificCause_chosen)
            {
                R4_Org_ManagementExtension* q = NULL;
                q = Clone_ManagementExtension(&(m_pOrgRec->diagnostics.u.manufacturerSpecificCause));

                pDest->diagnostics.choice = R4_Fin_manufacturerSpecificCause_chosen;
                pDest->diagnostics.u.manufacturerSpecificCause
                                            = *(R4_Fin_ManagementExtension*)q;
                delete q;
            }
        }

        if(m_pOrgRec->bit_mask
            & R4_Org_SGSNMMRecord_recordSequenceNumber_present)
        {
            pDest->recordSequenceNumber = m_pOrgRec->recordSequenceNumber;                        
        }

        //����nodeID�ֶ�
        if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_nodeID_present)
        {
            memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(R4_Org_NodeID));
        }

        //����recordExtensions�ֶ�
        if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_recordExtensions_present)
        {
            if(m_pOrgRec->recordExtensions != NULL)                
            {
				R4_Org_ManagementExtensions_* pTail = NULL;
				unsigned int nCount = 0;
                pDest->recordExtensions = (R4_Fin_ManagementExtensions_*)
                    Clone_ManagementExtensions(m_pOrgRec->recordExtensions, pTail, nCount);                                
            }
        }        

        pBegin = (char*)(&(m_pOrgRec->localSequenceNumber));
        pEnd = (char*)(&(m_pOrgRec->chChSelectionMode));
        /* 
         * servedMSISDN,chargingCharacteristics,
		 * cAMELInformationMM,systemType
         */
        //ע:����CAMEL��������޸�zouyongsheng
		memcpy(&(pDest->localSequenceNumber),pBegin,
            (pEnd - pBegin) + sizeof(R4_Fin_ChChSelectionMode));

        return (char*)m_pDestRec;
    }

    /*
     * �����Ǻϲ��󻰵�����Ϲ��̡�
     */

    pBegin = (char*)(&(m_pOrgRec->recordType));
    pEnd = (char*)(&(m_pOrgRec->changeLocation));

    /* 
    *recordType,servedIMSI,servedIMEI,sgsnAddress,
    *msNetworkCapability,routingArea,locationAreaCode,
    *cellIdentifier
    */
    memcpy(&(pDest->recordType),pBegin,(pEnd - pBegin));
    
    //�ϲ�changeLocation�ֶ�
    if(m_pOrgRec->bit_mask & R4_Org_changeLocation_present)
    {
        if(m_pLocation != NULL)
        {
            pDest->changeLocation = (R4_Fin_SGSNMMRecord::R4_Fin__seqof7*) m_pLocation;
                        
            m_pLocation = m_pLocationTail = NULL;
        }
    }

    pBegin = (char*)(&(m_pOrgRec->recordOpeningTime));
    pEnd = (char*)(&(m_pOrgRec->diagnostics));
    /* 
     * recordOpeningTime,duration,sgsnChange,
     * causeForRecClosing
     */
    memcpy(&(pDest->recordOpeningTime),pBegin,(pEnd - pBegin));

    //�ϲ�diagnostics�ֶ�
    if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_diagnostics_present)
    {
        memcpy(&(pDest->diagnostics),&(m_Diagnostics),
            sizeof(R4_Org_Diagnostics));
    }

    //�ϲ�recordSequenceNumber�ֶ�
    if(m_pOrgRec->bit_mask
        & R4_Org_SGSNMMRecord_recordSequenceNumber_present)
    {
        pDest->recordSequenceNumber = m_pOrgRec->recordSequenceNumber;  
    }

    //nodeID�ֶ�(һ������)
    if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_nodeID_present)
    {
        memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(R4_Org_NodeID));
    }

    //�ϲ�recordExtensions�ֶ�
    if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_recordExtensions_present)
    {
        if(m_pExtension != NULL)
        {
            pDest->recordExtensions = (R4_Fin_ManagementExtensions_*) m_pExtension;
                        
            m_pExtension = m_pExtensionTail = NULL;
        }
    }

    pBegin = (char*)(&(m_pOrgRec->localSequenceNumber));
    pEnd = (char*)(&(m_pOrgRec->chChSelectionMode));
    /* 
     * servedMSISDN,chargingCharacteristics,
	 * cAMELInformationMM,systemType
     */
    //CAMEL���ڴ˲�����������޸Ĵ���
	memcpy(&(pDest->localSequenceNumber),pBegin,
        (pEnd - pBegin) + sizeof(R4_Fin_ChChSelectionMode));

    pDest->duration = m_Duration;
    pDest->causeForRecClosing = m_CauseForRecClosing;

	memcpy(&(pDest->cAMELInformationMM), 
		&(m_CAMELInformationMM), sizeof(R4_Fin_CAMELInformationMM));

    return (char*)m_pDestRec;
}

UINT4 C3G_R4_MCDR::GetBillType()
{
    return m_pOrgRec->recordType;
}

CKey C3G_R4_MCDR::CreateKey()
{
    C3G_R4_MCDR_Key* pkey = new C3G_R4_MCDR_Key;
    if(pkey == NULL)
    {
        throw "Alloc memory fail.";
    }

    pkey->servedIMSI = m_pOrgRec->servedIMSI;

    CKey key;
    key.m_pDelegation = pkey;

    return key;
}

BOOL C3G_R4_MCDR::IsDuplicated(CBill* pBill)
{
    BOOL bSucc = FALSE;
    C3G_R4_MCDR* p = (C3G_R4_MCDR*)pBill;
    if((p->m_pOrgRec->bit_mask 
        & R4_Org_SGSNMMRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask 
        & R4_Org_SGSNMMRecord_recordSequenceNumber_present))
    {
        if((p->m_nCurRecordSequence <= m_nCurRecordSequence)
            && (p->m_nCurRecordSequence >= m_nMinRecordSequence))
        {
            bSucc = TRUE;
        }
    }
    
    return bSucc;
    
}

/******************************************************
* ������:   C3G_R4_MCDR::Merge
* ����:    Guofeng, Guan
* ����:    2003-06-02
* ����:    �ѻ���pBill���뵱ǰ�����С�
* ����: 
*       ����1 :CBill* pBill�����ϲ��Ļ�����
* ����ֵ: BOOL���ϲ��ɹ�����TRUE��ʧ�ܷ���FALSE��
* ˵��: 
*       �ϲ������и��µ��ֶ���:
*       ��Ҫ���ӵ��ֶ�(changeLocation,recordSequenceNumber,
*       recordExtensions),ȡ���һ�Ż���ֵ���ֶ�(causeForRecClosing,
*       diagnostics),��Ҫ�ۼӵ��ֶ�(Duration)��
*       ȡ��һ�Ż���ֵ���ֶβ��䡣
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*2003-06-02       Guofeng, Guan
******************************************************/
BOOL C3G_R4_MCDR::Merge(CBill* pBill)
{
    assert(pBill != NULL);

    if(!m_bPrepared)
    {
        if(!PrepareData())  return FALSE;
    }

    C3G_R4_MCDR* p = (C3G_R4_MCDR*)pBill;

    //�����ظ�������
    if((p->m_pOrgRec->bit_mask 
        & R4_Org_SGSNMMRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask 
        & R4_Org_SGSNMMRecord_recordSequenceNumber_present))
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
    case R4_Org_normalRelease:
    case R4_Org_abnormalRelease:
    case R4_Org_sGSNChange:
        m_bPDPCompleted = TRUE;
        break;
    default:
        break;
    }

    //����causeForRecClosing�ֶΡ�
    m_CauseForRecClosing = p->m_pOrgRec->causeForRecClosing;

    //����changeLocation�ֶΡ�
    if((p->m_pOrgRec->bit_mask & R4_Org_changeLocation_present)
        && (m_pOrgRec->bit_mask & R4_Org_changeLocation_present))
    {
        if(p->m_pOrgRec->changeLocation != NULL)            
        {
			unsigned int nCount = 0;
			R4_Org_SGSNMMRecord::R4_Org__seqof3* q = NULL;            
            q = Clone_ChangeLocationList(p->m_pOrgRec->changeLocation, nCount);                        

            if(m_pLocationTail == NULL)
            {
                m_pLocation = m_pLocationTail = q;
            }
            else
            {
                m_pLocationTail->next = q;
                m_pLocationTail = q;
            }

            /* 
             * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
             * 
             */
            m_nLen_Location_List += nCount;
        }
    }

    //����recordExtensions�ֶΡ�
    if((p->m_pOrgRec->bit_mask
        & R4_Org_SGSNMMRecord_recordExtensions_present)
        && (m_pOrgRec->bit_mask
        & R4_Org_SGSNMMRecord_recordExtensions_present))
    {
        if(p->m_pOrgRec->recordExtensions != NULL)            
        {
			R4_Org_ManagementExtensions_* pTail = NULL;
			unsigned int nCount = 0;
            R4_Org_ManagementExtensions_* q = NULL;                            
            q = Clone_ManagementExtensions(p->m_pOrgRec->recordExtensions, pTail, nCount); 
            if(m_pExtensionTail == NULL)
            {
                m_pExtension = m_pExtensionTail = q;
            }
            else
            {
                m_pExtensionTail->next = q;
                m_pExtensionTail = q;
            }

            /* 
             * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
             * 
             */
            m_nLen_Extension_List += nCount;
        }
    }

    //����recordSequenceNumber�ֶΡ�
    if((p->m_pOrgRec->bit_mask 
        & R4_Org_SGSNMMRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask 
        & R4_Org_SGSNMMRecord_recordSequenceNumber_present))
    {
        //
    }

    //�ۼ�Duration�ֶΡ�
    if((p->m_pOrgRec->bit_mask & R4_Org_duration_present)
        && (m_pOrgRec->bit_mask & R4_Org_duration_present))
    {
        int nDiff = Get_Duration(&(m_pOrgRec->recordOpeningTime),
                                 &(p->m_pOrgRec->recordOpeningTime));
        m_Duration = nDiff;
        m_Duration += p->m_pOrgRec->duration;
        
    }

    //diagnostics�ֶ�ȡ���һ�Ż�����ֵ��
    if((p->m_pOrgRec->bit_mask 
        & R4_Org_SGSNMMRecord_diagnostics_present)
        && (m_pOrgRec->bit_mask 
        & R4_Org_SGSNMMRecord_diagnostics_present))
    {
        R4_Org_ManagementExtension* q = NULL;
        if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_networkSpecificCause_chosen)
        {
            q = Clone_ManagementExtension(&(p->m_pOrgRec->diagnostics.u.networkSpecificCause));
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_manufacturerSpecificCause_chosen)
        {
            q = Clone_ManagementExtension(&(p->m_pOrgRec->diagnostics.u.manufacturerSpecificCause));
        }

        /*
         * ����ԭdiagnostics�ֶεĿռ䡣
         */
        if(m_Diagnostics.choice == R4_Org_manufacturerSpecificCause_chosen)
        {
            delete[] m_Diagnostics.u.manufacturerSpecificCause.identifier.value;
            delete[] m_Diagnostics.u.manufacturerSpecificCause.information.value;
            m_Diagnostics.choice = 0;
        }
        else if(m_Diagnostics.choice == R4_Org_networkSpecificCause_chosen)
        {
            delete[] m_Diagnostics.u.networkSpecificCause.identifier.value;
            delete[] m_Diagnostics.u.networkSpecificCause.information.value;
            m_Diagnostics.choice = 0;
        }
        /************************/

        if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_networkSpecificCause_chosen)
        {
            m_Diagnostics.choice = R4_Org_networkSpecificCause_chosen;
            
            m_Diagnostics.u.networkSpecificCause = *q;
            delete q;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_manufacturerSpecificCause_chosen)
        {
            m_Diagnostics.choice = R4_Org_manufacturerSpecificCause_chosen;
            
            m_Diagnostics.u.manufacturerSpecificCause = *q;
            delete q;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_gsm0408Cause_chosen)
        {
            m_Diagnostics.choice = R4_Org_gsm0408Cause_chosen;
            m_Diagnostics.u.gsm0408Cause = 
                p->m_pOrgRec->diagnostics.u.gsm0408Cause;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_gsm0902MapErrorValue_chosen)
        {
            m_Diagnostics.choice = R4_Org_gsm0902MapErrorValue_chosen;
            m_Diagnostics.u.gsm0902MapErrorValue = 
                p->m_pOrgRec->diagnostics.u.gsm0902MapErrorValue;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == R4_Org_ccittQ767Cause_chosen)
        {
            m_Diagnostics.choice = R4_Org_ccittQ767Cause_chosen;
            m_Diagnostics.u.ccittQ767Cause = 
                p->m_pOrgRec->diagnostics.u.ccittQ767Cause;
        }
    }

	//�ϲ�CAMELInformation�����Ϣ
    if((p->m_pOrgRec->bit_mask 
		& R4_Org_cAMELInformationMM_present)
		&& (m_pOrgRec->bit_mask
		& R4_Org_cAMELInformationMM_present))
	{
		/*
		 * sCFAddress,serviceKey,defaultTransactionHandling,
		 * ������ȡ��һ�Ų��ֻ���������
		 */

		/*
		 * numberOfDPEncountered,levelOfCAMELService
		 * ������ȡ���һ�Ų��ֻ���������
		 */
		m_CAMELInformationMM.numberOfDPEncountered = 
				p->m_pOrgRec->cAMELInformationMM.numberOfDPEncountered;

		m_CAMELInformationMM.levelOfCAMELService = 
				p->m_pOrgRec->cAMELInformationMM.levelOfCAMELService;
				
		/*
		 * fFDAppendIndicator�ֶδ�����ֵΪTRUE��freeFormatData
		 * ��Ϣ���Ȳ�Ϊ�㣬���ǰ��Ļ����ϲ�
		 */
		//fFDAppendIndicator �ֶβ����ڻ��߸���ΪFALSE
		//freeFormatData���ڶ�����Ϣ���Ȳ�Ϊ��
		if((!(p->m_pOrgRec->cAMELInformationMM.bit_mask 
			& R4_Org_CAMELInformationMM_fFDAppendIndicator_present)
			|| ((p->m_pOrgRec->cAMELInformationMM.bit_mask 
			& R4_Org_CAMELInformationMM_fFDAppendIndicator_present)
			&& (p->m_pOrgRec->cAMELInformationMM.fFDAppendIndicator == FALSE)))
			&& (p->m_pOrgRec->cAMELInformationMM.bit_mask
			& R4_Org_CAMELInformationMM_freeFormatData_present)
			&&(p->m_pOrgRec->cAMELInformationMM.freeFormatData.length != 0))
		{
			memcpy(m_CAMELInformationMM.freeFormatData.value,
				p->m_pOrgRec->cAMELInformationMM.freeFormatData.value,
				p->m_pOrgRec->cAMELInformationMM.freeFormatData.length);
			
			m_CAMELInformationMM.freeFormatData.length =
				p->m_pOrgRec->cAMELInformationMM.freeFormatData.length;

			m_CAMELInformationMM.bit_mask |= 
				R4_Org_CAMELInformationMM_freeFormatData_present;
		}
		
		//fFDAppendIndicator �ֶδ�����ֵΪTRUE
		//freeFormatData���ڶ�����Ϣ���Ȳ�Ϊ��
		if((p->m_pOrgRec->cAMELInformationMM.bit_mask
			& R4_Org_CAMELInformationMM_fFDAppendIndicator_present)
			&& (p->m_pOrgRec->cAMELInformationMM.fFDAppendIndicator == TRUE) 
			&&(p->m_pOrgRec->cAMELInformationMM.bit_mask
			& R4_Org_CAMELInformationMM_freeFormatData_present)
			&& (p->m_pOrgRec->cAMELInformationMM.freeFormatData.length != 0))
		{
			memcpy(m_CAMELInformationMM.freeFormatData.value 
				+ m_CAMELInformationMM.freeFormatData.length,
				p->m_pOrgRec->cAMELInformationMM.freeFormatData.value,
				p->m_pOrgRec->cAMELInformationMM.freeFormatData.length);

			m_CAMELInformationMM.freeFormatData.length +=
				p->m_pOrgRec->cAMELInformationMM.freeFormatData.length;

			m_CAMELInformationMM.bit_mask |= 
				R4_Org_CAMELInformationMM_freeFormatData_present;
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
