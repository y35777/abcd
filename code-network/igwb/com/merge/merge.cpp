/*****************************************************
*       Merge.cpp
******************************************************
*Copyright 2001 by Huawei CO.,LTD.
* All rights reserved.
* Compiler: Visual C++ 6.0
* Author:   Zhou Yonghui
* Date:     01-5-24
* Description:  
* 
* Side Effects: NONE
* Functions:
* Notes:    
* 
* Update: 01-5-24 ���� 05:39:16
* Date          Name          Description 
============== ================ ======================
## 01-5-24   Zhou Yonghui
*****************************************************/
#pragma warning(disable:4239)
#include "../include/base_type.h"
#include "../include/export_dll.h"
#include "merge.h"
#include "../toolbase/frame_tool.h"

/*
    ����Ĭ�������,��ϣ��Ĵ�С��Qw���е���󳤶ȡ�
    �������ȴ�ʱ��.
*/
#define DEFAULT_HASH_TABLE_LENGTH       100000
#define DEFAULT_MAX_QW_LENGTH           100000
#define DEFAULT_MAX_WAITTING_TIME       10800

CMerger::CMerger()
{
    m_uHashSize = DEFAULT_HASH_TABLE_LENGTH;
    m_uMaxQwLen = DEFAULT_MAX_QW_LENGTH;
    m_uM        = DEFAULT_MAX_WAITTING_TIME;
}

CMerger::~CMerger()
{
    CMergeDelegation* pDelegation = NULL;
    while(m_Delegation.GetSize() > 0)
    {
        pDelegation = m_Delegation.GetHead();
        m_Delegation.RemoveHead();
        delete pDelegation;
    }
}


/******************************************************
* ������:   CMerger::MergeABill
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    �ϲ�һ�Ż�����
*       CMerger������ݻ������ͣ��ѻ���ί�и�ǡ���ĺϲ�����
* ����: 
*   ����1 :CBill* pBill�����������ָ�롣
*   ����2 :PackID_t PackNum�������İ��š�
* ����ֵ: �ޡ� 
* ˵��:     ���ڴ����ʧ��ǿ�׳��쳣��
* ���¼�¼: 
*
* ����        ����          ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
void CMerger::MergeABill(CBill* pBill,PackID_t PackNum)
{
    assert(pBill != NULL);

    //�ǲ��ֻ������ȼƷѻ���
    if(!pBill->IsPartial() || pBill->IsHotBilling()) 
    {
        MoveToQok(pBill);
        return;
    }

    POSITION pos = m_Delegation.GetHeadPosition();
    CMergeDelegation* pDelegation = NULL;
    UINT4 uType = pBill->GetBillType();

    while(pos != NULL)
    {
        //ȡposλ�õ�ֵ������posָ����һ��Ԫ�ء�
        pDelegation = m_Delegation.GetNext(pos);
        if(uType == pDelegation->GetBillType())
        {
            pDelegation->MergeABill(pBill,PackNum);
            return;
        }
    }

    //��������û��ע�ᡣ
    //assert(0);
}

/******************************************************
* ������:   CMerger::MoveToQok
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ��һ��������Qok���С�
* ����: 
*   ����1 :CBill* pBill,���������ָ�롣
* ����ֵ: void 
* ˵��:     ���ڴ����ʧ��ǿ�׳��쳣��
* ���¼�¼: 
*
* ����        ����          ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
void CMerger::MoveToQok(CBill* pBill)
{
    m_Qok.AddTail(pBill);
}


/******************************************************
* ������:   RegisterBillType
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ��CMergerע�Ự�����͡�
*      CMerger���󽫶�ÿһ�ֻ�������һ���ϲ�����
* ����: 
*   ����1 :int uTypeID����������ֵ��
* ����ֵ: �ޡ�
* ˵��:     ���ڴ����ʧ�ܽ��׳��쳣��
* ���¼�¼: 
*
* ����        ����          ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
void CMerger::RegisterBillType(UINT4 uTypeID)
{
    //����һ���ϲ�����
    CMergeDelegation* pDelegation = new CMergeDelegation(this,uTypeID);
    if(!pDelegation)
    {
        throw "RegisterBillType";
    }

    pDelegation->SetM(m_uM);
    pDelegation->SetMaxQwLength(m_uMaxQwLen);
    pDelegation->Init(m_uHashSize);

    //�Ѻϲ�����׷�ӵ����е�β����
    m_Delegation.AddTail(pDelegation);
}


/******************************************************
* ������:   CMerger::Flush
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ������Qw�е����л�����Qok���С�
* ����:     �ޡ�
* ����ֵ:   �ޡ�
* ˵��:     ���ڴ����ʧ�ܽ��׳��쳣��
* ���¼�¼: 
*
* ����        ����          ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
void CMerger::Flush()
{
    POSITION pos = m_Delegation.GetHeadPosition();
    CMergeDelegation* pDelegation = NULL;

    while(pos != NULL)
    {
        pDelegation = m_Delegation.GetNext(pos);
        pDelegation->Flush();
    }
}


/******************************************************
* ������:   CMerger::GenerateBlackBill
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ���ɺڻ�����
*      ������Qw���������а�����lPackNum�Ĳ����m�Ļ�����Qok��
*      �����г��ȳ���Ԥ��ֵ��Qwβ�����ֻ�����Qok��
* ����: 
*   ����1 :PackID_t PackNum����ǰ�������š�
* ����ֵ: �ޡ�
* ˵��:     ���ڴ����ʧ�ܽ��׳��쳣��
* ���¼�¼: 
*
* ����        ����          ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
void CMerger::GenerateBlackBill(PackID_t PackNum)
{
    POSITION pos = m_Delegation.GetHeadPosition();
    CMergeDelegation* pDelegation = NULL;

    while(pos != NULL)
    {
        //ȡposλ�õ�ֵ������posָ����һ��Ԫ�ء�
        pDelegation = m_Delegation.GetNext(pos);
        pDelegation->GenerateBlackBill(PackNum);
    }
}


/******************************************************
* ������:  CMerger::SetMaxQwLength
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ����Qw���е���󳤶ȡ�
* ����: 
*   ����1 :long uLen��Qw���е���������ȡ�
* ����ֵ: �ޡ�
* ˵��: 
* ���¼�¼: 
*
* ����        ����          ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
void CMerger::SetMaxQwLength(UINT4 uLen)
{
    m_uMaxQwLen = uLen;
}


/******************************************************
* ������:   CMerger::SetM
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ����mֵ(��󻰵����Ų�)��
* ����: 
*   ����1 :long uValue
* ����ֵ: �ޡ� 
* ˵��: 
* ���¼�¼: 
*
* ����        ����          ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
void CMerger::SetM(UINT4 uValue)
{
    m_uM = uValue;
}

void CMerger::SetHashSize(UINT4 uSize)
{
    m_uHashSize = uSize;
}


/******************************************************
* ������:   CMerger::GetOKBill
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ����Qokβ����һ�Ż�����
* ����: 
* ����ֵ: CBill* �����������ָ�롣
* ˵��:     ��Qok�л����򷵻ػ���ָ�룬��û�оͷ���NULL��
*           ���ڴ����ʧ�ܽ��׳��쳣��
* ���¼�¼: 
*
* ����        ����          ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
CBill* CMerger::GetOKBill()
{
    if(m_Qok.IsEmpty())
    {
        return NULL;
    }

    CBill* pBill = m_Qok.GetHead();
    m_Qok.RemoveHead();
    
    return pBill;
}


/******************************************************
* ������:  CMergeDelegation::GetResumePID
* ����:    Zhou Yonghui
* ����:    01-11-22
* ����:    ����ǰ����ģ���ط�����������㡣
*
* ����:    uPid,�����ģ�鱣������һ���������İ��š�
* ����ֵ:  UINT4,ǰ����ģ���ط�����������㡣
* ˵��: 
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-11-22        Zhou Yonghui
******************************************************/
UINT4 CMerger::GetResumePID(UINT4 uPid)
{
    if(uPid <= 2 * m_uM)
        return 0;

    return (uPid - (uPid % m_uM) - 2 * m_uM);
}

/******************************************************
* ������:  CMergeDelegation::IsBillRemain
* ����:    Zhou Yonghui
* ����:    01-11-22
* ����:    �ж��Ƿ����л������ڴ��еȴ���
*
* ����ֵ:  ���л������ڴ��еȴ�,����TRUE,���򷵻�FALSE.
* ˵��: 
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-11-22        Zhou Yonghui
******************************************************/
BOOL CMerger::IsBillRemain()
{
    POSITION pos = m_Delegation.GetHeadPosition();
    CMergeDelegation* pDelegation = NULL;

    while(pos != NULL)
    {
        pDelegation = m_Delegation.GetNext(pos);

        if(pDelegation->GetQwLength() > 0)
            return TRUE;

        if(pDelegation->GetQtLength() > 0)
            return TRUE;
    }

    //�Ѿ�û�л������ڴ��еȴ���
    return FALSE;
}

//����Զ2002-03-06���
//����Qw�����еĵȴ���������
UINT4 CMerger::GetQwLength(UINT4 uBillTypeID)
{
	UINT4 uQwLength = 0;
	
    POSITION pos = m_Delegation.GetHeadPosition();
    CMergeDelegation* pDelegation = NULL;

    while(pos != NULL)
    {
        //ȡposλ�õ�ֵ������posָ����һ��Ԫ�ء�
        pDelegation = m_Delegation.GetNext(pos);
        if(uBillTypeID == pDelegation->GetBillType())
        {
            uQwLength = pDelegation->GetQwLength();
            break;
        }
    }

	return uQwLength;
}

//����Qt�д�ŵĵȴ����ߵĻ�������
UINT4 CMerger::GetQtLength(UINT4 uBillTypeID)
{
	UINT4 uQtLength = 0;

    POSITION pos = m_Delegation.GetHeadPosition();
    CMergeDelegation* pDelegation = NULL;

    while(pos != NULL)
    {
        //ȡposλ�õ�ֵ������posָ����һ��Ԫ�ء�
        pDelegation = m_Delegation.GetNext(pos);
        if(uBillTypeID == pDelegation->GetBillType())
        {
            uQtLength = pDelegation->GetQtLength();
            break;
        }
    }

	return uQtLength;
}

//��ȡҪ�ϲ��Ļ������͵ĸ���
int CMerger::GetBillTypeCount()
{
	return m_Delegation.GetSize();
}

//���ָ���Ļ������͵�ID,nIndex��0��ʼ
UINT4 CMerger::GetBillTypeID(int nIndex)
{
	UINT4 uBillTypeID = 0xffffffff;  //��ʼ��Ϊһ����Ч��ID

	//���nIndex���Ϸ����򷵻�һ����ЧID
	if (((UINT4)(nIndex + 1) > m_Delegation.GetSize()) || (nIndex < 0))
	{
		return uBillTypeID;
	}
    
	POSITION pos = m_Delegation.GetHeadPosition();
    CMergeDelegation* pDelegation = NULL;
	int nCounter = 0;

    while(pos != NULL)
    {
        //ȡposλ�õ�ֵ������posָ����һ��Ԫ�ء�
        pDelegation = m_Delegation.GetNext(pos);
        if(nCounter == nIndex)
        {
            uBillTypeID = pDelegation->GetBillType();
            break;
        }

		nCounter++;
    }

	return uBillTypeID;
}
//��ӽ���������Զ2002-03-06


/********** CMergeDelegetion��ʵ�� ***************/

CMergeDelegation::CMergeDelegation(CMerger* pMerger,UINT4 uBillType)
{
    assert(pMerger != NULL);

    m_uBillType = uBillType;
    m_uQwLen    = DEFAULT_MAX_QW_LENGTH;
    m_uMValue   = DEFAULT_MAX_WAITTING_TIME;
    m_uQtStep   = 0;
    m_pMerger   = pMerger;
}

CMergeDelegation::~CMergeDelegation()
{
}


/******************************************************
* ������:   CMergeDelegation::Init
* ����:    Zhou Yonghui
* ����:    01-6-7
* ����:    ��ʼ��HASH������
* ����: 
*       ����1 :long uIndexSize���µ�HASH��Ĵ�С��
* ����ֵ: void 
* ˵��:    ����ʼ���ɹ���HASH�������������ݶ�����ʧ��
*          ���ڴ����ʧ�ܣ����׳��쳣��
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-6-7        Zhou Yonghui
******************************************************/
void CMergeDelegation::Init(UINT4 uIndexSize)
{
    m_Index.InitHashTable(uIndexSize);
}


/******************************************************
* ������:   CMergeDelegation::MergeABill
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ��CMerger�����MergeABill�������ã����һ�Ż����ĺϲ���
* ����: 
*   ����1 :CBill* pBill�����������ָ�롣
*   ����2 :PackID_t PackNum�������İ��š�
* ����ֵ: �ޡ� 
* ˵��:     ���ڴ����ʧ�ܽ��׳��쳣��
* ���¼�¼: 
*
* ����        ����          ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
void CMergeDelegation::MergeABill(CBill* pBill, PackID_t PackNum)
{
    assert(pBill != NULL);
    assert(pBill->IsPartial());

    CBill* BillCur = pBill;
    CKey Key = BillCur->CreateKey();

    //pos��BillCur��Qw�ж�Ӧ�����ġ�λ��ָ�롱��
    POSITION pos = (POSITION)m_Index.Lookup(Key);

    if(pos == NULL)                         //û���ҵ�ƥ��Ļ�����
    {
        if(BillCur->IsLastPartialRec())     //ĩ�Ų��ֻ�������Qok��
        {
            MoveToQok(BillCur);
        }
        else if(BillCur->IsFirstPartialRec())//��һ�Ų��ֻ�����
        {
            CNode node;
            node.m_pBill = BillCur;
            node.m_PackNum = PackNum;
            
            //��BillCur����Qwͷ������������Qw�еġ�λ��ָ�롱��
            pos = m_Qw.AddHead(node);
            
            //��HASH�������аѻ������������롰λ��ָ�롱����������
            m_Index.SetAt(Key,(void*)pos);
        }
        else                                //�м䲿�ֻ�����
        {
            CNode node;
            node.m_pBill = BillCur;
            node.m_PackNum = PackNum - (PackNum % m_uMValue);

            //��BillCur����Qwβ������������Qw�еġ�λ��ָ�롱��
            pos = m_Qw.AddTail(node);
            m_Index.SetAt(Key,(void*)pos);
        }
        return;
    }

    CBill* BillW = m_Qw.GetAt(pos).m_pBill; //�ҵ�ƥ��Ļ�����pos���ı䣩��
    if(BillW->CanSafeMerge(BillCur))        //����ϲ���
    {
        //�ж��Ƿ��ص�
        if(BillW->IsDuplicated(BillCur))
        {            
            SInnerAlarm sAlarm;
            sAlarm.yAlarmType    = AT_EVENT;
            sAlarm.uAlarmID      = ALARM_ID_ABNORMAL_BILL;
            sAlarm.yAlarmPara[0] = ALARM_PARA_DUPLICATED_BILL;
            CFrameTools::SendAlarm(&sAlarm);
            
            delete BillCur;
            return;            
        }
        
        if(!BillW->Merge(BillCur))          //��BillCur����BillW�С�
        {
            throw "Merge bill fail.";       //���ϲ�ʧ���׳��쳣��
        }
        delete BillCur;                     //BillCur�Ѿ�����BillW�У�
                                            //��Ҫ���տռ䡣

        if(BillW->IsPDPCompleted())
        {
            if(BillW->NeedWaiting())
            {
                return;                     //�����ȴ�����������
            }

            m_Index.RemoveAt(Key,pos);      //ɾ��BillW��HASH���е�������
            m_Qw.RemoveAt(pos);             //ɾ��BillW��Qw�ж�Ӧ�Ľڵ㡣
            MoveToQok(BillW);               //�Ѻϲ���Ļ���(BillW)��Qok��
        }
    }
    else                                    //������ϲ���
    {
        if(BillCur->IsLastPartialRec())     //ĩ�Ų��ֻ�����BillW��BillCur�ֱ���Qok��
        {
            m_Index.RemoveAt(Key,pos);      //ɾ��BillW��HASH���е�������
            m_Qw.RemoveAt(pos);             //ɾ��BillW��Qw�ж�Ӧ�Ľڵ㡣
            MoveToQok(BillW);               //��BillW��Qok��
            MoveToQok(BillCur);             //��BillCur��Qok��
        }
        else                                //�м䲿�ֻ�������Qw�ȴ�����������
        {
            MoveToQok(BillW);               //BillW��Qok��
            CNode& node = m_Qw.GetAt(pos);  //BillCur�滻BillW��Qw�е�λ�á�
            node.m_pBill = BillCur;
        }
    }
}


/******************************************************
* ������:   CMergeDelegation::MoveToQok
* ����:    Zhou Yonghui
* ����:    01-6-7
* ����:    ��һ�Ż�����Qok���С�
* ����: 
*       ����1 :CBill* pBill
* ����ֵ: void 
* ˵��:    �������͵Ļ���ʹ��ͬһ����CMergerά����Qok���С�
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-6-7        Zhou Yonghui
******************************************************/
void CMergeDelegation::MoveToQok(CBill* pBill)
{
    assert(pBill != NULL);
    m_pMerger->MoveToQok(pBill);
}


/******************************************************
* ������:   CMergeDelegation::Flush
* ����:    Zhou Yonghui
* ����:    01-6-7
* ����:    ��Qw��Qt�е����л�������Ϊ�ڻ�����Qok���С�
* ����: 
* ����ֵ: void 
* ˵��:    ���ڴ����ʧ�ܣ����׳��쳣��
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-6-7        Zhou Yonghui
******************************************************/
void CMergeDelegation::Flush()
{
    POSITION pos = m_Qw.GetHeadPosition();

    while(pos != NULL)
    {
        //����posλ�õ�ֵ����posָ����һ��λ�á�
        CNode& node = m_Qw.GetNext(pos);
        MoveToQok(node.m_pBill);
    }

    //���Qok���С�
    m_Index.RemoveAll();
    //���Qw���С�
    m_Qw.RemoveAll();

    //Qt�д�ŵ���CBill*ָ�롣
    pos = m_Qt.GetHeadPosition();
    while(pos != NULL)
    {
        MoveToQok(m_Qt.GetNext(pos));
    }

    m_Qt.RemoveAll();
}


/******************************************************
* ������:   CMergeDelegation::GenerateBlackBill
* ����:    Zhou Yonghui
* ����:    01-6-7
* ����:    ���ɺڻ�����
* ����: 
*       ����1 :PackID_t PackNum����ǰ�������š�
* ����ֵ: �ޡ�
* ˵��:    ���ڴ����ʧ�ܣ����׳��쳣��
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-6-7        Zhou Yonghui
******************************************************/
void CMergeDelegation::GenerateBlackBill(PackID_t PackNum)
{
    POSITION pos = m_Qw.GetTailPosition();
    POSITION prev = NULL;

#ifdef _ONLY_FOR_TEST_

    if((PackNum % 10) == 0)
    {
        printf("Length of Qw: %d,Length of Qt:%d.\n",
            m_Qw.GetSize(),m_Qt.GetSize());
    }

#endif

    /* �ж�Qw���еĳ��ȡ� */
    while(m_uQwLen < m_Qw.GetSize())
    {
        prev = pos;
        /* ����posλ�õ�ֵ����posָ��ǰһ��λ�á� */
        CNode& node = m_Qw.GetPrev(pos);

        CKey Key = node.m_pBill->CreateKey();
        MoveToQok(node.m_pBill);
        m_Index.RemoveAt(Key,prev);     //ɾ��HASH�������еĽڵ㡣
        m_Qw.RemoveAt(prev);            //ɾ��Qw�ж�Ӧ�ڵ㡣
    }

    /* �ж��Ƿ񵽴ﳬʱ���㡣 */
    if((PackNum % m_uMValue) == 0)
    {
        /* 
         * �ѳ�ʱ����������Qt,��ʱQtһ��Ϊ�ա�
         */
        assert(m_Qt.IsEmpty());

        pos = m_Qw.GetTailPosition();
        
        while(pos != NULL)
        {
            prev = pos;
            /* ����posλ�õ�ֵ����posָ��ǰһ��λ�á� */
            CNode& node = m_Qw.GetPrev(pos);

            if(node.m_PackNum <= (PackNum - m_uMValue))
            {
                CKey Key = node.m_pBill->CreateKey();
                m_Qt.AddHead(node.m_pBill);

                m_Index.RemoveAt(Key,prev); //ɾ��HASH�������еĽڵ㡣
                m_Qw.RemoveAt(prev);        //ɾ��Qw�ж�Ӧ�ڵ㡣
            }
            else
            {
                break;
            }
        }
        
        UINT4 uLen = m_Qt.GetSize();
        
        /* 
         * ÿ�δ�Qt��ȡ���������������ֵ���뱣֤�ڵ�����һ������֮ǰ��
         * Qt�еĻ����Ѿ���ȫ����Qok��
         */
        m_uQtStep = (UINT4)(uLen / (m_uMValue - 1)) + 1;
    }
    else
    {
        /* ��Qt�еĲ��ֻ�����Qok�С� */
        if(m_Qt.GetSize() >= m_uQtStep)
        {
            for(UINT4 i = 0;i < m_uQtStep;i++)
            {
                MoveToQok(m_Qt.GetTail());
                m_Qt.RemoveTail();
            }
        }
        else
        {
            pos = m_Qt.GetHeadPosition();
            while(pos != NULL)
            {
                MoveToQok(m_Qt.GetNext(pos));
            }

            m_Qt.RemoveAll();
        }
    }
}


/******************************************************
* ������:   CMergeDelegation::SetMaxQwLength
* ����:    Zhou Yonghui
* ����:    01-6-7
* ����:    ����Qw���е���󳤶ȣ���Ϊ���ɺڻ��������ݡ�
* ����: 
*       ����1 :long uLen��Qw���е���󳤶ȡ�
* ����ֵ: void 
* ˵��: 
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-6-7        Zhou Yonghui
******************************************************/
void CMergeDelegation::SetMaxQwLength(UINT4 uLen)
{
    m_uQwLen = uLen;
}


/******************************************************
* ������:   CMergeDelegation::SetM
* ����:    Zhou Yonghui
* ����:    01-6-7
* ����:    ���������Ų��Ϊ���ɺڻ��������ݡ�
* ����: 
*       ����1 :long uValue�������Ų
* ����ֵ: �ޡ� 
* ˵��: 
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-6-7        Zhou Yonghui
******************************************************/
void CMergeDelegation::SetM(UINT4 uValue)
{
    m_uMValue = uValue;
}

UINT4 CMergeDelegation::GetBillType()
{
    return m_uBillType;
}

UINT4 CMergeDelegation::GetQtLength()
{
    return m_Qt.GetSize();
}

UINT4 CMergeDelegation::GetQwLength()
{
    return m_Qw.GetSize();
}
