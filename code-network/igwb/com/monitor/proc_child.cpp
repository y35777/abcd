/***************************************************************************
 *��Ʒ��        ��iGateway Bill V200
 *ģ����        ��monitor
 *�ļ���        ��proc_child.cpp
 *������Ϣ      ����CProcessChild��ʵ�֡����������һ�����̶��������������
                  ����ά�����̵ȡ�
 *�汾˵��      ��V200R001i00M00B00D1130A
 *��Ȩ��Ϣ      ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����          ������ƽ
 *����ʱ��      ��2001-10-30
 *�޸ļ�¼      ��
***************************************************************************/

#include "proc_child.h"
#include "monitor.h"

/**************************************************************************
����ԭ��	public: CProcessChild(SMappedAppInfo* pAppInfo)
��������	���캯��
�������	ҵ�������Ϣ����ָ�롣
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
***************************************************************************/
CProcessChild:: CProcessChild(SMappedAppInfo *pAppInfo)
{
    //ҵ�������Ϣ�ṹ��ָ�븳����Ʒ����m_pInfo�С�
    m_pInfo = pAppInfo;

    //��ʼ����Ա����
    m_nStatus = APP_STOPPED;
    m_PID     = 0;
    m_uRestartCount  = 0;
    m_uLastStartTime = 0;
    m_bBaseProc      = FALSE;

    //��ʼ�������в���������ҵ����̱����Ϊ�����в���
    if ((pAppInfo->nPType >= PT_CO_BASE) && (pAppInfo->nPType <= MAX_PT_CO_TYPE))
    {
        m_POptions.command_line("%s %d", pAppInfo->szPathName, 
                                pAppInfo->nPType - PT_CO_BASE + 1);
    }
    else if ((pAppInfo->nPType >= PT_AP_BASE) && (pAppInfo->nPType <= MAX_PT_AP_TYPE))
    {
        m_POptions.command_line("%s %d", pAppInfo->szPathName, 
                                pAppInfo->nPType - PT_AP_BASE + 1);
    }
    else
    {
        m_POptions.command_line("%s", pAppInfo->szPathName);
    }
}

/****************************************************************************
����ԭ��	public: ~CProcessChild()
��������	��������
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
*****************************************************************************/
CProcessChild:: ~CProcessChild()
{
    if (m_pInfo) 
    {
        delete m_pInfo;
        m_pInfo = NULL;
    }
}

/*****************************************************************************
����ԭ��	public: EAPPSTATUS GetStatus();
��������	���ظý���״̬
�������	�ޡ�
�������	�ޡ�
����ֵ  	���ظý��̵�״ֵ̬��
�׳��쳣	�ޡ�
******************************************************************************/
EAPPSTATUS CProcessChild:: GetStatus()
{
    return m_nStatus;
}

/***************************************************************************
����ԭ��	public: void SetStatus(EAPPSTATUS nStatus)
��������	���øý��̵�״̬
�������	���̵�״ֵ̬��
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
****************************************************************************/
void CProcessChild:: SetStatus(EAPPSTATUS nStatus)
{
    m_nStatus = nStatus;
}

/***************************************************************************
����ԭ��	public: time_t GetLastStartTime()
��������	�������ɹ�������ҵ����̵ľ���ʱ��
�������	�ޡ�
�������	�ޡ�
����ֵ  	��������ɹ�������ҵ����̵ľ���ʱ�䡣
�׳��쳣	�ޡ�
****************************************************************************/
time_t CProcessChild:: GetLastStartTime()
{
    return m_uLastStartTime;
}

/***************************************************************************
����ԭ��	public: PROCESS_TYPE GetProcessType()
��������	��ø�ҵ����̱��
�������	�ޡ�
�������	�ޡ�
����ֵ  	���ظ�ҵ����̱�š�
�׳��쳣	�ޡ�
****************************************************************************/
PROCESS_TYPE CProcessChild:: GetProcessType()
{
    return m_pInfo->nPType;
}

/***************************************************************************
����ԭ��	public: pid_t GetPid()
��������	��ø�ҵ����̵Ľ���ID �š�
�������	�ޡ�
�������	�ޡ�
����ֵ  	���ظ�ҵ����̵Ľ���ID �š�
�׳��쳣	�ޡ�
****************************************************************************/
pid_t CProcessChild:: GetPid()
{
    return m_PID;
}

/****************************************************************************
����ԭ��	public: EAUXUPGSTATUS GetAuxUpgStatus()
��������	��ø�ҵ����̵ĸ�������״̬
�������	�ޡ�
�������	�ޡ�
����ֵ  	���ظ�ҵ����̵ĸ�������״̬��
�׳��쳣	�ޡ�
*****************************************************************************/
EAUXUPGSTATUS CProcessChild:: GetAuxUpgStatus()
{
    return m_nAuxUpgStatus;
}

/****************************************************************************
����ԭ��	public: void SetAuxUpgStatus(EAUXUPGSTATUS nStatus)
��������	���ø�ҵ����̵ĸ������������Ƿ���ɵ�״̬
�������	��ҵ����̵ĸ�������״̬��
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
*****************************************************************************/
void CProcessChild:: SetAuxUpgStatus(EAUXUPGSTATUS nStatus)
{
    m_nAuxUpgStatus = nStatus;
}

/*****************************************************************************
����ԭ��	public: int Active()
��������	�����ҵ�����
�������	�ޡ�
�������	�ޡ�
����ֵ  	�����ҵ������Ƿ�ɹ�������ɹ�����ERR_SUCCESS�����򷵻�
                ERR_FAIL��
�׳��쳣	�ޡ�
******************************************************************************/
int CProcessChild:: Active()
{
    switch (m_nStatus)
    {
        //�Ѿ�����
        case APP_ACTIVE:
            return ERR_SUCCESS;

        case APP_READY:
        case APP_ACTIVING:
            //ͨ�����ģ�������ģ�鷢�ͼ�������
            CMonitor::SendMsgToManager(m_pInfo->nPType, CMD_ACTIVE);
            m_nStatus = APP_ACTIVING;
            return ERR_SUCCESS;

        default:
            return ERR_FAIL;
    }
}

/********************************************************************************
����ԭ��	public: BOOL CanRestart()
��������	��ҵ�������֧�ֱ��ػָ�
�������	�ޡ�
�������	�ޡ�
����ֵ  	���ظ�ҵ�������֧�ֱ��ػָ���
�׳��쳣	�ޡ�
********************************************************************************/
BOOL CProcessChild:: CanRestart()
{
    //�����̲�֧�������ָ�
    if (!m_pInfo->bCanRestart)
    {
        return FALSE;
    }
 
    time_t TimeNow;
    time(&TimeNow);
   
    //���������һ������ʱ�䳬��5���ӣ�����Ϊ��������
    if ((TimeNow - m_uLastStartTime) > FIVE_MINUTE_IN_SECOND)
    {
        return TRUE;
    }

    //����������С��2�Σ���ֱ������
    if (m_uRestartCount < 2)
    {
        return TRUE;
    }

	//��������������2�Σ����ӳ�250��������
    if (m_uRestartCount == 2)
    {
        m_nStartDelay = 250;
        m_nStatus = APP_SCHEDULED_START;
        return TRUE;
    } 
    
    return FALSE;  
}

/********************************************************************************
����ԭ��	public: void  Stop()
��������	ǿ�йر�ָ����ҵ�����
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
*********************************************************************************/
void CProcessChild:: Stop()
{
    if (APP_STOPPED == m_nStatus)
    {
        return;
    }
    
    m_nStatus = APP_STOP_PENDING;
    
    //����ACE�ľ�̬������ǿ��ɱ��ָ������
    ACE::terminate_process(m_PID);
   
    m_nStatus = APP_STOPPED;
}

/*******************************************************************************
����ԭ��	public: int Start(ACE_Process_Manager *pMgr)
��������	������ҵ�����
�������	�ޡ�
�������	�ޡ�
����ֵ  	������ҵ������Ƿ�ɹ�������ɹ�����ERR_SUCCESS�����򷵻�ERR_FAIL��
�׳��쳣	�ޡ�
********************************************************************************/
int CProcessChild:: Start(ACE_Process_Manager* pMgr)
{
    if (pMgr == NULL)
    {
        m_pInfo->bCanRestart = FALSE;
        return ERR_FAIL;
    }

    //��Ϊ��UNIX�£�����ӽ����ļ������ڣ�SPAWN���ǻ᷵�سɹ���
    //��CMonitor�����ӽ���ǰ�ȼ���ļ��Ƿ����
    if(ACE_OS::access(m_pInfo->szPathName, F_OK) != 0)
    {
        m_pInfo->bCanRestart = FALSE;
        TRACE(MTS_MONITOR, SZ_KNL_PROC_NOT_EXIST, m_pInfo->szPathName);
        return ERR_FAIL;
    }

    m_PID = pMgr->spawn(m_POptions);
    
    //����PIDΪ-1��ʾ��������ʧ��
    if (-1 == m_PID) 
    {
        m_pInfo->bCanRestart = FALSE;
        return ERR_FAIL;
    }

    m_nStatus = APP_START_PENDING;
   
    time_t TimeNow;
    time(&TimeNow);

    if (m_uLastStartTime > 0)
    {
        //��������ϴ�����ʱ����5�����ڣ�����Ϊ�Ƕ����������������������
        //�ۼ�
        if (((long)TimeNow - m_uLastStartTime) <= FIVE_MINUTE_IN_SECOND)
        {
            m_uRestartCount++;
        }
        else
        {
            m_uRestartCount = 1;
        }
    } 
 
    m_uLastStartTime = TimeNow;
    
    return ERR_SUCCESS;
}

/***********************************************************************************
����ԭ��	public: char * GetAppName()
��������	��ȡ��ҵ����̵�����
�������	�ޡ�
�������	�ޡ�
����ֵ  	���ظ�ҵ����̵����ơ�
�׳��쳣	�ޡ�
************************************************************************************/
const char* CProcessChild:: GetAppName()
{
    return m_pInfo->szAppName;
}

int CProcessChild:: GetLeftStartDelay(int nDiv)
{
    m_nStartDelay -= nDiv;
    return m_nStartDelay;
}
