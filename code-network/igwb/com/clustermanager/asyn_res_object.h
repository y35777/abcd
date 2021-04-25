#ifndef __ASYN_RESOURCE_OBJECT_H__
#define __ASYN_RESOURCE_OBJECT_H__


enum EASYNRES_STATUS
{
    EASYNRES_FAILURE,           //����״̬
    EASYNRES_ACTIVING,          //���ڼ���״̬
    EASYNRES_ACTIVE,            //����״̬
    EASYNRES_DEACTIVING,        //����ȥ����״̬
    EASYNRES_DEACTIVE           //ȥ����״̬
};

struct SClusterFrame;
class CComCluster;

class CAsynResObject
{
public:
    CAsynResObject(const char* const szResName);
    virtual ~CAsynResObject();

    //��CComCluster�����ָ��
    void Attach(CComCluster* pComCluster);
    //�����첽��Դ״̬
    void SetStatus(EASYNRES_STATUS nStatus);
    //��ȡ��Դ����
    const char* GetResName();
    //��ȡ�첽��Դ״̬
    EASYNRES_STATUS GetStatus();

    //��Դ��״̬��麯��
    virtual int CheckStatus() = 0;
    //��Դ��������
    virtual int Create();
    //��Դ�ļ����
    virtual int Active();
    //��Դ��ȥ�����
    virtual int DeActive();

    //����Զ˷��͵�˫����Ϣ
    virtual void OnReceive(SClusterFrame* pFrame);
    //��ȡ�������
    virtual int GetAttachedData(void*& pData, int& nLen);
    //�����������
    virtual void ProcAttachedData(void* pData, int nLen);

protected:
   //��Դ��
#define MAX_ASYN_RES_NAME   50
   char             m_szResName[MAX_ASYN_RES_NAME];

   //��Դ�����״̬
   EASYNRES_STATUS  m_nStatus;

   //CComCluster�����ָ��
   CComCluster*     m_pComCluster;
};

#endif //__ASYN_RESOURCE_OBJECT_H__
