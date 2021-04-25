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
单元测试
void TRACE(...)
{
    printf("TRACE\n");
}
#define S_ALARM_EMERGENCY_CNT   "紧急告警"
#define S_ALARM_IMPORTANT_CNT   "重要告警"
#define S_ALARM_MINOR_CNT       "次要告警"
#define S_ALARM_NORMAL_CNT      "警告告警"
***********************************************************************/
/**********************************************************************
功能描述    构造函数
输入参数    pReport----报文生成器指针
输出参数    无
返回值      无
抛出异常    无
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
功能描述    析构函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
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
    //析构m_CommBox对象
    if(m_CommBox != NULL)
    {
        delete m_CommBox;
        m_CommBox = NULL;
    }     
}
/**********************************************************************
功能描述    初始化函数，本函数初始化内部状态
输入参数    无
输出参数    无
返回值      初始化是否成功
抛出异常    无
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
    
    //张玉新 增加告警箱初始化
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
功能描述    改变状态函数
输入参数    pS----新的状态
输出参数    无
返回值      无
抛出异常    无
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
功能描述    处理客户端命令函数
输入参数    pFrame----含有客户端命令的消息帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CAlarmBox::HandleMMLCmdFromClient(MSG_INTRA *pFrame)
{
    if(m_pState != NULL)
    {
        m_pState->HandleMMLCmdFromClient(pFrame);
    }
}
/**********************************************************************
功能描述    处理告警箱命令函数
输入参数    pFrame----含有告警箱命令的消息帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CAlarmBox::HandleAlarmCmdFromBox(MSG_INTRA *pFrame)
{
    if(m_pState != NULL)
    {
        m_pState->HandleAlarmCmdFromBox(pFrame->Data, pFrame->Len);           
    }
}
/**********************************************************************
功能描述    处理内部告警函数
输入参数    pFrame----含有内部告警的消息帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CAlarmBox::HandleAlarm(SActiveAlarm *pFrame)
{
    if(m_pState != NULL)
    {
        m_pState->HandleAlarm(pFrame);
    }
}
/**********************************************************************
功能描述    定时器函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CAlarmBox::OnTimer()
{
    static int times =  0;    
    
    if(m_pState != NULL && m_nConnectedMode == CONNECTED_BY_CG)
    { 
        //定时发送握手帧
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
        
        //对于直连的告警箱，定时读取告警箱上报的帧        
        BYTE* pSerialData = NULL;
        int nLen = 0;
        if(m_CommBox != NULL)
        {
            m_CommBox->Read(&pSerialData, &nLen);
        }    
        //读出数据成功或者命令完整（完整的一个命令包至少7个字节 ）
        if(NULL != pSerialData && nLen >= 7)
        {
            if(m_pState != NULL)
            {
                //调用HandleAlarmCmdFromBox处理告警箱上报的数据
                m_pState->HandleAlarmCmdFromBox(pSerialData, nLen);  
                //释放告警箱上报帧的空间
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
功能描述    处理客户端命令函数
输入参数    pB----拥有此状态的告警箱
            pReport----报文生成器的指针
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
CState::CState(CAlarmBox *pB)
{
    //zhangyuxinASSERT(pB != NULL);
    m_pParent = pB;
}
/**********************************************************************
功能描述    处理客户端命令函数
输入参数    pFrame----含有客户端命令的消息帧
输出参数    无
返回值      无
抛出异常    无
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
功能描述    更新内部告警信息函数
输入参数    pAlarm----内部活动告警的消息帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CState::UpdateRecentAlarm(SActiveAlarm *pAlarm)
{
    //TRACE(MTS_ALARM,S_ALARM_UPDATE_BOX_ALARM,pAlarm->uCsn,pAlarm->uAlarmID);
    memcpy(&m_pParent->m_sRecentAlarm,pAlarm,(sizeof(SAlarmInfo) - sizeof(BOOL)));
    (m_pParent->m_sRecentAlarm).bACK = FALSE;
}
/**********************************************************************
功能描述    处理内部告警函数
输入参数    pFrame----含有内部告警的消息帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
BOOL CState::HandleAlarm(SActiveAlarm *pFrame)
{
    if(m_pParent->IsSendToAlarmBox(pFrame->uAlarmID))
    {
        //告警类型 == 故障告警，根据告警级别将相应的级别计数＋1，刷新m_sRecentAlarm             
        if(pFrame->yAlarmType == AT_FAULT)
        {
            m_pParent->m_AlarmCount[pFrame->yAlarmLevel] += 1;
            UpdateRecentAlarm(pFrame);
        }
        //告警类型 == 事件告警 && 告警级别 == 紧急，将流水号入队列, 刷新sRecentAlarm         
        if(pFrame->yAlarmType == AT_EVENT
           && pFrame->yAlarmLevel == AL_EMERGENCY)
        {
            (m_pParent->m_EvtCsnList).push_back(pFrame->uCsn);
            UpdateRecentAlarm(pFrame);
        }
        //告警类型 == 恢复告警，将相应级别的告警计数－1（为零就不再减少），
        //若与m_sRecentAlarm的流水号相等，则更新告警类型为恢复          
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
        TRACE(MTS_ALARM,"告警箱收到告警(LEVEL=%d, ID=%d,Para=%d)",
             pFrame->yAlarmLevel,pFrame->uAlarmID,pFrame->yPara[0]);
        TRACE(MTS_ALARM,"紧急计数=%d, 重要计数=%d, 次要计数=%d, 警告计数=%d, 紧急事件计数=%d",
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
功能描述    处理MML命令——告警确认——函数
输入参数    pFrame----含有MML命令信息的消息帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CState::HandleMMLAlarmACK(MSG_INTRA *pFrame)
{
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);
    //获得MML第一个参数：起始流水号（必须参数）
    int nStartCsn = 0;
    pParaResult->GetDigitVal(&nStartCsn, 1, 1);
    //获得MML第二个参数：终止流水号（必须参数）
    int nEndCsn = 0;
    pParaResult->GetDigitVal(&nEndCsn, 2, 1);
    if(nStartCsn <= (m_pParent->m_sRecentAlarm).uCsn
       && (m_pParent->m_sRecentAlarm).uCsn <= nEndCsn)
    {
        (m_pParent->m_sRecentAlarm).bACK = TRUE;
    }
    //如果紧急事件告警流水号队列中含有在确认流水号范围之内的告警找出这些告警在队列中删掉      
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
功能描述    处理MML命令——事件告——警确认函数
输入参数    pFrame----含有MML命令信息的消息帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CState::HandleMMLAlarmAckEVT(MSG_INTRA *pFrame)
{
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);
    //获得MML参数：流水号（必须参数）
    int nCsn = 0;
    pParaResult->GetDigitVal(&nCsn, 1, 1);
    if((m_pParent->m_sRecentAlarm).uCsn == nCsn)
    {
        (m_pParent->m_sRecentAlarm).bACK = TRUE;
    }
    //在m_pParent->m_EvtCsnList中能够找到这个流水号就删掉他
    (m_pParent->m_EvtCsnList).remove(nCsn);
}
/**********************************************************************
功能描述    发送命令到告警箱函数
输入参数    nCmd----命令码  nAlarmLevel----告警级别  nValue----参数值
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CState::SendCommandToBox(int nCmd, int nAlarmLevel, int nValue)
{
    unsigned char ucCommand[M_ALM_MAX_ALARM_BOX_COMMAND_LEN];
    unsigned char ucLen = 0;
    unsigned char ucChkSum = 0;
    memset(ucCommand, 0, M_ALM_MAX_ALARM_BOX_COMMAND_LEN);
    // 协议格式为 FS(1byte) +len(1byte) + TA(1byte) +SA(1Byte) + len(1Byte)
    //+ Cmd/Rsp(1Byte) + Para1 + para2 + Para3
    // 生成发送命令
    ucCommand[0] = M_ALM_BOX_PROTOCOL_FS;  //头0x7e
    ucCommand[2] = M_ALM_BOX_PROTOCOL_SEND_TA; //主机到告警箱TA
    ucCommand[3] = M_ALM_BOX_PROTOCOL_SEND_SA; //主机到告警箱SA
    ucCommand[5]= (unsigned char)nCmd;
    ucLen = 5;
    switch (nCmd)
    {
    case M_ALM_ALARM_BOX_RESET: // 复位告警箱
        //预期的message为: 0x7e 0x5 0x1f 0x00 0x01 0x02 chksum
        ucCommand[4] = 0x01;  // 填消息长度
        break;
        // 停告警音
    case M_ALM_ALARM_BOX_STOP_SOUND:
        //预期的message为: 0x7e 0x5 0x1f 0x00 0x01 0x07 chksum
        ucCommand[4] = 0x01; // 填消息长度
        break;
        //配置命令
    case  M_ALM_ALARM_BOX_CONFIG:
        //预期的message为: 0x7e 0x5 0x1f 0x00 0x01 0x08 chksum
        ucCommand[4] = 0x01; // 填消息长度
        break;
        // 握手应答帧
    case M_ALM_ALARM_BOX_RESPONSE_ONLINE:
        // 预期的message为: 0x7e 0x5 0x1f 0x00 0x01 0x28 chksum
        ucCommand[4] = 0x01; // 填消息长度
        break;
        //下发告警信息
    case M_ALM_ALARM_BOX_DOWN_ALARM:
        //预期的message为: 0x7e 0x7 0x1f 0x00 0x03 0x50 para1 para2 chksum
        ucCommand[4] = 0x03; // 填消息长度
        ucCommand[6] = (unsigned char) nAlarmLevel;
        ucCommand[7] = (unsigned char) nValue;
        ucLen = 7;
        break;
        // 重发告警信息
    case M_ALM_ALARM_BOX_REDOWN_ALARM:
        //预期的message为: 0x7e 0x7 0x1f 0x00 0x03 0x51 para1 para2 chksum */
        ucCommand[4] =0x03; // 填消息长度
        ucCommand[6] = (unsigned char) nAlarmLevel;
        ucCommand[7] = (unsigned char) nValue;  // 有告警、级别灯闪烁
        ucLen = 7;
        break;
        // 查询软件版本信息
    case M_ALM_ALARM_BOX_QUERY_VERSION:
        // 预期的message为: 0x7e 0x5 0x1f 0x00 0x01 0x52 chksum
        ucCommand[4] = 0x01; // 填消息长度
        ucLen = 5;
        break;
    default:
        break;
    }
    // 帧长度len = message lens + chksum
    ucCommand[1] = (unsigned char) ucCommand[4] + 3 + 1;
    // 计算chksum
    ucLen = (unsigned char)ucCommand[1] + 1;
    ucChkSum = ucCommand[0];//zyx 0
    for (int i = 1; i < ucLen; i++)//zyx i=0
    {
        ucChkSum ^= ucCommand[i];
    }
    ucCommand[ucLen] = (unsigned char)ucChkSum;
    ucCommand[ucLen + 1] = 0x00 ;  //新告警箱采用0x00结束
    
    //对于直连告警箱的模式，将命令直接发给告警箱
    //对于LMT连接告警箱的模式(m_pParent->m_CommBox对象为空)，将命令通过sock端口转发给网管台的告警箱 
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
功能描述    分析告警箱的响应函数
输入参数    pFrame----含有响应命令信息的消息帧
输出参数    nCmd---命令码 pPara---参数数组的指针
返回值      无
抛出异常    无
**********************************************************************/
void CState::AnalysizeCommandFromBox(unsigned char *pAlarmBoxFrame, int nFrameLen,
                                     int &nCmd, unsigned char *pPara)                                    
{
    nCmd  = 0xFF;
    unsigned char ucCheckSum = pAlarmBoxFrame[0];
    int nLen = pAlarmBoxFrame[1];
    if(nLen + 2 < nFrameLen )//如果收到的数据长度不够
    {
        return;
    }        
    // 协议格式为 FS + LEN + MESSAGE + CheckSum
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
功能描述    发送MML报文到客户端函数
输入参数    pFrame----含有MML命令信息的消息帧 pTitle----消息的标题
            pContent---消息的内容
            如果pTitle!= NULL报文格式为
            ＋＋＋ 标头
            命令回显
            Title = Content
            ---End
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CState::SendMMLReportToClient(MSG_INTRA *pFrame,const char *pTitle,const char *pContent)
{
    //构造回送报文
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
    //计算报文长度，按长度分配空间
    int nLength = strlen(m_pParent->m_pReport->GetReport());
    MSG_INTRA *pMsgToClient = new(nLength + 1)MSG_INTRA;
    memcpy(pMsgToClient->Data, m_pParent->m_pReport->GetReport(), nLength);
    pMsgToClient->Data[nLength] = '\0';
    //填写帧头内容
    pMsgToClient->IPAddr      = pFrame->IPAddr;
    pMsgToClient->AppType     = APP_MML_NORM;
    pMsgToClient->SenderPid   = PT_OM;
    pMsgToClient->SenderMid   = MT_ALARM;
    pMsgToClient->ReceiverPid = PT_OUTSIDE;
    pMsgToClient->ReceiverMid = MT_MMLDEBUG;
    //调用框架的发送函数将消息帧发送到网络模块转发
    CWorkModule::SendMsg(pMsgToClient);
    TRACE(MTS_ALARM,S_ALARM_MML_REPORT,m_pParent->m_pReport->GetReport());
}
/**********************************************************************
功能描述    初始化函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
BOOL CInitState::Init()
{
    m_pParent->m_nCnt = 0;
    SendCommandToBox(M_ALM_ALARM_BOX_RESET);
    return TRUE;
}
/**********************************************************************
功能描述    定时器函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CInitState::OnTimer()
{
    CState::OnTimer();
    if(m_pParent->m_nCnt >= 30)
    {
        MSGOUT(MTS_ALARM,MSGOUT_LEVEL_IMPORTANT,S_ALARM_DISCONNECT);
        //张玉新 联调告警箱增加
        TRACE(MTS_ALARM,S_ALARM_DISCONNECT);
        m_pParent->ChangeState(new CDisconnectState(m_pParent));
        //发送告警箱断连告警
        SInnerAlarm Alarm;
        Alarm.yAlarmType = AT_EVENT;
        Alarm.uAlarmID = ALARM_ID_ALARMBOX_CONNECT_FAIL;
        //Alarm.yAlarmPara = 0xFF;
        CMsgProcessor::SendAlarm(&Alarm);       
    }
    /*else
    {
        //张玉新 因为发送的第一个重起命令可能会丢失，所以在收到
        //重起成功之前不断重发。
        if(m_bRecvResetSuc == FALSE)
        SendCommandToBox(M_ALM_ALARM_BOX_RESET);
    }*/
}   
/**********************************************************************
功能描述    以下5个函数为处理MML新增关于告警箱的命令
输入参数    pFrame----含有MML命令信息的消息帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CInitState::HandleMMLSetBoxLGT(MSG_INTRA *pFrame)
{
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_INIT_NOT_QUERY_LGT);
    //定位信息  =  告警箱处于重起状态，无法查询告警灯状态
}
void CInitState::HandleMMLLSTBoxVER(MSG_INTRA *pFrame)
{
    //定位信息  =  告警箱处于重起状态，无法查询告警箱版本
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_INIT_NOT_QUERY_VER);
}
void CInitState::HandleMMLCLRBoxLGT(MSG_INTRA *pFrame)
{
    //定位信息  =  告警箱处于重起状态，不用熄灭告警灯
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_INIT_NOT_OFF_LGT);
}
void CInitState::HandleMMLSTPBoxSND(MSG_INTRA *pFrame)
{
    //定位信息  =  告警箱处于重起状态，不用停止告警音
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_INIT_NOT_STOP_SND);
}
void CInitState::HandleMMLRSTALMBox(MSG_INTRA *pFrame)
{
    //定位信息  =  告警箱处于重起状态，不必处理
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_INIT_NOT_HANDLE);
}
/**********************************************************************
功能描述    处理告警箱的命令函数
输入参数    pFrame----含有告警箱命令信息的消息帧
输出参数    无
返回值      无
抛出异常    无
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
功能描述    以下函数处理跟告警箱相关的五个MML命令
输入参数    pFrame----含有MML命令信息的消息帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CDisconnectState::HandleMMLSetBoxLGT(MSG_INTRA *pFrame)
{
    //定位信息  =  告警箱处于断连状态，无法查询告警灯状态
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_DISCONNECT_NOT_QUERY_LGT);
}
void CDisconnectState::HandleMMLLSTBoxVER(MSG_INTRA *pFrame)
{
    //定位信息  =  告警箱处于断连状态，无法查询告警箱版本
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_DISCONNECT_NOT_QUERY_VER);
}
void CDisconnectState::HandleMMLCLRBoxLGT(MSG_INTRA *pFrame)
{
    //定位信息  =  告警箱处于断连状态，不用熄灭告警灯
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_DISCONNECT_NOT_OFF_LGT);
}
void CDisconnectState::HandleMMLSTPBoxSND(MSG_INTRA *pFrame)
{
    //定位信息  =  告警箱处于断连状态，不用停止告警音
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_DISCONNECT_NOT_STOP_SND);
}
void CDisconnectState::HandleMMLRSTALMBox(MSG_INTRA *pFrame)
{
    //定位信息  =  告警箱处于断连状态，不必处理
    SendMMLReportToClient(pFrame,S_ALARM_ALARMPARA,S_ALARM_BOX_DISCONNECT_NOT_HANDLE);
}
/**********************************************************************
功能描述    处理告警箱的命令函数
输入参数    pFrame----含有告警箱命令信息的消息帧
输出参数    无
返回值      无
抛出异常    无
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
功能描述    初始化函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
BOOL CNormalState::Init()
{
    //检查4个级别的告警计数
    //不为0那么根据计数大小下发告警和重发告警
    AlarmCountMap::iterator iter = (m_pParent->m_AlarmCount).begin();
    for( ;iter != (m_pParent->m_AlarmCount).end();iter ++)
    {
        int i = 1;
        if(iter->first == AL_EMERGENCY)
        {
            if(iter->second + (m_pParent->m_EvtCsnList).size() > 0)
            {
                //int nCount = iter->second + (m_pParent->m_EvtCsnList).size();
                //要多发几次，否则断连后在连上告警箱不响应
                SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,AL_EMERGENCY,1);
                for(i = 0 ;i < 4;i++)
                {
                    SendCommandToBox(M_ALM_ALARM_BOX_REDOWN_ALARM,AL_EMERGENCY,1);
                }
            }
            else //紧急计数为0
            {   //恢复告警也是
                for(i = 0; i < 4; i++)
                {
                    SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,AL_EMERGENCY,0);
                }
            }
        }
        else //除了紧急告警以外的其他告警
        {
            if(iter->second > 0)
            {
                SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,iter->first,1);
                //要多发几次，否则断连后在连上告警箱不响应
                for(i = 0; i < 4; i++)
                {
                    SendCommandToBox(M_ALM_ALARM_BOX_REDOWN_ALARM,iter->first,1);
                }
            }
            else //其他告警级别的计数 ＝ 0
            {
                for(i = 0; i < 4; i++)
                {
                    SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,iter->first,0);
                }
            }
        }
    }
    //检查m_pParent->m_sRecentAlarm的bACK，如果==TRUE那么发送停止告警音命令
    //将m_pParent->m_sRecentAlarm的bACK = FALSE
    if((m_pParent->m_sRecentAlarm).bACK == TRUE)
    {
        SendCommandToBox(M_ALM_ALARM_BOX_STOP_SOUND);
        (m_pParent->m_sRecentAlarm).bACK = FALSE;
    }
    //张玉新 因为Convert不拼帧，所以在这里发送没用
    memset(m_pParent->m_szAlarmBoxVersion,0,64);
    //SendCommandToBox(M_ALM_ALARM_BOX_QUERY_VERSION);
    return TRUE;
}
/**********************************************************************
功能描述    定时器函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CNormalState::OnTimer()
{
    static int nTimerCnt = 0;
    CState::OnTimer();
    if(m_pParent->m_nCnt >= 30)
    {
        TRACE(MTS_ALARM,S_ALARM_DISCONNECT);
        m_pParent->ChangeState(new CDisconnectState(m_pParent));
        //发送告警箱断连告警
        SInnerAlarm Alarm;
        Alarm.yAlarmType = AT_EVENT;
        Alarm.uAlarmID = ALARM_ID_ALARMBOX_CONNECT_FAIL;
        CMsgProcessor::SendAlarm(&Alarm);            
    }
    else //增加版本查询命令
    {        
        if((m_pParent->m_szAlarmBoxVersion[0] == 0) && (nTimerCnt % 8 == 0))
        {
            SendCommandToBox(M_ALM_ALARM_BOX_QUERY_VERSION);
        }
    }   
    
    nTimerCnt ++;
}
/**********************************************************************
功能描述    以下函数处理跟告警箱相关的五个MML命令
输入参数    pFrame----含有MML命令信息的消息帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CNormalState::HandleMMLSetBoxLGT(MSG_INTRA *pFrame)
{
    //紧急级别计数 =  m_pParent->m_AlarmCount[AL_EMERGENCY] + m_pParent->m_EvtCsnList.size()
    //重要级别计数 =  m_pParent->m_AlarmCount[AL_IMPORTANT]
    //次要级别计数 =  m_pParent->m_AlarmCount[AL_MINOR]
    //一般级别计数 =  m_pParent->m_AlarmCount[AL_WARNING]
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
        //返回MML报文
        //告警箱版本  =  查询无结果，请稍后再查
        SendMMLReportToClient(pFrame,S_ALARM_BOX_VERSION,S_ALARM_QUERY_NO_RESULT);
    }
    else
    {
        //返回MML报文
        //告警箱版本  =  查询告警箱版本
        SendMMLReportToClient(pFrame,S_ALARM_BOX_VERSION,(char *)(&(m_pParent->m_szAlarmBoxVersion[0])));
    }
}
void CNormalState::HandleMMLCLRBoxLGT(MSG_INTRA *pFrame)
{
    int nAlarmLevel = 0;
    CParseResult *pParaResult = *((CParseResult **)pFrame->Data);
    pParaResult->GetDigitVal(&nAlarmLevel, 2, 1);
    //因为客户端送过来的级别与我们定义的不同
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
    //下发相应级别的告警灯命令
    SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,nAlarmLevel,0);
    //返回MML报文
    //操作执行成功
    SendMMLReportToClient(pFrame,NULL,S_ALARM_OPRSUCCESS);
}
void CNormalState::HandleMMLSTPBoxSND(MSG_INTRA *pFrame)
{
    SendCommandToBox(M_ALM_ALARM_BOX_STOP_SOUND);
    (m_pParent->m_sRecentAlarm).bACK = FALSE;
    //返回MML报文操作执行成功
    SendMMLReportToClient(pFrame,NULL,S_ALARM_OPRSUCCESS);
}
void CNormalState::HandleMMLRSTALMBox(MSG_INTRA *pFrame)
{
    AlarmCountMap::iterator iter = m_pParent->m_AlarmCount.begin();
    //zyx delete clear cnt
    //返回MML报文操作执行成功
    SendMMLReportToClient(pFrame,NULL,S_ALARM_OPRSUCCESS);
    TRACE(MTS_ALARM,S_ALARM_INIT);
    m_pParent->ChangeState(new CInitState(m_pParent));
}
/**********************************************************************
功能描述    处理内部告警函数
输入参数    pFrame----含有内部告警的消息帧
输出参数    无
返回值      无
抛出异常    无
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
                  //则使用0x50下发告警
                  SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,AL_EMERGENCY,1);
              }
              else
              {
                  //使用0x51重发告警
                  SendCommandToBox(M_ALM_ALARM_BOX_REDOWN_ALARM,AL_EMERGENCY,1);
              }
        }
         else
         {
             if(m_pParent->m_AlarmCount[pFrame->yAlarmLevel] == 1)
             {
                 //则使用0x50下发告警
                 SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,pFrame->yAlarmLevel,1);
             }
             else
             {
                 //使用0x51重发告警
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
                //下发灭告警灯命令
                SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,AL_EMERGENCY,0);
             }
        }
        else if(m_pParent->m_AlarmCount[pFrame->yAlarmLevel] == 0)
        {
            //下发0x50灭掉告警灯
            SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,pFrame->yAlarmLevel,0);
        }
    }
    return TRUE;
}
/**********************************************************************
功能描述    处理告警箱的命令函数
输入参数    pFrame----含有告警箱命令信息的消息帧
输出参数    无
返回值      无
抛出异常    无
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
                    //如果某个级别计数不为0那么就向告警箱重发一次相应级别的告警
                    if(iter->first == AL_EMERGENCY)
                    {
                        if(((m_pParent->m_EvtCsnList).size() + m_pParent->m_AlarmCount[AL_EMERGENCY]) != 0)
                        {
                            //不为空也要发送重发告警命令
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
功能描述    处理告警确认的MML命令函数
输入参数    pFrame----含有MML命令信息的消息帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CNormalState::HandleMMLAlarmACK(MSG_INTRA *pFrame)
{
    CState::HandleMMLAlarmACK(pFrame);
    if((m_pParent->m_EvtCsnList).size() + m_pParent->m_AlarmCount[AL_EMERGENCY] == 0)
    {
        //灭掉紧急告警灯
        SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,AL_EMERGENCY,0);
    }
    if((m_pParent->m_sRecentAlarm).bACK == TRUE)
    {
        //发送停告警音命令
        SendCommandToBox(M_ALM_ALARM_BOX_STOP_SOUND);
        (m_pParent->m_sRecentAlarm).bACK = FALSE;
    }
}
/**********************************************************************
功能描述    处理事件告警确认的MML命令函数
输入参数    pFrame----含有告警箱命令信息的消息帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CNormalState::HandleMMLAlarmAckEVT(MSG_INTRA *pFrame)
{
    CState::HandleMMLAlarmAckEVT(pFrame);
    if((m_pParent->m_EvtCsnList.size() + m_pParent->m_AlarmCount[AL_EMERGENCY])==0)
    {
        //灭掉紧急告警灯
        SendCommandToBox(M_ALM_ALARM_BOX_DOWN_ALARM,AL_EMERGENCY,0);
    }
    if((m_pParent->m_sRecentAlarm).bACK == TRUE)
    {
        //发送停告警音命令
        SendCommandToBox(M_ALM_ALARM_BOX_STOP_SOUND);
        (m_pParent->m_sRecentAlarm).bACK = FALSE;
    }
}
