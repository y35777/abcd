#ifndef __CLUSTER_TOOLBOX_H__
#define __CLUSTER_TOOLBOX_H__

int ExecuteProcess(const char* const szExeName,
                   UINT4* pExitCode);
int ExecuteProcess(const char* const szExeName);

// Reboot -- ��������NT
BOOL Reboot(UINT uFlags = 0);

//����ϵͳ�汾
extern int g_nOSVersion;
#define INVALID_OS_VERSION  -1

#endif //__CLUSTER_TOOLBOX_H__