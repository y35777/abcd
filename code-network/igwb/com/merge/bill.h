
#ifndef BILL_H
#define BILL_H
#pragma warning(disable:4100)

#include <iostream.h>
#include "../include/type_def.h"
#include "../utility/config.h"

typedef UINT4           PackID_t;


/*
    HASH��ʹ��CKey�������������
    ���л����඼�����������̳��Լ������࣬
    ��ʵ��GetKey��operator==������
    CKey�������һ��CKey��������ָ�룬���ʵ�ʵļ��㹤����

    HASH��ʹ��CKey������Ϊ������������CKey�����ָ�롣
    
    ����ָ��Ķ�����ʹ��copy��ʽ����ֵ��
    ����붨��copy���캯���͸�ֵ�����(=)��
*/

class IGWB_Svc_Export CKey
{
public:

    //ʵ����ɼ���Ķ���ָ�롣
    CKey* m_pDelegation;

public:

    //��������ֵ��
    virtual UINT4 GetKey();

    //�ж�����CKey�����Ƿ���ͬ��
    virtual BOOL operator==(CKey& Key);

    virtual BOOL IsMatch(CKey* pKey);

    CKey();

    /*
        Copy���캯������CKey������Copy�ķ�ʽ��ֵʱ��
        ����m_pDelegationָ�롣
    */
    CKey(const CKey& key);

    virtual ~CKey();

    //��ֵ���������CKey���󱻸�ֵʱ���ڴ���m_pDelegationָ�롣
    void operator=(CKey& key);
};

/*
    CBill����һ�������࣬���ṩ���������Ľӿڣ�
    ���е�ʵ�ֶ���������������ɡ�
*/
class IGWB_Svc_Export CBill
{
public:
    CBill();

    virtual ~CBill();

    //ʹ�û�����¼��ʼ����������
    virtual BOOL Init(char* pBuf,UINT4 uLen) = 0;

    //�жϵ�ǰ�����Ƿ�������pBill�ϲ���
    virtual BOOL CanSafeMerge(CBill* pBill) = 0;

    //��pBill���뵱ǰ�����С�
    virtual BOOL Merge(CBill* pBill) = 0;

    //�ж�PDP�Ѿ�������
    virtual BOOL IsPDPCompleted() = 0;

    //�ж��Ƿ���Ҫ�����ȴ�����������
    virtual BOOL NeedWaiting() = 0;

    //���ػ����ļ�¼�ṹ��
    virtual char* GetContents() = 0;

    //�ж��Ƿ񲿷ֻ�����
    virtual BOOL IsPartial() = 0;
    
    //�ж��Ƿ����ȼƷѻ���
    virtual BOOL IsHotBilling()
    {
        return FALSE;
    };

    //�ж��Ƿ�PDP�ĵ�һ�Ų��ֻ�����
    virtual BOOL IsFirstPartialRec() = 0;

    //�ж��Ƿ�PDP��ĩ�Ų��ֻ�����
    virtual BOOL IsLastPartialRec() = 0;

    //�ж��Ƿ��ص�
    virtual BOOL IsDuplicated(CBill* pBill) = 0;    

    //���ػ�������IDֵ��
    virtual UINT4 GetBillType() = 0;

    //������������
    virtual CKey CreateKey() = 0;

    //��ȡ�����ĳ��ȡ�
    virtual INT4 GetLength();
	
	//Z30971�����Ƿ�֧����ʱ�ƽӿ�
	//���úϲ�Duration�Ƿ�֧����ʱ��
	virtual void SetDSTFlag(BOOL bFlag = FALSE);
	
	//Added by ZhengYuqun 2003-11-04 SWPD02238
    virtual void SetMaxDuration(UINT4 uMaxDuration);

protected:
	BOOL m_bDSTFlg;//��ʱ�Ʊ�־
    UINT4 m_uMaxDuration; //Added by ZhengYuqun 2003-11-04 SPWD02238
        	
};

/*
    CBillParser���������ͺͻ����ṹ��ϸ�ڡ�
    ��Բ�ͬ�Ļ������ͣ��������̳�CBillParser�����࣬
    ʵ�ֻ������͵Ĺ���ͻ����ṹ�Ľ�����
*/
class CMerger;

class IGWB_Svc_Export CBillParser
{
protected:
    CBill* m_pBill;

public:
    CBillParser();

    virtual ~CBillParser();

    //����pBuf����һ����������
    virtual BOOL ParseARecord(char* pBuf,UINT4 uBufLen) = 0;

    //���ػ�����¼����
    virtual CBill* GetBillRecord() = 0;

    //���ػ�����������
    virtual UINT4 GetBillTypeCount() = 0;

    //�����������ػ�������ID��
    virtual UINT4 GetBillTypeID(UINT4 uIndex) = 0;

    //��CMerger������ע�Ự�����͡�
    virtual void RegisterBillType(CMerger* pMerger) = 0;

	//����Զ2002-03-06���
	//���ָ���Ļ�������ID�Ļ�����������
	virtual void GetBillTypeDesc(UINT4 uBillTypeID, char* szDesc) = 0;
	//��ӽ���������Զ2002-03-06
};

#endif

