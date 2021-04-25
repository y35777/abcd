/**********************************************************************
 *��Ʒ��    ��iGateway Bill V200        
 *ģ����    ��agent
 *�ļ���    ��ClusterAgent.h    
 *������Ϣ  �����ļ�����CClusterAgent�Ķ����ļ���
 *�汾˵��  ��V200R002i00M00B00D1130A   
 *��Ȩ��Ϣ  ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����      ������ƽ
 *����ʱ��  ��2001-10-30
 *�޸ļ�¼  ��
 **********************************************************************/

#ifndef _CLUSTER_AGENT_H_
#define _CLUSTER_AGENT_H_

#include "agent_hdr.h"
#include "../include/frame.h"
#include "cluster.h"
#include "cluster2.h"
#include "cluster3.h"

class CCluster;
class CCluster2;
class CCluster3;
class CClusterAlarm;

class CClusterAgent : public CWorkModule
{
public:
    CClusterAgent(BOOL bIsSingle);        //���캯��
    ~CClusterAgent(void);                 //��������
    
protected:
    int  InitInstance(void);            //�̳��ڿ�ܵĳ�ʼ������
    MODULE_TYPE GetModuleType();          //�̳��ڿ�ܵ����⺯��
    
    BOOL StartKernel(void);               //����KERNEL����
    BOOL CheckKernel(void);               //���KERNEL����
    BOOL StopKernel(void);                //�ر�KERNEL����
    void KillKernel(void);                //ǿ��ɱ��KERNEL����
    
    void DoAnsActive(BOOL bResponse);     //����˫������Ӧ��
    int  DoManualOver(MSG_INTRA* pMsg);   //�����ֹ�����
    void DoReset(MSG_INTRA* pMsgParsed);  //�����ֹ�����


    void DoAskFailover(EFAILOVERCAUSE_TYPE eCause);  //����������

    BOOL Switch(BOOL bCheckSanity);       //������鼰�л�
    
    void DoReqStatus(void);               //˫��״̬��ѯ
    
    void ParseMsg(MSG_INTRA* pMsg);       //��Ϣ����
    void OnTimer(const long nTimerID);    //��ʱ������
    
    //ϵͳ�źŴ���
    int  handle_signal(int signum, siginfo_t* st, ucontext_t* ut);

    //by ldf ��Ӧ���ⵥD11862 2002-3-16
    void proc_signal(int signum);
    
private:
    CCluster*       m_ptheCluster;        //˫���������
    CClusterAlarm*  m_ptheClusterAlarm;   //˫���澯���� 
    BOOL            m_bIsSingle;          //������˫��ָʾ
    pid_t           m_nPidKnl;             //KERNEL���̵�PID

    //���³�Ա����Ϊ��ʱ����������λ��Ϊ��
    UINT4           m_uAlarmInterval;     //�澯�����(��λ����)
    UINT4           m_uKnlActTimeout;     //�ȴ�KERNEL���̼���Ӧ��ʱ
    UINT4           m_uKnlOffTimeout;     //�ȴ�KERNEL���̹رճ�ʱ
    UINT4           m_uKnlOffInterval;    //�ȴ�KERNEL���̹ر�ʱ����
    UINT4           m_uKnlOffCount;       //�ȴ�KERNEL���̹رն�ʱ�ۼ�
    UINT4           m_uCheckKnlInterval;  //���KERNEL�����Ƿ�������ʱ���� 
   
    //���³�Ա����Ϊ��ʱ��ID
    long            m_nAlarmIntervalID;   //�澯��ⶨʱ��ID
    long            m_nKnlActTimeoutID;   //�ȴ�KERNEL����Ӧ��ʱ��ID
    long            m_nKnlOffIntervalID;  //���KERNEL�رն�ʱ��ID
    long            m_nCheckKnlID;        //���KERNEL���̵Ķ�ʱ��ID
    long            m_nSendHeartBeatID;   //�����PT_OM��������Ϣ�Ķ�ʱ��ID

    //by ldf ��Ӧ���ⵥD11862 2002-3-16
    long            m_nTimerIDBase;       //����źŵĶ�ʱ��ID
    int             m_nRecvSignal;        //�յ����ź�

    STRING          m_sSCVer;             //SC����汾��
};

#endif  //_CLUSTER_AGENT_H_
