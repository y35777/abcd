#ifndef __ASYN_RESOURCE_OBJECT_H__
#define __ASYN_RESOURCE_OBJECT_H__


enum EASYNRES_STATUS
{
    EASYNRES_FAILURE,           //故障状态
    EASYNRES_ACTIVING,          //正在激活状态
    EASYNRES_ACTIVE,            //激活状态
    EASYNRES_DEACTIVING,        //正在去激活状态
    EASYNRES_DEACTIVE           //去激活状态
};

struct SClusterFrame;
class CComCluster;

class CAsynResObject
{
public:
    CAsynResObject(const char* const szResName);
    virtual ~CAsynResObject();

    //绑定CComCluster对象的指针
    void Attach(CComCluster* pComCluster);
    //设置异步资源状态
    void SetStatus(EASYNRES_STATUS nStatus);
    //获取资源名称
    const char* GetResName();
    //获取异步资源状态
    EASYNRES_STATUS GetStatus();

    //资源的状态检查函数
    virtual int CheckStatus() = 0;
    //资源创建函数
    virtual int Create();
    //资源的激活函数
    virtual int Active();
    //资源的去激活函数
    virtual int DeActive();

    //处理对端发送的双机消息
    virtual void OnReceive(SClusterFrame* pFrame);
    //获取外带数据
    virtual int GetAttachedData(void*& pData, int& nLen);
    //处理外带数据
    virtual void ProcAttachedData(void* pData, int nLen);

protected:
   //资源名
#define MAX_ASYN_RES_NAME   50
   char             m_szResName[MAX_ASYN_RES_NAME];

   //资源对象的状态
   EASYNRES_STATUS  m_nStatus;

   //CComCluster对象的指针
   CComCluster*     m_pComCluster;
};

#endif //__ASYN_RESOURCE_OBJECT_H__
