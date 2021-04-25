#ifndef __WLAN_AS_STACK_H__
#define __WLAN_AS_STACK_H__


#include "i_wlan_as_oci.h"
#include "wlan_as_encap.h"

class CProtocolStack;
class CWlanAsStack : public CProtocolStack
{
public:
    CWlanAsStack(CNetApp* const pNetApp, 
                 UINT4 const uStackId);
    virtual ~CWlanAsStack(void);

    //���ر������е�ǰ�ѽ��յ��ҿ����ύ��ǰ����ģ��
    //����Ļ������ݵ��ֽ���
    virtual int GetReadyDataSize(void);

    //���ر������е�ǰ�ѽ��յ��ҿ����ύ��ǰ����ģ��
    //����Ļ������ݵļ�¼��
    virtual int GetReadyRecordCount(void);

    //����Э��ջ���ύ��ǰ����ģ�鱣��Ļ������ݷ���*pDataBuf������
    virtual void GetReadyData(CDataBuffer* const pDataBuf);

    //���ϴ��ṩ��ǰ����ģ�鱣��Ļ������ݽ��к����Ĵ���
    virtual void OnResponse(BOOL const bSaved);

    //����״̬�ļ��ж�ȡ��Э��ջ��״̬��Ϣ
    virtual int LoadStatus(void);

    //����״̬�ļ���д�뱾Э��ջ��״̬��Ϣ
    virtual int SaveStatus(void);

    //���¼�������
    virtual void Timer1Sec(void);

protected:

    //���Ӧ������ģ���GSN����Э��֡
    virtual int SendFrame2Ex(void* pFrame);

    IWlanAsOci*    m_pWlanAsOci;
    
    CWlanUtilEncap m_WlanUtilEncap;
};

#endif //__WLAN_AS_STACK_H__
