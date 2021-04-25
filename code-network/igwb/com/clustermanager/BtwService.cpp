#include "../include/frame.h"
#include "BtwService.h"

#define MAX_WAIT_COUNT 60   //停止或启动服务时，最大等待的次数。
//5停止或启动服务时，时间最长为 MAX_WAIT_COUNT * FIVE_SECOND。
#ifndef ONE_SECOND
#define ONE_SECOND (1*1000)
#endif

#ifndef FIVE_SECOND
#define FIVE_SECOND (5*1000)
#endif



BOOL StartService(LPCTSTR szService)
{
	MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "Starting Service: %s.\n", szService);
	return CtrlService(szService, CMD_START_SERVICE);
}

BOOL StopService(LPCTSTR szService)
{
	MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "Stopping Service: %s.\n", szService); 
	return CtrlService(szService, CMD_STOP_SERVICE);
}

BOOL QueryServiceStatus(LPCTSTR szService, LPSERVICE_STATUS lpServiceStatus)
{
	MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "Query Service %s status.\n", szService);
	return CtrlService(szService, CMD_QUERY_SERVICE, lpServiceStatus, sizeof(SERVICE_STATUS));
}

BOOL StartService(SC_HANDLE  schService)
{
    SERVICE_STATUS  ssServiceStatus; 
	BOOL Done;
	BOOL bSucc = FALSE;
	int count = 0;
	int nWaitHint;
	//查询服务的状态，决定启动服务的动作。
	if(QueryServiceStatus(schService, &ssServiceStatus))
	{
		if(ssServiceStatus.dwCurrentState == SERVICE_RUNNING)
		{
			//服务已启动时直接返回成功。
			MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "Started already.\n");
			return TRUE;
		}
	}
	else
	{
		ssServiceStatus.dwCurrentState = SERVICE_STOPPED;
	}

	if( (ssServiceStatus.dwCurrentState != SERVICE_START_PENDING) &&
		StartService( schService, 0, NULL ) == FALSE )
	{
		MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "Problem starting service.\nError %d.\n", GetLastError()); 
		if(GetLastError() == ERROR_SERVICE_ALREADY_RUNNING)
		{
			MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "ERROR_SERVICE_ALREADY_RUNNING.\n");
			bSucc = TRUE;
		}
		if(GetLastError() == ERROR_SERVICE_DEPENDENCY_FAIL)
		{
			MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "ERROR_SERVICE_DEPENDENCY_FAIL.\n");
		}
	}
	else
	{
		Done = FALSE;
		while(!Done && (count < MAX_WAIT_COUNT))
		{
			if ( QueryServiceStatus( schService, &ssServiceStatus ) == FALSE )
			{
				MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "Problem getting service status.\nError %d.\n", GetLastError() );
				Done = TRUE;
			}
			else
			{
				switch ( ssServiceStatus.dwCurrentState )
				{
				case SERVICE_START_PENDING:
					nWaitHint = max(ONE_SECOND, min(ssServiceStatus.dwWaitHint, FIVE_SECOND));
					MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "SERVICE_START_PENDING, count = %d, sleeping %d.\n", 
						count, nWaitHint);
					Sleep( nWaitHint );
					count++;
					break;
				case SERVICE_RUNNING:
					MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "SERVICE_RUNNING.\n");
					Done = TRUE;
					bSucc = TRUE;
					break;
				default:
					MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "Unexpected service state.\nState %d.\n", ssServiceStatus.dwCurrentState );
					Done = TRUE;
					break;
				}
			}
		}
	}
	MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "bSucc = %d.\n", bSucc);
	return bSucc;
}

BOOL StopDependentService(SC_HANDLE schService)
{
	DWORD BytesNeeded, ServicesReturned;
	BOOL bSucc = TRUE;
	LPENUM_SERVICE_STATUS lpServices = NULL;
	int i;
	if(!EnumDependentServices(schService, SERVICE_ACTIVE, NULL, 0, 
		&BytesNeeded, &ServicesReturned))
	{
		if(!BytesNeeded)
		{
			return TRUE;
		}
		if(GetLastError() == ERROR_MORE_DATA)
		{
			lpServices = (LPENUM_SERVICE_STATUS)new BYTE[BytesNeeded];
		}
		else
		{
			return FALSE;
		}
	}
	if(EnumDependentServices(schService, SERVICE_ACTIVE, lpServices, BytesNeeded, 
		&BytesNeeded, &ServicesReturned))
	{
		for(i = 0; i < ServicesReturned; i++)
		{
			//MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "Stop Service : %s.\n", lpServices[i].lpServiceName);
			bSucc = StopService(lpServices[i].lpServiceName);
			if(!bSucc)//停止任一服务失败则返回失败。
			{
				break;
			}
		}
	}
	delete lpServices;
	return bSucc;
}

BOOL StopService(SC_HANDLE  schService)
{
	SERVICE_STATUS  ssServiceStatus; 
	BOOL bSucc = FALSE;
	BOOL Done;
	DWORD nErr;
	int count = 0;
	int count1 = 0;
	Done = FALSE;
	int nWaitHint;
	while(!Done && count1 < MAX_WAIT_COUNT)
	{
		if(QueryServiceStatus(schService, &ssServiceStatus))
		{
			if(ssServiceStatus.dwCurrentState == SERVICE_STOPPED)
			{
				//服务已启动时直接返回成功。
				MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "Stopped already.\n");
				return TRUE;
			}
			if(ssServiceStatus.dwCurrentState == SERVICE_START_PENDING)
			{
				nWaitHint = max(ONE_SECOND, min(ssServiceStatus.dwWaitHint, FIVE_SECOND));
				MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "SERVICE_STOP_PENDING. sleep %d.\n", nWaitHint);
				Sleep(nWaitHint);
				count1++;
				continue;
			}
		}
		else
		{
			ssServiceStatus.dwCurrentState = SERVICE_RUNNING;
			ASSERT(0);
		}

		if((ssServiceStatus.dwCurrentState != SERVICE_STOP_PENDING)&&
			ControlService( schService, SERVICE_CONTROL_STOP, &ssServiceStatus ) == FALSE)
		{
			MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "Problem stopping service.\nError %d.\n", GetLastError() ); 
			nErr = GetLastError();
			switch(nErr)
			{
			case ERROR_DEPENDENT_SERVICES_RUNNING:
				MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "StopDependentService.\n");
				if(!StopDependentService(schService)) //停止本服务所依赖的服务
				{
					Done = TRUE;
				}
				break;
			default:
				Done = TRUE;
				break;
			}
		}
		else
		{
			count = 0;
			Done = FALSE;
            Sleep(500);
			while(!Done && (count < MAX_WAIT_COUNT))
			{
				if ( QueryServiceStatus( schService, &ssServiceStatus ) == FALSE )
				{
					MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "Problem getting service status.\nError %d.\n", GetLastError() );
					Done = TRUE;
				}
				else
				{
					switch ( ssServiceStatus.dwCurrentState )
					{
					case SERVICE_STOP_PENDING:
						nWaitHint = max(ONE_SECOND, min(ssServiceStatus.dwWaitHint, FIVE_SECOND));
						MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "SERVICE_STOP_PENDING, count = %d, sleeping %d.\n", 
							count, nWaitHint);
						Sleep( nWaitHint );
						count++;
						break;
					case SERVICE_STOPPED:
						MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "SERVICE_STOPPED.\n");
						Done = TRUE;
						bSucc = TRUE;
						break;
					default:
						MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "Unexpected service state.\nState %d.\n", ssServiceStatus.dwCurrentState );
						Done = TRUE;
						break;
					}
				}
			}
			Done = TRUE;
		}
	}
	
	MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "bSucc = %d.\n", bSucc);
	return bSucc;
}

BOOL CtrlService(LPCTSTR szService, int nCtrlCode, void* buf, int len)
{
    BOOL  bSucc;
    SC_HANDLE       schSCManager; 
    SC_HANDLE       schService; 
    SERVICE_STATUS  ssServiceStatus; 
    
	bSucc = FALSE;
    schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS ); 	
    if ( schSCManager == NULL )
	{
		MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "Problem accessing Service Control Manager.\nError %d.\n", GetLastError() ); 
    } 
	else
	{
		schService = OpenService( schSCManager, szService, SERVICE_ALL_ACCESS ); 
		if ( schService == NULL )
		{
			MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, "Problem opening %s service.\nError %d.\n", szService, GetLastError() );
		}
		else
		{
			switch(nCtrlCode)
			{
			case CMD_START_SERVICE: //启动服务。
				bSucc = StartService(schService);
				break;
			case CMD_STOP_SERVICE: //停止服务。
				bSucc = StopService(schService);
				break;
			case CMD_QUERY_SERVICE: //查询服务状态。
				bSucc = QueryServiceStatus(schService, &ssServiceStatus);
				ASSERT(NULL != buf);
				ASSERT(sizeof(SERVICE_STATUS) == len);
				if(bSucc && (sizeof(SERVICE_STATUS) == len) && buf)
				{
					memcpy((BYTE*)buf, (BYTE*)&ssServiceStatus, len);
				}
				else
				{
					bSucc = FALSE;
				}
				break;
			case CMD_CHECK_SERVICE: //服务存在。
				bSucc = TRUE;
				break;
			}
			CloseServiceHandle(schService);
		}
		CloseServiceHandle(schSCManager);
	}
	return bSucc;
}
