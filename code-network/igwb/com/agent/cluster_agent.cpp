/**************************************************************************
 *��Ʒ��    ��iGateway Bill V200        
 *ģ����    ��agent
 *�ļ���    ��cluster_agent.cpp
 *������Ϣ  �����ļ�����CClusterAgent��ʵ�֡�
 *�汾˵��  ��V200R002i00M00B00D1130A   
 *��Ȩ��Ϣ  ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����      ������ƽ
 *����ʱ��  ��2001-10-30
 *�޸ļ�¼  ��
 **************************************************************************/

#include "cluster_agent.h"
#include "cluster.h"
#include "cluster2.h"
#include "cluster3.h"
#include "cluster_alarm.h"
//by ldf ��Ӧ���ⵥD11862 2002-3-16
#include "../include/signal_tab.h"
#ifdef _PLATFORM_SOLARIS 
#include "../include/system1.h"
#endif
#include "../utility/mml_para.h"
#include "../utility/mml_report.h"

/**************************************************************************
����ԭ��    public: CClusterAgent(BOOL bIsSingle);
��������    CClusterAgent�๹�캯��
�������    bIsSingle������˫��������־������������Ϊ�档
�������    ��
����ֵ          ��
�׳��쳣    ��
***************************************************************************/
CClusterAgent:: CClusterAgent(BOOL bIsSingle)
{
    m_ptheCluster      = NULL;
    m_ptheClusterAlarm = NULL;
    m_bIsSingle        = bIsSingle;
    m_nPidKnl          = -1;
    //by ldf ��Ӧ���ⵥD11862 2002-3-16
    m_nRecvSignal      = 0;

}

/*************************************************************************
����ԭ��    public: ~CClusterAgent();
��������    �����������������ж�������ǰ����������
�������    ��
�������    ��
����ֵ          ��
�׳��쳣    ��
**************************************************************************/
CClusterAgent:: ~CClusterAgent()
{
    //����˫���澯����
    if (m_ptheClusterAlarm)
    {
        delete m_ptheClusterAlarm;
    }
    
    //����˫������ 
    if (m_ptheCluster)
    {
        delete m_ptheCluster;
    }
}

/*************************************************************************
����ԭ��    protected:  int InitInstance(void);
��������    ����������ĳ�ʼ���������̳и�����麯����
�������    ��
�������    ��
����ֵ          ��ʼ���Ƿ�ɹ���
�׳��쳣    ��
**************************************************************************/
int CClusterAgent:: InitInstance(void)
{
    //���ø���ĳ�ʼ��������ע��·��
    CWorkModule::InitInstance();    

    //�ڵ��������£�����Ҫ����m_ptheCluster����
    //ע��m_ptheCluster����Ӧ����m_ptheClusterAlarm���󴴽�
    if (FALSE == m_bIsSingle)
    {
        CINIFile* pINIFile = new CINIFile(GetCfgFilePath());
        pINIFile->Open();     //�򿪲��������ļ�billsvr.ini
        
        char szClusterVer[255] = "";
        pINIFile->GetString(CFG_SEC_CLUSTER, CFG_CLUSTER_VER, DEFAULT_CLUSTER_VER, szClusterVer, 254);          
        m_sSCVer = szClusterVer;
        
        delete pINIFile;  //CINIFile�������������л�ر�INI�����ļ����    
        
        if ( strcmp(m_sSCVer.c_str(),"2.2") == 0 )
            //GetCfgFilePath()��CClusterAgent��ĸ����ṩ��
            //��������ļ�ȫ·�����ľ�̬����
        {
            m_ptheCluster = new CCluster2(GetCfgFilePath());
        }
        else
        {
            m_ptheCluster = new CCluster3(GetCfgFilePath());
        }
    }
    
    //����˫���澯�������
    m_ptheClusterAlarm = new CClusterAlarm(m_ptheCluster); 
    
    //�����ò���
    CINIFile* pINIFile = new CINIFile(GetCfgFilePath());
    if (FALSE == pINIFile->Open())
    {
        delete pINIFile;
        return ERR_OPEN_CFG_FILE;
    }
    
    //�澯��ⶨʱ�������ƹ涨Agentÿ��m_nSendHeartBeatID
    //ʱ������PT_OM����һ��������Ϣ
    m_nSendHeartBeatID = SetTimer(FIVE_SECOND_IN_MILISEC);
    //�澯��ⶨʱ�������ƹ涨Agentÿ��m_uAlarmInterval
    //ʱ�������һ���йصĸ澯��
    m_uAlarmInterval = pINIFile->GetInt(CFG_SEC_ALM_AGENT, 
                                        CFG_ALM_INTERVAL, DEFAULT_ALM_INTERVAL);
    //����λת���ɺ���
    m_uAlarmInterval *= 1000;
    //�涨���ʱ�����3���ӵ�30����֮��
    if (((3*60*1000) > m_uAlarmInterval) || ((30*60*1000) < m_uAlarmInterval)) 
    {
        m_uAlarmInterval = DEFAULT_ALM_INTERVAL * 1000;
    }
   
    //�ȴ�KERNEL����Ӧ��ʱ����ƹ涨��Agent�ɹ�����
    //KERNEL���̺������m_uKnlActTimeoutʱ����֮��û���յ�KERNEL
    //�ļ���Ӧ����ִ����Ӧ������
    m_uKnlActTimeout = pINIFile->GetInt(CFG_SEC_ALM_AGENT, 
                                        CFG_KNL_ACT_TIMEOUT, DEFAULT_KNL_ACT_TIMEOUT);
    //����λת���ɺ���
    m_uKnlActTimeout *= 1000;
    //�涨���ʱ�����1���ӵ�10����֮��
    if (((60*1000) > m_uKnlActTimeout) || ((10*60*1000) < m_uKnlActTimeout))
    {
        m_uKnlActTimeout = DEFAULT_KNL_ACT_TIMEOUT * 1000;
    }

    //�ȴ�KERNEL�ر�Ӧ��ʱ����ƹ涨��Agent�����ر�
    //KERNEL���̺��m_uKnlOffTimeoutʱ���������KERNEL��û����
    //������ǿ��ɱ��KERNEL���̡�
    m_uKnlOffTimeout = pINIFile->GetInt(CFG_SEC_ALM_AGENT, 
                                        CFG_KNL_OFF_TIMEOUT, DEFAULT_KNL_OFF_TIMEOUT);

    //����λת���ɺ���
    m_uKnlOffTimeout *= 1000;

    //���KERNEL�رն�ʱ������ƹ涨��Agent�����ر�KERNEL
    //���̵���Ϣ�󣬽�ÿ��m_uKnlOffInterval������һ��KERNEL�����Ƿ�
    //�����ڡ�
    m_uKnlOffInterval = pINIFile->GetInt(CFG_SEC_ALM_AGENT, 
                                         CFG_KNL_OFF_INTERVAL, DEFAULT_KNL_OFF_INTERVAL);
    //����λת���ɺ���
    m_uKnlOffInterval *= 1000;
    //�涨���ʱ�����10���ӵ�1����֮��
    if (((10*1000) > m_uKnlOffInterval) || ((60*1000) < m_uKnlOffInterval))
    {
        m_uKnlOffInterval = DEFAULT_KNL_OFF_INTERVAL * 1000;
    }

    //���KERNEL���̶�ʱ������ƹ涨AgentӦ��ʱ���KERNEL
    //�����Ƿ��������˶�ʱ����ʱ�������˹�������Ӱ�����ܡ�
    m_uCheckKnlInterval = pINIFile->GetInt(CFG_SEC_ALM_AGENT, 
                                           CFG_KNL_CHECK_INTERVAL, DEFAULT_KNL_CHECK_INTERVAL);
    //����λת���ɺ���
    m_uCheckKnlInterval *= 1000;
    //�涨���ʱ�����10���ӵ�1����֮��
    if (((10*1000) > m_uCheckKnlInterval) || ((60*1000) < m_uCheckKnlInterval))
    {
        m_uCheckKnlInterval = DEFAULT_KNL_CHECK_INTERVAL * 1000;
    }
    
    
    //�ȴ�KERNEL���̹ر��ۼƴ���
    m_uKnlOffCount = pINIFile->GetInt(CFG_SEC_ALM_AGENT, 
                                      CFG_KNL_OFF_COUNT, DEFAULT_KNL_OFF_COUNT); 
    //�涨��3-6��֮��
    if ((3 > m_uKnlOffCount) || (6 < m_uKnlOffCount))
    {
        m_uKnlOffCount = DEFAULT_KNL_OFF_COUNT;
    }
    
    delete pINIFile;

    //�������KERNELʧ�ܣ����𵹻� 
    if (StartKernel() == FALSE)
    {
        //��ӡ���������KERNELʧ�ܵ���Ϣ
        TRACE(THISMODULE, S_AGENT_START_KNL_FAILED);
            
        Switch(FALSE);

        return ERR_FAIL; 
    }
    else
    {
        //��ӡ���������KERNEL�ɹ�����Ϣ
        TRACE(THISMODULE, S_AGENT_START_KNL_SUCCESS);
        MSGOUT(THISMODULE, MSGOUT_LEVEL_NORMAL, 
               S_AGENT_START_KNL_SUCCESS); 
    }   

    //���ö�ʱ�����ȴ�KERNEL����Ӧ��
    m_nKnlActTimeoutID = SetTimer(m_uKnlActTimeout); 

    //���ö�ʱ������ʱ���KERNEL����
    m_nCheckKnlID = SetTimer(m_uCheckKnlInterval); 

    //by ldf ��Ӧ���ⵥD11862 2002-3-16
    m_nTimerIDBase = SetTimer(ONE_SECOND_IN_MILISEC);

    //��ACE_Reactor��register_handler�����ѱ�����ע����Reactor��
    //����SIGTERM�ź�
    reactor()->register_handler(SIGTERM, this);

    //ע��SIGCLD�źţ�ʵʱ����KERNEL�����˳���Ϣ
    reactor()->register_handler(SIGCLD, this);

    MSGOUT(THISMODULE, MSGOUT_LEVEL_NORMAL, 
               S_AGENT_AGENT_INIT_SUCCESS);

    return ERR_SUCCESS; 
}

/************************************************************************ 
����ԭ��    protected:  MODULE_TYPE GetModuleType(); 
��������    ���CMsgProcessor�����ģ�����ͣ��̳и�����麯���� 
�������    �� 
�������    �� 
����ֵ          �������ģ������
�׳��쳣    ��
*************************************************************************/
MODULE_TYPE CClusterAgent:: GetModuleType()
{
    return MT_CLSTR;
}

/*************************************************************************
����ԭ��    protected:  BOOL StartKernel(void);
��������    ����KERNEL���̣������ȴ�KERNEL���̷��صļ���Ӧ��
�������    ��
�������    ��
����ֵ          ���KERNEL���������ɹ��򷵻�TRUE�����KERNEL��������ʧ����
                ����FALSE��
�׳��쳣    ��
**************************************************************************/
BOOL CClusterAgent:: StartKernel(void)
{
    char szAppPath[MAX_PATH];

    //���Ӧ�ó�������Ŀ¼
    strncpy(szAppPath, GetAppPath(), MAX_PATH);
    szAppPath[MAX_PATH - 1] = '\0';

    //���·�������û�д�'/'�������'/'
    if (szAppPath[strlen(szAppPath) - 1] != '/')
    {
        strncat(szAppPath, "/", MAX_PATH);
    }
    szAppPath[MAX_PATH - 1] = '\0';

    strncat(szAppPath, S_APPNAME_KERNEL, MAX_PATH);
    szAppPath[MAX_PATH - 1] = '\0';
   
    //��鵱ǰĿ¼���Ƿ����KERNEL���̵Ŀ�ִ���ļ�
    if (access(szAppPath, F_OK) < 0)
    {
        TRACE(THISMODULE, S_AGENT_CANNOT_FINDFILE, szAppPath);
        return FALSE; 
    }

    if ((m_nPidKnl = vfork()) < 0)
    {
        return FALSE;
    }    
    else if (m_nPidKnl == 0)   //�ӽ���,Modified according to D14774
    {
        if (execl(szAppPath, szAppPath, NULL) < 0)
        {
            TRACE(THISMODULE, S_AGENT_EXECL_KNL_FAILED, szAppPath, errno);
            _exit(1);
        }
        
        return TRUE;
    }

    return TRUE;
}

/**************************************************************************
����ԭ��    protected:  BOOL CheckKernel(void);
��������    ���KERNEL�����Ƿ�����
�������    ��
�������    ��
����ֵ          ���KERNEL���������򷵻�TRUE�����KERNEL�����쳣�򷵻�FALSE��
�׳��쳣    ��
***************************************************************************/
BOOL CClusterAgent:: CheckKernel(void)
{
    //��SUN Solaris����ϵͳ�У�һ��������������"/proc"Ŀ¼�»��Ӧһ��
    //��Ŀ¼����Ŀ¼�����Ǹý��̵�PID��Agent�������ַ������KERNEL������
    //�����    
    if (m_nPidKnl == -1)  return FALSE;
    
    int nPid = ACE_OS::waitpid(m_nPidKnl, 0, WNOHANG);
    if(nPid == m_nPidKnl)
    {
        //��ʾKERNEL����û������
        TRACE(THISMODULE, S_AGENT_KNL_NOT_RUNNING);
        m_nPidKnl = -1;
        return FALSE;
    }

/*
    char szDirKnl[MAX_PATH];
    sprintf(szDirKnl, "/proc/%d", m_nPidKnl);
    
    //���Ŀ¼�Ƿ����
    if (access(szDirKnl, F_OK) < 0)
    {
        //��ʾKERNEL����û������
        TRACE(THISMODULE, S_AGENT_KNL_NOT_RUNNING);

        return FALSE;
    }
*/


    //��ӡKERNEL����������
    MSGOUT(THISMODULE, MSGOUT_LEVEL_NORMAL, 
               S_AGENT_KNL_IS_RUNNING);

    return TRUE;
}

/**************************************************************************
����ԭ��    protected:  BOOL StopKernel(void);
��������    ��KERNEL���̷��͹ر����������ȴ�KERNEL�����˳�
�������    ��
�������    ��
����ֵ          ���عر�������Ƿ�ɹ�
�׳��쳣    ��
***************************************************************************/
BOOL CClusterAgent:: StopKernel(void)
{
    MSG_INTRA* pMsg   = new MSG_INTRA;

    pMsg->SenderPid   = PT_CLSTR;
    pMsg->SenderMid   = GetModuleType();
    pMsg->ReceiverPid = PT_KERNEL;          //���͸�KERNEL����
    pMsg->ReceiverMid = MT_CLSTRINTERFACE;  //���͸�˫���ӿ�ģ�� 
    pMsg->CmdCode     = CMD_EXIT;           //�˳�����

    if (CWorkModule::SendMsg(pMsg) != ERR_SUCCESS)
    {
        return FALSE;                       //������Ϣ
    }
    
    TRACE(THISMODULE, S_AGENT_SEND_MSG_TO_STOP_KNL);
    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
               S_AGENT_SEND_MSG_TO_STOP_KNL);

    return TRUE;
}

/*************************************************************************
����ԭ��    protected:  void KillKernel(void);
��������    ���KERNEL�����Ƿ������У��������������ǿ��ɱ����
�������    ��
�������    ��
����ֵ          ��
�׳��쳣    ��
**************************************************************************/
void CClusterAgent:: KillKernel(void)
{
    if (m_nPidKnl == -1)
    {
        return;
    }

    //���ȼ��KERNEL�����Ƿ����
    if (CheckKernel() == TRUE)
    {
        
        TRACE(THISMODULE, S_AGENT_SEND_SIG_TO_KILL_KNL);
        MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                   S_AGENT_SEND_SIG_TO_KILL_KNL);

        kill(m_nPidKnl, SIGTERM); //��KERNEL���̷���SIGTERM�ź�    
    }
}

/***************************************************************************
����ԭ��    protected:  void DoAnsActive(BOOL bResponse);
��������    ����˫���ӿ�ģ�鷵�صļ���Ӧ��
�������    bResponse��˫���ӿ�ģ�鷵�صļ����Ƿ�ɹ�Ӧ��
�������    ��
����ֵ          ��
�׳��쳣    ��
****************************************************************************/
void CClusterAgent:: DoAnsActive(BOOL bResponse)
{
    //ֹͣKERNEL����Ӧ��ʱ��
    //KERNEL�����Ѿ�Ӧ��ֹͣ�ȴ�
    KillTimer(m_nKnlActTimeoutID);

    if (bResponse) //����ɹ�Ӧ��
    {
        TRACE(THISMODULE, S_AGENT_KNL_ACTIVE_SUCCESS);
        MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                   S_AGENT_KNL_ACTIVE_SUCCESS);

        //��˫�������²��������澯
        if (m_bIsSingle == FALSE)
        {
            m_ptheClusterAlarm->AlarmSwitch(); 
        }
        
        //�����澯��ⶨʱ�� 
        m_nAlarmIntervalID = SetTimer(m_uAlarmInterval);
    }
    else  //����ʧ��Ӧ��
    {
        TRACE(THISMODULE, S_AGENT_KNL_ACTIVE_FAIL);
        MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                   S_AGENT_KNL_ACTIVE_FAIL);

        StopKernel();   //��˫��ģ�鷢���˳�����
        m_nKnlOffIntervalID = SetTimer(m_uKnlOffInterval);
    }
}

/*************************************************************************
����ԭ��    protected:  int DoManualOver(MSG_INTRA* pMsgParsed);
��������    ����˫���ӿ�ģ��ת����MML�ͻ��˵��ֶ������������MML�ͻ�
                �˷����ܷ񵹻�Ӧ��
�������    pMsgParsed������Ϣ����˫���ӿ�ģ��ת����MML�ͻ��˵��ֶ���
                ������
�������    ��
����ֵ          �л��ɹ�����ERR_SUCCESS�����򷵻�ERR_FAIL�� 
�׳��쳣    ��
**************************************************************************/
int CClusterAgent:: DoManualOver(MSG_INTRA* pMsg)
{
	SInnerAlarm alm;

    struct in_addr Addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
    Addr.s_addr = pMsg->IPAddr;    
#else //_PLATFORM_HPUX
    Addr.S_un.S_addr = pMsg->IPAddr;    
#endif//_PLATFORM_HPUX

	alm.uAlarmID   = ALARM_ID_MANUAL_SWITCHOVER;
	alm.yAlarmType = AT_EVENT;
	SNPRINTF((char *)(alm.yAlarmPara), MAX_ALARM_PARA, "%s(%s)", 
		      pMsg->UserName, ACE_OS::inet_ntoa(Addr));

	SendAlarm(&alm);

    //���Զ��ܷ�ӹ��߼�����
    if (Switch(TRUE) == FALSE)
    {
        return ERR_FAIL;
    }
    else //�Զ˿��Խӹܣ����𵹻�����
    {
       //����ʧ��
       if (Switch(FALSE) == FALSE)
       {
           return ERR_FAIL;
       }
    }
    
    return ERR_SUCCESS;
}

/****************************************************************************
����ԭ��    protected:  BOOL Switch(BOOL bCheckSanity);
��������    1�����bCheckSanityΪTRUE������ø߿������ݷ���ע���FM_CHECK
                ���������Զ��Ƿ��������ӹ��߼������� 2�����bCheckSanityΪ
                FALSE�����л��߼��������Զˡ�
�������    bCheckSanity�����bCheckSanityΪTRUE��������жԶ������Ľ���
                ��飻���bCheckSanityΪFALSE���������߼������л����̡�
�������    ��
����ֵ          ���Զ˿��Խӹ��߼������������л��ɹ��򷵻�TRUE
�׳��쳣    ��
*****************************************************************************/
BOOL CClusterAgent:: Switch(BOOL bCheckSantity)
{
    if (m_ptheCluster == NULL)   return FALSE;   //�����������޷��л�
   
    //��ǰ���Աֻ��һ�����޷��л� 
    if (m_ptheCluster->GetCurrMemNum() < 2)  return FALSE;

    char szCmd[255];
    int  nStatus = 1;
    
    if ( strcmp(m_sSCVer.c_str(),"2.2") == 0 )
    {
        //��ø߿������ݷ�����
        const STRING sDataService = m_ptheCluster->GetDataService();
        
        //����߼�������
        const STRING sLogHostName = m_ptheCluster->GetLogicalHost();
        
        if (bCheckSantity)
        {
            //���Զ��ܷ�ӹ�SC
            sprintf(szCmd, "hactl -n -g -s %s -l %s", 
                sDataService.c_str(), sLogHostName.c_str());
            
#ifdef _PLATFORM_SOLARIS 
            nStatus = system1(szCmd);
#else
            nStatus = system(szCmd);
#endif
        }
        else
        {
            //�л�SC���Զ�
            sprintf(szCmd, "hactl -g -s %s -l %s", 
                sDataService.c_str(), sLogHostName.c_str());
#ifdef _PLATFORM_SOLARIS 
            nStatus = system1(szCmd);
#else
            nStatus = system(szCmd);
#endif
        }
    }
    else
    {
        //��ñ�������
        const STRING sLocalNodename = m_ptheCluster->GetLocalNodename();
        
        if (bCheckSantity)
        {
            //���Զ��ܷ�ӹ�SC ,��ΪĿǰ���ж��ǲ��ÿ�online�Ľڵ������
            nStatus = 0 ;
            
        }
        else
        {
            //�л�SC���Զ�
			//by ldf 2004-06-03 ��Ӧ���ⵥ SWPD05528
            sprintf(szCmd, "scswitch -S -h  %s &", sLocalNodename.c_str());
#ifdef _PLATFORM_SOLARIS 
            nStatus = system1(szCmd);
#else
            nStatus = system(szCmd);
#endif
        }
        
    }
    
    return (nStatus == 0);  //system()ϵͳ���óɹ�ʱ����0
}

/****************************************************************************
����ԭ��    protected:  void DoReqStatus(void);
��������    ˫������ģ���Ⲣ����˫��״̬���ݣ����������������ά������
                �е�����ģ�顣
�������    ��
�������    ��
����ֵ          ��
�׳��쳣    ��
*****************************************************************************/
void CClusterAgent:: DoReqStatus(void)
{
    MSG_INTRA* pMsg   = new (2)MSG_INTRA;
   
    pMsg->SenderPid   = PT_CLSTR;
    pMsg->SenderMid   = GetModuleType();
    pMsg->ReceiverPid = PT_OM;
    pMsg->ReceiverMid = MT_PERF;     //���͸�����ͳ��ģ��

    pMsg->CmdCode     = CMD_ANS_HB_STATUS;  //����״̬��ѯӦ��
   
    int nPrvNet = 0;
    int nActivePrvNet = 0;    //��¼�˽������

    if (m_bIsSingle == FALSE)
    {
        //���˽������
        nPrvNet = m_ptheCluster->GetNumOfPrvNet();

        if(m_ptheCluster->GetCurrMemNum() > 1)
        {
            for (int i = 0; i < nPrvNet; i++)
            {
                if (m_ptheCluster->GetPrvNetStatus(i))
                {
                    nActivePrvNet++;
                }
            }
        }
    }
     
    pMsg->Data[0] = (BYTE)nPrvNet;        //��һ�ֽڴ��˽������
    pMsg->Data[1] = (BYTE)nActivePrvNet;  //�ڶ��ֽڴ�ſ�����������
    
    SendMsg(pMsg);   //������Ϣ
}

/***************************************************************************
����ԭ��      protected: void DoAskFailover(EFAILOVERCAUSE_TYPE eCause);
��������      ����˫���ӿ�ģ�鷢��˫������ģ��ĵ������룬����Զ˲��ܽӹ�
              �߼���������������Ϣ��
�������      eCause��˫���ӿ�ģ��ת��������ģ��ĵ�������ԭ��
�������      ��
����ֵ        ��
�׳��쳣      ��
****************************************************************************/
void CClusterAgent:: DoAskFailover(EFAILOVERCAUSE_TYPE eCause)
{
    char szCause[1024];
    char szMsg[1024];
    BOOL bDoFailOver = FALSE;
    
    //�������뵹����ԭ���ٲøôε����Ƿ����ִ��
    switch (eCause)
    {
    //����˫��ϵͳ�ǹ���������У����̿ռ䲻�������������һ���ģ���˵�����Ч
    case ERROR_NOENOUGHDISK_PRIMARY:    //��Ӳ�̿ռ䲻��
        strncpy(szCause, S_ERR_NOENOUGHDISK_PRIMARY, 1024);
        break;
        
    case ERROR_NOENOUGHDISK_SECONDARY:  //��Ӳ�̿ռ䲻��
        strncpy(szCause, S_ERR_NOENOUGHDISK_SECONDARY, 1024);
        break;
        
    case ERROR_HARDDISK:                //����RAID��Ӳ�̹���
        strncpy(szCause, S_ERR_HARDDISK, 1024);
        break;

    //����д�ļ�ʧ�ܡ������쳣����Դ���㣬�򵹻����Խ���
    case ERROR_WRITEFILE:
        strncpy(szCause, S_ERR_WRITEFILE, 1024);
        bDoFailOver = TRUE;
        break;
        
    case ERROR_PROCESS_EXCEPTION:
        strncpy(szCause, S_ERR_PROCESS_EXCEPTION, 1024);
        bDoFailOver = TRUE;
        break;
        
    case ERROR_ACTIVE:
        strncpy(szCause, S_ERR_ACTIVE, 1024);
        bDoFailOver = TRUE;
        break;
        
    case ERROR_RESOURCE:
        strncpy(szCause, S_ERR_RESOURCE, 1024);
        bDoFailOver = TRUE;
        break;

    default:
        SNPRINTF(szCause, 1024, "%s(CauseCode = %d)", 
                 S_ERR_UNRECOGNIZED, eCause);
        break;
    }
    szCause[1023] = 0;
    SNPRINTF(szMsg, 1024, S_AGENT_ASK_FAILOVER, szCause);
    szMsg[1023] = 0;
    
    TRACE(THISMODULE, szMsg);
    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, szMsg);


    
    if(bDoFailOver && Switch(TRUE))  //���Զ��Ƿ���Խӹ�
    {
        WriteLog(MTS_CLSTR, szMsg);
        Switch(FALSE); //�Զ˿��Խӹܣ�����SC�л�
    }
    //by ldf 2002.3.15 ��Ӧ���ⵥD11827
    else if(ERROR_PROCESS_EXCEPTION == eCause)
    //���ܵ����������н����˳�
    {
        m_nRecvSignal = SIGTERM;
    }
}

/****************************************************************************
����ԭ��    protected:  void ParseMsg(MSG_INTRA* pMsg) ;
��������    �������еķ��˳���Ϣ��
�������    pMsg����Ϣ���е�ͷ��Ϣ������Ϣ�д��ھ����ģ����о���
                �Ĵ�����Ϣ������󣬸���ϢӦ�ñ��ͷš�
�������    ��
����ֵ          ��
�׳��쳣    ��
*****************************************************************************/
void CClusterAgent:: ParseMsg(MSG_INTRA* pMsg)
{
    switch (pMsg->CmdCode)
    {
        case CMD_ANS_ACTIVE:     //KERNEL���̵ļ���Ӧ��
        {
            MSGOUT(THISMODULE, MSGOUT_LEVEL_NORMAL,
                       S_AGENT_RCV_KNL_ACTIVE);

            BOOL bActive = *((UINT4*)pMsg->Data);
        
            DoAnsActive(bActive);
            break;
        }
   
        case CMD_ASK_FAILOVER:   //��������
        {
            SFailOverCause* pCause = (SFailOverCause *)(pMsg->Data);
            EFAILOVERCAUSE_TYPE eCause = pCause->Cause;
             
            if (m_bIsSingle == FALSE)
            {
                DoAskFailover(eCause);
            }
            else if(ERROR_PROCESS_EXCEPTION == eCause)
            //�����²��ܵ����������н����˳�
            {
                m_nRecvSignal = SIGTERM;
            }
            break;    
        } 
      
        case CMD_REQ_STATUS:     //����ģ���˫��״̬��ѯ
        {
            DoReqStatus();
            break;
        }
    
        case CMD_MML:
        {
            //��MML��������ȡ������
            CParseResult pr(pMsg);
            pr.CreateParseResult(pMsg);
            char szAnswer[128];
            
            if (MML_RESET == pr.GetCmdCode() )
            {
                DoReset(pMsg);
               
            }


            //by ldf 2002.4.1 ��Ӧ���ⵥD12300 D13201
            static BOOL SwitchLock = FALSE;

            if ( (MML_SWP == pr.GetCmdCode()) && (FALSE == SwitchLock) )
            {
                //�޸����

                //ֻ����˫�������£�������������
                if (m_bIsSingle == FALSE)
                {
                    //ִ���л������������л����
                    int nRet = DoManualOver(pMsg);

                    //��Ҫ������д��־
                    SLog log;
                    log.LogType    = SLog::LOG_OPER;
                    log.uIPAddr    = pMsg->IPAddr;
                    if (ERR_SUCCESS == nRet)
                    {
                        sprintf(szAnswer, "%s", S_AGENT_HANDOVER_SUCCESS);
                        log.OperResult = SLog::OPER_SUCCESS;

                        //by ldf 2002.4.27 ��Ӧ���ⵥD12300 D13201
                        SwitchLock = TRUE;
                        //�޸����
                    }
                    else
                    {
                        sprintf(szAnswer, "%s", S_AGENT_HANDOVER_FAIL);
                        log.OperResult = SLog::OPER_FAIL;
                    }

                    strcpy(log.szUserName, pMsg->UserName);
                    strcpy(log.szOperCmd, S_AGENT_MML_CMD_SWP);
                    strcpy(log.szCmdDesc, S_AGENT_MML_CMDDESC_SWP);

                    WriteLog(&log);   //д��־
                      
                    CMMLReport Report(THISMODULE);
                    Report.MakeReportHead();
                    Report.MakeCmdDisplay(pr.GetCmdLine(), 0, nRet, THISMODULE);
                    Report.AddString(szAnswer);
                    Report.AddString(STR_NEWLINE);  //����
                    Report.MakeReportTail();
                     
                    const char* pChar = Report.GetReport();
                    
                    int nLen = strlen(pChar);
                    MSG_INTRA* pAns = new (nLen)MSG_INTRA;
    
                    memcpy((void *)(pAns->Data), pChar, nLen);
                    pAns->SenderPid   = GetProcessType();
                    pAns->SenderMid   = GetModuleType();
                    pAns->ReceiverPid = PT_OM;           
                    pAns->ReceiverMid = MT_MMLDEBUG;
                    pAns->AppType     = pMsg->AppType;
                    pAns->ClientNo    = pMsg->ClientNo;
                    pAns->IPAddr      = pMsg->IPAddr;
                     
                    SendMsg(pAns); 
 
                    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, szAnswer);
            }
               //by ldf 3-13 ��Ӧ���ⵥD11722 2002.3.14
                else
                {
                    sprintf(szAnswer, "%s", S_AGENT_CANNOT_HANDOVER);  
                    CMMLReport Report(THISMODULE);
                    Report.MakeReportHead();
                    Report.MakeCmdDisplay(pr.GetCmdLine(), 0, ERR_FAIL, THISMODULE);
                    Report.AddString(szAnswer);
                    Report.AddString(STR_NEWLINE);  //����
                    Report.MakeReportTail();
                     
                    const char* pChar = Report.GetReport();
                    
                    int nLen = strlen(pChar);
                    MSG_INTRA* pAns = new (nLen)MSG_INTRA;
    
                    memcpy((void *)(pAns->Data), pChar, nLen);
                    pAns->SenderPid   = GetProcessType();
                    pAns->SenderMid   = GetModuleType();
                    pAns->ReceiverPid = PT_OM;           
                    pAns->ReceiverMid = MT_MMLDEBUG;
                    pAns->AppType     = pMsg->AppType;
                    pAns->ClientNo    = pMsg->ClientNo;
                    pAns->IPAddr      = pMsg->IPAddr;
                     
                    SendMsg(pAns); 
 
                    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, szAnswer);
                }
            }
           
            break; 
        }

        default:
        {
            break;
        }
    }
}

/*************************************************************************
����ԭ��    protected:  void OnTimer(const long nTimerID);
��������    �Ա��ΪnTimerID�Ķ�ʱ���Ĵ����¼����д���
�������    nTimerID����ǰ��Ҫ����Ķ�ʱ���¼���Ӧ�Ķ�ʱ����ID
�������    ��
����ֵ          ��
�׳��쳣    ��
**************************************************************************/
void CClusterAgent:: OnTimer(const long nTimerID)
{
    if (nTimerID == m_nAlarmIntervalID)
    {
        //�澯��ʱ���
        m_ptheClusterAlarm->Alarm();
    }
    else if (nTimerID == m_nCheckKnlID)
    { 
        //��ʱ���KERNEL����
        if (!CheckKernel())            //���KERNEL�Ѿ��˳�����˫������Ҳ�˳� 
        {
            MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                   S_AGENT_SEND_QUIT_TO_MGR);
 
            SendQuitToMe();   //������ģ�鷢���˳�����
        }
    }
    else if (nTimerID == m_nKnlActTimeoutID)    
    {
        //KERNEL����Ӧ��ʱ
        if (!CheckKernel())
        {
            MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                   S_AGENT_SEND_QUIT_TO_MGR);

            SendQuitToMe();   //������ģ�鷢���˳�����
        }
        else
        {
            KillTimer(m_nKnlActTimeoutID);
            KillTimer(m_nCheckKnlID);
            KillTimer(m_nAlarmIntervalID);
            KillTimer(m_nSendHeartBeatID);
            StopKernel();      //��KERNEL���̷��͹ر�����
            
            //��ʱ���KERNEL�Ƿ��Ѿ��ر�
            m_nKnlOffIntervalID = SetTimer(m_uKnlOffInterval);
            m_uKnlOffCount = 0;
        }
    } 
    else if (nTimerID == m_nKnlOffIntervalID)
    {
        if (!CheckKernel())
        {
            MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                S_AGENT_SEND_QUIT_TO_MGR);
            
            SendQuitToMe();     //������ģ�鷢�˳�����
        }
        else
        {
            m_uKnlOffCount += m_uKnlOffInterval;
            if (m_uKnlOffCount >= m_uKnlOffTimeout)
            {
                KillKernel();               //ǿ��ɱ��KERNEL
                
                MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                    S_AGENT_SEND_QUIT_TO_MGR);
                
                SendQuitToMe();             //�����ģ�鷢�˳�����
            } 
        }
    }
    else if ( nTimerID == m_nSendHeartBeatID )
    {
        DoReqStatus();
    }

    //by ldf ��Ӧ���ⵥD11862 2002-3-16
    else if(nTimerID == m_nTimerIDBase)
    {
        proc_signal(m_nRecvSignal);
        m_nRecvSignal = 0;
    }

}

/****************************************************************************
����ԭ��    protected:  int handle_signal(int signum, siginfo_t*, 
                                              ucontext_t* ut)
��������    ���ݲ�ͬ�ź�ִ����Ӧ����
�������    signum���ź�
�������    ��
����ֵ          ����ֵ�������£� >0: �ñ�handler�ٴ�ȡ�ô���֮ǰ������handler
                �ɱ�����, ����handler��ĺ����� =0: ���ֵǼǣ� <0: ����
                handle_close��ȡ���Ǽǡ�
�׳��쳣    ��
*****************************************************************************/
int CClusterAgent:: handle_signal(int signum, siginfo_t* st,
                                  ucontext_t* ut)

{
    //by ldf ��Ӧ���ⵥD11862 2002-3-16
    if(( signum > 0) && ( signum <= 36))
    {
        m_nRecvSignal = signum;
    }

    return 0;
}

//by ldf ��Ӧ���ⵥD11862 2002-3-16
void CClusterAgent:: proc_signal(int signum)
{
     switch (signum)
     {
     case SIGTERM:
         TRACE(THISMODULE, S_AGENT_RCV_SIGTERM);
         MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT,
                S_AGENT_RCV_SIGTERM);

         if (!CheckKernel())
         {
             MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                    S_AGENT_SEND_QUIT_TO_MGR);

             SendQuitToMe();      //�����ģ�鷢���˳�����
         }
         else
         {
             KillTimer(m_nKnlActTimeoutID);
             KillTimer(m_nCheckKnlID);
             KillTimer(m_nAlarmIntervalID);
             KillTimer(m_nSendHeartBeatID);
             StopKernel();        //
            
             m_nKnlOffIntervalID = SetTimer(m_uKnlOffInterval);
         }
         break;
     
     //KERNEL�����˳�
     case SIGCLD:
         MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                S_AGENT_RCV_SIGCLD);
         TRACE(THISMODULE, S_AGENT_RCV_SIGCLD);
         
                   
         MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT,
                S_AGENT_SEND_QUIT_TO_MGR);

         SendQuitToMe();
 
         break;
         
     default:
         break; 
     }
}
/********************************************************
* ����: CClusterAgent::OnReset
* ����: �����ֹ���������
* ����: pMsgParsed : ��Ϣ֡
* ���:
* ����: void
********************************************************/
void CClusterAgent::DoReset(MSG_INTRA* pMsgParsed)
{
    //��¼����Ա�û���
    char szResetCfgPath[MAX_PATH];
    SNPRINTF(szResetCfgPath, sizeof(szResetCfgPath),
             "%s/%s", GetAppPath(), S_RESET_INFO_FILE_NAME);
    szResetCfgPath[sizeof(szResetCfgPath) - 1] = '\0';

    CINIFile ini(szResetCfgPath);
    BOOL bSucc = ini.Open();
    if(bSucc)
    {
        char szUserName[48];        
        strcpy(szUserName, pMsgParsed->UserName);
        
        ini.WriteString(CFG_SEC_OPERATOR,CFG_OPERATOR_KEY_NAME,szUserName);//��¼�û�����Ϣ
        ini.Close();
    }
    else
    {
        TRACE(MTS_CLSTR, S_BS_FAIL_OPEN_FILE, szResetCfgPath);
    }

    //��Ҫ������д��־
    SLog log;
    log.LogType    = SLog::LOG_OPER; 
    log.uIPAddr    = pMsgParsed->IPAddr;
    log.OperResult = SLog::OPER_SUCCESS;
    strcpy(log.szUserName, pMsgParsed->UserName);
    strcpy(log.szOperCmd, S_AGENT_MML_CMD_RESET);
    strcpy(log.szCmdDesc, S_AGENT_MML_CMDDESC_RESET);

    WriteLog(&log);   //д��־

    //��Ӧ�ֹ���������
    char szAnswer[128];
    sprintf(szAnswer, "%s", S_MANNUL_RESET_SUCCESS);

    CParseResult pr(pMsgParsed);
    pr.CreateParseResult(pMsgParsed);
    
    CMMLReport Report(THISMODULE);
    Report.MakeReportHead();
    Report.MakeCmdDisplay(pr.GetCmdLine(), 0, ERR_SUCCESS, THISMODULE);
    Report.AddString(szAnswer);
    Report.AddString(STR_NEWLINE);  //����
    Report.MakeReportTail();
    
    const char* pChar = Report.GetReport();
    
    int nLen          = strlen(pChar);
    MSG_INTRA* pAns   = new (nLen)MSG_INTRA;
    pAns->SenderPid   = GetProcessType();
    pAns->SenderMid   = GetModuleType();
    pAns->ReceiverPid = PT_OM;
    pAns->ReceiverMid = MT_MMLDEBUG;
    pAns->AppType     = pMsgParsed->AppType;
    pAns->ClientNo    = pMsgParsed->ClientNo;
    pAns->IPAddr      = pMsgParsed->IPAddr;
    
    memcpy(pAns->Data, pChar, nLen);
    SendMsg(pAns);
   
    //˫�������˳�
    StopKernel();
}

