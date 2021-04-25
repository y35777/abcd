#ifndef  __WLAN_AS_APP_H__
#define  __WLAN_AS_APP_H__

#include "../include/frame.h"

class CNetApp;
class CWlanAsApp : public CNetApp
{
public:

    CWlanAsApp(CNetWork* const pNetWork);
    virtual ~CWlanAsApp();

    //初始化函数
    virtual int Init(void);

    //从主状态文件中读取所有协议栈的状态信息
    virtual int LoadStatus(void);

    //向主状态文件中写入所有协议栈的状态信息
    virtual int SaveStatus(void);

    //秒事件处理函数
    virtual void Timer1Sec(void);

protected:

};


#endif //__WLAN_AS_APP_H__
