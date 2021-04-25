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
���¶�����澯��ͨѶ��ص������ָ��ݸ澯��ͨѶЭ��
************************************************************************************/
#define M_ALM_ALARM_BOX_RESET             0x02  /*��λ�澯�������� */
#define M_ALM_ALARM_BOX_STOP_SOUND        0x07  /*ͣ�澯�������� */
#define M_ALM_ALARM_BOX_CONFIG            0x08  /*���ø澯�������� */
#define M_ALM_ALARM_BOX_RESPONSE_ONLINE   0x28  /*����Ӧ��֡������ */
#define M_ALM_ALARM_BOX_DOWN_ALARM        0x50  /*�·��澯��Ϣ������ */
#define M_ALM_ALARM_BOX_REDOWN_ALARM      0x51  /*�ط��澯��Ϣ������ */
#define M_ALM_ALARM_BOX_QUERY_VERSION     0x52  /*��ѯ��ʾ�汾������ */
//*����Ϊ�澯����Ӧ֡������*/
#define M_ALM_ALARM_BOX_RESET_SUCCESS     0x14  /*��λ�ɹ�״̬������ */
#define M_ALM_ALARM_BOX_RESEND_ALARM      0x18  /*Ҫ���ط��澯������ */

#define M_ALM_MAX_ALARM_BOX_COMMAND_LEN   100   /*���͵��澯��������ַ�������󳤶� */
#define M_ALM_DEFAULT_ALARM_BOX_DATA_LEN  512   /*����澯�䶨ʱͨѶʱ�����ܻ�õ�������ݳ��� */
/***********************************************************************************
���¶�����澯����ͨѶЭ����صĲ���Э��ؼ���ֵ
***********************************************************************************/
#define M_ALM_BOX_PROTOCOL_FS             0x7e  /*������澯��ͨѶ��Э��ͷ��־ */
/* �������澯�� */
#define M_ALM_BOX_PROTOCOL_SEND_TA        0x1f  /*���������·�����ʱ��������ϢͷTA���� */
#define M_ALM_BOX_PROTOCOL_SEND_SA        0x00  /*���������·�����ʱ��������Ϣ����SA���� */

/* �澯����Ӧ */
#define M_ALM_BOX_PROTOCOL_RESPONSE_TA    0x00  /* ����澯����Ӧʱʱ��������ϢͷTA���� */
#define M_ALM_BOX_PROTOCOL_RESPONSE_SA    0x10  /* ����澯����Ӧʱʱ��������Ϣ����SA���� */
typedef MAP<BYTE,UINT4> AlarmCountMap;
typedef LIST<UINT4> CsnList;
struct SAlarmInfo{
    UINT4   uCsn;       //�澯��ˮ��
    BYTE    yModuleNO;  //���͸澯������ģ���,����iGateway bill,��Ϊ0,��Ϊ1
    UINT2   uAlarmID;   //�澯���
    BYTE    yAlarmType; //�澯����
    BYTE    yFuncID;    //�澯��澯����
    BYTE    yAlarmLevel;//�澯����
    BYTE    ySubSysID;  //���ܸ澯����
    //�澯����������,ʱ��
    UINT2   uYear;      //4λ��
    BYTE    yMonth;     //2λ��
    BYTE    yDay;       //2λ��
    BYTE    yHour;      //2λʱ
    BYTE    yMin;       //2λ��
    BYTE    ySec;       //2λ��
    BYTE    yCPUPos;    //A(0) B(1)��ָʾ,����iGateway bill,��Ϊ0,��Ϊ1
    BYTE    yPID;       //�澯�����Ľ��̱�ʶ
    BYTE    yPara[MAX_ALARM_PARA];  //�澯����
    BOOL    bACK;       //�Ƿ�Ҫ����ͣ�澯������
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
    CAlarmConfig  *m_pCfg;              //�澯���ò�ѯ
public:
    CState        *m_pState;            //״ָ̬��
    CMMLReport    *m_pReport;           //MML���Ĵ�����
    AlarmCountMap m_AlarmCount;         //STD::MAP  ���ֹ��ϸ澯�ļ������
    unsigned char m_szAlarmBoxVersion[64];  //�澯��汾
    int           m_nCnt;               //δ���ֵ�ʱ��
    SAlarmInfo    m_sRecentAlarm;       //���һ�θ澯��Ϣ
    CsnList       m_EvtCsnList;         //�����������ϴ��˹�����澯���������������н����¼��澯����ˮ�Ŷ���
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
    virtual void HandleMMLAlarmAckEVT(MSG_INTRA *pFrame); //�����¼��澯ȷ��
    virtual void HandleMMLAlarmACK(MSG_INTRA *pFrame);
    virtual void HandleMMLRSTALMBox(MSG_INTRA *pFrame) = 0;
    virtual void HandleMMLSTPBoxSND(MSG_INTRA *pFrame) = 0;
    virtual void HandleMMLCLRBoxLGT(MSG_INTRA *pFrame) = 0;
    virtual void HandleMMLLSTBoxVER(MSG_INTRA *pFrame) = 0;
    virtual void HandleMMLSetBoxLGT(MSG_INTRA *pFrame) = 0;
    CAlarmBox *m_pParent; //ָ��ӵ�д�״̬�ϲ����ָ��
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
    BOOL m_bRecvResetSuc;  //�Ƿ��Ѿ�����������ɹ�����
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
