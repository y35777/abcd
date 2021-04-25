#ifndef __FRONTSTORAGE_H__
#define __FRONTSTORAGE_H__

#include "../include/precom.h"
#include "front_save.h"

class CPacketCache;

class CFrontStorage : public CWorkModule  
{
public:
    //构造、析构函数
    virtual ~CFrontStorage();
    CFrontStorage(UINT4 uAPID, CPacketCache* pPacketCache);

protected:
    //继承的接口函数
    int InitInstance();     //初始化函数
    void ExitInstance();    //模块退出时的清理函数
    void OnTimer(const long nTimerID);  //定时器事件处理函数
    //获取模块类型
    MODULE_TYPE GetModuleType();
    void ParseMsg(MSG_INTRA* pMsgParsed); //实现消息分发及相关处理
    
    //内部函数
    //处理"请求保存话单包"命令帧
    void ProcSavePacketMsg(MSG_INTRA* pMessage);
    //处理"请求启动恢复"命令帧
    void ProcResumeMsg(MSG_INTRA* pMessage);
    //处理"通知状态消息"命令帧
    void ProcInformStatusMsg(MSG_INTRA* pMsgParsed);
    void ProcessPacket(BYTE* pPacketInfo);  //话单包存盘处理
protected:
    CFrontSave      m_Save;         //进行具体的存盘事务处理的对象
    CPacketCache*   m_pPacketCache; //话单包缓冲区指针
    BOOL            m_bError;       //表示是否存在错误
    long            m_lSecondTimerID;   //秒事件时钟
    UINT4           m_uLastPID;         //最后一包的包号
    BYTE*           m_pPacket;          //最后一包的数据指针
    UINT4           m_uPacketLen;       //最后一包的数据长度
    UINT4           m_uAPID;            //接入点号
};

#endif 
