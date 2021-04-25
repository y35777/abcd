//本文件由颜文远2002.04.09生成

#ifndef _SLIDE_WND_RELATED_H_
#define _SLIDE_WND_RELATED_H_

#include "../include/base_type.h"
#include "../include/comm_struct_out.h"
#include "data_buffer.h"
#include "protocol_stack.h"
#include "protocol_track.h"
#include "net_app.h"
#include "net_status.h"
#include "resource.h"
#include "protocol_stack.h"
#include "../include/perf_item.h"


//实现滑动窗口
class CSlideWnd 
{
public:
    //构造函数/析构函数
    CSlideWnd();
    ~CSlideWnd();

    //初始化滑动窗口
    void Init(int nSize, int nBaseSeqNumber, int nBillMsgSize, 
			  int nBillRecordSize, int nMpuWindowSize);

    //加入一条记录
    int AddRecord(SBillMessage *pMsg);
    //滑动滑动窗口
    void Slide(int nCount);
    //同步滑动窗口
    void Synchronize(int nBaseSeqNumber);

    //获取滑动窗口头帧号
    int GetBaseSeqNumber() 
	{ 
		return m_nBaseSeqNumber; 
	}
    //获取滑动窗口中第一个所缺的序列号
    int GetExpectedSeqNumber() 
	{ 
		return m_nExpectedSeqNumber; 
	}
    //获取滑动窗口中可以提交的记录数
    int GetReadyRecordCount() 
    { 
		return m_nExpectedSeqNumber - m_nBaseSeqNumber; 
	}
    //获取滑动窗口中总的记录数
    int GetRecordCount() 
	{ 
		return m_nRecordCount; 
	}
    //获取可提交数据
    int GetReadyData(CDataBuffer* pDataBuffer);

private:
	int m_nMpuWindowSize;		//主机的滑动窗口大小
    int m_nSize;                //滑动窗口大小
    BYTE* m_pData;				//滑动窗口数据
	int m_nBillRecordSize;		//话单记录大小
	int m_nBillMsgSize;			//话单消息大小
    int m_nBaseSeqNumber;       //滑动窗口头序列号
    int m_nExpectedSeqNumber;   //所缺的第一个序列号
    int m_nRecordCount;         //总记录数
    int m_nHead;                //头指针
};


class CSlideWndStack : public CProtocolStack
{
public:
	CSlideWndStack(CNetApp* const pNetApp, UINT4 const uStackId);
	virtual ~CSlideWndStack();

	//返回本对象中当前已接收的且可以提交给前存盘模块保存的话单数据的字节数
	int GetReadyDataSize(void);

    //返回本对象中当前已接收的且可以提交给前存盘模块保存的话单数据的记录数
	int GetReadyRecordCount(void);

    //将本协议栈可提交给前存盘模块保存的话单数据放入*pDataBuf对象中
	void GetReadyData(CDataBuffer* const pDataBuf);

    //从主状态文件中读取本协议栈的状态信息
	int LoadStatus(void);

    //向主状态文件中写入本协议栈的状态信息
	int SaveStatus(void);

    //处理从与本协议栈对应的主机模块或GSN发送过来的一帧数据 
	void OnReceive(void* pFrame);

    //对上次提供给前存盘模块保存的话单数据进行后续的处理
	void OnResponse(BOOL const bSaved);

	//进行话单预处理
	virtual void PreProcessBill(void* const pMsg);

	//进行话单帧的预处理
    virtual int PreProcessFrame(BYTE*& pData, int& nLen);

    //by ldf 2003-03-24 对应问题单D07718
    void Timer1Sec();
    //end
protected:
    //向对应的主机模块或GSN发送协议帧
	int SendFrame2Ex(void* pFrame);

	//从主机发送的消息中提取出命令码以及所带的数据
	virtual void ExplainFrame(const void* pFrame, int& nCmd, 
							  void*& pData, int& nLen);

	//发送同步应答帧号的应答帧
	virtual void SendResAckNumFrm();

	//发送确认帧
	virtual void SendAckFrm();

	//发送拒绝帧
	virtual void SendRejFrm();

protected:
	CSlideWnd m_SlideWnd;	//本协议栈对应的滑动窗口
	UINT4 m_nAckNumber;		//应答/拒绝帧的起始帧号
	SMemFrame* m_pAckFrame;	//给对应的主机模块的应答/拒绝帧
	int m_nBillRecordSize;	//一条话单记录的大小
	int m_nBillMsgSize;		//主机发送的一个话单消息的大小
	SSlideWndModInfo m_SlideNodeInfo;	//本协议栈对应的状态记录
    int m_nRecvAcceptCount;             //收到的Accept帧数
    int m_nMaxRecvAcceptCount;          //收到超过本值的Accept帧且一直
                                        //未给应答时，强制给一个应答
    BOOL m_bHasUnrepliedFrm;            //是否有未应答的帧
};

class CGSMStack : public CSlideWndStack
{
public:
	CGSMStack(CNetApp* const pNetApp, UINT4 const uStackId);
	virtual ~CGSMStack();

	//进行话单预处理
	void PreProcessBill(void* const pMsg);
};

class C128Stack : public CSlideWndStack
{
public:
	C128Stack(CNetApp* const pNetApp, UINT4 const uStackId);
	virtual ~C128Stack();

	//进行话单帧的预处理
    virtual int PreProcessFrame(BYTE*& pData, int& nLen);
protected:
    BYTE* m_pBillBuffer;
    int   m_nBuffLen;
};

class CSoftXStack : public CSlideWndStack
{
public:
	CSoftXStack(CNetApp* const pNetApp, UINT4 const uStackId);
	virtual ~CSoftXStack();

    //处理从与本协议栈对应的主机模块或GSN发送过来的一帧数据 
	void OnReceive(void* pFrame);


protected:
    //向对应的主机模块或GSN发送协议帧
	int SendFrame2Ex(void* pFrame);

	//从主机发送的消息中提取出命令码以及所带的数据
	virtual void ExplainFrame(const void* pFrame, int& nCmd, 
							  void*& pData, int& nLen);

	//发送同步应答帧号的应答帧
	virtual void SendResAckNumFrm();

	//发送确认帧
	virtual void SendAckFrm();

	//发送拒绝帧
	virtual void SendRejFrm();

protected:
	SSysFrame* m_pSysAckFrame;	//给对应的主机模块的应答/拒绝帧
};

class CSlideWndNetApp : public CNetApp
{
public:
	CSlideWndNetApp(CNetWork* const pNetWork);
	virtual ~CSlideWndNetApp();

    //初始化函数
    virtual int Init(void);

    //从主状态文件中读取所有协议栈的状态信息
    int LoadStatus(void);

    //向主状态文件中写入所有协议栈的状态信息
    int SaveStatus(void);

    //处理来自网络层的协议帧
    void OnReceive(SNetFrame* pFrame);

	//转发来自告警模块的告警帧
	void DispatchAlarm(MSG_INTRA* pMsg);


protected:
	//创建一个新的协议栈
	virtual CProtocolStack* CreateStack(UINT4 uStackId);

public:
	//统计所接收到的重复话单数据帧数(SLD_REPEATDATA_FRAME)
	CPerfItem* m_pRptBillPerf;

    //统计所接收到的正常话单数据帧数(SLD_NORMALDATA_FRAME)
    CPerfItem* m_pNmlBillPerf;

    //统计所接收到的所有话单数据帧数(SLD_ALL_FRAME)
    CPerfItem* m_pAllFrmPerf;
};

class CGSMNetApp : public CSlideWndNetApp
{
public:
	CGSMNetApp(CNetWork* const pNetWork);
	virtual ~CGSMNetApp();

protected:
	//创建一个新的协议栈
	CProtocolStack* CreateStack(UINT4 uStackId);
};

class C128NetApp : public CSlideWndNetApp
{
public:
	C128NetApp(CNetWork* const pNetWork);
	virtual ~C128NetApp();

protected:
	//创建一个新的协议栈
	CProtocolStack* CreateStack(UINT4 uStackId);
};


class CSoftXNetApp : public CSlideWndNetApp
{
public:
	CSoftXNetApp(CNetWork* const pNetWork);
	virtual ~CSoftXNetApp();

    //处理来自网络层的协议帧
    void OnReceive(SNetFrame* pFrame);

	//转发来自告警模块的告警帧
	void DispatchAlarm(MSG_INTRA* pMsg);


protected:
	//创建一个新的协议栈
	virtual CProtocolStack* CreateStack(UINT4 uStackId);
};


//实现了活动窗口协议数据帧的分析
class CSlideWndProtocolTrack : public CProtocolTrack
{
public:
    CSlideWndProtocolTrack();
    virtual ~CSlideWndProtocolTrack();

    //实施对协议帧的分析,由函数内部分配内存，调用者不需要释放
    const char* Analysis(void* const pFrame, int nProcRst = 0);

protected:
	//从指定的消息数据中提取出命令码以及所带的数据
	virtual void ExplainFrame(const SNetFrame* pMsg, 
							  int& nCmd, BYTE*& pData, int& nLen) = 0;

	//从指定的消息数据中提取出发送模块号
	virtual UINT4 GetSenderMid(const SNetFrame* pMsg) = 0;

	//从指定的消息数据中提取出接收模块号
	virtual UINT4 GetReceiverMid(const SNetFrame* pMsg) = 0;

	//判断指定的消息是否为主机告警应答
	virtual BOOL IsAlarmResponse(const SNetFrame* pMsg) = 0;

	//判断指定的消息是否为二进制告警消息
	virtual BOOL IsBinAlarm(const SNetFrame* pMsg) = 0;
	
	//根据指定的主机告警应答消息获取相关的描述信息
	virtual void GetAlarmResponseContent(char* szBuffer, 
										 const SNetFrame* pMsg) = 0;

	//根据指定的告警消息获取相关的描述信息
	virtual void GetAlarmContent(char* szBuffer, const SNetFrame* pMsg) = 0;
};

//实现了使用SAppFrame数据结构的滑动窗口协议数据帧的分析
class CAppFrmTrack : public CSlideWndProtocolTrack
{
public:
    CAppFrmTrack();
    virtual ~CAppFrmTrack();

protected:
	//从指定的消息数据中提取出命令码以及所带的数据
	virtual void ExplainFrame(const SNetFrame* pMsg, 
							  int& nCmd, BYTE*& pData, int& nLen);

	//从指定的消息数据中提取出发送模块号
	virtual UINT4 GetSenderMid(const SNetFrame* pMsg);

	//从指定的消息数据中提取出接收模块号
	virtual UINT4 GetReceiverMid(const SNetFrame* pMsg);

	//判断指定的消息是否为主机告警应答
	virtual BOOL IsAlarmResponse(const SNetFrame* pMsg);

	//判断指定的消息是否为二进制告警消息
	virtual BOOL IsBinAlarm(const SNetFrame* pMsg);

	//根据指定的主机告警应答消息获取相关的描述信息
	virtual void GetAlarmResponseContent(char* szBuffer, 
										 const SNetFrame* pMsg);

	//根据指定的告警消息获取相关的描述信息
	virtual void GetAlarmContent(char* szBuffer, const SNetFrame* pMsg);
};

//实现了使用SSysFrame数据结构的滑动窗口协议数据帧的分析
class CSysFrmTrack : public CSlideWndProtocolTrack
{
public:
    CSysFrmTrack();
    virtual ~CSysFrmTrack();

protected:
	//从指定的消息数据中提取出命令码以及所带的数据
	virtual void ExplainFrame(const SNetFrame* pMsg, 
							  int& nCmd, BYTE*& pData, int& nLen);

	//从指定的消息数据中提取出发送模块号
	virtual UINT4 GetSenderMid(const SNetFrame* pMsg);

	//从指定的消息数据中提取出接收模块号
	virtual UINT4 GetReceiverMid(const SNetFrame* pMsg);

	//判断指定的消息是否为主机告警应答
	virtual BOOL IsAlarmResponse(const SNetFrame* pMsg);

	//判断指定的消息是否为二进制告警消息
	virtual BOOL IsBinAlarm(const SNetFrame* pMsg);

	//根据指定的主机告警应答消息获取相关的描述信息
	virtual void GetAlarmResponseContent(char* szBuffer, 
										 const SNetFrame* pMsg);

	//根据指定的告警消息获取相关的描述信息
	virtual void GetAlarmContent(char* szBuffer, const SNetFrame* pMsg);
};

#endif
