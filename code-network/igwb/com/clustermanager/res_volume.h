#ifndef __RESOURCE_VOLUME_H__
#define __RESOURCE_VOLUME_H__

#include "res_object.h"

class CResVolume : public CResObject
{
public:

    CResVolume(const char* const szDriveName,
               const char* const szResName,
               ERES_TYPE nResType,
               int nMaxLocalRecovery);
    virtual ~CResVolume();

    virtual int Create();       //资源创建函数
    virtual int CheckStatus();  //资源的状态检查函数
    virtual int Active();       //资源的激活函数
    virtual int DeActive();     //资源的去激活函数

protected:
    //将指定的pszNtHardName设备指向szDriveName盘符
    BOOL AddDrive(const char* const pszDriveName,
                  const char* const pszNtHardName);
    //将卷标为m_szResName的设备指向m_szDriveName盘符
    BOOL AddDrive();
    //刷新驱动器资源
    BOOL RefreshDrive(const char* const pszDriveName, 
                      BOOL bErrTrace = TRUE);
    //锁定驱动器
    BOOL LockVolume(HANDLE hDisk);
    //锁定驱动器
    HANDLE LockVolume(const char* const szDir);
    //盘符是否还存在
    BOOL IsDriveAlive(const char* const pszDriveName, 
                      char* const szOutNtDeviceName = NULL,
                      int nLen = 0);
    //确认硬盘设备接入
    BOOL ChechDriveVerity(const char* const pszNtDeviceName);
    //删除指定盘符
    BOOL DeleteDrive(const char* const pszDriveName);
    //取指定逻辑盘的卷标名
    STRING GetVolumeLabel(const char* const szDriveName);

    //盘符
#define DRIVE_NAME_LENGTH   10
    char    m_szDriveName[DRIVE_NAME_LENGTH];
    char    m_szNtDeviceName[MAX_PATH];

    //锁定卷资源的句柄
    //HANDLE  m_hDevice;

    int     m_nMaxSearchDevice;
    int     m_nMaxSearchPartition;
    int     m_nDeviceNo;
    int     m_nPartitionNo;
    
    //启动时驱动器是否存在，如果存在，则在退出前恢复
    BOOL    m_bDriveExist;
};


#endif //__RESOURCE_VOLUME_H__
