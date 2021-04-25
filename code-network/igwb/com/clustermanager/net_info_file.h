#ifndef __NET_SYNC_INFO_FILE_H__
#define __NET_SYNC_INFO_FILE_H__

#include "sync_info_file.h"

#define NETWORK_CHANNEL_ID  254

class CNetInfoFile : public CSyncInfoFile
{
public:
    CNetInfoFile(const char* const szFilePath,
                 const char* const szFileName,
                 int nAPNum,
                 int nModule,
                 int nChl);
    virtual ~CNetInfoFile();

    //״̬�ļ��򿪲�����ֻ�б�ͬ���������
    virtual int Open();
    //״̬�ļ��رղ�����ֻ�б�ͬ���������
    virtual void Close();
    //��ȡͬ�����ݲ���
    virtual int GetSyncData(void* pData, int nLen, BOOL bReadFile = TRUE);
    //д��״̬���ݲ���
    virtual int PutSyncData(void* pData, int nLen);
    //���������е�����д��״̬�ļ���ֻ�б�ͬ���������
    virtual int Flush();

protected:

    //״̬�ļ���FILEָ��
    FILE*    m_pFile;
};

#endif //__NET_SYNC_INFO_FILE_H__
