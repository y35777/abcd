#ifndef __COLLECTOR_AGENT_H__
#define __COLLECTOR_AGENT_H__

#include "../include/frame.h"


class CCollectorAgent : public CWorkModule
{
public:
    // 构造函数
    CCollectorAgent(void);

    // 析构函数
    virtual ~CCollectorAgent();

protected:

    // 命令解析与分发函数
    virtual void ParseMsg (MSG_INTRA* pMsgParsed);

    // 初始化函数
    virtual int InitInstance(void);

    // 结束工作处理函数
    virtual void ExitInstance(void);

    // 获取本模块类型函数
    virtual MODULE_TYPE GetModuleType(void);

    virtual void OnTimer(const long nTimerID);

    void Timer5Sec();

    void SendMMLReport(const char* szReport,
                       int nAppType,
                       int nClientNo);

    BOOL WriteCCSConfig(void);

    BOOL LoadCCSConfig(void);

    BOOL UpdateUserRight(void);

    //删除一条采集任务信息
    void RemoveCCSCfgItem(MSG_INTRA* pMsg);

    void AddCCSCfgItem(MSG_INTRA* pMsg);

    void GetCCSCfgInfo(MSG_INTRA* pMsg);

    void ModifyCCSCfgItem(MSG_INTRA* pMsg);

    //采集任务名到进程号的MAP
    MAP<STRING, int>           m_ColToProcType;

    //采集任务名到上次收到状态信息时间的MAP
    MAP<STRING, time_t>        m_ColToSHKTime;

    enum CO_MD_STATUS
    {
        CM_FAILURE           = -1,
        CM_DEACTIVE          = 0,
        CM_ACTIVE            = 1
    };
    //采集任务名到状态的MAP
    MAP<STRING, int>            m_ColToStatus;

    long                        m_lTimerID5Sec;

    long                        m_lSHKTimeOut;

    char                        m_szCfgFile[MAX_PATH];

    LIST<STRING>                m_CfgInfo;

    //用户权限表
    MAP<STRING, MAP<STRING, int> >  m_UserRight;
};

#endif //__COLLECTOR_AGENT_H__


