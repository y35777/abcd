      
#ifndef     R99_BILL_H
#define     R99_BILL_H

#include "../asn1_r99/CDRF_CMCC_3GCDR.h"
#include "../asn1_r99/CDRF_CMCC_3GCDR_CG.h"
#include "../asn1_r99/oss_asn1_r99.h"
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

/*
*合并前话单类型。
*/
#define    S_CDR    CMCC_3GCDR_CallEventRecordType_sgsnPDPRecord
#define    G_CDR    CMCC_3GCDR_CallEventRecordType_ggsnPDPRecord


static BOOL Match_gsnAddress(CMCC_3GCDR_GSNAddress* p1, CMCC_3GCDR_GSNAddress* p2);

template<class T>
struct LINK_STRUCT1
{
	T Value;
	LINK_STRUCT1* m_pNext;

    static LINK_STRUCT1* Alloc()
	{
		LINK_STRUCT1* p = new LINK_STRUCT1;
		if(p == NULL)
		{
			throw "Alloc memory fail.";
		}

        memset(p, 0, sizeof(LINK_STRUCT1));
        
		return p;
    }
};

template<class T>
struct LINK_STRUCT2
{
	T* Value;
	unsigned int nCount;//Value数组的长度。

    LINK_STRUCT2* m_pNext;

	static LINK_STRUCT2* Alloc()
	{
		LINK_STRUCT2* p = new LINK_STRUCT2;
		if(p == NULL)
		{
			throw "Alloc memory fail.";
		}

		p->Value = NULL;
		p->nCount = 0;
		p->m_pNext = NULL;

		return p;
    }
};

template<class T1, class T2>
T1* List_To_Array1(T2* pHead, unsigned int& nLen)
{
	assert(pHead != NULL);

	unsigned int nCount = 0;
	T2* q = pHead;
	
	while(q)
	{
		nCount++;
		q = q->m_pNext;
	}

	nLen = nCount;
	if(nCount > 0)
	{
		int* p = new T1[nCount];
		if(p == NULL)
		{
			throw "Alloc memory fail.";
		}
            
		q = pHead;
		for(int i = 0;i < (int)nCount;i++)
		{
			p[i] = q->Value;
			q = q->m_pNext;
		}

		return p;
        
	}
    
    return NULL;
};

template<class T1, class T2>
T1* List_To_Array2(T2* pHead, unsigned int& nLen)
{
	assert(pHead != NULL);

	unsigned int nCount = 0;
	T2* q = pHead;
	while(q)
	{
		nCount += q->nCount;
		q = q->m_pNext;
	}

	nLen = nCount;
	if(nCount > 0)
	{
		T1* p = new T1[nCount];
		if(p == NULL)
		{
			throw "Alloc memory fail.";
		}

		q = pHead;
		nCount = 0;
		while(q)
		{
			memcpy(p + nCount,q->Value,
      		      (q->nCount * sizeof(T1)));
			nCount += q->nCount;
			q = q->m_pNext;
		}

		return p;
	}

	return NULL;
};

template<class T>
void Clear_List1(T* pHead)
{
	assert(pHead != NULL);

	T* q = pHead, *p = NULL;
	while(q)
	{
		p = q;
		q = q->m_pNext;
		delete p;
	}
};

template<class T>
void Clear_List2(T* pHead)
{
	assert(pHead != NULL);

	T* q = pHead, *p = NULL;
	while(q)
	{
		p = q;
		q = q->m_pNext;
		delete[] p->Value;
		delete p;
	}
};

/*
*CR99_BillParser继承自CBillParser类，管理中国移动3G R99话单类型和话单结构解析。
*/
class CR99_BillParser:public CBillParser
{
protected:
    CBill* m_pBill;

public:
    CR99_BillParser();
    virtual ~CR99_BillParser();

    virtual BOOL ParseARecord(char* pBuf,UINT4 uBufLen);//根据pBuf生成一个话单对象。
    virtual CBill* GetBillRecord();                     //返回话单记录对象。

    virtual UINT4 GetBillTypeCount();                   //返回话单类型数。
    virtual UINT4 GetBillTypeID(UINT4 uIndex);          //根据索引返回话单类型ID。
    virtual void RegisterBillType(CMerger* pMerger);    //在CMerger对象中注册话单类型。

	void GetBillTypeDesc(UINT4 uBillTypeID, char* szDesc);
};

/*
*CR99_CDR类封装了其他话单类使用的公共数据结构和函数。
*/
class CR99_CDR:public CBill
{
protected:
    //用作合并SGSNAddress字段的链表结构。
    //m_pSGSNAddress指向链表头的指针，
    //m_pSGSNAddressTail指向链表尾的指针。
	LINK_STRUCT2<CMCC_3GCDR_GSNAddress> *m_pSGSNAddress, *m_pSGSNAddressTail;

    //链表的长度(元素数)
    unsigned int m_nLen_SGSNAddress_List;

    //用作合并listOfTrafficVolumes字段的链表结构。
    //m_pCondition指向链表头的指针，
    //m_pConditionTail指向链表尾的指针。
	LINK_STRUCT2<CMCC_3GCDR_ChangeOfCharCondition> *m_pCondition,*m_pConditionTail;

    //链表的长度(元素数)
    unsigned int m_nLen_Condition_List;
    
    //用作合并recordExtensions字段的链表结构。
    //m_pExtension指向链表头的指针，
    //m_pExtensionTail指向链表尾的指针。
	LINK_STRUCT2<CMCC_3GCDR_ManagementExtension> *m_pExtension,*m_pExtensionTail;

    //链表的长度(元素数)
    unsigned int m_nLen_Extension_List;

    //用作合并recordSequenceNumber字段的链表结构。
    //m_pSequence指向链表头的指针，
    //m_pSequenceTail指向链表尾的指针。
	LINK_STRUCT1<int> *m_pSequence,*m_pSequenceTail;

	//用作合并LocalSequenceNumber字段的链表结构。
    //m_pLocalSequenceNumber指向链表头的指针，
    //m_pLocalSequenceNumberTail指向链表尾的指针。
	LINK_STRUCT1<int> *m_pLocalSequenceNumber,*m_pLocalSequenceNumberTail;

	//用作合并rNCUnsentDownlinkVolume字段的链表结构。
    //m_pRNCUnsentDownlinkVolume指向链表头的指针，
    //m_pRNCUnsentDownlinkVolumeTail指向链表尾的指针。
	LINK_STRUCT1<int> *m_pRNCUnsentDownlinkVolume,*m_pRNCUnsentDownlinkVolumeTail;

    //用作合并changeLocation字段的链表结构。
    //m_pLocation指向链表头的指针，
    //m_pLocationTail指向链表尾的指针。
	LINK_STRUCT2<CMCC_3GCDR_ChangeLocation> *m_pLocation, *m_pLocationTail;

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
    CR99_CDR()
    {
        m_pSGSNAddress = NULL;
        m_pSGSNAddressTail = NULL;

        m_pCondition = NULL;
        m_pConditionTail = NULL;

        m_pExtension = NULL;
        m_pExtensionTail = NULL;

        m_pSequence = NULL;
        m_pSequenceTail = NULL;

		m_pLocalSequenceNumber = NULL;
		m_pLocalSequenceNumberTail = NULL;

		m_pRNCUnsentDownlinkVolume = NULL;
		m_pRNCUnsentDownlinkVolumeTail = NULL;

        m_pLocation = NULL;
        m_pLocationTail = NULL;

        /*
         * 用于进行合并流量控制的成员变量。
         * 
         */
        m_nLen_SGSNAddress_List = 0;
        m_nLen_Condition_List = 0;
        m_nLen_Extension_List = 0;
        m_nLen_Location_List = 0;
    };

    ~CR99_CDR()
    {
        if(m_pSGSNAddress != NULL)
        {
            Clear_List2(m_pSGSNAddress);
            m_pSGSNAddress = NULL;
        }

        if(m_pCondition != NULL)
        {
            Clear_List2(m_pCondition);
            m_pCondition = NULL;
        }

        if(m_pExtension != NULL)
        {
            Clear_List2(m_pExtension);
            m_pExtension = NULL;
        }

        if(m_pSequence != NULL)
        {
            Clear_List1(m_pSequence);
            m_pSequence = NULL;
        }

		if(m_pLocalSequenceNumber != NULL)
        {
            Clear_List1(m_pLocalSequenceNumber);
            m_pLocalSequenceNumber = NULL;
        }

		if(m_pRNCUnsentDownlinkVolume != NULL)
        {
            Clear_List1(m_pRNCUnsentDownlinkVolume);
            m_pRNCUnsentDownlinkVolume = NULL;
        }

        if(m_pLocation != NULL)
        {
            Clear_List2(m_pLocation);
            m_pLocation = NULL;
        }
    };

protected:

    
    /******************************************************
    * 函数名:   Clear_Diagnostics
    * 作者:    GGF
    * 日期:    2003-09-28
    * 描述:    删除CMCC_3GCDR_Diagnostics结构中的指针成员。
    * 输入: 
    *       参数1 :CMCC_3GCDR_Diagnostics* p
    * 返回值: void 
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *
    ******************************************************/
    void Clear_Diagnostics(CMCC_3GCDR_Diagnostics* p)
    {
        assert(p != NULL);

        if(p->choice == CMCC_3GCDR_networkSpecificCause_chosen)
        {
            delete[] p->u.networkSpecificCause.identifier.value;
            delete[] p->u.networkSpecificCause.information.value;
        }
        else if(p->choice == CMCC_3GCDR_manufacturerSpecificCause_chosen)
        {
            delete[] p->u.manufacturerSpecificCause.identifier.value;
            delete[] p->u.manufacturerSpecificCause.information.value;
        }
    }

    /******************************************************
    * 函数名:   Clone_ManagementExtension
    * 作者:    GGF
    * 日期:    2003-09-28
    * 描述:    “克隆”一个CMCC_3GCDR_ManagementExtension结构数组。
    * 输入: 
    *       参数1 :CMCC_3GCDR_ManagementExtension* p,数组指针
    *       参数2 :unsigned int nCount,数组的元素数
    * 返回值: CMCC_3GCDR_ManagementExtension*,克隆的结果
    *       若内存操作失败，返回NULL。
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *
    ******************************************************/
    CMCC_3GCDR_ManagementExtension* Clone_ManagementExtension(
        CMCC_3GCDR_ManagementExtension* p,unsigned int nCount)
    {
        assert(p != NULL);
        assert(nCount > 0);

        CMCC_3GCDR_ManagementExtension* q
            = new CMCC_3GCDR_ManagementExtension[nCount];
        if(q == NULL)
        {
            throw "Alloc memory fail.";
        }

        memcpy(q,p,nCount * sizeof(CMCC_3GCDR_ManagementExtension));
        unsigned char* tmp = NULL;

        try
        {
            for(int i = 0;i < (int)nCount;i++)
            {
                tmp = new unsigned char[p[i].identifier.length];
                if(tmp == NULL)
                {
                    throw i;
                }

                memcpy(tmp,p[i].identifier.value,p[i].identifier.length);
                q[i].identifier.value = tmp;

                tmp = new unsigned char[p[i].information.length];
                if(tmp == NULL)
                {
                    throw -i;
                }

                memcpy(tmp,p[i].information.value,p[i].information.length);
                q[i].information.value = tmp;
            }
            return q;
        }
        catch(int nErrCode)
        {
            //若nErrCode > 0,说明复制identifier失败。
            //若nErrCode < 0,说明复制information失败。
            int nLen = (nErrCode >= 0)?(nErrCode):(-nErrCode),i = 0;

            for(i = 0;i < nLen;i++)
            {
                delete[] q[i].identifier.value;
                delete[] q[i].information.value;
            }
            if(nErrCode < 0)
            {
                delete[] q[i].identifier.value;
            }
            delete[] q;
            q = NULL;
        }
        
        throw "Alloc memory fail.";
        return NULL;
    }

    /******************************************************
    * 函数名:   Clear_ManagementExtension
    * 作者:    GGF
    * 日期:    2003-09-28
    * 描述:    删除结构CMCC_3GCDR_ManagementExtension的内存空间。
    * 输入: 
    *       参数1 :CMCC_3GCDR_ManagementExtension* p,结构数组。
    *       参数2 :unsigned int nCount,p数组的元素个数。
    * 返回值: void* 
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *
    ******************************************************/
    void Clear_ManagementExtension(CMCC_3GCDR_ManagementExtension* p,
        unsigned int nCount)
    {
        assert(p != NULL);
        assert(nCount > 0);

        for(unsigned int i = 0;i < nCount;i++)
        {
            delete[] p[i].identifier.value;
            delete[] p[i].information.value;
        }
        delete[] p;
    }

    /******************************************************
    * 函数名:   Clone_ChangeOfCharCondition
    * 作者:    GGF
    * 日期:    2003-09-28
    * 描述:    “克隆”一个CMCC_3GCDR_ChangeOfCharCondition结构数组。
    * 输入: 
    *       参数1 :CMCC_3GCDR_ChangeOfCharCondition* p,数组指针
    *       参数2 :unsigned int nCount,数组的元素数
    * 返回值: CMCC_3GCDR_ChangeOfCharCondition*,克隆的结果
    *       若内存操作失败，返回NULL。
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *
    ******************************************************/
    CMCC_3GCDR_ChangeOfCharCondition* Clone_ChangeOfCharCondition(
        CMCC_3GCDR_ChangeOfCharCondition* p,unsigned int nCount)
    {
        assert(p != NULL);
        assert(nCount > 0);

        CMCC_3GCDR_ChangeOfCharCondition* q = new CMCC_3GCDR_ChangeOfCharCondition[nCount];
        if(q == NULL)
        {
            throw "Alloc memory fail.";
        }

        memcpy(q,p,nCount * sizeof(CMCC_3GCDR_ChangeOfCharCondition));
        return q;
    }

    /******************************************************
    * 函数名:   Clone_GSNAddress
    * 作者:    GGF
    * 日期:    2003-09-28
    * 描述:    “克隆”一个CMCC_3GCDR_GSNAddress结构数组。
    * 输入: 
    *       参数1 :CMCC_3GCDR_GSNAddress* p,数组头指针
    *       参数2 :unsigned int nCount,数组元素数
    * 返回值: CMCC_3GCDR_GSNAddress*,克隆的结果
    *       若内存操作失败，抛出异常。
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *
    ******************************************************/
    CMCC_3GCDR_GSNAddress* Clone_GSNAddress(
        CMCC_3GCDR_GSNAddress* p,unsigned int nCount)
    {
        assert(p != NULL);
        assert(nCount > 0);

        CMCC_3GCDR_GSNAddress* q = new CMCC_3GCDR_GSNAddress[nCount];
        if(q == NULL)
        {
            throw "Alloc memory fail.";
        }

        memcpy(q,p,nCount * sizeof(CMCC_3GCDR_GSNAddress));
        return q;
    }

    /******************************************************
    * 函数名:   Clone_GSNAddress
    * 作者:    GGF
    * 日期:    2003-09-28
    * 描述:    “克隆”一个CMCC_3GCDR_GSNAddress结构数组。
    * 输入: 
    *       参数1 :CMCC_3GCDR_GSNAddress* pDiff,剔除重复的SGSN地址的参照
    *       参数2 :CMCC_3GCDR_GSNAddress* p,数组头指针
    *       参数3 :unsigned int nCount,数组元素数,并返回实际复制的
    *              元素数.
    * 返回值: CMCC_3GCDR_GSNAddress*,克隆的结果,已经剔除了重复的SGSN地址.
    *       若p中的所有元素都与pDiff重复,则返回空.
    *       若内存操作失败,抛出异常.
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *
    ******************************************************/
    CMCC_3GCDR_GSNAddress* Clone_GSNAddress(
        CMCC_3GCDR_GSNAddress* pDiff,
        CMCC_3GCDR_GSNAddress* p,unsigned int& nCount)
    {
        assert(p != NULL);
        assert(nCount > 0);

        CMCC_3GCDR_GSNAddress* pStartPos = p;
        int nRemains = nCount;

        while(nRemains > 0)
        {
            if(Match_gsnAddress(pDiff,pStartPos))
            {
                pStartPos++;
                nRemains--;
            }
            else
                break;
        }

        if(nRemains == 0)
        {
            nCount = 0;
            return NULL;
        }

        //克隆余下的部分
        CMCC_3GCDR_GSNAddress* q = new CMCC_3GCDR_GSNAddress[nRemains];
        if(q == NULL)
        {
            throw "Alloc memory fail.";
        }

        memcpy(q,pStartPos,nRemains * sizeof(CMCC_3GCDR_GSNAddress));
        nCount = nRemains;

        return q;
    }

    /******************************************************
    * 函数名:   Clone_ChangeLocation
    * 作者:    GGF
    * 日期:    2003-09-28
    * 描述:    “克隆”一个CMCC_3GCDR_ChangeLocation结构数组。
    * 输入: 
    *       参数1 :CMCC_3GCDR_ChangeLocation* p,数组头指针
    *       参数2 :unsigned int nCount,数组元素数
    * 返回值: CMCC_3GCDR_ChangeLocation*,克隆的结果
    *       若内存操作失败，抛出异常。
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *
    ******************************************************/
    CMCC_3GCDR_ChangeLocation* Clone_ChangeLocation(
        CMCC_3GCDR_ChangeLocation* p,unsigned int nCount)
    {
        assert(p != NULL);
        assert(nCount > 0);

        CMCC_3GCDR_ChangeLocation* q = new CMCC_3GCDR_ChangeLocation[nCount];
        if(q == NULL)
        {
            throw "Alloc memory fail.";
        }

        memcpy(q,p,nCount * sizeof(CMCC_3GCDR_ChangeLocation));
        return q;
    }


	//计算两个时间之差。
	/* 说明：函数基于以下考虑：如果t2时区为zz2，t1时区为zz1，
	 * 那么t2相当于格林尼治时间t2-zz2，t1则相当于格林尼治时间t1-zz1，
	 * 因此时间差为(t2-zz2)-(t1-zz1)，也就是t2-t1+zz1-zz2
	 */
    int Get_Duration(CMCC_3GCDR_TimeStamp* t1, CMCC_3GCDR_TimeStamp* t2)
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
class CR99_SCDR:public CR99_CDR
{
protected:
    PackID_t m_nMinRecordSequence;          //当前合并结果中最小的序列号。
    PackID_t m_nCurRecordSequence;          //当前完成合并的最大序列号。
    BOOL m_bPDPCompleted;                   //PDP是否结束标志。
    BOOL m_bPrepared;                       //标志是否为合并准备过数据。

protected:
    CMCC_3GCDR_CallDuration  m_Duration;         //用于累加通话时长字段。
    CMCC_3GCDR_DataVolumeGPRS m_UnsentVolume;    //用于累加rNCUnsentDownlinkVolume字段。
    CMCC_3GCDR_Diagnostics m_Diagnostics;        //用于存放最后一张话单的diagnostics字段。
    CMCC_3GCDR_CauseForRecClosing m_CauseForRecClosing;
                                            //用于存放最后一张话单的causeForRecClosing字段。
     
	CMCC_3GCDR_CAMELInformationPDP m_CAMELInformationPDP;
                                            //用于合并话单的CAMELInformationPDP字段

    typedef struct SGSN_DATA1 {

        CMCC_3GCDR_CG_GSNAddress sgsnAddress;
    	unsigned int nCount;    //pList链表的长度。
        LINK_STRUCT1<int>        *pList;  
        
        SGSN_DATA1()
	    {
            memset(this, 0, sizeof(*this));        
		    return;
        }

        ~SGSN_DATA1()
        {
            if(pList != 0)
            {
                Clear_List1(pList);
                pList = 0;
            }
        }

    } SGSN_DATA1;

    LINK_STRUCT1<SGSN_DATA1> *m_pRecordSequenceList, *m_pRecordSequenceListTail;
    unsigned int m_nLen_RecordSequence_List;

    LINK_STRUCT1<SGSN_DATA1> *m_pLocalSequenceList,  *m_pLocalSequenceListTail;
    unsigned int m_nLen_LocalSequence_List;

    LINK_STRUCT1<SGSN_DATA1> *m_pRNCDownlinkVolumeList, *m_pRNCDownlinkVolumeListTail;
    unsigned int m_nLen_RNCDownlinkVolume_List;
	
public:
    CMCC_3GCDR_SGSNPDPRecord* m_pOrgRec;         //原始话单结构。
    CMCC_3GCDR_CG_CallEventRecord* m_pDestRec;     //合并后话单结构。

    CR99_SCDR();
    ~CR99_SCDR();

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
class CR99_GCDR:public CR99_CDR
{
protected:
    PackID_t m_nMinRecordSequence;          //当前合并结果中最小的序列号。
    PackID_t m_nCurRecordSequence;          //当前合并结果中最大的序列号。
    BOOL m_bPDPCompleted;                   //PDP是否结束标志。
    BOOL m_bPrepared;                       //标志是否为合并准备过数据。

protected:
    CMCC_3GCDR_CallDuration  m_Duration;         //用于累加通话时长字段。
    CMCC_3GCDR_Diagnostics m_Diagnostics;        //用于存放最后一张话单的diagnostics字段。
    CMCC_3GCDR_CauseForRecClosing m_CauseForRecClosing;
                                            //用于存放最后一张话单的causeForRecClosing字段。

public:
    CMCC_3GCDR_GGSNPDPRecord* m_pOrgRec;         //原始话单结构。
    CMCC_3GCDR_CG_CallEventRecord* m_pDestRec;     //合并后话单结构。

    CR99_GCDR();
    ~CR99_GCDR();

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
/*
class CR99_MCDR:public CR99_CDR
{
protected:
    PackID_t m_nMinRecordSequence;          //当前合并结果中最小的序列号。
    PackID_t m_nCurRecordSequence;          //当前合并结果中最大的序列号。
    BOOL m_bPDPCompleted;                   //PDP是否结束标志。
    BOOL m_bPrepared;                       //标志是否为合并准备过数据。

protected:
    CMCC_3GCDR_CallDuration  m_Duration;         //用于累加通话时长字段。
    CMCC_3GCDR_Diagnostics m_Diagnostics;        //用于存放最后一张话单的diagnostics字段。
    CMCC_3GCDR_CauseForRecClosing m_CauseForRecClosing;
                                            //用于存放最后一张话单的causeForRecClosing字段。
    
	CMCC_3GCDR_CAMELInformationMM m_CAMELInformationMM;
                                            //用于存放合并话单的CAMELInforamtionMM字段。

public:
    CMCC_3GCDR_SGSNMMRecord* m_pOrgRec;          //原始话单结构。
    CMCC_3GCDR_CG_CallEventRecord* m_pDestRec;     //合并后话单结构。

    CR99_MCDR();
    ~CR99_MCDR();

    virtual BOOL Init(char* pBuf,UINT4 uLen); //使用话单记录初始化话单对象。

    virtual BOOL CanSafeMerge(CBill* pBill);//判断当前话单是否允许与pBill合并。
    virtual BOOL Merge(CBill* pBill);       //把pBill合入当前话单中。
    virtual BOOL IsPDPCompleted();          //判断PDP已经结束。
    virtual BOOL NeedWaiting();             //判断是否需要继续等待后续话单。

    virtual char* GetContents();            //返回话单的记录结构。

    virtual BOOL IsPartial();               //判断是否部分话单。
    virtual BOOL IsFirstPartialRec();       //判断是否PDP的第一张部分话单。
    virtual BOOL IsLastPartialRec();        //判断是否PDP的末张部分话单。

    virtual UINT4 GetBillType();            //返回话单类型ID值。

    virtual CKey CreateKey();               //创建索引对象。

    BOOL PrepareData();                     //为合并准备数据。
};
*/
class CR99_CDR_Key:public CKey
{
public:
    CR99_CDR_Key(){};
    ~CR99_CDR_Key(){};

    //比较两个IMSI地址
    BOOL Match_IMSI(CMCC_3GCDR_IMSI* p1, CMCC_3GCDR_IMSI* p2)
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

//与CR99_SCDR话单类对应的索引类。
class CR99_SCDR_Key:public CR99_CDR_Key
{
public:
    //CMCC_3GCDR_GSNAddress sgsnAddress;
    CMCC_3GCDR_GSNAddress ggsnAddressUsed;
    CMCC_3GCDR_ChargingID chargingID;
public:
    CR99_SCDR_Key(){};
    ~CR99_SCDR_Key(){};

    BOOL IsMatch(CKey* pKey)
    {
        assert(pKey != NULL);

        CR99_SCDR_Key* p = (CR99_SCDR_Key*)pKey;
        return (Match_gsnAddress(&(p->ggsnAddressUsed),&ggsnAddressUsed)
             && (p->chargingID == chargingID));
    }

    UINT4 GetKey()
    {
        return chargingID;
    }
};

//与CR99_GCDR话单类对应的索引类。
class CR99_GCDR_Key:public CR99_CDR_Key
{
public:
    CMCC_3GCDR_GSNAddress ggsnAddress;
    CMCC_3GCDR_ChargingID chargingID;
public:
    CR99_GCDR_Key(){};
    ~CR99_GCDR_Key(){};

    BOOL IsMatch(CKey* pKey)
    {
        assert(pKey != NULL);

        CR99_GCDR_Key* p = (CR99_GCDR_Key*)pKey;
        return (Match_gsnAddress(&(p->ggsnAddress),&ggsnAddress)
            && (p->chargingID == chargingID));
    }

    UINT4 GetKey()
    {
        return chargingID;
    }
};
/*
//与CR99_MCDR话单类对应的索引类。
class CR99_MCDR_Key:public CUpdatedR99_CDR_Key
{
public:
    CMCC_3GCDR_IMSI      servedIMSI;
public:
    CR99_MCDR_Key(){};
    ~CR99_MCDR_Key(){};

    BOOL IsMatch(CKey* pKey)
    {
        assert(pKey != NULL);

        CR99_MCDR_Key* p = (CR99_MCDR_Key*)pKey;
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
*/

//比较两个GSN地址
BOOL Match_gsnAddress(CMCC_3GCDR_GSNAddress* p1, CMCC_3GCDR_GSNAddress* p2)
{
    assert(p1 != NULL);
    assert(p2 != NULL);
    
    //两张话单的地址类型相同
    if(p1->choice == p2->choice)
    {
        switch(p1->choice)
        {
        case CMCC_3GCDR_iPBinaryAddress_chosen://比较二进制IP地址
            {
                if((p1->u.iPBinaryAddress.choice == CMCC_3GCDR_iPBinV4Address_chosen)
                    && (p2->u.iPBinaryAddress.choice == CMCC_3GCDR_iPBinV4Address_chosen))
                {
                    return (memcmp(p1->u.iPBinaryAddress.u.iPBinV4Address.value,
                        p2->u.iPBinaryAddress.u.iPBinV4Address.value,
                        p1->u.iPBinaryAddress.u.iPBinV4Address.length)
                        == 0);
                }
                else if((p1->u.iPBinaryAddress.choice == CMCC_3GCDR_iPBinV6Address_chosen)
                    && (p2->u.iPBinaryAddress.choice == CMCC_3GCDR_iPBinV6Address_chosen))
                {
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
        case CMCC_3GCDR_iPTextRepresentedAddress_chosen://比较文本IP地址
            {
                if((p1->u.iPTextRepresentedAddress.choice == CMCC_3GCDR_iPTextV4Address_chosen)
                    && (p2->u.iPTextRepresentedAddress.choice == CMCC_3GCDR_iPTextV4Address_chosen))
                {
                    return (strcmp(p1->u.iPTextRepresentedAddress.u.iPTextV4Address,
                        p2->u.iPTextRepresentedAddress.u.iPTextV4Address)
                        == 0);
                }
                else if((p1->u.iPTextRepresentedAddress.choice == CMCC_3GCDR_iPTextV6Address_chosen)
                    && (p2->u.iPTextRepresentedAddress.choice == CMCC_3GCDR_iPTextV6Address_chosen))
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
