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
* Update: 01-5-24 下午 04:20:02
* Date              Name              Description 
============== ================ ======================
## 01-5-24       Zhou Yonghui
*****************************************************/

#ifndef _INDEX_H
#define _INDEX_H

#include "../include/type_def.h"
#include "list.h"

/*
    CHashIndex_t实现HASH索引，参数类型T是索引参数类型。
    类型T必须实现GetKey方法和operator==方法。
*/
template<class T>
class CHashIndex_t
{
protected:
    struct CNode
    {
        void* value;                //表中只存放void*类型的数据。
        T key;                      //索引键。
    };

protected:
    UINT4 m_uSize;                  //HASH表的大小。
    CList_t<CNode>* m_pTable;       //表头指针。

public:
    CHashIndex_t();
    virtual ~CHashIndex_t();

    //初始化HASH表。
    virtual void InitHashTable(UINT4 uSize);

    //搜索与Key相关联的值。
    virtual void* Lookup(T& Key);

    //把Value与Key相关联。
    virtual void SetAt(T& Key,void* pValue);
    //删除与Key相关联的Value。
    virtual void RemoveAt(T& Key,void* pValue);

    //返回HASH表的大小。
    virtual UINT4 GetSize(); 
    //删除所有子表节点。
    virtual void RemoveAll();
};


/******************************************************
* 函数名:  CHashIndex_t<T>::InitHashTable
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    初始化HASH表，成功之后先前所有的索引信息都将丢失。
* 输入: 
*       参数1 :long nSize，新表的长度。
* 返回值: 无
* 说明:     内存操作失败将抛出异常。
* 更新记录: 
*
* 日期            姓名                  描述 
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
* 函数名:   CHashIndex_t<T>::Lookup
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    在HASH表中搜索与Key匹配的值。
*          返回值是一个代表话单在Qw中位置的指针。
* 输入: 
*       参数1 :T& Key
* 返回值: void* 
* 说明: 成功返回一个指针，失败返回NULL。
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
template<class T>
void* CHashIndex_t<T>::Lookup(T& Key)
{
    //计算索引值。
    UINT4 uKey = Key.GetKey() % m_uSize;

    //获得索引链表的指针。
    CList_t<CNode>* p = &m_pTable[uKey];
    POSITION pos = p->GetHeadPosition();

    while(pos != NULL)
    {
        //返回pos位置的值，并把pos指向下一个位置。
        CNode& node = p->GetNext(pos);
        if(Key == node.key)     //判断键是否匹配。
        {
            return node.value;  //返回Qw节点的位置。
        }
    }

    return NULL;
}


/******************************************************
* 函数名:   CHashIndex_t<T>::SetAt
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    把pValue与Key关联在一起。
* 输入: 
*       参数1 :T& Key,HASH表索引键。
*       参数2 :void* pValue,将要被假如HASH表的指针。
*       调用模块必须自己保证，在HASH表中Key没有重复值。
* 返回值: 无
* 说明:     内存操作失败将抛出异常。
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
template<class T>
void CHashIndex_t<T>::SetAt(T& Key,void* pValue)
{
    //计算索引值。
    UINT4 uKey = Key.GetKey() % m_uSize;

    //获得索引链表的指针。
    CList_t<CNode>* p = &m_pTable[uKey];

    //在链表中增加新节点。
    CNode node;
    node.key = Key;
    node.value = pValue;
    p->AddTail(node);
}


/******************************************************
* 函数名:   CHashIndex_t<T>::RemoveAt
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    在HASH表中删除与键Key匹配的值。
* 输入: 
*       参数1 :T& Key
*       参数2 :void* pValue
* 返回值: 无
* 说明: 调用者必须保证Key在索引表中存在。
*       若没有发现与Key匹配的值，则引发assert断言。
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
template<class T>
void CHashIndex_t<T>::RemoveAt(T& Key,void* pValue)
{
    //计算索引值。
    UINT4 uKey = Key.GetKey() % m_uSize;

    //获得索引链表的指针。
    CList_t<CNode>* p = &m_pTable[uKey];
    POSITION pos = p->GetHeadPosition();
    POSITION prev = NULL;

    while(pos != NULL)
    {
        prev = pos;
        //返回pos位置的值，并把pos指向下一个位置。
        CNode& node = p->GetNext(pos);
        if(Key == node.key)
        {
            assert(node.value == pValue);
            p->RemoveAt(prev);
            return;
        }
    }

    //没有找到匹配的索引节点。
    assert(0);
}

template<class T>
UINT4 CHashIndex_t<T>::GetSize()
{
    return m_uSize;
}


/******************************************************
* 函数名:   CHashIndex_t<T>::RemoveAll
* 作者:    Zhou Yonghui
* 日期:    01-6-7
* 描述:    删除HASH表中所有索引信息，但是并不回收HASH表占用的空间。
* 输入: 
* 返回值: void
* 说明:    若内存操作失败，则抛出异常。
* 更新记录: 
*
* 日期            姓名                  描述
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
