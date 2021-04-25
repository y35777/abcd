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
//功能描述  构造函数完成成员变量的初始化
//输入参数  BillsvrNo - 主备信息
//////////////////////////////////////////////////////////////////////
CMMLAlarmHandler::CMMLAlarmHandler(int BillsvrNo, CAlarmServer* pAlarmServer,
                                   const char* szAlarmShield, char *pcHisPath) 
                                   : CAlarmHandler(BillsvrNo, pAlarmServer)                                   
{
    //初始化成员变量

    m_nActiveTime     = ONE_HOUR;
    m_pAlarmMMLReport = NULL;
    m_pHistoryAlarm   = NULL;
    m_pAlarmBoxByCG   = NULL; //张玉新 告警箱
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
//析构函数
/////////////////////////////////////////////////////////////////////
CMMLAlarmHandler::~CMMLAlarmHandler()
{
    //删除历史告警库
    if (m_pHistoryAlarm != NULL)
    {
        delete m_pHistoryAlarm;
        m_pHistoryAlarm = NULL;
    }

    //删除报文构造对象
    if(m_pAlarmMMLReport != NULL)
    {
        delete m_pAlarmMMLReport;
        m_pAlarmMMLReport = NULL;
    }

    //删除CG告警箱
    if(m_pAlarmBoxByCG != NULL)
    {
        delete m_pAlarmBoxByCG;
        m_pAlarmBoxByCG = NULL;
    }
    
    //删除LMT告警箱
    if(m_pAlarmBoxByLMT != NULL)
    {
        delete m_pAlarmBoxByLMT;
        m_pAlarmBoxByLMT = NULL;
    }    

    //删除SNMP告警代理
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

    //取出hisalarm.his文件的头四个字符
    char szVer[5];
    memset(szVer,0 ,4);
    fread(szVer, sizeof(char), 4, pAlarmFile);
    szVer[4] = '\0';

    //获取文件的版本号信息
    UINT4 uVer = -1;
    if(ACE_OS::strncmp(szVer, "VER:", 4) != 0)//判断文件的头四个字符是否为"VER:"
    {        
        uVer = 0;//如果文件的头四个字符不是VER:，则说明结构为b01
    }
    else
    {
        fseek(pAlarmFile, 4, SEEK_SET);
        fread(&uVer,sizeof(UINT4), 1, pAlarmFile);  //读取文件中的版本号的信息
        if(uVer == LATEST_ALARM_HIS_VERSION)//如果为最新版本则退出
        {
            return TRUE;
        }
    }

    //在tmp文件中填写文件头信息
    STRING szAlarmFileTmp = sHisPath  + DELIMIT_CHAR + "alarmhistory.tmp";
    FILE* pAlarmFileTmp = fopen(szAlarmFileTmp.c_str(), "wb");
    if(pAlarmFileTmp == NULL)
    {
        fclose(pAlarmFile);
        return FALSE;
    }    

    //对于历史告警结构进行改变
    if(!FormatChange(uVer, pAlarmFile, pAlarmFileTmp))
    {
        fclose(pAlarmFile);
        fclose(pAlarmFileTmp);
        return FALSE;
    }

    fclose(pAlarmFile);
    fclose(pAlarmFileTmp);
    
    //将老版本的文件改名为alarmHistory.bak，临时文件改名为alarmHistory.his
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

    //老版本文件读入内存
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

        //是b01版本,csn+n*SHistoryAlarmB01
        if((nAlarmFileLen - SIZEOF_ALARMHEAD) % sizeof(SHistoryAlarmB01) == 0 )          
        {            
            nRecNum = (nAlarmFileLen - SIZEOF_ALARMHEAD) / sizeof(SHistoryAlarmB01);
            nCsnTmp = *(UINT4 *)(pMemAlarmFile + nAlarmFileLen - sizeof(SHistoryAlarmB01));
           
            if(*(UINT4 *)pMemAlarmFile == nCsnTmp)
            {
                nSubVer = 0;
            }
            
        }
        //b02非OM整改版,csn+n*SHistoryAlarm
        if((nAlarmFileLen - SIZEOF_ALARMHEAD) % sizeof(SHistoryAlarm) == 0  )
        {
            nRecNum = (nAlarmFileLen - SIZEOF_ALARMHEAD) / sizeof(SHistoryAlarm);
            nCsnTmp = *(UINT4 *)(pMemAlarmFile + nAlarmFileLen - sizeof(SHistoryAlarm)); 
            
            if(*(UINT4 *)pMemAlarmFile == nCsnTmp)
            {
                nSubVer = 1;
            }
        }
        //b02的OM整改版,csn+syn+baseid+n*SHistoryAlarm
        if((nAlarmFileLen - SIZEOF_ALARMHEAD*3) % sizeof(SHistoryAlarm) == 0)
        {
            nRecNum = (nAlarmFileLen - SIZEOF_ALARMHEAD) / sizeof(SHistoryAlarm);  
            nCsnTmp = *(UINT4 *)(pMemAlarmFile + nAlarmFileLen - sizeof(SHistoryAlarm));
            if(*(UINT4 *)pMemAlarmFile == nCsnTmp)
            {
                nSubVer = 2;
            }
        }
        //错误版本退出
        if(nSubVer == -1)
        {
            delete pMemAlarmFile;
            return FALSE;
        }        
        
        //填写新结构的文件头信息
        fwrite("VER:", sizeof(char), 4, pAlarmFileTmp);//填写"VER:"
        UINT4 uTmp = LATEST_ALARM_HIS_VERSION;
        fwrite(&uTmp, sizeof(UINT4), 1, pAlarmFileTmp);//填写版本号  
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

        //根据告警条数，申请新文件的内存空间
        pMemAlarmFileTmp = new BYTE[sizeof(SHistoryAlarm) * nRecNum];
        memset(pMemAlarmFileTmp, 0, sizeof(SHistoryAlarm) * nRecNum);
        SHistoryAlarm* pAlarmTmpHis = (SHistoryAlarm*)pMemAlarmFileTmp;  
        
        if(nSubVer == 0)
        {
            SHistoryAlarmB01* pAlarmHis = (SHistoryAlarmB01 *)(pMemAlarmFile + SIZEOF_ALARMHEAD);
            while(pAlarmHis < (SHistoryAlarmB01 *)pMemAlarmFile + nRecNum)
            {            
                pAlarmTmpHis->uSyn          = 90;                       //告警同步号
                //根据活动告警向历史告警结构赋值
                pAlarmTmpHis->uAlarmID      = pAlarmHis->uAlarmID;             //告警ID
                pAlarmTmpHis->uCsn          = pAlarmHis->uCsn;                 //告警流水号
                
                pAlarmTmpHis->uSYear        = pAlarmHis->uSYear;                //告警产生的年、月、日
                pAlarmTmpHis->ySMonth       = pAlarmHis->ySMonth;
                pAlarmTmpHis->ySDay         = pAlarmHis->ySDay;
                pAlarmTmpHis->ySHour        = pAlarmHis->ySHour;                //告警产生的时、分、秒
                pAlarmTmpHis->ySMin         = pAlarmHis->ySMin;
                pAlarmTmpHis->ySSec         = pAlarmHis->ySSec;
                pAlarmTmpHis->yAlarmLevel   = pAlarmHis->yAlarmLevel;          //告警级别
                pAlarmTmpHis->yAlarmType    = pAlarmHis->yAlarmType;           //告警类型
                pAlarmTmpHis->yCPUPos       = pAlarmHis->yCPUPos;              //CPU
                pAlarmTmpHis->yFuncID       = pAlarmHis->yFuncID;              //告警功能号
                pAlarmTmpHis->yModuleNO     = pAlarmHis->yModuleNO;            //产生告警的模块号
                pAlarmTmpHis->ySubSysID     = pAlarmHis->ySubSysID;            //告警子系统号
                memcpy(pAlarmTmpHis->yPara, pAlarmHis->yPara, MAX_ALARM_PARA); //告警参数
                pAlarmTmpHis->yPID          = pAlarmHis->yPID;                 //产生告警的进程ID
                
                memcpy(pAlarmTmpHis->szFAckOptr, pAlarmHis->szFAckOptr, sizeof(pAlarmHis->szFAckOptr));   //故障告警确认操作员
                pAlarmTmpHis->uFAckYear      = pAlarmHis->uFAckYear;  
                pAlarmTmpHis->yFAckMonth		= pAlarmHis->yFAckMonth;                //告警恢复的年、月、日
                pAlarmTmpHis->yFAckDay	    = pAlarmHis->yFAckDay;               
                pAlarmTmpHis->yFAckHour         = pAlarmHis->yFAckHour;                 
                pAlarmTmpHis->yFAckMin        = pAlarmHis->yFAckMin;                //告警恢复的时、分、秒
                pAlarmTmpHis->yFAckSec         = pAlarmHis->yFAckSec;
                
                memcpy(pAlarmTmpHis->szRAckOptr, pAlarmHis->szRAckOptr, sizeof(pAlarmHis->szRAckOptr));   //恢复告警确认操作员
                pAlarmTmpHis->uRAckYear      = pAlarmHis->uRAckYear;  
                pAlarmTmpHis->yRAckMonth		= pAlarmHis->yRAckMonth;                //告警恢复的年、月、日
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
                
                //将每一条告警先copy到新的内存文件中
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
//功能描述  恢复历史告警库中的所有故障告警
//输入参数  无
//输出参数  无
//返回值    成功返回0,否则返回非0
////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::RestoreAllFaultAlarm()
{
    if(m_pHistoryAlarm->Open() != ERR_SUCCESS)
    {
        return ERR_OPEN_HISTORY_ALARM;
    }
    
    //从文件中读取所有告警记录信息,如果读取错误则返回-1
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
            //张玉新 2002-04-26 问题单 12490
            delete[] pBuf;
            //修改结束
            return ERR_FAIL;
        }
        //遍历所有告警
        UINT4 i;
        for(i = 0; i < uNum; i++)
        {
            pHistoryAlarm = (SHistoryAlarm*)(pBuf+SIZEOF_HISTORYALARM * i);
        #ifdef _OM_MODIFY
            //若为故障告警且未恢复,继续处理
            if(AT_FAULT == pHistoryAlarm->yAlarmType
				&& NOT_RECOVER == pHistoryAlarm->yResType && pHistoryAlarm->uRYear==0)
            {
                //将对应的故障告警置为AUTO_RECORVER(复位恢复),
                //设置自动恢复的时间为当前时间
				RECOVERY_FAULTALARM(pHistoryAlarm,m_pHistoryAlarm,RESET_RECOVER);		

                //添加该告警的恢复告警到活动告警队列中
                //指针对历史告警库中的故障告警，而不是所有告警。
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
                //将对应的故障告警置为RESET_RECOVER(复位恢复),
                //设置自动恢复的时间为当前时间
                RECOVERY_FAULTALARM(pHistoryAlarm,m_pHistoryAlarm,RESET_RECOVER);
                
                //添加该告警的恢复告警到活动告警队列中
                //指针对历史告警库中的故障告警，而不是所有告警。
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
//功能描述  初始化函数
//输入参数  无
//输出参数  无
//返回值    当为0时表示初始化成功,非0为错误码
//////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::Init(const char *szCfgPath, int nConnectedMode, STRING szPortName)
{

    if(ERR_SUCCESS != CAlarmHandler::Init(szCfgPath))
    {
        return ERR_FAIL;
    }

    //初始化告警历史库
    //unit test change to 13,must recover when unit test over!
    m_pHistoryAlarm = new CHistoryAlarm(sHisPath.c_str(),ALARM_DEL_NUM,ALARM_UPPER_LIMIT);

    //将历史文件转换为最新结构的历史文件 
    if(!ChangeIntoLatesetAlarmHis())
    {
        TRACE(MTS_ALARM, GetStringFromID(5004));
        STRING  szAlarmFile = sHisPath + DELIMIT_CHAR + "alarmHistory.his";
        STRING  szAlarmFileBak = sHisPath + DELIMIT_CHAR + "alarmHistory.bak";
        
        FileMove(szAlarmFile.c_str(), szAlarmFileBak.c_str());
        //return FALSE;
    }
    //未恢复的故障告警全部置为复位恢复
    RestoreAllFaultAlarm();    

    //从配置文件中读入"源标识苻"字符串
    STRING szSrcName = S_IGATEWAY_BILL;

    //构造报文构造对象
    m_pAlarmMMLReport = new CAlarmMMLReport(szSrcName.c_str());

    //张玉新 告警箱添加
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


    //获取SNMP告警代理参数
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

        //初始化SNMP告警代理对象
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
//功能描述  该函数处理来自iGateway bill内部模块发送的告警消息
//输入参数  szReport - 要广播的MML报文信息
///////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::BroadCastMMLReport(const char *szReport)
{
    SendMessageToMML(szReport,APP_MML_WARN,0,0);
}
///////////////////////////////////////////////////////////////////////
//功能描述  秒事件处理函数
//输入参数  无
//输出参数  无
//返回值    无
///////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::Timer1Sec()
{
    //遍历活动告警队列,若如存在活动告警且其yReceived为AR_NOT_RECEIVED,
    //则按告警主动上报的方式将该告警转换成规范的MML报文,发给MML SERVER.
    //发送一条告警后将该告警的yReceived置AR_RECEIVED,即只发送一次.如果是
    //事件告警或恢复告警,若该告警保存的时间超过m_nActiveTime设置的时间长度
    //则从m_ActiveAlarm队列删除该告警
    SActiveAlarm *pActiveAlarm = NULL;
    CActiveAlarmList::iterator iter;
    static int nTimer1Day = 0;
    iter = m_ActiveAlarmList.begin();
    while(iter != m_ActiveAlarmList.end())//遍历活动告警队列
    {
        pActiveAlarm = (*iter); //获得活动告警数据帧
        if(pActiveAlarm == NULL)
        {
            iter = m_ActiveAlarmList.erase(iter);
            continue;
        }
        if(pActiveAlarm->yReceived == AR_NOT_RECEIVED)
        {
            //调用m_pAlarmMMLReport->MakeAlarmBroad()接口生成MML告警报文
            //并调用BroadCastMMLReport接口向所有MML客户端广播该告警信息;
			m_pAlarmMMLReport->MakeAlarmBroad(pActiveAlarm,m_pAlarmCfg,pActiveAlarm->yRecoverType);
            BroadCastMMLReport(m_pAlarmMMLReport->GetReport());
            pActiveAlarm->yReceived = AR_RECEIVED;
            //张玉新 2002-03-12，将TRACE中的流水号改为告警类型
            //张玉新 2002-04-24, 问题单D12820
            MSGOUT(MTS_ALARM,MSGOUT_LEVEL_IMPORTANT,
                   S_ALARM_SEND_ALARM,
                   pActiveAlarm->uAlarmID,
                   pActiveAlarm->yPara[0],
                   pActiveAlarm->yAlarmType);
            //结束
        }
        //若为事件或恢复告警,则当该告警保存的时间超过m_nActiveTime设置
        //的时间长度,则从m_ActiveAlarmList队列删除该告警.
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
    //清除活动告警队列中长时间(30天)未能恢复的故障告警对于活动告警队列中
    //超过30天的故障告警.

    if(nTimer1Day++>ONE_DAY)
    {
        nTimer1Day = 0;
        OnTimer1Day();
    }

    //删除超过指定条目的历史告警
    m_pHistoryAlarm->Sweep();
    //张玉新2002-08-10
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
//功能描述  清除超长时间告警
//输入参数  无
//输出参数  无
//返回值    无
/////////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::OnTimer1Day()
{
    CActiveAlarmList::iterator tempIter;
    tempIter = m_ActiveAlarmList.begin();
    UINT4 uNum = 1;
	SHistoryAlarm *pHistoryAlarm = NULL;

    //遍历活动告警队列中所有的告警
    while(tempIter != m_ActiveAlarmList.end())
    {
        //如果为故障告警且告警产生时间相对于当前时间早30天
        if(Diff((*tempIter)) >= 30 * ONE_DAY
           && (AT_FAULT == (*tempIter)->yAlarmType))
        {
            SHistoryAlarm ThisAlarm;
            int Ret;
            //并根据告警的流水号从历史告警库中找到对应的告警数据结构
            //将其yResType置为过期恢复,设置告警的恢复时间为当前时间

            Ret = m_pHistoryAlarm->FindRecord((*tempIter)->uCsn);
            if(Ret != ERR_FAIL)
            {
                //by ldf 2004-01-10 自动恢复告警也得写告警日志
                
                pHistoryAlarm = CreateHistoryAlarm( (*tempIter),EXPIRE_RECOVER);
                if(pHistoryAlarm == NULL)
                {
                   continue;
				}
                else
                {
                    //构造自动恢复告警日志,同步号 和 告警类型
                    pHistoryAlarm->uSyn = m_pHistoryAlarm->CreateSyn();
                    pHistoryAlarm->yAlarmType    = AT_RESTORE;
                    pHistoryAlarm->yResType      = EXPIRE_RECOVER;                   //是否恢复
					
                    time_t ltime;
                    time(&ltime);
                    tm *pTime = localtime(&ltime);
                    pHistoryAlarm->uRYear		= pTime->tm_year + 1900;          //告警恢复的年、月、日
                    pHistoryAlarm->yRMonth	    = pTime->tm_mon + 1;        
                    pHistoryAlarm->yRDay        = pTime->tm_mday;      
                    pHistoryAlarm->yRHour       = pTime->tm_hour;              //告警恢复的时、分、秒
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
//功能描述  根据pAlarm中时间计算该告警产生的时间与当前时间的时间差值,以秒为单位
//输入参数  PSActiveAlarm pAlarm
//输出参数  无
//返回值    与当前时间相差的秒数
//////////////////////////////////////////////////////////////////////////////////
UINT4 CMMLAlarmHandler::Diff(SActiveAlarm *pAlarm)
{
    //根据pAlarm中时间计算该告警产生的时间
    tm AlarmStartTime;
    memset(&AlarmStartTime,0,sizeof(tm));
    AlarmStartTime.tm_year = pAlarm->uYear - 1900;
    AlarmStartTime.tm_mon = pAlarm->yMonth-1;//0 ——代表1月份
    AlarmStartTime.tm_mday = pAlarm->yDay;//1-31
    AlarmStartTime.tm_hour = pAlarm->yHour;
    AlarmStartTime.tm_min = pAlarm->yMin;
    AlarmStartTime.tm_sec = pAlarm->ySec;

    //获得告警时间
    time_t alm_time = mktime(&AlarmStartTime);
    //获得当前时间
    time_t current;
    time(&current);

    //返回告警产生时间与当前时间的差值
    return  (UINT4) difftime(current,alm_time);

}
///////////////////////////////////////////////////////////////////////////
//功能描述  该函数处理来自iGateway bill内部模块发送的告警消息
//输入参数  MSG_INTRA  *pFrame
////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::HandleFrameFromApp(MSG_INTRA *pFrame)
{
	//by ldf 2004-06-03 对应问题单SWPD05582
	if (pFrame->CmdCode == CMD_MODULE_ACTIVE) 
	{
		//RestoreAllFaultAlarm();
		return;
	}
    //定义必要的变量
    SActiveAlarm *pActiveAlarm = NULL;
    SHistoryAlarm *pHistoryAlarm = NULL;
    UINT4 uNum;

    if(NULL == m_pHistoryAlarm)
    {
        return;
    }

    //如果属于主机应答的告警消息，不处理，直接返回；
    if(CMD_ALARM_RESPONSE == pFrame->CmdCode)
    {
        return;
    }
    //zyx 告警箱 添加
    if(0xAA == pFrame->CmdCode &&m_pAlarmBoxByLMT != NULL)
    {  //ALARM_CMD_BAU_TO_CONVERT
        m_pAlarmBoxByLMT->HandleAlarmCmdFromBox(pFrame);
        return;
    }    

    //如果告警的类型与配置库的不符就直接丢弃
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
    //如果是来自于接入点或采集进程的告警，则在告警参数中增加接入点号或采集点号
    if(PT_KERNEL != pFrame->SenderPid &&  PT_CLSTR != pFrame->SenderPid &&
        PT_OM != pFrame->SenderPid  &&  PT_CFGSVR != pFrame->SenderPid  &&
        PT_METERACC != pFrame->SenderPid && PT_BS != pFrame->SenderPid)
    {
        //先计算出接入点号与采集号
        BYTE yPID;
        if(pFrame->SenderPid >= 50)
        {
            yPID = pFrame->SenderPid - PT_CO_BASE + 1;
        }
        else
        {
            yPID = pFrame->SenderPid - PT_AP_BASE + 1;
        }
        
        //在告警参数中增加接入点号与采集号
        if(pExplain->yParaNum == 0)//无参数
        {
            pInnerAlarm->yAlarmPara[0] = yPID;
        }
        if(pExplain->yParaNum == 1)//一个参数
        {            
            sprintf((char *)&pInnerAlarm->yAlarmPara[1], "%d", yPID);            
        }
        if(pExplain->yParaNum == 2)//二个参数
        {
            char szTmp[MAX_ALARM_PARA - 1];
            sprintf(szTmp, "%d", yPID);
            
            int nLen = strlen(szTmp);
            szTmp[nLen] = ',';
            strncpy(&szTmp[nLen + 1], (char *)&pInnerAlarm->yAlarmPara[1], MAX_ALARM_PARA - 2 - nLen);
            strncpy((char *)&pInnerAlarm->yAlarmPara[1], szTmp, MAX_ALARM_PARA - 1);
        }
    }
    //若为内部告警消息，添加到活动告警队列中，若添加失败则直接返回，否则继续处理；
    if(AddAlarm(pInnerAlarm, &pActiveAlarm) != ADD_OK)
    {
        return;
    }

#ifdef _OM_MODIFY
	if(pActiveAlarm != NULL && pActiveAlarm->yAlarmType == AT_EVENT)
	{
		//by ldf 2004-01-12 OM整改
		//因为事件告警不如当前告警库，所以如果是事件告警，
		//并且添加成功则发主动上报报文
		//调用m_pAlarmMMLReport->MakeAlarmBroad()接口生成MML告警报文
		//并调用BroadCastMMLReport接口向所有MML客户端广播该告警信息;
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

    //如果是故障告警或者事件告警，则将此告警添加到历史告警库m_pHistoryAlarm
    //并刷新m_szHistoryFile中文件头的告警流水号；
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
            //如果是恢复告警则从历史告警库中查找对应的故障告警
            //根据告警流水号匹配的算法，
            //将对应的故障告警置为AUTO_RECORVER（自动恢复），
            //置自动恢复的时间为当前时间。
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
                    
                    //注意下方获得的pHistoryAlarm已经和上方的不一样了，此处获得的是该故障告警
                    m_pHistoryAlarm->GetRecord(pHistoryAlarm,uNum);
                    RECOVERY_FAULTALARM(pHistoryAlarm,m_pHistoryAlarm);
                }
                break;
            }
            //张玉新 2002-04-12 问题单D12566
        default:
            {
                break;
            }
            //结束
        }
        //张玉新 告警箱 添加
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
    //增加结束
    delete pHistoryAlarm;
}
///////////////////////////////////////////////////////////////////////////
//功能描述  根据活动告警创建历史告警
//输入参数  pActive - 活动告警结构指针
//输出参数  无
//返回值    历史告警结构指针
///////////////////////////////////////////////////////////////////////////
SHistoryAlarm * CMMLAlarmHandler::CreateHistoryAlarm(SActiveAlarm *pActive,int  nRecoverType)
{
    //创建一个历史告警结构
    SHistoryAlarm *pHistory = new SHistoryAlarm;
    memset(pHistory, 0, sizeof(SHistoryAlarm));

    //根据活动告警向历史告警结构赋值
    pHistory->uAlarmID      = pActive->uAlarmID;             //告警ID
    pHistory->uCsn          = pActive->uCsn;                 //告警流水号
    pHistory->uSyn          = pActive->uSyn;                 //告警同步号
    pHistory->uSYear        = pActive->uYear;                //告警产生的年、月、日
    pHistory->ySMonth       = pActive->yMonth;
    pHistory->ySDay         = pActive->yDay;
    pHistory->ySHour        = pActive->yHour;                //告警产生的时、分、秒
    pHistory->ySMin         = pActive->yMin;
    pHistory->ySSec         = pActive->ySec;
    pHistory->yAlarmLevel   = pActive->yAlarmLevel;          //告警级别
    pHistory->yAlarmType    = pActive->yAlarmType;           //告警类型
    pHistory->yCPUPos       = pActive->yCPUPos;              //CPU
    pHistory->yFuncID       = pActive->yFuncID;              //告警功能号
    pHistory->yModuleNO     = pActive->yModuleNO;            //产生告警的模块号
    pHistory->ySubSysID     = pActive->ySubSysID;            //告警子系统号
    memcpy(pHistory->yPara, pActive->yPara, MAX_ALARM_PARA); //告警参数
    pHistory->yPID          = pActive->yPID;                 //产生告警的进程ID
    pHistory->szFAckOptr[0] = '\0';                          //故障告警确认操作员
    pHistory->szRAckOptr[0] = '\0';                          //恢复告警确认操作员

#ifdef _OM_MODIFY
	if(pActive->yAlarmType != AT_RESTORE )
	{
	    pHistory->yResType      = NOT_RECOVER;                   //是否恢复
	}
	else
	{
		pHistory->yResType      = nRecoverType;                   //是否恢复
		
		pHistory->uRYear		= pActive->uYear;                //告警恢复的年、月、日
		pHistory->yRMonth	    = pActive->yMonth;               
		pHistory->yRDay         = pActive->yDay;                 
		pHistory->yRHour        = pActive->yHour;                //告警恢复的时、分、秒
		pHistory->yRMin         = pActive->yMin;
		pHistory->yRSec         = pActive->ySec;
	}
#else
    pHistory->yResType      = NOT_RECOVER; 
#endif

    return pHistory;
}

//////////////////////////////////////////////////////////////////////////////
//功能描述  此函数处理来自MML告警客户端的消息
//输入参数
//      MSG_INTRA *pFrame;
//////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::HandleFrameFromMML(MSG_INTRA *pFrame)
{
    //获得MML命令数据
    CParseResult *pParaResult = *(CParseResult **)pFrame->Data;

    //根据MML命令处理消息
    switch (pParaResult->GetCmdCode())
    {
    // 查询当前告警
	case MML_LST_ALMAF:	
        //TRACE(MTS_ALARM, S_ALARM_RECEIVE, "MML_LST_ALMFE");
        HandleCmdAlarmQuery(pFrame);
        break;

    //查询某个告警流水号的详细信息
    case MML_LST_IGWALMINF:
        HandleCmdAlarmDetail(pFrame);
        break;

    //查询历史告警（包括事件告警和故障告警）
    case MML_LST_ALMLOG:
        //TRACE(MTS_ALARM,S_ALARM_RECEIVE,"MML_LST_ALMLOG");
        HandleCmdAlarmQueryLOG(pFrame);
        break;

    //强制恢复当前故障告警
    case MML_RMV_ALMFLT:
        //TRACE(MTS_ALARM,S_ALARM_RECEIVE,"MML_RMV_ALMFLT");
        HandleCmdAlarmDelete(pFrame);
        break;

    //删除两个时间点之间的历史告警
    case MML_RMV_IGWALMLOG:
        HandleCmdAlarmClear(pFrame);
        break;
        
    //对故障告警进行确认
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

    //对事件告警的告警确认
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

    // 查询当前告警(ccs)
    case MML_LST_IGWALMFE:	
        HandleCmdIGWAlarmQuery(pFrame);
        break;

    //查询CCS历史告警（包括事件告警和故障告警）
    case MML_LST_IGWALMLOG:
        HandleCmdIGWAlarmQueryLOG(pFrame);
        break;
        
    //强制恢复当前故障告警
    case MML_RMV_IGWALMFLT:	
        HandleCmdIGWAlarmDelete(pFrame);
        break;

    //张玉新 告警箱添加
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
	//by ldf 2004-01-09 OM整改
    case MML_CHK_ALM:
		HandleCmdAlarmCHK(pFrame);
		break;
        
    case MML_LST_ALM:
        HandleCmdLstAlm(pFrame);
        break;
        
    case MML_SET_ALMSHIELD:
        HandleCmdSetShield(pFrame);
        break;
        
    //命令不对返回查询无结果
    default:
        //TRACE(MTS_ALARM,S_ALARM_NO_RESULT);
        HandleCmdDefault(pFrame);
        break;
    }

    delete pParaResult;
}

/////////////////////////////////////////////////////////////////////////////////
//功能描述  处理当前告警查询命令,本命令对应LST ALMAF
//输入参数
//       pFrame : 内部数据帧
/////////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::HandleCmdAlarmQuery(MSG_INTRA *pFrame)
{
    //MML报文生成的前期工作
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);
	
    int nRecCnt      = 0;   //查到的记录数
    int nTempCount   = 0;   //针对一页不能超过40条记录进行计数
    int nCurrentPage = 0;   //用于记录页数
    STRING  *pMMLPages;     //存放生成的众多MML报文
	
    char szTmpOptr[78];
    memset(szTmpOptr, 0, 78);
	
    m_nReportCsn ++;       //每次报文流水号＋1
	
    int nRetCount;
	
	int nAlarmSSSN = 0;
	int nAlarmESSN = 0;
	int nAlarmSCSN = 0;
	int nAlarmECSN = 0;
	
    //取第一个MML参数：sssn(起始同步号)
    if (pParaResult->GetDigitVal(&nAlarmSSSN, 1, 1) == FALSE)
    {
		nAlarmSSSN = -1;    //缺省查询所有告警
    }
	
	//取第二个MML参数：essn(终止同步号)
    if (pParaResult->GetDigitVal(&nAlarmESSN, 2, 1) == FALSE)
    {
		nAlarmESSN = -1;    //缺省查询所有告警
    }
	
	//取第三个MML参数：scsn(起始流水号)
    if (pParaResult->GetDigitVal(&nAlarmSCSN, 3, 1) == FALSE)
    {
		nAlarmSCSN = -1;    //缺省查询所有告警
    }
	
	//取第四个MML参数：ecsn(终止流水号)
    if (pParaResult->GetDigitVal(&nAlarmECSN, 4, 1) == FALSE)
    {
		nAlarmECSN = -1;    //缺省查询所有告警
    }
	
    //取第十个MML参数：返回条数，缺省为64条
    if (pParaResult->GetDigitVal(&nRetCount, 10, 1) == FALSE)
    {
        nRetCount = 64;    //缺省返回64条数
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
		//by ldf 2004-01-12 OM整改
		//因只针对为恢复故障告警，添加“ pHistoryArray[i].uRYear==0 ”是因为历史告警库（即告警日志）中
		//有故障告警日志，其yResType也为NOT_RECOVER
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
	}//for 循环结束
	
	//若结果队列为空,则调用HandleCmdDefault()发送操作无结果
	if (0 == TmpList.size())
	{
		delete[] pHistoryArray;
		HandleCmdDefault(pFrame);
		return;
	}
	
    //若结果队列不为空，则根据查询结构形成MML报文
	
    //by ldf 2002-10-29 对应问题单 D18685
    int nMMLPageNum = nRecCnt/m_nPageSize +1 ;
    pMMLPages = new STRING[nMMLPageNum];   //计算返回的MML报文需要的页数
    //修改完毕
    nTempCount   = 0;
    nRecCnt      = 0;
    nCurrentPage = 0;
    //ActiveIter = TmpList.begin();
    LIST<SHistoryAlarm *>::iterator Iter = TmpList.begin();
    SHistoryAlarm* pHistoryAlarm = NULL;
	
    while (Iter != TmpList.end())
    {
        //构造一页MML报文头
        nCurrentPage++;
        m_pAlarmMMLReport->MakeReportHead();
        m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(), m_nReportCsn);
		
        //根据查询结果构造一页MML报文体
        while (Iter != TmpList.end())
        {
			
            //若一条MML报文中的记录数大于一页MML报文最大的尺寸，则分页显示
            if (nTempCount >= m_nPageSize) //实现分页显示
            {
                nTempCount = 0;
                if (nRecCnt < TmpList.size())
                {
                    //对于中间页加上"TO BE CONTINUED..."
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                    m_pAlarmMMLReport->AddString(S_ALARM_TOBECONITUE);
                    //by ldf 2002-10-29 对应问题单 D18685
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                    //修改完毕
                }
				
                break;
            }
			
            //告警联调加上告警之间的空行
            m_pAlarmMMLReport->AddString(STR_NEWLINE);
            m_pAlarmMMLReport->MakeQueryAlarmBody((*Iter), m_pAlarmCfg);
			
            nTempCount ++;
            nRecCnt ++;
            Iter ++;
        }
		
        //by ldf 2002-10-29 对应问题单 D18685
        if (nRecCnt == TmpList.size())
        {
            char szContent[32];
			
            //然后再加上类似于 “(结果个数=2)”的字样
            //S_ALARM_ALARMLIST_TOTALNUM
            SNPRINTF(szContent,32,S_ALARM_ALARMLIST_TOTALNUM,TmpList.size());
            m_pAlarmMMLReport->AddString(szContent);
            m_pAlarmMMLReport->AddString(STR_NEWLINE);
            m_pAlarmMMLReport->AddString(STR_NEWLINE);
			
            //by ldf 2002-10-30 对应问题单 D18685 规范进一步修改
            if (nMMLPageNum > 1)
            {
                //对于最后一页加上  <共有><报告个数><个报告>
                //S_ALARM_ALARMREPORT_TOTALNUM
                SNPRINTF(szContent,32,S_ALARM_ALARMREPORT_TOTALNUM,nMMLPageNum);
                m_pAlarmMMLReport->AddString(szContent);
                m_pAlarmMMLReport->AddString(STR_NEWLINE);
                m_pAlarmMMLReport->AddString(STR_NEWLINE);
            }
            //修改完毕
        }
        //修改完毕
		
        //构造一页MML报文尾
        m_pAlarmMMLReport->MakeReportTail();
        pMMLPages[nCurrentPage-1] = m_pAlarmMMLReport->GetReport();
    }
	
    //依次发送MML的报文
    for (int I = 0 ; I < nCurrentPage; I++)
    {
        SendMessageToMML(pMMLPages[I].c_str(), APP_MML_NORM, pFrame->IPAddr, pFrame->ClientNo);        
    }
    delete[] pHistoryArray;
    delete[] pMMLPages;
    TmpList.clear();
}


//////////////////////////////////////////////////////////////////////////////
//功能描述  处理历史告警查询命令,本命令对应LST ALMLOG
//输入参数
//       pFrame : 内部数据帧
//////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::HandleCmdAlarmQueryLOG(MSG_INTRA *pFrame)
{
	//MML报文生成的前期工作   
	CParseResult *pParaResult = *((CParseResult **)pFrame->Data);//unit test change
	m_nReportCsn ++;
	
	int nRecCnt       = 0;    //查到的记录数	
	int nTempCount    = 0;    //用于页计数，针对一页不能超过40条记录	
	int nCurrentPage  = 0;    //用于记录页数	
	STRING *pMMLPages = NULL; //存放生成的众多MML报文  	
    
    int j;  //循环计数

    int nAlarmType = 0;    //告警类型

    //取MML命令第一个参数：告警类型（必须参数）
    pParaResult->GetDigitVal(&nAlarmType, 1, 1);
   
    int nResFlg[3];	   //恢复标志
     
    //取MML命令第二个参数：恢复标志（必须参数）
    GetIntArrayValue(pParaResult, 2, nResFlg, 3, 2);

    int nStartCsn = 0;    //起始流水号

    //取MML命令第三个参数：起始告警流水号（必须参数）    
	//如果没有第三，四个参数给出，则nCsnDef 为 0
    int nCsnDef = pParaResult->GetDigitVal(&nStartCsn, 3, 1);
    
    int nEndCsn = 0 ;      //终止流水号
    
    //取MML命令第四个参数：终止告警流水号（必须参数）    
    pParaResult->GetDigitVal(&nEndCsn, 4, 1);

    int nStartAlarmID = 0;   //起始告警编号
    
    //取MML命令第五个参数：起始告警编号（必须参数）   
	//如果没有第五、六个参数给出，则 nAlarmIDDef 为 0
    int nAlarmIDDef = pParaResult->GetDigitVal(&nStartAlarmID, 5, 1);

    int nEndAlarmID = 0;     //终止告警编号
    
    //取MML命令第六个参数：终止告警编号（必须参数） 
    pParaResult->GetDigitVal(&nEndAlarmID, 6, 1);

    int nAlmLvl[5];     //告警级别
    
    //取MML命令第七个参数：告警级别（必须参数）
    GetIntArrayValue(pParaResult, 7, nAlmLvl, 5, 4);

    int nASS[11];       //网管分类
    
    //取MML命令第八个参数：网管告警分类（必须参数）
    GetIntArrayValue(pParaResult, 8, nASS, 11, 10);
	
    tm StartDate;
    tm EndDate;
	
    //取MML命令第9，10，11，12个参数：起始日期、终止日期、起始时间、终止时间（必须参数）
	//by ldf 2002.1.18 先判断是否日期时间被指定，如没有则nDateDef为0
    int nDateDef = pParaResult->GetDigitVal(&nDateDef, 11, 1);
	if ( nDateDef != 0 )
	{
		GetDateValue(pParaResult, 9, &StartDate);
		GetDateValue(pParaResult, 10, &EndDate);
		GetTimeValue(pParaResult, 11, &StartDate);
		GetTimeValue(pParaResult, 12, &EndDate);    
	}

    int nModNum;      //模块号
    
    //取MML命令第13个参数：模块号（可选参数，缺省为全部模块）
    if (pParaResult->GetDigitVal(&nModNum, 13, 1) == FALSE)
    {
        nModNum = 255;
    }

    //LST ALMLOG命令的第14－19参数与iGateway Bill无关，因此不考虑

    int nStartSSSN = 0;    //起始流水号

    //取MML命令第20个参数：起始同步号（必须参数）    
	//如果没有第20，21个参数给出，则nSSNDef 为 0
    int nSSNDef = pParaResult->GetDigitVal(&nStartSSSN, 20, 1);

    //2004-07-23 ldf 对应问题单SWPD06736       
    int nEndESSN = 2147483646;      //终止流水号
    
    //取MML命令第21个参数：终止同步号（必须参数）    
    int nESNDef = pParaResult->GetDigitVal(&nEndESSN, 21, 1);
    //end

    int nRetCount = 0;   //返回记录数
    
    //取MML命令第22个参数：返回记录数（必须参数）
    pParaResult->GetDigitVal(&nRetCount, 22, 1);
    if(nRetCount == 0 || nRetCount >= 1000)
	{
		nRetCount = 64;
	}
	

	//遍历历史告警库，查找符合条件的告警记录并形成查询结果队列；
	LIST<SHistoryAlarm *> pFoundAlarmList;
    UINT4 uNum = m_pHistoryAlarm->GetRecordNum();
    
    if (uNum == 0)   //历史告警库中不存在告警记录
    {
		HandleCmdDefault(pFrame);
        return;        
    }

	SHistoryAlarm *pHistory = new SHistoryAlarm[uNum];
	m_pHistoryAlarm->Seek(0);  //将指针移到历史库告警的第一个记录

	char szAckOperator[78] = "";

	m_pHistoryAlarm->GetRecord(pHistory,uNum);
    
    int nParaCheck[15];

	for(j = uNum - 1; (j >= 0) && (nRecCnt < nRetCount) ;j--)
	{        
        memset(&nParaCheck, 0, 15 * sizeof(int));
 
        //判断第一个查询参数：告警类型
        switch (nAlarmType)
        {
        case 1:   //查询故障告警
            if (pHistory[j].yAlarmType == AT_FAULT)
            {
                nParaCheck[0] = 1;                 
            }
             
            break;
        case 2:   //查询事件告警
            if (pHistory[j].yAlarmType == AT_EVENT)
            {
                nParaCheck[0] = 1;
            }
             
            break;
        default:
            nParaCheck[0] = 1;
            break;
        }
      
        //判断第二个查询参数：恢复标志
        //查询已恢复或者未恢复告警
        if ((nResFlg[2] == 1) || ((nResFlg[0] == 1) && (nResFlg[1] == 1)))
        {
            nParaCheck[1] = 1;
        }
        //只查询已恢复告警
        else if ((nResFlg[0] == 1) && (nResFlg[1] == 0))
        {
            if (pHistory[j].yResType != NOT_RECOVER)
            {
                nParaCheck[1] = 1;
            }
        }
        //只查询未恢复告警
        else if ((nResFlg[0] == 0) && (nResFlg[1] == 1))
        {
            if (pHistory[j].yResType == NOT_RECOVER) 
            {
                nParaCheck[1] = 1;
            }
        }

	
        //判断第3、4个查询参数：起始流水号和终止流水号
		//第3、4个参数没有给出
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
 
        //判断第5、6个查询参数：起始告警编号和终止告警编号

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
 

        //确定第7个查询参数：告警级别        
        if (nAlmLvl[4] == 1) //所有级别
        {
            nParaCheck[6] = 1;
        } 

		if ( (nAlmLvl[pHistory[j].yAlarmLevel] == 1) 
			      && ( pHistory[j].yAlarmLevel < 5 ) 
			      && ( pHistory[j].yAlarmLevel > -1) ) 
		{
			nParaCheck[6] = 1;
		}


        //判断第8个查询参数：网管类型(iGateway Bill的告警属于“软件告警”
        if ((nASS[10] == 1) || (nASS[5] == 1))
        {
            nParaCheck[7] = 1; 
        }

        //判断第8,9,10,11个查询参数：查询起始日期、终止日期、起始时间、终止时间
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

        //判断第13个查询参数：模块号 
        if ((nModNum == 255) || (nModNum == pHistory[j].yModuleNO))
        {
            nParaCheck[12] = 1; 
        }

        //判断第20、21个查询参数：起始流水号和终止流水号
		//第20、21个参数没有给出
        //2004-07-23 ldf 对应问题单SWPD06736   
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
            //只要一个查询条件不满足，则认为该条告警不符合查询条件
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

   
	//若结果队列为空，则调用HandleCmdDefault发送操作无结果
	if(0 == pFoundAlarmList.size())
	{
		HandleCmdDefault(pFrame);
        delete[] pHistory;
		return;
	}
	
	//若结果队列不为空，则根据查询结构形成MML报文
    //by ldf 2002-10-29 对应问题单 D18685
    int nMMLPageNum = nRecCnt/m_nPageSize +1 ;
    pMMLPages = new STRING[nMMLPageNum];
    //修改完毕

	nTempCount = 0;
	nRecCnt = 0;
	nCurrentPage = 0;
	LIST<SHistoryAlarm *>::iterator Iter = pFoundAlarmList.begin();	
	
	while(Iter != pFoundAlarmList.end())
	{
		//构造一页MML报文头；
		nCurrentPage++;
		m_pAlarmMMLReport->MakeReportHead();
		m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(), m_nReportCsn);
		
        //据查询结果构造一页MML报文体；
		while(Iter != pFoundAlarmList.end())
		{  
	
			//若一条MML报文中的记录数大于一页MML报文最大的尺寸，则分页显示；
			if(nTempCount >= m_nPageSize)//实现分页显示
			{ 
				nTempCount = 0;
				if(nRecCnt < pFoundAlarmList.size())
				//by ldf 2002-10-29 对应问题单 D18685
                {
                    //对于中间页加上"TO BE CONTINUED..."
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                    m_pAlarmMMLReport->AddString(S_ALARM_TOBECONITUE);
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                }
                //修改完毕
				break;
			}

			//M2000联调加上告警之间的空行
			m_pAlarmMMLReport->AddString(STR_NEWLINE);
			m_pAlarmMMLReport->MakeQueryAlarmBody((*Iter), m_pAlarmCfg);

			nTempCount ++;
			nRecCnt ++;
			Iter ++;  
      	 }

        //by ldf 2002-10-29 对应问题单 D18685
        if (nRecCnt == pFoundAlarmList.size())
        {
            char szContent[32];

            //然后再加上类似于 “(结果个数=2)”的字样
            //S_ALARM_ALARMLIST_TOTALNUM
            SNPRINTF(szContent,32,S_ALARM_ALARMLIST_TOTALNUM,pFoundAlarmList.size());
            m_pAlarmMMLReport->AddString(szContent);
            m_pAlarmMMLReport->AddString(STR_NEWLINE);
            m_pAlarmMMLReport->AddString(STR_NEWLINE);

            //by ldf 2002-10-30 对应问题单 D18685 规范进一步修改
            if (nMMLPageNum > 1)
            {
                //对于最后一页加上	<共有><报告个数><个报告>
                //S_ALARM_ALARMREPORT_TOTALNUM
                SNPRINTF(szContent,32,S_ALARM_ALARMREPORT_TOTALNUM,nMMLPageNum);
                m_pAlarmMMLReport->AddString(szContent);
                m_pAlarmMMLReport->AddString(STR_NEWLINE);
                m_pAlarmMMLReport->AddString(STR_NEWLINE);
            }
            //修改完毕
        }
        //修改完毕

	
		//构造一页MML报文尾；
		m_pAlarmMMLReport->MakeReportTail();
		pMMLPages[nCurrentPage-1] = m_pAlarmMMLReport->GetReport();
	}
	
	//发送MML的报文
	for (int I = 0 ;I < nCurrentPage; I++)
	{
        SendMessageToMML(pMMLPages[I].c_str(),APP_MML_NORM,pFrame->IPAddr, pFrame->ClientNo);        
	}
	
	delete[] pMMLPages;
	delete[] pHistory;
	pFoundAlarmList.clear();

}

/////////////////////////////////////////////////////////////////////////////////
//功能描述  手动恢复当前告警，对应于RMV ALMFLT命令
//输入参数  MSG_INTRA *pFrame
//输出参数  无
//返回值    0 表示成功  负数表示失败
/////////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::HandleCmdAlarmDelete(MSG_INTRA *pFrame)
{
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);
    m_nReportCsn ++;

    int  iResFlag      = 0;  //是否有结果
    char szTmpOptr[78] = "";

    int nStartCsn;

    //取MML命令第一个参数：起始告警流水号（必须参数）
    pParaResult->GetDigitVal(&nStartCsn, 1, 1);

    UINT4 nRecNum = m_pHistoryAlarm->GetRecordNum();

    SActiveAlarm *pActive = NULL;

    //不但要处理告警队列，还要处理历史告警库.
    SHistoryAlarm *pHistory;
    pHistory = new SHistoryAlarm[nRecNum];
    m_pHistoryAlarm->Seek(0);
    m_pHistoryAlarm->GetRecord(pHistory, nRecNum);

    //手工恢复从nStartCsn开始的告警
    for (UINT4 i = 0; i < nRecNum; i++)
    {   //张玉新 2002-04-17 根据最新规范修改,问题单12740 Csn >= nStartCsn ==> ==
        if ((pHistory[i].uCsn == nStartCsn)
          && (pHistory[i].yAlarmType == AT_FAULT)
          && (pHistory[i].yResType   == NOT_RECOVER))
        {

            RECOVERY_FAULTALARM(&pHistory[i], m_pHistoryAlarm, MANNUAL_RECOVER);
            SInnerAlarm sInnerAlarm;
            sInnerAlarm.uAlarmID    = pHistory[i].uAlarmID;
            sInnerAlarm.yAlarmType  = AT_RESTORE;
            memcpy(sInnerAlarm.yAlarmPara, pHistory[i].yPara, MAX_ALARM_PARA);

            //在当前活动队列中添加告警
            //内部会分配内存来拷贝,所以可以使用局部变量
			//若为内部告警消息，添加到活动告警队列中，若添加失败则直接返回，否则继续处理；
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
                //张玉新 告警箱 添加
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

    //如果有记录被恢复生成MML报文
    delete[] pHistory;
    if (iResFlag == 1)
    {
        m_pAlarmMMLReport->MakeReportHead();
        m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(),m_nReportCsn);
        m_pAlarmMMLReport->AddString(STR_NEWLINE);
        m_pAlarmMMLReport->MakeRecoverAlarmBody(nStartCsn, nStartCsn, m_uBillsvrNo);
        m_pAlarmMMLReport->MakeReportTail();
        //现在从维护口发送一条告警
        SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_NORM,pFrame->IPAddr, pFrame->ClientNo);
#ifndef _OM_MODIFY
        //现在从上报口发送一条告警
        m_pAlarmMMLReport->MakeReportHead();
        m_pAlarmMMLReport->MakeRecoverAlarmBody(nStartCsn, nStartCsn, m_uBillsvrNo);
        m_pAlarmMMLReport->MakeReportTail();
        SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_WARN, pFrame->IPAddr, pFrame->ClientNo);        
#endif        

    }
    else //如果没有记录被恢复，则使用默认处理
    {
        HandleCmdDefault(pFrame);
    }

    return ERR_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////
//功能描述  默认处理，返回MML报文为查询无结果
//输入参数  内部消息帧
//输出参数  无
//返回值    0 表示成功  负数表示失败
/////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::HandleCmdDefault(MSG_INTRA *pFrame)
{
    //构造回送报文
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

    //由于对于操作无结果，只走维护口，所以向维护口发送报文数据；
    SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_NORM, pFrame->IPAddr, pFrame->ClientNo);    

    return ERR_SUCCESS;
}
///////////////////////////////////////////////////////////////////////////////
//功能描述  创建流水号
//输入参数  无
//输出参数  无
//返回值    新建流水号，0表示无效
///////////////////////////////////////////////////////////////////////////////
UINT4 CMMLAlarmHandler::CreateCsn()
{
    return m_pHistoryAlarm->CreateCsn();
}

//by ldf 2004-01-08 OM整改
UINT4 CMMLAlarmHandler::CreateSyn()
{
    return m_pHistoryAlarm->CreateSyn();
}

//////////////////////////////////////////////////////////////////////////////
//功能描述  回卷流水号
//输入参数  无
//输出参数  无
//返回值    回卷流水号,0表示无效
/////////////////////////////////////////////////////////////////////////////
UINT4 CMMLAlarmHandler::RollBackCsn()
{
    return m_pHistoryAlarm->RollBackCsn();
}

//by ldf 2004-01-08 OM整改
UINT4 CMMLAlarmHandler::RollBackSyn()
{
	return m_pHistoryAlarm->RollBackSyn();
}

/////////////////////////////////////////////////////////////////////////////
//功能描述  将pFrame中的信息填全,并发送到MML客户端
//输入参数  e
//
/////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::SendMessageToMML(const char *pMMLReport,
                                        UINT2 uAppType, UINT4 uIPAddr, UINT2 uClientNo)                                        
{
    if (pMMLReport == NULL)
    {
        return;
    }
    //计算报文长度，按长度分配空间
    int nLength       = strlen(pMMLReport);
    MSG_INTRA *pFrame = new(nLength + 1)MSG_INTRA;
    memcpy(pFrame->Data, pMMLReport, nLength);

    //最后一个字符为'\0 '，表示字符串结束
    pFrame->Data[nLength] = '\0';

    //填写帧头内容
    pFrame->IPAddr      = uIPAddr;
    pFrame->AppType     = uAppType;
    pFrame->ClientNo    = uClientNo;
    pFrame->SenderPid   = PT_OM;
    pFrame->SenderMid   = MT_ALARM;
    pFrame->ReceiverPid = PT_OUTSIDE;
    pFrame->ReceiverMid = MT_MMLDEBUG;    

    //调用框架的发送函数将消息帧发送到网络模块转发
    CWorkModule::SendMsg(pFrame);
}

/////////////////////////////////////////////////////////////////////////////////
//功能描述  从CParseResult中得到日期型参数值
//输入参数
//      pParaResult : 参数对象
//      nParaID     : 参数ID
//输出参数
//      pDate       : 获得的命令参数值
////////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::GetDateValue(CParseResult *pParaResult, int nParaID, tm *pDate)
{
    //获得年份参数
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
//功能描述  从CParseResult中得到时间型参数值
//输入参数
//      pParaResult  : 参数对象
//      nParaID      : 参数ID
//      pDefault     : 时间默认值
//
//输出参数
//      pTime        : 获得的命令参数值
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
//功能描述  从CParseResult中得到参数值并存到相应数组元素中
//输入参数
//      pParaResult : 参数对象
//      paraID      : 参数ID
//      array_size  : 数组尺寸
//      default_pos : 默认值在数组中的位置
//输出参数
//      pArray      : 获得的命令参数值
///////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::GetIntArrayValue(CParseResult *pParaResult, int nParaID,
                                        int *pArray, int nArraySize, int nDefPos)
{
    int nIndex;

    //初始化枚举数组为0
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
//功能描述  判断告警级别数组中的指定告警级别是否有效
//输入参数  alm_level     ——指定告警级别
//      pAlarmLevelArray  ——告警级别数组
//输出参数  无
//返回值    告警级别在告警类型数组中有效返回1，否则返回0
//////////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::AlmLevelTell(UINT2 uAlmLevel, int *pAlmLevelArray)
{
    //如果默认值有效，直接返回1
    if(pAlmLevelArray[4] == 1)
    {
        return 1;
    }

    //根据告警级别，来查询数组中的指定位置是否有效，每个指定位置，代表命令参数是否要查询这个级别的告警。1——表示Yes，0——表示No
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
//功能描述  判断SHistoryAlarm中的日期是否与指定日期相等
//输入参数  pHistory     ——历史告警结构
//      pCommit      ——指定的日期结构
//输出参数  无
//返回值    两者匹配返回1，否则返回0
///////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::AlmDateEqual(SHistoryAlarm *pHistory, tm *pCommit)
{
    //日期是否相等，但是tm结构中的年以1900年计数，月份以0计数，所以这里分别加上了1900和1。
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
//功能描述  判断制定日期是否有效
//输入参数  pDate  ——指定日期
//输出参数  无
//返回值    有效返回1，否则返回0
////////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::IsDate(tm *pDate)
{
    //当tm结构中的年、月、日都为零时表示是无效值
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
//功能描述  判断指定时间是否有效
//输入参数  pDate  - 指定时间
//输出参数  无
//返回值    有效返回1 否则返回0
////////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::IsTime(tm *pDate)
{
    //当tm结构中的时、分、秒都为零时表示是无效值
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
//功能描述  判断历史告警的产生日期在日期段之内
//输入参数
//      pHistory  : 历史告警
//      pStart    : 起始日期
//      pEnd      : 结束日期
//输出参数
//      无
//返回值
//      历史告警的产生日期在日期段之内返回1，否则返回0
/////////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::IsInDateRange(SHistoryAlarm *pHistory, tm *pStart, tm *pEnd)
{
    //定义临时变量，用于日期转换和判断
    tm tmpHistory,tmpStart,tmpEnd;
    time_t history,start, end;

    memcpy(&tmpStart, pStart, sizeof(tm));
    memcpy(&tmpEnd, pEnd, sizeof(tm));

    //转换历史告警的日期为tm结构
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

    //判断历史告警的日期是否在起始日期和结束日期之间
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
//功能描述  判断历史告警的产生时间在时间段之内
//输入参数  pHistory     ——历史告警
//      pStart       ——起始时间
//      pEnd         ——结束时间
//输出参数  无
//返回值    历史告警的产生时间在时间段之内返回1，否则返回0
//////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::IsInTimeRange(SHistoryAlarm *pHistory, tm *pStart, tm *pEnd)
{
       //定义临时变量，用于时间转换和判断
        tm temp_history;
        time_t history,start,end;
        temp_history.tm_isdst = pStart->tm_isdst;
        //转换历史告警的时间、起始时间和结束时间为tm结构,并且日期以1978年1月29日为同一比较日期的基础。
        temp_history.tm_mday = pHistory->ySDay;
        temp_history.tm_year = pHistory->uSYear - 1900;
        temp_history.tm_mon = pHistory->ySMonth - 1;
        temp_history.tm_hour = pHistory->ySHour;
        temp_history.tm_min = pHistory->ySMin;
        temp_history.tm_sec = pHistory->ySSec;

        //将tm结构转换为time_t时间的时间编码方式，time_t是C语言函数库的标准时间编码方式
        history = mktime(&temp_history);
        start = mktime(pStart);
        end = mktime(pEnd);
        //判断历史告警的时间是否在起始时间和结束时间之间，difftime是C语言标准库函数
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
        //设置自动恢复的时间为当前时间
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
        //恢复时如果故障被确认过则将恢复确认也填写了
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

//by ldf 2004-01-12 OM整改
int CMMLAlarmHandler::HandleCmdAlarmCHK(MSG_INTRA *pFrame)
{
	static BOOL bHaveBaseID = FALSE;
	
	//MML报文生成的前期工作   
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
			//取MML参数：MML告警BaseId
			bHaveBaseID = pParaResult->GetDigitVal(&m_nMMLBaseID, 1, 1);
			//m_pHistoryAlarm->m_uMMLAlmBaseID 为0时表示CG从未接收过同步基准值
			m_pHistoryAlarm->WriteMMLBaseID(m_nMMLBaseID); //只CG第一次收到基准值后记录，重启后也不再改变
			m_pAlarmMMLReport->SetMMLBaseAlarmID(m_nMMLBaseID);    //缺省值M2000基准值为 0
		}


	}
	//构造一页MML报文头
	m_pAlarmMMLReport->MakeReportHead();
	if(bHaveBaseID)
	{
			m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(), 0, 0);
	}
	else
	{
			m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(), 0, 1);
	}
	
	//根据查询结果构造一页MML报文体
	//告警联调加上告警之间的空行
	m_pAlarmMMLReport->AddString(STR_NEWLINE);

	//by ldf 2004-01-13 OM整改
	m_pAlarmMMLReport->MakeReportItem(S_ALARM_NEFLAG, "",20,ALIGN_RIGHT);
	m_pAlarmMMLReport->AddString(STR_NEWLINE);
	//end
	
	//构造一页MML报文尾
	m_pAlarmMMLReport->MakeReportTail();
	
	//由于对于操作无结果，只走维护口，所以向维护口发送报文数据；
    SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_NORM, pFrame->IPAddr, pFrame->ClientNo);    

	return bHaveBaseID;

}

int CMMLAlarmHandler::QueryAlmLog_Impl(SAlarmQueryCond &condition,LIST<SHistoryAlarm> &FoundAlarmList)
{
    FoundAlarmList.clear();
    UINT4 uNum = m_pHistoryAlarm->GetRecordNum();

    if(uNum == 0)   //历史告警库中不存在告警记录
    {
        return 0;
    }
    int nRecCnt = 0;
    SHistoryAlarm *pHistory = new SHistoryAlarm[uNum];
    m_pHistoryAlarm->Seek(0);  //将指针移到历史库告警的第一个记录

    char szAckOperator[78] = "";

    m_pHistoryAlarm->GetRecord(pHistory,uNum);

    int nParaCheck[14];
    int j = 0;
    for(j = uNum - 1; (j >= 0) && (nRecCnt < condition.nRetCount) ;j--)
    {
        memset(&nParaCheck, 0, 14 * sizeof(int));

        //判断第一个查询参数：告警类型
        switch(condition.nAlarmType)
        {
        case 1:   //查询故障告警
            if(pHistory[j].yAlarmType == AT_FAULT)
            {
                nParaCheck[0] = 1;
            }

            break;
        case 2:   //查询事件告警
            if(pHistory[j].yAlarmType == AT_EVENT)
            {
                nParaCheck[0] = 1;
            }

            break;
        default:
            nParaCheck[0] = 1;
            break;
        }

        //判断第二个查询参数：恢复标志
        //查询已恢复或者未恢复告警
        if((condition.nResFlg[2] == 1)
          || ((condition.nResFlg[0] == 1) && (condition.nResFlg[1] == 1)))
        {
            nParaCheck[1] = 1;
        }
        //只查询已恢复告警
        else if((condition.nResFlg[0] == 1)
                && (condition.nResFlg[1] == 0))
        {
            if(pHistory[j].yResType != NOT_RECOVER)
            {
                nParaCheck[1] = 1;
            }
        }
        //只查询未恢复告警
        else if((condition.nResFlg[0] == 0)
                && (condition.nResFlg[1] == 1))
        {
            if(pHistory[j].yResType == NOT_RECOVER)
            {
                nParaCheck[1] = 1;
            }
        }

        //判断第三个查询参数：确认标志
        //查询已确认或者未确认告警
        if(condition.nAckFlg[2] == 1
            ||(condition.nAckFlg[0] == 1 && condition.nAckFlg[1] == 1))
        {
            nParaCheck[2] = 1;
        }
        //只查询已确认告警
        else if((pHistory[j].szFAckOptr[0] != '\0')
                 && (condition.nAckFlg[0] == 1))
        {
             nParaCheck[2] = 1;
        }
        //只查询未确认告警
        //by ldf 2002.1.18
        else if((pHistory[j].szFAckOptr[0] == '\0')
               && (condition.nAckFlg[1] == 1))
        {
             nParaCheck[2] = 1;
        }

        //判断第四个查询参数：模块号
        if((condition.nModNum == 255)
           ||(condition.nModNum == pHistory[j].yModuleNO))
        {
            nParaCheck[3] = 1;
        }

        //判断第五、第六个查询参数：起始流水号和终止流水号
        //第五、六个参数没有给出
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

        //判断第七、第八个查询参数：起始告警编号和终止告警编号

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


        //确定第九个查询参数：告警级别
        if(condition.nAlmLvl[4] == 1) //所有级别
        {
            nParaCheck[8] = 1;
        }
        else if((condition.nAlmLvl[pHistory[j].yAlarmLevel] == 1)
                  && ( pHistory[j].yAlarmLevel < 5 )
                  && ( pHistory[j].yAlarmLevel > -1) )
        {
            nParaCheck[8] = 1;
        }


        //判断第十个查询参数：网管类型(iGateway Bill的告警属于“软件告警”
        if((condition.nASS[10] == 1) || (condition.nASS[5] == 1))
        {
            nParaCheck[9] = 1;
        }

        //判断第十一、十二、十三、十四个查询参数：查询起始日期、终止日期、起始时间、终止时间
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
            //只要一个查询条件不满足，则认为该条告警不符合查询条件
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
//功能描述  处理删除历史告警命令，对应RMV ALMLOG
//输入参数  内部数据帧
//输出参数  无
//返回值    0 表示成功  负数表示失败
/////////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::HandleCmdAlarmClear(MSG_INTRA *pFrame)
{
    //MML报文生成的前期工作
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);
    m_nReportCsn++;

    tm StartDate;
    tm EndDate;

    //取MML命令第一个参数：起始日期（必须参数）
    GetDateValue(pParaResult, 1, &StartDate);

    //取MML命令第二个参数：终止日期（必须参数）
    GetDateValue(pParaResult, 2, &EndDate);

    //如果终止日期小于起始日期
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
    //张玉新 2002-04-17 问题单12740，最多删除500条告警
    int nCnt = 0;
    //修改结束
    int nFindFlag      = 0;//查询结果变量
    int nRet           = ERR_SUCCESS;
    char szTmpOptr[78] = "";
    int nFltAlmDelFlg = 0;
    int nEvtAlmDelFlg = 0;
    UINT4 nRecNum = m_pHistoryAlarm->GetRecordNum();
    SHistoryAlarm *pHistory = new SHistoryAlarm[nRecNum];
    m_pHistoryAlarm->Seek(0);
    m_pHistoryAlarm->GetRecord(pHistory,nRecNum);

    //删除历史告警中介于StartDate和EndDate之间的告警，
    //注意：未恢复的故障告警不得删除
    for (UINT4 I = 0 ; I < nRecNum; I++)
    {
        if (IsInDateRange(&pHistory[I], &StartDate, &EndDate))
        {
            nFltAlmDelFlg = (pHistory[I].yAlarmType == AT_FAULT)
				             && (pHistory[I].yResType != NOT_RECOVER);				             ;
            if(pHistory[I].yAlarmType == AT_EVENT)
            {
                //对于紧急事件告警，未确认就不能删除
			    if(pHistory[I].yAlarmLevel == AL_EMERGENCY)
                {   nEvtAlmDelFlg = 0; }
                else
                {   nEvtAlmDelFlg = 1; }

            }
            if(nFltAlmDelFlg || nEvtAlmDelFlg)
            {
                nFindFlag = 1;
                //删除历史告警
                nRet = m_pHistoryAlarm->DeleteRecord(pHistory[I].uSyn);
                if (nRet != ERR_SUCCESS)
                {
                    TRACE(MTS_ALARM, S_ALARM_DEL_HIS_ERR);
                    delete[] pHistory;
                    return ERR_FAIL;
                }

                //清除活动告警
                LIST<SActiveAlarm *>::iterator it;
                m_nFindNode.m_uCsn = pHistory[I].uCsn;
                it = std::find_if(m_ActiveAlarmList.begin(),
                                  m_ActiveAlarmList.end(), m_nFindNode);
                if (it != m_ActiveAlarmList.end())
                {
                    delete (*it);
                    m_ActiveAlarmList.erase(it);
                }
                nCnt ++; //张玉新 2002-04-17 问题单12740
            }
            //张玉新 2002-04-17 问题单 12740
            if(nCnt >= DEFAULT_CMD_DEL_LIMIT)
            {
                TRACE(MTS_ALARM,S_ALARM_DEL_ALM_LIMIT);
                break;
            }
            //修改结束
        }

    } //for 循环结束

    delete[] pHistory;

    //形成MML报文
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

        //发送MML报文
        SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_NORM, pFrame->IPAddr, pFrame->ClientNo);        
    }
    else //没有符合条件的结果则使用默认的处理
    {
        HandleCmdDefault(pFrame);
    }

    return ERR_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//功能描述  处理故障告警确认命令，对应SET AMLACK命令
//输入参数
//        pFrame  : 内部数据帧
//输出参数
//        无
//返回值
//        0 表示成功  负数表示失败
///////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::HandleCmdAlarmAck(MSG_INTRA *pFrame)
{
    //MML报文生成的前期工作
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);//unit test
    m_nReportCsn ++;

    int  nStartCsn   = 0;       //张玉新 2002-04-17 问题单12743
    int  nEndCsn     = 0;       //张玉新 2002-04-17 问题单12743
    int  nCnt        = 0;       //张玉新 2002-04-17 问题单12743
    BOOL bFailAll    = TRUE;    //张玉新 2002-04-17 问题单12743
    BOOL bFirstPara  = TRUE;    //张玉新 2002-04-17 问题单12743
    BOOL bSecondPara = TRUE;    //张玉新 2002-04-17 问题单12743
    //获得MML第一个参数：起始流水号（必须参数）
    //张玉新 2002-04-17 问题单12743
    bFirstPara = pParaResult->GetDigitVal(&nStartCsn, 1, 1);

    //获得MML第二个参数：终止流水号（必须参数）
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
    //修改结束

    if (nStartCsn > nEndCsn)
    {
        nEndCsn = nStartCsn;
    }

    UINT4 nAlarmCount = m_pHistoryAlarm->GetRecordNum();
    int nRes = 1;    //表示结果

    char szAckOperator[78] = "";

    struct in_addr sin;
    sin.S_un.S_addr = pFrame->IPAddr;
    char *szIP = inet_ntoa(sin);

    SHistoryAlarm *pHistory = new SHistoryAlarm[nAlarmCount];
    m_pHistoryAlarm->Seek(0);
    m_pHistoryAlarm->GetRecord(pHistory, nAlarmCount);

    //by ldf 2002-10-29 对应问题单 D18685
    char AckCsn[10];
    //修改完毕

    for (UINT4 i = 0; i < nAlarmCount; i++)
    {
        //只处理告警流水号落在nStartCsn和nEndCsn之间的告警
        //张玉新 2002-04-17 问题单12743 一次只能确认200条
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
         // pHistory[i].yResType   = MANNUAL_RECOVER;  //手工恢复

            //如果是故障告警并且没有被确认
            if ((pHistory[i].yAlarmType == AT_FAULT)
                 && (pHistory[i].szFAckOptr[0] == '\0'))
            {
                sprintf(pHistory[i].szFAckOptr, "%s:%s", pFrame->UserName, szIP);
                //如果告警被确认时已经恢复，就连恢复确认一起加上
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
                //张玉新 2002-04-17 问题单12743
                if(nRes == ERR_SUCCESS)
                {
                    //by ldf 2002-10-29 对应问题单 D18685
                    sprintf(AckCsn, "%d", pHistory[i].uCsn);
                    CsnAcked.push_back(AckCsn);
                    //修改完毕

                    nCnt ++;
                    bFailAll = FALSE;
                }
                else
                {
                    TRACE(MTS_ALARM,S_ALARM_ACK_WRITE_ERR,pHistory[i].uCsn);
                }
                //修改结束,这个语句必须在这里重复两次
            }
            //如果是未确认的一级事件告警
            else if ((pHistory[i].yAlarmType    == AT_EVENT)
                  && (pHistory[i].yAlarmLevel   == AL_EMERGENCY)
                  && (pHistory[i].szFAckOptr[0] == '\0'))
            {
                sprintf(pHistory[i].szFAckOptr, "%s:%s", pFrame->UserName, szIP);
                nRes = m_pHistoryAlarm->RewriteRecord(&pHistory[i]);
                //张玉新 2002-04-17 问题单12743
                if(nRes == ERR_SUCCESS)
                {
                    //by ldf 2002-10-29 对应问题单 D18685
                    sprintf(AckCsn, "%d", pHistory[i].uCsn);
                    CsnAcked.push_back(AckCsn);
                    //修改完毕

                    nCnt ++;
                    bFailAll = FALSE;
                    //zyx 联调加上
                    m_ActiveAlarmList.RemoveAlarm(pHistory[i].yAlarmType,
                                                  pHistory[i].uAlarmID,
                                                  pHistory[i].yPara);
                }
                else
                {
                    TRACE(MTS_ALARM,S_ALARM_ACK_WRITE_ERR,pHistory[i].uCsn);
                }
                //修改结束
            }
            //张玉新 2002-04-17 问题单12743，对于告警的重复确认也认为确认成功
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
            //修改结束
        }
    }

    delete[] pHistory;
    //张玉新 2002-04-17 问题单12743
    if(bFailAll == FALSE)
    {
        //by ldf 2002-10-29 对应问题单 D18685
        //char sCsn[10];
        //char eCsn[10];

        sprintf(szAckOperator, "%s:%s", pFrame->UserName, szIP);

        //by ldf 2002-10-29 对应问题单 D18685
        //sprintf(sCsn, "%d", nStartCsn);
        //sprintf(eCsn, "%d", nEndCsn);

        //从维护口走一条告警
        m_pAlarmMMLReport->MakeReportHead();
        m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(), m_nReportCsn);
        //by ldf 2002-10-29 对应问题单 D18685
        m_pAlarmMMLReport->MakeAlarmFaultAckBody(szAckOperator,S_ALARM_AT_FAULT, CsnAcked);
        //修改完毕
        m_pAlarmMMLReport->MakeReportTail();
        SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_NORM, pFrame->IPAddr, pFrame->ClientNo);
        
        //从上报口走一条告警
        m_pAlarmMMLReport->MakeReportHead();
        //by ldf 2002-10-29 对应问题单 D18685
        m_pAlarmMMLReport->MakeAlarmFaultAckBody(szAckOperator, S_ALARM_AT_FAULT, CsnAcked);
        //修改完毕
        m_pAlarmMMLReport->MakeReportTail();
        SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_WARN, pFrame->IPAddr, pFrame->ClientNo);
        
        //张玉新 2002-04-17 问题单12743
        if(nCnt >= DEFAULT_ACK_CNT)
        {
            TRACE(MTS_ALARM,S_ALARM_ACK_REACH_LIMIT);
        }
    }
    else
    {
        HandleCmdDefault(pFrame);
    }
    //修改结束
    return ERR_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//功能描述  处理事件告警确认命令，对应SET EVTACK
//输入参数
//        pFrame  : 内部数据帧
//输出参数
//        无
//返回值
//        0 表示成功  负数表示失败
///////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::HandleCmdAlarmAckEVT(MSG_INTRA *pFrame)
{
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);

    int nCsn;
    tm Date;

    //获得MML命令第一个参数：告警流水号（必须参数）
    pParaResult->GetDigitVal(&nCsn, 1, 1);

    //获得MML命令第二个参数：确认日期（必须参数）
    GetDateValue(pParaResult, 2, &Date);

    //处理IP地址
    struct in_addr sin;
    sin.S_un.S_addr = pFrame->IPAddr;
    char *szIP = inet_ntoa(sin);

    int  iRes;
    iRes = m_pHistoryAlarm->FindRecord(nCsn);
    UINT4 uNum = 1;

    if (iRes == -1)  //未找到对应的告警
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
    //zyx 告警箱联调增加
    if(sHistory.yAlarmType == AT_EVENT
        && sHistory.yAlarmLevel == AL_EMERGENCY)
    {   //如果是紧急事件告警要从告警队列中删掉
        m_ActiveAlarmList.RemoveAlarm(sHistory.yAlarmType,sHistory.uAlarmID,sHistory.yPara);
    }
    char szCsn[11];
    sprintf(szCsn, "%d", nCsn);
    m_pAlarmMMLReport->MakeReportHead();
    m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(),m_nReportCsn);
    //by ldf 2002-10-29 对应问题单 D18685
    m_pAlarmMMLReport->MakeAlarmEventAckBody(sHistory.szFAckOptr, S_ALARM_AT_EVENT, szCsn);
    //修改完毕
    m_pAlarmMMLReport->MakeReportTail();

    //从维护口走的一条告警
    SendMessageToMML(m_pAlarmMMLReport->GetReport(),APP_MML_NORM,pFrame->IPAddr, pFrame->ClientNo);
    
    //生成从上报口走的MML告警报文
    m_pAlarmMMLReport->MakeReportHead();
    //by ldf 2002-10-29 对应问题单 D18685
    m_pAlarmMMLReport->MakeAlarmEventAckBody(sHistory.szFAckOptr, S_ALARM_AT_EVENT, szCsn);
    //修改完毕
    m_pAlarmMMLReport->MakeReportTail();
    
    //从上报口送一条告警
    SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_WARN, pFrame->IPAddr, pFrame->ClientNo);    

    return ERR_SUCCESS;
}
	
/////////////////////////////////////////////////////////////////////////////
//功能描述  告警详细信息查询，本命令对应LST ALMINF
//输入参数
//       pFrame : 内部数据帧
/////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::HandleCmdAlarmDetail(MSG_INTRA *pFrame)
{
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);//unit test

    m_nReportCsn ++;

    int nCsn;
    //取MML命令第一个参数：告警流水号（必须参数）
    pParaResult->GetDigitVal(&nCsn, 1, 1);

    SHistoryAlarm sHistory;
    UINT4 uNum = 1;

    //查找指定的告警纪录
    int iRes = m_pHistoryAlarm->FindRecord(nCsn);
    if (iRes != ERR_FAIL)
    {
        m_pHistoryAlarm->Seek(iRes);
        m_pHistoryAlarm->GetRecord(&sHistory, uNum);

        //根据查询到的记录生成MML报文
        m_pAlarmMMLReport->MakeReportHead();
        m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(), m_nReportCsn);
        m_pAlarmMMLReport->MakeDetailAlarmBody(&sHistory, m_pAlarmCfg);
        m_pAlarmMMLReport->MakeReportTail();

        SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_NORM, pFrame->IPAddr, pFrame->ClientNo);        
    }
    else //没有找到记录的话，使用默认处理
    {
        HandleCmdDefault(pFrame);
        return;
    }
}
/////////////////////////////////////////////////////////////////////////////////
//功能描述	处理当前告警查询命令,本命令对应LST IGWALMFE
//输入参数	
//       pFrame : 内部数据帧
/////////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::HandleCmdIGWAlarmQuery(MSG_INTRA *pFrame)
{
	//MML报文生成的前期工作   
	CParseResult *pParaResult = *((CParseResult **)pFrame->Data);

	int nRecCnt      = 0;   //查到的记录数
	int nTempCount   = 0;   //针对一页不能超过40条记录进行计数
	int nCurrentPage = 0;   //用于记录页数
	STRING  *pMMLPages;     //存放生成的众多MML报文	

	char szTmpOptr[78];
    memset(szTmpOptr, 0, 78);
	
	m_nReportCsn ++;       //每次报文流水号＋1   
	
    int nAlarmType;
    int nRetCount;

    //取第一个MML参数：告警类型，缺省为所有告警
    if (pParaResult->GetDigitVal(&nAlarmType, 1, 1) == FALSE)
    {
         nAlarmType = 3;    //缺省查询所有告警
    }
   
    //取第二个MML参数：返回条数，缺省为64条
    if (pParaResult->GetDigitVal(&nRetCount, 2, 1) == FALSE)
    {
        nRetCount = 64;    //缺省返回64条数
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
		//如果查询事件告警的话
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
		//查询全部告警的时候
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
	}//for 循环结束

	//若结果队列为空,则调用HandleCmdDefault()发送操作无结果
	if (0 == TmpList.size())
	{
		delete[] pHistoryArray;
		HandleCmdDefault(pFrame);
		return;
	}
   
	//若结果队列不为空，则根据查询结构形成MML报文

    //by ldf 2002-10-29 对应问题单 D18685
    int nMMLPageNum = nRecCnt/m_nPageSize +1 ;
    pMMLPages = new STRING[nMMLPageNum];   //计算返回的MML报文需要的页数
    //修改完毕
	nTempCount   = 0;
	nRecCnt      = 0;
	nCurrentPage = 0;
	//ActiveIter = TmpList.begin();
	LIST<SHistoryAlarm *>::iterator Iter = TmpList.begin();
	SHistoryAlarm* pHistoryAlarm = NULL;

	while (Iter != TmpList.end())
	{
		//构造一页MML报文头
		nCurrentPage++;
		m_pAlarmMMLReport->MakeReportHead();
		m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(), m_nReportCsn);
		
		//根据查询结果构造一页MML报文体
		while (Iter != TmpList.end())
		{  
 
			//若一条MML报文中的记录数大于一页MML报文最大的尺寸，则分页显示			
			if (nTempCount >= m_nPageSize) //实现分页显示
			{ 
				nTempCount = 0;
				if (nRecCnt < TmpList.size())
                {
				    //对于中间页加上"TO BE CONTINUED..."
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
				    m_pAlarmMMLReport->AddString(S_ALARM_TOBECONITUE);
                    //by ldf 2002-10-29 对应问题单 D18685
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                    //修改完毕
                }

				break;
			}

            //告警联调加上告警之间的空行
			m_pAlarmMMLReport->AddString(STR_NEWLINE);
			m_pAlarmMMLReport->MakeQueryAlarmBody((*Iter), m_pAlarmCfg);

			nTempCount ++;
			nRecCnt ++;
			Iter ++;  
      	}

        //by ldf 2002-10-29 对应问题单 D18685
        if (nRecCnt == TmpList.size())
        {
            char szContent[32];

            //然后再加上类似于 “(结果个数=2)”的字样
            //S_ALARM_ALARMLIST_TOTALNUM
            SNPRINTF(szContent,32,S_ALARM_ALARMLIST_TOTALNUM,TmpList.size());
            m_pAlarmMMLReport->AddString(szContent);
            m_pAlarmMMLReport->AddString(STR_NEWLINE);
            m_pAlarmMMLReport->AddString(STR_NEWLINE);

            //by ldf 2002-10-30 对应问题单 D18685 规范进一步修改
            if (nMMLPageNum > 1)
            {
                //对于最后一页加上	<共有><报告个数><个报告>
                //S_ALARM_ALARMREPORT_TOTALNUM
                SNPRINTF(szContent,32,S_ALARM_ALARMREPORT_TOTALNUM,nMMLPageNum);
                m_pAlarmMMLReport->AddString(szContent);
                m_pAlarmMMLReport->AddString(STR_NEWLINE);
                m_pAlarmMMLReport->AddString(STR_NEWLINE);
            }
            //修改完毕
        }
        //修改完毕

		//构造一页MML报文尾
		m_pAlarmMMLReport->MakeReportTail();
		pMMLPages[nCurrentPage-1] = m_pAlarmMMLReport->GetReport();
	}

	//依次发送MML的报文
	for (int I = 0 ; I < nCurrentPage; I++)
	{
        SendMessageToMML(pMMLPages[I].c_str(), APP_MML_NORM, pFrame->IPAddr, pFrame->ClientNo);        
	}
	delete[] pHistoryArray;
	delete[] pMMLPages;
	TmpList.clear();
}
//////////////////////////////////////////////////////////////////////////////
//功能描述	处理历史告警查询命令,本命令对应LST IGWALMLOG
//输入参数	
//       pFrame : 内部数据帧
//////////////////////////////////////////////////////////////////////////////
void CMMLAlarmHandler::HandleCmdIGWAlarmQueryLOG(MSG_INTRA *pFrame)
{
	//MML报文生成的前期工作   
	CParseResult *pParaResult = *((CParseResult **)pFrame->Data);//unit test change
	m_nReportCsn ++;
	
	int nRecCnt       = 0;    //查到的记录数	
	int nTempCount    = 0;    //用于页计数，针对一页不能超过40条记录	
	int nCurrentPage  = 0;    //用于记录页数	
	STRING *pMMLPages = NULL; //存放生成的众多MML报文  	
    
    int j;  //循环计数

    int nAlarmType;    //告警类型

    //取MML命令第一个参数：告警类型（必须参数）
    pParaResult->GetDigitVal(&nAlarmType, 1, 1);
   
    int nResFlg[3];	   //恢复标志
     
    //取MML命令第二个参数：恢复标志（必须参数）
    GetIntArrayValue(pParaResult, 2, nResFlg, 3, 2);
    
    int nAckFlg[3];    //确认标志
 
    //取MML命令第三个参数：确认标志（必须参数）   
    GetIntArrayValue(pParaResult, 3, nAckFlg, 3, 2);

    int nModNum;      //模块号
    
    //取MML命令第四个参数：模块号（可选参数，缺省为全部模块）
    if (pParaResult->GetDigitVal(&nModNum, 4, 1) == FALSE)
    {
        nModNum = 255;
    }

    int nStartCsn;    //起始流水号

    //取MML命令第五个参数：起始告警流水号（必须参数）    
	//如果没有第五、六个参数给出，则nCsnDef 为 0
    int nCsnDef = pParaResult->GetDigitVal(&nStartCsn, 5, 1);
    
    int nEndCsn;      //终止流水号
    
    //取MML命令第六个参数：终止告警流水号（必须参数）    
    pParaResult->GetDigitVal(&nEndCsn, 6, 1);

    int nStartAlarmID;   //起始告警编号
    
    //取MML命令第七个参数：起始告警编号（必须参数）   
	//如果没有第七、八个参数给出，则 nAlarmIDDef 为 0
    int nAlarmIDDef = pParaResult->GetDigitVal(&nStartAlarmID, 7, 1);

    int nEndAlarmID;     //终止告警编号
    
    //取MML命令第八个参数：终止告警编号（必须参数） 
    pParaResult->GetDigitVal(&nEndAlarmID, 8, 1);

    int nAlmLvl[5];     //告警级别
    
    //取MML命令第九个参数：告警级别（必须参数）
    GetIntArrayValue(pParaResult, 9, nAlmLvl, 5, 4);

    int nASS[11];       //网管分类
    
    //取MML命令第十个参数：网管告警分类（必须参数）
    GetIntArrayValue(pParaResult, 10, nASS, 11, 10);
	
    tm StartDate;
    tm EndDate;
	
    //取MML命令第11、12、13、14个参数：起始日期、终止日期、起始时间、终止时间（必须参数）
	//by ldf 2002.1.18 先判断是否日期时间被指定，如没有则nDateDef为0
    int nDateDef = pParaResult->GetDigitVal(&nDateDef, 11, 1);
	if ( nDateDef != 0 )
	{
		GetDateValue(pParaResult, 11, &StartDate);
		GetDateValue(pParaResult, 12, &EndDate);
		GetTimeValue(pParaResult, 13, &StartDate);
		GetTimeValue(pParaResult, 14, &EndDate);    
	}
    
    int nRetCount;   //返回记录数
    
    //取MML命令第15个参数：返回记录数（必须参数）
    pParaResult->GetDigitVal(&nRetCount, 15, 1);
    if(nRetCount == 0 || nRetCount >= 600)
	{
		nRetCount = 600;
	}
    //LST ALMLOG命令的第16－19参数与iGateway Bill无关，因此不考虑
	

	//遍历历史告警库，查找符合条件的告警记录并形成查询结果队列；
	LIST<SHistoryAlarm *> pFoundAlarmList;
    UINT4 uNum = m_pHistoryAlarm->GetRecordNum();
    
    if (uNum == 0)   //历史告警库中不存在告警记录
    {
		HandleCmdDefault(pFrame);
        return;        
    }

	SHistoryAlarm *pHistory = new SHistoryAlarm[uNum];
	m_pHistoryAlarm->Seek(0);  //将指针移到历史库告警的第一个记录

	char szAckOperator[78] = "";

	m_pHistoryAlarm->GetRecord(pHistory,uNum);
    
    int nParaCheck[14];

	for(j = uNum - 1; (j >= 0) && (nRecCnt < nRetCount) ;j--)
	{        
        memset(&nParaCheck, 0, 14 * sizeof(int));
 
        //判断第一个查询参数：告警类型
        switch (nAlarmType)
        {
        case 1:   //查询故障告警
            if (pHistory[j].yAlarmType == AT_FAULT)
            {
                nParaCheck[0] = 1;                 
            }
             
            break;
        case 2:   //查询事件告警
            if (pHistory[j].yAlarmType == AT_EVENT)
            {
                nParaCheck[0] = 1;
            }
             
            break;
        default:
            nParaCheck[0] = 1;
            break;
        }
      
        //判断第二个查询参数：恢复标志
        //查询已恢复或者未恢复告警
        if ((nResFlg[2] == 1) || ((nResFlg[0] == 1) && (nResFlg[1] == 1)))
        {
            nParaCheck[1] = 1;
        }
        //只查询已恢复告警
        else if ((nResFlg[0] == 1) && (nResFlg[1] == 0))
        {
            if (pHistory[j].yResType != NOT_RECOVER)
            {
                nParaCheck[1] = 1;
            }
        }
        //只查询未恢复告警
        else if ((nResFlg[0] == 0) && (nResFlg[1] == 1))
        {
            if (pHistory[j].yResType == NOT_RECOVER) 
            {
                nParaCheck[1] = 1;
            }
        }

        //判断第三个查询参数：确认标志         
        //查询已确认或者未确认告警
        if (nAckFlg[2] == 1)
        {
            nParaCheck[2] = 1;   
        }
        //只查询已确认告警
        else if ((pHistory[j].szFAckOptr[0] != '\0') && (nAckFlg[0] == 1))
        {
             nParaCheck[2] = 1;
        }
        //只查询未确认告警
		//by ldf 2002.1.18
        else if ((pHistory[j].szFAckOptr[0] == '\0') && (nAckFlg[1] == 1))
        {
             nParaCheck[2] = 1;
        } 
        
        //判断第四个查询参数：模块号 
        if ((nModNum == 255) || (nModNum == pHistory[j].yModuleNO))
        {
            nParaCheck[3] = 1; 
        }
		
        //判断第五、第六个查询参数：起始流水号和终止流水号
		//第五、六个参数没有给出
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
 
        //判断第七、第八个查询参数：起始告警编号和终止告警编号

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
 

        //确定第九个查询参数：告警级别        
        if (nAlmLvl[4] == 1) //所有级别
        {
            nParaCheck[8] = 1;
        } 

		if ( (nAlmLvl[pHistory[j].yAlarmLevel] == 1) 
			      && ( pHistory[j].yAlarmLevel < 5 ) 
			      && ( pHistory[j].yAlarmLevel > -1) ) 
		{
			nParaCheck[8] = 1;
		}


        //判断第十个查询参数：网管类型(iGateway Bill的告警属于“软件告警”
        if ((nASS[10] == 1) || (nASS[5] == 1))
        {
            nParaCheck[9] = 1;            
        }

        //判断第十一、十二、十三、十四个查询参数：查询起始日期、终止日期、起始时间、终止时间
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
            //只要一个查询条件不满足，则认为该条告警不符合查询条件
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

   
	//若结果队列为空，则调用HandleCmdDefault发送操作无结果
	if(0 == pFoundAlarmList.size())
	{
		HandleCmdDefault(pFrame);
        delete[] pHistory;
		return;
	}
	
	//若结果队列不为空，则根据查询结构形成MML报文
    //by ldf 2002-10-29 对应问题单 D18685
    int nMMLPageNum = nRecCnt/m_nPageSize +1 ;
    pMMLPages = new STRING[nMMLPageNum];
    //修改完毕

	nTempCount = 0;
	nRecCnt = 0;
	nCurrentPage = 0;
	LIST<SHistoryAlarm *>::iterator Iter = pFoundAlarmList.begin();	
	
	while(Iter != pFoundAlarmList.end())
	{
		//构造一页MML报文头；
		nCurrentPage++;
		m_pAlarmMMLReport->MakeReportHead();
		m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(), m_nReportCsn);
		
        //据查询结果构造一页MML报文体；
		while(Iter != pFoundAlarmList.end())
		{  
	
			//若一条MML报文中的记录数大于一页MML报文最大的尺寸，则分页显示；
			if(nTempCount >= m_nPageSize)//实现分页显示
			{ 
				nTempCount = 0;
				if(nRecCnt < pFoundAlarmList.size())
				//by ldf 2002-10-29 对应问题单 D18685
                {
                    //对于中间页加上"TO BE CONTINUED..."
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                    m_pAlarmMMLReport->AddString(S_ALARM_TOBECONITUE);
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                    m_pAlarmMMLReport->AddString(STR_NEWLINE);
                }
                //修改完毕
				break;
			}

			//M2000联调加上告警之间的空行
			m_pAlarmMMLReport->AddString(STR_NEWLINE);
			m_pAlarmMMLReport->MakeQueryAlarmBody((*Iter), m_pAlarmCfg);

			nTempCount ++;
			nRecCnt ++;
			Iter ++;  
      	 }

        //by ldf 2002-10-29 对应问题单 D18685
        if (nRecCnt == pFoundAlarmList.size())
        {
            char szContent[32];

            //然后再加上类似于 “(结果个数=2)”的字样
            //S_ALARM_ALARMLIST_TOTALNUM
            SNPRINTF(szContent,32,S_ALARM_ALARMLIST_TOTALNUM,pFoundAlarmList.size());
            m_pAlarmMMLReport->AddString(szContent);
            m_pAlarmMMLReport->AddString(STR_NEWLINE);
            m_pAlarmMMLReport->AddString(STR_NEWLINE);

            //by ldf 2002-10-30 对应问题单 D18685 规范进一步修改
            if (nMMLPageNum > 1)
            {
                //对于最后一页加上	<共有><报告个数><个报告>
                //S_ALARM_ALARMREPORT_TOTALNUM
                SNPRINTF(szContent,32,S_ALARM_ALARMREPORT_TOTALNUM,nMMLPageNum);
                m_pAlarmMMLReport->AddString(szContent);
                m_pAlarmMMLReport->AddString(STR_NEWLINE);
                m_pAlarmMMLReport->AddString(STR_NEWLINE);
            }
            //修改完毕
        }
        //修改完毕

	
		//构造一页MML报文尾；
		m_pAlarmMMLReport->MakeReportTail();
		pMMLPages[nCurrentPage-1] = m_pAlarmMMLReport->GetReport();
	}
	
	//发送MML的报文
	for (int I = 0 ;I < nCurrentPage; I++)
	{
        SendMessageToMML(pMMLPages[I].c_str(),APP_MML_NORM,pFrame->IPAddr, pFrame->ClientNo);        
	}
	
	delete[] pMMLPages;
	delete[] pHistory;
	pFoundAlarmList.clear();
}
/////////////////////////////////////////////////////////////////////////////////
//功能描述	手动恢复当前告警，对应于RMV IGWALMFLT命令
//输入参数	MSG_INTRA *pFrame
//输出参数	无
//返回值	0 表示成功  负数表示失败
/////////////////////////////////////////////////////////////////////////////////
int CMMLAlarmHandler::HandleCmdIGWAlarmDelete(MSG_INTRA *pFrame)
{
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);
    m_nReportCsn ++;
    
    int  iResFlag      = 0;  //是否有结果
    char szTmpOptr[78] = "";
    
    int nStartCsn;
    
    //取MML命令第一个参数：起始告警流水号（必须参数）
    pParaResult->GetDigitVal(&nStartCsn, 1, 1);	
    
    UINT4 nRecNum = m_pHistoryAlarm->GetRecordNum();
    
    SActiveAlarm *pActive = NULL;
    
    //不但要处理告警队列，还要处理历史告警库.
    SHistoryAlarm *pHistory;
    pHistory = new SHistoryAlarm[nRecNum];
    m_pHistoryAlarm->Seek(0);
    m_pHistoryAlarm->GetRecord(pHistory, nRecNum);
    
    //手工恢复从nStartCsn开始的告警
    for (UINT4 i = 0; i < nRecNum; i++)
    {   //张玉新 2002-04-17 根据最新规范修改,问题单12740 Csn >= nStartCsn ==> ==
        if ((pHistory[i].uCsn == nStartCsn)  
            && (pHistory[i].yAlarmType == AT_FAULT)
            && (pHistory[i].yResType   == NOT_RECOVER))
        {
            
            RECOVERY_FAULTALARM(&pHistory[i], m_pHistoryAlarm, MANNUAL_RECOVER);
            SInnerAlarm sInnerAlarm;
            sInnerAlarm.uAlarmID    = pHistory[i].uAlarmID;  
            sInnerAlarm.yAlarmType  = AT_RESTORE;
            memcpy(sInnerAlarm.yAlarmPara, pHistory[i].yPara, MAX_ALARM_PARA);
            
            //在当前活动队列中添加告警
            //内部会分配内存来拷贝,所以可以使用局部变量
            m_ActiveAlarmList.AddAlarm(&sInnerAlarm, &pActive);
            
            //by ldf 2002.1.7 
            if (pActive != NULL)
            {
                pActive->uCsn = pHistory[i].uCsn;
                //张玉新 告警箱 添加
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
    
    //如果有记录被恢复生成MML报文
    delete[] pHistory;
    if (iResFlag == 1)
    {
        m_pAlarmMMLReport->MakeReportHead();
        m_pAlarmMMLReport->MakeCmdDisplay(pParaResult->GetCmdLine(),m_nReportCsn);
        m_pAlarmMMLReport->AddString(STR_NEWLINE);
        m_pAlarmMMLReport->MakeRecoverAlarmBody(nStartCsn, nStartCsn, m_uBillsvrNo);
        m_pAlarmMMLReport->MakeReportTail();		
        //现在从维护口发送一条告警
        SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_NORM,pFrame->IPAddr, pFrame->ClientNo);
        
        //现在从上报口发送一条告警
        m_pAlarmMMLReport->MakeReportHead();
        m_pAlarmMMLReport->MakeRecoverAlarmBody(nStartCsn, nStartCsn, m_uBillsvrNo);
        m_pAlarmMMLReport->MakeReportTail();
        SendMessageToMML(m_pAlarmMMLReport->GetReport(), APP_MML_WARN, pFrame->IPAddr, pFrame->ClientNo);
        
    }	
    else //如果没有记录被恢复，则使用默认处理
    {
        HandleCmdDefault(pFrame);
    }    
    
    return ERR_SUCCESS;
}
// by	杨善	2004-5-19   对应问题单SWPD05267
/**********************************************************************
功能描述	往活动告警队列中加入一条新的告警
输入参数	pNewAlarm——待加入的新告警
输出参数	ppActiveAlarm——在活动告警队列中新增加的活动告警
返回值		增加新告警的结果
抛出异常	无
*********************************************************************/
ALARM_ADD_RESULT CMMLAlarmHandler::AddAlarm(SInnerAlarm* pNewAlarm, SActiveAlarm** ppActiveAlarm, int nRecoverType)
{
	assert(pNewAlarm->uAlarmID < 128);
	
	ALARM_ADD_RESULT nResult = ADD_OK;

	//如果要发送的告警被屏蔽，则不发送
    m_Mutex.acquire();
    char shield = m_szAlarmShield[pNewAlarm->uAlarmID];
    m_Mutex.release();    
    if ('0' != shield) 
        
	{
		nResult = CAlarmHandler::AddAlarm(pNewAlarm,ppActiveAlarm,nRecoverType);
	}

	return nResult;
}

// the end	2004-5-19	对应问题单SWPD05267
