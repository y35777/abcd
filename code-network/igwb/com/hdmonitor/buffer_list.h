/**************************************************************
**文件名：      BufferList.h                
**描述信息：    存放通信帧，并激活相应的线程进行处理
**版本说明：    V100R001
**版权信息：    华为技术有限公司
**作者：        
**创建时间：    
**主要函数列表：
**              (1)POSITION AddElement(ARG_TYPE);       //在缓冲队列尾部增加一个元素
**              (2)POSITION AddHeadElement(ARG_TYPE);   //在缓冲队列头部增加一个元素
**              (3)TYPE RemoveElement(void);            //从缓冲队列头部删除一个元素
**              (4)void DeleteAllElement();             //删除缓冲队列中的所有元素
**              (5)void EnterCriticalSection();         //获取临界区的所有权
**              (6)void LeaveCriticalSection();         //释放临界区的所有权
**              (7)BOOL SetEvent();                     //置m_hEvent为有信号状态
**修改记录：    
***************************************************************/

#ifndef BUFFER_LIST_H_
#define BUFFER_LIST_H_

#include "ace/Synch.h"
#include <list>

template<class TYPE>  class CBufferList
{
public:
    typedef TYPE& REFTYPE;
    typedef std::list<TYPE>::iterator iterator;
    ACE_Thread_Mutex m_Mutex;   //临界区变量
    ACE_Auto_Event m_hEvent;    //事件句柄，为自动重置事件
public :
    CBufferList();  //构造函数
    ~CBufferList(); //析构函数

    void AddElement(REFTYPE newElement);  //在缓冲队列尾部增加一个元素
    void AddHeadElement(REFTYPE newElement); //在缓冲队列头部增加一个元素
    void DeleteAllElement();    //删除缓冲队列中的所有元素
    bool SetEvent();    //置m_hEvent为有信号状态
    //list的函数接口
    int size()
    {
        ACE_GUARD_RETURN(ACE_Thread_Mutex,guard,m_Mutex,0);
        return m_list.size();
    }

    TYPE front()
    {
        ACE_GUARD_RETURN(ACE_Thread_Mutex,guard,m_Mutex,m_list.front());
        return m_list.front();
    }

    void pop_front()
    {
        ACE_GUARD(ACE_Thread_Mutex,guard,m_Mutex);
        m_list.pop_front();
        if(!m_list.empty())
        {
            SetEvent();
        }
    }
    iterator begin()
    {
        ACE_GUARD_RETURN(ACE_Thread_Mutex,guard,m_Mutex,m_list.end());
        return m_list.begin();
    }
    iterator end()
    {
        ACE_GUARD_RETURN(ACE_Thread_Mutex,guard,m_Mutex,m_list.end());
        return m_list.end();
    }
#ifdef _TEST
    void AddElementInTest(REFTYPE newElement);
#endif //_TEST
protected:
    std::list<TYPE> m_list;
};

//构造函数，用来创建事件对象和初始化临界区对象
template<class TYPE>
CBufferList<TYPE>::CBufferList()
{
}

//设置事件对象m_hEvent为有信号状态
template<class TYPE>
bool CBufferList<TYPE>::SetEvent()
{
    m_hEvent.signal();
    return true;
}

//析构函数，删除临界区对象，并清空缓冲队列
template<class TYPE>
CBufferList<TYPE>::~CBufferList()
{
    //清空缓冲队列，释放队列中元素占有的空间
    DeleteAllElement();
}

//删除缓冲队列中的所有元素
template<class TYPE>
void CBufferList<TYPE>::DeleteAllElement()
{
    ACE_GUARD(ACE_Thread_Mutex,guard,m_Mutex);
    //删除缓冲队列中的所有元素
    m_list.clear();
}

//在缓冲队列尾部增加一个元素
template<class TYPE> 
void CBufferList<TYPE>::AddElement(REFTYPE newElement)
{
    ACE_GUARD(ACE_Thread_Mutex,guard,m_Mutex);
    //在缓冲队列尾部增加一个元素，并置m_hEvent为有信号
    m_list.push_back(newElement);
    SetEvent();
}

//在缓冲队列头部增加一个元素
template<class TYPE>
void CBufferList<TYPE>::AddHeadElement(REFTYPE newElement)
{
    ACE_GUARD(ACE_Thread_Mutex,guard,m_Mutex);
    //在缓冲队列头部增加一个元素，并置m_hEvent为有信号
    m_list.push_front(newElement);
    SetEvent();
}

#ifdef _TEST
template<class TYPE>
void CBufferList<TYPE>::AddElementInTest(REFTYPE newElement)
{
    ACE_GUARD(ACE_Thread_Mutex,guard,m_Mutex);  
    //在缓冲队列尾部增加一个元素，但不置m_hEvent为有信号
    m_list.push_back(newElement);
}
#endif //_TEST

#endif //BUFFER_LIST_H_
