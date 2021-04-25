
#ifndef     _3G_BILL_H
#define     _3G_BILL_H

#include "../asn1_r99/CDRF_3GCDR.h"
#include "../asn1_r99/CDRF_3GCDR_CG.h"
#include "../../merge/bill.h"
#include <assert.h>

/*
*�������ϲ��Ļ�������
*/
#define MAX_MERGED_BILLS    255

/*
*�ϲ�ǰ�������͡�
*/
#define    S_CDR    CDR3G_CallEventRecordType_sgsnPDPRecord
#define    G_CDR    CDR3G_CallEventRecordType_ggsnPDPRecord
#define    M_CDR    CDR3G_CallEventRecordType_sgsnMMRecord
#define    SMO_CDR  CDR3G_CallEventRecordType_sgsnSMORecord
#define    SMT_CDR  CDR3G_CallEventRecordType_sgsnSMTRecord

static BOOL Match_gsnAddress(CDR3G_GSNAddress* p1,CDR3G_GSNAddress* p2);

/*
*C3G_BillParser�̳���CBillParser�࣬����3G�������ͺͻ����ṹ������
*/
class C3G_BillParser:public CBillParser
{
protected:
    CBill* m_pBill;

public:
    C3G_BillParser();
    virtual ~C3G_BillParser();

    virtual BOOL ParseARecord(char* pBuf,UINT4 uBufLen);//����pBuf����һ����������
    virtual CBill* GetBillRecord();                     //���ػ�����¼����

    virtual UINT4 GetBillTypeCount();                   //���ػ�����������
    virtual UINT4 GetBillTypeID(UINT4 uIndex);          //�����������ػ�������ID��
    virtual void RegisterBillType(CMerger* pMerger);    //��CMerger������ע�Ự�����͡�

	//����Զ2002-03-06���
	//���ָ���Ļ�������ID�Ļ�����������
	void GetBillTypeDesc(UINT4 uBillTypeID, char* szDesc);
	//��ӽ���������Զ2002-03-06
};

/*
*C3G_350_CDR���װ������������ʹ�õĹ������ݽṹ�ͺ�����
*/
class C3G_350_CDR:public CBill
{
protected:

     //�ﵽ���ޱ�־
     BOOL m_bIsLimit;
    //�����ϲ�SGSNAddress�ֶε�����ṹ��
    //m_pSGSNAddressָ������ͷ��ָ�룬
    //m_pSGSNAddressTailָ������β��ָ�롣
    struct SGSGAddress_LINK_STRUCT
    {
        CDR3G_GSNAddress* Value;
        unsigned int nCount;//Value����ĳ��ȡ�

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

    //����ĳ���(Ԫ����)
    unsigned int m_nLen_SGSNAddress_List;

    //�����ϲ�listOfTrafficVolumes�ֶε�����ṹ��
    //m_pConditionָ������ͷ��ָ�룬
    //m_pConditionTailָ������β��ָ�롣
    struct ChangeOfCharCondition_LINK_STRUCT
    {
        CDR3G_ChangeOfCharCondition* Value;
        unsigned int nCount;//Value����ĳ��ȡ�

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

    //����ĳ���(Ԫ����)
    unsigned int m_nLen_Condition_List;
    
    //�����ϲ�recordExtensions�ֶε�����ṹ��
    //m_pExtensionָ������ͷ��ָ�룬
    //m_pExtensionTailָ������β��ָ�롣
    struct ManagementExtension_LINK_STRUCT
    {
        CDR3G_ManagementExtension* Value;
        unsigned int nCount;//Value����ĳ��ȡ�

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

    //����ĳ���(Ԫ����)
    unsigned int m_nLen_Extension_List;

    //�����ϲ�recordSequenceNumber�ֶε�����ṹ��
    //m_pSequenceָ������ͷ��ָ�룬
    //m_pSequenceTailָ������β��ָ�롣
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

    //�����ϲ�changeLocation�ֶε�����ṹ��
    //m_pLocationָ������ͷ��ָ�룬
    //m_pLocationTailָ������β��ָ�롣
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

    //����ĳ���(Ԫ����)
    unsigned int m_nLen_Location_List;

protected:
    //�ж��Ƿ����ꡣ
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
    
    //�������£����ظ��µ�������
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
         * ���ڽ��кϲ��������Ƶĳ�Ա������
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
    * ������:   Clear_Diagnostics
    * ����:    Zhou Yonghui
    * ����:    01-6-8
    * ����:    ɾ��CDR3G_Diagnostics�ṹ�е�ָ���Ա��
    * ����: 
    *       ����1 :CDR3G_Diagnostics* p
    * ����ֵ: void 
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clone_CDR3G_ManagementExtension
    * ����:    Zhou Yonghui
    * ����:    01-5-30
    * ����:    ����¡��һ��CDR3G_ManagementExtension�ṹ���顣
    * ����: 
    *       ����1 :DR3G_ManagementExtension* p,����ָ��
    *       ����2 :unsigned int nCount,�����Ԫ����
    * ����ֵ: CDR3G_ManagementExtension*,��¡�Ľ��
    *       ���ڴ����ʧ�ܣ�����NULL��
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
            //��nErrCode > 0,˵������identifierʧ�ܡ�
            //��nErrCode < 0,˵������informationʧ�ܡ�
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
    * ������:   Clear_ManagementExtension
    * ����:    Zhou Yonghui
    * ����:    01-6-8
    * ����:    ɾ���ṹCDR3G_ManagementExtension���ڴ�ռ䡣
    * ����: 
    *       ����1 :CDR3G_ManagementExtension* p,�ṹ���顣
    *       ����2 :unsigned int nCount,p�����Ԫ�ظ�����
    * ����ֵ: void* 
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clone_CDR3G_ChangeOfCharCondition
    * ����:    Zhou Yonghui
    * ����:    01-5-30
    * ����:    ����¡��һ��CDR3G_ManagementExtension�ṹ���顣
    * ����: 
    *       ����1 :DR3G_ChangeOfCharCondition* p,����ָ��
    *       ����2 :unsigned int nCount,�����Ԫ����
    * ����ֵ: CDR3G_ChangeOfCharCondition*,��¡�Ľ��
    *       ���ڴ����ʧ�ܣ�����NULL��
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clone_CDR3G_GSNAddress
    * ����:    Zhou Yonghui
    * ����:    01-5-30
    * ����:    ����¡��һ��CDR3G_GSNAddress�ṹ���顣
    * ����: 
    *       ����1 :DR3G_GSNAddress* p,����ͷָ��
    *       ����2 :unsigned int nCount,����Ԫ����
    * ����ֵ: CDR3G_GSNAddress*,��¡�Ľ��
    *       ���ڴ����ʧ�ܣ��׳��쳣��
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clone_CDR3G_GSNAddress
    * ����:    Zhou Yonghui
    * ����:    01-5-30
    * ����:    ����¡��һ��CDR3G_GSNAddress�ṹ���顣
    * ����: 
    *       ����1 :DR3G_GSNAddress* pDiff,�޳��ظ���SGSN��ַ�Ĳ���
    *       ����2 :DR3G_GSNAddress* p,����ͷָ��
    *       ����3 :unsigned int nCount,����Ԫ����,������ʵ�ʸ��Ƶ�
    *              Ԫ����.
    * ����ֵ: CDR3G_GSNAddress*,��¡�Ľ��,�Ѿ��޳����ظ���SGSN��ַ.
    *       ��p�е�����Ԫ�ض���pDiff�ظ�,�򷵻ؿ�.
    *       ���ڴ����ʧ��,�׳��쳣.
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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

        //��¡���µĲ���
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
    * ������:   Clone_CDR3G_ChangeLocation
    * ����:    Zhou Yonghui
    * ����:    01-5-30
    * ����:    ����¡��һ��CDR3G_ChangeLocation�ṹ���顣
    * ����: 
    *       ����1 :CDR3G_ChangeLocation* p,����ͷָ��
    *       ����2 :unsigned int nCount,����Ԫ����
    * ����ֵ: CDR3G_ChangeLocation*,��¡�Ľ��
    *       ���ڴ����ʧ�ܣ��׳��쳣��
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:          List_To_Array
    * ����:    Zhou Yonghui
    * ����:    01-5-30
    * ����:    ��ChangeOfCharCondition_LINK_STRUCT�ṹת��Ϊ���顣
    * ����: 
    *       ����1 :ChangeOfCharCondition_LINK_STRUCT* pHead,
    *              �����ͷָ�롣
    *       ����2 :unsigned int& nLen,����������Ԫ�ص���Ŀ,
    *              Ҳ����ת���������е�Ԫ�ص���Ŀ��
    * ����ֵ: CDR3G_ChangeOfCharCondition*,ת��������顣
    *       ������NULL,��nLen����0,��������Ϊ�գ�
    *       ���ڴ����ʧ�ܣ��׳��쳣��
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clear_List
    * ����:    Zhou Yonghui
    * ����:    01-6-8
    * ����:    ɾ��ChangeOfCharCondition_LINK_STRUCT�ṹ����ռ䡣
    * ����: 
    *       ����1 :ChangeOfCharCondition_LINK_STRUCT* pHead
    * ����ֵ: void 
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:          List_To_Array
    * ����:    Zhou Yonghui
    * ����:    01-5-30
    * ����:    ��ManagementExtension_LINK_STRUCT�ṹת��Ϊ���顣
    * ����: 
    *       ����1 :ManagementExtension_LINK_STRUCT* pHead,
    *              �����ͷָ�롣
    *       ����2 :unsigned int& nLen,����������Ԫ�ص���Ŀ,
    *              Ҳ����ת���������е�Ԫ�ص���Ŀ��
    * ����ֵ: CDR3G_ManagementExtension*,ת��������顣
    *       ������NULL,��nLen����0,��������Ϊ�գ�
    *       ���ڴ����ʧ�ܣ��׳��쳣��
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clear_List
    * ����:    Zhou Yonghui
    * ����:    01-6-8
    * ����:    ɾ��ManagementExtension_LINK_STRUCT�ṹ����Ŀռ䡣
    * ����: 
    *       ����1 :ManagementExtension_LINK_STRUCT* pHead
    *       ����2 :unsigned int& nLen
    * ����ֵ: void 
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:          List_To_Array
    * ����:    Zhou Yonghui
    * ����:    01-5-30
    * ����:    ��RecordSequenceNumber_LINK_STRUCT�ṹת��Ϊ���顣
    * ����: 
    *       ����1 :RecordSequenceNumber_LINK_STRUCT* pHead,
    *              �����ͷָ�롣
    *       ����2 :unsigned int& nLen,����������Ԫ�ص���Ŀ,
    *              Ҳ����ת���������е�Ԫ�ص���Ŀ��
    * ����ֵ: int*,ת��������顣
    *       ������NULL,��nLen����0,��������Ϊ�գ�
    *       ���ڴ����ʧ�ܣ��׳��쳣��
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clear_List
    * ����:    Zhou Yonghui
    * ����:    01-6-8
    * ����:    ɾ��RecordSequenceNumber_LINK_STRUCT�ṹ����ռ䡣
    * ����: 
    *       ����1 :RecordSequenceNumber_LINK_STRUCT* pHead
    * ����ֵ: void 
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:          List_To_Array
    * ����:    Zhou Yonghui
    * ����:    01-5-30
    * ����:    ��SGSGAddress_LINK_STRUCT�ṹת��Ϊ���顣
    * ����: 
    *       ����1 :SGSGAddress_LINK_STRUCT* pHead,
    *              �����ͷָ�롣
    *       ����2 :unsigned int& nLen,����������Ԫ�ص���Ŀ,
    *              Ҳ����ת���������е�Ԫ�ص���Ŀ��
    * ����ֵ: CDR3G_GSNAddress*,ת��������顣
    *       ������NULL,��nLen����0,��������Ϊ�գ�
    *       ���ڴ����ʧ�ܣ��׳��쳣��
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clear_List
    * ����:    Zhou Yonghui
    * ����:    01-6-8
    * ����:    ɾ��SGSGAddress_LINK_STRUCT�ṹ����
    * ����: 
    *       ����1 :SGSGAddress_LINK_STRUCT* pHead
    * ����ֵ: void 
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:          List_To_Array
    * ����:    Zhou Yonghui
    * ����:    01-5-30
    * ����:    ��ChangeLocation_LINK_STRUCT�ṹת��Ϊ���顣
    * ����: 
    *       ����1 :ChangeLocation_LINK_STRUCT* pHead,
    *              �����ͷָ�롣
    *       ����2 :unsigned int& nLen,����������Ԫ�ص���Ŀ,
    *              Ҳ����ת���������е�Ԫ�ص���Ŀ��
    * ����ֵ: CDR3G_ChangeLocation*,ת��������顣
    *       ������NULL,��nLen����0,��������Ϊ�գ�
    *       ���ڴ����ʧ�ܣ��׳��쳣��
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clear_List
    * ����:    Zhou Yonghui
    * ����:    01-6-8
    * ����:    ɾ��ChangeLocation_LINK_STRUCT�ṹ����ռ䡣
    * ����: 
    *       ����1 :ChangeLocation_LINK_STRUCT* pHead
    * ����ֵ: void 
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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

    //��������ʱ��֮�
    int Get_Duration(CDR3G_TimeStamp* t1, CDR3G_TimeStamp* t2)
    {
        int nDiff = 0; //ʱ��֮��
        char zz1,yy1,mm1,dd1,hh1,MM1,ss1;   //zz1,zz2��ʾʱ��
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
            assert( yy2 > yy1);//�����Ⱥ��ϵУ��

            int m = 0;
            for(m = mm1; m <=12; m++)
                nDays += GetDaysOfMonth(yy1, m);

            for(m = 1; m < mm2; m++)
                nDays += GetDaysOfMonth(yy2, m);
        }
        
        /*
         * һ��GSN���ɵĻ������ꡢ�¡��ա�ʱ���֡��룬�����ܸı䣬
         * ����ʱ���ǲ���ı�ģ���������Ĺ�ʽ����Ľ����������Ϊ
         * ʱ������
         * Notice by zhouyonghui,2001/08/22.
         */
        nDiff = (((nDays + (dd2 - dd1)) * 24 
                + (hh2 - hh1 + zz2 - zz1)) * 60 
                + (MM2 - MM1))* 60 + (ss2 - ss1);

        return nDiff;
    }
};

/*
*S-CDR��ṹ��
*/
class C3G_350_SCDR:public C3G_350_CDR
{
protected:
    PackID_t m_nMinRecordSequence;          //��ǰ�ϲ��������С�����кš�
    PackID_t m_nCurRecordSequence;          //��ǰ��ɺϲ���������кš�
    BOOL m_bPDPCompleted;                   //PDP�Ƿ������־��
    BOOL m_bPrepared;                       //��־�Ƿ�Ϊ�ϲ�׼�������ݡ�

protected:
    CDR3G_CallDuration  m_Duration;         //�����ۼ�ͨ��ʱ���ֶΡ�
    CDR3G_DataVolumeGPRS m_UnsentVolume;    //�����ۼ�rNCUnsentDownlinkVolume�ֶΡ�
    CDR3G_Diagnostics m_Diagnostics;        //���ڴ�����һ�Ż�����diagnostics�ֶΡ�
    CDR3G_CauseForRecClosing m_CauseForRecClosing;
                                            //���ڴ�����һ�Ż�����causeForRecClosing�ֶΡ�
    
	//added by zouyongsheng  2002-08-06        
	CDR3G_CAMELInformationPDP m_CAMELInformationPDP;
                                            //���ںϲ�������CAMELInformationPDP�ֶ�
	
public:
    CDR3G_SGSNPDPRecord* m_pOrgRec;         //ԭʼ�����ṹ��
    _3GCDR_CallEventRecord* m_pDestRec;     //�ϲ��󻰵��ṹ��

    C3G_350_SCDR();
    ~C3G_350_SCDR();

    virtual BOOL Init(char* pBuf,UINT4 uLen); //ʹ�û�����¼��ʼ����������

    virtual BOOL CanSafeMerge(CBill* pBill);//�жϵ�ǰ�����Ƿ�������pBill�ϲ���
    virtual BOOL Merge(CBill* pBill);       //��pBill���뵱ǰ�����С�
    virtual BOOL IsPDPCompleted();          //�ж�PDP�Ѿ�������
    virtual BOOL NeedWaiting();             //�ж��Ƿ���Ҫ�����ȴ�����������

    virtual char* GetContents();            //���ػ����ļ�¼�ṹ��

    virtual BOOL IsPartial();               //�ж��Ƿ񲿷ֻ�����
    virtual BOOL IsHotBilling();            //�ж��Ƿ����ȼƷ�
    virtual BOOL IsFirstPartialRec();       //�ж��Ƿ�PDP�ĵ�һ�Ų��ֻ�����
    virtual BOOL IsLastPartialRec();        //�ж��Ƿ�PDP��ĩ�Ų��ֻ�����
    virtual BOOL IsDuplicated(CBill* pBill);  //�ж��Ƿ��ظ�����    

    virtual UINT4 GetBillType();              //���ػ�������IDֵ��

    virtual CKey CreateKey();               //������������

    BOOL PrepareData();                     //Ϊ�ϲ�׼�����ݡ�
};

/*
*G-CDR��ṹ��
*/
class C3G_350_GCDR:public C3G_350_CDR
{
protected:
    PackID_t m_nMinRecordSequence;          //��ǰ�ϲ��������С�����кš�
    PackID_t m_nCurRecordSequence;          //��ǰ�ϲ�������������кš�
    BOOL m_bPDPCompleted;                   //PDP�Ƿ������־��
    BOOL m_bPrepared;                       //��־�Ƿ�Ϊ�ϲ�׼�������ݡ�

protected:
    CDR3G_CallDuration  m_Duration;         //�����ۼ�ͨ��ʱ���ֶΡ�
    CDR3G_Diagnostics m_Diagnostics;        //���ڴ�����һ�Ż�����diagnostics�ֶΡ�
    CDR3G_CauseForRecClosing m_CauseForRecClosing;
                                            //���ڴ�����һ�Ż�����causeForRecClosing�ֶΡ�

public:
    CDR3G_GGSNPDPRecord* m_pOrgRec;         //ԭʼ�����ṹ��
    _3GCDR_CallEventRecord* m_pDestRec;     //�ϲ��󻰵��ṹ��

    C3G_350_GCDR();
    ~C3G_350_GCDR();

    virtual BOOL Init(char* pBuf,UINT4 uLen); //ʹ�û�����¼��ʼ����������

    virtual BOOL CanSafeMerge(CBill* pBill);//�жϵ�ǰ�����Ƿ�������pBill�ϲ���
    virtual BOOL Merge(CBill* pBill);       //��pBill���뵱ǰ�����С�
    virtual BOOL IsPDPCompleted();          //�ж�PDP�Ѿ�������
    virtual BOOL NeedWaiting();             //�ж��Ƿ���Ҫ�����ȴ�����������

    virtual char* GetContents();            //���ػ����ļ�¼�ṹ��

    virtual BOOL IsPartial();               //�ж��Ƿ񲿷ֻ�����
    virtual BOOL IsHotBilling();            //�ж��Ƿ����ȼƷ�
    virtual BOOL IsFirstPartialRec();       //�ж��Ƿ�PDP�ĵ�һ�Ų��ֻ�����
    virtual BOOL IsLastPartialRec();        //�ж��Ƿ�PDP��ĩ�Ų��ֻ�����
    virtual BOOL IsDuplicated(CBill* pBill);  //�ж��Ƿ��ظ�����    

    virtual UINT4 GetBillType();              //���ػ�������IDֵ��

    virtual CKey CreateKey();               //������������

    BOOL PrepareData();                     //Ϊ�ϲ�׼�����ݡ�
};

/*
*M-CDR��ṹ��
*/
/*
class C3G_350_MCDR:public C3G_350_CDR
{
protected:
    PackID_t m_nMinRecordSequence;          //��ǰ�ϲ��������С�����кš�
    PackID_t m_nCurRecordSequence;          //��ǰ�ϲ�������������кš�
    BOOL m_bPDPCompleted;                   //PDP�Ƿ������־��
    BOOL m_bPrepared;                       //��־�Ƿ�Ϊ�ϲ�׼�������ݡ�

protected:
    CDR3G_CallDuration  m_Duration;         //�����ۼ�ͨ��ʱ���ֶΡ�
    CDR3G_Diagnostics m_Diagnostics;        //���ڴ�����һ�Ż�����diagnostics�ֶΡ�
    CDR3G_CauseForRecClosing m_CauseForRecClosing;
                                            //���ڴ�����һ�Ż�����causeForRecClosing�ֶΡ�
    
	//added by zouyongsheng  2002-08-06
	CDR3G_CAMELInformationMM m_CAMELInformationMM;
                                            //���ڴ�źϲ�������CAMELInforamtionMM�ֶΡ�

public:
    CDR3G_SGSNMMRecord* m_pOrgRec;          //ԭʼ�����ṹ��
    _3GCDR_CallEventRecord* m_pDestRec;     //�ϲ��󻰵��ṹ��

    C3G_350_MCDR();
    ~C3G_350_MCDR();

    virtual BOOL Init(char* pBuf,UINT4 uLen); //ʹ�û�����¼��ʼ����������

    virtual BOOL CanSafeMerge(CBill* pBill);//�жϵ�ǰ�����Ƿ�������pBill�ϲ���
    virtual BOOL Merge(CBill* pBill);       //��pBill���뵱ǰ�����С�
    virtual BOOL IsPDPCompleted();          //�ж�PDP�Ѿ�������
    virtual BOOL NeedWaiting();             //�ж��Ƿ���Ҫ�����ȴ�����������

    virtual char* GetContents();            //���ػ����ļ�¼�ṹ��

    virtual BOOL IsPartial();               //�ж��Ƿ񲿷ֻ�����
    virtual BOOL IsFirstPartialRec();       //�ж��Ƿ�PDP�ĵ�һ�Ų��ֻ�����
    virtual BOOL IsLastPartialRec();        //�ж��Ƿ�PDP��ĩ�Ų��ֻ�����

    virtual UINT4 GetBillType();            //���ػ�������IDֵ��

    virtual CKey CreateKey();               //������������

    BOOL PrepareData();                     //Ϊ�ϲ�׼�����ݡ�
};
*/
class C3G_350_CDR_Key:public CKey
{
public:
    C3G_350_CDR_Key(){};
    ~C3G_350_CDR_Key(){};

    //�Ƚ�����IMSI��ַ
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

//��C3G_350_SCDR�������Ӧ�������ࡣ
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

//��C3G_350_GCDR�������Ӧ�������ࡣ
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
//��C3G_350_MCDR�������Ӧ�������ࡣ
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

//�Ƚ�����GSN��ַ
BOOL Match_gsnAddress(CDR3G_GSNAddress* p1,CDR3G_GSNAddress* p2)
{
    assert(p1 != NULL);
    assert(p2 != NULL);
    
    //���Ż����ĵ�ַ������ͬ
    if(p1->choice == p2->choice)
    {
        switch(p1->choice)
        {
        case CDR3G_iPBinaryAddress_chosen://�Ƚ϶�����IP��ַ
            {
                if((p1->u.iPBinaryAddress.choice == CDR3G_iPBinV4Address_chosen)
                    && (p2->u.iPBinaryAddress.choice == CDR3G_iPBinV4Address_chosen))
                {
					//���ⵥ18398ȥ��ԭ����assert
                    
                    return (memcmp(p1->u.iPBinaryAddress.u.iPBinV4Address.value,
                        p2->u.iPBinaryAddress.u.iPBinV4Address.value,
                        p1->u.iPBinaryAddress.u.iPBinV4Address.length)
                        == 0);
                }
                else if((p1->u.iPBinaryAddress.choice == CDR3G_iPBinV6Address_chosen)
                    && (p2->u.iPBinaryAddress.choice == CDR3G_iPBinV6Address_chosen))
                {
					//���ⵥ18398ȥ��ԭ����assert
                    
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
        case CDR3G_iPTextRepresentedAddress_chosen://�Ƚ��ı�IP��ַ
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
        default://choice�ֶ�ֵ�Ƿ�
            assert(0);
            break;
        }
    }
    //���Ż����ĵ�ַ���Ͳ�ͬ
    else
    {
        //�����ƺ��ı���ʽ�ĵ�ַ��Ϊ��ͬ
    }
    
    return FALSE;
}

#endif
