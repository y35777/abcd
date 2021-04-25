 //本文件由颜文远2002.04.17实现 

/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
#define THISMODULE "ALARM"
/****** End For PC-LINT ******/
#include "../toolbase/ini_file.h" //张玉新 2002-05-09
#include "../utility/config.h"
#include "../include/base_type.h"
#include "../alarm_util/alarm_config.h"
#include "../alarm_util/alarm_struct.h"
#include "bin_alarm_handler.h"
#include "../include/comm_struct_out.h"
//构造函数
CBinAlarmHandler::CBinAlarmHandler(BYTE yAccessPointID, UINT2 uBaseAlarmID,
                                   const char* szAlarmShield, int nProductType,
                                   int nBillsvrNo, CAlarmServer* pAlarmServer,
                                   const char* AccessPointName)
                                   : CAlarmHandler(nBillsvrNo, pAlarmServer)                                   
{
    ASSERT((AP_PS_RESERVED != nProductType) && (AP_PS != nProductType));

    memcpy(m_szAlarmShield, szAlarmShield, 128);
    m_yAPID = yAccessPointID;
    m_nAPType = nProductType;
    m_uBaseAlarmID = uBaseAlarmID;

    //根据配置文件决定是否处理所有接入点的告警
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

    m_bProcAllAPAlarm = iniFile.GetInt(AccessPointName, CFG_AP_KEY_PROC_ALL_AP_ALARM,
                               CFG_AP_KEY_PROC_ALL_AP_ALARM_DEFAULT);
}

/**********************************************************************
功能描述    初始化函数
输入参数    szCfgPath——对应的配置文件的路径
输出参数    无
返回值      0表示成功，非0表示失败
抛出异常    申请空间失败抛出异常
**********************************************************************/
/*int CBinAlarmHandler::Init(const char* szCfgPath)
{
    if(CAlarmHandler::Init(szCfgPath) != ERR_SUCCESS)
        return ERR_FAIL;
    m_pAlarmCfg->AlarmVisit(CAlarmCfgVisitor(this));
    return ERR_SUCCESS;
}*/
/**********************************************************************
功能描述    处理从其它模块发送过来的非MML消息
输入参数    pFrame——所收到的内部消息
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBinAlarmHandler::HandleFrameFromApp(MSG_INTRA *pFrame)
{
    //by ldf 2004-06-15 对应问题单SWPD05582
    if (pFrame->CmdCode == CMD_MODULE_ACTIVE) 
    {
        return;
    }
    //end

    if (CMD_ALARM_RESPONSE == pFrame->CmdCode)  //告警应答
    {
        if ((m_yAPID == pFrame->SenderPid) || m_bProcAllAPAlarm)
        {
            AlarmReceived(pFrame);
        }
    }
    else    //内部告警
    {
        ASSERT(CMD_INNER_ALARM == pFrame->CmdCode);

                SInnerAlarm* pInnerAlarm = (SInnerAlarm* )(pFrame->Data);
        SActiveAlarm* pActiveAlarm = NULL;
        //只发送从以下进程过来的告警：双机进程、KERNEL进程、OM进程、
        //本对象对应的接入点进程(根据配置决定是否处理所有的接入点进程)
        SAlarmExplain *pExplain = NULL;
        pExplain = m_pAlarmCfg->GetAlarmExplain(pInnerAlarm->uAlarmID);
        if ((PT_KERNEL == pFrame->SenderPid)
            || (PT_CLSTR == pFrame->SenderPid)
            || (PT_OM == pFrame->SenderPid)
            || (PT_SELF == pFrame->SenderPid) )
        {
            ASSERT(sizeof(SInnerAlarm) == pFrame->Len);           

            AddAlarm(pInnerAlarm, &pActiveAlarm);
        }
        else if(m_yAPID == pFrame->SenderPid
                || m_bProcAllAPAlarm)
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

            AddAlarm(pInnerAlarm, &pActiveAlarm);
        }

    }
}

/*void CBinAlarmHandler::HandleFrameFromMML(MSG_INTRA *pFrame)
{
    //获得MML命令数据
    CParseResult *pParaResult = *(CParseResult **)pFrame->Data;
    
    //根据MML命令处理消息
    switch (pParaResult->GetCmdCode())
    {
    case MML_LST_ALM:
        HandleCmdLstAlm(pFrame);
        break;
        
    case MML_SET_ALMSHIELD:
        HandleCmdSetShield(pFrame);
        break;
        
    default:
        break;
    }
    
    //delete pParaResult;
}*/

/**********************************************************************
功能描述    往活动告警队列中加入一条新的告警
输入参数    pNewAlarm——待加入的新告警
输出参数    ppActiveAlarm——在活动告警队列中新增加的活动告警
返回值      增加新告警的结果
抛出异常    无
**********************************************************************/
ALARM_ADD_RESULT CBinAlarmHandler::AddAlarm(SInnerAlarm* pNewAlarm, SActiveAlarm** ppActiveAlarm, int nRecoverType)
{
    ASSERT(pNewAlarm->uAlarmID < 128);

    ALARM_ADD_RESULT nResult = ADD_OK;

    //如果要发送的告警被屏蔽，则不发送
    if ('0' != m_szAlarmShield[pNewAlarm->uAlarmID])        
    {
        nResult = CAlarmHandler::AddAlarm(pNewAlarm, ppActiveAlarm, nRecoverType);
    }

    return nResult;
}
/**********************************************************************
功能描述    从SActiveAlarm结构中获取用于发往主机的告警ID
输入参数    pAlarm——指定的SActiveAlarm结构指针
输出参数    无
返回值      用于发往主机的告警ID
抛出异常    无
备注        在告警模块内部，故障告警以及相应的恢复告警的告警ID相同，
            但是在发往主机的告警消息中，恢复告警的ID为相应的故障告警的ID加1，
            而且发往主机的告警ID和内部告警ID间相差一个告警ID基数
**********************************************************************/
UINT2 CBinAlarmHandler::GetOuterAlarmId(SActiveAlarm* pAlarm)
{
    UINT2 uAlarmId;
    if (AT_RESTORE == pAlarm->yAlarmType)
    {
        uAlarmId = pAlarm->uAlarmID + 1;
    }
    else
    {
        uAlarmId = pAlarm->uAlarmID;
    }
    uAlarmId += m_uBaseAlarmID;

    return uAlarmId;
}

/**********************************************************************
功能描述    根据主机应答中的告警ID来设置指定SActiveAlarm结构中的
            告警ID以及告警类型
输入参数    pAlarm——等待设置告警ID及告警类型的SActiveAlarm结构指针
            uAlarmId——主机应答消息中的告警ID
输出参数    pAlarm——设置了告警ID及告警类型后的SActiveAlarm结构指针
返回值      无
抛出异常    无
备注        在告警模块内部，故障告警以及相应的恢复告警的告警ID相同，
            但是在发往主机的告警消息中，恢复告警的ID为相应的故障告警的ID加1
            而且发往主机的告警ID和内部告警ID间相差一个告警ID基数
**********************************************************************/
void CBinAlarmHandler::SetInnerAlarmId(SActiveAlarm* pAlarm, UINT2 uAlarmId)
{
    ASSERT(uAlarmId >= m_uBaseAlarmID);

    UINT2 uInnerAlarmId = uAlarmId - m_uBaseAlarmID;

    //发往主机的恢复告警ID不会配置，所以如果找不到该告警ID，则为恢复告警
    SAlarmExplain* pAlarmExplain = m_pAlarmCfg->GetAlarmExplain(uInnerAlarmId);
    if (NULL == pAlarmExplain)  //恢复告警
    {
        pAlarm->uAlarmID = uInnerAlarmId - 1;
        pAlarm->yAlarmType = AT_RESTORE;
    }
    else if(AT_FAULT == pAlarmExplain->yAlarmType)  //故障告警
    {
        pAlarm->uAlarmID = uInnerAlarmId;
        pAlarm->yAlarmType = AT_FAULT;
    }
    else //事件告警
    {
        pAlarm->uAlarmID = uInnerAlarmId;
        pAlarm->yAlarmType = AT_EVENT;
    }
}

//////////////////////////////////////////////////////////////////////
// CSoftXBinAlarmHandler Class
//////////////////////////////////////////////////////////////////////
CSoftXBinAlarmHandler::CSoftXBinAlarmHandler(BYTE yAccessPointID,UINT2 uBaseAlarmID,
                                             const char* szAlarmShield,int nProductType,
                                             int nBillsvrNo, CAlarmServer* pAlarmServer,
                                             const char* AccessPointName)
                                             :CBinAlarmHandler(yAccessPointID,uBaseAlarmID,
                                             szAlarmShield,nProductType,
                                             nBillsvrNo, pAlarmServer,
                                             AccessPointName)                                             
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
    m_nTimeOut = iniFile.GetInt(AccessPointName,CFG_AP_ALARM_SEND_TIMEOUT,CFG_AP_ALARM_SEND_TIMEOUT_DEFAULT);
    m_nAlarmTimerCnt = 0;
    m_nLastSendHandle = 0;
}
/**********************************************************************
功能描述    秒时钟处理函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CSoftXBinAlarmHandler::Timer1Sec()
{
    //张玉新 联调增加代码
    CAlarmHandler::Timer1Sec();
    if(m_ActiveAlarmList.size() > 0)
    {
        if(m_nAlarmTimerCnt == 0 || m_nAlarmTimerCnt >= m_nTimeOut)
        {
            //到了超时计数，要重发告警
            if(m_nAlarmTimerCnt >= m_nTimeOut)
            {
                TRACEDebug(MTS_ALARM,S_ALARM_RESEND_NEXT);
                m_nAlarmTimerCnt = 0;
            }

            //遍历活动告警队列
            SActiveAlarm* pActiveAlarm = NULL;
            CActiveAlarmList::iterator iter = m_ActiveAlarmList.begin();
            while(iter != m_ActiveAlarmList.end())
            {
                pActiveAlarm = (*iter); 
                if(NULL == pActiveAlarm)
                {
                    iter = m_ActiveAlarmList.erase(iter);
                    continue;
                }

                //没有收到应答则发送告警
                if(pActiveAlarm->yReceived == AR_NOT_RECEIVED)
                {
                    TRACEDebug(MTS_ALARM,S_ALARM_SEND_BIN_ALARM,
                           pActiveAlarm->uAlarmID,
                           pActiveAlarm->yAlarmType,
                           pActiveAlarm->yPara[0]);
                    MSGOUT(MTS_COMM, MSGOUT_LEVEL_URGENT,
                           S_ALARM_SEND_BIN_ALARM,
                           pActiveAlarm->uAlarmID,
                           pActiveAlarm->yAlarmType,
                           pActiveAlarm->yPara[0]);

                    SendAlarm(pActiveAlarm);

                    //开始对应答时间进行计数
                    m_nAlarmTimerCnt++;

                    //每次只发送一个告警
                    break;
                }
                
                iter++;
            }
        }
        else
        {
            //还没有收到应答，计数器加1
            m_nAlarmTimerCnt++;
        }
    } 
}
/**********************************************************************
功能描述    处理收到的主机告警应答
输入参数    pFrame——收到的告警应答消息
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CSoftXBinAlarmHandler::AlarmReceived(MSG_INTRA* pFrame)
{
    SActiveAlarm *pTmp = NULL;
    if(ParseAlarmResponse(pFrame,pTmp))
    {
        SActiveAlarm* pActiveAlarm = NULL;
        CActiveAlarmList::iterator iter = m_ActiveAlarmList.begin();
        while(iter != m_ActiveAlarmList.end())
        {
            pActiveAlarm = (*iter); 
            if(NULL == pActiveAlarm)
            {
                iter = m_ActiveAlarmList.erase(iter);
                continue;
            }

            if(pActiveAlarm->yReceived == AR_NOT_RECEIVED)
            {
                pActiveAlarm->yReceived = AR_RECEIVED;
                TRACEDebug(MTS_ALARM,S_ALARM_RECIEVE_RESPONSE,
                       pActiveAlarm->uAlarmID,
                       pActiveAlarm->yAlarmType,
                       pActiveAlarm->yPara[0]);
                MSGOUT(MTS_COMM, MSGOUT_LEVEL_URGENT,
                       S_ALARM_RECIEVE_RESPONSE,
                       pActiveAlarm->uAlarmID,
                       pActiveAlarm->yAlarmType,
                       pActiveAlarm->yPara[0]);


                //如果是故障告警的应答，则继续在活动告警队列中等待
                if(pActiveAlarm->yAlarmType != AT_FAULT)
                {
                    delete pActiveAlarm;
                    iter = m_ActiveAlarmList.erase(iter);
                }

                m_nAlarmTimerCnt = 0;
                m_nLastSendHandle++;

                break;
            }
            iter++;
        }
    }
}
/**********************************************************************
功能描述    根据SActiveAlarm结构发送二进制告警
输入参数    pAlarm——待发送的二进制告警的信息
输出参数    无
返回值      无
抛出异常    在申请空间失败时抛出异常
**********************************************************************/
void CSoftXBinAlarmHandler::SendAlarm(SActiveAlarm* pAlarm)
{
    //根据pAlarm的内容填写相应的MSG_INTRA消息，并发送给网络模块
    MSG_INTRA* pAlarmMsg
         = new(_SYSFRAME_EXHEAD_LENGTH + _SYS_ALARM_INFO_LENGTH) MSG_INTRA;
    if (NULL == pAlarmMsg)
    {
        throw "Fail to new pAlarmMsg";
    }
    pAlarmMsg->SenderPid = PT_OM;
    pAlarmMsg->SenderMid = MT_ALARM;
    pAlarmMsg->ReceiverPid = m_yAPID;
    pAlarmMsg->ReceiverMid = MT_NETWORK;
    pAlarmMsg->CmdCode = CMD_BIN_ALARM;

    SSysFrame* pSysFrame = (SSysFrame* )(pAlarmMsg->Data);
    pSysFrame->sender_mid(SYS_MID_BAU);
    pSysFrame->sender_pid(SYS_PID_BAU);
    pSysFrame->receiver_mid(SYS_MID_BAM);
    pSysFrame->receiver_pid(SYS_PID_ALARM);
    pSysFrame->nLen(_SYS_ALARM_INFO_LENGTH);
    pSysFrame->ucChannelNo(9);
    pSysFrame->ulBamTaskID(2);
    pSysFrame->ucHandle(m_nLastSendHandle); //张玉新联调时修改
    pSysFrame->ucCmdCode(SYS_CMD_ALARM);

    SSysAlarmInfo* pSysAlarmInfo = (SSysAlarmInfo* )(pSysFrame->Data());
    pSysAlarmInfo->ulAlarmSN(pAlarm->uCsn); //当前暂且将本域填为0
    pSysAlarmInfo->ulCauseID(1);//Cause ID = 1
    pSysAlarmInfo->ulAlarmID(GetOuterAlarmId(pAlarm));
    pSysAlarmInfo->ucAlarmType(pAlarm->yAlarmType);
    pSysAlarmInfo->ucByNMSType(0); //Not to send to NMS
    pSysAlarmInfo->ucAlarmBoxType(pAlarm->yFuncID);
    pSysAlarmInfo->ucAlarmLevel(pAlarm->yAlarmLevel);
    pSysAlarmInfo->ucSubSystemId(pAlarm->ySubSysID);
    pSysAlarmInfo->uwYear(pAlarm->uYear);
    pSysAlarmInfo->ucMonth(pAlarm->yMonth);
    pSysAlarmInfo->ucDate(pAlarm->yDay);
    pSysAlarmInfo->ucHour(pAlarm->yHour);
    pSysAlarmInfo->ucMinute(pAlarm->yMin);
    pSysAlarmInfo->ucSecond(pAlarm->ySec);
    pSysAlarmInfo->ulCpuID(pAlarm->yCPUPos);
    pSysAlarmInfo->ulAlarmPID(pAlarm->yModuleNO);

    //根据告警配置获取告警的参数个数
    SAlarmExplain* pAlarmExplain = m_pAlarmCfg->GetAlarmExplain(pAlarm->uAlarmID);
    ASSERT(NULL != pAlarmExplain);
    //zhangyuxin modify + 1
    pSysAlarmInfo->uwParaLength(MAX_ALARM_PARA);//软交换的参数最大只能为78
    int nParaNum = pAlarmExplain->yParaNum;

    //无参数或一个参数的情况
    memcpy(pSysAlarmInfo->ucPara, pAlarm->yPara, pAlarmExplain->yParaNum);
    pSysAlarmInfo->ucPara[pAlarmExplain->yParaNum] = pAlarm->yModuleNO;

    //两个参数的情况
    if(nParaNum == 2)
    {
        pSysAlarmInfo->ucPara[1] = pAlarm->yModuleNO;
        memcpy(&pSysAlarmInfo->ucPara[2], &pAlarm->yPara[1], MAX_ALARM_PARA - 3);
        pSysAlarmInfo->ucPara[MAX_ALARM_PARA - 1] = '\0';
    }        

    CWorkModule::SendMsg(pAlarmMsg);
}    
/**********************************************************************
功能描述    分析收到的主机的二进制告警应答消息，从中得到相应的告警信息
输入参数    pMsg——收到的二进制告警应答的消息
输出参数    无
返回值      从pMsg中分析出来的告警消息，调用者不得删除
抛出异常    无
**********************************************************************/
BOOL CSoftXBinAlarmHandler::ParseAlarmResponse(MSG_INTRA* pMsg,SActiveAlarm *&pActive)
{
    //张玉新 2002-05-10 必须写，否则以后查找告警队列会失败。
    //根据pMsg的Data域中的信息填写AlarmInfo
    SSysFrame* pSysFrame = (SSysFrame* )(pMsg->Data);
    if(_SYSFRAME_EXPAND_LENGTH == pSysFrame->nLen()
      && pSysFrame->sender_mid() == SYS_MID_BAM
      && pSysFrame->receiver_mid() == SYS_MID_BAU
      && pSysFrame->ucHandle() == m_nLastSendHandle)
    {
        return TRUE;
    }
    else
    {
        MSGOUT(MTS_COMM, MSGOUT_LEVEL_URGENT,
               S_ALARM_CORRECT_BIN_RESPONSE,
               _SYSFRAME_EXPAND_LENGTH,
               SYS_MID_BAM,SYS_MID_BAU,
               m_nLastSendHandle);
        MSGOUT(MTS_COMM, MSGOUT_LEVEL_URGENT,
               S_ALARM_RECEIVE_WRONG_RESPONSE,
               pSysFrame->nLen(),pSysFrame->sender_mid(),
               pSysFrame->receiver_mid(),
               pSysFrame->ucHandle());

        return FALSE;
    }
}

//////////////////////////////////////////////////////////////////////
// CMSCBinAlarmHandler Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMSCBinAlarmHandler::CMSCBinAlarmHandler(BYTE yAccessPointID, UINT2 uBaseAlarmID,
                                         const char* szAlarmShield, int nProductType,
                                         int nBillsvrNo, CAlarmServer* pAlarmServer,
                                         const char* AccessPointName)
                                         :CBinAlarmHandler(yAccessPointID, uBaseAlarmID,
                                         szAlarmShield, nProductType,
                                         nBillsvrNo, pAlarmServer,
                                         AccessPointName)                                         
{
}
/**********************************************************************
功能描述    秒时钟处理函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CMSCBinAlarmHandler::Timer1Sec()
{
    CAlarmHandler::Timer1Sec();
    SActiveAlarm* pActiveAlarm = NULL;
    CActiveAlarmList::iterator i = m_ActiveAlarmList.begin();

    //遍历m_ActiveAlarmList中的所有活动告警，根据告警类型分别进行不同的处理
    while (i != m_ActiveAlarmList.end())
    {
        pActiveAlarm = *i;
        if (NULL == pActiveAlarm)
        {
            i = m_ActiveAlarmList.erase(i);
            continue;
        }

        if (AT_EVENT == pActiveAlarm->yAlarmType)
        //对于每个事件告警，发送指定的次数
        {
            SendAlarm(pActiveAlarm);
            TRACEDebug(MTS_ALARM,S_ALARM_SEND_BIN_ALARM,
                       pActiveAlarm->uAlarmID,
                   pActiveAlarm->yAlarmType,
                   pActiveAlarm->yPara[0]);
            pActiveAlarm->ySendTimes--;
            if (pActiveAlarm->ySendTimes <= 0)
            {
                i = m_ActiveAlarmList.erase(i);
                delete pActiveAlarm;//张玉新 2002-05-10
                pActiveAlarm = NULL;
                continue;
            }
        }
        else if (AT_FAULT == pActiveAlarm->yAlarmType)
        //如果故障告警未收到应答，则发送
        {
            if (AR_RECEIVED != pActiveAlarm->yReceived)
            {
                SendAlarm(pActiveAlarm);
                TRACEDebug(MTS_ALARM,S_ALARM_SEND_BIN_ALARM,
                    pActiveAlarm->uAlarmID,
                    pActiveAlarm->yAlarmType,
                    pActiveAlarm->yPara[0]);
            }
        }
        else
        //如果恢复告警未收到应答，则发送，如果已经收到应答，则删除
        {
            ASSERT(AT_RESTORE == pActiveAlarm->yAlarmType);

            if (AR_RECEIVED != pActiveAlarm->yReceived)
            {
                SendAlarm(pActiveAlarm);
                TRACEDebug(MTS_ALARM,S_ALARM_SEND_BIN_ALARM,
                    pActiveAlarm->uAlarmID,
                    pActiveAlarm->yAlarmType,
                    pActiveAlarm->yPara[0]);
            }
            else
            {
                i = m_ActiveAlarmList.erase(i);
                delete pActiveAlarm; //张玉新 2005-05-10
                pActiveAlarm = NULL;
                continue;
            }
        }

        i++;
    }

}
/**********************************************************************
功能描述    处理收到的主机告警应答
输入参数    pFrame——收到的告警应答消息
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CMSCBinAlarmHandler::AlarmReceived(MSG_INTRA* pFrame)
{
    ASSERT(CMD_ALARM_RESPONSE == pFrame->CmdCode);

    SActiveAlarm* pRspAlarm = NULL;
    
    //by lx SWPD03356
    if(!ParseAlarmResponse(pFrame,pRspAlarm))
    {
        TRACE(MTS_ALARM,S_ALARM_ALARM_RESPONSE_PACKAGE_ERROR);
        return;
    }
    //end by lx
    
    //zhangyuxin modify
    if(pRspAlarm->yModuleNO != m_uBillsvrNo)
    {
        TRACE(MTS_ALARM,S_ALARM_ALARM_RESPONSE_ERROR);
        return;
    }
    //over
    //如果m_ActiveAlarmList队列中不存在对应的告警（利用告警ID、
    //告警参数同时匹配），则直接返回；
    //否则将其yReceived置为AR_RECEIVED，但不从活动告警队列中消除该告警
    SActiveAlarm* pActiveAlarm = NULL;
    pActiveAlarm = m_ActiveAlarmList.Find((SActiveAlarm* )pRspAlarm);
    if (NULL != pActiveAlarm)
    {
        pActiveAlarm->yReceived = AR_RECEIVED;
        TRACEDebug(MTS_ALARM,S_ALARM_RECIEVE_RESPONSE,
                   pActiveAlarm->uAlarmID,
                   pActiveAlarm->yAlarmType,
                   pActiveAlarm->yPara[0]);
    }
}
/**********************************************************************
功能描述    根据SActiveAlarm结构发送二进制告警
输入参数    pAlarm——待发送的二进制告警的信息
输出参数    无
返回值      无
抛出异常    在申请空间失败时抛出异常
**********************************************************************/
void CMSCBinAlarmHandler::SendAlarm(SActiveAlarm* pAlarm)
{
    SAlarmExplain* pAlarmExplain = m_pAlarmCfg->GetAlarmExplain(pAlarm->uAlarmID);
    ASSERT(NULL != pAlarmExplain);

    //根据pAlarm的内容填写相应的MSG_INTRA消息，并发送给网络模块
    UINT4 uDataLen = _MEMFRAME_HEAD_LENGTH + _MEM_ALARM_INFO_EXHEAD_LENGTH
                     + MAX_ALARM_PARA;
    MSG_INTRA* pAlarmMsg = new(uDataLen) MSG_INTRA;
    if (NULL == pAlarmMsg)
    {
        throw "Fail to new pAlarmMsg";
    }
    pAlarmMsg->SenderPid = PT_OM;
    pAlarmMsg->SenderMid = MT_ALARM;
    pAlarmMsg->ReceiverPid = m_yAPID;
    pAlarmMsg->ReceiverMid = MT_NETWORK;
    pAlarmMsg->CmdCode = CMD_BIN_ALARM;

    SMemFrame* pMemFrame = (SMemFrame* )(pAlarmMsg->Data);
    pMemFrame->nCmd(MEM_CMD_DATA);
    pMemFrame->nLen(_MEM_ALARM_INFO_EXHEAD_LENGTH + pAlarmExplain->yParaNum);

    SMemAlarmInfo* pMemAlarmInfo = (SMemAlarmInfo* )(pMemFrame->Data());
    pMemAlarmInfo->sender_mid(0);   //本域没有意义，填为0
    pMemAlarmInfo->sender_pid(APP_PID_BAU);
    pMemAlarmInfo->receiver_mid(APP_MID_BAM);
    pMemAlarmInfo->receiver_pid(APP_PID_ALARM);
    pMemAlarmInfo->nLen(_MEM_ALARM_INFO_EXPAND_LENGTH
                         + pAlarmExplain->yParaNum);
    pMemAlarmInfo->byBauNo(pAlarm->yCPUPos);
    pMemAlarmInfo->uAlarmID(GetOuterAlarmId(pAlarm));
    pMemAlarmInfo->byAllRestore(0); //本域没有意义，填为0
    if (pAlarmExplain->yParaNum > 0)
    {
        ASSERT(1 == pAlarmExplain->yParaNum);

        memcpy(pMemAlarmInfo->Paras(), pAlarm->yPara, MAX_ALARM_PARA);
    }

    CWorkModule::SendMsg(pAlarmMsg);
}
/**********************************************************************
功能描述    分析收到的主机的二进制告警应答消息，从中得到相应的告警信息
输入参数    pMsg——收到的二进制告警应答的消息
输出参数    无
返回值      从pMsg中分析出来的告警消息，调用者不得删除
抛出异常    无
**********************************************************************/
BOOL CMSCBinAlarmHandler::ParseAlarmResponse(MSG_INTRA* pMsg,SActiveAlarm *&pActive)
{
    static SActiveAlarm AlarmInfo;
        AlarmInfo.yPara[0] = 0xFF; //张玉新 2002－05－09
    //根据pMsg的Data域中的信息填写AlarmInfo
    SMemAlarmInfo* pMemAlarmInfo = (SMemAlarmInfo* )(pMsg->Data);

    SetInnerAlarmId(&AlarmInfo, pMemAlarmInfo->uAlarmID());

    SAlarmExplain* pAlarmExplain
         = m_pAlarmCfg->GetAlarmExplain(AlarmInfo.uAlarmID);
    if(NULL == pAlarmExplain)
        {
        pActive = NULL;
        return FALSE;
        }
    AlarmInfo.yCPUPos = pMemAlarmInfo->byBauNo();
    AlarmInfo.yModuleNO = pMemAlarmInfo->byBauNo();
    if (pAlarmExplain->yParaNum > 0)
    {
        memcpy(AlarmInfo.yPara, pMemAlarmInfo->Paras(),MAX_ALARM_PARA);
    }
    pActive = &AlarmInfo;
    return TRUE;
}