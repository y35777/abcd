#ifndef  __NET_APP_H__
#define  __NET_APP_H__

#include "../include/frame.h"
#include "../comm/comm_struct.h"
#include "../include/perf_item.h"

//CNetApp����������ģ���Ӧ�ò㡣֧�ֲ�ͬЭ���Ӧ�ò㣬
//��Ҫ��CNetApp�������������
class CNetWork;
class CProtocolStack;
class CDataBuffer;
class CNetApp
{
public:

    CNetApp(CNetWork* const pNetWork);
    virtual ~CNetApp();

    //��ʼ������
    virtual int Init(void);

    //��ǰ����ģ��֪ͨ����ģ�������ָ��Ѿ���ɺ󣬱�����������
    virtual void Active(void);

    //������ģ���ڽ��ո���������ȥ��������ʱ��������������
    virtual void DeActive(void);

    //����״̬�ļ��ж�ȡ����Э��ջ��״̬��Ϣ
    virtual int LoadStatus(void);

    //����״̬�ļ���д������Э��ջ��״̬��Ϣ
    virtual int SaveStatus(void);

    //��������������Э��֡
    virtual void OnReceive(SNetFrame* pFrame);

    //���¼�������
    virtual void Timer1Sec(void);

    //��������Э��ջ�е�ǰ�ѽ��յ��ҿ����ύ��ǰ����ģ�鱣��
    //�Ļ������ݵ��ֽ���
    virtual int GetReadyDataSize(void);

    //��������Э��ջ�е�ǰ�ѽ��յ��ҿ����ύ��ǰ����ģ�鱣��
    //�Ļ������ݵļ�¼��
    virtual int GetReadyRecordCount(void);

    //������Э��ջ���ύ��ǰ����ģ�鱣��Ļ������ݷ���*pDataBuf������
    virtual void GetReadyData(CDataBuffer* const pDataBuf);

    //���ϴ��ṩ��ǰ����ģ�鱣��Ļ������ݽ��к����Ĵ���
    virtual void OnResponse(BOOL const bSaved);

    //��ȡCNetWork*�����ָ��
    CNetWork* GetNetWork();

    //���Ӧ������ģ���GSN����Э��֡
    virtual int SendFrame2Ex(MSG_INTRA* pMsg, CProtocolStack* pStack);

	//����Զ2002.04.16���
	//ת�����Ը澯ģ��ĸ澯֡
	virtual void DispatchAlarm(MSG_INTRA* pMsg);
	//����Զ2002.04.16��ӽ���
 
    //������ 2003-05-16
    CPerfItem *m_pReceiveBillCnt;   //ƽ�����ջ�������
    
protected:

    //��MAP�ļ�ֵΪЭ��ջ�ı�ţ����ڻ�������Э��ջ��
    //���Ϊ��Ӧ������ģ��ı�ţ�����GTP'Э��ջ����
    //��Ϊ��Ӧ��GSN��IP��ַ,��MAP��Ԫ��ΪЭ��ջ�����ָ��
    typedef MAP<unsigned long, CProtocolStack*> MAP_ID_TO_PS;
    MAP_ID_TO_PS m_StackMan;

    //���ó�Ա����ȡֵΪSTACK_SLIDE����Ӧ�ò��Э��ջ��
    //״̬��ϢΪ_SlideWndModInfo�ṹ
    //���ó�Ա����ȡֵΪSTACK_GTP����Ӧ�ò��Э��ջ��״
    //̬��ϢΪ_GtpNodeInfo�ṹ
    int m_nStackStatusInfoType;

    //��ָ��ָ����Ϊ��������ģ���CNetWork����
    CNetWork* m_pNetWork;

    //�ó�Ա������ʾӦ�ò��Ƿ񱻼���
    BOOL m_bActive;

    //�������Э��ջ(���ڴ��ַΪ����)�ϴ��յ�Э��֡��ʱ���
    MAP<UINT4, UINT4>  m_Stack_To_RecvInv;
};


#endif //__NET_APP_H__
