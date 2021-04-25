// MMLAlarmHandler.cpp: implementation of the CMMLAlarmHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "../frame/wrk_module.h"
#include "../include/toolbox.h"
#include "mml_alarm_handler.h"
#include <algorithm>
#include "snmp_agent/snmp_agent_obj.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//��������  ���캯����ɳ�Ա�����ĳ�ʼ��
//�������  BillsvrNo - ������Ϣ
//////////////////////////////////////////////////////////////////////
CMMLAlarmHandler::CMMLAlarmHandler(int BillsvrNo, CAlarmServer* pAlarmServer,
                                   const char* szAlarmShield, char *pcHisPath) 
                                   : CAlarmHandler(BillsvrNo, pAlarmServer)                                   
{
    //��ʼ����Ա����

    m_nActiveTime     = ONE_HOUR;
    m_pAlarmMMLReport = NULL;
    m_pHistoryAlarm   = NULL;
    m_pAlarmBoxByCG   = NULL; //������ �澯��
    m_pAlarmBoxByLMT  = NULL;    
    m_nTimer1Day      = 0;
    m_nPageSize       = 40;
    m_nReportCsn      = 0;
    m_nTimer1DayCount = 0;
    sHisPath          = pcHisPath;
    m_pSNMPAgent      = NULL;

	memcpy(m_szAlarmShield,szAlarmShield,128);

	m_nMMLBaseID = 100 ;
}

/////////////////////////////////////////////////////////////////////
//��������
/////////////////////////////////////////////////////////////////////
CMMLAlarmHandler::~CMMLAlarmHandler()
{
    //ɾ����ʷ�澯��
    if (m_pHistoryAlarm != NULL)
    {
        delete m_pHistoryAlarm;
        m_pHistoryAlarm = NULL;
    }

    //ɾ�����Ĺ������
    if(m_pAlarmMMLReport != NULL)
    {
        delete m_pAlarmMMLReport;
        m_pAlarmMMLReport = NULL;
    }

    //ɾ��CG�澯��
    if(m_pAlarmBoxByCG != NULL)
    {
        delete m_pAlarmBoxByCG;
        m_pAlarmBoxByCG = NULL;
    }
    
    //ɾ��LMT�澯��
    if(m_pAlarmBoxByLMT != NULL)
    {
        delete m_pAlarmBoxByLMT;
        m_pAlarmBoxByLMT = NULL;
    }    

    //ɾ��SNMP�澯����
    if(m_pSNMPAgent != NULL)
    {
        m_pSNMPAgent->Exit();
        m_pSNMPAgent = NULL;
    }

}
BOOL CMMLAlarmHandler::ChangeIntoLatesetAlarmHis()
{
    STRING szAlarmFile = sHisPath + DELIMIT_CHAR + "alarmhistory.his";
    FILE* pAlarmFile = fopen(szAlarmFile.c_str(), "rb");
    if(pAlarmFile == NULL)
    {
        return TRUE;
    }

    //ȡ��hisalarm.his�ļ���ͷ�ĸ��ַ�
    char szVer[5];
    memset(szVer,0 ,4);
    fread(szVer, sizeof(char), 4, pAlarmFile);
    szVer[4] = '\0';

    //��ȡ�ļ��İ汾����Ϣ
    UINT4 uVer = -1;
    if(ACE_OS::strncmp(szVer, "VER:", 4) != 0)//�ж��ļ���ͷ�ĸ��ַ��Ƿ�Ϊ"VER:"
    {        
        uVer = 0;//����ļ���ͷ�ĸ��ַ�����VER:����˵���ṹΪb01
    }
    else
    {
        fseek(pAlarmFile, 4, SEEK_SET);
        fread(&uVer,sizeof(UINT4), 1, pAlarmFile);  //��ȡ�ļ��еİ汾�ŵ���Ϣ
        if(uVer == LATEST_ALARM_HIS_VERSION)//���Ϊ���°汾���˳�
        {
            return TRUE;
        }
    }

    //��tmp�ļ�����д�ļ�ͷ��Ϣ
    STRING szAlarmFileTmp = sHisPath  + DELIMIT_CHAR + "alarmhistory.tmp";
    FILE* pAlarmFileTmp = fopen(szAlarmFileTmp.c_str(), "wb");
    if(pAlarmFileTmp == NULL)
    {
        fclose(pAlarmFile);
        return FALSE;
    }    

    //������ʷ�澯�ṹ���иı�
    if(!FormatChange(uVer, pAlarmFile, pAlarmFileTmp))
    {
        fclose(pAlarmFile);
        fclose(pAlarmFileTmp);
        return FALSE;
    }

    fclose(pAlarmFile);
    fclose(pAlarmFileTmp);
    
    //���ϰ汾���ļ�����ΪalarmHistory.bak����ʱ�ļ�����ΪalarmHistory.his
    STRING  szAlarmFileBak = sHisPath + DELIMIT_CHAR + "alarmHistory.bak";
    BOOL bSucc = FALSE;
    bSucc = FileMove(szAlarmFile.c_str(), szAlarmFileBak.c_str());
    bSucc = FileMove(szAlarmFileTmp.c_str(), szAlarmFile.c_str());

    return TRUE;
}

BOOL CMMLAlarmHandler::FormatChange(UINT4 uVer, FILE* pAlarmFile, FILE* pAlarmFileTmp)
{
    if(pAlarmFile == NULL)
    {
        return FALSE;
    }
    if(pAlarmFileTmp == NULL)
    {
        return FALSE;
    }

    //�ϰ汾�ļ������ڴ�
    UINT4 nAlarmFileLen = FileSize(pAlarmFile);
    BYTE* pMemAlarmFile = new BYTE[nAlarmFileLen];
    fseek(pAlarmFile, 0, SEEK_SET);
    size_t nBytesRead = fread(pMemAlarmFile, nAlarmFileLen, 1, pAlarmFile);
    if (1 != nBytesRead)
    {
        delete pMemAlarmFile;
        return FALSE;
    }
    
    UINT4 nAlareadyRead = 0;
    UINT4 nAlareadyWrite = 0;
    BYTE* pMemAlarmFileTmp = NULL;
    fseek(pAlarmFileTmp, 0, SEEK_SET);

    switch(uVer)
    {
    case 0:

        int nSubVer = -1;
        int nRecNum = -1;
        UINT4 nCsnTmp = -1;

        //��b01�汾,csn+n*SHistoryAlarmB01
        if((nAlarmFileLen - SIZEOF_ALARMHEAD) % sizeof(SHistoryAlarmB01) == 0 )          
        {            
            nRecNum = (nAlarmFileLen - SIZEOF_ALARMHEAD) / sizeof(SHistoryAlarmB01);
            nCsnTmp = *(UINT4 *)(pMemAlarmFile + nAlarmFileLen - sizeof(SHistoryAlarmB01));
           
            if(*(UINT4 *)pMemAlarmFile == nCsnTmp)
            {
                nSubVer = 0;
            }
            
        }
        //b02��OM���İ�,csn+n*SHistoryAlarm
        if((nAlarmFileLen - SIZEOF_ALARMHEAD) % sizeof(SHistoryAlarm) == 0  )
        {
            nRecNum = (nAlarmFileLen - SIZEOF_ALARMHEAD) / sizeof(SHistoryAlarm);
            nCsnTmp = *(UINT4 *)(pMemAlarmFile + nAlarmFileLen - sizeof(SHistoryAlarm)); 
            
            if(*(UINT4 *)pMemAlarmFile == nCsnTmp)
            {
                nSubVer = 1;
            }
        }
        //b02��OM���İ�,csn+syn+baseid+n*SHistoryAlarm
        if((nAlarmFileLen - SIZEOF_ALARMHEAD*3) % sizeof(SHistoryAlarm) == 0)
        {
            nRecNum = (nAlarmFileLen - SIZEOF_ALARMHEAD) / sizeof(SHistoryAlarm);  
            nCsnTmp = *(UINT4 *)(pMemAlarmFile + nAlarmFileLen - sizeof(SHistoryAlarm));
            if(*(UINT4 *)pMemAlarmFile == nCsnTmp)
            {
                nSubVer = 2;
            }
        }
        //����汾�˳�
        if(nSubVer == -1)
        {
            delete pMemAlarmFile;
            return FALSE;
        }        
        
        //��д�½ṹ���ļ�ͷ��Ϣ
        fwrite("VER:", sizeof(char), 4, pAlarmFileTmp);//��д"VER:"
        UINT4 uTmp = LATEST_ALARM_HIS_VERSION;
        fwrite(&uTmp, sizeof(UINT4), 1, pAlarmFileTmp);//��д�汾��  
        fwrite(&nCsnTmp, sizeof(UINT4), 1, pAlarmFileTmp);//csn           
        if(nSubVer != 2)
        {
            uTmp = 99;
            fwrite(&uTmp, sizeof(UINT4), 1, pAlarmFileTmp);//syn
            uTmp = 100;
            fwrite(&uTmp, sizeof(UINT4), 1, pAlarmFileTmp);//BaseID
        }
        else
        {
            uTmp = *(UINT4 *)(pMemAlarmFile + sizeof(UINT4));
            fwrite(&uTmp, sizeof(UINT4), 1, pAlarmFileTmp);//syn
            uTmp = *(UINT4 *)(pMemAlarmFile + sizeof(UINT4) + sizeof(UINT4));
            fwrite(&uTmp, sizeof(UINT4), 1, pAlarmFileTmp);//BaseID
        }

        //���ݸ澯�������������ļ����ڴ�ռ�
        pMemAlarmFileTmp = new BYTE[sizeof(SHistoryAlarm) * nRecNum];
        memset(pMemAlarmFileTmp, 0, sizeof(SHistoryAlarm) * nRecNum);
        SHistoryAlarm* pAlarmTmpHis = (SHistoryAlarm*)pMemAlarmFileTmp;  
        
        if(nSubVer == 0)
        {
            SHistoryAlarmB01* pAlarmHis = (SHistoryAlarmB01 *)(pMemAlarmFile + SIZEOF_ALARMHEAD);
            while(pAlarmHis < (SHistoryAlarmB01 *)pMemAlarmFile + nRecNum)
            {            
                pAlarmTmpHis->uSyn          = 90;                       //�澯ͬ����
                //���ݻ�澯����ʷ�澯�ṹ��ֵ
                pAlarmTmpHis->uAlarmID      = pAlarmHis->uAlarmID;             //�澯ID
                pAlarmTmpHis->uCsn          = pAlarmHis->uCsn;                 //�澯��ˮ��
                
                pAlarmTmpHis->uSYear        = pAlarmHis->uSYear;                //�澯�������ꡢ�¡���
                pAlarmTmpHis->ySMonth       = pAlarmHis->ySMonth;
                pAlarmTmpHis->ySDay         = pAlarmHis->ySDay;
                pAlarmTmpHis->ySHour        = pAlarmHis->ySHour;                //�澯������ʱ���֡���
                pAlarmTmpHis->ySMin         = pAlarmHis->ySMin;
                pAlarmTmpHis->ySSec         = pAlarmHis->ySSec;
                pAlarmTmpHis->yAlarmLevel   = pAlarmHis->yAlarmLevel;          //�澯����
                pAlarmTmpHis->yAlarmType    = pAlarmHis->yAlarmType;           //�澯����
                pAlarmTmpHis->yCPUPos       = pAlarmHis->yCPUPos;              //CPU
                pAlarmTmpHis->yFuncID       = pAlarmHis->yFuncID;              //�澯���ܺ�
                pAlarmTmpHis->yModuleNO     = pAlarmHis->yModuleNO;            //�����澯��ģ���
                pAlarmTmpHis->ySubSysID     = pAlarmHis->ySubSysID;            //�澯��ϵͳ��
                memcpy(pAlarmTmpHis->yPara, pAlarmHis->yPara, MAX_ALARM_PARA); //�澯����
                pAlarmTmpHis->yPID          = pAlarmHis->yPID;                 //�����澯�Ľ���ID
                
                memcpy(pAlarmTmpHis->szFAckOptr, pAlarmHis->szFAckOptr, sizeof(pAlarmHis->szFAckOptr));   //���ϸ澯ȷ�ϲ���Ա
                pAlarmTmpHis->uFAckYear      = pAlarmHis->uFAckYear;  
                pAlarmTmpHis->yFAckMonth		= pAlarmHis->yFAckMonth;                //�澯�ָ����ꡢ�¡���
                pAlarmTmpHis->yFAckDay	    = pAlarmHis->yFAckDay;               
                pAlarmTmpHis->yFAckHour         = pAlarmHis->yFAckHour;                 
                pAlarmTmpHis->yFAckMin        = pAlarmHis->yFAckMin;                //�澯�ָ���ʱ���֡���
                pAlarmTmpHis->yFAckSec         = pAlarmHis->yFAckSec;
                
                memcpy(pAlarmTmpHis->szRAckOptr, pAlarmHis->szRAckOptr, sizeof(pAlarmHis->szRAckOptr));   //�ָ��澯ȷ�ϲ���Ա
                pAlarmTmpHis->uRAckYear      = pAlarmHis->uRAckYear;  
                pAlarmTmpHis->yRAckMonth		= pAlarmHis->yRAckMonth;                //�澯�ָ����ꡢ�¡���
                pAlarmTmpHis->yRAckDay	    = pAlarmHis->yRAckDay;               
                pAlarmTmpHis->yRAckHour         = pAlarmHis->yRAckHour;                 
                pAlarmTmpHis->yRAckMin        = pAlarmHis->yRAckMin;                
                pAlarmTmpHis->yRAckSec         = pAlarmHis->yRAckSec;             
                
                pAlarmTmpHis->yResType      = pAlarmHis->yResType;  
                pAlarmTmpHis->uRYear		= pAlarmHis->uRYear;                
                pAlarmTmpHis->yRMonth	    = pAlarmHis->yRMonth;               
                pAlarmTmpHis->yRDay         = pAlarmHis->yRDay;                 
                pAlarmTmpHis->yRHour        = pAlarmHis->yRHour;                
                pAlarmTmpHis->yRMin         = pAlarmHis->yRMin;
                pAlarmTmpHis->yRSec         = pAlarmHis->yRSec;          
                
                //��ÿһ���澯��copy���µ��ڴ��ļ���
                pAlarmHis = pAlarmHis + sizeof(SHistoryAlarmB01);
                pAlarmTmpHis = pAlarmTmpHis + sizeof(SHistoryAlarm);                
            }
        }
        if(nSubVer == 1)
        {
            memcpy(pMemAlarmFileTmp, pMemAlarmFile+SIZEOF_ALARMHEAD, sizeof(SHistoryAlarm)*nRecNum);
        }
        if(nSubVer == 2)
        {
            memcpy(pMemAlarmFileTmp, pMemAlarmFile+SIZEOF_ALARMHEAD*3, sizeof(SHistoryAlarm)*nRecNum);
        }
        
        fseek(pAlarmFileTmp, 4 + SIZEOF_ALARMHEAD*4, SEEK_SET);
        fwrite(pMemAlarmFileTmp, sizeof(SHistoryAlarm) * nRecNum, 1, pAlarmFileTmp);
        break;
    }   

    if(pMemAlarmFileTmp != NULL)
    {
        delete pMemAlarmFileTmp;
    }
    delete pMemAlarmFile;
    return TRUE;
}

////////////////////////////////////////////////////////////////////
//��������  �ָ���ʷ�澯���е����й��ϸ澯
//�������  ��
//�������  ��
//����ֵ    �ɹ�����0,���򷵻ط�0
////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::RestoreAllFaultAlarm()
{
    if(m_pHistoryAlarm->Open() != ERR_SUCCESS)
    {
        return ERR_OPEN_HISTORY_ALARM;
    }
    
    //���ļ��ж�ȡ���и澯��¼��Ϣ,�����ȡ�����򷵻�-1
    SInnerAlarm InnerAlarm;
    SActiveAlarm *pActiveAlarm   = NULL;
    SHistoryAlarm *pHistoryAlarm = NULL;

    UINT4 uNum = m_pHistoryAlarm->GetRecordNum();
    if (m_pHistoryAlarm->Seek(0) == ERR_READ_HISTORY_ALARM)
    {
        TRACE(MTS_ALARM,S_ALARM_SEEK_HIS_FAIL);
        return ERR_FAIL;
    }

    if(uNum)
    {
        char* pBuf = new char[SIZEOF_HISTORYALARM * uNum];
        if(pBuf == NULL)
        {
            return ERR_FAIL;
        }
        if(m_pHistoryAlarm->GetRecord((SHistoryAlarm *)pBuf, uNum) != ERR_SUCCESS)
        {
            //������ 2002-04-26 ���ⵥ 12490
            delete[] pBuf;
            //�޸Ľ���
            return ERR_FAIL;
        }
        //�������и澯
        UINT4 i;
        for(i = 0; i < uNum; i++)
        {
            pHistoryAlarm = (SHistoryAlarm*)(pBuf+SIZEOF_HISTORYALARM * i);
        #ifdef _OM_MODIFY
            //��Ϊ���ϸ澯��δ�ָ�,��������
            if(AT_FAULT == pHistoryAlarm->yAlarmType
				&& NOT_RECOVER == pHistoryAlarm->yResType && pHistoryAlarm->uRYear==0)
            {
                //����Ӧ�Ĺ��ϸ澯��ΪAUTO_RECORVER(��λ�ָ�),
                //�����Զ��ָ���ʱ��Ϊ��ǰʱ��
				RECOVERY_FAULTALARM(pHistoryAlarm,m_pHistoryAlarm,RESET_RECOVER);		

                //��Ӹø澯�Ļָ��澯����澯������
                //ָ�����ʷ�澯���еĹ��ϸ澯�����������и澯��
                InnerAlarm.yAlarmType = AT_RESTORE;
                InnerAlarm.uAlarmID = pHistoryAlarm->uAlarmID;
                memcpy(InnerAlarm.yAlarmPara, pHistoryAlarm->yPara, MAX_ALARM_PARA);
				if(AddAlarm(&InnerAlarm,&pActiveAlarm,RESET_RECOVER) == ADD_OK) 
				{
					pActiveAlarm->uCsn = pHistoryAlarm->uCsn;

					SHistoryAlarm *pHistoryManualAlarm = CreateHistoryAlarm(pActiveAlarm,RESET_RECOVER);				
					m_pHistoryAlarm->AddRecord(pHistoryManualAlarm);
					m_pHistoryAlarm->WriteSyn(pHistoryManualAlarm->uSyn);

				}               
            }
        #else
            if(AT_FAULT == pHistoryAlarm->yAlarmType
                && NOT_RECOVER == pHistoryAlarm->yResType)
            {
                //����Ӧ�Ĺ��ϸ澯��ΪRESET_RECOVER(��λ�ָ�),
                //�����Զ��ָ���ʱ��Ϊ��ǰʱ��
                RECOVERY_FAULTALARM(pHistoryAlarm,m_pHistoryAlarm,RESET_RECOVER);
                
                //��Ӹø澯�Ļָ��澯����澯������
                //ָ�����ʷ�澯���еĹ��ϸ澯�����������и澯��
                InnerAlarm.yAlarmType = AT_RESTORE;
                InnerAlarm.uAlarmID = pHistoryAlarm->uAlarmID;
                memcpy(InnerAlarm.yAlarmPara, pHistoryAlarm->yPara, MAX_ALARM_PARA);
                AddAlarm(&InnerAlarm, &pActiveAlarm, RESET_RECOVER);
                //by ldf 2002.1.7
                if ( pActiveAlarm != NULL )
                {
                    pActiveAlarm->uCsn = pHistoryAlarm->uCsn;
                }  
                m_pAlarmServer->AddBinAlarm(InnerAlarm);                
            }
        #endif
        }
        delete[] pBuf;
    }

    return ERR_SUCCESS;
}
//////////////////////////////////////////////////////////////////////
//��������  ��ʼ������
//�������  ��
//�������  ��
//����ֵ    ��Ϊ0ʱ��ʾ��ʼ���ɹ�,��0Ϊ������
//////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::Init(const char *szCfgPath, int nConnectedMode, STRING szPortName)
{

    if(ERR_SUCCESS != CAlarmHandler::Init(szCfgPath))
    {
        return ERR_FAIL;
    }

    //��ʼ���澯��ʷ��
    //unit test change to 13,must recover when unit test over!
    m_pHistoryAlarm = new CHistoryAlarm(sHisPath.c_str(),ALARM_DEL_NUM,ALARM_UPPER_LIMIT);

    //����ʷ�ļ�ת��Ϊ���½ṹ����ʷ�ļ� 
    if(!ChangeIntoLatesetAlarmHis())
    {
        TRACE(MTS_ALARM, GetStringFromID(5004));
        STRING  szAlarmFile = sHisPath + DELIMIT_CHAR + "alarmHistory.his";
        STRING  szAlarmFileBak = sHisPath + DELIMIT_CHAR + "alarmHistory.bak";
        
        FileMove(szAlarmFile.c_str(), szAlarmFileBak.c_str());
        //return FALSE;
    }
    //δ�ָ��Ĺ��ϸ澯ȫ����Ϊ��λ�ָ�
    RestoreAllFaultAlarm();    

    //�������ļ��ж���"Դ��ʶ��"�ַ���
    STRING szSrcName = S_IGATEWAY_BILL;

    //���챨�Ĺ������
    m_pAlarmMMLReport = new CAlarmMMLReport(szSrcName.c_str());

    //������ �澯�����
    switch(nConnectedMode) 
    {
        
    case CONNECTED_BY_LMT:
        m_pAlarmBoxByLMT = new CAlarmBox(m_pAlarmCfg,m_pAlarmMMLReport);
        break;
    case CONNECTED_BY_CG:
        m_pAlarmBoxByCG = new CAlarmBox(m_pAlarmCfg,m_pAlarmMMLReport);
        break;
    case CONNECTED_BY_BOTH:
        m_pAlarmBoxByLMT = new CAlarmBox(m_pAlarmCfg,m_pAlarmMMLReport);
        m_pAlarmBoxByCG = new CAlarmBox(m_pAlarmCfg,m_pAlarmMMLReport);
        break;
    }
    if(m_pAlarmBoxByLMT != NULL)
    {
        m_pAlarmBoxByLMT->Init(CONNECTED_BY_LMT, szPortName); 
    }
    if(m_pAlarmBoxByCG != NULL)
    {
        m_pAlarmBoxByCG->Init(CONNECTED_BY_CG, szPortName); 
    }    

	UINT4 uMMLAlmBaseID = m_pHistoryAlarm->GetMMLBaseID();
	if(uMMLAlmBaseID != 0)
	{
		m_pAlarmMMLReport->SetMMLBaseAlarmID(uMMLAlmBaseID);   
		m_nMMLBaseID = uMMLAlmBaseID;
	}


    //��ȡSNMP�澯�������
    int nRet = ERR_SUCCESS;
    BOOL bSnmpAgent = FALSE;
    CINIFile iniFile(GetCfgFilePath());
    if(iniFile.Open())
    {
        bSnmpAgent = iniFile.GetInt(CFG_SEC_ALARM, CFG_ALARM_AS_SNMP_AGENT,
                               CFG_ALARM_AS_SNMP_AGENT_DEFAULT);
    }

    if(bSnmpAgent)
    {
        char szIpAddr[40] = CFG_ALARM_SNMP_AGENT_IP_DEFAULT;
        int  nPort = CFG_ALARM_SNMP_AGENT_PORT_DEFAULT;
        char szAlmRootDir[MAX_PATH] = CFG_ALARM_ROOT_DIR_DEFAULT;

        if(iniFile.IsOpen())
        {
            iniFile.GetString(CFG_SEC_ALARM, CFG_ALARM_SNMP_AGENT_IP,
                              CFG_ALARM_SNMP_AGENT_IP_DEFAULT, szIpAddr,
                              sizeof(szIpAddr));

            nPort = iniFile.GetInt(CFG_SEC_ALARM, CFG_ALARM_SNMP_AGENT_PORT,
                                   CFG_ALARM_SNMP_AGENT_PORT_DEFAULT);

            iniFile.GetString(CFG_SEC_ALARM, CFG_ALARM_ROOT_DIR,
                              CFG_ALARM_ROOT_DIR_DEFAULT, szAlmRootDir,
                              sizeof(szAlmRootDir));
        }

        //��ʼ��SNMP�澯�������
        m_pSNMPAgent = CSnmpAgentObj::Instance();
        if(m_pSNMPAgent != NULL)
        {
            if(ERR_SUCCESS != m_pSNMPAgent->Init((void *)this, szIpAddr,
                                                 nPort, szAlmRootDir))
            {
                TRACE(MTS_ALARM,
                      S_ALARM_SNMP_AGENT_INIT_FAIL,
                      m_pSNMPAgent->GetLastError());
                m_pSNMPAgent->Exit();
                m_pSNMPAgent = NULL;
                nRet = ERR_FAIL;
            }
        }
        else
        {
            TRACE(MTS_ALARM, S_ALARM_SNMP_AGENT_CRT_FAIL);
            nRet = ERR_FAIL;
        }

    }

    return nRet;
}
///////////////////////////////////////////////////////////////////////
//��������  �ú�����������iGateway bill�ڲ�ģ�鷢�͵ĸ澯��Ϣ
//�������  szReport - Ҫ�㲥��MML������Ϣ
///////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::BroadCastMMLReport(const char *szReport)
{
    SendMessageToMML(szReport,APP_MML_WARN,0,0);
}
///////////////////////////////////////////////////////////////////////
//��������  ���¼�������
//�������  ��
//�������  ��
//����ֵ    ��
///////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::Timer1Sec()
{
    //������澯����,������ڻ�澯����yReceivedΪAR_NOT_RECEIVED,
    //�򰴸澯�����ϱ��ķ�ʽ���ø澯ת���ɹ淶��MML����,����MML SERVER.
    //����һ���澯�󽫸ø澯��yReceived��AR_RECEIVED,��ֻ����һ��.�����
    //�¼��澯��ָ��澯,���ø澯�����ʱ�䳬��m_nActiveTime���õ�ʱ�䳤��
    //���m_ActiveAlarm����ɾ���ø澯
    SActiveAlarm *pActiveAlarm = NULL;
    CActiveAlarmList::iterator iter;
    static int nTimer1Day = 0;
    iter = m_ActiveAlarmList.begin();
    while(iter != m_ActiveAlarmList.end())//������澯����
    {
        pActiveAlarm = (*iter); //��û�澯����֡
        if(pActiveAlarm == NULL)
        {
            iter = m_ActiveAlarmList.erase(iter);
            continue;
        }
        if(pActiveAlarm->yReceived == AR_NOT_RECEIVED)
        {
            //����m_pAlarmMMLReport->MakeAlarmBroad()�ӿ�����MML�澯����
            //������BroadCastMMLReport�ӿ�������MML�ͻ��˹㲥�ø澯��Ϣ;
			m_pAlarmMMLReport->MakeAlarmBroad(pActiveAlarm,m_pAlarmCfg,pActiveAlarm->yRecoverType);
            BroadCastMMLReport(m_pAlarmMMLReport->GetReport());
            pActiveAlarm->yReceived = AR_RECEIVED;
            //������ 2002-03-12����TRACE�е���ˮ�Ÿ�Ϊ�澯����
            //������ 2002-04-24, ���ⵥD12820
            MSGOUT(MTS_ALARM,MSGOUT_LEVEL_IMPORTANT,
                   S_ALARM_SEND_ALARM,
                   pActiveAlarm->uAlarmID,
                   pActiveAlarm->yPara[0],
                   pActiveAlarm->yAlarmType);
            //����
        }
        //��Ϊ�¼���ָ��澯,�򵱸ø澯�����ʱ�䳬��m_nActiveTime����
        //��ʱ�䳤��,���m_ActiveAlarmList����ɾ���ø澯.
        BOOL bResult = FALSE;
#ifdef _OM_MODIFY
        bResult = (AT_RESTORE == pActiveAlarm->yAlarmType);
#else
        bResult = (AT_EVENT == pActiveAlarm->yAlarmType) || 
                  (AT_RESTORE == pActiveAlarm->yAlarmType);
#endif
		if(bResult)
        {

            if(Diff(pActiveAlarm) > m_nActiveTime)
            {
                iter = m_ActiveAlarmList.erase(iter);
                delete pActiveAlarm;
                continue;
            }
        }
        iter++;
    }
    //�����澯�����г�ʱ��(30��)δ�ָܻ��Ĺ��ϸ澯���ڻ�澯������
    //����30��Ĺ��ϸ澯.

    if(nTimer1Day++>ONE_DAY)
    {
        nTimer1Day = 0;
        OnTimer1Day();
    }

    //ɾ������ָ����Ŀ����ʷ�澯
    m_pHistoryAlarm->Sweep();
    //������2002-08-10
    if(m_pAlarmBoxByLMT != NULL)
    {
        m_pAlarmBoxByLMT->OnTimer(); 
    }
    if(m_pAlarmBoxByCG != NULL)
    {
        m_pAlarmBoxByCG->OnTimer(); 
    }    
}
/////////////////////////////////////////////////////////////////////////////////
//��������  �������ʱ��澯
//�������  ��
//�������  ��
//����ֵ    ��
/////////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::OnTimer1Day()
{
    CActiveAlarmList::iterator tempIter;
    tempIter = m_ActiveAlarmList.begin();
    UINT4 uNum = 1;
	SHistoryAlarm *pHistoryAlarm = NULL;

    //������澯���������еĸ澯
    while(tempIter != m_ActiveAlarmList.end())
    {
        //���Ϊ���ϸ澯�Ҹ澯����ʱ������ڵ�ǰʱ����30��
        if(Diff((*tempIter)) >= 30 * ONE_DAY
           && (AT_FAULT == (*tempIter)->yAlarmType))
        {
            SHistoryAlarm ThisAlarm;
            int Ret;
            //�����ݸ澯����ˮ�Ŵ���ʷ�澯�����ҵ���Ӧ�ĸ澯���ݽṹ
            //����yResType��Ϊ���ڻָ�,���ø澯�Ļָ�ʱ��Ϊ��ǰʱ��

            Ret = m_pHistoryAlarm->FindRecord((*tempIter)->uCsn);
            if(Ret != ERR_FAIL)
            {
                //by ldf 2004-01-10 �Զ��ָ��澯Ҳ��д�澯��־
                
                pHistoryAlarm = CreateHistoryAlarm( (*tempIter),EXPIRE_RECOVER);
                if(pHistoryAlarm == NULL)
                {
                   continue;
				}
                else
                {
                    //�����Զ��ָ��澯��־,ͬ���� �� �澯����
                    pHistoryAlarm->uSyn = m_pHistoryAlarm->CreateSyn();
                    pHistoryAlarm->yAlarmType    = AT_RESTORE;
                    pHistoryAlarm->yResType      = EXPIRE_RECOVER;                   //�Ƿ�ָ�
					
                    time_t ltime;
                    time(&ltime);
                    tm *pTime = localtime(&ltime);
                    pHistoryAlarm->uRYear		= pTime->tm_year + 1900;          //�澯�ָ����ꡢ�¡���
                    pHistoryAlarm->yRMonth	    = pTime->tm_mon + 1;        
                    pHistoryAlarm->yRDay        = pTime->tm_mday;      
                    pHistoryAlarm->yRHour       = pTime->tm_hour;              //�澯�ָ���ʱ���֡���
                    pHistoryAlarm->yRMin        = pTime->tm_min;
                    pHistoryAlarm->yRSec        = pTime->tm_sec;

                    m_pHistoryAlarm->AddRecord(pHistoryAlarm);
                    pHistoryAlarm = NULL;
                    delete pHistoryAlarm;
                }
                //end
                m_pHistoryAlarm->Seek(Ret);
                m_pHistoryAlarm->GetRecord(&ThisAlarm,uNum);
                RECOVERY_FAULTALARM(&ThisAlarm,m_pHistoryAlarm,EXPIRE_RECOVER);
                SActiveAlarm *pAlarm = (*tempIter);
                tempIter = m_ActiveAlarmList.erase(tempIter);
                delete pAlarm;
                TRACE(MTS_ALARM,S_ALARM_CLEAN_HIS);
                continue;
            }
        }
        tempIter++;
    } //while is over
}
//////////////////////////////////////////////////////////////////////////////////
//��������  ����pAlarm��ʱ�����ø澯������ʱ���뵱ǰʱ���ʱ���ֵ,����Ϊ��λ
//�������  PSActiveAlarm pAlarm
//�������  ��
//����ֵ    �뵱ǰʱ����������
//////////////////////////////////////////////////////////////////////////////////
UINT4 CMMLAlarmHandler::Diff(SActiveAlarm *pAlarm)
{
    //����pAlarm��ʱ�����ø澯������ʱ��
    tm AlarmStartTime;
    memset(&AlarmStartTime,0,sizeof(tm));
    AlarmStartTime.tm_year = pAlarm->uYear - 1900;
    AlarmStartTime.tm_mon = pAlarm->yMonth-1;//0 ��������1�·�
    AlarmStartTime.tm_mday = pAlarm->yDay;//1-31
    AlarmStartTime.tm_hour = pAlarm->yHour;
    AlarmStartTime.tm_min = pAlarm->yMin;
    AlarmStartTime.tm_sec = pAlarm->ySec;

    //��ø澯ʱ��
    time_t alm_time = mktime(&AlarmStartTime);
    //��õ�ǰʱ��
    time_t current;
    time(&current);

    //���ظ澯����ʱ���뵱ǰʱ��Ĳ�ֵ
    return  (UINT4) difftime(current,alm_time);

}
///////////////////////////////////////////////////////////////////////////
//��������  �ú�����������iGateway bill�ڲ�ģ�鷢�͵ĸ澯��Ϣ
//�������  MSG_INTRA  *pFrame
////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::HandleFrameFromApp(MSG_INTRA *pFrame)
{
	//by ldf 2004-06-03 ��Ӧ���ⵥSWPD05582
	if (pFrame->CmdCode == CMD_MODULE_ACTIVE) 
	{
		//RestoreAllFaultAlarm();
		return;
	}
    //�����Ҫ�ı���
    SActiveAlarm *pActiveAlarm = NULL;
    SHistoryAlarm *pHistoryAlarm = NULL;
    UINT4 uNum;

    if(NULL == m_pHistoryAlarm)
    {
        return;
    }

    //�����������Ӧ��ĸ澯��Ϣ��������ֱ�ӷ��أ�
    if(CMD_ALARM_RESPONSE == pFrame->CmdCode)
    {
        return;
    }
    //zyx �澯�� ���
    if(0xAA == pFrame->CmdCode &&m_pAlarmBoxByLMT != NULL)
    {  //ALARM_CMD_BAU_TO_CONVERT
        m_pAlarmBoxByLMT->HandleAlarmCmdFromBox(pFrame);
        return;
    }    

    //����澯�����������ÿ�Ĳ�����ֱ�Ӷ���
    SInnerAlarm *pInner = (SInnerAlarm*)pFrame->Data;
    SAlarmExplain *pExplain = NULL;
    pExplain = m_pAlarmCfg->GetAlarmExplain(pInner->uAlarmID);
    if(pExplain == NULL)
    {
        TRACE(MTS_ALARM,S_ALARM_IDNOTDEFINE,pInner->uAlarmID);
        return;
    }
    if(pInner->yAlarmType == AT_FAULT || pInner->yAlarmType == AT_EVENT)
    {

        if( pExplain->yAlarmType != pInner->yAlarmType)
        {
            TRACE(MTS_ALARM, S_ALARM_TYPEERROR);
            return;
        }
    }
    else if(pInner->yAlarmType == AT_RESTORE)
    {
        if(pExplain->yAlarmType == AT_EVENT)
        {
            TRACE(MTS_ALARM,S_ALARM_RESTOREERROR);
            return;
        }
    }

    SInnerAlarm* pInnerAlarm = (SInnerAlarm*)pFrame->Data;
    //����������ڽ�����ɼ����̵ĸ澯�����ڸ澯���������ӽ����Ż�ɼ����
    if(PT_KERNEL != pFrame->SenderPid &&  PT_CLSTR != pFrame->SenderPid &&
        PT_OM != pFrame->SenderPid  &&  PT_CFGSVR != pFrame->SenderPid  &&
        PT_METERACC != pFrame->SenderPid && PT_BS != pFrame->SenderPid)
    {
        //�ȼ������������ɼ���
        BYTE yPID;
        if(pFrame->SenderPid >= 50)
        {
            yPID = pFrame->SenderPid - PT_CO_BASE + 1;
        }
        else
        {
            yPID = pFrame->SenderPid - PT_AP_BASE + 1;
        }
        
        //�ڸ澯���������ӽ�������ɼ���
        if(pExplain->yParaNum == 0)//�޲���
        {
            pInnerAlarm->yAlarmPara[0] = yPID;
        }
        if(pExplain->yParaNum == 1)//һ������
        {            
            sprintf((char *)&pInnerAlarm->yAlarmPara[1], "%d", yPID);            
        }
        if(pExplain->yParaNum == 2)//��������
        {
            char szTmp[MAX_ALARM_PARA - 1];
            sprintf(szTmp, "%d", yPID);
            
            int nLen = strlen(szTmp);
            szTmp[nLen] = ',';
            strncpy(&szTmp[nLen + 1], (char *)&pInnerAlarm->yAlarmPara[1], MAX_ALARM_PARA - 2 - nLen);
            strncpy((char *)&pInnerAlarm->yAlarmPara[1], szTmp, MAX_ALARM_PARA - 1);
        }
    }
    //��Ϊ�ڲ��澯��Ϣ����ӵ���澯�����У������ʧ����ֱ�ӷ��أ������������
    if(AddAlarm(pInnerAlarm, &pActiveAlarm) != ADD_OK)
    {
        return;
    }

#ifdef _OM_MODIFY
	if(pActiveAlarm != NULL && pActiveAlarm->yAlarmType == AT_EVENT)
	{
		//by ldf 2004-01-12 OM����
		//��Ϊ�¼��澯���統ǰ�澯�⣬����������¼��澯��
		//������ӳɹ��������ϱ�����
		//����m_pAlarmMMLReport->MakeAlarmBroad()�ӿ�����MML�澯����
		//������BroadCastMMLReport�ӿ�������MML�ͻ��˹㲥�ø澯��Ϣ;
		m_pAlarmMMLReport->MakeAlarmBroad(pActiveAlarm,m_pAlarmCfg,pActiveAlarm->yRecoverType);
		BroadCastMMLReport(m_pAlarmMMLReport->GetReport());
		
		MSGOUT(MTS_ALARM,MSGOUT_LEVEL_IMPORTANT,
			S_ALARM_SEND_ALARM,
			pActiveAlarm->uAlarmID,
			pActiveAlarm->yPara[0],
			pActiveAlarm->yAlarmType);
		//end
	}
#endif

    //����ǹ��ϸ澯�����¼��澯���򽫴˸澯��ӵ���ʷ�澯��m_pHistoryAlarm
    //��ˢ��m_szHistoryFile���ļ�ͷ�ĸ澯��ˮ�ţ�
    if(pActiveAlarm != NULL)
    {
        pHistoryAlarm = CreateHistoryAlarm(pActiveAlarm);
        switch(pActiveAlarm->yAlarmType)
        {
        case AT_EVENT:
        case AT_FAULT:
            {
                m_pHistoryAlarm->AddRecord(pHistoryAlarm);
                m_pHistoryAlarm->WriteCsn(pHistoryAlarm->uCsn);
//#ifdef _OM_MODIFY
                m_pHistoryAlarm->WriteSyn(pHistoryAlarm->uSyn);
//#endif
                break;
            }
            //����ǻָ��澯�����ʷ�澯���в��Ҷ�Ӧ�Ĺ��ϸ澯
            //���ݸ澯��ˮ��ƥ����㷨��
            //����Ӧ�Ĺ��ϸ澯��ΪAUTO_RECORVER���Զ��ָ�����
            //���Զ��ָ���ʱ��Ϊ��ǰʱ�䡣
        case AT_RESTORE:
            {
                int Ret = m_pHistoryAlarm->FindRecord(pHistoryAlarm->uCsn);// BySyn(pHistoryAlarm->uSyn);
                
                if(-1 != Ret)
                {
#ifdef _OM_MODIFY
                    m_pHistoryAlarm->AddRecord(pHistoryAlarm);
                    m_pHistoryAlarm->WriteSyn(pHistoryAlarm->uSyn);
#endif
                    m_pHistoryAlarm->Seek(Ret);
                    uNum = 1;
                    
                    //ע���·���õ�pHistoryAlarm�Ѿ����Ϸ��Ĳ�һ���ˣ��˴���õ��Ǹù��ϸ澯
                    m_pHistoryAlarm->GetRecord(pHistoryAlarm,uNum);
                    RECOVERY_FAULTALARM(pHistoryAlarm,m_pHistoryAlarm);
                }
                break;
            }
            //������ 2002-04-12 ���ⵥD12566
        default:
            {
                break;
            }
            //����
        }
        //������ �澯�� ���
        if(m_pAlarmBoxByLMT != NULL)
        {
            m_pAlarmBoxByLMT->HandleAlarm(pActiveAlarm); 
        }
        if(m_pAlarmBoxByCG != NULL)
        {
            m_pAlarmBoxByCG->HandleAlarm(pActiveAlarm); 
        }
        
        
        if(m_pSNMPAgent != NULL)
        {
            m_pSNMPAgent->ReportAlarm(pHistoryAlarm);
        }
    }
    //���ӽ���
    delete pHistoryAlarm;
}
///////////////////////////////////////////////////////////////////////////
//��������  ���ݻ�澯������ʷ�澯
//�������  pActive - ��澯�ṹָ��
//�������  ��
//����ֵ    ��ʷ�澯�ṹָ��
///////////////////////////////////////////////////////////////////////////
SHistoryAlarm * CMMLAlarmHandler::CreateHistoryAlarm(SActiveAlarm *pActive,int  nRecoverType)
{
    //����һ����ʷ�澯�ṹ
    SHistoryAlarm *pHistory = new SHistoryAlarm;
    memset(pHistory, 0, sizeof(SHistoryAlarm));

    //���ݻ�澯����ʷ�澯�ṹ��ֵ
    pHistory->uAlarmID      = pActive->uAlarmID;             //�澯ID
    pHistory->uCsn          = pActive->uCsn;                 //�澯��ˮ��
    pHistory->uSyn          = pActive->uSyn;                 //�澯ͬ����
    pHistory->uSYear        = pActive->uYear;                //�澯�������ꡢ�¡���
    pHistory->ySMonth       = pActive->yMonth;
    pHistory->ySDay         = pActive->yDay;
    pHistory->ySHour        = pActive->yHour;                //�澯������ʱ���֡���
    pHistory->ySMin         = pActive->yMin;
    pHistory->ySSec         = pActive->ySec;
    pHistory->yAlarmLevel   = pActive->yAlarmLevel;          //�澯����
    pHistory->yAlarmType    = pActive->yAlarmType;           //�澯����
    pHistory->yCPUPos       = pActive->yCPUPos;              //CPU
    pHistory->yFuncID       = pActive->yFuncID;              //�澯���ܺ�
    pHistory->yModuleNO     = pActive->yModuleNO;            //�����澯��ģ���
    pHistory->ySubSysID     = pActive->ySubSysID;            //�澯��ϵͳ��
    memcpy(pHistory->yPara, pActive->yPara, MAX_ALARM_PARA); //�澯����
    pHistory->yPID          = pActive->yPID;                 //�����澯�Ľ���ID
    pHistory->szFAckOptr[0] = '\0';                          //���ϸ澯ȷ�ϲ���Ա
    pHistory->szRAckOptr[0] = '\0';                          //�ָ��澯ȷ�ϲ���Ա

#ifdef _OM_MODIFY
	if(pActive->yAlarmType != AT_RESTORE )
	{
	    pHistory->yResType      = NOT_RECOVER;                   //�Ƿ�ָ�
	}
	else
	{
		pHistory->yResType      = nRecoverType;                   //�Ƿ�ָ�
		
		pHistory->uRYear		= pActive->uYear;                //�澯�ָ����ꡢ�¡���
		pHistory->yRMonth	    = pActive->yMonth;               
		pHistory->yRDay         = pActive->yDay;                 
		pHistory->yRHour        = pActive->yHour;                //�澯�ָ���ʱ���֡���
		pHistory->yRMin         = pActive->yMin;
		pHistory->yRSec         = pActive->ySec;
	}
#else
    pHistory->yResType      = NOT_RECOVER; 
#endif

    return pHistory;
}

//////////////////////////////////////////////////////////////////////////////
//��������  �˺�����������MML�澯�ͻ��˵���Ϣ
//�������
//      MSG_INTRA *pFrame;
//////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::HandleFrameFromMML(MSG_INTRA *pFrame)
{
    //���MML��������
    CParseResult *pParaResult = *(CParseResult **)pFrame->Data;

    //����MML�������Ϣ
    switch (pParaResult->GetCmdCode())
    {
    // ��ѯ��ǰ�澯
	case MML_LST_ALMAF:	
        //TRACE(MTS_ALARM, S_ALARM_RECEIVE, "MML_LST_ALMFE");
        HandleCmdAlarmQuery(pFrame);
        break;

    //��ѯĳ���澯��ˮ�ŵ���ϸ��Ϣ
    case MML_LST_IGWALMINF:
        HandleCmdAlarmDetail(pFrame);
        break;

    //��ѯ��ʷ�澯�������¼��澯�͹��ϸ澯��
    case MML_LST_ALMLOG:
        //TRACE(MTS_ALARM,S_ALARM_RECEIVE,"MML_LST_ALMLOG");
        HandleCmdAlarmQueryLOG(pFrame);
        break;

    //ǿ�ƻָ���ǰ���ϸ澯
    case MML_RMV_ALMFLT:
        //TRACE(MTS_ALARM,S_ALARM_RECEIVE,"MML_RMV_ALMFLT");
        HandleCmdAlarmDelete(pFrame);
        break;

    //ɾ������ʱ���֮�����ʷ�澯
    case MML_RMV_IGWALMLOG:
        HandleCmdAlarmClear(pFrame);
        break;
        
    //�Թ��ϸ澯����ȷ��
    case MML_SET_IGWALMACK:
        HandleCmdAlarmAck(pFrame);
        if(m_pAlarmBoxByLMT != NULL)
        {
            m_pAlarmBoxByLMT->HandleMMLCmdFromClient(pFrame);
        }
        if(m_pAlarmBoxByCG != NULL)
        {
            m_pAlarmBoxByCG->HandleMMLCmdFromClient(pFrame);
        }
        break;

    //���¼��澯�ĸ澯ȷ��
    case MML_SET_IGWEVTACK:
        HandleCmdAlarmAckEVT(pFrame);
        
        if(m_pAlarmBoxByLMT != NULL)
        {
            m_pAlarmBoxByLMT->HandleMMLCmdFromClient(pFrame);
        }
        if(m_pAlarmBoxByCG != NULL)
        {
            m_pAlarmBoxByCG->HandleMMLCmdFromClient(pFrame);
        }        
        break;

    // ��ѯ��ǰ�澯(ccs)
    case MML_LST_IGWALMFE:	
        HandleCmdIGWAlarmQuery(pFrame);
        break;

    //��ѯCCS��ʷ�澯�������¼��澯�͹��ϸ澯��
    case MML_LST_IGWALMLOG:
        HandleCmdIGWAlarmQueryLOG(pFrame);
        break;
        
    //ǿ�ƻָ���ǰ���ϸ澯
    case MML_RMV_IGWALMFLT:	
        HandleCmdIGWAlarmDelete(pFrame);
        break;

    //������ �澯�����
    case MML_LST_BOXLGT:
    case MML_LST_BOXVER:
    case MML_CLR_BOXLGT:
    case MML_STP_BOXSND:
    case MML_RST_ALMBOX:
        if(m_pAlarmBoxByLMT != NULL)
        {
            m_pAlarmBoxByLMT->HandleMMLCmdFromClient(pFrame);
        }
        if(m_pAlarmBoxByCG != NULL)
        {
            m_pAlarmBoxByCG->HandleMMLCmdFromClient(pFrame);
        }              
        break;
	//by ldf 2004-01-09 OM����
    case MML_CHK_ALM:
		HandleCmdAlarmCHK(pFrame);
		break;
        
    case MML_LST_ALM:
        HandleCmdLstAlm(pFrame);
        break;
        
    case MML_SET_ALMSHIELD:
        HandleCmdSetShield(pFrame);
        break;
        
    //����Է��ز�ѯ�޽��
    default:
        //TRACE(MTS_ALARM,S_ALARM_NO_RESULT);
        HandleCmdDefault(pFrame);
        break;
    }

    delete pParaResult;
}

/////////////////////////////////////////////////////////////////////////////////
//��������  ����ǰ�澯��ѯ����,�������ӦLST ALMAF
//�������
//       pFrame : �ڲ�����֡
/////////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::HandleCmdAlarmQuery(MSG_INTRA *pFrame)
{
    //MML�������ɵ�ǰ�ڹ���
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);
	
    int nRecCnt      = 0;   //�鵽�ļ�¼��
    int nTempCount   = 0;   //���һҳ���ܳ���40����¼���м���
    int nCurrentPage = 0;   //���ڼ�¼ҳ��
    STRING  *pMMLPages;     //������ɵ��ڶ�MML����
	
    char szTmpOptr[78];
    memset(szTmpOptr, 0, 78);
	
    m_nReportCsn ++;       //ÿ�α�����ˮ�ţ�1
	
    int nRetCount;
	
	int nAlarmSSSN = 0;
	int nAlarmESSN = 0;
	int nAlarmSCSN = 0;
	int nAlarmECSN = 0;
	
    //ȡ��һ��MML������sssn(��ʼͬ����)
    if (pParaResult->GetDigitVal(&nAlarmSSSN, 1, 1) == FALSE)
    {
		nAlarmSSSN = -1;    //ȱʡ��ѯ���и澯
    }
	
	//ȡ�ڶ���MML������essn(��ֹͬ����)
    if (pParaResult->GetDigitVal(&nAlarmESSN, 2, 1) == FALSE)
    {
		nAlarmESSN = -1;    //ȱʡ��ѯ���и澯
    }
	
	//ȡ������MML������scsn(��ʼ��ˮ��)
    if (pParaResult->GetDigitVal(&nAlarmSCSN, 3, 1) == FALSE)
    {
		nAlarmSCSN = -1;    //ȱʡ��ѯ���и澯
    }
	
	//ȡ���ĸ�MML������ecsn(��ֹ��ˮ��)
    if (pParaResult->GetDigitVal(&nAlarmECSN, 4, 1) == FALSE)
    {
		nAlarmECSN = -1;    //ȱʡ��ѯ���и澯
    }
	
    //ȡ��ʮ��MML����������������ȱʡΪ64��
    if (pParaResult->GetDigitVal(&nRetCount, 10, 1) == FALSE)
    {
        nRetCount = 64;    //ȱʡ����64����
    }
    if(nRetCount == 0 || nRetCount >= 600)
    {
        nRetCount = 600;
    }
	
    LIST<SHistoryAlarm *> TmpList;
    UINT4 nTmpCnt = m_pHistoryAlarm->GetRecordNum();
    SHistoryAlarm *pHistoryArray = new SHistoryAlarm[nTmpCnt];
    m_pHistoryAlarm->Seek(0);
	
    if(m_pHistoryAlarm->GetRecord(pHistoryArray,nTmpCnt) != ERR_SUCCESS || nTmpCnt == 0)
    {
        delete[] pHistoryArray;
        HandleCmdDefault(pFrame);
        return;
    }
	
    for(int i = nTmpCnt - 1; (i >= 0) && (nRecCnt < nRetCount); i--)
    {
		//by ldf 2004-01-12 OM����
		//��ֻ���Ϊ�ָ����ϸ澯����ӡ� pHistoryArray[i].uRYear==0 ������Ϊ��ʷ�澯�⣨���澯��־����
		//�й��ϸ澯��־����yResTypeҲΪNOT_RECOVER
		if(pHistoryArray[i].yAlarmType == AT_FAULT && pHistoryArray[i].yResType == NOT_RECOVER && pHistoryArray[i].uRYear==0 )
		{
			if( (nAlarmSSSN == -1 || pHistoryArray[i].uSyn + (m_nMMLBaseID-100) >= nAlarmSSSN) 
				&& (nAlarmESSN == -1 || pHistoryArray[i].uSyn + (m_nMMLBaseID-100) <= nAlarmESSN) 
				&&  (nAlarmSCSN == -1 || pHistoryArray[i].uCsn >= nAlarmSCSN) 
				&&  (nAlarmECSN == -1 || pHistoryArray[i].uCsn <= nAlarmECSN) )
			{
				nRecCnt++;
				TmpList.push_back(&pHistoryArray[i]);
			}
		}
	}//for ѭ������
	
	//���������Ϊ��,�����HandleCmdDefault()���Ͳ����޽��
	if (0 == TmpList.size())
	{
		delete[] pHistoryArray;
		HandleCmdDefault(pFrame);
		return;
	}
	
    //��������в�Ϊ�գ�����ݲ�ѯ�ṹ�γ�MML����
	
    //by ldf 2002-10-29 ��Ӧ���ⵥ D18685
    int nMMLPageNum = nRecCnt/m_nPageSize +1 ;
    pMMLPages = new STRING[nMMLPageNum];   //���㷵�ص�MML������Ҫ��ҳ��
    //�޸����
    nTempCount   = 0;
    nRecCnt      = 0;
    nCurrentPage = 0;
    //ActiveIter = TmpList.begin();
    LIST<SHistoryAlarm *>::iterator Iter = TmpList.begin();
    SHistoryAlarm* pHistoryAlarm = NULL;
	
    while (Iter != TmpList.end())
    {
        //����һҳMML����ͷ
        nCurrentPage++;
        m_pAlarmMMLReport->MakeReportHead();
        m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(), m_nReportCsn);
		
        //���ݲ�ѯ�������һҳMML������
        while (Iter != TmpList.end())
        {
			
            //��һ��MML�����еļ�¼������һҳMML�������ĳߴ磬���ҳ��ʾ
            if (nTempCount >= m_nPageSize) //ʵ�ַ�ҳ��ʾ
            {
                nTempCount = 0;
                if (nRecCnt < TmpList.size())
                {
                    //�����м�ҳ����"TO BE CONTINUED..."
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                    m_pAlarmMMLReport->AddString(S_ALARM_TOBECONITUE);
                    //by ldf 2002-10-29 ��Ӧ���ⵥ D18685
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                    //�޸����
                }
				
                break;
            }
			
            //�澯�������ϸ澯֮��Ŀ���
            m_pAlarmMMLReport->AddString(STR_NEWLINE);
            m_pAlarmMMLReport->MakeQueryAlarmBody((*Iter), m_pAlarmCfg);
			
            nTempCount ++;
            nRecCnt ++;
            Iter ++;
        }
		
        //by ldf 2002-10-29 ��Ӧ���ⵥ D18685
        if (nRecCnt == TmpList.size())
        {
            char szContent[32];
			
            //Ȼ���ټ��������� ��(�������=2)��������
            //S_ALARM_ALARMLIST_TOTALNUM
            SNPRINTF(szContent,32,S_ALARM_ALARMLIST_TOTALNUM,TmpList.size());
            m_pAlarmMMLReport->AddString(szContent);
            m_pAlarmMMLReport->AddString(STR_NEWLINE);
            m_pAlarmMMLReport->AddString(STR_NEWLINE);
			
            //by ldf 2002-10-30 ��Ӧ���ⵥ D18685 �淶��һ���޸�
            if (nMMLPageNum > 1)
            {
                //�������һҳ����  <����><�������><������>
                //S_ALARM_ALARMREPORT_TOTALNUM
                SNPRINTF(szContent,32,S_ALARM_ALARMREPORT_TOTALNUM,nMMLPageNum);
                m_pAlarmMMLReport->AddString(szContent);
                m_pAlarmMMLReport->AddString(STR_NEWLINE);
                m_pAlarmMMLReport->AddString(STR_NEWLINE);
            }
            //�޸����
        }
        //�޸����
		
        //����һҳMML����β
        m_pAlarmMMLReport->MakeReportTail();
        pMMLPages[nCurrentPage-1] = m_pAlarmMMLReport->GetReport();
    }
	
    //���η���MML�ı���
    for (int I = 0 ; I < nCurrentPage; I++)
    {
        SendMessageToMML(pMMLPages[I].c_str(), APP_MML_NORM, pFrame->IPAddr, pFrame->ClientNo);        
    }
    delete[] pHistoryArray;
    delete[] pMMLPages;
    TmpList.clear();
}


//////////////////////////////////////////////////////////////////////////////
//��������  ������ʷ�澯��ѯ����,�������ӦLST ALMLOG
//�������
//       pFrame : �ڲ�����֡
//////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::HandleCmdAlarmQueryLOG(MSG_INTRA *pFrame)
{
	//MML�������ɵ�ǰ�ڹ���   
	CParseResult *pParaResult = *((CParseResult **)pFrame->Data);//unit test change
	m_nReportCsn ++;
	
	int nRecCnt       = 0;    //�鵽�ļ�¼��	
	int nTempCount    = 0;    //����ҳ���������һҳ���ܳ���40����¼	
	int nCurrentPage  = 0;    //���ڼ�¼ҳ��	
	STRING *pMMLPages = NULL; //������ɵ��ڶ�MML����  	
    
    int j;  //ѭ������

    int nAlarmType = 0;    //�澯����

    //ȡMML�����һ���������澯���ͣ����������
    pParaResult->GetDigitVal(&nAlarmType, 1, 1);
   
    int nResFlg[3];	   //�ָ���־
     
    //ȡMML����ڶ����������ָ���־�����������
    GetIntArrayValue(pParaResult, 2, nResFlg, 3, 2);

    int nStartCsn = 0;    //��ʼ��ˮ��

    //ȡMML�����������������ʼ�澯��ˮ�ţ����������    
	//���û�е������ĸ�������������nCsnDef Ϊ 0
    int nCsnDef = pParaResult->GetDigitVal(&nStartCsn, 3, 1);
    
    int nEndCsn = 0 ;      //��ֹ��ˮ��
    
    //ȡMML������ĸ���������ֹ�澯��ˮ�ţ����������    
    pParaResult->GetDigitVal(&nEndCsn, 4, 1);

    int nStartAlarmID = 0;   //��ʼ�澯���
    
    //ȡMML����������������ʼ�澯��ţ����������   
	//���û�е��塢���������������� nAlarmIDDef Ϊ 0
    int nAlarmIDDef = pParaResult->GetDigitVal(&nStartAlarmID, 5, 1);

    int nEndAlarmID = 0;     //��ֹ�澯���
    
    //ȡMML�����������������ֹ�澯��ţ���������� 
    pParaResult->GetDigitVal(&nEndAlarmID, 6, 1);

    int nAlmLvl[5];     //�澯����
    
    //ȡMML������߸��������澯���𣨱��������
    GetIntArrayValue(pParaResult, 7, nAlmLvl, 5, 4);

    int nASS[11];       //���ܷ���
    
    //ȡMML����ڰ˸����������ܸ澯���ࣨ���������
    GetIntArrayValue(pParaResult, 8, nASS, 11, 10);
	
    tm StartDate;
    tm EndDate;
	
    //ȡMML�����9��10��11��12����������ʼ���ڡ���ֹ���ڡ���ʼʱ�䡢��ֹʱ�䣨���������
	//by ldf 2002.1.18 ���ж��Ƿ�����ʱ�䱻ָ������û����nDateDefΪ0
    int nDateDef = pParaResult->GetDigitVal(&nDateDef, 11, 1);
	if ( nDateDef != 0 )
	{
		GetDateValue(pParaResult, 9, &StartDate);
		GetDateValue(pParaResult, 10, &EndDate);
		GetTimeValue(pParaResult, 11, &StartDate);
		GetTimeValue(pParaResult, 12, &EndDate);    
	}

    int nModNum;      //ģ���
    
    //ȡMML�����13��������ģ��ţ���ѡ������ȱʡΪȫ��ģ�飩
    if (pParaResult->GetDigitVal(&nModNum, 13, 1) == FALSE)
    {
        nModNum = 255;
    }

    //LST ALMLOG����ĵ�14��19������iGateway Bill�޹أ���˲�����

    int nStartSSSN = 0;    //��ʼ��ˮ��

    //ȡMML�����20����������ʼͬ���ţ����������    
	//���û�е�20��21��������������nSSNDef Ϊ 0
    int nSSNDef = pParaResult->GetDigitVal(&nStartSSSN, 20, 1);

    //2004-07-23 ldf ��Ӧ���ⵥSWPD06736       
    int nEndESSN = 2147483646;      //��ֹ��ˮ��
    
    //ȡMML�����21����������ֹͬ���ţ����������    
    int nESNDef = pParaResult->GetDigitVal(&nEndESSN, 21, 1);
    //end

    int nRetCount = 0;   //���ؼ�¼��
    
    //ȡMML�����22�����������ؼ�¼�������������
    pParaResult->GetDigitVal(&nRetCount, 22, 1);
    if(nRetCount == 0 || nRetCount >= 1000)
	{
		nRetCount = 64;
	}
	

	//������ʷ�澯�⣬���ҷ��������ĸ澯��¼���γɲ�ѯ������У�
	LIST<SHistoryAlarm *> pFoundAlarmList;
    UINT4 uNum = m_pHistoryAlarm->GetRecordNum();
    
    if (uNum == 0)   //��ʷ�澯���в����ڸ澯��¼
    {
		HandleCmdDefault(pFrame);
        return;        
    }

	SHistoryAlarm *pHistory = new SHistoryAlarm[uNum];
	m_pHistoryAlarm->Seek(0);  //��ָ���Ƶ���ʷ��澯�ĵ�һ����¼

	char szAckOperator[78] = "";

	m_pHistoryAlarm->GetRecord(pHistory,uNum);
    
    int nParaCheck[15];

	for(j = uNum - 1; (j >= 0) && (nRecCnt < nRetCount) ;j--)
	{        
        memset(&nParaCheck, 0, 15 * sizeof(int));
 
        //�жϵ�һ����ѯ�������澯����
        switch (nAlarmType)
        {
        case 1:   //��ѯ���ϸ澯
            if (pHistory[j].yAlarmType == AT_FAULT)
            {
                nParaCheck[0] = 1;                 
            }
             
            break;
        case 2:   //��ѯ�¼��澯
            if (pHistory[j].yAlarmType == AT_EVENT)
            {
                nParaCheck[0] = 1;
            }
             
            break;
        default:
            nParaCheck[0] = 1;
            break;
        }
      
        //�жϵڶ�����ѯ�������ָ���־
        //��ѯ�ѻָ�����δ�ָ��澯
        if ((nResFlg[2] == 1) || ((nResFlg[0] == 1) && (nResFlg[1] == 1)))
        {
            nParaCheck[1] = 1;
        }
        //ֻ��ѯ�ѻָ��澯
        else if ((nResFlg[0] == 1) && (nResFlg[1] == 0))
        {
            if (pHistory[j].yResType != NOT_RECOVER)
            {
                nParaCheck[1] = 1;
            }
        }
        //ֻ��ѯδ�ָ��澯
        else if ((nResFlg[0] == 0) && (nResFlg[1] == 1))
        {
            if (pHistory[j].yResType == NOT_RECOVER) 
            {
                nParaCheck[1] = 1;
            }
        }

	
        //�жϵ�3��4����ѯ��������ʼ��ˮ�ź���ֹ��ˮ��
		//��3��4������û�и���
		if ( nCsnDef == 0 )
		{
			nParaCheck[2] = 1;
			nParaCheck[3] = 1;
		}

        if ((pHistory[j].uCsn >= nStartCsn) && (pHistory[j].uCsn <= nEndCsn))
        {
            nParaCheck[2] = 1;
            nParaCheck[3] = 1;
        }
 
        //�жϵ�5��6����ѯ��������ʼ�澯��ź���ֹ�澯���

		if ( nAlarmIDDef == 0 )
		{
			nParaCheck[4] = 1;
            nParaCheck[5] = 1;
		}

        if ((pHistory[j].uAlarmID >= nStartAlarmID) && (pHistory[j].uAlarmID <= nEndAlarmID))       
        {
            nParaCheck[4] = 1;
            nParaCheck[5] = 1;
        }
 

        //ȷ����7����ѯ�������澯����        
        if (nAlmLvl[4] == 1) //���м���
        {
            nParaCheck[6] = 1;
        } 

		if ( (nAlmLvl[pHistory[j].yAlarmLevel] == 1) 
			      && ( pHistory[j].yAlarmLevel < 5 ) 
			      && ( pHistory[j].yAlarmLevel > -1) ) 
		{
			nParaCheck[6] = 1;
		}


        //�жϵ�8����ѯ��������������(iGateway Bill�ĸ澯���ڡ�����澯��
        if ((nASS[10] == 1) || (nASS[5] == 1))
        {
            nParaCheck[7] = 1; 
        }

        //�жϵ�8,9,10,11����ѯ��������ѯ��ʼ���ڡ���ֹ���ڡ���ʼʱ�䡢��ֹʱ��
        if ( nDateDef == 0 )
		{
			nParaCheck[8] = 1;
            nParaCheck[9] = 1;
            nParaCheck[10] = 1;
            nParaCheck[11] = 1;
		}
		else
		{
			time_t  tStart, tEnd, tTarget;
			tStart = mktime(&StartDate);
			tEnd   = mktime(&EndDate);
			tm AlarmTime;
			AlarmTime.tm_year = pHistory[j].uSYear - 1900;
			AlarmTime.tm_mon  = pHistory[j].ySMonth - 1;
			AlarmTime.tm_mday = pHistory[j].ySDay;
			AlarmTime.tm_hour = pHistory[j].ySHour;
			AlarmTime.tm_min  = pHistory[j].ySMin;
			AlarmTime.tm_sec  = pHistory[j].ySSec;
			tTarget  = mktime(&AlarmTime);
			
			if ((tTarget >= tStart) && (tTarget <= tEnd)) 
			{
				nParaCheck[8] = 1;
				nParaCheck[9] = 1;
				nParaCheck[10] = 1;
				nParaCheck[11] = 1;
			}
		}

        //�жϵ�13����ѯ������ģ��� 
        if ((nModNum == 255) || (nModNum == pHistory[j].yModuleNO))
        {
            nParaCheck[12] = 1; 
        }

        //�жϵ�20��21����ѯ��������ʼ��ˮ�ź���ֹ��ˮ��
		//��20��21������û�и���
        //2004-07-23 ldf ��Ӧ���ⵥSWPD06736   
		if ( nSSNDef == 0 )
		{
			nParaCheck[13] = 1;
		}
		if ( nESNDef == 0 )
		{
			nParaCheck[14] = 1;
		}
        //end
		
        if ((pHistory[j].uSyn + (m_nMMLBaseID-100)>= nStartSSSN) 
			&& (pHistory[j].uSyn + (m_nMMLBaseID-100) <= nEndESSN))
        {
            nParaCheck[13] = 1;
            nParaCheck[14] = 1;
        }

        int nFound = 1;

        for (int i = 0; i < 15; i++)
        {
            //ֻҪһ����ѯ���������㣬����Ϊ�����澯�����ϲ�ѯ����
            if (nParaCheck[i] == 0)
            {
                nFound = 0;
                break;
            }
        }
		
        if (nFound)
        {
            nRecCnt++;
            pFoundAlarmList.push_back(&pHistory[j]);
        }
	}

   
	//���������Ϊ�գ������HandleCmdDefault���Ͳ����޽��
	if(0 == pFoundAlarmList.size())
	{
		HandleCmdDefault(pFrame);
        delete[] pHistory;
		return;
	}
	
	//��������в�Ϊ�գ�����ݲ�ѯ�ṹ�γ�MML����
    //by ldf 2002-10-29 ��Ӧ���ⵥ D18685
    int nMMLPageNum = nRecCnt/m_nPageSize +1 ;
    pMMLPages = new STRING[nMMLPageNum];
    //�޸����

	nTempCount = 0;
	nRecCnt = 0;
	nCurrentPage = 0;
	LIST<SHistoryAlarm *>::iterator Iter = pFoundAlarmList.begin();	
	
	while(Iter != pFoundAlarmList.end())
	{
		//����һҳMML����ͷ��
		nCurrentPage++;
		m_pAlarmMMLReport->MakeReportHead();
		m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(), m_nReportCsn);
		
        //�ݲ�ѯ�������һҳMML�����壻
		while(Iter != pFoundAlarmList.end())
		{  
	
			//��һ��MML�����еļ�¼������һҳMML�������ĳߴ磬���ҳ��ʾ��
			if(nTempCount >= m_nPageSize)//ʵ�ַ�ҳ��ʾ
			{ 
				nTempCount = 0;
				if(nRecCnt < pFoundAlarmList.size())
				//by ldf 2002-10-29 ��Ӧ���ⵥ D18685
                {
                    //�����м�ҳ����"TO BE CONTINUED..."
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                    m_pAlarmMMLReport->AddString(S_ALARM_TOBECONITUE);
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                }
                //�޸����
				break;
			}

			//M2000�������ϸ澯֮��Ŀ���
			m_pAlarmMMLReport->AddString(STR_NEWLINE);
			m_pAlarmMMLReport->MakeQueryAlarmBody((*Iter), m_pAlarmCfg);

			nTempCount ++;
			nRecCnt ++;
			Iter ++;  
      	 }

        //by ldf 2002-10-29 ��Ӧ���ⵥ D18685
        if (nRecCnt == pFoundAlarmList.size())
        {
            char szContent[32];

            //Ȼ���ټ��������� ��(�������=2)��������
            //S_ALARM_ALARMLIST_TOTALNUM
            SNPRINTF(szContent,32,S_ALARM_ALARMLIST_TOTALNUM,pFoundAlarmList.size());
            m_pAlarmMMLReport->AddString(szContent);
            m_pAlarmMMLReport->AddString(STR_NEWLINE);
            m_pAlarmMMLReport->AddString(STR_NEWLINE);

            //by ldf 2002-10-30 ��Ӧ���ⵥ D18685 �淶��һ���޸�
            if (nMMLPageNum > 1)
            {
                //�������һҳ����	<����><�������><������>
                //S_ALARM_ALARMREPORT_TOTALNUM
                SNPRINTF(szContent,32,S_ALARM_ALARMREPORT_TOTALNUM,nMMLPageNum);
                m_pAlarmMMLReport->AddString(szContent);
                m_pAlarmMMLReport->AddString(STR_NEWLINE);
                m_pAlarmMMLReport->AddString(STR_NEWLINE);
            }
            //�޸����
        }
        //�޸����

	
		//����һҳMML����β��
		m_pAlarmMMLReport->MakeReportTail();
		pMMLPages[nCurrentPage-1] = m_pAlarmMMLReport->GetReport();
	}
	
	//����MML�ı���
	for (int I = 0 ;I < nCurrentPage; I++)
	{
        SendMessageToMML(pMMLPages[I].c_str(),APP_MML_NORM,pFrame->IPAddr, pFrame->ClientNo);        
	}
	
	delete[] pMMLPages;
	delete[] pHistory;
	pFoundAlarmList.clear();

}

/////////////////////////////////////////////////////////////////////////////////
//��������  �ֶ��ָ���ǰ�澯����Ӧ��RMV ALMFLT����
//�������  MSG_INTRA *pFrame
//�������  ��
//����ֵ    0 ��ʾ�ɹ�  ������ʾʧ��
/////////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::HandleCmdAlarmDelete(MSG_INTRA *pFrame)
{
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);
    m_nReportCsn ++;

    int  iResFlag      = 0;  //�Ƿ��н��
    char szTmpOptr[78] = "";

    int nStartCsn;

    //ȡMML�����һ����������ʼ�澯��ˮ�ţ����������
    pParaResult->GetDigitVal(&nStartCsn, 1, 1);

    UINT4 nRecNum = m_pHistoryAlarm->GetRecordNum();

    SActiveAlarm *pActive = NULL;

    //����Ҫ����澯���У���Ҫ������ʷ�澯��.
    SHistoryAlarm *pHistory;
    pHistory = new SHistoryAlarm[nRecNum];
    m_pHistoryAlarm->Seek(0);
    m_pHistoryAlarm->GetRecord(pHistory, nRecNum);

    //�ֹ��ָ���nStartCsn��ʼ�ĸ澯
    for (UINT4 i = 0; i < nRecNum; i++)
    {   //������ 2002-04-17 �������¹淶�޸�,���ⵥ12740 Csn >= nStartCsn ==> ==
        if ((pHistory[i].uCsn == nStartCsn)
          && (pHistory[i].yAlarmType == AT_FAULT)
          && (pHistory[i].yResType   == NOT_RECOVER))
        {

            RECOVERY_FAULTALARM(&pHistory[i], m_pHistoryAlarm, MANNUAL_RECOVER);
            SInnerAlarm sInnerAlarm;
            sInnerAlarm.uAlarmID    = pHistory[i].uAlarmID;
            sInnerAlarm.yAlarmType  = AT_RESTORE;
            memcpy(sInnerAlarm.yAlarmPara, pHistory[i].yPara, MAX_ALARM_PARA);

            //�ڵ�ǰ���������Ӹ澯
            //�ڲ�������ڴ�������,���Կ���ʹ�þֲ�����
			//��Ϊ�ڲ��澯��Ϣ����ӵ���澯�����У������ʧ����ֱ�ӷ��أ������������
#ifdef _OM_MODIFY
            if(AddAlarm(&sInnerAlarm,&pActive,MANNUAL_RECOVER) == ADD_OK) 
			{
				SHistoryAlarm *pHistoryManualAlarm = CreateHistoryAlarm(pActive,MANNUAL_RECOVER);				
				m_pHistoryAlarm->AddRecord(pHistoryManualAlarm);
				m_pHistoryAlarm->WriteSyn(pHistoryManualAlarm->uSyn);
			}
#else
            m_ActiveAlarmList.AddAlarm(&sInnerAlarm, &pActive);            
#endif

            //by ldf 2002.1.7
            if (pActive != NULL)
            {
                pActive->uCsn = pHistory[i].uCsn;
                //������ �澯�� ���
                //m_pAlarmBox->HandleAlarm(pActive);
                if(m_pAlarmBoxByLMT != NULL)
                {
                    m_pAlarmBoxByLMT->HandleAlarm(pActive); 
                }
                if(m_pAlarmBoxByCG != NULL)
                {
                    m_pAlarmBoxByCG->HandleAlarm(pActive); 
                }
            }
            iResFlag = 1;

        }
    }

    //����м�¼���ָ�����MML����
    delete[] pHistory;
    if (iResFlag == 1)
    {
        m_pAlarmMMLReport->MakeReportHead();
        m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(),m_nReportCsn);
        m_pAlarmMMLReport->AddString(STR_NEWLINE);
        m_pAlarmMMLReport->MakeRecoverAlarmBody(nStartCsn, nStartCsn, m_uBillsvrNo);
        m_pAlarmMMLReport->MakeReportTail();
        //���ڴ�ά���ڷ���һ���澯
        SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_NORM,pFrame->IPAddr, pFrame->ClientNo);
#ifndef _OM_MODIFY
        //���ڴ��ϱ��ڷ���һ���澯
        m_pAlarmMMLReport->MakeReportHead();
        m_pAlarmMMLReport->MakeRecoverAlarmBody(nStartCsn, nStartCsn, m_uBillsvrNo);
        m_pAlarmMMLReport->MakeReportTail();
        SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_WARN, pFrame->IPAddr, pFrame->ClientNo);        
#endif        

    }
    else //���û�м�¼���ָ�����ʹ��Ĭ�ϴ���
    {
        HandleCmdDefault(pFrame);
    }

    return ERR_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////
//��������  Ĭ�ϴ�������MML����Ϊ��ѯ�޽��
//�������  �ڲ���Ϣ֡
//�������  ��
//����ֵ    0 ��ʾ�ɹ�  ������ʾʧ��
/////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::HandleCmdDefault(MSG_INTRA *pFrame)
{
    //������ͱ���
    CParseResult *parse = *((CParseResult **)pFrame->Data);
    if (m_pAlarmMMLReport == NULL)
    {
        return ERR_FAIL;
    }

    m_pAlarmMMLReport->MakeReportHead();
    m_pAlarmMMLReport->MakeCmdDisplay(parse->GetCmdLine(),6);
    m_pAlarmMMLReport->AddString(S_ALARM_NO_RESULT);
    m_pAlarmMMLReport->AddString(STR_NEWLINE);
    m_pAlarmMMLReport->MakeReportTail();

    //���ڶ��ڲ����޽����ֻ��ά���ڣ�������ά���ڷ��ͱ������ݣ�
    SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_NORM, pFrame->IPAddr, pFrame->ClientNo);    

    return ERR_SUCCESS;
}
///////////////////////////////////////////////////////////////////////////////
//��������  ������ˮ��
//�������  ��
//�������  ��
//����ֵ    �½���ˮ�ţ�0��ʾ��Ч
///////////////////////////////////////////////////////////////////////////////
UINT4 CMMLAlarmHandler::CreateCsn()
{
    return m_pHistoryAlarm->CreateCsn();
}

//by ldf 2004-01-08 OM����
UINT4 CMMLAlarmHandler::CreateSyn()
{
    return m_pHistoryAlarm->CreateSyn();
}

//////////////////////////////////////////////////////////////////////////////
//��������  �ؾ���ˮ��
//�������  ��
//�������  ��
//����ֵ    �ؾ���ˮ��,0��ʾ��Ч
/////////////////////////////////////////////////////////////////////////////
UINT4 CMMLAlarmHandler::RollBackCsn()
{
    return m_pHistoryAlarm->RollBackCsn();
}

//by ldf 2004-01-08 OM����
UINT4 CMMLAlarmHandler::RollBackSyn()
{
	return m_pHistoryAlarm->RollBackSyn();
}

/////////////////////////////////////////////////////////////////////////////
//��������  ��pFrame�е���Ϣ��ȫ,�����͵�MML�ͻ���
//�������  e
//
/////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::SendMessageToMML(const char *pMMLReport,
                                        UINT2 uAppType, UINT4 uIPAddr, UINT2 uClientNo)                                        
{
    if (pMMLReport == NULL)
    {
        return;
    }
    //���㱨�ĳ��ȣ������ȷ���ռ�
    int nLength       = strlen(pMMLReport);
    MSG_INTRA *pFrame = new(nLength + 1)MSG_INTRA;
    memcpy(pFrame->Data, pMMLReport, nLength);

    //���һ���ַ�Ϊ'\0 '����ʾ�ַ�������
    pFrame->Data[nLength] = '\0';

    //��д֡ͷ����
    pFrame->IPAddr      = uIPAddr;
    pFrame->AppType     = uAppType;
    pFrame->ClientNo    = uClientNo;
    pFrame->SenderPid   = PT_OM;
    pFrame->SenderMid   = MT_ALARM;
    pFrame->ReceiverPid = PT_OUTSIDE;
    pFrame->ReceiverMid = MT_MMLDEBUG;    

    //���ÿ�ܵķ��ͺ�������Ϣ֡���͵�����ģ��ת��
    CWorkModule::SendMsg(pFrame);
}

/////////////////////////////////////////////////////////////////////////////////
//��������  ��CParseResult�еõ������Ͳ���ֵ
//�������
//      pParaResult : ��������
//      nParaID     : ����ID
//�������
//      pDate       : ��õ��������ֵ
////////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::GetDateValue(CParseResult *pParaResult, int nParaID, tm *pDate)
{
    //�����ݲ���
    if (pParaResult == NULL)
    {
        return;
    }

    pParaResult->GetDigitVal(&(pDate->tm_year), nParaID, 1);
    pDate->tm_year -= 1900;

    pParaResult->GetDigitVal(&(pDate->tm_mon), nParaID, 2);
    pDate->tm_mon -= 1;

    pParaResult->GetDigitVal(&(pDate->tm_mday), nParaID, 3);
}


//////////////////////////////////////////////////////////////////////////////////
//��������  ��CParseResult�еõ�ʱ���Ͳ���ֵ
//�������
//      pParaResult  : ��������
//      nParaID      : ����ID
//      pDefault     : ʱ��Ĭ��ֵ
//
//�������
//      pTime        : ��õ��������ֵ
/////////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::GetTimeValue(CParseResult *pParaResult, int nParaID, tm *pTime)
{
    if (pParaResult == NULL)
    {
        return;
    }

    pParaResult->GetDigitVal(&(pTime->tm_hour), nParaID, 1);
    pParaResult->GetDigitVal(&(pTime->tm_min), nParaID, 2);
    pParaResult->GetDigitVal(&(pTime->tm_sec), nParaID, 3);
}

//////////////////////////////////////////////////////////////////////////////////
//��������  ��CParseResult�еõ�����ֵ���浽��Ӧ����Ԫ����
//�������
//      pParaResult : ��������
//      paraID      : ����ID
//      array_size  : ����ߴ�
//      default_pos : Ĭ��ֵ�������е�λ��
//�������
//      pArray      : ��õ��������ֵ
///////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::GetIntArrayValue(CParseResult *pParaResult, int nParaID,
                                        int *pArray, int nArraySize, int nDefPos)
{
    int nIndex;

    //��ʼ��ö������Ϊ0
    for (nIndex = 0; nIndex < nArraySize; nIndex ++)
    {
        pArray[nIndex] = 0;
    }

    if (pParaResult == NULL)
    {
        pArray[nDefPos] = 1;
        return;
    }

    STRING szParameter;
    const char *pcParameter;
    int nPos;
    int nArrayPos;
    int nValue;

    for (nIndex = 0; nIndex < nArraySize ; nIndex ++)
    {
        pcParameter = pParaResult->GetStringVal(nParaID, nIndex + 1);
        if (strlen(pcParameter) != 0)
        //by ldf 2002.1.18
        //if ((pcParameter = pParaResult->GetStringVal(nParaID, nIndex + 1)) != NULL)
        {
            szParameter = pcParameter;
            nPos = szParameter.find("-");
            nArrayPos = atoi((szParameter.substr(0, nPos)).c_str());
            nValue = atoi((szParameter.substr(nPos + 1)).c_str());
            pArray[nArrayPos] = nValue;
        }
        else
        {
            if (0 == nIndex)
            {
                pArray[nDefPos] = 1;
                break;
            }

        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//��������  �жϸ澯���������е�ָ���澯�����Ƿ���Ч
//�������  alm_level     ����ָ���澯����
//      pAlarmLevelArray  �����澯��������
//�������  ��
//����ֵ    �澯�����ڸ澯������������Ч����1�����򷵻�0
//////////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::AlmLevelTell(UINT2 uAlmLevel, int *pAlmLevelArray)
{
    //���Ĭ��ֵ��Ч��ֱ�ӷ���1
    if(pAlmLevelArray[4] == 1)
    {
        return 1;
    }

    //���ݸ澯��������ѯ�����е�ָ��λ���Ƿ���Ч��ÿ��ָ��λ�ã�������������Ƿ�Ҫ��ѯ�������ĸ澯��1������ʾYes��0������ʾNo
    switch(uAlmLevel)
    {
    case AL_EMERGENCY:
        return pAlmLevelArray[0];
    case AL_IMPORTANT:
        return pAlmLevelArray[1];
    case AL_MINOR:
        return pAlmLevelArray[2];
    case AL_WARNING:
        return pAlmLevelArray[3];
    }
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////
//��������  �ж�SHistoryAlarm�е������Ƿ���ָ���������
//�������  pHistory     ������ʷ�澯�ṹ
//      pCommit      ����ָ�������ڽṹ
//�������  ��
//����ֵ    ����ƥ�䷵��1�����򷵻�0
///////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::AlmDateEqual(SHistoryAlarm *pHistory, tm *pCommit)
{
    //�����Ƿ���ȣ�����tm�ṹ�е�����1900��������·���0��������������ֱ������1900��1��
    if((pHistory->uSYear == pCommit->tm_year+1900)
        && (pHistory->ySMonth == pCommit->tm_mon + 1)
        && (pHistory->ySDay == pCommit->tm_mday))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
///////////////////////////////////////////////////////////////////////////////////
//��������  �ж��ƶ������Ƿ���Ч
//�������  pDate  ����ָ������
//�������  ��
//����ֵ    ��Ч����1�����򷵻�0
////////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::IsDate(tm *pDate)
{
    //��tm�ṹ�е��ꡢ�¡��ն�Ϊ��ʱ��ʾ����Чֵ
    if((pDate->tm_year != 0) && (pDate->tm_mday != 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
///////////////////////////////////////////////////////////////////////////////////
//��������  �ж�ָ��ʱ���Ƿ���Ч
//�������  pDate  - ָ��ʱ��
//�������  ��
//����ֵ    ��Ч����1 ���򷵻�0
////////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::IsTime(tm *pDate)
{
    //��tm�ṹ�е�ʱ���֡��붼Ϊ��ʱ��ʾ����Чֵ
    if((pDate->tm_hour != 0) && (pDate->tm_min != 0) && (pDate->tm_sec != 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
//////////////////////////////////////////////////////////////////////////////////
//��������  �ж���ʷ�澯�Ĳ������������ڶ�֮��
//�������
//      pHistory  : ��ʷ�澯
//      pStart    : ��ʼ����
//      pEnd      : ��������
//�������
//      ��
//����ֵ
//      ��ʷ�澯�Ĳ������������ڶ�֮�ڷ���1�����򷵻�0
/////////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::IsInDateRange(SHistoryAlarm *pHistory, tm *pStart, tm *pEnd)
{
    //������ʱ��������������ת�����ж�
    tm tmpHistory,tmpStart,tmpEnd;
    time_t history,start, end;

    memcpy(&tmpStart, pStart, sizeof(tm));
    memcpy(&tmpEnd, pEnd, sizeof(tm));

    //ת����ʷ�澯������Ϊtm�ṹ
    tmpHistory.tm_isdst = pStart->tm_isdst;
    tmpHistory.tm_year  = pHistory->uSYear-1900;
    tmpHistory.tm_mon   = pHistory->ySMonth - 1;
    tmpHistory.tm_mday  = pHistory->ySDay;

    tmpHistory.tm_hour  = tmpHistory.tm_min = tmpHistory.tm_sec = 0;
    tmpStart.tm_hour    = tmpStart.tm_min   = tmpStart.tm_sec   = 0;
    tmpEnd.tm_hour      = tmpEnd.tm_min     = tmpEnd.tm_sec     = 0;

    history = mktime(&tmpHistory);
    start   = mktime(&tmpStart);
    end     = mktime(&tmpEnd);

    //�ж���ʷ�澯�������Ƿ�����ʼ���ںͽ�������֮��
    if ((difftime(history, start) >= 0) && (difftime(end,history) >= 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

///////////////////////////////////////////////////////////////////////////
//��������  �ж���ʷ�澯�Ĳ���ʱ����ʱ���֮��
//�������  pHistory     ������ʷ�澯
//      pStart       ������ʼʱ��
//      pEnd         ��������ʱ��
//�������  ��
//����ֵ    ��ʷ�澯�Ĳ���ʱ����ʱ���֮�ڷ���1�����򷵻�0
//////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::IsInTimeRange(SHistoryAlarm *pHistory, tm *pStart, tm *pEnd)
{
       //������ʱ����������ʱ��ת�����ж�
        tm temp_history;
        time_t history,start,end;
        temp_history.tm_isdst = pStart->tm_isdst;
        //ת����ʷ�澯��ʱ�䡢��ʼʱ��ͽ���ʱ��Ϊtm�ṹ,����������1978��1��29��Ϊͬһ�Ƚ����ڵĻ�����
        temp_history.tm_mday = pHistory->ySDay;
        temp_history.tm_year = pHistory->uSYear - 1900;
        temp_history.tm_mon = pHistory->ySMonth - 1;
        temp_history.tm_hour = pHistory->ySHour;
        temp_history.tm_min = pHistory->ySMin;
        temp_history.tm_sec = pHistory->ySSec;

        //��tm�ṹת��Ϊtime_tʱ���ʱ����뷽ʽ��time_t��C���Ժ�����ı�׼ʱ����뷽ʽ
        history = mktime(&temp_history);
        start = mktime(pStart);
        end = mktime(pEnd);
        //�ж���ʷ�澯��ʱ���Ƿ�����ʼʱ��ͽ���ʱ��֮�䣬difftime��C���Ա�׼�⺯��
        if((difftime(history,start) >= 0) && (difftime(end,history) >= 0))
        {
            return 1;
        }
        else
        {
            return 0;
        }
}

void CMMLAlarmHandler::RECOVERY_FAULTALARM(SHistoryAlarm *pHistoryAlarm, CHistoryAlarm *pHistoryAlarmLib,int nRecoverType)
{
      if(pHistoryAlarmLib == NULL)
        {
            return;
        }
        //�����Զ��ָ���ʱ��Ϊ��ǰʱ��
        time_t temp;
        tm *local_time;
        time(&temp);
        local_time = localtime(&temp);
        pHistoryAlarm->uRYear = local_time->tm_year + 1900;
        pHistoryAlarm->yRMonth = local_time->tm_mon+1;
        pHistoryAlarm->yRDay = local_time->tm_mday;
        pHistoryAlarm->yRHour = local_time->tm_hour;
        pHistoryAlarm->yRMin = local_time->tm_min;
        pHistoryAlarm->yRSec = local_time->tm_sec;
        char tmpOptr[78];
        memset(tmpOptr,0,78);
#ifndef _OM_MODIFY
        pHistoryAlarm->yResType = nRecoverType; 
        //�ָ�ʱ������ϱ�ȷ�Ϲ��򽫻ָ�ȷ��Ҳ��д��
        if(memcmp(pHistoryAlarm->szFAckOptr,tmpOptr,78) != 0) 
        { 
            pHistoryAlarm->uRAckYear = local_time->tm_year + 1900; 
            pHistoryAlarm->yRAckMonth = local_time->tm_mon+1; 
            pHistoryAlarm->yRAckDay = local_time->tm_mday; 
            pHistoryAlarm->yRAckHour = local_time->tm_hour;
            pHistoryAlarm->yRAckMin = local_time->tm_min; 
            pHistoryAlarm->yRAckSec = local_time->tm_sec; 
            memcpy(pHistoryAlarm->szRAckOptr,pHistoryAlarm->szFAckOptr,78); 
        } 
#endif

    pHistoryAlarmLib->RewriteRecord(pHistoryAlarm);

}

int CMMLAlarmHandler::CFindListNodeByCsn::operator ()(SActiveAlarm *pActive)
{

    if(pActive->uCsn == m_uCsn)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

//by ldf 2004-01-12 OM����
int CMMLAlarmHandler::HandleCmdAlarmCHK(MSG_INTRA *pFrame)
{
	static BOOL bHaveBaseID = FALSE;
	
	//MML�������ɵ�ǰ�ڹ���   
	CParseResult *pParaResult = *((CParseResult **)pFrame->Data);

	
	if(!bHaveBaseID)
	{
		UINT4 uMMLAlmBaseID = m_pHistoryAlarm->GetMMLBaseID();
		
		if(uMMLAlmBaseID != 0)
		{
			m_pAlarmMMLReport->SetMMLBaseAlarmID(uMMLAlmBaseID);   
			bHaveBaseID = TRUE;
		}
		else
		{
			//ȡMML������MML�澯BaseId
			bHaveBaseID = pParaResult->GetDigitVal(&m_nMMLBaseID, 1, 1);
			//m_pHistoryAlarm->m_uMMLAlmBaseID Ϊ0ʱ��ʾCG��δ���չ�ͬ����׼ֵ
			m_pHistoryAlarm->WriteMMLBaseID(m_nMMLBaseID); //ֻCG��һ���յ���׼ֵ���¼��������Ҳ���ٸı�
			m_pAlarmMMLReport->SetMMLBaseAlarmID(m_nMMLBaseID);    //ȱʡֵM2000��׼ֵΪ 0
		}


	}
	//����һҳMML����ͷ
	m_pAlarmMMLReport->MakeReportHead();
	if(bHaveBaseID)
	{
			m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(), 0, 0);
	}
	else
	{
			m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(), 0, 1);
	}
	
	//���ݲ�ѯ�������һҳMML������
	//�澯�������ϸ澯֮��Ŀ���
	m_pAlarmMMLReport->AddString(STR_NEWLINE);

	//by ldf 2004-01-13 OM����
	m_pAlarmMMLReport->MakeReportItem(S_ALARM_NEFLAG, "",20,ALIGN_RIGHT);
	m_pAlarmMMLReport->AddString(STR_NEWLINE);
	//end
	
	//����һҳMML����β
	m_pAlarmMMLReport->MakeReportTail();
	
	//���ڶ��ڲ����޽����ֻ��ά���ڣ�������ά���ڷ��ͱ������ݣ�
    SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_NORM, pFrame->IPAddr, pFrame->ClientNo);    

	return bHaveBaseID;

}

int CMMLAlarmHandler::QueryAlmLog_Impl(SAlarmQueryCond &condition,LIST<SHistoryAlarm> &FoundAlarmList)
{
    FoundAlarmList.clear();
    UINT4 uNum = m_pHistoryAlarm->GetRecordNum();

    if(uNum == 0)   //��ʷ�澯���в����ڸ澯��¼
    {
        return 0;
    }
    int nRecCnt = 0;
    SHistoryAlarm *pHistory = new SHistoryAlarm[uNum];
    m_pHistoryAlarm->Seek(0);  //��ָ���Ƶ���ʷ��澯�ĵ�һ����¼

    char szAckOperator[78] = "";

    m_pHistoryAlarm->GetRecord(pHistory,uNum);

    int nParaCheck[14];
    int j = 0;
    for(j = uNum - 1; (j >= 0) && (nRecCnt < condition.nRetCount) ;j--)
    {
        memset(&nParaCheck, 0, 14 * sizeof(int));

        //�жϵ�һ����ѯ�������澯����
        switch(condition.nAlarmType)
        {
        case 1:   //��ѯ���ϸ澯
            if(pHistory[j].yAlarmType == AT_FAULT)
            {
                nParaCheck[0] = 1;
            }

            break;
        case 2:   //��ѯ�¼��澯
            if(pHistory[j].yAlarmType == AT_EVENT)
            {
                nParaCheck[0] = 1;
            }

            break;
        default:
            nParaCheck[0] = 1;
            break;
        }

        //�жϵڶ�����ѯ�������ָ���־
        //��ѯ�ѻָ�����δ�ָ��澯
        if((condition.nResFlg[2] == 1)
          || ((condition.nResFlg[0] == 1) && (condition.nResFlg[1] == 1)))
        {
            nParaCheck[1] = 1;
        }
        //ֻ��ѯ�ѻָ��澯
        else if((condition.nResFlg[0] == 1)
                && (condition.nResFlg[1] == 0))
        {
            if(pHistory[j].yResType != NOT_RECOVER)
            {
                nParaCheck[1] = 1;
            }
        }
        //ֻ��ѯδ�ָ��澯
        else if((condition.nResFlg[0] == 0)
                && (condition.nResFlg[1] == 1))
        {
            if(pHistory[j].yResType == NOT_RECOVER)
            {
                nParaCheck[1] = 1;
            }
        }

        //�жϵ�������ѯ������ȷ�ϱ�־
        //��ѯ��ȷ�ϻ���δȷ�ϸ澯
        if(condition.nAckFlg[2] == 1
            ||(condition.nAckFlg[0] == 1 && condition.nAckFlg[1] == 1))
        {
            nParaCheck[2] = 1;
        }
        //ֻ��ѯ��ȷ�ϸ澯
        else if((pHistory[j].szFAckOptr[0] != '\0')
                 && (condition.nAckFlg[0] == 1))
        {
             nParaCheck[2] = 1;
        }
        //ֻ��ѯδȷ�ϸ澯
        //by ldf 2002.1.18
        else if((pHistory[j].szFAckOptr[0] == '\0')
               && (condition.nAckFlg[1] == 1))
        {
             nParaCheck[2] = 1;
        }

        //�жϵ��ĸ���ѯ������ģ���
        if((condition.nModNum == 255)
           ||(condition.nModNum == pHistory[j].yModuleNO))
        {
            nParaCheck[3] = 1;
        }

        //�жϵ��塢��������ѯ��������ʼ��ˮ�ź���ֹ��ˮ��
        //���塢��������û�и���
        if(condition.nStartCsn == 0
           && condition.nEndCsn == 0)
        {
            nParaCheck[4] = 1;
            nParaCheck[5] = 1;
        }
        else if((pHistory[j].uCsn >= condition.nStartCsn)
                && (pHistory[j].uCsn <= condition.nEndCsn))
        {
            nParaCheck[4] = 1;
            nParaCheck[5] = 1;
        }

        //�жϵ��ߡ��ڰ˸���ѯ��������ʼ�澯��ź���ֹ�澯���

        if(condition.nStartAlarmID == 0 && condition.nEndAlarmID == 0)
        {
            nParaCheck[6] = 1;
            nParaCheck[7] = 1;
        }
        else if((pHistory[j].uAlarmID >= condition.nStartAlarmID)
                && (pHistory[j].uAlarmID <= condition.nEndAlarmID))
        {
            nParaCheck[6] = 1;
            nParaCheck[7] = 1;
        }


        //ȷ���ھŸ���ѯ�������澯����
        if(condition.nAlmLvl[4] == 1) //���м���
        {
            nParaCheck[8] = 1;
        }
        else if((condition.nAlmLvl[pHistory[j].yAlarmLevel] == 1)
                  && ( pHistory[j].yAlarmLevel < 5 )
                  && ( pHistory[j].yAlarmLevel > -1) )
        {
            nParaCheck[8] = 1;
        }


        //�жϵ�ʮ����ѯ��������������(iGateway Bill�ĸ澯���ڡ�����澯��
        if((condition.nASS[10] == 1) || (condition.nASS[5] == 1))
        {
            nParaCheck[9] = 1;
        }

        //�жϵ�ʮһ��ʮ����ʮ����ʮ�ĸ���ѯ��������ѯ��ʼ���ڡ���ֹ���ڡ���ʼʱ�䡢��ֹʱ��
        tm tmZero;
        memset(&tmZero,0,sizeof(tm));
        if(memcmp(&tmZero,&(condition.StartDate),sizeof(tm)) == 0
          && memcmp(&tmZero,&(condition.EndDate),sizeof(tm)) == 0)
        {
            nParaCheck[10] = 1;
            nParaCheck[11] = 1;
            nParaCheck[12] = 1;
            nParaCheck[13] = 1;
        }
        else
        {
            time_t  tStart, tEnd, tTarget;
            tStart = mktime(&(condition.StartDate));
            tEnd   = mktime(&(condition.EndDate));
            tm AlarmTime;
            AlarmTime.tm_year = pHistory[j].uSYear - 1900;
            AlarmTime.tm_mon  = pHistory[j].ySMonth - 1;
            AlarmTime.tm_mday = pHistory[j].ySDay;
            AlarmTime.tm_hour = pHistory[j].ySHour;
            AlarmTime.tm_min  = pHistory[j].ySMin;
            AlarmTime.tm_sec  = pHistory[j].ySSec;
            tTarget  = mktime(&AlarmTime);

            if ((tTarget >= tStart) && (tTarget <= tEnd))
            {
                nParaCheck[10] = 1;
                nParaCheck[11] = 1;
                nParaCheck[12] = 1;
                nParaCheck[13] = 1;
            }
        }

        int nFound = 1;

        for (int i = 0; i < 14; i++)
        {
            //ֻҪһ����ѯ���������㣬����Ϊ�����澯�����ϲ�ѯ����
            if (nParaCheck[i] == 0)
            {
                nFound = 0;
                break;
            }
        }

        if (nFound)
        {
            nRecCnt++;
            FoundAlarmList.push_back(pHistory[j]);
        }
    }
    delete[] pHistory;
    return nRecCnt;
}


//=======================================================
 /////////////////////////////////////////////////////////////////////////////////
//��������  ����ɾ����ʷ�澯�����ӦRMV ALMLOG
//�������  �ڲ�����֡
//�������  ��
//����ֵ    0 ��ʾ�ɹ�  ������ʾʧ��
/////////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::HandleCmdAlarmClear(MSG_INTRA *pFrame)
{
    //MML�������ɵ�ǰ�ڹ���
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);
    m_nReportCsn++;

    tm StartDate;
    tm EndDate;

    //ȡMML�����һ����������ʼ���ڣ����������
    GetDateValue(pParaResult, 1, &StartDate);

    //ȡMML����ڶ�����������ֹ���ڣ����������
    GetDateValue(pParaResult, 2, &EndDate);

    //�����ֹ����С����ʼ����
    if (StartDate.tm_year > EndDate.tm_year)
    {
        EndDate.tm_year = StartDate.tm_year;
        EndDate.tm_mon  = StartDate.tm_mon;
        EndDate.tm_mday = StartDate.tm_mday;
    }
    else if ((StartDate.tm_year == EndDate.tm_year)
           && (StartDate.tm_mon > EndDate.tm_mon))
    {
        EndDate.tm_mon  = StartDate.tm_mon;
        EndDate.tm_mday = StartDate.tm_mday;
    }
    else if ((StartDate.tm_year == EndDate.tm_year)
            && (StartDate.tm_mon == EndDate.tm_mon)
            && (StartDate.tm_mday > EndDate.tm_mday))
    {
        EndDate.tm_mday = StartDate.tm_mday;
    }
    //������ 2002-04-17 ���ⵥ12740�����ɾ��500���澯
    int nCnt = 0;
    //�޸Ľ���
    int nFindFlag      = 0;//��ѯ�������
    int nRet           = ERR_SUCCESS;
    char szTmpOptr[78] = "";
    int nFltAlmDelFlg = 0;
    int nEvtAlmDelFlg = 0;
    UINT4 nRecNum = m_pHistoryAlarm->GetRecordNum();
    SHistoryAlarm *pHistory = new SHistoryAlarm[nRecNum];
    m_pHistoryAlarm->Seek(0);
    m_pHistoryAlarm->GetRecord(pHistory,nRecNum);

    //ɾ����ʷ�澯�н���StartDate��EndDate֮��ĸ澯��
    //ע�⣺δ�ָ��Ĺ��ϸ澯����ɾ��
    for (UINT4 I = 0 ; I < nRecNum; I++)
    {
        if (IsInDateRange(&pHistory[I], &StartDate, &EndDate))
        {
            nFltAlmDelFlg = (pHistory[I].yAlarmType == AT_FAULT)
				             && (pHistory[I].yResType != NOT_RECOVER);				             ;
            if(pHistory[I].yAlarmType == AT_EVENT)
            {
                //���ڽ����¼��澯��δȷ�ϾͲ���ɾ��
			    if(pHistory[I].yAlarmLevel == AL_EMERGENCY)
                {   nEvtAlmDelFlg = 0; }
                else
                {   nEvtAlmDelFlg = 1; }

            }
            if(nFltAlmDelFlg || nEvtAlmDelFlg)
            {
                nFindFlag = 1;
                //ɾ����ʷ�澯
                nRet = m_pHistoryAlarm->DeleteRecord(pHistory[I].uSyn);
                if (nRet != ERR_SUCCESS)
                {
                    TRACE(MTS_ALARM, S_ALARM_DEL_HIS_ERR);
                    delete[] pHistory;
                    return ERR_FAIL;
                }

                //�����澯
                LIST<SActiveAlarm *>::iterator it;
                m_nFindNode.m_uCsn = pHistory[I].uCsn;
                it = std::find_if(m_ActiveAlarmList.begin(),
                                  m_ActiveAlarmList.end(), m_nFindNode);
                if (it != m_ActiveAlarmList.end())
                {
                    delete (*it);
                    m_ActiveAlarmList.erase(it);
                }
                nCnt ++; //������ 2002-04-17 ���ⵥ12740
            }
            //������ 2002-04-17 ���ⵥ 12740
            if(nCnt >= DEFAULT_CMD_DEL_LIMIT)
            {
                TRACE(MTS_ALARM,S_ALARM_DEL_ALM_LIMIT);
                break;
            }
            //�޸Ľ���
        }

    } //for ѭ������

    delete[] pHistory;

    //�γ�MML����
    if(nFindFlag == 1)
    {
        char sDate[30];
        sprintf(sDate,"%04d-%02d-%02d %02d:%02d:%02d",
            StartDate.tm_year + 1900,
            StartDate.tm_mon + 1,
            StartDate.tm_mday,
            StartDate.tm_hour,
            StartDate.tm_min,
            StartDate.tm_sec);

        char eDate[30] ;
        sprintf(eDate,"%04d-%02d-%02d %02d:%02d:%02d",
            EndDate.tm_year + 1900,
            EndDate.tm_mon + 1,
            EndDate.tm_mday,
            EndDate.tm_hour,
            EndDate.tm_min,
            EndDate.tm_sec);

        m_pAlarmMMLReport->MakeReportHead();
        m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(), m_nReportCsn);
        m_pAlarmMMLReport->MakeDeleteAlarmBody(sDate, eDate, m_uBillsvrNo);
        m_pAlarmMMLReport->MakeReportTail();

        //����MML����
        SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_NORM, pFrame->IPAddr, pFrame->ClientNo);        
    }
    else //û�з��������Ľ����ʹ��Ĭ�ϵĴ���
    {
        HandleCmdDefault(pFrame);
    }

    return ERR_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//��������  ������ϸ澯ȷ�������ӦSET AMLACK����
//�������
//        pFrame  : �ڲ�����֡
//�������
//        ��
//����ֵ
//        0 ��ʾ�ɹ�  ������ʾʧ��
///////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::HandleCmdAlarmAck(MSG_INTRA *pFrame)
{
    //MML�������ɵ�ǰ�ڹ���
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);//unit test
    m_nReportCsn ++;

    int  nStartCsn   = 0;       //������ 2002-04-17 ���ⵥ12743
    int  nEndCsn     = 0;       //������ 2002-04-17 ���ⵥ12743
    int  nCnt        = 0;       //������ 2002-04-17 ���ⵥ12743
    BOOL bFailAll    = TRUE;    //������ 2002-04-17 ���ⵥ12743
    BOOL bFirstPara  = TRUE;    //������ 2002-04-17 ���ⵥ12743
    BOOL bSecondPara = TRUE;    //������ 2002-04-17 ���ⵥ12743
    //���MML��һ����������ʼ��ˮ�ţ����������
    //������ 2002-04-17 ���ⵥ12743
    bFirstPara = pParaResult->GetDigitVal(&nStartCsn, 1, 1);

    //���MML�ڶ�����������ֹ��ˮ�ţ����������
    bSecondPara = pParaResult->GetDigitVal(&nEndCsn, 2, 1);
    if(bFirstPara == FALSE && bSecondPara == FALSE)
    {
        HandleCmdDefault(pFrame);
        return ERR_SUCCESS;
    }
    else if(bFirstPara == TRUE && bSecondPara == FALSE)
    {
        nEndCsn = nStartCsn;
    }
    else if(bFirstPara == FALSE && bSecondPara == TRUE)
    {
        nStartCsn = nEndCsn;
    }
    //�޸Ľ���

    if (nStartCsn > nEndCsn)
    {
        nEndCsn = nStartCsn;
    }

    UINT4 nAlarmCount = m_pHistoryAlarm->GetRecordNum();
    int nRes = 1;    //��ʾ���

    char szAckOperator[78] = "";

    struct in_addr sin;
    sin.S_un.S_addr = pFrame->IPAddr;
    char *szIP = inet_ntoa(sin);

    SHistoryAlarm *pHistory = new SHistoryAlarm[nAlarmCount];
    m_pHistoryAlarm->Seek(0);
    m_pHistoryAlarm->GetRecord(pHistory, nAlarmCount);

    //by ldf 2002-10-29 ��Ӧ���ⵥ D18685
    char AckCsn[10];
    //�޸����

    for (UINT4 i = 0; i < nAlarmCount; i++)
    {
        //ֻ����澯��ˮ������nStartCsn��nEndCsn֮��ĸ澯
        //������ 2002-04-17 ���ⵥ12743 һ��ֻ��ȷ��200��
        if ((pHistory[i].uCsn >= nStartCsn)
            && (pHistory[i].uCsn <= nEndCsn)
            && (nCnt < DEFAULT_ACK_CNT))
        {
            time_t ltime;
            time(&ltime);
            tm *pTime = localtime(&ltime);

            pHistory[i].uFAckYear  = pTime->tm_year + 1900;
            pHistory[i].yFAckMonth = pTime->tm_mon + 1;
            pHistory[i].yFAckDay   = pTime->tm_mday;
            pHistory[i].yFAckHour  = pTime->tm_hour;
            pHistory[i].yFAckMin   = pTime->tm_min;
            pHistory[i].yFAckSec   = pTime->tm_sec;
         // pHistory[i].yResType   = MANNUAL_RECOVER;  //�ֹ��ָ�

            //����ǹ��ϸ澯����û�б�ȷ��
            if ((pHistory[i].yAlarmType == AT_FAULT)
                 && (pHistory[i].szFAckOptr[0] == '\0'))
            {
                sprintf(pHistory[i].szFAckOptr, "%s:%s", pFrame->UserName, szIP);
                //����澯��ȷ��ʱ�Ѿ��ָ��������ָ�ȷ��һ�����
                if(pHistory[i].yResType != NOT_RECOVER)
                {
                    sprintf(pHistory[i].szRAckOptr, "%s:%s", pFrame->UserName, szIP);
                    pHistory[i].uRAckYear  = pTime->tm_year + 1900;
                    pHistory[i].yRAckMonth = pTime->tm_mon + 1;
                    pHistory[i].yRAckDay   = pTime->tm_mday;
                    pHistory[i].yRAckHour  = pTime->tm_hour;
                    pHistory[i].yRAckMin   = pTime->tm_min;
                    pHistory[i].yRAckSec   = pTime->tm_sec;
                }
                nRes = m_pHistoryAlarm->RewriteRecord(&pHistory[i]);
                //������ 2002-04-17 ���ⵥ12743
                if(nRes == ERR_SUCCESS)
                {
                    //by ldf 2002-10-29 ��Ӧ���ⵥ D18685
                    sprintf(AckCsn, "%d", pHistory[i].uCsn);
                    CsnAcked.push_back(AckCsn);
                    //�޸����

                    nCnt ++;
                    bFailAll = FALSE;
                }
                else
                {
                    TRACE(MTS_ALARM,S_ALARM_ACK_WRITE_ERR,pHistory[i].uCsn);
                }
                //�޸Ľ���,����������������ظ�����
            }
            //�����δȷ�ϵ�һ���¼��澯
            else if ((pHistory[i].yAlarmType    == AT_EVENT)
                  && (pHistory[i].yAlarmLevel   == AL_EMERGENCY)
                  && (pHistory[i].szFAckOptr[0] == '\0'))
            {
                sprintf(pHistory[i].szFAckOptr, "%s:%s", pFrame->UserName, szIP);
                nRes = m_pHistoryAlarm->RewriteRecord(&pHistory[i]);
                //������ 2002-04-17 ���ⵥ12743
                if(nRes == ERR_SUCCESS)
                {
                    //by ldf 2002-10-29 ��Ӧ���ⵥ D18685
                    sprintf(AckCsn, "%d", pHistory[i].uCsn);
                    CsnAcked.push_back(AckCsn);
                    //�޸����

                    nCnt ++;
                    bFailAll = FALSE;
                    //zyx ��������
                    m_ActiveAlarmList.RemoveAlarm(pHistory[i].yAlarmType,
                                                  pHistory[i].uAlarmID,
                                                  pHistory[i].yPara);
                }
                else
                {
                    TRACE(MTS_ALARM,S_ALARM_ACK_WRITE_ERR,pHistory[i].uCsn);
                }
                //�޸Ľ���
            }
            //������ 2002-04-17 ���ⵥ12743�����ڸ澯���ظ�ȷ��Ҳ��Ϊȷ�ϳɹ�
            else if((pHistory[i].yAlarmType == AT_FAULT)
                 && (pHistory[i].szFAckOptr[0] != '\0'))
            {
                bFailAll = FALSE;
                TRACE(MTS_ALARM,S_ALARM_ACK_REPEAT,pHistory[i].uCsn);
            }
            else if((pHistory[i].yAlarmType    == AT_EVENT)
                  && (pHistory[i].yAlarmLevel  == AL_EMERGENCY)
                  && (pHistory[i].szFAckOptr[0] != '\0'))
            {
                bFailAll = FALSE;
                TRACE(MTS_ALARM,S_ALARM_ACK_REPEAT,pHistory[i].uCsn);
            }
            //�޸Ľ���
        }
    }

    delete[] pHistory;
    //������ 2002-04-17 ���ⵥ12743
    if(bFailAll == FALSE)
    {
        //by ldf 2002-10-29 ��Ӧ���ⵥ D18685
        //char sCsn[10];
        //char eCsn[10];

        sprintf(szAckOperator, "%s:%s", pFrame->UserName, szIP);

        //by ldf 2002-10-29 ��Ӧ���ⵥ D18685
        //sprintf(sCsn, "%d", nStartCsn);
        //sprintf(eCsn, "%d", nEndCsn);

        //��ά������һ���澯
        m_pAlarmMMLReport->MakeReportHead();
        m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(), m_nReportCsn);
        //by ldf 2002-10-29 ��Ӧ���ⵥ D18685
        m_pAlarmMMLReport->MakeAlarmFaultAckBody(szAckOperator,S_ALARM_AT_FAULT, CsnAcked);
        //�޸����
        m_pAlarmMMLReport->MakeReportTail();
        SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_NORM, pFrame->IPAddr, pFrame->ClientNo);
        
        //���ϱ�����һ���澯
        m_pAlarmMMLReport->MakeReportHead();
        //by ldf 2002-10-29 ��Ӧ���ⵥ D18685
        m_pAlarmMMLReport->MakeAlarmFaultAckBody(szAckOperator, S_ALARM_AT_FAULT, CsnAcked);
        //�޸����
        m_pAlarmMMLReport->MakeReportTail();
        SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_WARN, pFrame->IPAddr, pFrame->ClientNo);
        
        //������ 2002-04-17 ���ⵥ12743
        if(nCnt >= DEFAULT_ACK_CNT)
        {
            TRACE(MTS_ALARM,S_ALARM_ACK_REACH_LIMIT);
        }
    }
    else
    {
        HandleCmdDefault(pFrame);
    }
    //�޸Ľ���
    return ERR_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//��������  �����¼��澯ȷ�������ӦSET EVTACK
//�������
//        pFrame  : �ڲ�����֡
//�������
//        ��
//����ֵ
//        0 ��ʾ�ɹ�  ������ʾʧ��
///////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::HandleCmdAlarmAckEVT(MSG_INTRA *pFrame)
{
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);

    int nCsn;
    tm Date;

    //���MML�����һ���������澯��ˮ�ţ����������
    pParaResult->GetDigitVal(&nCsn, 1, 1);

    //���MML����ڶ���������ȷ�����ڣ����������
    GetDateValue(pParaResult, 2, &Date);

    //����IP��ַ
    struct in_addr sin;
    sin.S_un.S_addr = pFrame->IPAddr;
    char *szIP = inet_ntoa(sin);

    int  iRes;
    iRes = m_pHistoryAlarm->FindRecord(nCsn);
    UINT4 uNum = 1;

    if (iRes == -1)  //δ�ҵ���Ӧ�ĸ澯
    {
        HandleCmdDefault(pFrame);

        return ERR_FAIL;
    }

    SHistoryAlarm sHistory;
    m_pHistoryAlarm->Seek(iRes);
    m_pHistoryAlarm->GetRecord(&sHistory, uNum);

    if( !IsDate(&Date) || sHistory.yAlarmType != AT_EVENT)
    {
        HandleCmdDefault(pFrame);
        return ERR_FAIL;
    }

    int nRes = 1;

    sprintf(sHistory.szFAckOptr, "%s:%s", pFrame->UserName, szIP);
    time_t ltime;
    time(&ltime);
    tm *pTime = localtime(&ltime);
    sHistory.uFAckYear  = pTime->tm_year + 1900;
    sHistory.yFAckMonth = pTime->tm_mon + 1;
    sHistory.yFAckDay   = pTime->tm_mday;
    sHistory.yFAckHour  = pTime->tm_hour;
    sHistory.yFAckMin   = pTime->tm_min;
    sHistory.yFAckSec   = pTime->tm_sec;

    nRes = m_pHistoryAlarm->RewriteRecord(&sHistory);
    //zyx �澯����������
    if(sHistory.yAlarmType == AT_EVENT
        && sHistory.yAlarmLevel == AL_EMERGENCY)
    {   //����ǽ����¼��澯Ҫ�Ӹ澯������ɾ��
        m_ActiveAlarmList.RemoveAlarm(sHistory.yAlarmType,sHistory.uAlarmID,sHistory.yPara);
    }
    char szCsn[11];
    sprintf(szCsn, "%d", nCsn);
    m_pAlarmMMLReport->MakeReportHead();
    m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(),m_nReportCsn);
    //by ldf 2002-10-29 ��Ӧ���ⵥ D18685
    m_pAlarmMMLReport->MakeAlarmEventAckBody(sHistory.szFAckOptr, S_ALARM_AT_EVENT, szCsn);
    //�޸����
    m_pAlarmMMLReport->MakeReportTail();

    //��ά�����ߵ�һ���澯
    SendMessageToMML(m_pAlarmMMLReport->GetReport(),APP_MML_NORM,pFrame->IPAddr, pFrame->ClientNo);
    
    //���ɴ��ϱ����ߵ�MML�澯����
    m_pAlarmMMLReport->MakeReportHead();
    //by ldf 2002-10-29 ��Ӧ���ⵥ D18685
    m_pAlarmMMLReport->MakeAlarmEventAckBody(sHistory.szFAckOptr, S_ALARM_AT_EVENT, szCsn);
    //�޸����
    m_pAlarmMMLReport->MakeReportTail();
    
    //���ϱ�����һ���澯
    SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_WARN, pFrame->IPAddr, pFrame->ClientNo);    

    return ERR_SUCCESS;
}
	
/////////////////////////////////////////////////////////////////////////////
//��������  �澯��ϸ��Ϣ��ѯ���������ӦLST ALMINF
//�������
//       pFrame : �ڲ�����֡
/////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::HandleCmdAlarmDetail(MSG_INTRA *pFrame)
{
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);//unit test

    m_nReportCsn ++;

    int nCsn;
    //ȡMML�����һ���������澯��ˮ�ţ����������
    pParaResult->GetDigitVal(&nCsn, 1, 1);

    SHistoryAlarm sHistory;
    UINT4 uNum = 1;

    //����ָ���ĸ澯��¼
    int iRes = m_pHistoryAlarm->FindRecord(nCsn);
    if (iRes != ERR_FAIL)
    {
        m_pHistoryAlarm->Seek(iRes);
        m_pHistoryAlarm->GetRecord(&sHistory, uNum);

        //���ݲ�ѯ���ļ�¼����MML����
        m_pAlarmMMLReport->MakeReportHead();
        m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(), m_nReportCsn);
        m_pAlarmMMLReport->MakeDetailAlarmBody(&sHistory, m_pAlarmCfg);
        m_pAlarmMMLReport->MakeReportTail();

        SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_NORM, pFrame->IPAddr, pFrame->ClientNo);        
    }
    else //û���ҵ���¼�Ļ���ʹ��Ĭ�ϴ���
    {
        HandleCmdDefault(pFrame);
        return;
    }
}
/////////////////////////////////////////////////////////////////////////////////
//��������	����ǰ�澯��ѯ����,�������ӦLST IGWALMFE
//�������	
//       pFrame : �ڲ�����֡
/////////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::HandleCmdIGWAlarmQuery(MSG_INTRA *pFrame)
{
	//MML�������ɵ�ǰ�ڹ���   
	CParseResult *pParaResult = *((CParseResult **)pFrame->Data);

	int nRecCnt      = 0;   //�鵽�ļ�¼��
	int nTempCount   = 0;   //���һҳ���ܳ���40����¼���м���
	int nCurrentPage = 0;   //���ڼ�¼ҳ��
	STRING  *pMMLPages;     //������ɵ��ڶ�MML����	

	char szTmpOptr[78];
    memset(szTmpOptr, 0, 78);
	
	m_nReportCsn ++;       //ÿ�α�����ˮ�ţ�1   
	
    int nAlarmType;
    int nRetCount;

    //ȡ��һ��MML�������澯���ͣ�ȱʡΪ���и澯
    if (pParaResult->GetDigitVal(&nAlarmType, 1, 1) == FALSE)
    {
         nAlarmType = 3;    //ȱʡ��ѯ���и澯
    }
   
    //ȡ�ڶ���MML����������������ȱʡΪ64��
    if (pParaResult->GetDigitVal(&nRetCount, 2, 1) == FALSE)
    {
        nRetCount = 64;    //ȱʡ����64����
    }	
    if(nRetCount == 0 || nRetCount >= 600)
	{
		nRetCount = 600;
	}
    
	LIST<SHistoryAlarm *> TmpList;
	UINT4 nTmpCnt = m_pHistoryAlarm->GetRecordNum();
	SHistoryAlarm *pHistoryArray = new SHistoryAlarm[nTmpCnt];
	m_pHistoryAlarm->Seek(0);

	if(m_pHistoryAlarm->GetRecord(pHistoryArray,nTmpCnt) != ERR_SUCCESS || nTmpCnt == 0)
	{
		delete[] pHistoryArray;
		HandleCmdDefault(pFrame);
		return;
	}

    for(int i = nTmpCnt - 1; (i >= 0) && (nRecCnt < nRetCount); i--)
	{
		//�����ѯ�¼��澯�Ļ�
		if(2 == nAlarmType)
		{
		
			if(pHistoryArray[i].yAlarmType == AT_EVENT)
            {
                if( (pHistoryArray[i].yAlarmLevel == AL_EMERGENCY)
				&& (pHistoryArray[i].szFAckOptr[0] == '\0') )
			    {   
				    nRecCnt ++;
				    TmpList.push_back(&pHistoryArray[i]);
			    }
            }
        }
		else if(1 == nAlarmType)
		{
		
			if(pHistoryArray[i].yAlarmType == AT_FAULT)
            {
                if((pHistoryArray[i].szFAckOptr[0] == '\0') 
			        || (pHistoryArray[i].yResType == NOT_RECOVER))
			    {
				    nRecCnt++;
				    TmpList.push_back(&pHistoryArray[i]);              
			    }
            }
        }
		else
		//��ѯȫ���澯��ʱ��
		{
		
			if( ( pHistoryArray[i].yAlarmType == AT_EVENT 
				&& pHistoryArray[i].yAlarmLevel == AL_EMERGENCY
				&& pHistoryArray[i].szFAckOptr[0] == '\0' )
				|| ( pHistoryArray[i].yAlarmType == AT_FAULT 
				     && (pHistoryArray[i].szFAckOptr[0] == '\0' 
				        || pHistoryArray[i].yResType == NOT_RECOVER) ) )
			{
				nRecCnt++;
				TmpList.push_back(&pHistoryArray[i]);
			}
		}    
	}//for ѭ������

	//���������Ϊ��,�����HandleCmdDefault()���Ͳ����޽��
	if (0 == TmpList.size())
	{
		delete[] pHistoryArray;
		HandleCmdDefault(pFrame);
		return;
	}
   
	//��������в�Ϊ�գ�����ݲ�ѯ�ṹ�γ�MML����

    //by ldf 2002-10-29 ��Ӧ���ⵥ D18685
    int nMMLPageNum = nRecCnt/m_nPageSize +1 ;
    pMMLPages = new STRING[nMMLPageNum];   //���㷵�ص�MML������Ҫ��ҳ��
    //�޸����
	nTempCount   = 0;
	nRecCnt      = 0;
	nCurrentPage = 0;
	//ActiveIter = TmpList.begin();
	LIST<SHistoryAlarm *>::iterator Iter = TmpList.begin();
	SHistoryAlarm* pHistoryAlarm = NULL;

	while (Iter != TmpList.end())
	{
		//����һҳMML����ͷ
		nCurrentPage++;
		m_pAlarmMMLReport->MakeReportHead();
		m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(), m_nReportCsn);
		
		//���ݲ�ѯ�������һҳMML������
		while (Iter != TmpList.end())
		{  
 
			//��һ��MML�����еļ�¼������һҳMML�������ĳߴ磬���ҳ��ʾ			
			if (nTempCount >= m_nPageSize) //ʵ�ַ�ҳ��ʾ
			{ 
				nTempCount = 0;
				if (nRecCnt < TmpList.size())
                {
				    //�����м�ҳ����"TO BE CONTINUED..."
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
				    m_pAlarmMMLReport->AddString(S_ALARM_TOBECONITUE);
                    //by ldf 2002-10-29 ��Ӧ���ⵥ D18685
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                    //�޸����
                }

				break;
			}

            //�澯�������ϸ澯֮��Ŀ���
			m_pAlarmMMLReport->AddString(STR_NEWLINE);
			m_pAlarmMMLReport->MakeQueryAlarmBody((*Iter), m_pAlarmCfg);

			nTempCount ++;
			nRecCnt ++;
			Iter ++;  
      	}

        //by ldf 2002-10-29 ��Ӧ���ⵥ D18685
        if (nRecCnt == TmpList.size())
        {
            char szContent[32];

            //Ȼ���ټ��������� ��(�������=2)��������
            //S_ALARM_ALARMLIST_TOTALNUM
            SNPRINTF(szContent,32,S_ALARM_ALARMLIST_TOTALNUM,TmpList.size());
            m_pAlarmMMLReport->AddString(szContent);
            m_pAlarmMMLReport->AddString(STR_NEWLINE);
            m_pAlarmMMLReport->AddString(STR_NEWLINE);

            //by ldf 2002-10-30 ��Ӧ���ⵥ D18685 �淶��һ���޸�
            if (nMMLPageNum > 1)
            {
                //�������һҳ����	<����><�������><������>
                //S_ALARM_ALARMREPORT_TOTALNUM
                SNPRINTF(szContent,32,S_ALARM_ALARMREPORT_TOTALNUM,nMMLPageNum);
                m_pAlarmMMLReport->AddString(szContent);
                m_pAlarmMMLReport->AddString(STR_NEWLINE);
                m_pAlarmMMLReport->AddString(STR_NEWLINE);
            }
            //�޸����
        }
        //�޸����

		//����һҳMML����β
		m_pAlarmMMLReport->MakeReportTail();
		pMMLPages[nCurrentPage-1] = m_pAlarmMMLReport->GetReport();
	}

	//���η���MML�ı���
	for (int I = 0 ; I < nCurrentPage; I++)
	{
        SendMessageToMML(pMMLPages[I].c_str(), APP_MML_NORM, pFrame->IPAddr, pFrame->ClientNo);        
	}
	delete[] pHistoryArray;
	delete[] pMMLPages;
	TmpList.clear();
}
//////////////////////////////////////////////////////////////////////////////
//��������	������ʷ�澯��ѯ����,�������ӦLST IGWALMLOG
//�������	
//       pFrame : �ڲ�����֡
//////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::HandleCmdIGWAlarmQueryLOG(MSG_INTRA *pFrame)
{
	//MML�������ɵ�ǰ�ڹ���   
	CParseResult *pParaResult = *((CParseResult **)pFrame->Data);//unit test change
	m_nReportCsn ++;
	
	int nRecCnt       = 0;    //�鵽�ļ�¼��	
	int nTempCount    = 0;    //����ҳ���������һҳ���ܳ���40����¼	
	int nCurrentPage  = 0;    //���ڼ�¼ҳ��	
	STRING *pMMLPages = NULL; //������ɵ��ڶ�MML����  	
    
    int j;  //ѭ������

    int nAlarmType;    //�澯����

    //ȡMML�����һ���������澯���ͣ����������
    pParaResult->GetDigitVal(&nAlarmType, 1, 1);
   
    int nResFlg[3];	   //�ָ���־
     
    //ȡMML����ڶ����������ָ���־�����������
    GetIntArrayValue(pParaResult, 2, nResFlg, 3, 2);
    
    int nAckFlg[3];    //ȷ�ϱ�־
 
    //ȡMML���������������ȷ�ϱ�־�����������   
    GetIntArrayValue(pParaResult, 3, nAckFlg, 3, 2);

    int nModNum;      //ģ���
    
    //ȡMML������ĸ�������ģ��ţ���ѡ������ȱʡΪȫ��ģ�飩
    if (pParaResult->GetDigitVal(&nModNum, 4, 1) == FALSE)
    {
        nModNum = 255;
    }

    int nStartCsn;    //��ʼ��ˮ��

    //ȡMML����������������ʼ�澯��ˮ�ţ����������    
	//���û�е��塢����������������nCsnDef Ϊ 0
    int nCsnDef = pParaResult->GetDigitVal(&nStartCsn, 5, 1);
    
    int nEndCsn;      //��ֹ��ˮ��
    
    //ȡMML�����������������ֹ�澯��ˮ�ţ����������    
    pParaResult->GetDigitVal(&nEndCsn, 6, 1);

    int nStartAlarmID;   //��ʼ�澯���
    
    //ȡMML������߸���������ʼ�澯��ţ����������   
	//���û�е��ߡ��˸������������� nAlarmIDDef Ϊ 0
    int nAlarmIDDef = pParaResult->GetDigitVal(&nStartAlarmID, 7, 1);

    int nEndAlarmID;     //��ֹ�澯���
    
    //ȡMML����ڰ˸���������ֹ�澯��ţ���������� 
    pParaResult->GetDigitVal(&nEndAlarmID, 8, 1);

    int nAlmLvl[5];     //�澯����
    
    //ȡMML����ھŸ��������澯���𣨱��������
    GetIntArrayValue(pParaResult, 9, nAlmLvl, 5, 4);

    int nASS[11];       //���ܷ���
    
    //ȡMML�����ʮ�����������ܸ澯���ࣨ���������
    GetIntArrayValue(pParaResult, 10, nASS, 11, 10);
	
    tm StartDate;
    tm EndDate;
	
    //ȡMML�����11��12��13��14����������ʼ���ڡ���ֹ���ڡ���ʼʱ�䡢��ֹʱ�䣨���������
	//by ldf 2002.1.18 ���ж��Ƿ�����ʱ�䱻ָ������û����nDateDefΪ0
    int nDateDef = pParaResult->GetDigitVal(&nDateDef, 11, 1);
	if ( nDateDef != 0 )
	{
		GetDateValue(pParaResult, 11, &StartDate);
		GetDateValue(pParaResult, 12, &EndDate);
		GetTimeValue(pParaResult, 13, &StartDate);
		GetTimeValue(pParaResult, 14, &EndDate);    
	}
    
    int nRetCount;   //���ؼ�¼��
    
    //ȡMML�����15�����������ؼ�¼�������������
    pParaResult->GetDigitVal(&nRetCount, 15, 1);
    if(nRetCount == 0 || nRetCount >= 600)
	{
		nRetCount = 600;
	}
    //LST ALMLOG����ĵ�16��19������iGateway Bill�޹أ���˲�����
	

	//������ʷ�澯�⣬���ҷ��������ĸ澯��¼���γɲ�ѯ������У�
	LIST<SHistoryAlarm *> pFoundAlarmList;
    UINT4 uNum = m_pHistoryAlarm->GetRecordNum();
    
    if (uNum == 0)   //��ʷ�澯���в����ڸ澯��¼
    {
		HandleCmdDefault(pFrame);
        return;        
    }

	SHistoryAlarm *pHistory = new SHistoryAlarm[uNum];
	m_pHistoryAlarm->Seek(0);  //��ָ���Ƶ���ʷ��澯�ĵ�һ����¼

	char szAckOperator[78] = "";

	m_pHistoryAlarm->GetRecord(pHistory,uNum);
    
    int nParaCheck[14];

	for(j = uNum - 1; (j >= 0) && (nRecCnt < nRetCount) ;j--)
	{        
        memset(&nParaCheck, 0, 14 * sizeof(int));
 
        //�жϵ�һ����ѯ�������澯����
        switch (nAlarmType)
        {
        case 1:   //��ѯ���ϸ澯
            if (pHistory[j].yAlarmType == AT_FAULT)
            {
                nParaCheck[0] = 1;                 
            }
             
            break;
        case 2:   //��ѯ�¼��澯
            if (pHistory[j].yAlarmType == AT_EVENT)
            {
                nParaCheck[0] = 1;
            }
             
            break;
        default:
            nParaCheck[0] = 1;
            break;
        }
      
        //�жϵڶ�����ѯ�������ָ���־
        //��ѯ�ѻָ�����δ�ָ��澯
        if ((nResFlg[2] == 1) || ((nResFlg[0] == 1) && (nResFlg[1] == 1)))
        {
            nParaCheck[1] = 1;
        }
        //ֻ��ѯ�ѻָ��澯
        else if ((nResFlg[0] == 1) && (nResFlg[1] == 0))
        {
            if (pHistory[j].yResType != NOT_RECOVER)
            {
                nParaCheck[1] = 1;
            }
        }
        //ֻ��ѯδ�ָ��澯
        else if ((nResFlg[0] == 0) && (nResFlg[1] == 1))
        {
            if (pHistory[j].yResType == NOT_RECOVER) 
            {
                nParaCheck[1] = 1;
            }
        }

        //�жϵ�������ѯ������ȷ�ϱ�־         
        //��ѯ��ȷ�ϻ���δȷ�ϸ澯
        if (nAckFlg[2] == 1)
        {
            nParaCheck[2] = 1;   
        }
        //ֻ��ѯ��ȷ�ϸ澯
        else if ((pHistory[j].szFAckOptr[0] != '\0') && (nAckFlg[0] == 1))
        {
             nParaCheck[2] = 1;
        }
        //ֻ��ѯδȷ�ϸ澯
		//by ldf 2002.1.18
        else if ((pHistory[j].szFAckOptr[0] == '\0') && (nAckFlg[1] == 1))
        {
             nParaCheck[2] = 1;
        } 
        
        //�жϵ��ĸ���ѯ������ģ��� 
        if ((nModNum == 255) || (nModNum == pHistory[j].yModuleNO))
        {
            nParaCheck[3] = 1; 
        }
		
        //�жϵ��塢��������ѯ��������ʼ��ˮ�ź���ֹ��ˮ��
		//���塢��������û�и���
		if ( nCsnDef == 0 )
		{
			nParaCheck[4] = 1;
			nParaCheck[5] = 1;
		}

        if ((pHistory[j].uCsn >= nStartCsn) && (pHistory[j].uCsn <= nEndCsn))
        {
            nParaCheck[4] = 1;
            nParaCheck[5] = 1;
        }
 
        //�жϵ��ߡ��ڰ˸���ѯ��������ʼ�澯��ź���ֹ�澯���

		if ( nAlarmIDDef == 0 )
		{
			nParaCheck[6] = 1;
            nParaCheck[7] = 1;
		}

        if ((pHistory[j].uAlarmID >= nStartAlarmID) && (pHistory[j].uAlarmID <= nEndAlarmID))       
        {
            nParaCheck[6] = 1;
            nParaCheck[7] = 1;
        }
 

        //ȷ���ھŸ���ѯ�������澯����        
        if (nAlmLvl[4] == 1) //���м���
        {
            nParaCheck[8] = 1;
        } 

		if ( (nAlmLvl[pHistory[j].yAlarmLevel] == 1) 
			      && ( pHistory[j].yAlarmLevel < 5 ) 
			      && ( pHistory[j].yAlarmLevel > -1) ) 
		{
			nParaCheck[8] = 1;
		}


        //�жϵ�ʮ����ѯ��������������(iGateway Bill�ĸ澯���ڡ�����澯��
        if ((nASS[10] == 1) || (nASS[5] == 1))
        {
            nParaCheck[9] = 1;            
        }

        //�жϵ�ʮһ��ʮ����ʮ����ʮ�ĸ���ѯ��������ѯ��ʼ���ڡ���ֹ���ڡ���ʼʱ�䡢��ֹʱ��
        if ( nDateDef == 0 )
		{
			nParaCheck[10] = 1;
            nParaCheck[11] = 1;
            nParaCheck[12] = 1;
            nParaCheck[13] = 1;
		}
		else
		{
			time_t  tStart, tEnd, tTarget;
			tStart = mktime(&StartDate);
			tEnd   = mktime(&EndDate);
			tm AlarmTime;
			AlarmTime.tm_year = pHistory[j].uSYear - 1900;
			AlarmTime.tm_mon  = pHistory[j].ySMonth - 1;
			AlarmTime.tm_mday = pHistory[j].ySDay;
			AlarmTime.tm_hour = pHistory[j].ySHour;
			AlarmTime.tm_min  = pHistory[j].ySMin;
			AlarmTime.tm_sec  = pHistory[j].ySSec;
			tTarget  = mktime(&AlarmTime);
			
			if ((tTarget >= tStart) && (tTarget <= tEnd)) 
			{
				nParaCheck[10] = 1;
				nParaCheck[11] = 1;
				nParaCheck[12] = 1;
				nParaCheck[13] = 1;
			}
		}
        
        int nFound = 1;

        for (int i = 0; i < 14; i++)
        {
            //ֻҪһ����ѯ���������㣬����Ϊ�����澯�����ϲ�ѯ����
            if (nParaCheck[i] == 0)
            {
                nFound = 0;
                break;
            }
        }
		
        if (nFound)
        {
            nRecCnt++;
            pFoundAlarmList.push_back(&pHistory[j]);
        }
	}

   
	//���������Ϊ�գ������HandleCmdDefault���Ͳ����޽��
	if(0 == pFoundAlarmList.size())
	{
		HandleCmdDefault(pFrame);
        delete[] pHistory;
		return;
	}
	
	//��������в�Ϊ�գ�����ݲ�ѯ�ṹ�γ�MML����
    //by ldf 2002-10-29 ��Ӧ���ⵥ D18685
    int nMMLPageNum = nRecCnt/m_nPageSize +1 ;
    pMMLPages = new STRING[nMMLPageNum];
    //�޸����

	nTempCount = 0;
	nRecCnt = 0;
	nCurrentPage = 0;
	LIST<SHistoryAlarm *>::iterator Iter = pFoundAlarmList.begin();	
	
	while(Iter != pFoundAlarmList.end())
	{
		//����һҳMML����ͷ��
		nCurrentPage++;
		m_pAlarmMMLReport->MakeReportHead();
		m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(), m_nReportCsn);
		
        //�ݲ�ѯ�������һҳMML�����壻
		while(Iter != pFoundAlarmList.end())
		{  
	
			//��һ��MML�����еļ�¼������һҳMML�������ĳߴ磬���ҳ��ʾ��
			if(nTempCount >= m_nPageSize)//ʵ�ַ�ҳ��ʾ
			{ 
				nTempCount = 0;
				if(nRecCnt < pFoundAlarmList.size())
				//by ldf 2002-10-29 ��Ӧ���ⵥ D18685
                {
                    //�����м�ҳ����"TO BE CONTINUED..."
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                    m_pAlarmMMLReport->AddString(S_ALARM_TOBECONITUE);
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                }
                //�޸����
				break;
			}

			//M2000�������ϸ澯֮��Ŀ���
			m_pAlarmMMLReport->AddString(STR_NEWLINE);
			m_pAlarmMMLReport->MakeQueryAlarmBody((*Iter), m_pAlarmCfg);

			nTempCount ++;
			nRecCnt ++;
			Iter ++;  
      	 }

        //by ldf 2002-10-29 ��Ӧ���ⵥ D18685
        if (nRecCnt == pFoundAlarmList.size())
        {
            char szContent[32];

            //Ȼ���ټ��������� ��(�������=2)��������
            //S_ALARM_ALARMLIST_TOTALNUM
            SNPRINTF(szContent,32,S_ALARM_ALARMLIST_TOTALNUM,pFoundAlarmList.size());
            m_pAlarmMMLReport->AddString(szContent);
            m_pAlarmMMLReport->AddString(STR_NEWLINE);
            m_pAlarmMMLReport->AddString(STR_NEWLINE);

            //by ldf 2002-10-30 ��Ӧ���ⵥ D18685 �淶��һ���޸�
            if (nMMLPageNum > 1)
            {
                //�������һҳ����	<����><�������><������>
                //S_ALARM_ALARMREPORT_TOTALNUM
                SNPRINTF(szContent,32,S_ALARM_ALARMREPORT_TOTALNUM,nMMLPageNum);
                m_pAlarmMMLReport->AddString(szContent);
                m_pAlarmMMLReport->AddString(STR_NEWLINE);
                m_pAlarmMMLReport->AddString(STR_NEWLINE);
            }
            //�޸����
        }
        //�޸����

	
		//����һҳMML����β��
		m_pAlarmMMLReport->MakeReportTail();
		pMMLPages[nCurrentPage-1] = m_pAlarmMMLReport->GetReport();
	}
	
	//����MML�ı���
	for (int I = 0 ;I < nCurrentPage; I++)
	{
        SendMessageToMML(pMMLPages[I].c_str(),APP_MML_NORM,pFrame->IPAddr, pFrame->ClientNo);        
	}
	
	delete[] pMMLPages;
	delete[] pHistory;
	pFoundAlarmList.clear();
}
/////////////////////////////////////////////////////////////////////////////////
//��������	�ֶ��ָ���ǰ�澯����Ӧ��RMV IGWALMFLT����
//�������	MSG_INTRA *pFrame
//�������	��
//����ֵ	0 ��ʾ�ɹ�  ������ʾʧ��
/////////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::HandleCmdIGWAlarmDelete(MSG_INTRA *pFrame)
{
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);
    m_nReportCsn ++;
    
    int  iResFlag      = 0;  //�Ƿ��н��
    char szTmpOptr[78] = "";
    
    int nStartCsn;
    
    //ȡMML�����һ����������ʼ�澯��ˮ�ţ����������
    pParaResult->GetDigitVal(&nStartCsn, 1, 1);	
    
    UINT4 nRecNum = m_pHistoryAlarm->GetRecordNum();
    
    SActiveAlarm *pActive = NULL;
    
    //����Ҫ����澯���У���Ҫ������ʷ�澯��.
    SHistoryAlarm *pHistory;
    pHistory = new SHistoryAlarm[nRecNum];
    m_pHistoryAlarm->Seek(0);
    m_pHistoryAlarm->GetRecord(pHistory, nRecNum);
    
    //�ֹ��ָ���nStartCsn��ʼ�ĸ澯
    for (UINT4 i = 0; i < nRecNum; i++)
    {   //������ 2002-04-17 �������¹淶�޸�,���ⵥ12740 Csn >= nStartCsn ==> ==
        if ((pHistory[i].uCsn == nStartCsn)  
            && (pHistory[i].yAlarmType == AT_FAULT)
            && (pHistory[i].yResType   == NOT_RECOVER))
        {
            
            RECOVERY_FAULTALARM(&pHistory[i], m_pHistoryAlarm, MANNUAL_RECOVER);
            SInnerAlarm sInnerAlarm;
            sInnerAlarm.uAlarmID    = pHistory[i].uAlarmID;  
            sInnerAlarm.yAlarmType  = AT_RESTORE;
            memcpy(sInnerAlarm.yAlarmPara, pHistory[i].yPara, MAX_ALARM_PARA);
            
            //�ڵ�ǰ���������Ӹ澯
            //�ڲ�������ڴ�������,���Կ���ʹ�þֲ�����
            m_ActiveAlarmList.AddAlarm(&sInnerAlarm, &pActive);
            
            //by ldf 2002.1.7 
            if (pActive != NULL)
            {
                pActive->uCsn = pHistory[i].uCsn;
                //������ �澯�� ���
                //m_pAlarmBox->HandleAlarm(pActive);
                if(m_pAlarmBoxByLMT != NULL)
                {
                    m_pAlarmBoxByLMT->HandleAlarm(pActive); 
                }
                if(m_pAlarmBoxByCG != NULL)
                {
                    m_pAlarmBoxByCG->HandleAlarm(pActive); 
                }
            }
            iResFlag = 1;
            
        }
    }
    
    //����м�¼���ָ�����MML����
    delete[] pHistory;
    if (iResFlag == 1)
    {
        m_pAlarmMMLReport->MakeReportHead();
        m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(),m_nReportCsn);
        m_pAlarmMMLReport->AddString(STR_NEWLINE);
        m_pAlarmMMLReport->MakeRecoverAlarmBody(nStartCsn, nStartCsn, m_uBillsvrNo);
        m_pAlarmMMLReport->MakeReportTail();		
        //���ڴ�ά���ڷ���һ���澯
        SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_NORM,pFrame->IPAddr, pFrame->ClientNo);
        
        //���ڴ��ϱ��ڷ���һ���澯
        m_pAlarmMMLReport->MakeReportHead();
        m_pAlarmMMLReport->MakeRecoverAlarmBody(nStartCsn, nStartCsn, m_uBillsvrNo);
        m_pAlarmMMLReport->MakeReportTail();
        SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_WARN, pFrame->IPAddr, pFrame->ClientNo);
        
    }	
    else //���û�м�¼���ָ�����ʹ��Ĭ�ϴ���
    {
        HandleCmdDefault(pFrame);
    }    
    
    return ERR_SUCCESS;
}
// by	����	2004-5-19   ��Ӧ���ⵥSWPD05267
/**********************************************************************
��������	����澯�����м���һ���µĸ澯
�������	pNewAlarm������������¸澯
�������	ppActiveAlarm�����ڻ�澯�����������ӵĻ�澯
����ֵ		�����¸澯�Ľ��
�׳��쳣	��
*********************************************************************/
ALARM_ADD_RESULT CMMLAlarmHandler::AddAlarm(SInnerAlarm* pNewAlarm, SActiveAlarm** ppActiveAlarm, int nRecoverType)
{
	assert(pNewAlarm->uAlarmID < 128);
	
	ALARM_ADD_RESULT nResult = ADD_OK;

	//���Ҫ���͵ĸ澯�����Σ��򲻷���
    m_Mutex.acquire();
    char shield = m_szAlarmShield[pNewAlarm->uAlarmID];
    m_Mutex.release();    
    if ('0' != shield) 
        
	{
		nResult = CAlarmHandler::AddAlarm(pNewAlarm,ppActiveAlarm,nRecoverType);
	}

	return nResult;
}

// the end	2004-5-19	��Ӧ���ⵥSWPD05267
