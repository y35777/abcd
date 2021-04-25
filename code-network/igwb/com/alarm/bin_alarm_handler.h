//  作者：张玉新  版权：华为公司
#ifndef _BIN_ALARM_HANDLER
#define _BIN_ALARM_HANDLER

#include "alarm_handler.h"

//负责实现二进制告警，一个本对象可以负责发送1到多个接入点的告警
class CBinAlarmHandler : public CAlarmHandler
{
public:

    CBinAlarmHandler(BYTE yAccessPointID, UINT2 uBaseAlarmID,
                     const char* szAlarmShield, int nProductType,
                     int nBillsvrNo, CAlarmServer* pAlarmServer,
                     const char* AccessPointName);    
    virtual ~CBinAlarmHandler(){};
    //初始化函数
    //virtual int Init(const char* szCfgPath);

    //处理从其它模块发送过来的非MML消息
    virtual void HandleFrameFromApp(MSG_INTRA *pFrame);

    //往活动告警队列中加入一条新的告警
    virtual ALARM_ADD_RESULT AddAlarm(SInnerAlarm* pNewAlarm, SActiveAlarm** ppActiveAlarm, int nRecoverType = NOT_RECOVER);

    //根据SActiveAlarm结构发送二进制告警
    virtual void SendAlarm(SActiveAlarm* pAlarm) = 0;

    //virtual void HandleFrameFromMML(MSG_INTRA *pFrame);    

    //分析收到的主机的二进制告警应答消息，从中得到相应的告警信息
    virtual BOOL ParseAlarmResponse(MSG_INTRA* pMsg,SActiveAlarm *&pActive) = 0;
protected:
    //从SActiveAlarm结构中获取用于发往主机的告警ID
    virtual UINT2 GetOuterAlarmId(SActiveAlarm* pAlarm);

    //根据主机应答中的告警ID来设置指定SActiveAlarm结构中的告警ID及告警类型
    virtual void SetInnerAlarmId(SActiveAlarm* pAlarm, UINT2 uAlarmId);
    BYTE  m_yAPID;              //本对象对应的接入点号
    BOOL  m_bProcAllAPAlarm;    //是否处理所有接入点的告警
    int   m_nAPType;            //本接入点的产品类型
    UINT2 m_uBaseAlarmID;       //本接入点的告警ID基数
};


class CSoftXBinAlarmHandler : public CBinAlarmHandler
{
public:
    CSoftXBinAlarmHandler(BYTE yAccessPointID, UINT2 uBaseAlarmID,
                          const char* szAlarmShield, int nProductType,
                          int nBillsvrNo, CAlarmServer* pAlarmServer,
                          const char* AccessPointName);
    
    virtual ~CSoftXBinAlarmHandler(){};
    //秒时钟处理函数
    virtual void Timer1Sec();
    //处理收到的主机告警应答
    virtual void AlarmReceived(MSG_INTRA* pFrame);
    //往活动告警队列中加入一条新的告警
    virtual void SendAlarm(SActiveAlarm* pAlarm);
    //分析收到的主机的二进制告警应答消息，从中得到相应的告警信息
    virtual BOOL ParseAlarmResponse(MSG_INTRA* pMsg,SActiveAlarm *&pActive);
    unsigned char m_nLastSendHandle;   //上一次发送的Handle值
protected:
    UINT2 m_nAlarmTimerCnt;  //告警发送的超时计数
    UINT2 m_nTimeOut;        //发送告警的超时值
};


class CMSCBinAlarmHandler : public CBinAlarmHandler
{
public:
    CMSCBinAlarmHandler(BYTE yAccessPointID, UINT2 uBaseAlarmID,
                        const char* szAlarmShield, int nProductType,
                        int nBillsvrNo, CAlarmServer* pAlarmServer,
                        const char* AccessPointName);
    
    virtual ~CMSCBinAlarmHandler(){};
    //秒时钟处理函数
    virtual void Timer1Sec();
    //处理收到的主机告警应答
    virtual void AlarmReceived(MSG_INTRA* pFrame);
    //根据SActiveAlarm结构发送二进制告警
    virtual void SendAlarm(SActiveAlarm* pAlarm);
    //分析收到的主机的二进制告警应答消息，从中得到相应的告警信息
    virtual BOOL ParseAlarmResponse(MSG_INTRA* pMsg,SActiveAlarm *&pActive);
};
//定义访问器
class CAlarmCfgVisitor
{
    CBinAlarmHandler *m_pHandler;
public:
    CAlarmCfgVisitor(CBinAlarmHandler *pHandler):m_pHandler(pHandler){};
    bool operator()(SAlarmExplain *pExplain)
    {
        ALARM_ADD_RESULT nResult;
        SInnerAlarm InnerAlarm;
        SActiveAlarm* pActiveAlarm = NULL;
        InnerAlarm.yAlarmType = AT_RESTORE;
        InnerAlarm.uAlarmID = pExplain->uAlarmID;
        if (0 == pExplain->yParaNum)//张玉新 2002-05-10
        {
            //InnerAlarm.yAlarmPara = 0xFF; //0xFF表示无效的告警参数
            nResult = m_pHandler->AddAlarm(&InnerAlarm, &pActiveAlarm, RESET_RECOVER);
            if (ADD_OK != nResult) //张玉新删掉pActiveAlarm == NULL 判断
            {
                TRACE(MTS_ALARM,S_ALARM_INITADD_FAIL,InnerAlarm.uAlarmID,InnerAlarm.yAlarmType,InnerAlarm.yAlarmPara);
            }
        }
        else
        {
            for (BYTE j = 0; j < 15; j++)
            {
                InnerAlarm.yAlarmPara[0] =(BYTE)j;
                nResult = m_pHandler->AddAlarm(&InnerAlarm, &pActiveAlarm, RESET_RECOVER);
                if (ADD_OK != nResult) //张玉新删掉pActiveAlarm == NULL 判断
                {
                    TRACE(MTS_ALARM,S_ALARM_INITADD_FAIL,InnerAlarm.uAlarmID,InnerAlarm.yAlarmType,InnerAlarm.yAlarmPara);
                }
            }//for
        }//if
        return true;
    }
};  //定义的访问器结束
#endif
