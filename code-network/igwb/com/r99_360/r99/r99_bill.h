
#ifndef     _3G_BILL_H
#define     _3G_BILL_H

#include "../asn1_r99/CDRF_3GCDR.h"
#include "../asn1_r99/CDRF_3GCDR_CG.h"
#include "../../merge/bill.h"
#include <assert.h>

/*
*最大允许合并的话单数。
*/
#define MAX_MERGED_BILLS    255

/*
*合并前话单类型。
*/
#define    S_CDR    CDR3G_CallEventRecordType_sgsnPDPRecord
#define    G_CDR    CDR3G_CallEventRecordType_ggsnPDPRecord
#define    M_CDR    CDR3G_CallEventRecordType_sgsnMMRecord
#define    SMO_CDR  CDR3G_CallEventRecordType_sgsnSMORecord
#define    SMT_CDR  CDR3G_CallEventRecordType_sgsnSMTRecord

static BOOL Match_gsnAddress(CDR3G_GSNAddress* p1,CDR3G_GSNAddress* p2);

/*
*C3G_BillParser继承自CBillParser类，管理3G话单类型和话单结构解析。
*/
class C3G_BillParser:public CBillParser
{
protected:
    CBill* m_pBill;

public:
    C3G_BillParser();
    virtual ~C3G_BillParser();

    virtual BOOL ParseARecord(char* pBuf,UINT4 uBufLen);//根据pBuf生成一个话单对象。
    virtual CBill* GetBillRecord();                     //返回话单记录对象。

    virtual UINT4 GetBillTypeCount();                   //返回话单类型数。
    virtual UINT4 GetBillTypeID(UINT4 uIndex);          //根据索引返回话单类型ID。
    virtual void RegisterBillType(CMerger* pMerger);    //在CMerger对象中注册话单类型。

	//颜文远2002-03-06添加
	//获得指定的话单类型ID的话单类型描述
	void GetBillTypeDesc(UINT4 uBillTypeID, char* szDesc);
	//添加结束，颜文远2002-03-06
};

/*
*C3G_350_CDR类封装了其他话单类使用的公共数据结构和函数。
*/
class C3G_350_CDR:public CBill
{
protected:

     //达到门限标志
     BOOL m_bIsLimit;
    //用作合并SGSNAddress字段的链表结构。
    //m_pSGSNAddress指向链表头的指针，
    //m_pSGSNAddressTail指向链表尾的指针。
    struct SGSGAddress_LINK_STRUCT
    {
        CDR3G_GSNAddress* Value;
        unsigned int nCount;//Value数组的长度。

        SGSGAddress_LINK_STRUCT* m_pNext;

        static SGSGAddress_LINK_STRUCT* Alloc()
        {
            SGSGAddress_LINK_STRUCT* p = new
                SGSGAddress_LINK_STRUCT;
            if(p == NULL)
            {
                throw "Alloc memory fail.";
            }

            p->Value = NULL;
            p->nCount = 0;
            p->m_pNext = NULL;

            return p;
        }
    }*m_pSGSNAddress,*m_pSGSNAddressTail;

    //链表的长度(元素数)
    unsigned int m_nLen_SGSNAddress_List;

    //用作合并listOfTrafficVolumes字段的链表结构。
    //m_pCondition指向链表头的指针，
    //m_pConditionTail指向链表尾的指针。
    struct ChangeOfCharCondition_LINK_STRUCT
    {
        CDR3G_ChangeOfCharCondition* Value;
        unsigned int nCount;//Value数组的长度。

        ChangeOfCharCondition_LINK_STRUCT* m_pNext;

        static ChangeOfCharCondition_LINK_STRUCT* Alloc()
        {
            ChangeOfCharCondition_LINK_STRUCT* p = new
                ChangeOfCharCondition_LINK_STRUCT;
            if(p == NULL)
            {
                throw "Alloc memory fail.";
            }

            p->Value = NULL;
            p->nCount = 0;
            p->m_pNext = NULL;

            return p;
        }
    }*m_pCondition,*m_pConditionTail;

    //链表的长度(元素数)
    unsigned int m_nLen_Condition_List;
    
    //用作合并recordExtensions字段的链表结构。
    //m_pExtension指向链表头的指针，
    //m_pExtensionTail指向链表尾的指针。
    struct ManagementExtension_LINK_STRUCT
    {
        CDR3G_ManagementExtension* Value;
        unsigned int nCount;//Value数组的长度。

        ManagementExtension_LINK_STRUCT* m_pNext;

        static ManagementExtension_LINK_STRUCT* Alloc()
        {
            ManagementExtension_LINK_STRUCT* p = new
                ManagementExtension_LINK_STRUCT;
            if(p == NULL)
            {
                throw "Alloc memory fail.";
            }

            p->Value = NULL;
            p->nCount = 0;
            p->m_pNext = NULL;

            return p;
        }
    }*m_pExtension,*m_pExtensionTail;

    //链表的长度(元素数)
    unsigned int m_nLen_Extension_List;

    //用作合并recordSequenceNumber字段的链表结构。
    //m_pSequence指向链表头的指针，
    //m_pSequenceTail指向链表尾的指针。
    struct RecordSequenceNumber_LINK_STRUCT
    {
        int Value;
        RecordSequenceNumber_LINK_STRUCT* m_pNext;

        static RecordSequenceNumber_LINK_STRUCT* Alloc()
        {
            RecordSequenceNumber_LINK_STRUCT* p = new
                RecordSequenceNumber_LINK_STRUCT;
            if(p == NULL)
            {
                throw "Alloc memory fail.";
            }

            p->Value = 0;
            p->m_pNext = NULL;
            return p;
        }
    }*m_pSequence,*m_pSequenceTail;

    //用作合并changeLocation字段的链表结构。
    //m_pLocation指向链表头的指针，
    //m_pLocationTail指向链表尾的指针。
    struct ChangeLocation_LINK_STRUCT
    {
        CDR3G_ChangeLocation* Value;
        unsigned int nCount;

        ChangeLocation_LINK_STRUCT* m_pNext;

        static ChangeLocation_LINK_STRUCT* Alloc()
        {
            ChangeLocation_LINK_STRUCT* p = new 
                ChangeLocation_LINK_STRUCT;
            if(p == NULL)
            {
                throw "Alloc memory fail.";
            }

            p->Value = NULL;
            p->m_pNext = NULL;
            p->nCount = 0;
            return p;
        }
    }*m_pLocation,*m_pLocationTail;

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
    C3G_350_CDR()
    {
        m_pSGSNAddress = NULL;
        m_pSGSNAddressTail = NULL;

        m_pCondition = NULL;
        m_pConditionTail = NULL;

        m_pExtension = NULL;
        m_pExtensionTail = NULL;

        m_pSequence = NULL;
        m_pSequenceTail = NULL;

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

    ~C3G_350_CDR()
    {
        if(m_pSGSNAddress != NULL)
        {
            Clear_List(m_pSGSNAddress);
            m_pSGSNAddress = NULL;
        }

        if(m_pCondition != NULL)
        {
            Clear_List(m_pCondition);
            m_pCondition = NULL;
        }

        if(m_pExtension != NULL)
        {
            Clear_List(m_pExtension);
            m_pExtension = NULL;
        }

        if(m_pSequence != NULL)
        {
            Clear_List(m_pSequence);
            m_pSequence = NULL;
        }

        if(m_pLocation != NULL)
        {
            Clear_List(m_pLocation);
            m_pLocation = NULL;
        }
    };

protected:

    
    /******************************************************
    * 函数名:   Clear_Diagnostics
    * 作者:    Zhou Yonghui
    * 日期:    01-6-8
    * 描述:    删除CDR3G_Diagnostics结构中的指针成员。
    * 输入: 
    *       参数1 :CDR3G_Diagnostics* p
    * 返回值: void 
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *01-6-8        Zhou Yonghui
    ******************************************************/
    void Clear_Diagnostics(CDR3G_Diagnostics* p)
    {
        assert(p != NULL);

        if(p->choice == CDR3G_networkSpecificCause_chosen)
        {
            delete[] p->u.networkSpecificCause.identifier.value;
            delete[] p->u.networkSpecificCause.information.value;
        }
        else if(p->choice == CDR3G_manufacturerSpecificCause_chosen)
        {
            delete[] p->u.manufacturerSpecificCause.identifier.value;
            delete[] p->u.manufacturerSpecificCause.information.value;
        }
    }

    /******************************************************
    * 函数名:   Clone_CDR3G_ManagementExtension
    * 作者:    Zhou Yonghui
    * 日期:    01-5-30
    * 描述:    “克隆”一个CDR3G_ManagementExtension结构数组。
    * 输入: 
    *       参数1 :DR3G_ManagementExtension* p,数组指针
    *       参数2 :unsigned int nCount,数组的元素数
    * 返回值: CDR3G_ManagementExtension*,克隆的结果
    *       若内存操作失败，返回NULL。
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *01-5-30       Zhou Yonghui
    ******************************************************/
    CDR3G_ManagementExtension* Clone_CDR3G_ManagementExtension(
        CDR3G_ManagementExtension* p,unsigned int nCount)
    {
        assert(p != NULL);
        assert(nCount > 0);

        CDR3G_ManagementExtension* q
            = new CDR3G_ManagementExtension[nCount];
        if(q == NULL)
        {
            throw "Alloc memory fail.";
        }

        memcpy(q,p,nCount * sizeof(CDR3G_ManagementExtension));
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
    * 作者:    Zhou Yonghui
    * 日期:    01-6-8
    * 描述:    删除结构CDR3G_ManagementExtension的内存空间。
    * 输入: 
    *       参数1 :CDR3G_ManagementExtension* p,结构数组。
    *       参数2 :unsigned int nCount,p数组的元素个数。
    * 返回值: void* 
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *01-6-8        Zhou Yonghui
    ******************************************************/
    void Clear_ManagementExtension(CDR3G_ManagementExtension* p,
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
    * 函数名:   Clone_CDR3G_ChangeOfCharCondition
    * 作者:    Zhou Yonghui
    * 日期:    01-5-30
    * 描述:    “克隆”一个CDR3G_ManagementExtension结构数组。
    * 输入: 
    *       参数1 :DR3G_ChangeOfCharCondition* p,数组指针
    *       参数2 :unsigned int nCount,数组的元素数
    * 返回值: CDR3G_ChangeOfCharCondition*,克隆的结果
    *       若内存操作失败，返回NULL。
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *01-5-30       Zhou Yonghui
    ******************************************************/
    CDR3G_ChangeOfCharCondition* Clone_CDR3G_ChangeOfCharCondition(
        CDR3G_ChangeOfCharCondition* p,unsigned int nCount)
    {
        assert(p != NULL);
        assert(nCount > 0);

        CDR3G_ChangeOfCharCondition* q = new CDR3G_ChangeOfCharCondition[nCount];
        if(q == NULL)
        {
            throw "Alloc memory fail.";
        }

        memcpy(q,p,nCount * sizeof(CDR3G_ChangeOfCharCondition));
        return q;
    }

    /******************************************************
    * 函数名:   Clone_CDR3G_GSNAddress
    * 作者:    Zhou Yonghui
    * 日期:    01-5-30
    * 描述:    “克隆”一个CDR3G_GSNAddress结构数组。
    * 输入: 
    *       参数1 :DR3G_GSNAddress* p,数组头指针
    *       参数2 :unsigned int nCount,数组元素数
    * 返回值: CDR3G_GSNAddress*,克隆的结果
    *       若内存操作失败，抛出异常。
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *01-5-30       Zhou Yonghui
    ******************************************************/
    CDR3G_GSNAddress* Clone_CDR3G_GSNAddress(
        CDR3G_GSNAddress* p,unsigned int nCount)
    {
        assert(p != NULL);
        assert(nCount > 0);

        CDR3G_GSNAddress* q = new CDR3G_GSNAddress[nCount];
        if(q == NULL)
        {
            throw "Alloc memory fail.";
        }

        memcpy(q,p,nCount * sizeof(CDR3G_GSNAddress));
        return q;
    }

    /******************************************************
    * 函数名:   Clone_CDR3G_GSNAddress
    * 作者:    Zhou Yonghui
    * 日期:    01-5-30
    * 描述:    “克隆”一个CDR3G_GSNAddress结构数组。
    * 输入: 
    *       参数1 :DR3G_GSNAddress* pDiff,剔除重复的SGSN地址的参照
    *       参数2 :DR3G_GSNAddress* p,数组头指针
    *       参数3 :unsigned int nCount,数组元素数,并返回实际复制的
    *              元素数.
    * 返回值: CDR3G_GSNAddress*,克隆的结果,已经剔除了重复的SGSN地址.
    *       若p中的所有元素都与pDiff重复,则返回空.
    *       若内存操作失败,抛出异常.
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *01-5-30       Zhou Yonghui
    ******************************************************/
    CDR3G_GSNAddress* Clone_CDR3G_GSNAddress(
        CDR3G_GSNAddress* pDiff,
        CDR3G_GSNAddress* p,unsigned int& nCount)
    {
        assert(p != NULL);
        assert(nCount > 0);

        CDR3G_GSNAddress* pStartPos = p;
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
        CDR3G_GSNAddress* q = new CDR3G_GSNAddress[nRemains];
        if(q == NULL)
        {
            throw "Alloc memory fail.";
        }

        memcpy(q,pStartPos,nRemains * sizeof(CDR3G_GSNAddress));
        nCount = nRemains;

        return q;
    }

    /******************************************************
    * 函数名:   Clone_CDR3G_ChangeLocation
    * 作者:    Zhou Yonghui
    * 日期:    01-5-30
    * 描述:    “克隆”一个CDR3G_ChangeLocation结构数组。
    * 输入: 
    *       参数1 :CDR3G_ChangeLocation* p,数组头指针
    *       参数2 :unsigned int nCount,数组元素数
    * 返回值: CDR3G_ChangeLocation*,克隆的结果
    *       若内存操作失败，抛出异常。
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *01-5-30       Zhou Yonghui
    ******************************************************/
    CDR3G_ChangeLocation* Clone_CDR3G_ChangeLocation(
        CDR3G_ChangeLocation* p,unsigned int nCount)
    {
        assert(p != NULL);
        assert(nCount > 0);

        CDR3G_ChangeLocation* q = new CDR3G_ChangeLocation[nCount];
        if(q == NULL)
        {
            throw "Alloc memory fail.";
        }

        memcpy(q,p,nCount * sizeof(CDR3G_ChangeLocation));
        return q;
    }

    /******************************************************
    * 函数名:          List_To_Array
    * 作者:    Zhou Yonghui
    * 日期:    01-5-30
    * 描述:    把ChangeOfCharCondition_LINK_STRUCT结构转换为数组。
    * 输入: 
    *       参数1 :ChangeOfCharCondition_LINK_STRUCT* pHead,
    *              链表的头指针。
    *       参数2 :unsigned int& nLen,返回链表中元素的数目,
    *              也就是转换到数组中的元素的数目。
    * 返回值: CDR3G_ChangeOfCharCondition*,转换结果数组。
    *       若返回NULL,且nLen返回0,标明链表为空；
    *       若内存操作失败，抛出异常。
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *01-5-30       Zhou Yonghui
    ******************************************************/
    CDR3G_ChangeOfCharCondition* 
        List_To_Array(ChangeOfCharCondition_LINK_STRUCT* pHead,
        unsigned int& nLen)
    {
        assert(pHead != NULL);

        unsigned int nCount = 0;
        ChangeOfCharCondition_LINK_STRUCT* q = pHead,
                                         * tmp = NULL;
        while(q)
        {
            nCount += q->nCount;
            q = q->m_pNext;
        }

        nLen = nCount;
        if(nCount > 0)
        {
            CDR3G_ChangeOfCharCondition* p
                = new CDR3G_ChangeOfCharCondition[nCount];
            if(p == NULL)
            {
                throw "Alloc memory fail.";
            }
            
            q = pHead;
            nCount = 0;
            while(q)
            {
                memcpy(p + nCount,q->Value,
                      (q->nCount * sizeof(CDR3G_ChangeOfCharCondition)));
                nCount += q->nCount;
                q = q->m_pNext;
            }
            return p;
        }
        return NULL;
    }

    
    /******************************************************
    * 函数名:   Clear_List
    * 作者:    Zhou Yonghui
    * 日期:    01-6-8
    * 描述:    删除ChangeOfCharCondition_LINK_STRUCT结构链表空间。
    * 输入: 
    *       参数1 :ChangeOfCharCondition_LINK_STRUCT* pHead
    * 返回值: void 
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *01-6-8        Zhou Yonghui
    ******************************************************/
    void Clear_List(ChangeOfCharCondition_LINK_STRUCT* pHead)
    {
        assert(pHead != NULL);

        ChangeOfCharCondition_LINK_STRUCT* q = pHead,* p = NULL;
        while(q)
        {
            p = q;
            q = q->m_pNext;
            delete[] p->Value;
            delete p;
        }
    }

    /******************************************************
    * 函数名:          List_To_Array
    * 作者:    Zhou Yonghui
    * 日期:    01-5-30
    * 描述:    把ManagementExtension_LINK_STRUCT结构转换为数组。
    * 输入: 
    *       参数1 :ManagementExtension_LINK_STRUCT* pHead,
    *              链表的头指针。
    *       参数2 :unsigned int& nLen,返回链表中元素的数目,
    *              也就是转换到数组中的元素的数目。
    * 返回值: CDR3G_ManagementExtension*,转换结果数组。
    *       若返回NULL,且nLen返回0,标明链表为空；
    *       若内存操作失败，抛出异常。
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *01-5-30       Zhou Yonghui
    ******************************************************/
    CDR3G_ManagementExtension* 
        List_To_Array(ManagementExtension_LINK_STRUCT* pHead,
        unsigned int& nLen)
    {
        assert(pHead != NULL);

        unsigned int nCount = 0;
        ManagementExtension_LINK_STRUCT* q = pHead,
                                       * tmp = NULL;
        while(q)
        {
            nCount += q->nCount;
            q = q->m_pNext;
        }

        nLen = nCount;
        if(nCount > 0)
        {
            CDR3G_ManagementExtension* p
                = new CDR3G_ManagementExtension[nCount];
            if(p == NULL)
            {
                throw "Alloc memory fail.";
            }
            
            q = pHead;
            nCount = 0;
            while(q)
            {
                memcpy(p + nCount,q->Value,
                      (q->nCount * sizeof(CDR3G_ManagementExtension)));
                nCount += q->nCount;
                q = q->m_pNext;
            }
            return p;
        }
        return NULL;
    }

    
    /******************************************************
    * 函数名:   Clear_List
    * 作者:    Zhou Yonghui
    * 日期:    01-6-8
    * 描述:    删除ManagementExtension_LINK_STRUCT结构链表的空间。
    * 输入: 
    *       参数1 :ManagementExtension_LINK_STRUCT* pHead
    *       参数2 :unsigned int& nLen
    * 返回值: void 
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *01-6-8        Zhou Yonghui
    ******************************************************/
    void Clear_List(ManagementExtension_LINK_STRUCT* pHead)
    {
        assert(pHead != NULL);

        ManagementExtension_LINK_STRUCT* q = pHead,* p = NULL;
        while(q)
        {
            p = q;
            q = q->m_pNext;
            delete[] p->Value;
            delete p;
        }
    }

    /******************************************************
    * 函数名:          List_To_Array
    * 作者:    Zhou Yonghui
    * 日期:    01-5-30
    * 描述:    把RecordSequenceNumber_LINK_STRUCT结构转换为数组。
    * 输入: 
    *       参数1 :RecordSequenceNumber_LINK_STRUCT* pHead,
    *              链表的头指针。
    *       参数2 :unsigned int& nLen,返回链表中元素的数目,
    *              也就是转换到数组中的元素的数目。
    * 返回值: int*,转换结果数组。
    *       若返回NULL,且nLen返回0,标明链表为空；
    *       若内存操作失败，抛出异常。
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *01-5-30       Zhou Yonghui
    ******************************************************/
    int* List_To_Array(RecordSequenceNumber_LINK_STRUCT* pHead,
        unsigned int& nLen)
    {
        assert(pHead != NULL);

        unsigned int nCount = 0;
        RecordSequenceNumber_LINK_STRUCT* q = pHead,
                                        * tmp = NULL;
        while(q)
        {
            nCount++;
            q = q->m_pNext;
        }

        nLen = nCount;
        if(nCount > 0)
        {
            int* p = new int[nCount];
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
    }

    
    /******************************************************
    * 函数名:   Clear_List
    * 作者:    Zhou Yonghui
    * 日期:    01-6-8
    * 描述:    删除RecordSequenceNumber_LINK_STRUCT结构链表空间。
    * 输入: 
    *       参数1 :RecordSequenceNumber_LINK_STRUCT* pHead
    * 返回值: void 
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *01-6-8        Zhou Yonghui
    ******************************************************/
    void Clear_List(RecordSequenceNumber_LINK_STRUCT* pHead)
    {
        assert(pHead != NULL);

        RecordSequenceNumber_LINK_STRUCT* q = pHead,* p = NULL;
        while(q)
        {
            p = q;
            q = q->m_pNext;
            delete p;
        }
    }

    /******************************************************
    * 函数名:          List_To_Array
    * 作者:    Zhou Yonghui
    * 日期:    01-5-30
    * 描述:    把SGSGAddress_LINK_STRUCT结构转换为数组。
    * 输入: 
    *       参数1 :SGSGAddress_LINK_STRUCT* pHead,
    *              链表的头指针。
    *       参数2 :unsigned int& nLen,返回链表中元素的数目,
    *              也就是转换到数组中的元素的数目。
    * 返回值: CDR3G_GSNAddress*,转换结果数组。
    *       若返回NULL,且nLen返回0,标明链表为空；
    *       若内存操作失败，抛出异常。
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *01-5-30       Zhou Yonghui
    ******************************************************/
    CDR3G_GSNAddress* List_To_Array(SGSGAddress_LINK_STRUCT* pHead,
        unsigned int& nLen)
    {
        assert(pHead != NULL);

        unsigned int nCount = 0;
        SGSGAddress_LINK_STRUCT* q = pHead,
                               * tmp = NULL;
        while(q)
        {
            nCount += q->nCount;
            q = q->m_pNext;
        }

        nLen = nCount;
        if(nCount > 0)
        {
            CDR3G_GSNAddress* p = new CDR3G_GSNAddress[nCount];
            if(p == NULL)
            {
                throw "Alloc memory fail.";
            }
            
            q = pHead;
            nCount = 0;
            while(q)
            {
                memcpy(p + nCount,q->Value,
                      (q->nCount * sizeof(CDR3G_GSNAddress)));
                nCount += q->nCount;
                q = q->m_pNext;
            }
            return p;
        }
        return NULL;
    }

    
    /******************************************************
    * 函数名:   Clear_List
    * 作者:    Zhou Yonghui
    * 日期:    01-6-8
    * 描述:    删除SGSGAddress_LINK_STRUCT结构链表。
    * 输入: 
    *       参数1 :SGSGAddress_LINK_STRUCT* pHead
    * 返回值: void 
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *01-6-8        Zhou Yonghui
    ******************************************************/
    void Clear_List(SGSGAddress_LINK_STRUCT* pHead)
    {
        assert(pHead != NULL);

        SGSGAddress_LINK_STRUCT* q = pHead,* p = NULL;
        while(q)
        {
            p = q;
            q = q->m_pNext;
            delete[] p->Value;
            delete p;
        }
    }


    /******************************************************
    * 函数名:          List_To_Array
    * 作者:    Zhou Yonghui
    * 日期:    01-5-30
    * 描述:    把ChangeLocation_LINK_STRUCT结构转换为数组。
    * 输入: 
    *       参数1 :ChangeLocation_LINK_STRUCT* pHead,
    *              链表的头指针。
    *       参数2 :unsigned int& nLen,返回链表中元素的数目,
    *              也就是转换到数组中的元素的数目。
    * 返回值: CDR3G_ChangeLocation*,转换结果数组。
    *       若返回NULL,且nLen返回0,标明链表为空；
    *       若内存操作失败，抛出异常。
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *01-5-30       Zhou Yonghui
    ******************************************************/
    CDR3G_ChangeLocation* 
        List_To_Array(ChangeLocation_LINK_STRUCT* pHead,
                      unsigned int& nLen)
    {
        assert(pHead != NULL);

        unsigned int nCount = 0;
        ChangeLocation_LINK_STRUCT* q = pHead,
                                  * tmp = NULL;
        while(q)
        {
            nCount += q->nCount;
            q = q->m_pNext;
        }

        nLen = nCount;
        if(nCount > 0)
        {
            CDR3G_ChangeLocation* p = new CDR3G_ChangeLocation[nCount];
            if(p == NULL)
            {
                throw "Alloc memory fail.";
            }
            
            q = pHead;
            nCount = 0;
            while(q)
            {
                memcpy(p + nCount,q->Value,
                      (q->nCount * sizeof(CDR3G_ChangeLocation)));
                nCount += q->nCount;
                q = q->m_pNext;
            }
            return p;
        }
        return NULL;
    }

    
    /******************************************************
    * 函数名:   Clear_List
    * 作者:    Zhou Yonghui
    * 日期:    01-6-8
    * 描述:    删除ChangeLocation_LINK_STRUCT结构链表空间。
    * 输入: 
    *       参数1 :ChangeLocation_LINK_STRUCT* pHead
    * 返回值: void 
    * 说明: 
    * 更新记录: 
    *
    * 日期            姓名                  描述 
    * ========== ================== ======================
    *01-6-8        Zhou Yonghui
    ******************************************************/
    void Clear_List(ChangeLocation_LINK_STRUCT* pHead)
    {
        assert(pHead != NULL);

        ChangeLocation_LINK_STRUCT* q = pHead,* p = NULL;
        while(q)
        {
            p = q;
            q = q->m_pNext;
            delete[] p->Value;
            delete p;
        }
    }

    //计算两个时间之差。
    int Get_Duration(CDR3G_TimeStamp* t1, CDR3G_TimeStamp* t2)
    {
        int nDiff = 0; //时间之差
        char zz1,yy1,mm1,dd1,hh1,MM1,ss1;   //zz1,zz2表示时区
        char zz2,yy2,mm2,dd2,hh2,MM2,ss2;
        char szZz[3];
        
        yy1 = ((t1->value[0] & 0xF0 ) >> 4  ) * 10 + (t1->value[0] & 0x0f) ;
        mm1 = ((t1->value[1] & 0xF0 ) >> 4  ) * 10 + (t1->value[1] & 0x0f) ;
        dd1 = ((t1->value[2] & 0xF0 ) >> 4  ) * 10 + (t1->value[2] & 0x0f) ;
        hh1 = ((t1->value[3] & 0xF0 ) >> 4  ) * 10 + (t1->value[3] & 0x0f) ;
        MM1 = ((t1->value[4] & 0xF0 ) >> 4  ) * 10 + (t1->value[4] & 0x0f) ;
        ss1 = ((t1->value[5] & 0xF0 ) >> 4  ) * 10 + (t1->value[5] & 0x0f) ;
        memcpy( szZz,t1->value + 7, 2);
        szZz[2] = '\0';
        zz1 = atoi(szZz);
        zz1 = (t1->value[6] == '+') ? (zz1) : (-zz1);
        
        yy2 = ((t2->value[0] & 0xF0 ) >> 4  ) * 10 + (t2->value[0] & 0x0f) ;
        mm2 = ((t2->value[1] & 0xF0 ) >> 4  ) * 10 + (t2->value[1] & 0x0f) ;
        dd2 = ((t2->value[2] & 0xF0 ) >> 4  ) * 10 + (t2->value[2] & 0x0f) ;
        hh2 = ((t2->value[3] & 0xF0 ) >> 4  ) * 10 + (t2->value[3] & 0x0f) ;
        MM2 = ((t2->value[4] & 0xF0 ) >> 4  ) * 10 + (t2->value[4] & 0x0f) ;
        ss2 = ((t2->value[5] & 0xF0 ) >> 4  ) * 10 + (t2->value[5] & 0x0f) ;
        memcpy( szZz,t2->value + 7, 2);
        szZz[2] = '\0';
        zz2 = atoi(szZz);
        zz2 = (t2->value[6] == '+') ? (zz2) : (-zz2);
        
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
         * 但是时区是不会改变的，所以下面的公式计算的结果，不会因为
         * 时区有误差。
         * Notice by zhouyonghui,2001/08/22.
         */
        nDiff = (((nDays + (dd2 - dd1)) * 24 
                + (hh2 - hh1 + zz2 - zz1)) * 60 
                + (MM2 - MM1))* 60 + (ss2 - ss1);

        return nDiff;
    }
};

/*
*S-CDR类结构。
*/
class C3G_350_SCDR:public C3G_350_CDR
{
protected:
    PackID_t m_nMinRecordSequence;          //当前合并结果中最小的序列号。
    PackID_t m_nCurRecordSequence;          //当前完成合并的最大序列号。
    BOOL m_bPDPCompleted;                   //PDP是否结束标志。
    BOOL m_bPrepared;                       //标志是否为合并准备过数据。

protected:
    CDR3G_CallDuration  m_Duration;         //用于累加通话时长字段。
    CDR3G_DataVolumeGPRS m_UnsentVolume;    //用于累加rNCUnsentDownlinkVolume字段。
    CDR3G_Diagnostics m_Diagnostics;        //用于存放最后一张话单的diagnostics字段。
    CDR3G_CauseForRecClosing m_CauseForRecClosing;
                                            //用于存放最后一张话单的causeForRecClosing字段。
    
	//added by zouyongsheng  2002-08-06        
	CDR3G_CAMELInformationPDP m_CAMELInformationPDP;
                                            //用于合并话单的CAMELInformationPDP字段
	
public:
    CDR3G_SGSNPDPRecord* m_pOrgRec;         //原始话单结构。
    _3GCDR_CallEventRecord* m_pDestRec;     //合并后话单结构。

    C3G_350_SCDR();
    ~C3G_350_SCDR();

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
class C3G_350_GCDR:public C3G_350_CDR
{
protected:
    PackID_t m_nMinRecordSequence;          //当前合并结果中最小的序列号。
    PackID_t m_nCurRecordSequence;          //当前合并结果中最大的序列号。
    BOOL m_bPDPCompleted;                   //PDP是否结束标志。
    BOOL m_bPrepared;                       //标志是否为合并准备过数据。

protected:
    CDR3G_CallDuration  m_Duration;         //用于累加通话时长字段。
    CDR3G_Diagnostics m_Diagnostics;        //用于存放最后一张话单的diagnostics字段。
    CDR3G_CauseForRecClosing m_CauseForRecClosing;
                                            //用于存放最后一张话单的causeForRecClosing字段。

public:
    CDR3G_GGSNPDPRecord* m_pOrgRec;         //原始话单结构。
    _3GCDR_CallEventRecord* m_pDestRec;     //合并后话单结构。

    C3G_350_GCDR();
    ~C3G_350_GCDR();

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
class C3G_350_MCDR:public C3G_350_CDR
{
protected:
    PackID_t m_nMinRecordSequence;          //当前合并结果中最小的序列号。
    PackID_t m_nCurRecordSequence;          //当前合并结果中最大的序列号。
    BOOL m_bPDPCompleted;                   //PDP是否结束标志。
    BOOL m_bPrepared;                       //标志是否为合并准备过数据。

protected:
    CDR3G_CallDuration  m_Duration;         //用于累加通话时长字段。
    CDR3G_Diagnostics m_Diagnostics;        //用于存放最后一张话单的diagnostics字段。
    CDR3G_CauseForRecClosing m_CauseForRecClosing;
                                            //用于存放最后一张话单的causeForRecClosing字段。
    
	//added by zouyongsheng  2002-08-06
	CDR3G_CAMELInformationMM m_CAMELInformationMM;
                                            //用于存放合并话单的CAMELInforamtionMM字段。

public:
    CDR3G_SGSNMMRecord* m_pOrgRec;          //原始话单结构。
    _3GCDR_CallEventRecord* m_pDestRec;     //合并后话单结构。

    C3G_350_MCDR();
    ~C3G_350_MCDR();

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
class C3G_350_CDR_Key:public CKey
{
public:
    C3G_350_CDR_Key(){};
    ~C3G_350_CDR_Key(){};

    //比较两个IMSI地址
    BOOL Match_IMSI(CDR3G_IMSI* p1,CDR3G_IMSI* p2)
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

//与C3G_350_SCDR话单类对应的索引类。
class C3G_350_SCDR_Key:public C3G_350_CDR_Key
{
public:
    CDR3G_GSNAddress sgsnAddress;
    CDR3G_GSNAddress ggsnAddressUsed;
    CDR3G_ChargingID chargingID;
public:
    C3G_350_SCDR_Key(){};
    ~C3G_350_SCDR_Key(){};

    BOOL IsMatch(CKey* pKey)
    {
        assert(pKey != NULL);

        C3G_350_SCDR_Key* p = (C3G_350_SCDR_Key*)pKey;
        return (Match_gsnAddress(&(p->sgsnAddress),&sgsnAddress)
             && Match_gsnAddress(&(p->ggsnAddressUsed),&ggsnAddressUsed)
             && (p->chargingID == chargingID));
    }

    UINT4 GetKey()
    {
        return chargingID;
    }
};

//与C3G_350_GCDR话单类对应的索引类。
class C3G_350_GCDR_Key:public C3G_350_CDR_Key
{
public:
    CDR3G_GSNAddress ggsnAddress;
    CDR3G_ChargingID chargingID;
public:
    C3G_350_GCDR_Key(){};
    ~C3G_350_GCDR_Key(){};

    BOOL IsMatch(CKey* pKey)
    {
        assert(pKey != NULL);

        C3G_350_GCDR_Key* p = (C3G_350_GCDR_Key*)pKey;
        return (Match_gsnAddress(&(p->ggsnAddress),&ggsnAddress)
            && (p->chargingID == chargingID));
    }

    UINT4 GetKey()
    {
        return chargingID;
    }
};
/*
//与C3G_350_MCDR话单类对应的索引类。
class C3G_350_MCDR_Key:public C3G_350_CDR_Key
{
public:
    CDR3G_IMSI      servedIMSI;
public:
    C3G_350_MCDR_Key(){};
    ~C3G_350_MCDR_Key(){};

    BOOL IsMatch(CKey* pKey)
    {
        assert(pKey != NULL);

        C3G_350_MCDR_Key* p = (C3G_350_MCDR_Key*)pKey;
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
BOOL Match_gsnAddress(CDR3G_GSNAddress* p1,CDR3G_GSNAddress* p2)
{
    assert(p1 != NULL);
    assert(p2 != NULL);
    
    //两张话单的地址类型相同
    if(p1->choice == p2->choice)
    {
        switch(p1->choice)
        {
        case CDR3G_iPBinaryAddress_chosen://比较二进制IP地址
            {
                if((p1->u.iPBinaryAddress.choice == CDR3G_iPBinV4Address_chosen)
                    && (p2->u.iPBinaryAddress.choice == CDR3G_iPBinV4Address_chosen))
                {
					//问题单18398去掉原来的assert
                    
                    return (memcmp(p1->u.iPBinaryAddress.u.iPBinV4Address.value,
                        p2->u.iPBinaryAddress.u.iPBinV4Address.value,
                        p1->u.iPBinaryAddress.u.iPBinV4Address.length)
                        == 0);
                }
                else if((p1->u.iPBinaryAddress.choice == CDR3G_iPBinV6Address_chosen)
                    && (p2->u.iPBinaryAddress.choice == CDR3G_iPBinV6Address_chosen))
                {
					//问题单18398去掉原来的assert
                    
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
        case CDR3G_iPTextRepresentedAddress_chosen://比较文本IP地址
            {
                if((p1->u.iPTextRepresentedAddress.choice == CDR3G_iPTextV4Address_chosen)
                    && (p2->u.iPTextRepresentedAddress.choice == CDR3G_iPTextV4Address_chosen))
                {
                    return (strcmp(p1->u.iPTextRepresentedAddress.u.iPTextV4Address,
                        p2->u.iPTextRepresentedAddress.u.iPTextV4Address)
                        == 0);
                }
                else if((p1->u.iPTextRepresentedAddress.choice == CDR3G_iPTextV6Address_chosen)
                    && (p2->u.iPTextRepresentedAddress.choice == CDR3G_iPTextV6Address_chosen))
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
