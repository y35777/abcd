#include "../include/base_type.h"
#include "res_object.h"


/********************************************************
* ����: CResObject::CResObject
* ����: ���캯��
* ����: const char* const szResName : ��Դ��
*       ERES_TYPE nResType          : ��Դ����
*       int nMaxLocalRecovery       : ��󱾵ػָ�����
* ���:
* ����:
********************************************************/
CResObject::CResObject(const char* const szResName,
                       ERES_TYPE nResType,
                       int nMaxLocalRecovery)
{
    strncpy(m_szResName, szResName, MAX_RES_NAME);
    m_szResName[MAX_RES_NAME - 1] = '\0';

    m_nResType                = nResType;
    m_nMaxLocalRecovery       = nMaxLocalRecovery;
    m_nLocalRecovery          = 0;
    m_tLastActiveTime         = time(NULL);
    m_nStatus                 = ERES_STATUS_INIT;
    m_uAlarmID                = 0;
    m_uAlarmPara              = 0;
    m_uResIndex               = 0;
    m_nSwitchGroup            = 0;
}

/********************************************************
* ����: CResObject::~CResObject
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CResObject::~CResObject()
{
    //
}

/********************************************************
* ����: CResObject::GetAlarmID
* ����: ��ȡ��Դ�澯ID
* ����:
* ���:
* ����: UINT4 ��Դ�澯ID
********************************************************/
UINT4 CResObject::GetAlarmID()
{
    return m_uAlarmID;
}

/********************************************************
* ����: CResObject::GetAlarmPara
* ����: ��ȡ��Դ�澯����
* ����:
* ���:
* ����: UINT4 ��Դ�澯����
********************************************************/
UINT4 CResObject::GetAlarmPara()
{
    return m_uAlarmPara;
}

/********************************************************
* ����: CResObject::GetResIndex
* ����: ��ȡ��Դ������
* ����:
* ���:
* ����: UINT4 ��Դ������
********************************************************/
UINT4 CResObject::GetResIndex()
{
    return m_uResIndex;
}

/********************************************************
* ����: CResObject::GetResType
* ����: ��ȡ��Դ����
* ����:
* ���:
* ����: ERES_TYPE ��Դ����
********************************************************/
ERES_TYPE CResObject::GetResType()
{
    return m_nResType;
}

/********************************************************
* ����: CResObject::GetStatus
* ����: ��ȡ��Դ״̬
* ����:
* ���:
* ����: ERES_STATUS ��Դ״̬
********************************************************/
ERES_STATUS CResObject::GetStatus()
{
    return m_nStatus;
}

/********************************************************
* ����: CResObject::GetSwitchGroup
* ����: ��ȡ��Դ�ĵ������
* ����:
* ���:
* ����: ERES_TYPE ��Դ����
********************************************************/
int CResObject::GetSwitchGroup()
{
    return m_nSwitchGroup;
}

/********************************************************
* ����: CResObject::GetResName
* ����: ��ȡ��Դ����
* ����:
* ���:
* ����: const char* ��Դ����
********************************************************/
const char* CResObject::GetResName()
{
    return m_szResName;
}

/********************************************************
* ����: CResObject::Create
* ����: ��Դ��������
* ����:
* ���:
* ����: int ��Դ�����Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CResObject::Create()
{
    m_nStatus = ERES_STATUS_DEACTIVE;

    return ERR_SUCCESS;
}

/********************************************************
* ����: CResObject::Remove
* ����: �ͷ���Դǰ��������
* ����:
* ���:
* ����: void
********************************************************/
void CResObject::Remove()
{
    //���״̬Ϊ��������DeActive����
    if(ERES_STATUS_ACTIVE == m_nStatus)
    {
        DeActive();
    }
}

/********************************************************
* ����: CResObject::CheckStatus
* ����: ��Դ��״̬��麯��
* ����:
* ���:
* ����: int �Ƿ���ڲ��ɻָ��Ĺ��ϣ�0��ʾ�޹��ϣ�����ֵ��ʾ��⵽����
********************************************************/
int CResObject::CheckStatus()
{
    return ERR_SUCCESS;
}


/********************************************************
* ����: CResObject::LocalRecovery
* ����: ִ�б��ػָ�����
* ����: 
* ���: 
* ����: int ���ػָ��Ƿ�ɹ� 
********************************************************/
int CResObject::LocalRecovery()
{
    for(int i = 0; i < m_nMaxLocalRecovery; i++)
    {
        if(Active() == ERR_SUCCESS)
        {
            return ERR_SUCCESS;
        }

        if(i < m_nMaxLocalRecovery - 1)
        {
            ACE_OS::sleep(5);
        }
    }

    return ERR_FAIL;
}

/********************************************************
* ����: CResObject::Active
* ����: ��Դ�ļ����
* ����:
* ���:
* ����: int �Ƿ񼤻�ɹ���0��ʾ�ɹ�������ֵ��ʾ����ʧ��
********************************************************/
int CResObject::Active()
{
    m_tLastActiveTime = time(NULL);
    m_nStatus         = ERES_STATUS_ACTIVE;

    return ERR_SUCCESS;
}

/********************************************************
* ����: CResObject::DeActive
* ����: ��Դ��ȥ�����
* ����:
* ���:
* ����: int �Ƿ�ȥ����ɹ���0��ʾ�ɹ�������ֵ��ʾȥ����ʧ��
********************************************************/
int CResObject::DeActive()
{
    m_nStatus = ERES_STATUS_DEACTIVE;

    return ERR_SUCCESS;
}
