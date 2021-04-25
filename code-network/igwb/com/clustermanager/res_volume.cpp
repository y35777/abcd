#include "../include/frame.h"
#include "res_volume.h"
#include "winioctl.h"
#include "resource.h"


/********************************************************
* 名称: CResVolume::CResVolume
* 功能: 构造函数
* 输入: const char* const szDriveName : 驱动器盘符
*       const char* const szResName   : 资源名
*       ERES_TYPE nResType            : 资源类型
*       int nMaxLocalRecovery         : 最大本地恢复次数
* 输出:
* 返回:
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
* 名称: CResVolume::~CResVolume
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CResVolume::~CResVolume()
{
    if(m_bDriveExist && (m_szNtDeviceName[0] != '\0'))
    {
        AddDrive(m_szDriveName, m_szNtDeviceName);
    }
}

//卷标最长为32个字节，这里无效卷标长度为50个字节
#define INVALID_VOLUME_LABEL    "12345678901234567890123456789012345678901234567890"
/********************************************************
* 名称: CResVolume::Create
* 功能: 资源创建函数
* 输入:
* 输出:
* 返回: int 资源创建是否成功，0表示成功，非零值表示失败
********************************************************/
int CResVolume::Create()
{
    //如果该盘符存在，删除驱动器盘符
    if (IsDriveAlive(m_szDriveName, m_szNtDeviceName))
    {
        //资源不能配置成操作系统所在的驱动器
        char szSystemInfo[MAX_PATH];
        if(GetSystemDirectory(szSystemInfo, MAX_PATH) > 0)
        {
            if(szSystemInfo[0] == m_szDriveName[0])
            {
                TRACE(MTS_CLSTR, S_VOLUME_IS_SYSTEM_DRV, m_szDriveName);
                return ERR_FAIL;
            }
        }

        //取卷标名
        STRING strVolumeLabel = GetVolumeLabel(m_szDriveName);

         //如果卷标名与资源名一致，则删除盘符
        if((ACE_OS::strcasecmp(strVolumeLabel.c_str(), m_szResName) == 0)
           || (strVolumeLabel.compare(INVALID_VOLUME_LABEL) == 0))
        {
            m_bDriveExist = TRUE;

            //删除盘符
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
* 名称: CResVolume::CheckStatus
* 功能: 资源的状态检查函数
* 输入:
* 输出:
* 返回: int 是否存在不可恢复的故障，0表示无故障，非零值表示检测到故障
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
* 名称: CResVolume::Active
* 功能: 资源的激活函数
* 输入:
* 输出:
* 返回: int 是否激活成功，0表示成功，非零值表示启动失败
********************************************************/
int CResVolume::Active()
{
    if(m_nStatus != ERES_STATUS_ACTIVE)
    {
        //如果该盘符不存在，添加驱动器盘符
        if (!IsDriveAlive(m_szDriveName))
        {
            //添加驱动器盘符
            if (!AddDrive())
            {
                return ERR_FAIL;
            }
        }

        //刷新设备
        if (!RefreshDrive(m_szDriveName))
        {
            DeleteDrive(m_szDriveName);
            return ERR_FAIL;
        }
    }

    return CResObject::Active();
}

/********************************************************
* 名称: CResVolume::DeActive
* 功能: 资源的去激活函数
* 输入:
* 输出:
* 返回: int 是否去激活成功，0表示成功，非零值表示去激活失败
********************************************************/
int CResVolume::DeActive()
{
    if(m_nStatus == ERES_STATUS_ACTIVE)
    {
        RefreshDrive(m_szDriveName);

        //删除盘符
        DeleteDrive(m_szDriveName);
    }

    return CResObject::DeActive();
}

/********************************************************
* 名称: CResVolume::AddDrive
* 功能: 添加逻辑驱动盘符
* 输入: szDriveName : 驱动器名，如：D:
*       pszNtHardName : 驱动器的NT硬件名
* 输出:
* 返回: BOOL 添加成功为TRUE，否则为FALSE
********************************************************/
BOOL CResVolume::AddDrive(const char* const szDriveName,
                          const char* const pszNtHardName)
{
    int fResult;

    //添加逻辑驱动盘符
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
* 名称: CResVolume::AddDrive
* 功能: 将卷标为m_szResName的设备指向m_szDriveName盘符
* 输入: 
* 输出:
* 返回: BOOL 添加成功为TRUE，否则为FALSE
********************************************************/
BOOL CResVolume::AddDrive()
{
    char szDeviceName[MAX_DEVIVCE_NAME_LEN];

    //寻找卷标为m_szResName的设备
    for (int nDevice = m_nDeviceNo; 
         nDevice < m_nMaxSearchDevice;
         nDevice++)
    {
        //确认设备接入
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
            //将设备映射到盘符m_szDriveName中
            SNPRINTF(szDeviceName, MAX_DEVIVCE_NAME_LEN, 
                     "\\Device\\HardDisk%d\\Partition%d",
                     nDevice, nPartition);
            szDeviceName[MAX_DEVIVCE_NAME_LEN - 1] = '\0';

            if(AddDrive(m_szDriveName, szDeviceName))
            {
                //取卷标名
                STRING strVolumeLabel = GetVolumeLabel(m_szDriveName);

                //如果卷标名与资源名一致，则添加盘符
                if(ACE_OS::strcasecmp(strVolumeLabel.c_str(), m_szResName) == 0)
                {
                    m_nDeviceNo    = nDevice;
                    m_nPartitionNo = nPartition;
                    strncpy(m_szNtDeviceName, szDeviceName, MAX_PATH);
                    m_szNtDeviceName[MAX_PATH - 1] = '\0';
                    return TRUE;
                }

                //删除盘符
                DeleteDrive(m_szDriveName);
            }
        }
    }
    
    TRACE(MTS_CLSTR, S_CANNOT_FIND_VOLUME, m_szResName);

    return FALSE;
}

/********************************************************
* 名称: CResVolume::RefreshDrive
* 功能: 刷新驱动器资源
* 输入: pszDriveName : 驱动器名
*       bErrTrace    : 是否通过TRACE输出错误信息,缺省为TRUE
* 输出:
* 返回: BOOL 是否成功
********************************************************/
BOOL CResVolume::RefreshDrive(const char* const pszDriveName,
                              BOOL bErrTrace)
{
    HANDLE hDevice;
    char szDeviceName[MAX_PATH];

    //打开驱动器资源设备
    sprintf(szDeviceName, "\\\\.\\%s", pszDriveName);
    hDevice = CreateFile(szDeviceName,
                         GENERIC_READ | GENERIC_WRITE,
                         FILE_SHARE_READ|FILE_SHARE_WRITE,
                         NULL,
                         OPEN_EXISTING,
                         0,
                         NULL);

    if(hDevice != INVALID_HANDLE_VALUE)
    {//如果成功，则进行刷新操作
        DWORD ReturnedByteCount;
        if(!DeviceIoControl(hDevice,
                            FSCTL_DISMOUNT_VOLUME,
                            NULL,
                            0,
                            NULL,
                            0,
                            &ReturnedByteCount,
                            NULL))
        {//如果刷新操作失败，则返回FALSE
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
* 名称: CResVolume::LockVolume
* 功能: 锁定驱动器
* 输入: hDisk : 驱动器设备句柄
* 输出:
* 返回: BOOL 锁定是否成功
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
* 名称: CResVolume::LockVolume
* 功能: 锁定驱动器
* 输入: szDir : 驱动器名
* 输出:
* 返回: HANDLE 返回锁定驱动器的名柄
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
* 名称: CResVolume::ChechDriveVerity
* 功能: 确认硬盘设备接入
* 输入: szNtDeviceName : 驱动器的NT设备名
* 输出:
* 返回: BOOL 操作是否成功
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
* 名称: CResVolume::IsDriveAlive
* 功能: 检测逻辑驱动盘符是否还存在
* 输入: szDriveName       : 逻辑驱动盘符
*       szOutNtDeviceName : 向外输出NT设备号描述
*       nLen              : szOutNtDeviceName的长度
* 输出:
* 返回: BOOL 盘符是否存在
********************************************************/
BOOL CResVolume::IsDriveAlive(const char* const szDriveName, 
                              char* const szOutNtDeviceName,
                              int nLen)
{
    int nResult = 0;

    //查询是否存在该盘符，如果存在则取NT设备名
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
* 名称: CResVolume::DeleteDrive
* 功能: 将输入的盘符从操作系统中动态删除
* 输入: szDriveName : 输入的驱动器名，如：D:
* 输出:
* 返回: BOOL 删除成功为TRUE，否则为FALSE
********************************************************/
BOOL CResVolume::DeleteDrive(const char* const szDriveName)
{
    int fResult;

    //查询是否存在该盘符，如果存在则取NT设备名
    char szNtDeviceName[MAX_PATH];
    fResult = QueryDosDevice(szDriveName, szNtDeviceName,
                             MAX_PATH);
    szNtDeviceName[MAX_PATH - 1] = '\0';

    if (fResult)
    { //如果存在该盘符，则删除该盘符
        fResult = DefineDosDevice (DDD_RAW_TARGET_PATH |
                            DDD_REMOVE_DEFINITION |
                            DDD_EXACT_MATCH_ON_REMOVE,
                            szDriveName, szNtDeviceName);
    }
    else
    {//否则返回TRUE，并提示已删除该盘符
        fResult = TRUE;
    }

    return fResult;
}

/********************************************************
* 名称: CResVolume::GetVolumeLabel
* 功能: 取指定逻辑盘的卷标名
* 输入: szDriveName : 驱动器盘符
* 输出:
* 返回: STRING 卷标名
********************************************************/
STRING CResVolume::GetVolumeLabel(const char* const szDriveName)
{
    char szVolumeNameBuffer[51] = INVALID_VOLUME_LABEL;   //从系统API中取的卷标
    STRING strOutLabal(INVALID_VOLUME_LABEL);

    //取指定盘符的卷标
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
