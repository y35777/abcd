/*****************************************************
*       List.h
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
* Update: 01-5-24 ���� 05:39:06
* Date              Name              Description 
============== ================ ======================
## 01-5-24       Zhou Yonghui
*****************************************************/

#ifndef _LIST_H
#define _LIST_H

#include <iostream.h>
#include <assert.h>

#include "../include/type_def.h"

typedef void*           POSITION;

/*
*˫�������֧࣬������ڵ�Ĳ��롢ɾ��������ı���������
*CList_t����һ��ģ���࣬��������T������ڵ��д�ŵ��������͡�
*����T����ʵ�ָ�ֵ�������������
*
*�������ڵ���CList_t��ķ���֮ǰ�����뱣֤POSITION��������Ч�ġ�
*/
template<class T>
class CList_t
{
protected:
    struct CList_Node
    {
        CList_Node* m_pNext, *m_pPrev;  //����ָ�롣
        T  Value;                       //���ݳ�Ա��
    };

protected:
    CList_Node* m_pHead;                //����ͷָ�롣
    CList_Node*  m_pTail;               //����βָ�롣
    UINT4 m_uSize;                      //����ĳ��ȡ�

public:
    CList_t();
    ~CList_t();

    POSITION GetHeadPosition();         //���ͷ�ڵ��λ�á�
    POSITION GetTailPosition();         //���β�ڵ��λ�á�

    T& GetHead();                       //ȡͷ�ڵ��ֵ��
    T& GetTail();                       //ȡβ�ڵ��ֵ��

    T& GetNext(POSITION& pos);
    T& GetPrev(POSITION& pos);
    T& GetAt(POSITION& pos );           //���ָ��λ�õ�Ԫ�ء�

                                        //�滻ָ��λ�õ�Ԫ�ء�
    void SetAt(POSITION& pos ,T Value);

    void RemoveHead();                  //ɾ��ͷ�ڵ㡣
    void RemoveTail();                  //ɾ��β�ڵ㡣
    void RemoveAt(POSITION& pos);       //ɾ��posָ��Ľڵ㡣
    void RemoveAll();                   //ɾ�����нڵ㡣

    POSITION AddHead(T& Value);         //�������ͷ�����ӽڵ㡣
    POSITION AddTail(T& Value);         //�������β�����ӽڵ㡣
    BOOL IsEmpty();                     //�ж������Ƿ�Ϊ�ա�

                                        //��ָ��λ��֮ǰ����Ԫ�ء�
    POSITION InsertBefore(POSITION pos,T Value);
                                        //��ָ��λ��֮�����Ԫ�ء�
    POSITION InsertAfter(POSITION pos,T Value);
    UINT4 GetSize();                    //��������Ľڵ�����
};


template<class T> CList_t<T>::CList_t()
{
    m_pHead = m_pTail = NULL;
    m_uSize = 0;
}

template<class T> CList_t<T>::~CList_t()
{
    RemoveAll();
}


/******************************************************
* ������:   CList_t<T>::GetHeadPosition
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ��ȡ����ͷ�ڵ��λ�á�
* ����: 
* ����ֵ: POSITION����������ͷ�ڵ�ġ�λ��ָ�롱��
* ˵��:     ������Ϊ�գ��򷵻�NULL��
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
template<class T>
POSITION CList_t<T>::GetHeadPosition()
{
    return  (POSITION)m_pHead;
}

template<class T>
POSITION CList_t<T>::GetTailPosition()
{
    return  (POSITION)m_pTail;
}


/******************************************************
* ������:   CList_t<T>::GetHead
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ��ȡ����ͷ�ڵ�洢��ֵ�����á�
* ����: 
* ����ֵ:T& ������洢��ֵ�����á�
* ˵��:     ������Ϊ�ս�����assert���ԡ�
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
template<class T>
T& CList_t<T>::GetHead()
{
    assert(m_pHead != NULL);
    return m_pHead->Value;
};

template<class T>
T& CList_t<T>::GetTail()
{
    assert(m_pTail != NULL);
    return m_pTail->Value;
};


/******************************************************
* ������:   CList_t<T>::GetNext
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ��ȡpos��ָλ�õ�ֵ�����ã�ͬʱpos��ָ����һ��λ�á�
* ����: 
*       ����1 :POSITION& pos������ڵ�λ�õ�ָ�롣
* ����ֵ: T& 
* ˵��:     ��posΪ�գ�������assert���ԡ�
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
template<class T>
T& CList_t<T>::GetNext(POSITION& pos)
{
    assert(pos != NULL);

    CList_Node* pNode = (CList_Node*)pos;
    pos = (POSITION)pNode->m_pNext;
    return pNode->Value;
}

/******************************************************
* ������:   CList_t<T>::GetPrev
* ����:    Zhou Yonghui
* ����:    01-6-7
* ����:    ��ȡpos��ָλ�õ�ֵ�����ã�ͬʱpos��ָ��ǰһ��λ�á�
* ����: 
*       ����1 :POSITION& pos������ڵ�λ�õ�ָ�롣
* ����ֵ: T& 
* ˵��:     ��posΪ�գ�������assert���ԡ�
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-6-7        Zhou Yonghui
******************************************************/
template<class T>
T& CList_t<T>::GetPrev(POSITION& pos)
{
    assert(pos != NULL);

    CList_Node* pNode = (CList_Node*)pos;
    pos = (POSITION)pNode->m_pPrev;
    return pNode->Value;
}


/******************************************************
* ������:   CList_t<T>::GetAt
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ��ȡpos��ָλ�õ�ֵ�����ã�pos��ֵ����ı䡣
* ����: 
*       ����1 :POSITION& pos������ڵ�λ�õ�ָ�롣
* ����ֵ:T& 
* ˵��:     ��posΪ�գ�������assert���ԡ�
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
template<class T>
T& CList_t<T>::GetAt(POSITION& pos)
{
    assert(pos != NULL);

    CList_Node* pNode = (CList_Node*)pos;
    return pNode->Value;
}

/******************************************************
* ������:   CList_t<T>::SetAt
* ����:    Zhou Yonghui
* ����:    01-6-7
* ����:    ��Value���posλ�õ�ֵ��pos����ı䡣
* ����: 
*       ����1 :POSITION& pos
*       ����2 :T Value���µ�ֵ��
* ����ֵ: void 
* ˵��:     ��posΪ�գ�������assert���ԡ�
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-6-7        Zhou Yonghui
******************************************************/
template<class T>
void CList_t<T>::SetAt(POSITION& pos ,T Value)
{
    assert(pos != NULL);

    CList_Node* pNode = (CList_Node*)pos;
    pNode->Value = Value;
}


/******************************************************
* ������:   CList_t<T>::RemoveHead
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ɾ������ͷ���ڵ㡣
* ����: 
* ����ֵ: �ޡ�
* ˵��:     ������Ϊ�ս�����assert���ԡ�
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
template<class T>
void CList_t<T>::RemoveHead()
{
    assert(m_pHead != NULL);

    if(m_pHead != m_pTail)
    {
        CList_Node* pNode = m_pHead;
        m_pHead = m_pHead->m_pNext;
        m_pHead->m_pPrev = NULL;
        delete pNode;
    }
    else
    {
        delete m_pHead;
        m_pHead = m_pTail = NULL;
    }
    m_uSize--;
}


/******************************************************
* ������:   CList_t<T>::RemoveTail
* ����:    Zhou Yonghui
* ����:    01-6-7
* ����:    ɾ������β���ڵ㡣
* ����: 
* ����ֵ: void 
* ˵��: ������Ϊ�ս�����assert���ԡ�
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-6-7        Zhou Yonghui
******************************************************/
template<class T>
void CList_t<T>::RemoveTail()
{
    assert(m_pTail != NULL);

    if(m_pHead != m_pTail)
    {
        CList_Node* pNode = m_pTail;
        m_pTail = m_pTail->m_pPrev;
        m_pTail->m_pNext = NULL;
        delete pNode;
    }
    else
    {
        delete m_pTail;
        m_pHead = m_pTail = NULL;
    }
    m_uSize--;
}


/******************************************************
* ������:   CList_t<T>::RemoveAt
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ɾ��posָ��Ľڵ㡣
* ����: 
*       ����1 :POSITION& pos����Ҫɾ���Ľڵ��λ��ָ�롣
* ����ֵ: �ޡ�
* ˵��:     ��posΪ�գ�������assert���ԡ�
* ���¼�¼: 
*
* ����            ����                  ����
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
template<class T>
void CList_t<T>::RemoveAt(POSITION& pos)
{
    assert(pos != NULL);

    CList_Node* pNode = (CList_Node*)pos;
    if(pNode == m_pHead)
    {
        RemoveHead();
    }
    else if(pNode == m_pTail)
    {
        RemoveTail();
    }
    else
    {
        pNode->m_pNext->m_pPrev = pNode->m_pPrev;
        pNode->m_pPrev->m_pNext = pNode->m_pNext;
        delete pNode;
        m_uSize--;
    }
}


/******************************************************
* ������:   CList_t<T>::RemoveAll
* ����:    Zhou Yonghui
* ����:    01-6-7
* ����:    ɾ�������������ڵ㡣
* ����: 
* ����ֵ: void 
* ˵��: 
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-6-7        Zhou Yonghui
******************************************************/
template<class T>
void CList_t<T>::RemoveAll()
{
    unsigned int nCount = GetSize();
    for(unsigned int i = 0;i < nCount;i++)
    {
        RemoveHead();
    }
    m_uSize = 0;
}


/******************************************************
* ������:   CList_t<T>::AddHead
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    �������ͷ�ڵ�֮ǰ����һ���½ڵ㣬
*          �����½ڵ��ֵ����ΪValue��
* ����: 
*       ����1 :T& Value
* ����ֵ: POSITION���½ڵ��λ��ָ�롣
* ˵��:     ���ڴ����ʧ�ܽ��׳��쳣��
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
template<class T>
POSITION CList_t<T>::AddHead(T& Value)
{
    CList_Node* pNode = new CList_Node;
    if(!pNode)
    {
        throw "AddHead";
    }

    pNode->Value = Value;
    pNode->m_pPrev = NULL;
    pNode->m_pNext = m_pHead;

    if(m_pHead != NULL)
    {
        m_pHead->m_pPrev = pNode;
        m_pHead = pNode;
    }
    else
    {
        m_pTail = m_pHead = pNode;
    }

    m_uSize++;

    return (POSITION)pNode;
}


/******************************************************
* ������:   CList_t<T>::AddTail
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    �������β�ڵ�֮�����һ���½ڵ㣬
*          �����½ڵ��ֵ����ΪValue��
* ����: 
*       ����1 :T& Value
* ����ֵ: POSITION���½ڵ��λ��ָ�롣
* ˵��:     ���ڴ����ʧ�ܽ��׳��쳣��
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
template<class T>
POSITION CList_t<T>::AddTail(T& Value)
{
    CList_Node* pNode = new CList_Node;
    if(!pNode)
    {
        throw "AddTail";
    }

    pNode->Value = Value;
    pNode->m_pNext = NULL;
    pNode->m_pPrev = m_pTail;

    if(m_pTail != NULL)
    {
        m_pTail->m_pNext = pNode;
        m_pTail = pNode;
    }
    else
    {
        m_pHead = m_pTail = pNode;
    }

    m_uSize++;

    return (POSITION)pNode;
}

template<class T>
BOOL CList_t<T>::IsEmpty()
{
    return (m_uSize == 0);
}


/******************************************************
* ������:   CList_t<T>::InsertBefore
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ��������posλ��֮ǰ����һ���½ڵ㣬
*          �����½ڵ��ֵ����ΪValue��
* ����: 
*       ����1 :T& Value
* ����ֵ: POSITION���½ڵ��λ��ָ�롣
* ˵��:     ���ڴ����ʧ�ܽ��׳��쳣��
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
template<class T>
POSITION CList_t<T>::InsertBefore(POSITION pos,T Value)
{
    assert(pos != NULL);

    CList_Node* p = (CList_Node*)pos;
    if(p == m_pHead)
    {
        return AddHead(Value);
    }

    CList_Node* pNode = new CList_Node;
    if(!pNode)
    {
        throw "InsertBefore";
    }

    pNode->Value = Value;
    pNode->m_pNext = p;
    pNode->m_pPrev = p->m_pPrev;
    p->m_pPrev->m_pNext = pNode;
    p->m_pPrev = pNode;

    m_uSize++;

    return (POSITION)pNode;
}

/******************************************************
* ������:   CList_t<T>::InsertAfter
* ����:    Zhou Yonghui
* ����:    01-5-24
* ����:    ��������posλ��֮�����һ���½ڵ㣬
*          �����½ڵ��ֵ����ΪValue��
* ����: 
*       ����1 :T& Value
* ����ֵ: POSITION���½ڵ��λ��ָ�롣
* ˵��:     ���ڴ����ʧ�ܽ��׳��쳣��
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
template<class T>
POSITION CList_t<T>::InsertAfter(POSITION pos,T Value)
{
    assert(pos != NULL);

    CList_Node* p = (CList_Node*)pos;
    if(p == m_pTail)
    {
        return AddTail(Value);
    }

    CList_Node* pNode = new CList_Node;
    if(!pNode)
    {
        throw "InsertAfter";
    }

    pNode->Value = Value;
    pNode->m_pPrev = p;
    pNode->m_pNext = p->m_pNext;
    p->m_pNext->m_pPrev = pNode;
    p->m_pNext = pNode;

    m_uSize++;

    return (POSITION)pNode;
}

template<class T>
UINT4 CList_t<T>::GetSize()
{
    return m_uSize;
}

#endif
