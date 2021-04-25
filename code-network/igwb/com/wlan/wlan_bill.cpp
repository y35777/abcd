/******************************************************************************
  Copyright (C), 2003-2011, Huawei Tech. Co., Ltd.

/******************************************************************************
  File Name       : wlan_bill.cpp
  Version         : 1.0
  Author          : Gan Zhiliang
  Created         : 2003-3-25
  Last Modified   : 
  Description     : WLAN �����ϲ���ʵ��
  Function List   : 
  History         : 
  1 Date          : 2003-3-25
    Author        : Gan Zhiliang
    Modification  : Created file
******************************************************************************/
#include "../include/merge.h"
#include "../include/toolbox.h"
#include "../format/format_comm.h"
#include "wlan_bill.h"
const INT8 MAX_DATA_FLOW = 9999999999;
const UINT4 MAX_DURATION = 999999;
#include "../include/export_dll.h"

extern "C" IGWB_Svc_Export void* CreateParserObject()
{
    return (void*)new CWLAN_BillParser();
}

/******************************************************
* ������: CWLAN_BillParser::CWLAN_BillParser()
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   ���캯��	
* ����:   ��
* ����ֵ: ��
* ˵��:  
* ���¼�¼: 
*
* ����			  ����					���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
CWLAN_BillParser::CWLAN_BillParser()
{
	m_pBill = NULL;
	
}
/******************************************************
* ������: CWLAN_BillParser::~CWLAN_BillParser()
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   ��������	
* ����:   ��
* ����ֵ: ��
* ˵��:  
* ���¼�¼: 
*
* ����			  ����					���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
CWLAN_BillParser::~CWLAN_BillParser()
{
	if(m_pBill)
	{
		delete m_pBill;
	}
	
}
/******************************************************
* ������: CBill* CWLAN_BillParser::GetBillRecord()
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   �õ�һ��������¼	
* ����:   ��
* ����ֵ: ��
* ˵��:  
* ���¼�¼: 
*
* ����			  ����					���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
CBill* CWLAN_BillParser::GetBillRecord()
{
	CBill* pBill = m_pBill;
	m_pBill = NULL;
	return pBill;
	
}
/******************************************************
* ������: BOOL CWLAN_BillParser::ParseARecord(char *pBuf, int nBufLen)
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   ����һ��������¼		
* ����:   Char *pBuf  һ�κ��м�¼�Ļ���Int nBufLen �������ĳ���
* ����ֵ: BOOL
* ˵��:  
* ���¼�¼: 
*
* ����			  ����					���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
BOOL CWLAN_BillParser::ParseARecord(char *pBuf, UINT4 nBufLen)
{
	m_pBill = new CWLANBill;
	
	if(m_pBill != NULL)
	{
		//�û�����¼��ʼ����������ʧ�ܽ��׳��쳣��
		if(m_pBill->Init(pBuf, sizeof(WLAN_ORIGIAL_CDR)))
		{
			nBufLen++;//Ϊ������nBuflenû�����õĸ澯
			return TRUE;
		}
		else
		{
			throw "WLAN_S_CDR initialize fail.";
			return FALSE;
		}
	}
	
	return FALSE;
	
}
/******************************************************
* ������: void CWLAN_BillParser::RegisterBillType(CMerger *pMerger)
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   ��CMerger����ע�Ự������ 	
* ����:   CMerger *pMerger�ϲ�����
* ����ֵ: ��
* ˵��:  
* ���¼�¼: 
*
* ����			  ����					���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
void CWLAN_BillParser::RegisterBillType(CMerger *pMerger)
{
	assert(pMerger != NULL);
	pMerger->RegisterBillType(WLAN_S_CDR);
	
}
/******************************************************
* ������: int CWLAN_BillParser::GetBillTypeID(int nIndex)
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   �õ��Ѿ�ע�Ự�����͵�ID ���� 	
* ����:   ��������
* ����ֵ: int
* ˵��:  
* ���¼�¼: 
*
* ����			  ����					���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
UINT4 CWLAN_BillParser::GetBillTypeID(UINT4 uIndex)
{
	uIndex++; //Ϊ������nIndexû�����õĸ澯
	return WLAN_S_CDR;
}
/******************************************************
* ������: int CWLAN_BillParser::GetBillTypeCount()
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   �õ��Ѿ�ע�Ự�����͵���Ŀ	
* ����:   ��
* ����ֵ: int
* ˵��:  
* ���¼�¼: 
*
* ����			  ����					���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
UINT4 CWLAN_BillParser::GetBillTypeCount()
{
	return 1;
}


/*******************************************************************************
  Function        : CWLAN_BillParser::GetBillTypeDesc
  Description     : ���ָ���Ļ�������ID�Ļ�����������
  Input           : UINT4 uBillTypeID
                  : char* szDesc
  Output          : 
  Return          : void
  Calls           : 
  Called by       : 
  
  History         : 
  1 Date          : 2003-4-21
    Author        : Gan Zhiliang
    Modification  : Created file
*******************************************************************************/
void CWLAN_BillParser::GetBillTypeDesc(UINT4 uBillTypeID, char* szDesc)
{
    switch (uBillTypeID)
    {
    case WLAN_S_CDR:
        strcpy(szDesc, "WLAN_S_CDR");
        break;
    default:
		sprintf(szDesc, "%d-Unknown", uBillTypeID);
        break;
    }
}

/******************************************************
* ������: CWLANBill::CWLANBill()
* ����:   Gan ZHiliang 
* ����:    2003-4-21
* ����:    ���캯��
* ����:    �ޡ�
* ����ֵ:  �ޡ�
* ˵��:    
* ���¼�¼: 
*
* ����			  ����					���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
CWLANBill::CWLANBill()
{
	m_pInProcessRec = NULL;
	m_pOrgRec = NULL;
	m_pSequenceTail = NULL;
	m_bDataPrepared = FALSE;
	
}

/******************************************************
* ������: CWLANBill::~CWLANBill()
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   �����������ͷŻ�����¼�ռ䡣
* ����:   �ޡ�
* ����ֵ:  �ޡ�
* ˵��:    �����������˺ϲ��������������Ҫ�ͷš�
* ���¼�¼: 
*
* ����			  ����					���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
CWLANBill::~CWLANBill()
{
	if(m_pInProcessRec != NULL)
	{
		
		//����ɾ������ڵ㡣
		if(m_pInProcessRec->pRecordSequenceNumber)
		{
			//	���к�ָ��һ����ΪNULL��			
			TWLANListRecordSequenceNumber *p = m_pInProcessRec->pRecordSequenceNumber;
			TWLANListRecordSequenceNumber *q = NULL;
			p = p->pNext;
			while(p)
			{
				q = p;
				p = p->pNext;
				if(q != NULL)
				delete q;
			}
			
			delete m_pInProcessRec->pRecordSequenceNumber;
		}		
		
		delete m_pInProcessRec;
		m_pInProcessRec = NULL;
	}
	
	if(m_pOrgRec != NULL)
	{
		delete m_pOrgRec;
		m_pOrgRec = NULL;
	}
}
/******************************************************
* ������: CWLANBill::IsPartial()
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   �ж�һ�Ż����Ƿ��ǲ��ֻ���
* ����:   �ޡ�
* ����ֵ:  BOOL
* ˵��:   ���ԭ��ֵΪ0xFE�������Ż����ǲ��ֻ���
* ���¼�¼: 
*
* ����			  ����			���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/

BOOL CWLANBill::IsPartial()
{
	assert(m_pOrgRec != NULL);
	if(m_pOrgRec->ucCloseCause == WLAN_PartialRec)
		return TRUE;
    
	if(NTOHL(m_pOrgRec->ulRecordSequenceNumber) != 1)
		return TRUE;
	else
		return FALSE;
}

/******************************************************
* ������: CWLANBill::IsFirstPartialRec()
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   �ж�һ�Ż����Ƿ��ǵ�һ�Ų��ֻ���
* ����:   �ޡ�
* ����ֵ:  BOOL
* ˵��:   ���ԭ��ֵΪ0xFE�����к���Ϊ1���������Ż����ǵ�һ�Ų��ֻ���
* ���¼�¼: 
*
* ����			  ����			���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
BOOL CWLANBill::IsFirstPartialRec()
{
	if(!IsPartial())
	{
		return FALSE;
	}

	return (NTOHL(m_pOrgRec->ulRecordSequenceNumber) == 1); 
	
}

/******************************************************
* ������: BOOL CWLANBill::IsLastPartialRec()
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   �ж�һ�Ż����ǲ�������һ������		   
* ����:   �ޡ�
* ����ֵ:  BOOL
* ˵��:    �жϻ���ԭ�򼴿ɡ�
* ���¼�¼: 
*      
*
* ����			  ����			���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
BOOL CWLANBill::IsLastPartialRec()
{
    return (m_pOrgRec->ucCloseCause != WLAN_PartialRec);
   
}

/******************************************************
* ������: BOOL CWLANBill::NeedWaiting()
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   �жϵ���������PDP�Ѿ�����ʱ���Ƿ���Ҫ�����ȴ�����������		 
* ����:   �ޡ�
* ����ֵ:  BOOL
* ˵��:   �˺���Ŀǰֵ��ΪFALSE
* ���¼�¼: 
*
* ����			  ����			���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
BOOL CWLANBill::NeedWaiting()
{
	return FALSE;
	
}

/******************************************************
* ������: BOOL CWLANBill::IsPDPCompleted()
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   �жϵ�ǰ����������PDP�Ƿ��Ѿ����� 	  
* ����:   �ޡ�
* ����ֵ:  BOOL����PDP�Ѿ��������򷵻�TRUE�����򷵻�FALSE��
* ˵��:  ����������ֹԭ��Ϊ�������ͷš�������ʱ�������쳣������
*		  ˵��PDP�Ѿ�����������δ������
* ���¼�¼: 
*    
*
* ����			  ����			���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
BOOL CWLANBill::IsPDPCompleted()
{
	assert(m_pInProcessRec != NULL);

    return (m_pInProcessRec->ucCloseCause != WLAN_PartialRec);
   
}

/******************************************************
* ������: CKey CWLANBill::CreateKey()
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   ����������������
* ����:   �ޡ�
* ����ֵ: CKey
* ˵��:  WLANBill�Ļ���������������chargingID��ulASaddress��������뿼���ֽ��������
* ���¼�¼: 
*
* ����			  ����			���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
CKey CWLANBill::CreateKey()
{
	CWLANBill_Key* pkey = new CWLANBill_Key;
	if(pkey == NULL)
	{
		throw "Create key fail.";
	}
	
	if(m_pOrgRec != NULL)
	{
		pkey->m_ulchargingID = NTOHL(m_pOrgRec->ulChargingID);
		pkey->m_ulASaddress = m_pOrgRec->ulASAddress;
	}
	else if(m_pInProcessRec != NULL)
	{
		pkey->m_ulchargingID = m_pInProcessRec->ulChargingID;
		pkey->m_ulASaddress =  m_pInProcessRec->ulASAddress;
	}
	else
	{
		assert(0);
	}
	
	CKey key;
	key.m_pDelegation = pkey;
	
	return key;
	
}

/******************************************************
* ������: int CWLANBill::GetBillType()
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   �õ�ע��Ļ������͵�ֵ	  
* ����:   �ޡ�
* ����ֵ: int
* ˵��:  
* ���¼�¼: 
*
* ����			  ����		���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
UINT4 CWLANBill::GetBillType()
{
	return WLAN_S_CDR;
	
}
/******************************************************
* ������: char* CWLANBill::GetContents()
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   �������ո�ʽ�Ļ�����¼	  
* ����:   �ޡ�
* ����ֵ: char *
* ˵��:  
* ���¼�¼: 
*
* ����			  ����		���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
char* CWLANBill::GetContents()
{
	if(!m_bDataPrepared)
	{
		if(!PrepareData())
		{
			return FALSE;
		}
	}
	//�Ѻϲ���Ľṹm_pInProcessRecת����������Ҫ������WLAN_FINAL_CDR
	//����ɾ������ڵ㡣
	if(m_pInProcessRec->pRecordSequenceNumber)
	{
		//	���к�ָ��һ����ΪNULL��			
		TWLANListRecordSequenceNumber *p = m_pInProcessRec->pRecordSequenceNumber,*q = NULL;
		p = p->pNext;
		while(p)
		{
			q = p;
			p = p->pNext;
			delete q;
		}
		
		delete m_pInProcessRec->pRecordSequenceNumber;
		m_pInProcessRec->pRecordSequenceNumber = NULL;

	}	

    //����Ǻڻ��������Ķ���ԭ��Ϊ�Ự��ʱ
    if (m_pInProcessRec->ucCloseCause == WLAN_PartialRec)
    {
        m_pInProcessRec->ucCloseCause = WLAN_SessionTimeout;
    }
    
	char *pBuffer = (char*)(&(m_pInProcessRec->ucRecordType));
	SerialDstBill(pBuffer);
	return pBuffer;
	
}

/******************************************************
* ������: BOOL CWLANBill::Init()
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   ��ʼ��CBill����	   
* ����:   �ޡ�
* ����ֵ: BOOL
* ˵��:  
* ���¼�¼: 
*
* ����			  ����		���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
BOOL CWLANBill::Init(char *pBuf, UINT4 nLen)
{
	assert(pBuf != NULL);	
	m_pOrgRec = new WLAN_ORIGIAL_CDR;
	if(m_pOrgRec == NULL)
	{
		throw "Memory alloc fail.";
	}
	SerialBillBuf(pBuf, m_pOrgRec);
	return TRUE;
	
}

/******************************************************
* ������: BOOL CWLANBill::PrepareData()
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   ��ԭʼ������¼ת��Ϊ�������պϲ�ʱҪ��Ļ�����¼		
* ����:   �ޡ�
* ����ֵ: BOOL
* ˵��:  
* ���¼�¼: 
*
* ����			  ����			���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
BOOL CWLANBill::PrepareData()
{
	assert(m_pOrgRec != NULL);
	WLAN_INPROCESS_CDR* pEventRec = new WLAN_INPROCESS_CDR;
	if(pEventRec == NULL)
	{
		throw "Memory alloc fail.";
	}	
	pEventRec->pRecordSequenceNumber = NULL;
	//ȡ��һ���ֶ�
	pEventRec->ucRecordType = m_pOrgRec->ucRecordType;
	pEventRec->ulLocalSequenceNumber = NTOHL(m_pOrgRec->ulLocalSequenceNumber);
	pEventRec->ulChargingID = NTOHL(m_pOrgRec->ulChargingID);
	//�ѵ�4���ֶ�ת��������
	if(NTOHL(m_pOrgRec->ulRecordSequenceNumber))
	{
		TWLANListRecordSequenceNumber *pSequenceList = new TWLANListRecordSequenceNumber;
		if(pSequenceList == NULL)
		{
			delete pEventRec;
			throw "Memory alloc fail.";
		}
		
		pSequenceList->value = NTOHL(m_pOrgRec ->ulRecordSequenceNumber);
		pSequenceList->pNext = NULL;
		pEventRec->pRecordSequenceNumber = pSequenceList;
		m_pSequenceTail = pSequenceList;
	}
	else
	{
		return FALSE;
	}

	//ȡ��5���ֶ��Ժ��8���ֶ�
	memcpy(&(pEventRec->ucOperType), &(m_pOrgRec->ucOperType), offsetof(WLAN_ORIGIAL_CDR, ulDuration) - offsetof(WLAN_ORIGIAL_CDR, ucOperType));
	
	//ȡͨ��ʱ��ulDuration
	pEventRec->ulDuration = NTOHL(m_pOrgRec->ulDuration);
	//ȡulDataFlowUp
    pEventRec->ulDataFlowUp = NTOHL(m_pOrgRec->ulDataFlowUp);
	//ȡulDataFlowDn
	pEventRec->ulDataFlowDn = NTOHL(m_pOrgRec->ulDataFlowDn);
	memcpy(pEventRec->ucHotSpotID, m_pOrgRec->ucHotSpotID, sizeof(pEventRec->ucHotSpotID));
	//ȡulACAddress
	pEventRec->ulACAddress = m_pOrgRec->ulACAddress;
	//ȡulASAddress
	pEventRec->ulASAddress = m_pOrgRec->ulASAddress; 

	memcpy(pEventRec->ucServiceID, m_pOrgRec->ucServiceID, sizeof(pEventRec->ucServiceID));
	memcpy(pEventRec->ucIspID, m_pOrgRec->ucIspID, sizeof(pEventRec->ucIspID));
	
    //ȡucCloseCause
	pEventRec->ucCloseCause = m_pOrgRec->ucCloseCause;
	
    //otp�汾, ����һ���ֶ�Domain��add by lucy 2004.5.25
    memcpy(pEventRec->ucDomain, m_pOrgRec->ucDomain, sizeof(m_pOrgRec->ucDomain));
	
	m_pInProcessRec = pEventRec;
	//����ԭʼ�����ռ䡣
	delete m_pOrgRec;
	m_pOrgRec = NULL;
	m_bDataPrepared = TRUE;
	return TRUE;
	
}

/******************************************************
* ������: BOOL CWLANBill::CanSafeMerge(CBill *pBill)
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   �ж����Ż�(��ͬ����)���Ƿ��ܹ��ϲ���		
* ����:   CBill *pBill
* ����ֵ: BOOL
* ˵��:
*1����pBillָ��Ļ������ظ�����������TRUE���Ա���Merge�����ж����û�����
*2����pBillָ��Ļ������кŲ���������Ϊ���ܹ��ϲ���
*3���ж������Ƿ��������������Ϊ���ܺϲ������������еĻ����ͻᳬʱ�رա�
*4 ���ж�ͨ��ʱ���Ƿ񳬹�999999?��������Ϊ�ǲ��ܹ��ϲ�����֮��Ϊ���Ժϲ��� 
* ���¼�¼: 
*
* ����			  ����	    	���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
BOOL CWLANBill::CanSafeMerge(CBill *pBill)
{
	assert(pBill != NULL);
	
	if(!m_bDataPrepared)
	{
		if(!PrepareData())
		{
			return FALSE;
		}
	}
	CWLANBill *p = (CWLANBill*)pBill;
	UINT4 No1,No2,No3;
	// recordSequenceNumber�����ͷ�ڵ�ֵ
	No1 = m_pInProcessRec->pRecordSequenceNumber->value;
	//recordSequenceNumber�����β�ڵ�ֵ��	  
	No2 = m_pSequenceTail->value;
	No3 = NTOHL(p->m_pOrgRec->ulRecordSequenceNumber);
	// �����������к����Ѿ��ϲ��Ļ�����Χ�ڣ�˵�����ظ�������
	//�ظ�������Merge�����ж�����
	if((No3 <= No2) && (No3 >= No1))
	{
		return TRUE;
	}
	// �������ظ���������Ҫ�����к�������
	if((No2 + 1) != No3)
	{
		return FALSE;
	}
	
	INT8 iDataFlow1,iDataFlow2;
	//�ж����������Ƿ�Խ��
	iDataFlow1 = m_pInProcessRec->ulDataFlowUp;
	iDataFlow2 = NTOHL(p->m_pOrgRec->ulDataFlowUp);
	if(iDataFlow2 > (MAX_DATA_FLOW - iDataFlow1))
	{
		m_pInProcessRec->ucCloseCause = WLAN_SessionTimeout;
		return FALSE;
	}
	//�ж����������Ƿ�Խ��	   
	iDataFlow1 = m_pInProcessRec->ulDataFlowDn;
	iDataFlow2 = NTOHL(p->m_pOrgRec->ulDataFlowDn);
	if(iDataFlow2 > (MAX_DATA_FLOW - iDataFlow1))
	{
		m_pInProcessRec->ucCloseCause = WLAN_SessionTimeout;
		return FALSE;
	}

	No1 = m_pInProcessRec->ulDuration;
	No2 = NTOHL(p->m_pOrgRec->ulDuration);
	if((No1 > (MAX_DURATION - No2))| (No1 > MAX_DURATION) |(No2 > MAX_DURATION))
	{
		m_pInProcessRec->ucCloseCause = WLAN_SessionTimeout;
		return FALSE;
	}
	
	return TRUE;
	
}
BOOL CWLANBill::IsDuplicated(CBill* pBill)
{
    WLAN_ORIGIAL_CDR* p = ((CWLANBill*)pBill)->m_pOrgRec;
    WLAN_INPROCESS_CDR *pRec = m_pInProcessRec;
    if((NTOHL(p->ulRecordSequenceNumber)
        >= pRec->pRecordSequenceNumber->value)
        && (NTOHL(p->ulRecordSequenceNumber) <= m_pSequenceTail->value))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/******************************************************
* ������: BOOL CWLANBill::Merge(CBill *pBill)
* ����:   Gan ZHiliang 
* ����:   2003-4-21
* ����:   �ϲ����Ż���		
* ����:   CBill *pBill
* ����ֵ: BOOL
* ˵��:
* ���¼�¼: 
*
* ����			  ����			���� 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
BOOL CWLANBill::Merge(CBill *pBill)
{
	assert(pBill != NULL);
	assert(pBill->IsPartial());
	
	if(!m_bDataPrepared)
	{
		if(!PrepareData())
		{
			return FALSE;
		}
	}
	WLAN_ORIGIAL_CDR* p = ((CWLANBill*)pBill)->m_pOrgRec;
	assert(p != NULL);
	WLAN_INPROCESS_CDR *pRec = m_pInProcessRec;   
	//�����ظ�����	 
	if((NTOHL(p->ulRecordSequenceNumber)
		>= pRec->pRecordSequenceNumber->value)
		&& (NTOHL(p->ulRecordSequenceNumber) <= m_pSequenceTail->value))
	{
		return TRUE;
	}
	
	//RecordSequenceNumber������ 
	
	if(NTOHL(p->ulRecordSequenceNumber))
	{
		TWLANListRecordSequenceNumber* pSequenceNum = new TWLANListRecordSequenceNumber;
		if(pSequenceNum == NULL)
		{
			throw "Memory alloc fail.";
		}		
		pSequenceNum->value = NTOHL(p->ulRecordSequenceNumber);
		pSequenceNum->pNext = NULL;
		
		m_pSequenceTail->pNext = pSequenceNum;
		m_pSequenceTail = pSequenceNum; 	  
	}	 
	
	//ucCloseCause�ֶ�ȡ���һ�Ż�����ֵ��	   
	pRec->ucCloseCause = p->ucCloseCause;
	//ucStopTime�ֶ�ȡ���һ�Ż�����ֵ
	memcpy(pRec->ucStopTime, p->ucStopTime, sizeof(p->ucStopTime));
	//strcpy((char *)pRec->ucStopTime,(char *) p->ucStopTime);
	//uldurationֱ�����
	pRec->ulDuration += NTOHL(p->ulDuration); 
	//�������
    pRec->ulDataFlowUp += NTOHL(p->ulDataFlowUp);
	pRec->ulDataFlowDn += NTOHL(p->ulDataFlowDn);
	return TRUE;
	
}


INT4 CWLANBill::GetLength()
{
	return WLAN_FINAL_CDR_LENGH;
}

/*******************************************************************************
  Function        : CWLANBill::SerialBillBuf
  Description     : ��������ǰ���̷��͹����Ļ�������1�ֽڶ��룩���л��ɱ����ֽ���
  Input           : Byte *pIn Ϊ�����ֽ���
                  : Byte *pOut Ϊ�����ֽ���
  Output          : 
  Return          : BOOL
  Calls           : 
  Called by       : 
  
  History         : 
  1 Date          : 2003-4-21
    Author        : Gan Zhiliang
    Modification  : Created file
*******************************************************************************/
void CWLANBill::SerialBillBuf(char *pIn, WLAN_ORIGIAL_CDR *pOut)
{
	memcpy(&(pOut->ucRecordType), pIn, 1);
	memcpy(&(pOut->ulLocalSequenceNumber), (pIn + 1), 4);
	memcpy(&(pOut->ulChargingID), (pIn + 5), 4);
	memcpy(&(pOut->ulRecordSequenceNumber), (pIn + 9), 4);
	//���������ֶζ���char����char����,���Կ���һ����ת��
	//UCHAR ucOperType;			   //Oper_type
	//UCHAR ucOperID; 			   //Oper_ID
	//UCHAR ucServedMSISDN[8];	   // Charge_Dn, ��MSISDN������λF���
	//UCHAR ucServedIMSI[8];		   // IMSI������λF���
	//UCHAR ucUserType;			   // User_type
	//UCHAR ucAuthType;			   // Auth_type
	//UCHAR ucStartTime[14];		   // YYYYMMDDHHMISS,���Ż����ļƷѿ�ʼʱ��
	//UCHAR ucStopTime[14];
	//���Կ���һ����ת��
	memcpy(&(pOut->ucOperType), (pIn + 13), 48);
	memcpy(&(pOut->ulDuration), (pIn + 61), 4);
	memcpy(&(pOut->ulDataFlowUp), (pIn + 65), 4);
	memcpy(&(pOut->ulDataFlowDn), (pIn + 69), 4);
	memcpy(pOut->ucHotSpotID, (pIn + 73), 16);
	memcpy(&(pOut->ulACAddress), (pIn + 89), 4);
	memcpy(&(pOut->ulASAddress), (pIn + 93), 4);
	//��������ļ����ֶζ���char����char���飬����һ����ת��
    memcpy(pOut->ucServiceID, (pIn + 97), 47);
    
	return ;

}


/*******************************************************************************
  Function        : CWLANBill::SerialDstBill
  Description     : ������ʵ�ְ�ת����Ļ��������л�
  Input           : char *pBuffer �����ֽ���
  Output          : char *pBuffer �����ֽ���
  Return          : void
  Calls           : 
  Called by       : 
  
  History         : 
  1 Date          : 2003-4-21
    Author        : Gan Zhiliang
    Modification  : Created file
*******************************************************************************/
void CWLANBill::SerialDstBill(char *pBuffer)
{
	WLAN_FINAL_CDR *pDst = (WLAN_FINAL_CDR *)pBuffer;
	//pDst->ulDuration = HTONL(pDst->ulDuration);
	//pDst->ulDataFlowUp = HTONLL(pDst->ulDataFlowUp);
    //pDst->ulDataFlowUp = HTONLL(pDst->ulDataFlowUp);
    //pDst->ulACAddress = HTONL(pDst->ulACAddress);
	//pDst->ulASAddress = HTONL(pDst->ulASAddress);
	
	memcpy(pBuffer + 49, &(pDst->ulDuration), 4);
	memcpy(pBuffer + 53, &(pDst->ulDataFlowUp), 8);
	memcpy(pBuffer + 61, &(pDst->ulDataFlowDn), 8);
	memcpy(pBuffer + 69, pDst->ucHotSpotID, 16);
    memcpy(pBuffer + 85, &(pDst->ulACAddress), 4);
	memcpy(pBuffer + 89, &(pDst->ulASAddress), 4);	
    memcpy(pBuffer + 93, pDst->ucServiceID, 47); 

    return ;
}
