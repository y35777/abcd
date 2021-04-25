
#ifndef _MERGE_H
#define _MERGE_H

#include <assert.h>
#include "list.h"
#include "bill.h"
#include "index.h"

class CMergeDelegation
{
protected:
    //Qw���нڵ�Ľṹ��
    struct CNode
    {
        CBill*  m_pBill;      //���������ָ�롣
        PackID_t m_PackNum;   //������Ӧ�İ��š�
    };

protected:
    //ָ��ϲ������ָ�룬���а�����Qok���С�
    CMerger* m_pMerger;

    //Qw���е�ͷָ�롣
    CList_t<CNode> m_Qw;

    //��ų�ʱ�����Ķ��С�
    CList_t<CBill*> m_Qt;

    //������
    CHashIndex_t<CKey> m_Index;

    //Qw�ĳ��ȡ�
    UINT4 m_uQwLen;
    
    //ÿ�δ�Qt������ȡ������������
    UINT4 m_uQtStep;

    //mֵ��
    UINT4 m_uMValue;

    //����ϲ��Ļ��������͡�
    UINT4 m_uBillType;

public:
    CMergeDelegation(CMerger* pMerger,UINT4 uBillType);
    virtual ~CMergeDelegation();

    //��ʼ�����ϲ�����
    virtual void Init(UINT4 uIndexSize);

    //���غϲ��Ļ������͡�
    virtual UINT4 GetBillType();

    //�ϲ�һ��������
    virtual void MergeABill(CBill* pBill, PackID_t lPackNum);

    //��һ����������Qok�С�
    virtual void MoveToQok(CBill* pBill);

    //��Qw�еĻ���ȫ������Qok�С�
    virtual void Flush();

    //�����ڻ�����
    virtual void GenerateBlackBill(PackID_t lPackNum);

    //����Qw���е���󳤶ȣ���Ϊ�����ڻ��������ݡ�
    virtual void SetMaxQwLength(UINT4 uLen);

    //����Mֵ����Ϊ�����ڻ��������ݡ�
    virtual void SetM(UINT4 uValue);

    //����Qw�����еĵȴ���������
    virtual UINT4 GetQwLength();

    //����Qt�д�ŵĵȴ����ߵĻ�������
    virtual UINT4 GetQtLength();
};

class IGWB_Svc_Export CMerger
{
protected:
    //��ɺϲ����ܵĶ����б�
    CList_t<CMergeDelegation*> m_Delegation;

    //Qok���С�
    CList_t<CBill*> m_Qok;

    //HASH��Ĵ�С��
    UINT4 m_uHashSize;

    //Qw���е���󳤶ȡ�
    UINT4 m_uMaxQwLen;

    //�����Ų
    UINT4 m_uM;

public:
    CMerger();
    ~CMerger();

    //�ϲ�һ��������
    virtual void MergeABill(CBill* pBill, PackID_t lPackNum);

    //��һ����������Qok�С�
    virtual void MoveToQok(CBill* pBill);

    //��Qw�еĻ���ȫ������Qok�С�
    virtual void Flush();

    //�����ڻ�����
    virtual void GenerateBlackBill(PackID_t lPackNum);

    /*
        ע�Ự�����͡�
        �ú���������SetM��SetMaxQwLength��SetHashSize����֮����á�
    */
    virtual void RegisterBillType(UINT4 uTypeID);

    //����Qw���е���󳤶ȣ���Ϊ�����ڻ��������ݡ�
    virtual void SetMaxQwLength(UINT4 uLen);

    //���������Ų��Ϊ�����ڻ��������ݡ�
    virtual void SetM(UINT4 uValue); 

    //����HASH������Ĵ�С��
    virtual void SetHashSize(UINT4 uSize);
    
    //��Qok�з���һ����������QokΪ�գ��򷵻�NULL��
    virtual CBill* GetOKBill();

    //����ָ�����ʼ���š�
    virtual UINT4 GetResumePID(UINT4 uPid);

    //�ж��Ƿ����л������ڴ��еȴ���
    virtual BOOL IsBillRemain();

	//����Զ2002-03-06���
    //����Qw�����еĵȴ���������
    virtual UINT4 GetQwLength(UINT4 uBillTypeID);

    //����Qt�д�ŵĵȴ����ߵĻ�������
    virtual UINT4 GetQtLength(UINT4 uBillTypeID);

	//��ȡҪ�ϲ��Ļ������͵ĸ���
	int GetBillTypeCount();

	//���ָ���Ļ������͵�ID,nIndex��0��ʼ
	UINT4 GetBillTypeID(int nIndex);
	//��ӽ���������Զ2002-03-06
};

#endif

