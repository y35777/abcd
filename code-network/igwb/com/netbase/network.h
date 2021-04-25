#ifndef  __NETWORK_H__
#define  __NETWORK_H__

#include "../include/frame.h"
#include "protocol_track.h"

class CNetApp;
class CNetWorkStatusInfo;

class CNetWork:public CWorkModule
{
public:
    enum NWSTATUS
    {
        NWSTATUS_INIT,          //初始化状态
        NWSTATUS_ACTIVE,        //激活状态
        NWSTATUS_DEACTIVE,      //去激活状态
        NWSTATUS_AUXUPGRADE     //辅助升级状态
    };  

    CNetWork();
    virtual ~CNetWork();

    //获取当前包号
    UINT4 GetPacketId();

    //网络模块是否正在等待前存盘的应答
    BOOL IsWaitForResponse();

    //工作目录,如"D:\\statusfile\\network\\"
    static char m_szWorkDir[MAX_PATH];

    //发送消息到主机
    int SendMsgtoHost(MSG_INTRA* pMsg);

    void TrackProtocol(MSG_INTRA* pMsg, int nProcRst = 0);

    //获取网络模块当前状态
    inline UINT4 GetCurStatus()
    {
        return m_uStatus;
    };
 
protected:

    //初始化实例函数
    virtual int InitInstance(void);

    //退出时的清理工作
    virtual void ExitInstance(void);

    //返回模块号信息
    virtual MODULE_TYPE GetModuleType();

    //消息分发函数
    virtual void ParseMsg(MSG_INTRA* pMsgParsed);

    //定时器函数
    virtual void OnTimer(const long nTimerID);

    //1秒定时器函数
    void Timer1Sec();

    //定时打包事件处理
    void LowAtTimer();

    //进行启动恢复处理
    int AdjustStatus(UINT4 uPacketSaved);

    //发送话单数据包
    BOOL SendBillPackage(BOOL bGetNewData = TRUE);

    //向前存盘发送空包
    void SendEmptyPackage();

    //对上次提供给前存盘模块保存的话单数据进行后续的处理
    void OnResponse(BOOL const bSaved);

    //发送状态数据
    void SyncStatus();

    //保存状态
    int SaveStatus();

    //读取状态
    int LoadStatus();

    //在初始化完成后，第一次读取状态
    int InitStatus();

    //CNetWork与协议实现有关的部分都委托CNetApp子类对象完成
    CNetApp* m_pNetApp;

    //协议跟踪对象的指针
    CProtocolTrack* m_pPtclTracker;

    //网络模块的状态
    UINT4 m_uStatus;

    //主状态文件名
    char m_szStatusFileName[MAX_PATH];

    //是否要进行合并的标志，如果要进行合并则在无话单时需要发空包
    BOOL m_bMustMerge;

    //秒事件时钟ID
    long m_Timer1SecId;

    //低潮定时器时间
    int m_nLowAtTime;

    //定时打包时钟ID
    long m_TimerSendPacketId;

    //主状态文件读写对象
    CNetWorkStatusInfo* m_pNetWorkStatusInfo;

    //已发送的最后一包的包ID
    UINT4 m_uPacketId;

    //是否正在等待前存盘应答
    BOOL m_bWaitForResponse;

    //是否需要在收到前存盘应答后立即发送话单
    BOOL m_bSendOnResponse;

    //最大话单包长度
    UINT4 m_uMaxPackageSize;

    //已发空包数
    UINT4 m_uSendEmptyPackageCount;

    //每秒钟需要发送的最大空包数
    UINT4 m_uSendEmptyPackageMaxPerSec;

    //低潮包大小
    UINT4 m_uLowAtSize;

    //附加的协议类型
    char m_szStackType[40];

};
#endif //__NETWORK_H__
