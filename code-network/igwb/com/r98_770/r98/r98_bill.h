#ifndef     _GPRS_BILL_H
#define     _GPRS_BILL_H

#include "../../include/type_def.h"
#include <assert.h>
#include "../../merge/bill.h"
#include "../asn1_r98/CDRF_2GCDR_Org.h"
#include "../asn1_r98/CDRF_2GCDR_Fin.h"
#include "../asn1_r98/oss_asn1_r98.h"

#define GPRS_S_CDR     Org_CallEventRecordType_sgsnPDPRecord
#define GPRS_G_CDR     Org_CallEventRecordType_ggsnPDPRecord
#define GPRS_M_CDR     Org_CallEventRecordType_sgsnMMRecord
#define GPRS_SMO_CDR   Org_CallEventRecordType_sgsnSMORecord
#define GPRS_SMT_CDR   Org_CallEventRecordType_sgsnSMTRecord

#define MAX_MERGED_BILLS     255          //���Ļ����ϲ�����


//CGPRS_BillParser�̳���CBillParser�࣬����GPRS�������ͺͻ����ṹ������
class CGPRS_BillParser:public CBillParser
{


protected:
    CBill* m_pBill;

public:
    CGPRS_BillParser();
    virtual ~CGPRS_BillParser();

    BOOL ParseARecord(char* pBuf,UINT4 uBufLen);//����pBuf����һ����������
    CBill* GetBillRecord();                     //���ػ�����¼����

    UINT4 GetBillTypeCount();                   //���ػ�����������
    UINT4 GetBillTypeID(UINT4 uIndex);          //�����������ػ�������ID��
    void RegisterBillType(CMerger* pMerger);    //��CMerger������ע�Ự�����͡�
	//���ָ���Ļ�������ID�Ļ�����������
	void GetBillTypeDesc(UINT4 uBillTypeID, char* szDesc);
};

/****************************************************************************
ģ��T����������µĶ��壺
    struct T
    {
		T* next;
		...
	};
****************************************************************************/
//���ָ�������е����нڵ�
template <class T>
void ClearList(T*& pHead)
{
	T* pTmp = NULL;

	while (NULL != pHead)
	{
		pTmp = pHead->next;
		delete pHead;
		pHead = pTmp;
	}
}

//���������õ�ָ�������еĽڵ�����Լ����һ���ڵ�
template <class T>
int TravelList(const T* pHead, T*& pTail)
{
	int nCount = 0;
	T* pTmp = (T* )pHead;

	pTail = NULL;

	while (NULL != pTmp)
	{
		if (NULL == pTmp->next)
		{
			pTail = pTmp;
		}

		pTmp = pTmp->next;
		nCount++;
	}

	return nCount;
}

//��¡һ������
template <class T>
void CloneList(const T* pSrcHead, T*& pDstHead)
{
	T* pNode = NULL;
	T* pSrcTmp = (T* )pSrcHead;
	T* pDstTmp = NULL;

	pDstHead = NULL;

	while (NULL != pSrcTmp)
	{
		pNode = new T;
		if (NULL == pNode)
		{
			throw "Fail to new T when clone list.";
		}
		memcpy(pNode, pSrcTmp, sizeof(T));
		pNode->next = NULL;
		if (NULL == pDstHead)
		{
			pDstHead = pDstTmp = pNode;
		}
		else
		{
			pDstTmp->next = pNode;
			pDstTmp = pNode;
		}

		pSrcTmp = pSrcTmp->next;
	}
}        

//�����װ�˲���OSS���빤�ߵ�130�汾������һЩ�������ݲ�������
class CGPRS_130_OSS_CDR : public CBill
{
	
protected:
    BOOL m_bDataPrepared;  //��ʾ�Ƿ��Ѿ���ԭʼ��������ת��Ϊ���ջ�������

public:
	//��ȡ����ʱ�̼��ʱ����������Ϊ��λ
	Fin_CallDuration GetDuration(const Fin_TimeStamp* pSmallTime, 
								 const Org_TimeStamp* pLargeTime);
	//��¡Org_ManagementExtensions_������
	void CloneExtensionsList(const Org_ManagementExtensions_* pSrc, 
								   Org_ManagementExtensions_*& pDst);
	//ɾ��Org_ManagementExtensions_������
	void ClearExtensionsList(Org_ManagementExtensions_*& pExtension);

protected:
	CGPRS_130_OSS_CDR();
	virtual ~CGPRS_130_OSS_CDR();

    //�ж��Ƿ����ꡣ
    BOOL isLeapYear(int nYear);
    //�������£����ظ��µ�������
    int GetDaysOfMonth(int nYear, int nMonth);
};

//S-CDR������
class CGPRS_130_OSS_SCDR : public CGPRS_130_OSS_CDR
{
#ifdef _ONLY_TEST_
	friend class CSCDRManipulate;
#endif

public:
	CGPRS_130_OSS_SCDR();
	virtual ~CGPRS_130_OSS_SCDR();

    BOOL Init(char* pBuf,UINT4 uLen); //ʹ�û�����¼��ʼ����������

    BOOL CanSafeMerge(CBill* pBill);//�жϵ�ǰ�����Ƿ�������pBill�ϲ���
    BOOL Merge(CBill* pBill);       //��pBill���뵱ǰ�����С�
    BOOL IsPDPCompleted();          //�ж�PDP�Ƿ��Ѿ�������
    BOOL NeedWaiting();             //�ж��Ƿ���Ҫ�����ȴ�����������

    char* GetContents();            //�������ո�ʽ�Ļ�����¼

    BOOL IsPartial();               //�ж��Ƿ񲿷ֻ�����
    BOOL IsFirstPartialRec();       //�ж��Ƿ�PDP�ĵ�һ�Ų��ֻ�����
    BOOL IsLastPartialRec();        //�ж��Ƿ�PDP��ĩ�Ų��ֻ�����
    BOOL IsDuplicated(CBill* pBill);  //�ж��Ƿ��ظ�����    

    UINT4 GetBillType();            //���ػ�������IDֵ��

    CKey CreateKey();               //������������

	int GetQosLen();						//���listOfTrafficVolumes����ĳ���
	int GetExtensionLen();					//���recordExtensions����ĳ���
	//���listOfTrafficVolumes�����ͷ��β
	void GetQosList(Fin__seqof2_*& pHead, Fin__seqof2_*& pTail);
	//���recordExtensions�����ͷ��β
	void GetExtensionList(Fin_ManagementExtensions_*& pHead, 
						  Fin_ManagementExtensions_*& pTail);
	//ȥ��listOfTrafficVolumes���У��ڱ��������ϲ����������������
	void RemoveQosList();					
	//ȥ��recordExtensions���У��ڱ��������ϲ����������������
	void RemoveExtensionList();				
	const Org_SGSNPDPRecord* GetOrgRec();	//��ȡԭʼ�����ṹ��ָ��
#ifdef _ONLY_TEST_
public:
#else
protected:
#endif

    BOOL PrepareData();         //��ԭʼ������¼ת��Ϊ�������ո�ʽ�Ļ�����¼


	//���������βָ���Լ�����ĳ���
    Fin__seqof2_*               m_pQosTail;            
	int                         m_nQosLen;
    Fin_RecordNumberList_*      m_pSequenceTail;
	int                         m_nSequenceLen;
    Fin_ManagementExtensions_*  m_pExtensionTail;
	int                         m_nExtensionLen;

    Org_SGSNPDPRecord*   m_pOrgRec;            //ԭʼ������
    Fin_CallEventRecord* m_pDestEventRec;      //�ϲ��󻰵���
	Fin_SGSNPDPRecord*   m_pDestRec;
};

//G-CDR������
class CGPRS_130_OSS_GCDR : public CGPRS_130_OSS_CDR
{
#ifdef _ONLY_TEST_
	friend class CGCDRManipulate;
#endif
public:
	CGPRS_130_OSS_GCDR();
	virtual ~CGPRS_130_OSS_GCDR();

    BOOL Init(char* pBuf,UINT4 uLen); //ʹ�û�����¼��ʼ����������

    BOOL CanSafeMerge(CBill* pBill);//�жϵ�ǰ�����Ƿ�������pBill�ϲ���
    BOOL Merge(CBill* pBill);       //��pBill���뵱ǰ�����С�
    BOOL IsPDPCompleted();          //�ж�PDP�Ѿ�������
    BOOL NeedWaiting();             //�ж��Ƿ���Ҫ�����ȴ�����������

    char* GetContents();            //�������ո�ʽ�Ļ�����¼

    BOOL IsPartial();               //�ж��Ƿ񲿷ֻ�����
    BOOL IsFirstPartialRec();       //�ж��Ƿ�PDP�ĵ�һ�Ų��ֻ�����
    BOOL IsLastPartialRec();        //�ж��Ƿ�PDP��ĩ�Ų��ֻ�����
    BOOL IsDuplicated(CBill* pBill);   //�ж��Ƿ��ظ�����
    
    UINT4 GetBillType();            //���ػ�������IDֵ��

    CKey CreateKey();               //������������

	int GetSGSNLen();				//���sgsnAddress����ĳ���
	int GetPDPLen();				//���remotePDPAddress����ĳ���
	int GetQosLen();				//���listOfTrafficVolumes����ĳ���
	int GetExtensionLen();			//���recordExtensions����ĳ���
	//���sgsnAddress�����ͷ��β
	void GetSGSNList(Fin__seqof1_*& pHead, Fin__seqof1_*& pTail);
	//���remotePDPAddress�����ͷ��β
	void GetPDPList(Fin__seqof4*& pHead, Fin__seqof4*& pTail);
	//���listOfTrafficVolumes�����ͷ��β
	void GetQosList(Fin__seqof2_*& pHead, Fin__seqof2_*& pTail);
	//���recordExtensions�����ͷ��β
	void GetExtensionList(Fin_ManagementExtensions_*& pHead, 
						  Fin_ManagementExtensions_*& pTail);
	//ȥ��sgsnAddress���У��ڱ��������ϲ����������������
	void RemoveSGSNList();
	//ȥ��remotePDPAddress���У��ڱ��������ϲ����������������
	void RemovePDPList();
	//ȥ��listOfTrafficVolumes���У��ڱ��������ϲ����������������
	void RemoveQosList();
	//ȥ��recordExtensions���У��ڱ��������ϲ����������������
	void RemoveExtensionList();
	const Org_GGSNPDPRecord* GetOrgRec();	//��ȡԭʼ�����ṹ��ָ��
#ifdef _ONLY_TEST_
public:
#else
protected:
#endif
    BOOL PrepareData();			//��ԭʼ������¼ת��Ϊ�������ո�ʽ�Ļ�����¼

//protected:
	//���������βָ���Լ�����ĳ���
	Fin__seqof1_*				m_pSGSNTail;
	int							m_nSGSNLen;
	Fin__seqof4*				m_pPDPTail;
	int							m_nPDPLen;
	Fin__seqof2_*				m_pQosTail;
	int							m_nQosLen;
	Fin_RecordNumberList_*		m_pSequenceTail;
	int							m_nSequenceLen;
	Fin_ManagementExtensions_*	m_pExtensionTail;
	int							m_nExtensionLen;

	Org_GGSNPDPRecord*   m_pOrgRec;				//ԭʼ����
    Fin_CallEventRecord* m_pDestEventRec;       //�ϲ��󻰵���
	Fin_GGSNPDPRecord*   m_pDestRec;
};

//��CGPRS_130_OSS_SCDR�������Ӧ�������ࡣ
class CGPRS_130_OSS_SCDR_Key : public CKey
{
public:
    Org_GSNAddress  m_sgsnAddress;
    Org_GSNAddress  m_ggsnAddressUsed;
    Org_ChargingID  m_chargingID;

public:
    CGPRS_130_OSS_SCDR_Key();
    virtual ~CGPRS_130_OSS_SCDR_Key();

	void SetSGSNAddress(const Org_GSNAddress& sgsnAddress);
	void SetGGSNAddress(const Org_GSNAddress& ggsnAddress);
	void SetChargingID(const Org_ChargingID chargingID);
	const Org_GSNAddress& GetSGSNAddress();
	const Org_GSNAddress& GetGGSNAddress();
	Org_ChargingID GetChargingID();

    BOOL IsMatch(CKey* pKey);
    UINT4 GetKey();
};

//��CGPRS_130_OSS_GCDR�������Ӧ�������ࡣ
class CGPRS_130_OSS_GCDR_Key : public CKey
{
public:
    Org_GSNAddress  m_ggsnAddress;
    Org_ChargingID  m_chargingID;

public:
    CGPRS_130_OSS_GCDR_Key();
    virtual ~CGPRS_130_OSS_GCDR_Key();

	void SetGGSNAddress(const Org_GSNAddress& sgsnAddress);
	void SetChargingID(const Org_ChargingID chargingID);
	const Org_GSNAddress& GetGGSNAddress();
	Org_ChargingID GetChargingID();

    BOOL IsMatch(CKey* pKey);
    UINT4 GetKey();
};

//�Ƚ�����GSN��ַ
BOOL Match_gsnAddress(const Org_GSNAddress* p1, const Org_GSNAddress* p2);

#endif
