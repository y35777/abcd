//���ļ�������Զ2002.04.09����

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


//ʵ�ֻ�������
class CSlideWnd 
{
public:
    //���캯��/��������
    CSlideWnd();
    ~CSlideWnd();

    //��ʼ����������
    void Init(int nSize, int nBaseSeqNumber, int nBillMsgSize, 
			  int nBillRecordSize, int nMpuWindowSize);

    //����һ����¼
    int AddRecord(SBillMessage *pMsg);
    //������������
    void Slide(int nCount);
    //ͬ����������
    void Synchronize(int nBaseSeqNumber);

    //��ȡ��������ͷ֡��
    int GetBaseSeqNumber() 
	{ 
		return m_nBaseSeqNumber; 
	}
    //��ȡ���������е�һ����ȱ�����к�
    int GetExpectedSeqNumber() 
	{ 
		return m_nExpectedSeqNumber; 
	}
    //��ȡ���������п����ύ�ļ�¼��
    int GetReadyRecordCount() 
    { 
		return m_nExpectedSeqNumber - m_nBaseSeqNumber; 
	}
    //��ȡ�����������ܵļ�¼��
    int GetRecordCount() 
	{ 
		return m_nRecordCount; 
	}
    //��ȡ���ύ����
    int GetReadyData(CDataBuffer* pDataBuffer);

private:
	int m_nMpuWindowSize;		//�����Ļ������ڴ�С
    int m_nSize;                //�������ڴ�С
    BYTE* m_pData;				//������������
	int m_nBillRecordSize;		//������¼��С
	int m_nBillMsgSize;			//������Ϣ��С
    int m_nBaseSeqNumber;       //��������ͷ���к�
    int m_nExpectedSeqNumber;   //��ȱ�ĵ�һ�����к�
    int m_nRecordCount;         //�ܼ�¼��
    int m_nHead;                //ͷָ��
};


class CSlideWndStack : public CProtocolStack
{
public:
	CSlideWndStack(CNetApp* const pNetApp, UINT4 const uStackId);
	virtual ~CSlideWndStack();

	//���ر������е�ǰ�ѽ��յ��ҿ����ύ��ǰ����ģ�鱣��Ļ������ݵ��ֽ���
	int GetReadyDataSize(void);

    //���ر������е�ǰ�ѽ��յ��ҿ����ύ��ǰ����ģ�鱣��Ļ������ݵļ�¼��
	int GetReadyRecordCount(void);

    //����Э��ջ���ύ��ǰ����ģ�鱣��Ļ������ݷ���*pDataBuf������
	void GetReadyData(CDataBuffer* const pDataBuf);

    //����״̬�ļ��ж�ȡ��Э��ջ��״̬��Ϣ
	int LoadStatus(void);

    //����״̬�ļ���д�뱾Э��ջ��״̬��Ϣ
	int SaveStatus(void);

    //������뱾Э��ջ��Ӧ������ģ���GSN���͹�����һ֡���� 
	void OnReceive(void* pFrame);

    //���ϴ��ṩ��ǰ����ģ�鱣��Ļ������ݽ��к����Ĵ���
	void OnResponse(BOOL const bSaved);

	//���л���Ԥ����
	virtual void PreProcessBill(void* const pMsg);

	//���л���֡��Ԥ����
    virtual int PreProcessFrame(BYTE*& pData, int& nLen);

    //by ldf 2003-03-24 ��Ӧ���ⵥD07718
    void Timer1Sec();
    //end
protected:
    //���Ӧ������ģ���GSN����Э��֡
	int SendFrame2Ex(void* pFrame);

	//���������͵���Ϣ����ȡ���������Լ�����������
	virtual void ExplainFrame(const void* pFrame, int& nCmd, 
							  void*& pData, int& nLen);

	//����ͬ��Ӧ��֡�ŵ�Ӧ��֡
	virtual void SendResAckNumFrm();

	//����ȷ��֡
	virtual void SendAckFrm();

	//���;ܾ�֡
	virtual void SendRejFrm();

protected:
	CSlideWnd m_SlideWnd;	//��Э��ջ��Ӧ�Ļ�������
	UINT4 m_nAckNumber;		//Ӧ��/�ܾ�֡����ʼ֡��
	SMemFrame* m_pAckFrame;	//����Ӧ������ģ���Ӧ��/�ܾ�֡
	int m_nBillRecordSize;	//һ��������¼�Ĵ�С
	int m_nBillMsgSize;		//�������͵�һ��������Ϣ�Ĵ�С
	SSlideWndModInfo m_SlideNodeInfo;	//��Э��ջ��Ӧ��״̬��¼
    int m_nRecvAcceptCount;             //�յ���Accept֡��
    int m_nMaxRecvAcceptCount;          //�յ�������ֵ��Accept֡��һֱ
                                        //δ��Ӧ��ʱ��ǿ�Ƹ�һ��Ӧ��
    BOOL m_bHasUnrepliedFrm;            //�Ƿ���δӦ���֡
};

class CGSMStack : public CSlideWndStack
{
public:
	CGSMStack(CNetApp* const pNetApp, UINT4 const uStackId);
	virtual ~CGSMStack();

	//���л���Ԥ����
	void PreProcessBill(void* const pMsg);
};

class C128Stack : public CSlideWndStack
{
public:
	C128Stack(CNetApp* const pNetApp, UINT4 const uStackId);
	virtual ~C128Stack();

	//���л���֡��Ԥ����
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

    //������뱾Э��ջ��Ӧ������ģ���GSN���͹�����һ֡���� 
	void OnReceive(void* pFrame);


protected:
    //���Ӧ������ģ���GSN����Э��֡
	int SendFrame2Ex(void* pFrame);

	//���������͵���Ϣ����ȡ���������Լ�����������
	virtual void ExplainFrame(const void* pFrame, int& nCmd, 
							  void*& pData, int& nLen);

	//����ͬ��Ӧ��֡�ŵ�Ӧ��֡
	virtual void SendResAckNumFrm();

	//����ȷ��֡
	virtual void SendAckFrm();

	//���;ܾ�֡
	virtual void SendRejFrm();

protected:
	SSysFrame* m_pSysAckFrame;	//����Ӧ������ģ���Ӧ��/�ܾ�֡
};

class CSlideWndNetApp : public CNetApp
{
public:
	CSlideWndNetApp(CNetWork* const pNetWork);
	virtual ~CSlideWndNetApp();

    //��ʼ������
    virtual int Init(void);

    //����״̬�ļ��ж�ȡ����Э��ջ��״̬��Ϣ
    int LoadStatus(void);

    //����״̬�ļ���д������Э��ջ��״̬��Ϣ
    int SaveStatus(void);

    //��������������Э��֡
    void OnReceive(SNetFrame* pFrame);

	//ת�����Ը澯ģ��ĸ澯֡
	void DispatchAlarm(MSG_INTRA* pMsg);


protected:
	//����һ���µ�Э��ջ
	virtual CProtocolStack* CreateStack(UINT4 uStackId);

public:
	//ͳ�������յ����ظ���������֡��(SLD_REPEATDATA_FRAME)
	CPerfItem* m_pRptBillPerf;

    //ͳ�������յ���������������֡��(SLD_NORMALDATA_FRAME)
    CPerfItem* m_pNmlBillPerf;

    //ͳ�������յ������л�������֡��(SLD_ALL_FRAME)
    CPerfItem* m_pAllFrmPerf;
};

class CGSMNetApp : public CSlideWndNetApp
{
public:
	CGSMNetApp(CNetWork* const pNetWork);
	virtual ~CGSMNetApp();

protected:
	//����һ���µ�Э��ջ
	CProtocolStack* CreateStack(UINT4 uStackId);
};

class C128NetApp : public CSlideWndNetApp
{
public:
	C128NetApp(CNetWork* const pNetWork);
	virtual ~C128NetApp();

protected:
	//����һ���µ�Э��ջ
	CProtocolStack* CreateStack(UINT4 uStackId);
};


class CSoftXNetApp : public CSlideWndNetApp
{
public:
	CSoftXNetApp(CNetWork* const pNetWork);
	virtual ~CSoftXNetApp();

    //��������������Э��֡
    void OnReceive(SNetFrame* pFrame);

	//ת�����Ը澯ģ��ĸ澯֡
	void DispatchAlarm(MSG_INTRA* pMsg);


protected:
	//����һ���µ�Э��ջ
	virtual CProtocolStack* CreateStack(UINT4 uStackId);
};


//ʵ���˻����Э������֡�ķ���
class CSlideWndProtocolTrack : public CProtocolTrack
{
public:
    CSlideWndProtocolTrack();
    virtual ~CSlideWndProtocolTrack();

    //ʵʩ��Э��֡�ķ���,�ɺ����ڲ������ڴ棬�����߲���Ҫ�ͷ�
    const char* Analysis(void* const pFrame, int nProcRst = 0);

protected:
	//��ָ������Ϣ��������ȡ���������Լ�����������
	virtual void ExplainFrame(const SNetFrame* pMsg, 
							  int& nCmd, BYTE*& pData, int& nLen) = 0;

	//��ָ������Ϣ��������ȡ������ģ���
	virtual UINT4 GetSenderMid(const SNetFrame* pMsg) = 0;

	//��ָ������Ϣ��������ȡ������ģ���
	virtual UINT4 GetReceiverMid(const SNetFrame* pMsg) = 0;

	//�ж�ָ������Ϣ�Ƿ�Ϊ�����澯Ӧ��
	virtual BOOL IsAlarmResponse(const SNetFrame* pMsg) = 0;

	//�ж�ָ������Ϣ�Ƿ�Ϊ�����Ƹ澯��Ϣ
	virtual BOOL IsBinAlarm(const SNetFrame* pMsg) = 0;
	
	//����ָ���������澯Ӧ����Ϣ��ȡ��ص�������Ϣ
	virtual void GetAlarmResponseContent(char* szBuffer, 
										 const SNetFrame* pMsg) = 0;

	//����ָ���ĸ澯��Ϣ��ȡ��ص�������Ϣ
	virtual void GetAlarmContent(char* szBuffer, const SNetFrame* pMsg) = 0;
};

//ʵ����ʹ��SAppFrame���ݽṹ�Ļ�������Э������֡�ķ���
class CAppFrmTrack : public CSlideWndProtocolTrack
{
public:
    CAppFrmTrack();
    virtual ~CAppFrmTrack();

protected:
	//��ָ������Ϣ��������ȡ���������Լ�����������
	virtual void ExplainFrame(const SNetFrame* pMsg, 
							  int& nCmd, BYTE*& pData, int& nLen);

	//��ָ������Ϣ��������ȡ������ģ���
	virtual UINT4 GetSenderMid(const SNetFrame* pMsg);

	//��ָ������Ϣ��������ȡ������ģ���
	virtual UINT4 GetReceiverMid(const SNetFrame* pMsg);

	//�ж�ָ������Ϣ�Ƿ�Ϊ�����澯Ӧ��
	virtual BOOL IsAlarmResponse(const SNetFrame* pMsg);

	//�ж�ָ������Ϣ�Ƿ�Ϊ�����Ƹ澯��Ϣ
	virtual BOOL IsBinAlarm(const SNetFrame* pMsg);

	//����ָ���������澯Ӧ����Ϣ��ȡ��ص�������Ϣ
	virtual void GetAlarmResponseContent(char* szBuffer, 
										 const SNetFrame* pMsg);

	//����ָ���ĸ澯��Ϣ��ȡ��ص�������Ϣ
	virtual void GetAlarmContent(char* szBuffer, const SNetFrame* pMsg);
};

//ʵ����ʹ��SSysFrame���ݽṹ�Ļ�������Э������֡�ķ���
class CSysFrmTrack : public CSlideWndProtocolTrack
{
public:
    CSysFrmTrack();
    virtual ~CSysFrmTrack();

protected:
	//��ָ������Ϣ��������ȡ���������Լ�����������
	virtual void ExplainFrame(const SNetFrame* pMsg, 
							  int& nCmd, BYTE*& pData, int& nLen);

	//��ָ������Ϣ��������ȡ������ģ���
	virtual UINT4 GetSenderMid(const SNetFrame* pMsg);

	//��ָ������Ϣ��������ȡ������ģ���
	virtual UINT4 GetReceiverMid(const SNetFrame* pMsg);

	//�ж�ָ������Ϣ�Ƿ�Ϊ�����澯Ӧ��
	virtual BOOL IsAlarmResponse(const SNetFrame* pMsg);

	//�ж�ָ������Ϣ�Ƿ�Ϊ�����Ƹ澯��Ϣ
	virtual BOOL IsBinAlarm(const SNetFrame* pMsg);

	//����ָ���������澯Ӧ����Ϣ��ȡ��ص�������Ϣ
	virtual void GetAlarmResponseContent(char* szBuffer, 
										 const SNetFrame* pMsg);

	//����ָ���ĸ澯��Ϣ��ȡ��ص�������Ϣ
	virtual void GetAlarmContent(char* szBuffer, const SNetFrame* pMsg);
};

#endif
