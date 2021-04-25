#ifndef __CLUSTER_MUTEX_H__
#define __CLUSTER_MUTEX_H__

class CClusterMutex
{
public:
    //获得CClusterMutex单实例对象指针
    static CClusterMutex* const Instance();

    //销毁CClusterMutex单实例对象
    static void Destroy();

    //申请获得互斥量的使用权
    void Acquire();

    //释放互斥量的使用权
    void Release();

protected:
    CClusterMutex();
    ~CClusterMutex();

    //由于CClusterMutex只支持一个实例化对象，
    //所以该静态指针指向该对象
    static CClusterMutex* g_pTheMutex;

    //ACE提供的递归线程锁
    ACE_Recursive_Thread_Mutex m_AceMutex;
};

#endif //__CLUSTER_MUTEX_H__