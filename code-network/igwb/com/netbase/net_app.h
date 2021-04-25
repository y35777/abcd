#ifndef  __NET_APP_H__
#define  __NET_APP_H__

#include "../include/frame.h"
#include "../comm/comm_struct.h"
#include "../include/perf_item.h"

//CNetApp抽象了网络模块的应用层。支持不同协议的应用层，
//需要从CNetApp派生具体的子类
class CNetWork;
class CProtocolStack;
class CDataBuffer;
class CNetApp
{
public:

    CNetApp(CNetWork* const pNetWork);
    virtual ~CNetApp();

    //初始化函数
    virtual int Init(void);

    //当前存盘模块通知网络模块启动恢复已经完成后，本函数被调用
    virtual void Active(void);

    //当网络模块在接收辅助升级或去激活命令时，本函数被调用
    virtual void DeActive(void);

    //从主状态文件中读取所有协议栈的状态信息
    virtual int LoadStatus(void);

    //向主状态文件中写入所有协议栈的状态信息
    virtual int SaveStatus(void);

    //处理来自网络层的协议帧
    virtual void OnReceive(SNetFrame* pFrame);

    //秒事件处理函数
    virtual void Timer1Sec(void);

    //返回所有协议栈中当前已接收的且可以提交给前存盘模块保存
    //的话单数据的字节数
    virtual int GetReadyDataSize(void);

    //返回所有协议栈中当前已接收的且可以提交给前存盘模块保存
    //的话单数据的记录数
    virtual int GetReadyRecordCount(void);

    //将所有协议栈可提交给前存盘模块保存的话单数据放入*pDataBuf对象中
    virtual void GetReadyData(CDataBuffer* const pDataBuf);

    //对上次提供给前存盘模块保存的话单数据进行后续的处理
    virtual void OnResponse(BOOL const bSaved);

    //获取CNetWork*对象的指针
    CNetWork* GetNetWork();

    //向对应的主机模块或GSN发送协议帧
    virtual int SendFrame2Ex(MSG_INTRA* pMsg, CProtocolStack* pStack);

	//颜文远2002.04.16添加
	//转发来自告警模块的告警帧
	virtual void DispatchAlarm(MSG_INTRA* pMsg);
	//颜文远2002.04.16添加结束
 
    //张玉新 2003-05-16
    CPerfItem *m_pReceiveBillCnt;   //平均接收话单张数
    
protected:

    //该MAP的键值为协议栈的编号；对于滑动窗口协议栈，
    //编号为对应的主机模块的编号；对于GTP'协议栈，编
    //号为对应的GSN的IP地址,该MAP的元素为协议栈对象的指针
    typedef MAP<unsigned long, CProtocolStack*> MAP_ID_TO_PS;
    MAP_ID_TO_PS m_StackMan;

    //若该成员变量取值为STACK_SLIDE，则本应用层的协议栈的
    //状态信息为_SlideWndModInfo结构
    //若该成员变量取值为STACK_GTP，则本应用层的协议栈的状
    //态信息为_GtpNodeInfo结构
    int m_nStackStatusInfoType;

    //该指针指向作为整个网络模块的CNetWork对象
    CNetWork* m_pNetWork;

    //该成员变量表示应用层是否被激活
    BOOL m_bActive;

    //存放在用协议栈(以内存地址为索引)上次收到协议帧的时间差
    MAP<UINT4, UINT4>  m_Stack_To_RecvInv;
};


#endif //__NET_APP_H__
