#include "../include/frame.h"
#include "cluster_alarm.h"
#include "heartbeat_link.h"
#include "res_object.h"
#include "resource.h"


/********************************************************
* ����: CClusterAlarm::CClusterAlarm
* ����: ���캯��
* ����:
* ���:
* ����:
********************************************************/
CClusterAlarm::CClusterAlarm()
{
    //
}

/********************************************************
* ����: CClusterAlarm::~CClusterAlarm
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CClusterAlarm::~CClusterAlarm()
{
    //
}

/********************************************************
* ����: CClusterAlarm::OnNotify
* ����: ˫���¼���Ϣ������
* ����:
*       ECLUSTER_NOTIFY uMsg : �¼���Ϣ��
*       UINT4 uParam1        : ����һ�������ǹ��϶����ָ��
*       UINT4 uParam2        : ����������������
* ���:
* ����:
********************************************************/
void CClusterAlarm::OnNotify(ECLUSTER_NOTIFY uMsg,
                             UINT4 uParam1, UINT4 uParam2)
{
    switch (uMsg)
    {
        case NOTIFY_HEARTLINK_BROKEN:
        //������·�жϵ�֪ͨ������
        {
            CHeartBeatLink* pLink =  (CHeartBeatLink*) uParam1;
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_HEART_BEAT_LINK_BROKEN,
                   pLink->GetLinkIndex(),
                   pLink->GetLinkName(),
                   pLink->GetLinkType());
            TRACE(MTS_CLSTR,
                   S_HEART_BEAT_LINK_BROKEN,
                   pLink->GetLinkIndex(),
                   pLink->GetLinkName(),
                   pLink->GetLinkType());
            break;
        }
        case NOTIFY_HEARTLINK_RECOVERY:
        //������·�ָ���֪ͨ������
        {
            CHeartBeatLink* pLink =  (CHeartBeatLink*) uParam1;
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_HEART_BEAT_LINK_RECOVERY,
                   pLink->GetLinkIndex(),
                   pLink->GetLinkName(),
                   pLink->GetLinkType());
            TRACE(MTS_CLSTR,
                   S_HEART_BEAT_LINK_RECOVERY,
                   pLink->GetLinkIndex(),
                   pLink->GetLinkName(),
                   pLink->GetLinkType());
            break;
        }
        case NOTIFY_HEARTBEAT_BROKEN:
        //�����жϵ�֪ͨ������
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_HEART_BEAT_BROKEN);
            TRACE(MTS_CLSTR, S_HEART_BEAT_BROKEN);
            break;
        }
        case NOTIFY_HEARTBEAT_RECOVERY:
        //�����ָ���֪ͨ������
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_HEART_BEAT_RECOVERY);
            TRACE(MTS_CLSTR, S_HEART_BEAT_RECOVERY);
            break;
        }
        case NOTIFY_STARTRESOURCE_FAIL:
        //������Դʧ��
        {
            CResObject* pResObject =  (CResObject*) uParam1;
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_START_RES_FAIL, pResObject->GetResName(),
                   pResObject->GetResType());
            TRACE(MTS_CLSTR,
                  S_START_RES_FAIL, pResObject->GetResName(),
                  pResObject->GetResType());

            break;
        }
        case NOTIFY_START_ASYN_RES_FAIL:
        //�����첽��Դʧ��
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT, S_START_ASYNC_RES_FAIL);
            TRACE(MTS_CLSTR, S_START_ASYNC_RES_FAIL);
            break;
        }
        case NOTIFY_STOPRESOURCE_FAIL:
        //ֹͣ��Դʧ��
        {
            CResObject* pResObject =  (CResObject*) uParam1;
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_STOP_RES_FAIL, pResObject->GetResName(),
                   pResObject->GetResType());
            TRACE(MTS_CLSTR,
                  S_STOP_RES_FAIL, pResObject->GetResName(),
                  pResObject->GetResType());
            //ͨ��TRACE��MSGOUT�ȷ�ʽ���ʧ����Դ������;
            break;
        }
        case NOTIFY_RESOURCE_FAIL:
        //��Դʧ�ܵ�֪ͨ������
        {
            CResObject* pResObject =  (CResObject*) uParam1;
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_RESOURCE_FAIL, pResObject->GetResName(),
                   pResObject->GetResType());
            TRACE(MTS_CLSTR,
                  S_RESOURCE_FAIL, pResObject->GetResName(),
                  pResObject->GetResType());
            break;
        }
        case NOTIFY_RESOURCE_RECOVERY:
        //��Դ�ָ���֪ͨ������
        {
            CResObject* pResObject =  (CResObject*) uParam1;
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_RESOURCE_RECOVERY, pResObject->GetResName(),
                   pResObject->GetResType());
            TRACE(MTS_CLSTR,
                  S_RESOURCE_RECOVERY, pResObject->GetResName(),
                  pResObject->GetResType());
            break;
        }
        case NOTIFY_ASYN_RES_FAIL:
        //�첽��Դʧ�ܵ�֪ͨ������
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_AYSN_RESOURCE_FAIL);
            TRACE(MTS_CLSTR, S_AYSN_RESOURCE_FAIL);
            break;
        }
        case NOTIFY_HARDDISK_FAIL:
        //����Ӳ�̹���
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_HARDDISK_FAIL, uParam1);
            TRACE(MTS_CLSTR, S_HARDDISK_FAIL, uParam1);
            break;
        }
        case NOTIFY_HARDDISK_RECOVERY:
        //����Ӳ�̹��ϻָ�
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT,
                   S_HARDDISK_RECOVERY);
            TRACE(MTS_CLSTR, S_HARDDISK_RECOVERY);
            break;
        }
        case NOTIFY_ACTIVE_SUCCESS:
        //����ɹ�֪ͨ������
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, S_ACTIVE_SUCCESS);
            TRACE(MTS_CLSTR, S_ACTIVE_SUCCESS);
            break;
        }
        case NOTIFY_ACTIVE_FAIL:
        //����ʧ��֪ͨ������
        {
            TRACE(MTS_CLSTR, S_ACTIVE_FAILED);
            break;
        }
        case NOTIFY_DEACTIVE_SUCCESS:
        //ȥ����ɹ�֪ͨ������
        {
            TRACE(MTS_CLSTR, S_DEACTIVE_SUCCESS);
            break;
        }
        case NOTIFY_FAILOVER:
        //����FailOver�л���֪ͨ������
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT, S_BEGIN_FAIL_OVER, uParam1);
            TRACE(MTS_CLSTR, S_BEGIN_FAIL_OVER, uParam1);
            break;
        }
        case NOTIFY_FAILOVER_SUCCESS:
        //FailOver�л��ɹ�
        {
            TRACE(MTS_CLSTR, S_FAIL_OVER_SUCCESS);
            break;
        }
        case NOTIFY_FAILOVER_FAIL:
        //FailOver�л�ʧ��
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT, S_FAIL_OVER_FAILED);
            TRACE(MTS_CLSTR, S_FAIL_OVER_FAILED);
            break;
        }
        case NOTIFY_FAILOVER_RESP_SUCCESS:
        //FailOver��Ӧ�˲����ɹ�
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT, S_FAIL_OVER_RESP_SUCCESS);
            TRACE(MTS_CLSTR, S_FAIL_OVER_RESP_SUCCESS);
            break;
        }
        case NOTIFY_FAILOVER_RESP_FAIL:
        //FailOver��Ӧ�˲���ʧ��
        {
            TRACE(MTS_CLSTR, S_FAIL_OVER_RESP_FAILED);
            break;
        }
        case NOTIFY_AGGRESSIVE_ACTIVE:
        //��������
        {
            TRACE(MTS_CLSTR, S_AGGRESSIVE_ACTIVE);
            break;
        }
        case NOTIFY_AGGRESSIVE_ACTIVE_SUCCESS:
        //��������ɹ�
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT, S_AGGRESSIVE_ACTIVE_SUCCESS);
            TRACE(MTS_CLSTR, S_AGGRESSIVE_ACTIVE_SUCCESS);
            //ͨ��TRACE��MSGOUT�ȷ�ʽ���������������ɹ�����Ϣ;
            break;
        }
        case NOTIFY_AGGRESSIVE_ACTIVE_FAIL:
        //��������ʧ��
        {
            TRACE(MTS_CLSTR, S_AGGRESSIVE_ACTIVE_FAIL);
            break;
        }
        case NOTIFY_AGGRESSIVE_DEACTIVE:
        //����ȥ����
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT, S_AGGRESSIVE_DEACTIVE);
            TRACE(MTS_CLSTR, S_AGGRESSIVE_DEACTIVE);
            break;
        }
        case NOTIFY_AGGRESSIVE_DEACTIVE_COMPLETE:
        //����ȥ�������
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT, S_AGGRESSIVE_DEACTIVE_COMPLETE);
            TRACE(MTS_CLSTR, S_AGGRESSIVE_DEACTIVE_COMPLETE);
            break;
        }
        case NOTIFY_HANDOVER:
        //����HandOver�л���֪ͨ������
        {
            TRACE(MTS_CLSTR, S_BEGIN_HANDOVER);
            break;
        }
        case NOTIFY_HANDOVER_SUCCESS:
        //HandOver�л��ɹ�
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_URGENT, S_HANDOVER_SUCCESS);
            TRACE(MTS_CLSTR, S_HANDOVER_SUCCESS);
            break;
        }
        case NOTIFY_HANDOVER_FAIL:
        //HandOver�л�ʧ��
        {
            TRACE(MTS_CLSTR, S_HANDOVER_FAIL);
            break;
        }
        case NOTIFY_PEERNODE_RESTART:
        //�Զ�����
        {
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, S_PEERNODE_RESTART);
            TRACE(MTS_CLSTR, S_PEERNODE_RESTART);
            break;
        }
        case NOTIFY_LOCALNODE_FAILURE:
        //���˷������ϣ��������жϵ�����⣬�����ټ���
        {
            TRACE(MTS_CLSTR, S_LOCALNODE_FAILURE);
            break;
        }
        default:
            break;
    }
}

/********************************************************
* ����: CClusterAlarm::Timer1Sec
* ����: �붨ʱ��
* ����:
* ���:
* ����: void
********************************************************/
void CClusterAlarm::Timer1Sec()
{
    //
}
