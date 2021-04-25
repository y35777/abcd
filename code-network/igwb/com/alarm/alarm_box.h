// alarm_box.h: interface for the CAlarmBox class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _ALARM_BOX
#define _ALARM_BOX
#include "../utility/mml_report.h"
#include "../include/base_type.h"
#include "../alarm_util/alarm_config.h"
#include "../alarm_util/alarm_struct.h"
#include "../frame/wrk_module.h"
#include "resource.h"
#include "box_comm.h"
/***********************************************************************************
以下定义与告警箱通讯相关的命令字根据告警箱通讯协议
************************************************************************************/
#define M_ALM_ALARM_BOX_RESET             0x02  /*复位告警箱命令字 */
#define M_ALM_ALARM_BOX_STOP_SOUND        0x07  /*停告警音命令字 */
#define M_ALM_ALARM_BOX_CONFIG            0x08  /*配置告警箱命令字 */
#define M_ALM_ALARM_BOX_RESPONSE_ONLINE   0x28  /*握手应答帧命令字 */
#define M_ALM_ALARM_BOX_DOWN_ALARM        0x50  /*下发告警信息命令字 */
#define M_ALM_ALARM_BOX_REDOWN_ALARM      0x51  /*重发告警信息命令字 */
#define M_ALM_ALARM_BOX_QUERY_VERSION     0x52  /*查询显示版本命令字 */
//*以下为告警箱响应帧命令字*/
#define M_ALM_ALARM_BOX_RESET_SUCCESS     0x14  /*复位成功状态命令字 */
#define M_ALM_ALARM_BOX_RESEND_ALARM      0x18  /*要求重发告警命令字 */

#define M_ALM_MAX_ALARM_BOX_COMMAND_LEN   100   /*发送到告警箱的命令字符串的最大长度 */
#define M_ALM_DEFAULT_ALARM_BOX_DATA_LEN  512   /*在与告警箱定时通讯时，可能获得的最大数据长度 */
/***********************************************************************************
以下定义与告警箱体通讯协议相关的部分协议关键字值
***********************************************************************************/
#define M_ALM_BOX_PROTOCOL_FS             0x7e  /*定义与告警箱通讯的协议头标志 */
/* 主机到告警箱 */
#define M_ALM_BOX_PROTOCOL_SEND_TA        0x1f  /*定义主机下发命令时的数据消息头TA内容 */
#define M_ALM_BOX_PROTOCOL_SEND_SA        0x00  /*定义主机下发命令时的数据消息包的SA内容 */

/* 告警箱响应 */
#define M_ALM_BOX_PROTOCOL_RESPONSE_TA    0x00  /* 定义告警箱响应时时的数据消息头TA内容 */
#define M_ALM_BOX_PROTOCOL_RESPONSE_SA    0x10  /* 定义告警箱响应时时的数据消息包的SA内容 */
typedef MAP<BYTE,UINT4> AlarmCountMap;
typedef LIST<UINT4> CsnList;
struct SAlarmInfo{
    UINT4   uCsn;       //告警流水号
    BYTE    yModuleNO;  //发送告警的主机模块号,对于iGateway bill,主为0,备为1
    UINT2   uAlarmID;   //告警编号
    BYTE    yAlarmType; //告警类型
    BYTE    yFuncID;    //告警箱告警分类
    BYTE    yAlarmLevel;//告警级别
    BYTE    ySubSysID;  //网管告警分类
    //告警产生的日期,时间
    UINT2   uYear;      //4位年
    BYTE    yMonth;     //2位月
    BYTE    yDay;       //2位日
    BYTE    yHour;      //2位时
    BYTE    yMin;       //2位分
    BYTE    ySec;       //2位秒
    BYTE    yCPUPos;    //A(0) B(1)机指示,对于iGateway bill,主为0,备为1
    BYTE    yPID;       //告警产生的进程标识
    BYTE    yPara[MAX_ALARM_PARA];  //告警参数
    BOOL    bACK;       //是否要发送停告警音命令
};

class CState;
class CAlarmBox
{
public:
    void OnTimer();
    void HandleAlarm(SActiveAlarm  *pFrame);
    void HandleAlarmCmdFromBox(MSG_INTRA *pFrame);
    void HandleMMLCmdFromClient(MSG_INTRA *pFrame);
    BOOL ChangeState(CState *pS);
    BOOL Init(int nConnectedMode, STRING szPortName);     
    BYTE IsSendToAlarmBox(UINT2  uAlarmID);
    CAlarmBox(CAlarmConfig *pCfg,CMMLReport *pReport);
    ~CAlarmBox();
protected:
    CAlarmConfig  *m_pCfg;              //告警配置查询
public:
    CState        *m_pState;            //状态指针
    CMMLReport    *m_pReport;           //MML报文处理报文
    AlarmCountMap m_AlarmCount;         //STD::MAP  四种故障告警的级别计数
    unsigned char m_szAlarmBoxVersion[64];  //告警箱版本
    int           m_nCnt;               //未握手的时长
    SAlarmInfo    m_sRecentAlarm;       //最近一次告警信息
    CsnList       m_EvtCsnList;         //开机以来或上次人工灭掉告警灯以来发生的所有紧急事件告警的流水号队列
    CCommBox      *m_CommBox;    
    int           m_nConnectedMode;
    
};

class CState
{
public:
    void SendMMLReportToClient(MSG_INTRA *pFrame,const char *pTitle,const char *pContent);
    void UpdateRecentAlarm(SActiveAlarm *pAlarm);
    void AnalysizeCommandFromBox(unsigned char* pAlarmBoxFrame, int nFrameLen,
                                 int &nCmd, unsigned char *pPara = NULL);      
    void SendCommandToBox(int nCmd,int nAlarmLevel = 0xFF,int nValue = 0xFF);
    void HandleMMLCmdFromClient(MSG_INTRA *pFrame);
    virtual BOOL HandleAlarm(SActiveAlarm *pFrame);
    virtual void HandleAlarmCmdFromBox(unsigned char* pAlarmBoxFrame, int nLen)       
    {
        m_pParent->m_nCnt = 0;
    };
    virtual void OnTimer()
    {
        m_pParent->m_nCnt += 1;
    };
    virtual BOOL Init() = 0;
    CState(CAlarmBox *pB);
protected:
    virtual void HandleMMLAlarmAckEVT(MSG_INTRA *pFrame); //处理事件告警确认
    virtual void HandleMMLAlarmACK(MSG_INTRA *pFrame);
    virtual void HandleMMLRSTALMBox(MSG_INTRA *pFrame) = 0;
    virtual void HandleMMLSTPBoxSND(MSG_INTRA *pFrame) = 0;
    virtual void HandleMMLCLRBoxLGT(MSG_INTRA *pFrame) = 0;
    virtual void HandleMMLLSTBoxVER(MSG_INTRA *pFrame) = 0;
    virtual void HandleMMLSetBoxLGT(MSG_INTRA *pFrame) = 0;
    CAlarmBox *m_pParent; //指向拥有此状态上层类的指针
};

class CInitState : public CState
{
public:
    CInitState(CAlarmBox *pB):CState(pB){
        m_bRecvResetSuc = FALSE;
    };
    virtual void HandleAlarmCmdFromBox(unsigned char* pAlarmBoxFrame, int nLen);       
    virtual void OnTimer();
    virtual BOOL Init();
protected:
    virtual void HandleMMLRSTALMBox(MSG_INTRA *pFrame);
    virtual void HandleMMLSTPBoxSND(MSG_INTRA *pFrame);
    virtual void HandleMMLCLRBoxLGT(MSG_INTRA *pFrame);
    virtual void HandleMMLLSTBoxVER(MSG_INTRA *pFrame);
    virtual void HandleMMLSetBoxLGT(MSG_INTRA *pFrame);
private:
    BOOL m_bRecvResetSuc;  //是否已经接受了重起成功命令
};

class CDisconnectState : public CState
{
public:
    virtual BOOL Init(){
        return TRUE;
    };
    virtual void HandleAlarmCmdFromBox(unsigned char* pAlarmBoxFrame, int nLen);        
    CDisconnectState(CAlarmBox *pB):CState(pB){};
protected:
    virtual void HandleMMLRSTALMBox(MSG_INTRA *pFrame);
    virtual void HandleMMLSTPBoxSND(MSG_INTRA *pFrame);
    virtual void HandleMMLCLRBoxLGT(MSG_INTRA *pFrame);
    virtual void HandleMMLLSTBoxVER(MSG_INTRA *pFrame);
    virtual void HandleMMLSetBoxLGT(MSG_INTRA *pFrame);

};

class CNormalState : public CState
{
public:
    virtual void HandleAlarmCmdFromBox(unsigned char* pAlarmBoxFrame, int nLen);     
    virtual BOOL HandleAlarm(SActiveAlarm *pFrame);
    virtual void OnTimer();
    virtual BOOL Init();
    CNormalState(CAlarmBox *pB):CState(pB){};
protected:
    virtual void HandleMMLAlarmAckEVT(MSG_INTRA *pFrame);
    virtual void HandleMMLAlarmACK(MSG_INTRA *pFrame);
    virtual void HandleMMLRSTALMBox(MSG_INTRA *pFrame);
    virtual void HandleMMLSTPBoxSND(MSG_INTRA *pFrame);
    virtual void HandleMMLCLRBoxLGT(MSG_INTRA *pFrame);
    virtual void HandleMMLLSTBoxVER(MSG_INTRA *pFrame);
    virtual void HandleMMLSetBoxLGT(MSG_INTRA *pFrame);
};
#endif
