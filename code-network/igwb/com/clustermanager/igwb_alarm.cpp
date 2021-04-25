#include "../include/frame.h"
#include "heartbeat_link.h"
#include "res_object.h"
#include "igwb_alarm.h"


/********************************************************
* ����: CIGWBAlarm::CIGWBAlarm
* ����: ���캯��
* ����:
* ���:
* ����:
********************************************************/
CIGWBAlarm::CIGWBAlarm()
{
    //
    m_bSwitchFault = FALSE;
}

/********************************************************
* ����: CIGWBAlarm::~CIGWBAlarm
* ����:
* ����:
* ���:
* ����:
********************************************************/
CIGWBAlarm::~CIGWBAlarm()
{
    //
}

/********************************************************
* ����: CIGWBAlarm::OnNotify
* ����: ˫���¼���Ϣ������
* ����:
*       ECLUSTER_NOTIFY uMsg : �¼���Ϣ��
*       UINT4 uParam1        : ����һ�������ǹ��϶����ָ��
*       UINT4 uParam2        : ����������������
* ���:
* ����:
********************************************************/
void CIGWBAlarm::OnNotify(ECLUSTER_NOTIFY uMsg,
                          UINT4 uParam1, UINT4 uParam2)
{
    CClusterAlarm::OnNotify(uMsg, uParam1, uParam2);

    SInnerAlarm Alarm;
    switch (uMsg)
    {
        case NOTIFY_HEARTLINK_BROKEN:
        //������·�жϵ�֪ͨ�����롣
        {
            CHeartBeatLink* pLink = (CHeartBeatLink*)uParam1;
            Alarm.yAlarmType      = AT_FAULT;
            Alarm.uAlarmID        = ALARM_ID_PRVNET;
			memset(Alarm.yAlarmPara, 0xff, MAX_ALARM_PARA);
            Alarm.yAlarmPara[0]   = pLink->GetLinkIndex();              
			
            CMsgProcessor::SendAlarm(&Alarm);
            AddHisAlarm(Alarm);
            break;
        }
        case NOTIFY_HEARTLINK_RECOVERY:
        //�����ָ���֪ͨ�����롣
        {
            CHeartBeatLink* pLink = (CHeartBeatLink*)uParam1;
            Alarm.yAlarmType      = AT_RESTORE;
            Alarm.uAlarmID        = ALARM_ID_PRVNET;
			memset(Alarm.yAlarmPara, 0xff, MAX_ALARM_PARA);
            Alarm.yAlarmPara[0]   = pLink->GetLinkIndex();         
			
            CMsgProcessor::SendAlarm(&Alarm);
            Alarm.yAlarmType      = AT_FAULT;
            RmvHisAlarm(Alarm);
            break;
        }
        case NOTIFY_HEARTBEAT_BROKEN:
        //�����жϵ�֪ͨ�����롣
        {
            Alarm.yAlarmType = AT_FAULT;
            Alarm.uAlarmID   = ALARM_ID_HEARTBEAT;
			memset(Alarm.yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);           
			
            CMsgProcessor::SendAlarm(&Alarm);
            AddHisAlarm(Alarm);
            break;
        }
        case NOTIFY_HEARTBEAT_RECOVERY:
        //�����ָ���֪ͨ�����롣
        {
            Alarm.yAlarmType = AT_RESTORE;
            Alarm.uAlarmID   = ALARM_ID_HEARTBEAT;
			memset(Alarm.yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);           
			
            CMsgProcessor::SendAlarm(&Alarm);
            Alarm.yAlarmType = AT_FAULT;
            RmvHisAlarm(Alarm);
            break;
        }
        case NOTIFY_RESOURCE_FAIL:
        //��Դʧ�ܵ�֪ͨ�����롣
        {
            CResObject* pResObject = (CResObject*)uParam1;
            Alarm.yAlarmType       = AT_FAULT;
            Alarm.uAlarmID         = pResObject->GetAlarmID();
			memset(Alarm.yAlarmPara, 0xff, MAX_ALARM_PARA);
            Alarm.yAlarmPara[0]    = pResObject->GetAlarmPara();           
			
            CMsgProcessor::SendAlarm(&Alarm);
            break;
        }
        case NOTIFY_RESOURCE_RECOVERY:
        //��Դ�ָ���֪ͨ�����롣
        {
            CResObject* pResObject = (CResObject*)uParam1;
            Alarm.yAlarmType       = AT_RESTORE;
            Alarm.uAlarmID         = pResObject->GetAlarmID();
			memset(Alarm.yAlarmPara, 0xff, MAX_ALARM_PARA);
            Alarm.yAlarmPara[0]    = pResObject->GetAlarmPara();           

            CMsgProcessor::SendAlarm(&Alarm);
            break;
        }
        case NOTIFY_HARDDISK_FAIL:
        //����Ӳ�̹���
    {
            Alarm.yAlarmType     = AT_FAULT;
            Alarm.uAlarmID       = ALARM_ID_DISK;
			memset(Alarm.yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);               
			
            CMsgProcessor::SendAlarm(&Alarm);
            break;
        }
        case NOTIFY_HARDDISK_RECOVERY:
        //����Ӳ�̹��ϻָ�
        {
            Alarm.yAlarmType     = AT_RESTORE;
            Alarm.uAlarmID       = ALARM_ID_DISK;
			memset(Alarm.yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);              
			
            CMsgProcessor::SendAlarm(&Alarm);
            break;
        }
        case NOTIFY_FAILOVER_RESP_SUCCESS:
        //FailOver��Ӧ�˲����ɹ�
        {
            if(!m_bSwitchFault)
            {
                Alarm.yAlarmType     = AT_FAULT;
                Alarm.uAlarmID       = ALARM_ID_FAILOVER;
			    memset(Alarm.yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);
			    
                CMsgProcessor::SendAlarm(&Alarm);

                m_bSwitchFault = TRUE;
            }
            break;
        }
        case NOTIFY_PEERNODE_RESTART:
        //�Զ�����
        {
            if(m_bSwitchFault)
            {
                Alarm.yAlarmType     = AT_RESTORE;
                Alarm.uAlarmID       = ALARM_ID_FAILOVER;
			    memset(Alarm.yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);
			    
                CMsgProcessor::SendAlarm(&Alarm);

                m_bSwitchFault = FALSE;
            }
            break;
        }
        default:
            break;
    }
}

/********************************************************
* ����: CIGWBAlarm::Timer1Sec
* ����: �붨ʱ��
* ����:
* ���:
* ����: void
********************************************************/
void CIGWBAlarm::Timer1Sec()
{
    CClusterAlarm::Timer1Sec();

    static int nSendHisAlarmInv = 0;
    if(++nSendHisAlarmInv >= 10)
    {
        LIST<SInnerAlarm>::iterator i;
        for (i = m_HisAlarmList.begin(); i != m_HisAlarmList.end(); i++)
        {
            CMsgProcessor::SendAlarm(&(*i));
#ifdef _ONLY_FOR_TEST
            TRACE(MTS_CLSTR, 
                  "SendAlarm(ID=%d, TYPE=%d, PARA=%d)",
                  i->uAlarmID,
                  i->yAlarmType,
                  i->yAlarmPara);
#endif _ONLY_FOR_TEST
        }
        nSendHisAlarmInv = 0;
    }
}

/********************************************************
* ����: CIGWBAlarm::AddHisAlarm
* ����: ������ʷ�澯֡
* ����: alarm : ��ʷ�澯֡�ṹ������
* ���: 
* ����: void 
********************************************************/
void CIGWBAlarm::AddHisAlarm(SInnerAlarm& alarm)
{
    LIST<SInnerAlarm>::iterator i;
    for (i = m_HisAlarmList.begin(); i != m_HisAlarmList.end(); i++)
    {
        if((alarm.uAlarmID == i->uAlarmID)
           && (alarm.yAlarmType == i->yAlarmType)
           && (memcmp(alarm.yAlarmPara, i->yAlarmPara, MAX_ALARM_PARA) == 0))
        {
            return;
        }
    }
    m_HisAlarmList.push_back(alarm);
}

/********************************************************
* ����: CIGWBAlarm::RmvHisAlarm
* ����: ɾ����ʷ�澯֡
* ����: alarm : ��ʷ�澯֡�ṹ������
* ���: 
* ����: void 
********************************************************/
void CIGWBAlarm::RmvHisAlarm(SInnerAlarm& alarm)
{
    LIST<SInnerAlarm>::iterator i;
    for (i = m_HisAlarmList.begin(); i != m_HisAlarmList.end(); i++)
    {
        if((alarm.uAlarmID == i->uAlarmID)
           && (alarm.yAlarmType == i->yAlarmType)
           && (memcmp(alarm.yAlarmPara, i->yAlarmPara, MAX_ALARM_PARA) == 0))
        {
            m_HisAlarmList.erase(i);
            return;
        }
    }
}
