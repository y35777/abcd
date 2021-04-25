 #include "back_stor.h"
#include "save_chanl.h"
#include "hot_save_chanl.h"
#include "../include/perf_item.h"
#include "../include/format_out.h"
#include "../include/msg_def.h"
/*
#include "../wlan_cmcc_s/wlan_cmcc_s.h"
#include "../3g_cs_cmcc/CMCC_CS_BS_Patch.h"
#include "../ps_patch/ps_patch.h"
*/
#include "resource.h"
#include "../include/toolbox.h"


CBackStorage::CBackStorage(const UINT4 uAPID) : CWorkModule()
{
    m_uAPID = uAPID;

	//颜文远2002.04.03添加，问题单号D12398
    m_ChannelInfoArray = NULL;
	//颜文远2002.04.03添加结束
}

CBackStorage::~CBackStorage()
{

}

/**********************************************************************
功能描述    初始化函数
输入参数    无
输出参数    无
返回值      ERR_SUCCESS----初始化成功
            ERR_FAIL----初始化失败 
抛出异常    无
**********************************************************************/
int CBackStorage::InitInstance()
{
    int nResult = CWorkModule::InitInstance();
    if (0 != nResult)
    {
		TRACE(MTS_BACKSAVE, S_BS_TRC_FAIL_INIT_BASE);
        return ERR_FAIL;
    }

    BOOL bSucc;
    STRING szIGWBPath = STRING(GetAppPath()) + "/" 
                        + "config" + "/" + "format";
    CIGWB IGWB(szIGWBPath);
    IBillProcessor* pIBillProcessor = 
        (IBillProcessor* )IGWB.QueryInterface(IID_FP_IBILLPROCESSOR);
	if (NULL == pIBillProcessor)
    {
		TRACE(MTS_BACKSAVE, S_BS_TRC_FAIL_INIT_FMT);
        return ERR_FAIL;
    }
    nResult = pIBillProcessor->Init(m_uAPID); 
    if (0 != nResult)
    {
		TRACE(MTS_BACKSAVE, S_BS_TRC_FAIL_INIT_FMT);
        return ERR_FAIL;
    }
	
	//add by maimaoshi at 2002-12-12
	char szAPSection[25];
    sprintf(szAPSection, "%s%d", CFG_SEC_AP, m_uAPID);
    CINIFile cfgIniFile(GetCfgFilePath());
    bSucc = cfgIniFile.Open();
    if (!bSucc)
    {
        TRACE(MTS_BACKSAVE, S_BS_TRC_FAIL_OPEN_INI);
        return FALSE;
    }

	//读取本进程对应的接入点号
    m_uApType = cfgIniFile.GetInt(szAPSection, CFG_AP_KEY_APTYPE, CFG_AP_KEY_APTYPE_DEFAULT);

	//by ldf 2004-02-24 INI 的析构函数中有close部分.这里都去掉
    //cfgIniFile.Close();

    //获得通道数，必须包括缺省通道0
    LIST<STRING> ChannelNameList;
    m_uChannelCount = pIBillProcessor->GetChannelNames(ChannelNameList) + 1;  
    ChannelNameList.push_front(CHANNEL_0_NAME);

	//by ldf 2004-02-24 阿联酋分拣需求
    cfgIniFile.GetString(szAPSection, CFG_DISPATCH_TYPE, CFG_DISPATCH_DEFAULT,
                         m_szDispatchType, 49);
	m_szDispatchType[48] = '\0';
	m_uDispatchAllCDRCH = cfgIniFile.GetInt(szAPSection, CFG_DISPATCH_ALLCDR_CH, CFG_DISPATCH_ALLCDR_CH_DEFAULT);
	//如果设置的通道数值 比 (通道数-1) 大,那么调整为缺省通道0
	if(m_uDispatchAllCDRCH >= m_uChannelCount)
	{
		m_uDispatchAllCDRCH = 0;
	}
	//end

    if(m_PatchEncap.Init(m_uAPID) != ERR_SUCCESS)
    {
        return FALSE;
    }

    if(m_PatchEncap.GetbAviable())
    {
        BOOL bRet = m_PatchEncap.Init(GetCfgFilePath(), 
                                      m_uAPID,
                                      m_uChannelCount);
        if(!bRet)
        {
            return FALSE;
        }
    }

    BOOL bHotSaveSec;
    bHotSaveSec = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                    CFG_HOT_SAVE_SECOND, CFG_HOT_SAVE_SECOND_DEFAULT);
    bHotSaveSec = cfgIniFile.GetInt(szAPSection,
                                    CFG_HOT_SAVE_SECOND, bHotSaveSec);

    BOOL bProcBillCsn = cfgIniFile.GetInt(szAPSection, CFG_PROC_BILL_CSN, 
                                          CFG_PROC_BILL_CSN_DEFAULT);

    
    //初始化m_ChannelInfoArray
    m_ChannelInfoArray = new SChannelInfo[m_uChannelCount];
    int nChannelNo = 0;
    LIST<STRING>::iterator itr;

	//先将所有的pSaveChannel域设置为空，以免在退出时进行删除出现错误
    for (itr = ChannelNameList.begin(); itr != ChannelNameList.end(); itr++)
    {
        m_ChannelInfoArray[nChannelNo].pSaveChannel = NULL;
		nChannelNo++;
	}
	nChannelNo = 0;
    for (itr = ChannelNameList.begin(); itr != ChannelNameList.end(); itr++)
    {
        STRING szChannelName = *itr;

        SChannelInfo* pChannelInfo = &(m_ChannelInfoArray[nChannelNo]);
        if (!bHotSaveSec)
        {
            pChannelInfo->pSaveChannel = new CSaveChannel(&m_PatchEncap);
        }
        else
        {
            pChannelInfo->pSaveChannel = new CHotSaveChannel(&m_PatchEncap);
        }

        if(nChannelNo == 0)
        {
            int nActBillLen = pIBillProcessor->GetOrigLen();
            if((bProcBillCsn) && (nActBillLen != 0))
            {
                nActBillLen -= sizeof(UINT4);
            }
            bSucc = pChannelInfo->pSaveChannel->Init(m_uAPID, nChannelNo, szChannelName, 
                                                     nActBillLen);
        }
        else
        {
            bSucc = pChannelInfo->pSaveChannel->Init(m_uAPID, nChannelNo, szChannelName, 
                                                 pIBillProcessor->GetFinalLen(nChannelNo));
        }

        if (!bSucc)
        {
            return ERR_FAIL;
        }

        pChannelInfo->uLastPID = pChannelInfo->pSaveChannel->GetLastPID();

		nChannelNo++;
    }

    m_lSecondTimerID = SetTimer(1000);
    m_bError = FALSE;


    return ERR_SUCCESS;
}

/**********************************************************************
功能描述    模块退出时的清理函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBackStorage::ExitInstance()
{
    if (NULL != m_ChannelInfoArray)
    {
        for (int i = 0; i < m_uChannelCount; i++)
        {
            if (NULL != m_ChannelInfoArray[i].pSaveChannel)
            {
				m_ChannelInfoArray[i].pSaveChannel->CloseCurFile();
                delete m_ChannelInfoArray[i].pSaveChannel;
            }
        }

        delete[] m_ChannelInfoArray;
    }

    if(m_PatchEncap.GetbAviable())
    {
        m_PatchEncap.Terminate();
    }

    CWorkModule::ExitInstance();
}

/**********************************************************************
功能描述    定时器事件处理函数
输入参数    nTimerID----定时器的ID号
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBackStorage::OnTimer(const long nTimerID)
{
    if (nTimerID == m_lSecondTimerID)
    {
		//每10秒钟MSGOUT一次模块的信息
		static int nTimeCount = 0;
		nTimeCount++;
		if (nTimeCount >= 10)
		{
			nTimeCount = 0;

			if (m_bError)
			{
				MSGOUT(MTS_BACKSAVE, MSGOUT_LEVEL_URGENT, 
						S_BS_MSG_ERR_STATUS);
			}
			else
			{
				for (int i = 0; i < m_uChannelCount; i++)
				{
					
					MSGOUT(MTS_BACKSAVE, MSGOUT_LEVEL_IMPORTANT, 
						S_BS_MSG_NORMAL_STATUS, 
						m_ChannelInfoArray[i].pSaveChannel->GetChlName(), 
						m_ChannelInfoArray[i].pSaveChannel->GetCurCsn(TRUE));
				}
			}
		}
       for (int i = 0; i < m_uChannelCount; i++)
       {
					
			BOOL bSucc = m_ChannelInfoArray[i].pSaveChannel->Timer1Sec();		   
		   
			if (!bSucc)
			{
                m_bError = TRUE;

				MSGOUT(MTS_BACKSAVE, MSGOUT_LEVEL_URGENT, S_BS_MSG_FAIL_TIMER);
				TRACE(MTS_BACKSAVE, S_BS_TRC_FAIL_TIMER);

                STRING sFileName;
                m_ChannelInfoArray[i].pSaveChannel->GetCurrentFileName(sFileName);
                int nPos = 0;
#ifdef _PLATFORM_WIN32
                //将所有的'\'转化为'/'                
                while(nPos >= 0)
                {
                    nPos = sFileName.find_first_of('\\');
                    if(nPos >= 0)
                    {
                        sFileName[nPos] = '/'; 
                    }
                }
#endif
                nPos = 0;
                //对于超长的文件名截掉前面的文件路径信息
                while(nPos >= 0 && sFileName.length() >= MAX_ALARM_PARA - 1)
                {
                    nPos = sFileName.find_first_of('/');                  
                    sFileName = sFileName.substr(nPos + 1);             
                }
                //发送"保存原始话单失败"告警并请求倒换
                SInnerAlarm Alarm;
                Alarm.yAlarmType = AT_EVENT;
                Alarm.uAlarmID = ALARM_ID_WRITE_FILE_FAILED;
                Alarm.yAlarmPara[0] = ALARM_PARA_FINAL_BILL;
                strncpy((char *)&Alarm.yAlarmPara[1],  sFileName.c_str(), MAX_ALARM_PARA - 1);
                Alarm.yAlarmPara[MAX_ALARM_PARA - 1] = '\0';
                SendAlarm(&Alarm);
                
                AskForFailOver(GetModuleType(), ERROR_WRITEFILE);
			}
        }
	}
    else
    {
        CWorkModule::OnTimer(nTimerID);
    }
}

//获取模块类型
MODULE_TYPE CBackStorage::GetModuleType()
{
    return MT_BACKSAVE;
}

/**********************************************************************
功能描述    实现消息分发及相关处理
输入参数    pMsgParsed----待分发处理的消息帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBackStorage::ParseMsg(MSG_INTRA* pMsgParsed)
{
    switch (pMsgParsed->CmdCode)
    {
        case CMD_SAVE_END_BILL:
            ProcSavePacketMsg(pMsgParsed);
            break;
        case CMD_RESUME:
            ProcResumeMsg(pMsgParsed);
            break;
        case CMD_ANS_AUXUPGRADE:
            ProcUpdateOverMsg(pMsgParsed);
            break;
        case MML_PRODUCE_CDR:
            ProduceCDR(pMsgParsed);
            break;            
        default:
            CWorkModule::ParseMsg(pMsgParsed);
            break;
    }

}
/**********************************************************************
功能描述    立即生成话单文件
输入参数    pMessage----"立即产生话单"消息
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBackStorage::ProduceCDR(MSG_INTRA* pMessage)
{
    BOOL nErrCode = FALSE;
    
    if (NULL != m_ChannelInfoArray)
    {
        for (int nChannelNo = 0; nChannelNo < m_uChannelCount; nChannelNo++)
        {
            if (NULL != m_ChannelInfoArray[nChannelNo].pSaveChannel)
            {
                m_ChannelInfoArray[nChannelNo].pSaveChannel->CloseCurFile();   
            }
        }       
    }   
}

/**********************************************************************
功能描述    处理"请求处理最终话单包"消息
输入参数    pMessage----"请求处理最终话单包"消息
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBackStorage::ProcSavePacketMsg(MSG_INTRA* pMessage)
{
    SBackPacketInfo* pPacketInfo = (SBackPacketInfo* )pMessage->Data;
    UINT4 uChannelID = pPacketInfo->yChannelID;
    SChannelInfo* pChannelInfo = &m_ChannelInfoArray[uChannelID];
	
    if (!m_bError)
    {
        //如果话单包号不大于当前包号，则不进行任何操作，直接丢弃该话单包
        if (pPacketInfo->uPID > pChannelInfo->uLastPID)  
        {
            //如果发生PID的突变，则记录下相应的信息
            if (pPacketInfo->uPID > pChannelInfo->uLastPID + 1)
            {
                TRACE(MTS_BACKSAVE, S_BS_TRC_REC_JUNP_PID, 
                      uChannelID, pChannelInfo->uLastPID, pPacketInfo->uPID);
                MSGOUT(MTS_BACKSAVE, MSGOUT_LEVEL_URGENT, 
                       S_BS_MSG_REC_JUNP_PID, uChannelID, 
                       pChannelInfo->uLastPID, pPacketInfo->uPID);
            }

            CSaveChannel* pSaveChannel = pChannelInfo->pSaveChannel;
            pChannelInfo->uLastPID = pPacketInfo->uPID;

            UINT4 uWriteTime = 0;         
            INT4 nResult 
                = pSaveChannel->WritePacket((BYTE* )pMessage->Data, uWriteTime);

			//by ldf 2004-02-18 阿联酋需求
			if( (strcmp(m_szDispatchType,"UAE") == 0) && (uChannelID != m_uDispatchAllCDRCH) )
			{
				pChannelInfo = &m_ChannelInfoArray[m_uDispatchAllCDRCH];
				pSaveChannel = pChannelInfo->pSaveChannel;
				INT4 nResult 
                    = pSaveChannel->WritePacket((BYTE* )pMessage->Data, uWriteTime);				
			}
			//end
			
            if (0 == nResult)
            {
                //发送同步数据
                SSaveSyncData SyncData;
                SyncData.uPID = pChannelInfo->uLastPID;
                SyncData.uFileCsn = pSaveChannel->GetCurCsn();
                SyncData.uBillCsn = pSaveChannel->GetBillCsn();
                WriteSyncData(MT_BACKSAVE, &SyncData, 
                              sizeof(SSaveSyncData), uChannelID);
            }
            else
            {
			    TraceModuleErr(THISMODULE, nResult);
                m_bError = TRUE;

				MSGOUT(MTS_BACKSAVE, MSGOUT_LEVEL_URGENT, S_BS_MSG_FAIL_SAVE, 
						 pSaveChannel->GetChlName(), pChannelInfo->uLastPID, 
						 nResult);
				TRACE(MTS_BACKSAVE, S_BS_TRC_FAIL_SAVE, 
						pSaveChannel->GetChlName(), pChannelInfo->uLastPID, 
						nResult);

                STRING sFileName;
                pChannelInfo->pSaveChannel->GetCurrentFileName(sFileName);
                int nPos = 0;
#ifdef _PLATFORM_WIN32
                //将所有的'\'转化为'/'
                
                while(nPos >= 0)
                {
                    nPos = sFileName.find_first_of('\\');
                    if(nPos >= 0)
                    {
                        sFileName[nPos] = '/'; 
                    }
                }
#endif
                
                //对于超长的文件名截掉前面的文件路径信息
                nPos = 0;
                while(nPos >= 0 && sFileName.length() >= MAX_ALARM_PARA - 1)
                {
                    nPos = sFileName.find_first_of('/');                  
                    sFileName = sFileName.substr(nPos + 1);             
                }
                //发送"保存原始话单失败"告警并请求倒换
                SInnerAlarm Alarm;
                Alarm.yAlarmType = AT_EVENT;
                Alarm.uAlarmID = ALARM_ID_WRITE_FILE_FAILED;
                Alarm.yAlarmPara[0] = ALARM_PARA_FINAL_BILL;
                strncpy((char *)&Alarm.yAlarmPara[1],  sFileName.c_str(), MAX_ALARM_PARA - 1);
                Alarm.yAlarmPara[MAX_ALARM_PARA - 1] = '\0';
                SendAlarm(&Alarm);
                
                AskForFailOver(GetModuleType(), ERROR_WRITEFILE);
            }
        }
    }

    //后存盘模块负责删除话单包中的话单数据
    delete[] pPacketInfo->pPacket;
}

/**********************************************************************
功能描述    处理"请求启动恢复"消息
输入参数    pMessage----"请求启动恢复"消息
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBackStorage::ProcResumeMsg(MSG_INTRA* pMessage)
{
    MSG_INTRA* pMsg = new(sizeof(SInformStatus)) MSG_INTRA;
    pMsg->CmdCode = CMD_INFORM_STATUS;
    pMsg->SenderPid = PT_SELF;
    pMsg->SenderMid = MT_BACKSAVE;
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = MT_BILLPROCESSOR;

    //填写要通知的消息
    SInformStatus* pStatus = (SInformStatus* )pMsg->Data;
    UINT4 uLastPID, uLastBillCsn, uMinLastPID = 0xFFFFFFFF;
    for (int nChannelNo = 0; nChannelNo < m_uChannelCount; nChannelNo++)
    {
        CSaveChannel* pChannel = m_ChannelInfoArray[nChannelNo].pSaveChannel;
        pChannel->GetResumeParam(uLastPID, uLastBillCsn);
        if (uMinLastPID > uLastPID)
        {
            uMinLastPID = uLastPID;
        }
        pStatus->uaBillCsns[nChannelNo] = uLastBillCsn;
    }
    pStatus->uLastPID = uMinLastPID;

    SendMsg(pMsg);
}

/**********************************************************************
功能描述    处理"通知辅助升级完成"消息
输入参数    pMessage----"通知辅助升级完成"消息
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBackStorage::ProcUpdateOverMsg(MSG_INTRA* pMessage)
{
    MSG_INTRA* pMsg = new(0) MSG_INTRA;
    pMsg->CmdCode = CMD_ANS_AUXUPGRADE;
    pMsg->SenderPid = PT_SELF;
    pMsg->SenderMid = MT_BACKSAVE;
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = MT_MANAGER;

    SendMsg(pMsg);

    //辅助升级完毕, 则关闭当前文件
    if (NULL != m_ChannelInfoArray)
    {
        for (int i = 0; i < m_uChannelCount; i++)
        {
            if (NULL != m_ChannelInfoArray[i].pSaveChannel)
            {
                m_ChannelInfoArray[i].pSaveChannel->CloseCurFile();
            }
        }
    }
}
