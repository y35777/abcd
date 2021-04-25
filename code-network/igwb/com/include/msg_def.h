#ifndef __MESSAGEDEF_H__
#define __MESSAGEDEF_H__

#include "../include/precom.h"

//����ģ�鷢��ǰ����ģ���"���󱣴�ԭʼ������"��Ϣ����������
struct SFrontPacketInfo
{
    UINT4 uPID;             //�豣���ԭʼ�������İ���
    BYTE* pPacket;          //�豣���ԭʼ��������ָ��
    UINT4 uPacketLen;       //�豣���ԭʼ�������ĳ���
};

//��������ģ�鷢�������ģ���"���󱣴����ջ�����"��Ϣ����������
struct SBackPacketInfo
{
    BYTE yChannelID;//ͨ����
    UINT4 uPID;     //�豣���ԭʼ�������İ���
    BYTE* pPacket;  //�豣���ԭʼ��������ָ��
    UINT4 uLen;     //�豣���ԭʼ�������ĳ���
    UINT4 uBillCsn; //��ǰ����Ļ�����ˮ��
};

//�����ģ�鷢����������ģ���"֪ͨ״̬"��Ϣ����������
struct SInformStatus
{
    UINT4 uLastPID; //�����ģ��ĸ�ͨ��������������һ�����ջ����İ��ŵ���Сֵ
    UINT4 uaBillCsns[255];  //��ͨ�����Ļ�����ˮ�ŵ����飬�±��ʾͨ����
};

//ǰ�����ģ���ͬ�����ݽṹ
struct SSaveSyncData
{
    UINT4 uPID;       //��ǰ���ţ�
    UINT4 uFileCsn;   //��ǰ�ļ����кţ�
    UINT4 uBillCsn;   //�����ģ��Ļ������к�
};

#endif
