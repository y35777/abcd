#ifndef  __PROTOCOL_STACK_H__
#define  __PROTOCOL_STACK_H__

#include "../include/frame.h"

//MSC��128��STP������������ģ����Ʒ�����֮����û�������Э��
//���л����Ĵ��ͣ�GSN��Ʒ�����֮�����GTP'Э����л����Ĵ���.
//CProtocolStack����������ģ���GSN��Ʒ�����֮��Ļ�������Э�顣
//һ��CProcotolStack������һ������ģ���GSN���Ӧ��
//��Ծ���Ļ�������Э�飬��Ҫ��CProtocolStack�������������
class CNetApp;
class CDataBuffer;
class CStackStatusInfo;
class CProtocolStack
{
public:
    CProtocolStack(CNetApp* const pNetApp, UINT4 const uStackId);
    virtual ~CProtocolStack(void);

    //��ñ�Э��ջ�ı��
    UINT4 GetStackId(void);

    //���ر������е�ǰ�ѽ��յ��ҿ����ύ��ǰ����ģ�鱣��Ļ������ݵ��ֽ���
    virtual int GetReadyDataSize(void);

    //���ر������е�ǰ�ѽ��յ��ҿ����ύ��ǰ����ģ�鱣��Ļ������ݵļ�¼��
    virtual int GetReadyRecordCount(void);

    //����Э��ջ���ύ��ǰ����ģ�鱣��Ļ������ݷ���*pDataBuf������
    virtual void GetReadyData(CDataBuffer* const pDataBuf);

    //������뱾Э��ջ��Ӧ������ģ���GSN���͹�����һ֡���� 
    virtual void OnReceive(void* pFrame);

    //���ϴ��ṩ��ǰ����ģ�鱣��Ļ������ݽ��к����Ĵ���
    virtual void OnResponse(BOOL const bSaved);

    //����״̬�ļ��ж�ȡ��Э��ջ��״̬��Ϣ
    virtual int LoadStatus(void);

    //����״̬�ļ���д�뱾Э��ջ��״̬��Ϣ
    virtual int SaveStatus(void);

    //���¼�������
    virtual void Timer1Sec(void);

    //��ô����һ�ν��յ����ݿ�ʼ��������������ʱ�䣬��λΪ��
    UINT4 GetInertiaPeriod();

    inline time_t GetLastRecvTime(void)
    {
        return m_lLastRecvTime;
    }

protected:

    //���Ӧ������ģ���GSN����Э��֡
    virtual int SendFrame2Ex(void* pFrame) = 0;

    //�ó�Ա������ʾЭ��ջ�ı��
    //�ó�Ա������ȡֵ�������뱾�����Ӧ������ģ���ģ��ţ�
    //�������� �������Ӧ��GSN��IP��ַ
    UINT4 m_uStackId;

    //�����һ�ν��յ����ݿ�ʼ��������������ʱ�䣬��λΪ�룻
    UINT4 m_uInertiaPeriod;

    //��ָ��ָ��Э��ջ��״̬��Ϣ����
    CStackStatusInfo* m_pStatusInfo;

    //��ָ��ָ����Ϊ����ģ��Ӧ�ò�Ķ���
    CNetApp* m_pNetApp;

    //Э��ջ��ʶ
    char     m_szName[20];

    //�ϴ��յ�Э��֡��ʱ��
    time_t   m_lLastRecvTime;
};


#endif //__PROTOCOL_STACK_H__
