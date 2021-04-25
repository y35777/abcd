/***************************************************************************
 *��Ʒ��   ��iGateway Bill V200
 *ģ����   ��monitor
 *�ļ���   ��proc_mgr.h
 *������Ϣ ����CProcessManager�Ķ��塣
 *�汾˵�� ��V200R001i00M00B00D1130A
 *��Ȩ��Ϣ ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����     ������ƽ
 *����ʱ�� ��2001-10-30
 *�޸ļ�¼ ��
*****************************************************************************/

#ifndef _PROCESS_MANAGER_H_
#define _PROCESS_MANAGER_H_

#include "monitor_hdr.h"
#include "proc_child.h"

#define TIME_OF_PROC_INIT       60
#define TIME_OF_PROC_ACTIVE     60
#define TIME_OF_PROC_DEACTIVE   120

class CProcessChild;

class CProcessManager
{
public:
    CProcessManager();
    ~CProcessManager();

    int InitInstance();

    void ProcInitAns(PROCESS_TYPE nPType);      //�����ʼ��Ӧ��
    void ProcActiveAns(PROCESS_TYPE nPType);    //������Ӧ��
    int Start();                                //�������н���
    int Active();                               //�������н���
    void DeActive();                            //ȥ�������н���
    void Timer5Sec();
    EPROCMGRSTATUS GetStatus();                 //���ر������״̬
    void ProcAuxUpgradeCmd();                   //��������������
    void ProcAuxUpgradeAns(PROCESS_TYPE nPType);//����������Ӧ��
    EAUXUPGSTATUS GetAuxUpgStatus();            //���ص�ǰ����������״̬
    //by ldf 2002.4.8 ��Ӧ���ⵥ D12433
    void AuxUpgFailSetStatus();                 //��������ʧ�ܻ���AUXUPG_STOPPED״̬
    //�޸����
    void ProduceCDR();   

protected:
    int CreateAllProcessChild();                //���������ӽ��̶���
    int Add(CProcessChild *pChild);             //���ָ�����̶���
    void ProcFailedRecovery(PROCESS_TYPE nPType);//���б��ػָ�ʧ�ܵĴ���
    CProcessChild *LookUp(PROCESS_TYPE nPType); //ͨ������Ľ������Ͳ��ҽ��̶���
    CProcessChild *LookUp(pid_t nPid);          //ͨ�������PID���ҽ��̶���
    int Start(PROCESS_TYPE nPType);             //����ָ������
    void Stop(PROCESS_TYPE nPType);             //�ر�ָ������
    void Stop();                                //�ر����н���
    int StartWatchDog();                        //�����ػ��߳�

    static void ProcWatchDog(CProcessManager* pMgr);    
    void WatchDog();
    int ProcRecovery(PROCESS_TYPE nPType);      //���б��ػָ�
    EAPPSTATUS GetStatus(PROCESS_TYPE nPType);  //����ָ�����̱�ŵ�״̬
    int SetStatus(PROCESS_TYPE nPType,          //����ָ����̵�״̬
                  EAPPSTATUS nStatus);
    pid_t WaitExitPid(UINT4 uSec);              //�ȴ������ӽ���ֱ����ʱ������
                                                //�˳��Ľ���PID
    int Wait(UINT4 uSec);                       //�ȴ������ӽ���ֱ����ʱ������
                                                //��ǰδ�˳��Ľ�����
    int WaitPid(pid_t nPID, int nSec);          //�ȴ�ָ�������˳���nSec��ʱ
public:

	typedef LIST<CProcessChild *> LISTPROCCHILD;
    LISTPROCCHILD       m_ProcChild;        //ҵ����̶���ָ��list                                                

private:
    ACE_Process_Manager m_ProcMgr;          //ҵ����̵�ACE������
    ACE_Thread_Manager  m_ThdMgr;           //�ػ��̵߳��̹߳������

    BOOL                m_bExitFlag;        //ֻ���ر��ػ��߳�ʱ����TRUE�ı�־
    UINT2               m_uInitTimeOut;     //ҵ����̳�ʼ����ʱʱ���ʱ��,��λΪ��
    UINT2               m_uActiveTimeOut;   //ҵ����̼��ʱʱ���ʱ��,��λΪ��
    EAUXUPGSTATUS       m_nAuxUpdStatus;    //��������״̬
    EPROCMGRSTATUS      m_nStatus;          //ҵ����̹����߶���ǰ״̬
};


#endif //_PROCESS_MANAGER_H_
