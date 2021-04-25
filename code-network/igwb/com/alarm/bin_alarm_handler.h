//  ���ߣ�������  ��Ȩ����Ϊ��˾
#ifndef _BIN_ALARM_HANDLER
#define _BIN_ALARM_HANDLER

#include "alarm_handler.h"

//����ʵ�ֶ����Ƹ澯��һ����������Ը�����1����������ĸ澯
class CBinAlarmHandler : public CAlarmHandler
{
public:

    CBinAlarmHandler(BYTE yAccessPointID, UINT2 uBaseAlarmID,
                     const char* szAlarmShield, int nProductType,
                     int nBillsvrNo, CAlarmServer* pAlarmServer,
                     const char* AccessPointName);    
    virtual ~CBinAlarmHandler(){};
    //��ʼ������
    //virtual int Init(const char* szCfgPath);

    //���������ģ�鷢�͹����ķ�MML��Ϣ
    virtual void HandleFrameFromApp(MSG_INTRA *pFrame);

    //����澯�����м���һ���µĸ澯
    virtual ALARM_ADD_RESULT AddAlarm(SInnerAlarm* pNewAlarm, SActiveAlarm** ppActiveAlarm, int nRecoverType = NOT_RECOVER);

    //����SActiveAlarm�ṹ���Ͷ����Ƹ澯
    virtual void SendAlarm(SActiveAlarm* pAlarm) = 0;

    //virtual void HandleFrameFromMML(MSG_INTRA *pFrame);    

    //�����յ��������Ķ����Ƹ澯Ӧ����Ϣ�����еõ���Ӧ�ĸ澯��Ϣ
    virtual BOOL ParseAlarmResponse(MSG_INTRA* pMsg,SActiveAlarm *&pActive) = 0;
protected:
    //��SActiveAlarm�ṹ�л�ȡ���ڷ��������ĸ澯ID
    virtual UINT2 GetOuterAlarmId(SActiveAlarm* pAlarm);

    //��������Ӧ���еĸ澯ID������ָ��SActiveAlarm�ṹ�еĸ澯ID���澯����
    virtual void SetInnerAlarmId(SActiveAlarm* pAlarm, UINT2 uAlarmId);
    BYTE  m_yAPID;              //�������Ӧ�Ľ�����
    BOOL  m_bProcAllAPAlarm;    //�Ƿ������н����ĸ澯
    int   m_nAPType;            //�������Ĳ�Ʒ����
    UINT2 m_uBaseAlarmID;       //�������ĸ澯ID����
};


class CSoftXBinAlarmHandler : public CBinAlarmHandler
{
public:
    CSoftXBinAlarmHandler(BYTE yAccessPointID, UINT2 uBaseAlarmID,
                          const char* szAlarmShield, int nProductType,
                          int nBillsvrNo, CAlarmServer* pAlarmServer,
                          const char* AccessPointName);
    
    virtual ~CSoftXBinAlarmHandler(){};
    //��ʱ�Ӵ�����
    virtual void Timer1Sec();
    //�����յ��������澯Ӧ��
    virtual void AlarmReceived(MSG_INTRA* pFrame);
    //����澯�����м���һ���µĸ澯
    virtual void SendAlarm(SActiveAlarm* pAlarm);
    //�����յ��������Ķ����Ƹ澯Ӧ����Ϣ�����еõ���Ӧ�ĸ澯��Ϣ
    virtual BOOL ParseAlarmResponse(MSG_INTRA* pMsg,SActiveAlarm *&pActive);
    unsigned char m_nLastSendHandle;   //��һ�η��͵�Handleֵ
protected:
    UINT2 m_nAlarmTimerCnt;  //�澯���͵ĳ�ʱ����
    UINT2 m_nTimeOut;        //���͸澯�ĳ�ʱֵ
};


class CMSCBinAlarmHandler : public CBinAlarmHandler
{
public:
    CMSCBinAlarmHandler(BYTE yAccessPointID, UINT2 uBaseAlarmID,
                        const char* szAlarmShield, int nProductType,
                        int nBillsvrNo, CAlarmServer* pAlarmServer,
                        const char* AccessPointName);
    
    virtual ~CMSCBinAlarmHandler(){};
    //��ʱ�Ӵ�����
    virtual void Timer1Sec();
    //�����յ��������澯Ӧ��
    virtual void AlarmReceived(MSG_INTRA* pFrame);
    //����SActiveAlarm�ṹ���Ͷ����Ƹ澯
    virtual void SendAlarm(SActiveAlarm* pAlarm);
    //�����յ��������Ķ����Ƹ澯Ӧ����Ϣ�����еõ���Ӧ�ĸ澯��Ϣ
    virtual BOOL ParseAlarmResponse(MSG_INTRA* pMsg,SActiveAlarm *&pActive);
};
//���������
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
        if (0 == pExplain->yParaNum)//������ 2002-05-10
        {
            //InnerAlarm.yAlarmPara = 0xFF; //0xFF��ʾ��Ч�ĸ澯����
            nResult = m_pHandler->AddAlarm(&InnerAlarm, &pActiveAlarm, RESET_RECOVER);
            if (ADD_OK != nResult) //������ɾ��pActiveAlarm == NULL �ж�
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
                if (ADD_OK != nResult) //������ɾ��pActiveAlarm == NULL �ж�
                {
                    TRACE(MTS_ALARM,S_ALARM_INITADD_FAIL,InnerAlarm.uAlarmID,InnerAlarm.yAlarmType,InnerAlarm.yAlarmPara);
                }
            }//for
        }//if
        return true;
    }
};  //����ķ���������
#endif
