/*****************************************************
*       gprs_bill.cpp
******************************************************
*Copyright 2002 by Huawei CO.,LTD.
* All rights reserved.
* Author:       颜文远
* Date:         2002年3月
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

//构造函数
CGPRS_BillParser::CGPRS_BillParser()
{
	m_pBill = NULL;
}

//析构函数
CGPRS_BillParser::~CGPRS_BillParser()
{
	if (NULL != m_pBill)
	{
		delete m_pBill;
		m_pBill = NULL;
	}
}

/**********************************************************************
功能描述    解析一张话单
输入参数    pBuf——话单缓冲区指针
			uBufLen——话单缓冲区的长度
输出参数    无
返回值      话单类型是否需要合并
                TRUE----话单类型需要合并 
                FALSE----话单类型不需要合并
抛出异常    在解析失败时抛出异常——包含了错误信息的字符串
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
				//用话单记录初始化话单对象，失败将抛出异常。
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
				//用话单记录初始化话单对象，失败将抛出异常。
				if (m_pBill->Init((char* )&(pRec->u.ggsnPDPRecord), 
								  sizeof(Org_GGSNPDPRecord)))
				{
					return TRUE;
				}
			}

		    throw "G-CDR initialize fail.";
			break;	

		default:  //只有SCDR和GCDR需要合并，其它类型的话单不用合并
            break;
    }

	return FALSE;
}

//返回单张话单的解析结果
CBill* CGPRS_BillParser::GetBillRecord()
{
	CBill* pBill = m_pBill;
	m_pBill = NULL;

	return pBill;
}

//获得要合并的话单的类型数
UINT4 CGPRS_BillParser::GetBillTypeCount()
{
	//GPRS中两种话单需要合并：SCDR和GCDR
	return 2;
}

//根据索引返回话单类型ID
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

//在指定的CMerger对象中注册话单类型
void CGPRS_BillParser::RegisterBillType(CMerger* pMerger)
{
	assert(NULL != pMerger);

	for (UINT4 i = 0; i < GetBillTypeCount(); i++)
	{
		pMerger->RegisterBillType(GetBillTypeID(i));
	}
}

//获得指定的话单类型ID的话单类型描述
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

//构造函数
CGPRS_130_OSS_CDR::CGPRS_130_OSS_CDR()
{
    m_bDataPrepared = FALSE;
}

//析构函数
CGPRS_130_OSS_CDR::~CGPRS_130_OSS_CDR()
{
}

//获取两个时刻间的时间间隔，以秒为单位
//计算两个时间之差。
/* 说明：函数基于以下考虑：如果t2时区为zz2，t1时区为zz1，
 * 那么t2相当于格林尼治时间t2-zz2，t1则相当于格林尼治时间t1-zz1，
 * 因此时间差为(t2-zz2)-(t1-zz1)，也就是t2-t1+zz1-zz2
 */
Fin_CallDuration CGPRS_130_OSS_CDR::GetDuration(const Fin_TimeStamp* pSmallTime, 
												const Org_TimeStamp* pLargeTime)
{
    Fin_CallDuration nDiff = 0; //时间之差
    int zhh1,zMM1, yy1, mm1, dd1, hh1, MM1, ss1;	
    int zhh2,zMM2, yy2, mm2, dd2, hh2, MM2, ss2;	//zhh1,zMM1,zhh2,zMM2表示时区的小时、分钟
    
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

	//由于主机可能会修改时间，所以必须考虑一下2000年问题
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

    //计算两个时间之间相差多少天
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

	//在实际中，时间间隔不可能为负数，否则说明发生了时间跳变
	if (nDiff < 0)  
	{
		nDiff = 0;
	}
    return nDiff;
}

//克隆Org_ManagementExtensions_的链表
void CGPRS_130_OSS_CDR::CloneExtensionsList(const Org_ManagementExtensions_* pSrc, 
							   Org_ManagementExtensions_*& pDst)
{
	CloneList<Org_ManagementExtensions_>(pSrc, pDst);

	//初始化所有的节点中的指针成员变量
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

//删除Org_ManagementExtensions_的链表
void CGPRS_130_OSS_CDR::ClearExtensionsList(Org_ManagementExtensions_*& pExtension)
{
	//首先删除所有节点中的指针成员变量
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

//判断是否闰年。
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

//输入年月，返回该月的天数。
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


//构造函数
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

//析构函数
CGPRS_130_OSS_SCDR::~CGPRS_130_OSS_SCDR()
{
	if (NULL != m_pOrgRec)
	{
		assert((NULL == m_pDestEventRec) && !m_bDataPrepared);

		//清除话单结构中包含的链表
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

		//清除话单结构中包含的链表
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
功能描述    初始化CBill对象
输入参数    pBuf——话单记录缓冲区
			uLen——缓冲区长度
输出参数    无
返回值      初始化的结果
                TRUE----初始化成功
                FALSE----初始化失败
抛出异常    内存操作失败抛出异常
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

	//根据内容初始化Qos链表相关的成员变量
	CloneList<Org__seqof1_>(((Org_SGSNPDPRecord* )pBuf)->listOfTrafficVolumes, 
							m_pOrgRec->listOfTrafficVolumes);
	m_nQosLen = TravelList<Org__seqof1_>(m_pOrgRec->listOfTrafficVolumes, 
										 (Org__seqof1_*& )m_pQosTail);

	//根据内容初始化Extension链表相关的成员变量
	if (m_pOrgRec->bit_mask & Org_SGSNPDPRecord_recordExtensions_present)
	{
		CloneExtensionsList(((Org_SGSNPDPRecord* )pBuf)->recordExtensions, 
							m_pOrgRec->recordExtensions);
		m_nExtensionLen = TravelList<Org_ManagementExtensions_>(m_pOrgRec->recordExtensions, 
												(Org_ManagementExtensions_*& )m_pExtensionTail);
	}
	else  //张玉新 2002-04-16
	{
		m_pOrgRec->recordExtensions = NULL;
	}
	return TRUE;
}

/**********************************************************************
功能描述    判断当前话单是否允许与pBill合并
输入参数    pBill——话单指针
输出参数    无
返回值      是否能够合并
                TRUE----能够合并
                FALSE----不能合并
说明: 1、若pBill指向的话单是重复话单，返回TRUE，以便在
          Merge函数中丢弃该话单。
      2、若pBill指向的话单序列号不连续，认为不能够合并。
      3、若pBill中需要链接的字段长度过长，认为不能够合并。
**********************************************************************/
BOOL CGPRS_130_OSS_SCDR::CanSafeMerge(CBill* pBill)
{
	assert(NULL != pBill);
   	//首先确保当前话单已经转换为最终话单格式
    
    if (!m_bDataPrepared)
    {
        BOOL bResult = PrepareData();
        if (bResult == FALSE)  
		{
			return FALSE;
		}
    }

	CGPRS_130_OSS_SCDR* pSCDR = (CGPRS_130_OSS_SCDR* )pBill;

	//话单必须是部分话单，recordSequenceNumber域必须有效
	if (!(pSCDR->GetOrgRec()->bit_mask & Org_SGSNPDPRecord_recordSequenceNumber_present)
		  || !(m_pDestRec->bit_mask & Fin_SGSNPDPRecord_recordSequenceNumber_present))
	{
		return FALSE;
	}

    //重复话单在Merge方法中丢弃。
	if (IsDuplicated(pSCDR))
	{
		return TRUE;
	}

	//若不是重复话单，则要求序列号连续
	if (pSCDR->GetOrgRec()->recordSequenceNumber != m_pSequenceTail->value + 1)
	{
		return FALSE;
	}

	//判断是否超过数据门限。
	//若超过数据门限，则中断合并，且设置合并结果状态为Fin_reachLimit
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
功能描述    把pBill合入当前话单中
输入参数    pBill——待合入话单对象的指针
输出参数    无
返回值      合并的结果
                TRUE----合并成功
                FALSE----合并失败
抛出异常    内存操作失败抛出异常
**********************************************************************/
BOOL CGPRS_130_OSS_SCDR::Merge(CBill* pBill)
{
	assert((NULL != pBill) && pBill->IsPartial());

	//首先确保当前话单已经转换为最终话单格式
    if (!m_bDataPrepared)
    {
		BOOL bRes = PrepareData(); 
        if (bRes != TRUE)  
		{
			return FALSE;
		}
    }
	CGPRS_130_OSS_SCDR* pSCDR = (CGPRS_130_OSS_SCDR* )pBill;

	//丢弃重复话单
	if (IsDuplicated(pSCDR))
	{
		return TRUE;
	}

	//链接listOfTrafficVolumes字段
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

	//链接recordSequenceNumber字段
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

		//m_pSequenceTail肯定不为NULL
		m_pSequenceTail->next = pSeqNum;
		m_pSequenceTail = pSeqNum;
		m_nSequenceLen++;
	}

	//链接recordExtensions字段
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

	//causeForRecClosing域取最后一张话单的值
	m_pDestRec->causeForRecClosing
		 = (Fin_CauseForRecClosing)(pSCDR->GetOrgRec()->causeForRecClosing);

	//Z30971增加修改是否支持夏时制
	if(!m_bDSTFlg)
	{
		//duration域按照减法方式计算
		Fin_CallDuration nDiff = GetDuration(&(m_pDestRec->recordOpeningTime), 
			&(pSCDR->GetOrgRec()->recordOpeningTime));
		m_pDestRec->duration = nDiff;
	}
	m_pDestRec->duration += pSCDR->GetOrgRec()->duration;
	//修改结束
	
	/*
	设置consolidationResult字段:

	1.PDP结束,根据话终原因设置consolidationResult

	2.PDP未结束,若是因为合并结果超过限制,设置consolidationResult
	  为Fin_reachLimit,其他原因设置Fin_notNormal
	*/
    switch (pSCDR->GetOrgRec()->causeForRecClosing)
	{
    case Fin_normalRelease:             //正常释放
        m_pDestRec->consolidationResult = Fin_normal;
		break;
    case Fin_sGSNChange:                //SGSN改变
        m_pDestRec->consolidationResult = Fin_forInterSGSNConsolidation;
        break;
    case Fin_abnormalRelease:           //PDP异常结束
        m_pDestRec->consolidationResult = Fin_notNormal;
        break;
    default:                            //PDP未结束(置为PDP异常结束)
        m_pDestRec->consolidationResult = Fin_notNormal;
        break;
    }

    return TRUE;
}

/**********************************************************************
功能描述    判断当前话单所属的PDP是否已经结束
输入参数    无
输出参数    无
返回值      PDP是否已经结束
                TRUE----PDP已经结束
                FALSE----PDP未结束
说明:       若话单的终止原因为“正常释放”、“SGSN改变”、
           “PDP异常结束”说明PDP已经结束，否则未结束。
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

    //增加按通话时长结束合并 ZhengYuqun 2003-12-18
    if((UINT4)m_pDestRec->duration >= m_uMaxDuration)
    {
        return TRUE;
    }

	return FALSE;
}

//判断是否需要继续等待后续话单，在此始终返回FALSE
BOOL CGPRS_130_OSS_SCDR::NeedWaiting()
{
	return FALSE;
}

//返回最终格式的话单记录
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

//判断是否部分话单,如果recordSequenceNumber域有效则为部分话单
BOOL CGPRS_130_OSS_SCDR::IsPartial()
{
	return (m_pOrgRec->bit_mask & Org_SGSNPDPRecord_recordSequenceNumber_present);
}

//判断是否PDP的第一张部分话单,
//若为部分话单且recordSequenceNumber为0则为第一张部分话单
BOOL CGPRS_130_OSS_SCDR::IsFirstPartialRec()
{ 
	if (!IsPartial())
	{
		return FALSE;
	}

	return (0 == m_pOrgRec->recordSequenceNumber);
}

//判断是否PDP的末张部分话单
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

    //增加按通话时长结束合并 ZhengYuqun 2003-12-18
    if((UINT4)m_pOrgRec->duration >= m_uMaxDuration)
    {
        return TRUE;
    }

	return FALSE;
}

//返回话单类型ID值
UINT4 CGPRS_130_OSS_SCDR::GetBillType()
{
	return GPRS_S_CDR;
}

//创建索引对象,
//S-CDR的索引键包括三个字段：chargingID、ggsnAddressUsed、sgsnAddress
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

//获得listOfTrafficVolumes链表的长度
int CGPRS_130_OSS_SCDR::GetQosLen()
{
	return m_nQosLen;
}

//获得recordExtensions链表的长度
int CGPRS_130_OSS_SCDR::GetExtensionLen()
{
	return m_nExtensionLen;
}

//获得listOfTrafficVolumes链表的头和尾
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

//获得recordExtensions链表的头和尾
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

//去除listOfTrafficVolumes队列，在本话单被合并到其它话单后调用
void CGPRS_130_OSS_SCDR::RemoveQosList()
{
	//本函数只能对原始话单进行操作
	assert(NULL != m_pOrgRec);

	m_pOrgRec->listOfTrafficVolumes = NULL;
	m_pQosTail = NULL;
	m_nQosLen = 0;
}

//去除recordExtensions队列，在本话单被合并到其它话单后调用
void CGPRS_130_OSS_SCDR::RemoveExtensionList()
{
	//本函数只能对原始话单进行操作
	assert(NULL != m_pOrgRec);

	m_pOrgRec->recordExtensions = NULL;
	m_pExtensionTail = NULL;
	m_nExtensionLen = 0;
}

//获取原始话单结构的指针
const Org_SGSNPDPRecord* CGPRS_130_OSS_SCDR::GetOrgRec()
{
	assert(NULL != m_pOrgRec);

	return m_pOrgRec;
}

//判断是否重复话单
BOOL CGPRS_130_OSS_SCDR::IsDuplicated(CBill* pBill)
{
    CGPRS_130_OSS_SCDR* pSCDR = (CGPRS_130_OSS_SCDR* )pBill;
    int nHeadSeq, nTailSeq, nCurSeq;
    //当前最终话单的起始流水号
    nHeadSeq = m_pDestRec->recordSequenceNumber->value.recordNumberList->value;
    //当前最终话单的最后流水号
    nTailSeq = m_pSequenceTail->value;
    //当前待合并话单的流水号
    nCurSeq = pSCDR->GetOrgRec()->recordSequenceNumber;
    
    //若话单的序列号在已经合并的话单范围内，说明是重复话单。
    return ((nCurSeq >= nHeadSeq) && (nCurSeq <= nTailSeq));
}

/**********************************************************************
功能描述    把原始话单记录转换为符合最终格式的话单记录
输入参数    无
输出参数    无
返回值      转换是否成功
                TRUE----转换成功
                FALSE----转换不成功
抛出异常    内存操作失败抛出异常
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
	//张玉新 2002-4-25
	//转换bit_mask域，在bit_mask中有一些位域在两种话单中不一致
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
    //by ldf 对应问题单 D20357
    //servedMSISDN,此域已经改为optional
    if (m_pOrgRec->bit_mask & Org_SGSNPDPRecord_servedMSISDN_present)
    {
        pRec->bit_mask |= Fin_SGSNPDPRecord_servedMSISDN_present;
    }
    //end
    //by mms对应问题单 SWOD00063
    //apnSelectionMode,此域已经改为optional
    if (m_pOrgRec->bit_mask & Org_SGSNPDPRecord_apnSelectionMode_present)
    {
        pRec->bit_mask |= Fin_SGSNPDPRecord_apnSelectionMode_present;
    }
    //end
	//added by ggf,2003-11-19, 话单中添加sgsnPLMNIdentifier域
	if (m_pOrgRec->bit_mask & Org_sgsnPLMNIdentifier_present)
	{
		pRec->bit_mask |= Fin_sgsnPLMNIdentifier_present;
	}
	//end
	//直接拷贝recordType、networkInitiation、anonymousAccessIndicator、
	//servedIMSI、servedIMEI这几个话单域
	pBegin = (char* )&(m_pOrgRec->recordType);
	pEnd = (char* )&(m_pOrgRec->sgsnAddress);
	memcpy(&(pRec->recordType), pBegin, pEnd - pBegin);

	//转换sgsnAddress话单域
	Fin__seqof1_* pSGSNAddress = new Fin__seqof1_;
	if (NULL == pSGSNAddress)
	{
		throw "Fail to new sgsnAddress when prepare scdr data.";
	}
	pSGSNAddress->next = NULL;
	pSGSNAddress->value = (Fin_GSNAddress& )m_pOrgRec->sgsnAddress;
	pRec->sgsnAddress = pSGSNAddress;

	//直接拷贝msNetworkCapability、routingArea、locationAreaCode、
	//cellIdentity、chargingID、ggsnAddressUsed、accessPointNameNI、pdpType、
	//servedPDPAddress、listOfTrafficVolumes、recordOpeningTime、duration、
	//sgsnChange、causeForRecClosing这几个话单域
	pBegin = (char* )&(m_pOrgRec->msNetworkCapability);
	pEnd = (char* )&(m_pOrgRec->diagnostics);
	memcpy(&(pRec->msNetworkCapability), pBegin, pEnd - pBegin);
	m_pOrgRec->listOfTrafficVolumes = NULL;

	//转换recordSequenceNumber话单域
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

	//直接拷贝nodeID,recordExtensions这个话单域
	pBegin = (char* )&(m_pOrgRec->nodeID);
	pEnd = (char* )&(m_pOrgRec->localSequenceNumber);
	memcpy(&(pRec->nodeID), pBegin, pEnd - pBegin);
	m_pOrgRec->recordExtensions = NULL;

   
	//直接拷贝apnSelectionMode、accessPointNameOI、servedMSISDN、sgsnPLMNIdentifier这几个话单域的结构
	pBegin = (char* )&(m_pOrgRec->apnSelectionMode);
	pEnd = (char* )m_pOrgRec + sizeof(Org_SGSNPDPRecord);
	memcpy(&(pRec->apnSelectionMode), pBegin, pEnd - pBegin);

    /*
		转换consolidationResult字段:

		1.PDP结束,根据话终原因设置consolidationResult

		2.PDP未结束,设置Fin_notNormal

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
    default:            //PDP未结束(置为PDP异常结束)
        pRec->consolidationResult = Fin_notNormal;
        break;
    }
	//单张话单,置结束原因为onlyOneCDRGenerated.
	if (!IsPartial())
	{
		pRec->consolidationResult = Fin_onlyOneCDRGenerated;
	}

	m_pDestRec = pRec;
	m_pDestEventRec = pEventRec;

	//回收原始话单的空间
	delete m_pOrgRec;
	m_pOrgRec = NULL;
	m_bDataPrepared = TRUE;
	return TRUE;
}


//构造函数
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

//析构函数
CGPRS_130_OSS_GCDR::~CGPRS_130_OSS_GCDR()
{
	if (NULL != m_pOrgRec)
	{
		assert((NULL == m_pDestEventRec) && !m_bDataPrepared);

		//清除话单结构中包含的链表
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

		//清除话单结构中包含的链表
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
功能描述    初始化CBill对象
输入参数    pBuf——话单记录缓冲区
			uLen——缓冲区长度
输出参数    无
返回值      初始化的结果
                TRUE----初始化成功
                FALSE----初始化失败
抛出异常    内存操作失败抛出异常
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

	//根据内容初始化sgsnAddress链表相关的成员变量
	CloneList<Org__seqof2>(((Org_GGSNPDPRecord* )pBuf)->sgsnAddress, 
							m_pOrgRec->sgsnAddress);
	m_nSGSNLen = TravelList<Org__seqof2>(m_pOrgRec->sgsnAddress, 
										 (Org__seqof2*& )m_pSGSNTail);

	//根据内容初始化remotePDPAddress链表相关的成员变量
	if (m_pOrgRec->bit_mask & Org_remotePDPAddress_present)
	{
		CloneList<Org__seqof3>(((Org_GGSNPDPRecord* )pBuf)->remotePDPAddress, 
							   m_pOrgRec->remotePDPAddress);
		m_nPDPLen = TravelList<Org__seqof3>(m_pOrgRec->remotePDPAddress, 
											(Org__seqof3*& )m_pPDPTail);
	}

	//根据内容初始化listOfTrafficVolumes链表相关的成员变量
	CloneList<Org__seqof1_>(((Org_GGSNPDPRecord* )pBuf)->listOfTrafficVolumes, 
							m_pOrgRec->listOfTrafficVolumes);
	m_nQosLen = TravelList<Org__seqof1_>(m_pOrgRec->listOfTrafficVolumes, 
										 (Org__seqof1_*& )m_pQosTail);

	//根据内容初始化recordExtensions链表相关的成员变量
	if (m_pOrgRec->bit_mask & Org_GGSNPDPRecord_recordExtensions_present)
	{
		CloneExtensionsList(((Org_GGSNPDPRecord* )pBuf)->recordExtensions, 
							m_pOrgRec->recordExtensions);
		m_nExtensionLen = TravelList<Org_ManagementExtensions_>(m_pOrgRec->recordExtensions, 
											(Org_ManagementExtensions_*& )m_pExtensionTail);
	}
    else  //张玉新 2002-04-16
	{
		m_pOrgRec->recordExtensions = NULL;
	}
	return TRUE;
}

/**********************************************************************
功能描述    判断当前话单是否允许与pBill合并
输入参数    pBill——话单指针
输出参数    无
返回值      是否能够合并
                TRUE----能够合并
                FALSE----不能合并
说明: 1、若pBill指向的话单是重复话单，返回TRUE，以便在
          Merge函数中丢弃该话单。
      2、若pBill指向的话单序列号不连续，认为不能够合并。
      3、若pBill中需要链接的字段长度过长，认为不能够合并。
**********************************************************************/
BOOL CGPRS_130_OSS_GCDR::CanSafeMerge(CBill* pBill)
{
	assert(NULL != pBill);

	//首先确保当前话单已经转换为最终话单格式
    if (!m_bDataPrepared)
    {
		BOOL bRes = PrepareData();
        if (bRes != TRUE)  
		{
			return FALSE;
		}
    }

	CGPRS_130_OSS_GCDR* pGCDR = (CGPRS_130_OSS_GCDR* )pBill;

	//话单必须是部分话单，recordSequenceNumber域必须有效
	if (!(pGCDR->GetOrgRec()->bit_mask & Org_GGSNPDPRecord_recordSequenceNumber_present)
		 || !(m_pDestRec->bit_mask & Fin_GGSNPDPRecord_recordSequenceNumber_present))
	{

		return FALSE;
	}

    //重复话单在Merge方法中丢弃
	if (IsDuplicated(pGCDR))
	{


		return TRUE;
	}

	//若不是重复话单，则要求序列号连续
	if (pGCDR->GetOrgRec()->recordSequenceNumber != m_pSequenceTail->value + 1)
	{

		return FALSE;
	}

	//判断是否超过数据门限。
	//若超过数据门限，则中断合并，且设置合并结果状态为ReachLimit
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
功能描述    把pBill合入当前话单中
输入参数    pBill——待合入话单对象的指针
输出参数    无
返回值      合并的结果
                TRUE----合并成功
                FALSE----合并失败
抛出异常    内存操作失败抛出异常
**********************************************************************/
BOOL CGPRS_130_OSS_GCDR::Merge(CBill* pBill)
{
	assert((NULL != pBill) && pBill->IsPartial());

	//首先确保当前话单已经转换为最终话单格式
    if (!m_bDataPrepared)
    {
		BOOL bRes = PrepareData();
        if (bRes != TRUE)  
		{
			return FALSE;
		}
    }

	CGPRS_130_OSS_GCDR* pGCDR = (CGPRS_130_OSS_GCDR* )pBill;

	//丢弃重复话单
	if (IsDuplicated(pGCDR) == TRUE)
	{


		return TRUE;
	}

    //链接sgsnAddress字段，并剔除相邻且相同的sgsnAddress
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
			//删除新链表头的重复节点
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

	//链接remotePDPAddress字段
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

	//链接listOfTrafficVolumes字段
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

	//链接recordSequenceNumber字段
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

		//m_pSequenceTail肯定不为NULL
		m_pSequenceTail->next = pSeqNum;
		m_pSequenceTail = pSeqNum;
		m_nSequenceLen++;
	}

	//链接recordExtensions字段
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

	//causeForRecClosing域取最后一张话单的值
	m_pDestRec->causeForRecClosing 
		 = (Fin_CauseForRecClosing)(pGCDR->GetOrgRec()->causeForRecClosing);

	//Z30971增加修改是否支持夏时制
	if(!m_bDSTFlg)
	{
		//duration域按照减法方式计算
		Fin_CallDuration nDiff = GetDuration(&(m_pDestRec->recordOpeningTime), 
			&(pGCDR->GetOrgRec()->recordOpeningTime));
		m_pDestRec->duration = nDiff;
	}
	m_pDestRec->duration += pGCDR->GetOrgRec()->duration;
	//修改结束

	/*
	设置consolidationResult字段:

	1.PDP结束,根据话终原因设置consolidationResult

	2.PDP未结束,若是因为合并结果超过限制,设置consolidationResult
	  为Fin_reachLimit,其他原因设置Fin_notNormal
	*/
    switch (pGCDR->GetOrgRec()->causeForRecClosing)
	{
    case Fin_normalRelease:             //正常释放
        m_pDestRec->consolidationResult = Fin_normal;
		break;
    case Fin_sGSNChange:                //SGSN改变
        m_pDestRec->consolidationResult = Fin_forInterSGSNConsolidation;
        break;
    case Fin_abnormalRelease:           //PDP异常结束
        m_pDestRec->consolidationResult = Fin_notNormal;
        break;
    default:                            //PDP未结束(置为PDP异常结束)
        m_pDestRec->consolidationResult = Fin_notNormal;
        break;
    }

    return TRUE;
}

/**********************************************************************
功能描述    判断当前话单所属的PDP是否已经结束
输入参数    无
输出参数    无
返回值      PDP是否已经结束
                TRUE----PDP已经结束
                FALSE----PDP未结束
说明:       若话单的终止原因为“正常释放”、“SGSN改变”、
           “PDP异常结束”说明PDP已经结束，否则未结束。
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

    //增加按通话时长结束合并 ZhengYuqun 2003-12-18
    if((UINT4)m_pDestRec->duration >= m_uMaxDuration)
    {
        return TRUE;
    }

	return FALSE;
}

//判断是否需要继续等待后续话单，在此始终返回FALSE
BOOL CGPRS_130_OSS_GCDR::NeedWaiting()
{
	return FALSE;
}

//返回最终格式的话单记录。
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

//判断是否部分话单,如果recordSequenceNumber域有效则为部分话单
BOOL CGPRS_130_OSS_GCDR::IsPartial()
{
	return (m_pOrgRec->bit_mask & Org_GGSNPDPRecord_recordSequenceNumber_present);
}

//判断是否PDP的第一张部分话单,
//若为部分话单且recordSequenceNumber为0则为第一张部分话单
BOOL CGPRS_130_OSS_GCDR::IsFirstPartialRec()
{ 
	if (!IsPartial())
	{
		return FALSE;
	}

	return (0 == m_pOrgRec->recordSequenceNumber);
}

//判断是否PDP的末张部分话单
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

    //增加按通话时长结束合并 ZhengYuqun 2003-12-18
    if((UINT4)m_pOrgRec->duration >= m_uMaxDuration)
    {
        return TRUE;
    }

	return FALSE;
}

//返回话单类型ID值
UINT4 CGPRS_130_OSS_GCDR::GetBillType()
{
	return GPRS_G_CDR;
}

//创建索引对象。
//G-CDR的索引键包括2个字段：chargingID、ggsnAddressUsed
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

//获得sgsnAddress链表的长度
int CGPRS_130_OSS_GCDR::GetSGSNLen()
{
	return m_nSGSNLen;
}

//获得remotePDPAddress链表的长度
int CGPRS_130_OSS_GCDR::GetPDPLen()
{
	return m_nPDPLen;
}

//获得listOfTrafficVolumes链表的长度
int CGPRS_130_OSS_GCDR::GetQosLen()
{
	return m_nQosLen;
}

//获得recordExtensions链表的长度
int CGPRS_130_OSS_GCDR::GetExtensionLen()
{
	return m_nExtensionLen;
}

//获得sgsnAddress链表的头和尾
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

//获得remotePDPAddress链表的头和尾
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

//获得listOfTrafficVolumes链表的头和尾
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

//获得recordExtensions链表的头和尾
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

//去除sgsnAddress队列，在本话单被合并到其它话单后调用
void CGPRS_130_OSS_GCDR::RemoveSGSNList()
{
	//本函数只能对原始话单进行操作
	assert(NULL != m_pOrgRec);

	m_pOrgRec->sgsnAddress = NULL;
	m_pSGSNTail = NULL;
	m_nSGSNLen = 0;
}

//去除remotePDPAddress队列，在本话单被合并到其它话单后调用
void CGPRS_130_OSS_GCDR::RemovePDPList()
{
	//本函数只能对原始话单进行操作
	assert(NULL != m_pOrgRec);

	m_pOrgRec->remotePDPAddress = NULL;
	m_pPDPTail = NULL;
	m_nPDPLen = 0;
}

//去除listOfTrafficVolumes队列，在本话单被合并到其它话单后调用
void CGPRS_130_OSS_GCDR::RemoveQosList()
{
	//本函数只能对原始话单进行操作
	assert(NULL != m_pOrgRec);

	m_pOrgRec->listOfTrafficVolumes = NULL;
	m_pQosTail = NULL;
	m_nQosLen = 0;
}

//去除recordExtensions队列，在本话单被合并到其它话单后调用
void CGPRS_130_OSS_GCDR::RemoveExtensionList()
{
	//本函数只能对原始话单进行操作
	assert(NULL != m_pOrgRec);

	m_pOrgRec->recordExtensions = NULL;
	m_pExtensionTail = NULL;
	m_nExtensionLen = 0;
}

//获取原始话单结构的指针
const Org_GGSNPDPRecord* CGPRS_130_OSS_GCDR::GetOrgRec()
{
	assert(NULL != m_pOrgRec);

	return m_pOrgRec;
}

//判断是否重复话单
BOOL CGPRS_130_OSS_GCDR::IsDuplicated(CBill* pBill)
{
    if(m_pDestRec == NULL)
    {
        return FALSE;
    }
    
    CGPRS_130_OSS_GCDR* pGCDR = (CGPRS_130_OSS_GCDR* )pBill;
    int nHeadSeq, nTailSeq, nCurSeq;
    //当前最终话单的起始流水号
    nHeadSeq = m_pDestRec->recordSequenceNumber.recordNumberList->value;
    //当前最终话单的最后流水号
    nTailSeq = m_pSequenceTail->value;
    //当前待合并话单的流水号
    nCurSeq = pGCDR->GetOrgRec()->recordSequenceNumber;
    
    //若话单的序列号在已经合并的话单范围内，说明是重复话单。
    return ((nCurSeq >= nHeadSeq) && (nCurSeq <= nTailSeq));
}
/**********************************************************************
功能描述    把原始话单记录转换为符合最终格式的话单记录
输入参数    无
输出参数    无
返回值      转换是否成功
                TRUE----转换成功
                FALSE----转换不成功
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

	//转换bit_mask域，合并前后的bit_mask域长度不一致
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
    //by ldf 对应问题单 D20357 2003-02-18
    //servedMSISDN,这个域已经改为option域
    if (m_pOrgRec->bit_mask & Org_GGSNPDPRecord_servedMSISDN_present)
    {
        pRec->bit_mask |= Fin_GGSNPDPRecord_servedMSISDN_present;
    }
    //end
    //by mms对应问题单 SWOD00063
    //apnSelectionMode,此域已经改为optional
    if (m_pOrgRec->bit_mask & Org_GGSNPDPRecord_apnSelectionMode_present)
    {
        pRec->bit_mask |= Fin_GGSNPDPRecord_apnSelectionMode_present;
    }
    //end
	//added by ggf,2003-11-19 添加sgsnPLMNIdentifier域
	if (m_pOrgRec->bit_mask & Org_sgsnPLMNIdentifier_present)
	{
		pRec->bit_mask |= Fin_sgsnPLMNIdentifier_present;
	}
	//end
	//直接拷贝recordType、networkInitiation、anonymousAccessIndicator、
	//servedIMSI、ggsnAddress、chargingID、sgsnAddress、accessPointNameNI、
	//pdpType、servedPDPAddress、remotePDPAddress、dynamicAddressFlag、
	//listOfTrafficVolumes、recordOpeningTime、duration、
	//这几个话单域
	pBegin = (char* )&(m_pOrgRec->recordType);
	pEnd = (char* )&(m_pOrgRec->diagnostics);
	memcpy(&(pRec->recordType), pBegin, pEnd - pBegin);
	//m_pOrgRec->sgsnAddress = NULL; //张玉新删除
	m_pOrgRec->remotePDPAddress = NULL;
	m_pOrgRec->listOfTrafficVolumes = NULL;

	//转换recordSequenceNumber话单域
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
        m_pOrgRec->sgsnAddress = NULL;//张玉新 增加
		m_pSequenceTail = pSeqNum;
		m_nSequenceLen = 1;
	}

	//直接拷贝nodeID、recordExtensions这几个话单域
	pBegin = (char* )&(m_pOrgRec->nodeID);
	pEnd = (char* )&(m_pOrgRec->localSequenceNumber);
	//张玉新 2002-04-18 pEnd - pEnd ==> pEnd - pBegin
	memcpy(&(pRec->nodeID), pBegin, pEnd - pBegin);
	m_pOrgRec->recordExtensions = NULL;

    //直接拷贝apnSelectionMode、servedMSISDN这几个话单域
	pBegin = (char* )&(m_pOrgRec->apnSelectionMode);
	pEnd = (char* )m_pOrgRec + sizeof(Org_GGSNPDPRecord);
	memcpy(&(pRec->apnSelectionMode), pBegin, pEnd - pBegin);
	
    /*
		转换consolidationResult字段:

		1.PDP结束,根据话终原因设置consolidationResult

		2.PDP未结束,设置Fin_notNormal

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
    default:            //PDP未结束(置为PDP异常结束)
        pRec->consolidationResult = Fin_notNormal;
        break;
    }
	//单张话单,置结束原因为onlyOneCDRGenerated.
	if (!IsPartial())
	{
		pRec->consolidationResult = Fin_onlyOneCDRGenerated;
	}

	m_pDestRec = pRec;
	m_pDestEventRec = pEventRec;

	//回收原始话单的空间
	delete m_pOrgRec;
	m_pOrgRec = NULL;

	m_bDataPrepared = TRUE;

	return TRUE;
}


//构造函数
CGPRS_130_OSS_SCDR_Key::CGPRS_130_OSS_SCDR_Key()
{
}

//析构函数
CGPRS_130_OSS_SCDR_Key::~CGPRS_130_OSS_SCDR_Key()
{
}

//设置sgsn地址
void CGPRS_130_OSS_SCDR_Key::SetSGSNAddress(const Org_GSNAddress& sgsnAddress)
{
	m_sgsnAddress = sgsnAddress;
}

//设置ggsn地址
void CGPRS_130_OSS_SCDR_Key::SetGGSNAddress(const Org_GSNAddress& ggsnAddress)
{
	m_ggsnAddressUsed = ggsnAddress;
}

//设置chargingID
void CGPRS_130_OSS_SCDR_Key::SetChargingID(const Org_ChargingID chargingID)
{
	m_chargingID = chargingID;
}

//获得sgsn地址
const Org_GSNAddress& CGPRS_130_OSS_SCDR_Key::GetSGSNAddress()
{
	return m_sgsnAddress;
}

//获得ggsn地址
const Org_GSNAddress& CGPRS_130_OSS_SCDR_Key::GetGGSNAddress()
{
	return m_ggsnAddressUsed;
}

//获得chargingID
Org_ChargingID CGPRS_130_OSS_SCDR_Key::GetChargingID()
{
	return m_chargingID;
}

//判断是否与指定的pKey相同
BOOL CGPRS_130_OSS_SCDR_Key::IsMatch(CKey* pKey)
{
	CGPRS_130_OSS_SCDR_Key* p = (CGPRS_130_OSS_SCDR_Key* )pKey;

	return (Match_gsnAddress(&(p->GetSGSNAddress()), &m_sgsnAddress)
		     && Match_gsnAddress(&(p->GetGGSNAddress()), &m_ggsnAddressUsed)
			 && (p->GetChargingID() == m_chargingID));
}

//返回键值
UINT4 CGPRS_130_OSS_SCDR_Key::GetKey()
{
	return m_chargingID;
}


//构造函数
CGPRS_130_OSS_GCDR_Key::CGPRS_130_OSS_GCDR_Key()
{
}

//析构函数
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


//比较两个GSN地址
BOOL Match_gsnAddress(const Org_GSNAddress* p1, const Org_GSNAddress* p2)
{
	//张玉新 增加
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
		
		//根据不同的IP地址表示方法来计算IP地址占用的空间，并进行内容的比较
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
		
		//根据不同的IP地址表示方法来计算IP地址占用的空间，并进行内容的比较
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
