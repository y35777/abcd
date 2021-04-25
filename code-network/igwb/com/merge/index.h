/*****************************************************
*       Index.h
******************************************************
*Copyright 2001 by Huawei CO.,LTD.
* All rights reserved.
* Compiler:     Visual C++ 6.0
* Author:       Zhou Yonghui
* Date:         01-5-24
* Description:  
* 
* Side Effects: NONE
* Functions:
* Notes:    
* 
* Update: 01-5-24 ���� 04:20:02
* Date              Name              Description 
============== ================ ======================
## 01-5-24       Zhou Yonghui
*****************************************************/

#ifndef _INDEX_H
#define _INDEX_H

#include "../include/type_def.h"
#include "list.h"

/*
    CHashIndex_tʵ��HASH��������������T�������������͡�
    ����T����ʵ��GetKey������operator==������
*/
template<class T>
class CHashIndex_t
{
protected:
    struct CNode
    {
        void* value;                //����ֻ���void*���͵����ݡ�
        T key;                      //��������
    };

protected:
    UINT4 m_uSize;                  //HASH��Ĵ�С��
    CList_t<CNode>* m_pTable;       //��ͷָ�롣

public:
    CHashIndex_t();
    virtual ~CHashIndex_t();

    //��ʼ��HASH��
    virtual void InitHashTable(UINT4 uSize);

    //������Key�������ֵ��
    virtual void* Lookup(T& Key);

    //��Value��Key�������
    virtual void SetAt(T& Key,void* pValue);
    //ɾ����Key�������Value��
    virtual void RemoveAt(T& Key,void* pValue);

    //����HASH��Ĵ�С��
    virtual UINT4 GetSize(); 
    //ɾ�������ӱ�ڵ㡣
    virtual void RemoveAll();
};


/******************************************************
* ������:  CHashIndex_t<T>::InitHashTable
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ��ʼ��HASH���ɹ�֮����ǰ���е�������Ϣ������ʧ��
* ����: 
*       ����1 :long nSize���±�ĳ��ȡ�
* ����ֵ: ��
* ˵��:     �ڴ����ʧ�ܽ��׳��쳣��
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
template<class T>
void CHashIndex_t<T>::InitHashTable(UINT4 uSize)
{
    if(m_pTable != NULL)
    {
        delete[] m_pTable;
    }
    m_pTable = new CList_t<CNode>[uSize];
    if(m_pTable == NULL)
    {
        throw "InitHashTable";
    }
    m_uSize = uSize;
}


/******************************************************
* ������:   CHashIndex_t<T>::Lookup
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ��HASH����������Keyƥ���ֵ��
*          ����ֵ��һ����������Qw��λ�õ�ָ�롣
* ����: 
*       ����1 :T& Key
* ����ֵ: void* 
* ˵��: �ɹ�����һ��ָ�룬ʧ�ܷ���NULL��
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
template<class T>
void* CHashIndex_t<T>::Lookup(T& Key)
{
    //��������ֵ��
    UINT4 uKey = Key.GetKey() % m_uSize;

    //������������ָ�롣
    CList_t<CNode>* p = &m_pTable[uKey];
    POSITION pos = p->GetHeadPosition();

    while(pos != NULL)
    {
        //����posλ�õ�ֵ������posָ����һ��λ�á�
        CNode& node = p->GetNext(pos);
        if(Key == node.key)     //�жϼ��Ƿ�ƥ�䡣
        {
            return node.value;  //����Qw�ڵ��λ�á�
        }
    }

    return NULL;
}


/******************************************************
* ������:   CHashIndex_t<T>::SetAt
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ��pValue��Key������һ��
* ����: 
*       ����1 :T& Key,HASH����������
*       ����2 :void* pValue,��Ҫ������HASH���ָ�롣
*       ����ģ������Լ���֤����HASH����Keyû���ظ�ֵ��
* ����ֵ: ��
* ˵��:     �ڴ����ʧ�ܽ��׳��쳣��
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
template<class T>
void CHashIndex_t<T>::SetAt(T& Key,void* pValue)
{
    //��������ֵ��
    UINT4 uKey = Key.GetKey() % m_uSize;

    //������������ָ�롣
    CList_t<CNode>* p = &m_pTable[uKey];

    //�������������½ڵ㡣
    CNode node;
    node.key = Key;
    node.value = pValue;
    p->AddTail(node);
}


/******************************************************
* ������:   CHashIndex_t<T>::RemoveAt
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ��HASH����ɾ�����Keyƥ���ֵ��
* ����: 
*       ����1 :T& Key
*       ����2 :void* pValue
* ����ֵ: ��
* ˵��: �����߱��뱣֤Key���������д��ڡ�
*       ��û�з�����Keyƥ���ֵ��������assert���ԡ�
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
template<class T>
void CHashIndex_t<T>::RemoveAt(T& Key,void* pValue)
{
    //��������ֵ��
    UINT4 uKey = Key.GetKey() % m_uSize;

    //������������ָ�롣
    CList_t<CNode>* p = &m_pTable[uKey];
    POSITION pos = p->GetHeadPosition();
    POSITION prev = NULL;

    while(pos != NULL)
    {
        prev = pos;
        //����posλ�õ�ֵ������posָ����һ��λ�á�
        CNode& node = p->GetNext(pos);
        if(Key == node.key)
        {
            assert(node.value == pValue);
            p->RemoveAt(prev);
            return;
        }
    }

    //û���ҵ�ƥ��������ڵ㡣
    assert(0);
}

template<class T>
UINT4 CHashIndex_t<T>::GetSize()
{
    return m_uSize;
}


/******************************************************
* ������:   CHashIndex_t<T>::RemoveAll
* ����:    Zhou Yonghui
* ����:    01-6-7
* ����:    ɾ��HASH��������������Ϣ�����ǲ�������HASH��ռ�õĿռ䡣
* ����: 
* ����ֵ: void
* ˵��:    ���ڴ����ʧ�ܣ����׳��쳣��
* ���¼�¼: 
*
* ����            ����                  ����
* ========== ================== ======================
*01-6-7        Zhou Yonghui
******************************************************/
template<class T>
void CHashIndex_t<T>::RemoveAll()
{
    UINT4 uCount = GetSize();
    CList_t<CNode>* p = NULL;
    for(UINT4 i = 0;i < uCount;i++)
    {
        p = &m_pTable[i];
        p->RemoveAll();
    }
}

template<class T>
CHashIndex_t<T>::CHashIndex_t()
{
    m_pTable = NULL;
    m_uSize = 0;
}

template<class T>
CHashIndex_t<T>::~CHashIndex_t()
{
    if(m_pTable)
    {
        RemoveAll();
        delete[] m_pTable;
        m_pTable = NULL;
    }
}

#endif
