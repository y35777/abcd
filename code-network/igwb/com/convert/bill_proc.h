#ifndef __BILLPROCESS_H__
#define __BILLPROCESS_H__

#include "../include/precom.h"
#include "convert_comm.h"
#include "../include/format_out.h"
#include "proc_chanl.h"
#include "../include/merge.h"

class CPerfItem;
class CBillProcess  
{
public:
    //构造、析构函数
    CBillProcess();
    virtual ~CBillProcess();

    //参数设置和获取函数
    INT4 GetChannelCount();

    //初始化函数
    virtual BOOL Init(UINT4 uAPID, BOOL bNeedConvert, BOOL bNeedMerge); 
	//进行与话单合并相关的初始化
	virtual BOOL InitMerge(int nHashSize, int nMValue, int nMaxQwLen);
    void SetUpdateFlag(BOOL bIsAssistUpdate);            //设置辅助升级标志
    BOOL IsAssistUpdateOver();                           //判断辅助升级是否完成
    void ProcessSynInfo(const BYTE* pInfo, UINT4 uLen);  //处理同步信息
    //根据后存盘模块的最后保存包号计算出前存盘的恢复起始包号
    UINT4 GetResumePID(UINT4 uLastPID);
    //将一张话单分拣到所属的通道并进行格式转换等话单处理
    virtual void Distribute(SBill *pABill);
	virtual void SpecDistribute(SBill *pABill);
    //处理一包话单
    void ProcessBillPacket(UINT4 uPID, BYTE* pPacket, UINT4 uPacketLen, 
                           UINT4& uFrontBillCount, UINT4& uBackBillCount, 
                           UINT4& uMergeTime, UINT4& uProcessTime);
    //处理一包不需要合并的话单
    virtual BOOL ProcessABill(SBill& OrgBill, SBill& PreBill);

    //得到某一指定通道下的最终话单包
    void GetChannelPacket(UINT4 uChannelID, BYTE*& pPacket, UINT4& uPacketLen);
    //得到指定通道下的同步信息
    SChannelSynInfo* GetChannelSynInfo(UINT4 uChannelID);

	int GetErrorCDRCount(void);		//获得异常话单数
	void ResetErrorCDRCount();		//复位异常话单数
	
protected:
    //在话单长度固定的情况下，从话单包中拆分出一张话单，
    //在128、GSM、STP等产品中有效。被PreProcess函数调用
    BOOL GetFixedLenBill(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill);
    //在话单包中的话单存放方式为长度＋话单内容的情况下，
    //从话单包中拆分出一张话单，在GPRS、3G等产品中有效。被PreProcess函数调用
    BOOL GetVarLenBill(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill);
    //话单预处理，将话单包拆分为单张话单
    virtual BOOL PreProcess(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill) = 0;

protected:
    IBillProcessor* m_pIBillProcessor;  //话单处理接口
    //分拣后的通道对象，通道的下标表示通道号，数组的元素为CProcChannel类的指针
    CProcChannel** m_ChannelArray;      
    //当前处理的话单包是否为空包，在判断辅助升级是否完成时使用
    BOOL     m_bEmptyPacket;
    BOOL     m_bIsAssistUpdate; //辅助升级标志
    CIGWB*   m_pIGWB;           //格式库接口对象
    INT4     m_nChannelCount;   //通道数目（包括通道0）
    UINT4    m_uOrigLen;        //原始话单长度，在原始话单长度定长时有效
    CMerger* m_pMerge;          //话单合并对象，当话单需要合并时有效
	CBillParser* m_pParser;     //话单解析对象，当话单需要合并时有效
    BOOL     m_bBillRemain;     //是否存在等待合并话单标志
    UINT4    m_uAPID;           //当前的接入点ID号
    BOOL     m_bNeedConvert;    //是否需要格式转换
    BOOL     m_bNeedMerge;      //是否需要合并
    BOOL     m_bSupportDST;     //Z30971增加 是否支持夏时制 
    int      m_nErrorCDRCount;	//异常话单数
    UINT4    m_uMaxDuration;    //合并时话单中通话时长的上限 SWPD02238 ZhengYuqun 2003-11-04
    BOOL     m_bProcBillCsn;    //是否要对话单序列号进行特殊处理
	CPerfItem* m_pMergeQueue;
	int      m_nReDistribute;  //石家庄特殊分拣
};

#endif 
