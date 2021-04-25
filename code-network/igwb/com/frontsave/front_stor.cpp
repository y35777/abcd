#include "front_stor.h"
#include "../include/frame.h"
#include "../include/perf_item.h"
#include "../include/msg_def.h"
#include "resource.h"

/**********************************************************************
功能描述    构造函数
输入参数    uAPID----接入点ID号 
            pPacketCache----话单缓冲区
**********************************************************************/
CFrontStorage::CFrontStorage(UINT4 uAPID, CPacketCache* pPacketCache)
                    : CWorkModule()
{
    m_uAPID = uAPID;
    m_pPacketCache = pPacketCache;
}

CFrontStorage::~CFrontStorage()
{

}

/**********************************************************************
功能描述    初始化函数
输入参数    无
输出参数    无
返回值      0----初始化成功 
            非0----初始化失败
抛出异常    无
**********************************************************************/
int CFrontStorage::InitInstance()
{
    int nResult = CWorkModule::InitInstance();
    if (0 != nResult)
    {
		TRACE(MTS_FRONTSAVE, S_FS_TRC_FAIL_INIT_BASE);
        return ERR_FAIL;
    }
    BOOL bSucc = m_Save.Init(m_uAPID);
    if (!bSucc)
    {
        return ERR_FAIL;
    }

    m_uLastPID = m_Save.GetLastPID();

    m_bError = FALSE;
    m_lSecondTimerID = SetTimer(1000);

    return ERR_SUCCESS;
}

//模块退出时的清理函数
void CFrontStorage::ExitInstance()  
{
    m_Save.CloseCurFile();

    KillTimer(m_lSecondTimerID);
    CWorkModule::ExitInstance();
}

/**********************************************************************
功能描述    定时器事件处理函数
输入参数    uTimerID----定时器的ID号
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CFrontStorage::OnTimer(const long nTimerID)
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
				MSGOUT(MTS_FRONTSAVE, MSGOUT_LEVEL_URGENT, S_FS_MSG_ERR_STATUS);
			}
			else
			{
				MSGOUT(MTS_FRONTSAVE, MSGOUT_LEVEL_IMPORTANT, 
						S_FS_MSG_NORMAL_STATUS, m_Save.GetCurCsn(TRUE));
			}
		}

		BOOL bSucc = m_Save.Timer1Sec();
		
		if (!bSucc)
		{
			m_bError = TRUE;

			MSGOUT(MTS_FRONTSAVE, MSGOUT_LEVEL_URGENT, S_FS_MSG_FAIL_TIMER);
			TRACE(MTS_FRONTSAVE, S_FS_TRC_FAIL_TIMER);

            STRING sFileName;
            m_Save.GetCurrentFileName(sFileName);   
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
            Alarm.yAlarmPara[0] = ALARM_PARA_ORIG_BILL;
            strncpy((char *)&Alarm.yAlarmPara[1],  sFileName.c_str(), MAX_ALARM_PARA - 1);
            Alarm.yAlarmPara[MAX_ALARM_PARA - 1] = '\0';
            SendAlarm(&Alarm);
            
			AskForFailOver(GetModuleType(), ERROR_WRITEFILE);
		}
    }
    else
    {
        CWorkModule::OnTimer(nTimerID);
    }
}

//获取模块类型
MODULE_TYPE CFrontStorage::GetModuleType()
{
    return MT_FRONTSAVE;
}

/**********************************************************************
功能描述    实现消息分发及相关处理
输入参数    pMsgParsed----待分发处理的消息帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CFrontStorage::ParseMsg(MSG_INTRA* pMsgParsed)
{
    switch (pMsgParsed->CmdCode)
    {
        case CMD_SAVE_PACKET:   //"请求保存原始话单包"消息
            ProcSavePacketMsg(pMsgParsed);
            break;
        case CMD_RESUME:       //"请求启动恢复"消息
            ProcResumeMsg(pMsgParsed);
            break;
        case CMD_INFORM_STATUS:    //"通知状态"消息
            ProcInformStatusMsg(pMsgParsed);
            break;
        default:
            CWorkModule::ParseMsg(pMsgParsed);
            break;
    }
}

/**********************************************************************
功能描述    处理"请求保存话单包"命令帧
输入参数    pMessage----待处理的命令帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CFrontStorage::ProcSavePacketMsg(MSG_INTRA* pMessage)
{
    if (!m_bError)
    {
        SFrontPacketInfo* pPacketInfo = (SFrontPacketInfo* )pMessage->Data;

        //消息中带的包号与当前包号不连续
        if (pPacketInfo->uPID != m_uLastPID + 1)
        //消息中带的包号与当前包号不连续，则发消息要求网络模块调整包号
        {
            MSG_INTRA* pMsg = new(sizeof(UINT4)) MSG_INTRA;
            pMsg->CmdCode = CMD_SYNC_PACKETID;
            pMsg->SenderPid = PT_SELF;
            pMsg->SenderMid = MT_FRONTSAVE;
            pMsg->ReceiverPid = PT_SELF;
            pMsg->ReceiverMid = MT_NETWORK;
            UINT4* puPID = (UINT4* )(pMsg->Data);
            *puPID = m_uLastPID;

            SendMsg(pMsg);
        }
        else      //否则，保存话单包
        {
            m_uLastPID = pPacketInfo->uPID;
            m_pPacket = pPacketInfo->pPacket;
            m_uPacketLen = pPacketInfo->uPacketLen;

            ProcessPacket((BYTE* )pPacketInfo);
        }
    }
}

/**********************************************************************
功能描述    处理"请求启动恢复"命令帧
输入参数    pMessage----待处理的命令帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CFrontStorage::ProcResumeMsg(MSG_INTRA* pMessage)
{
    //向话单处理模块转发"请求启动恢复"消息
    MSG_INTRA* pMsg = new(0) MSG_INTRA;
    pMsg->CmdCode = CMD_RESUME;
    pMsg->SenderPid = PT_SELF;
    pMsg->SenderMid = MT_FRONTSAVE;
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = MT_BILLPROCESSOR;

    SendMsg(pMsg);
}

/**********************************************************************
功能描述    处理"通知状态消息"命令帧
输入参数    pMessage----待处理的命令帧
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CFrontStorage::ProcInformStatusMsg(MSG_INTRA* pMessage)
{
    MSG_INTRA* pMsg = NULL;
    UINT4 uResumePID = *(UINT4* )(pMessage->Data);
    UINT4 uLastPID = m_Save.GetLastPID();
    TRACE(MTS_FRONTSAVE, S_FS_TRC_BEGIN_RECOVER, uResumePID, uLastPID);
    MSGOUT(MTS_FRONTSAVE, MSGOUT_LEVEL_IMPORTANT, 
			S_FS_TRC_BEGIN_RECOVER, uResumePID, uLastPID);

    UINT4 uPID, uPacketLen, uStartPos;
    BYTE* pPacket = NULL;
    STRING szFileFullPath;

    //收到消息后，将前存盘中所有没有被后存盘保存的话单包都重新发送一遍
	if (uLastPID > 1)
	{
		//获取从uResumePID到uLastPID范围内的第一包话单和相关的参数
		INT4 nResult = m_Save.GetFirstBillPacket(uResumePID, uLastPID, uPID, 
												 pPacket, uPacketLen, 
												 szFileFullPath, uStartPos);
		if (ERR_SUCCESS != nResult)
		{
			TraceModuleErr(THISMODULE, nResult);
		}

		SPacketCacheEle* pCacheEle = NULL;
		//获取从uResumePID到uLastPID范围内的所有其余话单包和相关的参数，
		//并将所有读出的话单包发送给话单处理模块进行处理
		while (ERR_ALREADY_FINISH != nResult)
		{
			//如果读取话单包数据成功，则将该话单数据发送给后面的模块
			if (ERR_SUCCESS == nResult)
			{
				pCacheEle = new SPacketCacheEle;
				pCacheEle->uPacketID = uPID;
				pCacheEle->uPacketLen = uPacketLen;
				pCacheEle->pPacket = pPacket;
				strncpy(pCacheEle->szFilePath, szFileFullPath.c_str(), MAX_PATH);
                pCacheEle->szFilePath[MAX_PATH - 1] = 0;
				pCacheEle->uStartPos = uStartPos;

				m_pPacketCache->PutPacket(pCacheEle);//将取出的话单包放入缓冲区

				//向话单处理模块发送"请求计费预处理"消息
				pMsg = new(sizeof(UINT4)) MSG_INTRA;
				pMsg->CmdCode = CMD_BILL_PROCESS;
				pMsg->SenderPid = PT_SELF;
				pMsg->SenderMid = MT_FRONTSAVE;
				pMsg->ReceiverPid = PT_SELF;
				pMsg->ReceiverMid = MT_BILLPROCESSOR;
				UINT4* puPID = (UINT4* )(pMsg->Data);
				*puPID = uPID;

				SendMsg(pMsg);
			}
        
			//获得下一包话单
			nResult = m_Save.GetNextBillPacket(uPID, uPID, pPacket, uPacketLen, 
											   szFileFullPath, uStartPos);
			if ((ERR_SUCCESS != nResult) && (ERR_ALREADY_FINISH != nResult))
			{
			    TraceModuleErr(THISMODULE, nResult);
			}
		}
	}

    //启动恢复完成后，向网络模块发送"启动恢复完成"消息
    pMsg = new(sizeof(UINT4)) MSG_INTRA;
    pMsg->CmdCode = CMD_RESUME_OVER;
    pMsg->SenderPid = PT_SELF;
    pMsg->SenderMid = MT_FRONTSAVE;
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = MT_NETWORK;
    UINT4* puPID = (UINT4* )(pMsg->Data);
    *puPID = uLastPID;

    SendMsg(pMsg);

    TRACE(MTS_FRONTSAVE, S_FS_TRC_END_RECOVER);
    MSGOUT(MTS_FRONTSAVE, MSGOUT_LEVEL_IMPORTANT, S_FS_TRC_END_RECOVER);
}

/**********************************************************************
功能描述    话单包存盘处理
输入参数    pPacketInfo----话单包的相关信息，在函数外负责申请和释放
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CFrontStorage::ProcessPacket(BYTE* pPacketInfo)
{
    UINT4 uWriteTime;

	UINT4 uPID = ((SFrontPacketInfo* )pPacketInfo)->uPID;
    MSGOUT(MTS_FRONTSAVE, MSGOUT_LEVEL_SIDLY, S_FS_MSG_REC_PACKET, uPID);

    //进行写话单包操作
    INT4 nResult = m_Save.WritePacket(pPacketInfo, uWriteTime);
	
    if (0 == nResult)     //保存成功
    {
        //进行状态同步
        SSaveSyncData SyncData;
        SyncData.uPID = m_uLastPID;
        SyncData.uFileCsn = m_Save.GetCurCsn();
        WriteSyncData(GetModuleType(), &SyncData, sizeof(SSaveSyncData), 255);

        //向网络模块发送成功保存的应答
        MSG_INTRA* pMsg = new(sizeof(UINT4)) MSG_INTRA;
        pMsg->CmdCode = CMD_SAVE_SUCCEED;
        pMsg->SenderPid = PT_SELF;
        pMsg->SenderMid = MT_FRONTSAVE;
        pMsg->ReceiverPid = PT_SELF;
        pMsg->ReceiverMid = MT_NETWORK;
        UINT4* puPID = (UINT4* )(pMsg->Data);
        *puPID = m_uLastPID;

        SendMsg(pMsg);        

        STRING szFileFullPath;
        UINT4 uStartPos;
        m_Save.GetCurPacketParam(szFileFullPath, uStartPos);
        SPacketCacheEle* pCacheEle = new SPacketCacheEle;
        pCacheEle->uPacketID = m_uLastPID;
        pCacheEle->uPacketLen = m_uPacketLen;
        pCacheEle->pPacket = m_pPacket;
        strncpy(pCacheEle->szFilePath, szFileFullPath.c_str(), MAX_PATH); 
        pCacheEle->szFilePath[MAX_PATH - 1] = 0;
        pCacheEle->uStartPos = uStartPos;

        m_pPacketCache->PutPacket(pCacheEle);
        
        //向话单处理模块发送"请求计费预处理"消息
        pMsg = new(sizeof(UINT4)) MSG_INTRA;
        pMsg->CmdCode = CMD_BILL_PROCESS;
        pMsg->SenderPid = PT_SELF;
        pMsg->SenderMid = MT_FRONTSAVE;
        pMsg->ReceiverPid = PT_SELF;
        pMsg->ReceiverMid = MT_BILLPROCESSOR;
        puPID = (UINT4* )(pMsg->Data);
        *puPID = m_uLastPID;

        SendMsg(pMsg);
    }
    else
    {
        m_bError = TRUE;
		TraceModuleErr(THISMODULE, nResult);

		MSGOUT(MTS_FRONTSAVE, MSGOUT_LEVEL_URGENT, 
				S_FS_MSG_FAIL_SAVE, m_uLastPID, nResult);
		TRACE(MTS_FRONTSAVE, S_FS_TRC_FAIL_SAVE, m_uLastPID, nResult);

        STRING sFileName;
        m_Save.GetCurrentFileName(sFileName);
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
        Alarm.yAlarmPara[0] = ALARM_PARA_ORIG_BILL;
        strncpy((char *)&Alarm.yAlarmPara[1],  sFileName.c_str(), MAX_ALARM_PARA - 1);
        Alarm.yAlarmPara[MAX_ALARM_PARA - 1] = '\0';
        SendAlarm(&Alarm);
        
        AskForFailOver(GetModuleType(), ERROR_WRITEFILE);
    }
}
