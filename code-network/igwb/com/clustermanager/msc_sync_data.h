#ifndef __MSC_SYNC_OBJECT_H__
#define __MSC_SYNC_OBJECT_H__

#include "sync_struct.h"
#include "sync_data.h"

class CMscSyncData : public CSyncData
{
public:
    CMscSyncData(int nAccessPoint);
    virtual ~CMscSyncData();

    //��ʼ������
    virtual int InitInstance();

    //�����ѷ�װ��SMscAPSyncFrame�ṹ��ͬ�����ݵĳ���
    virtual int GetSyncDataLen();
    //��ȡͬ�����ݲ���
    virtual int GetSyncData(void* pData, int nLen, BOOL bReadFile = TRUE);
    //д��״̬���ݲ���
    virtual int PutSyncData(void* pData, int nLen);
    //ʵʩ����ͬ��������ֻ��ͬ���������
    virtual void DoSyncData(void* pData, int nLen);
    //��ȡ�ϴ�д״̬�ļ���ʱ��
    time_t GetLastWriteTime();

protected:

    //����ָ��ģ��ż�ͨ���ŵ�ͬ�����ݿ�ָ��
    SMscSyncData* FindSyncData(SMscAPSyncFrame* pAPFrame, 
                               UINT4 uMID, UINT4 uChannel);

    //ǰ���̵İ���
    UINT4   m_uFrontSavePID;

    //ǰ���̵��ļ����к�
    UINT4   m_uFrontFileCsn;

    //����ģ��İ���
    UINT4   m_uNetworkPID;
};

#endif //__MSC_SYNC_OBJECT_H__
