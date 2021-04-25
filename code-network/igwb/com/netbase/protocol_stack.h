#ifndef  __PROTOCOL_STACK_H__
#define  __PROTOCOL_STACK_H__

#include "../include/frame.h"

//MSC、128、STP等主机的主机模块与计费网关之间采用滑动窗口协议
//进行话单的传送，GSN与计费网关之间采用GTP'协议进行话单的传送.
//CProtocolStack抽象了主机模块或GSN与计费网关之间的话单传送协议。
//一个CProcotolStack对象与一个主机模块或GSN相对应。
//针对具体的话单传送协议，需要从CProtocolStack派生具体的子类
class CNetApp;
class CDataBuffer;
class CStackStatusInfo;
class CProtocolStack
{
public:
    CProtocolStack(CNetApp* const pNetApp, UINT4 const uStackId);
    virtual ~CProtocolStack(void);

    //获得本协议栈的编号
    UINT4 GetStackId(void);

    //返回本对象中当前已接收的且可以提交给前存盘模块保存的话单数据的字节数
    virtual int GetReadyDataSize(void);

    //返回本对象中当前已接收的且可以提交给前存盘模块保存的话单数据的记录数
    virtual int GetReadyRecordCount(void);

    //将本协议栈可提交给前存盘模块保存的话单数据放入*pDataBuf对象中
    virtual void GetReadyData(CDataBuffer* const pDataBuf);

    //处理从与本协议栈对应的主机模块或GSN发送过来的一帧数据 
    virtual void OnReceive(void* pFrame);

    //对上次提供给前存盘模块保存的话单数据进行后续的处理
    virtual void OnResponse(BOOL const bSaved);

    //从主状态文件中读取本协议栈的状态信息
    virtual int LoadStatus(void);

    //向主状态文件中写入本协议栈的状态信息
    virtual int SaveStatus(void);

    //秒事件处理函数
    virtual void Timer1Sec(void);

    //获得从最近一次接收到数据开始到现在所经历的时间，单位为秒
    UINT4 GetInertiaPeriod();

    inline time_t GetLastRecvTime(void)
    {
        return m_lLastRecvTime;
    }

protected:

    //向对应的主机模块或GSN发送协议帧
    virtual int SendFrame2Ex(void* pFrame) = 0;

    //该成员变量表示协议栈的编号
    //该成员变量的取值或者是与本对象对应的主机模块的模块号，
    //或者是与 本对象对应的GSN的IP地址
    UINT4 m_uStackId;

    //从最近一次接收到数据开始到现在所经历的时间，单位为秒；
    UINT4 m_uInertiaPeriod;

    //该指针指向本协议栈的状态信息对象
    CStackStatusInfo* m_pStatusInfo;

    //该指针指向作为网络模块应用层的对象
    CNetApp* m_pNetApp;

    //协议栈标识
    char     m_szName[20];

    //上次收到协议帧的时间
    time_t   m_lLastRecvTime;
};


#endif //__PROTOCOL_STACK_H__
