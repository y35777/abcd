#include "../include/frame.h"
#include "IpHlpApi.h"
#include "BtwIpHelper.h"


BOOL ChangeIpA(DWORD NewIpAddr, DWORD NewIpMask, DWORD NewIpIndex)
{
	char OutBuf[1024];

	struct TIpAddress
	{
		DWORD IpIndex; //用ipconfig看到的第几个IP地址
		DWORD Ip;
		DWORD Mask;
	}InBuf;

	DWORD ReturnedBytes;
	HANDLE handle;

	if (!DefineDosDevice(1,"Ip","\\Device\\Ip"))
	{
		TRACE(MTS_CLSTR, "define dos device fail 1");
		return FALSE;
	};
	
	handle = CreateFile(
		"\\\\.\\Ip",
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,OPEN_EXISTING,
		0,
		0);
	
	if (handle == INVALID_HANDLE_VALUE)
	{
		TRACE(MTS_CLSTR, "device open failed!!! 2 ");
		return FALSE;
	}
	
	//先将IP清零
	InBuf.IpIndex = NewIpIndex;
	InBuf.Ip = 0x0; 
	InBuf.Mask = 0xffff;
	if (!DeviceIoControl(handle, 0x00128004, &InBuf, sizeof(InBuf),
		OutBuf, sizeof(OutBuf), &ReturnedBytes, 0))
	{
		TRACE(MTS_CLSTR, "deviceiocontrol fail1 3 ");
		return FALSE;
	};
	CloseHandle(handle);
	if (!DefineDosDevice(7,"Ip","\\Device\\Ip"))
	{
		TRACE(MTS_CLSTR, "define dos device fail 4 ");
		return FALSE;
	};
	
	if (!DefineDosDevice(1,"Ip","\\Device\\Ip"))
	{
		TRACE(MTS_CLSTR, "define dos device fail 5 ");
		return FALSE;
	};
	
	handle = CreateFile("\\\\.\\Ip",
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0, 
		NULL);
	if (handle == INVALID_HANDLE_VALUE)
	{
		TRACE(MTS_CLSTR, "device open failed!!! 6 ");
		return FALSE;
	}
	
	//再设所需IP地址
	InBuf.IpIndex = NewIpIndex;
	InBuf.Ip = NewIpAddr;
	InBuf.Mask = NewIpMask;
	if (!DeviceIoControl(handle, 0x00128004, &InBuf, sizeof(InBuf),
		OutBuf, sizeof(OutBuf), &ReturnedBytes, 0))
	{
		TRACE(MTS_CLSTR, "deviceiocontrol fail 7 ");
		return FALSE;
	}
	CloseHandle(handle);
	if (!DefineDosDevice(7,"Ip","\\Device\\Ip"))
	{
		TRACE(MTS_CLSTR, "define dos device fail 8 ");
		return FALSE;
	};

	return TRUE;
}

//获得IpAddr在IP地址列表中的Index。
//INVALID_IP_INDEX 表示无该IP。
DWORD GetIpIndex(DWORD IpAddr)
{
	MIB_IPADDRTABLE* pIpAddrTable;
	DWORD dwSize = 0;
	DWORD i ;
	DWORD IpIndex = INVALID_IP_INDEX;

	if(GetIpAddrTable(NULL, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER)
	{	
		pIpAddrTable = (MIB_IPADDRTABLE*)new BYTE[dwSize];
		if(GetIpAddrTable(pIpAddrTable, &dwSize, FALSE) == NO_ERROR)
		{
			for(i = 0; i < pIpAddrTable->dwNumEntries; i++)
			{
				//TRACE("%s.\n", inet_ntoa(*(struct in_addr*)&pIpAddrTable->table[i].dwAddr));  ZhengYuqun 2003-09-19 SWPD01685
				if(pIpAddrTable->table[i].dwAddr == IpAddr)
				{
					IpIndex = pIpAddrTable->dwNumEntries - (i + 1);
					break;
				}
			}
		}
		delete pIpAddrTable;
	}
	return IpIndex;
}

BOOL ChangeIp(DWORD NewIpAddr, DWORD NewIpMask, DWORD OldIpAddr)
{
	DWORD IpIndex ;

	IpIndex = GetIpIndex(OldIpAddr);
	if(IpIndex != INVALID_IP_INDEX)
	{
		return ChangeIpA(NewIpAddr, NewIpMask, IpIndex);
	}
	return FALSE;
}
