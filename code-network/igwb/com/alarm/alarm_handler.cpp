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
//��������  ����CAlarmHandler����
//�������  BillsvrNo����������Ϣ
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
//��������  ��������
///////////////////////////////////////////////////////////////////
CAlarmHandler::~CAlarmHandler()
{
    //ɾ��m_pAlarmConfig����ָ��
    if(m_pAlarmCfg != NULL)
    {
        delete m_pAlarmCfg;
    }
}

/////////////////////////////////////////////////////////////////
//��������  �ڸú��������ȶ����澯��ϸ������Ϣ�����ļ����λ��,
//          ����m_pAlarmCfgָ��,���澯��ϸ��Ϣ�����ļ�·������
//          m_pAlarmCfg,��ͨ��m_pAlarmCfg��ɸ澯��Ϣ�����Ķ��빤��
//�������  ��
//�������  ��
//����ֵ    ����0��ʾ��ʼ���ɹ�,��ֵ��ʾ��ʼ��ʧ��.
//////////////////////////////////////////////////////////////////
int CAlarmHandler::Init(const char *szCfgPath, int nConnectedMode, STRING szPortName)
{
    //���ø澯ģ���ʼ���ڲ��澯����
    SActiveAlarm AlarmTemplate;
    //������2002-02-12 ��������-�޸�TRACE�и澯��ˮ�����������
    memset(&AlarmTemplate,0,sizeof(SActiveAlarm));
    //����
    AlarmTemplate.ySendTimes = 5;
    AlarmTemplate.yReceived = AR_NOT_RECEIVED;
    AlarmTemplate.yFuncID = AI_BILLING_SYSTEM;
    AlarmTemplate.yAlarmLevel = AL_EMERGENCY;
    AlarmTemplate.ySubSysID = AS_SOFTWARE_SYSTEM;
    AlarmTemplate.yModuleNO = m_uBillsvrNo;
    AlarmTemplate.yCPUPos = m_uBillsvrNo;
    AlarmTemplate.yPID = PT_OM;
    //����澯���ÿ��·����
    m_sAlarmCfgPath = szCfgPath;

    //����·��������CAlarmConfig����;
    m_pAlarmCfg = new CAlarmConfig(m_sAlarmCfgPath.c_str());

    //װ�ظ澯������Ϣ,���ת��ʧ���򷵻�ʧ����,���򷵻�0;
    if(m_pAlarmCfg->LoadConfig() != ERR_SUCCESS)
    {
        TRACE(MTS_ALARM,S_ALARM_LOAD_CFG_FAIL);
        return  ERR_FAIL;
    }

    m_ActiveAlarmList.InitList(AlarmTemplate,m_pAlarmCfg);
    return ERR_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
//��������  �պ���.����1�붨ʱ����,��CAlarmHandler�������и�����Ҫ����
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
//��������  �ڸú����и���pFrame��m_ySenderMid�жϣ��������APP_MML_WARN
//          ������Ϣ����MML�澯�ͻ���,����Ϣ����
//          HandleFrameFromMML(MSG_INTRA *pFrame)��������,
//          ���򽻸�HandleFrameFromApp(MSG_INTRA  *pFrame)��������,
//          �Ӷ������Ϣ�ַ�
//�������  MSG_INTRA *pFrame - �ڲ���Ϣ֡
////////////////////////////////////////////////////////////////////////
void CAlarmHandler::DispatchFrame(MSG_INTRA *pFrame)
{
    //����pFrame->SenderMid�ж���Ϣ��Դ,������� MT_MMLSERVER,����HandleFrameFromMML�ӿ�
    if(pFrame == NULL)
    {
        return;
    }

    if((pFrame->CmdCode == CMD_MML) && (pFrame->SenderMid == MT_MMLSERVER))
    {
        HandleFrameFromMML(pFrame);
    }
    //�������HandleFrameFromApp�����ӿ�;
    else
    {
        HandleFrameFromApp(pFrame);
    }

}

////////////////////////////////////////////////////////////////////////////////
//��������  ��һ���澯��¼���뵱ǰ��澯����
//�������  pNewAlarm - �ڲ���ģ���ĸ澯֡�ṹ
//�������  pActiveAlarm - ���ص���Ӧ��SActiveAlarm�ṹ�����ڶ���ʷ�澯��Ĳ���
//����ֵ    ALARM_ADD_RESULT
/////////////////////////////////////////////////////////////////////////////////
ALARM_ADD_RESULT CAlarmHandler::AddAlarm(SInnerAlarm *pNewAlarm,
                                         SActiveAlarm **ppActiveAlarm,int  nRecoverType)
{
    //��Ӹ澯��Ϣ����澯������,������ֵ��ΪADD_OK,���ش���ԭ��,�������ִ��

    int nRet = m_ActiveAlarmList.AddAlarm(pNewAlarm,ppActiveAlarm,nRecoverType);
    if(nRet == ADD_OK)
    {

        //������¼��澯���߹��ϸ澯���ҳɹ�����澯����,�����CreateCsn(void)����,
        //Ϊ�ø澯����һ���澯��ˮ��
        if(AT_EVENT == pNewAlarm->yAlarmType  || AT_FAULT == pNewAlarm->yAlarmType)
        {
            (*ppActiveAlarm)->uCsn = CreateCsn();
            //by ldf 2004-01-31 OM����
            //������ʲô�澯����Ҫ��ͬ������1
            (*ppActiveAlarm)->uSyn = CreateSyn();
        }
		else
		{
            if(pNewAlarm->yAlarmType == AT_RESTORE)
            {
            #ifdef _OM_MODIFY
                //by ldf OM����:������ʲô�澯����Ҫ��ͬ������1
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
//��������  �����澯��ˮ��
//      �ڶ����Ƹ澯��,�Ʒ�������Ϊ�澯������ˮ��û������,��Ϊ�����Ƹ澯�ǼƷ�
//      ���ط��͸������ڵ�,Ȼ����BAMͳһ����澯��ˮ��.��MML���и澯�б���Ϊ��
//      ������澯��ˮ��.��CAlarmHandler����,�ú���ֱ�ӷ���0,��CBinAlarmHandler
//      �����������ش˺���,����CMMLAlarmHandler���б������ش˺���
//�������  ��
//�������  ��
//����ֵ    UINT4 - �´����ĸ澯��ˮ��
/////////////////////////////////////////////////////////////////////////////////
UINT4 CAlarmHandler::CreateCsn()
{
    return 0;
}

//by ldf 2004-01-31 OM����
UINT4 CAlarmHandler::CreateSyn()
{
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////
//��������  ������ˮ��,Ϊ�պ���.��CAlarmHandler����,�ú���ֱ�ӷ���0,
//      ��CBinAlarmHandler�����������ش˺���,����CMMLAlarmHandler���б�������
//      �˺���
//�������  ��
//�������  ��
//����ֵ    UINT4 ���˺�ĸ澯��ˮ��
//////////////////////////////////////////////////////////////////////////////////
UINT4 CAlarmHandler::RollBackCsn()
{
    return 0;
}

//by ldf 2004-01-31 OM����
UINT4 CAlarmHandler::RollBackSyn()
{
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////
//��������  �պ���.������������������,��MML�澯���������������ֱ�ӷ���,�ڶ�����
//      �澯��������,��Ҫ������������������Ӧ֡.
//�������  MSG_INTRA *pFrame
//////////////////////////////////////////////////////////////////////////////////
void CAlarmHandler::AlarmReceived(MSG_INTRA *pFrame)
{
}

//////////////////////////////////////////////////////////////////////////////////
//��������  �պ���.�ú�����������iGateway bill�ڲ�ģ�鷢�͵ĸ澯��Ϣ
//      ��CAlarmHandler���в����κδ���,CAlarmHandler����������Ҫ���⴦��
//      Ӧ����virtual void HandleFrameFromApp(MSG_INTRA *pFrame)����
//�������  MSG_INTRA *pFrame
//////////////////////////////////////////////////////////////////////////////////
void CAlarmHandler::HandleFrameFromApp(MSG_INTRA *pFrame)
{
}

//////////////////////////////////////////////////////////////////////////////////
//��������  �պ���.��������MML�澯�ͻ��˵���Ϣ,��CAlarmHandler���в����κδ���
//      ����ֱ�ӷ���,CAlarmHandler�������������Ҫ���⴦��,Ӧ���ظú���.
//�������  MSG_INTRA *pFrame
//////////////////////////////////////////////////////////////////////////////////
void CAlarmHandler::HandleFrameFromMML(MSG_INTRA *pFrame)
{
}

//��ȡ�澯�����б�
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
            //����ccs�ĸ澯            
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
            //����ccs�ĸ澯
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

//���ø澯������
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
