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
*若解析成功，返回TRUE，失败则抛出一个异常。
*若话单类型不能识别，则返回FALSE。
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
                //用话单记录初始化话单对象，失败将抛出异常。
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
    //3G需要合并的话单有二种：S-CDR,G-CDR。
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
* 函数名:   CR99_BillParser::RegisterBillType
* 作者:    GGF
* 日期:    2003-09-28
* 描述:    向CMerger对象注册话单类型。
* 输入: 
*       参数1 :CMerger* pMerger
* 返回值: void 
* 说明:     若内存操作失败将抛出异常。
* 更新记录: 
*
* 日期            姓名                  描述 
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


//CR99_SCDR类实现(S-CDR)
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
    //回收原始话单记录空间。
    if(m_pOrgRec != NULL)
    {
        delete[] m_pOrgRec->listOfTrafficVolumes.value;
		
        if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_diagnostics_present)
        {
            Clear_Diagnostics(&(m_pOrgRec->diagnostics));
        }

        if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_recordExtensions_present)
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
		//删除sgsnAddress链表
		delete[] m_pDestRec->u.sgsnPDPRecord.sgsnAddress.value;

		//删除listOfTrafficVolumes链表
        delete[] m_pDestRec->u.sgsnPDPRecord.listOfTrafficVolumes.value;

		//删除Diagnostics
        if(m_pDestRec->u.sgsnPDPRecord.bit_mask
            & CMCC_3GCDR_CG_SGSNPDPRecord_diagnostics_present)
        {
            Clear_Diagnostics((CMCC_3GCDR_Diagnostics*)&
                (m_pDestRec->u.sgsnPDPRecord.diagnostics));
        }

		//删除recordExtensions
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

		//删除recSequenceNumList
        if(m_pDestRec->u.sgsnPDPRecord.bit_mask
            & CMCC_3GCDR_CG_SGSNPDPRecord_recSequenceNumList_present)
        {
            delete[] m_pDestRec->u.sgsnPDPRecord.
                recSequenceNumList.value->recSequenceNumber.value;
			delete[] m_pDestRec->u.sgsnPDPRecord.recSequenceNumList.value;
        }

		//删除localSequenceNumber
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
     * 合并过程中使用的数据结构的空间已经在GetContents中回收。
     */
}


/******************************************************
* 函数名:   CR99_SCDR::Init
* 作者:    GGF
* 日期:    2003-09-28
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
*
******************************************************/
BOOL CR99_SCDR::Init(char* pBuf,UINT4 uLen)
{
    assert(pBuf != NULL);
    
    CMCC_3GCDR_SGSNPDPRecord* p = (CMCC_3GCDR_SGSNPDPRecord*)pBuf;

    //备份原始话单
    m_pOrgRec = new CMCC_3GCDR_SGSNPDPRecord;
    if(m_pOrgRec == NULL)
    {
        return FALSE;
    }

    //复制话单记录
    memcpy(m_pOrgRec,p,sizeof(CMCC_3GCDR_SGSNPDPRecord));

    /*
     * 下面单独复制指针字段
     */

    //复制listOfTrafficVolumes字段
    if((p->listOfTrafficVolumes.value != NULL)
        && (p->listOfTrafficVolumes.count > 0))
    {
        m_pOrgRec->listOfTrafficVolumes.value
            = Clone_ChangeOfCharCondition(
            p->listOfTrafficVolumes.value,
            p->listOfTrafficVolumes.count);

        m_pOrgRec->listOfTrafficVolumes.count = p->listOfTrafficVolumes.count;

        /* 
         * 统计链表的长度，用于控制合并的话单数。
         * 
         */
        m_nLen_Condition_List = p->listOfTrafficVolumes.count;
    }

    //复制diagnostics字段
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

    //复制recordExtensions字段
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
             * 统计链表的长度，用于控制合并的话单数。
             * 
             */
            m_nLen_Extension_List = p->recordExtensions.count;
        }
    }

	/* 
     * 统计链表的长度，用于控制合并的话单数。
     * 
     */
	m_nLen_SGSNAddress_List = 1;

    //设置当前话单序列号
    m_nMinRecordSequence = m_nCurRecordSequence = p->recordSequenceNumber;

	//通话时长
	m_Duration = p->duration;

    //设置PDP结束标志
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

    //增加按通话时长结束合并 ZhengYuqun 2003-12-18
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

    //增加按通话时长结束合并 ZhengYuqun 2003-12-18
    if((UINT4)m_pOrgRec->duration >= m_uMaxDuration)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL CR99_SCDR::IsPartial()
{
    /* 
     * 若recordSequenceNumber字段有效，则话单是部分话单。
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
* 函数名:   CR99_SCDR::CanSafeMerge
* 作者:    GGF
* 日期:    2003-09-28
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

    /* added by ggf, 2003-12-24, 在同一SGSN上多次切换带来的话单判重问题,
	 * 增加判断是否落在已经合并话单的时间范围内：
	 * 1.后续话单落在当前话单的时间范围之外，不合并
	 * 2.后续话单落在当前时间范围之内，属重复话单，重复话单在Merge方法中丢弃。
	 * 3.后续话单打开时间与当前话单打开时间之差落在[m_Duration, m_Duration + SGSN_CHANGE_TIME]
	 * 才认为是连续话单，进行合并
	 */
	int nDiff = Get_Duration(&(m_pOrgRec->recordOpeningTime), &(p->m_pOrgRec->recordOpeningTime));
	if ((nDiff < 0) || (nDiff > (m_Duration + SGSN_CHANGE_TIME))) 
		return FALSE;
	else if	((nDiff >= 0) && (nDiff < m_Duration)) 
		return TRUE;

    /* 
     * 超大话单，强行送走。
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
       //SGSN如果已经发生切换, 则要求待合并话单的话终原因为sGSNChange，
       //合并话单的sgsnChange字段置位，才允许合并
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
	 * 合并CAMEL域中freeFormatData,
	 * 合并的话单的两者length和大于160
	 * 强行送走
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
    //当前的合并规则采用减法方式合并Duration字段
    //所以该函数只返回FALSE(当PDP结束时不需继续等待)。
    return FALSE;
}


/******************************************************
* 函数名:   CR99_SCDR::GetContents
* 作者:    GGF
* 日期:    2003-09-28
* 描述:    函数按照最终话单格式组合话单记录。
* 输入: 
* 返回值: char*,符合最终话单格式的话单记录。
* 说明: 
* 更新记录: 
*
* 日期            姓名                  描述 
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

    //初始化为0。
    memset(pDest,0,sizeof(CMCC_3GCDR_CG_SGSNPDPRecord));
    m_pDestRec = pCallEventRec;

    //待拷贝内存块开始/结束地址。
    char* pBegin = NULL,*pEnd = NULL;

    /*
     * 若话单没有经过合并，则从原始话单中构造最终话单。
     */

    //合并前bit_mask是4个字节长，合并后是2个字节长，不能直接赋值。
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

		//复制sgsnAddress字段
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

        //复制listOfTrafficVolumes字段
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

        //复制diagnostics字段
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
         * recordSequenceNumber转换为链表
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

        //复制nodeID字段
        if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_nodeID_present)
        {
            memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(CMCC_3GCDR_NodeID));
        }

        //复制recordExtensions字段
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
         * localSequenceNumber转换为链表
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
         * rNCUnsentDownlinkVolume转换为链表
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


        //consolidationResult字段
        switch(m_pOrgRec->causeForRecClosing)
        {
        case CMCC_3GCDR_normalRelease:
            pDest->consolidationResult = CMCC_3GCDR_CG_normal;
            break;
        case CMCC_3GCDR_sGSNChange:
            //PDP正常结束
            pDest->consolidationResult = CMCC_3GCDR_CG_forInterSGSNConsolidation;
            break;
        case CMCC_3GCDR_abnormalRelease:
            //PDP异常结束
            pDest->consolidationResult = CMCC_3GCDR_CG_notnormal;
            break;
        case CMCC_3GCDR_timeLimit:
            pDest->consolidationResult = CMCC_3GCDR_CG_reachLimit;
        default:
            //PDP未结束(置为PDP异常结束)
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
     * 下面是合并后话单的组合过程。
     */

    pBegin = (char*)(&(m_pOrgRec->recordType));
	pEnd = (char*)(&(m_pOrgRec->sgsnAddress));

	/*
     * recordType,networkInitiation,servedIMSI,
     * servedIMEI,sgsnAddress
     */
    memcpy(&(pDest->recordType),pBegin,(pEnd - pBegin));

	//合并sgsnAddress字段
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

    //合并listOfTrafficVolumes字段
    if(m_pCondition != NULL)
    {
		typedef LINK_STRUCT2<CMCC_3GCDR_ChangeOfCharCondition> link_Struct2;
		unsigned int nCount = 0;
        pDest->listOfTrafficVolumes.value = 
            List_To_Array2<CMCC_3GCDR_CG_ChangeOfCharCondition, link_Struct2>
			(m_pCondition,nCount/*in,out*/);
        
        pDest->listOfTrafficVolumes.count = nCount;

        //删除合并过程中使用的临时链表。
        Clear_List2(m_pCondition);
        m_pCondition = m_pConditionTail = NULL;
    }
    
    /* 
     * recordOpeningTime,duration,sgsnChange,
     * causeForRecClosing
     */
    memcpy(&(pDest->recordOpeningTime),pBegin,(pEnd - pBegin));

    //合并diagnostics字段
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_diagnostics_present)
    {
        memcpy(&(pDest->diagnostics),&(m_Diagnostics),
            sizeof(CMCC_3GCDR_Diagnostics));
    }

    //合并recordSequenceNumber字段
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

    //nodeID字段(一个数组)
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_nodeID_present)
    {
        memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(CMCC_3GCDR_NodeID));
    }

    //合并recordExtensions字段
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

	//合并localSequenceNumber字段
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

	//合并rNCUnsentDownlinkVolume字段
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

    //consolidationResult字段
    switch(m_CauseForRecClosing)
    {
    case CMCC_3GCDR_normalRelease:
        pDest->consolidationResult = CMCC_3GCDR_CG_normal;
        break;
    case CMCC_3GCDR_sGSNChange:
        //PDP正常结束
        pDest->consolidationResult = CMCC_3GCDR_CG_forInterSGSNConsolidation;
        break;
    case CMCC_3GCDR_abnormalRelease:
        //PDP异常结束
        pDest->consolidationResult = CMCC_3GCDR_CG_notnormal;
        break;
    case CMCC_3GCDR_timeLimit:
        pDest->consolidationResult = CMCC_3GCDR_CG_reachLimit;
    default:
        //PDP未结束(置为PDP异常结束)
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
* 函数名:   CR99_SCDR::PrepareData
* 作者:    GGF
* 日期:    2003-09-28
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
*
******************************************************/
BOOL CR99_SCDR::PrepareData()
{
	//转换SCDR中的sgsnAddress
    m_pSGSNAddress = LINK_STRUCT2<CMCC_3GCDR_GSNAddress>::Alloc();
	m_pSGSNAddress->Value = new CMCC_3GCDR_GSNAddress;
	if(m_pSGSNAddress->Value == NULL)
	{
		throw "Alloc memory fail.";
	}
	memcpy(m_pSGSNAddress->Value, &(m_pOrgRec->sgsnAddress), sizeof(CMCC_3GCDR_GSNAddress));
    m_pSGSNAddress->nCount = 1;
    m_pSGSNAddressTail = m_pSGSNAddress;

    //链接listOfTrafficVolumes
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

    //链接recordExtensions
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

    //链接recordSequenceNumber
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

	//链接localSequenceNumber
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

	//链接rNCUnsentDownlinkVolume
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

    //复制duration字段
    m_Duration = m_pOrgRec->duration;

	//复制cAMELInformationPDP
	if(m_pOrgRec->bit_mask
		& CMCC_3GCDR_cAMELInformationPDP_present)
	{
		m_CAMELInformationPDP = m_pOrgRec->cAMELInformationPDP;
	}

    //复制causeForRecClosing字段
    m_CauseForRecClosing = m_pOrgRec->causeForRecClosing;

    //复制diagnostics字段
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

    //准备完成
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
* 函数名:   CR99_SCDR::Merge
* 作者:    GGF
* 日期:    2003-09-28
* 描述:    把话单pBill合入当前话单中。
* 输入: 
*       参数1 :CBill* pBill，待合并的话单。
* 返回值: BOOL，合并成功返回TRUE，失败返回FALSE。
* 说明: 
*       合并过程中更新的字段有:
*       需要链接的字段(sgsnAddress,listOfTrafficVolumes,recordSequenceNumber,rNCUnsentDownlinkVolume,
*       recordExtensions),取最后一张话单值的字段(causeForRecClosing,
*       diagnostics),需要累加的字段(Duration)。
*       取第一张话单值的字段不变。
* 更新记录: 
*
* 日期            姓名                  描述 
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

    //丢弃重复话单。
    int nDiff = Get_Duration(&(m_pOrgRec->recordOpeningTime), &(p->m_pOrgRec->recordOpeningTime));
	if	((nDiff >= 0) && (nDiff < m_Duration)) 
		return TRUE;

    //等同于:
    //m_nCurRecordSequence = p->m_pOrgRec->recordSequenceNumber;
    //m_nCurRecordSequence++;

    //设置PDP结束标志。
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

    //复制causeForRecClosing字段。
    m_CauseForRecClosing = p->m_pOrgRec->causeForRecClosing;
    if(p->m_pOrgRec->bit_mask & CMCC_3GCDR_rNCUnsentDownlinkVolume_present)
    {
        m_pOrgRec->bit_mask |= CMCC_3GCDR_rNCUnsentDownlinkVolume_present;
    }

    //是否支持夏时制
	if(!m_bDSTFlg)
	{
		int nDiff = Get_Duration(&(m_pOrgRec->recordOpeningTime),
			&(p->m_pOrgRec->recordOpeningTime));
		m_Duration = nDiff;
	}
	//累加Duration字段。
	m_Duration += p->m_pOrgRec->duration;
	
	//链接sgsnAddress字段, 判断待合并SCDR的SGSN地址是否在链表中已存在，需要剔重  
    unsigned int nCount = 1;
    LINK_STRUCT2<CMCC_3GCDR_GSNAddress>* q = LINK_STRUCT2<CMCC_3GCDR_GSNAddress>::Alloc();

    q->Value = Clone_GSNAddress(&(m_pSGSNAddressTail->Value[m_pSGSNAddressTail->nCount - 1]), &(p->m_pOrgRec->sgsnAddress),nCount/*in,out*/);

    q->nCount = nCount;

    BOOL bSgsnChange = FALSE;    
    if(q->Value == NULL)
    {
		//此时nCount为0
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
     * 统计链表的长度，用于控制合并的话单数。
     * 
     */
    m_nLen_SGSNAddress_List += nCount;

    //链接listOfTrafficVolumes字段。
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
         * 统计链表的长度，用于控制合并的话单数。
         * 
         */
        m_nLen_Condition_List += q->nCount;
    }

    /*
    //链接recordExtensions字段。
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
             * 统计链表的长度，用于控制合并的话单数。
             * 
             
            m_nLen_Extension_List += q->nCount;
        }
    }
    */

    //链接recordSequenceNumber字段。
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

	//链接localSequenceNumber字段。
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

    //链接rNCUnsentDownlinkVolume字段。
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

    //diagnostics字段取最后一张话单的值。
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

        /*回收m_Diagnostics的空间。*/
        if(m_pOrgRec->bit_mask & CMCC_3GCDR_SGSNPDPRecord_diagnostics_present) //增加一个if YanYong 2004-04-26 SWPD04795
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
            m_pOrgRec->bit_mask |= CMCC_3GCDR_SGSNPDPRecord_diagnostics_present; //增加一行 YanYong 2004-04-26 SWPD04795
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

	//合并CAMELInformation域的信息
    if((p->m_pOrgRec->bit_mask 
		& CMCC_3GCDR_cAMELInformationPDP_present)
		&& (m_pOrgRec->bit_mask
		& CMCC_3GCDR_cAMELInformationPDP_present))
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
		
		//fFDAppendIndicator 字段存在且值为TRUE
		//freeFormatData存在而且信息长度不为零
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
   
    //增加按通话时长结束合并 ZhengYuqun 2003-12-18
    if((UINT4)m_Duration >= m_uMaxDuration)
    {
        m_bPDPCompleted = TRUE;
    }

    /*
     * 其他字段取第一张话单的值。
     */
    return TRUE;
}


//CR99_GCDR类实现(G-CDR)
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
    //回收原始话单记录空间。
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

    //回收最终话单记录空间。
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
     * 合并过程中使用的数据结构的空间已经在GetContents中回收。
     */
}

BOOL CR99_GCDR::PrepareData()
{
    //转换sgsnAddress
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

    //链接listOfTrafficVolumes
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

    //链接recordExtensions
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

    //链接recordSequenceNumber
    if(m_pOrgRec->bit_mask 
        & CMCC_3GCDR_GGSNPDPRecord_recordSequenceNumber_present)
    {
        m_pSequence = LINK_STRUCT1<int>::Alloc();

        m_pSequence->Value = m_pOrgRec->recordSequenceNumber;
        m_pSequenceTail = m_pSequence;
    }

	//链接localSequenceNumber
    if(m_pOrgRec->bit_mask 
        & CMCC_3GCDR_GGSNPDPRecord_localSequenceNumber_present)
    {
        m_pLocalSequenceNumber = LINK_STRUCT1<int>::Alloc();

        m_pLocalSequenceNumber->Value = m_pOrgRec->localSequenceNumber;
        m_pLocalSequenceNumberTail = m_pLocalSequenceNumber;
    }

    //复制duration字段
    m_Duration = m_pOrgRec->duration;

    //复制causeForRecClosing字段
    m_CauseForRecClosing = m_pOrgRec->causeForRecClosing;

    //复制diagnostics字段
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

    //准备完成
    m_bPrepared = TRUE;
    return TRUE;
}


/******************************************************
* 函数名:   CR99_GCDR::Init
* 作者:    GGF
* 日期:    2003-09-28
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
*
******************************************************/
BOOL CR99_GCDR::Init(char* pBuf,UINT4 uLen)
{
    assert(pBuf != NULL);

    //话单记录的所有字段都被存放在连续的内存块中
    CMCC_3GCDR_GGSNPDPRecord* p = (CMCC_3GCDR_GGSNPDPRecord*)pBuf;

    //备份原始话单
    m_pOrgRec = new CMCC_3GCDR_GGSNPDPRecord;
    if(m_pOrgRec == NULL)
    {
        throw "Alloc memory fail.";
    }

    memcpy(m_pOrgRec,p,sizeof(CMCC_3GCDR_GGSNPDPRecord));

    /*
     * 下面单独复制指针字段
     */

    //复制sgsnAddress
    if((p->sgsnAddress.value != NULL) && (p->sgsnAddress.count > 0))
    {
        m_pOrgRec->sgsnAddress.value = Clone_GSNAddress(
            p->sgsnAddress.value,
            p->sgsnAddress.count);
        
        m_pOrgRec->sgsnAddress.count = p->sgsnAddress.count;

        /* 
         * 统计链表的长度，用于控制合并的话单数。
         * 
         */
        m_nLen_SGSNAddress_List = p->sgsnAddress.count;
    }

    //复制listOfTrafficVolumes字段
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
         * 统计链表的长度，用于控制合并的话单数。
         * 
         */
        m_nLen_Condition_List = p->listOfTrafficVolumes.count;
    }

    //复制diagnostics字段
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

    //复制recordExtensions字段
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
             * 统计链表的长度，用于控制合并的话单数。
             * 
             */
            m_nLen_Extension_List = p->recordExtensions.count;
        }
    }

    //设置当前话单序列号
    m_nMinRecordSequence = m_nCurRecordSequence = p->recordSequenceNumber;

    //设置PDP结束标志
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

    //增加按通话时长结束合并 ZhengYuqun 2003-12-18
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

    //增加按通话时长结束合并 ZhengYuqun 2003-12-18
    if((UINT4)m_pOrgRec->duration >= m_uMaxDuration)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL CR99_GCDR::IsPartial()
{
    /* 
     * 若recordSequenceNumber字段有效，则话单是部分话单。
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
* 函数名:   CR99_GCDR::CanSafeMerge
* 作者:    GGF
* 日期:    2003-09-28
* 描述:    
* 输入: 
*       参数1 :CBill* pBill,待合并的话单对象指针。
* 返回值: BOOL ,若两张话单允许合并，返回TRUE，否则返回FALSE。
* 说明: 
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*
******************************************************/
BOOL CR99_GCDR::CanSafeMerge(CBill* pBill)
{
    assert(pBill != NULL);

    CR99_GCDR* p = (CR99_GCDR*)pBill;

    //若话单的序列号在已经合并的话单范围内，说明是重复话单。
    //重复话单在Merge方法中丢弃。
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
     * 超大话单，强行送走。
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

    //若序列号不连续，不允许合并。
    return (p->m_nCurRecordSequence == (m_nCurRecordSequence + 1));
}

BOOL CR99_GCDR::IsPDPCompleted()
{
    return m_bPDPCompleted;
}

BOOL CR99_GCDR::NeedWaiting()
{
    //当前的合并规则采用减法方式合并Duration字段
    //所以该函数只返回FALSE(当PDP结束时不需继续等待)。
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

    //待拷贝内存块开始/结束地址。
    char* pBegin = NULL,*pEnd = NULL;

    /*
     * 若话单没有经过合并，则从原始话单中构造最终话单。
     */

    //合并前bit_mask是4个字节长，合并后是2个字节长，不能直接赋值。
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
	//SWPD04515 CG对R99 3a0的话单合并不符合CMCC规范 ggf 2004.4.7
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
	//SWPD04515 CG对R99 3a0的话单合并不符合CMCC规范 ggf 2004.4.7
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

        //复制sgsnAddress字段
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

        //复制listOfTrafficVolumes字段
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
        
        //复制diagnostics字段
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
         * recordSequenceNumber转换为链表
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

        //复制nodeID字段(一个数组)
        if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_nodeID_present)
        {
            memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(CMCC_3GCDR_NodeID));
        }

        //复制recordExtensions字段
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
         * localSequenceNumber转换为链表
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
        
		//修改结束
        //by ldf 2004-07-05 对应问题单SWPD06029
        //consolidationResult字段
        switch(m_pOrgRec->causeForRecClosing)
        {
        case CMCC_3GCDR_normalRelease:
            pDest->consolidationResult = CMCC_3GCDR_CG_normal;
            break;
        case CMCC_3GCDR_sGSNChange:
            //PDP正常结束
            pDest->consolidationResult = CMCC_3GCDR_CG_forInterSGSNConsolidation;
            break;
        case CMCC_3GCDR_abnormalRelease:
            //PDP异常结束
            pDest->consolidationResult = CMCC_3GCDR_CG_notnormal;
            break;
        case CMCC_3GCDR_timeLimit:
            pDest->consolidationResult = CMCC_3GCDR_CG_reachLimit;
        default:
            //PDP未结束(置为PDP异常结束)
			pDest->consolidationResult = CMCC_3GCDR_CG_notnormal;
            break;
        }
		//end
		
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

    //合并listOfTrafficVolumes字段
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

    //合并diagnostics字段
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_diagnostics_present)
    {
        memcpy(&(pDest->diagnostics),&(m_Diagnostics),
            sizeof(CMCC_3GCDR_Diagnostics));
        /*
         * m_Diagnostics维护的指针转给了pDest。
         */
    }

    //合并recordSequenceNumber字段
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
         * m_pSequence肯定不为空。
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

    //复制nodeID字段(一个数组)
    if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_nodeID_present)
    {
        memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(CMCC_3GCDR_NodeID));
    }

    //合并recordExtensions字段
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

	//合并localSequenceNumber字段
    if(m_pOrgRec->bit_mask
        & CMCC_3GCDR_GGSNPDPRecord_localSequenceNumber_present)
    {
		//SWPD04515 CG对R99 3a0的话单合并不符合CMCC规范 ggf 2004.4.7
        memcpy(&(pDest->localSequenceNumberList.gsnAddress),
                &(m_pOrgRec->ggsnAddress),sizeof(CMCC_3GCDR_GSNAddress));

        /*
         * m_pLocalSequenceNumber肯定不为空。
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

    //consolidationResult字段
    switch(m_CauseForRecClosing)
    {
    case CMCC_3GCDR_normalRelease:
        //PDP正常结束
        pDest->consolidationResult
            = CMCC_3GCDR_CG_normal;
        break;
    case CMCC_3GCDR_sGSNChange:
        pDest->consolidationResult
            = CMCC_3GCDR_CG_forInterSGSNConsolidation;
        break;
    case CMCC_3GCDR_abnormalRelease:
        //PDP异常结束
        pDest->consolidationResult
            = CMCC_3GCDR_CG_notnormal;
        break;
    default:
        //PDP未结束(置为PDP异常结束)
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
* 函数名:   CR99_GCDR::Merge
* 作者:    GGF
* 日期:    2003-09-28
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

    //丢弃重复话单。
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

    //等同于:
    //m_nCurRecordSequence = p->m_pOrgRec->recordSequenceNumber;
    m_nCurRecordSequence++;

    //设置PDP结束标志。
    switch(p->m_pOrgRec->causeForRecClosing)
    {
    case CMCC_3GCDR_normalRelease:
    case CMCC_3GCDR_abnormalRelease:
        m_bPDPCompleted = TRUE;
        break;
    default:
        break;
    }

    //复制causeForRecClosing字段。
    m_CauseForRecClosing = p->m_pOrgRec->causeForRecClosing;

	if(!m_bDSTFlg)
	{
		int nDiff = Get_Duration(&(m_pOrgRec->recordOpeningTime),
			&(p->m_pOrgRec->recordOpeningTime));
		m_Duration = nDiff;
	}
	//累加Duration字段。
	m_Duration += p->m_pOrgRec->duration;
	//修改结束

    //链接sgsnAddress字段。    
    if((p->m_pOrgRec->sgsnAddress.value != NULL)
        && (p->m_pOrgRec->sgsnAddress.count > 0))
    {
        unsigned int nCount = p->m_pOrgRec->sgsnAddress.count;

        if(m_pSGSNAddressTail == NULL)
        {
            //这种情况不用判断重复SGSN地址

            LINK_STRUCT2<CMCC_3GCDR_GSNAddress>* q = LINK_STRUCT2<CMCC_3GCDR_GSNAddress>::Alloc();

            q->Value = Clone_GSNAddress(
                p->m_pOrgRec->sgsnAddress.value,nCount/*in*/);

            q->nCount = nCount;
            m_pSGSNAddressTail = m_pSGSNAddress = q;
        }
        else
        {
            //这时判断是否有重复SGSN地址

            LINK_STRUCT2<CMCC_3GCDR_GSNAddress>* q = LINK_STRUCT2<CMCC_3GCDR_GSNAddress>::Alloc();

            q->Value = Clone_GSNAddress(
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
         * 
         */
        m_nLen_SGSNAddress_List += nCount;
    }

    //链接listOfTrafficVolumes字段。
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
         * 统计链表的长度，用于控制合并的话单数。
         * 
         */
        m_nLen_Condition_List += q->nCount;
    }

    //链接recordExtensions字段。
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
             * 统计链表的长度，用于控制合并的话单数。
             * 
             */
            m_nLen_Extension_List += q->nCount;
        }
    }

    //链接recordSequenceNumber字段。
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

	//链接localSequenceNumber字段。
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

    //diagnostics字段取最后一张话单的值。
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

        /*回收原diagnostics字段的空间。*/
        if(m_pOrgRec->bit_mask & CMCC_3GCDR_GGSNPDPRecord_diagnostics_present) //增加一个if YanYong 2004-04-26 SWPD04795
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
            m_pOrgRec->bit_mask |= CMCC_3GCDR_GGSNPDPRecord_diagnostics_present; //增加一行 YanYong 2004-04-26 SWPD04795           
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

    //增加按通话时长结束合并 ZhengYuqun 2003-12-18
    if((UINT4)m_Duration >= m_uMaxDuration)
    {
        m_bPDPCompleted = TRUE;
    }
    
    /*
     * 其他字段取第一张话单的值。
     */
    return TRUE;
}
