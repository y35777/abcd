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
* Update: 01-8-29 上午 09:09:51
* Date              Name              Description 
============== ================ ======================
## 01-8-29       Zhou Yonghui       作如下修改
   
   在所有字段进行合并之前检查如下条件：

   1、若话单是可选字段，判断有效性标记是否设置。
   若标记没有设置，则不合并该字段。

   2、若字段不可选，或者可选字段的有效性标记已经设置，
   则判断字段数据是否有效。
   若数据有效则合并该字段，否则不合并。

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
*若解析成功，返回TRUE，失败则抛出一个异常。
*若话单类型不能识别，则返回FALSE。
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
                //用话单记录初始化话单对象，失败将抛出异常。
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
    //3G需要合并的话单有二种：S-CDR,G-CDR。
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
* 函数名:   C3G_BillParser::RegisterBillType
* 作者:    Zhou Yonghui
* 日期:    01-6-8
* 描述:    向CMerger对象注册话单类型。
* 输入: 
*       参数1 :CMerger* pMerger
* 返回值: void 
* 说明:     若内存操作失败将抛出异常。
* 更新记录: 
*
* 日期            姓名                  描述 
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

//颜文远2002-03-06添加
//获得指定的话单类型ID的话单类型描述,szDesc为输出参数
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
//添加结束，颜文远2002-03-06


//C3G_350_SCDR类实现(S-CDR)
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
    //回收原始话单记录空间。
    if(m_pOrgRec != NULL)
    {
        delete[] m_pOrgRec->listOfTrafficVolumes.value;
		
        if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_diagnostics_present)
        {
            Clear_Diagnostics(&(m_pOrgRec->diagnostics));
        }

        if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_recordExtensions_present)
        {
            //检查字段的数据是否有效。
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

    //回收最终话单记录空间。
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
     * 合并过程中使用的数据结构的空间已经在GetContents中回收。
     */
}


/******************************************************
* 函数名:   C3G_350_SCDR::Init
* 作者:    Zhou Yonghui
* 日期:    01-5-28
* 描述: 处理过程如下：
*       1,备份原始话单记录，作为IsPDPCompleted,CanSafeMerge,
*           IsPartial,IsFirstPartialRec,IsLastPartialRec判断的参考。
*       2,用listOfTrafficVolumes,recordExtensions,recordSequenceNumber
*           字段初始化链表。
*       3,记录当前合并完成的最大的内部序列号。
*       4,设置PDP结束标志。
* 输入: 
*       参数1 :char* pBuf，原始话单记录
*       参数2 :int nLen，话单数据的长度
* 返回值: BOOL，成功返回TRUE。内存操作失败返回FALSE。
* 说明:
*       若该函数返回FALSE，则话单对象将被删除。
*       在话单对象的析构函数中完成空间的回收。
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*01-5-28       Zhou Yonghui
*01-9-25       Zhou Yonghui
*              增加了初始化m_nLen_Condition_List和m_nLen_Extension_List
*              的代码，以便进行流量控制。
******************************************************/
BOOL C3G_350_SCDR::Init(char* pBuf,UINT4 uLen)
{
    assert(pBuf != NULL);
    
    CDR3G_SGSNPDPRecord* p = (CDR3G_SGSNPDPRecord*)pBuf;

    //备份原始话单
    m_pOrgRec = new CDR3G_SGSNPDPRecord;
    if(m_pOrgRec == NULL)
    {
        return FALSE;
    }

    //复制话单记录
    memcpy(m_pOrgRec,p,sizeof(CDR3G_SGSNPDPRecord));

    /*
     * 下面单独复制指针字段
     */

    //复制listOfTrafficVolumes字段
    if((p->listOfTrafficVolumes.value != NULL)
        && (p->listOfTrafficVolumes.count > 0))
    {
        m_pOrgRec->listOfTrafficVolumes.value
            = Clone_CDR3G_ChangeOfCharCondition(
            p->listOfTrafficVolumes.value,
            p->listOfTrafficVolumes.count);

        m_pOrgRec->listOfTrafficVolumes.count = p->listOfTrafficVolumes.count;

        /* 
         * 统计链表的长度，用于控制合并的话单数。
         * Added by zhouyonghui,2001/09/25.
         */
        m_nLen_Condition_List = p->listOfTrafficVolumes.count;
    }

    //复制diagnostics字段
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

    //复制recordExtensions字段
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
             * 统计链表的长度，用于控制合并的话单数。
             * Added by zhouyonghui,2001/09/25.
             */
            m_nLen_Extension_List = p->recordExtensions.count;
        }
    }

    //设置当前话单序列号
    m_nMinRecordSequence = m_nCurRecordSequence = p->recordSequenceNumber;

    //设置PDP结束标志
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
     * 若recordSequenceNumber字段有效，则话单是部分话单。
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
* 函数名:   C3G_350_SCDR::CanSafeMerge
* 作者:    Zhou Yonghui
* 日期:    01-5-29
* 描述:    判断pBill指向的话单是否能够与当前话单合并。
*          若函数返回TRUE，则当前话单的Merge方法将被调用，
*          否则pBill话单将被送Qok。
* 输入: 
*       参数1 :CBill* pBill，待合并的话单。
* 返回值: BOOL 
* 说明: 
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*01-5-29       Zhou Yonghui
*01-9-25       Zhou Yonghui     增加了流量控制代码，
*                               超大话单强行中断合并。
*02-8-07       Zou Yongsheng    增加对于CAMEL域中的
                                freeFormatData超大的
								话单强行中断合并
******************************************************/
BOOL C3G_350_SCDR::CanSafeMerge(CBill* pBill)
{
    assert(pBill != NULL);

    C3G_350_SCDR* p = (C3G_350_SCDR*)pBill;

    //若话单的序列号在已经合并的话单范围内，说明是重复话单。
    //重复话单在Merge方法中丢弃。
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
     * 超大话单，强行送走。
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
	 * 合并CAMEL域中freeFormatData,
	 * 合并的话单的两者length和大于160
	 * 强行送走
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
     
    //若不是重复话单，则要求序列号连续。
    return (p->m_nCurRecordSequence == (m_nCurRecordSequence + 1));
}

BOOL C3G_350_SCDR::IsPDPCompleted()
{
    return m_bPDPCompleted;
}

BOOL C3G_350_SCDR::NeedWaiting()
{
    //当前的合并规则采用减法方式合并Duration字段
    //所以该函数只返回FALSE(当PDP结束时不需继续等待)。
    return FALSE;
}


/******************************************************
* 函数名:   C3G_350_SCDR::GetContents
* 作者:    Zhou Yonghui
* 日期:    01-5-29
* 描述:    函数按照最终话单格式组合话单记录。
* 输入: 
* 返回值: char*,符合最终话单格式的话单记录。
* 说明: 
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*01-5-29       Zhou Yonghui
*02-8-07       Zou Yongsheng    增加对CAMEL域的处理
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

    //初始化为0。
    memset(pDest,0,sizeof(_3GCDR_SGSNPDPRecord));
    m_pDestRec = pCallEventRec;

    //待拷贝内存块开始/结束地址。
    char* pBegin = NULL,*pEnd = NULL;

    /*
     * 若话单没有经过合并，则从原始话单中构造最终话单。
     */

    //合并前bit_mask是4个字节长，合并后是2个字节长，不能直接赋值。
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
	//颜文远2002.05.27添加，问题单号D13999
	if (m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_servedPDPAddress_present)
	{
		pDest->bit_mask |= _3GCDR_SGSNPDPRecord_servedPDPAddress_present;
	}
	//颜文远2002.05.27添加结束
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

        //复制listOfTrafficVolumes字段
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

        //复制diagnostics字段
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
         * recordSequenceNumber转换为链表
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

        //复制nodeID字段
        if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_nodeID_present)
        {
            memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(CDR3G_NodeID));
        }

        //复制recordExtensions字段
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
         * localSequenceNumber字段被丢弃
         */

        pBegin = (char*)(&(m_pOrgRec->apnSelectionMode));
        pEnd = (char*)(&(m_pOrgRec->rNCUnsentDownlinkVolume));

        /*
         * apnSelectionMode,accessPggsnPDPRecordointNameOI,servedMSISDN,
         * chargingCharacteristics,systemType, cAMELInformationPDP,
		 * rNCUnsentDownlinkVolume
         */
		//注:增加CAMEL域后，无需修改zouyongsheng
		memcpy(&(pDest->apnSelectionMode),pBegin,
            (pEnd - pBegin) + sizeof(CDR3G_DataVolumeGPRS));
        
        //复制causeForRecClosing字段。
		pDest->causeForRecClosing = m_pOrgRec->causeForRecClosing;
       	//consolidationResult字段
		switch(m_pOrgRec->causeForRecClosing)
		{
		case CDR3G_normalRelease:
			//PDP正常结束
			pDest->consolidationResult
				= _3GCDR_consolidationResult_normal;
			break;
			//SGSN改变
		case CDR3G_sGSNChange:
			pDest->consolidationResult
				= _3GCDR_consolidationResult_ForInterSGSNConsolidation;
			break;
		case CDR3G_abnormalRelease:
			//PDP异常结束
			pDest->consolidationResult
				= _3GCDR_consolidationResult_notnormal;
			break;
		default:
			//PDP未结束(置为PDP异常结束)
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
     * 下面是合并后话单的组合过程。
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

    //合并listOfTrafficVolumes字段
    if(m_pCondition != NULL)
    {
        pDest->listOfTrafficVolumes.value = (_3GCDR_ChangeOfCharCondition*)
            List_To_Array(m_pCondition,nCount/*in,out*/);
        
        pDest->listOfTrafficVolumes.count = nCount;

        //删除合并过程中使用的临时链表。
        Clear_List(m_pCondition);
        m_pCondition = m_pConditionTail = NULL;
    }
    
    /* 
     * recordOpeningTime,duration,sgsnChange,
     * causeForRecClosing
     */
    memcpy(&(pDest->recordOpeningTime),pBegin,(pEnd - pBegin));

    //合并diagnostics字段
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_diagnostics_present)
    {
        memcpy(&(pDest->diagnostics),&(m_Diagnostics),
            sizeof(CDR3G_Diagnostics));
    }

    //合并recordSequenceNumber字段
    if(m_pOrgRec->bit_mask
        & CDR3G_SGSNPDPRecord_recordSequenceNumber_present)
    {
        memcpy(&(pDest->recSequenceNumList.sgsnAddress),
            &(m_pOrgRec->sgsnAddress),sizeof(CDR3G_GSNAddress));

        /*
         * m_pSequence一定不为NULL。
         */
        pDest->recSequenceNumList.recSequenceNumber.value
            = List_To_Array(m_pSequence,nCount/*in,out*/);

        pDest->recSequenceNumList.recSequenceNumber.count = nCount;

        pDest->recSequenceNumList.bit_mask 
            |= _3GCDR_recSequenceNumber_present;

        Clear_List(m_pSequence);
        m_pSequence = m_pSequenceTail = NULL;
    }

    //nodeID字段(一个数组)
    if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_nodeID_present)
    {
        memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(_3GCDR_NodeID));
    }

    //合并recordExtensions字段
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
    //注：zouyongsheng 增加CAMEL域后在此不作处理。最后修改此域
	memcpy(&(pDest->apnSelectionMode),pBegin,
        (pEnd - pBegin) + sizeof(CDR3G_DataVolumeGPRS));

	//consolidationResult字段
    switch(m_CauseForRecClosing)
    {
    case CDR3G_normalRelease:
		//PDP正常结束
        pDest->consolidationResult
            = _3GCDR_consolidationResult_normal;
        break;
		//SGSN改变
	case CDR3G_sGSNChange:
		pDest->consolidationResult
            = _3GCDR_consolidationResult_ForInterSGSNConsolidation;
        break;        
    case CDR3G_abnormalRelease:
        //PDP异常结束
        pDest->consolidationResult
            = _3GCDR_consolidationResult_notnormal;
        break;
    default:
        //PDP未结束(置为PDP异常结束)
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
* 函数名:   C3G_350_SCDR::PrepareData
* 作者:    Zhou Yonghui
* 日期:    01-6-5
* 描述:    为合并准备数据。
* 输入:    无。
* 返回值: BOOL，成功返回TRUE，失败返回FALSE。
* 说明:    合并过程中需要把一些字段链接起来，形成一个链表，
*          合并完成后再把链表转换为数组。
*          该函数把数组转换为链表。
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*01-6-5        Zhou Yonghui
*02-8-7        Zou Yonsheng     增加对于CAMEL域的处理
******************************************************/
BOOL C3G_350_SCDR::PrepareData()
{
    //链接listOfTrafficVolumes
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

    //链接recordExtensions
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

    //链接recordSequenceNumber
    if(m_pOrgRec->bit_mask 
        & CDR3G_SGSNPDPRecord_recordSequenceNumber_present)
    {
        m_pSequence = RecordSequenceNumber_LINK_STRUCT::Alloc();

        m_pSequence->Value = m_pOrgRec->recordSequenceNumber;
        m_pSequenceTail = m_pSequence;
    }

    //复制duration字段
    m_Duration = m_pOrgRec->duration;

    //added by zouyongsheng 
	//复制cAMELInformationPDP
	if(m_pOrgRec->bit_mask
		& CDR3G_cAMELInformationPDP_present)
	{
		m_CAMELInformationPDP = m_pOrgRec->cAMELInformationPDP;
	}

	//复制rNCUnsentDownlinkVolume字段
    if(m_pOrgRec->bit_mask
        & CDR3G_rNCUnsentDownlinkVolume_present)
    {
        m_UnsentVolume = m_pOrgRec->rNCUnsentDownlinkVolume;
    }

    //复制causeForRecClosing字段
    m_CauseForRecClosing = m_pOrgRec->causeForRecClosing;

    //复制diagnostics字段
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

    //准备完成
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
* 函数名:   C3G_350_SCDR::Merge
* 作者:    Zhou Yonghui
* 日期:    01-5-30
* 描述:    把话单pBill合入当前话单中。
* 输入: 
*       参数1 :CBill* pBill，待合并的话单。
* 返回值: BOOL，合并成功返回TRUE，失败返回FALSE。
* 说明: 
*       合并过程中更新的字段有:
*       需要链接的字段(listOfTrafficVolumes,recordSequenceNumber,
*       recordExtensions),取最后一张话单值的字段(causeForRecClosing,
*       diagnostics),需要累加的字段(Duration,rNCUnsentDownlinkVolume)。
*       取第一张话单值的字段不变。
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*01-5-30       Zhou Yonghui
*01-9-25       Zhou Yonghui
*              增加了统计listOfTrafficVolumes和recordExtensions的代码，
*              以便进行流量控制。
*02-8-07       Zou Yongsheng     增加对于CAMEL域的处理
******************************************************/
BOOL C3G_350_SCDR::Merge(CBill* pBill)
{
    assert(pBill != NULL);

    if(!m_bPrepared)
    {
        if(!PrepareData())  return FALSE;
    }

    C3G_350_SCDR* p = (C3G_350_SCDR*)pBill;

    //丢弃重复话单。
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

    //等同于:
    //m_nCurRecordSequence = p->m_pOrgRec->recordSequenceNumber;
    m_nCurRecordSequence++;

    //设置PDP结束标志。
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

    //复制causeForRecClosing字段。
    m_CauseForRecClosing = p->m_pOrgRec->causeForRecClosing;

    //Z30971增加修改是否支持夏时制
	if(!m_bDSTFlg)
	{
		int nDiff = Get_Duration(&(m_pOrgRec->recordOpeningTime),
			&(p->m_pOrgRec->recordOpeningTime));
		m_Duration = nDiff;
	}
	//累加Duration字段。
	m_Duration += p->m_pOrgRec->duration;
	//修改结束

    //Added by ZhengYuqun 2003-11-04 SWPD02238
    if((UINT4)m_Duration >= m_uMaxDuration)
    {
        m_bPDPCompleted = TRUE;
    }
	
    //累加rNCUnsentDownlinkVolume字段。
    if(p->m_pOrgRec->bit_mask & CDR3G_rNCUnsentDownlinkVolume_present)
    {
        //Added by ZhengYuqun 2003-11-03 SWPD02228
        if(m_UnsentVolume == 0)  //当第一次NCUnsentDownlinkVolum不为空的时候置位
        {
            m_pOrgRec->bit_mask |= CDR3G_rNCUnsentDownlinkVolume_present;
        }
        //End SWPD02228
        m_UnsentVolume += p->m_pOrgRec->rNCUnsentDownlinkVolume;
    }

    //链接listOfTrafficVolumes字段。
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
         * 统计链表的长度，用于控制合并的话单数。
         * Added by zhouyonghui,2001/09/25.
         */
        m_nLen_Condition_List += q->nCount;
    }

    /*
    //链接recordExtensions字段。
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
             * 统计链表的长度，用于控制合并的话单数。
             * Added by zhouyonghui,2001/09/25.
             
            m_nLen_Extension_List += q->nCount;
        }
    }
    */


    //链接recordSequenceNumber字段。
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

    //diagnostics字段取最后一张话单的值。
    if((p->m_pOrgRec->bit_mask 
        & CDR3G_SGSNPDPRecord_diagnostics_present))
        //&& (m_pOrgRec->bit_mask          //这两行去掉 ZhengYuqun 2003-11-03 SWPD02228
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

        if(m_pOrgRec->bit_mask & CDR3G_SGSNPDPRecord_diagnostics_present) //增加一个if ZhengYuqun 2003-11-03 SWPD02228
        {
            /*回收m_Diagnostics的空间。*/
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
            m_pOrgRec->bit_mask |= CDR3G_SGSNPDPRecord_diagnostics_present; //增加一行 ZhengYuqun 2003-11-03 SWPD02228
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

	//added by zouyongsheng 合并CAMELInformation域的信息
    if((p->m_pOrgRec->bit_mask 
		& CDR3G_cAMELInformationPDP_present)
		&& (m_pOrgRec->bit_mask
		& CDR3G_cAMELInformationPDP_present))
	{
		/*
		 * sCFAddress,serviceKey,defaultTransactionHandling,
		 * cAMELAccessPointNameNI,cAMELAccessPointNameOI
		 * 以上域取第一张部分话单的内容
		 */

		/*
		 * numberOfDPEncountered,levelOfCAMELService
		 * 以上域取最后一张部分话单的内容
		 */
		m_CAMELInformationPDP.numberOfDPEncountered = 
				p->m_pOrgRec->cAMELInformationPDP.numberOfDPEncountered;

		m_CAMELInformationPDP.levelOfCAMELService = 
				p->m_pOrgRec->cAMELInformationPDP.levelOfCAMELService;
				
		/*
		 * fFDAppendIndicator字段存在且值为TRUE且freeFormatData
		 * 信息长度不为零，则和前面的话单合并
		 */
		//fFDAppendIndicator 字段不存或者该域为FALSE
		//freeFormatData存在而且信息长度不为零
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
		
		//fFDAppendIndicator 字段存在且值为TRUE
		//freeFormatData存在而且信息长度不为零
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
     * 其他字段取第一张话单的值。
     */
    return TRUE;
}


//C3G_350_GCDR类实现(S-CDR)
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
    //回收原始话单记录空间。
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

    //回收最终话单记录空间。
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
     * 合并过程中使用的数据结构的空间已经在GetContents中回收。
     */
}

BOOL C3G_350_GCDR::PrepareData()
{
    //转换sgsnAddress
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

    //链接listOfTrafficVolumes
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

    //链接recordExtensions
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

    //链接recordSequenceNumber
    if(m_pOrgRec->bit_mask 
        & CDR3G_GGSNPDPRecord_recordSequenceNumber_present)
    {
        m_pSequence = RecordSequenceNumber_LINK_STRUCT::Alloc();

        m_pSequence->Value = m_pOrgRec->recordSequenceNumber;
        m_pSequenceTail = m_pSequence;
    }

    //复制duration字段
    m_Duration = m_pOrgRec->duration;

    //复制causeForRecClosing字段
    m_CauseForRecClosing = m_pOrgRec->causeForRecClosing;

    //复制diagnostics字段
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

    //准备完成
    m_bPrepared = TRUE;
    return TRUE;
}


/******************************************************
* 函数名:   C3G_350_GCDR::Init
* 作者:    Zhou Yonghui
* 日期:    01-5-28
* 描述:    
* 输入: 
*       参数1 :char* pBuf，G-CDR话单记录。
*       参数2 :int nLen，话单记录的长度。
* 返回值: BOOL 初始化成功返回TRUE，内存操作失败抛出异常。
* 说明: 
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*01-5-28       Zhou Yonghui
*01-9-25       Zhou Yonghui
*              增加了初始化m_nLen_SGSNAddress_List、m_nLen_Condition_List和
*              m_nLen_Extension_List的代码，以便进行流量控制。
******************************************************/
BOOL C3G_350_GCDR::Init(char* pBuf,UINT4 uLen)
{
    assert(pBuf != NULL);

    //话单记录的所有字段都被存放在连续的内存块中
    CDR3G_GGSNPDPRecord* p = (CDR3G_GGSNPDPRecord*)pBuf;

    //备份原始话单
    m_pOrgRec = new CDR3G_GGSNPDPRecord;
    if(m_pOrgRec == NULL)
    {
        throw "Alloc memory fail.";
    }

    memcpy(m_pOrgRec,p,sizeof(CDR3G_GGSNPDPRecord));

    /*
     * 下面单独复制指针字段
     */

    //复制sgsnAddress
    if((p->sgsnAddress.value != NULL) && (p->sgsnAddress.count > 0))
    {
        m_pOrgRec->sgsnAddress.value = Clone_CDR3G_GSNAddress(
            p->sgsnAddress.value,
            p->sgsnAddress.count);
        
        m_pOrgRec->sgsnAddress.count = p->sgsnAddress.count;

        /* 
         * 统计链表的长度，用于控制合并的话单数。
         * Added by zhouyonghui,2001/09/25.
         */
        m_nLen_SGSNAddress_List = p->sgsnAddress.count;
    }

    //复制listOfTrafficVolumes字段
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
         * 统计链表的长度，用于控制合并的话单数。
         * Added by zhouyonghui,2001/09/25.
         */
        m_nLen_Condition_List = p->listOfTrafficVolumes.count;
    }

    //复制diagnostics字段
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

    //复制recordExtensions字段
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
             * 统计链表的长度，用于控制合并的话单数。
             * Added by zhouyonghui,2001/09/25.
             */
            m_nLen_Extension_List = p->recordExtensions.count;
        }
    }

    //设置当前话单序列号
    m_nMinRecordSequence = m_nCurRecordSequence = p->recordSequenceNumber;

    //设置PDP结束标志
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
     * 若recordSequenceNumber字段有效，则话单是部分话单。
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
* 函数名:   C3G_350_GCDR::CanSafeMerge
* 作者:    Zhou Yonghui
* 日期:    01-5-28
* 描述:    
* 输入: 
*       参数1 :CBill* pBill,待合并的话单对象指针。
* 返回值: BOOL ,若两张话单允许合并，返回TRUE，否则返回FALSE。
* 说明: 
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*01-5-28       Zhou Yonghui
*01-9-25       Zhou Yonghui     增加了流量控制代码，
*                               超大话单强行中断合并。
******************************************************/
BOOL C3G_350_GCDR::CanSafeMerge(CBill* pBill)
{
    assert(pBill != NULL);

    C3G_350_GCDR* p = (C3G_350_GCDR*)pBill;

    //若话单的序列号在已经合并的话单范围内，说明是重复话单。
    //重复话单在Merge方法中丢弃。
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
     * 超大话单，强行送走。
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

    //若序列号不连续，不允许合并。
    return (p->m_nCurRecordSequence == (m_nCurRecordSequence + 1));
}

BOOL C3G_350_GCDR::IsPDPCompleted()
{
    return m_bPDPCompleted;
}

BOOL C3G_350_GCDR::NeedWaiting()
{
    //当前的合并规则采用减法方式合并Duration字段
    //所以该函数只返回FALSE(当PDP结束时不需继续等待)。
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

    //待拷贝内存块开始/结束地址。
    char* pBegin = NULL,*pEnd = NULL;

    /*
     * 若话单没有经过合并，则从原始话单中构造最终话单。
     */

    //合并前bit_mask是4个字节长，合并后是2个字节长，不能直接赋值。
    if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_networkInitiation_present)
    {
        pDest->bit_mask |= _3GCDR_GGSNPDPRecord_networkInitiation_present;
    }
	//颜文远2002.05.27添加，问题单号D13999
	if (m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_servedPDPAddress_present)
	{
		pDest->bit_mask |= _3GCDR_GGSNPDPRecord_servedPDPAddress_present;
	}
	//颜文远2002.05.27添加结束
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

        //复制sgsnAddress字段
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

        //复制listOfTrafficVolumes字段
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
        
        //复制diagnostics字段
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
         * recordSequenceNumber转换为链表
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
                //若SGSN地址无效，把链表中的SGSN地址设置为空
                memset(&(pDest->recSequenceNumList.sgsnAddress),
                    0,sizeof(CDR3G_GSNAddress));
            }
        }

        //复制nodeID字段(一个数组)
        if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_nodeID_present)
        {
            memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(CDR3G_NodeID));
        }

        //复制recordExtensions字段
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
         * localSequenceNumber字段被丢弃
         */

        pBegin = (char*)(&(m_pOrgRec->apnSelectionMode));
        pEnd = (char*)(&(m_pOrgRec->chargingCharacteristics));
        /* 
         * apnSelectionMode,servedMSISDN,chargingCharacteristics
         */
        memcpy(&(pDest->apnSelectionMode),pBegin,
            (pEnd - pBegin) + sizeof(CDR3G_ChargingCharacteristics));
        
		//复制causeForRecClosing字段。
		pDest->causeForRecClosing = m_pOrgRec->causeForRecClosing;
        
		//consolidationResult字段
		switch(m_pOrgRec->causeForRecClosing)
		{
		case CDR3G_normalRelease:
			 //PDP正常结束
			pDest->consolidationResult
				= _3GCDR_consolidationResult_normal;
			break;
			//SGSN改变
		case CDR3G_sGSNChange:
			pDest->consolidationResult
				= _3GCDR_consolidationResult_ForInterSGSNConsolidation;
			break;     
		case CDR3G_abnormalRelease:
			//PDP异常结束
			pDest->consolidationResult
				= _3GCDR_consolidationResult_notnormal;
			break;
		default:
			//PDP未结束(置为PDP异常结束)
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
     * 下面是合并后话单的组合过程。
     */

    pBegin = (char*)(&(m_pOrgRec->recordType));
    pEnd = (char*)(&(m_pOrgRec->sgsnAddress));

    /* 
     * recordType,networkInitiation,servedIMSI,
     * ggsnAddress,chargingID
     */
    memcpy(&(pDest->recordType),pBegin,(pEnd - pBegin));
    
    unsigned int nCount = 0;
    //合并sgsnAddress字段
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

    //合并listOfTrafficVolumes字段
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

    //合并diagnostics字段
    if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_diagnostics_present)
    {
        memcpy(&(pDest->diagnostics),&(m_Diagnostics),
            sizeof(CDR3G_Diagnostics));
        /*
         * m_Diagnostics维护的指针转给了pDest。
         */
    }

    //合并recordSequenceNumber字段
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
            //若SGSN地址无效。
            memset(&(pDest->recSequenceNumList.sgsnAddress),
                0,sizeof(CDR3G_GSNAddress));
        }

        /*
         * m_pSequence肯定不为空。
         */
        pDest->recSequenceNumList.recSequenceNumber.value = 
            List_To_Array(m_pSequence,nCount/*in,out*/);

        pDest->recSequenceNumList.bit_mask
            |= _3GCDR_recSequenceNumber_present;
        pDest->recSequenceNumList.recSequenceNumber.count = nCount;

        Clear_List(m_pSequence);
        m_pSequence = m_pSequenceTail = NULL;
    }

    //复制nodeID字段(一个数组)
    if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_nodeID_present)
    {
        memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(CDR3G_NodeID));
    }

    //合并recordExtensions字段
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

    //consolidationResult字段
    switch(m_CauseForRecClosing)
    {
    case CDR3G_normalRelease:
		//PDP正常结束
        pDest->consolidationResult
            = _3GCDR_consolidationResult_normal;
        break;
		//SGSN改变
	case CDR3G_sGSNChange:
		pDest->consolidationResult
            = _3GCDR_consolidationResult_ForInterSGSNConsolidation;
        break;         
    case CDR3G_abnormalRelease:
        //PDP异常结束
        pDest->consolidationResult
            = _3GCDR_consolidationResult_notnormal;
        break;
    default:
        //PDP未结束(置为PDP异常结束)
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
* 函数名:   C3G_350_GCDR::Merge
* 作者:    Zhou Yonghui
* 日期:    01-5-30
* 描述:    把话单pBill合入当前话单中。
* 输入: 
*       参数1 :CBill* pBill，待合并的话单。
* 返回值: BOOL，合并成功返回TRUE，失败返回FALSE。
* 说明: 
*       合并过程中更新的字段有:
*       需要链接的字段(sgsnAddress,listOfTrafficVolumes,
*       recordSequenceNumber,recordExtensions),
*       取最后一张话单值的字段(causeForRecClosing,diagnostics),
*       需要累加的字段(Duration)。
*       取第一张话单值的字段不变。
*
*       内存操作失败将抛出异常。
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*01-5-30       Zhou Yonghui
*01-9-25       Zhou Yonghui
*              增加了统计sgsnAddress、listOfTrafficVolumes和
*              recordExtensions的代码，以便进行流量控制。
******************************************************/
BOOL C3G_350_GCDR::Merge(CBill* pBill)
{
    assert(pBill != NULL);

    if(!m_bPrepared)
    {
        if(!PrepareData())  return FALSE;
    }

    C3G_350_GCDR* p = (C3G_350_GCDR*)pBill;

    //丢弃重复话单。
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

    //等同于:
    //m_nCurRecordSequence = p->m_pOrgRec->recordSequenceNumber;
    m_nCurRecordSequence++;

    //设置PDP结束标志。
    switch(p->m_pOrgRec->causeForRecClosing)
    {
    case CDR3G_normalRelease:
    case CDR3G_abnormalRelease:
        m_bPDPCompleted = TRUE;
        break;
    default:
        break;
    }

    //复制causeForRecClosing字段。
    m_CauseForRecClosing = p->m_pOrgRec->causeForRecClosing;

    //Z30971增加修改是否支持夏时制
	if(!m_bDSTFlg)
	{
		int nDiff = Get_Duration(&(m_pOrgRec->recordOpeningTime),
			&(p->m_pOrgRec->recordOpeningTime));
		m_Duration = nDiff;
	}
	//累加Duration字段。
	m_Duration += p->m_pOrgRec->duration;
	//修改结束

    //Added by ZhengYuqun 2003-11-04 SWPD02238
    if((UINT4)m_Duration >= m_uMaxDuration)
    {
        m_bPDPCompleted = TRUE;
    }

    //链接sgsnAddress字段。
    
    //2002-01-18,Modified by zhouyonghui.
    //剔除重复的SGSN地址.

    if((p->m_pOrgRec->sgsnAddress.value != NULL)
        && (p->m_pOrgRec->sgsnAddress.count > 0))
    {
        unsigned int nCount = p->m_pOrgRec->sgsnAddress.count;

        if(m_pSGSNAddressTail == NULL)
        {
            //这种情况不用判断重复SGSN地址

            SGSGAddress_LINK_STRUCT* q = SGSGAddress_LINK_STRUCT::Alloc();

            q->Value = Clone_CDR3G_GSNAddress(
                p->m_pOrgRec->sgsnAddress.value,nCount/*in*/);

            q->nCount = nCount;
            m_pSGSNAddressTail = m_pSGSNAddress = q;
        }
        else
        {
            //这时判断是否有重复SGSN地址

            SGSGAddress_LINK_STRUCT* q = SGSGAddress_LINK_STRUCT::Alloc();

            q->Value = Clone_CDR3G_GSNAddress(
                &(m_pSGSNAddressTail->Value[m_pSGSNAddressTail->nCount - 1]),
                p->m_pOrgRec->sgsnAddress.value,nCount/*in,out*/);

            q->nCount = nCount;
            if(q->Value == NULL)
            {
                //此时nCount为0
                delete q;
            }
            else
            {
                m_pSGSNAddressTail->m_pNext = q;
                m_pSGSNAddressTail = q;
            }
        }

        /* 
         * 统计链表的长度，用于控制合并的话单数。
         * Added by zhouyonghui,2001/09/25.
         */
        m_nLen_SGSNAddress_List += nCount;
    }

    //链接listOfTrafficVolumes字段。
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
         * 统计链表的长度，用于控制合并的话单数。
         * Added by zhouyonghui,2001/09/25.
         */
        m_nLen_Condition_List += q->nCount;
    }

    //链接recordExtensions字段。
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
             * 统计链表的长度，用于控制合并的话单数。
             * Added by zhouyonghui,2001/09/25.
             */
            m_nLen_Extension_List += q->nCount;
        }
    }

    //链接recordSequenceNumber字段。
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

    //diagnostics字段取最后一张话单的值。
    if((p->m_pOrgRec->bit_mask
        & CDR3G_GGSNPDPRecord_diagnostics_present))
        //&& (m_pOrgRec->bit_mask     //这两行去掉  ZhengYuqun 2003-11-03 SWPD02228
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

        /*回收原diagnostics字段的空间。*/
        if(m_pOrgRec->bit_mask & CDR3G_GGSNPDPRecord_diagnostics_present) //增加一个if ZhengYuqun 2003-11-03 SWPD02228
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
            m_pOrgRec->bit_mask |= CDR3G_GGSNPDPRecord_diagnostics_present; //增加一行 ZhengYuqun 2003-11-03 SWPD02228
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
     * 其他字段取第一张话单的值。
     */
    return TRUE;
}
