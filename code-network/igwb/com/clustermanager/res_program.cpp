#include "../include/base_type.h"
#include "res_program.h"


/********************************************************
* ����: CResProgram::CResProgram
* ����: ���캯��
* ����: const char* const szAppPath : ������Դ��ȫ·����
*       const char* const szResName : ��Դ��
*       ERES_TYPE nResType          : ��Դ����
*       int nMaxLocalRecovery       : ��󱾵ػָ�����
* ���:
* ����:
********************************************************/
CResProgram::CResProgram(const char* const szAppPath,
                         const char* const szResName,
                         ERES_TYPE nResType,
                         int nMaxLocalRecovery):
CResObject(szResName, nResType, nMaxLocalRecovery)
{
    strncpy(m_szAppPath, szAppPath, MAX_PATH);
    m_szAppPath[MAX_PATH - 1] = '\0';
    m_nPid                    = ACE_INVALID_PID;
}

/********************************************************
* ����: CResProgram::~CResProgram
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CResProgram::~CResProgram()
{
    //
}

/********************************************************
* ����: CResProgram::Create
* ����: ��Դ��������
* ����:
* ���:
* ����: int ��Դ�����Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CResProgram::Create()
{
    int nRet = ACE_OS::access(m_szAppPath, F_OK);
    if (ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    m_POptions.command_line("%s", m_szAppPath);

    return CResObject::Create();
}

/********************************************************
* ����: CResProgram::CheckStatus
* ����: ��Դ��״̬��麯��
* ����:
* ���:
* ����: int �Ƿ���ڲ��ɻָ��Ĺ��ϣ�0��ʾ�޹��ϣ�����ֵ��ʾ��⵽����
********************************************************/
int CResProgram::CheckStatus()
{
    if(ACE_INVALID_PID == m_nPid)
    {
        m_nStatus = ERES_STATUS_DEACTIVE;
        return ERR_FAIL;
    }

    int nRet = m_PMgr.wait(m_nPid, ACE_Time_Value(0));
    if(0 != nRet)
    //0��ʾ��ʱ�������̻������У��������ʾ�������˳������
    {
        m_nPid = ACE_INVALID_PID;
        m_nStatus = ERES_STATUS_DEACTIVE;
        nRet = ERR_FAIL;
    }

    return nRet;
}

/********************************************************
* ����: CResProgram::Active
* ����: ��Դ�ļ����
* ����:
* ���:
* ����: int �Ƿ񼤻�ɹ���0��ʾ�ɹ�������ֵ��ʾ����ʧ��
********************************************************/
int CResProgram::Active()
{
    //����Ѿ�����,��ֱ�ӷ���
    if(CheckStatus() == ERR_SUCCESS)
    {
        m_nStatus = ERES_STATUS_ACTIVE;
        return ERR_SUCCESS;
    }
 
    m_nPid = m_PMgr.spawn(m_POptions);
    if (ACE_INVALID_PID == m_nPid)
    {
        return ERR_FAIL;
    }

    return CResObject::Active();
}

/********************************************************
* ����: CResProgram::DeActive
* ����: ��Դ��ȥ�����
* ����:
* ���:
* ����: int �Ƿ�ȥ����ɹ���0��ʾ�ɹ�������ֵ��ʾȥ����ʧ��
********************************************************/
int CResProgram::DeActive()
{
    if(CheckStatus() != ERR_SUCCESS)
    {
        return ERR_SUCCESS;
    }

    ACE::terminate_process(m_nPid);

    return CResObject::DeActive();
}
