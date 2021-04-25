#include "front_stor.h"
#include "../include/frame.h"
#include "../include/perf_item.h"
#include "../include/msg_def.h"
#include "resource.h"

/**********************************************************************
��������    ���캯��
�������    uAPID----�����ID�� 
            pPacketCache----����������
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
��������    ��ʼ������
�������    ��
�������    ��
����ֵ      0----��ʼ���ɹ� 
            ��0----��ʼ��ʧ��
�׳��쳣    ��
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

//ģ���˳�ʱ��������
void CFrontStorage::ExitInstance()  
{
    m_Save.CloseCurFile();

    KillTimer(m_lSecondTimerID);
    CWorkModule::ExitInstance();
}

/**********************************************************************
��������    ��ʱ���¼�������
�������    uTimerID----��ʱ����ID��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CFrontStorage::OnTimer(const long nTimerID)
{
    if (nTimerID == m_lSecondTimerID)
    {
		//ÿ10����MSGOUTһ��ģ�����Ϣ
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
            //�����е�'\'ת��Ϊ'/'
            
            while(nPos >= 0)
            {
                nPos = sFileName.find_first_of('\\');
                if(nPos >= 0)
                {
                    sFileName[nPos] = '/'; 
                }
            }
#endif            
            //���ڳ������ļ����ص�ǰ����ļ�·����Ϣ
            nPos = 0;
            while(nPos >= 0 && sFileName.length() >= MAX_ALARM_PARA - 1)
            {
                nPos = sFileName.find_first_of('/');                  
                sFileName = sFileName.substr(nPos + 1);             
            }
            //����"����ԭʼ����ʧ��"�澯�����󵹻�
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

//��ȡģ������
MODULE_TYPE CFrontStorage::GetModuleType()
{
    return MT_FRONTSAVE;
}

/**********************************************************************
��������    ʵ����Ϣ�ַ�����ش���
�������    pMsgParsed----���ַ��������Ϣ֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CFrontStorage::ParseMsg(MSG_INTRA* pMsgParsed)
{
    switch (pMsgParsed->CmdCode)
    {
        case CMD_SAVE_PACKET:   //"���󱣴�ԭʼ������"��Ϣ
            ProcSavePacketMsg(pMsgParsed);
            break;
        case CMD_RESUME:       //"���������ָ�"��Ϣ
            ProcResumeMsg(pMsgParsed);
            break;
        case CMD_INFORM_STATUS:    //"֪ͨ״̬"��Ϣ
            ProcInformStatusMsg(pMsgParsed);
            break;
        default:
            CWorkModule::ParseMsg(pMsgParsed);
            break;
    }
}

/**********************************************************************
��������    ����"���󱣴滰����"����֡
�������    pMessage----�����������֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CFrontStorage::ProcSavePacketMsg(MSG_INTRA* pMessage)
{
    if (!m_bError)
    {
        SFrontPacketInfo* pPacketInfo = (SFrontPacketInfo* )pMessage->Data;

        //��Ϣ�д��İ����뵱ǰ���Ų�����
        if (pPacketInfo->uPID != m_uLastPID + 1)
        //��Ϣ�д��İ����뵱ǰ���Ų�����������ϢҪ������ģ���������
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
        else      //���򣬱��滰����
        {
            m_uLastPID = pPacketInfo->uPID;
            m_pPacket = pPacketInfo->pPacket;
            m_uPacketLen = pPacketInfo->uPacketLen;

            ProcessPacket((BYTE* )pPacketInfo);
        }
    }
}

/**********************************************************************
��������    ����"���������ָ�"����֡
�������    pMessage----�����������֡
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CFrontStorage::ProcResumeMsg(MSG_INTRA* pMessage)
{
    //�򻰵�����ģ��ת��"���������ָ�"��Ϣ
    MSG_INTRA* pMsg = new(0) MSG_INTRA;
    pMsg->CmdCode = CMD_RESUME;
    pMsg->SenderPid = PT_SELF;
    pMsg->SenderMid = MT_FRONTSAVE;
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = MT_BILLPROCESSOR;

    SendMsg(pMsg);
}

/**********************************************************************
��������    ����"֪ͨ״̬��Ϣ"����֡
�������    pMessage----�����������֡
�������    ��
����ֵ      ��
�׳��쳣    ��
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

    //�յ���Ϣ�󣬽�ǰ����������û�б�����̱���Ļ����������·���һ��
	if (uLastPID > 1)
	{
		//��ȡ��uResumePID��uLastPID��Χ�ڵĵ�һ����������صĲ���
		INT4 nResult = m_Save.GetFirstBillPacket(uResumePID, uLastPID, uPID, 
												 pPacket, uPacketLen, 
												 szFileFullPath, uStartPos);
		if (ERR_SUCCESS != nResult)
		{
			TraceModuleErr(THISMODULE, nResult);
		}

		SPacketCacheEle* pCacheEle = NULL;
		//��ȡ��uResumePID��uLastPID��Χ�ڵ��������໰��������صĲ�����
		//�������ж����Ļ��������͸���������ģ����д���
		while (ERR_ALREADY_FINISH != nResult)
		{
			//�����ȡ���������ݳɹ����򽫸û������ݷ��͸������ģ��
			if (ERR_SUCCESS == nResult)
			{
				pCacheEle = new SPacketCacheEle;
				pCacheEle->uPacketID = uPID;
				pCacheEle->uPacketLen = uPacketLen;
				pCacheEle->pPacket = pPacket;
				strncpy(pCacheEle->szFilePath, szFileFullPath.c_str(), MAX_PATH);
                pCacheEle->szFilePath[MAX_PATH - 1] = 0;
				pCacheEle->uStartPos = uStartPos;

				m_pPacketCache->PutPacket(pCacheEle);//��ȡ���Ļ��������뻺����

				//�򻰵�����ģ�鷢��"����Ʒ�Ԥ����"��Ϣ
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
        
			//�����һ������
			nResult = m_Save.GetNextBillPacket(uPID, uPID, pPacket, uPacketLen, 
											   szFileFullPath, uStartPos);
			if ((ERR_SUCCESS != nResult) && (ERR_ALREADY_FINISH != nResult))
			{
			    TraceModuleErr(THISMODULE, nResult);
			}
		}
	}

    //�����ָ���ɺ�������ģ�鷢��"�����ָ����"��Ϣ
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
��������    ���������̴���
�������    pPacketInfo----�������������Ϣ���ں����⸺��������ͷ�
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CFrontStorage::ProcessPacket(BYTE* pPacketInfo)
{
    UINT4 uWriteTime;

	UINT4 uPID = ((SFrontPacketInfo* )pPacketInfo)->uPID;
    MSGOUT(MTS_FRONTSAVE, MSGOUT_LEVEL_SIDLY, S_FS_MSG_REC_PACKET, uPID);

    //����д����������
    INT4 nResult = m_Save.WritePacket(pPacketInfo, uWriteTime);
	
    if (0 == nResult)     //����ɹ�
    {
        //����״̬ͬ��
        SSaveSyncData SyncData;
        SyncData.uPID = m_uLastPID;
        SyncData.uFileCsn = m_Save.GetCurCsn();
        WriteSyncData(GetModuleType(), &SyncData, sizeof(SSaveSyncData), 255);

        //������ģ�鷢�ͳɹ������Ӧ��
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
        
        //�򻰵�����ģ�鷢��"����Ʒ�Ԥ����"��Ϣ
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
        //�����е�'\'ת��Ϊ'/'        
        while(nPos >= 0)
        {
            nPos = sFileName.find_first_of('\\');
            if(nPos >= 0)
            {
                sFileName[nPos] = '/'; 
            }
        }
#endif
        
        //���ڳ������ļ����ص�ǰ����ļ�·����Ϣ
        nPos = 0;
        while(nPos >= 0 && sFileName.length() >= MAX_ALARM_PARA - 1)
        {
            nPos = sFileName.find_first_of('/');                  
            sFileName = sFileName.substr(nPos + 1);             
        }
        //����"����ԭʼ����ʧ��"�澯�����󵹻�
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
