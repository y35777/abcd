// alarm_box.cpp: implementation of the CAlarmBox class.
//////////////////////////////////////////////////////////////////////
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/
#include "alarm_box.h"
#include "../utility/mml_para.h"
/*********************************************************************
��Ԫ����
void TRACE(...)
{
    printf("TRACE\n");
}
#define S_ALARM_EMERGENCY_CNT   "�����澯"
#define S_ALARM_IMPORTANT_CNT   "��Ҫ�澯"
#define S_ALARM_MINOR_CNT       "��Ҫ�澯"
#define S_ALARM_NORMAL_CNT      "����澯"
***********************************************************************/
/**********************************************************************
��������    ���캯��
�������    pReport----����������ָ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
CAlarmBox::CAlarmBox(CAlarmConfig *pCfg,CMMLReport *pReport)
{
    ASSERT(pReport != NULL && pCfg != NULL);
    m_pState = new CInitState(this);
    m_pReport = pReport;
    m_pCfg = pCfg;
    m_nCnt = 0;
    m_CommBox = NULL;           
}
/**********************************************************************
��������    ��������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
CAlarmBox::~CAlarmBox()
{
    if(m_pState != NULL)
    {
        delete m_pState;
        m_pState = NULL;
    }
    m_pCfg = NULL;
    m_pReport = NULL;
    //����m_CommBox����
    if(m_CommBox != NULL)
    {
        delete m_CommBox;
        m_CommBox = NULL;
    }     
}
/**********************************************************************
��������    ��ʼ����������������ʼ���ڲ�״̬
�������    ��
�������    ��
����ֵ      ��ʼ���Ƿ�ɹ�
�׳��쳣    ��
**********************************************************************/
BOOL CAlarmBox::Init(int nConnectedMode, STRING szPortName)
{
    if(m_pState == NULL)
    {
        return FALSE;
    }
    
    m_nConnectedMode = nConnectedMode;    
    m_nCnt = 0;
    m_AlarmCount[AL_EMERGENCY] = 0;
    m_AlarmCount[AL_IMPORTANT] = 0;
    m_AlarmCount[AL_MINOR] = 0;
    m_AlarmCount[AL_WARNING] = 0;
    memset(m_szAlarmBoxVersion,0,64);
    memset(&m_sRecentAlarm,0,sizeof(SAlarmInfo));
    
    if(nConnectedMode == CONNECTED_BY_CG)
    {
        m_CommBox = new CCommBox();
        if(!(m_CommBox->Init(szPortName))) 
        {
            delete m_CommBox;
            m_CommBox = NULL;
            TRACE(MTS_ALARM, S_ALARM_BOX_INIT_FAIL);
        }  
    }    
    
    //������ ���Ӹ澯���ʼ��
    return  m_pState->Init();       
}
BYTE CAlarmBox::IsSendToAlarmBox(UINT2 uAlarmID)
{
    SAlarmExplain *pExplain = m_pCfg->GetAlarmExplain(uAlarmID);
    if(pExplain != NULL)
        return pExplain->ySendToBox;
    else
        return 0;
}
/**********************************************************************
��������    �ı�״̬����
�������    pS----�µ�״̬
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
BOOL CAlarmBox::ChangeState(CState *pS)
{
    delete m_pState;
    m_pState = pS;
    BOOL nRes = pS->Init();
    if(nRes == FALSE)
    {
        delete pS,pS = NULL;
        m_pState = NULL;
        TRACE(MTS_ALARM,S_ALARM_CHANGE_STATE_ERROR);
    }
    return nRes;
}
/**********************************************************************
��������    ����ͻ��������
�������    pFrame----���пͻ����������Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CAlarmBox::HandleMMLCmdFromClient(MSG_INTRA *pFrame)
{
    if(m_pState != NULL)
    {
        m_pState->HandleMMLCmdFromClient(pFrame);
    }
}
/**********************************************************************
��������    ����澯�������
�������    pFrame----���и澯���������Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CAlarmBox::HandleAlarmCmdFromBox(MSG_INTRA *pFrame)
{
    if(m_pState != NULL)
    {
        m_pState->HandleAlarmCmdFromBox(pFrame->Data, pFrame->Len);           
    }
}
/**********************************************************************
��������    �����ڲ��澯����
�������    pFrame----�����ڲ��澯����Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CAlarmBox::HandleAlarm(SActiveAlarm *pFrame)
{
    if(m_pState != NULL)
    {
        m_pState->HandleAlarm(pFrame);
    }
}
/**********************************************************************
��������    ��ʱ������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CAlarmBox::OnTimer()
{
    static int times =  0;    
    
    if(m_pState != NULL && m_nConnectedMode == CONNECTED_BY_CG)
    { 
        //��ʱ��������֡
        m_pState->OnTimer();
        if(times == 0)
        {
            m_pState->SendCommandToBox(M_ALM_ALARM_BOX_RESPONSE_ONLINE);
            times = 1;
        }
        else
        {
            times = 0;
        }
        
        //����ֱ���ĸ澯�䣬��ʱ��ȡ�澯���ϱ���֡        
        BYTE* pSerialData = NULL;
        int nLen = 0;
        if(m_CommBox != NULL)
        {
            m_CommBox->Read(&pSerialData, &nLen);
        }    
        //�������ݳɹ���������������������һ�����������7���ֽ� ��
        if(NULL != pSerialData && nLen >= 7)
        {
            if(m_pState != NULL)
            {
                //����HandleAlarmCmdFromBox����澯���ϱ�������
                m_pState->HandleAlarmCmdFromBox(pSerialData, nLen);  
                //�ͷŸ澯���ϱ�֡�Ŀռ�
                delete pSerialData;
                pSerialData = NULL; 
            }
        }    
    }
    
    if(m_pState != NULL && m_nConnectedMode == CONNECTED_BY_LMT)
    {
        m_pState->OnTimer();        
    }    
}
/**********************************************************************
��������    ����ͻ��������
�������    pB----ӵ�д�״̬�ĸ澯��
            pReport----������������ָ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
CState::CState(CAlarmBox *pB)
{
    //zhangyuxinASSERT(pB != NULL);
    m_pParent = pB;
}
/**********************************************************************
��������    ����ͻ��������
�������    pFrame----���пͻ����������Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CState::HandleMMLCmdFromClient(MSG_INTRA *pFrame)
{
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);
    //TRACE(MTS_ALARM,S_ALARM_RECEIVE_MMLCMD,pParaResult->GetCmdCode());
    switch(pParaResult->GetCmdCode())
    {
        case MML_SET_IGWALMACK:
            HandleMMLAlarmACK(pFrame);
            break;
        case MML_SET_IGWEVTACK:
            HandleMMLAlarmAckEVT(pFrame);
            break;
        default:
            {
                int nLocal = 0;
                pParaResult->GetDigitVal(&nLocal, 1, 1);
                if(nLocal != 0)
                {
                    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_LOCATION_ERROR);
                    return;
                }
                switch(pParaResult->GetCmdCode())
                {
                case MML_LST_BOXLGT:
                    HandleMMLSetBoxLGT(pFrame);
                    break;
                case MML_LST_BOXVER:
                    HandleMMLLSTBoxVER(pFrame);
                    break;
                case MML_CLR_BOXLGT:
                    HandleMMLCLRBoxLGT(pFrame);
                    break;
                case MML_STP_BOXSND:
                    HandleMMLSTPBoxSND(pFrame);
                    break;
                case MML_RST_ALMBOX:
                    HandleMMLRSTALMBox(pFrame);
                    break;
                }//switch 2
            }
    }//switch
}
/**********************************************************************
��������    �����ڲ��澯��Ϣ����
�������    pAlarm----�ڲ���澯����Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CState::UpdateRecentAlarm(SActiveAlarm *pAlarm)
{
    //TRACE(MTS_ALARM,S_ALARM_UPDATE_BOX_ALARM,pAlarm->uCsn,pAlarm->uAlarmID);
    memcpy(&m_pParent->m_sRecentAlarm,pAlarm,(sizeof(SAlarmInfo) - sizeof(BOOL)));
    (m_pParent->m_sRecentAlarm).bACK = FALSE;
}
/**********************************************************************
��������    �����ڲ��澯����
�������    pFrame----�����ڲ��澯����Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
BOOL CState::HandleAlarm(SActiveAlarm *pFrame)
{
    if(m_pParent->IsSendToAlarmBox(pFrame->uAlarmID))
    {
        //�澯���� == ���ϸ澯�����ݸ澯������Ӧ�ļ��������1��ˢ��m_sRecentAlarm             
        if(pFrame->yAlarmType == AT_FAULT)
        {
            m_pParent->m_AlarmCount[pFrame->yAlarmLevel] += 1;
            UpdateRecentAlarm(pFrame);
        }
        //�澯���� == �¼��澯 && �澯���� == ����������ˮ�������, ˢ��sRecentAlarm         
        if(pFrame->yAlarmType == AT_EVENT
           && pFrame->yAlarmLevel == AL_EMERGENCY)
        {
            (m_pParent->m_EvtCsnList).push_back(pFrame->uCsn);
            UpdateRecentAlarm(pFrame);
        }
        //�澯���� == �ָ��澯������Ӧ����ĸ澯������1��Ϊ��Ͳ��ټ��٣���
        //����m_sRecentAlarm����ˮ����ȣ�����¸澯����Ϊ�ָ�          
        if(pFrame->yAlarmType == AT_RESTORE)
        {
            if(m_pParent->m_AlarmCount[pFrame->yAlarmLevel] > 0)
            {
                m_pParent->m_AlarmCount[pFrame->yAlarmLevel] -= 1;
            }
            if((m_pParent->m_sRecentAlarm).uCsn == pFrame->uCsn)
            {
                (m_pParent->m_sRecentAlarm).yAlarmType = AT_RESTORE;
            }
        }
#ifdef _ONLY_TEST_
        TRACE(MTS_ALARM,"�澯���յ��澯(LEVEL=%d, ID=%d,Para=%d)",
             pFrame->yAlarmLevel,pFrame->uAlarmID,pFrame->yPara[0]);
        TRACE(MTS_ALARM,"��������=%d, ��Ҫ����=%d, ��Ҫ����=%d, �������=%d, �����¼�����=%d",
             m_pParent->m_AlarmCount[AL_EMERGENCY],
             m_pParent->m_AlarmCount[AL_IMPORTANT],
             m_pParent->m_AlarmCount[AL_MINOR],
             m_pParent->m_AlarmCount[AL_WARNING],
             m_pParent->m_EvtCsnList.size());
#endif
        return TRUE;
    }
    return FALSE;
}
/**********************************************************************
��������    ����MML������澯ȷ�ϡ�������
�������    pFrame----����MML������Ϣ����Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CState::HandleMMLAlarmACK(MSG_INTRA *pFrame)
{
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);
    //���MML��һ����������ʼ��ˮ�ţ����������
    int nStartCsn = 0;
    pParaResult->GetDigitVal(&nStartCsn, 1, 1);
    //���MML�ڶ�����������ֹ��ˮ�ţ����������
    int nEndCsn = 0;
    pParaResult->GetDigitVal(&nEndCsn, 2, 1);
    if(nStartCsn <= (m_pParent->m_sRecentAlarm).uCsn
       && (m_pParent->m_sRecentAlarm).uCsn <= nEndCsn)
    {
        (m_pParent->m_sRecentAlarm).bACK = TRUE;
    }
    //��������¼��澯��ˮ�Ŷ����к�����ȷ����ˮ�ŷ�Χ֮�ڵĸ澯�ҳ���Щ�澯�ڶ�����ɾ��      
    CsnList::iterator iter = (m_pParent->m_EvtCsnList).begin();
    while(iter != (m_pParent->m_EvtCsnList).end())
    {
        if((*iter) >= nStartCsn && (*iter) <= nEndCsn)
        {
            iter = (m_pParent->m_EvtCsnList).erase(iter);
            continue;
        }
        iter ++;
    }
}
/**********************************************************************
��������    ����MML������¼��桪����ȷ�Ϻ���
�������    pFrame----����MML������Ϣ����Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CState::HandleMMLAlarmAckEVT(MSG_INTRA *pFrame)
{
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);
    //���MML��������ˮ�ţ����������
    int nCsn = 0;
    pParaResult->GetDigitVal(&nCsn, 1, 1);
    if((m_pParent->m_sRecentAlarm).uCsn == nCsn)
    {
        (m_pParent->m_sRecentAlarm).bACK = TRUE;
    }
    //��m_pParent->m_EvtCsnList���ܹ��ҵ������ˮ�ž�ɾ����
    (m_pParent->m_EvtCsnList).remove(nCsn);
}
/**********************************************************************
��������    ��������澯�亯��
�������    nCmd----������  nAlarmLevel----�澯����  nValue----����ֵ
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CState::SendCommandToBox(int nCmd, int nAlarmLevel, int nValue)
{
    unsigned char ucCommand[M_ALM_MAX_ALARM_BOX_COMMAND_LEN];
    unsigned char ucLen = 0;
    unsigned char ucChkSum = 0;
    memset(ucCommand, 0, M_ALM_MAX_ALARM_BOX_COMMAND_LEN);
    // Э���ʽΪ FS(1byte) +len(1byte) + TA(1byte) +SA(1Byte) + len(1Byte)
    //+ Cmd/Rsp(1Byte) + Para1 + para2 + Para3
    // ���ɷ�������
    ucCommand[0] = M_ALM_BOX_PROTOCOL_FS;  //ͷ0x7e
    ucCommand[2] = M_ALM_BOX_PROTOCOL_SEND_TA; //�������澯��TA
    ucCommand[3] = M_ALM_BOX_PROTOCOL_SEND_SA; //�������澯��SA
    ucCommand[5]= (unsigned char)nCmd;
    ucLen = 5;
    switch (nCmd)
    {
    case M_ALM_ALARM_BOX_RESET: // ��λ�澯��
        //Ԥ�ڵ�messageΪ: 0x7e 0x5 0x1f 0x00 0x01 0x02 chksum
        ucCommand[4] = 0x01;  // ����Ϣ����
        break;
        // ͣ�澯��
    case M_ALM_ALARM_BOX_STOP_SOUND:
        //Ԥ�ڵ�messageΪ: 0x7e 0x5 0x1f 0x00 0x01 0x07 chksum
        ucCommand[4] = 0x01; // ����Ϣ����
        break;
        //��������
    case  M_ALM_ALARM_BOX_CONFIG:
        //Ԥ�ڵ�messageΪ: 0x7e 0x5 0x1f 0x00 0x01 0x08 chksum
        ucCommand[4] = 0x01; // ����Ϣ����
        break;
        // ����Ӧ��֡
    case M_ALM_ALARM_BOX_RESPONSE_ONLINE:
        // Ԥ�ڵ�messageΪ: 0x7e 0x5 0x1f 0x00 0x01 0x28 chksum
        ucCommand[4] = 0x01; // ����Ϣ����
        break;
        //�·��澯��Ϣ
    case M_ALM_ALARM_BOX_DOWN_ALARM:
        //Ԥ�ڵ�messageΪ: 0x7e 0x7 0x1f 0x00 0x03 0x50 para1 para2 chksum
        ucCommand[4] = 0x03; // ����Ϣ����
        ucCommand[6] = (unsigned char) nAlarmLevel;
        ucCommand[7] = (unsigned char) nValue;
        ucLen = 7;
        break;
        // �ط��澯��Ϣ
    case M_ALM_ALARM_BOX_REDOWN_ALARM:
        //Ԥ�ڵ�messageΪ: 0x7e 0x7 0x1f 0x00 0x03 0x51 para1 para2 chksum */
        ucCommand[4] =0x03; // ����Ϣ����
        ucCommand[6] = (unsigned char) nAlarmLevel;
        ucCommand[7] = (unsigned char) nValue;  // �и澯���������˸
        ucLen = 7;
        break;
        // ��ѯ����汾��Ϣ
    case M_ALM_ALARM_BOX_QUERY_VERSION:
        // Ԥ�ڵ�messageΪ: 0x7e 0x5 0x1f 0x00 0x01 0x52 chksum
        ucCommand[4] = 0x01; // ����Ϣ����
        ucLen = 5;
        break;
    default:
        break;
    }
    // ֡����len = message lens + chksum
    ucCommand[1] = (unsigned char) ucCommand[4] + 3 + 1;
    // ����chksum
    ucLen = (unsigned char)ucCommand[1] + 1;
    ucChkSum = ucCommand[0];//zyx 0
    for (int i = 1; i < ucLen; i++)//zyx i=0
    {
        ucChkSum ^= ucCommand[i];
    }
    ucCommand[ucLen] = (unsigned char)ucChkSum;
    ucCommand[ucLen + 1] = 0x00 ;  //�¸澯�����0x00����
    
    //����ֱ���澯���ģʽ��������ֱ�ӷ����澯��
    //����LMT���Ӹ澯���ģʽ(m_pParent->m_CommBox����Ϊ��)��������ͨ��sock�˿�ת��������̨�ĸ澯�� 
    if(m_pParent->m_CommBox != NULL)
    {
        m_pParent->m_CommBox->Write(ucCommand, ucLen + 2);
    }
    else
    {
        MSG_INTRA *pMsgToBox = new(ucLen+2) MSG_INTRA;
        memcpy((unsigned char *)pMsgToBox->Data,ucCommand,ucLen+2);
        pMsgToBox->CmdCode = 0xAA; //ALARM_CMD_BAU_TO_CONVERT
        pMsgToBox->ReceiverPid = PT_OUTSIDE;
        pMsgToBox->ReceiverMid = MT_UNKNOWN;
        pMsgToBox->SenderPid = PT_SELF;
        pMsgToBox->SenderMid = MT_ALARM;
        pMsgToBox->AppType = APP_ALARMBOX;
        int nRet = CWorkModule::SendMsg(pMsgToBox);
    }     
}
/**********************************************************************
��������    �����澯�����Ӧ����
�������    pFrame----������Ӧ������Ϣ����Ϣ֡
�������    nCmd---������ pPara---���������ָ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CState::AnalysizeCommandFromBox(unsigned char *pAlarmBoxFrame, int nFrameLen,
                                     int &nCmd, unsigned char *pPara)                                    
{
    nCmd  = 0xFF;
    unsigned char ucCheckSum = pAlarmBoxFrame[0];
    int nLen = pAlarmBoxFrame[1];
    if(nLen + 2 < nFrameLen )//����յ������ݳ��Ȳ���
    {
        return;
    }        
    // Э���ʽΪ FS + LEN + MESSAGE + CheckSum
    // checksum = FS ^ LEN ^ Message
    for (int i = 1; i < nLen + 1; i++)
    {
         ucCheckSum ^= pAlarmBoxFrame[i];
    }
    if((pAlarmBoxFrame[nLen + 1] != ucCheckSum)
        ||(pAlarmBoxFrame[2] != M_ALM_BOX_PROTOCOL_RESPONSE_TA)
        ||(pAlarmBoxFrame[3] != M_ALM_BOX_PROTOCOL_RESPONSE_SA))
    {
        //TRACE(MTS_ALARM,S_ALARM_CHECKSUM_ERR,pAlarmBoxFrame[5]);
        return;
    }
     nCmd = pAlarmBoxFrame[5];
     /*if(nCmd != 0x28)
     {
         TRACE(MTS_ALARM,S_ALARM_ANALYSIZE_BOX_RESPONSE,nCmd);
     }*/
     if(nCmd == M_ALM_ALARM_BOX_QUERY_VERSION && pPara != NULL)
     {
        BYTE bPos = pAlarmBoxFrame[4] + 4;
        pAlarmBoxFrame[bPos] = 0;
        strncpy((char *)pPara,(char *)(&pAlarmBoxFrame[6]),64);
        pPara[63] = 0;
     }
}
/**********************************************************************
��������    ����MML���ĵ��ͻ��˺���
�������    pFrame----����MML������Ϣ����Ϣ֡ pTitle----��Ϣ�ı���
            pContent---��Ϣ������
            ���pTitle!= NULL���ĸ�ʽΪ
            ������ ��ͷ
            �������
            Title = Content
            ---End
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CState::SendMMLReportToClient(MSG_INTRA *pFrame,const char *pTitle,const char *pContent)
{
    //������ͱ���
    CParseResult *parse = *((CParseResult **)pFrame->Data);
    if (m_pParent->m_pReport == NULL)
    {
        return;
    }
    m_pParent->m_pReport->MakeReportHead();
    m_pParent->m_pReport->MakeCmdDisplay(parse->GetCmdLine(),6);
    if(pTitle != NULL)
    {
        m_pParent->m_pReport->MakeReportItem(pTitle,pContent,strlen(pTitle),ALIGN_RIGHT,strlen(pContent));
    }
    else
    {
        m_pParent->m_pReport->AddString(pContent);
        m_pParent->m_pReport->AddString(STR_NEWLINE);
    }
    m_pParent->m_pReport->MakeReportTail();
    //���㱨�ĳ��ȣ������ȷ���ռ�
    int nLength = strlen(m_pParent->m_pReport->GetReport());
    MSG_INTRA *pMsgToClient = new(nLength + 1)MSG_INTRA;
    memcpy(pMsgToClient->Data, m_pParent->m_pReport->GetReport(), nLength);
    pMsgToClient->Data[nLength] = '\0';
    //��д֡ͷ����
    pMsgToClient->IPAddr      = pFrame->IPAddr;
    pMsgToClient->AppType     = APP_MML_NORM;
    pMsgToClient->SenderPid   = PT_OM;
    pMsgToClient->SenderMid   = MT_ALARM;
    pMsgToClient->ReceiverPid = PT_OUTSIDE;
    pMsgToClient->ReceiverMid = MT_MMLDEBUG;
    //���ÿ�ܵķ��ͺ�������Ϣ֡���͵�����ģ��ת��
    CWorkModule::SendMsg(pMsgToClient);
    TRACE(MTS_ALARM,S_ALARM_MML_REPORT,m_pParent->m_pReport->GetReport());
}
/**********************************************************************
��������    ��ʼ������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
BOOL CInitState::Init()
{
    m_pParent->m_nCnt = 0;
    SendCommandToBox(M_ALM_ALARM_BOX_RESET);
    return TRUE;
}
/**********************************************************************
��������    ��ʱ������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CInitState::OnTimer()
{
    CState::OnTimer();
    if(m_pParent->m_nCnt >= 30)
    {
        MSGOUT(MTS_ALARM,MSGOUT_LEVEL_IMPORTANT,S_ALARM_DISCONNECT);
        //������ �����澯������
        TRACE(MTS_ALARM,S_ALARM_DISCONNECT);
        m_pParent->ChangeState(new CDisconnectState(m_pParent));
        //���͸澯������澯
        SInnerAlarm Alarm;
        Alarm.yAlarmType = AT_EVENT;
        Alarm.uAlarmID = ALARM_ID_ALARMBOX_CONNECT_FAIL;
        //Alarm.yAlarmPara = 0xFF;
        CMsgProcessor::SendAlarm(&Alarm);       
    }
    /*else
    {
        //������ ��Ϊ���͵ĵ�һ������������ܻᶪʧ���������յ�
        //����ɹ�֮ǰ�����ط���
        if(m_bRecvResetSuc == FALSE)
        SendCommandToBox(M_ALM_ALARM_BOX_RESET);
    }*/
}   
/**********************************************************************
��������    ����5������Ϊ����MML�������ڸ澯�������
�������    pFrame----����MML������Ϣ����Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CInitState::HandleMMLSetBoxLGT(MSG_INTRA *pFrame)
{
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_INIT_NOT_QUERY_LGT);
    //��λ��Ϣ  =  �澯�䴦������״̬���޷���ѯ�澯��״̬
}
void CInitState::HandleMMLLSTBoxVER(MSG_INTRA *pFrame)
{
    //��λ��Ϣ  =  �澯�䴦������״̬���޷���ѯ�澯��汾
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_INIT_NOT_QUERY_VER);
}
void CInitState::HandleMMLCLRBoxLGT(MSG_INTRA *pFrame)
{
    //��λ��Ϣ  =  �澯�䴦������״̬������Ϩ��澯��
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_INIT_NOT_OFF_LGT);
}
void CInitState::HandleMMLSTPBoxSND(MSG_INTRA *pFrame)
{
    //��λ��Ϣ  =  �澯�䴦������״̬������ֹͣ�澯��
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_INIT_NOT_STOP_SND);
}
void CInitState::HandleMMLRSTALMBox(MSG_INTRA *pFrame)
{
    //��λ��Ϣ  =  �澯�䴦������״̬�����ش���
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_INIT_NOT_HANDLE);
}
/**********************************************************************
��������    ����澯��������
�������    pFrame----���и澯��������Ϣ����Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CInitState::HandleAlarmCmdFromBox(unsigned char* pAlarmBoxFrame, int nLen)
{
    int nCmd = 0;
    CState::HandleAlarmCmdFromBox(pAlarmBoxFrame, nLen);
    AnalysizeCommandFromBox(pAlarmBoxFrame, nLen, nCmd, NULL);    
    switch(nCmd)
    {
        case M_ALM_ALARM_BOX_RESET_SUCCESS:
            m_bRecvResetSuc = TRUE;
            SendCommandToBox(M_ALM_ALARM_BOX_CONFIG);
            break;
        case M_ALM_ALARM_BOX_RESEND_ALARM:

            TRACE(MTS_ALARM,S_ALARM_NORMAL);
            m_pParent->ChangeState(new CNormalState(m_pParent));
            return;
        default:
            break;
    }
}
/**********************************************************************
��������    ���º���������澯����ص����MML����
�������    pFrame----����MML������Ϣ����Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CDisconnectState::HandleMMLSetBoxLGT(MSG_INTRA *pFrame)
{
    //��λ��Ϣ  =  �澯�䴦�ڶ���״̬���޷���ѯ�澯��״̬
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_DISCONNECT_NOT_QUERY_LGT);
}
void CDisconnectState::HandleMMLLSTBoxVER(MSG_INTRA *pFrame)
{
    //��λ��Ϣ  =  �澯�䴦�ڶ���״̬���޷���ѯ�澯��汾
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_DISCONNECT_NOT_QUERY_VER);
}
void CDisconnectState::HandleMMLCLRBoxLGT(MSG_INTRA *pFrame)
{
    //��λ��Ϣ  =  �澯�䴦�ڶ���״̬������Ϩ��澯��
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_DISCONNECT_NOT_OFF_LGT);
}
void CDisconnectState::HandleMMLSTPBoxSND(MSG_INTRA *pFrame)
{
    //��λ��Ϣ  =  �澯�䴦�ڶ���״̬������ֹͣ�澯��
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_DISCONNECT_NOT_STOP_SND);
}
void CDisconnectState::HandleMMLRSTALMBox(MSG_INTRA *pFrame)
{
    //��λ��Ϣ  =  �澯�䴦�ڶ���״̬�����ش���
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_DISCONNECT_NOT_HANDLE);
}
/**********************************************************************
��������    ����澯��������
�������    pFrame----���и澯��������Ϣ����Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CDisconnectState::HandleAlarmCmdFromBox(unsigned char* pAlarmBoxFrame, int nLen)
{
     int nCmd = 0;
     CState::HandleAlarmCmdFromBox(pAlarmBoxFrame, nLen);
     AnalysizeCommandFromBox(pAlarmBoxFrame,nLen,nCmd,NULL);       
     switch(nCmd)
     {
          case M_ALM_ALARM_BOX_RESET_SUCCESS:
              TRACE(MTS_ALARM,S_ALARM_INIT);
              m_pParent->ChangeState(new CInitState(m_pParent));
              return;
          case M_ALM_ALARM_BOX_RESEND_ALARM:
          case M_ALM_ALARM_BOX_RESPONSE_ONLINE:
               TRACE(MTS_ALARM,S_ALARM_NORMAL);
               m_pParent->ChangeState(new CNormalState(m_pParent));
               return;
          default:
              break;
     }
}
/**********************************************************************
��������    ��ʼ������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
BOOL CNormalState::Init()
{
    //���4������ĸ澯����
    //��Ϊ0��ô���ݼ�����С�·��澯���ط��澯
    AlarmCountMap::iterator iter = (m_pParent->m_AlarmCount).begin();
    for( ;iter != (m_pParent->m_AlarmCount).end();iter ++)
    {
        int i = 1;
        if(iter->first == AL_EMERGENCY)
        {
            if(iter->second + (m_pParent->m_EvtCsnList).size() > 0)
            {
                //int nCount = iter->second + (m_pParent->m_EvtCsnList).size();
                //Ҫ�෢���Σ���������������ϸ澯�䲻��Ӧ
                SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,AL_EMERGENCY,1);
                for(i = 0 ;i < 4;i++)
                {
                    SendCommandToBox(M_ALM_ALARM_BOX_REDOWN_ALARM,AL_EMERGENCY,1);
                }
            }
            else //��������Ϊ0
            {   //�ָ��澯Ҳ��
                for(i = 0; i < 4; i++)
                {
                    SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,AL_EMERGENCY,0);
                }
            }
        }
        else //���˽����澯����������澯
        {
            if(iter->second > 0)
            {
                SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,iter->first,1);
                //Ҫ�෢���Σ���������������ϸ澯�䲻��Ӧ
                for(i = 0; i < 4; i++)
                {
                    SendCommandToBox(M_ALM_ALARM_BOX_REDOWN_ALARM,iter->first,1);
                }
            }
            else //�����澯����ļ��� �� 0
            {
                for(i = 0; i < 4; i++)
                {
                    SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,iter->first,0);
                }
            }
        }
    }
    //���m_pParent->m_sRecentAlarm��bACK�����==TRUE��ô����ֹͣ�澯������
    //��m_pParent->m_sRecentAlarm��bACK = FALSE
    if((m_pParent->m_sRecentAlarm).bACK == TRUE)
    {
        SendCommandToBox(M_ALM_ALARM_BOX_STOP_SOUND);
        (m_pParent->m_sRecentAlarm).bACK = FALSE;
    }
    //������ ��ΪConvert��ƴ֡�����������﷢��û��
    memset(m_pParent->m_szAlarmBoxVersion,0,64);
    //SendCommandToBox(M_ALM_ALARM_BOX_QUERY_VERSION);
    return TRUE;
}
/**********************************************************************
��������    ��ʱ������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CNormalState::OnTimer()
{
    static int nTimerCnt = 0;
    CState::OnTimer();
    if(m_pParent->m_nCnt >= 30)
    {
        TRACE(MTS_ALARM,S_ALARM_DISCONNECT);
        m_pParent->ChangeState(new CDisconnectState(m_pParent));
        //���͸澯������澯
        SInnerAlarm Alarm;
        Alarm.yAlarmType = AT_EVENT;
        Alarm.uAlarmID = ALARM_ID_ALARMBOX_CONNECT_FAIL;
        CMsgProcessor::SendAlarm(&Alarm);            
    }
    else //���Ӱ汾��ѯ����
    {        
        if((m_pParent->m_szAlarmBoxVersion[0] == 0) && (nTimerCnt % 8 == 0))
        {
            SendCommandToBox(M_ALM_ALARM_BOX_QUERY_VERSION);
        }
    }   
    
    nTimerCnt ++;
}
/**********************************************************************
��������    ���º���������澯����ص����MML����
�������    pFrame----����MML������Ϣ����Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CNormalState::HandleMMLSetBoxLGT(MSG_INTRA *pFrame)
{
    //����������� =  m_pParent->m_AlarmCount[AL_EMERGENCY] + m_pParent->m_EvtCsnList.size()
    //��Ҫ������� =  m_pParent->m_AlarmCount[AL_IMPORTANT]
    //��Ҫ������� =  m_pParent->m_AlarmCount[AL_MINOR]
    //һ�㼶����� =  m_pParent->m_AlarmCount[AL_WARNING]
    char szContent[256];
    char szTemp[32];
    SNPRINTF(szTemp,32,"%s  =  %d%s",S_ALARM_EMERGENCY_CNT,(m_pParent->m_AlarmCount[AL_EMERGENCY] + m_pParent->m_EvtCsnList.size()),STR_NEWLINE);
    strncpy(szContent,szTemp,256);
    SNPRINTF(szTemp,32,"%s  =  %d%s",S_ALARM_IMPORTANT_CNT,m_pParent->m_AlarmCount[AL_IMPORTANT],STR_NEWLINE);
    strncat(szContent,szTemp,256);
    SNPRINTF(szTemp,32,"%s  =  %d%s",S_ALARM_MINOR_CNT,m_pParent->m_AlarmCount[AL_MINOR],STR_NEWLINE);
    strncat(szContent,szTemp,256);
    SNPRINTF(szTemp,32,"%s  =  %d",S_ALARM_NORMAL_CNT,m_pParent->m_AlarmCount[AL_WARNING],STR_NEWLINE);
    strncat(szContent,szTemp,256);
    SendMMLReportToClient(pFrame,NULL,szContent);
}
void CNormalState::HandleMMLLSTBoxVER(MSG_INTRA *pFrame)
{
    if(m_pParent->m_szAlarmBoxVersion[0] == 0)
    {
        //����MML����
        //�澯��汾  =  ��ѯ�޽�������Ժ��ٲ�
        SendMMLReportToClient(pFrame,S_ALARM_BOX_VERSION,S_ALARM_QUERY_NO_RESULT);
    }
    else
    {
        //����MML����
        //�澯��汾  =  ��ѯ�澯��汾
        SendMMLReportToClient(pFrame,S_ALARM_BOX_VERSION,(char *)(&(m_pParent->m_szAlarmBoxVersion[0])));
    }
}
void CNormalState::HandleMMLCLRBoxLGT(MSG_INTRA *pFrame)
{
    int nAlarmLevel = 0;
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);
    pParaResult->GetDigitVal(&nAlarmLevel, 2, 1);
    //��Ϊ�ͻ����͹����ļ��������Ƕ���Ĳ�ͬ
    switch(nAlarmLevel)
    {
    case 1:
        nAlarmLevel = AL_EMERGENCY;
        break;
    case 2:
        nAlarmLevel = AL_IMPORTANT;
        break;
    case 3:
        nAlarmLevel = AL_MINOR;
        break;
    case 4:
        nAlarmLevel = AL_WARNING;
    }

    if(nAlarmLevel == AL_EMERGENCY)
    {
         m_pParent->m_AlarmCount[AL_EMERGENCY] = 0;
         (m_pParent->m_EvtCsnList).clear();
    }
    else
    {
         m_pParent->m_AlarmCount[nAlarmLevel] = 0;
    }
    if((m_pParent->m_sRecentAlarm).yAlarmLevel == nAlarmLevel)
    {
         memset(&(m_pParent->m_sRecentAlarm),0,sizeof(SAlarmInfo));
    }
    (m_pParent->m_sRecentAlarm).bACK = FALSE;
    //�·���Ӧ����ĸ澯������
    SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,nAlarmLevel,0);
    //����MML����
    //����ִ�гɹ�
    SendMMLReportToClient(pFrame,NULL,S_ALARM_OPRSUCCESS);
}
void CNormalState::HandleMMLSTPBoxSND(MSG_INTRA *pFrame)
{
    SendCommandToBox(M_ALM_ALARM_BOX_STOP_SOUND);
    (m_pParent->m_sRecentAlarm).bACK = FALSE;
    //����MML���Ĳ���ִ�гɹ�
    SendMMLReportToClient(pFrame,NULL,S_ALARM_OPRSUCCESS);
}
void CNormalState::HandleMMLRSTALMBox(MSG_INTRA *pFrame)
{
    AlarmCountMap::iterator iter = m_pParent->m_AlarmCount.begin();
    //zyx delete clear cnt
    //����MML���Ĳ���ִ�гɹ�
    SendMMLReportToClient(pFrame,NULL,S_ALARM_OPRSUCCESS);
    TRACE(MTS_ALARM,S_ALARM_INIT);
    m_pParent->ChangeState(new CInitState(m_pParent));
}
/**********************************************************************
��������    �����ڲ��澯����
�������    pFrame----�����ڲ��澯����Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
BOOL CNormalState::HandleAlarm(SActiveAlarm *pFrame)
{
    if(CState::HandleAlarm(pFrame) == FALSE)
        return FALSE;
    if(pFrame->yAlarmType == AT_FAULT
       ||pFrame->yAlarmType == AT_EVENT)
    {
        if(pFrame->yAlarmLevel == AL_EMERGENCY)
        {
              if((m_pParent->m_AlarmCount[AL_EMERGENCY] + m_pParent->m_EvtCsnList.size()) == 1)
              {
                  //��ʹ��0x50�·��澯
                  SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,AL_EMERGENCY,1);
              }
              else
              {
                  //ʹ��0x51�ط��澯
                  SendCommandToBox(M_ALM_ALARM_BOX_REDOWN_ALARM,AL_EMERGENCY,1);
              }
        }
         else
         {
             if(m_pParent->m_AlarmCount[pFrame->yAlarmLevel] == 1)
             {
                 //��ʹ��0x50�·��澯
                 SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,pFrame->yAlarmLevel,1);
             }
             else
             {
                 //ʹ��0x51�ط��澯
                 SendCommandToBox(M_ALM_ALARM_BOX_REDOWN_ALARM,pFrame->yAlarmLevel,1);
             }
        }
    }
    if(pFrame->yAlarmType == AT_RESTORE)
    {
        if(pFrame->yAlarmLevel == AL_EMERGENCY)
        {
            if(m_pParent->m_AlarmCount[AL_EMERGENCY] + m_pParent->m_EvtCsnList.size() == 0)
             {
                //�·���澯������
                SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,AL_EMERGENCY,0);
             }
        }
        else if(m_pParent->m_AlarmCount[pFrame->yAlarmLevel] == 0)
        {
            //�·�0x50����澯��
            SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,pFrame->yAlarmLevel,0);
        }
    }
    return TRUE;
}
/**********************************************************************
��������    ����澯��������
�������    pFrame----���и澯��������Ϣ����Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CNormalState::HandleAlarmCmdFromBox(unsigned char* pAlarmBoxFrame, int nLen)
{
    int nCmd = 0;
    CState::HandleAlarmCmdFromBox(pAlarmBoxFrame, nLen);
    AnalysizeCommandFromBox(pAlarmBoxFrame, nLen, nCmd,&(m_pParent->m_szAlarmBoxVersion[0]));     
    switch(nCmd)
    {
        case M_ALM_ALARM_BOX_RESEND_ALARM:
            {
                AlarmCountMap::iterator iter = m_pParent->m_AlarmCount.begin();
                for(; iter != (m_pParent->m_AlarmCount).end(); iter ++)
                {
                    //���ĳ�����������Ϊ0��ô����澯���ط�һ����Ӧ����ĸ澯
                    if(iter->first == AL_EMERGENCY)
                    {
                        if(((m_pParent->m_EvtCsnList).size() + m_pParent->m_AlarmCount[AL_EMERGENCY]) != 0)
                        {
                            //��Ϊ��ҲҪ�����ط��澯����
                            SendCommandToBox(M_ALM_ALARM_BOX_REDOWN_ALARM,AL_EMERGENCY,1);
                        }
                        continue;
                    }
                    if(iter->second > 0)
                    {
                        SendCommandToBox(M_ALM_ALARM_BOX_REDOWN_ALARM,iter->first,1);
                    }
                }
                break;
            }
        case M_ALM_ALARM_BOX_RESET_SUCCESS:
            {
                TRACE(MTS_ALARM,S_ALARM_INIT);
                m_pParent->ChangeState(new CInitState(m_pParent));
                return;
            }
        default:
            {
                break;
            }
    }
}
/**********************************************************************
��������    ����澯ȷ�ϵ�MML�����
�������    pFrame----����MML������Ϣ����Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CNormalState::HandleMMLAlarmACK(MSG_INTRA *pFrame)
{
    CState::HandleMMLAlarmACK(pFrame);
    if((m_pParent->m_EvtCsnList).size() + m_pParent->m_AlarmCount[AL_EMERGENCY] == 0)
    {
        //��������澯��
        SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,AL_EMERGENCY,0);
    }
    if((m_pParent->m_sRecentAlarm).bACK == TRUE)
    {
        //����ͣ�澯������
        SendCommandToBox(M_ALM_ALARM_BOX_STOP_SOUND);
        (m_pParent->m_sRecentAlarm).bACK = FALSE;
    }
}
/**********************************************************************
��������    �����¼��澯ȷ�ϵ�MML�����
�������    pFrame----���и澯��������Ϣ����Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CNormalState::HandleMMLAlarmAckEVT(MSG_INTRA *pFrame)
{
    CState::HandleMMLAlarmAckEVT(pFrame);
    if((m_pParent->m_EvtCsnList.size() + m_pParent->m_AlarmCount[AL_EMERGENCY])==0)
    {
        //��������澯��
        SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,AL_EMERGENCY,0);
    }
    if((m_pParent->m_sRecentAlarm).bACK == TRUE)
    {
        //����ͣ�澯������
        SendCommandToBox(M_ALM_ALARM_BOX_STOP_SOUND);
        (m_pParent->m_sRecentAlarm).bACK = FALSE;
    }
}
