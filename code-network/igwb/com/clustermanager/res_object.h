#ifndef __RESOURCE_OBJECT_H__
#define __RESOURCE_OBJECT_H__

enum ERES_TYPE
{
    ERES_VIRTUAL_IP,            //虚拟IP资源
    ERES_VOLUME,                //通用卷资源
    ERES_IBM_VOLUME,            //IBM磁盘阵列的卷资源
    ERES_SERVICE,               //服务资源
    ERES_PROGRAM,               //通用应用程序资源
    ERES_USERDEFINED            //用户自定义资源
};
#define S_RES_VIRTUAL_IP        "IP"
#define S_RES_VOLUME            "Volume"
#define S_RES_IBM_VOLUME        "IBMVolume"
#define S_RES_SERVICE           "Service"
#define S_RES_PROGRAM           "Program"
#define S_RES_USERDEFINED       "UserDefined"


enum ERES_STATUS
{
    ERES_STATUS_INIT,           //初始化状态
    ERES_STATUS_ACTIVE,         //激活状态
    ERES_STATUS_DEACTIVE,       //去激活状态
    ERES_STATUS_FAILURE         //故障状态
};

#define MAX_RES_NUM     31      //可配置的最大资源数为31个
class CResObject
{
/*
    成员函数
*/
public:
    CResObject(const char* const szResName,
               ERES_TYPE nResType,
               int nMaxLocalRecovery);
    virtual ~CResObject();

    UINT4 GetAlarmID();         //获取资源告警ID
    UINT4 GetAlarmPara();       //获取资源告警参数
    UINT4 GetResIndex();        //获取资源索引号
    ERES_TYPE GetResType();     //获取资源类型
    ERES_STATUS GetStatus();    //获取资源状态
    int   GetSwitchGroup();     //获取资源的倒换组号

    //获取资源名称
    const char* CResObject::GetResName();

    virtual int Create();       //资源创建函数
    virtual int CheckStatus();  //资源的状态检查函数
    virtual int Active();       //资源的激活函数
    virtual int DeActive();     //资源的去激活函数
    virtual void Remove();      //释放资源前的清理函数
    int LocalRecovery();        //执行本地恢复操作

/*
    成员变量
*/
protected:

    //资源名
#define MAX_RES_NAME  50
    char        m_szResName[MAX_RES_NAME];

    //资源类型
    ERES_TYPE   m_nResType;

    //最大本地恢复次数
    int         m_nMaxLocalRecovery;

    //已本地恢复的次数
    int         m_nLocalRecovery;

    //上次激活时间
    time_t      m_tLastActiveTime;

    //资源对象的状态
    ERES_STATUS m_nStatus;

    //告警ID
    UINT4       m_uAlarmID;

    //告警参数
    UINT4       m_uAlarmPara;

    //资源索引号
    UINT4       m_uResIndex;

    //倒换组号，为负值表示不倒换，为0表示发故障立即倒换
    //大于0表示要等同组号的所有资源全部失效才倒换
    int         m_nSwitchGroup;

    friend class CResObjectCreator;
};

#endif //__RESOURCE_OBJECT_H__
