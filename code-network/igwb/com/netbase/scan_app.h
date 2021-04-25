#ifndef  __SCAN_APP_H__
#define  __SCAN_APP_H__

#include "../include/frame.h"
#include "../comm/comm_struct.h"

class CNetApp;
class CScanApp : public CNetApp
{
public:

    CScanApp(CNetWork* const pNetWork);
    virtual ~CScanApp();

    //初始化函数
    virtual int Init(void);

    //从主状态文件中读取所有协议栈的状态信息
    virtual int LoadStatus(void);

    //向主状态文件中写入所有协议栈的状态信息
    virtual int SaveStatus(void);

    //秒事件处理函数
    virtual void Timer1Sec(void);

protected:

    //搜索szPath目录中所有文件
    int ScanStack(const char* const szPath);

    //本接入点扫描的目录
    char m_szPathName[MAX_PATH];

    //扫描间隔，单位：秒
    int m_nScanInv;

    //扫描的秒计数器
    int m_nScanCounter;
};


#endif //__SCAN_APP_H__
