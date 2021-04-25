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

    virtual int Create();       //��Դ��������
    virtual int CheckStatus();  //��Դ��״̬��麯��
    virtual int Active();       //��Դ�ļ����
    virtual int DeActive();     //��Դ��ȥ�����

protected:
    //��ָ����pszNtHardName�豸ָ��szDriveName�̷�
    BOOL AddDrive(const char* const pszDriveName,
                  const char* const pszNtHardName);
    //�����Ϊm_szResName���豸ָ��m_szDriveName�̷�
    BOOL AddDrive();
    //ˢ����������Դ
    BOOL RefreshDrive(const char* const pszDriveName, 
                      BOOL bErrTrace = TRUE);
    //����������
    BOOL LockVolume(HANDLE hDisk);
    //����������
    HANDLE LockVolume(const char* const szDir);
    //�̷��Ƿ񻹴���
    BOOL IsDriveAlive(const char* const pszDriveName, 
                      char* const szOutNtDeviceName = NULL,
                      int nLen = 0);
    //ȷ��Ӳ���豸����
    BOOL ChechDriveVerity(const char* const pszNtDeviceName);
    //ɾ��ָ���̷�
    BOOL DeleteDrive(const char* const pszDriveName);
    //ȡָ���߼��̵ľ����
    STRING GetVolumeLabel(const char* const szDriveName);

    //�̷�
#define DRIVE_NAME_LENGTH   10
    char    m_szDriveName[DRIVE_NAME_LENGTH];
    char    m_szNtDeviceName[MAX_PATH];

    //��������Դ�ľ��
    //HANDLE  m_hDevice;

    int     m_nMaxSearchDevice;
    int     m_nMaxSearchPartition;
    int     m_nDeviceNo;
    int     m_nPartitionNo;
    
    //����ʱ�������Ƿ���ڣ�������ڣ������˳�ǰ�ָ�
    BOOL    m_bDriveExist;
};


#endif //__RESOURCE_VOLUME_H__
