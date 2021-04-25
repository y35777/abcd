#include "../include/frame.h"
#include "res_volume.h"
#include "winioctl.h"
#include "resource.h"


/********************************************************
* ����: CResVolume::CResVolume
* ����: ���캯��
* ����: const char* const szDriveName : �������̷�
*       const char* const szResName   : ��Դ��
*       ERES_TYPE nResType            : ��Դ����
*       int nMaxLocalRecovery         : ��󱾵ػָ�����
* ���:
* ����:
********************************************************/
CResVolume::CResVolume(const char* const szDriveName,
                       const char* const szResName,
                       ERES_TYPE nResType,
                       int nMaxLocalRecovery):
CResObject(szResName, nResType, nMaxLocalRecovery)
{
    strncpy(m_szDriveName, szDriveName, DRIVE_NAME_LENGTH);
    m_szDriveName[DRIVE_NAME_LENGTH - 1] = '\0';
    m_szNtDeviceName[0]   = '\0';

    m_nMaxSearchDevice    = 10;
    m_nMaxSearchPartition = 5;
    m_nDeviceNo           = 0;
    m_nPartitionNo        = 1;

    m_bDriveExist         = FALSE; 
}

/********************************************************
* ����: CResVolume::~CResVolume
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CResVolume::~CResVolume()
{
    if(m_bDriveExist && (m_szNtDeviceName[0] != '\0'))
    {
        AddDrive(m_szDriveName, m_szNtDeviceName);
    }
}

//����Ϊ32���ֽڣ�������Ч��곤��Ϊ50���ֽ�
#define INVALID_VOLUME_LABEL    "12345678901234567890123456789012345678901234567890"
/********************************************************
* ����: CResVolume::Create
* ����: ��Դ��������
* ����:
* ���:
* ����: int ��Դ�����Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CResVolume::Create()
{
    //������̷����ڣ�ɾ���������̷�
    if (IsDriveAlive(m_szDriveName, m_szNtDeviceName))
    {
        //��Դ�������óɲ���ϵͳ���ڵ�������
        char szSystemInfo[MAX_PATH];
        if(GetSystemDirectory(szSystemInfo, MAX_PATH) > 0)
        {
            if(szSystemInfo[0] == m_szDriveName[0])
            {
                TRACE(MTS_CLSTR, S_VOLUME_IS_SYSTEM_DRV, m_szDriveName);
                return ERR_FAIL;
            }
        }

        //ȡ�����
        STRING strVolumeLabel = GetVolumeLabel(m_szDriveName);

         //������������Դ��һ�£���ɾ���̷�
        if((ACE_OS::strcasecmp(strVolumeLabel.c_str(), m_szResName) == 0)
           || (strVolumeLabel.compare(INVALID_VOLUME_LABEL) == 0))
        {
            m_bDriveExist = TRUE;

            //ɾ���̷�
            DeleteDrive(m_szDriveName);
        }
        else
        {
            TRACE(MTS_CLSTR, S_VOLUME_LABEL_DISMATCH, 
                  m_szResName, strVolumeLabel.c_str());
            return ERR_FAIL;
        }
    }

    return CResObject::Create();
}

/********************************************************
* ����: CResVolume::CheckStatus
* ����: ��Դ��״̬��麯��
* ����:
* ���:
* ����: int �Ƿ���ڲ��ɻָ��Ĺ��ϣ�0��ʾ�޹��ϣ�����ֵ��ʾ��⵽����
********************************************************/
int CResVolume::CheckStatus()
{
    char szFileName[MAX_PATH];
    HANDLE hFile;
    sprintf(szFileName, "%s\\test.dat", m_szDriveName);
    hFile = CreateFile(
        szFileName,
        GENERIC_READ|GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        CREATE_ALWAYS,
        0,
        NULL);

    if(INVALID_HANDLE_VALUE != hFile)
    {
        CloseHandle(hFile);
        return ERR_SUCCESS;
    }

    m_nStatus = ERES_STATUS_FAILURE;
    return ERR_FAIL;
}

/********************************************************
* ����: CResVolume::Active
* ����: ��Դ�ļ����
* ����:
* ���:
* ����: int �Ƿ񼤻�ɹ���0��ʾ�ɹ�������ֵ��ʾ����ʧ��
********************************************************/
int CResVolume::Active()
{
    if(m_nStatus != ERES_STATUS_ACTIVE)
    {
        //������̷������ڣ�����������̷�
        if (!IsDriveAlive(m_szDriveName))
        {
            //����������̷�
            if (!AddDrive())
            {
                return ERR_FAIL;
            }
        }

        //ˢ���豸
        if (!RefreshDrive(m_szDriveName))
        {
            DeleteDrive(m_szDriveName);
            return ERR_FAIL;
        }
    }

    return CResObject::Active();
}

/********************************************************
* ����: CResVolume::DeActive
* ����: ��Դ��ȥ�����
* ����:
* ���:
* ����: int �Ƿ�ȥ����ɹ���0��ʾ�ɹ�������ֵ��ʾȥ����ʧ��
********************************************************/
int CResVolume::DeActive()
{
    if(m_nStatus == ERES_STATUS_ACTIVE)
    {
        RefreshDrive(m_szDriveName);

        //ɾ���̷�
        DeleteDrive(m_szDriveName);
    }

    return CResObject::DeActive();
}

/********************************************************
* ����: CResVolume::AddDrive
* ����: ����߼������̷�
* ����: szDriveName : �����������磺D:
*       pszNtHardName : ��������NTӲ����
* ���:
* ����: BOOL ��ӳɹ�ΪTRUE������ΪFALSE
********************************************************/
BOOL CResVolume::AddDrive(const char* const szDriveName,
                          const char* const pszNtHardName)
{
    int fResult;

    //����߼������̷�
    fResult = DefineDosDevice(DDD_RAW_TARGET_PATH, szDriveName,
                              pszNtHardName);

    if (!fResult)
    {
        long lReErr = GetLastError();
        TRACE(MTS_CLSTR, S_ADD_DRIVE_FAIL,
              szDriveName, pszNtHardName, lReErr);
    }
    return fResult;
}

#define MAX_DEVIVCE_NAME_LEN    100
/********************************************************
* ����: CResVolume::AddDrive
* ����: �����Ϊm_szResName���豸ָ��m_szDriveName�̷�
* ����: 
* ���:
* ����: BOOL ��ӳɹ�ΪTRUE������ΪFALSE
********************************************************/
BOOL CResVolume::AddDrive()
{
    char szDeviceName[MAX_DEVIVCE_NAME_LEN];

    //Ѱ�Ҿ��Ϊm_szResName���豸
    for (int nDevice = m_nDeviceNo; 
         nDevice < m_nMaxSearchDevice;
         nDevice++)
    {
        //ȷ���豸����
        SNPRINTF(szDeviceName, MAX_DEVIVCE_NAME_LEN, 
                 "\\\\.\\PhysicalDrive%d",
                 nDevice);
        szDeviceName[MAX_DEVIVCE_NAME_LEN - 1] = '\0';
        if (!ChechDriveVerity(szDeviceName))
        {
            continue;
        }

        for (int nPartition = m_nPartitionNo;
             nPartition <= m_nMaxSearchPartition; 
             nPartition++)
        {
            //���豸ӳ�䵽�̷�m_szDriveName��
            SNPRINTF(szDeviceName, MAX_DEVIVCE_NAME_LEN, 
                     "\\Device\\HardDisk%d\\Partition%d",
                     nDevice, nPartition);
            szDeviceName[MAX_DEVIVCE_NAME_LEN - 1] = '\0';

            if(AddDrive(m_szDriveName, szDeviceName))
            {
                //ȡ�����
                STRING strVolumeLabel = GetVolumeLabel(m_szDriveName);

                //������������Դ��һ�£�������̷�
                if(ACE_OS::strcasecmp(strVolumeLabel.c_str(), m_szResName) == 0)
                {
                    m_nDeviceNo    = nDevice;
                    m_nPartitionNo = nPartition;
                    strncpy(m_szNtDeviceName, szDeviceName, MAX_PATH);
                    m_szNtDeviceName[MAX_PATH - 1] = '\0';
                    return TRUE;
                }

                //ɾ���̷�
                DeleteDrive(m_szDriveName);
            }
        }
    }
    
    TRACE(MTS_CLSTR, S_CANNOT_FIND_VOLUME, m_szResName);

    return FALSE;
}

/********************************************************
* ����: CResVolume::RefreshDrive
* ����: ˢ����������Դ
* ����: pszDriveName : ��������
*       bErrTrace    : �Ƿ�ͨ��TRACE���������Ϣ,ȱʡΪTRUE
* ���:
* ����: BOOL �Ƿ�ɹ�
********************************************************/
BOOL CResVolume::RefreshDrive(const char* const pszDriveName,
                              BOOL bErrTrace)
{
    HANDLE hDevice;
    char szDeviceName[MAX_PATH];

    //����������Դ�豸
    sprintf(szDeviceName, "\\\\.\\%s", pszDriveName);
    hDevice = CreateFile(szDeviceName,
                         GENERIC_READ | GENERIC_WRITE,
                         FILE_SHARE_READ|FILE_SHARE_WRITE,
                         NULL,
                         OPEN_EXISTING,
                         0,
                         NULL);

    if(hDevice != INVALID_HANDLE_VALUE)
    {//����ɹ��������ˢ�²���
        DWORD ReturnedByteCount;
        if(!DeviceIoControl(hDevice,
                            FSCTL_DISMOUNT_VOLUME,
                            NULL,
                            0,
                            NULL,
                            0,
                            &ReturnedByteCount,
                            NULL))
        {//���ˢ�²���ʧ�ܣ��򷵻�FALSE
            CloseHandle(hDevice);
            if(bErrTrace)
            {
                TRACE(MTS_CLSTR, S_FAILED_TO_REFRESH_VOLUME,
                      szDeviceName, GetLastError());
            }
            return FALSE;
        }
        CloseHandle(hDevice);
        return TRUE;
    }
    else if(bErrTrace)
    {
        TRACE(MTS_CLSTR, S_FAILED_TO_REFRESH_VOLUME,
              szDeviceName, GetLastError());
    }

    return FALSE;
}


/********************************************************
* ����: CResVolume::LockVolume
* ����: ����������
* ����: hDisk : �������豸���
* ���:
* ����: BOOL �����Ƿ�ɹ�
********************************************************/
BOOL CResVolume::LockVolume(HANDLE hDisk)
{
    DWORD ReturnedByteCount;
    char buff[3000];
    return DeviceIoControl(hDisk,           // device we are querying
                        IOCTL_DISK_RESERVE, // operation to perform
                        NULL, 0,            // no input buffer, so pass zero
                        buff, 3000,         // output buffer
                        &ReturnedByteCount, // discard count of bytes returned
                        (LPOVERLAPPED) NULL);  // synchronous I/O
}

/********************************************************
* ����: CResVolume::LockVolume
* ����: ����������
* ����: szDir : ��������
* ���:
* ����: HANDLE ��������������������
********************************************************/
HANDLE CResVolume::LockVolume(const char* const szDir)
{
    char szDeviceName[MAX_PATH];
    HANDLE hDevice;
    sprintf(szDeviceName, "\\\\.\\%s", szDir);
    hDevice = CreateFile(szDeviceName,
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ|FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        0,
                        NULL);

    if(hDevice != INVALID_HANDLE_VALUE)
    {
        if(!LockVolume(hDevice))
        {
            TRACE(MTS_CLSTR, S_FAILED_TO_LOCK_VOLUME,
                  szDeviceName, GetLastError());
            CloseHandle(hDevice);
            hDevice = INVALID_HANDLE_VALUE;
        }
    }
    else
    {
        TRACE(MTS_CLSTR, S_FAILED_TO_LOCK_VOLUME,
              szDeviceName, GetLastError());
    }

    return hDevice;
}

/********************************************************
* ����: CResVolume::ChechDriveVerity
* ����: ȷ��Ӳ���豸����
* ����: szNtDeviceName : ��������NT�豸��
* ���:
* ����: BOOL �����Ƿ�ɹ�
********************************************************/
BOOL CResVolume::ChechDriveVerity(const char* const szNtDeviceName)
{
    HANDLE hDevice;               // handle to the drive to be examined
    BOOL bResult;                 // results flag
    DWORD junk;                   // discard results

    hDevice = CreateFile(szNtDeviceName,                    // drive to open
                       GENERIC_READ | GENERIC_WRITE,        // don't need any access to the drive
                       FILE_SHARE_READ | FILE_SHARE_WRITE,  // share mode
                       NULL,                                // default security attributes
                       OPEN_EXISTING,                       // disposition
                       0,                                   // file attributes
                       NULL);                               // don't copy any file's attributes

    if (hDevice == INVALID_HANDLE_VALUE)
    { 
        return FALSE;
    }

    bResult = DeviceIoControl(hDevice,                  // the device we are querying
                            IOCTL_STORAGE_CHECK_VERIFY, // operation to perform
                            NULL, 0,                    // no input buffer, so we pass zero
                            NULL, 0,                    // no input buffer, so we pass zero
                            &junk,                      // discard the count of bytes returned
                            (LPOVERLAPPED) NULL);       // synchronous I/O

    CloseHandle(hDevice);         // we're done with the handle

    return (bResult);
}

/********************************************************
* ����: CResVolume::IsDriveAlive
* ����: ����߼������̷��Ƿ񻹴���
* ����: szDriveName       : �߼������̷�
*       szOutNtDeviceName : �������NT�豸������
*       nLen              : szOutNtDeviceName�ĳ���
* ���:
* ����: BOOL �̷��Ƿ����
********************************************************/
BOOL CResVolume::IsDriveAlive(const char* const szDriveName, 
                              char* const szOutNtDeviceName,
                              int nLen)
{
    int nResult = 0;

    //��ѯ�Ƿ���ڸ��̷������������ȡNT�豸��
    char szNtDeviceName[MAX_PATH];
    szNtDeviceName[0] = '\0';
    nResult = QueryDosDevice(szDriveName, szNtDeviceName,
                             MAX_PATH);
    szNtDeviceName[MAX_PATH - 1] = '\0';

    if((nResult > 0) && ('\0' != szNtDeviceName[0]) && 
       (NULL != szOutNtDeviceName) && 
       (nLen > strlen(szNtDeviceName)))
    {
        strcpy(szOutNtDeviceName, szNtDeviceName);
        szOutNtDeviceName[nLen - 1] = '\0';
    }

    return nResult > 0;
}

/********************************************************
* ����: CResVolume::DeleteDrive
* ����: ��������̷��Ӳ���ϵͳ�ж�̬ɾ��
* ����: szDriveName : ����������������磺D:
* ���:
* ����: BOOL ɾ���ɹ�ΪTRUE������ΪFALSE
********************************************************/
BOOL CResVolume::DeleteDrive(const char* const szDriveName)
{
    int fResult;

    //��ѯ�Ƿ���ڸ��̷������������ȡNT�豸��
    char szNtDeviceName[MAX_PATH];
    fResult = QueryDosDevice(szDriveName, szNtDeviceName,
                             MAX_PATH);
    szNtDeviceName[MAX_PATH - 1] = '\0';

    if (fResult)
    { //������ڸ��̷�����ɾ�����̷�
        fResult = DefineDosDevice (DDD_RAW_TARGET_PATH |
                            DDD_REMOVE_DEFINITION |
                            DDD_EXACT_MATCH_ON_REMOVE,
                            szDriveName, szNtDeviceName);
    }
    else
    {//���򷵻�TRUE������ʾ��ɾ�����̷�
        fResult = TRUE;
    }

    return fResult;
}

/********************************************************
* ����: CResVolume::GetVolumeLabel
* ����: ȡָ���߼��̵ľ����
* ����: szDriveName : �������̷�
* ���:
* ����: STRING �����
********************************************************/
STRING CResVolume::GetVolumeLabel(const char* const szDriveName)
{
    char szVolumeNameBuffer[51] = INVALID_VOLUME_LABEL;   //��ϵͳAPI��ȡ�ľ��
    STRING strOutLabal(INVALID_VOLUME_LABEL);

    //ȡָ���̷��ľ��
    int fResult = GetVolumeInformation(
            szDriveName,
            (LPTSTR) szVolumeNameBuffer,       // volume name buffer
            sizeof(szVolumeNameBuffer) - 1,    // length of name buffer
            NULL,    // volume serial number
            NULL,    // file system name buffer
            NULL,    // file system name buffer
            NULL,    // file system name buffer
            NULL     // length of file system name buffer
    );

    if(fResult)
    {
        strOutLabal = szVolumeNameBuffer;
    }

    return strOutLabal;
}
