#ifndef __RAID_DEV_CTL_H__
#define __RAID_DEV_CTL_H__

#include <afxwin.h>
#include <basetsd.h>
#include <setupapi.h> // Used for SetupDiXxx functions
#include <cfgmgr32.h> // Used for CM_Xxxx functions


//Raid设备控制类
class CRaidDeviceControl
{
public:

	CRaidDeviceControl();
	virtual ~CRaidDeviceControl();

	BOOL Refresh();     //刷新Raid设备
	BOOL Enable();		//启用Raid设备
	BOOL Disable();		//禁用Raid设备

protected:
	BOOL RaidDevStateChange(DWORD NewState); //改变Raid设备的状态
	BOOL StateChange(DWORD NewState);        //改变设备的状态
	
	BOOL GetRegistryProperty(HDEVINFO  DeviceInfoSet,  //得到设备注册表属性
		PSP_DEVINFO_DATA  DeviceInfoData,
		ULONG Property,
		PVOID Buffer,
		PULONG Length);
	
	BOOL ConstructDeviceName(HDEVINFO  DeviceInfoSet,  //得到设备的名称
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

