#ifndef __BACKSTORAGE_H__
#define __BACKSTORAGE_H__

#include "../include/precom.h"
#include "../include/frame.h"
#include "patch_encap.h"

class CSaveChannel;
class CPerfItem;

//设置以下的数据结构来描述一个通道的信息
struct SChannelInfo
{
    CSaveChannel*   pSaveChannel;   //相应的通道对象
    UINT4           uLastPID;       //本通道的最后一个话单包号
};

class CBackStorage : public CWorkModule  
{
public:
    //构造、析构函数
    CBackStorage(const UINT4 uAPID);
    virtual ~CBackStorage();

protected:
    //继承的接口函数
    int InitInstance();        //初始化函数
    void ExitInstance();        //模块退出时的清理函数
    void OnTimer(const long nTimerID);    //定时器事件处理函数
    MODULE_TYPE GetModuleType();       //获取模块类型
    void ParseMsg(MSG_INTRA* pMsgParsed); //实现消息分发及相关处理

    //内部函数
    void ProcSavePacketMsg(MSG_INTRA* pMessage); //处理"请求处理最终话单包"消息
    void ProcResumeMsg(MSG_INTRA* pMessage);    //处理"请求启动恢复"消息
    void ProcUpdateOverMsg(MSG_INTRA* pMessage);//处理"通知辅助升级完成"消息
    void ProduceCDR(MSG_INTRA* pMessage);   

protected:
    //存盘通道对象相关信息的数组，下标为通道号
    SChannelInfo* m_ChannelInfoArray;   
    UINT4         m_uChannelCount;      //通道数
    long          m_lSecondTimerID;     //秒事件时钟
    BOOL          m_bError;             //是否存在错误的标志
    UINT4         m_uAPID;              //接入点号
    UINT4         m_uApType;            //接入点类型
    CPatchEncap   m_PatchEncap;         //后存盘适配器

    //by ldf 2004-02-24
    //是否要阿联酋分拣方式
    char          m_szDispatchType[49];  //配置是否启动UAE类型的分拣方式
	UINT4         m_uDispatchAllCDRCH;   //配置存放 所有话单类型 的通道
    //end
	
};

#endif
