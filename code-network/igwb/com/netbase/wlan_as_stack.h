#ifndef __WLAN_AS_STACK_H__
#define __WLAN_AS_STACK_H__


#include "i_wlan_as_oci.h"
#include "wlan_as_encap.h"

class CProtocolStack;
class CWlanAsStack : public CProtocolStack
{
public:
    CWlanAsStack(CNetApp* const pNetApp, 
                 UINT4 const uStackId);
    virtual ~CWlanAsStack(void);

    //返回本对象中当前已接收的且可以提交给前存盘模块
    //保存的话单数据的字节数
    virtual int GetReadyDataSize(void);

    //返回本对象中当前已接收的且可以提交给前存盘模块
    //保存的话单数据的记录数
    virtual int GetReadyRecordCount(void);

    //将本协议栈可提交给前存盘模块保存的话单数据放入*pDataBuf对象中
    virtual void GetReadyData(CDataBuffer* const pDataBuf);

    //对上次提供给前存盘模块保存的话单数据进行后续的处理
    virtual void OnResponse(BOOL const bSaved);

    //从主状态文件中读取本协议栈的状态信息
    virtual int LoadStatus(void);

    //向主状态文件中写入本协议栈的状态信息
    virtual int SaveStatus(void);

    //秒事件处理函数
    virtual void Timer1Sec(void);

protected:

    //向对应的主机模块或GSN发送协议帧
    virtual int SendFrame2Ex(void* pFrame);

    IWlanAsOci*    m_pWlanAsOci;
    
    CWlanUtilEncap m_WlanUtilEncap;
};

#endif //__WLAN_AS_STACK_H__
