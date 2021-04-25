/**************************************************************
**�ļ�����      BufferList.h                
**������Ϣ��    ���ͨ��֡����������Ӧ���߳̽��д���
**�汾˵����    V100R001
**��Ȩ��Ϣ��    ��Ϊ�������޹�˾
**���ߣ�        
**����ʱ�䣺    
**��Ҫ�����б�
**              (1)POSITION AddElement(ARG_TYPE);       //�ڻ������β������һ��Ԫ��
**              (2)POSITION AddHeadElement(ARG_TYPE);   //�ڻ������ͷ������һ��Ԫ��
**              (3)TYPE RemoveElement(void);            //�ӻ������ͷ��ɾ��һ��Ԫ��
**              (4)void DeleteAllElement();             //ɾ����������е�����Ԫ��
**              (5)void EnterCriticalSection();         //��ȡ�ٽ���������Ȩ
**              (6)void LeaveCriticalSection();         //�ͷ��ٽ���������Ȩ
**              (7)BOOL SetEvent();                     //��m_hEventΪ���ź�״̬
**�޸ļ�¼��    
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
    ACE_Thread_Mutex m_Mutex;   //�ٽ�������
    ACE_Auto_Event m_hEvent;    //�¼������Ϊ�Զ������¼�
public :
    CBufferList();  //���캯��
    ~CBufferList(); //��������

    void AddElement(REFTYPE newElement);  //�ڻ������β������һ��Ԫ��
    void AddHeadElement(REFTYPE newElement); //�ڻ������ͷ������һ��Ԫ��
    void DeleteAllElement();    //ɾ����������е�����Ԫ��
    bool SetEvent();    //��m_hEventΪ���ź�״̬
    //list�ĺ����ӿ�
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

//���캯�������������¼�����ͳ�ʼ���ٽ�������
template<class TYPE>
CBufferList<TYPE>::CBufferList()
{
}

//�����¼�����m_hEventΪ���ź�״̬
template<class TYPE>
bool CBufferList<TYPE>::SetEvent()
{
    m_hEvent.signal();
    return true;
}

//����������ɾ���ٽ������󣬲���ջ������
template<class TYPE>
CBufferList<TYPE>::~CBufferList()
{
    //��ջ�����У��ͷŶ�����Ԫ��ռ�еĿռ�
    DeleteAllElement();
}

//ɾ����������е�����Ԫ��
template<class TYPE>
void CBufferList<TYPE>::DeleteAllElement()
{
    ACE_GUARD(ACE_Thread_Mutex,guard,m_Mutex);
    //ɾ����������е�����Ԫ��
    m_list.clear();
}

//�ڻ������β������һ��Ԫ��
template<class TYPE> 
void CBufferList<TYPE>::AddElement(REFTYPE newElement)
{
    ACE_GUARD(ACE_Thread_Mutex,guard,m_Mutex);
    //�ڻ������β������һ��Ԫ�أ�����m_hEventΪ���ź�
    m_list.push_back(newElement);
    SetEvent();
}

//�ڻ������ͷ������һ��Ԫ��
template<class TYPE>
void CBufferList<TYPE>::AddHeadElement(REFTYPE newElement)
{
    ACE_GUARD(ACE_Thread_Mutex,guard,m_Mutex);
    //�ڻ������ͷ������һ��Ԫ�أ�����m_hEventΪ���ź�
    m_list.push_front(newElement);
    SetEvent();
}

#ifdef _TEST
template<class TYPE>
void CBufferList<TYPE>::AddElementInTest(REFTYPE newElement)
{
    ACE_GUARD(ACE_Thread_Mutex,guard,m_Mutex);  
    //�ڻ������β������һ��Ԫ�أ�������m_hEventΪ���ź�
    m_list.push_back(newElement);
}
#endif //_TEST

#endif //BUFFER_LIST_H_
