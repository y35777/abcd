#include "../include/frame.h"
#include "res_ibm_volume.h"
#include "winioctl.h"
#include "cluster_toolbox.h"
#include "resource.h"
#include "ace/DLL.h"

#ifndef FIVE_SECOND
#define FIVE_SECOND (5*1000)
#endif

/********************************************************
* ����: CResIBMVolume::CResIBMVolume
* ����: ���캯��
* ����: const char* const szDriveName : �������̷�
*       const char* const szResName   : ��Դ��
*       ERES_TYPE nResType            : ��Դ����
*       int nMaxLocalRecovery         : ��󱾵ػָ�����
* ���:
* ����:
********************************************************/
CResIBMVolume::CResIBMVolume(const char* const szDriveName,
                             const char* const szResName,
                             ERES_TYPE nResType,
                             int nMaxLocalRecovery):
CResVolume(szDriveName, szResName, nResType, nMaxLocalRecovery)
{
    //
}

/********************************************************
* ����: CResIBMVolume::~CResIBMVolume
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CResIBMVolume::~CResIBMVolume()
{
    //
}

/********************************************************
* ����: CResIBMVolume::Active
* ����: ��Դ�ļ����
* ����:
* ���:
* ����: int �Ƿ񼤻�ɹ���0��ʾ�ɹ�������ֵ��ʾ����ʧ��
********************************************************/
int CResIBMVolume::Active()
{
    if(m_nStatus != ERES_STATUS_ACTIVE)
    {
        //�ӹ�IBM RAID��Դ
        TakeOver();

        if(g_nOSVersion >= 5)
        {
            RefreshIBMRaid f = GetRefreshIBMRaid();
            if(NULL != f)
            {
                f();
                Sleep(FIVE_SECOND);
            }
        }
    
        return CResVolume::Active();
    }

    return ERR_SUCCESS;
}

RefreshIBMRaid CResIBMVolume::GetRefreshIBMRaid()
{
    static ACE_Mutex mutex;
    static RefreshIBMRaid f = NULL;
    static ACE_DLL dll;

    mutex.acquire();

    if(NULL != f)
    {
        mutex.release();
        return f;
    }

    int nRet = dll.open("raidctl.dll");
    if(0 != nRet)
    {
        mutex.release();
        return NULL;
    }

    f = (RefreshIBMRaid)dll.symbol("RefreshIBMRaid");

    mutex.release();

    return f;
}
/********************************************************
* ����: CResIBMVolume::TakeOver
* ����: �ӹ�RAIDӲ����Դ
* ����:
* ���:
* ����: BOOL �Ƿ�ӹܳɹ�
********************************************************/
BOOL CResIBMVolume::TakeOver()
{
    char szWorkDirectory[MAX_PATH];
    char szExeName[MAX_PATH];

    GetSystemDirectory(szWorkDirectory, MAX_PATH);
    sprintf(szExeName, "%s\\ipshahto.exe ", szWorkDirectory);

    int nRet = ExecuteProcess(szExeName);
    if(ERR_SUCCESS == nRet)
    {
        TRACE(MTS_CLSTR, S_EXECUTE_IPSHAHTO_SUCCESS);
    }
    else
    {
        TRACE(MTS_CLSTR, S_EXECUTE_IPSHAHTO_FAIL);
    }

    return TRUE;
}

