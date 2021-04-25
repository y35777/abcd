#ifndef _BTWSERVICE_H
#define _BTWSERVICE_H
#include "winsvc.h"

//以下宏定义了CtrlService的nCtrlCode的命令字。
#define CMD_START_SERVICE 0 //启动服务命令字。
#define CMD_STOP_SERVICE 1  //停止服务命令字。
#define CMD_QUERY_SERVICE 2 //查询服务状态命令字。
#define CMD_CHECK_SERVICE 3 //检查服务是否在系统中存在。
//对szService字符串指定的服务进行操作，命令字如上的宏定义。
BOOL CtrlService(LPCTSTR szService, int nCtrlCode, void* buf = NULL, int len = 0);

//启动szService字符串指定的服务
BOOL StartService(LPCTSTR szService);
//启动schService指定的服务，schService为OpenService()函数返回的句柄
BOOL StartService(SC_HANDLE  schService);

//停止szService字符串指定的服务,它会将它依赖的服务停止。
BOOL StopService(LPCTSTR szService);
//停止schService指定的服务，schService为OpenService()函数返回的句柄
BOOL StopService(SC_HANDLE  schService);
//停止schService依赖的服务。
BOOL StopDependentService(SC_HANDLE schService);

//查询szService字符串指定的服务的当前状态。
BOOL QueryServiceStatus(LPCTSTR szService, LPSERVICE_STATUS lpServiceStatus);

#endif