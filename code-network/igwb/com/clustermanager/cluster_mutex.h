#ifndef __CLUSTER_MUTEX_H__
#define __CLUSTER_MUTEX_H__

class CClusterMutex
{
public:
    //���CClusterMutex��ʵ������ָ��
    static CClusterMutex* const Instance();

    //����CClusterMutex��ʵ������
    static void Destroy();

    //�����û�������ʹ��Ȩ
    void Acquire();

    //�ͷŻ�������ʹ��Ȩ
    void Release();

protected:
    CClusterMutex();
    ~CClusterMutex();

    //����CClusterMutexֻ֧��һ��ʵ��������
    //���Ըþ�ָ̬��ָ��ö���
    static CClusterMutex* g_pTheMutex;

    //ACE�ṩ�ĵݹ��߳���
    ACE_Recursive_Thread_Mutex m_AceMutex;
};

#endif //__CLUSTER_MUTEX_H__