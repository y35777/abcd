#include "../include/base_type.h"
#include "../include/msg_def.h"
#include "../frontsave/front_save.h"
#include "sync_struct.h"
#include "save_info_file.h"


/********************************************************
* ����: CSaveInfoFile::CSaveInfoFile
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
CSaveInfoFile::CSaveInfoFile(const char* const szFilePath,
                             const char* const szFileName,
                             const char* const szBakFilePath,
                             const char* const szBakFileName,
                             int nAPNum,
                             int nModule,
                             int nChl)
                             :CSyncInfoFile(szFilePath,
                                            szFileName,
                                            nAPNum,
                                            nModule,
                                            nChl)
{
    strncpy(m_szBakFilePath, szBakFilePath, MAX_PATH);
    m_szBakFilePath[MAX_PATH - 1] = '\0';

    strncpy(m_szBakFileName, szBakFileName, MAX_PATH);
    m_szBakFileName[MAX_PATH - 1] = '\0';

    m_uHeadLen     = -1;
    m_uUnitLen     = -1;
    m_uUnitNum     = -1;
    m_nSyncDataLen = sizeof(SSaveSyncData);

    //������/��״̬�ļ����ʹ��·��
    m_InfoFile.Init(m_szFilePath, m_szFileName, 
                    m_szBakFilePath, m_szBakFileName);
}

/********************************************************
* ����: CSaveInfoFile::~CSaveInfoFile
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CSaveInfoFile::~CSaveInfoFile()
{
    //
}

/********************************************************
* ����: CSaveInfoFile::SetFileStruct
* ����: ����״̬�ļ�ͷ��
* ����: pData    : ������ָ��
*       uHeadLen : ״̬�ļ�ͷ��
*       uUnitLen : ��Ԫ����
*       uUnitNum : ��Ԫ����
* ���:
* ����: void
********************************************************/
void CSaveInfoFile::SetFileStruct(void* pData,
                                  const UINT4 uHeadLen,
                                  const UINT4 uUnitLen, 
                                  const UINT4 uUnitNum)
{
    assert(0    != uHeadLen);
    m_uHeadLen   = uHeadLen;
    m_uUnitLen   = uUnitLen;
    m_uUnitNum   = uUnitNum;

    assert(NULL == m_pBuffer);
    m_nBufferLen = uHeadLen;
    m_pBuffer    = (char*)pData;

    //���ú����������ļ�ͷ���ȣ���¼��Ԫ���Ⱥͼ�¼��Ԫ����
    m_InfoFile.SetFileStruct(m_uHeadLen, m_uUnitLen, m_uUnitNum);
}

/********************************************************
* ����: CSaveInfoFile::Open
* ����: ״̬�ļ��򿪲�����ֻ�б�ͬ���������
* ����:
* ���:
* ����: int �򿪲����Ƿ�ɹ�
********************************************************/
int CSaveInfoFile::Open()
{
    //��״̬�ļ�������ļ����ڣ����ȡ״̬��Ϣ
    if(m_InfoFile.OpenInfoFile(FALSE))
    {
        //����״̬�ļ���ͷ����Ϣ
        assert(m_pBuffer != NULL);
        if(!m_InfoFile.ReadHead((BYTE*)m_pBuffer))
        {
            return ERR_FAIL;
        }
    }

    return ERR_SUCCESS;
}

/********************************************************
* ����: CSaveInfoFile::Close
* ����: ״̬�ļ��رղ�����ֻ�б�ͬ���������
* ����:
* ���:
* ����: void
********************************************************/
void CSaveInfoFile::Close()
{
    //�ر�״̬�ļ�
    m_InfoFile.Close();
}

/********************************************************
* ����: CSaveInfoFile::GetSyncData
* ����: ��ȡͬ�����ݲ���
* ����: nData     : �����������ָ��
*       nLen      : ����������ĳ���
*       bReadFile : �Ƿ���ļ��ж����µ����ݣ�TRUE��ʾҪ
*                   ���ļ��ж�����FALSE��ʾ�ӻ������ж���
* ���:
* ����: int ��õ�ʵ�ʶ��������ݳ���
********************************************************/
int CSaveInfoFile::GetSyncData(void* pData, int nLen, BOOL bReadFile)
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

    //��m_pBufferǿ��ת��ΪFS_STATUS_FILE_HEAD�ṹ����pData
    //ǿ��ת��ΪSSaveSyncData�ṹ������m_pBufferָ��Ļ�����
    //�л�ȡ���ż��ļ����кţ���д��pDataָ��Ļ�������
    ((SSaveSyncData*)pData)->uFileCsn = ((FS_STATUS_FILE_HEAD*)m_pBuffer)->uFileCsn;
    ((SSaveSyncData*)pData)->uPID     = ((FS_STATUS_FILE_HEAD*)m_pBuffer)->uLastPID;
    ((SSaveSyncData*)pData)->uBillCsn = 0;

    return m_nSyncDataLen;
}

/********************************************************
* ����: CSaveInfoFile::PutSyncData
* ����: д��״̬���ݲ���
* ����: pData : ���뻺������ָ��
*       nLen  : ���뻺�����ĳ���
* ���:
* ����: int ��õ�ʵ��д������ݳ���
********************************************************/
int CSaveInfoFile::PutSyncData(void* pData, int nLen)
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

    ((FS_STATUS_FILE_HEAD*)m_pBuffer)->uFileCsn = 
        ((SSaveSyncData*)pData)->uFileCsn;

    return m_nSyncDataLen;
}

/********************************************************
* ����: CSaveInfoFile::Flush
* ����: ���������е�����д��״̬�ļ���ֻ�б�ͬ���������
* ����:
* ���:
* ����: int д״̬�ļ��Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CSaveInfoFile::Flush()
{
    if(m_InfoFile.OpenInfoFile(TRUE, (BYTE*)m_pBuffer))
    {
        //��m_pBuffer������д��״̬�ļ�
        if((!m_InfoFile.WriteHead((BYTE*)m_pBuffer, m_nBufferLen)) ||
           (!m_InfoFile.Flush()))
        {
            return ERR_FAIL;
        }
    }

    return ERR_SUCCESS;
}