#include "../include/frame.h"
#include "com_cluster.h"
#include "cluster_module.h"
#include "asyn_res_kernel.h"


/********************************************************
* ����: CAsynResKernel::CAsynResKernel
* ����: ���캯��
* ����: szAppPath      : Kernel���̵�ȫ·����
*       pClusterModule : CClusterModule�����ָ��
*       szResName      : ��Դ��
*       pComCluster    : CComCluster�����ָ��
* ���:
* ����:
********************************************************/
CAsynResKernel::CAsynResKernel(const char* const szAppPath,
                               CClusterModule* pClusterModule,
                               const char* const szResName)
                               :CAsynResObject(szResName)
{
    strncpy(m_szAppPath, szAppPath, MAX_PATH);
    m_szAppPath[MAX_PATH - 1] = '\0';

    assert(NULL != pClusterModule);
    m_pClusterModule = pClusterModule;

    m_nPid           = ACE_INVALID_PID;
}

/********************************************************
* ����: CAsynResKernel::~CAsynResKernel
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CAsynResKernel::~CAsynResKernel()
{
    m_pClusterModule = NULL;
}

/********************************************************
* ����: CAsynResKernel::AcquireLock
* ����: ���뻥����
* ����: 
* ���: 
* ����: void 
********************************************************/
void CAsynResKernel::AcquireLock()
{
    m_Mutex.acquire();
}

/********************************************************
* ����: CAsynResKernel::ReleaseLock
* ����: �ͷŻ�����
* ����: 
* ���: 
* ����: void 
********************************************************/
void CAsynResKernel::ReleaseLock()
{
    m_Mutex.release();
}

/********************************************************
* ����: CAsynResKernel::SetPidInValid
* ����: ����PIDΪ��Ч
* ����: 
* ���: 
* ����: void 
********************************************************/
void CAsynResKernel::SetPidInValid()
{
    m_nPid = ACE_INVALID_PID;
}

/********************************************************
* ����: CAsynResKernel::Create
* ����: ��Դ��������
* ����:
* ���:
* ����: int ��Դ�����Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CAsynResKernel::Create()
{

    //����ACE_OS::access�������KERNEL�����ļ��Ƿ����
    if (ACE_OS::access(m_szAppPath, F_OK) != 0)
    {
        return ERR_FILE_NOT_PRESENT;
    }

    //����m_szAppPath����ȫ·��������ʼ��m_POptions
    m_POptions.command_line("%s", m_szAppPath);

    m_nStatus = EASYNRES_DEACTIVE;
    return ERR_SUCCESS;
}

/********************************************************
* ����: CAsynResKernel::Active
* ����: ��Դ�ļ����
* ����:
* ���:
* ����: int �Ƿ񼤻�ɹ���0��ʾ�ɹ�������ֵ��ʾ����ʧ��
********************************************************/
int CAsynResKernel::Active()
{
    AcquireLock();

    int nRet = ERR_SUCCESS;
    if((ACE_INVALID_PID == m_nPid) || (CheckStatus() != ERR_SUCCESS))
    {
        m_pClusterModule->Active();

        m_nStatus = EASYNRES_ACTIVING;
        m_nPid    = m_PMgr.spawn(m_POptions);

        if(m_nPid == ACE_INVALID_PID)
        {
            nRet = ERR_FAIL;
        }
    }

    ReleaseLock();
    
    return nRet;
}

/********************************************************
* ����: CAsynResKernel::CheckStatus
* ����: ��Դ��״̬��麯��
* ����:
* ���:
* ����: int �Ƿ���ڲ��ɻָ��Ĺ��ϣ�0��ʾ�޹��ϣ�����ֵ��ʾ��⵽����
********************************************************/
int CAsynResKernel::CheckStatus()
{
    int nRet = m_PMgr.wait(m_nPid, ACE_Time_Value(0));

    return nRet;
}

/********************************************************
* ����: CAsynResKernel::DeActive
* ����: ��Դ��ȥ�����
* ����:
* ���:
* ����: int �Ƿ�ȥ����ɹ���0��ʾ�ɹ�������ֵ��ʾȥ����ʧ��
********************************************************/
int CAsynResKernel::DeActive()
{
    AcquireLock();

    m_nStatus = EASYNRES_DEACTIVING;
    int nRet = m_pClusterModule->DeActive();

    ReleaseLock();

    return nRet;
}

/********************************************************
* ����: CAsynResKernel::terminate
* ����: ǿ��ɱ��Kernel
* ����:
* ���:
* ����: void
********************************************************/
void CAsynResKernel::Terminate()
{
    ACE::terminate_process(m_nPid);
}

/********************************************************
* ����: CAsynResKernel::OnReceive
* ����: ����Զ˷��͵�˫����Ϣ
* ����: SClusterFrame* pFrame :
* ���:
* ����: void
********************************************************/
void CAsynResKernel::OnReceive(SClusterFrame* pFrame)
{
    switch(pFrame->nCmdCode)
    {
    case CMD_CLS_SYNC_DATA:
        //�ַ�ͬ������
        if (NULL != m_pClusterModule)
        {
            m_pClusterModule->OnDoSyncData(pFrame->Data, pFrame->nLen);
        }
        break;

    default:
        break;
    }
}

/********************************************************
* ����: CAsynResKernel::GetAttachedData
* ����: ��ȡ�������
* ����:
* ���: pData : ���������ݻ�������ָ��
*       nLen  : ���������ݻ������ĳ���
* ����: int �������������ݻ������ĳ���
********************************************************/
int CAsynResKernel::GetAttachedData(void*& pData, int& nLen)
{
    //����������
    if (NULL == m_pClusterModule)
    {
        pData = NULL;
        nLen  = 0;
        return 0;
    }

    return m_pClusterModule->GetSyncData(pData, nLen);
}

/********************************************************
* ����: CAsynResKernel::ProcAttachedData
* ����: �����������
* ����: pData : ����������ݻ�������ָ��
*       nLen  : ����������ݻ������ĳ���
* ���:
* ����: void
********************************************************/
void CAsynResKernel::ProcAttachedData(void* pData, int nLen)
{
    //�ַ�ͬ������
    if (NULL != m_pClusterModule)
    {
        m_pClusterModule->OnDoSyncData(pData, nLen);
    }
}

