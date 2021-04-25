/******************************************************************************
  Copyright (C), 2003-2011, Huawei Tech. Co., Ltd.

/******************************************************************************
  File Name       : wlan_bill.cpp
  Version         : 1.0
  Author          : Gan Zhiliang
  Created         : 2003-3-25
  Last Modified   : 
  Description     : WLAN 话单合并的实现
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
* 函数名: CWLAN_BillParser::CWLAN_BillParser()
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   构造函数	
* 输入:   无
* 返回值: 无
* 说明:  
* 更新记录: 
*
* 日期			  姓名					描述 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
CWLAN_BillParser::CWLAN_BillParser()
{
	m_pBill = NULL;
	
}
/******************************************************
* 函数名: CWLAN_BillParser::~CWLAN_BillParser()
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   析构函数	
* 输入:   无
* 返回值: 无
* 说明:  
* 更新记录: 
*
* 日期			  姓名					描述 
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
* 函数名: CBill* CWLAN_BillParser::GetBillRecord()
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   得到一条话单记录	
* 输入:   无
* 返回值: 无
* 说明:  
* 更新记录: 
*
* 日期			  姓名					描述 
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
* 函数名: BOOL CWLAN_BillParser::ParseARecord(char *pBuf, int nBufLen)
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   解析一条话单记录		
* 输入:   Char *pBuf  一次呼叫记录的话单Int nBufLen 缓冲区的长度
* 返回值: BOOL
* 说明:  
* 更新记录: 
*
* 日期			  姓名					描述 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
BOOL CWLAN_BillParser::ParseARecord(char *pBuf, UINT4 nBufLen)
{
	m_pBill = new CWLANBill;
	
	if(m_pBill != NULL)
	{
		//用话单记录初始化话单对象，失败将抛出异常。
		if(m_pBill->Init(pBuf, sizeof(WLAN_ORIGIAL_CDR)))
		{
			nBufLen++;//为了消除nBuflen没有引用的告警
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
* 函数名: void CWLAN_BillParser::RegisterBillType(CMerger *pMerger)
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   向CMerger对象注册话单类型 	
* 输入:   CMerger *pMerger合并对象
* 返回值: 无
* 说明:  
* 更新记录: 
*
* 日期			  姓名					描述 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
void CWLAN_BillParser::RegisterBillType(CMerger *pMerger)
{
	assert(pMerger != NULL);
	pMerger->RegisterBillType(WLAN_S_CDR);
	
}
/******************************************************
* 函数名: int CWLAN_BillParser::GetBillTypeID(int nIndex)
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   得到已经注册话单类型的ID 号码 	
* 输入:   索引号码
* 返回值: int
* 说明:  
* 更新记录: 
*
* 日期			  姓名					描述 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
UINT4 CWLAN_BillParser::GetBillTypeID(UINT4 uIndex)
{
	uIndex++; //为了消除nIndex没有引用的告警
	return WLAN_S_CDR;
}
/******************************************************
* 函数名: int CWLAN_BillParser::GetBillTypeCount()
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   得到已经注册话单类型的数目	
* 输入:   无
* 返回值: int
* 说明:  
* 更新记录: 
*
* 日期			  姓名					描述 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
UINT4 CWLAN_BillParser::GetBillTypeCount()
{
	return 1;
}


/*******************************************************************************
  Function        : CWLAN_BillParser::GetBillTypeDesc
  Description     : 获得指定的话单类型ID的话单类型描述
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
* 函数名: CWLANBill::CWLANBill()
* 作者:   Gan ZHiliang 
* 日期:    2003-4-21
* 描述:    构造函数
* 输入:    无。
* 返回值:  无。
* 说明:    
* 更新记录: 
*
* 日期			  姓名					描述 
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
* 函数名: CWLANBill::~CWLANBill()
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   析构函数，释放话单记录空间。
* 输入:   无。
* 返回值:  无。
* 说明:    若话单经过了合并，则存在链表需要释放。
* 更新记录: 
*
* 日期			  姓名					描述 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
CWLANBill::~CWLANBill()
{
	if(m_pInProcessRec != NULL)
	{
		
		//首先删除链表节点。
		if(m_pInProcessRec->pRecordSequenceNumber)
		{
			//	序列号指针一定不为NULL。			
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
* 函数名: CWLANBill::IsPartial()
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   判断一张话单是否是部分话单
* 输入:   无。
* 返回值:  BOOL
* 说明:   如果原因值为0xFE表明这张话单是部分话单
* 更新记录: 
*
* 日期			  姓名			描述 
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
* 函数名: CWLANBill::IsFirstPartialRec()
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   判断一张话单是否是第一张部分话单
* 输入:   无。
* 返回值:  BOOL
* 说明:   如果原因值为0xFE且序列号码为1，表明这张话单是第一张部分话单
* 更新记录: 
*
* 日期			  姓名			描述 
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
* 函数名: BOOL CWLANBill::IsLastPartialRec()
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   判断一张话单是不是最后的一条话单		   
* 输入:   无。
* 返回值:  BOOL
* 说明:    判断话终原因即可　
* 更新记录: 
*      
*
* 日期			  姓名			描述 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
BOOL CWLANBill::IsLastPartialRec()
{
    return (m_pOrgRec->ucCloseCause != WLAN_PartialRec);
   
}

/******************************************************
* 函数名: BOOL CWLANBill::NeedWaiting()
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   判断当话单所属PDP已经结束时，是否需要继续等待后续话单。		 
* 输入:   无。
* 返回值:  BOOL
* 说明:   此函数目前值恒为FALSE
* 更新记录: 
*
* 日期			  姓名			描述 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
BOOL CWLANBill::NeedWaiting()
{
	return FALSE;
	
}

/******************************************************
* 函数名: BOOL CWLANBill::IsPDPCompleted()
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   判断当前话单所属的PDP是否已经结束 	  
* 输入:   无。
* 返回值:  BOOL，若PDP已经结束，则返回TRUE；否则返回FALSE。
* 说明:  若话单的终止原因为“正常释放”、“超时”、“异常结束”
*		  说明PDP已经结束。否则未结束。
* 更新记录: 
*    
*
* 日期			  姓名			描述 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
BOOL CWLANBill::IsPDPCompleted()
{
	assert(m_pInProcessRec != NULL);

    return (m_pInProcessRec->ucCloseCause != WLAN_PartialRec);
   
}

/******************************************************
* 函数名: CKey CWLANBill::CreateKey()
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   创建话单的索引键
* 输入:   无。
* 返回值: CKey
* 说明:  WLANBill的话单的索引键包括chargingID、ulASaddress，这里必须考虑字节序的问题
* 更新记录: 
*
* 日期			  姓名			描述 
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
* 函数名: int CWLANBill::GetBillType()
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   得到注册的话单类型的值	  
* 输入:   无。
* 返回值: int
* 说明:  
* 更新记录: 
*
* 日期			  姓名		描述 
* ========== ================== ======================
*2003-4-21		Gan Zhiliang
******************************************************/
UINT4 CWLANBill::GetBillType()
{
	return WLAN_S_CDR;
	
}
/******************************************************
* 函数名: char* CWLANBill::GetContents()
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   返回最终格式的话单记录	  
* 输入:   无。
* 返回值: char *
* 说明:  
* 更新记录: 
*
* 日期			  姓名		描述 
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
	//把合并后的结构m_pInProcessRec转换成最终需要的类型WLAN_FINAL_CDR
	//首先删除链表节点。
	if(m_pInProcessRec->pRecordSequenceNumber)
	{
		//	序列号指针一定不为NULL。			
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

    //如果是黑话单，更改断线原因为会话超时
    if (m_pInProcessRec->ucCloseCause == WLAN_PartialRec)
    {
        m_pInProcessRec->ucCloseCause = WLAN_SessionTimeout;
    }
    
	char *pBuffer = (char*)(&(m_pInProcessRec->ucRecordType));
	SerialDstBill(pBuffer);
	return pBuffer;
	
}

/******************************************************
* 函数名: BOOL CWLANBill::Init()
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   初始化CBill对象	   
* 输入:   无。
* 返回值: BOOL
* 说明:  
* 更新记录: 
*
* 日期			  姓名		描述 
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
* 函数名: BOOL CWLANBill::PrepareData()
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   把原始话单记录转换为符合最终合并时要求的话单记录		
* 输入:   无。
* 返回值: BOOL
* 说明:  
* 更新记录: 
*
* 日期			  姓名			描述 
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
	//取第一个字段
	pEventRec->ucRecordType = m_pOrgRec->ucRecordType;
	pEventRec->ulLocalSequenceNumber = NTOHL(m_pOrgRec->ulLocalSequenceNumber);
	pEventRec->ulChargingID = NTOHL(m_pOrgRec->ulChargingID);
	//把第4个字段转换成链表
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

	//取第5个字段以后的8个字段
	memcpy(&(pEventRec->ucOperType), &(m_pOrgRec->ucOperType), offsetof(WLAN_ORIGIAL_CDR, ulDuration) - offsetof(WLAN_ORIGIAL_CDR, ucOperType));
	
	//取通话时长ulDuration
	pEventRec->ulDuration = NTOHL(m_pOrgRec->ulDuration);
	//取ulDataFlowUp
    pEventRec->ulDataFlowUp = NTOHL(m_pOrgRec->ulDataFlowUp);
	//取ulDataFlowDn
	pEventRec->ulDataFlowDn = NTOHL(m_pOrgRec->ulDataFlowDn);
	memcpy(pEventRec->ucHotSpotID, m_pOrgRec->ucHotSpotID, sizeof(pEventRec->ucHotSpotID));
	//取ulACAddress
	pEventRec->ulACAddress = m_pOrgRec->ulACAddress;
	//取ulASAddress
	pEventRec->ulASAddress = m_pOrgRec->ulASAddress; 

	memcpy(pEventRec->ucServiceID, m_pOrgRec->ucServiceID, sizeof(pEventRec->ucServiceID));
	memcpy(pEventRec->ucIspID, m_pOrgRec->ucIspID, sizeof(pEventRec->ucIspID));
	
    //取ucCloseCause
	pEventRec->ucCloseCause = m_pOrgRec->ucCloseCause;
	
    //otp版本, 新增一个字段Domain，add by lucy 2004.5.25
    memcpy(pEventRec->ucDomain, m_pOrgRec->ucDomain, sizeof(m_pOrgRec->ucDomain));
	
	m_pInProcessRec = pEventRec;
	//回收原始话单空间。
	delete m_pOrgRec;
	m_pOrgRec = NULL;
	m_bDataPrepared = TRUE;
	return TRUE;
	
}

/******************************************************
* 函数名: BOOL CWLANBill::CanSafeMerge(CBill *pBill)
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   判断两张话(相同类型)单是否能够合并。		
* 输入:   CBill *pBill
* 返回值: BOOL
* 说明:
*1、若pBill指向的话单是重复话单，返回TRUE，以便在Merge函数中丢弃该话单。
*2、若pBill指向的话单序列号不连续，认为不能够合并。
*3、判断流量是否溢出？如果溢出认为不能合并，这样队列中的话单就会超时关闭。
*4 、判断通话时长是否超过999999?超过则认为是不能够合并，反之认为可以合并。 
* 更新记录: 
*
* 日期			  姓名	    	描述 
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
	// recordSequenceNumber链表的头节点值
	No1 = m_pInProcessRec->pRecordSequenceNumber->value;
	//recordSequenceNumber链表的尾节点值。	  
	No2 = m_pSequenceTail->value;
	No3 = NTOHL(p->m_pOrgRec->ulRecordSequenceNumber);
	// 若话单的序列号在已经合并的话单范围内，说明是重复话单。
	//重复话单在Merge方法中丢弃。
	if((No3 <= No2) && (No3 >= No1))
	{
		return TRUE;
	}
	// 若不是重复话单，则要求序列号连续。
	if((No2 + 1) != No3)
	{
		return FALSE;
	}
	
	INT8 iDataFlow1,iDataFlow2;
	//判断上行流量是否越界
	iDataFlow1 = m_pInProcessRec->ulDataFlowUp;
	iDataFlow2 = NTOHL(p->m_pOrgRec->ulDataFlowUp);
	if(iDataFlow2 > (MAX_DATA_FLOW - iDataFlow1))
	{
		m_pInProcessRec->ucCloseCause = WLAN_SessionTimeout;
		return FALSE;
	}
	//判断下行流量是否越界	   
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
* 函数名: BOOL CWLANBill::Merge(CBill *pBill)
* 作者:   Gan ZHiliang 
* 日期:   2003-4-21
* 描述:   合并两张话单		
* 输入:   CBill *pBill
* 返回值: BOOL
* 说明:
* 更新记录: 
*
* 日期			  姓名			描述 
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
	//丢弃重复话单	 
	if((NTOHL(p->ulRecordSequenceNumber)
		>= pRec->pRecordSequenceNumber->value)
		&& (NTOHL(p->ulRecordSequenceNumber) <= m_pSequenceTail->value))
	{
		return TRUE;
	}
	
	//RecordSequenceNumber相连接 
	
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
	
	//ucCloseCause字段取最后一张话单的值。	   
	pRec->ucCloseCause = p->ucCloseCause;
	//ucStopTime字段取最后一张话单的值
	memcpy(pRec->ucStopTime, p->ucStopTime, sizeof(p->ucStopTime));
	//strcpy((char *)pRec->ucStopTime,(char *) p->ucStopTime);
	//ulduration直接相加
	pRec->ulDuration += NTOHL(p->ulDuration); 
	//流量相加
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
  Description     : 本函数把前存盘发送过来的话单包（1字节对齐）序列化成本机字节序
  Input           : Byte *pIn 为网络字节序
                  : Byte *pOut 为网络字节序
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
	//由于下面字段都是char或者char数组,所以可以一次性转换
	//UCHAR ucOperType;			   //Oper_type
	//UCHAR ucOperID; 			   //Oper_ID
	//UCHAR ucServedMSISDN[8];	   // Charge_Dn, 即MSISDN，不足位F填充
	//UCHAR ucServedIMSI[8];		   // IMSI，不足位F填充
	//UCHAR ucUserType;			   // User_type
	//UCHAR ucAuthType;			   // Auth_type
	//UCHAR ucStartTime[14];		   // YYYYMMDDHHMISS,该张话单的计费开始时间
	//UCHAR ucStopTime[14];
	//所以可以一次性转换
	memcpy(&(pOut->ucOperType), (pIn + 13), 48);
	memcpy(&(pOut->ulDuration), (pIn + 61), 4);
	memcpy(&(pOut->ulDataFlowUp), (pIn + 65), 4);
	memcpy(&(pOut->ulDataFlowDn), (pIn + 69), 4);
	memcpy(pOut->ucHotSpotID, (pIn + 73), 16);
	memcpy(&(pOut->ulACAddress), (pIn + 89), 4);
	memcpy(&(pOut->ulASAddress), (pIn + 93), 4);
	//由于下面的几个字段都是char或者char数组，所以一次性转换
    memcpy(pOut->ucServiceID, (pIn + 97), 47);
    
	return ;

}


/*******************************************************************************
  Function        : CWLANBill::SerialDstBill
  Description     : 本函数实现把转换后的话单的序列化
  Input           : char *pBuffer 主机字节序
  Output          : char *pBuffer 主机字节序
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
