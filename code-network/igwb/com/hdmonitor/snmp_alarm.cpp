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
//  函数名:     CSnmpAlarm::TrapExecutionThread（）
//  功能:       执行服务线程
//  输入参数:   无
//  输出参数:   无
//  调用函数：  trap（）
//  返回值:     void *
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
//  函数名:     CSnmpAlarm::StartRcvThread（）
//  功能:       启动接收线程
//  输入参数:   无
//  输出参数:   无
//  调用函数：  
//  返回值:     BOOL
//              FALSE 表明线程启动不成功
//              TRUE  表明线程启动成功
//----------------------------------------------------
bool CSnmpAlarm::StartRcvThread(const char *szCfgPath,int nPort)
{	
    //赋值端口号
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

    //初始化关键字配置文件(全路径)      
    sprintf(szPath, "%s%s", 
            szPath, S_SNMP_HD_ALARM_FILE);

    m_AlarmProperty.SetCfgDirPath(szPath);


    //创建接收TRAP线程
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
//  函数名:     CSnmpAlarm::ExitRcvThread（）
//  功能:       退出接收线程（包括Trap线程和Schedule线程）
//  输入参数:   无
//  输出参数:   无
//  调用函数：  无
//  返回值:     BOOL
//----------------------------------------------------
bool CSnmpAlarm::ExitRcvThread()
{
    if(!CHardwareAlarm::ExitRcvThread())
    {
        return false;
    }
    
	if (m_nTrapRunning)
	{
		m_IsExit = true; //设置退出线程标志

	    //关闭m_hTrapThread句柄
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
**	产品名：	SoftX3000
**	模块名：	接收TRAP模块
**	文件名：	receivetrap.cpp
**	描述信息：	实现TRAP PDU的接收及解析
**	作者：		郑利锋（34756）
**	版本说明：	V100R001
**	版权信息：	华为技术有限公司
**	创建时间：	2002.12.10
**	修改记录：	
**	1、作者：郑利锋	
**	   日期：2003.04.04
**	   修改说明：增加HP服务器硬盘监控功能
**					
**	2、作者：郑利锋
**     日期：2003.04.22
**     修改说明：增加DELL服务器电源监控功能
**
********************************************************************/
//记录SNMP会话状态
//---------------------------------------------------
//  函数名:     trap_callback
//  功能:       收到trap时的回调函数
//  输入参数:   int reason , Snmp *session ,
//              Pdu &pdu, SnmpTarget &target,
//              void *cbd
//  输出参数:   无
//  调用函数：  GetId（）；
//              WriteToWarn();
//  返回值:     void 
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

    GenAddress address;         //trap来源IP地址
    Oid trapid;                //trap oid 
    Vb vb;                     //trap pdu 中的变量绑定数据
    char Specid[50];      //trap oid 字符串
    SWarnData TempWarnData;   //构造的上报帧
    int len = 1;             //上报帧中，PARAS的长度
    unsigned char value = 0;          //记录get_vb_value()字符对应整数值
    char statusexplain[60];  //HP硬盘状态解释字符串
    CSnmpAlarm* pAlarm = (CSnmpAlarm*)g_pHDAlarm;
    
    if ( reason == SNMP_CLASS_NOTIFICATION) 
    {	
        //获得trapid  
        pdu.get_notify_id(trapid);
        
        //如果TRAP OID长度大于50字节，一定不是所需的OID，直接返回。
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
        
        //判断是否是需要的trap
        if( pAlarmProperty->ReadCng(Specid) == 1)
        {		    
            memset(&TempWarnData, 0, sizeof(SWarnData));
            
            //填充告警帧		
            TempWarnData.AlarmId     = (unsigned short) pAlarmProperty->GetAlarmId();            //上报进程ID
            TempWarnData.AlarmType   = (unsigned char) pAlarmProperty->GetAlarmType();         //故障类型
            TempWarnData.Paras[0]    = BAM;
            //判断是否是IBM 硬盘故障
            switch (pAlarmProperty->GetMachineType())
            {
                //ibm 磁盘阵列故障
            case 0:
                
                {
                    for( int z=0 ; z<pdu.get_vb_count() ; z++) 
                    {
                        pdu.get_vb(vb,z);
                        
                        //获取服务器名，参数长度为SERVER_NAME_LEN个字节。
                        if( z == 0 )
                        { 
                            memcpy(&TempWarnData.Paras[len] , vb.get_printable_value() , SERVER_NAME_LEN);
                            len += SERVER_NAME_LEN;
                            
                        }//end if( z == 0 )
                        
                        else if ( (z == 3) && ( (TempWarnData.AlarmId == 29004)||(TempWarnData.AlarmId == 29006) ) )
                        {
                            //丢弃逻辑驱动器重建失败和磁盘同步失败时的错误代码参数
                            //以保证参数个数的一致性。
                        }
                        
                        //获取上报参数，参数格式为UINT。
                        else
                        {
                            value =(unsigned char) atoi(vb.get_printable_value());
                            
                            TempWarnData.Paras[len] = value;
                            len += 1;
                            
                        }//end else
                    }//end for( int z=0 ; z<pdu.get_vb_count() ; z++) 
                }// end case 0
                break;
                
                //dell 磁盘阵列故障
            case 1:					
                {
                    //读出服务器的IP地址
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
                
                //HP 磁盘柜故障		
            case 2:
                {
                    //物理硬盘故障
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
                                //获取状态值
                                memset(statusexplain, 0, 60);
                                value =(unsigned char) atoi(vb.get_printable_value());
                                
                                //解释硬盘状态
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
                                
                                //将硬件状态写入告警帧参数
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
                    
                    //逻辑磁盘故障
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
                                //获取状态值
                                memset(statusexplain, 0, 60);
                                value =(unsigned char) atoi(vb.get_printable_value());
                                
                                //解释硬盘状态
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
                                
                                //将硬件状态写入告警帧参数
                                memcpy(&TempWarnData.Paras[len] , statusexplain, 
                                    HP_STATUS_MAX_LEN);
                                len += HP_STATUS_MAX_LEN;
                            }//end if( z == 2 )
                        }//end for( int z=0 ; z<pdu.get_vb_count() ; z++)
                    }//end if (pdu.get_vb_count() == 3)
                }//end 	case 3:	
                break;
                    
            //DELL服务器硬件故障
            case 3:
                {
                    for( int z=0 ; z<pdu.get_vb_count() ; z++) 
                    {
                        pdu.get_vb(vb,z);
                
                        //获取服务器名，参数长度为SERVER_NAME_LEN个字节。
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
				
            //调用writetowarn发送帧到网管程序
    
            TempWarnData.Len = (unsigned short) len;
    
            //cout << "添加告警帧到队列中..." << endl;
            pAlarm->SetAlarm(TempWarnData, len);
            //Beep(450, 150);
        }
    } 
};
//------------------------------------------------------------
//  函数名:     trap()
//  功能:       建立SNMP会话，然后处于监听状态，当监听到
//              有trap PDU 包，触发相应的处理函数。 
//  输入参数:   无
//  输出参数:   无
//  返回值:     int 
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

      ExitRcvThread(); //退出线程
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

       ExitRcvThread(); //退出线程
       return false;
   }
    
	while (!m_IsExit)
    {	
        snmp.eventListHolder->SNMPProcessEvents(30);            
    }
    return true;
}
