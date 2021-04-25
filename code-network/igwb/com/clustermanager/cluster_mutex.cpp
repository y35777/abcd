#include "../include/base_type.h"
#include "cluster_mutex.h"

CClusterMutex* CClusterMutex::g_pTheMutex = NULL;

/********************************************************
* ����: CClusterMutex::CClusterMutex
* ����: ���캯��
* ����:
* ���:
* ����:
********************************************************/
CClusterMutex::CClusterMutex()
{
    //
}

/********************************************************
* ����: CClusterMutex::~CClusterMutex
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CClusterMutex::~CClusterMutex()
{
    //
}

/********************************************************
* ����: CClusterMutex::Instance
* ����: ��õ�ʵ�������ָ��
* ����:
* ���:
* ����: CClusterMutex* const
********************************************************/
CClusterMutex* const CClusterMutex::Instance()
{
    if(g_pTheMutex == NULL)
    {
        //����һ��CClusterMutex����
        g_pTheMutex = new CClusterMutex;
    }
    return g_pTheMutex;
}

/********************************************************
* ����: CClusterMutex::Destroy
* ����: ɾ����ʵ������
* ����:
* ���:
* ����: void
********************************************************/
void CClusterMutex::Destroy()
{
    if(g_pTheMutex != NULL)
    {
        delete g_pTheMutex;
        g_pTheMutex = 0;
    }
}

/********************************************************
* ����: CClusterMutex::Acquire
* ����: ��û�����
* ����:
* ���:
* ����: void
********************************************************/
void CClusterMutex::Acquire()
{
    m_AceMutex.acquire();
}

/********************************************************
* ����: CClusterMutex::Release
* ����: �ͷŻ�����
* ����:
* ���:
* ����: void
********************************************************/
void CClusterMutex::Release()
{
    m_AceMutex.release();
}
