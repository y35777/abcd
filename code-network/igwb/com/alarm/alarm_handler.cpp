// AlarmHandler.cpp: implementation of the CAlarmHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "../include/toolbox.h" //clear in v2
#include "alarm_handler.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//功能描述  构造CAlarmHandler对象
//输入参数  BillsvrNo——主备信息
/////////////////////////////////////////////////////////////////////
CAlarmHandler::CAlarmHandler(int BillsvrNo, CAlarmServer* pAlarmServer)
               :m_pAlarmCfg(NULL),m_uBillsvrNo(BillsvrNo)
{
    m_pAlarmServer = pAlarmServer;    
    m_uCsn = 99;
#ifndef _ONLY_TEST_
    CINIFile iniFile((char *)GetCfgFilePath());
#else
    CINIFile iniFile("c:/igwb.ini");
#endif
    int nRet = iniFile.Open();
    if(nRet != TRUE)
    {
        TRACE(MTS_ALARM, S_ALARM_INI_OPEN_FAIL);
        m_bTraceDebug = 0;
    }
    else
    {
        m_bTraceDebug = iniFile.GetInt(CFG_SEC_ALARM,
                               CFG_ALARM_TRACE_DEBUG,
                               CFG_ALARM_TRACE_DEBUG_DEFAULT);
    }
}

///////////////////////////////////////////////////////////////////
//功能描述  析构函数
///////////////////////////////////////////////////////////////////
CAlarmHandler::~CAlarmHandler()
{
    //删除m_pAlarmConfig对象指针
    if(m_pAlarmCfg != NULL)
    {
        delete m_pAlarmCfg;
    }
}

/////////////////////////////////////////////////////////////////
//功能描述  在该函数中首先读出告警详细参数信息配置文件存放位置,
//          创建m_pAlarmCfg指针,将告警详细信息配置文件路径传给
//          m_pAlarmCfg,并通过m_pAlarmCfg完成告警信息参数的读入工作
//输入参数  无
//输出参数  无
//返回值    返回0表示初始化成功,负值表示初始化失败.
//////////////////////////////////////////////////////////////////
int CAlarmHandler::Init(const char *szCfgPath, int nConnectedMode, STRING szPortName)
{
    //利用告警模板初始化内部告警队列
    SActiveAlarm AlarmTemplate;
    //张玉新2002-02-12 新增代码-修改TRACE中告警流水号乱码的问题
    memset(&AlarmTemplate,0,sizeof(SActiveAlarm));
    //结束
    AlarmTemplate.ySendTimes = 5;
    AlarmTemplate.yReceived = AR_NOT_RECEIVED;
    AlarmTemplate.yFuncID = AI_BILLING_SYSTEM;
    AlarmTemplate.yAlarmLevel = AL_EMERGENCY;
    AlarmTemplate.ySubSysID = AS_SOFTWARE_SYSTEM;
    AlarmTemplate.yModuleNO = m_uBillsvrNo;
    AlarmTemplate.yCPUPos = m_uBillsvrNo;
    AlarmTemplate.yPID = PT_OM;
    //保存告警配置库的路径名
    m_sAlarmCfgPath = szCfgPath;

    //根据路径名构造CAlarmConfig对象;
    m_pAlarmCfg = new CAlarmConfig(m_sAlarmCfgPath.c_str());

    //装载告警配置信息,如果转载失败则返回失败码,否则返回0;
    if(m_pAlarmCfg->LoadConfig() != ERR_SUCCESS)
    {
        TRACE(MTS_ALARM,S_ALARM_LOAD_CFG_FAIL);
        return  ERR_FAIL;
    }

    m_ActiveAlarmList.InitList(AlarmTemplate,m_pAlarmCfg);
    return ERR_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
//功能描述  空函数.处理1秒定时任务,在CAlarmHandler的子类中根据需要重载
//////////////////////////////////////////////////////////////////////
void CAlarmHandler::Timer1Sec()
{
    static int nTimerCount = 0;
    nTimerCount ++;
    if(nTimerCount >= 60)
    {
#ifndef _ONLY_TEST_
        CINIFile iniFile((char *)GetCfgFilePath());
#else
        CINIFile iniFile("c:/igwb.ini");
#endif
        int nRet = iniFile.Open();
        if(nRet != TRUE)
        {
            TRACE(MTS_ALARM, S_ALARM_INI_OPEN_FAIL);
            throw "INI File Open Fail";
        }

        m_bTraceDebug = iniFile.GetInt(CFG_SEC_ALARM,
                                       CFG_ALARM_TRACE_DEBUG,
                                       CFG_ALARM_TRACE_DEBUG_DEFAULT);
        nTimerCount = 0;
    }
}

////////////////////////////////////////////////////////////////////////
//功能描述  在该函数中根据pFrame的m_ySenderMid判断，如果来自APP_MML_WARN
//          表明消息来自MML告警客户端,则将消息交给
//          HandleFrameFromMML(MSG_INTRA *pFrame)函数处理,
//          否则交给HandleFrameFromApp(MSG_INTRA  *pFrame)函数处理,
//          从而完成消息分发
//输入参数  MSG_INTRA *pFrame - 内部消息帧
////////////////////////////////////////////////////////////////////////
void CAlarmHandler::DispatchFrame(MSG_INTRA *pFrame)
{
    //根据pFrame->SenderMid判断消息来源,如果等于 MT_MMLSERVER,调用HandleFrameFromMML接口
    if(pFrame == NULL)
    {
        return;
    }

    if((pFrame->CmdCode == CMD_MML) && (pFrame->SenderMid == MT_MMLSERVER))
    {
        HandleFrameFromMML(pFrame);
    }
    //否则调用HandleFrameFromApp函数接口;
    else
    {
        HandleFrameFromApp(pFrame);
    }

}

////////////////////////////////////////////////////////////////////////////////
//功能描述  将一条告警纪录加入当前活动告警队列
//输入参数  pNewAlarm - 内部各模块间的告警帧结构
//输出参数  pActiveAlarm - 返回的相应的SActiveAlarm结构，用于对历史告警库的操作
//返回值    ALARM_ADD_RESULT
/////////////////////////////////////////////////////////////////////////////////
ALARM_ADD_RESULT CAlarmHandler::AddAlarm(SInnerAlarm *pNewAlarm,
                                         SActiveAlarm **ppActiveAlarm,int  nRecoverType)
{
    //添加告警信息到活动告警队列中,若返回值不为ADD_OK,返回错误原因,否则继续执行

    int nRet = m_ActiveAlarmList.AddAlarm(pNewAlarm,ppActiveAlarm,nRecoverType);
    if(nRet == ADD_OK)
    {

        //如果是事件告警或者故障告警并且成功加入告警队列,则调用CreateCsn(void)函数,
        //为该告警创建一个告警流水号
        if(AT_EVENT == pNewAlarm->yAlarmType  || AT_FAULT == pNewAlarm->yAlarmType)
        {
            (*ppActiveAlarm)->uCsn = CreateCsn();
            //by ldf 2004-01-31 OM整改
            //不论是什么告警，都要把同步号增1
            (*ppActiveAlarm)->uSyn = CreateSyn();
        }
		else
		{
            if(pNewAlarm->yAlarmType == AT_RESTORE)
            {
            #ifdef _OM_MODIFY
                //by ldf OM整改:不论是什么告警，都要把同步号增1
                (*ppActiveAlarm)->uSyn = CreateSyn();
            #endif

            }
            else
            {
                (*ppActiveAlarm) =  NULL;
            }
        }
    }

    return (ALARM_ADD_RESULT)nRet;
}

//////////////////////////////////////////////////////////////////////////////////
//功能描述  创建告警流水号
//      在二进制告警中,计费网关中为告警分配流水号没有意义,因为二进制告警是计费
//      网关发送给主机节点,然后由BAM统一分配告警流水号.在MML集中告警中必须为告
//      警分配告警流水号.在CAlarmHandler类中,该函数直接返回0,在CBinAlarmHandler
//      类中无需重载此函数,但在CMMLAlarmHandler类中必须重载此函数
//输入参数  无
//输出参数  无
//返回值    UINT4 - 新创建的告警流水号
/////////////////////////////////////////////////////////////////////////////////
UINT4 CAlarmHandler::CreateCsn()
{
    return 0;
}

//by ldf 2004-01-31 OM整改
UINT4 CAlarmHandler::CreateSyn()
{
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////
//功能描述  回退流水号,为空函数.在CAlarmHandler类中,该函数直接返回0,
//      在CBinAlarmHandler类中无需重载此函数,但在CMMLAlarmHandler类中必须重载
//      此函数
//输入参数  无
//输出参数  无
//返回值    UINT4 回退后的告警流水号
//////////////////////////////////////////////////////////////////////////////////
UINT4 CAlarmHandler::RollBackCsn()
{
    return 0;
}

//by ldf 2004-01-31 OM整改
UINT4 CAlarmHandler::RollBackSyn()
{
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////
//功能描述  空函数.子类必须重载这个函数,在MML告警处理类中这个函数直接返回,在二进制
//      告警处理类中,需要处理主机发过来的响应帧.
//输入参数  MSG_INTRA *pFrame
//////////////////////////////////////////////////////////////////////////////////
void CAlarmHandler::AlarmReceived(MSG_INTRA *pFrame)
{
}

//////////////////////////////////////////////////////////////////////////////////
//功能描述  空函数.该函数处理来自iGateway bill内部模块发送的告警消息
//      在CAlarmHandler类中不作任何处理,CAlarmHandler的子类如需要特殊处理
//      应重载virtual void HandleFrameFromApp(MSG_INTRA *pFrame)函数
//输入参数  MSG_INTRA *pFrame
//////////////////////////////////////////////////////////////////////////////////
void CAlarmHandler::HandleFrameFromApp(MSG_INTRA *pFrame)
{
}

//////////////////////////////////////////////////////////////////////////////////
//功能描述  空函数.处理来自MML告警客户端的消息,在CAlarmHandler类中不作任何处理
//      函数直接返回,CAlarmHandler的子类中如果需要特殊处理,应重载该函数.
//输入参数  MSG_INTRA *pFrame
//////////////////////////////////////////////////////////////////////////////////
void CAlarmHandler::HandleFrameFromMML(MSG_INTRA *pFrame)
{
}

//获取告警名称列表
void CAlarmHandler::HandleCmdLstAlm(MSG_INTRA *pFrame)
{
    LIST<STRING>  title, content;
    LIST<SAlarmExplain> AlarmExplainList;
    m_pAlarmCfg->GetAlarmExplainList(AlarmExplainList);

    LIST<SAlarmExplain>::iterator it;
    char szTmp[20];
    for(it = AlarmExplainList.begin(); it != AlarmExplainList.end(); it++)
    {        
        if((*it).uProductType == ALARM_PRODUCT_CG)
        {
            //增加ccs的告警            
            sprintf(szTmp, "%d", (*it).uAlarmID);
            content.push_back(szTmp);
            content.push_back((*it).szAlarmName);
            m_Mutex.acquire();
            sprintf(szTmp, "%c", m_szAlarmShield[(*it).uAlarmID - 1]);
            m_Mutex.release();
            content.push_back(szTmp);   
        }
        else if((*it).uProductType == ALARM_PRODUCT_CCS)
        {
#ifdef _PRODUCT_CCS
            //增加ccs的告警
            sprintf(szTmp, "%d", (*it).uAlarmID);
            content.push_back(szTmp);
            content.push_back((*it).szAlarmName);
            m_Mutex.acquire();
            sprintf(szTmp, "%c", m_szAlarmShield[(*it).uAlarmID - 1]);
            m_Mutex.release();
            content.push_back(szTmp);
#endif  
        }          
    }

    title.push_back(S_ALARM_ID);
    title.push_back(S_ALARM_NAME);
    title.push_back(S_ALARM_SHIELD);

    CMMLReport Report(MTS_ALARM);
    Report.MakeReportHead();
    
    CParseResult *pParse = *((CParseResult **)pFrame->Data);
    Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, 0, MTS_ALARM);
    Report.AddHoriTable(title, content);
    Report.AddString(STR_NEWLINE);
    Report.MakeReportTail();

    const char* pChar = Report.GetReport();
    
    MSG_INTRA* pAns = new(strlen(pChar) + 1) MSG_INTRA; 
    strcpy((char *)(pAns->Data), pChar);
    
    pAns->SenderPid   = PT_OM;
    pAns->SenderMid   = MT_ALARM;
    pAns->ReceiverPid = PT_OUTSIDE;
    pAns->ReceiverMid = MT_MMLDEBUG;
    pAns->AppType     = pFrame->AppType;
    pAns->ClientNo    = pFrame->ClientNo;
    pAns->IPAddr      = pFrame->IPAddr;
    
    CWorkModule::SendMsg(pAns); 
}

//设置告警屏蔽码
void CAlarmHandler::HandleCmdSetShield(MSG_INTRA *pFrame)
{
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);
    
    m_Mutex.acquire();
    memcpy(m_szAlarmShield, pParaResult->GetStringVal(1), strlen(pParaResult->GetStringVal(1)));
    m_Mutex.release();
    
    CMMLReport Report(MTS_ALARM);
    Report.MakeReportHead();
    
    Report.MakeCmdDisplay(pParaResult->GetCmdLine(), 0, 0, MTS_ALARM);
    
    Report.AddString(S_MML_SUCCEED);
    Report.AddString(STR_NEWLINE);
    Report.MakeReportTail();
    
    const char* pChar = Report.GetReport();
    
    MSG_INTRA* pAns = new(strlen(pChar) + 1) MSG_INTRA; 
    strcpy((char *)(pAns->Data), pChar);
    
    pAns->SenderPid   = PT_OM;
    pAns->SenderMid   = MT_ALARM;
    pAns->ReceiverPid = PT_OUTSIDE;
    pAns->ReceiverMid = pFrame->SenderMid;
    pAns->AppType     = pFrame->AppType;
    pAns->ClientNo    = pFrame->ClientNo;
    pAns->IPAddr      = pFrame->IPAddr;
    pAns->CmdCode     = CMD_MML;
    
    CWorkModule::SendMsg(pAns);    
}

void CAlarmHandler::SetCfgPath(const char *pcCfgPath = S_ALARM_CONFIG_PATH)
{
    m_sAlarmCfgPath = pcCfgPath;
}

void CAlarmHandler::TRACEDebug(const char *szModule, const char *format_str,...)
{
    if(m_bTraceDebug)
    {
        char pbuf[512];
        va_list argptr;
        va_start(argptr, format_str);
        VSNPRINTF(pbuf,512,format_str,argptr);
        pbuf[511] = 0;
        va_end(argptr);
        TRACE(szModule,pbuf);
    }
    return;
}

CActiveAlarmList* CAlarmHandler::GetActiveAlarmList()
{
    return  &m_ActiveAlarmList;
}
