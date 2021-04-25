    #if defined (WIN32)
#define SNMP_PP_DLL
#endif

#include "snmp_alarm.h"

extern IHardwareAlarm* g_pHDAlarm;

CSnmpAlarm::CSnmpAlarm()
{
	m_nTrapRunning     = false;      
    m_trap_port        = 162;
	m_hTrapThread      = 0;
}

CSnmpAlarm::~CSnmpAlarm()
{
    ExitRcvThread();
    m_pGlobalFunc = NULL;
}

//---------------------------------------------------
//  ������:     CSnmpAlarm::TrapExecutionThread����
//  ����:       ִ�з����߳�
//  �������:   ��
//  �������:   ��
//  ���ú�����  trap����
//  ����ֵ:     void *
//----------------------------------------------------
void *CSnmpAlarm::TrapExecutionThread(void *param)
{
	CSnmpAlarm *This = (CSnmpAlarm *)param;
	if(This)
	{
		This->trap();
        This->m_nTrapRunning = false;
	}
    else
    {
        This->m_strError = "Param in TrapExecutionThread is NULL !";
    }
    
    
	return 0;
}

//---------------------------------------------------
//  ������:     CSnmpAlarm::StartRcvThread����
//  ����:       ���������߳�
//  �������:   ��
//  �������:   ��
//  ���ú�����  
//  ����ֵ:     BOOL
//              FALSE �����߳��������ɹ�
//              TRUE  �����߳������ɹ�
//----------------------------------------------------
bool CSnmpAlarm::StartRcvThread(const char *szCfgPath,int nPort)
{	
    //��ֵ�˿ں�
	m_trap_port = nPort; 

    if ((szCfgPath == NULL) || (szCfgPath[0] == '\0'))
    {
        return false;
    }

    char szPath[256] = ""; 
    strcpy(szPath, szCfgPath);

    int nLen = strlen(szPath);
    if((szPath[nLen - 1] != '\\') 
       && (szPath[nLen - 1] != '/'))
    {
        szPath[nLen] = '/';
        szPath[nLen + 1] = '\0';
    }

    //��ʼ���ؼ��������ļ�(ȫ·��)      
    sprintf(szPath, "%s%s", 
            szPath, S_SNMP_HD_ALARM_FILE);

    m_AlarmProperty.SetCfgDirPath(szPath);


    //��������TRAP�߳�
    int nRet =m_ThreadManager.spawn((ACE_THR_FUNC)TrapExecutionThread,
                                      (void*)this,
                                      THR_NEW_LWP | THR_JOINABLE,
                                      &m_hTrapThread);
	if(nRet < 0)
	{
        m_strError = "create TrapExecutionThread failed!";
        m_strError += ACE_OS::strerror(ACE_OS::last_error());

		m_IsExit = true;
        ExitRcvThread();
		return false;
	}

	m_nTrapRunning = true;
    
    return CHardwareAlarm::StartRcvThread(szCfgPath, nPort);
}


//---------------------------------------------------
//  ������:     CSnmpAlarm::ExitRcvThread����
//  ����:       �˳������̣߳�����Trap�̺߳�Schedule�̣߳�
//  �������:   ��
//  �������:   ��
//  ���ú�����  ��
//  ����ֵ:     BOOL
//----------------------------------------------------
bool CSnmpAlarm::ExitRcvThread()
{
    if(!CHardwareAlarm::ExitRcvThread())
    {
        return false;
    }
    
	if (m_nTrapRunning)
	{
		m_IsExit = true; //�����˳��̱߳�־

	    //�ر�m_hTrapThread���
	    if (m_hTrapThread != 0)
	    {
		    m_ThreadManager.wait();
            m_hTrapThread = 0;
	    }

        m_nTrapRunning = false;
    }
    
	return true;    
}

/*******************************************************************
**
**	��Ʒ����	SoftX3000
**	ģ������	����TRAPģ��
**	�ļ�����	receivetrap.cpp
**	������Ϣ��	ʵ��TRAP PDU�Ľ��ռ�����
**	���ߣ�		֣���棨34756��
**	�汾˵����	V100R001
**	��Ȩ��Ϣ��	��Ϊ�������޹�˾
**	����ʱ�䣺	2002.12.10
**	�޸ļ�¼��	
**	1�����ߣ�֣����	
**	   ���ڣ�2003.04.04
**	   �޸�˵��������HP������Ӳ�̼�ع���
**					
**	2�����ߣ�֣����
**     ���ڣ�2003.04.22
**     �޸�˵��������DELL��������Դ��ع���
**
********************************************************************/
//��¼SNMP�Ự״̬
//---------------------------------------------------
//  ������:     trap_callback
//  ����:       �յ�trapʱ�Ļص�����
//  �������:   int reason , Snmp *session ,
//              Pdu &pdu, SnmpTarget &target,
//              void *cbd
//  �������:   ��
//  ���ú�����  GetId������
//              WriteToWarn();
//  ����ֵ:     void 
//----------------------------------------------------
void CSnmpAlarm::trap_callback(int reason,
                               Snmp *session,
                               Pdu &pdu,
                               SnmpTarget &target,
                               void *cbd) 
{
    if(!g_pHDAlarm)
    {
        return;
    }

    GenAddress address;         //trap��ԴIP��ַ
    Oid trapid;                //trap oid 
    Vb vb;                     //trap pdu �еı���������
    char Specid[50];      //trap oid �ַ���
    SWarnData TempWarnData;   //������ϱ�֡
    int len = 1;             //�ϱ�֡�У�PARAS�ĳ���
    unsigned char value = 0;          //��¼get_vb_value()�ַ���Ӧ����ֵ
    char statusexplain[60];  //HPӲ��״̬�����ַ���
    CSnmpAlarm* pAlarm = (CSnmpAlarm*)g_pHDAlarm;
    
    if ( reason == SNMP_CLASS_NOTIFICATION) 
    {	
        //���trapid  
        pdu.get_notify_id(trapid);
        
        //���TRAP OID���ȴ���50�ֽڣ�һ�����������OID��ֱ�ӷ��ء�
        if (strlen(trapid.get_printable()) > 50)
        {
            pAlarm->m_strError = "Oid is greater than 50";
            return;
        }
        
        memset(Specid, 0, 50);
        strcpy(Specid, trapid.get_printable());
        
        CAlarmProperty *pAlarmProperty = &(pAlarm->m_AlarmProperty);
        if (pAlarmProperty->ReadCng(Specid) == -1)
        {
            pAlarm->m_strError = "Can't open the configuration file SnmpHDAlarm.ini! ";
            return ;
        }
        
        //�ж��Ƿ�����Ҫ��trap
        if( pAlarmProperty->ReadCng(Specid) == 1)
        {		    
            memset(&TempWarnData, 0, sizeof(SWarnData));
            
            //���澯֡		
            TempWarnData.AlarmId     = (unsigned short) pAlarmProperty->GetAlarmId();            //�ϱ�����ID
            TempWarnData.AlarmType   = (unsigned char) pAlarmProperty->GetAlarmType();         //��������
            TempWarnData.Paras[0]    = BAM;
            //�ж��Ƿ���IBM Ӳ�̹���
            switch (pAlarmProperty->GetMachineType())
            {
                //ibm �������й���
            case 0:
                
                {
                    for( int z=0 ; z<pdu.get_vb_count() ; z++) 
                    {
                        pdu.get_vb(vb,z);
                        
                        //��ȡ������������������ΪSERVER_NAME_LEN���ֽڡ�
                        if( z == 0 )
                        { 
                            memcpy(&TempWarnData.Paras[len] , vb.get_printable_value() , SERVER_NAME_LEN);
                            len += SERVER_NAME_LEN;
                            
                        }//end if( z == 0 )
                        
                        else if ( (z == 3) && ( (TempWarnData.AlarmId == 29004)||(TempWarnData.AlarmId == 29006) ) )
                        {
                            //�����߼��������ؽ�ʧ�ܺʹ���ͬ��ʧ��ʱ�Ĵ���������
                            //�Ա�֤����������һ���ԡ�
                        }
                        
                        //��ȡ�ϱ�������������ʽΪUINT��
                        else
                        {
                            value =(unsigned char) atoi(vb.get_printable_value());
                            
                            TempWarnData.Paras[len] = value;
                            len += 1;
                            
                        }//end else
                    }//end for( int z=0 ; z<pdu.get_vb_count() ; z++) 
                }// end case 0
                break;
                
                //dell �������й���
            case 1:					
                {
                    //������������IP��ַ
                    target.get_address( address);
                    memcpy(&TempWarnData.Paras[len], 
                        address.get_printable(), SERVER_NAME_LEN);
                    len += SERVER_NAME_LEN;
                    
                    
                    for( int z=0 ; z<pdu.get_vb_count() ; z++) 
                    {
                        pdu.get_vb(vb,z);
                        len += DELLDSK_MAX_LEN;
                        memcpy(&TempWarnData.Paras[len - DELLDSK_MAX_LEN], 
                            vb.get_printable_value(), DELLDSK_MAX_LEN);
                        
                        
                    }// end for( int z=0 ; z<pdu.get_vb_count() ; z++)
                }// end case 1
                break;
                
                //HP ���̹����		
            case 2:
                {
                    //����Ӳ�̹���
                    if (pdu.get_vb_count() == 10)
                    {
                        for( int z=0 ; z<pdu.get_vb_count() ; z++)
                        {
                            pdu.get_vb(vb,z);
                            
                            if (z == 0)
                            {
                                memcpy(&TempWarnData.Paras[len] , vb.get_printable_value() ,
                                    SERVER_NAME_LEN);
                                len += SERVER_NAME_LEN;
                                
                            }//end if( z == 0 )
                            
                            
                            else if (z == 2)
                            {
                                //��ȡ״ֵ̬
                                memset(statusexplain, 0, 60);
                                value =(unsigned char) atoi(vb.get_printable_value());
                                
                                //����Ӳ��״̬
                                switch(value)
                                {
                                case 1:
                                    strcpy(statusexplain, "need to upgrade instrument agent	and/or driver software");
                                    break;
                                    
                                case 2:
                                    strcpy(statusexplain, "ok");
                                    break;
                                    
                                case 3:
                                    strcpy(statusexplain, "failed");
                                    break;
                                    
                                case 4:
                                    strcpy(statusexplain, "PredictiveFailure");
                                    break;
                                    
                                default:
                                    break;
                                }//end switch(value)
                                
                                //��Ӳ��״̬д��澯֡����
                                memcpy(&TempWarnData.Paras[len] , statusexplain, 
                                    HP_STATUS_MAX_LEN);
                                len += HP_STATUS_MAX_LEN;
                            }//end if( z == 2 )
                            
                            else if( (z > 2) && (z < 6) )
                            {
                                value = (unsigned char) atoi(vb.get_printable_value());
                                TempWarnData.Paras[len] = value;
                                len += 1;
                            }//end else
                        }//end for( int z=0 ; z<pdu.get_vb_count() ; z++) 
                    }// end if (pdu.get_vb_count() == 10)
                    
                    //�߼����̹���
                    if (pdu.get_vb_count() == 3)
                    {
                        for( int z=0 ; z<pdu.get_vb_count() ; z++)
                        {
                            pdu.get_vb(vb,z);
                            
                            if (z == 0)
                            {
                                memcpy(&TempWarnData.Paras[len] , vb.get_printable_value() ,
                                    SERVER_NAME_LEN);
                                len += SERVER_NAME_LEN;
                            }//end if( z == 0 )
                            
                            if (z == 1)
                            {
                                //this vb is useless,ignore it, do nothing
                            }
                            
                            if (z == 2)
                            {
                                //��ȡ״ֵ̬
                                memset(statusexplain, 0, 60);
                                value =(unsigned char) atoi(vb.get_printable_value());
                                
                                //����Ӳ��״̬
                                switch(value)
                                {
                                case 1:
                                    strcpy(statusexplain, "need to upgrade instrument agent	and/or driver software");
                                    break;
                                    
                                case 2:
                                    strcpy(statusexplain, "ok");
                                    break;
                                    
                                case 3:
                                    strcpy(statusexplain, "failed");
                                    break;
                                    
                                case 4:
                                    strcpy(statusexplain, "unconfigured");
                                    break;
                                    
                                case 5:
                                    strcpy(statusexplain, "need recovering");
                                    break;
                                    
                                case 6:
                                    strcpy(statusexplain, "readyForRebuild");
                                    break;
                                    
                                case 7:
                                    strcpy(statusexplain, "rebuilding");
                                    break;
                                    
                                case 8:
                                    strcpy(statusexplain, "wrongDrive");
                                    break;
                                    
                                case 9:
                                    strcpy(statusexplain, "badConnect");
                                    break;
                                    
                                case 10:
                                    strcpy(statusexplain, "overheating");
                                    break;
                                    
                                case 11:
                                    strcpy(statusexplain, "shutdown");
                                    break;
                                    
                                case 12:
                                    strcpy(statusexplain, "expanding");
                                    break;
                                    
                                case 13:
                                    strcpy(statusexplain, "Not Available");
                                    break;
                                    
                                case 14:
                                    strcpy(statusexplain, "Queued For Expansion");
                                    break;	
                                    
                                default:
                                    break;
                                }//end switch(value)
                                
                                //��Ӳ��״̬д��澯֡����
                                memcpy(&TempWarnData.Paras[len] , statusexplain, 
                                    HP_STATUS_MAX_LEN);
                                len += HP_STATUS_MAX_LEN;
                            }//end if( z == 2 )
                        }//end for( int z=0 ; z<pdu.get_vb_count() ; z++)
                    }//end if (pdu.get_vb_count() == 3)
                }//end 	case 3:	
                break;
                    
            //DELL������Ӳ������
            case 3:
                {
                    for( int z=0 ; z<pdu.get_vb_count() ; z++) 
                    {
                        pdu.get_vb(vb,z);
                
                        //��ȡ������������������ΪSERVER_NAME_LEN���ֽڡ�
                        if( z == 0 )
                        { 
                            memcpy(&TempWarnData.Paras[len] , 
                                vb.get_printable_value() , SERVER_NAME_LEN);
                            len += SERVER_NAME_LEN;
                        }
                
                        if(z == 2)
                        {
                            pdu.get_vb(vb,z);
                            len += DELLSER_MAX_LEN;
                    
                            memcpy(&TempWarnData.Paras[len - DELLSER_MAX_LEN], 
                                vb.get_printable_value(), DELLSER_MAX_LEN);
                        }
                    }
                }
                break;
                
            default:
                break;
            }	// end switch (pAlarmProperty->GetMachineType())	
				
            //����writetowarn����֡�����ܳ���
    
            TempWarnData.Len = (unsigned short) len;
    
            //cout << "��Ӹ澯֡��������..." << endl;
            pAlarm->SetAlarm(TempWarnData, len);
            //Beep(450, 150);
        }
    } 
};
//------------------------------------------------------------
//  ������:     trap()
//  ����:       ����SNMP�Ự��Ȼ���ڼ���״̬����������
//              ��trap PDU ����������Ӧ�Ĵ������� 
//  �������:   ��
//  �������:   ��
//  ����ֵ:     int 
//-------------------------------------------------------------
bool CSnmpAlarm::trap() 
{
   //----------[ create a SNMP++ session ]-----------------------------------
   int status; 
   Snmp snmp( status, 0);                // check construction status
   if ( status != SNMP_CLASS_SUCCESS) 
   {
      m_strError = "SNMP++ Session Create Fail, ";
      m_strError += snmp.error_msg(status);

      ExitRcvThread(); //�˳��߳�
      return false;
   }

   OidCollection oidc;
   TargetCollection targetc;
   CNotifyEventQueue::set_listen_port(m_trap_port);
   status = snmp.notify_register(oidc, targetc, &(CSnmpAlarm::trap_callback), NULL);
   if (status != SNMP_CLASS_SUCCESS)
   {
       m_strError = "Error register for notify! ";
       m_strError += snmp.error_msg(status);

       ExitRcvThread(); //�˳��߳�
       return false;
   }
    
	while (!m_IsExit)
    {	
        snmp.eventListHolder->SNMPProcessEvents(30);            
    }
    return true;
}
