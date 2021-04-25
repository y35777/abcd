#include "../include/base_type.h"
#include "res_user_defined.h"
#include "cluster_toolbox.h"


/********************************************************
* ����: CResUserDefined::CResUserDefined
* ����: ���캯��
* ����: const char* const szCreateName  : ִ�д��������ĳ���
*       const char* const szActiveName  : ִ�м�������ĳ���
*       const char* const szDeActiveName: ִ��ȥ��������ĳ���
*       const char* const szCheckStatus : ִ��״̬�������ĳ���
*       const char* const szResName     : ��Դ��
*       ERES_TYPE nResType              : ��Դ����
*       int nMaxLocalRecovery           : ��󱾵ػָ�����
* ���:
* ����:
********************************************************/
CResUserDefined::CResUserDefined(const char* const szCreateName,
                                 const char* const szActiveName,
                                 const char* const szDeActiveName,
                                 const char* const szCheckStatus,
                                 const char* const szResName,
                                 ERES_TYPE nResType,
                                 int nMaxLocalRecovery):
CResObject(szResName, nResType, nMaxLocalRecovery)
{
    strncpy(m_szCreateName, szCreateName, MAX_PATH);
    m_szCreateName[MAX_PATH - 1] = '\0';

    strncpy(m_szActiveName, szActiveName, MAX_PATH);
    m_szActiveName[MAX_PATH - 1] = '\0';

    strncpy(m_szDeActiveName, szDeActiveName, MAX_PATH);
    m_szCreateName[MAX_PATH - 1] = '\0';

    strncpy(m_szCheckName, szCheckStatus, MAX_PATH);
    m_szCheckName[MAX_PATH - 1] = '\0';
}

/********************************************************
* ����: CResUserDefined::~CResUserDefined
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CResUserDefined::~CResUserDefined()
{
    //
}

/********************************************************
* ����: CResUserDefined::Create
* ����: ��Դ��������
* ����:
* ���:
* ����: int ��Դ�����Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CResUserDefined::Create()
{
    if(('\0' != m_szCreateName[0]) && 
       (ERR_SUCCESS != ExecuteProcess(m_szCreateName)))
    {
        return ERR_FAIL;
    }

    return CResObject::Create();
}

/********************************************************
* ����: CResUserDefined::CheckStatus
* ����: ��Դ��״̬��麯��
* ����:
* ���:
* ����: int �Ƿ���ڲ��ɻָ��Ĺ��ϣ�0��ʾ�޹��ϣ�����ֵ��ʾ��⵽����
********************************************************/
int CResUserDefined::CheckStatus()
{
    if(('\0' != m_szCheckName[0]) && 
       (ERR_SUCCESS != ExecuteProcess(m_szCheckName)))
    {
        m_nStatus = ERES_STATUS_DEACTIVE;
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

/********************************************************
* ����: CResUserDefined::Active
* ����: ��Դ�ļ����
* ����:
* ���:
* ����: int �Ƿ񼤻�ɹ���0��ʾ�ɹ�������ֵ��ʾ����ʧ��
********************************************************/
int CResUserDefined::Active()
{
    if(('\0' != m_szActiveName[0]) && 
       (ERR_SUCCESS != ExecuteProcess(m_szActiveName)))
    {
        return ERR_FAIL;
    }

    return CResObject::Active();
}

/********************************************************
* ����: CResUserDefined::DeActive
* ����: ��Դ��ȥ�����
* ����:
* ���:
* ����: int �Ƿ�ȥ����ɹ���0��ʾ�ɹ�������ֵ��ʾȥ����ʧ��
********************************************************/
int CResUserDefined::DeActive()
{
    if(('\0' != m_szDeActiveName[0]) && 
       (ERR_SUCCESS != ExecuteProcess(m_szDeActiveName)))
    {
        return ERR_FAIL;
    }

    return CResObject::DeActive();
}
