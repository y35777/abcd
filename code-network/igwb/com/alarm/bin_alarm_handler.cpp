 //���ļ�������Զ2002.04.17ʵ�� 

/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
#define THISMODULE "ALARM"
/****** End For PC-LINT ******/
#include "../toolbase/ini_file.h" //������ 2002-05-09
#include "../utility/config.h"
#include "../include/base_type.h"
#include "../alarm_util/alarm_config.h"
#include "../alarm_util/alarm_struct.h"
#include "bin_alarm_handler.h"
#include "../include/comm_struct_out.h"
//���캯��
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

    //���������ļ������Ƿ������н����ĸ澯
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
��������    ��ʼ������
�������    szCfgPath������Ӧ�������ļ���·��
�������    ��
����ֵ      0��ʾ�ɹ�����0��ʾʧ��
�׳��쳣    ����ռ�ʧ���׳��쳣
**********************************************************************/
/*int CBinAlarmHandler::Init(const char* szCfgPath)
{
    if(CAlarmHandler::Init(szCfgPath) != ERR_SUCCESS)
        return ERR_FAIL;
    m_pAlarmCfg->AlarmVisit(CAlarmCfgVisitor(this));
    return ERR_SUCCESS;
}*/
/**********************************************************************
��������    ���������ģ�鷢�͹����ķ�MML��Ϣ
�������    pFrame�������յ����ڲ���Ϣ
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBinAlarmHandler::HandleFrameFromApp(MSG_INTRA *pFrame)
{
    //by ldf 2004-06-15 ��Ӧ���ⵥSWPD05582
    if (pFrame->CmdCode == CMD_MODULE_ACTIVE) 
    {
        return;
    }
    //end

    if (CMD_ALARM_RESPONSE == pFrame->CmdCode)  //�澯Ӧ��
    {
        if ((m_yAPID == pFrame->SenderPid) || m_bProcAllAPAlarm)
        {
            AlarmReceived(pFrame);
        }
    }
    else    //�ڲ��澯
    {
        ASSERT(CMD_INNER_ALARM == pFrame->CmdCode);

                SInnerAlarm* pInnerAlarm = (SInnerAlarm* )(pFrame->Data);
        SActiveAlarm* pActiveAlarm = NULL;
        //ֻ���ʹ����½��̹����ĸ澯��˫�����̡�KERNEL���̡�OM���̡�
        //�������Ӧ�Ľ�������(�������þ����Ƿ������еĽ�������)
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

            AddAlarm(pInnerAlarm, &pActiveAlarm);
        }

    }
}

/*void CBinAlarmHandler::HandleFrameFromMML(MSG_INTRA *pFrame)
{
    //���MML��������
    CParseResult *pParaResult = *(CParseResult **)pFrame->Data;
    
    //����MML�������Ϣ
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
��������    ����澯�����м���һ���µĸ澯
�������    pNewAlarm������������¸澯
�������    ppActiveAlarm�����ڻ�澯�����������ӵĻ�澯
����ֵ      �����¸澯�Ľ��
�׳��쳣    ��
**********************************************************************/
ALARM_ADD_RESULT CBinAlarmHandler::AddAlarm(SInnerAlarm* pNewAlarm, SActiveAlarm** ppActiveAlarm, int nRecoverType)
{
    ASSERT(pNewAlarm->uAlarmID < 128);

    ALARM_ADD_RESULT nResult = ADD_OK;

    //���Ҫ���͵ĸ澯�����Σ��򲻷���
    if ('0' != m_szAlarmShield[pNewAlarm->uAlarmID])        
    {
        nResult = CAlarmHandler::AddAlarm(pNewAlarm, ppActiveAlarm, nRecoverType);
    }

    return nResult;
}
/**********************************************************************
��������    ��SActiveAlarm�ṹ�л�ȡ���ڷ��������ĸ澯ID
�������    pAlarm����ָ����SActiveAlarm�ṹָ��
�������    ��
����ֵ      ���ڷ��������ĸ澯ID
�׳��쳣    ��
��ע        �ڸ澯ģ���ڲ������ϸ澯�Լ���Ӧ�Ļָ��澯�ĸ澯ID��ͬ��
            �����ڷ��������ĸ澯��Ϣ�У��ָ��澯��IDΪ��Ӧ�Ĺ��ϸ澯��ID��1��
            ���ҷ��������ĸ澯ID���ڲ��澯ID�����һ���澯ID����
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
��������    ��������Ӧ���еĸ澯ID������ָ��SActiveAlarm�ṹ�е�
            �澯ID�Լ��澯����
�������    pAlarm�����ȴ����ø澯ID���澯���͵�SActiveAlarm�ṹָ��
            uAlarmId��������Ӧ����Ϣ�еĸ澯ID
�������    pAlarm���������˸澯ID���澯���ͺ��SActiveAlarm�ṹָ��
����ֵ      ��
�׳��쳣    ��
��ע        �ڸ澯ģ���ڲ������ϸ澯�Լ���Ӧ�Ļָ��澯�ĸ澯ID��ͬ��
            �����ڷ��������ĸ澯��Ϣ�У��ָ��澯��IDΪ��Ӧ�Ĺ��ϸ澯��ID��1
            ���ҷ��������ĸ澯ID���ڲ��澯ID�����һ���澯ID����
**********************************************************************/
void CBinAlarmHandler::SetInnerAlarmId(SActiveAlarm* pAlarm, UINT2 uAlarmId)
{
    ASSERT(uAlarmId >= m_uBaseAlarmID);

    UINT2 uInnerAlarmId = uAlarmId - m_uBaseAlarmID;

    //���������Ļָ��澯ID�������ã���������Ҳ����ø澯ID����Ϊ�ָ��澯
    SAlarmExplain* pAlarmExplain = m_pAlarmCfg->GetAlarmExplain(uInnerAlarmId);
    if (NULL == pAlarmExplain)  //�ָ��澯
    {
        pAlarm->uAlarmID = uInnerAlarmId - 1;
        pAlarm->yAlarmType = AT_RESTORE;
    }
    else if(AT_FAULT == pAlarmExplain->yAlarmType)  //���ϸ澯
    {
        pAlarm->uAlarmID = uInnerAlarmId;
        pAlarm->yAlarmType = AT_FAULT;
    }
    else //�¼��澯
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
��������    ��ʱ�Ӵ�����
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSoftXBinAlarmHandler::Timer1Sec()
{
    //������ �������Ӵ���
    CAlarmHandler::Timer1Sec();
    if(m_ActiveAlarmList.size() > 0)
    {
        if(m_nAlarmTimerCnt == 0 || m_nAlarmTimerCnt >= m_nTimeOut)
        {
            //���˳�ʱ������Ҫ�ط��澯
            if(m_nAlarmTimerCnt >= m_nTimeOut)
            {
                TRACEDebug(MTS_ALARM,S_ALARM_RESEND_NEXT);
                m_nAlarmTimerCnt = 0;
            }

            //������澯����
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

                //û���յ�Ӧ�����͸澯
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

                    //��ʼ��Ӧ��ʱ����м���
                    m_nAlarmTimerCnt++;

                    //ÿ��ֻ����һ���澯
                    break;
                }
                
                iter++;
            }
        }
        else
        {
            //��û���յ�Ӧ�𣬼�������1
            m_nAlarmTimerCnt++;
        }
    } 
}
/**********************************************************************
��������    �����յ��������澯Ӧ��
�������    pFrame�����յ��ĸ澯Ӧ����Ϣ
�������    ��
����ֵ      ��
�׳��쳣    ��
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


                //����ǹ��ϸ澯��Ӧ��������ڻ�澯�����еȴ�
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
��������    ����SActiveAlarm�ṹ���Ͷ����Ƹ澯
�������    pAlarm���������͵Ķ����Ƹ澯����Ϣ
�������    ��
����ֵ      ��
�׳��쳣    ������ռ�ʧ��ʱ�׳��쳣
**********************************************************************/
void CSoftXBinAlarmHandler::SendAlarm(SActiveAlarm* pAlarm)
{
    //����pAlarm��������д��Ӧ��MSG_INTRA��Ϣ�������͸�����ģ��
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
    pSysFrame->ucHandle(m_nLastSendHandle); //����������ʱ�޸�
    pSysFrame->ucCmdCode(SYS_CMD_ALARM);

    SSysAlarmInfo* pSysAlarmInfo = (SSysAlarmInfo* )(pSysFrame->Data());
    pSysAlarmInfo->ulAlarmSN(pAlarm->uCsn); //��ǰ���ҽ�������Ϊ0
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

    //���ݸ澯���û�ȡ�澯�Ĳ�������
    SAlarmExplain* pAlarmExplain = m_pAlarmCfg->GetAlarmExplain(pAlarm->uAlarmID);
    ASSERT(NULL != pAlarmExplain);
    //zhangyuxin modify + 1
    pSysAlarmInfo->uwParaLength(MAX_ALARM_PARA);//�����Ĳ������ֻ��Ϊ78
    int nParaNum = pAlarmExplain->yParaNum;

    //�޲�����һ�����������
    memcpy(pSysAlarmInfo->ucPara, pAlarm->yPara, pAlarmExplain->yParaNum);
    pSysAlarmInfo->ucPara[pAlarmExplain->yParaNum] = pAlarm->yModuleNO;

    //�������������
    if(nParaNum == 2)
    {
        pSysAlarmInfo->ucPara[1] = pAlarm->yModuleNO;
        memcpy(&pSysAlarmInfo->ucPara[2], &pAlarm->yPara[1], MAX_ALARM_PARA - 3);
        pSysAlarmInfo->ucPara[MAX_ALARM_PARA - 1] = '\0';
    }        

    CWorkModule::SendMsg(pAlarmMsg);
}    
/**********************************************************************
��������    �����յ��������Ķ����Ƹ澯Ӧ����Ϣ�����еõ���Ӧ�ĸ澯��Ϣ
�������    pMsg�����յ��Ķ����Ƹ澯Ӧ�����Ϣ
�������    ��
����ֵ      ��pMsg�з��������ĸ澯��Ϣ�������߲���ɾ��
�׳��쳣    ��
**********************************************************************/
BOOL CSoftXBinAlarmHandler::ParseAlarmResponse(MSG_INTRA* pMsg,SActiveAlarm *&pActive)
{
    //������ 2002-05-10 ����д�������Ժ���Ҹ澯���л�ʧ�ܡ�
    //����pMsg��Data���е���Ϣ��дAlarmInfo
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
��������    ��ʱ�Ӵ�����
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CMSCBinAlarmHandler::Timer1Sec()
{
    CAlarmHandler::Timer1Sec();
    SActiveAlarm* pActiveAlarm = NULL;
    CActiveAlarmList::iterator i = m_ActiveAlarmList.begin();

    //����m_ActiveAlarmList�е����л�澯�����ݸ澯���ͷֱ���в�ͬ�Ĵ���
    while (i != m_ActiveAlarmList.end())
    {
        pActiveAlarm = *i;
        if (NULL == pActiveAlarm)
        {
            i = m_ActiveAlarmList.erase(i);
            continue;
        }

        if (AT_EVENT == pActiveAlarm->yAlarmType)
        //����ÿ���¼��澯������ָ���Ĵ���
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
                delete pActiveAlarm;//������ 2002-05-10
                pActiveAlarm = NULL;
                continue;
            }
        }
        else if (AT_FAULT == pActiveAlarm->yAlarmType)
        //������ϸ澯δ�յ�Ӧ������
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
        //����ָ��澯δ�յ�Ӧ�����ͣ�����Ѿ��յ�Ӧ����ɾ��
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
                delete pActiveAlarm; //������ 2005-05-10
                pActiveAlarm = NULL;
                continue;
            }
        }

        i++;
    }

}
/**********************************************************************
��������    �����յ��������澯Ӧ��
�������    pFrame�����յ��ĸ澯Ӧ����Ϣ
�������    ��
����ֵ      ��
�׳��쳣    ��
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
    //���m_ActiveAlarmList�����в����ڶ�Ӧ�ĸ澯�����ø澯ID��
    //�澯����ͬʱƥ�䣩����ֱ�ӷ��أ�
    //������yReceived��ΪAR_RECEIVED�������ӻ�澯�����������ø澯
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
��������    ����SActiveAlarm�ṹ���Ͷ����Ƹ澯
�������    pAlarm���������͵Ķ����Ƹ澯����Ϣ
�������    ��
����ֵ      ��
�׳��쳣    ������ռ�ʧ��ʱ�׳��쳣
**********************************************************************/
void CMSCBinAlarmHandler::SendAlarm(SActiveAlarm* pAlarm)
{
    SAlarmExplain* pAlarmExplain = m_pAlarmCfg->GetAlarmExplain(pAlarm->uAlarmID);
    ASSERT(NULL != pAlarmExplain);

    //����pAlarm��������д��Ӧ��MSG_INTRA��Ϣ�������͸�����ģ��
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
    pMemAlarmInfo->sender_mid(0);   //����û�����壬��Ϊ0
    pMemAlarmInfo->sender_pid(APP_PID_BAU);
    pMemAlarmInfo->receiver_mid(APP_MID_BAM);
    pMemAlarmInfo->receiver_pid(APP_PID_ALARM);
    pMemAlarmInfo->nLen(_MEM_ALARM_INFO_EXPAND_LENGTH
                         + pAlarmExplain->yParaNum);
    pMemAlarmInfo->byBauNo(pAlarm->yCPUPos);
    pMemAlarmInfo->uAlarmID(GetOuterAlarmId(pAlarm));
    pMemAlarmInfo->byAllRestore(0); //����û�����壬��Ϊ0
    if (pAlarmExplain->yParaNum > 0)
    {
        ASSERT(1 == pAlarmExplain->yParaNum);

        memcpy(pMemAlarmInfo->Paras(), pAlarm->yPara, MAX_ALARM_PARA);
    }

    CWorkModule::SendMsg(pAlarmMsg);
}
/**********************************************************************
��������    �����յ��������Ķ����Ƹ澯Ӧ����Ϣ�����еõ���Ӧ�ĸ澯��Ϣ
�������    pMsg�����յ��Ķ����Ƹ澯Ӧ�����Ϣ
�������    ��
����ֵ      ��pMsg�з��������ĸ澯��Ϣ�������߲���ɾ��
�׳��쳣    ��
**********************************************************************/
BOOL CMSCBinAlarmHandler::ParseAlarmResponse(MSG_INTRA* pMsg,SActiveAlarm *&pActive)
{
    static SActiveAlarm AlarmInfo;
        AlarmInfo.yPara[0] = 0xFF; //������ 2002��05��09
    //����pMsg��Data���е���Ϣ��дAlarmInfo
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