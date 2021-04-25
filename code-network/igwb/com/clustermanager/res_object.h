#ifndef __RESOURCE_OBJECT_H__
#define __RESOURCE_OBJECT_H__

enum ERES_TYPE
{
    ERES_VIRTUAL_IP,            //����IP��Դ
    ERES_VOLUME,                //ͨ�þ���Դ
    ERES_IBM_VOLUME,            //IBM�������еľ���Դ
    ERES_SERVICE,               //������Դ
    ERES_PROGRAM,               //ͨ��Ӧ�ó�����Դ
    ERES_USERDEFINED            //�û��Զ�����Դ
};
#define S_RES_VIRTUAL_IP        "IP"
#define S_RES_VOLUME            "Volume"
#define S_RES_IBM_VOLUME        "IBMVolume"
#define S_RES_SERVICE           "Service"
#define S_RES_PROGRAM           "Program"
#define S_RES_USERDEFINED       "UserDefined"


enum ERES_STATUS
{
    ERES_STATUS_INIT,           //��ʼ��״̬
    ERES_STATUS_ACTIVE,         //����״̬
    ERES_STATUS_DEACTIVE,       //ȥ����״̬
    ERES_STATUS_FAILURE         //����״̬
};

#define MAX_RES_NUM     31      //�����õ������Դ��Ϊ31��
class CResObject
{
/*
    ��Ա����
*/
public:
    CResObject(const char* const szResName,
               ERES_TYPE nResType,
               int nMaxLocalRecovery);
    virtual ~CResObject();

    UINT4 GetAlarmID();         //��ȡ��Դ�澯ID
    UINT4 GetAlarmPara();       //��ȡ��Դ�澯����
    UINT4 GetResIndex();        //��ȡ��Դ������
    ERES_TYPE GetResType();     //��ȡ��Դ����
    ERES_STATUS GetStatus();    //��ȡ��Դ״̬
    int   GetSwitchGroup();     //��ȡ��Դ�ĵ������

    //��ȡ��Դ����
    const char* CResObject::GetResName();

    virtual int Create();       //��Դ��������
    virtual int CheckStatus();  //��Դ��״̬��麯��
    virtual int Active();       //��Դ�ļ����
    virtual int DeActive();     //��Դ��ȥ�����
    virtual void Remove();      //�ͷ���Դǰ��������
    int LocalRecovery();        //ִ�б��ػָ�����

/*
    ��Ա����
*/
protected:

    //��Դ��
#define MAX_RES_NAME  50
    char        m_szResName[MAX_RES_NAME];

    //��Դ����
    ERES_TYPE   m_nResType;

    //��󱾵ػָ�����
    int         m_nMaxLocalRecovery;

    //�ѱ��ػָ��Ĵ���
    int         m_nLocalRecovery;

    //�ϴμ���ʱ��
    time_t      m_tLastActiveTime;

    //��Դ�����״̬
    ERES_STATUS m_nStatus;

    //�澯ID
    UINT4       m_uAlarmID;

    //�澯����
    UINT4       m_uAlarmPara;

    //��Դ������
    UINT4       m_uResIndex;

    //������ţ�Ϊ��ֵ��ʾ��������Ϊ0��ʾ��������������
    //����0��ʾҪ��ͬ��ŵ�������Դȫ��ʧЧ�ŵ���
    int         m_nSwitchGroup;

    friend class CResObjectCreator;
};

#endif //__RESOURCE_OBJECT_H__
