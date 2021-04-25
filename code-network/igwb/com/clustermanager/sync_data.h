#ifndef __SYNC_OBJECT_H__
#define __SYNC_OBJECT_H__

#include "sync_info_file.h"

class CSyncData
{
public:
    CSyncData(int nAccessPoint);
    virtual ~CSyncData();

    //��ʼ������
    virtual int InitInstance() = 0;

    //״̬�ļ��򿪲�����ֻ���ڱ�ͬ����׼������ͬ������ʱ����
    int Open();
    //״̬�ļ��رղ�����ֻ���ڱ�ͬ������ɽ���ͬ�����ݺ����
    void Close();

    //�����ѷ�װ��SMscAPSyncFrame�ṹ��ͬ�����ݵĳ���
    virtual int GetSyncDataLen();
    //��ȡͬ�����ݲ���
    virtual int GetSyncData(void* pData, int nLen, BOOL bReadFile = TRUE) = 0;
    //д��״̬���ݲ���
    virtual int PutSyncData(void* pData, int nLen) = 0;
    //��ȡͬ�����ݲ�����ֻ��ͬ���������
    virtual void DoSyncData(void* pData, int nLen) = 0;

protected:

    //ͬ��״̬�ļ���д����ָ��Ķ���
    LIST<CSyncInfoFile*>    m_SyncInfoFileList;

    //�������ͬ�����ݽṹ�ĳ���
    int                     m_nBufferLen;

    //������
    int                     m_nAccessPoint;

    //�ϴμ����ʱ��
    time_t                  m_tLastActiveTime;
};

#endif //__SYNC_OBJECT_H__
