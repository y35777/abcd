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
*若解析成功，返回TRUE，失败则抛出一个异常。
*若话单类型不能识别，则返回FALSE。
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
                //用话单记录初始化话单对象，失败将抛出异常。
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
    //3G R4需要合并的话单有两种：S-CDR,G-CDR。
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
* 函数名:   C3G_R4_BillParser::RegisterBillType
* 作者:    Guofeng, Guan
* 日期:    2003-06-02
* 描述:    向CMerger对象注册话单类型。
* 输入: 
*       参数1 :CMerger* pMerger
* 返回值: void 
* 说明:     若内存操作失败将抛出异常。
* 更新记录: 
*
* 日期            姓名                  描述 
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

//获得指定的话单类型ID的话单类型描述,szDesc为输出参数
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


//C3G_R4_SCDR类实现(S-CDR)
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
    //回收原始话单记录空间。
    if(m_pOrgRec != NULL)
    {
		//2003-07-24 listOfTrafficVolumes为可选 ggf
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
            //检查字段的数据是否有效。
            if(m_pOrgRec->recordExtensions != NULL)                
            {
                Clear_ManagementExtensions(m_pOrgRec->recordExtensions);
            }
        }

        delete m_pOrgRec;
        m_pOrgRec = NULL;
    }

    //回收最终话单记录空间。
    if(m_pDestRec != NULL)
    {
		//2003-07-24 最终话单中sgsnAddress为链表
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
     * 合并过程中使用的数据结构的空间已经在GetContents中回收。
     */
}


/******************************************************
* 函数名:   C3G_R4_SCDR::Init
* 作者:    Guofeng, Guan
* 日期:    2003-06-02
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
*2003-06-02       Zhou Yonghui
******************************************************/
BOOL C3G_R4_SCDR::Init(char* pBuf,UINT4 uLen)
{
    assert(pBuf != NULL);
    
    R4_Org_SGSNPDPRecord* p = (R4_Org_SGSNPDPRecord*)pBuf;

    //备份原始话单
    m_pOrgRec = new R4_Org_SGSNPDPRecord;
    if(m_pOrgRec == NULL)
    {
        return FALSE;
    }

    //复制话单记录
    memcpy(m_pOrgRec,p,sizeof(R4_Org_SGSNPDPRecord));

    /*
     * 下面单独复制指针字段
     */

    //复制listOfTrafficVolumes字段
	if(p->bit_mask & R4_Org_SGSNPDPRecord_listOfTrafficVolumes_present)
	{
		if(p->listOfTrafficVolumes != NULL)
		{
		   /* 
            * 统计链表的长度，用于控制合并的话单数。
            * 
            */
			R4_Org__seqof1_* pTail = NULL;
			unsigned int nCount = 0;
			m_pOrgRec->listOfTrafficVolumes = Clone_ChangeOfCharCondition(
				p->listOfTrafficVolumes, pTail, nCount);
			m_nLen_Condition_List = nCount;
		}
    }

    //复制diagnostics字段
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

    //复制recordExtensions字段
    if(p->bit_mask & R4_Org_SGSNPDPRecord_recordExtensions_present)
    {
        if(p->recordExtensions != NULL)
        {
			/* 
             * 统计链表的长度，用于控制合并的话单数。
             * 
             */
			R4_Org_ManagementExtensions_* pTail = NULL;
			unsigned int nCount = 0;
			m_pOrgRec->recordExtensions = 
				Clone_ManagementExtensions(p->recordExtensions, pTail, nCount);       
			m_nLen_Extension_List = nCount;
				
        }
    }

    //设置当前话单序列号
    m_nMinRecordSequence = m_nCurRecordSequence = p->recordSequenceNumber;

	//通话时长
	m_Duration = p->duration;

    //设置PDP结束标志
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

    //增加按通话时长结束合并 ZhengYuqun 2003-12-18
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

    //增加按通话时长结束合并 ZhengYuqun 2003-12-18
    if((UINT4)m_pOrgRec->duration >= m_uMaxDuration)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL C3G_R4_SCDR::IsPartial()
{
    /* 
     * 若recordSequenceNumber字段有效，则话单是部分话单。
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
* 函数名:   C3G_R4_SCDR::CanSafeMerge
* 作者:    Guofeng, Guan
* 日期:    2003-06-02
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
           //SGSN如果已经发生切换, 则要求待合并话单的话终原因为sGSNChange，
           //合并话单的sgsnChange字段置位，才允许合并
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
	 * 合并CAMEL域中freeFormatData,
	 * 合并的话单的两者length和大于160
	 * 强行送走
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
    //当前的合并规则采用减法方式合并Duration字段
    //所以该函数只返回FALSE(当PDP结束时不需继续等待)。
    return FALSE;
}


/******************************************************
* 函数名:   C3G_R4_SCDR::GetContents
* 作者:    Guofeng, Guan
* 日期:    2003-06-02
* 描述:    函数按照最终话单格式组合话单记录。
* 输入: 
* 返回值: char*,符合最终话单格式的话单记录。
* 说明: 
* 更新记录: 
*
* 日期            姓名                  描述 
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

    //初始化为0。
    memset(pDest,0,sizeof(R4_Fin_SGSNPDPRecord));
    m_pDestRec = pCallEventRec;

    //待拷贝内存块开始/结束地址。
    char* pBegin = NULL,*pEnd = NULL;

    /*
     * 若话单没有经过合并，则从原始话单中构造最终话单。
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
		//2003-07-24 最终话单中sgsnAddress为链表，应拷贝到它前面一个域servedIMEI为止
		pEnd = (char*)(&(m_pOrgRec->sgsnAddress));

        /*
         * recordType,networkInitiation,servedIMSI,
         * servedIMEI
         */
        memcpy(&(pDest->recordType),pBegin,(pEnd - pBegin));
		
		/*
         * sgsnAddress转换为链表
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

        //复制listOfTrafficVolumes字段
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

        //复制diagnostics字段
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
         * recordSequenceNumber转换为链表
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

            //问题单 SWPD05036 YanYong 2004-05-10
            pDest->recSequenceNumList->value.bit_mask |= R4_Fin_recSequenceNumberList_present;
            //</SWPD05036>

			if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_sgsnAddress_present)
			{
				memcpy(&(pDest->recSequenceNumList->value.sgsnAddress),
                   &(m_pOrgRec->sgsnAddress),sizeof(R4_Org_GSNAddress));            
			}
			else
			{				
				//若SGSN地址无效，把链表中的SGSN地址设置为空
                memset(&(pDest->recSequenceNumList->value.sgsnAddress),
                    0,sizeof(R4_Org_GSNAddress));
			}
        }

        //复制nodeID字段
        if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_nodeID_present)
        {
            memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(R4_Org_NodeID));
        }

        //复制recordExtensions字段
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
         * localSequenceNumber转换为链表
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
				//若SGSN地址无效，把链表中的SGSN地址设置为空
                memset(&(pDest->localSequenceNumberList->value.gsnAddress),
                    0,sizeof(R4_Org_GSNAddress));
				//2003-07-29 ggf 需要将choice正确置位，否则解码失败
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
         * rNCDownlinkVolume转换为链表
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
				//若SGSN地址无效，把链表中的SGSN地址设置为空
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
        
        //consolidationResult字段
        switch(m_pOrgRec->causeForRecClosing)
        {
        case R4_Org_normalRelease:
            pDest->consolidationResult = R4_Fin_normal;
            break;
        case R4_Org_sGSNChange:
            //PDP正常结束
            pDest->consolidationResult = R4_Fin_forInterSGSNConsolidation;
            break;
        case R4_Org_abnormalRelease:
            //PDP异常结束
            pDest->consolidationResult = R4_Fin_notNormal;
            break;
        case R4_Org_timeLimit:
            pDest->consolidationResult = R4_Fin_reachLimit;
        default:
            //PDP未结束(置为PDP异常结束)
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
     * 下面是合并后话单的组合过程。
     */

    pBegin = (char*)(&(m_pOrgRec->recordType));
    pEnd = (char*)(&(m_pOrgRec->sgsnAddress));

    /*
     * recordType,networkInitiation,servedIMSI,
     * servedIMEI
     */
    memcpy(&(pDest->recordType),pBegin,(pEnd - pBegin));

	//合并sgsnAddress字段
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

    //合并listOfTrafficVolumes字段
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

    //合并diagnostics字段
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_diagnostics_present)
    {
        memcpy(&(pDest->diagnostics),&(m_Diagnostics),
            sizeof(R4_Org_Diagnostics));
    }

    //合并recordSequenceNumber字段
    if(m_pOrgRec->bit_mask
        & R4_Org_SGSNPDPRecord_recordSequenceNumber_present)		
    {		        
        /*
         * m_pSequence一定不为NULL。
         */
		pDest->recSequenceNumList = m_pRecordSequenceList;
        m_pRecordSequenceList = m_pRecordSequenceListTail = NULL;
        m_pRecordSequenceTail = NULL;
    }

    //nodeID字段(一个数组)
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_nodeID_present)
    {
        memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(R4_Org_NodeID));
    }

    //合并recordExtensions字段
    if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_recordExtensions_present)
    {
        if(m_pExtension != NULL)
        {
            pDest->recordExtensions = (R4_Fin_ManagementExtensions_*) m_pExtension;
            
            m_pExtension = m_pExtensionTail = NULL;
        }
    }

	//合并localSequenceNumber字段
    if(m_pOrgRec->bit_mask
        & R4_Org_SGSNPDPRecord_localSequenceNumber_present)		
    {		        
        /*
         * m_pSequence一定不为NULL。
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
    //注：zouyongsheng 增加CAMEL域后在此不作处理。最后修改此域
	memcpy(&(pDest->apnSelectionMode),pBegin,
        (pEnd - pBegin) + sizeof(R4_Org_CAMELInformationPDP));

	//合并rNCDownlinkVolume字段
    if(m_pRNCDownlinkVolumeList != NULL)		
    {		        
        /*
         * m_pRNCDownlinkSequence一定不为NULL。
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
    //注：zouyongsheng 增加CAMEL域后在此不作处理。最后修改此域
	memcpy(&(pDest->chChSelectionMode),pBegin,
        (pEnd - pBegin) + sizeof(R4_Org_DynamicAddressFlag));

	//consolidationResult字段
    switch(m_CauseForRecClosing)
    {
    case R4_Org_normalRelease:
        pDest->consolidationResult = R4_Fin_normal;
        break;
    case R4_Org_sGSNChange:
        //PDP正常结束
        pDest->consolidationResult = R4_Fin_forInterSGSNConsolidation;
        break;
    case R4_Org_abnormalRelease:
        //PDP异常结束
        pDest->consolidationResult = R4_Fin_notNormal;
        break;
    case R4_Org_timeLimit:
        pDest->consolidationResult = R4_Fin_reachLimit;
        break;
    default:
        //PDP未结束(置为PDP异常结束)
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

	//2003-07-24 S-CDR中sgsnAddress为可选 ggf
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
* 函数名:   C3G_R4_SCDR::PrepareData
* 作者:    Guofeng, Guan
* 日期:    2003-06-02
* 描述:    为合并准备数据。
* 输入:    无。
* 返回值: BOOL，成功返回TRUE，失败返回FALSE。
* 说明:    合并过程中需要把一些字段链接起来，形成一个链表，
* 
* 
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*2003-06-02       Guofeng, Guan
******************************************************/
BOOL C3G_R4_SCDR::PrepareData()
{
	//2003-07-24 链接sgsnAddress ggf
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

    //链接listOfTrafficVolumes
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

    //链接recordExtensions
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

    //链接recordSequenceNumber
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

	//链接localSequenceNumber
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

    //复制duration字段
    m_Duration = m_pOrgRec->duration;

	//复制cAMELInformationPDP
	if(m_pOrgRec->bit_mask
		& R4_Org_cAMELInformationPDP_present)
	{
		m_CAMELInformationPDP = m_pOrgRec->cAMELInformationPDP;
	}

	//链接rNCUnsentDownlinkVolume字段
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

    //复制causeForRecClosing字段
    m_CauseForRecClosing = m_pOrgRec->causeForRecClosing;

    //复制diagnostics字段
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

    //准备完成
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
* 函数名:   C3G_R4_SCDR::Merge
* 作者:    Guofeng, Guan
* 日期:    2003-06-02
* 描述:    把话单pBill合入当前话单中。
* 输入: 
*       参数1 :CBill* pBill，待合并的话单。
* 返回值: BOOL，合并成功返回TRUE，失败返回FALSE。
* 说明: 
*       合并过程中更新的字段有:
*       需要链接的字段(sgsnAddress,listOfTrafficVolumes,recordSequenceNumber,
*       recordExtensions),取最后一张话单值的字段(causeForRecClosing,
*       diagnostics),需要累加的字段(Duration,rNCUnsentDownlinkVolume)。
*       取第一张话单值的字段不变。
* 更新记录: 
*
* 日期            姓名                  描述 
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
    case R4_Org_normalRelease:
    case R4_Org_abnormalRelease:
    //comment by chenliangwei, 2004-06-23, for diffrent sgsn merge
    //case R4_Org_sGSNChange:
        m_bPDPCompleted = TRUE;
        break;
    default:
        break;
    }

    //复制causeForRecClosing字段。
    m_CauseForRecClosing = p->m_pOrgRec->causeForRecClosing;
    if(p->m_pOrgRec->bit_mask & R4_Org_rNCUnsentDownlinkVolume_present)
    {
        m_pOrgRec->bit_mask |= R4_Org_rNCUnsentDownlinkVolume_present;
    }


	//Z30971增加修改是否支持夏时制
	if(!m_bDSTFlg)
	{
		//duration域按照减法方式计算
        int nDiff = Get_Duration(&(m_pOrgRec->recordOpeningTime),
                                 &(p->m_pOrgRec->recordOpeningTime));
        m_Duration = nDiff;
	}
    m_Duration += p->m_pOrgRec->duration;
	//修改结束

    //Added by ZhengYuqun 2003-11-04 SWPD02238
    if((UINT4)m_Duration >= m_uMaxDuration)
    {
        m_bPDPCompleted = TRUE;
    }

	//2003-07-24 链接sgsnAddress字段 ggf
    BOOL bSgsnChange = FALSE;
	if((p->m_pOrgRec->bit_mask 
        & R4_Org_SGSNPDPRecord_sgsnAddress_present)
        && (m_pOrgRec->bit_mask 
        & R4_Org_SGSNPDPRecord_sgsnAddress_present))
    {		
        //这时判断是否SGSN发生切换			
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
             * 统计链表的长度，用于控制合并的话单数。
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

    //链接listOfTrafficVolumes字段。
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
			 * 统计链表的长度，用于控制合并的话单数。
			 * 
			 */
			m_nLen_Condition_List += nCount;
		}
	}

    /*
    //链接recordExtensions字段。
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
             * 统计链表的长度，用于控制合并的话单数。
             * 
             *
            m_nLen_Extension_List += nCount;
        }
    }
    */

    //链接recordSequenceNumber字段。
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

    //diagnostics字段取最后一张话单的值。
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

        /*回收m_Diagnostics的空间。*/
        if(m_pOrgRec->bit_mask & R4_Org_SGSNPDPRecord_diagnostics_present) //增加一个if ZhengYuqun 2003-11-03 SWPD02228

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
            m_pOrgRec->bit_mask |= R4_Org_SGSNPDPRecord_diagnostics_present; //增加一行 ZhengYuqun 2003-11-03 SWPD02228

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

	//链接localSequenceNumber字段。
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

	//合并CAMELInformation域的信息
    if((p->m_pOrgRec->bit_mask 
		& R4_Org_cAMELInformationPDP_present)
		&& (m_pOrgRec->bit_mask
		& R4_Org_cAMELInformationPDP_present))
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
		
		//fFDAppendIndicator 字段存在且值为TRUE
		//freeFormatData存在而且信息长度不为零
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

	//链接rNCDownlinkVolume字段。
    if((p->m_pOrgRec->bit_mask 
        & R4_Org_rNCUnsentDownlinkVolume_present))
//        && (m_pOrgRec->bit_mask           //删除该行 问题单SWPD05164 YanYong 2004-05-12
//        & R4_Org_rNCUnsentDownlinkVolume_present)) //删除该行 问题单SWPD05164 YanYong 2004-05-12
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


//C3G_R4_GCDR类实现(S-CDR)
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
    //回收原始话单记录空间。
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

    //回收最终话单记录空间。
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
     * 合并过程中使用的数据结构的空间已经在GetContents中回收。
     */
}

BOOL C3G_R4_GCDR::PrepareData()
{
    //转换sgsnAddress
    if(m_pOrgRec->sgsnAddress != NULL)
    {
		R4_Fin__seqof2_* pTail = NULL;
		unsigned int nCount = 0;
        m_pSGSNAddress = Clone_GsnAddressList((R4_Fin__seqof2_*)(m_pOrgRec->sgsnAddress), (R4_Fin__seqof2_*)pTail, nCount);

        m_pSGSNAddressTail = pTail;
    }

    //链接listOfTrafficVolumes
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

    //链接recordExtensions
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

    //链接recordSequenceNumber
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

	//链接localSequenceNumber
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

    //复制duration字段
    m_Duration = m_pOrgRec->duration;

    //复制causeForRecClosing字段
    m_CauseForRecClosing = m_pOrgRec->causeForRecClosing;

    //复制diagnostics字段
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

    //准备完成
    m_bPrepared = TRUE;
    return TRUE;
}


/******************************************************
* 函数名:   C3G_R4_GCDR::Init
* 作者:    Guofeng, Guan
* 日期:    2003-06-02
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
*2003-06-02       Guofeng, Guan
******************************************************/
BOOL C3G_R4_GCDR::Init(char* pBuf,UINT4 uLen)
{
    assert(pBuf != NULL);

    //话单记录的所有字段都被存放在连续的内存块中
    R4_Org_GGSNPDPRecord* p = (R4_Org_GGSNPDPRecord*)pBuf;

    //备份原始话单
    m_pOrgRec = new R4_Org_GGSNPDPRecord;
    if(m_pOrgRec == NULL)
    {
        throw "Alloc memory fail.";
    }

    memcpy(m_pOrgRec,p,sizeof(R4_Org_GGSNPDPRecord));

    /*
     * 下面单独复制指针字段
     */

    //复制sgsnAddress
    if(p->sgsnAddress != NULL)
    {
		R4_Fin__seqof2_* pTail = NULL;
		unsigned int nCount = 0;
        m_pOrgRec->sgsnAddress = (R4_Org_GGSNPDPRecord::R4_Org__seqof2 *)Clone_GsnAddressList((R4_Fin__seqof2_*)(p->sgsnAddress), (R4_Fin__seqof2_*)pTail, nCount);
        

        /* 
         * 统计链表的长度，用于控制合并的话单数。
         * 
         */
        m_nLen_SGSNAddress_List = nCount;
    }

    //复制listOfTrafficVolumes字段
	if(p->bit_mask & R4_Org_GGSNPDPRecord_listOfTrafficVolumes_present)
	{
		if(p->listOfTrafficVolumes != NULL)
		{
			R4_Org__seqof1_* pTail = NULL;
			unsigned int nCount = 0;
			m_pOrgRec->listOfTrafficVolumes
				= Clone_ChangeOfCharCondition(p->listOfTrafficVolumes, pTail, nCount);            
			
			/* 
			 * 统计链表的长度，用于控制合并的话单数。
			 * 
			 */
			m_nLen_Condition_List = nCount;
		}
    }

    //复制diagnostics字段
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

    //复制recordExtensions字段
    if(p->bit_mask & R4_Org_GGSNPDPRecord_recordExtensions_present)
    {
        if(p->recordExtensions != NULL)
        {
			R4_Org_ManagementExtensions_* pTail = NULL;
			unsigned int nCount = 0;
            m_pOrgRec->recordExtensions
                = Clone_ManagementExtensions(p->recordExtensions, pTail, nCount);            

            /* 
             * 统计链表的长度，用于控制合并的话单数。
             * 
             */
            m_nLen_Extension_List = nCount;
        }
    }

    //设置当前话单序列号
    m_nMinRecordSequence = m_nCurRecordSequence = p->recordSequenceNumber;

    //设置PDP结束标志
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

    //增加按通话时长结束合并 ZhengYuqun 2003-12-18
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

    //增加按通话时长结束合并 ZhengYuqun 2003-12-18
    if((UINT4)m_pOrgRec->duration >= m_uMaxDuration)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL C3G_R4_GCDR::IsPartial()
{
    /* 
     * 若recordSequenceNumber字段有效，则话单是部分话单。
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
* 函数名:   C3G_R4_GCDR::CanSafeMerge
* 作者:     Guofeng, Guan
* 日期:     2003-06-02
* 描述:    
* 输入: 
*       参数1 :CBill* pBill,待合并的话单对象指针。
* 返回值: BOOL ,若两张话单允许合并，返回TRUE，否则返回FALSE。
* 说明: 
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*2003-06-02       Guofeng, Guan
******************************************************/
BOOL C3G_R4_GCDR::CanSafeMerge(CBill* pBill)
{
    assert(pBill != NULL);

    C3G_R4_GCDR* p = (C3G_R4_GCDR*)pBill;

    //若话单的序列号在已经合并的话单范围内，说明是重复话单。
    //重复话单在Merge方法中丢弃。
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
     * 超大话单，强行送走。
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

    //若序列号不连续，不允许合并。
    return (p->m_nCurRecordSequence == (m_nCurRecordSequence + 1));
}

BOOL C3G_R4_GCDR::IsPDPCompleted()
{
    return m_bPDPCompleted;
}

BOOL C3G_R4_GCDR::NeedWaiting()
{
    //当前的合并规则采用减法方式合并Duration字段
    //所以该函数只返回FALSE(当PDP结束时不需继续等待)。
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

    //待拷贝内存块开始/结束地址。
    char* pBegin = NULL,*pEnd = NULL;

    /*
     * 若话单没有经过合并，则从原始话单中构造最终话单。
     */

    //合并前bit_mask是4个字节长，合并后是2个字节长，不能直接赋值。
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

        //复制sgsnAddress字段
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

        //复制listOfTrafficVolumes字段
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
        
        //复制diagnostics字段
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
         * recordSequenceNumber转换为链表
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

        //复制nodeID字段(一个数组)
        if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_nodeID_present)
        {
            memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(R4_Org_NodeID));
        }

        //复制recordExtensions字段
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
         * localSequenceNumber转换为链表
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

        //consolidationResult字段
        switch(m_pOrgRec->causeForRecClosing)
        {
        case R4_Org_normalRelease:
            //PDP正常结束
            pDest->consolidationResult
                = R4_Fin_normal;
            break;
        case R4_Org_sGSNChange:
            pDest->consolidationResult
                = R4_Fin_forInterSGSNConsolidation; 
            break;
        case R4_Org_abnormalRelease:
            //PDP异常结束
            pDest->consolidationResult
                = R4_Fin_notNormal;
            break;
        default:
            //PDP未结束(置为PDP异常结束)
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
     * 下面是合并后话单的组合过程。
     */

    pBegin = (char*)(&(m_pOrgRec->recordType));
    pEnd = (char*)(&(m_pOrgRec->sgsnAddress));

    /* 
     * recordType,networkInitiation,servedIMSI,
     * ggsnAddress,chargingID
     */
    memcpy(&(pDest->recordType),pBegin,(pEnd - pBegin));
    
    //合并sgsnAddress字段
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

    //合并listOfTrafficVolumes字段
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

    //合并diagnostics字段
    if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_diagnostics_present)
    {
        memcpy(&(pDest->diagnostics),&(m_Diagnostics),
            sizeof(R4_Org_Diagnostics));
        /*
         * m_Diagnostics维护的指针转给了pDest。
         */
    }

    //合并recordSequenceNumber字段
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
         * m_pSequence肯定不为空。
         */
        pDest->recSequenceNumList.recSequenceNumberList = m_pRecordSequence;

        pDest->recSequenceNumList.bit_mask
            |= R4_Fin_recSequenceNumberList_present;
        
        m_pRecordSequence = m_pRecordSequenceTail = NULL;
    }

    //复制nodeID字段(一个数组)
    if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_nodeID_present)
    {
        memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(R4_Org_NodeID));
    }

    //合并recordExtensions字段
    if(m_pOrgRec->bit_mask
        & R4_Org_GGSNPDPRecord_recordExtensions_present)
    {
        if(m_pExtension != NULL)
        {
            pDest->recordExtensions = (R4_Fin_ManagementExtensions_*) m_pExtension;
                        
            m_pExtension = m_pExtensionTail = NULL;
        }
    }

	//合并localSequenceNumber字段
	if(m_pOrgRec->bit_mask
        & R4_Org_GGSNPDPRecord_localSequenceNumber_present)
    {
        memcpy(&(pDest->localSequenceNumberList.gsnAddress),
            &(m_pOrgRec->ggsnAddress), sizeof(R4_Org_GSNAddress));        

        /*
         * m_pSequence肯定不为空。
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

    //consolidationResult字段
    switch(m_CauseForRecClosing)
    {
    case R4_Org_normalRelease:
    case R4_Org_sGSNChange:
        //PDP正常结束
        pDest->consolidationResult
            = R4_Fin_normal;
        break;
    case R4_Org_abnormalRelease:
        //PDP异常结束
        pDest->consolidationResult
            = R4_Fin_notNormal;
        break;
    default:
        //PDP未结束(置为PDP异常结束)
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
* 函数名:   C3G_R4_GCDR::Merge
* 作者:    Guofeng, Guan
* 日期:    2003-06-02
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

    //丢弃重复话单。
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

    //等同于:
    //m_nCurRecordSequence = p->m_pOrgRec->recordSequenceNumber;
    m_nCurRecordSequence++;

    //设置PDP结束标志。
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

    //复制causeForRecClosing字段。
    m_CauseForRecClosing = p->m_pOrgRec->causeForRecClosing;

	//Z30971增加修改是否支持夏时制
	if(!m_bDSTFlg)
	{
		//duration域按照减法方式计算
        int nDiff = Get_Duration(&(m_pOrgRec->recordOpeningTime),
                                 &(p->m_pOrgRec->recordOpeningTime));
        m_Duration = nDiff;
	}
    m_Duration += p->m_pOrgRec->duration;
	//修改结束

    //Added by ZhengYuqun 2003-11-04 SWPD02238
    if((UINT4)m_Duration >= m_uMaxDuration)
    {
        m_bPDPCompleted = TRUE;
    }

    //链接sgsnAddress字段。
    
    //剔除重复的SGSN地址.

    if(p->m_pOrgRec->sgsnAddress != NULL)        
    {
        unsigned int nCount = 0;

        if(m_pSGSNAddressTail == NULL)
        {
            //这种情况不用判断重复SGSN地址
			R4_Fin__seqof2_* pTail = NULL;
			R4_Fin__seqof2_* q = NULL;
            q = Clone_GsnAddressList((R4_Fin__seqof2_*)p->m_pOrgRec->sgsnAddress, (R4_Fin__seqof2_*)pTail, nCount);

            m_pSGSNAddress = q;
			m_pSGSNAddressTail = pTail;
        }
        else
        {
            //这时判断是否有重复SGSN地址
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
         * 统计链表的长度，用于控制合并的话单数。
         * 
         */
        m_nLen_SGSNAddress_List += nCount;
    }

    //链接listOfTrafficVolumes字段。
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
			 * 统计链表的长度，用于控制合并的话单数。
			 * 
			 */
			m_nLen_Condition_List += nCount;              
		}
    }

    //链接recordExtensions字段。
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
             * 统计链表的长度，用于控制合并的话单数。
             * 
             */
            m_nLen_Extension_List += nCount;
        }
    }

    //链接recordSequenceNumber字段。
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

	//链接localSequenceNumber字段。
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

    //diagnostics字段取最后一张话单的值。
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

        /*回收原diagnostics字段的空间。*/
        if(m_pOrgRec->bit_mask & R4_Org_GGSNPDPRecord_diagnostics_present) //增加一个if ZhengYuqun 2003-11-03 SWPD02228
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
            m_pOrgRec->bit_mask |= R4_Org_GGSNPDPRecord_diagnostics_present; //增加一行 ZhengYuqun 2003-11-03 SWPD02228
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


//C3G_R4_MCDR类实现(M-CDR)
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
    //回收原始话单记录空间。
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

    //回收最终话单记录空间。
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
     * 合并过程中使用的数据结构的空间已经在GetContents中回收。
     */
}

BOOL C3G_R4_MCDR::PrepareData()
{
    //复制duration字段
    m_Duration = m_pOrgRec->duration;

    //复制causeForRecClosing字段
    m_CauseForRecClosing = m_pOrgRec->causeForRecClosing;

    //转换changeLocation
    if(m_pOrgRec->bit_mask & R4_Org_changeLocation_present)
    {
        if(m_pOrgRec->changeLocation != NULL)            
        {
			unsigned int nCount = 0;
            m_pLocation = Clone_ChangeLocationList(m_pOrgRec->changeLocation, nCount);
                    
            m_pLocationTail = m_pLocation;
        }
    }

    //转换recordExtensions
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
    //转换recordSequenceNumber
    if(m_pOrgRec->bit_mask 
        & R4_Org_SGSNMMRecord_recordSequenceNumber_present)
    {
		
    }
*/
    //复制diagnostics字段
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

	//复制cAMELInformationMM
	if(m_pOrgRec->bit_mask
		& R4_Org_cAMELInformationMM_present)
	{
		m_CAMELInformationMM = m_pOrgRec->cAMELInformationMM;
	}

    //准备完成
    m_bPrepared = TRUE;
    return TRUE;
}


/******************************************************
* 函数名:   C3G_R4_MCDR::Init
* 作者:    Guofeng, Guan
* 日期:    2003-06-02
* 描述:    
* 输入: 
*       参数1 :char* pBuf，M-CDR话单记录。
*       参数2 :int nLen，话单记录的长度。
* 返回值: BOOL ，初始化成功，返回TRUE，内存操作失败抛出异常。
* 说明: 
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*2003-06-02       Guofeng, Guan
******************************************************/
BOOL C3G_R4_MCDR::Init(char* pBuf,UINT4 uLen)
{
    assert(pBuf != NULL);

    R4_Org_SGSNMMRecord* p = (R4_Org_SGSNMMRecord*)pBuf;

    //备份原始话单
    m_pOrgRec = new R4_Org_SGSNMMRecord;
    if(m_pOrgRec == NULL)
    {
        throw "Alloc memory fail.";
    }

    memcpy(m_pOrgRec,p,sizeof(R4_Org_SGSNMMRecord));

    /*
     * 下面单独复制指针字段
     */

    //复制changeLocation
    if(p->bit_mask & R4_Org_changeLocation_present)
    {
        if(p->changeLocation != NULL)           
        {
			unsigned int nCount = 0;
            m_pOrgRec->changeLocation = Clone_ChangeLocationList(p->changeLocation, nCount);                        

            /* 
             * 统计链表的长度，用于控制合并的话单数。
             * 
             */
            m_nLen_Location_List = nCount;
        }
    }

    //转换recordExtensions
    if(p->bit_mask & R4_Org_SGSNMMRecord_recordExtensions_present)
    {
        if(p->recordExtensions != NULL)            
        {
			R4_Org_ManagementExtensions_* pTail = NULL;
			unsigned int nCount = 0;
            m_pOrgRec->recordExtensions = 
				Clone_ManagementExtensions(p->recordExtensions, pTail, nCount);                       

            /*
             * 统计链表的长度，用于控制合并的话单数。
             * 
             */
            m_nLen_Extension_List = nCount;
        }
    }

    //复制diagnostics字段
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

    //设置当前话单序列号
    m_nMinRecordSequence = m_nCurRecordSequence = p->recordSequenceNumber;

    //设置PDP结束标志
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

    //增加按通话时长结束合并 ZhengYuqun 2003-12-18
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

    //增加按通话时长结束合并 ZhengYuqun 2003-12-18
    if((UINT4)m_pOrgRec->duration >= m_uMaxDuration)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL C3G_R4_MCDR::IsPartial()
{
    /* 
     * 若recordSequenceNumber字段有效，则话单是部分话单。
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
* 函数名:   C3G_R4_MCDR::CanSafeMerge
* 作者:    Guofeng, Guan
* 日期:    2003-06-02
* 描述:    
* 输入: 
*       参数1 :CBill* pBill,待合并的话单对象的指针。
* 返回值: BOOL ,若两张话单允许合并，返回TRUE，否则返回FALSE。
* 说明: 
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*2003-06-02       Guofeng, Guan
******************************************************/
BOOL C3G_R4_MCDR::CanSafeMerge(CBill* pBill)
{
    assert(pBill != NULL);

    C3G_R4_MCDR* p = (C3G_R4_MCDR*)pBill;

    //若话单的序列号在已经合并的话单范围内，说明是重复话单。
    //重复话单在Merge方法中丢弃。
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
     * 超大话单，强行送走。 
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
	 * 合并CAMEL域中freeFormatData,
	 * 合并的话单的两者length和大于160
	 * 强行送走
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
    //当前的合并规则采用减法方式合并Duration字段
    //所以该函数只返回FALSE(当PDP结束时不需继续等待)。
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

    //待拷贝内存块开始/结束地址。
    char* pBegin = NULL,*pEnd = NULL;

    /*
     * 若话单没有经过合并，则从原始话单中构造最终话单。
     */

    //合并前bit_mask是4个字节长，合并后是2个字节长，不能直接赋值。
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
    
        //复制changeLocation字段
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

        //复制diagnostics字段
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

        //复制nodeID字段
        if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_nodeID_present)
        {
            memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(R4_Org_NodeID));
        }

        //复制recordExtensions字段
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
        //注:增加CAMEL域后，无需修改zouyongsheng
		memcpy(&(pDest->localSequenceNumber),pBegin,
            (pEnd - pBegin) + sizeof(R4_Fin_ChChSelectionMode));

        return (char*)m_pDestRec;
    }

    /*
     * 下面是合并后话单的组合过程。
     */

    pBegin = (char*)(&(m_pOrgRec->recordType));
    pEnd = (char*)(&(m_pOrgRec->changeLocation));

    /* 
    *recordType,servedIMSI,servedIMEI,sgsnAddress,
    *msNetworkCapability,routingArea,locationAreaCode,
    *cellIdentifier
    */
    memcpy(&(pDest->recordType),pBegin,(pEnd - pBegin));
    
    //合并changeLocation字段
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

    //合并diagnostics字段
    if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_diagnostics_present)
    {
        memcpy(&(pDest->diagnostics),&(m_Diagnostics),
            sizeof(R4_Org_Diagnostics));
    }

    //合并recordSequenceNumber字段
    if(m_pOrgRec->bit_mask
        & R4_Org_SGSNMMRecord_recordSequenceNumber_present)
    {
        pDest->recordSequenceNumber = m_pOrgRec->recordSequenceNumber;  
    }

    //nodeID字段(一个数组)
    if(m_pOrgRec->bit_mask & R4_Org_SGSNMMRecord_nodeID_present)
    {
        memcpy(pDest->nodeID,m_pOrgRec->nodeID,sizeof(R4_Org_NodeID));
    }

    //合并recordExtensions字段
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
    //CAMEL域在此不作处理。最后修改此域
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
* 函数名:   C3G_R4_MCDR::Merge
* 作者:    Guofeng, Guan
* 日期:    2003-06-02
* 描述:    把话单pBill合入当前话单中。
* 输入: 
*       参数1 :CBill* pBill，待合并的话单。
* 返回值: BOOL，合并成功返回TRUE，失败返回FALSE。
* 说明: 
*       合并过程中更新的字段有:
*       需要链接的字段(changeLocation,recordSequenceNumber,
*       recordExtensions),取最后一张话单值的字段(causeForRecClosing,
*       diagnostics),需要累加的字段(Duration)。
*       取第一张话单值的字段不变。
* 更新记录: 
*
* 日期            姓名                  描述 
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

    //丢弃重复话单。
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

    //等同于:
    //m_nCurRecordSequence = p->m_pOrgRec->recordSequenceNumber;
    m_nCurRecordSequence++;

    //设置PDP结束标志。
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

    //复制causeForRecClosing字段。
    m_CauseForRecClosing = p->m_pOrgRec->causeForRecClosing;

    //链接changeLocation字段。
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
             * 统计链表的长度，用于控制合并的话单数。
             * 
             */
            m_nLen_Location_List += nCount;
        }
    }

    //链接recordExtensions字段。
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
             * 统计链表的长度，用于控制合并的话单数。
             * 
             */
            m_nLen_Extension_List += nCount;
        }
    }

    //链接recordSequenceNumber字段。
    if((p->m_pOrgRec->bit_mask 
        & R4_Org_SGSNMMRecord_recordSequenceNumber_present)
        && (m_pOrgRec->bit_mask 
        & R4_Org_SGSNMMRecord_recordSequenceNumber_present))
    {
        //
    }

    //累加Duration字段。
    if((p->m_pOrgRec->bit_mask & R4_Org_duration_present)
        && (m_pOrgRec->bit_mask & R4_Org_duration_present))
    {
        int nDiff = Get_Duration(&(m_pOrgRec->recordOpeningTime),
                                 &(p->m_pOrgRec->recordOpeningTime));
        m_Duration = nDiff;
        m_Duration += p->m_pOrgRec->duration;
        
    }

    //diagnostics字段取最后一张话单的值。
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
         * 回收原diagnostics字段的空间。
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

	//合并CAMELInformation域的信息
    if((p->m_pOrgRec->bit_mask 
		& R4_Org_cAMELInformationMM_present)
		&& (m_pOrgRec->bit_mask
		& R4_Org_cAMELInformationMM_present))
	{
		/*
		 * sCFAddress,serviceKey,defaultTransactionHandling,
		 * 以上域取第一张部分话单的内容
		 */

		/*
		 * numberOfDPEncountered,levelOfCAMELService
		 * 以上域取最后一张部分话单的内容
		 */
		m_CAMELInformationMM.numberOfDPEncountered = 
				p->m_pOrgRec->cAMELInformationMM.numberOfDPEncountered;

		m_CAMELInformationMM.levelOfCAMELService = 
				p->m_pOrgRec->cAMELInformationMM.levelOfCAMELService;
				
		/*
		 * fFDAppendIndicator字段存在且值为TRUE且freeFormatData
		 * 信息长度不为零，则和前面的话单合并
		 */
		//fFDAppendIndicator 字段不存在或者该域为FALSE
		//freeFormatData存在而且信息长度不为零
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
		
		//fFDAppendIndicator 字段存在且值为TRUE
		//freeFormatData存在而且信息长度不为零
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
