#include "../include/base_type.h"
#include "com_cluster.h"
#include "asyn_res_object.h"


/********************************************************
* ����: CAsynResObject::CAsynResObject
* ����: ���캯��
* ����: szResName   : ��Դ��
* ���:
* ����:
********************************************************/
CAsynResObject::CAsynResObject(const char* const szResName)
:m_nStatus(EASYNRES_DEACTIVE), m_pComCluster(NULL)
{
    strncpy(m_szResName, szResName, MAX_ASYN_RES_NAME);
    m_szResName[MAX_ASYN_RES_NAME - 1] = '\0';
}

/********************************************************
* ����: CAsynResObject::~CAsynResObject
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CAsynResObject::~CAsynResObject()
{
    m_pComCluster = NULL;
}

/********************************************************
* ����: CAsynResObject::GetStatus
* ����: ��ȡ�첽��Դ״̬
* ����:
* ���:
* ����: EASYNRES_STATUS �첽��Դ״̬
********************************************************/
EASYNRES_STATUS CAsynResObject::GetStatus()
{
    return m_nStatus;
}

/********************************************************
* ����: CAsynResObject::SetStatus
* ����: �����첽��Դ״̬
* ����: EASYNRES_STATUS nStatus : �첽��Դ״̬
* ���:
* ����: void
********************************************************/
void CAsynResObject::SetStatus(EASYNRES_STATUS nStatus)
{
    m_nStatus = nStatus;
}

/********************************************************
* ����: CAsynResObject::GetResName
* ����: ��ȡ��Դ����
* ����:
* ���:
* ����: const char* ��Դ����
********************************************************/
const char* CAsynResObject::GetResName()
{
    return m_szResName;
}

/********************************************************
* ����: CAsynResObject::Attach
* ����: ��CComCluster�����ָ��
* ����: pComCluster : CComCluster�����ָ��
* ���:
* ����: void
********************************************************/
void CAsynResObject::Attach(CComCluster* pComCluster)
{
    m_pComCluster = pComCluster;
}

/********************************************************
* ����: CAsynResObject::Create
* ����: ��Դ��������
* ����:
* ���:
* ����: int ��Դ�����Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CAsynResObject::Create()
{
    m_nStatus = EASYNRES_DEACTIVE;

    return ERR_SUCCESS;
}

/********************************************************
* ����: CAsynResObject::Active
* ����: ��Դ�ļ����
* ����:
* ���:
* ����: int �Ƿ񼤻�ɹ���0��ʾ�ɹ�������ֵ��ʾ����ʧ��
********************************************************/
int CAsynResObject::Active()
{
    m_nStatus = EASYNRES_ACTIVING;

    return ERR_SUCCESS;
}

/********************************************************
* ����: CAsynResObject::DeActive
* ����: ��Դ��ȥ�����
* ����:
* ���:
* ����: int �Ƿ�ȥ����ɹ���0��ʾ�ɹ�������ֵ��ʾȥ����ʧ��
********************************************************/
int CAsynResObject::DeActive()
{
    m_nStatus = EASYNRES_DEACTIVING;

    return ERR_SUCCESS;
}

/********************************************************
* ����: CAsynResObject::OnReceive
* ����: ����Զ˷��͵�˫����Ϣ
* ����: SClusterFrame* pFrame :
* ���:
* ����: void
********************************************************/
void CAsynResObject::OnReceive(SClusterFrame* pFrame)
{
    //
}

/********************************************************
* ����: CAsynResObject::GetAttachedData
* ����: ��ȡ�������
* ����:
* ���: pData : ���������ݻ�������ָ��
*       nLen  : ���������ݻ������ĳ���
* ����: int �������������ݻ������ĳ���
********************************************************/
int CAsynResObject::GetAttachedData(void*& pData, int& nLen)
{
    return 0;
}

/********************************************************
* ����: CAsynResObject::ProcAttachedData
* ����: �����������
* ����: pData : ����������ݻ�������ָ��
*       nLen  : ����������ݻ������ĳ���
* ���:
* ����: void
********************************************************/
void CAsynResObject::ProcAttachedData(void* pData, int nLen)
{
    //
}

