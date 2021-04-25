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
*�������ϲ��Ļ�������
*/
#define MAX_MERGED_BILLS    255

/*
* SGSN�л������ʱ�����
*/
#define SGSN_CHANGE_TIME 2
/*
*�ϲ�ǰ�������͡�
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

    virtual BOOL ParseARecord(char* pBuf,UINT4 uBufLen);//����pBuf����һ����������
	virtual CBill* GetBillRecord();                     //���ػ�����¼����

	virtual UINT4 GetBillTypeCount();                   //���ػ�����������
    virtual UINT4 GetBillTypeID(UINT4 uIndex);          //�����������ػ�������ID��
    virtual void RegisterBillType(CMerger* pMerger);    //��CMerger������ע�Ự�����͡�

	void GetBillTypeDesc(UINT4 uBillTypeID, char* szDesc);
};

class C3G_R4_CDR:public CBill
{
protected:

     //�ﵽ���ޱ�־
     BOOL m_bIsLimit;
    //�����ϲ�SGSNAddress�ֶε�����ṹ��
    //m_pSGSNAddressָ������ͷ��ָ�룬
    //m_pSGSNAddressTailָ������β��ָ�롣
	R4_Fin__seqof2_ *m_pSGSNAddress,*m_pSGSNAddressTail;

    //����ĳ���(Ԫ����)
    unsigned int m_nLen_SGSNAddress_List;

    //�����ϲ�listOfTrafficVolumes�ֶε�����ṹ��
    //m_pConditionָ������ͷ��ָ�룬
    //m_pConditionTailָ������β��ָ�롣
    R4_Org__seqof1_* m_pCondition,*m_pConditionTail;

    //����ĳ���(Ԫ����)
    unsigned int m_nLen_Condition_List;
    
    //�����ϲ�recordExtensions�ֶε�����ṹ��
    //m_pExtensionָ������ͷ��ָ�룬
    //m_pExtensionTailָ������β��ָ�롣
    R4_Org_ManagementExtensions_ *m_pExtension,*m_pExtensionTail;

    //����ĳ���(Ԫ����)
    unsigned int m_nLen_Extension_List;

    //�����ϲ�recordSequenceNumber�ֶε�����ṹ��
    //m_pSequenceָ������ͷ��ָ�룬
    //m_pSequenceTailָ������β��ָ�롣
	R4_Fin__seqof1_* m_pRecordSequence,*m_pRecordSequenceTail;

	//�����ϲ�localSequenceNumber�ֶε�����ṹ��
    //m_pSequenceָ������ͷ��ָ�룬
    //m_pSequenceTailָ������β��ָ�롣
	R4_Fin__seqof1_* m_pLocalSequence,*m_pLocalSequenceTail;

	//�����ϲ�rNCDownlinkVolume�ֶε�����ṹ��
    //m_pSequenceָ������ͷ��ָ�룬
    //m_pSequenceTailָ������β��ָ�롣
	R4_Fin__seqof1_* m_pRNCDownlinkVolume,*m_pRNCDownlinkVolumeTail;

    //�����ϲ�changeLocation�ֶε�����ṹ��
    //m_pLocationָ������ͷ��ָ�룬
    //m_pLocationTailָ������β��ָ�롣
	R4_Org_SGSNMMRecord::R4_Org__seqof3* m_pLocation,*m_pLocationTail;

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
         * ���ڽ��кϲ��������Ƶĳ�Ա������
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
    * ������:   Clear_Diagnostics
    * ����:    Guofeng, Guan
    * ����:    2003-06-02
    * ����:    ɾ��R4_Fin_Diagnostics�ṹ�е�ָ���Ա��
    * ����: 
    *       ����1 :R4_Fin_Diagnostics* p
    * ����ֵ: void 
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clone_ManagementExtension
    * ����:    Guofeng, Guan
    * ����:    05-28-2003
    * ����:    ����¡��һ��R4_Org_ManagementExtension�ṹ��
    * ����: 
    *       ����1 :R4_Org_ManagementExtension* p,�ṹָ��
    *       
    * ����ֵ: R4_Org_ManagementExtension*,��¡�Ľ��
    *       ���ڴ����ʧ�ܣ�����NULL��
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clone_ManagementExtensions
    * ����:    Guofeng, Guan
    * ����:    2003-06-02
    * ����:    ����¡��һ��R4_Org_ManagementExtensions_����
    * ����: 
    *       ����1 :R4_Org_ManagementExtensions_* p,����ָ��
    *       ����2 :R4_Org_ManagementExtensions_*& pTail,���������βָ��
	*       ����3 :unsigned int& nCount,����������Ԫ�ظ���
    * ����ֵ: R4_Org_ManagementExtensions_*,��¡�Ľ��
    *       ���ڴ����ʧ�ܣ�����NULL��
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clear_ManagementExtension
    * ����:    Guofeng, Guan
    * ����:    2003-06-02
    * ����:    ɾ���ṹR4_Org_ManagementExtension���ڴ�ռ䡣
    * ����: 
    *       ����1 :R4_Org_ManagementExtension* p,�ṹָ�롣
    *     
    * ����ֵ: void* 
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clear_ManagementExtensions
    * ����:    Zhou Yonghui
    * ����:    01-6-8
    * ����:    ɾ������R4_Org_ManagementExtensions_���ڴ�ռ䡣
    * ����: 
    *       ����1 :R4_Org_ManagementExtensions_* p,����ָ�롣
    *     
    * ����ֵ: void* 
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clone_ChangeOfCharCondition
    * ����:    Guofeng, Guan
    * ����:    2003-06-02
    * ����:    ����¡��һ��R4_Org__seqof1_*����
    * ����: 
    *       ����1 :R4_Org__seqof1_* p,����ָ��
    *       ����2 :unsigned int& nCount,����������Ԫ�ظ���
    * ����ֵ: R4_Org__seqof1_*,��¡�Ľ��
    *       ���ڴ����ʧ�ܣ�����NULL��
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clone_GsnAddressList
    * ����:    Guofeng, Guan
    * ����:    2003-06-02
    * ����:    ����¡��һ��R4_Org_GGSNPDPRecord::R4_Org__seqof2*����
    * ����: 
    *       ����1 :R4_Org_GGSNPDPRecord::R4_Org__seqof2* p,����ָ��
    *       ����2 :unsigned int& nCount,����������Ԫ�ظ���
    * ����ֵ: R4_Org_GGSNPDPRecord::R4_Org__seqof2*,��¡�Ľ��
    *       ���ڴ����ʧ�ܣ��׳��쳣��
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clone_GsnAddressList
    * ����:    Guofeng, Guan
    * ����:    2003-06-02
    * ����:    ����¡��һ��R4_Org_GGSNPDPRecord::R4_Org__seqof2*����
    * ����: 
    *       ����1 :R4_Org_GSNAddress* pDiff,�޳��ظ���SGSN��ַ�Ĳ���
    *       ����2 :R4_Org_GGSNPDPRecord::R4_Org__seqof2* p,����ָ��
    *       ����3 :unsigned int& nCount,����ʵ�ʸ��Ƶ�
    *              Ԫ����.
    * ����ֵ: R4_Org_GGSNPDPRecord::R4_Org__seqof2*,��¡�Ľ��,�Ѿ��޳����ظ���SGSN��ַ.
    *       ��p�е�����Ԫ�ض���pDiff�ظ�,�򷵻ؿ�.
    *       ���ڴ����ʧ��,�׳��쳣.
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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

        //��¡���µĲ���
		R4_Fin__seqof2_* q = Clone_GsnAddressList(pStartPos, pTail, nCount);
		if(q == NULL)
		{				
			throw "Alloc memory fail.";
		}

        return q;
    }

    /******************************************************
    * ������:   Clone_ChangeLocationList
    * ����:    Guofeng, Guan
    * ����:    2003-06-02
    * ����:    ����¡��һ��R4_Org_SGSNMMRecord::R4_Org__seqof3*����
    * ����: 
    *       ����1 :R4_Org_SGSNMMRecord::R4_Org__seqof3* p,����ָ��
	*       ����2 :
    *       ����3 :unsigned int& nCount,����������Ԫ����
    * ����ֵ: R4_Org_SGSNMMRecord::R4_Org__seqof3*,��¡�Ľ��
    *       ���ڴ����ʧ�ܣ��׳��쳣��
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clear_ChangeOfCharCondition
    * ����:    Guofeng, Guan
    * ����:    2003-06-02
    * ����:    ɾ��ChangeOfCharCondition����ռ䡣
    * ����: 
    *       ����1 :R4_Org__seqof1_* pHead
    * ����ֵ: void 
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clear_SequenceNumList
    * ����:    Guofeng, Guan
    * ����:    2003-06-02
    * ����:    ɾ��R4_Fin__seqof1_����ռ䣬
	*          �������recordSequenceNumberList 
	*          LocalSequenceNumberList 
	*          rNCDownlinkVolumeListʹ�õ���
    * ����: 
    *       ����1 :R4_Fin__seqof1* pHead
    * ����ֵ: void 
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clear_RecSequenceNumList
    * ����:    Guofeng, Guan
    * ����:    2003-06-02
    * ����:    ɾ��recordSequenceNumberList  
    * ����: 
    *       ����1 :T* pHead
    * ����ֵ: void 
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clear_LocalSequenceNumList
    * ����:    Guofeng, Guan
    * ����:    2003-06-02
    * ����:    ɾ��localSequenceNumberList 
    * ����: 
    *       ����1 :R4_Fin_SGSNPDPRecord::R4_Fin__seqof5* pHead
    * ����ֵ: void 
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clear_RNCDownlinkVolumeList
    * ����:    Guofeng, Guan
    * ����:    2003-06-02
    * ����:    ɾ��localSequenceNumberList 
    * ����: 
    *       ����1 :R4_Fin_SGSNPDPRecord::R4_Fin__seqof6* pHead
    * ����ֵ: void 
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clear_GsnAddressList
    * ����:    Guofeng, Guan
    * ����:    2003-06-02
    * ����:    ɾ��GsnAddressList�ṹ����
    * ����: 
    *       ����1 :R4_Org__seqof2_* pHead
    * ����ֵ: void 
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clear_ChangeLocationList
    * ����:    Guofeng, Guan
    * ����:    2003-06-02
    * ����:    ɾ��ChangeLocationList����ռ䡣
    * ����: 
    *       ����1 :R4_Org__seqof3* pHead
    * ����ֵ: void 
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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

    //��������ʱ��֮�
	/* ˵���������������¿��ǣ����t2ʱ��Ϊzz2��t1ʱ��Ϊzz1��
	 * ��ôt2�൱�ڸ�������ʱ��t2-zz2��t1���൱�ڸ�������ʱ��t1-zz1��
	 * ���ʱ���Ϊ(t2-zz2)-(t1-zz1)��Ҳ����t2-t1+zz1-zz2
	 */
    int Get_Duration(R4_Org_TimeStamp* t1, R4_Org_TimeStamp* t2)
    {
        int nDiff = 0; //ʱ��֮��
        
        char zhh1,zMM1,yy1,mm1,dd1,hh1,MM1,ss1; //zhh1,zMM1,zhh2,zMM2��ʾʱ����Сʱ������
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
            assert( yy2 > yy1);//�����Ⱥ��ϵУ��

            int m = 0;
            for(m = mm1; m <=12; m++)
                nDays += GetDaysOfMonth(yy1, m);

            for(m = 1; m < mm2; m++)
                nDays += GetDaysOfMonth(yy2, m);
        }
        
        /*
         * һ��GSN���ɵĻ������ꡢ�¡��ա�ʱ���֡��룬�����ܸı䣬
         * GSN��ʱ��Ҳ���ܸı�
         * 
         */
        nDiff = (((nDays + (dd2 - dd1)) * 24 
                + (hh2 - hh1 + zhh1 - zhh2)) * 60 
                + (MM2 - MM1 + zMM1 - zMM2))* 60 + (ss2 - ss1);
        return nDiff;
    }
};

/*
*S-CDR��ṹ��
*/
class C3G_R4_SCDR:public C3G_R4_CDR
{
protected:
    PackID_t m_nMinRecordSequence;          //��ǰ�ϲ��������С�����кš�
    PackID_t m_nCurRecordSequence;          //��ǰ��ɺϲ���������кš�
    BOOL m_bPDPCompleted;                   //PDP�Ƿ������־��
    BOOL m_bPrepared;                       //��־�Ƿ�Ϊ�ϲ�׼�������ݡ�

protected:
    R4_Org_CallDuration  m_Duration;         //�����ۼ�ͨ��ʱ���ֶΡ�
    R4_Org_DataVolumeGPRS m_UnsentVolume;    //�����ۼ�rNCUnsentDownlinkVolume�ֶΡ�
    R4_Org_Diagnostics m_Diagnostics;        //���ڴ�����һ�Ż�����diagnostics�ֶΡ�
    R4_Org_CauseForRecClosing m_CauseForRecClosing;
                                            //���ڴ�����һ�Ż�����causeForRecClosing�ֶΡ�
         
	R4_Org_CAMELInformationPDP m_CAMELInformationPDP;
                                            //���ںϲ�������CAMELInformationPDP�ֶ�

    R4_Fin_SGSNPDPRecord::R4_Fin__seqof4 *m_pRecordSequenceList, *m_pRecordSequenceListTail;
    R4_Fin_SGSNPDPRecord::R4_Fin__seqof5 *m_pLocalSequenceList,  *m_pLocalSequenceListTail;
    R4_Fin_SGSNPDPRecord::R4_Fin__seqof6 *m_pRNCDownlinkVolumeList, *m_pRNCDownlinkVolumeListTail;
	
public:
    R4_Org_SGSNPDPRecord* m_pOrgRec;         //ԭʼ�����ṹ��
    R4_Fin_CallEventRecord* m_pDestRec;     //�ϲ��󻰵��ṹ��

    C3G_R4_SCDR();
    ~C3G_R4_SCDR();

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
class C3G_R4_GCDR:public C3G_R4_CDR
{
protected:
    PackID_t m_nMinRecordSequence;          //��ǰ�ϲ��������С�����кš�
    PackID_t m_nCurRecordSequence;          //��ǰ�ϲ�������������кš�
    BOOL m_bPDPCompleted;                   //PDP�Ƿ������־��
    BOOL m_bPrepared;                       //��־�Ƿ�Ϊ�ϲ�׼�������ݡ�

protected:
    R4_Org_CallDuration  m_Duration;         //�����ۼ�ͨ��ʱ���ֶΡ�
    R4_Org_Diagnostics m_Diagnostics;        //���ڴ�����һ�Ż�����diagnostics�ֶΡ�
    R4_Org_CauseForRecClosing m_CauseForRecClosing;
                                            //���ڴ�����һ�Ż�����causeForRecClosing�ֶΡ�

public:
    R4_Org_GGSNPDPRecord* m_pOrgRec;         //ԭʼ�����ṹ��
    R4_Fin_CallEventRecord* m_pDestRec;     //�ϲ��󻰵��ṹ��

    C3G_R4_GCDR();
    ~C3G_R4_GCDR();

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
class C3G_R4_MCDR:public C3G_R4_CDR
{
protected:
    PackID_t m_nMinRecordSequence;          //��ǰ�ϲ��������С�����кš�
    PackID_t m_nCurRecordSequence;          //��ǰ�ϲ�������������кš�
    BOOL m_bPDPCompleted;                   //PDP�Ƿ������־��
    BOOL m_bPrepared;                       //��־�Ƿ�Ϊ�ϲ�׼�������ݡ�

protected:
    R4_Org_CallDuration  m_Duration;         //�����ۼ�ͨ��ʱ���ֶΡ�
    R4_Org_Diagnostics m_Diagnostics;        //���ڴ�����һ�Ż�����diagnostics�ֶΡ�
    R4_Org_CauseForRecClosing m_CauseForRecClosing;
                                            //���ڴ�����һ�Ż�����causeForRecClosing�ֶΡ�
    
	R4_Org_CAMELInformationMM m_CAMELInformationMM;
                                            //���ڴ�źϲ�������CAMELInforamtionMM�ֶΡ�

public:
    R4_Org_SGSNMMRecord* m_pOrgRec;          //ԭʼ�����ṹ��
    R4_Fin_CallEventRecord* m_pDestRec;     //�ϲ��󻰵��ṹ��

    C3G_R4_MCDR();
    ~C3G_R4_MCDR();

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

    virtual UINT4 GetBillType();            //���ػ�������IDֵ��

    virtual CKey CreateKey();               //������������

    BOOL PrepareData();                     //Ϊ�ϲ�׼�����ݡ�
};

class C3G_R4_CDR_Key:public CKey
{
public:
    C3G_R4_CDR_Key(){};
    ~C3G_R4_CDR_Key(){};

    //�Ƚ�����IMSI��ַ
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

//��C3G_R4_SCDR�������Ӧ�������ࡣ
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

//��C3G_R4_GCDR�������Ӧ�������ࡣ
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

//��C3G_R4_MCDR�������Ӧ�������ࡣ
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


//�Ƚ�����GSN��ַ
BOOL Match_gsnAddress(R4_Org_GSNAddress* p1,R4_Org_GSNAddress* p2)
{
    assert(p1 != NULL);
    assert(p2 != NULL);
    
    //���Ż����ĵ�ַ������ͬ
    if(p1->choice == p2->choice)
    {
        switch(p1->choice)
        {
        case R4_Org_iPBinaryAddress_chosen://�Ƚ϶�����IP��ַ
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
        case R4_Org_iPTextRepresentedAddress_chosen://�Ƚ��ı�IP��ַ
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
