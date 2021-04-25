/*****************************************************
*       gprs_bill.cpp
******************************************************
*Copyright 2002 by Huawei CO.,LTD.
* All rights reserved.
* Author:       ����Զ
* Date:         2002��3��
* Description:  
* 
* Side Effects: NONE
* Functions:
* Notes:    
* 
*****************************************************/
#include "r98_bill.h"
#include "../../merge/bill.h"
#include "../../merge/merge.h"
#include <time.h>
#include "../../include/export_dll.h"

extern "C" IGWB_Svc_Export void* CreateParserObject()
{
    return (void*)new CGPRS_BillParser();
}

//���캯��
CGPRS_BillParser::CGPRS_BillParser()
{
	m_pBill = NULL;
}

//��������
CGPRS_BillParser::~CGPRS_BillParser()
{
	if (NULL != m_pBill)
	{
		delete m_pBill;
		m_pBill = NULL;
	}
}

/**********************************************************************
��������    ����һ�Ż���
�������    pBuf��������������ָ��
			uBufLen���������������ĳ���
�������    ��
����ֵ      ���������Ƿ���Ҫ�ϲ�
                TRUE----����������Ҫ�ϲ� 
                FALSE----�������Ͳ���Ҫ�ϲ�
�׳��쳣    �ڽ���ʧ��ʱ�׳��쳣���������˴�����Ϣ���ַ���
**********************************************************************/
BOOL CGPRS_BillParser::ParseARecord(char* pBuf,UINT4 uBufLen)
{
    assert(NULL != pBuf);
  
	Org_CallEventRecord* pRec = (Org_CallEventRecord* )pBuf;
	switch (pRec->choice)
	{
		case Org_sgsnPDPRecord_chosen:
			m_pBill = new CGPRS_130_OSS_SCDR;
			if (NULL != m_pBill)
			{
				//�û�����¼��ʼ����������ʧ�ܽ��׳��쳣��
				if (m_pBill->Init((char* )&(pRec->u.sgsnPDPRecord), 
								  sizeof(Org_SGSNPDPRecord)))
				{
					return TRUE;
				}
			}
			throw "S-CDR initialize fail.";
	        break;

		case Org_ggsnPDPRecord_chosen:
			m_pBill = new CGPRS_130_OSS_GCDR;
			if (NULL != m_pBill)
			{
				//�û�����¼��ʼ����������ʧ�ܽ��׳��쳣��
				if (m_pBill->Init((char* )&(pRec->u.ggsnPDPRecord), 
								  sizeof(Org_GGSNPDPRecord)))
				{
					return TRUE;
				}
			}

		    throw "G-CDR initialize fail.";
			break;	

		default:  //ֻ��SCDR��GCDR��Ҫ�ϲ����������͵Ļ������úϲ�
            break;
    }

	return FALSE;
}

//���ص��Ż����Ľ������
CBill* CGPRS_BillParser::GetBillRecord()
{
	CBill* pBill = m_pBill;
	m_pBill = NULL;

	return pBill;
}

//���Ҫ�ϲ��Ļ�����������
UINT4 CGPRS_BillParser::GetBillTypeCount()
{
	//GPRS�����ֻ�����Ҫ�ϲ���SCDR��GCDR
	return 2;
}

//�����������ػ�������ID
UINT4 CGPRS_BillParser::GetBillTypeID(UINT4 uIndex)
{
	UINT4 uBillTypeID = 0;
	switch (uIndex)
	{
		case 0:
			uBillTypeID = GPRS_S_CDR;
			break;
		case 1:
			uBillTypeID = GPRS_G_CDR;
			break;
		default:
			break;
	}

	return uBillTypeID;
}

//��ָ����CMerger������ע�Ự������
void CGPRS_BillParser::RegisterBillType(CMerger* pMerger)
{
	assert(NULL != pMerger);

	for (UINT4 i = 0; i < GetBillTypeCount(); i++)
	{
		pMerger->RegisterBillType(GetBillTypeID(i));
	}
}

//���ָ���Ļ�������ID�Ļ�����������
void CGPRS_BillParser::GetBillTypeDesc(UINT4 uBillTypeID, char* szDesc)
{
    switch (uBillTypeID)
    {
    case GPRS_S_CDR:
        strcpy(szDesc, "S-CDR");
        break;
    case GPRS_G_CDR:
        strcpy(szDesc, "G-CDR");
        break;
    default:
		sprintf(szDesc, "%d-Unknown", uBillTypeID);
        break;
    }
}

//���캯��
CGPRS_130_OSS_CDR::CGPRS_130_OSS_CDR()
{
    m_bDataPrepared = FALSE;
}

//��������
CGPRS_130_OSS_CDR::~CGPRS_130_OSS_CDR()
{
}

//��ȡ����ʱ�̼��ʱ����������Ϊ��λ
//��������ʱ��֮�
/* ˵���������������¿��ǣ����t2ʱ��Ϊzz2��t1ʱ��Ϊzz1��
 * ��ôt2�൱�ڸ�������ʱ��t2-zz2��t1���൱�ڸ�������ʱ��t1-zz1��
 * ���ʱ���Ϊ(t2-zz2)-(t1-zz1)��Ҳ����t2-t1+zz1-zz2
 */
Fin_CallDuration CGPRS_130_OSS_CDR::GetDuration(const Fin_TimeStamp* pSmallTime, 
												const Org_TimeStamp* pLargeTime)
{
    Fin_CallDuration nDiff = 0; //ʱ��֮��
    int zhh1,zMM1, yy1, mm1, dd1, hh1, MM1, ss1;	
    int zhh2,zMM2, yy2, mm2, dd2, hh2, MM2, ss2;	//zhh1,zMM1,zhh2,zMM2��ʾʱ����Сʱ������
    
    yy1 = ((pSmallTime->value[0] & 0xF0 ) >> 4  ) * 10 + (pSmallTime->value[0] & 0x0f);
    mm1 = ((pSmallTime->value[1] & 0xF0 ) >> 4  ) * 10 + (pSmallTime->value[1] & 0x0f);
    dd1 = ((pSmallTime->value[2] & 0xF0 ) >> 4  ) * 10 + (pSmallTime->value[2] & 0x0f);
    hh1 = ((pSmallTime->value[3] & 0xF0 ) >> 4  ) * 10 + (pSmallTime->value[3] & 0x0f);
    MM1 = ((pSmallTime->value[4] & 0xF0 ) >> 4  ) * 10 + (pSmallTime->value[4] & 0x0f);
    ss1 = ((pSmallTime->value[5] & 0xF0 ) >> 4  ) * 10 + (pSmallTime->value[5] & 0x0f);
    
	zhh1 = ((pSmallTime->value[7] & 0xF0 ) >> 4  ) * 10 + (pSmallTime->value[7] & 0x0f) ;
    zMM1 = ((pSmallTime->value[8] & 0xF0 ) >> 4  ) * 10 + (pSmallTime->value[8] & 0x0f) ;
        
    if(pSmallTime->value[6] == '-')
    {
		zhh1 = -zhh1;
        zMM1 = - zMM1;
    }
    
    yy2 = ((pLargeTime->value[0] & 0xF0 ) >> 4  ) * 10 + (pLargeTime->value[0] & 0x0f);
    mm2 = ((pLargeTime->value[1] & 0xF0 ) >> 4  ) * 10 + (pLargeTime->value[1] & 0x0f);
    dd2 = ((pLargeTime->value[2] & 0xF0 ) >> 4  ) * 10 + (pLargeTime->value[2] & 0x0f);
    hh2 = ((pLargeTime->value[3] & 0xF0 ) >> 4  ) * 10 + (pLargeTime->value[3] & 0x0f);
    MM2 = ((pLargeTime->value[4] & 0xF0 ) >> 4  ) * 10 + (pLargeTime->value[4] & 0x0f);
    ss2 = ((pLargeTime->value[5] & 0xF0 ) >> 4  ) * 10 + (pLargeTime->value[5] & 0x0f);
    
	zhh2 = ((pLargeTime->value[7] & 0xF0 ) >> 4  ) * 10 + (pLargeTime->value[7] & 0x0f) ;
    zMM2 = ((pLargeTime->value[8] & 0xF0 ) >> 4  ) * 10 + (pLargeTime->value[8] & 0x0f) ;

    if(pLargeTime->value[6] == '-')
    {
		zhh2 = -zhh2;
        zMM2 = -zMM2;
    }

	//�����������ܻ��޸�ʱ�䣬���Ա��뿼��һ��2000������
	if (yy1 > 70) 
	{
		yy1 += 1900;
	}
	else
	{
		yy1 += 2000;
	}
	if (yy2 > 70)
	{
		yy2 += 1900;
	}
	else
	{
		yy2 += 2000;
	}

    //��������ʱ��֮����������
    int nDaySpanInSec = 0;
	tm tmpTm1,tmpTm2;
	memset(&tmpTm1,0,sizeof(tm));
	memset(&tmpTm2,0,sizeof(tm));
	tmpTm1.tm_year = yy1 - 1900;
	tmpTm1.tm_mon = mm1-1;
	tmpTm1.tm_mday = dd1;
	tmpTm2.tm_year = yy2 - 1900;
	tmpTm2.tm_mon = mm2-1;
	tmpTm2.tm_mday = dd2;
	time_t tmpTime1 = mktime(&tmpTm1);
	time_t tmpTime2 = mktime(&tmpTm2);
	double dEelapsedSec = difftime(tmpTime2,tmpTime1);
    nDaySpanInSec = (int)dEelapsedSec;

    nDiff = nDaySpanInSec + (hh2 - hh1 + zhh1 - zhh2) * 3600 + (MM2 - MM1 + zMM1 - zMM2) * 60 + (ss2 - ss1);

	//��ʵ���У�ʱ����������Ϊ����������˵��������ʱ������
	if (nDiff < 0)  
	{
		nDiff = 0;
	}
    return nDiff;
}

//��¡Org_ManagementExtensions_������
void CGPRS_130_OSS_CDR::CloneExtensionsList(const Org_ManagementExtensions_* pSrc, 
							   Org_ManagementExtensions_*& pDst)
{
	CloneList<Org_ManagementExtensions_>(pSrc, pDst);

	//��ʼ�����еĽڵ��е�ָ���Ա����
	unsigned char *pBuf = NULL;
	Org_ManagementExtensions_* pSrcTmp = (Org_ManagementExtensions_* )pSrc;
	Org_ManagementExtensions_* pDstTmp = (Org_ManagementExtensions_* )pDst;
	while (NULL != pSrcTmp)
	{
		if (pSrcTmp->value.identifier.length > 0)
		{
			pBuf = new unsigned char[pSrcTmp->value.identifier.length];
			if (NULL == pBuf)
			{
				throw "Failed to new information when clone extension list.";
			}
			memcpy(pBuf, pSrcTmp->value.identifier.value, 
				   pSrcTmp->value.identifier.length);

			pDstTmp->value.identifier.value = pBuf;
		}
		else
		{
			pDstTmp->value.identifier.length = 0;
			pDstTmp->value.identifier.value = NULL;
		}

		if (pSrcTmp->value.information.length > 0)
		{
			pBuf = new unsigned char[pSrcTmp->value.information.length];
			if (NULL == pBuf)
			{
				throw "Failed to new information when clone extension list.";
			}
			memcpy(pBuf, pSrcTmp->value.information.value, 
				   pSrcTmp->value.information.length);

			pDstTmp->value.information.value = pBuf;
		}
		else
		{
			pDstTmp->value.information.length = 0;
			pDstTmp->value.information.value = NULL;
		}

		pSrcTmp = pSrcTmp->next;
		pDstTmp = pDstTmp->next;
	}
}

//ɾ��Org_ManagementExtensions_������
void CGPRS_130_OSS_CDR::ClearExtensionsList(Org_ManagementExtensions_*& pExtension)
{
	//����ɾ�����нڵ��е�ָ���Ա����
	Org_ManagementExtensions_* pTmp = pExtension;
	while (NULL != pTmp)
	{
		if ((NULL != pTmp->value.identifier.value) 
			 && (pTmp->value.identifier.length > 0))
		{
			delete[] pTmp->value.identifier.value;
		}
		if ((NULL != pTmp->value.information.value) 
			 && (pTmp->value.information.length > 0))
		{
			delete[] pTmp->value.information.value;
		}

		pTmp = pTmp->next;
	}
    
	ClearList<Org_ManagementExtensions_>(pExtension);
}

//�ж��Ƿ����ꡣ
BOOL CGPRS_130_OSS_CDR::isLeapYear(int nYear)
{
    if (nYear % 400 == 0)
	{
        return TRUE;
	}
    else if (nYear % 100 == 0)
	{
        return FALSE;
	}
    else if (nYear % 4 == 0)
	{
        return TRUE;
	}

    return FALSE;
}

//�������£����ظ��µ�������
int CGPRS_130_OSS_CDR::GetDaysOfMonth(int nYear, int nMonth)
{
    switch (nMonth)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return 31;
        
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
       
    case 2:
        if (isLeapYear(nYear))
		{
            return 29;
		}
        else
		{
            return 28;
		}
        
    default:
        return 0;
       
    }
}


//���캯��
CGPRS_130_OSS_SCDR::CGPRS_130_OSS_SCDR()
{
    m_pQosTail = NULL;
	m_nQosLen = 0;
    m_pSequenceTail = NULL;
	m_nSequenceLen = 0;
    m_pExtensionTail = NULL;
	m_nExtensionLen = 0;

    m_pOrgRec = NULL;
    m_pDestRec = NULL;
	m_pDestEventRec = NULL;
}

//��������
CGPRS_130_OSS_SCDR::~CGPRS_130_OSS_SCDR()
{
	if (NULL != m_pOrgRec)
	{
		assert((NULL == m_pDestEventRec) && !m_bDataPrepared);

		//��������ṹ�а���������
		if (NULL != m_pOrgRec->listOfTrafficVolumes)
		{
			ClearList<Org__seqof1_>(m_pOrgRec->listOfTrafficVolumes);
			m_nQosLen = 0;
			m_pQosTail = NULL;
		}
		if ((m_pOrgRec->bit_mask & Org_SGSNPDPRecord_recordExtensions_present) 
			  && (NULL != m_pOrgRec->recordExtensions))
		{
			ClearExtensionsList(m_pOrgRec->recordExtensions);
			m_nExtensionLen = 0;
			m_pExtensionTail = NULL;
		}

		delete m_pOrgRec;
		m_pOrgRec = NULL;
	}

    if (NULL != m_pDestEventRec)
    {
		assert((NULL == m_pOrgRec) && m_bDataPrepared);

		//��������ṹ�а���������
		if (NULL != m_pDestRec->sgsnAddress)
		{
			ClearList<Fin__seqof1_>(m_pDestRec->sgsnAddress);
		}
		if (NULL != m_pDestRec->listOfTrafficVolumes)
		{
			ClearList<Fin__seqof2_>(m_pDestRec->listOfTrafficVolumes);
			m_nQosLen = 0;
			m_pQosTail = NULL;
		}
		if ((m_pDestRec->bit_mask & Fin_SGSNPDPRecord_recordSequenceNumber_present)
				&& (NULL != m_pDestRec->recordSequenceNumber))
		{
			Fin__seqof3* pTmp = m_pDestRec->recordSequenceNumber;
			while (NULL != pTmp)
			{
				ClearList<Fin_RecordNumberList_>(pTmp->value.recordNumberList);
				pTmp = pTmp->next;
			}
			ClearList<Fin__seqof3>(m_pDestRec->recordSequenceNumber);

			m_pSequenceTail = NULL;
			m_nSequenceLen = 0;
		}
		if ((m_pDestRec->bit_mask & Fin_SGSNPDPRecord_recordExtensions_present)
				&& (NULL != m_pDestRec->recordExtensions))
		{
			ClearExtensionsList((Org_ManagementExtensions_*& )m_pDestRec->recordExtensions);
			m_nExtensionLen = 0;
			m_pExtensionTail = NULL;
		}

		delete m_pDestEventRec;
		m_pDestEventRec = NULL;
		m_pDestRec = NULL;
	}
}

/**********************************************************************
��������    ��ʼ��CBill����
�������    pBuf����������¼������
			uLen��������������
�������    ��
����ֵ      ��ʼ���Ľ��
                TRUE----��ʼ���ɹ�
                FALSE----��ʼ��ʧ��
�׳��쳣    �ڴ����ʧ���׳��쳣
**********************************************************************/
BOOL CGPRS_130_OSS_SCDR::Init(char* pBuf, UINT4 uLen)
{
	assert((NULL != pBuf) && (0 != uLen));

	if (uLen != sizeof(Org_SGSNPDPRecord))
	{
		return FALSE;
	}

	m_pOrgRec = new Org_SGSNPDPRecord;
	if (NULL == m_pOrgRec)
	{
		throw "Fail to new Org_SGSNPDPRecord when init.";
	}
	memset(m_pOrgRec,0,sizeof(Org_SGSNPDPRecord));
	memcpy(m_pOrgRec, pBuf, uLen);

	//�������ݳ�ʼ��Qos������صĳ�Ա����
	CloneList<Org__seqof1_>(((Org_SGSNPDPRecord* )pBuf)->listOfTrafficVolumes, 
							m_pOrgRec->listOfTrafficVolumes);
	m_nQosLen = TravelList<Org__seqof1_>(m_pOrgRec->listOfTrafficVolumes, 
										 (Org__seqof1_*& )m_pQosTail);

	//�������ݳ�ʼ��Extension������صĳ�Ա����
	if (m_pOrgRec->bit_mask & Org_SGSNPDPRecord_recordExtensions_present)
	{
		CloneExtensionsList(((Org_SGSNPDPRecord* )pBuf)->recordExtensions, 
							m_pOrgRec->recordExtensions);
		m_nExtensionLen = TravelList<Org_ManagementExtensions_>(m_pOrgRec->recordExtensions, 
												(Org_ManagementExtensions_*& )m_pExtensionTail);
	}
	else  //������ 2002-04-16
	{
		m_pOrgRec->recordExtensions = NULL;
	}
	return TRUE;
}

/**********************************************************************
��������    �жϵ�ǰ�����Ƿ�������pBill�ϲ�
�������    pBill��������ָ��
�������    ��
����ֵ      �Ƿ��ܹ��ϲ�
                TRUE----�ܹ��ϲ�
                FALSE----���ܺϲ�
˵��: 1����pBillָ��Ļ������ظ�����������TRUE���Ա���
          Merge�����ж����û�����
      2����pBillָ��Ļ������кŲ���������Ϊ���ܹ��ϲ���
      3����pBill����Ҫ���ӵ��ֶγ��ȹ�������Ϊ���ܹ��ϲ���
**********************************************************************/
BOOL CGPRS_130_OSS_SCDR::CanSafeMerge(CBill* pBill)
{
	assert(NULL != pBill);
   	//����ȷ����ǰ�����Ѿ�ת��Ϊ���ջ�����ʽ
    
    if (!m_bDataPrepared)
    {
        BOOL bResult = PrepareData();
        if (bResult == FALSE)  
		{
			return FALSE;
		}
    }

	CGPRS_130_OSS_SCDR* pSCDR = (CGPRS_130_OSS_SCDR* )pBill;

	//���������ǲ��ֻ�����recordSequenceNumber�������Ч
	if (!(pSCDR->GetOrgRec()->bit_mask & Org_SGSNPDPRecord_recordSequenceNumber_present)
		  || !(m_pDestRec->bit_mask & Fin_SGSNPDPRecord_recordSequenceNumber_present))
	{
		return FALSE;
	}

    //�ظ�������Merge�����ж�����
	if (IsDuplicated(pSCDR))
	{
		return TRUE;
	}

	//�������ظ���������Ҫ�����к�����
	if (pSCDR->GetOrgRec()->recordSequenceNumber != m_pSequenceTail->value + 1)
	{
		return FALSE;
	}

	//�ж��Ƿ񳬹��������ޡ�
	//�������������ޣ����жϺϲ��������úϲ����״̬ΪFin_reachLimit
	if (m_nQosLen + pSCDR->GetQosLen() > MAX_MERGED_BILLS)
	{
		m_pDestRec->consolidationResult = Fin_reachLimit;
		return FALSE;
	}
	if (m_nSequenceLen + 1 > MAX_MERGED_BILLS)
	{

		m_pDestRec->consolidationResult = Fin_reachLimit;
		return FALSE;
	}
	if ((pSCDR->GetOrgRec()->bit_mask & Org_SGSNPDPRecord_recordExtensions_present)
		  && (m_pDestRec->bit_mask & Fin_SGSNPDPRecord_recordExtensions_present))
	{
		if (m_nExtensionLen + pSCDR->GetExtensionLen() > MAX_MERGED_BILLS)
		{
			m_pDestRec->consolidationResult = Fin_reachLimit;
			return FALSE;
		}
	}

	return TRUE;
}

/**********************************************************************
��������    ��pBill���뵱ǰ������
�������    pBill���������뻰�������ָ��
�������    ��
����ֵ      �ϲ��Ľ��
                TRUE----�ϲ��ɹ�
                FALSE----�ϲ�ʧ��
�׳��쳣    �ڴ����ʧ���׳��쳣
**********************************************************************/
BOOL CGPRS_130_OSS_SCDR::Merge(CBill* pBill)
{
	assert((NULL != pBill) && pBill->IsPartial());

	//����ȷ����ǰ�����Ѿ�ת��Ϊ���ջ�����ʽ
    if (!m_bDataPrepared)
    {
		BOOL bRes = PrepareData(); 
        if (bRes != TRUE)  
		{
			return FALSE;
		}
    }
	CGPRS_130_OSS_SCDR* pSCDR = (CGPRS_130_OSS_SCDR* )pBill;

	//�����ظ�����
	if (IsDuplicated(pSCDR))
	{
		return TRUE;
	}

	//����listOfTrafficVolumes�ֶ�
	if (NULL != pSCDR->GetOrgRec()->listOfTrafficVolumes)
	{
		Fin__seqof2_* pHead = NULL;
		Fin__seqof2_* pTail = NULL;
		pSCDR->GetQosList(pHead, pTail);

		if (NULL == m_pQosTail)
		{
			m_pDestRec->listOfTrafficVolumes = pHead;
		}
		else
		{
			m_pQosTail->next = pHead;
		}

		m_pQosTail = pTail;
		m_nQosLen += pSCDR->GetQosLen();
		pSCDR->RemoveQosList();
	}

	//����recordSequenceNumber�ֶ�
	if ((pSCDR->GetOrgRec()->bit_mask & Org_SGSNPDPRecord_recordSequenceNumber_present)
		  && (m_pDestRec->bit_mask & Fin_SGSNPDPRecord_recordSequenceNumber_present))
	{
		Fin_RecordNumberList_* pSeqNum = new Fin_RecordNumberList_;
		if (NULL == pSeqNum)
		{
			throw "Fail to new pSeqNum when merge SCDR";
		}

		pSeqNum->next = NULL;
		pSeqNum->value = pSCDR->GetOrgRec()->recordSequenceNumber;

		//m_pSequenceTail�϶���ΪNULL
		m_pSequenceTail->next = pSeqNum;
		m_pSequenceTail = pSeqNum;
		m_nSequenceLen++;
	}

	//����recordExtensions�ֶ�
	if ((pSCDR->GetOrgRec()->bit_mask & Org_SGSNPDPRecord_recordExtensions_present)
		 && (m_pDestRec->bit_mask & Fin_SGSNPDPRecord_recordExtensions_present)
		 && (NULL != pSCDR->GetOrgRec()->recordExtensions))
	{
		Fin_ManagementExtensions_* pHead = NULL;
		Fin_ManagementExtensions_* pTail = NULL;
		pSCDR->GetExtensionList(pHead, pTail);

		if (NULL == m_pExtensionTail)
		{
			m_pDestRec->recordExtensions = pHead;
		}
		else
		{
			m_pExtensionTail->next = pHead;
		}

		m_pExtensionTail = pTail;
		m_nExtensionLen += pSCDR->GetExtensionLen();
		pSCDR->RemoveExtensionList();
	}

	//causeForRecClosing��ȡ���һ�Ż�����ֵ
	m_pDestRec->causeForRecClosing
		 = (Fin_CauseForRecClosing)(pSCDR->GetOrgRec()->causeForRecClosing);

	//Z30971�����޸��Ƿ�֧����ʱ��
	if(!m_bDSTFlg)
	{
		//duration���ռ�����ʽ����
		Fin_CallDuration nDiff = GetDuration(&(m_pDestRec->recordOpeningTime), 
			&(pSCDR->GetOrgRec()->recordOpeningTime));
		m_pDestRec->duration = nDiff;
	}
	m_pDestRec->duration += pSCDR->GetOrgRec()->duration;
	//�޸Ľ���
	
	/*
	����consolidationResult�ֶ�:

	1.PDP����,���ݻ���ԭ������consolidationResult

	2.PDPδ����,������Ϊ�ϲ������������,����consolidationResult
	  ΪFin_reachLimit,����ԭ������Fin_notNormal
	*/
    switch (pSCDR->GetOrgRec()->causeForRecClosing)
	{
    case Fin_normalRelease:             //�����ͷ�
        m_pDestRec->consolidationResult = Fin_normal;
		break;
    case Fin_sGSNChange:                //SGSN�ı�
        m_pDestRec->consolidationResult = Fin_forInterSGSNConsolidation;
        break;
    case Fin_abnormalRelease:           //PDP�쳣����
        m_pDestRec->consolidationResult = Fin_notNormal;
        break;
    default:                            //PDPδ����(��ΪPDP�쳣����)
        m_pDestRec->consolidationResult = Fin_notNormal;
        break;
    }

    return TRUE;
}

/**********************************************************************
��������    �жϵ�ǰ����������PDP�Ƿ��Ѿ�����
�������    ��
�������    ��
����ֵ      PDP�Ƿ��Ѿ�����
                TRUE----PDP�Ѿ�����
                FALSE----PDPδ����
˵��:       ����������ֹԭ��Ϊ�������ͷš�����SGSN�ı䡱��
           ��PDP�쳣������˵��PDP�Ѿ�����������δ������
**********************************************************************/
BOOL CGPRS_130_OSS_SCDR::IsPDPCompleted()
{
	assert(NULL != m_pDestRec);

	switch (m_pDestRec->causeForRecClosing)
	{
	case Fin_normalRelease:
	case Fin_abnormalRelease:
	case Fin_sGSNChange:
		return TRUE;
	
	default:
		break;
	}

    //���Ӱ�ͨ��ʱ�������ϲ� ZhengYuqun 2003-12-18
    if((UINT4)m_pDestRec->duration >= m_uMaxDuration)
    {
        return TRUE;
    }

	return FALSE;
}

//�ж��Ƿ���Ҫ�����ȴ������������ڴ�ʼ�շ���FALSE
BOOL CGPRS_130_OSS_SCDR::NeedWaiting()
{
	return FALSE;
}

//�������ո�ʽ�Ļ�����¼
char* CGPRS_130_OSS_SCDR::GetContents()
{
    if (m_bDataPrepared != TRUE)
    {
        BOOL bRes = PrepareData();
		if (bRes != TRUE)
		{
			return NULL;
		}
    }

	return (char* )m_pDestEventRec;
}

//�ж��Ƿ񲿷ֻ���,���recordSequenceNumber����Ч��Ϊ���ֻ���
BOOL CGPRS_130_OSS_SCDR::IsPartial()
{
	return (m_pOrgRec->bit_mask & Org_SGSNPDPRecord_recordSequenceNumber_present);
}

//�ж��Ƿ�PDP�ĵ�һ�Ų��ֻ���,
//��Ϊ���ֻ�����recordSequenceNumberΪ0��Ϊ��һ�Ų��ֻ���
BOOL CGPRS_130_OSS_SCDR::IsFirstPartialRec()
{ 
	if (!IsPartial())
	{
		return FALSE;
	}

	return (0 == m_pOrgRec->recordSequenceNumber);
}

//�ж��Ƿ�PDP��ĩ�Ų��ֻ���
BOOL CGPRS_130_OSS_SCDR::IsLastPartialRec()
{
	assert(NULL != m_pOrgRec);

	switch (m_pOrgRec->causeForRecClosing)
	{
	case Fin_normalRelease:
	case Fin_abnormalRelease:
	case Fin_sGSNChange:
		return TRUE;

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

//���ػ�������IDֵ
UINT4 CGPRS_130_OSS_SCDR::GetBillType()
{
	return GPRS_S_CDR;
}

//������������,
//S-CDR�����������������ֶΣ�chargingID��ggsnAddressUsed��sgsnAddress
CKey CGPRS_130_OSS_SCDR::CreateKey()
{
    CGPRS_130_OSS_SCDR_Key* pKey = new CGPRS_130_OSS_SCDR_Key;
	if (NULL == pKey)
	{
		throw "Fail to new s-cdr key.";
	}
	if (NULL != m_pOrgRec)
	{
		pKey->SetChargingID(m_pOrgRec->chargingID);
		pKey->SetGGSNAddress(m_pOrgRec->ggsnAddressUsed);
		pKey->SetSGSNAddress(m_pOrgRec->sgsnAddress);
	}
	else if (NULL != m_pDestRec)
	{
    	pKey->SetChargingID(m_pDestRec->chargingID);
		pKey->SetGGSNAddress((Org_GSNAddress& )(m_pDestRec->ggsnAddressUsed));
		pKey->SetSGSNAddress((Org_GSNAddress& )(m_pDestRec->sgsnAddress->value));
	}
	else
	{
		assert(FALSE);
	}

    CKey key;
    key.m_pDelegation = pKey;

    return key;
}

//���listOfTrafficVolumes����ĳ���
int CGPRS_130_OSS_SCDR::GetQosLen()
{
	return m_nQosLen;
}

//���recordExtensions����ĳ���
int CGPRS_130_OSS_SCDR::GetExtensionLen()
{
	return m_nExtensionLen;
}

//���listOfTrafficVolumes�����ͷ��β
void CGPRS_130_OSS_SCDR::GetQosList(Fin__seqof2_*& pHead, Fin__seqof2_*& pTail)
{
	if (NULL != m_pOrgRec)
	{
		pHead = (Fin__seqof2_* )(m_pOrgRec->listOfTrafficVolumes);
	}
	else if (NULL != m_pDestRec)
	{
		pHead = m_pDestRec->listOfTrafficVolumes;
	}
	else
	{
		assert(FALSE);
		pHead = NULL;
	}

	pTail = m_pQosTail;
}

//���recordExtensions�����ͷ��β
void CGPRS_130_OSS_SCDR::GetExtensionList(Fin_ManagementExtensions_*& pHead, 
										  Fin_ManagementExtensions_*& pTail)
{
	if (NULL != m_pOrgRec)
	{
		pHead = (Fin_ManagementExtensions_* )(m_pOrgRec->recordExtensions);
	}
	else if (NULL != m_pDestRec)
	{
		pHead = m_pDestRec->recordExtensions;
	}
	else
	{
		assert(FALSE);
		pHead = NULL;
	}

	pTail = m_pExtensionTail;
}

//ȥ��listOfTrafficVolumes���У��ڱ��������ϲ����������������
void CGPRS_130_OSS_SCDR::RemoveQosList()
{
	//������ֻ�ܶ�ԭʼ�������в���
	assert(NULL != m_pOrgRec);

	m_pOrgRec->listOfTrafficVolumes = NULL;
	m_pQosTail = NULL;
	m_nQosLen = 0;
}

//ȥ��recordExtensions���У��ڱ��������ϲ����������������
void CGPRS_130_OSS_SCDR::RemoveExtensionList()
{
	//������ֻ�ܶ�ԭʼ�������в���
	assert(NULL != m_pOrgRec);

	m_pOrgRec->recordExtensions = NULL;
	m_pExtensionTail = NULL;
	m_nExtensionLen = 0;
}

//��ȡԭʼ�����ṹ��ָ��
const Org_SGSNPDPRecord* CGPRS_130_OSS_SCDR::GetOrgRec()
{
	assert(NULL != m_pOrgRec);

	return m_pOrgRec;
}

//�ж��Ƿ��ظ�����
BOOL CGPRS_130_OSS_SCDR::IsDuplicated(CBill* pBill)
{
    CGPRS_130_OSS_SCDR* pSCDR = (CGPRS_130_OSS_SCDR* )pBill;
    int nHeadSeq, nTailSeq, nCurSeq;
    //��ǰ���ջ�������ʼ��ˮ��
    nHeadSeq = m_pDestRec->recordSequenceNumber->value.recordNumberList->value;
    //��ǰ���ջ����������ˮ��
    nTailSeq = m_pSequenceTail->value;
    //��ǰ���ϲ���������ˮ��
    nCurSeq = pSCDR->GetOrgRec()->recordSequenceNumber;
    
    //�����������к����Ѿ��ϲ��Ļ�����Χ�ڣ�˵�����ظ�������
    return ((nCurSeq >= nHeadSeq) && (nCurSeq <= nTailSeq));
}

/**********************************************************************
��������    ��ԭʼ������¼ת��Ϊ�������ո�ʽ�Ļ�����¼
�������    ��
�������    ��
����ֵ      ת���Ƿ�ɹ�
                TRUE----ת���ɹ�
                FALSE----ת�����ɹ�
�׳��쳣    �ڴ����ʧ���׳��쳣
**********************************************************************/
BOOL CGPRS_130_OSS_SCDR::PrepareData()
{
	assert(NULL != m_pOrgRec);
	char* pBegin = NULL, *pEnd = NULL;
	Fin_CallEventRecord* pEventRec = new Fin_CallEventRecord;
	if (NULL == pEventRec)
	{
		throw "Fail to new Fin_CallEventRecord when prepare scdr data.";
	}
	pEventRec->choice = Fin_sgsnPDPRecord_chosen;
	Fin_SGSNPDPRecord* pRec 
		 = (Fin_SGSNPDPRecord* )&(pEventRec->u.sgsnPDPRecord);
	memset(pRec, 0, sizeof(Fin_SGSNPDPRecord));
	//������ 2002-4-25
	//ת��bit_mask����bit_mask����һЩλ�������ֻ����в�һ��
	if(m_pOrgRec->bit_mask & Org_SGSNPDPRecord_networkInitiation_present)
	{
		pRec->bit_mask |= Fin_SGSNPDPRecord_networkInitiation_present;
	}
	if(m_pOrgRec->bit_mask & Org_SGSNPDPRecord_anonymousAccessIndicator_present)
	{
		pRec->bit_mask |= Fin_SGSNPDPRecord_anonymousAccessIndicator_present;
	}
	if(m_pOrgRec->bit_mask & Org_SGSNPDPRecord_servedIMEI_present)
	{
		pRec->bit_mask |= Fin_SGSNPDPRecord_servedIMEI_present;
	}
	if(m_pOrgRec->bit_mask & Org_SGSNPDPRecord_msNetworkCapability_present)
	{
		pRec->bit_mask |= Fin_SGSNPDPRecord_msNetworkCapability_present;
	}
	if(m_pOrgRec->bit_mask & Org_SGSNPDPRecord_routingArea_present)
	{
		pRec->bit_mask |= Fin_SGSNPDPRecord_routingArea_present;
	}
	if(m_pOrgRec->bit_mask & Org_SGSNPDPRecord_locationAreaCode_present)
	{
		pRec->bit_mask |= Fin_SGSNPDPRecord_locationAreaCode_present;
	}
	if(m_pOrgRec->bit_mask & Org_SGSNPDPRecord_cellIdentity_present)
	{
		pRec->bit_mask |= Fin_SGSNPDPRecord_cellIdentity_present;
	}
	if(m_pOrgRec->bit_mask & Org_SGSNPDPRecord_sgsnChange_present)
	{
		pRec->bit_mask |= Fin_SGSNPDPRecord_sgsnChange_present;
	}
	if (m_pOrgRec->bit_mask & Org_SGSNPDPRecord_recordSequenceNumber_present)
	{
		pRec->bit_mask |= Fin_SGSNPDPRecord_recordSequenceNumber_present;
	}
	if (m_pOrgRec->bit_mask & Org_SGSNPDPRecord_nodeID_present)
	{
		pRec->bit_mask |= Fin_SGSNPDPRecord_nodeID_present;
	}
	if (m_pOrgRec->bit_mask & Org_SGSNPDPRecord_recordExtensions_present)
	{
		pRec->bit_mask |= Fin_SGSNPDPRecord_recordExtensions_present;
	}
    //by ldf ��Ӧ���ⵥ D20357
    //servedMSISDN,�����Ѿ���Ϊoptional
    if (m_pOrgRec->bit_mask & Org_SGSNPDPRecord_servedMSISDN_present)
    {
        pRec->bit_mask |= Fin_SGSNPDPRecord_servedMSISDN_present;
    }
    //end
    //by mms��Ӧ���ⵥ SWOD00063
    //apnSelectionMode,�����Ѿ���Ϊoptional
    if (m_pOrgRec->bit_mask & Org_SGSNPDPRecord_apnSelectionMode_present)
    {
        pRec->bit_mask |= Fin_SGSNPDPRecord_apnSelectionMode_present;
    }
    //end
	//added by ggf,2003-11-19, ���������sgsnPLMNIdentifier��
	if (m_pOrgRec->bit_mask & Org_sgsnPLMNIdentifier_present)
	{
		pRec->bit_mask |= Fin_sgsnPLMNIdentifier_present;
	}
	//end
	//ֱ�ӿ���recordType��networkInitiation��anonymousAccessIndicator��
	//servedIMSI��servedIMEI�⼸��������
	pBegin = (char* )&(m_pOrgRec->recordType);
	pEnd = (char* )&(m_pOrgRec->sgsnAddress);
	memcpy(&(pRec->recordType), pBegin, pEnd - pBegin);

	//ת��sgsnAddress������
	Fin__seqof1_* pSGSNAddress = new Fin__seqof1_;
	if (NULL == pSGSNAddress)
	{
		throw "Fail to new sgsnAddress when prepare scdr data.";
	}
	pSGSNAddress->next = NULL;
	pSGSNAddress->value = (Fin_GSNAddress& )m_pOrgRec->sgsnAddress;
	pRec->sgsnAddress = pSGSNAddress;

	//ֱ�ӿ���msNetworkCapability��routingArea��locationAreaCode��
	//cellIdentity��chargingID��ggsnAddressUsed��accessPointNameNI��pdpType��
	//servedPDPAddress��listOfTrafficVolumes��recordOpeningTime��duration��
	//sgsnChange��causeForRecClosing�⼸��������
	pBegin = (char* )&(m_pOrgRec->msNetworkCapability);
	pEnd = (char* )&(m_pOrgRec->diagnostics);
	memcpy(&(pRec->msNetworkCapability), pBegin, pEnd - pBegin);
	m_pOrgRec->listOfTrafficVolumes = NULL;

	//ת��recordSequenceNumber������
	if (m_pOrgRec->bit_mask & Org_SGSNPDPRecord_recordSequenceNumber_present)
	{
		Fin_RecordNumberList_* pSeqNum = new Fin_RecordNumberList_;
		if (NULL == pSeqNum)
		{
			throw "Fail to new pSeqNum when prepare scdr data.";
		}
		Fin__seqof3* pRecSeqNode = new Fin__seqof3;
		if (NULL == pRecSeqNode)
		{
			throw "Fail to new recordSequenceNumber when prepare scdr data.";
		}


		pSeqNum->next = NULL;
		pSeqNum->value = m_pOrgRec->recordSequenceNumber;
		pRecSeqNode->next = NULL;
		pRecSeqNode->value.gsnAddress = (Fin_GSNAddress& )m_pOrgRec->sgsnAddress;
		pRecSeqNode->value.recordNumberList = pSeqNum;
		pRec->recordSequenceNumber = pRecSeqNode;

		m_pSequenceTail = pSeqNum;
		m_nSequenceLen = 1;
	}

	//ֱ�ӿ���nodeID,recordExtensions���������
	pBegin = (char* )&(m_pOrgRec->nodeID);
	pEnd = (char* )&(m_pOrgRec->localSequenceNumber);
	memcpy(&(pRec->nodeID), pBegin, pEnd - pBegin);
	m_pOrgRec->recordExtensions = NULL;

   
	//ֱ�ӿ���apnSelectionMode��accessPointNameOI��servedMSISDN��sgsnPLMNIdentifier�⼸��������Ľṹ
	pBegin = (char* )&(m_pOrgRec->apnSelectionMode);
	pEnd = (char* )m_pOrgRec + sizeof(Org_SGSNPDPRecord);
	memcpy(&(pRec->apnSelectionMode), pBegin, pEnd - pBegin);

    /*
		ת��consolidationResult�ֶ�:

		1.PDP����,���ݻ���ԭ������consolidationResult

		2.PDPδ����,����Fin_notNormal

     */
    switch (m_pOrgRec->causeForRecClosing)

    {
    case Fin_normalRelease:
        pRec->consolidationResult = Fin_normal;
		break;
    case Fin_sGSNChange:
        pRec->consolidationResult = Fin_forInterSGSNConsolidation;
        break;
    case Fin_abnormalRelease:
        pRec->consolidationResult = Fin_notNormal;
        break;
    default:            //PDPδ����(��ΪPDP�쳣����)
        pRec->consolidationResult = Fin_notNormal;
        break;
    }
	//���Ż���,�ý���ԭ��ΪonlyOneCDRGenerated.
	if (!IsPartial())
	{
		pRec->consolidationResult = Fin_onlyOneCDRGenerated;
	}

	m_pDestRec = pRec;
	m_pDestEventRec = pEventRec;

	//����ԭʼ�����Ŀռ�
	delete m_pOrgRec;
	m_pOrgRec = NULL;
	m_bDataPrepared = TRUE;
	return TRUE;
}


//���캯��
CGPRS_130_OSS_GCDR::CGPRS_130_OSS_GCDR()
{
	m_pSGSNTail = NULL;
	m_nSGSNLen = 0;
	m_pPDPTail = NULL;
	m_nPDPLen = 0;
	m_pQosTail = NULL;
	m_nQosLen = 0;
	m_pSequenceTail = NULL;
	m_nSequenceLen = 0;
	m_pExtensionTail = NULL;
	m_nExtensionLen = 0;

	m_pOrgRec = NULL;
    m_pDestEventRec = NULL;
	m_pDestRec = NULL;
}

//��������
CGPRS_130_OSS_GCDR::~CGPRS_130_OSS_GCDR()
{
	if (NULL != m_pOrgRec)
	{
		assert((NULL == m_pDestEventRec) && !m_bDataPrepared);

		//��������ṹ�а���������
		if (NULL != m_pOrgRec->sgsnAddress)
		{
			ClearList<Org__seqof2>(m_pOrgRec->sgsnAddress);
			m_pSGSNTail = NULL;
			m_nSGSNLen = 0;
		}
		if ((m_pOrgRec->bit_mask & Org_remotePDPAddress_present)
			 && (NULL != m_pOrgRec->remotePDPAddress))
		{
			ClearList<Org__seqof3>(m_pOrgRec->remotePDPAddress);
			m_pPDPTail = NULL;
			m_nPDPLen = 0;
		}
		if (NULL != m_pOrgRec->listOfTrafficVolumes)
		{
			ClearList<Org__seqof1_>(m_pOrgRec->listOfTrafficVolumes);
			m_pQosTail = NULL;
			m_nQosLen = 0;
		}
		if ((m_pOrgRec->bit_mask & Org_GGSNPDPRecord_recordExtensions_present)
			 && (NULL != m_pOrgRec->recordExtensions))
		{
			ClearExtensionsList(m_pOrgRec->recordExtensions);
			m_pExtensionTail = NULL;
			m_nExtensionLen = 0;
		}

		delete m_pOrgRec;
		m_pOrgRec = NULL;
	}

    if (NULL != m_pDestEventRec)
    {
		assert((NULL == m_pOrgRec) && m_bDataPrepared);

		//��������ṹ�а���������
		if (NULL != m_pDestRec->sgsnAddress)
		{
			ClearList<Fin__seqof1_>(m_pDestRec->sgsnAddress);
			m_pSGSNTail = NULL;
			m_nSGSNLen = 0;
		}
		if ((m_pDestRec->bit_mask & Fin_remotePDPAddress_present)
			 && (NULL != m_pDestRec->remotePDPAddress))
		{
			ClearList<Fin__seqof4>(m_pDestRec->remotePDPAddress);
			m_pPDPTail = NULL;
			m_nPDPLen = 0;
		}
		if (NULL != m_pDestRec->listOfTrafficVolumes)
		{
			ClearList<Fin__seqof2_>(m_pDestRec->listOfTrafficVolumes);
			m_pQosTail = NULL;
			m_nQosLen = 0;
		}
		if ((m_pDestRec->bit_mask & Fin_GGSNPDPRecord_recordSequenceNumber_present)
			 && (NULL != m_pDestRec->recordSequenceNumber.recordNumberList))
		{
			ClearList<Fin_RecordNumberList_>(m_pDestRec->recordSequenceNumber.recordNumberList);
			m_pSequenceTail = NULL;
			m_nSequenceLen = 0;
		}
		if ((m_pDestRec->bit_mask & Fin_GGSNPDPRecord_recordExtensions_present)
			 && (NULL != m_pDestRec->recordExtensions))
		{
			ClearExtensionsList((Org_ManagementExtensions_*& )m_pDestRec->recordExtensions);
			m_pExtensionTail = NULL;
			m_nExtensionLen = 0;
		}

		delete m_pDestEventRec;
		m_pDestEventRec = NULL;
		m_pDestRec = NULL;
	}
}

/**********************************************************************
��������    ��ʼ��CBill����
�������    pBuf����������¼������
			uLen��������������
�������    ��
����ֵ      ��ʼ���Ľ��
                TRUE----��ʼ���ɹ�
                FALSE----��ʼ��ʧ��
�׳��쳣    �ڴ����ʧ���׳��쳣
**********************************************************************/
BOOL CGPRS_130_OSS_GCDR::Init(char* pBuf,UINT4 uLen)
{
	assert((NULL != pBuf) && (0 != uLen));

	if (uLen != sizeof(Org_GGSNPDPRecord))
	{
		return FALSE;
	}

	m_pOrgRec = new Org_GGSNPDPRecord;
	if (NULL == m_pOrgRec)
	{
		throw "Fail to new Org_GGSNPDPRecord when init.";
	}

	memcpy(m_pOrgRec, pBuf, uLen);

	//�������ݳ�ʼ��sgsnAddress������صĳ�Ա����
	CloneList<Org__seqof2>(((Org_GGSNPDPRecord* )pBuf)->sgsnAddress, 
							m_pOrgRec->sgsnAddress);
	m_nSGSNLen = TravelList<Org__seqof2>(m_pOrgRec->sgsnAddress, 
										 (Org__seqof2*& )m_pSGSNTail);

	//�������ݳ�ʼ��remotePDPAddress������صĳ�Ա����
	if (m_pOrgRec->bit_mask & Org_remotePDPAddress_present)
	{
		CloneList<Org__seqof3>(((Org_GGSNPDPRecord* )pBuf)->remotePDPAddress, 
							   m_pOrgRec->remotePDPAddress);
		m_nPDPLen = TravelList<Org__seqof3>(m_pOrgRec->remotePDPAddress, 
											(Org__seqof3*& )m_pPDPTail);
	}

	//�������ݳ�ʼ��listOfTrafficVolumes������صĳ�Ա����
	CloneList<Org__seqof1_>(((Org_GGSNPDPRecord* )pBuf)->listOfTrafficVolumes, 
							m_pOrgRec->listOfTrafficVolumes);
	m_nQosLen = TravelList<Org__seqof1_>(m_pOrgRec->listOfTrafficVolumes, 
										 (Org__seqof1_*& )m_pQosTail);

	//�������ݳ�ʼ��recordExtensions������صĳ�Ա����
	if (m_pOrgRec->bit_mask & Org_GGSNPDPRecord_recordExtensions_present)
	{
		CloneExtensionsList(((Org_GGSNPDPRecord* )pBuf)->recordExtensions, 
							m_pOrgRec->recordExtensions);
		m_nExtensionLen = TravelList<Org_ManagementExtensions_>(m_pOrgRec->recordExtensions, 
											(Org_ManagementExtensions_*& )m_pExtensionTail);
	}
    else  //������ 2002-04-16
	{
		m_pOrgRec->recordExtensions = NULL;
	}
	return TRUE;
}

/**********************************************************************
��������    �жϵ�ǰ�����Ƿ�������pBill�ϲ�
�������    pBill��������ָ��
�������    ��
����ֵ      �Ƿ��ܹ��ϲ�
                TRUE----�ܹ��ϲ�
                FALSE----���ܺϲ�
˵��: 1����pBillָ��Ļ������ظ�����������TRUE���Ա���
          Merge�����ж����û�����
      2����pBillָ��Ļ������кŲ���������Ϊ���ܹ��ϲ���
      3����pBill����Ҫ���ӵ��ֶγ��ȹ�������Ϊ���ܹ��ϲ���
**********************************************************************/
BOOL CGPRS_130_OSS_GCDR::CanSafeMerge(CBill* pBill)
{
	assert(NULL != pBill);

	//����ȷ����ǰ�����Ѿ�ת��Ϊ���ջ�����ʽ
    if (!m_bDataPrepared)
    {
		BOOL bRes = PrepareData();
        if (bRes != TRUE)  
		{
			return FALSE;
		}
    }

	CGPRS_130_OSS_GCDR* pGCDR = (CGPRS_130_OSS_GCDR* )pBill;

	//���������ǲ��ֻ�����recordSequenceNumber�������Ч
	if (!(pGCDR->GetOrgRec()->bit_mask & Org_GGSNPDPRecord_recordSequenceNumber_present)
		 || !(m_pDestRec->bit_mask & Fin_GGSNPDPRecord_recordSequenceNumber_present))
	{

		return FALSE;
	}

    //�ظ�������Merge�����ж���
	if (IsDuplicated(pGCDR))
	{


		return TRUE;
	}

	//�������ظ���������Ҫ�����к�����
	if (pGCDR->GetOrgRec()->recordSequenceNumber != m_pSequenceTail->value + 1)
	{

		return FALSE;
	}

	//�ж��Ƿ񳬹��������ޡ�
	//�������������ޣ����жϺϲ��������úϲ����״̬ΪReachLimit
	if (m_nSGSNLen + pGCDR->GetSGSNLen() > MAX_MERGED_BILLS)
	{

		m_pDestRec->consolidationResult = Fin_reachLimit;
		return FALSE;
	}
	if ((pGCDR->GetOrgRec()->bit_mask & Org_remotePDPAddress_present) 
		 && (m_pDestRec->bit_mask & Fin_remotePDPAddress_present))
	{


		if (m_nPDPLen + pGCDR->GetPDPLen() > MAX_MERGED_BILLS)
		{

			m_pDestRec->consolidationResult = Fin_reachLimit;
			return FALSE;
		}
	}
	if (m_nQosLen + pGCDR->GetQosLen() > MAX_MERGED_BILLS)
	{

		
		m_pDestRec->consolidationResult = Fin_reachLimit;
		return FALSE;
	}
	if (m_nSequenceLen + 1 > MAX_MERGED_BILLS)
	{


		m_pDestRec->consolidationResult = Fin_reachLimit;
		return FALSE;
	}
	if ((pGCDR->GetOrgRec()->bit_mask & Org_GGSNPDPRecord_recordExtensions_present) 
		 && (m_pDestRec->bit_mask & Fin_GGSNPDPRecord_recordExtensions_present))
	{


		if (m_nExtensionLen + pGCDR->GetExtensionLen() > MAX_MERGED_BILLS)
		{


			m_pDestRec->consolidationResult = Fin_reachLimit;
			return FALSE;
		}
	}

	return TRUE;
}

/**********************************************************************
��������    ��pBill���뵱ǰ������
�������    pBill���������뻰�������ָ��
�������    ��
����ֵ      �ϲ��Ľ��
                TRUE----�ϲ��ɹ�
                FALSE----�ϲ�ʧ��
�׳��쳣    �ڴ����ʧ���׳��쳣
**********************************************************************/
BOOL CGPRS_130_OSS_GCDR::Merge(CBill* pBill)
{
	assert((NULL != pBill) && pBill->IsPartial());

	//����ȷ����ǰ�����Ѿ�ת��Ϊ���ջ�����ʽ
    if (!m_bDataPrepared)
    {
		BOOL bRes = PrepareData();
        if (bRes != TRUE)  
		{
			return FALSE;
		}
    }

	CGPRS_130_OSS_GCDR* pGCDR = (CGPRS_130_OSS_GCDR* )pBill;

	//�����ظ�����
	if (IsDuplicated(pGCDR) == TRUE)
	{


		return TRUE;
	}

    //����sgsnAddress�ֶΣ����޳���������ͬ��sgsnAddress
	if (NULL != pGCDR->GetOrgRec()->sgsnAddress)
	{
		int nSGSNLen = pGCDR->GetSGSNLen();
		Fin__seqof1_* pHead = NULL;
		Fin__seqof1_* pTail = NULL;
		pGCDR->GetSGSNList(pHead, pTail);

		if (NULL == m_pSGSNTail)
		{
			m_pDestRec->sgsnAddress = pHead;
		}
		else
		{
			//ɾ��������ͷ���ظ��ڵ�
			if (Match_gsnAddress((Org_GSNAddress* )&(m_pSGSNTail->value), 
								 (Org_GSNAddress* )&(pHead->value)))
			{
				Fin__seqof1_* pTmp = pHead->next;
				delete pHead;
				pHead = pTmp;
				nSGSNLen--;
			}

			m_pSGSNTail->next = pHead;
		}

		if (NULL != pHead)
		{
			m_pSGSNTail = pTail;
			m_nSGSNLen += nSGSNLen;
		}
		pGCDR->RemoveSGSNList();
	}

	//����remotePDPAddress�ֶ�
	if ((pGCDR->GetOrgRec()->bit_mask & Org_remotePDPAddress_present) 
		 && (m_pDestRec->bit_mask & Fin_remotePDPAddress_present)
		 && (NULL != pGCDR->GetOrgRec()->remotePDPAddress))
	{
		Fin__seqof4* pHead = NULL;
		Fin__seqof4* pTail = NULL;
		pGCDR->GetPDPList(pHead, pTail);

		if (NULL == m_pPDPTail)
		{
			m_pDestRec->remotePDPAddress = pHead;
		}
		else
		{
			m_pPDPTail->next = pHead;
		}

		m_pPDPTail = pTail;
		m_nPDPLen += pGCDR->GetPDPLen();
		pGCDR->RemovePDPList();
	}

	//����listOfTrafficVolumes�ֶ�
	if (NULL != pGCDR->GetOrgRec()->listOfTrafficVolumes)
	{
		Fin__seqof2_* pHead = NULL;
		Fin__seqof2_* pTail = NULL;
		pGCDR->GetQosList(pHead, pTail);

		if (NULL == m_pQosTail)
		{
			m_pDestRec->listOfTrafficVolumes = pHead;
		}
		else
		{
			m_pQosTail->next = pHead;
		}

		m_pQosTail = pTail;
		m_nQosLen += pGCDR->GetQosLen();
		pGCDR->RemoveQosList();
	}

	//����recordSequenceNumber�ֶ�
	if ((pGCDR->GetOrgRec()->bit_mask & Org_GGSNPDPRecord_recordSequenceNumber_present)
		 && (m_pDestRec->bit_mask & Fin_GGSNPDPRecord_recordSequenceNumber_present))
	{
		Fin_RecordNumberList_* pSeqNum = new Fin_RecordNumberList_;
		if (NULL == pSeqNum)
		{
			throw "Fail to new pSeqNum when merge GCDR.";
		}

		pSeqNum->next = NULL;
		pSeqNum->value = pGCDR->GetOrgRec()->recordSequenceNumber;

		//m_pSequenceTail�϶���ΪNULL
		m_pSequenceTail->next = pSeqNum;
		m_pSequenceTail = pSeqNum;
		m_nSequenceLen++;
	}

	//����recordExtensions�ֶ�
	if ((pGCDR->GetOrgRec()->bit_mask & Org_GGSNPDPRecord_recordExtensions_present)
		 && (m_pDestRec->bit_mask & Fin_GGSNPDPRecord_recordExtensions_present)
		 && (NULL != pGCDR->GetOrgRec()->recordExtensions))
	{
		Fin_ManagementExtensions_* pHead = NULL;
		Fin_ManagementExtensions_* pTail = NULL;
		pGCDR->GetExtensionList(pHead, pTail);

		if (NULL == m_pExtensionTail)
		{
			m_pDestRec->recordExtensions = pHead;
		}
		else
		{
			m_pExtensionTail->next = pHead;
		}

		m_pExtensionTail = pTail;
		m_nExtensionLen += pGCDR->GetExtensionLen();
		pGCDR->RemoveExtensionList();
	}

	//causeForRecClosing��ȡ���һ�Ż�����ֵ
	m_pDestRec->causeForRecClosing 
		 = (Fin_CauseForRecClosing)(pGCDR->GetOrgRec()->causeForRecClosing);

	//Z30971�����޸��Ƿ�֧����ʱ��
	if(!m_bDSTFlg)
	{
		//duration���ռ�����ʽ����
		Fin_CallDuration nDiff = GetDuration(&(m_pDestRec->recordOpeningTime), 
			&(pGCDR->GetOrgRec()->recordOpeningTime));
		m_pDestRec->duration = nDiff;
	}
	m_pDestRec->duration += pGCDR->GetOrgRec()->duration;
	//�޸Ľ���

	/*
	����consolidationResult�ֶ�:

	1.PDP����,���ݻ���ԭ������consolidationResult

	2.PDPδ����,������Ϊ�ϲ������������,����consolidationResult
	  ΪFin_reachLimit,����ԭ������Fin_notNormal
	*/
    switch (pGCDR->GetOrgRec()->causeForRecClosing)
	{
    case Fin_normalRelease:             //�����ͷ�
        m_pDestRec->consolidationResult = Fin_normal;
		break;
    case Fin_sGSNChange:                //SGSN�ı�
        m_pDestRec->consolidationResult = Fin_forInterSGSNConsolidation;
        break;
    case Fin_abnormalRelease:           //PDP�쳣����
        m_pDestRec->consolidationResult = Fin_notNormal;
        break;
    default:                            //PDPδ����(��ΪPDP�쳣����)
        m_pDestRec->consolidationResult = Fin_notNormal;
        break;
    }

    return TRUE;
}

/**********************************************************************
��������    �жϵ�ǰ����������PDP�Ƿ��Ѿ�����
�������    ��
�������    ��
����ֵ      PDP�Ƿ��Ѿ�����
                TRUE----PDP�Ѿ�����
                FALSE----PDPδ����
˵��:       ����������ֹԭ��Ϊ�������ͷš�����SGSN�ı䡱��
           ��PDP�쳣������˵��PDP�Ѿ�����������δ������
**********************************************************************/
BOOL CGPRS_130_OSS_GCDR::IsPDPCompleted()
{
	assert(NULL != m_pDestRec);

	switch (m_pDestRec->causeForRecClosing)
	{
	case Fin_normalRelease:
	case Fin_abnormalRelease:
		return TRUE;
	
	default:
		break;
	}

    //���Ӱ�ͨ��ʱ�������ϲ� ZhengYuqun 2003-12-18
    if((UINT4)m_pDestRec->duration >= m_uMaxDuration)
    {
        return TRUE;
    }

	return FALSE;
}

//�ж��Ƿ���Ҫ�����ȴ������������ڴ�ʼ�շ���FALSE
BOOL CGPRS_130_OSS_GCDR::NeedWaiting()
{
	return FALSE;
}

//�������ո�ʽ�Ļ�����¼��
char* CGPRS_130_OSS_GCDR::GetContents()
{
    if (m_bDataPrepared != TRUE)
    {
	    if (PrepareData() == FALSE)
		{
			return NULL;
		}
    }

	return (char* )m_pDestEventRec;
}

//�ж��Ƿ񲿷ֻ���,���recordSequenceNumber����Ч��Ϊ���ֻ���
BOOL CGPRS_130_OSS_GCDR::IsPartial()
{
	return (m_pOrgRec->bit_mask & Org_GGSNPDPRecord_recordSequenceNumber_present);
}

//�ж��Ƿ�PDP�ĵ�һ�Ų��ֻ���,
//��Ϊ���ֻ�����recordSequenceNumberΪ0��Ϊ��һ�Ų��ֻ���
BOOL CGPRS_130_OSS_GCDR::IsFirstPartialRec()
{ 
	if (!IsPartial())
	{
		return FALSE;
	}

	return (0 == m_pOrgRec->recordSequenceNumber);
}

//�ж��Ƿ�PDP��ĩ�Ų��ֻ���
BOOL CGPRS_130_OSS_GCDR::IsLastPartialRec()
{
	assert(NULL != m_pOrgRec);

	switch (m_pOrgRec->causeForRecClosing)
	{
	case Fin_normalRelease:
	case Fin_abnormalRelease:
		return TRUE;
	
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

//���ػ�������IDֵ
UINT4 CGPRS_130_OSS_GCDR::GetBillType()
{
	return GPRS_G_CDR;
}

//������������
//G-CDR������������2���ֶΣ�chargingID��ggsnAddressUsed
CKey CGPRS_130_OSS_GCDR::CreateKey()
{
    CGPRS_130_OSS_GCDR_Key* pKey = new CGPRS_130_OSS_GCDR_Key;
	if (NULL == pKey)
	{
		throw "Fail to new g-cdr key.";
	}
	if (NULL != m_pOrgRec)
	{
		pKey->SetChargingID(m_pOrgRec->chargingID);
		pKey->SetGGSNAddress(m_pOrgRec->ggsnAddress);
	}
	else if (NULL != m_pDestRec)
	{
		pKey->SetChargingID(m_pDestRec->chargingID);
		pKey->SetGGSNAddress((Org_GSNAddress& )(m_pDestRec->ggsnAddress));
	}
	else
	{
		assert(FALSE);
	}

    CKey key;
    key.m_pDelegation = pKey;

    return key;
}

//���sgsnAddress����ĳ���
int CGPRS_130_OSS_GCDR::GetSGSNLen()
{
	return m_nSGSNLen;
}

//���remotePDPAddress����ĳ���
int CGPRS_130_OSS_GCDR::GetPDPLen()
{
	return m_nPDPLen;
}

//���listOfTrafficVolumes����ĳ���
int CGPRS_130_OSS_GCDR::GetQosLen()
{
	return m_nQosLen;
}

//���recordExtensions����ĳ���
int CGPRS_130_OSS_GCDR::GetExtensionLen()
{
	return m_nExtensionLen;
}

//���sgsnAddress�����ͷ��β
void CGPRS_130_OSS_GCDR::GetSGSNList(Fin__seqof1_*& pHead, Fin__seqof1_*& pTail)
{
	if (NULL != m_pOrgRec)
	{
		pHead = (Fin__seqof1_* )(m_pOrgRec->sgsnAddress);
	}
	else if (NULL != m_pDestRec)
	{
		pHead = m_pDestRec->sgsnAddress;
	}
	else
	{
		assert(FALSE);
		pHead = NULL;
	}

	pTail = m_pSGSNTail;
}

//���remotePDPAddress�����ͷ��β
void CGPRS_130_OSS_GCDR::GetPDPList(Fin__seqof4*& pHead, Fin__seqof4*& pTail)
{ 
	if (NULL != m_pOrgRec)
	{
		pHead = (Fin__seqof4* )(m_pOrgRec->remotePDPAddress);
	}
	else if (NULL != m_pDestRec)
	{
		pHead = m_pDestRec->remotePDPAddress;
	}
    else
	{
		assert(FALSE);
		pHead = NULL;
	}
	
	pTail = m_pPDPTail;
}

//���listOfTrafficVolumes�����ͷ��β
void CGPRS_130_OSS_GCDR::GetQosList(Fin__seqof2_*& pHead, Fin__seqof2_*& pTail)
{
	if (NULL != m_pOrgRec)
	{
		pHead = (Fin__seqof2_* )(m_pOrgRec->listOfTrafficVolumes);
	}
	else if (NULL != m_pDestRec)
	{
		pHead = m_pDestRec->listOfTrafficVolumes;
	}
	else
	{
		assert(FALSE);
		pHead = NULL;
	}

	pTail = m_pQosTail;
}

//���recordExtensions�����ͷ��β
void CGPRS_130_OSS_GCDR::GetExtensionList(Fin_ManagementExtensions_*& pHead, 
										  Fin_ManagementExtensions_*& pTail)
{
	if (NULL != m_pOrgRec)
	{
		pHead = (Fin_ManagementExtensions_* )(m_pOrgRec->recordExtensions);
	}
	else if (NULL != m_pDestRec)
	{
		pHead = m_pDestRec->recordExtensions;
	}
	else
	{
		assert(FALSE);
		pHead = NULL;
	}

	pTail = m_pExtensionTail;
}

//ȥ��sgsnAddress���У��ڱ��������ϲ����������������
void CGPRS_130_OSS_GCDR::RemoveSGSNList()
{
	//������ֻ�ܶ�ԭʼ�������в���
	assert(NULL != m_pOrgRec);

	m_pOrgRec->sgsnAddress = NULL;
	m_pSGSNTail = NULL;
	m_nSGSNLen = 0;
}

//ȥ��remotePDPAddress���У��ڱ��������ϲ����������������
void CGPRS_130_OSS_GCDR::RemovePDPList()
{
	//������ֻ�ܶ�ԭʼ�������в���
	assert(NULL != m_pOrgRec);

	m_pOrgRec->remotePDPAddress = NULL;
	m_pPDPTail = NULL;
	m_nPDPLen = 0;
}

//ȥ��listOfTrafficVolumes���У��ڱ��������ϲ����������������
void CGPRS_130_OSS_GCDR::RemoveQosList()
{
	//������ֻ�ܶ�ԭʼ�������в���
	assert(NULL != m_pOrgRec);

	m_pOrgRec->listOfTrafficVolumes = NULL;
	m_pQosTail = NULL;
	m_nQosLen = 0;
}

//ȥ��recordExtensions���У��ڱ��������ϲ����������������
void CGPRS_130_OSS_GCDR::RemoveExtensionList()
{
	//������ֻ�ܶ�ԭʼ�������в���
	assert(NULL != m_pOrgRec);

	m_pOrgRec->recordExtensions = NULL;
	m_pExtensionTail = NULL;
	m_nExtensionLen = 0;
}

//��ȡԭʼ�����ṹ��ָ��
const Org_GGSNPDPRecord* CGPRS_130_OSS_GCDR::GetOrgRec()
{
	assert(NULL != m_pOrgRec);

	return m_pOrgRec;
}

//�ж��Ƿ��ظ�����
BOOL CGPRS_130_OSS_GCDR::IsDuplicated(CBill* pBill)
{
    if(m_pDestRec == NULL)
    {
        return FALSE;
    }
    
    CGPRS_130_OSS_GCDR* pGCDR = (CGPRS_130_OSS_GCDR* )pBill;
    int nHeadSeq, nTailSeq, nCurSeq;
    //��ǰ���ջ�������ʼ��ˮ��
    nHeadSeq = m_pDestRec->recordSequenceNumber.recordNumberList->value;
    //��ǰ���ջ����������ˮ��
    nTailSeq = m_pSequenceTail->value;
    //��ǰ���ϲ���������ˮ��
    nCurSeq = pGCDR->GetOrgRec()->recordSequenceNumber;
    
    //�����������к����Ѿ��ϲ��Ļ�����Χ�ڣ�˵�����ظ�������
    return ((nCurSeq >= nHeadSeq) && (nCurSeq <= nTailSeq));
}
/**********************************************************************
��������    ��ԭʼ������¼ת��Ϊ�������ո�ʽ�Ļ�����¼
�������    ��
�������    ��
����ֵ      ת���Ƿ�ɹ�
                TRUE----ת���ɹ�
                FALSE----ת�����ɹ�
**********************************************************************/
BOOL CGPRS_130_OSS_GCDR::PrepareData()
{
	assert(NULL != m_pOrgRec);

	char* pBegin = NULL, *pEnd = NULL;
	Fin_CallEventRecord* pEventRec = new Fin_CallEventRecord;
	if (NULL == pEventRec)
	{
		throw "Fail to new Fin_CallEventRecord when prepare gcdr data.";
	}
	pEventRec->choice = Fin_ggsnPDPRecord_chosen;
	Fin_GGSNPDPRecord* pRec 
		 = (Fin_GGSNPDPRecord* )&(pEventRec->u.ggsnPDPRecord);
	memset(pRec, 0, sizeof(Fin_GGSNPDPRecord));

	//ת��bit_mask�򣬺ϲ�ǰ���bit_mask�򳤶Ȳ�һ��
	if (m_pOrgRec->bit_mask & Org_GGSNPDPRecord_networkInitiation_present)
	{
		pRec->bit_mask |= Fin_GGSNPDPRecord_networkInitiation_present;
	}
	if (m_pOrgRec->bit_mask & Org_GGSNPDPRecord_anonymousAccessIndicator_present)
	{
		pRec->bit_mask |= Fin_GGSNPDPRecord_anonymousAccessIndicator_present;
	}
	if (m_pOrgRec->bit_mask & Org_remotePDPAddress_present)
	{
		pRec->bit_mask |= Fin_remotePDPAddress_present;
	}
	if (m_pOrgRec->bit_mask & Org_dynamicAddressFlag_present)
	{
		pRec->bit_mask |= Fin_dynamicAddressFlag_present;
	}
	if (m_pOrgRec->bit_mask & Org_GGSNPDPRecord_recordSequenceNumber_present)
	{
		pRec->bit_mask |= Fin_GGSNPDPRecord_recordSequenceNumber_present;
	}
	if (m_pOrgRec->bit_mask & Org_GGSNPDPRecord_nodeID_present)
	{
		pRec->bit_mask |= Fin_GGSNPDPRecord_nodeID_present;
	}
	if (m_pOrgRec->bit_mask & Org_GGSNPDPRecord_recordExtensions_present)
	{
		pRec->bit_mask |= Fin_GGSNPDPRecord_recordExtensions_present;
	}
    //by ldf ��Ӧ���ⵥ D20357 2003-02-18
    //servedMSISDN,������Ѿ���Ϊoption��
    if (m_pOrgRec->bit_mask & Org_GGSNPDPRecord_servedMSISDN_present)
    {
        pRec->bit_mask |= Fin_GGSNPDPRecord_servedMSISDN_present;
    }
    //end
    //by mms��Ӧ���ⵥ SWOD00063
    //apnSelectionMode,�����Ѿ���Ϊoptional
    if (m_pOrgRec->bit_mask & Org_GGSNPDPRecord_apnSelectionMode_present)
    {
        pRec->bit_mask |= Fin_GGSNPDPRecord_apnSelectionMode_present;
    }
    //end
	//added by ggf,2003-11-19 ���sgsnPLMNIdentifier��
	if (m_pOrgRec->bit_mask & Org_sgsnPLMNIdentifier_present)
	{
		pRec->bit_mask |= Fin_sgsnPLMNIdentifier_present;
	}
	//end
	//ֱ�ӿ���recordType��networkInitiation��anonymousAccessIndicator��
	//servedIMSI��ggsnAddress��chargingID��sgsnAddress��accessPointNameNI��
	//pdpType��servedPDPAddress��remotePDPAddress��dynamicAddressFlag��
	//listOfTrafficVolumes��recordOpeningTime��duration��
	//�⼸��������
	pBegin = (char* )&(m_pOrgRec->recordType);
	pEnd = (char* )&(m_pOrgRec->diagnostics);
	memcpy(&(pRec->recordType), pBegin, pEnd - pBegin);
	//m_pOrgRec->sgsnAddress = NULL; //������ɾ��
	m_pOrgRec->remotePDPAddress = NULL;
	m_pOrgRec->listOfTrafficVolumes = NULL;

	//ת��recordSequenceNumber������
	if (m_pOrgRec->bit_mask & Org_GGSNPDPRecord_recordSequenceNumber_present)
	{
		Fin_RecordNumberList_* pSeqNum = new Fin_RecordNumberList_;
		if (NULL == pSeqNum)
		{
			throw "Fail to new pSeqNum when prepare gcdr data.";
		}

		pSeqNum->next = NULL;
		pSeqNum->value = m_pOrgRec->recordSequenceNumber;
		if (NULL != m_pOrgRec->sgsnAddress)
		{
			Fin_IPAddress *pFin = &(pRec->recordSequenceNumber.gsnAddress);
			Org_IPAddress *pOrg = &(m_pOrgRec->sgsnAddress->value);
			memcpy((char *)pFin,(char *)pOrg,sizeof(Fin_IPAddress));
		}
		pRec->recordSequenceNumber.recordNumberList = pSeqNum;
        m_pOrgRec->sgsnAddress = NULL;//������ ����
		m_pSequenceTail = pSeqNum;
		m_nSequenceLen = 1;
	}

	//ֱ�ӿ���nodeID��recordExtensions�⼸��������
	pBegin = (char* )&(m_pOrgRec->nodeID);
	pEnd = (char* )&(m_pOrgRec->localSequenceNumber);
	//������ 2002-04-18 pEnd - pEnd ==> pEnd - pBegin
	memcpy(&(pRec->nodeID), pBegin, pEnd - pBegin);
	m_pOrgRec->recordExtensions = NULL;

    //ֱ�ӿ���apnSelectionMode��servedMSISDN�⼸��������
	pBegin = (char* )&(m_pOrgRec->apnSelectionMode);
	pEnd = (char* )m_pOrgRec + sizeof(Org_GGSNPDPRecord);
	memcpy(&(pRec->apnSelectionMode), pBegin, pEnd - pBegin);
	
    /*
		ת��consolidationResult�ֶ�:

		1.PDP����,���ݻ���ԭ������consolidationResult

		2.PDPδ����,����Fin_notNormal

     */
    switch (m_pOrgRec->causeForRecClosing)
    {
    case Fin_normalRelease:
        pRec->consolidationResult = Fin_normal;
		break;
    case Fin_sGSNChange:
        pRec->consolidationResult = Fin_forInterSGSNConsolidation;
        break;
    case Fin_abnormalRelease:
        pRec->consolidationResult = Fin_notNormal;
        break;
    default:            //PDPδ����(��ΪPDP�쳣����)
        pRec->consolidationResult = Fin_notNormal;
        break;
    }
	//���Ż���,�ý���ԭ��ΪonlyOneCDRGenerated.
	if (!IsPartial())
	{
		pRec->consolidationResult = Fin_onlyOneCDRGenerated;
	}

	m_pDestRec = pRec;
	m_pDestEventRec = pEventRec;

	//����ԭʼ�����Ŀռ�
	delete m_pOrgRec;
	m_pOrgRec = NULL;

	m_bDataPrepared = TRUE;

	return TRUE;
}


//���캯��
CGPRS_130_OSS_SCDR_Key::CGPRS_130_OSS_SCDR_Key()
{
}

//��������
CGPRS_130_OSS_SCDR_Key::~CGPRS_130_OSS_SCDR_Key()
{
}

//����sgsn��ַ
void CGPRS_130_OSS_SCDR_Key::SetSGSNAddress(const Org_GSNAddress& sgsnAddress)
{
	m_sgsnAddress = sgsnAddress;
}

//����ggsn��ַ
void CGPRS_130_OSS_SCDR_Key::SetGGSNAddress(const Org_GSNAddress& ggsnAddress)
{
	m_ggsnAddressUsed = ggsnAddress;
}

//����chargingID
void CGPRS_130_OSS_SCDR_Key::SetChargingID(const Org_ChargingID chargingID)
{
	m_chargingID = chargingID;
}

//���sgsn��ַ
const Org_GSNAddress& CGPRS_130_OSS_SCDR_Key::GetSGSNAddress()
{
	return m_sgsnAddress;
}

//���ggsn��ַ
const Org_GSNAddress& CGPRS_130_OSS_SCDR_Key::GetGGSNAddress()
{
	return m_ggsnAddressUsed;
}

//���chargingID
Org_ChargingID CGPRS_130_OSS_SCDR_Key::GetChargingID()
{
	return m_chargingID;
}

//�ж��Ƿ���ָ����pKey��ͬ
BOOL CGPRS_130_OSS_SCDR_Key::IsMatch(CKey* pKey)
{
	CGPRS_130_OSS_SCDR_Key* p = (CGPRS_130_OSS_SCDR_Key* )pKey;

	return (Match_gsnAddress(&(p->GetSGSNAddress()), &m_sgsnAddress)
		     && Match_gsnAddress(&(p->GetGGSNAddress()), &m_ggsnAddressUsed)
			 && (p->GetChargingID() == m_chargingID));
}

//���ؼ�ֵ
UINT4 CGPRS_130_OSS_SCDR_Key::GetKey()
{
	return m_chargingID;
}


//���캯��
CGPRS_130_OSS_GCDR_Key::CGPRS_130_OSS_GCDR_Key()
{
}

//��������
CGPRS_130_OSS_GCDR_Key::~CGPRS_130_OSS_GCDR_Key()
{
}

void CGPRS_130_OSS_GCDR_Key::SetGGSNAddress(const Org_GSNAddress& ggsnAddress)
{
	m_ggsnAddress = ggsnAddress;

	
}

void CGPRS_130_OSS_GCDR_Key::SetChargingID(const Org_ChargingID chargingID)
{
	m_chargingID = chargingID;
}

const Org_GSNAddress& CGPRS_130_OSS_GCDR_Key::GetGGSNAddress()
{
	return m_ggsnAddress;
}

Org_ChargingID CGPRS_130_OSS_GCDR_Key::GetChargingID()
{
	return m_chargingID;
}

BOOL CGPRS_130_OSS_GCDR_Key::IsMatch(CKey* pKey)
{
	CGPRS_130_OSS_GCDR_Key* p = (CGPRS_130_OSS_GCDR_Key* )pKey;

	return (Match_gsnAddress(&(p->GetGGSNAddress()), &m_ggsnAddress)
			  && (m_chargingID == p->GetChargingID()));
}

UINT4 CGPRS_130_OSS_GCDR_Key::GetKey()
{
	return m_chargingID;
}


//�Ƚ�����GSN��ַ
BOOL Match_gsnAddress(const Org_GSNAddress* p1, const Org_GSNAddress* p2)
{
	//������ ����
    if(p1 == NULL || p2 == NULL)
	{
		return FALSE;
	}
	//over
	if (p1->choice != p2->choice)
	{
		return FALSE;
	}
	
	int nResult;
	switch (p1->choice)
	{
	case Org_iPBinaryAddress_chosen:
		if (p1->u.iPBinaryAddress.choice != p2->u.iPBinaryAddress.choice)
		{
			return FALSE;
		}
		
		//���ݲ�ͬ��IP��ַ��ʾ����������IP��ַռ�õĿռ䣬���������ݵıȽ�
		if (Org_iPBinV4Address_chosen == p1->u.iPBinaryAddress.choice)
		{
			nResult = memcmp(&(p1->u.iPBinaryAddress.u.iPBinV4Address), 
							 &(p2->u.iPBinaryAddress.u.iPBinV4Address), 
							 sizeof(Org__octet1));
			if (0 != nResult)
			{
				return FALSE;
			}
		}
		else if (Org_iPBinV6Address_chosen == p1->u.iPBinaryAddress.choice)
		{
			nResult = memcmp(&(p1->u.iPBinaryAddress.u.iPBinV6Address), 
					 		 &(p2->u.iPBinaryAddress.u.iPBinV6Address), 
							 sizeof(Org__octet2));
			if (0 != nResult)
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}

		break;
		
	case Org_iPTextRepresentedAddress_chosen:
		if (p1->u.iPBinaryAddress.choice != p2->u.iPBinaryAddress.choice)
		{
			return FALSE;
		}
		
		//���ݲ�ͬ��IP��ַ��ʾ����������IP��ַռ�õĿռ䣬���������ݵıȽ�
		if (Org_iPTextV4Address_chosen == p1->u.iPTextRepresentedAddress.choice)
		{
			nResult = strcmp(p1->u.iPTextRepresentedAddress.u.iPTextV4Address, 
							 p2->u.iPTextRepresentedAddress.u.iPTextV4Address);
			if (0 != nResult)
			{
				return FALSE;
			}
		}
		else if (Org_iPTextV6Address_chosen == p1->u.iPTextRepresentedAddress.choice)
		{
			nResult = strcmp(p1->u.iPTextRepresentedAddress.u.iPTextV6Address, 
							 p2->u.iPTextRepresentedAddress.u.iPTextV6Address);
			if (0 != nResult)
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}

		break;

	default:
		return FALSE;
	
	}

	return TRUE;
}
