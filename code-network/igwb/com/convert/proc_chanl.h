#ifndef __PROCCHANNEL_H__
#define __PROCCHANNEL_H__

#include "../include/precom.h"
#include "convert_comm.h"

struct SBill;
class CFormat;

class CBillBuffer
{
public:
    CBillBuffer();
    virtual ~CBillBuffer();

    UINT4 GetBillDataLength();     //获取缓冲区中的话单数据总长度
    void AddABill(SBill* pABill);  //向缓冲区中添加一张话单
    //从缓冲区中取出第一张话单，并将该话单从链表中删除
    SBill* GetABill();
protected:
    UINT4 m_uBillDataLen;       //缓冲区中的话单数据总长度
    LIST<SBill*> m_BillList;    //缓冲池中保存的所有话单的链表
};

class CProcChannel
{
public:
    //构造、析构函数
    CProcChannel();
    virtual ~CProcChannel();

    //参数设置和获取函数
    void SetLastPID(UINT4 uLastPID);

    //根据通道的配置参数，进行初始化
    virtual BOOL Init(UINT4 uAPID, INT4 nChannelID, BOOL bNeedConvert);
    //转换一张话单
    virtual BOOL ConvertABill(SBill* pABill, SBill* pConvertedBill, 
                              CFormat* pDestFmt);
    //处理一张话单
    void ProcessABill(SBill* pABill, CFormat* pDestFmt);
    void GetBillPacket(BYTE* &pPacket, UINT4 &uPacketLen);  //将话单数据打包
    //处理从后存盘模块发送过来的同步信息
    void SynChannelInfo(const SChannelSynInfo* pInfo);
    SChannelSynInfo* GetSynInfo();  //获取同步信息
    virtual BOOL AddABill(SBill& ABill);

protected:
    //纠正转换后的话单校验和
    BOOL CorrectChkSum(BYTE* pABill, UINT4 uLen, 
                       INT4 nCheckSumPos, 
                       INT4 nCheckPosLow, 
                       INT4 nCheckPosHigh);

protected:
    CBillBuffer     m_BillBuffer;       //话单缓冲区对象
    SChannelSynInfo m_ChannelSynInfo;   //同步状态信息
    BOOL            m_bNeedConvert;     //是否进行格式转换标志
    UINT4           m_uAPID;            //当前接入点的ID号
    INT4            m_nChannelID;       //当前的通道号

    BOOL            m_bCorrectChkSum;   //是否要纠正转换后的话单校验和
    INT4            m_nCheckSumPos;     //校验和在最终话单中的位置
    INT4            m_nCheckPosLow;     //要校验的低字节位置  
    INT4            m_nCheckPosHigh;    //要校验的高字节位置
};

#endif
