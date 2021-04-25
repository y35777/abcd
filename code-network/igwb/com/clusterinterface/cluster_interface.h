/***************************************************************************
 *产品名        ：iGateway Bill V200
 *模块名        ：双机接口模块(ClusterInterface)
 *文件名        ：cluster_interface.h
 *描述信息      ：该类封装了双机接口模块管理双机事务的接口。
 *版本说明      ：V200R001i00M00B00D1130A
 *版权信息      ：版权所有（C）2001-2002 华为技术有限公司
 *作者          ：邱意平
 *创建时间      ：2001-10-30
 *修改记录      ：
****************************************************************************/

#ifndef _CLUSTER_INTERFACE_H_
#define _CLUSTER_INTERFACE_H_

#include "cluster_hdr.h"

class CClusterInterface : public CWorkModule
{
public:
    CClusterInterface();                 //构造函数
    virtual ~CClusterInterface();        //析构函数
    virtual int InitInstance();          //初始化函数(继承父类)
    virtual void ExitInstance();         //退出清理函数(继承父类)

protected:
    void ParseMsg(MSG_INTRA* pMsg);      //消息处理函数(继承父类)
    virtual MODULE_TYPE GetModuleType(); //获得本模块的类型(继承父类)
    void OnTimer(const long nTimerID);   //定时器消息(继承父类) 

    void SendActiveAnsToCluster(BOOL bResult);    //发送激活应答
    void ProcInitAns(BOOL bResult);               //初始化应答
    void ProcActiveAns(BOOL bResult);             //处理激活应答
    void Active(void);                            //激活操作
    void DeActive(void);                          //去激活操作
    void Shutdown(void);                          //关闭操作

    void ProcAskedFailOver(const PROCESS_TYPE pt,
                           const MODULE_TYPE  mt,
                           const BYTE nCause, 
                           const void* pData = 0,
                           const UINT4 nLen = 0); //处理倒换申请

    void ProcManualOverCmd(void);                 //处理手工倒换申请
    void ProcDiskSpaceFull(void);                 //处理磁盘空间满 
    void ProcDiskSpaceRecovery(void);             //处理磁盘空间恢复
    
    void Timer5Sec(void);                         //5秒钟定时事务处理
    
    
//成员变量
private:
    ECLSTRINTSTATUS   m_nStatus;         //双机接口模块当前状态
    SFailOverCause*   m_pFailOverCause;  //当前正在进行倒换的原因
    long              m_lTimerID5Sec;    //5秒钟定时器ID
    BOOL              m_bNetworkPaused;  //网络模块已停止接收话单的标志
    BYTE              m_yDiskFullMask;   //磁盘空间满标志 
    UINT2             m_nAPCount;        //接入点个数
    
    BOOL              m_bSingleServer;  //added by chenliangwei, 2003-04-23

};


#endif //_CLUSTER_INTERFACE_H_
