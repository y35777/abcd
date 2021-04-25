/***************************************************************************
 *��Ʒ��        ��iGateway Bill V200
 *ģ����        ��˫���ӿ�ģ��(ClusterInterface)
 *�ļ���        ��cluster_interface.cpp
 *������Ϣ      �������װ��˫���ӿ�ģ�����˫������Ľӿڡ�
 *�汾˵��      ��V200R001i00M00B00D1130A
 *��Ȩ��Ϣ      ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����          ������ƽ
 *����ʱ��      ��2001-10-30
 *�޸ļ�¼      ��
****************************************************************************/

#include "cluster_interface.h"
#include "resource.h"

/***************************************************************************
����ԭ��	public: CClusterInterface()
��������	���캯������ʼ����Ա������
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
****************************************************************************/
CClusterInterface:: CClusterInterface()
{
    //���������̿ռ䲻��ʱ��˫���ӿ�ģ��������н���������
    //ģ�鷢����ͣ���ջ��������m_bNetworkPausedָʾ�Ƿ�
    //�͹��������ʼ��ΪFALSE����ʾ�������͹���ͣ���ջ�����
    //����
    m_bNetworkPaused = FALSE;            
    
    //m_yDiskFullMaskָʾ�Ƿ���������̿ռ䲻��Ĺ��ϣ�bit0Ϊ1
    //ָʾǰ���̿ռ䲻�㣬bit1Ϊ1ָʾ����̿ռ䲻�㣬��ʼ��Ϊ
    //0��ʾ��������ǰ������̿ռ䲻��Ĺ���
    m_yDiskFullMask   = NO_DISK_FULL;

    m_nAPCount        = 0;
    m_pFailOverCause  = NULL;
}

/***************************************************************************
����ԭ��	public: ~CClusterInterface()
��������	��������
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
****************************************************************************/
CClusterInterface:: ~CClusterInterface()
{
    if (m_pFailOverCause)
    {
        delete m_pFailOverCause;
        m_pFailOverCause = NULL;
    }
}

/**************************************************************************
����ԭ��	public: virtual int InitInstance()
��������	��ʼ������
�������	�ޡ�
�������	�ޡ�
����ֵ  	���س�ʼ���Ƿ�ɹ���ʧ�ܷ��ط���ֵ���ɹ�����0��
�׳��쳣	�ޡ�
***************************************************************************/
int CClusterInterface::InitInstance()
{
    //���û���ĳ�ʼ������
    int nRet = CWorkModule::InitInstance();

    if (nRet != ERR_SUCCESS)
    {
        m_nStatus = CLSTRINT_FAILURE;
        return nRet;
    }

    //ע�ᶨʱ������ʼ��״̬
    m_lTimerID5Sec = SetTimer(FIVE_SECOND_IN_MILISEC);
    
    m_nStatus = CLSTRINT_INIT;      //˫���ӿ����ڳ�ʼ��

    //�������ļ��ж�ȡ�������
    CINIFile *pIniFile = new CINIFile(GetCfgFilePath());
    if (!pIniFile->Open())
    {
        pIniFile->Close();
        delete pIniFile;
        m_nStatus = CLSTRINT_FAILURE;

        return ERR_OPEN_CFG_FILE;
    }
    
    m_nAPCount = (UINT4)pIniFile->GetInt(CFG_SEC_COMMON,
					 CFG_COMMON_KEY_APCOUNT, 
					 CFG_COMMON_KEY_APCOUNT_DEFAULT);
    if (0 > m_nAPCount)
    {
		pIniFile->Close();
		delete pIniFile;
        return ERR_FAIL;
    }

    int nNoCluster = pIniFile->GetInt(
                          CFG_SEC_COMMON,
                          CFG_COMMON_KEY_NO_CLUSTER,
                          CFG_COMMON_KEY_NO_CLUSTER_DEFAULT);

    int nClusterMode = pIniFile->GetInt(
                          CFG_SEC_COMMON,
                          CFG_COMMON_KEY_CLUSTER_MODE,
                          CFG_COMMON_KEY_CLUSTER_MODE_TEST);
    if(nClusterMode == CFG_COMMON_KEY_CLUSTER_MODE_TEST)
    //ClusterModeδ����
    {
        if(nNoCluster == CFG_COMMON_KEY_NO_CLUSTER_DEFAULT)
        //NoClusterδ����
        {
            m_bSingleServer = !CFG_COMMON_KEY_CLUSTER_MODE_DEFAULT;
        }
        else
        {
            m_bSingleServer = nNoCluster;
        }
    }
    else
    {
        m_bSingleServer = !nClusterMode;
    }

    pIniFile->Close();
    delete pIniFile;

    return ERR_SUCCESS;
}

/**************************************************************************
����ԭ��	public: virtual void ExitInstance()
��������	�˳�ʱ��������
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
***************************************************************************/
void CClusterInterface:: ExitInstance()
{
    //ע����ʱ��
    KillTimer(m_lTimerID5Sec);

    CWorkModule::ExitInstance();
}

/**************************************************************************
����ԭ��	protected: void  ParseMsg(MSG_INTRA* pMsg)
��������	��Ϣ�ַ�����
�������	������ģ�鷢��������Ϣ��
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
***************************************************************************/
void CClusterInterface:: ParseMsg(MSG_INTRA* pMsg)
{
    switch (pMsg->CmdCode)
    {
        //���ģ�鷢�͵ĳ�ʼ��Ӧ��
        case CMD_ANS_INIT:
        {
            //�ж��Ƿ��Ǽ��ģ�鷢�͵ĳ�ʼ��Ӧ����Ϣ
            if (MT_MONITOR == pMsg->SenderMid)
            {
                ProcInitAns(*((BOOL *)(pMsg->Data)));
            }

            break;
        }

        //���ģ�鷢�͵ļ���Ӧ��
        case CMD_ANS_ACTIVE:
        {
            if (MT_MONITOR == pMsg->SenderMid)
            {
                ProcActiveAns(*((BOOL *)(pMsg->Data)));
            }

            break;
        }

        //˫��������߹���ģ�鷢�͵��˳�����
        case CMD_EXIT:
        {
            if ((CLSTRINT_DEACTIVING != m_nStatus) 
             && (CLSTRINT_DEACTIVE   != m_nStatus))
            {
                 TRACE(THISMODULE, S_CLUSTERINTERFACE_MSG_DEACTIVEME);
                 MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, 
                        S_CLUSTERINTERFACE_MSG_DEACTIVEME);
                 
                 DeActive();
            }

	    break;
        }

        case CMD_ANS_DEACTIVE:
        {
            if (CLSTRINT_DEACTIVING == m_nStatus)
            {
                TRACE(THISMODULE, S_CLUSTERINTERFACE_MSG_SHUTDOWN);
                MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, 
                       S_CLUSTERINTERFACE_MSG_SHUTDOWN);
                Shutdown();
            }
        
            break;
        }

         //����˫����������
         case CMD_ASK_FAILOVER:
         {
              BYTE nCause = *((BYTE *)(pMsg->Data));
              void* pData = (void*) ((BYTE *)(pMsg->Data) + sizeof(BYTE));
              UINT4 nLen = pMsg->Len - sizeof(BYTE);

              TRACE(THISMODULE, S_CLUSTERINTERFACE_MSG_RCV_ASKFAILOVER, nCause);
              MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, 
                     S_CLUSTERINTERFACE_MSG_RCV_ASKFAILOVER, nCause);

              ProcAskedFailOver((PROCESS_TYPE)(pMsg->SenderPid), 
                                (MODULE_TYPE)(pMsg->SenderMid), 
                                 nCause, 
                                 pData, 
                                 nLen);
              break;
          }
   
          //�ֹ���������
          case CMD_MANUALOVER:
          {
              //�ֹ�����������MML SERVERֱ��ת����˫��������
              //����Ҫ����˫���ӿ�ת��
              //ProcManualOverCmd();

              break;
          }

          case CMD_DISKSPACERECOVERY:
          {
              BYTE yDiskFullMask = *((BYTE *)(pMsg->Data));

              if ((yDiskFullMask == PRIMARY_DISK_FULL) 
               || (yDiskFullMask == SECONDARY_DISK_FULL))
              {
                  //ֻ��ǰ����̿ռ���������ŶԴ��̿ռ�ָ����д���
                  m_yDiskFullMask = m_yDiskFullMask & (~yDiskFullMask);

                  if (NO_DISK_FULL == m_yDiskFullMask) 
                  {
                      ProcDiskSpaceRecovery();
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

/***********************************************************************************
����ԭ��	protected: virtual MODULE_TYPE GetModuleType();
��������	����ģ������
�������	�ޡ�
�������	�ޡ�
����ֵ  	ģ�����͡�
�׳��쳣	�ޡ�
************************************************************************************/
MODULE_TYPE CClusterInterface:: GetModuleType()
{
     return MT_CLSTRINTERFACE;
}

/**********************************************************************************
����ԭ��	protected: void SendActiveAnsToCluster(BOOL bResult)
��������	��˫�����̵�˫������/����ģ�鷢����Ӧ����Ϣ
�������	bResult---�������� 
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
***********************************************************************************/
void CClusterInterface:: SendActiveAnsToCluster(BOOL bResult)
{
    MSG_INTRA *pMsg  = new(sizeof(UINT4)) MSG_INTRA;

    pMsg->SenderPid   = GetProcessType(); 
    pMsg->SenderMid   = GetModuleType();
    pMsg->ReceiverPid = PT_CLSTR;
    pMsg->ReceiverMid = MT_CLSTR;
    pMsg->CmdCode     = CMD_ANS_ACTIVE;
    pMsg->Len         = sizeof(UINT4);

    *(UINT4 *)(pMsg->Data) = bResult;

    SendMsg(pMsg);
}

/***********************************************************************************
����ԭ��	protected: void ProcInitAns(BOOL bResult)
��������	�����ʼ��Ӧ����Ϣ
�������	�����ʼ���ɹ���ΪTRUE������ΪFALSE��
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
************************************************************************************/
void CClusterInterface:: ProcInitAns(BOOL bResult)
{
    //��ǰ״̬�����ڳ�ʼ��
    if (CLSTRINT_INIT != m_nStatus)
    {
         return;
    }
   
    if (TRUE == bResult)
    {
        //���ģ���ʼ���ɹ������𼤻����
        TRACE(THISMODULE, S_CLUSTERINTERFACE_MSG_MONITORINIT_OK, bResult);
        
        m_nStatus = CLSTRINT_ACTIVING;
        Active();
    }
    else
    {
        //ϵͳ��ʼ��ʧ�ܣ���˫������ģ�鷢�ͳ�ʼ��ʧ�ܵ���Ϣ
        TRACE(THISMODULE, S_CLUSTERINTERFACE_MSG_MONITORINIT_FAIL);

        m_nStatus = CLSTRINT_FAILURE;
        SendActiveAnsToCluster(FALSE);

/*
#ifdef _PLATFORM_HPUX
#ifdef _PRODUCT_CCS
        //added by chenliangwei, 2003-04-30
        if (m_bSingleServer)
        {
            Shutdown();
        }
        //added end
#endif //_PRODUCT_CCS
#endif //_PLATFORM_HPUX
*/
    }
}

/***********************************************************************************
����ԭ��	protected: void ProcActiveAns(BOOL bResult)
��������	������Ӧ����Ϣ
�������	�������ɹ���ΪTRUE ������ΪFALSE��
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
************************************************************************************/
void CClusterInterface:: ProcActiveAns(BOOL bResult)
{
    if (CLSTRINT_ACTIVING != m_nStatus)
    {
        return;
    }

    TRACE(THISMODULE, S_CLUSTERINTERFACE_MSG_RCV_ANSACTIVE, bResult);
    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, 
           S_CLUSTERINTERFACE_MSG_RCV_ANSACTIVE, bResult);

    //�������״̬Ǩ�ƣ�����˫������ģ��Ӧ��
    if (TRUE == bResult)
    {
        m_nStatus = CLSTRINT_ACTIVE;
        SendActiveAnsToCluster(TRUE);     
    } 
    else
    {
        m_nStatus = CLSTRINT_FAILURE;
        SendActiveAnsToCluster(FALSE); 

/*
#ifdef _PLATFORM_HPUX
#ifdef _PRODUCT_CCS
        //added by chenliangwei, 2003-04-30
        if (m_bSingleServer)
        {
            Shutdown();
        }
        //added end
#endif //_PRODUCT_CCS
#endif //_PLATFORM_HPUX
*/
    }
}

/*************************************************************************************
����ԭ��	protected: void Active()
��������	���𼤻����
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
*************************************************************************************/
void CClusterInterface:: Active()
{
    //����ģ�鷢�ͼ�������
    if ((CLSTRINT_INIT == m_nStatus) || (CLSTRINT_ACTIVING == m_nStatus))
    {
        MSG_INTRA *pMsg = new MSG_INTRA;

        pMsg->SenderPid   = PT_SELF; 
        pMsg->SenderMid   = GetModuleType();
        pMsg->ReceiverPid = PT_SELF;
        pMsg->ReceiverMid = MT_MONITOR;
        pMsg->CmdCode     = CMD_ACTIVE;

        SendMsg(pMsg);
        
        //������״̬Ǩ�Ƶ����ڼ���״̬
        m_nStatus = CLSTRINT_ACTIVING;
    }
}

/*********************************************************************************
����ԭ��	protected: void DeActive()
��������	����ȥ�������
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
**********************************************************************************/
void CClusterInterface:: DeActive()
{
    //����ģ�鷢��ȥ��������
    if (CLSTRINT_DEACTIVE != m_nStatus) 
    { 
        MSG_INTRA *pMsg   = new MSG_INTRA;

        pMsg->SenderPid   = PT_SELF; 
        pMsg->SenderMid   = GetModuleType();
        pMsg->ReceiverPid = PT_SELF;
        pMsg->ReceiverMid = MT_MONITOR;
        pMsg->CmdCode     = CMD_DEACTIVE;

        SendMsg(pMsg);

        //������״̬Ǩ�Ƶ�ȥ����״̬
        m_nStatus = CLSTRINT_DEACTIVING;
    }
}

/**********************************************************************************
����ԭ��	protected: void  Shutdown()
��������	����ر��˳�����
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
***********************************************************************************/
void CClusterInterface:: Shutdown()
{
    MSG_INTRA *pMsg   = new MSG_INTRA;

    pMsg->SenderPid   = PT_SELF; 
    pMsg->SenderMid   = GetModuleType();
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = MT_MONITOR;
    pMsg->CmdCode     = CMD_EXIT;

    SendMsg(pMsg);
}

/**********************************************************************************
����ԭ��	protected: void ProcAskedFailOver(const PROCESS_TYPE pt, 
                                                  const MODULE_TYPE mt, 
                                                  const BYTE nCause, 
                                                  const void* pData = 0, 
                                                  const UINT4 nLen =0)
��������	��������ģ�鷢�͵ĵ�������
�������	pt---���͵�������Ľ��̱�š� 
                mt---���͵��������ģ��� 
                nCause---����ԭ�� 
                pData---�������ݵ�ָ�� 
                nLen---�������ݳ��ȡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
***********************************************************************************/
void CClusterInterface:: ProcAskedFailOver(const PROCESS_TYPE pt, 
                                           const MODULE_TYPE mt, 
                                           const BYTE nCause, 
                                           const void* pData, 
                                           const UINT4 nLen)
{
    if (m_pFailOverCause)
    {
        delete m_pFailOverCause;
        m_pFailOverCause = NULL;
    }

    m_pFailOverCause = new (nLen)SFailOverCause;
    m_pFailOverCause->PID   = pt;
    m_pFailOverCause->MID   = mt;
    m_pFailOverCause->Cause = (EFAILOVERCAUSE_TYPE) nCause;

    memcpy(m_pFailOverCause->Para, pData, nLen);

    MSG_INTRA *pMsg = new(FAILOVERCAUSE_HEADLENTGH 
                          + m_pFailOverCause->Length)MSG_INTRA;

    pMsg->SenderPid   = GetProcessType(); 
    pMsg->SenderMid   = GetModuleType();
    pMsg->ReceiverPid = PT_CLSTR;
    pMsg->ReceiverMid = MT_CLSTR;
    pMsg->CmdCode     = CMD_ASK_FAILOVER;

    memcpy(pMsg->Data, m_pFailOverCause, pMsg->Len);

    //��˫�������͵�������
    SendMsg(pMsg);

    //����ԭ����ǰ���̿ռ䲻��
    if (ERROR_NOENOUGHDISK_PRIMARY == nCause)
    {
        if (NO_DISK_FULL == m_yDiskFullMask)
        {
           //��������������̵�����ģ����ͣ�շ�����
           MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT,
                  S_MSG_DISKFULL_MTNETWORK_PAUSING);

           ProcDiskSpaceFull();
        }
	
        m_yDiskFullMask = m_yDiskFullMask | PRIMARY_DISK_FULL;
    }
    //����ԭ���Ǻ���̿ռ䲻��
    else if (ERROR_NOENOUGHDISK_SECONDARY == nCause)
    {
        if (NO_DISK_FULL == m_yDiskFullMask)
        {
            //��������������̵�����ģ����ͣ�շ�����
            MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, 
                   S_MSG_DISKFULL_MTNETWORK_PAUSING);

            ProcDiskSpaceFull();
        }
 
        m_yDiskFullMask = m_yDiskFullMask | SECONDARY_DISK_FULL;
     }
     else
     {
          m_nStatus = CLSTRINT_FAILOVER;
     }
}

/*****************************************************************************************
����ԭ��	protected: void  ProcManualOverCmd()
��������	����MMLģ�鷢�����ֶ���������
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
******************************************************************************************/
void CClusterInterface:: ProcManualOverCmd()
{
    MSG_INTRA *pMsg   = new MSG_INTRA;

    pMsg->SenderPid   = GetProcessType(); 
    pMsg->SenderMid   = GetModuleType();
    pMsg->ReceiverPid = PT_CLSTR;
    pMsg->ReceiverMid = MT_CLSTR;
    pMsg->CmdCode     = CMD_MANUALOVER;
		
	SendMsg(pMsg);
}

/***************************************************************************************
����ԭ��	protected: void ProcDiskSpaceFull()
��������	������̿ռ�����֪ͨ����������ģ����ͣ�շ�����
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
****************************************************************************************/
void CClusterInterface:: ProcDiskSpaceFull()
{
    int i = 0;
    for (i = 0; i < m_nAPCount; i++)
    {
        MSG_INTRA *pMsg   = new MSG_INTRA;

        pMsg->SenderPid   = GetProcessType(); 
        pMsg->SenderMid   = GetModuleType();
        pMsg->ReceiverPid = PT_AP_BASE + i;
        pMsg->ReceiverMid = MT_NETWORK;
        pMsg->CmdCode     = CMD_STOP_NETWORK;
        SendMsg(pMsg);

        pMsg              = new MSG_INTRA;
        pMsg->SenderPid   = GetProcessType(); 
        pMsg->SenderMid   = GetModuleType();
        pMsg->ReceiverPid = PT_CO_BASE + i;
        pMsg->ReceiverMid = MT_COLLECTOR;
        pMsg->CmdCode     = CMD_STOP_NETWORK;
        SendMsg(pMsg);
   }

   m_bNetworkPaused = TRUE;
}

/****************************************************************************************
����ԭ��	protected: void  ProcDiskSpaceRecovery()
��������	������̿ռ�ָ���֪ͨ��������ֹͣ������ģ�����¿�ʼ�շ�����
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
*****************************************************************************************/
void CClusterInterface:: ProcDiskSpaceRecovery()
{
    //û��������ģ�鷢�͹���ͣ����
    if (m_bNetworkPaused == FALSE) 
    {
        return;
    }

    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, 
           S_MSG_DISKRECOVERY_MTNETWORK_GOINGON);

    int i = 0;
    for(i = 0; i < m_nAPCount; i++)
    {
        MSG_INTRA *pMsg   = new MSG_INTRA;
        pMsg->SenderPid   = GetProcessType(); 
        pMsg->SenderMid   = GetModuleType();
        pMsg->ReceiverPid = PT_AP_BASE + i;
        pMsg->ReceiverMid = MT_NETWORK;
        pMsg->CmdCode     = CMD_RESUME_NETWORK;
        SendMsg(pMsg);

        pMsg   = new MSG_INTRA;
        pMsg->SenderPid   = GetProcessType(); 
        pMsg->SenderMid   = GetModuleType();
        pMsg->ReceiverPid = PT_CO_BASE + i;
        pMsg->ReceiverMid = MT_COLLECTOR;
        pMsg->CmdCode     = CMD_RESUME_NETWORK;
        SendMsg(pMsg);
    }

    m_bNetworkPaused = FALSE;
}

/***************************************************************************************
����ԭ��	protected: void  OnTimer(const long nTimerID);
��������	��ʱ������
�������	ʱ��ID ��ֻ����
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
****************************************************************************************/
void CClusterInterface:: OnTimer(const long nTimerID)
{
    if (nTimerID == m_lTimerID5Sec)
    {
        Timer5Sec();
    }

    CWorkModule::OnTimer(nTimerID);
}

/*****************************************************************************************
����ԭ��	protected: void  Timer5Sec();
��������	5 �붨ʱ������
�������	�ޡ�
�������	�ޡ�
����ֵ  	�ޡ�
�׳��쳣	�ޡ�
******************************************************************************************/
void CClusterInterface:: Timer5Sec()
{
    MSG_INTRA *pMsg = NULL;

    switch (m_nStatus)
    {
    case CLSTRINT_FAILOVER:
        //��˫������/����ģ�鷢�͵�����������(CMD_ASK_FAILOVER)��
        pMsg = new(FAILOVERCAUSE_HEADLENTGH 
                  + m_pFailOverCause->Length) MSG_INTRA;
        pMsg->SenderPid   = GetProcessType(); 
        pMsg->SenderMid   = GetModuleType();
        pMsg->ReceiverPid = PT_CLSTR;
        pMsg->ReceiverMid = MT_CLSTR;
        pMsg->CmdCode     = CMD_ASK_FAILOVER;
   
        memcpy(pMsg->Data, m_pFailOverCause, pMsg->Len);
        SendMsg(pMsg);
   
        break;

    case CLSTRINT_DEACTIVING:
        //����ģ�鷢��ȥ��������
        DeActive();

        break;
	
    case CLSTRINT_ACTIVING:
        //����ģ�鷢�ͼ�������
        Active();

        break;

    case CLSTRINT_DEACTIVE:
        //����ģ�鷢���˳�����
        Shutdown();

        break;

    default:
	    break;
    }
}
