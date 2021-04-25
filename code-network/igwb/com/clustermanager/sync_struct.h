#ifndef __SYNC_STRUCT_H__
#define __SYNC_STRUCT_H__

struct SSyncFrame
{
    UINT4   uLen;           //����֡����
    BYTE    Data[1];        //ͬ������
};
#define SYNC_FRAME_HEAD_LENGTH  offsetof(SSyncFrame, Data)

struct SMscAPSyncFrame      //һ��������µ�ͬ������
{
    UINT4   uAPointID;      //������
    UINT4   uDiffTime;      //ʱ���
    UINT4   uDiffPackId;    //���Ų�
    UINT4   uLen;           //����֡����
    BYTE    Data[1];        //ͬ������
};
#define  MSC_AP_SYNC_FRAME_HEAD_LENGTH  offsetof(SMscAPSyncFrame, Data)

struct SMscSyncData
{
    UINT4   uMID;           //����ͬ�����ݵ�ģ���
    UINT4   uChannel;       //ͨ���ţ���ʶͨ�������ں����ģ�飻
    UINT4   uLength;        //ͬ�����ݳ���
    BYTE    Data[1];        //ͬ������
};
#define  MSC_SYNC_DATA_HEAD_LENGTH  offsetof(SMscSyncData, Data)

struct SMscNetSyncData      //����ģ��״̬ͬ������
{
    UINT4   uPackId;            //����ģ��İ���
    BOOL    bWaitForResponse;   //�Ƿ�ȴ�ǰ����Ӧ�����ύ��֡����
    BYTE    Data[1];            //����ģ��״̬�ļ��е�������Ϣ����������������ˮ��
};

#endif //__SYNC_STRUCT_H__
