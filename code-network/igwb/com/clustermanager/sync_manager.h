#ifndef __SYNC_OBJECT_MANAGER_H__
#define __SYNC_OBJECT_MANAGER_H__

#include "sync_data.h"

class CSyncManager
{
public:
    CSyncManager();
    virtual ~CSyncManager();

    int InitInstance(); //��ʼ������

    //״̬�ļ��򿪲�����ֻ���ڱ�ͬ����׼������ͬ������ʱ����
    int Open();
    //״̬�ļ��رղ�����ֻ���ڱ�ͬ������ɽ���ͬ�����ݺ����
    void Close();

    //�����ѷ�װ��SMscAPSyncFrame�ṹ��ͬ�����ݵĳ���
    int GetSyncDataLen();
    //��ȡͬ�����ݲ���
    int GetSyncData(void*& pData, int& nLen, BOOL bReadFile = TRUE);
    //д��״̬���ݲ���
    int PutSyncData(void* pData, int nLen);
    //��ȡͬ�����ݲ�����ֻ��ͬ���������
    void DoSyncData(void* pData, int nLen);            

protected:

    void ExitInstance();    //�˳�ǰ���������

    //�����ŵ�ͬ�����ݶ����MAP
    MAP<BYTE, CSyncData*>*     m_pSyncDataMap;
};

#endif //__SYNC_OBJECT_MANAGER_H__
