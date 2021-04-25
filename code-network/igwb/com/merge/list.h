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
* Update: 01-5-24 下午 05:39:06
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
*双向链表类，支持链表节点的插入、删除和链表的遍历操作。
*CList_t类是一个模板类，参数类型T是链表节点中存放的数据类型。
*类型T必须实现赋值运算符“＝”。
*
*调用者在调用CList_t类的方法之前，必须保证POSITION参数是有效的。
*/
template<class T>
class CList_t
{
protected:
    struct CList_Node
    {
        CList_Node* m_pNext, *m_pPrev;  //链接指针。
        T  Value;                       //数据成员。
    };

protected:
    CList_Node* m_pHead;                //链表头指针。
    CList_Node*  m_pTail;               //链表尾指针。
    UINT4 m_uSize;                      //链表的长度。

public:
    CList_t();
    ~CList_t();

    POSITION GetHeadPosition();         //获得头节点的位置。
    POSITION GetTailPosition();         //获得尾节点的位置。

    T& GetHead();                       //取头节点的值。
    T& GetTail();                       //取尾节点的值。

    T& GetNext(POSITION& pos);
    T& GetPrev(POSITION& pos);
    T& GetAt(POSITION& pos );           //获得指定位置的元素。

                                        //替换指定位置的元素。
    void SetAt(POSITION& pos ,T Value);

    void RemoveHead();                  //删除头节点。
    void RemoveTail();                  //删除尾节点。
    void RemoveAt(POSITION& pos);       //删除pos指向的节点。
    void RemoveAll();                   //删除所有节点。

    POSITION AddHead(T& Value);         //在链表的头部增加节点。
    POSITION AddTail(T& Value);         //在链表的尾部增加节点。
    BOOL IsEmpty();                     //判断链表是否为空。

                                        //在指定位置之前插入元素。
    POSITION InsertBefore(POSITION pos,T Value);
                                        //在指定位置之后插入元素。
    POSITION InsertAfter(POSITION pos,T Value);
    UINT4 GetSize();                    //返回链表的节点数。
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
* 函数名:   CList_t<T>::GetHeadPosition
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    获取链表头节点的位置。
* 输入: 
* 返回值: POSITION，代表链表头节点的“位置指针”。
* 说明:     若链表为空，则返回NULL。
* 更新记录: 
*
* 日期            姓名                  描述 
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
* 函数名:   CList_t<T>::GetHead
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    获取链表头节点存储的值的引用。
* 输入: 
* 返回值:T& ，链表存储的值的引用。
* 说明:     若链表为空将引发assert断言。
* 更新记录: 
*
* 日期            姓名                  描述 
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
* 函数名:   CList_t<T>::GetNext
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    获取pos所指位置的值的引用，同时pos被指向下一个位置。
* 输入: 
*       参数1 :POSITION& pos，链表节点位置的指针。
* 返回值: T& 
* 说明:     若pos为空，将引发assert断言。
* 更新记录: 
*
* 日期            姓名                  描述 
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
* 函数名:   CList_t<T>::GetPrev
* 作者:    Zhou Yonghui
* 日期:    01-6-7
* 描述:    获取pos所指位置的值的引用，同时pos被指向前一个位置。
* 输入: 
*       参数1 :POSITION& pos，链表节点位置的指针。
* 返回值: T& 
* 说明:     若pos为空，将引发assert断言。
* 更新记录: 
*
* 日期            姓名                  描述 
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
* 函数名:   CList_t<T>::GetAt
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    获取pos所指位置的值的引用，pos的值不会改变。
* 输入: 
*       参数1 :POSITION& pos，链表节点位置的指针。
* 返回值:T& 
* 说明:     若pos为空，将引发assert断言。
* 更新记录: 
*
* 日期            姓名                  描述 
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
* 函数名:   CList_t<T>::SetAt
* 作者:    Zhou Yonghui
* 日期:    01-6-7
* 描述:    用Value替代pos位置的值，pos不会改变。
* 输入: 
*       参数1 :POSITION& pos
*       参数2 :T Value，新的值。
* 返回值: void 
* 说明:     若pos为空，将引发assert断言。
* 更新记录: 
*
* 日期            姓名                  描述 
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
* 函数名:   CList_t<T>::RemoveHead
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    删除链表头部节点。
* 输入: 
* 返回值: 无。
* 说明:     若链表为空将引发assert断言。
* 更新记录: 
*
* 日期            姓名                  描述 
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
* 函数名:   CList_t<T>::RemoveTail
* 作者:    Zhou Yonghui
* 日期:    01-6-7
* 描述:    删除链表尾部节点。
* 输入: 
* 返回值: void 
* 说明: 若链表为空将引发assert断言。
* 更新记录: 
*
* 日期            姓名                  描述 
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
* 函数名:   CList_t<T>::RemoveAt
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    删除pos指向的节点。
* 输入: 
*       参数1 :POSITION& pos，需要删除的节点的位置指针。
* 返回值: 无。
* 说明:     若pos为空，将引发assert断言。
* 更新记录: 
*
* 日期            姓名                  描述
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
* 函数名:   CList_t<T>::RemoveAll
* 作者:    Zhou Yonghui
* 日期:    01-6-7
* 描述:    删除链表中索引节点。
* 输入: 
* 返回值: void 
* 说明: 
* 更新记录: 
*
* 日期            姓名                  描述 
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
* 函数名:   CList_t<T>::AddHead
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    在链表的头节点之前插入一个新节点，
*          并且新节点的值被设为Value。
* 输入: 
*       参数1 :T& Value
* 返回值: POSITION，新节点的位置指针。
* 说明:     若内存操作失败将抛出异常。
* 更新记录: 
*
* 日期            姓名                  描述 
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
* 函数名:   CList_t<T>::AddTail
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    在链表的尾节点之后插入一个新节点，
*          并且新节点的值被设为Value。
* 输入: 
*       参数1 :T& Value
* 返回值: POSITION，新节点的位置指针。
* 说明:     若内存操作失败将抛出异常。
* 更新记录: 
*
* 日期            姓名                  描述 
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
* 函数名:   CList_t<T>::InsertBefore
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    在链表中pos位置之前插入一个新节点，
*          并且新节点的值被设为Value。
* 输入: 
*       参数1 :T& Value
* 返回值: POSITION，新节点的位置指针。
* 说明:     若内存操作失败将抛出异常。
* 更新记录: 
*
* 日期            姓名                  描述 
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
* 函数名:   CList_t<T>::InsertAfter
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    在链表中pos位置之后插入一个新节点，
*          并且新节点的值被设为Value。
* 输入: 
*       参数1 :T& Value
* 返回值: POSITION，新节点的位置指针。
* 说明:     若内存操作失败将抛出异常。
* 更新记录: 
*
* 日期            姓名                  描述 
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
