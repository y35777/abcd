#ifndef  __WLAN_AS_APP_H__
#define  __WLAN_AS_APP_H__

#include "../include/frame.h"

class CNetApp;
class CWlanAsApp : public CNetApp
{
public:

    CWlanAsApp(CNetWork* const pNetWork);
    virtual ~CWlanAsApp();

    //��ʼ������
    virtual int Init(void);

    //����״̬�ļ��ж�ȡ����Э��ջ��״̬��Ϣ
    virtual int LoadStatus(void);

    //����״̬�ļ���д������Э��ջ��״̬��Ϣ
    virtual int SaveStatus(void);

    //���¼�������
    virtual void Timer1Sec(void);

protected:

};


#endif //__WLAN_AS_APP_H__
