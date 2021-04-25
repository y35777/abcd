#include "../include/base_type.h"
#include "sync_data.h"



/********************************************************
* ����: CSyncData::CSyncData
* ����: ���캯��
* ����: nAccessPoint : ������
* ���:
* ����:
********************************************************/
CSyncData::CSyncData(int nAccessPoint)
{
    m_nBufferLen        = 0;
    m_nAccessPoint      = nAccessPoint;
    m_tLastActiveTime   = 0;
}

/********************************************************
* ����: CSyncData::~CSyncData
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CSyncData::~CSyncData()
{
    while(m_SyncInfoFileList.size() > 0)
    {
        delete m_SyncInfoFileList.back();
        m_SyncInfoFileList.pop_back();
    }
}

/********************************************************
* ����: CSyncData::Open
* ����: ״̬�ļ��򿪲�����ֻ���ڱ�ͬ����׼������ͬ������ʱ����
* ����:
* ���:
* ����: int �򿪲����Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CSyncData::Open()
{
    LIST<CSyncInfoFile*>::iterator i;

    for(i = m_SyncInfoFileList.begin(); i != m_SyncInfoFileList.end(); i++)
    {
        if((*i)->Open() != ERR_SUCCESS)
        {
            return ERR_FAIL;
        }
    }

    return ERR_SUCCESS;
}

/********************************************************
* ����: CSyncData::Close
* ����: ״̬�ļ��رղ�����ֻ���ڱ�ͬ������ɽ���ͬ�����ݺ����
* ����:
* ���:
* ����: void
********************************************************/
void CSyncData::Close()
{
    LIST<CSyncInfoFile*>::iterator i;

    for(i = m_SyncInfoFileList.begin(); i != m_SyncInfoFileList.end(); i++)
    {
        (*i)->Close();
    }
}

/********************************************************
* ����: CSyncData::GetSyncDataLen
* ����: �����ѷ�װ��SMscAPSyncFrame�ṹ��ͬ�����ݵĳ���
* ����:
* ���:
* ����: int ����ͬ�����ݵĳ���
********************************************************/
int CSyncData::GetSyncDataLen()
{
    return 0;
}

