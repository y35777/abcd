#ifndef __CLUSTER_STRUCT_H_
#define __CLUSTER_STRUCT_H_

#include "../include/base_type.h"

enum ECLUSTER_NOTIFY
{
    NOTIFY_HEARTLINK_BROKEN,            //������·�жϵ�֪ͨ������
    NOTIFY_HEARTLINK_RECOVERY,          //�����ָ���֪ͨ������
    NOTIFY_HEARTBEAT_BROKEN,            //�����жϵ�֪ͨ������
    NOTIFY_HEARTBEAT_RECOVERY,          //�����ָ���֪ͨ������
    NOTIFY_STARTRESOURCE_FAIL,          //������Դʧ��
    NOTIFY_START_ASYN_RES_FAIL,         //�����첽��Դʧ��
    NOTIFY_STOPRESOURCE_FAIL,           //ֹͣ��Դʧ��
    NOTIFY_RESOURCE_FAIL,               //��Դʧ�ܵ�֪ͨ������
    NOTIFY_RESOURCE_RECOVERY,           //��Դ�ָ���֪ͨ������
    NOTIFY_ASYN_RES_FAIL,               //�첽��Դʧ��
    NOTIFY_HARDDISK_FAIL,               //���д���ʧ�ܵ�֪ͨ������
    NOTIFY_HARDDISK_RECOVERY,           //���д��ָ̻���֪ͨ������
    NOTIFY_ACTIVE_SUCCESS,              //����ɹ�֪ͨ������
    NOTIFY_ACTIVE_FAIL,                 //����ʧ��֪ͨ������
    NOTIFY_DEACTIVE_SUCCESS,            //ȥ����ɹ�֪ͨ������
    NOTIFY_FAILOVER,                    //����FailOver�л���֪ͨ������
    NOTIFY_FAILOVER_SUCCESS,            //FailOver�л��ɹ�
    NOTIFY_FAILOVER_FAIL,               //FailOver�л�ʧ��
    NOTIFY_FAILOVER_RESP_SUCCESS,       //FailOver��Ӧ�˲����ɹ�
    NOTIFY_FAILOVER_RESP_FAIL,          //FailOver��Ӧ�˲���ʧ��
    NOTIFY_AGGRESSIVE_ACTIVE,           //��������
    NOTIFY_AGGRESSIVE_ACTIVE_FAIL,      //��������ʧ��
    NOTIFY_AGGRESSIVE_ACTIVE_SUCCESS,   //��������ɹ�
    NOTIFY_AGGRESSIVE_DEACTIVE,         //����ȥ����
    NOTIFY_AGGRESSIVE_DEACTIVE_COMPLETE,//����ȥ�������
    NOTIFY_HANDOVER,                    //����HandOver�л���֪ͨ������
    NOTIFY_HANDOVER_SUCCESS,            //HandOver�л��ɹ�
    NOTIFY_HANDOVER_FAIL,               //HandOver�л�ʧ��
    NOTIFY_PEERNODE_RESTART,            //�Զ�����
    NOTIFY_LOCALNODE_FAILURE            //���˷������ϣ��������жϵ�����⣬�����ټ���
};

#define CLUSTER_FRAME_MAXSIZE    (64 * 1024)
#define CLUSTER_FRAME_HEADLENGTH (offsetof(SClusterFrame,Data))
struct SClusterFrame
{
    UINT4 uCheckSum;     //У����
    UINT4 uSeqNum;       //֡���к�
    INT4  nCmdCode;      //�������������
    INT4  nLen;          //��data��ʼ�����ݳ���
    BYTE  Data[1];       //����

    void* operator new(size_t classSize, size_t DataLength = 0)
    {
        void* ptr = NULL;
        UINT4 uLen = (DataLength > 0) ? DataLength : 0;

        UINT4 length = uLen + CLUSTER_FRAME_HEADLENGTH;
        if (length > CLUSTER_FRAME_MAXSIZE)
        {
            return NULL;
        }

        ptr = ::new char[length];
        if(ptr)
        {
            memset(ptr, 0, length);
            ((SClusterFrame*)ptr)->nLen = length - CLUSTER_FRAME_HEADLENGTH;
        }
        return ptr;
    };

    //��дУ����
    void FillCheckSum()
    {
        uCheckSum = uSeqNum ^ nCmdCode ^ nLen;
    };

    //��֤У����
    BOOL VerifyCheckSum()
    {
        UINT4 uValue = uSeqNum ^ nCmdCode ^ nLen;

        return uValue == uCheckSum;
    };
};

enum CLS_CMD_TYPE
{

    /*
        ˫�������õ���������
    */
    #define CMD_CLS_LOGICAL_MINCODE     0
    #define CMD_CLS_LOGICAL_MAXCODE     99

    CMD_CLS_SHAKEHANDS          = 0,	//˫������֡
    CMD_CLS_HEARTBEAT			= 1,	//˫������֡
    CMD_CLS_HANDOVER			= 2,	//HandOver������
    CMD_CLS_ANS_HANDOVER		= 3,	//HandOverӦ����
    CMD_CLS_FAILOVER			= 4,	//FailOver������
    CMD_CLS_ANS_FAILOVER		= 5,	//FailOverӦ����
    CMD_CLS_ASKFOR_HANDOVER		= 6,	//�ֶ��л�ʱ������Զ˷���HandOver������������
    CMD_CLS_ASKFOR_ACTIVE		= 7,	//�˳�ǰ������Զ˷����������������������

    /*
        ˫�����첽��ԴͨѶ�õ���������
    */
    #define CMD_CLS_ASYNRES_MINCODE     100
    #define CMD_CLS_ASYNRES_MAXCODE     199

    CMD_CLS_SYNC_DATA           = 100	//ͬ������������
};

#endif //__CLUSTER_STRUCT_H_
