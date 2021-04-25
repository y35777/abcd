#ifndef _3G_R4_BILL_H
#define _3G_R4_BILL_H

#pragma warning(disable:4101)
#pragma warning(disable:4244)


#include "../asn1_r4/CDRF_3GPPR4_Org.h"
#include "../asn1_r4/CDRF_3GPPR4_Fin.h"
#include "../asn1_r4/oss_asn1_r4.h"
#include "../../merge/bill.h"
#include <assert.h>

/*
*最大允许合并的话单数。
*/
#define MAX_MERGED_BILLS    255

/*
* SGSN切换合理的时间误差
*/
#define SGSN_CHANGE_TIME 2
/*
*合并前话单类型。
*/
#define    R4_S_CDR    R4_Org_CallEventRecordType_sgsnPDPRecord
#define    R4_G_CDR    R4_Org_CallEventRecordType_ggsnPDPRecord

static BOOL Match_gsnAddress(R4_Org_GSNAddress* p1,R4_Org_GSNAddress* p2);

class IGWB_Svc_Export C3G_R4_BillParser:public CBillParser
{
protected:
    CBill* m_pBill;

public:
    C3G_R4_BillParser();
    virtual ~C3G_R4_BillParser();

    virtual BOOL ParseARecord(char* pBuf,UINT4 uBufLen);//根据pBuf生成一个话单对象。
	virtual CBill* GetBillRecord();                     //返回话单记录对象。

	virtual UINT4 GetBillTypeCount();                   //返回话单类型数。
    virtual UINT4 GetBillTypeID(UINT4 uIndex);          //根据索引返回话单类型ID。
    virtual void RegisterBillType(CMerger* pMerger);    //在CMerger对象中注册话单类型。

	void GetBillTypeDesc(UINT4 uBillTypeID, char* szDesc);
};

class C3G_R4_CDR:public CBill
{
protected:

     //达到门限标志
     BOOL m_bIsLimit;
    //用作合并SGSNAddress字段的链表结构。
    //m_pSGSNAddress指向链表头的指针，
    //m_pSGSNAddressTail指向链表尾的指针。
	R4_Fin__seqof2_ *m_pSGSNAddress,*m_pSGSNAddressTail;

    //链表的长度(元素数)
    unsigned int m_nLen_SGSNAddress_List;

    //用作合并listOfTrafficVolumes字段的链表结构。
    //m_pCondition指向链表头的指针，
    //m_pConditionTail指向链表尾的指针。
    R4_Org__seqof1_* m_pCondition,*m_pConditionTail;

    //链表的长度(元素数)
    unsigned int m_nLen_Condition_List;
    
    //用作合并recordExtensions字段的链表结构。
    //m_pExtension指向链表头的指针，
    //m_pExtensionTail指向链表尾的指针。
    R4_Org_ManagementExtensions_ *m_pExtension,*m_pExtensionTail;

    //链表的长度(元素数)
    unsigned int m_nLen_Extension_List;

    //用作合并recordSequenceNumber字段的链表结构。
    //m_pSequence指向链表头的指针，
    //m_pSequenceTail指向链表尾的指针。
	R4_Fin__seqof1_* m_pRecordSequence,*m_pRecordSequenceTail;

	//用作合并localSequenceNumber字段的链表结构。
    //m_pSequence指向链表头的指针，
    //m_pSequenceTail指向链表尾的指针。
	R4_Fin__seqof1_* m_pLocalSequence,*m_pLocalSequenceTail;

	//用作合并rNCDownlinkVolume字段的链表结构。
    //m_pSequence指向链表头的指针，
    //m_pSequenceTail指向链表尾的指针。
	R4_Fin__seqof1_* m_pRNCDownlinkVolume,*m_pRNCDownlinkVolumeTail;

    //用作合并changeLocation字段的链表结构。
    //m_pLocation指向链表头的指针，
    //m_pLocationTail指向链表尾的指针。
	R4_Org_SGSNMMRecord::R4_Org__seqof3* m_pLocation,*m_pLocationTail;

    //链表的长度(元素数)
    unsigned int m_nLen_Location_List;

protected:
    //判断是否闰年。
    BOOL isLeapYear(int nYear)
    {
        if( nYear % 400 == 0)
            return TRUE;
        if( nYear % 100 == 0)
            return FALSE;
        if( nYear % 4 == 0)
            return TRUE;
        return FALSE;
    }
    
    //输入年月，返回该月的天数。
    int GetDaysOfMonth(int nYear, int nMonth)
    {
        switch( nMonth )
        {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
            break;
        case 2:
            if(isLeapYear(nYear))
                return 29;
            else
                return 28;
            break;
        default:
            return -1;
            break;
        }
    }

public:
    C3G_R4_CDR()
    {
        m_pSGSNAddress = NULL;
        m_pSGSNAddressTail = NULL;

        m_pCondition = NULL;
        m_pConditionTail = NULL;

        m_pExtension = NULL;
        m_pExtensionTail = NULL;

        m_pRecordSequence = NULL;
        m_pRecordSequenceTail = NULL;

		m_pLocalSequence = NULL;
		m_pLocalSequenceTail = NULL;

		m_pRNCDownlinkVolume = NULL;
		m_pRNCDownlinkVolumeTail = NULL;

        m_pLocation = NULL;
        m_pLocationTail = NULL;

        /*
         * 用于进行合并流量控制的成员变量。
         * Added by zhouyonghui,2001/09/25.
         */
        m_nLen_SGSNAddress_List = 0;
        m_nLen_Condition_List = 0;
        m_nLen_Extension_List = 0;
        m_nLen_Location_List = 0;
    };

    ~C3G_R4_CDR()
    {
        if(m_pSGSNAddress != NULL)
        {
            Clear_GsnAddressList(m_pSGSNAddress);
            m_pSGSNAddress = NULL;
        }

        if(m_pCondition != NULL)
        {
            Clear_ChangeOfCharCondition(m_pCondition);
            m_pCondition = NULL;
        }

        if(m_pExtension != NULL)
        {
            Clear_ManagementExtensions(m_pExtension);
            m_pExtension = NULL;
        }

        if(m_pRecordSequence != NULL)
        {
            Clear_SequenceNumList(m_pRecordSequence);
            m_pRecordSequence = NULL;
        }

		if(m_pLocalSequence != NULL)
        {
            Clear_SequenceNumList(m_pLocalSequence);
            m_pLocalSequence = NULL;
        }

		if(m_pRNCDownlinkVolume != NULL)
        {
            Clear_SequenceNumList(m_pRNCDownlinkVolume);
            m_pRNCDownlinkVolume = NULL;
        }

        if(m_pLocation != NULL)
        {
            Clear_ChangeLocationList(m_pLocation);
            m_pLocation = NULL;
        }
    };

protected:

    
    /******************************************************
    * 函数名:   Clear_Diagnostics
    * 作者:    Guofeng, Guan
    * 日期:    2003-06-02
    * 描述:    删除R4_Fin_Diagnostics结构中的指针成员。
    * 输入: 
    *       参数1 :R4_Fin_Diagnostics* p
    * 返回值: void 
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *2003-06-02        Guofeng, Guan
    ******************************************************/
    void Clear_Diagnostics(R4_Org_Diagnostics* p)
    {
        assert(p != NULL);

        if(p->choice == R4_Org_networkSpecificCause_chosen)
        {
            delete[] p->u.networkSpecificCause.identifier.value;
            delete[] p->u.networkSpecificCause.information.value;
        }
        else if(p->choice == R4_Org_manufacturerSpecificCause_chosen)
        {
            delete[] p->u.manufacturerSpecificCause.identifier.value;
            delete[] p->u.manufacturerSpecificCause.information.value;
        }
    }

	/******************************************************
    * 函数名:   Clone_ManagementExtension
    * 作者:    Guofeng, Guan
    * 日期:    05-28-2003
    * 描述:    “克隆”一个R4_Org_ManagementExtension结构。
    * 输入: 
    *       参数1 :R4_Org_ManagementExtension* p,结构指针
    *       
    * 返回值: R4_Org_ManagementExtension*,克隆的结果
    *       若内存操作失败，返回NULL。
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *2003-06-02       Guofeng, Guan
    ******************************************************/
	R4_Org_ManagementExtension* 
		Clone_ManagementExtension(R4_Org_ManagementExtension * p)
    {
        assert(p != NULL);

		R4_Org_ManagementExtension* q = NULL;
		
		try
		{
			q = new R4_Org_ManagementExtension;
			if (q == NULL)
			{
				throw "Alloc memory fail.";
			}

			memcpy(q, p, sizeof(R4_Org_ManagementExtension));
			
			unsigned char* tmp = NULL;
			
			tmp = new unsigned char[p->identifier.length];
			if (tmp == NULL)
			{
				throw "Alloc memory fail.";
			}
			
			memcpy(tmp, p->identifier.value, p->identifier.length);
			q->identifier.value = tmp;

			tmp = new unsigned char[p->information.length];
			if (tmp == NULL)
			{
				throw "Alloc memory fail.";
			}
			
			memcpy(tmp, p->information.value, p->information.length);
			q->information.value = tmp;
			
			return q;
		}
		catch(char* e)
		{
			if (q != NULL)
			{
				delete[] q->identifier.value;
				delete[] q->information.value;
				delete q;
			}
		}

		throw "Alloc memory fail.";
		return NULL;
    }
    /******************************************************
    * 函数名:   Clone_ManagementExtensions
    * 作者:    Guofeng, Guan
    * 日期:    2003-06-02
    * 描述:    “克隆”一个R4_Org_ManagementExtensions_链表。
    * 输入: 
    *       参数1 :R4_Org_ManagementExtensions_* p,链表指针
    *       参数2 :R4_Org_ManagementExtensions_*& pTail,拷贝链表的尾指针
	*       参数3 :unsigned int& nCount,拷贝的链表元素个数
    * 返回值: R4_Org_ManagementExtensions_*,克隆的结果
    *       若内存操作失败，返回NULL。
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *2003-06-02       Guofeng, Guan
    ******************************************************/
    R4_Org_ManagementExtensions_* Clone_ManagementExtensions(
		R4_Org_ManagementExtensions_* p, R4_Org_ManagementExtensions_*& pTail, unsigned int& nCount)
    {
        assert(p != NULL);

		nCount = 0;
		R4_Org_ManagementExtensions_* pHead = NULL, *q = NULL, *pTemp = NULL;
		R4_Org_ManagementExtension* pValue = NULL;
		
		try
		{
			pHead = new R4_Org_ManagementExtensions_;
			if (pHead == NULL)
			{
				throw "Alloc memory fail.";
			}
			pHead->next = NULL;
			nCount++;
			pValue = Clone_ManagementExtension(&(p->value));
			pHead->value = *pValue;
			delete pValue;
			
			q = pHead;
			p = p->next;
			while(p != NULL)
			{
				pTemp = new R4_Org_ManagementExtensions_;
				if(pTemp == NULL)
				{
					throw "Alloc memory fail.";
				}
				pTemp->next = NULL;				
				pValue = Clone_ManagementExtension(&(p->value));
				pTemp->value = *pValue;
				delete pValue;
				
				q->next = pTemp;
				q = pTemp;
				nCount++;
				
				p = p->next;
			}

			pTail = q;
			return pHead;
		}
		catch(char* e)
		{
			q = pHead;			
			while(q != NULL)
			{
				pTemp = q;
				q = q->next;
				delete[] pTemp->value.identifier.value;
				delete[] pTemp->value.information.value;
				delete pTemp;				
			}

			pTail = NULL;
			nCount = 0;
		}

		throw "Alloc memory fail.";
		return NULL;
    }

    /******************************************************
    * 函数名:   Clear_ManagementExtension
    * 作者:    Guofeng, Guan
    * 日期:    2003-06-02
    * 描述:    删除结构R4_Org_ManagementExtension的内存空间。
    * 输入: 
    *       参数1 :R4_Org_ManagementExtension* p,结构指针。
    *     
    * 返回值: void* 
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *2003-06-02        Guofeng, Guan
    ******************************************************/
    void Clear_ManagementExtension(R4_Org_ManagementExtension* p)
    {
        assert(p != NULL);

        delete[] p->identifier.value;
        delete[] p->information.value;
    }

	/******************************************************
    * 函数名:   Clear_ManagementExtensions
    * 作者:    Zhou Yonghui
    * 日期:    01-6-8
    * 描述:    删除链表R4_Org_ManagementExtensions_的内存空间。
    * 输入: 
    *       参数1 :R4_Org_ManagementExtensions_* p,链表指针。
    *     
    * 返回值: void* 
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *2003-06-02        Guofeng, Guan
    ******************************************************/
	void Clear_ManagementExtensions(R4_Org_ManagementExtensions_* p)
    {
        assert(p != NULL);

        R4_Org_ManagementExtensions_* pPrev = NULL, *pNext = NULL;
		pPrev = p;
		pNext = p;
		while(pNext != NULL)
		{
			pNext = pNext->next;
			delete[] pPrev->value.identifier.value;
			delete[] pPrev->value.information.value;
			delete pPrev;
			pPrev = pNext;
		}
    }

    /******************************************************
    * 函数名:   Clone_ChangeOfCharCondition
    * 作者:    Guofeng, Guan
    * 日期:    2003-06-02
    * 描述:    “克隆”一个R4_Org__seqof1_*链表。
    * 输入: 
    *       参数1 :R4_Org__seqof1_* p,链表指针
    *       参数2 :unsigned int& nCount,拷贝的链表元素个数
    * 返回值: R4_Org__seqof1_*,克隆的结果
    *       若内存操作失败，返回NULL。
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *2003-06-02       Guofeng, Guan
    ******************************************************/
    R4_Org__seqof1_* Clone_ChangeOfCharCondition( 
		R4_Org__seqof1_* p, R4_Org__seqof1_*&pTail, unsigned int& nCount)
    {
        assert(p != NULL);

		nCount = 0;
		R4_Org__seqof1_* pHead = NULL, *q = NULL, *pTemp = NULL;
		try
		{
			pHead = new R4_Org__seqof1_;
			if (pHead == NULL)
			{
				throw "Alloc memory fail.";
			}
			memcpy(&(pHead->value), &(p->value), sizeof(R4_Org_ChangeOfCharCondition));
			pHead->next = NULL;
			nCount++;

			q = pHead;
			p = p->next;
			while(p != NULL)
			{
				pTemp = new R4_Org__seqof1_;
				if (pTemp == NULL)
				{
					throw "Alloc memory fail.";
				}
				memcpy(&(pTemp->value), &(p->value), sizeof(R4_Org_ChangeOfCharCondition));
				pTemp->next = NULL;
				q->next = pTemp;
				q = pTemp;			
				nCount++;

				p = p->next;
			}

			pTail = q;
			return pHead;
		}
		catch(char* e)
		{
			while(pHead != NULL)
			{
				pTemp = pHead;
				pHead = pHead->next;
				delete pTemp;
			}

			pTail = NULL;
			nCount = 0;
		}

		throw "Alloc memory fail.";
		return NULL;
    }

    /******************************************************
    * 函数名:   Clone_GsnAddressList
    * 作者:    Guofeng, Guan
    * 日期:    2003-06-02
    * 描述:    “克隆”一个R4_Org_GGSNPDPRecord::R4_Org__seqof2*链表。
    * 输入: 
    *       参数1 :R4_Org_GGSNPDPRecord::R4_Org__seqof2* p,链表指针
    *       参数2 :unsigned int& nCount,拷贝的链表元素个数
    * 返回值: R4_Org_GGSNPDPRecord::R4_Org__seqof2*,克隆的结果
    *       若内存操作失败，抛出异常。
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *2003-06-02       Guofeng, Guan
    ******************************************************/
	R4_Fin__seqof2_* Clone_GsnAddressList(R4_Fin__seqof2_* p, 
		R4_Fin__seqof2_*& pTail, unsigned int& nCount)
    {
        assert(p != NULL);

		nCount = 0;
		R4_Fin__seqof2_* pHead = NULL, *q = NULL, *pTemp = NULL;
		try
		{
			pHead = new R4_Fin__seqof2_;
			if (pHead == NULL)
			{
				throw "Alloc memory fail.";
			}
			pHead->next = NULL;
			pHead->value = p->value;
			nCount++;

			q = pHead;
			p = p->next;
			while(p)
			{
				pTemp = new R4_Fin__seqof2_;
				if (pTemp == NULL)
				{
					throw "Alloc memory fail.";
				}				
				pTemp->value = p->value;
				pTemp->next = NULL;
				q->next = pTemp;
				q = pTemp;
				nCount++;

				p = p->next;				
			}

			pTail = q;
			return pHead;
		}
		catch(char* e)
		{			
			q = pHead;
			while(q)
			{
				pTemp = q;
				q = q->next;
				delete pTemp;				
			}
			
			pTail = NULL;
			nCount = 0;
		}
        
		throw "Alloc memory fail.";
        return NULL;
    }

    /******************************************************
    * 函数名:   Clone_GsnAddressList
    * 作者:    Guofeng, Guan
    * 日期:    2003-06-02
    * 描述:    “克隆”一个R4_Org_GGSNPDPRecord::R4_Org__seqof2*链表。
    * 输入: 
    *       参数1 :R4_Org_GSNAddress* pDiff,剔除重复的SGSN地址的参照
    *       参数2 :R4_Org_GGSNPDPRecord::R4_Org__seqof2* p,链表指针
    *       参数3 :unsigned int& nCount,返回实际复制的
    *              元素数.
    * 返回值: R4_Org_GGSNPDPRecord::R4_Org__seqof2*,克隆的结果,已经剔除了重复的SGSN地址.
    *       若p中的所有元素都与pDiff重复,则返回空.
    *       若内存操作失败,抛出异常.
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *2003-06-02       Guofeng, Guan
    ******************************************************/
	R4_Fin__seqof2_* Clone_GsnAddressList(R4_Fin_GSNAddress* pDiff,
		R4_Fin__seqof2_* p, R4_Fin__seqof2_*& pTail, unsigned int& nCount)
    {
        assert(p != NULL);
		
		nCount = 0;
		R4_Fin__seqof2_* pStartPos = p;

        while(pStartPos)
        {
            if(Match_gsnAddress((R4_Org_GSNAddress*)pDiff, (R4_Org_GSNAddress*)(&(pStartPos->value))))
            {
                pStartPos = pStartPos->next;               
            }
            else
                break;
        }

        if(pStartPos == NULL)
        {
			pTail = NULL;
            nCount = 0;
            return NULL;
        }

        //克隆余下的部分
		R4_Fin__seqof2_* q = Clone_GsnAddressList(pStartPos, pTail, nCount);
		if(q == NULL)
		{				
			throw "Alloc memory fail.";
		}

        return q;
    }

    /******************************************************
    * 函数名:   Clone_ChangeLocationList
    * 作者:    Guofeng, Guan
    * 日期:    2003-06-02
    * 描述:    “克隆”一个R4_Org_SGSNMMRecord::R4_Org__seqof3*链表。
    * 输入: 
    *       参数1 :R4_Org_SGSNMMRecord::R4_Org__seqof3* p,链表指针
	*       参数2 :
    *       参数3 :unsigned int& nCount,拷贝的链表元素数
    * 返回值: R4_Org_SGSNMMRecord::R4_Org__seqof3*,克隆的结果
    *       若内存操作失败，抛出异常。
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *2003-06-02       Guofeng, Guan
    ******************************************************/
    R4_Org_SGSNMMRecord::R4_Org__seqof3* Clone_ChangeLocationList(R4_Org_SGSNMMRecord::R4_Org__seqof3* p,unsigned int& nCount)
    {
        assert(p != NULL);

		nCount = 0;
		R4_Org_SGSNMMRecord::R4_Org__seqof3* pHead = NULL, *pPrev = NULL, *pNext = NULL;
		try
		{
			pHead = new R4_Org_SGSNMMRecord::R4_Org__seqof3;
			if (pHead == NULL)
			{
				throw "Alloc memory fail.";
			}
			pHead->next = pNext;
			pHead->value = p->value;
			nCount++;

			pPrev = pHead;
			p = p->next;
			while(p)
			{
				pNext = new R4_Org_SGSNMMRecord::R4_Org__seqof3;
				if (pNext == NULL)
				{
					throw "Alloc memory fail.";
				}
				pPrev->next = pNext;
				pNext->value = p->value;
				pNext->next = NULL;
				pPrev = pNext;
				pNext = NULL;
				nCount++;

				p = p->next;
			}

			return pHead;
		}
		catch(char* e)
		{
			pPrev = pHead;
			pNext = pHead;
			while(pNext)
			{
				pNext = pNext->next;
				delete pPrev;
				pPrev = pNext;
			}

			nCount = 0;
		}
        
		throw "Alloc memory fail.";
        return NULL;
    }
    
    /******************************************************
    * 函数名:   Clear_ChangeOfCharCondition
    * 作者:    Guofeng, Guan
    * 日期:    2003-06-02
    * 描述:    删除ChangeOfCharCondition链表空间。
    * 输入: 
    *       参数1 :R4_Org__seqof1_* pHead
    * 返回值: void 
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *2003-06-02       Guofeng, Guan
    ******************************************************/
    void Clear_ChangeOfCharCondition(R4_Org__seqof1_* pHead)
    {
        assert(pHead != NULL);

        R4_Org__seqof1_* q = pHead,* p = NULL;
        while(q)
        {
            p = q;
            q = q->next;
  
            delete p;
        }
    }
    
    /******************************************************
    * 函数名:   Clear_SequenceNumList
    * 作者:    Guofeng, Guan
    * 日期:    2003-06-02
    * 描述:    删除R4_Fin__seqof1_链表空间，
	*          这个链表被recordSequenceNumberList 
	*          LocalSequenceNumberList 
	*          rNCDownlinkVolumeList使用到。
    * 输入: 
    *       参数1 :R4_Fin__seqof1* pHead
    * 返回值: void 
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *2003-06-02       Guofeng, Guan
    ******************************************************/
    void Clear_SequenceNumList(R4_Fin__seqof1_* pHead)
    {
        assert(pHead != NULL);

		R4_Fin__seqof1_* q = pHead,* p = NULL;
        while(q)
        {
            p = q;
            q = q->next;
            delete p;
        }
    }

	/******************************************************
    * 函数名:   Clear_RecSequenceNumList
    * 作者:    Guofeng, Guan
    * 日期:    2003-06-02
    * 描述:    删除recordSequenceNumberList  
    * 输入: 
    *       参数1 :T* pHead
    * 返回值: void 
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *2003-06-02       Guofeng, Guan
    ******************************************************/
	void Clear_RecSequenceNumList(R4_Fin_SGSNPDPRecord::R4_Fin__seqof4* pHead)
    {
        assert(pHead != NULL);

		R4_Fin_SGSNPDPRecord::R4_Fin__seqof4* q = pHead,* p = NULL;
        while(q)
        {
            p = q;
            q = q->next;
			Clear_SequenceNumList(p->value.recSequenceNumberList);

            delete p;
        }
    }

	/******************************************************
    * 函数名:   Clear_LocalSequenceNumList
    * 作者:    Guofeng, Guan
    * 日期:    2003-06-02
    * 描述:    删除localSequenceNumberList 
    * 输入: 
    *       参数1 :R4_Fin_SGSNPDPRecord::R4_Fin__seqof5* pHead
    * 返回值: void 
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *2003-06-02       Guofeng, Guan
    ******************************************************/
	void Clear_LocalSequenceNumList(R4_Fin_SGSNPDPRecord::R4_Fin__seqof5* pHead)
    {
        assert(pHead != NULL);

		R4_Fin_SGSNPDPRecord::R4_Fin__seqof5* q = pHead,* p = NULL;
        while(q)
        {
            p = q;
            q = q->next;
			Clear_SequenceNumList(p->value.localSeqNumberList);

            delete p;
        }
    }
    
	/******************************************************
    * 函数名:   Clear_RNCDownlinkVolumeList
    * 作者:    Guofeng, Guan
    * 日期:    2003-06-02
    * 描述:    删除localSequenceNumberList 
    * 输入: 
    *       参数1 :R4_Fin_SGSNPDPRecord::R4_Fin__seqof6* pHead
    * 返回值: void 
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *2003-06-02       Guofeng, Guan
    ******************************************************/
	void Clear_RNCDownlinkVolumeList(R4_Fin_SGSNPDPRecord::R4_Fin__seqof6* pHead)
    {
        assert(pHead != NULL);

		R4_Fin_SGSNPDPRecord::R4_Fin__seqof6* q = pHead,* p = NULL;
        while(q)
        {
            p = q;
            q = q->next;
			Clear_SequenceNumList(p->value.rNCUnsentDnVolumeList);

            delete p;
        }
    }

    /******************************************************
    * 函数名:   Clear_GsnAddressList
    * 作者:    Guofeng, Guan
    * 日期:    2003-06-02
    * 描述:    删除GsnAddressList结构链表。
    * 输入: 
    *       参数1 :R4_Org__seqof2_* pHead
    * 返回值: void 
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *2003-06-02        Guofeng, Guan
    ******************************************************/
    void Clear_GsnAddressList(R4_Fin__seqof2_* pHead)
    {
        assert(pHead != NULL);

		R4_Fin__seqof2_* q = pHead,* p = NULL;
        while(q)
        {
            p = q;
            q = q->next;

            delete p;
        }
    }
    
    /******************************************************
    * 函数名:   Clear_ChangeLocationList
    * 作者:    Guofeng, Guan
    * 日期:    2003-06-02
    * 描述:    删除ChangeLocationList链表空间。
    * 输入: 
    *       参数1 :R4_Org__seqof3* pHead
    * 返回值: void 
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *2003-06-02        Guofeng, Guan
    ******************************************************/
    void Clear_ChangeLocationList(R4_Org_SGSNMMRecord::R4_Org__seqof3* pHead)
    {
        assert(pHead != NULL);

		R4_Org_SGSNMMRecord::R4_Org__seqof3* q = pHead,* p = NULL;
        while(q)
        {
            p = q;
            q = q->next;

            delete p;
        }
    }

    //计算两个时间之差。
	/* 说明：函数基于以下考虑：如果t2时区为zz2，t1时区为zz1，
	 * 那么t2相当于格林尼治时间t2-zz2，t1则相当于格林尼治时间t1-zz1，
	 * 因此时间差为(t2-zz2)-(t1-zz1)，也就是t2-t1+zz1-zz2
	 */
    int Get_Duration(R4_Org_TimeStamp* t1, R4_Org_TimeStamp* t2)
    {
        int nDiff = 0; //时间之差
        
        char zhh1,zMM1,yy1,mm1,dd1,hh1,MM1,ss1; //zhh1,zMM1,zhh2,zMM2表示时区的小时、分钟
        char zhh2,zMM2,yy2,mm2,dd2,hh2,MM2,ss2;
        
        yy1 = ((t1->value[0] & 0xF0 ) >> 4  ) * 10 + (t1->value[0] & 0x0f) ;
        mm1 = ((t1->value[1] & 0xF0 ) >> 4  ) * 10 + (t1->value[1] & 0x0f) ;
        dd1 = ((t1->value[2] & 0xF0 ) >> 4  ) * 10 + (t1->value[2] & 0x0f) ;
        hh1 = ((t1->value[3] & 0xF0 ) >> 4  ) * 10 + (t1->value[3] & 0x0f) ;
        MM1 = ((t1->value[4] & 0xF0 ) >> 4  ) * 10 + (t1->value[4] & 0x0f) ;
        ss1 = ((t1->value[5] & 0xF0 ) >> 4  ) * 10 + (t1->value[5] & 0x0f) ;
        
        zhh1 = ((t1->value[7] & 0xF0 ) >> 4  ) * 10 + (t1->value[7] & 0x0f) ;
        zMM1 = ((t1->value[8] & 0xF0 ) >> 4  ) * 10 + (t1->value[8] & 0x0f) ;
        
        if(t1->value[6] == '-')
        {
           zhh1 = -zhh1;
           zMM1 = - zMM1;
        }
        
        yy2 = ((t2->value[0] & 0xF0 ) >> 4  ) * 10 + (t2->value[0] & 0x0f) ;
        mm2 = ((t2->value[1] & 0xF0 ) >> 4  ) * 10 + (t2->value[1] & 0x0f) ;
        dd2 = ((t2->value[2] & 0xF0 ) >> 4  ) * 10 + (t2->value[2] & 0x0f) ;
        hh2 = ((t2->value[3] & 0xF0 ) >> 4  ) * 10 + (t2->value[3] & 0x0f) ;
        MM2 = ((t2->value[4] & 0xF0 ) >> 4  ) * 10 + (t2->value[4] & 0x0f) ;
        ss2 = ((t2->value[5] & 0xF0 ) >> 4  ) * 10 + (t2->value[5] & 0x0f) ;
        
        zhh2 = ((t2->value[7] & 0xF0 ) >> 4  ) * 10 + (t2->value[7] & 0x0f) ;
        zMM2 = ((t2->value[8] & 0xF0 ) >> 4  ) * 10 + (t2->value[8] & 0x0f) ;

        if(t2->value[6] == '-')
        {
           zhh2 = -zhh2;
           zMM2 = -zMM2;
        }
        
        int nDays = 0;
        if( yy2 == yy1) 
        {
            for( int m = mm1; m < mm2; m++)
                nDays += GetDaysOfMonth(yy1,m);
        }
        else
        {
            assert( yy2 > yy1);//日期先后关系校验

            int m = 0;
            for(m = mm1; m <=12; m++)
                nDays += GetDaysOfMonth(yy1, m);

            for(m = 1; m < mm2; m++)
                nDays += GetDaysOfMonth(yy2, m);
        }
        
        /*
         * 一个GSN生成的话单中年、月、日、时、分、秒，都可能改变，
         * GSN的时区也可能改变
         * 
         */
        nDiff = (((nDays + (dd2 - dd1)) * 24 
                + (hh2 - hh1 + zhh1 - zhh2)) * 60 
                + (MM2 - MM1 + zMM1 - zMM2))* 60 + (ss2 - ss1);
        return nDiff;
    }
};

/*
*S-CDR类结构。
*/
class C3G_R4_SCDR:public C3G_R4_CDR
{
protected:
    PackID_t m_nMinRecordSequence;          //当前合并结果中最小的序列号。
    PackID_t m_nCurRecordSequence;          //当前完成合并的最大序列号。
    BOOL m_bPDPCompleted;                   //PDP是否结束标志。
    BOOL m_bPrepared;                       //标志是否为合并准备过数据。

protected:
    R4_Org_CallDuration  m_Duration;         //用于累加通话时长字段。
    R4_Org_DataVolumeGPRS m_UnsentVolume;    //用于累加rNCUnsentDownlinkVolume字段。
    R4_Org_Diagnostics m_Diagnostics;        //用于存放最后一张话单的diagnostics字段。
    R4_Org_CauseForRecClosing m_CauseForRecClosing;
                                            //用于存放最后一张话单的causeForRecClosing字段。
         
	R4_Org_CAMELInformationPDP m_CAMELInformationPDP;
                                            //用于合并话单的CAMELInformationPDP字段

    R4_Fin_SGSNPDPRecord::R4_Fin__seqof4 *m_pRecordSequenceList, *m_pRecordSequenceListTail;
    R4_Fin_SGSNPDPRecord::R4_Fin__seqof5 *m_pLocalSequenceList,  *m_pLocalSequenceListTail;
    R4_Fin_SGSNPDPRecord::R4_Fin__seqof6 *m_pRNCDownlinkVolumeList, *m_pRNCDownlinkVolumeListTail;
	
public:
    R4_Org_SGSNPDPRecord* m_pOrgRec;         //原始话单结构。
    R4_Fin_CallEventRecord* m_pDestRec;     //合并后话单结构。

    C3G_R4_SCDR();
    ~C3G_R4_SCDR();

    virtual BOOL Init(char* pBuf,UINT4 uLen); //使用话单记录初始化话单对象。

    virtual BOOL CanSafeMerge(CBill* pBill);//判断当前话单是否允许与pBill合并。
    virtual BOOL Merge(CBill* pBill);       //把pBill合入当前话单中。
    virtual BOOL IsPDPCompleted();          //判断PDP已经结束。
    virtual BOOL NeedWaiting();             //判断是否需要继续等待后续话单。

    virtual char* GetContents();            //返回话单的记录结构。

    virtual BOOL IsPartial();               //判断是否部分话单。
    virtual BOOL IsHotBilling();            //判断是否是热计费
    virtual BOOL IsFirstPartialRec();       //判断是否PDP的第一张部分话单。
    virtual BOOL IsLastPartialRec();        //判断是否PDP的末张部分话单。
    virtual BOOL IsDuplicated(CBill* pBill);  //判断是否重复话单    

    virtual UINT4 GetBillType();              //返回话单类型ID值。

    virtual CKey CreateKey();               //创建索引对象。

    BOOL PrepareData();                     //为合并准备数据。
};

/*
*G-CDR类结构。
*/
class C3G_R4_GCDR:public C3G_R4_CDR
{
protected:
    PackID_t m_nMinRecordSequence;          //当前合并结果中最小的序列号。
    PackID_t m_nCurRecordSequence;          //当前合并结果中最大的序列号。
    BOOL m_bPDPCompleted;                   //PDP是否结束标志。
    BOOL m_bPrepared;                       //标志是否为合并准备过数据。

protected:
    R4_Org_CallDuration  m_Duration;         //用于累加通话时长字段。
    R4_Org_Diagnostics m_Diagnostics;        //用于存放最后一张话单的diagnostics字段。
    R4_Org_CauseForRecClosing m_CauseForRecClosing;
                                            //用于存放最后一张话单的causeForRecClosing字段。

public:
    R4_Org_GGSNPDPRecord* m_pOrgRec;         //原始话单结构。
    R4_Fin_CallEventRecord* m_pDestRec;     //合并后话单结构。

    C3G_R4_GCDR();
    ~C3G_R4_GCDR();

    virtual BOOL Init(char* pBuf,UINT4 uLen); //使用话单记录初始化话单对象。

    virtual BOOL CanSafeMerge(CBill* pBill);//判断当前话单是否允许与pBill合并。
    virtual BOOL Merge(CBill* pBill);       //把pBill合入当前话单中。
    virtual BOOL IsPDPCompleted();          //判断PDP已经结束。
    virtual BOOL NeedWaiting();             //判断是否需要继续等待后续话单。

    virtual char* GetContents();            //返回话单的记录结构。

    virtual BOOL IsPartial();               //判断是否部分话单。
    virtual BOOL IsHotBilling();            //判断是否是热计费
    virtual BOOL IsFirstPartialRec();       //判断是否PDP的第一张部分话单。
    virtual BOOL IsLastPartialRec();        //判断是否PDP的末张部分话单。
    virtual BOOL IsDuplicated(CBill* pBill);  //判断是否重复话单    

    virtual UINT4 GetBillType();              //返回话单类型ID值。

    virtual CKey CreateKey();               //创建索引对象。

    BOOL PrepareData();                     //为合并准备数据。
};

/*
*M-CDR类结构。
*/
class C3G_R4_MCDR:public C3G_R4_CDR
{
protected:
    PackID_t m_nMinRecordSequence;          //当前合并结果中最小的序列号。
    PackID_t m_nCurRecordSequence;          //当前合并结果中最大的序列号。
    BOOL m_bPDPCompleted;                   //PDP是否结束标志。
    BOOL m_bPrepared;                       //标志是否为合并准备过数据。

protected:
    R4_Org_CallDuration  m_Duration;         //用于累加通话时长字段。
    R4_Org_Diagnostics m_Diagnostics;        //用于存放最后一张话单的diagnostics字段。
    R4_Org_CauseForRecClosing m_CauseForRecClosing;
                                            //用于存放最后一张话单的causeForRecClosing字段。
    
	R4_Org_CAMELInformationMM m_CAMELInformationMM;
                                            //用于存放合并话单的CAMELInforamtionMM字段。

public:
    R4_Org_SGSNMMRecord* m_pOrgRec;          //原始话单结构。
    R4_Fin_CallEventRecord* m_pDestRec;     //合并后话单结构。

    C3G_R4_MCDR();
    ~C3G_R4_MCDR();

    virtual BOOL Init(char* pBuf,UINT4 uLen); //使用话单记录初始化话单对象。

    virtual BOOL CanSafeMerge(CBill* pBill);//判断当前话单是否允许与pBill合并。
    virtual BOOL Merge(CBill* pBill);       //把pBill合入当前话单中。
    virtual BOOL IsPDPCompleted();          //判断PDP已经结束。
    virtual BOOL NeedWaiting();             //判断是否需要继续等待后续话单。

    virtual char* GetContents();            //返回话单的记录结构。

    virtual BOOL IsPartial();               //判断是否部分话单。
    virtual BOOL IsHotBilling();            //判断是否是热计费

    virtual BOOL IsFirstPartialRec();       //判断是否PDP的第一张部分话单。
    virtual BOOL IsLastPartialRec();        //判断是否PDP的末张部分话单。
    virtual BOOL IsDuplicated(CBill* pBill);  //判断是否重复话单    

    virtual UINT4 GetBillType();            //返回话单类型ID值。

    virtual CKey CreateKey();               //创建索引对象。

    BOOL PrepareData();                     //为合并准备数据。
};

class C3G_R4_CDR_Key:public CKey
{
public:
    C3G_R4_CDR_Key(){};
    ~C3G_R4_CDR_Key(){};

    //比较两个IMSI地址
    BOOL Match_IMSI(R4_Org_IMSI* p1,R4_Org_IMSI* p2)
    {
        assert(p1 != NULL);
        assert(p2 != NULL);

        if(p1->length != p2->length)
        {
            return FALSE;
        }
        
        return (memcmp(p1->value,p2->value,p1->length) == 0);
    }
};

//与C3G_R4_SCDR话单类对应的索引类。
class C3G_R4_SCDR_Key:public C3G_R4_CDR_Key
{
public:
    //R4_Org_GSNAddress sgsnAddress;
	//BOOL m_bSgsnAddressPresent;
    R4_Org_GSNAddress ggsnAddressUsed;
    R4_Org_ChargingID chargingID;
public:
    C3G_R4_SCDR_Key(){};
    ~C3G_R4_SCDR_Key(){};

    BOOL IsMatch(CKey* pKey)
    {
        assert(pKey != NULL);

        C3G_R4_SCDR_Key* p = (C3G_R4_SCDR_Key*)pKey;
		return (Match_gsnAddress(&(p->ggsnAddressUsed),&ggsnAddressUsed)
         && (p->chargingID == chargingID));
    }

    UINT4 GetKey()
    {
        return chargingID;
    }
};

//与C3G_R4_GCDR话单类对应的索引类。
class C3G_R4_GCDR_Key:public C3G_R4_CDR_Key
{
public:
    R4_Org_GSNAddress ggsnAddress;
    R4_Org_ChargingID chargingID;
public:
    C3G_R4_GCDR_Key(){};
    ~C3G_R4_GCDR_Key(){};

    BOOL IsMatch(CKey* pKey)
    {
        assert(pKey != NULL);

        C3G_R4_GCDR_Key* p = (C3G_R4_GCDR_Key*)pKey;
        return (Match_gsnAddress(&(p->ggsnAddress),&ggsnAddress)
            && (p->chargingID == chargingID));
    }

    UINT4 GetKey()
    {
        return chargingID;
    }
};

//与C3G_R4_MCDR话单类对应的索引类。
class C3G_R4_MCDR_Key:public C3G_R4_CDR_Key
{
public:
    R4_Org_IMSI      servedIMSI;
public:
    C3G_R4_MCDR_Key(){};
    ~C3G_R4_MCDR_Key(){};

    BOOL IsMatch(CKey* pKey)
    {
        assert(pKey != NULL);

        C3G_R4_MCDR_Key* p = (C3G_R4_MCDR_Key*)pKey;
        return Match_IMSI(&(p->servedIMSI),&servedIMSI);
    }

    UINT4 GetKey()
    {
        UINT4 uHash = 0;
        char* p = (char*)&servedIMSI.value;

        for(int i = 0;i < servedIMSI.length;i++)
        {
            uHash = (uHash<<5) + uHash + *(p++);
        }

        return uHash;
    }
};


//比较两个GSN地址
BOOL Match_gsnAddress(R4_Org_GSNAddress* p1,R4_Org_GSNAddress* p2)
{
    assert(p1 != NULL);
    assert(p2 != NULL);
    
    //两张话单的地址类型相同
    if(p1->choice == p2->choice)
    {
        switch(p1->choice)
        {
        case R4_Org_iPBinaryAddress_chosen://比较二进制IP地址
            {
                if((p1->u.iPBinaryAddress.choice == R4_Org_iPBinV4Address_chosen)
                    && (p2->u.iPBinaryAddress.choice == R4_Org_iPBinV4Address_chosen))
                {
                    assert(p1->u.iPBinaryAddress.u.iPBinV4Address.length == 4);
                    assert(p2->u.iPBinaryAddress.u.iPBinV4Address.length == 4);
                    
                    return (memcmp(p1->u.iPBinaryAddress.u.iPBinV4Address.value,
                        p2->u.iPBinaryAddress.u.iPBinV4Address.value,
                        p1->u.iPBinaryAddress.u.iPBinV4Address.length)
                        == 0);
                }
                else if((p1->u.iPBinaryAddress.choice == R4_Org_iPBinV6Address_chosen)
                    && (p2->u.iPBinaryAddress.choice == R4_Org_iPBinV6Address_chosen))
                {
                    assert(p1->u.iPBinaryAddress.u.iPBinV6Address.length == 16);
                    assert(p2->u.iPBinaryAddress.u.iPBinV4Address.length == 16);
                    
                    return (memcmp(p1->u.iPBinaryAddress.u.iPBinV6Address.value,
                        p2->u.iPBinaryAddress.u.iPBinV6Address.value,
                        p1->u.iPBinaryAddress.u.iPBinV6Address.length)
                        == 0);
                }
                else
                {
                    assert(0);
                }
            }
            break;
        case R4_Org_iPTextRepresentedAddress_chosen://比较文本IP地址
            {
                if((p1->u.iPTextRepresentedAddress.choice == R4_Org_iPTextV4Address_chosen)
                    && (p2->u.iPTextRepresentedAddress.choice == R4_Org_iPTextV4Address_chosen))
                {
                    return (strcmp(p1->u.iPTextRepresentedAddress.u.iPTextV4Address,
                        p2->u.iPTextRepresentedAddress.u.iPTextV4Address)
                        == 0);
                }
                else if((p1->u.iPTextRepresentedAddress.choice == R4_Org_iPTextV6Address_chosen)
                    && (p2->u.iPTextRepresentedAddress.choice == R4_Org_iPTextV6Address_chosen))
                {
                    return (strcmp(p1->u.iPTextRepresentedAddress.u.iPTextV6Address,
                        p2->u.iPTextRepresentedAddress.u.iPTextV6Address)
                        == 0);
                }
                else
                {
                    assert(0);
                }
            }
            break;
        default://choice字段值非法
            assert(0);
            break;
        }
    }
    //两张话单的地址类型不同
    else
    {
        //二进制和文本格式的地址认为不同
    }
    
    return FALSE;
}

#endif
