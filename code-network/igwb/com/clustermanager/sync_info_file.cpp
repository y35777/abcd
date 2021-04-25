#include "../include/base_type.h"
#include "sync_info_file.h"


/********************************************************
* ����: CSyncInfoFile::CSyncInfoFile
* ����: ���캯��
* ����: const char* const szFilePath : �ļ�����·��
*       const char* const szFileName : �ļ���
*       int nAPNum                   : ������
*       int nModule                  : ģ���
*       int nChl                     : ͨ����
* ���:
* ����:
********************************************************/
CSyncInfoFile::CSyncInfoFile(const char* const szFilePath,
                             const char* const szFileName,
                             int nAPNum,
                             int nModule,
                             int nChl)
{
    m_nBufferLen    = 0;
    m_pBuffer       = NULL;
    m_nSyncDataLen  = 0;
    m_nAccessPoint  = nAPNum;
    m_nModule       = nModule;
    m_nChannel      = nChl;

    strncpy(m_szFilePath, szFilePath, MAX_PATH);
    m_szFilePath[MAX_PATH - 1] = '\0';

    strncpy(m_szFileName, szFileName, MAX_PATH);
    m_szFileName[MAX_PATH - 1] = '\0';

    strncpy(m_szFullFileName, m_szFilePath, MAX_PATH);
    m_szFullFileName[MAX_PATH - 1] = '\0';

    if((strlen(m_szFullFileName) > 0) &&
       (m_szFullFileName[strlen(m_szFullFileName) - 1] != '\\') &&
       (m_szFullFileName[strlen(m_szFullFileName) - 1] != '/'))
    {
        strncat(m_szFullFileName, "/", MAX_PATH);
        m_szFullFileName[MAX_PATH - 1] = '\0';
    }

    strncat(m_szFullFileName, m_szFileName, MAX_PATH);
    m_szFullFileName[MAX_PATH - 1] = '\0';
}

/********************************************************
* ����: CSyncInfoFile::~CSyncInfoFile
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CSyncInfoFile::~CSyncInfoFile()
{
    if (NULL != m_pBuffer)
    {
        delete [] m_pBuffer;
        m_pBuffer = NULL;
    }
}

/********************************************************
* ����: CSyncInfoFile::GetAccessPoint
* ����: ��ý�����
* ����:
* ���:
* ����: int ������
********************************************************/
int CSyncInfoFile::GetAccessPoint()
{
    return m_nAccessPoint;
}

/********************************************************
* ����: CSyncInfoFile::GetModule
* ����: ���ģ���
* ����:
* ���:
* ����: int ģ���
********************************************************/
int CSyncInfoFile::GetModule()
{
    return m_nModule;
}

/********************************************************
* ����: CSyncInfoFile::GetChannel
* ����: ���ͨ����
* ����:
* ���:
* ����: int ͨ����
********************************************************/
int CSyncInfoFile::GetChannel()
{
    return m_nChannel;
}

/********************************************************
* ����: CSyncInfoFile::GetSyncDataLen
* ����: ���ͬ�����ݳ���
* ����:
* ���:
* ����: int ͬ�����ݳ���
********************************************************/
int CSyncInfoFile::GetSyncDataLen()
{
    return m_nSyncDataLen;
}

/********************************************************
* ����: CSyncInfoFile::GetLastWriteTime
* ����: ȡ״̬�ļ��������޸�ʱ��
* ����:
* ���:
* ����: time_t
********************************************************/
time_t CSyncInfoFile::GetLastWriteTime()
{
    //��ȡm_szFileNameָ���ļ������¸���ʱ��
    ACE_stat FStat;
    memset((char*)&FStat, 0, sizeof(FStat));

    ACE_OS::stat(m_szFullFileName, &FStat);

    return FStat.st_mtime;
}
