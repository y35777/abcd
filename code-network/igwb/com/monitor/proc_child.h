/***************************************************************************
 *��Ʒ��        ��iGateway Bill V200
 *ģ����        ��monitor
 *�ļ���        ��proc_child.h
 *������Ϣ      ����CProcessChild�Ķ��塣���������һ�����̶��������������
                  ����ά�����̵ȡ�
 *�汾˵��      ��V200R001i00M00B00D1130A
 *��Ȩ��Ϣ      ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����          ������ƽ
 *����ʱ��      ��2001-10-30
 *�޸ļ�¼      ��
*****************************************************************************/

#ifndef _PROCESS_CHILD_H_
#define _PROCESS_CHILD_H_

#include "monitor_hdr.h"

class CProcessChild
{
public:
    CProcessChild(SMappedAppInfo *pAppInfo);
    ~CProcessChild();

    EAPPSTATUS GetStatus();                 //���ر����̵�״̬
    void SetStatus(EAPPSTATUS nStatus);     //���ñ����̵�״̬
    time_t GetLastStartTime();              //�����ϴ�������ʱ��
    PROCESS_TYPE GetProcessType();          //���ؽ��̱��
    pid_t GetPid();                         //���ؽ���PID
    EAUXUPGSTATUS GetAuxUpgStatus();        //���ظ���������״̬
    void SetAuxUpgStatus(EAUXUPGSTATUS nStatus);    //���ø���������״̬
    int Active();                           //�������
    BOOL CanRestart();                      //�����Ƿ��ܽ��б��ػָ�
    void Stop();                            //�رձ�����
    int Start(ACE_Process_Manager *pMgr);   //����������
    const char *GetAppName();               //���ر�������
	int GetLeftStartDelay(int nDiv);        //ȡ��ǰʣ��������ӳ�ʱ��,nDivΪÿ�μ��ļ��ʱ��(s)

    inline BOOL GetbBaseProc(void)
    {
        return m_bBaseProc;
    }

    inline void EnableBaseProc(void)
    {
        m_bBaseProc = TRUE;
    }

    inline void DisableBaseProc(void)
    {
        m_bBaseProc = FALSE;
    }

private:
    EAPPSTATUS          m_nStatus;          //��ҵ����̵�״̬
    EAUXUPGSTATUS       m_nAuxUpgStatus;    //��ҵ����̵ĸ������������Ƿ����
    SMappedAppInfo      *m_pInfo;           //ҵ�������Ϣ  
    pid_t               m_PID;              //��ҵ����̵Ľ���ID
    UINT2               m_uRestartCount;    //��������������������������������򵹻�
    time_t              m_uLastStartTime;   //�ϴ������ľ���ʱ��
    ACE_Process_Options m_POptions;         //ҵ����̵�ACE����
	int                 m_nStartDelay;      //������״̬ΪAPP_SCHEDULED_STARTʱ���ӳ�������ʱ��(s)
    BOOL                m_bBaseProc;        //�Ƿ��ǻ������̣���Ҫ�Ƚ����ˣ�
};


#endif //_PROCESS_CHILD_H_

