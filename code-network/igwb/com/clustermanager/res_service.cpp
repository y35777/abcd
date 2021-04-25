#include "../include/base_type.h"
#include "res_service.h"
#include "BtwService.h"


/********************************************************
* ����: CResService::CResService
* ����: ���캯��
* ����: const char* const szServiceName : ������
*       const char* const szResName     : ��Դ��
*       ERES_TYPE nResType              : ��Դ����
*       int nMaxLocalRecovery           : ��󱾵ػָ�����
* ���:
* ����:
********************************************************/
CResService::CResService(const char* const szServiceName,
                         const char* const szResName,
                         ERES_TYPE nResType,
                         int nMaxLocalRecovery):
CResObject(szResName, nResType, nMaxLocalRecovery)
{
    strncpy(m_szServiceName, szServiceName, MAX_SERVICE_NAME);
    m_szServiceName[MAX_SERVICE_NAME - 1] = '\0';
}

/********************************************************
* ����: CResService::~CResService
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CResService::~CResService()
{
    //
}

/********************************************************
* ����: CResService::Create
* ����: ��Դ��������
* ����:
* ���:
* ����: int ��Դ�����Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CResService::Create()
{
    //�������Ƿ����
    if(!CtrlService(m_szServiceName, CMD_CHECK_SERVICE))
    {
        return ERR_FAIL;
    }

    //�����������ֹͣ��������
    StopService(m_szServiceName);

    return CResObject::Create();
}

/********************************************************
* ����: CResService::CheckStatus
* ����: ��Դ��״̬��麯��
* ����:
* ���:
* ����: int �Ƿ���ڲ��ɻָ��Ĺ��ϣ�0��ʾ�޹��ϣ�����ֵ��ʾ��⵽����
********************************************************/
int CResService::CheckStatus()
{
    SERVICE_STATUS ServiceStatus;
    if(QueryServiceStatus(m_szServiceName, &ServiceStatus))
    {
        if(ServiceStatus.dwCurrentState != SERVICE_STOPPED)
        {
            return ERR_SUCCESS;
        }
    }

    m_nStatus = ERES_STATUS_DEACTIVE;
    return ERR_FAIL;
}

/********************************************************
* ����: CResService::Active
* ����: ��Դ�ļ����
* ����:
* ���:
* ����: int �Ƿ񼤻�ɹ���0��ʾ�ɹ�������ֵ��ʾ����ʧ��
********************************************************/
int CResService::Active()
{
    if (!StartService(m_szServiceName))
    {
        return ERR_FAIL;
    }

    return CResObject::Active();
}

/********************************************************
* ����: CResService::DeActive
* ����: ��Դ��ȥ�����
* ����:
* ���:
* ����: int �Ƿ�ȥ����ɹ���0��ʾ�ɹ�������ֵ��ʾȥ����ʧ��
********************************************************/
int CResService::DeActive()
{
    if (!StopService(m_szServiceName))
    {
        return ERR_FAIL;
    }

    return CResObject::DeActive();
}
