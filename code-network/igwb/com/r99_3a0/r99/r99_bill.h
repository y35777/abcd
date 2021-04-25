      
#ifndef     R99_BILL_H
#define     R99_BILL_H

#include "../asn1_r99/CDRF_CMCC_3GCDR.h"
#include "../asn1_r99/CDRF_CMCC_3GCDR_CG.h"
#include "../asn1_r99/oss_asn1_r99.h"
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

/*
*�ϲ�ǰ�������͡�
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
	unsigned int nCount;//Value����ĳ��ȡ�

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
*CR99_BillParser�̳���CBillParser�࣬�����й��ƶ�3G R99�������ͺͻ����ṹ������
*/
class CR99_BillParser:public CBillParser
{
protected:
    CBill* m_pBill;

public:
    CR99_BillParser();
    virtual ~CR99_BillParser();

    virtual BOOL ParseARecord(char* pBuf,UINT4 uBufLen);//����pBuf����һ����������
    virtual CBill* GetBillRecord();                     //���ػ�����¼����

    virtual UINT4 GetBillTypeCount();                   //���ػ�����������
    virtual UINT4 GetBillTypeID(UINT4 uIndex);          //�����������ػ�������ID��
    virtual void RegisterBillType(CMerger* pMerger);    //��CMerger������ע�Ự�����͡�

	void GetBillTypeDesc(UINT4 uBillTypeID, char* szDesc);
};

/*
*CR99_CDR���װ������������ʹ�õĹ������ݽṹ�ͺ�����
*/
class CR99_CDR:public CBill
{
protected:
    //�����ϲ�SGSNAddress�ֶε�����ṹ��
    //m_pSGSNAddressָ������ͷ��ָ�룬
    //m_pSGSNAddressTailָ������β��ָ�롣
	LINK_STRUCT2<CMCC_3GCDR_GSNAddress> *m_pSGSNAddress, *m_pSGSNAddressTail;

    //����ĳ���(Ԫ����)
    unsigned int m_nLen_SGSNAddress_List;

    //�����ϲ�listOfTrafficVolumes�ֶε�����ṹ��
    //m_pConditionָ������ͷ��ָ�룬
    //m_pConditionTailָ������β��ָ�롣
	LINK_STRUCT2<CMCC_3GCDR_ChangeOfCharCondition> *m_pCondition,*m_pConditionTail;

    //����ĳ���(Ԫ����)
    unsigned int m_nLen_Condition_List;
    
    //�����ϲ�recordExtensions�ֶε�����ṹ��
    //m_pExtensionָ������ͷ��ָ�룬
    //m_pExtensionTailָ������β��ָ�롣
	LINK_STRUCT2<CMCC_3GCDR_ManagementExtension> *m_pExtension,*m_pExtensionTail;

    //����ĳ���(Ԫ����)
    unsigned int m_nLen_Extension_List;

    //�����ϲ�recordSequenceNumber�ֶε�����ṹ��
    //m_pSequenceָ������ͷ��ָ�룬
    //m_pSequenceTailָ������β��ָ�롣
	LINK_STRUCT1<int> *m_pSequence,*m_pSequenceTail;

	//�����ϲ�LocalSequenceNumber�ֶε�����ṹ��
    //m_pLocalSequenceNumberָ������ͷ��ָ�룬
    //m_pLocalSequenceNumberTailָ������β��ָ�롣
	LINK_STRUCT1<int> *m_pLocalSequenceNumber,*m_pLocalSequenceNumberTail;

	//�����ϲ�rNCUnsentDownlinkVolume�ֶε�����ṹ��
    //m_pRNCUnsentDownlinkVolumeָ������ͷ��ָ�룬
    //m_pRNCUnsentDownlinkVolumeTailָ������β��ָ�롣
	LINK_STRUCT1<int> *m_pRNCUnsentDownlinkVolume,*m_pRNCUnsentDownlinkVolumeTail;

    //�����ϲ�changeLocation�ֶε�����ṹ��
    //m_pLocationָ������ͷ��ָ�룬
    //m_pLocationTailָ������β��ָ�롣
	LINK_STRUCT2<CMCC_3GCDR_ChangeLocation> *m_pLocation, *m_pLocationTail;

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
         * ���ڽ��кϲ��������Ƶĳ�Ա������
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
    * ������:   Clear_Diagnostics
    * ����:    GGF
    * ����:    2003-09-28
    * ����:    ɾ��CMCC_3GCDR_Diagnostics�ṹ�е�ָ���Ա��
    * ����: 
    *       ����1 :CMCC_3GCDR_Diagnostics* p
    * ����ֵ: void 
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clone_ManagementExtension
    * ����:    GGF
    * ����:    2003-09-28
    * ����:    ����¡��һ��CMCC_3GCDR_ManagementExtension�ṹ���顣
    * ����: 
    *       ����1 :CMCC_3GCDR_ManagementExtension* p,����ָ��
    *       ����2 :unsigned int nCount,�����Ԫ����
    * ����ֵ: CMCC_3GCDR_ManagementExtension*,��¡�Ľ��
    *       ���ڴ����ʧ�ܣ�����NULL��
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ����:    GGF
    * ����:    2003-09-28
    * ����:    ɾ���ṹCMCC_3GCDR_ManagementExtension���ڴ�ռ䡣
    * ����: 
    *       ����1 :CMCC_3GCDR_ManagementExtension* p,�ṹ���顣
    *       ����2 :unsigned int nCount,p�����Ԫ�ظ�����
    * ����ֵ: void* 
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clone_ChangeOfCharCondition
    * ����:    GGF
    * ����:    2003-09-28
    * ����:    ����¡��һ��CMCC_3GCDR_ChangeOfCharCondition�ṹ���顣
    * ����: 
    *       ����1 :CMCC_3GCDR_ChangeOfCharCondition* p,����ָ��
    *       ����2 :unsigned int nCount,�����Ԫ����
    * ����ֵ: CMCC_3GCDR_ChangeOfCharCondition*,��¡�Ľ��
    *       ���ڴ����ʧ�ܣ�����NULL��
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clone_GSNAddress
    * ����:    GGF
    * ����:    2003-09-28
    * ����:    ����¡��һ��CMCC_3GCDR_GSNAddress�ṹ���顣
    * ����: 
    *       ����1 :CMCC_3GCDR_GSNAddress* p,����ͷָ��
    *       ����2 :unsigned int nCount,����Ԫ����
    * ����ֵ: CMCC_3GCDR_GSNAddress*,��¡�Ľ��
    *       ���ڴ����ʧ�ܣ��׳��쳣��
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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
    * ������:   Clone_GSNAddress
    * ����:    GGF
    * ����:    2003-09-28
    * ����:    ����¡��һ��CMCC_3GCDR_GSNAddress�ṹ���顣
    * ����: 
    *       ����1 :CMCC_3GCDR_GSNAddress* pDiff,�޳��ظ���SGSN��ַ�Ĳ���
    *       ����2 :CMCC_3GCDR_GSNAddress* p,����ͷָ��
    *       ����3 :unsigned int nCount,����Ԫ����,������ʵ�ʸ��Ƶ�
    *              Ԫ����.
    * ����ֵ: CMCC_3GCDR_GSNAddress*,��¡�Ľ��,�Ѿ��޳����ظ���SGSN��ַ.
    *       ��p�е�����Ԫ�ض���pDiff�ظ�,�򷵻ؿ�.
    *       ���ڴ����ʧ��,�׳��쳣.
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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

        //��¡���µĲ���
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
    * ������:   Clone_ChangeLocation
    * ����:    GGF
    * ����:    2003-09-28
    * ����:    ����¡��һ��CMCC_3GCDR_ChangeLocation�ṹ���顣
    * ����: 
    *       ����1 :CMCC_3GCDR_ChangeLocation* p,����ͷָ��
    *       ����2 :unsigned int nCount,����Ԫ����
    * ����ֵ: CMCC_3GCDR_ChangeLocation*,��¡�Ľ��
    *       ���ڴ����ʧ�ܣ��׳��쳣��
    * ˵��: 
    * ���¼�¼: 
    *
    * ����            ����                  ���� 
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


	//��������ʱ��֮�
	/* ˵���������������¿��ǣ����t2ʱ��Ϊzz2��t1ʱ��Ϊzz1��
	 * ��ôt2�൱�ڸ�������ʱ��t2-zz2��t1���൱�ڸ�������ʱ��t1-zz1��
	 * ���ʱ���Ϊ(t2-zz2)-(t1-zz1)��Ҳ����t2-t1+zz1-zz2
	 */
    int Get_Duration(CMCC_3GCDR_TimeStamp* t1, CMCC_3GCDR_TimeStamp* t2)
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
class CR99_SCDR:public CR99_CDR
{
protected:
    PackID_t m_nMinRecordSequence;          //��ǰ�ϲ��������С�����кš�
    PackID_t m_nCurRecordSequence;          //��ǰ��ɺϲ���������кš�
    BOOL m_bPDPCompleted;                   //PDP�Ƿ������־��
    BOOL m_bPrepared;                       //��־�Ƿ�Ϊ�ϲ�׼�������ݡ�

protected:
    CMCC_3GCDR_CallDuration  m_Duration;         //�����ۼ�ͨ��ʱ���ֶΡ�
    CMCC_3GCDR_DataVolumeGPRS m_UnsentVolume;    //�����ۼ�rNCUnsentDownlinkVolume�ֶΡ�
    CMCC_3GCDR_Diagnostics m_Diagnostics;        //���ڴ�����һ�Ż�����diagnostics�ֶΡ�
    CMCC_3GCDR_CauseForRecClosing m_CauseForRecClosing;
                                            //���ڴ�����һ�Ż�����causeForRecClosing�ֶΡ�
     
	CMCC_3GCDR_CAMELInformationPDP m_CAMELInformationPDP;
                                            //���ںϲ�������CAMELInformationPDP�ֶ�

    typedef struct SGSN_DATA1 {

        CMCC_3GCDR_CG_GSNAddress sgsnAddress;
    	unsigned int nCount;    //pList����ĳ��ȡ�
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
    CMCC_3GCDR_SGSNPDPRecord* m_pOrgRec;         //ԭʼ�����ṹ��
    CMCC_3GCDR_CG_CallEventRecord* m_pDestRec;     //�ϲ��󻰵��ṹ��

    CR99_SCDR();
    ~CR99_SCDR();

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
class CR99_GCDR:public CR99_CDR
{
protected:
    PackID_t m_nMinRecordSequence;          //��ǰ�ϲ��������С�����кš�
    PackID_t m_nCurRecordSequence;          //��ǰ�ϲ�������������кš�
    BOOL m_bPDPCompleted;                   //PDP�Ƿ������־��
    BOOL m_bPrepared;                       //��־�Ƿ�Ϊ�ϲ�׼�������ݡ�

protected:
    CMCC_3GCDR_CallDuration  m_Duration;         //�����ۼ�ͨ��ʱ���ֶΡ�
    CMCC_3GCDR_Diagnostics m_Diagnostics;        //���ڴ�����һ�Ż�����diagnostics�ֶΡ�
    CMCC_3GCDR_CauseForRecClosing m_CauseForRecClosing;
                                            //���ڴ�����һ�Ż�����causeForRecClosing�ֶΡ�

public:
    CMCC_3GCDR_GGSNPDPRecord* m_pOrgRec;         //ԭʼ�����ṹ��
    CMCC_3GCDR_CG_CallEventRecord* m_pDestRec;     //�ϲ��󻰵��ṹ��

    CR99_GCDR();
    ~CR99_GCDR();

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
class CR99_MCDR:public CR99_CDR
{
protected:
    PackID_t m_nMinRecordSequence;          //��ǰ�ϲ��������С�����кš�
    PackID_t m_nCurRecordSequence;          //��ǰ�ϲ�������������кš�
    BOOL m_bPDPCompleted;                   //PDP�Ƿ������־��
    BOOL m_bPrepared;                       //��־�Ƿ�Ϊ�ϲ�׼�������ݡ�

protected:
    CMCC_3GCDR_CallDuration  m_Duration;         //�����ۼ�ͨ��ʱ���ֶΡ�
    CMCC_3GCDR_Diagnostics m_Diagnostics;        //���ڴ�����һ�Ż�����diagnostics�ֶΡ�
    CMCC_3GCDR_CauseForRecClosing m_CauseForRecClosing;
                                            //���ڴ�����һ�Ż�����causeForRecClosing�ֶΡ�
    
	CMCC_3GCDR_CAMELInformationMM m_CAMELInformationMM;
                                            //���ڴ�źϲ�������CAMELInforamtionMM�ֶΡ�

public:
    CMCC_3GCDR_SGSNMMRecord* m_pOrgRec;          //ԭʼ�����ṹ��
    CMCC_3GCDR_CG_CallEventRecord* m_pDestRec;     //�ϲ��󻰵��ṹ��

    CR99_MCDR();
    ~CR99_MCDR();

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
class CR99_CDR_Key:public CKey
{
public:
    CR99_CDR_Key(){};
    ~CR99_CDR_Key(){};

    //�Ƚ�����IMSI��ַ
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

//��CR99_SCDR�������Ӧ�������ࡣ
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

//��CR99_GCDR�������Ӧ�������ࡣ
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
//��CR99_MCDR�������Ӧ�������ࡣ
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

//�Ƚ�����GSN��ַ
BOOL Match_gsnAddress(CMCC_3GCDR_GSNAddress* p1, CMCC_3GCDR_GSNAddress* p2)
{
    assert(p1 != NULL);
    assert(p2 != NULL);
    
    //���Ż����ĵ�ַ������ͬ
    if(p1->choice == p2->choice)
    {
        switch(p1->choice)
        {
        case CMCC_3GCDR_iPBinaryAddress_chosen://�Ƚ϶�����IP��ַ
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
        case CMCC_3GCDR_iPTextRepresentedAddress_chosen://�Ƚ��ı�IP��ַ
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
