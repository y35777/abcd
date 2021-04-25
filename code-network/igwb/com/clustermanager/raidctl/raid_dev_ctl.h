#ifndef __RAID_DEV_CTL_H__
#define __RAID_DEV_CTL_H__

#include <afxwin.h>
#include <basetsd.h>
#include <setupapi.h> // Used for SetupDiXxx functions
#include <cfgmgr32.h> // Used for CM_Xxxx functions


//Raid�豸������
class CRaidDeviceControl
{
public:

	CRaidDeviceControl();
	virtual ~CRaidDeviceControl();

	BOOL Refresh();     //ˢ��Raid�豸
	BOOL Enable();		//����Raid�豸
	BOOL Disable();		//����Raid�豸

protected:
	BOOL RaidDevStateChange(DWORD NewState); //�ı�Raid�豸��״̬
	BOOL StateChange(DWORD NewState);        //�ı��豸��״̬
	
	BOOL GetRegistryProperty(HDEVINFO  DeviceInfoSet,  //�õ��豸ע�������
		PSP_DEVINFO_DATA  DeviceInfoData,
		ULONG Property,
		PVOID Buffer,
		PULONG Length);
	
	BOOL ConstructDeviceName(HDEVINFO  DeviceInfoSet,  //�õ��豸������
		PSP_DEVINFO_DATA  DeviceInfoData,
		PVOID Buffer,
		PULONG Length);

private:
	HDEVINFO  m_hDevInfo;
	SP_DEVINFO_DATA m_DeviceInfoData;
	LPSTR  m_szRaidDevName;
	unsigned long   m_nNameLen;
};

#endif //__RAID_DEV_CTL_H__

