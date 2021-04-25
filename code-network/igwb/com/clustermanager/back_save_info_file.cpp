#include "../include/base_type.h"
#include "../include/msg_def.h"
#include "../backsave/save_chanl.h"
#include "sync_struct.h"
#include "back_save_info_file.h"


/********************************************************
* ����: CBackSaveInfoFile::CBackSaveInfoFile
* ����: ���캯��
* ����: const char* const szFilePath    : ��״̬�ļ�����·��
*       const char* const szFileName    : ��״̬�ļ���
*       const char* const szBakFilePath : ��״̬�ļ�����·��
*       const char* const szBakFileName : ��״̬�ļ���
*       int nAPNum                      : ������
*       int nModule                     : ģ���
*       int nChl                        : ͨ����
* ���:
* ����:
********************************************************/
CBackSaveInfoFile::CBackSaveInfoFile(const char* const szFilePath,
                                     const char* const szFileName,
                                     const char* const szBakFilePath,
                                     const char* const szBakFileName,
                                     int nAPNum,
                                     int nModule,
                                     int nChl)
                                     :CSaveInfoFile(szFilePath,
                                                    szFileName,
                                                    szBakFilePath,
                                                    szBakFileName,
                                                    nAPNum,
                                                    nModule,
                                                    nChl)
{
    //
}

/********************************************************
* ����: CBackSaveInfoFile::~CBackSaveInfoFile
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CBackSaveInfoFile::~CBackSaveInfoFile()
{
    //
}

/********************************************************
* ����: CBackSaveInfoFile::GetSyncData
* ����: ��ȡͬ�����ݲ���
* ����: nData     : �����������ָ��
*       nLen      : ����������ĳ���
*       bReadFile : �Ƿ���ļ��ж����µ����ݣ�TRUE��ʾҪ
*                   ���ļ��ж�����FALSE��ʾ�ӻ������ж���
* ���:
* ����: int ��õ�ʵ�ʶ��������ݳ���
********************************************************/
int CBackSaveInfoFile::GetSyncData(void* pData, int nLen, BOOL bReadFile)
{
    errno = 0;

    if (nLen < m_nSyncDataLen)
    {
        errno = ERR_BUFFER_NOT_ENOUGH;
        return 0;
    }

    if(bReadFile)
    {
        //��״̬�ļ�;
        if (Open() != ERR_SUCCESS)
        {
            return 0;
        }
    }

    //��m_pBufferǿ��ת��ΪBS_STATUS_FILE_HEAD�ṹ����pData
    //ǿ��ת��ΪSSaveSyncData�ṹ������m_pBufferָ��Ļ�����
    //�л�ȡ���š��ļ����кż��������кţ���д��pDataָ��Ļ�������
    ((SSaveSyncData*)pData)->uFileCsn = ((BS_STATUS_FILE_HEAD*)m_pBuffer)->uFileCsn;
    ((SSaveSyncData*)pData)->uPID     = ((BS_STATUS_FILE_HEAD*)m_pBuffer)->uLastPID;
    ((SSaveSyncData*)pData)->uBillCsn = ((BS_STATUS_FILE_HEAD*)m_pBuffer)->uBillCsn;

    return m_nSyncDataLen;
}

/********************************************************
* ����: CBackSaveInfoFile::PutSyncData
* ����: д��״̬���ݲ���
* ����: pData : ���뻺������ָ��
*       nLen  : ���뻺�����ĳ���
* ���:
* ����: int ��õ�ʵ��д������ݳ���
********************************************************/
int CBackSaveInfoFile::PutSyncData(void* pData, int nLen)
{
    errno = 0;

    //���û��������ֱ���˳�
    if(nLen <= 0)
    {
        return 0;
    }

    //�����������ݳ���С����Ҫ�ĳ���,�򷵻�ʧ��
    if(nLen < m_nSyncDataLen)
    {
        errno = ERR_BUFFER_NOT_ENOUGH;
        return 0;
    }

    ((BS_STATUS_FILE_HEAD*)m_pBuffer)->uFileCsn = 
        ((SSaveSyncData*)pData)->uFileCsn;
    ((BS_STATUS_FILE_HEAD*)m_pBuffer)->uBillCsn = 
        ((SSaveSyncData*)pData)->uBillCsn;

    return m_nSyncDataLen;
}
