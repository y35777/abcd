#include "raid_dev_ctl.h"
#include <string>


typedef std::string STRING;

///////////////////////////////////////////////////////////
// CRaidDeviceControl -- Raid�豸������
///////////////////////////////////////////////////////////
//RAID��SCSI�豸��GUID{4D36E97B-E325-11CE-BFC1-08002BE10318}
DEFINE_GUID(GUID_CLASS_SCSI_RAID, 
			0x4D36E97B, 0xE325, 0x11CE, 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18);


/******************************************************
// ������: 	CRaidDeviceControl::CRaidDeviceControl
// ����:    chenliangwei
// ʱ��:    2002-11-20
// ����:    ���캯��,�ú�����ʼ���ڲ���Ա
******************************************************/
CRaidDeviceControl::CRaidDeviceControl()
{
    m_hDevInfo = INVALID_HANDLE_VALUE;
	
    if (INVALID_HANDLE_VALUE == (m_hDevInfo = 
		SetupDiGetClassDevs(&GUID_CLASS_SCSI_RAID,NULL,NULL,
		DIGCF_PRESENT)))
    {
		return;
	}
    
    m_DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);   

	m_szRaidDevName = NULL;
	m_nNameLen = 0;
}

/******************************************************
// ������: 	CRaidDeviceControl::~CRaidDeviceControl
// ����:    chenliangwei
// ʱ��:    2002-11-20
// ����:    ��������,�ú����ͷ��ڲ���Ա

******************************************************/
CRaidDeviceControl::~CRaidDeviceControl()
{
	if (m_szRaidDevName != NULL)
	{
		delete []m_szRaidDevName;
	}

	if (INVALID_HANDLE_VALUE != m_hDevInfo)
	{
		SetupDiDestroyDeviceInfoList(m_hDevInfo);		
	}
}



/******************************************************
// ������: 	CRaidDeviceControl::Refresh
// ����:    chenliangwei
// ʱ��:    2002-11-20
// ����:    ˢ��Raid�豸
******************************************************/
BOOL CRaidDeviceControl::Refresh()
{
	return RaidDevStateChange(DICS_PROPCHANGE);
}

/******************************************************
// ������: 	CRaidDeviceControl::Enable
// ����:    chenliangwei
// ʱ��:    2002-11-20
// ����:    ����Raid�豸
******************************************************/
BOOL CRaidDeviceControl::Enable()
{
	return RaidDevStateChange(DICS_ENABLE);
}

/******************************************************
// ������: 	CRaidDeviceControl::Disable
// ����:    chenliangwei
// ʱ��:    2002-11-20
// ����:    ����Raid�豸
******************************************************/

BOOL CRaidDeviceControl::Disable()
{
	return RaidDevStateChange(DICS_DISABLE);
}


/******************************************************
// ������: 	CRaidDeviceControl::RaidDevStateChange
// ����:    chenliangwei
// ʱ��:    2002-11-20
// ����:    �ı�Raid�豸��״̬
// ����:
//       ����1 :DWORD NewState
// ����ֵ: BOOL 
******************************************************/
BOOL CRaidDeviceControl::RaidDevStateChange(DWORD NewState)
{

	if (INVALID_HANDLE_VALUE == m_hDevInfo)
	{
		return FALSE;
	}

	STRING strNewState = "";

	switch (NewState)
	{
	case  DICS_ENABLE:
		strNewState = "Enable";
		break;
		
	case  DICS_DISABLE:
		strNewState = "Disable";
		break;

	case  DICS_PROPCHANGE:
		strNewState = "Refresh";
		break;

	default:
		return FALSE;
	}


	DWORD i, Status, Problem;
    //
    // Enumerate though all the devices.
    //
    for (i = 0; SetupDiEnumDeviceInfo(m_hDevInfo, i, &m_DeviceInfoData); i++)
    {    
        if (CR_SUCCESS != CM_Get_DevNode_Status(&Status, &Problem,
            m_DeviceInfoData.DevInst,0))
        {
            continue;
        } 
  
        ConstructDeviceName(m_hDevInfo,&m_DeviceInfoData,
			&m_szRaidDevName,
            &m_nNameLen);
		
		LPSTR szUpperRaidDevName= _strupr( _strdup( m_szRaidDevName ) ); 
		
        if ((strstr(szUpperRaidDevName, "IBM SERVERAID") != NULL)
			&& (strstr(szUpperRaidDevName, "CONTROLLER") != NULL))
        {
            if (!StateChange(NewState))
            {
				return FALSE;
            }
        }
		
		delete []szUpperRaidDevName;
    }

	return TRUE;
}


/******************************************************
// ������: 	CRaidDeviceControl::StateChange
// ����:    chenliangwei
// ʱ��:    2002-11-20
// ����:    �ı��豸��״̬
// ����:
//       ����1 :DWORD NewState
// ����ֵ: BOOL 
******************************************************/
BOOL CRaidDeviceControl::StateChange(DWORD NewState)
{
	
	if (INVALID_HANDLE_VALUE == m_hDevInfo)
	{
		return FALSE;
	}
	
    SP_PROPCHANGE_PARAMS PropChangeParams = {sizeof(SP_CLASSINSTALL_HEADER)};
	
    //
    // Set the PropChangeParams structure.
    //
    PropChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
    PropChangeParams.Scope = DICS_FLAG_GLOBAL;
    PropChangeParams.StateChange = NewState; 
    
    if (!SetupDiSetClassInstallParams(m_hDevInfo,
        &m_DeviceInfoData,
        (SP_CLASSINSTALL_HEADER *)&PropChangeParams,
        sizeof(PropChangeParams)))
    {
        return FALSE;
    }
    
    //
    // Call the ClassInstaller and perform the change.
    //
    if (!SetupDiCallClassInstaller(DIF_PROPERTYCHANGE,
        m_hDevInfo,
        &m_DeviceInfoData))
    {
        return FALSE;
    }
    
    return TRUE;	
}

/******************************************************
// ������: 	CRaidDeviceControl::GetRegistryProperty
// ����:    chenliangwei
// ʱ��:    2002-11-20
// ����:    �õ��豸ע�������
// ����:

******************************************************/
BOOL CRaidDeviceControl::GetRegistryProperty(HDEVINFO  DeviceInfoSet,
						 PSP_DEVINFO_DATA  DeviceInfoData,
						 ULONG Property,
						 PVOID Buffer,
						 PULONG Length)
{
	if (INVALID_HANDLE_VALUE == DeviceInfoSet)
	{
		return FALSE;
	}

    while (!SetupDiGetDeviceRegistryProperty(DeviceInfoSet,
        DeviceInfoData,
        Property,
        NULL,
        (PBYTE)*(TCHAR **)Buffer,
        *Length,
        Length
        ))
    {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            //
            // We need to change the buffer size.
            //
            if (*(LPSTR *)Buffer) 
                delete [](*(LPSTR *)Buffer);
            *(LPSTR *)Buffer = (char*)new char[*Length];
        }
        else
        {
            //
            // Unknown Failure.
            //
            return FALSE;
        }            
    }
    
    return (*(LPSTR *)Buffer)[0];
}

#define UnknownDevice _T("<Unknown Device>")
/******************************************************
// ������: 	CRaidDeviceControl::ConstructDeviceName
// ����:    chenliangwei
// ʱ��:    2002-11-20
// ����:    �õ��豸������
******************************************************/
BOOL CRaidDeviceControl::ConstructDeviceName( HDEVINFO DeviceInfoSet,
						PSP_DEVINFO_DATA DeviceInfoData,
						PVOID Buffer,
						PULONG Length)
{
	if (INVALID_HANDLE_VALUE == DeviceInfoSet)
	{
		return FALSE;
	}

    if (!GetRegistryProperty(DeviceInfoSet,
        DeviceInfoData,
        SPDRP_FRIENDLYNAME ,
        Buffer,
        Length))
    {
        if (!GetRegistryProperty(DeviceInfoSet,
            DeviceInfoData,
            SPDRP_DEVICEDESC ,
            Buffer,
            Length))
        {
            if (!GetRegistryProperty(DeviceInfoSet,
                DeviceInfoData,
                SPDRP_CLASS ,
                Buffer,
                Length))
            {
                if (!GetRegistryProperty(DeviceInfoSet,
                    DeviceInfoData,
                    SPDRP_CLASSGUID ,
                    Buffer,
                    Length))
                {
                    *Length = (strlen(UnknownDevice)+1)*sizeof(TCHAR);
                    *(LPSTR *)Buffer = (char*)new char[*Length];
                    strcpy(*(LPSTR *)Buffer,UnknownDevice);
                }
            }
        }
    }
    return TRUE;
}

extern "C" {

__declspec (dllexport) int RefreshIBMRaid()
{
    CRaidDeviceControl dev;

    return dev.Refresh() ? 0 : -1;
}

}