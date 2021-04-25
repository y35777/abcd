#ifndef  __SCAN_APP_H__
#define  __SCAN_APP_H__

#include "../include/frame.h"
#include "../comm/comm_struct.h"

class CNetApp;
class CScanApp : public CNetApp
{
public:

    CScanApp(CNetWork* const pNetWork);
    virtual ~CScanApp();

    //��ʼ������
    virtual int Init(void);

    //����״̬�ļ��ж�ȡ����Э��ջ��״̬��Ϣ
    virtual int LoadStatus(void);

    //����״̬�ļ���д������Э��ջ��״̬��Ϣ
    virtual int SaveStatus(void);

    //���¼�������
    virtual void Timer1Sec(void);

protected:

    //����szPathĿ¼�������ļ�
    int ScanStack(const char* const szPath);

    //�������ɨ���Ŀ¼
    char m_szPathName[MAX_PATH];

    //ɨ��������λ����
    int m_nScanInv;

    //ɨ����������
    int m_nScanCounter;
};


#endif //__SCAN_APP_H__
