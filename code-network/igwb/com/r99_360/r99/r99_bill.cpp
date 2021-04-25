/*****************************************************
*       3G_Bill.cpp
******************************************************
*Copyright 2001 by Huawei CO.,LTD.
* All rights reserved.
* Compiler:     Visual C++ 6.0
* Author:       Zhou Yonghui
* Date:         01-8-29
* Description:  
* 
* Side Effects: NONE
* Functions:
* Notes:    
* 
* Update: 01-8-29 ���� 09:09:51
* Date              Name              Description 
============== ================ ======================
## 01-8-29       Zhou Yonghui       �������޸�
   
   �������ֶν��кϲ�֮ǰ�������������

   1���������ǿ�ѡ�ֶΣ��ж���Ч�Ա���Ƿ����á�
   �����û�����ã��򲻺ϲ����ֶΡ�

   2�����ֶβ���ѡ�����߿�ѡ�ֶε���Ч�Ա���Ѿ����ã�
   ���ж��ֶ������Ƿ���Ч��
   ��������Ч��ϲ����ֶΣ����򲻺ϲ���

*****************************************************/
#include "r99_bill.h"
#include "../../merge/merge.h"
#include "../../include/export_dll.h"

extern "C" IGWB_Svc_Export void* CreateParserObject()
{
    return (void*)new C3G_BillParser();
}


C3G_BillParser::C3G_BillParser()
{
    m_pBill = NULL;
}

C3G_BillParser::~C3G_BillParser()
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
BOOL C3G_BillParser::ParseARecord(char* pBuf,UINT4 uBufLen)
{
    assert(pBuf != NULL);
    
    CDR3G_CallEventRecord* pRec = (CDR3G_CallEventRecord*)pBuf;
    switch(pRec->choice)
    {
        case CDR3G_sgsnPDPRecord_chosen:
            m_pBill = new C3G_350_SCDR;

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

        case CDR3G_ggsnPDPRecord_chosen:
            m_pBill = new C3G_350_GCDR;
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
        //updated by maimaoshi,2003-04-24
	/*
        case CDR3G_sgsnMMRecord_chosen:
            m_pBill = new C3G_350_MCDR;
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

CBill* C3G_BillParser::GetBillRecord()
{
    CBill* pBill = m_pBill;
    m_pBill = NULL;
    return pBill;
}

UINT4 C3G_BillParser::GetBillTypeCount()
{
    //3G��Ҫ�ϲ��Ļ����ж��֣�S-CDR,G-CDR��
    return 2;
}

UINT4 C3G_BillParser::GetBillTypeID(UINT4 uIndex)
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
* ������:   C3G_BillParser::RegisterBillType
* ����:    Zhou Yonghui
* ����:    01-6-8
* ����:    ��CMerger����ע�Ự�����͡�
* ����: 
*       ����1 :CMerger* pMerger
* ����ֵ: void 
* ˵��:     ���ڴ����ʧ�ܽ��׳��쳣��
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-6-8        Zhou Yonghui
******************************************************/
void C3G_BillParser::RegisterBillType(CMerger* pMerger)
{
    assert(pMerger != NULL);
    
    for(UINT4 i = 0; i < GetBillTypeCount(); i++)
    {
        pMerger->RegisterBillType(GetBillTypeID(i));
    }
}

//����Զ2002-03-06���
//���ָ���Ļ�������ID�Ļ�����������,szDescΪ�������
void C3G_BillParser::GetBillTypeDesc(UINT4 uBillTypeID, char* szDesc)
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
//��ӽ���������Զ2002-03-06


//C3G_350_SCDR��ʵ��(S-CDR)
C3G_350_SCDR::C3G_350_SCDR()
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
    memset(&m_Diagnostics,0,sizeof(CDR3G_Diagnostics));
    m_CauseForRecClosing = CDR3G_normalRelease;

	//added by zouyongsheng
	memset(&m_CAMELInformationPDP, 0, sizeof(CDR3G_CAMELInformationPDP));

    m_pOrgRec = NULL;
    m_pDestRec = NULL;
    
    m_bIsLimit = FALSE;
}

C3G_350_SCDR::~C3G_350_SCDR()
{
    //����ԭʼ������¼�ռ䡣
    if(m_pOrgRec != NULL)
    {
        delete[] m_pOrgRec->listOfTrafficVolumes.value;
		
        if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_diagnostics_present)
        {
            Clear_Diagnostics(&(m_pOrgRec->diagnostics));
        }

        if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_recordExtensions_present)
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
        delete[] m_pDestRec->u.sgsnPDPRecord.listOfTrafficVolumes.value;

        if(m_pDestRec->u.sgsnPDPRecord.bit_mask
            & _3GCDR_SGSNPDPRecord_diagnostics_present)
        {
            Clear_Diagnostics((CDR3G_Diagnostics*)&
                (m_pDestRec->u.sgsnPDPRecord.diagnostics));
        }

        if(m_pDestRec->u.sgsnPDPRecord.bit_mask
            & _3GCDR_SGSNPDPRecord_recordExtensions_present)
        {
            if((m_pDestRec->u.sgsnPDPRecord.recordExtensions.value != NULL)
                && (m_pDestRec->u.sgsnPDPRecord.recordExtensions.count > 0))
            {
                Clear_ManagementExtension((CDR3G_ManagementExtension*)
                    m_pDestRec->u.sgsnPDPRecord.recordExtensions.value,
                    m_pDestRec->u.sgsnPDPRecord.recordExtensions.count);
            }
        }

        if(m_pDestRec->u.sgsnPDPRecord.bit_mask
            & _3GCDR_SGSNPDPRecord_recSequenceNumList_present)
        {
            delete[] m_pDestRec->u.sgsnPDPRecord.
                recSequenceNumList.recSequenceNumber.value;
        }

        delete m_pDestRec;
        m_pDestRec = NULL;
    }

    /*
     * �ϲ�������ʹ�õ����ݽṹ�Ŀռ��Ѿ���GetContents�л��ա�
     */
}


/******************************************************
* ������:   C3G_350_SCDR::Init
* ����:    Zhou Yonghui
* ����:    01-5-28
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
*01-5-28       Zhou Yonghui
*01-9-25       Zhou Yonghui
*              �����˳�ʼ��m_nLen_Condition_List��m_nLen_Extension_List
*              �Ĵ��룬�Ա�����������ơ�
******************************************************/
BOOL C3G_350_SCDR::Init(char* pBuf,UINT4 uLen)
{
    assert(pBuf != NULL);
    
    CDR3G_SGSNPDPRecord* p = (CDR3G_SGSNPDPRecord*)pBuf;

    //����ԭʼ����
    m_pOrgRec = new CDR3G_SGSNPDPRecord;
    if(m_pOrgRec == NULL)
    {
        return FALSE;
    }

    //���ƻ�����¼
    memcpy(m_pOrgRec,p,sizeof(CDR3G_SGSNPDPRecord));

    /*
     * ���浥������ָ���ֶ�
     */

    //����listOfTrafficVolumes�ֶ�
    if((p->listOfTrafficVolumes.value != NULL)
        && (p->listOfTrafficVolumes.count > 0))
    {
        m_pOrgRec->listOfTrafficVolumes.value
            = Clone_CDR3G_ChangeOfCharCondition(
            p->listOfTrafficVolumes.value,
            p->listOfTrafficVolumes.count);

        m_pOrgRec->listOfTrafficVolumes.count = p->listOfTrafficVolumes.count;

        /* 
         * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
         * Added by zhouyonghui,2001/09/25.
         */
        m_nLen_Condition_List = p->listOfTrafficVolumes.count;
    }

    //����diagnostics�ֶ�
    if(p->bit_mask & CDR3G_SGSNPDPRecord_diagnostics_present)
    {
        if(p->diagnostics.choice == CDR3G_networkSpecificCause_chosen)
        {
            CDR3G_ManagementExtension* q = NULL;
            q = Clone_CDR3G_ManagementExtension(
                        &(p->diagnostics.u.networkSpecificCause),1);

            m_pOrgRec->diagnostics.choice = CDR3G_networkSpecificCause_chosen;
            m_pOrgRec->diagnostics.u.networkSpecificCause = *q;
            
            delete[] q;
        }
        else if(p->diagnostics.choice 
            == CDR3G_manufacturerSpecificCause_chosen)
        {
            CDR3G_ManagementExtension* q = NULL;
            q = Clone_CDR3G_ManagementExtension(
                    &(p->diagnostics.u.manufacturerSpecificCause),1);

            m_pOrgRec->diagnostics.choice = CDR3G_manufacturerSpecificCause_chosen;
            m_pOrgRec->diagnostics.u.manufacturerSpecificCause = *q;
            
            delete[] q;
        }
    }

    //����recordExtensions�ֶ�
    if(p->bit_mask & CDR3G_SGSNPDPRecord_recordExtensions_present)
    {
        if((p->recordExtensions.value != NULL)
            && (p->recordExtensions.count > 0))
        {
            m_pOrgRec->recordExtensions.value
                = Clone_CDR3G_ManagementExtension(
                p->recordExtensions.value,
                p->recordExtensions.count);
            
            m_pOrgRec->recordExtensions.count = p->recordExtensions.count;

            /* 
             * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
             * Added by zhouyonghui,2001/09/25.
             */
            m_nLen_Extension_List = p->recordExtensions.count;
        }
    }

    //���õ�ǰ�������к�
    m_nMinRecordSequence = m_nCurRecordSequence = p->recordSequenceNumber;

    //����PDP������־
    switch(p->causeForRecClosing)
    {
    case CDR3G_normalRelease:
    case CDR3G_abnormalRelease:
    case CDR3G_sGSNChange:
        m_bPDPCompleted = TRUE;
        break;
    default:
        break;
    }

    //Added by ZhengYuqun 2003-11-04 SWPD02238
    if((UINT4)p->duration >= m_uMaxDuration)
    {
        m_bPDPCompleted = TRUE;
    }

    return TRUE;
}

BOOL C3G_350_SCDR::IsFirstPartialRec()
{
    //return (m_pOrgRec->recordSequenceNumber == 1);
    //maimaoshi,2001.06.26
    if (m_pOrgRec->sgsnChange == 0)
    {
        return (m_pOrgRec->recordSequenceNumber == 0);
    }
    else
    {
        return (m_pOrgRec->recordSequenceNumber == 1);
    }
}

BOOL C3G_350_SCDR::IsLastPartialRec()
{
    switch(m_pOrgRec->causeForRecClosing)
    {
    case CDR3G_normalRelease:
    case CDR3G_abnormalRelease:
    case CDR3G_sGSNChange:
        return TRUE;
        break;
    default:
        break;
    }

    //Added by ZhengYuqun 2003-11-04 SWPD02238
    if((UINT4)m_pOrgRec->duration >= m_uMaxDuration)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL C3G_350_SCDR::IsPartial()
{
    /* 
     * ��recordSequenceNumber�ֶ���Ч���򻰵��ǲ��ֻ�����
     * Added by zhouyonghui,2001-07-03
     */
    return (m_pOrgRec->bit_mask &
        CDR3G_SGSNPDPRecord_recordSequenceNumber_present);
}

BOOL C3G_350_SCDR::IsHotBilling()
{
    if((m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_chargingCharacteristics_present)
        && ((m_pOrgRec->chargingCharacteristics.value[0] & 0x0f) == 1))
    {
        return TRUE;
    }
    return FALSE;
}

/******************************************************
* ������:   C3G_350_SCDR::CanSafeMerge
* ����:    Zhou Yonghui
* ����:    01-5-29
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
*01-5-29       Zhou Yonghui
*01-9-25       Zhou Yonghui     �������������ƴ��룬
*                               ���󻰵�ǿ���жϺϲ���
*02-8-07       Zou Yongsheng    ���Ӷ���CAMEL���е�
                                freeFormatData�����
								����ǿ���жϺϲ�
******************************************************/
BOOL C3G_350_SCDR::CanSafeMerge(CBill* pBill)
{
    assert(pBill != NULL);

    C3G_350_SCDR* p = (C3G_350_SCDR*)pBill;

    //�����������к����Ѿ��ϲ��Ļ�����Χ�ڣ�˵�����ظ�������
    //�ظ�������Merge�����ж�����
    if((p->m_pOrgRec->bit_mask 
        & CDR3G_SGSNPDPRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask 
        & CDR3G_SGSNPDPRecord_recordSequenceNumber_present))
    {
        if((p->m_nCurRecordSequence <= m_nCurRecordSequence)
            && (p->m_nCurRecordSequence >= m_nMinRecordSequence))
        {
            return TRUE;
        }
    }

    /* 
     * ���󻰵���ǿ�����ߡ�
     * Added by zhouyonghui,2001/09/25.
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
	 * added by zouyongsheng 
	 */
	if((m_pOrgRec->bit_mask 
		    & CDR3G_cAMELInformationPDP_present)
		&& (p->m_pOrgRec->bit_mask 
		    & CDR3G_cAMELInformationPDP_present)
		&& (m_pOrgRec->cAMELInformationPDP.bit_mask
		    & CDR3G_CAMELInformationPDP_freeFormatData_present)
		&& (p->m_pOrgRec->cAMELInformationPDP.bit_mask
		    & CDR3G_CAMELInformationPDP_fFDAppendIndicator_present)
		&& (p->m_pOrgRec->cAMELInformationPDP.fFDAppendIndicator == TRUE) 
		&&(p->m_pOrgRec->cAMELInformationPDP.bit_mask
		   & CDR3G_CAMELInformationPDP_freeFormatData_present)
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
     
    //�������ظ���������Ҫ�����к�������
    return (p->m_nCurRecordSequence == (m_nCurRecordSequence + 1));
}

BOOL C3G_350_SCDR::IsPDPCompleted()
{
    return m_bPDPCompleted;
}

BOOL C3G_350_SCDR::NeedWaiting()
{
    //��ǰ�ĺϲ�������ü�����ʽ�ϲ�Duration�ֶ�
    //���Ըú���ֻ����FALSE(��PDP����ʱ��������ȴ�)��
    return FALSE;
}


/******************************************************
* ������:   C3G_350_SCDR::GetContents
* ����:    Zhou Yonghui
* ����:    01-5-29
* ����:    �����������ջ�����ʽ��ϻ�����¼��
* ����: 
* ����ֵ: char*,�������ջ�����ʽ�Ļ�����¼��
* ˵��: 
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-5-29       Zhou Yonghui
*02-8-07       Zou Yongsheng    ���Ӷ�CAMEL��Ĵ���
******************************************************/
char* C3G_350_SCDR::GetContents()
{
    if(m_pDestRec != NULL)
    {
        return (char*)m_pDestRec;
    }

    _3GCDR_CallEventRecord* pCallEventRec = new _3GCDR_CallEventRecord;
    if(pCallEventRec == NULL)
    {
        throw "Alloc memory fail.";
    }

    pCallEventRec->choice = _3GCDR_sgsnPDPRecord_chosen;
    _3GCDR_SGSNPDPRecord* pDest = &(pCallEventRec->u.sgsnPDPRecord);

    //��ʼ��Ϊ0��
    memset(pDest,0,sizeof(_3GCDR_SGSNPDPRecord));
    m_pDestRec = pCallEventRec;

    //�������ڴ�鿪ʼ/������ַ��
    char* pBegin = NULL,*pEnd = NULL;

    /*
     * ������û�о����ϲ������ԭʼ�����й������ջ�����
     */

    //�ϲ�ǰbit_mask��4���ֽڳ����ϲ�����2���ֽڳ�������ֱ�Ӹ�ֵ��
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_networkInitiation_present)
    {
        pDest->bit_mask |= _3GCDR_SGSNPDPRecord_networkInitiation_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_servedIMEI_present)
    {
        pDest->bit_mask |= _3GCDR_SGSNPDPRecord_servedIMEI_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_msNetworkCapability_present)
    {
        pDest->bit_mask |= _3GCDR_SGSNPDPRecord_msNetworkCapability_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_routingArea_present)
    {
        pDest->bit_mask |= _3GCDR_SGSNPDPRecord_routingArea_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_locationAreaCode_present)
    {
        pDest->bit_mask |= _3GCDR_SGSNPDPRecord_locationAreaCode_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_cellIdentifier_present)
    {
        pDest->bit_mask |= _3GCDR_SGSNPDPRecord_cellIdentifier_present;
    }
	//����Զ2002.05.27��ӣ����ⵥ��D13999
	if (m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_servedPDPAddress_present)
	{
		pDest->bit_mask |= _3GCDR_SGSNPDPRecord_servedPDPAddress_present;
	}
	//����Զ2002.05.27��ӽ���
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_sgsnChange_present)
    {
        pDest->bit_mask |= _3GCDR_SGSNPDPRecord_sgsnChange_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_diagnostics_present)
    {
        pDest->bit_mask |= _3GCDR_SGSNPDPRecord_diagnostics_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_recordSequenceNumber_present)
    {
        pDest->bit_mask |= _3GCDR_SGSNPDPRecord_recSequenceNumList_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_nodeID_present)
    {
        pDest->bit_mask |= _3GCDR_SGSNPDPRecord_nodeID_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_recordExtensions_present)
    {
        pDest->bit_mask |= _3GCDR_SGSNPDPRecord_recordExtensions_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_apnSelectionMode_present)
    {
        pDest->bit_mask |= _3GCDR_SGSNPDPRecord_apnSelectionMode_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_servedMSISDN_present)
    {
        pDest->bit_mask |= _3GCDR_SGSNPDPRecord_servedMSISDN_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_chargingCharacteristics_present)
    {
        pDest->bit_mask |= _3GCDR_SGSNPDPRecord_chargingCharacteristics_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_systemType_present)
    {
        pDest->bit_mask |= _3GCDR_SGSNPDPRecord_systemType_present;
    }
    
	//added by zouyongsheng 2002-08-06
	if(m_pOrgRec->bit_mask & CDR3G_cAMELInformationPDP_present)
	{
		pDest->bit_mask |=  _3GCDR_cAMELInformationPDP_present;
	}
    //end 

    if(m_pOrgRec->bit_mask & CDR3G_rNCUnsentDownlinkVolume_present)
    {
        pDest->bit_mask |= _3GCDR_rNCUnsentDownlinkVolume_present;
    }

    if(!m_bPrepared)
    {
        pBegin = (char*)(&(m_pOrgRec->recordType));
        pEnd = (char*)(&(m_pOrgRec->listOfTrafficVolumes));

        /*
         * recordType,networkInitiation,servedIMSI,
         * servedIMEI,sgsnAddress,msNetworkCapability,routingArea,
         * locationAreaCode,cellIdentifier,chargingID,ggsnAddressUsed,
         * accessPointNameNI,pdpType,servedPDPAddress
         */
        memcpy(&(pDest->recordType),pBegin,(pEnd - pBegin));

        //����listOfTrafficVolumes�ֶ�
        if((m_pOrgRec->listOfTrafficVolumes.value != NULL)
            && (m_pOrgRec->listOfTrafficVolumes.count > 0))
        {
            pDest->listOfTrafficVolumes.value = (_3GCDR_ChangeOfCharCondition*)
                Clone_CDR3G_ChangeOfCharCondition(
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
        if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_diagnostics_present)
        {
            memcpy(&(pDest->diagnostics),&(m_pOrgRec->diagnostics),
                sizeof(CDR3G_Diagnostics));

            if(m_pOrgRec->diagnostics.choice
                == CDR3G_networkSpecificCause_chosen)
            {
                CDR3G_ManagementExtension* q = NULL;
                q = Clone_CDR3G_ManagementExtension(
                    &(m_pOrgRec->diagnostics.u.networkSpecificCause),1);

                pDest->diagnostics.choice = CDR3G_networkSpecificCause_chosen;
                pDest->diagnostics.u.networkSpecificCause
                    = *(_3GCDR_ManagementExtension*)q;

                delete[] q;
            }
            else if(m_pOrgRec->diagnostics.choice
                == CDR3G_manufacturerSpecificCause_chosen)
            {
                CDR3G_ManagementExtension* q = NULL;
                q = Clone_CDR3G_ManagementExtension(
                    &(m_pOrgRec->diagnostics.u.manufacturerSpecificCause),1);

                pDest->diagnostics.choice = CDR3G_manufacturerSpecificCause_chosen;
                pDest->diagnostics.u.manufacturerSpecificCause
                    = *(_3GCDR_ManagementExtension*)q;

                delete[] q;
            }
        }

        /*
         * recordSequenceNumberת��Ϊ����
         */
        if(m_pOrgRec->bit_mask
            & CDR3G_SGSNPDPRecord_recordSequenceNumber_present)
        {
            pDest->recSequenceNumList.recSequenceNumber.value = new int[1];
            if(pDest->recSequenceNumList.recSequenceNumber.value == NULL)
            {
                throw "Alloc memory fail.";
            }

            pDest->recSequenceNumList.recSequenceNumber.value[0]
                = m_pOrgRec->recordSequenceNumber;
            pDest->recSequenceNumList.recSequenceNumber.count = 1;
            memcpy(&(pDest->recSequenceNumList.sgsnAddress),
                   &(m_pOrgRec->sgsnAddress),sizeof(CDR3G_GSNAddress));
            pDest->recSequenceNumList.bit_mask |= _3GCDR_recSequenceNumber_present;
        }

        //����nodeID�ֶ�
        if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_nodeID_present)
        {
            memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(CDR3G_NodeID));
        }

        //����recordExtensions�ֶ�
        if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_recordExtensions_present)
        {
            if((m_pOrgRec->recordExtensions.value != NULL)
                && (m_pOrgRec->recordExtensions.count > 0))
            {
                pDest->recordExtensions.value = (_3GCDR_ManagementExtension*)
                    Clone_CDR3G_ManagementExtension(
                    m_pOrgRec->recordExtensions.value,
                    m_pOrgRec->recordExtensions.count);

                pDest->recordExtensions.count = m_pOrgRec->recordExtensions.count;
            }
        }

        /*
         * localSequenceNumber�ֶα�����
         */

        pBegin = (char*)(&(m_pOrgRec->apnSelectionMode));
        pEnd = (char*)(&(m_pOrgRec->rNCUnsentDownlinkVolume));

        /*
         * apnSelectionMode,accessPggsnPDPRecordointNameOI,servedMSISDN,
         * chargingCharacteristics,systemType, cAMELInformationPDP,
		 * rNCUnsentDownlinkVolume
         */
		//ע:����CAMEL��������޸�zouyongsheng
		memcpy(&(pDest->apnSelectionMode),pBegin,
            (pEnd - pBegin) + sizeof(CDR3G_DataVolumeGPRS));
        
        //����causeForRecClosing�ֶΡ�
		pDest->causeForRecClosing = m_pOrgRec->causeForRecClosing;
       	//consolidationResult�ֶ�
		switch(m_pOrgRec->causeForRecClosing)
		{
		case CDR3G_normalRelease:
			//PDP��������
			pDest->consolidationResult
				= _3GCDR_consolidationResult_normal;
			break;
			//SGSN�ı�
		case CDR3G_sGSNChange:
			pDest->consolidationResult
				= _3GCDR_consolidationResult_ForInterSGSNConsolidation;
			break;
		case CDR3G_abnormalRelease:
			//PDP�쳣����
			pDest->consolidationResult
				= _3GCDR_consolidationResult_notnormal;
			break;
		default:
			//PDPδ����(��ΪPDP�쳣����)
			if (m_bIsLimit)
			{
        		pDest->consolidationResult = _3GCDR_consolidationResult_ReachLimit;
			}
			else
			{
        		pDest->consolidationResult = _3GCDR_consolidationResult_notnormal;
			}
			break;
		}
    
		if (!IsPartial())
		{
    		pDest->consolidationResult = _3GCDR_consolidationResult_onlyOneCDRGenerated;	
		}

        return (char*)m_pDestRec;
    }

    /*
     * �����Ǻϲ��󻰵�����Ϲ��̡�
     */

    pBegin = (char*)(&(m_pOrgRec->recordType));
    pEnd = (char*)(&(m_pOrgRec->listOfTrafficVolumes));

    /*
     * recordType,networkInitiation,servedIMSI,
     * servedIMEI,sgsnAddress,msNetworkCapability,
     * routingArea,locationAreaCode,cellIdentifier,
     * chargingID,ggsnAddressUsed,accessPointNameNI,
     * pdpType,servedPDPAddress
     */
    memcpy(&(pDest->recordType),pBegin,(pEnd - pBegin));

    pBegin = (char*)(&(m_pOrgRec->recordOpeningTime));
    pEnd = (char*)(&(m_pOrgRec->diagnostics));

    unsigned int nCount = 0;

    //�ϲ�listOfTrafficVolumes�ֶ�
    if(m_pCondition != NULL)
    {
        pDest->listOfTrafficVolumes.value = (_3GCDR_ChangeOfCharCondition*)
            List_To_Array(m_pCondition,nCount/*in,out*/);
        
        pDest->listOfTrafficVolumes.count = nCount;

        //ɾ���ϲ�������ʹ�õ���ʱ����
        Clear_List(m_pCondition);
        m_pCondition = m_pConditionTail = NULL;
    }
    
    /* 
     * recordOpeningTime,duration,sgsnChange,
     * causeForRecClosing
     */
    memcpy(&(pDest->recordOpeningTime),pBegin,(pEnd - pBegin));

    //�ϲ�diagnostics�ֶ�
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_diagnostics_present)
    {
        memcpy(&(pDest->diagnostics),&(m_Diagnostics),
            sizeof(CDR3G_Diagnostics));
    }

    //�ϲ�recordSequenceNumber�ֶ�
    if(m_pOrgRec->bit_mask
        & CDR3G_SGSNPDPRecord_recordSequenceNumber_present)
    {
        memcpy(&(pDest->recSequenceNumList.sgsnAddress),
            &(m_pOrgRec->sgsnAddress),sizeof(CDR3G_GSNAddress));

        /*
         * m_pSequenceһ����ΪNULL��
         */
        pDest->recSequenceNumList.recSequenceNumber.value
            = List_To_Array(m_pSequence,nCount/*in,out*/);

        pDest->recSequenceNumList.recSequenceNumber.count = nCount;

        pDest->recSequenceNumList.bit_mask 
            |= _3GCDR_recSequenceNumber_present;

        Clear_List(m_pSequence);
        m_pSequence = m_pSequenceTail = NULL;
    }

    //nodeID�ֶ�(һ������)
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_nodeID_present)
    {
        memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(_3GCDR_NodeID));
    }

    //�ϲ�recordExtensions�ֶ�
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_recordExtensions_present)
    {
        if(m_pExtension != NULL)
        {
            pDest->recordExtensions.value = (_3GCDR_ManagementExtension*)
                List_To_Array(m_pExtension,nCount/*in,out*/);
            
            pDest->recordExtensions.count = nCount;
            
            Clear_List(m_pExtension);
            m_pExtension = m_pExtensionTail = NULL;
        }
    }

    pBegin = (char*)(&(m_pOrgRec->apnSelectionMode));
    pEnd = (char*)(&(m_pOrgRec->rNCUnsentDownlinkVolume));

    /*
     * apnSelectionMode,accessPointNameOI,
     * servedMSISDN,chargingCharacteristics,
     * systemType,cAMELInformationPDP;
	 * rNCUnsentDownlinkVolume
     */
    //ע��zouyongsheng ����CAMEL����ڴ˲�����������޸Ĵ���
	memcpy(&(pDest->apnSelectionMode),pBegin,
        (pEnd - pBegin) + sizeof(CDR3G_DataVolumeGPRS));

	//consolidationResult�ֶ�
    switch(m_CauseForRecClosing)
    {
    case CDR3G_normalRelease:
		//PDP��������
        pDest->consolidationResult
            = _3GCDR_consolidationResult_normal;
        break;
		//SGSN�ı�
	case CDR3G_sGSNChange:
		pDest->consolidationResult
            = _3GCDR_consolidationResult_ForInterSGSNConsolidation;
        break;        
    case CDR3G_abnormalRelease:
        //PDP�쳣����
        pDest->consolidationResult
            = _3GCDR_consolidationResult_notnormal;
        break;
    default:
        //PDPδ����(��ΪPDP�쳣����)
        if (m_bIsLimit)
        {
        	pDest->consolidationResult = _3GCDR_consolidationResult_ReachLimit;
        }
        else
        {
        	pDest->consolidationResult = _3GCDR_consolidationResult_notnormal;
        }
        break;
    }
    
    if (!IsPartial())
    {
    	pDest->consolidationResult = _3GCDR_consolidationResult_onlyOneCDRGenerated;	
    }
	
    pDest->duration = m_Duration;
    pDest->rNCUnsentDownlinkVolume = m_UnsentVolume;
    pDest->causeForRecClosing = m_CauseForRecClosing;
	
	//added by zouyongsheng 2002-08-07
	memcpy(&(pDest->cAMELInformationPDP), 
		&(m_CAMELInformationPDP), sizeof(_3GCDR_CAMELInformationPDP));
    
	return (char*)m_pDestRec;
}

UINT4 C3G_350_SCDR::GetBillType()
{
    return m_pOrgRec->recordType;
}

CKey C3G_350_SCDR::CreateKey()
{
    C3G_350_SCDR_Key* pkey = new C3G_350_SCDR_Key;
    if(pkey == NULL)
    {
        throw "Alloc memory fail.";
    }
    pkey->chargingID         = m_pOrgRec->chargingID;
    pkey->ggsnAddressUsed    = m_pOrgRec->ggsnAddressUsed;
    pkey->sgsnAddress        = m_pOrgRec->sgsnAddress;

    CKey key;
    key.m_pDelegation = pkey;

    return key;
}


/******************************************************
* ������:   C3G_350_SCDR::PrepareData
* ����:    Zhou Yonghui
* ����:    01-6-5
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
*01-6-5        Zhou Yonghui
*02-8-7        Zou Yonsheng     ���Ӷ���CAMEL��Ĵ���
******************************************************/
BOOL C3G_350_SCDR::PrepareData()
{
    //����listOfTrafficVolumes
    if((m_pOrgRec->listOfTrafficVolumes.value != NULL)
        && (m_pOrgRec->listOfTrafficVolumes.count > 0))
    {
        m_pCondition = ChangeOfCharCondition_LINK_STRUCT::Alloc();

        m_pCondition->Value = Clone_CDR3G_ChangeOfCharCondition(
            m_pOrgRec->listOfTrafficVolumes.value,
            m_pOrgRec->listOfTrafficVolumes.count);

        m_pCondition->nCount = m_pOrgRec->listOfTrafficVolumes.count;
        m_pConditionTail = m_pCondition;
    }

    //����recordExtensions
    if(m_pOrgRec->bit_mask 
        & CDR3G_SGSNPDPRecord_recordExtensions_present)
    {
        if((m_pOrgRec->recordExtensions.value != NULL)
            && (m_pOrgRec->recordExtensions.count > 0))
        {
            m_pExtension = ManagementExtension_LINK_STRUCT::Alloc();

            m_pExtension->Value = Clone_CDR3G_ManagementExtension(
                m_pOrgRec->recordExtensions.value,
                m_pOrgRec->recordExtensions.count);

            m_pExtension->nCount = m_pOrgRec->recordExtensions.count;
            m_pExtensionTail = m_pExtension;
        }
    }

    //����recordSequenceNumber
    if(m_pOrgRec->bit_mask 
        & CDR3G_SGSNPDPRecord_recordSequenceNumber_present)
    {
        m_pSequence = RecordSequenceNumber_LINK_STRUCT::Alloc();

        m_pSequence->Value = m_pOrgRec->recordSequenceNumber;
        m_pSequenceTail = m_pSequence;
    }

    //����duration�ֶ�
    m_Duration = m_pOrgRec->duration;

    //added by zouyongsheng 
	//����cAMELInformationPDP
	if(m_pOrgRec->bit_mask
		& CDR3G_cAMELInformationPDP_present)
	{
		m_CAMELInformationPDP = m_pOrgRec->cAMELInformationPDP;
	}

	//����rNCUnsentDownlinkVolume�ֶ�
    if(m_pOrgRec->bit_mask
        & CDR3G_rNCUnsentDownlinkVolume_present)
    {
        m_UnsentVolume = m_pOrgRec->rNCUnsentDownlinkVolume;
    }

    //����causeForRecClosing�ֶ�
    m_CauseForRecClosing = m_pOrgRec->causeForRecClosing;

    //����diagnostics�ֶ�
    if(m_pOrgRec->bit_mask
        & CDR3G_SGSNPDPRecord_diagnostics_present)
    {
        m_Diagnostics = m_pOrgRec->diagnostics;
        
        if(m_Diagnostics.choice == CDR3G_networkSpecificCause_chosen)
        {
            CDR3G_ManagementExtension* q = NULL;
            q = Clone_CDR3G_ManagementExtension(
                &(m_pOrgRec->diagnostics.u.networkSpecificCause),1);

            m_Diagnostics.choice = CDR3G_networkSpecificCause_chosen;
            m_Diagnostics.u.networkSpecificCause = *q;
            
            delete[] q;
        }
        else if(m_Diagnostics.choice 
            == CDR3G_manufacturerSpecificCause_chosen)
        {
            CDR3G_ManagementExtension* q = NULL;
            q = Clone_CDR3G_ManagementExtension(
                &(m_pOrgRec->diagnostics.u.manufacturerSpecificCause),1);

            m_Diagnostics.choice = CDR3G_manufacturerSpecificCause_chosen;
            m_Diagnostics.u.manufacturerSpecificCause = *q;
            
            delete[] q;
        }
    }

    //׼�����
    m_bPrepared = TRUE;
    return TRUE;
}

BOOL C3G_350_SCDR::IsDuplicated(CBill* pBill)
{
    BOOL bSucc = FALSE;
    C3G_350_SCDR* p = (C3G_350_SCDR*)pBill;
    if((p->m_pOrgRec->bit_mask 
        & CDR3G_SGSNPDPRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask 
        & CDR3G_SGSNPDPRecord_recordSequenceNumber_present))
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
* ������:   C3G_350_SCDR::Merge
* ����:    Zhou Yonghui
* ����:    01-5-30
* ����:    �ѻ���pBill���뵱ǰ�����С�
* ����: 
*       ����1 :CBill* pBill�����ϲ��Ļ�����
* ����ֵ: BOOL���ϲ��ɹ�����TRUE��ʧ�ܷ���FALSE��
* ˵��: 
*       �ϲ������и��µ��ֶ���:
*       ��Ҫ���ӵ��ֶ�(listOfTrafficVolumes,recordSequenceNumber,
*       recordExtensions),ȡ���һ�Ż���ֵ���ֶ�(causeForRecClosing,
*       diagnostics),��Ҫ�ۼӵ��ֶ�(Duration,rNCUnsentDownlinkVolume)��
*       ȡ��һ�Ż���ֵ���ֶβ��䡣
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-5-30       Zhou Yonghui
*01-9-25       Zhou Yonghui
*              ������ͳ��listOfTrafficVolumes��recordExtensions�Ĵ��룬
*              �Ա�����������ơ�
*02-8-07       Zou Yongsheng     ���Ӷ���CAMEL��Ĵ���
******************************************************/
BOOL C3G_350_SCDR::Merge(CBill* pBill)
{
    assert(pBill != NULL);

    if(!m_bPrepared)
    {
        if(!PrepareData())  return FALSE;
    }

    C3G_350_SCDR* p = (C3G_350_SCDR*)pBill;

    //�����ظ�������
    if((p->m_pOrgRec->bit_mask 
        & CDR3G_SGSNPDPRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask 
        & CDR3G_SGSNPDPRecord_recordSequenceNumber_present))
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
    case CDR3G_normalRelease:
    case CDR3G_abnormalRelease:
    case CDR3G_sGSNChange:
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
		int nDiff = Get_Duration(&(m_pOrgRec->recordOpeningTime),
			&(p->m_pOrgRec->recordOpeningTime));
		m_Duration = nDiff;
	}
	//�ۼ�Duration�ֶΡ�
	m_Duration += p->m_pOrgRec->duration;
	//�޸Ľ���

    //Added by ZhengYuqun 2003-11-04 SWPD02238
    if((UINT4)m_Duration >= m_uMaxDuration)
    {
        m_bPDPCompleted = TRUE;
    }
	
    //�ۼ�rNCUnsentDownlinkVolume�ֶΡ�
    if(p->m_pOrgRec->bit_mask & CDR3G_rNCUnsentDownlinkVolume_present)
    {
        //Added by ZhengYuqun 2003-11-03 SWPD02228
        if(m_UnsentVolume == 0)  //����һ��NCUnsentDownlinkVolum��Ϊ�յ�ʱ����λ
        {
            m_pOrgRec->bit_mask |= CDR3G_rNCUnsentDownlinkVolume_present;
        }
        //End SWPD02228
        m_UnsentVolume += p->m_pOrgRec->rNCUnsentDownlinkVolume;
    }

    //����listOfTrafficVolumes�ֶΡ�
    if((p->m_pOrgRec->listOfTrafficVolumes.value != NULL)
        && (p->m_pOrgRec->listOfTrafficVolumes.count > 0))
    {
        ChangeOfCharCondition_LINK_STRUCT* q 
            = ChangeOfCharCondition_LINK_STRUCT::Alloc();

        q->Value = Clone_CDR3G_ChangeOfCharCondition(
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
         * Added by zhouyonghui,2001/09/25.
         */
        m_nLen_Condition_List += q->nCount;
    }

    /*
    //����recordExtensions�ֶΡ�
    if((p->m_pOrgRec->bit_mask
        & CDR3G_SGSNPDPRecord_recordExtensions_present)
        && (m_pOrgRec->bit_mask
        & CDR3G_SGSNPDPRecord_recordExtensions_present))
    {
        if((p->m_pOrgRec->recordExtensions.value != NULL)
            && (p->m_pOrgRec->recordExtensions.count > 0))
        {
            ManagementExtension_LINK_STRUCT* q
                = ManagementExtension_LINK_STRUCT::Alloc();
            
            q->Value = Clone_CDR3G_ManagementExtension(
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
             * Added by zhouyonghui,2001/09/25.
             
            m_nLen_Extension_List += q->nCount;
        }
    }
    */


    //����recordSequenceNumber�ֶΡ�
    if((p->m_pOrgRec->bit_mask 
        & CDR3G_SGSNPDPRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask 
        & CDR3G_SGSNPDPRecord_recordSequenceNumber_present))
    {
        RecordSequenceNumber_LINK_STRUCT* q
            = RecordSequenceNumber_LINK_STRUCT::Alloc();
        
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

    //diagnostics�ֶ�ȡ���һ�Ż�����ֵ��
    if((p->m_pOrgRec->bit_mask 
        & CDR3G_SGSNPDPRecord_diagnostics_present))
        //&& (m_pOrgRec->bit_mask          //������ȥ�� ZhengYuqun 2003-11-03 SWPD02228
        //& CDR3G_SGSNPDPRecord_diagnostics_present))
    {
        CDR3G_ManagementExtension* q = NULL;
        if(p->m_pOrgRec->diagnostics.choice
            == CDR3G_networkSpecificCause_chosen)
        {
            q = Clone_CDR3G_ManagementExtension(
                &(p->m_pOrgRec->diagnostics.u.networkSpecificCause),1);
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CDR3G_manufacturerSpecificCause_chosen)
        {
            q = Clone_CDR3G_ManagementExtension(
                &(p->m_pOrgRec->diagnostics.u.manufacturerSpecificCause),1);
        }

        if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_diagnostics_present) //����һ��if ZhengYuqun 2003-11-03 SWPD02228
        {
            /*����m_Diagnostics�Ŀռ䡣*/
            if(m_Diagnostics.choice == CDR3G_manufacturerSpecificCause_chosen)
            {
                delete[] m_Diagnostics.u.manufacturerSpecificCause.identifier.value;
                delete[] m_Diagnostics.u.manufacturerSpecificCause.information.value;
            }
            else if(m_Diagnostics.choice == CDR3G_networkSpecificCause_chosen)
            {
                delete[] m_Diagnostics.u.networkSpecificCause.identifier.value;
                delete[] m_Diagnostics.u.networkSpecificCause.information.value;
            }
        }
        else
        {
            m_pOrgRec->bit_mask |= CDR3G_SGSNPDPRecord_diagnostics_present; //����һ�� ZhengYuqun 2003-11-03 SWPD02228
        }
        /************************/

        if(p->m_pOrgRec->diagnostics.choice
            == CDR3G_networkSpecificCause_chosen)
        {
            m_Diagnostics.choice = CDR3G_networkSpecificCause_chosen;
            
            m_Diagnostics.u.networkSpecificCause = *q;
            delete[] q;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CDR3G_manufacturerSpecificCause_chosen)
        {
            m_Diagnostics.choice = CDR3G_manufacturerSpecificCause_chosen;
            
            m_Diagnostics.u.manufacturerSpecificCause = *q;
            delete[] q;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CDR3G_gsm0408Cause_chosen)
        {
            m_Diagnostics.choice = CDR3G_gsm0408Cause_chosen;
            m_Diagnostics.u.gsm0408Cause = 
                p->m_pOrgRec->diagnostics.u.gsm0408Cause;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CDR3G_gsm0902MapErrorValue_chosen)
        {
            m_Diagnostics.choice = CDR3G_gsm0902MapErrorValue_chosen;
            m_Diagnostics.u.gsm0902MapErrorValue = 
                p->m_pOrgRec->diagnostics.u.gsm0902MapErrorValue;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CDR3G_ccittQ767Cause_chosen)
        {
            m_Diagnostics.choice = CDR3G_ccittQ767Cause_chosen;
            m_Diagnostics.u.ccittQ767Cause = 
                p->m_pOrgRec->diagnostics.u.ccittQ767Cause;
        }
    }

	//added by zouyongsheng �ϲ�CAMELInformation�����Ϣ
    if((p->m_pOrgRec->bit_mask 
		& CDR3G_cAMELInformationPDP_present)
		&& (m_pOrgRec->bit_mask
		& CDR3G_cAMELInformationPDP_present))
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
			& CDR3G_CAMELInformationPDP_fFDAppendIndicator_present)
			|| ((p->m_pOrgRec->cAMELInformationPDP.bit_mask 
			& CDR3G_CAMELInformationPDP_fFDAppendIndicator_present)
            &&  (p->m_pOrgRec->cAMELInformationPDP.fFDAppendIndicator == FALSE))) 
			&& (p->m_pOrgRec->cAMELInformationPDP.bit_mask
			& CDR3G_CAMELInformationPDP_freeFormatData_present)
			&&(p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length != 0))
		{
			memcpy(m_CAMELInformationPDP.freeFormatData.value,
				p->m_pOrgRec->cAMELInformationPDP.freeFormatData.value,
				p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length);
			
			m_CAMELInformationPDP.freeFormatData.length =
				p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length;

			m_CAMELInformationPDP.bit_mask |= 
				CDR3G_CAMELInformationPDP_freeFormatData_present;
		}
		
		//fFDAppendIndicator �ֶδ�����ֵΪTRUE
		//freeFormatData���ڶ�����Ϣ���Ȳ�Ϊ��
		if((p->m_pOrgRec->cAMELInformationPDP.bit_mask
			& CDR3G_CAMELInformationPDP_fFDAppendIndicator_present)
			&& (p->m_pOrgRec->cAMELInformationPDP.fFDAppendIndicator == TRUE) 
			&&(p->m_pOrgRec->cAMELInformationPDP.bit_mask
			& CDR3G_CAMELInformationPDP_freeFormatData_present)
			&& (p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length != 0))
		{
			memcpy(m_CAMELInformationPDP.freeFormatData.value 
				+ m_CAMELInformationPDP.freeFormatData.length,
				p->m_pOrgRec->cAMELInformationPDP.freeFormatData.value,
				p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length);

			m_CAMELInformationPDP.freeFormatData.length +=
				p->m_pOrgRec->cAMELInformationPDP.freeFormatData.length;

			m_CAMELInformationPDP.bit_mask |= 
				CDR3G_CAMELInformationPDP_freeFormatData_present;
		}
		
	}
   
     /*
     * �����ֶ�ȡ��һ�Ż�����ֵ��
     */
    return TRUE;
}


//C3G_350_GCDR��ʵ��(S-CDR)
C3G_350_GCDR::C3G_350_GCDR()
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
    memset(&m_Diagnostics,0,sizeof(CDR3G_Diagnostics));
    m_CauseForRecClosing = CDR3G_normalRelease;

    m_pDestRec = NULL;
    m_pOrgRec = NULL;
    m_bIsLimit = FALSE;
}

C3G_350_GCDR::~C3G_350_GCDR()
{
    //����ԭʼ������¼�ռ䡣
    if(m_pOrgRec != NULL)
    {
        delete[] m_pOrgRec->sgsnAddress.value;

        delete[] m_pOrgRec->listOfTrafficVolumes.value;

        if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_diagnostics_present)
        {
            Clear_Diagnostics(&(m_pOrgRec->diagnostics));
        }

        if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_recordExtensions_present)
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
            & _3GCDR_GGSNPDPRecord_diagnostics_present)
        {
            Clear_Diagnostics((CDR3G_Diagnostics*)
                &(m_pDestRec->u.ggsnPDPRecord.diagnostics));
        }

        if(m_pDestRec->u.ggsnPDPRecord.bit_mask
            & _3GCDR_GGSNPDPRecord_recordExtensions_present)
        {
            if((m_pDestRec->u.ggsnPDPRecord.recordExtensions.value != NULL)
                && (m_pDestRec->u.ggsnPDPRecord.recordExtensions.count > 0))
            {
                Clear_ManagementExtension((CDR3G_ManagementExtension*)
                    m_pDestRec->u.ggsnPDPRecord.recordExtensions.value,
                    m_pDestRec->u.ggsnPDPRecord.recordExtensions.count);
            }
        }

        if(m_pDestRec->u.ggsnPDPRecord.bit_mask
            & _3GCDR_GGSNPDPRecord_recSequenceNumList_present)
        {
            delete[] m_pDestRec->u.ggsnPDPRecord.recSequenceNumList.
                recSequenceNumber.value;
        }

        delete m_pDestRec;
        m_pDestRec = NULL;
    }

    /*
     * �ϲ�������ʹ�õ����ݽṹ�Ŀռ��Ѿ���GetContents�л��ա�
     */
}

BOOL C3G_350_GCDR::PrepareData()
{
    //ת��sgsnAddress
    if((m_pOrgRec->sgsnAddress.value != NULL)
        && (m_pOrgRec->sgsnAddress.count > 0))
    {
        m_pSGSNAddress = SGSGAddress_LINK_STRUCT::Alloc();

        m_pSGSNAddress->Value = Clone_CDR3G_GSNAddress(
            m_pOrgRec->sgsnAddress.value,
            m_pOrgRec->sgsnAddress.count);

        m_pSGSNAddress->nCount = m_pOrgRec->sgsnAddress.count;
        m_pSGSNAddressTail = m_pSGSNAddress;
    }

    //����listOfTrafficVolumes
    if((m_pOrgRec->listOfTrafficVolumes.value != NULL)
        && (m_pOrgRec->listOfTrafficVolumes.count > 0))
    {
        m_pCondition =  ChangeOfCharCondition_LINK_STRUCT::Alloc();

        m_pCondition->Value = Clone_CDR3G_ChangeOfCharCondition(
            m_pOrgRec->listOfTrafficVolumes.value,
            m_pOrgRec->listOfTrafficVolumes.count);

        m_pCondition->nCount = m_pOrgRec->listOfTrafficVolumes.count;
        m_pConditionTail = m_pCondition;
    }

    //����recordExtensions
    if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_recordExtensions_present)
    {
        if((m_pOrgRec->recordExtensions.value != NULL)
            && (m_pOrgRec->recordExtensions.count > 0))
        {
            m_pExtension = ManagementExtension_LINK_STRUCT::Alloc();
            
            m_pExtension->Value = Clone_CDR3G_ManagementExtension(
                m_pOrgRec->recordExtensions.value,
                m_pOrgRec->recordExtensions.count);
            
            m_pExtension->nCount = m_pOrgRec->recordExtensions.count;
            m_pExtensionTail = m_pExtension;
        }
    }

    //����recordSequenceNumber
    if(m_pOrgRec->bit_mask 
        & CDR3G_GGSNPDPRecord_recordSequenceNumber_present)
    {
        m_pSequence = RecordSequenceNumber_LINK_STRUCT::Alloc();

        m_pSequence->Value = m_pOrgRec->recordSequenceNumber;
        m_pSequenceTail = m_pSequence;
    }

    //����duration�ֶ�
    m_Duration = m_pOrgRec->duration;

    //����causeForRecClosing�ֶ�
    m_CauseForRecClosing = m_pOrgRec->causeForRecClosing;

    //����diagnostics�ֶ�
    if(m_pOrgRec->bit_mask
        & CDR3G_GGSNPDPRecord_diagnostics_present)
    {
        m_Diagnostics = m_pOrgRec->diagnostics;
        
        if(m_Diagnostics.choice == CDR3G_networkSpecificCause_chosen)
        {
            CDR3G_ManagementExtension* q = NULL;
            q = Clone_CDR3G_ManagementExtension(
                &(m_pOrgRec->diagnostics.u.networkSpecificCause),1);

            m_Diagnostics.choice = CDR3G_networkSpecificCause_chosen;
            m_Diagnostics.u.networkSpecificCause = *q;
            
            delete[] q;
        }
        else if(m_Diagnostics.choice 
            == CDR3G_manufacturerSpecificCause_chosen)
        {
            CDR3G_ManagementExtension* q = NULL;
            q = Clone_CDR3G_ManagementExtension(
                &(m_pOrgRec->diagnostics.u.manufacturerSpecificCause),1);

            m_Diagnostics.choice = CDR3G_manufacturerSpecificCause_chosen;
            m_Diagnostics.u.manufacturerSpecificCause = *q;
            
            delete[] q;
        }
    }

    //׼�����
    m_bPrepared = TRUE;
    return TRUE;
}


/******************************************************
* ������:   C3G_350_GCDR::Init
* ����:    Zhou Yonghui
* ����:    01-5-28
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
*01-5-28       Zhou Yonghui
*01-9-25       Zhou Yonghui
*              �����˳�ʼ��m_nLen_SGSNAddress_List��m_nLen_Condition_List��
*              m_nLen_Extension_List�Ĵ��룬�Ա�����������ơ�
******************************************************/
BOOL C3G_350_GCDR::Init(char* pBuf,UINT4 uLen)
{
    assert(pBuf != NULL);

    //������¼�������ֶζ���������������ڴ����
    CDR3G_GGSNPDPRecord* p = (CDR3G_GGSNPDPRecord*)pBuf;

    //����ԭʼ����
    m_pOrgRec = new CDR3G_GGSNPDPRecord;
    if(m_pOrgRec == NULL)
    {
        throw "Alloc memory fail.";
    }

    memcpy(m_pOrgRec,p,sizeof(CDR3G_GGSNPDPRecord));

    /*
     * ���浥������ָ���ֶ�
     */

    //����sgsnAddress
    if((p->sgsnAddress.value != NULL) && (p->sgsnAddress.count > 0))
    {
        m_pOrgRec->sgsnAddress.value = Clone_CDR3G_GSNAddress(
            p->sgsnAddress.value,
            p->sgsnAddress.count);
        
        m_pOrgRec->sgsnAddress.count = p->sgsnAddress.count;

        /* 
         * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
         * Added by zhouyonghui,2001/09/25.
         */
        m_nLen_SGSNAddress_List = p->sgsnAddress.count;
    }

    //����listOfTrafficVolumes�ֶ�
    if((p->listOfTrafficVolumes.value != NULL)
        && (p->listOfTrafficVolumes.count > 0))
    {
        m_pOrgRec->listOfTrafficVolumes.value
            = Clone_CDR3G_ChangeOfCharCondition(
            p->listOfTrafficVolumes.value,
            p->listOfTrafficVolumes.count);
        
        m_pOrgRec->listOfTrafficVolumes.count
            = p->listOfTrafficVolumes.count;

        /* 
         * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
         * Added by zhouyonghui,2001/09/25.
         */
        m_nLen_Condition_List = p->listOfTrafficVolumes.count;
    }

    //����diagnostics�ֶ�
    if(p->bit_mask & CDR3G_GGSNPDPRecord_diagnostics_present)
    {
        m_pOrgRec->diagnostics = p->diagnostics;
        
        if(p->diagnostics.choice == CDR3G_networkSpecificCause_chosen)
        {
            CDR3G_ManagementExtension* q = NULL;
            q = Clone_CDR3G_ManagementExtension(
                        &(p->diagnostics.u.networkSpecificCause),1);

            m_pOrgRec->diagnostics.choice
                = CDR3G_networkSpecificCause_chosen;
            m_pOrgRec->diagnostics.u.networkSpecificCause = *q;
            
            delete[] q;
        }
        else if(p->diagnostics.choice 
            == CDR3G_manufacturerSpecificCause_chosen)
        {
            CDR3G_ManagementExtension* q = NULL;
            q = Clone_CDR3G_ManagementExtension(
                    &(p->diagnostics.u.manufacturerSpecificCause),1);

            m_pOrgRec->diagnostics.choice
                = CDR3G_manufacturerSpecificCause_chosen;
            m_pOrgRec->diagnostics.u.manufacturerSpecificCause = *q;

            delete[] q;
        }
    }

    //����recordExtensions�ֶ�
    if(p->bit_mask & CDR3G_GGSNPDPRecord_recordExtensions_present)
    {
        if((p->recordExtensions.value != NULL)
            && (p->recordExtensions.count > 0))
        {
            m_pOrgRec->recordExtensions.value
                = Clone_CDR3G_ManagementExtension(
                p->recordExtensions.value,
                p->recordExtensions.count);

            m_pOrgRec->recordExtensions.count = p->recordExtensions.count;

            /* 
             * ͳ������ĳ��ȣ����ڿ��ƺϲ��Ļ�������
             * Added by zhouyonghui,2001/09/25.
             */
            m_nLen_Extension_List = p->recordExtensions.count;
        }
    }

    //���õ�ǰ�������к�
    m_nMinRecordSequence = m_nCurRecordSequence = p->recordSequenceNumber;

    //����PDP������־
    switch(p->causeForRecClosing)
    {
    case CDR3G_normalRelease:
    case CDR3G_abnormalRelease:
    case CDR3G_sGSNChange:
        m_bPDPCompleted = TRUE;
        break;
    default:
        break;
    }

    //Added by ZhengYuqun 2003-11-04 SWPD02238
    if((UINT4)p->duration >= m_uMaxDuration)
    {
        m_bPDPCompleted = TRUE;
    }

    return TRUE;
}

BOOL C3G_350_GCDR::IsFirstPartialRec()
{
    //return (m_pOrgRec->recordSequenceNumber == 1);
    //maimaoshi,2001.06.26
    return (m_pOrgRec->recordSequenceNumber == 0);
}

BOOL C3G_350_GCDR::IsLastPartialRec()
{
    switch(m_pOrgRec->causeForRecClosing)
    {
    case CDR3G_normalRelease:
    case CDR3G_abnormalRelease:
    case CDR3G_sGSNChange:
        return TRUE;
        break;
    default:
        break;
    }
    
    //Added by ZhengYuqun 2003-11-04 SWPD02238
    if((UINT4)m_pOrgRec->duration >= m_uMaxDuration)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL C3G_350_GCDR::IsPartial()
{
    /* 
     * ��recordSequenceNumber�ֶ���Ч���򻰵��ǲ��ֻ�����
     * Added by zhouyonghui,2001-07-03
     */
    return (m_pOrgRec->bit_mask &
        CDR3G_GGSNPDPRecord_recordSequenceNumber_present);
}

BOOL C3G_350_GCDR::IsHotBilling()
{
    if((m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_chargingCharacteristics_present)
        && ((m_pOrgRec->chargingCharacteristics.value[0] & 0x0f) == 1))
    {
        return TRUE;
    }
    return FALSE;
}


/******************************************************
* ������:   C3G_350_GCDR::CanSafeMerge
* ����:    Zhou Yonghui
* ����:    01-5-28
* ����:    
* ����: 
*       ����1 :CBill* pBill,���ϲ��Ļ�������ָ�롣
* ����ֵ: BOOL ,�����Ż�������ϲ�������TRUE�����򷵻�FALSE��
* ˵��: 
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-5-28       Zhou Yonghui
*01-9-25       Zhou Yonghui     �������������ƴ��룬
*                               ���󻰵�ǿ���жϺϲ���
******************************************************/
BOOL C3G_350_GCDR::CanSafeMerge(CBill* pBill)
{
    assert(pBill != NULL);

    C3G_350_GCDR* p = (C3G_350_GCDR*)pBill;

    //�����������к����Ѿ��ϲ��Ļ�����Χ�ڣ�˵�����ظ�������
    //�ظ�������Merge�����ж�����
    if((p->m_pOrgRec->bit_mask
        & CDR3G_GGSNPDPRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask
        & CDR3G_GGSNPDPRecord_recordSequenceNumber_present))
    {
        if((p->m_nCurRecordSequence <= m_nCurRecordSequence)
            && (p->m_nCurRecordSequence >= m_nMinRecordSequence))
        {
            return TRUE;
        }
    }

    /* 
     * ���󻰵���ǿ�����ߡ�
     * Added by zhouyonghui,2001/09/25.
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

BOOL C3G_350_GCDR::IsPDPCompleted()
{
    return m_bPDPCompleted;
}

BOOL C3G_350_GCDR::NeedWaiting()
{
    //��ǰ�ĺϲ�������ü�����ʽ�ϲ�Duration�ֶ�
    //���Ըú���ֻ����FALSE(��PDP����ʱ��������ȴ�)��
    BOOL bTmp = FALSE;
    return bTmp;
}

char* C3G_350_GCDR::GetContents()
{
    if(m_pDestRec != NULL)
    {
        return (char*)m_pDestRec;
    }

    _3GCDR_CallEventRecord* pCallEventRec = new _3GCDR_CallEventRecord;
    if(pCallEventRec == NULL)
    {
        throw "Alloc memory fail.";
    }

    pCallEventRec->choice = _3GCDR_ggsnPDPRecord_chosen;
    _3GCDR_GGSNPDPRecord* pDest = &(pCallEventRec->u.ggsnPDPRecord);

    memset(pDest,0,sizeof(_3GCDR_GGSNPDPRecord));
    m_pDestRec = pCallEventRec;

    //�������ڴ�鿪ʼ/������ַ��
    char* pBegin = NULL,*pEnd = NULL;

    /*
     * ������û�о����ϲ������ԭʼ�����й������ջ�����
     */

    //�ϲ�ǰbit_mask��4���ֽڳ����ϲ�����2���ֽڳ�������ֱ�Ӹ�ֵ��
    if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_networkInitiation_present)
    {
        pDest->bit_mask |= _3GCDR_GGSNPDPRecord_networkInitiation_present;
    }
	//����Զ2002.05.27��ӣ����ⵥ��D13999
	if (m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_servedPDPAddress_present)
	{
		pDest->bit_mask |= _3GCDR_GGSNPDPRecord_servedPDPAddress_present;
	}
	//����Զ2002.05.27��ӽ���
    if(m_pOrgRec->bit_mask & CDR3G_dynamicAddressFlag_present)
    {
        pDest->bit_mask |= _3GCDR_dynamicAddressFlag_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_diagnostics_present)
    {
        pDest->bit_mask |= _3GCDR_GGSNPDPRecord_diagnostics_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_recordSequenceNumber_present)
    {
        pDest->bit_mask |= _3GCDR_GGSNPDPRecord_recSequenceNumList_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_nodeID_present)
    {
        pDest->bit_mask |= _3GCDR_GGSNPDPRecord_nodeID_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_recordExtensions_present)
    {
        pDest->bit_mask |= _3GCDR_GGSNPDPRecord_recordExtensions_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_apnSelectionMode_present)
    {
        pDest->bit_mask |= _3GCDR_GGSNPDPRecord_apnSelectionMode_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_servedMSISDN_present)
    {
        pDest->bit_mask |= _3GCDR_GGSNPDPRecord_servedMSISDN_present;
    }
    if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_chargingCharacteristics_present)
    {
        pDest->bit_mask |= _3GCDR_GGSNPDPRecord_chargingCharacteristics_present;
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
            pDest->sgsnAddress.value = (_3GCDR_GSNAddress*)
                Clone_CDR3G_GSNAddress(m_pOrgRec->sgsnAddress.value,
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
            pDest->listOfTrafficVolumes.value = (_3GCDR_ChangeOfCharCondition*)
                Clone_CDR3G_ChangeOfCharCondition(
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
        if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_diagnostics_present)
        {
            memcpy(&(pDest->diagnostics),&(m_pOrgRec->diagnostics),
                sizeof(CDR3G_Diagnostics));

            if(m_pOrgRec->diagnostics.choice
                == CDR3G_networkSpecificCause_chosen)
            {
                CDR3G_ManagementExtension* q = NULL;
                q = Clone_CDR3G_ManagementExtension(
                    &(m_pOrgRec->diagnostics.u.networkSpecificCause),1);

                pDest->diagnostics.choice = CDR3G_networkSpecificCause_chosen;
                pDest->diagnostics.u.networkSpecificCause
                    = *(_3GCDR_ManagementExtension*)q;

                delete[] q;
            }
            else if(m_pOrgRec->diagnostics.choice 
                == CDR3G_manufacturerSpecificCause_chosen)
            {
                CDR3G_ManagementExtension* q = NULL;
                q = Clone_CDR3G_ManagementExtension(
                    &(m_pOrgRec->diagnostics.u.manufacturerSpecificCause),1);

                pDest->diagnostics.choice = CDR3G_manufacturerSpecificCause_chosen;
                pDest->diagnostics.u.manufacturerSpecificCause
                    = *(_3GCDR_ManagementExtension*)q;
                
                delete[] q;
            }
        }

        /*
         * recordSequenceNumberת��Ϊ����
         */
        if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_recordSequenceNumber_present)
        {
            pDest->recSequenceNumList.recSequenceNumber.value = new int[1];
            if(pDest->recSequenceNumList.recSequenceNumber.value == NULL)
            {
                throw "Alloc memory fail.";
            }
            
            pDest->recSequenceNumList.recSequenceNumber.value[0]
                = m_pOrgRec->recordSequenceNumber;
            pDest->recSequenceNumList.recSequenceNumber.count = 1;
            pDest->recSequenceNumList.bit_mask |= _3GCDR_recSequenceNumber_present;
            
            if(m_pOrgRec->sgsnAddress.count > 0)
            {
                memcpy(&(pDest->recSequenceNumList.sgsnAddress),
                    &(m_pOrgRec->sgsnAddress.value[0]),sizeof(CDR3G_GSNAddress));
            }
            else
            {
                //��SGSN��ַ��Ч���������е�SGSN��ַ����Ϊ��
                memset(&(pDest->recSequenceNumList.sgsnAddress),
                    0,sizeof(CDR3G_GSNAddress));
            }
        }

        //����nodeID�ֶ�(һ������)
        if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_nodeID_present)
        {
            memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(CDR3G_NodeID));
        }

        //����recordExtensions�ֶ�
        if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_recordExtensions_present)
        {
            if((m_pOrgRec->recordExtensions.value != NULL)
                && (m_pOrgRec->recordExtensions.count > 0))
            {
                pDest->recordExtensions.value = (_3GCDR_ManagementExtension*)
                    Clone_CDR3G_ManagementExtension(
                    m_pOrgRec->recordExtensions.value,
                    m_pOrgRec->recordExtensions.count);
                
                pDest->recordExtensions.count = m_pOrgRec->recordExtensions.count;
            }
        }

        /*
         * localSequenceNumber�ֶα�����
         */

        pBegin = (char*)(&(m_pOrgRec->apnSelectionMode));
        pEnd = (char*)(&(m_pOrgRec->chargingCharacteristics));
        /* 
         * apnSelectionMode,servedMSISDN,chargingCharacteristics
         */
        memcpy(&(pDest->apnSelectionMode),pBegin,
            (pEnd - pBegin) + sizeof(CDR3G_ChargingCharacteristics));
        
		//����causeForRecClosing�ֶΡ�
		pDest->causeForRecClosing = m_pOrgRec->causeForRecClosing;
        
		//consolidationResult�ֶ�
		switch(m_pOrgRec->causeForRecClosing)
		{
		case CDR3G_normalRelease:
			 //PDP��������
			pDest->consolidationResult
				= _3GCDR_consolidationResult_normal;
			break;
			//SGSN�ı�
		case CDR3G_sGSNChange:
			pDest->consolidationResult
				= _3GCDR_consolidationResult_ForInterSGSNConsolidation;
			break;     
		case CDR3G_abnormalRelease:
			//PDP�쳣����
			pDest->consolidationResult
				= _3GCDR_consolidationResult_notnormal;
			break;
		default:
			//PDPδ����(��ΪPDP�쳣����)
			if (m_bIsLimit)
			{
        		pDest->consolidationResult = _3GCDR_consolidationResult_ReachLimit;
			}
			else
			{
        		pDest->consolidationResult = _3GCDR_consolidationResult_notnormal;
			}
			break;
		}
    
		if (!IsPartial())
		{
    		pDest->consolidationResult = _3GCDR_consolidationResult_onlyOneCDRGenerated;	
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
    
    unsigned int nCount = 0;
    //�ϲ�sgsnAddress�ֶ�
    if(m_pSGSNAddress != NULL)
    {
        pDest->sgsnAddress.value = (_3GCDR_GSNAddress*)
            List_To_Array(m_pSGSNAddress,nCount/*in,out*/);

        pDest->sgsnAddress.count = nCount;

        Clear_List(m_pSGSNAddress);
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
        pDest->listOfTrafficVolumes.value = (_3GCDR_ChangeOfCharCondition*)
            List_To_Array(m_pCondition,nCount/*in,out*/);
        
        pDest->listOfTrafficVolumes.count = nCount;

        Clear_List(m_pCondition);
        m_pCondition = m_pConditionTail = NULL;
    }

    pBegin = (char*)(&(m_pOrgRec->recordOpeningTime));
    pEnd = (char*)(&(m_pOrgRec->diagnostics));
    /* 
     * recordOpeningTime,duration,causeForRecClosing
     */
    memcpy(&(pDest->recordOpeningTime),pBegin,(pEnd - pBegin));

    //�ϲ�diagnostics�ֶ�
    if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_diagnostics_present)
    {
        memcpy(&(pDest->diagnostics),&(m_Diagnostics),
            sizeof(CDR3G_Diagnostics));
        /*
         * m_Diagnosticsά����ָ��ת����pDest��
         */
    }

    //�ϲ�recordSequenceNumber�ֶ�
    if(m_pOrgRec->bit_mask
        & CDR3G_GGSNPDPRecord_recordSequenceNumber_present)
    {
        if(m_pOrgRec->sgsnAddress.count > 0)
        {
            memcpy(&(pDest->recSequenceNumList.sgsnAddress),
                m_pOrgRec->sgsnAddress.value,
                sizeof(CDR3G_GSNAddress));
        }
        else
        {
            //��SGSN��ַ��Ч��
            memset(&(pDest->recSequenceNumList.sgsnAddress),
                0,sizeof(CDR3G_GSNAddress));
        }

        /*
         * m_pSequence�϶���Ϊ�ա�
         */
        pDest->recSequenceNumList.recSequenceNumber.value = 
            List_To_Array(m_pSequence,nCount/*in,out*/);

        pDest->recSequenceNumList.bit_mask
            |= _3GCDR_recSequenceNumber_present;
        pDest->recSequenceNumList.recSequenceNumber.count = nCount;

        Clear_List(m_pSequence);
        m_pSequence = m_pSequenceTail = NULL;
    }

    //����nodeID�ֶ�(һ������)
    if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_nodeID_present)
    {
        memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(CDR3G_NodeID));
    }

    //�ϲ�recordExtensions�ֶ�
    if(m_pOrgRec->bit_mask
        & CDR3G_GGSNPDPRecord_recordExtensions_present)
    {
        if(m_pExtension != NULL)
        {
            pDest->recordExtensions.value = (_3GCDR_ManagementExtension*)
                List_To_Array(m_pExtension,nCount/*in,out*/);
            
            pDest->recordExtensions.count = nCount;

            Clear_List(m_pExtension);
            m_pExtension = m_pExtensionTail = NULL;
        }
    }

    pBegin = (char*)(&(m_pOrgRec->apnSelectionMode));
    pEnd = (char*)(&(m_pOrgRec->chargingCharacteristics));

    /* 
     * apnSelectionMode,servedMSISDN,chargingCharacteristics
     */
    memcpy(&(pDest->apnSelectionMode),pBegin,
            (pEnd - pBegin) + sizeof(CDR3G_ChargingCharacteristics));

    //consolidationResult�ֶ�
    switch(m_CauseForRecClosing)
    {
    case CDR3G_normalRelease:
		//PDP��������
        pDest->consolidationResult
            = _3GCDR_consolidationResult_normal;
        break;
		//SGSN�ı�
	case CDR3G_sGSNChange:
		pDest->consolidationResult
            = _3GCDR_consolidationResult_ForInterSGSNConsolidation;
        break;         
    case CDR3G_abnormalRelease:
        //PDP�쳣����
        pDest->consolidationResult
            = _3GCDR_consolidationResult_notnormal;
        break;
    default:
        //PDPδ����(��ΪPDP�쳣����)
        if (m_bIsLimit)
        {
        	pDest->consolidationResult = _3GCDR_consolidationResult_ReachLimit;
        }
        else
        {
        	pDest->consolidationResult = _3GCDR_consolidationResult_notnormal;
        }
        break;
    }
    
    if (!IsPartial())
    {
    	pDest->consolidationResult = _3GCDR_consolidationResult_onlyOneCDRGenerated;	
    }

    pDest->duration = m_Duration;
    pDest->causeForRecClosing = m_CauseForRecClosing;

    return (char*)m_pDestRec;
}

UINT4 C3G_350_GCDR::GetBillType()
{
    return (m_pOrgRec->recordType);
}

CKey C3G_350_GCDR::CreateKey()
{
    C3G_350_GCDR_Key* pkey = new C3G_350_GCDR_Key;
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

BOOL C3G_350_GCDR::IsDuplicated(CBill* pBill)
{
    BOOL bSucc = FALSE;
    C3G_350_GCDR* p = (C3G_350_GCDR*)pBill;
    if((p->m_pOrgRec->bit_mask
        & CDR3G_GGSNPDPRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask
        & CDR3G_GGSNPDPRecord_recordSequenceNumber_present))
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
* ������:   C3G_350_GCDR::Merge
* ����:    Zhou Yonghui
* ����:    01-5-30
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
*01-5-30       Zhou Yonghui
*01-9-25       Zhou Yonghui
*              ������ͳ��sgsnAddress��listOfTrafficVolumes��
*              recordExtensions�Ĵ��룬�Ա�����������ơ�
******************************************************/
BOOL C3G_350_GCDR::Merge(CBill* pBill)
{
    assert(pBill != NULL);

    if(!m_bPrepared)
    {
        if(!PrepareData())  return FALSE;
    }

    C3G_350_GCDR* p = (C3G_350_GCDR*)pBill;

    //�����ظ�������
    if((p->m_pOrgRec->bit_mask
        & CDR3G_GGSNPDPRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask
        & CDR3G_GGSNPDPRecord_recordSequenceNumber_present))
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
    case CDR3G_normalRelease:
    case CDR3G_abnormalRelease:
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
		int nDiff = Get_Duration(&(m_pOrgRec->recordOpeningTime),
			&(p->m_pOrgRec->recordOpeningTime));
		m_Duration = nDiff;
	}
	//�ۼ�Duration�ֶΡ�
	m_Duration += p->m_pOrgRec->duration;
	//�޸Ľ���

    //Added by ZhengYuqun 2003-11-04 SWPD02238
    if((UINT4)m_Duration >= m_uMaxDuration)
    {
        m_bPDPCompleted = TRUE;
    }

    //����sgsnAddress�ֶΡ�
    
    //2002-01-18,Modified by zhouyonghui.
    //�޳��ظ���SGSN��ַ.

    if((p->m_pOrgRec->sgsnAddress.value != NULL)
        && (p->m_pOrgRec->sgsnAddress.count > 0))
    {
        unsigned int nCount = p->m_pOrgRec->sgsnAddress.count;

        if(m_pSGSNAddressTail == NULL)
        {
            //������������ж��ظ�SGSN��ַ

            SGSGAddress_LINK_STRUCT* q = SGSGAddress_LINK_STRUCT::Alloc();

            q->Value = Clone_CDR3G_GSNAddress(
                p->m_pOrgRec->sgsnAddress.value,nCount/*in*/);

            q->nCount = nCount;
            m_pSGSNAddressTail = m_pSGSNAddress = q;
        }
        else
        {
            //��ʱ�ж��Ƿ����ظ�SGSN��ַ

            SGSGAddress_LINK_STRUCT* q = SGSGAddress_LINK_STRUCT::Alloc();

            q->Value = Clone_CDR3G_GSNAddress(
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
         * Added by zhouyonghui,2001/09/25.
         */
        m_nLen_SGSNAddress_List += nCount;
    }

    //����listOfTrafficVolumes�ֶΡ�
    if((p->m_pOrgRec->listOfTrafficVolumes.value != NULL)
        && (p->m_pOrgRec->listOfTrafficVolumes.count > 0))
    {
        ChangeOfCharCondition_LINK_STRUCT* q
            = ChangeOfCharCondition_LINK_STRUCT::Alloc();

        q->Value = Clone_CDR3G_ChangeOfCharCondition(
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
         * Added by zhouyonghui,2001/09/25.
         */
        m_nLen_Condition_List += q->nCount;
    }

    //����recordExtensions�ֶΡ�
    if((p->m_pOrgRec->bit_mask
        & CDR3G_GGSNPDPRecord_recordExtensions_present)
        && (m_pOrgRec->bit_mask
        & CDR3G_GGSNPDPRecord_recordExtensions_present))
    {
        if((p->m_pOrgRec->recordExtensions.value != NULL)
            && (p->m_pOrgRec->recordExtensions.count > 0))
        {
            ManagementExtension_LINK_STRUCT* q
                = ManagementExtension_LINK_STRUCT::Alloc();
            
            q->Value = Clone_CDR3G_ManagementExtension(
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
             * Added by zhouyonghui,2001/09/25.
             */
            m_nLen_Extension_List += q->nCount;
        }
    }

    //����recordSequenceNumber�ֶΡ�
    if((p->m_pOrgRec->bit_mask
        & CDR3G_GGSNPDPRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask
        & CDR3G_GGSNPDPRecord_recordSequenceNumber_present))
    {
        RecordSequenceNumber_LINK_STRUCT* q
            = RecordSequenceNumber_LINK_STRUCT::Alloc();
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

    //diagnostics�ֶ�ȡ���һ�Ż�����ֵ��
    if((p->m_pOrgRec->bit_mask
        & CDR3G_GGSNPDPRecord_diagnostics_present))
        //&& (m_pOrgRec->bit_mask     //������ȥ��  ZhengYuqun 2003-11-03 SWPD02228
        //& CDR3G_GGSNPDPRecord_diagnostics_present))
    {
        CDR3G_ManagementExtension* q = NULL;
        if(p->m_pOrgRec->diagnostics.choice
            == CDR3G_networkSpecificCause_chosen)
        {
            q = Clone_CDR3G_ManagementExtension(
                &(p->m_pOrgRec->diagnostics.u.networkSpecificCause),1);
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CDR3G_manufacturerSpecificCause_chosen)
        {
            q = Clone_CDR3G_ManagementExtension(
                &(p->m_pOrgRec->diagnostics.u.manufacturerSpecificCause),1);
        }

        /*����ԭdiagnostics�ֶεĿռ䡣*/
        if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_diagnostics_present) //����һ��if ZhengYuqun 2003-11-03 SWPD02228
        {
            if(m_Diagnostics.choice == CDR3G_manufacturerSpecificCause_chosen)
            {
                delete[] m_Diagnostics.u.manufacturerSpecificCause.identifier.value;
                delete[] m_Diagnostics.u.manufacturerSpecificCause.information.value;
            }
            else if(m_Diagnostics.choice == CDR3G_networkSpecificCause_chosen)
            {
                delete[] m_Diagnostics.u.networkSpecificCause.identifier.value;
                delete[] m_Diagnostics.u.networkSpecificCause.information.value;
            }
        }
        else
        {
            m_pOrgRec->bit_mask |= CDR3G_GGSNPDPRecord_diagnostics_present; //����һ�� ZhengYuqun 2003-11-03 SWPD02228
        }
        /************************/
        if(p->m_pOrgRec->diagnostics.choice
            == CDR3G_networkSpecificCause_chosen)
        {
            m_Diagnostics.choice = CDR3G_networkSpecificCause_chosen;
            
            m_Diagnostics.u.networkSpecificCause = *q;
            delete[] q;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CDR3G_manufacturerSpecificCause_chosen)
        {
            m_Diagnostics.choice = CDR3G_manufacturerSpecificCause_chosen;
            
            m_Diagnostics.u.manufacturerSpecificCause = *q;
            delete[] q;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CDR3G_gsm0408Cause_chosen)
        {
            m_Diagnostics.choice = CDR3G_gsm0408Cause_chosen;
            m_Diagnostics.u.gsm0408Cause = 
                p->m_pOrgRec->diagnostics.u.gsm0408Cause;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CDR3G_gsm0902MapErrorValue_chosen)
        {
            m_Diagnostics.choice = CDR3G_gsm0902MapErrorValue_chosen;
            m_Diagnostics.u.gsm0902MapErrorValue = 
                p->m_pOrgRec->diagnostics.u.gsm0902MapErrorValue;
        }
        else if(p->m_pOrgRec->diagnostics.choice
            == CDR3G_ccittQ767Cause_chosen)
        {
            m_Diagnostics.choice = CDR3G_ccittQ767Cause_chosen;
            m_Diagnostics.u.ccittQ767Cause = 
                p->m_pOrgRec->diagnostics.u.ccittQ767Cause;
        }
    }

    /*
     * �����ֶ�ȡ��һ�Ż�����ֵ��
     */
    return TRUE;
}
